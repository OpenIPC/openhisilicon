#*************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

KM_MKP_DIR := $(PWD)
export CONFIG_DRIVER_KM_SUPPORT=y

# all source file in this module
SRCS:=

# add include files
MKP_CFLAGS  +=  -I$(KM_MKP_DIR) -I$(KM_MKP_DIR)/../../crypto_osal_lib \
    -I$(KM_MKP_DIR)/../../security_subsys_common/include/common_include \
    -I$(KM_MKP_DIR)/../../security_subsys_common/include/drv_include \
    -I$(KM_MKP_DIR)/../../security_subsys_common/include/ioctl_include \
    -I$(KM_MKP_DIR)/../../security_subsys_common/include/kapi_include \
    -I$(KM_MKP_DIR)/../../security_subsys_common/include/hal_include \
    -I$(KM_MKP_DIR)/../../security_subsys_common/hal_code/km_v4 \
    -I$(KM_MKP_DIR)/../../security_subsys_common/hal_code/trng_v4 \
    -I$(KM_MKP_DIR)/../../security_subsys_common/dispatch_code \
    -I$(KM_MKP_DIR)/../../security_subsys_common/drv_code \
    -I$(KM_MKP_DIR)/../../cipher/mkp -I$(KM_MKP_DIR)/../../

# add compiling macros
MKP_CFLAGS  +=  -DCONFIG_CRYPTO_CHIP_HI3519DV500

INIT_FILE   := $(KM_MKP_DIR)/crypto_km_drv_init.c
INIT_INC	:= $(KM_MKP_DIR)/init_km.h

MKP_SRCS	:= crypto_km_drv_init.c init_km.c \
                ../../security_subsys_common/dispatch_code/dispatch_km.c \
                ../../security_subsys_common/drv_code/drv_klad.c \
                ../../security_subsys_common/drv_code/drv_keyslot.c \
                ../../security_subsys_common/hal_code/km_v4/hal_rkp.c \
                ../../security_subsys_common/hal_code/km_v4/hal_klad.c \
                ../../security_subsys_common/hal_code/km_v4/hal_keyslot.c \
                ../../security_subsys_common/kapi_code/kapi_km.c

DRIVER_OBJS-y	+= $(patsubst %.c, %.o, $(MKP_SRCS))

SRCS	    += $(MKP_SRCS)

# Debug
# MKP_CFLAGS      += -DCRYPTO_HAL_FUNC_TRACE_ENABLE
# MKP_CFLAGS      += -DCRYPTO_KAPI_FUNC_TRACE_ENABLE
# MKP_CFLAGS      += -DCRYPTO_DRV_FUNC_TRACE_ENABLE
# MKP_CFLAGS      += -DCRYPTO_DISPATCH_FUNC_TRACE_ENABLE

SRCS_O := $(SRCS)

#*************************************************************************
TARGET := $(KO_PREFIX)_km

#*************************************************************************

include $(MAKE_DRV_FILE)