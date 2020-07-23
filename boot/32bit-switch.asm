[bits 16]
switch_to_pm:
    mov bx, MSG_SW_PROT_MODE ; print msg
    call print
    call print_nl

    cli                     ; 1. disable interrupts
    lgdt [gdt_descriptor]   ; 2. load the GDT descriptor

    mov eax, cr0
    or eax, 0x1             ; 3. set 32-bit mode bit in cr0
    mov cr0, eax

    jmp CODE_SEG:init_pm    ; 4. far jump by using a different segment / jump to another segment


[bits 32]
init_pm:
    mov ax, DATA_SEG        ; 5. update the segment registers
    mov dx, ax              ; data segment
    mov ss, ax              ; stack segment
    mov es, ax              ; destination segment
    mov fs, ax              ;
    mov gs, ax

    mov ebp, 0x90000        ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call BEGIN_PM

