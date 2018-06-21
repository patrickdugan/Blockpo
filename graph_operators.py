#! /usr/bin/python

import numpy as np
import globales
import setglobales
from amount_status import*

globales.init()
setglobales.stuff()

def clearing_operator(M_file, obj_trk, idx_iter, average_incr, amount_trd_sum, path_complex_two, idx_i, bool_del_path_row):

    for i in xrange(1, len(M_file)):                

        N_filei = []
        N_filei = M_file[:][i]
        obj_trk_inloop = status_amounts_inloop(N_filei, obj_trk.addrs_trk)

        if obj_trk.addrs_trk in str(N_filei):            
            idx_iter += 1

        average_incr = vector_pos_incr(obj_trk.addrs_trk, obj_trk_inloop, N_filei, average_incr, i)
        row_path_del = []

        if obj_trk.addrs_trk in str(N_filei) and obj_trk_inloop.status_trki in str(globales.netted_status_long):

            average_incr = average_posincreased(average_incr, obj_trk_inloop.amount_trdi)

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + obj_trk_inloop.amount_trdi
            amount_trd_sum_l = amount_trd_sum

            print "\n------------------------------------------------------\n"
            print "\nAmounts!!!\namount_trdi: ", obj_trk_inloop.amount_trdi, " | lives_trki: ", obj_trk_inloop.lives_trki, " | lives_srci: ", obj_trk_inloop.lives_srci, " | amount_trd_sum: ", amount_trd_sum
            bool_src = obj_trk_inloop.status_srci in globales.open_incr_long

            d_amounts = obj_trk.amount_trd - amount_trd_sum
            if d_amounts > 0:

                print "Opened contrats: ", obj_trk.amount_trd, " > Sum amounts traded: ", amount_trd_sum, "\n"
                print "'iteration: ", i, "|addrs_trk: ", obj_trk.addrs_trk, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"

                path_complex_value_ele_two = [obj_trk_inloop.addrs_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))
                        
                path_complex_two.append(path_complex_ele_two)
                idx_i.append(i)

                if bool_src:                            
                    idx_k, M_file = looking_for_netted(i, M_file, obj_trk_inloop.addrs_srci)
                    idx_i = id_newrow(idx_k, idx_i)

                continue

            elif d_amounts < 0:

                diff_r = abs(obj_trk.amount_trd - amount_trd_sum_b)
                print "Opened contrats: ", obj_trk.amount_trd, " < Sum amounts traded: ", amount_trd_sum, "diff_r: ", diff_r, "\n"

                print "amount_trdi", obj_trk_inloop.amount_trdi, "lives_trki", obj_trk_inloop.lives_trki, "status_trki: ", obj_trk_inloop.status_trki
                x_src = abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_srci) if "Long" in str(obj_trk_inloop.status_srci) else -abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_srci)
                y_trk = abs(obj_trk_inloop.amount_trdi + obj_trk_inloop.lives_trki) if "Long" in str(obj_trk_inloop.status_trki) else -abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk
                    
                path_complex_ele_value_thr = [obj_trk_inloop.addrs_srci, abs(x_src + diff_r), obj_trk_inloop.addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                path_complex_two.append(path_complex_ele_thr)
                print "'iteration: ", i, "|addrs_trk: ", obj_trk.addrs_trk, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"
                print "\n------------------------------------------------------\n"

                amount_new = abs(diff_r - obj_trk_inloop.amount_trdi)
                new_row = np.array([obj_trk_inloop.addrs_srci, obj_trk_inloop.status_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_trki, obj_trk_inloop.lives_trki, amount_new])
                M_file = np.delete(M_file, i, 0)
                M_file = np.insert(M_file, i, new_row, 0)

                bool_new_row = True
                print "\nNew row added: ", new_row, "\n"

                if bool_src and obj_trk.amount_trd > amount_trd_sum:                            
                    idx_k, M_file = looking_for_netted(i, M_file, obj_trk_inloop.addrs_srci)
                else:
                    break

                break
                    
            elif d_amounts == 0:

                print "Opened contrats: ", obj_trk.amount_trd, " = Sum amounts traded: ", amount_trd_sum, "\n"
                print "'iteration: ", i, " |addrs_trk: ", obj_trk.addrs_trk, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"

                path_complex_ele_value_one = [obj_trk_inloop.addrs_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                path_complex_two.append(path_complex_ele_one)

                idx_i.append(i)

                if bool_src:                            
                    idx_k, M_file = looking_for_netted(i, M_file, obj_trk_inloop.addrs_srci)
                    idx_i = id_newrow(idx_k, idx_i)

                break
        ###################################################################
        if obj_trk.addrs_trk in str(N_filei) and obj_trk_inloop.status_trki in str(globales.netted_status_short):
            
            average_incr = average_posincreased(average_incr, obj_trk_inloop.amount_trdi)
            row_path_del.append(i)

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + obj_trk_inloop.amount_trdi
            amount_trd_sum_l = amount_trd_sum

            print "\n------------------------------------------------------\n"
            print "\nAmounts!!!\namount_trdi: ", obj_trk_inloop.amount_trdi, " | lives_trki: ", obj_trk_inloop.lives_trki, " | lives_srci: ", obj_trk_inloop.lives_srci, " | amount_trd_sum: ", amount_trd_sum
            bool_src = obj_trk_inloop.status_srci in globales.open_incr_short

            d_amounts = obj_trk.amount_trd - amount_trd_sum
            if d_amounts > 0:

                print "Opened contrats: ", obj_trk.amount_trd, " > Sum amounts traded: ", amount_trd_sum, "\n"
                print "'iteration: ", i, "|addrs_trk: ", obj_trk.addrs_trk, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"

                path_complex_value_ele_two = [obj_trk_inloop.addrs_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))
                        
                path_complex_two.append(path_complex_ele_two)
                idx_i.append(i)

                if bool_src:                            
                    idx_k, M_file = looking_for_netted(i, M_file, obj_trk_inloop.addrs_srci)
                    idx_i = id_newrow(idx_k, idx_i)

                continue

            elif d_amounts < 0:

                diff_r = abs(obj_trk.amount_trd - amount_trd_sum_b)
                print "Opened contrats: ", obj_trk.amount_trd, " < Sum amounts traded: ", amount_trd_sum, "diff_r: ", diff_r, "\n"

                print "amount_trdi", obj_trk_inloop.amount_trdi, "lives_trki", obj_trk_inloop.lives_trki, "status_trki: ", obj_trk_inloop.status_trki
                x_src = abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_srci) if "Long" in str(obj_trk_inloop.status_srci) else -abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_srci)
                y_trk = abs(obj_trk_inloop.amount_trdi + obj_trk_inloop.lives_trki) if "Long" in str(obj_trk_inloop.status_trki) else -abs(obj_trk_inloop.amount_trdi - obj_trk_inloop.lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk
                    
                path_complex_ele_value_thr = [obj_trk_inloop.addrs_srci, abs(x_src + diff_r), obj_trk_inloop.addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                path_complex_two.append(path_complex_ele_thr)
                print "'iteration: ", i, "|addrs_trk: ", obj_trk.addrs_trk, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"
                print "\n------------------------------------------------------\n"

                amount_new = abs(diff_r - obj_trk_inloop.amount_trdi)
                new_row = np.array([obj_trk_inloop.addrs_srci, obj_trk_inloop.status_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.status_trki, obj_trk_inloop.lives_trki, amount_new])
                M_file = np.delete(M_file, i, 0)
                M_file = np.insert(M_file, i, new_row, 0)

                bool_new_row = True
                print "\nNew row added: ", new_row, "\n"

                if bool_src and obj_trk.amount_trd > amount_trd_sum:                            
                    idx_k, M_file = looking_for_netted(i, M_file, obj_trk_inloop.addrs_srci)
                else:
                    break

                break
                    
            elif d_amounts == 0:

                print "Opened contrats: ", obj_trk.amount_trd, " = Sum amounts traded: ", amount_trd_sum, "\n"
                print "'iteration: ", i, " |addrs_trk: ", obj_trk.addrs_trk, "|status_trki: ", obj_trk_inloop.status_trki,  "|amount_trdi: ", obj_trk_inloop.amount_trdi, "|addrs_srci: ", obj_trk_inloop.addrs_srci, "|status_srci: ",obj_trk_inloop.status_srci,"\n"

                path_complex_ele_value_one = [obj_trk_inloop.addrs_srci, obj_trk_inloop.lives_srci, obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.amount_trdi, 0]
                path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                path_complex_two.append(path_complex_ele_one)

                idx_i.append(i)

                if bool_src:                            
                    idx_k, M_file = looking_for_netted(i, M_file, obj_trk_inloop.addrs_srci)
                    idx_i = id_newrow(idx_k, idx_i)

                break

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

    return M_file, idx_j, path_complex_two

def looking_for_netted(i, m, addrs_trk):

    idx_k = []    
    v = m[:][i]

    obj_ni = status_amounts_inloop(v, addrs_trk)
    amount_trd_sum = 0

    path_complex_two = []

    print "______________________________\n"
    print "New netted event for:", obj_ni.addrs_trki, "??\n"
    print "|addrs_trk:", obj_ni.addrs_trki, "|lives_trk: ", obj_ni.lives_trki,"|status_trk: ", obj_ni.status_trki,"|amount_trd: " , obj_ni.amount_trdi, "|\n"

    idx_iter = 0    

    average_incr = []            
    average_incr.append([obj_ni.addrs_trki, obj_ni.lives_trki, obj_ni.status_trki, obj_ni.amount_trdi, 0])

    new_addrs_trk = ""
    for k in xrange(i+1, len(m)):
                                
        u = m[:][k]
        obj_mk = status_amounts_inloop(u, addrs_trk)

        if obj_ni.addrs_trki in str(u):
            idx_iter += 1

        average_incr = vector_pos_incr(obj_ni.addrs_trki, obj_mk, u, average_incr, k)

        if obj_ni.addrs_trki in str(u) and obj_mk.status_trki in str(globales.netted_status_long):
            
            average_incr = average_posincreased(average_incr, obj_mk.amount_trdi)

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + obj_mk.amount_trdi
            amount_trd_sum_l = amount_trd_sum

            bool_src = obj_mk.status_srci in globales.open_incr_long

            d_amounts = obj_ni.amount_trdi - amount_trd_sum
            if d_amounts > 0:
                
                print "______________________________\n"
                print "Check!! amount_trd > amount_trd_sum!!: | addrs_trk | status_trki | amount_trdi |", obj_ni.addrs_srci, obj_mk.status_srci, obj_mk.amount_trdi, "\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi

                path_complex_value_ele_two = [obj_mk.addrs_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.amount_trdi, 0]
                path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))

                path_complex_two.append(path_complex_ele_two)
                idx_k.append(k)

                if bool_src:
                    idx_q, m = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)
 
            elif d_amounts < 0:

                print "______________________________\n"
                print "Check!! amount_trd = %d < amount_trd_sum = %d: |addrs_trk: " %(obj_ni.amount_trdi, amount_trd_sum), obj_ni.addrs_trki, "|status_trki: ", obj_mk.status_trki,  "|amount_trdi: ", obj_mk.amount_trdi, "|addrs_srci: ", obj_mk.addrs_srci, "|status_srci: ",obj_mk.status_srci,"\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi
                diff_r = abs(obj_ni.amount_trdi - amount_trd_sum_b)

                print "\namount_trdi", obj_mk.amount_trdi, "lives_trki", obj_mk.lives_trki, "status_trki: ", obj_mk.status_trki
                x_src = abs(obj_mk.amount_trdi - obj_mk.lives_srci) if "Long" in str(obj_mk.status_srci) else -abs(obj_mk.amount_trdi - obj_mk.lives_srci)
                y_trk = abs(obj_mk.amount_trdi + obj_mk.lives_trki) if "Long" in str(obj_mk.status_trki) else -abs(obj_mk.amount_trdi - obj_mk.lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk

                path_complex_ele_value_thr = [obj_mk.addrs_srci, abs(x_src + diff_r), obj_mk.addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                path_complex_two.append(path_complex_ele_thr)                

                amount_new = abs(diff_r - obj_mk.amount_trdi)
                new_row = np.array([obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, amount_new])
                m = np.delete(m, i, 0)
                m = np.insert(m, i, new_row, 0)

                print "______________________________\n"
                print "\nNew row added new path: ", new_row, "\n"

                if bool_src and obj_ni.amount_trdi > amount_trd_sum:                            
                    idx_q, m = looking_for_netted(k, m, obj_mk.addrs_srci)
                else:
                    break

                break

            elif d_amounts == 0:

                print "______________________________\n"
                print "Check!! amount_trd = amount_trd_sum: |addrs_trk: ", obj_ni.addrs_trki, "|status_trki: ", obj_mk.status_trki,  "|amount_trdi: ", obj_mk.amount_trdi, "|addrs_srci: ", obj_mk.addrs_srci, "|status_srci: ",obj_mk.status_srci,"\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi

                path_complex_ele_value_one = [obj_mk.addrs_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.amount_trdi, 0]
                path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                idx_k.append(k)
                path_complex_two.append(path_complex_ele_one)                

                if bool_src:                            
                    idx_q, m = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)

                break

        if obj_ni.addrs_trki in str(u) and obj_mk.status_trki in str(globales.netted_status_short):

            average_incr = average_posincreased(average_incr, obj_mk.amount_trdi)

            amount_trd_sum_b = amount_trd_sum
            amount_trd_sum   = amount_trd_sum + obj_mk.amount_trdi
            amount_trd_sum_l = amount_trd_sum

            bool_src = obj_mk.status_srci in globales.open_incr_short

            d_amounts = obj_ni.amount_trdi - amount_trd_sum
            if d_amounts > 0:
                
                print "______________________________\n"
                print "Check!! amount_trd > amount_trd_sum!!: | addrs_trk | status_trki | amount_trdi |", obj_ni.addrs_srci, obj_mk.status_srci, obj_mk.amount_trdi, "\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi

                path_complex_value_ele_two = [obj_mk.addrs_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.amount_trdi, 0]
                path_complex_ele_two = dict(zip(globales.key_path, path_complex_value_ele_two))

                path_complex_two.append(path_complex_ele_two)
                idx_k.append(k)

                if bool_src:
                    idx_q, m = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)
 
            elif d_amounts < 0:

                print "______________________________\n"
                print "Check!! amount_trd = %d < amount_trd_sum = %d: |addrs_trk: " %(obj_ni.amount_trdi, amount_trd_sum), obj_ni.addrs_trki, "|status_trki: ", obj_mk.status_trki,  "|amount_trdi: ", obj_mk.amount_trdi, "|addrs_srci: ", obj_mk.addrs_srci, "|status_srci: ",obj_mk.status_srci,"\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi
                diff_r = abs(obj_ni.amount_trdi - amount_trd_sum_b)

                print "\namount_trdi", obj_mk.amount_trdi, "lives_trki", obj_mk.lives_trki, "status_trki: ", obj_mk.status_trki
                x_src = abs(obj_mk.amount_trdi - obj_mk.lives_srci) if "Long" in str(obj_mk.status_srci) else -abs(obj_mk.amount_trdi - obj_mk.lives_srci)
                y_trk = abs(obj_mk.amount_trdi + obj_mk.lives_trki) if "Long" in str(obj_mk.status_trki) else -abs(obj_mk.amount_trdi - obj_mk.lives_trki)
                print "x_src: ", x_src, "y_trk", y_trk

                path_complex_ele_value_thr = [obj_mk.addrs_srci, abs(x_src + diff_r), obj_mk.addrs_trki, abs(y_trk - diff_r), diff_r, 0]                      
                path_complex_ele_thr = dict(zip(globales.key_path, path_complex_ele_value_thr))

                path_complex_two.append(path_complex_ele_thr)                

                amount_new = abs(diff_r - obj_mk.amount_trdi)
                new_row = np.array([obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, amount_new])
                m = np.delete(m, i, 0)
                m = np.insert(m, i, new_row, 0)

                print "______________________________\n"
                print "\nNew row added new path: ", new_row, "\n"

                if bool_src and obj_ni.amount_trdi > amount_trd_sum:                            
                    idx_q, m = looking_for_netted(k, m, obj_mk.addrs_srci)
                else:
                    break

                break

            elif d_amounts == 0:

                print "______________________________\n"
                print "Check!! amount_trd = amount_trd_sum: |addrs_trk: ", obj_ni.addrs_trki, "|status_trki: ", obj_mk.status_trki,  "|amount_trdi: ", obj_mk.amount_trdi, "|addrs_srci: ", obj_mk.addrs_srci, "|status_srci: ",obj_mk.status_srci,"\n"
                print "'Iter: ", k , "': ", obj_mk.addrs_trki, obj_mk.status_trki, obj_mk.lives_trki, obj_mk.addrs_srci, obj_mk.status_srci, obj_mk.lives_srci, obj_mk.amount_trdi

                path_complex_ele_value_one = [obj_mk.addrs_srci, obj_mk.lives_srci, obj_mk.addrs_trki, obj_mk.lives_trki, obj_mk.amount_trdi, 0]
                path_complex_ele_one = dict(zip(globales.key_path, path_complex_ele_value_one))                      

                idx_k.append(k)
                path_complex_two.append(path_complex_ele_one)                

                if bool_src:                            
                    idx_q, m = looking_for_netted(k, m, obj_mk.addrs_srci)
                    idx_k = id_newrow(idx_q, idx_k)

                break

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

    if len(path_complex_two) >= 1:
        print "______________________________\n"
        print "New path!!: ", np.array(path_complex_two)

    if len(idx_k) == 0:
        print "______________________________\n"
        print "No more netted events for addrs_trk!!"

    return idx_k, m

