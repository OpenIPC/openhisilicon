#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc5603.h"
#include "gc5603_ctrl.h"

#define GC5603_REG_ADDR_MIRROR_FLIP 0x0101

static xmedia_s32 g_gc5603_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
static xmedia_s32 g_gc5603_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

static void gc5603_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

static xmedia_s32 gc5603_2lane_linear_2960x1620_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc5603_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc5603_write_reg(dev, 0x03fe, 0x00);
    ret |= gc5603_write_reg(dev, 0x03fe, 0x10);
    ret |= gc5603_write_reg(dev, 0x03fe, 0x00);
    ret |= gc5603_write_reg(dev, 0x0a38, 0x02);
    ret |= gc5603_write_reg(dev, 0x0a38, 0x03);
    ret |= gc5603_write_reg(dev, 0x0a20, 0x07);
    ret |= gc5603_write_reg(dev, 0x061b, 0x03);
    ret |= gc5603_write_reg(dev, 0x061c, 0x50);
    ret |= gc5603_write_reg(dev, 0x061d, 0x05);
    ret |= gc5603_write_reg(dev, 0x061e, 0x70);
    ret |= gc5603_write_reg(dev, 0x061f, 0x03);
    ret |= gc5603_write_reg(dev, 0x0a21, 0x08);
    ret |= gc5603_write_reg(dev, 0x0a34, 0x40);
    ret |= gc5603_write_reg(dev, 0x0a35, 0x11);
    ret |= gc5603_write_reg(dev, 0x0a36, 0x5e);
    ret |= gc5603_write_reg(dev, 0x0a37, 0x03);
    ret |= gc5603_write_reg(dev, 0x0314, 0x50);
    ret |= gc5603_write_reg(dev, 0x0315, 0x32);
    ret |= gc5603_write_reg(dev, 0x031c, 0xce);
    ret |= gc5603_write_reg(dev, 0x0219, 0x47);
    ret |= gc5603_write_reg(dev, 0x0342, 0x04);
    ret |= gc5603_write_reg(dev, 0x0343, 0xb0);
    ret |= gc5603_write_reg(dev, 0x0259, 0x06);
    ret |= gc5603_write_reg(dev, 0x025a, 0x8a);
    ret |= gc5603_write_reg(dev, 0x0340, 0x06);
    ret |= gc5603_write_reg(dev, 0x0341, 0xd6);
    ret |= gc5603_write_reg(dev, 0x0345, 0x02);
    ret |= gc5603_write_reg(dev, 0x0347, 0x02);
    ret |= gc5603_write_reg(dev, 0x0348, 0x0b);
    ret |= gc5603_write_reg(dev, 0x0349, 0x98);
    ret |= gc5603_write_reg(dev, 0x034a, 0x06);
    ret |= gc5603_write_reg(dev, 0x034b, 0x8a);
    ret |= gc5603_write_reg(dev, 0x0094, 0x0b);
    ret |= gc5603_write_reg(dev, 0x0095, 0x90);
    ret |= gc5603_write_reg(dev, 0x0096, 0x06);
    ret |= gc5603_write_reg(dev, 0x0097, 0x54);
    ret |= gc5603_write_reg(dev, 0x0099, 0x04);
    ret |= gc5603_write_reg(dev, 0x009b, 0x04);
    ret |= gc5603_write_reg(dev, 0x060c, 0x01);
    ret |= gc5603_write_reg(dev, 0x060e, 0xd2);
    ret |= gc5603_write_reg(dev, 0x060f, 0x05);
    ret |= gc5603_write_reg(dev, 0x070c, 0x01);
    ret |= gc5603_write_reg(dev, 0x070e, 0xd2);
    ret |= gc5603_write_reg(dev, 0x070f, 0x05);
    ret |= gc5603_write_reg(dev, 0x0709, 0x40);
    ret |= gc5603_write_reg(dev, 0x0719, 0x40);
    ret |= gc5603_write_reg(dev, 0x0909, 0x07);
    ret |= gc5603_write_reg(dev, 0x0902, 0x04);
    ret |= gc5603_write_reg(dev, 0x0904, 0x0b);
    ret |= gc5603_write_reg(dev, 0x0907, 0x54);
    ret |= gc5603_write_reg(dev, 0x0908, 0x06);
    ret |= gc5603_write_reg(dev, 0x0903, 0x9d);
    ret |= gc5603_write_reg(dev, 0x072a, 0x1c);
    ret |= gc5603_write_reg(dev, 0x072b, 0x1c);
    ret |= gc5603_write_reg(dev, 0x0724, 0x2b);
    ret |= gc5603_write_reg(dev, 0x0727, 0x2b);
    ret |= gc5603_write_reg(dev, 0x1466, 0x18);
    ret |= gc5603_write_reg(dev, 0x1467, 0x15);
    ret |= gc5603_write_reg(dev, 0x1468, 0x15);
    ret |= gc5603_write_reg(dev, 0x1469, 0x70);
    ret |= gc5603_write_reg(dev, 0x146a, 0xe8);
    ret |= gc5603_write_reg(dev, 0x0707, 0x07);
    ret |= gc5603_write_reg(dev, 0x0737, 0x0f);
    ret |= gc5603_write_reg(dev, 0x0704, 0x01);
    ret |= gc5603_write_reg(dev, 0x0706, 0x02);
    ret |= gc5603_write_reg(dev, 0x0716, 0x02);
    ret |= gc5603_write_reg(dev, 0x0708, 0xc8);
    ret |= gc5603_write_reg(dev, 0x0718, 0xc8);
    ret |= gc5603_write_reg(dev, 0x061a, 0x02);
    ret |= gc5603_write_reg(dev, 0x1430, 0x80);
    ret |= gc5603_write_reg(dev, 0x1407, 0x10);
    ret |= gc5603_write_reg(dev, 0x1408, 0x16);
    ret |= gc5603_write_reg(dev, 0x1409, 0x03);
    ret |= gc5603_write_reg(dev, 0x1438, 0x01);
    ret |= gc5603_write_reg(dev, 0x02ce, 0x03);
    ret |= gc5603_write_reg(dev, 0x0245, 0xc9);
    ret |= gc5603_write_reg(dev, 0x023a, 0x08);
    ret |= gc5603_write_reg(dev, 0x02cd, 0x88);
    ret |= gc5603_write_reg(dev, 0x0612, 0x02);
    ret |= gc5603_write_reg(dev, 0x0613, 0xc7);
    ret |= gc5603_write_reg(dev, 0x0243, 0x03);
    ret |= gc5603_write_reg(dev, 0x0089, 0x03);
    ret |= gc5603_write_reg(dev, 0x0002, 0xab);
    ret |= gc5603_write_reg(dev, 0x0040, 0xa3);
    ret |= gc5603_write_reg(dev, 0x0075, 0x64);
    ret |= gc5603_write_reg(dev, 0x0004, 0x0f);
    ret |= gc5603_write_reg(dev, 0x0053, 0x0a);
    ret |= gc5603_write_reg(dev, 0x0205, 0x0c);
    ret |= gc5603_write_reg(dev, 0x0052, 0x02);
    ret |= gc5603_write_reg(dev, 0x0076, 0x01);
    ret |= gc5603_write_reg(dev, 0x021a, 0x10);
    ret |= gc5603_write_reg(dev, 0x0049, 0x0f);
    ret |= gc5603_write_reg(dev, 0x004a, 0x3c);
    ret |= gc5603_write_reg(dev, 0x004b, 0x00);
    ret |= gc5603_write_reg(dev, 0x0430, 0x25);
    ret |= gc5603_write_reg(dev, 0x0431, 0x25);
    ret |= gc5603_write_reg(dev, 0x0432, 0x25);
    ret |= gc5603_write_reg(dev, 0x0433, 0x25);
    ret |= gc5603_write_reg(dev, 0x0434, 0x59);
    ret |= gc5603_write_reg(dev, 0x0435, 0x59);
    ret |= gc5603_write_reg(dev, 0x0436, 0x59);
    ret |= gc5603_write_reg(dev, 0x0437, 0x59);
    ret |= gc5603_write_reg(dev, 0x0181, 0x30);
    ret |= gc5603_write_reg(dev, 0x0182, 0x05);
    ret |= gc5603_write_reg(dev, 0x0185, 0x01);
    ret |= gc5603_write_reg(dev, 0x0180, 0x46);
    ret |= gc5603_write_reg(dev, 0x0100, 0x08);
    ret |= gc5603_write_reg(dev, 0x010d, 0x74);
    ret |= gc5603_write_reg(dev, 0x010e, 0x0e);
    ret |= gc5603_write_reg(dev, 0x0113, 0x02);
    ret |= gc5603_write_reg(dev, 0x0114, 0x01);
    ret |= gc5603_write_reg(dev, 0x0115, 0x10);
    ret |= gc5603_write_reg(dev, 0x031f, 0x01);
    ret |= gc5603_write_reg(dev, 0x031f, 0x00);
    ret |= gc5603_write_reg(dev, 0x0317, 0x1c);
    ret |= gc5603_write_reg(dev, 0x0a67, 0x80);
    ret |= gc5603_write_reg(dev, 0x0a50, 0x80);
    ret |= gc5603_write_reg(dev, 0x0a53, 0x00);
    ret |= gc5603_write_reg(dev, 0x0a51, 0x80);
    ret |= gc5603_write_reg(dev, 0x0a54, 0xa0);
    ret |= gc5603_write_reg(dev, 0x0a63, 0x20);
    ret |= gc5603_write_reg(dev, 0x0a65, 0x20);
    ret |= gc5603_write_reg(dev, 0x0a68, 0x21);
    ret |= gc5603_write_reg(dev, 0x05be, 0x00);
    ret |= gc5603_write_reg(dev, 0x05a9, 0x01);
    ret |= gc5603_write_reg(dev, 0x0024, 0x00);
    ret |= gc5603_write_reg(dev, 0x0025, 0x00);
    ret |= gc5603_write_reg(dev, 0x0028, 0x0b);
    ret |= gc5603_write_reg(dev, 0x0029, 0x98);
    ret |= gc5603_write_reg(dev, 0x002a, 0x06);
    ret |= gc5603_write_reg(dev, 0x002b, 0x8a);
    ret |= gc5603_write_reg(dev, 0x0a82, 0x00);
    ret |= gc5603_write_reg(dev, 0x0a83, 0xe0);
    ret |= gc5603_write_reg(dev, 0x0a72, 0x02);
    ret |= gc5603_write_reg(dev, 0x0a73, 0x60);
    ret |= gc5603_write_reg(dev, 0x0a80, 0x01);
    ret |= gc5603_write_reg(dev, 0x0a70, 0x03);
    ret |= gc5603_write_reg(dev, 0x0a75, 0x41);
    ret |= gc5603_write_reg(dev, 0x0a5a, 0x80);
    gc5603_delay_ms(20);
    ret |= gc5603_write_reg(dev, 0x05be, 0x01);
    ret |= gc5603_write_reg(dev, 0x0a70, 0x00);
    ret |= gc5603_write_reg(dev, 0x0080, 0x02);
    ret |= gc5603_write_reg(dev, 0x0021, 0x40);
    ret |= gc5603_write_reg(dev, 0x0a67, 0x00);
    ret |= gc5603_write_reg(dev, 0x0100, 0x09);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc5603_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC5603_27MInput_MIPI_2lane_10bit_linear_2960x1620_30fps init success!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc5603_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC5603_5M_10BIT_LINEAR_MODE:
            ret = gc5603_2lane_linear_2960x1620_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc5603_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc5603_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc5603_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc5603_i2c_fd[dev]);
        g_gc5603_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc5603_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc5603_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc5603_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc5603_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc5603_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc5603_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc5603_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc5603_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc5603_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc5603_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC5603_DATA_BYTE];

    if (g_gc5603_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc5603_i2c_fd[dev], g_gc5603_i2c_addr[dev], addr, GC5603_ADDR_BYTE, buf, GC5603_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC5603_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc5603_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc5603_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc5603_i2c_fd[dev], g_gc5603_i2c_addr[dev], buf, GC5603_ADDR_BYTE + GC5603_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc5603_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((!mirror_en) && (!flip_en)) {
        ret |= gc5603_write_reg(dev, 0x022c, 0x00);
        ret |= gc5603_write_reg(dev, 0x0063, 0x00);
        ret |= gc5603_write_reg(dev, 0x031f, 0x01);
        ret |= gc5603_write_reg(dev, 0x031f, 0x00);
        ret |= gc5603_write_reg(dev, 0x0317, 0x1c);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a50, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a53, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a51, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a54, 0xa0);
        ret |= gc5603_write_reg(dev, 0x0a63, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a65, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a68, 0x21);
        ret |= gc5603_write_reg(dev, 0x05be, 0x00);
        ret |= gc5603_write_reg(dev, 0x05a9, 0x01);
        ret |= gc5603_write_reg(dev, 0x0024, 0x00);
        ret |= gc5603_write_reg(dev, 0x0025, 0x00);
        ret |= gc5603_write_reg(dev, 0x0028, 0x0b);
        ret |= gc5603_write_reg(dev, 0x0029, 0x98);
        ret |= gc5603_write_reg(dev, 0x002a, 0x06);
        ret |= gc5603_write_reg(dev, 0x002b, 0x8a);
        ret |= gc5603_write_reg(dev, 0x0a82, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a83, 0xe0);
        ret |= gc5603_write_reg(dev, 0x0a72, 0x02);
        ret |= gc5603_write_reg(dev, 0x0a73, 0x60);
        ret |= gc5603_write_reg(dev, 0x0a80, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x03);
        ret |= gc5603_write_reg(dev, 0x0a75, 0x41);
        ret |= gc5603_write_reg(dev, 0x0a5a, 0x80);
        gc5603_delay_ms(20);
        ret |= gc5603_write_reg(dev, 0x05be, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x00);
        ret |= gc5603_write_reg(dev, 0x0080, 0x02);
        ret |= gc5603_write_reg(dev, 0x0021, 0x40);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x00);
    } else if ((mirror_en) && (!flip_en)) {
        ret |= gc5603_write_reg(dev, 0x022c, 0x01);
        ret |= gc5603_write_reg(dev, 0x0063, 0x01);
        ret |= gc5603_write_reg(dev, 0x031f, 0x01);
        ret |= gc5603_write_reg(dev, 0x031f, 0x00);
        ret |= gc5603_write_reg(dev, 0x0317, 0x1c);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a50, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a53, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a51, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a54, 0xa0);
        ret |= gc5603_write_reg(dev, 0x0a63, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a65, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a68, 0x21);
        ret |= gc5603_write_reg(dev, 0x05be, 0x00);
        ret |= gc5603_write_reg(dev, 0x05a9, 0x01);
        ret |= gc5603_write_reg(dev, 0x0024, 0x00);
        ret |= gc5603_write_reg(dev, 0x0025, 0x00);
        ret |= gc5603_write_reg(dev, 0x0028, 0x0b);
        ret |= gc5603_write_reg(dev, 0x0029, 0x98);
        ret |= gc5603_write_reg(dev, 0x002a, 0x06);
        ret |= gc5603_write_reg(dev, 0x002b, 0x8a);
        ret |= gc5603_write_reg(dev, 0x0a82, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a83, 0xe0);
        ret |= gc5603_write_reg(dev, 0x0a72, 0x02);
        ret |= gc5603_write_reg(dev, 0x0a73, 0x61);
        ret |= gc5603_write_reg(dev, 0x0a80, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x03);
        ret |= gc5603_write_reg(dev, 0x0a75, 0x41);
        ret |= gc5603_write_reg(dev, 0x0a5a, 0x80);
        gc5603_delay_ms(20);
        ret |= gc5603_write_reg(dev, 0x05be, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x00);
        ret |= gc5603_write_reg(dev, 0x0080, 0x02);
        ret |= gc5603_write_reg(dev, 0x0021, 0x40);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x00);
    } else if ((!mirror_en) && (flip_en)) {
        ret |= gc5603_write_reg(dev, 0x022c, 0x02);
        ret |= gc5603_write_reg(dev, 0x0063, 0x02);
        ret |= gc5603_write_reg(dev, 0x031f, 0x01);
        ret |= gc5603_write_reg(dev, 0x031f, 0x00);
        ret |= gc5603_write_reg(dev, 0x0317, 0x1c);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a50, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a53, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a51, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a54, 0xa0);
        ret |= gc5603_write_reg(dev, 0x0a63, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a65, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a68, 0x21);
        ret |= gc5603_write_reg(dev, 0x05be, 0x00);
        ret |= gc5603_write_reg(dev, 0x05a9, 0x01);
        ret |= gc5603_write_reg(dev, 0x0024, 0x00);
        ret |= gc5603_write_reg(dev, 0x0025, 0x00);
        ret |= gc5603_write_reg(dev, 0x0028, 0x0b);
        ret |= gc5603_write_reg(dev, 0x0029, 0x98);
        ret |= gc5603_write_reg(dev, 0x002a, 0x06);
        ret |= gc5603_write_reg(dev, 0x002b, 0x8a);
        ret |= gc5603_write_reg(dev, 0x0a82, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a83, 0xe0);
        ret |= gc5603_write_reg(dev, 0x0a72, 0x02);
        ret |= gc5603_write_reg(dev, 0x0a73, 0x62);
        ret |= gc5603_write_reg(dev, 0x0a80, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x03);
        ret |= gc5603_write_reg(dev, 0x0a75, 0x41);
        ret |= gc5603_write_reg(dev, 0x0a5a, 0x80);
        gc5603_delay_ms(20);
        ret |= gc5603_write_reg(dev, 0x05be, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x00);
        ret |= gc5603_write_reg(dev, 0x0080, 0x02);
        ret |= gc5603_write_reg(dev, 0x0021, 0x40);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x00);
    } else if ((mirror_en) && (flip_en)) {
        ret |= gc5603_write_reg(dev, 0x022c, 0x03);
        ret |= gc5603_write_reg(dev, 0x0063, 0x03);
        ret |= gc5603_write_reg(dev, 0x031f, 0x01);
        ret |= gc5603_write_reg(dev, 0x031f, 0x00);
        ret |= gc5603_write_reg(dev, 0x0317, 0x1c);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a50, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a53, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a51, 0x80);
        ret |= gc5603_write_reg(dev, 0x0a54, 0xa0);
        ret |= gc5603_write_reg(dev, 0x0a63, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a65, 0x20);
        ret |= gc5603_write_reg(dev, 0x0a68, 0x21);
        ret |= gc5603_write_reg(dev, 0x05be, 0x00);
        ret |= gc5603_write_reg(dev, 0x05a9, 0x01);
        ret |= gc5603_write_reg(dev, 0x0024, 0x00);
        ret |= gc5603_write_reg(dev, 0x0025, 0x00);
        ret |= gc5603_write_reg(dev, 0x0028, 0x0b);
        ret |= gc5603_write_reg(dev, 0x0029, 0x98);
        ret |= gc5603_write_reg(dev, 0x002a, 0x06);
        ret |= gc5603_write_reg(dev, 0x002b, 0x8a);
        ret |= gc5603_write_reg(dev, 0x0a82, 0x00);
        ret |= gc5603_write_reg(dev, 0x0a83, 0xe0);
        ret |= gc5603_write_reg(dev, 0x0a72, 0x02);
        ret |= gc5603_write_reg(dev, 0x0a73, 0x63);
        ret |= gc5603_write_reg(dev, 0x0a80, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x03);
        ret |= gc5603_write_reg(dev, 0x0a75, 0x41);
        ret |= gc5603_write_reg(dev, 0x0a5a, 0x80);
        gc5603_delay_ms(20);
        ret |= gc5603_write_reg(dev, 0x05be, 0x01);
        ret |= gc5603_write_reg(dev, 0x0a70, 0x00);
        ret |= gc5603_write_reg(dev, 0x0080, 0x02);
        ret |= gc5603_write_reg(dev, 0x0021, 0x40);
        ret |= gc5603_write_reg(dev, 0x0a67, 0x00);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc5603_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

