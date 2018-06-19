#! /usr/bin/python

import sys
import numpy as np
from graph_operators import*

import globales
import setglobales

sys.stdout = open('out', 'w')

print "\n#------------------------'Definitions'---------------------------------#\n"            
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\topened or keep open contracts at the end date"

globales.init()
setglobales.stuff()

with open('graphInfoSixth.txt') as file:
    M_file = [[int(digit) if digit.isdigit() else digit for digit in line.split()] for line in file]

print "Negative or Positive:"
M_file = negative_for_short(M_file)

bool_new_row = False
idx_j = [0]
Interval = range(len(M_file))

q = 0
for j in Interval:  

    if j > 0:
        print "------------------------------------------------------"
        print "\nRows deleted: ", idx_j
        M_file = np.delete(M_file, idx_j, 0)

    print "M_file:\n", np.array(M_file), "\nLength M_file: ", len(M_file), "\n"
    bool_M_file, single_path_begin = first_single_path(M_file)

    if bool_M_file:        
        print "###################################################################################\n"
        print "Source: ", M_file[0][3], "; Tracked: ", M_file[0][0], "\n"
        print "Last Single path:\n", np.array(single_path_begin), "\n"
        break

    M_filej = []
    M_filej = M_file[:][0]

    obj_mj = status_amounts(M_filej)
    path_complex_one = []
    path_complex_two = []

    if obj_mj.status_src == "OpenShortPosition" or obj_mj.status_src == "ShortPosIncreased":

        if obj_mj.status_trk == "ShortPosNetted" or obj_mj.status_trk == "ShortPosNettedPartly":
            
            print "###################################################################################\n"
            print "Source: ", obj_mj.addrs_src, "; Tracked: ", obj_mj.addrs_trk, "\n"

            single_path_value_ele = [obj_mj.addrs_src, obj_mj.lives_src, obj_mj.addrs_trk, obj_mj.lives_trk, obj_mj.amount_trd, obj_mj.amount_trd]
            single_path_ele = dict(zip(globales.key_path, single_path_value_ele))

            print "Single path:\n", single_path_ele, "\n"           
            print "Length M_file: ", 1
            idx_j = [0]

            continue

        elif obj_mj.status_trk == "OpenLongPosition" or obj_mj.status_trk == "LongPosIncreased":

            print "###################################################################################\n"
            print "Source: ", obj_mj.addrs_src, "; Tracked: ", obj_mj.addrs_trk, "\n"

            update_lives_src, update_lives_trk = new_lives(obj_mj.status_src, obj_mj.status_trk, obj_mj.amount_trd, obj_mj.lives_src, obj_mj.lives_trk)

            single_path_value_ele = [obj_mj.addrs_src, update_lives_src, obj_mj.addrs_trk, update_lives_trk, obj_mj.amount_trd, 0]
            single_path_ele = dict(zip(globales.key_path, single_path_value_ele))

            path_complex_two.append(single_path_ele)

            idx_iter = 0
            amount_trd_sum = 0
            path_complex_ele_fth = []

            idx_i  = [0]
            average_longincr = []            
            average_longincr.append([obj_mj.addrs_trk, obj_mj.lives_trk, obj_mj.status_trk, obj_mj.amount_trd, 0])

            for i in xrange(1, len(M_file)):                

                N_filei = []
                N_filei = M_file[:][i]
                obj_ni = status_amounts_j(N_filei, obj_mj.addrs_trk)

                lives_srci_new = abs(obj_ni.lives_srci - obj_ni.amount_trdi) if obj_ni.status_srci in str(globales.open_incr_status) else obj_ni.lives_srci

                if obj_mj.addrs_trk in str(N_filei):
                    idx_iter += 1

                if obj_mj.addrs_trk in str(N_filei) and obj_ni.status_trki == "LongPosIncreased":
                    average_longincr.append([obj_ni.addrs_trki, obj_ni.lives_trki, obj_ni.status_trki, obj_ni.amount_trdi, i])

                if obj_mj.addrs_trk in str(N_filei) and obj_ni.status_trki in str(globales.netted_status):

                    average_longincr = average_lonposincreased(average_longincr, obj_ni.amount_trdi)

                    amount_trd_sum_b = amount_trd_sum
                    amount_trd_sum   = amount_trd_sum + obj_ni.amount_trdi
                    amount_trd_sum_l = amount_trd_sum

                    print "\n------------------------------------------------------\n"
                    print "\nAmounts!!!\namount_trdi: ", obj_ni.amount_trdi, " | lives_trki: ", obj_ni.lives_trki, " | lives_srci: ", obj_ni.lives_srci, " | amount_trd_sum: ", amount_trd_sum
                    bool_src = obj_ni.status_srci == "OpenLongPosition" or obj_ni.status_srci == "LongPosIncreased"

                    d_amounts = obj_mj.amount_trd - amount_trd_sum
                    if d_amounts > 0:

                        print "Opened contrats: ", obj_mj.amount_trd, " > Sum amounts traded: ", amount_trd_sum, "\n"
                        print "'iteration: ", i, "|addrs_trk: ", obj_mj.addrs_trk, "|status_trki: ", obj_ni.status_trki,  "|amount_trdi: ", obj_ni.amount_trdi, "|addrs_srci: ", obj_ni.addrs_srci, "|status_srci: ",obj_ni.status_srci,"\n"

                        path_complex_value_ele_two = [obj_ni.addrs_srci, lives_srci_new, obj_ni.addrs_trki, obj_ni.lives_trki, obj_ni.amount_trdi, 0]
                        path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))
                        
                        path_complex_two.append(path_complex_ele_two)
                        idx_i.append(i)

                        if bool_src:                            
                            idx_k, M_file = looking_for_netted(i, M_file, obj_ni.addrs_srci)
                            idx_i = id_newrow(idx_k, idx_i)

                        continue

                    elif d_amounts < 0:

                        diff_r = abs(obj_mj.amount_trd - amount_trd_sum_b)
                        print "Opened contrats: ", obj_mj.amount_trd, " < Sum amounts traded: ", amount_trd_sum, "diff_r: ", diff_r, "\n"

                        print "amount_trdi", obj_ni.amount_trdi, "lives_trki", obj_ni.lives_trki, "status_trki: ", obj_ni.status_trki
                        x_src = abs(obj_ni.amount_trdi - obj_ni.lives_srci) if "Long" in str(obj_ni.status_srci) else -abs(obj_ni.amount_trdi - obj_ni.lives_srci)
                        y_trk = abs(obj_ni.amount_trdi + obj_ni.lives_trki) if "Long" in str(obj_ni.status_trki) else -abs(obj_ni.amount_trdi - obj_ni.lives_trki)
                        print "x_src: ", x_src, "y_trk", y_trk
                        
                        path_complex_ele_value_thr = [obj_ni.addrs_srci, abs(x_src + diff_r), obj_ni.addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                        path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                        path_complex_two.append(path_complex_ele_thr)
                        print "'iteration: ", i, "|addrs_trk: ", obj_mj.addrs_trk, "|status_trki: ", obj_ni.status_trki,  "|amount_trdi: ", obj_ni.amount_trdi, "|addrs_srci: ", obj_ni.addrs_srci, "|status_srci: ",obj_ni.status_srci,"\n"
                        print "\n------------------------------------------------------\n"

                        amount_new = abs(diff_r - obj_ni.amount_trdi)
                        new_row = np.array([obj_ni.addrs_srci, obj_ni.status_srci, obj_ni.lives_srci, obj_ni.addrs_trki, obj_ni.status_trki, obj_ni.lives_trki, amount_new])
                        M_file = np.delete(M_file, i, 0)
                        M_file = np.insert(M_file, i, new_row, 0)

                        bool_new_row = True
                        print "\nNew row added: ", new_row, "\n"

                        if bool_src and obj_mj.amount_trd > amount_trd_sum:                            
                            idx_k, M_file = looking_for_netted(i, M_file, obj_ni.addrs_srci)
                        else:
                            break

                        break
                    
                    elif d_amounts == 0:

                        print "Opened contrats: ", obj_mj.amount_trd, " = Sum amounts traded: ", amount_trd_sum, "\n"
                        print "'iteration: ", i, " |addrs_trk: ", obj_mj.addrs_trk, "|status_trki: ", obj_ni.status_trki,  "|amount_trdi: ", obj_ni.amount_trdi, "|addrs_srci: ", obj_ni.addrs_srci, "|status_srci: ",obj_ni.status_srci,"\n"

                        path_complex_ele_value_one = [obj_ni.addrs_srci, lives_srci_new, obj_ni.addrs_trki, obj_ni.lives_trki, obj_ni.amount_trdi, 0]
                        path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                        path_complex_two.append(path_complex_ele_one)

                        idx_i.append(i)

                        if bool_src:                            
                            idx_k, M_file = looking_for_netted(i, M_file, obj_ni.addrs_srci)
                            idx_i = id_newrow(idx_k, idx_i)

                        break
            ####################################################################
            idx_j = idx_i

            total_amount_trade = 0
            for j in range(len(path_complex_two)):              
                total_amount_trade += int(path_complex_two[j]['amount_trd'])

            contrats_opened   = int(path_complex_two[0]['amount_trd'])
            total_next_trades = abs(contrats_opened - total_amount_trade)

            contracts_opened_sett = abs(contrats_opened - total_next_trades)
            if total_next_trades < contrats_opened and total_next_trades != 0:
                path_complex_two[0]['opened_sett']=contracts_opened_sett

            if idx_iter == 0 or len(path_complex_two) == 1:

                single_path_value_begin = [M_file[0][0], M_file[0][2], M_file[0][3], M_file[0][5], M_file[0][6], M_file[0][6]]
                path_complex_one = dict(zip(globales.key_path,single_path_value_begin))
                print "Single path:\n", path_complex_one, "\n"
                print "Length M_file: ", 1

                idx_j = [0]
            else:
                print "\n------------------------------------------------------\n"
                print "\nPath:\n", np.array(path_complex_two), "\n"
    else:
        
        q += 1
        print "###################################################################################\n"
        print "Source:", obj_mj.addrs_src, "; Tracked:", obj_mj.addrs_trk,"\n"
        print "Status source: ", obj_mj.status_src, "; Status tracked: ", obj_mj.status_trk, "; q-esimo: ", q,"\n"
        single_path_value_begin = [M_file[0][0], M_file[0][2], M_file[0][3], M_file[0][5], M_file[0][6], M_file[0][6]]
        path_complex_one = dict(zip(globales.key_path,single_path_value_begin))

        print "Single path:\n", path_complex_one, "\n"
        print "Length M_file: ", 1

        idx_j = [0]