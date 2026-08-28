OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);   /* Read | Execute */
    rodata PT_LOAD FLAGS(4); /* Read */
    data PT_LOAD FLAGS(6);   /* Read | Write */
}

SECTIONS
{
    . = 0xffffffff80000000;

    .text : {
        *(.text .text.*)
    } :text

    . = ALIGN(0x1000);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    . = ALIGN(0x1000);

    .data : {
        *(.data .data.*)
    } :data

    .requests : {
        KEEP(*(.requests))
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data
}