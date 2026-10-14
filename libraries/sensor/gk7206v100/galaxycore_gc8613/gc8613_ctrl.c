#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc8613.h"
#include "gc8613_ctrl.h"

#define GC8613_REG_ADDR_MIRROR_FLIP 0x0063
#define GC8613_REG_ADDR_FLIP_FLIP   0x022c

static xmedia_s32 g_gc8613_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_gc8613_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static void gc8613_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

#ifdef FPGA
static xmedia_s32 gc8613_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc8613_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x00);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a38, 0x01);
    ret |= gc8613_write_reg(dev, 0x0a20, 0x19);
    ret |= gc8613_write_reg(dev, 0x061b, 0x17);
    ret |= gc8613_write_reg(dev, 0x061c, 0x48);
    ret |= gc8613_write_reg(dev, 0x061d, 0x05);
    ret |= gc8613_write_reg(dev, 0x061e, 0x50);
    ret |= gc8613_write_reg(dev, 0x061f, 0x05);
    ret |= gc8613_write_reg(dev, 0x0a21, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a31, 0xb0);
    ret |= gc8613_write_reg(dev, 0x0a34, 0x40);
    ret |= gc8613_write_reg(dev, 0x0a35, 0x08);
    ret |= gc8613_write_reg(dev, 0x0a37, 0x44);
    ret |= gc8613_write_reg(dev, 0x0314, 0x70);
    ret |= gc8613_write_reg(dev, 0x0315, 0x00);
    ret |= gc8613_write_reg(dev, 0x031c, 0xce);
    ret |= gc8613_write_reg(dev, 0x0219, 0x47);

    // ret |= gc8613_write_reg(dev, 0x0342, 0x03);
    // ret |= gc8613_write_reg(dev, 0x0343, 0x10);
    ret |= gc8613_write_reg(dev, 0x0342, 0x06);    // line len xjh
    ret |= gc8613_write_reg(dev, 0x0343, 0x20);    // line len xjh
    // ret |= gc8613_write_reg(dev, 0x0342, 0x0c); // line len xjh
    // ret |= gc8613_write_reg(dev, 0x0343, 0x40); // line len xjh

    ret |= gc8613_write_reg(dev, 0x0259, 0x08);
    ret |= gc8613_write_reg(dev, 0x025a, 0x98);

    // ret |= gc8613_write_reg(dev, 0x0340, 0x08);
    // ret |= gc8613_write_reg(dev, 0x0341, 0xf8);
    // ret |= gc8613_write_reg(dev, 0x0340, 0x11); // frame len xjh
    // ret |= gc8613_write_reg(dev, 0x0341, 0xf0); // frame len xjh
    ret |= gc8613_write_reg(dev, 0x0340, 0x23);    // frame len xjh
    ret |= gc8613_write_reg(dev, 0x0341, 0xe0);    // frame len xjh

    ret |= gc8613_write_reg(dev, 0x0345, 0x02);
    ret |= gc8613_write_reg(dev, 0x0347, 0x02);
    ret |= gc8613_write_reg(dev, 0x0348, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0349, 0x18);
    ret |= gc8613_write_reg(dev, 0x034a, 0x08);
    ret |= gc8613_write_reg(dev, 0x034b, 0x88);
    ret |= gc8613_write_reg(dev, 0x034f, 0xf0);
    ret |= gc8613_write_reg(dev, 0x0094, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0095, 0x00);
    ret |= gc8613_write_reg(dev, 0x0096, 0x08);
    ret |= gc8613_write_reg(dev, 0x0097, 0x70);
    ret |= gc8613_write_reg(dev, 0x0099, 0x0c);
    ret |= gc8613_write_reg(dev, 0x009b, 0x0c);
    ret |= gc8613_write_reg(dev, 0x060c, 0x06);
    ret |= gc8613_write_reg(dev, 0x060e, 0x20);
    ret |= gc8613_write_reg(dev, 0x060f, 0x0f);
    ret |= gc8613_write_reg(dev, 0x070c, 0x06);
    ret |= gc8613_write_reg(dev, 0x070e, 0x20);
    ret |= gc8613_write_reg(dev, 0x070f, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0087, 0x50);
    ret |= gc8613_write_reg(dev, 0x0907, 0xd5);
    ret |= gc8613_write_reg(dev, 0x0909, 0x06);
    ret |= gc8613_write_reg(dev, 0x0902, 0x0b);
    ret |= gc8613_write_reg(dev, 0x0904, 0x08);
    ret |= gc8613_write_reg(dev, 0x0908, 0x09);
    ret |= gc8613_write_reg(dev, 0x0903, 0xc5);
    ret |= gc8613_write_reg(dev, 0x090c, 0x09);
    ret |= gc8613_write_reg(dev, 0x0905, 0x10);
    ret |= gc8613_write_reg(dev, 0x0906, 0x00);
    ret |= gc8613_write_reg(dev, 0x0724, 0x2b);
    ret |= gc8613_write_reg(dev, 0x0727, 0x2b);
    ret |= gc8613_write_reg(dev, 0x072b, 0x1c);
    ret |= gc8613_write_reg(dev, 0x072a, 0x7c);
    ret |= gc8613_write_reg(dev, 0x073e, 0x40);
    ret |= gc8613_write_reg(dev, 0x0078, 0x88);
    ret |= gc8613_write_reg(dev, 0x0268, 0x40);
    ret |= gc8613_write_reg(dev, 0x0269, 0x44);
    ret |= gc8613_write_reg(dev, 0x0351, 0x54);
    ret |= gc8613_write_reg(dev, 0x0618, 0x01);
    ret |= gc8613_write_reg(dev, 0x1466, 0x45);
    ret |= gc8613_write_reg(dev, 0x1468, 0x46);
    ret |= gc8613_write_reg(dev, 0x1467, 0x46);
    ret |= gc8613_write_reg(dev, 0x0709, 0x40);
    ret |= gc8613_write_reg(dev, 0x0719, 0x40);
    ret |= gc8613_write_reg(dev, 0x1469, 0xf0);
    ret |= gc8613_write_reg(dev, 0x146a, 0xd0);
    ret |= gc8613_write_reg(dev, 0x146b, 0x03);
    ret |= gc8613_write_reg(dev, 0x1480, 0x07);
    ret |= gc8613_write_reg(dev, 0x1481, 0x80);
    ret |= gc8613_write_reg(dev, 0x1484, 0x0b);
    ret |= gc8613_write_reg(dev, 0x1485, 0xc0);
    ret |= gc8613_write_reg(dev, 0x1430, 0x80);
    ret |= gc8613_write_reg(dev, 0x1407, 0x10);
    ret |= gc8613_write_reg(dev, 0x1408, 0x16);
    ret |= gc8613_write_reg(dev, 0x1409, 0x03);
    ret |= gc8613_write_reg(dev, 0x1434, 0x04);
    ret |= gc8613_write_reg(dev, 0x1447, 0x75);
    ret |= gc8613_write_reg(dev, 0x1470, 0x10);
    ret |= gc8613_write_reg(dev, 0x1471, 0x13);
    ret |= gc8613_write_reg(dev, 0x0122, 0x0b);
    ret |= gc8613_write_reg(dev, 0x0123, 0x30);
    ret |= gc8613_write_reg(dev, 0x0124, 0x04);
    ret |= gc8613_write_reg(dev, 0x0125, 0x30);
    ret |= gc8613_write_reg(dev, 0x0126, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0127, 0x15);
    ret |= gc8613_write_reg(dev, 0x0128, 0xa8);
    ret |= gc8613_write_reg(dev, 0x0129, 0x0c);
    ret |= gc8613_write_reg(dev, 0x012a, 0x18);
    ret |= gc8613_write_reg(dev, 0x012b, 0x18);
    ret |= gc8613_write_reg(dev, 0x1438, 0x00);
    ret |= gc8613_write_reg(dev, 0x143a, 0x00);
    ret |= gc8613_write_reg(dev, 0x024b, 0x03);
    ret |= gc8613_write_reg(dev, 0x0245, 0xc7);
    ret |= gc8613_write_reg(dev, 0x025b, 0x07);
    ret |= gc8613_write_reg(dev, 0x02bb, 0x77);
    ret |= gc8613_write_reg(dev, 0x0612, 0x01);
    ret |= gc8613_write_reg(dev, 0x0613, 0x24);
    ret |= gc8613_write_reg(dev, 0x0243, 0x66);
    ret |= gc8613_write_reg(dev, 0x0087, 0x53);
    ret |= gc8613_write_reg(dev, 0x0053, 0x05);
    ret |= gc8613_write_reg(dev, 0x0089, 0x00);
    ret |= gc8613_write_reg(dev, 0x0002, 0xeb);
    ret |= gc8613_write_reg(dev, 0x005a, 0x0c);
    ret |= gc8613_write_reg(dev, 0x0040, 0x83);
    ret |= gc8613_write_reg(dev, 0x0075, 0x68);
    ret |= gc8613_write_reg(dev, 0x0205, 0x0c);
    ret |= gc8613_write_reg(dev, 0x0202, 0x03);
    ret |= gc8613_write_reg(dev, 0x0203, 0x27);
    ret |= gc8613_write_reg(dev, 0x061a, 0x02);
    ret |= gc8613_write_reg(dev, 0x0213, 0x64);
    ret |= gc8613_write_reg(dev, 0x0265, 0x01);
    ret |= gc8613_write_reg(dev, 0x0618, 0x05);
    ret |= gc8613_write_reg(dev, 0x026e, 0x74);
    ret |= gc8613_write_reg(dev, 0x0270, 0x02);
    ret |= gc8613_write_reg(dev, 0x0709, 0x00);
    ret |= gc8613_write_reg(dev, 0x0719, 0x00);
    ret |= gc8613_write_reg(dev, 0x0812, 0xdb);
    ret |= gc8613_write_reg(dev, 0x0822, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0821, 0x18);
    ret |= gc8613_write_reg(dev, 0x0002, 0xef);
    ret |= gc8613_write_reg(dev, 0x0813, 0xfb);
    ret |= gc8613_write_reg(dev, 0x0070, 0x88);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x00);
    ret |= gc8613_write_reg(dev, 0x0106, 0x78);
    ret |= gc8613_write_reg(dev, 0x0136, 0x03);
    ret |= gc8613_write_reg(dev, 0x0181, 0xf0);
    ret |= gc8613_write_reg(dev, 0x0185, 0x01);
    ret |= gc8613_write_reg(dev, 0x0180, 0x46);
    ret |= gc8613_write_reg(dev, 0x0106, 0x38);
    ret |= gc8613_write_reg(dev, 0x010d, 0x40);
    ret |= gc8613_write_reg(dev, 0x010e, 0x1a);
    ret |= gc8613_write_reg(dev, 0x0111, 0x2d);
    ret |= gc8613_write_reg(dev, 0x0112, 0x03);
    ret |= gc8613_write_reg(dev, 0x0114, 0x03);

    ret |= gc8613_write_reg(dev, 0x0115, 0x12); // 连续模式  xjh

    ret |= gc8613_write_reg(dev, 0x0100, 0x09);
    ret |= gc8613_write_reg(dev, 0x79cf, 0x01);
    ret |= gc8613_write_reg(dev, 0x0219, 0x47);
    ret |= gc8613_write_reg(dev, 0x0054, 0x98);
    ret |= gc8613_write_reg(dev, 0x0076, 0x01);
    ret |= gc8613_write_reg(dev, 0x0052, 0x02);
    ret |= gc8613_write_reg(dev, 0x021a, 0x10);
    ret |= gc8613_write_reg(dev, 0x0430, 0x05);
    ret |= gc8613_write_reg(dev, 0x0431, 0x05);
    ret |= gc8613_write_reg(dev, 0x0432, 0x05);
    ret |= gc8613_write_reg(dev, 0x0433, 0x05);
    ret |= gc8613_write_reg(dev, 0x0434, 0x70);
    ret |= gc8613_write_reg(dev, 0x0435, 0x70);
    ret |= gc8613_write_reg(dev, 0x0436, 0x70);
    ret |= gc8613_write_reg(dev, 0x0437, 0x70);
    ret |= gc8613_write_reg(dev, 0x0004, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0704, 0x07);
    ret |= gc8613_write_reg(dev, 0x0706, 0x02);
    ret |= gc8613_write_reg(dev, 0x0716, 0x02);
    ret |= gc8613_write_reg(dev, 0x0708, 0xc8);
    ret |= gc8613_write_reg(dev, 0x0718, 0xc8);
    ret |= gc8613_write_reg(dev, 0x1469, 0x80);
    //otp autoload
    ret |= gc8613_write_reg(dev, 0x031f, 0x01);
    ret |= gc8613_write_reg(dev, 0x031f, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a67, 0x80);
    ret |= gc8613_write_reg(dev, 0x0a54, 0x0e);
    ret |= gc8613_write_reg(dev, 0x0a65, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a98, 0x04);
    ret |= gc8613_write_reg(dev, 0x05be, 0x00);
    ret |= gc8613_write_reg(dev, 0x05a9, 0x01);
    ret |= gc8613_write_reg(dev, 0x0089, 0x02);
    ret |= gc8613_write_reg(dev, 0x0aa0, 0x00);
    ret |= gc8613_write_reg(dev, 0x0023, 0x00);
    ret |= gc8613_write_reg(dev, 0x0022, 0x00);
    ret |= gc8613_write_reg(dev, 0x0025, 0x00);
    ret |= gc8613_write_reg(dev, 0x0024, 0x00);
    ret |= gc8613_write_reg(dev, 0x0028, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0029, 0x18);
    ret |= gc8613_write_reg(dev, 0x002a, 0x08);
    ret |= gc8613_write_reg(dev, 0x002b, 0x88);
    ret |= gc8613_write_reg(dev, 0x0317, 0x1c);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x03);
    ret |= gc8613_write_reg(dev, 0x0a82, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a83, 0xe0);
    ret |= gc8613_write_reg(dev, 0x0a71, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a72, 0x02);
    ret |= gc8613_write_reg(dev, 0x0a73, 0x60);
    ret |= gc8613_write_reg(dev, 0x0a75, 0x41);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x03);
    ret |= gc8613_write_reg(dev, 0x0a5a, 0x80);
    gc8613_delay_ms(1);
    ret |= gc8613_write_reg(dev, 0x0089, 0x00);
    ret |= gc8613_write_reg(dev, 0x05be, 0x01);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x00);
    ret |= gc8613_write_reg(dev, 0x0080, 0x02);
    ret |= gc8613_write_reg(dev, 0x0a67, 0x00);
    ret |= gc8613_write_reg(dev, 0x024b, 0x02);
    ret |= gc8613_write_reg(dev, 0x0220, 0xcf);
    ret |= gc8613_write_reg(dev, 0x0100, 0x09);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc8613_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC8613_27MInput_MIPI_4lane_344Mbps_14bit_linear_3840x2160_30fps init success!=====\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_2lane_linear_3840x2160_init(xmedia_u32 dev)
{
    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC8613_27MInput_MIPI_2lane_10bit_linear_3840x2160_30fps not support!==============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}
