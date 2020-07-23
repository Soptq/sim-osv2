; Generally, in our system
; KERNEL starts at 0x1000
; Stack starts at 0x90000 and it grows downward

; Heap is set to 0x1000000 and it grows upward.


[org 0x7c00]
KERNEL_OFFSET equ 0x1000
mov [BOOT_DRIVE], dl    ; BOOT_DRIVE is set to 0

mov bp, 0x9000          ; stack starts at 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE   ; print msg
call print
call print_nl

call load_kernel        ; load kernel to KERNEL_OFFSET
call switch_to_pm

jmp $

%include "./boot/16bit-print.asm"
%include "./boot/16bit-print_hex.asm"
%include "./boot/16bit-disk.asm"
%include "./boot/32bit-gdt.asm"
%include "./boot/32bit-print.asm"
%include "./boot/32bit-switch.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL ; print msg
    call print
    call print_nl

    mov bx, KERNEL_OFFSET   ; our kernel is located at 0x1000
                            ; so before calling int 13, we set bx to our kernel offset
                            ; so that int 13 will read our kernel to bx
    mov dh, 54              ; read 31 sectors, each sector contains 512 bytes / 0.5 Kb
                            ; plus the boot sector, we read in total 32 sectors
    mov dl, [BOOT_DRIVE]    ; designate boot drive 0
    call disk_load          ; load disk, read data to the memory

    mov bx, MSG_LOADED_KERNEL ; print msg
    call print
    call print_nl
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE  ; print msg
    call print_string_pm

    call KERNEL_OFFSET      ; call our c_entry code

    mov ebx, MSG_RETURNED_KERNEL    ; this line code should never be run
    call print_string_pm

    jmp $


BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in RM", 0
MSG_SW_PROT_MODE db "Switching to PM", 0
MSG_PROT_MODE db "Landed in 32-bit PM", 0
MSG_LOAD_KERNEL db "Loading kernel", 0
MSG_LOADED_KERNEL db "Loading kernel completed", 0
MSG_RETURNED_KERNEL db "Returned from kernel. Error?", 0

times 510-($-$$) db 0
dw 0xaa55