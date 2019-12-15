#!/bin/bash

# IPv4
nft add rule ip filter input ct state established,related accept
nft add rule ip filter input ct state invalid drop
nft add rule ip filter input iif lo accept
nft add rule ip filter input ip protocol icmp icmp type { echo-request, echo-reply, destination-unreachable, router-advertisement, time-exceeded, parameter-problem } accept
nft add rule ip filter input tcp dport ssh ip saddr { 192.168.0.0-192.168.255.255, 172.16.0.0-172.16.255.255 , 10.0.0.0-10.255.255.255 } accept
nft add rule ip filter input udp dport ntp ip saddr { 10.0.0.0/8, 172.16.0.0/16, 192.168.0.0/16 } accept
nft add rule ip filter input tcp dport { http, https } ip saddr { 0.0.0.0/0 } accept
nft chain ip filter input { policy drop \; }

# IPv6
nft add rule ip6 filter input ct state established,related accept
nft add rule ip6 filter input ct state invalid drop
nft add rule ip6 filter input iif lo accept
nft chain ip6 filter input { policy drop \; }
