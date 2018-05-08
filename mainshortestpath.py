#! /usr/bin/python

from collections import defaultdict
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import requests
import pylab
import os, sys

from networkx.utils import not_implemented_for, arbitrary_element
from functools import partial
from itertools import chain

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

# Covering the graph following the trades for every addresses in the order that were saved at the database

for addrs in addressj:
	paths_array = []
	# print "\nPath asociated to:", addrs, "\n"
	for i in range(len(paths_file)):
		if addrs in str(paths_file[:][i]):
			paths_vector = []
			paths_vector.extend([paths_file[i][3], paths_file[i][6], paths_file[i][0]])
			paths_array.append(paths_vector)			
	# print paths_array

print "#---------------------------------------------------------------------------------#"

##################################################################

for addrs in sources_vector:

	for i in range(len(paths_file)):

		vec_pathsl = []
		vec_pathsl = paths_file[:][i]	

		open_shortl = ( addrs == vec_pathsl[0] and vec_pathsl[1] == "OpenShortPosition" )
		open_shortr = ( addrs == vec_pathsl[3] and vec_pathsl[4] == "OpenShortPosition" )
		incr_shortl = ( addrs == vec_pathsl[0] and vec_pathsl[1] == "ShortPosIncreased" )
		incr_shortr = ( addrs == vec_pathsl[3] and vec_pathsl[4] == "ShortPosIncreased" )

		if ( open_shortl or open_shortr ) or ( incr_shortl or incr_shortl ):
	
			addrs_tracked = ""
			if addrs == vec_pathsl[0]:
				addrs_tracked = vec_pathsl[3]
			else:
				addrs_tracked = vec_pathsl[0]
			print "\n", addrs, "is opening short pos of", vec_pathsl[6], "contracts with", addrs_tracked, ":\n"	

			array_longer_path = []

			for j in xrange(i, len(paths_file)):

				vec_pathsr = []
				vec_pathsr = paths_file[:][j]
				path_single = []

				if ( addrs_tracked == vec_pathsr[0] and "ShortPosNettedPartly" in str(vec_pathsr[1]) ) or ( addrs_tracked == vec_pathsr[3] and "ShortPosNettedPartly" in str(vec_pathsr[4]) ):

					path_single.append(vec_pathsr[0])
					path_single.append(vec_pathsr[6])
					path_single.append(vec_pathsr[3])
					print "Path Simple: ", path_single, "\n"

					# array_longer_path.append(path_single)

				elif ( addrs_tracked == vec_pathsr[0] and "ShortPosNetted" in str(vec_pathsr[1]) ) or ( addrs_tracked == vec_pathsr[3] and "ShortPosNetted" in str(vec_pathsr[4]) ):		

					path_single.append(vec_pathsr[0])
					path_single.append(vec_pathsr[6])
					path_single.append(vec_pathsr[3])
					print "Path Simple: ", path_single, "\n"

					# array_longer_path.append(path_single)

				elif ( addrs_tracked == vec_pathsr[0] and "OpenLongPosition" in str(vec_pathsr[1]) ) or ( addrs_tracked == vec_pathsr[3] and "OpenLongPosition" in str(vec_pathsr[4]) ):

					path_single.append(vec_pathsr[0])
					path_single.append(vec_pathsr[6])
					path_single.append(vec_pathsr[3])

					array_longer_path.append(path_single)

				elif ( addrs_tracked == vec_pathsr[0] and "LongPosIncreased" in str(vec_pathsr[1]) ) or ( addrs_tracked == vec_pathsr[3] and "LongPosIncreased" in str(vec_pathsr[4]) ):

					path_single.append(vec_pathsr[0])
					path_single.append(vec_pathsr[6])
					path_single.append(vec_pathsr[3])

					array_longer_path.append(path_single)	

			print "Path Complex: ", array_longer_path, "\n"

print "\n#------------------------'Definitions'---------------------------------#\n"			
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\topened or keep open contracts at the end date"
print "\n#---------------------------------------------------------------------------------#\n"

M_file = np.genfromtxt('graphInfoSixth.txt', dtype=None)

for j in range(len(M_file)):

	M_filej = []
	M_filej = M_file[:][j]

	if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased":

		addrs_src  = M_filej[3]
		status_src = M_filej[4] 
		addrs_trk  = M_filej[0]
		status_trk = M_filej[1]
		amount_trd = M_filej[6]

		if status_trk == "ShortPosNetted":
	
			print "Source: ", addrs_src, "\n"
			single_path = []

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			print "Single path:\n", single_path, "\n"

		if status_trk == "OpenLongPosition":
			
			print "Source: ", addrs_src, "\n"
			print "Complex path:"
			path_complex = []

			for i in xrange(j-1, len(M_file)):
					
				M_filei = []
				M_filei = M_file[:][i]

				if addrs_trk in str(M_filei) and "Netted" in str(M_filei):

					path_complex_ele = []

					path_complex_ele.append(M_filei[0])
					path_complex_ele.append(M_filei[6])
					path_complex_ele.append(M_filei[3])

					path_complex.append(path_complex_ele)

				if addrs_trk in str(M_filei) and "" in str(M_filei):

					path_complex_ele = []

					path_complex_ele.append(M_filei[0])
					path_complex_ele.append(M_filei[6])
					path_complex_ele.append(M_filei[3])

					path_complex.append(path_complex_ele)

			print path_complex

			print "#---------------------------------------------------------------------------------#"
	
	# if M_filej[1] == "OpenShortPosition" or M_filej[1] == "ShortPosIncreased":

	# 	addrs_src = M_filej[0]
	# 	addrs_trk = M_filej[3]

	# 	single_path = []
	# 	single_path.append(addrs_src)
	# 	single_path.append(M_filej[6])
	# 	single_path.append(addrs_trk)

	# 	print single_path


# 	if "OpenShortPosition" in str(M_filej) or "ShortPosIncreased" in str(M_filej):

# 		addrs_source  = ""
# 		addrs_tracked = ""

# 		if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased":

# 			addrs_source  = M_filej[3]
# 			addrs_tracked = M_filej[0]
# 		else:
# 			addrs_source  = M_filej[0]
# 			addrs_tracked = M_filej[3]

# 		# print "\n[", addrs_source, ",", M_filej[6], ",", addrs_tracked,"]\n"

# 		array_longer_path = []

# 		for j in xrange(i, len(M_file)):
			
# 			M_filer = []
# 			M_filer = M_file[:][j]
# 			path_single = []

# 			# if ( addrs_tracked == M_filer[0] and "ShortPosNettedPartly" in str(M_filer[1]) ) or ( addrs_tracked == M_filer[3] and "ShortPosNettedPartly" in str(M_filer[4]) ):

# 			# 	path_single.append(M_filer[0])
# 			# 	path_single.append(M_filer[6])
# 			# 	path_single.append(M_filer[3])

# 			if ( addrs_tracked == M_filer[0] and "ShortPosNetted" in str(M_filer[1]) ) or ( addrs_tracked == M_filer[3] and "ShortPosNetted" in str(M_filer[4]) ):

# 				path_single.append(M_filer[0])
# 				path_single.append(M_filer[6])
# 				path_single.append(M_filer[3])

# 				print "Simple path:\t", path_single

# 			if ( addrs_tracked == M_filer[0] and "OpenLongPosition" in str(M_filer[1]) ) or ( addrs_tracked == M_filer[3] and "OpenLongPosition" in str(M_filer[4]) ):

# 				path_single.append(M_filer[0])
# 				path_single.append(M_filer[6])
# 				path_single.append(M_filer[3])
# 				array_longer_path.append(path_single)

# 				print "Complex path:\t", array_longer_path, "\n"
			
# 			# if ( addrs_tracked == M_filer[0] and "LongPosIncreased" in str(M_filer[1]) ) or ( addrs_tracked == M_filer[3] and "LongPosIncreased" in str(M_filer[4]) ):

# 			# 	path_single.append(M_filer[0])
# 			# 	path_single.append(M_filer[6])
# 			# 	path_single.append(M_filer[3])

# 		print "#---------------------------------------------------------------------------------#\n"


##########################################################
plt.savefig("graphSimulation.png")
# pylab.show()