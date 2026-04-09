/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov12870_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "ov9284_cmos.h"
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
int ov9284_i2c_init(VI_PIPE vi_pipe)
{
    int ret;
    HI_U8 u8DevNum;
    char acDevFile[I2C_DEV_FILE_NUM];
    ISP_SNS_COMMBUS_U *ov12870businfo = HI_NULL;
    ov12870businfo = ov9284_get_bus_Info(vi_pipe);
    if (g_fd[vi_pipe] >= 0) {
        return HI_SUCCESS;
    }
    u8DevNum = ov12870businfo->s8I2cDev;

    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (OV9284_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
    return HI_SUCCESS;
}

int ov9284_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int ov9284_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}
int ov9284_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

    if (OV9284_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (OV9284_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, (OV9284_ADDR_BYTE + OV9284_DATA_BYTE));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE DATA error!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

void ov9284_standby(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = ov9284_write_register(vi_pipe, 0x100, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_TRUE;
    return;
}

void ov9284_restart(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = ov9284_write_register(vi_pipe, 0x100, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;
    return;
}

void ov9284_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            ov9284_write_register(vi_pipe, 0x3821, 0x40);
            ov9284_write_register(vi_pipe, 0x3820, 0x0);
            break;

        case ISP_SNS_MIRROR:
            ov9284_write_register(vi_pipe, 0x3821, 0x40);
            ov9284_write_register(vi_pipe, 0x3820, 0x4);
            break;

        case ISP_SNS_FLIP:
            ov9284_write_register(vi_pipe, 0x3821, 0x44);
            ov9284_write_register(vi_pipe, 0x3820, 0x0);
            break;

        case ISP_SNS_MIRROR_FLIP:
            ov9284_write_register(vi_pipe, 0x3821, 0x44);
            ov9284_write_register(vi_pipe, 0x3820, 0x4);
            break;
    }

    return;
}

#define OV9284_SENSOR_FCG_SEQ_COMM_COL    2
static const HI_U16 g_au16SensorCfgSeqComm[][OV9284_SENSOR_FCG_SEQ_COMM_COL] = {
    {0x01, 0x0103},
    {0x32, 0x0302},
    {0x50, 0x030d},
    {0x02, 0x030e},
    {0x00, 0x3001},
    {0x00, 0x3004},
    {0x00, 0x3005},
    {0x04, 0x3006},
    {0x0a, 0x3011},
    {0x18, 0x3013},
    {0xf0, 0x301c},
    {0x01, 0x3022},
    {0x10, 0x3030},
    {0x32, 0x3039},
    {0x00, 0x303a},
    {0x00, 0x3500},
    {0x2a, 0x3501},
    {0x90, 0x3502},
    {0x08, 0x3503},
    {0x8c, 0x3505},
    {0x03, 0x3507},
    {0x00, 0x3508},
    {0x10, 0x3509},
    {0x80, 0x3610},
    {0xa0, 0x3611},
    {0x6e, 0x3620},
    {0x56, 0x3632},
    {0x78, 0x3633},
    {0x05, 0x3662},
    {0x00, 0x3666},
    {0x5a, 0x366f},
    {0x84, 0x3680},
    {0x80, 0x3712},
    {0x22, 0x372d},
    {0x80, 0x3731},
    {0x30, 0x3732},
    {0x00, 0x3778},
    {0x22, 0x377d},
    {0x02, 0x3788},
    {0xa4, 0x3789},
    {0x00, 0x378a},
    {0x4a, 0x378b},
    {0x20, 0x3799},
    {0x00, 0x3800},
    {0x00, 0x3801},
    {0x00, 0x3802},
    {0x00, 0x3803},
    {0x05, 0x3804},
    {0x0f, 0x3805},
    {0x03, 0x3806},
    {0x2f, 0x3807},
    {0x05, 0x3808},
    {0x00, 0x3809},
    {0x03, 0x380a},
    {0x20, 0x380b},
    {0x02, 0x380c},
    {0xd8, 0x380d},
    {0x07, 0x380e},
    {0x27, 0x380f},
    {0x00, 0x3810},
    {0x08, 0x3811},
    {0x00, 0x3812},
    {0x08, 0x3813},
    {0x11, 0x3814},
    {0x11, 0x3815},
    {0x40, 0x3820},
    {0x00, 0x3821},
    {0x05, 0x382c},
    {0xb0, 0x382d},
    {0x00, 0x389d},
    {0x42, 0x3881},
    {0x01, 0x3882},
    {0x00, 0x3883},
    {0x02, 0x3885},
    {0x02, 0x38a8},
    {0x80, 0x38a9},
    {0x00, 0x38b1},
    {0x02, 0x38b3},
    {0x00, 0x38c4},
    {0xc0, 0x38c5},
    {0x04, 0x38c6},
    {0x80, 0x38c7},
    {0xff, 0x3920},
    {0x40, 0x4003},
    {0x04, 0x4008},
    {0x0b, 0x4009},
    {0x00, 0x400c},
    {0x07, 0x400d},
    {0x40, 0x4010},
    {0x40, 0x4043},
    {0x30, 0x4307},
    {0x00, 0x4317},
    {0x00, 0x4501},
    {0x00, 0x4507},
    {0x00, 0x4509},
    {0x08, 0x450a},
    {0x04, 0x4601},
    {0x00, 0x470f},
    {0x00, 0x4f07},
    {0x00, 0x4800},
    {0x9f, 0x5000},
    {0x00, 0x5001},
    {0x00, 0x5e00},
    {0x07, 0x5d00},
    {0x00, 0x5d01},
    {0x04, 0x4f00},
    {0x00, 0x4f10},
    {0x98, 0x4f11},
    {0x0f, 0x4f12},
    {0xc4, 0x4f13},
    {0x01, 0x0100},
    {0x44, 0x3501},
    {0x40, 0x3502},
    {0x87, 0x5000},
};

static const HI_U16 g_au16SensorCfgSeq[][OV9284_MODE_BUTT + 1] = {
    {0x08, 0x3006},
    {0xff, 0x3920},
    {0xc0, 0x392f},
    {0x00, 0x3921},
    {0x00, 0x3922},
    {0x00, 0x3923},
    {0x00, 0x3924},
    {0x00, 0x3926},
    {0x01, 0x3927},
    {0x80, 0x3928},

};

void ov9284_init(VI_PIPE vi_pipe)
{
    HI_U32 i, u32SeqEntries;
    HI_S32    ret = HI_SUCCESS;
    HI_U16 u16RegData, u16RegAddr;
    ISP_SNS_STATE_S *pastov12870 = HI_NULL;
    const OV9284_VIDEO_MODE_TBL_S *ov12870modetb1 = HI_NULL;
    pastov12870 = ov9284_get_ctx(vi_pipe);
    HI_BOOL bInit       = pastov12870->bInit;
    HI_U8 u8ImgMode   = pastov12870->u8ImgMode;
    ov12870modetb1 = ov9284_get_mode_tb1(u8ImgMode);
    if (bInit == HI_FALSE) {
        ov9284_i2c_init(vi_pipe); /* 2. sensor i2c init */
    }
    if (!g_bStandby[vi_pipe] || !pastov12870->bInit) {
        u32SeqEntries = sizeof(g_au16SensorCfgSeqComm) / sizeof(g_au16SensorCfgSeqComm[0]);
        for (i = 0; i < u32SeqEntries; i++) {
            u16RegAddr = g_au16SensorCfgSeqComm[i][1];
            u16RegData = g_au16SensorCfgSeqComm[i][0];

            if (u16RegData == NA) {
                continue;
            }
            ov9284_write_register(vi_pipe, u16RegAddr, u16RegData);
        }
    }
    u32SeqEntries = sizeof(g_au16SensorCfgSeq) / sizeof(g_au16SensorCfgSeq[0]);
    for (i = 0; i < u32SeqEntries; i++) {
        u16RegAddr = g_au16SensorCfgSeq[i][OV9284_MODE_BUTT];
        u16RegData = g_au16SensorCfgSeq[i][u8ImgMode];
        if (u16RegData == NA) {
            continue;
        }
        ret += ov9284_write_register(vi_pipe, u16RegAddr, u16RegData);
        if (ret != HI_SUCCESS) {
            SNS_ERR_TRACE("write register failed!\n");
            return;
        }
    }
    for (i = 0; i < pastov12870->astRegsInfo[0].u32RegNum; i++) {
        ret += ov9284_write_register(vi_pipe, pastov12870->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                     pastov12870->astRegsInfo[0].astI2cData[i].u32Data);
    }
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    pastov12870->bInit = HI_TRUE;
    ov9284_restart(vi_pipe);
    printf("OV9284 %s init succuss!\n", ov12870modetb1->pszModeName);
}

void ov9284_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = ov9284_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;
    return;
}
