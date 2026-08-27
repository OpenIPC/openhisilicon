CIPHER_OBJS += drv/cipher_v1.0/compat/hi_drv_compat.o
CIPHER_OBJS += drv/cipher_v1.0/compat/drv_klad.o
CIPHER_OBJS += drv/cipher_v1.0/compat/hal_efuse.o
CIPHER_OBJS += drv/cipher_v1.0/compat/hal_otp.o

CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/compat

