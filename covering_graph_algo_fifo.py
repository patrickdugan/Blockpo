#! /usr/bin/python

import sys
import numpy as np
from graph_operators_fifo import*
from collections import*
from time import sleep

import globales
import setglobales
import logging
import re

sys.stdout = open('out_fifo.txt', 'w')

print("###################################################################################\n")
print("\n#------------------------'Definitions'---------------------------------#\n")
print("Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n")
print("Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\t\t\topened or keep open contracts at the end date")

globales.init()
setglobales.stuff()

print("\n------------------------------------------------------\n")

M_file = globales.database_matrix

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

sum_gamma_p = 0
sum_gamma_q = 0
price_settlement_union = 0

listof_longlives  = []
listof_shortlives = []
path_counting = 0
 
for j in range(len(N_file)):

	N_filej = N_file[:][j]
	path_complex_two_long = []
	path_complex_two_short = []
	
	obj_long_trk = status_amounts_long_trk(N_filej)
	bool_track_long = True if obj_long_trk.status_trk in globales.open_incr_long else False
	obj_short_trk = status_amounts_short_trk(N_filej)
	bool_track_short = True if obj_short_trk.status_trk in globales.open_incr_short else False
	
	amount_trd_sum = 0
	diff_trdamount = 0

	counting_netted_long  = 0
	counting_netted_short = 0

	path_complex_main = []

	if bool_track_long and bool_track_short:

		path_counting += 1
		sum_oflives = 0
		exit_price_desired = 0 
		PNL_total = 0
		gamma_p =0
		gamma_q = 0

		listof_longlives_ele  = []
		listof_shortlives_ele = []

		print("###################################################################################\n")
		print("N_file:\n\n", np.array(N_file), "\n\nLength N_file: ", len(N_file), "\n")

		print("\n(Tracking Long Position)", " Source: ", obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "\n")
		print("Row where were opened the contracts:", j, "!!")
		N_file, path_complex_two_long, counting_netted_long = clearing_operator_fifo(N_file, obj_long_trk, amount_trd_sum, path_complex_two_long, j, obj_long_trk.addrs_trk, obj_long_trk.amount_trd, 0, diff_trdamount, obj_long_trk.amount_trd, path_counting, counting_netted_long)
		
		print("\n*********************************************************************\n")
		print("(Tracking Short Position)", " Source: ", obj_short_trk.addrs_src, "| Tracked: ", obj_short_trk.addrs_trk, "\n")
		N_file, path_complex_two_short, counting_netted_short = clearing_operator_fifo(N_file, obj_short_trk, amount_trd_sum, path_complex_two_short, j, obj_short_trk.addrs_trk, obj_long_trk.amount_trd, 1, diff_trdamount, obj_long_trk.amount_trd, path_counting, counting_netted_short)

		single_path_value = [obj_long_trk.addrs_src, obj_long_trk.addrs_trk, obj_long_trk.status_src, obj_long_trk.status_trk, obj_long_trk.matched_price, obj_long_trk.matched_price, 0, 0, obj_long_trk.amount_trd, j, path_counting, 0]
		single_path_value_ele = OrderedDict(zip(globales.key_path_fifo, single_path_value))

		path_complex_main.append(single_path_value_ele)
		path_complex_main = append_fromlist_tolist(path_complex_two_long, path_complex_main)
		path_complex_main = append_fromlist_tolist(path_complex_two_short, path_complex_main)

		path_complex_main = calculating_livescontracts(path_complex_main)

		contracts_opened, contracts_closed, contracts_lives, path_complex_main = checking_zeronetted_bypath(path_complex_main)

		warning_ifthereisno_zeronetted(path_complex_main, contracts_opened, contracts_closed, contracts_lives)

		sum_oflives, exit_price_desired, PNL_total, gamma_p, gamma_q = checking_pathcontaining_livesnonzero(path_complex_main, sum_oflives, exit_price_desired, PNL_total, gamma_p, gamma_q)	

		listof_longlives_ele, listof_shortlives_ele = obtaining_arraysfor_liveslongshort(path_complex_main, listof_longlives_ele, listof_shortlives_ele)

		if len(listof_longlives_ele) != 0:
			print('\nChecking on this Path:\nlistof_longlives:\n', np.array(listof_longlives_ele))
			listof_longlives  = append_fromlist_tolist(listof_longlives_ele, listof_longlives)
		else:
			print('\nThere is no long contracts lives!!')

		if len(listof_shortlives_ele) != 0:
			print('\nChecking on this Path:\nlistof_shortlives:\n', np.array(listof_shortlives_ele))
			listof_shortlives = append_fromlist_tolist(listof_shortlives_ele, listof_shortlives)
		else:
			print('\nThere is no short contracts lives')

		path_complex_two_matrix.append(path_complex_main)

	sum_gamma_p += gamma_p
	sum_gamma_q += gamma_q

price_settlement_union = float(sum_gamma_p)/sum_gamma_q
print('\nPrice Settlement Union = ', price_settlement_union)

sumof_lives_longs, sumof_lives_shorts = counting_livesfor_longshort(listof_longlives, listof_shortlives)
print('\nsumof_lives_shorts = ', sumof_lives_shorts, '\tsumof_lives_longs', sumof_lives_longs, '\tDifference = ', abs(sumof_lives_shorts-sumof_lives_longs))

plot_data(path_complex_two_matrix)

print("\nFrom here start Ghost Paths building:\n")
# path_complex_two_matrix = updating_lives_inthepath(path_complex_two_matrix)

ghost_edges_array = []
ghost_edges_array = calculating_ghost_edges(listof_longlives, listof_shortlives, price_settlement_union, ghost_edges_array)
# ghost_edges_array = remove_duplicate_rows_json(ghost_edges_array, ghost_edges_array)
# print('\n\nghost_edges_array:\n', np.array(ghost_edges_array))

path_complex_two_matrix = joining_pathclear_ghostpath(path_complex_two_matrix, ghost_edges_array)
print('\nFinal matrix with Ghost Edges added:\n\n', np.array(path_complex_two_matrix))

sum_oflives = 0
exit_price_desired = 0 
PNL_total = 0
gamma_p =0
gamma_q = 0

print('\nChecking zero netted for final matrix with ghost edges added:\n')
for row in path_complex_two_matrix:
	print('\nPath with ghost edges:\n\n', np.array(row))
	contracts_lives, netted_byghosts = checking_zeronetted_bypath_withghostedges(row)
	# sum_oflives, exit_price_desired, PNL_total, gamma_p, gamma_q = checking_pathcontaining_livesnonzero(row, sum_oflives, exit_price_desired, PNL_total, gamma_p, gamma_q)