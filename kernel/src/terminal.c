
#include <kernel.h>

#include "terminal.h"
#include "keycodes.h"
#include "vga.h"

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

// keyboard states
static int shiftPressed = 0;

// terminal display states
static int isEscape = 0;
static vgacolor_t bgcolor = VGA_COLOR_BLACK;
static vgacolor_t fgcolor = VGA_COLOR_DARK_GREY;

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

/* Display character on screen */
void term_putc(char ch)
{
    int pos;
    vga_getcursor(&pos);
    // vga_setbg(bgcolor, pos);
    // vga_setfg(bgcolor, pos);

    switch (ch) {
        case 0:
            // null byte
            break;
        case '\b':
            // backspace
            if (pos) {
                vga_setcursor(--pos);
                vga_setchar(' ', pos);
            }
            break;
        // case '\x1b':
        //     // escape sequence
        //     isEscape = 1;
        //     break;
        case '\n':
            // newline + carriage return
            pos += TERM_COLS;
            pos -= pos % TERM_COLS;
            vga_setcursor(pos);
            break;
        default:
            // general case
            vga_setchar(ch, pos);
            vga_setcursor(++pos);
            break;
    }
}
