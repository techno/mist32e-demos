#include "uart.h"

void uart_init(void)
{
  *(volatile unsigned int *)OFFSET(DEVICE_SCI_START, SCI_CFG) = SCI_TEN | SCI_REN;
}

void uart_putc(int c)
{
  *(volatile unsigned int *)OFFSET(DEVICE_SCI_START, SCI_TXD) = c;
}

void uart_puts(char *str)
{
  while(*str) {
    uart_putc(*(str++));
  }
  uart_putc('\n');
}

void uart_put_uint(unsigned int value)
{
  int i;
  unsigned char c;

  uart_putc('0');
  uart_putc('x');

  for(i = 0; i < 8; i++) {
    c = (value & 0xf0000000) >> 28;
    uart_putc((c < 10 ? '0' : 'A' - 10) + c);
    value = (value & 0x0fffffff) << 4;
  }

  uart_putc('\n');
}

void uart_put_mem(void *p, unsigned int size)
{
  char c, h, l;
  unsigned int i;

  for(i = 0; i < size; i++) {
    if(!(i % 16) && i > 0) {
      uart_putc('\n');
    }

    c = *((char *)p + i);
    h = (c >> 4) & 0xf;
    l = c & 0xf;

    uart_putc((h < 10 ? '0' : 'A' - 10) + h);
    uart_putc((l < 10 ? '0' : 'A' - 10) + l);
    uart_putc(' ');
  }
  uart_putc('\n');
}
