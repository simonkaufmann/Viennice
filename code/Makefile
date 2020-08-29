SRCS = os.asm init.c print.c pci.c util.h string.c ps2.c pic.c interrupt.c gdt.c
OBJS = $(addsuffix .o,$(basename $(SRCS)))

# If using system gcc:
LD = ld
AS = as
CC = gcc

LDFLAGS = -melf_i386 -Ttext=0x100000
ASFLAGS = --32 # Generate output for i386!
CCFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc -g

#CCFLAGS += -DDEBUG_PRINT_SCANCODE

all: kernel

%.o: %.asm
	$(AS) $(ASFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $^ -c  -o $@

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) kernel

run: all
	../scripts/copy &
