#! /usr/bin/python

from itertools import product
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import requests
import pylab

from networkx.utils import not_implemented_for, arbitrary_element
from functools import partial
from itertools import chain

print "/////////////////////////////////////////////////////Ejemplo #1: Uso de 'defaultdict(list)' and 'join'//////////"
from collections import defaultdict

city_list = [('TX','Austin'), ('TX','Houston'), ('NY','Albany'), ('NY', 'Syracuse'), ('NY', 'Buffalo'), ('NY', 'Rochester'), ('TX', 'Dallas'), ('CA','Sacramento'), ('CA', 'Palo Alto'), ('GA', 'Atlanta')]

cities_by_state = defaultdict(list)

for state, city in city_list:
	cities_by_state[state].append(city)

for state, cities in cities_by_state.iteritems():
	print state, ', '.join(cities)

print "/////////////////////////////////////////////////////Ejemplo #2: Uso de 'insert', 'append' an 'pop':"
#########################################################
# How to use "insert" and "append"
a = ['x', 'y', 'z']
print a  
a.insert(0, 'Start')
print a  
a.insert(len(a), 'End')
print a
print "--------------------------------------------------"
print a
print a.pop()
print a
print "--------------------------------------------------"
b = [1, 'x', 2]  
b.append('y')
print(b)  
b.append(3)
print(b)  

print "/////////////////////////////////////////////////////Ejemplo #3: Position in the graph:"

DG=nx.DiGraph()
DG.add_weighted_edges_from([("a", "b", 1)])
pos = nx.spring_layout(DG)
points=np.array([pos[v] for v in sorted(DG)])

print "Positions in the graph:\t", pos
print "Abscissa axis:\t", points[:,0]
print "Ordinate axis:\t", points[:,1]

# nx.draw(DG, pos, node_size=100)
# pylab.show()
print "/////////////////////////////////////////////////////Ejemplo #4: How to use column_stack:"

a = np.array((1,2,3))
b = np.array((2,3,4))
c = np.column_stack((a,b))
print c

with open('statusbyaddrres.txt', 'w') as f:
    print >> f, c 

print "//////////////////////////////////////////////////////////////////////////////////////////"

x = np.array([1, 2, 3])
grid = np.array([[9, 8, 7],
                 [6, 5, 4]])

M = np.vstack([grid, x])
print M 
print "//////////////////////////////////////////////////////////////////////////////////////////"