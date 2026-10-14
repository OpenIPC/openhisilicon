#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc2337p.h"
#include "sc2337p_ctrl.h"

static xmedia_s32 g_sc2337p_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_sc2337p_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define SC2337P_REG_ADDR_MIRROR_FLIP 0x3221

xmedia_void sc2337p_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

/*
 * 函数功能: sc2337p MIPI 2lane 1080p线性模式初始化序列 - 1920x1080x15fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc2337p_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc2337p_write_reg(dev, 0x0103, 0x01);
    sc2337p_delay_ms(10);
    ret |= sc2337p_write_reg(dev, 0x0100, 0x00);
    ret |= sc2337p_write_reg(dev, 0x36e9, 0x80);
    ret |= sc2337p_write_reg(dev, 0x37f9, 0x80);
    ret |= sc2337p_write_reg(dev, 0x301f, 0x02);
    ret |= sc2337p_write_reg(dev, 0x3106, 0x05);
    ret |= sc2337p_write_reg(dev, 0x3248, 0x04);
    ret |= sc2337p_write_reg(dev, 0x3249, 0x0b);
    ret |= sc2337p_write_reg(dev, 0x3253, 0x08);
    ret |= sc2337p_write_reg(dev, 0x3301, 0x09);
    ret |= sc2337p_write_reg(dev, 0x3302, 0xff);
    ret |= sc2337p_write_reg(dev, 0x3303, 0x10);
    ret |= sc2337p_write_reg(dev, 0x3306, 0x80);
    ret |= sc2337p_write_reg(dev, 0x3307, 0x02);
    ret |= sc2337p_write_reg(dev, 0x3309, 0xc8);
    ret |= sc2337p_write_reg(dev, 0x330a, 0x01);
    ret |= sc2337p_write_reg(dev, 0x330b, 0x30);
    ret |= sc2337p_write_reg(dev, 0x330c, 0x16);
    ret |= sc2337p_write_reg(dev, 0x330d, 0xff);
    ret |= sc2337p_write_reg(dev, 0x3318, 0x02);
    ret |= sc2337p_write_reg(dev, 0x331f, 0xb9);
    ret |= sc2337p_write_reg(dev, 0x3321, 0x0a);
    ret |= sc2337p_write_reg(dev, 0x3327, 0x0e);
    ret |= sc2337p_write_reg(dev, 0x332b, 0x12);
    ret |= sc2337p_write_reg(dev, 0x3333, 0x10);
    ret |= sc2337p_write_reg(dev, 0x3334, 0x40);
    ret |= sc2337p_write_reg(dev, 0x335e, 0x06);
    ret |= sc2337p_write_reg(dev, 0x335f, 0x0a);
    ret |= sc2337p_write_reg(dev, 0x3364, 0x1f);
    ret |= sc2337p_write_reg(dev, 0x337c, 0x02);
    ret |= sc2337p_write_reg(dev, 0x337d, 0x0e);
    ret |= sc2337p_write_reg(dev, 0x3390, 0x09);
    ret |= sc2337p_write_reg(dev, 0x3391, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x3392, 0x1f);
    ret |= sc2337p_write_reg(dev, 0x3393, 0x20);
    ret |= sc2337p_write_reg(dev, 0x3394, 0x20);
    ret |= sc2337p_write_reg(dev, 0x3395, 0xe0);
    ret |= sc2337p_write_reg(dev, 0x33a2, 0x04);
    ret |= sc2337p_write_reg(dev, 0x33b1, 0x80);
    ret |= sc2337p_write_reg(dev, 0x33b2, 0x68);
    ret |= sc2337p_write_reg(dev, 0x33b3, 0x42);
    ret |= sc2337p_write_reg(dev, 0x33f9, 0x90);
    ret |= sc2337p_write_reg(dev, 0x33fb, 0xd0);
    ret |= sc2337p_write_reg(dev, 0x33fc, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x33fd, 0x1f);
    ret |= sc2337p_write_reg(dev, 0x349f, 0x03);
    ret |= sc2337p_write_reg(dev, 0x34a6, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x34a7, 0x1f);
    ret |= sc2337p_write_reg(dev, 0x34a8, 0x42);
    ret |= sc2337p_write_reg(dev, 0x34a9, 0x18);
    ret |= sc2337p_write_reg(dev, 0x34aa, 0x01);
    ret |= sc2337p_write_reg(dev, 0x34ab, 0x43);
    ret |= sc2337p_write_reg(dev, 0x34ac, 0x01);
    ret |= sc2337p_write_reg(dev, 0x34ad, 0x80);
    ret |= sc2337p_write_reg(dev, 0x3630, 0xf4);
    ret |= sc2337p_write_reg(dev, 0x3632, 0x44);
    ret |= sc2337p_write_reg(dev, 0x3633, 0x22);
    ret |= sc2337p_write_reg(dev, 0x3639, 0xf4);
    ret |= sc2337p_write_reg(dev, 0x363c, 0x47);
    ret |= sc2337p_write_reg(dev, 0x3670, 0x09);
    ret |= sc2337p_write_reg(dev, 0x3674, 0xf4);
    ret |= sc2337p_write_reg(dev, 0x3675, 0xfb);
    ret |= sc2337p_write_reg(dev, 0x3676, 0xed);
    ret |= sc2337p_write_reg(dev, 0x367c, 0x09);
    ret |= sc2337p_write_reg(dev, 0x367d, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x3690, 0x22);
    ret |= sc2337p_write_reg(dev, 0x3691, 0x22);
    ret |= sc2337p_write_reg(dev, 0x3692, 0x22);
    ret |= sc2337p_write_reg(dev, 0x3698, 0x89);
    ret |= sc2337p_write_reg(dev, 0x3699, 0x96);
    ret |= sc2337p_write_reg(dev, 0x369a, 0xd0);
    ret |= sc2337p_write_reg(dev, 0x369b, 0xd0);
    ret |= sc2337p_write_reg(dev, 0x369c, 0x09);
    ret |= sc2337p_write_reg(dev, 0x369d, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x36a2, 0x09);
    ret |= sc2337p_write_reg(dev, 0x36a3, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x36a4, 0x1f);
    ret |= sc2337p_write_reg(dev, 0x36d0, 0x01);
    ret |= sc2337p_write_reg(dev, 0x3722, 0xc1);
    ret |= sc2337p_write_reg(dev, 0x3724, 0x41);
    ret |= sc2337p_write_reg(dev, 0x3725, 0xc1);
    ret |= sc2337p_write_reg(dev, 0x3728, 0x20);
    ret |= sc2337p_write_reg(dev, 0x3900, 0x0d);
    ret |= sc2337p_write_reg(dev, 0x3905, 0x98);
    ret |= sc2337p_write_reg(dev, 0x3919, 0x04);
    ret |= sc2337p_write_reg(dev, 0x391b, 0x81);
    ret |= sc2337p_write_reg(dev, 0x391c, 0x10);
    ret |= sc2337p_write_reg(dev, 0x3933, 0x81);
    ret |= sc2337p_write_reg(dev, 0x3934, 0xd0);
    ret |= sc2337p_write_reg(dev, 0x3940, 0x75);
    ret |= sc2337p_write_reg(dev, 0x3941, 0x00);
    ret |= sc2337p_write_reg(dev, 0x3942, 0x01);
    ret |= sc2337p_write_reg(dev, 0x3943, 0xd1);
    ret |= sc2337p_write_reg(dev, 0x3952, 0x02);
    ret |= sc2337p_write_reg(dev, 0x3953, 0x0f);
    ret |= sc2337p_write_reg(dev, 0x3e01, 0x45);
    ret |= sc2337p_write_reg(dev, 0x3e02, 0xf0);
    ret |= sc2337p_write_reg(dev, 0x3e08, 0x1f);
    ret |= sc2337p_write_reg(dev, 0x3e1b, 0x14);
    ret |= sc2337p_write_reg(dev, 0x440e, 0x02);
    ret |= sc2337p_write_reg(dev, 0x4509, 0x38);
    ret |= sc2337p_write_reg(dev, 0x5799, 0x06);
    ret |= sc2337p_write_reg(dev, 0x5ae0, 0xfe);
    ret |= sc2337p_write_reg(dev, 0x5ae1, 0x40);
    ret |= sc2337p_write_reg(dev, 0x5ae2, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5ae3, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5ae4, 0x20);
    ret |= sc2337p_write_reg(dev, 0x5ae5, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5ae6, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5ae7, 0x20);
    ret |= sc2337p_write_reg(dev, 0x5ae8, 0x3c);
    ret |= sc2337p_write_reg(dev, 0x5ae9, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5aea, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5aeb, 0x3c);
    ret |= sc2337p_write_reg(dev, 0x5aec, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5aed, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5aee, 0xfe);
    ret |= sc2337p_write_reg(dev, 0x5aef, 0x40);
    ret |= sc2337p_write_reg(dev, 0x5af4, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5af5, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5af6, 0x20);
    ret |= sc2337p_write_reg(dev, 0x5af7, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5af8, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5af9, 0x20);
    ret |= sc2337p_write_reg(dev, 0x5afa, 0x3c);
    ret |= sc2337p_write_reg(dev, 0x5afb, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5afc, 0x28);
    ret |= sc2337p_write_reg(dev, 0x5afd, 0x3c);
    ret |= sc2337p_write_reg(dev, 0x5afe, 0x30);
    ret |= sc2337p_write_reg(dev, 0x5aff, 0x28);
    ret |= sc2337p_write_reg(dev, 0x36e9, 0x20);
    ret |= sc2337p_write_reg(dev, 0x37f9, 0x27);
    ret |= sc2337p_write_reg(dev, 0x0100, 0x01);
    sc2337p_delay_ms(10);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc2337p_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("========================================================================================\n");
        SENSOR_PRINT("========= SC2337P_27MInput_371.25Mbps_MIPI_2lane_10bit_1920x1080_30fps init success!====\n");
        SENSOR_PRINT("========================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 sc2337p_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC2337P_2M_1080P_10BIT_LINEAR_MODE:
            ret = sc2337p_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc2337p_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_sc2337p_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc2337p_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc2337p_i2c_fd[dev]);
        g_sc2337p_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc2337p_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc2337p_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc2337p_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc2337p_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc2337p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc2337p_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc2337p_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc2337p_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc2337p_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc2337p_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC2337P_DATA_BYTE];

    if (g_sc2337p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc2337p_i2c_fd[dev], g_sc2337p_i2c_addr[dev], addr, SC2337P_ADDR_BYTE, buf, SC2337P_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC2337P_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc2337p_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc2337p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc2337p_i2c_fd[dev], g_sc2337p_i2c_addr[dev], buf, SC2337P_ADDR_BYTE + SC2337P_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 sc2337p_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc2337p_read_reg(dev, SC2337P_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

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

    ret = sc2337p_write_reg(dev, SC2337P_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc2337p_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
