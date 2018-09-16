#! /usr/bin/python

import globales

def opening_filetxt(namefile):

	with open(namefile, "r") as file:
		M_file = [[int(digit) if digit.isdigit()
				   else digit for digit in line.split()] for line in file]

	return M_file

def stuff():

    globales.key_path = ['addrs_src', 'addrs_trk', 'status_src', 'status_trk', 'entry_price', 'exit_price', 'lives_src', 'lives_trk', 'amount_trd', 'edge_row', 'path_number']
    globales.key_path_fifo = ['addrs_src', 'addrs_trk', 'status_src', 'status_trk', 'entry_price', 'exit_price', 'lives_src', 'lives_trk', 'amount_trd', 'edge_row', 'path_number', 'ghost_edge']
    globales.netted_status_long = ["LongPosNetted", "LongPosNettedPartly"]
    globales.netted_status_short = ["ShortPosNetted", "ShortPosNettedPartly"]
    globales.open_incr_short = ["OpenShortPosition", "ShortPosIncreased"]
    globales.open_incr_long = ["OpenLongPosition", "LongPosIncreased"]
    globales.open_incr_long_short = ["OpenLongPosition", "LongPosIncreased", "OpenShortPosition", "ShortPosIncreased"]
    globales.all_netted_status = ["LongPosNetted", "LongPosNettedPartly", "ShortPosNetted", "ShortPosNettedPartly"]
    globales.incr_positions = ["LongPosIncreased", "ShortPosIncreased"]
    globales.addresses_vector = opening_filetxt("graphInfoAddresses.txt")
    globales.database_matrix = opening_filetxt("graphInfoSixth.txt")
    globales.ghost_edges = opening_filetxt('data_ghost_plot_python.txt')
    globales.lives_data = ['address', 'status', 'lives_contracts', 'entry_price', 'edge_row', 'path_number']