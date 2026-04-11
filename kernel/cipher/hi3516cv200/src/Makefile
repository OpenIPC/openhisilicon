ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../mpp/Makefile.param
        include $(PARAM_FILE)
endif

ifeq ($(KERNELRELEASE),)
export CIPHER_SRC_BASE=$(PWD)
endif

CONFIG_CIPHER_MULTICIPHER_SUPPORT=y
CONFIG_CIPHER_HASH_SUPPORT=y
CONFIG_CIPHER_RSA_SUPPORT=y
CONFIG_CIPHER_RNG_SUPPORT=y


#KBUILD_EXTRA_SYMBOLS +="$(PWD)/../../../common/mmz/Module.symvers"

EXTRA_CFLAGS += -DCHIP_TYPE_$(HIARCH)

EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/drv
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/drv/cipher
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/drv/hash
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/drv/efuse
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/include
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/../include
EXTRA_CFLAGS += -I$(MMZ_DIR)


obj-m := hi_cipher.o
#hi_cipher-y += ./drv_cipher.o ./hal_cipher.o ./cipher_intf.o ./test_cipher.o
hi_cipher-y += drv/drv_cipher_intf.o drv/efuse/drv_efuse.o drv/efuse/hal_efuse.o
ifeq ($(CONFIG_CIPHER_MULTICIPHER_SUPPORT),y)
EXTRA_CFLAGS += -DCFG_HI_CIPHER_MULTICIPHER_SUPPORT
hi_cipher-y += drv/cipher/drv_cipher.o drv/cipher/hal_cipher.o
endif
ifeq ($(CONFIG_CIPHER_HASH_SUPPORT),y)
EXTRA_CFLAGS += -DCFG_HI_CIPHER_HASH_SUPPORT
hi_cipher-y += drv/hash/drv_hash_intf.o
endif

ifeq ($(CONFIG_CIPHER_RSA_SUPPORT),y)
EXTRA_CFLAGS += -DCFG_HI_CIPHER_RSA_SUPPORT
endif

ifeq ($(CONFIG_CIPHER_RNG_SUPPORT),y)
EXTRA_CFLAGS += -DCFG_HI_CIPHER_RNG_SUPPORT
endif

EXTRA_CFLAGS += -DCHIP_TYPE_$(HIARCH)

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin" "*.so" "*.a"
all: 
	make -C $(LINUX_ROOT) M=$(PWD) modules
	$(CC) $(EXTRA_CFLAGS) -shared -o libhi_cipher.so api/unf_cipher.c api/mpi_cipher.c
	$(CC) $(EXTRA_CFLAGS) -c api/unf_cipher.c api/mpi_cipher.c -Wall
	$(AR) -rv libhi_cipher.a unf_cipher.o mpi_cipher.o
	@cp hi_cipher.ko $(REL_KO)
	
clean: 
	@$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf
