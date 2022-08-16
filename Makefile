
QEMU=qemu-system-i386
QEMUOPTS=-cdrom os.iso -serial stdio -m 128 -d int,cpu_reset -no-reboot

all: kernel iso

.PHONY: all kernel clean iso run debug

kernel:
	make -C kernel

iso: kernel iso/boot/grub/grub.cfg
	cp kernel/bin/kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o os.iso -d /usr/lib/grub/i386-pc iso

clean:
	rm -f os.iso
	make clean -C kernel

run: iso
	$(QEMU) $(QEMUOPTS) 2> qemu.log

debug: iso
	$(QEMU) $(QEMUOPTS) -s -S 2> qemu.log
