#! /usr/bin/python

import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import requests
import pylab

from networkx.utils import not_implemented_for, arbitrary_element
from functools import partial
from itertools import chain

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
# Labels for edges:
# arc_weight = nx.get_edge_attributes(G,'len')
# nx.draw_networkx_edge_labels(G, pos = nx.nx_pydot.graphviz_layout(G), font_size=3, edge_labels = arc_weight)
#------------------------------------------------------------#
# print "Number of cycles in the graph:\t", len(list(nx.simple_cycles(G)))
# print "Number of cycles in the graph:\n", list(nx.find_cycle(G, orientation='original'))
# print "Number of cycles in the graph:\t", len(list(nx.find_cycle(G, orientation='original')))
#------------------------------------------------------------#
pos = nx.nx_pydot.graphviz_layout(G)
points=np.array([pos[v] for v in sorted(G)])

print "Abscissa axis:\n", points[:,0]
print "Ordinate axis:\n", points[:,1]
print "Grid for the grap:\n", points
print "Number of rows:\n", len(points[:,1])
print "Number of cols:\n", len(points[:,0])

nx.draw(G, pos, **options)
plt.savefig("graphAddresses.png")
pylab.show()