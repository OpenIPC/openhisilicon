# openhisilicon

Opensource Hisilicon SoCs SDK



## hisi-hwrng

```
ifneq ($(KERNELRELEASE),)
obj-m := hisi-hwrng.o
else
KDIR := $(HOME)/projects/cameras/sdk/Hi3516EV200_SDK_V1.0.1.2/osdrv/opensource/kernel/linux-4.9.37

all:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	-rm *.ko *.o

endif

```

```
make ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi- -j$(nproc)
```



## sys_config

```
KDIR := $(HOME)/git/openipc-2.1/output/build/linux-4.9.37
obj-m += sys_config.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
```

```
make ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi- -j$(nproc)
````


