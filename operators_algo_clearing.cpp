#include "operators_algo_clearing.h"
#include <bits/stdc++.h>
#include "tradelayer_matrices.h"
#include "externfns.h"

extern VectorTL *pt_open_incr_anypos;
extern VectorTL *pt_open_incr_long;

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

/**************************************************************/
/** Functions for Settlement Algorithm */

void counting_databaserows()
{
  extern int n_rows;
  std::string address1, status1, address2, status2;
  double lives1, lives2, amount_trade, matched_price;
  int rows_counter = 0;
  
  ifstream file;
  file.open("graphInfoSixth.txt");  
  if ( file.is_open() ) {
    while ( !file.eof() ) {
      file >> address1 >> status1 >> lives1 >> address2 >> status2 >> lives2 >> amount_trade >> matched_price;
      ++rows_counter;
    }
  }
  file.close();
  n_rows = rows_counter;
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
      pt_status->nlives_trk = stol(v[8].c_str());
      pt_status->nlives_src = stol(v[9].c_str());
    } else
    {
      pt_status->addrs_src  = v[0];
      pt_status->status_src = v[1];
      pt_status->lives_src  = stol(v[2].c_str()); 
      pt_status->addrs_trk  = v[3];
      pt_status->status_trk = v[4];
      pt_status->lives_trk  = stol(v[5].c_str());
      pt_status->nlives_src = stol(v[8].c_str());
      pt_status->nlives_trk = stol(v[9].c_str());
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
      pt_status->nlives_trk = stol(v[8].c_str());
      pt_status->nlives_src = stol(v[9].c_str());
    }
  else
    {
      pt_status->addrs_src  = v[0];
      pt_status->status_src = v[1];
      pt_status->lives_src  = stol(v[2].c_str()); 
      pt_status->addrs_trk  = v[3];
      pt_status->status_trk = v[4];
      pt_status->lives_trk  = stol(v[5].c_str());
      pt_status->nlives_src = stol(v[8].c_str());
      pt_status->nlives_trk = stol(v[9].c_str());
    }
  pt_status->amount_trd    = stol(v[6].c_str());      
  pt_status->matched_price = stol(v[7].c_str());
  
  return pt_status;   
}

struct status_amounts_edge *get_status_byedge(std::map<std::string, std::string> &edge)
{
  struct status_amounts_edge *pt_status = new status_amounts_edge;

  pt_status->addrs_src   = edge["addrs_src"];
  pt_status->addrs_trk   = edge["addrs_trk"];
  pt_status->status_src  = edge["status_src"];
  pt_status->status_trk  = edge["status_trk"];
  pt_status->entry_price = stol(edge["entry_price"]);
  pt_status->exit_price  = stol(edge["exit_price"]);
  pt_status->lives_src   = stol(edge["lives_src"]);
  pt_status->lives_trk   = stol(edge["lives_trk"]); 
  pt_status->amount_trd  = stol(edge["amount_trd"]);
  pt_status->edge_row    = stol(edge["edge_row"]);
  pt_status->path_number = stol(edge["path_number"]);
  pt_status->ghost_edge  = stol(edge["ghost_edge"]);
   
  return pt_status;   
}

VectorTL status_open_incr(VectorTL &status_q, int q)
{
  extern VectorTL *pt_open_incr_long; VectorTL &open_incr_long = *pt_open_incr_long;
  extern VectorTL *pt_open_incr_short; VectorTL &open_incr_short = *pt_open_incr_short;

  if ( q == 0 )
    {
      status_q[0] = open_incr_long[0];
      status_q[1] = open_incr_long[1];
    } else
    {
      status_q[0] = open_incr_short[0];
      status_q[1] = open_incr_short[1];
    }
  return status_q;
}

VectorTL status_netted_npartly(VectorTL &status_q, int q)
{
  extern VectorTL *pt_netted_npartly_long;  VectorTL &netted_npartly_long  = *pt_netted_npartly_long;
  extern VectorTL *pt_netted_npartly_short; VectorTL &netted_npartly_short = *pt_netted_npartly_short;
  
  if ( q == 0 )
    {
      status_q[0] = netted_npartly_long[0];
      status_q[1] = netted_npartly_long[1];
    }
  else
    {
      status_q[0] = netted_npartly_short[0];
      status_q[1] = netted_npartly_short[1];
    }
  return status_q;
}

