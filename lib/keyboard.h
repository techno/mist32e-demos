#include "memory.h"

#define DEVICE_KEYBOARD_START WORD_ADDR(0x04000000)
#define DEVICE_KEYBOARD_END   WORD_ADDR(0x04000002)

#define KEYBOARD_FLAGR WORD_ADDR(0)
#define KEYBOARD_DATA WORD_ADDR(1)

unsigned int keyboard_get(void);
