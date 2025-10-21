#pragma once

#include <stdint.h>
#include <stddef.h>

/* Serial Port Register Offsets */

#define SERIAL_REG_READ_DATA            0
#define SERIAL_REG_WRITE_DATA           0
#define SERIAL_REG_WRITE_BAUD_LO        0
#define SERIAL_REG_WRITE_BAUD_HI        1
#define SERIAL_REG_INTR_ENABLE          1
#define SERIAL_REG_READ_INTR_ID         2
#define SERIAL_REG_WRITE_FIFO_CTRL      2
#define SERIAL_REG_LINE_CTRL            3
#define SERIAL_REG_MODEM_CTRL           4
#define SERIAL_REG_READ_LINE_STATUS     5
#define SERIAL_REG_READ_MODEM_STATUS    6
#define SERIAL_REG_SCRATCH              7

/* Serial Port Modem Control Bits */

#define SERIAL_MODEM_CTRL_DTR   0x01
#define SERIAL_MODEM_CTRL_RTS   0x02
#define SERIAL_MODEM_CTRL_IRQ   0x08
#define SERIAL_MODEM_CTRL_LOOP  0x10

/* Serial Port FIFO Bits */

#define SERIAL_FIFO_ENABLE      0x01
#define SERIAL_FIFO_CLEAR_RX    0x02
#define SERIAL_FIFO_CLEAR_TX    0x04
#define SERIAL_FIFO_DMA_SELECT  0x08

/* Serial Port Line Status Bits */

#define SERIAL_LINE_DR      0x01    /* Data Ready */
#define SERIAL_LINE_OE      0x02    /* Overrun Error */
#define SERIAL_LINE_PE      0x04    /* Parity Error */
#define SERIAL_LINE_FE      0x08    /* Framing Error */
#define SERIAL_LINE_BI      0x10    /* Break Indicator */
#define SERIAL_LINE_THRE    0x20    /* Transmitter Holding Register Empty */
#define SERIAL_LINE_TEMT    0x40    /* Transmitter Empty */
#define SERIAL_LINE_IE      0x80    /* Impending Error */

/* Serial Port Modem Status Bits */

#define SERIAL_MODEM_DCTS   0x01    /* Delta Clear To Send */
#define SERIAL_MODEM_DDSR   0x02    /* Delta Data Set Ready */
#define SERIAL_MODEM_TERI   0x04    /* Delta Trailing Edge of Ring Indicator */
#define SERIAL_MODEM_DDCD   0x08    /* Delta Data Carrier Detect */
#define SERIAL_MODEM_CTS    0x10    /* Clear To Send */
#define SERIAL_MODEM_DSR    0x20    /* Data Set Ready */
#define SERIAL_MODEM_RI     0x40    /* Ring Indicator */
#define SERIAL_MODEM_DCD    0x80    /* Data Carrier Detect */

/* Serial Port Interrupts */

#define SERIAL_INTR_DATA    0x01    /* Received Data Available */
#define SERIAL_INTR_THRE    0x02    /* Transmitter Holding Register Empty */
#define SERIAL_INTR_LINE    0x04    /* Receiver Line Status */
#define SERIAL_INTR_MODEM   0x08    /* Modem Status */

