CC	= mist32-elf-gcc
CFLAGS	= -nostdlib -ffreestanding -std=gnu11 -O0 -Wall -I include-freestanding-c99

LIBDIR	= ../lib
STDLIBDIR	= ../stdlib
LIBSRC	= $(LIBDIR)/uart.c $(LIBDIR)/display.c
LIBHEAD	= $(LIBDIR)/uart.h $(LIBDIR)/display.h
LIBGCC = `${CC} -print-libgcc-file-name`

SRC	= uart_test.c
ELF	= uart_test.elf
BIN	= $(ELF:.elf=.bin)

all:	$(BIN)

clean:
	rm -f $(BIN) $(ELF)

.SUFFIXES:	.elf .bin
.elf.bin:
	mist32-elf-objcopy -O binary $< $@

$(BIN):	$(ELF)

$(ELF):	$(SRC) $(LIBSRC) $(LIBHEAD)
	$(CC) $(CFLAGS) -I $(LIBDIR) -I $(STDLIBDIR) -o $@ $(LIBDIR)/start.s $(LIBSRC) $(SRC) $(LIBGCC)
