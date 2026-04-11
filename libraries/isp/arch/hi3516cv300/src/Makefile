#
# isp firmware Makefile
#

ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../../../../Makefile.param
	include $(PARAM_FILE)
endif

ISP_PATH := $(MPP_PATH)/component/isp
LIB_PATH = $(ISP_PATH)/lib/
OBJ_PATH = $(ISP_PATH)/firmware/src/obj

ifeq ($(MPP_BUILD), y)
EXT_PATH := $(MPP_PATH)/$(EXTDRV)
else
EXT_PATH := $(MPP_PATH)/extdrv
endif

ISP_INC := -I$(ISP_PATH)/include\
           -I$(ISP_PATH)/3a/include\
           -I$(ISP_PATH)/firmware/drv\
           -I$(ISP_PATH)/firmware/vreg\
           -I$(ISP_PATH)/firmware/vreg/arch/$(HIARCH)\
           -I$(ISP_PATH)/firmware/src/algorithms\
           -I$(ISP_PATH)/firmware/src/arch/$(HIARCH)\
           -I$(ISP_PATH)/firmware/src/main\
           -I$(REL_INC) \
           -I$(EXT_PATH)\
           -I$(MPP_PATH)/code/arch/$(HIARCH)/include \
           -I$(MPP_PATH)/code/mkp/include \
           -I$(MPP_PATH)/code/mkp_inc \
           -I$(MPP_PATH)/code/include/inner_inc\
           -I$(MPP_PATH)/code/include

SRC_DIR := $(ISP_PATH)/firmware/src/algorithms\
           $(ISP_PATH)/firmware/src/arch/$(HIARCH)\
           $(ISP_PATH)/firmware/src/main\
           $(ISP_PATH)/firmware/vreg

LIB_A := $(LIB_PATH)/libisp.a
LIB_S := $(LIB_PATH)/libisp.so

CFLAGS += -Wall -fPIC

ifeq ($(HIGDB),HI_GDB)
CFLAGS += -g
endif

CFLAGS  += -O2
CFLAGS  += $(LIBS_CFLAGS)
DFLAGS  := -DEXT_REG -D$(HI_FPGA)

DEPEND_FILE := $(foreach file,$(subst -I, ,$(ISP_INC)), $(wildcard $(file)/*.h))
SRCS_TMP := $(foreach file,$(subst -S, ,$(SRC_DIR)), $(notdir $(wildcard $(file)/*.c)))
SRCS = $(filter-out hi_drv_vreg.c, $(SRCS_TMP))

ifeq ($(CONFIG_JPEGEDCF), y)
     CFLAGS += -D ENABLE_JPEGEDCF
else
     DEL_SRC_C := isp_dcfinfo.c
     SRCS := $(filter-out $(DEL_SRC_C), $(SRCS))
endif

ifeq ($(CONFIG_QUICK_START), y)
CFLAGS += -DCONFIG_QUICK_START
endif

OBJS := $(SRCS:%.c=$(OBJ_PATH)/%.o)
VPATH = $(subst -S, ,$(SRC_DIR))

COMPILE = $(CC) $(CFLAGS) $(DFLAGS) $(ISP_INC) -lm

$(OBJ_PATH)/%.o : %.c $(DEPEND_FILE)
	@(echo "compiling $< ...")
	@[ -e $(LIB_PATH) ] || mkdir $(LIB_PATH)
	@[ -e $(OBJ_PATH) ] || mkdir $(OBJ_PATH)
	@($(COMPILE) -o $@ -c $<)

all:$(OBJS)
	@($(AR) -rcv $(LIB_A) $(OBJS))
	@($(CC) $(LIBS_LD_CFLAGS) -shared -fPIC -o $(LIB_S) $(OBJS))

clean:
	@$(RM) -rf $(LIB_A) $(LIB_S) $(OBJS)
	@$(RM) -rf $(LIB_PATH) $(OBJ_PATH)
