#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc6603.h"
#include "gc6603_ctrl.h"


#define GC6603_REG_ADDR_MIRROR_FLIP 0x022c
#define GC6603_REG_ADDR_FLIP_FLIP   0x0a73
#define GC6603_REG_ADDR_FLIP        0x0722
#define GC6603_REG_ADDR_MIRROR      0x0063

static xmedia_s32 g_gc6603_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_gc6603_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

xmedia_void gc6603_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

static xmedia_s32 gc6603_2lane_linear_2688x2048_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    //version:0b370d
    //<MODE_1 type="00_GC6603_MIPI2L_24M_2688x2048_25fps_raw10_linear">
    //<RESOLUTION width="2688" height="2048"/>
    //mclk  24 Mhz
    //mipi 1032 Mbps/lane
    //vts = 2150
    //window 2688¡Á2048
    //row time=15.504us
    //bayer order  rggb
    ret |= gc6603_write_reg(dev, 0x03fe,0xf0);
    ret |= gc6603_write_reg(dev, 0x03fe,0x00);
    ret |= gc6603_write_reg(dev, 0x03fe,0x10);
    ret |= gc6603_write_reg(dev, 0x0100,0x00);
    ret |= gc6603_write_reg(dev, 0x0938,0x01);
    ret |= gc6603_write_reg(dev, 0x0360,0xfd);
    ret |= gc6603_write_reg(dev, 0x091b,0x1a);
    ret |= gc6603_write_reg(dev, 0x091c,0x28);
    ret |= gc6603_write_reg(dev, 0x091e,0x00);
    ret |= gc6603_write_reg(dev, 0x091d,0x06);
    ret |= gc6603_write_reg(dev, 0x091f,0x81);
    ret |= gc6603_write_reg(dev, 0x0920,0xa1);
    ret |= gc6603_write_reg(dev, 0x0922,0x3a);
    ret |= gc6603_write_reg(dev, 0x0923,0x10);
    ret |= gc6603_write_reg(dev, 0x0928,0x00);
    ret |= gc6603_write_reg(dev, 0x0934,0xb7);
    ret |= gc6603_write_reg(dev, 0x0935,0x06);
    ret |= gc6603_write_reg(dev, 0x0936,0x00);
    ret |= gc6603_write_reg(dev, 0x0937,0x81);
    ret |= gc6603_write_reg(dev, 0x031b,0x00);
    ret |= gc6603_write_reg(dev, 0x031c,0x4f);
    ret |= gc6603_write_reg(dev, 0x031e,0x00);
    ret |= gc6603_write_reg(dev, 0x03e0,0x00);
    ret |= gc6603_write_reg(dev, 0x0314,0x10);
    ret |= gc6603_write_reg(dev, 0x0219,0x47);
    ret |= gc6603_write_reg(dev, 0x022b,0x10);
    ret |= gc6603_write_reg(dev, 0x0259,0x08);
    ret |= gc6603_write_reg(dev, 0x025a,0x44);
    ret |= gc6603_write_reg(dev, 0x025b,0x10);
    ret |= gc6603_write_reg(dev, 0x0340,0x08);
    ret |= gc6603_write_reg(dev, 0x0341,0x66);
    ret |= gc6603_write_reg(dev, 0x0342,0x02);
    ret |= gc6603_write_reg(dev, 0x0343,0x58);     //0x258=600 30fps 0x2d0=720 25fps
    ret |= gc6603_write_reg(dev, 0x0346,0x00);
    ret |= gc6603_write_reg(dev, 0x0347,0x40);
    ret |= gc6603_write_reg(dev, 0x0348,0x0a);
    ret |= gc6603_write_reg(dev, 0x0349,0x90);
    ret |= gc6603_write_reg(dev, 0x034a,0x08);
    ret |= gc6603_write_reg(dev, 0x034b,0x20);
    ret |= gc6603_write_reg(dev, 0x034e,0x0a);
    ret |= gc6603_write_reg(dev, 0x034f,0xc0);
    ret |= gc6603_write_reg(dev, 0x070c,0x03);
    ret |= gc6603_write_reg(dev, 0x070d,0x00);
    ret |= gc6603_write_reg(dev, 0x070e,0x94);
    ret |= gc6603_write_reg(dev, 0x070f,0x0a);
    ret |= gc6603_write_reg(dev, 0x0053,0x05);
    ret |= gc6603_write_reg(dev, 0x0099,0x10);
    ret |= gc6603_write_reg(dev, 0x009b,0x08);
    ret |= gc6603_write_reg(dev, 0x0094,0x0a);
    ret |= gc6603_write_reg(dev, 0x0095,0x80);
    ret |= gc6603_write_reg(dev, 0x0096,0x08);
    ret |= gc6603_write_reg(dev, 0x0097,0x00);
    ret |= gc6603_write_reg(dev, 0x0e4c,0x3e);
    ret |= gc6603_write_reg(dev, 0x0902,0x0b);
    ret |= gc6603_write_reg(dev, 0x0903,0x15);
    ret |= gc6603_write_reg(dev, 0x0904,0x14);
    ret |= gc6603_write_reg(dev, 0x0907,0x14);
    ret |= gc6603_write_reg(dev, 0x0908,0x15);
    ret |= gc6603_write_reg(dev, 0x090e,0x26);
    ret |= gc6603_write_reg(dev, 0x090f,0x15);
    ret |= gc6603_write_reg(dev, 0x0244,0x75);
    ret |= gc6603_write_reg(dev, 0x0724,0x0c);
    ret |= gc6603_write_reg(dev, 0x0727,0x0c);
    ret |= gc6603_write_reg(dev, 0x072a,0x18);
    ret |= gc6603_write_reg(dev, 0x072b,0x19);
    ret |= gc6603_write_reg(dev, 0x0709,0x40);
    ret |= gc6603_write_reg(dev, 0x0719,0x40);
    ret |= gc6603_write_reg(dev, 0x0912,0x03);
    ret |= gc6603_write_reg(dev, 0x0913,0x00);
    ret |= gc6603_write_reg(dev, 0x0e66,0x10);
    ret |= gc6603_write_reg(dev, 0x0e69,0x80);
    ret |= gc6603_write_reg(dev, 0x0e6a,0xc0);
    ret |= gc6603_write_reg(dev, 0x0e6b,0x02);
    ret |= gc6603_write_reg(dev, 0x0223,0x00);
    ret |= gc6603_write_reg(dev, 0x0e81,0x02);
    ret |= gc6603_write_reg(dev, 0x0e30,0x00);
    ret |= gc6603_write_reg(dev, 0x0e33,0x80);
    ret |= gc6603_write_reg(dev, 0x0242,0x35);
    ret |= gc6603_write_reg(dev, 0x0243,0x08);
    ret |= gc6603_write_reg(dev, 0x0361,0xbc);
    ret |= gc6603_write_reg(dev, 0x0362,0x0f);
    ret |= gc6603_write_reg(dev, 0x0e34,0x04);
    ret |= gc6603_write_reg(dev, 0x0e47,0x55);
    ret |= gc6603_write_reg(dev, 0x0e61,0x0d);
    ret |= gc6603_write_reg(dev, 0x0e62,0x0d);
    ret |= gc6603_write_reg(dev, 0x023a,0x05);
    ret |= gc6603_write_reg(dev, 0x0e64,0x0c);
    ret |= gc6603_write_reg(dev, 0x0e20,0x0c);
    ret |= gc6603_write_reg(dev, 0x0e6e,0x20);
    ret |= gc6603_write_reg(dev, 0x0e6f,0x28);
    ret |= gc6603_write_reg(dev, 0x0e70,0x18);
    ret |= gc6603_write_reg(dev, 0x0e71,0x28);
    ret |= gc6603_write_reg(dev, 0x0e28,0x78);
    ret |= gc6603_write_reg(dev, 0x0e4d,0x80);
    ret |= gc6603_write_reg(dev, 0x0245,0x08);
    ret |= gc6603_write_reg(dev, 0x0240,0x06);
    ret |= gc6603_write_reg(dev, 0x0e63,0x06);
    ret |= gc6603_write_reg(dev, 0x0236,0x02);
    ret |= gc6603_write_reg(dev, 0x0261,0x60);
    ret |= gc6603_write_reg(dev, 0x0262,0x28);
    ret |= gc6603_write_reg(dev, 0x0072,0x00);
    ret |= gc6603_write_reg(dev, 0x0074,0x01);
    ret |= gc6603_write_reg(dev, 0x0087,0x53);
    ret |= gc6603_write_reg(dev, 0x0704,0x07);
    ret |= gc6603_write_reg(dev, 0x0705,0x28);
    ret |= gc6603_write_reg(dev, 0x0706,0x02);
    ret |= gc6603_write_reg(dev, 0x0715,0x28);
    ret |= gc6603_write_reg(dev, 0x0716,0x02);
    ret |= gc6603_write_reg(dev, 0x0708,0xc0);
    ret |= gc6603_write_reg(dev, 0x0718,0xc0);
    ret |= gc6603_write_reg(dev, 0x0076,0x01);
    ret |= gc6603_write_reg(dev, 0x021a,0x10);
    ret |= gc6603_write_reg(dev, 0x0052,0x02);
    ret |= gc6603_write_reg(dev, 0x0448,0x06);
    ret |= gc6603_write_reg(dev, 0x0449,0x04);
    ret |= gc6603_write_reg(dev, 0x044a,0x04);
    ret |= gc6603_write_reg(dev, 0x044b,0x06);
    ret |= gc6603_write_reg(dev, 0x044c,0x78);
    ret |= gc6603_write_reg(dev, 0x044d,0x7a);
    ret |= gc6603_write_reg(dev, 0x044e,0x7a);
    ret |= gc6603_write_reg(dev, 0x044f,0x78);
    ret |= gc6603_write_reg(dev, 0x0046,0x30);
    ret |= gc6603_write_reg(dev, 0x0002,0xa9);
    ret |= gc6603_write_reg(dev, 0x0005,0x83);
    ret |= gc6603_write_reg(dev, 0x0006,0x83);
    ret |= gc6603_write_reg(dev, 0x001a,0x83);
    ret |= gc6603_write_reg(dev, 0x0075,0x65);
    ret |= gc6603_write_reg(dev, 0x0202,0x08);
    ret |= gc6603_write_reg(dev, 0x0203,0x46);
    ret |= gc6603_write_reg(dev, 0x0914,0x03);
    ret |= gc6603_write_reg(dev, 0x0915,0x00);
    ret |= gc6603_write_reg(dev, 0x0225,0x00);
    ret |= gc6603_write_reg(dev, 0x0e67,0x0d);
    ret |= gc6603_write_reg(dev, 0x0e68,0x0d);
    ret |= gc6603_write_reg(dev, 0x0089,0x03);
    ret |= gc6603_write_reg(dev, 0x0144,0x00);
    ret |= gc6603_write_reg(dev, 0x0122,0x08);
    ret |= gc6603_write_reg(dev, 0x0123,0x27);
    ret |= gc6603_write_reg(dev, 0x0126,0x0a);
    ret |= gc6603_write_reg(dev, 0x0129,0x08);
    ret |= gc6603_write_reg(dev, 0x012a,0x0d);
    ret |= gc6603_write_reg(dev, 0x012b,0x0a);
    ret |= gc6603_write_reg(dev, 0x0180,0x46);
    ret |= gc6603_write_reg(dev, 0x0181,0x30);
    ret |= gc6603_write_reg(dev, 0x0185,0x01);
    ret |= gc6603_write_reg(dev, 0x0106,0x38);
    ret |= gc6603_write_reg(dev, 0x010d,0x0d);
    ret |= gc6603_write_reg(dev, 0x010e,0x20);
    ret |= gc6603_write_reg(dev, 0x0111,0x2b);
    ret |= gc6603_write_reg(dev, 0x0112,0x0a);
    ret |= gc6603_write_reg(dev, 0x0113,0x0a);
    ret |= gc6603_write_reg(dev, 0x0114,0x01);
    //ret |= gc6603_write_reg(dev, 0x0100,0x09);
    ret |= gc6603_write_reg(dev, 0x0221,0x05);
    ret |= gc6603_write_reg(dev, 0x023b,0x13);
    ret |= gc6603_write_reg(dev, 0x0352,0x70);
    ret |= gc6603_write_reg(dev, 0x0357,0x00);
    ret |= gc6603_write_reg(dev, 0x0b00,0x40);
    ret |= gc6603_write_reg(dev, 0x08ef,0x01);
    ret |= gc6603_write_reg(dev, 0x03fe,0x00);
    ret |= gc6603_write_reg(dev, 0x031f,0x01);
    ret |= gc6603_write_reg(dev, 0x031f,0x00);
    ret |= gc6603_write_reg(dev, 0x0318,0x0e);
    ret |= gc6603_write_reg(dev, 0x0a67,0x80);
    ret |= gc6603_write_reg(dev, 0x0a50,0x41);
    ret |= gc6603_write_reg(dev, 0x0a51,0x41);
    ret |= gc6603_write_reg(dev, 0x0a52,0x41);
    ret |= gc6603_write_reg(dev, 0x0a54,0x26);
    ret |= gc6603_write_reg(dev, 0x0a55,0x26);
    ret |= gc6603_write_reg(dev, 0x0a4e,0x0c);
    ret |= gc6603_write_reg(dev, 0x0a4f,0x0c);
    ret |= gc6603_write_reg(dev, 0x0a65,0x17);
    ret |= gc6603_write_reg(dev, 0x0a53,0x00);
    ret |= gc6603_write_reg(dev, 0x0a98,0x04);
    ret |= gc6603_write_reg(dev, 0x05be,0x00);
    ret |= gc6603_write_reg(dev, 0x05a9,0x01);
    ret |= gc6603_write_reg(dev, 0x0a67,0x80);
    ret |= gc6603_write_reg(dev, 0x0023,0x00);
    ret |= gc6603_write_reg(dev, 0x0025,0x00);
    ret |= gc6603_write_reg(dev, 0x0028,0x0a);
    ret |= gc6603_write_reg(dev, 0x0029,0x90);
    ret |= gc6603_write_reg(dev, 0x002a,0x08);
    ret |= gc6603_write_reg(dev, 0x002b,0x20);
    ret |= gc6603_write_reg(dev, 0x0a8b,0x0a);
    ret |= gc6603_write_reg(dev, 0x0a8a,0x90);
    ret |= gc6603_write_reg(dev, 0x0a89,0x08);
    ret |= gc6603_write_reg(dev, 0x0a88,0x20);
    ret |= gc6603_write_reg(dev, 0x0a70,0x07);
    ret |= gc6603_write_reg(dev, 0x0a73,0xe0);
    ret |= gc6603_write_reg(dev, 0x0a80,0x7b);
    ret |= gc6603_write_reg(dev, 0x0a82,0x00);
    ret |= gc6603_write_reg(dev, 0x0a83,0x80);
    ret |= gc6603_write_reg(dev, 0x0a5a,0x80);
    gc6603_delay_ms(20);
    ret |= gc6603_write_reg(dev, 0x05be,0x01);
    ret |= gc6603_write_reg(dev, 0x0a70,0x00);
    ret |= gc6603_write_reg(dev, 0x0080,0x02);
    ret |= gc6603_write_reg(dev, 0x0021,0x40);
    ret |= gc6603_write_reg(dev, 0x0a67,0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc6603_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC6603_24MInput_MIPI_2lane_10bit_linear_2688x2048_25fps init success!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc6603_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC6603_5M_10BIT_LINEAR_MODE:
            ret = gc6603_2lane_linear_2688x2048_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc6603_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc6603_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc6603_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc6603_i2c_fd[dev]);
        g_gc6603_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc6603_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc6603_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc6603_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc6603_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc6603_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc6603_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc6603_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc6603_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc6603_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc6603_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC6603_DATA_BYTE];

    if (g_gc6603_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc6603_i2c_fd[dev], g_gc6603_i2c_addr[dev], addr, GC6603_ADDR_BYTE, buf, GC6603_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC6603_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc6603_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc6603_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc6603_i2c_fd[dev], g_gc6603_i2c_addr[dev], buf, GC6603_ADDR_BYTE + GC6603_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc6603_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((!mirror_en) && (!flip_en)) {
         ret |= gc6603_write_reg(dev, 0x031f,0x01);
        ret |= gc6603_write_reg(dev, 0x031f,0x00);
        ret |= gc6603_write_reg(dev, 0x0318,0x0e);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0a50,0x41);
        ret |= gc6603_write_reg(dev, 0x0a51,0x41);
        ret |= gc6603_write_reg(dev, 0x0a52,0x41);
        ret |= gc6603_write_reg(dev, 0x0a54,0x26);
        ret |= gc6603_write_reg(dev, 0x0a55,0x26);
        ret |= gc6603_write_reg(dev, 0x0a4e,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a4f,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a65,0x17);
        ret |= gc6603_write_reg(dev, 0x0a53,0x00);
        ret |= gc6603_write_reg(dev, 0x0a98,0x04);
        ret |= gc6603_write_reg(dev, 0x05be,0x00);
        ret |= gc6603_write_reg(dev, 0x05a9,0x01);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0023,0x00);
        ret |= gc6603_write_reg(dev, 0x0025,0x00);
        ret |= gc6603_write_reg(dev, 0x0028,0x0a);
        ret |= gc6603_write_reg(dev, 0x0029,0x90);
        ret |= gc6603_write_reg(dev, 0x002a,0x08);
        ret |= gc6603_write_reg(dev, 0x002b,0x20);
        ret |= gc6603_write_reg(dev, 0x0a70,0x07);
        ret |= gc6603_write_reg(dev, 0x0a73,0xe0);
        ret |= gc6603_write_reg(dev, 0x022c,0x00);
        ret |= gc6603_write_reg(dev, 0x0063,0x00);
        ret |= gc6603_write_reg(dev, 0x0722,0x00);
        ret |= gc6603_write_reg(dev, 0x0a80,0x7b);
        ret |= gc6603_write_reg(dev, 0x0a82,0x00);
        ret |= gc6603_write_reg(dev, 0x0a83,0x80);
        ret |= gc6603_write_reg(dev, 0x0a5a,0x80);
        gc6603_delay_ms(20);
        ret |= gc6603_write_reg(dev, 0x05be,0x01);
        ret |= gc6603_write_reg(dev, 0x0080,0x02);
        ret |= gc6603_write_reg(dev, 0x0021,0x40);
        ret |= gc6603_write_reg(dev, 0x0089,0x03);
    } else if ((mirror_en) && (!flip_en)) {
        ret |= gc6603_write_reg(dev, 0x031f,0x01);
        ret |= gc6603_write_reg(dev, 0x031f,0x00);
        ret |= gc6603_write_reg(dev, 0x0318,0x0e);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0a50,0x41);
        ret |= gc6603_write_reg(dev, 0x0a51,0x41);
        ret |= gc6603_write_reg(dev, 0x0a52,0x41);
        ret |= gc6603_write_reg(dev, 0x0a54,0x26);
        ret |= gc6603_write_reg(dev, 0x0a55,0x26);
        ret |= gc6603_write_reg(dev, 0x0a4e,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a4f,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a65,0x17);
        ret |= gc6603_write_reg(dev, 0x0a53,0x00);
        ret |= gc6603_write_reg(dev, 0x0a98,0x04);
        ret |= gc6603_write_reg(dev, 0x05be,0x00);
        ret |= gc6603_write_reg(dev, 0x05a9,0x01);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0023,0x00);
        ret |= gc6603_write_reg(dev, 0x0025,0x00);
        ret |= gc6603_write_reg(dev, 0x0028,0x0a);
        ret |= gc6603_write_reg(dev, 0x0029,0x90);
        ret |= gc6603_write_reg(dev, 0x002a,0x08);
        ret |= gc6603_write_reg(dev, 0x002b,0x20);
        ret |= gc6603_write_reg(dev, 0x0a70,0x07);
        ret |= gc6603_write_reg(dev, 0x0a73,0xe1);
        ret |= gc6603_write_reg(dev, 0x022c,0x01);
        ret |= gc6603_write_reg(dev, 0x0063,0x01);
        ret |= gc6603_write_reg(dev, 0x0722,0x02);
        ret |= gc6603_write_reg(dev, 0x0a80,0x7b);
        ret |= gc6603_write_reg(dev, 0x0a82,0x00);
        ret |= gc6603_write_reg(dev, 0x0a83,0x80);
        ret |= gc6603_write_reg(dev, 0x0a5a,0x80);
        gc6603_delay_ms(20);
        ret |= gc6603_write_reg(dev, 0x05be,0x01);
        ret |= gc6603_write_reg(dev, 0x0080,0x02);
        ret |= gc6603_write_reg(dev, 0x0021,0x40);
        ret |= gc6603_write_reg(dev, 0x0089,0x03);
    } else if ((!mirror_en) && (flip_en)) {
        ret |= gc6603_write_reg(dev, 0x031f,0x01);
        ret |= gc6603_write_reg(dev, 0x031f,0x00);
        ret |= gc6603_write_reg(dev, 0x0318,0x0e);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0a50,0x41);
        ret |= gc6603_write_reg(dev, 0x0a51,0x41);
        ret |= gc6603_write_reg(dev, 0x0a52,0x41);
        ret |= gc6603_write_reg(dev, 0x0a54,0x26);
        ret |= gc6603_write_reg(dev, 0x0a55,0x26);
        ret |= gc6603_write_reg(dev, 0x0a4e,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a4f,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a65,0x17);
        ret |= gc6603_write_reg(dev, 0x0a53,0x00);
        ret |= gc6603_write_reg(dev, 0x0a98,0x04);
        ret |= gc6603_write_reg(dev, 0x05be,0x00);
        ret |= gc6603_write_reg(dev, 0x05a9,0x01);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0023,0x00);
        ret |= gc6603_write_reg(dev, 0x0025,0x00);
        ret |= gc6603_write_reg(dev, 0x0028,0x0a);
        ret |= gc6603_write_reg(dev, 0x0029,0x90);
        ret |= gc6603_write_reg(dev, 0x002a,0x08);
        ret |= gc6603_write_reg(dev, 0x002b,0x20);
        ret |= gc6603_write_reg(dev, 0x0a70,0x07);
        ret |= gc6603_write_reg(dev, 0x0a73,0xe2);
        ret |= gc6603_write_reg(dev, 0x022c,0x02);
        ret |= gc6603_write_reg(dev, 0x0063,0x02);
        ret |= gc6603_write_reg(dev, 0x0722,0x00);
        ret |= gc6603_write_reg(dev, 0x0a80,0x7b);
        ret |= gc6603_write_reg(dev, 0x0a82,0x00);
        ret |= gc6603_write_reg(dev, 0x0a83,0x80);
        ret |= gc6603_write_reg(dev, 0x0a5a,0x80);
        gc6603_delay_ms(20);
        ret |= gc6603_write_reg(dev, 0x05be,0x01);
        ret |= gc6603_write_reg(dev, 0x0080,0x02);
        ret |= gc6603_write_reg(dev, 0x0021,0x40);
        ret |= gc6603_write_reg(dev, 0x0089,0x03);
    } else if ((mirror_en) && (flip_en)) {
        ret |= gc6603_write_reg(dev, 0x031f,0x01);
        ret |= gc6603_write_reg(dev, 0x031f,0x00);
        ret |= gc6603_write_reg(dev, 0x0318,0x0e);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0a50,0x41);
        ret |= gc6603_write_reg(dev, 0x0a51,0x41);
        ret |= gc6603_write_reg(dev, 0x0a52,0x41);
        ret |= gc6603_write_reg(dev, 0x0a54,0x26);
        ret |= gc6603_write_reg(dev, 0x0a55,0x26);
        ret |= gc6603_write_reg(dev, 0x0a4e,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a4f,0x0c);
        ret |= gc6603_write_reg(dev, 0x0a65,0x17);
        ret |= gc6603_write_reg(dev, 0x0a53,0x00);
        ret |= gc6603_write_reg(dev, 0x0a98,0x04);
        ret |= gc6603_write_reg(dev, 0x05be,0x00);
        ret |= gc6603_write_reg(dev, 0x05a9,0x01);
        ret |= gc6603_write_reg(dev, 0x0a67,0x80);
        ret |= gc6603_write_reg(dev, 0x0023,0x00);
        ret |= gc6603_write_reg(dev, 0x0025,0x00);
        ret |= gc6603_write_reg(dev, 0x0028,0x0a);
        ret |= gc6603_write_reg(dev, 0x0029,0x90);
        ret |= gc6603_write_reg(dev, 0x002a,0x08);
        ret |= gc6603_write_reg(dev, 0x002b,0x20);
        ret |= gc6603_write_reg(dev, 0x0a70,0x07);
        ret |= gc6603_write_reg(dev, 0x0a73,0xe3);
        ret |= gc6603_write_reg(dev, 0x022c,0x03);
        ret |= gc6603_write_reg(dev, 0x0063,0x03);
        ret |= gc6603_write_reg(dev, 0x0722,0x02);
        ret |= gc6603_write_reg(dev, 0x0a80,0x7b);
        ret |= gc6603_write_reg(dev, 0x0a82,0x00);
        ret |= gc6603_write_reg(dev, 0x0a83,0x80);
        ret |= gc6603_write_reg(dev, 0x0a5a,0x80);
        gc6603_delay_ms(20);
        ret |= gc6603_write_reg(dev, 0x05be,0x01);
        ret |= gc6603_write_reg(dev, 0x0080,0x02);
        ret |= gc6603_write_reg(dev, 0x0021,0x40);
        ret |= gc6603_write_reg(dev, 0x0089,0x03);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc6603_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc6603_set_stream_mode(xmedia_u32 dev, xmedia_bool stream_en) {
    xmedia_s32 ret;

    if(stream_en == XMEDIA_TRUE) {
        ret = gc6603_write_reg(dev, GC6603_REG_ADDR_STANDBY, 0x9);
        if (ret != XMEDIA_SUCCESS) {
            SENSOR_TRACE(MODULE_DBG_ERR, "gc6603_start_stream failed\n");
            return XMEDIA_FAILURE;
        }
    }

    return XMEDIA_SUCCESS;
}

