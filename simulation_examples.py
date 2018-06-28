#! /usr/bin/python

# from itertools import product
# import matplotlib.pyplot as plt
# import networkx as nx
# import numpy as np
# import requests
# import pylab

# from networkx.utils import not_implemented_for, arbitrary_element
# from functools import partial
# from itertools import chain

# print "/////////////////////////////////////////////////////Ejemplo #1: Uso de 'defaultdict(list)' and 'join'//////////"
# from collections import defaultdict

# city_list = [('TX','Austin'), ('TX','Houston'), ('NY','Albany'), ('NY', 'Syracuse'), ('NY', 'Buffalo'), ('NY', 'Rochester'), ('TX', 'Dallas'), ('CA','Sacramento'), ('CA', 'Palo Alto'), ('GA', 'Atlanta')]

# cities_by_state = defaultdict(list)

# for state, city in city_list:
# 	cities_by_state[state].append(city)

# for state, cities in cities_by_state.iteritems():
# 	print state, ', '.join(cities)

# print "/////////////////////////////////////////////////////Ejemplo #2: Uso de 'insert', 'append' an 'pop':"
# #########################################################
# # How to use "insert" and "append"
# a = ['x', 'y', 'z']
# print a  
# a.insert(0, 'Start')
# print a  
# a.insert(len(a), 'End')
# print a
# print "--------------------------------------------------"
# print a
# print a.pop()
# print a
# print "--------------------------------------------------"
# b = [1, 'x', 2]  
# b.append('y')
# print(b)  
# b.append(3)
# print(b)  

# print "/////////////////////////////////////////////////////Ejemplo #3: Position in the graph:"

# DG=nx.DiGraph()
# DG.add_weighted_edges_from([("a", "b", 1)])
# pos = nx.spring_layout(DG)
# points=np.array([pos[v] for v in sorted(DG)])

# print "Positions in the graph:\t", pos
# print "Abscissa axis:\t", points[:,0]
# print "Ordinate axis:\t", points[:,1]

# # nx.draw(DG, pos, node_size=100)
# # pylab.show()
# print "/////////////////////////////////////////////////////Ejemplo #4: How to use column_stack:"

# a = np.array((1,2,3))
# b = np.array((2,3,4))
# c = np.column_stack((a,b))
# print c

# with open('statusbyaddrres.txt', 'w') as f:
#     print >> f, c 

# print "LAST//////////////////////////////////////////////////////////////////////////////////////////"

# a = np.zeros((2, 2))
# print "a: ", a
# # In the following line 1 is the index before which to insert, 0 is the axis.
# b = np.insert(a, 1, np.array((1, 1)), 0)
# print "np.insert(a, 1, np.array((1, 1)), 0):\n", b   
# print "np.insert(a, 1, np.array((1, 1)), 1):\n", np.insert(a, 1, np.array((1, 1)), 1)

print "//////////////////////////////////////////////////////////////////////////////////////////"

l = [{'a': 123, 'b': 1234},
        {'a': 3222, 'b': 1234},
        {'a': 123, 'b': 1234}]

seen = set()
new_l = []
for d in l:
    t = tuple(d.items())
    if t not in seen:
        seen.add(t)
        new_l.append(d)

print new_l