void adding_newtwocols_trdamount(MatrixTL &M_file, MatrixTL &database)
{
  int n_cols =  size(database, 1);
  for (int i = 0; i < size(database, 0); ++i)
    {
      for (int j = 0; j < n_cols; ++j) M_file[i][j] = database[i][j];
      M_file[i][n_cols] = database[i][6].c_str();
      M_file[i][n_cols+1] = database[i][6].c_str();
    }
}

void settlement_algorithm_fifo(MatrixTL &M_file)
{
  extern int cols_news;
  extern int n_rows;
  extern VectorTL *pt_open_incr_long;  VectorTL &open_incr_long  = *pt_open_incr_long;
  extern VectorTL *pt_open_incr_short; VectorTL &open_incr_short = *pt_open_incr_short;
  std::vector<std::vector<std::map<std::string, std::string>>> path_main;
  std::vector<std::vector<std::map<std::string, std::string>>>::iterator it_path_main;
  std::vector<std::map<std::string, std::string>>::iterator it_path_maini;
  
  int path_number = 0;
  VectorTL vdata(cols_news);
   
  for (int i = 0; i < n_rows; ++i)
    {
      for (int j = 0; j < cols_news; ++j) vdata[j] = M_file[i][j];
      
      struct status_amounts *pt_vdata_long  = get_status_amounts_open_incr(vdata, 0);
      struct status_amounts *pt_vdata_short = get_status_amounts_open_incr(vdata, 1);
      std::vector<std::map<std::string, std::string>> path_maini;
      
      if ( finding(pt_vdata_long->status_trk, open_incr_long) && finding(pt_vdata_short->status_trk, open_incr_short) )
        {
	  path_number += 1;

	  printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	  printf("New Edge Source: Row #%d\n\n", i);
	  std::map<std::string, std::string> edge_source;
	  building_edge(edge_source, pt_vdata_long, pt_vdata_long->matched_price, 0, i, path_number, pt_vdata_long->amount_trd, 0);
	  path_maini.push_back(edge_source);
	  printing_edges(edge_source);
	  
	  int counting_netted_long = 0;
	  long int amount_trd_sum_long = 0;
	  printf("\n*************************************************");
	  printf("\nTracking Long Position:");
	  clearing_operator_fifo(vdata, M_file, i, pt_vdata_long, 0, counting_netted_long, amount_trd_sum_long, path_maini, path_number, pt_vdata_long->nlives_trk);

	  int counting_netted_short = 0;
	  long int amount_trd_sum_short = 0;
	  printf("\n*************************************************");
	  printf("\nTracking Short Position:");
	  clearing_operator_fifo(vdata, M_file, i, pt_vdata_short, 1, counting_netted_short, amount_trd_sum_short, path_maini, path_number, pt_vdata_short->nlives_trk);

	  printf("\n\nPath #%d\n\n", path_number);
	  for (std::vector<std::map<std::string, std::string>>::iterator it = path_maini.begin(); it != path_maini.end(); ++it)
	    printing_edges(*it);
        }
      if ( path_maini.size() != 0 ) path_main.push_back(path_maini);
    }
  printf("\n\nPath Main:\n\n");
  for (it_path_main = path_main.begin(); it_path_main != path_main.end(); ++it_path_main)
    {
      computing_lives_bypath(*it_path_main);
    }
}

void computing_lives_bypath(std::vector<std::map<std::string, std::string>> &it_path_main)
{
  for (std::vector<std::map<std::string, std::string>>::iterator it = it_path_main.begin(); it != it_path_main.end(); ++it)
    {
      struct status_amounts_edge *pt_status_byedge = get_status_byedge(*it);
      if ( find_open_incr_anypos(pt_status_byedge->status_src, pt_open_incr_anypos) )
      	{
	  
      	}
      if ( find_open_incr_anypos(pt_status_byedge->status_trk, pt_open_incr_anypos) )
      	{
	  
      	}
      settinglives_bypath(*it);
      printing_edges(*it);
    }
}

