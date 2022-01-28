
#include <kernel.h>

#include <terminal.h>
#include <keycodes.h>
#include <vga.h>

#define TERM_COLS 80
#define TERM_ROWS 25

static char layout[2][58] = {
    {
    '\0', '\x1b', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 
    ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 
    ',', '.', '/', '\0', '\0', '\0', ' '
    },{
    '\0', '\x1b', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', 
    '<', '>', '?', '\0', '\0', '\0', ' '
    }
};

static int shiftPressed = 0;

void term_init() {
	vga_clear();
	vga_setcursor(0);
	vga_enablecursor();
}

// input keycodes from keyboard input
void term_input(keycode_t kc) {
    // TODO: use a circular buffer
    if (kc == KEY_LSHIFT || kc == KEY_RSHIFT) {
        shiftPressed = 1;
    }
    else if (kc == -KEY_LSHIFT || kc == -KEY_RSHIFT) {
        shiftPressed = 0;
    }

    if (kc > 0 && kc < 58) {
        term_putc(layout[shiftPressed][kc]);
    }
}

void term_putc(char ch)
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
            vga_setchar(' ', pos);
        }
    }
    else if (ch == '\n') {
        // newline case (carriage return implied)
        pos += TERM_COLS;
        pos -= pos % TERM_COLS;
        vga_setcursor(pos);
    }
    else {
        // general case
        vga_setchar(ch, pos);
        vga_setcursor(++pos);
    }
}
