SECTIONS {
    . = 0x8000020;
    .text : {
        __text__ = .;
        bin/libsys/entry.o(.text) *(.text)
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
