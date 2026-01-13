#!/bin/bash

# Used projects
# sdr_receiver_hpsdr
# sdr_receiver_hpsdr_z20
# sdr_receiver_hpsdr_122_88
# sdr_transceiver_hpsdr
# sdr_transceiver_hpsdr_z20
# sdr_transceiver_hpsdr_122_88
# vna
# vna_z20
# vna_122_88
# sdr_transceiver
# sdr_transceiver_z20
# sdr_transceiver_122_88

cd ..

# Update links and info
git apply rp_patchs/vna.patch

# Copy bazaar projects
cp -rv ./projects/sdr_receiver_hpsdr ./projects/sdr_receiver_hpsdr_z20
cp -rv ./projects/sdr_receiver_hpsdr/bazaar ./projects/sdr_receiver_hpsdr_122_88

cp -rv ./projects/sdr_transceiver_hpsdr ./projects/sdr_transceiver_hpsdr_z20
cp -rv ./projects/sdr_transceiver_hpsdr/bazaar ./projects/sdr_transceiver_hpsdr_122_88

cp -rv ./projects/vna ./projects/vna_z20
cp -rv ./projects/vna/bazaar ./projects/vna_122_88

cp -rv ./projects/sdr_transceiver ./projects/sdr_transceiver_z20
cp -rv ./projects/sdr_transceiver/bazaar ./projects/sdr_transceiver_122_88



