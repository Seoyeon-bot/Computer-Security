#!/usr/bin/env python3
from scapy.all import *
import sys 

def run(ttl):
	a = IP()
	a.dst = '2.2.2.2'
	a.ttl = ttl
	#a.ttl = int(sys.argv[1]) # take argument vector as string and conver to integer  -> so we can do ttl more than 3 times.

	b = ICMP()
	#send(a/b)

	a = sr1(a/b) # send completed packet. 
	print("src: " , a.src) # printout source 
