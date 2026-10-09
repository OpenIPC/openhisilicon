// SPDX-License-Identifier: GPL-2.0
/*
 * libxmedia_npu: open-source replacement for the XMedia SDK's library of the
 * same name (XMediaIPCLinuxV100R002C00SPC020, GK7205V500 family).
 *
 * This file is the device half: the xmedia_npu_* API of xmedia_api_npu.h
 * on top of /dev/npu.N (kernel/npu/gk7205v500), and the gh_npu_* register
 * accessors, which reach the NPU through the register page the driver
 * lets userspace map. libxmedia_cl.so, the vendor's graph runtime, links
 * against these by name.
 */

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * comm_npu.h only: xmedia_api_npu.h declares close, reset and get_base_addr
 * void, while the library they describe returns a status from each.
 */
#include "comm_npu.h"
#include "npu_ioctl.h"
#include "npu_priv.h"

/* CRG word holding the NPU clock gate (bit 1) and soft reset (bit 0) */
#define CRG_BASE		0x12010000
#define CRG_NPU			0xb4
#define CRG_NPU_SRST		(1u << 0)
#define NPU_RESET_POLLS		1000000

int set_uncache_mem_size(int size);
int uncache_mem_init(void);
int uncache_mem_uninit(void);
int get_uncache_mem_size(void);

/* Each access maps the register page for just that access, as the SDK does */
static int reg_access(int fd, int reg, int *val, int write)
{
	volatile uint32_t *regs;

	regs = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (regs == MAP_FAILED) {
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
		return -1;
	}
	if (write)
		regs[reg / 4] = *val;
	else
		*val = regs[reg / 4];
	munmap((void *)regs, 0x1000);
	return 0;
}

int xmedia_npu_register_write(int fd, int reg, int val)
{
	return reg_access(fd, reg, &val, 1);
}

/* The value, or -1 if the page cannot be mapped (as the SDK's) */
int xmedia_npu_register_read(int fd, int reg)
{
	int val;

	return reg_access(fd, reg, &val, 0) ? -1 : val;
}

int xmedia_register_assign(int set, int mask, int *val)
{
	if (set)
		*val |= mask;
	else
		*val &= ~mask;
	return set;
}

static int reg_assign(int fd, int reg, int set, int mask)
{
	int val = xmedia_npu_register_read(fd, reg);

	xmedia_register_assign(set, mask, &val);
	return xmedia_npu_register_write(fd, reg, val);
}

int gh_npu_get_version(int fd)
{
	return xmedia_npu_register_read(fd, NPU_VERSION);
}

/* The window is physical and the SDK has no reverse mapping: 0 on success */
int gh_npu_get_ddr_base_addr(int fd)
{
	if (xmedia_npu_register_read(fd, NPU_DDR_BASE) < 0)
		return -1;
	return xmedia_phy_to_virt();
}

int gh_npu_set_ddr_base_addr(int fd, int virt)
{
	return xmedia_npu_register_write(fd, NPU_DDR_BASE,
					 xmedia_virt_to_phy(virt));
}

int gh_npu_set_dma_cmd(int fd, int addr, int size)
{
	xmedia_npu_register_write(fd, NPU_CMD_ADDR, addr);
	return xmedia_npu_register_write(fd, NPU_CMD_SIZE, size);
}

int gh_npu_get_dma_cmd_addr(int fd)
{
	return xmedia_npu_register_read(fd, NPU_CMD_ADDR);
}

int gh_npu_get_dma_cmd_size(int fd)
{
	return xmedia_npu_register_read(fd, NPU_CMD_SIZE);
}

int gh_npu_enable_interrupt(int fd, int done, int si, int dfx, int pc)
{
	int val = xmedia_npu_register_read(fd, NPU_INT_EN);

	xmedia_register_assign(done, 1, &val);
	xmedia_register_assign(si, 2, &val);
	xmedia_register_assign(dfx, 4, &val);
	xmedia_register_assign(pc, 8, &val);
	return xmedia_npu_register_write(fd, NPU_INT_EN, val);
}

