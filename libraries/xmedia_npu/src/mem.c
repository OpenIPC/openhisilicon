// SPDX-License-Identifier: GPL-2.0
/*
 * NPU memory for libxmedia_npu. Every buffer the NPU reads or writes
 * (command stream, weights, feature maps) is an MMZ block mapped uncached
 * into the process; "uncache memory" in the SDK's names. The pool size the
 * library reports is the driver's memsize parameter.
 */

#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "npu_ioctl.h"
#include "npu_priv.h"

/* /dev/mmz_userdev ABI (osal_mmz.h, 32-bit) */
struct mmb_info {
	uint32_t phys_addr;
	uint32_t align;
	uint32_t size;
	uint32_t order;
	uint32_t mapped;
	uint32_t prot : 8;
	uint32_t flags : 12;
	uint32_t : 12;
	char mmb_name[16];
	char mmz_name[32];
	uint32_t gfp;
};

_Static_assert(sizeof(struct mmb_info) == 76, "mmb_info is 0x4c bytes");

#define IOC_MMB_ALLOC		_IOWR('m', 10, struct mmb_info)
#define IOC_MMB_FREE		_IOW('m', 12, struct mmb_info)
#define IOC_MMB_USER_REMAP	_IOWR('m', 20, struct mmb_info)
#define IOC_MMB_USER_UNMAP	_IOWR('m', 22, struct mmb_info)
#define IOC_MMB_VIRT_GET_PHYS	_IOWR('m', 23, struct mmb_info)

#define MMZ_PROT_RW		3	/* PROT_READ | PROT_WRITE */
#define MMZ_MAP_SHARED		1
#define MMZ_ALIGN		16

static int mmz_fd = -1;
static pthread_mutex_t mmz_lock = PTHREAD_MUTEX_INITIALIZER;

/* Caller holds mmz_lock */
static int mmz_open(void)
{
	if (mmz_fd < 0)
		mmz_fd = open("/dev/mmz_userdev", O_RDWR | O_SYNC | O_CLOEXEC);
	if (mmz_fd < 0)
		xmedia_printf("Open dev/mmz_userdev error");
	return mmz_fd;
}

/*
 * Allocates @size bytes of MMZ named @mmb (from zone @mmz, or the default
 * one) and maps them uncached. The SDK hands the physical address back as
 * 64 bits.
 */
int xmedia_sys_mmzalloc(uint32_t *phys, uint32_t *virt, const char *mmb,
			const char *mmz, uint32_t size)
{
	struct mmb_info info;
	int ret;

	memset(&info, 0, sizeof(info));
	pthread_mutex_lock(&mmz_lock);
	if (mmz_open() < 0) {
		pthread_mutex_unlock(&mmz_lock);
		return -1;
	}
	info.size = size;
	info.align = MMZ_ALIGN;
	info.prot = MMZ_PROT_RW;
	info.flags = MMZ_MAP_SHARED;
	if (mmb)
		strncpy(info.mmb_name, mmb, sizeof(info.mmb_name) - 1);
	if (mmz)
		strncpy(info.mmz_name, mmz, sizeof(info.mmz_name) - 1);

	ret = ioctl(mmz_fd, IOC_MMB_ALLOC, &info);
	if (ret) {
		xmedia_printf("System alloc mmz memory failed!\n");
	} else {
		ret = ioctl(mmz_fd, IOC_MMB_USER_REMAP, &info);
		if (ret) {
			xmedia_printf("System remap mmz memory failed!\n");
		} else {
			phys[0] = info.phys_addr;
			phys[1] = 0;
			*virt = info.mapped;
		}
	}
	pthread_mutex_unlock(&mmz_lock);
	return ret;
}

int xmedia_sys_mmzfree(uint32_t phys)
{
	struct mmb_info info;
	int ret;

	memset(&info, 0, sizeof(info));
	pthread_mutex_lock(&mmz_lock);
	if (mmz_open() < 0) {
		pthread_mutex_unlock(&mmz_lock);
		return -1;
	}
	info.phys_addr = phys;
	ret = ioctl(mmz_fd, IOC_MMB_USER_UNMAP, &info);
	if (ret) {
		xmedia_printf("System unmap mmz memory failed!\n");
	} else {
		ret = ioctl(mmz_fd, IOC_MMB_FREE, &info);
		if (ret)
			xmedia_printf("System free mmz memory failed!\n");
	}
	pthread_mutex_unlock(&mmz_lock);
	return ret;
}

int xmedia_sys_getphy(uint32_t virt, uint32_t *phys)
{
	struct mmb_info info;
	int ret;

	memset(&info, 0, sizeof(info));
	pthread_mutex_lock(&mmz_lock);
	if (mmz_open() < 0) {
		pthread_mutex_unlock(&mmz_lock);
		return -1;
	}
	info.mapped = virt;
	ret = ioctl(mmz_fd, IOC_MMB_VIRT_GET_PHYS, &info);
	if (!ret) {
		phys[0] = info.phys_addr;
		phys[1] = 0;
	}
	pthread_mutex_unlock(&mmz_lock);
	return ret;
}

/* NPU_IOC_SET/GET_MEMSIZE on /dev/npu.0, opened just for the call */
static int npu_memsize_ioctl(unsigned long cmd, uint32_t *size)
{
	char path[64];
	int fd, ret;

	snprintf(path, sizeof(path), NPU_DEV_FMT, 0);
	fd = open(path, O_RDWR);
	if (fd < 0)
		return fd;
	ret = ioctl(fd, cmd, size);
	close(fd);
	return ret;
}

int set_uncache_mem_size(int size)
{
	uint32_t val = size;

	if (npu_check(size))
		return 1;
	return npu_memsize_ioctl(NPU_IOC_SET_MEMSIZE, &val);
}

int get_uncache_mem_size(void)
{
	uint32_t val = 0;
	int ret;

	ret = npu_memsize_ioctl(NPU_IOC_GET_MEMSIZE, &val);
	return ret ? ret : (int)val;
}

/*
 * The SDK's uncache memory was a single carve-out once; with MMZ blocks
 * there is no contiguous range or reverse mapping to report, and the
 * library it ships answers 0 here too.
 */
int get_uncache_mem_start_addr(void)
{
	return 0;
}

int get_uncache_mem_end_addr(void)
{
	return 0;
}

int xmedia_phy_to_virt(void)
{
	return 0;
}

int xmedia_virt_to_phy(int virt)
{
	uint32_t phys[2];

	if (xmedia_sys_getphy(virt, phys))
		return 0;
	return phys[0];
}

int uncache_mem_init(void)
{
	return 0;
}

int uncache_mem_uninit(void)
{
	return 0;
}

/* Returns the mapped address, or 0. MMZ blocks are page aligned. */
int uncache_mem_alloc_align(int size)
{
	uint32_t phys[2], virt;

	if (xmedia_sys_mmzalloc(phys, &virt, "Npu_Module", NULL, size))
		return 0;
	return virt;
}

int uncache_mem_alloc(int size)
{
	return uncache_mem_alloc_align(size);
}

int uncache_mem_free(int virt)
{
	int phys = xmedia_virt_to_phy(virt);

	if (npu_check(phys))
		return 1;
	return xmedia_sys_mmzfree(phys);
}
