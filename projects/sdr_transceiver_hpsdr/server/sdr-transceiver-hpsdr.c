/*
19.04.2016 DC2PD: add code for bandpass and antenna switching via I2C.
22.08.2016 DL4AOI: add code for TX level switching via I2C.
22.08.2016 DL4AOI: output first four open collector outputs to the pins DIO4_P - DIO7_P of the extension connector E1.
02.09.2016 ON3VNA: add code for TX level switching via DS1803-10 (I2C).
21.09.2016 DC2PD: add code for controlling AD8331 VGA with MCP4725 DAC (I2C).
02.10.2016 DL9LJ: add code for controlling ICOM IC-735 (UART).
03.12.2016 KA6S: add CW keyer code.
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
24.02.2017 DG8MG: Added 60m band usage together with the 40m LPF filter on a Charly 25LC board
05.03.2017 DG8MG: Changed the behaviour of the RX/TX and PA relay switching routines to handle CW keying correctly
*/

// DG8MG
// Define CHARLY25AB for Charly 25AB specific builds
#define CHARLY25AB 1

// Define CHARLY25LC together with CHARLY25AB for Charly 25LC (4 band version) compatible builds
#define CHARLY25LC 1

// Define CHARLY25LC_60M_BAND together with CHARLY25LC for 60m band usage together with the 40m LPF filter on a Charly 25LC board
// #define CHARLY25LC_60M_BAND

// Define if codec is used
#define CHARLY25_CODEC 1

// Define CHARLY25AB_HAMLAB together with CHARLY25AB for HAMlab specific builds
#define CHARLY25AB_HAMLAB 1

// Define DEBUG for debug messages
// # define DEBUG 1

// Define DEBUG_ATT for ATT & PRE function call debug messages
// #define DEBUG_ATT 1

// Define DEBUG_CW for CW debug messages
// #define DEBUG_CW 1

// Define DEBUG_PA for PA & PTT debug messages
// #define DEBUG_PA 1
// DG8MG

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
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/i2c-dev.h>


#define I2C_SLAVE       0x0703 /* Use this slave address */
#define I2C_SLAVE_FORCE 0x0706 /* Use this slave address, even if it
                                  is already in use by a driver! */

#ifndef CHARLY25AB
#define ADDR_PENE 0x20 /* PCA9555 address 0 */
#define ADDR_ALEX 0x21 /* PCA9555 address 1 */
#define ADDR_LEVEL 0x22 /* PCA9555 address 2 */
#define ADDR_MISC 0x23 /* PCA9555 address 3 */
#define ADDR_DRIVE 0x28 /* DS1803 address 0 */
#define ADDR_CODEC 0x1A /* WM8731 or TLV320AIC23B address 0 */
#define ADDR_DAC0 0x60 /* MCP4725 address 0 */
#define ADDR_DAC1 0x61 /* MCP4725 address 1 */
#endif

#ifdef CHARLY25AB
#ifdef CHARLY25AB_HAMLAB
#define C25_I2C_DEVICE "/dev/i2c-1"
#else
#define C25_I2C_DEVICE "/dev/i2c-0"
#endif

/* I2C address of the Charly 25 audio codec WM8731 or TLV320AIC23B address 0 */
const uint8_t C25_CODEC_ADDR = 0x1A;

/* I2C address of the Charly 25 trx frontend */
const uint8_t C25_ADDR = 0x20;

/* I2C address of the first Charly 25 receiver BPF board */
const uint8_t C25_RX1_BPF_ADDR = 0x21;

/* I2C address of the second Charly 25 receiver BPF board */
const uint8_t C25_RX2_BPF_ADDR = 0x22;

/* C25 filter frequencies */
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
#endif

volatile uint32_t *rx_freq[4], *rx_rate, *tx_freq, *alex, *tx_mux, *dac_freq, *dac_mux;
volatile uint16_t *rx_cntr, *tx_cntr, *tx_level, *dac_cntr, *dac_level, *adc_cntr;
volatile uint8_t *gpio_in, *gpio_out, *rx_rst, *tx_rst, *codec_rst;
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

#ifndef CHARLY25AB
/* variables to handle I2C devices */
int i2c_fd;
int i2c_pene = 0;
int i2c_alex = 0;
int i2c_level = 0;
int i2c_misc = 0;
int i2c_drive = 0;
#endif

int i2c_codec = 0;

#ifndef CHARLY25AB
int i2c_dac0 = 0;
int i2c_dac1 = 0;

uint16_t i2c_pene_data = 0;
uint16_t i2c_alex_data = 0;
uint16_t i2c_level_data = 0;
uint16_t i2c_misc_data = 0;
uint16_t i2c_drive_data = 0;
uint16_t i2c_dac0_data = 0xfff;
uint16_t i2c_dac1_data = 0xfff;
#endif

uint8_t i2c_boost_data = 0;

uint8_t dac_mux_data = 0;
uint8_t dac_level_data = 0;

uint8_t cw_int_data = 0;

#ifndef CHARLY25AB
uint8_t rx_att_data = 0;
#endif

uint8_t tx_mux_data = 0;

uint16_t cw_hang = 0;
uint8_t cw_reversed = 0;
uint8_t cw_speed = 25;
uint8_t cw_mode = 0;
uint8_t cw_weight = 50;
uint8_t cw_spacing = 0;
uint8_t cw_delay = 0;

int cw_memory[2] = {0, 0};
int cw_ptt_delay = 0;

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

#ifndef CHARLY25AB
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

uint32_t freq_data[3] = {0, 0, 0};

void alex_write()
{
  uint32_t max = freq_data[1] > freq_data[2] ? freq_data[1] : freq_data[2];
  uint16_t manual = (alex_data_1 >> 15) & 0x01;
  uint16_t preamp = manual ? (alex_data_1 >> 6) & 0x01 : max > 50000000;
  uint16_t ptt = alex_data_0 & 0x01;
  uint32_t freq = 0;
  uint16_t hpf = 0, lpf = 0, data = 0;

  if(i2c_codec) return;

  freq = freq_data[1] < freq_data[2] ? freq_data[1] : freq_data[2];

  if(preamp) hpf = 0;
  else if(manual) hpf = alex_data_1 & 0x3f;
  else if(freq < 1416000) hpf = 0x20; /* bypass */
  else if(freq < 6500000) hpf = 0x10; /* 1.5 MHz HPF */
  else if(freq < 9500000) hpf = 0x08; /* 6.5 MHz HPF */
  else if(freq < 13000000) hpf = 0x04; /* 9.5 MHz HPF */
  else if(freq < 20000000) hpf = 0x01; /* 13 MHz HPF */
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

  if(alex_data_rx != data)
  {
    alex_data_rx = data;
    *alex = 1 << 16 | data;
  }

  freq = ptt ? freq_data[0] : max;

  if(manual) lpf = (alex_data_1 >> 8) & 0x7f;
  else if(freq > 32000000) lpf = 0x10; /* bypass */
  else if(freq > 22000000) lpf = 0x20; /* 12/10 meters */
  else if(freq > 15000000) lpf = 0x40; /* 17/15 meters */
  else if(freq > 8000000) lpf = 0x01; /* 30/20 meters */
  else if(freq > 4500000) lpf = 0x02; /* 60/40 meters */
  else if(freq > 2400000) lpf = 0x04; /* 80 meters */
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

  if(alex_data_tx != data)
  {
    alex_data_tx = data;
    *alex = 1 << 17 | data;
  }
}

uint16_t misc_data_0 = 0;
uint16_t misc_data_1 = 0;

inline int lower_bound(int *array, int size, int value)
{
  int i = 0, j = size, k;
  while(i < j)
  {
    k = (i + j) / 2;
    if(value > array[k]) i = k + 1;
    else j = k;
  }
  return i;
}

void misc_write()
{
  uint16_t code, data = 0;
  int i, freqs[20] = {1700000, 2100000, 3400000, 4100000, 6900000, 7350000, 9950000, 10200000, 13850000, 14500000, 18000000, 18250000, 20850000, 21650000, 24700000, 25150000, 27000000, 30000000, 49000000, 55000000};

  for(i = 0; i < 3; ++i)
  {
    code = lower_bound(freqs, 20, freq_data[i]);
    code = code % 2 ? code / 2 + 1 : 0;
    data |= code << (i * 4);
  }

  data |= (misc_data_0 & 0x03) << 14 | (misc_data_1 & 0x18) << 9;

  if(i2c_misc_data != data)
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
  uint8_t buffer[10] = {0xfe, 0xfe, 0x04, 0xe0, 0x05, 0x00, 0x00, 0x08, 0x01, 0xfd};

  if(freq < 2000000) band = 0x01;       /* 160m */
  else if(freq < 4000000) band = 0x03;  /*  80m */
  else if(freq < 8000000) band = 0x07;  /*  40m */
  else if(freq < 11000000) band = 0x10; /*  30m */
  else if(freq < 15000000) band = 0x14; /*  20m */
  else if(freq < 20000000) band = 0x18; /*  17m */
  else if(freq < 22000000) band = 0x21; /*  15m */
  else if(freq < 26000000) band = 0x24; /*  12m */
  else band = 0x28;                     /*  10m */

  switch(band)
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

  if(icom_band_data != band)
  {
    icom_band_data = band;
    if(uart_fd >= 0) write(uart_fd, buffer, 10);
  }
}
#endif

#ifdef CHARLY25AB
/* I2C handling Charly 25AB */
int i2c_fd;

bool c25ab_i2c_present = false;
bool c25lc_i2c_present = false;
bool c25_rx1_bpf_i2c_present = false;
bool c25_rx2_bpf_i2c_present = false;

uint16_t c25_switch_att_pre(uint8_t frame_3, uint16_t c25_i2c_data)
{
  static uint16_t c25_att_pre_i2c_data = 0;
  
  /* Wipe bits that might get changed */
  /* first and second f are LPFs */
  uint16_t c25_att_pre_i2c_new_data = c25_i2c_data & 0xfff0;
  
  /* Attenuator */
  c25_att_pre_i2c_new_data |= frame_3 & 3;  // C3: Bit 0-1 - Alex Attenuator (00 = 0dB, 01 = 10dB, 10 = 20dB, 11 = 30dB)
  
/*
DG8MG: On Charly 25AB hardware C3 bit 3 is used for the switching of the second preamp
C3
0 0 0 0 0 0 0 0
| | | | | | | |
| | | | | | + +------------ Alex Attenuator (00 = 0dB, 01 = 10dB, 10 = 20dB, 11 = 30dB)
| | | | | +---------------- Preamp On/Off (0 = Off, 1 = On)
| | | | +------------------ LT2208 Dither (0 = Off, 1 = On)  // DG8MG: On Charly 25AB hardware this bit is used for the switching of the second preamp
| | | + ------------------- LT2208 Random (0= Off, 1 = On)
| + + --------------------- Alex Rx Antenna (00 = none, 01 = Rx1, 10 = Rx2, 11 = XV)
+ ------------------------- Alex Rx out (0 = off, 1 = on). Set if Alex Rx Antenna > 0.
*/

  /* Activate preamp one and two as expected from the frontend software (f.e. PowerSDR Charly 25 / HAMlab Edition) */
  c25_att_pre_i2c_new_data |= frame_3 & 12;  // C3: Bit 2 - Preamp On/Off (0 = Off, 1 = On), Bit 3 -  LT2208 Dither (0 = Off, 1 = On)
  
  if (c25_att_pre_i2c_new_data != c25_att_pre_i2c_data)
  {
    c25_att_pre_i2c_data = c25_att_pre_i2c_new_data;
    ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
    i2c_write_addr_data16(i2c_fd, 0x02, c25_att_pre_i2c_data);

#ifdef DEBUG_ATT
    fprintf(stderr, "ATT & PRE bitmask in hex: %x\n", c25_att_pre_i2c_data & 0x000f);
#endif  
  }
  
  return c25_att_pre_i2c_data; 
}

uint16_t c25ab_switch_tx_lpf(bool mox, uint16_t c25ab_i2c_data, uint32_t c25_tx_freq)
{
  static uint16_t c25ab_tx_lpf_i2c_data = 0;
  
  /* Wipe bits that might get changed */  
  uint16_t c25ab_tx_lpf_i2c_new_data = c25ab_i2c_data & 0x004f;

  /* Switch LPF depending on TX frequency */
  if(C25_6M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_6M_LOW_FREQ)  /* 6m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 8;
  }
  else if((C25_10M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_10M_LOW_FREQ) || (C25_12M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_12M_LOW_FREQ))  /* 10/12m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 9;
  }
  else if((C25_15M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_15M_LOW_FREQ) || (C25_17M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_17M_LOW_FREQ))  /* 15/17m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 10;
  }
  else if((C25_20M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_20M_LOW_FREQ) || (C25_30M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_30M_LOW_FREQ))  /* 20/30m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 11;
  }
  else if((C25_40M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_40M_LOW_FREQ) || (C25_60M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_60M_LOW_FREQ))  /* 40/60m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 12;
  }
  else if(C25_80M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_80M_LOW_FREQ)  /* 80m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 13;
  }
  else if(C25_160M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_160M_LOW_FREQ)  /* 160m LPF */
  {
    c25ab_tx_lpf_i2c_new_data |= 1 << 14;
  }
  
  /* Turn PTT and PA only on if a LPF is active */
  if((c25ab_tx_lpf_i2c_new_data & 0x7f00) != 0)
  {
    c25ab_tx_lpf_i2c_new_data |= ((mox & 1) | (tx_mux_data & 1)) << 4;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
    c25ab_tx_lpf_i2c_new_data |= ((mox & 1) | (tx_mux_data & 1)) << 5;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
  }

#ifdef DEBUG
  fprintf(stderr, "LPF bitmask in hex: %x\n", (c25ab_tx_lpf_i2c_new_data & 0x7f00) >> 8);
  fprintf(stderr, "PA and PTT state %d\n", (c25ab_tx_lpf_i2c_new_data & 0x0030) >> 4);
  fprintf(stderr, "c25ab_i2c_present: %d, c25ab_tx_lpf_i2c_new_data in hex: %x\n", c25ab_i2c_present, c25ab_tx_lpf_i2c_new_data); 
#endif  

  if (c25ab_tx_lpf_i2c_new_data != c25ab_tx_lpf_i2c_data)
  {
    c25ab_tx_lpf_i2c_data = c25ab_tx_lpf_i2c_new_data;
    ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
    i2c_write_addr_data16(i2c_fd, 0x02, c25ab_tx_lpf_i2c_data);
    
#ifdef DEBUG_PA
    fprintf(stderr, "gpio_in: %u, mox: %u, tx_mux_data: %u, c25_tx_freq: %u\n", *gpio_in, mox, tx_mux_data, c25_tx_freq);
    fprintf(stderr, "PA and PTT state %d\n", (c25ab_tx_lpf_i2c_data & 0x3000) >> 12);
#endif 
  }
  
  return c25ab_tx_lpf_i2c_data; 
}
      
uint16_t c25lc_switch_tx_lpf(bool mox, uint16_t c25lc_i2c_data, uint32_t c25_tx_freq)
{
  static uint16_t c25lc_tx_lpf_i2c_data = 0;
  
  /* Wipe bits that might get changed */  
  uint16_t c25lc_tx_lpf_i2c_new_data = c25lc_i2c_data & 0x00ff;

  /* Switch LPF depending on TX frequency */
  if(C25_10M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_10M_LOW_FREQ)  /* 10m LPF */
  {
    c25lc_tx_lpf_i2c_new_data |= 1 << 8;
  }
  else if(C25_20M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_20M_LOW_FREQ)  /* 20m LPF */
  {
    c25lc_tx_lpf_i2c_new_data |= 1 << 9;
  }
  else if(C25_40M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_40M_LOW_FREQ)  /* 40m LPF */
  {
    c25lc_tx_lpf_i2c_new_data |= 1 << 10;
  }
  
#ifdef CHARLY25LC_60M_BAND
  else if(C25_60M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_60M_LOW_FREQ)  /* 40m LPF on 60m band */
  {
    c25lc_tx_lpf_i2c_new_data |= 1 << 10;
  }
#endif

  else if(C25_80M_HIGH_FREQ > c25_tx_freq && c25_tx_freq >= C25_80M_LOW_FREQ)  /* 80m LPF */
  {
    c25lc_tx_lpf_i2c_new_data |= 1 << 11;
  }
  
  /* Turn PTT and PA only on if a LPF is active */
  if((c25lc_tx_lpf_i2c_new_data & 0x0f00) != 0)
  {
    c25lc_tx_lpf_i2c_new_data |= ((mox & 1) | (tx_mux_data & 1)) << 12;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
    c25lc_tx_lpf_i2c_new_data |= ((mox & 1) | (tx_mux_data & 1)) << 13;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
  }

#ifdef DEBUG
  fprintf(stderr, "LPF bitmask in hex: %x\n", (c25lc_tx_lpf_i2c_new_data & 0x0f00) >> 8);
  fprintf(stderr, "PA and PTT state %d\n", (c25lc_tx_lpf_i2c_new_data & 0x3000) >> 12);
  fprintf(stderr, "c25lc_i2c_present: %d, c25lc_tx_lpf_i2c_new_data in hex: %x\n", c25lc_i2c_present, c25lc_tx_lpf_i2c_new_data); 
#endif  

  if (c25lc_tx_lpf_i2c_new_data != c25lc_tx_lpf_i2c_data)
  {
    c25lc_tx_lpf_i2c_data = c25lc_tx_lpf_i2c_new_data;
    ioctl(i2c_fd, I2C_SLAVE, C25_ADDR);
    i2c_write_addr_data16(i2c_fd, 0x02, c25lc_tx_lpf_i2c_data);

#ifdef DEBUG_PA
    fprintf(stderr, "gpio_in: %u, mox: %u, tx_mux_data: %u, c25_tx_freq: %u\n", *gpio_in, mox, tx_mux_data, c25_tx_freq);
    fprintf(stderr, "PA and PTT state %d\n", (c25lc_tx_lpf_i2c_data & 0x3000) >> 12);
#endif   
  }
  return c25lc_tx_lpf_i2c_data;
}

ssize_t c25_switch_rx_bpf(uint8_t c25_rx_bpf_addr, uint32_t c25_rx_freq)
{
  static uint16_t c25_rx_bpf_i2c_data = 0;
  
  /* Wipe bits that might get changed in this frame */  
  uint16_t c25_rx_bpf_i2c_new_data = 0x0000;

  /* Switch BPF depending on RX frequency */
  if(C25_6M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_6M_LOW_FREQ)  /* 6m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 12;
  }
  else if(C25_10M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_10M_LOW_FREQ)  /* 10m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 13;
  }
  else if(C25_12M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_12M_LOW_FREQ)  /* 12m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 14;
  }
  else if(C25_15M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_15M_LOW_FREQ)  /* 15m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 15;
  }    
  else if(C25_17M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_17M_LOW_FREQ)  /* 17m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 0;
  }
  else if(C25_20M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_20M_LOW_FREQ)  /* 20m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 1;
  }
  else if(C25_30M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_30M_LOW_FREQ)  /* 30m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 2;
  }
  else if(C25_40M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_40M_LOW_FREQ)  /* 40m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 3;
  }
  else if(C25_60M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_60M_LOW_FREQ)  /* 60m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 4;
  }
  else if(C25_80M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_80M_LOW_FREQ) /* 80m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 5;
  }
  else if(C25_160M_HIGH_FREQ > c25_rx_freq && c25_rx_freq >= C25_160M_LOW_FREQ) /* 160m BPF */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 6;
  }
  else  /* Default filter for LF or outside of the HAM bands */
  {
    c25_rx_bpf_i2c_new_data |= 1 << 7;
  }
  
#ifdef DEBUG 
  fprintf(stderr, "BPF bitmask in hex: %x\n", c25_rx_bpf_i2c_new_data);
#endif
  if (c25_rx_bpf_i2c_new_data != c25_rx_bpf_i2c_data)
  {
    c25_rx_bpf_i2c_data = c25_rx_bpf_i2c_new_data;
    ioctl(i2c_fd, I2C_SLAVE, c25_rx_bpf_addr);
    return i2c_write_addr_data16(i2c_fd, 0x02, c25_rx_bpf_i2c_data);
  }      
}
#endif

