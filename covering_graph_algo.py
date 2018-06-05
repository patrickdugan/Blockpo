#! /usr/bin/python

import sys
import numpy as np
from graph_operators import*

sys.stdout = open('out', 'w')

print "\n#------------------------'Definitions'---------------------------------#\n"            
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\topened or keep open contracts at the end date"

with open('graphInfoSixth.txt') as file:
    M_file = [[int(digit) if digit.isdigit() else digit for digit in line.split()] for line in file]

print "Negative or Positive:"
M_file = negative_for_short(M_file)

idx_j = [0]
Interval = range(len(M_file))

key_path = ['addrs_src', 'lives_src', 'addrs_trk', 'lives_trk', 'amount_trd', 'opened_sett']
for j in Interval:  

    if j > 0:
        print "\nRows to deleted: ", idx_j
        M_file = np.delete(M_file, idx_j, 0)
        
    print "M_file:\n", M_file, "\nLength M_file: ", len(M_file), "\n"

    if len(M_file) == 1:
      print "#---------------------------------------------------------------------------------#\n"
      print "Source: ", M_file[0][3], "; Tracked: ", M_file[0][0], "\n"   
      single_path_value_begin = [M_file[0][0], M_file[0][2], M_file[0][3], M_file[0][5], M_file[0][6], M_file[0][6]]
      single_path_begin = dict(zip(key_path,single_path_value_begin))
      print "Single path:\n", single_path_begin, "\n"
      print "Length M_file: ", 0
      break

    M_filej = []
    M_filej = M_file[:][0]

    addrs_trk, status_trk, lives_trk, addrs_src, status_src, lives_src, amount_trd = amounts_addresses_status(M_filej) 

    path_complex_one = []
    path_complex_two = []

    if status_src == "OpenShortPosition" or status_src == "ShortPosIncreased":
    
        if status_trk == "ShortPosNetted" or status_trk == "ShortPosNettedPartly":
            
            print "#---------------------------------------------------------------------------------#\n"
            print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"

            single_path_value_ele = [addrs_src, lives_src, addrs_trk, lives_trk, amount_trd, amount_trd]
            single_path_ele = dict(zip(key_path, single_path_value_ele))

            print "Single path:\n", single_path_ele, "\n"           
            print "Length M_file: ", 1
            idx_j = [0]

            continue

        elif status_trk == "OpenLongPosition" or status_trk == "LongPosIncreased":

            print "#---------------------------------------------------------------------------------#\n"
            print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"

            single_path_value_ele = [addrs_src, lives_src, addrs_trk, lives_trk, amount_trd, 0]
            single_path_ele = dict(zip(key_path, single_path_value_ele))

            path_complex_two.append(single_path_ele)

            idx_iter = 0
            amount_trd_sum = 0
            path_complex_ele_fth = []

            idx_i  = [0]

            for i in xrange(1, len(M_file)):                

                N_filei = []
                N_filei = M_file[:][i]

                addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi = amounts_addresses_status_inloop(N_filei, addrs_trk)

                if addrs_trk in str(N_filei):
                    idx_iter += 1

                netted_status = ["LongPosNetted", "LongPosNettedPartly"]

                ####################################################################

                if addrs_trk in str(N_filei) and status_trki in str(netted_status):

                    amount_trd_sum_b = amount_trd_sum
                    amount_trd_sum   = amount_trd_sum + amount_trdi
                    amount_trd_sum_l = amount_trd_sum

                    print "................................\n"
                    print "Amounts!!! amount_trdi: ", amount_trdi, " | amount_trd: ", amount_trd, " | lives_trki: ", lives_trki, " | lives_srci: ", lives_srci, " | amount_trd_sum: ", amount_trd_sum

                    d_amounts = amount_trd - amount_trd_sum
                    if d_amounts > 0:

                        print "Opened contrats: ", amount_trd, " > Total amount traded: ", amount_trd_sum, "\n"
                        print "amount_trd > amount_trd_sum: | addrs_trk | status_trki | amount_trdi |", addrs_trk, status_trki, amount_trdi, "\n"

                        path_complex_value_ele_two = [addrs_srci, lives_srci, addrs_trki, lives_trki, amount_trdi, 0]
                        path_complex_ele_two = dict(zip(key_path, path_complex_value_ele_two))
                        
                        path_complex_two.append(path_complex_ele_two)

                        print "Checking: |index i:", i,"|addrs_srci:",addrs_srci,"|status_srci:",status_srci,"|amount_trdi:",amount_trdi,"|\n"                        
                        if status_srci == "OpenLongPosition" or status_srci == "LongPosIncreased":                            
                            looking_for_netted(i, M_file, addrs_srci)
                        else:
                            continue

                        idx_i.append(i)

                    elif d_amounts < 0:

                        diff_r = abs(amount_trd - amount_trd_sum_b)
                        print "Opened contrats: ", amount_trd, " < Total amount traded: ", amount_trd_sum, "diff_r: ", diff_r, "\n"

                        print "amount_trdi", amount_trdi, "lives_trki", lives_trki, "status_trki: ", status_trki
                        x_src = abs(amount_trdi - lives_srci) if "Long" in str(status_srci) else -abs(amount_trdi - lives_srci)
                        y_trk = abs(amount_trdi + lives_trki) if "Long" in str(status_trki) else -abs(amount_trdi - lives_trki)
                        print "x_src: ", x_src, "y_trk", y_trk
                        
                        path_complex_ele_value_thr = [addrs_srci, abs(x_src + diff_r), addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                        path_complex_ele_thr = dict(zip(key_path, path_complex_ele_value_thr))

                        path_complex_two.append(path_complex_ele_thr)
                        print "amount_trd < amount_trd_sum: | addrs_trk | status_trki | amount_trdi | diff_r |", addrs_trk, status_trki, amount_trdi, diff_r, "\n"
                        print "................................"

                        idx_i.append(i)

                        amount_new = abs(diff_r - amount_trdi)
                        new_row = np.array([addrs_srci, status_srci, lives_srci, addrs_trki, status_trki, lives_trki, amount_new])
                        M_file = np.vstack([M_file, new_row])

                        print "\nNew row added: ", new_row, "\n"

                        print "Checking: |index i:", i,"|addrs_srci:",addrs_srci,"|status_srci:",status_srci,"|amount_trdi:",amount_trdi,"|\n"                        
                        if status_srci == "OpenLongPosition" or status_srci == "LongPosIncreased":
                            looking_for_netted(i, M_file, addrs_srci)
                        else:
                            continue

                        break
                    
                    elif d_amounts == 0:

                        print "Opened contrats: ", amount_trd, " = Total amount traded: ", amount_trd_sum, "\n"
                        print "amount_trd = amount_trd_sum: | addrs_trk | status_trki | amount_trdi |", addrs_trk, status_trki, amount_trdi, "\n"

                        path_complex_ele_value_one = [addrs_srci, lives_srci, addrs_trki, lives_trki, amount_trdi, 0]
                        path_complex_ele_one = dict(zip(key_path, path_complex_ele_value_one))                      

                        path_complex_two.append(path_complex_ele_one)

                        print "Checking: |index i:", i,"|addrs_srci:",addrs_srci,"|status_srci:",status_srci,"|amount_trdi:",amount_trdi,"|\n"                        
                        if status_srci == "OpenLongPosition" or status_srci == "LongPosIncreased":
                            looking_for_netted(i, M_file, addrs_srci)
                        else:
                            continue

                        idx_i.append(i)

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

                print "total_next_trades: ", total_next_trades, "\tcontrats_opened: ", contrats_opened
                print "contracts_opened_sett: ", contracts_opened_sett, "\n"
                path_complex_two[0]['opened_sett']=contracts_opened_sett

            print "................................"

            if idx_iter == 0 or len(path_complex_two) == 1:

                single_path_value_begin = [M_file[0][0], M_file[0][2], M_file[0][3], M_file[0][5], M_file[0][6], M_file[0][6]]
                path_complex_one = dict(zip(key_path,single_path_value_begin))
                print "Single path:\n", path_complex_one, "\n"
                print "Length M_file: ", 1

                idx_j = [0]
            else:
                print "\nPath:\n", np.array(path_complex_two), "\n"

            print "................................"