# Host-runnable LoadModel sanity test.
#
# Pure-userspace: parses a synthesised .wk file in memory and asserts
# every decoded field. Doesn't open /dev/nnie. Builds with the system
# gcc on whatever host the CI / developer is on.
#
# Like libraries/ive_neo/test, default `all` is a no-op so the
# package-level recursive build doesn't try to cross-compile this
# host test for the target.
#
# Usage:
#   make -C libraries/nnie_neo/test test_loadmodel
#   ./libraries/nnie_neo/test/test_loadmodel

CC      ?= gcc
ROOT    := $(abspath ../../..)

CFLAGS  := -O2 -Wall -Wextra -Werror -std=gnu99 \
           -I$(ROOT)/libraries/include \
           -I$(ROOT)/libraries/nnie_neo/include

SRCS := test_loadmodel.c $(ROOT)/libraries/nnie_neo/src/nnie_ops.c

all:
	@:

test_loadmodel: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) -lpthread

run: test_loadmodel
	./test_loadmodel

clean:
	rm -f test_loadmodel

.PHONY: all run clean
