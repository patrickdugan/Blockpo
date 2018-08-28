#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <type_traits>
#include <stdio.h>

#include "operators_algo_clearing.h"

using namespace std;

int main()
{
    std::string address1, status1, address2, status2;
    double lives1, lives2, amount_trade, matched_price;
    int numItems = 0, n_cols = 8;

    ifstream file;
    file.open("graphInfoSixth.txt");  
    if ( file.is_open() ) {
        while ( !file.eof() ) {
	  file >> address1 >> status1 >> lives1 >> address2 >> status2 >> lives2 >> amount_trade >> matched_price;
	  ++numItems;
        }
    }
    int n_rows = numItems;
    file.close();

    std::string **gdata = new std::string*[n_cols+1];
    for (int i = 0; i < n_cols; ++i) {
        gdata[i] = new std::string[n_rows];
    }

    std::fstream fileg;
    fileg.open("graphInfoSixth.txt", std::ios::in);
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            fileg >> gdata[j][i];
        }
    }
    fileg.close();

    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            std::cout << gdata[j][i] << "\t";
        }
        std::cout << "\n";
    }
    printf("Number of rows: %d,\t Number of cols: %d\n", n_rows, n_cols);
    
    
    delete [] gdata;
    return 0;
}
