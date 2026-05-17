# Build test-nnie-forward.c as a static ARM binary and pack it into
# a cpio initramfs suitable for booting under qemu-system-arm.
#
# Usage:
#   make                     # builds test-nnie-forward and .cpio
#   make CC=<other-gcc>      # override toolchain
#   make clean
#
# CI uses arm-linux-gnueabi-gcc (from apt gcc-arm-linux-gnueabi); the
# test is /dev/mem-direct so it doesn't need a musl userspace.

CC        ?= arm-linux-gnueabi-gcc
CFLAGS    ?= -static -O2 -Wall
TEST_BIN  := test-nnie-forward
CPIO      := test-nnie-forward.cpio

all: $(CPIO)

$(TEST_BIN): test-nnie-forward.c
	$(CC) $(CFLAGS) -o $@ $<

$(CPIO): $(TEST_BIN)
	@rm -rf initramfs
	@mkdir -p initramfs
	@cp $(TEST_BIN) initramfs/init
	@(cd initramfs && echo init | cpio -o -H newc --quiet) > $@
	@echo "built $(CPIO) ($$(stat -c%s $(CPIO)) bytes)"

clean:
	rm -rf $(TEST_BIN) $(CPIO) initramfs

.PHONY: all clean
