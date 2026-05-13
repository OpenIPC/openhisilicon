/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of sensor_i2c.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/preempt.h>
#include <linux/printk.h>

#include "../../../compat/kernel_compat.h"

#ifdef __HuaweiLite__
#include <i2c.h>
#include "fcntl.h"
#endif
#include <linux/delay.h>
#include "isp_ext.h"

#define I2C_MAX_NUM 7
#define ISP_MAX_DEV 4
#ifndef __HuaweiLite__
static struct i2c_board_info g_hi_info = {
    I2C_BOARD_INFO("sensor_i2c", (0x6c >> 1)),
};
#endif

static struct i2c_client *g_sensor_client[I2C_MAX_NUM] = { HI_NULL };

#ifndef __HuaweiLite__
/*
 * The cv500 ISP vendor blob calls hi_sensor_i2c_write() from its IRQ
 * bottom-half (isp_drv_int_bottom_half) for per-frame AE/AGC reconfig.
 * i2c_master_send() acquires the adapter's rt_mutex, which trips WARN_ON
 * in atomic context on 4.9 kernels and then the controller logs
 * "wait idle abort" for the failed transfer. Defer such writes to a
 * per-bus ordered workqueue so the actual i2c transfer runs in process
 * context, while process-context callers (sensor init, MPI ioctls) keep
 * the original synchronous fast path.
 */
#define SENSOR_I2C_DEFER_RING 64
struct sensor_i2c_defer_entry {
    unsigned char  dev_addr;
    unsigned int   reg_addr;
    unsigned int   reg_addr_num;
    unsigned int   data;
    unsigned int   data_byte_num;
};
struct sensor_i2c_bus_defer {
    struct workqueue_struct *wq;
    struct work_struct       work;
    spinlock_t               lock;
    struct sensor_i2c_defer_entry ring[SENSOR_I2C_DEFER_RING];
    unsigned int head;
    unsigned int tail;
    unsigned int drops;
};
static struct sensor_i2c_bus_defer g_defer[I2C_MAX_NUM];

static int do_sensor_i2c_write_sync(unsigned char i2c_dev, unsigned char dev_addr,
                                    unsigned int reg_addr, unsigned int reg_addr_num,
                                    unsigned int data, unsigned int data_byte_num);

static void sensor_i2c_defer_worker(struct work_struct *w)
{
    struct sensor_i2c_bus_defer *bd = container_of(w, struct sensor_i2c_bus_defer, work);
    unsigned int bus = bd - g_defer;
    struct sensor_i2c_defer_entry e;
    unsigned long flags;
    int ret;

    while (1) {
        spin_lock_irqsave(&bd->lock, flags);
        if (bd->head == bd->tail) {
            spin_unlock_irqrestore(&bd->lock, flags);
            break;
        }
        e = bd->ring[bd->tail & (SENSOR_I2C_DEFER_RING - 1)];
        bd->tail++;
        spin_unlock_irqrestore(&bd->lock, flags);

        ret = do_sensor_i2c_write_sync((unsigned char)bus, e.dev_addr,
                                       e.reg_addr, e.reg_addr_num,
                                       e.data, e.data_byte_num);
        if (ret != 0) {
            printk_ratelimited(KERN_WARNING
                "open_sensor_i2c: deferred write bus=%u dev=0x%02x reg=0x%x failed (%d)\n",
                bus, e.dev_addr, e.reg_addr, ret);
        }
    }
}

static int sensor_i2c_enqueue_defer(unsigned char i2c_dev, unsigned char dev_addr,
                                    unsigned int reg_addr, unsigned int reg_addr_num,
                                    unsigned int data, unsigned int data_byte_num)
{
    struct sensor_i2c_bus_defer *bd = &g_defer[i2c_dev];
    unsigned long flags;
    unsigned int used;

    if (bd->wq == HI_NULL) {
        return -ENODEV;
    }

    spin_lock_irqsave(&bd->lock, flags);
    used = bd->head - bd->tail;
    if (used >= SENSOR_I2C_DEFER_RING) {
        bd->drops++;
        spin_unlock_irqrestore(&bd->lock, flags);
        printk_ratelimited(KERN_WARNING
            "open_sensor_i2c: defer ring full on bus %u (drops=%u)\n",
            i2c_dev, bd->drops);
        return -EBUSY;
    }
    bd->ring[bd->head & (SENSOR_I2C_DEFER_RING - 1)] =
        (struct sensor_i2c_defer_entry){
            .dev_addr      = dev_addr,
            .reg_addr      = reg_addr,
            .reg_addr_num  = reg_addr_num,
            .data          = data,
            .data_byte_num = data_byte_num,
        };
    bd->head++;
    spin_unlock_irqrestore(&bd->lock, flags);

    queue_work(bd->wq, &bd->work);
    return 0;
}
#endif /* !__HuaweiLite__ */

