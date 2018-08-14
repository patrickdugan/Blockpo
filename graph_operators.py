#! /usr/bin/python

import numpy as np
import globales
import json
import setglobales
from amount_status import*
from collections import*

globales.init()
setglobales.stuff()


def clearing_operator(M_file, obj_trk, amount_trd_sum, path_complex_two, idx_i, index_init, addrs_trk_arg, amount_trd_begining, index_long_short, diff_newtrdamount):

    average_incr = []
    average_incr.append([obj_trk.addrs_src, obj_trk.lives_src, obj_trk.status_src, obj_trk.addrs_trk,
                         obj_trk.lives_trk, obj_trk.status_trk, amount_trd_begining, obj_trk.matched_price, index_init])

    howmany_netted = 0
    numberof_lives_contracts_byaddress = 0

    howmany_netted, average_incr = howmany_netted_events_and_vectorwithincrs(
        howmany_netted, index_init, M_file, addrs_trk_arg, index_long_short, average_incr, diff_newtrdamount)
    print("\nChecking here average_incr:\n", np.array(
        average_incr), "\nlen(average_incr): ", len(average_incr))

    balance_increasing = 0
    balance_increasing = total_balance_incr(average_incr, balance_increasing)

    numberof_lives_contracts_byaddress = livecontracts_byaddress(
        addrs_trk_arg, index_init, balance_increasing, numberof_lives_contracts_byaddress)

    balance_incr = 0
    amount_selected = 0

    path_complex_first = []
    path_complex_third = []
    path_complex_fourh = []
    path_complex_incrs = []

    many_netted_incr = 0
    for i in range(index_init+1, len(M_file)):

        N_filei = M_file[:][i]

        obj_trk_inloop = status_amounts_inloop(N_filei, addrs_trk_arg)
        amount_trd_sum_new = 0
        bool_netted_status = boolean_for_netted_status(obj_trk_inloop)

        traded_position_incr = []

        if addrs_trk_arg in N_filei and bool_netted_status:

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum = amount_trd_sum + obj_trk_inloop.amount_trd
            amount_trd_sum_l = amount_trd_sum

            if len(average_incr) > 1:

                balance_incr += obj_trk_inloop.amount_trd

                if balance_incr <= balance_increasing:

                    print("\nChecking balance_incr: ", balance_incr)
                    print("\nNew netted event for ", addrs_trk_arg,
                          " in the row: ", i+1, "!!")
                    M_file = update_lasttwo_columns(
                        obj_trk_inloop, N_filei, M_file, i)
                    print("\nRow update:\n", M_file[:][i])
                    amount_selected = obj_trk_inloop.amount_trd
                    print("\namount_selected", amount_selected,
                          ", diff_newtrdamount", diff_newtrdamount)
                    traded_position_incr = average_posincreased_float(
                        average_incr, amount_selected, traded_position_incr)
                    print("\nbalance_incr: ", balance_incr,
                          "<= opened contracts: ", balance_increasing)
                    print("\ntraded_position_incr:\n",
                          np.array(traded_position_incr))
                    print("\nChecking the index_init: ", index_init)
                    path_complex_fourh = long_short_incr_path(
                        traded_position_incr, obj_trk_inloop, path_complex_fourh, index_init, i)

                    many_netted_incr += path_complex_fourh[-1]['amount_trd']

                    print("many_netted_incr!!: ", many_netted_incr)
                    print("\npath_complex_fourh\n",
                          np.array(path_complex_fourh))
                    print("obj_trk.amount_trd: ", obj_trk.amount_trd)
                    print("\n------------------------------------------------------\n")
                    # path_complex_fourh[0]['opened_sett']=obj_trk.amount_trd-many_netted_incr

                    if path_complex_fourh[-1]['status_src'] in globales.open_incr_long_short:

                        print("Can be %s a new source??" %
                              (path_complex_fourh[-1]['addrs_src']))
                        idx_long_or_short = 0 if obj_trk_inloop.status_src in globales.open_incr_long else 1
                        addrs_trk_incr = path_complex_fourh[-1]['addrs_src']
                        obj_trk_incr = status_amounts_inloop(
                            N_filei, addrs_trk_incr)
                        print("\nTrade amount on this edge path: ",
                              path_complex_fourh[-1]['amount_trd'])
                        print("Trade amount on this edge matrix: ",
                              obj_trk_incr.amount_trd)
                        diff_newtrdamount = obj_trk_incr.amount_trd - \
                            path_complex_fourh[-1]['amount_trd']
                        print("diff_newtrdamount: ", diff_newtrdamount)
                        M_file, idx_i, path_complex_incrs = clearing_operator(
                            M_file, obj_trk_incr, amount_trd_sum_new, path_complex_incrs, idx_i, i, addrs_trk_incr, path_complex_fourh[-1]['amount_trd'], idx_long_or_short, diff_newtrdamount)

                    continue

            print("\nAmounts!!!\n\namount_trdi: ", obj_trk_inloop.amount_trd, " | lives_trki: ", obj_trk_inloop.lives_trk,
                  " | lives_srci: ", obj_trk_inloop.lives_src, " | amount_trd_sum: ", amount_trd_sum)
            bool_src = obj_trk_inloop.status_src in globales.open_incr_long or obj_trk_inloop.status_src in globales.open_incr_short

            print("\nhowmany_netted: ", howmany_netted, "\n")
            print("\nNew netted event for ", addrs_trk_arg,
                  " in the row: ", i+1, "!!")

            d_amounts = obj_trk.matched_price - amount_trd_sum
            if d_amounts > 0:

                M_file = update_lasttwo_columns(
                    obj_trk_inloop, N_filei, M_file, i)
                print("\nRow update:\n", M_file[:][i])
                print("\nOpened contrats: ", amount_trd_begining,
                      " > Sum amounts traded: ", amount_trd_sum, "\n")
                print("'iteration: ", i, "|addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trk,  "|amount_trdi: ",
                      obj_trk_inloop.amount_trd, "|addrs_srci: ", obj_trk_inloop.addrs_src, "|status_srci: ", obj_trk_inloop.status_src, "\n")

                path_complex_value_ele_two = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src,
                                              obj_trk_inloop.status_trk, obj_trk.matched_price, obj_trk_inloop.matched_price, 0, 0, obj_trk_inloop.amount_trd]
                path_complex_ele_two = OrderedDict(
                    zip(globales.key_path, path_complex_value_ele_two))

                print("(d_amounts > 0) path_complex_ele_two: \n\n",
                      path_complex_ele_two)

                path_complex_two.append(path_complex_ele_two)
                idx_i.append(i)

                if bool_src:
                    idx_long_or_short = 0 if obj_trk_inloop.status_trk in globales.open_incr_long else 1
                    print("\nLooking New Path for: ",
                          obj_trk_inloop.addrs_src, "!!\n")
                    M_file, idx_i, path_complex_first = clearing_operator(
                        M_file, obj_trk_inloop, amount_trd_sum_new, path_complex_first, idx_i, i, obj_trk_inloop.addrs_src, obj_trk_inloop.amount_trd, idx_long_or_short, diff_newtrdamount)
                continue

            elif d_amounts == 0:

                M_file = update_lasttwo_columns(
                    obj_trk_inloop, N_filei, M_file, i)
                print("\nRow update:\n", M_file[:][i])
                print("\nOpened contrats: ", amount_trd_begining,
                      " = Sum amounts traded: ", amount_trd_sum, "\n")
                print("'iteration: ", i, " |addrs_trk: ", addrs_trk_arg, "|status_trki: ", obj_trk_inloop.status_trk,  "|amount_trdi: ",
                      obj_trk_inloop.amount_trd, "|addrs_srci: ", obj_trk_inloop.addrs_src, "|status_srci: ", obj_trk_inloop.status_src, "\n")

                path_complex_ele_value_one = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src,
                                              obj_trk_inloop.status_trk, obj_trk.matched_price, obj_trk_inloop.matched_price, 0, 0, obj_trk_inloop.amount_trd]
                path_complex_ele_one = OrderedDict(
                    zip(globales.key_path, path_complex_ele_value_one))

                print("(d_amounts = 0) path_complex_ele_one:\n\n",
                      path_complex_ele_one, "\n")

                path_complex_two.append(path_complex_ele_one)

                idx_i.append(i)

                if bool_src:
                    idx_long_or_short = 0 if obj_trk_inloop.status_trk in globales.open_incr_long else 1
                    print("\nLooking New Path for: ",
                          obj_trk_inloop.addrs_src, "!!\n")
                    M_file, idx_i, path_complex_third = clearing_operator(
                        M_file, obj_trk_inloop, amount_trd_sum_new, path_complex_third, idx_i, i, obj_trk_inloop.addrs_src, obj_trk_inloop.amount_trd, idx_long_or_short, diff_newtrdamount)
                break

    path_complex_two = append_fromlist_tolist(
        path_complex_first, path_complex_two)
    path_complex_two = append_fromlist_tolist(
        path_complex_third, path_complex_two)
    path_complex_two = append_fromlist_tolist(
        path_complex_fourh, path_complex_two)
    path_complex_two = append_fromlist_tolist(
        path_complex_incrs, path_complex_two)

    return M_file, list(set(idx_i)), path_complex_two


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


