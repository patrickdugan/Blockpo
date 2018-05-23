#! /usr/bin/python

from collections import OrderedDict
from collections import defaultdict
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import requests
import pylab
import os, sys
import itertools
import json

from networkx.utils import not_implemented_for, arbitrary_element
from functools import partial
from itertools import chain
from numpy import array

from shortestpath import Graph

print "#----------------Graph Simulation---------------------#"

addrs = np.genfromtxt('graphInfoFourth.txt', dtype=None)
rows = np.size(addrs, 0)

G = nx.DiGraph()
for i in range(rows):
	G.add_edges_from( [( addrs[i][0], addrs[i][2] )], len = addrs[i][1] )

print "---------------------------------------"
print "List of Nodes: \n", list(G.nodes) 
print "---------------------------------------"
print "List of Edges: \n", list(G.edges) 
print "---------------------------------------"
print "Number of Nodes: ",  G.number_of_nodes()
print "---------------------------------------"
print "Number of Edges: ", G.number_of_edges() 
print "---------------------------------------"

options = {
 'with_labels': False,
 'node_color': 'red',
 'node_size': 20,
 'linewidths': 0.5,
 'width': 0.5,
}

###################################################################

pos = nx.nx_pydot.graphviz_layout(G)
points=np.array([pos[v] for v in sorted(G)])
nx.draw(G, pos, **options)

info_omni = np.genfromtxt('graphInfoSixth.txt', dtype=None)

DG = Graph(list(G.nodes))

for i in xrange(len(info_omni)):
	DG.addEdge(info_omni[i][0], info_omni[i][3], info_omni[i][6])

print "#----------------Testing Shortest Paths from:", info_omni[0][0], " ---------------------#"
DG.shortestPath(info_omni[0][0])

###################################################################
print "#----------------Status vector by address---------------------#"

status_map = np.genfromtxt('graphInfoFirst.txt', dtype=None)
pairs = np.column_stack((status_map[:, 0], status_map[:, 1]))
status_by_addrs = defaultdict(list)

for addrs, status in pairs:
	status_by_addrs[addrs].append(status)

os.remove("statusByAddress.txt")
for addrs, status in status_by_addrs.iteritems():
	with open('statusByAddress.txt', 'a') as f:	
		print >> f, addrs,' '.join(status)

addressj = np.genfromtxt('graphInfoAddresses.txt', dtype=None)
for j in range(len(addressj)):
	print addressj[j],":\t", status_by_addrs[addressj[j]]

###################################################################
addrs_srcs = []
status_byaddress = ""
sources_vector = []
array_status_byaddress = []

for addrs, status in status_by_addrs.iteritems():

	addrs_srcs = addrs,':'.join(status)
	status_byaddress = addrs_srcs[1] 

	status_vector = []
	for j in status_byaddress.split(":"):
		status_vector.append(j)

	status_vectorl = ''.join(status_vector)

	print "------------------------------------------------"
	print "Address:", addrs, "\n"
	if ( "OpenShortPosition" in status_vectorl ) or ( "ShortPosIncreased" in status_vectorl ):
		sources_vector.append(addrs)

		print "Conclusion:"
		print "This address can be a source!!\n"
		print "Status vector for ", addrs, ":"
		print status_vector

	else:
		print "Conclusion:"
		print "This address can not be a source!!\n"
		print "Status vector for ", addrs, ":"
		print status_vector, "\n"

	print "\n", addrs,": ", status_vector

print "\nNumber of sources:\t", len(sources_vector) 
print "Number of nodes:\t",  G.number_of_nodes()
print "Sources vector:\t", sources_vector

##################################################################

print "\n#----------------Logic to find zero netted branchs start here---------------------#\n"

paths_file = np.genfromtxt('graphInfoSixth.txt', dtype=None)
print paths_file, "\n"

# Covering the graph by following the trades for every addresses in the order that were saved at the database

