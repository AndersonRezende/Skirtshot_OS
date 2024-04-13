[ORG 0x7c00]                        ; Endereço inicial que a BIOS devolve ao encontrar o bootloader
[bits 16]                           ; Execução em modo real 16 bits

mov [BOOT_DISK], dl                 ; O BIOS coloca o disco atual do registrador dl. Armazena o disco nesse identificador para utiliza-lo depois caso seja substituido ao longo do processo.

KERNEL_LOCATION equ 0x1000
mov bp, 0x9000
mov sp, bp

BOOT_DISK: db 0

_start:
    mov si, msg
    call print
    call print_nl

    mov bx, KERNEL_LOCATION         ; Local onde os dados serão carregados
    call disk_load                  ; Carrega o setor contendo o kernel
    call switch_to_32bit            ; Altera para o modo protegido
    call KERNEL_LOCATION            ; Pula para onde o kernel foi carregado
    jmp $                           ; Loop caso retorne


%include "src/boot/print-16.asm"
%include "src/boot/disk.asm"
%include "src/boot/gdt.asm"
%include "src/boot/switch_to_32bit.asm"

msg db 'Skirtshot OS',0

times 510 - ($-$$) db 0             ; Complementa os bytes restantes com zero até 510
dw 0xaa55                           ; Coloca a assinatura aa55 nos últimos dois bytes para configurar como bootável