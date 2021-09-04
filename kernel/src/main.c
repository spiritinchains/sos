
#include <kernel.h>

#include <terminal.h>

// only included for sanity checking cause I screwed up vga once
#define debug_vga_print		*((int*)0xb8000)=0x07690748

void idt_init();

int main() {
    idt_init();
	tty_init();
    printk("yay I managed to print this text");
}
