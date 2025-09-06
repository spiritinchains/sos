#include "serial.h"
#include "common.h"

void serial_read(const int id, uint8_t data[], const size_t len)
{
    // TODO
}

void serial_write(const int id, const uint8_t data[], const size_t len)
{
    // TODO: make this more modular
    const uint16_t base = 0x3f8;

    for (int i = 0; i < len; i++)
    {
        uint8_t linestat;
        do {
            inb(base + SERIAL_READ_LINE_STATUS, linestat);
            linestat &= 0x20;
        }
        while (!linestat);
        
        outb(base, data[i]);
    }
}
