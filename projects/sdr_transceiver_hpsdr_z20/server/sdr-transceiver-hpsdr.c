/*
19.04.2016 DC2PD: add code for bandpass and antenna switching via I2C.
22.08.2016 DL4AOI: add code for TX level switching via I2C.
22.08.2016 DL4AOI: output first four open collector outputs to the pins DIO4_P - DIO7_P of the extension connector E1.
02.09.2016 ON3VNA: add code for TX level switching via DS1803-10 (I2C).
21.09.2016 DC2PD: add code for controlling AD8331 VGA with MCP4725 DAC (I2C).
02.10.2016 DL9LJ: add code for controlling ICOM IC-735 (UART).
03.12.2016 KA6S: add CW keyer code.
16.08.2017 G8NJJ: add code for controlling G8NJJ Arduino sketch (I2C).
*/
/*
03.10.2016 DL8GM and DG8MG: Modified code for Charly 25 - 4 band transceiver board switching via I2C.
11.10.2016 DG8MG: Modified code for band independent switching of the two preamps on the Charly 25LC board.
15.11.2016 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/e6bcfe06d8e7f9191cce2b8f7463f82f81b0d3b0
19.11.2016 DG8MG: Changed LPF frequency ranges to cover the IARU region 1-3 band plan requirements.
08.12.2016 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/b478ace697b260edab1a2de4eb4e38ccfdbc5d18
12.12.2016 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/8d92eafdecda8046b36da44b18150edcda0b1afa
23.12.2016 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/899f9c9172b0a87a3638e529739f6232bafedb9f
14.01.2017 DG8MG: Changed I2C bus handling when no Charly 25LC board is present.
29.01.2017 DG8MG: Added support for the LPFs on the Charly 25AB board.
30.01.2017 DG8MG: Added audio codec support for Charly 25 hardware outside a HAMlab.
03.02.2017 DG8MG: Added basic support for the BPFs on the Charly 25 RX BPF board based on the first prototype board.
04.02.2017 DG8MG: Added support for RX1 and RX2 using the BPFs on the new Charly 25 RX BPF board (different layout and PCA9555 I/O routing).
16.02.2017 DG8MG: Added backward compatibility for the Charly 25LC (4 band version) board.
24.02.2017 DG8MG: Added 60m band usage together with the 40m LPF filter on a Charly 25LC board.
05.03.2017 DG8MG: Changed the behaviour of the RX/TX and PA relay switching routines to handle CW keying correctly.
29.03.2017 DG8MG: Modified code to make it compatible with Red Pitaya's commit: https://github.com/RedPitaya/red-pitaya-notes/commit/eec0f694700ba94e58640817fbd072737ad2d7bf
01.04.2017 DG8MG: Changed CW straight key behaviour for Charly 25 boards.
06.04.2017 DG8MG: Added support for Antenna 1 and Antenna 2 switching on the Charly 25AB board.
13.04.2017 DG8MG: Changed the behaviour of the RX BPF boards detection routine due to the solved I2C multiplexer issues in the HAMlab.
25.04.2017 DG8MG: Added HAMlab audio codec support.
26.04.2017 DG8MG: Added hardware detection routine for Charly 25 boards in and outside of a HAMlab.
11.06.2017 DG8MG: Extended the hardware detection routine, it's now reading the board id's and transfers them to the frontend software.
12.06.2017 DG8MG: Changed the CW keyer handling for Charly 25 boards due to sporadic PA blocking issues when starting CW.
14.06.2017 DG8MG: Extended the hardware detection routine, it's now also working with Charly 25 boards without ID chips.
06.08.2017 DG8MG: Added TCP protocol support for the transmission between the Red Pitaya device and the frontend software.
25.08.2017 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/b3c63cc2b5522cd72057414995a317a34efd6a23
15.09.2017 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/c8508a7ad0385c3db16322420472a5e787a9a68c
19.10.2017 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/892e9b941f8a4d21bb6f04359836de79e4bdf3e4
15.03.2018 DG8MG: Added support for the LPFs on the Charly 25PP board.
19.03.2018 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/4c9c20971f696f330b5e9742aff56dee70f5fb0b
03.04.2018 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/fd78f4169a19919dc59360f9eef0ffc850d3f226
25.04.2018 DG8MG: Added support for switching the RX BPF boards own RX/TX relay.
04.07.2018 DG8MG: Added support for the functionalities on the Charly 25PP extension board.
24.07.2018 DG8MG: Modified code to make it compatible with Pavel Demin's commit: https://github.com/pavel-demin/red-pitaya-notes/commit/77ca831432fd5f5d0f601fd4052f17e60bd49d7c
25.09.2018 DG8MG: Added support for the measurement head on the Charly 25PP extension board.
09.10.2018 DG8MG: Added command line parameter to override Charly 25 TRX board type detection.
27.10.2018 DG8MG: Added support to use the Red Pitaya internal slow ADC's for a measurement head if no Charly 25PP extension board is present.
24.11.2018 DG8MG: Improved the TCP protocol handling based on a patch from Christoph / DL1YCF.
29.11.2018 DG8MG: Extended the TCP protocol handling to support the Red Pitaya device detection over different subnets.
27.12.2018 DG8MG: Added support for the new STEMlab 122.88-16 SDR hardware.
*/

// DG8MG
// Define CHARLY25 for Charly 25 specific builds
#define CHARLY25

// Define CHARLY25LC_60M_BAND for 60m band usage together with the 40m LPF filter on a Charly 25LC board
// #define CHARLY25LC_60M_BAND

// Define CHARLY25_TCP together with CHARLY25 to support TCP as protocol between the Red Pitaya device and the frontend software
#define CHARLY25_TCP

// Define DEBUG_EP2 for endpoint 2 debug messages
// #define DEBUG_EP2

// Define DEBUG_ATT for ATT, PRE and ANT function call debug messages
// #define DEBUG_ATT

// Define DEBUG_BPF for BPF debug messages
// #define DEBUG_BPF

// Define DEBUG_CW for CW debug messages
// #define DEBUG_CW

// Define DEBUG_LPF for TX LPF debug messages
// #define DEBUG_LPF

// Define DEBUG_PA for PA & PTT debug messages
// #define DEBUG_PA

// Define DEBUG_PROT for HPSDR protocol debug messages
// #define DEBUG_PROT

// Define DEBUG_TCP to debug possible TCP problems
// #define DEBUG_TCP

// Define DEBUG_UDP to debug possible UDP problems
// #define DEBUG_UDP

// Define DEBUG_SEQ to check for sequence numbers of ep2 packets
// #define DEBUG_SEQ

// Define DEBUG_EXT for Extension board debug messages
// #define DEBUG_EXT

// Define DEBUG_BCD for BCD band encoder debug messages
// #define DEBUG_BCD

// Define DEBUG_ADC for measuring head ADC debug messages
// #define DEBUG_ADC

// Define DEBUG_ADC_FWD for measuring head forward power debug messages
// #define DEBUG_ADC_FWD

// Define DEBUG_ADC_REF for measuring head reflected power debug messages
// #define DEBUG_ADC_REF

// Define DEBUG_ADC_TOTAL_CUR for measuring head forward power debug messages
// #define DEBUG_ADC_TOTAL_CUR

// Define DEBUG_ADC_PA_CUR for measuring head reflected power debug messages
// #define DEBUG_ADC_PA_CUR
// DG8MG

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/i2c-dev.h>


#define I2C_SLAVE       0x0703 /* Use this slave address */
#define I2C_SLAVE_FORCE 0x0706 /* Use this slave address, even if it is already in use by a driver! */

#ifndef CHARLY25
#define ADDR_PENE 0x20 /* PCA9555 address 0 */
#define ADDR_ALEX 0x21 /* PCA9555 address 1 */
#define ADDR_LEVEL 0x22 /* PCA9555 address 2 */
#define ADDR_MISC 0x23 /* PCA9555 address 3 */
#define ADDR_DRIVE 0x28 /* DS1803 address 0 */
#define ADDR_CODEC 0x1A /* WM8731 or TLV320AIC23B address 0 */
#define ADDR_DAC0 0x60 /* MCP4725 address 0 */
#define ADDR_DAC1 0x61 /* MCP4725 address 1 */
#define ADDR_ARDUINO 0x40 /* G8NJJ Arduino sketch */
#endif

#ifdef CHARLY25
#define SDR_APP_VERSION "20181227"

#define C25_I2C_DEVICE "/dev/i2c-0"
#define C25_HAMLAB_I2C_DEVICE "/dev/i2c-1"
#define HAMLAB_AUDIO_I2C_DEVICE "/dev/i2c-6"

// I2C address of the Charly 25 audio codec WM8731 or TLV320AIC23B address 0
#define AUDIO_CODEC_ADDR 0x1A

// I2C address of the Charly 25 trx frontend
#define C25_ADDR 0x20

// I2C address of the first Charly 25 receiver BPF board
#define C25_RX1_BPF_ADDR 0x21

// I2C address of the second Charly 25 receiver BPF board
#define C25_RX2_BPF_ADDR 0x22

// I2C address of the Charly 25 Extension board
#define C25_EXT_BOARD_ADDR 0x23

// I2C address of the Charly 25 BCD encoder
#define C25_BCD_ENCODER_ADDR 0x38

// I2C address of the ADC ADS 1015
#define C25_ADC_1015_ADDR 0x48

// I2C address of the Charly 25 TRX board ID chip
#define C25_TRX_ID_ADDR 0x1A

// I2C address of the new Charly 25 TRX board ID chip
#define C25_NEW_TRX_ID_ADDR 0x3A

// C25 filter frequencies
const uint32_t C25_6M_LOW_FREQ = 49995000;
const uint32_t C25_6M_HIGH_FREQ = 54005000;

const uint32_t C25_10M_LOW_FREQ = 27995000;
const uint32_t C25_10M_HIGH_FREQ = 29705000;

const uint32_t C25_12M_LOW_FREQ = 24885000;
const uint32_t C25_12M_HIGH_FREQ = 24995000;

const uint32_t C25_15M_LOW_FREQ = 20995000;
const uint32_t C25_15M_HIGH_FREQ = 21455000;

const uint32_t C25_17M_LOW_FREQ = 18063000;
const uint32_t C25_17M_HIGH_FREQ = 18173000;

const uint32_t C25_20M_LOW_FREQ = 13995000;
const uint32_t C25_20M_HIGH_FREQ = 14355000;

const uint32_t C25_30M_LOW_FREQ = 10095000;
const uint32_t C25_30M_HIGH_FREQ = 10160000;

const uint32_t C25_40M_LOW_FREQ = 6995000;
const uint32_t C25_40M_HIGH_FREQ = 7305000;

const uint32_t C25_60M_LOW_FREQ = 5055000;
const uint32_t C25_60M_HIGH_FREQ = 5455000;

const uint32_t C25_80M_LOW_FREQ = 3495000;
const uint32_t C25_80M_HIGH_FREQ = 4005000;

const uint32_t C25_160M_LOW_FREQ = 1795000;
const uint32_t C25_160M_HIGH_FREQ = 2005000;

uint16_t c25_adc_conversion_register[4] = {0};
void *c25_adc_handler(void *arg);

#ifdef CHARLY25_TCP
int sock_TCP_Server = -1;
int sock_TCP_Client = -1;
#endif

#endif

volatile uint32_t *rx_freq[2], *tx_freq, *alex, *tx_mux, *dac_freq;
volatile uint16_t *rx_rate, *rx_cntr, *tx_cntr, *dac_cntr, *adc_cntr;
volatile int16_t *tx_level, *dac_level;
volatile uint8_t *gpio_in, *gpio_out, *rx_rst, *tx_rst, *lo_rst;
volatile uint64_t *rx_data;
volatile uint32_t *tx_data, *dac_data;
volatile uint16_t *adc_data;
volatile int32_t *xadc;

const uint32_t freq_min = 0;
const uint32_t freq_max = 61440000;

int receivers = 1;
int rate = 0;
int sock_ep2;

struct sockaddr_in addr_ep6;

int enable_thread = 0;
int active_thread = 0;

void process_ep2(uint8_t *frame);
void *handler_ep6(void *arg);
void *handler_keyer(void *arg);

#ifndef CHARLY25
// variables to handle I2C devices
int i2c_fd;
int i2c_pene = 0;
int i2c_alex = 0;
int i2c_level = 0;
int i2c_misc = 0;
int i2c_drive = 0;
#endif

int i2c_codec = 0;

#ifndef CHARLY25
int i2c_dac0 = 0;
int i2c_dac1 = 0;
int i2c_arduino = 0;

uint16_t i2c_pene_data = 0;
uint16_t i2c_alex_data = 0;
uint16_t i2c_level_data = 0;
uint16_t i2c_misc_data = 0;
uint16_t i2c_drive_data = 0;
uint16_t i2c_dac0_data = 0xfff;
uint16_t i2c_dac1_data = 0xfff;

uint16_t i2c_ard_frx1_data = 0; /* rx 1 freq in kHz */
uint16_t i2c_ard_frx2_data = 0; /* rx 2 freq in kHz */
uint16_t i2c_ard_ftx_data = 0; /* tx freq in kHz */
uint32_t i2c_ard_ocant_data = 0; /* oc output and ant */
uint32_t i2c_ard_txatt_data = 0; /* tx attenuation and oddments  */
uint16_t i2c_ard_rxatt_data = 0; /* rx attenuation */

uint8_t log_table_lookup[256]; /* lookup table from linear scale to
								  6 bit / 0.5 dB attenuation */
#endif

uint8_t i2c_boost_data = 0;

uint8_t dac_level_data = 0;

uint8_t cw_int_data = 0;

#ifndef CHARLY25
uint8_t rx_att_data = 0;
#endif

uint8_t tx_mux_data = 0;
uint8_t tx_eer_data = 0;

uint16_t cw_hang = 0;
uint8_t cw_reversed = 0;
uint8_t cw_speed = 25;
uint8_t cw_mode = 0;
uint8_t cw_weight = 50;
uint8_t cw_spacing = 0;
uint8_t cw_delay = 0;
uint8_t cw_ptt = 0;

int cw_memory[2] = { 0, 0 };
int cw_ptt_delay = 0;

uint16_t rx_sync_data = 0;

ssize_t i2c_write_addr_data8(int fd, uint8_t addr, uint8_t data)
{
	uint8_t buffer[2];
	buffer[0] = addr;
	buffer[1] = data;
	return write(fd, buffer, 2);
}

ssize_t i2c_write_addr_data16(int fd, uint8_t addr, uint16_t data)
{
	uint8_t buffer[3];
	buffer[0] = addr;
	buffer[1] = data;
	buffer[2] = data >> 8;
	return write(fd, buffer, 3);
}

