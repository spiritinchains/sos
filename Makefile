
all: boot kernel iso

boot:
	make -C boot

kernel:
	make -C kernel

run: dist/sos.iso
	qemu-system-x86_64 -cdrom dist/sos.iso

clean:
	make -C kernel clean
	make -C boot clean

iso: dist/sos.iso

dist/sos.iso: boot kernel
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/limine
	cp -v kernel/build/kernel.elf iso/boot/
	cp -v boot/limine.conf boot/limine/*.sys boot/limine/*.bin iso/boot/limine/
	mkdir -p iso/EFI/BOOT
	cp -v boot/limine/BOOTX64.EFI iso/EFI/BOOT/
	cp -v boot/limine/BOOTIA32.EFI iso/EFI/BOOT/
	mkdir -p dist
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso -o dist/sos.iso
	boot/limine/limine bios-install dist/sos.iso

.PHONY: all boot kernel iso clean
