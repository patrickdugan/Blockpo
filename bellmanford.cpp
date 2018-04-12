#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <cmath>
#include "bellmanford.h"
#include <bits/stdc++.h>

struct Graph *createGraph(int V, int E)
{
    struct Graph *graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}
 
void BellmanFord(struct Graph* graph, int src)
{
    int V = graph->V;
    int E = graph->E;
    int dist[V];
 
    // Step 1: Initialize distances from src to all other vertices as INFINITE
    for (int i = 0; i < V; i++)
        dist[i]   = INT_MAX;
        dist[src] = 0;
 
    // Step 2: Relax all edges |V| - 1 times. A simple shortest 
    for (int i = 1; i <= V-1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int w = graph->edge[j].weight;

            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }
 
    // Step 3: Check for negative-weight cycles
    for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int w = graph->edge[i].weight;

        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
            printf("Graph contains negative weight cycle");
    }
 
    printDistances(dist, V);

    return;
}

void printDistances(int dist[], int n)
{
    printf("Distances from source to vertex\n");
    for (int i = 0; i < n; ++i)
        printf("%d      %d\n", i, dist[i]);
}

float **read_file(std::string filename, int rows, int cols)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::in);

    if(!file.is_open()){ return 0; }

    float **floats = new float*[cols+1];

    for(int i = 0; i < cols; ++i) 
    { 
        floats[i] = new float[rows+1]; 
    }

    for(int i =0; i < rows ;++i)
    {
        for(int j =0; j < cols ; ++j)
            { 
                file >>floats[j][i]; 
            }
    }
    file.close();

    return floats;
}