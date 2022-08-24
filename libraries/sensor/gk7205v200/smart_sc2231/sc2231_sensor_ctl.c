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

#include "gk_api_isp.h"
const unsigned char sc2231_i2c_addr = 0x60; /* I2C Address of sc2231 */
const unsigned int sc2231_addr_byte = 2;
const unsigned int sc2231_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastSc2231[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunSc2231BusInfo[];

GK_S32 sc2231_GetQuickStartFlag(VI_PIPE ViPipe, GK_U32 *quickStartFlag)
{
	GK_S32 s32Ret = 0;
	ISP_MOD_PARAM_S stModParam;
	s32Ret = GK_API_ISP_GetModParam(&stModParam);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "sc2231 Get QuickStart Error\n");
		return s32Ret;
	}
	*quickStartFlag = stModParam.u32QuickStart;
	return GK_SUCCESS;
}

int sc2231_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunSc2231BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc2231_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int sc2231_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int sc2231_read_register(VI_PIPE ViPipe, GK_U32 addr)
{
	GK_S32 s32RegVal = 0;
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sc2231_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret = 0;
	GK_U32 u32RegWidth = sc2231_addr_byte;
	GK_U32 u32DataWidth = sc2231_data_byte;
	GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (sc2231_i2c_addr >> 1);
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

int sc2231_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = sc2231_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = sc2231_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = sc2231_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (sc2231_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	}
	buf[idx] = data & 0xff;
	idx++;

	ret = write(g_fd[ViPipe], buf, sc2231_addr_byte + sc2231_data_byte);
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

void sc2231_standby(VI_PIPE ViPipe)
{
	return;
}

void sc2231_restart(VI_PIPE ViPipe)
{
	return;
}

#define SC2231_SENSOR_1080P_30FPS_LINEAR_MODE (1)

void sc2231_linear_1080P30_init(VI_PIPE ViPipe);

void sc2231_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	GK_U32 quickStartFlag = 0;
	sc2231_GetQuickStartFlag(ViPipe, &quickStartFlag);
	sc2231_i2c_init(ViPipe);
	/* When sensor first init, config all registers */
	if (quickStartFlag == 0) {
		sc2231_linear_1080P30_init(ViPipe);
		delay_ms(10);
		for (i = 0; i < g_pastSc2231[ViPipe]->astRegsInfo[0].u32RegNum;
		     i++) {
			sc2231_write_register(ViPipe,
					      g_pastSc2231[ViPipe]
						      ->astRegsInfo[0]
						      .astI2cData[i]
						      .u32RegAddr,
					      g_pastSc2231[ViPipe]
						      ->astRegsInfo[0]
						      .astI2cData[i]
						      .u32Data);
		}
	}
	g_pastSc2231[ViPipe]->bInit = GK_TRUE;

	return;
}

void sc2231_exit(VI_PIPE ViPipe)
{
	sc2231_i2c_exit(ViPipe);

	return;
}