#else
static xmedia_s32 gc8613_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc8613_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x00);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a38, 0x01);
    ret |= gc8613_write_reg(dev, 0x0a20, 0x19);
    ret |= gc8613_write_reg(dev, 0x061b, 0x17);
    ret |= gc8613_write_reg(dev, 0x061c, 0x48);
    ret |= gc8613_write_reg(dev, 0x061d, 0x05);
    ret |= gc8613_write_reg(dev, 0x061e, 0x50);
    ret |= gc8613_write_reg(dev, 0x061f, 0x05);
    ret |= gc8613_write_reg(dev, 0x0a21, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a31, 0xb0);
    ret |= gc8613_write_reg(dev, 0x0a34, 0x40);
    ret |= gc8613_write_reg(dev, 0x0a35, 0x08);
    ret |= gc8613_write_reg(dev, 0x0a37, 0x44);
    ret |= gc8613_write_reg(dev, 0x0314, 0x70);
    ret |= gc8613_write_reg(dev, 0x0315, 0x00);
    ret |= gc8613_write_reg(dev, 0x031c, 0xce);
    ret |= gc8613_write_reg(dev, 0x0219, 0x47);
    ret |= gc8613_write_reg(dev, 0x0342, 0x03);
    ret |= gc8613_write_reg(dev, 0x0343, 0x10);
    ret |= gc8613_write_reg(dev, 0x0259, 0x08);
    ret |= gc8613_write_reg(dev, 0x025a, 0x98);
    ret |= gc8613_write_reg(dev, 0x0340, 0x08);
    ret |= gc8613_write_reg(dev, 0x0341, 0xf8);
    ret |= gc8613_write_reg(dev, 0x0345, 0x02);
    ret |= gc8613_write_reg(dev, 0x0347, 0x02);
    ret |= gc8613_write_reg(dev, 0x0348, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0349, 0x18);
    ret |= gc8613_write_reg(dev, 0x034a, 0x08);
    ret |= gc8613_write_reg(dev, 0x034b, 0x88);
    ret |= gc8613_write_reg(dev, 0x034f, 0xf0);
    ret |= gc8613_write_reg(dev, 0x0094, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0095, 0x00);
    ret |= gc8613_write_reg(dev, 0x0096, 0x08);
    ret |= gc8613_write_reg(dev, 0x0097, 0x70);
    ret |= gc8613_write_reg(dev, 0x0099, 0x0c);
    ret |= gc8613_write_reg(dev, 0x009b, 0x0c);
    ret |= gc8613_write_reg(dev, 0x060c, 0x06);
    ret |= gc8613_write_reg(dev, 0x060e, 0x20);
    ret |= gc8613_write_reg(dev, 0x060f, 0x0f);
    ret |= gc8613_write_reg(dev, 0x070c, 0x06);
    ret |= gc8613_write_reg(dev, 0x070e, 0x20);
    ret |= gc8613_write_reg(dev, 0x070f, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0087, 0x50);
    ret |= gc8613_write_reg(dev, 0x0907, 0xd5);
    ret |= gc8613_write_reg(dev, 0x0909, 0x06);
    ret |= gc8613_write_reg(dev, 0x0902, 0x0b);
    ret |= gc8613_write_reg(dev, 0x0904, 0x08);
    ret |= gc8613_write_reg(dev, 0x0908, 0x09);
    ret |= gc8613_write_reg(dev, 0x0903, 0xc5);
    ret |= gc8613_write_reg(dev, 0x090c, 0x09);
    ret |= gc8613_write_reg(dev, 0x0905, 0x10);
    ret |= gc8613_write_reg(dev, 0x0906, 0x00);
    ret |= gc8613_write_reg(dev, 0x0724, 0x2b);
    ret |= gc8613_write_reg(dev, 0x0727, 0x2b);
    ret |= gc8613_write_reg(dev, 0x072b, 0x1c);
    ret |= gc8613_write_reg(dev, 0x072a, 0x7c);
    ret |= gc8613_write_reg(dev, 0x073e, 0x40);
    ret |= gc8613_write_reg(dev, 0x0078, 0x88);
    ret |= gc8613_write_reg(dev, 0x0268, 0x40);
    ret |= gc8613_write_reg(dev, 0x0269, 0x44);
    ret |= gc8613_write_reg(dev, 0x0351, 0x54);
    ret |= gc8613_write_reg(dev, 0x0618, 0x01);
    ret |= gc8613_write_reg(dev, 0x1466, 0x45);
    ret |= gc8613_write_reg(dev, 0x1468, 0x46);
    ret |= gc8613_write_reg(dev, 0x1467, 0x46);
    ret |= gc8613_write_reg(dev, 0x0709, 0x40);
    ret |= gc8613_write_reg(dev, 0x0719, 0x40);
    ret |= gc8613_write_reg(dev, 0x1469, 0xf0);
    ret |= gc8613_write_reg(dev, 0x146a, 0xd0);
    ret |= gc8613_write_reg(dev, 0x146b, 0x03);
    ret |= gc8613_write_reg(dev, 0x1480, 0x07);
    ret |= gc8613_write_reg(dev, 0x1481, 0x80);
    ret |= gc8613_write_reg(dev, 0x1484, 0x0b);
    ret |= gc8613_write_reg(dev, 0x1485, 0xc0);
    ret |= gc8613_write_reg(dev, 0x1430, 0x80);
    ret |= gc8613_write_reg(dev, 0x1407, 0x10);
    ret |= gc8613_write_reg(dev, 0x1408, 0x16);
    ret |= gc8613_write_reg(dev, 0x1409, 0x03);
    ret |= gc8613_write_reg(dev, 0x1434, 0x04);
    ret |= gc8613_write_reg(dev, 0x1447, 0x75);
    ret |= gc8613_write_reg(dev, 0x1470, 0x10);
    ret |= gc8613_write_reg(dev, 0x1471, 0x13);
    ret |= gc8613_write_reg(dev, 0x0122, 0x0b);
    ret |= gc8613_write_reg(dev, 0x0123, 0x30);
    ret |= gc8613_write_reg(dev, 0x0124, 0x04);
    ret |= gc8613_write_reg(dev, 0x0125, 0x30);
    ret |= gc8613_write_reg(dev, 0x0126, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0127, 0x15);
    ret |= gc8613_write_reg(dev, 0x0128, 0xa8);
    ret |= gc8613_write_reg(dev, 0x0129, 0x0c);
    ret |= gc8613_write_reg(dev, 0x012a, 0x18);
    ret |= gc8613_write_reg(dev, 0x012b, 0x18);
    ret |= gc8613_write_reg(dev, 0x1438, 0x00);
    ret |= gc8613_write_reg(dev, 0x143a, 0x00);
    ret |= gc8613_write_reg(dev, 0x024b, 0x03);
    ret |= gc8613_write_reg(dev, 0x0245, 0xc7);
    ret |= gc8613_write_reg(dev, 0x025b, 0x07);
    ret |= gc8613_write_reg(dev, 0x02bb, 0x77);
    ret |= gc8613_write_reg(dev, 0x0612, 0x01);
    ret |= gc8613_write_reg(dev, 0x0613, 0x26);
    ret |= gc8613_write_reg(dev, 0x0243, 0x66);
    ret |= gc8613_write_reg(dev, 0x0087, 0x53);
    ret |= gc8613_write_reg(dev, 0x0053, 0x05);
    ret |= gc8613_write_reg(dev, 0x0089, 0x00);
    ret |= gc8613_write_reg(dev, 0x0002, 0xeb);
    ret |= gc8613_write_reg(dev, 0x005a, 0x0c);
    ret |= gc8613_write_reg(dev, 0x0040, 0x83);
    ret |= gc8613_write_reg(dev, 0x0075, 0x68);
    ret |= gc8613_write_reg(dev, 0x0205, 0x0c);
    ret |= gc8613_write_reg(dev, 0x0202, 0x03);
    ret |= gc8613_write_reg(dev, 0x0203, 0x27);
    ret |= gc8613_write_reg(dev, 0x061a, 0x02);
    ret |= gc8613_write_reg(dev, 0x0213, 0x64);
    ret |= gc8613_write_reg(dev, 0x0265, 0x01);
    ret |= gc8613_write_reg(dev, 0x0618, 0x05);
    ret |= gc8613_write_reg(dev, 0x026e, 0x74);
    ret |= gc8613_write_reg(dev, 0x0270, 0x02);
    ret |= gc8613_write_reg(dev, 0x0709, 0x00);
    ret |= gc8613_write_reg(dev, 0x0719, 0x00);
    ret |= gc8613_write_reg(dev, 0x0812, 0xdb);
    ret |= gc8613_write_reg(dev, 0x0822, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0821, 0x18);
    ret |= gc8613_write_reg(dev, 0x0002, 0xef);
    ret |= gc8613_write_reg(dev, 0x0813, 0xfb);
    ret |= gc8613_write_reg(dev, 0x0070, 0x88);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x00);
    ret |= gc8613_write_reg(dev, 0x0106, 0x78);
    ret |= gc8613_write_reg(dev, 0x0136, 0x03);
    ret |= gc8613_write_reg(dev, 0x0181, 0xf0);
    ret |= gc8613_write_reg(dev, 0x0185, 0x01);
    ret |= gc8613_write_reg(dev, 0x0180, 0x46);
    ret |= gc8613_write_reg(dev, 0x0106, 0x38);
    ret |= gc8613_write_reg(dev, 0x010d, 0x40);
    ret |= gc8613_write_reg(dev, 0x010e, 0x1a);
    ret |= gc8613_write_reg(dev, 0x0111, 0x2d);
    ret |= gc8613_write_reg(dev, 0x0112, 0x03);
    ret |= gc8613_write_reg(dev, 0x0114, 0x03);
    ret |= gc8613_write_reg(dev, 0x0100, 0x09);
    ret |= gc8613_write_reg(dev, 0x79cf, 0x01);
    ret |= gc8613_write_reg(dev, 0x0219, 0x47);
    ret |= gc8613_write_reg(dev, 0x0054, 0x98);
    ret |= gc8613_write_reg(dev, 0x0076, 0x01);
    ret |= gc8613_write_reg(dev, 0x0052, 0x02);
    ret |= gc8613_write_reg(dev, 0x021a, 0x10);
    ret |= gc8613_write_reg(dev, 0x0430, 0x05);
    ret |= gc8613_write_reg(dev, 0x0431, 0x05);
    ret |= gc8613_write_reg(dev, 0x0432, 0x05);
    ret |= gc8613_write_reg(dev, 0x0433, 0x05);
    ret |= gc8613_write_reg(dev, 0x0434, 0x70);
    ret |= gc8613_write_reg(dev, 0x0435, 0x70);
    ret |= gc8613_write_reg(dev, 0x0436, 0x70);
    ret |= gc8613_write_reg(dev, 0x0437, 0x70);
    ret |= gc8613_write_reg(dev, 0x0004, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0704, 0x07);
    ret |= gc8613_write_reg(dev, 0x0706, 0x02);
    ret |= gc8613_write_reg(dev, 0x0716, 0x02);
    ret |= gc8613_write_reg(dev, 0x0708, 0xc8);
    ret |= gc8613_write_reg(dev, 0x0718, 0xc8);
    ret |= gc8613_write_reg(dev, 0x1469, 0x80);
    ret |= gc8613_write_reg(dev, 0x031f, 0x01);
    ret |= gc8613_write_reg(dev, 0x031f, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a67, 0x80);
    ret |= gc8613_write_reg(dev, 0x0a54, 0x0e);
    ret |= gc8613_write_reg(dev, 0x0a65, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a98, 0x04);
    ret |= gc8613_write_reg(dev, 0x05be, 0x00);
    ret |= gc8613_write_reg(dev, 0x05a9, 0x01);
    ret |= gc8613_write_reg(dev, 0x0089, 0x02);
    ret |= gc8613_write_reg(dev, 0x0aa0, 0x00);
    ret |= gc8613_write_reg(dev, 0x0023, 0x00);
    ret |= gc8613_write_reg(dev, 0x0022, 0x00);
    ret |= gc8613_write_reg(dev, 0x0025, 0x00);
    ret |= gc8613_write_reg(dev, 0x0024, 0x00);
    ret |= gc8613_write_reg(dev, 0x0028, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0029, 0x18);
    ret |= gc8613_write_reg(dev, 0x002a, 0x08);
    ret |= gc8613_write_reg(dev, 0x002b, 0x88);
    ret |= gc8613_write_reg(dev, 0x0317, 0x1c);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x03);
    ret |= gc8613_write_reg(dev, 0x0a82, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a83, 0xe0);
    ret |= gc8613_write_reg(dev, 0x0a71, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a72, 0x02);
    ret |= gc8613_write_reg(dev, 0x0a73, 0x60);
    ret |= gc8613_write_reg(dev, 0x0a75, 0x41);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x03);
    ret |= gc8613_write_reg(dev, 0x0a5a, 0x80);
    gc8613_delay_ms(20);
    ret |= gc8613_write_reg(dev, 0x0089, 0x00);
    ret |= gc8613_write_reg(dev, 0x05be, 0x01);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x00);
    ret |= gc8613_write_reg(dev, 0x0080, 0x02);
    ret |= gc8613_write_reg(dev, 0x0a67, 0x00);
    ret |= gc8613_write_reg(dev, 0x024b, 0x02);
    ret |= gc8613_write_reg(dev, 0x0220, 0xcf);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc8613_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("========== GC8613_27MInput_MIPI_4lane_14bit_linear_3840x2160_30fps init success!=======\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_2lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc8613_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x00);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a38, 0x01);
    ret |= gc8613_write_reg(dev, 0x0a20, 0x19);
    ret |= gc8613_write_reg(dev, 0x061b, 0x17);
    ret |= gc8613_write_reg(dev, 0x061c, 0x50);
    ret |= gc8613_write_reg(dev, 0x061d, 0x05);
    ret |= gc8613_write_reg(dev, 0x061e, 0x50);
    ret |= gc8613_write_reg(dev, 0x061f, 0x05);
    ret |= gc8613_write_reg(dev, 0x0a21, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a31, 0xa0);
    ret |= gc8613_write_reg(dev, 0x0a34, 0x40);
    ret |= gc8613_write_reg(dev, 0x0a35, 0x08);
    ret |= gc8613_write_reg(dev, 0x0a37, 0x43);
    ret |= gc8613_write_reg(dev, 0x0314, 0x70);
    ret |= gc8613_write_reg(dev, 0x0315, 0x00);
    ret |= gc8613_write_reg(dev, 0x031c, 0xce);
    ret |= gc8613_write_reg(dev, 0x0219, 0x47);
    ret |= gc8613_write_reg(dev, 0x0342, 0x03);
    ret |= gc8613_write_reg(dev, 0x0343, 0x20);
    ret |= gc8613_write_reg(dev, 0x0259, 0x08);
    ret |= gc8613_write_reg(dev, 0x025a, 0x96);
    ret |= gc8613_write_reg(dev, 0x0340, 0x08);
    ret |= gc8613_write_reg(dev, 0x0341, 0xca);
    ret |= gc8613_write_reg(dev, 0x0351, 0x00);
    ret |= gc8613_write_reg(dev, 0x0345, 0x02);
    ret |= gc8613_write_reg(dev, 0x0347, 0x02);
    ret |= gc8613_write_reg(dev, 0x0348, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0349, 0x18);
    ret |= gc8613_write_reg(dev, 0x034a, 0x08);
    ret |= gc8613_write_reg(dev, 0x034b, 0x88);
    ret |= gc8613_write_reg(dev, 0x034f, 0xf0);
    ret |= gc8613_write_reg(dev, 0x0094, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0095, 0x00);
    ret |= gc8613_write_reg(dev, 0x0096, 0x08);
    ret |= gc8613_write_reg(dev, 0x0097, 0x70);
    ret |= gc8613_write_reg(dev, 0x0099, 0x0c);
    ret |= gc8613_write_reg(dev, 0x009b, 0x0c);
    ret |= gc8613_write_reg(dev, 0x060c, 0x06);
    ret |= gc8613_write_reg(dev, 0x060e, 0x20);
    ret |= gc8613_write_reg(dev, 0x060f, 0x0f);
    ret |= gc8613_write_reg(dev, 0x070c, 0x06);
    ret |= gc8613_write_reg(dev, 0x070e, 0x20);
    ret |= gc8613_write_reg(dev, 0x070f, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0087, 0x50);
    ret |= gc8613_write_reg(dev, 0x0907, 0xd5);
    ret |= gc8613_write_reg(dev, 0x0909, 0x06);
    ret |= gc8613_write_reg(dev, 0x0902, 0x0b);
    ret |= gc8613_write_reg(dev, 0x0904, 0x08);
    ret |= gc8613_write_reg(dev, 0x0908, 0x09);
    ret |= gc8613_write_reg(dev, 0x0903, 0xc5);
    ret |= gc8613_write_reg(dev, 0x090c, 0x09);
    ret |= gc8613_write_reg(dev, 0x0905, 0x10);
    ret |= gc8613_write_reg(dev, 0x0906, 0x00);
    ret |= gc8613_write_reg(dev, 0x072a, 0x7c);
    ret |= gc8613_write_reg(dev, 0x0724, 0x2b);
    ret |= gc8613_write_reg(dev, 0x0727, 0x2b);
    ret |= gc8613_write_reg(dev, 0x072b, 0x1c);
    ret |= gc8613_write_reg(dev, 0x073e, 0x40);
    ret |= gc8613_write_reg(dev, 0x0078, 0x88);
    ret |= gc8613_write_reg(dev, 0x0618, 0x01);
    ret |= gc8613_write_reg(dev, 0x1466, 0x12);
    ret |= gc8613_write_reg(dev, 0x1468, 0x10);
    ret |= gc8613_write_reg(dev, 0x1467, 0x10);
    ret |= gc8613_write_reg(dev, 0x0709, 0x40);
    ret |= gc8613_write_reg(dev, 0x0719, 0x40);
    ret |= gc8613_write_reg(dev, 0x1469, 0x80);
    ret |= gc8613_write_reg(dev, 0x146a, 0xc0);
    ret |= gc8613_write_reg(dev, 0x146b, 0x03);
    ret |= gc8613_write_reg(dev, 0x1480, 0x02);
    ret |= gc8613_write_reg(dev, 0x1481, 0x80);
    ret |= gc8613_write_reg(dev, 0x1484, 0x08);
    ret |= gc8613_write_reg(dev, 0x1485, 0xc0);
    ret |= gc8613_write_reg(dev, 0x1430, 0x80);
    ret |= gc8613_write_reg(dev, 0x1407, 0x10);
    ret |= gc8613_write_reg(dev, 0x1408, 0x16);
    ret |= gc8613_write_reg(dev, 0x1409, 0x03);
    ret |= gc8613_write_reg(dev, 0x1434, 0x04);
    ret |= gc8613_write_reg(dev, 0x1447, 0x75);
    ret |= gc8613_write_reg(dev, 0x1470, 0x10);
    ret |= gc8613_write_reg(dev, 0x1471, 0x13);
    ret |= gc8613_write_reg(dev, 0x1438, 0x00);
    ret |= gc8613_write_reg(dev, 0x143a, 0x00);
    ret |= gc8613_write_reg(dev, 0x024b, 0x02);
    ret |= gc8613_write_reg(dev, 0x0245, 0xc7);
    ret |= gc8613_write_reg(dev, 0x025b, 0x07);
    ret |= gc8613_write_reg(dev, 0x02bb, 0x77);
    ret |= gc8613_write_reg(dev, 0x0612, 0x01);
    ret |= gc8613_write_reg(dev, 0x0613, 0x26);
    ret |= gc8613_write_reg(dev, 0x0243, 0x66);
    ret |= gc8613_write_reg(dev, 0x0087, 0x53);
    ret |= gc8613_write_reg(dev, 0x0053, 0x05);
    ret |= gc8613_write_reg(dev, 0x0089, 0x02);
    ret |= gc8613_write_reg(dev, 0x0002, 0xeb);
    ret |= gc8613_write_reg(dev, 0x005a, 0x0c);
    ret |= gc8613_write_reg(dev, 0x0040, 0x83);
    ret |= gc8613_write_reg(dev, 0x0075, 0x54);
    ret |= gc8613_write_reg(dev, 0x0205, 0x0c);
    ret |= gc8613_write_reg(dev, 0x0202, 0x01);
    ret |= gc8613_write_reg(dev, 0x0203, 0x27);
    ret |= gc8613_write_reg(dev, 0x061a, 0x02);
    ret |= gc8613_write_reg(dev, 0x03fe, 0x00);
    ret |= gc8613_write_reg(dev, 0x0106, 0x78);
    ret |= gc8613_write_reg(dev, 0x0136, 0x03);
    ret |= gc8613_write_reg(dev, 0x0181, 0xf0);
    ret |= gc8613_write_reg(dev, 0x0185, 0x01);
    ret |= gc8613_write_reg(dev, 0x0180, 0x46);
    ret |= gc8613_write_reg(dev, 0x0106, 0x38);
    ret |= gc8613_write_reg(dev, 0x010d, 0xc0);
    ret |= gc8613_write_reg(dev, 0x010e, 0x12);
    ret |= gc8613_write_reg(dev, 0x0113, 0x02);
    ret |= gc8613_write_reg(dev, 0x0114, 0x01);
    ret |= gc8613_write_reg(dev, 0x0115, 0x12);
    ret |= gc8613_write_reg(dev, 0x0122, 0x11);
    ret |= gc8613_write_reg(dev, 0x0123, 0x40);
    ret |= gc8613_write_reg(dev, 0x0126, 0x0e);
    ret |= gc8613_write_reg(dev, 0x0129, 0x12);
    ret |= gc8613_write_reg(dev, 0x012a, 0x1a);
    ret |= gc8613_write_reg(dev, 0x012b, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0004, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0219, 0x47);
    ret |= gc8613_write_reg(dev, 0x0054, 0x98);
    ret |= gc8613_write_reg(dev, 0x0076, 0x01);
    ret |= gc8613_write_reg(dev, 0x0052, 0x02);
    ret |= gc8613_write_reg(dev, 0x021a, 0x10);
    ret |= gc8613_write_reg(dev, 0x0430, 0x21);
    ret |= gc8613_write_reg(dev, 0x0431, 0x21);
    ret |= gc8613_write_reg(dev, 0x0432, 0x21);
    ret |= gc8613_write_reg(dev, 0x0433, 0x21);
    ret |= gc8613_write_reg(dev, 0x0434, 0x61);
    ret |= gc8613_write_reg(dev, 0x0435, 0x61);
    ret |= gc8613_write_reg(dev, 0x0436, 0x61);
    ret |= gc8613_write_reg(dev, 0x0437, 0x61);
    ret |= gc8613_write_reg(dev, 0x0704, 0x07);
    ret |= gc8613_write_reg(dev, 0x0706, 0x02);
    ret |= gc8613_write_reg(dev, 0x0716, 0x02);
    ret |= gc8613_write_reg(dev, 0x0708, 0xc8);
    ret |= gc8613_write_reg(dev, 0x0718, 0xc8);
    //otp autoload
    ret |= gc8613_write_reg(dev, 0x031f, 0x01);
    ret |= gc8613_write_reg(dev, 0x031f, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a67, 0x80);
    ret |= gc8613_write_reg(dev, 0x0a54, 0x0e);
    ret |= gc8613_write_reg(dev, 0x0a65, 0x10);
    ret |= gc8613_write_reg(dev, 0x0a98, 0x04);
    ret |= gc8613_write_reg(dev, 0x05be, 0x00);
    ret |= gc8613_write_reg(dev, 0x05a9, 0x01);
    ret |= gc8613_write_reg(dev, 0x0089, 0x02);
    ret |= gc8613_write_reg(dev, 0x0aa0, 0x00);
    ret |= gc8613_write_reg(dev, 0x0023, 0x00);
    ret |= gc8613_write_reg(dev, 0x0022, 0x00);
    ret |= gc8613_write_reg(dev, 0x0025, 0x00);
    ret |= gc8613_write_reg(dev, 0x0024, 0x00);
    ret |= gc8613_write_reg(dev, 0x0028, 0x0f);
    ret |= gc8613_write_reg(dev, 0x0029, 0x18);
    ret |= gc8613_write_reg(dev, 0x002a, 0x08);
    ret |= gc8613_write_reg(dev, 0x002b, 0x88);
    ret |= gc8613_write_reg(dev, 0x0317, 0x1c);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x03);
    ret |= gc8613_write_reg(dev, 0x0a82, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a83, 0xe0);
    ret |= gc8613_write_reg(dev, 0x0a71, 0x00);
    ret |= gc8613_write_reg(dev, 0x0a72, 0x02);
    ret |= gc8613_write_reg(dev, 0x0a73, 0x60);
    ret |= gc8613_write_reg(dev, 0x0a75, 0x41);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x03);
    ret |= gc8613_write_reg(dev, 0x0a5a, 0x80);
    gc8613_delay_ms(20);
    ret |= gc8613_write_reg(dev, 0x0089, 0x02);
    ret |= gc8613_write_reg(dev, 0x05be, 0x01);
    ret |= gc8613_write_reg(dev, 0x0a70, 0x00);
    ret |= gc8613_write_reg(dev, 0x0080, 0x02);
    ret |= gc8613_write_reg(dev, 0x0a67, 0x00);
    ret |= gc8613_write_reg(dev, 0x0100, 0x09);


    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc8613_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC8613_27MInput_MIPI_2lane_10bit_linear_3840x2160_30fps init success!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}
