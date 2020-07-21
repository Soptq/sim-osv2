# $@ = target file
# $< = first dependency
# $^ = all dependencies
KERNEL_C_SOURCES = $(wildcard kernel/*.c)
KERNEL_HEADERS = $(wildcard kernel/*.h)
DRIVERS_C_SOURCES = $(wildcard drivers/*.c)
DRIVERS_HEADERS = $(wildcard drivers/*.h)

KERNEL_OBJ = $(KERNEL_C_SOURCES:kernel/%.c=build/%.o)
DRIVERS_OBJ = $(DRIVERS_C_SOURCES:drivers/%.c=build/%.o)

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g

all: run

build/kernel.bin: build/kernel_entry.o ${KERNEL_OBJ} ${DRIVERS_OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

build/kernel.elf: build/kernel_entry.o ${KERNEL_OBJ} ${DRIVERS_OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

kernel.dis: build/kernel.bin
	ndisasm -b 32 $< > <@

os-image.bin: build/boot-sector.bin build/kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

debug: os-image.bin build/kernel.elf
	qemu-system-i386 -s -S -fda $< &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file build/kernel.elf"

build/%.o: kernel/%.c
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

build/%.o: drivers/%.c
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

build/%.o: boot/%.asm
	nasm $< -f elf -o $@

build/%.bin: boot/%.asm
	nasm $< -f bin -o $@

clean:
	rm build/*