cp -rv ./rp_patchs/rp/* ./projects


# Patch makefile
# need for build DTS
cp -f Makefile Makefile.bak
sed -i "s/xilinx_v2025.2/xilinx-v2025.1/g" Makefile
cat ./rp_patchs/Makefile >> ./Makefile

# Patch devicetree
cp -f scripts/devicetree.tcl scripts/devicetree.tcl.bak
# Enable overlay in DT
awk 'NR==21{print "hsi set_property CONFIG.dt_overlay true [hsi get_os]"}1' scripts/devicetree.tcl > scripts/devicetree.tcl.tmp
cat scripts/devicetree.tcl.tmp > scripts/devicetree.tcl

cp -f scripts/bitstream.tcl scripts/bitstream.tcl.bak
awk 'NR==16{print "write_cfgmem -format BIN -interface SMAPx32 -disablebitswap -loadbit \"up 0x0 tmp/$project_name.bit\" -file tmp/$project_name.bit.bin"}1' scripts/bitstream.tcl > scripts/bitstream.tcl.tmp
cat scripts/bitstream.tcl.tmp > scripts/bitstream.tcl


# Patch sdr_receiver_hpsdr, sdr_receiver_hpsdr_z20, sdr_receiver_hpsdr_122_88
sed -i "s/'sdr_receiver_hpsdr'/'sdr_receiver_hpsdr'/g" projects/sdr_receiver_hpsdr/bazaar/index.html
sed -i "s/'sdr_receiver_hpsdr'/'sdr_receiver_hpsdr_122_88'/g" projects/sdr_receiver_hpsdr_122_88/bazaar/index.html
sed -i "s/'sdr_receiver_hpsdr'/'sdr_receiver_hpsdr_z20'/g" projects/sdr_receiver_hpsdr_z20/bazaar/index.html

sed -i "s/sdr_receiver_hpsdr/sdr_receiver_hpsdr/g" projects/sdr_receiver_hpsdr/bazaar/src/main.c
sed -i "s/sdr_receiver_hpsdr/sdr_receiver_hpsdr_122_88/g" projects/sdr_receiver_hpsdr_122_88/bazaar/src/main.c
sed -i "s/sdr_receiver_hpsdr/sdr_receiver_hpsdr_z20/g" projects/sdr_receiver_hpsdr_z20/bazaar/src/main.c

sed -i "s/sdr_receiver_hpsdr/sdr_receiver_hpsdr/g" projects/sdr_receiver_hpsdr/bazaar/start.sh
# Was copyed from rp directory
#sed -i "s/sdr_receiver_hpsdr/sdr_receiver_hpsdr_122_88/g" projects/sdr_receiver_hpsdr_122_88/bazaar/start.sh
sed -i "s/sdr_receiver_hpsdr/sdr_receiver_hpsdr_z20/g" projects/sdr_receiver_hpsdr_z20/bazaar/start.sh




# Patch sdr_transceiver_hpsdr, sdr_transceiver_hpsdr_z20, sdr_transceiver_hpsdr_122_88
sed -i "s/'sdr_transceiver_hpsdr'/'sdr_transceiver_hpsdr'/g" projects/sdr_transceiver_hpsdr/bazaar/index.html
sed -i "s/'sdr_transceiver_hpsdr'/'sdr_transceiver_hpsdr_122_88'/g" projects/sdr_transceiver_hpsdr_122_88/bazaar/index.html
sed -i "s/'sdr_transceiver_hpsdr'/'sdr_transceiver_hpsdr_z20'/g" projects/sdr_transceiver_hpsdr_z20/bazaar/index.html

sed -i "s/sdr_transceiver_hpsdr/sdr_transceiver_hpsdr/g" projects/sdr_transceiver_hpsdr/bazaar/src/main.c
sed -i "s/sdr_transceiver_hpsdr/sdr_transceiver_hpsdr_122_88/g" projects/sdr_transceiver_hpsdr_122_88/bazaar/src/main.c
sed -i "s/sdr_transceiver_hpsdr/sdr_transceiver_hpsdr_z20/g" projects/sdr_transceiver_hpsdr_z20/bazaar/src/main.c


# Patch vna, vna_z20, vna_122_88
sed -i "s/'vna'/'vna'/g" projects/vna/bazaar/index.html
sed -i "s/'vna'/'vna_122_88'/g" projects/vna_122_88/bazaar/index.html
sed -i "s/'vna'/'vna_z20'/g" projects/vna_z20/bazaar/index.html


sed -i "s/vna/vna/g" projects/vna/bazaar/src/main.c
sed -i "s/vna/vna_122_88/g" projects/vna_122_88/bazaar/src/main.c
sed -i "s/vna/vna_z20/g" projects/vna_z20/bazaar/src/main.c

sed -i "s/vna\//vna\//g" projects/vna/bazaar/start.sh
sed -i "s/vna\//vna_122_88\//g" projects/vna_122_88/bazaar/start.sh
sed -i "s/vna\//vna_z20\//g" projects/vna_z20/bazaar/start.sh


# Patch sdr_transceiver, sdr_transceiver_z20, sdr_transceiver_122_88
sed -i "s/'sdr_transceiver'/'sdr_transceiver'/g" projects/sdr_transceiver/bazaar/index.html
sed -i "s/'sdr_transceiver'/'sdr_transceiver_122_88'/g" projects/sdr_transceiver_122_88/bazaar/index.html
sed -i "s/'sdr_transceiver'/'sdr_transceiver_z20'/g" projects/sdr_transceiver_z20/bazaar/index.html

sed -i "s/sdr_transceiver/sdr_transceiver/g" projects/sdr_transceiver/bazaar/src/main.c
sed -i "s/sdr_transceiver/sdr_transceiver_122_88/g" projects/sdr_transceiver_122_88/bazaar/src/main.c
sed -i "s/sdr_transceiver/sdr_transceiver_z20/g" projects/sdr_transceiver_z20/bazaar/src/main.c

sed -i "s/sdr_transceiver/sdr_transceiver/g" projects/sdr_transceiver/bazaar/start.sh
sed -i "s/sdr_transceiver/sdr_transceiver_122_88/g" projects/sdr_transceiver_122_88/bazaar/start.sh
sed -i "s/sdr_transceiver/sdr_transceiver_z20/g" projects/sdr_transceiver_z20/bazaar/start.sh
