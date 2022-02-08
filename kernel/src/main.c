
#include <kernel.h>
#include <multiboot2.h>
#include <terminal.h>
#include <serial.h>

void idt_init();
struct multiboot_tag_mmap*		tag_mmap = NULL;
struct multiboot_tag_string*	tag_marg = NULL;

void multiboot_getinfo(void* addr) {
	size_t mbi_size = *(uint32_t*)addr;
	int i = 8;
	while (i < mbi_size) {
		struct multiboot_tag* curtag = addr + i;

		// NOTE: I have no clue if the pointer assignments will work gracefully
		switch (curtag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				tag_marg = curtag;
				break;
			case MULTIBOOT_TAG_TYPE_MMAP:
				tag_mmap = curtag;
				break;
			case MULTIBOOT_TAG_TYPE_END:
				goto exit;
				break;
			default:
				break;
		}
		// get remaining bytes until next tag (8-byte aligned)
		int rem = curtag->size % 8;
		i += curtag->size - rem + (rem ? 8 : 0);
		continue;
		exit:
		break;
	}
}

int main(uint32_t magic, void* addr) {
	idt_init();
	term_init();
	serial_init();
	multiboot_getinfo(addr);

	printk("OK");

	return 0;
}
