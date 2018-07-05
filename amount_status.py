#! /usr/bin/python

import numpy as np
import globales
import setglobales

globales.init()
setglobales.stuff()

class status_amounts:

    def __init__(self, v):
    
        if v[4] in globales.open_incr_short:

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
        self.matched_price = int(v[7])

class status_amounts_long_trk:

    def __init__(self, v):
    
        if v[1] in globales.open_incr_long:

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
        self.matched_price = int(v[7])

class status_amounts_short_trk:

    def __init__(self, v):
    
        if v[1] in globales.open_incr_short:

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
        self.matched_price = int(v[7])

class status_amounts_inloop:

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
        self.matched_pricei = int(v[7])

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

        PNL_first = 0
        single_path_value_begin = [m[0][0], m[0][2], m[0][1], m[0][3], m[0][5], m[0][4], m[0][6], m[0][6], m[0][7], PNL_first]
        single_path_begin = dict(zip(globales.key_path,single_path_value_begin))
        single_path.append(single_path_begin)

        bool_single_path = True

    return bool_single_path, single_path