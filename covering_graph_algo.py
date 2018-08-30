#! /usr/bin/python

import sys
import numpy as np
from graph_operators import*
from collections import*
from time import sleep

import globales
import setglobales
import logging
import re

sys.stdout = open('out.txt', 'w')

print("###################################################################################\n")
print("\n#------------------------'Definitions'---------------------------------#\n")
print("Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n")
print("Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\t\t\topened or keep open contracts at the end date")

globales.init()
setglobales.stuff()

print("\n------------------------------------------------------\n")

M_file = opening_filetxt("graphInfoSixth.txt")

M_file = negative_for_short(M_file)
print("\n\nNegative or Positive signs for Short or Longs respectively:\n\n", np.array(M_file))
print("\n------------------------------------------------------\n")

CTRD = column(M_file, 6)
N_file = np.insert(M_file, 8, CTRD, 1)
N_file = np.insert(N_file, 9, CTRD, 1)
print("Two new columns for lives contracts to check those closed:\n\n", np.array(N_file))
print("\n------------------------------------------------------\n")

path_complex_two_matrix = []
k=0
for j in range(len(N_file)):

	bool_N_file, single_path_begin = first_single_path(N_file)
	if bool_N_file:
		print("Source: ", N_file[0][3], "; Tracked: ", N_file[0][0], "\n")
		print("Last Single path:\n", np.array(single_path_begin), "\n")
		print("###################################################################################\n")
		break
	###################################################################################

	N_filej = N_file[:][j]
	path_complex_two_long = []
	path_complex_two_short = []
	
	###################################################################################
	# Status to determinate if an edge could be a source #
	obj_long_trk = status_amounts_long_trk(N_filej)
	bool_track_long = True if obj_long_trk.status_trk in globales.open_incr_long else False
	obj_short_trk = status_amounts_short_trk(N_filej)
	bool_track_short = True if obj_short_trk.status_trk in globales.open_incr_short else False
	###################################################################################
	
	amount_trd_sum = 0
	diff_trdamount = 0

	###################################################################################
	# If an Edge can be a source: "clearing_operator" look for all netted events in the path #
	path_complex_main = []

	if bool_track_long and bool_track_short:

		print("###################################################################################\n")
		# When we found the last Edge which means "len(N_file) = 1" . The clearing algo Finish #
		idx_i = [0]
		print("N_file:\n\n", np.array(N_file), "\n\nLength N_file: ", len(N_file), "\n")

		print("\n(Tracking Long Position)", " Source: ", obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "\n")
		print("Row where were opened the contracts:", j, "!!")
		N_file, idx_i, path_complex_two_long = clearing_operator(N_file, obj_long_trk, amount_trd_sum, path_complex_two_long, idx_i, j, obj_long_trk.addrs_trk, obj_long_trk.amount_trd, 0, diff_trdamount, obj_long_trk.amount_trd)
		
		print("\n*********************************************************************\n")
		print("(Tracking Short Position)", " Source: ", obj_short_trk.addrs_src, "| Tracked: ", obj_short_trk.addrs_trk, "\n")
		N_file, idx_i, path_complex_two_short = clearing_operator(N_file, obj_short_trk, amount_trd_sum, path_complex_two_short, idx_i, j, obj_short_trk.addrs_trk, obj_long_trk.amount_trd, 1, diff_trdamount, obj_long_trk.amount_trd)

		single_path_value = [obj_long_trk.addrs_src, obj_long_trk.addrs_trk, obj_long_trk.status_src, obj_long_trk.status_trk, obj_long_trk.matched_price, obj_long_trk.matched_price, 0, 0, obj_long_trk.amount_trd, j]
		single_path_value_ele = OrderedDict(zip(globales.key_path, single_path_value))

		###################################################################################
		# Joining single paths from tracking long or short position opened #
		path_complex_main = append_fromlist_tolist(path_complex_two_long, path_complex_main)
		path_complex_main = append_fromlist_tolist(path_complex_two_short, path_complex_main)
		###################################################################################

		path_complex_main = calculating_livescontracts(path_complex_main, single_path_value_ele, N_file)

		contracts_opened, contracts_closed, contracts_lives, path_complex_main = checking_zeronetted_bypath(path_complex_main)

	warning_ifthereisno_zeronetted(path_complex_main, contracts_opened, contracts_closed, contracts_lives)

	checking_pathcontaining_livesnonzero(path_complex_main)	

	path_complex_two_matrix.append(path_complex_main)

pnl_total = total_pnl(path_complex_two_matrix)
print("Total PNL = ", pnl_total)