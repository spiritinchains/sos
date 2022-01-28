bits 32

section .multiboot

align 8

header_start:
    dd 0xe85250d6                   ; magic number
    dd 0                            ; architecture 0 (i386)
    dd header_end - header_start    ; header length
    
    ; checksum
    dd -(0xe85250d6 + 0 + (header_end - header_start))
    
    ; request info tag
    reqinfo_tag_start:
    dw 1    ; type
    dw 0    ; flags
    dd reqinfo_tag_end - reqinfo_tag_start
    dd 1, 4
    reqinfo_tag_end:
    
    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .text
global start

extern main

start:
    cli                             ; clear interrupts
    lgdt [gdt_descriptor]
    mov ecx, cr0
    or cl, 1                        ; enable protected mode
    ;or eax, (1 << 31)               ; enable paging
    ;or eax, (1 << 16)               ; enable write protection
    mov cr0, ecx
    jmp 0x08:dummylabel             ; far jump to flush cs
    dummylabel:                     ; properly set data segments
    mov cx, 0x10
    mov ds, cx
    mov ss, cx
    mov es, cx
    mov esp, stack_top              ; set stack pointer
    push ebx                        ; addr
    push eax                        ; magic
    call main                       ; call entry point
    ;int 0x21
    jmp $                           ; infinite loop

%include "src/isr.asm"

section .rodata
gdt_start:

    gdt_null:       ; null descriptor
    dd 0x0
    dd 0x0

    gdt_code:       ; code descriptor
    dw 0xffff           ; LIMIT (0-15)
    dw 0x0              ; BASE (0-15)
    db 0x0              ; BASE (16-23)
    db 10011010b        ; P, DPL, S, TYPE
    db 11001111b        ; G, DB, L, AVL, LIMIT (16-19)
    db 0x0              ; BASE (24-31)

    gdt_data:       ; data descriptor
    dw 0xffff           ; LIMIT (0-15)
    dw 0x0              ; BASE (0-15)
    db 0x0              ; BASE (16-23)
    db 10010010b        ; P, DPL, S, TYPE
    db 11001111b        ; G, DB, L, AVL, LIMIT (16-19)
    db 0x0              ; BASE (24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size of gdt
    dd gdt_start                ; address of gdt

section .bss

; stack memory space
stack_bottom:
    resb 65536
stack_top:
