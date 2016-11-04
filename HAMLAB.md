# Original pinout defined by Pavel Demin

| function          | XDC/TCL name    | FPGA | E1 conn. |  / |  / | E1 conn. | FPGA | XDC/TCL name                  | function  |
|-------------------|-----------------|------|----------|----|----|----------|------|-------------------------------|-----------|
|                   |                 |      | 3.3V     |  1 |  2 | 3.3V     |      |                               |           |
| o PTT             | exp_p_tri_io[0] | G17  | ext_p[0] |  3 |  4 | ext_n[0] | G18  | exp_n_tri_io[0]               | i PTT     |
| o Preamp          | exp_p_tri_io[1] | H16  | ext_p[1] |  5 |  6 | ext_n[1] | H17  | exp_n_tri_io[1]               | i DASH    |
| o Atenuator 10 dB | exp_p_tri_io[2] | J18  | ext_p[2] |  7 |  8 | ext_n[2] | H18  | exp_n_tri_io[2]               | i DOT     |
| o Atenuator 20 dB | exp_p_tri_io[3] | K17  | ext_p[3] |  9 | 10 | ext_n[3] | K18  | exp_n_tri_io[3]               | i GPS PPS |
| o Hermes CTRL 1   | exp_p_tri_io[4] | L14  | ext_p[4] | 11 | 12 | ext_n[4] | L15  | exp_n_tri_io[4] exp_n_alex[0] | i BCLK    |
| o Hermes CTRL 2   | exp_p_tri_io[5] | L16  | ext_p[5] | 13 | 14 | ext_n[5] | L17  | exp_n_tri_io[5] exp_n_alex[1] | i ADCDAT  |
| o Hermes CTRL 3   | exp_p_tri_io[6] | K16  | ext_p[6] | 15 | 16 | ext_n[6] | J16  | exp_n_tri_io[6] exp_n_alex[2] | o DACDAT  |
| o Hermes CTRL 4   | exp_p_tri_io[7] | M14  | ext_p[7] | 17 | 18 | ext_n[7] | M15  | exp_n_tri_io[7] exp_n_alex[3] | i ADCLRC  |
|                   |                 |      | NC       | 19 | 20 | NC       |      |                               |           |
|                   |                 |      | NC       | 21 | 22 | NC       |      |                               |           |
|                   |                 |      | NC       | 13 | 24 | NC       |      |                               |           |
|                   |                 |      | GND      | 25 | 26 | GND      |      |                               |           |

# Desired pinout for HAMLAB


| FPGA | E1 conn. |  / |  / | E1 conn. | FPGA | XDC/TCL name    | HAMLAB    |
|------|----------|----|----|----------|------|-----------------|-----------|
|      | 3.3V     |  1 |  2 | 3.3V     |      |                 |           |
| G17  | ext_p[0] |  3 |  4 | ext_n[0] | G18  | exp_p_tri_io[0] | o PPT out |
| H16  | ext_p[1] |  5 |  6 | ext_n[1] | H17  | exp_n_tri_io[0] | i PPT in  |
| J18  | ext_p[2] |  7 |  8 | ext_n[2] | H18  | exp_n_tri_io[2] | i DASH    |
| K17  | ext_p[3] |  9 | 10 | ext_n[3] | K18  | exp_n_tri_io[1] | i DOT     |
| L14  | ext_p[4] | 11 | 12 | ext_n[4] | L15  | exp_n_alex[0]   | i BCLK    |
| L16  | ext_p[5] | 13 | 14 | ext_n[5] | L17  | exp_n_alex[1]   | i ADCDAT  |
| K16  | ext_p[6] | 15 | 16 | ext_n[6] | J16  | exp_n_alex[2]   | o DACDAT  |
| M14  | ext_p[7] | 17 | 18 | ext_n[7] | M15  | exp_n_alex[3]   | i ADCLRC  |
|      | NC       | 19 | 20 | NC       |      |                 |           |
|      | NC       | 21 | 22 | NC       |      |                 |           |
|      | NC       | 13 | 24 | NC       |      |                 |           |
|      | GND      | 25 | 26 | GND      |      |                 |           |
