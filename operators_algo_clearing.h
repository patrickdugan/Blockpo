#ifndef OPERATORS_ALGO_CLEARING_H
#define OPERATORS_ALGO_CLEARING_H

#include <map>
#include <unordered_set>
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
  long int lives_src, lives_trk, amount_trd, nlives_src, nlives_trk;
  double matched_price;
};


struct status_amounts_edge
{
  std::string addrs_src, status_src, addrs_trk, status_trk;
  long int lives_src, lives_trk, amount_trd, edge_row, path_number, ghost_edge;
  double entry_price, exit_price;
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

void building_edge(std::map<std::string, std::string> &path_first, std::string addrs_src, std::string addrs_trk, std::string status_src, std::string status_trk, double entry_price, double exit_price, long int lives, int index_row, int path_number, long int amount_path, int ghost_edge);

void printing_edges(std::map<std::string, std::string> &path_first);

bool find_open_incr_anypos(std::string &s, VectorTL *v);

bool find_open_incr_long(std::string &s, VectorTL *v);

void computing_lives_bypath(std::vector<std::map<std::string, std::string>> &it_path_main);

struct status_amounts_edge *get_status_byedge(std::map<std::string, std::string> &edge);

void looking_netted_events(std::string &addrs_obj, std::vector<std::map<std::string, std::string>> &it_path_main, int q, long int amount_opened, int index_src_trk);

void printing_path_maini(std::vector<std::map<std::string, std::string>> &it_path_maini);

void checking_zeronetted_bypath(std::vector<std::map<std::string, std::string>> &path_maini);

bool find_netted_npartly_anypos(std::string &s, VectorTL *v);

void computing_livesvectors_forlongshort(std::vector<std::map<std::string, std::string>> &it_path_main, std::vector<std::map<std::string, std::string>> &lives_longs, std::vector<std::map<std::string, std::string>> &lives_shorts);

void building_lives_edges(std::map<std::string, std::string> &path_first, std::string addrs, std::string status, long int lives, double entry_price, struct status_amounts_edge *pt_status_byedge);

void printing_edges_lives(std::map<std::string, std::string> &path_first);

void counting_lives_longshorts(std::vector<std::map<std::string, std::string>> &lives_longs, std::vector<std::map<std::string, std::string>> &lives_shorts);

void computing_settlement_exitprice(std::vector<std::map<std::string, std::string>> &it_path_main, long int &sum_oflives, double &PNL_total, double &gamma_p, double &gamma_q);

void calculate_pnltrk_bypath(std::vector<std::map<std::string, std::string>> &path_main, double &PNL_total, std::unordered_set<std::string> &addrs_set, std::vector<std::string> addrsv);

void listof_addresses_bypath(std::vector<std::map<std::string, std::string>> &it_path_main, std::vector<std::string> &addrsv);

double PNL_function(double entry_price, double exit_price, long int amount_trd, struct status_amounts *pt_jrow_database);

void getting_gammapq_bypath(std::vector<std::map<std::string, std::string>> &path_main, double PNL_total, double &gamma_p, double &gamma_q, std::unordered_set<std::string> addrs_set);

void calculating_ghost_edges(std::vector<std::map<std::string, std::string>> lives_longs, std::vector<std::map<std::string, std::string>> lives_shorts, double exit_price_desired, std::vector<std::map<std::string, std::string>> &ghost_edges_array);

#endif
