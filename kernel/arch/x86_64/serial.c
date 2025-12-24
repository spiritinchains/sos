#include <serial.h>
#include <common.h>

#include <stdbool.h>

static const uint16_t PORT[] = {
    0x3f8,
    0x2f8
};

#define IS_PORT_ID_VALID(id) (((id) >= 0) && ((id) < (int) (sizeof(PORT) / sizeof(uint16_t))))

static bool serial_is_data_available(const uint16_t address)
{
    uint16_t value;
    inb(address + SERIAL_REG_READ_LINE_STATUS, value);
    return (bool) (value & SERIAL_LINE_DR);
}

static bool serial_is_transmit_empty(const uint16_t address)
{
    uint16_t value;
    inb(address + SERIAL_REG_READ_LINE_STATUS, value);
    return (bool) (value & SERIAL_LINE_THRE);
}

static inline uint8_t serial_receive_byte(const uint16_t address)
{
    uint8_t data;
    inb(address, data);
    return data;
}

static inline void serial_transmit_byte(const uint16_t address, const uint8_t data)
{
    outb(address, data);
}

void serial_read(const int id, uint8_t data[], const size_t len)
{
    if (!IS_PORT_ID_VALID(id))
    {
        return;
    }

    for (size_t i = 0; i < len; i++)
    {
        while (!serial_is_data_available(PORT[id])) {}
        data[i] = serial_receive_byte(PORT[id]);
    }
}

void serial_write(const int id, const uint8_t data[], const size_t len)
{
    if (!IS_PORT_ID_VALID(id))
    {
        return;
    }

    for (size_t i = 0; i < len; i++)
    {
        while (!serial_is_transmit_empty(PORT[id])) {}
        serial_transmit_byte(PORT[id], data[i]);
    }
}

void serial_set_baud(const int id, const serial_baud_t baud)
{
    // TODO
}

void serial_set_line(const int id, const serial_line_t format)
{
    // TODO
}

void serial_set_interrupts(const int id, const uint8_t flags)
{
    // TODO
}