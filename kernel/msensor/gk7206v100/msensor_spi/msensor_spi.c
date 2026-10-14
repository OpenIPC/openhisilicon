/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor_spi.h"
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
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
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

#include "osal.h"
#define xmedia_unused(x)              ((xmedia_void)(x))

#define SSP_DBG_ERR                   KERN_ALERT
#define SSP_DBG_INFO                  KERN_DEBUG
#define ssp_trace(level, fmt, ...)    printk(level fmt, ##__VA_ARGS__)

#define SPI_DATAWIDTH_8               0
#define DEV_NAME                      "ssp"
#define MAX_MSENSOR_DEV_NUM           2
#define SSP_DEV_NUM                   3

#define ssp_readw(addr, ret)          ((ret) = (*(volatile xmedia_u32 *)(addr)))
#define ssp_writew(addr, value)       ((*(volatile xmedia_u32 *)(addr)) = (value))

#define xmedia_reg_read(addr, ret)    ((ret) = (*(volatile xmedia_u32 *)(addr)))
#define xmedia_reg_write(addr, value) ((*(volatile xmedia_u32 *)(addr)) = (value))

#define SSP_BASE                      0x12070000
#define SSP_SIZE                      0x10000 /* 64KB */

#define CRG_BASE                      0x12010000
#define CRG_SIZE                      0x10000
#define SSP_CRG_OFFSET                0x01bc

//spi 管脚复用基地址，需要根据对应芯片做修改
#define XM7206_GPIO_BASE           0x100c0000 // 管脚复用基地址

#define IO1_SIZE                      0x1000

#define CS_SIZE                       0x4
#define SPI1_CS_BASE_ADDRS            0x12028000

static xmedia_void __iomem *g_reg_ssp_base_va = XMEDIA_NULL;
static xmedia_void __iomem *g_reg_crg_base_va = XMEDIA_NULL;
static xmedia_void __iomem *g_reg_io1_base_va = XMEDIA_NULL;
static xmedia_void __iomem *g_reg_spi1_cs_base_va = XMEDIA_NULL;

#define io_address_verify(x) ((xmedia_ulong)(xmedia_uintptr_t)g_reg_ssp_base_va + ((x) - (SSP_BASE)))
#define SSP_CRG_ADDRESS      ((xmedia_ulong)(xmedia_uintptr_t)g_reg_crg_base_va + SSP_CRG_OFFSET)
#define io1_addr(offset)     ((xmedia_ulong)(xmedia_uintptr_t)g_reg_io1_base_va + offset)
#define spi1_cs_addr(offset)     ((xmedia_ulong)(xmedia_uintptr_t)g_reg_spi1_cs_base_va + offset)

/* SSP register definition . */
#define ssp_cr0(ssp_no)   io_address_verify(SSP_BASE + 0x00 + ((ssp_no) * 0x1000))
#define ssp_cr1(ssp_no)   io_address_verify(SSP_BASE + 0x04 + ((ssp_no) * 0x1000))
#define ssp_dr(ssp_no)    io_address_verify(SSP_BASE + 0x08 + ((ssp_no) * 0x1000))
#define ssp_sr(ssp_no)    io_address_verify(SSP_BASE + 0x0C + ((ssp_no) * 0x1000))
#define ssp_cpsr(ssp_no)  io_address_verify(SSP_BASE + 0x10 + ((ssp_no) * 0x1000))
#define ssp_imsc(ssp_no)  io_address_verify(SSP_BASE + 0x14 + ((ssp_no) * 0x1000))
#define ssp_ris(ssp_no)   io_address_verify(SSP_BASE + 0x18 + ((ssp_no) * 0x1000))
#define ssp_mis(ssp_no)   io_address_verify(SSP_BASE + 0x1C + ((ssp_no) * 0x1000))
#define ssp_icr(ssp_no)   io_address_verify(SSP_BASE + 0x20 + ((ssp_no) * 0x1000))
#define ssp_dmacr(ssp_no) io_address_verify(SSP_BASE + 0x24 + ((ssp_no) * 0x1000))
#define ssp_csr(ssp_no)   io_address_verify(SSP_BASE + 0x30 + ((ssp_no) * 0x1000))

