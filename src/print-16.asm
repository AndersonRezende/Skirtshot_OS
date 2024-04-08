print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret

print:
    pusha
    mov ah, 0x0e
    .print_loop:
        mov al, [si]
        cmp al, 0
        je .end_print
        int 0x10
        inc si
        jne .print_loop
    .end_print:
        popa
        ret