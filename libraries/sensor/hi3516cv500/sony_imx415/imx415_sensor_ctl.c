/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx415_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx415_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <memory.h>

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"

#endif

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};
static HI_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = {0};

#ifndef NA
#define NA 0xFFFF
#endif
#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
int imx415_i2c_init(VI_PIPE vi_pipe)
{
    int ret;
    HI_U8 u8DevNum;
    char acDevFile[I2C_DEV_FILE_NUM];

    if (g_fd[vi_pipe] >= 0) {
        return HI_SUCCESS;
    }
    ISP_SNS_COMMBUS_U *imx415businfo = HI_NULL;
    imx415businfo = imx415_get_bus_Info(vi_pipe);
    u8DevNum = imx415businfo->s8I2cDev;

    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (IMX415_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }

    return HI_SUCCESS;
}

int imx415_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

int imx415_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}
int imx415_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

    if (IMX415_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (IMX415_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, (IMX415_ADDR_BYTE + IMX415_DATA_BYTE));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE DATA error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

void imx415_standby(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx415_write_register(vi_pipe, 0x3000, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_TRUE;
    return;
}

void imx415_restart(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx415_write_register(vi_pipe, 0x3000, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);  /* 1ms: 1000us */
}

void imx415_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            imx415_write_register(vi_pipe, 0x3030, 0x0);
            break;

        case ISP_SNS_MIRROR:
            imx415_write_register(vi_pipe, 0x3030, 0x01);
            break;

        case ISP_SNS_FLIP:
            imx415_write_register(vi_pipe, 0x3030, 0x02);
            break;

        case ISP_SNS_MIRROR_FLIP:
            imx415_write_register(vi_pipe, 0x3030, 0x03);
            break;
    }

    return;
}

/* Mode columns: 0 = 4K@30 10bit (891Mbps), 1 = 4K@20 10bit (594Mbps),
 *               2 = 1080p@60 12bit binning (891Mbps),
 *               3 = 4K@60 10bit (1782Mbps).
 * The last column is the I2C register address. NA means "skip — leave at sensor default".
 * For Mode 3, MIPI D-PHY timing values (0x4018-0x4029) are per IMX415 datasheet p55 col 6;
 * HMAX = 0x0226 and SYS_MODE = 0x04 select the 1782 Mbps / 60 fps all-pixel mode. */
