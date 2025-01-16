/*
 * Copyright (c) GK. All rights reserved.
 */

#include "gc5603_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"

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
#include <linux/fb.h>
#endif

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

/* sensor fps mode */
#define GC5603_1620P_30FPS_LINEAR_MODE  0

GK_S32 gc5603_GetQuickStartFlag(VI_PIPE ViPipe, GK_U32 *quickStartFlag)
{
    GK_S32 s32Ret;
    ISP_MOD_PARAM_S stModParam;

    s32Ret = GK_API_ISP_GetModParam(&stModParam);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "gc5603 Get QuickStart Error\n");
        return s32Ret;
    }
    *quickStartFlag = stModParam.u32QuickStart;
    return GK_SUCCESS;
}

int gc5603_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[I2C_DEV_FILE_NUM] = {0};
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
    ISP_SNS_COMMBUS_U *gc5603businfo = GK_NULL;

    gc5603businfo = gc5603_get_bus_Info(ViPipe);
    u8DevNum = gc5603businfo->s8I2cDev;
    ret = snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);
    if (ret < 0) {
        return GK_FAILURE;
    }
    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (GC5603_I2C_ADDR >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int gc5603_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}


struct i2c_rdwr_ioctl_data {
    struct i2c_msg  *msgs;
    unsigned int nmsgs;
};
#define GC5603_I2C_MSG_NUM    2
#define GC5603_RECV_BUF_NUM   4
static GK_S32 gc5603_read_register_arecvbuf(VI_PIPE ViPipe, GK_U32 addr, GK_U8 *aRecvbuf)
{
    GK_S32 s32Ret;
    GK_U32 u32RegWidth = GC5603_ADDR_BYTE;
    GK_U32 u32DataWidth = GC5603_DATA_BYTE;

#ifdef LOSCFG_HOST_TYPE_VENDOR
    GK_U32 u32SnsI2cAddr = (GC5603_I2C_ADDR >> 1);
    struct i2c_rdwr_ioctl_data stRdwr;
    struct i2c_msg astMsg[GC5603_I2C_MSG_NUM];
    (gk_void)memset_s(&stRdwr, sizeof(stRdwr), 0x0, sizeof(stRdwr));
    (gk_void)memset_s(astMsg, sizeof(astMsg), 0x0, sizeof(astMsg));
#endif
    (gk_void)memset_s(aRecvbuf, 4 * sizeof(GK_U8), 0x0, 4 * sizeof(GK_U8)); /* 4,4 len of aRecvbuf */
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
    stRdwr.nmsgs = 2; /* RegWidth 2  */
#endif

#ifdef LOSCFG_HOST_TYPE_VENDOR
    if (u32RegWidth == 2) {  /* RegWidth 2  */
        aRecvbuf[0] = (addr >> 8) & 0xff;  /* shift 8 */
        aRecvbuf[1] = addr & 0xff;
    } else {
        aRecvbuf[0] = addr & 0xff;
    }
    s32Ret = ioctl(g_fd[ViPipe], I2C_RDWR, &stRdwr);
#else
    if (u32RegWidth == 2) {  /* RegWidth 2  */
        aRecvbuf[0] = addr & 0xff;
        aRecvbuf[1] = (addr >> 8) & 0xff;  /* shift 8 */
    } else {
        aRecvbuf[0] = addr & 0xff;
    }
    s32Ret = read(g_fd[ViPipe], aRecvbuf, u32RegWidth + u32DataWidth);
#endif
    return s32Ret;
}


int gc5603_read_register(VI_PIPE ViPipe, GK_S32 addr)
{
    GK_S32 s32RegVal;

    if (g_fd[ViPipe] < 0) {
        return GK_FAILURE;
    }
    GK_S32 s32Ret;

    GK_U32 u32DataWidth = GC5603_DATA_BYTE;
    GK_U8 aRecvbuf[GC5603_RECV_BUF_NUM];

    s32Ret = gc5603_read_register_arecvbuf(ViPipe, addr, &aRecvbuf[0]);
    if (s32Ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "gc5603_read_register failed!\n");
        return GK_FAILURE;
    }
    if (u32DataWidth == 2) {  /* RegWidth 2  */
        s32RegVal = aRecvbuf[0] | (aRecvbuf[1] << 8);  /* shift 8  */
    } else {
        s32RegVal = aRecvbuf[0];
    }
    return s32RegVal;
}