typedef enum serial_baud_t
{
    SERIAL_BAUD_115200 = 1,
    SERIAL_BAUD_57600 = 2,
    SERIAL_BAUD_38400 = 3,
    SERIAL_BAUD_28800 = 4,
    SERIAL_BAUD_23040 = 5,
    SERIAL_BAUD_19200 = 6,
    SERIAL_BAUD_14400 = 8,
    SERIAL_BAUD_12800 = 9,
    SERIAL_BAUD_11520 = 10,
    SERIAL_BAUD_9600 = 12,
    SERIAL_BAUD_7680 = 15,
    SERIAL_BAUD_7200 = 16,
    SERIAL_BAUD_6400 = 18,
    SERIAL_BAUD_5760 = 20,
    SERIAL_BAUD_4800 = 24,
    SERIAL_BAUD_4608 = 25,
    SERIAL_BAUD_3840 = 30,
    SERIAL_BAUD_3600 = 32,
    SERIAL_BAUD_3200 = 36,
    SERIAL_BAUD_2880 = 40,
    SERIAL_BAUD_2560 = 45,
    SERIAL_BAUD_2400 = 48,
    SERIAL_BAUD_2304 = 50,
    SERIAL_BAUD_1920 = 60,
    SERIAL_BAUD_1800 = 64,
    SERIAL_BAUD_1600 = 72,
    SERIAL_BAUD_1536 = 75,
    SERIAL_BAUD_1440 = 80,
    SERIAL_BAUD_1280 = 90,
    SERIAL_BAUD_1200 = 96,
    SERIAL_BAUD_1152 = 100,
    SERIAL_BAUD_960 = 120,
    SERIAL_BAUD_900 = 128,
    SERIAL_BAUD_800 = 144,
    SERIAL_BAUD_768 = 150,
    SERIAL_BAUD_720 = 160,
    SERIAL_BAUD_640 = 180,
    SERIAL_BAUD_600 = 192,
    SERIAL_BAUD_576 = 200,
    SERIAL_BAUD_512 = 225,
    SERIAL_BAUD_480 = 240,
    SERIAL_BAUD_450 = 256,
    SERIAL_BAUD_400 = 288,
    SERIAL_BAUD_384 = 300,
    SERIAL_BAUD_360 = 320,
    SERIAL_BAUD_320 = 360,
    SERIAL_BAUD_300 = 384,
    SERIAL_BAUD_288 = 400,
    SERIAL_BAUD_256 = 450,
    SERIAL_BAUD_240 = 480,
    SERIAL_BAUD_225 = 512,
    SERIAL_BAUD_200 = 576,
    SERIAL_BAUD_192 = 600,
    SERIAL_BAUD_180 = 640,
    SERIAL_BAUD_160 = 720,
    SERIAL_BAUD_150 = 768,
    SERIAL_BAUD_144 = 800,
    SERIAL_BAUD_128 = 900,
    SERIAL_BAUD_120 = 960,
    SERIAL_BAUD_100 = 1152,
    SERIAL_BAUD_96 = 1200,
} serial_baud_t;

typedef enum serial_data_bits_t
{
    SERIAL_DATA_BITS_5 = 0,
    SERIAL_DATA_BITS_6 = 1,
    SERIAL_DATA_BITS_7 = 2,
    SERIAL_DATA_BITS_8 = 3,
} serial_data_bits_t;

typedef enum serial_stop_bits_t
{
    SERIAL_STOP_BITS_1 = 0,
    SERIAL_STOP_BITS_2 = 1,
} serial_stop_bits_t;

typedef enum serial_parity_t
{
    SERIAL_PARITY_NONE = 0,
    SERIAL_PARITY_ODD = 1,
    SERIAL_PARITY_EVEN = 3,
    SERIAL_PARITY_MARK = 5,
    SERIAL_PARITY_SPACE = 7,
} serial_parity_t;

typedef enum serial_fifo_level_t
{
    SERIAL_FIFO_LEVEL_1 = 0,
    SERIAL_FIFO_LEVEL_4 = 64,
    SERIAL_FIFO_LEVEL_8 = 128,
    SERIAL_FIFO_LEVEL_14 = 192
} serial_fifo_level_t;

typedef struct serial_line_t
{
    serial_data_bits_t data;
    serial_stop_bits_t stop;
    serial_parity_t parity;
} serial_line_t;

void serial_read(const int id, uint8_t data[], const size_t len);
void serial_write(const int id, const uint8_t data[], const size_t len);

void serial_set_baud(const int id, const serial_baud_t baud);
void serial_set_line(const int id, const serial_line_t format);
void serial_set_interrupts(const int id, const uint8_t flags);
