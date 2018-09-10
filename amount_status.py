#! /usr/bin/python

import numpy as np
import globales
import setglobales

globales.init()
setglobales.stuff()


class status_amounts_long_trk:

    def __init__(self, v):

        if v[1] in globales.open_incr_long:

            self.addrs_trk = v[0]
            self.status_trk = v[1]
            self.lives_trk = int(v[2])
            self.addrs_src = v[3]
            self.status_src = v[4]
            self.lives_src = int(v[5])
            self.lives_trk_updated = float(v[8])
            self.lives_src_updated = float(v[9])
        else:
            self.addrs_trk = v[3]
            self.status_trk = v[4]
            self.lives_trk = int(v[5])
            self.addrs_src = v[0]
            self.status_src = v[1]
            self.lives_src = int(v[2])
            self.lives_trk_updated = float(v[9])
            self.lives_src_updated = float(v[8])
            
        self.amount_trd = int(v[6])
        self.matched_price = int(v[7])

class status_amounts_short_trk:

    def __init__(self, v):

        if v[1] in globales.open_incr_short:

            self.addrs_trk = v[0]
            self.status_trk = v[1]
            self.lives_trk = int(v[2])
            self.addrs_src = v[3]
            self.status_src = v[4]
            self.lives_src = int(v[5])
            self.lives_trk_updated = float(v[8])
            self.lives_src_updated = float(v[9])
        else:
            self.addrs_trk = v[3]
            self.status_trk = v[4]
            self.lives_trk = int(v[5])
            self.addrs_src = v[0]
            self.status_src = v[1]
            self.lives_src = int(v[2])
            self.lives_trk_updated = float(v[9])
            self.lives_src_updated = float(v[8])

        self.amount_trd = int(v[6])
        self.matched_price = int(v[7])

class status_amounts_open_incr_pos:

    def __init__(self, v, addrs_trk):

        if addrs_trk == v[0] and v[0] in globales.open_incr_long_short:
            self.status_trk = v[1]
            self.status_src = v[4]
            self.addrs_trk = v[0]
            self.addrs_src = v[3]
            self.lives_trk = int(v[2])
            self.lives_src = int(v[5])
        else:
            self.status_trk = v[4]
            self.status_src = v[1]
            self.addrs_trk = v[3]
            self.addrs_src = v[0]
            self.lives_trk = int(v[5])
            self.lives_src = int(v[2])

        self.amount_trd = int(v[6])
        self.matched_price = int(v[7])


class status_amounts_inloop:

    def __init__(self, v, addrs_trk):

        if addrs_trk == v[0]:
            self.status_trk = v[1]
            self.status_src = v[4]
            self.addrs_trk = v[0]
            self.addrs_src = v[3]
            self.lives_trk = int(v[2])
            self.lives_src = int(v[5])
            self.lives_trk_updated = int(v[8])
            self.lives_src_updated = int(v[9])
        else:
            self.status_trk = v[4]
            self.status_src = v[1]
            self.addrs_trk = v[3]
            self.addrs_src = v[0]
            self.lives_trk = int(v[5])
            self.lives_src = int(v[2])
            self.lives_trk_updated = int(v[9])
            self.lives_src_updated = int(v[8])

        self.amount_trd = int(v[6])
        self.matched_price = int(v[7])

class status_for_contracts_stillopened:

    def __init__(self, v, addrs_trk):

        if addrs_trk == v[0]:
            self.status_trk = v[1]
            self.status_src = v[4]
            self.addrs_trk = v[0]
            self.addrs_src = v[3]
            self.lives_trk = int(v[2])
            self.lives_src = int(v[5])
        else:
            self.status_trk = v[4]
            self.status_src = v[1]
            self.addrs_trk = v[3]
            self.addrs_src = v[0]
            self.lives_trk = int(v[5])
            self.lives_src = int(v[2])

        self.amount_trd = int(v[6])
        self.matched_price = int(v[7])

class status_amounts_inthepath:

    def __init__(self, v, addrs_trk):

        if addrs_trk == v['addrs_trk']:
            self.status_trk = v['status_trk']
            self.status_src = v['status_src']
            self.addrs_trk = v['addrs_trk']
            self.addrs_src = v['addrs_src']
            self.lives_trk = int(v['lives_trk'])
            self.lives_src = int(v['lives_src'])
        else:
            self.status_trk = v['status_src']
            self.status_src = v['status_trk']
            self.addrs_trk = v['addrs_src']
            self.addrs_src = v['addrs_trk']
            self.lives_trk = int(v['lives_src'])
            self.lives_src = int(v['lives_trk'])

        self.amount_trd = int(v['amount_trd'])