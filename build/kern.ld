ENTRY(kmain)
SECTIONS {
    __bios__ = 0xa0000;
    vgamemptr = 0xb8000;
    .text 0x100000 : {
        code = .;
        bin/entry.o(.text) bin/main.o(.text) *(.text);
        *(.rodata);
        . = ALIGN(4096);
    }
    .data : {
        data = .;
        *(.data)
        . = ALIGN(4096);
    }
    .bss : {
        bss = .;
        *(.bss);
        . = ALIGN(4096);
    }
    end = .;
}

