#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc200ai_ctrl.h"

#define sc200ai_REG_ADDR_MIRROR_FLIP 0x3221
static xmedia_s32 g_sc200ai_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
static xmedia_s32 g_sc200ai_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

/*
 * 函数功能: sc200ai MIPI 2lanes 720p线性初始化序列 - 1280x720@21.548fps
 * 函数参数：
 *      dev -  senor 设备号
 * 返回值：
 *
 */
 #ifdef FPGA
xmedia_s32 sc200ai_2lane_linear_1280x720_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc200ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc200ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc200ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc200ai_write_reg(dev, 0x301f, 0xf9);
    ret |= sc200ai_write_reg(dev, 0x3200, 0x01);
    ret |= sc200ai_write_reg(dev, 0x3201, 0x40);
    ret |= sc200ai_write_reg(dev, 0x3202, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3203, 0xb4);
    ret |= sc200ai_write_reg(dev, 0x3204, 0x06);
    ret |= sc200ai_write_reg(dev, 0x3205, 0x47);
    ret |= sc200ai_write_reg(dev, 0x3206, 0x03);
    ret |= sc200ai_write_reg(dev, 0x3207, 0x8b);
    ret |= sc200ai_write_reg(dev, 0x3208, 0x05);
    ret |= sc200ai_write_reg(dev, 0x3209, 0x00);
    ret |= sc200ai_write_reg(dev, 0x320a, 0x02);
    ret |= sc200ai_write_reg(dev, 0x320b, 0xd0);
    ret |= sc200ai_write_reg(dev, 0x320c, 0x03);
    ret |= sc200ai_write_reg(dev, 0x320d, 0xde);
    ret |= sc200ai_write_reg(dev, 0x320e, 0x02);
    ret |= sc200ai_write_reg(dev, 0x320f, 0xee);
    ret |= sc200ai_write_reg(dev, 0x3210, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3211, 0x04);
    ret |= sc200ai_write_reg(dev, 0x3212, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3213, 0x04);
    ret |= sc200ai_write_reg(dev, 0x3243, 0x01);
    ret |= sc200ai_write_reg(dev, 0x3248, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3249, 0x09);
    ret |= sc200ai_write_reg(dev, 0x3253, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3271, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x3301, 0x06);
    ret |= sc200ai_write_reg(dev, 0x3302, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x3303, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3304, 0x50);
    ret |= sc200ai_write_reg(dev, 0x3306, 0x28);
    ret |= sc200ai_write_reg(dev, 0x3308, 0x10);
    ret |= sc200ai_write_reg(dev, 0x3309, 0x60);
    ret |= sc200ai_write_reg(dev, 0x330b, 0x78);
    ret |= sc200ai_write_reg(dev, 0x330d, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x330e, 0x1c);
    ret |= sc200ai_write_reg(dev, 0x330f, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3310, 0x02);
    ret |= sc200ai_write_reg(dev, 0x331c, 0x04);
    ret |= sc200ai_write_reg(dev, 0x331e, 0x41);
    ret |= sc200ai_write_reg(dev, 0x331f, 0x51);
    ret |= sc200ai_write_reg(dev, 0x3320, 0x07);
    ret |= sc200ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc200ai_write_reg(dev, 0x334c, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3356, 0x09);
    ret |= sc200ai_write_reg(dev, 0x3364, 0x17);
    ret |= sc200ai_write_reg(dev, 0x3390, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3391, 0x18);
    ret |= sc200ai_write_reg(dev, 0x3392, 0x38);
    ret |= sc200ai_write_reg(dev, 0x3393, 0x06);
    ret |= sc200ai_write_reg(dev, 0x3394, 0x06);
    ret |= sc200ai_write_reg(dev, 0x3395, 0x06);
    ret |= sc200ai_write_reg(dev, 0x3396, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3397, 0x18);
    ret |= sc200ai_write_reg(dev, 0x3398, 0x38);
    ret |= sc200ai_write_reg(dev, 0x3399, 0x06);
    ret |= sc200ai_write_reg(dev, 0x339a, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x339b, 0x10);
    ret |= sc200ai_write_reg(dev, 0x339c, 0x20);
    ret |= sc200ai_write_reg(dev, 0x33ac, 0x08);
    ret |= sc200ai_write_reg(dev, 0x33ae, 0x10);
    ret |= sc200ai_write_reg(dev, 0x33af, 0x19);
    ret |= sc200ai_write_reg(dev, 0x3621, 0xe8);
    ret |= sc200ai_write_reg(dev, 0x3622, 0x16);
    ret |= sc200ai_write_reg(dev, 0x3630, 0xa0);
    ret |= sc200ai_write_reg(dev, 0x3637, 0x36);
    ret |= sc200ai_write_reg(dev, 0x363a, 0x1f);
    ret |= sc200ai_write_reg(dev, 0x363b, 0xc6);
    ret |= sc200ai_write_reg(dev, 0x363c, 0x0e);
    ret |= sc200ai_write_reg(dev, 0x3670, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x3674, 0x82);
    ret |= sc200ai_write_reg(dev, 0x3675, 0x76);
    ret |= sc200ai_write_reg(dev, 0x3676, 0x78);
    ret |= sc200ai_write_reg(dev, 0x367c, 0x48);
    ret |= sc200ai_write_reg(dev, 0x367d, 0x58);
    ret |= sc200ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc200ai_write_reg(dev, 0x3691, 0x33);
    ret |= sc200ai_write_reg(dev, 0x3692, 0x44);
    ret |= sc200ai_write_reg(dev, 0x369c, 0x40);
    ret |= sc200ai_write_reg(dev, 0x369d, 0x48);
    ret |= sc200ai_write_reg(dev, 0x36ea, 0x36);
    ret |= sc200ai_write_reg(dev, 0x36eb, 0x3c);
    ret |= sc200ai_write_reg(dev, 0x36ec, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x36ed, 0x14);
    ret |= sc200ai_write_reg(dev, 0x36fa, 0x38);
    ret |= sc200ai_write_reg(dev, 0x36fb, 0x00);
    ret |= sc200ai_write_reg(dev, 0x36fc, 0x11);
    ret |= sc200ai_write_reg(dev, 0x36fd, 0x14);
    ret |= sc200ai_write_reg(dev, 0x3901, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3904, 0x04);
    ret |= sc200ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc200ai_write_reg(dev, 0x391f, 0x10);
    ret |= sc200ai_write_reg(dev, 0x3e01, 0x5d);
    ret |= sc200ai_write_reg(dev, 0x3e02, 0x40);
    ret |= sc200ai_write_reg(dev, 0x3e16, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e17, 0x80);
    ret |= sc200ai_write_reg(dev, 0x3f09, 0x48);
    ret |= sc200ai_write_reg(dev, 0x4800, 0x44);
    ret |= sc200ai_write_reg(dev, 0x4819, 0x03);
    ret |= sc200ai_write_reg(dev, 0x481b, 0x02);
    ret |= sc200ai_write_reg(dev, 0x481d, 0x05);
    ret |= sc200ai_write_reg(dev, 0x481f, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4821, 0x07);
    ret |= sc200ai_write_reg(dev, 0x4823, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4825, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4827, 0x02);
    ret |= sc200ai_write_reg(dev, 0x4829, 0x02);
    ret |= sc200ai_write_reg(dev, 0x5787, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5788, 0x06);
    ret |= sc200ai_write_reg(dev, 0x578a, 0x10);
    ret |= sc200ai_write_reg(dev, 0x578b, 0x06);
    ret |= sc200ai_write_reg(dev, 0x5790, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5791, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5792, 0x00);
    ret |= sc200ai_write_reg(dev, 0x5793, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5794, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5795, 0x00);
    ret |= sc200ai_write_reg(dev, 0x5799, 0x00);
    ret |= sc200ai_write_reg(dev, 0x57c7, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57c8, 0x06);
    ret |= sc200ai_write_reg(dev, 0x57ca, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57cb, 0x06);
    ret |= sc200ai_write_reg(dev, 0x57d1, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57d4, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57d9, 0x00);
    ret |= sc200ai_write_reg(dev, 0x59e0, 0x60);
    ret |= sc200ai_write_reg(dev, 0x59e1, 0x08);
    ret |= sc200ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e3, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59e4, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e6, 0x06);
    ret |= sc200ai_write_reg(dev, 0x59e7, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59e8, 0x38);
    ret |= sc200ai_write_reg(dev, 0x59e9, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59ea, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59eb, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59ec, 0x04);
    ret |= sc200ai_write_reg(dev, 0x59ed, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59ee, 0xa0);
    ret |= sc200ai_write_reg(dev, 0x59ef, 0x08);
    ret |= sc200ai_write_reg(dev, 0x59f4, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59f5, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59f6, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59f7, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59f8, 0x06);
    ret |= sc200ai_write_reg(dev, 0x59f9, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59fa, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59fb, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59fc, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59fd, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59fe, 0x04);
    ret |= sc200ai_write_reg(dev, 0x59ff, 0x02);
    ret |= sc200ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc200ai_write_reg(dev, 0x36f9, 0x44);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x01);

    printf("============================================================================================\n");
    printf("===== SC200AI_MIPI_24Minput_160Mbps_2lane_10bit_1280x720_21.548fps linear init success! ====\n");
    printf("============================================================================================\n");
    return ret;
}

