/*
 * (c) OpenIPC.org (c)
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "comm_video.h"
#include "sns_ctrl.h"

#include <linux/i2c.h>
#include "i2c.h"

const unsigned char mis2009_i2c_addr = 0x60; /* I2C Address of MIS2009 */
const unsigned int mis2009_addr_byte = 2;
const unsigned int mis2009_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastMis2009[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunMis2009BusInfo[];

int mis2009_i2c_init(VI_PIPE ViPipe)
{
	char acDevFile[16] = { 0 };
	GK_U8 u8DevNum;
	int ret;

	if (g_fd[ViPipe] >= 0) {
		return GK_SUCCESS;
	}

	u8DevNum = g_aunMis2009BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);
	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c-%u error!\n", u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (mis2009_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

int mis2009_i2c_exit(VI_PIPE ViPipe)
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

/* The register's value, or GK_FAILURE unless both messages went through. */
int mis2009_read_register(VI_PIPE ViPipe, int addr)
{
	GK_U8 aBuf[2] = { (addr >> 8) & 0xff, addr & 0xff };
	struct i2c_msg astMsg[2];
	struct i2c_rdwr_ioctl_data stRdwr;

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "mis2009_read_register fd not opened!\n");
		return GK_FAILURE;
	}

	memset(astMsg, 0, sizeof(astMsg));
	astMsg[0].addr = mis2009_i2c_addr >> 1;
	astMsg[0].len = mis2009_addr_byte;
	astMsg[0].buf = aBuf;
	astMsg[1].addr = mis2009_i2c_addr >> 1;
	astMsg[1].flags = I2C_M_RD;
	astMsg[1].len = mis2009_data_byte;
	astMsg[1].buf = aBuf;
	stRdwr.msgs = astMsg;
	stRdwr.nmsgs = 2;

	if (ioctl(g_fd[ViPipe], I2C_RDWR, &stRdwr) != 2) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_READ error!\n");
		return GK_FAILURE;
	}

	return aBuf[0];
}

int mis2009_write_register(VI_PIPE ViPipe, int addr, int data)
{
	GK_U8 aBuf[3] = { (addr >> 8) & 0xff, addr & 0xff, data & 0xff };

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "mis2009_write_register fd not opened!\n");
		return GK_FAILURE;
	}

	if (write(g_fd[ViPipe], aBuf, sizeof(aBuf)) != (ssize_t)sizeof(aBuf)) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

void mis2009_standby(VI_PIPE ViPipe)
{
	mis2009_write_register(ViPipe, 0x3006, 0x02);
	return;
}

void mis2009_restart(VI_PIPE ViPipe)
{
	mis2009_write_register(ViPipe, 0x3006, 0x00); /* streaming, as the init table leaves it */
	return;
}

#define MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define MIS2009_SENSOR_1080P_50FPS_LINEAR_MODE (2)

/*
 * 1920x1080 RAW10 linear, 30 fps from a 27 MHz clock, 2-lane MIPI: the
 * vendor's mis2009_linear_2M30_init, in its order and with no delays. It holds
 * the sensor in standby (0x3006) while it programs it.
 */
static const struct {
	GK_U16 u16Addr;
	GK_U8 u8Data;
} g_astMis2009Linear1080p30[] = {
	{ 0x300a, 0x01 },
	{ 0x3006, 0x02 },
	{ 0x3201, 0x65 },
	{ 0x3200, 0x04 },
	{ 0x3203, 0xc0 },
	{ 0x3202, 0x08 },
	{ 0x3205, 0x08 },
	{ 0x3204, 0x00 },
	{ 0x3207, 0x3f },
	{ 0x3206, 0x04 },
	{ 0x3209, 0x07 },
	{ 0x3208, 0x00 },
	{ 0x320b, 0x86 },
	{ 0x320a, 0x07 },
	{ 0x3007, 0x00 },
	{ 0x3007, 0x00 },
	{ 0x3300, 0x1c },
	{ 0x3301, 0x00 },
	{ 0x3302, 0x02 },
	{ 0x3303, 0x05 },
	{ 0x330d, 0x01 },
	{ 0x330b, 0x01 },
	{ 0x330f, 0x0f },
	{ 0x3013, 0x00 },
	{ 0x3011, 0x2b },
	{ 0x3c20, 0x2b },
	{ 0x3c21, 0x6b },
	{ 0x3c22, 0xab },
	{ 0x3c23, 0xeb },
	{ 0x3900, 0x07 },
	{ 0x2107, 0x00 },
	{ 0x2108, 0x01 },
	{ 0x3c16, 0x02 },
	{ 0x3c17, 0xdc },
	{ 0x3c18, 0x6c },
	{ 0x3c40, 0x8d },
	{ 0x3b01, 0x3f },
	{ 0x3b03, 0x3f },
	{ 0x3902, 0x01 },
	{ 0x3904, 0x00 },
	{ 0x3903, 0x00 },
	{ 0x3906, 0x1e },
	{ 0x3905, 0x00 },
	{ 0x3908, 0x71 },
	{ 0x3907, 0x10 },
	{ 0x390a, 0xff },
	{ 0x3909, 0x1f },
	{ 0x390c, 0x67 },
	{ 0x390b, 0x03 },
	{ 0x390e, 0x40 },
	{ 0x390d, 0x00 },
	{ 0x3910, 0x71 },
	{ 0x390f, 0x10 },
	{ 0x3912, 0xff },
	{ 0x3911, 0x1f },
	{ 0x3919, 0x00 },
	{ 0x3918, 0x00 },
	{ 0x391b, 0x91 },
	{ 0x391a, 0x01 },
	{ 0x3983, 0x5a },
	{ 0x3982, 0x00 },
	{ 0x3985, 0x0f },
	{ 0x3984, 0x00 },
	{ 0x391d, 0x00 },
	{ 0x391c, 0x00 },
	{ 0x391f, 0x65 },
	{ 0x391e, 0x10 },
	{ 0x3921, 0xff },
	{ 0x3920, 0x1f },
	{ 0x3923, 0xff },
	{ 0x3922, 0x1f },
	{ 0x3932, 0x00 },
	{ 0x3931, 0x00 },
	{ 0x3934, 0x65 },
	{ 0x3933, 0x01 },
	{ 0x393f, 0x6c },
	{ 0x393e, 0x00 },
	{ 0x3941, 0x67 },
	{ 0x3940, 0x00 },
	{ 0x3943, 0x50 },
	{ 0x3942, 0x01 },
	{ 0x3945, 0xc2 },
	{ 0x3944, 0x02 },
	{ 0x3925, 0x95 },
	{ 0x3924, 0x00 },
	{ 0x3927, 0xe1 },
	{ 0x3926, 0x02 },
	{ 0x3947, 0x74 },
	{ 0x3946, 0x01 },
	{ 0x3949, 0xda },
	{ 0x3948, 0x0e },
	{ 0x394b, 0x42 },
	{ 0x394a, 0x03 },
	{ 0x394d, 0xf2 },
	{ 0x394c, 0x01 },
	{ 0x3913, 0x01 },
	{ 0x3915, 0x0f },
	{ 0x3914, 0x00 },
	{ 0x3917, 0x67 },
	{ 0x3916, 0x03 },
	{ 0x392a, 0x1e },
	{ 0x3929, 0x00 },
	{ 0x392c, 0x0f },
	{ 0x392b, 0x00 },
	{ 0x392e, 0x0f },
	{ 0x392d, 0x00 },
	{ 0x3930, 0x6e },
	{ 0x392f, 0x03 },
	{ 0x397f, 0x00 },
	{ 0x397e, 0x00 },
	{ 0x3981, 0x40 },
	{ 0x3980, 0x00 },
	{ 0x395d, 0x80 },
	{ 0x395c, 0x10 },
	{ 0x3962, 0x9e },
	{ 0x3961, 0x10 },
	{ 0x3967, 0x50 },
	{ 0x3977, 0x22 },
	{ 0x3976, 0x00 },
	{ 0x3978, 0x00 },
	{ 0x3979, 0x04 },
	{ 0x396d, 0xc2 },
	{ 0x396c, 0x02 },
	{ 0x396f, 0xc2 },
	{ 0x396e, 0x02 },
	{ 0x3971, 0xc2 },
	{ 0x3970, 0x02 },
	{ 0x3973, 0xc2 },
	{ 0x3972, 0x02 },
	{ 0x3900, 0x00 },
	{ 0x3012, 0x01 },
	{ 0x3600, 0x13 },
	{ 0x3601, 0x02 },
	{ 0x360e, 0x00 },
	{ 0x360f, 0x00 },
	{ 0x3610, 0x02 },
	{ 0x3637, 0x1e },
	{ 0x3701, 0xa0 },
	{ 0x3707, 0x00 },
	{ 0x3708, 0x40 },
	{ 0x3709, 0x00 },
	{ 0x370a, 0x40 },
	{ 0x370b, 0x00 },
	{ 0x370c, 0x40 },
	{ 0x370d, 0x00 },
	{ 0x370e, 0x40 },
	{ 0x3800, 0x00 },
	{ 0x3a03, 0x09 },
	{ 0x3a02, 0x0b },
	{ 0x3a08, 0x34 },
	{ 0x3a1b, 0x50 },
	{ 0x3a16, 0x4d },
	{ 0x3a1e, 0x01 },
	{ 0x3a1c, 0x1f },
	{ 0x3a0c, 0x04 },
	{ 0x3a0d, 0x12 },
	{ 0x3a0e, 0x15 },
	{ 0x3a0f, 0x18 },
	{ 0x3a10, 0x20 },
	{ 0x3a11, 0x3c },
	{ 0x3006, 0x00 }
};

