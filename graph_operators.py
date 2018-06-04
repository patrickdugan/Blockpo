#! /usr/bin/python

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

def amounts_addresses_status(v):

    addrs_trk  = v[0] if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased" else v[3]
    status_trk = v[1] if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased" else v[4] 
    lives_trk  = int(v[2]) if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased" else int(v[5])

    addrs_src  = v[3] if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased" else v[0]
    status_src = v[4] if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased" else v[1]
    lives_src  = int(v[5]) if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased" else int(v[2])

    amount_trd = int(v[6])

    return addrs_trk, status_trk, lives_trk, addrs_src, status_src, lives_src, amount_trd

def amounts_addresses_status_inloop(v, addrs_trk):

    status_trki = v[1] if addrs_trk == v[0] else v[4]
    status_srci = v[4] if addrs_trk == v[0] else v[1]
    
    addrs_trki  = v[0] if addrs_trk == v[0] else v[3]
    addrs_srci  = v[3] if addrs_trk == v[0] else v[0]

    lives_trki  = int(v[2]) if addrs_trk == v[0] else int(v[5])
    lives_srci  = int(v[5]) if addrs_trk == v[0] else int(v[2])

    amount_trdi = int(v[6])

    return addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi