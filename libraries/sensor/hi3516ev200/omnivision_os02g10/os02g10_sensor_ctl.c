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
#include "os02g10_cmos.h"

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
#define OS02G10_SENSOR_1080p30_LINEAR_MODE   (0)

int os02g10_i2c_init(VI_PIPE ViPipe)
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

    ISP_SNS_COMMBUS_U *os02g10businfo = GK_NULL;

    os02g10businfo = os02g10_get_bus_Info(ViPipe);
    u8DevNum = os02g10businfo->s8I2cDev;
    ret = snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);
	//ret = snprintf(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);
    if (ret < 0) {
        return GK_FAILURE;
    }

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (OS02G10_I2C_ADDR >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int os02g10_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int os02g10_read_register(VI_PIPE ViPipe, GK_S32 addr)
{
    GK_S32 s32RegVal = 0;
    if (g_fd[ViPipe] < 0) {
        return GK_FAILURE;
    }

    GK_S32 s32Ret = 0;
    GK_U32 u32RegWidth = OS02G10_ADDR_BYTE;
    GK_U32 u32DataWidth = OS02G10_DATA_BYTE;
    GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
    GK_U32 u32SnsI2cAddr = (OS02G10_I2C_ADDR >> 1);
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
        ISP_TRACE(MODULE_DBG_ERR, "os02g10_read_register faild!\n");
        return GK_FAILURE;
    }

    if (u32DataWidth == 2) {
        s32RegVal = aRecvbuf[0] | (aRecvbuf[1] << 8);
    } else {
        s32RegVal = aRecvbuf[0];
    }
    return s32RegVal;
}


int os02g10_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = OS02G10_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OS02G10_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OS02G10_DATA_BYTE;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else

	int idx = 0;
	int ret;
	char buf[8];

    if (OS02G10_ADDR_BYTE == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (OS02G10_DATA_BYTE == 2) {
        buf[idx] = (data >> 8) & 0xff;
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, OS02G10_ADDR_BYTE + OS02G10_DATA_BYTE);
	if(addr != 0x20){
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error======%d======%d!\n", addr, data);
        return GK_FAILURE;
    	}
	}

#endif
    return GK_SUCCESS;
}

void os02g10_standby(VI_PIPE ViPipe)
{
    return;
}

void os02g10_restart(VI_PIPE ViPipe)
{
    return;
}

void os02g10_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    int mirror_flip = os02g10_read_register(ViPipe,0x3f);
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
    os02g10_write_register(ViPipe,0xfd,0x01);
    os02g10_write_register(ViPipe,0x3f,mirror_flip);
    os02g10_write_register(ViPipe,0x01,0x01);
    return;
}

void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void os02g10_linear_1080p30_init(VI_PIPE ViPipe);

void os02g10_init(VI_PIPE ViPipe)
{
    GK_U32           i;
    GK_U8            u8ImgMode;
    GK_BOOL          bInit;

    ISP_SNS_STATE_S *pastOs02g10 = GK_NULL;
    pastOs02g10 = os02g10_get_ctx(ViPipe);
    bInit      = pastOs02g10->bInit;
    u8ImgMode  = pastOs02g10->u8ImgMode;

    if (GK_FALSE == bInit) {
        /* sensor i2c init */
        // printf("binit false OS02G10 i2c init\n");
        os02g10_i2c_init(ViPipe);
    }

    switch (u8ImgMode) {
        case OS02G10_SENSOR_1080p30_LINEAR_MODE:
			// printf("os02g10_init======OS02G10_SENSOR_1080p30_LINEAR_MODE\n");
            os02g10_linear_1080p30_init(ViPipe);
            break;
        default:
            printf("OS02G10_SENSOR_CTL_Not support this mode\n");
            pastOs02g10->bInit = GK_FALSE;
            break;
    }

    for (i = 0; i < pastOs02g10->astRegsInfo[0].u32RegNum; i++) {
        os02g10_write_register(ViPipe, pastOs02g10->astRegsInfo[0].astI2cData[i].u32RegAddr,
                              pastOs02g10->astRegsInfo[0].astI2cData[i].u32Data);
    }
    pastOs02g10->bInit = GK_TRUE;


    return;
}

