/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
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

const unsigned char f37_i2c_addr   = 0x80; /* Sensor I2C Address */
const unsigned int  f37_addr_byte  = 1;
const unsigned int  f37_data_byte  = 1;
static int  g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S     *g_pastF37[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U    g_aunF37BusInfo[];

GK_S32 f37_GetQuickStartFlag(VI_PIPE ViPipe, GK_U32 *quickStartFlag)
{
    GK_S32 s32Ret = 0;
    ISP_MOD_PARAM_S stModParam;
    s32Ret = GK_API_ISP_GetModParam(&stModParam);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "f37 Get QuickStart Error\n");
        return s32Ret;
    }

    *quickStartFlag = stModParam.u32QuickStart;

    return GK_SUCCESS;
}

int f37_i2c_init(VI_PIPE ViPipe)
{
    char    acDevFile[16] = {0};
    GK_U8   u8DevNum;

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

    u8DevNum = g_aunF37BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (f37_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int f37_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }

    return GK_FAILURE;
}

int f37_read_register(VI_PIPE ViPipe, int addr)
{
    GK_S32 s32RegVal = 0;
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "f37 fd not opened!\n");
        return GK_FAILURE;
    }

    GK_S32 s32Ret = 0;
    GK_U32 u32RegWidth = f37_addr_byte;
    GK_U32 u32DataWidth = f37_data_byte;
    GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
    GK_U32 u32SnsI2cAddr = (f37_i2c_addr >> 1);
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

int f37_write_register(VI_PIPE ViPipe, int addr, int data)
{
    if (0 > g_fd[ViPipe]) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr       = f37_i2c_addr;
    i2c_data.reg_addr       = addr;
    i2c_data.addr_byte_num  = f37_addr_byte;
    i2c_data.data           = data;
    i2c_data.data_byte_num  = f37_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int   idx = 0;
    int   ret;
    char  buf[8] = {0};

    if (f37_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (f37_data_byte == 2) {
        buf[idx] = (data >> 8) & 0xff;
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, f37_addr_byte + f37_data_byte);
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }
#endif

    return GK_SUCCESS;
}

void f37_standby(VI_PIPE ViPipe)
{
    return;
}

void f37_restart(VI_PIPE ViPipe)
{
    return;
}

#define   F37_1080P_30FPS_LINEAR_MODE  (1)

void f37_linear_1080p30_init(VI_PIPE ViPipe);

void f37_init(VI_PIPE ViPipe)
{
    GK_U32 quickStartFlag = 0;
    f37_GetQuickStartFlag(ViPipe, &quickStartFlag);

    f37_i2c_init(ViPipe);

    if (quickStartFlag == 0) {
        f37_linear_1080p30_init(ViPipe);
    }

    g_pastF37[ViPipe]->bInit = GK_TRUE;

    return;
}

void f37_exit(VI_PIPE ViPipe)
{
    f37_i2c_exit(ViPipe);

    return;
}

