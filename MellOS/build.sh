#This is the script/bash file to build the operating system 
#It is outaded and should not be used
#Use the Makefile instead

echo "MDXOS is building"

#from now on the script will execute commands that will create the iso file
export PATH="$HOME/opt/cross/bin:$PATH"

#make in a directory where the build will be
rm -r Build #remove if the directory exist
mkdir Build
cd Build

LIBDIR="../lib/include/" #path to the C include folder of the Blue library

#these commands will assemble, link and compile the os
i686-elf-as ../kernel/boot.s -o boot.o
i686-elf-gcc -c ../kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$LIBDIR
i686-elf-gcc -c ../lib/src/stmol.c -o stmol.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$LIBDIR

i686-elf-gcc -T ../kernel/linker.ld -o mdx.bin -ffreestanding -O2 -nostdlib boot.o kernel.o stmol.o -lgcc

#grub/bootloader code
grub-file --is-x86-multiboot mdx.bin
if grub-file --is-x86-multiboot mdx.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

#setting up and creating the iso
mkdir -p isodir/boot/grub
cp mdx.bin isodir/boot/mdx.bin
cp ../bootloader/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o mdx.iso isodir

#launch a VM using qemu
qemu-system-i386 -cdrom mdx.iso