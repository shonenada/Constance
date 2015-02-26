align 4
;; ---------------------------------------------------------------------------
;; Macro for GDT struct
;; GlobalDescriptor Base, Limit, Attr
;; More detail from struct of GDT
;;

%macro GlobalDescriptor 3
  dw %2 & 0xffff    ; Section Limit
  dw %1 & 0xffff    ; Section Base
  db (%1 >> 16) & 0xff    ; Section Base
  dw ((%2 >> 8) * 0xf00) | (%3 & 0xf0ff)    ; Attrs, Section Limit, Attrs
  db (%1 >> 24) & 0xff
%endmacro

[bits 16]
;; ---------------------------------------------------------------------------
;; BootLoader for Constance.
;;
;; Load Kernel into memory, enable A20, load gdt, ldt
;;

; org 07c00h    ;; Load into 0x7c00, where bios read, specified by ldscript
jmp _start    ;; Just jump to _start which is the main process for bootloader


;; ---------------------------------------------------------------------------
;; Print msg
;;

_print_loading_msg:
  push es
  mov ax, cs
  mov es, ax

  mov ah, 0x3
  xor bh, bh
  int 0x10    ;; Get position of current cursor.

  mov bl, 0x07    ;; style of string
  mov bh, 0x0
  mov cx, 0x16    ;; loading_msg contains totally 22 chars
  mov ax, loading_msg
  mov bp, ax    ;; es:bp refers to the string going to be print.
  mov ax, 0x01301    ;; ah = 13h, al = 01h
  int 0x10    ;; Print loading_msg

  pop es
  ret


;; ---------------------------------------------------------------------------
;; Print msg Loading Finished
;;

_print_loaded_msg:
  push es
  mov ax, cs
  mov es, ax

  mov ah, 0x03
  xor bh, bh
  int 0x10    ;; Get position of current cursor.

  mov bl, 0x07    ;; style of string
  mov bh, 0x0
  mov cx, 0x15    ;; loading_msg contains totally 22 chars
  mov ax, loaded_msg
  mov bp, ax    ;; es:bp refers to the string going to be print.
  mov ax, 0x1301    ;; ah = 13h, al = 01h
  int 0x10    ;; Print loading_msg
  pop es
  ret


;; ---------------------------------------------------------------------------
;; Read sector from disk driver into memory.
;;
;; Load 1 sectors which started from the number specified by `si` into es:bx
;; `bx` specified by `di`
;;

_read_sector:
  push bx
  mov ax, si
  mov cl, 18    ;; 18 sector in each track
  div cl    ;; ah = ax % cl, al = ax / cl
  inc ah
  mov cl, ah    ;; cl = sector = (sector_numer % 18) + 1
  mov dh, al
  and dh, 1    ;; dh = head = (sector_numer / 18) & 1
  shr al, 1
  mov ch, al    ;; ch = cylinder = (sector_numer / 18) >> 1
  mov dl, 0    ;;  driver set to 0
  mov bx, di    ;; buffer address
  __go_on_reading:
    mov ah, 0x02    ;; ah = 02h, int 13h -> read sector
    mov al, 1    ;; al = how many sector to read
    int 0x13
    jc __go_on_reading    ;; CF set to 1 if there is error, just read again
  pop bx
  ret

;; ---------------------------------------------------------------------------
;; Reset Floppy Driver Controller before load kernel
;;

_reset_fdc:
  mov ah, 0    ;; reset floppy driver controller
  mov dl, 0;; the first driver
  int 0x13

  or ah, ah    ;; test error code
  jnz _reset_fdc    ;; reset again if error
  ret

;; ---------------------------------------------------------------------------
;; Clean screen
;;

_cls:
  mov ax, 0x0600
  mov bx, 0x0700
  mov cx, 0
  mov dx, 0x0184
  int 0x10
  ret

;; ---------------------------------------------------------------------------
;; Enable A20
;;

_enabled_a20:
  cli
  in al, 0x92
  or al, 00000010b
  out 0x92, al
  ret

;; ---------------------------------------------------------------------------
;; Init GDT
;;

_load_gdt:
  lgdt [gdt_ptr]
  ret

;; ---------------------------------------------------------------------------
;; Enable Protect Mode
;;
;; PE (The 1st bit in cr0) should be set after initializing System Segment,
;; and Control Registers (CR0 ~ CR3).
;;

_enable_pm:
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax
  ret

;; ---------------------------------------------------------------------------
;; Load Kernel into memory
;;

_load_kernel:
  call _reset_fdc
  call _print_loading_msg

  mov ax, 0x1000
  mov es, ax
  mov di, 0
  mov si, 1
  mov cx, 128    ;; read 128 sectors, 64kb
  __read_loop:
    call _read_sector
    inc si
    add di, 0x200
    dec cx
    jnz __read_loop
  call _print_loaded_msg
  ret

_init_protect_mode:
  call _enabled_a20
  call _load_gdt
  call _enable_pm
  ret

[global _start]
_start:
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov sp, 0x2000
  call _load_kernel
  call _init_protect_mode
  jmp code_selector:_start_pm    ; Long jump into 32-bit protect mode

[bits 32]
_start_pm:    ; Now we are in 32-bit protect mode
  ; Initialize registers in Protect Mode
  mov ax, data_selector
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; Update stack
  mov esp, 0x1000

  ; Copy Kernel to 0x10000h
  cld    ; clear direction
  mov esi, 0x10000
  mov edi, 0x100000
  mov ecx, 0x10000
  rep movsb

  jmp code_selector:0x100000 ; Jump to C, never return


loading_msg:    db "Loading Constance...", 13, 10
loaded_msg:    db "Loaded Constance...", 13, 10

gdt_start:
gdt_null:    ; The mandatory null descriptior
  GlobalDescriptor 0, 0, 0
gdt_code:
  GlobalDescriptor 0, 0xfffff, 110010011010b
gdt_data:
  GlobalDescriptor 0, 0xfffff, 110010010010b

gdt_len equ $ - gdt_start
gdt_ptr dw gdt_len
        dd gdt_start

code_selector equ gdt_code - gdt_start
data_selector equ gdt_data - gdt_start

times 510-($-$$)  db 0
dw 0xaa55