int main(int argc, char *argv[])
{
  int fd, i, j, size;
  struct sched_param param;
  pthread_attr_t attr;
  pthread_t thread;
  volatile uint32_t *slcr;
  volatile void *cfg, *sts;
  volatile int32_t *tx_ramp, *dac_ramp;
  volatile uint16_t *tx_size, *dac_size;
  float scale, ramp[1024], a[4] = {0.35875, 0.48829, 0.14128, 0.01168};
  uint8_t reply[11] = {0xef, 0xfe, 2, 0, 0, 0, 0, 0, 0, 21, 0};
  uint8_t id[4] = {0xef, 0xfe, 1, 6};
  uint32_t code;
  struct termios tty;
  struct ifreq hwaddr;
  struct sockaddr_in addr_ep2, addr_from[10];
  uint8_t buffer[8][1032];
  struct iovec iovec[8][1];
  struct mmsghdr datagram[8];
  struct timeval tv;
  struct timespec ts;
  int yes = 1;

  if((fd = open("/dev/mem", O_RDWR)) < 0)
  {
    perror("open");
    return EXIT_FAILURE;
  }

#ifdef CHARLY25AB
  if((i2c_fd = open(C25_I2C_DEVICE, O_RDWR)) >= 0)
  {    
    if(ioctl(i2c_fd, I2C_SLAVE_FORCE, C25_ADDR) >= 0)
    {
      /* set all pins to low */
      if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
      {

#ifdef CHARLY25LC
        c25lc_i2c_present = true;
#else
        c25ab_i2c_present = true;
#endif

        /* configure all pins as output */
        i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
      }
      else
      {
        fprintf(stderr, "Charly 25 TRX - I2C write error!\n");
      }
    }
    else
    {
      fprintf(stderr, "Charly 25 TRX - I2C ioctl error!\n");
    }
  }
  else
  {
    fprintf(stderr, "Charly 25 TRX - I2C open error!\n");
    // return EXIT_FAILURE;
  }

#ifndef CHARLY25AB_HAMLAB 
  if(ioctl(i2c_fd, I2C_SLAVE, C25_RX1_BPF_ADDR) >= 0)
  {
    /* set all pins to low */
    if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) >= 0)
    {
      c25_rx1_bpf_i2c_present = true;
      /* configure all pins as output */
      i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
    }
    else
    {
      fprintf(stderr, "Charly 25 RX1 BPF - I2C write error!\n");
    }
  }
  else
  {
    fprintf(stderr, "Charly 25 RX1 BPF - I2C ioctl error!\n");
  }

  if(ioctl(i2c_fd, I2C_SLAVE, C25_RX2_BPF_ADDR) >= 0)
  {
    /* set all pins to low */
    if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) >= 0)
    {
      c25_rx2_bpf_i2c_present = true;
      /* configure all pins as output */
      i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
    }
    else
    {
      fprintf(stderr, "Charly 25 RX2 BPF - I2C write error!\n");
    }
  }
  else
  {
    fprintf(stderr, "Charly 25 RX2 BPF - I2C ioctl error!\n");
  }
#endif

#ifdef CHARLY25_CODEC
#ifndef CHARLY25AB_HAMLAB  // Charly 25 with audio codec present but not in a HAMlab
  if(ioctl(i2c_fd, I2C_SLAVE, C25_CODEC_ADDR) >= 0)
  {
    /* reset */
    if(i2c_write_addr_data8(i2c_fd, 0x1e, 0x00) >= 0)
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
    else
    {
      fprintf(stderr, "Charly 25 AUDIO CODEC - I2C write error!\n");
    }
  }
  else
  {
    fprintf(stderr, "Charly 25 AUDIO CODEC - I2C ioctl error!\n");
  }
#endif
#endif
  
  // Version and hardware info for debugging only!
  fprintf(stderr, "Version 05032017: ");
#ifdef CHARLY25AB_HAMLAB
  fprintf(stderr, "HAMlab Edition\n");
#else
  fprintf(stderr, "Charly 25 Edition\n");
#endif
  if(c25ab_i2c_present) fprintf(stderr, "Hardware: Charly 25AB present\n");
  if(c25lc_i2c_present) fprintf(stderr, "Hardware: Charly 25LC present\n");
  if(c25_rx1_bpf_i2c_present) fprintf(stderr, "Hardware: Charly 25 RX 1 BPF present\n");
  if(c25_rx2_bpf_i2c_present) fprintf(stderr, "Hardware: Charly 25 RX 2 BPF present\n");
  if(i2c_codec) fprintf(stderr, "Hardware: Charly 25 AUDIO CODEC present\n");
#endif

#ifndef CHARLY25AB
  if((uart_fd = open("/dev/ttyPS1", O_RDWR | O_NOCTTY | O_NDELAY)) >= 0)
  {
    tcgetattr(uart_fd, &tty);
    cfsetspeed(&tty, (speed_t)B1200);
    cfmakeraw(&tty);
    tty.c_cflag &= ~(CSTOPB | CRTSCTS);
    tty.c_cflag |= CLOCAL | CREAD;
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &tty);
  }

  if((i2c_fd = open("/dev/i2c-0", O_RDWR)) >= 0)
  {
    if(ioctl(i2c_fd, I2C_SLAVE_FORCE, ADDR_PENE) >= 0)
    {
      /* set all pins to low */
      if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
      {
        i2c_pene = 1;
        /* configure all pins as output */
        i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_ALEX) >= 0)
    {
      /* set all pins to low */
      if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
      {
        i2c_alex = 1;
        /* configure all pins as output */
        i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_LEVEL) >= 0)
    {
      /* set all pins to low */
      if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
      {
        i2c_level = 1;
        /* configure all pins as output */
        i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_MISC) >= 0)
    {
      /* set all pins to low */
      if(i2c_write_addr_data16(i2c_fd, 0x02, 0x0000) > 0)
      {
        i2c_misc = 1;
        /* configure all pins as output */
        i2c_write_addr_data16(i2c_fd, 0x06, 0x0000);
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_DRIVE) >= 0)
    {
      /* set both potentiometers to 0 */
      if(i2c_write_addr_data16(i2c_fd, 0xa9, 0x0000) > 0)
      {
        i2c_drive = 1;
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_DAC0) >= 0)
    {
      if(i2c_write_data16(i2c_fd, i2c_dac0_data) > 0)
      {
      i2c_dac0 = 1;
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_DAC1) >= 0)
    {
      if(i2c_write_data16(i2c_fd, i2c_dac1_data) > 0)
      {
      i2c_dac1 = 1;
      }
    }
    if(ioctl(i2c_fd, I2C_SLAVE, ADDR_CODEC) >= 0)
    {
      /* reset */
      if(i2c_write_addr_data8(i2c_fd, 0x1e, 0x00) > 0)
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

  slcr = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0xF8000000);
  sts = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40000000);
  cfg = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40001000);

#ifndef CHARLY25AB
  alex = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40002000);
