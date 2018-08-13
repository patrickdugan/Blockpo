#! /usr/bin/python

import sys
import numpy as np
from graph_operators import*
from collections import*

import globales
import setglobales

sys.stdout = open('out', 'w')

print("###################################################################################\n")
print("\n#------------------------'Definitions'---------------------------------#\n")
print("Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n")
print("Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\t\t\topened or keep open contracts at the end date")

globales.init()
setglobales.stuff()

print("\n------------------------------------------------------\n")

M_file = opening_filetxt("graphInfoSixth.txt")

opened_atsett = 0
opened_atsett = difference_bet_openedandnetted(M_file, opened_atsett)
print("opened_atsett:", opened_atsett)

M_file = negative_for_short(M_file)

print("\n\nNegative or Positive signs for Short or Longs respectively:\n\n", np.array(M_file))
print("\n------------------------------------------------------\n")

Last_cols = [777 for row in M_file]

N_file = np.insert(M_file, 8, Last_cols, 1)
N_file = np.insert(N_file, 9, Last_cols, 1)

print("Two new columns for lives contracts to check those closed:\n\n", np.array(N_file))
print("\n------------------------------------------------------\n")

path_complex_two_matrix = []

for j in range(len(N_file)):

    idx_i = [0]
    print("N_file:\n\n", np.array(N_file),
          "\n\nLength N_file: ", len(N_file), "\n")
    bool_N_file, single_path_begin = first_single_path(N_file)

    if bool_N_file:
        print("###################################################################################\n")
        print("Source: ", N_file[0][3], "; Tracked: ", N_file[0][0], "\n")
        print("Last Single path:\n", np.array(single_path_begin), "\n")
        break

    N_filej = N_file[:][j]
    path_complex_two_long = []
    path_complex_two_short = []

    print("###################################################################################\n")

    obj_long_trk = status_amounts_long_trk(N_filej)
    bool_track_long = True if obj_long_trk.status_trk in globales.open_incr_long else False

    obj_short_trk = status_amounts_short_trk(N_filej)
    bool_track_short = True if obj_short_trk.status_trk in globales.open_incr_short else False

    amount_trd_sum = 0
    diff_trdamount = 0

    if bool_track_long and bool_track_short:

        print("\n(Tracking Long Position)", " Source: ",
              obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "\n")
        print("Row where were opened the contracts:", j, "!!")
        N_file, idx_i, path_complex_two_long = clearing_operator(
            N_file, obj_long_trk, amount_trd_sum, path_complex_two_long, idx_i, j, obj_long_trk.addrs_trk, obj_long_trk.amount_trd, 0, diff_trdamount)
        print("\n*********************************************************************\n")
        print("(Tracking Short Position)", " Source: ",
              obj_short_trk.addrs_src, "| Tracked: ", obj_short_trk.addrs_trk, "\n")
        N_file, idx_i, path_complex_two_short = clearing_operator(
            N_file, obj_short_trk, amount_trd_sum, path_complex_two_short, idx_i, j, obj_short_trk.addrs_trk, obj_long_trk.amount_trd, 1, diff_trdamount)
        N_file[:][j][8] = 0
        N_file[:][j][9] = 0
        print("\nFirst row update:\n", N_file[:][j])

    path_complex_main = []
    single_path_value = [obj_long_trk.addrs_src, obj_long_trk.addrs_trk, obj_long_trk.status_src, obj_long_trk.status_trk, obj_long_trk.matched_price, obj_long_trk.matched_price, 0, 0, obj_long_trk.amount_trd]
    single_path_value_ele = OrderedDict(zip(globales.key_path, single_path_value))
    
    path_complex_main = append_fromlist_tolist(path_complex_two_long, path_complex_main)
    path_complex_main = append_fromlist_tolist(path_complex_two_short, path_complex_main)

    print("\nComputing lives contracts:")
    print("Main addresses opening contracts: ", single_path_value_ele['addrs_src'], single_path_value_ele['addrs_trk'], "\n")

    openedfor_first_adrrs = single_path_value_ele['amount_trd']
    sum_amountsfor_src_first = 0
    index_src_first = 0
    sum_amountsfor_trk_first = 0
    index_trk_first = 0
    
    for j in range(len(path_complex_main)):
        pathj = path_complex_main[j]
        if pathj['addrs_trk'] == single_path_value_ele['addrs_src']:
            sum_amountsfor_src_first += pathj['amount_trd']
            index_src_first = j
            lookingforlives_insidepath(j, path_complex_main, pathj['status_src'], pathj['addrs_src'], pathj['amount_trd'])
                
        if pathj['addrs_trk'] == single_path_value_ele['addrs_trk']:
            sum_amountsfor_trk_first += pathj['amount_trd']
            index_trk_first = j
            lookingforlives_insidepath(j, path_complex_main, pathj['status_src'], pathj['addrs_src'], pathj['amount_trd'])

    print("\nLives contracts for ", single_path_value_ele['addrs_src'], "here:", "openedfor_first_adrrs - sum_amountsfor_src = ", openedfor_first_adrrs, "-", sum_amountsfor_src_first, "=", int(openedfor_first_adrrs-sum_amountsfor_src_first))
    print("\nindex_src_first", index_src_first)
    
    print("\nLives contracts for ", single_path_value_ele['addrs_trk'], "here:", "openedfor_first_adrrs - sum_amountsfor_src = ", openedfor_first_adrrs, "-", sum_amountsfor_trk_first, "=", int(openedfor_first_adrrs-sum_amountsfor_trk_first))
    print("\nindex_trk_first", index_trk_first)

    for index in range(len(path_complex_main)):
        if index == index_src_first:
            path_complex_main[index]['lives_src'] = int(openedfor_first_adrrs-sum_amountsfor_src_first)
        elif index == index_trk_first:
            path_complex_main[index]['lives_trk'] = int(openedfor_first_adrrs-sum_amountsfor_trk_first)        
    
    path_complex_main.insert(0, single_path_value_ele)
    print("\nPath:\n", np.array(path_complex_main), "\n")
    path_complex_two_matrix.append(path_complex_main)

