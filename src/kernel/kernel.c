#include "../drivers/ports.h"
#include "../drivers/video.h"

#define VGA_CTRL_REGISTER 0x3d4     // Porta de controle VGA
#define VGA_DATA_REGISTER 0x3d5     // Porta de dados VGA
#define VGA_OFFSET_HIGH 0x0e        // Byte alto da posição do cursor
#define VGA_OFFSET_LOW 0x0f         // Byte baixo da posição do cursor

void print_s(int x) {
    const short color = 0xf100;             // Dois primeiros bytes para cor do texto e de fundo
	const char* hello = "Hello cpp world!";
	short* vga = (short*)0xb8000;

    /* Modo 80x25 (linhas x colunas)
     * Linha 0, coluna 0 = 0
     * Linha 1, coluna 0 = 80
     * Imprime o caractere na segunda linha na coluna i
    */
	for (int i = 0; i < 16; i++) {
		vga[i + 80] = color | hello[i];     // 0xf100 ou 1 Caractere = 0xf1(caractere) 
	}
}

// Extern "C" (c++) necessário para corrigir o problema do name mangling, onde o compilador pode alterar o nome para propósitos próprios.
void main() {
    set_cursor(get_screen_offset_from_row_col(2, 5));
    set_char_at_video_memory((char) 'A', (int) 0);
    set_char_at_video_memory((char) 'N', (int) 1);
    print_s(30);

    char *hello_world = "Hello world!";
    print_string(hello_world);
}
