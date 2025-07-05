
all: boot kernel

boot:
	make -C boot

kernel:
	make -C kernel

run: dist/sos.iso
	qemu-system-x86_64 -cdrom dist/sos.iso

clean:
	make -C kernel clean
	make -C boot clean

dist/sos.iso: boot kernel
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/limine
	cp -v kernel/build/kernel.elf iso/boot/
	cp -v boot/limine.conf boot/limine/*.sys boot/limine/*.bin iso/boot/limine/
	mkdir -p iso/EFI/BOOT
	cp -v boot/limine/BOOTX64.EFI iso/EFI/BOOT/
	cp -v boot/limine/BOOTIA32.EFI iso/EFI/BOOT/
	bash mkiso.sh

.PHONY: all kernel clean
