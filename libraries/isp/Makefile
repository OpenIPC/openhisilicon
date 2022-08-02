ifeq ($(CFG_SDK_EXPORT_FLAG),)
    SDK_DIR := $(shell cd $(CURDIR)/../../../../../.. && /bin/pwd)
endif

include $(SDK_DIR)/build/base.mk
include $(SDK_DIR)/source/gmp/modules/isp/arch.mk

LIB_NAME := libisp

ISP_ROOT := $(GMP_DIR)/modules/isp

SDK_USR_CFLAGS += -I$(GMP_DIR)/include \
		  -I$(ISP_ROOT)/include \
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

SDK_USR_CFLAGS += -DENABLE_JPEGEDCF

SRCS := $(wildcard arch/$(CHIPARCH)/algorithms/*.c)
SRCS += $(wildcard arch/$(CHIPARCH)/hal/*.c)
SRCS += $(wildcard src/main/*.c)

ifneq ($(CONFIG_ISP_AF_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_af.c, $(SRCS))
endif

ifneq ($(CONFIG_ISP_CR_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_green_equalization.c, $(SRCS))
endif

ifneq ($(CONFIG_ISP_PREGAMMA_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_pregamma.c, $(SRCS))
endif

ifneq ($(CONFIG_ISP_GCAC_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_gcac.c, $(SRCS))
endif

ifneq ($(CONFIG_ISP_CA_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_ca.c, $(SRCS))
endif

ifneq ($(CONFIG_ISP_EDGEMARK_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_edgemark.c, $(SRCS))
endif

ifneq ($(CONFIG_ISP_HLC_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_hlc.c, $(SRCS))
endif

ifneq ($(CONFIG_VI_FPN_SUPPORT), y)
     SRCS := $(filter-out arch/$(CHIPARCH)/algorithms/isp_fpn.c, $(SRCS))
endif

THIS_LIB_STATIC_INSTALL_DIR := $(SDK_STATIC_LIB_DIR)/base
THIS_LIB_SHARE_INSTALL_DIR := $(SDK_SHARE_LIB_DIR)/base

include $(SDK_DIR)/build/sdk_lib_rules.mk
