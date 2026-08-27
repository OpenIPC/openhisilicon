

ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../../mpp/Makefile.param
        include $(PARAM_FILE)
endif

ifeq ($(KERNELRELEASE),)
export CIPHER_SRC_BASE=$(PWD)
endif

include $(CIPHER_SRC_BASE)/api/cipher_v1.0/build.mak
include $(CIPHER_SRC_BASE)/drv/cipher_v1.0/build.mak

KBUILD_EXTRA_SYMBOLS +="$(OSAL_ROOT)/$(OSTYPE)/kernel/Module.symvers"
KBUILD_EXTRA_SYMBOLS +="$(MPP_PATH)/cbb/base/Module.symvers"

EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/include
EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/../include

EXTRA_CFLAGS += -I$(CIPHER_SRC_BASE)/../arch/$(INTER_DRV)
EXTRA_CFLAGS += -I$(OSAL_ROOT)/include
EXTRA_CFLAGS += -I$(REL_DIR)/include
EXTRA_CFLAGS += $(CIPHER_INS)

DRV_OBJ := $(CIPHER_OBJS)

EXTRA_CFLAGS += -DCHIP_TYPE_$(INTER_DRV)
EXTRA_CFLAGS += $(CIPHER_CFLAGS)
EXTRA_CFLAGS += $(DRV_CFLAGS)
EXTRA_CFLAGS += -DARCH_TYPE_$(ARM_ARCH)

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

ifeq ($(OSTYPE),liteos)
EXTRA_CFLAGS += $(CFLAGS)
endif

DRV_OBJ += drv/cipher_initdevice.o

API_SRCS := api/cipher_v1.0/unf_cipher.c      \
              api/cipher_v1.0/mpi_cipher.c    \
              api/cipher_v1.0/sys_cipher.c    \
              api/cipher_v1.0/user_osal_lib.c

API_OBJS := $(CIPHER_OBJ)

OBJS = $(DRV_OBJ) $(API_OBJS)

TARGET := hi_cipher
obj-m := hi_cipher.o
hi_cipher-y += $(DRV_OBJ)

LIBS_LD_CFLAGS +=-L$(REL_DIR)/lib
LIBS_CFLAGS += -lsecurec
#*************************************************************************
# all source file in this module
SRCS   := $(patsubst %.o,%.c,$(OBJ))

.PHONY: all clean

all: $(OSTYPE)_build

clean :
	@$(AT)find -L ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf
	@rm -f  libhi_cipher.a libhi_cipher.so
	@for x in `find $(REL_DIR)/init -name "*cipher_init.o.cmd"`;do rm -rf $$x;done
	@for x in `find $(REL_DIR)/init -name "*cipher_init.o*"`;do rm -rf $$x;done
###################################
liteos_build: $(OBJS)
	@$(AR) $(ARFLAGS) lib$(TARGET).a $(OBJS)
	@mkdir -p $(REL_KO) && cp -rf libhi_cipher.a $(REL_KO)
	@cp $(CIPHER_SRC_BASE)/../include/hi_unf_cipher.h $(REL_INC)
	@cp $(CIPHER_SRC_BASE)/../include/hi_common_cipher.h $(REL_INC)
	@cp $(CIPHER_SRC_BASE)/../include/hi_types.h $(REL_INC)

linux_build: $(API_OBJS)
	@echo -e "\e[0;32;1m... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- HIARCH=$(HIARCH), HICHIP=$(HICHIP), CVER=$(CVER), DBG=$(HIDBG), HI_FPGA=$(HI_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@echo -e "\e[0m"
	@echo $(EXTRA_CFLAGS)
	@make -C $(KERNEL_ROOT) M=$(PWD) modules
	@mkdir -p $(REL_KO) && cp -rf $(TARGET).ko $(REL_KO)
	@$(CC) $(EXTRA_CFLAGS) $(LIBS_LD_CFLAGS) $(LIBS_CFLAGS) -shared -fPIC -o libhi_cipher.so $(API_SRCS)
	@$(AR) -rv libhi_cipher.a $(API_OBJS)
	@cp $(CIPHER_SRC_BASE)/../include/hi_unf_cipher.h $(REL_INC)
	@cp $(CIPHER_SRC_BASE)/../include/hi_common_cipher.h $(REL_INC)
	@cp $(CIPHER_SRC_BASE)/../include/hi_types.h $(REL_INC)
	@mkdir -p $(REL_LIB) && cp libhi_cipher.a libhi_cipher.so $(REL_LIB)

$(OBJS): %.o : %.c
	@echo $(CC) $< ...
	@$(CC) $(EXTRA_CFLAGS) $(LIBS_CFLAGS) -c $< -shared -fPIC -o $@