uint32_t freq_data[3] = {0, 0, 0};

#ifndef CHARLY25
ssize_t i2c_write_addr_data24(int fd, uint8_t addr, uint32_t data)
{
	uint8_t buffer[4];
	buffer[0] = addr;
	buffer[1] = data;
	buffer[2] = data >> 8;
	buffer[3] = data >> 16;
	return write(fd, buffer, 4);
}

ssize_t i2c_write_data16(int fd, uint16_t data)
{
	uint8_t buffer[2];
	buffer[0] = data >> 8;
	buffer[1] = data;
	return write(fd, buffer, 2);
}

uint16_t alex_data_rx = 0;
uint16_t alex_data_tx = 0;
uint16_t alex_data_0 = 0;
uint16_t alex_data_1 = 0;

/* calculate lookup table from drive scale value to 0.5 dB attenuation units */
void calc_log_lookup()
{
	int index;
	float value;
	uint8_t att;

	log_table_lookup[0] = 63; /* max att if no drive */
	for(index = 1; index < 256; ++index)
	{
		value = -40.0 * log10((float)index / 255.0);
		if(value > 63.0)
		{
			att = 63;
		}
		else
		{
			att = (uint8_t)value;
		}
		log_table_lookup[index] = att;
	}
}

void alex_write()
{
	uint32_t max = freq_data[1] > freq_data[2] ? freq_data[1] : freq_data[2];
	uint16_t manual = (alex_data_1 >> 15) & 0x01;
	uint16_t preamp = manual ? (alex_data_1 >> 6) & 0x01 : max > 50000000;
	uint16_t ptt = alex_data_0 & 0x01;
	uint32_t freq = 0;
	uint16_t hpf = 0, lpf = 0, data = 0;

	if (i2c_codec) return;

	freq = freq_data[1] < freq_data[2] ? freq_data[1] : freq_data[2];

	if (preamp) hpf = 0;
	else if (manual) hpf = alex_data_1 & 0x3f;
	else if (freq < 1416000) hpf = 0x20; /* bypass */
	else if (freq < 6500000) hpf = 0x10; /* 1.5 MHz HPF */
	else if (freq < 9500000) hpf = 0x08; /* 6.5 MHz HPF */
	else if (freq < 13000000) hpf = 0x04; /* 9.5 MHz HPF */
	else if (freq < 20000000) hpf = 0x01; /* 13 MHz HPF */
	else hpf = 0x02; /* 20 MHz HPF */

	data =
		ptt << 15 |
		((alex_data_0 >> 1) & 0x01) << 14 |
		((alex_data_0 >> 2) & 0x01) << 13 |
		((hpf >> 5) & 0x01) << 12 |
		((alex_data_0 >> 7) & 0x01) << 11 |
		(((alex_data_0 >> 5) & 0x03) == 0x01) << 10 |
		(((alex_data_0 >> 5) & 0x03) == 0x02) << 9 |
		(((alex_data_0 >> 5) & 0x03) == 0x03) << 8 |
		((hpf >> 2) & 0x07) << 4 |
		preamp << 3 |
		(hpf & 0x03) << 1 |
		1;

	if (alex_data_rx != data)
	{
		alex_data_rx = data;
		*alex = 1 << 16 | data;
	}

	freq = ptt ? freq_data[0] : max;

	if (manual) lpf = (alex_data_1 >> 8) & 0x7f;
	else if (freq > 32000000) lpf = 0x10; /* bypass */
	else if (freq > 22000000) lpf = 0x20; /* 12/10 meters */
	else if (freq > 15000000) lpf = 0x40; /* 17/15 meters */
	else if (freq > 8000000) lpf = 0x01; /* 30/20 meters */
	else if (freq > 4500000) lpf = 0x02; /* 60/40 meters */
	else if (freq > 2400000) lpf = 0x04; /* 80 meters */
	else lpf = 0x08; /* 160 meters */

	data =
		((lpf >> 4) & 0x07) << 13 |
		ptt << 12 |
		(~(alex_data_1 >> 7) & ptt) << 11 |
		(((alex_data_0 >> 8) & 0x03) == 0x02) << 10 |
		(((alex_data_0 >> 8) & 0x03) == 0x01) << 9 |
		(((alex_data_0 >> 8) & 0x03) == 0x00) << 8 |
		(lpf & 0x0f) << 4 |
		1 << 3;

	if (alex_data_tx != data)
	{
		alex_data_tx = data;
		*alex = 1 << 17 | data;
	}
}

uint16_t misc_data_0 = 0;
uint16_t misc_data_1 = 0;
uint16_t misc_data_2 = 0;

inline int lower_bound(int *array, int size, int value)
{
  int i = 0, j = size, k;
  while(i < j)
  {
	k = i + (j - i) / 2;
	if(value > array[k]) i = k + 1;
	else j = k;
  }
  return i;
}

void misc_write()
{
	uint16_t code[3], data = 0;
	int i, freqs[20] = { 1700000, 2100000, 3400000, 4100000, 6900000, 7350000, 9950000, 10200000, 12075000, 16209000, 16210000, 19584000, 19585000, 23170000, 23171000, 26465000, 26466000, 39850000, 39851000, 61000000 };

	for (i = 0; i < 3; ++i)
	{
		code[i] = lower_bound(freqs, 20, freq_data[i]);
		code[i] = code[i] % 2 ? code[i] / 2 + 1 : 0;
	}

	data |= (code[0] != code[1]) << 8 | code[2] << 4 | code[1];
	data |= (misc_data_1 & 0x18) << 8 | (misc_data_0 & 0x18) << 6;
	data |= (misc_data_2 & 0x03) << 13;

	if (i2c_misc_data != data)
	{
		i2c_misc_data = data;
		ioctl(i2c_fd, I2C_SLAVE, ADDR_MISC);
		i2c_write_addr_data16(i2c_fd, 0x02, data);
	}
}

int uart_fd;
uint8_t icom_band_data = 0;

void icom_write()
{
	uint32_t freq = freq_data[0];
	uint8_t band;
	uint8_t buffer[10] = { 0xfe, 0xfe, 0x04, 0xe0, 0x05, 0x00, 0x00, 0x08, 0x01, 0xfd };

	if (freq < 2000000) band = 0x01;       /* 160m */
	else if (freq < 4000000) band = 0x03;  /*  80m */
	else if (freq < 8000000) band = 0x07;  /*  40m */
	else if (freq < 11000000) band = 0x10; /*  30m */
	else if (freq < 15000000) band = 0x14; /*  20m */
	else if (freq < 20000000) band = 0x18; /*  17m */
	else if (freq < 22000000) band = 0x21; /*  15m */
	else if (freq < 26000000) band = 0x24; /*  12m */
	else band = 0x28;                     /*  10m */

	switch (band)
	{
	case 0x01:
		buffer[7] = 0x80;
		break;
	case 0x03:
		buffer[7] = 0x50;
		break;
	default:
		buffer[7] = 0x00;
	}

	buffer[8] = band;

	if (icom_band_data != band)
	{
		icom_band_data = band;
		if (uart_fd >= 0) write(uart_fd, buffer, 10);
	}
}
#endif

#ifdef CHARLY25
bool c25_mox = false;
uint16_t c25_i2c_data = 0;
uint16_t c25_ext_board_i2c_data = 0;
uint8_t c25_bcd_encoder_i2c_data = 0;
uint32_t c25_tx_freq = 0;
uint16_t c25_rx1_bpf_board_i2c_data = 0;
uint16_t c25_rx2_bpf_board_i2c_data = 0;

// I2C handling Charly 25
int i2c_fd;

// I2C handling HAMlab audio codec
int hamlab_audio_i2c_fd;

bool c25ab_trx_present = false;
bool c25lc_trx_present = false;
bool c25pp_trx_present = false;
bool c25_rx1_bpf_present = false;
bool c25_rx2_bpf_present = false;
bool c25_ext_board_present = false;
bool c25_bcd_encoder_present = false;
bool charly25_present = false;
bool hamlab_present = false;

// Charly 25 TRX board ID
uint8_t C25_TRX_ID = 0;
long c25_command_line_trx_board_id = 0;

// Charly 25 specific FPGA model differentiation
uint8_t c25_fpga_model = 0;

void c25_detect_hardware(void)
{
	uint8_t input_register[1];
	input_register[0] = 0;

	// Check if a HAMlab is present
	if ((i2c_fd = open(C25_HAMLAB_I2C_DEVICE, O_RDWR)) >= 0)
	{
		// HAMlab is present
		hamlab_present = true;
	}
	// Suspect that a Charly 25 is running stand-alone
	else
	{
		i2c_fd = open(C25_I2C_DEVICE, O_RDWR);
	}

	if (i2c_fd >= 0)
	{
		// If a FAKED board id via the command line is given, override the automatic trx board id detection
		if (c25_command_line_trx_board_id > -1)
		{
			C25_TRX_ID = c25_command_line_trx_board_id;
			fprintf(stderr, "Read FAKED board ID: %u from command line parameter!\n", C25_TRX_ID);

			switch (C25_TRX_ID)
			{
				case 128:
				case 129:
				c25lc_trx_present = true;
				break;

				case 130:
				c25ab_trx_present = true;
				break;

				case 131:
				c25pp_trx_present = true;
				// c25_ext_board_present = true;
				break;

				default:
				fprintf(stderr, "Charly 25 TRX board with FAKED unknown ID: %u!\nPrototype present?\n", C25_TRX_ID);
				break;
			}
		}
		// No FAKED board id is given, so work with the automatic trx board id detection
		else
		{
			// Uninvert input - default is 0xf0 at PCA9557 I/O chip
			ioctl(i2c_fd, I2C_SLAVE, C25_TRX_ID_ADDR);

			// Check if an ID chip on C25_TRX_ID_ADDR is present
			if (i2c_write_addr_data8(i2c_fd, 0x02, 0x00) < 0)
			{
				// it's not present there, so try it with an ID chip on C25_NEW_TRX_ID_ADDR
				ioctl(i2c_fd, I2C_SLAVE, C25_NEW_TRX_ID_ADDR);
			}

			// Check if it works with an ID chip on C25_NEW_TRX_ID_ADDR
			if (i2c_write_addr_data8(i2c_fd, 0x02, 0x00) >= 0)
			{
				// set pins for input
				i2c_write_addr_data8(i2c_fd, 0x03, 0xff);

				// address the input port register
				i2c_write_addr_data8(i2c_fd, 0x00, 0x00);

				// read the Charly 25 TRX board ID and model
				c25lc_trx_present = false;
				c25ab_trx_present = false;
				c25pp_trx_present = false;
				c25_ext_board_present = false;

				if (read(i2c_fd, input_register, 1) == 1)
				{
					C25_TRX_ID = input_register[0];

					switch (C25_TRX_ID)
					{
						case 128:
						case 129:
						c25lc_trx_present = true;
						break;

						case 130:
						c25ab_trx_present = true;
						break;

						case 131:
						c25pp_trx_present = true;
						// c25_ext_board_present = true;
						break;

						default:
						fprintf(stderr, "Charly 25 TRX board with unknown ID: %u found!\nPrototype present?\n", C25_TRX_ID);
						break;
					}
				}
				else
				{
					fprintf(stderr, "Charly 25 TRX ID chip - I2C read error!\n");
				}
			}
			else
			{
				// No ID chip is present
				fprintf(stderr, "No ID chip on the Charly 25 TRX board found!\n");
			}
		}

		if (ioctl(i2c_fd, I2C_SLAVE_FORCE, C25_ADDR) >= 0)
		{
			// set all pins to low and check if a Charly 25 TRX board is present
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) >= 0)
			{
				// If no HAMlab is present
				if (!hamlab_present)
				{
					// a Charly 25 stand-alone version is present
					charly25_present = true;
				}

				// configure all pins as output
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
			else
			{
				fprintf(stderr, "No Charly 25 TRX board found!\n");
			}
		}
		else
		{
			fprintf(stderr, "Charly 25 TRX - I2C ioctl error!\n");
		}

		// Detect RX BPF boards
		if (ioctl(i2c_fd, I2C_SLAVE, C25_RX1_BPF_ADDR) >= 0)
		{
			// set all pins to low
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) >= 0)
			{
				c25_rx1_bpf_present = true;

				// configure all pins as output
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
			else
			{
				fprintf(stderr, "No Charly 25 RX1 BPF found!\n");
			}
		}
		else
		{
			fprintf(stderr, "Charly 25 RX1 BPF - I2C ioctl error!\n");
		}

		if (ioctl(i2c_fd, I2C_SLAVE, C25_RX2_BPF_ADDR) >= 0)
		{
			// set all pins to low
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) >= 0)
			{
				c25_rx2_bpf_present = true;

				// configure all pins as output
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
			else
			{
				fprintf(stderr, "No Charly 25 RX2 BPF found!\n");
			}
		}
		else
		{
			fprintf(stderr, "Charly 25 RX2 BPF - I2C ioctl error!\n");
		}

		if (ioctl(i2c_fd, I2C_SLAVE, C25_EXT_BOARD_ADDR) >= 0)
		{
			// set all pins to low
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) >= 0)
			{
				c25_ext_board_present = true;

				// configure all pins as output
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
			else
			{
				fprintf(stderr, "No Charly 25 EXT board found!\n");
			}
		}
		else
		{
			fprintf(stderr, "Charly 25 EXT board - I2C ioctl error!\n");
		}

		if (ioctl(i2c_fd, I2C_SLAVE, C25_BCD_ENCODER_ADDR) >= 0)
		{
			// set all pins to low
			if (i2c_write_addr_data8(i2c_fd, 0x01, 0x00) >= 0)
			{
				c25_bcd_encoder_present = true;

				// configure all pins as output
				i2c_write_addr_data8(i2c_fd, 0x03, 0x00);
			}
			else
			{
				fprintf(stderr, "No Charly 25 BCD encoder found!\n");
			}
		}
		else
		{
			fprintf(stderr, "Charly 25 BCD encoder - I2C ioctl error!\n");
		}
	}
	else
	{
		fprintf(stderr, "No devices on the I2C bus detected!\n");
	}

	// Detect audio codec board
	// Check if a HAMlab audio codec is present
	if (hamlab_present)
	{
		hamlab_audio_i2c_fd = open(HAMLAB_AUDIO_I2C_DEVICE, O_RDWR);
		if (hamlab_audio_i2c_fd >= 0)
		{
			if (ioctl(hamlab_audio_i2c_fd, I2C_SLAVE_FORCE, AUDIO_CODEC_ADDR) >= 0)
			{
				// reset
				if (i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x1e, 0x00) >= 0)
				{
					i2c_codec = 1;
					// set power down register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x0c, 0x51);
					// reset activate register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x12, 0x00);
					// set volume to -10 dB
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x04, 0x6f);
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x06, 0x6f);
					// set analog audio path register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x08, 0x14);
					// set digital audio path register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x0a, 0x00);
					// set format register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x0e, 0x42);
					// set activate register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x12, 0x01);
					// set power down register
					i2c_write_addr_data8(hamlab_audio_i2c_fd, 0x0c, 0x41);
				}
				else
				{
					fprintf(stderr, "No HAMlab AUDIO CODEC found!\n");
				}
			}
			else
			{
				fprintf(stderr, "HAMlab AUDIO CODEC - I2C ioctl error!\n");
			}
		}
		else
		{
			fprintf(stderr, "HAMlab AUDIO CODEC - I2C open error!\n");
		}
	}
	else if (ioctl(i2c_fd, I2C_SLAVE, AUDIO_CODEC_ADDR) >= 0)
	{
		// reset
		// Check if a Charly 25 stand-alone audio codec is present
		if (i2c_write_addr_data8(i2c_fd, 0x1e, 0x00) >= 0)
		{
			i2c_codec = 1;
			// set power down register
			i2c_write_addr_data8(i2c_fd, 0x0c, 0x51);
			// reset activate register
			i2c_write_addr_data8(i2c_fd, 0x12, 0x00);
			// set volume to -10 dB
			i2c_write_addr_data8(i2c_fd, 0x04, 0x6f);
			i2c_write_addr_data8(i2c_fd, 0x06, 0x6f);
			// set analog audio path register
			i2c_write_addr_data8(i2c_fd, 0x08, 0x14);
			// set digital audio path register
			i2c_write_addr_data8(i2c_fd, 0x0a, 0x00);
			// set format register
			i2c_write_addr_data8(i2c_fd, 0x0e, 0x42);
			// set activate register
			i2c_write_addr_data8(i2c_fd, 0x12, 0x01);
			// set power down register
			i2c_write_addr_data8(i2c_fd, 0x0c, 0x41);
		}
		else
		{
			fprintf(stderr, "No Charly 25 AUDIO CODEC found!\n");
		}
	}
	else
	{
		fprintf(stderr, "Charly 25 AUDIO CODEC - I2C ioctl error!\n");
	}

	// Version and hardware info for debugging only!
	fprintf(stderr, "Version %s with the following hardware is present:\n", SDR_APP_VERSION);

	switch (c25_fpga_model)
	{
		case 0:
			fprintf(stderr, "- FPGA Z10 by default\n");
		break;

		case 1:
			fprintf(stderr, "- FPGA Z10 specified\n");
		break;

		case 2:
			fprintf(stderr, "- FPGA Z20 specified\n");
		break;

		default:
			fprintf(stderr, "- FPGA type unknown (prototype present?)\n");
		break;
	}

	if (charly25_present) fprintf(stderr, "- Charly 25 with ");
	if (hamlab_present) fprintf(stderr, "- HAMlab with ");
	if (c25lc_trx_present) fprintf(stderr, "C25 LC TRX board ID: %u\n", C25_TRX_ID);
	if (c25ab_trx_present) fprintf(stderr, "C25 AB TRX board ID: %u\n", C25_TRX_ID);
	if (c25pp_trx_present) fprintf(stderr, "C25 PP TRX board ID: %u\n", C25_TRX_ID);
	if (c25_rx1_bpf_present) fprintf(stderr, "- Charly 25 RX 1 BPF\n");
	if (c25_rx2_bpf_present) fprintf(stderr, "- Charly 25 RX 2 BPF\n");
	if (c25_ext_board_present) fprintf(stderr, "- Charly 25 EXT board\n");
	if (c25_bcd_encoder_present) fprintf(stderr, "- Charly 25 BCD encoder\n");
	if (i2c_codec) fprintf(stderr, "- AUDIO CODEC\n");
}

