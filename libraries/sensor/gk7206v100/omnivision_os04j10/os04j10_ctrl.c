#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os04j10.h"
#include "os04j10_ctrl.h"

SENSOR_PRIORITY_DATA static xmedia_s32 g_os04j10_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_os04j10_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

SENSOR_PRIORITY_FUNC void delay_ms(int ms)
{
    sleep_us(ms * 1000);
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os04j10_4lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    xmedia_u32 dcg_ratio = 0;

    ret |= os04j10_write_reg(dev, 0xfc, 0x01);
    delay_ms(8);
    ret |= os04j10_write_reg(dev, 0xfd, 0x00);
    ret |= os04j10_write_reg(dev, 0x09, 0x06);
    ret |= os04j10_write_reg(dev, 0x09, 0x00);
    ret |= os04j10_write_reg(dev, 0x14, 0xde);
    ret |= os04j10_write_reg(dev, 0x23, 0x74);
    ret |= os04j10_write_reg(dev, 0x24, 0x45);
    ret |= os04j10_write_reg(dev, 0x25, 0x21);
    ret |= os04j10_write_reg(dev, 0x30, 0x0a);
    ret |= os04j10_write_reg(dev, 0x31, 0x08);
    ret |= os04j10_write_reg(dev, 0x32, 0x05);
    ret |= os04j10_write_reg(dev, 0x33, 0xa8);
    ret |= os04j10_write_reg(dev, 0x3d, 0x03);
    ret |= os04j10_write_reg(dev, 0x39, 0x2c);
    ret |= os04j10_write_reg(dev, 0x47, 0x00);
    ret |= os04j10_write_reg(dev, 0x53, 0x00);
    ret |= os04j10_write_reg(dev, 0x58, 0xd0);
    ret |= os04j10_write_reg(dev, 0x5a, 0x0d);
    ret |= os04j10_write_reg(dev, 0x6b, 0x01);
    ret |= os04j10_write_reg(dev, 0x60, 0x04);
    ret |= os04j10_write_reg(dev, 0x61, 0xa8);
    ret |= os04j10_write_reg(dev, 0x62, 0xbc);
    ret |= os04j10_write_reg(dev, 0x63, 0xf8);
    ret |= os04j10_write_reg(dev, 0x64, 0x07);
    ret |= os04j10_write_reg(dev, 0x65, 0xae);
    ret |= os04j10_write_reg(dev, 0x0a, 0x23);
    ret |= os04j10_write_reg(dev, 0x0b, 0xf4);
    ret |= os04j10_write_reg(dev, 0xa1, 0x16);
    ret |= os04j10_write_reg(dev, 0xa2, 0xca);
    ret |= os04j10_write_reg(dev, 0xa4, 0x3f);
    ret |= os04j10_write_reg(dev, 0xa8, 0x48);
    ret |= os04j10_write_reg(dev, 0xaa, 0x0b);
    ret |= os04j10_write_reg(dev, 0xb2, 0x01);
    ret |= os04j10_write_reg(dev, 0xb4, 0x00);
    ret |= os04j10_write_reg(dev, 0xb5, 0xdf);
    ret |= os04j10_write_reg(dev, 0xb8, 0xdd);
    ret |= os04j10_write_reg(dev, 0xb9, 0x0f);

    ret |= os04j10_write_reg(dev, 0xab, 0x07);
    ret |= os04j10_write_reg(dev, 0xac, 0x06);
    ret |= os04j10_write_reg(dev, 0xad, 0x0f);
    ret |= os04j10_write_reg(dev, 0xaf, 0xd9);
    ret |= os04j10_write_reg(dev, 0xb0, 0xcb);
    ret |= os04j10_write_reg(dev, 0xb1, 0x00);
    ret |= os04j10_write_reg(dev, 0xbf, 0x2e);
    ret |= os04j10_write_reg(dev, 0xc0, 0x5d);
    ret |= os04j10_write_reg(dev, 0xc1, 0xc1);
    ret |= os04j10_write_reg(dev, 0xc2, 0xc0);
    ret |= os04j10_write_reg(dev, 0xc3, 0x04);
    ret |= os04j10_write_reg(dev, 0xc4, 0x0a);
    ret |= os04j10_write_reg(dev, 0xc5, 0x1a);
    ret |= os04j10_write_reg(dev, 0xc6, 0x26);
    ret |= os04j10_write_reg(dev, 0xd8, 0x20);
    ret |= os04j10_write_reg(dev, 0xd9, 0x00);

    ret |= os04j10_write_reg(dev, 0xc7, 0x40);
    ret |= os04j10_write_reg(dev, 0xc8, 0xf8);
    ret |= os04j10_write_reg(dev, 0xc9, 0x00);
    ret |= os04j10_write_reg(dev, 0xca, 0xb0);
    ret |= os04j10_write_reg(dev, 0xcb, 0x77);
    ret |= os04j10_write_reg(dev, 0xcc, 0x67);
    ret |= os04j10_write_reg(dev, 0xcd, 0x44);
    ret |= os04j10_write_reg(dev, 0xce, 0x34);
    ret |= os04j10_write_reg(dev, 0xcf, 0xa9);
    ret |= os04j10_write_reg(dev, 0xd0, 0xea);
    ret |= os04j10_write_reg(dev, 0xd2, 0xda);
    ret |= os04j10_write_reg(dev, 0xe0, 0xff);
    ret |= os04j10_write_reg(dev, 0xe6, 0x00);
    ret |= os04j10_write_reg(dev, 0xeb, 0x55);
    ret |= os04j10_write_reg(dev, 0xec, 0x55);
    ret |= os04j10_write_reg(dev, 0xed, 0x0a);

    ret |= os04j10_write_reg(dev, 0xfd, 0x01);
    ret |= os04j10_write_reg(dev, 0x0e, 0x02);//exp 8msb
    ret |= os04j10_write_reg(dev, 0x0f, 0xe4);//exp 8lsb
    ret |= os04j10_write_reg(dev, 0x15, 0x08);//vb
    ret |= os04j10_write_reg(dev, 0x19, 0x00);//linear mode
    ret |= os04j10_write_reg(dev, 0x1f, 0x00);//dig gain 3msb
    ret |= os04j10_write_reg(dev, 0x20, 0x40);//dig gain 8lsb
    ret |= os04j10_write_reg(dev, 0x22, 0x01);//rpc en, rpc map2(ramp gain)
    ret |= os04j10_write_reg(dev, 0x23, 0x00);//ana gain 2msb
    ret |= os04j10_write_reg(dev, 0x24, 0x10);//ana gain 8lsb
    ret |= os04j10_write_reg(dev, 0x50, 0xff);
    ret |= os04j10_write_reg(dev, 0x53, 0xff);
    ret |= os04j10_write_reg(dev, 0x55, 0xff);
    ret |= os04j10_write_reg(dev, 0x58, 0xff);
    ret |= os04j10_write_reg(dev, 0x5c, 0x14);
    ret |= os04j10_write_reg(dev, 0x5d, 0x1c);
    ret |= os04j10_write_reg(dev, 0x5e, 0x13);
    ret |= os04j10_write_reg(dev, 0x66, 0x14);
    ret |= os04j10_write_reg(dev, 0x69, 0x13);
    ret |= os04j10_write_reg(dev, 0x6d, 0x7c);
    ret |= os04j10_write_reg(dev, 0x6e, 0x20);
    ret |= os04j10_write_reg(dev, 0x74, 0x01);
    ret |= os04j10_write_reg(dev, 0x7b, 0x20);
    ret |= os04j10_write_reg(dev, 0x82, 0x12);
    ret |= os04j10_write_reg(dev, 0x83, 0x06);
    ret |= os04j10_write_reg(dev, 0x8e, 0x32);
    ret |= os04j10_write_reg(dev, 0x8f, 0x06);
    ret |= os04j10_write_reg(dev, 0x90, 0x75);
    ret |= os04j10_write_reg(dev, 0x91, 0x20);
    ret |= os04j10_write_reg(dev, 0x92, 0x60);
    ret |= os04j10_write_reg(dev, 0x93, 0x40);
    ret |= os04j10_write_reg(dev, 0x95, 0xf2);
    ret |= os04j10_write_reg(dev, 0xb0, 0x15);
    ret |= os04j10_write_reg(dev, 0xb1, 0x08);
    ret |= os04j10_write_reg(dev, 0xb6, 0x02);
    ret |= os04j10_write_reg(dev, 0xb7, 0x11);

    ret |= os04j10_write_reg(dev, 0xfd, 0x02);
    ret |= os04j10_write_reg(dev, 0x11, 0x16);
    ret |= os04j10_write_reg(dev, 0x12, 0xbc);
    ret |= os04j10_write_reg(dev, 0x13, 0x55);
    ret |= os04j10_write_reg(dev, 0x14, 0x6f);
    ret |= os04j10_write_reg(dev, 0x17, 0xf2);
    ret |= os04j10_write_reg(dev, 0x20, 0x2a);
    ret |= os04j10_write_reg(dev, 0x21, 0x32);
    ret |= os04j10_write_reg(dev, 0x22, 0x32);
    ret |= os04j10_write_reg(dev, 0x23, 0x44);
    ret |= os04j10_write_reg(dev, 0x1b, 0x44);
    ret |= os04j10_write_reg(dev, 0x28, 0x2c);
    ret |= os04j10_write_reg(dev, 0x29, 0x30);
    ret |= os04j10_write_reg(dev, 0x2a, 0x30);
    ret |= os04j10_write_reg(dev, 0x2b, 0x42);
    ret |= os04j10_write_reg(dev, 0x1d, 0x42);
    ret |= os04j10_write_reg(dev, 0x2e, 0x2f);
    ret |= os04j10_write_reg(dev, 0x2f, 0x33);
    ret |= os04j10_write_reg(dev, 0x30, 0x33);
    ret |= os04j10_write_reg(dev, 0x31, 0x45);
    ret |= os04j10_write_reg(dev, 0x1e, 0x45);
    ret |= os04j10_write_reg(dev, 0x2c, 0x3f);
    ret |= os04j10_write_reg(dev, 0x37, 0x15);
    ret |= os04j10_write_reg(dev, 0x38, 0x15);

    ret |= os04j10_write_reg(dev, 0xfd, 0x03);
    ret |= os04j10_write_reg(dev, 0x0d, 0x40);
    ret |= os04j10_write_reg(dev, 0x9a, 0x00);

    ret |= os04j10_write_reg(dev, 0xfd, 0x04);
    ret |= os04j10_write_reg(dev, 0x1b, 0x01);
    ret |= os04j10_write_reg(dev, 0x9b, 0x01);
    ret |= os04j10_write_reg(dev, 0x05, 0x01);
    ret |= os04j10_write_reg(dev, 0x0b, 0x0c);
    ret |= os04j10_write_reg(dev, 0x0c, 0x0f);

    ret |= os04j10_write_reg(dev, 0xfd, 0x05);
    ret |= os04j10_write_reg(dev, 0x10, 0xd0);
    ret |= os04j10_write_reg(dev, 0x17, 0x08);
    ret |= os04j10_write_reg(dev, 0x42, 0x00);//blc_k_b
    ret |= os04j10_write_reg(dev, 0x43, 0x85);//blc_k_b
    ret |= os04j10_write_reg(dev, 0x44, 0x00);//blc_k_gb
    ret |= os04j10_write_reg(dev, 0x45, 0x85);//blc_k_gb
    ret |= os04j10_write_reg(dev, 0x46, 0x00);//blc_k_gr
    ret |= os04j10_write_reg(dev, 0x47, 0x85);//blc_k_gr
    ret |= os04j10_write_reg(dev, 0x48, 0x00);//blc_k_r
    ret |= os04j10_write_reg(dev, 0x49, 0x85);//blc_k_r
    ret |= os04j10_write_reg(dev, 0xb0, 0x01);
    ret |= os04j10_write_reg(dev, 0xb3, 0x01);
    ret |= os04j10_write_reg(dev, 0xb4, 0xff);

    ret |= os04j10_write_reg(dev, 0xfd, 0x03);
    ret |= os04j10_write_reg(dev, 0x16, 0x00);
    ret |= os04j10_write_reg(dev, 0x17, 0x04);
    ret |= os04j10_write_reg(dev, 0x18, 0x0a);//2560
    ret |= os04j10_write_reg(dev, 0x19, 0x00);
    ret |= os04j10_write_reg(dev, 0x1a, 0x00);
    ret |= os04j10_write_reg(dev, 0x1b, 0x04);
    ret |= os04j10_write_reg(dev, 0x1c, 0x05);//1440
    ret |= os04j10_write_reg(dev, 0x1d, 0xa0);
    ret |= os04j10_write_reg(dev, 0x1d, 0xa0);
    ret |= os04j10_write_reg(dev, 0xfd, 0x00);
    ret |= os04j10_write_reg(dev, 0x30, 0x0a);
    ret |= os04j10_write_reg(dev, 0x31, 0x00);
    ret |= os04j10_write_reg(dev, 0x32, 0x05);
    ret |= os04j10_write_reg(dev, 0x33, 0xa0);
    ret |= os04j10_write_reg(dev, 0xfd, 0x05);
    ret |= os04j10_write_reg(dev, 0xc5, 0x06);
    ret |= os04j10_write_reg(dev, 0xc9, 0x06);
    ret |= os04j10_write_reg(dev, 0xcd, 0x06);
    ret |= os04j10_write_reg(dev, 0xc6, 0x13);
    ret |= os04j10_write_reg(dev, 0xca, 0x13);
    ret |= os04j10_write_reg(dev, 0xce, 0x13);
    ret |= os04j10_write_reg(dev, 0xc3, 0x05);
    ret |= os04j10_write_reg(dev, 0xc7, 0x05);
    ret |= os04j10_write_reg(dev, 0xcb, 0x05);
    ret |= os04j10_write_reg(dev, 0xc4, 0x04);
    ret |= os04j10_write_reg(dev, 0xc8, 0x04);
    ret |= os04j10_write_reg(dev, 0xcc, 0x04);
    ret |= os04j10_write_reg(dev, 0xcf, 0x04);
    ret |= os04j10_write_reg(dev, 0xd3, 0x04);
    ret |= os04j10_write_reg(dev, 0xd7, 0x04);
    ret |= os04j10_write_reg(dev, 0xd0, 0x08);
    ret |= os04j10_write_reg(dev, 0xd4, 0x08);
    ret |= os04j10_write_reg(dev, 0xd8, 0x08);
    ret |= os04j10_write_reg(dev, 0xbb, 0x00);
    ret |= os04j10_write_reg(dev, 0xd1, 0x01);
    ret |= os04j10_write_reg(dev, 0xd5, 0x01);
    ret |= os04j10_write_reg(dev, 0xd9, 0x01);
    ret |= os04j10_write_reg(dev, 0xd2, 0x0A);
    ret |= os04j10_write_reg(dev, 0xd6, 0x0A);
    ret |= os04j10_write_reg(dev, 0xda, 0x0A);
    ret |= os04j10_write_reg(dev, 0xbc, 0x3F);
    ret |= os04j10_write_reg(dev, 0xfd, 0x09);
    ret |= os04j10_write_reg(dev, 0x9d, 0xc1);//otp isp en
    ret |= os04j10_write_reg(dev, 0x83, 0x08);//04
    ret |= os04j10_write_reg(dev, 0x8b, 0x17);//04
    ret |= os04j10_write_reg(dev, 0xc1, 0x18);
    ret |= os04j10_write_reg(dev, 0xc3, 0x18);
    ret |= os04j10_write_reg(dev, 0xfd, 0x00);
    ret |= os04j10_write_reg(dev, 0x59, 0x00);
    ret |= os04j10_write_reg(dev, 0x2c, 0x01);//mipi en
    ret |= os04j10_write_reg(dev, 0x57, 0x10);//otp soft rst
    ret |= os04j10_write_reg(dev, 0xfb, 0x03);

    ret |= os04j10_write_reg(dev, 0xfd, 0x09);
    ret |= os04j10_write_reg(dev, 0x9a, 0x00);
    ret |= os04j10_write_reg(dev, 0x9d, 0x00);
    ret |= os04j10_write_reg(dev, 0x88, 0x00);
    ret |= os04j10_write_reg(dev, 0x89, 0x00);
    ret |= os04j10_write_reg(dev, 0x8a, 0x00);
    ret |= os04j10_write_reg(dev, 0x8b, 0x17);
    ret |= os04j10_write_reg(dev, 0x84, 0x40);
    ret |= os04j10_write_reg(dev, 0x81, 0x01);

    ret |= os04j10_write_reg(dev, 0xfd, 0x0c);

    ret |= os04j10_read_reg(dev, 0x00, &dcg_ratio);
    dcg_ratio = dcg_ratio / 0x20;

    os04j10_set_dcg_ratio(dev, dcg_ratio);
    SENSOR_PRINT("====-OV OS04J10_dcg_ratio  0x%x--===\n", dcg_ratio);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04j10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== OS04J10_24MInput_MIPI_4lane_12bit_linear_2560x1440_30fps init success!====\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os04j10_4lane_wdr_2560x1440_init(xmedia_u32 dev)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OS04J10_4M_LINEAR_MODE:
            ret = os04j10_4lane_linear_2560x1440_init(dev);
            break;

        case OS04J10_4M_WDR_MODE:
            ret = os04j10_4lane_wdr_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_os04j10_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os04j10_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "i2c_set_slave_addr error!\n");
        i2c_close(g_os04j10_i2c_fd[dev]);
        g_os04j10_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os04j10_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os04j10_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os04j10_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os04j10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os04j10_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os04j10_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os04j10_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS04J10_DATA_BYTE];

    if (g_os04j10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os04j10_i2c_fd[dev], g_os04j10_i2c_addr[dev], addr, OS04J10_ADDR_BYTE, buf, OS04J10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS04J10_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os04j10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os04j10_i2c_fd[dev], g_os04j10_i2c_addr[dev], buf, OS04J10_ADDR_BYTE + OS04J10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "i2c_write error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04j10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    os04j10_write_reg(dev, 0xfd, 0x01);
    os04j10_read_reg(dev, 0x12, &mirror_flip_type);

    if (mirror_en){
        mirror_flip_type |= 0x0a;
    } else {
        mirror_flip_type &= 0xf5;
    }

    if (flip_en){
        mirror_flip_type |= 0x05;
    } else {
        mirror_flip_type &= 0xfa;
    }

    ret = os04j10_write_reg(dev, 0x12, mirror_flip_type);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04j10_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
