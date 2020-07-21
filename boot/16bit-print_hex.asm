print_hex:
    pusha
    mov cx, 0


; to print hex 0x1234, we need to convert it to string 0x1234 and then call print function
; for 0-9, we will just add 0x30 to them so that 0x0 could be 0x30
; for A-F, we will add to them so that 0xA could be 0x41

hex_loop:
    cmp cx, 4       ; iterate 4 times as each register currently has 16 bits / 4 bytes
    je end

    mov ax, dx;
    and ax, 0x000f  ; mask every byte except the last one
    add al, 0x30
    cmp al, 0x39
    jle step2       ; if alphabet is 0-9, continue
    add al, 7       ; after adding 0x30, A becomes 0x3A. In order to make it 0x41, we add another 7 to it.

step2:
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al
    ror dx, 4       ; circle right shift 4 bits / 1 byte
    add cx, 1
    jmp hex_loop

end:
    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT:
    db '0x0000', 0