static const HI_U16 g_au16SensorCfgSeq[][IMX415_MODE_BUTT + 1] = {
    {0x7F, 0x7F, 0x7F, 0x7F, 0x3008},
    {0x5B, 0x5B, 0x5B, 0x5B, 0x300A},
    {  NA,   NA, 0x01,   NA, 0X3020},
    {  NA,   NA, 0x01,   NA, 0X3021},
    {  NA,   NA, 0x01,   NA, 0X3022},
    {  NA,   NA, 0xF8,   NA, 0X3024},
    {0x4C, 0x72, 0x1B, 0x26, 0x3028},  /* HMAX low: Mode3 = 0x0226 for 1782Mbps */
    {0x04, 0x06,   NA, 0x02, 0x3029},  /* HMAX hi */
    {  NA,   NA, 0x00,   NA, 0X3031},
    {0x05, 0x07, 0x05, 0x04, 0x3033},  /* SYS_MODE: Mode3 = 0x04 (1782Mbps 4lane 10bit) */
    {0x08, 0x08, 0x08, 0x08, 0x3050},  /* SHR0 */
    {0x00, 0x00, 0x00, 0x00, 0x30C1},
    {  NA,   NA, 0x02,   NA, 0X30D9},
    {  NA,   NA, 0x01,   NA, 0X30DA},
    {0x24, 0x24, 0x24, 0x24, 0x3116},
    {0xC0, 0x80,   NA, 0xC0, 0x3118},  /* Mode3 inherits Mode0 (all-pixel 4K) */
    {0x24, 0x24, 0x24, 0x24, 0x311E},
    {0x21, 0x21, 0x21, 0x21, 0x32D4},
    {0xA1, 0xA1, 0xA1, 0xA1, 0x32EC},
    {0x7F, 0x7F, 0x7F, 0x7F, 0x3452},
    {0x03, 0x03, 0x03, 0x03, 0x3453},
    {0x04, 0x04, 0x04, 0x04, 0x358A},
    {0x02, 0x02, 0x02, 0x02, 0x35A1},
    {0x0C, 0x0C, 0x0C, 0x0C, 0x36BC},
    {0x53, 0x53, 0x53, 0x53, 0x36CC},
    {0x00, 0x00, 0x00, 0x00, 0x36CD},
    {0x3C, 0x3C, 0x3C, 0x3C, 0x36CE},
    {0x8C, 0x8C, 0x8C, 0x8C, 0x36D0},
    {0x00, 0x00, 0x00, 0x00, 0x36D1},
    {0x71, 0x71, 0x71, 0x71, 0x36D2},
    {0x3C, 0x3C, 0x3C, 0x3C, 0x36D4},
    {0x53, 0x53, 0x53, 0x53, 0x36D6},
    {0x00, 0x00, 0x00, 0x00, 0x36D7},
    {0x71, 0x71, 0x71, 0x71, 0x36D8},
    {0x8C, 0x8C, 0x8C, 0x8C, 0x36DA},
    {0x00, 0x00, 0x00, 0x00, 0x36DB},
    {  NA,   NA, 0x00,   NA, 0X3701},
    {0x02, 0x02, 0x02, 0x02, 0x3724},
    {0x02, 0x02, 0x02, 0x02, 0x3726},
    {0x02, 0x02, 0x02, 0x02, 0x3732},
    {0x03, 0x03, 0x03, 0x03, 0x3734},
    {0x03, 0x03, 0x03, 0x03, 0x3736},
    {0x03, 0x03, 0x03, 0x03, 0x3742},
    {0xE0, 0xE0, 0xE0, 0xE0, 0x3862},
    {0x30, 0x30, 0x30, 0x30, 0x38CC},
    {0x2F, 0x2F, 0x2F, 0x2F, 0x38CD},
    {0x0C, 0x0C, 0x0C, 0x0C, 0x395C},
    {0xD1, 0xD1, 0xD1, 0xD1, 0x3A42},
    {0x77, 0x77, 0x77, 0x77, 0x3A4C},
    {0x02, 0x02, 0x02, 0x02, 0x3AE0},
    {0x0C, 0x0C, 0x0C, 0x0C, 0x3AEC},
    {0x2E, 0x2E, 0x2E, 0x2E, 0x3B00},
    {0x29, 0x29, 0x29, 0x29, 0x3B06},
    {0x25, 0x25, 0x25, 0x25, 0x3B98},
    {0x21, 0x21, 0x21, 0x21, 0x3B99},
    {0x13, 0x13, 0x13, 0x13, 0x3B9B},
    {0x13, 0x13, 0x13, 0x13, 0x3B9C},
    {0x13, 0x13, 0x13, 0x13, 0x3B9D},
    {0x13, 0x13, 0x13, 0x13, 0x3B9E},
    {0x00, 0x00, 0x00, 0x00, 0x3BA1},
    {0x06, 0x06, 0x06, 0x06, 0x3BA2},
    {0x0B, 0x0B, 0x0B, 0x0B, 0x3BA3},
    {0x10, 0x10, 0x10, 0x10, 0x3BA4},
    {0x14, 0x14, 0x14, 0x14, 0x3BA5},
    {0x18, 0x18, 0x18, 0x18, 0x3BA6},
    {0x1A, 0x1A, 0x1A, 0x1A, 0x3BA7},
    {0x1A, 0x1A, 0x1A, 0x1A, 0x3BA8},
    {0x1A, 0x1A, 0x1A, 0x1A, 0x3BA9},
    {0xED, 0xED, 0xED, 0xED, 0x3BAC},
    {0x01, 0x01, 0x01, 0x01, 0x3BAD},
    {0xF6, 0xF6, 0xF6, 0xF6, 0x3BAE},
    {0x02, 0x02, 0x02, 0x02, 0x3BAF},
    {0xA2, 0xA2, 0xA2, 0xA2, 0x3BB0},
    {0x03, 0x03, 0x03, 0x03, 0x3BB1},
    {0xE0, 0xE0, 0xE0, 0xE0, 0x3BB2},
    {0x03, 0x03, 0x03, 0x03, 0x3BB3},
    {0xE0, 0xE0, 0xE0, 0xE0, 0x3BB4},
    {0x03, 0x03, 0x03, 0x03, 0x3BB5},
    {0xE0, 0xE0, 0xE0, 0xE0, 0x3BB6},
    {0x03, 0x03, 0x03, 0x03, 0x3BB7},
    {0xE0, 0xE0, 0xE0, 0xE0, 0x3BB8},
    {0xE0, 0xE0, 0xE0, 0xE0, 0x3BBA},
    {0xDA, 0xDA, 0xDA, 0xDA, 0x3BBC},
    {0x88, 0x88, 0x88, 0x88, 0x3BBE},
    {0x44, 0x44, 0x44, 0x44, 0x3BC0},
    {0x7B, 0x7B, 0x7B, 0x7B, 0x3BC2},
    {0xA2, 0xA2, 0xA2, 0xA2, 0x3BC4},
    {0xBD, 0xBD, 0xBD, 0xBD, 0x3BC8},
    {0xBD, 0xBD, 0xBD, 0xBD, 0x3BCA},
    {0x48, 0x48, 0x48, 0x48, 0x4004},
    {0x09, 0x09, 0x09, 0x09, 0x4005},
    {0x00, 0x00, 0x00, 0x00, 0x400C},
    {0x7F, 0x67, 0x7F, 0xB7, 0x4018},  /* TCLKPOST low */
    {0x37, 0x27, 0x37, 0x67, 0x401A},  /* TCLKPREPARE low */
    {0x37, 0x27, 0x37, 0x6F, 0x401C},  /* TCLKTRAIL low */
    {0xF7, 0xB7, 0xF7, 0xDF, 0x401E},  /* TCLKZERO low */
    {0x00, 0x00, 0x00, 0x01, 0x401F},  /* TCLKZERO hi: Mode3 needs 0x01 (0x01DF) */
    {0x3F, 0x2F, 0x3F, 0x87, 0x4020},  /* THSPREPARE low */
    {0x6F, 0x4F, 0x6F, 0xCF, 0x4022},  /* THSZERO low */
    {0x3F, 0x2F, 0x3F, 0x87, 0x4024},  /* THSTRAIL low */
    {0x5F, 0x47, 0x5F, 0xDF, 0x4026},  /* THSEXIT low */
    {0x2F, 0x27, 0x2F, 0x5F, 0x4028},  /* TLPX low */
    {0x01, 0x01, 0x01, 0x01, 0x4074},
};

