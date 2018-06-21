#! /usr/bin/python

import globales

def stuff():

    globales.key_path = ['addrs_src', 'lives_src', 'addrs_trk', 'lives_trk', 'amount_trd', 'opened_sett']
    globales.netted_status_long = ["LongPosNetted", "LongPosNettedPartly"]
    globales.netted_status_short = ["ShortPosNetted", "ShortPosNettedPartly"]
    globales.open_incr_short = ["OpenShortPosition", "ShortPosIncreased"]
    globales.open_incr_long = ["OpenLongPosition", "LongPosIncreased"]
    globales.all_netted_status = ["LongPosNetted", "LongPosNettedPartly", "ShortPosNetted", "ShortPosNettedPartly"]