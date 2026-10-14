#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc20c3.h"
#include "gc20c3_ctrl.h"

static xmedia_s32 g_gc20c3_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_gc20c3_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define GC20C3_REG_ADDR_MIRROR_FLIP 0x022C

#if GC20C3_2LANE_LINEAR_1920X1080_30FPS
static xmedia_s32 gc20c3_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    // mipi_rate = 540 Mbps/lane
    // wpclk = 135 Mhz
    // rpclk = 108 Mhz
    // FL = 1125
    ret |= gc20c3_write_reg(dev, 0x03fe, 0xff);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x10);
    ret |= gc20c3_write_reg(dev, 0x0190, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0b4d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0d40, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0087, 0x50);
    ret |= gc20c3_write_reg(dev, 0x0209, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03b5, 0x11);
    ret |= gc20c3_write_reg(dev, 0x031c, 0x18);
    ret |= gc20c3_write_reg(dev, 0x03b2, 0x03);
    ret |= gc20c3_write_reg(dev, 0x03bb, 0xff);
    ret |= gc20c3_write_reg(dev, 0x03be, 0xff);
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0d11, 0x0b);
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0d1a, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0d15, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0d12, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0d16, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0d17, 0x78);
    ret |= gc20c3_write_reg(dev, 0x0d18, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0d19, 0x48);
    ret |= gc20c3_write_reg(dev, 0x0145, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x0144, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0142, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0143, 0x14);
    ret |= gc20c3_write_reg(dev, 0x0146, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0141, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0149, 0x05);
    ret |= gc20c3_write_reg(dev, 0x014a, 0x07);
    ret |= gc20c3_write_reg(dev, 0x014b, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0b4e, 0x88);
    ret |= gc20c3_write_reg(dev, 0x0e0c, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0e0f, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0e3a, 0x98);
    ret |= gc20c3_write_reg(dev, 0x0b45, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0b47, 0xf0);
    ret |= gc20c3_write_reg(dev, 0x0d30, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0d2f, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0b40, 0x57);
    ret |= gc20c3_write_reg(dev, 0x0b43, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0b41, 0x0c);
    ret |= gc20c3_write_reg(dev, 0x0d31, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0c23, 0x40);
    ret |= gc20c3_write_reg(dev, 0x0e23, 0x1a);
    ret |= gc20c3_write_reg(dev, 0x0e2a, 0x09);
    ret |= gc20c3_write_reg(dev, 0x0e2b, 0xc9);
    ret |= gc20c3_write_reg(dev, 0x0e41, 0x87);
    ret |= gc20c3_write_reg(dev, 0x0e3b, 0xb5);
    ret |= gc20c3_write_reg(dev, 0x0e3a, 0x15);
    ret |= gc20c3_write_reg(dev, 0x0e37, 0x1f);
    ret |= gc20c3_write_reg(dev, 0x0217, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0213, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0219, 0xc2);
    ret |= gc20c3_write_reg(dev, 0x0259, 0x04);
    ret |= gc20c3_write_reg(dev, 0x025a, 0x5e);
    ret |= gc20c3_write_reg(dev, 0x0211, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0340, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0341, 0x65);
    ret |= gc20c3_write_reg(dev, 0x0342, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0343, 0xe8);
    ret |= gc20c3_write_reg(dev, 0x0212, 0x14);
    ret |= gc20c3_write_reg(dev, 0x0350, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0348, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0349, 0x88);
    ret |= gc20c3_write_reg(dev, 0x034a, 0x04);
    ret |= gc20c3_write_reg(dev, 0x034b, 0x40);
    ret |= gc20c3_write_reg(dev, 0x0347, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0b0c, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0b0d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0b0e, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0b0f, 0x8a);
    ret |= gc20c3_write_reg(dev, 0x034e, 0x07);
    ret |= gc20c3_write_reg(dev, 0x034f, 0xa8);
    ret |= gc20c3_write_reg(dev, 0x0004, 0x0f);
    ret |= gc20c3_write_reg(dev, 0x0444, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0038, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0039, 0x20);
    ret |= gc20c3_write_reg(dev, 0x003a, 0x20);
    ret |= gc20c3_write_reg(dev, 0x003b, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0492, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0493, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0070, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0094, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0095, 0x80);
    ret |= gc20c3_write_reg(dev, 0x0096, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0097, 0x38);
    ret |= gc20c3_write_reg(dev, 0x0099, 0x04);
    ret |= gc20c3_write_reg(dev, 0x009b, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0438, 0x0f);
    ret |= gc20c3_write_reg(dev, 0x0439, 0xf0);
    ret |= gc20c3_write_reg(dev, 0x021a, 0x10);
    ret |= gc20c3_write_reg(dev, 0x0476, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0430, 0x23);
    ret |= gc20c3_write_reg(dev, 0x0443, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0038, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0039, 0x00);
    ret |= gc20c3_write_reg(dev, 0x003a, 0x00);
    ret |= gc20c3_write_reg(dev, 0x003b, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0070, 0x80);
    ret |= gc20c3_write_reg(dev, 0x0448, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x0449, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x044a, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x044b, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x044c, 0x74);
    ret |= gc20c3_write_reg(dev, 0x044d, 0x74);
    ret |= gc20c3_write_reg(dev, 0x044e, 0x74);
    ret |= gc20c3_write_reg(dev, 0x044f, 0x74);
    ret |= gc20c3_write_reg(dev, 0x0485, 0x68);
    ret |= gc20c3_write_reg(dev, 0x0d38, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0d39, 0x57);
    ret |= gc20c3_write_reg(dev, 0x0e4e, 0xa9);
    ret |= gc20c3_write_reg(dev, 0x0072, 0x09);
    ret |= gc20c3_write_reg(dev, 0x0073, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0c20, 0x09);
    ret |= gc20c3_write_reg(dev, 0x0c1d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0c1e, 0x2c);
    ret |= gc20c3_write_reg(dev, 0x0c1f, 0xe6);
    ret |= gc20c3_write_reg(dev, 0x0c19, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0c1a, 0x11);
    ret |= gc20c3_write_reg(dev, 0x0c1b, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0c1c, 0x80);
    ret |= gc20c3_write_reg(dev, 0x0261, 0x13);
    ret |= gc20c3_write_reg(dev, 0x0004, 0x0f);
    ret |= gc20c3_write_reg(dev, 0x0052, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0053, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0055, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0152, 0x14);
    ret |= gc20c3_write_reg(dev, 0x0100, 0x03);
    ret |= gc20c3_write_reg(dev, 0x031c, 0x1f);
    ret |= gc20c3_write_reg(dev, 0x0336, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0336, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc20c3_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= GC20C3_27MInput_MIPI_2lane_10bit_1920x1080_30fps init success!==========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}
