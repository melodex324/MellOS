ENTRY (_start)

SECTIONS
{
    . = 2M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4k)
    {
        *(.rodata)
    }

    .data BLOCK(4k) : ALIGN(4k)
    {
        *(.data)
    }

    .bss BLOCK(4k) : ALIGN(4k)
    {
        *(COMMON)
        *(.bss)
    }

}