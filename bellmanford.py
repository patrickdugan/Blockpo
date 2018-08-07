from collections import defaultdict


class Graph:

    def __init__(self, vertices):

        self.V = vertices
        self.graph = []

    def addEdge(self, u, v, w):

        self.graph.append([u, v, w])

    def printArr(self, dist):

        for i in range(self.V):
            print "Distance from source\t0\tto the vertex\t", i, ":\t\t", dist[i]

    def BellmanFord(self, src):

        dist = [float("Inf")]*self.V
        dist[src] = 0

        for i in range(self.V-1):
            for u, v, w in self.graph:
                if dist[u] != float("Inf") and dist[u] + w < dist[v]:
                    dist[v] = dist[u] + w

        for u, v, w in self.graph:
            if dist[u] != float("Inf") and dist[u] + w < dist[v]:
                print "Graph contains negative weight cycle"
                return

        self.printArr(dist)
