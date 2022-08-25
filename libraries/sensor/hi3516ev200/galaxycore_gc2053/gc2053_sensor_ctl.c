/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#ifdef __LITEOS__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif
#include "gk_api_isp.h"
const unsigned char gc2053_i2c_addr = 0x6e; /* I2C Address of GC2053 */
const unsigned int gc2053_addr_byte = 1;
const unsigned int gc2053_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };
static GK_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = { 0 };

extern ISP_SNS_STATE_S *g_pastGc2053[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunGc2053BusInfo[];

// sensor fps mode
#define GC2053_SENSOR_1080P_30FPS_LINEAR_MODE (0)
GK_S32 gc2053_GetQuickStartFlag(VI_PIPE ViPipe, GK_U32 *quickStartFlag)
{
	GK_S32 s32Ret = 0;
	ISP_MOD_PARAM_S stModParam;
	s32Ret = GK_API_ISP_GetModParam(&stModParam);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "gc2053 Get QuickStart Error\n");
		return s32Ret;
	}
	*quickStartFlag = stModParam.u32QuickStart;
	return GK_SUCCESS;
}

int gc2053_i2c_init(VI_PIPE ViPipe)
{
	char acDevFile[16] = { 0 };
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

	u8DevNum = g_aunGc2053BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (gc2053_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int gc2053_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int gc2053_read_register(VI_PIPE ViPipe, int addr)
{
	GK_S32 s32RegVal = 0;
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "gc2053_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret = 0;
	GK_U32 u32RegWidth = gc2053_addr_byte;
	GK_U32 u32DataWidth = gc2053_data_byte;
	GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (gc2053_i2c_addr >> 1);
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

int gc2053_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = gc2053_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = gc2053_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = gc2053_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (gc2053_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (gc2053_data_byte == 2) {
		buf[idx] = (data >> 8) & 0xff;
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, gc2053_addr_byte + gc2053_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void gc2053_standby(VI_PIPE ViPipe)
{
	gc2053_write_register(ViPipe, 0xf2, 0x01);
	gc2053_write_register(ViPipe, 0xfc, 0x8f);
	g_bStandby[ViPipe] = GK_TRUE;
	return;
}

void gc2053_restart(VI_PIPE ViPipe)
{
	gc2053_write_register(ViPipe, 0xf2, 0x00);
	gc2053_write_register(ViPipe, 0xfc, 0x8e);
	g_bStandby[ViPipe] = GK_FALSE;
	return;
}

void gc2053_mirror_flip(VI_PIPE ViPipe,
			ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
	switch (eSnsMirrorFlip) {
	default:
	case ISP_SNS_NORMAL:
		gc2053_write_register(ViPipe, 0x17, 0x80);
		break;

	case ISP_SNS_MIRROR:
		gc2053_write_register(ViPipe, 0x17, 0x81);
		break;

	case ISP_SNS_FLIP:
		gc2053_write_register(ViPipe, 0x17, 0x82);
		break;

	case ISP_SNS_MIRROR_FLIP:
		gc2053_write_register(ViPipe, 0x17, 0x83);
		break;
	}

	return;
}

void gc2053_linear_1080p30_init(VI_PIPE ViPipe);
void gc2053_FSWDR_1080p30_init(VI_PIPE ViPipe);

void gc2053_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	GK_U8 u8ImgMode;
	GK_U32 quickStartFlag = 0;
	GK_BOOL bInit;

	bInit = g_pastGc2053[ViPipe]->bInit;
	u8ImgMode = g_pastGc2053[ViPipe]->u8ImgMode;
	gc2053_GetQuickStartFlag(ViPipe, &quickStartFlag);
	if (bInit == GK_FALSE) {
		/* 1. sensor i2c init */
		gc2053_i2c_init(ViPipe);
	}
	if (quickStartFlag == 0) {
		switch (u8ImgMode) {
		case 0:
			gc2053_linear_1080p30_init(ViPipe);
			break;
		case 1:
			gc2053_FSWDR_1080p30_init(ViPipe);
			break;

		default:
			printf("GC2053_SENSOR_CTL_Not support this mode\n");
			g_pastGc2053[ViPipe]->bInit = GK_FALSE;
			break;
		}
		for (i = 0; i < g_pastGc2053[ViPipe]->astRegsInfo[0].u32RegNum;
		     i++) {
			gc2053_write_register(ViPipe,
					      g_pastGc2053[ViPipe]
						      ->astRegsInfo[0]
						      .astI2cData[i]
						      .u32RegAddr,
					      g_pastGc2053[ViPipe]
						      ->astRegsInfo[0]
						      .astI2cData[i]
						      .u32Data);
		}
	}
	g_pastGc2053[ViPipe]->bInit = GK_TRUE;
	return;
}

void gc2053_exit(VI_PIPE ViPipe)
{
	gc2053_i2c_exit(ViPipe);
	g_bStandby[ViPipe] = GK_FALSE;

	return;
}

void gc2053_linear_1080p30_init(VI_PIPE ViPipe)
{
	gc2053_write_register(ViPipe, 0xfe, 0x80);
	gc2053_write_register(ViPipe, 0xfe, 0x80);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0xf2, 0x00);
	gc2053_write_register(ViPipe, 0xf3, 0x00);
	gc2053_write_register(ViPipe, 0xf4, 0x36);
	gc2053_write_register(ViPipe, 0xf5, 0xc0);
	gc2053_write_register(ViPipe, 0xf6, 0x44);
	gc2053_write_register(ViPipe, 0xf7, 0x01);
	gc2053_write_register(ViPipe, 0xf8, 0x2c);
	gc2053_write_register(ViPipe, 0xf9, 0x42);
	gc2053_write_register(ViPipe, 0xfc, 0x8e);

	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0x87, 0x18);
	gc2053_write_register(ViPipe, 0xee, 0x30);
	gc2053_write_register(ViPipe, 0xd0, 0xb7);
	gc2053_write_register(ViPipe, 0x03, 0x04);
	gc2053_write_register(ViPipe, 0x04, 0x60);
	gc2053_write_register(ViPipe, 0x05, 0x04);
	gc2053_write_register(ViPipe, 0x06, 0x4c);
	gc2053_write_register(ViPipe, 0x07, 0x00);
	gc2053_write_register(ViPipe, 0x08, 0x11);
	gc2053_write_register(ViPipe, 0x0a, 0x02);
	gc2053_write_register(ViPipe, 0x0c, 0x02);
	gc2053_write_register(ViPipe, 0x0d, 0x04);
	gc2053_write_register(ViPipe, 0x0e, 0x40);
	gc2053_write_register(ViPipe, 0xfe, 0x01);
	gc2053_write_register(ViPipe, 0x83, 0x01);
	gc2053_write_register(ViPipe, 0x87, 0x51);
	gc2053_write_register(ViPipe, 0xfe, 0x03);
	gc2053_write_register(ViPipe, 0x01, 0x27);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0x3e, 0x91);
	gc2053_write_register(ViPipe, 0x12, 0xe2);
	gc2053_write_register(ViPipe, 0x13, 0x16);
	gc2053_write_register(ViPipe, 0x19, 0x0a);
	gc2053_write_register(ViPipe, 0x28, 0x0a);
	gc2053_write_register(ViPipe, 0x2b, 0x04);
	gc2053_write_register(ViPipe, 0x37, 0x03);
	gc2053_write_register(ViPipe, 0x43, 0x07);
	gc2053_write_register(ViPipe, 0x44, 0x40);
	gc2053_write_register(ViPipe, 0x46, 0x0b);
	gc2053_write_register(ViPipe, 0x4b, 0x20);
	gc2053_write_register(ViPipe, 0x4e, 0x08);
	gc2053_write_register(ViPipe, 0x55, 0x20);
	gc2053_write_register(ViPipe, 0x77, 0x01);
	gc2053_write_register(ViPipe, 0x78, 0x00);
	gc2053_write_register(ViPipe, 0x7c, 0x93);
	gc2053_write_register(ViPipe, 0x8d, 0x92);
	gc2053_write_register(ViPipe, 0x90, 0x00);
	gc2053_write_register(ViPipe, 0x41, 0x04);
	gc2053_write_register(ViPipe, 0x42, 0x65);
	gc2053_write_register(ViPipe, 0xce, 0x7c);
	gc2053_write_register(ViPipe, 0xd2, 0x41);
	gc2053_write_register(ViPipe, 0xd3, 0xdc);
	gc2053_write_register(ViPipe, 0xe6, 0x50);
	gc2053_write_register(ViPipe, 0xb6, 0xc0);
	gc2053_write_register(ViPipe, 0xb0, 0x70);
	gc2053_write_register(ViPipe, 0x26, 0x30);
	gc2053_write_register(ViPipe, 0xfe, 0x01);
	gc2053_write_register(ViPipe, 0x55, 0x07);
	//dither
	gc2053_write_register(ViPipe, 0x58, 0x00); //default 0x80
	gc2053_write_register(ViPipe, 0x04, 0x00);
	gc2053_write_register(ViPipe, 0x94, 0x03);
	gc2053_write_register(ViPipe, 0x97, 0x07);
	gc2053_write_register(ViPipe, 0x98, 0x80);
	gc2053_write_register(ViPipe, 0xfe, 0x04);
	gc2053_write_register(ViPipe, 0x14, 0x78);
	gc2053_write_register(ViPipe, 0x15, 0x78);
	gc2053_write_register(ViPipe, 0x16, 0x78);
	gc2053_write_register(ViPipe, 0x17, 0x78);
	gc2053_write_register(ViPipe, 0xfe, 0x01);
	gc2053_write_register(ViPipe, 0x9a, 0x06);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0x7b, 0x2a);
	gc2053_write_register(ViPipe, 0x23, 0x2d);
	gc2053_write_register(ViPipe, 0xfe, 0x03);
	gc2053_write_register(ViPipe, 0x02, 0x56);
	gc2053_write_register(ViPipe, 0x03, 0xb6);
	gc2053_write_register(ViPipe, 0x12, 0x80);
	gc2053_write_register(ViPipe, 0x13, 0x07);
	gc2053_write_register(ViPipe, 0x15, 0x12);
	gc2053_write_register(ViPipe, 0xfe, 0x00);

	printf("===Galaxycore GC2053_1080P_30FPS_10BIT_LINEAR_Init_OK!===\n");
	return;
}
void gc2053_FSWDR_1080p30_init(VI_PIPE ViPipe)
{
	gc2053_write_register(ViPipe, 0xfe, 0x80);
	gc2053_write_register(ViPipe, 0xfe, 0x80);
	gc2053_write_register(ViPipe, 0xfe, 0x80);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0xf2, 0x00);
	gc2053_write_register(ViPipe, 0xf3, 0x00);
	gc2053_write_register(ViPipe, 0xf4, 0x36);
	gc2053_write_register(ViPipe, 0xf5, 0xc0);
	gc2053_write_register(ViPipe, 0xf6, 0x44);
	gc2053_write_register(ViPipe, 0xf7, 0x01);
	gc2053_write_register(ViPipe, 0xf8, 0x2c);
	gc2053_write_register(ViPipe, 0xf9, 0x42);
	gc2053_write_register(ViPipe, 0xfc, 0x8e);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0x87, 0x18);
	gc2053_write_register(ViPipe, 0xee, 0x30);
	gc2053_write_register(ViPipe, 0xd0, 0xb7);
	gc2053_write_register(ViPipe, 0x03, 0x04);
	gc2053_write_register(ViPipe, 0x04, 0x60);
	gc2053_write_register(ViPipe, 0x05, 0x04);
	gc2053_write_register(ViPipe, 0x06, 0x4c);
	gc2053_write_register(ViPipe, 0x07, 0x00);
	gc2053_write_register(ViPipe, 0x08, 0x11);
	gc2053_write_register(ViPipe, 0x0a, 0x02);
	gc2053_write_register(ViPipe, 0x0c, 0x02);
	gc2053_write_register(ViPipe, 0x0d, 0x04);
	gc2053_write_register(ViPipe, 0x0e, 0x40);
	gc2053_write_register(ViPipe, 0x12, 0xe2);
	gc2053_write_register(ViPipe, 0x13, 0x16);
	gc2053_write_register(ViPipe, 0x19, 0x0a);
	gc2053_write_register(ViPipe, 0x28, 0x0a);
	gc2053_write_register(ViPipe, 0x2b, 0x04);
	gc2053_write_register(ViPipe, 0x37, 0x03);
	gc2053_write_register(ViPipe, 0x43, 0x07);
	gc2053_write_register(ViPipe, 0x44, 0x40);
	gc2053_write_register(ViPipe, 0x46, 0x0b);
	gc2053_write_register(ViPipe, 0x4b, 0x20);
	gc2053_write_register(ViPipe, 0x4e, 0x08);
	gc2053_write_register(ViPipe, 0x55, 0x20);
	gc2053_write_register(ViPipe, 0x77, 0x01);
	gc2053_write_register(ViPipe, 0x78, 0x00);
	gc2053_write_register(ViPipe, 0x7c, 0x93);
	gc2053_write_register(ViPipe, 0x8d, 0x92);
	gc2053_write_register(ViPipe, 0x90, 0x00);
	gc2053_write_register(ViPipe, 0x41, 0x04);
	gc2053_write_register(ViPipe, 0x42, 0x65);
	gc2053_write_register(ViPipe, 0xce, 0x7c);
	gc2053_write_register(ViPipe, 0xd2, 0x41);
	gc2053_write_register(ViPipe, 0xd3, 0xdc);
	gc2053_write_register(ViPipe, 0xe6, 0x38);
	gc2053_write_register(ViPipe, 0xb6, 0xc0);
	gc2053_write_register(ViPipe, 0xb0, 0x70);
	gc2053_write_register(ViPipe, 0x26, 0x20);
	gc2053_write_register(ViPipe, 0xfe, 0x04);
	gc2053_write_register(ViPipe, 0xe0, 0x18);
	gc2053_write_register(ViPipe, 0xfe, 0x01);
	gc2053_write_register(ViPipe, 0x55, 0x07);
	gc2053_write_register(ViPipe, 0x58, 0x00);
	gc2053_write_register(ViPipe, 0xfe, 0x04);
	gc2053_write_register(ViPipe, 0x14, 0x78);
	gc2053_write_register(ViPipe, 0x15, 0x78);
	gc2053_write_register(ViPipe, 0x16, 0x78);
	gc2053_write_register(ViPipe, 0x17, 0x78);
	gc2053_write_register(ViPipe, 0xfe, 0x01);
	gc2053_write_register(ViPipe, 0x04, 0x00);
	gc2053_write_register(ViPipe, 0x94, 0x03);
	gc2053_write_register(ViPipe, 0x97, 0x07);
	gc2053_write_register(ViPipe, 0x98, 0x80);
	gc2053_write_register(ViPipe, 0x9a, 0x06);
	gc2053_write_register(ViPipe, 0x40, 0x03);
	gc2053_write_register(ViPipe, 0x42, 0x20);
	gc2053_write_register(ViPipe, 0x46, 0x4f);
	gc2053_write_register(ViPipe, 0x47, 0x40);
	gc2053_write_register(ViPipe, 0x4f, 0x02);
	gc2053_write_register(ViPipe, 0x50, 0x3a);
	gc2053_write_register(ViPipe, 0x51, 0x1d);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0x7b, 0x2a);
	gc2053_write_register(ViPipe, 0x23, 0x2d);
	gc2053_write_register(ViPipe, 0xfe, 0x03);
	gc2053_write_register(ViPipe, 0x01, 0x27);
	gc2053_write_register(ViPipe, 0x02, 0x56);
	gc2053_write_register(ViPipe, 0x03, 0xb6);
	gc2053_write_register(ViPipe, 0x12, 0x80);
	gc2053_write_register(ViPipe, 0x13, 0x07);
	gc2053_write_register(ViPipe, 0x15, 0x12);
	gc2053_write_register(ViPipe, 0xfe, 0x00);
	gc2053_write_register(ViPipe, 0x3e, 0x91);
	printf("=== Galaxycore GC2053_1080P_30FPS_10BIT_WDR_Init_OK!===\n");
	return;
}
