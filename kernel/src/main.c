
#include <kernel.h>
#include <multiboot2.h>

#include "terminal.h"
#include "serial.h"

void idt_init();

struct multiboot_tag_mmap*			tag_mmap = NULL;
struct multiboot_tag_string*		tag_marg = NULL;
struct multiboot_tag_vbe*			tag_mvbe = NULL;
struct multiboot_tag_framebuffer*	tag_mfbi = NULL;

void multiboot_getinfo(void* addr)
{
	size_t mbi_size = *(uint32_t*)addr;
	size_t i = 8;
	while (i < mbi_size) {
		struct multiboot_tag* curtag = addr + i;

		// NOTE: I have no clue if the pointer assignments will work gracefully
		switch (curtag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				tag_marg = (struct multiboot_tag_string*)curtag;
				break;
			case MULTIBOOT_TAG_TYPE_MMAP:
				tag_mmap = (struct multiboot_tag_mmap*)curtag;
				break;
			case MULTIBOOT_TAG_TYPE_VBE:
				tag_mvbe = (struct multiboot_tag_vbe*)curtag;
				break;
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				tag_mfbi = (struct multiboot_tag_framebuffer*)curtag;
				break;
			case MULTIBOOT_TAG_TYPE_END:
				break;
			default:
				break;
		}
		// advance offset to next tag (8-byte aligned)
		size_t mod = curtag->size % 8;
		i += curtag->size - mod + (mod ? 8 : 0);
		continue;
	}
}

void print_mmap_entry(struct multiboot_mmap_entry* entry)
{
	printk("Range: 0x%x-0x%x\n", entry->addr, entry->addr + entry->len);
	
	printk("Length: %d\n", entry->len);

	printk("Type: %d ", entry->type);
	switch (entry->type)
	{
	case MULTIBOOT_MEMORY_AVAILABLE:
		printk("(RAM)");
		break;
	case MULTIBOOT_MEMORY_RESERVED:
		printk("(Reserved)");
		break;
	case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
		printk("(ACPI)");
		break;
	case MULTIBOOT_MEMORY_NVS:
		printk("(NVS)");
		break;
	case MULTIBOOT_MEMORY_BADRAM:
		printk("(BAD RAM)");
		break;
	
	default:
		break;
	}
	printk("\n");
}

void print_mmap_info(struct multiboot_tag_mmap* mmap)
{
	printk("entries: 0x%x\n", mmap->entries);
	printk("entry_size: %d\n", mmap->entry_size);
	printk("entry_version: %d\n", mmap->entry_version);
	printk("size: %d\n", mmap->size);
	printk("type: %d\n\n", mmap->type);

	for (int i = 0; i < mmap->size / mmap->entry_size; i++)
	{
		printk("Block %d\n", i+1);
		print_mmap_entry(&(mmap->entries[i]));
		printk("\n");
	}
}

int main(uint32_t magic, void* addr) 
{
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
		return -1;

	multiboot_getinfo(addr);

	extern uintptr_t _KERNEL_END;

	idt_init();
	serial_init();

	// uint32_t* fb = (uint32_t*)tag_mfbi->common.framebuffer_addr;

	printk("\n\nmain at 0x%x\n", &main);
	printk("addr: %x, magic: %x\n", addr, magic);
	// printk("framebuffer at 0x%x\n", fb);
	printk("\nmemory:\n");
	print_mmap_info(tag_mmap);

	printk("End of Kernel: 0x%x\n", &_KERNEL_END);
	printk("OK\n");

	return 0;
}