uint16_t c25_switch_att_pre_ant(uint8_t frame_3)
{
	static uint16_t c25_att_pre_ant_i2c_data = 0;

	// Wipe bits that might get changed
	// first and second f are LPFs
	uint16_t c25_att_pre_ant_i2c_new_data = c25_i2c_data & 0xffb0;

	// Attenuator
	c25_att_pre_ant_i2c_new_data |= frame_3 & 3;  // C3: Bit 0-1 - Alex Attenuator (00 = 0dB, 01 = 10dB, 10 = 20dB, 11 = 30dB)

	/*
	DG8MG: On Charly 25AB hardware C3 bit 3 is used for the switching of the second preamp
	C3
	0 0 0 0 0 0 0 0
	| | | | | | | |
	| | | | | | + +------------ Alex Attenuator (00 = 0dB, 01 = 10dB, 10 = 20dB, 11 = 30dB)
	| | | | | +---------------- Preamp On/Off (0 = Off, 1 = On)
	| | | | +------------------ LT2208 Dither (0 = Off, 1 = On)  // DG8MG: On Charly 25AB hardware this bit is used for the switching of the second preamp
	| | | + ------------------- LT2208 Random (0= Off, 1 = On)
	| + + --------------------- Alex Rx Antenna (00 = none, 01 = Rx1, 10 = Rx2, 11 = XV)  // DG8MG: On Charly 25AB hardware these bits are used for the switching of the antennas
	+ ------------------------- Alex Rx out (0 = off, 1 = on). Set if Alex Rx Antenna > 0.
	*/

	// Activate preamp one and two as expected from the frontend software (f.e. PowerSDR Charly 25 / HAMlab Edition)
	c25_att_pre_ant_i2c_new_data |= frame_3 & 12;  // C3: Bit 2 - Preamp 1 On/Off (0 = Off, 1 = On), Bit 3 - Preamp 2 On/Off (0 = Off, 1 = On)

	// Activate antenna 1 or 2 as expected from the frontend software
	c25_att_pre_ant_i2c_new_data |= (frame_3 & 32) << 1;  // C3: Bit 5 and 6 - Antenna 1 - 3 (00 = Antenna 1, 01 = Antenna 2, 10 = Antenna 3, 11 = Transverter)

	if (c25_att_pre_ant_i2c_new_data != c25_att_pre_ant_i2c_data)
	{
		c25_att_pre_ant_i2c_data = c25_att_pre_ant_i2c_new_data;
		ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
		i2c_write_addr_data16(i2c_fd, 0x02, c25_att_pre_ant_i2c_data);

#ifdef DEBUG_ATT
		fprintf(stderr, "DEBUG_ATT: ATT, PRE and ANT bitmask in hex: %x\n", c25_att_pre_ant_i2c_data & 0x004f);
#endif
	}

	return c25_att_pre_ant_i2c_data;
}

uint16_t c25pp_switch_tx_lpf(void)
{
	static uint16_t c25pp_tx_lpf_i2c_data = 0;

	// Wipe bits that might get changed
	uint16_t c25pp_tx_lpf_i2c_new_data = c25_i2c_data & 0x004f;

	// Switch LPF depending on TX frequency
	if (C25_6M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_6M_LOW_FREQ)  // 6m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 8;
	}
	else if ((C25_10M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_10M_LOW_FREQ) || (C25_12M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_12M_LOW_FREQ))  // 10/12m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 9;
	}
	else if ((C25_15M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_15M_LOW_FREQ) || (C25_17M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_17M_LOW_FREQ))  // 15/17m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 10;
	}
	else if ((C25_20M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_20M_LOW_FREQ) || (C25_30M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_30M_LOW_FREQ))  // 20/30m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 11;
	}
	else if ((C25_60M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_60M_LOW_FREQ) || (C25_40M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_40M_LOW_FREQ))  // 40/60m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 12;
	}
	else if (C25_80M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_80M_LOW_FREQ)  // 80m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 13;
	}
	else if (C25_160M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_160M_LOW_FREQ)  // 160m LPF
	{
		c25pp_tx_lpf_i2c_new_data |= 1 << 14;
	}

	// Turn PTT and PA only on if a LPF is active
	if ((c25pp_tx_lpf_i2c_new_data & 0x7f00) != 0)
	{
		c25pp_tx_lpf_i2c_new_data |= ((c25_mox & 1) | (tx_mux_data & 1)) << 4;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
		c25pp_tx_lpf_i2c_new_data |= ((c25_mox & 1) | (tx_mux_data & 1)) << 5;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
	}

	if (c25pp_tx_lpf_i2c_new_data != c25pp_tx_lpf_i2c_data)
	{
		c25pp_tx_lpf_i2c_data = c25pp_tx_lpf_i2c_new_data;
		ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
		i2c_write_addr_data16(i2c_fd, 0x02, c25pp_tx_lpf_i2c_data);

#ifdef DEBUG_LPF
		fprintf(stderr, "DEBUG_LPF: LPF bitmask in hex: %x\n", (c25pp_tx_lpf_i2c_new_data & 0x7f00) >> 8);
		fprintf(stderr, "DEBUG_LPF: PA and PTT state: %d\n", (c25pp_tx_lpf_i2c_new_data & 0x0030) >> 4);
		fprintf(stderr, "DEBUG_LPF: c25pp_tx_lpf_i2c_new_data in hex: %x\n", c25pp_tx_lpf_i2c_new_data);
#endif

#ifdef DEBUG_PA
		fprintf(stderr, "DEBUG_PA: gpio_in: %u, c25_mox: %u, tx_mux_data: %u, c25_tx_freq: %u\n", *gpio_in, c25_mox, tx_mux_data, c25_tx_freq);
		fprintf(stderr, "DEBUG_PA: PA and PTT state: %d\n", (c25pp_tx_lpf_i2c_data & 0x0030) >> 4);
#endif
	}

	return c25pp_tx_lpf_i2c_data;
}

uint16_t c25ab_switch_tx_lpf(void)
{
	static uint16_t c25ab_tx_lpf_i2c_data = 0;

	// Wipe bits that might get changed
	uint16_t c25ab_tx_lpf_i2c_new_data = c25_i2c_data & 0x004f;

	// Switch LPF depending on TX frequency
	if (C25_6M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_6M_LOW_FREQ)  // 6m LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 8;
	}
	else if ((C25_10M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_10M_LOW_FREQ) || (C25_12M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_12M_LOW_FREQ))  // 10/12m LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 9;
	}
	else if ((C25_15M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_15M_LOW_FREQ) || (C25_17M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_17M_LOW_FREQ))  // 15/17m LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 10;
	}
	else if (C25_20M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_20M_LOW_FREQ)  // 20 LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 11;
	}
	else if ((C25_30M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_30M_LOW_FREQ) || (C25_40M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_40M_LOW_FREQ))  // 30/40m LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 12;
	}
	else if ((C25_60M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_60M_LOW_FREQ) || (C25_80M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_80M_LOW_FREQ))  // 60/80m LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 13;
	}
	else if (C25_160M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_160M_LOW_FREQ)  // 160m LPF
	{
		c25ab_tx_lpf_i2c_new_data |= 1 << 14;
	}

	// Turn PTT and PA only on if a LPF is active
	if ((c25ab_tx_lpf_i2c_new_data & 0x7f00) != 0)
	{
		c25ab_tx_lpf_i2c_new_data |= ((c25_mox & 1) | (tx_mux_data & 1)) << 4;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
		c25ab_tx_lpf_i2c_new_data |= ((c25_mox & 1) | (tx_mux_data & 1)) << 5;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
	}

	if (c25ab_tx_lpf_i2c_new_data != c25ab_tx_lpf_i2c_data)
	{
		c25ab_tx_lpf_i2c_data = c25ab_tx_lpf_i2c_new_data;
		ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
		i2c_write_addr_data16(i2c_fd, 0x02, c25ab_tx_lpf_i2c_data);

#ifdef DEBUG_LPF
		fprintf(stderr, "DEBUG_LPF: LPF bitmask in hex: %x\n", (c25ab_tx_lpf_i2c_new_data & 0x7f00) >> 8);
		fprintf(stderr, "DEBUG_LPF: PA and PTT state: %d\n", (c25ab_tx_lpf_i2c_new_data & 0x0030) >> 4);
		fprintf(stderr, "DEBUG_LPF: c25ab_tx_lpf_i2c_new_data in hex: %x\n", c25ab_tx_lpf_i2c_new_data);
#endif

#ifdef DEBUG_PA
		fprintf(stderr, "DEBUG_PA: gpio_in: %u, c25_mox: %u, tx_mux_data: %u, c25_tx_freq: %u\n", *gpio_in, c25_mox, tx_mux_data, c25_tx_freq);
		fprintf(stderr, "DEBUG_PA: PA and PTT state: %d\n", (c25ab_tx_lpf_i2c_data & 0x0030) >> 4);
#endif
	}

	return c25ab_tx_lpf_i2c_data;
}

uint16_t c25lc_switch_tx_lpf(void)
{
	static uint16_t c25lc_tx_lpf_i2c_data = 0;

	// Wipe bits that might get changed
	uint16_t c25lc_tx_lpf_i2c_new_data = c25_i2c_data & 0x00ff;

	// Switch LPF depending on TX frequency
	if (C25_10M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_10M_LOW_FREQ)  // 10m LPF
	{
		c25lc_tx_lpf_i2c_new_data |= 1 << 8;
	}
	else if (C25_20M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_20M_LOW_FREQ)  // 20m LPF
	{
		c25lc_tx_lpf_i2c_new_data |= 1 << 9;
	}
	else if (C25_40M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_40M_LOW_FREQ)  // 40m LPF
	{
		c25lc_tx_lpf_i2c_new_data |= 1 << 10;
	}

#ifdef CHARLY25LC_60M_BAND
	else if (C25_60M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_60M_LOW_FREQ)  // 40m LPF on 60m band
	{
		c25lc_tx_lpf_i2c_new_data |= 1 << 10;
	}
#endif

	else if (C25_80M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_80M_LOW_FREQ)  // 80m LPF
	{
		c25lc_tx_lpf_i2c_new_data |= 1 << 11;
	}

	// Turn PTT and PA only on if a LPF is active
	if ((c25lc_tx_lpf_i2c_new_data & 0x0f00) != 0)
	{
		c25lc_tx_lpf_i2c_new_data |= ((c25_mox & 1) | (tx_mux_data & 1)) << 12;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
		c25lc_tx_lpf_i2c_new_data |= ((c25_mox & 1) | (tx_mux_data & 1)) << 13;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
	}

#ifdef DEBUG_LPF
	fprintf(stderr, "DEBUG_LPF: LPF bitmask in hex: %x\n", (c25lc_tx_lpf_i2c_new_data & 0x0f00) >> 8);
	fprintf(stderr, "DEBUG_LPF: PA and PTT state: %d\n", (c25lc_tx_lpf_i2c_new_data & 0x3000) >> 12);
	fprintf(stderr, "DEBUG_LPF: c25lc_tx_lpf_i2c_new_data in hex: %x\n", c25lc_tx_lpf_i2c_new_data);
#endif

	if (c25lc_tx_lpf_i2c_new_data != c25lc_tx_lpf_i2c_data)
	{
		c25lc_tx_lpf_i2c_data = c25lc_tx_lpf_i2c_new_data;
		ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
		i2c_write_addr_data16(i2c_fd, 0x02, c25lc_tx_lpf_i2c_data);

#ifdef DEBUG_PA
		fprintf(stderr, "DEBUG_PA: gpio_in: %u, c25_mox: %u, tx_mux_data: %u, c25_tx_freq: %u\n", *gpio_in, c25_mox, tx_mux_data, c25_tx_freq);
		fprintf(stderr, "DEBUG_PA: PA and PTT state: %d\n", (c25lc_tx_lpf_i2c_data & 0x3000) >> 12);
#endif
	}
	return c25lc_tx_lpf_i2c_data;
}

