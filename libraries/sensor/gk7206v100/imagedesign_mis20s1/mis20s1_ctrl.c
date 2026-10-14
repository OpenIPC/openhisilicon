#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "mis20s1.h"
#include "mis20s1_ctrl.h"

static xmedia_s32 g_mis20s1_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_mis20s1_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define MIS20S1_REG_ADDR_MIRROR_FLIP 0x3007

/*
 * 函数功能: mis20s1 MIPI 2lane 1080p线性模式初始化序列 - 1920x1080x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
 SENSOR_PRIORITY_FUNC static void mis20s1_delay_ms(int ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 mis20s1_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= mis20s1_write_reg(dev, 0x3006, 0x01);
    mis20s1_delay_ms(1);
    ret |= mis20s1_write_reg(dev, 0x3006, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3018, 0x50);
    ret |= mis20s1_write_reg(dev, 0x3023, 0x2b);
    ret |= mis20s1_write_reg(dev, 0x3113, 0x04);
    ret |= mis20s1_write_reg(dev, 0x3115, 0x3b);
    ret |= mis20s1_write_reg(dev, 0x3117, 0x04);
    ret |= mis20s1_write_reg(dev, 0x3119, 0x83);
    ret |= mis20s1_write_reg(dev, 0x311a, 0x01);
    ret |= mis20s1_write_reg(dev, 0x311b, 0xd6);
    ret |= mis20s1_write_reg(dev, 0x311c, 0x00);
    ret |= mis20s1_write_reg(dev, 0x311d, 0x19);
    ret |= mis20s1_write_reg(dev, 0x311e, 0x00);
    ret |= mis20s1_write_reg(dev, 0x311f, 0x00);
    ret |= mis20s1_write_reg(dev, 0x3120, 0xf8);
    ret |= mis20s1_write_reg(dev, 0x3121, 0x01);
    ret |= mis20s1_write_reg(dev, 0x3122, 0x2c);
    ret |= mis20s1_write_reg(dev, 0x3123, 0x12);
    ret |= mis20s1_write_reg(dev, 0x3205, 0xb2);
    ret |= mis20s1_write_reg(dev, 0x3304, 0x6e);
    ret |= mis20s1_write_reg(dev, 0x3305, 0xa9);
    ret |= mis20s1_write_reg(dev, 0x3306, 0x2f);
    ret |= mis20s1_write_reg(dev, 0x3307, 0x04);
    ret |= mis20s1_write_reg(dev, 0x3308, 0xb8);
    ret |= mis20s1_write_reg(dev, 0x3309, 0xb1);
    ret |= mis20s1_write_reg(dev, 0x330a, 0x07);
    ret |= mis20s1_write_reg(dev, 0x330b, 0x06);
    ret |= mis20s1_write_reg(dev, 0x3314, 0x25);
    ret |= mis20s1_write_reg(dev, 0x3502, 0x0d);
    ret |= mis20s1_write_reg(dev, 0x3604, 0x05);
    ret |= mis20s1_write_reg(dev, 0x360c, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3612, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3613, 0x40);
    ret |= mis20s1_write_reg(dev, 0x3615, 0x90);
    ret |= mis20s1_write_reg(dev, 0x361c, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3620, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3624, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3625, 0x4f);
    ret |= mis20s1_write_reg(dev, 0x3628, 0x05);
    ret |= mis20s1_write_reg(dev, 0x362c, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3630, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3631, 0x4f);
    ret |= mis20s1_write_reg(dev, 0x363d, 0x30);
    ret |= mis20s1_write_reg(dev, 0x363f, 0x40);
    ret |= mis20s1_write_reg(dev, 0x3641, 0xf0);
    ret |= mis20s1_write_reg(dev, 0x365c, 0x02);
    ret |= mis20s1_write_reg(dev, 0x365d, 0x40);
    ret |= mis20s1_write_reg(dev, 0x365e, 0x03);
    ret |= mis20s1_write_reg(dev, 0x365f, 0x00);
    ret |= mis20s1_write_reg(dev, 0x3660, 0x05);
    ret |= mis20s1_write_reg(dev, 0x367c, 0x02);
    ret |= mis20s1_write_reg(dev, 0x367d, 0x3c);
    ret |= mis20s1_write_reg(dev, 0x367f, 0xd5);
    ret |= mis20s1_write_reg(dev, 0x3680, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3681, 0xec);
    ret |= mis20s1_write_reg(dev, 0x3688, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3689, 0x4f);
    ret |= mis20s1_write_reg(dev, 0x368b, 0xd5);
    ret |= mis20s1_write_reg(dev, 0x368c, 0x05);
    ret |= mis20s1_write_reg(dev, 0x368f, 0xc0);
    ret |= mis20s1_write_reg(dev, 0x3690, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3691, 0x51);
    ret |= mis20s1_write_reg(dev, 0x3692, 0x03);
    ret |= mis20s1_write_reg(dev, 0x3693, 0x64);
    ret |= mis20s1_write_reg(dev, 0x3694, 0x05);
    ret |= mis20s1_write_reg(dev, 0x36b3, 0x00);
    ret |= mis20s1_write_reg(dev, 0x36b5, 0x02);
    ret |= mis20s1_write_reg(dev, 0x36b6, 0x40);
    ret |= mis20s1_write_reg(dev, 0x36b7, 0x02);
    ret |= mis20s1_write_reg(dev, 0x36b8, 0xf0);
    ret |= mis20s1_write_reg(dev, 0x36b9, 0xe4);
    ret |= mis20s1_write_reg(dev, 0x36c2, 0x02);
    ret |= mis20s1_write_reg(dev, 0x36c3, 0x4f);
    ret |= mis20s1_write_reg(dev, 0x36c4, 0x02);
    ret |= mis20s1_write_reg(dev, 0x36c5, 0x5e);
    ret |= mis20s1_write_reg(dev, 0x3702, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3704, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3712, 0x06);
    ret |= mis20s1_write_reg(dev, 0x371a, 0x00);
    ret |= mis20s1_write_reg(dev, 0x371c, 0x00);
    ret |= mis20s1_write_reg(dev, 0x371d, 0x02);
    ret |= mis20s1_write_reg(dev, 0x371e, 0x6d);
    ret |= mis20s1_write_reg(dev, 0x3720, 0xa8);
    ret |= mis20s1_write_reg(dev, 0x3723, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3724, 0x7c);
    ret |= mis20s1_write_reg(dev, 0x3725, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3726, 0x3e);
    ret |= mis20s1_write_reg(dev, 0x3728, 0x9a);
    ret |= mis20s1_write_reg(dev, 0x3729, 0x02);
    ret |= mis20s1_write_reg(dev, 0x372a, 0x3b);
    ret |= mis20s1_write_reg(dev, 0x372b, 0x02);
    ret |= mis20s1_write_reg(dev, 0x372c, 0x3f);
    ret |= mis20s1_write_reg(dev, 0x372f, 0x05);
    ret |= mis20s1_write_reg(dev, 0x3741, 0x02);
    ret |= mis20s1_write_reg(dev, 0x3742, 0x3e);
    ret |= mis20s1_write_reg(dev, 0x3744, 0x9a);
    ret |= mis20s1_write_reg(dev, 0x375d, 0x02);
    ret |= mis20s1_write_reg(dev, 0x375e, 0x4f);
    ret |= mis20s1_write_reg(dev, 0x3760, 0xa0);
    ret |= mis20s1_write_reg(dev, 0x3902, 0x3f);
    ret |= mis20s1_write_reg(dev, 0x3903, 0x1b);
    ret |= mis20s1_write_reg(dev, 0x3905, 0x0f);
    ret |= mis20s1_write_reg(dev, 0x390A, 0x10);
    ret |= mis20s1_write_reg(dev, 0x390B, 0x1f);
    ret |= mis20s1_write_reg(dev, 0x390c, 0x09);
    ret |= mis20s1_write_reg(dev, 0x390d, 0x09);
    ret |= mis20s1_write_reg(dev, 0x390e, 0x09);
    ret |= mis20s1_write_reg(dev, 0x390f, 0x09);
    ret |= mis20s1_write_reg(dev, 0x3910, 0x09);
    ret |= mis20s1_write_reg(dev, 0x3911, 0x09);
    ret |= mis20s1_write_reg(dev, 0x3913, 0x1c);
    ret |= mis20s1_write_reg(dev, 0x3a05, 0x9d);
    ret |= mis20s1_write_reg(dev, 0x3a08, 0x37);
    ret |= mis20s1_write_reg(dev, 0x3a0d, 0x0f);
    ret |= mis20s1_write_reg(dev, 0x3a1b, 0x61);
    ret |= mis20s1_write_reg(dev, 0x3a1c, 0x00);
    ret |= mis20s1_write_reg(dev, 0x3a1d, 0x7f);
    ret |= mis20s1_write_reg(dev, 0x3a1e, 0xdf);
    ret |= mis20s1_write_reg(dev, 0x3a21, 0x0e);
    ret |= mis20s1_write_reg(dev, 0x3b05, 0x3f);
    ret |= mis20s1_write_reg(dev, 0x3b07, 0x01);
    ret |= mis20s1_write_reg(dev, 0x3c03, 0x0a);
    ret |= mis20s1_write_reg(dev, 0x3c1a, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4102, 0x13);
    ret |= mis20s1_write_reg(dev, 0x410e, 0x02);
    ret |= mis20s1_write_reg(dev, 0x410f, 0x39);
    ret |= mis20s1_write_reg(dev, 0x4303, 0x13);
    ret |= mis20s1_write_reg(dev, 0x4304, 0x39);
    ret |= mis20s1_write_reg(dev, 0x4309, 0x00);
    ret |= mis20s1_write_reg(dev, 0x432E, 0x00);
    ret |= mis20s1_write_reg(dev, 0x432F, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4330, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4331, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4332, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4333, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4334, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4335, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4336, 0x13);
    ret |= mis20s1_write_reg(dev, 0x4361, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4362, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4363, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4364, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4365, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4366, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4367, 0x00);
    ret |= mis20s1_write_reg(dev, 0x4368, 0x80);
    ret |= mis20s1_write_reg(dev, 0x4402, 0x3f);
    ret |= mis20s1_write_reg(dev, 0x4403, 0x12);
    ret |= mis20s1_write_reg(dev, 0x3c1a, 0x01);
    ret |= mis20s1_write_reg(dev, 0x3031, 0x0c);
    ret |= mis20s1_write_reg(dev, 0x3008, 0x01);
    ret |= mis20s1_write_reg(dev, 0x3006, 0x00);
    mis20s1_delay_ms(1);
    ret |= mis20s1_write_reg(dev, 0x300c, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "mis20s1_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("==========================================================================================\n");
        SENSOR_PRINT("========= MIS20S1_27MInput_MIPI_2lane_10bit_372M/bps_1920x1080_30fps init success!========\n");
        SENSOR_PRINT("==========================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 mis20s1_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case MIS20S1_2M_1080_10BIT_LINEAR_MODE:
            ret = mis20s1_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis20s1_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_mis20s1_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_mis20s1_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_mis20s1_i2c_fd[dev]);
        g_mis20s1_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_mis20s1_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis20s1_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_mis20s1_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_mis20s1_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_mis20s1_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = mis20s1_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis20s1_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_mis20s1_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_mis20s1_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis20s1_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[MIS20S1_DATA_BYTE];

    if (g_mis20s1_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_mis20s1_i2c_fd[dev], g_mis20s1_i2c_addr[dev], addr, MIS20S1_ADDR_BYTE, buf, MIS20S1_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //MIS20S1_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 mis20s1_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_mis20s1_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_mis20s1_i2c_fd[dev], g_mis20s1_i2c_addr[dev], buf, MIS20S1_ADDR_BYTE + MIS20S1_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 mis20s1_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;

    if (mirror_en == XMEDIA_FALSE && flip_en == XMEDIA_FALSE) {
        ret = mis20s1_write_reg(dev, MIS20S1_REG_ADDR_MIRROR_FLIP, 0x0);
    } else if (mirror_en == XMEDIA_TRUE && flip_en == XMEDIA_FALSE) {
        ret = mis20s1_write_reg(dev, MIS20S1_REG_ADDR_MIRROR_FLIP, 0x1);
    } else if (mirror_en == XMEDIA_FALSE && flip_en == XMEDIA_TRUE) {
        ret = mis20s1_write_reg(dev, MIS20S1_REG_ADDR_MIRROR_FLIP, 0x2);
    } else {
        ret = mis20s1_write_reg(dev, MIS20S1_REG_ADDR_MIRROR_FLIP, 0x3);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "mis20s1_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
