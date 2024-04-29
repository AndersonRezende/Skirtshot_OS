disk_load:
    pusha
    mov ah, 0x02               ; Modo leitura.
    mov al, 0x31               ; Número de setores a serem lidos.
    mov ch, 0x00               ; Número do cilindro.
    mov dh, 0x00               ; Número da cabeça do disco (head).
    mov cl, 0x02               ; Número do setor que será lido (o setor 1 é onde está o boot).
    mov dl, [BOOT_DISK]        ; Unidade de disco que será lida (dispositivo).
    int 0x13                   ; Interrupção da BIOS
    
    jc disk_error              ; Checa erro no carry bit
    popa
    ret

disk_error:
    mov si, error_disk
    call print
    call print_nl
    jmp $

error_disk db 'Erro de leitura no disco',0