def average_posincreased_float(average_posincr, trade_amount, amounts_forthepath):

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

        col_amounts = [float("{0:.2f}".format(
            (float(row[6])/divider)*trade_amount)) for row in average_posincr]
        print('\ncol_amounts: ', col_amounts)

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
            amounts_forthepath.append([averaged_amount, prices_intheloop, src_intheloop,
                                       trk_intheloop, status_src_inloop, status_trk_inloop, idx])

    return amounts_forthepath


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
                row[2] = -row[2]
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

        single_path_value_begin = [
            m[0][0], m[0][3], m[0][1], m[0][4], m[0][7], 0, m[0][2], m[0][5], m[0][6]]
        single_path_begin = OrderedDict(
            zip(globales.key_path, single_path_value_begin))
        single_path.append(single_path_begin)

        bool_single_path = True

    return bool_single_path, single_path


def update_lasttwo_columns(obj_trk_inloop, N_filei, M_file, i):

    if obj_trk_inloop.addrs_trk in N_filei and obj_trk_inloop.status_trk in globales.all_netted_status:
        M_file = updating_withzero(
            obj_trk_inloop.addrs_trk, N_filei, M_file, i)

    return M_file


def updating_withzero(addrs, N_filei, M_file, i):

    if addrs == N_filei[0]:
        M_file[:][i][8] = 0
    else:
        M_file[:][i][9] = 0

    return M_file


