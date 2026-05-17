# Build against the linux-custom kernel tree from the openhisilicon firmware
# build. KDIR can be overridden if you've built the kernel elsewhere.
KDIR ?= $(HOME)/git/firmware/output-cv500/build/linux-custom
ARCH ?= arm
CROSS_COMPILE ?= arm-openipc-linux-gnueabi-

obj-m += nnie_phys_dump.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

.PHONY: all clean
