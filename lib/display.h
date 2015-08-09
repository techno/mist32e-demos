#include <stdint.h>

#define DEVICE_DISPLAY_START WORD_ADDR(0x040000c0)
#define DEVICE_DISPLAY_END   WORD_ADDR(0x0404b1c0)

#define DISPLAY_CLR   WORD_ADDR(0x00000000)
#define DISPLAY_PIXEL WORD_ADDR(0x00000100)

#define DISPLAY_WIDTH  640
#define DISPLAY_HEIGHT 480

#define DISPLAY_COLOR_WHITE 0xffff
#define DISPLAY_COLOR_BLACK 0
#define DISPLAY_COLOR_RED   0xf800
#define DISPLAY_COLOR_GREEN 0x07e0
#define DISPLAY_COLOR_BLUE  0x001f

void display_clear(uint32_t color);
void display_set_pixel(unsigned int x, unsigned int y, uint32_t color);
