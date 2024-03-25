; Endereço inicial que a BIOS devolve ao encontrar o bootloader
[ORG 0x7c00]
; Execução em modo real 16 bits
[bits 16]

_start:
    call print
    jmp $

print:
    pusha
    mov si, msg
    mov ah, 0x0e
    .print_loop:
        mov al, [si]
        cmp al, 0
        je .end_print
        int 0x10
        inc si
        jne .print_loop
    .end_print
        popa
        ret

msg db 'Skirtshot OS',0

; padding
times 510 - ($-$$) db 0
; magic number
dw 0xaa55