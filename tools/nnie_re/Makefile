# Tools for NNIE reverse engineering — host build of the ioctl-snoop
# LD_PRELOAD .so. The kernel-side dumper lives in nnie_phys_dump/ and
# has its own Makefile.
CROSS_CC ?= arm-openipc-linux-gnueabi-gcc
CFLAGS  := -O2 -Wall -fPIC

all: libdump_bbox.so

libdump_bbox.so: libdump_bbox.c
	$(CROSS_CC) $(CFLAGS) -shared $< -o $@ -ldl

clean:
	rm -f libdump_bbox.so

.PHONY: all clean
