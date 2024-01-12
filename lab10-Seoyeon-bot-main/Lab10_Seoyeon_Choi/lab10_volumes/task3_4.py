#!/usr/bin/env python3

import sys
import os
from scapy.all import *


def spoof_pkt(pkt):
    # sniff and print out icmp echo request packet
    if ICMP in pkt and pkt[ICMP].type == 8:
        print("Original Packet.........")
        print("Source IP : ", pkt[IP].src)
        print("Destination IP :", pkt[IP].dst)

        # spoof an icmp echo reply packet
        # swap srcip and dstip
        ip = IP(src=pkt[IP].dst, dst=pkt[IP].src, ihl=pkt[IP].ihl)
        icmp = ICMP(type=0, id=pkt[ICMP].id, seq=pkt[ICMP].seq)
        data = pkt[Raw].load
        newpkt = ip/icmp/data

        print("Spoofed Packet.........")
        print("Source IP : ", newpkt[IP].src)
        print("Destination IP :", newpkt[IP].dst)

        send(newpkt, verbose=0)

# first ping 1.2.3.4      
#pkt = sniff(iface= 'br-2fc7646697c0', filter='icmp and host 1.2.3.4' ,prn=spoof_pkt)

#second ping 10.9.0.99   # a non-existing host on the LAN
#pkt = sniff(iface= 'br-2fc7646697c0', filter='icmp and host 10.9.0.99' ,prn=spoof_pkt)

# third ping 8.8.8.8 
pkt = sniff(iface= 'br-2fc7646697c0', filter='icmp and host 8.8.8.8' ,prn=spoof_pkt)


print("pkt : " , pkt)

