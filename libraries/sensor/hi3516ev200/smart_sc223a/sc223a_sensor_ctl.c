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

#define LOSCFG_HOST_TYPE_VENDOR
#ifdef LOSCFG_HOST_TYPE_VENDOR
#include <linux/fb.h>
#endif

const unsigned char sc223a_i2c_addr = 0x60; /* I2C Address of SC223a */
const unsigned int sc223a_addr_byte = 2;
const unsigned int sc223a_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastSc223a[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunSc223aBusInfo[];

int sc223a_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunSc223aBusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);
	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc223a_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int sc223a_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;
	unsigned int nmsgs;
};

int sc223a_read_register(VI_PIPE ViPipe, GK_U32 addr)
{
	GK_S32 s32RegVal = 0;

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sc223a_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret = 0;
	GK_U32 u32RegWidth = sc223a_addr_byte;
	GK_U32 u32DataWidth = sc223a_data_byte;
	GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (sc223a_i2c_addr >> 1);
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

int sc223a_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = sc223a_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = sc223a_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = sc223a_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (sc223a_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (sc223a_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, sc223a_addr_byte + sc223a_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void sc223a_standby(VI_PIPE ViPipe)
{
	return;
}

void sc223a_restart(VI_PIPE ViPipe)
{
	return;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

#define SC223a_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define SC223a_SENSOR_1080P_30FPS_2t1_WDR_MODE (2)

void sc223a_linear_1080p30_init(VI_PIPE ViPipe);
void sc223a_wdr_1080p30_init(VI_PIPE ViPipe);
void sc223a_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	for (i = 0; i < g_pastSc223a[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		sc223a_write_register(ViPipe,
				      g_pastSc223a[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32RegAddr,
				      g_pastSc223a[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32Data);
	}
}
void sc223a_init(VI_PIPE ViPipe)
{
	GK_U8 u8ImgMode;

	u8ImgMode = g_pastSc223a[ViPipe]->u8ImgMode;
	sc223a_i2c_init(ViPipe);
	switch (u8ImgMode) {
	case SC223a_SENSOR_1080P_30FPS_2t1_WDR_MODE:
		sc223a_wdr_1080p30_init(ViPipe);
		break;
	case SC223a_SENSOR_1080P_30FPS_LINEAR_MODE:
		sc223a_linear_1080p30_init(ViPipe);
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n",
			  u8ImgMode);
		break;
	}

	delay_ms(10);
	sc223a_default_reg_init(ViPipe);
	g_pastSc223a[ViPipe]->bInit = GK_TRUE;
	return;
}

void sc223a_exit(VI_PIPE ViPipe)
{
	sc223a_i2c_exit(ViPipe);

	return;
}

/* cleaned_0x01_FT_SC223A_MIPI_27Minput_2lane_371.25Mbps_10bit_1920x1080_30fps.ini*/
void sc223a_linear_1080p30_init(VI_PIPE ViPipe)
{
	sc223a_write_register(ViPipe, 0x0100, 0x00);
	sc223a_write_register(ViPipe, 0x36e9, 0x80);
	sc223a_write_register(ViPipe, 0x37f9, 0x80);
	sc223a_write_register(ViPipe, 0x301f, 0x01);
	sc223a_write_register(ViPipe, 0x3253, 0x0c);
	sc223a_write_register(ViPipe, 0x3281, 0x80);
	sc223a_write_register(ViPipe, 0x3301, 0x08);
	sc223a_write_register(ViPipe, 0x3302, 0x12);
	sc223a_write_register(ViPipe, 0x3306, 0xb0);
	sc223a_write_register(ViPipe, 0x3309, 0x60);
	sc223a_write_register(ViPipe, 0x330a, 0x01);
	sc223a_write_register(ViPipe, 0x330b, 0x60);
	sc223a_write_register(ViPipe, 0x330d, 0x20);
	sc223a_write_register(ViPipe, 0x331e, 0x41);
	sc223a_write_register(ViPipe, 0x331f, 0x51);
	sc223a_write_register(ViPipe, 0x3320, 0x0a);
	sc223a_write_register(ViPipe, 0x3326, 0x0e);
	sc223a_write_register(ViPipe, 0x3333, 0x10);
	sc223a_write_register(ViPipe, 0x3334, 0x40);
	sc223a_write_register(ViPipe, 0x335d, 0x60);
	sc223a_write_register(ViPipe, 0x335e, 0x06);
	sc223a_write_register(ViPipe, 0x335f, 0x08);
	sc223a_write_register(ViPipe, 0x3364, 0x56);
	sc223a_write_register(ViPipe, 0x337a, 0x06);
	sc223a_write_register(ViPipe, 0x337b, 0x0e);
	sc223a_write_register(ViPipe, 0x337c, 0x02);
	sc223a_write_register(ViPipe, 0x337d, 0x0a);
	sc223a_write_register(ViPipe, 0x3390, 0x03);
	sc223a_write_register(ViPipe, 0x3391, 0x0f);
	sc223a_write_register(ViPipe, 0x3392, 0x1f);
	sc223a_write_register(ViPipe, 0x3393, 0x08);
	sc223a_write_register(ViPipe, 0x3394, 0x08);
	sc223a_write_register(ViPipe, 0x3395, 0x08);
	sc223a_write_register(ViPipe, 0x3396, 0x41);
	sc223a_write_register(ViPipe, 0x3397, 0x47);
	sc223a_write_register(ViPipe, 0x3398, 0x5f);
	sc223a_write_register(ViPipe, 0x3399, 0x08);
	sc223a_write_register(ViPipe, 0x339a, 0x10);
	sc223a_write_register(ViPipe, 0x339b, 0x38);
	sc223a_write_register(ViPipe, 0x339c, 0x3c);
	sc223a_write_register(ViPipe, 0x33a2, 0x04);
	sc223a_write_register(ViPipe, 0x33a3, 0x0a);
	sc223a_write_register(ViPipe, 0x33ad, 0x18);
	sc223a_write_register(ViPipe, 0x33af, 0x40);
	sc223a_write_register(ViPipe, 0x33b1, 0x80);
	sc223a_write_register(ViPipe, 0x33b3, 0x20);
	sc223a_write_register(ViPipe, 0x349f, 0x02);
	sc223a_write_register(ViPipe, 0x34a6, 0x41);
	sc223a_write_register(ViPipe, 0x34a7, 0x47);
	sc223a_write_register(ViPipe, 0x34a8, 0x20);
	sc223a_write_register(ViPipe, 0x34a9, 0x20);
	sc223a_write_register(ViPipe, 0x34f8, 0x5f);
	sc223a_write_register(ViPipe, 0x34f9, 0x20);
	sc223a_write_register(ViPipe, 0x3630, 0xc0);
	sc223a_write_register(ViPipe, 0x3631, 0x86);
	sc223a_write_register(ViPipe, 0x3632, 0x26);
	sc223a_write_register(ViPipe, 0x3633, 0x32);
	sc223a_write_register(ViPipe, 0x363a, 0x84);
	sc223a_write_register(ViPipe, 0x3641, 0x3a);
	sc223a_write_register(ViPipe, 0x3670, 0x4e);
	sc223a_write_register(ViPipe, 0x3674, 0xc0);
	sc223a_write_register(ViPipe, 0x3675, 0xc0);
	sc223a_write_register(ViPipe, 0x3676, 0xc0);
	sc223a_write_register(ViPipe, 0x3677, 0x86);
	sc223a_write_register(ViPipe, 0x3678, 0x8b);
	sc223a_write_register(ViPipe, 0x3679, 0x8c);
	sc223a_write_register(ViPipe, 0x367c, 0x47);
	sc223a_write_register(ViPipe, 0x367d, 0x5f);
	sc223a_write_register(ViPipe, 0x367e, 0x47);
	sc223a_write_register(ViPipe, 0x367f, 0x5f);
	sc223a_write_register(ViPipe, 0x3690, 0x42);
	sc223a_write_register(ViPipe, 0x3691, 0x43);
	sc223a_write_register(ViPipe, 0x3692, 0x63);
	sc223a_write_register(ViPipe, 0x3699, 0x84);
	sc223a_write_register(ViPipe, 0x369a, 0x8c);
	sc223a_write_register(ViPipe, 0x369b, 0xa4);
	sc223a_write_register(ViPipe, 0x369c, 0x41);
	sc223a_write_register(ViPipe, 0x369d, 0x47);
	sc223a_write_register(ViPipe, 0x36a2, 0x41);
	sc223a_write_register(ViPipe, 0x36a3, 0x47);
	sc223a_write_register(ViPipe, 0x370f, 0x01);
	sc223a_write_register(ViPipe, 0x3721, 0x6c);
	sc223a_write_register(ViPipe, 0x3722, 0x09);
	sc223a_write_register(ViPipe, 0x3724, 0x41);
	sc223a_write_register(ViPipe, 0x3725, 0xc4);
	sc223a_write_register(ViPipe, 0x37b0, 0x09);
	sc223a_write_register(ViPipe, 0x37b1, 0x09);
	sc223a_write_register(ViPipe, 0x37b2, 0x05);
	sc223a_write_register(ViPipe, 0x37b3, 0x41);
	sc223a_write_register(ViPipe, 0x37b4, 0x5f);
	sc223a_write_register(ViPipe, 0x3901, 0x02);
	sc223a_write_register(ViPipe, 0x3e01, 0x8c);
	sc223a_write_register(ViPipe, 0x4509, 0x28);
	sc223a_write_register(ViPipe, 0x450d, 0x11);
	sc223a_write_register(ViPipe, 0x4518, 0x00);
	sc223a_write_register(ViPipe, 0x5799, 0x06);
	sc223a_write_register(ViPipe, 0x5ae0, 0xfe);
	sc223a_write_register(ViPipe, 0x5ae1, 0x40);
	sc223a_write_register(ViPipe, 0x5ae2, 0x38);
	sc223a_write_register(ViPipe, 0x5ae3, 0x30);
	sc223a_write_register(ViPipe, 0x5ae4, 0x28);
	sc223a_write_register(ViPipe, 0x5ae5, 0x38);
	sc223a_write_register(ViPipe, 0x5ae6, 0x30);
	sc223a_write_register(ViPipe, 0x5ae7, 0x28);
	sc223a_write_register(ViPipe, 0x5ae8, 0x3f);
	sc223a_write_register(ViPipe, 0x5ae9, 0x34);
	sc223a_write_register(ViPipe, 0x5aea, 0x2c);
	sc223a_write_register(ViPipe, 0x5aeb, 0x3f);
	sc223a_write_register(ViPipe, 0x5aec, 0x34);
	sc223a_write_register(ViPipe, 0x5aed, 0x2c);
	sc223a_write_register(ViPipe, 0x5aee, 0xfe);
	sc223a_write_register(ViPipe, 0x5aef, 0x40);
	sc223a_write_register(ViPipe, 0x5af4, 0x38);
	sc223a_write_register(ViPipe, 0x5af5, 0x30);
	sc223a_write_register(ViPipe, 0x5af6, 0x28);
	sc223a_write_register(ViPipe, 0x5af7, 0x38);
	sc223a_write_register(ViPipe, 0x5af8, 0x30);
	sc223a_write_register(ViPipe, 0x5af9, 0x28);
	sc223a_write_register(ViPipe, 0x5afa, 0x3f);
	sc223a_write_register(ViPipe, 0x5afb, 0x34);
	sc223a_write_register(ViPipe, 0x5afc, 0x2c);
	sc223a_write_register(ViPipe, 0x5afd, 0x3f);
	sc223a_write_register(ViPipe, 0x5afe, 0x34);
	sc223a_write_register(ViPipe, 0x5aff, 0x2c);
	sc223a_write_register(ViPipe, 0x36e9, 0x20);
	sc223a_write_register(ViPipe, 0x37f9, 0x27);
	sc223a_write_register(ViPipe, 0x0100, 0x01);

	printf("=== Smart SC223A_1080P_30FPS_10BIT_LINE_Init_OK!===\n");
	return;
}

/* cleaned_0x15_SC223A_MIPI_27Minput_2lane_742.5Mbps_10bit_1920x1080_30fps_SHDR_VC.ini */
void sc223a_wdr_1080p30_init(VI_PIPE ViPipe)
{
	sc223a_write_register(ViPipe, 0x0103, 0x01);
	sc223a_write_register(ViPipe, 0x0100, 0x00);
	sc223a_write_register(ViPipe, 0x36e9, 0x80);
	sc223a_write_register(ViPipe, 0x36f9, 0x80);
	sc223a_write_register(ViPipe, 0x301f, 0x02);
	sc223a_write_register(ViPipe, 0x320e, 0x08);
	sc223a_write_register(ViPipe, 0x320f, 0xca);
	sc223a_write_register(ViPipe, 0x3220, 0x53);
	sc223a_write_register(ViPipe, 0x3243, 0x01);
	sc223a_write_register(ViPipe, 0x3248, 0x02);
	sc223a_write_register(ViPipe, 0x3249, 0x09);
	sc223a_write_register(ViPipe, 0x3250, 0x3f);
	sc223a_write_register(ViPipe, 0x3253, 0x08);
	sc223a_write_register(ViPipe, 0x3271, 0x0a);
	sc223a_write_register(ViPipe, 0x3301, 0x06);
	sc223a_write_register(ViPipe, 0x3302, 0x0c);
	sc223a_write_register(ViPipe, 0x3303, 0x08);
	sc223a_write_register(ViPipe, 0x3304, 0x60);
	sc223a_write_register(ViPipe, 0x3306, 0x30);
	sc223a_write_register(ViPipe, 0x3308, 0x10);
	sc223a_write_register(ViPipe, 0x3309, 0x70);
	sc223a_write_register(ViPipe, 0x330b, 0x80);
	sc223a_write_register(ViPipe, 0x330d, 0x16);
	sc223a_write_register(ViPipe, 0x330e, 0x1c);
	sc223a_write_register(ViPipe, 0x330f, 0x02);
	sc223a_write_register(ViPipe, 0x3310, 0x02);
	sc223a_write_register(ViPipe, 0x331c, 0x04);
	sc223a_write_register(ViPipe, 0x331e, 0x51);
	sc223a_write_register(ViPipe, 0x331f, 0x61);
	sc223a_write_register(ViPipe, 0x3320, 0x07);
	sc223a_write_register(ViPipe, 0x3333, 0x10);
	sc223a_write_register(ViPipe, 0x334c, 0x08);
	sc223a_write_register(ViPipe, 0x3356, 0x09);
	sc223a_write_register(ViPipe, 0x3364, 0x17);
	sc223a_write_register(ViPipe, 0x3390, 0x08);
	sc223a_write_register(ViPipe, 0x3391, 0x18);
	sc223a_write_register(ViPipe, 0x3392, 0x38);
	sc223a_write_register(ViPipe, 0x3393, 0x06);
	sc223a_write_register(ViPipe, 0x3394, 0x06);
	sc223a_write_register(ViPipe, 0x3395, 0x06);
	sc223a_write_register(ViPipe, 0x3396, 0x08);
	sc223a_write_register(ViPipe, 0x3397, 0x18);
	sc223a_write_register(ViPipe, 0x3398, 0x38);
	sc223a_write_register(ViPipe, 0x3399, 0x06);
	sc223a_write_register(ViPipe, 0x339a, 0x0a);
	sc223a_write_register(ViPipe, 0x339b, 0x10);
	sc223a_write_register(ViPipe, 0x339c, 0x20);
	sc223a_write_register(ViPipe, 0x33ac, 0x08);
	sc223a_write_register(ViPipe, 0x33ae, 0x10);
	sc223a_write_register(ViPipe, 0x33af, 0x19);
	sc223a_write_register(ViPipe, 0x3621, 0xe8);
	sc223a_write_register(ViPipe, 0x3622, 0x16);
	sc223a_write_register(ViPipe, 0x3630, 0xa0);
	sc223a_write_register(ViPipe, 0x3637, 0x36);
	sc223a_write_register(ViPipe, 0x363a, 0x1f);
	sc223a_write_register(ViPipe, 0x363b, 0xc6);
	sc223a_write_register(ViPipe, 0x363c, 0x0e);
	sc223a_write_register(ViPipe, 0x3670, 0x0a);
	sc223a_write_register(ViPipe, 0x3674, 0x82);
	sc223a_write_register(ViPipe, 0x3675, 0x76);
	sc223a_write_register(ViPipe, 0x3676, 0x78);
	sc223a_write_register(ViPipe, 0x367c, 0x48);
	sc223a_write_register(ViPipe, 0x367d, 0x58);
	sc223a_write_register(ViPipe, 0x3690, 0x34);
	sc223a_write_register(ViPipe, 0x3691, 0x33);
	sc223a_write_register(ViPipe, 0x3692, 0x44);
	sc223a_write_register(ViPipe, 0x369c, 0x40);
	sc223a_write_register(ViPipe, 0x369d, 0x48);
	sc223a_write_register(ViPipe, 0x36eb, 0x0c);
	sc223a_write_register(ViPipe, 0x36ec, 0x0c);
	sc223a_write_register(ViPipe, 0x36fd, 0x14);
	sc223a_write_register(ViPipe, 0x3901, 0x02);
	sc223a_write_register(ViPipe, 0x3904, 0x04);
	sc223a_write_register(ViPipe, 0x3908, 0x41);
	sc223a_write_register(ViPipe, 0x391f, 0x10);
	sc223a_write_register(ViPipe, 0x3e00, 0x01);
	sc223a_write_register(ViPipe, 0x3e01, 0x06);
	sc223a_write_register(ViPipe, 0x3e02, 0x00);
	sc223a_write_register(ViPipe, 0x3e04, 0x10);
	sc223a_write_register(ViPipe, 0x3e05, 0x60);
	sc223a_write_register(ViPipe, 0x3e06, 0x00);
	sc223a_write_register(ViPipe, 0x3e07, 0x80);
	sc223a_write_register(ViPipe, 0x3e08, 0x03);
	sc223a_write_register(ViPipe, 0x3e09, 0x40);
	sc223a_write_register(ViPipe, 0x3e10, 0x00);
	sc223a_write_register(ViPipe, 0x3e11, 0x80);
	sc223a_write_register(ViPipe, 0x3e12, 0x03);
	sc223a_write_register(ViPipe, 0x3e13, 0x40);
	sc223a_write_register(ViPipe, 0x3e16, 0x00);
	sc223a_write_register(ViPipe, 0x3e17, 0x80);
	sc223a_write_register(ViPipe, 0x3e23, 0x00);
	sc223a_write_register(ViPipe, 0x3e24, 0x88);
	sc223a_write_register(ViPipe, 0x3f09, 0x48);
	sc223a_write_register(ViPipe, 0x4816, 0xb1);
	sc223a_write_register(ViPipe, 0x4819, 0x09);
	sc223a_write_register(ViPipe, 0x481b, 0x05);
	sc223a_write_register(ViPipe, 0x481d, 0x14);
	sc223a_write_register(ViPipe, 0x481f, 0x04);
	sc223a_write_register(ViPipe, 0x4821, 0x0a);
	sc223a_write_register(ViPipe, 0x4823, 0x05);
	sc223a_write_register(ViPipe, 0x4825, 0x04);
	sc223a_write_register(ViPipe, 0x4827, 0x05);
	sc223a_write_register(ViPipe, 0x4829, 0x08);
	sc223a_write_register(ViPipe, 0x5787, 0x10);
	sc223a_write_register(ViPipe, 0x5788, 0x06);
	sc223a_write_register(ViPipe, 0x578a, 0x10);
	sc223a_write_register(ViPipe, 0x578b, 0x06);
	sc223a_write_register(ViPipe, 0x5790, 0x10);
	sc223a_write_register(ViPipe, 0x5791, 0x10);
	sc223a_write_register(ViPipe, 0x5792, 0x00);
	sc223a_write_register(ViPipe, 0x5793, 0x10);
	sc223a_write_register(ViPipe, 0x5794, 0x10);
	sc223a_write_register(ViPipe, 0x5795, 0x00);
	sc223a_write_register(ViPipe, 0x5799, 0x00);
	sc223a_write_register(ViPipe, 0x57c7, 0x10);
	sc223a_write_register(ViPipe, 0x57c8, 0x06);
	sc223a_write_register(ViPipe, 0x57ca, 0x10);
	sc223a_write_register(ViPipe, 0x57cb, 0x06);
	sc223a_write_register(ViPipe, 0x57d1, 0x10);
	sc223a_write_register(ViPipe, 0x57d4, 0x10);
	sc223a_write_register(ViPipe, 0x57d9, 0x00);
	sc223a_write_register(ViPipe, 0x59e0, 0x60);
	sc223a_write_register(ViPipe, 0x59e1, 0x08);
	sc223a_write_register(ViPipe, 0x59e2, 0x3f);
	sc223a_write_register(ViPipe, 0x59e3, 0x18);
	sc223a_write_register(ViPipe, 0x59e4, 0x18);
	sc223a_write_register(ViPipe, 0x59e5, 0x3f);
	sc223a_write_register(ViPipe, 0x59e6, 0x06);
	sc223a_write_register(ViPipe, 0x59e7, 0x02);
	sc223a_write_register(ViPipe, 0x59e8, 0x38);
	sc223a_write_register(ViPipe, 0x59e9, 0x10);
	sc223a_write_register(ViPipe, 0x59ea, 0x0c);
	sc223a_write_register(ViPipe, 0x59eb, 0x10);
	sc223a_write_register(ViPipe, 0x59ec, 0x04);
	sc223a_write_register(ViPipe, 0x59ed, 0x02);
	sc223a_write_register(ViPipe, 0x59ee, 0xa0);
	sc223a_write_register(ViPipe, 0x59ef, 0x08);
	sc223a_write_register(ViPipe, 0x59f4, 0x18);
	sc223a_write_register(ViPipe, 0x59f5, 0x10);
	sc223a_write_register(ViPipe, 0x59f6, 0x0c);
	sc223a_write_register(ViPipe, 0x59f7, 0x10);
	sc223a_write_register(ViPipe, 0x59f8, 0x06);
	sc223a_write_register(ViPipe, 0x59f9, 0x02);
	sc223a_write_register(ViPipe, 0x59fa, 0x18);
	sc223a_write_register(ViPipe, 0x59fb, 0x10);
	sc223a_write_register(ViPipe, 0x59fc, 0x0c);
	sc223a_write_register(ViPipe, 0x59fd, 0x10);
	sc223a_write_register(ViPipe, 0x59fe, 0x04);
	sc223a_write_register(ViPipe, 0x59ff, 0x02);
	sc223a_write_register(ViPipe, 0x36e9, 0x20);
	sc223a_write_register(ViPipe, 0x36f9, 0x24);
	sc223a_write_register(ViPipe, 0x0100, 0x01);

	printf("=== Smart SC223A_1080P_30FPS_10BIT_WDR_Init_OK!===\n");
	return;
}