static xmedia_u32 g_dev_ssp_no[MAX_MSENSOR_DEV_NUM] = {2,};
static spinlock_t g_ssp_lock[SSP_DEV_NUM];
#define ssp_spin_lock_init(ssp_no)     spin_lock_init(&g_ssp_lock[ssp_no])
#define ssp_spin_lock(ssp_no, flags)   spin_lock_irqsave(&g_ssp_lock[ssp_no], flags)
#define ssp_spin_unlock(ssp_no, flags) spin_unlock_irqrestore(&g_ssp_lock[ssp_no], flags)

static spinlock_t g_ssp_crg_lock;
#define ssp_crg_spin_lock_init()   spin_lock_init(&g_ssp_crg_lock)
#define ssp_crg_spin_lock(flags)   spin_lock_irqsave(&g_ssp_crg_lock, flags)
#define ssp_crg_spin_unlock(flags) spin_unlock_irqrestore(&g_ssp_crg_lock, flags)

static xmedia_void xmedia_ssp_clock_enable(xmedia_u32 ssp_no)
{
    xmedia_u32 ret = 0;
    xmedia_ulong flags;

    ssp_crg_spin_lock(flags);

    xmedia_reg_read(SSP_CRG_ADDRESS, ret);
    ret |= 0x1 << (12 + ssp_no); /* ssp clock enable bit: 4 */
    xmedia_reg_write(SSP_CRG_ADDRESS, ret);

    ssp_crg_spin_unlock(flags);
    return;
}

static xmedia_void xmedia_ssp_clock_disable(xmedia_u32 ssp_no)
{
    xmedia_u32 ret = 0;
    xmedia_ulong flags;

    ssp_crg_spin_lock(flags);

    xmedia_reg_read(SSP_CRG_ADDRESS, ret);
    ret = ret & (~(0x1 << (12 + ssp_no))); /* ssp clock enable bit: 4 */
    xmedia_reg_write(SSP_CRG_ADDRESS, ret);

    ssp_crg_spin_unlock(flags);
    return;
}

static xmedia_void xmedia_ssp_clock_reset(xmedia_u32 ssp_no)
{
#ifdef MSENSOR_SPI_DEBUG
    xmedia_u32 ret = 0;
    xmedia_ulong flags;

    ssp_crg_spin_lock(flags);

    xmedia_reg_read(SSP_CRG_ADDRESS, ret);
    ret |= 0x1 << (16 + ssp_no); /* ssp clock reset bit: 0 */
    xmedia_reg_write(SSP_CRG_ADDRESS, ret);

    ssp_crg_spin_unlock(flags);
#endif
}

static xmedia_void xmedia_ssp_clock_unreset(xmedia_u32 ssp_no)
{
#ifdef MSENSOR_SPI_DEBUG
    xmedia_u32 ret = 0;
    xmedia_ulong flags;

    ssp_crg_spin_lock(flags);

    xmedia_reg_read(SSP_CRG_ADDRESS, ret);
    ret = ret & (~(0x1 << (16 + ssp_no))); /* ssp clock reset bit: 0 */
    xmedia_reg_write(SSP_CRG_ADDRESS, ret);

    ssp_crg_spin_unlock(flags);
#endif
}

/*
 * set SSP frame form routine.
 *
 * @param framemode: frame form
 * 00: Motorola SPI frame form.
 * when set the mode,need set SSPCLKOUT phase and SSPCLKOUT voltage level.
 * 01: TI synchronous serial frame form
 * 10: National Microwire frame form
 * 11: reserved
 * @param sphvalue: SSPCLKOUT phase (0/1)
 * @param sp0: SSPCLKOUT voltage level (0/1)
 * @param datavalue: data bit
 * 0000: reserved    0001: reserved    0010: reserved    0011: 4bit data
 * 0100: 5bit data   0101: 6bit data   0110:7bit data    0111: 8bit data
 * 1000: 9bit data   1001: 10bit data  1010:11bit data   1011: 12bit data
 * 1100: 13bit data  1101: 14bit data  1110:15bit data   1111: 16bit data
 *
 * @return value: 0--success; -1--error.
 */
