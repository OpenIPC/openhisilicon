/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "sc500ai_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else

#ifdef __LITEOS__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif

static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };
#define I2C_DEV_FILE_NUM 16
#define I2C_BUF_NUM 8
#define SC500AI_1620P_30FPS_LINEAR_MODE 1

GK_S32 sc500ai_get_quick_start_flag(VI_PIPE ViPipe, GK_U32 *quick_start_flag)
{
	GK_S32 s32Ret;
	ISP_MOD_PARAM_S stModParam;
	s32Ret = GK_API_ISP_GetModParam(&stModParam);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "sc500ai Get QuickStart Error\n");
		return s32Ret;
	}

	*quick_start_flag = stModParam.u32QuickStart;

	return GK_SUCCESS;
}

int sc500ai_i2c_init(VI_PIPE ViPipe)
{
	char acDevFile[I2C_DEV_FILE_NUM] = { 0 };
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
	ISP_SNS_COMMBUS_U *sc500aibusinfo = GK_NULL;
	sc500aibusinfo = sc500ai_get_bus_Info(ViPipe);
	u8DevNum = sc500aibusinfo[ViPipe].s8I2cDev;
	ret = snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1,
			 "/dev/i2c-%u", u8DevNum);
	if (ret < 0) {
		return GK_FAILURE;
	}
	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (SC500AI_I2C_ADDR >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int sc500ai_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}

	return GK_FAILURE;
}
#define SC500AI_I2C_MSG_NUM 2

static GK_S32 sc500ai_read_register_arecvbuf(VI_PIPE ViPipe, GK_U32 addr,
					     GK_U8 *aRecvbuf)
{
	GK_S32 s32Ret;
	GK_U32 u32RegWidth = SC500AI_ADDR_BYTE;
	GK_U32 u32DataWidth = SC500AI_DATA_BYTE;

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (SC500AI_I2C_ADDR >> 1);
	struct i2c_rdwr_ioctl_data stRdwr;
	struct i2c_msg astMsg[SC500AI_I2C_MSG_NUM];
	memset_s(&stRdwr, sizeof(stRdwr), 0x0, sizeof(stRdwr));
	memset_s(astMsg, sizeof(astMsg), 0x0, sizeof(astMsg));
#endif

	memset_s(aRecvbuf, 4 * sizeof(GK_U8), 0x0,
		 4 * sizeof(GK_U8)); /* 4,4 len of aRecvbuf */

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
	stRdwr.nmsgs = 2; /* 2  */
#endif

#ifdef LOSCFG_HOST_TYPE_VENDOR
	if (u32RegWidth == 2) { /* 2  */
		aRecvbuf[0] = (addr >> 8) & 0xff; /* shift 8 */
		aRecvbuf[1] = addr & 0xff;
	} else {
		aRecvbuf[0] = addr & 0xff;
	}
	s32Ret = ioctl(g_fd[ViPipe], I2C_RDWR, &stRdwr);
#else
	if (u32RegWidth == 2) { /* 2  */
		aRecvbuf[0] = addr & 0xff;
		aRecvbuf[1] = (addr >> 8) & 0xff; /* shift 8 */
	} else {
		aRecvbuf[0] = addr & 0xff;
	}
	s32Ret = read(g_fd[ViPipe], aRecvbuf, u32RegWidth + u32DataWidth);
#endif

	return s32Ret;
}

#define SC500AI_RECV_BUF_NUM 4
int sc500ai_read_register(VI_PIPE ViPipe, GK_S32 addr)
{
	GK_S32 s32RegVal;
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "sc500ai_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret;
	GK_U32 u32DataWidth = SC500AI_DATA_BYTE;
	GK_U8 aRecvbuf[SC500AI_RECV_BUF_NUM];

	s32Ret = sc500ai_read_register_arecvbuf(ViPipe, addr, &aRecvbuf[0]);
	if (s32Ret < 0) {
		return GK_FAILURE;
	}

	if (u32DataWidth == 2) { /* 2  */
		s32RegVal = aRecvbuf[0] | (aRecvbuf[1] << 8); /* shift 8  */
	} else {
		s32RegVal = aRecvbuf[0];
	}

	return s32RegVal;
}

int sc500ai_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = SC500AI_I2C_ADDR;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = SC500AI_ADDR_BYTE;
	i2c_data.data = data;
	i2c_data.data_byte_num = SC500AI_DATA_BYTE;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	GK_U32 idx = 0;
	GK_S32 ret;
	GK_U8 buf[I2C_BUF_NUM];

	if (SC500AI_ADDR_BYTE == 2) { /* 2 byte */
		buf[idx] = (addr >> 8) & 0xff; /* shift 8 */
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (SC500AI_DATA_BYTE == 2) { /* 2 byte */
		buf[idx] = (data >> 8) & 0xff; /* shift 8 */
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, SC500AI_ADDR_BYTE + SC500AI_DATA_BYTE);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}
#endif

	return GK_SUCCESS;
}

