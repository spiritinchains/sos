
#include <kernel.h>

#include <keyboard.h>
#include <8042ctrl.h>
#include <terminal.h>

#define SC1_KEY_LSHIFT 0x2a
#define SC1_RELEASE 0x80

/*
 * Keyboard TODO:
 * Handle multibyte keycodes
 * Handle other Scan code sets
 * Come up with a standard keycode set
 */

void pic1_ack();

static uint8_t keycode;
static bool shiftPressed = false;

void kb_processKeycode()
{
    // set shiftPressed when keycode is LSHIFT, do nothing otherwise
    shiftPressed |= (keycode == SC1_KEY_LSHIFT);
    // unset shiftPressed when keycode is LSHIFT Released, do nothing otherwise
    shiftPressed &= (keycode != (SC1_KEY_LSHIFT + SC1_RELEASE));
    
    // stopgap solution before real keycodes are implemented
    if ((keycode > 0 && keycode < 58)) {
        // vga_putc(layout_enUS[shiftPressed][keycode]);
        term_input(keycode + (shiftPressed * 58));
    }
}

// gets called upon every IRQ1
void kb_irq()
{
    ps2ctrl_readb(&keycode);
    kb_processKeycode();
    pic1_ack();
}
