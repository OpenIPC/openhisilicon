#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc285sl.h"
#include "sc285sl_ctrl.h"

static xmedia_s32 g_sc285sl_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_sc285sl_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define SC285SL_REG_ADDR_MIRROR_FLIP 0x3221

/*
 * 函数功能: sc285sl MIPI 2lane 1080p线性模式初始化序列 - 1920x1080x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc285sl_4lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc285sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc285sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc285sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc285sl_write_reg(dev, 0x37f9, 0x80);
    ret |= sc285sl_write_reg(dev, 0x23b0, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23b1, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23b2, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23b3, 0x18);
    ret |= sc285sl_write_reg(dev, 0x23b4, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23b5, 0x38);
    ret |= sc285sl_write_reg(dev, 0x23b6, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23b7, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23b8, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23b9, 0x18);
    ret |= sc285sl_write_reg(dev, 0x23ba, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23bb, 0x38);
    ret |= sc285sl_write_reg(dev, 0x23bc, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23bd, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23be, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23bf, 0x78);
    ret |= sc285sl_write_reg(dev, 0x23c0, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c1, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23c2, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c3, 0x18);
    ret |= sc285sl_write_reg(dev, 0x23c4, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c5, 0x38);
    ret |= sc285sl_write_reg(dev, 0x23c6, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c7, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23c8, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c9, 0x78);
    ret |= sc285sl_write_reg(dev, 0x3018, 0x7a);
    ret |= sc285sl_write_reg(dev, 0x301c, 0x70);
    ret |= sc285sl_write_reg(dev, 0x301e, 0xf0);
    ret |= sc285sl_write_reg(dev, 0x301f, 0x02);
    ret |= sc285sl_write_reg(dev, 0x302b, 0x6f);
    ret |= sc285sl_write_reg(dev, 0x3031, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x3037, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3058, 0x21);
    ret |= sc285sl_write_reg(dev, 0x3059, 0x43);
    ret |= sc285sl_write_reg(dev, 0x30b0, 0x00);
    ret |= sc285sl_write_reg(dev, 0x30b8, 0x44);
    ret |= sc285sl_write_reg(dev, 0x3107, 0xbb);
    ret |= sc285sl_write_reg(dev, 0x3108, 0x98);
    ret |= sc285sl_write_reg(dev, 0x3204, 0x07);
    ret |= sc285sl_write_reg(dev, 0x3205, 0x8f);
    ret |= sc285sl_write_reg(dev, 0x3206, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3207, 0x47);
    ret |= sc285sl_write_reg(dev, 0x3208, 0x07);
    ret |= sc285sl_write_reg(dev, 0x3209, 0x80);
    ret |= sc285sl_write_reg(dev, 0x320a, 0x04);
    ret |= sc285sl_write_reg(dev, 0x320b, 0x38);
    ret |= sc285sl_write_reg(dev, 0x320c, 0x06);
    ret |= sc285sl_write_reg(dev, 0x320d, 0x40);
    ret |= sc285sl_write_reg(dev, 0x320e, 0x09);
    ret |= sc285sl_write_reg(dev, 0x320f, 0x10);
    ret |= sc285sl_write_reg(dev, 0x3210, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3211, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3212, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3213, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3214, 0x11);
    ret |= sc285sl_write_reg(dev, 0x3215, 0x11);
    ret |= sc285sl_write_reg(dev, 0x3220, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3223, 0xc0);
    ret |= sc285sl_write_reg(dev, 0x3250, 0x00);
    ret |= sc285sl_write_reg(dev, 0x325f, 0xa0);
    ret |= sc285sl_write_reg(dev, 0x3271, 0x10);
    ret |= sc285sl_write_reg(dev, 0x3273, 0x13);
    ret |= sc285sl_write_reg(dev, 0x32d1, 0x90);
    ret |= sc285sl_write_reg(dev, 0x32e0, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3301, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x3304, 0x60);
    ret |= sc285sl_write_reg(dev, 0x3305, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3306, 0xf8);
    ret |= sc285sl_write_reg(dev, 0x3309, 0x80);
    ret |= sc285sl_write_reg(dev, 0x330a, 0x02);
    ret |= sc285sl_write_reg(dev, 0x330b, 0x48);
    ret |= sc285sl_write_reg(dev, 0x331c, 0x01);
    ret |= sc285sl_write_reg(dev, 0x331e, 0x49);
    ret |= sc285sl_write_reg(dev, 0x331f, 0x69);
    ret |= sc285sl_write_reg(dev, 0x3320, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x3326, 0x14);
    ret |= sc285sl_write_reg(dev, 0x3328, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3329, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3353, 0x14);
    ret |= sc285sl_write_reg(dev, 0x3364, 0x5e);
    ret |= sc285sl_write_reg(dev, 0x3366, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3393, 0x14);
    ret |= sc285sl_write_reg(dev, 0x3394, 0x2c);
    ret |= sc285sl_write_reg(dev, 0x3395, 0x3c);
    ret |= sc285sl_write_reg(dev, 0x3399, 0x12);
    ret |= sc285sl_write_reg(dev, 0x339a, 0x14);
    ret |= sc285sl_write_reg(dev, 0x339b, 0x20);
    ret |= sc285sl_write_reg(dev, 0x339c, 0x50);
    ret |= sc285sl_write_reg(dev, 0x33ac, 0x18);
    ret |= sc285sl_write_reg(dev, 0x33ad, 0x2c);
    ret |= sc285sl_write_reg(dev, 0x33ae, 0x40);
    ret |= sc285sl_write_reg(dev, 0x33af, 0x60);
    ret |= sc285sl_write_reg(dev, 0x33b0, 0x0f);
    ret |= sc285sl_write_reg(dev, 0x33b2, 0x50);
    ret |= sc285sl_write_reg(dev, 0x33b3, 0x18);
    ret |= sc285sl_write_reg(dev, 0x33f8, 0x01);
    ret |= sc285sl_write_reg(dev, 0x33f9, 0x28);
    ret |= sc285sl_write_reg(dev, 0x33fa, 0x01);
    ret |= sc285sl_write_reg(dev, 0x33fb, 0x58);
    ret |= sc285sl_write_reg(dev, 0x3428, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3429, 0x04);
    ret |= sc285sl_write_reg(dev, 0x342a, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3447, 0x11);
    ret |= sc285sl_write_reg(dev, 0x349f, 0x03);
    ret |= sc285sl_write_reg(dev, 0x34a5, 0x01);
    ret |= sc285sl_write_reg(dev, 0x34a8, 0x10);
    ret |= sc285sl_write_reg(dev, 0x34a9, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x34aa, 0x02);
    ret |= sc285sl_write_reg(dev, 0x34ab, 0x78);
    ret |= sc285sl_write_reg(dev, 0x34ac, 0x02);
    ret |= sc285sl_write_reg(dev, 0x34ad, 0xa8);
    ret |= sc285sl_write_reg(dev, 0x34ba, 0x04);
    ret |= sc285sl_write_reg(dev, 0x34bb, 0x0f);
    ret |= sc285sl_write_reg(dev, 0x34f9, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3616, 0xbc);
    ret |= sc285sl_write_reg(dev, 0x3622, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3623, 0x38);
    ret |= sc285sl_write_reg(dev, 0x3624, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3625, 0x78);
    ret |= sc285sl_write_reg(dev, 0x3626, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3627, 0x38);
    ret |= sc285sl_write_reg(dev, 0x362f, 0x10);
    ret |= sc285sl_write_reg(dev, 0x3632, 0x64);
    ret |= sc285sl_write_reg(dev, 0x3633, 0x44);
    ret |= sc285sl_write_reg(dev, 0x3634, 0x24);
    ret |= sc285sl_write_reg(dev, 0x3637, 0xdc);
    ret |= sc285sl_write_reg(dev, 0x363d, 0x24);
    ret |= sc285sl_write_reg(dev, 0x3670, 0x41);
    ret |= sc285sl_write_reg(dev, 0x3671, 0x42);
    ret |= sc285sl_write_reg(dev, 0x3672, 0x33);
    ret |= sc285sl_write_reg(dev, 0x3673, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3674, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3675, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3676, 0x18);
    ret |= sc285sl_write_reg(dev, 0x367e, 0x44);
    ret |= sc285sl_write_reg(dev, 0x367f, 0x44);
    ret |= sc285sl_write_reg(dev, 0x3680, 0x34);
    ret |= sc285sl_write_reg(dev, 0x3681, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3682, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3683, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3684, 0x18);
    ret |= sc285sl_write_reg(dev, 0x3685, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3686, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3687, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3688, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3689, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368a, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368b, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368c, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368d, 0x00);
    ret |= sc285sl_write_reg(dev, 0x368e, 0x08);
    ret |= sc285sl_write_reg(dev, 0x368f, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3690, 0x18);
    ret |= sc285sl_write_reg(dev, 0x3691, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3692, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3693, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3694, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3695, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3696, 0x18);
    ret |= sc285sl_write_reg(dev, 0x3697, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3698, 0x38);
    ret |= sc285sl_write_reg(dev, 0x3699, 0x04);
    ret |= sc285sl_write_reg(dev, 0x369a, 0x78);
    ret |= sc285sl_write_reg(dev, 0x36b9, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36ba, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36bb, 0x23);
    ret |= sc285sl_write_reg(dev, 0x36bc, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36bd, 0x08);
    ret |= sc285sl_write_reg(dev, 0x36be, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36bf, 0x38);
    ret |= sc285sl_write_reg(dev, 0x36d0, 0x09);
    ret |= sc285sl_write_reg(dev, 0x36d1, 0x30);
    ret |= sc285sl_write_reg(dev, 0x36d2, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36d3, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36d4, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36d5, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36d6, 0x00);
    ret |= sc285sl_write_reg(dev, 0x36d7, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36d8, 0x18);
    ret |= sc285sl_write_reg(dev, 0x36e0, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36e1, 0x43);
    ret |= sc285sl_write_reg(dev, 0x36e2, 0x34);
    ret |= sc285sl_write_reg(dev, 0x36ea, 0x21);
    ret |= sc285sl_write_reg(dev, 0x36eb, 0x47);
    ret |= sc285sl_write_reg(dev, 0x36ec, 0x53);
    ret |= sc285sl_write_reg(dev, 0x36ed, 0x38);
    ret |= sc285sl_write_reg(dev, 0x370f, 0x71);
    ret |= sc285sl_write_reg(dev, 0x3722, 0x03);
    ret |= sc285sl_write_reg(dev, 0x3724, 0xb0);
    ret |= sc285sl_write_reg(dev, 0x3727, 0x20);
    ret |= sc285sl_write_reg(dev, 0x3729, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37b0, 0x03);
    ret |= sc285sl_write_reg(dev, 0x37b1, 0x03);
    ret |= sc285sl_write_reg(dev, 0x37b2, 0x33);
    ret |= sc285sl_write_reg(dev, 0x37b3, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37b4, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37b5, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37b6, 0x78);
    ret |= sc285sl_write_reg(dev, 0x37b7, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37b8, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37b9, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37ba, 0x84);
    ret |= sc285sl_write_reg(dev, 0x37bb, 0x88);
    ret |= sc285sl_write_reg(dev, 0x37bc, 0x98);
    ret |= sc285sl_write_reg(dev, 0x37bd, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37be, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37bf, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37c0, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37c1, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c2, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37c3, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c4, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37c5, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c6, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37c7, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c8, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37c9, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37ca, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37cb, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37cc, 0xa8);
    ret |= sc285sl_write_reg(dev, 0x37cd, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37ce, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37cf, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37d6, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37d7, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37d8, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37d9, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37da, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37db, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37dc, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37dd, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37de, 0x38);
    ret |= sc285sl_write_reg(dev, 0x37df, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37e0, 0x78);
    ret |= sc285sl_write_reg(dev, 0x37e1, 0x28);
    ret |= sc285sl_write_reg(dev, 0x37e2, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e3, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e4, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e5, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e6, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37e7, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37e8, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37e9, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37ea, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37eb, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37ec, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37ed, 0x38);
    ret |= sc285sl_write_reg(dev, 0x37ee, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37ef, 0x78);
    ret |= sc285sl_write_reg(dev, 0x37fa, 0x21);
    ret |= sc285sl_write_reg(dev, 0x37fb, 0x65);
    ret |= sc285sl_write_reg(dev, 0x37fc, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37fd, 0x35);
    ret |= sc285sl_write_reg(dev, 0x3900, 0x05);
    ret |= sc285sl_write_reg(dev, 0x3903, 0x40);
    ret |= sc285sl_write_reg(dev, 0x3907, 0x01);
    ret |= sc285sl_write_reg(dev, 0x3908, 0x00);
    ret |= sc285sl_write_reg(dev, 0x391a, 0x80);
    ret |= sc285sl_write_reg(dev, 0x391b, 0x60);
    ret |= sc285sl_write_reg(dev, 0x391c, 0x33);
    ret |= sc285sl_write_reg(dev, 0x391d, 0x00);
    ret |= sc285sl_write_reg(dev, 0x391f, 0x41);
    ret |= sc285sl_write_reg(dev, 0x3926, 0xe2);
    ret |= sc285sl_write_reg(dev, 0x3933, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3934, 0x02);
    ret |= sc285sl_write_reg(dev, 0x3935, 0x01);
    ret |= sc285sl_write_reg(dev, 0x3936, 0x30);
    ret |= sc285sl_write_reg(dev, 0x3937, 0x53);
    ret |= sc285sl_write_reg(dev, 0x3938, 0x52);
    ret |= sc285sl_write_reg(dev, 0x3939, 0x00);
    ret |= sc285sl_write_reg(dev, 0x393a, 0x00);
    ret |= sc285sl_write_reg(dev, 0x393b, 0x00);
    ret |= sc285sl_write_reg(dev, 0x393c, 0x40);
    ret |= sc285sl_write_reg(dev, 0x39c9, 0x30);
    ret |= sc285sl_write_reg(dev, 0x39dd, 0x00);
    ret |= sc285sl_write_reg(dev, 0x39de, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x39e7, 0x08);
    ret |= sc285sl_write_reg(dev, 0x39e8, 0x08);
    ret |= sc285sl_write_reg(dev, 0x39e9, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3e00, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3e01, 0x90);
    ret |= sc285sl_write_reg(dev, 0x3e02, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc285sl_write_reg(dev, 0x3e06, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3e07, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3e08, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3e09, 0x20);
    ret |= sc285sl_write_reg(dev, 0x3e16, 0x02);
    ret |= sc285sl_write_reg(dev, 0x3e17, 0xb3);
    ret |= sc285sl_write_reg(dev, 0x3e18, 0x01);
    ret |= sc285sl_write_reg(dev, 0x3e19, 0x59);
    ret |= sc285sl_write_reg(dev, 0x4509, 0x1e);
    ret |= sc285sl_write_reg(dev, 0x450d, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc285sl_write_reg(dev, 0x480f, 0x03);
    ret |= sc285sl_write_reg(dev, 0x4837, 0x18);
    ret |= sc285sl_write_reg(dev, 0x5011, 0x01);
    ret |= sc285sl_write_reg(dev, 0x530d, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5310, 0x01);
    ret |= sc285sl_write_reg(dev, 0x5311, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5312, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5314, 0x07);
    ret |= sc285sl_write_reg(dev, 0x5315, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5316, 0x0b);
    ret |= sc285sl_write_reg(dev, 0x5318, 0x0e);
    ret |= sc285sl_write_reg(dev, 0x531a, 0x11);
    ret |= sc285sl_write_reg(dev, 0x531c, 0x14);
    ret |= sc285sl_write_reg(dev, 0x531d, 0x10);
    ret |= sc285sl_write_reg(dev, 0x531e, 0x1a);
    ret |= sc285sl_write_reg(dev, 0x531f, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5400, 0x01);
    ret |= sc285sl_write_reg(dev, 0x5404, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x5405, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5784, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5785, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5787, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x5788, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x5789, 0x08);
    ret |= sc285sl_write_reg(dev, 0x578a, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x578b, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x578c, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5790, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5791, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5792, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5793, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5794, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5795, 0x04);
    ret |= sc285sl_write_reg(dev, 0x57ac, 0x00);
    ret |= sc285sl_write_reg(dev, 0x57ad, 0x00);
    ret |= sc285sl_write_reg(dev, 0x36e9, 0x20);
    ret |= sc285sl_write_reg(dev, 0x37f9, 0x20);
    ret |= sc285sl_write_reg(dev, 0x4402, 0x03);//置于0100前
    ret |= sc285sl_write_reg(dev, 0x4403, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x4404, 0x22);
    ret |= sc285sl_write_reg(dev, 0x4405, 0x2c);
    ret |= sc285sl_write_reg(dev, 0x440c, 0x38);
    ret |= sc285sl_write_reg(dev, 0x440d, 0x38);
    ret |= sc285sl_write_reg(dev, 0x440e, 0x2a);
    ret |= sc285sl_write_reg(dev, 0x440f, 0x47);
    ret |= sc285sl_write_reg(dev, 0x4412, 0x01);
    ret |= sc285sl_write_reg(dev, 0x4406, 0x00);
    ret |= sc285sl_write_reg(dev, 0x4407, 0x7e);
    ret |= sc285sl_write_reg(dev, 0x4424, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc285sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC285SL_27MInput_MIPI_4lane_12bit_1920x1080_30fps init success!========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc285sl_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc285sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc285sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc285sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc285sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc285sl_write_reg(dev, 0x37f9, 0x80);
    ret |= sc285sl_write_reg(dev, 0x23b0, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23b1, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23b2, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23b3, 0x18);
    ret |= sc285sl_write_reg(dev, 0x23b4, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23b5, 0x38);
    ret |= sc285sl_write_reg(dev, 0x23b6, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23b7, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23b8, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23b9, 0x18);
    ret |= sc285sl_write_reg(dev, 0x23ba, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23bb, 0x38);
    ret |= sc285sl_write_reg(dev, 0x23bc, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23bd, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23be, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23bf, 0x78);
    ret |= sc285sl_write_reg(dev, 0x23c0, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c1, 0x00);
    ret |= sc285sl_write_reg(dev, 0x23c2, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c3, 0x18);
    ret |= sc285sl_write_reg(dev, 0x23c4, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c5, 0x38);
    ret |= sc285sl_write_reg(dev, 0x23c6, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c7, 0x08);
    ret |= sc285sl_write_reg(dev, 0x23c8, 0x04);
    ret |= sc285sl_write_reg(dev, 0x23c9, 0x78);
    ret |= sc285sl_write_reg(dev, 0x3018, 0x3a);
    ret |= sc285sl_write_reg(dev, 0x3019, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x301c, 0x70);
    ret |= sc285sl_write_reg(dev, 0x301e, 0xf0);
    ret |= sc285sl_write_reg(dev, 0x301f, 0x04);
    ret |= sc285sl_write_reg(dev, 0x302b, 0x6f);
    ret |= sc285sl_write_reg(dev, 0x3031, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x3037, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3058, 0x21);
    ret |= sc285sl_write_reg(dev, 0x3059, 0x43);
    ret |= sc285sl_write_reg(dev, 0x30b0, 0x00);
    ret |= sc285sl_write_reg(dev, 0x30b8, 0x44);
    ret |= sc285sl_write_reg(dev, 0x3107, 0xbb);
    ret |= sc285sl_write_reg(dev, 0x3108, 0x98);
    ret |= sc285sl_write_reg(dev, 0x3204, 0x07);
    ret |= sc285sl_write_reg(dev, 0x3205, 0x8f);
    ret |= sc285sl_write_reg(dev, 0x3206, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3207, 0x47);
    ret |= sc285sl_write_reg(dev, 0x3208, 0x07);
    ret |= sc285sl_write_reg(dev, 0x3209, 0x80);
    ret |= sc285sl_write_reg(dev, 0x320a, 0x04);
    ret |= sc285sl_write_reg(dev, 0x320b, 0x38);
    ret |= sc285sl_write_reg(dev, 0x320c, 0x06);
    ret |= sc285sl_write_reg(dev, 0x320d, 0x40);
    ret |= sc285sl_write_reg(dev, 0x320e, 0x08);
    ret |= sc285sl_write_reg(dev, 0x320f, 0xca);
    ret |= sc285sl_write_reg(dev, 0x3210, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3211, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3212, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3213, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3214, 0x11);
    ret |= sc285sl_write_reg(dev, 0x3215, 0x11);
    ret |= sc285sl_write_reg(dev, 0x3220, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3223, 0xc0);
    ret |= sc285sl_write_reg(dev, 0x3250, 0x00);
    ret |= sc285sl_write_reg(dev, 0x325f, 0xa0);
    ret |= sc285sl_write_reg(dev, 0x3271, 0x10);
    ret |= sc285sl_write_reg(dev, 0x3273, 0x13);
    ret |= sc285sl_write_reg(dev, 0x32d1, 0x90);
    ret |= sc285sl_write_reg(dev, 0x32e0, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3301, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x3304, 0x60);
    ret |= sc285sl_write_reg(dev, 0x3305, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3306, 0xf8);
    ret |= sc285sl_write_reg(dev, 0x3309, 0x80);
    ret |= sc285sl_write_reg(dev, 0x330a, 0x02);
    ret |= sc285sl_write_reg(dev, 0x330b, 0x48);
    ret |= sc285sl_write_reg(dev, 0x331c, 0x01);
    ret |= sc285sl_write_reg(dev, 0x331e, 0x49);
    ret |= sc285sl_write_reg(dev, 0x331f, 0x69);
    ret |= sc285sl_write_reg(dev, 0x3320, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x3326, 0x14);
    ret |= sc285sl_write_reg(dev, 0x3328, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3329, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3353, 0x14);
    ret |= sc285sl_write_reg(dev, 0x3364, 0x5e);
    ret |= sc285sl_write_reg(dev, 0x3366, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3393, 0x14);
    ret |= sc285sl_write_reg(dev, 0x3394, 0x2c);
    ret |= sc285sl_write_reg(dev, 0x3395, 0x3c);
    ret |= sc285sl_write_reg(dev, 0x3399, 0x12);
    ret |= sc285sl_write_reg(dev, 0x339a, 0x14);
    ret |= sc285sl_write_reg(dev, 0x339b, 0x20);
    ret |= sc285sl_write_reg(dev, 0x339c, 0x50);
    ret |= sc285sl_write_reg(dev, 0x33ac, 0x18);
    ret |= sc285sl_write_reg(dev, 0x33ad, 0x2c);
    ret |= sc285sl_write_reg(dev, 0x33ae, 0x40);
    ret |= sc285sl_write_reg(dev, 0x33af, 0x60);
    ret |= sc285sl_write_reg(dev, 0x33b0, 0x0f);
    ret |= sc285sl_write_reg(dev, 0x33b2, 0x50);
    ret |= sc285sl_write_reg(dev, 0x33b3, 0x18);
    ret |= sc285sl_write_reg(dev, 0x33f8, 0x01);
    ret |= sc285sl_write_reg(dev, 0x33f9, 0x28);
    ret |= sc285sl_write_reg(dev, 0x33fa, 0x01);
    ret |= sc285sl_write_reg(dev, 0x33fb, 0x58);
    ret |= sc285sl_write_reg(dev, 0x3428, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3429, 0x04);
    ret |= sc285sl_write_reg(dev, 0x342a, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3447, 0x11);
    ret |= sc285sl_write_reg(dev, 0x349f, 0x03);
    ret |= sc285sl_write_reg(dev, 0x34a5, 0x01);
    ret |= sc285sl_write_reg(dev, 0x34a8, 0x10);
    ret |= sc285sl_write_reg(dev, 0x34a9, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x34aa, 0x02);
    ret |= sc285sl_write_reg(dev, 0x34ab, 0x78);
    ret |= sc285sl_write_reg(dev, 0x34ac, 0x02);
    ret |= sc285sl_write_reg(dev, 0x34ad, 0xa8);
    ret |= sc285sl_write_reg(dev, 0x34ba, 0x04);
    ret |= sc285sl_write_reg(dev, 0x34bb, 0x0f);
    ret |= sc285sl_write_reg(dev, 0x34f9, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3616, 0xbc);
    ret |= sc285sl_write_reg(dev, 0x3622, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3623, 0x38);
    ret |= sc285sl_write_reg(dev, 0x3624, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3625, 0x78);
    ret |= sc285sl_write_reg(dev, 0x3626, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3627, 0x38);
    ret |= sc285sl_write_reg(dev, 0x362f, 0x10);
    ret |= sc285sl_write_reg(dev, 0x3632, 0x64);
    ret |= sc285sl_write_reg(dev, 0x3633, 0x44);
    ret |= sc285sl_write_reg(dev, 0x3634, 0x24);
    ret |= sc285sl_write_reg(dev, 0x3637, 0xdc);
    ret |= sc285sl_write_reg(dev, 0x363d, 0x24);
    ret |= sc285sl_write_reg(dev, 0x3670, 0x41);
    ret |= sc285sl_write_reg(dev, 0x3671, 0x42);
    ret |= sc285sl_write_reg(dev, 0x3672, 0x33);
    ret |= sc285sl_write_reg(dev, 0x3673, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3674, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3675, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3676, 0x18);
    ret |= sc285sl_write_reg(dev, 0x367e, 0x44);
    ret |= sc285sl_write_reg(dev, 0x367f, 0x44);
    ret |= sc285sl_write_reg(dev, 0x3680, 0x34);
    ret |= sc285sl_write_reg(dev, 0x3681, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3682, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3683, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3684, 0x18);
    ret |= sc285sl_write_reg(dev, 0x3685, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3686, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3687, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3688, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3689, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368a, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368b, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368c, 0x80);
    ret |= sc285sl_write_reg(dev, 0x368d, 0x00);
    ret |= sc285sl_write_reg(dev, 0x368e, 0x08);
    ret |= sc285sl_write_reg(dev, 0x368f, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3690, 0x18);
    ret |= sc285sl_write_reg(dev, 0x3691, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3692, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3693, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3694, 0x08);
    ret |= sc285sl_write_reg(dev, 0x3695, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3696, 0x18);
    ret |= sc285sl_write_reg(dev, 0x3697, 0x04);
    ret |= sc285sl_write_reg(dev, 0x3698, 0x38);
    ret |= sc285sl_write_reg(dev, 0x3699, 0x04);
    ret |= sc285sl_write_reg(dev, 0x369a, 0x78);
    ret |= sc285sl_write_reg(dev, 0x36b9, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36ba, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36bb, 0x23);
    ret |= sc285sl_write_reg(dev, 0x36bc, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36bd, 0x08);
    ret |= sc285sl_write_reg(dev, 0x36be, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36bf, 0x38);
    ret |= sc285sl_write_reg(dev, 0x36d0, 0x09);
    ret |= sc285sl_write_reg(dev, 0x36d1, 0x30);
    ret |= sc285sl_write_reg(dev, 0x36d2, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36d3, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36d4, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36d5, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36d6, 0x00);
    ret |= sc285sl_write_reg(dev, 0x36d7, 0x04);
    ret |= sc285sl_write_reg(dev, 0x36d8, 0x18);
    ret |= sc285sl_write_reg(dev, 0x36e0, 0x33);
    ret |= sc285sl_write_reg(dev, 0x36e1, 0x43);
    ret |= sc285sl_write_reg(dev, 0x36e2, 0x34);
    ret |= sc285sl_write_reg(dev, 0x36ea, 0x12);
    ret |= sc285sl_write_reg(dev, 0x36eb, 0x47);
    ret |= sc285sl_write_reg(dev, 0x36ec, 0x43);
    ret |= sc285sl_write_reg(dev, 0x36ed, 0x38);
    ret |= sc285sl_write_reg(dev, 0x370f, 0x71);
    ret |= sc285sl_write_reg(dev, 0x3722, 0x03);
    ret |= sc285sl_write_reg(dev, 0x3724, 0xb0);
    ret |= sc285sl_write_reg(dev, 0x3727, 0x20);
    ret |= sc285sl_write_reg(dev, 0x3729, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37b0, 0x03);
    ret |= sc285sl_write_reg(dev, 0x37b1, 0x03);
    ret |= sc285sl_write_reg(dev, 0x37b2, 0x33);
    ret |= sc285sl_write_reg(dev, 0x37b3, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37b4, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37b5, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37b6, 0x78);
    ret |= sc285sl_write_reg(dev, 0x37b7, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37b8, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37b9, 0x82);
    ret |= sc285sl_write_reg(dev, 0x37ba, 0x84);
    ret |= sc285sl_write_reg(dev, 0x37bb, 0x88);
    ret |= sc285sl_write_reg(dev, 0x37bc, 0x98);
    ret |= sc285sl_write_reg(dev, 0x37bd, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37be, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37bf, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37c0, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37c1, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c2, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37c3, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c4, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37c5, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c6, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37c7, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37c8, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37c9, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37ca, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37cb, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37cc, 0xa8);
    ret |= sc285sl_write_reg(dev, 0x37cd, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37ce, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37cf, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37d6, 0xb4);
    ret |= sc285sl_write_reg(dev, 0x37d7, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37d8, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37d9, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37da, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37db, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37dc, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37dd, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37de, 0x38);
    ret |= sc285sl_write_reg(dev, 0x37df, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37e0, 0x78);
    ret |= sc285sl_write_reg(dev, 0x37e1, 0x28);
    ret |= sc285sl_write_reg(dev, 0x37e2, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e3, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e4, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e5, 0x34);
    ret |= sc285sl_write_reg(dev, 0x37e6, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37e7, 0x00);
    ret |= sc285sl_write_reg(dev, 0x37e8, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37e9, 0x08);
    ret |= sc285sl_write_reg(dev, 0x37ea, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37eb, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37ec, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37ed, 0x38);
    ret |= sc285sl_write_reg(dev, 0x37ee, 0x04);
    ret |= sc285sl_write_reg(dev, 0x37ef, 0x78);
    ret |= sc285sl_write_reg(dev, 0x37fa, 0x12);
    ret |= sc285sl_write_reg(dev, 0x37fb, 0x65);
    ret |= sc285sl_write_reg(dev, 0x37fc, 0x18);
    ret |= sc285sl_write_reg(dev, 0x37fd, 0x35);
    ret |= sc285sl_write_reg(dev, 0x3900, 0x05);
    ret |= sc285sl_write_reg(dev, 0x3903, 0x40);
    ret |= sc285sl_write_reg(dev, 0x3907, 0x01);
    ret |= sc285sl_write_reg(dev, 0x3908, 0x00);
    ret |= sc285sl_write_reg(dev, 0x391a, 0x80);
    ret |= sc285sl_write_reg(dev, 0x391b, 0x60);
    ret |= sc285sl_write_reg(dev, 0x391c, 0x33);
    ret |= sc285sl_write_reg(dev, 0x391d, 0x00);
    ret |= sc285sl_write_reg(dev, 0x391f, 0x41);
    ret |= sc285sl_write_reg(dev, 0x3926, 0xe2);
    ret |= sc285sl_write_reg(dev, 0x3933, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3934, 0x02);
    ret |= sc285sl_write_reg(dev, 0x3935, 0x01);
    ret |= sc285sl_write_reg(dev, 0x3936, 0x30);
    ret |= sc285sl_write_reg(dev, 0x3937, 0x75);
    ret |= sc285sl_write_reg(dev, 0x3938, 0x75);
    ret |= sc285sl_write_reg(dev, 0x3939, 0x00);
    ret |= sc285sl_write_reg(dev, 0x393a, 0x0b);
    ret |= sc285sl_write_reg(dev, 0x393b, 0x00);
    ret |= sc285sl_write_reg(dev, 0x393c, 0x2e);
    ret |= sc285sl_write_reg(dev, 0x39c9, 0x30);
    ret |= sc285sl_write_reg(dev, 0x39dd, 0x00);
    ret |= sc285sl_write_reg(dev, 0x39de, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x39e7, 0x08);
    ret |= sc285sl_write_reg(dev, 0x39e8, 0x08);
    ret |= sc285sl_write_reg(dev, 0x39e9, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3e00, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3e01, 0x8c);
    ret |= sc285sl_write_reg(dev, 0x3e02, 0x20);
    ret |= sc285sl_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc285sl_write_reg(dev, 0x3e06, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3e07, 0x80);
    ret |= sc285sl_write_reg(dev, 0x3e08, 0x00);
    ret |= sc285sl_write_reg(dev, 0x3e09, 0x20);
    ret |= sc285sl_write_reg(dev, 0x3e16, 0x02);
    ret |= sc285sl_write_reg(dev, 0x3e17, 0xb3);
    ret |= sc285sl_write_reg(dev, 0x3e18, 0x01);
    ret |= sc285sl_write_reg(dev, 0x3e19, 0x59);
    ret |= sc285sl_write_reg(dev, 0x4509, 0x1e);
    ret |= sc285sl_write_reg(dev, 0x450d, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc285sl_write_reg(dev, 0x480f, 0x03);
    ret |= sc285sl_write_reg(dev, 0x4837, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x5000, 0x2e);
    ret |= sc285sl_write_reg(dev, 0x5011, 0x01);
    ret |= sc285sl_write_reg(dev, 0x530d, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5310, 0x01);
    ret |= sc285sl_write_reg(dev, 0x5311, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5312, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5314, 0x07);
    ret |= sc285sl_write_reg(dev, 0x5315, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5316, 0x0b);
    ret |= sc285sl_write_reg(dev, 0x5318, 0x0e);
    ret |= sc285sl_write_reg(dev, 0x531a, 0x11);
    ret |= sc285sl_write_reg(dev, 0x531c, 0x14);
    ret |= sc285sl_write_reg(dev, 0x531d, 0x10);
    ret |= sc285sl_write_reg(dev, 0x531e, 0x1a);
    ret |= sc285sl_write_reg(dev, 0x531f, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5400, 0x01);
    ret |= sc285sl_write_reg(dev, 0x5404, 0x0c);
    ret |= sc285sl_write_reg(dev, 0x5405, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5784, 0x10);
    ret |= sc285sl_write_reg(dev, 0x5785, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5787, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x5788, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x5789, 0x08);
    ret |= sc285sl_write_reg(dev, 0x578a, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x578b, 0x0a);
    ret |= sc285sl_write_reg(dev, 0x578c, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5790, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5791, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5792, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5793, 0x08);
    ret |= sc285sl_write_reg(dev, 0x5794, 0x04);
    ret |= sc285sl_write_reg(dev, 0x5795, 0x04);
    ret |= sc285sl_write_reg(dev, 0x57ac, 0x00);
    ret |= sc285sl_write_reg(dev, 0x57ad, 0x00);
    ret |= sc285sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc285sl_write_reg(dev, 0x37f9, 0x24);
    ret |= sc285sl_write_reg(dev, 0x4402, 0x03);//置于0100前
    ret |= sc285sl_write_reg(dev, 0x4403, 0x0b);
    ret |= sc285sl_write_reg(dev, 0x4404, 0x21);
    ret |= sc285sl_write_reg(dev, 0x4405, 0x2b);
    ret |= sc285sl_write_reg(dev, 0x440c, 0x36);
    ret |= sc285sl_write_reg(dev, 0x440d, 0x36);
    ret |= sc285sl_write_reg(dev, 0x440e, 0x29);
    ret |= sc285sl_write_reg(dev, 0x440f, 0x45);
    ret |= sc285sl_write_reg(dev, 0x4412, 0x01);
    ret |= sc285sl_write_reg(dev, 0x4406, 0x00);
    ret |= sc285sl_write_reg(dev, 0x4407, 0x7e);
    ret |= sc285sl_write_reg(dev, 0x4424, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc285sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC285SL_24MInput_MIPI_2lane_12bit_1920x1080_30fps init success!========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 sc285sl_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC285SL_2M_4LANE_1080_12BIT_LINEAR_MODE:
            ret = sc285sl_4lane_linear_1920x1080_init(dev);
            break;
        case SC285SL_2M_2LANE_1080_12BIT_LINEAR_MODE:
            ret = sc285sl_2lane_linear_1920x1080_init(dev);
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc285sl_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_sc285sl_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc285sl_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc285sl_i2c_fd[dev]);
        g_sc285sl_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc285sl_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc285sl_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc285sl_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc285sl_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc285sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc285sl_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc285sl_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc285sl_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc285sl_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc285sl_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC285SL_DATA_BYTE];

    if (g_sc285sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc285sl_i2c_fd[dev], g_sc285sl_i2c_addr[dev], addr, SC285SL_ADDR_BYTE, buf, SC285SL_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC285SL_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc285sl_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc285sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc285sl_i2c_fd[dev], g_sc285sl_i2c_addr[dev], buf, SC285SL_ADDR_BYTE + SC285SL_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 sc285sl_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;
    usleep(30 * 1000);

    sc285sl_read_reg(dev, SC285SL_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc285sl_write_reg(dev, SC285SL_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc285sl_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_set_stream_mode(xmedia_u32 dev, xmedia_bool stream_en) {
    xmedia_s32 ret;

    ret = sc285sl_write_reg(dev, SC285SL_REG_ADDR_STANDBY, stream_en);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc285sl_start_stream failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

