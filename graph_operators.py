#! /usr/bin/python

import numpy as np
import globales
import setglobales

globales.init()
setglobales.stuff()

class status_amounts:

    def __init__(self, v):
    
        if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased":

            self.addrs_trk  = v[0]
            self.status_trk = v[1]
            self.lives_trk  = int(v[2]) 
            self.addrs_src  = v[3] 
            self.status_src = v[4] 
            self.lives_src  = int(v[5]) 
        else:
            self.addrs_trk  = v[3]
            self.status_trk = v[4] 
            self.lives_trk  = int(v[5])
            self.addrs_src  = v[0]
            self.status_src = v[1]
            self.lives_src  = int(v[2])

        self.amount_trd = int(v[6])

class status_amounts_j:

    def __init__(self, v, addrs_trk):

        if addrs_trk == v[0]:   
            self.status_trki = v[1] 
            self.status_srci = v[4] 
            self.addrs_trki  = v[0] 
            self.addrs_srci  = v[3] 
            self.lives_trki  = int(v[2]) 
            self.lives_srci  = int(v[5]) 
        else:
            self.status_trki = v[4]
            self.status_srci = v[1]
            self.addrs_trki  = v[3]
            self.addrs_srci  = v[0]
            self.lives_trki  = int(v[5])
            self.lives_srci  = int(v[2])

        self.amount_trdi = int(v[6])

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

def looking_for_netted(i, m, addrs_trk):

    idx_k = []    
    v = m[:][i]

    obj_ni = status_amounts_j(v, addrs_trk)
    amount_trd_sum = 0

    path_complex_two = []

    single_path_value_ele = [obj_ni.addrs_srci, obj_ni.lives_srci, obj_ni.addrs_trki, obj_ni.lives_trki, obj_ni.amount_trdi, 0]
    single_path_ele = dict(zip(globales.key_path, single_path_value_ele))
    path_complex_two.append(single_path_ele)

    print "______________________________\n"
    print "New path for:", obj_ni.addrs_trki, "??\n"
    print "|addrs_trk:", obj_ni.addrs_trki, "|lives_trk: ", obj_ni.lives_trki,"|status_trk: ", obj_ni.status_trki,"|amount_trd: " , obj_ni.amount_trdi, "|\n"

    idx_iter = 0    

    average_longincr = []            
    average_longincr.append([obj_ni.addrs_trki, obj_ni.lives_trki, obj_ni.status_trki, obj_ni.amount_trdi, 0])

    for k in xrange(i+1, len(m)):
                                
        u = m[:][k]
        obj_mk = status_amounts_j(u, addrs_trk)

        if obj_ni.addrs_trki in str(u):
            idx_iter += 1

        if obj_ni.addrs_trki in str(u) and obj_mk.status_trki == "LongPosIncreased":
            average_longincr.append([obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.status_trki, obj_mk.amount_trdi, k])

        if obj_ni.addrs_trki in str(u) and obj_mk.status_trki in str(globales.netted_status):
            
            average_lonposincreased(average_longincr, obj_mk.amount_trdi)

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + obj_mk.amount_trdi
            amount_trd_sum_l = amount_trd_sum

            bool_src = obj_mk.status_srci == "OpenLongPosition" or obj_mk.status_srci == "LongPosIncreased"
            d_amounts = obj_ni.amount_trdi - amount_trd_sum

            if d_amounts > 0:
                
                print "______________________________\n"
                print "Check!! amount_trd > amount_trd_sum!!: | addrs_trk | obj_mk.status_trki | obj_mk.amount_trdi |", obj_ni.addrs_srci, obj_mk.status_srci, obj_mk.amount_trdi, "\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi
                path_complex_value_ele_two = [obj_mk.addrs_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.amount_trdi, 0]
                path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))

                path_complex_two.append(path_complex_ele_two)

                idx_k.append(k)

                if bool_src:                            
                    idx_q, M_file = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)
 
            elif d_amounts < 0:

                print "______________________________\n"
                print "Check!! amount_trd < amount_trd_sum: |addrs_trk: ", obj_ni.addrs_trki, "|obj_mk.status_trki: ", obj_mk.status_trki,  "|obj_mk.amount_trdi: ", obj_mk.amount_trdi, "|obj_mk.addrs_srci: ", obj_mk.addrs_srci, "|obj_mk.status_srci: ",obj_mk.status_srci,"\n"
                print "'Iter: ", k , "': ", k, obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi
                diff_r = abs(obj_ni.amount_trdi - amount_trd_sum_b)

                print "\nobj_mk.amount_trdi", obj_mk.amount_trdi, "obj_mk.lives_trki", obj_mk.lives_trki, "obj_mk.status_trki: ", obj_mk.status_trki
                x_src = abs(obj_mk.amount_trdi - obj_mk.lives_srci) if "Long" in str(obj_mk.status_srci) else -abs(obj_mk.amount_trdi - obj_mk.lives_srci)
                y_trk = abs(obj_mk.amount_trdi + obj_mk.lives_trki) if "Long" in str(obj_mk.status_trki) else -abs(obj_mk.amount_trdi - obj_mk.lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk

                path_complex_ele_value_thr = [obj_mk.addrs_srci, abs(x_src + diff_r), obj_mk.addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                idx_k.append(k)
                path_complex_two.append(path_complex_ele_thr)                

                amount_new = abs(diff_r - obj_mk.amount_trdi)
                new_row = np.array([obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, amount_new])
                m = np.vstack([m, new_row])
                print "______________________________\n"
                print "\nNew row added new path: ", new_row, "\n"

                if bool_src:                            
                    idx_q, M_file = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)

                break

            elif d_amounts == 0:

                print "______________________________\n"
                print "Check!! amount_trd = amount_trd_sum: |addrs_trk: ", obj_ni.addrs_trki, "|obj_mk.status_trki: ", obj_mk.status_trki,  "|obj_mk.amount_trdi: ", obj_mk.amount_trdi, "|obj_mk.addrs_srci: ", obj_mk.addrs_srci, "|obj_mk.status_srci: ",obj_mk.status_srci,"\n"
                print "'Iter: ", k , "': ", k, obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi

                path_complex_ele_value_one = [obj_mk.addrs_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.amount_trdi, 0]
                path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                idx_k.append(k)
                path_complex_two.append(path_complex_ele_one)                

                if bool_src:                            
                    idx_q, M_file = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)

                break

    if len(idx_k) == 0:
        print "There is no netted events for: ", obj_ni.addrs_trki, "!!\n"

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

