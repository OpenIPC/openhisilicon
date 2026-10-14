#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc20b3.h"
#include "gc20b3_ctrl.h"

static xmedia_s32 g_gc20b3_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_gc20b3_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 gc20b3_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

//00_GC20B3_MIPI2L_24M_1920x1080_30fps_raw10_linear
//version 1.0
//mclk 24Mhz
//mipiclk 600Mbps/lane, wpllclk 200Mhz, rpllclk 150Mhz
//framelength 1125
//rowtime 29.629us
//pattern rggb
    ret |= gc20b3_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc20b3_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc20b3_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc20b3_write_reg(dev, 0x03fe, 0x00);
    ret |= gc20b3_write_reg(dev, 0x03f2, 0x00);
    ret |= gc20b3_write_reg(dev, 0x03f3, 0x00);
    ret |= gc20b3_write_reg(dev, 0x03f4, 0x36);
    ret |= gc20b3_write_reg(dev, 0x03f5, 0xc0);
    ret |= gc20b3_write_reg(dev, 0x03f6, 0x24);
    ret |= gc20b3_write_reg(dev, 0x03f7, 0x01);
    ret |= gc20b3_write_reg(dev, 0x03f8, 0x32);
    ret |= gc20b3_write_reg(dev, 0x03f9, 0x43);
    ret |= gc20b3_write_reg(dev, 0x03fc, 0x8e);
    ret |= gc20b3_write_reg(dev, 0x0381, 0x07);
    ret |= gc20b3_write_reg(dev, 0x00d7, 0x29);
    ret |= gc20b3_write_reg(dev, 0x0d6d, 0x18);
    ret |= gc20b3_write_reg(dev, 0x00d5, 0x03);
    ret |= gc20b3_write_reg(dev, 0x0082, 0x01);
    ret |= gc20b3_write_reg(dev, 0x0db3, 0xd4);
    ret |= gc20b3_write_reg(dev, 0x0db0, 0x0d);
    ret |= gc20b3_write_reg(dev, 0x0db5, 0x96);
    ret |= gc20b3_write_reg(dev, 0x0d03, 0x02);
    ret |= gc20b3_write_reg(dev, 0x0d04, 0x02);
    ret |= gc20b3_write_reg(dev, 0x0d05, 0x05);
    ret |= gc20b3_write_reg(dev, 0x0d06, 0xc9);
    ret |= gc20b3_write_reg(dev, 0x0d07, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0d08, 0x11);
    ret |= gc20b3_write_reg(dev, 0x0d09, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0d0a, 0x02);
    ret |= gc20b3_write_reg(dev, 0x000b, 0x00);
    ret |= gc20b3_write_reg(dev, 0x000c, 0x02);
    ret |= gc20b3_write_reg(dev, 0x0d0d, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0d0e, 0x40);
    ret |= gc20b3_write_reg(dev, 0x000f, 0x07);
    ret |= gc20b3_write_reg(dev, 0x0010, 0x90);
    ret |= gc20b3_write_reg(dev, 0x0017, 0x0c);
    ret |= gc20b3_write_reg(dev, 0x0d73, 0x92);
    ret |= gc20b3_write_reg(dev, 0x0076, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0d76, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0d41, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0d42, 0x65);
    ret |= gc20b3_write_reg(dev, 0x0d7a, 0x10);
    ret |= gc20b3_write_reg(dev, 0x0d19, 0x31);
    ret |= gc20b3_write_reg(dev, 0x0d25, 0x0b);
    ret |= gc20b3_write_reg(dev, 0x0d20, 0x70);
    ret |= gc20b3_write_reg(dev, 0x0d27, 0x03);
    ret |= gc20b3_write_reg(dev, 0x0d29, 0x90);
    ret |= gc20b3_write_reg(dev, 0x0d43, 0x25);
    ret |= gc20b3_write_reg(dev, 0x0d49, 0x10);
    ret |= gc20b3_write_reg(dev, 0x0d55, 0x18);
    ret |= gc20b3_write_reg(dev, 0x0dc2, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0058, 0x3c);
    ret |= gc20b3_write_reg(dev, 0x00d8, 0x68);
    ret |= gc20b3_write_reg(dev, 0x00d9, 0x14);
    ret |= gc20b3_write_reg(dev, 0x00da, 0xc1);
    ret |= gc20b3_write_reg(dev, 0x0050, 0x18);
    ret |= gc20b3_write_reg(dev, 0x0db6, 0x3d);
    ret |= gc20b3_write_reg(dev, 0x00d2, 0xbc);
    ret |= gc20b3_write_reg(dev, 0x0d66, 0x42);
    ret |= gc20b3_write_reg(dev, 0x008c, 0x07);
    ret |= gc20b3_write_reg(dev, 0x008d, 0xff);
    ret |= gc20b3_write_reg(dev, 0x007a, 0x50);
    ret |= gc20b3_write_reg(dev, 0x00d0, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0dc1, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0102, 0xa9);
    ret |= gc20b3_write_reg(dev, 0x0158, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0107, 0xa6);
    ret |= gc20b3_write_reg(dev, 0x0108, 0xa9);
    ret |= gc20b3_write_reg(dev, 0x0109, 0xa8);
    ret |= gc20b3_write_reg(dev, 0x010a, 0xa7);
    ret |= gc20b3_write_reg(dev, 0x010b, 0xff);
    ret |= gc20b3_write_reg(dev, 0x010c, 0xff);
    ret |= gc20b3_write_reg(dev, 0x0428, 0x86);
    ret |= gc20b3_write_reg(dev, 0x0429, 0x86);
    ret |= gc20b3_write_reg(dev, 0x042a, 0x86);
    ret |= gc20b3_write_reg(dev, 0x042b, 0x68);
    ret |= gc20b3_write_reg(dev, 0x042c, 0x68);
    ret |= gc20b3_write_reg(dev, 0x042d, 0x68);
    ret |= gc20b3_write_reg(dev, 0x042e, 0x68);
    ret |= gc20b3_write_reg(dev, 0x042f, 0x68);
    ret |= gc20b3_write_reg(dev, 0x0430, 0x4f);
    ret |= gc20b3_write_reg(dev, 0x0431, 0x68);
    ret |= gc20b3_write_reg(dev, 0x0432, 0x67);
    ret |= gc20b3_write_reg(dev, 0x0433, 0x66);
    ret |= gc20b3_write_reg(dev, 0x0434, 0x66);
    ret |= gc20b3_write_reg(dev, 0x0435, 0x66);
    ret |= gc20b3_write_reg(dev, 0x0436, 0x66);
    ret |= gc20b3_write_reg(dev, 0x0437, 0x66);
    ret |= gc20b3_write_reg(dev, 0x0438, 0x62);
    ret |= gc20b3_write_reg(dev, 0x0439, 0x62);
    ret |= gc20b3_write_reg(dev, 0x043a, 0x62);
    ret |= gc20b3_write_reg(dev, 0x043b, 0x62);
    ret |= gc20b3_write_reg(dev, 0x043c, 0x62);
    ret |= gc20b3_write_reg(dev, 0x043d, 0x62);
    ret |= gc20b3_write_reg(dev, 0x043e, 0x62);
    ret |= gc20b3_write_reg(dev, 0x043f, 0x62);
    ret |= gc20b3_write_reg(dev, 0x0077, 0x01);
    ret |= gc20b3_write_reg(dev, 0x0078, 0x65);
    ret |= gc20b3_write_reg(dev, 0x0079, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0067, 0xa0);
    ret |= gc20b3_write_reg(dev, 0x0054, 0xff);
    ret |= gc20b3_write_reg(dev, 0x0055, 0x02);
    ret |= gc20b3_write_reg(dev, 0x0056, 0x00);
    ret |= gc20b3_write_reg(dev, 0x0057, 0x04);
    ret |= gc20b3_write_reg(dev, 0x005a, 0xff);
    ret |= gc20b3_write_reg(dev, 0x005b, 0x07);
    ret |= gc20b3_write_reg(dev, 0x0026, 0x01);
    ret |= gc20b3_write_reg(dev, 0x0152, 0x02);
    ret |= gc20b3_write_reg(dev, 0x0153, 0x50);
    ret |= gc20b3_write_reg(dev, 0x0155, 0x05);
    ret |= gc20b3_write_reg(dev, 0x0410, 0x0b);
    ret |= gc20b3_write_reg(dev, 0x0411, 0x0a);
    ret |= gc20b3_write_reg(dev, 0x0412, 0x0a);
    ret |= gc20b3_write_reg(dev, 0x0413, 0x0b);
    ret |= gc20b3_write_reg(dev, 0x0414, 0x75);
    ret |= gc20b3_write_reg(dev, 0x0415, 0x76);
    ret |= gc20b3_write_reg(dev, 0x0416, 0x76);
    ret |= gc20b3_write_reg(dev, 0x0417, 0x75);
    ret |= gc20b3_write_reg(dev, 0x04e0, 0x18);
    ret |= gc20b3_write_reg(dev, 0x0192, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0194, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0195, 0x04);
    ret |= gc20b3_write_reg(dev, 0x0196, 0x38);
    ret |= gc20b3_write_reg(dev, 0x0197, 0x07);
    ret |= gc20b3_write_reg(dev, 0x0198, 0x80);
    ret |= gc20b3_write_reg(dev, 0x0201, 0x27);
    ret |= gc20b3_write_reg(dev, 0x0202, 0x53);
    ret |= gc20b3_write_reg(dev, 0x0203, 0xce);
    ret |= gc20b3_write_reg(dev, 0x0204, 0x40);
    ret |= gc20b3_write_reg(dev, 0x0212, 0x07);
    ret |= gc20b3_write_reg(dev, 0x0213, 0x80);
    ret |= gc20b3_write_reg(dev, 0x0215, 0x12);
    ret |= gc20b3_write_reg(dev, 0x0229, 0x05);
    ret |= gc20b3_write_reg(dev, 0x0237, 0x03);
    ret |= gc20b3_write_reg(dev, 0x023e, 0x99);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc20b3_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC20B3_24MInput_MIPI_2lane_10bit_linear_1920x1080_30fps init success!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc20b3_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC20B3_2M_10BIT_LINEAR_MODE:
            ret = gc20b3_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20b3_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc20b3_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc20b3_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc20b3_i2c_fd[dev]);
        g_gc20b3_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc20b3_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20b3_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc20b3_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc20b3_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc20b3_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc20b3_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20b3_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc20b3_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc20b3_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20b3_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC20B3_DATA_BYTE];

    if (g_gc20b3_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc20b3_i2c_fd[dev], g_gc20b3_i2c_addr[dev], addr, GC20B3_ADDR_BYTE, buf, GC20B3_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC20B3_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20b3_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc20b3_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc20b3_i2c_fd[dev], g_gc20b3_i2c_addr[dev], buf, GC20B3_ADDR_BYTE + GC20B3_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc20b3_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((!mirror_en) && (!flip_en)) {
        ret |= gc20b3_write_reg(dev, 0x0015,0x00);
        ret |= gc20b3_write_reg(dev, 0x0d15,0x00);

    } else if ((mirror_en) && (!flip_en)) {
        ret |= gc20b3_write_reg(dev, 0x0015,0x01);
        ret |= gc20b3_write_reg(dev, 0x0d15,0x01);
    } else if ((!mirror_en) && (flip_en)) {
        ret |= gc20b3_write_reg(dev, 0x0015,0x02);
        ret |= gc20b3_write_reg(dev, 0x0d15,0x02);
    } else if ((mirror_en) && (flip_en)) {
        ret |= gc20b3_write_reg(dev, 0x0015,0x03);
        ret |= gc20b3_write_reg(dev, 0x0d15,0x03);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc20b3_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

