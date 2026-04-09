/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of gc2053_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "gc2053_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <hi_math.h>

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};
static HI_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = {0};

/* sensor fps mode */
#define GC2053_SENSOR_1080P_30FPS_LINEAR_MODE  0

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
int gc2053_i2c_init(VI_PIPE vi_pipe)
{
    char acDevFile[I2C_DEV_FILE_NUM] = {0};
    HI_U8 u8DevNum;

    if (g_fd[vi_pipe] >= 0) {
        return HI_SUCCESS;
    }
#ifdef HI_GPIO_I2C
    int ret;

    g_fd[vi_pipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open gpioi2c_ex error!\n");
        return HI_FAILURE;
    }
#else
    int ret;
    ISP_SNS_COMMBUS_U *gc2053businfo = HI_NULL;
    gc2053businfo = gc2053_get_bus_Info(vi_pipe);
    u8DevNum = gc2053businfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (GC2053_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int gc2053_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int gc2053_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}

int gc2053_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = GC2053_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = GC2053_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = GC2053_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (GC2053_ADDR_BYTE == 2) { /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (GC2053_DATA_BYTE == 2) { /* 2 byte */
        buf[idx] = (data >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, GC2053_ADDR_BYTE + GC2053_DATA_BYTE);
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

void gc2053_standby(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += gc2053_write_register(vi_pipe, 0xf2, 0x01);

    ret += gc2053_write_register(vi_pipe, 0xfc, 0x8f);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_TRUE;
    return;
}

void gc2053_restart(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += gc2053_write_register(vi_pipe, 0xf2, 0x00);

    ret += gc2053_write_register(vi_pipe, 0xfc, 0x8e);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;
    return;
}

void gc2053_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            gc2053_write_register(vi_pipe, 0x17, 0x80);
            break;

        case ISP_SNS_MIRROR:
            gc2053_write_register(vi_pipe, 0x17, 0x81);
            break;

        case ISP_SNS_FLIP:
            gc2053_write_register(vi_pipe, 0x17, 0x82);
            break;

        case ISP_SNS_MIRROR_FLIP:
            gc2053_write_register(vi_pipe, 0x17, 0x83);
            break;
    }

    return;
}

void gc2053_linear_1080p30_init(VI_PIPE vi_pipe);

void gc2053_init(VI_PIPE vi_pipe)
{
    HI_U32   i;
    HI_U8    u8ImgMode;
    HI_BOOL  bInit;
    HI_S32   ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastgc2053 = HI_NULL;
    pastgc2053 = gc2053_get_ctx(vi_pipe);
    bInit       = pastgc2053->bInit;
    u8ImgMode   = pastgc2053->u8ImgMode;

    if (bInit == HI_FALSE) {
        /* 1. sensor i2c init */
        gc2053_i2c_init(vi_pipe);
    }

    switch (u8ImgMode) {
        case 0: /* 0 */
            gc2053_linear_1080p30_init(vi_pipe);
            break;
        default:
            printf("GC2053_SENSOR_CTL_Not support this mode\n");
            pastgc2053->bInit = HI_FALSE;
            break;
    }

    for (i = 0; i < pastgc2053->astRegsInfo[0].u32RegNum; i++) {
        ret += gc2053_write_register(vi_pipe,
                                     pastgc2053->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                     pastgc2053->astRegsInfo[0].astI2cData[i].u32Data);
    }
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }

    pastgc2053->bInit = HI_TRUE;

    printf("GC2053 init succuss!\n");

    return;
}

void gc2053_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = gc2053_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
    }
    g_bStandby[vi_pipe] = HI_FALSE;

    return;
}