def first_single_path(m):

    bool_single_path = False
    single_path = []
    if len(m) == 1:

        single_path_value_begin = [m[0][0], m[0][2], m[0][3], m[0][5], m[0][6], m[0][6]]
        single_path_begin = dict(zip(globales.key_path,single_path_value_begin))
        single_path.append(single_path_begin)

        bool_single_path = True

    return bool_single_path, single_path

def average_lonposincreased(average_longincr, trade_amount):

    if len(average_longincr) > 1:
            
        print "average_longincr before:\n"
        for row in average_longincr:
            print row

        divider = 0
        for row in average_longincr:
            divider += row[3]
                        
        residue = 0
        if divider >= trade_amount:
            col_amounts = [int((float(row[3])/divider)*trade_amount) for row in average_longincr]
            residue = abs(trade_amount - np.sum(col_amounts))
            print "\ndivider: ", divider, " >= obj_ni.amount_trdi: ", trade_amount                           
            print '\ncol_amounts: ', col_amounts, ", amount_trdi - sum(col_amounts): ", trade_amount - np.sum(col_amounts), ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts)
        else:
            col_amounts = [row[3] for row in average_longincr]
            residue = 0
            print "\ndivider: ", divider, " < obj_ni.amount_trdi: ", trade_amount                            
            print '\ncol_amounts: ', col_amounts, ", divider - sum(col_amounts): ", divider - np.sum(col_amounts), ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts)

        k = 0
        last_amount_trade = 0
        for row in average_longincr:
            k += 1
            row[3] = row[3]-(col_amounts[k-1]+residue) if k == np.argmax(col_amounts)+1 else row[3]-col_amounts[k-1]
            row[1] = row[3] if k == 1 else row[3]+last_amount_trade
            last_amount_trade += row[3]

        print "\naverage_longincr later:\n"
        for row in average_longincr:
            print row
    return