uint16_t c25_switch_rx_bpf(uint8_t c25_rx_bpf_addr, uint32_t c25_rx_freq)
{
	static uint16_t c25_rx1_bpf_i2c_data = 0;
	static uint16_t c25_rx2_bpf_i2c_data = 0;

	uint16_t c25_rx_bpf_i2c_new_data = 0x0000;

	switch (c25_rx_bpf_addr)
	{
		case C25_RX1_BPF_ADDR:
		{
			// Wipe bits that might get changed in this frame
			c25_rx_bpf_i2c_new_data = c25_rx1_bpf_board_i2c_data & 0x0f00;
			break;
		}

		case C25_RX2_BPF_ADDR:
		{
			// Wipe bits that might get changed in this frame
			c25_rx_bpf_i2c_new_data = c25_rx2_bpf_board_i2c_data & 0x0f00;
			break;
		}

		default:
		{
			return 0;
		}
	}

	// Switch BPF depending on RX frequency
	if (C25_6M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_6M_LOW_FREQ)  // 6m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 12;
	}
	else if (C25_10M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_10M_LOW_FREQ)  // 10m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 13;
	}
	else if (C25_12M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_12M_LOW_FREQ)  // 12m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 14;
	}
	else if (C25_15M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_15M_LOW_FREQ)  // 15m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 15;
	}
	else if (C25_17M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_17M_LOW_FREQ)  // 17m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 0;
	}
	else if (C25_20M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_20M_LOW_FREQ)  // 20m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 1;
	}
	else if (C25_30M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_30M_LOW_FREQ)  // 30m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 2;
	}
	else if (C25_40M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_40M_LOW_FREQ)  // 40m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 3;
	}
	else if (C25_60M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_60M_LOW_FREQ)  // 60m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 4;
	}
	else if (C25_80M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_80M_LOW_FREQ) // 80m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 5;
	}
	else if (C25_160M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_160M_LOW_FREQ) // 160m BPF
	{
		c25_rx_bpf_i2c_new_data |= 1 << 6;
	}
	else  // Default filter for LF or outside of the HAM bands
	{
		c25_rx_bpf_i2c_new_data |= 1 << 7;
	}

	switch (c25_rx_bpf_addr)
	{
		case C25_RX1_BPF_ADDR:
		{
			if (c25_rx_bpf_i2c_new_data != c25_rx1_bpf_i2c_data)
			{
				c25_rx1_bpf_i2c_data = c25_rx_bpf_i2c_new_data;
				ioctl(i2c_fd, I2C_SLAVE, c25_rx_bpf_addr);

#ifdef DEBUG_BPF
				fprintf(stderr, "DEBUG_BPF: RX1 BPF frequency: %d - RX1 BPF bitmask in hex: %04x\n", c25_rx_freq, c25_rx_bpf_i2c_new_data);
#endif

				i2c_write_addr_data16(i2c_fd, 0x02, c25_rx1_bpf_i2c_data);
			}

			return c25_rx1_bpf_i2c_data;
			break;
		}

		case C25_RX2_BPF_ADDR:
		{
			if (c25_rx_bpf_i2c_new_data != c25_rx2_bpf_i2c_data)
			{
				c25_rx2_bpf_i2c_data = c25_rx_bpf_i2c_new_data;
				ioctl(i2c_fd, I2C_SLAVE, c25_rx_bpf_addr);

#ifdef DEBUG_BPF
				fprintf(stderr, "DEBUG_BPF: RX2 BPF frequency: %d - RX2 BPF bitmask in hex: %04x\n", c25_rx_freq, c25_rx_bpf_i2c_new_data);
#endif

				i2c_write_addr_data16(i2c_fd, 0x02, c25_rx2_bpf_i2c_data);
			}

			return c25_rx2_bpf_i2c_data;
			break;
		}

		default:
		{
			return 0;
		}
	}
}

uint16_t c25_switch_bpf_tx_relay(uint8_t c25_rx_bpf_addr, bool state)
{
	static uint16_t c25_rx1_bpf_i2c_data = 0;
	static uint16_t c25_rx2_bpf_i2c_data = 0;

	uint16_t c25_rx_bpf_i2c_new_data = 0x0000;

	switch (c25_rx_bpf_addr)
	{
		case C25_RX1_BPF_ADDR:
		{
			// Wipe bit that might get changed in this frame
			c25_rx_bpf_i2c_new_data = c25_rx1_bpf_board_i2c_data & 0xf7ff;
			break;
		}

		case C25_RX2_BPF_ADDR:
		{
			// Wipe bit that might get changed in this frame
			c25_rx_bpf_i2c_new_data = c25_rx2_bpf_board_i2c_data & 0xf7ff;
			break;
		}

		default:
		{
			return 0;
		}
	}

	// Switch BPF RX/TX relay depending on the requested state
	if (state)
	{
		c25_rx_bpf_i2c_new_data |= 1 << 11;  // switch to TX path
	}
	else
	{
		c25_rx_bpf_i2c_new_data |= 0 << 11;  // switch to RX path
	}

	switch (c25_rx_bpf_addr)
	{
		case C25_RX1_BPF_ADDR:
		{
			if (c25_rx_bpf_i2c_new_data != c25_rx1_bpf_i2c_data)
			{
				c25_rx1_bpf_i2c_data = c25_rx_bpf_i2c_new_data;
				ioctl(i2c_fd, I2C_SLAVE, c25_rx_bpf_addr);

#ifdef DEBUG_BPF
				fprintf(stderr, "DEBUG_BPF: RX1 BPF RX/TX relay state: %d - RX1 BPF bitmask in hex: %04x\n", state, c25_rx_bpf_i2c_new_data);
#endif

				i2c_write_addr_data16(i2c_fd, 0x02, c25_rx1_bpf_i2c_data);
			}

			return c25_rx1_bpf_i2c_data;
			break;
		}

		case C25_RX2_BPF_ADDR:
		{
			if (c25_rx_bpf_i2c_new_data != c25_rx2_bpf_i2c_data)
			{
				c25_rx2_bpf_i2c_data = c25_rx_bpf_i2c_new_data;
				ioctl(i2c_fd, I2C_SLAVE, c25_rx_bpf_addr);

#ifdef DEBUG_BPF
				fprintf(stderr, "DEBUG_BPF: RX2 BPF RX/TX relay state: %d - RX2 BPF bitmask in hex: %04x\n", state, c25_rx_bpf_i2c_new_data);
#endif

				i2c_write_addr_data16(i2c_fd, 0x02, c25_rx2_bpf_i2c_data);
			}

			return c25_rx2_bpf_i2c_data;
			break;
		}

		default:
		{
			return 0;
		}
	}
}

uint16_t c25_switch_ext_board(uint16_t frame_1_2)
{
	/*
	C1
	0 0 0 0 0 0 0 0
	| | | | | | | |
	| | | | | | + +------------ Bit 0-1 - RX2 12dB attenuator and RX2 24db attenuator
	| | | | + +---------------- Bit 2-3 - RX2 18dB preamp 1 and 2
	| | | + ------------------- Bit 4 - unused
	| | + --------------------- Bit 5 - RX2 Predistorsion switch
	| + ----------------------- Bit 6 - VHF/UHF switch RX1
	+ ------------------------- Bit 7 - VHF/UHF switch TX

	C2
	0 0 0 0 0 0 0 0
	| | | | | | | |
	| | | + + + + +------------ Bit 0-4 - Step attenuator 0-31dB
	| | + --------------------- Bit 5 - RP external usage OFF (SDR mode on)
	| + ----------------------- Bit 6 - RP TX channel 2 envelope modulation
	+ ------------------------- Bit 7 - unused
	*/

	if (frame_1_2 != c25_ext_board_i2c_data)
	{
		c25_ext_board_i2c_data = frame_1_2;
		ioctl(i2c_fd, I2C_SLAVE, C25_EXT_BOARD_ADDR);
		i2c_write_addr_data16(i2c_fd, 0x02, c25_ext_board_i2c_data);

#ifdef DEBUG_EXT
		fprintf(stderr, "DEBUG_EXT: c25_ext_board_i2c_data: 0x%04x\n", c25_ext_board_i2c_data);
#endif
	}

	return c25_ext_board_i2c_data;
}

uint8_t c25_switch_bcd_encoder(void)
{
/*
	0 0 0 0 0 0 0 0
	| | | | | | | |
	| | | | + + + +------------ Bit 0-3 - BCD frequency encoder
	+ + + +-------------------- Bit 4-7 unused

	Band  = Value
	160m = $01 %00000001
	 80m = $02 %00000010
	 60m = $03 %00000011
	 40m = $03 %00000011
	 30m = $04 %00000100
	 20m = $05 %00000101
	 17m = $06 %00000110
	 15m = $07 %00000111
	 12m = $08 %00001000
	 10m = $09 %00001001
	  6m = $0A %00001010
*/

	uint16_t c25_new_bcd_encoder_i2c_data = 0x0000;

	// Switch BCD encoder in dependence on the TX frequency based on the YAESU standard
	if (C25_6M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_6M_LOW_FREQ)  // 6m
	{
		c25_new_bcd_encoder_i2c_data = 0b00001010;
	}
	else if (C25_10M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_10M_LOW_FREQ)  // 10m
	{
		c25_new_bcd_encoder_i2c_data = 0b00001001;
	}
	else if (C25_12M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_12M_LOW_FREQ)  // 12m
	{
		c25_new_bcd_encoder_i2c_data = 0b00001000;
	}
	else if (C25_15M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_15M_LOW_FREQ)  // 15m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000111;
	}
	else if (C25_17M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_17M_LOW_FREQ)  // 17m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000110;
	}
	else if (C25_20M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_20M_LOW_FREQ)  // 20m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000101;
	}
	else if (C25_30M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_30M_LOW_FREQ)  // 30m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000100;
	}
	else if (C25_40M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_40M_LOW_FREQ)  // 40m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000011;
	}
	else if (C25_60M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_60M_LOW_FREQ)  // 60m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000011;
	}
	else if (C25_80M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_80M_LOW_FREQ)  // 80m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000010;
	}
	else if (C25_160M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_160M_LOW_FREQ)  // 160m
	{
		c25_new_bcd_encoder_i2c_data = 0b00000001;
	}
	else  // outside a ham radio band
	{
		c25_new_bcd_encoder_i2c_data = 0b00000000;
	}

	if (c25_new_bcd_encoder_i2c_data != c25_bcd_encoder_i2c_data)
	{
		c25_bcd_encoder_i2c_data = c25_new_bcd_encoder_i2c_data;
		ioctl(i2c_fd, I2C_SLAVE, C25_BCD_ENCODER_ADDR);
		i2c_write_addr_data8(i2c_fd, 0x03, 0x00);
		i2c_write_addr_data8(i2c_fd, 0x01, c25_bcd_encoder_i2c_data);

#ifdef DEBUG_BCD
		fprintf(stderr, "DEBUG_BCD: c25_bcd_encoder_i2c_data: 0x%02x\n", c25_bcd_encoder_i2c_data);
#endif
	}

	return c25_bcd_encoder_i2c_data;
}
#endif

