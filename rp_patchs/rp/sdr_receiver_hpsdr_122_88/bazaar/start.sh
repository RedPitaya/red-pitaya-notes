#! /bin/sh

address=`awk -F : '$5="FF"' OFS=: /sys/class/net/eth0/address`

echo 2 > /proc/sys/net/ipv4/conf/all/arp_announce
echo 1 > /proc/sys/net/ipv4/conf/all/arp_ignore
echo 2 > /proc/sys/net/ipv4/conf/all/rp_filter

ip link add mvl0 link eth0 address $address type macvlan mode passthru

/opt/redpitaya/www/apps/sdr_receiver_hpsdr_122_88/sdr-receiver-hpsdr eth0 1 1 1 1 1 1 1 1 &
/opt/redpitaya/www/apps/sdr_receiver_hpsdr_122_88/sdr-receiver-hpsdr mvl0 1 1 1 1 1 1 1 1 &
