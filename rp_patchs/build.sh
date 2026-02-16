# For vivado 2020.1
function buildDTBO(){
    DEST_PATH=$1
    NAME=$2
    rm -rf $DEST_PATH/dts 2> /dev/null
    mkdir -p $DEST_PATH/dts
    cp -r $REPO_DIR/tmp/$NAME.tree/* $DEST_PATH/dts
# patch dtsi file
    sed -i 's/#address-cells/\/\/#address-cells/g' $DEST_PATH/dts/pl.dtsi
    sed -i 's/#size-cells/\/\/#size-cells/g' $DEST_PATH/dts/pl.dtsi
    sed -i 's/.bin/fpga.bit.bin/g' $DEST_PATH/dts/pl.dtsi
    dtc -@ -I dts -O dtb -o $DEST_PATH/fpga.dtbo $DEST_PATH/dts/pl.dtsi
	exit 1
}

# For vivado 2020.1
function convertFPGA(){
    DEST_PATH=$1
    NAME=$2
    cp $REPO_DIR/tmp/$PRJ.bit $DEST_PATH/fpga.bit
    echo -n "all:{ $DEST_PATH/fpga.bit }" >  $CUR_DIR/build/fpga.bif
    bootgen -image $CUR_DIR/build/fpga.bif -arch zynq -process_bitstream bin -o $DEST_PATH/fpga.bit.bin -w
    rm $CUR_DIR/build/fpga.bif
}

function convertFPGA_2025(){
    DEST_PATH=$1
    NAME=$2
    mkdir -p $DEST_PATH
    cp $REPO_DIR/tmp/$PRJ.bit.bin $DEST_PATH/fpga.bit.bin
}

function makeBazar(){
    DEST_PATH=$1
    NAME=$2
	mkdir -p $DEST_PATH
    cp -a $REPO_DIR/projects/$NAME/bazaar/* $DEST_PATH

    arm-linux-gnueabihf-gcc -shared -Wall -fPIC -Os $DEST_PATH/src/main.c -o $DEST_PATH/controllerhf.so

    rm -rf $DEST_PATH/src

    version=1.0-`date +%Y%m%d`
    sed -i "s/REVISION/$GIT_SHORT/; s/VERSION/$version/"  $DEST_PATH/info/info.json
}

function makeServer(){
    DEST_PATH=$1
    NAME=$2
    SERVER=$3
    DEST_SERVER=$4
	FLAGS=$5

    arm-linux-gnueabihf-gcc  -O3 -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard $FLAGS $REPO_DIR/projects/$NAME/server/$SERVER.c -D_GNU_SOURCE -lm -lpthread -o $DEST_PATH/$DEST_SERVER
}



sdr_receiver_hpsdr=true
sdr_receiver_hpsdr_122_88=true
sdr_receiver_hpsdr_z20=true

sdr_transceiver_hpsdr=true
sdr_transceiver_hpsdr_122_88=true
sdr_transceiver_hpsdr_z20=true

vna=true
vna_122_88=true
vna_z20=true

sdr_transceiver=true
sdr_transceiver_122_88=true
sdr_transceiver_z20=true

cd ..

JOBS=`nproc 2> /dev/null || echo 1`

source /media/500gb/Xilinx/2025.1/Vitis/settings64.sh

CUR_DIR=$(pwd)

mkdir -p $CUR_DIR/build

REPO_DIR=$CUR_DIR/REPO
BUILD_DIR=$CUR_DIR/build

# For local assembly, you need to comment out this line.
export REPO_DIR=$CUR_DIR

cd $REPO_DIR

git rev-parse --short HEAD > $REPO_DIR/gitrev.txt

GIT_SHORT=$(cat $REPO_DIR/gitrev.txt)

echo $BUILD_NUMBER
echo $GIT_SHORT

cd rp_patchs
./patch.sh
cd $REPO_DIR

if [ "$sdr_receiver_hpsdr" = true ]; then
PRJ=sdr_receiver_hpsdr
PRJ_SERVER=sdr-receiver-hpsdr
PRJ_SERVER_DEST=sdr-receiver-hpsdr
DEST=$CUR_DIR/build/sdr_receiver_hpsdr

make NAME=$PRJ clean
make NAME=$PRJ bit

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

if [ "$sdr_receiver_hpsdr_z20" = true ]; then
PRJ=sdr_receiver_hpsdr_z20
PRJ_SERVER=sdr-receiver-hpsdr
PRJ_SERVER_DEST=sdr-receiver-hpsdr
DEST=$CUR_DIR/build/sdr_receiver_hpsdr_z20

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

if [ "$sdr_receiver_hpsdr_122_88" = true ]; then
PRJ=sdr_receiver_hpsdr_122_88
PRJ_SERVER=sdr-receiver-hpsdr
PRJ_SERVER_DEST=sdr-receiver-hpsdr
DEST=$CUR_DIR/build/sdr_receiver_hpsdr_122_88

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

echo "SDR Transceiver Hpsdr  Projects"

if [ "$sdr_transceiver_hpsdr" = true ]; then
PRJ=sdr_transceiver_hpsdr
PRJ_THETIS=sdr_transceiver_hpsdr_thetis
PRJ_SERVER=sdr-transceiver-hpsdr
PRJ_SERVER_DEST=sdr-transceiver-hpsdr
PRJ_SERVER_DEST_THETIS=sdr-transceiver-hpsdr-thetis
DEST=$CUR_DIR/build/sdr_transceiver_hpsdr

make NAME=$PRJ clean
make NAME=$PRJ bit

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST_THETIS -DTHETIS

$REPO_DIR/rp_patchs/patch_thetis_transceiver_hpsdr.sh $BUILD_DIR $PRJ $PRJ_THETIS $PRJ_SERVER_DEST_THETIS $REPO_DIR

fi

if [ "$sdr_transceiver_hpsdr_z20" = true ]; then
PRJ=sdr_transceiver_hpsdr_z20
PRJ_THETIS=sdr_transceiver_hpsdr_thetis_z20
PRJ_SERVER=sdr-transceiver-hpsdr
PRJ_SERVER_DEST=sdr-transceiver-hpsdr
PRJ_SERVER_DEST_THETIS=sdr-transceiver-hpsdr-thetis
DEST=$CUR_DIR/build/sdr_transceiver_hpsdr_z20

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST_THETIS -DTHETIS

$REPO_DIR/rp_patchs/patch_thetis_transceiver_hpsdr.sh $BUILD_DIR $PRJ $PRJ_THETIS $PRJ_SERVER_DEST_THETIS $REPO_DIR

fi

if [ "$sdr_transceiver_hpsdr_122_88" = true ]; then
PRJ=sdr_transceiver_hpsdr_122_88
PRJ_THETIS=sdr_transceiver_hpsdr_thetis_122_88
PRJ_SERVER=sdr-transceiver-hpsdr
PRJ_SERVER_REC=sdr-receiver
PRJ_SERVER_DEST=sdr-transceiver-hpsdr
PRJ_SERVER_DEST_THETIS=sdr-transceiver-hpsdr-thetis
PRJ_SERVER_DEST_REC=sdr-receiver
DEST=$CUR_DIR/build/sdr_transceiver_hpsdr_122_88

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST_THETIS -DTHETIS
makeServer $DEST $PRJ $PRJ_SERVER_REC  $PRJ_SERVER_DEST_REC

$REPO_DIR/rp_patchs/patch_thetis_transceiver_hpsdr.sh $BUILD_DIR $PRJ $PRJ_THETIS $PRJ_SERVER_DEST_THETIS $REPO_DIR
fi

echo "VNA  Projects"

if [ "$vna" = true ]; then
PRJ=vna
PRJ_SERVER=vna
PRJ_SERVER_DEST=vna
DEST=$CUR_DIR/build/vna

make NAME=$PRJ clean
make NAME=$PRJ bit

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

if [ "$vna_z20" = true ]; then
PRJ=vna_z20
PRJ_SERVER=vna
PRJ_SERVER_DEST=vna
DEST=$CUR_DIR/build/vna_z20

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

if [ "$vna_122_88" = true ]; then
PRJ=vna_122_88
PRJ_SERVER=vna
PRJ_SERVER_DEST=vna
DEST=$CUR_DIR/build/vna_122_88

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

echo "SDR Transceiver Projects"

if [ "$sdr_transceiver" = true ]; then
PRJ=sdr_transceiver
PRJ_SERVER=sdr-transceiver
PRJ_SERVER_DEST=sdr-transceiver
DEST=$CUR_DIR/build/sdr_transceiver

make NAME=$PRJ clean
make NAME=$PRJ bit

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

if [ "$sdr_transceiver_z20" = true ]; then
PRJ=sdr_transceiver_z20
PRJ_SERVER=sdr-transceiver
PRJ_SERVER_DEST=sdr-transceiver
DEST=$CUR_DIR/build/sdr_transceiver_z20

make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

if [ "$sdr_transceiver_122_88" = true ]; then
PRJ=sdr_transceiver_122_88
PRJ_SERVER=sdr-transceiver
PRJ_SERVER_DEST=sdr-transceiver
DEST=$CUR_DIR/build/sdr_transceiver_122_88
make NAME=$PRJ clean
make NAME=$PRJ bit PART=xc7z020clg400-1

## for Vivado 2020
#make NAME=$PRJ devicetree.dtb
#buildDTBO $DEST $PRJ
#convertFPGA $DEST $PRJ

## for Vivado 2025
convertFPGA_2025 $DEST $PRJ
#

makeBazar $DEST $PRJ
makeServer $DEST $PRJ $PRJ_SERVER  $PRJ_SERVER_DEST
fi

cd $CUR_DIR/build

zip -r SDR-bundle_$BUILD_NUMBER-$GIT_SHORT.zip *
mv SDR-bundle_$BUILD_NUMBER-$GIT_SHORT.zip ../SDR-bundle-$BUILD_NUMBER-$GIT_SHORT.zip