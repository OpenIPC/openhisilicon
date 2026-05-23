CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -O2 -Wall -Wextra

openipc_event_trigger: openipc_event_trigger.c ../../include/openipc_frame_ts.h
	$(CC) $(CFLAGS) -o $@ openipc_event_trigger.c

clean:
	rm -f openipc_event_trigger

.PHONY: clean