#endif

xmedia_s32 gc8613_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC8613_4L_8M_LINEAR_MODE:
            ret = gc8613_4lane_linear_3840x2160_init(dev);
            break;

        case GC8613_2L_8M_LINEAR_MODE:
            ret = gc8613_2lane_linear_3840x2160_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc8613_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc8613_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc8613_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc8613_i2c_fd[dev]);
        g_gc8613_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc8613_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc8613_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc8613_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc8613_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc8613_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc8613_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc8613_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc8613_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc8613_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc8613_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC8613_DATA_BYTE];

    if (g_gc8613_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc8613_i2c_fd[dev], g_gc8613_i2c_addr[dev], addr, GC8613_ADDR_BYTE, buf, GC8613_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC8613_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc8613_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc8613_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc8613_i2c_fd[dev], g_gc8613_i2c_addr[dev], buf, GC8613_ADDR_BYTE + GC8613_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc8613_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (mirror_en && flip_en) {
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_MIRROR_FLIP, 0x05);
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_FLIP_FLIP,   0x01);
    } else if (mirror_en && (!flip_en)) {
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_MIRROR_FLIP, 0x05);
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_FLIP_FLIP,   0x00);
    } else if ((!mirror_en) && flip_en) {
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_MIRROR_FLIP, 0x02);
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_FLIP_FLIP,   0x01);
    } else if ((!mirror_en) && (!flip_en)) {
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_MIRROR_FLIP, 0x00);
        ret |= gc8613_write_reg(dev, GC8613_REG_ADDR_FLIP_FLIP,   0x00);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc8613_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
