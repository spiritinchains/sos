
all: kernel

kernel:
	make -C kernel

clean:
	make -C kernel clean

.PHONY: all kernel clean