/*
 * 函数功能: sc200ai MIPI 2lanes 1080p线性初始化序列 - 1920x1080@12.929fps
 * 函数参数：
 *      dev -  senor 设备号
 * 返回值：
 *
 */
xmedia_s32 sc200ai_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc200ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc200ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc200ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc200ai_write_reg(dev, 0x301f, 0xf8);
    ret |= sc200ai_write_reg(dev, 0x3243, 0x01);
    ret |= sc200ai_write_reg(dev, 0x3248, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3249, 0x09);
    ret |= sc200ai_write_reg(dev, 0x3253, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3271, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x3301, 0x20);
    ret |= sc200ai_write_reg(dev, 0x3304, 0x40);
    ret |= sc200ai_write_reg(dev, 0x3306, 0x32);
    ret |= sc200ai_write_reg(dev, 0x330b, 0x88);
    ret |= sc200ai_write_reg(dev, 0x330f, 0x02);
    ret |= sc200ai_write_reg(dev, 0x331e, 0x39);
    ret |= sc200ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc200ai_write_reg(dev, 0x3621, 0xe8);
    ret |= sc200ai_write_reg(dev, 0x3622, 0x16);
    ret |= sc200ai_write_reg(dev, 0x3637, 0x1b);
    ret |= sc200ai_write_reg(dev, 0x363a, 0x1f);
    ret |= sc200ai_write_reg(dev, 0x363b, 0xc6);
    ret |= sc200ai_write_reg(dev, 0x363c, 0x0e);
    ret |= sc200ai_write_reg(dev, 0x3670, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x3674, 0x82);
    ret |= sc200ai_write_reg(dev, 0x3675, 0x76);
    ret |= sc200ai_write_reg(dev, 0x3676, 0x78);
    ret |= sc200ai_write_reg(dev, 0x367c, 0x48);
    ret |= sc200ai_write_reg(dev, 0x367d, 0x58);
    ret |= sc200ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc200ai_write_reg(dev, 0x3691, 0x33);
    ret |= sc200ai_write_reg(dev, 0x3692, 0x44);
    ret |= sc200ai_write_reg(dev, 0x369c, 0x40);
    ret |= sc200ai_write_reg(dev, 0x369d, 0x48);
    ret |= sc200ai_write_reg(dev, 0x36ea, 0x36);
    ret |= sc200ai_write_reg(dev, 0x36eb, 0x3c);
    ret |= sc200ai_write_reg(dev, 0x36ec, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x36ed, 0x14);
    ret |= sc200ai_write_reg(dev, 0x36fa, 0x38);
    ret |= sc200ai_write_reg(dev, 0x36fb, 0x00);
    ret |= sc200ai_write_reg(dev, 0x36fc, 0x11);
    ret |= sc200ai_write_reg(dev, 0x36fd, 0x14);
    ret |= sc200ai_write_reg(dev, 0x3901, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3904, 0x04);
    ret |= sc200ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc200ai_write_reg(dev, 0x391d, 0x14);
    ret |= sc200ai_write_reg(dev, 0x391f, 0x18);
    ret |= sc200ai_write_reg(dev, 0x3e01, 0x8c);
    ret |= sc200ai_write_reg(dev, 0x3e02, 0x20);
    ret |= sc200ai_write_reg(dev, 0x3e16, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e17, 0x80);
    ret |= sc200ai_write_reg(dev, 0x3f09, 0x48);
    ret |= sc200ai_write_reg(dev, 0x4819, 0x03);
    ret |= sc200ai_write_reg(dev, 0x481b, 0x02);
    ret |= sc200ai_write_reg(dev, 0x481d, 0x05);
    ret |= sc200ai_write_reg(dev, 0x481f, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4821, 0x07);
    ret |= sc200ai_write_reg(dev, 0x4823, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4825, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4827, 0x02);
    ret |= sc200ai_write_reg(dev, 0x4829, 0x02);
    ret |= sc200ai_write_reg(dev, 0x5787, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5788, 0x06);
    ret |= sc200ai_write_reg(dev, 0x578a, 0x10);
    ret |= sc200ai_write_reg(dev, 0x578b, 0x06);
    ret |= sc200ai_write_reg(dev, 0x5790, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5791, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5792, 0x00);
    ret |= sc200ai_write_reg(dev, 0x5793, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5794, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5795, 0x00);
    ret |= sc200ai_write_reg(dev, 0x5799, 0x00);
    ret |= sc200ai_write_reg(dev, 0x57c7, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57c8, 0x06);
    ret |= sc200ai_write_reg(dev, 0x57ca, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57cb, 0x06);
    ret |= sc200ai_write_reg(dev, 0x57d1, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57d4, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57d9, 0x00);
    ret |= sc200ai_write_reg(dev, 0x59e0, 0x60);
    ret |= sc200ai_write_reg(dev, 0x59e1, 0x08);
    ret |= sc200ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e3, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59e4, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e6, 0x06);
    ret |= sc200ai_write_reg(dev, 0x59e7, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59e8, 0x38);
    ret |= sc200ai_write_reg(dev, 0x59e9, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59ea, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59eb, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59ec, 0x04);
    ret |= sc200ai_write_reg(dev, 0x59ed, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59ee, 0xa0);
    ret |= sc200ai_write_reg(dev, 0x59ef, 0x08);
    ret |= sc200ai_write_reg(dev, 0x59f4, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59f5, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59f6, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59f7, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59f8, 0x06);
    ret |= sc200ai_write_reg(dev, 0x59f9, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59fa, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59fb, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59fc, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59fd, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59fe, 0x04);
    ret |= sc200ai_write_reg(dev, 0x59ff, 0x02);
    ret |= sc200ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc200ai_write_reg(dev, 0x36f9, 0x44);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x01);

    printf("===========================================================================================\n");
    printf("===== SC200AI_MIPI_24Minput_160Mbps_2lane_10bit_1920x1080_12.929fps linear success! =======\n");
    printf("===========================================================================================\n");
    return ret;
}

xmedia_s32 sc200ai_2lane_wdr_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc200ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc200ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc200ai_write_reg(dev, 0x301f, 0xf7);
    ret |= sc200ai_write_reg(dev, 0x320e, 0x08);
    ret |= sc200ai_write_reg(dev, 0x320f, 0xcc);
    ret |= sc200ai_write_reg(dev, 0x3220, 0x53);
    ret |= sc200ai_write_reg(dev, 0x3243, 0x01);
    ret |= sc200ai_write_reg(dev, 0x3248, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3249, 0x09);
    ret |= sc200ai_write_reg(dev, 0x3250, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x3253, 0x08);
    ret |= sc200ai_write_reg(dev, 0x3271, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x3301, 0x20);
    ret |= sc200ai_write_reg(dev, 0x3304, 0x40);
    ret |= sc200ai_write_reg(dev, 0x3306, 0x32);
    ret |= sc200ai_write_reg(dev, 0x330b, 0x88);
    ret |= sc200ai_write_reg(dev, 0x330f, 0x02);
    ret |= sc200ai_write_reg(dev, 0x331e, 0x39);
    ret |= sc200ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc200ai_write_reg(dev, 0x3347, 0x77);
    ret |= sc200ai_write_reg(dev, 0x336c, 0xcc);
    ret |= sc200ai_write_reg(dev, 0x3621, 0xe8);
    ret |= sc200ai_write_reg(dev, 0x3622, 0x16);
    ret |= sc200ai_write_reg(dev, 0x3637, 0x1b);
    ret |= sc200ai_write_reg(dev, 0x363a, 0x1f);
    ret |= sc200ai_write_reg(dev, 0x363b, 0xc6);
    ret |= sc200ai_write_reg(dev, 0x363c, 0x0e);
    ret |= sc200ai_write_reg(dev, 0x3670, 0x0a);
    ret |= sc200ai_write_reg(dev, 0x3674, 0x82);
    ret |= sc200ai_write_reg(dev, 0x3675, 0x76);
    ret |= sc200ai_write_reg(dev, 0x3676, 0x78);
    ret |= sc200ai_write_reg(dev, 0x367c, 0x48);
    ret |= sc200ai_write_reg(dev, 0x367d, 0x58);
    ret |= sc200ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc200ai_write_reg(dev, 0x3691, 0x33);
    ret |= sc200ai_write_reg(dev, 0x3692, 0x44);
    ret |= sc200ai_write_reg(dev, 0x369c, 0x40);
    ret |= sc200ai_write_reg(dev, 0x369d, 0x48);
    ret |= sc200ai_write_reg(dev, 0x36ea, 0x36);
    ret |= sc200ai_write_reg(dev, 0x36eb, 0x3c);
    ret |= sc200ai_write_reg(dev, 0x36ec, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x36ed, 0x14);
    ret |= sc200ai_write_reg(dev, 0x36fa, 0x38);
    ret |= sc200ai_write_reg(dev, 0x36fb, 0x00);
    ret |= sc200ai_write_reg(dev, 0x36fc, 0x11);
    ret |= sc200ai_write_reg(dev, 0x36fd, 0x14);
    ret |= sc200ai_write_reg(dev, 0x3901, 0x02);
    ret |= sc200ai_write_reg(dev, 0x3904, 0x04);
    ret |= sc200ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc200ai_write_reg(dev, 0x391d, 0x14);
    ret |= sc200ai_write_reg(dev, 0x391f, 0x18);
    ret |= sc200ai_write_reg(dev, 0x3e00, 0x01);
    ret |= sc200ai_write_reg(dev, 0x3e01, 0x06);
    ret |= sc200ai_write_reg(dev, 0x3e02, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e04, 0x10);
    ret |= sc200ai_write_reg(dev, 0x3e05, 0x60);
    ret |= sc200ai_write_reg(dev, 0x3e06, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e07, 0x80);
    ret |= sc200ai_write_reg(dev, 0x3e08, 0x03);
    ret |= sc200ai_write_reg(dev, 0x3e09, 0x40);
    ret |= sc200ai_write_reg(dev, 0x3e10, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e11, 0x80);
    ret |= sc200ai_write_reg(dev, 0x3e12, 0x03);
    ret |= sc200ai_write_reg(dev, 0x3e13, 0x40);
    ret |= sc200ai_write_reg(dev, 0x3e16, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e17, 0x80);
    ret |= sc200ai_write_reg(dev, 0x3e23, 0x00);
    ret |= sc200ai_write_reg(dev, 0x3e24, 0x88);
    ret |= sc200ai_write_reg(dev, 0x3f09, 0x48);
    ret |= sc200ai_write_reg(dev, 0x4816, 0xb1);
    ret |= sc200ai_write_reg(dev, 0x4819, 0x03);
    ret |= sc200ai_write_reg(dev, 0x481b, 0x02);
    ret |= sc200ai_write_reg(dev, 0x481d, 0x05);
    ret |= sc200ai_write_reg(dev, 0x481f, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4821, 0x07);
    ret |= sc200ai_write_reg(dev, 0x4823, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4825, 0x01);
    ret |= sc200ai_write_reg(dev, 0x4827, 0x02);
    ret |= sc200ai_write_reg(dev, 0x4829, 0x02);
    ret |= sc200ai_write_reg(dev, 0x5787, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5788, 0x06);
    ret |= sc200ai_write_reg(dev, 0x578a, 0x10);
    ret |= sc200ai_write_reg(dev, 0x578b, 0x06);
    ret |= sc200ai_write_reg(dev, 0x5790, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5791, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5792, 0x00);
    ret |= sc200ai_write_reg(dev, 0x5793, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5794, 0x10);
    ret |= sc200ai_write_reg(dev, 0x5795, 0x00);
    ret |= sc200ai_write_reg(dev, 0x5799, 0x00);
    ret |= sc200ai_write_reg(dev, 0x57c7, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57c8, 0x06);
    ret |= sc200ai_write_reg(dev, 0x57ca, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57cb, 0x06);
    ret |= sc200ai_write_reg(dev, 0x57d1, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57d4, 0x10);
    ret |= sc200ai_write_reg(dev, 0x57d9, 0x00);
    ret |= sc200ai_write_reg(dev, 0x59e0, 0x60);
    ret |= sc200ai_write_reg(dev, 0x59e1, 0x08);
    ret |= sc200ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e3, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59e4, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e6, 0x06);
    ret |= sc200ai_write_reg(dev, 0x59e7, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59e8, 0x38);
    ret |= sc200ai_write_reg(dev, 0x59e9, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59ea, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59eb, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59ec, 0x04);
    ret |= sc200ai_write_reg(dev, 0x59ed, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59ee, 0xa0);
    ret |= sc200ai_write_reg(dev, 0x59ef, 0x08);
    ret |= sc200ai_write_reg(dev, 0x59f4, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59f5, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59f6, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59f7, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59f8, 0x06);
    ret |= sc200ai_write_reg(dev, 0x59f9, 0x02);
    ret |= sc200ai_write_reg(dev, 0x59fa, 0x18);
    ret |= sc200ai_write_reg(dev, 0x59fb, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59fc, 0x0c);
    ret |= sc200ai_write_reg(dev, 0x59fd, 0x10);
    ret |= sc200ai_write_reg(dev, 0x59fe, 0x04);
    ret |= sc200ai_write_reg(dev, 0x59ff, 0x02);
    ret |= sc200ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc200ai_write_reg(dev, 0x36f9, 0x44);

    return ret;
}

