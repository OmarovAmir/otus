#!/bin/bash

sudo nft add table inet adapter
sudo nft add chain inet adapter prerouting "{ type filter hook prerouting priority mangle; }"
sudo nft add rule inet adapter prerouting meta l4proto { tcp, udp } socket transparent 1 meta mark set 1 accept
sudo ip rule add fwmark 1 lookup 1
sudo ip route add local 0.0.0.0/0 dev lo table 1