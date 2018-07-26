#! /usr/bin/python

import sys
import numpy as np
from graph_operators import*
import pandas as pd
from collections import*

import globales
import setglobales

sys.stdout = open('out', 'w')

print "###################################################################################\n"
print "\n#------------------------'Definitions'---------------------------------#\n"            
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\t\t\topened or keep open contracts at the end date"

globales.init()
setglobales.stuff()

print "\n------------------------------------------------------\n"

M_file = opening_filetxt("graphInfoSixth.txt")

M_file = negative_for_short(M_file)

print "Negative or Positive signs for Short or Longs respectively:\n\n", np.array(M_file)
print "\n------------------------------------------------------\n"
C_file_src = column(M_file, 2)
C_file_trk = column(M_file, 5)

N_file = np.insert(M_file, 8, np.array(C_file_src), 1)
N_file = np.insert(N_file, 9, np.array(C_file_trk), 1)

print "Two new columns for lives contracts to check those closed:\n\n", np.array(N_file)
print "\n------------------------------------------------------\n"

path_complex_two_matrix = []

for j in range(len(N_file)):

	idx_i = [0]
	print "N_file:\n\n", np.array(N_file), "\n\nLength N_file: ", len(N_file), "\n"
	bool_N_file, single_path_begin = first_single_path(N_file)

	if bool_N_file:        

		print "###################################################################################\n"
		print "Source: ", N_file[0][3], "; Tracked: ", N_file[0][0], "\n"
		print "Last Single path:\n", np.array(single_path_begin), "\n"
		break

	N_filej = N_file[:][j]

	if N_filej[8] == 0 and N_filej[9] == 0:
		continue

	path_complex_two_long = []
	path_complex_two_short = []

	print "###################################################################################\n"

	obj_long_trk  = status_amounts_long_trk(N_filej)
	bool_track_long  = True if obj_long_trk.status_trk  in globales.open_incr_long  else False

	obj_short_trk = status_amounts_short_trk(N_filej)
	bool_track_short = True if obj_short_trk.status_trk in globales.open_incr_short else False

	amount_trd_sum = 0

	if ( bool_track_long and N_filej[8] != 0 ) and ( bool_track_short and N_filej[9] != 0 ):

		print "\n(Tracking Long Position)", " Source: ", obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "\n"
                print "Row where were opened the contracts: ", j, "!!"
		N_file, idx_i, path_complex_two_long = clearing_operator(N_file, obj_long_trk, amount_trd_sum, path_complex_two_long, idx_i, j, obj_long_trk.addrs_trk, obj_long_trk.amount_trd, 0)

		print "\n*********************************************************************\n"
		print "(Tracking Short Position)", " Source: ", obj_short_trk.addrs_src, "| Tracked: ", obj_short_trk.addrs_trk, "\n"
		N_file, idx_i, path_complex_two_short = clearing_operator(N_file, obj_short_trk, amount_trd_sum, path_complex_two_short, idx_i, j, obj_short_trk.addrs_trk, obj_long_trk.amount_trd, 1)

                N_file[:][j][8] = 0
		N_file[:][j][9] = 0

                print "\nFirst row update:\n", N_file[:][j]

	path_complex_two = []

	single_path_value = [obj_long_trk.addrs_src, obj_long_trk.addrs_trk, obj_long_trk.status_src, obj_long_trk.status_trk, obj_long_trk.matched_price, obj_long_trk.amount_trd, 'Edge Source']
        single_path_value_ele = OrderedDict(zip(globales.key_path_new, single_path_value))

	path_complex_two.append(single_path_value_ele)		
	path_complex_two = append_fromlist_tolist(path_complex_two_long, path_complex_two)
	path_complex_two = append_fromlist_tolist(path_complex_two_short, path_complex_two)

	print "\nPath:\n", np.array(path_complex_two), "\n"
	path_complex_two_matrix.append(path_complex_two)
