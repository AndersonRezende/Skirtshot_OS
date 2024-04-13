#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_HIGH 0x0e
#define VGA_OFFSET_LOW 0x0f

void set_cursor(int offset);
int get_cursor();
void set_char_at_video_memory(char character, int offset);
int get_screen_offset_from_row_col(int row, int col);