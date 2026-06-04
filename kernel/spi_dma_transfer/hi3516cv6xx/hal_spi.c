/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "hal_spi.h"
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include "securec.h"
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/bitops.h>
#include <asm/irq.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include "ot_common.h"
#include "ot_osal.h"

#define SSP_ERR KERN_ALERT
#define SSP_INFO KERN_DEBUG
#define ssp_trace(level, fmt, ...) printk(level fmt, ##__VA_ARGS__)

#define SPI_DATAWIDTH_8 0
#define SSP_DEV_NUM 2

#define SSP0_BASE 0x11070000
#define SSP1_BASE 0x11071000
#define SSP_SIZE 0x1000

static td_void __iomem *g_reg_spi_base_va;
#define ssp_cr0 0x00
#define ssp_cr1 0x04
#define ssp_dr 0x08
#define ssp_sr 0x0C
#define ssp_cpsr 0x10
#define ssp_imsc 0x14
#define ssp_ris 0x18
#define ssp_mis 0x1C
#define ssp_icr 0x20
#define ssp_dmacr 0x24
#define ssp_rxfifo_cr 0x2c
#define ssp_txfifo_cr 0x28
#define DATA_WIDTH_8 8
#define DATA_WIDTH_16 16

typedef enum {
    SPEED_5M = 0,
    SPEED_10M,
    SPEED_20M,
    SPEED_25M,
    SPEED_BUTT
} transfer_speed_t;

static inline unsigned int spi_readw(void* addr, unsigned int offset)
{
    return (*((volatile unsigned int *)((uintptr_t)(addr) + (offset))));
}

static inline void spi_writew(void* addr, unsigned int offset, unsigned int value)
{
    *((volatile unsigned int *)((uintptr_t)(addr) + (offset))) = value;
}

static td_s32 ot_ssp_set_frameform(td_u8 framemode, td_u8 spo, td_u8 sph, td_u8 datawidth, td_u16 *cr0)
{
    if (framemode > 3) { /* frame form 3 */
        ssp_trace(SSP_ERR, "set frame parameter err.\n");
        return -1;
    }
    *cr0 = (*cr0 & 0xFFCF) | (framemode << 4); /* 4 bits */
    if ((*cr0 & 0x30) == 0) {
        if (spo > 1) {
            ssp_trace(SSP_ERR, "set spo parameter err.\n");
            return -1;
        }
        if (sph > 1) {
            ssp_trace(SSP_ERR, "set sph parameter err.\n");
            return -1;
        }
        *cr0 = (*cr0 & 0xFF3F) | (sph << 7) | (spo << 6); /* sph 7 bits, spo 6 bits */
    }
    if ((datawidth > 16) || (datawidth < 4)) { /* datawidth between 4 bits and 16 bits */
        ssp_trace(SSP_ERR, "set datawidth parameter err.\n");
        return -1;
    }
    *cr0 = (*cr0 & 0xFFF0) | (datawidth - 1);
    return 0;
}

static td_s32 ot_ssp_set_serialclock(td_u8 scr, td_u8 cpsdvsr, td_u16 *cr0)
{
    *cr0 = (*cr0 & 0xFF) | (scr << 8); /* 8 bits */
    if ((cpsdvsr & 0x1)) {
        ssp_trace(SSP_ERR, "set cpsdvsr parameter err.\n");
        return -1;
    }
    spi_writew(g_reg_spi_base_va, ssp_cpsr, cpsdvsr);
    return 0;
}

static td_s32 ot_ssp_big_end_set(td_u16 *cr1)
{
    /* 4: high bit first */
    *cr1 = *cr1 & ~(1 << 4);
    return 0;
}

static td_s32 ot_ssp_alt_mode_set(td_u16 *cr1)
{
    /* 6: spi cs mode */
    *cr1 = *cr1 | (1 << 6);
    return 0;
}