#ifdef __HuaweiLite__
static int hi_sensor_i2c_write(unsigned char i2c_dev, unsigned char dev_addr, unsigned int reg_addr,
                               unsigned int reg_addr_num, unsigned int data, unsigned int data_byte_num)
{
    unsigned char tmp_buf[8]; /* tmp buf size 8 */
    int ret;
    int idx = 0;
    struct i2c_client client;
    hi_u32 tries = 0;

    if (i2c_dev >= I2C_MAX_NUM) {
        return HI_FAILURE;
    }

    if (g_sensor_client[i2c_dev] == HI_NULL) {
        return HI_FAILURE;
    }

    osal_memcpy(&client, g_sensor_client[i2c_dev], sizeof(struct i2c_client));

    client.addr = ((dev_addr >> 1) & 0xff);

    /* reg_addr config */
    if (reg_addr_num == 1) {
        client.flags &= ~I2C_M_16BIT_REG;
        tmp_buf[idx++] = reg_addr & 0xff;
    } else {
        client.flags |= I2C_M_16BIT_REG;
        tmp_buf[idx++] = (reg_addr >> 8) & 0xff; /* reg_addr shift by 8 */
        tmp_buf[idx++] = reg_addr & 0xff;
    }

    /* data config */
    if (data_byte_num == 1) {
        client.flags &= ~I2C_M_16BIT_DATA;
        tmp_buf[idx++] = data & 0xff;
    } else {
        client.flags |= I2C_M_16BIT_DATA;
        tmp_buf[idx++] = (data >> 8) & 0xff; /* reg_addr shift by 8 */
        tmp_buf[idx++] = data & 0xff;
    }

    while (1) {
        ret = i2c_master_send(&client, (const char *)tmp_buf, idx);
        if (ret == idx) {
            break;
        } else if ((ret == -EAGAIN)) {
            tries++;
            if (tries > 5) { /* threshold 5 */
                return HI_FAILURE;
            }
        } else {
            osal_printk("[%s %d] i2c_master_send error, ret=%d. \n", __func__, __LINE__, ret);
            return ret;
        }
    }

    return 0;
}
#else
static int do_sensor_i2c_write_sync(unsigned char i2c_dev, unsigned char dev_addr,
                                    unsigned int reg_addr, unsigned int reg_addr_num,
                                    unsigned int data, unsigned int data_byte_num)
{
    unsigned char tmp_buf[8]; /* tmp buf size 8 */
    int ret;
    int idx = 0;
    struct i2c_client client;
    hi_u32 tries = 0;

    if (i2c_dev >= I2C_MAX_NUM) {
        return HI_FAILURE;
    }

    if (g_sensor_client[i2c_dev] == HI_NULL) {
        return HI_FAILURE;
    }

    osal_memcpy(&client, g_sensor_client[i2c_dev], sizeof(struct i2c_client));

    client.addr = (dev_addr >> 1);

    /* reg_addr config */
    if (reg_addr_num == 1) {
        tmp_buf[idx++] = reg_addr & 0xff;
    } else {
        tmp_buf[idx++] = (reg_addr >> 8) & 0xff; /* reg_addr shift by 8 */
        tmp_buf[idx++] = reg_addr & 0xff;
    }

    /* data config */
    if (data_byte_num == 1) {
        tmp_buf[idx++] = data & 0xff;
    } else {
        tmp_buf[idx++] = (data >> 8) & 0xff; /* reg_addr shift by 8 */
        tmp_buf[idx++] = data & 0xff;
    }

    while (1) {
        ret = i2c_master_send(&client, (const char *)tmp_buf, idx);
        if (ret == idx) {
            break;
        } else if ((ret == -EAGAIN) && (in_atomic() || irqs_disabled())) {
            tries++;
            if (tries > 5) { /* threshold 5 */
                return HI_FAILURE;
            }
        } else {
            osal_printk("[%s %d] i2c_master_send error, ret=%d. \n", __func__, __LINE__, ret);
            return ret;
        }
    }

    return 0;
}

