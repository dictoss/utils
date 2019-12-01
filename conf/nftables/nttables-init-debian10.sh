#!/bin/bash

# IPv4
nft add rule ip filter INPUT ct state established,related accept
nft add rule ip filter INPUT ct state invalid drop
nft add rule ip filter INPUT iif lo accept
nft add rule ip filter INPUT ip protocol icmp icmp type { destination-unreachable, router-advertisement, time-exceeded, parameter-problem } accept
nft add rule ip filter INPUT tcp dport ssh accept
nft add rule ip filter INPUT tcp dport { http, https } accept
nft chain ip filter INPUT { policy drop \; }

# IPv6
nft add rule ip6 filter INPUT ct state established,related accept
nft add rule ip6 filter INPUT ct state invalid drop
nft add rule ip6 filter INPUT iif lo accept
nft chain ip6 filter INPUT { policy drop \; }
