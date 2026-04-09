ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../../mpp/Makefile.param
        include $(PARAM_FILE)
endif

ifneq ($(HIARCH),$(filter $(HIARCH), hi3516cv500 hi3516ev200))
include $(SAMPLE_DIR)/Makefile.param
endif

ifeq ($(HI_SUBARCH),hi3569v100)
CFLAGS += -DCHIP_TYPE_$(HI_SUBARCH)
else
CFLAGS += -DCHIP_TYPE_$(HIARCH)
endif

SRC_ROOT :=$(PWD)

INC_FLAGS += -I$(SRC_ROOT)/../include -I./ -I$(SRC_ROOT)/../src -I$(REL_DIR)/include -I$(SRC_ROOT)/../src/api -I$(SRC_ROOT)/../src/drv/cipher_v1.0/osal/include
CFLAGS += -Wall -g $(INC_FLAGS) -lpthread

ifeq ($(OSTYPE),liteos)
CFLAGS += -I$(MPP_PATH)/cbb/init/$(HIARCH)/liteos
else
LIBA := -L$(SRC_ROOT)/../src  -lhi_cipher -L$(REL_DIR)/lib -lsecurec
endif

TARGET := sample
SRCS  := $(wildcard *.c)
HEAD := $(wildcard *.h)

# compile linux or HuaweiLite
include $(OSTYPE).mak
