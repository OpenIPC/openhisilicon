LIB_NAME := libisp

LIB_DIR := $(CURDIR)
KO_DIR := $(CURDIR)/../../kernel/isp

override CFLAGS += -fPIC -DENABLE_JPEGEDCF

# CV500 uses SDK headers exclusively (different type system from EV200)
ifeq ($(CHIPARCH),hi3516cv500)
override CFLAGS += \
	-I$(LIB_DIR)/include/hi3516cv500 \
	-I$(LIB_DIR)/include/hi3516cv500/3a \
	-I$(LIB_DIR)/include/hi3516cv500/3a/adapt \
	-I$(LIB_DIR)/include/hi3516cv500/vreg \
	-I$(LIB_DIR)/src/hi3516cv500/main \
	-I$(LIB_DIR)/arch/hi3516cv500/include \
	-I$(LIB_DIR)/arch/hi3516cv500/include/algorithms/include \
	-I$(KO_DIR)/../include/hi3516cv500 \
	-I$(KO_DIR)/../include/hi3516cv500/adapt \
	-I$(KO_DIR)/../osal/hi3516cv500 \
	-I$(KO_DIR)/arch/hi3516cv500/ext_inc \
	-I$(KO_DIR)/arch/hi3516cv500/include \
	-I$(KO_DIR)/arch/hi3516cv500/mkp/include
else
override CFLAGS += \
	-I$(LIB_DIR)/include \
	-I$(LIB_DIR)/../../include \
	-I$(LIB_DIR)/arch/$(CHIPARCH)/include \
	-I$(KO_DIR)/include \
	-I$(KO_DIR)/../ext_inc \
	-I$(KO_DIR)/mkp/include \
	-I$(KO_DIR)/arch/$(CHIPARCH)/include
endif

SRCS := $(wildcard arch/$(CHIPARCH)/algorithms/*.c)
SRCS += $(wildcard arch/$(CHIPARCH)/hal/*.c)
ifeq ($(CHIPARCH),hi3516cv500)
SRCS += $(wildcard src/hi3516cv500/main/*.c)
SRCS += $(wildcard src/hi3516cv500/main/adapt/*.c)
else
SRCS += $(wildcard src/main/*.c)
endif

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

OBJS := $(SRCS:%.c=%.o) $(ASM_SRCS:%.S=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm $(OBJS) $(LIB_NAME).so
