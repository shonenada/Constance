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
  push byte 0    ;; dummy error code
  push byte 0    ;; vector
  jmp isr_common_hdl

; 1: Debug
isr1:
  cli
  push byte 0
  push byte 1
  jmp isr_common_hdl

; 2: Non Maskable Interrupt
isr2:
  cli
  push byte 0
  push byte 2
  jmp isr_common_hdl

; 3: Breakpoint
isr3:
  cli
  push byte 0
  push byte 3
  jmp isr_common_hdl

; 4: Overflow
isr4:
  cli
  push byte 0
  push byte 4
  jmp isr_common_hdl

; 5: Out of Bounds
isr5:
  cli
  push byte 0
  push byte 5
  jmp isr_common_hdl

; 6: Invalid Operand Code
isr6:
  cli
  push byte 0
  push byte 6
  jmp isr_common_hdl

; 7: No Coprocessor
isr7:
  cli
  push byte 0
  push byte 7
  jmp isr_common_hdl

; 8: Double Fault
isr8:
  cli
  ;; error code pushed already
  push byte 8
  jmp isr_common_hdl

; 9: Coprocessor Segment Overrun
isr9:
  cli
  push byte 0
  push byte 9
  jmp isr_common_hdl

; 10:  Bad TTS
isr10:
  cli
  push byte 10
  jmp isr_common_hdl

; 11: Segment Not Present
isr11:
  cli
  push byte 11
  jmp isr_common_hdl

; 12: Stack Fault
isr12:
  cli
  push byte 12
  jmp isr_common_hdl

; 13: General Protection Fault
isr13:
  cli
  push byte 13
  jmp isr_common_hdl

; 14: Page Fault
isr14:
  cli
  push byte 14
  jmp isr_common_hdl

; 15: Unknown Interrupt
isr15:
  cli
  push byte 0
  push byte 15
  jmp isr_common_hdl

; 16: Coprocessor Fault
isr16:
  cli
  push byte 0
  push byte 16
  jmp isr_common_hdl

; Alignment Check
isr17:
  cli
  push byte 0
  push byte 17
  jmp isr_common_hdl

; Machine Check
isr18:
  cli
  push byte 0
  push byte 18
  jmp isr_common_hdl

; SIMD Floating-Point
isr19:
  cli
  push byte 0
  push byte 19
  jmp isr_common_hdl

; Reserved
isr20:
  cli
  push byte 0
  push byte 20
  jmp isr_common_hdl

; Reserved
isr21:
  cli
  push byte 0
  push byte 21
  jmp isr_common_hdl

; Reserved
isr22:
  cli
  push byte 0
  push byte 22
  jmp isr_common_hdl

; Reserved
isr23:
  cli
  push byte 0
  push byte 23
  jmp isr_common_hdl

; Reserved
isr24:
  cli
  push byte 0
  push byte 24
  jmp isr_common_hdl

; Reserved
isr25:
  cli
  push byte 0
  push byte 25
  jmp isr_common_hdl

; Reserved
isr26:
  cli
  push byte 0
  push byte 26
  jmp isr_common_hdl

; Reserved
isr27:
  cli
  push byte 0
  push byte 27
  jmp isr_common_hdl

; Reserved
isr28:
  cli
  push byte 0
  push byte 28
  jmp isr_common_hdl

; Reserved
isr29:
  cli
  push byte 0
  push byte 29
  jmp isr_common_hdl

; Reserved
isr30:
  cli
  push byte 0
  push byte 30
  jmp isr_common_hdl

; Reserved
isr31:
  cli
  push byte 0
  push byte 31
  jmp isr_common_hdl

;; ISR stub, saves processor state, sets up for kernel mode segments,
;;  calls fault handler, finally restore stack
extern fault_handler    ;; from C
isr_common_hdl:
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
  mov eax, fault_handler
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
  push 0
  push 32
  jmp irq_common_hdl

irq1:
  cli
  push 0
  push 33
  jmp irq_common_hdl

irq2:
  cli
  push 0
  push 34
  jmp irq_common_hdl

irq3:
  cli
  push 0
  push 35
  jmp irq_common_hdl

irq4:
  cli
  push 0
  push 36
  jmp irq_common_hdl

irq5:
  cli
  push 0
  push 37
  jmp irq_common_hdl

irq6:
  cli
  push 0
  push 38
  jmp irq_common_hdl

irq7:
  cli
  push 0
  push 39
  jmp irq_common_hdl

irq8:
  cli
  push 0
  push 40
  jmp irq_common_hdl

irq9:
  cli
  push 0
  push 41
  jmp irq_common_hdl

irq10:
  cli
  push 0
  push 42
  jmp irq_common_hdl

irq11:
  cli
  push 0
  push 43
  jmp irq_common_hdl

irq12:
  cli
  push 0
  push 44
  jmp irq_common_hdl

irq13:
  cli
  push 0
  push 45
  jmp irq_common_hdl

irq14:
  cli
  push 0
  push 46
  jmp irq_common_hdl

irq15:
  cli
  push 0
  push 47
  jmp irq_common_hdl


[extern irq_handler]

irq_common_hdl:
  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp
  push eax
  mov eax, irq_handler
  call eax
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret
