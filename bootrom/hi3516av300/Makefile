CROSS_COMPILE ?= arm-none-eabi-
CC      := $(CROSS_COMPILE)gcc
LD      := $(CROSS_COMPILE)ld
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy

CFLAGS  := -mcpu=cortex-a7 -marm -nostdlib -ffreestanding \
           -fno-builtin -fno-stack-protector -fno-pic \
           -Wall -Wextra -Werror -Wno-unused-parameter \
           -O2 -g \
           -I$(CURDIR)/re -I$(CURDIR)/../common

LDFLAGS := -nostdlib -Wl,--build-id=none -T re/re.lds

OBJS := re/start.o re/bootloader.o re/stubs.o

.PHONY: all build disasm verify clean

all: build

build: re.elf

re.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

re/%.o: re/%.c
	$(CC) $(CFLAGS) -c $< -o $@

re/%.o: re/%.S
	$(CC) $(CFLAGS) -c $< -o $@

disasm:
	@../scripts/disasm.sh hi3516av300

verify:
	@../scripts/verify.sh hi3516av300

clean:
	rm -f $(OBJS) re.elf
