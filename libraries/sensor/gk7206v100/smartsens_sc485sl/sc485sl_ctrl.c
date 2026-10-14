#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc485sl_ctrl.h"

#define SC485SL_REG_ADDR_MIRROR_FLIP 0x3221

SENSOR_PRIORITY_DATA static xmedia_s32 g_sc485sl_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_sc485sl_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

SENSOR_PRIORITY_FUNC static xmedia_s32 sc485sl_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= sc485sl_write_reg(dev, 0x0100, 0x01);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc485sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc485sl_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc485sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc485sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x23b0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b1, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b2, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23b4, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23b6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b9, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23ba, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bb, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bf, 0x78);
    ret |= sc485sl_write_reg(dev, 0x23c0, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c1, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23c2, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23c4, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23c6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23c8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c9, 0x78);
    ret |= sc485sl_write_reg(dev, 0x3018, 0x7a);
    ret |= sc485sl_write_reg(dev, 0x301e, 0xf0);
    ret |= sc485sl_write_reg(dev, 0x301f, 0x04);
    ret |= sc485sl_write_reg(dev, 0x302c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3031, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x3034, 0xee);
    ret |= sc485sl_write_reg(dev, 0x30b0, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3204, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3205, 0x8f);
    ret |= sc485sl_write_reg(dev, 0x3206, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3207, 0xff);
    ret |= sc485sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3209, 0x80);
    ret |= sc485sl_write_reg(dev, 0x320a, 0x05);
    ret |= sc485sl_write_reg(dev, 0x320b, 0xf0);
    ret |= sc485sl_write_reg(dev, 0x320c, 0x08);
    ret |= sc485sl_write_reg(dev, 0x320d, 0xe8);
    ret |= sc485sl_write_reg(dev, 0x320e, 0x06);
    ret |= sc485sl_write_reg(dev, 0x320f, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3211, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3213, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3214, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3215, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3250, 0x00);
    ret |= sc485sl_write_reg(dev, 0x325f, 0x2d);
    ret |= sc485sl_write_reg(dev, 0x32d1, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x32e0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x32e4, 0x02);
    ret |= sc485sl_write_reg(dev, 0x3301, 0x0e);
    ret |= sc485sl_write_reg(dev, 0x3303, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3304, 0x60);
    ret |= sc485sl_write_reg(dev, 0x3305, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3306, 0xe0);
    ret |= sc485sl_write_reg(dev, 0x3308, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3309, 0x60);
    ret |= sc485sl_write_reg(dev, 0x330a, 0x02);
    ret |= sc485sl_write_reg(dev, 0x330b, 0x48);
    ret |= sc485sl_write_reg(dev, 0x330d, 0x18);
    ret |= sc485sl_write_reg(dev, 0x330e, 0x18);
    ret |= sc485sl_write_reg(dev, 0x330f, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3310, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3314, 0x94);
    ret |= sc485sl_write_reg(dev, 0x331e, 0x51);
    ret |= sc485sl_write_reg(dev, 0x331f, 0x51);
    ret |= sc485sl_write_reg(dev, 0x3333, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3334, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3347, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x334c, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3364, 0x5e);
    ret |= sc485sl_write_reg(dev, 0x3393, 0x12);
    ret |= sc485sl_write_reg(dev, 0x3394, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3395, 0x3c);
    ret |= sc485sl_write_reg(dev, 0x3399, 0x0e);
    ret |= sc485sl_write_reg(dev, 0x339a, 0x12);
    ret |= sc485sl_write_reg(dev, 0x339b, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x339c, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x33ac, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x33ad, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x33ae, 0x50);
    ret |= sc485sl_write_reg(dev, 0x33af, 0x50);
    ret |= sc485sl_write_reg(dev, 0x33b0, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x33b2, 0x48);
    ret |= sc485sl_write_reg(dev, 0x33b3, 0x28);
    ret |= sc485sl_write_reg(dev, 0x33f8, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33f9, 0xf0);
    ret |= sc485sl_write_reg(dev, 0x33fa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33fb, 0x70);
    ret |= sc485sl_write_reg(dev, 0x3445, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3447, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3448, 0x14);
    ret |= sc485sl_write_reg(dev, 0x3449, 0x04);
    ret |= sc485sl_write_reg(dev, 0x344a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x344b, 0x04);
    ret |= sc485sl_write_reg(dev, 0x344c, 0x38);
    ret |= sc485sl_write_reg(dev, 0x344d, 0x04);
    ret |= sc485sl_write_reg(dev, 0x344e, 0x78);
    ret |= sc485sl_write_reg(dev, 0x349f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x34a8, 0x20);
    ret |= sc485sl_write_reg(dev, 0x34a9, 0x18);
    ret |= sc485sl_write_reg(dev, 0x34aa, 0x02);
    ret |= sc485sl_write_reg(dev, 0x34ab, 0x58);
    ret |= sc485sl_write_reg(dev, 0x34ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ad, 0xd0);
    ret |= sc485sl_write_reg(dev, 0x34f9, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x3616, 0xbc);
    ret |= sc485sl_write_reg(dev, 0x3632, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3633, 0x41);
    ret |= sc485sl_write_reg(dev, 0x363a, 0x8c);
    ret |= sc485sl_write_reg(dev, 0x363b, 0xde);
    ret |= sc485sl_write_reg(dev, 0x3670, 0x41);
    ret |= sc485sl_write_reg(dev, 0x3671, 0x42);
    ret |= sc485sl_write_reg(dev, 0x3672, 0x33);
    ret |= sc485sl_write_reg(dev, 0x3673, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3674, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3675, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3676, 0x18);
    ret |= sc485sl_write_reg(dev, 0x367e, 0x69);
    ret |= sc485sl_write_reg(dev, 0x367f, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3680, 0x91);
    ret |= sc485sl_write_reg(dev, 0x3681, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3682, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3683, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3684, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3685, 0x81);
    ret |= sc485sl_write_reg(dev, 0x3686, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3687, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3688, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3689, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368a, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368b, 0x85);
    ret |= sc485sl_write_reg(dev, 0x368c, 0x88);
    ret |= sc485sl_write_reg(dev, 0x368d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x368e, 0x08);
    ret |= sc485sl_write_reg(dev, 0x368f, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3690, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3691, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3692, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3693, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3694, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3695, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3696, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3697, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3698, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3699, 0x04);
    ret |= sc485sl_write_reg(dev, 0x369a, 0x78);
    ret |= sc485sl_write_reg(dev, 0x369b, 0xde);
    ret |= sc485sl_write_reg(dev, 0x369c, 0xde);
    ret |= sc485sl_write_reg(dev, 0x369d, 0x5e);
    ret |= sc485sl_write_reg(dev, 0x369e, 0x04);
    ret |= sc485sl_write_reg(dev, 0x369f, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36a0, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36a1, 0x78);
    ret |= sc485sl_write_reg(dev, 0x36b9, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36ba, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36bb, 0x13);
    ret |= sc485sl_write_reg(dev, 0x36bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x36be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bf, 0x38);
    ret |= sc485sl_write_reg(dev, 0x36d0, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x36d1, 0x20);
    ret |= sc485sl_write_reg(dev, 0x36ea, 0x14);
    ret |= sc485sl_write_reg(dev, 0x36eb, 0x45);
    ret |= sc485sl_write_reg(dev, 0x36ec, 0x69);
    ret |= sc485sl_write_reg(dev, 0x36ed, 0x98);
    ret |= sc485sl_write_reg(dev, 0x370f, 0x31);
    ret |= sc485sl_write_reg(dev, 0x3722, 0x03);
    ret |= sc485sl_write_reg(dev, 0x3724, 0xc1);
    ret |= sc485sl_write_reg(dev, 0x3727, 0x24);
    ret |= sc485sl_write_reg(dev, 0x3729, 0x84);
    ret |= sc485sl_write_reg(dev, 0x37b0, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b1, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b2, 0xf3);
    ret |= sc485sl_write_reg(dev, 0x37b3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b4, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37b5, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b6, 0x78);
    ret |= sc485sl_write_reg(dev, 0x37b7, 0xc4);
    ret |= sc485sl_write_reg(dev, 0x37b8, 0xf4);
    ret |= sc485sl_write_reg(dev, 0x37b9, 0xb4);
    ret |= sc485sl_write_reg(dev, 0x37ba, 0x10);
    ret |= sc485sl_write_reg(dev, 0x37bb, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x37bc, 0x4e);
    ret |= sc485sl_write_reg(dev, 0x37bd, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37be, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bf, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c0, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37c1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c2, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37c3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c4, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fa, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fb, 0x55);
    ret |= sc485sl_write_reg(dev, 0x37fc, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fd, 0x1a);
    ret |= sc485sl_write_reg(dev, 0x3900, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3903, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3907, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3908, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391a, 0x70);
    ret |= sc485sl_write_reg(dev, 0x391b, 0x46);
    ret |= sc485sl_write_reg(dev, 0x391c, 0x26);
    ret |= sc485sl_write_reg(dev, 0x391d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391f, 0x41);
    ret |= sc485sl_write_reg(dev, 0x3926, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3933, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3934, 0x07);
    ret |= sc485sl_write_reg(dev, 0x3935, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3936, 0x34);
    ret |= sc485sl_write_reg(dev, 0x3937, 0x70);
    ret |= sc485sl_write_reg(dev, 0x3938, 0x74);
    ret |= sc485sl_write_reg(dev, 0x3939, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393a, 0x03);
    ret |= sc485sl_write_reg(dev, 0x393b, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393c, 0x06);
    ret |= sc485sl_write_reg(dev, 0x39c9, 0x60);
    ret |= sc485sl_write_reg(dev, 0x39dd, 0x00);
    ret |= sc485sl_write_reg(dev, 0x39de, 0x08);
    ret |= sc485sl_write_reg(dev, 0x39e7, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3e00, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e01, 0x62);
    ret |= sc485sl_write_reg(dev, 0x3e02, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc485sl_write_reg(dev, 0x3e08, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e16, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e17, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3e18, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e19, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x4402, 0x04);
    ret |= sc485sl_write_reg(dev, 0x4403, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x4404, 0x38);
    ret |= sc485sl_write_reg(dev, 0x4405, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x440c, 0x38);
    ret |= sc485sl_write_reg(dev, 0x440d, 0x7c);
    ret |= sc485sl_write_reg(dev, 0x440e, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x440f, 0x44);
    ret |= sc485sl_write_reg(dev, 0x4412, 0x01);
    ret |= sc485sl_write_reg(dev, 0x4424, 0x01);
    ret |= sc485sl_write_reg(dev, 0x4509, 0x18);
    ret |= sc485sl_write_reg(dev, 0x450d, 0x0b);
    ret |= sc485sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc485sl_write_reg(dev, 0x480f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x4837, 0x1e);
    ret |= sc485sl_write_reg(dev, 0x5784, 0x10);
    ret |= sc485sl_write_reg(dev, 0x5785, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5787, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x5788, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5789, 0x04);
    ret |= sc485sl_write_reg(dev, 0x578a, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x578b, 0x08);
    ret |= sc485sl_write_reg(dev, 0x578c, 0x04);
    ret |= sc485sl_write_reg(dev, 0x578d, 0x40);
    ret |= sc485sl_write_reg(dev, 0x5790, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5791, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5792, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5793, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5794, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5795, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5799, 0x46);
    ret |= sc485sl_write_reg(dev, 0x579a, 0x77);
    ret |= sc485sl_write_reg(dev, 0x57a1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc485sl_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x57ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x57ad, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x5000, 0x20);
    ret |= sc485sl_write_reg(dev, 0x0100, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3221, 0x66);

    ret |= sc485sl_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc485sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC485SL_27MInput_MIPI_4lane_12bit_2688x1520_linear init success! ====\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc485sl_4lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc485sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc485sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x23b0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b1, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b2, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23b4, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23b6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b9, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23ba, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bb, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bf, 0x78);
    ret |= sc485sl_write_reg(dev, 0x23c0, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c1, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23c2, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23c4, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23c6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23c8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c9, 0x78);
    ret |= sc485sl_write_reg(dev, 0x3018, 0x7a);
    ret |= sc485sl_write_reg(dev, 0x3019, 0x00);
    ret |= sc485sl_write_reg(dev, 0x301e, 0xf0);
    ret |= sc485sl_write_reg(dev, 0x301f, 0x06);
    ret |= sc485sl_write_reg(dev, 0x302c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3031, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x3032, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3034, 0xee);
    ret |= sc485sl_write_reg(dev, 0x30b0, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3200, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3201, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3202, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3203, 0x28);
    ret |= sc485sl_write_reg(dev, 0x3204, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3205, 0x4f);
    ret |= sc485sl_write_reg(dev, 0x3206, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3207, 0xd7);
    ret |= sc485sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3209, 0x00);
    ret |= sc485sl_write_reg(dev, 0x320a, 0x05);
    ret |= sc485sl_write_reg(dev, 0x320b, 0xa0);
    ret |= sc485sl_write_reg(dev, 0x320c, 0x02);
    ret |= sc485sl_write_reg(dev, 0x320d, 0xee);
    ret |= sc485sl_write_reg(dev, 0x320e, 0x06);
    ret |= sc485sl_write_reg(dev, 0x320f, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3210, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3211, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3212, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3213, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3214, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3215, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3250, 0x00);
    ret |= sc485sl_write_reg(dev, 0x325f, 0x2d);
    ret |= sc485sl_write_reg(dev, 0x32d1, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x32e0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3301, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3304, 0x50);
    ret |= sc485sl_write_reg(dev, 0x3305, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3306, 0x58);
    ret |= sc485sl_write_reg(dev, 0x3308, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3309, 0x70);
    ret |= sc485sl_write_reg(dev, 0x330a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x330b, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x330d, 0x08);
    ret |= sc485sl_write_reg(dev, 0x330e, 0x18);
    ret |= sc485sl_write_reg(dev, 0x330f, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3310, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3314, 0x94);
    ret |= sc485sl_write_reg(dev, 0x331e, 0x39);
    ret |= sc485sl_write_reg(dev, 0x331f, 0x59);
    ret |= sc485sl_write_reg(dev, 0x3333, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3334, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3347, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x334c, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3364, 0x5e);
    ret |= sc485sl_write_reg(dev, 0x3393, 0x0e);
    ret |= sc485sl_write_reg(dev, 0x3394, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3395, 0x3c);
    ret |= sc485sl_write_reg(dev, 0x3399, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x339a, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x339b, 0x10);
    ret |= sc485sl_write_reg(dev, 0x339c, 0x52);
    ret |= sc485sl_write_reg(dev, 0x33ac, 0x10);
    ret |= sc485sl_write_reg(dev, 0x33ad, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x33ae, 0x40);
    ret |= sc485sl_write_reg(dev, 0x33af, 0x60);
    ret |= sc485sl_write_reg(dev, 0x33b0, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x33b2, 0x32);
    ret |= sc485sl_write_reg(dev, 0x33b3, 0x08);
    ret |= sc485sl_write_reg(dev, 0x33f8, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33f9, 0x58);
    ret |= sc485sl_write_reg(dev, 0x33fa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33fb, 0x58);
    ret |= sc485sl_write_reg(dev, 0x349f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x34a8, 0x08);
    ret |= sc485sl_write_reg(dev, 0x34a9, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34aa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ab, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x34ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ad, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x34f9, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3632, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3633, 0x41);
    ret |= sc485sl_write_reg(dev, 0x363a, 0x8c);
    ret |= sc485sl_write_reg(dev, 0x363b, 0x5d);
    ret |= sc485sl_write_reg(dev, 0x3670, 0x41);
    ret |= sc485sl_write_reg(dev, 0x3671, 0x42);
    ret |= sc485sl_write_reg(dev, 0x3672, 0x33);
    ret |= sc485sl_write_reg(dev, 0x3673, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3674, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3675, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3676, 0x18);
    ret |= sc485sl_write_reg(dev, 0x367e, 0x69);
    ret |= sc485sl_write_reg(dev, 0x367f, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3680, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3681, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3682, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3683, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3684, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3685, 0x81);
    ret |= sc485sl_write_reg(dev, 0x3686, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3687, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3688, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3689, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368a, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368b, 0x85);
    ret |= sc485sl_write_reg(dev, 0x368c, 0x88);
    ret |= sc485sl_write_reg(dev, 0x368d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x368e, 0x08);
    ret |= sc485sl_write_reg(dev, 0x368f, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3690, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3691, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3692, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3693, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3694, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3695, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3696, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3697, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3698, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3699, 0x04);
    ret |= sc485sl_write_reg(dev, 0x369a, 0x78);
    ret |= sc485sl_write_reg(dev, 0x36b9, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36ba, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36bb, 0x13);
    ret |= sc485sl_write_reg(dev, 0x36bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x36be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bf, 0x38);
    ret |= sc485sl_write_reg(dev, 0x36d0, 0x0d);
    ret |= sc485sl_write_reg(dev, 0x36d1, 0x20);
    ret |= sc485sl_write_reg(dev, 0x36ea, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x36eb, 0x45);
    ret |= sc485sl_write_reg(dev, 0x36ec, 0x69);
    ret |= sc485sl_write_reg(dev, 0x36ed, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x370f, 0x31);
    ret |= sc485sl_write_reg(dev, 0x3722, 0x03);
    ret |= sc485sl_write_reg(dev, 0x3724, 0xc1);
    ret |= sc485sl_write_reg(dev, 0x3727, 0x24);
    ret |= sc485sl_write_reg(dev, 0x3729, 0x84);
    ret |= sc485sl_write_reg(dev, 0x37b0, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b1, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b2, 0xf3);
    ret |= sc485sl_write_reg(dev, 0x37b3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b4, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37b5, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b6, 0x78);
    ret |= sc485sl_write_reg(dev, 0x37b7, 0xa4);
    ret |= sc485sl_write_reg(dev, 0x37b8, 0xf4);
    ret |= sc485sl_write_reg(dev, 0x37b9, 0xb4);
    ret |= sc485sl_write_reg(dev, 0x37ba, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bb, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x37bc, 0x4e);
    ret |= sc485sl_write_reg(dev, 0x37bd, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37be, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bf, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c0, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37c1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c2, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37c3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c4, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fa, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x37fb, 0x55);
    ret |= sc485sl_write_reg(dev, 0x37fc, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fd, 0x3a);
    ret |= sc485sl_write_reg(dev, 0x3900, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3901, 0x02);
    ret |= sc485sl_write_reg(dev, 0x3903, 0x60);
    ret |= sc485sl_write_reg(dev, 0x3907, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3908, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391a, 0x70);
    ret |= sc485sl_write_reg(dev, 0x391b, 0x46);
    ret |= sc485sl_write_reg(dev, 0x391c, 0x26);
    ret |= sc485sl_write_reg(dev, 0x391d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391f, 0x61);
    ret |= sc485sl_write_reg(dev, 0x3926, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3933, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3934, 0x07);
    ret |= sc485sl_write_reg(dev, 0x3935, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3936, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3937, 0x7b);
    ret |= sc485sl_write_reg(dev, 0x3938, 0x7d);
    ret |= sc485sl_write_reg(dev, 0x3939, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393b, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393f, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3940, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3941, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3942, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3943, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3944, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3945, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x3946, 0x80);
    ret |= sc485sl_write_reg(dev, 0x39c9, 0x60);
    ret |= sc485sl_write_reg(dev, 0x39dd, 0x00);
    ret |= sc485sl_write_reg(dev, 0x39de, 0x08);
    ret |= sc485sl_write_reg(dev, 0x39e7, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3e00, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e01, 0x63);
    ret |= sc485sl_write_reg(dev, 0x3e02, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc485sl_write_reg(dev, 0x3e08, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e16, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e17, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3e18, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e19, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x4402, 0x02);
    ret |= sc485sl_write_reg(dev, 0x4403, 0x08);
    ret |= sc485sl_write_reg(dev, 0x4404, 0x16);
    ret |= sc485sl_write_reg(dev, 0x4405, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x440c, 0x24);
    ret |= sc485sl_write_reg(dev, 0x440d, 0x24);
    ret |= sc485sl_write_reg(dev, 0x440e, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x440f, 0x2d);
    ret |= sc485sl_write_reg(dev, 0x4412, 0x01);
    ret |= sc485sl_write_reg(dev, 0x4424, 0x01);
    ret |= sc485sl_write_reg(dev, 0x450d, 0x08);
    ret |= sc485sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc485sl_write_reg(dev, 0x480f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x4837, 0x25);
    ret |= sc485sl_write_reg(dev, 0x5784, 0x10);
    ret |= sc485sl_write_reg(dev, 0x5785, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5787, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x5788, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x5789, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578a, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578b, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578c, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578d, 0x40);
    ret |= sc485sl_write_reg(dev, 0x5790, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5791, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5792, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5793, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5794, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5795, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5799, 0x46);
    ret |= sc485sl_write_reg(dev, 0x579a, 0x77);
    ret |= sc485sl_write_reg(dev, 0x57a1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc485sl_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x57ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x57ad, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x5000, 0x20);
    ret |= sc485sl_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc485sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC485SL_24MInput_MIPI_4lane_12bit_2560x1440_linear init success! ====\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc485sl_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc485sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc485sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x23b0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b1, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b2, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23b4, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23b6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b9, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23ba, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bb, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bf, 0x78);
    ret |= sc485sl_write_reg(dev, 0x23c0, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c1, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23c2, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23c4, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23c6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23c8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c9, 0x78);
    ret |= sc485sl_write_reg(dev, 0x3018, 0x3a);
    ret |= sc485sl_write_reg(dev, 0x3019, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x301e, 0xf0);
    ret |= sc485sl_write_reg(dev, 0x301f, 0x39);
    ret |= sc485sl_write_reg(dev, 0x302c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3031, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x3032, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3034, 0xee);
    ret |= sc485sl_write_reg(dev, 0x30b0, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3200, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3201, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3202, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3203, 0x28);
    ret |= sc485sl_write_reg(dev, 0x3204, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3205, 0x4f);
    ret |= sc485sl_write_reg(dev, 0x3206, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3207, 0xd7);
    ret |= sc485sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3209, 0x00);
    ret |= sc485sl_write_reg(dev, 0x320a, 0x05);
    ret |= sc485sl_write_reg(dev, 0x320b, 0xa0);
    ret |= sc485sl_write_reg(dev, 0x320c, 0x02);
    ret |= sc485sl_write_reg(dev, 0x320d, 0xee);
    ret |= sc485sl_write_reg(dev, 0x320e, 0x07);
    ret |= sc485sl_write_reg(dev, 0x320f, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3210, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3211, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3212, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3213, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3214, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3215, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3250, 0x00);
    ret |= sc485sl_write_reg(dev, 0x325f, 0x2d);
    ret |= sc485sl_write_reg(dev, 0x32d1, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x32e0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3301, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3304, 0x50);
    ret |= sc485sl_write_reg(dev, 0x3305, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3306, 0x58);
    ret |= sc485sl_write_reg(dev, 0x3308, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3309, 0x70);
    ret |= sc485sl_write_reg(dev, 0x330a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x330b, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x330d, 0x08);
    ret |= sc485sl_write_reg(dev, 0x330e, 0x18);
    ret |= sc485sl_write_reg(dev, 0x330f, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3310, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3314, 0x94);
    ret |= sc485sl_write_reg(dev, 0x331e, 0x39);
    ret |= sc485sl_write_reg(dev, 0x331f, 0x59);
    ret |= sc485sl_write_reg(dev, 0x3333, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3334, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3347, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x334c, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3364, 0x5e);
    ret |= sc485sl_write_reg(dev, 0x3393, 0x0e);
    ret |= sc485sl_write_reg(dev, 0x3394, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3395, 0x3c);
    ret |= sc485sl_write_reg(dev, 0x3399, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x339a, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x339b, 0x10);
    ret |= sc485sl_write_reg(dev, 0x339c, 0x52);
    ret |= sc485sl_write_reg(dev, 0x33ac, 0x10);
    ret |= sc485sl_write_reg(dev, 0x33ad, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x33ae, 0x40);
    ret |= sc485sl_write_reg(dev, 0x33af, 0x60);
    ret |= sc485sl_write_reg(dev, 0x33b0, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x33b2, 0x32);
    ret |= sc485sl_write_reg(dev, 0x33b3, 0x08);
    ret |= sc485sl_write_reg(dev, 0x33f8, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33f9, 0x58);
    ret |= sc485sl_write_reg(dev, 0x33fa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33fb, 0x58);
    ret |= sc485sl_write_reg(dev, 0x349f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x34a8, 0x08);
    ret |= sc485sl_write_reg(dev, 0x34a9, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34aa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ab, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x34ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ad, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x34f9, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3632, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3633, 0x41);
    ret |= sc485sl_write_reg(dev, 0x363a, 0x8c);
    ret |= sc485sl_write_reg(dev, 0x363b, 0x5d);
    ret |= sc485sl_write_reg(dev, 0x3670, 0x41);
    ret |= sc485sl_write_reg(dev, 0x3671, 0x42);
    ret |= sc485sl_write_reg(dev, 0x3672, 0x33);
    ret |= sc485sl_write_reg(dev, 0x3673, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3674, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3675, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3676, 0x18);
    ret |= sc485sl_write_reg(dev, 0x367e, 0x69);
    ret |= sc485sl_write_reg(dev, 0x367f, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3680, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3681, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3682, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3683, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3684, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3685, 0x81);
    ret |= sc485sl_write_reg(dev, 0x3686, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3687, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3688, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3689, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368a, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368b, 0x85);
    ret |= sc485sl_write_reg(dev, 0x368c, 0x88);
    ret |= sc485sl_write_reg(dev, 0x368d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x368e, 0x08);
    ret |= sc485sl_write_reg(dev, 0x368f, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3690, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3691, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3692, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3693, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3694, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3695, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3696, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3697, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3698, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3699, 0x04);
    ret |= sc485sl_write_reg(dev, 0x369a, 0x78);
    ret |= sc485sl_write_reg(dev, 0x36b9, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36ba, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36bb, 0x13);
    ret |= sc485sl_write_reg(dev, 0x36bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x36be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bf, 0x38);
    ret |= sc485sl_write_reg(dev, 0x36d0, 0x0d);
    ret |= sc485sl_write_reg(dev, 0x36d1, 0x20);
    ret |= sc485sl_write_reg(dev, 0x36ea, 0x09);
    ret |= sc485sl_write_reg(dev, 0x36eb, 0x4f);
    ret |= sc485sl_write_reg(dev, 0x36ec, 0x69);
    ret |= sc485sl_write_reg(dev, 0x36ed, 0xa8);
    ret |= sc485sl_write_reg(dev, 0x370f, 0x31);
    ret |= sc485sl_write_reg(dev, 0x3722, 0x03);
    ret |= sc485sl_write_reg(dev, 0x3724, 0xc1);
    ret |= sc485sl_write_reg(dev, 0x3727, 0x24);
    ret |= sc485sl_write_reg(dev, 0x3729, 0x84);
    ret |= sc485sl_write_reg(dev, 0x37b0, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b1, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b2, 0xf3);
    ret |= sc485sl_write_reg(dev, 0x37b3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b4, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37b5, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b6, 0x78);
    ret |= sc485sl_write_reg(dev, 0x37b7, 0xa4);
    ret |= sc485sl_write_reg(dev, 0x37b8, 0xf4);
    ret |= sc485sl_write_reg(dev, 0x37b9, 0xb4);
    ret |= sc485sl_write_reg(dev, 0x37ba, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bb, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x37bc, 0x4e);
    ret |= sc485sl_write_reg(dev, 0x37bd, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37be, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bf, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c0, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37c1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c2, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37c3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c4, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fa, 0x09);
    ret |= sc485sl_write_reg(dev, 0x37fb, 0x55);
    ret |= sc485sl_write_reg(dev, 0x37fc, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fd, 0x3a);
    ret |= sc485sl_write_reg(dev, 0x3900, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3901, 0x02);
    ret |= sc485sl_write_reg(dev, 0x3903, 0x60);
    ret |= sc485sl_write_reg(dev, 0x3907, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3908, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391a, 0x70);
    ret |= sc485sl_write_reg(dev, 0x391b, 0x46);
    ret |= sc485sl_write_reg(dev, 0x391c, 0x26);
    ret |= sc485sl_write_reg(dev, 0x391d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391f, 0x61);
    ret |= sc485sl_write_reg(dev, 0x3926, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3933, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3934, 0x07);
    ret |= sc485sl_write_reg(dev, 0x3935, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3936, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3937, 0x7b);
    ret |= sc485sl_write_reg(dev, 0x3938, 0x7d);
    ret |= sc485sl_write_reg(dev, 0x3939, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393b, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393f, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3940, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3941, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3942, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3943, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3944, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3945, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x3946, 0x80);
    ret |= sc485sl_write_reg(dev, 0x39c9, 0x60);
    ret |= sc485sl_write_reg(dev, 0x39dd, 0x00);
    ret |= sc485sl_write_reg(dev, 0x39de, 0x08);
    ret |= sc485sl_write_reg(dev, 0x39e7, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3e00, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e01, 0x70);
    ret |= sc485sl_write_reg(dev, 0x3e02, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc485sl_write_reg(dev, 0x3e08, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e16, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e17, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3e18, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e19, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x4402, 0x01);
    ret |= sc485sl_write_reg(dev, 0x4403, 0x05);
    ret |= sc485sl_write_reg(dev, 0x4404, 0x0d);
    ret |= sc485sl_write_reg(dev, 0x4405, 0x10);
    ret |= sc485sl_write_reg(dev, 0x440c, 0x15);
    ret |= sc485sl_write_reg(dev, 0x440d, 0x15);
    ret |= sc485sl_write_reg(dev, 0x440e, 0x10);
    ret |= sc485sl_write_reg(dev, 0x440f, 0x1a);
    ret |= sc485sl_write_reg(dev, 0x4412, 0x01);
    ret |= sc485sl_write_reg(dev, 0x4424, 0x01);
    ret |= sc485sl_write_reg(dev, 0x450d, 0x08);
    ret |= sc485sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc485sl_write_reg(dev, 0x480f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x4837, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x5784, 0x10);
    ret |= sc485sl_write_reg(dev, 0x5785, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5787, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x5788, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x5789, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578a, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578b, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578c, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578d, 0x40);
    ret |= sc485sl_write_reg(dev, 0x5790, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5791, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5792, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5793, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5794, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5795, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5799, 0x46);
    ret |= sc485sl_write_reg(dev, 0x579a, 0x77);
    ret |= sc485sl_write_reg(dev, 0x57a1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc485sl_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x57ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x57ad, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x53);
    ret |= sc485sl_write_reg(dev, 0x5000, 0x20);
    ret |= sc485sl_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc485sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC485SL_24MInput_MIPI_2lane_12bit_2560x1440_linear init success! ====\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_void sc485sl_slave_init(xmedia_u32 dev)
{
    sc485sl_write_reg(dev, 0x0100, 0x00);
    sc485sl_write_reg(dev, 0x3222, 0x01);
    sc485sl_write_reg(dev, 0x3224, 0xd2);
    sc485sl_write_reg(dev, 0x3230, 0x00);
    sc485sl_write_reg(dev, 0x3231, 0x64);
    sc485sl_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("====== SC485sl slave setting init success!=======\n");
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc485sl_4lane_wdr_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc485sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc485sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x23b0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b1, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b2, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23b4, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23b5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23b6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23b8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23b9, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23ba, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bb, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23bf, 0x78);
    ret |= sc485sl_write_reg(dev, 0x23c0, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c1, 0x00);
    ret |= sc485sl_write_reg(dev, 0x23c2, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c3, 0x18);
    ret |= sc485sl_write_reg(dev, 0x23c4, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c5, 0x38);
    ret |= sc485sl_write_reg(dev, 0x23c6, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c7, 0x08);
    ret |= sc485sl_write_reg(dev, 0x23c8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x23c9, 0x78);
    ret |= sc485sl_write_reg(dev, 0x3018, 0x7a);
    ret |= sc485sl_write_reg(dev, 0x301e, 0xf0);
    ret |= sc485sl_write_reg(dev, 0x301f, 0x3c);
    ret |= sc485sl_write_reg(dev, 0x302c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3032, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3034, 0xee);
    ret |= sc485sl_write_reg(dev, 0x30b0, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3200, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3201, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3202, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3203, 0x28);
    ret |= sc485sl_write_reg(dev, 0x3204, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3205, 0x4f);
    ret |= sc485sl_write_reg(dev, 0x3206, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3207, 0xd7);
    ret |= sc485sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3209, 0x00);
    ret |= sc485sl_write_reg(dev, 0x320a, 0x05);
    ret |= sc485sl_write_reg(dev, 0x320b, 0xa0);
    ret |= sc485sl_write_reg(dev, 0x320c, 0x02);
    ret |= sc485sl_write_reg(dev, 0x320d, 0xee);
    ret |= sc485sl_write_reg(dev, 0x320e, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x320f, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3210, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3211, 0x07);
    ret |= sc485sl_write_reg(dev, 0x3212, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3213, 0x07);
    ret |= sc485sl_write_reg(dev, 0x3214, 0x11);
    ret |= sc485sl_write_reg(dev, 0x3215, 0x11);
    // ret |= sc485sl_write_reg(dev, 0x3250, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3250, 0xff);
    ret |= sc485sl_write_reg(dev, 0x3432, 0x72);
    ret |= sc485sl_write_reg(dev, 0x325f, 0x2d);
    ret |= sc485sl_write_reg(dev, 0x3281, 0x01);
    ret |= sc485sl_write_reg(dev, 0x32d1, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x32e0, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3301, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x3304, 0x50);
    ret |= sc485sl_write_reg(dev, 0x3305, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3306, 0x58);
    ret |= sc485sl_write_reg(dev, 0x3308, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3309, 0x70);
    ret |= sc485sl_write_reg(dev, 0x330a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x330b, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x330d, 0x08);
    ret |= sc485sl_write_reg(dev, 0x330e, 0x18);
    ret |= sc485sl_write_reg(dev, 0x330f, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3310, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3314, 0x94);
    ret |= sc485sl_write_reg(dev, 0x331e, 0x39);
    ret |= sc485sl_write_reg(dev, 0x331f, 0x59);
    ret |= sc485sl_write_reg(dev, 0x3333, 0x10);
    ret |= sc485sl_write_reg(dev, 0x3334, 0x40);
    ret |= sc485sl_write_reg(dev, 0x3347, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x334c, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3364, 0x5e);
    ret |= sc485sl_write_reg(dev, 0x338f, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3393, 0x0e);
    ret |= sc485sl_write_reg(dev, 0x3394, 0x2c);
    ret |= sc485sl_write_reg(dev, 0x3395, 0x3c);
    ret |= sc485sl_write_reg(dev, 0x3399, 0x0a);
    ret |= sc485sl_write_reg(dev, 0x339a, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x339b, 0x10);
    ret |= sc485sl_write_reg(dev, 0x339c, 0x52);
    ret |= sc485sl_write_reg(dev, 0x33ac, 0x10);
    ret |= sc485sl_write_reg(dev, 0x33ad, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x33ae, 0x40);
    ret |= sc485sl_write_reg(dev, 0x33af, 0x60);
    ret |= sc485sl_write_reg(dev, 0x33b0, 0x0f);
    ret |= sc485sl_write_reg(dev, 0x33b2, 0x32);
    ret |= sc485sl_write_reg(dev, 0x33b3, 0x08);
    ret |= sc485sl_write_reg(dev, 0x33f8, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33f9, 0x58);
    ret |= sc485sl_write_reg(dev, 0x33fa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x33fb, 0x58);
    ret |= sc485sl_write_reg(dev, 0x349f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x34a8, 0x08);
    ret |= sc485sl_write_reg(dev, 0x34a9, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34aa, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ab, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x34ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x34ad, 0xb8);
    ret |= sc485sl_write_reg(dev, 0x34f9, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3632, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3633, 0x41);
    ret |= sc485sl_write_reg(dev, 0x363a, 0x8c);
    ret |= sc485sl_write_reg(dev, 0x363b, 0x5d);
    ret |= sc485sl_write_reg(dev, 0x3670, 0x41);
    ret |= sc485sl_write_reg(dev, 0x3671, 0x42);
    ret |= sc485sl_write_reg(dev, 0x3672, 0x33);
    ret |= sc485sl_write_reg(dev, 0x3673, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3674, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3675, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3676, 0x18);
    ret |= sc485sl_write_reg(dev, 0x367e, 0x69);
    ret |= sc485sl_write_reg(dev, 0x367f, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3680, 0x6d);
    ret |= sc485sl_write_reg(dev, 0x3681, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3682, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3683, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3684, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3685, 0x81);
    ret |= sc485sl_write_reg(dev, 0x3686, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3687, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3688, 0x84);
    ret |= sc485sl_write_reg(dev, 0x3689, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368a, 0x83);
    ret |= sc485sl_write_reg(dev, 0x368b, 0x85);
    ret |= sc485sl_write_reg(dev, 0x368c, 0x88);
    ret |= sc485sl_write_reg(dev, 0x368d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x368e, 0x08);
    ret |= sc485sl_write_reg(dev, 0x368f, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3690, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3691, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3692, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3693, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3694, 0x08);
    ret |= sc485sl_write_reg(dev, 0x3695, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3696, 0x18);
    ret |= sc485sl_write_reg(dev, 0x3697, 0x04);
    ret |= sc485sl_write_reg(dev, 0x3698, 0x38);
    ret |= sc485sl_write_reg(dev, 0x3699, 0x04);
    ret |= sc485sl_write_reg(dev, 0x369a, 0x78);
    ret |= sc485sl_write_reg(dev, 0x36b9, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36ba, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x36bb, 0x13);
    ret |= sc485sl_write_reg(dev, 0x36bc, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bd, 0x08);
    ret |= sc485sl_write_reg(dev, 0x36be, 0x04);
    ret |= sc485sl_write_reg(dev, 0x36bf, 0x38);
    ret |= sc485sl_write_reg(dev, 0x36d0, 0x0d);
    ret |= sc485sl_write_reg(dev, 0x36d1, 0x20);
    ret |= sc485sl_write_reg(dev, 0x36ea, 0x14);
    ret |= sc485sl_write_reg(dev, 0x36eb, 0x45);
    ret |= sc485sl_write_reg(dev, 0x36ec, 0x69);
    ret |= sc485sl_write_reg(dev, 0x36ed, 0x98);
    ret |= sc485sl_write_reg(dev, 0x370f, 0x31);
    ret |= sc485sl_write_reg(dev, 0x3722, 0x03);
    ret |= sc485sl_write_reg(dev, 0x3724, 0xc1);
    ret |= sc485sl_write_reg(dev, 0x3727, 0x24);
    ret |= sc485sl_write_reg(dev, 0x3729, 0x84);
    ret |= sc485sl_write_reg(dev, 0x37b0, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b1, 0x03);
    ret |= sc485sl_write_reg(dev, 0x37b2, 0xf3);
    ret |= sc485sl_write_reg(dev, 0x37b3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b4, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37b5, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37b6, 0x78);
    ret |= sc485sl_write_reg(dev, 0x37b7, 0xa4);
    ret |= sc485sl_write_reg(dev, 0x37b8, 0xf4);
    ret |= sc485sl_write_reg(dev, 0x37b9, 0xb4);
    ret |= sc485sl_write_reg(dev, 0x37ba, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bb, 0x1c);
    ret |= sc485sl_write_reg(dev, 0x37bc, 0x4e);
    ret |= sc485sl_write_reg(dev, 0x37bd, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37be, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37bf, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c0, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37c1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c2, 0x08);
    ret |= sc485sl_write_reg(dev, 0x37c3, 0x04);
    ret |= sc485sl_write_reg(dev, 0x37c4, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fa, 0x10);
    ret |= sc485sl_write_reg(dev, 0x37fb, 0x55);
    ret |= sc485sl_write_reg(dev, 0x37fc, 0x18);
    ret |= sc485sl_write_reg(dev, 0x37fd, 0x1a);
    ret |= sc485sl_write_reg(dev, 0x3900, 0x05);
    ret |= sc485sl_write_reg(dev, 0x3901, 0x02);
    ret |= sc485sl_write_reg(dev, 0x3903, 0x60);
    ret |= sc485sl_write_reg(dev, 0x3907, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3908, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391a, 0x70);
    ret |= sc485sl_write_reg(dev, 0x391b, 0x46);
    ret |= sc485sl_write_reg(dev, 0x391c, 0x26);
    ret |= sc485sl_write_reg(dev, 0x391d, 0x00);
    ret |= sc485sl_write_reg(dev, 0x391f, 0x61);
    ret |= sc485sl_write_reg(dev, 0x3926, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3933, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3934, 0x07);
    ret |= sc485sl_write_reg(dev, 0x3935, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3936, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3937, 0x7b);
    ret |= sc485sl_write_reg(dev, 0x3938, 0x7d);
    ret |= sc485sl_write_reg(dev, 0x3939, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393a, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393b, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393c, 0x00);
    ret |= sc485sl_write_reg(dev, 0x393f, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3940, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3941, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3942, 0x20);
    ret |= sc485sl_write_reg(dev, 0x3943, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3944, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3945, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x3946, 0x80);
    ret |= sc485sl_write_reg(dev, 0x39c9, 0x60);
    ret |= sc485sl_write_reg(dev, 0x39dd, 0x00);
    ret |= sc485sl_write_reg(dev, 0x39de, 0x08);
    ret |= sc485sl_write_reg(dev, 0x39e7, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e8, 0x04);
    ret |= sc485sl_write_reg(dev, 0x39e9, 0x80);
    ret |= sc485sl_write_reg(dev, 0x3e00, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e01, 0xb9);
    ret |= sc485sl_write_reg(dev, 0x3e02, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc485sl_write_reg(dev, 0x3e04, 0x0b);
    ret |= sc485sl_write_reg(dev, 0x3e05, 0x90);
    ret |= sc485sl_write_reg(dev, 0x3e08, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e16, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e17, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3e18, 0x01);
    ret |= sc485sl_write_reg(dev, 0x3e19, 0xe2);
    ret |= sc485sl_write_reg(dev, 0x3e23, 0x00);
    ret |= sc485sl_write_reg(dev, 0x3e24, 0xc4);
    ret |= sc485sl_write_reg(dev, 0x4402, 0x02);
    ret |= sc485sl_write_reg(dev, 0x4403, 0x08);
    ret |= sc485sl_write_reg(dev, 0x4404, 0x16);
    ret |= sc485sl_write_reg(dev, 0x4405, 0x1d);
    ret |= sc485sl_write_reg(dev, 0x440c, 0x24);
    ret |= sc485sl_write_reg(dev, 0x440d, 0x24);
    ret |= sc485sl_write_reg(dev, 0x440e, 0x1b);
    ret |= sc485sl_write_reg(dev, 0x440f, 0x2d);
    ret |= sc485sl_write_reg(dev, 0x4412, 0x01);
    ret |= sc485sl_write_reg(dev, 0x4424, 0x01);
    ret |= sc485sl_write_reg(dev, 0x450d, 0x08);
    ret |= sc485sl_write_reg(dev, 0x4800, 0x24);//与0x3250共同决定时钟连续与非连续模式
    ret |= sc485sl_write_reg(dev, 0x4816, 0x21);
    ret |= sc485sl_write_reg(dev, 0x480f, 0x03);
    ret |= sc485sl_write_reg(dev, 0x4837, 0x1e);
    ret |= sc485sl_write_reg(dev, 0x5406, 0x03);
    ret |= sc485sl_write_reg(dev, 0x5407, 0x3c);
    ret |= sc485sl_write_reg(dev, 0x5784, 0x10);
    ret |= sc485sl_write_reg(dev, 0x5785, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5787, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x5788, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x5789, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578a, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578b, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578c, 0x0c);
    ret |= sc485sl_write_reg(dev, 0x578d, 0x40);
    ret |= sc485sl_write_reg(dev, 0x5790, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5791, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5792, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5793, 0x08);
    ret |= sc485sl_write_reg(dev, 0x5794, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5795, 0x04);
    ret |= sc485sl_write_reg(dev, 0x5799, 0x46);
    ret |= sc485sl_write_reg(dev, 0x579a, 0x77);
    ret |= sc485sl_write_reg(dev, 0x57a1, 0x04);
    ret |= sc485sl_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc485sl_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc485sl_write_reg(dev, 0x57ac, 0x00);
    ret |= sc485sl_write_reg(dev, 0x57ad, 0x00);
    ret |= sc485sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x37f9, 0x24);
    ret |= sc485sl_write_reg(dev, 0x5000, 0x20);
    ret |= sc485sl_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc485sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC485SL_27MInput_MIPI_4lane_10bit_2560x1440_wdr init success! =======\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC485SL_2688_1520_12BIT_4LANE_LINEAR_MODE:
            ret = sc485sl_4lane_linear_2688x1520_init(dev);
            break;
        case SC485SL_2560_1440_12BIT_4LANE_LINEAR_MODE:
            ret = sc485sl_4lane_linear_2560x1440_init(dev);
            break;
        case SC485SL_2560_1440_12BIT_2LANE_LINEAR_MODE:
            ret = sc485sl_2lane_linear_2560x1440_init(dev);
            break;
        case SC485SL_2560_1440_10BIT_4LANE_WDR_MODE:
            ret = sc485sl_4lane_wdr_2560x1440_init(dev);
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        sc485sl_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc485sl_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc485sl_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc485sl_i2c_fd[dev]);
        g_sc485sl_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc485sl_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc485sl_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc485sl_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc485sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc485sl_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc485sl_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc485sl_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC485SL_DATA_BYTE];

    if (g_sc485sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc485sl_i2c_fd[dev], g_sc485sl_i2c_addr[dev], addr, SC485SL_ADDR_BYTE, buf, SC485SL_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC485SL_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc485sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc485sl_i2c_fd[dev], g_sc485sl_i2c_addr[dev], buf, SC485SL_ADDR_BYTE + SC485SL_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc485sl_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{

    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc485sl_read_reg(dev, SC485SL_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc485sl_write_reg(dev, SC485SL_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc485sl_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
