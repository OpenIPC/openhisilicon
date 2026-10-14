LIB_NAME := libsns_os02k10

KO_DIR := $(CURDIR)/../../../../kernel

override CFLAGS += -fPIC -Wall -O2 \
	-I$(KO_DIR)/include/gk7206v100 \
	-I.

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $@ $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
