
#include <kernel.h>
#include <terminal.h>
#include <serial.h>
#include <multiboot2.h>

void idt_init();

int main(uint32_t magic, void* addr) {
    idt_init();
	term_init();
    serial_init();

    size_t mbi_size = *(uint32_t*)addr;
    printk("Multiboot information address: 0x%x\n", addr);
    printk("Information size = %d bytes\n", mbi_size);

    // print line to serial
    const char x[] = "Heya";
    int i = 0;
    while (x[i]) { serial_write(x[i]); i++; }

    return 0;
}
