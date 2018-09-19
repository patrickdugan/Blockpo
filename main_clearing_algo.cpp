#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <type_traits>
#include <stdio.h>

#include "operators_algo_clearing.h"
#include "tradelayer_matrices.h"
#include "externfns.h"

using namespace std;

int main()
{
  extern int n_cols;
  extern int n_rows;
  extern int cols_news;
  extern VectorTL *pt_open_incr_long;
  extern VectorTL *pt_open_incr_short;
  extern VectorTL *pt_netted_npartly_long;
  extern VectorTL *pt_netted_npartly_short;
  extern MatrixTL *pt_database;
  extern VectorTL *pt_open_incr_anypos;
  
  counting_databaserows();
  
#include "init.h"
  
  MatrixTL M_file(n_rows, cols_news);
  adding_newtwocols_trdamount(M_file, database);
  settlement_algorithm_fifo(M_file);
  
  printf("\n\n|rows|: %d,\t |cols|: %d\n", size(database, 0), size(database, 1));
  
  return 0;
}
