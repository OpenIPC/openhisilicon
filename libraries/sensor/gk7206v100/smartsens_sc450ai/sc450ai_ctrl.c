#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc450ai.h"
#include "sc450ai_ctrl.h"


SENSOR_PRIORITY_DATA static xmedia_s32 g_sc450ai_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

SENSOR_PRIORITY_DATA static xmedia_s32 g_sc450ai_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define SC450AI_REG_ADDR_MIRROR_FLIP 0x3221

/*
 * 函数功能: sc450ai MIPI 4lane 760p线性binning模式初始化序列 - 1344x760x120fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_4lane_linear_1344x760_binning_120fps_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc450ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc450ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc450ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc450ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc450ai_write_reg(dev, 0x3018, 0x7a);
    ret |= sc450ai_write_reg(dev, 0x3019, 0x00);
    ret |= sc450ai_write_reg(dev, 0x301c, 0x78);
    ret |= sc450ai_write_reg(dev, 0x301f, 0x95);
    ret |= sc450ai_write_reg(dev, 0x302d, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x302e, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3208, 0x05);
    ret |= sc450ai_write_reg(dev, 0x3209, 0x40);
    ret |= sc450ai_write_reg(dev, 0x320a, 0x02);
    ret |= sc450ai_write_reg(dev, 0x320b, 0xf8);
    ret |= sc450ai_write_reg(dev, 0x320c, 0x03);
    ret |= sc450ai_write_reg(dev, 0x320d, 0xa8);
    ret |= sc450ai_write_reg(dev, 0x320e, 0x03);
    ret |= sc450ai_write_reg(dev, 0x320f, 0x0c);
    ret |= sc450ai_write_reg(dev, 0x3211, 0x04);
    ret |= sc450ai_write_reg(dev, 0x3213, 0x04);
    ret |= sc450ai_write_reg(dev, 0x3214, 0x11);
    ret |= sc450ai_write_reg(dev, 0x3215, 0x31);
    ret |= sc450ai_write_reg(dev, 0x3220, 0x01);
    ret |= sc450ai_write_reg(dev, 0x3223, 0xc0);
    ret |= sc450ai_write_reg(dev, 0x3253, 0x10);
    ret |= sc450ai_write_reg(dev, 0x325f, 0x44);
    ret |= sc450ai_write_reg(dev, 0x3274, 0x09);
    ret |= sc450ai_write_reg(dev, 0x3280, 0x01);
    ret |= sc450ai_write_reg(dev, 0x3301, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3306, 0x24);
    ret |= sc450ai_write_reg(dev, 0x3309, 0x60);
    ret |= sc450ai_write_reg(dev, 0x330b, 0x64);
    ret |= sc450ai_write_reg(dev, 0x330d, 0x30);
    ret |= sc450ai_write_reg(dev, 0x3315, 0x00);
    ret |= sc450ai_write_reg(dev, 0x331f, 0x59);
    ret |= sc450ai_write_reg(dev, 0x335d, 0x60);
    ret |= sc450ai_write_reg(dev, 0x3364, 0x56);
    ret |= sc450ai_write_reg(dev, 0x338f, 0x80);
    ret |= sc450ai_write_reg(dev, 0x3390, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3391, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3392, 0x38);
    ret |= sc450ai_write_reg(dev, 0x3393, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x3394, 0x10);
    ret |= sc450ai_write_reg(dev, 0x3395, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3396, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3397, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3398, 0x38);
    ret |= sc450ai_write_reg(dev, 0x3399, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x339a, 0x12);
    ret |= sc450ai_write_reg(dev, 0x339b, 0x14);
    ret |= sc450ai_write_reg(dev, 0x339c, 0x18);
    ret |= sc450ai_write_reg(dev, 0x33af, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3400, 0x16);
    ret |= sc450ai_write_reg(dev, 0x360f, 0x13);
    ret |= sc450ai_write_reg(dev, 0x3621, 0xec);
    ret |= sc450ai_write_reg(dev, 0x3627, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x3630, 0x90);
    ret |= sc450ai_write_reg(dev, 0x3633, 0x56);
    ret |= sc450ai_write_reg(dev, 0x3637, 0x1d);
    ret |= sc450ai_write_reg(dev, 0x3638, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x363c, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x363d, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x363e, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3670, 0x4a);
    ret |= sc450ai_write_reg(dev, 0x3671, 0xe0);
    ret |= sc450ai_write_reg(dev, 0x3672, 0xe0);
    ret |= sc450ai_write_reg(dev, 0x3673, 0xe0);
    ret |= sc450ai_write_reg(dev, 0x3674, 0xb0);
    ret |= sc450ai_write_reg(dev, 0x3675, 0x88);
    ret |= sc450ai_write_reg(dev, 0x3676, 0x8c);
    ret |= sc450ai_write_reg(dev, 0x367a, 0x48);
    ret |= sc450ai_write_reg(dev, 0x367b, 0x58);
    ret |= sc450ai_write_reg(dev, 0x367c, 0x48);
    ret |= sc450ai_write_reg(dev, 0x367d, 0x58);
    ret |= sc450ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc450ai_write_reg(dev, 0x3691, 0x43);
    ret |= sc450ai_write_reg(dev, 0x3692, 0x44);
    ret |= sc450ai_write_reg(dev, 0x3699, 0x03);
    ret |= sc450ai_write_reg(dev, 0x369a, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x369b, 0x1f);
    ret |= sc450ai_write_reg(dev, 0x369c, 0x40);
    ret |= sc450ai_write_reg(dev, 0x369d, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36a2, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36a3, 0x78);
    ret |= sc450ai_write_reg(dev, 0x36b0, 0x54);
    ret |= sc450ai_write_reg(dev, 0x36b1, 0x75);
    ret |= sc450ai_write_reg(dev, 0x36b2, 0x35);
    ret |= sc450ai_write_reg(dev, 0x36b3, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36b4, 0x78);
    ret |= sc450ai_write_reg(dev, 0x36b7, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b8, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b9, 0x20);
    ret |= sc450ai_write_reg(dev, 0x36bd, 0x40);
    ret |= sc450ai_write_reg(dev, 0x36be, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36d0, 0x20);
    ret |= sc450ai_write_reg(dev, 0x36e0, 0x08);
    ret |= sc450ai_write_reg(dev, 0x36e1, 0x08);
    ret |= sc450ai_write_reg(dev, 0x36e2, 0x12);
    ret |= sc450ai_write_reg(dev, 0x36e3, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36e4, 0x78);
    ret |= sc450ai_write_reg(dev, 0x36ec, 0x53);
    ret |= sc450ai_write_reg(dev, 0x36fa, 0x0d);
    ret |= sc450ai_write_reg(dev, 0x36fb, 0xa4);
    ret |= sc450ai_write_reg(dev, 0x36fc, 0x00);
    ret |= sc450ai_write_reg(dev, 0x36fd, 0x24);
    ret |= sc450ai_write_reg(dev, 0x3907, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc450ai_write_reg(dev, 0x391e, 0x01);
    ret |= sc450ai_write_reg(dev, 0x391f, 0x11);
    ret |= sc450ai_write_reg(dev, 0x3921, 0x10);
    ret |= sc450ai_write_reg(dev, 0x3933, 0x82);
    ret |= sc450ai_write_reg(dev, 0x3934, 0x0b);
    ret |= sc450ai_write_reg(dev, 0x3935, 0x02);
    ret |= sc450ai_write_reg(dev, 0x3936, 0x5e);
    ret |= sc450ai_write_reg(dev, 0x3937, 0x76);
    ret |= sc450ai_write_reg(dev, 0x3938, 0x78);
    ret |= sc450ai_write_reg(dev, 0x3939, 0x00);
    ret |= sc450ai_write_reg(dev, 0x393a, 0x28);
    ret |= sc450ai_write_reg(dev, 0x393b, 0x00);
    ret |= sc450ai_write_reg(dev, 0x393c, 0x1d);
    ret |= sc450ai_write_reg(dev, 0x3e00, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3e01, 0x61);
    ret |= sc450ai_write_reg(dev, 0x3e02, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc450ai_write_reg(dev, 0x3e08, 0x03);
    ret |= sc450ai_write_reg(dev, 0x3e1b, 0x2a);
    ret |= sc450ai_write_reg(dev, 0x440e, 0x02);
    ret |= sc450ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc450ai_write_reg(dev, 0x4837, 0x2c);
    ret |= sc450ai_write_reg(dev, 0x5000, 0x4e);
    ret |= sc450ai_write_reg(dev, 0x5001, 0x44);
    ret |= sc450ai_write_reg(dev, 0x5780, 0x76);
    ret |= sc450ai_write_reg(dev, 0x5784, 0x08);
    ret |= sc450ai_write_reg(dev, 0x5785, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5787, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x5788, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x5789, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578a, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578b, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578c, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578d, 0x40);
    ret |= sc450ai_write_reg(dev, 0x5790, 0x08);
    ret |= sc450ai_write_reg(dev, 0x5791, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5792, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5793, 0x08);
    ret |= sc450ai_write_reg(dev, 0x5794, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5795, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5799, 0x46);
    ret |= sc450ai_write_reg(dev, 0x579a, 0x77);
    ret |= sc450ai_write_reg(dev, 0x57a1, 0x04);
    ret |= sc450ai_write_reg(dev, 0x57a8, 0xd0);
    ret |= sc450ai_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc450ai_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc450ai_write_reg(dev, 0x57ac, 0x00);
    ret |= sc450ai_write_reg(dev, 0x57ad, 0x00);
    ret |= sc450ai_write_reg(dev, 0x5900, 0x01);
    ret |= sc450ai_write_reg(dev, 0x5901, 0x04);
    ret |= sc450ai_write_reg(dev, 0x59e0, 0xfe);
    ret |= sc450ai_write_reg(dev, 0x59e1, 0x40);
    ret |= sc450ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e3, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59e4, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e6, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59e7, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59e8, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e9, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ea, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59eb, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59ec, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ed, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59ee, 0xfe);
    ret |= sc450ai_write_reg(dev, 0x59ef, 0x40);
    ret |= sc450ai_write_reg(dev, 0x59f4, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f5, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59f6, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59f7, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f8, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59f9, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59fa, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fb, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59fc, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59fd, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fe, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ff, 0x38);
    ret |= sc450ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc450ai_write_reg(dev, 0x36f9, 0x20);
    ret |= sc450ai_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc450ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=======================================================================================\n");
        SENSOR_PRINT("========= sc450ai_4lane_10bit_linear_1344x760_binning_120fps_init init success!========\n");
        SENSOR_PRINT("=======================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}


/*
 * 函数功能: sc450ai MIPI 2lane 1440p线性模式初始化序列 - 2560x1440x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc450ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc450ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc450ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc450ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc450ai_write_reg(dev, 0x3018, 0x3a);
    ret |= sc450ai_write_reg(dev, 0x3019, 0x0c);
    ret |= sc450ai_write_reg(dev, 0x301c, 0x78);
    ret |= sc450ai_write_reg(dev, 0x301f, 0x23);
    ret |= sc450ai_write_reg(dev, 0x302e, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3200, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3201, 0x40);
    ret |= sc450ai_write_reg(dev, 0x3202, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3203, 0x28);
    ret |= sc450ai_write_reg(dev, 0x3204, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x3205, 0x4f);
    ret |= sc450ai_write_reg(dev, 0x3206, 0x05);
    ret |= sc450ai_write_reg(dev, 0x3207, 0xd7);
    ret |= sc450ai_write_reg(dev, 0x3208, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x3209, 0x00);
    ret |= sc450ai_write_reg(dev, 0x320a, 0x05);
    ret |= sc450ai_write_reg(dev, 0x320b, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x320c, 0x03);
    ret |= sc450ai_write_reg(dev, 0x320d, 0x84);
    ret |= sc450ai_write_reg(dev, 0x320e, 0x06);
    ret |= sc450ai_write_reg(dev, 0x320f, 0x40);
    ret |= sc450ai_write_reg(dev, 0x3210, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3211, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3212, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3213, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3214, 0x11);
    ret |= sc450ai_write_reg(dev, 0x3215, 0x11);
    ret |= sc450ai_write_reg(dev, 0x3220, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3223, 0xc0);
    ret |= sc450ai_write_reg(dev, 0x3253, 0x10);
    ret |= sc450ai_write_reg(dev, 0x325f, 0x44);
    ret |= sc450ai_write_reg(dev, 0x3274, 0x09);
    ret |= sc450ai_write_reg(dev, 0x3280, 0x01);
    ret |= sc450ai_write_reg(dev, 0x3301, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3306, 0x24);
    ret |= sc450ai_write_reg(dev, 0x3309, 0x60);
    ret |= sc450ai_write_reg(dev, 0x330b, 0x64);
    ret |= sc450ai_write_reg(dev, 0x330d, 0x30);
    ret |= sc450ai_write_reg(dev, 0x3315, 0x00);
    ret |= sc450ai_write_reg(dev, 0x331f, 0x59);
    ret |= sc450ai_write_reg(dev, 0x335d, 0x60);
    ret |= sc450ai_write_reg(dev, 0x3364, 0x56);
    ret |= sc450ai_write_reg(dev, 0x338f, 0x80);
    ret |= sc450ai_write_reg(dev, 0x3390, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3391, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3392, 0x38);
    ret |= sc450ai_write_reg(dev, 0x3393, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x3394, 0x10);
    ret |= sc450ai_write_reg(dev, 0x3395, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3396, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3397, 0x18);
    ret |= sc450ai_write_reg(dev, 0x3398, 0x38);
    ret |= sc450ai_write_reg(dev, 0x3399, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x339a, 0x12);
    ret |= sc450ai_write_reg(dev, 0x339b, 0x14);
    ret |= sc450ai_write_reg(dev, 0x339c, 0x18);
    ret |= sc450ai_write_reg(dev, 0x33af, 0x18);
    ret |= sc450ai_write_reg(dev, 0x360f, 0x13);
    ret |= sc450ai_write_reg(dev, 0x3621, 0xec);
    ret |= sc450ai_write_reg(dev, 0x3627, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x3630, 0x90);
    ret |= sc450ai_write_reg(dev, 0x3633, 0x56);
    ret |= sc450ai_write_reg(dev, 0x3637, 0x1d);
    ret |= sc450ai_write_reg(dev, 0x3638, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x363c, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x363d, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x363e, 0x08);
    ret |= sc450ai_write_reg(dev, 0x3670, 0x4a);
    ret |= sc450ai_write_reg(dev, 0x3671, 0xe0);
    ret |= sc450ai_write_reg(dev, 0x3672, 0xe0);
    ret |= sc450ai_write_reg(dev, 0x3673, 0xe0);
    ret |= sc450ai_write_reg(dev, 0x3674, 0xb0);
    ret |= sc450ai_write_reg(dev, 0x3675, 0x88);
    ret |= sc450ai_write_reg(dev, 0x3676, 0x8c);
    ret |= sc450ai_write_reg(dev, 0x367a, 0x48);
    ret |= sc450ai_write_reg(dev, 0x367b, 0x58);
    ret |= sc450ai_write_reg(dev, 0x367c, 0x48);
    ret |= sc450ai_write_reg(dev, 0x367d, 0x58);
    ret |= sc450ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc450ai_write_reg(dev, 0x3691, 0x43);
    ret |= sc450ai_write_reg(dev, 0x3692, 0x44);
    ret |= sc450ai_write_reg(dev, 0x3699, 0x03);
    ret |= sc450ai_write_reg(dev, 0x369a, 0x0f);
    ret |= sc450ai_write_reg(dev, 0x369b, 0x1f);
    ret |= sc450ai_write_reg(dev, 0x369c, 0x40);
    ret |= sc450ai_write_reg(dev, 0x369d, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36a2, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36a3, 0x78);
    ret |= sc450ai_write_reg(dev, 0x36b0, 0x54);
    ret |= sc450ai_write_reg(dev, 0x36b1, 0x75);
    ret |= sc450ai_write_reg(dev, 0x36b2, 0x35);
    ret |= sc450ai_write_reg(dev, 0x36b3, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36b4, 0x78);
    ret |= sc450ai_write_reg(dev, 0x36b7, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b8, 0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b9, 0x20);
    ret |= sc450ai_write_reg(dev, 0x36bd, 0x40);
    ret |= sc450ai_write_reg(dev, 0x36be, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36d0, 0x20);
    ret |= sc450ai_write_reg(dev, 0x36e0, 0x08);
    ret |= sc450ai_write_reg(dev, 0x36e1, 0x08);
    ret |= sc450ai_write_reg(dev, 0x36e2, 0x12);
    ret |= sc450ai_write_reg(dev, 0x36e3, 0x48);
    ret |= sc450ai_write_reg(dev, 0x36e4, 0x78);
    ret |= sc450ai_write_reg(dev, 0x36ea, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x36eb, 0x05);
    ret |= sc450ai_write_reg(dev, 0x36ec, 0x43);
    ret |= sc450ai_write_reg(dev, 0x36ed, 0x34);
    ret |= sc450ai_write_reg(dev, 0x36fa, 0x06);
    ret |= sc450ai_write_reg(dev, 0x36fb, 0xa4);
    ret |= sc450ai_write_reg(dev, 0x36fc, 0x00);
    ret |= sc450ai_write_reg(dev, 0x36fd, 0x34);
    ret |= sc450ai_write_reg(dev, 0x3907, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc450ai_write_reg(dev, 0x391e, 0x01);
    ret |= sc450ai_write_reg(dev, 0x391f, 0x11);
    ret |= sc450ai_write_reg(dev, 0x3933, 0x82);
    ret |= sc450ai_write_reg(dev, 0x3934, 0x0b);
    ret |= sc450ai_write_reg(dev, 0x3935, 0x02);
    ret |= sc450ai_write_reg(dev, 0x3936, 0x5e);
    ret |= sc450ai_write_reg(dev, 0x3937, 0x76);
    ret |= sc450ai_write_reg(dev, 0x3938, 0x78);
    ret |= sc450ai_write_reg(dev, 0x3939, 0x00);
    ret |= sc450ai_write_reg(dev, 0x393a, 0x28);
    ret |= sc450ai_write_reg(dev, 0x393b, 0x00);
    ret |= sc450ai_write_reg(dev, 0x393c, 0x1d);
    ret |= sc450ai_write_reg(dev, 0x3e00, 0x00);
    ret |= sc450ai_write_reg(dev, 0x3e01, 0xc7);
    ret |= sc450ai_write_reg(dev, 0x3e02, 0x80);
    ret |= sc450ai_write_reg(dev, 0x3e03, 0x0b);
    ret |= sc450ai_write_reg(dev, 0x3e08, 0x03);
    ret |= sc450ai_write_reg(dev, 0x3e1b, 0x2a);
    ret |= sc450ai_write_reg(dev, 0x440e, 0x02);
    ret |= sc450ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc450ai_write_reg(dev, 0x4837, 0x16);
    ret |= sc450ai_write_reg(dev, 0x5000, 0x0e);
    ret |= sc450ai_write_reg(dev, 0x5001, 0x44);
    ret |= sc450ai_write_reg(dev, 0x5780, 0x76);
    ret |= sc450ai_write_reg(dev, 0x5784, 0x08);
    ret |= sc450ai_write_reg(dev, 0x5785, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5787, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x5788, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x5789, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578a, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578b, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578c, 0x0a);
    ret |= sc450ai_write_reg(dev, 0x578d, 0x40);
    ret |= sc450ai_write_reg(dev, 0x5790, 0x08);
    ret |= sc450ai_write_reg(dev, 0x5791, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5792, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5793, 0x08);
    ret |= sc450ai_write_reg(dev, 0x5794, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5795, 0x04);
    ret |= sc450ai_write_reg(dev, 0x5799, 0x46);
    ret |= sc450ai_write_reg(dev, 0x579a, 0x77);
    ret |= sc450ai_write_reg(dev, 0x57a1, 0x04);
    ret |= sc450ai_write_reg(dev, 0x57a8, 0xd0);
    ret |= sc450ai_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc450ai_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc450ai_write_reg(dev, 0x57ac, 0x00);
    ret |= sc450ai_write_reg(dev, 0x57ad, 0x00);
    ret |= sc450ai_write_reg(dev, 0x59e0, 0xfe);
    ret |= sc450ai_write_reg(dev, 0x59e1, 0x40);
    ret |= sc450ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e3, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59e4, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e6, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59e7, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59e8, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e9, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ea, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59eb, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59ec, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ed, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59ee, 0xfe);
    ret |= sc450ai_write_reg(dev, 0x59ef, 0x40);
    ret |= sc450ai_write_reg(dev, 0x59f4, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f5, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59f6, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59f7, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f8, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59f9, 0x30);
    ret |= sc450ai_write_reg(dev, 0x59fa, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fb, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59fc, 0x38);
    ret |= sc450ai_write_reg(dev, 0x59fd, 0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fe, 0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ff, 0x38);
    ret |= sc450ai_write_reg(dev, 0x36e9, 0x20);
    ret |= sc450ai_write_reg(dev, 0x36f9, 0x20);
    ret |= sc450ai_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc450ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC450AI_24MInput_MIPI_2lane_10bit_2560x1440_30fps init success!========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_4lane_wdr_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc450ai_write_reg(dev, 0x0103,0x01);
    ret |= sc450ai_write_reg(dev, 0x0100,0x00);
    ret |= sc450ai_write_reg(dev, 0x36e9,0x80);
    ret |= sc450ai_write_reg(dev, 0x36f9,0x80);
    ret |= sc450ai_write_reg(dev, 0x301c,0x78);
    ret |= sc450ai_write_reg(dev, 0x301f,0x6f);
    ret |= sc450ai_write_reg(dev, 0x302e,0x00);
    ret |= sc450ai_write_reg(dev, 0x3200,0x00);
    ret |= sc450ai_write_reg(dev, 0x3201,0x40);
    ret |= sc450ai_write_reg(dev, 0x3202,0x00);
    ret |= sc450ai_write_reg(dev, 0x3203,0x28);
    ret |= sc450ai_write_reg(dev, 0x3204,0x0a);
    ret |= sc450ai_write_reg(dev, 0x3205,0x4f);
    ret |= sc450ai_write_reg(dev, 0x3206,0x05);
    ret |= sc450ai_write_reg(dev, 0x3207,0xd7);
    ret |= sc450ai_write_reg(dev, 0x3208,0x0a);
    ret |= sc450ai_write_reg(dev, 0x3209,0x00);
    ret |= sc450ai_write_reg(dev, 0x320a,0x05);
    ret |= sc450ai_write_reg(dev, 0x320b,0xa0);
    ret |= sc450ai_write_reg(dev, 0x320c,0x03);
    ret |= sc450ai_write_reg(dev, 0x320d,0x84);
    ret |= sc450ai_write_reg(dev, 0x320e,0x0c);
    ret |= sc450ai_write_reg(dev, 0x320f,0x80);
    ret |= sc450ai_write_reg(dev, 0x3210,0x00);
    ret |= sc450ai_write_reg(dev, 0x3211,0x08);
    ret |= sc450ai_write_reg(dev, 0x3212,0x00);
    ret |= sc450ai_write_reg(dev, 0x3213,0x16);
    ret |= sc450ai_write_reg(dev, 0x3214,0x11);
    ret |= sc450ai_write_reg(dev, 0x3215,0x11);
    ret |= sc450ai_write_reg(dev, 0x3220,0x00);
    ret |= sc450ai_write_reg(dev, 0x3223,0xc0);
    ret |= sc450ai_write_reg(dev, 0x3250,0xff);
    ret |= sc450ai_write_reg(dev, 0x3253,0x10);
    ret |= sc450ai_write_reg(dev, 0x325f,0x44);
    ret |= sc450ai_write_reg(dev, 0x3274,0x09);
    ret |= sc450ai_write_reg(dev, 0x3280,0x01);
    ret |= sc450ai_write_reg(dev, 0x3281,0x01);
    ret |= sc450ai_write_reg(dev, 0x3301,0x08);
    ret |= sc450ai_write_reg(dev, 0x3306,0x24);
    ret |= sc450ai_write_reg(dev, 0x3309,0x60);
    ret |= sc450ai_write_reg(dev, 0x330b,0x64);
    ret |= sc450ai_write_reg(dev, 0x330d,0x30);
    ret |= sc450ai_write_reg(dev, 0x3314,0x94);
    ret |= sc450ai_write_reg(dev, 0x3315,0x00);
    ret |= sc450ai_write_reg(dev, 0x331f,0x59);
    ret |= sc450ai_write_reg(dev, 0x335d,0x60);
    ret |= sc450ai_write_reg(dev, 0x3364,0x56);
    ret |= sc450ai_write_reg(dev, 0x338f,0x80);
    ret |= sc450ai_write_reg(dev, 0x3390,0x08);
    ret |= sc450ai_write_reg(dev, 0x3391,0x18);
    ret |= sc450ai_write_reg(dev, 0x3392,0x38);
    ret |= sc450ai_write_reg(dev, 0x3393,0x0a);
    ret |= sc450ai_write_reg(dev, 0x3394,0x10);
    ret |= sc450ai_write_reg(dev, 0x3395,0x18);
    ret |= sc450ai_write_reg(dev, 0x3396,0x08);
    ret |= sc450ai_write_reg(dev, 0x3397,0x18);
    ret |= sc450ai_write_reg(dev, 0x3398,0x38);
    ret |= sc450ai_write_reg(dev, 0x3399,0x0f);
    ret |= sc450ai_write_reg(dev, 0x339a,0x12);
    ret |= sc450ai_write_reg(dev, 0x339b,0x14);
    ret |= sc450ai_write_reg(dev, 0x339c,0x18);
    ret |= sc450ai_write_reg(dev, 0x33af,0x18);
    ret |= sc450ai_write_reg(dev, 0x3400,0x04);
    ret |= sc450ai_write_reg(dev, 0x3410,0x04);
    ret |= sc450ai_write_reg(dev, 0x360f,0x13);
    ret |= sc450ai_write_reg(dev, 0x3621,0xec);
    ret |= sc450ai_write_reg(dev, 0x3627,0xa0);
    ret |= sc450ai_write_reg(dev, 0x3630,0x90);
    ret |= sc450ai_write_reg(dev, 0x3633,0x56);
    ret |= sc450ai_write_reg(dev, 0x3637,0x1d);
    ret |= sc450ai_write_reg(dev, 0x3638,0x0a);
    ret |= sc450ai_write_reg(dev, 0x363c,0x0f);
    ret |= sc450ai_write_reg(dev, 0x363d,0x0f);
    ret |= sc450ai_write_reg(dev, 0x363e,0x08);
    ret |= sc450ai_write_reg(dev, 0x3670,0x4a);
    ret |= sc450ai_write_reg(dev, 0x3671,0xe0);
    ret |= sc450ai_write_reg(dev, 0x3672,0xe0);
    ret |= sc450ai_write_reg(dev, 0x3673,0xe0);
    ret |= sc450ai_write_reg(dev, 0x3674,0xb0);
    ret |= sc450ai_write_reg(dev, 0x3675,0x88);
    ret |= sc450ai_write_reg(dev, 0x3676,0x8c);
    ret |= sc450ai_write_reg(dev, 0x367a,0x48);
    ret |= sc450ai_write_reg(dev, 0x367b,0x58);
    ret |= sc450ai_write_reg(dev, 0x367c,0x48);
    ret |= sc450ai_write_reg(dev, 0x367d,0x58);
    ret |= sc450ai_write_reg(dev, 0x3690,0x34);
    ret |= sc450ai_write_reg(dev, 0x3691,0x43);
    ret |= sc450ai_write_reg(dev, 0x3692,0x44);
    ret |= sc450ai_write_reg(dev, 0x3699,0x03);
    ret |= sc450ai_write_reg(dev, 0x369a,0x0f);
    ret |= sc450ai_write_reg(dev, 0x369b,0x1f);
    ret |= sc450ai_write_reg(dev, 0x369c,0x40);
    ret |= sc450ai_write_reg(dev, 0x369d,0x48);
    ret |= sc450ai_write_reg(dev, 0x36a2,0x48);
    ret |= sc450ai_write_reg(dev, 0x36a3,0x78);
    ret |= sc450ai_write_reg(dev, 0x36b0,0x54);
    ret |= sc450ai_write_reg(dev, 0x36b1,0x55);
    ret |= sc450ai_write_reg(dev, 0x36b2,0x55);
    ret |= sc450ai_write_reg(dev, 0x36b3,0x48);
    ret |= sc450ai_write_reg(dev, 0x36b4,0x78);
    ret |= sc450ai_write_reg(dev, 0x36b7,0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b8,0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b9,0x20);
    ret |= sc450ai_write_reg(dev, 0x36bd,0x40);
    ret |= sc450ai_write_reg(dev, 0x36be,0x48);
    ret |= sc450ai_write_reg(dev, 0x36d0,0x20);
    ret |= sc450ai_write_reg(dev, 0x36e0,0x08);
    ret |= sc450ai_write_reg(dev, 0x36e1,0x08);
    ret |= sc450ai_write_reg(dev, 0x36e2,0x12);
    ret |= sc450ai_write_reg(dev, 0x36e3,0x48);
    ret |= sc450ai_write_reg(dev, 0x36e4,0x78);
    ret |= sc450ai_write_reg(dev, 0x36ea,0x0a);
    ret |= sc450ai_write_reg(dev, 0x36eb,0x05);
    ret |= sc450ai_write_reg(dev, 0x36ec,0x43);
    ret |= sc450ai_write_reg(dev, 0x36ed,0x14);
    ret |= sc450ai_write_reg(dev, 0x36fa,0x08);
    ret |= sc450ai_write_reg(dev, 0x36fb,0xa4);
    ret |= sc450ai_write_reg(dev, 0x36fc,0x00);
    ret |= sc450ai_write_reg(dev, 0x36fd,0x14);
    ret |= sc450ai_write_reg(dev, 0x3900,0x07);
    ret |= sc450ai_write_reg(dev, 0x3902,0xf0);
    ret |= sc450ai_write_reg(dev, 0x3907,0x00);
    ret |= sc450ai_write_reg(dev, 0x3908,0x41);
    ret |= sc450ai_write_reg(dev, 0x391e,0x01);
    ret |= sc450ai_write_reg(dev, 0x391f,0x11);
    ret |= sc450ai_write_reg(dev, 0x3933,0x82);
    ret |= sc450ai_write_reg(dev, 0x3934,0x0b);
    ret |= sc450ai_write_reg(dev, 0x3935,0x02);
    ret |= sc450ai_write_reg(dev, 0x3936,0x5e);
    ret |= sc450ai_write_reg(dev, 0x3937,0x76);
    ret |= sc450ai_write_reg(dev, 0x3938,0x78);
    ret |= sc450ai_write_reg(dev, 0x3939,0x00);
    ret |= sc450ai_write_reg(dev, 0x393a,0x28);
    ret |= sc450ai_write_reg(dev, 0x393b,0x00);
    ret |= sc450ai_write_reg(dev, 0x393c,0x1d);
    ret |= sc450ai_write_reg(dev, 0x3e00,0x01);
    ret |= sc450ai_write_reg(dev, 0x3e01,0x82);
    ret |= sc450ai_write_reg(dev, 0x3e02,0x00);
    ret |= sc450ai_write_reg(dev, 0x3e03,0x0b);
    ret |= sc450ai_write_reg(dev, 0x3e04,0x18);
    ret |= sc450ai_write_reg(dev, 0x3e05,0x20);
    ret |= sc450ai_write_reg(dev, 0x3e06,0x00);
    ret |= sc450ai_write_reg(dev, 0x3e07,0x80);
    ret |= sc450ai_write_reg(dev, 0x3e08,0x03);
    ret |= sc450ai_write_reg(dev, 0x3e09,0x40);
    ret |= sc450ai_write_reg(dev, 0x3e10,0x00);
    ret |= sc450ai_write_reg(dev, 0x3e11,0x80);
    ret |= sc450ai_write_reg(dev, 0x3e12,0x03);
    ret |= sc450ai_write_reg(dev, 0x3e13,0x40);
    ret |= sc450ai_write_reg(dev, 0x3e1b,0x2a);
    ret |= sc450ai_write_reg(dev, 0x3e23,0x00);
    ret |= sc450ai_write_reg(dev, 0x3e24,0xc6);
    ret |= sc450ai_write_reg(dev, 0x440e,0x02);
    ret |= sc450ai_write_reg(dev, 0x4503,0x60);
    ret |= sc450ai_write_reg(dev, 0x4509,0x20);
    ret |= sc450ai_write_reg(dev, 0x4837,0x16);
    ret |= sc450ai_write_reg(dev, 0x4853,0xf8);
    ret |= sc450ai_write_reg(dev, 0x5000,0x0e);
    ret |= sc450ai_write_reg(dev, 0x5001,0x44);
    ret |= sc450ai_write_reg(dev, 0x5011,0x80);
    ret |= sc450ai_write_reg(dev, 0x5780,0x76);
    ret |= sc450ai_write_reg(dev, 0x5784,0x08);
    ret |= sc450ai_write_reg(dev, 0x5785,0x04);
    ret |= sc450ai_write_reg(dev, 0x5787,0x0a);
    ret |= sc450ai_write_reg(dev, 0x5788,0x0a);
    ret |= sc450ai_write_reg(dev, 0x5789,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578a,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578b,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578c,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578d,0x40);
    ret |= sc450ai_write_reg(dev, 0x5790,0x08);
    ret |= sc450ai_write_reg(dev, 0x5791,0x04);
    ret |= sc450ai_write_reg(dev, 0x5792,0x04);
    ret |= sc450ai_write_reg(dev, 0x5793,0x08);
    ret |= sc450ai_write_reg(dev, 0x5794,0x04);
    ret |= sc450ai_write_reg(dev, 0x5795,0x04);
    ret |= sc450ai_write_reg(dev, 0x5799,0x46);
    ret |= sc450ai_write_reg(dev, 0x579a,0x77);
    ret |= sc450ai_write_reg(dev, 0x57a1,0x04);
    ret |= sc450ai_write_reg(dev, 0x57a8,0xd0);
    ret |= sc450ai_write_reg(dev, 0x57aa,0x2a);
    ret |= sc450ai_write_reg(dev, 0x57ab,0x7f);
    ret |= sc450ai_write_reg(dev, 0x57ac,0x00);
    ret |= sc450ai_write_reg(dev, 0x57ad,0x00);
    ret |= sc450ai_write_reg(dev, 0x59e0,0xfe);
    ret |= sc450ai_write_reg(dev, 0x59e1,0x40);
    ret |= sc450ai_write_reg(dev, 0x59e2,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e3,0x38);
    ret |= sc450ai_write_reg(dev, 0x59e4,0x30);
    ret |= sc450ai_write_reg(dev, 0x59e5,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e6,0x38);
    ret |= sc450ai_write_reg(dev, 0x59e7,0x30);
    ret |= sc450ai_write_reg(dev, 0x59e8,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e9,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ea,0x38);
    ret |= sc450ai_write_reg(dev, 0x59eb,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59ec,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ed,0x38);
    ret |= sc450ai_write_reg(dev, 0x59ee,0xfe);
    ret |= sc450ai_write_reg(dev, 0x59ef,0x40);
    ret |= sc450ai_write_reg(dev, 0x59f4,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f5,0x38);
    ret |= sc450ai_write_reg(dev, 0x59f6,0x30);
    ret |= sc450ai_write_reg(dev, 0x59f7,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f8,0x38);
    ret |= sc450ai_write_reg(dev, 0x59f9,0x30);
    ret |= sc450ai_write_reg(dev, 0x59fa,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fb,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59fc,0x38);
    ret |= sc450ai_write_reg(dev, 0x59fd,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fe,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ff,0x38);
    ret |= sc450ai_write_reg(dev, 0x36e9,0x44);
    ret |= sc450ai_write_reg(dev, 0x36f9,0x24);
    ret |= sc450ai_write_reg(dev, 0x0100,0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc450ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC450AI_27MInput_MIPI_4lane_10bit_2560x1440_30fps wdr init success!====\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: sc450ai MIPI 2lane 1520p线性模式初始化序列 - 2688x1520x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_2lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc450ai_write_reg(dev, 0x0103,0x01);
    ret |= sc450ai_write_reg(dev, 0x0100,0x00);
    ret |= sc450ai_write_reg(dev, 0x36e9,0x80);
    ret |= sc450ai_write_reg(dev, 0x36f9,0x80);
    ret |= sc450ai_write_reg(dev, 0x3018,0x3a);
    ret |= sc450ai_write_reg(dev, 0x3019,0x0c);
    ret |= sc450ai_write_reg(dev, 0x301c,0x78);
    ret |= sc450ai_write_reg(dev, 0x301f,0x15);
    ret |= sc450ai_write_reg(dev, 0x302e,0x00);
    ret |= sc450ai_write_reg(dev, 0x3200,0x00);
    ret |= sc450ai_write_reg(dev, 0x3201,0x00);
    ret |= sc450ai_write_reg(dev, 0x3202,0x00);
    ret |= sc450ai_write_reg(dev, 0x3203,0x00);
    ret |= sc450ai_write_reg(dev, 0x3204,0x0a);
    ret |= sc450ai_write_reg(dev, 0x3205,0x8f);
    ret |= sc450ai_write_reg(dev, 0x3206,0x05);
    ret |= sc450ai_write_reg(dev, 0x3207,0xff);
    ret |= sc450ai_write_reg(dev, 0x3208,0x0a);
    ret |= sc450ai_write_reg(dev, 0x3209,0x80);
    ret |= sc450ai_write_reg(dev, 0x320a,0x05);
    ret |= sc450ai_write_reg(dev, 0x320b,0xf0);
    ret |= sc450ai_write_reg(dev, 0x320c,0x02);
    ret |= sc450ai_write_reg(dev, 0x320d,0xee);
    ret |= sc450ai_write_reg(dev, 0x320e,0x06);
    ret |= sc450ai_write_reg(dev, 0x320f,0x18);
    ret |= sc450ai_write_reg(dev, 0x3210,0x00);
    ret |= sc450ai_write_reg(dev, 0x3211,0x08);
    ret |= sc450ai_write_reg(dev, 0x3212,0x00);
    ret |= sc450ai_write_reg(dev, 0x3213,0x08);
    ret |= sc450ai_write_reg(dev, 0x3214,0x11);
    ret |= sc450ai_write_reg(dev, 0x3215,0x11);
    ret |= sc450ai_write_reg(dev, 0x3220,0x00);
    ret |= sc450ai_write_reg(dev, 0x3223,0xc0);
    ret |= sc450ai_write_reg(dev, 0x3253,0x10);
    ret |= sc450ai_write_reg(dev, 0x325f,0x44);
    ret |= sc450ai_write_reg(dev, 0x3274,0x09);
    ret |= sc450ai_write_reg(dev, 0x3280,0x01);
    ret |= sc450ai_write_reg(dev, 0x3301,0x07);
    ret |= sc450ai_write_reg(dev, 0x3306,0x20);
    ret |= sc450ai_write_reg(dev, 0x3308,0x08);
    ret |= sc450ai_write_reg(dev, 0x330b,0x58);
    ret |= sc450ai_write_reg(dev, 0x330e,0x18);
    ret |= sc450ai_write_reg(dev, 0x3315,0x00);
    ret |= sc450ai_write_reg(dev, 0x335d,0x60);
    ret |= sc450ai_write_reg(dev, 0x3364,0x56);
    ret |= sc450ai_write_reg(dev, 0x338f,0x80);
    ret |= sc450ai_write_reg(dev, 0x3390,0x08);
    ret |= sc450ai_write_reg(dev, 0x3391,0x18);
    ret |= sc450ai_write_reg(dev, 0x3392,0x38);
    ret |= sc450ai_write_reg(dev, 0x3393,0x07);
    ret |= sc450ai_write_reg(dev, 0x3394,0x10);
    ret |= sc450ai_write_reg(dev, 0x3395,0x18);
    ret |= sc450ai_write_reg(dev, 0x3396,0x08);
    ret |= sc450ai_write_reg(dev, 0x3397,0x18);
    ret |= sc450ai_write_reg(dev, 0x3398,0x38);
    ret |= sc450ai_write_reg(dev, 0x3399,0x10);
    ret |= sc450ai_write_reg(dev, 0x339a,0x13);
    ret |= sc450ai_write_reg(dev, 0x339b,0x15);
    ret |= sc450ai_write_reg(dev, 0x339c,0x18);
    ret |= sc450ai_write_reg(dev, 0x33af,0x18);
    ret |= sc450ai_write_reg(dev, 0x360f,0x13);
    ret |= sc450ai_write_reg(dev, 0x3621,0xec);
    ret |= sc450ai_write_reg(dev, 0x3622,0x00);
    ret |= sc450ai_write_reg(dev, 0x3625,0x0b);
    ret |= sc450ai_write_reg(dev, 0x3627,0x20);
    ret |= sc450ai_write_reg(dev, 0x3630,0x90);
    ret |= sc450ai_write_reg(dev, 0x3633,0x56);
    ret |= sc450ai_write_reg(dev, 0x3637,0x1d);
    ret |= sc450ai_write_reg(dev, 0x3638,0x12);
    ret |= sc450ai_write_reg(dev, 0x363c,0x0f);
    ret |= sc450ai_write_reg(dev, 0x363d,0x0f);
    ret |= sc450ai_write_reg(dev, 0x363e,0x08);
    ret |= sc450ai_write_reg(dev, 0x3670,0x4a);
    ret |= sc450ai_write_reg(dev, 0x3671,0xe0);
    ret |= sc450ai_write_reg(dev, 0x3672,0xe0);
    ret |= sc450ai_write_reg(dev, 0x3673,0xe0);
    ret |= sc450ai_write_reg(dev, 0x3674,0xc0);
    ret |= sc450ai_write_reg(dev, 0x3675,0x87);
    ret |= sc450ai_write_reg(dev, 0x3676,0x8c);
    ret |= sc450ai_write_reg(dev, 0x367a,0x48);
    ret |= sc450ai_write_reg(dev, 0x367b,0x58);
    ret |= sc450ai_write_reg(dev, 0x367c,0x48);
    ret |= sc450ai_write_reg(dev, 0x367d,0x58);
    ret |= sc450ai_write_reg(dev, 0x3690,0x22);
    ret |= sc450ai_write_reg(dev, 0x3691,0x33);
    ret |= sc450ai_write_reg(dev, 0x3692,0x44);
    ret |= sc450ai_write_reg(dev, 0x3699,0x03);
    ret |= sc450ai_write_reg(dev, 0x369a,0x0f);
    ret |= sc450ai_write_reg(dev, 0x369b,0x1f);
    ret |= sc450ai_write_reg(dev, 0x369c,0x40);
    ret |= sc450ai_write_reg(dev, 0x369d,0x78);
    ret |= sc450ai_write_reg(dev, 0x36a2,0x48);
    ret |= sc450ai_write_reg(dev, 0x36a3,0x78);
    ret |= sc450ai_write_reg(dev, 0x36b0,0x53);
    ret |= sc450ai_write_reg(dev, 0x36b1,0x74);
    ret |= sc450ai_write_reg(dev, 0x36b2,0x34);
    ret |= sc450ai_write_reg(dev, 0x36b3,0x40);
    ret |= sc450ai_write_reg(dev, 0x36b4,0x78);
    ret |= sc450ai_write_reg(dev, 0x36b7,0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b8,0xa0);
    ret |= sc450ai_write_reg(dev, 0x36b9,0x20);
    ret |= sc450ai_write_reg(dev, 0x36bd,0x40);
    ret |= sc450ai_write_reg(dev, 0x36be,0x48);
    ret |= sc450ai_write_reg(dev, 0x36d0,0x20);
    ret |= sc450ai_write_reg(dev, 0x36e0,0x08);
    ret |= sc450ai_write_reg(dev, 0x36e1,0x08);
    ret |= sc450ai_write_reg(dev, 0x36e2,0x12);
    ret |= sc450ai_write_reg(dev, 0x36e3,0x48);
    ret |= sc450ai_write_reg(dev, 0x36e4,0x78);
    ret |= sc450ai_write_reg(dev, 0x36ea,0x0f);
    ret |= sc450ai_write_reg(dev, 0x36eb,0x05);
    ret |= sc450ai_write_reg(dev, 0x36ec,0x43);
    ret |= sc450ai_write_reg(dev, 0x36ed,0x14);
    ret |= sc450ai_write_reg(dev, 0x36fa,0xcd);
    ret |= sc450ai_write_reg(dev, 0x36fb,0xb4);
    ret |= sc450ai_write_reg(dev, 0x36fc,0x00);
    ret |= sc450ai_write_reg(dev, 0x36fd,0x04);
    ret |= sc450ai_write_reg(dev, 0x3907,0x00);
    ret |= sc450ai_write_reg(dev, 0x3908,0x41);
    ret |= sc450ai_write_reg(dev, 0x391e,0xf1);
    ret |= sc450ai_write_reg(dev, 0x391f,0x11);
    ret |= sc450ai_write_reg(dev, 0x3933,0x82);
    ret |= sc450ai_write_reg(dev, 0x3934,0x30);
    ret |= sc450ai_write_reg(dev, 0x3935,0x02);
    ret |= sc450ai_write_reg(dev, 0x3936,0xc7);
    ret |= sc450ai_write_reg(dev, 0x3937,0x76);
    ret |= sc450ai_write_reg(dev, 0x3938,0x76);
    ret |= sc450ai_write_reg(dev, 0x3939,0x00);
    ret |= sc450ai_write_reg(dev, 0x393a,0x28);
    ret |= sc450ai_write_reg(dev, 0x393b,0x00);
    ret |= sc450ai_write_reg(dev, 0x393c,0x23);
    ret |= sc450ai_write_reg(dev, 0x3e01,0xc2);
    ret |= sc450ai_write_reg(dev, 0x3e02,0x60);
    ret |= sc450ai_write_reg(dev, 0x3e03,0x0b);
    ret |= sc450ai_write_reg(dev, 0x3e08,0x03);
    ret |= sc450ai_write_reg(dev, 0x3e1b,0x2a);
    ret |= sc450ai_write_reg(dev, 0x440e,0x02);
    ret |= sc450ai_write_reg(dev, 0x4509,0x20);
    ret |= sc450ai_write_reg(dev, 0x4837,0x16);
    ret |= sc450ai_write_reg(dev, 0x5000,0x0e);
    ret |= sc450ai_write_reg(dev, 0x5001,0x44);
    ret |= sc450ai_write_reg(dev, 0x5780,0x76);
    ret |= sc450ai_write_reg(dev, 0x5784,0x08);
    ret |= sc450ai_write_reg(dev, 0x5785,0x04);
    ret |= sc450ai_write_reg(dev, 0x5787,0x0a);
    ret |= sc450ai_write_reg(dev, 0x5788,0x0a);
    ret |= sc450ai_write_reg(dev, 0x5789,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578a,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578b,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578c,0x0a);
    ret |= sc450ai_write_reg(dev, 0x578d,0x40);
    ret |= sc450ai_write_reg(dev, 0x5790,0x08);
    ret |= sc450ai_write_reg(dev, 0x5791,0x04);
    ret |= sc450ai_write_reg(dev, 0x5792,0x04);
    ret |= sc450ai_write_reg(dev, 0x5793,0x08);
    ret |= sc450ai_write_reg(dev, 0x5794,0x04);
    ret |= sc450ai_write_reg(dev, 0x5795,0x04);
    ret |= sc450ai_write_reg(dev, 0x5799,0x46);
    ret |= sc450ai_write_reg(dev, 0x579a,0x77);
    ret |= sc450ai_write_reg(dev, 0x57a1,0x04);
    ret |= sc450ai_write_reg(dev, 0x57a8,0xd0);
    ret |= sc450ai_write_reg(dev, 0x57aa,0x2a);
    ret |= sc450ai_write_reg(dev, 0x57ab,0x7f);
    ret |= sc450ai_write_reg(dev, 0x57ac,0x00);
    ret |= sc450ai_write_reg(dev, 0x57ad,0x00);
    ret |= sc450ai_write_reg(dev, 0x59e0,0xfe);
    ret |= sc450ai_write_reg(dev, 0x59e1,0x40);
    ret |= sc450ai_write_reg(dev, 0x59e2,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e3,0x38);
    ret |= sc450ai_write_reg(dev, 0x59e4,0x30);
    ret |= sc450ai_write_reg(dev, 0x59e5,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e6,0x38);
    ret |= sc450ai_write_reg(dev, 0x59e7,0x30);
    ret |= sc450ai_write_reg(dev, 0x59e8,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59e9,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ea,0x38);
    ret |= sc450ai_write_reg(dev, 0x59eb,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59ec,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ed,0x38);
    ret |= sc450ai_write_reg(dev, 0x59ee,0xfe);
    ret |= sc450ai_write_reg(dev, 0x59ef,0x40);
    ret |= sc450ai_write_reg(dev, 0x59f4,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f5,0x38);
    ret |= sc450ai_write_reg(dev, 0x59f6,0x30);
    ret |= sc450ai_write_reg(dev, 0x59f7,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59f8,0x38);
    ret |= sc450ai_write_reg(dev, 0x59f9,0x30);
    ret |= sc450ai_write_reg(dev, 0x59fa,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fb,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59fc,0x38);
    ret |= sc450ai_write_reg(dev, 0x59fd,0x3f);
    ret |= sc450ai_write_reg(dev, 0x59fe,0x3c);
    ret |= sc450ai_write_reg(dev, 0x59ff,0x38);
    ret |= sc450ai_write_reg(dev, 0x36e9,0x20);
    ret |= sc450ai_write_reg(dev, 0x36f9,0x53);
    ret |= sc450ai_write_reg(dev, 0x0100,0x01);


    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc450ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC450AI_24MInput_MIPI_2lane_10bit_2688x1520_30fps init success!========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC450AI_4M_1440_10BIT_LINEAR_MODE:
            ret = sc450ai_2lane_linear_2560x1440_init(dev);
            break;
        case SC450AI_4M_1440_10BIT_WDR_MODE:
            ret = sc450ai_4lane_wdr_2560x1440_init(dev);
            break;
        case SC450AI_4M_1520_10BIT_LINEAR_MODE:
            ret = sc450ai_2lane_linear_2688x1520_init(dev);
            break;
        case SC450AI_1M_760_10BIT_LINEAR_MODE:
            ret = sc450ai_4lane_linear_1344x760_binning_120fps_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_sc450ai_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc450ai_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc450ai_i2c_fd[dev]);
        g_sc450ai_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc450ai_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc450ai_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc450ai_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc450ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc450ai_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc450ai_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc450ai_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC450AI_DATA_BYTE];

    if (g_sc450ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc450ai_i2c_fd[dev], g_sc450ai_i2c_addr[dev], addr, SC450AI_ADDR_BYTE, buf, SC450AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC450AI_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc450ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;
    ret = i2c_write(g_sc450ai_i2c_fd[dev], g_sc450ai_i2c_addr[dev], buf, SC450AI_ADDR_BYTE + SC450AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error %#x %#x!\n", addr, data);
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
xmedia_s32 sc450ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc450ai_read_reg(dev, SC450AI_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc450ai_write_reg(dev, SC450AI_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc450ai_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
