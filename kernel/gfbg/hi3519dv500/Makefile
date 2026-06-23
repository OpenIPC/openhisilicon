# $(PWD)/gfbg/Makefile

ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../Makefile.param
    include $(CBB_PARAM_FILE)
endif

ifeq ($(CONFIG_OT_GFBG_SUPPORT), y)
# Head file path variable's
CFG_INC := -I$(PWD)/include
CFG_INC += -I$(PWD)/ext_inc
CFG_INC += -I$(PWD)/src
CFG_INC += -I$(PWD)/src/init
CFG_INC += -I$(PWD)/drv/adp
CFG_INC += -I$(PWD)/drv/include
CFG_INC += -I$(TDE_ROOT)/drv/include

ifeq ($(CONFIG_SYNC_FILE),y)
CFG_INC += -I$(KERNEL_ROOT)/drivers/ot_fence
EXTRA_CFLAGS += -DCONFIG_GFBG_FENCE_SUPPORT
endif

ifneq ($(findstring $(OT_ARCH), hi3519dv500),)
EXTRA_CFLAGS += -DCONFIG_GFBG_G0_FHD_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_LOW_DELAY_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_RESOLUTION_SUPPORT_SQUARE
EXTRA_CFLAGS += -DCONFIG_TDE_CLUT_RECT_SUPPORT_G0
EXTRA_CFLAGS += -DCONFIG_TDE_GFBG_COMPRESS_V2
EXTRA_CFLAGS += -DCONFIG_TDE_CLUT_RECT_V2
endif

ifeq ($(findstring $(OT_ARCH), hi3519dv500),)
EXTRA_CFLAGS += -DCONFIG_GFBG_NOT_SUPPORT_FUNC
endif

EXTRA_CFLAGS += -DCONFIG_COMPRESS_ECONOMIZE_MEMERY

EXTRA_CFLAGS += $(CFG_INC)

SRCS :=
include $(PWD)/src/Makefile
include $(PWD)/drv/adp/Makefile

TARGET:=gfbg

#*************************************************************************
# release header
INC_FILE := $(wildcard $(GFBG_ROOT)/include/*.h)

# compile linux or liteos
include $(MAKE_DRV_FILE)

#CONFIG_OT_GFBG_SUPPORT endif
else
all:
	@echo "gfbg module not compile"
clean:
	@echo "gfbg module don't need clean"
endif
