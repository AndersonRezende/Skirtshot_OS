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
    /* Converte a posição de linha x coluna para deslocamento*/
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
    /* Como Short = 2 Bytes e Char = 1 Byte
    * Ao utilizar o short é necessário definir a coloração seguido do caractere
    * Caso fosse utilizado 1 Byte para representar o endereço de memória, 
    * seria necessário preencher o primeiro byte com a coloração e o byte seguinte
    * com o caractere desejado.
    */
    short *vga = (short*) VIDEO_ADDRESS;
    vga[offset] = character | 0x0100; 
    //vga[offset] = character | 0xf100; 
}

void memory_copy(short *source, short *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
        //dest[i] = source[i];
    }
}

int scroll_ln(int offset) {
    /* Pega a posição (offset) da segunda linha em diate, coloca os dados na
    * posição da linha anterior. Como cada posição representa um short (2 bytes)
    * sendo um para a coloração e outro para o caractere, multiplica por 2 para 
    * obter os 2 bytes e pular para a próxima posição
    */
    memory_copy(
        (short *) (get_screen_offset_from_row_col(1, 0) * 2 + VIDEO_ADDRESS),
        (short *) (get_screen_offset_from_row_col(0, 0) * 2 + VIDEO_ADDRESS), 
        MAX_COLS * (MAX_ROWS - 1)
    );

    /* Obtém as linhas scrolladas e limpa os caracteres que estavam preenchidos*/
    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(
            ' ', 
            get_screen_offset_from_row_col(MAX_ROWS - 1, col));
    }
    return MAX_COLS * MAX_ROWS - MAX_COLS;
}

void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        set_char_at_video_memory(' ', i);
    }
    set_cursor(0);
}

void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0)
    {
        if (offset > MAX_COLS * MAX_ROWS - 1) {
            offset = scroll_ln(offset);
        }
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