int main(int argc, char *argv[])
{
	int fd, i, j, size;
	struct sched_param param;
	pthread_attr_t attr;
	pthread_t thread;

#ifdef CHARLY25
	pthread_t c25_adc_thread;
#endif

	volatile void *cfg, *sts;
	volatile int32_t *tx_ramp, *dac_ramp;
	volatile uint16_t *tx_size, *dac_size;
	volatile int16_t *ps_level;
	volatile uint8_t *rx_sel, *tx_sel;
	float scale, ramp[1024], a[4] = { 0.35875, 0.48829, 0.14128, 0.01168 };

#ifdef CHARLY25
	// Changed last byte from 1 to 100 to identify as Red Pitaya / STEMlab with a Xilinx Zynq 7010 FPGA instead of Hermes device in the frontend software
	uint8_t reply[11] = { 0xef, 0xfe, 2, 0, 0, 0, 0, 0, 0, 32, 100 };
#else
	uint8_t reply[11] = { 0xef, 0xfe, 2, 0, 0, 0, 0, 0, 0, 32, 1 };
#endif

	uint8_t id[4] = { 0xef, 0xfe, 1, 6 };
	uint32_t code;

#ifndef CHARLY25
	struct termios tty;
#endif

	struct ifreq hwaddr;
	struct sockaddr_in addr_ep2, addr_from[10];
	uint8_t buffer[8][1032];
	struct iovec iovec[8][1];
	struct mmsghdr datagram[8];
	struct timeval tv;
	struct timespec ts;
	int yes = 1;
	char *end;
	uint8_t chan = 0;
	long number;

#ifdef DEBUG_SEQ
	uint32_t last_seqnum = 0xffffffff, seqnum;  // sequence number of received packet
#endif

#ifdef CHARLY25

#ifdef CHARLY25_TCP
	int bytes_read, bytes_left;
	uint32_t *code0 = (uint32_t *) buffer[0];  // fast access to code of first buffer
#endif

	c25_command_line_trx_board_id = -1;

	if (argc > 6)
	{
		c25_fpga_model = (strncasecmp(argv[6], "z20\0", 3) == 0) ? 2 : 1;

		if (c25_fpga_model > 1)
		{
			// Change last byte from 100 to 101 to identify as STEMlab with a Xilinx Zynq 7020 FPGA instead of Hermes device in the frontend software
			reply[10] = 101;
		}

		if (argc > 7)
		{
			c25_command_line_trx_board_id = strtol(argv[7], NULL, 10);
		}

		argc = 6;
	}

	// Be benign to experimental users:
	// If this program is invoked with less than 5 arguments, assume 1 2 2 1 2
	if (argc < 6)
	{
		chan = 0x16;
	}
	else
#endif
	{
		for(i = 0; i < 5; ++i)
		{
			errno = 0;
			number = (argc == 6) ? strtol(argv[i + 1], &end, 10) : -1;
			if(errno != 0 || end == argv[i + 1] || number < 1 || number > 2)
			{
				printf("Usage: sdr-transceiver-hpsdr 1|2 1|2 1|2 1|2 1|2\n");
				return EXIT_FAILURE;
			}
			chan |= (number - 1) << i;
		}
	}

	if ((fd = open("/dev/mem", O_RDWR)) < 0)
	{
		perror("open");
		return EXIT_FAILURE;
	}

#ifdef CHARLY25
	// Detect the present Charly 25 and HAMlab hardware
	c25_detect_hardware();
#endif

#ifndef CHARLY25
	if ((uart_fd = open("/dev/ttyPS1", O_RDWR | O_NOCTTY | O_NDELAY)) >= 0)
	{
		tcgetattr(uart_fd, &tty);
		cfsetspeed(&tty, (speed_t)B1200);
		cfmakeraw(&tty);
		tty.c_cflag &= ~(CSTOPB | CRTSCTS);
		tty.c_cflag |= CLOCAL | CREAD;
		tcflush(uart_fd, TCIFLUSH);
		tcsetattr(uart_fd, TCSANOW, &tty);
	}

	if ((i2c_fd = open("/dev/i2c-0", O_RDWR)) >= 0)
	{
		if (ioctl(i2c_fd, I2C_SLAVE_FORCE, ADDR_PENE) >= 0)
		{
			/* set all pins to low */
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
			{
				i2c_pene = 1;
				/* configure all pins as output */
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
		}
		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_ALEX) >= 0)
		{
			/* set all pins to low */
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
			{
				i2c_alex = 1;
				/* configure all pins as output */
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
		}
		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_LEVEL) >= 0)
		{
			/* set all pins to low */
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
			{
				i2c_level = 1;
				/* configure all pins as output */
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
		}
		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_MISC) >= 0)
		{
			/* set all pins to low */
			if (i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
			{
				i2c_misc = 1;
				/* configure all pins as output */
				i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
			}
		}
		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_DRIVE) >= 0)
		{
			/* set both potentiometers to 0 */
			if (i2c_write_addr_data16(i2c_fd, 0xa9, 0x0000) > 0)
			{
				i2c_drive = 1;
			}
		}
		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_DAC0) >= 0)
		{
			if (i2c_write_data16(i2c_fd, i2c_dac0_data) > 0)
			{
				i2c_dac0 = 1;
			}
		}
		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_DAC1) >= 0)
		{
			if (i2c_write_data16(i2c_fd, i2c_dac1_data) > 0)
			{
				i2c_dac1 = 1;
			}
		}
		if(ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO) >= 0)
		{
			if(i2c_write_addr_data16(i2c_fd, 0x1, i2c_ard_frx1_data) > 0)
			{
				i2c_arduino = 1;
			}
		}

		if (ioctl(i2c_fd, I2C_SLAVE, ADDR_CODEC) >= 0)
		{
			/* reset */
			if (i2c_write_addr_data8(i2c_fd, 0x1e, 0x00) > 0)
			{
				i2c_codec = 1;
				/* set power down register */
				i2c_write_addr_data8(i2c_fd, 0x0c, 0x51);
				/* reset activate register */
				i2c_write_addr_data8(i2c_fd, 0x12, 0x00);
				/* set volume to -10 dB */
				i2c_write_addr_data8(i2c_fd, 0x04, 0x6f);
				i2c_write_addr_data8(i2c_fd, 0x06, 0x6f);
				/* set analog audio path register */
				i2c_write_addr_data8(i2c_fd, 0x08, 0x14);
				/* set digital audio path register */
				i2c_write_addr_data8(i2c_fd, 0x0a, 0x00);
				/* set format register */
				i2c_write_addr_data8(i2c_fd, 0x0e, 0x42);
				/* set activate register */
				i2c_write_addr_data8(i2c_fd, 0x12, 0x01);
				/* set power down register */
				i2c_write_addr_data8(i2c_fd, 0x0c, 0x41);
			}
		}
	}
#endif

	sts = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40000000);
	cfg = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40001000);

#ifndef CHARLY25
	alex = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40002000);
#endif

	tx_mux = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40003000);
	tx_ramp = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40004000);
	dac_ramp = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40005000);
	dac_data = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40006000);
	adc_data = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40007000);
	tx_data = mmap(NULL, 4*sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd,  0x4000c000);
	rx_data = mmap(NULL, 8 * sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40010000);
	xadc = mmap(NULL, 16 * sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40020000);

	rx_rst = ((uint8_t *)(cfg + 0));
	lo_rst = ((uint8_t *)(cfg + 1));
	tx_rst = ((uint8_t *)(cfg + 2));
	gpio_out = ((uint8_t *)(cfg + 3));

	rx_rate = ((uint16_t *)(cfg + 4));

	rx_sel = ((uint8_t *)(cfg + 6));

	rx_freq[0] = ((uint32_t *)(cfg + 8));
	rx_freq[1] = ((uint32_t *)(cfg + 12));

	tx_freq = ((uint32_t *)(cfg + 16));
	tx_size = ((uint16_t *)(cfg + 20));
	tx_level = ((int16_t *)(cfg + 22));
	ps_level = ((int16_t *)(cfg + 24));

	tx_sel = ((uint8_t *)(cfg + 26));

	dac_freq = ((uint32_t *)(cfg + 28));
	dac_size = ((uint16_t *)(cfg + 32));
	dac_level = ((int16_t *)(cfg + 34));

	rx_cntr = ((uint16_t *)(sts + 12));
	tx_cntr = ((uint16_t *)(sts + 14));
	dac_cntr = ((uint16_t *)(sts + 16));
	adc_cntr = ((uint16_t *)(sts + 18));
	gpio_in = ((uint8_t *)(sts + 20));

	/* set rx and tx selectors */
	*rx_sel = chan & 7;
	*tx_sel = (chan >> 3) & 3;

	/* set all GPIO pins to low */
	*gpio_out = 0;

#ifdef CHARLY25
	if (c25_fpga_model > 1)
	{
		/* set default rx phase increment */
		*rx_freq[0] = (uint32_t)floor(600000 / 122.88e6 * (1 << 30) + 0.5);
		*rx_freq[1] = (uint32_t)floor(600000 / 122.88e6 * (1 << 30) + 0.5);

		/* set default rx sample rate */
		*rx_rate = 1280;

		/* set default tx phase increment */
		*tx_freq = (uint32_t)floor(600000 / 122.88e6 * (1 << 30) + 0.5);
	}
	else
#endif
	{
		/* set default rx phase increment */
		*rx_freq[0] = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);
		*rx_freq[1] = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);

		/* set default rx sample rate */
		*rx_rate = 1000;

		/* set default tx phase increment */
		*tx_freq = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);
	}

	/* set tx ramp */
	size = 1001;
	ramp[0] = 0.0;
	for (i = 1; i <= size; ++i)
	{
		ramp[i] = ramp[i - 1] + a[0] - a[1] * cos(2.0 * M_PI * i / size) + a[2] * cos(4.0 * M_PI * i / size) - a[3] * cos(6.0 * M_PI * i / size);
	}
	scale = 6.1e6 / ramp[size];
	for (i = 0; i <= size; ++i)
	{
		tx_ramp[i] = (int32_t)floor(ramp[i] * scale + 0.5);
	}
	*tx_size = size;

#ifdef CHARLY25
	if (c25_fpga_model > 1)
	{
		/* set default tx level */
		*tx_level = 32766;
		/* set ps level */
		*ps_level = 18716;
	}
	else
#endif
	{
		/* set default tx level */
		*tx_level = 32110;
		/* set ps level */
		*ps_level = 23080;
	}

	/* set default tx mux channel */
	tx_mux[16] = 0;
	tx_mux[0] = 2;

	/* reset tx and codec DAC fifo */
	*tx_rst |= 3;
	*tx_rst &= ~3;

	/* reset tx lo */
	*lo_rst &= ~4;
	*lo_rst |= 4;

	if (i2c_codec)
	{
		/* reset codec ADC fifo */
		*rx_rst |= 2;
		*rx_rst &= ~2;
		/* enable I2S interface */
		*rx_rst &= ~4;

		/* set default dac phase increment */
		*dac_freq = (uint32_t)floor(600 / 48.0e3 * (1 << 30) + 0.5);

		/* set dac ramp */
		size = 481;
		ramp[0] = 0.0;
		for (i = 1; i <= size; ++i)
		{
			ramp[i] = ramp[i - 1] + a[0] - a[1] * cos(2.0 * M_PI * i / size) + a[2] * cos(4.0 * M_PI * i / size) - a[3] * cos(6.0 * M_PI * i / size);
		}
		scale = 3.2e4 / ramp[size];
		for (i = 0; i <= size; ++i)
		{
			dac_ramp[i] = (int32_t)floor(ramp[i] * scale + 0.5);
		}
		*dac_size = size;

		/* set default dac level */
		*dac_level = 3200;
	}
	else
	{
		/* enable ALEX interface */
		*rx_rst |= 4;
	}

#ifndef CHARLY25
	calc_log_lookup();
