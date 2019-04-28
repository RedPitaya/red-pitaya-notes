#!/bin/bash
killall -q sdr-transceiver-hpsdr

rw

if [ -e "/dev/i2c-1" ]
then
  hamrf -i
  gpiorelay -i
  rm /opt/redpitaya/www/apps/hamlab.device
else
  rm /opt/redpitaya/www/apps/stemlab.device
fi

ro