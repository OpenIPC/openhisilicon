/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "hicompat.h"

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else

#ifdef __LITEOS__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif

const unsigned char SC401AI_I2C_ADDR = 0x60; /* I2C Address of sc401ai */
const unsigned int SC401AI_ADDR_BYTE = 2;
const unsigned int SC401AI_DATA_BYTE = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastSc401ai[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunSc401aiBusInfo[];

int sc401ai_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunSc401aiBusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (SC401AI_I2C_ADDR >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int sc401ai_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

/*
 * A combined write-then-read, as the vendor driver does. A bare read() on
 * /dev/i2c-N never sends the register address, so it returns whatever the
 * sensor's address pointer last held.
 */
int sc401ai_read_register(VI_PIPE ViPipe, GK_U32 addr)
{
	GK_U8 aRecvbuf[2];
	struct i2c_msg astMsg[2];
	struct i2c_rdwr_ioctl_data stRdwr;

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sc401ai_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	aRecvbuf[0] = (addr >> 8) & 0xff;
	aRecvbuf[1] = addr & 0xff;

	astMsg[0].addr = SC401AI_I2C_ADDR >> 1;
	astMsg[0].flags = 0;
	astMsg[0].len = SC401AI_ADDR_BYTE;
	astMsg[0].buf = aRecvbuf;

	astMsg[1].addr = SC401AI_I2C_ADDR >> 1;
	astMsg[1].flags = I2C_M_RD;
	astMsg[1].len = SC401AI_DATA_BYTE;
	astMsg[1].buf = aRecvbuf;

	stRdwr.msgs = astMsg;
	stRdwr.nmsgs = 2;

	if (ioctl(g_fd[ViPipe], I2C_RDWR, &stRdwr) < 0) {
		return GK_FAILURE;
	}

	return aRecvbuf[0];
}

int sc401ai_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = SC401AI_I2C_ADDR;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = SC401AI_ADDR_BYTE;
	i2c_data.data = data;
	i2c_data.data_byte_num = SC401AI_DATA_BYTE;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (SC401AI_ADDR_BYTE == 2) {
		buf[idx] = (((GK_U32)addr) >> 8) & 0xff;
		idx++;
		buf[idx] = ((GK_U32)addr) & 0xff;
		idx++;
	} else {
	}

	if (SC401AI_DATA_BYTE == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, SC401AI_ADDR_BYTE + SC401AI_DATA_BYTE);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void sc401ai_mirror_flip(VI_PIPE ViPipe,
			 ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
	switch (eSnsMirrorFlip) {
	default:
	case ISP_SNS_NORMAL:
		sc401ai_write_register(ViPipe, 0x3221, 0x0);
		break;

	case ISP_SNS_MIRROR:
		sc401ai_write_register(ViPipe, 0x3221, 0x6);
		break;

	case ISP_SNS_FLIP:
		sc401ai_write_register(ViPipe, 0x3221, 0x60);
		break;

	case ISP_SNS_MIRROR_FLIP:
		sc401ai_write_register(ViPipe, 0x3221, 0x66);
		break;
	}
	return;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

void sc401ai_standby(VI_PIPE ViPipe)
{
	return;
}

void sc401ai_restart(VI_PIPE ViPipe)
{
	return;
}

#define sc401ai_SENSOR_1296P_30FPS_LINEAR_MODE (1)

void sc401ai_linear_4m30_init(VI_PIPE ViPipe);

void sc401ai_init(VI_PIPE ViPipe)
{
	GK_U32 i;

	sc401ai_i2c_init(ViPipe);

	sc401ai_linear_4m30_init(ViPipe);
	delay_ms(10);

	for (i = 0; i < g_pastSc401ai[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		sc401ai_write_register(ViPipe,
				       g_pastSc401ai[ViPipe]
					       ->astRegsInfo[0]
					       .astI2cData[i]
					       .u32RegAddr,
				       g_pastSc401ai[ViPipe]
					       ->astRegsInfo[0]
					       .astI2cData[i]
					       .u32Data);
	}

	g_pastSc401ai[ViPipe]->bInit = GK_TRUE;
	return;
}

void sc401ai_exit(VI_PIPE ViPipe)
{
	sc401ai_i2c_exit(ViPipe);
	return;
}

#define MIPI_2LANE
/*  (2560X1440@30fps) */
void sc401ai_linear_4m30_init(VI_PIPE ViPipe)
{
	sc401ai_write_register(ViPipe, 0x0103, 0x01);
	sc401ai_write_register(ViPipe, 0x0100, 0x00);
	sc401ai_write_register(ViPipe, 0x36e9, 0x80);
	sc401ai_write_register(ViPipe, 0x36f9, 0x80);
#ifdef MIPI_2LANE
	sc401ai_write_register(ViPipe, 0x3018, 0x3a);
	sc401ai_write_register(ViPipe, 0x3019, 0x0c);
	sc401ai_write_register(ViPipe, 0x301c, 0x78);
	sc401ai_write_register(ViPipe, 0x301f, 0x05);
#else
	sc401ai_write_register(ViPipe, 0x301c, 0x78);
	sc401ai_write_register(ViPipe, 0x301f, 0x01);
#endif
	sc401ai_write_register(ViPipe, 0x3208, 0x0a);
	sc401ai_write_register(ViPipe, 0x3209, 0x00);
	sc401ai_write_register(ViPipe, 0x320a, 0x05);
	sc401ai_write_register(ViPipe, 0x320b, 0xa0);
	sc401ai_write_register(ViPipe, 0x320e, 0x05);
	sc401ai_write_register(ViPipe, 0x320f, 0xdc);
	sc401ai_write_register(ViPipe, 0x3214, 0x11);
	sc401ai_write_register(ViPipe, 0x3215, 0x11);
	sc401ai_write_register(ViPipe, 0x3223, 0x80);
	sc401ai_write_register(ViPipe, 0x3250, 0x00);
	sc401ai_write_register(ViPipe, 0x3253, 0x08);
	sc401ai_write_register(ViPipe, 0x3274, 0x01);
	sc401ai_write_register(ViPipe, 0x3301, 0x20);
	sc401ai_write_register(ViPipe, 0x3302, 0x18);
	sc401ai_write_register(ViPipe, 0x3303, 0x10);
	sc401ai_write_register(ViPipe, 0x3304, 0x50);
	sc401ai_write_register(ViPipe, 0x3306, 0x38);
	sc401ai_write_register(ViPipe, 0x3308, 0x18);
	sc401ai_write_register(ViPipe, 0x3309, 0x60);
	sc401ai_write_register(ViPipe, 0x330b, 0xc0);
	sc401ai_write_register(ViPipe, 0x330d, 0x10);
	sc401ai_write_register(ViPipe, 0x330e, 0x18);
	sc401ai_write_register(ViPipe, 0x330f, 0x04);
	sc401ai_write_register(ViPipe, 0x3310, 0x02);
	sc401ai_write_register(ViPipe, 0x331c, 0x04);
	sc401ai_write_register(ViPipe, 0x331e, 0x41);
	sc401ai_write_register(ViPipe, 0x331f, 0x51);
	sc401ai_write_register(ViPipe, 0x3320, 0x09);
	sc401ai_write_register(ViPipe, 0x3333, 0x10);
	sc401ai_write_register(ViPipe, 0x334c, 0x08);
	sc401ai_write_register(ViPipe, 0x3356, 0x09);
	sc401ai_write_register(ViPipe, 0x3364, 0x17);
	sc401ai_write_register(ViPipe, 0x338e, 0xfd);
	sc401ai_write_register(ViPipe, 0x3390, 0x08);
	sc401ai_write_register(ViPipe, 0x3391, 0x18);
	sc401ai_write_register(ViPipe, 0x3392, 0x38);
	sc401ai_write_register(ViPipe, 0x3393, 0x20);
	sc401ai_write_register(ViPipe, 0x3394, 0x20);
	sc401ai_write_register(ViPipe, 0x3395, 0x20);
	sc401ai_write_register(ViPipe, 0x3396, 0x08);
	sc401ai_write_register(ViPipe, 0x3397, 0x18);
	sc401ai_write_register(ViPipe, 0x3398, 0x38);
	sc401ai_write_register(ViPipe, 0x3399, 0x20);
	sc401ai_write_register(ViPipe, 0x339a, 0x20);
	sc401ai_write_register(ViPipe, 0x339b, 0x20);
	sc401ai_write_register(ViPipe, 0x339c, 0x20);
	sc401ai_write_register(ViPipe, 0x33ac, 0x10);
	sc401ai_write_register(ViPipe, 0x33ae, 0x18);
	sc401ai_write_register(ViPipe, 0x33af, 0x19);
	sc401ai_write_register(ViPipe, 0x360f, 0x01);
	sc401ai_write_register(ViPipe, 0x3620, 0x08);
	sc401ai_write_register(ViPipe, 0x3637, 0x25);
	sc401ai_write_register(ViPipe, 0x363a, 0x12);
	sc401ai_write_register(ViPipe, 0x3670, 0x0a);
	sc401ai_write_register(ViPipe, 0x3671, 0x07);
	sc401ai_write_register(ViPipe, 0x3672, 0x57);
	sc401ai_write_register(ViPipe, 0x3673, 0x5e);
	sc401ai_write_register(ViPipe, 0x3674, 0x84);
	sc401ai_write_register(ViPipe, 0x3675, 0x88);
	sc401ai_write_register(ViPipe, 0x3676, 0x8a);
	sc401ai_write_register(ViPipe, 0x367a, 0x58);
	sc401ai_write_register(ViPipe, 0x367b, 0x78);
	sc401ai_write_register(ViPipe, 0x367c, 0x58);
	sc401ai_write_register(ViPipe, 0x367d, 0x78);
	sc401ai_write_register(ViPipe, 0x3690, 0x33);
	sc401ai_write_register(ViPipe, 0x3691, 0x43);
	sc401ai_write_register(ViPipe, 0x3692, 0x34);
	sc401ai_write_register(ViPipe, 0x369c, 0x40);
	sc401ai_write_register(ViPipe, 0x369d, 0x78);
	sc401ai_write_register(ViPipe, 0x36ea, 0x39);
	sc401ai_write_register(ViPipe, 0x36eb, 0x0d);
#ifdef MIPI_2LANE
	sc401ai_write_register(ViPipe, 0x36ec, 0x1c);
#else
	sc401ai_write_register(ViPipe, 0x36ec, 0x2c);
#endif
	sc401ai_write_register(ViPipe, 0x36ed, 0x24);
	sc401ai_write_register(ViPipe, 0x36fa, 0x39);
	sc401ai_write_register(ViPipe, 0x36fb, 0x33);
	sc401ai_write_register(ViPipe, 0x36fc, 0x10);
	sc401ai_write_register(ViPipe, 0x36fd, 0x14);
	sc401ai_write_register(ViPipe, 0x3908, 0x41);
	sc401ai_write_register(ViPipe, 0x396c, 0x0e);
	sc401ai_write_register(ViPipe, 0x3e00, 0x00);
	sc401ai_write_register(ViPipe, 0x3e01, 0xb6);
#ifdef MIPI_2LANE
	sc401ai_write_register(ViPipe, 0x3e02, 0x00);
#else
	sc401ai_write_register(ViPipe, 0x3e02, 0x60);
#endif
	sc401ai_write_register(ViPipe, 0x3e03, 0x0b);
	sc401ai_write_register(ViPipe, 0x3e08, 0x03);
	sc401ai_write_register(ViPipe, 0x3e09, 0x40);
	sc401ai_write_register(ViPipe, 0x3e1b, 0x2a);
	sc401ai_write_register(ViPipe, 0x4509, 0x30);
#ifdef MIPI_2LANE
	sc401ai_write_register(ViPipe, 0x4819, 0x08);
	sc401ai_write_register(ViPipe, 0x481b, 0x05);
	sc401ai_write_register(ViPipe, 0x481d, 0x11);
	sc401ai_write_register(ViPipe, 0x481f, 0x04);
	sc401ai_write_register(ViPipe, 0x4821, 0x09);
	sc401ai_write_register(ViPipe, 0x4823, 0x04);
	sc401ai_write_register(ViPipe, 0x4825, 0x04);
	sc401ai_write_register(ViPipe, 0x4827, 0x04);
	sc401ai_write_register(ViPipe, 0x4829, 0x07);
#endif
	sc401ai_write_register(ViPipe, 0x5001, 0x44);
	sc401ai_write_register(ViPipe, 0x57a8, 0xd0);
	sc401ai_write_register(ViPipe, 0x36e9, 0x14);
	sc401ai_write_register(ViPipe, 0x36f9, 0x14);
	sc401ai_write_register(ViPipe, 0x0100, 0x01);

	printf("=========================================================\n");
#ifdef MIPI_2LANE
	printf("===Smart sc401ai sensor 4M(MIPI_2LANE) init success!=====\n");
#else
	printf("===Smart sc401ai sensor 4M(MIPI_4LANE) init success!=====\n");
#endif
	printf("=========================================================\n");
	return;
}
