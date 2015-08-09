#include <stdint.h>
#include <uart.h>
#include <display.h>

#define N_VALUES 1024

unsigned int values[N_VALUES];

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

void q_sort(unsigned int numbers[], int left, int right)
{
  int pivot, l_hold, r_hold;
  
  l_hold = left;
  r_hold = right;
  pivot = numbers[left];
  
  while (left < right) {
    while ((numbers[right] >= pivot) && (left < right))
      right--;

    if (left != right) {
      numbers[left] = numbers[right];
      left++;
    }

    while ((numbers[left] <= pivot) && (left < right))
      left++;
    
    if (left != right) {
      numbers[right] = numbers[left];
      right--;
    }
  }
  
  numbers[left] = pivot;
  pivot = left;
  left = l_hold;
  right = r_hold;
  
  if (left < pivot)
    q_sort(numbers, left, pivot-1);
  if (right > pivot)
    q_sort(numbers, pivot+1, right);
}

int main(void)
{
  unsigned int i;

  display_clear(DISPLAY_COLOR_BLACK);
  uart_init();
  display_clear(DISPLAY_COLOR_WHITE);

  for(i = 0; i < N_VALUES; i++) {
    values[i] = xor128();
    uart_put_uint(values[i]);
  }

  display_clear(DISPLAY_COLOR_RED);
  
  q_sort(values, 0, N_VALUES - 1);

  display_clear(DISPLAY_COLOR_BLUE);
  
  for(i = 0; i < N_VALUES; i++) {
    uart_put_uint(values[i]);
  }

  display_clear(DISPLAY_COLOR_GREEN);
  
  return 0;
}
