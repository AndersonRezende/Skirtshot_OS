gdt_start:
    ; O GDT começa com 8 bytes nulos
    dq 0x0          ; Define Quadword 8 bytes

; GDT para segmento de código. Base = 0x00000000 e comprimento 0xfffff
gdt_code:           ; base 0 (32 bit)
    dw 0xffff       ; limit: 20 bit value describing where the segment ends, can be multiplied by 4096 if granularity = 1
    dw 0x0          ; pres, priv, type. Present must be 1 for the entry to be valid
    db 0x0          ; Type flags
    db 0b10011010   ; Other flags
    db 0b11001111   ; Last bits of base
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 0b10010010
    db 0b11001111
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start