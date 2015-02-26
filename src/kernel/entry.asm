[bits 32]

[extern kmain]
  call kmain
  jmp $

[global gdt_flush]
[extern gp]    ;; gp defined by C in gdt.c
gdt_flush:
  lgdt [gp]    ;; Load GDT with _gp
  mov ax, 0x10    ;; 0x10, offset in the GDT to data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:_ret_c    ;; 0x08, offset in the GDT to code segment
_ret_c:
  ret    ;; return to C