void gc2053_linear_1080p30_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x80);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x80);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x80);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x00);
    ret += gc2053_write_register(vi_pipe, 0xf2, 0x00);
    ret += gc2053_write_register(vi_pipe, 0xf3, 0x00);
    ret += gc2053_write_register(vi_pipe, 0xf4, 0x36);
    ret += gc2053_write_register(vi_pipe, 0xf5, 0xc0);
    ret += gc2053_write_register(vi_pipe, 0xf6, 0x44);
    ret += gc2053_write_register(vi_pipe, 0xf7, 0x01);
    ret += gc2053_write_register(vi_pipe, 0xf8, 0x2c);
    ret += gc2053_write_register(vi_pipe, 0xf9, 0x42);
    ret += gc2053_write_register(vi_pipe, 0xfc, 0x8e);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x00);
    ret += gc2053_write_register(vi_pipe, 0x87, 0x18);
    ret += gc2053_write_register(vi_pipe, 0xee, 0x30);
    ret += gc2053_write_register(vi_pipe, 0xd0, 0xb7);
    ret += gc2053_write_register(vi_pipe, 0x03, 0x04);
    ret += gc2053_write_register(vi_pipe, 0x04, 0x60);
    ret += gc2053_write_register(vi_pipe, 0x05, 0x04);
    ret += gc2053_write_register(vi_pipe, 0x06, 0x4c);
    ret += gc2053_write_register(vi_pipe, 0x07, 0x00);
    ret += gc2053_write_register(vi_pipe, 0x08, 0x11);
    ret += gc2053_write_register(vi_pipe, 0x0a, 0x02);
    ret += gc2053_write_register(vi_pipe, 0x0c, 0x02);
    ret += gc2053_write_register(vi_pipe, 0x0d, 0x04);
    ret += gc2053_write_register(vi_pipe, 0x0e, 0x40);
    ret += gc2053_write_register(vi_pipe, 0x12, 0xe2);
    ret += gc2053_write_register(vi_pipe, 0x13, 0x16);
    ret += gc2053_write_register(vi_pipe, 0x19, 0x0a);
    ret += gc2053_write_register(vi_pipe, 0x28, 0x0a);
    ret += gc2053_write_register(vi_pipe, 0x2b, 0x04);
    ret += gc2053_write_register(vi_pipe, 0x37, 0x03);
    ret += gc2053_write_register(vi_pipe, 0x43, 0x07);
    ret += gc2053_write_register(vi_pipe, 0x44, 0x40);
    ret += gc2053_write_register(vi_pipe, 0x46, 0x0b);
    ret += gc2053_write_register(vi_pipe, 0x4b, 0x20);
    ret += gc2053_write_register(vi_pipe, 0x4e, 0x08);
    ret += gc2053_write_register(vi_pipe, 0x55, 0x20);
    ret += gc2053_write_register(vi_pipe, 0x77, 0x01);
    ret += gc2053_write_register(vi_pipe, 0x78, 0x00);
    ret += gc2053_write_register(vi_pipe, 0x7c, 0x93);
    ret += gc2053_write_register(vi_pipe, 0x8d, 0x92);
    ret += gc2053_write_register(vi_pipe, 0x90, 0x00);

    ret += gc2053_write_register(vi_pipe, 0x41, 0x04);
    ret += gc2053_write_register(vi_pipe, 0x42, 0x65);
    ret += gc2053_write_register(vi_pipe, 0xce, 0x7c);
    ret += gc2053_write_register(vi_pipe, 0xd2, 0x41);
    ret += gc2053_write_register(vi_pipe, 0xd3, 0xdc);
    ret += gc2053_write_register(vi_pipe, 0xe6, 0x50);
    ret += gc2053_write_register(vi_pipe, 0xb6, 0xc0);
    ret += gc2053_write_register(vi_pipe, 0xb0, 0x70);
    ret += gc2053_write_register(vi_pipe, 0x26, 0x30);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x01);
    ret += gc2053_write_register(vi_pipe, 0x55, 0x07);
    /* dither */
    ret += gc2053_write_register(vi_pipe, 0x58, 0x00); /* default 0x80 */
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x04);
    ret += gc2053_write_register(vi_pipe, 0x14, 0x78);
    ret += gc2053_write_register(vi_pipe, 0x15, 0x78);
    ret += gc2053_write_register(vi_pipe, 0x16, 0x78);
    ret += gc2053_write_register(vi_pipe, 0x17, 0x78);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x01);
    ret += gc2053_write_register(vi_pipe, 0x04, 0x00);
    ret += gc2053_write_register(vi_pipe, 0x94, 0x03);
    ret += gc2053_write_register(vi_pipe, 0x97, 0x07);
    ret += gc2053_write_register(vi_pipe, 0x98, 0x80);
    ret += gc2053_write_register(vi_pipe, 0x9a, 0x06);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x00);
    ret += gc2053_write_register(vi_pipe, 0x7b, 0x2a);
    ret += gc2053_write_register(vi_pipe, 0x23, 0x2d);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x03);
    ret += gc2053_write_register(vi_pipe, 0x01, 0x27);
    ret += gc2053_write_register(vi_pipe, 0x02, 0x56);
    ret += gc2053_write_register(vi_pipe, 0x03, 0xb6);
    ret += gc2053_write_register(vi_pipe, 0x12, 0x80);
    ret += gc2053_write_register(vi_pipe, 0x13, 0x07);
    ret += gc2053_write_register(vi_pipe, 0x15, 0x12);
    ret += gc2053_write_register(vi_pipe, 0xfe, 0x00);
    ret += gc2053_write_register(vi_pipe, 0x3e, 0x91);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("=== Galaxycore GC2053_1080P_30FPS_10BIT_LINE_Init_OK!===\n");
    return;
}
