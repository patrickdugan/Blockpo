#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <cmath>
#include "operators_algo_clearing.h"
#include <bits/stdc++.h>
#include "tradelayer_matrices.h"

///////////////////////////////////////////////////////////////////////
/** Structures for clearing algo */

struct Graph *createGraph(int V, int E)
{
    struct Graph *graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];

    return graph;
}

struct status_amounts *get_status_amounts_long(VectorTL<std::string> &v)
{
    struct status_amounts *pt_status = new status_amounts;

    if ( v[1] == "OpenLongPosition" || v[1] == "LongPosIncreased" )
    {
        pt_status->addrs_trk  = v[0];
        pt_status->status_trk = v[1];
        pt_status->lives_trk  = stol(v[2].c_str()); 
        pt_status->addrs_src  = v[3];
        pt_status->status_src = v[4];
        pt_status->lives_src  = stol(v[5].c_str());
    } else 
    {
        pt_status->addrs_src  = v[0];
        pt_status->status_src = v[1];
        pt_status->lives_src  = stol(v[2].c_str()); 
        pt_status->addrs_trk  = v[3];
        pt_status->status_trk = v[4];
        pt_status->lives_trk  = stol(v[5].c_str());
    }
    
    pt_status->amount_trd    = stol(v[6].c_str());      
    pt_status->matched_price = stol(v[7].c_str());  
    
    return pt_status;   
}

///////////////////////////////////////////////////////////////////////
/** Functions for clearing algo */

void BellmanFord(struct Graph* graph, int src)
{
    int V = graph->V;
    int E = graph->E;
    int dist[V];
 
    // Step 1: Initialize distances from src to all other vertices as INFINITE
    for (int i = 0; i < V; i++) {
        dist[i]   = INT_MAX;
        dist[src] = 0;
    }
 
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

    for(int i = 0; i < cols; ++i) { 
        floats[i] = new float[rows+1]; 
    }

    for(int i =0; i < rows ;++i) {
        for(int j =0; j < cols ; ++j) { 
                file >>floats[j][i]; 
        }
    }
    file.close();

    return floats;
}

void printing_matrix(MatrixTL<std::string> &gdata)
{
    int n_rows = size(gdata, 0);
    int n_cols = size(gdata, 1);

    for (int i = 0; i < n_rows; ++i) 
    {
        for (int j = 0; j < n_cols; ++j) 
        {
            std::cout << gdata[i][j] << "\t";
        }
        std::cout << "\n";
    }
}