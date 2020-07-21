[bits 32]

VIDEO_MEMORY equ 0xb8000    ; starting address of VGA
WHITE_ON_BLACK equ 0x0f     ; attr of characters

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je print_string_pm_done

    mov [edx], ax           ; ax = al (character) + ah (attr)
    add ebx, 1
    add edx, 2

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret