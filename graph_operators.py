#! /usr/bin/python

import numpy as np
import globales
import setglobales
from amount_status import*
from collections import*

globales.init()
setglobales.stuff()

def clearing_operator(M_file, obj_trk, amount_trd_sum, path_complex_two, idx_i, index_init, addrs_trk_arg, amount_trd_begining):

    average_long_incr  = []
    average_short_incr = []

    average_long_incr.append([obj_trk.addrs_src, obj_trk.lives_src, obj_trk.status_src, obj_trk.addrs_trk, obj_trk.lives_trk, obj_trk.status_trk, obj_trk.amount_trd, obj_trk.matched_price, 0])
    average_short_incr.append([obj_trk.addrs_src, obj_trk.lives_src, obj_trk.status_src, obj_trk.addrs_trk, obj_trk.lives_trk, obj_trk.status_trk, obj_trk.amount_trd, obj_trk.matched_price, 0])

    for i in xrange(index_init+1, len(M_file)):                

        N_filei = M_file[:][i]

        obj_trk_inloop = status_amounts_inloop(N_filei, addrs_trk_arg)

        average_long_incr, average_short_incr, i, M_file = vector_incr_averages(addrs_trk_arg, N_filei, obj_trk_inloop, average_long_incr, average_short_incr, M_file, i)

        amount_trd_sum_new = 0

        bool_netted_status_long  = True if obj_trk_inloop.status_trki in globales.netted_status_long  else False
        bool_netted_status_short = True if obj_trk_inloop.status_trki in globales.netted_status_short else False

        bool_netted_status = bool_netted_status_long or bool_netted_status_short
        bool_lasttwo_cols = N_filei[8] != 0 or N_filei[9] != 0

        traded_long_incr  = []
        traded_short_incr = []
        if addrs_trk_arg in N_filei and bool_netted_status and bool_lasttwo_cols:

            traded_long_incr  = average_posincreased(average_long_incr,  obj_trk_inloop.amount_trdi, traded_long_incr)
            traded_short_incr = average_posincreased(average_short_incr, obj_trk_inloop.amount_trdi, traded_short_incr)
 
            print "\n"
            for trade_amount_incr in traded_short_incr:
                print "Checking traded_short_incr!!: ", trade_amount_incr
                entry_price_increase = trade_amount_incr
                path_complex_value_ele_two = [obj_trk_inloop.addrs_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_srci, obj_trk_inloop.status_trki, entry_price_increase, obj_trk_inloop.matched_pricei, trade_amount_incr, 0]
                path_complex_ele_two = OrderedDict(zip(globales.key_path, path_complex_value_ele_two))
                path_complex_two.append(path_complex_ele_two)

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + obj_trk_inloop.amount_trdi
            amount_trd_sum_l = amount_trd_sum

            print "\n------------------------------------------------------\n"
            print "\nAmounts!!!\namount_trdi: ", obj_trk_inloop.amount_trdi, " | lives_trki: ", obj_trk_inloop.lives_trki, " | lives_srci: ", obj_trk_inloop.lives_srci, " | amount_trd_sum: ", amount_trd_sum
            bool_src = obj_trk_inloop.status_srci in globales.open_incr_long or obj_trk_inloop.status_srci in globales.open_incr_short 

            d_amounts = obj_trk.amount_trd - amount_trd_sum
            if d_amounts > 0:

                M_file = update_lasttwo_columns(addrs_trk_arg, obj_trk_inloop, N_filei, M_file, i)                
                print "Opened contrats: ", obj_trk.amount_trd, " > Sum amounts traded: ", amount_trd_sum, "\n"
                print "'iteration: ", i, "|addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"

                path_complex_value_ele_two = [obj_trk_inloop.addrs_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_srci, obj_trk_inloop.status_trki, obj_trk.matched_price, obj_trk_inloop.matched_pricei, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_two = OrderedDict(zip(globales.key_path, path_complex_value_ele_two))
                        
                path_complex_two.append(path_complex_ele_two)
                idx_i.append(i)

                if bool_src:
                    print "Looking for New Path for: ", obj_trk_inloop.addrs_srci, "!!\n"
                    M_file, idx_i, path_complex_two = clearing_operator(M_file, obj_trk, amount_trd_sum_new, path_complex_two, idx_i, i, obj_trk_inloop.addrs_srci, obj_trk_inloop.amount_trdi)

                continue

            elif d_amounts < 0:

                M_file = update_lasttwo_columns(addrs_trk_arg, obj_trk_inloop, N_filei, M_file, i)
                diff_r = abs(obj_trk.amount_trd - amount_trd_sum_b)
                print "Opened contrats: ", obj_trk.amount_trd, " < Sum amounts traded: ", amount_trd_sum, "diff_r: ", diff_r, "\n"

                print "amount_trdi", obj_trk_inloop.amount_trdi, "lives_trki", obj_trk_inloop.lives_trki, "status_trki: ", obj_trk_inloop.status_trki
                x_src = abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_srci) if "Long" in str(obj_trk_inloop.status_srci) else -abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_srci)
                y_trk = abs(obj_trk_inloop.amount_trdi + obj_trk_inloop.lives_trki) if "Long" in str(obj_trk_inloop.status_trki) else -abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk

                path_complex_ele_value_thr = [obj_trk_inloop.addrs_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_srci, obj_trk_inloop.status_trki, obj_trk.matched_price, obj_trk_inloop.matched_pricei, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_thr = OrderedDict(zip(globales.key_path, path_complex_ele_value_thr))

                path_complex_two.append(path_complex_ele_thr)
                print "'iteration: ", i, "|addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"
                print "\n------------------------------------------------------\n"
                idx_i.append(i)

                amount_new = abs(diff_r - obj_trk_inloop.amount_trdi)
                new_row = np.array([obj_trk_inloop.addrs_srci, obj_trk_inloop.status_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_trki, obj_trk_inloop.lives_trki, amount_new, obj_trk_inloop.matched_pricei, obj_trk_inloop.lives_srci, obj_trk_inloop.lives_trki])
                M_file = np.delete(M_file, i, 0)
                M_file = np.insert(M_file, i, new_row, 0)

                bool_new_row = True
                print "\nNew row added: ", new_row, "\n"

                if bool_src:                            
                    print "Looking for New Path for: ", obj_trk_inloop.addrs_srci, "!!\n"
                    M_file, idx_i, path_complex_two = clearing_operator(M_file, obj_trk, amount_trd_sum_new, path_complex_two, idx_i, i, obj_trk_inloop.addrs_srci, obj_trk_inloop.amount_trdi)
                else:
                    break

                break
                    
            elif d_amounts == 0:

                M_file = update_lasttwo_columns(addrs_trk_arg, obj_trk_inloop, N_filei, M_file, i)
                print "Opened contrats: ", obj_trk.amount_trd, " = Sum amounts traded: ", amount_trd_sum, "\n"
                print "'iteration: ", i, " |addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"

                path_complex_ele_value_one = [obj_trk_inloop.addrs_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_srci, obj_trk_inloop.status_trki, obj_trk.matched_price, obj_trk_inloop.matched_pricei, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_one = OrderedDict(zip(globales.key_path, path_complex_ele_value_one))                      

                path_complex_two.append(path_complex_ele_one)

                idx_i.append(i)

                if bool_src:
                    print "Looking for New Path for: ", obj_trk_inloop.addrs_srci, "!!\n"
                    M_file, idx_i, path_complex_two = clearing_operator(M_file, obj_trk, amount_trd_sum_new, path_complex_two, idx_i, i, obj_trk_inloop.addrs_srci, obj_trk_inloop.amount_trdi)                    

                break
                                
    total_amount_trade = 0

    for j in range(len(path_complex_two)):                          
        total_amount_trade += int(path_complex_two[j]['amount_trd'])

    total_next_trades = abs(amount_trd_begining - total_amount_trade)
    contracts_opened_sett = abs(amount_trd_begining - total_next_trades)

    if total_next_trades < amount_trd_begining and total_next_trades != 0:
        path_complex_two[0]['opened_sett']=contracts_opened_sett

    return M_file, list(set(idx_i)), path_complex_two

def average_posincreased(average_posincr, trade_amount, amounts_forthepath):

    if len(average_posincr) > 1:
            
        divider = 0
        for row in average_posincr:
            divider += row[6]
                        
        residue = 0
        if divider >= trade_amount:
            col_amounts = [int((float(row[6])/divider)*trade_amount) for row in average_posincr]
            residue = abs(trade_amount - np.sum(col_amounts))
            print "\ndivider: ", divider, " >= obj_ni.amount_trdi: ", trade_amount                           
            print '\ncol_amounts: ', col_amounts, ", amount_trdi - sum(col_amounts): ", trade_amount - np.sum(col_amounts), ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts)
        else:
            col_amounts = [row[6] for row in average_posincr]
            residue = 0
            print "\ndivider: ", divider, " < obj_ni.amount_trdi: ", trade_amount                            
            print '\ncol_amounts: ', col_amounts, ", divider - sum(col_amounts): ", divider - np.sum(col_amounts), ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts)

        k = 0
        for j in range(len(average_posincr)):
            k += 1
            averaged_amount = col_amounts[k-1]+residue if k == np.argmax(col_amounts)+1 else col_amounts[k-1]
            amounts_forthepath.append(averaged_amount)            

        print "\naverage_posincr:\n", np.array(amounts_forthepath)

    return amounts_forthepath

def vector_pos_incr(obj_trk, obj_trk_inloop, N_filei, average_incr, i):

    if obj_trk in N_filei and obj_trk_inloop.status_trki in globales.incr_positions:
        average_incr.append([obj_trk_inloop.addrs_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.status_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.status_trki, obj_trk_inloop.amount_trdi, obj_trk_inloop.matched_pricei, i])

    return average_incr, i

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

def PNL_function(entry_price, exit_price, amount_closed):

    return amount_closed*(1/float(entry_price)-1/float(exit_price))

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
                row[2]=-row[2]
    return m                

def id_newrow(idx_k, idx_i):
    
    if len(idx_k) != 0:
        for l in range(len(idx_k)):
            idx_i.append(idx_k[l])

    return idx_i

def first_single_path(m):

    bool_single_path = False
    single_path = []

    if len(m) == 1:
        
        single_path_value_begin = [m[0][0], m[0][3], m[0][1], m[0][4], m[0][7], 0, m[0][6], m[0][6]]
        single_path_begin = OrderedDict(zip(globales.key_path,single_path_value_begin))
        single_path.append(single_path_begin)

        bool_single_path = True

    return bool_single_path, single_path

def update_lasttwo_columns(addrs_trk_arg, obj_trk_inloop, N_filei, M_file, i):

    if addrs_trk_arg in N_filei and obj_trk_inloop.status_trki in globales.netted_status_short:

        M_file[:][i][8] = 0

    elif addrs_trk_arg in N_filei and obj_trk_inloop.status_trki in  globales.netted_status_long:

        M_file[:][i][9] = 0

    return M_file

def vector_incr_averages(addrs_trk_arg, N_filei, obj_trk_inloop, average_long_incr, average_short_incr, M_file, i):

    if addrs_trk_arg in N_filei and obj_trk_inloop.status_trki == "LongPosIncreased":

        M_file[:][i][8] = 0
        average_long_incr, i  = vector_pos_incr(addrs_trk_arg, obj_trk_inloop, N_filei, average_long_incr, i) 

    elif addrs_trk_arg in N_filei and obj_trk_inloop.status_trki == "ShortPosIncreased":

        M_file[:][i][9] = 0
        average_short_incr, i = vector_pos_incr(addrs_trk_arg, obj_trk_inloop, N_filei, average_short_incr, i)

    return average_long_incr, average_short_incr, i, M_file