#endif

  tx_mux = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40003000);
  tx_ramp = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40004000);
  dac_mux = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40005000);
  dac_ramp = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40006000);
  dac_data = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40007000);
  adc_data = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40008000);
  tx_data = mmap(NULL, 2*sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x4000a000);
  rx_data = mmap(NULL, 8*sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40010000);
  xadc = mmap(NULL, 16*sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40020000);

  rx_rst = ((uint8_t *)(cfg + 0));
  tx_rst = ((uint8_t *)(cfg + 1));
  codec_rst = ((uint8_t *)(cfg + 2));
  gpio_out = ((uint8_t *)(cfg + 3));

  rx_rate = ((uint32_t *)(cfg + 4));

  rx_freq[0] = ((uint32_t *)(cfg + 8));
  rx_freq[1] = ((uint32_t *)(cfg + 12));
  rx_freq[2] = ((uint32_t *)(cfg + 16));
  rx_freq[3] = ((uint32_t *)(cfg + 20));

  tx_freq = ((uint32_t *)(cfg + 24));
  tx_size = ((uint16_t *)(cfg + 28));
  tx_level = ((uint16_t *)(cfg + 30));

  dac_freq = ((uint32_t *)(cfg + 32));
  dac_size = ((uint16_t *)(cfg + 36));
  dac_level = ((uint16_t *)(cfg + 38));

  rx_cntr = ((uint16_t *)(sts + 12));
  tx_cntr = ((uint16_t *)(sts + 14));
  dac_cntr = ((uint16_t *)(sts + 16));
  adc_cntr = ((uint16_t *)(sts + 18));
  gpio_in = ((uint8_t *)(sts + 20));

  /* set FPGA clock to 143 MHz */
  slcr[2] = 0xDF0D;
  slcr[92] = (slcr[92] & ~0x03F03F30) | 0x00100700;

  /* set all GPIO pins to low */
  *gpio_out = 0;

  /* set default rx phase increment */
  *rx_freq[0] = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);
  *rx_freq[1] = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);
  *rx_freq[2] = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);
  *rx_freq[3] = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);

  /* set default rx sample rate */
  *rx_rate = 1000;

  /* set default tx phase increment */
  *tx_freq = (uint32_t)floor(600000 / 125.0e6 * (1 << 30) + 0.5);

  /* set tx ramp */
  size = 1001;
  ramp[0] = 0.0;
  for(i = 1; i <= size; ++i)
  {
    ramp[i] = ramp[i - 1] + a[0] - a[1] * cos(2.0 * M_PI * i / size) + a[2] * cos(4.0 * M_PI * i / size) - a[3] * cos(6.0 * M_PI * i / size);
  }
  scale = 6.1e6 / ramp[size];
  for(i = 0; i <= size; ++i)
  {
    tx_ramp[i] = (int32_t)floor(ramp[i] * scale + 0.5);
  }
  *tx_size = size;

  /* set default tx level */
  *tx_level = 32767;

  /* set default tx mux channel */
  tx_mux[16] = 0;
  tx_mux[0] = 2;

  /* reset tx and codec DAC fifo */
  *tx_rst |= 3;
  *tx_rst &= ~3;

  if(i2c_codec)
  {
    /* reset codec ADC fifo */
    *codec_rst |= 1;
    *codec_rst &= ~1;
    /* enable I2S interface */
    *codec_rst &= ~2;

    /* set default dac phase increment */
    *dac_freq = (uint32_t)floor(600 / 48.0e3 * (1 << 30) + 0.5);

    /* set dac ramp */
    size = 481;
    ramp[0] = 0.0;
    for(i = 1; i <= size; ++i)
    {
      ramp[i] = ramp[i - 1] + a[0] - a[1] * cos(2.0 * M_PI * i / size) + a[2] * cos(4.0 * M_PI * i / size) - a[3] * cos(6.0 * M_PI * i / size);
    }
    scale = 3.2e4 / ramp[size];
    for(i = 0; i <= size; ++i)
    {
      dac_ramp[i] = (int32_t)floor(ramp[i] * scale + 0.5);
    }
    *dac_size = size;

    /* set default dac level */
    *dac_level = 32767;

    /* set default dac mux channel */
    dac_mux[16] = 0;
    dac_mux[0] = 2;
  }
  else
  {
    /* enable ALEX interface */
    *codec_rst |= 2;
  }

  if((sock_ep2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    return EXIT_FAILURE;
  }

  strncpy(hwaddr.ifr_name, "eth0", IFNAMSIZ);
  ioctl(sock_ep2, SIOCGIFHWADDR, &hwaddr);
  for(i = 0; i < 6; ++i) reply[i + 3] = hwaddr.ifr_addr.sa_data[i];

  setsockopt(sock_ep2, SOL_SOCKET, SO_REUSEADDR, (void *)&yes , sizeof(yes));

  tv.tv_sec = 0;
  tv.tv_usec = 1000;
  setsockopt(sock_ep2, SOL_SOCKET, SO_RCVTIMEO, (void *)&tv , sizeof(tv));

  memset(&addr_ep2, 0, sizeof(addr_ep2));
  addr_ep2.sin_family = AF_INET;
  addr_ep2.sin_addr.s_addr = htonl(INADDR_ANY);
  addr_ep2.sin_port = htons(1024);

  if(bind(sock_ep2, (struct sockaddr *)&addr_ep2, sizeof(addr_ep2)) < 0)
  {
    perror("bind");
    return EXIT_FAILURE;
  }

  pthread_attr_init(&attr);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  param.sched_priority = 99;
  pthread_attr_setschedparam(&attr, &param);
  if(pthread_create(&thread, &attr, handler_keyer, NULL) < 0)
  {
    perror("pthread_create");
    return EXIT_FAILURE;
  }
  pthread_detach(thread);

  while(1)
  {
    memset(iovec, 0, sizeof(iovec));
    memset(datagram, 0, sizeof(datagram));

    for(i = 0; i < 8; ++i)
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

    size = recvmmsg(sock_ep2, datagram, 8, 0, &ts);
    if(size < 0 && errno != EAGAIN)
    {
      perror("recvfrom");
      return EXIT_FAILURE;
    }

    for(i = 0; i < size; ++i)
    {
      memcpy(&code, buffer[i], 4);

      switch(code)
      {
        case 0x0201feef:
          if(!tx_mux_data)
          {
            while(*tx_cntr > 1922) usleep(1000);
            if(*tx_cntr == 0) for(j = 0; j < 1260; ++j) *tx_data = 0;
            if((*gpio_out & 1) | (*gpio_in & 1))
            {
              for(j = 0; j < 504; j += 8) *tx_data = *(uint32_t *)(buffer[i] + 20 + j);
              for(j = 0; j < 504; j += 8) *tx_data = *(uint32_t *)(buffer[i] + 532 + j);
            }
            else
            {
              for(j = 0; j < 126; ++j) *tx_data = 0;
            }
          }

          if(i2c_codec)
          {
            if(!dac_mux_data)
            {
              while(*dac_cntr > 898) usleep(1000);
              if(*dac_cntr == 0) for(j = 0; j < 504; ++j) *dac_data = 0;
              for(j = 0; j < 504; j += 8) *dac_data = *(uint32_t *)(buffer[i] + 16 + j);
              for(j = 0; j < 504; j += 8) *dac_data = *(uint32_t *)(buffer[i] + 528 + j);
            }
          }
          process_ep2(buffer[i] + 11);
          process_ep2(buffer[i] + 523);
          break;
          
        case 0x0002feef:
          reply[2] = 2 + active_thread;
          memset(buffer[i], 0, 60);
          memcpy(buffer[i], reply, 11);
          sendto(sock_ep2, buffer[i], 60, 0, (struct sockaddr *)&addr_from[i], sizeof(addr_from[i]));
          break;
          
        case 0x0004feef:
          enable_thread = 0;
          while(active_thread) usleep(1000);
          break;
          
        case 0x0104feef:
        case 0x0204feef:
        case 0x0304feef:
          enable_thread = 0;
          while(active_thread) usleep(1000);
          memset(&addr_ep6, 0, sizeof(addr_ep6));
          addr_ep6.sin_family = AF_INET;
          addr_ep6.sin_addr.s_addr = addr_from[i].sin_addr.s_addr;
          addr_ep6.sin_port = addr_from[i].sin_port;
          enable_thread = 1;
          active_thread = 1;
          if(pthread_create(&thread, NULL, handler_ep6, NULL) < 0)
          {
            perror("pthread_create");
            return EXIT_FAILURE;
          }
          pthread_detach(thread);
          break;
      }
    }
  }
  close(sock_ep2);

  return EXIT_SUCCESS;
}

