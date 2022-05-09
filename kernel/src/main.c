
#include <kernel.h>
#include <multiboot2.h>
#include <terminal.h>
#include <serial.h>

void idt_init();

struct multiboot_tag_mmap*		tag_mmap = NULL;
struct multiboot_tag_string*	tag_marg = NULL;

void multiboot_getinfo(void* addr)
{
	size_t mbi_size = *(uint32_t*)addr;
	size_t i = 8;
	while (i < mbi_size) {
		struct multiboot_tag* curtag = addr + i;

		// NOTE: I have no clue if the pointer assignments will work gracefully
		switch (curtag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				tag_marg = curtag;
				break;
			case MULTIBOOT_TAG_TYPE_MMAP:
				tag_mmap = curtag;
				print_mmap_info(tag_mmap);
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

void print_mmap_info(struct multiboot_tag_mmap* mmap)
{
	// printk("entries: 0x%x\n", mmap->entries);
	printk("entry_size: %d\n", mmap->entry_size);
	printk("entry_version: %d\n", mmap->entry_version);
	printk("size: %d\n", mmap->size);
	printk("type: %d\n", mmap->type);
	for (int i = 0; i < mmap->size / mmap->entry_size; i++)
	{
		print_mmap_entry(&mmap->entries[i]);
	}
}

void print_mmap_entry(struct multiboot_mmap_entry* entry)
{
	// skip non-RAM entries
	if (entry->type != 1) return;
	printk("0x%x, Length %d\n", entry->addr, entry->len);
}

int main(uint32_t magic, void* addr) 
{
	idt_init();
	term_init();
	serial_init();
	multiboot_getinfo(addr);

	printk("\n\nmain at 0x%x\n", &main);
	// printk("addr: %x, magic: %x\n", addr, magic);


	printk("OK");

	return 0;
}
