LIB_NAME := libsns_sc4336p

ISP_DIR := $(CURDIR)/../../../isp
KO_DIR  := $(CURDIR)/../../../../kernel
COMMON  := $(CURDIR)/../common

override CFLAGS += -fPIC -Wall -O2 \
	-I$(ISP_DIR)/include/hi3516cv6xx \
	-I$(ISP_DIR)/include/hi3516cv6xx/3a \
	-I$(ISP_DIR)/include/hi3516cv6xx/ext_inc \
	-I$(KO_DIR)/include/hi3516cv6xx \
	-I$(KO_DIR)/include/hi3516cv6xx/exp_inc \
	-I$(KO_DIR)/include/hi3516cv6xx/module \
	-I$(COMMON) \
	-I.

SRCS := $(wildcard *.c) $(COMMON)/sensor_common.c
OBJS := $(notdir $(SRCS:%.c=%.o))

all: $(LIB_NAME).so $(LIB_NAME).a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(COMMON)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $@ $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
