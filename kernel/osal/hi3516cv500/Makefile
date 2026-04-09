ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

src = $(PWD)
EXTRA_CFLAGS := -I$(src)
EXTRA_CFLAGS += -I$(src)/himedia
EXTRA_CFLAGS += -I$(src)/../../include/
EXTRA_CFLAGS += -DHICHIP=$(HICHIP)
EXTRA_CFLAGS += $(MPP_CFLAGS)
csrcs = $(wildcard *.c ./himedia/*.c)
oobjs = $(patsubst %.c,%.o,$(csrcs))

obj-m += hi_osal.o
hi_osal-y := osal_fileops.o osal_vmalloc.o osal_addr.o osal.o osal_atomic.o osal_barrier.o osal_cache.o osal_debug.o osal_device.o osal_interrupt.o osal_math.o osal_mutex.o osal_proc.o osal_schedule.o osal_semaphore.o osal_spinlock.o osal_string.o osal_task.o osal_timer.o osal_wait.o osal_workqueue.o osal_notifier.o osal_platform.o ./himedia/base.o ./himedia/himedia.o ./mmz/media_mem.o ./mmz/mmz_userdev.o ./mmz/hisi_allocator.o
hi_osal-$(CONFIG_CMA) += ./mmz/cma_allocator.o

all:
	@make -C $(KERNEL_ROOT) M=$(PWD) modules
	@mkdir -p $(REL_KO) && cp hi_osal.ko $(REL_KO)
	@mkdir -p $(REL_INC) && cp $(src)/../../include/* $(REL_INC)
clean: 
	@rm hi_osal.ko -f
	@rm hi_osal.mod.c hi_osal.mod.o hi_osal.o modules.order Module.symvers -f
	@rm $(hi_osal-y) -f
	@rm ./mmz/*.o -f
	@for x in `find . -name "*.mod.c"`;do rm -rf $$x;done
	@for x in `find . -name "*.cmd"`;do rm -rf $$x;done
	@rm .tmp_versions -rf