void process_ep2(uint8_t *frame)
{
  bool mox = false;
  static uint16_t c25_i2c_data = 0;
    
  uint32_t freq, c25_tx_freq, c25_rx1_freq, c25_rx2_freq, c25_rx3_freq, c25_rx4_freq;
  uint16_t data;
  uint8_t ptt, preamp, att, boost;
 
  switch(frame[0])
  {
    case 0:
    case 1:
      receivers = ((frame[4] >> 3) & 7) + 1;  // C4: Bit 3-5 - Number of Receivers (000 = 1, 111 = 8)

#ifndef CHARLY25AB
      /* set output pins */
      ptt = frame[0] & 0x01;  // C0: Bit 0 - MOX (1 = active, 0 = inactive)
      att = frame[3] & 0x03;  // C3: Bit 0-1 - Alex Attenuator (00 = 0dB, 01 = 10dB, 10 = 20dB, 11 = 30dB)
      preamp = ptt | (*gpio_in & 1) ? 0 : (frame[3] & 0x04) >> 2 | (rx_att_data == 0);
      *gpio_out = (frame[2] & 0x1e) << 3 | att << 2 | preamp << 1 | ptt;
#else
      /* set output pins */
      ptt = frame[0] & 0x01;
      *gpio_out = (*gpio_out & 0xfe) | ptt;
#endif

      /* set rx sample rate */
      rate = frame[1] & 3;  // C1: Bit 0-1 - Speed (00 = 48kHz, 01 = 96kHz, 10 = 192kHz, 11 = 384kHz)
      switch(frame[1] & 3)
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

#ifdef CHARLY25AB
      /* switch the attenuators and preamps on the Charly 25 board */
      c25_i2c_data = c25_switch_att_pre(frame[3], c25_i2c_data);
      break;
#endif

#ifndef CHARLY25AB
      data = (frame[4] & 0x03) << 8 | (frame[3] & 0xe0) | (frame[3] & 0x03) << 1 | (frame[0] & 0x01);
      if(alex_data_0 != data)
      {
        alex_data_0 = data;
        alex_write();
      }

      /* configure PENELOPE */
      if(i2c_pene)
      {
        data = (frame[3] & 0x1c) << 11 | (frame[4] & 0x03) << 11 | (frame[3] & 0x60) << 4 | (frame[3] & 0x03) << 7 | frame[2] >> 1;
        if(i2c_pene_data != data)
        {
          i2c_pene_data = data;
          ioctl(i2c_fd, I2C_SLAVE, ADDR_PENE);
          i2c_write_addr_data16(i2c_fd, 0x02, data);
        }
      }

      if(i2c_dac0)
      {
        data = rx_att_data + 10 * att;
        data = 4065 - (uint32_t)data * 4095 / 61;
        if(i2c_dac0_data != data)
        {
          i2c_dac0_data = data;
          ioctl(i2c_fd, I2C_SLAVE, ADDR_DAC0);
          i2c_write_data16(i2c_fd, data);
        }
      }
      break; 
#endif

    case 2:  // C0: Bit 1-4 - Transmitter - C0: Bit 0 - MOX -> 0 = inactive 
    case 3:  // C0: Bit 1-4 - Transmitter - C0: Bit 0 - MOX -> 1 = active
      /* set tx phase increment */

#ifdef CHARLY25AB    
      mox = frame[0] & 1;
      c25_tx_freq = ntohl(*(uint32_t *)(frame + 1));
      
#ifdef DEBUG
      fprintf(stderr, "c25_tx_freq: %zu\n\n", c25_tx_freq);
#endif

      if(c25_tx_freq < freq_min || c25_tx_freq > freq_max) break;
      *tx_freq = (uint32_t)floor(c25_tx_freq / 125.0e6 * (1 << 30) + 0.5);

      if (c25ab_i2c_present)
      {
        c25_i2c_data = c25ab_switch_tx_lpf(mox, c25_i2c_data, c25_tx_freq);
      }
      
      if (c25lc_i2c_present)
      {
        c25_i2c_data = c25lc_switch_tx_lpf(mox, c25_i2c_data, c25_tx_freq);
      }
      
      break;
#endif

#ifndef CHARLY25AB
     freq = ntohl(*(uint32_t *)(frame + 1));
     if(freq_data[0] != freq)
      {
        freq_data[0] = freq;
        alex_write();
        icom_write();
        if(i2c_misc) misc_write();
      }
      if(freq < freq_min || freq > freq_max) break;
      *tx_freq = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);
      break;
#endif

    case 4:  // C0: Bit 1-4 - Receiver 1 - C0: Bit 0 - MOX -> 0 = inactive
    case 5:  // C0: Bit 1-4 - Receiver 1 - C0: Bit 0 - MOX -> 1 = active
      /* set rx phase increment */

#ifdef CHARLY25AB
      c25_rx1_freq = ntohl(*(uint32_t *)(frame + 1));
      
#ifdef DEBUG
      fprintf(stderr, "c25_rx1_freq: %zu\n\n", c25_rx1_freq);
#endif

      if(c25_rx1_freq < freq_min || c25_rx1_freq > freq_max) break;
      *rx_freq[0] = (uint32_t)floor(c25_rx1_freq / 125.0e6 * (1 << 30) + 0.5);

      if (c25_rx1_bpf_i2c_present)
      {
        c25_switch_rx_bpf(C25_RX1_BPF_ADDR, c25_rx1_freq);  
      }
      break;
#endif      

#ifndef CHARLY25AB
      freq = ntohl(*(uint32_t *)(frame + 1));
      if(freq_data[1] != freq)
      {
        freq_data[1] = freq;
        alex_write();
        if(i2c_misc) misc_write();
      }
      if(freq < freq_min || freq > freq_max) break;
      *rx_freq[0] = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);
      break;
#endif
      
    case 6:  // C0: Bit 1-4 - Receiver 2 - C0: Bit 0 - MOX -> 0 = inactive
    case 7:  // C0: Bit 1-4 - Receiver 2 - C0: Bit 0 - MOX -> 1 = active
      /* set rx phase increment */

