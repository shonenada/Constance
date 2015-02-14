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

;; ---------------------------------------------------------------------------
;; BootLoader for Constance.
;;
;; Load Kernel into memory, enable A20, load gdt, ldt
;;

org 07c00h    ;; Load into 0x7c00, where bios read
jmp _start    ;; Just jump to _start which is the main process for bootloader


;; ---------------------------------------------------------------------------
;; Print msg
;;

_print_loading_msg:
  mov ax, 03h
  xor bh, bh
  int 10h    ;; Get position of current cursor.

  mov bx, 07h    ;; style of string
  mov cx, 16h    ;; loading_msg contains totally 22 chars
  mov ax, loading_msg
  mov bp, ax    ;; es:bp refers to the string going to be print.
  mov ax, 01301h    ;; ah = 13h, al = 01h
  int 10h    ;; Print loading_msg

  ret

;; ---------------------------------------------------------------------------
;; Read sector from disk driver into memory.
;;
;; Load `cl` sectors which started from the number specified by `ax` into es:bx
;;

_read_sector:
  push bp
  mov bp, sp
  sub esp, 2    ;; Place for amount of sector to load
  mov byte [bp-2], cl    ;; cl = how many sector to load
  push bx
  mov bl, 18    ;; 18 sector in each track
  div bl    ;; ah = ax % bl, al = ax / bl
  inc ah
  mov cl, ah    ;; cl = sector = (sector_numer % 18) + 1
  mov dh, al
  and dh, 1    ;; dh = head = (sector_numer / 18) & 1
  shr al, 1
  mov ch, al    ;; ch = cylinder = (sector_numer / 18) >> 1
  pop bx
  mov dl, 0    ;;  drive set to 0
  __go_on_reading:
    mov ah, 2    ;; ah = 02h, int 13h -> read sector
    mov al, byte [bp-2]    ;; al = how many sector to read
    int 13h
    jc __go_on_reading    ;; CF set to 1 if there is error, just read again
  add esp, 2
  pop bp
  ret

;; ---------------------------------------------------------------------------
;; Reset Floppy Driver Controller before load kernel
;;

_reset_fdc:
  xor ah, ah    ;; reset floppy driver controller
  xor dl, dl    ;; the first driver
  int 13h

  or ah, ah    ;; test error code
  jnz _reset_fdc    ;; reset again if error
  ret

;; ---------------------------------------------------------------------------
;; Clean screen
;;

_cls:
  mov ax, 0600h
  mov bx, 0700h
  mov cx, 0
  mov dx, 0184h
  int 10h
  ret

;; ---------------------------------------------------------------------------
;; Enable A20
;;

_enabled_a20:
  cli
  in al, 92h
  or al, 00000010b
  out 92h, al
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
  or eax, 1h
  mov cr0, eax
  ret

;; ---------------------------------------------------------------------------
;; Load Kernel into memory
;;

_load_kernel:
  call _print_loading_msg
  call _reset_fdc

  mov ax, 1000h
  mov es, ax
  mov bx, 0
  mov ax, si
  mov cl, 128    ;; read 128 sectors, 64kb
  call _read_sector
  ret

_init_protect_mode:
  call _enabled_a20
  call _load_gdt
  call _enable_pm
  ret

[global _start]
_start:
  mov ax, cs
  mov ds, ax
  mov es, ax
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
  mov esp, 90000h

  ; Copy Kernel to 0x10000h
  cld    ; clear direction
  mov esi, 10000h
  mov edi, 100000h
  mov ecx, 10000h
  rep movsb

  jmp code_selector:100000h ; Jump to C, never return


loading_msg:    db "Loading Constance...", 13, 10

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
