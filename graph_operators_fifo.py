#! /usr/bin/python

import numpy as np
import globales
import json
import setglobales
from amount_status import*
from collections import*
import re

globales.init()
setglobales.stuff()

def clearing_operator_fifo(M_file, obj_trk, amount_trd_sum, path_complex_two, index_init, addrs_trk_arg, amount_trd_begining, index_long_short, diff_newtrdamount, amount_inthepath, path_counting, counting_netted):
	
	amount_trd_sum_new = 0
	diff_newtrdamount = 0
	path_complex_first = []
	path_complex_secnd = []
	path_complex_third = []

	for i in range(index_init+1, len(M_file)):

		N_filei = M_file[:][i]

		if addrs_trk_arg in N_filei:

			obj_trk_inloop = status_amounts_inloop(N_filei, addrs_trk_arg)

			status_netted = globales.netted_status_long if index_long_short == 0 else globales.netted_status_short

			if obj_trk_inloop.status_trk in status_netted and obj_trk_inloop.lives_trk_updated != 0:

				counting_netted += 1
				amount_trd_sum += obj_trk_inloop.lives_trk_updated
				print('\nChecking I am here!!\n', 'amount_trd_sum <= amount_trd_begining: ', 'amount_trd_sum =', amount_trd_sum, 'amount_trd_begining = ', amount_trd_begining)

				d_amounts = amount_trd_begining - amount_trd_sum
			
				if d_amounts > 0:
				
					print('Netted Event d_amounts > 0')
					print('Lives at this time: ', abs(d_amounts))
					print("\nRow to update:\n", M_file[:][i])
					M_file = updating_lasttwocols_fromdatabase(addrs_trk_arg, N_filei, M_file, i, 0)
					print("\nRow updated:\n", M_file[:][i])
					print("\nOpened contrats: ", amount_trd_begining, " > Sum amounts traded: ", amount_trd_sum, "\n")
					print("'iteration: ", i, "|addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trk,  "|amount_trdi: ", obj_trk_inloop.amount_trd, "|addrs_srci: ", obj_trk_inloop.addrs_src, "|status_srci: ", obj_trk_inloop.status_src, "\n")
					
					path_complex_value_ele_two = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src, obj_trk_inloop.status_trk, obj_trk.matched_price, obj_trk_inloop.matched_price, 0, 0, obj_trk_inloop.lives_trk_updated, i, path_counting]
					path_complex_ele_two = OrderedDict(zip(globales.key_path, path_complex_value_ele_two))

					print("(d_amounts > 0) path_complex_ele_two: \n\n", path_complex_ele_two)

					path_complex_two.append(path_complex_ele_two)

					if obj_trk_inloop.status_src in globales.open_incr_long_short:
					
						print('.................................................................')
						idx_long_or_short = 0 if obj_trk_inloop.status_src in globales.open_incr_long else 1
						print("\nLooking New Path for: ", obj_trk_inloop.addrs_src, "!!")
						obj_clearing_again_first = status_amounts_inloop(N_filei, obj_trk_inloop.addrs_src)
						counting_netted_first = 0
						M_file, path_complex_first, counting_netted_first = clearing_operator_fifo(M_file, obj_clearing_again_first, amount_trd_sum_new, path_complex_first, i, obj_clearing_again_first.addrs_trk, obj_clearing_again_first.lives_trk_updated, idx_long_or_short, diff_newtrdamount, obj_clearing_again_first.lives_trk_updated, path_counting, counting_netted_first)

						if counting_netted_first != 0:
							path_complex_two = append_fromlist_tolist(path_complex_first, path_complex_two)
	
						print('.................................................................')

				if d_amounts < 0:
				
					print('Netted Event d_amounts < 0')
					print('Lives at this time: ', 0)
					print("\nRow to update:\n", M_file[:][i])
					M_file = updating_lasttwocols_fromdatabase(addrs_trk_arg, N_filei, M_file, i, abs(d_amounts))
					print("\nRow updated:\n", M_file[:][i])
					print("\nOpened contrats: ", amount_trd_begining, " < Sum amounts traded: ", amount_trd_sum, "\n")
					print("'iteration: ", i, "|addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trk,  "|amount_trdi: ", obj_trk_inloop.lives_trk_updated, "|addrs_srci: ", obj_trk_inloop.addrs_src, "|status_srci: ", obj_trk_inloop.status_src, "\n")
					
					path_complex_value_ele_two = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src, obj_trk_inloop.status_trk, obj_trk.matched_price, obj_trk_inloop.matched_price, 0, 0, obj_trk_inloop.lives_trk_updated - abs(d_amounts), i, path_counting]
					path_complex_ele_two = OrderedDict(zip(globales.key_path, path_complex_value_ele_two))

					print("(d_amounts < 0) path_complex_ele_two: \n\n", path_complex_ele_two)
					path_complex_two.append(path_complex_ele_two)

					if obj_trk_inloop.status_src in globales.open_incr_long_short:
					
						print('.................................................................')
						idx_long_or_short = 0 if obj_trk_inloop.status_src in globales.open_incr_long else 1
						print("\nLooking New Path for: ", obj_trk_inloop.addrs_src, "!!")
						obj_clearing_again_secnd = status_amounts_inloop(N_filei, obj_trk_inloop.addrs_src)
						counting_netted_secnd = 0
						M_file, path_complex_secnd, counting_netted_secnd = clearing_operator_fifo(M_file, obj_clearing_again_secnd, amount_trd_sum_new, path_complex_secnd, i, obj_clearing_again_secnd.addrs_trk, obj_clearing_again_secnd.amount_trd - abs(d_amounts), idx_long_or_short, diff_newtrdamount, obj_clearing_again_secnd.lives_trk_updated - abs(d_amounts), path_counting, counting_netted_secnd)

						if counting_netted_secnd != 0:
							path_complex_two = append_fromlist_tolist(path_complex_secnd, path_complex_two)
	
						print('.................................................................')

					break

				if d_amounts == 0:
				
					print('Netted Event d_amounts == 0')
					print('Lives at this time: ', abs(d_amounts))
					print("\nRow to update:\n", M_file[:][i])
					M_file = updating_lasttwocols_fromdatabase(addrs_trk_arg, N_filei, M_file, i, 0)
					print("\nRow updated:\n", M_file[:][i])
					print("\nOpened contrats: ", amount_trd_begining, " > Sum amounts traded: ", amount_trd_sum, "\n")
					print("'iteration: ", i, "|addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trk,  "|amount_trdi: ", obj_trk_inloop.amount_trd, "|addrs_srci: ", obj_trk_inloop.addrs_src, "|status_srci: ", obj_trk_inloop.status_src, "\n")
					
					path_complex_value_ele_two = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src, obj_trk_inloop.status_trk, obj_trk.matched_price, obj_trk_inloop.matched_price, 0, 0, obj_trk_inloop.lives_trk_updated, i, path_counting]
					path_complex_ele_two = OrderedDict(zip(globales.key_path, path_complex_value_ele_two))

					print("(d_amounts > 0) path_complex_ele_two: \n\n", path_complex_ele_two)

					path_complex_two.append(path_complex_ele_two)

					if obj_trk_inloop.status_src in globales.open_incr_long_short:
					
						print('.................................................................')
						idx_long_or_short = 0 if obj_trk_inloop.status_src in globales.open_incr_long else 1
						print("\nLooking New Path for: ", obj_trk_inloop.addrs_src, "!!")
						obj_clearing_again_third = status_amounts_inloop(N_filei, obj_trk_inloop.addrs_src)
						counting_netted_third = 0
						M_file, path_complex_third, counting_netted_third = clearing_operator_fifo(M_file, obj_clearing_again_third, amount_trd_sum_new, path_complex_third, i, obj_clearing_again_third.addrs_trk, obj_clearing_again_third.lives_trk_updated, idx_long_or_short, diff_newtrdamount, obj_clearing_again_third.lives_trk_updated, path_counting, counting_netted_third)
	
						if counting_netted_third != 0:
							path_complex_two = append_fromlist_tolist(path_complex_third, path_complex_two)
	
						print('.................................................................')

					break
				
	return M_file, path_complex_two, counting_netted

