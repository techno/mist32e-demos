#include <stdint.h>
#include <display.h>

unsigned short pixels[256][256];

uint32_t xor128(void) {
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;

  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

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
    x = xor128() & 0xff;
    y = xor128() % 0xff;

    pixels[x][y] |= (1 << (xor128() & 0xf));
    display_set_pixel(x + 100, y + 100, pixels[x][y]);
  }

  return 0;
}
