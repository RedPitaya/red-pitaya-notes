project=sdr_transceiver_hpsdr
project1=hamlab_sdr_transceiver_hpsdr
server=sdr-transceiver-hpsdr

cp -a projects/$project/bazaar $project1
arm-linux-gnueabihf-gcc -shared -Wall -fPIC -Os -s $project1/src/main.c -o $project1/controllerhf.so
arm-linux-gnueabihf-gcc -static -O3 -march=armv7-a -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard projects/$project/server/$server.c -D_GNU_SOURCE -Iprojects/$project/server -lm -lpthread -o $project1/$server
cp tmp/$project.bit $project1

build_number=`git rev-list HEAD --count`
revision=`git log -n1 --pretty=%h`

sed -i "s/REVISION/$revision/; s/BUILD_NUMBER/$build_number/" $project1/info/info.json

zip -r $project1-0.94-$build_number.zip $project1