def long_short_incr_path(traded_pos_incr, obj_trk_inloop, path_complex_two, index_init, i):

    for rows in traded_pos_incr:
        if rows[-1] == index_init:
            path_complex_value_ele_two = [obj_trk_inloop.addrs_src, obj_trk_inloop.addrs_trk, obj_trk_inloop.status_src,
                                          obj_trk_inloop.status_trk, rows[1], obj_trk_inloop.matched_price, 0, 0, rows[0]]
            path_complex_ele_two = OrderedDict(
                zip(globales.key_path, path_complex_value_ele_two))
            path_complex_two.append(path_complex_ele_two)
        else:
            continue

    return path_complex_two


def howmany_netted_events_and_vectorwithincrs(howmany_netted, index_init, M_file, addrs_trk_arg, index_long_short, average_incr, diff_newtrdamount):

    for i in range(index_init+1, len(M_file)):

        N_filei = M_file[:][i]

        obj_trk_inloop = status_amounts_inloop(N_filei, addrs_trk_arg)
        average_incr, i = vector_pos_incr(addrs_trk_arg, obj_trk_inloop, N_filei,
                                          average_incr, i, index_init, M_file, index_long_short, diff_newtrdamount)
        bool_netted_status = boolean_for_netted_status(obj_trk_inloop)

        if addrs_trk_arg in N_filei:
            if bool_netted_status:
                howmany_netted += 1

    average_incr = reverseiterator_incr_pos(
        index_init, addrs_trk_arg, average_incr, diff_newtrdamount)

    return howmany_netted, average_incr


