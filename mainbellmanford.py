from bellmanford import Graph

g = Graph(5)
g.addEdge(0, 1, 2)
g.addEdge(0, 2, 4)
g.addEdge(1, 2, 3)
g.addEdge(1, 3, 2)
g.addEdge(1, 4, 2)
g.addEdge(3, 2, 5)
g.addEdge(3, 1, 1)
g.addEdge(4, 3, 3)

# Print the solution
g.BellmanFord(0)
