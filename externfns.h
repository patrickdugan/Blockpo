#ifndef EXTERNFNS_H
#define EXTERNFNS_H

#include "tradelayer_matrices.h"

void printing_matrix(MatrixTL &gdata);
void printing_vector(VectorTL &vdata);
bool finding(std::string &s, VectorTL &v);
void sub_row(VectorTL &jrow_databe, MatrixTL &databe, int i);
bool is_number(const std::string& s);

#endif
