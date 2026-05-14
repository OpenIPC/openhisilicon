ENTRY(_entry)

SECTIONS
{
    . = 0x04000000;
    .text : {
        re/start.o(.text*)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss* COMMON) }
}
