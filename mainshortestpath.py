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
#------------------------------------------------------------#
pos = nx.nx_pydot.graphviz_layout(G)
points=np.array([pos[v] for v in sorted(G)])

print "Abscissa axis:\n", points[:,0]
print "Ordinate axis:\n", points[:,1]
print "Grid for the grap:\n", points
print "Number of rows:\n", len(points[:,1])
print "Number of cols:\n", len(points[:,0])

nx.draw(G, pos, **options)

print "#----------------Testing Shortest Paths---------------------#"

infoOmni = np.genfromtxt('graphInfoFifth.txt', dtype=None)

DG = Graph(list(G.nodes))

for i in xrange(len(infoOmni)):
	DG.addEdge(infoOmni[i][0], infoOmni[i][3], infoOmni[i][6])

DG.shortestPath(infoOmni[0][0])

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
	print addressj[j]," :\t", status_by_addrs[addressj[j]]

print "#----------------Looking sources for the shortest paths---------------------#"

addrs_srcs = []
status_byaddress = ""
sources_vector = []

for addrs, status in status_by_addrs.iteritems():

	print "\n#----------------Checking if the address could be a source---------------------#\n"
	addrs_srcs = addrs,':'.join(status)
	status_byaddress = addrs_srcs[1]

	status_vector = []
	for j in status_byaddress.split(":"):
		status_vector.append(j)

	print "Status vector for address ", "'", addrs, "'", ":\n", status_vector, "\n"
	if status_vector[0] == "OpenLongPosition" or status_vector[0] == "OpenShortPosition":
		print "Initial status for the address", "'", addrs, "':"
		print status_vector[0]

	status_vectorl = ''.join(status_vector)
	print "\nConclusion:"
	if "Netted" in status_vectorl:
		sources_vector.append(addrs)

		print "This address could be a source!\n"
		print "Netted status after open a position:"
		for i in xrange(1, len(status_vector)):
			if "Netted" in status_vector[i]:
				print status_vector[i]
	else:
		print "This address can't be a source!"
	
print "\n#----------------Sources for the shortest paths found---------------------#"
print "\nSource vector:\n", sources_vector
print "\nNumber of sources:\t", len(sources_vector) 
print "Number of nodes:\t",  G.number_of_nodes()

print "\n#----------------Logic to find zero netted branchs start here---------------------#"


print "\n#----------------Saving plot---------------------#"
plt.savefig("graphSimulation.png")
# pylab.show()