/***************** 1080P30 (1920X1080@30fps) ***************/
void sc2231_linear_1080P30_init(VI_PIPE ViPipe)
{
	sc2231_write_register(ViPipe, 0x0103, 0x01);
	sc2231_write_register(ViPipe, 0x0100, 0x00);
	sc2231_write_register(ViPipe, 0x3000, 0x00);
	sc2231_write_register(ViPipe, 0x3001, 0x00);
	sc2231_write_register(ViPipe, 0x3002, 0x80);
	sc2231_write_register(ViPipe, 0x3018, 0x33);
	sc2231_write_register(ViPipe, 0x301a, 0xf0);
	sc2231_write_register(ViPipe, 0x301c, 0x78);
	sc2231_write_register(ViPipe, 0x3461, 0x0d);
	sc2231_write_register(ViPipe, 0x3037, 0x20);
	sc2231_write_register(ViPipe, 0x3038, 0x6e);
	sc2231_write_register(ViPipe, 0x3039, 0x52);
	sc2231_write_register(ViPipe, 0x303b, 0x14);
	sc2231_write_register(ViPipe, 0x303d, 0x10);
	sc2231_write_register(ViPipe, 0x303f, 0x01);
	sc2231_write_register(ViPipe, 0x3461, 0x01);
	sc2231_write_register(ViPipe, 0x3221, 0x80);
	sc2231_write_register(ViPipe, 0x3301, 0x08);
	sc2231_write_register(ViPipe, 0x3303, 0x38);
	sc2231_write_register(ViPipe, 0x3306, 0x50);
	sc2231_write_register(ViPipe, 0x3309, 0x70);
	sc2231_write_register(ViPipe, 0x330a, 0x00);
	sc2231_write_register(ViPipe, 0x330b, 0xd0);
	sc2231_write_register(ViPipe, 0x330d, 0x36);
	sc2231_write_register(ViPipe, 0x330e, 0x18);
	sc2231_write_register(ViPipe, 0x330f, 0x01);
	sc2231_write_register(ViPipe, 0x3310, 0x23);
	sc2231_write_register(ViPipe, 0x3314, 0x14);
	sc2231_write_register(ViPipe, 0x331e, 0x31);
	sc2231_write_register(ViPipe, 0x331f, 0x69);
	sc2231_write_register(ViPipe, 0x3338, 0x37);
	sc2231_write_register(ViPipe, 0x3339, 0x37);
	sc2231_write_register(ViPipe, 0x333a, 0x33);
	sc2231_write_register(ViPipe, 0x335d, 0x20);
	sc2231_write_register(ViPipe, 0x3364, 0x1d);
	sc2231_write_register(ViPipe, 0x3367, 0x08);
	sc2231_write_register(ViPipe, 0x33aa, 0x00);
	sc2231_write_register(ViPipe, 0x33ae, 0x32);
	sc2231_write_register(ViPipe, 0x33b3, 0x32);
	sc2231_write_register(ViPipe, 0x33b4, 0x32);
	sc2231_write_register(ViPipe, 0x33b6, 0x0f);
	sc2231_write_register(ViPipe, 0x33b7, 0x3e);
	sc2231_write_register(ViPipe, 0x33b8, 0x08);
	sc2231_write_register(ViPipe, 0x33b9, 0x80);
	sc2231_write_register(ViPipe, 0x33ba, 0xc0);
	sc2231_write_register(ViPipe, 0x360f, 0x05);
	sc2231_write_register(ViPipe, 0x3614, 0x80);
	sc2231_write_register(ViPipe, 0x3622, 0xf6);
	sc2231_write_register(ViPipe, 0x3630, 0x40);
	sc2231_write_register(ViPipe, 0x3631, 0x88);
	sc2231_write_register(ViPipe, 0x3632, 0x88);
	sc2231_write_register(ViPipe, 0x3633, 0x24);
	sc2231_write_register(ViPipe, 0x3635, 0x1c);
	sc2231_write_register(ViPipe, 0x3637, 0x2c);
	sc2231_write_register(ViPipe, 0x3638, 0x24);
	sc2231_write_register(ViPipe, 0x363a, 0x80);
	sc2231_write_register(ViPipe, 0x363b, 0x16);
	sc2231_write_register(ViPipe, 0x363c, 0x06);
	sc2231_write_register(ViPipe, 0x366e, 0x04);
	sc2231_write_register(ViPipe, 0x3670, 0x48);
	sc2231_write_register(ViPipe, 0x3671, 0xf6);
	sc2231_write_register(ViPipe, 0x3672, 0x16);
	sc2231_write_register(ViPipe, 0x3673, 0x16);
	sc2231_write_register(ViPipe, 0x367a, 0x38);
	sc2231_write_register(ViPipe, 0x367b, 0x38);
	sc2231_write_register(ViPipe, 0x3690, 0x24);
	sc2231_write_register(ViPipe, 0x3691, 0x44);
	sc2231_write_register(ViPipe, 0x3692, 0x44);
	sc2231_write_register(ViPipe, 0x3699, 0x80);
	sc2231_write_register(ViPipe, 0x369a, 0x80);
	sc2231_write_register(ViPipe, 0x369b, 0x9f);
	sc2231_write_register(ViPipe, 0x369c, 0x38);
	sc2231_write_register(ViPipe, 0x369d, 0x38);
	sc2231_write_register(ViPipe, 0x36a2, 0x00);
	sc2231_write_register(ViPipe, 0x36a3, 0x3f);
	sc2231_write_register(ViPipe, 0x3902, 0xc5);
	sc2231_write_register(ViPipe, 0x391e, 0x00);
	sc2231_write_register(ViPipe, 0x3933, 0x0a);
	sc2231_write_register(ViPipe, 0x3934, 0x00);
	sc2231_write_register(ViPipe, 0x3940, 0x69);
	sc2231_write_register(ViPipe, 0x3942, 0x02);
	sc2231_write_register(ViPipe, 0x3943, 0x18);
	sc2231_write_register(ViPipe, 0x3e09, 0x20);
	sc2231_write_register(ViPipe, 0x3e26, 0x20);
	sc2231_write_register(ViPipe, 0x3f00, 0x0f);
	sc2231_write_register(ViPipe, 0x3f04, 0x04);
	sc2231_write_register(ViPipe, 0x3f05, 0x28);
	sc2231_write_register(ViPipe, 0x4603, 0x00);
	sc2231_write_register(ViPipe, 0x4800, 0x64);
	sc2231_write_register(ViPipe, 0x4837, 0x35);
	sc2231_write_register(ViPipe, 0x5000, 0x00);
	sc2231_write_register(ViPipe, 0x5002, 0x00);
	sc2231_write_register(ViPipe, 0x5988, 0x02);
	sc2231_write_register(ViPipe, 0x598e, 0x08);
	sc2231_write_register(ViPipe, 0x598f, 0x50);

	sc2231_write_register(ViPipe, 0x3221, 0x66); // mirror & filp

	sc2231_write_register(ViPipe, 0x0100, 0x01);

	printf("============================================================================\n");
	printf("===Smart Sc2231 27MInput 2lane 371.25Mbps 1080P30fps(MIPI) init success!=====\n");
	printf("============================================================================\n");
	return;
}
