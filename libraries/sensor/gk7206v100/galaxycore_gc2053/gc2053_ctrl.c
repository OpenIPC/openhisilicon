#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc2053.h"
#include "gc2053_ctrl.h"

#define GC2053_REG_ADDR_MIRROR_FLIP 0x0017

SENSOR_PRIORITY_DATA static xmedia_s32 g_gc2053_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_gc2053_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

#ifdef FPGA
#ifdef GC2053_DVP
static xmedia_s32 gc2053_dvp_linear_1920X1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc2053_write_reg(dev,0xfe ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0xf2 ,0x00);
    ret |= gc2053_write_reg(dev,0xf3 ,0x0f);
    ret |= gc2053_write_reg(dev,0xf4 ,0x36);
    ret |= gc2053_write_reg(dev,0xf5 ,0xc0);
    ret |= gc2053_write_reg(dev,0xf6 ,0x44);
    ret |= gc2053_write_reg(dev,0xf7 ,0x01);
    ret |= gc2053_write_reg(dev,0xf8 ,0x2c);
    ret |= gc2053_write_reg(dev,0xf9 ,0x42);
    ret |= gc2053_write_reg(dev,0xfc ,0x8e);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0x87 ,0x18);
    ret |= gc2053_write_reg(dev,0xee ,0x30);
    ret |= gc2053_write_reg(dev,0xd0 ,0xb7);
    ret |= gc2053_write_reg(dev,0x03 ,0x04);
    ret |= gc2053_write_reg(dev,0x04 ,0x60);
    ret |= gc2053_write_reg(dev,0x05 ,0x05); //line length
    ret |= gc2053_write_reg(dev,0x06 ,0x28);
    ret |= gc2053_write_reg(dev,0x07 ,0x00);//VBlank
    ret |= gc2053_write_reg(dev,0x08 ,0x11);
    ret |= gc2053_write_reg(dev,0x09 ,0x00);
    ret |= gc2053_write_reg(dev,0x0a ,0x02);
    ret |= gc2053_write_reg(dev,0x0b ,0x00);
    ret |= gc2053_write_reg(dev,0x0c ,0x02);
    ret |= gc2053_write_reg(dev,0x0d ,0x04);//valid line time
    ret |= gc2053_write_reg(dev,0x0e ,0x40);
    ret |= gc2053_write_reg(dev,0x12 ,0xe2);
    ret |= gc2053_write_reg(dev,0x13 ,0x16);
    ret |= gc2053_write_reg(dev,0x19 ,0x0a);
    ret |= gc2053_write_reg(dev,0x21 ,0x1c);
    ret |= gc2053_write_reg(dev,0x28 ,0x0a);
    ret |= gc2053_write_reg(dev,0x29 ,0x24);
    ret |= gc2053_write_reg(dev,0x2b ,0x04);
    ret |= gc2053_write_reg(dev,0x32 ,0xf8);
    ret |= gc2053_write_reg(dev,0x37 ,0x03);
    ret |= gc2053_write_reg(dev,0x39 ,0x15);
    ret |= gc2053_write_reg(dev,0x43 ,0x07);
    ret |= gc2053_write_reg(dev,0x44 ,0x40);
    ret |= gc2053_write_reg(dev,0x46 ,0x0b);
    ret |= gc2053_write_reg(dev,0x4b ,0x20);
    ret |= gc2053_write_reg(dev,0x4e ,0x08);
    ret |= gc2053_write_reg(dev,0x55 ,0x20);
    ret |= gc2053_write_reg(dev,0x66 ,0x05);
    ret |= gc2053_write_reg(dev,0x67 ,0x05);
    ret |= gc2053_write_reg(dev,0x77 ,0x01);
    ret |= gc2053_write_reg(dev,0x78 ,0x00);
    ret |= gc2053_write_reg(dev,0x7c ,0x93);
    ret |= gc2053_write_reg(dev,0x8c ,0x12);
    ret |= gc2053_write_reg(dev,0x8d ,0x92);
    ret |= gc2053_write_reg(dev,0x90 ,0x00);
    ret |= gc2053_write_reg(dev,0x41 ,0x04);//Frame length c35
    ret |= gc2053_write_reg(dev,0x42 ,0x65);
    ret |= gc2053_write_reg(dev,0x9d ,0x10);
    ret |= gc2053_write_reg(dev,0xce ,0x7c);
    ret |= gc2053_write_reg(dev,0xd2 ,0x41);
    ret |= gc2053_write_reg(dev,0xd3 ,0xdc);
    ret |= gc2053_write_reg(dev,0xe6 ,0x50);
    ret |= gc2053_write_reg(dev,0xb6 ,0xc0);
    ret |= gc2053_write_reg(dev,0xb0 ,0x70);
    ret |= gc2053_write_reg(dev,0xb1 ,0x01);
    ret |= gc2053_write_reg(dev,0xb2 ,0x00);
    ret |= gc2053_write_reg(dev,0xb3 ,0x00);
    ret |= gc2053_write_reg(dev,0xb4 ,0x00);
    ret |= gc2053_write_reg(dev,0xb8 ,0x01);
    ret |= gc2053_write_reg(dev,0xb9 ,0x00);
    ret |= gc2053_write_reg(dev,0x26 ,0x30);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x40 ,0x23);
    ret |= gc2053_write_reg(dev,0x55 ,0x07);
    ret |= gc2053_write_reg(dev,0x60 ,0x40);
    ret |= gc2053_write_reg(dev,0xfe ,0x04);
    ret |= gc2053_write_reg(dev,0x14 ,0x78);
    ret |= gc2053_write_reg(dev,0x15 ,0x78);
    ret |= gc2053_write_reg(dev,0x16 ,0x78);
    ret |= gc2053_write_reg(dev,0x17 ,0x78);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x92 ,0x00);
    ret |= gc2053_write_reg(dev,0x94 ,0x03);
    ret |= gc2053_write_reg(dev,0x95 ,0x04);//out_window
    ret |= gc2053_write_reg(dev,0x96 ,0x38);
    ret |= gc2053_write_reg(dev,0x97 ,0x07);
    ret |= gc2053_write_reg(dev,0x98 ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x01 ,0x05);
    ret |= gc2053_write_reg(dev,0x02 ,0x89);
    ret |= gc2053_write_reg(dev,0x04 ,0x01);
    ret |= gc2053_write_reg(dev,0x07 ,0xa6);
    ret |= gc2053_write_reg(dev,0x08 ,0xa9);
    ret |= gc2053_write_reg(dev,0x09 ,0xa8);
    ret |= gc2053_write_reg(dev,0x0a ,0xa7);
    ret |= gc2053_write_reg(dev,0x0b ,0xff);
    ret |= gc2053_write_reg(dev,0x0c ,0xff);
    ret |= gc2053_write_reg(dev,0x0f ,0x00);
    ret |= gc2053_write_reg(dev,0x50 ,0x1c);
    ret |= gc2053_write_reg(dev,0x89 ,0x03);
    ret |= gc2053_write_reg(dev,0xfe ,0x04);
    ret |= gc2053_write_reg(dev,0x28 ,0x86);
    ret |= gc2053_write_reg(dev,0x29 ,0x86);
    ret |= gc2053_write_reg(dev,0x2a ,0x86);
    ret |= gc2053_write_reg(dev,0x2b ,0x68);
    ret |= gc2053_write_reg(dev,0x2c ,0x68);
    ret |= gc2053_write_reg(dev,0x2d ,0x68);
    ret |= gc2053_write_reg(dev,0x2e ,0x68);
    ret |= gc2053_write_reg(dev,0x2f ,0x68);
    ret |= gc2053_write_reg(dev,0x30 ,0x4f);
    ret |= gc2053_write_reg(dev,0x31 ,0x68);
    ret |= gc2053_write_reg(dev,0x32 ,0x67);
    ret |= gc2053_write_reg(dev,0x33 ,0x66);
    ret |= gc2053_write_reg(dev,0x34 ,0x66);
    ret |= gc2053_write_reg(dev,0x35 ,0x66);
    ret |= gc2053_write_reg(dev,0x36 ,0x66);
    ret |= gc2053_write_reg(dev,0x37 ,0x66);
    ret |= gc2053_write_reg(dev,0x38 ,0x62);
    ret |= gc2053_write_reg(dev,0x39 ,0x62);
    ret |= gc2053_write_reg(dev,0x3a ,0x62);
    ret |= gc2053_write_reg(dev,0x3b ,0x62);
    ret |= gc2053_write_reg(dev,0x3c ,0x62);
    ret |= gc2053_write_reg(dev,0x3d ,0x62);
    ret |= gc2053_write_reg(dev,0x3e ,0x62);
    ret |= gc2053_write_reg(dev,0x3f ,0x62);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x9a ,0x06);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0x7b ,0x2a);
    ret |= gc2053_write_reg(dev,0x23 ,0x2d);
    ret |= gc2053_write_reg(dev,0xfe ,0x03);
    ret |= gc2053_write_reg(dev,0x01 ,0x20);
    ret |= gc2053_write_reg(dev,0x02 ,0x56);
    ret |= gc2053_write_reg(dev,0x03 ,0xb2);
    ret |= gc2053_write_reg(dev,0x12 ,0x80);
    ret |= gc2053_write_reg(dev,0x13 ,0x07);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0x3e ,0x40);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2053_write_reg failed\n");
        return XMEDIA_FAILURE;

    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("==========Galaxycore GC2053_1080P_6.6FPS_10BIT_LINEAR_Init_OK!!!!================\n");
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}
#else
static xmedia_s32 gc2053_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    SENSOR_PRINT("========================================================================================\n");
    SENSOR_PRINT("==================================== GC2053 LINEAR NOT SUPPORT! ========================\n");
    SENSOR_PRINT("========================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}
#endif

#else
#ifdef GC2053_DVP
static xmedia_s32 gc2053_dvp_linear_1920X1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc2053_write_reg(dev,0xfe ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0xf2 ,0x00);
    ret |= gc2053_write_reg(dev,0xf3 ,0x0f);
    ret |= gc2053_write_reg(dev,0xf4 ,0x36);
    ret |= gc2053_write_reg(dev,0xf5 ,0xc0);
    ret |= gc2053_write_reg(dev,0xf6 ,0x44);
    ret |= gc2053_write_reg(dev,0xf7 ,0x01);
    ret |= gc2053_write_reg(dev,0xf8 ,0x2c);
    ret |= gc2053_write_reg(dev,0xf9 ,0x42);
    ret |= gc2053_write_reg(dev,0xfc ,0x8e);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0x87 ,0x18);
    ret |= gc2053_write_reg(dev,0xee ,0x30);
    ret |= gc2053_write_reg(dev,0xd0 ,0xb7);
    ret |= gc2053_write_reg(dev,0x03 ,0x04);
    ret |= gc2053_write_reg(dev,0x04 ,0x60);
    ret |= gc2053_write_reg(dev,0x05 ,0x05); //line length
    ret |= gc2053_write_reg(dev,0x06 ,0x28);
    ret |= gc2053_write_reg(dev,0x07 ,0x00);//VBlank
    ret |= gc2053_write_reg(dev,0x08 ,0x11);
    ret |= gc2053_write_reg(dev,0x09 ,0x00);
    ret |= gc2053_write_reg(dev,0x0a ,0x02);
    ret |= gc2053_write_reg(dev,0x0b ,0x00);
    ret |= gc2053_write_reg(dev,0x0c ,0x02);
    ret |= gc2053_write_reg(dev,0x0d ,0x04);//valid line time
    ret |= gc2053_write_reg(dev,0x0e ,0x40);
    ret |= gc2053_write_reg(dev,0x12 ,0xe2);
    ret |= gc2053_write_reg(dev,0x13 ,0x16);
    ret |= gc2053_write_reg(dev,0x19 ,0x0a);
    ret |= gc2053_write_reg(dev,0x21 ,0x1c);
    ret |= gc2053_write_reg(dev,0x28 ,0x0a);
    ret |= gc2053_write_reg(dev,0x29 ,0x24);
    ret |= gc2053_write_reg(dev,0x2b ,0x04);
    ret |= gc2053_write_reg(dev,0x32 ,0xf8);
    ret |= gc2053_write_reg(dev,0x37 ,0x03);
    ret |= gc2053_write_reg(dev,0x39 ,0x15);
    ret |= gc2053_write_reg(dev,0x43 ,0x07);
    ret |= gc2053_write_reg(dev,0x44 ,0x40);
    ret |= gc2053_write_reg(dev,0x46 ,0x0b);
    ret |= gc2053_write_reg(dev,0x4b ,0x20);
    ret |= gc2053_write_reg(dev,0x4e ,0x08);
    ret |= gc2053_write_reg(dev,0x55 ,0x20);
    ret |= gc2053_write_reg(dev,0x66 ,0x05);
    ret |= gc2053_write_reg(dev,0x67 ,0x05);
    ret |= gc2053_write_reg(dev,0x77 ,0x01);
    ret |= gc2053_write_reg(dev,0x78 ,0x00);
    ret |= gc2053_write_reg(dev,0x7c ,0x93);
    ret |= gc2053_write_reg(dev,0x8c ,0x12);
    ret |= gc2053_write_reg(dev,0x8d ,0x92);
    ret |= gc2053_write_reg(dev,0x90 ,0x00);
    ret |= gc2053_write_reg(dev,0x41 ,0x04);
    ret |= gc2053_write_reg(dev,0x42 ,0x65);
    ret |= gc2053_write_reg(dev,0x9d ,0x10);
    ret |= gc2053_write_reg(dev,0xce ,0x7c);
    ret |= gc2053_write_reg(dev,0xd2 ,0x41);
    ret |= gc2053_write_reg(dev,0xd3 ,0xdc);
    ret |= gc2053_write_reg(dev,0xe6 ,0x50);
    ret |= gc2053_write_reg(dev,0xb6 ,0xc0);
    ret |= gc2053_write_reg(dev,0xb0 ,0x70);
    ret |= gc2053_write_reg(dev,0xb1 ,0x01);
    ret |= gc2053_write_reg(dev,0xb2 ,0x00);
    ret |= gc2053_write_reg(dev,0xb3 ,0x00);
    ret |= gc2053_write_reg(dev,0xb4 ,0x00);
    ret |= gc2053_write_reg(dev,0xb8 ,0x01);
    ret |= gc2053_write_reg(dev,0xb9 ,0x00);
    ret |= gc2053_write_reg(dev,0x26 ,0x30);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x40 ,0x23);
    ret |= gc2053_write_reg(dev,0x55 ,0x07);
    ret |= gc2053_write_reg(dev,0x60 ,0x40);
    ret |= gc2053_write_reg(dev,0xfe ,0x04);
    ret |= gc2053_write_reg(dev,0x14 ,0x78);
    ret |= gc2053_write_reg(dev,0x15 ,0x78);
    ret |= gc2053_write_reg(dev,0x16 ,0x78);
    ret |= gc2053_write_reg(dev,0x17 ,0x78);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x92 ,0x00);
    ret |= gc2053_write_reg(dev,0x94 ,0x03);
    ret |= gc2053_write_reg(dev,0x95 ,0x04);//out_window
    ret |= gc2053_write_reg(dev,0x96 ,0x38);
    ret |= gc2053_write_reg(dev,0x97 ,0x07);
    ret |= gc2053_write_reg(dev,0x98 ,0x80);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x01 ,0x05);
    ret |= gc2053_write_reg(dev,0x02 ,0x89);
    ret |= gc2053_write_reg(dev,0x04 ,0x01);
    ret |= gc2053_write_reg(dev,0x07 ,0xa6);
    ret |= gc2053_write_reg(dev,0x08 ,0xa9);
    ret |= gc2053_write_reg(dev,0x09 ,0xa8);
    ret |= gc2053_write_reg(dev,0x0a ,0xa7);
    ret |= gc2053_write_reg(dev,0x0b ,0xff);
    ret |= gc2053_write_reg(dev,0x0c ,0xff);
    ret |= gc2053_write_reg(dev,0x0f ,0x00);
    ret |= gc2053_write_reg(dev,0x50 ,0x1c);
    ret |= gc2053_write_reg(dev,0x89 ,0x03);
    ret |= gc2053_write_reg(dev,0xfe ,0x04);
    ret |= gc2053_write_reg(dev,0x28 ,0x86);
    ret |= gc2053_write_reg(dev,0x29 ,0x86);
    ret |= gc2053_write_reg(dev,0x2a ,0x86);
    ret |= gc2053_write_reg(dev,0x2b ,0x68);
    ret |= gc2053_write_reg(dev,0x2c ,0x68);
    ret |= gc2053_write_reg(dev,0x2d ,0x68);
    ret |= gc2053_write_reg(dev,0x2e ,0x68);
    ret |= gc2053_write_reg(dev,0x2f ,0x68);
    ret |= gc2053_write_reg(dev,0x30 ,0x4f);
    ret |= gc2053_write_reg(dev,0x31 ,0x68);
    ret |= gc2053_write_reg(dev,0x32 ,0x67);
    ret |= gc2053_write_reg(dev,0x33 ,0x66);
    ret |= gc2053_write_reg(dev,0x34 ,0x66);
    ret |= gc2053_write_reg(dev,0x35 ,0x66);
    ret |= gc2053_write_reg(dev,0x36 ,0x66);
    ret |= gc2053_write_reg(dev,0x37 ,0x66);
    ret |= gc2053_write_reg(dev,0x38 ,0x62);
    ret |= gc2053_write_reg(dev,0x39 ,0x62);
    ret |= gc2053_write_reg(dev,0x3a ,0x62);
    ret |= gc2053_write_reg(dev,0x3b ,0x62);
    ret |= gc2053_write_reg(dev,0x3c ,0x62);
    ret |= gc2053_write_reg(dev,0x3d ,0x62);
    ret |= gc2053_write_reg(dev,0x3e ,0x62);
    ret |= gc2053_write_reg(dev,0x3f ,0x62);
    ret |= gc2053_write_reg(dev,0xfe ,0x01);
    ret |= gc2053_write_reg(dev,0x9a ,0x06);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0x7b ,0x2a);
    ret |= gc2053_write_reg(dev,0x23 ,0x2d);
    ret |= gc2053_write_reg(dev,0xfe ,0x03);
    ret |= gc2053_write_reg(dev,0x01 ,0x20);
    ret |= gc2053_write_reg(dev,0x02 ,0x56);
    ret |= gc2053_write_reg(dev,0x03 ,0xb2);
    ret |= gc2053_write_reg(dev,0x12 ,0x80);
    ret |= gc2053_write_reg(dev,0x13 ,0x07);
    ret |= gc2053_write_reg(dev,0xfe ,0x00);
    ret |= gc2053_write_reg(dev,0x3e ,0x40);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2053_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("==========Galaxycore GC2053_1080P_25FPS_12BIT_LINEAR_Init_OK!!!!=================\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;
}

#else
SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc2053_write_reg(dev, 0xfe, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0xf2, 0x00);
    ret |= gc2053_write_reg(dev, 0xf3, 0x00);
    ret |= gc2053_write_reg(dev, 0xf4, 0x36);
    ret |= gc2053_write_reg(dev, 0xf5, 0xc0);
    ret |= gc2053_write_reg(dev, 0xf6, 0x84);
    ret |= gc2053_write_reg(dev, 0xf7, 0x11);
    ret |= gc2053_write_reg(dev, 0xf8, 0x3e);
    ret |= gc2053_write_reg(dev, 0xf9, 0x82);
    ret |= gc2053_write_reg(dev, 0xfc, 0x8e);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0x87, 0x18);
    ret |= gc2053_write_reg(dev, 0xee, 0x30);
    ret |= gc2053_write_reg(dev, 0xd0, 0xb7);
    ret |= gc2053_write_reg(dev, 0x03, 0x04);
    ret |= gc2053_write_reg(dev, 0x04, 0x60);
    ret |= gc2053_write_reg(dev, 0x05, 0x04);
    ret |= gc2053_write_reg(dev, 0x06, 0x4c);
    ret |= gc2053_write_reg(dev, 0x07, 0x00);
    ret |= gc2053_write_reg(dev, 0x08, 0x11);
    ret |= gc2053_write_reg(dev, 0x09, 0x00);
    ret |= gc2053_write_reg(dev, 0x0a, 0x02);
    ret |= gc2053_write_reg(dev, 0x0b, 0x00);
    ret |= gc2053_write_reg(dev, 0x0c, 0x02);
    ret |= gc2053_write_reg(dev, 0x0d, 0x04);
    ret |= gc2053_write_reg(dev, 0x0e, 0x40);
    ret |= gc2053_write_reg(dev, 0x12, 0xe2);
    ret |= gc2053_write_reg(dev, 0x13, 0x16);
    ret |= gc2053_write_reg(dev, 0x19, 0x0a);
    ret |= gc2053_write_reg(dev, 0x21, 0x1c);
    ret |= gc2053_write_reg(dev, 0x28, 0x0a);
    ret |= gc2053_write_reg(dev, 0x29, 0x24);
    ret |= gc2053_write_reg(dev, 0x2b, 0x04);
    ret |= gc2053_write_reg(dev, 0x32, 0xf8);
    ret |= gc2053_write_reg(dev, 0x37, 0x03);
    ret |= gc2053_write_reg(dev, 0x39, 0x15);
    ret |= gc2053_write_reg(dev, 0x43, 0x07);
    ret |= gc2053_write_reg(dev, 0x44, 0x40);
    ret |= gc2053_write_reg(dev, 0x46, 0x0b);
    ret |= gc2053_write_reg(dev, 0x4b, 0x20);
    ret |= gc2053_write_reg(dev, 0x4e, 0x08);
    ret |= gc2053_write_reg(dev, 0x55, 0x20);
    ret |= gc2053_write_reg(dev, 0x66, 0x05);
    ret |= gc2053_write_reg(dev, 0x67, 0x05);
    ret |= gc2053_write_reg(dev, 0x77, 0x01);
    ret |= gc2053_write_reg(dev, 0x78, 0x00);
    ret |= gc2053_write_reg(dev, 0x7c, 0x93);
    ret |= gc2053_write_reg(dev, 0x8c, 0x12);
    ret |= gc2053_write_reg(dev, 0x8d, 0x92);
    ret |= gc2053_write_reg(dev, 0x90, 0x00);
    ret |= gc2053_write_reg(dev, 0x41, 0x04);
    ret |= gc2053_write_reg(dev, 0x42, 0x65);
    ret |= gc2053_write_reg(dev, 0x9d, 0x10);
    ret |= gc2053_write_reg(dev, 0xce, 0x7c);
    ret |= gc2053_write_reg(dev, 0xd2, 0x41);
    ret |= gc2053_write_reg(dev, 0xd3, 0xdc);
    ret |= gc2053_write_reg(dev, 0xe6, 0x50);
    ret |= gc2053_write_reg(dev, 0xb6, 0xc0);
    ret |= gc2053_write_reg(dev, 0xb0, 0x70);
    ret |= gc2053_write_reg(dev, 0xb1, 0x01);
    ret |= gc2053_write_reg(dev, 0xb2, 0x00);
    ret |= gc2053_write_reg(dev, 0xb3, 0x00);
    ret |= gc2053_write_reg(dev, 0xb4, 0x00);
    ret |= gc2053_write_reg(dev, 0xb8, 0x01);
    ret |= gc2053_write_reg(dev, 0xb9, 0x00);
    ret |= gc2053_write_reg(dev, 0x26, 0x30);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x40, 0x23);
    ret |= gc2053_write_reg(dev, 0x55, 0x07);
    ret |= gc2053_write_reg(dev, 0x60, 0x40);
    ret |= gc2053_write_reg(dev, 0xfe, 0x04);
    ret |= gc2053_write_reg(dev, 0x14, 0x78);
    ret |= gc2053_write_reg(dev, 0x15, 0x78);
    ret |= gc2053_write_reg(dev, 0x16, 0x78);
    ret |= gc2053_write_reg(dev, 0x17, 0x78);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x92, 0x01);
    ret |= gc2053_write_reg(dev, 0x94, 0x04);
    ret |= gc2053_write_reg(dev, 0x95, 0x04);
    ret |= gc2053_write_reg(dev, 0x96, 0x38);
    ret |= gc2053_write_reg(dev, 0x97, 0x07);
    ret |= gc2053_write_reg(dev, 0x98, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x83, 0x01);
    ret |= gc2053_write_reg(dev, 0x87, 0x50);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x01, 0x05);
    ret |= gc2053_write_reg(dev, 0x02, 0x89);
    ret |= gc2053_write_reg(dev, 0x04, 0x00);
    ret |= gc2053_write_reg(dev, 0x07, 0xa6);
    ret |= gc2053_write_reg(dev, 0x08, 0xa9);
    ret |= gc2053_write_reg(dev, 0x09, 0xa8);
    ret |= gc2053_write_reg(dev, 0x0a, 0xa7);
    ret |= gc2053_write_reg(dev, 0x0b, 0xff);
    ret |= gc2053_write_reg(dev, 0x0c, 0xff);
    ret |= gc2053_write_reg(dev, 0x0f, 0x00);
    ret |= gc2053_write_reg(dev, 0x50, 0x1c);
    ret |= gc2053_write_reg(dev, 0x89, 0x03);
    ret |= gc2053_write_reg(dev, 0xfe, 0x04);
    ret |= gc2053_write_reg(dev, 0x28, 0x86);
    ret |= gc2053_write_reg(dev, 0x29, 0x86);
    ret |= gc2053_write_reg(dev, 0x2a, 0x86);
    ret |= gc2053_write_reg(dev, 0x2b, 0x68);
    ret |= gc2053_write_reg(dev, 0x2c, 0x68);
    ret |= gc2053_write_reg(dev, 0x2d, 0x68);
    ret |= gc2053_write_reg(dev, 0x2e, 0x68);
    ret |= gc2053_write_reg(dev, 0x2f, 0x68);
    ret |= gc2053_write_reg(dev, 0x30, 0x4f);
    ret |= gc2053_write_reg(dev, 0x31, 0x68);
    ret |= gc2053_write_reg(dev, 0x32, 0x67);
    ret |= gc2053_write_reg(dev, 0x33, 0x66);
    ret |= gc2053_write_reg(dev, 0x34, 0x66);
    ret |= gc2053_write_reg(dev, 0x35, 0x66);
    ret |= gc2053_write_reg(dev, 0x36, 0x66);
    ret |= gc2053_write_reg(dev, 0x37, 0x66);
    ret |= gc2053_write_reg(dev, 0x38, 0x62);
    ret |= gc2053_write_reg(dev, 0x39, 0x62);
    ret |= gc2053_write_reg(dev, 0x3a, 0x62);
    ret |= gc2053_write_reg(dev, 0x3b, 0x62);
    ret |= gc2053_write_reg(dev, 0x3c, 0x62);
    ret |= gc2053_write_reg(dev, 0x3d, 0x62);
    ret |= gc2053_write_reg(dev, 0x3e, 0x62);
    ret |= gc2053_write_reg(dev, 0x3f, 0x62);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x9a, 0x06);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0x7b, 0x2a);
    ret |= gc2053_write_reg(dev, 0x23, 0x2d);
    ret |= gc2053_write_reg(dev, 0xfe, 0x03);
    ret |= gc2053_write_reg(dev, 0x01, 0x27);
    ret |= gc2053_write_reg(dev, 0x02, 0x56);
    ret |= gc2053_write_reg(dev, 0x03, 0x8e);
    ret |= gc2053_write_reg(dev, 0x12, 0x80);
    ret |= gc2053_write_reg(dev, 0x13, 0x07);
    ret |= gc2053_write_reg(dev, 0x15, 0x10);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0x3e, 0x91);

/* 76上可运行的版本，72上不能正常出图
    ret |= gc2053_write_reg(dev, 0xfe, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0xf2, 0x00);
    ret |= gc2053_write_reg(dev, 0xf3, 0x00);
    ret |= gc2053_write_reg(dev, 0xf4, 0x36);
    ret |= gc2053_write_reg(dev, 0xf5, 0xc0);
    ret |= gc2053_write_reg(dev, 0xf6, 0x44);
    ret |= gc2053_write_reg(dev, 0xf7, 0x01);
    ret |= gc2053_write_reg(dev, 0xf8, 0x2c);
    ret |= gc2053_write_reg(dev, 0xf9, 0x42);
    ret |= gc2053_write_reg(dev, 0xfc, 0x8e);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0x87, 0x18);
    ret |= gc2053_write_reg(dev, 0xee, 0x30);
    ret |= gc2053_write_reg(dev, 0xd0, 0xb7);
    ret |= gc2053_write_reg(dev, 0x03, 0x04);
    ret |= gc2053_write_reg(dev, 0x04, 0x60);
    ret |= gc2053_write_reg(dev, 0x05, 0x04);//line length
    ret |= gc2053_write_reg(dev, 0x06, 0x4c);
    ret |= gc2053_write_reg(dev, 0x07, 0x00);
    ret |= gc2053_write_reg(dev, 0x08, 0x11);
    ret |= gc2053_write_reg(dev, 0x0a, 0x02);
    ret |= gc2053_write_reg(dev, 0x0c, 0x02);
    ret |= gc2053_write_reg(dev, 0x0d, 0x04);//win_height
    ret |= gc2053_write_reg(dev, 0x0e, 0x40);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x83, 0x01);
    ret |= gc2053_write_reg(dev, 0x87, 0x51);
    ret |= gc2053_write_reg(dev, 0xfe, 0x03);
    ret |= gc2053_write_reg(dev, 0x01, 0x27);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0x3e, 0x91);
    ret |= gc2053_write_reg(dev, 0x12, 0xe2);
    ret |= gc2053_write_reg(dev, 0x13, 0x16);
    ret |= gc2053_write_reg(dev, 0x19, 0x0a);
    ret |= gc2053_write_reg(dev, 0x28, 0x0a);
    ret |= gc2053_write_reg(dev, 0x2b, 0x04);
    ret |= gc2053_write_reg(dev, 0x37, 0x03);
    ret |= gc2053_write_reg(dev, 0x43, 0x07);
    ret |= gc2053_write_reg(dev, 0x44, 0x40);
    ret |= gc2053_write_reg(dev, 0x46, 0x0b);
    ret |= gc2053_write_reg(dev, 0x4b, 0x20);
    ret |= gc2053_write_reg(dev, 0x4e, 0x08);
    ret |= gc2053_write_reg(dev, 0x55, 0x20);
    ret |= gc2053_write_reg(dev, 0x77, 0x01);
    ret |= gc2053_write_reg(dev, 0x78, 0x00);
    ret |= gc2053_write_reg(dev, 0x7c, 0x93);
    ret |= gc2053_write_reg(dev, 0x8d, 0x92);
    ret |= gc2053_write_reg(dev, 0x90, 0x00);
    ret |= gc2053_write_reg(dev, 0x41, 0x04);
    ret |= gc2053_write_reg(dev, 0x42, 0x65);
    ret |= gc2053_write_reg(dev, 0xce, 0x7c);
    ret |= gc2053_write_reg(dev, 0xd2, 0x41);
    ret |= gc2053_write_reg(dev, 0xd3, 0xdc);
    ret |= gc2053_write_reg(dev, 0xe6, 0x50);
    ret |= gc2053_write_reg(dev, 0xb6, 0xc0);
    ret |= gc2053_write_reg(dev, 0xb0, 0x70);
    ret |= gc2053_write_reg(dev, 0x26, 0x30);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x55, 0x07);
    //dither
    ret |= gc2053_write_reg(dev, 0x58, 0x00); //default 0x80
    ret |= gc2053_write_reg(dev, 0x04, 0x00);
    ret |= gc2053_write_reg(dev, 0x94, 0x03);
    ret |= gc2053_write_reg(dev, 0x97, 0x07); //out_x_size
    ret |= gc2053_write_reg(dev, 0x98, 0x80);
    ret |= gc2053_write_reg(dev, 0xfe, 0x04);
    ret |= gc2053_write_reg(dev, 0x14, 0x78);
    ret |= gc2053_write_reg(dev, 0x15, 0x78);
    ret |= gc2053_write_reg(dev, 0x16, 0x78);
    ret |= gc2053_write_reg(dev, 0x17, 0x78);
    ret |= gc2053_write_reg(dev, 0xfe, 0x01);
    ret |= gc2053_write_reg(dev, 0x9a, 0x06);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
    ret |= gc2053_write_reg(dev, 0x7b, 0x2a);
    ret |= gc2053_write_reg(dev, 0x23, 0x2d);
    ret |= gc2053_write_reg(dev, 0xfe, 0x03);
    ret |= gc2053_write_reg(dev, 0x02, 0x56);
    ret |= gc2053_write_reg(dev, 0x03, 0xb6);
    ret |= gc2053_write_reg(dev, 0x12, 0x80);
    ret |= gc2053_write_reg(dev, 0x13, 0x07);
    ret |= gc2053_write_reg(dev, 0x15, 0x12);
    ret |= gc2053_write_reg(dev, 0xfe, 0x00);
*/

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2053_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("==========Galaxycore GC2053_1080P_30FPS_10BIT_LINEAR_Init_OK!====================\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;
}

