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

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#ifdef __LITEOS__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif

const unsigned char sc3235_i2c_addr = 0x60; /* I2C Address of SC3235 */
const unsigned int sc3235_addr_byte = 2;
const unsigned int sc3235_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastSc3235[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunSc3235BusInfo[];

int sc3235_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunSc3235BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc3235_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int sc3235_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int sc3235_read_register(VI_PIPE ViPipe, GK_U32 addr)
{
	GK_S32 s32RegVal = 0;
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sc3235_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret = 0;
	GK_U32 u32RegWidth = sc3235_addr_byte;
	GK_U32 u32DataWidth = sc3235_data_byte;
	GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (sc3235_i2c_addr >> 1);
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

int sc3235_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = sc3235_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = sc3235_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = sc3235_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (sc3235_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (sc3235_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, sc3235_addr_byte + sc3235_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void sc3235_standby(VI_PIPE ViPipe)
{
	return;
}

void sc3235_restart(VI_PIPE ViPipe)
{
	return;
}

#define SC3235_SENSOR_1296P_30FPS_LINEAR_MODE (1)

void sc3235_linear_1296p30_init(VI_PIPE ViPipe);

void sc3235_init(VI_PIPE ViPipe)
{
	GK_U32 i;

	sc3235_i2c_init(ViPipe);

	sc3235_linear_1296p30_init(ViPipe);

	for (i = 0; i < g_pastSc3235[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		sc3235_write_register(ViPipe,
				      g_pastSc3235[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32RegAddr,
				      g_pastSc3235[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32Data);
	}

	g_pastSc3235[ViPipe]->bInit = GK_TRUE;

	return;
}

void sc3235_exit(VI_PIPE ViPipe)
{
	sc3235_i2c_exit(ViPipe);

	return;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

/* 1296P30 (2304X1296@30fps) */
void sc3235_linear_1296p30_init(VI_PIPE ViPipe)
{
	sc3235_write_register(ViPipe, 0x0103, 0x01);
	sc3235_write_register(ViPipe, 0x0100, 0x00);
	sc3235_write_register(ViPipe, 0x36e9, 0x87);
	sc3235_write_register(ViPipe, 0x36f9, 0xb3);
	sc3235_write_register(ViPipe, 0x301f, 0x01);
	sc3235_write_register(ViPipe, 0x3038, 0x66);
	sc3235_write_register(ViPipe, 0x3253, 0x08);
	sc3235_write_register(ViPipe, 0x3301, 0x11);
	sc3235_write_register(ViPipe, 0x3304, 0x30);
	sc3235_write_register(ViPipe, 0x3306, 0x50);
	sc3235_write_register(ViPipe, 0x330a, 0x00);
	sc3235_write_register(ViPipe, 0x330b, 0xd0);
	sc3235_write_register(ViPipe, 0x330e, 0x30);
	sc3235_write_register(ViPipe, 0x3314, 0x94);
	sc3235_write_register(ViPipe, 0x331c, 0x01);
	sc3235_write_register(ViPipe, 0x331e, 0x29);
	sc3235_write_register(ViPipe, 0x3320, 0x03);
	sc3235_write_register(ViPipe, 0x3347, 0x05);
	sc3235_write_register(ViPipe, 0x334c, 0x10);
	sc3235_write_register(ViPipe, 0x3356, 0x01);
	sc3235_write_register(ViPipe, 0x3364, 0x17);
	sc3235_write_register(ViPipe, 0x3367, 0x10);
	sc3235_write_register(ViPipe, 0x3368, 0x04);
	sc3235_write_register(ViPipe, 0x3369, 0x00);
	sc3235_write_register(ViPipe, 0x336a, 0x00);
	sc3235_write_register(ViPipe, 0x336b, 0x00);
	sc3235_write_register(ViPipe, 0x3390, 0x08);
	sc3235_write_register(ViPipe, 0x3391, 0x38);
	sc3235_write_register(ViPipe, 0x3392, 0x38);
	sc3235_write_register(ViPipe, 0x3393, 0x1a);
	sc3235_write_register(ViPipe, 0x3394, 0x88);
	sc3235_write_register(ViPipe, 0x3395, 0x88);
	sc3235_write_register(ViPipe, 0x360f, 0x05);
	sc3235_write_register(ViPipe, 0x3614, 0x80);
	sc3235_write_register(ViPipe, 0x3622, 0xf6);
	sc3235_write_register(ViPipe, 0x3630, 0xc3);
	sc3235_write_register(ViPipe, 0x3631, 0x8a);
	sc3235_write_register(ViPipe, 0x3632, 0x18);
	sc3235_write_register(ViPipe, 0x3633, 0x44);
	sc3235_write_register(ViPipe, 0x3635, 0x20);
	sc3235_write_register(ViPipe, 0x3637, 0x2c);
	sc3235_write_register(ViPipe, 0x3638, 0x28);
	sc3235_write_register(ViPipe, 0x363a, 0xa8);
	sc3235_write_register(ViPipe, 0x363b, 0x20);
	sc3235_write_register(ViPipe, 0x363c, 0x06);
	sc3235_write_register(ViPipe, 0x3641, 0x00);
	sc3235_write_register(ViPipe, 0x3670, 0x0a);
	sc3235_write_register(ViPipe, 0x3671, 0xf6);
	sc3235_write_register(ViPipe, 0x3672, 0x76);
	sc3235_write_register(ViPipe, 0x3673, 0x16);
	sc3235_write_register(ViPipe, 0x3674, 0xa0);
	sc3235_write_register(ViPipe, 0x3675, 0x98);
	sc3235_write_register(ViPipe, 0x3676, 0x6a);
	sc3235_write_register(ViPipe, 0x367a, 0x08);
	sc3235_write_register(ViPipe, 0x367b, 0x38);
	sc3235_write_register(ViPipe, 0x367c, 0x08);
	sc3235_write_register(ViPipe, 0x367d, 0x38);
	sc3235_write_register(ViPipe, 0x3690, 0x64);
	sc3235_write_register(ViPipe, 0x3691, 0x63);
	sc3235_write_register(ViPipe, 0x3692, 0x64);
	sc3235_write_register(ViPipe, 0x369c, 0x08);
	sc3235_write_register(ViPipe, 0x369d, 0x38);
	sc3235_write_register(ViPipe, 0x36eb, 0x05);
	sc3235_write_register(ViPipe, 0x36ec, 0x15);
	sc3235_write_register(ViPipe, 0x36ed, 0x04);
	sc3235_write_register(ViPipe, 0x36fb, 0x23);
	sc3235_write_register(ViPipe, 0x36fc, 0x01);
	sc3235_write_register(ViPipe, 0x36fd, 0x14);
	sc3235_write_register(ViPipe, 0x3900, 0x29);
	sc3235_write_register(ViPipe, 0x3902, 0xc5);
	sc3235_write_register(ViPipe, 0x3905, 0xd1);
	sc3235_write_register(ViPipe, 0x3906, 0x62);
	sc3235_write_register(ViPipe, 0x3908, 0x41);
	sc3235_write_register(ViPipe, 0x3909, 0x00);
	sc3235_write_register(ViPipe, 0x390a, 0x19);
	sc3235_write_register(ViPipe, 0x390b, 0x00);
	sc3235_write_register(ViPipe, 0x390c, 0x4c);
	sc3235_write_register(ViPipe, 0x390d, 0x00);
	sc3235_write_register(ViPipe, 0x390e, 0x19);
	sc3235_write_register(ViPipe, 0x390f, 0x00);
	sc3235_write_register(ViPipe, 0x3910, 0x4c);
	sc3235_write_register(ViPipe, 0x391d, 0x01); //0x4
	sc3235_write_register(ViPipe, 0x391e, 0x00);
	sc3235_write_register(ViPipe, 0x3920, 0x00);
	sc3235_write_register(ViPipe, 0x3921, 0x4c);
	sc3235_write_register(ViPipe, 0x3922, 0x00);
	sc3235_write_register(ViPipe, 0x3923, 0x19);
	sc3235_write_register(ViPipe, 0x3924, 0x00);
	sc3235_write_register(ViPipe, 0x3925, 0x4c);
	sc3235_write_register(ViPipe, 0x3926, 0x00);
	sc3235_write_register(ViPipe, 0x3927, 0x19);
	sc3235_write_register(ViPipe, 0x3933, 0x0a);
	sc3235_write_register(ViPipe, 0x3934, 0x28);
	sc3235_write_register(ViPipe, 0x3935, 0x18);
	sc3235_write_register(ViPipe, 0x3936, 0x08);
	sc3235_write_register(ViPipe, 0x3937, 0x13);
	sc3235_write_register(ViPipe, 0x3940, 0x68);
	sc3235_write_register(ViPipe, 0x3942, 0x02);
	sc3235_write_register(ViPipe, 0x3943, 0x33);
	sc3235_write_register(ViPipe, 0x3e01, 0xa8);
	sc3235_write_register(ViPipe, 0x3e02, 0x40);
	sc3235_write_register(ViPipe, 0x3e09, 0x20);
	sc3235_write_register(ViPipe, 0x3e1b, 0x35);
	sc3235_write_register(ViPipe, 0x3e25, 0x03);
	sc3235_write_register(ViPipe, 0x3e26, 0x20);
	sc3235_write_register(ViPipe, 0x5000, 0x00); //0x06
	sc3235_write_register(ViPipe, 0x5002, 0x00); //0x06
	sc3235_write_register(ViPipe, 0x5781, 0x04);
	sc3235_write_register(ViPipe, 0x5782, 0x04);
	sc3235_write_register(ViPipe, 0x5783, 0x02);
	sc3235_write_register(ViPipe, 0x5784, 0x02);
	sc3235_write_register(ViPipe, 0x5785, 0x40);
	sc3235_write_register(ViPipe, 0x5786, 0x20);
	sc3235_write_register(ViPipe, 0x5787, 0x18);
	sc3235_write_register(ViPipe, 0x5788, 0x10);
	sc3235_write_register(ViPipe, 0x5789, 0x10);
	sc3235_write_register(ViPipe, 0x578a, 0x30);
	sc3235_write_register(ViPipe, 0x57a4, 0xa0);
	sc3235_write_register(ViPipe, 0x36e9, 0x07);
	sc3235_write_register(ViPipe, 0x36f9, 0x33);
	sc3235_write_register(ViPipe, 0x0100, 0x01);

	sc3235_write_register(ViPipe, 0x3221, 0x66); // mirror & filp
	delay_ms(10);

	printf("==============================================================\n");
	printf("===Smart sc3235 sensor 1296P30fps(MIPI) init success!=====\n");
	printf("==============================================================\n");

	return;
}
