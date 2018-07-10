from shortestpath import*

g = Graph(['a', 'b', 'c', 'd', 'e'])

print "\nAddresses:\t", ['a', 'b', 'c', 'd', 'e'], "\n"

g.addEdge('a', 'b', 2)
g.addEdge('a', 'c', 4)
g.addEdge('b', 'c', 3)
g.addEdge('b', 'd', 2)
g.addEdge('b', 'e', 2)
g.addEdge('d', 'c', 5)
g.addEdge('d', 'b', 1)
g.addEdge('e', 'd', 3)

g.shortestPath('a')