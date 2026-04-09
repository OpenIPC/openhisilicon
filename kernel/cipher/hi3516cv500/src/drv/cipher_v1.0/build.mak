CIPHER_DIR := $(CIPHER_SRC_BASE)

#CFG_KAPI_TEST_SUPPORT=y

CIPHER_CFLAGS += -DAMP_NONSECURE_VERSION

include $(CIPHER_DIR)/drv/cipher_v1.0/drivers/build.mak
include $(CIPHER_DIR)/drv/cipher_v1.0/osal/build.mak
include $(CIPHER_DIR)/drv/cipher_v1.0/compat/build.mak

ifeq ($(CFG_KAPI_TEST_SUPPORT),y)
include $(CIPHER_DIR)/drv/cipher_v1.0/test/build.mak
endif

EXTRA_CFLAGS += $(CIPHER_CFLAGS)
