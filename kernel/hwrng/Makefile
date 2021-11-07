ifneq ($(KERNELRELEASE),)
obj-m := hisi-hwrng.o
else
KDIR := $(HOME)/projects/cameras/sdk/Hi3516EV200_SDK_V1.0.1.2/osdrv/opensource/kernel/linux-4.9.37

all:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	-rm *.ko *.o

endif