void imx415_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_U8 u8ImgMode;
    HI_BOOL bInit;
    HI_U16 reg_data;
    HI_U16 u16RegAddr;
    HI_U32 u32SeqEntries;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastimx415 = HI_NULL;
    const IMX415_VIDEO_MODE_TBL_S *imx415modetb1 = HI_NULL;
    pastimx415 = imx415_get_ctx(vi_pipe);
    bInit       = pastimx415->bInit;
    u8ImgMode   = pastimx415->u8ImgMode;
    imx415modetb1 = imx415_get_mode_tb1(u8ImgMode);
    if (bInit == HI_FALSE) {
        /* 2. sensor i2c init */
        imx415_i2c_init(vi_pipe);
    }

    u32SeqEntries = sizeof(g_au16SensorCfgSeq) / sizeof(g_au16SensorCfgSeq[0]);

    for (i = 0 ; i < u32SeqEntries; i++) {
        u16RegAddr = g_au16SensorCfgSeq[i][IMX415_MODE_BUTT];
        reg_data = g_au16SensorCfgSeq[i][u8ImgMode];

        if (reg_data == NA) {
            continue;
        }

        ret += imx415_write_register(vi_pipe, u16RegAddr, reg_data);
    }

    for (i = 0; i < pastimx415->astRegsInfo[0].u32RegNum; i++) {
        ret += imx415_write_register(vi_pipe,
                                     pastimx415->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                     pastimx415->astRegsInfo[0].astI2cData[i].u32Data);
    }
    ret += imx415_write_register(vi_pipe, 0x3000, 0x00); /* Standby Cancel */

    delay_ms(24);  /* delay 24 ms */
    ret += imx415_write_register(vi_pipe, 0x3002, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }

    pastimx415->bInit = HI_TRUE;

    printf("IMX415 %s init succuss!\n", imx415modetb1->pszModeName);

    return ;
}

void imx415_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx415_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;

    return;
}
