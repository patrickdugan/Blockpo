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

#******************************************************** Ale's Changes *********************************************************************#

def takeSecond(elem):
    return elem[1]

def translate(st):   
  # print(st)
  # print(" OpenLongPosition")
   if st == " OpenLongPosition" or st == " LongPosIncreased" or st == " LongPosNettedPartly" or st == " LongPosNetted":
       st = "Long"
   elif st == " OpenShortPosition" or st == " ShortPosIncreased" or st == " ShortPosNettedPartly" or st == " ShortPosNetted":         
        st = "Short"
   else:
      print("NO CHANGE")
   return st

#********************************************************************************************************************************************#

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
for j in range(len(N_file)):

<<<<<<< HEAD:covering_graph_algo1.py
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
    counting = 0 
    if bool_track_long and bool_track_short:

        counting +=1
        print("###################################################################################\n")
        # When we found the last Edge which means "len(N_file) = 1" . The clearing algo Finish #
        idx_i = [0]
        print("N_file:\n\n", np.array(N_file), "\n\nLength N_file: ", len(N_file), "\n")

        print("\n(Tracking Long Position)", " Source: ", obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "| Row: ", j, "\n")
        print("Row where were opened the contracts:", j, "!!")
        N_file, idx_i, path_complex_two_long = clearing_operator(N_file, obj_long_trk, amount_trd_sum, path_complex_two_long, idx_i, j, obj_long_trk.addrs_trk, obj_long_trk.amount_trd, 0, diff_trdamount, obj_long_trk.amount_trd)
        
        print("\n*********************************************************************\n")
        print("(Tracking Short Position)", " Source: ", obj_short_trk.addrs_src, "| Tracked: ", obj_short_trk.addrs_trk, "| Row: ", j, "\n")
        N_file, idx_i, path_complex_two_short = clearing_operator(N_file, obj_short_trk, amount_trd_sum, path_complex_two_short, idx_i, j, obj_short_trk.addrs_trk, obj_long_trk.amount_trd, 1, diff_trdamount, obj_long_trk.amount_trd)

        ###################################################################################
        # Joining all the single paths in just one" #
        pnl_trk = PNL_function(obj_long_trk.matched_price, obj_long_trk.matched_price, obj_long_trk.amount_trd)
        single_path_value = [obj_long_trk.addrs_src, obj_long_trk.addrs_trk, obj_long_trk.status_src, obj_long_trk.status_trk, obj_long_trk.matched_price, obj_long_trk.matched_price, 0, 0, obj_long_trk.amount_trd, j, pnl_trk]
        single_path_value_ele = OrderedDict(zip(globales.key_path, single_path_value))
        path_complex_main = append_fromlist_tolist(path_complex_two_long, path_complex_main)
        path_complex_main = append_fromlist_tolist(path_complex_two_short, path_complex_main)

        ###################################################################################
        # Function to calculate lives contracts by path and address #
        print("\nMain addresses opening contracts: ", single_path_value_ele['addrs_src'], single_path_value_ele['addrs_trk'])

        openedfor_first_adrrs = single_path_value_ele['amount_trd']
        sum_amountsfor_src_first = 0
        index_src_first = 0
        sum_amountsfor_trk_first = 0
        index_trk_first = 0
        counter_src = 0
        counter_trk = 0
    
        for j in range(len(path_complex_main)):
            pathj = path_complex_main[j]
            # Looking for netted events for the address to the left #
            if pathj['addrs_trk'] == single_path_value_ele['addrs_src']:
                counter_src += 1
                sum_amountsfor_src_first += pathj['amount_trd']
                index_src_first = j
                # Checking here if the address to the left of the path open or increase a position #
                path_complex_main = lookingforlives_insidepath(j, path_complex_main, pathj['status_src'], pathj['addrs_src'], pathj['amount_trd'], N_file)
                # Looking for netted events for the address to the right #                
            if pathj['addrs_trk'] == single_path_value_ele['addrs_trk']:
                counter_trk += 1
                sum_amountsfor_trk_first += pathj['amount_trd']
                index_trk_first = j
                # Checking here if the address to the left of the path open or increase a position #
                path_complex_main = lookingforlives_insidepath(j, path_complex_main, pathj['status_src'], pathj['addrs_src'], pathj['amount_trd'], N_file)
        ###################################################################################
        # Copying lives contracts after netted events #
        for index in range(len(path_complex_main)):
            if index == index_src_first and counter_src != 0:
                path_complex_main[index]['lives_trk'] = float("{0:.2f}".format(abs(openedfor_first_adrrs-sum_amountsfor_src_first)))
            elif index == index_trk_first and counter_trk != 0:
                path_complex_main[index]['lives_trk'] = float("{0:.2f}".format(abs(openedfor_first_adrrs-sum_amountsfor_trk_first)))

        print("counter_src: ", counter_src, "counter_trk", counter_trk)            
        path_complex_main.insert(0, single_path_value_ele)
        ###################################################################################
        # Just in case the tracked address never netted #
        if counter_trk == 0:
            path_complex_main[0]['lives_trk'] = openedfor_first_adrrs
        elif counter_src == 0:
            path_complex_main[0]['lives_src'] = openedfor_first_adrrs
        ###################################################################################
        # Function to check Zero Netted events by Path #
        contracts_opened = 0
        contracts_closed = 0
        contracts_lives = 0

        for row in path_complex_main:
            if row['status_src'] in globales.open_incr_long_short and row['status_trk'] in globales.open_incr_long_short:
                contracts_opened += row['amount_trd']
            if row['status_src'] in globales.all_netted_status and row['status_trk'] in globales.all_netted_status:
                contracts_closed += row['amount_trd']
            contracts_lives += row['lives_src']+row['lives_trk']

        if len(path_complex_main) == 1:
            path_complex_main[0]['lives_trk'] = path_complex_main[0]['amount_trd']
            path_complex_main[0]['lives_src'] = path_complex_main[0]['amount_trd']
            contracts_lives = path_complex_main[0]['lives_trk'] + path_complex_main[0]['lives_src'] 
            
        print("\nChecking Zero Netted by Path:\n(contracts_opened - contracts_closed)-contracts_lives = ", "(", abs(float("{0:.1f}".format(2*contracts_opened))), "-", abs(float("{0:.1f}".format(contracts_closed))), ") -", abs(float("{0:.1f}".format(contracts_lives))), "=", abs(float("{0:.1f}".format((2*contracts_opened - contracts_closed)))), "-", abs(float("{0:.1f}".format(contracts_lives))),"=", abs(float("{0:.1f}".format((2*contracts_opened - contracts_closed)-contracts_lives))))
        ###################################################################################

    if len(path_complex_main)!=0:
        if abs(float("{0:.1f}".format((2*contracts_opened - contracts_closed)-contracts_lives))) != 0:
            k += 1
            print(k, ": Warning!! There is no zero netted event in the path")
        print("\nPath:\n", np.array(path_complex_main), "\n")
        path_complex_two_matrix.append(path_complex_main)
    print(path_complex_two_matrix)    
#******************************************************** Ale's Changes *********************************************************************#
    buyers = []
    sellers = []
    for row in path_complex_two_matrix:
        list = str(row).replace("OrderedDict", "").strip("[]").replace("[","").replace("]","").replace("(","").replace(")","").replace("'","")
        array = list.split(",")
        i = 0
        line = []
        for elem in array :
            line.append(str(elem))
            if i == 21:
               trans = translate(str(line[5]))
               trans1 = translate(str(line[7]))
               if line[13] != " 0" and line[13] != " 0.0":
                   print(str(line[1]).ljust(3),end=" ")
                   print(trans.rjust(3),end=" ")
                   print(str(line[9]).rjust(3),end=" ")
                   print(str(line[13]).rjust(3))
                   if trans1 == "Long":
                       sellers.append((float(line[13]),float(line[9]))) # putting (amount,price)
                   elif trans == "Short":
                       buyers.append((float(line[13]),float(line[9])))
               if line[15] != " 0" and line[15] != " 0.0":
                   print(str(line[3]).ljust(3),end=" ")
                   print(trans1.rjust(3),end=" ")
                   print(str(line[9]).rjust(3),end=" ")
                   print(str(line[15]).rjust(3))
               i = 0
               line = []
            else:
               i = i + 1
    sortedSellers = sorted(sellers, key=takeSecond)
    sortedBuyers = sorted(buyers, key=takeSecond)
    print("\n")
    print("##########*****SELLER SIDE OF ORDERBOOK*****##########")
    print("\n")
    for elem in reversed(sortedSellers):
        print(elem)
    print("\n")
    print("##########******BUYER SIDE OF ORDERBOOK*****##########")
    print("\n")
    for elem in reversed(sortedBuyers):
        print(elem)
    print("\n")


=======
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
>>>>>>> c0101147776c42462bca36d4990cf7b05b3f4d4e:covering_graph_algo.py