def average_posincreased(average_posincr, trade_amount, amounts_forthepath):

	if len(average_posincr) > 1:

		column_prices = []
		column_src = []
		column_trk = []
		column_status_src = []
		column_status_trk = []
		column_idx = []
		divider = 0
		for row in average_posincr:
			divider += row[6]
			column_prices.append(int(row[7]))
			column_src.append(row[0])
			column_trk.append(row[3])
			column_status_src.append(row[2])
			column_status_trk.append(row[5])
			column_idx.append(row[8])

		residue = 0

		if divider >= trade_amount:
			col_amounts = [int((float(row[6])/divider)*trade_amount)
						   for row in average_posincr]
			residue = abs(trade_amount - np.sum(col_amounts))
			print("\ndivider: ", divider,
				  " >= obj_ni.amount_trdi :", trade_amount)
			print('\ncol_amounts: ', col_amounts, ", amount_trdi - sum(col_amounts): ", trade_amount - np.sum(col_amounts),
				  ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts))
		else:
			col_amounts = [row[6] for row in average_posincr]
			residue = 0
			print("\ndivider: ", divider, " < obj_ni.amount_trdi: ", trade_amount)
			print('\ncol_amounts: ', col_amounts, ", divider - sum(col_amounts): ", divider - np.sum(col_amounts),
				  ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts))

		k = 0
		for j in range(len(average_posincr)):
			k += 1
			averaged_amount = col_amounts[k-1]+residue if k == np.argmax(
				col_amounts)+1 else col_amounts[k-1]
			prices_intheloop = column_prices[k-1]
			src_intheloop = column_src[k-1]
			trk_intheloop = column_trk[k-1]
			status_src_inloop = column_status_src[k-1]
			status_trk_inloop = column_status_trk[k-1]
			idx = column_idx[k-1]
			amounts_forthepath.append([averaged_amount, prices_intheloop, src_intheloop,
									   trk_intheloop, status_src_inloop, status_trk_inloop, idx])

	return amounts_forthepath


