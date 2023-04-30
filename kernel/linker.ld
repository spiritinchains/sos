ENTRY(start)

_KERN_BASE = 0xC0000000;
_KERN_LOAD_OFFSET = _KERN_BASE - 4M;

SECTIONS {
    . = 1M;
    .boot ALIGN(4K) : {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot)
        *(.entry)
    }

    . = 2M;
    .kdata ALIGN(4K) : {
        _kdata_start = .;
        /* kernel data structures (paging etc) */
        *(.kdata)
        _kdata_end = .;
    }

    . = _KERN_BASE;
    .text ALIGN(4K) : AT(ADDR(.text) - _KERN_LOAD_OFFSET) {
        *(.text)
    }
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - _KERN_LOAD_OFFSET) {
        *(.rodata)
    }
    .data ALIGN(4K) : AT(ADDR(.data) - _KERN_LOAD_OFFSET) {
        *(.data)
    }
    .bss ALIGN(4K) : AT(ADDR(.bss) - _KERN_LOAD_OFFSET) {
        *(.COMMON)
        *(.bss)
        *(.stack)
    }

    . = ALIGN(4K);
    _kern_end = .;
    _kern_end_load = _kern_end - _KERN_LOAD_OFFSET;
}

