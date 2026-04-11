#ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../../mpp/Makefile.param
        include $(PARAM_FILE)
#endif

include $(SAMPLE_DIR)/Makefile.param


CFLAGS += -DCHIP_TYPE_$(HIARCH)

SRC_ROOT :=$(PWD)
CFLAGS += -I$(SRC_ROOT)/../include \
		  -I$(SRC_ROOT)/../src \
		  -I$(SRC_ROOT)/../arch/$(INTERDRVVER)\
		  -I$(REL_DIR)/include

ifeq ($(OSTYPE),HuaweiLite)
CFLAGS += -I$(MPP_PATH)/code/init/HuaweiLite
CFLAGS += -I$(SDK_PATH)/drv/interdrv/$(HIARCH)/init/HuaweiLite/
else
SENSOR_LIBS += $(SRC_ROOT)/../src/libhi_cipher.a
endif

TARGET := sample_cipher
SRCS := $(wildcard *.c)

# compile linux or HuaweiLite

#include $(SAMPLE_DIR)/Make.$(OSTYPE)
include Make.$(OSTYPE)
#include Make.$(OSTYPE)

