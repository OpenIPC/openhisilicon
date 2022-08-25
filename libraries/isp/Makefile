LIB_NAME := libisp

LIB_DIR := $(CURDIR)
KO_DIR := $(CURDIR)/../../kernel/isp

CFLAGS += -fPIC -O3 -DNDEBUG

CFLAGS += -I$(LIB_DIR)/include \
		  -I$(LIB_DIR)/../../include \
		  -I$(LIB_DIR)/arch/$(CHIPARCH)/include \
		  -I$(KO_DIR)/include \
		  -I$(KO_DIR)/ext_inc \
		  -I$(KO_DIR)/mkp/include \
		  -I$(KO_DIR)/arch/$(CHIPARCH)/include

CFLAGS += -DENABLE_JPEGEDCF

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

OBJS := $(SRCS:%.c=%.o) $(ASM_SRCS:%.S=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm $(OBJS) $(LIB_NAME).so
