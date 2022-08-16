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
    dd 1, 4, 6, 7, 8
    reqinfo_tag_end:

    align 8

    ; ; framebuffer tag
    ; fb_tag_start:
    ; dw 5    ; type
    ; dw 0    ; flags
    ; dd 20   ; size
    ; dd 0    ; width
    ; dd 0    ; height
    ; dd 0    ; depth
    ; fb_tag_end:

    align 8
    
    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .entry
global start

extern main

start:
    cli                             ; clear interrupts
    lgdt [gdt_descriptor]
    mov ecx, cr0
    or ecx, 1                       ; enable protected mode
    mov cr0, ecx
    jmp 0x08:.far_jump              ; far jump to flush cs
    .far_jump:

    ; properly set data segments
    mov cx, 0x10
    mov ds, cx
    mov ss, cx
    mov es, cx

    ; ; paging
    ; mov ecx, pd_start               ; set page directory address
    ; mov cr3, ecx
    ; mov ecx, pt1_start              ; point first page directory to first page table
    ; or ecx, 7                       ; set flags
    ; mov [pd_start], ecx
    ; mov ecx, pt1_start              ; point 0x300th page directory to second page table (0xC00...)
    ; or ecx, 7
    ; mov [pd_start+0xC00], ecx
    ; ; 0xC0000000-0xC03FFFFF is a mirror of 0x00000000-0x003FFFFF

    ; mov ecx, cr0
    ; or ecx, (1 << 31)               ; enable paging (set CR0.PG)
    ; ; or ecx, (1 << 16)               ; enable write protection (set CR0.WP)
    ; mov cr0, ecx
    ; ; mov ecx, cr4
    ; ; or ecx, (1 << 4)                ; set CR4.PSE (Page size extension)
    ; ; or ecx, (1 << 5)                ; set CR4.PAE (32-bit paging)
    ; ; mov cr4, ecx

    mov esp, stack_top              ; set stack pointer
    push ebx                        ; addr
    push eax                        ; magic
    call main                       ; call entry point
    ;int 0x21
    jmp $                           ; infinite loop

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

section .bstack

; stack memory space
stack_bottom:
    resb 16*1024               ; 16K
stack_top:

