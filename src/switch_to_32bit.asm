switch_to_32bit:
	; Linha A20 é uma linha de endereço de memória no barramento de endereço do processador x86. Ela refere-se ao bit 20 do endereço físico da memória.
	;mov ax, 0x2401
    ;int 0x15 ; Habilita o A20 bit.

    ; Entra no modo texto VGA
    mov ah, 0x00
    mov al, 0x03
    int 0x10

	cli		 							; Desabilita as interrupções
	lgdt [gdt_descriptor] 				; Carrega a GDT
	mov eax, cr0 
	or eax, 0x1							; Configura o modo 32 bits no cr0. O CR0 não pode ser alterado diretamente.
	mov cr0, eax

	jmp CODE_SEG:start_protected_mode 	; Pulo distante para o segmento
	;jmp $

[bits 32]
start_protected_mode:					; Começando a usar instruções de 32 bits
	mov ax, DATA_SEG					; Atualiza os registradores de segmentação
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	mov ebp, 0x90000					; ponteiro de 32 bits para base da stack
	mov esp, ebp
	ret
	

.print_real_mode:
	mov esi, MSG 						; Move o início da string para ESI
	mov ebx, 0xb8000					; VGA Text Buffer. Início da memória VGA, cada palavra de 16 bits representa um espaço em tela para preencher com um caractere, a parte inferior contém o caractere e a parte superior a configuração de cor.
.loop:
	lodsb 								; Carrega um byte da memória no endereço armazenado em DS:SI para AL e incrementa SI para apontar para o próximo byte.
	or al, al 							; Operação lógica OR bit a bit para checar se encontra 0.
	jz halt 							; Jump if Zero (JZ) condicional que pula para halt quando 0 for encontrado no anterior.
	or eax, 0x0100 						; Coloca o MSB como 1 em eax garantindo uma palavra de 16 bits no registrador. Os primeiros bytes são as cores de fundo e do texto respectivamente.
	mov word [ebx], ax 					; Move os 16 bits de AX para a memória no endereço apontado por EBX.
	add ebx, 2 							; Incrementa o registrador EBX que contém o endereço de video da bios para o próximo índice. 1 byte para cor e 1 byte para o ascii.
	jmp .loop 							; Volta para o loop.
halt:
	cli 								; Desabilita interrupções.
	hlt 								; Coloca o processador em estado de espera até ser acordado por alguma interrupção, porém, como foi desabilitado, então é encerrado.
MSG: db "SKIRTSHOT OS!",0