void sc500ai_standby(VI_PIPE ViPipe)
{
	return;
}

void sc500ai_restart(VI_PIPE ViPipe)
{
	return;
}

void sc500ai_linear_1296p30_init(VI_PIPE ViPipe);

void sc500ai_init(VI_PIPE ViPipe)
{
	GK_S32 ret = GK_SUCCESS;
	GK_U32 quick_start_flag = 0;
	ISP_SNS_STATE_S *pastsc500ai = GK_NULL;
	pastsc500ai = sc500ai_get_ctx(ViPipe);

	ret += sc500ai_get_quick_start_flag(ViPipe, &quick_start_flag);
	ret += sc500ai_i2c_init(ViPipe);
	if (ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "i2c init failed!\n");
		return;
	}

	if (quick_start_flag == 0) {
		sc500ai_linear_1296p30_init(ViPipe);
	}

	pastsc500ai->bInit = GK_TRUE;
	return;
}

void sc500ai_exit(VI_PIPE ViPipe)
{
	GK_S32 ret;

	ret = sc500ai_i2c_exit(ViPipe);
	if (ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "i2c exit failed!\n");
	}
	return;
}

GK_S32 sc500ai_linear_1296p30_init_part1(VI_PIPE ViPipe)
{
	GK_S32 ret = GK_SUCCESS;
	ret += sc500ai_write_register(ViPipe, 0x0103, 0x01);
	ret += sc500ai_write_register(ViPipe, 0x0100, 0x00);
	ret += sc500ai_write_register(ViPipe, 0x36e9, 0x80);
	ret += sc500ai_write_register(ViPipe, 0x36f9, 0x80);
	ret += sc500ai_write_register(ViPipe, 0x59e0, 0x60);
	ret += sc500ai_write_register(ViPipe, 0x59e1, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x59e2, 0x3f);
	ret += sc500ai_write_register(ViPipe, 0x59e3, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x59e4, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x59e5, 0x3f);
	ret += sc500ai_write_register(ViPipe, 0x59e7, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x59e8, 0x38);
	ret += sc500ai_write_register(ViPipe, 0x59e9, 0x20);
	ret += sc500ai_write_register(ViPipe, 0x59ea, 0x0c);
	ret += sc500ai_write_register(ViPipe, 0x59ec, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x59ed, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x59ee, 0xa0);
	ret += sc500ai_write_register(ViPipe, 0x59ef, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x59f4, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x59f5, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x59f6, 0x0c);
	ret += sc500ai_write_register(ViPipe, 0x59f9, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x59fa, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x59fb, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x59fc, 0x0c);
	ret += sc500ai_write_register(ViPipe, 0x59ff, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x3e16, 0x00);
	ret += sc500ai_write_register(ViPipe, 0x3e17, 0x80);
	ret += sc500ai_write_register(ViPipe, 0x5799, 0x00);
	ret += sc500ai_write_register(ViPipe, 0x3e01, 0xcd);
	ret += sc500ai_write_register(ViPipe, 0x3e02, 0xc0);
	ret += sc500ai_write_register(ViPipe, 0x3308, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x3356, 0x23);
	ret += sc500ai_write_register(ViPipe, 0x4500, 0x88);
	ret += sc500ai_write_register(ViPipe, 0x3651, 0x7d);
	ret += sc500ai_write_register(ViPipe, 0x3622, 0x0a);

	return ret;
}

