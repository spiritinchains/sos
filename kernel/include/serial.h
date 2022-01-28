
#pragma once

#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8

// serial port offsets
#define SERIAL_DATA 0
#define SERIAL_INT_ENABLE 1
#define SERIAL_INT_ID_FIFO 2
#define SERIAL_LINE_CTRL 3
#define SERIAL_MODEM_CTRL 4
#define SERIAL_LINE_STAT 5
#define SERIAL_MODEM_STAT 6
#define SERIAL_SCRATCH 7

#define SERIAL_DLAB 0x80

void serial_init();

uint8_t serial_read();
void serial_write(uint8_t);
