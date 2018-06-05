#! /usr/bin/python

import numpy as np

print "\n#------------------------'Definitions'---------------------------------#\n"			
print "Path Simple:\tThe netting event of the tracked address that receives the contracts happens at the begining\n"
print "Path Complex:\tThe netting event of the tracked address that receives the contracts happens after the contracts are\n\t\topened or keep open contracts at the end date"


with open('graphInfoSixth.txt') as file:
    M_file = [[int(digit) if digit.isdigit() else digit for digit in line.split()] for line in file]

idx_i = [0]
Interval = range(len(M_file))

for j in Interval:	

	if j > 0:
		print "\nRows to deleted: ", idx_i
		M_file = np.delete(M_file, idx_i, 0)
		
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
	lives_trk  = M_filej[2] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[5]
	addrs_src  = M_filej[3] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[0]
	status_src = M_filej[4] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[1]
	lives_src  = M_filej[5] if M_filej[4] == "OpenShortPosition" or M_filej[4] == "ShortPosIncreased" else M_filej[2]
	amount_trd = M_filej[6] 

	path_complex_one = []
	path_complex_two = []
	contracts_opened = []
	R_partly = []
	L_partly = []

	if status_src == "OpenShortPosition" or status_src == "ShortPosIncreased":
	
		if status_trk == "ShortPosNetted" or status_trk == "ShortPosNettedPartly":
			
			print "#---------------------------------------------------------------------------------#\n"
			print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			print "Single path:\n", single_path, "\n"			
			idx_i = [0]

		elif status_trk == "OpenLongPosition" or status_trk == "LongPosIncreased":

			print "#---------------------------------------------------------------------------------#\n"
			print "Source: ", addrs_src, "; Tracked: ", addrs_trk, "\n"

			single_path.append(addrs_src)
			single_path.append(amount_trd)
			single_path.append(addrs_trk)

			path_complex_one.append(single_path)
			path_complex_two.append(single_path)

			Lives_amountsj = []
			Lives_amountsj.append(lives_src)
			Lives_amountsj.append(lives_trk)
			Lives_amountsj.append(amount_trd)		
			L_partly.append(Lives_amountsj)

			idx_iter = 0
			Long_pos_incr = []
			status_addrs_trk_v = []

			amount_trd_sum = 0
			path_complex_ele_thr = []
			path_complex_ele_fth = []

			for i in xrange(1, len(M_file)):				

				N_filei = []
				N_filei = M_file[:][i]

				status_trki = N_filei[1] if addrs_trk == N_filei[0] else N_filei[4]
				addrs_srci  = N_filei[3] if addrs_trk == N_filei[0] else N_filei[0]
				lives_trki  = N_filei[2] if addrs_trk == N_filei[0] else N_filei[5]
				lives_srci  = N_filei[2] if addrs_src == N_filei[0] else N_filei[5]
				amount_trdi = N_filei[6]

				if addrs_trk in str(N_filei):
					idx_iter += 1
					status_addrs_trk_v.append(status_trki)

				path_long_pos_incr = []
				path_long_pos_incr.append(N_filei[0])
				path_long_pos_incr.append(N_filei[6])
				path_long_pos_incr.append(N_filei[3])
				Long_pos_incr.append(path_long_pos_incr)

				if addrs_trk in str(N_filei) and status_trki == "LongPosNetted":

					path_complex_ele_one = []
					path_complex_ele_one.append(N_filei[0])
					path_complex_ele_one.append(N_filei[6])
					path_complex_ele_one.append(N_filei[3])

					path_complex_one.append(path_complex_ele_one) 

					print "Complex path:\n", path_complex_one

					idx_i.append(i)

				elif addrs_trk in str(N_filei) and status_trki == "LongPosNettedPartly":

					print "Hola!!!!!!!!: ", N_filei[2], N_filei[5]
					
					amount_trd_sum_before = amount_trd_sum
					amount_trd_sum = int(amount_trd_sum) + int(N_filei[6])
					amount_trd_sum_later = amount_trd_sum

					if amount_trd_sum <= amount_trd:
						
						path_complex_ele_two = []
						path_complex_ele_two.append(N_filei[0])
						path_complex_ele_two.append(N_filei[6])
						path_complex_ele_two.append(N_filei[3])

						path_complex_two.append(path_complex_ele_two)

						idx_i.append(i)

						diff_later = int(amount_trd) - int(amount_trd_sum_later)
						path_complex_ele_fth.append(addrs_src)
						path_complex_ele_fth.append(abs(diff_later))
						path_complex_ele_fth.append(addrs_trk)

					elif amount_trd < amount_trd_sum:

						diff_before = int(amount_trd) - int(amount_trd_sum_before)
						path_complex_ele_thr.append(N_filei[0])
						path_complex_ele_thr.append(abs(diff_before))
						path_complex_ele_thr.append(N_filei[3])

						print "amount_trd_sum_before: ", amount_trd_sum_before, "amount_trd_sum_later: ", amount_trd_sum_later, "amount_trd: ", N_filei[6]
						idx_i.append(i)

						amount_new = abs(diff_before - N_filei[6])
						M_file.append([N_filei[0], N_filei[1], a, N_filei[3], N_filei[4], b, amount_new])

			if path_complex_ele_thr != [] and path_complex_two != []:

				print "Opened contrats: ", amount_trd, " < Total traded contracts: ", amount_trd_sum, " -> Amount traded this contract", abs(amount_trd_sum_later - amount_trd_sum_before), "\n"				
				print "Complex path:\n", path_complex_two
				print "New edge added:\n", path_complex_ele_thr

			elif path_complex_ele_fth != [] and path_complex_two != []:

				print "Opened contrats: ", amount_trd, " >= Total traded contracts: ", amount_trd_sum, " -> Amount traded this contract", abs(amount_trd_sum_later - amount_trd_sum_before), "\n"
				print "Complex path:\n", path_complex_two
				print "Contracts opened:\n", path_complex_ele_fth

			if "Netted" not in str(status_addrs_trk_v) or len(status_addrs_trk_v) == 0:

				print "Single path: ", single_path
				idx_i = [0]