static xmedia_s32 xmedia_ssp_set_frameform(xmedia_u32 ssp_no, xmedia_u8 framemode,
    xmedia_u8 spo, xmedia_u8 sph, xmedia_u8 datawidth)
{
    xmedia_u32 ret = 0;

    ssp_readw(ssp_cr0(ssp_no), ret);
    if (framemode > 3) { /* frame form 3 */
        ssp_trace(SSP_DBG_ERR, "set framemode failed.\n");
        return -1;
    }
    ret = (ret & 0xFFCF) | (framemode << 4); /* 4 bits */
    if ((ret & 0x30) == 0) {
        if (spo > 1) {
            ssp_trace(SSP_DBG_ERR, "set spo failed.\n");
            return -1;
        }
        if (sph > 1) {
            ssp_trace(SSP_DBG_ERR, "set sph failed.\n");
            return -1;
        }
        ret = (ret & 0xFF3F) | (sph << 7) | (spo << 6); /* sph 7 bits, spo 6 bits */
    }
    if ((datawidth > 16) || (datawidth < 4)) { /* datawidth between 4 bits and 16 bits */
        ssp_trace(SSP_DBG_ERR, "set datawidth parameter err.\n");
        return -1;
    }
    ret = (ret & 0xFFF0) | (datawidth - 1);
    ssp_writew(ssp_cr0(ssp_no), ret);
    return 0;
}

/*
 * set SSP serial clock rate routine.
 *
 * @param scr: scr value.(0-255,usually it is 0)
 * @param cpsdvsr: clock prescale divisor.(2-254 even)
 *
 * @return value: 0--success; -1--error.
 */
static xmedia_s32 xmedia_ssp_set_serialclock(xmedia_u32 ssp_no, xmedia_u8 scr, xmedia_u8 cpsdvsr)
{
    xmedia_u32 ret = 0;

    ssp_readw(ssp_cr0(ssp_no), ret);
    ret = (ret & 0xFF) | (scr << 8); /* 8 bits */
    ssp_writew(ssp_cr0(ssp_no), ret);
    if ((cpsdvsr & 0x1)) {
        ssp_trace(SSP_DBG_ERR, "set cpsdvsr failed.\n");
        return -1;
    }
    ssp_writew(ssp_cpsr(ssp_no), cpsdvsr);
    return 0;
}

static xmedia_s32 xmedia_ssp_big_end_set(xmedia_u32 ssp_no)
{
    xmedia_u32 ret = 0;

    ssp_readw(ssp_cr1(ssp_no), ret);
    ret = (ret & 0xFF) & 0xEF;
    ssp_writew(ssp_cr1(ssp_no), ret);
    return 0;
}

static xmedia_s32 xmedia_ssp_alt_mode_set(xmedia_u32 ssp_no, xmedia_s32 enable)
{
    xmedia_u32 ret = 0;

    ssp_readw(ssp_cr1(ssp_no), ret);
    if (enable == 0) {
        ret = ret & (~0x40);
    } else {
        ret = (ret & 0xFF) | 0x40;
    }
    ssp_writew(ssp_cr1(ssp_no), ret);
    return 0;
}

static xmedia_u32 xmedia_ssp_is_fifo_busy(xmedia_u32 ssp_no)
{
    xmedia_u32 ret = 0;

    ssp_readw(ssp_sr(ssp_no), ret);
    return (ret & 0x10);
}