#endif

	if ((sock_ep2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	strncpy(hwaddr.ifr_name, "eth0", IFNAMSIZ);
	ioctl(sock_ep2, SIOCGIFHWADDR, &hwaddr);
	for (i = 0; i < 6; ++i) reply[i + 3] = hwaddr.ifr_addr.sa_data[i];

	setsockopt(sock_ep2, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes));

	tv.tv_sec = 0;
	tv.tv_usec = 1000;
	setsockopt(sock_ep2, SOL_SOCKET, SO_RCVTIMEO, (void *)&tv, sizeof(tv));

	memset(&addr_ep2, 0, sizeof(addr_ep2));
	addr_ep2.sin_family = AF_INET;
	addr_ep2.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_ep2.sin_port = htons(1024);

	if (bind(sock_ep2, (struct sockaddr *)&addr_ep2, sizeof(addr_ep2)) < 0)
	{
		perror("bind");
		return EXIT_FAILURE;
	}

#ifdef CHARLY25_TCP
	if ((sock_TCP_Server = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
	{
		perror("socket tcp");
		return EXIT_FAILURE;
	}

#ifdef DEBUG_TCP
	fprintf(stderr, "DEBUG_TCP: RP <--> PC: sock_TCP_Server: %d\n", sock_TCP_Server);
#endif

	setsockopt(sock_TCP_Server, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes));

	int tcpmaxseg = 1032;
	setsockopt(sock_TCP_Server, IPPROTO_TCP, TCP_MAXSEG, (const char *)&tcpmaxseg, sizeof(int));

	int sndbufsize = 65535;
	int rcvbufsize = 65535;
	setsockopt(sock_TCP_Server, SOL_SOCKET, SO_SNDBUF, (const char *)&sndbufsize, sizeof(int));
	setsockopt(sock_TCP_Server, SOL_SOCKET, SO_RCVBUF, (const char *)&rcvbufsize, sizeof(int));

	if (bind(sock_TCP_Server, (struct sockaddr *)&addr_ep2, sizeof(addr_ep2)) < 0)
	{
		perror("bind tcp");
		return EXIT_FAILURE;
	}

	listen(sock_TCP_Server, 1024);

#ifdef DEBUG_TCP
	fprintf(stderr, "DEBUG_TCP: RP <--> PC: Listening for TCP client connection request\n");
#endif
#endif

	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	param.sched_priority = 99;
	pthread_attr_setschedparam(&attr, &param);
	if (pthread_create(&thread, &attr, handler_keyer, NULL) < 0)
	{
		perror("pthread_create");
		return EXIT_FAILURE;
	}
	pthread_detach(thread);

#ifdef CHARLY25
	if (c25_ext_board_present)
	{
		if (pthread_create(&c25_adc_thread, NULL, c25_adc_handler, NULL) < 0)
		{
			perror("c25_adc_thread pthread_create");
			return EXIT_FAILURE;
		}
		pthread_detach(c25_adc_thread);
	}
#endif

	while (1)
	{
		memset(iovec, 0, sizeof(iovec));
		memset(datagram, 0, sizeof(datagram));

		for (i = 0; i < 8; ++i)
		{
			memcpy(buffer[i], id, 4);
			iovec[i][0].iov_base = buffer[i];
			iovec[i][0].iov_len = 1032;
			datagram[i].msg_hdr.msg_iov = iovec[i];
			datagram[i].msg_hdr.msg_iovlen = 1;
			datagram[i].msg_hdr.msg_name = &addr_from[i];
			datagram[i].msg_hdr.msg_namelen = sizeof(addr_from[i]);
		}

		ts.tv_sec = 0;
		ts.tv_nsec = 1000000;

#ifdef CHARLY25_TCP
		if (sock_TCP_Client < 0)
		{
			if((sock_TCP_Client = accept(sock_TCP_Server, NULL, NULL)) > -1)
			{
#ifdef DEBUG_TCP
				fprintf(stderr, "DEBUG_TCP: RP <--> PC: sock_TCP_Client: %d connected to sock_TCP_Server: %d\n", sock_TCP_Client, sock_TCP_Server);
#endif
			}
		}

		if (sock_TCP_Client > -1)
		{
			// Using recvmmsg with a time-out should be used for a byte-stream protocol like TCP
			// (Each "packet" in the datagram may be incomplete). This is especially true if the
			// socket has a receive time-out, but this problem also occurs if the is no such
			// receive time-out.
			// Therefore we read a complete packet here (1032 bytes). Our TCP-extension to the
			// HPSDR protocol ensures that only 1032-byte packets may arrive here.
			bytes_read = 0;
			bytes_left = 1032;
			while (bytes_left > 0)
			{
				size = recvfrom(sock_TCP_Client, buffer[0] + bytes_read, (size_t)bytes_left, 0, NULL, 0);
				if (size < 0 && errno == EAGAIN) continue;
				if (size < 0) break;
				bytes_read += size;
				bytes_left -= size;

#ifdef DEBUG_TCP
				if (bytes_read != 1032)
				{
					fprintf(stderr,"DEBUG_TCP: bytes_read: %d, bytes_left: %d\n", bytes_read, bytes_left);
				}
#endif
			}

			if (size >= 0)
			{
				// 1032 bytes have successfully been read by TCP.
				// Let the downstream code know that there is a single packet, and its size
				size = 1;
				datagram[0].msg_len = bytes_read;

				// In the case of a METIS-discovery packet, change the size to 63
				if (*code0 == 0x0002feef)
				{
					datagram[0].msg_len = 63;
				}

				// In the case of a METIS-stop packet, change the size to 64
				if (*code0 == 0x0004feef)
				{
					datagram[0].msg_len = 64;
				}

				// In the case of a METIS-start TCP packet, change the size to 64
				if (*code0 == 0x1104feef)
				{
					datagram[0].msg_len = 64;
				}
			}
		}
		else
#endif
		{
			size = recvmmsg(sock_ep2, datagram, 8, 0, &ts);

#ifdef DEBUG_UDP
			if (size > 0)
			{
				fprintf(stderr,"DEBUG_UDP: size: %d\n", size);
			}
#endif
		}

		if (size < 0 && errno != EAGAIN)
		{
			perror("recvfrom");
			return EXIT_FAILURE;
		}

		for (i = 0; i < size; ++i)
		{
			memcpy(&code, buffer[i], 4);

			switch (code)
			{
				// PC to Red Pitaya transmission via process_ep2
				case 0x0201feef:

#ifdef DEBUG_PROT
				fprintf(stderr, "DEBUG_PROT: PC -> RP: data transmission via process_ep2 / code: 0x%08x\n", code);
#endif

				// processing an invalid packet is too dangerous -- skip it!
				if (datagram[i].msg_len != 1032)
				{
#ifdef DEBUG_PROT
					fprintf(stderr,"DEBUG_PROT: RvcMsg %d(%d) Code=0x%08x Len=%d\n", i, (int)(size-1), code, (int)datagram[i].msg_len);
#endif
					break;
				}

#ifdef DEBUG_SEQ
				// sequence number check
				seqnum = ((buffer[i][4]&0xFF)<<24) + ((buffer[i][5]&0xFF)<<16) + ((buffer[i][6]&0xFF)<<8) + (buffer[i][7]&0xFF);

				if (seqnum != last_seqnum + 1)
				{
					fprintf(stderr,"DEBUG_SEQ: SEQ ERROR: last %ld, recvd %ld\n", (long)last_seqnum, (long)seqnum);
				}

				last_seqnum = seqnum;
#endif

				if (!tx_mux_data)
				{
					while (*tx_cntr > 3844) usleep(1000);
					if (*tx_cntr == 0) for (j = 0; j < 2520; ++j) *tx_data = 0;
					if ((*gpio_out & 1) | (*gpio_in & 1))
					{
						for(j = 0; j < 504; j += 8)
						{
							*tx_data = tx_eer_data ? *(uint32_t *)(buffer[i] + 16 + j) : 0;
							*tx_data = *(uint32_t *)(buffer[i] + 20 + j);
						}
						for(j = 0; j < 504; j += 8)
						{
							*tx_data = tx_eer_data ? *(uint32_t *)(buffer[i] + 528 + j) : 0;
							*tx_data = *(uint32_t *)(buffer[i] + 532 + j);
						}
					}
					else
					{
						for (j = 0; j < 126; ++j) *tx_data = 0;
					}
				}

				if (i2c_codec)
				{
					while (*dac_cntr > 898) usleep(1000);
					if (*dac_cntr == 0) for (j = 0; j < 504; ++j) *dac_data = 0;
					for (j = 0; j < 504; j += 8) *dac_data = *(uint32_t *)(buffer[i] + 16 + j);
					for (j = 0; j < 504; j += 8) *dac_data = *(uint32_t *)(buffer[i] + 528 + j);
				}

				process_ep2(buffer[i] + 11);
				process_ep2(buffer[i] + 523);
				break;

				// respond to an incoming Metis detection request
				case 0x0002feef:

#ifdef DEBUG_PROT
				fprintf(stderr, "DEBUG_PROT: RP -> PC: respond to an incoming Metis detection request / code: 0x%08x\n", code);
#endif

				// processing an invalid packet is too dangerous -- skip it!
				if (datagram[i].msg_len != 63)
				{
#ifdef DEBUG_PROT
					fprintf(stderr,"DEBUG_PROT: RvcMsg %d(%d) Code=0x%08x Len=%d\n", i, (int)(size-1), code, (int)datagram[i].msg_len);
#endif
					break;
				}

				reply[2] = 2 + active_thread;
				memset(buffer[i], 0, 60);
				memcpy(buffer[i], reply, 11);

#ifdef CHARLY25_TCP
				if (sock_TCP_Client > -1)
				{
					if (send(sock_TCP_Client, buffer[i], 60, 0) < 0)
					{
#ifdef DEBUG_TCP
						fprintf(stderr, "DEBUG_TCP: RP -> PC: TCP send error occurred when responding to an incoming Metis detection request!\n");
#endif
					}

					// close the TCP socket which was only used for the detection
					close(sock_TCP_Client);
					sock_TCP_Client = -1;
				}
				else
#endif
				{
					sendto(sock_ep2, buffer[i], 60, 0, (struct sockaddr *)&addr_from[i], sizeof(addr_from[i]));
				}

				break;

				// stop the Red Pitaya to PC transmission via handler_ep6
				case 0x0004feef:

#ifdef DEBUG_PROT
				fprintf(stderr, "DEBUG_PROT: RP -> PC: stop the transmission via handler_ep6 / code: 0x%08x\n", code);
#endif

				// processing an invalid packet is too dangerous -- skip it!
				if (datagram[i].msg_len != 64)
				{
#ifdef DEBUG_PROT
					fprintf(stderr,"DEBUG_PROT: RvcMsg %d(%d) Code=0x%08x Len=%d\n", i, (int)(size-1), code, (int)datagram[i].msg_len);
#endif
					break;
				}

				enable_thread = 0;
				while (active_thread) usleep(1000);

#ifdef CHARLY25_TCP
				if (sock_TCP_Client > -1)
				{
					close(sock_TCP_Client);
					sock_TCP_Client = -1;
				}
#endif
				break;

				// start the Red Pitaya to PC transmission via handler_ep6
#ifdef CHARLY25_TCP
				case 0x1104feef:


#ifdef DEBUG_TCP
				fprintf(stderr, "DEBUG_TCP: PC -> RP: TCP METIS-start message received / code: 0x%08x\n", code);
#endif

				/* FALLTHROUGH */
#endif

				case 0x0104feef:
				case 0x0204feef:
				case 0x0304feef:

#ifdef DEBUG_PROT
				fprintf(stderr, "DEBUG_PROT: RP <--> PC: start the handler_ep6 thread / code: 0x%08x\n", code);
#endif

				// processing an invalid packet is too dangerous -- skip it!
				if (datagram[i].msg_len != 64)
				{
#ifdef DEBUG_PROT
					fprintf(stderr,"DEBUG_PROT: RvcMsg %d(%d) Code=0x%08x Len=%d\n", i, (int)(size-1), code, (int)datagram[i].msg_len);
#endif
					break;
				}

				enable_thread = 0;
				while (active_thread) usleep(1000);
				memset(&addr_ep6, 0, sizeof(addr_ep6));
				addr_ep6.sin_family = AF_INET;
				addr_ep6.sin_addr.s_addr = addr_from[i].sin_addr.s_addr;
				addr_ep6.sin_port = addr_from[i].sin_port;

				enable_thread = 1;
				active_thread = 1;
				rx_sync_data = 0;
				/* reset rx los */
				*lo_rst &= ~3;
				*lo_rst |= 3;
				if (pthread_create(&thread, NULL, handler_ep6, NULL) < 0)
				{
					perror("pthread_create");
					return EXIT_FAILURE;
				}
				pthread_detach(thread);
				break;

#ifdef DEBUG_PROT
				default:
				// Possibly a discovery packet of the New protocol, otherwise a severe error
				if (datagram[i].msg_len == 60 && code == 0 && buffer[i][4] == 0x02)
				{
					fprintf(stderr,"DEBUG_PROT: PC -> RP: NewProtocol discovery packet received (no response)\n");
				}
				else
				{
					fprintf(stderr,"DEBUG_PROT: PC -> RP: invalid code: 0x%08x (Len=%d)\n", code, datagram[i].msg_len);
				}
				break;
#endif
			}
		}
	}

	close(sock_ep2);

#ifdef CHARLY25_TCP
	if (sock_TCP_Client > -1)
	{
		close(sock_TCP_Client);
	}

	if (sock_TCP_Server > -1)
	{
		close(sock_TCP_Server);
	}
#endif

	return EXIT_SUCCESS;
}

void process_ep2(uint8_t *frame)
{
#ifdef CHARLY25
	uint32_t c25_rx1_freq, c25_rx2_freq;
#endif

	uint16_t data;
	uint32_t freq;

#ifndef CHARLY25
	uint32_t data32;
	uint8_t preamp, att;
	uint8_t data8;
#endif

uint8_t ptt, boost;

	switch (frame[0])
	{
	case 0:
	case 1:
		receivers = ((frame[4] >> 3) & 7) + 1;  // C4: Bit 3-5 - Number of Receivers (000 = 1, 111 = 8)

#ifdef DEBUG_PROT
		fprintf(stderr, "DEBUG_PROT: PC -> RP: number of receivers: %u\n", receivers);
#endif

		data = (frame[4] >> 7) & 1;
		if (rx_sync_data != data)
		{
			rx_sync_data = data;
			if (rx_sync_data)
			{
				*rx_freq[1] = *rx_freq[0];
				/* reset rx los */
				*lo_rst &= ~3;
				*lo_rst |= 3;
			}
		}
		tx_eer_data = frame[2] & 1;

#ifndef CHARLY25
		/* set output pins */
		ptt = frame[0] & 0x01;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
		att = frame[3] & 0x03;  // C3: Bit 0-1 - Alex Attenuator (00 = 0dB, 01 = 10dB, 10 = 20dB, 11 = 30dB)
		preamp = ptt | (*gpio_in & 1) ? 0 : (frame[3] & 0x04) >> 2 | (rx_att_data == 0);
		*gpio_out = (frame[2] & 0x1e) << 3 | att << 2 | preamp << 1 | ptt;
#else
		// set output pins */
		ptt = frame[0] & 0x01;
		*gpio_out = (*gpio_out & 0xfe) | ptt;
#endif

		/* set rx sample rate */
		rate = frame[1] & 3;  // C1: Bit 0-1 - Speed (00 = 48kHz, 01 = 96kHz, 10 = 192kHz, 11 = 384kHz)

#ifdef CHARLY25
		if (c25_fpga_model > 1)
		{
			switch (frame[1] & 3)
			{
			case 0:
				*rx_rate = 1280;
				break;
			case 1:
				*rx_rate = 640;
				break;
			case 2:
				*rx_rate = 320;
				break;
			case 3:
				*rx_rate = 160;
				break;
			}
		}
		else
#endif
		{
			switch (frame[1] & 3)
			{
			case 0:
				*rx_rate = 1000;
				break;
			case 1:
				*rx_rate = 500;
				break;
			case 2:
				*rx_rate = 250;
				break;
			case 3:
				*rx_rate = 125;
				break;
			}
		}

#ifdef CHARLY25
		// switch the attenuators, preamps and antenna on the Charly 25 board
		c25_i2c_data = c25_switch_att_pre_ant(frame[3]);
		break;
#endif

#ifndef CHARLY25
		data = (frame[4] & 0x03) << 8 | (frame[3] & 0xe0) | (frame[3] & 0x03) << 1 | (frame[0] & 0x01);
		if (alex_data_0 != data)
		{
			alex_data_0 = data;
			alex_write();
		}

		/* configure PENELOPE */
		if (i2c_pene)
		{
			data = (frame[3] & 0x1c) << 11 | (frame[4] & 0x03) << 11 | (frame[3] & 0x60) << 4 | (frame[3] & 0x03) << 7 | frame[2] >> 1;
			if (i2c_pene_data != data)
			{
				i2c_pene_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_PENE);
				i2c_write_addr_data16(i2c_fd, 0x02, data);
			}
		}

		if (i2c_dac0)
		{
			data = rx_att_data + 10 * att;
			data = 4065 - (uint32_t)data * 4095 / 61;
			if (i2c_dac0_data != data)
			{
				i2c_dac0_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_DAC0);
				i2c_write_data16(i2c_fd, data);
			}
		}

		if(i2c_arduino)
		{
			/*
			24 bit data field: 0RRR00TT0XXXXXXX0YYYYYYY
			RRR=RX ant
			TT=TX ant
			XXXXXXX=RX OC
			YYYYYYY=TX OC
			*/
			if(frame[0] & 0x01)
			{
				data32 = i2c_ard_ocant_data & 0x00fc7f00;
				data32 |= (frame[2] >> 1); /* add back in OC bits */
				data32 |= (frame[4] & 0x03) << 16;  /* add back TX ant bits */
			}
			else
			{
				data32 = i2c_ard_ocant_data & 0x0003007f;
				data32 |= (frame[2] << 7); /* add back in OC bits */
				data8 = (frame[3] & 0x60) >> 5; /* RX aux bits */
				if(data8 == 0)
				{
					data32 |= (frame[4] & 0x03) << 20; /* use TX bit positions */
				}
				else
				{
					data8 += 2;
					data32 |= (data8 & 0x07) << 20; /* RX bit positions */
				}
			}
			if(data32 != i2c_ard_ocant_data)
			{
				i2c_ard_ocant_data = data32;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
				i2c_write_addr_data24(i2c_fd, 0x4, data32);
			}
		}

	  break;
#endif

	case 2:  // C0: Bit 1-4 - Transmitter - C0: Bit 0 - MOX -> 0 = inactive
	case 3:  // C0: Bit 1-4 - Transmitter - C0: Bit 0 - MOX -> 1 = active
		/* set tx phase increment */

#ifdef CHARLY25
		c25_mox = frame[0] & 1;
		c25_tx_freq = ntohl(*(uint32_t *)(frame + 1));

#ifdef DEBUG_EP2
		fprintf(stderr, "DEBUG_EP2: c25_tx_freq: %zu\n\n", c25_tx_freq);
#endif

		if (c25_tx_freq < freq_min || c25_tx_freq > freq_max) break;

		if (c25_fpga_model > 1)
		{
			*tx_freq = (uint32_t)floor(c25_tx_freq / 122.88e6 * (1 << 30) + 0.5);
		}
		else
		{
			*tx_freq = (uint32_t)floor(c25_tx_freq / 125.0e6 * (1 << 30) + 0.5);
		}

		if (c25_rx1_bpf_present)
		{
			c25_rx1_bpf_board_i2c_data = c25_switch_bpf_tx_relay(C25_RX1_BPF_ADDR, c25_mox);
		}

		if (c25lc_trx_present)
		{
			c25_i2c_data = c25lc_switch_tx_lpf();
		}
		else if (c25ab_trx_present)
		{
			c25_i2c_data = c25ab_switch_tx_lpf();
		}
		else if (c25pp_trx_present)
		{
			c25_i2c_data = c25pp_switch_tx_lpf();

			if (c25_ext_board_present)
			{
				c25_bcd_encoder_i2c_data = c25_switch_bcd_encoder();
			}
		}
		break;
#endif

#ifndef CHARLY25
		freq = ntohl(*(uint32_t *)(frame + 1));
		if (freq < freq_min || freq > freq_max) break;

		*tx_freq = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);

		if (freq_data[0] != freq)
		{
			freq_data[0] = freq;
			alex_write();
			icom_write();
			if (i2c_misc) misc_write();
			if(i2c_arduino)
			{
				data = freq / 1000;
				if(data != i2c_ard_ftx_data)
				{
					i2c_ard_ftx_data = data;
					ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
					i2c_write_addr_data16(i2c_fd, 0x03, data);
				}
			}
		}
		break;
