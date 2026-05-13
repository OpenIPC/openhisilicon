# Build the HW pixel-correctness round-trip test.
#
# Opt-in only: the top-level libraries/Makefile recurses into every
# subdir with a Makefile and runs `make all`; for this test that path
# is wrong (cross-compile against a cv500 firmware staging tree, not
# the in-package sensor build). So `all` is a no-op here, and the
# test is built explicitly via `make test_persp_hog`.
#
# Typical usage:
#   make -C libraries/ive_neo/test test_persp_hog \
#        STAGING=$HOME/git/firmware/output-cv500/build/hisilicon-opensdk-custom \
#        CC=$HOME/git/firmware/output-cv500/host/bin/arm-openipc-linux-gnueabi-gcc
#
# Then scp test_persp_hog to the board and run.

CC      ?= arm-openipc-linux-gnueabi-gcc
STAGING ?= ../../../../firmware/output-cv500/build/hisilicon-opensdk-custom
ROOT    := $(abspath ../../..)

CFLAGS  := -O2 -Wall -Wextra -std=gnu99 \
           -I$(ROOT)/libraries/include \
           -I$(ROOT)/libraries/ive_neo/include \
           -I$(ROOT)/libraries/mpi_neo/include

LDFLAGS := -L$(STAGING)/libraries/ive_neo \
           -L$(STAGING)/libraries/mpi_neo \
           -Wl,-rpath-link,$(STAGING)/libraries/ive_neo \
           -Wl,-rpath-link,$(STAGING)/libraries/mpi_neo

LDLIBS  := -live_neo -lmpi_neo -lpthread

# Default target is a no-op so the package-level recursive build
# doesn't try to cross-compile us with the wrong toolchain config.
all:
	@:

test_persp_hog: test_persp_hog.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f test_persp_hog

.PHONY: all clean
