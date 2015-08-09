#include "memory.h"

#define DEVICE_SCI_START WORD_ADDR(0x04000040)
#define DEVICE_SCI_END   WORD_ADDR(0x04000044)

#define SCI_TXD  WORD_ADDR(0)
#define SCI_RXD  WORD_ADDR(1)
#define SCI_CFG  WORD_ADDR(2)
#define SCI_FLAG WORD_ADDR(3)

#define SCI_TEN 1
#define SCI_REN 2

void uart_init(void);
void uart_putc(int c);
void uart_puts(char *str);
void uart_put_uint(unsigned int value);
void uart_put_mem(void *p, unsigned int size);
