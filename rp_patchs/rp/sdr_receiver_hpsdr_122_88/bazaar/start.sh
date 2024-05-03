#! /bin/sh

/opt/redpitaya/www/apps/sdr_receiver_hpsdr_122_88/stop.sh

address=`awk -F : '$5="FF"' OFS=: /sys/class/net/eth0/address`

echo 2 > /proc/sys/net/ipv4/conf/all/arp_announce
echo 1 > /proc/sys/net/ipv4/conf/all/arp_ignore
echo 2 > /proc/sys/net/ipv4/conf/all/rp_filter

ip link add mvl0 link eth0 type macvlan mode bridge
ip link set dev mvl0 up

/opt/redpitaya/www/apps/sdr_receiver_hpsdr_122_88/sdr-receiver-hpsdr eth0 1 1 1 1 1 1 1 1 &
/opt/redpitaya/www/apps/sdr_receiver_hpsdr_122_88/sdr-receiver-hpsdr mvl0 1 1 1 1 1 1 1 1 &
