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
#include "i2c.h"
#endif

const unsigned char gc2053_forcar_i2c_addr =
	0x6e; /* I2C Address of GC2053_forcar */
const unsigned int gc2053_forcar_addr_byte = 1;
const unsigned int gc2053_forcar_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };
static GK_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = { 0 };

extern ISP_SNS_STATE_S *g_pastGc2053_forcar[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunGc2053_forcar_BusInfo[];

// sensor fps mode
#define GC2053_SENSOR_1080P_30FPS_LINEAR_MODE (0)

int gc2053_forcar_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunGc2053_forcar_BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE,
		    (gc2053_forcar_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int gc2053_forcar_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int gc2053_forcar_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int gc2053_forcar_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = gc2053_forcar_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = gc2053_forcar_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = gc2053_forcar_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (gc2053_forcar_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (gc2053_forcar_data_byte == 2) {
		buf[idx] = (data >> 8) & 0xff;
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf,
		    gc2053_forcar_addr_byte + gc2053_forcar_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void gc2053_forcar_standby(VI_PIPE ViPipe)
{
	gc2053_forcar_write_register(ViPipe, 0xf2, 0x01);
	gc2053_forcar_write_register(ViPipe, 0xfc, 0x8f);
	g_bStandby[ViPipe] = GK_TRUE;
	return;
}

void gc2053_forcar_restart(VI_PIPE ViPipe)
{
	gc2053_forcar_write_register(ViPipe, 0xf2, 0x00);
	gc2053_forcar_write_register(ViPipe, 0xfc, 0x8e);
	g_bStandby[ViPipe] = GK_FALSE;
	return;
}

void gc2053_forcar_mirror_flip(VI_PIPE ViPipe,
			       ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
	switch (eSnsMirrorFlip) {
	default:
	case ISP_SNS_NORMAL:
		gc2053_forcar_write_register(ViPipe, 0x17, 0x80);
		break;

	case ISP_SNS_MIRROR:
		gc2053_forcar_write_register(ViPipe, 0x17, 0x81);
		break;

	case ISP_SNS_FLIP:
		gc2053_forcar_write_register(ViPipe, 0x17, 0x82);
		break;

	case ISP_SNS_MIRROR_FLIP:
		gc2053_forcar_write_register(ViPipe, 0x17, 0x83);
		break;
	}

	return;
}

void gc2053_forcar_linear_1080p30_init(VI_PIPE ViPipe);

void gc2053_forcar_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	GK_U8 u8ImgMode;
	GK_BOOL bInit;

	bInit = g_pastGc2053_forcar[ViPipe]->bInit;
	u8ImgMode = g_pastGc2053_forcar[ViPipe]->u8ImgMode;

	if (bInit == GK_FALSE) {
		/* 1. sensor i2c init */
		gc2053_forcar_i2c_init(ViPipe);
	}

	switch (u8ImgMode) {
	case 0:
		gc2053_forcar_linear_1080p30_init(ViPipe);
		break;
	default:
		printf("GC2053_SENSOR_CTL_Not support this mode\n");
		g_pastGc2053_forcar[ViPipe]->bInit = GK_FALSE;
		break;
	}

	for (i = 0; i < g_pastGc2053_forcar[ViPipe]->astRegsInfo[0].u32RegNum;
	     i++) {
		gc2053_forcar_write_register(ViPipe,
					     g_pastGc2053_forcar[ViPipe]
						     ->astRegsInfo[0]
						     .astI2cData[i]
						     .u32RegAddr,
					     g_pastGc2053_forcar[ViPipe]
						     ->astRegsInfo[0]
						     .astI2cData[i]
						     .u32Data);
	}

	g_pastGc2053_forcar[ViPipe]->bInit = GK_TRUE;

	printf("GC2053 init succuss!\n");

	return;
}

void gc2053_forcar_exit(VI_PIPE ViPipe)
{
	gc2053_forcar_i2c_exit(ViPipe);
	g_bStandby[ViPipe] = GK_FALSE;

	return;
}

void gc2053_forcar_linear_1080p30_init(VI_PIPE ViPipe)
{
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x80);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x80);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x80);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x00);
	gc2053_forcar_write_register(ViPipe, 0xf2, 0x00);
	gc2053_forcar_write_register(ViPipe, 0xf3, 0x00);
	gc2053_forcar_write_register(ViPipe, 0xf4, 0x36);
	gc2053_forcar_write_register(ViPipe, 0xf5, 0xc0);
	gc2053_forcar_write_register(ViPipe, 0xf6, 0x44);
	gc2053_forcar_write_register(ViPipe, 0xf7, 0x01);
	gc2053_forcar_write_register(ViPipe, 0xf8, 0x2c);
	gc2053_forcar_write_register(ViPipe, 0xf9, 0x42);
	gc2053_forcar_write_register(ViPipe, 0xfc, 0x8e);

	gc2053_forcar_write_register(ViPipe, 0xfe, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x87, 0x18);
	gc2053_forcar_write_register(ViPipe, 0xee, 0x30);
	gc2053_forcar_write_register(ViPipe, 0xd0, 0xb7);
	gc2053_forcar_write_register(ViPipe, 0x03, 0x04);
	gc2053_forcar_write_register(ViPipe, 0x04, 0x60);
	gc2053_forcar_write_register(ViPipe, 0x05, 0x04);
	gc2053_forcar_write_register(ViPipe, 0x06, 0x4c);
	gc2053_forcar_write_register(ViPipe, 0x07, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x08, 0x11);
	gc2053_forcar_write_register(ViPipe, 0x0a, 0x02);
	gc2053_forcar_write_register(ViPipe, 0x0c, 0x02);
	gc2053_forcar_write_register(ViPipe, 0x0d, 0x04);
	gc2053_forcar_write_register(ViPipe, 0x0e, 0x40);
	gc2053_forcar_write_register(ViPipe, 0x12, 0xe2);
	gc2053_forcar_write_register(ViPipe, 0x13, 0x16);
	gc2053_forcar_write_register(ViPipe, 0x19, 0x0a);
	gc2053_forcar_write_register(ViPipe, 0x28, 0x0a);
	gc2053_forcar_write_register(ViPipe, 0x2b, 0x04);
	gc2053_forcar_write_register(ViPipe, 0x37, 0x03);
	gc2053_forcar_write_register(ViPipe, 0x43, 0x07);
	gc2053_forcar_write_register(ViPipe, 0x44, 0x40);
	gc2053_forcar_write_register(ViPipe, 0x46, 0x0b);
	gc2053_forcar_write_register(ViPipe, 0x4b, 0x20);
	gc2053_forcar_write_register(ViPipe, 0x4e, 0x08);
	gc2053_forcar_write_register(ViPipe, 0x55, 0x20);
	gc2053_forcar_write_register(ViPipe, 0x77, 0x01);
	gc2053_forcar_write_register(ViPipe, 0x78, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x7c, 0x93);
	gc2053_forcar_write_register(ViPipe, 0x8d, 0x92);
	gc2053_forcar_write_register(ViPipe, 0x90, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x41, 0x04);
	gc2053_forcar_write_register(ViPipe, 0x42, 0x65);
	gc2053_forcar_write_register(ViPipe, 0xce, 0x7c);
	gc2053_forcar_write_register(ViPipe, 0xd2, 0x41);
	gc2053_forcar_write_register(ViPipe, 0xd3, 0xdc);
	gc2053_forcar_write_register(ViPipe, 0xe6, 0x50);
	gc2053_forcar_write_register(ViPipe, 0xb6, 0xc0);
	gc2053_forcar_write_register(ViPipe, 0xb0, 0x70);
	gc2053_forcar_write_register(ViPipe, 0x26, 0x30);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x01);
	gc2053_forcar_write_register(ViPipe, 0x55, 0x07);
	gc2053_forcar_write_register(ViPipe, 0x58, 0x00);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x04);
	gc2053_forcar_write_register(ViPipe, 0x14, 0x78);
	gc2053_forcar_write_register(ViPipe, 0x15, 0x78);
	gc2053_forcar_write_register(ViPipe, 0x16, 0x78);
	gc2053_forcar_write_register(ViPipe, 0x17, 0x78);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x01);
	gc2053_forcar_write_register(ViPipe, 0x04, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x94, 0x03);
	gc2053_forcar_write_register(ViPipe, 0x97, 0x07);
	gc2053_forcar_write_register(ViPipe, 0x98, 0x80);
	gc2053_forcar_write_register(ViPipe, 0x9a, 0x06);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x7b, 0x2a);
	gc2053_forcar_write_register(ViPipe, 0x23, 0x2d);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x03);
	gc2053_forcar_write_register(ViPipe, 0x01, 0x27);
	gc2053_forcar_write_register(ViPipe, 0x02, 0x56);
	gc2053_forcar_write_register(ViPipe, 0x03, 0xb6);
	gc2053_forcar_write_register(ViPipe, 0x12, 0x80);
	gc2053_forcar_write_register(ViPipe, 0x13, 0x07);
	gc2053_forcar_write_register(ViPipe, 0x15, 0x12);
	gc2053_forcar_write_register(ViPipe, 0xfe, 0x00);
	gc2053_forcar_write_register(ViPipe, 0x3e, 0x91);

	printf("=== Galaxycore GC2053_1080P_30FPS_10BIT_LINE_Init_OK!===\n");

	return;
}
