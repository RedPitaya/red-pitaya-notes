#!/bin/sh
rmdir /configfs/device-tree/overlays/Full 2> /dev/null
rm /tmp/loaded_fpga.inf 2> /dev/null
sleep 0.5s
/opt/redpitaya/bin/fpgautil -b /opt/redpitaya/www/apps/sdr_transceiver_hpsdr_z20/fpga.bit.bin -o /opt/redpitaya/www/apps/sdr_transceiver_hpsdr_z20/fpga.dtbo -n Full
