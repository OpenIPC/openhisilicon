/*
 * Copyright (c) GK.  All rights reserved.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "comm_video.h"
#include "sns_ctrl.h"
#include "sp2308_cmos.h"

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#ifdef __LITEOS__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif

#define LOSCFG_HOST_TYPE_VENDOR
#ifdef LOSCFG_HOST_TYPE_VENDOR
#include<linux/fb.h>
#endif

struct i2c_rdwr_ioctl_data {
        struct i2c_msg  *msgs;
        unsigned int nmsgs;
};

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};
static GK_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = {0};


// sensor fps mode
#define SP2308_SENSOR_1080p30_LINEAR_MODE   (0)

int sp2308_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[16] = {0};
    GK_U8 u8DevNum;

    if (g_fd[ViPipe] >= 0) {
        return GK_SUCCESS;
    }
#ifdef GPIO_I2C
    int ret;

    g_fd[ViPipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open gpioi2c_ex error!\n");
        return GK_FAILURE;
    }
#else
    int ret;

    ISP_SNS_COMMBUS_U *sp2308businfo = GK_NULL;

    sp2308businfo = sp2308_get_bus_Info(ViPipe);
    u8DevNum = sp2308businfo->s8I2cDev;
    ret = snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);
    if (ret < 0) {
        return GK_FAILURE;
    }

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (SP2308_I2C_ADDR >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int sp2308_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int sp2308_read_register(VI_PIPE ViPipe, GK_S32 addr)
{
    GK_S32 s32RegVal = 0;
    if (g_fd[ViPipe] < 0) {
        return GK_FAILURE;
    }

    GK_S32 s32Ret = 0;
    GK_U32 u32RegWidth = SP2308_ADDR_BYTE;
    GK_U32 u32DataWidth = SP2308_DATA_BYTE;
    GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
    GK_U32 u32SnsI2cAddr = (SP2308_I2C_ADDR >> 1);
    struct i2c_rdwr_ioctl_data stRdwr;
    struct i2c_msg astMsg[2];
    memset(&stRdwr, 0x0, sizeof(stRdwr));
    memset(astMsg, 0x0, sizeof(astMsg));
#endif

    memset(aRecvbuf, 0x0, sizeof(aRecvbuf));

#ifdef LOSCFG_HOST_TYPE_VENDOR
    astMsg[0].addr = u32SnsI2cAddr;
    astMsg[0].flags = 0;
    astMsg[0].len = u32RegWidth;
    astMsg[0].buf = aRecvbuf;

    astMsg[1].addr = u32SnsI2cAddr;
    astMsg[1].flags = 0;
    astMsg[1].flags |= I2C_M_RD;
    astMsg[1].len = u32DataWidth;
    astMsg[1].buf = aRecvbuf;
    stRdwr.msgs = &astMsg[0];
    stRdwr.nmsgs = 2;
#endif

#ifdef LOSCFG_HOST_TYPE_VENDOR
    if (u32RegWidth == 2) {
        aRecvbuf[0] = (addr >> 8) & 0xff;
        aRecvbuf[1] = addr & 0xff;
    } else {
        aRecvbuf[0] = addr & 0xff;
    }
    s32Ret = ioctl(g_fd[ViPipe], I2C_RDWR, &stRdwr);
#else
    if (u32RegWidth == 2) {
        aRecvbuf[0] = addr & 0xff;
        aRecvbuf[1] = (addr >> 8) & 0xff;
    } else {
        aRecvbuf[0] = addr & 0xff;
    }
    s32Ret = read(g_fd[ViPipe], aRecvbuf, u32RegWidth + u32DataWidth);
#endif
    if (s32Ret < 0) {
        return GK_FAILURE;
    }

    if (u32DataWidth == 2) {
        s32RegVal = aRecvbuf[0] | (aRecvbuf[1] << 8);
    } else {
        s32RegVal = aRecvbuf[0];
    }
    return s32RegVal;
}


int sp2308_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = SP2308_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = SP2308_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = SP2308_DATA_BYTE;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else

	int idx = 0;
	int ret;
	char buf[8];

    if (SP2308_ADDR_BYTE == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (SP2308_DATA_BYTE == 2) {
        buf[idx] = (data >> 8) & 0xff;
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, SP2308_ADDR_BYTE + SP2308_DATA_BYTE);
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }

#endif
    return GK_SUCCESS;
}

void sp2308_standby(VI_PIPE ViPipe)
{
    return;
}

void sp2308_restart(VI_PIPE ViPipe)
{
    return;
}

void sp2308_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    int mirror_flip = sp2308_read_register(ViPipe,0x3f);
    mirror_flip &=  0xfc;
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            break;
        case ISP_SNS_MIRROR:
            mirror_flip |= 0x01;
            break;
        case ISP_SNS_FLIP:
            mirror_flip |= 0x02;
            break;
        case ISP_SNS_MIRROR_FLIP:
            mirror_flip |= 0x03;
            break;
    }
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0x3f,mirror_flip);
    sp2308_write_register(ViPipe,0x01,0x01);
    return;
}

void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void sp2308_linear_1080p30_init(VI_PIPE ViPipe);

void sp2308_init(VI_PIPE ViPipe)
{
    GK_U32           i;
    GK_U8            u8ImgMode;
    GK_BOOL          bInit;

    ISP_SNS_STATE_S *pastSp2308 = GK_NULL;
    pastSp2308 = sp2308_get_ctx(ViPipe);
    bInit      = pastSp2308->bInit;
    u8ImgMode  = pastSp2308->u8ImgMode;

    if (GK_FALSE == bInit) {
        /* sensor i2c init */
        printf("binit false SP2308 i2c init\n");
        sp2308_i2c_init(ViPipe);
    }

    switch (u8ImgMode) {
        case SP2308_SENSOR_1080p30_LINEAR_MODE:
            sp2308_linear_1080p30_init(ViPipe);
            break;
        default:
            printf("SP2308_SENSOR_CTL_Not support this mode\n");
            pastSp2308->bInit = GK_FALSE;
            break;
    }

    for (i = 0; i < pastSp2308->astRegsInfo[0].u32RegNum; i++) {
        sp2308_write_register(ViPipe, pastSp2308->astRegsInfo[0].astI2cData[i].u32RegAddr,
                              pastSp2308->astRegsInfo[0].astI2cData[i].u32Data);
    }
    pastSp2308->bInit = GK_TRUE;


    return;
}

