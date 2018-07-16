#! /usr/bin/python

import globales

def stuff():

    globales.key_path = ['addrs_src', 'addrs_trk', 'status_src', 'status_trk', 'entry_price', 'exit_price', 'amount_trd', 'opened_sett']
    globales.netted_status_long = ["LongPosNetted", "LongPosNettedPartly"]
    globales.netted_status_short = ["ShortPosNetted", "ShortPosNettedPartly"]
    globales.open_incr_short = ["OpenShortPosition", "ShortPosIncreased"]
    globales.open_incr_long = ["OpenLongPosition", "LongPosIncreased"]
    globales.open_incr_long_short = ["OpenLongPosition", "LongPosIncreased", "OpenShortPosition", "ShortPosIncreased"]
    globales.all_netted_status = ["LongPosNetted", "LongPosNettedPartly", "ShortPosNetted", "ShortPosNettedPartly"]
    globales.incr_positions = ["LongPosIncreased", "ShortPosIncreased"]
