
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
#include "gc4023_cmos.h"

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
extern ISP_SNS_STATE_S   *g_pastGC4023[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U  g_aunGC4023_BusInfo[];

// sensor fps mode
#define GC4023_SENSOR_4M_30FPS_LINEAR_MODE   (0)   // 2560x1440

int GC4023_i2c_init(VI_PIPE ViPipe)
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

    u8DevNum = g_aunGC4023_BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (GC4023_I2C_ADDR >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int GC4023_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int GC4023_read_register(VI_PIPE ViPipe, GK_S32 addr)
{
    GK_S32 s32RegVal = 0;
    if (g_fd[ViPipe] < 0) {
        return GK_FAILURE;
    }

    GK_S32 s32Ret = 0;
    GK_U32 u32RegWidth = GC4023_ADDR_BYTE;
    GK_U32 u32DataWidth = GC4023_DATA_BYTE;
    GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
    GK_U32 u32SnsI2cAddr = (GC4023_I2C_ADDR >> 1);
    struct i2c_rdwr_ioctl_data stRdwr;
    struct i2c_msg astMsg[2];
    memset_s(&stRdwr, sizeof(stRdwr), 0x0, sizeof(stRdwr));
    memset_s(astMsg, sizeof(astMsg), 0x0, sizeof(astMsg));
#endif

    memset_s(aRecvbuf, sizeof(aRecvbuf), 0x0, sizeof(aRecvbuf));

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


int GC4023_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = GC4023_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = GC4023_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = GC4023_DATA_BYTE;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else

	int idx = 0;
	int ret;
	char buf[8];

    if (GC4023_ADDR_BYTE == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (GC4023_DATA_BYTE == 2) {
        buf[idx] = (data >> 8) & 0xff;
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, GC4023_ADDR_BYTE + GC4023_DATA_BYTE);
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }

#endif
    return GK_SUCCESS;
}

void GC4023_standby(VI_PIPE ViPipe)
{
    return;
}

void GC4023_restart(VI_PIPE ViPipe)
{
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void GC4023_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            GC4023_write_register(ViPipe, 0x0202,0x00);
			GC4023_write_register(ViPipe, 0x0a67,0x80);
			GC4023_write_register(ViPipe, 0x0a54,0x0e);
			GC4023_write_register(ViPipe, 0x0a65,0x10);
			GC4023_write_register(ViPipe, 0x0a98,0x10);
			GC4023_write_register(ViPipe, 0x05be,0x00);
			GC4023_write_register(ViPipe, 0x05a9,0x01);
			GC4023_write_register(ViPipe, 0x0029,0x08);
			GC4023_write_register(ViPipe, 0x002b,0xa8);
			GC4023_write_register(ViPipe, 0x0a83,0xe0);
			GC4023_write_register(ViPipe, 0x0a72,0x02);
			GC4023_write_register(ViPipe, 0x0a73,0x60);
			GC4023_write_register(ViPipe, 0x0a75,0x41);
			GC4023_write_register(ViPipe, 0x0a70,0x03);
			GC4023_write_register(ViPipe, 0x0a5a,0x80);
			delay_ms(20);
			GC4023_write_register(ViPipe, 0x05be,0x01);
			GC4023_write_register(ViPipe, 0x0a70,0x00);
			GC4023_write_register(ViPipe, 0x0080,0x02);
			GC4023_write_register(ViPipe, 0x0a67,0x00);
            break;

        case ISP_SNS_MIRROR:
            GC4023_write_register(ViPipe, 0x0202,0x01);
			GC4023_write_register(ViPipe, 0x0a67,0x80);
			GC4023_write_register(ViPipe, 0x0a54,0x0e);
			GC4023_write_register(ViPipe, 0x0a65,0x10);
			GC4023_write_register(ViPipe, 0x0a98,0x10);
			GC4023_write_register(ViPipe, 0x05be,0x00);
			GC4023_write_register(ViPipe, 0x05a9,0x01);
			GC4023_write_register(ViPipe, 0x0029,0x08);
			GC4023_write_register(ViPipe, 0x002b,0xa8);
			GC4023_write_register(ViPipe, 0x0a83,0xe0);
			GC4023_write_register(ViPipe, 0x0a72,0x02);
			GC4023_write_register(ViPipe, 0x0a73,0x61);
			GC4023_write_register(ViPipe, 0x0a75,0x41);
			GC4023_write_register(ViPipe, 0x0a70,0x03);
			GC4023_write_register(ViPipe, 0x0a5a,0x80);
			delay_ms(20);
			GC4023_write_register(ViPipe, 0x05be,0x01);
			GC4023_write_register(ViPipe, 0x0a70,0x00);
			GC4023_write_register(ViPipe, 0x0080,0x02);
			GC4023_write_register(ViPipe, 0x0a67,0x00);
            break;

        case ISP_SNS_FLIP:
            GC4023_write_register(ViPipe, 0x0202,0x02);
			GC4023_write_register(ViPipe, 0x0a67,0x80);
			GC4023_write_register(ViPipe, 0x0a54,0x0e);
			GC4023_write_register(ViPipe, 0x0a65,0x10);
			GC4023_write_register(ViPipe, 0x0a98,0x10);
			GC4023_write_register(ViPipe, 0x05be,0x00);
			GC4023_write_register(ViPipe, 0x05a9,0x01);
			GC4023_write_register(ViPipe, 0x0029,0x08);
			GC4023_write_register(ViPipe, 0x002b,0xa8);
			GC4023_write_register(ViPipe, 0x0a83,0xe0);
			GC4023_write_register(ViPipe, 0x0a72,0x02);
			GC4023_write_register(ViPipe, 0x0a73,0x62);
			GC4023_write_register(ViPipe, 0x0a75,0x41);
			GC4023_write_register(ViPipe, 0x0a70,0x03);
			GC4023_write_register(ViPipe, 0x0a5a,0x80);
			delay_ms(20);
			GC4023_write_register(ViPipe, 0x05be,0x01);
			GC4023_write_register(ViPipe, 0x0a70,0x00);
			GC4023_write_register(ViPipe, 0x0080,0x02);
			GC4023_write_register(ViPipe, 0x0a67,0x00);
            break;

        case ISP_SNS_MIRROR_FLIP:
            GC4023_write_register(ViPipe, 0x0202,0x03);
			GC4023_write_register(ViPipe, 0x0a67,0x80);
			GC4023_write_register(ViPipe, 0x0a54,0x0e);
			GC4023_write_register(ViPipe, 0x0a65,0x10);
			GC4023_write_register(ViPipe, 0x0a98,0x10);
			GC4023_write_register(ViPipe, 0x05be,0x00);
			GC4023_write_register(ViPipe, 0x05a9,0x01);
			GC4023_write_register(ViPipe, 0x0029,0x08);
			GC4023_write_register(ViPipe, 0x002b,0xa8);
			GC4023_write_register(ViPipe, 0x0a83,0xe0);
			GC4023_write_register(ViPipe, 0x0a72,0x02);
			GC4023_write_register(ViPipe, 0x0a73,0x63);
			GC4023_write_register(ViPipe, 0x0a75,0x41);
			GC4023_write_register(ViPipe, 0x0a70,0x03);
			GC4023_write_register(ViPipe, 0x0a5a,0x80);
			delay_ms(20);
			GC4023_write_register(ViPipe, 0x05be,0x01);
			GC4023_write_register(ViPipe, 0x0a70,0x00);
			GC4023_write_register(ViPipe, 0x0080,0x02);
			GC4023_write_register(ViPipe, 0x0a67,0x00);
            break;
    }

    return;
}

void GC4023_linear_4M30_init(VI_PIPE ViPipe);

void GC4023_init(VI_PIPE ViPipe)
{
    GK_U32       i;
    GK_U8        u8ImgMode;
    GK_BOOL      bInit;

    bInit       = g_pastGC4023[ViPipe]->bInit;
    u8ImgMode   = g_pastGC4023[ViPipe]->u8ImgMode;

    if (GK_FALSE == bInit) {
        /* sensor i2c init */
        printf("binit false GC4023 i2c init\n");
        GC4023_i2c_init(ViPipe);
    }

    switch (u8ImgMode) {
        case 0:
            GC4023_linear_4M30_init(ViPipe);
            break;
        default:
            printf("GC4023_SENSOR_CTL_Not support this mode\n");
            g_pastGC4023[ViPipe]->bInit = GK_FALSE;
            break;
    }

    for (i = 0; i < g_pastGC4023[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        GC4023_write_register(ViPipe, g_pastGC4023[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr,
                              g_pastGC4023[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }
    g_pastGC4023[ViPipe]->bInit = GK_TRUE;

    printf("GC4023 init succuss!\n");

    return;
}

void GC4023_exit(VI_PIPE ViPipe)
{
    GC4023_i2c_exit(ViPipe);
    g_bStandby[ViPipe] = GK_FALSE;

    return;
}

void GC4023_linear_4M30_init(VI_PIPE ViPipe)
{
    GC4023_write_register(ViPipe, 0x03fe, 0xf0);
    GC4023_write_register(ViPipe, 0x03fe, 0x00);
    GC4023_write_register(ViPipe, 0x03fe, 0x10);
    GC4023_write_register(ViPipe, 0x03fe, 0x00);
    GC4023_write_register(ViPipe, 0x0a38, 0x00);
    GC4023_write_register(ViPipe, 0x0a38, 0x01);
    GC4023_write_register(ViPipe, 0x0a20, 0x07);
    GC4023_write_register(ViPipe, 0x061c, 0x50);
    GC4023_write_register(ViPipe, 0x061d, 0x22);
    GC4023_write_register(ViPipe, 0x061e, 0x78);
    GC4023_write_register(ViPipe, 0x061f, 0x06);
    GC4023_write_register(ViPipe, 0x0a21, 0x10);
    GC4023_write_register(ViPipe, 0x0a34, 0x40);
    GC4023_write_register(ViPipe, 0x0a35, 0x01);
    GC4023_write_register(ViPipe, 0x0a36, 0x4e);
    GC4023_write_register(ViPipe, 0x0a37, 0x06);
    GC4023_write_register(ViPipe, 0x0314, 0x50);
    GC4023_write_register(ViPipe, 0x0315, 0x00);
    GC4023_write_register(ViPipe, 0x031c, 0xce);
    GC4023_write_register(ViPipe, 0x0219, 0x47);
    GC4023_write_register(ViPipe, 0x0342, 0x04);
    GC4023_write_register(ViPipe, 0x0343, 0xb0);
    GC4023_write_register(ViPipe, 0x0259, 0x05);
    GC4023_write_register(ViPipe, 0x025a, 0xa0);
    GC4023_write_register(ViPipe, 0x0340, 0x05);
    GC4023_write_register(ViPipe, 0x0341, 0xdc);
    GC4023_write_register(ViPipe, 0x0347, 0x02);
    GC4023_write_register(ViPipe, 0x0348, 0x0a);
    GC4023_write_register(ViPipe, 0x0349, 0x08);
    GC4023_write_register(ViPipe, 0x034a, 0x05);
    GC4023_write_register(ViPipe, 0x034b, 0xa8);
    GC4023_write_register(ViPipe, 0x0094, 0x0a);
    GC4023_write_register(ViPipe, 0x0095, 0x00);
    GC4023_write_register(ViPipe, 0x0096, 0x05);
    GC4023_write_register(ViPipe, 0x0097, 0xa0);
    GC4023_write_register(ViPipe, 0x0099, 0x04);
    GC4023_write_register(ViPipe, 0x009b, 0x04);
    GC4023_write_register(ViPipe, 0x060c, 0x01);
    GC4023_write_register(ViPipe, 0x060e, 0x08);
    GC4023_write_register(ViPipe, 0x060f, 0x05);
    GC4023_write_register(ViPipe, 0x070c, 0x01);
    GC4023_write_register(ViPipe, 0x070e, 0x08);
    GC4023_write_register(ViPipe, 0x070f, 0x05);
    GC4023_write_register(ViPipe, 0x0909, 0x03);
    GC4023_write_register(ViPipe, 0x0902, 0x04);
    GC4023_write_register(ViPipe, 0x0904, 0x0b);
    GC4023_write_register(ViPipe, 0x0907, 0x54);
    GC4023_write_register(ViPipe, 0x0908, 0x06);
    GC4023_write_register(ViPipe, 0x0903, 0x9d);
    GC4023_write_register(ViPipe, 0x072a, 0x18);
	GC4023_write_register(ViPipe, 0x0724, 0x0a);
	GC4023_write_register(ViPipe, 0x0727, 0x0a);
	GC4023_write_register(ViPipe, 0x072a,0x1c);
	GC4023_write_register(ViPipe, 0x072b,0x0a);
	GC4023_write_register(ViPipe, 0x1466,0x10);
	GC4023_write_register(ViPipe, 0x1468,0x0b);
	GC4023_write_register(ViPipe, 0x1467,0x13);
	GC4023_write_register(ViPipe, 0x1469,0x80);
	GC4023_write_register(ViPipe, 0x146a,0xe8);
	GC4023_write_register(ViPipe, 0x0707,0x07);
	GC4023_write_register(ViPipe, 0x0737,0x0f);
	GC4023_write_register(ViPipe, 0x0704,0x01);
	GC4023_write_register(ViPipe, 0x0706,0x03);
	GC4023_write_register(ViPipe, 0x0716,0x03);
	GC4023_write_register(ViPipe, 0x0708,0xc8);
	GC4023_write_register(ViPipe, 0x0718,0xc8);
	GC4023_write_register(ViPipe, 0x061a,0x02);
	GC4023_write_register(ViPipe, 0x1430,0x80);
	GC4023_write_register(ViPipe, 0x1407,0x10);
	GC4023_write_register(ViPipe, 0x1408,0x16);
	GC4023_write_register(ViPipe, 0x1409,0x03);
	GC4023_write_register(ViPipe, 0x146d,0x0e);
	GC4023_write_register(ViPipe, 0x146e,0x42);
	GC4023_write_register(ViPipe, 0x146f,0x43);
	GC4023_write_register(ViPipe, 0x1470,0x3c);
	GC4023_write_register(ViPipe, 0x1471,0x3d);
	GC4023_write_register(ViPipe, 0x1472,0x3a);
	GC4023_write_register(ViPipe, 0x1473,0x3a);
	GC4023_write_register(ViPipe, 0x1474,0x40);
	GC4023_write_register(ViPipe, 0x1475,0x46);
	GC4023_write_register(ViPipe, 0x1420,0x14);
	GC4023_write_register(ViPipe, 0x1464,0x15);
	GC4023_write_register(ViPipe, 0x146c,0x40);
	GC4023_write_register(ViPipe, 0x146d,0x40);
	GC4023_write_register(ViPipe, 0x1423,0x08);
	GC4023_write_register(ViPipe, 0x1428,0x10);
	GC4023_write_register(ViPipe, 0x1462,0x18);
	GC4023_write_register(ViPipe, 0x02ce,0x04);
	GC4023_write_register(ViPipe, 0x143a,0x0f);
	GC4023_write_register(ViPipe, 0x142b,0x88);
	GC4023_write_register(ViPipe, 0x0245,0xc9);
	GC4023_write_register(ViPipe, 0x023a,0x08);
	GC4023_write_register(ViPipe, 0x02cd,0x99);
	GC4023_write_register(ViPipe, 0x0612,0x02);
	GC4023_write_register(ViPipe, 0x0613,0xc7);
	GC4023_write_register(ViPipe, 0x0243,0x03);
	GC4023_write_register(ViPipe, 0x021b,0x09);
	GC4023_write_register(ViPipe, 0x0089,0x03);
	GC4023_write_register(ViPipe, 0x0040,0xa3);
	GC4023_write_register(ViPipe, 0x0075,0x64);
	GC4023_write_register(ViPipe, 0x0004,0x0f);
	GC4023_write_register(ViPipe, 0x0002,0xab);
	GC4023_write_register(ViPipe, 0x0053,0x0a);
	GC4023_write_register(ViPipe, 0x0205,0x0c);
	GC4023_write_register(ViPipe, 0x0202,0x06);
	GC4023_write_register(ViPipe, 0x0203,0x27);
	GC4023_write_register(ViPipe, 0x0614,0x00);
	GC4023_write_register(ViPipe, 0x0615,0x00);
	GC4023_write_register(ViPipe, 0x0181,0x0c);
	GC4023_write_register(ViPipe, 0x0182,0x05);
	GC4023_write_register(ViPipe, 0x0185,0x01);
	GC4023_write_register(ViPipe, 0x0180,0x46);
	GC4023_write_register(ViPipe, 0x0100,0x08);
	GC4023_write_register(ViPipe, 0x0106,0x38);
	GC4023_write_register(ViPipe, 0x010d,0x80);
	GC4023_write_register(ViPipe, 0x010e,0x0c);
	GC4023_write_register(ViPipe, 0x0113,0x02);
	GC4023_write_register(ViPipe, 0x0114,0x01);
	GC4023_write_register(ViPipe, 0x0115,0x10);
	GC4023_write_register(ViPipe, 0x022c,0x00);
	GC4023_write_register(ViPipe, 0x0100,0x09);
	GC4023_write_register(ViPipe, 0x0a67,0x80);
	GC4023_write_register(ViPipe, 0x0a54,0x0e);
	GC4023_write_register(ViPipe, 0x0a65,0x10);
	GC4023_write_register(ViPipe, 0x0a98,0x10);
	GC4023_write_register(ViPipe, 0x05be,0x00);
	GC4023_write_register(ViPipe, 0x05a9,0x01);
	GC4023_write_register(ViPipe, 0x0029,0x08);
	GC4023_write_register(ViPipe, 0x002b,0xa8);
	GC4023_write_register(ViPipe, 0x0a83,0xe0);
	GC4023_write_register(ViPipe, 0x0a72,0x02);
	GC4023_write_register(ViPipe, 0x0a73,0x60);
	GC4023_write_register(ViPipe, 0x0a75,0x41);
	GC4023_write_register(ViPipe, 0x0a70,0x03);
	GC4023_write_register(ViPipe, 0x0a5a,0x80);
	delay_ms(20);
	GC4023_write_register(ViPipe, 0x05be,0x01);
	GC4023_write_register(ViPipe, 0x0a70,0x00);
	GC4023_write_register(ViPipe, 0x0080,0x02);
	GC4023_write_register(ViPipe, 0x0a67,0x00);

    printf("-------Galaxycore GC4023_init_4M_2560x1440_10bit_linear30 Initial OK!-------\n");
}

