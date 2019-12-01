#!/bin/bash

# IPv4
nft add rule ip filter INPUT ct state established,related accept
nft add rule ip filter INPUT ct state invalid drop
nft add rule ip filter INPUT iif lo accept
nft add rule ip filter INPUT ip protocol icmp icmp type { destination-unreachable, router-advertisement, time-exceeded, parameter-problem } accept
nft add rule ip filter INPUT tcp dport ssh ip saddr { 192.168.0.0-192.168.255.255, 172.16.0.0-172.16.255.255 , 10.0.0.0-10.255.255.255 } accept
nft add rule ip filter INPUT tcp dport { http, https } ip saddr { 0.0.0.0/0 } accept
nft chain ip filter INPUT { policy drop \; }

# IPv6
nft add rule ip6 filter INPUT ct state established,related accept
nft add rule ip6 filter INPUT ct state invalid drop
nft add rule ip6 filter INPUT iif lo accept
nft chain ip6 filter INPUT { policy drop \; }
