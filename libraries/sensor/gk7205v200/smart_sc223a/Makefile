ifeq ($(CFG_SDK_EXPORT_FLAG),)
    SDK_DIR := $(shell cd $(CURDIR)/../../../../../../../.. && /bin/pwd)
endif

include $(SDK_DIR)/build/base.mk
include $(SDK_DIR)/source/gmp/modules/isp/arch.mk

LIB_NAME := libsns_sc223a

ISP_ROOT := $(GMP_DIR)/modules/isp

SDK_USR_CFLAGS += -I$(GMP_DIR)/include \
		  -I$(GMP_DIR)/include/mpi \
                  -I$(ISP_ROOT)/kernel/include/arch/$(CHIPARCH)/include \
		  -I$(ISP_ROOT)/kernel/arch/$(CHIPARCH)/include \
		  -I$(ISP_ROOT)/kernel/mkp/include \
		  -I$(ISP_ROOT)/kernel/arch/include \
		  -I$(ISP_ROOT)/user/3a/include \
                  -I$(ISP_ROOT)/user/firmware/include \
		  -I$(ISP_ROOT)/user/firmware/drv \
		  -I$(ISP_ROOT)/user/firmware/arch/include \
                  -I$(ISP_ROOT)/user/firmware/arch/$(CHIPARCH)/include \
                  -I$(ISP_ROOT)/ext_inc

ifeq ($(ISP_INI_CONFIG), y)
    SDK_USR_CFLAGS += -DINIFILE_CONFIG_MODE
endif

ifeq ($(CONFIG_JPEGEDCF), y)
    SDK_USR_CFLAGS += -DENABLE_JPEGEDCF
endif

SRCS := $(wildcard *.c)

include $(SDK_DIR)/build/sdk_lib_rules.mk
