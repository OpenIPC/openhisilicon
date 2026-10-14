#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "mis40h1.h"
#include "mis40h1_ctrl.h"

static xmedia_s32 g_mis40h1_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_mis40h1_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define MIS40H1_REG_ADDR_MIRROR_FLIP 0x3007

/*
 * 函数功能: mis40h1 MIPI 2lane 1080p线性模式初始化序列 - 2568x1520x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
 SENSOR_PRIORITY_FUNC static void mis40h1_delay_ms(int ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 mis40h1_2lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= mis40h1_write_reg(dev, 0x3006, 0x01);
    mis40h1_delay_ms(1);
    ret |= mis40h1_write_reg(dev, 0x3006, 0x02);
    ret |= mis40h1_write_reg(dev, 0x3304, 0xc6);
    ret |= mis40h1_write_reg(dev, 0x3305, 0xca);
    ret |= mis40h1_write_reg(dev, 0x3023, 0x2b);
    ret |= mis40h1_write_reg(dev, 0x3113, 0x04);
    ret |= mis40h1_write_reg(dev, 0x3115, 0xf3);
    ret |= mis40h1_write_reg(dev, 0x3117, 0x05);
    ret |= mis40h1_write_reg(dev, 0x3119, 0x84);
    ret |= mis40h1_write_reg(dev, 0x3120, 0x08);
    ret |= mis40h1_write_reg(dev, 0x3123, 0x12);
    ret |= mis40h1_write_reg(dev, 0x3306, 0x25);
    ret |= mis40h1_write_reg(dev, 0x3605, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x360d, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x3615, 0x58);
    ret |= mis40h1_write_reg(dev, 0x361d, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x3621, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x3629, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x362d, 0xb9);
    ret |= mis40h1_write_reg(dev, 0x363d, 0xF7);
    ret |= mis40h1_write_reg(dev, 0x363f, 0x7d);
    ret |= mis40h1_write_reg(dev, 0x3641, 0xB0);
    ret |= mis40h1_write_reg(dev, 0x365d, 0x08);
    ret |= mis40h1_write_reg(dev, 0x365f, 0xb9);
    ret |= mis40h1_write_reg(dev, 0x3661, 0xa2);
    ret |= mis40h1_write_reg(dev, 0x367f, 0xad);
    ret |= mis40h1_write_reg(dev, 0x3681, 0xa0);
    ret |= mis40h1_write_reg(dev, 0x368b, 0xad);
    ret |= mis40h1_write_reg(dev, 0x368d, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x3692, 0x03);
    ret |= mis40h1_write_reg(dev, 0x3693, 0x39);
    ret |= mis40h1_write_reg(dev, 0x3695, 0xbf);
    ret |= mis40h1_write_reg(dev, 0x36b9, 0xb9);
    ret |= mis40h1_write_reg(dev, 0x3703, 0xbf);
    ret |= mis40h1_write_reg(dev, 0x3705, 0xc5);
    ret |= mis40h1_write_reg(dev, 0x3713, 0xdd);
    ret |= mis40h1_write_reg(dev, 0x371e, 0x66);
    ret |= mis40h1_write_reg(dev, 0x3720, 0x95);
    ret |= mis40h1_write_reg(dev, 0x3724, 0x72);
    ret |= mis40h1_write_reg(dev, 0x3728, 0x89);
    ret |= mis40h1_write_reg(dev, 0x3730, 0x42);
    ret |= mis40h1_write_reg(dev, 0x3740, 0x89);
    ret |= mis40h1_write_reg(dev, 0x375c, 0xA8);
    ret |= mis40h1_write_reg(dev, 0x3c03, 0x03);
    ret |= mis40h1_write_reg(dev, 0x3c39, 0x03);
    ret |= mis40h1_write_reg(dev, 0x4102, 0x13);
    ret |= mis40h1_write_reg(dev, 0x410e, 0x02);
    ret |= mis40h1_write_reg(dev, 0x4303, 0x13);
    ret |= mis40h1_write_reg(dev, 0x432e, 0x00);
    ret |= mis40h1_write_reg(dev, 0x432f, 0x80);
    ret |= mis40h1_write_reg(dev, 0x4330, 0x00);
    ret |= mis40h1_write_reg(dev, 0x4331, 0x80);
    ret |= mis40h1_write_reg(dev, 0x4332, 0x00);
    ret |= mis40h1_write_reg(dev, 0x4333, 0x80);
    ret |= mis40h1_write_reg(dev, 0x4334, 0x00);
    ret |= mis40h1_write_reg(dev, 0x4335, 0x80);
    ret |= mis40h1_write_reg(dev, 0x4402, 0x56);
    ret |= mis40h1_write_reg(dev, 0x390c, 0x1b);
    ret |= mis40h1_write_reg(dev, 0x390d, 0x1b);
    ret |= mis40h1_write_reg(dev, 0x390e, 0x1b);
    ret |= mis40h1_write_reg(dev, 0x390f, 0x1b);
    ret |= mis40h1_write_reg(dev, 0x3910, 0x1b);
    ret |= mis40h1_write_reg(dev, 0x3911, 0x1b);
    ret |= mis40h1_write_reg(dev, 0x3a0e, 0x0f);
    ret |= mis40h1_write_reg(dev, 0x4102, 0x13);
    ret |= mis40h1_write_reg(dev, 0x410e, 0x02);
    ret |= mis40h1_write_reg(dev, 0x410f, 0x39);
    ret |= mis40h1_write_reg(dev, 0x4303, 0x13);
    ret |= mis40h1_write_reg(dev, 0x4304, 0x39);
    ret |= mis40h1_write_reg(dev, 0x4309, 0x01);
    ret |= mis40h1_write_reg(dev, 0x367C, 0x01);
    ret |= mis40h1_write_reg(dev, 0x367D, 0xE8);
    ret |= mis40h1_write_reg(dev, 0x3a1e, 0x7f);
    ret |= mis40h1_write_reg(dev, 0x3a1f, 0x00);
    ret |= mis40h1_write_reg(dev, 0x3a20, 0xff);
    ret |= mis40h1_write_reg(dev, 0x3915, 0x00);
    ret |= mis40h1_write_reg(dev, 0x3913, 0x1c);
    ret |= mis40h1_write_reg(dev, 0x3914, 0x1c);
    ret |= mis40h1_write_reg(dev, 0x3031, 0x0c);
    ret |= mis40h1_write_reg(dev, 0x3008, 0x01);
    ret |= mis40h1_write_reg(dev, 0x3006, 0x00);
    ret |= mis40h1_write_reg(dev, 0x3c1a, 0x01);
    mis40h1_delay_ms(1);
    ret |= mis40h1_write_reg(dev, 0x300c, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "mis40h1_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("==========================================================================================\n");
        SENSOR_PRINT("========= MIS40H1_24MInput_MIPI_2lane_10bit_792M/bps_2688x1520_30fps init success!========\n");
        SENSOR_PRINT("==========================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 mis40h1_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case MIS40H1_4M_1520_10BIT_LINEAR_MODE:
            ret = mis40h1_2lane_linear_2688x1520_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis40h1_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_mis40h1_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_mis40h1_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_mis40h1_i2c_fd[dev]);
        g_mis40h1_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_mis40h1_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis40h1_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_mis40h1_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_mis40h1_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_mis40h1_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = mis40h1_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis40h1_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_mis40h1_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_mis40h1_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis40h1_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[MIS40H1_DATA_BYTE];

    if (g_mis40h1_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_mis40h1_i2c_fd[dev], g_mis40h1_i2c_addr[dev], addr, MIS40H1_ADDR_BYTE, buf, MIS40H1_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //MIS40H1_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis40h1_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_mis40h1_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_mis40h1_i2c_fd[dev], g_mis40h1_i2c_addr[dev], buf, MIS40H1_ADDR_BYTE + MIS40H1_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 mis40h1_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;

    if (mirror_en == XMEDIA_FALSE && flip_en == XMEDIA_FALSE) {
        ret = mis40h1_write_reg(dev, MIS40H1_REG_ADDR_MIRROR_FLIP, 0x0);
    } else if (mirror_en == XMEDIA_TRUE && flip_en == XMEDIA_FALSE) {
        ret = mis40h1_write_reg(dev, MIS40H1_REG_ADDR_MIRROR_FLIP, 0x1);
    } else if (mirror_en == XMEDIA_FALSE && flip_en == XMEDIA_TRUE) {
        ret = mis40h1_write_reg(dev, MIS40H1_REG_ADDR_MIRROR_FLIP, 0x2);
    } else {
        ret = mis40h1_write_reg(dev, MIS40H1_REG_ADDR_MIRROR_FLIP, 0x3);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "mis40h1_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
