#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <type_traits>
#include "bellmanford.h"

using namespace std;

int main()
{
    /*----------------------------------------------------*/
    string cola, colc; 
    double colb; 
    int numItems = 0, n_cols = 3;
    /*----------------------------------------------------*/
    ifstream file;
    file.open("graphInfoFourth.txt");  
    if ( file.is_open() ) {
        while ( !file.eof() ) {
            file >> cola >> colb >> colc;
            ++numItems;
        }
    }
    int n_rows = numItems;
    file.close();
    /*----------------------------------------------------*/
    string **gdata = new string*[n_cols+1];
    for (int i = 0; i < n_cols; ++i)
    {
        gdata[i] = new string[n_rows];
    }

    std::fstream fileg;
    fileg.open("graphInfoFourth.txt", std::ios::in);
    for (int i = 0; i < n_rows; ++i)
    {
        for (int j = 0; j < n_cols; ++j)
        {
            fileg >> gdata[j][i];
        }
    }
    fileg.close();
    /*----------------------------------------------------*/
    for (int i = 0; i < n_rows; ++i)
    {
        for (int j = 0; j < n_cols; ++j)
        {
            std::cout << gdata[j][i] << "\t";
        }
        std::cout << "\n";
    }
    printf("Number of rows: %d,\t Number of cols: %d\n", n_rows, n_cols);
    /*----------------------------------------------------*/

    /* Let us create the graph given in above example */
    int V = 5;  // Number of vertices in graph
    int E = 8;  // Number of edges in graph
    struct Graph *graph = createGraph(V, E);
 
    // add edge 0-1 (or A-B in above figure)
    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;
    graph->edge[0].weight = -1;
 
    // add edge 0-2 (or A-C in above figure)
    graph->edge[1].src = 0;
    graph->edge[1].dest = 2;
    graph->edge[1].weight = 4;
 
    // add edge 1-2 (or B-C in above figure)
    graph->edge[2].src = 1;
    graph->edge[2].dest = 2;
    graph->edge[2].weight = 3;
 
    // add edge 1-3 (or B-D in above figure)
    graph->edge[3].src = 1;
    graph->edge[3].dest = 3;
    graph->edge[3].weight = 2;
 
    // add edge 1-4 (or A-E in above figure)
    graph->edge[4].src = 1;
    graph->edge[4].dest = 4;
    graph->edge[4].weight = 2;
 
    // add edge 3-2 (or D-C in above figure)
    graph->edge[5].src = 3;
    graph->edge[5].dest = 2;
    graph->edge[5].weight = 5;
 
    // add edge 3-1 (or D-B in above figure)
    graph->edge[6].src = 3;
    graph->edge[6].dest = 1;
    graph->edge[6].weight = 1;
 
    // add edge 4-3 (or E-D in above figure)
    graph->edge[7].src = 4;
    graph->edge[7].dest = 3;
    graph->edge[7].weight = -3;
 
    BellmanFord(graph, 0);
    
    delete [] gdata;
    return 0;
}