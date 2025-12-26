ARCH=x86_64
ARCH_DIR=arch/$(ARCH)
PREFIX=x86_64-elf

CC=$(PREFIX)-gcc
AS=$(PREFIX)-as
LD=$(PREFIX)-ld

CFLAGS=-ffreestanding -fno-pic -mgeneral-regs-only -mcmodel=kernel -mno-red-zone \
       -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function \
	   -DKERNEL_ARCH="$(ARCH)"
LFLAGS=-nostdlib -ffreestanding -lgcc

SRC_FILES=$(wildcard common/*.c) $(wildcard lib/*.c) $(wildcard $(ARCH_DIR)/*.c)
OBJ_FILES=$(patsubst %.c, build/%.o, $(SRC_FILES))

OUTPUT_FILE=build/kernel.elf

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(OBJ_FILES) linker.ld | build
	$(CC) -T linker.ld $(OBJ_FILES) -o $(OUTPUT_FILE) $(LFLAGS)

build/%.o: %.c | build
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -Iinclude -I$(ARCH_DIR)/include -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build/*

.PHONY: all clean