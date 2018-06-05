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

    if v[4] == "OpenShortPosition" or v[4] == "ShortPosIncreased":

        addrs_trk  = v[0]
        status_trk = v[1]
        lives_trk  = int(v[2]) 

        addrs_src  = v[3] 
        status_src = v[4] 
        lives_src  = int(v[5]) 
    else:
        addrs_trk  = v[3]
        status_trk = v[4] 
        lives_trk  = int(v[5])

        addrs_src  = v[0]
        status_src = v[1]
        lives_src  = int(v[2])

    amount_trd = int(v[6])

    return addrs_trk, status_trk, lives_trk, addrs_src, status_src, lives_src, amount_trd

def amounts_addresses_status_inloop(v, addrs_trk):

    if addrs_trk == v[0]:   
    
        status_trki = v[1] 
        status_srci = v[4] 

        addrs_trki  = v[0] 
        addrs_srci  = v[3] 

        lives_trki  = int(v[2]) 
        lives_srci  = int(v[5]) 
    else:
        status_trki = v[4]
        status_srci = v[1]
    
        addrs_trki  = v[3]
        addrs_srci  = v[0]

        lives_trki  = int(v[5])
        lives_srci  = int(v[2])

    amount_trdi = int(v[6])

    return addrs_trki, status_trki, lives_trki, addrs_srci, status_srci, lives_srci, amount_trdi

def looking_for_netted(i, m, addrs_srci):
    
    for k in xrange(i+1, len(m)):
                                
        u = []
        u = m[:][k]
        s = u[1] if addrs_srci == u[0] else u[4]

        if addrs_srci in str(u):

            print "|index i:", k,"|addrs_srci:",addrs_srci,"|status_srci:",s,"|amount_trdi:",u[6],"|\n"
            print u[0], u[1], u[2], u[3], u[4], u[5], u[6]
        
        return