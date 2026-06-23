ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

ifeq ($(arch), )
    arch := arm64
endif

CRYPTO_HARDEN_DIR := $(PWD)
SUBSYS_DTR := $(CRYPTO_HARDEN_DIR)/..

SRCS := reg_aes.o
SRCS += reg_hash.o
SRCS += ot_reg.o

MKP_CFLAGS += -DCONFIG_CRYPTO_CHIP_HI3519DV500
MKP_CFLAGS += -I$(SUBSYS_DTR)/security_subsys_common/include/kapi_include
MKP_CFLAGS += -I$(SUBSYS_DTR)/security_subsys_common/drv_code
MKP_CFLAGS += -I$(SUBSYS_DTR)/security_subsys_common/include/drv_include
MKP_CFLAGS += -I$(SUBSYS_DTR)/security_subsys_common/include/hal_include
MKP_CFLAGS += -I$(SUBSYS_DTR)/security_subsys_common/include/common_include
MKP_CFLAGS += -I$(SUBSYS_DTR)/crypto_osal_lib
MKP_CFLAGS += -I$(SUBSYS_DTR)
MKP_CFLAGS += -I$(SUBSYS_DTR)/../../../osal/include/
MKP_CFLAGS += -I$(SUBSYS_DTR)/../../../bsp/components/secure_c/include/

SRCS_O := $(SRCS)

TARGET := $(KO_PREFIX)_hardware_cryptodev

include $(MAKE_DRV_FILE)