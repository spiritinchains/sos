#pragma once

#include <stdint.h>
#include <stddef.h>

/* Serial Port Register Offsets */

#define SERIAL_READ_DATA            0
#define SERIAL_WRITE_DATA           0
#define SERIAL_WRITE_BAUD_LO        0
#define SERIAL_WRITE_BAUD_HI        1
#define SERIAL_INTR_ENABLE          1
#define SERIAL_READ_INTR_ID         2
#define SERIAL_LINE_CTRL            3
#define SERIAL_MODEM_CTRL           4
#define SERIAL_READ_LINE_STATUS     5
#define SERIAL_READ_MODEM_STATUS    6
#define SERIAL_SCRATCH              7

void serial_read(const int id, uint8_t data[], const size_t len);
void serial_write(const int id, const uint8_t data[], const size_t len);
