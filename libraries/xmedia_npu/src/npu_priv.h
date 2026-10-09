/* SPDX-License-Identifier: GPL-2.0 */
#ifndef NPU_PRIV_H
#define NPU_PRIV_H

#include <stdio.h>

#define NPU_DEV_FMT	"/dev/npu.%d"

/* register offsets inside the page /dev/npu.N maps (see npu.c in the kernel) */
#define NPU_VERSION		0x000
#define NPU_DDR_BASE		0x004
#define NPU_CMD_ADDR		0x008
#define NPU_CMD_SIZE		0x00c
#define NPU_INT_EN		0x010
#define NPU_INT_CLR		0x014
#define NPU_INT_STATUS		0x018
#define NPU_LOGIC_STATUS	0x01c
#define NPU_START		0x020
#define NPU_DEBUG		0x024
#define NPU_DEBUG_BRKPC		0x028
#define NPU_RUN_CYCLES		0x02c
#define NPU_CMD_CNT		0x030
#define NPU_STCT_ADDR		0x034
#define NPU_STCT_SIZE		0x038
#define NPU_DFX_RPT		0x03c
#define NPU_END_ADDR		0x040
#define NPU_DFX_MASK		0x044
#define NPU_DMA_OUTSTANDING	0x048
#define NPU_CRM			0x04c

int xmedia_printf(const char *fmt, ...);

/* What the SDK's library prints when a precondition fails; 1 if it did */
static inline int npu_check_at(int ok, const char *file, unsigned int line,
			       const char *func)
{
	if (ok)
		return 0;
	xmedia_printf("Assertion is failed. file = %s, line = %u, "
		      "function = %s\r\n", file, line, func);
	return 1;
}

#define npu_check(cond) npu_check_at(!!(cond), __FILE__, __LINE__, __func__)

int xmedia_npu_register_read(int fd, int reg);
int xmedia_npu_register_write(int fd, int reg, int val);
int xmedia_virt_to_phy(int virt);
int xmedia_phy_to_virt(void);

#endif /* NPU_PRIV_H */
