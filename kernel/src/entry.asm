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

    ; framebuffer tag
    fb_tag_start:
    dw 5    ; type
    dw 0    ; flags
    dd 20   ; size
    dd 0    ; width
    dd 0    ; height
    dd 0    ; depth
    fb_tag_end:

    align 8
    
    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .entry
global start

extern kmain
extern _kern_end_load

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

    ; move multiboot structs to safe location
    mov edi, 0
    .loop1:
    mov dl, [ebx+edi]
    mov [multiboot_data+edi], dl
    inc edi
    cmp edi, [ebx]                  ; multiboot data size
    jl .loop1
    lea ebx, [multiboot_data]

    ;; paging

    ; map page directory entries to page tables in kernelspace
    mov ecx, kernel_page_tables
    mov edi, kernel_page_dir + (768*4)
    .loop2:
    mov edx, ecx
    or edx, 7
    mov [edi], edx
    add edi, 4
    add ecx, 4096
    cmp ecx, kernel_page_tables_end
    jl .loop2

    ; set up page tables
    mov ecx, 0x400003               ; pte value
    mov edi, kernel_page_tables
    .loop3:
    mov [edi], ecx
    add edi, 4
    add ecx, 4096
    cmp ecx, _kern_end_load
    jl .loop3

    ; set up registers
    mov ecx, kernel_page_dir        ; set page directory address
    mov cr3, ecx

    mov ecx, cr4
    or ecx, (1 << 4)                ; set CR4.PSE (Page size extension)
    ; or ecx, (1 << 5)                ; set CR4.PAE (32-bit paging)
    mov cr4, ecx

    mov ecx, cr0
    or ecx, (1 << 31)               ; enable paging (set CR0.PG)
    ; or ecx, (1 << 16)               ; enable write protection (set CR0.WP)
    mov cr0, ecx

    mov esp, stack_top              ; set stack pointer
    push ebx                        ; addr
    push eax                        ; magic
    call kmain                      ; call entry point
    ;int 0x21
    jmp $                           ; infinite loop

section .kdata

align 4096

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

align 4096

kernel_page_dir:
    dd 0x00000087               ; identity map first 4MB
    ; user pages
    times 767 dd 0              ; pad until entry 768 (0xC00xxxx)
    ; kernel pages
    dd 0x00400087               ; map 2nd 4MB to 0xC0000000
    times 254 dd 0              ; pad until last entry
    ; recursive map
    dd kernel_page_dir + 7

kernel_page_tables:
resb 255 * 4096                 ; 255 kernel-space pages pre-allocated
kernel_page_tables_end:

multiboot_data:
resb 8192                       ; size reserved for multiboot tags

section .stack

; stack memory space
stack_bottom:
    resb 256*1024               ; 256K
stack_top:

