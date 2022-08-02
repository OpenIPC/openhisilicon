ifeq ($(CFG_GK_EXPORT_FLAG),)
    SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
endif

include $(SDK_DIR)/build/base.mk

TARGET := adc_test

LIBS := 

INCLUDES := -I$(GMP_DIR)/include \
	    -I$(GMP_DIR)/modules/osal/include 

CFLAGS := $(SDK_USR_CFLAGS) $(LIBS) $(INCLUDES)

SRCS := $(wildcard ./*.c)

OBJS := $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean

all: $(OBJS)
	$(AT)$(CC) -o $(TARGET) $^ $(CFLAGS)

%.o : %.c
	$(AT)$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(AT)rm -rf $(OBJS) $(TARGET)

