#include "../drivers/ports.h"
#include "../drivers/video.h"

#define VGA_CTRL_REGISTER 0x3d4     // Porta de controle VGA
#define VGA_DATA_REGISTER 0x3d5     // Porta de dados VGA
#define VGA_OFFSET_HIGH 0x0e        // Byte alto da posição do cursor
#define VGA_OFFSET_LOW 0x0f         // Byte baixo da posição do cursor

// Extern "C" (c++) necessário para corrigir o problema do name mangling, onde o compilador pode alterar o nome para propósitos próprios.
void main() {
    set_cursor(get_screen_offset_from_row_col(0, 0));
    char *scroll = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28Teste\n29";
    print_string(scroll);
    //clear_screen();
}
