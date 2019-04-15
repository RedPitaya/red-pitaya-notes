#!/bin/bash
if [ -e "/dev/i2c-1" ]
then
  echo "HAMlab with i2c-mux is present."
  hamrf -s
  gpiorelay -s
else
  echo "STEMlab without i2c-mux is present."
fi

killall sdr-transceiver-hpsdr

if [[ $(fw_printenv hw_rev) = *"122-16"* ]]; then
  cat /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr_transceiver_hpsdr_z20.bit > /dev/xdevcfg
  /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr-transceiver-hpsdr 1 2 2 1 2 Z20 &
else
  cat /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr_transceiver_hpsdr.bit > /dev/xdevcfg
  /opt/redpitaya/www/apps/stemlab_sdr_transceiver_hpsdr/sdr-transceiver-hpsdr 1 2 2 1 2 &
fi
