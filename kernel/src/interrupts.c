
#include <kernel.h>

#define PIC1_CMD 0x20
#define PIC2_CMD 0xA0
#define PIC1_DAT 0x21
#define PIC2_DAT 0xA1

#define PIC_EOI  0x20

#define ISR __attribute__((interrupt))

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

struct interrupt_frame {
    uint32_t EIP;
    uint32_t CS;
    uint32_t EFLAGS;
}__attribute__((packed));


extern void kb_irq();

// NOTE: handle errors better (blue screen of death for unrecoverables?)
// for now just print text
static char excmsg[32][8] = {
    "#DE\n",
    "#DB\n",
    "#02\n",
    "#BP\n",
    "#OF\n",
    "#BR\n",
    "#UD\n",
    "#NM\n",
    "#DF\n",
    "#09\n",
    "#TS\n",
    "#NP\n",
    "#SS\n",
    "#GP\n",
    "#PF\n",
    "#15\n",
    "#MF\n",
    "#AC\n",
    "#MC\n",
    "#XM\n",
    "#VE\n",
    "#CP\n",
    "#22\n",
    "#23\n",
    "#24\n",
    "#25\n",
    "#26\n",
    "#27\n",
    "#HV\n",
    "#VC\n",
    "#SX\n",
    "#31\n"
};

ISR void
isr_dummy(struct interrupt_frame* frame)
{
    // do nothing
}

ISR void
isr0(struct interrupt_frame* frame)
{
    // Divide-by-zero error #DE
    printk(excmsg[0]);
}

ISR void
isr1(struct interrupt_frame* frame)
{
    // Debug #DB
    printk(excmsg[1]);
}

ISR void
isr2(struct interrupt_frame* frame)
{
    // Non-maskable interrupt
    printk(excmsg[2]);
}

ISR void
isr3(struct interrupt_frame* frame)
{
    // Breakpoint #BP
    printk(excmsg[3]);
}

ISR void
isr4(struct interrupt_frame* frame)
{
    // Overflow #OF
    printk(excmsg[4]);
}

ISR void
isr5(struct interrupt_frame* frame)
{
    // Bound range exceeded #BR
    printk(excmsg[5]);
}

ISR void
isr6(struct interrupt_frame* frame)
{
    // Invalid opcode #UD
    printk(excmsg[6]);
}

ISR void
isr7(struct interrupt_frame* frame)
{
    // Device not available #NM
    printk(excmsg[7]);
}

ISR void
isr8(struct interrupt_frame* frame, uint32_t error_code)
{
    // Double fault #DF
    printk(excmsg[8]);
}

ISR void
isr9(struct interrupt_frame* frame)
{
    // Coprocessor segment overrun 
    printk(excmsg[9]);
}

ISR void
isr10(struct interrupt_frame* frame, uint32_t error_code)
{
    // Invalid TSS #TS
    printk(excmsg[10]);
}

ISR void
isr11(struct interrupt_frame* frame, uint32_t error_code)
{
    // Segment not present #NP
    printk(excmsg[11]);
}

ISR void
isr12(struct interrupt_frame* frame, uint32_t error_code)
{
    // Stack-Segment fault #SS
    printk(excmsg[12]);
}

ISR void
isr13(struct interrupt_frame* frame, uint32_t error_code)
{
    // General protection fault #GP
    printk(excmsg[13]);
}

ISR void
isr14(struct interrupt_frame* frame, uint32_t error_code)
{
    // Page fault #PF
    printk(excmsg[14]);
    printk("Page fault\n");
    printk("EIP: %x\n", frame->EIP);
    printk("Flags: %x\n", error_code);
    uint32_t addr;
    __asm__("mov %%cr3, %0" : "=r" (addr));
    printk("Address %x\n", addr);
}

ISR void
isr15(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[15]);
}

ISR void
isr16(struct interrupt_frame* frame)
{
    // x87 Floating point exception #MF
    printk(excmsg[16]);
}

ISR void
isr17(struct interrupt_frame* frame, uint32_t error_code)
{
    // Alignment check #AC
    printk(excmsg[17]);
}

ISR void
isr18(struct interrupt_frame* frame)
{
    // Machine check #MC
    printk(excmsg[18]);
}

ISR void
isr19(struct interrupt_frame* frame)
{
    // SIMD floating point exception #XM/#XF
    printk(excmsg[19]);
}

ISR void
isr20(struct interrupt_frame* frame)
{
    // Virtualization Exception #VE
    printk(excmsg[20]);
}

ISR void
isr21(struct interrupt_frame* frame, uint32_t error_code)
{
    // Control protection exception #CP
    printk(excmsg[21]);
}

ISR void
isr22(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[22]);
}

ISR void
isr23(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[23]);
}

ISR void
isr24(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[24]);
}

ISR void
isr25(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[25]);
}

ISR void
isr26(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[26]);
}

ISR void
isr27(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[27]);
}

ISR void
isr28(struct interrupt_frame* frame)
{
    // Hypervisor injection exception #HV
    printk(excmsg[28]);
}

ISR void
isr29(struct interrupt_frame* frame, uint32_t error_code)
{
    // VMM communication exception #VC
    printk(excmsg[29]);
}

ISR void
isr30(struct interrupt_frame* frame, uint32_t error_code)
{
    // Security exception #SX
    printk(excmsg[30]);
}

ISR void
isr31(struct interrupt_frame* frame)
{
    // Reserved
    printk(excmsg[31]);
}

ISR void 
isr33(struct interrupt_frame* frame)
{
    kb_irq();
}

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

void setoffset(void* offset, int entry) {
    IDT[entry].offset_l = ((uint32_t)offset) & 0xFFFF;
    IDT[entry].offset_h = ((uint32_t)offset >> 16) & 0xFFFF;
}

void idt_init() {
    // initialize pic
    pic_init();

    // initialize interrupt vectors
    for (int i = 0; i < 256; i++) {
        // initialize IRQs to dummies (temporary solution)
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

    // (yes this is ugly but it works without using macros)
    // cpu exceptions
    setoffset(isr0, 0);
    setoffset(isr1, 1);
    setoffset(isr2, 2);
    setoffset(isr3, 3);
    setoffset(isr4, 4);
    setoffset(isr5, 5);
    setoffset(isr6, 6);
    setoffset(isr7, 7);
    setoffset(isr8, 8);
    setoffset(isr9, 9);
    setoffset(isr10, 10);
    setoffset(isr11, 11);
    setoffset(isr12, 12);
    setoffset(isr13, 13);
    setoffset(isr14, 14);
    setoffset(isr15, 15);
    setoffset(isr16, 16);
    setoffset(isr17, 17);
    setoffset(isr18, 18);
    setoffset(isr19, 19);
    setoffset(isr20, 20);
    setoffset(isr21, 21);
    setoffset(isr22, 22);
    setoffset(isr23, 23);
    setoffset(isr24, 24);
    setoffset(isr25, 25);
    setoffset(isr26, 26);
    setoffset(isr27, 27);
    setoffset(isr28, 28);
    setoffset(isr29, 29);
    setoffset(isr30, 30);
    setoffset(isr31, 31);
    // irqs
    setoffset(isr33, 33);
    
    // initialize descriptor
    IDTR.size = sizeof(IDT);
    IDTR.base = (uint32_t)IDT;

    // load descriptor
    __asm__("lidt %0" : : "m"(IDTR));
}
