# Build the HW pixel-correctness round-trip test.
#
# Targets a cv500/av300 board. Links against libive_neo, libmpi_neo,
# libpthread; libraries come from the per-build firmware staging area
# at $(STAGING_DIR) unless overridden. Default CC matches the
# OpenIPC cv500 toolchain (gnueabi); pass CC=... to override.
#
# Typical usage:
#   make -C libraries/ive_neo/test \
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

test_persp_hog: test_persp_hog.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f test_persp_hog

.PHONY: clean
