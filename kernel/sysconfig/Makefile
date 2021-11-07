KDIR := $(HOME)/git/openipc-2.1/output/build/linux-4.9.37
obj-m += sys_config.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
