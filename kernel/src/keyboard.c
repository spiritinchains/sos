
#include <kernel.h>

#include <keyboard.h>
#include <terminal.h>

#define PS2CTRL_DATA        0x60        // PS2 Controller Data Port
#define PS2CTRL_STAT_CMD    0x64        // Status/Command Register

#define KB_RES_SELF_TEST_PASS 0xAA
#define KB_RES_ECHO 0xEE
#define KB_RES_ACK 0xFA
#define KB_RES_RESEND 0xFE

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

// get input from keyboard
void kb_read(char* data) {
    char d;
    struct ps2ctrl_status x;
    kb_status(&x);
    if (x.isOutputBufferFull) {
        inb(PS2CTRL_DATA, d)
    }
    *data = d;
}

// send data to keyboard
void kb_send(char cmdbyte) {
    outb(PS2CTRL_STAT_CMD, cmdbyte);
}

// get keyboard status
void kb_status(struct ps2ctrl_status* stat) {
    char statb;
    inb(PS2CTRL_STAT_CMD, statb);
    // unholy pointer hack because C was being pedantic about struct casting
    *stat = *(struct ps2ctrl_status*)&statb;
}

// process input keycode
void kb_proc()
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
    kb_read(&keycode);
    kb_proc();
    pic1_ack();
}
