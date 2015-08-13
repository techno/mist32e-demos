#include <printf.h>
#include <uart.h>

int main(void)
{
  uart_init();
  uart_puts ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  uart_putc ('\n');
  uart_puts ("Program compiled with 'register' attribute\n");
  uart_putc ('\n');
  uart_puts ("Program compiled without 'register' attribute\n");
  uart_putc ('\n');
  uart_puts ("Please give the number of runs through the benchmark: ");
  uart_putc ('\n');
  uart_puts ("Execution starts, %d runs through Dhrystone\n");
  
  printf ("\n");
  printf ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  printf ("\n");
  printf ("Program compiled with 'register' attribute\n");
  printf ("\n");
  printf ("Program compiled without 'register' attribute\n");
  printf ("\n");
  printf ("Please give the number of runs through the benchmark: ");
  printf ("\n");
  printf ("Execution starts, %d runs through Dhrystone\n", 100000);
  uart_puts ("End\n");

  return 0;
}
