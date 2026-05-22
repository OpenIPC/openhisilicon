CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -O2 -Wall -Wextra

openipc_frame_ts_test: openipc_frame_ts_test.c ../../include/openipc_frame_ts.h
	$(CC) $(CFLAGS) -o $@ openipc_frame_ts_test.c

clean:
	rm -f openipc_frame_ts_test

.PHONY: clean