GK_S32 sc500ai_linear_1296p30_init_part2(VI_PIPE ViPipe)
{
	GK_S32 ret = GK_SUCCESS;
	ret += sc500ai_write_register(ViPipe, 0x36ec, 0x1a);
	ret += sc500ai_write_register(ViPipe, 0x36e9, 0x1c);
	ret += sc500ai_write_register(ViPipe, 0x36f9, 0x24);
	ret += sc500ai_write_register(ViPipe, 0x330a, 0x00);
	ret += sc500ai_write_register(ViPipe, 0x330b, 0xd0);
	ret += sc500ai_write_register(ViPipe, 0x3253, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x363a, 0x1f);
	ret += sc500ai_write_register(ViPipe, 0x3908, 0x41);
	ret += sc500ai_write_register(ViPipe, 0x391d, 0x04);
	ret += sc500ai_write_register(ViPipe, 0x330d, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x3302, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x3301, 0x28);
	ret += sc500ai_write_register(ViPipe, 0x3304, 0x70);
	ret += sc500ai_write_register(ViPipe, 0x3303, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x3308, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x3309, 0x80);
	ret += sc500ai_write_register(ViPipe, 0x3310, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x334c, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x330f, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x330e, 0x20);
	ret += sc500ai_write_register(ViPipe, 0x331c, 0x04);
	ret += sc500ai_write_register(ViPipe, 0x3320, 0x09);
	ret += sc500ai_write_register(ViPipe, 0x33ac, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x33ae, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x331e, 0x61);
	ret += sc500ai_write_register(ViPipe, 0x3356, 0x09);
	ret += sc500ai_write_register(ViPipe, 0x33af, 0x19);
	ret += sc500ai_write_register(ViPipe, 0x331f, 0x71);
	ret += sc500ai_write_register(ViPipe, 0x3333, 0x10);
	ret += sc500ai_write_register(ViPipe, 0x336d, 0x40);
	ret += sc500ai_write_register(ViPipe, 0x3306, 0x40);
	ret += sc500ai_write_register(ViPipe, 0x3622, 0x03);
	ret += sc500ai_write_register(ViPipe, 0x3364, 0x17);
	ret += sc500ai_write_register(ViPipe, 0x3390, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x3391, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x3392, 0x38);
	ret += sc500ai_write_register(ViPipe, 0x3301, 0x0a);
	ret += sc500ai_write_register(ViPipe, 0x3393, 0x0a);
	ret += sc500ai_write_register(ViPipe, 0x3394, 0x28);

	return ret;
}

GK_S32 sc500ai_linear_1296p30_init_part3(VI_PIPE ViPipe)
{
	GK_S32 ret = GK_SUCCESS;
	ret += sc500ai_write_register(ViPipe, 0x3395, 0x28);
	ret += sc500ai_write_register(ViPipe, 0x3396, 0x08);
	ret += sc500ai_write_register(ViPipe, 0x3397, 0x18);
	ret += sc500ai_write_register(ViPipe, 0x3398, 0x38);
	ret += sc500ai_write_register(ViPipe, 0x3399, 0x0a);
	ret += sc500ai_write_register(ViPipe, 0x339a, 0x28);
	ret += sc500ai_write_register(ViPipe, 0x339b, 0x28);
	ret += sc500ai_write_register(ViPipe, 0x339c, 0x28);
	ret += sc500ai_write_register(ViPipe, 0x3670, 0x02);
	ret += sc500ai_write_register(ViPipe, 0x367c, 0x58);
	ret += sc500ai_write_register(ViPipe, 0x367d, 0x78);
	ret += sc500ai_write_register(ViPipe, 0x3674, 0x82);
	ret += sc500ai_write_register(ViPipe, 0x3675, 0x64);
	ret += sc500ai_write_register(ViPipe, 0x3676, 0x66);
	ret += sc500ai_write_register(ViPipe, 0x3670, 0x0a);
	ret += sc500ai_write_register(ViPipe, 0x369c, 0x40);
	ret += sc500ai_write_register(ViPipe, 0x369d, 0x48);
	ret += sc500ai_write_register(ViPipe, 0x3690, 0x33);
	ret += sc500ai_write_register(ViPipe, 0x3691, 0x33);
	ret += sc500ai_write_register(ViPipe, 0x3692, 0x44);
	ret += sc500ai_write_register(ViPipe, 0x360f, 0x01);
	ret += sc500ai_write_register(ViPipe, 0x367a, 0x40);
	ret += sc500ai_write_register(ViPipe, 0x367b, 0x48);
	ret += sc500ai_write_register(ViPipe, 0x3671, 0x07);
	ret += sc500ai_write_register(ViPipe, 0x3672, 0x07);
	ret += sc500ai_write_register(ViPipe, 0x3673, 0x17);
	ret += sc500ai_write_register(ViPipe, 0x3253, 0x0a);
	ret += sc500ai_write_register(ViPipe, 0x36e9, 0x1c);
	ret += sc500ai_write_register(ViPipe, 0x36f9, 0x24);
	ret += sc500ai_write_register(ViPipe, 0x3221, 0x66); // mirror & filp
	ret += sc500ai_write_register(ViPipe, 0x0100, 0x01);

	return ret;
}

void sc500ai_linear_1296p30_init(VI_PIPE ViPipe)
{
	GK_S32 ret = GK_SUCCESS;

	ret += sc500ai_linear_1296p30_init_part1(ViPipe);
	ret += sc500ai_linear_1296p30_init_part2(ViPipe);
	ret += sc500ai_linear_1296p30_init_part3(ViPipe);
	if (ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "write sensor register failed!\n");
		return;
	}

	printf("============================================================================\n");
	printf("=========Smart Sc500ai sensor 2880x1620@30fps(MIPI) init success!===========\n");
	printf("============================================================================\n");

	return;
}
