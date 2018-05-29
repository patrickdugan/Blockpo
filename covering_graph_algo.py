#! /usr/bin/python

import sys
import numpy as np

sys.stdout = open('out', 'w')

print "\n#------------------------'Definitions'---------------------------------#\n"			
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\topened or keep open contracts at the end date"

with open('graphInfoSixth.txt') as file:
    M_file = [[int(digit) if digit.isdigit() else digit for digit in line.split()] for line in file]

idx_j = [0]
Interval = range(len(M_file))

for j in Interval:	

	if j > 0:
		print "\nRows to deleted: ", idx_j
		M_file = np.delete(M_file, idx_j, 0)
		
	print "M_file:\n", M_file, "\nLength M_file: ", len(M_file), "\n"

	single_path = []

	if len(M_file) == 1:
		print "#---------------------------------------------------------------------------------#\n"
		print "Source: ", M_file[0][3], "; Tracked: ", M_file[0][0], "\n"	
		single_path.append(M_file[0][0])
		single_path.append(M_file[0][6])
		single_path.append(M_file[0][3])
		print "Single path:\n", single_path, "\n"
		print "Length M_file: ", 0
		break

	M_filej = []
	M_filej = M_file[:][0]

	addrs_trk  = M_filej[0] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[3]
	status_trk = M_filej[1] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[4] 
	lives_trk  = int(M_filej[2]) if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else int(M_filej[5])

	addrs_src  = M_filej[3] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[0]
	status_src = M_filej[4] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[1]
	lives_src  = int(M_filej[5]) if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else int(M_filej[2])

	amount_trd = int(M_filej[6]) 

	path_complex = []
	path_complex_one = []
	path_complex_two = []
	contracts_opened = []
	R_partly = []

	if status_src == "OpenShortPosition" or status_src == "ShortPosIncreased":
	
		if status_trk == "ShortPosNetted" or status_trk == "ShortPosNettedPartly":
			
			print "#---------------------------------------------------------------------------------#\n"
			print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"
			print "................................"

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			print "Single path:\n", single_path, "\n"			
			idx_j = [0]

			continue

		elif status_trk == "OpenLongPosition" or status_trk == "LongPosIncreased":

			print "#---------------------------------------------------------------------------------#\n"
			print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"
			print "................................"

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			path_complex_one.append(single_path)
			path_complex_two.append(single_path)
			path_complex.append(single_path)

			idx_iter = 0
			Long_pos_incr = []
			status_addrs_trk_v = []

			amount_trd_sum = 0
			path_complex_ele_fth = []

			idx_i  = [0]
			idx_j1 = [0]
			idx_j2 = [0]
			idx_j3 = [0]

			for i in xrange(1, len(M_file)):				

				N_filei = []
				N_filei = M_file[:][i]

				status_trki = N_filei[1] if addrs_trk == N_filei[0] else N_filei[4]
				status_srci = N_filei[1] if addrs_src == N_filei[0] else N_filei[4]
				addrs_trki  = N_filei[0] if addrs_trk == N_filei[0] else N_filei[3]
				addrs_srci  = N_filei[0] if addrs_src == N_filei[0] else N_filei[3]
				lives_trki  = int(N_filei[2]) if addrs_trk == N_filei[0] else int(N_filei[5])
				lives_srci  = int(N_filei[2]) if addrs_src == N_filei[0] else int(N_filei[5])
				amount_trdi = int(N_filei[6])

				if addrs_trk in str(N_filei):
					idx_iter += 1
					status_addrs_trk_v.append(status_trki)

				path_long_pos_incr = []
				path_long_pos_incr.append(N_filei[0])
				path_long_pos_incr.append(amount_trdi)
				path_long_pos_incr.append(N_filei[3])
				Long_pos_incr.append(path_long_pos_incr)

				netted_status = ["LongPosNetted", "LongPosNettedPartly"]

				if addrs_trk in str(N_filei) and status_trki in str(netted_status):

					amount_trd_sum_b = amount_trd_sum
					amount_trd_sum   = amount_trd_sum + amount_trdi
					amount_trd_sum_l = amount_trd_sum

					print "Amounts!!! amount_trdi: ", amount_trdi, " | amount_trd: ", amount_trd, " | lives_trki: ", lives_trki, " | amount_trd_sum: ", amount_trd_sum

					d_amounts = amount_trd - amount_trd_sum
					if d_amounts > 0:

						print "Opened contrats: ", amount_trd, " > Total amount traded: ", amount_trd_sum, "\n"
						print "amount_trd > amount_trd_sum: | addrs_trk | status_trki | amount_trdi |", addrs_trk, status_trki, amount_trdi, "\n"
						print "................................"

						path_complex_ele_two = []
						path_complex_ele_two.append(addrs_srci)
						path_complex_ele_two.append(amount_trdi)
						path_complex_ele_two.append(addrs_trki)

						path_complex_two.append(path_complex_ele_two)
						idx_i.append(i)

					elif d_amounts < 0:

						print "Opened contrats: ", amount_trd, " < Total amount traded: ", amount_trd_sum, "\n"
						diff_r = abs(amount_trd - amount_trd_sum_b)
 
 						path_complex_ele_thr = []
						path_complex_ele_thr.append(addrs_srci)
						path_complex_ele_thr.append(diff_r)
						path_complex_ele_thr.append(addrs_trki)

						path_complex_two.append(path_complex_ele_thr)
						print "amount_trd < amount_trd_sum: | addrs_trk | status_trki | amount_trdi | diff_r |", addrs_trk, status_trki, amount_trdi, diff_r, "\n"
						print "................................"

						idx_i.append(i)

						amount_new = abs(diff_r - amount_trd_sum_l)
						new_row = np.array([N_filei[0], N_filei[1], N_filei[2], N_filei[3], N_filei[4], N_filei[5], amount_new])
						M_file = np.vstack([M_file, new_row])

						print "New row added: ", new_row
						print "................................"
						break
					
					elif d_amounts == 0:

						print "Opened contrats: ", amount_trd, " = Total amount traded: ", amount_trd_sum, "\n"
						print "amount_trd = amount_trd_sum: | addrs_trk | status_trki | amount_trdi |", addrs_trk, status_trki, amount_trdi, "\n"
						print "................................"

						path_complex_ele_one = []
						path_complex_ele_one.append(addrs_srci)
						path_complex_ele_one.append(amount_trdi)
						path_complex_ele_one.append(addrs_trki)
						path_complex_two.append(path_complex_ele_one)

						idx_i.append(i)

						break

			idx_j = idx_i
			if len(status_addrs_trk_v) == 0 and len(status_addrs_trk_v) == 0:

				print "Opened contrats: ", amount_trd, "\n"
				print "Single path: ", single_path
				idx_j = [0]

				continue

			print "\nPath:\n", path_complex_two
			print "................................"