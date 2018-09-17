#include "operators_algo_clearing.h"
#include <bits/stdc++.h>
#include "tradelayer_matrices.h"
#include "externfns.h"

/**************************************************************/
/** Structures for clearing algo */

struct Graph *createGraph(int V, int E)
{
    struct Graph *graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];

    return graph;
}

/**************************************************************/
/** Functions for clearing algo */

void BellmanFord(struct Graph* graph, int src)
{
    int V = graph->V;
    int E = graph->E;
    int dist[V];
 
    /** Step 1: Initialize distances from src to all other vertices as INFINITE */
    for (int i = 0; i < V; i++) {
        dist[i]   = INT_MAX;
        dist[src] = 0;
    }
 
    /** Step 2: Relax all edges |V| - 1 times. A simple shortest */
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
 
    /** Step 3: Check for negative-weight cycles */
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
    for (int i = 0; i < n; ++i) printf("%d      %d\n", i, dist[i]);
}

struct status_amounts *get_status_amounts_open_incr(VectorTL &v, int q)
{
    struct status_amounts *pt_status = new status_amounts;   
    VectorTL status_z(2);
    VectorTL status_q = status_open_incr(status_z, q);

    if ( finding(v[1], status_q) )
    {
        pt_status->addrs_trk  = v[0];
        pt_status->status_trk = v[1];
        pt_status->lives_trk  = stol(v[2].c_str()); 
        pt_status->addrs_src  = v[3];
        pt_status->status_src = v[4];
        pt_status->lives_src  = stol(v[5].c_str());
    } else {
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

struct status_amounts *get_status_amounts_byaddrs(VectorTL &v, std::string addrs)
{
    struct status_amounts *pt_status = new status_amounts;   

    if ( addrs == v[0] )
    {
        pt_status->addrs_trk  = v[0];
        pt_status->status_trk = v[1];
        pt_status->lives_trk  = stol(v[2].c_str()); 
        pt_status->addrs_src  = v[3];
        pt_status->status_src = v[4];
        pt_status->lives_src  = stol(v[5].c_str());
    } else {
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

VectorTL status_open_incr(VectorTL &status_q, int q)
{
    extern VectorTL *pt_open_incr_long; VectorTL &open_incr_long = *pt_open_incr_long;
    extern VectorTL *pt_open_incr_short; VectorTL &open_incr_short = *pt_open_incr_short;

    if ( q == 0 ) {
        status_q[0] = open_incr_long[0];
        status_q[1] = open_incr_long[1];
    } else {
        status_q[0] = open_incr_short[0];
        status_q[1] = open_incr_short[1];
    }
    return status_q;
}

void clearing_operator_fifo(VectorTL &vdata, MatrixTL &databe, int index_init, struct status_amounts *pt_pos)
{
    printf("\n\nEdge Source: Row #%d\n", index_init);
    printing_vector(vdata);

    int rows_databe = size(databe, 0);
    int cols_databe = size(databe, 1);

    int opened_contracts = pt_pos->amount_trd;
    std::string addrs_opening = pt_pos->addrs_trk;

    for (int i = index_init+1; i < rows_databe; ++i)
    {
        VectorTL jrow_databe(cols_databe);
        sub_row(jrow_databe, databe, i);

        if ( finding(addrs_opening, jrow_databe) )
        {
            printf("\n\nRow i: %d\t Address Tracked: %s\n", i, addrs_opening.c_str());
            printing_vector(jrow_databe);
            struct status_amounts *pt_status_addrs_trk = get_status_amounts_byaddrs(jrow_databe, addrs_opening);
        }
    }
}