#ifdef CHARLY25AB
      c25_rx2_freq = ntohl(*(uint32_t *)(frame + 1));
      
#ifdef DEBUG
      fprintf(stderr, "c25_rx2_freq: %zu\n\n", c25_rx2_freq);
#endif

      if(c25_rx2_freq < freq_min || c25_rx2_freq > freq_max) break;
      *rx_freq[1] = (uint32_t)floor(c25_rx2_freq / 125.0e6 * (1 << 30) + 0.5);

      if (c25_rx2_bpf_i2c_present)
      {
        c25_switch_rx_bpf(C25_RX2_BPF_ADDR, c25_rx2_freq);
      }
      break;
#endif  

#ifndef CHARLY25AB
      freq = ntohl(*(uint32_t *)(frame + 1));
      if(freq_data[2] != freq)
      {
        freq_data[2] = freq;
        alex_write();
        if(i2c_misc) misc_write();
      }
      if(freq < freq_min || freq > freq_max) break;
      *rx_freq[1] = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);
      break;
#endif
      
    case 8:  // C0: Bit 1-4 - Receiver 3 - C0: Bit 0 - MOX -> 0 = inactive
    case 9:  // C0: Bit 1-4 - Receiver 3 - C0: Bit 0 - MOX -> 1 = active
      /* set rx phase increment */
      freq = ntohl(*(uint32_t *)(frame + 1));
      if(freq < freq_min || freq > freq_max) break;
      *rx_freq[2] = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);
      break;
      
    case 10:  // C0: Bit 1-4 - Receiver 4 - C0: Bit 0 - MOX -> 0 = inactive
    case 11:  // C0: Bit 1-4 - Receiver 4 - C0: Bit 0 - MOX -> 1 = active
      /* set rx phase increment */
      freq = ntohl(*(uint32_t *)(frame + 1));
      if(freq < freq_min || freq > freq_max) break;
      *rx_freq[3] = (uint32_t)floor(freq / 125.0e6 * (1 << 30) + 0.5);
      break;
      
    case 18:
    case 19:

#ifndef CHARLY25AB
      data = (frame[2] & 0x40) << 9 | frame[4] << 8 | frame[3];
      if(alex_data_1 != data)
      {
        alex_data_1 = data;
        alex_write();
      }

      if(i2c_misc)
      {
        data = (frame[3] & 0x80) >> 6 | (frame[3] & 0x20) >> 5;
        if(misc_data_0 != data)
        {
          misc_data_0 = data;
          misc_write();
        }
      }

      /* configure ALEX */
      if(i2c_alex)
      {
        data = frame[4] << 8 | frame[3];
        if(i2c_alex_data != data)
        {
          i2c_alex_data = data;
          ioctl(i2c_fd, I2C_SLAVE, ADDR_ALEX);
          i2c_write_addr_data16(i2c_fd, 0x02, data);
        }
      }

      /* configure level */
      data = frame[1];
      if(i2c_level)
      {
        if(i2c_level_data != data)
        {
          i2c_level_data = data;
          ioctl(i2c_fd, I2C_SLAVE, ADDR_LEVEL);
          i2c_write_addr_data16(i2c_fd, 0x02, data);
        }
      }
      else if(i2c_drive)
      {
        if(i2c_drive_data != data)
        {
          i2c_drive_data = data;
          ioctl(i2c_fd, I2C_SLAVE, ADDR_DRIVE);
          i2c_write_addr_data16(i2c_fd, 0xa9, data << 8 | data);
        }
      }
      else
      {
        *tx_level = (data + 1) * 128 - 1;
      }
#endif

#ifdef CHARLY25AB
      data = frame[1];
      *tx_level = (data + 1) * 128 - 1;
#endif

      /* configure microphone boost */
      if(i2c_codec)
      {
        boost = frame[2] & 0x01;
        if(i2c_boost_data != boost)
        {
          i2c_boost_data = boost;
#ifdef CHARLY25_CODEC
          ioctl(i2c_fd, I2C_SLAVE, C25_CODEC_ADDR);
#else
          ioctl(i2c_fd, I2C_SLAVE, ADDR_CODEC);
#endif

          i2c_write_addr_data8(i2c_fd, 0x08, 0x14 + boost);
        }
      }
      break;
      
    case 20:
    case 21:

#ifndef CHARLY25AB
      rx_att_data = frame[4] & 0x1f;
#endif

      break;

    case 22:
    case 23:

#ifndef CHARLY25AB
      if(i2c_misc)
      {
        data = frame[1] & 0x1f;
        if(misc_data_1 != data)
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

    case 30:
    case 31:
      cw_int_data = frame[1] & 1;
      dac_level_data = frame[2];
      cw_delay = frame[3];
      
      if(i2c_codec)
      {
        data = dac_level_data;
        *dac_level = (data + 1) * 256 - 1;
      }
      break;

    case 32:
    case 33:
      cw_hang = (frame[1] << 2) | (frame[2] & 3);

      if(i2c_codec)
      {
        freq = (frame[3] << 4) | (frame[4] & 255);
        *dac_freq = (uint32_t)floor(freq / 48.0e3 * (1 << 30) + 0.5);
      }
      break;
  }
  
