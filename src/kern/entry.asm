[bits 32]
align 4

[extern kmain]
  call kmain
  jmp $

[global gdt_flush]
[extern gp]    ;; gp defined by C in segment.c
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

[extern pgd0]
[global flush_cr3]
flush_cr3:
  mov eax, [pgd0]
  mov cr3, eax
  ret

[global page_enable]
page_enable:
 push eax
 mov eax, cr0
 or eax, 0x80000000
 mov cr0, eax
 pop eax
 ret

[global do_swtch]
do_swtch:
  mov eax, dword [esp+4]
  pop dword [eax]
  mov dword [eax+4], esp
  mov dword [eax+8], ebx
  mov dword [eax+12], ecx
  mov dword [eax+16], edx
  mov dword [eax+20], esi
  mov dword [eax+24], edi
  mov dword [eax+28], ebp

  mov eax, dword [esp+4]

  mov ebp, dword [eax+28]
  mov edi, dword [eax+24]
  mov esi, dword [eax+20]
  mov edx, dword [eax+16]
  mov ecx, dword [eax+12]
  mov ebx, dword [eax+8]
  mov esp, dword [eax+4]
  push dword [eax]
  ret

[global _jump_u]
_jump_u:
  pop dword eax    ;; retuen eip
  pop dword ebx    ;; eip
  pop dword ecx    ;; esp3
  mov ax, 0x23    ;; ds = (4<<3|3)
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  push dword 0x23    ;; ss3
  push dword ecx    ;; esp3
  pushf    ;; eflags
  push dword 0x1B    ;; cs = (3<<3|3)
  push dword ebx    ;; eip
  iretd

[section .text]
[global _int_common_ret]
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
_int_common_ret:
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
%rep 256
mkhwint i
%assign i i+1
%endrep

[section .data]
[global hwint]
hwint:
  %assign i 0
  %rep 256
    def_hwint i
  %assign i i+1
  %endrep
