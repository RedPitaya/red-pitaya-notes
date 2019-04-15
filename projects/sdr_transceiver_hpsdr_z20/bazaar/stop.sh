#!/bin/bash
killall sdr-transceiver-hpsdr

if [ -e "/dev/i2c-1" ]
then
  echo "HAMlab with i2c-mux is present."
  hamrf -i
  gpiorelay -i
else
  echo "STEMlab without i2c-mux is present."
fi