def average_posincreased_float(average_posincr, trade_amount, amounts_forthepath, M_file, addrs_trk_arg, col_amounts, q, amount_inthepath, balance_increasing, amount_trd_begining, scalar_w, total_trdsincr):

	if len(average_posincr) > 1:

		column_prices = []
		column_src = []
		column_trk = []
		column_status_src = []
		column_status_trk = []
		column_idx = []
		
		divider = 0
		for row in average_posincr:
			divider += row[6]
			column_prices.append(int(row[7]))
			column_src.append(row[0])
			column_trk.append(row[3])
			column_status_src.append(row[2])
			column_status_trk.append(row[5])
			column_idx.append(row[8])

		trade_amount_w = (float(scalar_w)/total_trdsincr)*trade_amount if amount_inthepath < trade_amount else trade_amount    
		col_amounts = [(float(trade_amount_w)/divider)*row[6] for row in average_posincr]
						
		k = 0
		for j in range(len(average_posincr)):

			k += 1
			averaged_amount = col_amounts[k-1]
			prices_intheloop = column_prices[k-1]
			src_intheloop = column_src[k-1]
			trk_intheloop = column_trk[k-1]
			status_src_inloop = column_status_src[k-1]
			status_trk_inloop = column_status_trk[k-1]
			idx = column_idx[k-1]
			
			amounts_forthepath.append([averaged_amount, prices_intheloop, src_intheloop, trk_intheloop, status_src_inloop, status_trk_inloop, idx])

	return amounts_forthepath, col_amounts, M_file

