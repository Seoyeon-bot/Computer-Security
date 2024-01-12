#!/usr/bin/env python3
from scapy.all import *


def print_pkt(pkt):
  print_pkt.packet += 1
  print("---Packet sending/received --- {}\n".format(print_pkt.packet))
  pkt.show()
print_pkt.packet=0

#1.1B-1 task Capture only ICMP packet   
#pkt = sniff(iface='br-2fc7646697c0', filter='icmp', prn=print_pkt)

#•1.1B-2 Capture any TCP packet that comes from a particular IP and with a destination port number 23.
#pkt = sniff(iface='br-2fc7646697c0', filter='tcp && src host 10.9.0.6 && dst port 23', prn=print_pkt)

# task • Capture packets comes from or to go to a particular subnet. You can pick any subnet, such as 128.230.0.0/16; you should not pick the subnet that your VM is attached to. -> I chose subnet as 10.0.2.15

pkt = sniff(iface='br-2fc7646697c0', filter='net 128.230.0.0/16', prn=print_pkt)

