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

print "\n#----------------Logic to find zero netted branchs start here---------------------#"

paths_file = np.genfromtxt('graphInfoSixth.txt', dtype=None)
print paths_file, "\n"

for addrs in addressj:
	paths_array = []
	print "\nPath asociated to:", addrs, "\n"
	for i in range(len(paths_file)):
		if addrs in str(paths_file[:][i]):
			paths_vector = []
			paths_vector.extend([paths_file[i][3], paths_file[i][6], paths_file[i][0]])
			paths_array.append(paths_vector)			
	print paths_array

for addrs in sources_vector:
	for i in range(len(paths_file)):
		if addrs in str(paths_file[:][i]):
			print paths_file[:][i]

print "\n#----------------Saving plot---------------------#"

plt.savefig("graphSimulation.png")
# pylab.show()