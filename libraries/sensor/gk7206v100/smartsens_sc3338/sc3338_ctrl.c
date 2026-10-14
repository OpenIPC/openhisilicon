#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc3338_ctrl.h"
#include "sc3338.h"

SENSOR_PRIORITY_DATA static xmedia_s32 g_sc3338_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_sc3338_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
#define SC3338_REG_ADDR_MIRROR_FLIP 0x3221

SENSOR_PRIORITY_FUNC static xmedia_s32 sc3338_2lane_linear_2304x1296_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc3338_write_reg(dev, 0x0103, 0x01);
    sc3338_delay_ms(3);
    ret |= sc3338_write_reg(dev, 0x36e9,0x80);
    ret |= sc3338_write_reg(dev, 0x37f9,0x80);
    ret |= sc3338_write_reg(dev, 0x301f,0x01);
    ret |= sc3338_write_reg(dev, 0x30b8,0x33);
	ret |= sc3338_write_reg(dev, 0x320e,0x02);
	ret |= sc3338_write_reg(dev, 0x320f,0xa3);
    ret |= sc3338_write_reg(dev, 0x3253,0x10);
    ret |= sc3338_write_reg(dev, 0x325f,0x20);
    ret |= sc3338_write_reg(dev, 0x3301,0x04);
    ret |= sc3338_write_reg(dev, 0x3306,0x50);
    ret |= sc3338_write_reg(dev, 0x330a,0x00);
    ret |= sc3338_write_reg(dev, 0x330b,0xd8);
    ret |= sc3338_write_reg(dev, 0x3314,0x13);
    ret |= sc3338_write_reg(dev, 0x3333,0x10);
    ret |= sc3338_write_reg(dev, 0x3334,0x40);
    ret |= sc3338_write_reg(dev, 0x335e,0x06);
    ret |= sc3338_write_reg(dev, 0x335f,0x0a);
    ret |= sc3338_write_reg(dev, 0x3364,0x5e);
    ret |= sc3338_write_reg(dev, 0x337c,0x02);
    ret |= sc3338_write_reg(dev, 0x337d,0x0e);
    ret |= sc3338_write_reg(dev, 0x3390,0x01);
    ret |= sc3338_write_reg(dev, 0x3391,0x03);
    ret |= sc3338_write_reg(dev, 0x3392,0x07);
    ret |= sc3338_write_reg(dev, 0x3393,0x04);
    ret |= sc3338_write_reg(dev, 0x3394,0x04);
    ret |= sc3338_write_reg(dev, 0x3395,0x04);
    ret |= sc3338_write_reg(dev, 0x3396,0x08);
    ret |= sc3338_write_reg(dev, 0x3397,0x0b);
    ret |= sc3338_write_reg(dev, 0x3398,0x1f);
    ret |= sc3338_write_reg(dev, 0x3399,0x04);
    ret |= sc3338_write_reg(dev, 0x339a,0x0a);
    ret |= sc3338_write_reg(dev, 0x339b,0x3a);
    ret |= sc3338_write_reg(dev, 0x339c,0xc4);
    ret |= sc3338_write_reg(dev, 0x33a2,0x04);
    ret |= sc3338_write_reg(dev, 0x33ac,0x08);
    ret |= sc3338_write_reg(dev, 0x33ad,0x1c);
    ret |= sc3338_write_reg(dev, 0x33ae,0x10);
    ret |= sc3338_write_reg(dev, 0x33af,0x30);
    ret |= sc3338_write_reg(dev, 0x33b1,0x80);
    ret |= sc3338_write_reg(dev, 0x33b3,0x48);
    ret |= sc3338_write_reg(dev, 0x33f9,0x60);
    ret |= sc3338_write_reg(dev, 0x33fb,0x74);
    ret |= sc3338_write_reg(dev, 0x33fc,0x4b);
    ret |= sc3338_write_reg(dev, 0x33fd,0x5f);
    ret |= sc3338_write_reg(dev, 0x349f,0x03);
    ret |= sc3338_write_reg(dev, 0x34a6,0x4b);
    ret |= sc3338_write_reg(dev, 0x34a7,0x5f);
    ret |= sc3338_write_reg(dev, 0x34a8,0x20);
    ret |= sc3338_write_reg(dev, 0x34a9,0x18);
    ret |= sc3338_write_reg(dev, 0x34ab,0xe8);
    ret |= sc3338_write_reg(dev, 0x34ac,0x01);
    ret |= sc3338_write_reg(dev, 0x34ad,0x00);
    ret |= sc3338_write_reg(dev, 0x34f8,0x5f);
    ret |= sc3338_write_reg(dev, 0x34f9,0x18);
    ret |= sc3338_write_reg(dev, 0x3630,0xc0);
    ret |= sc3338_write_reg(dev, 0x3631,0x84);
    ret |= sc3338_write_reg(dev, 0x3632,0x64);
    ret |= sc3338_write_reg(dev, 0x3633,0x32);
    ret |= sc3338_write_reg(dev, 0x363b,0x03);
    ret |= sc3338_write_reg(dev, 0x363c,0x08);
    ret |= sc3338_write_reg(dev, 0x3641,0x38);
    ret |= sc3338_write_reg(dev, 0x3670,0x4e);
    ret |= sc3338_write_reg(dev, 0x3674,0xc0);
    ret |= sc3338_write_reg(dev, 0x3675,0xc0);
    ret |= sc3338_write_reg(dev, 0x3676,0xc0);
    ret |= sc3338_write_reg(dev, 0x3677,0x84);
    ret |= sc3338_write_reg(dev, 0x3678,0x8a);
    ret |= sc3338_write_reg(dev, 0x3679,0x8c);
    ret |= sc3338_write_reg(dev, 0x367c,0x48);
    ret |= sc3338_write_reg(dev, 0x367d,0x49);
    ret |= sc3338_write_reg(dev, 0x367e,0x4b);
    ret |= sc3338_write_reg(dev, 0x367f,0x5f);
    ret |= sc3338_write_reg(dev, 0x3690,0x33);
    ret |= sc3338_write_reg(dev, 0x3691,0x33);
    ret |= sc3338_write_reg(dev, 0x3692,0x44);
    ret |= sc3338_write_reg(dev, 0x369c,0x4b);
    ret |= sc3338_write_reg(dev, 0x369d,0x5f);
    ret |= sc3338_write_reg(dev, 0x36b0,0x87);
    ret |= sc3338_write_reg(dev, 0x36b1,0x90);
    ret |= sc3338_write_reg(dev, 0x36b2,0xa1);
    ret |= sc3338_write_reg(dev, 0x36b3,0xd8);
    ret |= sc3338_write_reg(dev, 0x36b4,0x49);
    ret |= sc3338_write_reg(dev, 0x36b5,0x4b);
    ret |= sc3338_write_reg(dev, 0x36b6,0x4f);
    ret |= sc3338_write_reg(dev, 0x370f,0x01);
    ret |= sc3338_write_reg(dev, 0x3722,0x09);
    ret |= sc3338_write_reg(dev, 0x3724,0x41);
    ret |= sc3338_write_reg(dev, 0x3725,0xc1);
    ret |= sc3338_write_reg(dev, 0x3771,0x09);
    ret |= sc3338_write_reg(dev, 0x3772,0x09);
    ret |= sc3338_write_reg(dev, 0x3773,0x05);
    ret |= sc3338_write_reg(dev, 0x377a,0x48);
    ret |= sc3338_write_reg(dev, 0x377b,0x5f);
    ret |= sc3338_write_reg(dev, 0x3904,0x04);
    ret |= sc3338_write_reg(dev, 0x3905,0x8c);
    ret |= sc3338_write_reg(dev, 0x391d,0x04);
    ret |= sc3338_write_reg(dev, 0x3921,0x20);
    ret |= sc3338_write_reg(dev, 0x3926,0x21);
    ret |= sc3338_write_reg(dev, 0x3933,0x80);
    ret |= sc3338_write_reg(dev, 0x3934,0x0a);
    ret |= sc3338_write_reg(dev, 0x3935,0x00);
    ret |= sc3338_write_reg(dev, 0x3936,0x2a);
    ret |= sc3338_write_reg(dev, 0x3937,0x6a);
    ret |= sc3338_write_reg(dev, 0x3938,0x6a);
    ret |= sc3338_write_reg(dev, 0x39dc,0x02);
    ret |= sc3338_write_reg(dev, 0x3e01,0x53);
    ret |= sc3338_write_reg(dev, 0x3e02,0xe0);
    ret |= sc3338_write_reg(dev, 0x3e09,0x00);
    ret |= sc3338_write_reg(dev, 0x440e,0x02);
    ret |= sc3338_write_reg(dev, 0x4509,0x20);
    ret |= sc3338_write_reg(dev, 0x5ae0,0xfe);
    ret |= sc3338_write_reg(dev, 0x5ae1,0x40);
    ret |= sc3338_write_reg(dev, 0x5ae2,0x38);
    ret |= sc3338_write_reg(dev, 0x5ae3,0x30);
    ret |= sc3338_write_reg(dev, 0x5ae4,0x28);
    ret |= sc3338_write_reg(dev, 0x5ae5,0x38);
    ret |= sc3338_write_reg(dev, 0x5ae6,0x30);
    ret |= sc3338_write_reg(dev, 0x5ae7,0x28);
    ret |= sc3338_write_reg(dev, 0x5ae8,0x3f);
    ret |= sc3338_write_reg(dev, 0x5ae9,0x34);
    ret |= sc3338_write_reg(dev, 0x5aea,0x2c);
    ret |= sc3338_write_reg(dev, 0x5aeb,0x3f);
    ret |= sc3338_write_reg(dev, 0x5aec,0x34);
    ret |= sc3338_write_reg(dev, 0x5aed,0x2c);
    ret |= sc3338_write_reg(dev, 0x36e9,0x54);
    ret |= sc3338_write_reg(dev, 0x37f9,0x27);
    ret |= sc3338_write_reg(dev, 0x0100,0x01);

    sc3338_delay_ms(10);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc3338_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== SC3338_MIPI_27MInput_10bit_2lane_linear_30fps_2304x1296_init success!========\n");
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC3338_3M_10BIT_LINEAR_MODE:
            ret = sc3338_2lane_linear_2304x1296_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc3338_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc3338_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "i2c_set_slave_addr error!\n");
        i2c_close(g_sc3338_i2c_fd[dev]);
        g_sc3338_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc3338_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32  ret;

    if (g_sc3338_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc3338_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc3338_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc3338_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc3338_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc3338_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC3338_DATA_BYTE];

    if (g_sc3338_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc3338_i2c_fd[dev], g_sc3338_i2c_addr[dev], addr, SC3338_ADDR_BYTE, buf, SC3338_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC3338_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc3338_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc3338_i2c_fd[dev], g_sc3338_i2c_addr[dev], buf, SC3338_ADDR_BYTE + SC3338_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc3338_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc3338_read_reg(dev, SC3338_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x06;
    } else {
        mirror_flip_type &= 0xf9;
    }

    if (flip_en) {
        mirror_flip_type |= 0x60;
    } else {
        mirror_flip_type &= 0x9f;
    }

    ret = sc3338_write_reg(dev, SC3338_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc3338_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
