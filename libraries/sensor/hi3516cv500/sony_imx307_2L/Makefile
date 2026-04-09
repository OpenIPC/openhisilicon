LIB_NAME := libsns_imx307_2l

ISP_DIR := $(CURDIR)/../../../isp
KO_DIR := $(CURDIR)/../../../../kernel

override CFLAGS += -fPIC -Wall -O2 \
	-I$(ISP_DIR)/include/hi3516cv500/3a \
	-I$(ISP_DIR)/include/hi3516cv500 \
	-I$(KO_DIR)/include/hi3516cv500 \
	-I$(KO_DIR)/include/hi3516cv500/adapt \
	-I$(ISP_DIR)/include/hi3516cv500 \
	-I.

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
