#ifndef OPERATORS_ALGO_CLEARING_H
#define OPERATORS_ALGO_CLEARING_H

#include <map>
#include <vector>

#include "tradelayer_matrices.h"

/**************************************************************/
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
  long int lives_src, lives_trk, amount_trd, matched_price, nlives_src, nlives_trk; 
};

/**************************************************************/
/** Functions for clearing algo */

void printDistances(int dist[], int n);

void BellmanFord(struct Graph *graph, int src);

struct Graph *createGraph(int V, int E);

struct status_amounts *get_status_amounts_open_incr(VectorTL &v, int q);

struct status_amounts *get_status_amounts_byaddrs(VectorTL &v, std::string addrs);

VectorTL status_open_incr(VectorTL &status_q, int q);

VectorTL status_netted_npartly(VectorTL &status_q, int q);

void clearing_operator_fifo(VectorTL &vdata, MatrixTL &M_file, int index_init, struct status_amounts *pt_pos, int idx_long_short, int &counting_netted, long int amount_trd_sum, std::vector<std::map<std::string, std::string>> &path_main, int path_number, long int opened_contracts);

void adding_newtwocols_trdamount(MatrixTL &M_file, MatrixTL &database);

void settlement_algorithm_fifo(MatrixTL &M_file);

void counting_databaserows();

void updating_lasttwocols_fromdatabase(std::string addrs, MatrixTL &M_file, int i, long int live_updated);

void building_edge(std::map<std::string, std::string> &path_first, struct status_amounts *pt_status_addrs_trk, long int entry_price, long int lives, int index_row, int path_number, long int amount_path);

void printing_edges(std::map<std::string, std::string> &path_first);

bool find_open_incr_anypos(std::string &s, VectorTL *v);

bool find_open_incr_long(std::string &s, VectorTL *v);

#endif
