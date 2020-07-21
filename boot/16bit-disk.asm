; read dh sectors from drive dl into es:bx
disk_load:
    pusha
    push dx         ; save parameters

    mov ah, 0x02    ; read disk
    mov al, dh      ; read dh sectors
    mov cl, 0x02    ; read the second sector
    mov ch, 0x00    ; read 0x00 cylinder
    mov dh, 0x00    ; head number

    int 0x13        ; interrupt to start reading
    jc disk_error   ; if carry == 1, error occured

    pop dx
    cmp al, dh      ; check if we read dh sectors
    jne sectors_error
    popa
    ret

disk_error:
    mov bx, DISK_ERROR
    call print
    call print_nl
    mov dh, ah      ; ah = error code, dl = disk drive that dropped the error
    call print_hex
    jmp disk_loop

sectors_error:
    mov bx, SECTORS_ERROR
    call print

disk_loop:
    jmp $


DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0