#include "common.h"
#include "interrupts.h"

/*
 * x86_64 defines the first 32 interrupts for processor traps, faults and exceptions.
 * These are, in order:
 * 
 * 0: Divide-by-zero (#DE)
 * 1: Debug Exception (#DB)
 * 2: Non-maskable Interrupt
 * 3: Breakpoint (#BP)
 * 4: Overflow (#OF)
 * 5: Bound Range Exceeded (#BR)
 * 6: Undefined Opcode (#UD)
 * 7: Device Not Available/No Math Coprocessor (#NM)
 * 8: Double Fault (#DF)
 * 9: Coprocessor Segment Overrrun - RESERVED
 * 10: Invalid TSS (#TS)
 * 11: Segment Not Present (#NP)
 * 12: Stack-Segment Fault (#SS)
 * 13: General Protection Fault (#GP)
 * 14: Page Fault (#PF)
 * 15: RESERVED
 * 16: x87 FPU Float Error/Math Fault (#MF)
 * 17: Alignment Check (#AC)
 * 18: Machine Check (#MC)
 * 19: SIMD Float Exception (#XM)
 * 20: Virtualization Exception (#VE)
 * 21: Control Protection Exception (#CP)
 * 22-31: Reserved
 * 
 * Additionally, interrupts 32 through 255 are user-defined, and are defined
 * based on particular implementations of the controller.
 */

#define INTR_DIVIDE_BY_ZERO 0
#define INTR_DEBUG_EXCEPTION 1
#define INTR_NON_MASKABLE_INTERRUPT 2
#define INTR_BREAKPOINT 3
#define INTR_OVERFLOW 4
#define INTR_BOUND_RANGE_EXCEEDED 5
#define INTR_UNDEFINED_OPCODE 6
#define INTR_NO_MATH_COPROCESSOR 7
#define INTR_DOUBLE_FAULT 8
#define INTR_COPROCESSOR_SEGMENT_OVERRUN 9
#define INTR_INVALID_TSS 10
#define INTR_SEGMENT_NOT_PRESENT 11
#define INTR_STACK_SEGMENT_FAULT 12
#define INTR_GENERAL_PROTECTION_FAULT 13
#define INTR_PAGE_FAULT 14
#define INTR_FLOAT_ERROR 16
#define INTR_ALIGNMENT_CHECK 17
#define INTR_MACHINE_CHECK 18
#define INTR_SIMD_FLOAT_EXCEPTION 19
#define INTR_VIRTUALIZATION_EXCEPTION 20
#define INTR_CONTROL_PROTECTION_EXCEPTION 21

#define ISR __attribute__((interrupt))

ISR void isr_dummy();

static struct idt_descriptor {
    uint16_t size;
    uint32_t base;
}__attribute__((packed)); IDTR;

struct idt_entry {
    uint16_t offset_l;          // lower 16 bits of code offset
    uint16_t selector;          // Segment selector for destination code segment
    unsigned reserved   : 5;
    unsigned bits_5_7   : 3;    // Always 0 for Interrupt and Trap gates
    unsigned gate_type  : 2;    // 01 - Task Gate, 10 - Interrupt Gate, 11 - Trap Gate
    unsigned bit_10     : 1;    // always 1
    unsigned D          : 1;    // gate size (1 if 32-bit, 0 if 16-bit)
    unsigned bit_12     : 1;    // always 0
    unsigned DPL        : 2;    // Descriptor Privilege Level
    unsigned P          : 1;    // Segment Present
    uint16_t offset_h           // higher 16 bits of code offset
    //                    64 bits total
}__attribute__((packed));

struct interrupt_frame {
    uint32_t EIP;
    uint32_t CS;
    uint32_t EFLAGS;
}__attribute__((packed));

#define IDT_ENTRY_TASK_GATE(isr) \
    (struct idt_entry) {\
        .reserved = 0,\
        .bits_5_7 = 0,\
        .gate_type = 1,\
        .bit_10 = 1,\
        .D = 1,\
        .bit_12 = 0,\
        .DPL = 0,\
        .P = 1,\
        .selector = 0x8,\
        .offset_h = (((uint32_t) isr) >> 16) & 0xFFFF,\
        .offset_l = ((uint32_t) isr) & 0xFFFF,}

#define IDT_ENTRY_INTR_GATE(isr) \
    (struct idt_entry) {\
        .reserved = 0,\
        .bits_5_7 = 0,\
        .gate_type = 2,\
        .bit_10 = 1,\
        .D = 1,\
        .bit_12 = 0,\
        .DPL = 0,\
        .P = 1,\
        .selector = 0x8,\
        .offset_h = (((uint32_t) isr) >> 16) & 0xFFFF,\
        .offset_l = ((uint32_t) isr) & 0xFFFF,}

#define IDT_ENTRY_TRAP_GATE(isr) \
    (struct idt_entry) {\
        .reserved = 0,\
        .bits_5_7 = 0,\
        .gate_type = 3,\
        .bit_10 = 1,\
        .D = 1,\
        .bit_12 = 0,\
        .DPL = 0,\
        .P = 1,\
        .selector = 0x8,\
        .offset_h = (((uint32_t) isr) >> 16) & 0xFFFF,\
        .offset_l = ((uint32_t) isr) & 0xFFFF,}

static void (*s_intr_callback[256])(uint8_t) = {};

static struct idt_entry IDT[256] = {};

void interrupts_init(void)
{
    // Initialize Interrupt Controller

    // Initialize Interrupt Descriptor Table
    IDTR.size = sizeof(IDT);
    __asm__("lidt %0" : : "m"(IDTR));
}

void interrupts_set_callback(uint8_t index, void (*callback)(uint8_t))
{
}

ISR void isr_dummy()
{
    // Do Nothing
}