/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/i2c.h>

#ifdef __LITEOS__
#include <i2c.h>
#include "fcntl.h"
#endif

#include <linux/delay.h>
#include "isp_ext.h"

#define I2C_MAX_NUM 2
#define ISP_MAX_DEV 2

#ifndef __LITEOS__
static struct i2c_board_info info = {
	I2C_BOARD_INFO("sensor_i2c", (0x6c >> 1)),
};
#endif

static struct i2c_client *sensor_client[I2C_MAX_NUM];

static int sensor_i2c_write(unsigned char i2c_dev, unsigned char dev_addr,
			    unsigned int reg_addr, unsigned int reg_addr_num,
			    unsigned int data, unsigned int data_byte_num)
{
	unsigned char tmp_buf[8];
	int ret = 0;
	int idx = 0;
	struct i2c_client client;
	GK_U32 u32Tries = 0;

	if (i2c_dev >= I2C_MAX_NUM) {
		return GK_FAILURE;
	}

	if (sensor_client[i2c_dev] == NULL) {
		return GK_FAILURE;
	}

	osal_memcpy(&client, sensor_client[i2c_dev], sizeof(struct i2c_client));

#ifdef __LITEOS__
	client.addr = ((dev_addr >> 1) & 0xff);

	/* reg_addr config */
	if (reg_addr_num == 1) {
		client.flags &= ~I2C_M_16BIT_REG;
		tmp_buf[idx++] = reg_addr & 0xff;
	} else {
		client.flags |= I2C_M_16BIT_REG;
		tmp_buf[idx++] = (reg_addr >> 8) & 0xff;
		tmp_buf[idx++] = reg_addr & 0xff;
	}

	/* data config */
	if (data_byte_num == 1) {
		client.flags &= ~I2C_M_16BIT_DATA;
		tmp_buf[idx++] = data & 0xff;
	} else {
		client.flags |= I2C_M_16BIT_DATA;
		tmp_buf[idx++] = (data >> 8) & 0xff;
		tmp_buf[idx++] = data & 0xff;
	}
#else
	client.addr = (dev_addr >> 1);

	/* reg_addr config */
	if (reg_addr_num == 1) {
		tmp_buf[idx++] = reg_addr & 0xff;
	} else {
		tmp_buf[idx++] = (reg_addr >> 8) & 0xff;
		tmp_buf[idx++] = reg_addr & 0xff;
	}

	/* data config */
	if (data_byte_num == 1) {
		tmp_buf[idx++] = data & 0xff;
	} else {
		tmp_buf[idx++] = (data >> 8) & 0xff;
		tmp_buf[idx++] = data & 0xff;
	}
#endif

	while (1) {
		ret = gk_i2c_master_send(&client, (const char *)tmp_buf, idx);
		if (ret == idx) {
			break;
		}
#ifdef __LITEOS__
		else if ((ret == -EAGAIN))
#else
		else if ((ret == -EAGAIN) && (in_atomic() || irqs_disabled()))
#endif
		{
			u32Tries++;
			if (u32Tries > 5) {
				return GK_FAILURE;
			}
		} else {
			osal_printk(
				"[%s %d] gk_i2c_master_send error, ret=%d. \n",
				__func__, __LINE__, ret);
			return ret;
		}
	}

	return 0;
}

static int dev_isp_register(void)
{
	int i = 0;
	ISP_BUS_CALLBACK_S stBusCb = { 0 };

	stBusCb.pfnISPWriteI2CData = sensor_i2c_write;
	if ((FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) != NULL) &&
	    (CKFN_ISP_RegisterBusCallBack())) {
		for (i = 0; i < ISP_MAX_DEV; i++) {
			CALL_ISP_RegisterBusCallBack(i, ISP_BUS_TYPE_I2C,
						     &stBusCb);
		}
	} else {
		osal_printk(
			"register i2c_write_callback to isp failed, i2c init is failed!\n");
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

#ifdef __LITEOS__
static struct i2c_client sensor_client_obj[I2C_MAX_NUM];
static struct i2c_client *sensor_i2c_client_init(int i2c_dev)
{
	int ret = 0;
	struct i2c_client *i2c_client0 = &sensor_client_obj[i2c_dev];
	i2c_client0->addr = 0x6c >> 1;
	ret = client_attach(i2c_client0, i2c_dev);
	if (ret) {
		dprintf("Fail to attach client!\n");
		return NULL;
	}
	return &sensor_client_obj[i2c_dev];
}

int dev_init(void)
{
	int i = 0;

	for (i = 0; i < I2C_MAX_NUM; i++) {
		sensor_client[i] = sensor_i2c_client_init(i);
	}

	dev_isp_register();

	return 0;
}

void dev_exit(void)
{
}

#else
static int __init dev_init(void)
{
	int i = 0;
	struct i2c_adapter *i2c_adap;

	for (i = 0; i < I2C_MAX_NUM; i++) {
		i2c_adap = i2c_get_adapter(i);
		sensor_client[i] = i2c_new_device(i2c_adap, &info);

		i2c_put_adapter(i2c_adap);
	}

	dev_isp_register();

	return 0;
}
static void __exit dev_exit(void)
{
	int i = 0;

	for (i = 0; i < I2C_MAX_NUM; i++) {
		i2c_unregister_device(sensor_client[i]);
	}
}

#ifndef __LITEOS__
module_init(dev_init);
#else
module_init(((void *)dev_init));
#endif

module_exit(dev_exit);
MODULE_LICENSE("GPL");

#endif
