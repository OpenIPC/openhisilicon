# libxmedia_npu: open-source drop-in for the XMedia SDK's libxmedia_npu.so
# (gk7205v500 family), serving the vendor's libxmedia_cl.so and SDK
# samples over /dev/npu.0 (kernel/npu/gk7205v500).

LIB_NAME := libxmedia_npu

LIB_DIR := $(CURDIR)
TOP := $(LIB_DIR)/../..

override CFLAGS += \
	-fPIC \
	-Wall \
	-D_GNU_SOURCE \
	-I$(TOP)/kernel/include/gk7205v500 \
	-I$(TOP)/kernel/npu/gk7205v500

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -Wl,-soname,$(LIB_NAME).so -o $@ $(OBJS) -lpthread

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a

.PHONY: all clean
