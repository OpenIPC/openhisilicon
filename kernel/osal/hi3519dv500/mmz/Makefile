ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../Makefile.param
    include $(PARAM_FILE)
endif

EXTRA_CFLAGS := -I$(PWD)
EXTRA_CFLAGS += -I$(PWD)/../../include/
EXTRA_CFLAGS += -I$(PWD)/../../mkp/include/

ifeq ($(CONFIG_OT_SYS_SMMU_SUPPORT),y)
EXTRA_CFLAGS += -DCONFIG_OT_SYS_SMMU_SUPPORT
endif

SRCS := media_mem.c mmz_userdev.c ot_allocator.c mmz_init.c
SRCS_O := $(SRCS)

TARGET   := $(KO_PREFIX)_mmz

$(TARGET)-$(CONFIG_CMA) += cma_allocator.o

include $(MAKE_DRV_FILE)
