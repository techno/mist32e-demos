#define MEMORY_MAX_ADDR 0x40000
#define STACK_SIZE 0x400

#define OFFSET(ptr, offset) ((void *)((char *)ptr + offset))
#define WORD_ADDR(addr) (addr << 2)
