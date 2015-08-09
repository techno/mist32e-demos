#include <display.h>

int main(void)
{
  unsigned int x, y;

  while(1) {
    display_clear(DISPLAY_COLOR_WHITE);

    for(x = 0; x < DISPLAY_WIDTH; x++) {
      for(y = 0; y < DISPLAY_HEIGHT; y++) {
	display_set_pixel(x, y, DISPLAY_COLOR_BLUE);
      }
    }
  }

  return 0;
}
