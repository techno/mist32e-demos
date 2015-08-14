#include "keyboard.h"
#include "uart.h"

unsigned int keyboard_get(void)
{
  unsigned int keycode;
  volatile unsigned int dummy;
  unsigned int result = 0;

  while(1) {
    dummy = *(volatile unsigned int *)OFFSET(DEVICE_KEYBOARD_START, KEYBOARD_FLAGR);
    keycode = *(volatile unsigned int *)OFFSET(DEVICE_KEYBOARD_START, KEYBOARD_DATA);
    
    uart_put_uint(keycode);
    if(keycode & 0x100) {
      result = keycode & 0xff;
    }
    else {
      return result;
    }
  }
}