int gc5603_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = GC5603_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = GC5603_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = GC5603_DATA_BYTE;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    GK_U32 idx = 0;
    GK_S32 ret;
    GK_U8 buf[I2C_BUF_NUM];

    if (GC5603_ADDR_BYTE == 2) { /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (GC5603_DATA_BYTE == 2) { /* 2 byte */
        buf[idx] = (data >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, GC5603_ADDR_BYTE + GC5603_DATA_BYTE);
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }

#endif
    return GK_SUCCESS;
}

void gc5603_standby(VI_PIPE ViPipe)
{
    return;
}

void gc5603_restart(VI_PIPE ViPipe)
{
    return;
}

void gc5603_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{

    return;
}

void gc5603_linear_1620p30_init(VI_PIPE ViPipe);

void gc5603_init(VI_PIPE ViPipe)
{
    GK_U32 i;
    GK_U32 quickStartFlag = 0;
    GK_BOOL bInit;
    GK_S32  ret = GK_SUCCESS;

    ISP_SNS_STATE_S *pastgc5603 = GK_NULL;

    pastgc5603 = gc5603_get_ctx(ViPipe);
    bInit = pastgc5603->bInit;

    ret += gc5603_GetQuickStartFlag(ViPipe, &quickStartFlag);

    if (bInit == GK_FALSE) {
        /* 1. sensor i2c init */
        gc5603_i2c_init(ViPipe);
        if (ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "i2c init failed!\n");
            return;
        }
    }
    if (quickStartFlag == 0) {

                gc5603_linear_1620p30_init(ViPipe);

    }
    for (i = 0; i < pastgc5603->astRegsInfo[0].u32RegNum; i++) {
            ret += gc5603_write_register(ViPipe,
                                         pastgc5603->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                         pastgc5603->astRegsInfo[0].astI2cData[i].u32Data);
    }

    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "write register failed!\n");
        return;
    }

    pastgc5603->bInit = GK_TRUE;
    return;
}

void gc5603_exit(VI_PIPE ViPipe)
{
    GK_S32  ret;
    ret = gc5603_i2c_exit(ViPipe);
    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "i2c exit failed!\n");
        return;
    }
    return;
}