#else
static xmedia_s32 gc20c3_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    // 03_GC20C3_MIPI2L_27M_1920x1080_40fps_raw10_linear
    // RESOLUTION width="1920" height="1080"
    // BAYERMODE type= GrRBGb
    // mipi_rate = 783 Mbps/lane
    // vts = 1220,row time=18.214us,40fps
    ret |= gc20c3_write_reg(dev, 0x03fe, 0xff);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x10);
    ret |= gc20c3_write_reg(dev, 0x0190, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0b4d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0d40, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0087, 0x50);
    ret |= gc20c3_write_reg(dev, 0x0209, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03b5, 0x11);
    ret |= gc20c3_write_reg(dev, 0x031c, 0x18);
    ret |= gc20c3_write_reg(dev, 0x03b2, 0x03);
    ret |= gc20c3_write_reg(dev, 0x03bb, 0xff);
    ret |= gc20c3_write_reg(dev, 0x03be, 0xff);
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0d11, 0x0b);
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0d1a, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0d15, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0d12, 0x25);
    ret |= gc20c3_write_reg(dev, 0x0d16, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0d17, 0xc8); // 0xf4
    ret |= gc20c3_write_reg(dev, 0x0d18, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0d19, 0x48);
    ret |= gc20c3_write_reg(dev, 0x0145, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x0144, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0142, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0143, 0x1c);
    ret |= gc20c3_write_reg(dev, 0x0146, 0x08);
    ret |= gc20c3_write_reg(dev, 0x0141, 0x08);
    ret |= gc20c3_write_reg(dev, 0x0149, 0x07);
    ret |= gc20c3_write_reg(dev, 0x014a, 0x0b);
    ret |= gc20c3_write_reg(dev, 0x014b, 0x08);
    ret |= gc20c3_write_reg(dev, 0x0b4e, 0x88);
    ret |= gc20c3_write_reg(dev, 0x0e0c, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0e0f, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0e3a, 0x98);
    ret |= gc20c3_write_reg(dev, 0x0b45, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0b47, 0xf0);
    ret |= gc20c3_write_reg(dev, 0x0d30, 0x06);
    ret |= gc20c3_write_reg(dev, 0x0d2f, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0b40, 0x57);
    ret |= gc20c3_write_reg(dev, 0x0b43, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0b41, 0x0c);
    ret |= gc20c3_write_reg(dev, 0x0d31, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0c23, 0x40);
    ret |= gc20c3_write_reg(dev, 0x0e23, 0x1a);
    ret |= gc20c3_write_reg(dev, 0x0e2a, 0x09);
    ret |= gc20c3_write_reg(dev, 0x0e2b, 0xc9);
    ret |= gc20c3_write_reg(dev, 0x0e41, 0x87);
    ret |= gc20c3_write_reg(dev, 0x0e3b, 0xb5);
    ret |= gc20c3_write_reg(dev, 0x0e3a, 0x15);
    ret |= gc20c3_write_reg(dev, 0x0e37, 0x1f);
    ret |= gc20c3_write_reg(dev, 0x0217, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0212, 0x14);
    ret |= gc20c3_write_reg(dev, 0x0213, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0219, 0xc2);
    ret |= gc20c3_write_reg(dev, 0x0259, 0x04);
    ret |= gc20c3_write_reg(dev, 0x025a, 0x5e);
    ret |= gc20c3_write_reg(dev, 0x0211, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0340, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0341, 0x66);
    ret |= gc20c3_write_reg(dev, 0x0342, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0343, 0xa9);
    ret |= gc20c3_write_reg(dev, 0x0350, 0x14);
    ret |= gc20c3_write_reg(dev, 0x0348, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0349, 0x88);
    ret |= gc20c3_write_reg(dev, 0x034a, 0x04);
    ret |= gc20c3_write_reg(dev, 0x034b, 0x40);
    ret |= gc20c3_write_reg(dev, 0x0347, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0b0c, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0b0d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0b0e, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0b0f, 0x8a);
    ret |= gc20c3_write_reg(dev, 0x034e, 0x07);
    ret |= gc20c3_write_reg(dev, 0x034f, 0xa8);
    ret |= gc20c3_write_reg(dev, 0x0004, 0x0f);
    ret |= gc20c3_write_reg(dev, 0x0444, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0038, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0039, 0x20);
    ret |= gc20c3_write_reg(dev, 0x003a, 0x20);
    ret |= gc20c3_write_reg(dev, 0x003b, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0492, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0493, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0070, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0094, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0095, 0x80);
    ret |= gc20c3_write_reg(dev, 0x0096, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0097, 0x38);
    ret |= gc20c3_write_reg(dev, 0x0099, 0x04);
    ret |= gc20c3_write_reg(dev, 0x009b, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0438, 0x0f);
    ret |= gc20c3_write_reg(dev, 0x0439, 0xf0);
    ret |= gc20c3_write_reg(dev, 0x021a, 0x10);
    ret |= gc20c3_write_reg(dev, 0x0476, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0430, 0x23);
    ret |= gc20c3_write_reg(dev, 0x0443, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0038, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0039, 0x00);
    ret |= gc20c3_write_reg(dev, 0x003a, 0x00);
    ret |= gc20c3_write_reg(dev, 0x003b, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0070, 0x80);
    ret |= gc20c3_write_reg(dev, 0x0448, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x0449, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x044a, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x044b, 0x0d);
    ret |= gc20c3_write_reg(dev, 0x044c, 0x74);
    ret |= gc20c3_write_reg(dev, 0x044d, 0x74);
    ret |= gc20c3_write_reg(dev, 0x044e, 0x74);
    ret |= gc20c3_write_reg(dev, 0x044f, 0x74);
    ret |= gc20c3_write_reg(dev, 0x0485, 0x68);
    ret |= gc20c3_write_reg(dev, 0x0d38, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0d39, 0x57);
    ret |= gc20c3_write_reg(dev, 0x0e4e, 0xa9);
    ret |= gc20c3_write_reg(dev, 0x0072, 0x09);
    ret |= gc20c3_write_reg(dev, 0x0073, 0x05);
    ret |= gc20c3_write_reg(dev, 0x0c20, 0x09);
    ret |= gc20c3_write_reg(dev, 0x0c1d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x0c1e, 0x2c);
    ret |= gc20c3_write_reg(dev, 0x0c1f, 0xe6);
    ret |= gc20c3_write_reg(dev, 0x0c19, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0c1a, 0x11);
    ret |= gc20c3_write_reg(dev, 0x0c1b, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0c1c, 0x80);
    ret |= gc20c3_write_reg(dev, 0x0261, 0x13);
    ret |= gc20c3_write_reg(dev, 0x0004, 0x0f);
    ret |= gc20c3_write_reg(dev, 0x0052, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0053, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0055, 0x20);
    ret |= gc20c3_write_reg(dev, 0x0100, 0x03);
    ret |= gc20c3_write_reg(dev, 0x0152, 0x14);
    ret |= gc20c3_write_reg(dev, 0x031c, 0x1f);
    ret |= gc20c3_write_reg(dev, 0x0336, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0336, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc20c3_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================================\n");
        SENSOR_PRINT("=== gc20c3_normal_mipi_27Minput_2lane_783Mbps_10bit_linear_1920x1080_40fps init success! ===\n");
        SENSOR_PRINT("============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}
#endif

xmedia_s32 gc20c3_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC20C3_2M_1920X1080_LINEAR_MODE:
            ret = gc20c3_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20c3_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_gc20c3_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc20c3_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc20c3_i2c_fd[dev]);
        g_gc20c3_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc20c3_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20c3_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc20c3_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc20c3_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc20c3_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc20c3_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20c3_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc20c3_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc20c3_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20c3_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC20C3_DATA_BYTE];

    if (g_gc20c3_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc20c3_i2c_fd[dev], g_gc20c3_i2c_addr[dev], addr, GC20C3_ADDR_BYTE, buf, GC20C3_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC20C3_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc20c3_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc20c3_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc20c3_i2c_fd[dev], g_gc20c3_i2c_addr[dev], buf, GC20C3_ADDR_BYTE + GC20C3_DATA_BYTE);
    if (ret != (GC20C3_ADDR_BYTE + GC20C3_DATA_BYTE)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!ret=%d.\n", ret);
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 gc20c3_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    gc20c3_read_reg(dev, GC20C3_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = gc20c3_write_reg(dev, GC20C3_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc20c3_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
