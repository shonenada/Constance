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

[global isr0]
[global isr1]
[global isr2]
[global isr3]
[global isr4]
[global isr5]
[global isr6]
[global isr7]
[global isr8]
[global isr9]
[global isr10]
[global isr11]
[global isr12]
[global isr13]
[global isr14]
[global isr15]
[global isr16]
[global isr17]
[global isr18]
[global isr19]
[global isr20]
[global isr21]
[global isr22]
[global isr23]
[global isr24]
[global isr25]
[global isr26]
[global isr27]
[global isr28]
[global isr29]
[global isr30]
[global isr31]

; 0: Division By Zero
isr0:
  cli
  push dword 0    ;; dummy error code
  push dword 0    ;; vector
  jmp int_common_hdl

; 1: Debug
isr1:
  cli
  push dword 0
  push dword 1
  jmp int_common_hdl

; 2: Non Maskable Interrupt
isr2:
  cli
  push dword 0
  push dword 2
  jmp int_common_hdl

; 3: Breakpoint
isr3:
  cli
  push dword 0
  push dword 3
  jmp int_common_hdl

; 4: Overflow
isr4:
  cli
  push dword 0
  push dword 4
  jmp int_common_hdl

; 5: Out of Bounds
isr5:
  cli
  push dword 0
  push dword 5
  jmp int_common_hdl

; 6: Invalid Operand Code
isr6:
  cli
  push dword 0
  push dword 6
  jmp int_common_hdl

; 7: No Coprocessor
isr7:
  cli
  push dword 0
  push dword 7
  jmp int_common_hdl

; 8: Double Fault
isr8:
  cli
  ;; error code pushed already
  push dword 8
  jmp int_common_hdl

; 9: Coprocessor Segment Overrun
isr9:
  cli
  push dword 0
  push dword 9
  jmp int_common_hdl

; 10:  Bad TTS
isr10:
  cli
  push dword 10
  jmp int_common_hdl

; 11: Segment Not Present
isr11:
  cli
  push dword 11
  jmp int_common_hdl

; 12: Stack Fault
isr12:
  cli
  push dword 12
  jmp int_common_hdl

; 13: General Protection Fault
isr13:
  cli
  push dword 13
  jmp int_common_hdl

; 14: Page Fault
isr14:
  cli
  push dword 14
  jmp int_common_hdl

; 15: Unknown Interrupt
isr15:
  cli
  push dword 0
  push dword 15
  jmp int_common_hdl

; 16: Coprocessor Fault
isr16:
  cli
  push dword 0
  push dword 16
  jmp int_common_hdl

; Alignment Check
isr17:
  cli
  push dword 0
  push dword 17
  jmp int_common_hdl

; Machine Check
isr18:
  cli
  push dword 0
  push dword 18
  jmp int_common_hdl

; SIMD Floating-Point
isr19:
  cli
  push dword 0
  push dword 19
  jmp int_common_hdl

; Reserved
isr20:
  cli
  push dword 0
  push dword 20
  jmp int_common_hdl

; Reserved
isr21:
  cli
  push dword 0
  push dword 21
  jmp int_common_hdl

; Reserved
isr22:
  cli
  push dword 0
  push dword 22
  jmp int_common_hdl

; Reserved
isr23:
  cli
  push dword 0
  push dword 23
  jmp int_common_hdl

; Reserved
isr24:
  cli
  push dword 0
  push dword 24
  jmp int_common_hdl

; Reserved
isr25:
  cli
  push dword 0
  push dword 25
  jmp int_common_hdl

; Reserved
isr26:
  cli
  push dword 0
  push dword 26
  jmp int_common_hdl

; Reserved
isr27:
  cli
  push dword 0
  push dword 27
  jmp int_common_hdl

; Reserved
isr28:
  cli
  push dword 0
  push dword 28
  jmp int_common_hdl

; Reserved
isr29:
  cli
  push dword 0
  push dword 29
  jmp int_common_hdl

; Reserved
isr30:
  cli
  push dword 0
  push dword 30
  jmp int_common_hdl

; Reserved
isr31:
  cli
  push dword 0
  push dword 31
  jmp int_common_hdl

;; IRQs
[global irq0]
[global irq1]
[global irq2]
[global irq3]
[global irq4]
[global irq5]
[global irq6]
[global irq7]
[global irq8]
[global irq9]
[global irq10]
[global irq11]
[global irq12]
[global irq13]
[global irq14]
[global irq15]

irq0:
  cli
  push dword 0 
  push dword 32
  jmp int_common_hdl

irq1:
  cli
  push dword 0
  push dword 33
  jmp int_common_hdl

irq2:
  cli
  push dword 0
  push dword 34
  jmp int_common_hdl

irq3:
  cli
  push dword 0
  push dword 35
  jmp int_common_hdl

irq4:
  cli
  push dword 0
  push dword 36
  jmp int_common_hdl

irq5:
  cli
  push dword 0
  push dword 37
  jmp int_common_hdl

irq6:
  cli
  push dword 0
  push dword 38
  jmp int_common_hdl

irq7:
  cli
  push dword 0
  push dword 39
  jmp int_common_hdl

irq8:
  cli
  push dword 0
  push dword 40
  jmp int_common_hdl

irq9:
  cli
  push dword 0
  push dword 41
  jmp int_common_hdl

irq10:
  cli
  push dword 0
  push dword 42
  jmp int_common_hdl

irq11:
  cli
  push dword 0
  push dword 43
  jmp int_common_hdl

irq12:
  cli
  push dword 0
  push dword 44
  jmp int_common_hdl

irq13:
  cli
  push dword 0
  push dword 45
  jmp int_common_hdl

irq14:
  cli
  push dword 0
  push dword 46
  jmp int_common_hdl

irq15:
  cli
  push dword 0
  push dword 47
  jmp int_common_hdl

;; hardware interrupt common stub,
;;  saves processor state, sets up for kernel mode segments,
;;  calls fault handler, finally restore stack
extern int_handler    ;; from C
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
