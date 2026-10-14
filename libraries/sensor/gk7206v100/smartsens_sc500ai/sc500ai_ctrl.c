#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc500ai.h"
#include "sc500ai_ctrl.h"

#define SENSOR_READ_REG_BY_IOCTL
#ifdef SENSOR_READ_REG_BY_IOCTL
#include <linux/fb.h>
static xmedia_s32 g_sc500ai_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
#endif

#define SC500AI_REG_ADDR_MIRROR_FLIP 0x3221
#define SC500AI_RECV_BUF_NUM 4

static xmedia_s32 g_sc500ai_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

/*
 * 函数功能: sc500ai MIPI 4lane 1620p线性模式初始化序列 - 2880x1620x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc500ai_4lane_linear_2880x1620_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc500ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc500ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc500ai_write_reg(dev, 0x59e0, 0x60);
    ret |= sc500ai_write_reg(dev, 0x59e1, 0x08);
    ret |= sc500ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc500ai_write_reg(dev, 0x59e3, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59e4, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc500ai_write_reg(dev, 0x59e7, 0x02);
    ret |= sc500ai_write_reg(dev, 0x59e8, 0x38);
    ret |= sc500ai_write_reg(dev, 0x59e9, 0x20);
    ret |= sc500ai_write_reg(dev, 0x59ea, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x59ec, 0x08);
    ret |= sc500ai_write_reg(dev, 0x59ed, 0x02);
    ret |= sc500ai_write_reg(dev, 0x59ee, 0xa0);
    ret |= sc500ai_write_reg(dev, 0x59ef, 0x08);
    ret |= sc500ai_write_reg(dev, 0x59f4, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59f5, 0x10);
    ret |= sc500ai_write_reg(dev, 0x59f6, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x59f9, 0x02);
    ret |= sc500ai_write_reg(dev, 0x59fa, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59fb, 0x10);
    ret |= sc500ai_write_reg(dev, 0x59fc, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x59ff, 0x02);
    ret |= sc500ai_write_reg(dev, 0x3e16, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3e17, 0x80);
    ret |= sc500ai_write_reg(dev, 0x5799, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3e01, 0xcd);
    ret |= sc500ai_write_reg(dev, 0x3e02, 0xc0);
    ret |= sc500ai_write_reg(dev, 0x3308, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3356, 0x23);
    ret |= sc500ai_write_reg(dev, 0x4500, 0x88);
    ret |= sc500ai_write_reg(dev, 0x3651, 0x7d);
    ret |= sc500ai_write_reg(dev, 0x3622, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x320e, 0x06);    // vts 0xce4 0x157c
    ret |= sc500ai_write_reg(dev, 0x320f, 0x72);
    ret |= sc500ai_write_reg(dev, 0x36ec, 0x1a);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x1c);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x24);
    ret |= sc500ai_write_reg(dev, 0x330a, 0x00);
    ret |= sc500ai_write_reg(dev, 0x330b, 0xd0);
    ret |= sc500ai_write_reg(dev, 0x3253, 0x08);
    ret |= sc500ai_write_reg(dev, 0x363a, 0x1f);
    ret |= sc500ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc500ai_write_reg(dev, 0x391d, 0x04);
    ret |= sc500ai_write_reg(dev, 0x330d, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3302, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3301, 0x28);
    ret |= sc500ai_write_reg(dev, 0x3304, 0x70);
    ret |= sc500ai_write_reg(dev, 0x3303, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3308, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3309, 0x80);
    ret |= sc500ai_write_reg(dev, 0x3310, 0x02);
    ret |= sc500ai_write_reg(dev, 0x334c, 0x08);
    ret |= sc500ai_write_reg(dev, 0x330f, 0x02);
    ret |= sc500ai_write_reg(dev, 0x330e, 0x20);
    ret |= sc500ai_write_reg(dev, 0x331c, 0x04);
    ret |= sc500ai_write_reg(dev, 0x3320, 0x09);
    ret |= sc500ai_write_reg(dev, 0x33ac, 0x10);
    ret |= sc500ai_write_reg(dev, 0x33ae, 0x10);
    ret |= sc500ai_write_reg(dev, 0x331e, 0x61);
    ret |= sc500ai_write_reg(dev, 0x3356, 0x09);
    ret |= sc500ai_write_reg(dev, 0x33af, 0x19);
    ret |= sc500ai_write_reg(dev, 0x331f, 0x71);
    ret |= sc500ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc500ai_write_reg(dev, 0x336d, 0x40);
    ret |= sc500ai_write_reg(dev, 0x3306, 0x40);
    ret |= sc500ai_write_reg(dev, 0x3622, 0x03);
    ret |= sc500ai_write_reg(dev, 0x3364, 0x17);
    ret |= sc500ai_write_reg(dev, 0x3390, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3391, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3392, 0x38);
    ret |= sc500ai_write_reg(dev, 0x3301, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3393, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3394, 0x28);
    ret |= sc500ai_write_reg(dev, 0x3395, 0x28);
    ret |= sc500ai_write_reg(dev, 0x3396, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3397, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3398, 0x38);
    ret |= sc500ai_write_reg(dev, 0x3399, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x339a, 0x28);
    ret |= sc500ai_write_reg(dev, 0x339b, 0x28);
    ret |= sc500ai_write_reg(dev, 0x339c, 0x28);
    ret |= sc500ai_write_reg(dev, 0x3670, 0x02);
    ret |= sc500ai_write_reg(dev, 0x367c, 0x58);
    ret |= sc500ai_write_reg(dev, 0x367d, 0x78);
    ret |= sc500ai_write_reg(dev, 0x3674, 0x82);
    ret |= sc500ai_write_reg(dev, 0x3675, 0x64);
    ret |= sc500ai_write_reg(dev, 0x3676, 0x66);
    ret |= sc500ai_write_reg(dev, 0x3670, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x369c, 0x40);
    ret |= sc500ai_write_reg(dev, 0x369d, 0x48);
    ret |= sc500ai_write_reg(dev, 0x3690, 0x33);
    ret |= sc500ai_write_reg(dev, 0x3691, 0x33);
    ret |= sc500ai_write_reg(dev, 0x3692, 0x44);
    ret |= sc500ai_write_reg(dev, 0x360f, 0x01);
    ret |= sc500ai_write_reg(dev, 0x367a, 0x40);
    ret |= sc500ai_write_reg(dev, 0x367b, 0x48);
    ret |= sc500ai_write_reg(dev, 0x3671, 0x07);
    ret |= sc500ai_write_reg(dev, 0x3672, 0x07);
    ret |= sc500ai_write_reg(dev, 0x3673, 0x17);
    ret |= sc500ai_write_reg(dev, 0x3253, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x1c);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x24);
    ret |= sc500ai_write_reg(dev, 0x3221, 0x66);  // mirror & filp
    ret |= sc500ai_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc500ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC500AI_27MInput_MIPI_4lane_10bit_2880x1620_30fps init success!========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc500ai_4lane_wdr_2880x1620_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc500ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc500ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc500ai_write_reg(dev, 0x301f, 0x23);
    ret |= sc500ai_write_reg(dev, 0x3106, 0x01);
    ret |= sc500ai_write_reg(dev, 0x320e, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x320f, 0xe4);
    ret |= sc500ai_write_reg(dev, 0x3220, 0x53);
    ret |= sc500ai_write_reg(dev, 0x3250, 0xff); //old-3f  new-ff
    ret |= sc500ai_write_reg(dev, 0x3253, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3301, 0x0b);
    ret |= sc500ai_write_reg(dev, 0x3302, 0x20);
    ret |= sc500ai_write_reg(dev, 0x3303, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3304, 0x70);
    ret |= sc500ai_write_reg(dev, 0x3306, 0x50);
    ret |= sc500ai_write_reg(dev, 0x3308, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3309, 0x80);
    ret |= sc500ai_write_reg(dev, 0x330a, 0x00);
    ret |= sc500ai_write_reg(dev, 0x330b, 0xe8);
    ret |= sc500ai_write_reg(dev, 0x330d, 0x30);
    ret |= sc500ai_write_reg(dev, 0x330e, 0x30);
    ret |= sc500ai_write_reg(dev, 0x330f, 0x02);
    ret |= sc500ai_write_reg(dev, 0x3310, 0x02);
    ret |= sc500ai_write_reg(dev, 0x331c, 0x08);
    ret |= sc500ai_write_reg(dev, 0x331e, 0x61);
    ret |= sc500ai_write_reg(dev, 0x331f, 0x71);
    ret |= sc500ai_write_reg(dev, 0x3320, 0x11);
    ret |= sc500ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc500ai_write_reg(dev, 0x334c, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3356, 0x11);
    ret |= sc500ai_write_reg(dev, 0x3364, 0x17);
    ret |= sc500ai_write_reg(dev, 0x336d, 0x03);
    ret |= sc500ai_write_reg(dev, 0x3390, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3391, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3392, 0x38);
    ret |= sc500ai_write_reg(dev, 0x3393, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3394, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3395, 0x12);
    ret |= sc500ai_write_reg(dev, 0x3396, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3397, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3398, 0x38);
    ret |= sc500ai_write_reg(dev, 0x3399, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x339a, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x339b, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x339c, 0x12);
    ret |= sc500ai_write_reg(dev, 0x33ac, 0x10);
    ret |= sc500ai_write_reg(dev, 0x33ae, 0x20);
    ret |= sc500ai_write_reg(dev, 0x33af, 0x21);
    ret |= sc500ai_write_reg(dev, 0x360f, 0x01);
    ret |= sc500ai_write_reg(dev, 0x3621, 0xe8);
    ret |= sc500ai_write_reg(dev, 0x3622, 0x06);
    ret |= sc500ai_write_reg(dev, 0x3630, 0x82);
    ret |= sc500ai_write_reg(dev, 0x3633, 0x33);
    ret |= sc500ai_write_reg(dev, 0x3634, 0x64);
    ret |= sc500ai_write_reg(dev, 0x3637, 0x50);
    ret |= sc500ai_write_reg(dev, 0x363a, 0x1f);
    ret |= sc500ai_write_reg(dev, 0x363c, 0x40);
    ret |= sc500ai_write_reg(dev, 0x3651, 0x7d);
    ret |= sc500ai_write_reg(dev, 0x3670, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3671, 0x06);
    ret |= sc500ai_write_reg(dev, 0x3672, 0x16);
    ret |= sc500ai_write_reg(dev, 0x3673, 0x17);
    ret |= sc500ai_write_reg(dev, 0x3674, 0x82);
    ret |= sc500ai_write_reg(dev, 0x3675, 0x62);
    ret |= sc500ai_write_reg(dev, 0x3676, 0x44);
    ret |= sc500ai_write_reg(dev, 0x367a, 0x48);
    ret |= sc500ai_write_reg(dev, 0x367b, 0x78);
    ret |= sc500ai_write_reg(dev, 0x367c, 0x48);
    ret |= sc500ai_write_reg(dev, 0x367d, 0x58);
    ret |= sc500ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc500ai_write_reg(dev, 0x3691, 0x34);
    ret |= sc500ai_write_reg(dev, 0x3692, 0x54);
    ret |= sc500ai_write_reg(dev, 0x369c, 0x48);
    ret |= sc500ai_write_reg(dev, 0x369d, 0x78);
    ret |= sc500ai_write_reg(dev, 0x36ea, 0x2d);
    ret |= sc500ai_write_reg(dev, 0x36eb, 0x04);
    ret |= sc500ai_write_reg(dev, 0x36ec, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x36ed, 0x14);
    ret |= sc500ai_write_reg(dev, 0x36fa, 0x35);
    ret |= sc500ai_write_reg(dev, 0x36fb, 0x04);
    ret |= sc500ai_write_reg(dev, 0x36fc, 0x00);
    ret |= sc500ai_write_reg(dev, 0x36fd, 0x16);
    ret |= sc500ai_write_reg(dev, 0x3904, 0x04);
    ret |= sc500ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc500ai_write_reg(dev, 0x391f, 0x10);
    ret |= sc500ai_write_reg(dev, 0x39c2, 0x30);
    ret |= sc500ai_write_reg(dev, 0x3e00, 0x01);
    ret |= sc500ai_write_reg(dev, 0x3e01, 0x82);
    ret |= sc500ai_write_reg(dev, 0x3e02, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3e04, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3e05, 0x20);
    ret |= sc500ai_write_reg(dev, 0x3e23, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3e24, 0xc6);
    ret |= sc500ai_write_reg(dev, 0x4500, 0x88);
    ret |= sc500ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc500ai_write_reg(dev, 0x4800, 0x24);
    ret |= sc500ai_write_reg(dev, 0x4837, 0x14);
    ret |= sc500ai_write_reg(dev, 0x4853, 0xfd);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x30);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x44);
    ret |= sc500ai_write_reg(dev, 0x0100, 0x01);
    ret |= sc500ai_write_reg(dev, 0x3221, 0x66);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc500ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================================\n");
        SENSOR_PRINT("====  sc500ai_raw10_2880X1620_2to1wdr_VC_30fps_4lane_820.8Mbps_27MInput init success!  =====\n");
        SENSOR_PRINT("============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc500ai_2lane_linear_640x480_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc500ai_write_reg(dev, 0x0103, 0x01);
    ret |= sc500ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x80);
    ret |= sc500ai_write_reg(dev, 0x3018, 0x32);
    ret |= sc500ai_write_reg(dev, 0x301f, 0xfd);
    ret |= sc500ai_write_reg(dev, 0x3031, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3106, 0x01);
    ret |= sc500ai_write_reg(dev, 0x3200, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3201, 0x60);
    ret |= sc500ai_write_reg(dev, 0x3202, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3203, 0x32);
    ret |= sc500ai_write_reg(dev, 0x3204, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3205, 0xe7);
    ret |= sc500ai_write_reg(dev, 0x3206, 0x06);
    ret |= sc500ai_write_reg(dev, 0x3207, 0x29);
    ret |= sc500ai_write_reg(dev, 0x3208, 0x02);
    ret |= sc500ai_write_reg(dev, 0x3209, 0x80);
    ret |= sc500ai_write_reg(dev, 0x320a, 0x01);
    ret |= sc500ai_write_reg(dev, 0x320b, 0xe0);

//    ret |= sc500ai_write_reg(dev, 0x320e, 0x0c);
//    ret |= sc500ai_write_reg(dev, 0x320f, 0xe4);
    
    ret |= sc500ai_write_reg(dev, 0x3210, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3211, 0x04);
    ret |= sc500ai_write_reg(dev, 0x3212, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3213, 0x04);
    ret |= sc500ai_write_reg(dev, 0x3222, 0x00); //master/slave
    ret |= sc500ai_write_reg(dev, 0x3224, 0x82);
    ret |= sc500ai_write_reg(dev, 0x3225, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3230, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3231, 0x04);
    ret |= sc500ai_write_reg(dev, 0x3253, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3301, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3302, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3303, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3304, 0x60);
    ret |= sc500ai_write_reg(dev, 0x3306, 0x60);
    ret |= sc500ai_write_reg(dev, 0x3308, 0x10);
    ret |= sc500ai_write_reg(dev, 0x3309, 0x70);
    ret |= sc500ai_write_reg(dev, 0x330a, 0x00);
    ret |= sc500ai_write_reg(dev, 0x330b, 0xf0);
    ret |= sc500ai_write_reg(dev, 0x330d, 0x18);
    ret |= sc500ai_write_reg(dev, 0x330e, 0x20);
    ret |= sc500ai_write_reg(dev, 0x330f, 0x02);
    ret |= sc500ai_write_reg(dev, 0x3310, 0x02);
    ret |= sc500ai_write_reg(dev, 0x331c, 0x04);
    ret |= sc500ai_write_reg(dev, 0x331e, 0x51);
    ret |= sc500ai_write_reg(dev, 0x331f, 0x61);
    ret |= sc500ai_write_reg(dev, 0x3320, 0x09);
    ret |= sc500ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc500ai_write_reg(dev, 0x334c, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3356, 0x09);
    ret |= sc500ai_write_reg(dev, 0x3364, 0x17);
    ret |= sc500ai_write_reg(dev, 0x336d, 0x03);
    ret |= sc500ai_write_reg(dev, 0x3390, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3391, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3392, 0x38);
    ret |= sc500ai_write_reg(dev, 0x3393, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3394, 0x20);
    ret |= sc500ai_write_reg(dev, 0x3395, 0x20);
    ret |= sc500ai_write_reg(dev, 0x3396, 0x08);
    ret |= sc500ai_write_reg(dev, 0x3397, 0x18);
    ret |= sc500ai_write_reg(dev, 0x3398, 0x38);
    ret |= sc500ai_write_reg(dev, 0x3399, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x339a, 0x20);
    ret |= sc500ai_write_reg(dev, 0x339b, 0x20);
    ret |= sc500ai_write_reg(dev, 0x339c, 0x20);
    ret |= sc500ai_write_reg(dev, 0x33ac, 0x10);
    ret |= sc500ai_write_reg(dev, 0x33ae, 0x10);
    ret |= sc500ai_write_reg(dev, 0x33af, 0x19);
    ret |= sc500ai_write_reg(dev, 0x360f, 0x01);
    ret |= sc500ai_write_reg(dev, 0x3622, 0x03);
    ret |= sc500ai_write_reg(dev, 0x363a, 0x1f);
    ret |= sc500ai_write_reg(dev, 0x363c, 0x40);
    ret |= sc500ai_write_reg(dev, 0x3651, 0x7d);
    ret |= sc500ai_write_reg(dev, 0x3670, 0x0a);
    ret |= sc500ai_write_reg(dev, 0x3671, 0x07);
    ret |= sc500ai_write_reg(dev, 0x3672, 0x17);
    ret |= sc500ai_write_reg(dev, 0x3673, 0x1e);
    ret |= sc500ai_write_reg(dev, 0x3674, 0x82);
    ret |= sc500ai_write_reg(dev, 0x3675, 0x64);
    ret |= sc500ai_write_reg(dev, 0x3676, 0x66);
    ret |= sc500ai_write_reg(dev, 0x367a, 0x48);
    ret |= sc500ai_write_reg(dev, 0x367b, 0x78);
    ret |= sc500ai_write_reg(dev, 0x367c, 0x58);
    ret |= sc500ai_write_reg(dev, 0x367d, 0x78);
    ret |= sc500ai_write_reg(dev, 0x3690, 0x34);
    ret |= sc500ai_write_reg(dev, 0x3691, 0x34);
    ret |= sc500ai_write_reg(dev, 0x3692, 0x54);
    ret |= sc500ai_write_reg(dev, 0x369c, 0x48);
    ret |= sc500ai_write_reg(dev, 0x369d, 0x78);
    ret |= sc500ai_write_reg(dev, 0x36ea, 0x36);
    ret |= sc500ai_write_reg(dev, 0x36eb, 0x1c);
    ret |= sc500ai_write_reg(dev, 0x36ec, 0x2a);
    ret |= sc500ai_write_reg(dev, 0x36ed, 0x14);
    ret |= sc500ai_write_reg(dev, 0x36fa, 0x36);
    ret |= sc500ai_write_reg(dev, 0x36fb, 0x1e);
    ret |= sc500ai_write_reg(dev, 0x36fc, 0x11);
    ret |= sc500ai_write_reg(dev, 0x36fd, 0x24);
    ret |= sc500ai_write_reg(dev, 0x3904, 0x04);
    ret |= sc500ai_write_reg(dev, 0x3908, 0x41);
    ret |= sc500ai_write_reg(dev, 0x391d, 0x04);
    ret |= sc500ai_write_reg(dev, 0x39c2, 0x30);
    ret |= sc500ai_write_reg(dev, 0x3e00, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3e01, 0xcd);
    ret |= sc500ai_write_reg(dev, 0x3e02, 0xc0);
    ret |= sc500ai_write_reg(dev, 0x3e16, 0x00);
    ret |= sc500ai_write_reg(dev, 0x3e17, 0x80);
    ret |= sc500ai_write_reg(dev, 0x4500, 0x88);
    ret |= sc500ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc500ai_write_reg(dev, 0x5799, 0x00);
    ret |= sc500ai_write_reg(dev, 0x59e0, 0x60);
    ret |= sc500ai_write_reg(dev, 0x59e1, 0x08);
    ret |= sc500ai_write_reg(dev, 0x59e2, 0x3f);
    ret |= sc500ai_write_reg(dev, 0x59e3, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59e4, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59e5, 0x3f);
    ret |= sc500ai_write_reg(dev, 0x59e7, 0x02);
    ret |= sc500ai_write_reg(dev, 0x59e8, 0x38);
    ret |= sc500ai_write_reg(dev, 0x59e9, 0x20);
    ret |= sc500ai_write_reg(dev, 0x59ea, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x59ec, 0x08);
    ret |= sc500ai_write_reg(dev, 0x59ed, 0x02);
    ret |= sc500ai_write_reg(dev, 0x59ee, 0xa0);
    ret |= sc500ai_write_reg(dev, 0x59ef, 0x08);
    ret |= sc500ai_write_reg(dev, 0x59f4, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59f5, 0x10);
    ret |= sc500ai_write_reg(dev, 0x59f6, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x59f9, 0x02);
    ret |= sc500ai_write_reg(dev, 0x59fa, 0x18);
    ret |= sc500ai_write_reg(dev, 0x59fb, 0x10);
    ret |= sc500ai_write_reg(dev, 0x59fc, 0x0c);
    ret |= sc500ai_write_reg(dev, 0x59ff, 0x02);
    ret |= sc500ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc500ai_write_reg(dev, 0x36f9, 0x54);
    ret |= sc500ai_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc500ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================================\n");
        SENSOR_PRINT("====  sc500ai_raw8_640X480_linear_2lane_80Mbps_24MInput  init success!  ====================\n");
        SENSOR_PRINT("====  sc500ai 640X480 slave setting init success!  =========================================\n");
        SENSOR_PRINT("============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_void sc500ai_slave_init(xmedia_u32 dev)
{
    sc500ai_write_reg(dev, 0x0100, 0x00);
    sc500ai_write_reg(dev, 0x3222, 0x02);
    sc500ai_write_reg(dev, 0x3225, 0x10);
    sc500ai_write_reg(dev, 0x3224, 0x82);
    sc500ai_write_reg(dev, 0x3230, 0x00);
    sc500ai_write_reg(dev, 0x3231, 0x04);
    sc500ai_write_reg(dev, 0x300a, 0x20);
    sc500ai_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("====== SC500AI slave setting init success!=======\n");
}

xmedia_s32 sc500ai_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC500AI_5M_10BIT_LINEAR_MODE:
            ret = sc500ai_4lane_linear_2880x1620_init(dev);
            break;

        case SC500AI_5M_10BIT_WDR_MODE:
            ret = sc500ai_4lane_wdr_2880x1620_init(dev);
            break;
        case SC500AI_640_480_10BIT_LINEAR_MODE:
            ret = sc500ai_2lane_linear_640x480_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE && image_mode == SC500AI_5M_10BIT_LINEAR_MODE) {
        sc500ai_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc500ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc500ai_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc500ai_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc500ai_i2c_fd[dev]);
        g_sc500ai_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc500ai_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc500ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc500ai_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc500ai_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc500ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc500ai_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc500ai_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc500ai_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc500ai_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc500ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC500AI_DATA_BYTE];

    if (g_sc500ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc500ai_i2c_fd[dev], g_sc500ai_i2c_addr[dev], addr, SC500AI_ADDR_BYTE, buf, SC500AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC500AI_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc500ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc500ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc500ai_i2c_fd[dev], g_sc500ai_i2c_addr[dev], buf, SC500AI_ADDR_BYTE + SC500AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 sc500ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc500ai_read_reg(dev, SC500AI_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc500ai_write_reg(dev, SC500AI_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc500ai_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
