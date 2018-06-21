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

    path_complex_one = []
    path_complex_two = []

    obj_long_trk = status_amounts_long_trk(M_filej)

    if obj_long_trk.status_trk in globales.open_incr_long:

        print "###################################################################################\n"
        print "(Tracking Long Position)", " Source: ", obj_long_trk.addrs_src, "| Tracked: ", obj_long_trk.addrs_trk, "\n"        

        single_path_value_ele = [obj_long_trk.addrs_src, obj_long_trk.lives_src, obj_long_trk.addrs_trk, obj_long_trk.lives_trk , obj_long_trk.amount_trd, 0]
        single_path_ele = dict(zip(globales.key_path, single_path_value_ele))

        path_complex_two.append(single_path_ele)

        idx_iter = 0
        amount_trd_sum = 0
        path_complex_ele_fth = []

        idx_i  = [0]
        average_longincr = []            
        average_longincr.append([obj_long_trk.addrs_trk, obj_long_trk.lives_trk, obj_long_trk.status_trk, obj_long_trk.amount_trd, 0])

        clearing_operator(M_file, obj_long_trk, idx_iter, average_longincr, amount_trd_sum, path_complex_two, idx_i)

    obj_short_trk = status_amounts_short_trk(M_filej)