static xmedia_u32 xmedia_ssp_is_fifo_empty(xmedia_u32 ssp_no, xmedia_s32 send)
{
    xmedia_u32 ret = 0;

    ssp_readw(ssp_sr(ssp_no), ret);
    if (send != 0) {
        if ((ret & 0x1) == 0x1) { /* send fifo */
            return 0;
        } else {
            return 1;
        }
    } else {
        if ((ret & 0x4) == 0x4) { /* receive fifo */
            return 0;
        } else {
            return 1;
        }
    }
}

/* fsspclkout is fsspclk / (cpsdvsr * (1 + scr)) */
static xmedia_s32 xmedia_ssp_init_cfg(xmedia_u32 ssp_no)
{
    const xmedia_u8 framemode = 0;
    const xmedia_u8 spo = 1;
    const xmedia_u8 sph = 1;

#if SPI_DATAWIDTH_8
    const xmedia_u8 datawidth = 8;
#else
    const xmedia_u8 datawidth = 16;
#endif

    const xmedia_u8 scr = 29; /* scr 4 */
    const xmedia_u8 cpsdvsr = 2; /* cpsdvsr 2 */

    xmedia_ssp_set_frameform(ssp_no, framemode, spo, sph, datawidth);

    xmedia_ssp_set_serialclock(ssp_no, scr, cpsdvsr);
    xmedia_ssp_big_end_set(ssp_no);

    /* altasens mode, which CS won't be pull high between 16bit data transfer */
    xmedia_ssp_alt_mode_set(ssp_no, 0);
    return 0;
}

static xmedia_void spi_enable(xmedia_u32 ssp_no)
{
    xmedia_u32 ret = 0;

    xmedia_ssp_clock_enable(ssp_no);
    xmedia_ssp_clock_unreset(ssp_no);
    /* little endian */
    xmedia_reg_read(ssp_cr1(ssp_no), ret);
    ret = (ret & 0xff) | 0x2;
    xmedia_reg_write(ssp_cr1(ssp_no), ret); /* 0x2 */
    xmedia_ssp_init_cfg(ssp_no);
    return;
}

static xmedia_void spi_disable(xmedia_u32 ssp_no)
{
    xmedia_u32 ret = 0;

    xmedia_reg_read(ssp_cr1(ssp_no), ret);
    ret = (ret & 0xff) & (~0x2);
    xmedia_reg_write(ssp_cr1(ssp_no), ret); /* 0x0 */
    xmedia_ssp_clock_reset(ssp_no);
    xmedia_ssp_clock_disable(ssp_no);
    return;
}

xmedia_u16 xmedia_msensor_ssp_read_alt(xmedia_s32 dev, xmedia_u8 reg_addr,
    xmedia_u8 *reg_data, xmedia_u32 cnt, xmedia_bool fifo_mode)
{
    xmedia_u32 ret = 0;
    xmedia_ulong flags;
    xmedia_s32 ssp_no = 0;

#if SPI_DATAWIDTH_8
    xmedia_u8 buf[2] = { 0 }; /* 2 buf */
#else
    xmedia_u16 buf_16;
#endif
    xmedia_u32 i;

    ssp_no = g_dev_ssp_no[dev];

    ssp_spin_lock(ssp_no, flags);

    spi_enable(ssp_no);

    for (i = 0; i < cnt; i++) {

#if SPI_DATAWIDTH_8
        if (fifo_mode != XMEDIA_FALSE) {
            buf[0] = reg_addr | 0x80;
            buf[1] = 0x0;
        } else {
            buf[0] = (reg_addr + i) | 0x80;
            buf[1] = 0x0;
        }

        ssp_writew(ssp_dr(ssp_no), buf[0]);
        ssp_writew(ssp_dr(ssp_no), buf[1]);
#else

        if (fifo_mode != XMEDIA_FALSE) {
            buf_16 = (reg_addr | 0x80) << 8; /* 8 bit */
        } else {
            buf_16 = ((reg_addr + i) | 0x80) << 8; /* 8 bit */
        }
        ssp_writew(ssp_dr(ssp_no), buf_16);
#endif

        while (xmedia_ssp_is_fifo_busy(ssp_no) != 0) {
        }
        while (xmedia_ssp_is_fifo_empty(ssp_no, 1) != 0) {
        }
        while (xmedia_ssp_is_fifo_empty(ssp_no, 0) != 0) {
        }
        while (xmedia_ssp_is_fifo_empty(ssp_no, 0) == 0) {
            ssp_readw(ssp_dr(ssp_no), ret);
        }
        reg_data[i] = ret & 0xff;
    }

    spi_disable(ssp_no);

    ssp_spin_unlock(ssp_no, flags);

    return 0;
}

