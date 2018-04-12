from collections import defaultdict
 
class Graph:
    
    def __init__(self, vertices):
 
        self.v = vertices 
        self.graph = defaultdict(list)
 
    def addEdge(self, u, v, w):
    
        self.graph[u].append((v, w))

    def topologicalSortUtil(self, v, visited, stack):
 
        visited[v] = True
 
        if v in self.graph.keys():
            for node, weight in self.graph[v]:
                if visited[node] == False:
                    self.topologicalSortUtil(node, visited, stack)

        stack.append(v)
 
    def shortestPath(self, s):

        visited = {}

        for i in self.v:
            visited[i] = False

        stack =[]
 
        for i in self.v:
            if visited[i] == False:
                self.topologicalSortUtil(s, visited, stack)

        dist = {}

        for i in self.v:
            dist[i] = float("Inf")

        dist[s] = 0
  
        while stack:
 
            i = stack.pop() 

            for node, weight in self.graph[i]:
                if  dist[node] > dist[i] + weight:
                    dist[node] = dist[i] + weight
 
        for i in self.v:
            print ("%d" %dist[i]) if dist[i] != float("Inf") else  "Inf"