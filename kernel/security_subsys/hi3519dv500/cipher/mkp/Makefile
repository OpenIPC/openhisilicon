#*************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

SECURITY_SUBSYS_DIR := $(PWD)
MKP_CFLAGS	+= -DCONFIG_CRYPTO_CHIP_HI3519DV500

# all source file in this module
SRCS:=

# init
SRCS    += crypto_drv_init.c
INIT_FILE   := $(SECURITY_SUBSYS_DIR)/crypto_drv_init.c
INIT_INC	:= $(SECURITY_SUBSYS_DIR)/crypto_drv_init.h

# IRQ
SRCS    += crypto_drv_irq.c

# Common
SRCS    += ../../security_subsys_common/drv_code/crypto_drv_common.c

# crypto_osal
SRCS    += ../../crypto_osal_lib/crypto_osal_lib.c

# DRV
SRCS    += ../../security_subsys_common/drv_code/drv_trng.c
SRCS    += ../../security_subsys_common/drv_code/drv_symc.c
SRCS    += ../../security_subsys_common/drv_code/drv_symc_mac_hard.c
SRCS    += ../../security_subsys_common/drv_code/drv_hash.c
SRCS    += ../../security_subsys_common/drv_code/drv_pbkdf2_hard.c
SRCS    += ../../security_subsys_common/drv_code/drv_pke.c
SRCS    += ../../security_subsys_common/drv_code/drv_pke_ecc.c
SRCS    += ../../security_subsys_common/drv_code/drv_pke_ecc_curve.c
SRCS    += ../../security_subsys_common/drv_code/drv_pke_rsa.c

# KAPI
SRCS    += ../../security_subsys_common/kapi_code/kapi_init.c
SRCS    += ../../security_subsys_common/kapi_code/kapi_trng.c
SRCS    += ../../security_subsys_common/kapi_code/kapi_symc.c
SRCS    += ../../security_subsys_common/kapi_code/kapi_hash.c
SRCS    += ../../security_subsys_common/kapi_code/kapi_pke.c

# Dispatch
SRCS    += ../../security_subsys_common/dispatch_code/crypto_dispatch.c

# TRNG HAL
SRCS    += ../../security_subsys_common/hal_code/trng_v4/hal_trng.c

# SPACC HAL
SRCS    += ../../security_subsys_common/hal_code/spacc_v4/hal_symc.c
SRCS    += ../../security_subsys_common/hal_code/spacc_v4/hal_hash.c

# PKE HAL
SRCS    += ../../security_subsys_common/hal_code/pke_v4/hal_pke.c

# Include
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/include/common_include
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/include/drv_include
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/include/hal_include
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/include/ioctl_include
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/include/kapi_include

MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../..
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../crypto_osal_lib/
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/dispatch_code
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/drv_code
MKP_CFLAGS  += -I$(SECURITY_SUBSYS_DIR)/../../security_subsys_common/kapi_code

# Debug
# MKP_CFLAGS  += -DCRYPTO_LOG_LEVEL=4
# MKP_CFLAGS  += -DCRYPTO_CTR_TRACE_ENABLE
# MKP_CFLAGS  += -DCRYPTO_NORMAL_TRACE_ENABLE
# MKP_CFLAGS  += -DCRYPTO_CCM_TRACE_ENABLE
# MKP_CFLAGS  += -DCRYPTO_GCM_TRACE_ENABLE
# MKP_CFLAGS	+= -DCRYPTO_DISPATCH_FUNC_TRACE_ENABLE
# MKP_CFLAGS	+= -DCRYPTO_KAPI_FUNC_TRACE_ENABLE
# MKP_CFLAGS	+= -DCRYPTO_HAL_FUNC_TRACE_ENABLE
# MKP_CFLAGS	+= -DCRYPTO_SYMC_HAL_NODE_DEBUG_ENABLE
# MKP_CFLAGS	+= -DCRYPTO_DRV_FUNC_TRACE_ENABLE
# MKP_CFLAGS  += -DCRYPTO_MAC_TRACE_ENABLE

MKP_CFLAGS  += -DCRYPTO_CTR_NON_ALIGN_SUPPORT
MKP_CFLAGS  += -DCRYPTO_SYMC_ADDR_NOT_ALIGN_SUPPORT

SRCS_O := $(SRCS)

#*************************************************************************
# release header
INC_FILE    := $(wildcard ../../security_subsys_common/include/common_include/*.h)

#*************************************************************************
TARGET := $(KO_PREFIX)_cipher

#*************************************************************************
# compile linux or liteos
include $(MAKE_DRV_FILE)