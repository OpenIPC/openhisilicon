LIB_NAME := libsns_os04a10

ISP_DIR := $(CURDIR)/../../../isp
KO_DIR  := $(CURDIR)/../../../../kernel

override CFLAGS += -fPIC -Wall -O2 \
	-I$(ISP_DIR)/include/hi3519dv500 \
	-I$(ISP_DIR)/include/hi3519dv500/3a \
	-I$(ISP_DIR)/include/hi3519dv500/ext_inc \
	-I$(KO_DIR)/include/hi3519dv500 \
	-I.

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a
$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)
$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $@ $(OBJS)
clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
