#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "cv8001.h"
#include "cv8001_ctrl.h"

#define CV8001_REG_ADDR_MIRROR_FLIP 0x3034

SENSOR_PRIORITY_DATA static xmedia_s32 g_cv8001_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_cv8001_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

SENSOR_PRIORITY_FUNC static void cv8001_delay_ms(int ms)
{
    sleep_us(ms * 1000);
}

//目前序列实际帧率只有20
SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_2lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= cv8001_write_reg(dev, 0x3401, 0x01);
    ret |= cv8001_write_reg(dev, 0x3915, 0x00);
    ret |= cv8001_write_reg(dev, 0x3220, 0x08);
    ret |= cv8001_write_reg(dev, 0x3185, 0x03);
    ret |= cv8001_write_reg(dev, 0x318A, 0x03);
    // W4K
    ret |= cv8001_write_reg(dev, 0x3020, 0x04);
    ret |= cv8001_write_reg(dev, 0x3040, 0x01);
    // H
    ret |= cv8001_write_reg(dev, 0x3048, 0x08);
    ret |= cv8001_write_reg(dev, 0x3049, 0x00);
    ret |= cv8001_write_reg(dev, 0x304A, 0x00);
    ret |= cv8001_write_reg(dev, 0x304B, 0x0F);
    // V
    ret |= cv8001_write_reg(dev, 0x3044, 0x04);
    ret |= cv8001_write_reg(dev, 0x3045, 0x00);
    ret |= cv8001_write_reg(dev, 0x3046, 0x70);
    ret |= cv8001_write_reg(dev, 0x3047, 0x08);
    //adjust1
    ret |= cv8001_write_reg(dev, 0x3041, 0x00);

    ret |= cv8001_write_reg(dev, 0x3b68, 0x00);
    ret |= cv8001_write_reg(dev, 0x3b69, 0x00);
    ret |= cv8001_write_reg(dev, 0x3b6a, 0xff);
    ret |= cv8001_write_reg(dev, 0x3b6b, 0x01);
    //adjust2
    ret |= cv8001_write_reg(dev, 0x387D, 0x3F);
    ret |= cv8001_write_reg(dev, 0x397a, 0x08);
    ret |= cv8001_write_reg(dev, 0x3804, 0x14);
    ret |= cv8001_write_reg(dev, 0x3587, 0x2a);
    ret |= cv8001_write_reg(dev, 0x3348, 0x00);
    ret |= cv8001_write_reg(dev, 0x31AC, 0xC8);
    ret |= cv8001_write_reg(dev, 0x35EC, 0x08);
    ret |= cv8001_write_reg(dev, 0x3220, 0x08);

    ret |= cv8001_write_reg(dev, 0x3b75, 0x00);
    ret |= cv8001_write_reg(dev, 0x3b5E, 0x01);
    ret |= cv8001_write_reg(dev, 0x3a10, 0x0A);
    ret |= cv8001_write_reg(dev, 0x3a11, 0x0A);
    //adjust
    ret |= cv8001_write_reg(dev, 0x3a92, 0x56);
    ret |= cv8001_write_reg(dev, 0x3914, 0x2E);
    ret |= cv8001_write_reg(dev, 0x3908, 0x4A);
    ret |= cv8001_write_reg(dev, 0x302C, 0x3B);
    ret |= cv8001_write_reg(dev, 0x302D, 0x01);
    ret |= cv8001_write_reg(dev, 0x316C, 0x02);
    ret |= cv8001_write_reg(dev, 0x3686, 0x03);
    ret |= cv8001_write_reg(dev, 0x36b8, 0x00);
    ret |= cv8001_write_reg(dev, 0x36b6, 0x00);
    //adjust3
    ret |= cv8001_write_reg(dev, 0x36C0, 0x00);
    ret |= cv8001_write_reg(dev, 0x36C5, 0x02);
    //adjust4
    ret |= cv8001_write_reg(dev, 0x36B6, 0x02);
    ret |= cv8001_write_reg(dev, 0x35DF, 0x00);
    ret |= cv8001_write_reg(dev, 0x35C4, 0x09);
    ret |= cv8001_write_reg(dev, 0x3685, 0x01);
    //adjust5
    ret |= cv8001_write_reg(dev, 0x3A86, 0x01);
    ret |= cv8001_write_reg(dev, 0x3A88, 0x07);
    ret |= cv8001_write_reg(dev, 0x3A8C, 0x3C);
    ret |= cv8001_write_reg(dev, 0x3B19, 0x0A);
    ret |= cv8001_write_reg(dev, 0x3B1C, 0x2B);
    ret |= cv8001_write_reg(dev, 0x3518, 0x6A);
    ret |= cv8001_write_reg(dev, 0x3519, 0x00);
    ret |= cv8001_write_reg(dev, 0x351A, 0x08);

    ret |= cv8001_write_reg(dev, 0x3852, 0x00);
    ret |= cv8001_write_reg(dev, 0x36AA, 0x00);
    ret |= cv8001_write_reg(dev, 0x3000, 0x00);

    cv8001_delay_ms(5);  // delay 5 ms
    ret |= cv8001_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv8001_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== CV8001_24MInput_MIPI_2lane_10bit_3840x2160_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case CV8001_8M_LINEAR_MODE:
            ret = cv8001_2lane_linear_3840x2160_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_cv8001_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_cv8001_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_cv8001_i2c_fd[dev]);
        g_cv8001_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_cv8001_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_cv8001_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_cv8001_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_cv8001_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = cv8001_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_cv8001_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_cv8001_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[CV8001_DATA_BYTE];

    if (g_cv8001_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_cv8001_i2c_fd[dev], g_cv8001_i2c_addr[dev], addr, CV8001_ADDR_BYTE, buf, CV8001_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //CV8001_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_cv8001_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_cv8001_i2c_fd[dev], g_cv8001_i2c_addr[dev], buf, CV8001_ADDR_BYTE + CV8001_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 mirror_flip_type;

    cv8001_read_reg(dev, CV8001_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en){
        mirror_flip_type |= 0x01;
    } else {
        mirror_flip_type &= 0xFE;
    }

    if (flip_en){
        mirror_flip_type |= 0x02;
    } else {
        mirror_flip_type &= 0xFD;
    }

    ret  = cv8001_write_reg(dev, CV8001_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv8001_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