def remove_duplicate_rows_json(path_complex_two, new_l):

	seen = set()
	for d in path_complex_two:
		t = tuple(d.items())
		if t not in seen:
			np.array(seen.add(t))
			np.array(new_l.append(d))

	return new_l

def append_fromlist_tolist(path_complex_two_long, path_complex_two):

	for row in path_complex_two_long:
		path_complex_two.append(row)

	return path_complex_two


def PNL_function(entry_price, exit_price, amount_closed, obj_addressj):

	PNL = 0

	if 'Long' in obj_addressj.status_trk:
		PNL = amount_closed*(1/float(entry_price)-1/float(exit_price))

	elif 'Short' in obj_addressj.status_trk:
		PNL = amount_closed*(1/float(exit_price)-1/float(entry_price))

	return PNL

def column(matrix, i):

	return [row[i] for row in matrix]


def negative_for_short(m):

	for row in m:
		if "Short" in str(row[1]):
			row[2] = -row[2]
			if "Short" in str(row[4]):
				row[5] = -row[5]
		elif "Short" in str(row[4]):
			row[5] = - row[5]
			if "Short" in str(row[1]):
				row[2] = -row[2]
	return m


def updating_lasttwocols_fromdatabase(addrs, N_filei, M_file, i, live_updated):

	if addrs == N_filei[0]:
		M_file[:][i][8] = live_updated
	else:
		M_file[:][i][9] = live_updated

	return M_file

def long_short_incr_path(traded_pos_incr, obj_trk_inloop, path_complex_two, index_init, i, path_counting):

	print("Checking weighted average index and the row where were opened the contracts:\n")
	for rows in traded_pos_incr:
		print("rows[-1] = ", rows[-1], "index_init = ", index_init)
		if rows[-1] == index_init:
			path_complex_value_ele_two = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src, obj_trk_inloop.status_trk, rows[1], obj_trk_inloop.matched_price, 0, 0, rows[0], i, path_counting]
			path_complex_ele_two = OrderedDict(zip(globales.key_path, path_complex_value_ele_two))
			path_complex_two.append(path_complex_ele_two)
		else:
			continue

	return path_complex_two

def reverseiterator_incr_pos(index_init, addrs_trk_arg, average_incr, diff_newtrdamount):

	for j in range(index_init, 0, -1):
		M_filej = globales.database_matrix[:][j-1]
		obj_trk_inloop = status_amounts_open_incr_pos(M_filej, addrs_trk_arg)
		if addrs_trk_arg in M_filej:
			if obj_trk_inloop.status_trk in globales.open_incr_long_short:
				row_path = [obj_trk_inloop.addrs_src, obj_trk_inloop.lives_src, obj_trk_inloop.status_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.lives_trk, obj_trk_inloop.status_trk, obj_trk_inloop.amount_trd, obj_trk_inloop.matched_price, j-1]
				average_incr.insert(0, row_path)
			else:
				break
		else:
			continue

	return average_incr

def boolean_for_netted_status(obj_trk_inloop):

	bool_netted_status_long = True if obj_trk_inloop.status_trk in globales.netted_status_long else False
	bool_netted_status_short = True if obj_trk_inloop.status_trk in globales.netted_status_short else False
	bool_netted_status = bool_netted_status_long or bool_netted_status_short

	return bool_netted_status


def livecontracts_byaddress(addrs_trk_arg, index_init, opened_init, numberof_lives_contracts_byaddress):

	numberof_lives_contracts_byaddress = tracking_lastlive_byaddrs(globales.database_matrix, index_init, opened_init, addrs_trk_arg, numberof_lives_contracts_byaddress)

	return numberof_lives_contracts_byaddress

