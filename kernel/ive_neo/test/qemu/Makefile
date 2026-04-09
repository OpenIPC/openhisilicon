# SPDX-License-Identifier: GPL-2.0
#
# test-ive-ops.c — build the IVE register regression test as a static
# ARM binary and package it into a cpio initramfs suitable for booting
# with qemu-system-arm -M hi3516ev300.
#
# Usage:
#   make                 # builds test-ive-ops and test-ive-ops.cpio
#   make CC=arm-openipc-linux-musleabi-gcc   # override toolchain
#   make clean
#
# CI uses `arm-linux-gnueabi-gcc` (from apt gcc-arm-linux-gnueabi) since
# the test is /dev/mem-direct and doesn't need the musl userspace.
CC        ?= arm-linux-gnueabi-gcc
CFLAGS    ?= -static -O2 -Wall
TEST_BIN  := test-ive-ops
CPIO      := test-ive-ops.cpio

all: $(CPIO)

$(TEST_BIN): test-ive-ops.c
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
