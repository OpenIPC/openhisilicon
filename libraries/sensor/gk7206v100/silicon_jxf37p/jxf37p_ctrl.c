#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "jxf37p.h"
#include "jxf37p_ctrl.h"

static xmedia_s32 g_jxf37p_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_jxf37p_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define JXF37P_REG_ADDR_MIRROR_FLIP 0x12

xmedia_void jxf37p_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 jxf37p_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= jxf37p_write_reg(dev, 0x12, 0x40);
    ret |= jxf37p_write_reg(dev, 0x48, 0x8A);
    ret |= jxf37p_write_reg(dev, 0x48, 0x0A);
    ret |= jxf37p_write_reg(dev, 0x0E, 0x19);
    ret |= jxf37p_write_reg(dev, 0x0F, 0x04);
    ret |= jxf37p_write_reg(dev, 0x10, 0x24);
    ret |= jxf37p_write_reg(dev, 0x11, 0x80);
    ret |= jxf37p_write_reg(dev, 0x46, 0x09);
    ret |= jxf37p_write_reg(dev, 0x47, 0x66);
    ret |= jxf37p_write_reg(dev, 0x0D, 0xF2);
    ret |= jxf37p_write_reg(dev, 0x57, 0x6A);
    ret |= jxf37p_write_reg(dev, 0x58, 0x22);
    ret |= jxf37p_write_reg(dev, 0x5F, 0x41);
    ret |= jxf37p_write_reg(dev, 0x60, 0x28);
    ret |= jxf37p_write_reg(dev, 0xA5, 0xC0);
    ret |= jxf37p_write_reg(dev, 0x20, 0x00);
    ret |= jxf37p_write_reg(dev, 0x21, 0x05);
    ret |= jxf37p_write_reg(dev, 0x22, 0x65);
    ret |= jxf37p_write_reg(dev, 0x23, 0x04);
    ret |= jxf37p_write_reg(dev, 0x24, 0xC0);
    ret |= jxf37p_write_reg(dev, 0x25, 0x40);
    ret |= jxf37p_write_reg(dev, 0x26, 0x43);
    ret |= jxf37p_write_reg(dev, 0x27, 0xC6);
    ret |= jxf37p_write_reg(dev, 0x28, 0x11);
    ret |= jxf37p_write_reg(dev, 0x29, 0x04);
    ret |= jxf37p_write_reg(dev, 0x2A, 0xBB);
    ret |= jxf37p_write_reg(dev, 0x2B, 0x14);
    ret |= jxf37p_write_reg(dev, 0x2C, 0x02);
    ret |= jxf37p_write_reg(dev, 0x2D, 0x00);
    ret |= jxf37p_write_reg(dev, 0x2E, 0x16);
    ret |= jxf37p_write_reg(dev, 0x2F, 0x04);
    ret |= jxf37p_write_reg(dev, 0x41, 0xC5);
    ret |= jxf37p_write_reg(dev, 0x42, 0x33);
    ret |= jxf37p_write_reg(dev, 0x47, 0x46);
    ret |= jxf37p_write_reg(dev, 0x76, 0x60);
    ret |= jxf37p_write_reg(dev, 0x77, 0x09);
    ret |= jxf37p_write_reg(dev, 0x80, 0x01);
    ret |= jxf37p_write_reg(dev, 0xAF, 0x22);
    ret |= jxf37p_write_reg(dev, 0xAB, 0x00);
    ret |= jxf37p_write_reg(dev, 0x1D, 0x00);
    ret |= jxf37p_write_reg(dev, 0x1E, 0x04);
    ret |= jxf37p_write_reg(dev, 0x6C, 0x40);
    ret |= jxf37p_write_reg(dev, 0x9E, 0xF8);
    ret |= jxf37p_write_reg(dev, 0x6E, 0x2C);
    ret |= jxf37p_write_reg(dev, 0x70, 0x6C);
    ret |= jxf37p_write_reg(dev, 0x71, 0x6D);
    ret |= jxf37p_write_reg(dev, 0x72, 0x6A);
    ret |= jxf37p_write_reg(dev, 0x73, 0x56);
    ret |= jxf37p_write_reg(dev, 0x74, 0x02);
    ret |= jxf37p_write_reg(dev, 0x78, 0x9D);
    ret |= jxf37p_write_reg(dev, 0x89, 0x01);
    ret |= jxf37p_write_reg(dev, 0x6B, 0x20);
    ret |= jxf37p_write_reg(dev, 0x86, 0x40);
    ret |= jxf37p_write_reg(dev, 0x31, 0x10);
    ret |= jxf37p_write_reg(dev, 0x32, 0x18);
    ret |= jxf37p_write_reg(dev, 0x33, 0xE8);
    ret |= jxf37p_write_reg(dev, 0x34, 0x5E);
    ret |= jxf37p_write_reg(dev, 0x35, 0x5E);
    ret |= jxf37p_write_reg(dev, 0x3A, 0xAF);
    ret |= jxf37p_write_reg(dev, 0x3B, 0x00);
    ret |= jxf37p_write_reg(dev, 0x3C, 0xFF);
    ret |= jxf37p_write_reg(dev, 0x3D, 0xFF);
    ret |= jxf37p_write_reg(dev, 0x3E, 0xFF);
    ret |= jxf37p_write_reg(dev, 0x3F, 0xBB);
    ret |= jxf37p_write_reg(dev, 0x40, 0xFF);
    ret |= jxf37p_write_reg(dev, 0x56, 0x92);
    ret |= jxf37p_write_reg(dev, 0x59, 0xAF);
    ret |= jxf37p_write_reg(dev, 0x5A, 0x47);
    ret |= jxf37p_write_reg(dev, 0x61, 0x18);
    ret |= jxf37p_write_reg(dev, 0x6F, 0x04);
    ret |= jxf37p_write_reg(dev, 0x85, 0x5F);
    ret |= jxf37p_write_reg(dev, 0x8A, 0x44);
    ret |= jxf37p_write_reg(dev, 0x91, 0x13);
    ret |= jxf37p_write_reg(dev, 0x94, 0xA0);
    ret |= jxf37p_write_reg(dev, 0x9B, 0x83);
    ret |= jxf37p_write_reg(dev, 0x9C, 0xE1);
    ret |= jxf37p_write_reg(dev, 0xA4, 0x80);
    ret |= jxf37p_write_reg(dev, 0xA6, 0x22);
    ret |= jxf37p_write_reg(dev, 0xA9, 0x1C);
    ret |= jxf37p_write_reg(dev, 0x5B, 0xE7);
    ret |= jxf37p_write_reg(dev, 0x5C, 0x28);
    ret |= jxf37p_write_reg(dev, 0x5D, 0x67);
    ret |= jxf37p_write_reg(dev, 0x5E, 0x11);
    ret |= jxf37p_write_reg(dev, 0x62, 0x21);
    ret |= jxf37p_write_reg(dev, 0x63, 0x0F);
    ret |= jxf37p_write_reg(dev, 0x64, 0xD0);
    ret |= jxf37p_write_reg(dev, 0x65, 0x02);
    ret |= jxf37p_write_reg(dev, 0x67, 0x49);
    ret |= jxf37p_write_reg(dev, 0x66, 0x00);
    ret |= jxf37p_write_reg(dev, 0x68, 0x00);
    ret |= jxf37p_write_reg(dev, 0x69, 0x72);
    ret |= jxf37p_write_reg(dev, 0x6A, 0x12);
    ret |= jxf37p_write_reg(dev, 0x7A, 0x00);
    ret |= jxf37p_write_reg(dev, 0x82, 0x20);
    ret |= jxf37p_write_reg(dev, 0x8D, 0x47);
    ret |= jxf37p_write_reg(dev, 0x8F, 0x90);
    ret |= jxf37p_write_reg(dev, 0x45, 0x01);
    ret |= jxf37p_write_reg(dev, 0x97, 0x20);
    ret |= jxf37p_write_reg(dev, 0x13, 0x81);
    ret |= jxf37p_write_reg(dev, 0x96, 0x84);
    ret |= jxf37p_write_reg(dev, 0x4A, 0x01);
    ret |= jxf37p_write_reg(dev, 0xB1, 0x00);
    ret |= jxf37p_write_reg(dev, 0xA1, 0x0F);
    ret |= jxf37p_write_reg(dev, 0xBE, 0x00);
    ret |= jxf37p_write_reg(dev, 0x7E, 0x48);
    ret |= jxf37p_write_reg(dev, 0xB5, 0xC0);
    ret |= jxf37p_write_reg(dev, 0x50, 0x02);
    ret |= jxf37p_write_reg(dev, 0x49, 0x10);
    ret |= jxf37p_write_reg(dev, 0x7F, 0x57);
    ret |= jxf37p_write_reg(dev, 0x90, 0x00);
    ret |= jxf37p_write_reg(dev, 0x7B, 0x4A);
    ret |= jxf37p_write_reg(dev, 0x7C, 0x0C);
    ret |= jxf37p_write_reg(dev, 0x8C, 0xFF);
    ret |= jxf37p_write_reg(dev, 0x8E, 0x00);
    ret |= jxf37p_write_reg(dev, 0x8B, 0x01);
    ret |= jxf37p_write_reg(dev, 0x0C, 0x00);
    ret |= jxf37p_write_reg(dev, 0xBC, 0x11);
    ret |= jxf37p_write_reg(dev, 0x19, 0x20);
    ret |= jxf37p_write_reg(dev, 0x1B, 0x4F);
    ret |= jxf37p_write_reg(dev, 0x12, 0x00);
    ret |= jxf37p_write_reg(dev, 0x00, 0x10);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "jxf37p_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=============================================================================================\n");
        SENSOR_PRINT("==== jxf37p_normal_mipi_24Minput_2lane_432Mbps_10bit_linear_1920x1080_30fps init success!====\n");
        SENSOR_PRINT("=============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 jxf37p_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (image_mode) {
        case JXF37P_2LANE_1920X1080_LINEAR_MODE:
            ret = jxf37p_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 jxf37p_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_jxf37p_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_jxf37p_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_jxf37p_i2c_fd[dev]);
        g_jxf37p_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_jxf37p_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 jxf37p_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_jxf37p_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_jxf37p_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_jxf37p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = jxf37p_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 jxf37p_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_jxf37p_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_jxf37p_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 jxf37p_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[JXF37P_DATA_BYTE];

    if (g_jxf37p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_jxf37p_i2c_fd[dev], g_jxf37p_i2c_addr[dev], addr, JXF37P_ADDR_BYTE, buf, JXF37P_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //JXF37P_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 jxf37p_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_jxf37p_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_jxf37p_i2c_fd[dev], g_jxf37p_i2c_addr[dev], buf, JXF37P_ADDR_BYTE + JXF37P_DATA_BYTE);
    if (ret != (JXF37P_ADDR_BYTE + JXF37P_DATA_BYTE)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!ret=%d.\n", ret);
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 jxf37p_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    jxf37p_read_reg(dev, JXF37P_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x20;
    } else {
        mirror_flip_type &= 0xdf;
    }

    if (flip_en) {
        mirror_flip_type |= 0x10;
    } else {
        mirror_flip_type &= 0xef;
    }

    ret = jxf37p_write_reg(dev, JXF37P_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "jxf37p_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