static int hi_sensor_i2c_write(unsigned char i2c_dev, unsigned char dev_addr, unsigned int reg_addr,
                               unsigned int reg_addr_num, unsigned int data, unsigned int data_byte_num)
{
    if (i2c_dev >= I2C_MAX_NUM) {
        return HI_FAILURE;
    }

    if (in_interrupt() || in_atomic() || irqs_disabled()) {
        int qret = sensor_i2c_enqueue_defer(i2c_dev, dev_addr, reg_addr,
                                            reg_addr_num, data, data_byte_num);
        if (qret == 0) {
            return 0;
        }
        /* Workqueue not initialised or ring full -- fall through to the
         * synchronous path so the write still attempts to land. The
         * legacy WARN may fire in that fallback case. */
    }

    return do_sensor_i2c_write_sync(i2c_dev, dev_addr, reg_addr,
                                    reg_addr_num, data, data_byte_num);
}
#endif

static int hi_dev_isp_register(void)
{
    int i = 0;
    isp_bus_callback bus_cb = {0};

    bus_cb.pfn_isp_write_i2c_data = hi_sensor_i2c_write;
    if (ckfn_isp() && (ckfn_isp_register_bus_callback())) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            call_isp_register_bus_callback(i, ISP_BUS_TYPE_I2C, &bus_cb);
        }
    } else {
        osal_printk("register i2c_write_callback to isp failed, hi_i2c init is failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __HuaweiLite__
static struct i2c_client g_sensor_client_obj[I2C_MAX_NUM];
static struct i2c_client *hi_sensor_i2c_client_init(int i2c_dev)
{
    int ret;
    struct i2c_client *i2c_client0 = &g_sensor_client_obj[i2c_dev];
    i2c_client0->addr = 0x6c >> 1;
    ret = client_attach(i2c_client0, i2c_dev);
    if (ret) {
        dprintf("Fail to attach client!\n");
        return HI_NULL;
    }
    return &g_sensor_client_obj[i2c_dev];
}

int hi_dev_init(void)
{
    int i;
    int ret;

    for (i = 0; i < I2C_MAX_NUM; i++) {
        g_sensor_client[i] = hi_sensor_i2c_client_init(i);
    }

    ret = hi_dev_isp_register();
    if (ret != HI_SUCCESS) {
        printf("hi_dev_init failed !\n");
        return -1;
    }

    return 0;
}

void hi_dev_exit(void)
{
}

#else
static int __init hi_dev_init(void)
{
    int i;
    struct i2c_adapter *i2c_adap = HI_NULL;

    for (i = 0; i < I2C_MAX_NUM; i++) {
        i2c_adap = i2c_get_adapter(i);
        if (i2c_adap != HI_NULL) {
            g_sensor_client[i] = i2c_new_client_device(i2c_adap, &g_hi_info);

            i2c_put_adapter(i2c_adap);

            spin_lock_init(&g_defer[i].lock);
            INIT_WORK(&g_defer[i].work, sensor_i2c_defer_worker);
            g_defer[i].wq = alloc_ordered_workqueue("sensor_i2c_%d", WQ_MEM_RECLAIM, i);
            if (g_defer[i].wq == HI_NULL) {
                osal_printk("open_sensor_i2c: alloc_ordered_workqueue bus %d failed\n", i);
            }
        } else {
            osal_printk("i2c:%d get adapter error!\n", i);
        }
    }

    hi_dev_isp_register();

    return 0;
}
static void __exit hi_dev_exit(void)
{
    int i;

    for (i = 0; i < I2C_MAX_NUM; i++) {
        if (g_defer[i].wq != HI_NULL) {
            cancel_work_sync(&g_defer[i].work);
            destroy_workqueue(g_defer[i].wq);
            g_defer[i].wq = HI_NULL;
        }
        if (g_sensor_client[i] != HI_NULL) {
            i2c_unregister_device(g_sensor_client[i]);
        }
    }
}

#ifndef __HuaweiLite__
module_init(hi_dev_init);
#else
module_init(((void *)hi_dev_init));
#endif
module_exit(hi_dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");

#endif
