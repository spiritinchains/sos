
all: kernel iso

.PHONY: all kernel clean iso run debug

#run: all
#	qemu-system-x86_64 -cdrom os.iso

kernel:
	make -C kernel

iso: kernel
	cp kernel/bin/kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o os.iso -d /usr/lib/grub/i386-pc iso

clean:
	rm -f os.iso
	make clean -C kernel

run: iso
	qemu-system-i386 -cdrom os.iso -serial stdio

debug: iso
	qemu-system-i386 -cdrom os.iso -serial stdio -S
