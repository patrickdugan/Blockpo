#! /usr/bin/python

from collections import defaultdict
import numpy as np
import os
import sys
import random
import json

# ###################################################################
print "#----------------Status vector by address---------------------#"

status_map = np.genfromtxt('graphInfoFirst.txt', dtype=None)
pairs = np.column_stack((status_map[:, 0], status_map[:, 1]))
status_by_addrs = defaultdict(list)

for addrs, status in pairs:
    status_by_addrs[addrs].append(status)

os.remove("statusByAddress.txt")
for addrs, status in status_by_addrs.iteritems():
    with open('statusByAddress.txt', 'a') as f:
        print >> f, addrs, ' '.join(status)

addressj = np.genfromtxt('graphInfoAddresses.txt', dtype=None)
for j in range(len(addressj)):
    print addressj[j], ":\t", status_by_addrs[addressj[j]]

###################################################################

addrs_srcs = []
status_byaddress = ""
sources_vector = []
array_status_byaddress = []

for addrs, status in status_by_addrs.iteritems():

    addrs_srcs = addrs, ':'.join(status)
    status_byaddress = addrs_srcs[1]

    status_vector = []
    for j in status_byaddress.split(":"):
        status_vector.append(j)

    status_vectorl = ''.join(status_vector)

    print "------------------------------------------------"
    print "Address:", addrs, "\n"
    if ("OpenShortPosition" in status_vectorl) or ("ShortPosIncreased" in status_vectorl):
        sources_vector.append(addrs)

        print "Conclusion:"
        print "This address can be a source!!\n"
        print "Status vector for ", addrs, ":"
        print status_vector

    else:
        print "Conclusion:"
        print "This address can not be a source!!\n"
        print "Status vector for ", addrs, ":"
        print status_vector, "\n"

    print "\n", addrs, ": ", status_vector

print "\nNumber of sources:\t", len(sources_vector)

###################################################################

with open('graphInfoFourth.txt') as file:
    M_json = [[int(digit) if digit.isdigit()
               else digit for digit in line.split()] for line in file]

Mv_json = []
for j in range(len(M_json)):
    Mv_json.append(M_json[j][0])
    Mv_json.append(M_json[j][2])
M_unique = np.unique(Mv_json)

Keys = ['id', 'label', 'color']
Values = []
Dict = dict(zip(Keys, Values))
Data = []

x = 0
while x < len(M_unique):
    x += 1

    if M_unique[x-1] in str(sources_vector):
        MoreValues = [M_unique[x-1], M_unique[x-1], '#97C2FC']
    else:
        MoreValues = [M_unique[x-1], M_unique[x-1], '#FF3300']

    Dict = dict(zip(Keys, MoreValues))
    Data.append(Dict)

K_json = ['from', 'to', 'label', 'arrows']
V_json = []
D_json = dict(zip(K_json, V_json))
L_json = []

for k in range(len(M_json)):
    X = [M_json[k][0], M_json[k][2], '%d' % M_json[k][1],
         '{middle:{scaleFactor:0.5},from:true},smooth:{type: "curvedCW", roundness: %0.2f}' % random.uniform(0, 1)]
    D_json = dict(zip(K_json, X))
    L_json.append(D_json)

os.remove('nodes_edges.txt')
with open('nodes_edges.txt', 'w') as outfile:
    outfile.write('Nodes:\n')
    json.dump(Data, outfile)
    outfile.write('\n\n')
    outfile.write('Edges:\n')
    json.dump(L_json, outfile)
