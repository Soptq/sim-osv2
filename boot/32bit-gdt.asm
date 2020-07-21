; in x86, there is only one GDT, which pointed to 0x0 and has a length of 0xFFFFFFFF.

gdt_start:
    ; The CPU requires that the first entry in the GDT
    ; purposely be an invalid null descriptor, this
    ; is to avoid accidentally modifying GDT when segment
    ; register is set to 0x0
    dd 0x0;     ; let us call it code segment
    dd 0x0;     ; let us call it data segment

; real GDT starts at 0x08

gdt_code:
    dw 0xffff   ; 00-15     segment limit
    dw 0x0      ; 00-15     base address
    db 0x0      ; 16-23     base address
    db 10011010b; flag1
    db 11001111b; flag2
    db 0x0      ; 24-31     base addresss

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b; not for code
    db 11001111b
    db 0x0

gdt_end:

; When initializing, CPU needs two values:
; a 16 bits GDT size (one less of its true size) and 32 bits entry adrress
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
