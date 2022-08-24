
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
const unsigned char gc4653_2l_i2c_addr = 0x52; /* I2C Address of gc4653 */
const unsigned int gc4653_2l_addr_byte = 2;
const unsigned int gc4653_2l_data_byte = 1;

static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };
static GK_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = { 0 };
extern ISP_SNS_STATE_S *g_pastGc4653_2l[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunGc4653_2l_BusInfo[];

// sensor fps mode
#define Gc4653_2L_SENSOR_4M_30FPS_LINEAR_MODE (0) // 2560x1440

int gc4653_2l_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunGc4653_2l_BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (gc4653_2l_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int gc4653_2l_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int gc4653_2l_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int gc4653_2l_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = gc4653_2l_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = gc4653_2l_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = gc4653_2l_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else

	int idx = 0;
	int ret;
	char buf[8];

	if (gc4653_2l_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (gc4653_2l_data_byte == 2) {
		buf[idx] = (data >> 8) & 0xff;
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf,
		    gc4653_2l_addr_byte + gc4653_2l_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void gc4653_2l_standby(VI_PIPE ViPipe)
{
	return;
}

void gc4653_2l_restart(VI_PIPE ViPipe)
{
	return;
}

void gc4653_2l_mirror_flip(VI_PIPE ViPipe,
			   ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
	switch (eSnsMirrorFlip) {
	default:
	case ISP_SNS_NORMAL:
		gc4653_2l_write_register(ViPipe, 0x0101, 0x00);
		break;

	case ISP_SNS_MIRROR:
		gc4653_2l_write_register(ViPipe, 0x0101, 0x01);
		break;

	case ISP_SNS_FLIP:
		gc4653_2l_write_register(ViPipe, 0x0101, 0x02);
		break;

	case ISP_SNS_MIRROR_FLIP:
		gc4653_2l_write_register(ViPipe, 0x0101, 0x03);
		break;
	}

	return;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

void gc4653_2l_linear_4M30_init(VI_PIPE ViPipe);

void gc4653_2l_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	GK_U8 u8ImgMode;
	GK_BOOL bInit;

	bInit = g_pastGc4653_2l[ViPipe]->bInit;
	u8ImgMode = g_pastGc4653_2l[ViPipe]->u8ImgMode;

	if (GK_FALSE == bInit) {
		/* sensor i2c init */
		printf("binit false GC4653 i2c init\n");
		gc4653_2l_i2c_init(ViPipe);
	}

	switch (u8ImgMode) {
	case 0:
		gc4653_2l_linear_4M30_init(ViPipe);
		break;
	default:
		printf("Gc4653_2L_SENSOR_CTL_Not support this mode\n");
		g_pastGc4653_2l[ViPipe]->bInit = GK_FALSE;
		break;
	}

	for (i = 0; i < g_pastGc4653_2l[ViPipe]->astRegsInfo[0].u32RegNum;
	     i++) {
		gc4653_2l_write_register(ViPipe,
					 g_pastGc4653_2l[ViPipe]
						 ->astRegsInfo[0]
						 .astI2cData[i]
						 .u32RegAddr,
					 g_pastGc4653_2l[ViPipe]
						 ->astRegsInfo[0]
						 .astI2cData[i]
						 .u32Data);
	}
	g_pastGc4653_2l[ViPipe]->bInit = GK_TRUE;

	printf("GC4653 init succuss!\n");

	return;
}

void gc4653_2l_exit(VI_PIPE ViPipe)
{
	gc4653_2l_i2c_exit(ViPipe);
	g_bStandby[ViPipe] = GK_FALSE;

	return;
}

void gc4653_2l_linear_4M30_init(VI_PIPE ViPipe)
{
	gc4653_2l_write_register(ViPipe, 0x03fe, 0xf0);
	gc4653_2l_write_register(ViPipe, 0x03fe, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0317, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0320, 0x77);
	gc4653_2l_write_register(ViPipe, 0x0324, 0xc8);
	gc4653_2l_write_register(ViPipe, 0x0325, 0x06);
	gc4653_2l_write_register(ViPipe, 0x0326, 0x60);
	gc4653_2l_write_register(ViPipe, 0x0327, 0x03);
	gc4653_2l_write_register(ViPipe, 0x0334, 0x40);
	gc4653_2l_write_register(ViPipe, 0x0336, 0x60);
	gc4653_2l_write_register(ViPipe, 0x0337, 0x82);
	gc4653_2l_write_register(ViPipe, 0x0315, 0x25);
	gc4653_2l_write_register(ViPipe, 0x031c, 0xc6);
	gc4653_2l_write_register(ViPipe, 0x0287, 0x18);
	gc4653_2l_write_register(ViPipe, 0x0084, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0087, 0x50);
	gc4653_2l_write_register(ViPipe, 0x029d, 0x08);
	gc4653_2l_write_register(ViPipe, 0x0290, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0340, 0x05);
	gc4653_2l_write_register(ViPipe, 0x0341, 0xdc);
	gc4653_2l_write_register(ViPipe, 0x0345, 0x06);
	gc4653_2l_write_register(ViPipe, 0x034b, 0xb0);
	gc4653_2l_write_register(ViPipe, 0x0352, 0x08);
	gc4653_2l_write_register(ViPipe, 0x0354, 0x08);
	gc4653_2l_write_register(ViPipe, 0x02d1, 0xe0);
	gc4653_2l_write_register(ViPipe, 0x0223, 0xf2);
	gc4653_2l_write_register(ViPipe, 0x0238, 0xa4);
	gc4653_2l_write_register(ViPipe, 0x02ce, 0x7f);
	gc4653_2l_write_register(ViPipe, 0x0232, 0xc4);
	gc4653_2l_write_register(ViPipe, 0x02d3, 0x05);
	gc4653_2l_write_register(ViPipe, 0x0243, 0x06);
	gc4653_2l_write_register(ViPipe, 0x02ee, 0x30);
	gc4653_2l_write_register(ViPipe, 0x026f, 0x70);
	gc4653_2l_write_register(ViPipe, 0x0257, 0x09);
	gc4653_2l_write_register(ViPipe, 0x0211, 0x02);
	gc4653_2l_write_register(ViPipe, 0x0219, 0x09);
	gc4653_2l_write_register(ViPipe, 0x023f, 0x2d);
	gc4653_2l_write_register(ViPipe, 0x0518, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0519, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0515, 0x08);
	gc4653_2l_write_register(ViPipe, 0x02d9, 0x3f);
	gc4653_2l_write_register(ViPipe, 0x02da, 0x02);
	gc4653_2l_write_register(ViPipe, 0x02db, 0xe8);
	gc4653_2l_write_register(ViPipe, 0x02e6, 0x20);
	gc4653_2l_write_register(ViPipe, 0x021b, 0x10);
	gc4653_2l_write_register(ViPipe, 0x0252, 0x22);
	gc4653_2l_write_register(ViPipe, 0x024e, 0x22);
	gc4653_2l_write_register(ViPipe, 0x02c4, 0x01);
	gc4653_2l_write_register(ViPipe, 0x021d, 0x17);
	gc4653_2l_write_register(ViPipe, 0x024a, 0x01);
	gc4653_2l_write_register(ViPipe, 0x02ca, 0x02);
	gc4653_2l_write_register(ViPipe, 0x0262, 0x10);
	gc4653_2l_write_register(ViPipe, 0x029a, 0x20);
	gc4653_2l_write_register(ViPipe, 0x021c, 0x0e);
	gc4653_2l_write_register(ViPipe, 0x0298, 0x03);
	gc4653_2l_write_register(ViPipe, 0x029c, 0x00);
	gc4653_2l_write_register(ViPipe, 0x027e, 0x14);
	gc4653_2l_write_register(ViPipe, 0x02c2, 0x10);
	gc4653_2l_write_register(ViPipe, 0x0540, 0x20);
	gc4653_2l_write_register(ViPipe, 0x0546, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0548, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0544, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0242, 0x1b);
	gc4653_2l_write_register(ViPipe, 0x02c0, 0x1b);
	gc4653_2l_write_register(ViPipe, 0x02c3, 0x20);
	gc4653_2l_write_register(ViPipe, 0x02e4, 0x10);
	gc4653_2l_write_register(ViPipe, 0x022e, 0x00);
	gc4653_2l_write_register(ViPipe, 0x027b, 0x3f);
	gc4653_2l_write_register(ViPipe, 0x0269, 0x0f);
	gc4653_2l_write_register(ViPipe, 0x02d2, 0x40);
	gc4653_2l_write_register(ViPipe, 0x027c, 0x08);
	gc4653_2l_write_register(ViPipe, 0x023a, 0x2e);
	gc4653_2l_write_register(ViPipe, 0x0245, 0xce);
	gc4653_2l_write_register(ViPipe, 0x0530, 0x20);
	gc4653_2l_write_register(ViPipe, 0x0531, 0x02);
	gc4653_2l_write_register(ViPipe, 0x0228, 0x50);
	gc4653_2l_write_register(ViPipe, 0x02ab, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0250, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0221, 0x50);
	gc4653_2l_write_register(ViPipe, 0x02ac, 0x00);
	gc4653_2l_write_register(ViPipe, 0x02a5, 0x02);
	gc4653_2l_write_register(ViPipe, 0x0260, 0x0b);
	gc4653_2l_write_register(ViPipe, 0x0216, 0x04);
	gc4653_2l_write_register(ViPipe, 0x0299, 0x1C);
	gc4653_2l_write_register(ViPipe, 0x02bb, 0x0d);
	gc4653_2l_write_register(ViPipe, 0x02a3, 0x02);
	gc4653_2l_write_register(ViPipe, 0x02a4, 0x02);
	gc4653_2l_write_register(ViPipe, 0x021e, 0x02);
	gc4653_2l_write_register(ViPipe, 0x024f, 0x08);
	gc4653_2l_write_register(ViPipe, 0x028c, 0x08);
	gc4653_2l_write_register(ViPipe, 0x0532, 0x3f);
	gc4653_2l_write_register(ViPipe, 0x0533, 0x02);
	gc4653_2l_write_register(ViPipe, 0x0277, 0xc0);
	gc4653_2l_write_register(ViPipe, 0x0276, 0xc0);
	gc4653_2l_write_register(ViPipe, 0x0239, 0xc0);
	gc4653_2l_write_register(ViPipe, 0x0202, 0x05);
	gc4653_2l_write_register(ViPipe, 0x0203, 0xd0);
	gc4653_2l_write_register(ViPipe, 0x0205, 0xc0);
	gc4653_2l_write_register(ViPipe, 0x02b0, 0x68);
	gc4653_2l_write_register(ViPipe, 0x0002, 0xa9);
	gc4653_2l_write_register(ViPipe, 0x0004, 0x01);
	gc4653_2l_write_register(ViPipe, 0x021a, 0x98);
	gc4653_2l_write_register(ViPipe, 0x0266, 0xa0);
	gc4653_2l_write_register(ViPipe, 0x0020, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0021, 0x03);
	gc4653_2l_write_register(ViPipe, 0x0022, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0023, 0x04);
	gc4653_2l_write_register(ViPipe, 0x0342, 0x06);
	gc4653_2l_write_register(ViPipe, 0x0343, 0x40);
	gc4653_2l_write_register(ViPipe, 0x03fe, 0x10);
	gc4653_2l_write_register(ViPipe, 0x03fe, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0106, 0x78);
	gc4653_2l_write_register(ViPipe, 0x0108, 0x0c);
	gc4653_2l_write_register(ViPipe, 0x0114, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0115, 0x12);
	gc4653_2l_write_register(ViPipe, 0x0180, 0x46);
	gc4653_2l_write_register(ViPipe, 0x0181, 0x30);
	gc4653_2l_write_register(ViPipe, 0x0182, 0x05);
	gc4653_2l_write_register(ViPipe, 0x0185, 0x01);
	gc4653_2l_write_register(ViPipe, 0x03fe, 0x10);
	gc4653_2l_write_register(ViPipe, 0x03fe, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0100, 0x09);
	gc4653_2l_write_register(ViPipe, 0x000f, 0x00);

	gc4653_2l_write_register(ViPipe, 0x0080, 0x02);
	gc4653_2l_write_register(ViPipe, 0x0097, 0x0a);
	gc4653_2l_write_register(ViPipe, 0x0098, 0x10);
	gc4653_2l_write_register(ViPipe, 0x0099, 0x05);
	gc4653_2l_write_register(ViPipe, 0x009a, 0xb0);
	gc4653_2l_write_register(ViPipe, 0x0317, 0x08);
	gc4653_2l_write_register(ViPipe, 0x0a67, 0x80);
	gc4653_2l_write_register(ViPipe, 0x0a70, 0x03);
	gc4653_2l_write_register(ViPipe, 0x0a82, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0a83, 0x10);
	gc4653_2l_write_register(ViPipe, 0x0a80, 0x2b);
	gc4653_2l_write_register(ViPipe, 0x05be, 0x00);
	gc4653_2l_write_register(ViPipe, 0x05a9, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0313, 0x80);
	gc4653_2l_write_register(ViPipe, 0x05be, 0x01);
	gc4653_2l_write_register(ViPipe, 0x0317, 0x00);
	gc4653_2l_write_register(ViPipe, 0x0a67, 0x00);

	delay_ms(18);

	printf("-------Galaxycore Gc4653_2L_init_4M_2560x1440_10bit_linear30 Initial OK!-------\n");
}
