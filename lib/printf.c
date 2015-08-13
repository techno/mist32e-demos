#include <mini-printf.h>
#include <uart.h>

#define BUFSIZ 1024

int printf(char *fmt, ...)
{
  char buffer[BUFSIZ];
  char *str;
  int ret;
  va_list va;
  va_start(va, fmt);
  ret = mini_vsnprintf(buffer, BUFSIZ, fmt, va);
  va_end(va);

  str = buffer;
  while(*str) {
    uart_putc(*(str++));
  }

  return ret;
}
