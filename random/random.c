#include <stdint.h>
#include <display.h>
#include <uart.h>

unsigned short pixels[256][256];

#define HARD_RAND

#ifdef HARD_RAND
#define rand() (hard_rand())

uint32_t hard_rand(void)
{
  static uint32_t y = 2463534242;
  asm volatile("rand %0, %1" : "=r"(y) : "r"(y));
  return y;
}
#else
#define rand() (xorshift())

uint32_t xorshift(void)
{
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;

  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}
#endif

int main(void)
{
  unsigned int x, y;

  display_clear(DISPLAY_COLOR_BLACK);

  for(x = 0; x < 256; x++) {
    for(y = 0; y < 256; y++) {
      pixels[x][y] = 0;
    }
  }
  
  while(1) {
    x = rand() & 0xff;
    y = rand() % 0xff;

    pixels[x][y] |= (1 << (rand() & 0xf));
    display_set_pixel(x + 100, y + 100, pixels[x][y]);
  }

  return 0;
}
