
SRC_DIR=src
OBJ_DIR=build

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: bin/kernel.bin

.PHONY: all run clean

build/%.o: src/%.c
	gcc -g -ffreestanding -m32 -Iinclude -c $< -o $@

build/entry.o: src/entry.asm
	nasm -f elf32 $< -o $@

bin/kernel.bin: build/entry.o $(OBJ_FILES)
	ld -m elf_i386 -n -o bin/kernel.bin -T linker.ld build/entry.o $(OBJ_FILES)

clean:
	rm -f bin/*.bin
	rm -f build/*.o