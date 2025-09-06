ARCH=x86_64
PREFIX=x86_64-elf
CC=$(PREFIX)-gcc
AS=$(PREFIX)-as
LD=$(PREFIX)-ld

CFLAGS=-ffreestanding -mgeneral-regs-only -Wall -Werror
LFLAGS=-nostdlib -ffreestanding -lgcc

SRC_FILES=$(wildcard common/*.c) arch/$(ARCH)/serial.c
OBJ_FILES=$(patsubst %.c, build/%.o, $(SRC_FILES))

OUTPUT_FILE=build/kernel.elf

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(OBJ_FILES) linker.ld | build
	$(CC) -T linker.ld $(OBJ_FILES) -o $(OUTPUT_FILE) $(LFLAGS)

build/%.o: %.c | build
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I. -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build/*

.PHONY: all clean