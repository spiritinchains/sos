ENTRY(start)

KERN_BASE = 0xC0000000;

SECTIONS {
    . = 1M;
    .boot ALIGN(4K) : {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot)
        *(.entry)
    }
    .initstack ALIGN(4K) : {
        *(.initstack)
    }
    .pagedata ALIGN(4K) : {
        *(.pagedata)
    }
    .text ALIGN(4K) : {
        *(.text)
    }
    .rodata ALIGN(4K) : {
        *(.rodata)
    }
    .data ALIGN(4K) : {
        *(.data)
    }
    .bss ALIGN(4K) : {
        *(.COMMON)
        *(.bss)
    }
}