/*
 * 函数功能: sc200ai MIPI 2lanes 720p WDR初始化序列 -从 1080p wdr crop
 * 函数参数：
 *      dev -  senor 设备号
 * 返回值：
 *
 */
xmedia_s32 sc200ai_2lane_wdr_1280x720_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc200ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc200ai_2lane_wdr_comm_init(dev);
    ret |= sc200ai_write_reg(dev, 0x3208, 0x05);
    ret |= sc200ai_write_reg(dev, 0x3209, 0x00); // output window width
    ret |= sc200ai_write_reg(dev, 0x320a, 0x02);
    ret |= sc200ai_write_reg(dev, 0x320b, 0xd0); // output window height
    ret |= sc200ai_write_reg(dev, 0x0100, 0x01);

    printf("===========================================================================================\n");
    printf("===== SC200AI_MIPI_VC_24Minput_160Mbps_2lane_10bit_1280x720_6.459fps wdr success! =========\n");
    printf("===========================================================================================\n");
    return ret;
}

/*
 * 函数功能: sc200ai MIPI 2lanes 1080p线性初始化序列 - 1920x1080@6.459fps
 * 函数参数：
 *      dev -  senor 设备号
 * 返回值：
 *
 */
xmedia_s32 sc200ai_2lane_wdr_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc200ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc200ai_2lane_wdr_comm_init(dev);
    ret |= sc200ai_write_reg(dev, 0x0100, 0x01);

    printf("===========================================================================================\n");
    printf("===== SC200AI_MIPI_24Minput_160Mbps_2lane_10bit_1920x1080_6.459fps wdr success! ===========\n");
    printf("===========================================================================================\n");
    return ret;
}
#else
xmedia_void sc200ai_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000);
}

