#! /usr/bin/python

import numpy as np
import networkx as nx
from shortestpath import Graph
import matplotlib.pyplot as plt
import pylab

print("#----------------Graph Simulation---------------------#")

addrs = np.genfromtxt('data_fifo_plot_python.txt', dtype=None)
addrs2 = np.genfromtxt('data_ghost_plot_python.txt', dtype=None)
rows = np.size(addrs, 0)

G = nx.DiGraph()
for i in range(rows):
    G.add_edges_from([(addrs[i][0], addrs[i][1])], len=addrs[i][2])

G2 = nx.DiGraph()
for i in range(rows):
    G2.add_edges_from([(addrs2[i][0], addrs2[i][1])], len=addrs2[i][2])

print("---------------------------------------")
print("List of Nodes: \n", list(G.nodes))
print("---------------------------------------")
print("List of Edges: \n", list(G.edges))
print("---------------------------------------")
print("Number of Nodes: ",  G.number_of_nodes())
print("---------------------------------------")
print("Number of Edges: ", G.number_of_edges())
print("---------------------------------------")

options = {
    'with_labels': False,
    'node_color': 'red',
    'node_size': 50,
    'linewidths': 0.5,
    'width': 0.5,
}

options2 = {
    'with_labels': False,
    'node_color': 'blue',
    'node_size': 50,
    'linewidths': 0.5,
    'width': 0.5,
}
###################################################################

pos = nx.nx_pydot.graphviz_layout(G)
points = np.array([pos[v] for v in sorted(G)])
nx.draw(G, pos, **options)

pos = nx.nx_pydot.graphviz_layout(G2)
points = np.array([pos[v] for v in sorted(G2)])
nx.draw(G2, pos, **options2)


info_omni = np.genfromtxt('data_ghost_plot_python.txt', dtype=None)

DG = Graph(list(G.nodes))

for i in xrange(len(info_omni)):
    DG.addEdge(info_omni[i][0], info_omni[i][1], info_omni[i][2])

print("#----------------Testing Shortest Paths from:", info_omni[0][0], " ---------------------#")
DG.shortestPath(info_omni[0][0])

print("Number of traded nodes:\t",  G.number_of_nodes())

plt.savefig("plot_fifo_clearing_pluss_ghostcells.png")
pylab.show()
