#include "externfns.h"
#include "tradelayer_matrices.h"

using namespace std;

void printing_matrix(MatrixTL &gdata)
{
    int n_rows = size(gdata, 0);
    int n_cols = size(gdata, 1);

    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            std::cout << gdata[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void printing_vector(VectorTL &vdata)
{
    int n = length(vdata);
    for (int i = 0; i < n; ++i) cout << vdata[i] << "\t";
}

bool finding(std::string &s, VectorTL &v)
{
  int n = length(v);
  bool findout = false;
  
  for (int i = 0; i < n; ++i)
    { 
      if ( v[i] == s )
	{
	  findout = true;
	  break;
        }
    }   
  return findout;
}

void sub_row(VectorTL &jrow_databe, MatrixTL &databe, int i)
{
  for (int j = 0; j < size(databe, 1); ++j) jrow_databe[j] = databe[i][j];
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
