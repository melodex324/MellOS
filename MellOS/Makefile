#This is the Makefile that build the MDX/MDXM operating system

#This is the path to cross compiler
export PATH := $(HOME)/opt/cross/bin:$(PATH)

#The alias for the compiler tools
AS = i686-elf-as 		#ASM compiler
GCC = i686-elf-gcc		#C compiler

#The compiler flags
C_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(DIR_LIB) -I$(DIR_INTERRUPTS) -I$(DIR_PIT) -I$(DIR_SHELL) -I$(DIR_COMMANDS)
LD_FLAGS = -ffreestanding -O2 -nostdlib -lgcc


#The directories used
DIR_BUILD = Build
DIR_LIB = lib/include
DIR_INTERRUPTS = interrupts
DIR_PIT = pit
DIR_SHELL = shell
DIR_COMMANDS = $(DIR_SHELL)/commands

DIR_ISO = $(DIR_BUILD)/isodir
DIR_BIN = $(DIR_BUILD)/binaries

DIR_SRC = kernel lib/src $(DIR_INTERRUPTS)	$(DIR_PIT) $(DIR_SHELL) $(DIR_COMMANDS)		#code files in these paths will be compiled

#The output directories where the compiled files and iso will be placed
KERNEL_BIN	= $(DIR_BUILD)/binaries/mdx.bin
ISO	= $(DIR_BUILD)/mdx.iso


#Locate all .c and .s files
SRC_C = $(foreach dir, $(DIR_SRC), $(wildcard $(dir)/*.c))
SRC_ASM = $(foreach dir, $(DIR_SRC), $(wildcard $(dir)/*.s))

#Object and binary files path (Build/binaries)
O_C = $(patsubst %.c, $(DIR_BIN)/%.o, $(SRC_C))
O_ASM = $(patsubst %.c, $(DIR_BIN)/%.s, $(SRC_ASM))
O_PATH = $(O_C) $(O_ASM)



#Comile every C files in paths listen in DIR_SRC
$(DIR_BIN)/%.o: %.c
	@mkdir -p $(dir $@)
	$(GCC) -c $< -o $@ $(C_FLAGS)

$(DIR_BIN)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

# Link
$(KERNEL_BIN): $(O_PATH)
	$(GCC) -T kernel/linker.ld -o $@ $(O_PATH) $(LD_FLAGS)
	@if grub-file --is-x86-multiboot $@; then \
		echo "Multiboot confirmed"; \
	else \
		echo "ERROR: Not a multiboot file!"; exit 1; \
	fi

#setting up and creating the iso
$(ISO): $(KERNEL_BIN)
	@echo "MDXOS is building"
	mkdir -p $(DIR_ISO)/boot/grub
	cp $(KERNEL_BIN) $(DIR_ISO)/boot/mdx.bin
	cp bootloader/grub.cfg $(DIR_ISO)/boot/grub/grub.cfg
	@grub-mkrescue -o $@ $(DIR_ISO)


#Makefile target
.PHONY:
all: $(ISO) check

.PHONY: build 
build: rebuild run

.PHONY: build -d
build: rebuild run

# Rebuild everything
.PHONY: rebuild 
rebuild: clean all

#launch a VM using qemu
.PHONY: run
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

#boot the iso without recompiling everytime
.PHONY: boot
boot:
	@qemu-system-i386 -cdrom $(ISO)

#clean the Build directory
.PHONY: clean
clean:
	rm -rf $(DIR_BUILD)

.PHONY: debug
debug: $(ISO)
	@qemu-system-i386 -cdrom $(ISO) -monitor stdio

.PHONY: check
check:
	@if test -f $(ISO); then \
		echo "MDXOS iso has been build : $(ISO)"; \
	else \
		echo "ERROR: ISO not found" \
		echo "MDXOS Build has failed"; exit 1; \
	fi

