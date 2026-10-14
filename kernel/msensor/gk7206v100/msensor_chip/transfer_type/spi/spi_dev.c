/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "spi_dev.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include "common.h"
#include "msensor.h"

#define REG_WIDTH 1

static xmedia_u32 g_csn = 0;
static spinlock_t g_lock_rd;

static xmedia_u32 g_bus_num = 1;

module_param(g_bus_num, uint, S_IRUGO);
MODULE_PARM_DESC(g_bus_num, "spi bus number");
module_param(g_csn, uint, S_IRUGO);
MODULE_PARM_DESC(g_csn, "chip select number");

struct spi_master *g_xmedia_master;

static xmedia_s32 ssp_func_write(struct spi_device *xmedia_spi, xmedia_u8 addr, const xmedia_u8 *data, xmedia_u32 cnt)
{
    struct spi_master *master = g_xmedia_master;
    static struct spi_transfer t;
    static struct spi_message msg;
    static xmedia_u8 buf[4] = { 0 }; /* 4 byte buf */
    xmedia_s16 ret;
    xmedia_ulong flags;

    if (xmedia_spi == XMEDIA_NULL) {
        return -ENODEV;
    }

    xmedia_unused(cnt);

    /* check spi_message is or no finish */
    spin_lock_irqsave(&master->queue_lock, flags);

    if (msg.state != XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("msg.state not null!!\n");
        return -EFAULT;
    }

    spin_unlock_irqrestore(&master->queue_lock, flags);

    buf[0] = addr;
    buf[0] &= (~0x80);
    buf[1] = *data;

    t.tx_buf = buf;
    t.len = 2; /* length 2 */

    spi_message_init(&msg);
    spi_message_add_tail(&t, &msg);

    msg.state = &msg;
    ret = spi_sync(xmedia_spi, &msg);
    if (ret != 0) {
        MSENSOR_ERR_TRACE(" spi_async() error(%d)!\n", ret);
        return -EAGAIN;
    }
    return ret;
}

static xmedia_s32 ssp_func_read(struct spi_device *xmedia_spi, xmedia_u8 addr, xmedia_u8 *data, xmedia_u32 cnt)
{
    struct spi_master *master = g_xmedia_master;
    static struct spi_transfer t;
    static struct spi_message msg;
    static xmedia_u8 buf[4] = { 0 }; /* 4 byte buf */
    xmedia_s16 ret;
    xmedia_ulong flags;
    static xmedia_u8 body[1024] = { 0 }; /* 1024 byte buf */

    if (xmedia_spi == XMEDIA_NULL) {
        return -ENODEV;
    }

    /* check spi_message is or no finish */
    spin_lock_irqsave(&master->queue_lock, flags);

    if (msg.state != XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("msg.state not null!!(%x)\n", addr);
        spin_unlock_irqrestore(&master->queue_lock, flags);
        return -EFAULT;
    }

    spin_unlock_irqrestore(&master->queue_lock, flags);

    buf[0] = addr;
    buf[0] |= 0x80;
    buf[1] = 0;

    t.tx_buf = buf;
    t.rx_buf = body;
    t.len = cnt + 1;

    spi_message_init(&msg);
    spi_message_add_tail(&t, &msg);
    msg.state = &msg;
    ret = spi_sync(xmedia_spi, &msg);
    if (ret != 0) {
        MSENSOR_ERR_TRACE(" spi_async() error(%d)!\n", ret);
        return -EAGAIN;
    }

    osal_memcpy(data, body + 1, cnt);
    return ret;
}

xmedia_s32 msensor_spi_write(struct spi_device *xmedia_spi, xmedia_u8 addr, const xmedia_u8 *data, xmedia_u32 cnt)
{
    return ssp_func_write(xmedia_spi, addr, data, cnt);
}

xmedia_s32 msensor_spi_read(struct spi_device *xmedia_spi, xmedia_u8 addr, xmedia_u8 *data, xmedia_u32 cnt)
{
    xmedia_s16 ret;

    ret = ssp_func_read(xmedia_spi, addr, data, cnt);
    return ret;
}

xmedia_s32 msensor_spi_init(struct spi_device **xmedia_spi)
{
    xmedia_s32 status = XMEDIA_SUCCESS;
    struct device *d = XMEDIA_NULL;
    xmedia_char *spi_name = XMEDIA_NULL;
    xmedia_s32 spi_name_len;

    g_xmedia_master = spi_busnum_to_master(g_bus_num);
    if (g_xmedia_master == XMEDIA_NULL) {
        status = -ENXIO;
        goto err0;
    }

    spi_name_len = strlen(dev_name(&g_xmedia_master->dev)) + 10; /* extend 10 */

    spi_name = osal_kmalloc(spi_name_len, osal_gfp_kernel);
    if (spi_name == XMEDIA_NULL) {
        status = -ENOMEM;
        goto err0;
    }

    (xmedia_void) osal_memset(spi_name, 0, spi_name_len);
    if (osal_sprintf(spi_name, "%s.%u", dev_name(&g_xmedia_master->dev), g_csn) == XMEDIA_FAILURE) {
        status = XMEDIA_FAILURE;
        goto err1;
    }

    d = bus_find_device_by_name(&spi_bus_type, XMEDIA_NULL, spi_name);
    if (d == XMEDIA_NULL) {
        status = -ENXIO;
        goto err1;
    }
    *xmedia_spi = to_spi_device(d);
    if (*xmedia_spi == XMEDIA_NULL) {
        status = -ENXIO;
        goto err2;
    }

    spin_lock_init(&g_lock_rd);
err2:
    put_device(d);
err1:
    if (spi_name != XMEDIA_NULL) {
        osal_kfree(spi_name);
        spi_name = XMEDIA_NULL;
    }
err0:
    return status;
}

xmedia_s32 msensor_spi_deinit(const struct spi_device *spi_device)
{
    xmedia_unused(spi_device);
    return XMEDIA_SUCCESS;
}