#endif

	case 4:  // C0: Bit 1-4 - Receiver 1 - C0: Bit 0 - MOX -> 0 = inactive
	case 5:  // C0: Bit 1-4 - Receiver 1 - C0: Bit 0 - MOX -> 1 = active
		/* set rx phase increment */

#ifdef CHARLY25
		c25_rx1_freq = ntohl(*(uint32_t *)(frame + 1));

#ifdef DEBUG_EP2
		fprintf(stderr, "DEBUG_EP2: c25_rx1_freq: %zu\n\n", c25_rx1_freq);
#endif

		if (c25_rx1_freq < freq_min || c25_rx1_freq > freq_max) break;

		if (c25_fpga_model > 1)
		{
			*rx_freq[0] = (uint32_t)floor(c25_rx1_freq / 122.88e6 * (1 << 30) + 0.5);
		}
		else
		{
			*rx_freq[0] = (uint32_t)floor(c25_rx1_freq / 125.0e6 * (1 << 30) + 0.5);
		}

		if (rx_sync_data) *rx_freq[1] = *rx_freq[0];

		if (freq_data[1] != c25_rx1_freq)
		{
			freq_data[1] = c25_rx1_freq;

			if (rx_sync_data)
			{
				/* reset rx los */
				*lo_rst &= ~3;
				*lo_rst |= 3;
			}

			if (c25_rx1_bpf_present)
			{
				c25_rx1_bpf_board_i2c_data = c25_switch_rx_bpf(C25_RX1_BPF_ADDR, c25_rx1_freq);
			}
		}
		break;
#endif

#ifndef CHARLY25
		freq = ntohl(*(uint32_t *)(frame + 1));
		if (freq < freq_min || freq > freq_max) break;

		*rx_freq[0] = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);

		if (rx_sync_data) *rx_freq[1] = *rx_freq[0];
		if (freq_data[1] != freq)
		{
			freq_data[1] = freq;
			if (rx_sync_data)
			{
				/* reset rx los */
				*lo_rst &= ~3;
				*lo_rst |= 3;
			}
			alex_write();
			if (i2c_misc) misc_write();

			if(i2c_arduino)
			{
				data = freq / 1000;
				if(data != i2c_ard_frx1_data)
				{
					i2c_ard_frx1_data = data;
					ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
					i2c_write_addr_data16(i2c_fd, 0x01, data);
				}
			}
		}
		break;
#endif

	case 6:  // C0: Bit 1-4 - Receiver 2 - C0: Bit 0 - MOX -> 0 = inactive
	case 7:  // C0: Bit 1-4 - Receiver 2 - C0: Bit 0 - MOX -> 1 = active
		/* set rx phase increment */
		if (rx_sync_data) break;

#ifdef CHARLY25
		c25_rx2_freq = ntohl(*(uint32_t *)(frame + 1));

#ifdef DEBUG_EP2
		fprintf(stderr, "DEBUG_EP2: c25_rx2_freq: %zu\n\n", c25_rx2_freq);
#endif

		if (c25_rx2_freq < freq_min || c25_rx2_freq > freq_max) break;

		if (c25_fpga_model > 1)
		{
			*rx_freq[1] = (uint32_t)floor(c25_rx2_freq / 122.88e6 * (1 << 30) + 0.5);
		}
		else
		{
			*rx_freq[1] = (uint32_t)floor(c25_rx2_freq / 125.0e6 * (1 << 30) + 0.5);
		}

		if (freq_data[2] != c25_rx2_freq)
		{
			freq_data[2] = c25_rx2_freq;

			if (c25_rx2_bpf_present)
			{
				c25_rx2_bpf_board_i2c_data = c25_switch_rx_bpf(C25_RX2_BPF_ADDR, c25_rx2_freq);
			}
		}
		break;
#endif

#ifndef CHARLY25
		freq = ntohl(*(uint32_t *)(frame + 1));
		if (freq < freq_min || freq > freq_max) break;

		*rx_freq[1] = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);

		if (freq_data[2] != freq)
		{
			freq_data[2] = freq;
			alex_write();
			if (i2c_misc) misc_write();
			if(i2c_arduino)
			{
				data = freq / 1000;
				if(data != i2c_ard_frx2_data)
				{
				i2c_ard_frx2_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
				i2c_write_addr_data16(i2c_fd, 0x02, data);
				}
			}
		}
		break;
#endif

	case 18:
	case 19:

#ifndef CHARLY25
		data = (frame[2] & 0x40) << 9 | frame[4] << 8 | frame[3];
		if (alex_data_1 != data)
		{
			alex_data_1 = data;
			alex_write();
		}

		if (i2c_misc)
		{
			data = (frame[3] & 0x80) >> 6 | (frame[3] & 0x20) >> 5;
			if (misc_data_2 != data)
			{
				misc_data_2 = data;
				misc_write();
			}
		}

		/* configure ALEX */
		if (i2c_alex)
		{
			data = frame[4] << 8 | frame[3];
			if (i2c_alex_data != data)
			{
				i2c_alex_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ALEX);
				i2c_write_addr_data16(i2c_fd, 0x02, data);
			}
		}

		/* configure level */
		data = frame[1];
		if (i2c_level)
		{
			if (i2c_level_data != data)
			{
				i2c_level_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_LEVEL);
				i2c_write_addr_data16(i2c_fd, 0x02, data);
			}
		}
		else if (i2c_drive)
		{
			if (i2c_drive_data != data)
			{
				i2c_drive_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_DRIVE);
				i2c_write_addr_data16(i2c_fd, 0xa9, data << 8 | data);
			}
		}
		else if(i2c_arduino)
		{
			/*
			24 bit data field 000000BA000RRRRR00TTTTTT
			BA=PA disable, 6m LNA
			RRRRR=5 bits RX att when in TX
			TTTTTT=6 bits TX att (0.5 dB units)
			*/
			data32 = i2c_ard_txatt_data & 0x00001f00; /* remove TX att */
			data32 |= log_table_lookup[data];
			data32 |= (frame[3] & 0xc0) << 10;
			if(data32 != i2c_ard_txatt_data)
			{
				i2c_ard_txatt_data = data32;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
				i2c_write_addr_data24(i2c_fd, 0x05, data32);
			}
		}
		else
		{
			*tx_level = (int16_t)floor(data * 125.92 + 0.5);
		}
#else
		data = frame[1];
		if (c25_fpga_model > 1)
		{
			*tx_level = (int16_t)floor(data * 128.494 + 0.5);
		}
		else
		{
			*tx_level = (int16_t)floor(data * 125.92 + 0.5);
		}
#endif

		/* configure microphone boost */
		if (i2c_codec)
		{
			boost = frame[2] & 0x01;
			if (i2c_boost_data != boost)
			{
				i2c_boost_data = boost;
#ifdef CHARLY25
				ioctl(i2c_fd, I2C_SLAVE, AUDIO_CODEC_ADDR);
#else
				ioctl(i2c_fd, I2C_SLAVE, ADDR_CODEC);
#endif

				i2c_write_addr_data8(i2c_fd, 0x08, 0x14 + boost);
			}
		}
		break;

#ifndef CHARLY25
	case 20:
	case 21:
		rx_att_data = frame[4] & 0x1f;
		if(i2c_misc)
		{
			data = frame[4] & 0x1f;
			if(misc_data_0 != data)
			{
				misc_data_0 = data;
				misc_write();
			}
		}
		if(i2c_arduino)
		{
			/*
			16 bit data field 000RRRRR000TTTTT
			RRRRR=RX2
			TTTTT=RX1
			*/
			data = i2c_ard_rxatt_data & 0x00001f00; /* remove RX1 att */
			data |= (frame[4] & 0x1f);
			if(data != i2c_ard_rxatt_data)
			{
				i2c_ard_rxatt_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
				i2c_write_addr_data16(i2c_fd, 0x06, data);
			}
		}
		break;
#endif

	case 22:
	case 23:

#ifndef CHARLY25
		if (i2c_misc)
		{
			data = frame[1] & 0x1f;
			if (misc_data_1 != data)
			{
				misc_data_1 = data;
				misc_write();
			}
		}
#endif

		cw_reversed = (frame[2] >> 6) & 1;
		cw_speed = frame[3] & 63;
		cw_mode = (frame[3] >> 6) & 3;
		cw_weight = frame[4] & 127;
		cw_spacing = (frame[4] >> 7) & 1;
		break;

#ifndef CHARLY25
		if(i2c_arduino)
		{
			/*
			16 bit data field 000RRRRR000TTTTT
			RRRRR=RX2;
			TTTTT=RX1
			*/
			data = i2c_ard_rxatt_data & 0x1f; /* remove RX2 att */
			data |= (frame[1] & 0x1f) << 8;
			if(data != i2c_ard_rxatt_data)
			{
				i2c_ard_rxatt_data = data;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
				i2c_write_addr_data16(i2c_fd, 0x06, data);
			}
		}
		break;
#endif

#ifdef CHARLY25
	case 24:
	case 25:
		if(c25_ext_board_present)
		{
			// C1: Bit 0-1 - RX2 12dB attenuator and RX2 24db attenuator, Bit 2-3 - RX2 18dB preamp 1 and 2, Bit 4 - unused, Bit 5 - RX2 Predistorsion switch, Bit 6 - VHF/UHF switch RX1, Bit 7 - VHF/UHF switch TX
			// C2: Bit 0-4 - Step attenuator 0-31dB, Bit 5 - RP external OFF (SDR mode on), Bit 6 - RP TX channel 2 envelope modulation, Bit 7 - unused

			// switch the functions of the Charly 25 extension board
			data = frame[1];
			data |= frame[2] << 8;

			c25_ext_board_i2c_data = c25_switch_ext_board(data);

			// switch the BCD frequency encoder
			c25_bcd_encoder_i2c_data = c25_switch_bcd_encoder();
		}
		break;
#endif

#ifndef CHARLY25
	case 28:
	case 29:
		if(i2c_arduino)
		{
			/*
			24 bit data field 000000BA000RRRRR00TTTTTT
			BA=PA disable, 6m LNA
			RRRRR=5 bits RX att when in TX
			TTTTTT=6 bits TX att (0.5 dB units)
			*/
			data32 = i2c_ard_txatt_data & 0x0003003f; /* remove RX att */
			data32 |= (frame[3] & 0x1f) << 8;
			if(data32 != i2c_ard_txatt_data)
			{
				i2c_ard_txatt_data = data32;
				ioctl(i2c_fd, I2C_SLAVE, ADDR_ARDUINO);
				i2c_write_addr_data24(i2c_fd, 0x05, data32);
			}
		}
		break;
#endif

	case 30:
	case 31:
		cw_int_data = frame[1] & 1;
		dac_level_data = frame[2];
		cw_delay = frame[3];

		if (i2c_codec)
		{
			data = dac_level_data;
			*dac_level = data * 64;
		}
		break;

	case 32:
	case 33:
		cw_hang = (frame[1] << 2) | (frame[2] & 3);

		if (i2c_codec)
		{
			freq = (frame[3] << 4) | (frame[4] & 255);
			*dac_freq = (uint32_t)floor(freq / 48.0e3 * (1 << 30) + 0.5);
		}
		break;
	}

#ifdef DEBUG_EP2
	fprintf(stderr, "DEBUG_EP2: Frames after switch case statement:\n");
	fprintf(stderr, "DEBUG_EP2: frame[0]: %d\n", frame[0]);
	fprintf(stderr, "DEBUG_EP2: frame[1]: %d\n", frame[1]);
	fprintf(stderr, "DEBUG_EP2: frame[2]: %d\n", frame[2]);
	fprintf(stderr, "DEBUG_EP2: frame[3]: %d\n", frame[3]);
	fprintf(stderr, "DEBUG_EP2: frame[4]: %d\n", frame[4]);
#endif
}

