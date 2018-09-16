#ifndef OPERATORS_ALGO_CLEARING_H
#define OPERATORS_ALGO_CLEARING_H

#include "tradelayer_matrices.h"

///////////////////////////////////////////////////////////////////////
/** Structures for clearing algo */

struct Edge
{
    int src, dest, weight;
};
 
struct Graph
{
    int V, E;
    struct Edge *edge;
};

struct status_amounts
{
	std::string addrs_src, status_src, addrs_trk, status_trk;
	long int lives_src, lives_trk, amount_trd, matched_price; 
};

///////////////////////////////////////////////////////////////////////
/** Functions for clearing algo */

void printDistances(int dist[], int n);

void BellmanFord(struct Graph *graph, int src);

struct Graph *createGraph(int V, int E);

float** read_file(std::string filename,int rows,int cols);

struct status_amounts *get_status_amounts_long(VectorTL<std::string> &v);

void printing_matrix(MatrixTL<std::string> &gdata);

#endif