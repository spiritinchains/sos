
#include <kernel.h>

#define PIC1_CMD 0x20
#define PIC2_CMD 0xA0
#define PIC1_DAT 0x21
#define PIC2_DAT 0xA1

#define PIC_EOI  0x20

/*
struct idt_entry {
    uint16_t offset_l;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_h;
}__attribute__((packed));
*/

static struct idt_descriptor {
    uint16_t size;
    uint32_t base;
}__attribute__((packed)) IDTR;

struct idt_entry {
    uint16_t offset_l;          // lower 16 bits of code offset
    uint16_t selector;
    unsigned reserved   : 5;
    unsigned bits_5_7   : 3;
    unsigned gate_type  : 2;    // 01 - Task Gate, 10 - Interrupt Gate, 11 - Trap Gate
    unsigned bit_10     : 1;    // always 1
    unsigned D          : 1;    // gate size (1 if 32-bit, 0 if 16-bit)
    unsigned bit_12     : 1;    // always 0
    unsigned DPL        : 2;    // Descriptor Privilege Level
    unsigned P          : 1;    // Segment Present
    uint16_t offset_h           // higher 16 bits of code offset
    //                    64 bits total
}__attribute__((packed));

static struct idt_entry IDT[256];


/* PIC Initialization */
void pic_init() {
    // TODO: Make this part clearer by using #defines for ICWs

    // ICW1
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    // ICW2
    outb(PIC1_DAT, 0x20);
    outb(PIC2_DAT, 0x28);

    // ICW3
    outb(PIC1_DAT, 0x04);
    outb(PIC2_DAT, 0x02);

    // ICW4
    outb(PIC1_DAT, 0x01);
    outb(PIC2_DAT, 0x01);

    // Masking
    outb(PIC1_DAT, 0xFD);
    outb(PIC2_DAT, 0xFF);

    // EOI
    outb(PIC1_CMD, PIC_EOI);
    outb(PIC2_CMD, PIC_EOI);

    asm("sti");
}

/* Acknowledge PIC Interrupt signal */
void pic1_ack() {
    outb(PIC1_CMD, PIC_EOI);
}

extern void isr_dummy();
extern void isr_exception();
extern void isr33();

void setoffset(void* offset, int entry) {
    IDT[entry].offset_l = ((uint32_t)offset) & 0xFFFF;
    IDT[entry].offset_h = ((uint32_t)offset >> 16) & 0xFFFF;
}

void idt_init() {
    // initialize pic
    pic_init();

    // initialize interrupt vectors
    // initialize IRQs to dummies (temporary solution)
    for (int i = 0; i < 256; i++) {
        setoffset(isr_dummy, i);
        IDT[i].reserved = 0;
        IDT[i].bits_5_7 = 0;
        IDT[i].gate_type = 3;
        IDT[i].bit_10 = 1;
        IDT[i].D = 1;
        IDT[i].bit_12 = 0;
        IDT[i].DPL = 0;
        IDT[i].P = 1;
        IDT[i].selector = 0x8;
    }

    for (int i = 0; i < 32; i++) {
        setoffset(isr_exception, i);
    }
    setoffset(isr33, 33);

    
    // initialize descriptor
    IDTR.size = sizeof(IDT);
    IDTR.base = (uint32_t)IDT;

    // load descriptor
    __asm__("lidt %0" : : "m"(IDTR));
}
