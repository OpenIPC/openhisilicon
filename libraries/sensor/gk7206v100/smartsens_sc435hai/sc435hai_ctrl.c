#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc435hai.h"
#include "sc435hai_ctrl.h"

static xmedia_s32 g_sc435hai_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_sc435hai_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define SC435HAI_REG_ADDR_MIRROR_FLIP 0x3221

/*
 * 函数功能: sc435hai MIPI 2lane 1440p线性模式初始化序列 - 2560x1440x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc435hai_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc435hai_write_reg(dev, 0x0100, 0x00);
    ret |= sc435hai_write_reg(dev, 0x36e9, 0x80);
    ret |= sc435hai_write_reg(dev, 0x37f9, 0x80);
    ret |= sc435hai_write_reg(dev, 0x3018, 0x3a);
    ret |= sc435hai_write_reg(dev, 0x3019, 0x0c);
    ret |= sc435hai_write_reg(dev, 0x301f, 0x06);
    ret |= sc435hai_write_reg(dev, 0x3058, 0x21);
    ret |= sc435hai_write_reg(dev, 0x3059, 0x53);
    ret |= sc435hai_write_reg(dev, 0x305a, 0x40);
    ret |= sc435hai_write_reg(dev, 0x320c, 0x05);
    ret |= sc435hai_write_reg(dev, 0x320d, 0xdc);
    ret |= sc435hai_write_reg(dev, 0x320e, 0x07);
    ret |= sc435hai_write_reg(dev, 0x320f, 0xd0);
    ret |= sc435hai_write_reg(dev, 0x3250, 0x00);
    ret |= sc435hai_write_reg(dev, 0x3301, 0x12);
    ret |= sc435hai_write_reg(dev, 0x3304, 0x50);
    ret |= sc435hai_write_reg(dev, 0x3305, 0x00);
    ret |= sc435hai_write_reg(dev, 0x3306, 0x68);
    ret |= sc435hai_write_reg(dev, 0x3307, 0x04);
    ret |= sc435hai_write_reg(dev, 0x3308, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x3309, 0x88);
    ret |= sc435hai_write_reg(dev, 0x330b, 0xd8);
    ret |= sc435hai_write_reg(dev, 0x330d, 0x08);
    ret |= sc435hai_write_reg(dev, 0x330e, 0x28);
    ret |= sc435hai_write_reg(dev, 0x331e, 0x41);
    ret |= sc435hai_write_reg(dev, 0x331f, 0x79);
    ret |= sc435hai_write_reg(dev, 0x3333, 0x10);
    ret |= sc435hai_write_reg(dev, 0x3334, 0x40);
    ret |= sc435hai_write_reg(dev, 0x3364, 0x5e);
    ret |= sc435hai_write_reg(dev, 0x338e, 0xe2);
    ret |= sc435hai_write_reg(dev, 0x338f, 0x80);
    ret |= sc435hai_write_reg(dev, 0x3390, 0x08);
    ret |= sc435hai_write_reg(dev, 0x3391, 0x18);
    ret |= sc435hai_write_reg(dev, 0x3392, 0xb8);
    ret |= sc435hai_write_reg(dev, 0x3393, 0x12);
    ret |= sc435hai_write_reg(dev, 0x3394, 0x14);
    ret |= sc435hai_write_reg(dev, 0x3395, 0x10);
    ret |= sc435hai_write_reg(dev, 0x3396, 0x88);
    ret |= sc435hai_write_reg(dev, 0x3397, 0x98);
    ret |= sc435hai_write_reg(dev, 0x3398, 0xb8);
    ret |= sc435hai_write_reg(dev, 0x3399, 0x0e);
    ret |= sc435hai_write_reg(dev, 0x339a, 0x18);
    ret |= sc435hai_write_reg(dev, 0x339b, 0x20);
    ret |= sc435hai_write_reg(dev, 0x339c, 0x24);
    ret |= sc435hai_write_reg(dev, 0x33ac, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x33ad, 0x10);
    ret |= sc435hai_write_reg(dev, 0x33ae, 0x4c);
    ret |= sc435hai_write_reg(dev, 0x33af, 0x84);
    ret |= sc435hai_write_reg(dev, 0x33b2, 0x50);
    ret |= sc435hai_write_reg(dev, 0x33b3, 0x10);
    ret |= sc435hai_write_reg(dev, 0x33f8, 0x00);
    ret |= sc435hai_write_reg(dev, 0x33f9, 0x68);
    ret |= sc435hai_write_reg(dev, 0x33fa, 0x00);
    ret |= sc435hai_write_reg(dev, 0x33fb, 0x68);
    ret |= sc435hai_write_reg(dev, 0x33fc, 0x48);
    ret |= sc435hai_write_reg(dev, 0x33fd, 0x78);
    ret |= sc435hai_write_reg(dev, 0x349f, 0x03);
    ret |= sc435hai_write_reg(dev, 0x34a6, 0x40);
    ret |= sc435hai_write_reg(dev, 0x34a7, 0x58);
    ret |= sc435hai_write_reg(dev, 0x34a8, 0x10);
    ret |= sc435hai_write_reg(dev, 0x34a9, 0x10);
    ret |= sc435hai_write_reg(dev, 0x34f8, 0x78);
    ret |= sc435hai_write_reg(dev, 0x34f9, 0x10);
    ret |= sc435hai_write_reg(dev, 0x3633, 0x44);
    ret |= sc435hai_write_reg(dev, 0x363b, 0x8f);
    ret |= sc435hai_write_reg(dev, 0x363c, 0x02);
    ret |= sc435hai_write_reg(dev, 0x3641, 0x08);
    ret |= sc435hai_write_reg(dev, 0x3654, 0x20);
    ret |= sc435hai_write_reg(dev, 0x3674, 0xc2);
    ret |= sc435hai_write_reg(dev, 0x3675, 0xb4);
    ret |= sc435hai_write_reg(dev, 0x3676, 0x88);
    ret |= sc435hai_write_reg(dev, 0x367c, 0x88);
    ret |= sc435hai_write_reg(dev, 0x367d, 0xb8);
    ret |= sc435hai_write_reg(dev, 0x3690, 0x34);
    ret |= sc435hai_write_reg(dev, 0x3691, 0x45);
    ret |= sc435hai_write_reg(dev, 0x3692, 0x55);
    ret |= sc435hai_write_reg(dev, 0x3693, 0x88);
    ret |= sc435hai_write_reg(dev, 0x3694, 0xb8);
    ret |= sc435hai_write_reg(dev, 0x3696, 0x80);
    ret |= sc435hai_write_reg(dev, 0x3697, 0x83);
    ret |= sc435hai_write_reg(dev, 0x3698, 0x81);
    ret |= sc435hai_write_reg(dev, 0x3699, 0x81);
    ret |= sc435hai_write_reg(dev, 0x369a, 0x84);
    ret |= sc435hai_write_reg(dev, 0x369b, 0x82);
    ret |= sc435hai_write_reg(dev, 0x36a2, 0x80);
    ret |= sc435hai_write_reg(dev, 0x36a3, 0x88);
    ret |= sc435hai_write_reg(dev, 0x36a4, 0xf8);
    ret |= sc435hai_write_reg(dev, 0x36a5, 0xb8);
    ret |= sc435hai_write_reg(dev, 0x36a6, 0x98);
    ret |= sc435hai_write_reg(dev, 0x36d0, 0x15);
    ret |= sc435hai_write_reg(dev, 0x36ea, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x36eb, 0x0c);
    ret |= sc435hai_write_reg(dev, 0x36ec, 0x45);
    ret |= sc435hai_write_reg(dev, 0x36ed, 0x28);
    ret |= sc435hai_write_reg(dev, 0x370f, 0x01);
    ret |= sc435hai_write_reg(dev, 0x3722, 0x03);
    ret |= sc435hai_write_reg(dev, 0x3724, 0x92);
    ret |= sc435hai_write_reg(dev, 0x3727, 0x14);
    ret |= sc435hai_write_reg(dev, 0x37b0, 0x17);
    ret |= sc435hai_write_reg(dev, 0x37b1, 0x9b);
    ret |= sc435hai_write_reg(dev, 0x37b2, 0x9b);
    ret |= sc435hai_write_reg(dev, 0x37b3, 0x88);
    ret |= sc435hai_write_reg(dev, 0x37b4, 0xb8);
    ret |= sc435hai_write_reg(dev, 0x37fa, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x37fb, 0x44);
    ret |= sc435hai_write_reg(dev, 0x37fc, 0x20);
    ret |= sc435hai_write_reg(dev, 0x37fd, 0x2c);
    ret |= sc435hai_write_reg(dev, 0x391f, 0x41);
    ret |= sc435hai_write_reg(dev, 0x3926, 0xe0);
    ret |= sc435hai_write_reg(dev, 0x3933, 0x81);
    ret |= sc435hai_write_reg(dev, 0x3934, 0x03);
    ret |= sc435hai_write_reg(dev, 0x3935, 0x07);
    ret |= sc435hai_write_reg(dev, 0x3936, 0xff);
    ret |= sc435hai_write_reg(dev, 0x3937, 0x6b);
    ret |= sc435hai_write_reg(dev, 0x3938, 0x6f);
    ret |= sc435hai_write_reg(dev, 0x3939, 0x00);
    ret |= sc435hai_write_reg(dev, 0x393a, 0x00);
    ret |= sc435hai_write_reg(dev, 0x393b, 0x00);
    ret |= sc435hai_write_reg(dev, 0x393c, 0x00);
    ret |= sc435hai_write_reg(dev, 0x393d, 0x02);
    ret |= sc435hai_write_reg(dev, 0x393e, 0x00);
    ret |= sc435hai_write_reg(dev, 0x3e00, 0x00);
    ret |= sc435hai_write_reg(dev, 0x3e01, 0xf9);
    ret |= sc435hai_write_reg(dev, 0x3e02, 0x50);
    ret |= sc435hai_write_reg(dev, 0x3e16, 0x00);
    ret |= sc435hai_write_reg(dev, 0x3e17, 0xc5);
    ret |= sc435hai_write_reg(dev, 0x3e18, 0x00);
    ret |= sc435hai_write_reg(dev, 0x3e19, 0xc5);
    ret |= sc435hai_write_reg(dev, 0x4509, 0x20);
    ret |= sc435hai_write_reg(dev, 0x450d, 0x0b);
    ret |= sc435hai_write_reg(dev, 0x4819, 0x0b);
    ret |= sc435hai_write_reg(dev, 0x481b, 0x06);
    ret |= sc435hai_write_reg(dev, 0x481d, 0x18);
    ret |= sc435hai_write_reg(dev, 0x481f, 0x05);
    ret |= sc435hai_write_reg(dev, 0x4821, 0x0b);
    ret |= sc435hai_write_reg(dev, 0x4823, 0x06);
    ret |= sc435hai_write_reg(dev, 0x4825, 0x05);
    ret |= sc435hai_write_reg(dev, 0x4827, 0x05);
    ret |= sc435hai_write_reg(dev, 0x4829, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x5780, 0x76);
    ret |= sc435hai_write_reg(dev, 0x5784, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x5785, 0x04);
    ret |= sc435hai_write_reg(dev, 0x5787, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x5788, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x5789, 0x08);
    ret |= sc435hai_write_reg(dev, 0x578a, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x578b, 0x0a);
    ret |= sc435hai_write_reg(dev, 0x578c, 0x08);
    ret |= sc435hai_write_reg(dev, 0x578d, 0x40);
    ret |= sc435hai_write_reg(dev, 0x5790, 0x08);
    ret |= sc435hai_write_reg(dev, 0x5791, 0x04);
    ret |= sc435hai_write_reg(dev, 0x5792, 0x04);
    ret |= sc435hai_write_reg(dev, 0x5793, 0x08);
    ret |= sc435hai_write_reg(dev, 0x5794, 0x04);
    ret |= sc435hai_write_reg(dev, 0x5795, 0x04);
    ret |= sc435hai_write_reg(dev, 0x57ac, 0x00);
    ret |= sc435hai_write_reg(dev, 0x57ad, 0x00);
    ret |= sc435hai_write_reg(dev, 0x36e9, 0x53);
    ret |= sc435hai_write_reg(dev, 0x37f9, 0x53);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc435hai_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("========= SC435HAI_24MInput_MIPI_2lane_10bit_2560x1440_30fps init success!=======\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 sc435hai_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC435HAI_4M_1440_10BIT_LINEAR_MODE:
            ret = sc435hai_2lane_linear_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc435hai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_sc435hai_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc435hai_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc435hai_i2c_fd[dev]);
        g_sc435hai_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc435hai_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc435hai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc435hai_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc435hai_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc435hai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc435hai_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc435hai_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc435hai_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc435hai_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc435hai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC435HAI_DATA_BYTE];

    if (g_sc435hai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc435hai_i2c_fd[dev], g_sc435hai_i2c_addr[dev], addr, SC435HAI_ADDR_BYTE, buf, SC435HAI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC435HAI_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc435hai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc435hai_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc435hai_i2c_fd[dev], g_sc435hai_i2c_addr[dev], buf, SC435HAI_ADDR_BYTE + SC435HAI_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 sc435hai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc435hai_read_reg(dev, SC435HAI_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc435hai_write_reg(dev, SC435HAI_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc435hai_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_set_stream_mode(xmedia_u32 dev, xmedia_bool stream_en) {
    xmedia_s32 ret;

    ret = sc435hai_write_reg(dev, SC435HAI_REG_ADDR_STANDBY, stream_en);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc435hai_start_stream failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

