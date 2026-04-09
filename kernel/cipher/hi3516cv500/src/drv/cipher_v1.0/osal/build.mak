CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/osal/include

ifeq ($(OSTYPE),liteos)
CIPHER_OBJS   += drv/cipher_v1.0/osal/drv_osal_init_liteos.o
CIPHER_OBJS   += drv/cipher_v1.0/osal/drv_osal_sys_liteos.o
else
CIPHER_OBJS   += drv/cipher_v1.0/osal/drv_osal_init_linux.o
CIPHER_OBJS   += drv/cipher_v1.0/osal/drv_osal_sys_linux.o
endif

