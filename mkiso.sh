#!/usr/bin/env bash

mkdir -p dist

# Create the bootable ISO.
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso -o dist/sos.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
boot/limine/limine bios-install dist/sos.iso
