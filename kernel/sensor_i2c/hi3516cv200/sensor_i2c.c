#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/hardirq.h>
#include <linux/version.h>

#include <linux/delay.h>
#include "isp_ext.h"

/*
 * OpenIPC's hisilicon-hi3516cv200 kernel patches i2c with a vendor
 * extension: `hi_i2c_master_send` calls `hibvt_i2c_xfer` directly,
 * bypassing i2c-core's rt_mutex_trylock so it is safe from the ISP_ISR
 * hot path. The vendor uapi i2c.h also defines I2C_M_16BIT_REG and
 * I2C_M_16BIT_DATA so the controller can switch reg/data width per
 * transfer. On mainline kernels neither exists; we fall back to the
 * standard i2c API (which WARNs from atomic context via
 * rt_mutex_trylock — see OpenIPC/firmware#2144) and lose true 16-bit
 * sensor addressing until a kernel-side patch lands.
 *
 * Gate the entire compat shim on the I2C_M_16BIT_REG macro — it is
 * the only macro symbol the vendor provides that the preprocessor can
 * see. The function symbol `hi_i2c_master_send` is an extern, so a
 * naive `#ifndef hi_i2c_master_send` triggers the macro fallback even
 * on the vendor kernel, silently routing all sensor writes through
 * rt_mutex_trylock and making AE writes fail from ISR context.
 */
#ifndef I2C_M_16BIT_REG
#define hi_i2c_master_send i2c_master_send
#define hi_i2c_master_recv i2c_master_recv
#define I2C_M_16BIT_REG 0
#define I2C_M_16BIT_DATA 0
#endif

/* i2c_new_device() removed in 5.x — replaced by i2c_new_client_device()
 * which returns ERR_PTR on failure (vs NULL). */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
#define i2c_new_device(adap, info) i2c_new_client_device((adap), (info))
#endif

static struct i2c_board_info hi_info =
{
     I2C_BOARD_INFO("sensor_i2c", 0x6c),
};

static struct i2c_client *sensor_client;

int hi_i2c_read(unsigned char dev_addr, unsigned int reg_addr,
                unsigned int reg_addr_num, unsigned int data_byte_num);

int hi_sensor_i2c_write(unsigned char dev_addr,
                        unsigned int reg_addr, unsigned int reg_addr_num,
                        unsigned int data, unsigned int data_byte_num)
{
    unsigned char tmp_buf[8];
	int ret = 0;
    int idx = 0;
	struct i2c_client *client = sensor_client;
    unsigned int u32Tries = 0;

    sensor_client->addr = dev_addr;

    /* reg_addr config */
    tmp_buf[idx++] = reg_addr;
	if (reg_addr_num == 2)
	{
		client->flags  |= I2C_M_16BIT_REG;
        tmp_buf[idx++]  = (reg_addr >> 8);
	}
    else
    {
        client->flags &= ~I2C_M_16BIT_REG;
    }

    /* data config */
    tmp_buf[idx++] = data;
	if (data_byte_num == 2)
	{
		client->flags  |= I2C_M_16BIT_DATA;
        tmp_buf[idx++] = data >> 8;
	}
    else
    {
        client->flags &= ~I2C_M_16BIT_DATA;
    }

    while (1)
    {
        ret = hi_i2c_master_send(client, tmp_buf, idx);
        if (ret == idx)
        {
            break;
        }
        else if ((ret == -EAGAIN) && (in_atomic() || irqs_disabled()))
        {
            u32Tries++;
            if (u32Tries > 5)
            {
                return -1;
            }
        }
        else
        {
            printk("[%s %d] hi_i2c_master_send error, ret=%d. \n", __func__, __LINE__,
                ret);
            return ret;
        }
    }

	return 0;
}

int hi_i2c_read(unsigned char dev_addr, unsigned int reg_addr,
                unsigned int reg_addr_num, unsigned int data_byte_num)
{
    unsigned char tmp_buf[8];
	int ret = 0;
    int ret_data = 0xFF;
    int idx = 0;
	struct i2c_client *client = sensor_client;

    sensor_client->addr = dev_addr;

    /* reg_addr config */
    tmp_buf[idx++] = reg_addr;
	if (reg_addr_num == 2)
	{
		client->flags  |= I2C_M_16BIT_REG;
        tmp_buf[idx++] = reg_addr >> 8;
	}
    else
    {
        client->flags &= ~I2C_M_16BIT_REG;
    }

    /* data config */
	if (data_byte_num == 2)
	{
		client->flags |= I2C_M_16BIT_DATA;
	}
    else
    {
        client->flags &= ~I2C_M_16BIT_DATA;
    }

    while (1)
    {
        ret = hi_i2c_master_recv(client, tmp_buf, idx);;
        if (ret == idx)
        {
            if (data_byte_num == 2)
            {
                ret_data = tmp_buf[0] | (tmp_buf[1] << 8);
            }
            else
            {
                ret_data = tmp_buf[0];
            }
            break;
        }
        else if ((ret == -EAGAIN) && (in_atomic() || irqs_disabled()))
        {
            continue;
        }
        else
        {
            printk("[%s %d] hi_i2c_master_recv error, ret=%d. \n", __func__, __LINE__,
                ret);
            break;
        }
    }

	return ret_data;
}

static int hi_dev_isp_register(void)
{
    ISP_BUS_CALLBACK_S stBusCb = {0};

    stBusCb.pfnISPWriteI2CData = hi_sensor_i2c_write;
    if (CKFN_ISP_RegisterBusCallBack())
    {
        CALL_ISP_RegisterBusCallBack(0, ISP_BUS_TYPE_I2C, &stBusCb);
    }
    else
    {
        printk("register i2c_write_callback to isp failed, hi_i2c init is failed!\n");
        return -1;
    }

    return 0;
}

static int hi_dev_init(void)
{
    struct i2c_adapter *i2c_adap;

    // use i2c0
    i2c_adap = i2c_get_adapter(0);
	sensor_client = i2c_new_device(i2c_adap, &hi_info);

	i2c_put_adapter(i2c_adap);
    hi_dev_isp_register();

    printk(KERN_INFO "load sensor_i2c.ko ...OK!\n");
    return 0;
}

static void hi_dev_exit(void)
{
    i2c_unregister_device(sensor_client);
    printk(KERN_INFO "unload sensor_i2c.ko ...OK!\n");
}

module_init(hi_dev_init);
module_exit(hi_dev_exit);
MODULE_LICENSE("GPL");
