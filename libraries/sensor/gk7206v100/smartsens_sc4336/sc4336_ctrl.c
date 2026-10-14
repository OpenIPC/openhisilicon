#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc4336.h"
#include "sc4336_ctrl.h"

static xmedia_s32 g_sc4336_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_sc4336_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
#define SC4336_REG_ADDR_MIRROR_FLIP 0x3221
#define SC4336_RECV_BUF_NUM   4

static void gc4336_delay_ms(int ms)
{
    usleep(ms * 1000);
    return;
}

/*
 * 函数功能: sc4336 MIPI 2lane 1440p线性模式初始化序列 - 2560x1440x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc4336_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc4336_write_reg(dev, 0x0103, 0x01);
    gc4336_delay_ms(3);
    ret |= sc4336_write_reg(dev, 0x0100, 0x00);
    ret |= sc4336_write_reg(dev, 0x36e9, 0x80);
    ret |= sc4336_write_reg(dev, 0x37f9, 0x80);
    ret |= sc4336_write_reg(dev, 0x301f, 0x01);
    ret |= sc4336_write_reg(dev, 0x30b8, 0x44);
    ret |= sc4336_write_reg(dev, 0x3253, 0x10);
    ret |= sc4336_write_reg(dev, 0x3301, 0x0a);
    ret |= sc4336_write_reg(dev, 0x3302, 0xff);
    ret |= sc4336_write_reg(dev, 0x3305, 0x00);
    ret |= sc4336_write_reg(dev, 0x3306, 0x90);
    ret |= sc4336_write_reg(dev, 0x3308, 0x20);
    ret |= sc4336_write_reg(dev, 0x330a, 0x01);
    ret |= sc4336_write_reg(dev, 0x330b, 0xb0);
    ret |= sc4336_write_reg(dev, 0x330d, 0xf0);
    ret |= sc4336_write_reg(dev, 0x3333, 0x10);
    ret |= sc4336_write_reg(dev, 0x335e, 0x06);
    ret |= sc4336_write_reg(dev, 0x335f, 0x0a);
    ret |= sc4336_write_reg(dev, 0x3364, 0x5e);
    ret |= sc4336_write_reg(dev, 0x337d, 0x0e);
    ret |= sc4336_write_reg(dev, 0x3390, 0x08);
    ret |= sc4336_write_reg(dev, 0x3391, 0x09);
    ret |= sc4336_write_reg(dev, 0x3392, 0x0f);
    ret |= sc4336_write_reg(dev, 0x3393, 0x18);
    ret |= sc4336_write_reg(dev, 0x3394, 0x60);
    ret |= sc4336_write_reg(dev, 0x3395, 0xff);
    ret |= sc4336_write_reg(dev, 0x3396, 0x08);
    ret |= sc4336_write_reg(dev, 0x3397, 0x09);
    ret |= sc4336_write_reg(dev, 0x3398, 0x0f);
    ret |= sc4336_write_reg(dev, 0x3399, 0x0a);
    ret |= sc4336_write_reg(dev, 0x339a, 0x18);
    ret |= sc4336_write_reg(dev, 0x339b, 0x60);
    ret |= sc4336_write_reg(dev, 0x339c, 0xff);
    ret |= sc4336_write_reg(dev, 0x33a2, 0x04);
    ret |= sc4336_write_reg(dev, 0x33ad, 0x3c);
    ret |= sc4336_write_reg(dev, 0x33b2, 0x40);
    ret |= sc4336_write_reg(dev, 0x33b3, 0x30);
    ret |= sc4336_write_reg(dev, 0x33f8, 0x00);
    ret |= sc4336_write_reg(dev, 0x33f9, 0xa0);
    ret |= sc4336_write_reg(dev, 0x33fa, 0x00);
    ret |= sc4336_write_reg(dev, 0x33fb, 0xc8);
    ret |= sc4336_write_reg(dev, 0x33fc, 0x09);
    ret |= sc4336_write_reg(dev, 0x33fd, 0x1f);
    ret |= sc4336_write_reg(dev, 0x349f, 0x03);
    ret |= sc4336_write_reg(dev, 0x34a6, 0x09);
    ret |= sc4336_write_reg(dev, 0x34a7, 0x1f);
    ret |= sc4336_write_reg(dev, 0x34a8, 0x28);
    ret |= sc4336_write_reg(dev, 0x34a9, 0x28);
    ret |= sc4336_write_reg(dev, 0x34aa, 0x01);
    ret |= sc4336_write_reg(dev, 0x34ab, 0xd0);
    ret |= sc4336_write_reg(dev, 0x34ac, 0x02);
    ret |= sc4336_write_reg(dev, 0x34ad, 0x10);
    ret |= sc4336_write_reg(dev, 0x34f8, 0x1f);
    ret |= sc4336_write_reg(dev, 0x34f9, 0x20);
    ret |= sc4336_write_reg(dev, 0x3630, 0xc0);
    ret |= sc4336_write_reg(dev, 0x3631, 0x84);
    ret |= sc4336_write_reg(dev, 0x3633, 0x44);
    ret |= sc4336_write_reg(dev, 0x3637, 0x4c);
    ret |= sc4336_write_reg(dev, 0x3641, 0x38);
    ret |= sc4336_write_reg(dev, 0x3670, 0x56);
    ret |= sc4336_write_reg(dev, 0x3674, 0xc0);
    ret |= sc4336_write_reg(dev, 0x3675, 0xa0);
    ret |= sc4336_write_reg(dev, 0x3676, 0xa0);
    ret |= sc4336_write_reg(dev, 0x3677, 0x84);
    ret |= sc4336_write_reg(dev, 0x3678, 0x88);
    ret |= sc4336_write_reg(dev, 0x3679, 0x8a);
    ret |= sc4336_write_reg(dev, 0x367c, 0x09);
    ret |= sc4336_write_reg(dev, 0x367d, 0x0b);
    ret |= sc4336_write_reg(dev, 0x367e, 0x08);
    ret |= sc4336_write_reg(dev, 0x367f, 0x0f);
    ret |= sc4336_write_reg(dev, 0x3696, 0x44);
    ret |= sc4336_write_reg(dev, 0x3697, 0x54);
    ret |= sc4336_write_reg(dev, 0x3698, 0x54);
    ret |= sc4336_write_reg(dev, 0x36a0, 0x0f);
    ret |= sc4336_write_reg(dev, 0x36a1, 0x1f);
    ret |= sc4336_write_reg(dev, 0x36b0, 0x81);
    ret |= sc4336_write_reg(dev, 0x36b1, 0x83);
    ret |= sc4336_write_reg(dev, 0x36b2, 0x85);
    ret |= sc4336_write_reg(dev, 0x36b3, 0x8b);
    ret |= sc4336_write_reg(dev, 0x36b4, 0x09);
    ret |= sc4336_write_reg(dev, 0x36b5, 0x0b);
    ret |= sc4336_write_reg(dev, 0x36b6, 0x0f);
    ret |= sc4336_write_reg(dev, 0x370f, 0x01);
    ret |= sc4336_write_reg(dev, 0x3722, 0x09);
    ret |= sc4336_write_reg(dev, 0x3724, 0x21);
    ret |= sc4336_write_reg(dev, 0x3771, 0x09);
    ret |= sc4336_write_reg(dev, 0x3772, 0x05);
    ret |= sc4336_write_reg(dev, 0x3773, 0x05);
    ret |= sc4336_write_reg(dev, 0x377a, 0x0f);
    ret |= sc4336_write_reg(dev, 0x377b, 0x1f);
    ret |= sc4336_write_reg(dev, 0x3905, 0x8c);
    ret |= sc4336_write_reg(dev, 0x391d, 0x04);
    ret |= sc4336_write_reg(dev, 0x3926, 0x21);
    ret |= sc4336_write_reg(dev, 0x3933, 0x80);
    ret |= sc4336_write_reg(dev, 0x3934, 0x03);
    ret |= sc4336_write_reg(dev, 0x3935, 0x00);
    ret |= sc4336_write_reg(dev, 0x3936, 0x1b);
    ret |= sc4336_write_reg(dev, 0x3937, 0x76);
    ret |= sc4336_write_reg(dev, 0x3938, 0x75);
    ret |= sc4336_write_reg(dev, 0x3939, 0x00);
    ret |= sc4336_write_reg(dev, 0x393a, 0x00);
    ret |= sc4336_write_reg(dev, 0x39dc, 0x02);
    ret |= sc4336_write_reg(dev, 0x3e00, 0x00);
    ret |= sc4336_write_reg(dev, 0x3e01, 0x5d);
    ret |= sc4336_write_reg(dev, 0x3e02, 0x40);
    ret |= sc4336_write_reg(dev, 0x440e, 0x02);
    ret |= sc4336_write_reg(dev, 0x4509, 0x28);
    ret |= sc4336_write_reg(dev, 0x450d, 0x32);
    ret |= sc4336_write_reg(dev, 0x5000, 0x06);
    ret |= sc4336_write_reg(dev, 0x5799, 0x46);
    ret |= sc4336_write_reg(dev, 0x579a, 0x77);
    ret |= sc4336_write_reg(dev, 0x57d9, 0x46);
    ret |= sc4336_write_reg(dev, 0x57da, 0x77);
    ret |= sc4336_write_reg(dev, 0x36e9, 0x44);
    ret |= sc4336_write_reg(dev, 0x37f9, 0x44);
    ret |= sc4336_write_reg(dev, 0x0100, 0x01);
    ret |= sc4336_write_reg(dev, 0x3221, 0x60);
    gc4336_delay_ms(10);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc4336_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC4336_27MInput_MIPI_2lane_10bit_2560x1440_30fps init success!=========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: sc4336 SLAVE MIPI 2lane 1440p线性模式初始化序列 - 2560x1440x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
 /*
static xmedia_s32 sc4336_slave_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc4336_write_reg(dev, 0x0103, 0x01);
    gc4336_delay_ms(3);
    ret |= sc4336_write_reg(dev, 0x0100, 0x00);
    ret |= sc4336_write_reg(dev, 0x36e9, 0x80);
    ret |= sc4336_write_reg(dev, 0x37f9, 0x80);
    ret |= sc4336_write_reg(dev, 0x301f, 0x01);
    ret |= sc4336_write_reg(dev, 0x30b8, 0x44);
    ret |= sc4336_write_reg(dev, 0x3253, 0x10);
    ret |= sc4336_write_reg(dev, 0x3301, 0x0a);
    ret |= sc4336_write_reg(dev, 0x3302, 0xff);
    ret |= sc4336_write_reg(dev, 0x3305, 0x00);
    ret |= sc4336_write_reg(dev, 0x3306, 0x90);
    ret |= sc4336_write_reg(dev, 0x3308, 0x20);
    ret |= sc4336_write_reg(dev, 0x330a, 0x01);
    ret |= sc4336_write_reg(dev, 0x330b, 0xb0);
    ret |= sc4336_write_reg(dev, 0x330d, 0xf0);
    ret |= sc4336_write_reg(dev, 0x3333, 0x10);
    ret |= sc4336_write_reg(dev, 0x335e, 0x06);
    ret |= sc4336_write_reg(dev, 0x335f, 0x0a);
    ret |= sc4336_write_reg(dev, 0x3364, 0x5e);
    ret |= sc4336_write_reg(dev, 0x337d, 0x0e);
    ret |= sc4336_write_reg(dev, 0x3390, 0x08);
    ret |= sc4336_write_reg(dev, 0x3391, 0x09);
    ret |= sc4336_write_reg(dev, 0x3392, 0x0f);
    ret |= sc4336_write_reg(dev, 0x3393, 0x18);
    ret |= sc4336_write_reg(dev, 0x3394, 0x60);
    ret |= sc4336_write_reg(dev, 0x3395, 0xff);
    ret |= sc4336_write_reg(dev, 0x3396, 0x08);
    ret |= sc4336_write_reg(dev, 0x3397, 0x09);
    ret |= sc4336_write_reg(dev, 0x3398, 0x0f);
    ret |= sc4336_write_reg(dev, 0x3399, 0x0a);
    ret |= sc4336_write_reg(dev, 0x339a, 0x18);
    ret |= sc4336_write_reg(dev, 0x339b, 0x60);
    ret |= sc4336_write_reg(dev, 0x339c, 0xff);
    ret |= sc4336_write_reg(dev, 0x33a2, 0x04);
    ret |= sc4336_write_reg(dev, 0x33ad, 0x3c);
    ret |= sc4336_write_reg(dev, 0x33b2, 0x40);
    ret |= sc4336_write_reg(dev, 0x33b3, 0x30);
    ret |= sc4336_write_reg(dev, 0x33f8, 0x00);
    ret |= sc4336_write_reg(dev, 0x33f9, 0xa0);
    ret |= sc4336_write_reg(dev, 0x33fa, 0x00);
    ret |= sc4336_write_reg(dev, 0x33fb, 0xc8);
    ret |= sc4336_write_reg(dev, 0x33fc, 0x09);
    ret |= sc4336_write_reg(dev, 0x33fd, 0x1f);
    ret |= sc4336_write_reg(dev, 0x349f, 0x03);
    ret |= sc4336_write_reg(dev, 0x34a6, 0x09);
    ret |= sc4336_write_reg(dev, 0x34a7, 0x1f);
    ret |= sc4336_write_reg(dev, 0x34a8, 0x28);
    ret |= sc4336_write_reg(dev, 0x34a9, 0x28);
    ret |= sc4336_write_reg(dev, 0x34aa, 0x01);
    ret |= sc4336_write_reg(dev, 0x34ab, 0xd0);
    ret |= sc4336_write_reg(dev, 0x34ac, 0x02);
    ret |= sc4336_write_reg(dev, 0x34ad, 0x10);
    ret |= sc4336_write_reg(dev, 0x34f8, 0x1f);
    ret |= sc4336_write_reg(dev, 0x34f9, 0x20);
    ret |= sc4336_write_reg(dev, 0x3630, 0xc0);
    ret |= sc4336_write_reg(dev, 0x3631, 0x84);
    ret |= sc4336_write_reg(dev, 0x3633, 0x44);
    ret |= sc4336_write_reg(dev, 0x3637, 0x4c);
    ret |= sc4336_write_reg(dev, 0x3641, 0x38);
    ret |= sc4336_write_reg(dev, 0x3670, 0x56);
    ret |= sc4336_write_reg(dev, 0x3674, 0xc0);
    ret |= sc4336_write_reg(dev, 0x3675, 0xa0);
    ret |= sc4336_write_reg(dev, 0x3676, 0xa0);
    ret |= sc4336_write_reg(dev, 0x3677, 0x84);
    ret |= sc4336_write_reg(dev, 0x3678, 0x88);
    ret |= sc4336_write_reg(dev, 0x3679, 0x8a);
    ret |= sc4336_write_reg(dev, 0x367c, 0x09);
    ret |= sc4336_write_reg(dev, 0x367d, 0x0b);
    ret |= sc4336_write_reg(dev, 0x367e, 0x08);
    ret |= sc4336_write_reg(dev, 0x367f, 0x0f);
    ret |= sc4336_write_reg(dev, 0x3696, 0x44);
    ret |= sc4336_write_reg(dev, 0x3697, 0x54);
    ret |= sc4336_write_reg(dev, 0x3698, 0x54);
    ret |= sc4336_write_reg(dev, 0x36a0, 0x0f);
    ret |= sc4336_write_reg(dev, 0x36a1, 0x1f);
    ret |= sc4336_write_reg(dev, 0x36b0, 0x81);
    ret |= sc4336_write_reg(dev, 0x36b1, 0x83);
    ret |= sc4336_write_reg(dev, 0x36b2, 0x85);
    ret |= sc4336_write_reg(dev, 0x36b3, 0x8b);
    ret |= sc4336_write_reg(dev, 0x36b4, 0x09);
    ret |= sc4336_write_reg(dev, 0x36b5, 0x0b);
    ret |= sc4336_write_reg(dev, 0x36b6, 0x0f);
    ret |= sc4336_write_reg(dev, 0x370f, 0x01);
    ret |= sc4336_write_reg(dev, 0x3722, 0x09);
    ret |= sc4336_write_reg(dev, 0x3724, 0x21);
    ret |= sc4336_write_reg(dev, 0x3771, 0x09);
    ret |= sc4336_write_reg(dev, 0x3772, 0x05);
    ret |= sc4336_write_reg(dev, 0x3773, 0x05);
    ret |= sc4336_write_reg(dev, 0x377a, 0x0f);
    ret |= sc4336_write_reg(dev, 0x377b, 0x1f);
    ret |= sc4336_write_reg(dev, 0x3905, 0x8c);
    ret |= sc4336_write_reg(dev, 0x391d, 0x04);
    ret |= sc4336_write_reg(dev, 0x3926, 0x21);
    ret |= sc4336_write_reg(dev, 0x3933, 0x80);
    ret |= sc4336_write_reg(dev, 0x3934, 0x03);
    ret |= sc4336_write_reg(dev, 0x3935, 0x00);
    ret |= sc4336_write_reg(dev, 0x3936, 0x1b);
    ret |= sc4336_write_reg(dev, 0x3937, 0x76);
    ret |= sc4336_write_reg(dev, 0x3938, 0x75);
    ret |= sc4336_write_reg(dev, 0x3939, 0x00);
    ret |= sc4336_write_reg(dev, 0x393a, 0x00);
    ret |= sc4336_write_reg(dev, 0x39dc, 0x02);
    ret |= sc4336_write_reg(dev, 0x3e00, 0x00);
    ret |= sc4336_write_reg(dev, 0x3e01, 0x5d);
    ret |= sc4336_write_reg(dev, 0x3e02, 0x40);
    ret |= sc4336_write_reg(dev, 0x440e, 0x02);
    ret |= sc4336_write_reg(dev, 0x4509, 0x28);
    ret |= sc4336_write_reg(dev, 0x450d, 0x32);
    ret |= sc4336_write_reg(dev, 0x5000, 0x06);
    ret |= sc4336_write_reg(dev, 0x5799, 0x46);
    ret |= sc4336_write_reg(dev, 0x579a, 0x77);
    ret |= sc4336_write_reg(dev, 0x57d9, 0x46);
    ret |= sc4336_write_reg(dev, 0x57da, 0x77);
    ret |= sc4336_write_reg(dev, 0x36e9, 0x44);
    ret |= sc4336_write_reg(dev, 0x37f9, 0x44);

    ret |= sc4336_write_reg(dev, 0x322e, 0x0b);
    ret |= sc4336_write_reg(dev, 0x322f, 0xb4);
    ret |= sc4336_write_reg(dev, 0x320e, 0x0b);
    ret |= sc4336_write_reg(dev, 0x320f, 0xb8);
    ret |= sc4336_write_reg(dev, 0x0100, 0x01);
    ret |= sc4336_write_reg(dev, 0x3221, 0x60);
    //slave mode
    ret |= sc4336_write_reg(dev, 0x3222, 0x01);
    ret |= sc4336_write_reg(dev, 0x3225, 0x10);
    ret |= sc4336_write_reg(dev, 0x3224, 0x82);
    ret |= sc4336_write_reg(dev, 0x3230, 0x00);
    ret |= sc4336_write_reg(dev, 0x3231, 0x04);
    gc4336_delay_ms(10);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc4336_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=====================================================================================\n");
        SENSOR_PRINT("=========SC4336_SLAVE_27MInput_MIPI_2lane_10bit_2560x1440_30fps init success!========\n");
        SENSOR_PRINT("=====================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}
*/
xmedia_s32 sc4336_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC4336_4M_30FPS_10BIT_LINEAR_MODE:
            ret = sc4336_2lane_linear_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc4336_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc4336_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc4336_i2c_fd[dev]);
        g_sc4336_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc4336_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc4336_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc4336_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc4336_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc4336_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc4336_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc4336_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC4336_DATA_BYTE];

    if (g_sc4336_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc4336_i2c_fd[dev], g_sc4336_i2c_addr[dev], addr, SC4336_ADDR_BYTE, buf, SC4336_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC4336_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc4336_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc4336_i2c_fd[dev], g_sc4336_i2c_addr[dev], buf, SC4336_ADDR_BYTE + SC4336_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 sc4336_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc4336_read_reg(dev, SC4336_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc4336_write_reg(dev, SC4336_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc4336_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
