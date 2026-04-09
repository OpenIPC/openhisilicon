/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os04b_2l_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "os04b_2l_cmos.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};

#define Os04b_2l_4M_2560x1440_10bit_linear30   0
#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
/* VI 297M */
void os04b_2l_init_4M_2560x1440_10bit_linear(VI_PIPE vi_pipe);

int os04b_2l_i2c_init(VI_PIPE vi_pipe)
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
    ISP_SNS_COMMBUS_U *os04b2lbusinfo = HI_NULL;
    os04b2lbusinfo = os04b_2l_get_bus_Info(vi_pipe);
    u8DevNum = os04b2lbusinfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (OS04B_2L_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int os04b_2l_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int os04b_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}

int os04b_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = OS04B_2L_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OS04B_2L_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OS04B_2L_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (OS04B_2L_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (OS04B_2L_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, (OS04B_2L_ADDR_BYTE + OS04B_2L_DATA_BYTE));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE error! addr=0x%x data=0x%x\n", addr, data);
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

void os04b_2l_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            os04b_2l_write_register(vi_pipe, 0x3f, 0x01);
            os04b_2l_write_register(vi_pipe, 0x01, 0x01);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x02);
            os04b_2l_write_register(vi_pipe, 0x62, 0x09);
            os04b_2l_write_register(vi_pipe, 0x63, 0x00);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            break;

        case ISP_SNS_MIRROR:
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            os04b_2l_write_register(vi_pipe, 0x3f, 0x00);
            os04b_2l_write_register(vi_pipe, 0x01, 0x01);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x02);
            os04b_2l_write_register(vi_pipe, 0x62, 0x09);
            os04b_2l_write_register(vi_pipe, 0x63, 0x00);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            break;

        case ISP_SNS_FLIP:
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            os04b_2l_write_register(vi_pipe, 0x3f, 0x03);
            os04b_2l_write_register(vi_pipe, 0x01, 0x01);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x02);
            os04b_2l_write_register(vi_pipe, 0x62, 0xa8);
            os04b_2l_write_register(vi_pipe, 0x63, 0x05);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            break;

        case ISP_SNS_MIRROR_FLIP:
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            os04b_2l_write_register(vi_pipe, 0x3f, 0x02);
            os04b_2l_write_register(vi_pipe, 0x01, 0x01);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x02);
            os04b_2l_write_register(vi_pipe, 0x62, 0xa8);
            os04b_2l_write_register(vi_pipe, 0x63, 0x05);
            os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
            break;
    }

    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);  /* 1ms: 1000us */
    return;
}

void os04b_2l_standby(VI_PIPE vi_pipe)
{
    return;
}

void os04b_2l_restart(VI_PIPE vi_pipe)
{
    return;
}

void os04b_2l_prog(VI_PIPE vi_pipe, const HI_U32 *rom)
{
    int i = 0;
    while (1) {
        HI_U32 lookup = rom[i++];
        HI_U32 addr = (lookup >> 16) & 0xFFFF; /* shift 16 */
        HI_U32 data = lookup & 0xFFFF;
        if (addr == 0xFFFE) {
            delay_ms(data);
        } else if (addr == 0xFFFF) {
            return;
        } else {
            os04b_2l_write_register(vi_pipe, addr, data);
        }
    }
}

void os04b_2l_default_reg_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastos04b2l = HI_NULL;
    pastos04b2l = os04b_2l_get_ctx(vi_pipe);
    for (i = 0; i < pastos04b2l->astRegsInfo[0].u32RegNum; i++) {
        ret += os04b_2l_write_register(vi_pipe,
                                       pastos04b2l->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                       pastos04b2l->astRegsInfo[0].astI2cData[i].u32Data);
    }
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void os04b_2l_init(VI_PIPE vi_pipe)
{
    HI_BOOL  bInit;
    HI_U8    u8ImgMode;
    HI_S32   ret;
    ISP_SNS_STATE_S *pastos04b2l = HI_NULL;
    pastos04b2l = os04b_2l_get_ctx(vi_pipe);
    bInit       = pastos04b2l->bInit;
    u8ImgMode = pastos04b2l->u8ImgMode;

    /*  2. os04b i2c init  */
    ret = os04b_2l_i2c_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c init failed!\n");
        return;
    }
    /* When sensor first init, config all registers */
    if (bInit == HI_FALSE) {
        switch (u8ImgMode) {
            case Os04b_2l_4M_2560x1440_10bit_linear30:
                os04b_2l_init_4M_2560x1440_10bit_linear(vi_pipe);
                break;

            default:
                SNS_ERR_TRACE("Not Support Image Mode %d\n", u8ImgMode);
                break;
        }
    } else {
        /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        switch (u8ImgMode) {
            case Os04b_2l_4M_2560x1440_10bit_linear30:
                os04b_2l_init_4M_2560x1440_10bit_linear(vi_pipe);
                break;

            default:
                SNS_ERR_TRACE("Not Support Image Mode %d\n", u8ImgMode);
                break;
        }
    }

    pastos04b2l->bInit = HI_TRUE;

    return;
}

