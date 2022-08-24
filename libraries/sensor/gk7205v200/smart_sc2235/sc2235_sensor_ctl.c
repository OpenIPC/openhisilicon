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

const unsigned char sc2235_i2c_addr = 0x60; /* I2C Address of SC2235 */
const unsigned int sc2235_addr_byte = 2;
const unsigned int sc2235_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastSc2235[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunSc2235BusInfo[];

// sensor fps mode
#define SC2235_SENSOR_1080P_30FPS_LINEAR_MODE (0)

int sc2235_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunSc2235BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc2235_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int sc2235_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int sc2235_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int sc2235_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = sc2235_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = sc2235_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = sc2235_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (sc2235_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (sc2235_data_byte == 2) {
		buf[idx] = (data >> 8) & 0xff;
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, sc2235_addr_byte + sc2235_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

void sc2235_standby(VI_PIPE ViPipe)
{
	return;
}

void sc2235_restart(VI_PIPE ViPipe)
{
	return;
}

void sc2235_linear_1080p30_init(VI_PIPE ViPipe);

void sc2235_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	GK_U8 u8ImgMode;
	u8ImgMode = g_pastSc2235[ViPipe]->u8ImgMode;
	sc2235_i2c_init(ViPipe);
	switch (u8ImgMode) {
	case 0:
		sc2235_linear_1080p30_init(ViPipe);
		break;
	default:
		printf("SC2235_SENSOR_CTL_Not support this mode\n");
		g_pastSc2235[ViPipe]->bInit = GK_FALSE;
		break;
	}

	for (i = 0; i < g_pastSc2235[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		sc2235_write_register(ViPipe,
				      g_pastSc2235[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32RegAddr,
				      g_pastSc2235[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32Data);
	}

	g_pastSc2235[ViPipe]->bInit = GK_TRUE;

	return;
}

void sc2235_exit(VI_PIPE ViPipe)
{
	sc2235_i2c_exit(ViPipe);

	return;
}

void sc2235_linear_1080p30_init(VI_PIPE ViPipe)
{
	sc2235_write_register(ViPipe, 0x0103, 0x01);
	delay_ms(20);
	sc2235_write_register(ViPipe, 0x0100, 0x00);
	sc2235_write_register(ViPipe, 0x3621, 0x28);
	sc2235_write_register(ViPipe, 0x3309, 0x60);
	sc2235_write_register(ViPipe, 0x331f, 0x4d);
	sc2235_write_register(ViPipe, 0x3321, 0x4f);
	sc2235_write_register(ViPipe, 0x33b5, 0x10);
	sc2235_write_register(ViPipe, 0x3303, 0x20);
	sc2235_write_register(ViPipe, 0x331e, 0x0d);
	sc2235_write_register(ViPipe, 0x3320, 0x0f);
	sc2235_write_register(ViPipe, 0x3622, 0x02);
	sc2235_write_register(ViPipe, 0x3633, 0x42);
	sc2235_write_register(ViPipe, 0x3634, 0x42);
	sc2235_write_register(ViPipe, 0x3306, 0x66);
	sc2235_write_register(ViPipe, 0x330b, 0xd1);
	sc2235_write_register(ViPipe, 0x3301, 0x0e);
	sc2235_write_register(ViPipe, 0x320c, 0x08);
	sc2235_write_register(ViPipe, 0x320d, 0x98);
	sc2235_write_register(ViPipe, 0x3364, 0x05);
	sc2235_write_register(ViPipe, 0x363c, 0x28);
	sc2235_write_register(ViPipe, 0x363b, 0x0a);
	sc2235_write_register(ViPipe, 0x3635, 0xa0);
	sc2235_write_register(ViPipe, 0x4500, 0x59);
	sc2235_write_register(ViPipe, 0x3d08, 0x00);
	sc2235_write_register(ViPipe, 0x3908, 0x11);
	sc2235_write_register(ViPipe, 0x363c, 0x08);
	sc2235_write_register(ViPipe, 0x3e03, 0x03);
	sc2235_write_register(ViPipe, 0x3e01, 0x46);
	sc2235_write_register(ViPipe, 0x3381, 0x0a);
	sc2235_write_register(ViPipe, 0x3348, 0x09);
	sc2235_write_register(ViPipe, 0x3349, 0x50);
	sc2235_write_register(ViPipe, 0x334a, 0x02);
	sc2235_write_register(ViPipe, 0x334b, 0x60);
	sc2235_write_register(ViPipe, 0x3380, 0x04);
	sc2235_write_register(ViPipe, 0x3340, 0x06);
	sc2235_write_register(ViPipe, 0x3341, 0x50);
	sc2235_write_register(ViPipe, 0x3342, 0x02);
	sc2235_write_register(ViPipe, 0x3343, 0x60);
	sc2235_write_register(ViPipe, 0x3632, 0x88);
	sc2235_write_register(ViPipe, 0x3309, 0xa0);
	sc2235_write_register(ViPipe, 0x331f, 0x8d);
	sc2235_write_register(ViPipe, 0x3321, 0x8f);
	sc2235_write_register(ViPipe, 0x335e, 0x01);
	sc2235_write_register(ViPipe, 0x335f, 0x03);
	sc2235_write_register(ViPipe, 0x337c, 0x04);
	sc2235_write_register(ViPipe, 0x337d, 0x06);
	sc2235_write_register(ViPipe, 0x33a0, 0x05);
	sc2235_write_register(ViPipe, 0x3301, 0x05);
	sc2235_write_register(ViPipe, 0x3670, 0x08);
	sc2235_write_register(ViPipe, 0x367e, 0x07);
	sc2235_write_register(ViPipe, 0x367f, 0x0f);
	sc2235_write_register(ViPipe, 0x3677, 0x2f);
	sc2235_write_register(ViPipe, 0x3678, 0x22);
	sc2235_write_register(ViPipe, 0x3679, 0x43);
	sc2235_write_register(ViPipe, 0x337f, 0x03);
	sc2235_write_register(ViPipe, 0x3368, 0x02);
	sc2235_write_register(ViPipe, 0x3369, 0x00);
	sc2235_write_register(ViPipe, 0x336a, 0x00);
	sc2235_write_register(ViPipe, 0x336b, 0x00);
	sc2235_write_register(ViPipe, 0x3367, 0x08);
	sc2235_write_register(ViPipe, 0x330e, 0x30);
	sc2235_write_register(ViPipe, 0x3366, 0x7c);
	sc2235_write_register(ViPipe, 0x3635, 0xc1);
	sc2235_write_register(ViPipe, 0x363b, 0x09);
	sc2235_write_register(ViPipe, 0x363c, 0x07);
	sc2235_write_register(ViPipe, 0x391e, 0x00);
	sc2235_write_register(ViPipe, 0x3637, 0x14);
	sc2235_write_register(ViPipe, 0x3306, 0x54);
	sc2235_write_register(ViPipe, 0x330b, 0xd8);
	sc2235_write_register(ViPipe, 0x366e, 0x08);
	sc2235_write_register(ViPipe, 0x366f, 0x2f);
	sc2235_write_register(ViPipe, 0x3631, 0x84);
	sc2235_write_register(ViPipe, 0x3630, 0x48);
	sc2235_write_register(ViPipe, 0x3622, 0x06);
	sc2235_write_register(ViPipe, 0x3638, 0x1f);
	sc2235_write_register(ViPipe, 0x3625, 0x02);
	sc2235_write_register(ViPipe, 0x3636, 0x24);
	sc2235_write_register(ViPipe, 0x3348, 0x08);
	sc2235_write_register(ViPipe, 0x3e03, 0x0b);
	sc2235_write_register(ViPipe, 0x3342, 0x03);
	sc2235_write_register(ViPipe, 0x3343, 0xa0);
	sc2235_write_register(ViPipe, 0x334a, 0x03);
	sc2235_write_register(ViPipe, 0x334b, 0xa0);
	sc2235_write_register(ViPipe, 0x3343, 0xb0);
	sc2235_write_register(ViPipe, 0x334b, 0xb0);
	sc2235_write_register(ViPipe, 0x3802, 0x01);
	sc2235_write_register(ViPipe, 0x3235, 0x04);
	sc2235_write_register(ViPipe, 0x3236, 0x63);
	sc2235_write_register(ViPipe, 0x3343, 0xd0);
	sc2235_write_register(ViPipe, 0x334b, 0xd0);
	sc2235_write_register(ViPipe, 0x3348, 0x07);
	sc2235_write_register(ViPipe, 0x3349, 0x80);
	sc2235_write_register(ViPipe, 0x391b, 0x4d);
	sc2235_write_register(ViPipe, 0x3342, 0x04);
	sc2235_write_register(ViPipe, 0x3343, 0x20);
	sc2235_write_register(ViPipe, 0x334a, 0x04);
	sc2235_write_register(ViPipe, 0x334b, 0x20);
	sc2235_write_register(ViPipe, 0x3222, 0x29);
	sc2235_write_register(ViPipe, 0x3901, 0x02);
	sc2235_write_register(ViPipe, 0x3f00, 0x07);
	sc2235_write_register(ViPipe, 0x3f04, 0x08);
	sc2235_write_register(ViPipe, 0x3f05, 0x74);
	sc2235_write_register(ViPipe, 0x330b, 0xc8);
	sc2235_write_register(ViPipe, 0x3306, 0x4a);
	sc2235_write_register(ViPipe, 0x330b, 0xca);
	sc2235_write_register(ViPipe, 0x3639, 0x09);
	sc2235_write_register(ViPipe, 0x5780, 0xff);
	sc2235_write_register(ViPipe, 0x5781, 0x04);
	sc2235_write_register(ViPipe, 0x5785, 0x18);
	sc2235_write_register(ViPipe, 0x3313, 0x05);
	sc2235_write_register(ViPipe, 0x3678, 0x42);
	sc2235_write_register(ViPipe, 0x3237, 0x05);
	sc2235_write_register(ViPipe, 0x3238, 0x00);
	sc2235_write_register(ViPipe, 0x330b, 0xc8);
	sc2235_write_register(ViPipe, 0x3306, 0x40);
	sc2235_write_register(ViPipe, 0x3802, 0x00);
	// init
	sc2235_write_register(ViPipe, 0x3e01, 0x00);
	sc2235_write_register(ViPipe, 0x3e02, 0x10);
	sc2235_write_register(ViPipe, 0x3e03, 0x0b);
	sc2235_write_register(ViPipe, 0x3e07, 0x00);
	sc2235_write_register(ViPipe, 0x3e08, 0x03);
	sc2235_write_register(ViPipe, 0x3e09, 0x10);

	sc2235_write_register(ViPipe, 0x3221, 0x66);
	// Platform_Dependent
	sc2235_write_register(ViPipe, 0x3d08, 0x01);
	sc2235_write_register(ViPipe, 0x3641, 0x01);
	delay_ms(20);
	sc2235_write_register(ViPipe, 0x0100, 0x01);

	printf("=== Smart SC2235_1080P_30FPS_10BIT_LINE_Init_OK!===\n");

	return;
}
