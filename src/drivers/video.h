#pragma once

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define VGA_CTRL_REGISTER 0x3d4             // Porta de controle VGA
#define VGA_DATA_REGISTER 0x3d5             // Porta de dados VGA
#define VGA_OFFSET_HIGH 0x0e                // Byte alto da posição do cursor
#define VGA_OFFSET_LOW 0x0f                 // Byte baixo da posição do cursor

void set_cursor(int offset);
int get_cursor();
int get_screen_offset_from_row_col(int row, int col);
int get_row_from_offset(int offset);
void set_char_at_video_memory(char character, int offset);
void print_string(char *string);
void print_nl();
void clear_screen();