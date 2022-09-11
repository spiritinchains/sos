
#include <kernel.h>

#define TIMER_C0_DATA   0x40
#define TIMER_C1_DATA   0x41
#define TIMER_C2_DATA   0x42
#define TIMER_MODE_CMD  0x43

#define TIMER_FREQUENCY 1.193181666666666666

struct timer_cmd {
    unsigned bcd_mode : 1;
    unsigned op_mode  : 3;
    unsigned acc_mode : 2;
    unsigned channel  : 2;
}__attribute__((packed));

enum timer_op_mode {
    TIMER_INT_ON_FINAL_COUNT = 0,
    TIMER_ONESHOT = 1,
    TIMER_RATE_GENERATOR = 2,
    TIMER_RATE_GENERATOR_DUP = 6,
    TIMER_SQ_WAVE_GENERATOR = 3,
    TIMER_SQ_WAVE_GENERATOR_DUP = 7,
    TIMER_SW_STROBE = 4,
    TIMER_HW_STROBE = 5
};

enum timer_acc_mode {
    TIMER_ACC_LATCH_COUNT = 0,
    TIMER_ACC_LOBYTE_ONLY = 1,
    TIMER_ACC_HIBYTE_ONLY = 2,
    TIMER_ACC_LOBYTE_HIBYTE = 3
};

uint64_t timer_ticks = 0;

void pic1_ack();

void timer_init()
{
    struct timer_cmd tm = {
        .bcd_mode = 0,
        .channel = 0,
        .acc_mode = TIMER_ACC_LOBYTE_HIBYTE,
        .op_mode = TIMER_INT_ON_FINAL_COUNT
    };
    outb(TIMER_MODE_CMD, tm);
    // send value for counter register
    outb(TIMER_C0_DATA, 0xFF);          // send low byte
    outb(TIMER_C0_DATA, 0xFF);          // send high byte
}

void timer_irq()
{
    struct timer_cmd tm = {
        .bcd_mode = 0,
        .channel = 0,
        .acc_mode = TIMER_ACC_LOBYTE_HIBYTE,
        .op_mode = TIMER_INT_ON_FINAL_COUNT
    };
    timer_ticks++;
    outb(TIMER_MODE_CMD, tm);
    // send value for counter register
    outb(TIMER_C0_DATA, 0xFF);          // send low byte
    outb(TIMER_C0_DATA, 0xFF);          // send high byte

    pic1_ack();
}

uint16_t get_timer_count()
{
    uint16_t r;
    uint8_t hi, lo;
    inb(TIMER_C0_DATA, lo);
    inb(TIMER_C0_DATA, hi);
    r = (hi << 8) | lo;
}
