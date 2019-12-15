#!/bin/bash

# IPv4/IPv6 1st step
nft add rule inet filter input ct state established,related accept
nft add rule inet filter input ct state invalid drop
nft add rule inet filter input iif lo accept
nft add rule inet filter input ip protocol icmp icmp type { echo-request, echo-reply, destination-unreachable, router-advertisement, time-exceeded, parameter-problem } accept

# IPv4
nft add table ip filter
nft add chain ip filter input
nft add rule ip filter input tcp dport ssh ip saddr { 192.168.0.0-192.168.255.255, 172.16.0.0-172.16.255.255 , 10.0.0.0-10.255.255.255 } accept
nft add rule ip filter input udp dport ntp ip saddr { 10.0.0.0/8, 172.16.0.0/16, 192.168.0.0/16 } accept
nft add rule ip filter input tcp dport { http, https } ip saddr { 0.0.0.0/0 } accept

# IPv6
## no receive

# IPv4/IPv6 2nd step
nft chain inet filter input { policy drop \; }