/* status bits DONE/SI/DFX/PC clear at bits 0/1/3/4; bit 2 is WFI */
int gh_npu_clr_int_status(int fd, int status)
{
	int val = xmedia_npu_register_read(fd, NPU_INT_CLR);

	if (status & 1)
		val |= 1;
	if (status & 2)
		val |= 2;
	if (status & 4)
		val |= 8;
	if (status & 8)
		val |= 0x10;
	return xmedia_npu_register_write(fd, NPU_INT_CLR, val);
}

int gh_npu_clr_wfi_int_status(int fd)
{
	return reg_assign(fd, NPU_INT_CLR, 1, 4);
}

int gh_npu_get_int_status(int fd)
{
	return xmedia_npu_register_read(fd, NPU_INT_STATUS);
}

int gh_npu_get_logic_status(int fd)
{
	return xmedia_npu_register_read(fd, NPU_LOGIC_STATUS);
}

int gh_npu_set_start(int fd)
{
	return reg_assign(fd, NPU_START, 1, 1);
}

int gh_npu_set_dma_outstanding(int fd, int rd, int wr)
{
	int val = xmedia_npu_register_read(fd, NPU_DMA_OUTSTANDING);

	val |= ((wr - 1) << 12) | ((rd - 1) << 8);
	return xmedia_npu_register_write(fd, NPU_DMA_OUTSTANDING, val);
}

int gh_npu_cfg_debug_mode(int fd, int mode, int mailbox_wr)
{
	int val = xmedia_npu_register_read(fd, NPU_DEBUG);

	xmedia_register_assign(mode, 1, &val);
	xmedia_register_assign(mailbox_wr, 2, &val);
	return xmedia_npu_register_write(fd, NPU_DEBUG, val);
}

int gh_npu_stct_rpt_en(int fd, int en)
{
	return reg_assign(fd, NPU_DEBUG, en, 0x100);
}

int gh_npu_cfg_debug_brkpc(int fd, int pc)
{
	return xmedia_npu_register_write(fd, NPU_DEBUG_BRKPC, pc);
}

int gh_npu_get_debug_brkpc(int fd)
{
	return xmedia_npu_register_read(fd, NPU_DEBUG_BRKPC);
}

int gh_npu_get_cfg_running_cycle_cnt(int fd)
{
	return xmedia_npu_register_read(fd, NPU_RUN_CYCLES);
}

int gh_npu_get_cfg_cmd_cnt_rpt(int fd)
{
	return xmedia_npu_register_read(fd, NPU_CMD_CNT);
}

int gh_npu_set_cfg_stct(int fd, int addr, int size)
{
	xmedia_npu_register_write(fd, NPU_STCT_ADDR, addr);
	return xmedia_npu_register_write(fd, NPU_STCT_SIZE, size);
}

int gh_npu_get_cfg_stct_addr(int fd)
{
	int val = xmedia_npu_register_read(fd, NPU_STCT_ADDR);

	return val < 0 ? -1 : val;
}

int gh_npu_get_cfg_stct_size(int fd)
{
	return xmedia_npu_register_read(fd, NPU_STCT_SIZE);
}

int gh_npu_get_cfg_dfx_rpt(int fd)
{
	return xmedia_npu_register_read(fd, NPU_DFX_RPT);
}

/* write-one-to-clear */
int gh_npu_clr_cfg_dfx_rpt(int fd, int bits)
{
	return xmedia_npu_register_write(fd, NPU_DFX_RPT, bits);
}

int gh_npu_set_end_addr(int fd, int virt)
{
	return xmedia_npu_register_write(fd, NPU_END_ADDR,
					 xmedia_virt_to_phy(virt));
}

int gh_npu_get_end_addr(int fd)
{
	xmedia_npu_register_read(fd, NPU_END_ADDR);
	return xmedia_phy_to_virt();
}

int gh_npu_set_dfx_mask(int fd, int mask)
{
	int val = xmedia_npu_register_read(fd, NPU_DFX_MASK);

	return xmedia_npu_register_write(fd, NPU_DFX_MASK, val | mask);
}

int gh_npu_cfg_crm_module_en(int fd, int en)
{
	return reg_assign(fd, NPU_CRM, en, 1);
}

/* Low power gates the module first; leaving it ungates the module last. */
int gh_npu_cfg_crm_lowpower(int fd, int en)
{
	int val = xmedia_npu_register_read(fd, NPU_CRM);

	if (en)
		gh_npu_cfg_crm_module_en(fd, 0);
	xmedia_register_assign(en, 4, &val);
	xmedia_register_assign(en, 8, &val);
	if (!en)
		gh_npu_cfg_crm_module_en(fd, 1);
	return xmedia_npu_register_write(fd, NPU_CRM, val);
}

static const int npu_mailbox[4] = { 0x1a8, 0x1ac, 0x1c8, 0x1cc };

int gh_npu_set_mailbox_reg(int fd, int idx, int val)
{
	if (idx < 0 || idx > 3)
		return fd;
	return xmedia_npu_register_write(fd, npu_mailbox[idx], val);
}

int gh_npu_get_mailbox_reg(int fd, int idx)
{
	if (idx < 0 || idx > 3)
		return -1;
	return xmedia_npu_register_read(fd, npu_mailbox[idx]);
}

/*
 * Soft reset through the CRG: hold the NPU in reset until its running
 * cycle counter reads zero, then let it go. Kept for the SDK's API; it
 * goes behind the driver's back (and needs /dev/mem), so xmedia_npu_reset()
 * leaves resetting to the driver.
 */
int gh_npu_reset(int fd)
{
	volatile uint32_t *crg;
	int memfd, cycles, polls = 0;

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (memfd < 0) {
		xmedia_printf("%s: open /dev/mem: %s\n", __func__,
			      strerror(errno));
		return -1;
	}
	crg = mmap(NULL, 0x100, PROT_READ | PROT_WRITE, MAP_SHARED, memfd,
		   CRG_BASE);
	close(memfd);
	if (crg == MAP_FAILED) {
		xmedia_printf("%s: map /dev/mem: %s\n", __func__,
			      strerror(errno));
		return -1;
	}

	crg[CRG_NPU / 4] |= CRG_NPU_SRST;
	do
		cycles = gh_npu_get_cfg_running_cycle_cnt(fd);
	while (cycles > 0 && ++polls < NPU_RESET_POLLS);
	crg[CRG_NPU / 4] &= ~CRG_NPU_SRST;
	munmap((void *)crg, 0x100);

	return cycles > 0 ? -1 : cycles;
}

/*
 * The SDK's build reads an uncache pool size from r0 although the header
 * declares no argument, so header users pass garbage; the pool is sized by
 * the driver's memsize parameter (or NPU_IOC_SET_MEMSIZE) instead.
 */
xmedia_s32 xmedia_npu_module_init(xmedia_void)
{
	return uncache_mem_init();
}

xmedia_s32 xmedia_npu_module_uninit(xmedia_void)
{
	uncache_mem_uninit();
	return 0;
}

xmedia_u32 xmedia_npu_get_memsize(xmedia_void)
{
	return get_uncache_mem_size();
}

xmedia_s32 xmedia_npu_get_devcnt(xmedia_u32 *devcnt)
{
	struct dirent *de;
	unsigned int n = 0;
	DIR *dir;

	dir = opendir("/dev/");
	if (!dir) {
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
		return -1;
	}
	while ((de = readdir(dir)))
		if (!strncmp(de->d_name, "npu.", 4))
			n++;
	closedir(dir);
	*devcnt = n;
	return 0;
}

xmedia_s32 xmedia_npu_open(xmedia_u32 dev_index, xmedia_u32 *dev_fd,
			   xmedia_npu_open_params_s *params)
{
	struct npu_open_params open_params = {
		/* the window covers all of DDR; jobs narrow it */
		.base_addr = 0,
		.end_addr = 0xffffffff,
	};
	char path[64];
	int fd, ret;

	if (npu_check(dev_fd) || npu_check(params))
		return 1;

	snprintf(path, sizeof(path), NPU_DEV_FMT, dev_index);
	fd = open(path, O_RDWR);
	if (fd < 0)
		return fd;
	*dev_fd = fd;

	ret = ioctl(fd, NPU_IOC_OPEN, &open_params);
	if (ret)
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
	return ret;
}

/* Declared void in xmedia_api_npu.h; the SDK's returns 0/1 all the same. */
int xmedia_npu_close(xmedia_u32 dev_fd)
{
	if (ioctl(dev_fd, NPU_IOC_CLOSE, 0)) {
		npu_check(0);
		return 1;
	}
	close(dev_fd);
	return 0;
}

xmedia_s32 xmedia_npu_create_queue(xmedia_u32 dev_fd)
{
	int ret = ioctl(dev_fd, NPU_IOC_CREATE_QUEUE, 0);

	if (ret)
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
	return ret;
}

xmedia_s32 xmedia_npu_destroy_queue(xmedia_u32 dev_fd)
{
	int ret = ioctl(dev_fd, NPU_IOC_DESTROY_QUEUE, 0);

	if (ret)
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
	return ret;
}

xmedia_s32 xmedia_npu_submit_job(xmedia_u32 dev_fd,
				 xmedia_npu_start_params_s *params,
				 xmedia_u32 *jb_id)
{
	int ret;

	if (npu_check(params) || npu_check(jb_id) ||
	    npu_check(params->npu_cmd_data_size))
		return 1;

	/* the driver answers with the job id */
	ret = ioctl(dev_fd, NPU_IOC_SUBMIT_JOB, params);
	if (ret < 0) {
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
		return ret;
	}
	*jb_id = ret;
	return 0;
}

xmedia_s32 xmedia_npu_wait_job(xmedia_u32 dev_fd,
			       xmedia_npu_job_state_t *status)
{
	if (npu_check(status))
		return 1;
	return ioctl(dev_fd, NPU_IOC_WAIT_JOB, status);
}

xmedia_s32 xmedia_npu_query_job(xmedia_u32 dev_fd,
				xmedia_npu_job_state_t *status)
{
	if (npu_check(status))
		return 1;
	return ioctl(dev_fd, NPU_IOC_QUERY_JOB, status);
}

/* Declared void in xmedia_api_npu.h; returns 0, 1 or the register error. */
int xmedia_npu_get_base_addr(xmedia_u32 dev_fd, xmedia_u32 *addr)
{
	int base;

	if (npu_check(addr))
		return 1;
	base = gh_npu_get_ddr_base_addr(dev_fd);
	if (base > 0) {
		*addr = base;
		return 0;
	}
	return base;
}

/* NPU_DFX_RPT bits, as the hardware reports them on a DFX abort */
static const struct {
	unsigned int bit;
	const char *name;
} dfx_err_table[] = {
	{ 0x000001, "sram_dma_acc_err" },
	{ 0x000002, "sram_conv_bias_err" },
	{ 0x000004, "sram_conv_fm_err" },
	{ 0x000008, "sram_conv_wt_err" },
	{ 0x000010, "sram_pool_fm_err" },
	{ 0x000020, "sram_pooldma_lut_err" },
	{ 0x000040, "sram_convdma_bias_err" },
	{ 0x000080, "dec_inst_err" },
	{ 0x000100, "conv_addr_err" },
	{ 0x000200, "pool_addr_err" },
	{ 0x000400, "dma_addr_err" },
	{ 0x000800, "wsram_lut_16B_align_err" },
	{ 0x001000, "wsram_bias_8B_align_err" },
	{ 0x002000, "wsram_wt_8B_align_err" },
	{ 0x004000, "wsram_fm_8B_align_err" },
	{ 0x008000, "rsram_fm_8B_align_err" },
	{ 0x010000, "rwddr_out_of_bound_err" },
	{ 0x020000, "dma_ar_cross4k_p_err" },
	{ 0x040000, "dma_r_resp_p_err" },
	{ 0x080000, "dma_aw_cross4k_p_err" },
	{ 0x100000, "dma_b_resp_p_err" },
	{ 0x200000, "wddr_out_of_stct_size_err" },
	{ 0x400000, "pool_online_err" },
};

xmedia_s32 xmedia_npu_getabort(xmedia_npu_job_state_t *status)
{
	unsigned int i;

	if (npu_check(status))
		return 1;
	if (status->state != JOB_ABORT_FINISH)
		return 0;
	for (i = 0; i < sizeof(dfx_err_table) / sizeof(dfx_err_table[0]); i++)
		if (status->abort_value & dfx_err_table[i].bit)
			xmedia_printf("%s\n", dfx_err_table[i].name);
	return 0;
}

int xmedia_npu_lowpower(int dev_fd, int en)
{
	gh_npu_cfg_crm_lowpower(dev_fd, en);
	return 0;
}

/*
 * Drops the device's queue (and with it any job) and gets the NPU back to
 * clear status with every interrupt enabled. The driver does all of it,
 * soft-resetting the NPU if a job or an abort left it running; the SDK's
 * library pulsed the reset again from /dev/mem and rewrote the interrupt
 * registers itself, racing every other process on the NPU.
 */
int xmedia_npu_reset(xmedia_u32 dev_fd)
{
	if (npu_check(!ioctl(dev_fd, NPU_IOC_RESET, 0)))
		return 1;
	return 0;
}

int xmedia_npu_clrwfi(int dev_fd)
{
	int ret = ioctl(dev_fd, NPU_IOC_CLR_WFI, 0);

	if (ret)
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
	return ret;
}

int xmedia_npu_set_profiling(int dev_fd, void *profiling)
{
	int ret;

	if (npu_check(profiling))
		return 1;
	ret = ioctl(dev_fd, NPU_IOC_SET_PROFILING, profiling);
	if (ret)
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
	return ret;
}

/* First word of a cycle dump file (12-byte header) */
int xmedia_npu_get_cycle_size(const char *file)
{
	uint32_t hdr[3] = { 0 };
	FILE *fp;
	size_t n;

	if (!file) {
		xmedia_printf("%s error, line = %d: file_name_bin is NULL\n",
			      __func__, __LINE__);
		npu_check(0);
		return 1;
	}
	fp = fopen(file, "r");
	if (!fp) {
		xmedia_printf("open cycle_bin_file.bin fail\n");
		npu_check(0);
		return 1;
	}
	n = fread(hdr, sizeof(hdr), 1, fp);
	fclose(fp);
	if (!n) {
		xmedia_printf("read cycle_bin_file.bin fail\n");
		npu_check(0);
		return 1;
	}
	return hdr[0];
}

xmedia_s32 xmedia_npu_get_capability(xmedia_u32 dev_fd,
				     xmedia_npu_attribute_e param,
				     xmedia_s32 *value)
{
	uint32_t attr = param;

	if (npu_check(value))
		return 1;
	/* the driver answers with the value */
	*value = ioctl(dev_fd, NPU_IOC_GET_CAPABILITY, &attr);
	if (*value < 0) {
		xmedia_printf("%s error, line = %d\n", __func__, __LINE__);
		return *value;
	}
	return 0;
}

/* Reads mailbox @idx, which a job may use for results, into @value */
int xmedia_npu_communicate(int dev_fd, int idx, int *value)
{
	if (npu_check(value))
		return 1;
	if (idx < 0 || idx > 3)
		return -1;
	return reg_access(dev_fd, npu_mailbox[idx], value, 0);
}
