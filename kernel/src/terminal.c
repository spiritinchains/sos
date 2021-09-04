
#include <vga.h>

void tty_init() {
	vga_clear();
	vga_setcursor(0);
	vga_enablecursor();
}


void tty_putc(char ch)
{
	// TODO: handle special chars
    int pos;
    vga_getcursor(&pos);

    if (!ch) {
        // null byte case (ignored)
    }
    else if (ch == '\b') {
        //backspace case
        if (pos) {
            vga_setcursor(--pos);
            vga_putc(' ', pos);
        }
    }
    else if (ch == '\n') {
        // newline case (carriage return implied)
        pos += VGA_COLS;
        pos -= pos % VGA_COLS;
        vga_setcursor(pos);
    }
    else {
        // general case
        vga_putc(ch, pos);
        vga_setcursor(++pos);
    }
}
