#include "ports.h"
#include "video.h"

    /* O controlador CRT (VGA 0x3d4) usa um único registro de dados (0x3d5).
     * O controlador usa um registro de índice para determinar o tipo
     * de dado no registro de dados.
     * Os tipos de dado 0x0e e 0x0f representam a posição alta e baixa do
     * cursor.
    */
void set_cursor(int offset) {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) offset >> 8);
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) offset);
}

int get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset;
}

int get_screen_offset_from_row_col(int row, int col) {
    return col + row * MAX_COLS;
}

int get_row_from_offset(int offset) {
    return offset / MAX_COLS;
}

int move_offset_to_next_line(int offset) {
    return get_screen_offset_from_row_col( 
        get_row_from_offset(offset) + 1, 0);
}

void set_char_at_video_memory(char character, int offset) {
    short* vga = (short*) VIDEO_ADDRESS;
    vga[offset] = character | 0xf100; 
}

void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0)
    {
        if (string[i] == '\n') {
            offset = move_offset_to_next_line(offset);
        } else {
            set_char_at_video_memory(string[i], offset);
            offset++;
        }
        i++;   
    }
    set_cursor(offset);
}