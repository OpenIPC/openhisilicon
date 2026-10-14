# libxmedia_msensor.so -- userspace half of the IMU (msensor) driver, from the
# GK7206 SDK's source/gmp/usr/ext/msensor. Replaces the SDK's uClibc build.
LIB_NAME := libxmedia_msensor

KO_DIR := $(CURDIR)/../../../kernel

override CFLAGS += -fPIC -Wall -O2 \
	-I$(KO_DIR)/include/gk7206v100 \
	-I$(KO_DIR)/msensor/gk7206v100/include \
	-I$(KO_DIR)/msensor/gk7206v100/msensor_mng/include

SRCS := xmedia_msensor.c
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $@ $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
