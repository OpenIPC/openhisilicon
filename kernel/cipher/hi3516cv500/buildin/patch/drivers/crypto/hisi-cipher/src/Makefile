export CIPHER_SRC_BASE=$(CURDIR)/drivers/crypto/hisi-cipher/src

include $(CIPHER_SRC_BASE)/drv/cipher_v1.0/build.mak

INTER_DRV := hi3559av100

EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/../include
EXTRA_CFLAGS += $(CIPHER_INS)

CIPHER_OBJS += drv/cipher_initdevice.o

EXTRA_CFLAGS += -DCHIP_TYPE_$(INTER_DRV)
EXTRA_CFLAGS += $(CIPHER_CFLAGS)

obj-y += hi_cipher.o
hi_cipher-y += $(CIPHER_OBJS)
