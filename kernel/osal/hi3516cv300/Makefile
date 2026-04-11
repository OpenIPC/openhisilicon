ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

src = $(PWD)
EXTRA_CFLAGS := -I$(src)
EXTRA_CFLAGS += -I$(src)/himedia
EXTRA_CFLAGS += -I$(src)/../../include/
EXTRA_CFLAGS += -DHICHIP=$(HICHIP)
csrcs = $(wildcard *.c ./himedia/*.c)
oobjs = $(patsubst %.c,%.o,$(csrcs))

obj-m += hi_osal.o
hi_osal-y := osal_vmalloc.o osal_addr.o osal.o osal_atomic.o osal_barrier.o osal_cache.o osal_debug.o osal_device.o osal_interrupt.o osal_math.o osal_mutex.o osal_proc.o osal_schedule.o osal_semaphore.o osal_spinlock.o osal_string.o osal_task.o osal_timer.o osal_wait.o osal_workqueue.o osal_notifier.o osal_platform.o osal_dma.o ./himedia/base.o ./himedia/himedia.o ./mmz/media-mem.o ./mmz/mmz-userdev.o ./mmz/hisi_allocator.o ./mmz/cma_allocator.o
#drval-y:=$(patsubst %.c,%.o,$(objs))

#set kernel path here
#KERNEL_ROOT = /home/w00266574/upstream_4.4-rc5_debug

all:
	@make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL_ROOT) M=$(PWD) modules
	mkdir -p $(REL_KO) && cp hi_osal.ko $(REL_KO)
clean: 
	@make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL_ROOT) M=$(PWD) clean