void mis2009_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	for (i = 0; i < g_pastMis2009[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		mis2009_write_register(ViPipe, g_pastMis2009[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr,
				       g_pastMis2009[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
	}
}

/*
 * 1080p up to 51 fps: the 30 fps table with the PLL raised from 756 to
 * 1296 MHz (FBDIV 28 -> 48). The line stays 2240 PCLK, so the timing
 * generator's 0x39xx table, counted in ACLK cycles, still fits it unchanged;
 * PCLK 129.6 MHz, 57857 lines per second, MIPI 648 Mbps per lane.
 *
 * The datasheet's own 1080p60 row (FBDIV 55, VCO 1485 MHz, line 2154) streams
 * at 61 fps on this part, but bright pixels come out dark with a coloured
 * fringe -- measured: clean up to VCO 1296, a few at 1404, dozens a frame at
 * 1485, at any line length and any ACLK divider. TSDIV/CPDIV are that row's.
 */
static const struct {
	GK_U16 u16Addr;
	GK_U8 u8Slow; /* what the 30 fps table leaves: reset values, read back from a sensor */
	GK_U8 u8Fast;
} g_astMis2009Pll[] = {
	{ 0x3300, 0x1c, 0x30 }, /* FBDIV: VCO 27 MHz x 28 = 756 MHz, x 48 = 1296 MHz */
	{ 0x330f, 0x0f, 0x12 }, /* TSDIV 15, 18 */
	{ 0x3310, 0x02, 0x06 }, /* CPDIV 2, 6 */
	{ 0x3c01, 0x09, 0x05 }, /* MIPI clk_period: 4x the bit clock period in ns, rounded down */
};

static void mis2009_linear_1080p_init(VI_PIPE ViPipe, GK_BOOL bFast)
{
	const GK_U32 n = sizeof(g_astMis2009Linear1080p30) / sizeof(g_astMis2009Linear1080p30[0]);
	GK_U32 i;

	/* the table's last entry takes the sensor out of standby; the PLL goes in
	 * before it, written either way -- the 30 fps table leaves CPDIV and the
	 * MIPI clock period at their reset values, so a sensor coming back from the
	 * faster mode without a reset would otherwise keep the faster ones */
	for (i = 0; i < n - 1; i++) {
		mis2009_write_register(ViPipe, g_astMis2009Linear1080p30[i].u16Addr, g_astMis2009Linear1080p30[i].u8Data);
	}
	for (i = 0; i < sizeof(g_astMis2009Pll) / sizeof(g_astMis2009Pll[0]); i++) {
		mis2009_write_register(ViPipe, g_astMis2009Pll[i].u16Addr,
				       bFast ? g_astMis2009Pll[i].u8Fast : g_astMis2009Pll[i].u8Slow);
	}
	mis2009_write_register(ViPipe, g_astMis2009Linear1080p30[n - 1].u16Addr, g_astMis2009Linear1080p30[n - 1].u8Data);
	printf("===MIS2009 1080P %dfps 10bit LINE Init OK!===\n", bFast ? 50 : 30);
}

void mis2009_init(VI_PIPE ViPipe)
{
	GK_U8 u8ImgMode;

	u8ImgMode = g_pastMis2009[ViPipe]->u8ImgMode;
	if (mis2009_i2c_init(ViPipe) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "MIS2009: no I2C bus, sensor not initialised\n");
		return;
	}
	switch (u8ImgMode) {
	case MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE:
		mis2009_linear_1080p_init(ViPipe, GK_FALSE);
		break;
	case MIS2009_SENSOR_1080P_50FPS_LINEAR_MODE:
		mis2009_linear_1080p_init(ViPipe, GK_TRUE);
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n", u8ImgMode);
		break;
	}

	mis2009_default_reg_init(ViPipe);
	g_pastMis2009[ViPipe]->bInit = GK_TRUE;
	return;
}

void mis2009_exit(VI_PIPE ViPipe)
{
	mis2009_i2c_exit(ViPipe);

	return;
}