#if 0
static xmedia_s32 gc2053_2lane_wdr_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc2053_write_reg(dev,0xfe, 0x80);
    ret |= gc2053_write_reg(dev,0xfe, 0x80);
    ret |= gc2053_write_reg(dev,0xfe, 0x80);
    ret |= gc2053_write_reg(dev,0xfe, 0x00);
    ret |= gc2053_write_reg(dev,0xf2, 0x00);
    ret |= gc2053_write_reg(dev,0xf3, 0x00);
    ret |= gc2053_write_reg(dev,0xf4, 0x36);
    ret |= gc2053_write_reg(dev,0xf5, 0xc0);
    ret |= gc2053_write_reg(dev,0xf6, 0x44);
    ret |= gc2053_write_reg(dev,0xf7, 0x01);
    ret |= gc2053_write_reg(dev,0xf8, 0x2c);
    ret |= gc2053_write_reg(dev,0xf9, 0x42);
    ret |= gc2053_write_reg(dev,0xfc, 0x8e);
    ret |= gc2053_write_reg(dev,0xfe, 0x00);
    ret |= gc2053_write_reg(dev,0x87, 0x18);
    ret |= gc2053_write_reg(dev,0xee, 0x30);
    ret |= gc2053_write_reg(dev,0xd0, 0xb7);
    ret |= gc2053_write_reg(dev,0x03, 0x04);
    ret |= gc2053_write_reg(dev,0x04, 0x60);
    ret |= gc2053_write_reg(dev,0x05, 0x04);
    ret |= gc2053_write_reg(dev,0x06, 0x4c);
    ret |= gc2053_write_reg(dev,0x07, 0x00);
    ret |= gc2053_write_reg(dev,0x08, 0x11);
    ret |= gc2053_write_reg(dev,0x0a, 0x02);
    ret |= gc2053_write_reg(dev,0x0c, 0x02);

    ret |= gc2053_write_reg(dev,0x0d, 0x04);//win_height
    ret |= gc2053_write_reg(dev,0x0e, 0x40);
    ret |= gc2053_write_reg(dev,0x12, 0xe2);
    ret |= gc2053_write_reg(dev,0x13, 0x16);
    ret |= gc2053_write_reg(dev,0x19, 0x0a);
    ret |= gc2053_write_reg(dev,0x28, 0x0a);
    ret |= gc2053_write_reg(dev,0x2b, 0x04);
    ret |= gc2053_write_reg(dev,0x37, 0x03);
    ret |= gc2053_write_reg(dev,0x43, 0x07);
    ret |= gc2053_write_reg(dev,0x44, 0x40);
    ret |= gc2053_write_reg(dev,0x46, 0x0b);
    ret |= gc2053_write_reg(dev,0x4b, 0x20);
    ret |= gc2053_write_reg(dev,0x4e, 0x08);
    ret |= gc2053_write_reg(dev,0x55, 0x20);
    ret |= gc2053_write_reg(dev,0x77, 0x01);
    ret |= gc2053_write_reg(dev,0x78, 0x00);
    ret |= gc2053_write_reg(dev,0x7c, 0x93);
    ret |= gc2053_write_reg(dev,0x8d, 0x92);
    ret |= gc2053_write_reg(dev,0x90, 0x00);

    ret |= gc2053_write_reg(dev,0x41, 0x04);//VTS
    ret |= gc2053_write_reg(dev,0x42, 0x65);
    ret |= gc2053_write_reg(dev,0xce, 0x7c);
    ret |= gc2053_write_reg(dev,0xd2, 0x41);
    ret |= gc2053_write_reg(dev,0xd3, 0xdc);
    ret |= gc2053_write_reg(dev,0xe6, 0x38);
    ret |= gc2053_write_reg(dev,0xb6, 0xc0);
    ret |= gc2053_write_reg(dev,0xb0, 0x70);
    ret |= gc2053_write_reg(dev,0x26, 0x20);
    ret |= gc2053_write_reg(dev,0xfe, 0x04);
    ret |= gc2053_write_reg(dev,0xe0, 0x18);
    ret |= gc2053_write_reg(dev,0xfe, 0x01);
    ret |= gc2053_write_reg(dev,0x55, 0x07);
    ret |= gc2053_write_reg(dev,0x58, 0x00);
    ret |= gc2053_write_reg(dev,0xfe, 0x04);
    ret |= gc2053_write_reg(dev,0x14, 0x78);
    ret |= gc2053_write_reg(dev,0x15, 0x78);
    ret |= gc2053_write_reg(dev,0x16, 0x78);
    ret |= gc2053_write_reg(dev,0x17, 0x78);
    ret |= gc2053_write_reg(dev,0xfe, 0x01);
    ret |= gc2053_write_reg(dev,0x04, 0x00);
    ret |= gc2053_write_reg(dev,0x94, 0x03);
    ret |= gc2053_write_reg(dev,0x97, 0x07);
    ret |= gc2053_write_reg(dev,0x98, 0x80);
    ret |= gc2053_write_reg(dev,0x9a, 0x06);
    ret |= gc2053_write_reg(dev,0x40, 0x03);
    ret |= gc2053_write_reg(dev,0x42, 0x20);
    ret |= gc2053_write_reg(dev,0x46, 0x4f);
    ret |= gc2053_write_reg(dev,0x47, 0x40);
    ret |= gc2053_write_reg(dev,0x4f, 0x02);
    ret |= gc2053_write_reg(dev,0x50, 0x3a);
    ret |= gc2053_write_reg(dev,0x51, 0x1d);
    ret |= gc2053_write_reg(dev,0xfe, 0x00);
    ret |= gc2053_write_reg(dev,0x7b, 0x2a);
    ret |= gc2053_write_reg(dev,0x23, 0x2d);
    ret |= gc2053_write_reg(dev,0xfe, 0x03);
    ret |= gc2053_write_reg(dev,0x01, 0x27);
    ret |= gc2053_write_reg(dev,0x02, 0x56);
    ret |= gc2053_write_reg(dev,0x03, 0xb6);
    ret |= gc2053_write_reg(dev,0x12, 0x80);
    ret |= gc2053_write_reg(dev,0x13, 0x07);
    ret |= gc2053_write_reg(dev,0x15, 0x12);
    ret |= gc2053_write_reg(dev,0xfe, 0x00);
    ret |= gc2053_write_reg(dev,0x3e, 0x91);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2053_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("==========Galaxycore GC2053_1080P_30FPS_10BIT_WDR_Init_OK!=======================\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;
}
#endif
#endif
#endif

