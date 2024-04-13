#include "../drivers/ports.h"
#include "../drivers/video.h"

#define VGA_CTRL_REGISTER 0x3d4     // Porta de controle VGA
#define VGA_DATA_REGISTER 0x3d5     // Porta de dados VGA
#define VGA_OFFSET_HIGH 0x0e        // Byte alto da posição do cursor
#define VGA_OFFSET_LOW 0x0f         // Byte baixo da posição do cursor

// Extern "C" (c++) necessário para corrigir o problema do name mangling, onde o compilador pode alterar o nome para propósitos próprios.
void main() {
    set_cursor(get_screen_offset_from_row_col(0, 0));
    char *hello_world = "Hello world!\nOla mundo!";
    print_string(hello_world);
}