for addrs in addressj:
	paths_array = []
	# print "\nPath asociated to:", addrs, "\n"
	for i in range(len(paths_file)):
		if addrs in str(paths_file[:][i]):
			paths_vector = []
			paths_vector.extend([paths_file[i][3], paths_file[i][6], paths_file[i][0]])
			paths_array.append(paths_vector)			
	# print paths_array

##################################################################

print "\n#------------------------'Definitions'---------------------------------#\n"			
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\topened or keep open contracts at the end date"

M_file = np.genfromtxt('graphInfoSixth.txt', dtype=None)
M_file = array(M_file)

idx_i = [0]
Interval = range(len(M_file))

for j in Interval:	

	if j > 0:
		print "Rows to delete: ", idx_i
		M_file = np.delete(M_file, idx_i, 0)
		
	print "M_file:\n", M_file, "\nLength M_file: ", len(M_file), "\n"

	single_path = []

	if len(M_file) == 1:
		print "#---------------------------------------------------------------------------------#\n"
		print "Source: ", M_file[0][3], "; Tracked: ", M_file[0][0], "\n"	
		single_path.append(M_file[0][0])
		single_path.append(M_file[0][6])
		single_path.append(M_file[0][3])
		print "Single path:\n", single_path, "\n"
		print "Length M_file: ", 0
		break

	M_filej = []
	M_filej = M_file[:][0]

	addrs_trk  = M_filej[0] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[3]
	status_trk = M_filej[1] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[4] 
	lives_trk  = M_filej[2] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[5]
	addrs_src  = M_filej[3] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[0]
	status_src = M_filej[4] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[1]
	lives_src  = M_filej[5] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[2]
	amount_trd = M_filej[6] 

	path_complex_one = []
	path_complex_two = []
	contracts_opened = []
	R_partly = []
	L_partly = []

	if status_src == "OpenShortPosition" or status_src == "ShortPosIncreased":
	
		if status_trk == "ShortPosNetted" or status_trk == "ShortPosNettedPartly":
			
			print "#---------------------------------------------------------------------------------#\n"
			print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			print "Single path:\n", single_path, "\n"			
			idx_i = [0]

		elif status_trk == "OpenLongPosition" or status_trk == "LongPosIncreased":

			print "#---------------------------------------------------------------------------------#\n"
			print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			path_complex_one.append(single_path)
			path_complex_two.append(single_path)

			Lives_amountsj = []
			Lives_amountsj.append(lives_src)
			Lives_amountsj.append(lives_trk)
			Lives_amountsj.append(amount_trd)		
			L_partly.append(Lives_amountsj)

			idx_iter = 0
			Long_pos_incr = []
			status_addrs_trk_v = []

			amount_trd_sum = 0
			for i in xrange(1, len(M_file)):				

				N_filei = []
				N_filei = M_file[:][i]

				status_trki = N_filei[1] if addrs_trk == N_filei[0] else N_filei[4]
				addrs_srci  = N_filei[3] if addrs_trk == N_filei[0] else N_filei[0]
				lives_trki  = N_filei[2] if addrs_trk == N_filei[0] else N_filei[5]
				lives_srci  = N_filei[2] if addrs_src == N_filei[0] else N_filei[5]
				amount_trdi = N_filei[6]

				if addrs_trk in str(N_filei):
					idx_iter += 1
					status_addrs_trk_v.append(status_trki)

				path_long_pos_incr = []
				path_long_pos_incr.append(N_filei[0])
				path_long_pos_incr.append(N_filei[6])
				path_long_pos_incr.append(N_filei[3])
				Long_pos_incr.append(path_long_pos_incr)

				if addrs_trk in str(N_filei) and status_trki == "LongPosNetted":

					path_complex_ele_one = []
					path_complex_ele_one.append(N_filei[0])
					path_complex_ele_one.append(N_filei[6])
					path_complex_ele_one.append(N_filei[3])

					path_complex_one.append(path_complex_ele_one) 

					print "Complex path:\n", path_complex_one

					idx_i.append(i)

				elif addrs_trk in str(N_filei) and status_trki == "LongPosNettedPartly":

					amount_trd_sum = amount_trd_sum + N_filei[6]

					if amount_trd_sum <= amount_trd:

						path_complex_ele_two = []
						path_complex_ele_two.append(N_filei[0])
						path_complex_ele_two.append(N_filei[6])
						path_complex_ele_two.append(N_filei[3])

						R_partly.append(path_complex_ele_two)

						Lives_amountsi = []
						Lives_amountsi.append(lives_srci)
						Lives_amountsi.append(lives_trki)
						Lives_amountsi.append(amount_trdi)

						L_partly.append(Lives_amountsi)

						idx_i.append(i)

						print "Opened contrats: ", amount_trd, " >= Total contracts traded: ", amount_trd_sum
					else:
						print "Opened contrats: ", amount_trd, " < Total contracts traded: ", amount_trd_sum
						break

			if "Netted" not in str(status_addrs_trk_v) or len(status_addrs_trk_v) == 0:

				print "Single path: ", single_path
				idx_i = [0]

			# if len(R_partly) > 1:

			# 	A_partly = str(R_partly[-1:])[2:-2]
			# 	A_partly = A_partly.replace('\'', '')
			# 	A_partly = A_partly.split(", ")

			# 	path_complex_ele_two = []
			# 	for val in range(len(A_partly)):
			# 		path_complex_ele_two.append(int(A_partly[val])) if val == 1 else path_complex_ele_two.append(A_partly[val])
		
			# 	path_complex_two.append(path_complex_ele_two)

			# 	contracts_opened.append(A_partly[0])
			# 	contracts_opened.append(abs(L_partly[0][-1]-L_partly[1][-1]))
			# 	contracts_opened.append(A_partly[-1])

			# 	print "Complex path:\n",  path_complex_two, "\n"
			# 	print "Contrats opened:\n", contracts_opened, "\n"
			# 	print "[lives_src, lives_trk, amount_trd]:\n",  L_partly, "\n"
			# 	print "Opened contrats: ", amount_trd, " > Total contracts traded: ", amount_trd_sum

			# elif len(R_partly) == 1:

			# 	A_partly = str(R_partly)[2:-2]
			# 	A_partly = A_partly.replace('\'', '')
			# 	A_partly = A_partly.split(", ")

			# 	path_complex_ele_two = []
			# 	for val in range(len(A_partly)):
			# 		path_complex_ele_two.append(int(A_partly[val])) if val == 1 else path_complex_ele_two.append(A_partly[val])
		
			# 	path_complex_two.append(path_complex_ele_two)

			# 	contracts_opened.append(A_partly[0])
			# 	contracts_opened.append(abs(L_partly[0][-1]-L_partly[1][-1]))
			# 	contracts_opened.append(A_partly[-1])

			# 	print "Complex path:\n",  path_complex_two, "\n"
			# 	print "Contrats opened:\n", contracts_opened, "\n"
			# 	print "[lives_src, lives_trk, amount_trd]:\n",  L_partly, "\n"
			# 	print "Opened contrats: ", amount_trd, " > Total contracts traded: ", amount_trd_sum
	
print "#---------------------------------------------------------------------------------#\n"

Json_file = np.genfromtxt('graphInfoAddresses.txt', dtype=None)

Keys = ['id','label'] 
Values = [] 
Dict = dict(zip(Keys, Values)) 
Data = []

x=0
while x < len(Json_file):
    x += 1 
    MoreValues = [Json_file[x-1],Json_file[x-1]] 
    Dict = dict(zip(Keys, MoreValues))
    Data.append(Dict)

# print Data, "\n"

#############################################
M = np.genfromtxt('graphInfoFourth.txt', dtype=None)

KK = ['from','to', 'label', 'arrows'] 
VV = [] 
DDIC = dict(zip(KK, VV))
DDA2 = []

for k in range(len(M)):
    X = [M[k][0], M[k][2], M[k][1], '{middle:{scaleFactor:0.5},from:true}']
    DDIC = dict(zip(KK, X))
    DDA2.append(DDIC)

# print DDA2

##########################################################
plt.savefig("graphSimulation.png")
# pylab.show()