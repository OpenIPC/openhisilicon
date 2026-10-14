#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc530ai.h"
#include "sc530ai_ctrl.h"

#define SC530AI_REG_ADDR_MIRROR_FLIP 0x3221
#define SC530AI_RECV_BUF_NUM 4

static xmedia_s32 g_sc530ai_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
static xmedia_s32 g_sc530ai_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

static void sc530ai_delay_ms(int ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

/*
 * 函数功能: sc530ai MIPI 4lane 1620p线性模式初始化序列 - 2880x1620x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc530ai_4lane_linear_2880x1620_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc530ai_write_reg(dev, 0x0103, 0x01);
    sc530ai_delay_ms(100);
    ret |= sc530ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc530ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc530ai_write_reg(dev, 0x37f9, 0x80);
    ret |= sc530ai_write_reg(dev, 0x301f, 0x41);
    ret |= sc530ai_write_reg(dev, 0x320c, 0x06);
    ret |= sc530ai_write_reg(dev, 0x320d, 0x27);
    ret |= sc530ai_write_reg(dev, 0x320e, 0x07);
    ret |= sc530ai_write_reg(dev, 0x320f, 0xbc);
    ret |= sc530ai_write_reg(dev, 0x3250, 0x40);
    ret |= sc530ai_write_reg(dev, 0x3251, 0x98);
    ret |= sc530ai_write_reg(dev, 0x3253, 0x0c);
    ret |= sc530ai_write_reg(dev, 0x325f, 0x20);
    ret |= sc530ai_write_reg(dev, 0x3301, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3304, 0x50);
    ret |= sc530ai_write_reg(dev, 0x3306, 0x88);
    ret |= sc530ai_write_reg(dev, 0x3308, 0x14);
    ret |= sc530ai_write_reg(dev, 0x3309, 0x70);
    ret |= sc530ai_write_reg(dev, 0x330a, 0x00);
    ret |= sc530ai_write_reg(dev, 0x330b, 0xf8);
    ret |= sc530ai_write_reg(dev, 0x330d, 0x10);
    ret |= sc530ai_write_reg(dev, 0x331e, 0x41);
    ret |= sc530ai_write_reg(dev, 0x331f, 0x61);
    ret |= sc530ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc530ai_write_reg(dev, 0x335d, 0x60);
    ret |= sc530ai_write_reg(dev, 0x335e, 0x06);
    ret |= sc530ai_write_reg(dev, 0x335f, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3364, 0x56);
    ret |= sc530ai_write_reg(dev, 0x3366, 0x01);
    ret |= sc530ai_write_reg(dev, 0x337c, 0x02);
    ret |= sc530ai_write_reg(dev, 0x337d, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x3390, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3391, 0x03);
    ret |= sc530ai_write_reg(dev, 0x3392, 0x07);
    ret |= sc530ai_write_reg(dev, 0x3393, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3394, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3395, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3396, 0x40);
    ret |= sc530ai_write_reg(dev, 0x3397, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3398, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x3399, 0x08);
    ret |= sc530ai_write_reg(dev, 0x339a, 0x08);
    ret |= sc530ai_write_reg(dev, 0x339b, 0x08);
    ret |= sc530ai_write_reg(dev, 0x339c, 0x1d);
    ret |= sc530ai_write_reg(dev, 0x33a2, 0x04);
    ret |= sc530ai_write_reg(dev, 0x33ae, 0x30);
    ret |= sc530ai_write_reg(dev, 0x33af, 0x50);
    ret |= sc530ai_write_reg(dev, 0x33b1, 0x80);
    ret |= sc530ai_write_reg(dev, 0x33b2, 0x48);
    ret |= sc530ai_write_reg(dev, 0x33b3, 0x30);
    ret |= sc530ai_write_reg(dev, 0x349f, 0x02);
    ret |= sc530ai_write_reg(dev, 0x34a6, 0x48);
    ret |= sc530ai_write_reg(dev, 0x34a7, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x34a8, 0x30);
    ret |= sc530ai_write_reg(dev, 0x34a9, 0x18);
    ret |= sc530ai_write_reg(dev, 0x34f8, 0x5f);
    ret |= sc530ai_write_reg(dev, 0x34f9, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3632, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3633, 0x32);
    ret |= sc530ai_write_reg(dev, 0x3637, 0x29);
    ret |= sc530ai_write_reg(dev, 0x3638, 0xc1);
    ret |= sc530ai_write_reg(dev, 0x363b, 0x20);
    ret |= sc530ai_write_reg(dev, 0x363d, 0x02);
    ret |= sc530ai_write_reg(dev, 0x3670, 0x09);
    ret |= sc530ai_write_reg(dev, 0x3674, 0x8b);
    ret |= sc530ai_write_reg(dev, 0x3675, 0xc6);
    ret |= sc530ai_write_reg(dev, 0x3676, 0x8b);
    ret |= sc530ai_write_reg(dev, 0x367c, 0x40);
    ret |= sc530ai_write_reg(dev, 0x367d, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3690, 0x32);
    ret |= sc530ai_write_reg(dev, 0x3691, 0x43);
    ret |= sc530ai_write_reg(dev, 0x3692, 0x33);
    ret |= sc530ai_write_reg(dev, 0x3693, 0x40);
    ret |= sc530ai_write_reg(dev, 0x3694, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x3698, 0x85);
    ret |= sc530ai_write_reg(dev, 0x3699, 0x8f);
    ret |= sc530ai_write_reg(dev, 0x369a, 0xa0);
    ret |= sc530ai_write_reg(dev, 0x369b, 0xc3);
    ret |= sc530ai_write_reg(dev, 0x36a2, 0x49);
    ret |= sc530ai_write_reg(dev, 0x36a3, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x36a4, 0x4f);
    ret |= sc530ai_write_reg(dev, 0x36d0, 0x01);
    ret |= sc530ai_write_reg(dev, 0x36ea, 0x0d);
    ret |= sc530ai_write_reg(dev, 0x36eb, 0x04);
    ret |= sc530ai_write_reg(dev, 0x36ec, 0x13);
    ret |= sc530ai_write_reg(dev, 0x36ed, 0x14);
    ret |= sc530ai_write_reg(dev, 0x370f, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3722, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3728, 0x10);
    ret |= sc530ai_write_reg(dev, 0x37b0, 0x03);
    ret |= sc530ai_write_reg(dev, 0x37b1, 0x03);
    ret |= sc530ai_write_reg(dev, 0x37b2, 0x83);
    ret |= sc530ai_write_reg(dev, 0x37b3, 0x48);
    ret |= sc530ai_write_reg(dev, 0x37b4, 0x49);
    ret |= sc530ai_write_reg(dev, 0x37fa, 0x0d);
    ret |= sc530ai_write_reg(dev, 0x37fb, 0x24);
    ret |= sc530ai_write_reg(dev, 0x37fc, 0x01);
    ret |= sc530ai_write_reg(dev, 0x37fd, 0x14);
    ret |= sc530ai_write_reg(dev, 0x3901, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3902, 0xc5);
    ret |= sc530ai_write_reg(dev, 0x3904, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3905, 0x8c);
    ret |= sc530ai_write_reg(dev, 0x3909, 0x00);
    ret |= sc530ai_write_reg(dev, 0x391d, 0x04);
    ret |= sc530ai_write_reg(dev, 0x391f, 0x44);
    ret |= sc530ai_write_reg(dev, 0x3926, 0x21);
    ret |= sc530ai_write_reg(dev, 0x3929, 0x18);
    ret |= sc530ai_write_reg(dev, 0x3933, 0x82);
    ret |= sc530ai_write_reg(dev, 0x3934, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x3937, 0x5f);
    ret |= sc530ai_write_reg(dev, 0x3939, 0x00);
    ret |= sc530ai_write_reg(dev, 0x393a, 0x00);
    ret |= sc530ai_write_reg(dev, 0x39dc, 0x02);
    ret |= sc530ai_write_reg(dev, 0x3e01, 0xf6);
    ret |= sc530ai_write_reg(dev, 0x3e02, 0xe0);
    ret |= sc530ai_write_reg(dev, 0x440e, 0x02);
    ret |= sc530ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc530ai_write_reg(dev, 0x4837, 0x22);
    ret |= sc530ai_write_reg(dev, 0x5010, 0x10);
    ret |= sc530ai_write_reg(dev, 0x5780, 0x66);
    ret |= sc530ai_write_reg(dev, 0x578d, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5799, 0x06);
    ret |= sc530ai_write_reg(dev, 0x57ad, 0x00);
    ret |= sc530ai_write_reg(dev, 0x5ae0, 0xfe);
    ret |= sc530ai_write_reg(dev, 0x5ae1, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5ae2, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5ae3, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5ae4, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5ae5, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5ae6, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5ae7, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5ae8, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5ae9, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aea, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5aeb, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5aec, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aed, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5aee, 0xfe);
    ret |= sc530ai_write_reg(dev, 0x5aef, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5af4, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5af5, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5af6, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5af7, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5af8, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5af9, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5afa, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5afb, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5afc, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5afd, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5afe, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aff, 0x28);
    ret |= sc530ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc530ai_write_reg(dev, 0x37f9, 0x44);
    ret |= sc530ai_write_reg(dev, 0x0100, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3221, 0x60);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc530ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=========================================================================================\n");
        SENSOR_PRINT("========= SC530AI_27Minput_396Mbps_MIPI_4lane_10bit_2880x1620_30fps init success!========\n");
        SENSOR_PRINT("=========================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: sc530ai MIPI 2lane 1620p线性模式初始化序列 - 2880x1620x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc530ai_2lane_linear_2880x1620_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc530ai_write_reg(dev, 0x0103, 0x01);
    sc530ai_delay_ms(100);
    ret |= sc530ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc530ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc530ai_write_reg(dev, 0x37f9, 0x80);
    ret |= sc530ai_write_reg(dev, 0x3018, 0x32);
    ret |= sc530ai_write_reg(dev, 0x3019, 0x0c);
    ret |= sc530ai_write_reg(dev, 0x301f, 0x18);
    ret |= sc530ai_write_reg(dev, 0x3250, 0x40);
    ret |= sc530ai_write_reg(dev, 0x3251, 0x98);
    ret |= sc530ai_write_reg(dev, 0x3253, 0x0c);
    ret |= sc530ai_write_reg(dev, 0x325f, 0x20);
    ret |= sc530ai_write_reg(dev, 0x3301, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3304, 0x50);
    ret |= sc530ai_write_reg(dev, 0x3306, 0x88);
    ret |= sc530ai_write_reg(dev, 0x3308, 0x14);
    ret |= sc530ai_write_reg(dev, 0x3309, 0x70);
    ret |= sc530ai_write_reg(dev, 0x330a, 0x00);
    ret |= sc530ai_write_reg(dev, 0x330b, 0xf8);
    ret |= sc530ai_write_reg(dev, 0x330d, 0x10);
    ret |= sc530ai_write_reg(dev, 0x331e, 0x41);
    ret |= sc530ai_write_reg(dev, 0x331f, 0x61);
    ret |= sc530ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc530ai_write_reg(dev, 0x335d, 0x60);
    ret |= sc530ai_write_reg(dev, 0x335e, 0x06);
    ret |= sc530ai_write_reg(dev, 0x335f, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3364, 0x56);
    ret |= sc530ai_write_reg(dev, 0x3366, 0x01);
    ret |= sc530ai_write_reg(dev, 0x337c, 0x02);
    ret |= sc530ai_write_reg(dev, 0x337d, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x3390, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3391, 0x03);
    ret |= sc530ai_write_reg(dev, 0x3392, 0x07);
    ret |= sc530ai_write_reg(dev, 0x3393, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3394, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3395, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3396, 0x40);
    ret |= sc530ai_write_reg(dev, 0x3397, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3398, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x3399, 0x08);
    ret |= sc530ai_write_reg(dev, 0x339a, 0x08);
    ret |= sc530ai_write_reg(dev, 0x339b, 0x08);
    ret |= sc530ai_write_reg(dev, 0x339c, 0x1d);
    ret |= sc530ai_write_reg(dev, 0x33a2, 0x04);
    ret |= sc530ai_write_reg(dev, 0x33ae, 0x30);
    ret |= sc530ai_write_reg(dev, 0x33af, 0x50);
    ret |= sc530ai_write_reg(dev, 0x33b1, 0x80);
    ret |= sc530ai_write_reg(dev, 0x33b2, 0x48);
    ret |= sc530ai_write_reg(dev, 0x33b3, 0x30);
    ret |= sc530ai_write_reg(dev, 0x349f, 0x02);
    ret |= sc530ai_write_reg(dev, 0x34a6, 0x48);
    ret |= sc530ai_write_reg(dev, 0x34a7, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x34a8, 0x30);
    ret |= sc530ai_write_reg(dev, 0x34a9, 0x18);
    ret |= sc530ai_write_reg(dev, 0x34f8, 0x5f);
    ret |= sc530ai_write_reg(dev, 0x34f9, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3632, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3633, 0x32);
    ret |= sc530ai_write_reg(dev, 0x3637, 0x29);
    ret |= sc530ai_write_reg(dev, 0x3638, 0xc1);
    ret |= sc530ai_write_reg(dev, 0x363b, 0x20);
    ret |= sc530ai_write_reg(dev, 0x363d, 0x02);
    ret |= sc530ai_write_reg(dev, 0x3670, 0x09);
    ret |= sc530ai_write_reg(dev, 0x3674, 0x8b);
    ret |= sc530ai_write_reg(dev, 0x3675, 0xc6);
    ret |= sc530ai_write_reg(dev, 0x3676, 0x8b);
    ret |= sc530ai_write_reg(dev, 0x367c, 0x40);
    ret |= sc530ai_write_reg(dev, 0x367d, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3690, 0x32);
    ret |= sc530ai_write_reg(dev, 0x3691, 0x43);
    ret |= sc530ai_write_reg(dev, 0x3692, 0x33);
    ret |= sc530ai_write_reg(dev, 0x3693, 0x40);
    ret |= sc530ai_write_reg(dev, 0x3694, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x3698, 0x85);
    ret |= sc530ai_write_reg(dev, 0x3699, 0x8f);
    ret |= sc530ai_write_reg(dev, 0x369a, 0xa0);
    ret |= sc530ai_write_reg(dev, 0x369b, 0xc3);
    ret |= sc530ai_write_reg(dev, 0x36a2, 0x49);
    ret |= sc530ai_write_reg(dev, 0x36a3, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x36a4, 0x4f);
    ret |= sc530ai_write_reg(dev, 0x36d0, 0x01);
    ret |= sc530ai_write_reg(dev, 0x36ec, 0x03);
    ret |= sc530ai_write_reg(dev, 0x370f, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3722, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3728, 0x10);
    ret |= sc530ai_write_reg(dev, 0x37b0, 0x03);
    ret |= sc530ai_write_reg(dev, 0x37b1, 0x03);
    ret |= sc530ai_write_reg(dev, 0x37b2, 0x83);
    ret |= sc530ai_write_reg(dev, 0x37b3, 0x48);
    ret |= sc530ai_write_reg(dev, 0x37b4, 0x49);
    ret |= sc530ai_write_reg(dev, 0x37fb, 0x24);
    ret |= sc530ai_write_reg(dev, 0x37fc, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3901, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3902, 0xc5);
    ret |= sc530ai_write_reg(dev, 0x3904, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3905, 0x8c);
    ret |= sc530ai_write_reg(dev, 0x3909, 0x00);
    ret |= sc530ai_write_reg(dev, 0x391d, 0x04);
    ret |= sc530ai_write_reg(dev, 0x391f, 0x44);
    ret |= sc530ai_write_reg(dev, 0x3926, 0x21);
    ret |= sc530ai_write_reg(dev, 0x3929, 0x18);
    ret |= sc530ai_write_reg(dev, 0x3933, 0x82);
    ret |= sc530ai_write_reg(dev, 0x3934, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x3937, 0x5f);
    ret |= sc530ai_write_reg(dev, 0x3939, 0x00);
    ret |= sc530ai_write_reg(dev, 0x393a, 0x00);
    ret |= sc530ai_write_reg(dev, 0x39dc, 0x02);
    ret |= sc530ai_write_reg(dev, 0x3e01, 0xcd);
    ret |= sc530ai_write_reg(dev, 0x3e02, 0xa0);
    ret |= sc530ai_write_reg(dev, 0x440e, 0x02);
    ret |= sc530ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc530ai_write_reg(dev, 0x4837, 0x14);
    ret |= sc530ai_write_reg(dev, 0x5010, 0x10);
    ret |= sc530ai_write_reg(dev, 0x5780, 0x66);
    ret |= sc530ai_write_reg(dev, 0x578d, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5799, 0x06);
    ret |= sc530ai_write_reg(dev, 0x57ad, 0x00);
    ret |= sc530ai_write_reg(dev, 0x5ae0, 0xfe);
    ret |= sc530ai_write_reg(dev, 0x5ae1, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5ae2, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5ae3, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5ae4, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5ae5, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5ae6, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5ae7, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5ae8, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5ae9, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aea, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5aeb, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5aec, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aed, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5aee, 0xfe);
    ret |= sc530ai_write_reg(dev, 0x5aef, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5af4, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5af5, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5af6, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5af7, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5af8, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5af9, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5afa, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5afb, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5afc, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5afd, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5afe, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aff, 0x28);
    ret |= sc530ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc530ai_write_reg(dev, 0x37f9, 0x44);
    ret |= sc530ai_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc530ai_2l_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=========================================================================================\n");
        SENSOR_PRINT("========= SC530AI_27Minput_792Mbps_MIPI_2lane_10bit_2880x1620_30fps init success!========\n");
        SENSOR_PRINT("=========================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc530ai_4lane_wdr_2880x1620_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc530ai_write_reg(dev, 0x0103, 0x01);
    sc530ai_delay_ms(100);
    ret |= sc530ai_write_reg(dev, 0x0100, 0x00);
    ret |= sc530ai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc530ai_write_reg(dev, 0x37f9, 0x80);
    ret |= sc530ai_write_reg(dev, 0x301f, 0x03);
    ret |= sc530ai_write_reg(dev, 0x320e, 0x0c);
    ret |= sc530ai_write_reg(dev, 0x320f, 0xe4);
    ret |= sc530ai_write_reg(dev, 0x3250, 0xff);
    ret |= sc530ai_write_reg(dev, 0x3251, 0x98);
    ret |= sc530ai_write_reg(dev, 0x3253, 0x0c);
    ret |= sc530ai_write_reg(dev, 0x325f, 0x20);
    ret |= sc530ai_write_reg(dev, 0x3281, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3301, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3304, 0x58);
    ret |= sc530ai_write_reg(dev, 0x3306, 0xa0);
    ret |= sc530ai_write_reg(dev, 0x3308, 0x14);
    ret |= sc530ai_write_reg(dev, 0x3309, 0x50);
    ret |= sc530ai_write_reg(dev, 0x330a, 0x01);
    ret |= sc530ai_write_reg(dev, 0x330b, 0x10);
    ret |= sc530ai_write_reg(dev, 0x330d, 0x10);
    ret |= sc530ai_write_reg(dev, 0x331e, 0x49);
    ret |= sc530ai_write_reg(dev, 0x331f, 0x41);
    ret |= sc530ai_write_reg(dev, 0x3333, 0x10);
    ret |= sc530ai_write_reg(dev, 0x335d, 0x60);
    ret |= sc530ai_write_reg(dev, 0x335e, 0x06);
    ret |= sc530ai_write_reg(dev, 0x335f, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3364, 0x56);
    ret |= sc530ai_write_reg(dev, 0x3366, 0x01);
    ret |= sc530ai_write_reg(dev, 0x337c, 0x02);
    ret |= sc530ai_write_reg(dev, 0x337d, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x3390, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3391, 0x03);
    ret |= sc530ai_write_reg(dev, 0x3392, 0x07);
    ret |= sc530ai_write_reg(dev, 0x3393, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3394, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3395, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3396, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3397, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x3398, 0x4f);
    ret |= sc530ai_write_reg(dev, 0x3399, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x339a, 0x0a);
    ret |= sc530ai_write_reg(dev, 0x339b, 0x10);
    ret |= sc530ai_write_reg(dev, 0x339c, 0x22);
    ret |= sc530ai_write_reg(dev, 0x33a2, 0x04);
    ret |= sc530ai_write_reg(dev, 0x33ad, 0x24);
    ret |= sc530ai_write_reg(dev, 0x33ae, 0x38);
    ret |= sc530ai_write_reg(dev, 0x33af, 0x38);
    ret |= sc530ai_write_reg(dev, 0x33b1, 0x80);
    ret |= sc530ai_write_reg(dev, 0x33b2, 0x48);
    ret |= sc530ai_write_reg(dev, 0x33b3, 0x20);
    ret |= sc530ai_write_reg(dev, 0x349f, 0x02);
    ret |= sc530ai_write_reg(dev, 0x34a6, 0x48);
    ret |= sc530ai_write_reg(dev, 0x34a7, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x34a8, 0x20);
    ret |= sc530ai_write_reg(dev, 0x34a9, 0x18);
    ret |= sc530ai_write_reg(dev, 0x34f8, 0x5f);
    ret |= sc530ai_write_reg(dev, 0x34f9, 0x04);
    ret |= sc530ai_write_reg(dev, 0x3632, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3633, 0x32);
    ret |= sc530ai_write_reg(dev, 0x3637, 0x29);
    ret |= sc530ai_write_reg(dev, 0x3638, 0xc1);
    ret |= sc530ai_write_reg(dev, 0x363b, 0x20);
    ret |= sc530ai_write_reg(dev, 0x363d, 0x02);
    ret |= sc530ai_write_reg(dev, 0x3670, 0x09);
    ret |= sc530ai_write_reg(dev, 0x3674, 0x88);
    ret |= sc530ai_write_reg(dev, 0x3675, 0x88);
    ret |= sc530ai_write_reg(dev, 0x3676, 0x88);
    ret |= sc530ai_write_reg(dev, 0x367c, 0x40);
    ret |= sc530ai_write_reg(dev, 0x367d, 0x48);
    ret |= sc530ai_write_reg(dev, 0x3690, 0x33);
    ret |= sc530ai_write_reg(dev, 0x3691, 0x34);
    ret |= sc530ai_write_reg(dev, 0x3692, 0x55);
    ret |= sc530ai_write_reg(dev, 0x3693, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x3694, 0x4f);
    ret |= sc530ai_write_reg(dev, 0x3698, 0x85);
    ret |= sc530ai_write_reg(dev, 0x3699, 0x8f);
    ret |= sc530ai_write_reg(dev, 0x369a, 0xa0);
    ret |= sc530ai_write_reg(dev, 0x369b, 0xc3);
    ret |= sc530ai_write_reg(dev, 0x36a2, 0x49);
    ret |= sc530ai_write_reg(dev, 0x36a3, 0x4b);
    ret |= sc530ai_write_reg(dev, 0x36a4, 0x4f);
    ret |= sc530ai_write_reg(dev, 0x36d0, 0x01);
    ret |= sc530ai_write_reg(dev, 0x370f, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3722, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3728, 0x10);
    ret |= sc530ai_write_reg(dev, 0x37b0, 0x03);
    ret |= sc530ai_write_reg(dev, 0x37b1, 0x03);
    ret |= sc530ai_write_reg(dev, 0x37b2, 0x83);
    ret |= sc530ai_write_reg(dev, 0x37b3, 0x48);
    ret |= sc530ai_write_reg(dev, 0x37b4, 0x4f);
    ret |= sc530ai_write_reg(dev, 0x3901, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3902, 0xc5);
    ret |= sc530ai_write_reg(dev, 0x3904, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3905, 0x8d);
    ret |= sc530ai_write_reg(dev, 0x3909, 0x00);
    ret |= sc530ai_write_reg(dev, 0x391d, 0x04);
    ret |= sc530ai_write_reg(dev, 0x3926, 0x21);
    ret |= sc530ai_write_reg(dev, 0x3929, 0x18);
    ret |= sc530ai_write_reg(dev, 0x3933, 0x82);
    ret |= sc530ai_write_reg(dev, 0x3934, 0x08);
    ret |= sc530ai_write_reg(dev, 0x3937, 0x5b);
    ret |= sc530ai_write_reg(dev, 0x3939, 0x00);
    ret |= sc530ai_write_reg(dev, 0x393a, 0x01);
    ret |= sc530ai_write_reg(dev, 0x39dc, 0x02);
    ret |= sc530ai_write_reg(dev, 0x3c0f, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3e00, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3e01, 0x82);
    ret |= sc530ai_write_reg(dev, 0x3e02, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3e04, 0x18);
    ret |= sc530ai_write_reg(dev, 0x3e05, 0x20);
    ret |= sc530ai_write_reg(dev, 0x3e23, 0x00);
    ret |= sc530ai_write_reg(dev, 0x3e24, 0xc8);
    ret |= sc530ai_write_reg(dev, 0x440e, 0x02);
    ret |= sc530ai_write_reg(dev, 0x4509, 0x20);
    ret |= sc530ai_write_reg(dev, 0x4816, 0x11);
    ret |= sc530ai_write_reg(dev, 0x5010, 0x10);
    ret |= sc530ai_write_reg(dev, 0x5799, 0x06);
    ret |= sc530ai_write_reg(dev, 0x57ad, 0x00);
    ret |= sc530ai_write_reg(dev, 0x5ae0, 0xfe);
    ret |= sc530ai_write_reg(dev, 0x5ae1, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5ae2, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5ae3, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5ae4, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5ae5, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5ae6, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5ae7, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5ae8, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5ae9, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aea, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5aeb, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5aec, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aed, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5aee, 0xfe);
    ret |= sc530ai_write_reg(dev, 0x5aef, 0x40);
    ret |= sc530ai_write_reg(dev, 0x5af4, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5af5, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5af6, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5af7, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5af8, 0x2a);
    ret |= sc530ai_write_reg(dev, 0x5af9, 0x24);
    ret |= sc530ai_write_reg(dev, 0x5afa, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5afb, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5afc, 0x28);
    ret |= sc530ai_write_reg(dev, 0x5afd, 0x3c);
    ret |= sc530ai_write_reg(dev, 0x5afe, 0x30);
    ret |= sc530ai_write_reg(dev, 0x5aff, 0x28);
    ret |= sc530ai_write_reg(dev, 0x36e9, 0x44);
    ret |= sc530ai_write_reg(dev, 0x37f9, 0x44);
    ret |= sc530ai_write_reg(dev, 0x0100, 0x01);
    ret |= sc530ai_write_reg(dev, 0x3221, 0x60);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc530ai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================================\n");
        SENSOR_PRINT("======  sc530ai_27Minput_792Mbps_raw10_2880X1620_2to1wdr_VC_30fps_4lane init success!  =====\n");
        SENSOR_PRINT("============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_void sc530ai_slave_init(xmedia_u32 dev)
{
    sc530ai_write_reg(dev, 0x0100, 0x00);
    sc530ai_write_reg(dev, 0x3222, 0x01); // BIT[0] work
    sc530ai_write_reg(dev, 0x3225, 0x20);
    sc530ai_write_reg(dev, 0x3224, 0x82);
    sc530ai_write_reg(dev, 0x300a, 0x20);
    sc530ai_write_reg(dev, 0x3230, 0x00);
    sc530ai_write_reg(dev, 0x3231, 0x04);
    sc530ai_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("====== SC530AI slave setting init success!=======\n");
}

xmedia_s32 sc530ai_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC530AI_5M_30FPS_10BIT_LINEAR_MODE:
            ret = sc530ai_4lane_linear_2880x1620_init(dev);
            break;

        case SC530AI_2L_5M_25FPS_10BIT_LINEAR_MODE:
            ret = sc530ai_2lane_linear_2880x1620_init(dev);
            break;

        case SC530AI_5M_30FPS_10BIT_WDR_MODE:
            ret = sc530ai_4lane_wdr_2880x1620_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (image_mode != SC530AI_5M_30FPS_10BIT_WDR_MODE && work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        sc530ai_slave_init(dev);
    }

    sc530ai_delay_ms(10);

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc530ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc530ai_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc530ai_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc530ai_i2c_fd[dev]);
        g_sc530ai_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc530ai_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc530ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc530ai_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc530ai_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc530ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc530ai_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc530ai_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc530ai_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc530ai_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc530ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC530AI_DATA_BYTE];

    if (g_sc530ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc530ai_i2c_fd[dev], g_sc530ai_i2c_addr[dev], addr, SC530AI_ADDR_BYTE, buf, SC530AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC530AI_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc530ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc530ai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc530ai_i2c_fd[dev], g_sc530ai_i2c_addr[dev], buf, SC530AI_ADDR_BYTE + SC530AI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 sc530ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc530ai_read_reg(dev, SC530AI_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc530ai_write_reg(dev, SC530AI_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc530ai_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
