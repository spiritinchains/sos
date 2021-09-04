bits 32

section .multiboot_header

header_start:
    dd 0xe85250d6                   ; magic number
    dd 0                            ; architecture 0 (i386)
    dd header_end - header_start    ; header length
    ; checksum
    dd -(0xe85250d6 + 0 + (header_end - header_start))
    ; optional multiboot tags
    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .text
global start

extern gdt_init
extern main

start:
    cli                             ; clear interrupts
    call gdt_init                   ; set gdt
    mov esp, stack_top              ; set stack pointer
    call main                       ; call entry point
    ;int 0x21
    jmp $                           ; infinite loop

%include "src/gdt.asm"
%include "src/isr.asm"

section .bss

; stack memory space
stack_bottom:
    resb 65536
stack_top:
