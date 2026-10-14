#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc4683.h"
#include "gc4683_ctrl.h"

#define GC4683_REG_ADDR_MIRROR_FLIP 0x022c

static xmedia_s32 g_gc4683_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_gc4683_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static void gc4683_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

static xmedia_s32 gc4683_4lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
//version:0e70f3
//<MODE_2 type="00_GC4683_MIPI4L_27M_2560x1440_30fps_raw10_linear">
//<RESOLUTION width="2560" height="1440"/>
//<MCLK type="27.0"/>
//<BAYERMODE type="GrRBGb"/>
//mipi_rate 801Mbps/lane
//wpclk=211.5Mhz
//rpclk=319.5Mhz
//FL=3200
//HB=552
    ret|=gc4683_write_reg(dev,0x03fe,0xf0);
    ret|=gc4683_write_reg(dev,0x03fe,0x00);
    ret|=gc4683_write_reg(dev,0x03fe,0x10);
    ret|=gc4683_write_reg(dev,0x0a38,0x00);
    ret|=gc4683_write_reg(dev,0x0a38,0x05);
    ret|=gc4683_write_reg(dev,0x0331,0x07);
    ret|=gc4683_write_reg(dev,0x0320,0xf2);
    ret|=gc4683_write_reg(dev,0x0a22,0x04);
    ret|=gc4683_write_reg(dev,0x0a27,0x02);
    ret|=gc4683_write_reg(dev,0x0a20,0x1a);
    ret|=gc4683_write_reg(dev,0x0a21,0x1a);
    ret|=gc4683_write_reg(dev,0x032b,0x54);
    ret|=gc4683_write_reg(dev,0x032a,0x55);
    ret|=gc4683_write_reg(dev,0x0a22,0x17);
    ret|=gc4683_write_reg(dev,0x0a23,0x24);
    ret|=gc4683_write_reg(dev,0x0a24,0x0c);
    ret|=gc4683_write_reg(dev,0x0a25,0x47);
    ret|=gc4683_write_reg(dev,0x0a34,0x00);
    ret|=gc4683_write_reg(dev,0x0a35,0x60);
    ret|=gc4683_write_reg(dev,0x0a36,0x0c);
    ret|=gc4683_write_reg(dev,0x0a37,0x59);
    ret|=gc4683_write_reg(dev,0x0a27,0x0b);
    ret|=gc4683_write_reg(dev,0x0a28,0x24);
    ret|=gc4683_write_reg(dev,0x0a29,0x14);
    ret|=gc4683_write_reg(dev,0x0a2a,0x9d);
    ret|=gc4683_write_reg(dev,0x031c,0x46);
    ret|=gc4683_write_reg(dev,0x0213,0x1c);
    ret|=gc4683_write_reg(dev,0x0219,0xc7);
    ret|=gc4683_write_reg(dev,0x0261,0x98);
    ret|=gc4683_write_reg(dev,0x0342,0x02);
    ret|=gc4683_write_reg(dev,0x0343,0x28);
    ret|=gc4683_write_reg(dev,0x0259,0x06);
    ret|=gc4683_write_reg(dev,0x025a,0x18);
    ret|=gc4683_write_reg(dev,0x0340,0x0c);
    ret|=gc4683_write_reg(dev,0x0341,0x80);
    ret|=gc4683_write_reg(dev,0x0346,0x00);
    ret|=gc4683_write_reg(dev,0x0347,0x30);
    ret|=gc4683_write_reg(dev,0x0348,0x0a);
    ret|=gc4683_write_reg(dev,0x0349,0x08);
    ret|=gc4683_write_reg(dev,0x034a,0x05);
    ret|=gc4683_write_reg(dev,0x034b,0xa8);
    ret|=gc4683_write_reg(dev,0x034e,0x0a);
    ret|=gc4683_write_reg(dev,0x034f,0xb0);
    ret|=gc4683_write_reg(dev,0x0094,0x0a);
    ret|=gc4683_write_reg(dev,0x0095,0x00);
    ret|=gc4683_write_reg(dev,0x0096,0x05);
    ret|=gc4683_write_reg(dev,0x0097,0xa0);
    ret|=gc4683_write_reg(dev,0x0099,0x04);
    ret|=gc4683_write_reg(dev,0x009b,0x04);
    ret|=gc4683_write_reg(dev,0x070c,0x00);
    ret|=gc4683_write_reg(dev,0x070d,0x0a);
    ret|=gc4683_write_reg(dev,0x070e,0x05);
    ret|=gc4683_write_reg(dev,0x070f,0x0e);
    ret|=gc4683_write_reg(dev,0x0902,0x0b);
    ret|=gc4683_write_reg(dev,0x0903,0x03);
    ret|=gc4683_write_reg(dev,0x0904,0x0a);
    ret|=gc4683_write_reg(dev,0x0907,0x35);
    ret|=gc4683_write_reg(dev,0x0909,0x07);
    ret|=gc4683_write_reg(dev,0x090d,0x0c);
    ret|=gc4683_write_reg(dev,0x0276,0x07);
    ret|=gc4683_write_reg(dev,0x0277,0xa4);
    ret|=gc4683_write_reg(dev,0x0278,0x3d);
    ret|=gc4683_write_reg(dev,0x0279,0x57);
    ret|=gc4683_write_reg(dev,0x027b,0x16);
    ret|=gc4683_write_reg(dev,0x072a,0x38);
    ret|=gc4683_write_reg(dev,0x0724,0x03);
    ret|=gc4683_write_reg(dev,0x0727,0x03);
    ret|=gc4683_write_reg(dev,0x072a,0x38);
    ret|=gc4683_write_reg(dev,0x072b,0x19);
    ret|=gc4683_write_reg(dev,0x072f,0x02);
    ret|=gc4683_write_reg(dev,0x0002,0x80);
    ret|=gc4683_write_reg(dev,0x0004,0x1f);
    ret|=gc4683_write_reg(dev,0x0060,0x40);
    ret|=gc4683_write_reg(dev,0x0038,0x40);
    ret|=gc4683_write_reg(dev,0x0039,0x40);
    ret|=gc4683_write_reg(dev,0x003a,0x40);
    ret|=gc4683_write_reg(dev,0x003b,0x40);
    ret|=gc4683_write_reg(dev,0x02ac,0x00);
    ret|=gc4683_write_reg(dev,0x0274,0x0a);
    ret|=gc4683_write_reg(dev,0x02ad,0x04);
    ret|=gc4683_write_reg(dev,0x02ae,0x01);
    ret|=gc4683_write_reg(dev,0x0247,0x00);
    ret|=gc4683_write_reg(dev,0x0248,0x00);
    ret|=gc4683_write_reg(dev,0x1466,0x20);
    ret|=gc4683_write_reg(dev,0x1467,0x24);
    ret|=gc4683_write_reg(dev,0x1468,0x24);
    ret|=gc4683_write_reg(dev,0x1469,0x02);
    ret|=gc4683_write_reg(dev,0x146a,0x40);
    ret|=gc4683_write_reg(dev,0x146b,0x00);
    ret|=gc4683_write_reg(dev,0x0707,0x08);
    ret|=gc4683_write_reg(dev,0x0704,0x00);
    ret|=gc4683_write_reg(dev,0x0719,0x00);
    ret|=gc4683_write_reg(dev,0x071a,0x40);
    ret|=gc4683_write_reg(dev,0x021b,0xb0);
    ret|=gc4683_write_reg(dev,0x0006,0x00);
    ret|=gc4683_write_reg(dev,0x0216,0x01);
    ret|=gc4683_write_reg(dev,0x027c,0x0f);
    ret|=gc4683_write_reg(dev,0x1430,0x00);
    ret|=gc4683_write_reg(dev,0x1409,0x03);
    ret|=gc4683_write_reg(dev,0x143a,0x03);
    ret|=gc4683_write_reg(dev,0x1433,0x80);
    ret|=gc4683_write_reg(dev,0x140f,0x21);
    ret|=gc4683_write_reg(dev,0x1461,0x20);
    ret|=gc4683_write_reg(dev,0x1462,0x20);
    ret|=gc4683_write_reg(dev,0x146e,0x40);
    ret|=gc4683_write_reg(dev,0x146f,0x02);
    ret|=gc4683_write_reg(dev,0x1470,0x3e);
    ret|=gc4683_write_reg(dev,0x1471,0x02);
    ret|=gc4683_write_reg(dev,0x1474,0x40);
    ret|=gc4683_write_reg(dev,0x1479,0x12);
    ret|=gc4683_write_reg(dev,0x1485,0x06);
    ret|=gc4683_write_reg(dev,0x1475,0x12);
    ret|=gc4683_write_reg(dev,0x1476,0xe0);
    ret|=gc4683_write_reg(dev,0x14a1,0x0d);
    ret|=gc4683_write_reg(dev,0x14a8,0x70);
    ret|=gc4683_write_reg(dev,0x14a6,0x30);
    ret|=gc4683_write_reg(dev,0x1420,0x14);
    ret|=gc4683_write_reg(dev,0x1464,0x15);
    ret|=gc4683_write_reg(dev,0x146c,0x08);
    ret|=gc4683_write_reg(dev,0x146d,0x08);
    ret|=gc4683_write_reg(dev,0x1423,0x08);
    ret|=gc4683_write_reg(dev,0x1428,0x40);
    ret|=gc4683_write_reg(dev,0x0245,0xd9);
    ret|=gc4683_write_reg(dev,0x023a,0x08);
    ret|=gc4683_write_reg(dev,0x02cd,0x42);
    ret|=gc4683_write_reg(dev,0x0243,0x04);
    ret|=gc4683_write_reg(dev,0x029e,0x3f);
    ret|=gc4683_write_reg(dev,0x029d,0x3c);
    ret|=gc4683_write_reg(dev,0x0089,0x03);
    ret|=gc4683_write_reg(dev,0x0040,0xa3);
    ret|=gc4683_write_reg(dev,0x0075,0x60);
    ret|=gc4683_write_reg(dev,0x0004,0x1f);
    ret|=gc4683_write_reg(dev,0x0002,0x82);
    ret|=gc4683_write_reg(dev,0x0053,0x00);
    ret|=gc4683_write_reg(dev,0x0205,0x0c);
    ret|=gc4683_write_reg(dev,0x0317,0x00);
    ret|=gc4683_write_reg(dev,0x021a,0x10);
    ret|=gc4683_write_reg(dev,0x0076,0x01);
    ret|=gc4683_write_reg(dev,0x0054,0x98);
    ret|=gc4683_write_reg(dev,0x0042,0x60);
    ret|=gc4683_write_reg(dev,0x0052,0x02);
    ret|=gc4683_write_reg(dev,0x0046,0x60);
    ret|=gc4683_write_reg(dev,0x0448,0x09);
    ret|=gc4683_write_reg(dev,0x0449,0x09);
    ret|=gc4683_write_reg(dev,0x044a,0x09);
    ret|=gc4683_write_reg(dev,0x044b,0x09);
    ret|=gc4683_write_reg(dev,0x044c,0x77);
    ret|=gc4683_write_reg(dev,0x044d,0x77);
    ret|=gc4683_write_reg(dev,0x044e,0x77);
    ret|=gc4683_write_reg(dev,0x044f,0x77);
    ret|=gc4683_write_reg(dev,0x0468,0x00);
    ret|=gc4683_write_reg(dev,0x0010,0x08);
    ret|=gc4683_write_reg(dev,0x04b0,0x30);
    ret|=gc4683_write_reg(dev,0x04b1,0x10);
    ret|=gc4683_write_reg(dev,0x04c0,0x20);
    ret|=gc4683_write_reg(dev,0x04c1,0x20);
    ret|=gc4683_write_reg(dev,0x04d0,0x20);
    ret|=gc4683_write_reg(dev,0x04d1,0x5f);
    ret|=gc4683_write_reg(dev,0x04b2,0x30);
    ret|=gc4683_write_reg(dev,0x04b3,0x10);
    ret|=gc4683_write_reg(dev,0x04c2,0x20);
    ret|=gc4683_write_reg(dev,0x04c3,0x20);
    ret|=gc4683_write_reg(dev,0x04d2,0x20);
    ret|=gc4683_write_reg(dev,0x04d3,0x5f);
    ret|=gc4683_write_reg(dev,0x04b4,0x30);
    ret|=gc4683_write_reg(dev,0x04b5,0x10);
    ret|=gc4683_write_reg(dev,0x04c4,0x20);
    ret|=gc4683_write_reg(dev,0x04c5,0x20);
    ret|=gc4683_write_reg(dev,0x04d4,0x20);
    ret|=gc4683_write_reg(dev,0x04d5,0x5f);
    ret|=gc4683_write_reg(dev,0x04b6,0x40);
    ret|=gc4683_write_reg(dev,0x04b7,0x10);
    ret|=gc4683_write_reg(dev,0x04c6,0x20);
    ret|=gc4683_write_reg(dev,0x04c7,0x20);
    ret|=gc4683_write_reg(dev,0x04d6,0x20);
    ret|=gc4683_write_reg(dev,0x04d7,0x5f);
    ret|=gc4683_write_reg(dev,0x04b8,0x40);
    ret|=gc4683_write_reg(dev,0x04b9,0x10);
    ret|=gc4683_write_reg(dev,0x04c8,0x20);
    ret|=gc4683_write_reg(dev,0x04c9,0x20);
    ret|=gc4683_write_reg(dev,0x04d8,0x20);
    ret|=gc4683_write_reg(dev,0x04d9,0x5f);
    ret|=gc4683_write_reg(dev,0x04ba,0x40);
    ret|=gc4683_write_reg(dev,0x04bb,0x10);
    ret|=gc4683_write_reg(dev,0x04ca,0x20);
    ret|=gc4683_write_reg(dev,0x04cb,0x20);
    ret|=gc4683_write_reg(dev,0x04da,0x20);
    ret|=gc4683_write_reg(dev,0x04db,0x5f);
    ret|=gc4683_write_reg(dev,0x04bc,0x40);
    ret|=gc4683_write_reg(dev,0x04bd,0x10);
    ret|=gc4683_write_reg(dev,0x04cc,0x20);
    ret|=gc4683_write_reg(dev,0x04cd,0x20);
    ret|=gc4683_write_reg(dev,0x04dc,0x20);
    ret|=gc4683_write_reg(dev,0x04dd,0x5f);
    ret|=gc4683_write_reg(dev,0x04be,0x50);
    ret|=gc4683_write_reg(dev,0x04bf,0x10);
    ret|=gc4683_write_reg(dev,0x04ce,0x20);
    ret|=gc4683_write_reg(dev,0x04cf,0x20);
    ret|=gc4683_write_reg(dev,0x04de,0x20);
    ret|=gc4683_write_reg(dev,0x04df,0x5f);
    ret|=gc4683_write_reg(dev,0x0704,0x07);
    ret|=gc4683_write_reg(dev,0x0715,0x04);
    ret|=gc4683_write_reg(dev,0x0716,0xb0);
    ret|=gc4683_write_reg(dev,0x0718,0xd0);
    ret|=gc4683_write_reg(dev,0x071b,0x00);
    ret|=gc4683_write_reg(dev,0x071c,0x40);
    ret|=gc4683_write_reg(dev,0x071d,0x00);
    ret|=gc4683_write_reg(dev,0x071e,0x40);
    ret|=gc4683_write_reg(dev,0x031f,0x02);
    ret|=gc4683_write_reg(dev,0x031f,0x00);
    ret|=gc4683_write_reg(dev,0x0a67,0x80);
    ret|=gc4683_write_reg(dev,0x0a51,0x41);
    ret|=gc4683_write_reg(dev,0x0a52,0x41);
    ret|=gc4683_write_reg(dev,0x0a4e,0x0c);
    ret|=gc4683_write_reg(dev,0x0a4f,0x0c);
    ret|=gc4683_write_reg(dev,0x0a54,0x36);
    ret|=gc4683_write_reg(dev,0x0a55,0x36);
    ret|=gc4683_write_reg(dev,0x0a9f,0x17);
    ret|=gc4683_write_reg(dev,0x0a9e,0x10);
    ret|=gc4683_write_reg(dev,0x0aa1,0x10);
    ret|=gc4683_write_reg(dev,0x0a53,0x00);
    ret|=gc4683_write_reg(dev,0x05be,0x00);
    ret|=gc4683_write_reg(dev,0x05a9,0x01);
    ret|=gc4683_write_reg(dev,0x0028,0x0a);
    ret|=gc4683_write_reg(dev,0x0029,0x08);
    ret|=gc4683_write_reg(dev,0x002a,0x05);
    ret|=gc4683_write_reg(dev,0x002b,0xa8);
    ret|=gc4683_write_reg(dev,0x0022,0x00);
    ret|=gc4683_write_reg(dev,0x0023,0x00);
    ret|=gc4683_write_reg(dev,0x0024,0x00);
    ret|=gc4683_write_reg(dev,0x0025,0x00);
    ret|=gc4683_write_reg(dev,0x0a70,0x03);
    ret|=gc4683_write_reg(dev,0x0a71,0x02);
    ret|=gc4683_write_reg(dev,0x0a73,0x60);
    ret|=gc4683_write_reg(dev,0x0a82,0x01);
    ret|=gc4683_write_reg(dev,0x0a83,0x10);
    ret|=gc4683_write_reg(dev,0x0a5a,0x80);
    ret|=gc4683_write_reg(dev,0x0313,0x80);
    gc4683_delay_ms(20);
    ret|=gc4683_write_reg(dev,0x05be,0x01);
    ret|=gc4683_write_reg(dev,0x0080,0x02);
    ret|=gc4683_write_reg(dev,0x0021,0x40);
    ret|=gc4683_write_reg(dev,0x0020,0x8c);
    ret|=gc4683_write_reg(dev,0x0202,0x01);
    ret|=gc4683_write_reg(dev,0x0203,0x00);
    ret|=gc4683_write_reg(dev,0x02aa,0x00);
    ret|=gc4683_write_reg(dev,0x02ab,0x00);
    ret|=gc4683_write_reg(dev,0x0181,0xf0);
    ret|=gc4683_write_reg(dev,0x0185,0x08);
    ret|=gc4683_write_reg(dev,0x0111,0x2b);
    ret|=gc4683_write_reg(dev,0x0180,0x46);
    ret|=gc4683_write_reg(dev,0x02ce,0x45);
    ret|=gc4683_write_reg(dev,0x0100,0x09);
    ret|=gc4683_write_reg(dev,0x0106,0x38);
    ret|=gc4683_write_reg(dev,0x010d,0x0c);
    ret|=gc4683_write_reg(dev,0x010e,0x80);
    ret|=gc4683_write_reg(dev,0x0112,0x01);
    ret|=gc4683_write_reg(dev,0x0114,0x03);
    ret|=gc4683_write_reg(dev,0x0115,0x10);
    ret|=gc4683_write_reg(dev,0x0125,0x20);
    ret|=gc4683_write_reg(dev,0x0124,0x02);
    ret|=gc4683_write_reg(dev,0x0122,0x06);
    ret|=gc4683_write_reg(dev,0x0123,0x20);
    ret|=gc4683_write_reg(dev,0x0126,0x08);
    ret|=gc4683_write_reg(dev,0x0121,0x10);
    ret|=gc4683_write_reg(dev,0x0129,0x08);
    ret|=gc4683_write_reg(dev,0x012a,0x0a);
    ret|=gc4683_write_reg(dev,0x012b,0x08);
    ret|=gc4683_write_reg(dev,0x03fe,0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc4683_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= GC4683_27MInput_MIPI_4lane_10bit_2560x1440_30fps init success!=========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 gc4683_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC4683_4M_10BIT_LINEAR_MODE:
            ret = gc4683_4lane_linear_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4683_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc4683_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc4683_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc4683_i2c_fd[dev]);
        g_gc4683_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc4683_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4683_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc4683_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc4683_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc4683_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc4683_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4683_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc4683_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc4683_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4683_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC4683_DATA_BYTE];

    if (g_gc4683_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc4683_i2c_fd[dev], g_gc4683_i2c_addr[dev], addr, GC4683_ADDR_BYTE, buf, GC4683_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC4683_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4683_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc4683_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc4683_i2c_fd[dev], g_gc4683_i2c_addr[dev], buf, GC4683_ADDR_BYTE + GC4683_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4683_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((!mirror_en) && (!flip_en)) {
        ret |= gc4683_write_reg(dev, 0x022c, 0x00);
    } else if ((mirror_en) && (!flip_en)) {
        ret |= gc4683_write_reg(dev, 0x022c, 0x01);
    } else if ((!mirror_en) && (flip_en)) {
        ret |= gc4683_write_reg(dev, 0x022c, 0x02);
    } else if ((mirror_en) && (flip_en)) {
        ret |= gc4683_write_reg(dev, 0x022c, 0x03);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc4683_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