void gc5603_linear_1620p30_init(VI_PIPE ViPipe)
{
    GK_S32 ret = GK_SUCCESS;
	ret += gc5603_write_register(ViPipe, 0x03fe, 0xf0);
	ret += gc5603_write_register(ViPipe, 0x03fe, 0x00);
	ret += gc5603_write_register(ViPipe, 0x03fe, 0x10);
	ret += gc5603_write_register(ViPipe, 0x03fe, 0x00);
	ret += gc5603_write_register(ViPipe, 0x0a38, 0x02);
	ret += gc5603_write_register(ViPipe, 0x0a38, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0a20, 0x07);
	ret += gc5603_write_register(ViPipe, 0x061c, 0x50);
	ret += gc5603_write_register(ViPipe, 0x061d, 0x05);
	ret += gc5603_write_register(ViPipe, 0x061e, 0x70);
	ret += gc5603_write_register(ViPipe, 0x061f, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0a21, 0x08);
	ret += gc5603_write_register(ViPipe, 0x0a34, 0x40);
	ret += gc5603_write_register(ViPipe, 0x0a35, 0x51);
	ret += gc5603_write_register(ViPipe, 0x0a36, 0x5e);
	ret += gc5603_write_register(ViPipe, 0x0a37, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0314, 0x50);
	ret += gc5603_write_register(ViPipe, 0x0315, 0x32);
	ret += gc5603_write_register(ViPipe, 0x031c, 0xce);
	ret += gc5603_write_register(ViPipe, 0x0219, 0x47);
	ret += gc5603_write_register(ViPipe, 0x0342, 0x04);
	ret += gc5603_write_register(ViPipe, 0x0343, 0xb0);
	ret += gc5603_write_register(ViPipe, 0x0340, 0x06);
	ret += gc5603_write_register(ViPipe, 0x0341, 0xd6);
	ret += gc5603_write_register(ViPipe, 0x0345, 0x02);
	ret += gc5603_write_register(ViPipe, 0x0347, 0x02);
	ret += gc5603_write_register(ViPipe, 0x0348, 0x0b);
	ret += gc5603_write_register(ViPipe, 0x0349, 0x98);
	ret += gc5603_write_register(ViPipe, 0x034a, 0x06);
	ret += gc5603_write_register(ViPipe, 0x034b, 0x8a);
	ret += gc5603_write_register(ViPipe, 0x0094, 0x0b); //b40=2960
	ret += gc5603_write_register(ViPipe, 0x0095, 0x90);
	ret += gc5603_write_register(ViPipe, 0x0096, 0x06);
	ret += gc5603_write_register(ViPipe, 0x0097, 0x80);// 654=1620

	ret += gc5603_write_register(ViPipe, 0x0099, 0x04);
	ret += gc5603_write_register(ViPipe, 0x009b, 0x04);
	ret += gc5603_write_register(ViPipe, 0x060e, 0xd2);
	ret += gc5603_write_register(ViPipe, 0x060f, 0x05);
	ret += gc5603_write_register(ViPipe, 0x070e, 0xd2);
	ret += gc5603_write_register(ViPipe, 0x070f, 0x05);
	ret += gc5603_write_register(ViPipe, 0x0909, 0x07);
	ret += gc5603_write_register(ViPipe, 0x0902, 0x04);
	ret += gc5603_write_register(ViPipe, 0x0904, 0x0b);
	ret += gc5603_write_register(ViPipe, 0x0907, 0x54);
	ret += gc5603_write_register(ViPipe, 0x0908, 0x06);
	ret += gc5603_write_register(ViPipe, 0x0903, 0x9d);
	ret += gc5603_write_register(ViPipe, 0x072a, 0x1c);
	ret += gc5603_write_register(ViPipe, 0x072b, 0x1c);
	ret += gc5603_write_register(ViPipe, 0x0724, 0x2b);
	ret += gc5603_write_register(ViPipe, 0x0727, 0x2b);
	ret += gc5603_write_register(ViPipe, 0x1466, 0x18);
	ret += gc5603_write_register(ViPipe, 0x1467, 0x08);
	ret += gc5603_write_register(ViPipe, 0x1468, 0x10);
	ret += gc5603_write_register(ViPipe, 0x1469, 0x80);
	ret += gc5603_write_register(ViPipe, 0x146a, 0xe8);
	ret += gc5603_write_register(ViPipe, 0x0707, 0x07);
	ret += gc5603_write_register(ViPipe, 0x0737, 0x0f);
	ret += gc5603_write_register(ViPipe, 0x0704, 0x01);
	ret += gc5603_write_register(ViPipe, 0x0706, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0716, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0708, 0xc8);
	ret += gc5603_write_register(ViPipe, 0x0718, 0xc8);
	ret += gc5603_write_register(ViPipe, 0x061a, 0x02);
	ret += gc5603_write_register(ViPipe, 0x1430, 0x80);
	ret += gc5603_write_register(ViPipe, 0x1407, 0x10);
	ret += gc5603_write_register(ViPipe, 0x1408, 0x16);
	ret += gc5603_write_register(ViPipe, 0x1409, 0x03);
	ret += gc5603_write_register(ViPipe, 0x1438, 0x01);
	ret += gc5603_write_register(ViPipe, 0x02ce, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0245, 0xc9);
	ret += gc5603_write_register(ViPipe, 0x023a, 0x08);
	ret += gc5603_write_register(ViPipe, 0x02cd, 0x88);
	ret += gc5603_write_register(ViPipe, 0x0612, 0x02);
	ret += gc5603_write_register(ViPipe, 0x0613, 0xc7);
	ret += gc5603_write_register(ViPipe, 0x0243, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0089, 0x03);
	ret += gc5603_write_register(ViPipe, 0x0040, 0xa3);
	ret += gc5603_write_register(ViPipe, 0x0075, 0x64);
	ret += gc5603_write_register(ViPipe, 0x0004, 0x0e);
	ret += gc5603_write_register(ViPipe, 0x0053, 0x0a);
	ret += gc5603_write_register(ViPipe, 0x0205, 0x0c);
	ret += gc5603_write_register(ViPipe, 0x0181, 0x30);
	ret += gc5603_write_register(ViPipe, 0x0182, 0x05);
	ret += gc5603_write_register(ViPipe, 0x0185, 0x01);
	ret += gc5603_write_register(ViPipe, 0x0180, 0x46);
	ret += gc5603_write_register(ViPipe, 0x0100, 0x08);
	ret += gc5603_write_register(ViPipe, 0x010d, 0x74);
	ret += gc5603_write_register(ViPipe, 0x010e, 0x0e);
	ret += gc5603_write_register(ViPipe, 0x0113, 0x02);
	ret += gc5603_write_register(ViPipe, 0x0114, 0x01);
	ret += gc5603_write_register(ViPipe, 0x0115, 0x10);
	ret += gc5603_write_register(ViPipe, 0x0100, 0x09);
    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "write register failed!\n");
        return;
    }

    printf("=== Galaxycore GC5603_1620P_30FPS_10BIT_LINEAR_Init_OK!===\n");
    return;
}

