#ifndef OPERATORS_ALGO_CLEARING_H
#define OPERATORS_ALGO_CLEARING_H

struct Edge
{
    int src, dest, weight;
};
 
struct Graph
{
    int V, E;
    struct Edge *edge;
};
 
void printDistances(int dist[], int n);

void BellmanFord(struct Graph *graph, int src);

struct Graph *createGraph(int V, int E);

float** read_file(std::string filename,int rows,int cols);

#endif