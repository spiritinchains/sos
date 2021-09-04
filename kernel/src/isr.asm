global isr_exception
global isr_dummy
global isr33

extern kb_irq

excmsg db "exception", 0

isr_dummy:
    ; do nothing
    iret

isr_exception:
    ; TODO 
    push eax
    push ebx
    mov ebx, excmsg
    mov ecx, 0
    .loop:
    mov al, [ebx]
    mov [0xb8f00+ecx*2], al
    cmp al, 0
    jne .loop
    iret

isr33:
    ; keyboard IRQ
    call kb_irq
    iret