def tracking_lastlive_byaddrs(K_file, index_init, opened_init, addrs_trk_arg, numberof_lives_contracts_byaddress):

	counting = 0
	for j in range(index_init+1, len(K_file)):
		K_filej = K_file[:][j]
		obj_stillopened = status_for_contracts_stillopened(
			K_filej, addrs_trk_arg)
		if addrs_trk_arg in K_filej and obj_stillopened.status_trk in globales.all_netted_status:
			counting += K_filej[6]
			if opened_init - counting >= 0:
				print("\nTrade amount: ", K_filej[6], ", Sum amount trade: ", counting,
					  ", Opened contracts: ", opened_init, ", Lives contracts: ", obj_stillopened.lives_trk)
				numberof_lives_contracts_byaddress = obj_stillopened.lives_trk
			else:
				break

	return numberof_lives_contracts_byaddress

def suming_opened_contracts(path_complex_two_matrix, sum_opened_sett):

	# Make the respective change with: lives_src and lives_trk
	for row_out in path_complex_two_matrix:
		for row_ins in row_out:
			if 'Edge Source' not in str(row_ins) and row_ins['opened_sett'] != 0:
				sum_opened_sett += row_ins['opened_sett']

	return sum_opened_sett


def difference_bet_openedandnetted(M_file, opened_atsett):

	sum_for_opening = 0
	for row in M_file:
		if row[1] in globales.open_incr_long_short and row[4] in globales.open_incr_long_short:
			sum_for_opening += row[6]

	sum_for_netting = 0
	for row in M_file:
		if row[1] in globales.all_netted_status and row[4] in globales.all_netted_status:
			sum_for_netting += row[6]

	opened_atsett = sum_for_opening-sum_for_netting

	return opened_atsett

def total_balance_incr(average_incr, balance_increasing, last_balance, addrs_trk_arg):
	
	for row in average_incr:
		last_balance = row[1] if addrs_trk_arg == row[0] else row[4]
		balance_increasing += row[6]

	return balance_increasing, last_balance

def lookingforlives_insidepath(j, path_complex_main, status_src, addrs_src, amount_trd, N_file):            

	netted_status_trk = globales.netted_status_long if status_src in globales.open_incr_long else globales.netted_status_short
	if status_src in globales.open_incr_long_short:
		
		howmany_closed = 0
		index_i = 0
		count_netted = 0
		for i in range(j+1, len(path_complex_main)):
			if addrs_src == path_complex_main[i]['addrs_trk'] and path_complex_main[i]['status_trk'] in netted_status_trk:
				count_netted += 1
				index_i = i
				howmany_closed += path_complex_main[i]['amount_trd']
				path_complex_main = lookingforlives_insidepath(i, path_complex_main, path_complex_main[i]['status_src'], path_complex_main[i]['addrs_src'], path_complex_main[i]['amount_trd'], N_file)
				
		if count_netted == 0:
			path_complex_main[j]['lives_src'] = float("{0:.2f}".format(path_complex_main[j]['amount_trd']))
		elif count_netted != 0 and howmany_closed != 0:
			path_complex_main[index_i]['lives_trk'] = float("{0:.2f}".format(amount_trd-howmany_closed))
		
	return path_complex_main

def calculating_livescontracts(path_complex_main, single_path_value_ele, N_file):

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
	# Copying lives contracts after netted events #
	for index in range(len(path_complex_main)):
		if index == index_src_first and counter_src != 0:
			path_complex_main[index]['lives_trk'] = float("{0:.2f}".format(abs(openedfor_first_adrrs-sum_amountsfor_src_first)))
		elif index == index_trk_first and counter_trk != 0:
			path_complex_main[index]['lives_trk'] = float("{0:.2f}".format(abs(openedfor_first_adrrs-sum_amountsfor_trk_first)))

	print("counter_src: ", counter_src, "counter_trk", counter_trk)            
	path_complex_main.insert(0, single_path_value_ele)
	# Just in case the tracked address never netted #
	if counter_trk == 0:
		path_complex_main[0]['lives_trk'] = openedfor_first_adrrs
	elif counter_src == 0:
		path_complex_main[0]['lives_src'] = openedfor_first_adrrs

	return path_complex_main

def checking_zeronetted_bypath(path_complex_main):

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

	return contracts_opened, contracts_closed, contracts_lives, path_complex_main

def warning_ifthereisno_zeronetted(path_complex_main, contracts_opened, contracts_closed, contracts_lives):

	if len(path_complex_main)!=0:
		if abs(float("{0:.1f}".format((2*contracts_opened - contracts_closed)-contracts_lives))) != 0:
			k += 1
			print(k, ": Warning!! There is no zero netted event in the path")
		print('\n...........................................................')
		print("\nPath:\n", np.array(path_complex_main), "\n")
		print('...........................................................\n')

def checking_pathcontaining_livesnonzero(path_complex_main, sum_oflives, exit_price_desired, PNL_total, gamma_p, gamma_q):

	if len(path_complex_main) != 0:

		for row in path_complex_main:
			sum_oflives += abs(row['lives_src']) + abs(row['lives_trk'])

		if sum_oflives != 0:
			print("This path has ", sum_oflives, " lives contracts !!", "Edge source: ", path_complex_main[0]['edge_row'])
			exit_price_desired, PNL_total, gamma_p, gamma_q = calculate_pnltrk_bypath(path_complex_main, exit_price_desired, PNL_total, gamma_p, gamma_q)
		else:
			print("\nThis path does not have lives contracts!!", "Edge source: ", path_complex_main[0]['edge_row'])
			print('\nPath:\n', path_complex_main)

	return sum_oflives, exit_price_desired, PNL_total, gamma_p, gamma_q

def listof_addresses_bypath(path_complex_main):

	list_address_inthepath = []
	for row in path_complex_main:
		if row['addrs_src'] not in list_address_inthepath:
			list_address_inthepath.append(row['addrs_src'])
		if row['addrs_trk'] not in list_address_inthepath:
			list_address_inthepath.append(row['addrs_trk'])

	return list_address_inthepath

def calculate_pnltrk_bypath(path_complex_main, exit_price_desired, PNL_trk, gamma_p, gamma_q):

	list_address_inthepath = listof_addresses_bypath(path_complex_main)
	total_pnl_zerolives = 0
	sum_alpha_beta_i = 0
	sum_alpha_beta_j = 0 
	sum_alpha_i = 0
	sum_alpha_j = 0 
	exit_price_desired = 0
	addressj_pnlinthepathv = []

	for j in range(len(list_address_inthepath)):
		
		addressj = list_address_inthepath[j]

		# Here is calculated the PNL when an exit price exists #
		for row in path_complex_main:
			if addressj == row['addrs_trk']:
				obj_addressj = status_for_contracts_stillopened(globales.database_matrix[:][row['edge_row']], addressj)
				print('\nAddress for PNL in the path!!: ', addressj, ' status: ', obj_addressj.status_trk, '\nentry price = ', row['entry_price'], ' exit price = ', row['exit_price'], ' amount_trd = ', row['amount_trd'], ' edge_row = ', row['edge_row'])
				addressj_pnlinthepathv.append(addressj)
				PNL_trk = PNL_function(row['entry_price'], row['exit_price'], row['amount_trd'], obj_addressj)
				total_pnl_zerolives += PNL_trk
				print('PNL_trk for: ', addressj, " = ", PNL_trk)

		# Here are counted the addresses with lives contracts #
		for row in path_complex_main:
			if addressj == row['addrs_src'] and row['addrs_src'] not in addressj_pnlinthepathv:
				obj_addressj = status_for_contracts_stillopened(globales.database_matrix[:][row['edge_row']], addressj)
				if "Netted" not in obj_addressj.status_trk:
					print('\nAddress for PNL in the settlement!!: ', addressj, ' status: ', obj_addressj.status_trk, '\nRow = ', row, '\namount_trd = ', row['amount_trd'], 'entry_price = ', obj_addressj.matched_price)
					if "Long" in obj_addressj.status_trk:
						sum_alpha_beta_i += float(row['amount_trd'])*obj_addressj.matched_price
						sum_alpha_i += row['amount_trd']
					if "Short" in obj_addressj.status_trk:
						sum_alpha_beta_j += float(row['amount_trd'])*obj_addressj.matched_price
						sum_alpha_j += row['amount_trd']

	if sum_alpha_i != sum_alpha_j:
		exit_price_desired = float(total_pnl_zerolives - sum_alpha_beta_i + sum_alpha_beta_j)/(sum_alpha_j - sum_alpha_i)
		gamma_p = total_pnl_zerolives - sum_alpha_beta_i + sum_alpha_beta_j
		gamma_q = sum_alpha_j - sum_alpha_i

	print("\nExit price in this path = ", exit_price_desired)
	print('\nThis path has PNL = ', total_pnl_zerolives)

	return exit_price_desired, PNL_trk, gamma_p, gamma_q

