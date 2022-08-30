
#include <kernel.h>
#include <multiboot2.h>

#include "terminal.h"
#include "serial.h"
#include "vmm.h"

void multiboot_getinfo(void* addr);
void idt_init();

// very primitive interactive prompt for minor stuff
void prompt()
{
	char buf[1025];
	size_t end = 0;
	const char PS[] = "> ";
	printk(PS);
	while (true)
	{
		char x = serial_read();
		if (x >= 32 && x < 127 && end < 1024)
		{
			serial_write(x);
			buf[end] = x;
			end++;
		}
		else if (x == 0xb || x == 0x7f && end > 0)
		{
			end--;
			buf[end] = 0;
			serial_write('\b');
			serial_write(' ');
			serial_write('\b');
		}
		else if (x == 0xd)
		{
			// process input
			buf[end] = 0;
			end = 0;
			printk("\n");
			if (kstrncmp(buf, "ping", 4) == 0)
			{
				printk("pong\n");
			}
			printk(PS);
		}
	}
}

int kmain(uint32_t magic, void* addr) 
{
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
		return -1;

	multiboot_getinfo(addr);

	extern uintptr_t _kern_end, _kdata_start, _kdata_end;

	idt_init();
	serial_init();

	printk("\n\nmain at 0x%x\n", &kmain);
	printk("addr: 0x%x, size: %d\n", addr, *(uint32_t*)addr);
	// printk("framebuffer at 0x%x\n", fb);
	printk("Start of Kernel Data: 0x%x\n", &_kdata_start);
	printk("End of Kernel Data: 0x%x\n", &_kdata_end);
	printk("End of Kernel: 0x%x\n", &_kern_end);

	// printk("\nmemory:\n");
	// print_mmap_info(tag_mmap);
	// void* a = kmalloc(144);
	// kfree(a);

	vmm_init();

	printk("OK\n");

	// this causes a page fault
	//*(uint32_t*)0xdeadc0de = 69;

	prompt();

	return 0;
}
