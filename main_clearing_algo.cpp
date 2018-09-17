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
	extern VectorTL *pt_open_incr_long;
	extern VectorTL *pt_open_incr_short;
	extern MatrixTL *pt_database;

	std::string address1, status1, address2, status2;
	double lives1, lives2, amount_trade, matched_price;
	int numItems = 0;

	ifstream file;
	file.open("graphInfoSixth.txt");  
	if ( file.is_open() ) {
		while ( !file.eof() ) {
			file >> address1 >> status1 >> lives1 >> address2 >> status2 >> lives2 >> amount_trade >> matched_price;
			++numItems;
		}
	}
	file.close();
	n_rows = numItems;

#include "init.h"

	/** Here start the clearing algorithm */
	printing_matrix(database);
	VectorTL vdata(n_cols);
	int path_counting = 0;
	
	for (int i = 0; i < n_rows; ++i)
	{
		extern VectorTL *pt_open_incr_long; VectorTL &open_incr_long = *pt_open_incr_long;
		extern VectorTL *pt_open_incr_short; VectorTL &open_incr_short = *pt_open_incr_short;

		for (int j = 0; j < n_cols; ++j) vdata[j] = database[i][j];

		struct status_amounts *pt_vdata_long  = get_status_amounts_open_incr(vdata, 0);
		struct status_amounts *pt_vdata_short = get_status_amounts_open_incr(vdata, 1);

		if ( finding(pt_vdata_long->status_trk, open_incr_long) && finding(pt_vdata_short->status_trk, open_incr_short) )
		{
			path_counting += 1;
			clearing_operator_fifo(vdata, database, i, pt_vdata_long);
		}
	}

	printf("\n\n|rows|: %d,\t |cols|: %d\n", size(database, 0), size(database, 1));
	return 0;
}