static xmedia_void gc2053_slave_init(xmedia_u32 dev)
{
//    gc2053_write_reg(dev, 0xfe, 0x00);
//    gc2053_write_reg(dev, 0x7f, 0x9d);
//    gc2053_write_reg(dev, 0x82, 0x0a);
//    gc2053_write_reg(dev, 0x83, 0x0b);
//    gc2053_write_reg(dev, 0x84, 0x80);
//    gc2053_write_reg(dev, 0x85, 0x51);
//    gc2053_write_reg(dev, 0x86, 0x00);
//    gc2053_write_reg(dev, 0x87, 0x13);
//    gc2053_write_reg(dev, 0x88, 0x00);
//    gc2053_write_reg(dev, 0x89, 0x04);
//    gc2053_write_reg(dev, 0x8a, 0x00);
//    gc2053_write_reg(dev, 0x8b, 0x12);
//    gc2053_write_reg(dev, 0xfe, 0x00);

    gc2053_write_reg(dev, 0xfe, 0x00);
    gc2053_write_reg(dev, 0x7f, 0x09);
    gc2053_write_reg(dev, 0x82, 0x0a);
    gc2053_write_reg(dev, 0x83, 0x0b);
    gc2053_write_reg(dev, 0x84, 0x80);
    gc2053_write_reg(dev, 0x85, 0x51);

    SENSOR_PRINT("====== gc2053 slave setting init success!=======\n");
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC2053_2M_10BIT_LINEAR_MODE:
#ifdef GC2053_DVP
            ret = gc2053_dvp_linear_1920X1080_init(dev);
            break;
#else
            ret = gc2053_2lane_linear_1920x1080_init(dev);
            break;
#endif
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE && image_mode == GC2053_2M_10BIT_LINEAR_MODE) {
        gc2053_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc2053_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc2053_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc2053_i2c_fd[dev]);
        g_gc2053_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc2053_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32             ret;

    if (g_gc2053_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc2053_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc2053_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc2053_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc2053_i2c_fd[dev]);
    if(ret < 0 ){
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc2053_i2c_fd[dev] = SENSOR_I2C_INVALID;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_read_reg(xmedia_u32 dev, xmedia_u32 addr,xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC2053_DATA_BYTE];

    if (g_gc2053_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc2053_i2c_fd[dev], g_gc2053_i2c_addr[dev], addr, GC2053_ADDR_BYTE, buf, GC2053_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC2053_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}


SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_s32   idx = 0;
    xmedia_u8    buf[GC2053_ADDR_BYTE + GC2053_DATA_BYTE];
    xmedia_s32   ret = 0;

    if (g_gc2053_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not open!\n", dev);
        return XMEDIA_FAILURE;
    }

//    if (GC2053_ADDR_BYTE == 2) {
//        buf[idx] = (addr >> 8) & 0xff;
//        idx++;
//        buf[idx] = addr & 0xff;
//        idx++;
//    } else {
        buf[idx] = addr & 0xFF; //GC2053_ADDR_BYTE == 1
        idx++;
//    }

//    if (GC2053_DATA_BYTE == 2) {
//        buf[idx] = (data >> 8) & 0xff;
//        idx++;
//        buf[idx] = data & 0xff;
//        idx++;
//    } else {
        buf[idx] = data & 0xFF;  //GC2053_DATA_BYTE == 1
//        idx++;
//    }

    ret = i2c_write(g_gc2053_i2c_fd[dev], g_gc2053_i2c_addr[dev], buf, GC2053_ADDR_BYTE + GC2053_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    gc2053_read_reg(dev, GC2053_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x01;
    } else {
        mirror_flip_type &= 0xFE;
    }

    if (flip_en) {
        mirror_flip_type |= 0x02;
    } else {
        mirror_flip_type &= 0xFD;
    }

    ret = gc2053_write_reg(dev, GC2053_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
