#include <stdint.h>
#include "display.h"
#include "memory.h"

void display_clear(uint32_t color)
{
  *((volatile uint32_t *)OFFSET(DEVICE_DISPLAY_START, DISPLAY_CLR)) = color;
}

void display_set_pixel(unsigned int x, unsigned int y, uint32_t color)
{
  volatile uint32_t *start = OFFSET(DEVICE_DISPLAY_START, DISPLAY_PIXEL);

  if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
    start[x + (y * 640)] = color;
  }
}
