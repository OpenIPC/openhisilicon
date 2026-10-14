/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

/* include <liteos/i2c.h> */
#include "i2c_dev.h"
#include "linux/i2c.h"
#include "msensor.h"

struct i2c_client g_i2c_client_obj; /* i2c control struct */
#define SLAVE_ADDR     0x34 /* i2c dev addr */
#define SLAVE_REG_ADDR 0x300f /* i2c dev register */

/* client initial */
static xmedia_s32 i2c_client_init(struct i2c_client **xmedia_i2c_client)
{
    xmedia_s32 ret;

    /* struct i2c_client * i2c_client0 is &g_i2c_client_obj */
    /* i2c_client0->addr is SLAVE_ADDR >> 1 */
    xmedia_i2c_client->addr = ICM40690_DEV_ADDR;

    ret = client_attach(*xmedia_i2c_client, 0);
    if (ret != XMEDIA_SUCCESS) {
        dprintf("fail to attach client!\n");
        return -1;
    }
    return 0;
}

static xmedia_s32 sample_i2c_write(struct i2c_client *xmedia_i2c_client, xmedia_u8 reg_addr, const xmedia_u8 *reg_data,
                                   xmedia_u32 cnt)
{
    xmedia_s32 ret;

    /* struct i2c_client * i2c_client0 is & g_i2c_client_obj */
    xmedia_char buf[4] = { 0 }; /* 4 buf */
    /* i2c_client_init */
    buf[0] = reg_addr & 0xff;
    osal_memcpy(&buf[1], reg_data, cnt);
    /* buf[1] is (SLAVE_REG_ADDR >> 8) & 0xff */
    /* buf[2] is 0x03   write value to i2c */
    /* call I2C standard function drv to write */
    ret = i2c_master_send(xmedia_i2c_client, &buf, cnt + 1);
    return ret;
}

static xmedia_s32 sample_i2c_read(struct i2c_client *xmedia_i2c_client, xmedia_u8 reg_addr, xmedia_u8 *reg_data,
                                  xmedia_u32 cnt)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    /* struct i2c_client *i2c_client0 is & g_i2c_client_obj */
    struct i2c_rdwr_data rdwr;
    struct i2c_msg msg[2]; /* 2 msg */
    xmedia_u8 recvbuf[4]; /* 4 bytes */
    (xmedia_void) osal_memset(recvbuf, 0x0, sizeof(recvbuf));
    /* i2c_client_init */
    msg[0].addr = xmedia_i2c_client->addr;
    msg[0].flags = xmedia_i2c_client->flags & I2C_M_TEN;
    msg[0].len = 1;
    msg[0].buf = reg_addr;
    msg[1].addr = xmedia_i2c_client->addr;
    msg[1].flags = xmedia_i2c_client->flags & I2C_M_TEN;
    msg[1].flags |= I2C_M_RD;
    msg[1].len = cnt;
    msg[1].buf = reg_data;
    /* rdwr.msgs = &msg[0] */
    rdwr.nmsgs = 2; /* 2 msg */
    recvbuf[0] = SLAVE_REG_ADDR & 0xff;
    recvbuf[1] = (SLAVE_REG_ADDR >> 8) & 0xff; /* 8 bits */
    i2c_transfer(xmedia_i2c_client->adapter, msg, rdwr.nmsgs);
    /* dprintf("val is 0x%x\n",recvbuf[0])  buf[0] save the value read from i2c dev */
    return ret;
}

xmedia_u8 msensor_i2c_write(struct i2c_client *xmedia_i2c_client, xmedia_u8 reg_addr, const xmedia_u8 *reg_data,
                                 xmedia_u32 cnt)
{
    return sample_i2c_write(xmedia_i2c_client, reg_addr, reg_data, cnt);
}

xmedia_u8 msensor_i2c_read(struct i2c_client *xmedia_i2c_client, xmedia_u8 reg_addr, xmedia_u8 *reg_data,
                                xmedia_u32 cnt)
{
    return sample_i2c_read(xmedia_i2c_client, reg_addr, reg_data, cnt);
}