void sp2308_exit(VI_PIPE ViPipe)
{
    sp2308_i2c_exit(ViPipe);
    g_bStandby[ViPipe] = GK_FALSE;

    return;
}


/* 1080P30 and 1080P25 */
void sp2308_linear_1080p30_init(VI_PIPE ViPipe)
{
    sp2308_write_register(ViPipe,0xfd,0x00);
    sp2308_write_register(ViPipe,0xfd,0x00);
    sp2308_write_register(ViPipe,0x35,0x04);
    sp2308_write_register(ViPipe,0x2e,0x1b);
    sp2308_write_register(ViPipe,0xfd,0x00);
    sp2308_write_register(ViPipe,0x36,0x00);
    sp2308_write_register(ViPipe,0x30,0x19);
    sp2308_write_register(ViPipe,0x38,0x15);
    sp2308_write_register(ViPipe,0x41,0x06);
    sp2308_write_register(ViPipe,0x43,0x01);
    sp2308_write_register(ViPipe,0x44,0x00);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0x03,0x04);
    sp2308_write_register(ViPipe,0x04,0x7d);
    sp2308_write_register(ViPipe,0x24,0x10);
    sp2308_write_register(ViPipe,0x66,0x33);
    sp2308_write_register(ViPipe,0x01,0x01);
    sp2308_write_register(ViPipe,0x19,0x50);
    sp2308_write_register(ViPipe,0x1b,0x1c);
    sp2308_write_register(ViPipe,0x1c,0x00);
    sp2308_write_register(ViPipe,0x2f,0x05);
    sp2308_write_register(ViPipe,0x38,0x00);
    sp2308_write_register(ViPipe,0x3c,0xca);
    sp2308_write_register(ViPipe,0x3d,0x4a);
    sp2308_write_register(ViPipe,0x40,0x3f);
    sp2308_write_register(ViPipe,0x43,0x3b);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0x51,0x0c);
    sp2308_write_register(ViPipe,0x52,0x09);
    sp2308_write_register(ViPipe,0x57,0x18);
    sp2308_write_register(ViPipe,0x5a,0x4f);
    sp2308_write_register(ViPipe,0x5e,0x0a);
    sp2308_write_register(ViPipe,0x66,0x24);
    sp2308_write_register(ViPipe,0x6e,0x20);
    sp2308_write_register(ViPipe,0x71,0x80);
    sp2308_write_register(ViPipe,0x73,0x2f);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0xbd,0xaa);
    sp2308_write_register(ViPipe,0xd7,0xaa);
    sp2308_write_register(ViPipe,0xd8,0xaa);
    sp2308_write_register(ViPipe,0xe8,0x0e);
    sp2308_write_register(ViPipe,0xe9,0x0e);
    sp2308_write_register(ViPipe,0xea,0x0d);
    sp2308_write_register(ViPipe,0xeb,0x0a);
    sp2308_write_register(ViPipe,0xed,0x7f);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0xf0,0x40);
    sp2308_write_register(ViPipe,0xf1,0x40);
    sp2308_write_register(ViPipe,0xf2,0x40);
    sp2308_write_register(ViPipe,0xf3,0x40);
    sp2308_write_register(ViPipe,0xf4,0x00);
    sp2308_write_register(ViPipe,0xfa,0x1c);
    sp2308_write_register(ViPipe,0xfb,0x6b);
    sp2308_write_register(ViPipe,0xf6,0xa0);
    sp2308_write_register(ViPipe,0xfc,0xff);
    sp2308_write_register(ViPipe,0xfe,0x01);
    sp2308_write_register(ViPipe,0xc4,0x7a);
    sp2308_write_register(ViPipe,0xc5,0x7a);
    sp2308_write_register(ViPipe,0xc6,0x7a);
    sp2308_write_register(ViPipe,0xc7,0x7a);
    sp2308_write_register(ViPipe,0xce,0x7c);
    sp2308_write_register(ViPipe,0xfd,0x03);
    sp2308_write_register(ViPipe,0x03,0xcf);
    sp2308_write_register(ViPipe,0x00,0x49);
    sp2308_write_register(ViPipe,0x04,0x35);
    sp2308_write_register(ViPipe,0x05,0x00);
    sp2308_write_register(ViPipe,0x06,0x06);
    sp2308_write_register(ViPipe,0x07,0x0e);
    sp2308_write_register(ViPipe,0x09,0x5f);
    sp2308_write_register(ViPipe,0x0b,0x0c);
    sp2308_write_register(ViPipe,0x0d,0x26);
    sp2308_write_register(ViPipe,0x0f,0x02);
    sp2308_write_register(ViPipe,0xfd,0x02);
    sp2308_write_register(ViPipe,0x34,0x80);
    //sp2308_write_register(ViPipe,0x5e,0x22);
    sp2308_write_register(ViPipe,0x5e,0x32); //mirror & flip, the first pixel is B.
    sp2308_write_register(ViPipe,0xa1,0x04);
    sp2308_write_register(ViPipe,0xa3,0x38);
    sp2308_write_register(ViPipe,0xa5,0x04);
    sp2308_write_register(ViPipe,0xa7,0x80);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0xc9,0x00);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0x8f,0x80);
    sp2308_write_register(ViPipe,0x91,0x38);
    sp2308_write_register(ViPipe,0xa1,0x05);
    sp2308_write_register(ViPipe,0x92,0x19);
    sp2308_write_register(ViPipe,0x94,0x44);
    sp2308_write_register(ViPipe,0x95,0x44);
    sp2308_write_register(ViPipe,0x96,0x09);
    sp2308_write_register(ViPipe,0x98,0x44);
    sp2308_write_register(ViPipe,0x9c,0x0e);
    sp2308_write_register(ViPipe,0x9d,0x01);
    sp2308_write_register(ViPipe,0x9e,0x5f);
    sp2308_write_register(ViPipe,0xb1,0x01);
    sp2308_write_register(ViPipe,0xa4,0x13);
    sp2308_write_register(ViPipe,0xa5,0xff);
    sp2308_write_register(ViPipe,0xa6,0xff);
    sp2308_write_register(ViPipe,0xfd,0x01);
    sp2308_write_register(ViPipe,0x01,0x02);
    sp2308_write_register(ViPipe,0xb1,0x03);
    printf("===SP2308 1080P 30fps 10bit LINE Init OK!===\n");

    return;
}