void os02g10_exit(VI_PIPE ViPipe)
{
    os02g10_i2c_exit(ViPipe);
    g_bStandby[ViPipe] = GK_FALSE;

    return;
}


/* 1080P30 and 1080P25 */
void os02g10_linear_1080p30_init(VI_PIPE ViPipe)
{
	os02g10_write_register(ViPipe,0Xfd,0X00);
	os02g10_write_register(ViPipe,0X36,0X01);
	os02g10_write_register(ViPipe,0Xfd,0X00);
	os02g10_write_register(ViPipe,0X36,0X00);
	os02g10_write_register(ViPipe,0Xfd,0X00);

	//printf("os02g10_write_register(ViPipe,0X20,0X00)======begin\n");
	//os02g10_write_register(ViPipe,0X20,0X00);
	//printf("os02g10_write_register(ViPipe,0X20,0X00)======after\n");
	//delay_ms(5);

	os02g10_write_register(ViPipe,0Xfd,0X00);
	os02g10_write_register(ViPipe,0Xfd,0X00);
	os02g10_write_register(ViPipe,0X30,0X0a);
	os02g10_write_register(ViPipe,0X35,0X04);
	os02g10_write_register(ViPipe,0X38,0X11);
	os02g10_write_register(ViPipe,0X41,0X06);
	os02g10_write_register(ViPipe,0X44,0X20);
	os02g10_write_register(ViPipe,0Xfd,0X01);
	os02g10_write_register(ViPipe,0X03,0X04);
	os02g10_write_register(ViPipe,0X04,0X4c);
	os02g10_write_register(ViPipe,0X06,0X00);
	os02g10_write_register(ViPipe,0X24,0X30);
	os02g10_write_register(ViPipe,0X01,0X01);
	os02g10_write_register(ViPipe,0X19,0X50);
	os02g10_write_register(ViPipe,0X1a,0X0c);
	os02g10_write_register(ViPipe,0X1b,0X0d);
	os02g10_write_register(ViPipe,0X1c,0X00);
	os02g10_write_register(ViPipe,0X1d,0X75);
	os02g10_write_register(ViPipe,0X1e,0X52);
	os02g10_write_register(ViPipe,0X22,0X14);
	os02g10_write_register(ViPipe,0X25,0X44);
	os02g10_write_register(ViPipe,0X26,0X0f);
	os02g10_write_register(ViPipe,0X3c,0Xca);
	os02g10_write_register(ViPipe,0X3d,0X4a);
	os02g10_write_register(ViPipe,0X40,0X0f);
	os02g10_write_register(ViPipe,0X43,0X38);
	os02g10_write_register(ViPipe,0X46,0X00);
	os02g10_write_register(ViPipe,0X47,0X00);
	os02g10_write_register(ViPipe,0X49,0X32);
	os02g10_write_register(ViPipe,0X50,0X01);
	os02g10_write_register(ViPipe,0X51,0X28);
	os02g10_write_register(ViPipe,0X52,0X20);
	os02g10_write_register(ViPipe,0X53,0X03);
	os02g10_write_register(ViPipe,0X57,0X16);
	os02g10_write_register(ViPipe,0X59,0X01);
	os02g10_write_register(ViPipe,0X5a,0X01);
	os02g10_write_register(ViPipe,0X5d,0X04);
	os02g10_write_register(ViPipe,0X6a,0X04);
	os02g10_write_register(ViPipe,0X6b,0X03);
	os02g10_write_register(ViPipe,0X6e,0X28);
	os02g10_write_register(ViPipe,0X71,0Xbe);
	os02g10_write_register(ViPipe,0X72,0X06);
	os02g10_write_register(ViPipe,0X73,0X38);
	os02g10_write_register(ViPipe,0X74,0X06);
	os02g10_write_register(ViPipe,0X79,0X00);
	os02g10_write_register(ViPipe,0X7a,0Xb2);
	os02g10_write_register(ViPipe,0X7b,0X10);
	os02g10_write_register(ViPipe,0X8f,0X80);
	os02g10_write_register(ViPipe,0X91,0X38);
	os02g10_write_register(ViPipe,0X92,0X02);
	os02g10_write_register(ViPipe,0X9d,0X03);
	os02g10_write_register(ViPipe,0X9e,0X55);
	os02g10_write_register(ViPipe,0Xb8,0X70);
	os02g10_write_register(ViPipe,0Xb9,0X70);
	os02g10_write_register(ViPipe,0Xba,0X70);
	os02g10_write_register(ViPipe,0Xbb,0X70);
	os02g10_write_register(ViPipe,0Xbc,0X00);
	os02g10_write_register(ViPipe,0Xc0,0X00);
	os02g10_write_register(ViPipe,0Xc1,0X00);
	os02g10_write_register(ViPipe,0Xc2,0X00);
	os02g10_write_register(ViPipe,0Xc3,0X00);
	os02g10_write_register(ViPipe,0Xc4,0X6e);
	os02g10_write_register(ViPipe,0Xc5,0X6e);
	os02g10_write_register(ViPipe,0Xc6,0X6b);
	os02g10_write_register(ViPipe,0Xc7,0X6b);
	os02g10_write_register(ViPipe,0Xcc,0X11);
	os02g10_write_register(ViPipe,0Xcd,0Xe0);
	os02g10_write_register(ViPipe,0Xd0,0X1b);
	os02g10_write_register(ViPipe,0Xd2,0X76);
	os02g10_write_register(ViPipe,0Xd3,0X68); //mirror & flip, the first pixel is B.
	os02g10_write_register(ViPipe,0Xd4,0X68);
	os02g10_write_register(ViPipe,0Xd5,0X73);
	os02g10_write_register(ViPipe,0Xd6,0X73);
	os02g10_write_register(ViPipe,0Xe8,0X55);
	os02g10_write_register(ViPipe,0Xf0,0X40);
	os02g10_write_register(ViPipe,0Xf1,0X40);
	os02g10_write_register(ViPipe,0Xf2,0X40);
	os02g10_write_register(ViPipe,0Xf3,0X40);
	os02g10_write_register(ViPipe,0Xf4,0X00);
	os02g10_write_register(ViPipe,0Xfa,0X1c);
	os02g10_write_register(ViPipe,0Xfb,0X33);
	os02g10_write_register(ViPipe,0Xfc,0Xff);
	os02g10_write_register(ViPipe,0Xfe,0X01);
	os02g10_write_register(ViPipe,0Xfd,0X03);
	os02g10_write_register(ViPipe,0X03,0X67);
	os02g10_write_register(ViPipe,0X00,0X59);
	os02g10_write_register(ViPipe,0X04,0X11);
	os02g10_write_register(ViPipe,0X05,0X04);
	os02g10_write_register(ViPipe,0X06,0X0c);
	os02g10_write_register(ViPipe,0X07,0X08);
	os02g10_write_register(ViPipe,0X08,0X08);
	os02g10_write_register(ViPipe,0X09,0X4f);
	os02g10_write_register(ViPipe,0X0b,0X08);
	os02g10_write_register(ViPipe,0X0d,0X26);
	os02g10_write_register(ViPipe,0X0f,0X00);
	os02g10_write_register(ViPipe,0Xfd,0X02);
	os02g10_write_register(ViPipe,0X34,0Xfe);
	os02g10_write_register(ViPipe,0X5e,0X32);	//mirror & flip
	os02g10_write_register(ViPipe,0Xa1,0X06);
	os02g10_write_register(ViPipe,0Xa3,0X38);
	os02g10_write_register(ViPipe,0Xa5,0X02);
	os02g10_write_register(ViPipe,0Xa7,0X80);
	os02g10_write_register(ViPipe,0Xfd,0X01);
	os02g10_write_register(ViPipe,0Xa1,0X05);
	os02g10_write_register(ViPipe,0X94,0X44);
	os02g10_write_register(ViPipe,0X95,0X44);
	os02g10_write_register(ViPipe,0X96,0X09);
	os02g10_write_register(ViPipe,0X98,0X44);
	os02g10_write_register(ViPipe,0X9c,0X0e);
	os02g10_write_register(ViPipe,0Xb1,0X01);
	os02g10_write_register(ViPipe,0Xfd,0X01);
	//os02g10_write_register(ViPipe,0X3f,0X03);
	os02g10_write_register(ViPipe,0Xb1,0X03);

	printf("===os02g10 1080P 30fps 10bit LINE Init OK!===\n");

return;
}
