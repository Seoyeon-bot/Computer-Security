#!/usr/bin/env python3
from scapy.all import *
a = IP() 
a.dst = '1.2.3.4' 
b = ICMP() 
p = a/b 

ls(a)
send(p, iface='br-2fc7646697c0')