xmedia_s32 xmedia_msensor_ssp_write_alt(xmedia_s32 dev, xmedia_u8 reg_addr, const xmedia_u8 *data)
{
    xmedia_u32 ret;
    xmedia_s32 ssp_no = 0;
    xmedia_ulong flags;
#if SPI_DATAWIDTH_8
    xmedia_u8 buf[2] = { 0 }; /* 2 buf */
#else
    xmedia_u16 buf_16;
#endif

    ssp_no = g_dev_ssp_no[dev];

    ssp_spin_lock(ssp_no, flags);

    spi_enable(ssp_no);

#if SPI_DATAWIDTH_8
    buf[0] = reg_addr & (~0x80);
    buf[1] = *data;
    ssp_writew(ssp_dr(ssp_no), buf[0]);
    ssp_writew(ssp_dr(ssp_no), buf[1]);
#else
    buf_16 = (reg_addr & (~0x80)) << 8; /* 8 bit */
    buf_16 = buf_16 | *data;
    ssp_writew(ssp_dr(ssp_no), buf_16);
#endif

    while (xmedia_ssp_is_fifo_busy(ssp_no) != 0) {
    }
    while (xmedia_ssp_is_fifo_empty(ssp_no, 1) != 0) {
    }
    while (xmedia_ssp_is_fifo_empty(ssp_no, 0) != 0) {
    }
    while (xmedia_ssp_is_fifo_empty(ssp_no, 0) == 0) {
        ssp_readw(ssp_dr(ssp_no), ret);
    }
    xmedia_unused(ret);

    spi_disable(ssp_no);

    ssp_spin_unlock(ssp_no, flags);
    return 0;
}

// 根据实际使用gpio 正确复用spi 功能管脚
static xmedia_void spi_pin_muxing(xmedia_void)
{

    // xm7206 默认使用一路spi spi2
    xmedia_reg_write(io1_addr(0x28), 0x1204); /* SPI2_SCLK gpio4_7 打开上拉 */
    xmedia_reg_write(io1_addr(0x2c), 0x1004); /* SPI2_SDO gpio5_0 */
    xmedia_reg_write(io1_addr(0x30), 0x1004); /* SPI2_CSN gpio5_1 */
    xmedia_reg_write(io1_addr(0x34), 0x1004); /* SPI2_SDI gpio5_2 */

    ssp_trace(SSP_DBG_INFO, "use spi12\n");
}

static struct file_operations g_ssp_fops = {
    .owner = THIS_MODULE,
};

static struct miscdevice g_ssp_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME,
    .fops = &g_ssp_fops,
};

xmedia_s32 xmedia_msensor_spi_base_addr(xmedia_u32 *base_addr)
{
    *base_addr = XM7206_GPIO_BASE;
    return XMEDIA_SUCCESS;
}

/*
 * initializes SSP interface routine.
 * @return value:0--success.
 */
