#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc4336p.h"
#include "sc4336p_ctrl.h"

static xmedia_s32 g_sc4336p_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_sc4336p_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
#define SC4336P_REG_ADDR_MIRROR_FLIP 0x3221

/*
 * 函数功能: sc4336p MIPI 2lane 1440p线性模式初始化序列 - 2560x1440x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc4336p_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc4336p_write_reg(dev, 0x0103, 0x01);
    ret |= sc4336p_write_reg(dev, 0x36e9, 0x80);
    ret |= sc4336p_write_reg(dev, 0x37f9, 0x80);
    ret |= sc4336p_write_reg(dev, 0x301f, 0x01);
    ret |= sc4336p_write_reg(dev, 0x30b8, 0x44);
    ret |= sc4336p_write_reg(dev, 0x3028, 0x37);
    ret |= sc4336p_write_reg(dev, 0x3250, 0xff);
    ret |= sc4336p_write_reg(dev, 0x3253, 0x10);
    ret |= sc4336p_write_reg(dev, 0x3301, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x3302, 0xff);
    ret |= sc4336p_write_reg(dev, 0x3305, 0x00);
    ret |= sc4336p_write_reg(dev, 0x3306, 0x90);
    ret |= sc4336p_write_reg(dev, 0x3308, 0x08);
    ret |= sc4336p_write_reg(dev, 0x330a, 0x01);
    ret |= sc4336p_write_reg(dev, 0x330b, 0xb0);
    ret |= sc4336p_write_reg(dev, 0x330d, 0xf0);
    ret |= sc4336p_write_reg(dev, 0x3314, 0x14);
    ret |= sc4336p_write_reg(dev, 0x3333, 0x10);
    ret |= sc4336p_write_reg(dev, 0x3334, 0x40);
    ret |= sc4336p_write_reg(dev, 0x335e, 0x06);
    ret |= sc4336p_write_reg(dev, 0x335f, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x3364, 0x5e);
    ret |= sc4336p_write_reg(dev, 0x337d, 0x0e);
    ret |= sc4336p_write_reg(dev, 0x338f, 0x20);
    ret |= sc4336p_write_reg(dev, 0x3390, 0x08);
    ret |= sc4336p_write_reg(dev, 0x3391, 0x09);
    ret |= sc4336p_write_reg(dev, 0x3392, 0x0f);
    ret |= sc4336p_write_reg(dev, 0x3393, 0x18);
    ret |= sc4336p_write_reg(dev, 0x3394, 0x60);
    ret |= sc4336p_write_reg(dev, 0x3395, 0xff);
    ret |= sc4336p_write_reg(dev, 0x3396, 0x08);
    ret |= sc4336p_write_reg(dev, 0x3397, 0x09);
    ret |= sc4336p_write_reg(dev, 0x3398, 0x0f);
    ret |= sc4336p_write_reg(dev, 0x3399, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x339a, 0x18);
    ret |= sc4336p_write_reg(dev, 0x339b, 0x60);
    ret |= sc4336p_write_reg(dev, 0x339c, 0xff);
    ret |= sc4336p_write_reg(dev, 0x33a2, 0x04);
    ret |= sc4336p_write_reg(dev, 0x33ad, 0x0c);
    ret |= sc4336p_write_reg(dev, 0x33b2, 0x40);
    ret |= sc4336p_write_reg(dev, 0x33b3, 0x30);
    ret |= sc4336p_write_reg(dev, 0x33f8, 0x00);
    ret |= sc4336p_write_reg(dev, 0x33f9, 0xb0);
    ret |= sc4336p_write_reg(dev, 0x33fa, 0x00);
    ret |= sc4336p_write_reg(dev, 0x33fb, 0xf8);
    ret |= sc4336p_write_reg(dev, 0x33fc, 0x09);
    ret |= sc4336p_write_reg(dev, 0x33fd, 0x1f);
    ret |= sc4336p_write_reg(dev, 0x349f, 0x03);
    ret |= sc4336p_write_reg(dev, 0x34a6, 0x09);
    ret |= sc4336p_write_reg(dev, 0x34a7, 0x1f);
    ret |= sc4336p_write_reg(dev, 0x34a8, 0x28);
    ret |= sc4336p_write_reg(dev, 0x34a9, 0x28);
    ret |= sc4336p_write_reg(dev, 0x34aa, 0x01);
    ret |= sc4336p_write_reg(dev, 0x34ab, 0xe0);
    ret |= sc4336p_write_reg(dev, 0x34ac, 0x02);
    ret |= sc4336p_write_reg(dev, 0x34ad, 0x28);
    ret |= sc4336p_write_reg(dev, 0x34f8, 0x1f);
    ret |= sc4336p_write_reg(dev, 0x34f9, 0x20);
    ret |= sc4336p_write_reg(dev, 0x3630, 0xc0);
    ret |= sc4336p_write_reg(dev, 0x3631, 0x84);
    ret |= sc4336p_write_reg(dev, 0x3632, 0x54);
    ret |= sc4336p_write_reg(dev, 0x3633, 0x44);
    ret |= sc4336p_write_reg(dev, 0x3637, 0x49);
    ret |= sc4336p_write_reg(dev, 0x363f, 0xc0);
    ret |= sc4336p_write_reg(dev, 0x3641, 0x28);
    ret |= sc4336p_write_reg(dev, 0x3670, 0x56);
    ret |= sc4336p_write_reg(dev, 0x3674, 0xb0);
    ret |= sc4336p_write_reg(dev, 0x3675, 0xa0);
    ret |= sc4336p_write_reg(dev, 0x3676, 0xa0);
    ret |= sc4336p_write_reg(dev, 0x3677, 0x84);
    ret |= sc4336p_write_reg(dev, 0x3678, 0x88);
    ret |= sc4336p_write_reg(dev, 0x3679, 0x8d);
    ret |= sc4336p_write_reg(dev, 0x367c, 0x09);
    ret |= sc4336p_write_reg(dev, 0x367d, 0x0b);
    ret |= sc4336p_write_reg(dev, 0x367e, 0x08);
    ret |= sc4336p_write_reg(dev, 0x367f, 0x0f);
    ret |= sc4336p_write_reg(dev, 0x3696, 0x24);
    ret |= sc4336p_write_reg(dev, 0x3697, 0x34);
    ret |= sc4336p_write_reg(dev, 0x3698, 0x34);
    ret |= sc4336p_write_reg(dev, 0x36a0, 0x0f);
    ret |= sc4336p_write_reg(dev, 0x36a1, 0x1f);
    ret |= sc4336p_write_reg(dev, 0x36b0, 0x81);
    ret |= sc4336p_write_reg(dev, 0x36b1, 0x83);
    ret |= sc4336p_write_reg(dev, 0x36b2, 0x85);
    ret |= sc4336p_write_reg(dev, 0x36b3, 0x8b);
    ret |= sc4336p_write_reg(dev, 0x36b4, 0x09);
    ret |= sc4336p_write_reg(dev, 0x36b5, 0x0b);
    ret |= sc4336p_write_reg(dev, 0x36b6, 0x0f);
    ret |= sc4336p_write_reg(dev, 0x370f, 0x01);
    ret |= sc4336p_write_reg(dev, 0x3722, 0x09);
    ret |= sc4336p_write_reg(dev, 0x3724, 0x21);
    ret |= sc4336p_write_reg(dev, 0x3771, 0x09);
    ret |= sc4336p_write_reg(dev, 0x3772, 0x05);
    ret |= sc4336p_write_reg(dev, 0x3773, 0x05);
    ret |= sc4336p_write_reg(dev, 0x377a, 0x0f);
    ret |= sc4336p_write_reg(dev, 0x377b, 0x1f);
    ret |= sc4336p_write_reg(dev, 0x3905, 0x8c);
    ret |= sc4336p_write_reg(dev, 0x391d, 0x02);
    ret |= sc4336p_write_reg(dev, 0x391f, 0x49);
    ret |= sc4336p_write_reg(dev, 0x3926, 0x21);
    ret |= sc4336p_write_reg(dev, 0x3933, 0x80);
    ret |= sc4336p_write_reg(dev, 0x3934, 0x03);
    ret |= sc4336p_write_reg(dev, 0x3937, 0x7b);
    ret |= sc4336p_write_reg(dev, 0x3939, 0x00);
    ret |= sc4336p_write_reg(dev, 0x393a, 0x00);
    ret |= sc4336p_write_reg(dev, 0x39dc, 0x02);
    ret |= sc4336p_write_reg(dev, 0x3e00, 0x00);
    ret |= sc4336p_write_reg(dev, 0x3e01, 0x5d);
    ret |= sc4336p_write_reg(dev, 0x3e02, 0x40);
    ret |= sc4336p_write_reg(dev, 0x440d, 0x10);
    ret |= sc4336p_write_reg(dev, 0x440e, 0x01);
    ret |= sc4336p_write_reg(dev, 0x4509, 0x28);
    ret |= sc4336p_write_reg(dev, 0x450d, 0x32);
    ret |= sc4336p_write_reg(dev, 0x5000, 0x06);
    ret |= sc4336p_write_reg(dev, 0x5780, 0x76);
    ret |= sc4336p_write_reg(dev, 0x5784, 0x10);
    ret |= sc4336p_write_reg(dev, 0x5785, 0x04);
    ret |= sc4336p_write_reg(dev, 0x5787, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x5788, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x5789, 0x08);
    ret |= sc4336p_write_reg(dev, 0x578a, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x578b, 0x0a);
    ret |= sc4336p_write_reg(dev, 0x578c, 0x08);
    ret |= sc4336p_write_reg(dev, 0x578d, 0x40);
    ret |= sc4336p_write_reg(dev, 0x5790, 0x08);
    ret |= sc4336p_write_reg(dev, 0x5791, 0x04);
    ret |= sc4336p_write_reg(dev, 0x5792, 0x04);
    ret |= sc4336p_write_reg(dev, 0x5793, 0x08);
    ret |= sc4336p_write_reg(dev, 0x5794, 0x04);
    ret |= sc4336p_write_reg(dev, 0x5795, 0x04);
    ret |= sc4336p_write_reg(dev, 0x5799, 0x46);
    ret |= sc4336p_write_reg(dev, 0x579a, 0x77);
    ret |= sc4336p_write_reg(dev, 0x57a1, 0x04);
    ret |= sc4336p_write_reg(dev, 0x57a8, 0xd2);
    ret |= sc4336p_write_reg(dev, 0x57aa, 0x2a);
    ret |= sc4336p_write_reg(dev, 0x57ab, 0x7f);
    ret |= sc4336p_write_reg(dev, 0x57ac, 0x00);
    ret |= sc4336p_write_reg(dev, 0x57ad, 0x00);
    ret |= sc4336p_write_reg(dev, 0x57d9, 0x46);
    ret |= sc4336p_write_reg(dev, 0x57da, 0x77);
    ret |= sc4336p_write_reg(dev, 0x59e2, 0x08);
    ret |= sc4336p_write_reg(dev, 0x59e3, 0x03);
    ret |= sc4336p_write_reg(dev, 0x59e4, 0x00);
    ret |= sc4336p_write_reg(dev, 0x59e5, 0x10);
    ret |= sc4336p_write_reg(dev, 0x59e6, 0x06);
    ret |= sc4336p_write_reg(dev, 0x59e7, 0x00);
    ret |= sc4336p_write_reg(dev, 0x59e8, 0x08);
    ret |= sc4336p_write_reg(dev, 0x59e9, 0x02);
    ret |= sc4336p_write_reg(dev, 0x59ea, 0x00);
    ret |= sc4336p_write_reg(dev, 0x59eb, 0x10);
    ret |= sc4336p_write_reg(dev, 0x59ec, 0x04);
    ret |= sc4336p_write_reg(dev, 0x59ed, 0x00);
    ret |= sc4336p_write_reg(dev, 0x5ae0, 0xfe);
    ret |= sc4336p_write_reg(dev, 0x5ae1, 0x40);
    ret |= sc4336p_write_reg(dev, 0x5ae2, 0x38);
    ret |= sc4336p_write_reg(dev, 0x5ae3, 0x30);
    ret |= sc4336p_write_reg(dev, 0x5ae4, 0x28);
    ret |= sc4336p_write_reg(dev, 0x5ae5, 0x38);
    ret |= sc4336p_write_reg(dev, 0x5ae6, 0x30);
    ret |= sc4336p_write_reg(dev, 0x5ae7, 0x28);
    ret |= sc4336p_write_reg(dev, 0x5ae8, 0x3f);
    ret |= sc4336p_write_reg(dev, 0x5ae9, 0x34);
    ret |= sc4336p_write_reg(dev, 0x5aea, 0x2c);
    ret |= sc4336p_write_reg(dev, 0x5aeb, 0x3f);
    ret |= sc4336p_write_reg(dev, 0x5aec, 0x34);
    ret |= sc4336p_write_reg(dev, 0x5aed, 0x2c);
    ret |= sc4336p_write_reg(dev, 0x36e9, 0x44);
    ret |= sc4336p_write_reg(dev, 0x37f9, 0x44);
    ret |= sc4336p_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc4336p_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC4336P_27MInput_MIPI_2lane_10bit_2560x1440_30fps init success!========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_void sc4336p_slave_init(xmedia_u32 dev)
{
    sc4336p_write_reg(dev, 0x0100, 0x00);
    sc4336p_write_reg(dev, 0x301f, 0x0a);
    sc4336p_write_reg(dev, 0x3222, 0x01);//slave fsync
    sc4336p_write_reg(dev, 0x300a, 0x22);
    sc4336p_write_reg(dev, 0x3224, 0x92);
    sc4336p_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("====== SC4336P slave setting init success!=======\n");
}

xmedia_s32 sc4336p_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC4336P_4M_30FPS_10BIT_LINEAR_MODE:
            ret = sc4336p_2lane_linear_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        sc4336p_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336p_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_sc4336p_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc4336p_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc4336p_i2c_fd[dev]);
        g_sc4336p_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc4336p_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336p_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc4336p_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc4336p_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc4336p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc4336p_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336p_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc4336p_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc4336p_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336p_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC4336P_DATA_BYTE];

    if (g_sc4336p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc4336p_i2c_fd[dev], g_sc4336p_i2c_addr[dev], addr, SC4336P_ADDR_BYTE, buf, SC4336P_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC4336P_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc4336p_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc4336p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc4336p_i2c_fd[dev], g_sc4336p_i2c_addr[dev], buf, SC4336P_ADDR_BYTE + SC4336P_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 sc4336p_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc4336p_read_reg(dev, SC4336P_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc4336p_write_reg(dev, SC4336P_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc4336p_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
