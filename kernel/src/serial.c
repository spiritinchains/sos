
#include <kernel.h>
#include <serial.h>

void serial_init() {
    outb((COM1 + SERIAL_INT_ENABLE), 0x00);
    // set baud rate to 9600 (divisor 12)
    outb((COM1 + SERIAL_LINE_CTRL), SERIAL_DLAB);
    outb((COM1 + 0), 0x0C);
    outb((COM1 + 1), 0x00);
    // 8 bits, no parity, 1 stop bit
    outb((COM1 + SERIAL_LINE_CTRL), 0x03);

    outb((COM1 + SERIAL_INT_ID_FIFO), 0xC7);
    outb((COM1 + SERIAL_MODEM_CTRL), 0x0B);

    // test serial device
    outb((COM1 + SERIAL_MODEM_CTRL), 0x1E);
    outb(COM1, 0xAE);
    uint8_t x;
    inb(COM1, x);
    if (x == 0xAE) {
        // test passed
        outb((COM1 + SERIAL_MODEM_CTRL), 0x0F);
    }
}

uint8_t serial_read() {
    // loop until DR set (Data is Ready)
    uint8_t linestat;
    do {
        inb((COM1 + SERIAL_LINE_STAT), linestat);
        linestat &= 1;
    }
    while (!linestat);

    uint8_t r;
    inb(COM1, r);
    return r;
}

void serial_write(uint8_t d) {
    // loop until THRE set (Data can be sent)
    uint8_t linestat;
    do {
        inb((COM1 + SERIAL_LINE_STAT), linestat);
        linestat &= 0x20;
    }
    while (!linestat);

    outb(COM1, d);
}
