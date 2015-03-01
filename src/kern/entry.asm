[bits 32]
align 4

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

[section .text]
;; hardware interrupt common stub,
;;  saves processor state, sets up for kernel mode segments,
;;  calls fault handler, finally restore stack
[extern int_handler]    ;; from C
int_common_hdl:
  pusha    ;; push eax, ecx, edx, ebx, esp, ebp, esi, edi
  push ds
  push es
  push fs
  push gs
  ;; saved current state
  mov ax, 0x10    ;;  0x10 -> Kernel Data Segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp    ;; Push up stack
  push eax    ;; Push esp, which used by fault_handler
  mov eax, int_handler
  call eax
  ;; restore state
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8    ;; Cleans up pushed error code, isr number, cs, eip, eflags, ss, esp
  iret


%macro mkhwint 1
_hwint%1:
  %if %1 != 8 && (%1 < 10 || %1 > 14)
    push dword 0
  %endif
  push dword %1
  jmp int_common_hdl
%endmacro

%macro def_hwint 1
  dd _hwint%1
%endmacro

%assign i 0
%rep 49
mkhwint i
%assign i i+1
%endrep

[section .data]
[global hwint]
hwint:
  %assign i 0
  %rep 49
    def_hwint i
  %assign i i+1
  %endrep