def vector_pos_incr(addrs_trk, obj_trk_inloop, N_filei, average_incr, i, index_init, M_file, index_long_short, diff_newtrdamount):

    globales.incr_positions = globales.open_incr_long if index_long_short == 0 else globales.open_incr_short
    if addrs_trk in N_filei and obj_trk_inloop.status_trk in globales.incr_positions:
        print("\nPosition increase in the row: ", i+1, "\tIteration: ", i)
        average_incr.append([obj_trk_inloop.addrs_src, obj_trk_inloop.lives_src, obj_trk_inloop.status_src, obj_trk_inloop.addrs_trk,
                             obj_trk_inloop.lives_trk, obj_trk_inloop.status_trk, obj_trk_inloop.amount_trd, obj_trk_inloop.matched_price, i])

    return average_incr, i


def reverseiterator_incr_pos(index_init, addrs_trk_arg, average_incr, diff_newtrdamount):

    M_file = opening_filetxt("graphInfoSixth.txt")
    for j in range(index_init, 0, -1):
        M_filej = M_file[:][j-1]
        obj_trk_inloop = status_amounts_open_incr_pos(M_filej, addrs_trk_arg)
        if addrs_trk_arg in M_filej and obj_trk_inloop.status_src in globales.open_incr_long_short:
            row_path = [obj_trk_inloop.addrs_src, obj_trk_inloop.lives_src, obj_trk_inloop.status_src, obj_trk_inloop.addrs_trk,
                        obj_trk_inloop.lives_trk, obj_trk_inloop.status_trk, obj_trk_inloop.amount_trd, obj_trk_inloop.matched_price, j-1]
            average_incr.insert(0, row_path)

    return average_incr


def boolean_for_netted_status(obj_trk_inloop):

    bool_netted_status_long = True if obj_trk_inloop.status_trk in globales.netted_status_long else False
    bool_netted_status_short = True if obj_trk_inloop.status_trk in globales.netted_status_short else False
    bool_netted_status = bool_netted_status_long or bool_netted_status_short

    return bool_netted_status


def livecontracts_byaddress(addrs_trk_arg, index_init, opened_init, numberof_lives_contracts_byaddress):

    K_file = opening_filetxt("graphInfoSixth.txt")
    numberof_lives_contracts_byaddress = tracking_lastlive_byaddrs(
        K_file, index_init, opened_init, addrs_trk_arg, numberof_lives_contracts_byaddress)

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


def opening_filetxt(namefile):

    with open(namefile, "r") as file:
        M_file = [[int(digit) if digit.isdigit()
                   else digit for digit in line.split()] for line in file]

    return M_file


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


def total_balance_incr(average_incr, balance_increasing):

    for row in average_incr:
        balance_increasing += row[6]

    return balance_increasing

def lookingforlives_insidepath(j, path_complex_main, status_src, addrs_src, amount_trd):

    if status_src in globales.open_incr_long_short:
        
        howmany_closed = 0
        index_i = 0
        count_netted = 0
        for i in range(j+1, len(path_complex_main)):
            if addrs_src == path_complex_main[i]['addrs_trk'] and path_complex_main[i]['status_trk'] in globales.all_netted_status:
                count_netted += 1
                index_i = i
                howmany_closed += path_complex_main[i]['amount_trd']
                path_complex_main = lookingforlives_insidepath(i, path_complex_main, path_complex_main[i]['status_src'], path_complex_main[i]['addrs_src'], path_complex_main[i]['amount_trd'])

        if count_netted == 0:
            path_complex_main[j]['lives_src'] = float("{0:.2f}".format(path_complex_main[j]['amount_trd']))
        elif count_netted != 0 and howmany_closed != 0:
            path_complex_main[index_i]['lives_trk'] = float("{0:.2f}".format(amount_trd-howmany_closed))

        print("HELLO!! ", path_complex_main[j])
        
    return path_complex_main
