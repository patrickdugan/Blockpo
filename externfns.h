#ifndef EXTERNFNS_H
#define EXTERNFNS_H

#include "tradelayer_matrices.h"
#include <vector>
#include <unordered_set>

void printing_matrix(MatrixTL &gdata);
void printing_vector(VectorTL &vdata);
bool finding(std::string &s, VectorTL &v);
void sub_row(VectorTL &jrow_databe, MatrixTL &databe, int i);
bool is_number(const std::string& s);
bool finding_string(std::string sub_word, std::string word_target);
bool find_string_strv(std::string s, std::vector<std::string> v);
bool find_string_set(std::string s, std::unordered_set<std::string> addrs_set);

#endif