#ifdef DEBUG
  fprintf(stderr, "Frames after switch case statement:\n");
  fprintf(stderr, "Frame[0]: %d\n", frame[0]);
  fprintf(stderr, "Frame[1]: %d\n", frame[1]);
  fprintf(stderr, "Frame[2]: %d\n", frame[2]);
  fprintf(stderr, "Frame[3]: %d\n", frame[3]);
  fprintf(stderr, "Frame[4]: %d\n", frame[4]);
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
  uint8_t id[4] = {0xef, 0xfe, 1, 6};
  uint8_t header[40] =
  {
    127, 127, 127, 0, 0, 33, 17, 21,
    127, 127, 127, 8, 0, 0, 0, 0,
    127, 127, 127, 16, 0, 0, 0, 0,
    127, 127, 127, 24, 0, 0, 0, 0,
    127, 127, 127, 32, 66, 66, 66, 66
  };

  memset(audio, 0, sizeof(audio));
  memset(iovec, 0, sizeof(iovec));
  memset(datagram, 0, sizeof(datagram));

  for(i = 0; i < 25; ++i)
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

  if(i2c_codec)
  {
    /* reset codec ADC fifo */
    *codec_rst |= 1;
    *codec_rst &= ~1;
  }
  
  /* reset rx fifo */
  *rx_rst |= 1;
  *rx_rst &= ~1;

  while(1)
  {
    if(!enable_thread) break;

    size = receivers * 6 + 2;
    n = 504 / size;
    m = 256 / n;

    if(*rx_cntr >= 8192)
    {
      /* reset rx fifo */
      *rx_rst |= 1;
      *rx_rst &= ~1;
    }

    if((i2c_codec && *adc_cntr >= 1024) || *rx_cntr >= 8192)
    {
      if(i2c_codec)
      {
        /* reset codec ADC fifo */
        *codec_rst |= 1;
        *codec_rst &= ~1;
      }

      /* reset rx fifo */
      *rx_rst |= 1;
      *rx_rst &= ~1;
    }

    while(*rx_cntr < m * n * 16) usleep(1000);

    if(i2c_codec && --rate_counter == 0)
    {
      for(i = 0; i < m * n * 2; ++i)
      {
        audio[i] = *adc_data;
      }
      rate_counter = 1 << rate;
      k = 0;
    }

    for(i = 0; i < m * n * 16; i += 8)
    {
      *(uint64_t *)(data0 + i) = *rx_data;
      *(uint64_t *)(data1 + i) = *rx_data;
      *(uint64_t *)(data2 + i) = *rx_data;
      *(uint64_t *)(data3 + i) = *rx_data;
    }

    data_offset = 0;
    for(i = 0; i < m; ++i)
    {
      *(uint32_t *)(buffer + i * 1032 + 4) = htonl(counter);
      ++counter;
    }

    for(i = 0; i < m * 2; ++i)
    {
      pointer = buffer + i * 516 - i % 2 * 4 + 8;
      memcpy(pointer, header + header_offset, 8);
      pointer[3] |= *gpio_in & 7;
      pointer[3] |= tx_mux_data & 0x01;

      if(header_offset == 8)
      {
        value = xadc[153] >> 3;
        pointer[4] = (value >> 8) & 0xff;
        pointer[5] = value & 0xff;
        value = xadc[152] >> 3;
        pointer[6] = (value >> 8) & 0xff;
        pointer[7] = value & 0xff;
      }
      else if(header_offset == 16)
      {
        value = xadc[144] >> 3;
        pointer[4] = (value >> 8) & 0xff;
        pointer[5] = value & 0xff;
        value = xadc[145] >> 3;
        pointer[6] = (value >> 8) & 0xff;
        pointer[7] = value & 0xff;
      }
      header_offset = header_offset >= 32 ? 0 : header_offset + 8;

      pointer += 8;
      memset(pointer, 0, 504);
      for(j = 0; j < n; ++j)
      {
        memcpy(pointer, data0 + data_offset, 6);
        if(size > 8)
        {
          memcpy(pointer + 6, data1 + data_offset, 6);
        }
        if(size > 14)
        {
          memcpy(pointer + 12, data2 + data_offset, 6);
        }
        if(size > 20)
        {
          memcpy(pointer + 18, data3 + data_offset, 6);
        }
        data_offset += 8;
        pointer += size;

        if(i2c_codec) memcpy(pointer - 2, &audio[(k++) >> rate], 2);
      }
    }
    sendmmsg(sock_ep2, datagram, m, 0);
  }
  active_thread = 0;
  return NULL;
}

inline int cw_input()
{
  int input;
  if(!cw_int_data) return 0;
  input = (*gpio_in >> 1) & 3;
  if(cw_reversed) input = (input & 1) << 1 | input >> 1;
  return input;
}

inline void cw_on()
{
  int delay = 1200 / cw_speed;
  if(cw_delay < delay) delay = cw_delay;
  *tx_rst |= 16; /* PTT on */
  tx_mux[16] = 1;
  tx_mux[0] = 2;
  tx_mux_data = 1;
  if(i2c_codec && dac_level_data > 0)
  {
    dac_mux[16] = 1;
    dac_mux[0] = 2;
    dac_mux_data = 1;
    *tx_rst |= 8; /* sidetone on */
  }
  while(delay--)
  {
    usleep(1000);
    cw_memory[0] = cw_input();
    if(cw_mode == 1 && !cw_memory[0]) cw_memory[1] = 0;
    else cw_memory[1] |= cw_memory[0];
  }
  *tx_rst |= 4; /* RF on */

#ifdef DEBUG_CW
  fprintf(stderr, "CW: RF on\n");
#endif
}

inline void cw_off()
{
  int delay = 1200 / cw_speed;
  if(cw_delay < delay) delay = cw_delay;
  if(i2c_codec && dac_mux_data)
  {
    *tx_rst &= ~8; /* sidetone off */
  }
  while(delay--)
  {
    usleep(1000);
    cw_memory[0] = cw_input();
    cw_memory[1] |= cw_memory[0];
  }
  *tx_rst &= ~4; /* RF off */
  cw_ptt_delay = cw_hang > 0 ? cw_hang : 10;
  
#ifdef DEBUG_CW
  fprintf(stderr, "CW: RF off\n");
#endif
}

inline void cw_ptt_off()
{
  if(--cw_ptt_delay > 0) return;
  *tx_rst &= ~16; /* PTT off */
  /* reset tx fifo */
  *tx_rst |= 1;
  *tx_rst &= ~1;
  tx_mux[16] = 0;
  tx_mux[0] = 2;
  tx_mux_data = 0;
  if(i2c_codec && dac_mux_data)
  {
    /* reset codec DAC fifo */
    *tx_rst |= 2;
    *tx_rst &= ~2;
    dac_mux[16] = 0;
    dac_mux[0] = 2;
    dac_mux_data = 0;
  }
  
#ifdef DEBUG_CW
  fprintf(stderr, "CW: PTT off ------\n");
#endif
}

inline void cw_signal_delay(int code)
{
  int delay = code ? 1200 / cw_speed : 3600 * cw_weight / (50 * cw_speed);
  delay -= cw_delay;
  if(delay < 0) delay = 0;
  while(delay--)
  {
    usleep(1000);
    cw_memory[0] = cw_input();
    if(cw_mode == 1 && !cw_memory[0]) cw_memory[1] = 0;
    else cw_memory[1] |= cw_memory[0];
  }
}

inline void cw_space_delay(int code)
{
  int delay = code ? 1200 / cw_speed - cw_delay : 2400 / cw_speed;
  if(delay < 0) delay = 0;
  while(delay--)
  {
    usleep(1000);
    if(tx_mux_data) cw_ptt_off();
    cw_memory[0] = cw_input();
    cw_memory[1] |= cw_memory[0];
  }
}

void *handler_keyer(void *arg)
{
  int state, delay;

  while(1)
  {
    usleep(1000);
    if(tx_mux_data) cw_ptt_off();
    if(!(cw_memory[0] = cw_input())) continue;

    if(cw_mode == 0)
    {
      if(cw_memory[0] & 1)
      {
        cw_on();
        while(cw_memory[0] & 1)
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
        if(delay > 0) usleep(delay * 1000);
        cw_off();
        cw_space_delay(1);
      }
    }
    else
    {
      state = 1;
      cw_memory[1] = cw_memory[0];
      while(1)
      {
        if(cw_memory[1] & (1 << state))
        {
          cw_memory[1] = 0;
          cw_on();
          cw_signal_delay(state);
          cw_off();
          cw_space_delay(1);
          cw_memory[1] &= ~(1 << state);
          cw_memory[1] |= cw_memory[0];
        }
        if(cw_memory[1])
        {
          state ^= 1;
        }
        else
        {
          if(cw_spacing)
          {
            state = 1;
            cw_space_delay(0);
            if(cw_memory[1]) continue;
          }
          break;
        }
      }
    }
  }
  return NULL;
}
