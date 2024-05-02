CFLAGS = -O3 -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard -D_GNU_SOURCE

all: sdr-transceiver-hpsdr sdr-transceiver-hpsdr-thetis sdr-receiver

sdr-transceiver-hpsdr: sdr-transceiver-hpsdr.c
	gcc $(CFLAGS) -o $@ $^ -lm -lpthread

sdr-transceiver-hpsdr-thetis: sdr-transceiver-hpsdr.c
	gcc $(CFLAGS) -DTHETIS -o $@ $^ -lm -lpthread

sdr-receiver: sdr-receiver.c
	gcc $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f sdr-transceiver-hpsdr sdr-transceiver-hpsdr-thetis
	rm -f sdr-receiver