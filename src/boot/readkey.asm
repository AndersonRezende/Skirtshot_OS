Readkey:
    mov ah, 0x00
    int 0x16
    ret

Printkey:
    mov dx, ax
    mov ah, 0x0e
    mov al, dl
    int 0x10
    ret 