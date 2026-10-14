#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "bf3a03.h"
#include "bf3a03_ctrl.h"

static xmedia_s32 g_bf3a03_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_bf3a03_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define BF3A03_REG_ADDR_MIRROR_FLIP 0x12

xmedia_void bf3a03_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 bf3a03_dvp_linear_640x480_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    bf3a03_write_reg(dev, 0x12, 0x80);
    ret |= bf3a03_write_reg(dev, 0x09, 0x55);
    ret |= bf3a03_write_reg(dev, 0x15, 0x02);
    ret |= bf3a03_write_reg(dev, 0x1e, 0x40);
    ret |= bf3a03_write_reg(dev, 0x06, 0x78);
    ret |= bf3a03_write_reg(dev, 0x21, 0x00);
    ret |= bf3a03_write_reg(dev, 0x3e, 0x37);
    ret |= bf3a03_write_reg(dev, 0x29, 0x2b);
    ret |= bf3a03_write_reg(dev, 0x27, 0x98);
    ret |= bf3a03_write_reg(dev, 0x16, 0x25);
    ret |= bf3a03_write_reg(dev, 0x20, 0x00);
    ret |= bf3a03_write_reg(dev, 0xd2, 0x18);
    ret |= bf3a03_write_reg(dev, 0x11, 0x10);
    ret |= bf3a03_write_reg(dev, 0x2f, 0x4e);
    ret |= bf3a03_write_reg(dev, 0x1b, 0x09);
    ret |= bf3a03_write_reg(dev, 0x3a, 0x00);
    ret |= bf3a03_write_reg(dev, 0x4a, 0x98);
    ret |= bf3a03_write_reg(dev, 0x12, 0x00);
    ret |= bf3a03_write_reg(dev, 0x13, 0x08);
    ret |= bf3a03_write_reg(dev, 0x01, 0x14);
    ret |= bf3a03_write_reg(dev, 0x02, 0x20);
    ret |= bf3a03_write_reg(dev, 0x8c, 0x02);
    ret |= bf3a03_write_reg(dev, 0x8d, 0x4c);
    ret |= bf3a03_write_reg(dev, 0x87, 0x16);
    ret |= bf3a03_write_reg(dev, 0x13, 0x07);
    ret |= bf3a03_write_reg(dev, 0x70, 0x0f);
    ret |= bf3a03_write_reg(dev, 0x3b, 0x00);
    ret |= bf3a03_write_reg(dev, 0x71, 0x0c);
    ret |= bf3a03_write_reg(dev, 0x73, 0x27);
    ret |= bf3a03_write_reg(dev, 0x75, 0x88);
    ret |= bf3a03_write_reg(dev, 0x76, 0xd8);
    ret |= bf3a03_write_reg(dev, 0x77, 0x0a);
    ret |= bf3a03_write_reg(dev, 0x78, 0xff);
    ret |= bf3a03_write_reg(dev, 0x79, 0x14);
    ret |= bf3a03_write_reg(dev, 0x7a, 0x23);
    ret |= bf3a03_write_reg(dev, 0x9e, 0x04);
    ret |= bf3a03_write_reg(dev, 0x7d, 0x2a);
    ret |= bf3a03_write_reg(dev, 0x39, 0xa0);
    ret |= bf3a03_write_reg(dev, 0x3f, 0xa0);
    ret |= bf3a03_write_reg(dev, 0x40, 0x19);
    ret |= bf3a03_write_reg(dev, 0x41, 0x1e);
    ret |= bf3a03_write_reg(dev, 0x42, 0x1f);
    ret |= bf3a03_write_reg(dev, 0x43, 0x20);
    ret |= bf3a03_write_reg(dev, 0x44, 0x1d);
    ret |= bf3a03_write_reg(dev, 0x45, 0x19);
    ret |= bf3a03_write_reg(dev, 0x46, 0x17);
    ret |= bf3a03_write_reg(dev, 0x47, 0x17);
    ret |= bf3a03_write_reg(dev, 0x48, 0x14);
    ret |= bf3a03_write_reg(dev, 0x49, 0x12);
    ret |= bf3a03_write_reg(dev, 0x4b, 0x0f);
    ret |= bf3a03_write_reg(dev, 0x4c, 0x0c);
    ret |= bf3a03_write_reg(dev, 0x4e, 0x08);
    ret |= bf3a03_write_reg(dev, 0x4f, 0x06);
    ret |= bf3a03_write_reg(dev, 0x50, 0x03);
    ret |= bf3a03_write_reg(dev, 0x24, 0x50);
    ret |= bf3a03_write_reg(dev, 0x97, 0x40);
    ret |= bf3a03_write_reg(dev, 0x25, 0x88);
    ret |= bf3a03_write_reg(dev, 0x81, 0x00);
    ret |= bf3a03_write_reg(dev, 0x82, 0x18);
    ret |= bf3a03_write_reg(dev, 0x83, 0x30);
    ret |= bf3a03_write_reg(dev, 0x84, 0x20);
    ret |= bf3a03_write_reg(dev, 0x85, 0x38);
    ret |= bf3a03_write_reg(dev, 0x86, 0xa0);
    ret |= bf3a03_write_reg(dev, 0x94, 0x82);
    ret |= bf3a03_write_reg(dev, 0x80, 0x92);
    ret |= bf3a03_write_reg(dev, 0x98, 0x8a);
    ret |= bf3a03_write_reg(dev, 0x89, 0x7d);
    ret |= bf3a03_write_reg(dev, 0x8e, 0x2c);
    ret |= bf3a03_write_reg(dev, 0x8f, 0x82);
    ret |= bf3a03_write_reg(dev, 0x2b, 0x20);
    ret |= bf3a03_write_reg(dev, 0x8a, 0x49);
    ret |= bf3a03_write_reg(dev, 0x8b, 0x3d);
    ret |= bf3a03_write_reg(dev, 0x92, 0x6D);
    ret |= bf3a03_write_reg(dev, 0x5a, 0xec);
    ret |= bf3a03_write_reg(dev, 0x51, 0x90);
    ret |= bf3a03_write_reg(dev, 0x52, 0x10);
    ret |= bf3a03_write_reg(dev, 0x53, 0x8d);
    ret |= bf3a03_write_reg(dev, 0x54, 0x88);
    ret |= bf3a03_write_reg(dev, 0x57, 0x82);
    ret |= bf3a03_write_reg(dev, 0x58, 0x8d);
    ret |= bf3a03_write_reg(dev, 0x5a, 0x7c);
    ret |= bf3a03_write_reg(dev, 0x51, 0x80);
    ret |= bf3a03_write_reg(dev, 0x52, 0x04);
    ret |= bf3a03_write_reg(dev, 0x53, 0x8d);
    ret |= bf3a03_write_reg(dev, 0x54, 0x88);
    ret |= bf3a03_write_reg(dev, 0x57, 0x82);
    ret |= bf3a03_write_reg(dev, 0x58, 0x8d);
    ret |= bf3a03_write_reg(dev, 0x5a, 0x6c);
    ret |= bf3a03_write_reg(dev, 0x51, 0x8a);
    ret |= bf3a03_write_reg(dev, 0x52, 0x04);
    ret |= bf3a03_write_reg(dev, 0x53, 0x8a);
    ret |= bf3a03_write_reg(dev, 0x54, 0x85);
    ret |= bf3a03_write_reg(dev, 0x57, 0x04);
    ret |= bf3a03_write_reg(dev, 0x58, 0x8a);
    ret |= bf3a03_write_reg(dev, 0xb0, 0xa0);
    ret |= bf3a03_write_reg(dev, 0xb1, 0x26);
    ret |= bf3a03_write_reg(dev, 0xb2, 0x1c);
    ret |= bf3a03_write_reg(dev, 0xb4, 0xfd);
    ret |= bf3a03_write_reg(dev, 0xb0, 0x30);
    ret |= bf3a03_write_reg(dev, 0xb1, 0xd8);
    ret |= bf3a03_write_reg(dev, 0xb2, 0xb0);
    ret |= bf3a03_write_reg(dev, 0xb4, 0xf1);
    ret |= bf3a03_write_reg(dev, 0x3c, 0x40);
    ret |= bf3a03_write_reg(dev, 0x56, 0x48);
    ret |= bf3a03_write_reg(dev, 0x4d, 0x40);
    ret |= bf3a03_write_reg(dev, 0x59, 0x40);
    ret |= bf3a03_write_reg(dev, 0x35, 0x46);
    ret |= bf3a03_write_reg(dev, 0x65, 0x38);
    ret |= bf3a03_write_reg(dev, 0x66, 0x42);
    ret |= bf3a03_write_reg(dev, 0x6a, 0xd1);
    ret |= bf3a03_write_reg(dev, 0x23, 0x11);
    ret |= bf3a03_write_reg(dev, 0xa2, 0x0b);
    ret |= bf3a03_write_reg(dev, 0xa3, 0x26);
    ret |= bf3a03_write_reg(dev, 0xa4, 0x04);
    ret |= bf3a03_write_reg(dev, 0xa5, 0x26);
    ret |= bf3a03_write_reg(dev, 0xa7, 0x15);
    ret |= bf3a03_write_reg(dev, 0xa8, 0x0e);
    ret |= bf3a03_write_reg(dev, 0xa9, 0x20);
    ret |= bf3a03_write_reg(dev, 0xaa, 0x20);
    ret |= bf3a03_write_reg(dev, 0xab, 0x16);
    ret |= bf3a03_write_reg(dev, 0xac, 0x30);
    ret |= bf3a03_write_reg(dev, 0xad, 0xf0);
    ret |= bf3a03_write_reg(dev, 0xae, 0x57);
    ret |= bf3a03_write_reg(dev, 0xc5, 0x66);
    ret |= bf3a03_write_reg(dev, 0xc7, 0x38);
    ret |= bf3a03_write_reg(dev, 0xc8, 0x0d);
    ret |= bf3a03_write_reg(dev, 0xc9, 0x16);
    ret |= bf3a03_write_reg(dev, 0xd3, 0x09);
    ret |= bf3a03_write_reg(dev, 0xd4, 0x15);
    ret |= bf3a03_write_reg(dev, 0xd0, 0x00);
    ret |= bf3a03_write_reg(dev, 0xd1, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "bf3a03_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=============================================================================================\n");
        SENSOR_PRINT("============ bf3a03_normal_dvp_24Minput_yuv422_linear_640x480_30fps init success! ===========\n");
        SENSOR_PRINT("=============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 bf3a03_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (image_mode) {
        case BF3A03_DVP_640X480_LINEAR_MODE:
            ret = bf3a03_dvp_linear_640x480_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 bf3a03_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_bf3a03_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_bf3a03_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_bf3a03_i2c_fd[dev]);
        g_bf3a03_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_bf3a03_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 bf3a03_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_bf3a03_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_bf3a03_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_bf3a03_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = bf3a03_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 bf3a03_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_bf3a03_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_bf3a03_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 bf3a03_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[BF3A03_DATA_BYTE];

    if (g_bf3a03_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_bf3a03_i2c_fd[dev], g_bf3a03_i2c_addr[dev], addr, BF3A03_ADDR_BYTE, buf, BF3A03_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //BF3A03_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 bf3a03_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_bf3a03_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_bf3a03_i2c_fd[dev], g_bf3a03_i2c_addr[dev], buf, BF3A03_ADDR_BYTE + BF3A03_DATA_BYTE);
    if (ret != (BF3A03_ADDR_BYTE + BF3A03_DATA_BYTE)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!ret=%d.\n", ret);
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 bf3a03_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    bf3a03_read_reg(dev, BF3A03_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x20;
    } else {
        mirror_flip_type &= 0xdf;
    }

    if (flip_en) {
        mirror_flip_type |= 0x10;
    } else {
        mirror_flip_type &= 0xef;
    }

    ret = bf3a03_write_reg(dev, BF3A03_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "bf3a03_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