static xmedia_s32 __init xmedia_msensor_ssp_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_u32 ssp_no;
    xmedia_u32 spi_base_addr = 0x12345678;

    g_reg_ssp_base_va = ioremap((unsigned long)SSP_BASE, (unsigned long)(SSP_SIZE * SSP_DEV_NUM));
    if (g_reg_ssp_base_va == XMEDIA_NULL) {
        ssp_trace(SSP_DBG_ERR, "ioremap ssp base failed!\n");
        return -ENOMEM;
    }

    g_reg_crg_base_va = ioremap((unsigned long)CRG_BASE, (unsigned long)(CRG_SIZE));
    if (g_reg_crg_base_va == XMEDIA_NULL) {
        ssp_trace(SSP_DBG_ERR, "ioremap ssp crg failed!\n");
        ret = -ENOMEM;
        goto err0;
    }
    ret = xmedia_msensor_spi_base_addr(&spi_base_addr);
    if (ret != XMEDIA_SUCCESS) {
        goto err1;
    }
    g_reg_io1_base_va = ioremap((unsigned long)spi_base_addr, (unsigned long)(IO1_SIZE));
    if (g_reg_io1_base_va == XMEDIA_NULL) {
        ssp_trace(SSP_DBG_ERR, "ioremap ssp io failed!\n");
        ret = -ENOMEM;
        goto err1;
    }

    g_reg_spi1_cs_base_va = ioremap((unsigned long)SPI1_CS_BASE_ADDRS, (unsigned long)(CS_SIZE));
    if (g_reg_spi1_cs_base_va == XMEDIA_NULL) {
        ssp_trace(SSP_DBG_ERR, "ioremap ssp gpio failed!\n");
        ret = -ENOMEM;
        goto err2;
    }

    spi_pin_muxing();

    ret = misc_register(&g_ssp_dev);
    if (ret != XMEDIA_SUCCESS) {
        ssp_trace(SSP_DBG_ERR, "register ssp_0 device failed!");
        ret = XMEDIA_FAILURE;
        goto err3;
    }

    for (ssp_no = 0; ssp_no < SSP_DEV_NUM; ssp_no++) {
        ssp_spin_lock_init(ssp_no);
    }

    ssp_crg_spin_lock_init();

    ssp_trace(SSP_DBG_INFO, "kernel: ssp initial ok!\n");
    printk(KERN_INFO "load xm_msensor_spi.ko OK!\n");
    return 0;

err3:
    iounmap((xmedia_void *)g_reg_spi1_cs_base_va);
    g_reg_spi1_cs_base_va = XMEDIA_NULL;
err2:
    iounmap((xmedia_void *)g_reg_io1_base_va);
    g_reg_io1_base_va = XMEDIA_NULL;
err1:
    iounmap((xmedia_void *)g_reg_crg_base_va);
    g_reg_crg_base_va = XMEDIA_NULL;
err0:
    iounmap((xmedia_void *)g_reg_ssp_base_va);
    g_reg_ssp_base_va = XMEDIA_NULL;
    return ret;
}

static xmedia_void __exit xmedia_msensor_ssp_exit(xmedia_void)
{
    misc_deregister(&g_ssp_dev);

    iounmap((xmedia_void *)g_reg_spi1_cs_base_va);
    g_reg_spi1_cs_base_va = XMEDIA_NULL;

    iounmap((xmedia_void *)g_reg_io1_base_va);
    g_reg_io1_base_va = XMEDIA_NULL;

    iounmap((xmedia_void *)g_reg_crg_base_va);
    g_reg_crg_base_va = XMEDIA_NULL;

    iounmap((xmedia_void *)g_reg_ssp_base_va);
    g_reg_ssp_base_va = XMEDIA_NULL;

    printk(KERN_INFO "unload xm_msensor_spi.ko OK!\n");
}

#ifdef MODULE
EXPORT_SYMBOL(xmedia_msensor_ssp_read_alt);
EXPORT_SYMBOL(xmedia_msensor_ssp_write_alt);

module_init(xmedia_msensor_ssp_init);
module_exit(xmedia_msensor_ssp_exit);
MODULE_DESCRIPTION("ssp driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("otlicon");
#else
int __init msensor_spi_driver_init(void)
{
    return xmedia_msensor_ssp_init();
}
#endif
