CC	= mist32-elf-gcc
CFLAGS	= -nostdlib -ffreestanding -std=gnu11 -O0 -Wall -I include-freestanding-c99

LIBDIR	= ../lib
STDLIBDIR	= ../stdlib
LIBSRC	= $(LIBDIR)/uart.c $(LIBDIR)/display.c $(LIBDIR)/keyboard.c $(LIBDIR)/printf.c ../mini-printf/mini-printf.c $(STDLIBDIR)/stdlib.c
LIBHEAD	= $(LIBDIR)/uart.h $(LIBDIR)/display.h $(LIBDIR)/keyboard.h $(LIBDIR)/printf.h
LIBGCC = `${CC} -print-libgcc-file-name`

SRC	= tetris.c
ELF	= tetris.elf
BIN	= $(ELF:.elf=.bin)

all:	$(BIN)

clean:
	rm -f $(BIN) $(ELF)

.SUFFIXES:	.elf .bin
.elf.bin:
	mist32-elf-objcopy -O binary $< $@

$(BIN):	$(ELF)

$(ELF):	$(SRC) $(LIBSRC) $(LIBHEAD)
	$(CC) $(CFLAGS) -I $(LIBDIR) -I $(STDLIBDIR) -I ../mini-printf -o $@ $(LIBDIR)/start.s $(LIBSRC) $(SRC) $(LIBGCC)
