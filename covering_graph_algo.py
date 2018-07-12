#! /usr/bin/python

import sys
import numpy as np
from graph_operators import*
import pandas as pd

import globales
import setglobales

sys.stdout = open('out', 'w')

print "\n#------------------------'Definitions'---------------------------------#\n"            
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\t\t\topened or keep open contracts at the end date"

globales.init()
setglobales.stuff()

with open('graphInfoSixth.txt') as file:
	M_file = [[int(digit) if digit.isdigit() else digit for digit in line.split()] for line in file]

print "Negative or Positive:\n"
M_file = negative_for_short(M_file)

C_file_l = column(M_file, 2)
C_file_r = column(M_file, 5)

print np.array(C_file_r)
print np.array(C_file_l)

Interval = xrange(1, len(M_file))
path_complex_two_matrix = []

for j in Interval:

	idx_i = [0]
	print "M_file:\n", np.array(M_file), "\nLength M_file: ", len(M_file), "\n"
	bool_M_file, single_path_begin = first_single_path(M_file)

	if bool_M_file:        

		print "###################################################################################\n"
		print "Source: ", M_file[0][3], "; Tracked: ", M_file[0][0], "\n"
		print "Last Single path:\n", np.array(single_path_begin), "\n"
		break

	index_init = 0
	M_filej = []
	M_filej = M_file[:][index_init]

	path_complex_two_long = []
	path_complex_two_shrt = []

	print "###################################################################################\n"

	obj_long_trk  = status_amounts_long_trk(M_filej)
	bool_track_long  = True if obj_long_trk.status_trk  in globales.open_incr_long  else False

	obj_short_trk = status_amounts_short_trk(M_filej)
	bool_track_short = True if obj_short_trk.status_trk in globales.open_incr_short else False

	amount_trd_sum = 0

	if bool_track_long:

		print "(Tracking Long Position)", " Source: ", obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "\n"        
		M_file, idx_i, path_complex_two_long = clearing_operator(M_file, obj_long_trk, amount_trd_sum, path_complex_two_long, idx_i, index_init, obj_long_trk.addrs_trk, obj_long_trk.amount_trd)

	if bool_track_short:

		print "*********************************************************************\n"
		print "(Tracking Short Position)", " Source: ", obj_short_trk.addrs_src, "| Tracked: ", obj_short_trk.addrs_trk, "\n"
		M_file, idx_i, path_complex_two_shrt = clearing_operator(M_file, obj_short_trk, amount_trd_sum, path_complex_two_shrt, idx_i, index_init, obj_short_trk.addrs_trk, obj_long_trk.amount_trd)

	print "idx_new: ", idx_i

	path_complex_two = []

	PNL = 0
	single_path_value_ele = [obj_long_trk.addrs_src, obj_long_trk.lives_src, obj_long_trk.status_src, obj_long_trk.addrs_trk, obj_long_trk.lives_trk, obj_long_trk.status_trk, obj_long_trk.amount_trd, 0, obj_long_trk.matched_price, PNL]
	single_path_ele = dict(zip(globales.key_path, single_path_value_ele))

	path_complex_two.append(single_path_ele)
	path_complex_two = append_fromlist_tolist(path_complex_two_long, path_complex_two)
	path_complex_two = append_fromlist_tolist(path_complex_two_shrt, path_complex_two)

	print "\nPath:\n", np.array(path_complex_two), "\n"
	path_complex_two_matrix.append(path_complex_two)

	print "------------------------------------------------------"
	print "\nRows deleted: ", idx_i
	M_file = np.delete(M_file, idx_i, 0)