; Endereço inicial que a BIOS devolve ao encontrar o bootloader
[ORG 0x7c00]
; Execução em modo real 16 bits
[bits 16]

_start:
    mov si, msg
    call print
    call print_nl
    call switch_to_32bit
    jmp $


%include "src/print-16.asm"
%include "src/gdt.asm"
%include "src/switch_to_32bit.asm"

msg db 'Skirtshot OS',0

; padding
times 510 - ($-$$) db 0
; magic number
dw 0xaa55