void clearing_operator_fifo(VectorTL &vdata, MatrixTL &M_file, int index_init, struct status_amounts *pt_pos, int idx_long_short, int &counting_netted, long int amount_trd_sum, std::vector<std::map<std::string, std::string>> &path_main, int path_number, long int opened_contracts)
{  
  extern int n_rows;
  extern int cols_news;
  
  VectorTL status_z(2);
  std::string addrs_opening = pt_pos->addrs_trk;
  long int d_amounts = 0;
  
  for (int i = index_init+1; i < n_rows; ++i)
    {
      VectorTL jrow_database(cols_news);
      sub_row(jrow_database, M_file, i);
      
      if ( finding(addrs_opening, jrow_database) )
        {
	  struct status_amounts *pt_status_addrs_trk = get_status_amounts_byaddrs(jrow_database, addrs_opening);
	  VectorTL status_netted = status_netted_npartly(status_z, idx_long_short);
	    
	  if ( finding(pt_status_addrs_trk->status_trk, status_netted) && pt_status_addrs_trk->nlives_trk != 0 )
	    {
	      counting_netted +=1;
	      amount_trd_sum += pt_status_addrs_trk->nlives_trk;
	      printf("\n\nNetted Event in the Row #%d\t Address Tracked: %s\n", i, addrs_opening.c_str());
	      printing_vector(jrow_database);
	      d_amounts = opened_contracts - amount_trd_sum;
	      printf("\n\nReview of d_amounts before cases:\t%ld", d_amounts);
	     
	      if ( d_amounts > 0 )
		{
		  printf("\n\nd_amounts = %ld > 0", d_amounts);
		  updating_lasttwocols_fromdatabase(addrs_opening, M_file, i, 0);
		  cout<<"\n\nCheck last two columns Updated: Row #"<<i<<"\n"<<M_file[i][8]<<"\t"<<M_file[i][9]<<"\n";
		  printf("\nOpened contrats: %ld > Sum amounts traded: %ld\n", opened_contracts, amount_trd_sum);

		  std::map<std::string, std::string> path_first;
		  building_edge(path_first, pt_status_addrs_trk, pt_pos->matched_price, 0, i, path_number, pt_status_addrs_trk->nlives_trk, 0);
		  path_main.push_back(path_first);
		  printf("\nEdge:\n");
		  printing_edges(path_first);

		  if ( find_open_incr_anypos(pt_status_addrs_trk->status_src, pt_open_incr_anypos) )
		    {
		      printf("\n_________________________________________\n");
		      int idx_long_shorti = find_open_incr_long(pt_status_addrs_trk->status_src, pt_open_incr_long) ? 0 : 1;
		      printf("\nLooking New Path for: %s", pt_status_addrs_trk->addrs_src.c_str());
		      struct status_amounts *pt_status_addrs_src = get_status_amounts_byaddrs(jrow_database, pt_status_addrs_trk->addrs_src);
		      int counting_nettedi = 0;
		      long int amount_trd_sumi = 0;
		      clearing_operator_fifo(jrow_database, M_file, i, pt_status_addrs_src, idx_long_shorti, counting_nettedi, amount_trd_sumi, path_main, path_number, pt_status_addrs_src->nlives_trk);
		      printf("\n_________________________________________\n");
		    }
		}
	      if ( d_amounts < 0)
		{
		  printf("\n\nd_amounts = %ld < 0", d_amounts);
		  updating_lasttwocols_fromdatabase(addrs_opening, M_file, i, labs(d_amounts));
		  cout<<"\n\nCheck last two columns Updated: Row #"<<i<<"\n"<<M_file[i][8]<<"\t"<<M_file[i][9]<<"\n";
		  printf("\nOpened contrats: %ld < Sum amounts traded: %ld\n", opened_contracts, amount_trd_sum);

		  std::map<std::string, std::string> path_first;
		  building_edge(path_first, pt_status_addrs_trk, pt_pos->matched_price, 0, i, path_number, pt_status_addrs_trk->nlives_trk-labs(d_amounts), 0);
		  path_main.push_back(path_first);
		  printf("\nEdge:\n");
		  printing_edges(path_first);

		  if ( find_open_incr_anypos(pt_status_addrs_trk->status_src, pt_open_incr_anypos) )
		    {
		      printf("\n_________________________________________\n");
		      int idx_long_shorti = find_open_incr_long(pt_status_addrs_trk->status_src, pt_open_incr_long) ? 0 : 1;
		      printf("\nLooking New Path for: %s", pt_status_addrs_trk->addrs_src.c_str());
		      struct status_amounts *pt_status_addrs_src = get_status_amounts_byaddrs(jrow_database, pt_status_addrs_trk->addrs_src);
		      int counting_nettedi = 0;
		      long int amount_trd_sumi = 0;
		      clearing_operator_fifo(jrow_database, M_file, i, pt_status_addrs_src, idx_long_shorti, counting_nettedi, amount_trd_sumi, path_main, path_number, pt_status_addrs_src->nlives_trk-labs(d_amounts)); 
		      printf("\n_________________________________________\n");
		    }
		  break;
		}
	      if ( d_amounts == 0)
		{
		  printf("\n\nd_amounts = %ld = 0", d_amounts);
		  updating_lasttwocols_fromdatabase(addrs_opening, M_file, i, 0);
		  cout<<"\n\nCheck last two columns Updated: Row #"<<i<<"\n"<<M_file[i][8]<<"\t"<<M_file[i][9]<<"\n";
		  printf("\nOpened contrats: %ld < Sum amounts traded: %ld\n", opened_contracts, amount_trd_sum);
		  
		  std::map<std::string, std::string> path_first;
		  building_edge(path_first, pt_status_addrs_trk, pt_pos->matched_price, 0, i, path_number, pt_status_addrs_trk->nlives_trk, 0);
		  path_main.push_back(path_first);
		  printf("\nEdge:\n");
		  printing_edges(path_first);

		  if ( find_open_incr_anypos(pt_status_addrs_trk->status_src, pt_open_incr_anypos) )
		    {
		      printf("\n_________________________________________\n");
		      int idx_long_shorti = find_open_incr_long(pt_status_addrs_trk->status_src, pt_open_incr_long) ? 0 : 1;
		      printf("\nLooking New Path for: %s", pt_status_addrs_trk->addrs_src.c_str());
		      struct status_amounts *pt_status_addrs_src = get_status_amounts_byaddrs(jrow_database, pt_status_addrs_trk->addrs_src);
		      int counting_nettedi = 0;
		      long int amount_trd_sumi = 0;
		      clearing_operator_fifo(jrow_database, M_file, i, pt_status_addrs_src, idx_long_shorti, counting_nettedi, amount_trd_sumi, path_main, path_number, pt_status_addrs_src->nlives_trk); 
		      printf("\n_________________________________________\n");
		    }
		  break;
		}	    
	    }
        }
    }
}

void updating_lasttwocols_fromdatabase(std::string addrs, MatrixTL &M_file, int i, long int live_updated)
{
  if ( addrs == M_file[i][0] )
    M_file[i][8] = std::to_string(live_updated);
  else
    M_file[i][9] = std::to_string(live_updated);
}

void building_edge(std::map<std::string, std::string> &path_first, struct status_amounts *pt_status_addrs_trk, long int entry_price, long int lives, int index_row, int path_number, long int amount_path, int ghost_edge)
{
  path_first["addrs_src"] = pt_status_addrs_trk->addrs_src;
  path_first["addrs_trk"] = pt_status_addrs_trk->addrs_trk;
  path_first["status_src"] = pt_status_addrs_trk->status_src;
  path_first["status_trk"] = pt_status_addrs_trk->status_trk;
  path_first["entry_price"] = std::to_string(entry_price);
  path_first["exit_price"] = std::to_string(pt_status_addrs_trk->matched_price);
  path_first["lives_src"] = std::to_string(lives);
  path_first["lives_trk"] = std::to_string(lives);
  path_first["amount_trd"] = std::to_string(amount_path);
  path_first["edge_row"] = std::to_string(index_row);
  path_first["path_number"] = std::to_string(path_number);
  path_first["ghost_edge"] = std::to_string(ghost_edge);
}

void printing_edges(std::map<std::string, std::string> &path_first)
{
  cout <<"{ "<<"addrs_src : "<<path_first["addrs_src"]<<", status_src : "<<path_first["status_src"]<<", lives_src : "<<path_first["lives_src"]<<", addrs_trk : "<<path_first["addrs_trk"]<<", status_trk : "<<path_first["status_trk"]<<", lives_trk : "<<path_first["lives_trk"]<<", entry_price : "<<path_first["entry_price"]<<", exit_price: "<<path_first["exit_price"]<<", amount_trd : "<<path_first["amount_trd"]<<", edge_row : "<<path_first["edge_row"]<<", path_number : "<<path_first["path_number"]<<", ghost_edge : "<<path_first["ghost_edge"]<<" }\n";
}

bool find_open_incr_anypos(std::string &s, VectorTL *v)
{
  VectorTL open_incr_anypos(4);
  VectorTL &u = *v;
  for (int i = 0; i < length(open_incr_anypos); i++) open_incr_anypos[i] = u[i];
  return finding(s, open_incr_anypos);
}

bool find_open_incr_long(std::string &s, VectorTL *v)
{
  VectorTL open_incr_long(2);
  VectorTL &u = *v;
  for (int i = 0; i < length(open_incr_long); i++) open_incr_long[i] = u[i];
  return finding(s, open_incr_long);
}

void settinglives_bypath(std::map<std::string, std::string> &path_maini)
{
  path_maini["lives_src"] = std::to_string(777);
  path_maini["lives_trk"] = std::to_string(777);
}
