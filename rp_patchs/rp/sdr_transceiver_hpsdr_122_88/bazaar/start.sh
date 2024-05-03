#! /bin/sh

/opt/redpitaya/www/apps/sdr_transceiver_hpsdr_122_88/stop.sh

/opt/redpitaya/www/apps/sdr_transceiver_hpsdr_122_88/sdr-transceiver-hpsdr 1 2 2 2 1 2 &

address=`awk -F : '$5="FF"' OFS=: /sys/class/net/eth0/address`

echo 2 > /proc/sys/net/ipv4/conf/all/arp_announce
echo 1 > /proc/sys/net/ipv4/conf/all/arp_ignore
echo 2 > /proc/sys/net/ipv4/conf/all/rp_filter

ip link add mvl0 link eth0 type macvlan mode bridge
ip link set dev mvl0 up

/opt/redpitaya/www/apps/sdr_transceiver_hpsdr_122_88/sdr-receiver mvl0 &
