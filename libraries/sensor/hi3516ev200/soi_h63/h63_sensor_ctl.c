/*
 * Copyright (c) OpenIPC.org
 */
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

const unsigned char h63_i2c_addr = 0x80; /* Sensor I2C Address */
const unsigned int h63_addr_byte = 1;
const unsigned int h63_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastH63[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunH63BusInfo[];

int h63_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunH63BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (h63_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int h63_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}

	return GK_FAILURE;
}

int h63_read_register(VI_PIPE ViPipe, int addr)
{
	GK_S32 s32RegVal = 0;
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "h63 fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret = 0;
	GK_U32 u32RegWidth = h63_addr_byte;
	GK_U32 u32DataWidth = h63_data_byte;
	GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (h63_i2c_addr >> 1);
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

int h63_write_register(VI_PIPE ViPipe, int addr, int data)
{
	if (0 > g_fd[ViPipe]) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = h63_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = h63_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = h63_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8] = { 0 };

	if (h63_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (h63_data_byte == 2) {
		buf[idx] = (data >> 8) & 0xff;
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, h63_addr_byte + h63_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}
#endif

	return GK_SUCCESS;
}

void h63_standby(VI_PIPE ViPipe)
{
	return;
}

void h63_restart(VI_PIPE ViPipe)
{
	return;
}

#define H63_720P_30FPS_LINEAR_MODE (1)

void h63_linear_720p30_init(VI_PIPE ViPipe);

void h63_init(VI_PIPE ViPipe)
{
	h63_i2c_init(ViPipe);

	h63_linear_720p30_init(ViPipe);

	g_pastH63[ViPipe]->bInit = GK_TRUE;

	return;
}

void h63_exit(VI_PIPE ViPipe)
{
	h63_i2c_exit(ViPipe);

	return;
}

void h63_linear_720p30_init(VI_PIPE ViPipe)
{
	h63_write_register(ViPipe, 18, 68);
	h63_write_register(ViPipe, 72, 133);
	h63_write_register(ViPipe, 72, 5);
	h63_write_register(ViPipe, 14, 17);
	h63_write_register(ViPipe, 15, 20);
	h63_write_register(ViPipe, 16, 32);
	h63_write_register(ViPipe, 17, 128);
	h63_write_register(ViPipe, 13, 208);
	h63_write_register(ViPipe, 95, 66);
	h63_write_register(ViPipe, 96, 43);
	h63_write_register(ViPipe, 88, 24);
	h63_write_register(ViPipe, 87, 96);
	h63_write_register(ViPipe, 32, 192);
	h63_write_register(ViPipe, 33, 3);
	h63_write_register(ViPipe, 34, 238);
	h63_write_register(ViPipe, 35, 2);
	h63_write_register(ViPipe, 36, 128);
	h63_write_register(ViPipe, 37, 208);
	h63_write_register(ViPipe, 38, 34);
	h63_write_register(ViPipe, 39, 241);
	h63_write_register(ViPipe, 40, 21);
	h63_write_register(ViPipe, 41, 2);
	h63_write_register(ViPipe, 42, 230);
	h63_write_register(ViPipe, 43, 18);
	h63_write_register(ViPipe, 44, 0);
	h63_write_register(ViPipe, 45, 0);
	h63_write_register(ViPipe, 46, 186);
	h63_write_register(ViPipe, 47, 64);
	h63_write_register(ViPipe, 65, 132);
	h63_write_register(ViPipe, 66, 50);
	h63_write_register(ViPipe, 70, 0);
	h63_write_register(ViPipe, 71, 66);
	h63_write_register(ViPipe, 118, 64);
	h63_write_register(ViPipe, 119, 6);
	h63_write_register(ViPipe, 128, 1);
	h63_write_register(ViPipe, 175, 34);
	h63_write_register(ViPipe, 29, 0);
	h63_write_register(ViPipe, 30, 4);
	h63_write_register(ViPipe, 108, 80);
	h63_write_register(ViPipe, 110, 44);
	h63_write_register(ViPipe, 112, 144);
	h63_write_register(ViPipe, 113, 141);
	h63_write_register(ViPipe, 114, 170);
	h63_write_register(ViPipe, 115, 86);
	h63_write_register(ViPipe, 116, 2);
	h63_write_register(ViPipe, 120, 144);
	h63_write_register(ViPipe, 137, 1);
	h63_write_register(ViPipe, 107, 32);
	h63_write_register(ViPipe, 134, 64);
	h63_write_register(ViPipe, 48, 134);
	h63_write_register(ViPipe, 49, 4);
	h63_write_register(ViPipe, 50, 25);
	h63_write_register(ViPipe, 51, 16);
	h63_write_register(ViPipe, 52, 42);
	h63_write_register(ViPipe, 53, 42);
	h63_write_register(ViPipe, 58, 160);
	h63_write_register(ViPipe, 59, 0);
	h63_write_register(ViPipe, 60, 56);
	h63_write_register(ViPipe, 61, 65);
	h63_write_register(ViPipe, 62, 224);
	h63_write_register(ViPipe, 86, 18);
	h63_write_register(ViPipe, 89, 70);
	h63_write_register(ViPipe, 90, 2);
	h63_write_register(ViPipe, 133, 30);
	h63_write_register(ViPipe, 138, 4);
	h63_write_register(ViPipe, 156, 97);
	h63_write_register(ViPipe, 91, 172);
	h63_write_register(ViPipe, 92, 97);
	h63_write_register(ViPipe, 93, 166);
	h63_write_register(ViPipe, 94, 20);
	h63_write_register(ViPipe, 100, 224);
	h63_write_register(ViPipe, 102, 4);
	h63_write_register(ViPipe, 103, 83);
	h63_write_register(ViPipe, 104, 0);
	h63_write_register(ViPipe, 105, 116);
	h63_write_register(ViPipe, 122, 96);
	h63_write_register(ViPipe, 143, 145);
	h63_write_register(ViPipe, 174, 48);
	h63_write_register(ViPipe, 19, 129);
	h63_write_register(ViPipe, 150, 132);
	h63_write_register(ViPipe, 74, 5);
	h63_write_register(ViPipe, 126, 205);
	h63_write_register(ViPipe, 80, 2);
	h63_write_register(ViPipe, 73, 16);
	h63_write_register(ViPipe, 123, 74);
	h63_write_register(ViPipe, 124, 12);
	h63_write_register(ViPipe, 127, 86);
	h63_write_register(ViPipe, 98, 33);
	h63_write_register(ViPipe, 144, 0);
	h63_write_register(ViPipe, 140, 255);
	h63_write_register(ViPipe, 141, 199);
	h63_write_register(ViPipe, 142, 0);
	h63_write_register(ViPipe, 139, 1);
	h63_write_register(ViPipe, 12, 0);
	h63_write_register(ViPipe, 187, 17);
	h63_write_register(ViPipe, 160, 16);
	h63_write_register(ViPipe, 106, 23);
	h63_write_register(ViPipe, 101, 52);
	h63_write_register(ViPipe, 130, 0);
	h63_write_register(ViPipe, 25, 32);
	h63_write_register(ViPipe, 27, 79);
	h63_write_register(ViPipe, 18, 4);
	h63_write_register(ViPipe, 72, 133);
	h63_write_register(ViPipe, 72, 5);
	usleep(10000);

	printf("================================================================\n");
	printf("==== SOI H63 27MInput 1lane 720P30fps(MIPI) init success!======\n");
	printf("================================================================\n");
	return;
}
