/*
 * (c) OpenIPC.org (c)
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

const unsigned char mis2008_i2c_addr = 0x60; /* I2C Address of MIS2008 */
const unsigned int mis2008_addr_byte = 2;
const unsigned int mis2008_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastMis2008[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunMis2008BusInfo[];

int mis2008_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunMis2008BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);
	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (mis2008_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int mis2008_i2c_exit(VI_PIPE ViPipe)
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

int mis2008_read_register(VI_PIPE ViPipe, GK_U32 addr)
{
	GK_S32 s32RegVal = 0;

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "mis2008_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	GK_S32 s32Ret = 0;
	GK_U32 u32RegWidth = mis2008_addr_byte;
	GK_U32 u32DataWidth = mis2008_data_byte;
	GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
	GK_U32 u32SnsI2cAddr = (mis2008_i2c_addr >> 1);
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

int mis2008_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = mis2008_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = mis2008_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = mis2008_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (mis2008_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (mis2008_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, mis2008_addr_byte + mis2008_data_byte);
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

#endif
	return GK_SUCCESS;
}

void mis2008_standby(VI_PIPE ViPipe)
{
	mis2008_write_register(ViPipe, 0x3006, 0x02);
	return;
}

void mis2008_restart(VI_PIPE ViPipe)
{
	mis2008_write_register(ViPipe, 0x3006, 0x01);
	return;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

#define MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define MIS2008_SENSOR_1080P_30FPS_2t1_WDR_MODE (2)

void mis2008_linear_1080p30_init(VI_PIPE ViPipe);
void mis2008_linear_1080p30_10b_init(VI_PIPE ViPipe);
void mis2008_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	for (i = 0; i < g_pastMis2008[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		mis2008_write_register(ViPipe, g_pastMis2008[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr,
				       g_pastMis2008[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
	}
}
void mis2008_init(VI_PIPE ViPipe)
{
	GK_U8 u8ImgMode;

	u8ImgMode = g_pastMis2008[ViPipe]->u8ImgMode;
	mis2008_i2c_init(ViPipe);
	switch (u8ImgMode) {
	case MIS2008_SENSOR_1080P_30FPS_2t1_WDR_MODE:
		break;
	case MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE:
		// mis2008_linear_1080p30_init(ViPipe);
		mis2008_linear_1080p30_10b_init(ViPipe);
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n", u8ImgMode);
		break;
	}

	delay_ms(10);
	mis2008_default_reg_init(ViPipe);
	g_pastMis2008[ViPipe]->bInit = GK_TRUE;
	return;
}

void mis2008_exit(VI_PIPE ViPipe)
{
	mis2008_i2c_exit(ViPipe);

	return;
}

void mis2008_linear_1080p30_init(VI_PIPE ViPipe)
{
	/* [ParaList] */
	mis2008_write_register(ViPipe, 0x300a, 0x01);
	mis2008_write_register(ViPipe, 0x3006, 0x02);
	mis2008_write_register(ViPipe, 0x3201, 0x46);
	mis2008_write_register(ViPipe, 0x3200, 0x05);
	mis2008_write_register(ViPipe, 0x3203, 0xc0);
	mis2008_write_register(ViPipe, 0x3202, 0x08);

	mis2008_write_register(ViPipe, 0x3205, 0x05);
	mis2008_write_register(ViPipe, 0x3204, 0x00);
	mis2008_write_register(ViPipe, 0x3207, 0x40);
	mis2008_write_register(ViPipe, 0x3206, 0x04);

	mis2008_write_register(ViPipe, 0x3209, 0x0a);
	mis2008_write_register(ViPipe, 0x3208, 0x00);
	mis2008_write_register(ViPipe, 0x320b, 0x89);
	mis2008_write_register(ViPipe, 0x320a, 0x07);

	//mis2008_write_register(ViPipe, 0x3007 ,0x00);
	mis2008_write_register(ViPipe, 0x3007, 0x00);
	mis2008_write_register(ViPipe, 0x3300, 0x1c); // PLL_FBDIV
	mis2008_write_register(ViPipe, 0x3301, 0x00);
	mis2008_write_register(ViPipe, 0x3302, 0x02);
	mis2008_write_register(ViPipe, 0x3303, 0x05);
	mis2008_write_register(ViPipe, 0x330d, 0x00); // 12 bit
	mis2008_write_register(ViPipe, 0x3011, 0x2c); // 12 bit
	// mis2008_write_register(ViPipe, 0x330d ,0x01); // 10 bit
	// mis2008_write_register(ViPipe, 0x3011 ,0x2b); // 10 bit
	mis2008_write_register(ViPipe, 0x330b, 0x01);
	mis2008_write_register(ViPipe, 0x330f, 0x0f);
	mis2008_write_register(ViPipe, 0x3013, 0x00);
	mis2008_write_register(ViPipe, 0x3637, 0x1e);
	mis2008_write_register(ViPipe, 0x3900, 0x07);
	mis2008_write_register(ViPipe, 0x2107, 0x00);
	mis2008_write_register(ViPipe, 0x330e, 0x00);

	mis2008_write_register(ViPipe, 0x3900, 0x07);
	mis2008_write_register(ViPipe, 0x2108, 0x01);
	mis2008_write_register(ViPipe, 0x3c40, 0x8c);
	mis2008_write_register(ViPipe, 0x3b01, 0x3f);
	mis2008_write_register(ViPipe, 0x3b03, 0x3f);
	mis2008_write_register(ViPipe, 0x3902, 0x01);
	mis2008_write_register(ViPipe, 0x3904, 0x00);
	mis2008_write_register(ViPipe, 0x3903, 0x00);
	mis2008_write_register(ViPipe, 0x3906, 0x1e);
	mis2008_write_register(ViPipe, 0x3905, 0x00);
	mis2008_write_register(ViPipe, 0x3908, 0x71);
	mis2008_write_register(ViPipe, 0x3907, 0x10);
	mis2008_write_register(ViPipe, 0x390a, 0xff);
	mis2008_write_register(ViPipe, 0x3909, 0x1f);
	mis2008_write_register(ViPipe, 0x390c, 0x67);
	mis2008_write_register(ViPipe, 0x390b, 0x03);
	mis2008_write_register(ViPipe, 0x390e, 0x77);
	mis2008_write_register(ViPipe, 0x390d, 0x00);
	mis2008_write_register(ViPipe, 0x3910, 0x71);
	mis2008_write_register(ViPipe, 0x390f, 0x10);
	mis2008_write_register(ViPipe, 0x3912, 0xff);
	mis2008_write_register(ViPipe, 0x3911, 0x1f);
	mis2008_write_register(ViPipe, 0x3919, 0x00);
	mis2008_write_register(ViPipe, 0x3918, 0x00);
	mis2008_write_register(ViPipe, 0x391b, 0x91);
	mis2008_write_register(ViPipe, 0x391a, 0x01);
	mis2008_write_register(ViPipe, 0x3983, 0x5a);
	mis2008_write_register(ViPipe, 0x3982, 0x00);
	mis2008_write_register(ViPipe, 0x3985, 0x0f);
	mis2008_write_register(ViPipe, 0x3984, 0x00);
	mis2008_write_register(ViPipe, 0x391d, 0x00);
	mis2008_write_register(ViPipe, 0x391c, 0x00);
	mis2008_write_register(ViPipe, 0x391f, 0x65);
	mis2008_write_register(ViPipe, 0x391e, 0x10);
	mis2008_write_register(ViPipe, 0x3921, 0xff);
	mis2008_write_register(ViPipe, 0x3920, 0x1f);
	mis2008_write_register(ViPipe, 0x3923, 0xff);
	mis2008_write_register(ViPipe, 0x3922, 0x1f);
	mis2008_write_register(ViPipe, 0x3932, 0x00);
	mis2008_write_register(ViPipe, 0x3931, 0x00);
	mis2008_write_register(ViPipe, 0x3934, 0x65);
	mis2008_write_register(ViPipe, 0x3933, 0x01);
	mis2008_write_register(ViPipe, 0x393f, 0x6c);
	mis2008_write_register(ViPipe, 0x393e, 0x00);
	mis2008_write_register(ViPipe, 0x3941, 0x67);
	mis2008_write_register(ViPipe, 0x3940, 0x00);
	mis2008_write_register(ViPipe, 0x3943, 0x55);
	mis2008_write_register(ViPipe, 0x3942, 0x01);
	mis2008_write_register(ViPipe, 0x3945, 0xc2);
	mis2008_write_register(ViPipe, 0x3944, 0x02);
	mis2008_write_register(ViPipe, 0x3925, 0x95);
	mis2008_write_register(ViPipe, 0x3924, 0x00);
	mis2008_write_register(ViPipe, 0x3927, 0xe1);
	mis2008_write_register(ViPipe, 0x3926, 0x02);
	mis2008_write_register(ViPipe, 0x3947, 0x74);
	mis2008_write_register(ViPipe, 0x3946, 0x01);
	mis2008_write_register(ViPipe, 0x3949, 0xda);
	mis2008_write_register(ViPipe, 0x3948, 0x0e);
	mis2008_write_register(ViPipe, 0x394b, 0x42);
	mis2008_write_register(ViPipe, 0x394a, 0x03);
	mis2008_write_register(ViPipe, 0x394d, 0xf2);
	mis2008_write_register(ViPipe, 0x394c, 0x01);
	mis2008_write_register(ViPipe, 0x3913, 0x01);
	mis2008_write_register(ViPipe, 0x3915, 0x0f);
	mis2008_write_register(ViPipe, 0x3914, 0x00);
	mis2008_write_register(ViPipe, 0x3917, 0x67);
	mis2008_write_register(ViPipe, 0x3916, 0x03);
	mis2008_write_register(ViPipe, 0x392a, 0x1e);
	mis2008_write_register(ViPipe, 0x3929, 0x00);
	mis2008_write_register(ViPipe, 0x392c, 0x0f);
	mis2008_write_register(ViPipe, 0x392b, 0x00);
	mis2008_write_register(ViPipe, 0x392e, 0x0f);
	mis2008_write_register(ViPipe, 0x392d, 0x00);
	mis2008_write_register(ViPipe, 0x3930, 0x6e);
	mis2008_write_register(ViPipe, 0x392f, 0x03);
	mis2008_write_register(ViPipe, 0x397f, 0x00);
	mis2008_write_register(ViPipe, 0x397e, 0x00);
	mis2008_write_register(ViPipe, 0x3981, 0x77);
	mis2008_write_register(ViPipe, 0x3980, 0x00);
	mis2008_write_register(ViPipe, 0x395d, 0x80);
	mis2008_write_register(ViPipe, 0x395c, 0x10);
	mis2008_write_register(ViPipe, 0x3962, 0x9e);
	mis2008_write_register(ViPipe, 0x3961, 0x10);
	mis2008_write_register(ViPipe, 0x3977, 0x22);
	mis2008_write_register(ViPipe, 0x3976, 0x00);
	mis2008_write_register(ViPipe, 0x3978, 0x00);
	mis2008_write_register(ViPipe, 0x3979, 0x04);
	mis2008_write_register(ViPipe, 0x396d, 0xc2);
	mis2008_write_register(ViPipe, 0x396c, 0x02);
	mis2008_write_register(ViPipe, 0x396f, 0xc2);
	mis2008_write_register(ViPipe, 0x396e, 0x02);
	mis2008_write_register(ViPipe, 0x3971, 0xc2);
	mis2008_write_register(ViPipe, 0x3970, 0x02);
	mis2008_write_register(ViPipe, 0x3973, 0xc2);
	mis2008_write_register(ViPipe, 0x3972, 0x02);

	mis2008_write_register(ViPipe, 0x3900, 0x01);
	mis2008_write_register(ViPipe, 0x3600, 0x00);
	mis2008_write_register(ViPipe, 0x3707, 0x00);
	mis2008_write_register(ViPipe, 0x3708, 0x80);
	mis2008_write_register(ViPipe, 0x3709, 0x00);
	mis2008_write_register(ViPipe, 0x370a, 0x80);
	mis2008_write_register(ViPipe, 0x370b, 0x00);
	mis2008_write_register(ViPipe, 0x370c, 0x80);
	mis2008_write_register(ViPipe, 0x370d, 0x00);
	mis2008_write_register(ViPipe, 0x370e, 0x80);
	mis2008_write_register(ViPipe, 0x3006, 0x00);

	mis2008_write_register(ViPipe, 0x3012, 0x01);
	mis2008_write_register(ViPipe, 0x3600, 0x13);
	mis2008_write_register(ViPipe, 0x3601, 0x02);
	mis2008_write_register(ViPipe, 0x360e, 0x00);
	mis2008_write_register(ViPipe, 0x360f, 0x00);
	mis2008_write_register(ViPipe, 0x3610, 0x02);
	mis2008_write_register(ViPipe, 0x3707, 0x00);
	mis2008_write_register(ViPipe, 0x3708, 0x40);
	mis2008_write_register(ViPipe, 0x3709, 0x00);
	mis2008_write_register(ViPipe, 0x370a, 0x40);
	mis2008_write_register(ViPipe, 0x370b, 0x00);
	mis2008_write_register(ViPipe, 0x370c, 0x40);
	mis2008_write_register(ViPipe, 0x370d, 0x00);
	mis2008_write_register(ViPipe, 0x370e, 0x40);
	mis2008_write_register(ViPipe, 0x3800, 0x00); // Tsensor
	mis2008_write_register(ViPipe, 0x3a03, 0x03);
	mis2008_write_register(ViPipe, 0x3a02, 0x0b);
	mis2008_write_register(ViPipe, 0x3a08, 0x34);
	mis2008_write_register(ViPipe, 0x3a1b, 0x54);
	mis2008_write_register(ViPipe, 0x3a1e, 0x80);
	mis2008_write_register(ViPipe, 0x3100, 0x04);
	mis2008_write_register(ViPipe, 0x3101, 0x64);
	mis2008_write_register(ViPipe, 0x3a1c, 0x10);
	mis2008_write_register(ViPipe, 0x3a0C, 0x04);
	mis2008_write_register(ViPipe, 0x3a0D, 0x12);
	mis2008_write_register(ViPipe, 0x3a0E, 0x15);
	mis2008_write_register(ViPipe, 0x3a0F, 0x18);
	mis2008_write_register(ViPipe, 0x3a10, 0x20);
	mis2008_write_register(ViPipe, 0x3a11, 0x3c);

	delay_ms(100);

	printf("===MIS2008 1080P 30fps 12bit LINE Init OK!===\n");
}

void mis2008_linear_1080p30_10b_init(VI_PIPE ViPipe)
{
	mis2008_write_register(ViPipe, 0x300a, 0x01);
	mis2008_write_register(ViPipe, 0x3006, 0x02);
	mis2008_write_register(ViPipe, 0x3201, 0x65);
	mis2008_write_register(ViPipe, 0x3200, 0x04);
	mis2008_write_register(ViPipe, 0x3203, 0xc0);
	mis2008_write_register(ViPipe, 0x3202, 0x08);
	mis2008_write_register(ViPipe, 0x3205, 0x00);
	mis2008_write_register(ViPipe, 0x3204, 0x00);
	mis2008_write_register(ViPipe, 0x3207, 0x37);
	mis2008_write_register(ViPipe, 0x3206, 0x04);
	mis2008_write_register(ViPipe, 0x3209, 0x00);
	mis2008_write_register(ViPipe, 0x3208, 0x00);
	mis2008_write_register(ViPipe, 0x320b, 0x7f);
	mis2008_write_register(ViPipe, 0x320a, 0x07);
	mis2008_write_register(ViPipe, 0x3007, 0x00);
	mis2008_write_register(ViPipe, 0x3300, 0x1c);
	mis2008_write_register(ViPipe, 0x3301, 0x00);
	mis2008_write_register(ViPipe, 0x3302, 0x02);
	mis2008_write_register(ViPipe, 0x3303, 0x05);
	mis2008_write_register(ViPipe, 0x330d, 0x01);
	mis2008_write_register(ViPipe, 0x330b, 0x01);
	mis2008_write_register(ViPipe, 0x330f, 0x0f);
	mis2008_write_register(ViPipe, 0x3013, 0x00);
	mis2008_write_register(ViPipe, 0x3011, 0x2b);
	mis2008_write_register(ViPipe, 0x3c20, 0x2b);
	mis2008_write_register(ViPipe, 0x3c21, 0x6b);
	mis2008_write_register(ViPipe, 0x3c22, 0xab);
	mis2008_write_register(ViPipe, 0x3c23, 0xeb);
	mis2008_write_register(ViPipe, 0x3900, 0x07);
	mis2008_write_register(ViPipe, 0x2107, 0x00);
	mis2008_write_register(ViPipe, 0x2108, 0x01);
	mis2008_write_register(ViPipe, 0x3c16, 0x02);
	mis2008_write_register(ViPipe, 0x3c17, 0xdc);
	mis2008_write_register(ViPipe, 0x3c18, 0x6c);
	mis2008_write_register(ViPipe, 0x3c40, 0x8d);
	mis2008_write_register(ViPipe, 0x3b01, 0x3f);
	mis2008_write_register(ViPipe, 0x3b03, 0x3f);
	mis2008_write_register(ViPipe, 0x3902, 0x01);
	mis2008_write_register(ViPipe, 0x3904, 0x00);
	mis2008_write_register(ViPipe, 0x3903, 0x00);
	mis2008_write_register(ViPipe, 0x3906, 0x1e);
	mis2008_write_register(ViPipe, 0x3905, 0x00);
	mis2008_write_register(ViPipe, 0x3908, 0x71);
	mis2008_write_register(ViPipe, 0x3907, 0x10);
	mis2008_write_register(ViPipe, 0x390a, 0xff);
	mis2008_write_register(ViPipe, 0x3909, 0x1f);
	mis2008_write_register(ViPipe, 0x390c, 0x67);
	mis2008_write_register(ViPipe, 0x390b, 0x03);
	mis2008_write_register(ViPipe, 0x390e, 0x40);
	mis2008_write_register(ViPipe, 0x390d, 0x00);
	mis2008_write_register(ViPipe, 0x3910, 0x71);
	mis2008_write_register(ViPipe, 0x390f, 0x10);
	mis2008_write_register(ViPipe, 0x3912, 0xff);
	mis2008_write_register(ViPipe, 0x3911, 0x1f);
	mis2008_write_register(ViPipe, 0x3919, 0x00);
	mis2008_write_register(ViPipe, 0x3918, 0x00);
	mis2008_write_register(ViPipe, 0x391b, 0x91);
	mis2008_write_register(ViPipe, 0x391a, 0x01);
	mis2008_write_register(ViPipe, 0x3983, 0x5a);
	mis2008_write_register(ViPipe, 0x3982, 0x00);
	mis2008_write_register(ViPipe, 0x3985, 0x0f);
	mis2008_write_register(ViPipe, 0x3984, 0x00);
	mis2008_write_register(ViPipe, 0x391d, 0x00);
	mis2008_write_register(ViPipe, 0x391c, 0x00);
	mis2008_write_register(ViPipe, 0x391f, 0x65);
	mis2008_write_register(ViPipe, 0x391e, 0x10);
	mis2008_write_register(ViPipe, 0x3921, 0xff);
	mis2008_write_register(ViPipe, 0x3920, 0x1f);
	mis2008_write_register(ViPipe, 0x3923, 0xff);
	mis2008_write_register(ViPipe, 0x3922, 0x1f);
	mis2008_write_register(ViPipe, 0x3932, 0x00);
	mis2008_write_register(ViPipe, 0x3931, 0x00);
	mis2008_write_register(ViPipe, 0x3934, 0x65);
	mis2008_write_register(ViPipe, 0x3933, 0x01);
	mis2008_write_register(ViPipe, 0x393f, 0x6c);
	mis2008_write_register(ViPipe, 0x393e, 0x00);
	mis2008_write_register(ViPipe, 0x3941, 0x67);
	mis2008_write_register(ViPipe, 0x3940, 0x00);
	mis2008_write_register(ViPipe, 0x3943, 0x50);
	mis2008_write_register(ViPipe, 0x3942, 0x01);
	mis2008_write_register(ViPipe, 0x3945, 0xc2);
	mis2008_write_register(ViPipe, 0x3944, 0x02);
	mis2008_write_register(ViPipe, 0x3925, 0x95);
	mis2008_write_register(ViPipe, 0x3924, 0x00);
	mis2008_write_register(ViPipe, 0x3927, 0xe1);
	mis2008_write_register(ViPipe, 0x3926, 0x02);
	mis2008_write_register(ViPipe, 0x3947, 0x74);
	mis2008_write_register(ViPipe, 0x3946, 0x01);
	mis2008_write_register(ViPipe, 0x3949, 0xda);
	mis2008_write_register(ViPipe, 0x3948, 0x0e);
	mis2008_write_register(ViPipe, 0x394b, 0x42);
	mis2008_write_register(ViPipe, 0x394a, 0x03);
	mis2008_write_register(ViPipe, 0x394d, 0xf2);
	mis2008_write_register(ViPipe, 0x394c, 0x01);
	mis2008_write_register(ViPipe, 0x3913, 0x01);
	mis2008_write_register(ViPipe, 0x3915, 0x0f);
	mis2008_write_register(ViPipe, 0x3914, 0x00);
	mis2008_write_register(ViPipe, 0x3917, 0x67);
	mis2008_write_register(ViPipe, 0x3916, 0x03);
	mis2008_write_register(ViPipe, 0x392a, 0x1e);
	mis2008_write_register(ViPipe, 0x3929, 0x00);
	mis2008_write_register(ViPipe, 0x392c, 0x0f);
	mis2008_write_register(ViPipe, 0x392b, 0x00);
	mis2008_write_register(ViPipe, 0x392e, 0x0f);
	mis2008_write_register(ViPipe, 0x392d, 0x00);
	mis2008_write_register(ViPipe, 0x3930, 0x6e);
	mis2008_write_register(ViPipe, 0x392f, 0x03);
	mis2008_write_register(ViPipe, 0x397f, 0x00);
	mis2008_write_register(ViPipe, 0x397e, 0x00);
	mis2008_write_register(ViPipe, 0x3981, 0x40);
	mis2008_write_register(ViPipe, 0x3980, 0x00);
	mis2008_write_register(ViPipe, 0x395d, 0x80);
	mis2008_write_register(ViPipe, 0x395c, 0x10);
	mis2008_write_register(ViPipe, 0x3962, 0x9e);
	mis2008_write_register(ViPipe, 0x3961, 0x10);
	mis2008_write_register(ViPipe, 0x3967, 0x50);
	mis2008_write_register(ViPipe, 0x3977, 0x22);
	mis2008_write_register(ViPipe, 0x3976, 0x00);
	mis2008_write_register(ViPipe, 0x3978, 0x00);
	mis2008_write_register(ViPipe, 0x3979, 0x04);
	mis2008_write_register(ViPipe, 0x396d, 0xc2);
	mis2008_write_register(ViPipe, 0x396c, 0x02);
	mis2008_write_register(ViPipe, 0x396f, 0xc2);
	mis2008_write_register(ViPipe, 0x396e, 0x02);
	mis2008_write_register(ViPipe, 0x3971, 0xc2);
	mis2008_write_register(ViPipe, 0x3970, 0x02);
	mis2008_write_register(ViPipe, 0x3973, 0xc2);
	mis2008_write_register(ViPipe, 0x3972, 0x02);
	mis2008_write_register(ViPipe, 0x3900, 0x00);
	mis2008_write_register(ViPipe, 0x3012, 0x01);
	mis2008_write_register(ViPipe, 0x3600, 0x13);
	mis2008_write_register(ViPipe, 0x3601, 0x02);
	mis2008_write_register(ViPipe, 0x360e, 0x00);
	mis2008_write_register(ViPipe, 0x360f, 0x00);
	mis2008_write_register(ViPipe, 0x3610, 0x02);
	mis2008_write_register(ViPipe, 0x3637, 0x1e);
	mis2008_write_register(ViPipe, 0x3701, 0xa0);
	mis2008_write_register(ViPipe, 0x3707, 0x00);
	mis2008_write_register(ViPipe, 0x3708, 0x40);
	mis2008_write_register(ViPipe, 0x3709, 0x00);
	mis2008_write_register(ViPipe, 0x370a, 0x40);
	mis2008_write_register(ViPipe, 0x370b, 0x00);
	mis2008_write_register(ViPipe, 0x370c, 0x40);
	mis2008_write_register(ViPipe, 0x370d, 0x00);
	mis2008_write_register(ViPipe, 0x370e, 0x40);
	mis2008_write_register(ViPipe, 0x3800, 0x00);
	mis2008_write_register(ViPipe, 0x3a03, 0x09);
	mis2008_write_register(ViPipe, 0x3a02, 0x0b);
	mis2008_write_register(ViPipe, 0x3a08, 0x34);
	mis2008_write_register(ViPipe, 0x3a1b, 0x50);
	mis2008_write_register(ViPipe, 0x3a16, 0x4d);
	mis2008_write_register(ViPipe, 0x3a1e, 0x01);
	mis2008_write_register(ViPipe, 0x3a1c, 0x1f);
	mis2008_write_register(ViPipe, 0x3a0c, 0x04);
	mis2008_write_register(ViPipe, 0x3a0d, 0x12);
	mis2008_write_register(ViPipe, 0x3a0e, 0x15);
	mis2008_write_register(ViPipe, 0x3a0f, 0x18);
	mis2008_write_register(ViPipe, 0x3a10, 0x20);
	mis2008_write_register(ViPipe, 0x3a11, 0x3c);
	mis2008_write_register(ViPipe, 0x3006, 0x00);
	printf("===MIS2008 1080P 30fps 10bit LINE Init OK!===\n");
}