xmedia_s32 sc200ai_2lane_linear_1280x720_init(xmedia_u32 dev)
{
    SENSOR_PRINT("======================================================================================\n");
    SENSOR_PRINT("==================================== SC200AI LINEAR NOT SUPPORT! =====================\n");
    SENSOR_PRINT("======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

xmedia_s32 sc200ai_2lane_wdr_1280x720_init(xmedia_u32 dev)
{
    SENSOR_PRINT("======================================================================================\n");
    SENSOR_PRINT("==================================== SC200AI WDR NOT SUPPORT! ========================\n");
    SENSOR_PRINT("======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

xmedia_s32 sc200ai_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    //cleaned_0x03_FT_SC200AI_MIPI_27MInput_371.25Mbps_2lane_10bit_1920x1080_30fps
    ret |= sc200ai_write_reg(dev, 0x0103,0x01);
    ret |= sc200ai_write_reg(dev, 0x0100,0x00);
    ret |= sc200ai_write_reg(dev, 0x36e9,0x80);
    ret |= sc200ai_write_reg(dev, 0x36f9,0x80);
    ret |= sc200ai_write_reg(dev, 0x301f,0x03);
    ret |= sc200ai_write_reg(dev, 0x320e,0x04);
    ret |= sc200ai_write_reg(dev, 0x320f,0x65);
    ret |= sc200ai_write_reg(dev, 0x3243,0x01);
    ret |= sc200ai_write_reg(dev, 0x3248,0x02);
    ret |= sc200ai_write_reg(dev, 0x3249,0x09);
    ret |= sc200ai_write_reg(dev, 0x3253,0x08);
    ret |= sc200ai_write_reg(dev, 0x3271,0x0a);
    ret |= sc200ai_write_reg(dev, 0x3301,0x20);
    ret |= sc200ai_write_reg(dev, 0x3304,0x40);
    ret |= sc200ai_write_reg(dev, 0x3306,0x32);
    ret |= sc200ai_write_reg(dev, 0x330b,0x88);
    ret |= sc200ai_write_reg(dev, 0x330f,0x02);
    ret |= sc200ai_write_reg(dev, 0x331e,0x39);
    ret |= sc200ai_write_reg(dev, 0x3333,0x10);
    ret |= sc200ai_write_reg(dev, 0x3621,0xe8);
    ret |= sc200ai_write_reg(dev, 0x3622,0x16);
    ret |= sc200ai_write_reg(dev, 0x3637,0x1b);
    ret |= sc200ai_write_reg(dev, 0x363a,0x1f);
    ret |= sc200ai_write_reg(dev, 0x363b,0xc6);
    ret |= sc200ai_write_reg(dev, 0x363c,0x0e);
    ret |= sc200ai_write_reg(dev, 0x3670,0x0a);
    ret |= sc200ai_write_reg(dev, 0x3674,0x82);
    ret |= sc200ai_write_reg(dev, 0x3675,0x76);
    ret |= sc200ai_write_reg(dev, 0x3676,0x78);
    ret |= sc200ai_write_reg(dev, 0x367c,0x48);
    ret |= sc200ai_write_reg(dev, 0x367d,0x58);
    ret |= sc200ai_write_reg(dev, 0x3690,0x34);
    ret |= sc200ai_write_reg(dev, 0x3691,0x33);
    ret |= sc200ai_write_reg(dev, 0x3692,0x44);
    ret |= sc200ai_write_reg(dev, 0x369c,0x40);
    ret |= sc200ai_write_reg(dev, 0x369d,0x48);
    ret |= sc200ai_write_reg(dev, 0x3901,0x02);
    ret |= sc200ai_write_reg(dev, 0x3904,0x04);
    ret |= sc200ai_write_reg(dev, 0x3908,0x41);
    ret |= sc200ai_write_reg(dev, 0x391d,0x14);
    ret |= sc200ai_write_reg(dev, 0x391f,0x18);
    ret |= sc200ai_write_reg(dev, 0x3e01,0x8c);
    ret |= sc200ai_write_reg(dev, 0x3e02,0x20);
    ret |= sc200ai_write_reg(dev, 0x3e16,0x00);
    ret |= sc200ai_write_reg(dev, 0x3e17,0x80);
    ret |= sc200ai_write_reg(dev, 0x3f09,0x48);
    ret |= sc200ai_write_reg(dev, 0x5787,0x10);
    ret |= sc200ai_write_reg(dev, 0x5788,0x06);
    ret |= sc200ai_write_reg(dev, 0x578a,0x10);
    ret |= sc200ai_write_reg(dev, 0x578b,0x06);
    ret |= sc200ai_write_reg(dev, 0x5790,0x10);
    ret |= sc200ai_write_reg(dev, 0x5791,0x10);
    ret |= sc200ai_write_reg(dev, 0x5792,0x00);
    ret |= sc200ai_write_reg(dev, 0x5793,0x10);
    ret |= sc200ai_write_reg(dev, 0x5794,0x10);
    ret |= sc200ai_write_reg(dev, 0x5795,0x00);
    ret |= sc200ai_write_reg(dev, 0x5799,0x00);
    ret |= sc200ai_write_reg(dev, 0x57c7,0x10);
    ret |= sc200ai_write_reg(dev, 0x57c8,0x06);
    ret |= sc200ai_write_reg(dev, 0x57ca,0x10);
    ret |= sc200ai_write_reg(dev, 0x57cb,0x06);
    ret |= sc200ai_write_reg(dev, 0x57d1,0x10);
    ret |= sc200ai_write_reg(dev, 0x57d4,0x10);
    ret |= sc200ai_write_reg(dev, 0x57d9,0x00);
    ret |= sc200ai_write_reg(dev, 0x59e0,0x60);
    ret |= sc200ai_write_reg(dev, 0x59e1,0x08);
    ret |= sc200ai_write_reg(dev, 0x59e2,0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e3,0x18);
    ret |= sc200ai_write_reg(dev, 0x59e4,0x18);
    ret |= sc200ai_write_reg(dev, 0x59e5,0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e6,0x06);
    ret |= sc200ai_write_reg(dev, 0x59e7,0x02);
    ret |= sc200ai_write_reg(dev, 0x59e8,0x38);
    ret |= sc200ai_write_reg(dev, 0x59e9,0x10);
    ret |= sc200ai_write_reg(dev, 0x59ea,0x0c);
    ret |= sc200ai_write_reg(dev, 0x59eb,0x10);
    ret |= sc200ai_write_reg(dev, 0x59ec,0x04);
    ret |= sc200ai_write_reg(dev, 0x59ed,0x02);
    ret |= sc200ai_write_reg(dev, 0x59ee,0xa0);
    ret |= sc200ai_write_reg(dev, 0x59ef,0x08);
    ret |= sc200ai_write_reg(dev, 0x59f4,0x18);
    ret |= sc200ai_write_reg(dev, 0x59f5,0x10);
    ret |= sc200ai_write_reg(dev, 0x59f6,0x0c);
    ret |= sc200ai_write_reg(dev, 0x59f7,0x10);
    ret |= sc200ai_write_reg(dev, 0x59f8,0x06);
    ret |= sc200ai_write_reg(dev, 0x59f9,0x02);
    ret |= sc200ai_write_reg(dev, 0x59fa,0x18);
    ret |= sc200ai_write_reg(dev, 0x59fb,0x10);
    ret |= sc200ai_write_reg(dev, 0x59fc,0x0c);
    ret |= sc200ai_write_reg(dev, 0x59fd,0x10);
    ret |= sc200ai_write_reg(dev, 0x59fe,0x04);
    ret |= sc200ai_write_reg(dev, 0x59ff,0x02);
    ret |= sc200ai_write_reg(dev, 0x36e9,0x20);
    ret |= sc200ai_write_reg(dev, 0x36f9,0x27);

    sc200ai_delay_ms(20);
    ret |= sc200ai_write_reg(dev, 0x0100,0x01);

    printf("====================================================================================\n");
    printf("===sc200ai_27MInput_371.25Mbps_2lane sensor 1080P30fps 10bit linear init success!===\n");
    printf("====================================================================================\n");

    return ret;
}

xmedia_s32 sc200ai_2lane_wdr_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc200ai_write_reg(dev, 0x0103,0x01);
    ret |= sc200ai_write_reg(dev, 0x0100,0x00);
    ret |= sc200ai_write_reg(dev, 0x36e9,0x80);
    ret |= sc200ai_write_reg(dev, 0x36f9,0x80);
    ret |= sc200ai_write_reg(dev, 0x301f,0x02);
    ret |= sc200ai_write_reg(dev, 0x320e,0x08);
    ret |= sc200ai_write_reg(dev, 0x320f,0xca);
    ret |= sc200ai_write_reg(dev, 0x3220,0x53);
    ret |= sc200ai_write_reg(dev, 0x3243,0x01);
    ret |= sc200ai_write_reg(dev, 0x3248,0x02);
    ret |= sc200ai_write_reg(dev, 0x3249,0x09);
    ret |= sc200ai_write_reg(dev, 0x3250,0x3f);
    ret |= sc200ai_write_reg(dev, 0x3253,0x08);
    ret |= sc200ai_write_reg(dev, 0x3271,0x0a);
    ret |= sc200ai_write_reg(dev, 0x3301,0x06);
    ret |= sc200ai_write_reg(dev, 0x3302,0x0c);
    ret |= sc200ai_write_reg(dev, 0x3303,0x08);
    ret |= sc200ai_write_reg(dev, 0x3304,0x60);
    ret |= sc200ai_write_reg(dev, 0x3306,0x30);
    ret |= sc200ai_write_reg(dev, 0x3308,0x10);
    ret |= sc200ai_write_reg(dev, 0x3309,0x70);
    ret |= sc200ai_write_reg(dev, 0x330b,0x80);
    ret |= sc200ai_write_reg(dev, 0x330d,0x16);
    ret |= sc200ai_write_reg(dev, 0x330e,0x1c);
    ret |= sc200ai_write_reg(dev, 0x330f,0x02);
    ret |= sc200ai_write_reg(dev, 0x3310,0x02);
    ret |= sc200ai_write_reg(dev, 0x331c,0x04);
    ret |= sc200ai_write_reg(dev, 0x331e,0x51);
    ret |= sc200ai_write_reg(dev, 0x331f,0x61);
    ret |= sc200ai_write_reg(dev, 0x3320,0x07);
    ret |= sc200ai_write_reg(dev, 0x3333,0x10);
    ret |= sc200ai_write_reg(dev, 0x3347,0x77);
    ret |= sc200ai_write_reg(dev, 0x334c,0x08);
    ret |= sc200ai_write_reg(dev, 0x3356,0x09);
    ret |= sc200ai_write_reg(dev, 0x3364,0x17);
    ret |= sc200ai_write_reg(dev, 0x336c,0xcc);
    ret |= sc200ai_write_reg(dev, 0x3390,0x08);
    ret |= sc200ai_write_reg(dev, 0x3391,0x18);
    ret |= sc200ai_write_reg(dev, 0x3392,0x38);
    ret |= sc200ai_write_reg(dev, 0x3393,0x06);
    ret |= sc200ai_write_reg(dev, 0x3394,0x06);
    ret |= sc200ai_write_reg(dev, 0x3395,0x06);
    ret |= sc200ai_write_reg(dev, 0x3396,0x08);
    ret |= sc200ai_write_reg(dev, 0x3397,0x18);
    ret |= sc200ai_write_reg(dev, 0x3398,0x38);
    ret |= sc200ai_write_reg(dev, 0x3399,0x06);
    ret |= sc200ai_write_reg(dev, 0x339a,0x0a);
    ret |= sc200ai_write_reg(dev, 0x339b,0x10);
    ret |= sc200ai_write_reg(dev, 0x339c,0x20);
    ret |= sc200ai_write_reg(dev, 0x33ac,0x08);
    ret |= sc200ai_write_reg(dev, 0x33ae,0x10);
    ret |= sc200ai_write_reg(dev, 0x33af,0x19);
    ret |= sc200ai_write_reg(dev, 0x3621,0xe8);
    ret |= sc200ai_write_reg(dev, 0x3622,0x16);
    ret |= sc200ai_write_reg(dev, 0x3630,0xa0);
    ret |= sc200ai_write_reg(dev, 0x3637,0x36);
    ret |= sc200ai_write_reg(dev, 0x363a,0x1f);
    ret |= sc200ai_write_reg(dev, 0x363b,0xc6);
    ret |= sc200ai_write_reg(dev, 0x363c,0x0e);
    ret |= sc200ai_write_reg(dev, 0x3670,0x0a);
    ret |= sc200ai_write_reg(dev, 0x3674,0x82);
    ret |= sc200ai_write_reg(dev, 0x3675,0x76);
    ret |= sc200ai_write_reg(dev, 0x3676,0x78);
    ret |= sc200ai_write_reg(dev, 0x367c,0x48);
    ret |= sc200ai_write_reg(dev, 0x367d,0x58);
    ret |= sc200ai_write_reg(dev, 0x3690,0x34);
    ret |= sc200ai_write_reg(dev, 0x3691,0x33);
    ret |= sc200ai_write_reg(dev, 0x3692,0x44);
    ret |= sc200ai_write_reg(dev, 0x369c,0x40);
    ret |= sc200ai_write_reg(dev, 0x369d,0x48);
    ret |= sc200ai_write_reg(dev, 0x36eb,0x0c);
    ret |= sc200ai_write_reg(dev, 0x36ec,0x0c);
    ret |= sc200ai_write_reg(dev, 0x36fd,0x14);
    ret |= sc200ai_write_reg(dev, 0x3901,0x02);
    ret |= sc200ai_write_reg(dev, 0x3904,0x04);
    ret |= sc200ai_write_reg(dev, 0x3908,0x41);
    ret |= sc200ai_write_reg(dev, 0x391f,0x10);
    ret |= sc200ai_write_reg(dev, 0x3e00,0x01); //LEF {3e00[3:0],3e01[7:0],3e02[7:4]}
    ret |= sc200ai_write_reg(dev, 0x3e01,0x06);
    ret |= sc200ai_write_reg(dev, 0x3e02,0x00);
    ret |= sc200ai_write_reg(dev, 0x3e04,0x10); //SEF {3e22[3:0],3e04[7:0],3e05[7:4]}
    ret |= sc200ai_write_reg(dev, 0x3e05,0x60);
    ret |= sc200ai_write_reg(dev, 0x3e06,0x00);
    ret |= sc200ai_write_reg(dev, 0x3e07,0x80);
    ret |= sc200ai_write_reg(dev, 0x3e08,0x03);
    ret |= sc200ai_write_reg(dev, 0x3e09,0x40);
    ret |= sc200ai_write_reg(dev, 0x3e10,0x00);
    ret |= sc200ai_write_reg(dev, 0x3e11,0x80);
    ret |= sc200ai_write_reg(dev, 0x3e12,0x03);
    ret |= sc200ai_write_reg(dev, 0x3e13,0x40);
    ret |= sc200ai_write_reg(dev, 0x3e16,0x00);
    ret |= sc200ai_write_reg(dev, 0x3e17,0x80);
    ret |= sc200ai_write_reg(dev, 0x3e23,0x00);
    ret |= sc200ai_write_reg(dev, 0x3e24,0x88);//0x88 0x87 0xa3
    ret |= sc200ai_write_reg(dev, 0x3f09,0x48);
    ret |= sc200ai_write_reg(dev, 0x4816,0xb1);
    ret |= sc200ai_write_reg(dev, 0x4819,0x09);
    ret |= sc200ai_write_reg(dev, 0x481b,0x05);
    ret |= sc200ai_write_reg(dev, 0x481d,0x14);
    ret |= sc200ai_write_reg(dev, 0x481f,0x04);
    ret |= sc200ai_write_reg(dev, 0x4821,0x0a);
    ret |= sc200ai_write_reg(dev, 0x4823,0x05);
    ret |= sc200ai_write_reg(dev, 0x4825,0x04);
    ret |= sc200ai_write_reg(dev, 0x4827,0x05);
    ret |= sc200ai_write_reg(dev, 0x4829,0x08);
    ret |= sc200ai_write_reg(dev, 0x5787,0x10);
    ret |= sc200ai_write_reg(dev, 0x5788,0x06);
    ret |= sc200ai_write_reg(dev, 0x578a,0x10);
    ret |= sc200ai_write_reg(dev, 0x578b,0x06);
    ret |= sc200ai_write_reg(dev, 0x5790,0x10);
    ret |= sc200ai_write_reg(dev, 0x5791,0x10);
    ret |= sc200ai_write_reg(dev, 0x5792,0x00);
    ret |= sc200ai_write_reg(dev, 0x5793,0x10);
    ret |= sc200ai_write_reg(dev, 0x5794,0x10);
    ret |= sc200ai_write_reg(dev, 0x5795,0x00);
    ret |= sc200ai_write_reg(dev, 0x5799,0x00);
    ret |= sc200ai_write_reg(dev, 0x57c7,0x10);
    ret |= sc200ai_write_reg(dev, 0x57c8,0x06);
    ret |= sc200ai_write_reg(dev, 0x57ca,0x10);
    ret |= sc200ai_write_reg(dev, 0x57cb,0x06);
    ret |= sc200ai_write_reg(dev, 0x57d1,0x10);
    ret |= sc200ai_write_reg(dev, 0x57d4,0x10);
    ret |= sc200ai_write_reg(dev, 0x57d9,0x00);
    ret |= sc200ai_write_reg(dev, 0x59e0,0x60);
    ret |= sc200ai_write_reg(dev, 0x59e1,0x08);
    ret |= sc200ai_write_reg(dev, 0x59e2,0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e3,0x18);
    ret |= sc200ai_write_reg(dev, 0x59e4,0x18);
    ret |= sc200ai_write_reg(dev, 0x59e5,0x3f);
    ret |= sc200ai_write_reg(dev, 0x59e6,0x06);
    ret |= sc200ai_write_reg(dev, 0x59e7,0x02);
    ret |= sc200ai_write_reg(dev, 0x59e8,0x38);
    ret |= sc200ai_write_reg(dev, 0x59e9,0x10);
    ret |= sc200ai_write_reg(dev, 0x59ea,0x0c);
    ret |= sc200ai_write_reg(dev, 0x59eb,0x10);
    ret |= sc200ai_write_reg(dev, 0x59ec,0x04);
    ret |= sc200ai_write_reg(dev, 0x59ed,0x02);
    ret |= sc200ai_write_reg(dev, 0x59ee,0xa0);
    ret |= sc200ai_write_reg(dev, 0x59ef,0x08);
    ret |= sc200ai_write_reg(dev, 0x59f4,0x18);
    ret |= sc200ai_write_reg(dev, 0x59f5,0x10);
    ret |= sc200ai_write_reg(dev, 0x59f6,0x0c);
    ret |= sc200ai_write_reg(dev, 0x59f7,0x10);
    ret |= sc200ai_write_reg(dev, 0x59f8,0x06);
    ret |= sc200ai_write_reg(dev, 0x59f9,0x02);
    ret |= sc200ai_write_reg(dev, 0x59fa,0x18);
    ret |= sc200ai_write_reg(dev, 0x59fb,0x10);
    ret |= sc200ai_write_reg(dev, 0x59fc,0x0c);
    ret |= sc200ai_write_reg(dev, 0x59fd,0x10);
    ret |= sc200ai_write_reg(dev, 0x59fe,0x04);
    ret |= sc200ai_write_reg(dev, 0x59ff,0x02);
    ret |= sc200ai_write_reg(dev, 0x36e9,0x20);
    ret |= sc200ai_write_reg(dev, 0x36f9,0x24);

    sc200ai_delay_ms(20);
    ret |= sc200ai_write_reg(dev, 0x0100,0x01);

    printf("==========================================================================\n");
    printf("===SC200ai_MIPI_27MInput_742.5Mbps_2lane_10bit_1920x1080_HDR 30fps(VC)====\n");
    printf("==========================================================================\n");

    return ret;
}
#endif

xmedia_s32 sc200ai_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC200AI_1M_20FPS_10BIT_LINEAR_MODE:
            ret = sc200ai_2lane_linear_1280x720_init(dev);
            break;

        case SC200AI_1M_6FPS_10BIT_WDR_MODE:
            ret = sc200ai_2lane_wdr_1280x720_init(dev);
            break;

        case SC200AI_2M_12FPS_10BIT_LINEAR_MODE:
            ret = sc200ai_2lane_linear_1920x1080_init(dev);
            break;

        case SC200AI_2M_6FPS_10BIT_WDR_MODE:
            ret = sc200ai_2lane_wdr_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc200ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc200ai_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc200ai_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc200ai_i2c_fd[dev]);
        g_sc200ai_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc200ai_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc200ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc200ai_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc200ai_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc200ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc200ai_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc200ai_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc200ai_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc200ai_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc200ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC200AI_DATA_BYTE];

    if (g_sc200ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc200ai_i2c_fd[dev], g_sc200ai_i2c_addr[dev], addr, SC200AI_ADDR_BYTE, buf, SC200AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC200AI_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc200ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc200ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc200ai_i2c_fd[dev], g_sc200ai_i2c_addr[dev], buf, SC200AI_ADDR_BYTE + SC200AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 sc200ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc200ai_read_reg(dev, sc200ai_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc200ai_write_reg(dev, sc200ai_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc200ai_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