void *handler_ep6(void *arg)
{
	int i, j, k, m, n, size, rate_counter;
	int data_offset, header_offset;
	uint32_t counter;
	int32_t value;
	uint16_t audio[512];
	uint8_t data0[4096];
	uint8_t data1[4096];
	uint8_t data2[4096];
	uint8_t data3[4096];
	uint8_t buffer[25 * 1032];
	uint8_t *pointer;
	struct iovec iovec[25][1];
	struct mmsghdr datagram[25];
	uint8_t id[4] = { 0xef, 0xfe, 1, 6 };
	uint8_t header[40] =
	{
		127, 127, 127, 0, 0, 33, 17, 21,
		127, 127, 127, 8, 0, 0, 0, 0,
		127, 127, 127, 16, 0, 0, 0, 0,
		127, 127, 127, 24, 0, 0, 0, 0,
		127, 127, 127, 32, 66, 66, 66, 66
	};

#ifdef CHARLY25
	// C2 – Mercury (receiver) software serial number (0 to 255) - set to 33 by default
	if (c25_rx1_bpf_present) header[5] = 146;
	if (c25_rx2_bpf_present) header[5] = 147;

	// C3 - Penelope (transmitter) software serial number (0 to 255) – set to 17 by default
	header[6] = C25_TRX_ID;

	// C4 - Ozy/Magister or Metis (Ethernet interface) or Hermes software serial number (0 to 255) - set to 21 by default
	if (i2c_codec) header[7] = 128;
#endif

	memset(audio, 0, sizeof(audio));
	memset(iovec, 0, sizeof(iovec));
	memset(datagram, 0, sizeof(datagram));

	for (i = 0; i < 25; ++i)
	{
		memcpy(buffer + i * 1032, id, 4);
		iovec[i][0].iov_base = buffer + i * 1032;
		iovec[i][0].iov_len = 1032;
		datagram[i].msg_hdr.msg_iov = iovec[i];
		datagram[i].msg_hdr.msg_iovlen = 1;
		datagram[i].msg_hdr.msg_name = &addr_ep6;
		datagram[i].msg_hdr.msg_namelen = sizeof(addr_ep6);
	}

	header_offset = 0;
	counter = 0;
	rate_counter = 1 << rate;
	k = 0;

	if (i2c_codec)
	{
		/* reset codec ADC fifo */
		*rx_rst |= 2;
		*rx_rst &= ~2;
	}

	/* reset rx fifo */
	*rx_rst |= 1;
	*rx_rst &= ~1;

	while (1)
	{
		if (!enable_thread) break;

		size = receivers * 6 + 2;
		n = 504 / size;
		m = 256 / n;

		if ((i2c_codec && *adc_cntr >= 1024) || *rx_cntr >= 8192)
		{
			if (i2c_codec)
			{
				/* reset codec ADC fifo */
				*rx_rst |= 2;
				*rx_rst &= ~2;
			}

			/* reset rx fifo */
			*rx_rst |= 1;
			*rx_rst &= ~1;
		}

		while (*rx_cntr < m * n * 16) usleep(1000);

		if (i2c_codec && --rate_counter == 0)
		{
			for (i = 0; i < m * n * 2; ++i)
			{
				audio[i] = *adc_data;
			}
			rate_counter = 1 << rate;
			k = 0;
		}

		for (i = 0; i < m * n * 16; i += 8)
		{
			*(uint64_t *)(data0 + i) = *rx_data;
			*(uint64_t *)(data1 + i) = *rx_data;
			*(uint64_t *)(data2 + i) = *rx_data;
			*(uint64_t *)(data3 + i) = *rx_data;
		}

		data_offset = 0;
		for (i = 0; i < m; ++i)
		{
			*(uint32_t *)(buffer + i * 1032 + 4) = htonl(counter);
			++counter;
		}

		for (i = 0; i < m * 2; ++i)
		{
			pointer = buffer + i * 516 - i % 2 * 4 + 8;
			memcpy(pointer, header + header_offset, 8);
			pointer[3] |= (*gpio_in & 7) | cw_ptt;

#ifdef CHARLY25
			if(c25_ext_board_present)
			{
				if (header_offset == 8)
				{
					// Forward power from ADS 1015 Ain2
					pointer[6] = (c25_adc_conversion_register[2] >> 8) & 0xff;
					pointer[7] = c25_adc_conversion_register[2] & 0xff;

#ifdef DEBUG_ADC_FWD
					fprintf(stderr, "DEBUG_ADC_FWD: c25_adc_conversion_register[2], pointer[6] and pointer[7]: %u, %u, %u\n",c25_adc_conversion_register[2], pointer[6], pointer[7]);
#endif
				}
				else if (header_offset == 16)
				{
					// Reverse power from ADS 1015 Ain3

					pointer[4] = (c25_adc_conversion_register[3] >> 8) & 0xff;
					pointer[5] = c25_adc_conversion_register[3] & 0xff;

#ifdef DEBUG_ADC_REF
					fprintf(stderr, "DEBUG_ADC_REF: c25_adc_conversion_register[3], pointer[4] and pointer[5]: %u, %u, %u\n",c25_adc_conversion_register[3], pointer[4], pointer[5]);
#endif

					// Device current from ADS 1015 Ain0
					pointer[6] = (c25_adc_conversion_register[0] >> 8) & 0xff;
					pointer[7] = c25_adc_conversion_register[0] & 0xff;
#ifdef DEBUG_ADC_TOTAL_CUR
					fprintf(stderr, "DEBUG_ADC_TOTAL_CUR: c25_adc_conversion_register[0], pointer[6] and pointer[7]: %u, %u, %u\n",c25_adc_conversion_register[0], pointer[6], pointer[7]);
#endif
				}
				else if(header_offset == 24)
				{
					// PA current from ADS 1015 Ain1
					pointer[4] = (c25_adc_conversion_register[1] >> 8) & 0xff;
					pointer[5] = c25_adc_conversion_register[1] & 0xff;
#ifdef DEBUG_ADC_PA_CUR
					fprintf(stderr, "DEBUG_ADC_PA_CUR: c25_adc_conversion_register[1], pointer[4] and pointer[5]: %u, %u, %u\n",c25_adc_conversion_register[1], pointer[4], pointer[5]);
#endif
				}
			}
			else
#endif
			{
				if (header_offset == 8)
				{
					value = xadc[152] >> 3;
					pointer[6] = (value >> 8) & 0xff;
					pointer[7] = value & 0xff;
				}
				else if (header_offset == 16)
				{
					value = xadc[144] >> 3;
					pointer[4] = (value >> 8) & 0xff;
					pointer[5] = value & 0xff;
					value = xadc[145] >> 3;
					pointer[6] = (value >> 8) & 0xff;
					pointer[7] = value & 0xff;
				}
				else if(header_offset == 24)
				{
					value = xadc[153] >> 3;
					pointer[4] = (value >> 8) & 0xff;
					pointer[5] = value & 0xff;
				}
			}
			header_offset = header_offset >= 32 ? 0 : header_offset + 8;

			pointer += 8;
			memset(pointer, 0, 504);
			for (j = 0; j < n; ++j)
			{
				memcpy(pointer, data0 + data_offset, 6);
				if (size > 8)
				{
					memcpy(pointer + 6, data1 + data_offset, 6);
				}
#ifndef ANANXD
				if (size > 14)
				{
					memcpy(pointer + 12, data2 + data_offset, 6);
				}
				if (size > 20)
				{
					memcpy(pointer + 18, data3 + data_offset, 6);
				}
#else
				if(size > 20)
				{
					memcpy(pointer + 18, data2 + data_offset, 6);
				}
				if(size > 26)
				{
					memcpy(pointer + 24, data3 + data_offset, 6);
				}
#endif
				data_offset += 8;
				pointer += size;

				if (i2c_codec) memcpy(pointer - 2, &audio[(k++) >> rate], 2);
			}
		}

#ifdef CHARLY25_TCP
		if (sock_TCP_Client > -1)
		{
			if (sendmmsg(sock_TCP_Client, datagram, m, 0) < 0)
			{
#ifdef DEBUG_TCP
				fprintf(stderr, "DEBUG_TCP: RP -> PC: TCP sendmmsg error occurred at sequence number: %u !\n", counter);
#endif
			}
		}
		else
#endif
		{
			sendmmsg(sock_ep2, datagram, m, 0);
		}

#ifdef DEBUG_PROT
		fprintf(stderr, "DEBUG_PROT: RP -> PC: Sequence number: %u and %u further sequences from %u receivers sent!\n", counter, m-1, receivers);
#endif
	}
	active_thread = 0;
	return NULL;
}

inline int cw_input()
{

#ifdef DEBUG_CW
	static int cw_debug_memory;
#endif

	int input;
	if (!cw_int_data) return 0;
	input = (*gpio_in >> 1) & 3;

#ifdef DEBUG_CW
	if (input != cw_debug_memory)
	{
		cw_debug_memory = input;
		fprintf(stderr, "DEBUG_CW: input: %u\n", input);
	}
#endif

	if (cw_reversed) input = (input & 1) << 1 | input >> 1;
	return input;
}

inline void cw_on()
{
	int delay = 1200 / cw_speed;
	if (cw_delay < delay) delay = cw_delay;
	/* PTT on */
	*tx_rst |= 16;
	cw_ptt = 1;
	tx_mux[16] = 1;
	tx_mux[0] = 2;
	tx_mux_data = 1;

#ifdef CHARLY25
	c25_mox = true;

	if (c25_rx1_bpf_present)
	{
		c25_rx1_bpf_board_i2c_data = c25_switch_bpf_tx_relay(C25_RX1_BPF_ADDR, c25_mox);
	}

	if (c25lc_trx_present)
	{
		c25lc_switch_tx_lpf();
	}
	else if (c25ab_trx_present)
	{
		c25ab_switch_tx_lpf();
	}
	else if (c25pp_trx_present)
	{
		c25pp_switch_tx_lpf();
	}
#endif

#ifdef DEBUG_CW
	fprintf(stderr, "DEBUG_CW: PTT on ------\n");
#endif

	if (i2c_codec && dac_level_data > 0)
	{
		*tx_rst |= 8; /* sidetone on */
	}
	while (delay--)
	{
		usleep(1000);
		cw_memory[0] = cw_input();
		if (cw_mode == 1 && !cw_memory[0]) cw_memory[1] = 0;
		else cw_memory[1] |= cw_memory[0];
	}
	*tx_rst |= 4; /* RF on */

#ifdef DEBUG_CW
	fprintf(stderr, "DEBUG_CW: RF on\n");
#endif
}

inline void cw_off()
{
	int delay = 1200 / cw_speed;
	if (cw_delay < delay) delay = cw_delay;
	if (i2c_codec)
	{
		*tx_rst &= ~8; /* sidetone off */
	}
	while (delay--)
	{
		usleep(1000);
		cw_memory[0] = cw_input();
		cw_memory[1] |= cw_memory[0];
	}
	*tx_rst &= ~4; /* RF off */
	cw_ptt_delay = cw_hang > 0 ? cw_hang : 10;

#ifdef DEBUG_CW
	fprintf(stderr, "DEBUG_CW: RF off\n");
#endif
}

inline void cw_ptt_off()
{
	if (--cw_ptt_delay > 0) return;

	/* PTT off */
	*tx_rst &= ~16;
	cw_ptt = 0;
	/* reset tx fifo */
	*tx_rst |= 1;
	*tx_rst &= ~1;
	tx_mux[16] = 0;
	tx_mux[0] = 2;
	tx_mux_data = 0;

#ifdef CHARLY25
	c25_mox = false;

	if (c25lc_trx_present)
	{
		c25lc_switch_tx_lpf();
	}
	else if (c25ab_trx_present)
	{
		c25ab_switch_tx_lpf();
	}
	else if (c25pp_trx_present)
	{
		c25pp_switch_tx_lpf();
	}

	if (c25_rx1_bpf_present)
	{
		c25_rx1_bpf_board_i2c_data = c25_switch_bpf_tx_relay(C25_RX1_BPF_ADDR, c25_mox);
	}
#endif

#ifdef DEBUG_CW
	fprintf(stderr, "DEBUG_CW: PTT off ------\n");
#endif
}

inline void cw_signal_delay(int code)
{
	int delay = code ? 1200 / cw_speed : 3600 * cw_weight / (50 * cw_speed);
	delay -= cw_delay;
	if (delay < 0) delay = 0;
	while (delay--)
	{
		usleep(1000);
		cw_memory[0] = cw_input();
		if (cw_mode == 1 && !cw_memory[0]) cw_memory[1] = 0;
		else cw_memory[1] |= cw_memory[0];
	}
}

inline void cw_space_delay(int code)
{
	int delay = code ? 1200 / cw_speed - cw_delay : 2400 / cw_speed;
	if (delay < 0) delay = 0;
	while (delay--)
	{
		usleep(1000);
		if (cw_ptt) cw_ptt_off();
		cw_memory[0] = cw_input();
		cw_memory[1] |= cw_memory[0];
	}
}

void *handler_keyer(void *arg)
{
	int state, delay;

	while (1)
	{
		usleep(1000);
		if (cw_ptt) cw_ptt_off();
		if (!(cw_memory[0] = cw_input())) continue;

		if (cw_mode == 0)
		{

#ifdef CHARLY25
			if (cw_memory[0] & 3)
#else
			if (cw_memory[0] & 1)
#endif

			{
				cw_on();

#ifdef CHARLY25
				while (cw_memory[0] & 3)
#else
				while (cw_memory[1] & 1)
#endif

				{
					usleep(1000);
					cw_memory[0] = cw_input();
				}
				cw_off();
			}
			else
			{
				cw_on();
				delay = 1200 / cw_speed - cw_delay;
				if (delay > 0) usleep(delay * 1000);
				cw_off();
				cw_space_delay(1);
			}
		}
		else
		{
			state = 1;
			cw_memory[1] = cw_memory[0];
			while (1)
			{
				if (cw_memory[1] & (1 << state))
				{
					cw_memory[1] = 0;
					cw_on();
					cw_signal_delay(state);
					cw_off();
					cw_space_delay(1);
					cw_memory[1] &= ~(1 << state);
					cw_memory[1] |= cw_memory[0];
				}
				if (cw_memory[1])
				{
					state ^= 1;
				}
				else
				{
					if (cw_spacing)
					{
						state = 1;
						cw_space_delay(0);
						if (cw_memory[1]) continue;
					}
					break;
				}
			}
		}
	}
	return NULL;
}

#ifdef CHARLY25
void *c25_adc_handler(void *arg)
{
	uint8_t channel = 0;
	uint16_t config = 0;
	uint8_t conversion_result[2] = {0};

	while (1)
	{
		for (channel = 0; channel < 4; channel++)
		{
			// Start values
			config = 0x8385;

			// Set single-ended input channel
			switch (channel)
			{
				case 0:
					config |= 0x0040;
					break;
				case 1:
					config |= 0x0050;
					break;
				case 2:
					config |= 0x0060;
					break;
				case 3:
					config |= 0x0070;
					break;
			}

			// Address the config register to write to
			ioctl(i2c_fd, I2C_SLAVE, C25_ADC_1015_ADDR);

			// Write config register settings
			if (i2c_write_addr_data16(i2c_fd, 0x01, config) != 3)
			{
#ifdef DEBUG_ADC
				fprintf(stderr, "DEBUG_ADC: Config register write error!\n");
#endif
			}

			// Give the ADC time for the conversion
			usleep(10000);

			conversion_result[0] = 0;
			conversion_result[1] = 0;

			// Address the conversion register to read from
			ioctl(i2c_fd, I2C_SLAVE, C25_ADC_1015_ADDR);

			if (write(i2c_fd, conversion_result, 1) != 1)
			{
#ifdef DEBUG_ADC
				fprintf(stderr, "DEBUG_ADC: Conversion register write error!\n");
#endif
			}

			// Read the conversion result
			if (read(i2c_fd, conversion_result, 2) != 2)
			{
#ifdef DEBUG_ADC
				fprintf(stderr, "DEBUG_ADC: Read error!\n");
#endif
			}
			else
			{
				c25_adc_conversion_register[channel] = conversion_result[0] * 256 + (conversion_result[1] & 0b11110000);

				// Shift 12-bit results right by 4 bits
				c25_adc_conversion_register[channel] >>= 4;
			}

#ifdef DEBUG_ADC
			fprintf(stderr, "DEBUG_ADC: channel: %u, c25_adc_conversion_register: %u\n", channel, c25_adc_conversion_register[channel]);
#endif
		}
	}
	return NULL;
}
#endif