void os04b_2l_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = os04b_2l_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
    }
    return;
}

void os04b_2l_init_4M_2560x1440_10bit_linear(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x00); /* 0x01 */
    ret += os04b_2l_write_register(vi_pipe, 0x20, 0x00);
    delay_ms(3);  /* delay 3 ms */
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x34, 0x71);
    ret += os04b_2l_write_register(vi_pipe, 0x32, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x33, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x2e, 0x0c);
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x03, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x04, 0xc6);
    ret += os04b_2l_write_register(vi_pipe, 0x06, 0x0b);
    ret += os04b_2l_write_register(vi_pipe, 0x0a, 0x50);
    ret += os04b_2l_write_register(vi_pipe, 0x38, 0x20);
    ret += os04b_2l_write_register(vi_pipe, 0x39, 0x08);
    ret += os04b_2l_write_register(vi_pipe, 0x31, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x24, 0xff);
    ret += os04b_2l_write_register(vi_pipe, 0x01, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x11, 0x59);
    ret += os04b_2l_write_register(vi_pipe, 0x13, 0xf4);
    ret += os04b_2l_write_register(vi_pipe, 0x14, 0xff);
    ret += os04b_2l_write_register(vi_pipe, 0x19, 0xf2);
    ret += os04b_2l_write_register(vi_pipe, 0x16, 0x68);
    ret += os04b_2l_write_register(vi_pipe, 0x1a, 0x5e);
    ret += os04b_2l_write_register(vi_pipe, 0x1c, 0x1a);
    ret += os04b_2l_write_register(vi_pipe, 0x1d, 0xd6);
    ret += os04b_2l_write_register(vi_pipe, 0x1f, 0x17);
    ret += os04b_2l_write_register(vi_pipe, 0x20, 0x99);
    ret += os04b_2l_write_register(vi_pipe, 0x26, 0x76);
    ret += os04b_2l_write_register(vi_pipe, 0x27, 0x0c);
    ret += os04b_2l_write_register(vi_pipe, 0x29, 0x3b);
    ret += os04b_2l_write_register(vi_pipe, 0x2a, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x2b, 0x8e);
    ret += os04b_2l_write_register(vi_pipe, 0x2c, 0x0b);
    ret += os04b_2l_write_register(vi_pipe, 0x2e, 0x02);
    ret += os04b_2l_write_register(vi_pipe, 0x44, 0x03);
    ret += os04b_2l_write_register(vi_pipe, 0x45, 0xbe);
    ret += os04b_2l_write_register(vi_pipe, 0x50, 0x06);
    ret += os04b_2l_write_register(vi_pipe, 0x51, 0x10); /* 0x18 */
    ret += os04b_2l_write_register(vi_pipe, 0x52, 0x0d); /* 0x10 */
    ret += os04b_2l_write_register(vi_pipe, 0x53, 0x08);
    ret += os04b_2l_write_register(vi_pipe, 0x55, 0x15);
    ret += os04b_2l_write_register(vi_pipe, 0x56, 0x00);

    ret += os04b_2l_write_register(vi_pipe, 0x57, 0x09); /* 0x0d */
    ret += os04b_2l_write_register(vi_pipe, 0x59, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x5a, 0x04);
    ret += os04b_2l_write_register(vi_pipe, 0x5b, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x5c, 0xe0); /* 0xb8,0xc0 */
    ret += os04b_2l_write_register(vi_pipe, 0x5d, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x65, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x67, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x66, 0x2a);
    ret += os04b_2l_write_register(vi_pipe, 0x68, 0x2c);
    ret += os04b_2l_write_register(vi_pipe, 0x69, 0x0c); /* 0x16 */
    ret += os04b_2l_write_register(vi_pipe, 0x6a, 0x0a); /* 0x10 */
    ret += os04b_2l_write_register(vi_pipe, 0x6b, 0x03);
    ret += os04b_2l_write_register(vi_pipe, 0x6c, 0x18);
    ret += os04b_2l_write_register(vi_pipe, 0x71, 0x42);
    ret += os04b_2l_write_register(vi_pipe, 0x72, 0x04);
    ret += os04b_2l_write_register(vi_pipe, 0x73, 0x30);
    ret += os04b_2l_write_register(vi_pipe, 0x74, 0x03);
    ret += os04b_2l_write_register(vi_pipe, 0x77, 0x28);
    ret += os04b_2l_write_register(vi_pipe, 0x7b, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x7f, 0x18);
    ret += os04b_2l_write_register(vi_pipe, 0x83, 0xf0);
    ret += os04b_2l_write_register(vi_pipe, 0x85, 0x10);
    ret += os04b_2l_write_register(vi_pipe, 0x86, 0xf0);
    ret += os04b_2l_write_register(vi_pipe, 0x8a, 0x33);
    ret += os04b_2l_write_register(vi_pipe, 0x8b, 0x33);
    ret += os04b_2l_write_register(vi_pipe, 0x28, 0x04);
    ret += os04b_2l_write_register(vi_pipe, 0x34, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x35, 0x08);
    ret += os04b_2l_write_register(vi_pipe, 0x36, 0x0a);
    ret += os04b_2l_write_register(vi_pipe, 0x37, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x4a, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x4b, 0x04);
    ret += os04b_2l_write_register(vi_pipe, 0x4c, 0x05);
    ret += os04b_2l_write_register(vi_pipe, 0x4d, 0xa0);
    ret += os04b_2l_write_register(vi_pipe, 0x01, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x8e, 0x0a);
    ret += os04b_2l_write_register(vi_pipe, 0x8f, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x90, 0x05);
    ret += os04b_2l_write_register(vi_pipe, 0x91, 0xa0);
    ret += os04b_2l_write_register(vi_pipe, 0xa1, 0x04);
    ret += os04b_2l_write_register(vi_pipe, 0xc4, 0x80);
    ret += os04b_2l_write_register(vi_pipe, 0xc5, 0x80);
    ret += os04b_2l_write_register(vi_pipe, 0xc6, 0x80);
    ret += os04b_2l_write_register(vi_pipe, 0xc7, 0x80);
    ret += os04b_2l_write_register(vi_pipe, 0xfb, 0x00); /* 0x03 */
    ret += os04b_2l_write_register(vi_pipe, 0xfa, 0x16);
    ret += os04b_2l_write_register(vi_pipe, 0xfa, 0x14);
    ret += os04b_2l_write_register(vi_pipe, 0xfa, 0x02);

    ret += os04b_2l_write_register(vi_pipe, 0xf0, 0x40);
    ret += os04b_2l_write_register(vi_pipe, 0xf1, 0x40);
    ret += os04b_2l_write_register(vi_pipe, 0xf2, 0x40);
    ret += os04b_2l_write_register(vi_pipe, 0xf3, 0x40);
    ret += os04b_2l_write_register(vi_pipe, 0xb1, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0xb6, 0x80);
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x36, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0x34, 0x72);
    ret += os04b_2l_write_register(vi_pipe, 0x34, 0x71);
    ret += os04b_2l_write_register(vi_pipe, 0x36, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0xfb, 0x03);
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x03);
    ret += os04b_2l_write_register(vi_pipe, 0xc0, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x02);
    ret += os04b_2l_write_register(vi_pipe, 0xa8, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0xa9, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0xaa, 0x08);
    ret += os04b_2l_write_register(vi_pipe, 0xab, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0xac, 0x08);
    ret += os04b_2l_write_register(vi_pipe, 0xad, 0x05);
    ret += os04b_2l_write_register(vi_pipe, 0xae, 0xa0);
    ret += os04b_2l_write_register(vi_pipe, 0xaf, 0x0a);
    ret += os04b_2l_write_register(vi_pipe, 0xb0, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0x62, 0x09);
    ret += os04b_2l_write_register(vi_pipe, 0x63, 0x00);
    ret += os04b_2l_write_register(vi_pipe, 0xfd, 0x01);
    ret += os04b_2l_write_register(vi_pipe, 0xb1, 0x03);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("-------OV sssos04b_2l_init_4M_2560x1440_10bit_linear Initial OK!-------\n");
    return;
}
