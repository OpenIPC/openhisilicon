/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/i2c.h>
#ifdef __LITEOS__
#include <i2c.h>
#include "fcntl.h"
#endif
#include "isp_ext.h"
#include "securec.h"
#include "sensor_i2c_hal.h"
#include "ot_debug.h"

#ifndef __LITEOS__
static struct i2c_board_info g_ot_info = {
    I2C_BOARD_INFO("sensor_i2c", (0x6c >> 1)),
};
#endif
static osal_spinlock g_i2c_lock[I2C_MAX_NUM];
static struct i2c_client *g_sensor_client[I2C_MAX_NUM] = { TD_NULL };
#define i2c_err_trace(fmt, ...) \
    OT_ERR_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)


#ifdef __LITEOS__
static td_s32 sensor_i2c_send_client(struct i2c_client *client, td_u8* tmp_buf, td_s32 idx)
{
    td_s32 ret;
    td_u32 tries = 0;
    while (1) {
        ret = bsp_i2c_master_send(client, (const char *)tmp_buf, idx);
        if (ret == idx) {
            break;
        } else if ((ret == -EAGAIN)) {
            tries++;
            if (tries > 5) { /* threshold 5 */
                return TD_FAILURE;
            }
        } else {
            i2c_err_trace("ot_i2c_master_send error, ret=%d. \n", ret);
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 ot_sensor_i2c_write(td_u8 i2c_dev, td_u8 dev_addr, td_u32 reg_addr,
    td_u32 reg_addr_num, td_u32 data, td_u32 data_byte_num)
{
    td_u8 tmp_buf[8]; /* tmp buf size 8 */
    td_s32 ret;
    td_s32 idx = 0;
    struct i2c_client *client = TD_NULL;

    td_ulong flags = 0;
    td_u16 client_flags_temp;
    if (i2c_dev >= I2C_MAX_NUM) {
        return TD_FAILURE;
    }
    osal_spin_lock_irqsave(&g_i2c_lock[i2c_dev], &flags);
    if (g_sensor_client[i2c_dev] == TD_NULL) {
        osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
        return TD_FAILURE;
    }
    client = g_sensor_client[i2c_dev];
    client->addr = ((dev_addr >> 1) & 0xff);
    client_flags_temp = client->flags;
    /* reg_addr config */
    if (reg_addr_num == 1) {
        client->flags &= ~I2C_M_16BIT_REG;
        tmp_buf[idx++] = reg_addr & 0xff;
    } else {
        client->flags |= I2C_M_16BIT_REG;
        tmp_buf[idx++] = (reg_addr >> 8) & 0xff; /* reg_addr shift by 8 */
        tmp_buf[idx++] = reg_addr & 0xff;
    }

    /* data config */
    if (data_byte_num == 1) {
        client->flags &= ~I2C_M_16BIT_DATA;
        tmp_buf[idx++] = data & 0xff;
    } else {
        client->flags |= I2C_M_16BIT_DATA;
        tmp_buf[idx++] = (data >> 8) & 0xff; /* reg_addr shift by 8 */
        tmp_buf[idx++] = data & 0xff;
    }
    ret = sensor_i2c_send_client(client, tmp_buf, idx);
    client->flags = client_flags_temp;
    osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
    return ret;
}
#else
#ifndef ISP_WRITE_I2C_THROUGH_MUL_REG
static td_s32 ot_sensor_i2c_write(td_u8 i2c_dev, td_u8 dev_addr, td_u32 reg_addr,
    td_u32 reg_addr_num, td_u32 data, td_u32 data_byte_num)
{
    td_u8 tmp_buf[8]; /* tmp buf size 8 */
    td_s32 ret;
    td_s32 idx = 0;
    struct i2c_client *client = TD_NULL;
    td_u32 tries = 0;
    td_ulong flags = 0;

    if (i2c_dev >= I2C_MAX_NUM) {
        return TD_FAILURE;
    }
    osal_spin_lock_irqsave(&g_i2c_lock[i2c_dev], &flags);
    if (g_sensor_client[i2c_dev] == TD_NULL) {
        osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
        return TD_FAILURE;
    }
    client = g_sensor_client[i2c_dev];
    client->addr = (dev_addr >> 1);

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
        ret = bsp_i2c_master_send(client, (const char *)tmp_buf, idx);
        if (ret == idx) {
            break;
        } else if ((ret == -EAGAIN) && (in_atomic() || irqs_disabled())) {
            tries++;
            if (tries > 5) { /* threshold 5 */
                osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
                return TD_FAILURE;
            }
        } else {
            osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
            i2c_err_trace("ot_i2c_master_send error, ret=%d. \n", ret);
            return ret;
        }
    }
    osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
    return 0;
}
#else
static td_s32 ot_sensor_i2c_write(td_u8 i2c_dev, td_u8 dev_addr, td_s8* data_buf,
    td_u8 data_buf_len, td_u8 data_len, td_u32 reg_addr_num, td_u32 data_byte_num)
{
    td_s32 ret;
    td_u8 tries = 0;
    td_u8 byte_num;
    struct i2c_client *client = TD_NULL;
    td_ulong flags = 0;

    if (data_buf == TD_NULL) {
        return TD_FAILURE;
    }
    if (data_len >= data_buf_len) {
        return TD_FAILURE;
    }
    if (i2c_dev >= I2C_MAX_NUM) {
        return TD_FAILURE;
    }
    osal_spin_lock_irqsave(&g_i2c_lock[i2c_dev], &flags);
    if (g_sensor_client[i2c_dev] == TD_NULL) {
        osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
        return TD_FAILURE;
    }

    client = g_sensor_client[i2c_dev];

    client->addr = (dev_addr >> 1);
    byte_num = reg_addr_num + data_byte_num;
    while (1) {
        ret = bsp_i2c_master_send_mul_reg(client, (const char *)data_buf, data_len, byte_num);
        if (ret == data_len) {
            break;
        } else if ((ret == -EAGAIN) && (in_atomic() || irqs_disabled())) {
            tries++;
            if (tries > 5) { /* threshold 5 */
                osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
                return TD_FAILURE;
            }
        } else {
            osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
            i2c_err_trace("bsp_i2c_master_send_mul_reg error, ret=%d. \n", ret);
            return ret;
        }
    }
    osal_spin_unlock_irqrestore(&g_i2c_lock[i2c_dev], &flags);
    return 0;
}
#endif
#endif

static td_s32 ot_dev_isp_unregister(td_void)
{
    td_s32 i;
    isp_bus_callback bus_cb = {0};

    bus_cb.pfn_isp_write_i2c_data = TD_NULL;
    if (ckfn_isp() && (ckfn_isp_register_bus_callback())) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            call_isp_register_bus_callback(i, ISP_BUS_TYPE_I2C, &bus_cb);
        }
    } else {
        i2c_err_trace("unregister i2c_write_callback to isp failed, ot_i2c exit is failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 ot_dev_isp_register(td_void)
{
    td_s32 i, ret;
    isp_bus_callback bus_cb = {0};

    bus_cb.pfn_isp_write_i2c_data = ot_sensor_i2c_write;
    if (ckfn_isp() && (ckfn_isp_register_bus_callback())) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            ret = call_isp_register_bus_callback(i, ISP_BUS_TYPE_I2C, &bus_cb);
            if (ret != TD_SUCCESS) {
                i2c_err_trace("register i2c_write_callback to isp[%d] failed!\n", i);
            }
        }
    } else {
        i2c_err_trace("register i2c_write_callback to isp failed, ot_i2c init is failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifdef __LITEOS__
static struct i2c_client g_sensor_client_obj[I2C_MAX_NUM];
static struct i2c_client *ot_sensor_i2c_client_init(td_s32 i2c_dev)
{
    td_s32 ret;
    struct i2c_client *i2c_client0 = &g_sensor_client_obj[i2c_dev];
    i2c_client0->addr = 0x6c >> 1;
    ret = client_attach(i2c_client0, i2c_dev);
    if (ret) {
        dprintf("Fail to attach client!\n");
        return TD_NULL;
    }
    return &g_sensor_client_obj[i2c_dev];
}

td_s32 i2c_mod_init(td_void)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < I2C_MAX_NUM; i++) {
        g_sensor_client[i] = ot_sensor_i2c_client_init(i);
        osal_spin_lock_init(&g_i2c_lock[i]);
    }

    ret = ot_dev_isp_register();
    if (ret != TD_SUCCESS) {
        for (i = 0; i < I2C_MAX_NUM; i++) {
            osal_spin_lock_destroy(&g_i2c_lock[i]);
            g_sensor_client[i] = TD_NULL;
        }
        printf("i2c_mod_init failed !\n");
        return -1;
    }

    return 0;
}

td_void i2c_mod_exit(td_void)
{
    td_s32 i;
    for (i = 0; i < I2C_MAX_NUM; i++) {
        osal_spin_lock_destroy(&g_i2c_lock[i]);
        g_sensor_client[i] = TD_NULL;
    }
}

#else
td_s32 i2c_mod_init(td_void)
{
    td_s32 i;
    td_s32 ret;
    struct i2c_adapter *i2c_adap = TD_NULL;

    for (i = 0; i < I2C_MAX_NUM; i++) {
        i2c_adap = i2c_get_adapter(i);
        if (i2c_adap != TD_NULL) {
            g_sensor_client[i] =  i2c_new_client_device(i2c_adap, &g_ot_info);

            i2c_put_adapter(i2c_adap);
        } else {
            osal_printk("i2c:%d get adapter error!\n", i);
        }
        osal_spin_lock_init(&g_i2c_lock[i]);
    }

    ret = ot_dev_isp_register();
    if (ret != TD_SUCCESS) {
        for (i = 0; i < I2C_MAX_NUM; i++) {
            osal_spin_lock_destroy(&g_i2c_lock[i]);
            if (g_sensor_client[i] != TD_NULL) {
                i2c_unregister_device(g_sensor_client[i]);
                g_sensor_client[i] = TD_NULL;
            }
        }
        osal_printk("i2c_mod_init failed !\n");
        return -1;
    }
    osal_printk("load sensor_i2c.ko ....OK!\n");

    return 0;
}
td_void i2c_mod_exit(td_void)
{
    td_s32 i;

    for (i = 0; i < I2C_MAX_NUM; i++) {
        if (g_sensor_client[i] != TD_NULL) {
            i2c_unregister_device(g_sensor_client[i]);
            g_sensor_client[i] = TD_NULL;
        }
        osal_spin_lock_destroy(&g_i2c_lock[i]);
    }

    ot_dev_isp_unregister();
    osal_printk("unload sensor_i2c.ko ....OK!\n");
}
#endif
