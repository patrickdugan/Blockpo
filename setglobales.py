#! /usr/bin/python

import globales

def stuff():

    globales.key_path = ['addrs_src', 'lives_src', 'addrs_trk', 'lives_trk', 'amount_trd', 'opened_sett']
    globales.netted_status = ["LongPosNetted", "LongPosNettedPartly"]
    globales.open_incr_status = ["OpenLongPosition", "LongPosIncreased", "OpenShortPosition", "ShortPosIncreased"]