#!/bin/bash

BUILD=$1
SRC=$2
DEST=$3
NAME=$4
REPO=$5

# Copy bazaar projects
cp -rv $BUILD/$SRC $BUILD/$DEST
mv -f $BUILD/$DEST/sdr-transceiver-hpsdr-thetis $BUILD/$DEST/sdr-transceiver-hpsdr
cp -rvf $REPO/rp_patchs/rp/$DEST $BUILD