td_s32 ot_ssp_init_cfg(transfer_speed_t speed, td_u8 datawidth)
{
    const td_u8 framemode = 0;
    const td_u8 spo = 1;
    const td_u8 sph = 1;

    td_u8 scr = 4;     /* Serial Clock Rate 4 */
    td_u8 cpsdvsr = 2; /*  Clock Divider 2 */

    td_u16 ssp_cr0_cfg = 0;
    td_u16 ssp_cr1_cfg = 0;

    ssp_cr0_cfg = spi_readw(g_reg_spi_base_va, ssp_cr0);
    ssp_cr1_cfg = spi_readw(g_reg_spi_base_va, ssp_cr1);

    ot_ssp_set_frameform(framemode, spo, sph, datawidth, &ssp_cr0_cfg);

    if (speed == SPEED_25M) {
        scr = 1;
        cpsdvsr = 2; // 2: clock divider
    }
    /*
     * CLK = Fsspclk/(cpsdvsr*(1+src))  Fsspclk=100M
     * cpsdvsr Must be two-aligned
     * scr [0, 255]
     */
    ot_ssp_set_serialclock(scr, cpsdvsr, &ssp_cr0_cfg);

    ot_ssp_big_end_set(&ssp_cr1_cfg);

    ot_ssp_alt_mode_set(&ssp_cr1_cfg);

    spi_writew(g_reg_spi_base_va, ssp_cr0, ssp_cr0_cfg);
    spi_writew(g_reg_spi_base_va, ssp_cr1, ssp_cr1_cfg);

    spi_writew(g_reg_spi_base_va, ssp_txfifo_cr, 0x9);
    spi_writew(g_reg_spi_base_va, ssp_rxfifo_cr, 0x8);
    spi_writew(g_reg_spi_base_va, ssp_imsc, 0x0); /* Interrupt mask */

    return 0;
}

static td_void spi_clk_enable(td_u32 ssp_no)
{
    td_u32 offset;
    td_void *crg_base = NULL;

    crg_base = ioremap(0x11010000, 0x10000);
    if (crg_base == NULL) {
        ssp_trace(SSP_ERR, "kernel: ioremap crg base failed!\n");
        return;
    }
    offset = (ssp_no == 0) ? 0x4480 : 0x4488;
    *((volatile unsigned int *)(crg_base + offset)) = 0x10; /* SSP CLK enable */
    iounmap(crg_base);
}

td_s32 ot_spi_dma_ssp_init(td_u32 ssp_no)
{
    if (ssp_no == 0) {
        g_reg_spi_base_va = ioremap((unsigned long)SSP0_BASE, (unsigned long)SSP_SIZE);
        if (g_reg_spi_base_va == TD_NULL) {
            ssp_trace(SSP_ERR, "kernel: ioremap ssp base failed!\n");
            return -ENOMEM;
        }
    } else {
        g_reg_spi_base_va = ioremap((unsigned long)SSP1_BASE, (unsigned long)SSP_SIZE);
        if (g_reg_spi_base_va == TD_NULL) {
            ssp_trace(SSP_ERR, "kernel: ioremap ssp base failed!\n");
            return -ENOMEM;
        }
    }
    spi_clk_enable(ssp_no);

    ot_ssp_init_cfg(SPEED_25M, DATA_WIDTH_16);
    return 0;
}

td_void ot_spi_dma_ssp_exit(td_void)
{
    iounmap((td_void *)g_reg_spi_base_va);
    g_reg_spi_base_va = TD_NULL;
}

td_void spi_enable(td_void)
{
    td_u16 ret;

    spi_writew(g_reg_spi_base_va, ssp_dmacr, 0x2); /* enable dma */

    ret = spi_readw(g_reg_spi_base_va, ssp_cr1);
    ret = ret | 0x2;
    spi_writew(g_reg_spi_base_va, ssp_cr1, ret); /* enable spi */

    return;
}

td_void spi_disable(td_void)
{
    td_u32 ret;

    ret = spi_readw(g_reg_spi_base_va, ssp_cr1);
    ret = ret & (~0x2);
    spi_writew(g_reg_spi_base_va, ssp_cr1, ret); /* 0x0 */

    spi_writew(g_reg_spi_base_va, ssp_dmacr, 0x0); /* disable dma */
    return;
}