void f37_linear_1080p30_init(VI_PIPE ViPipe)
{
    f37_write_register( ViPipe, 0x12, 0x60 );
    f37_write_register( ViPipe, 0x48, 0x8A );
    f37_write_register( ViPipe, 0x48, 0x0A );
    f37_write_register( ViPipe, 0x0E, 0x11 );
    f37_write_register( ViPipe, 0x0F, 0x14 );
    f37_write_register( ViPipe, 0x10, 0x36 );
    f37_write_register( ViPipe, 0x11, 0x80 );
    f37_write_register( ViPipe, 0x0D, 0xF0 );
    f37_write_register( ViPipe, 0x5F, 0x41 );
    f37_write_register( ViPipe, 0x60, 0x20 );
    f37_write_register( ViPipe, 0x58, 0x12 );
    f37_write_register( ViPipe, 0x57, 0x60 );
    f37_write_register( ViPipe, 0x9D, 0x00 );
    f37_write_register( ViPipe, 0x20, 0x80 );
    f37_write_register( ViPipe, 0x21, 0x07 );
    f37_write_register( ViPipe, 0x22, 0x65 );
    f37_write_register( ViPipe, 0x23, 0x04 );
    f37_write_register( ViPipe, 0x24, 0xC0 );
    f37_write_register( ViPipe, 0x25, 0x38 );
    f37_write_register( ViPipe, 0x26, 0x43 );
    f37_write_register( ViPipe, 0x27, 0x1A );
    f37_write_register( ViPipe, 0x28, 0x15 );
    f37_write_register( ViPipe, 0x29, 0x07 );
    f37_write_register( ViPipe, 0x2A, 0x0A );
    f37_write_register( ViPipe, 0x2B, 0x17 );
    f37_write_register( ViPipe, 0x2C, 0x00 );
    f37_write_register( ViPipe, 0x2D, 0x00 );
    f37_write_register( ViPipe, 0x2E, 0x16 );
    f37_write_register( ViPipe, 0x2F, 0x44 );
    f37_write_register( ViPipe, 0x41, 0xC8 );
    f37_write_register( ViPipe, 0x42, 0x3B );
    f37_write_register( ViPipe, 0x47, 0x42 );
    f37_write_register( ViPipe, 0x76, 0x60 );
    f37_write_register( ViPipe, 0x77, 0x09 );
    f37_write_register( ViPipe, 0x1D, 0x00 );
    f37_write_register( ViPipe, 0x1E, 0x04 );
    f37_write_register( ViPipe, 0x6C, 0x40 );
    f37_write_register( ViPipe, 0x6E, 0x2C );
    f37_write_register( ViPipe, 0x70, 0xDC );
    f37_write_register( ViPipe, 0x71, 0xD3 );
    f37_write_register( ViPipe, 0x72, 0xD4 );
    f37_write_register( ViPipe, 0x73, 0x58 );
    f37_write_register( ViPipe, 0x74, 0x02 );
    f37_write_register( ViPipe, 0x78, 0x96 );
    f37_write_register( ViPipe, 0x89, 0x01 );
    f37_write_register( ViPipe, 0x6B, 0x20 );
    f37_write_register( ViPipe, 0x86, 0x40 );
    f37_write_register( ViPipe, 0x31, 0x0C );
    f37_write_register( ViPipe, 0x32, 0x38 );
    f37_write_register( ViPipe, 0x33, 0x6C );
    f37_write_register( ViPipe, 0x34, 0x88 );
    f37_write_register( ViPipe, 0x35, 0x88 );
    f37_write_register( ViPipe, 0x3A, 0xAF );
    f37_write_register( ViPipe, 0x3B, 0x00 );
    f37_write_register( ViPipe, 0x3C, 0x57 );
    f37_write_register( ViPipe, 0x3D, 0x78 );
    f37_write_register( ViPipe, 0x3E, 0xFF );
    f37_write_register( ViPipe, 0x3F, 0xF8 );
    f37_write_register( ViPipe, 0x40, 0xFF );
    f37_write_register( ViPipe, 0x56, 0xB2 );
    f37_write_register( ViPipe, 0x59, 0xE8 );
    f37_write_register( ViPipe, 0x5A, 0x04 );
    f37_write_register( ViPipe, 0x85, 0x70 );
    f37_write_register( ViPipe, 0x8A, 0x04 );
    f37_write_register( ViPipe, 0x91, 0x13 );
    f37_write_register( ViPipe, 0x9B, 0x03 );
    f37_write_register( ViPipe, 0x9C, 0xE1 );
    f37_write_register( ViPipe, 0xA9, 0x78 );
    f37_write_register( ViPipe, 0x5B, 0xB0 );
    f37_write_register( ViPipe, 0x5C, 0x71 );
    f37_write_register( ViPipe, 0x5D, 0x46 );
    f37_write_register( ViPipe, 0x5E, 0x14 );
    f37_write_register( ViPipe, 0x62, 0x01 );
    f37_write_register( ViPipe, 0x63, 0x0F );
    f37_write_register( ViPipe, 0x64, 0xC0 );
    f37_write_register( ViPipe, 0x65, 0x02 );
    f37_write_register( ViPipe, 0x67, 0x65 );
    f37_write_register( ViPipe, 0x66, 0x04 );
    f37_write_register( ViPipe, 0x68, 0x00 );
    f37_write_register( ViPipe, 0x69, 0x7C );
    f37_write_register( ViPipe, 0x6A, 0x12 );
    f37_write_register( ViPipe, 0x7A, 0x80 );
    f37_write_register( ViPipe, 0x82, 0x21 );
    f37_write_register( ViPipe, 0x8F, 0x91 );
    f37_write_register( ViPipe, 0xAE, 0x30 );
    f37_write_register( ViPipe, 0x13, 0x81 );
    f37_write_register( ViPipe, 0x96, 0x04 );
    f37_write_register( ViPipe, 0x4A, 0x05 );
    f37_write_register( ViPipe, 0x7E, 0xCD );
    f37_write_register( ViPipe, 0x50, 0x02 );
    f37_write_register( ViPipe, 0x49, 0x10 );
    f37_write_register( ViPipe, 0xAF, 0x12 );
    f37_write_register( ViPipe, 0x80, 0x41 );
    f37_write_register( ViPipe, 0x7B, 0x4A );
    f37_write_register( ViPipe, 0x7C, 0x08 );
    f37_write_register( ViPipe, 0x7F, 0x57 );
    f37_write_register( ViPipe, 0x90, 0x00 );
    f37_write_register( ViPipe, 0x8C, 0xFF );
    f37_write_register( ViPipe, 0x8D, 0xC7 );
    f37_write_register( ViPipe, 0x8E, 0x00 );
    f37_write_register( ViPipe, 0x8B, 0x01 );
    f37_write_register( ViPipe, 0x0C, 0x00 );
    f37_write_register( ViPipe, 0x81, 0x74 );
    f37_write_register( ViPipe, 0x19, 0x20 );
    f37_write_register( ViPipe, 0x46, 0x00 );
    f37_write_register( ViPipe, 0x12, 0x10 );
    f37_write_register( ViPipe, 0x48, 0x8A );
    f37_write_register( ViPipe, 0x48, 0x0A );

    printf("================================================================\n");
    printf("==== SOI F37 24MInput 2lane 1080P30fps(MIPI) init success!======\n");
    printf("================================================================\n");
    return;
}

