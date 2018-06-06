#! /usr/bin/python

import numpy as np
import globales
import setglobales

globales.init()
setglobales.stuff()

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

def amounts_addresses_status(v):

    if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased":

        addrs_trk  = v[0]
        status_trk = v[1]
        lives_trk  = int(v[2]) 

        addrs_src  = v[3] 
        status_src = v[4] 
        lives_src  = int(v[5]) 
    else:
        addrs_trk  = v[3]
        status_trk = v[4] 
        lives_trk  = int(v[5])

        addrs_src  = v[0]
        status_src = v[1]
        lives_src  = int(v[2])

    amount_trd = int(v[6])

    return addrs_trk, status_trk, lives_trk, addrs_src, status_src, lives_src, amount_trd

def amounts_addresses_status_inloop(v, addrs_trk):

    if addrs_trk == v[0]:   
    
        status_trki = v[1] 
        status_srci = v[4] 

        addrs_trki  = v[0] 
        addrs_srci  = v[3] 

        lives_trki  = int(v[2]) 
        lives_srci  = int(v[5]) 
    else:
        status_trki = v[4]
        status_srci = v[1]
    
        addrs_trki  = v[3]
        addrs_srci  = v[0]

        lives_trki  = int(v[5])
        lives_srci  = int(v[2])

    amount_trdi = int(v[6])

    return addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi

def looking_for_netted(i, m, addrs_trk):

    idx_k = []    
    v = m[:][i]

    addrs_trk, status_trk, lives_trk, addrs_src, status_src, lives_src, amount_trd = amounts_addresses_status_inloop(v, addrs_trk)
    amount_trd_sum = 0

    path_complex_two = []

    single_path_value_ele = [addrs_src, lives_src, addrs_trk, lives_trk, amount_trd, 0]
    single_path_ele = dict(zip(globales.key_path, single_path_value_ele))
    path_complex_two.append(single_path_ele)

    print "______________________________\n"
    print "New path for:", addrs_trk, "??\n"
    print "|addrs_trk:", addrs_trk, "|lives_trk: ", lives_trk,"|status_trk: ", status_trk,"|amount_trd: " ,amount_trd, "|\n"

    idx_iter = 0    

    for k in xrange(i+1, len(m)):
                                
        u = m[:][k]
        addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi = amounts_addresses_status_inloop(u, addrs_trk)

        if addrs_trk in str(u):
            idx_iter += 1

        if addrs_trk in str(u) and status_trki in str(globales.netted_status):
            
            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + amount_trdi
            amount_trd_sum_l = amount_trd_sum

            d_amounts = amount_trd - amount_trd_sum
            if d_amounts > 0:
                
                print "______________________________\n"
                print "Check!! amount_trd > amount_trd_sum!!: | addrs_trk | status_trki | amount_trdi |", addrs_src, status_srci, amount_trdi, "\n"
                print "Iter: ", k, addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi
                path_complex_value_ele_two = [addrs_srci, lives_srci, addrs_trki, lives_trki, amount_trdi, 0]
                path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))

                path_complex_two.append(path_complex_ele_two)

                idx_k.append(k)                
 
            elif d_amounts < 0:

                print "______________________________\n"
                print "Check!! amount_trd < amount_trd_sum: |addrs_trk: ", addrs_trk, "|status_trki: ", status_trki,  "|amount_trdi: ", amount_trdi, "|addrs_srci: ", addrs_srci, "|status_srci: ",status_srci,"\n"
                print "Iter: ", k, addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi
                diff_r = abs(amount_trd - amount_trd_sum_b)

                print "\namount_trdi", amount_trdi, "lives_trki", lives_trki, "status_trki: ", status_trki
                x_src = abs(amount_trdi - lives_srci) if "Long" in str(status_srci) else -abs(amount_trdi - lives_srci)
                y_trk = abs(amount_trdi + lives_trki) if "Long" in str(status_trki) else -abs(amount_trdi - lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk

                path_complex_ele_value_thr = [addrs_srci, abs(x_src + diff_r), addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                idx_k.append(k)
                path_complex_two.append(path_complex_ele_thr)                

                amount_new = abs(diff_r - amount_trdi)
                new_row = np.array([addrs_srci, status_srci, lives_srci, addrs_trki, status_trki, lives_trki, amount_new])
                m = np.vstack([m, new_row])
                print "______________________________\n"
                print "\nNew row added new path: ", new_row, "\n"

                break

            elif d_amounts == 0:

                print "______________________________\n"
                print "Check!! amount_trd = amount_trd_sum: |addrs_trk: ", addrs_trk, "|status_trki: ", status_trki,  "|amount_trdi: ", amount_trdi, "|addrs_srci: ", addrs_srci, "|status_srci: ",status_srci,"\n"
                print "Iter: ", k, addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi

                path_complex_ele_value_one = [addrs_srci, lives_srci, addrs_trki, lives_trki, amount_trdi, 0]
                path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                idx_k.append(k)
                path_complex_two.append(path_complex_ele_one)                

                break
        else:
            continue

    if len(idx_k) != 0:
        print "______________________________\n"
        print "\nidx_k: ", idx_k, "\n"
    else:
        print "There is no netted events for: ", addrs_trk, "!!\n"

    total_amount_trade = 0
    for j in range(len(path_complex_two)):              
        total_amount_trade += int(path_complex_two[j]['amount_trd'])

    contrats_opened = 0
    if len(path_complex_two) != 0:
        contrats_opened = int(path_complex_two[0]['amount_trd']) 
    total_next_trades = abs(contrats_opened - total_amount_trade)

    contracts_opened_sett = abs(contrats_opened - total_next_trades)
    if total_next_trades < contrats_opened and total_next_trades != 0:
        path_complex_two[0]['opened_sett']=contracts_opened_sett

    if len(path_complex_two) > 1:
        print "New path: ", np.array(path_complex_two)

    return idx_k, m

def id_newrow(idx_k, idx_i):
    
    if len(idx_k) != 0:
        for l in range(len(idx_k)):
            idx_i.append(idx_k[l])

    return idx_i