def obtaining_arraysfor_liveslongshort(path_complex_main, listof_longlives, listof_shortlives):

	if len(path_complex_main) != 0:

		for row in path_complex_main:
			if row['lives_src'] != 0:
				if row['status_src'] in globales.open_incr_long:
					new_edge_forsett = [row['addrs_src'], row['status_src'], row['lives_src'], row['exit_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_longlives: 
						listof_longlives.append(new_edge_forsett_ele)
				elif row['status_src'] in globales.netted_status_long:
					new_edge_forsett = [row['addrs_src'], row['status_src'], row['lives_src'], row['entry_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_longlives: 
						listof_longlives.append(new_edge_forsett_ele)				
				if row['status_src'] in globales.open_incr_short:
					new_edge_forsett = [row['addrs_src'], row['status_src'], row['lives_src'], row['exit_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_shortlives: 
						listof_shortlives.append(new_edge_forsett_ele)
				elif row['status_src'] in globales.netted_status_short:
					new_edge_forsett = [row['addrs_src'], row['status_src'], row['lives_src'], row['entry_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_shortlives: 
						listof_shortlives.append(new_edge_forsett_ele)

			if row['lives_trk'] != 0:

				if row['status_trk'] in globales.open_incr_long:
					new_edge_forsett = [row['addrs_trk'], row['status_trk'], row['lives_trk'], row['exit_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_longlives: 
						listof_longlives.append(new_edge_forsett_ele)
				elif row['status_trk'] in globales.netted_status_long:
					new_edge_forsett = [row['addrs_trk'], row['status_trk'], row['lives_trk'], row['entry_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_longlives: 
						listof_longlives.append(new_edge_forsett_ele)
				
				if row['status_trk'] in globales.open_incr_short:
					new_edge_forsett = [row['addrs_trk'], row['status_trk'], row['lives_trk'], row['exit_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_shortlives: 
						listof_shortlives.append(new_edge_forsett_ele)
				elif row['status_trk'] in globales.netted_status_short:
					new_edge_forsett = [row['addrs_trk'], row['status_trk'], row['lives_trk'], row['entry_price'], row['edge_row'], row['path_number']]
					new_edge_forsett_ele = OrderedDict(zip(globales.lives_data, new_edge_forsett))
					if new_edge_forsett_ele not in listof_shortlives: 
						listof_shortlives.append(new_edge_forsett_ele)

	return listof_longlives, listof_shortlives

def counting_livesfor_longshort(listof_longlives, listof_shortlives):

	sumof_lives_longs = 0
	for row in listof_longlives:
		sumof_lives_longs += row['lives_contracts']

	sumof_lives_shorts = 0
	for row in listof_shortlives:
		sumof_lives_shorts += row['lives_contracts']
	
	return sumof_lives_longs, sumof_lives_shorts

def calculating_ghost_edges(listof_longlives, listof_shortlives, price_settlement_union, ghost_edges_array):

	index_start = 0

	for i in range(len(listof_longlives)):

		row_longi = listof_longlives[i]
		amountj_long = row_longi['lives_contracts']
		sum_amountj_short = 0

		print('index_start = ', index_start)

		for j in range(index_start, len(listof_shortlives)):

			row_shortj = listof_shortlives[j]
			print('\nrow_shortj:\n', row_shortj, len(listof_shortlives))

			amountj_short = row_shortj['lives_contracts']
			sum_amountj_short += amountj_short

			if amountj_long >= sum_amountj_short:
		
				ghost_node_ele_path_long_one = [row_shortj['address'], row_longi['address'], row_shortj['status'], row_longi['status'], row_longi['entry_price'], price_settlement_union, 0, 0, row_shortj['lives_contracts'], row_longi['edge_row'], row_longi['path_number']]
				ghost_node_path_long_one = OrderedDict(zip(globales.key_path, ghost_node_ele_path_long_one))
		
				ghost_node_ele_path_short_one = [row_longi['address'], row_shortj['address'], row_longi['status'], row_shortj['status'], row_shortj['entry_price'], price_settlement_union, 0, 0, row_shortj['lives_contracts'], row_shortj['edge_row'], row_shortj['path_number']]
				ghost_node_path_short_one = OrderedDict(zip(globales.key_path, ghost_node_ele_path_short_one))
		
				print('\nghost_node_path_long_one', ghost_node_path_long_one)
				print('\nghost_node_path_short_one', ghost_node_path_short_one)

				ghost_edges_array.append(ghost_node_path_long_one)
				ghost_edges_array.append(ghost_node_path_short_one)

			else:

				difference_amounts = sum_amountj_short - amountj_long

				ghost_node_ele_path_long_two = [row_shortj['address'], row_longi['address'], row_shortj['status'], row_longi['status'], row_longi['entry_price'], price_settlement_union, 0, 0, row_longi['lives_contracts'], row_longi['edge_row'], row_longi['path_number']]
				ghost_node_path_long_two = OrderedDict(zip(globales.key_path, ghost_node_ele_path_long_two))
		
				ghost_node_ele_path_short_two = [row_longi['address'], row_shortj['address'], row_longi['status'], row_shortj['status'], row_shortj['entry_price'], price_settlement_union, 0, 0, row_longi['lives_contracts'], row_shortj['edge_row'], row_shortj['path_number']]
				ghost_node_path_short_two = OrderedDict(zip(globales.key_path, ghost_node_ele_path_short_two))

				index_start = j
				listof_shortlives[j]['lives_contracts'] = difference_amounts
		
				print('\nghost_node_path_long_two', ghost_node_path_long_two)
				print('\nghost_node_path_short_two', ghost_node_path_short_two)

				ghost_edges_array.append(ghost_node_path_long_two)
				ghost_edges_array.append(ghost_node_path_short_two)

				break

	return ghost_edges_array

def updating_lives_inthepath(path_complex_two_matrix):

	for row_bypath in path_complex_two_matrix:
		for row_inpath in row_bypath:
			row_inpath['lives_src'] = 0
			row_inpath['lives_trk'] = 0

	return path_complex_two_matrix

def joining_pathclear_ghostpath(path_complex_main, ghost_edges_array):

	for row_path in path_complex_main:
		index_path = row_path[0]['path_number']
		for row_ghost in ghost_edges_array:
			if row_ghost['path_number'] == index_path:
				row_path.append(row_ghost)

	return path_complex_main

def checking_zeronetted_bypath_withghostedges(path_complex_main):

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

	return contracts_opened, contracts_closed, contracts_lives, path_complex_main