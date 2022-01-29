
CC=i686-elf-gcc
AS=nasm
CFLAGS=-g -ffreestanding -m32 -Iinclude
LFLAGS=-nostdlib -ffreestanding -lgcc

SRC_DIR=src
OBJ_DIR=build

TARGET_DIR=bin

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: bin/kernel.bin

.PHONY: all run clean

build/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

build/entry.o: src/entry.asm | $(OBJ_DIR)
	$(AS) -f elf32 $< -o $@

bin/kernel.bin: linker.ld build/entry.o $(OBJ_FILES) | $(TARGET_DIR)
	$(CC) -T linker.ld build/entry.o $(OBJ_FILES) -o bin/kernel.bin $(LFLAGS)

$(OBJ_DIR):
	mkdir -p $@

$(TARGET_DIR):
	mkdir -p $@

clean:
	rm -f bin/*.bin
	rm -f build/*.o