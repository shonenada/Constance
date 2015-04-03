SECTIONS {
    . = 0x8100000;
    .text : {
        __text__ = .;
        *(.rodata .gnu.linkonce.*)
        . = ALIGN(0x1000);
        __text_end__ = .;
    }
    .data : {
        __data__ = .;
        *(.data)
        . = ALIGN(0x1000);
    }
    .bss : {
        __bss__ = .;
        *(.bss)
        . = ALIGN(0x1000);
    }
}