def average_posincreased(average_posincr, trade_amount):

    if len(average_posincr) > 1:
            
        divider = 0
        for row in average_posincr:
            divider += row[3]
                        
        residue = 0
        if divider >= trade_amount:
            col_amounts = [int((float(row[3])/divider)*trade_amount) for row in average_posincr]
            residue = abs(trade_amount - np.sum(col_amounts))
            print "\ndivider: ", divider, " >= obj_ni.amount_trdi: ", trade_amount                           
            print '\ncol_amounts: ', col_amounts, ", amount_trdi - sum(col_amounts): ", trade_amount - np.sum(col_amounts), ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts)
        else:
            col_amounts = [row[3] for row in average_posincr]
            residue = 0
            print "\ndivider: ", divider, " < obj_ni.amount_trdi: ", trade_amount                            
            print '\ncol_amounts: ', col_amounts, ", divider - sum(col_amounts): ", divider - np.sum(col_amounts), ", max(col_amounts): ", np.amax(col_amounts), ", argmax(col_amounts): ", np.argmax(col_amounts)

        k = 0
        last_amount_trade = 0
        for row in average_posincr:
            k += 1
            row[3] = row[3]-(col_amounts[k-1]+residue) if k == np.argmax(col_amounts)+1 else row[3]-col_amounts[k-1]
            row[1] = row[3] if k == 1 else row[3]+last_amount_trade
            last_amount_trade += row[3]

        print "\naverage_posincr:\n", np.array(average_posincr)

    return average_posincr

def vector_pos_incr(obj_trk, obj_trk_inloop, N_filei, average_incr, i):

    if obj_trk in str(N_filei):

        if obj_trk_inloop.status_trki == "LongPosIncreased":

            average_incr.append([obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.status_trki, obj_trk_inloop.amount_trdi, i])

        elif obj_trk_inloop.status_trki == "ShortPosIncreased":     

            average_incr.append([obj_trk_inloop.addrs_trki, obj_trk_inloop.lives_trki, obj_trk_inloop.status_trki, obj_trk_inloop.amount_trdi, i])

    return average_incr