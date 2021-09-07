
SRC_DIR=src
OBJ_DIR=build

TARGET_DIR=bin

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: bin/kernel.bin

.PHONY: all run clean

build/%.o: src/%.c | $(OBJ_DIR)
	gcc -g -ffreestanding -m32 -Iinclude -c $< -o $@

build/entry.o: src/entry.asm | $(OBJ_DIR)
	nasm -f elf32 $< -o $@

bin/kernel.bin: build/entry.o $(OBJ_FILES) | $(TARGET_DIR)
	ld -m elf_i386 -n -o bin/kernel.bin -T linker.ld build/entry.o $(OBJ_FILES)

$(OBJ_DIR):
	mkdir -p $@

$(TARGET_DIR):
	mkdir -p $@

clean:
	rm -f bin/*.bin
	rm -f build/*.o