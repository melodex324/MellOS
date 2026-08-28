.PHONY: all limine clean run build

CC = gcc
AS = nasm
LD = ld

C_FLAG = -std=c11 -Wall -Wextra -O2 -pipe \
    -ffreestanding -fno-stack-protector -fno-stack-check \
    -fno-lto -fno-PIC -m64 -march=x86-64 \
    -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel \
	-c \
	-I$(DIR_INC) -I$(DIR_IO) -I$(DIR_GDT) -I$(DIR_IDT) -I$(DIR_PIT) -I$(DIR_UEFI) -I$(DIR_FONT) -I$(DIR_KEYBOARD)

LD_FLAGS = -m elf_x86_64 -nostdlib -static -z max-page-size=0x1000 -T

DIR_BUILD = Build
DIR_KERNEL = kernel
DIR_UEFI = UEFI
DIR_FONT = font
DIR_IO = $(DIR_KERNEL)/io
DIR_GDT = $(DIR_KERNEL)/gdt
DIR_IDT = $(DIR_KERNEL)/idt
DIR_PIT = $(DIR_KERNEL)/pit
DIR_LIB = lib
DIR_KEYBOARD = keyboard

DIR_INC = lib/include

DIR_BIN = $(DIR_BUILD)/bin
DIR_ISO = $(DIR_BUILD)/iso
DIR_BIN_UEFI = $(DIR_BUILD)/uefi

FONT_FILE = iso_02_f16.psf

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1
	$(MAKE) -C limine

all: limine
	mkdir -p Build Build/bin Build/iso Build/uefi
	
	$(AS) -f elf64 $(DIR_KERNEL)/boot.asm -o $(DIR_BIN)/boot.o
	$(AS) -f elf64 $(DIR_GDT)/gdt.asm -o $(DIR_BIN)/gdt_asm.o
	$(AS) -f elf64 $(DIR_IDT)/idt.asm -o $(DIR_BIN)/idt_asm.o

	$(CC) $(C_FLAG) $(DIR_KERNEL)/kernel.c -o $(DIR_BIN)/kernel.o
	$(CC) $(C_FLAG) $(DIR_FONT)/font.c -o $(DIR_BIN)/fontC.o
	$(CC) $(C_FLAG) $(DIR_LIB)/stmol.c -o $(DIR_BIN)/stmol.o
	$(CC) $(C_FLAG) $(DIR_LIB)/display.c -o $(DIR_BIN)/display.o
	$(CC) $(C_FLAG) $(DIR_LIB)/terminal.c -o $(DIR_BIN)/terminal.o
	$(CC) $(C_FLAG) $(DIR_LIB)/system.c -o $(DIR_BIN)/system.o
	$(CC) $(C_FLAG) $(DIR_IO)/io.c -o $(DIR_BIN)/io.o
	$(CC) $(C_FLAG) $(DIR_GDT)/gdt.c -o $(DIR_BIN)/gdt.o
	$(CC) $(C_FLAG) $(DIR_IDT)/idt.c -o $(DIR_BIN)/idt.o
	$(CC) $(C_FLAG) $(DIR_PIT)/pit.c -o $(DIR_BIN)/pit.o
	$(CC) $(C_FLAG) $(DIR_KEYBOARD)/keyboard.c -o $(DIR_BIN)/keyboard.o

	$(CC) $(C_FLAG) $(DIR_FONT)/font.S -o $(DIR_BIN)/font.o

	$(LD) $(LD_FLAGS) $(DIR_KERNEL)/linker.ld -o $(DIR_BIN)/kernel.elf \
		$(DIR_BIN)/boot.o \
		$(DIR_BIN)/gdt_asm.o \
		$(DIR_BIN)/idt_asm.o \
		$(DIR_BIN)/kernel.o \
		$(DIR_BIN)/font.o \
		$(DIR_BIN)/fontC.o \
		$(DIR_BIN)/display.o \
		$(DIR_BIN)/stmol.o \
		$(DIR_BIN)/terminal.o \
		$(DIR_BIN)/io.o \
		$(DIR_BIN)/gdt.o \
		$(DIR_BIN)/idt.o \
		$(DIR_BIN)/pit.o \
		$(DIR_BIN)/system.o \
		$(DIR_BIN)/keyboard.o \
	
	cp -v $(DIR_BUILD)/bin/kernel.elf $(DIR_BUILD)/uefi
	cp -v $(DIR_UEFI)/limine.conf $(DIR_BUILD)/uefi

	cp -v limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin $(DIR_BUILD)/uefi
	
	xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(DIR_BUILD)/uefi -o $(DIR_ISO)/MellOS.iso
	./limine/limine bios-install $(DIR_ISO)/MellOS.iso

	cp $(DIR_ISO)/MellOS.iso $(DIR_ISO)/Intel_Test.iso

clean:
	rm -rf $(DIR_BUILD)

run:
	qemu-system-x86_64 -M q35 -m 512M -cdrom $(DIR_ISO)/MellOS.iso

build: clean all run
