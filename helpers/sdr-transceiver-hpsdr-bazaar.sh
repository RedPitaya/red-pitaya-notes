project=sdr-transceiver-hpsdr
project_z20=sdr_transceiver_hpsdr_z20
project_package=stemlab_sdr_transceiver_hpsdr
server=sdr-transceiver-hpsdr

cp -a projects/$project_z20/bazaar $project_package

arm-linux-gnueabihf-gcc -shared -Wall -fPIC -Os -s $project_package/src/main.c -o $project_package/controllerhf.so
arm-linux-gnueabihf-gcc -static -O3 -march=armv7-a -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard projects/$project_z20/server/$server.c -D_GNU_SOURCE -Iprojects/$project_z20/server -lm -lpthread -o $project_package/$server

cp tmp/$project.bit $project_package
cp tmp/$project_z20.bit $project_package

build_number=`git rev-list HEAD --count`
revision=`git log -n1 --pretty=%h`

sed -i "s/REVISION/$revision/; s/BUILD_NUMBER/$build_number/" $project_package/info/info.json

zip -r $project_package-0.98-$build_number.zip $project_package
