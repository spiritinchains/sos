
#include <kernel.h>

#include <vga.h>

static vgach_t* VGA_BUFFER = (vgach_t*)VGA_BUFFER_ADDR;

void vga_clear() {
    for (int i = 0; i < 25 * 80; i++) {
        VGA_BUFFER[i].data = ' ';
        VGA_BUFFER[i].attr = 0x07;
    }
}

void vga_putc(char data, int index)
{
    VGA_BUFFER[index].data = data;
}

void vga_setbg(vgacolor_t bgcolor, int index)
{
    VGA_BUFFER[index].attr &= 0x0F;
    VGA_BUFFER[index].attr |= (bgcolor << 4);
}

void vga_setfg(vgacolor_t fgcolor, int index)
{
    VGA_BUFFER[index].attr &= 0xF0;
    VGA_BUFFER[index].attr |= fgcolor;
}

void vga_enablecursor()
{
    uint8_t d;
    outb(VGA_CTRL, VGA_REG_CURSOR_START);
    inb(VGA_DATA, d);
    d &= 0xC0;              // disable bit 5 to enable
    outb(VGA_DATA, d);

    outb(VGA_CTRL, VGA_REG_CURSOR_END);
    inb(VGA_DATA, d);
    d &= 0xE0;
    d |= 0x0F;              // set end scanline
    outb(VGA_DATA, d);
}

void vga_disablecursor()
{
    uint8_t d;
    outb(VGA_CTRL, VGA_REG_CURSOR_START);
    inb(VGA_DATA, d);
    d |= 0x20;              // set bit 5 to disable
    outb(VGA_DATA, d);
}

void vga_getcursor(int* pos)
{
    uint8_t offsetHigh, offsetLow;
    // get high and low bytes of cursor's 16-bit index
    outb(VGA_CTRL, VGA_REG_CURSOR_HI);     // cursor high byte
    inb(VGA_DATA, offsetHigh);
    outb(VGA_CTRL, VGA_REG_CURSOR_LO);     // cursor low byte
    inb(VGA_DATA, offsetLow);

    *pos = (offsetHigh << 8) + offsetLow;
}

void vga_setcursor(int pos)
{
    uint8_t offsetHigh, offsetLow;
    offsetHigh = (pos >> 8) & 0xFF;
    offsetLow = pos & 0xFF;
    outb(VGA_CTRL, VGA_REG_CURSOR_HI);
    outb(VGA_DATA, offsetHigh);    
    outb(VGA_CTRL, VGA_REG_CURSOR_LO);
    outb(VGA_DATA, offsetLow);
}
