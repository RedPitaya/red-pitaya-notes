#!/bin/bash
rw

if [ -e "/dev/i2c-1" ]
then
  touch /opt/redpitaya/www/apps/hamlab.device
  rohamrf -s
  gpiorelay -s
else
  touch /opt/redpitaya/www/apps/stemlab.device
fi

ro

killall -q sdr-transceiver-hpsdr

if [[ $(fw_printenv hw_rev) = *"122-16"* ]]; then
  cat /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr_transceiver_hpsdr_z20.bit > /dev/xdevcfg
  /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr-transceiver-hpsdr 1 2 2 1 2 Z20 &
else
  cat /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr_transceiver_hpsdr.bit > /dev/xdevcfg
  /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr-transceiver-hpsdr 1 2 2 1 2 &
fi
