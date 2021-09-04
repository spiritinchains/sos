
#include <kernel.h>

#include <keyboard.h>
#include <8042ctrl.h>
#include <terminal.h>

/*
 * Keyboard TODO:
 * Handle multibyte keycodes
 */

void pic1_ack();


static uint8_t keycode;
static bool shiftPressed = false;

// this part of the code probs doesn't belong here

static char layout_enUS[2][58] = {
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

void kb_processKeycode()
{
    // set shiftPressed when keycode is LSHIFT, do nothing otherwise
    shiftPressed |= (keycode == SC1_KEY_LSHIFT);
    // unset shiftPressed when keycode is LSHIFT Released, do nothing otherwise
    shiftPressed &= (keycode != (SC1_KEY_LSHIFT + SC1_RELEASE));
    
    if ((keycode > 0 && keycode < 58)) {
        // vga_putc(layout_enUS[shiftPressed][keycode]);
        tty_putc(layout_enUS[shiftPressed][keycode]);
    }
}

// gets called upon every IRQ1
void kb_irq()
{
    ps2ctrl_readb(&keycode);
    kb_processKeycode();
    pic1_ack();
}
