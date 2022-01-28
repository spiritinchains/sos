
#include <kernel.h>

#include <keyboard.h>
#include <keycodes.h>

#include <terminal.h>

/*
 * Keyboard TODO:
 * Handle multibyte keycodes
 * Handle other Scan code sets
 * Come up with a standard keycode set
 */

void pic1_ack();

static uint8_t scancode;
static keycode_t keycode;
static int escapeState = 0;

/*
 * get scan code from keyboard
 */
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

/*
 * get keyboard status
 */
void kb_status(struct ps2ctrl_status* stat) {
    char statb;
    inb(PS2CTRL_STAT_CMD, statb);
    // unholy pointer hack because C was being pedantic about struct casting
    *stat = *(struct ps2ctrl_status*)&statb;
}

/* 
 * process scancode input as keycode
 */
void kb_proc()
{
    // convert from 'break' code to 'make' code
    uint8_t idx = scancode & ~0x80;
    
    if (!escapeState) {
        if (scancode == 0xE0) {
            escapeState = 1;
        }
        else {
            keycode = scanset1[idx];
        }
    }
    else {
        keycode = scanset1_esc[idx];
        escapeState = 0;
    }

    if (keycode) {
        if (scancode & 0x80)
            keycode = -keycode;
        // temporary stopgap before event system implementation
        term_input(keycode);
        keycode = 0;
    }
}

// gets called upon every IRQ1
void kb_irq()
{
    kb_read(&scancode);
    kb_proc();
    pic1_ack();
}
