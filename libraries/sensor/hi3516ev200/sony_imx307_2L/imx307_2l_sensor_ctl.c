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
#include "i2c.h"
#endif

const unsigned char imx307_2l_i2c_addr = 0x34; /* I2C Address of IMX307 */
const unsigned int imx307_2l_addr_byte = 2;
const unsigned int imx307_2l_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastImx307_2l[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunImx307_2l_BusInfo[];

int imx307_2l_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunImx307_2l_BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx307_2l_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int imx307_2l_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int imx307_2l_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int imx307_2l_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = imx307_2l_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = imx307_2l_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = imx307_2l_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (imx307_2l_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (imx307_2l_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf,
		    imx307_2l_addr_byte + imx307_2l_data_byte);
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

void imx307_2l_standby(VI_PIPE ViPipe)
{
	imx307_2l_write_register(ViPipe, 0x3000, 0x01); /* STANDBY */
	imx307_2l_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

	return;
}

void imx307_2l_restart(VI_PIPE ViPipe)
{
	imx307_2l_write_register(ViPipe, 0x3000, 0x00); /* standby */
	delay_ms(20);
	imx307_2l_write_register(ViPipe, 0x3002, 0x00); /* master mode start */
	imx307_2l_write_register(ViPipe, 0x304b, 0x0a);

	return;
}

#define IMX307_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE (2)
#define IMX307_SENSOR_1080P_60FPS_LINEAR_MODE (3)
#define IMX307_SENSOR_720P_60FPS_LINEAR_MODE  (4)
#define IMX307_SENSOR_FLEX_CROP_LINEAR_MODE   (5)

extern void imx307_2l_get_crop(VI_PIPE ViPipe, GK_U32 *pu32W, GK_U32 *pu32H);

void imx307_2l_wdr_1080p30_2to1_init(VI_PIPE ViPipe);
void imx307_2l_linear_1080p30_init(VI_PIPE ViPipe);
void imx307_2l_linear_1080p60_init(VI_PIPE ViPipe);
void imx307_2l_linear_720p60_init(VI_PIPE ViPipe);
void imx307_2l_flex_crop_init(VI_PIPE ViPipe);

void imx307_2l_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;

	for (i = 0; i < g_pastImx307_2l[ViPipe]->astRegsInfo[0].u32RegNum;
	     i++) {
		imx307_2l_write_register(ViPipe,
					 g_pastImx307_2l[ViPipe]
						 ->astRegsInfo[0]
						 .astI2cData[i]
						 .u32RegAddr,
					 g_pastImx307_2l[ViPipe]
						 ->astRegsInfo[0]
						 .astI2cData[i]
						 .u32Data);
	}
}

void imx307_2l_init(VI_PIPE ViPipe)
{
	WDR_MODE_E enWDRMode;
	GK_U8 u8ImgMode;

	enWDRMode = g_pastImx307_2l[ViPipe]->enWDRMode;
	u8ImgMode = g_pastImx307_2l[ViPipe]->u8ImgMode;

	imx307_2l_i2c_init(ViPipe);

	/* When sensor first init, config all registers */
	if (WDR_MODE_2To1_LINE == enWDRMode) {
		if (IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE ==
		    u8ImgMode) { /* IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE */
			imx307_2l_wdr_1080p30_2to1_init(ViPipe);
		} else {
		}
	} else if (IMX307_SENSOR_1080P_60FPS_LINEAR_MODE == u8ImgMode) {
		imx307_2l_linear_1080p60_init(ViPipe);
	} else if (IMX307_SENSOR_720P_60FPS_LINEAR_MODE == u8ImgMode) {
		imx307_2l_linear_720p60_init(ViPipe);
	} else if (IMX307_SENSOR_FLEX_CROP_LINEAR_MODE == u8ImgMode) {
		imx307_2l_flex_crop_init(ViPipe);
	} else {
		imx307_2l_linear_1080p30_init(ViPipe);
	}
	g_pastImx307_2l[ViPipe]->bInit = GK_TRUE;
	return;
}

void imx307_2l_exit(VI_PIPE ViPipe)
{
	imx307_2l_i2c_exit(ViPipe);

	return;
}

/* 1080P30 and 1080P25 */
void imx307_2l_linear_1080p30_init(VI_PIPE ViPipe)
{
	// Enter Standby
	imx307_2l_write_register(ViPipe, 0x3000, 0x01); // Standby mode
	imx307_2l_write_register(ViPipe, 0x3002, 0x00); // XMSTA
	imx307_2l_write_register(ViPipe, 0x3005, 0x00); // ADBIT
	imx307_2l_write_register(ViPipe, 0x3007,
				 0x00); // VREVERSE & HREVERSE & WINMODE
	imx307_2l_write_register(ViPipe, 0x3009, 0x02); // FRSEL & FDG_SEL
	imx307_2l_write_register(ViPipe, 0x300A, 0x3C); // BLKLEVEL
	imx307_2l_write_register(ViPipe, 0x3011, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3018, 0x65); // VMAX
	imx307_2l_write_register(ViPipe, 0x3019, 0x04); // VMAX
	imx307_2l_write_register(ViPipe, 0x301C, 0x30); // HMAX;
	imx307_2l_write_register(ViPipe, 0x301D, 0x11); // HMAX;
	imx307_2l_write_register(ViPipe, 0x3046, 0x00); // ODBIT & OPORTSEL
	imx307_2l_write_register(ViPipe, 0x304B, 0x0A); // XVSOUTSEL & XHSOUTSEL
	imx307_2l_write_register(ViPipe, 0x305C, 0x18); // INCKSEL1 37.125MHz
	imx307_2l_write_register(ViPipe, 0x305D, 0x03); // INCKSEL2
	imx307_2l_write_register(ViPipe, 0x305E, 0x20);
	imx307_2l_write_register(ViPipe, 0x305F, 0x01);
	imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
	imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

	imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
	imx307_2l_write_register(ViPipe, 0x3128, 0x04);
	imx307_2l_write_register(ViPipe, 0x3129, 0x1D); // ADBIT1
	imx307_2l_write_register(ViPipe, 0x313B, 0x41);
	imx307_2l_write_register(ViPipe, 0x315E,
				 0x1A); // INCKSEL5 37.125MHz INCK5 Setting
	imx307_2l_write_register(ViPipe, 0x3164, 0x1A); // INCKSEL6 37.125MHz
	imx307_2l_write_register(ViPipe, 0x317C, 0x12); // ADBIT2
	imx307_2l_write_register(ViPipe, 0x31EC, 0x37); // ADBIT3

	imx307_2l_write_register(ViPipe, 0x3405, 0x10); // REPETITION
	imx307_2l_write_register(ViPipe, 0x3407, 0x01); // PHYSICAL_LANE_NUM
	imx307_2l_write_register(ViPipe, 0x3414, 0x0A); // OPB_SIZE_V
	imx307_2l_write_register(ViPipe, 0x3418, 0x49); // Y_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3419, 0x04); // Y_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3441, 0x0A); // CSI_DT_FMT
	imx307_2l_write_register(ViPipe, 0x3442, 0x0A); // CSI_DT_FMT
	imx307_2l_write_register(ViPipe, 0x3443, 0x01); // CSI_LANE_MODE
	imx307_2l_write_register(ViPipe, 0x3444, 0x20); // EXTCK_FREQ
	imx307_2l_write_register(ViPipe, 0x3445, 0x25); // EXTCK_FREQ
	imx307_2l_write_register(ViPipe, 0x3446, 0x57); // TCLKPOST
	imx307_2l_write_register(ViPipe, 0x3447, 0x00);
	imx307_2l_write_register(ViPipe, 0x3448, 0x37); // THSZERO
	imx307_2l_write_register(ViPipe, 0x3449, 0x00);
	imx307_2l_write_register(ViPipe, 0x344A, 0x1F); // THSPREPARE
	imx307_2l_write_register(ViPipe, 0x344B, 0x00);
	imx307_2l_write_register(ViPipe, 0x344C, 0x1F); // TCLKTRAIL
	imx307_2l_write_register(ViPipe, 0x344D, 0x00);
	imx307_2l_write_register(ViPipe, 0x344E, 0x1F); // THSTRAIL
	imx307_2l_write_register(ViPipe, 0x344F, 0x00);
	imx307_2l_write_register(ViPipe, 0x3450, 0x77); // TCLKZERO
	imx307_2l_write_register(ViPipe, 0x3451, 0x00);
	imx307_2l_write_register(ViPipe, 0x3452, 0x1F); // TCLKPREPARE
	imx307_2l_write_register(ViPipe, 0x3453, 0x00);
	imx307_2l_write_register(ViPipe, 0x3454, 0x17); // TLPX
	imx307_2l_write_register(ViPipe, 0x3455, 0x00);
	imx307_2l_write_register(ViPipe, 0x3472, 0x9C); // X_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3473, 0x07); // X_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3480, 0x49); // INCKSEL7

	imx307_2l_default_reg_init(ViPipe);

	// Standby Cancel
	imx307_2l_write_register(ViPipe, 0x3000, 0x00); // standby

	printf("==============================================================\n");
	printf("=====Sony imx307_2l sensor 1080P30fps(MIPI) init success!=====\n");
	printf("==============================================================\n");
	return;
}

/* 1080P60 — derived from 30fps init with FRSEL=1 (891 Mbps/lane),
 * HMAX=0x0898, and the MIPI D-PHY HS timings from IMX307LQD-C
 * datasheet page 56 column "60/50 fps 2-Lane". All other registers
 * identical to 30fps linear init.
 */
void imx307_2l_linear_1080p60_init(VI_PIPE ViPipe)
{
	imx307_2l_write_register(ViPipe, 0x3000, 0x01); // Standby
	imx307_2l_write_register(ViPipe, 0x3002, 0x00); // XMSTA
	imx307_2l_write_register(ViPipe, 0x3005, 0x00); // ADBIT 10-bit
	imx307_2l_write_register(ViPipe, 0x3007, 0x00); // VREVERSE/HREVERSE/WINMODE
	imx307_2l_write_register(ViPipe, 0x3009, 0x01); // FRSEL=1 (60fps, 891 Mbps/lane)
	imx307_2l_write_register(ViPipe, 0x300A, 0x3C); // BLKLEVEL
	imx307_2l_write_register(ViPipe, 0x3011, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3018, 0x65); // VMAX[7:0]
	imx307_2l_write_register(ViPipe, 0x3019, 0x04); // VMAX[15:8]
	imx307_2l_write_register(ViPipe, 0x301C, 0x98); // HMAX=0x0898
	imx307_2l_write_register(ViPipe, 0x301D, 0x08);
	imx307_2l_write_register(ViPipe, 0x3046, 0x00); // ODBIT & OPORTSEL
	imx307_2l_write_register(ViPipe, 0x304B, 0x0A); // XVSOUTSEL & XHSOUTSEL
	imx307_2l_write_register(ViPipe, 0x305C, 0x18); // INCKSEL1 37.125MHz
	imx307_2l_write_register(ViPipe, 0x305D, 0x03); // INCKSEL2
	imx307_2l_write_register(ViPipe, 0x305E, 0x20);
	imx307_2l_write_register(ViPipe, 0x305F, 0x01);
	imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
	imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

	imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
	imx307_2l_write_register(ViPipe, 0x3128, 0x04);
	imx307_2l_write_register(ViPipe, 0x3129, 0x1D); // ADBIT1
	imx307_2l_write_register(ViPipe, 0x313B, 0x41);
	imx307_2l_write_register(ViPipe, 0x315E, 0x1A); // INCKSEL5 37.125MHz
	imx307_2l_write_register(ViPipe, 0x3164, 0x1A); // INCKSEL6
	imx307_2l_write_register(ViPipe, 0x317C, 0x12); // ADBIT2
	imx307_2l_write_register(ViPipe, 0x31EC, 0x37); // ADBIT3

	imx307_2l_write_register(ViPipe, 0x3405, 0x00); // REPETITION (60fps)
	imx307_2l_write_register(ViPipe, 0x3407, 0x01); // PHYSICAL_LANE_NUM
	imx307_2l_write_register(ViPipe, 0x3414, 0x0A); // OPB_SIZE_V
	imx307_2l_write_register(ViPipe, 0x3418, 0x49); // Y_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3419, 0x04);
	imx307_2l_write_register(ViPipe, 0x3441, 0x0A); // CSI_DT_FMT
	imx307_2l_write_register(ViPipe, 0x3442, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3443, 0x01); // CSI_LANE_MODE
	imx307_2l_write_register(ViPipe, 0x3444, 0x20); // EXTCK_FREQ
	imx307_2l_write_register(ViPipe, 0x3445, 0x25);
	imx307_2l_write_register(ViPipe, 0x3446, 0x77); // TCLKPOST (60fps PHY)
	imx307_2l_write_register(ViPipe, 0x3447, 0x00);
	imx307_2l_write_register(ViPipe, 0x3448, 0x67); // THSZERO
	imx307_2l_write_register(ViPipe, 0x3449, 0x00);
	imx307_2l_write_register(ViPipe, 0x344A, 0x47); // THSPREPARE
	imx307_2l_write_register(ViPipe, 0x344B, 0x00);
	imx307_2l_write_register(ViPipe, 0x344C, 0x37); // TCLKTRAIL
	imx307_2l_write_register(ViPipe, 0x344D, 0x00);
	imx307_2l_write_register(ViPipe, 0x344E, 0x3F); // THSTRAIL
	imx307_2l_write_register(ViPipe, 0x344F, 0x00);
	imx307_2l_write_register(ViPipe, 0x3450, 0xFF); // TCLKZERO
	imx307_2l_write_register(ViPipe, 0x3451, 0x00);
	imx307_2l_write_register(ViPipe, 0x3452, 0x3F); // TCLKPREPARE
	imx307_2l_write_register(ViPipe, 0x3453, 0x00);
	imx307_2l_write_register(ViPipe, 0x3454, 0x37); // TLPX
	imx307_2l_write_register(ViPipe, 0x3455, 0x00);
	imx307_2l_write_register(ViPipe, 0x3472, 0x9C); // X_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3473, 0x07);
	imx307_2l_write_register(ViPipe, 0x3480, 0x49); // INCKSEL7

	imx307_2l_default_reg_init(ViPipe);

	imx307_2l_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel

	printf("==============================================================\n");
	printf("=====Sony imx307_2l sensor 1080P60fps(MIPI) init success!=====\n");
	printf("==============================================================\n");
	return;
}

/* HD 720p sub-readout, WINMODE=1h, FRSEL=1h, 594 Mbps/lane (10-bit
 * 2-lane CSI-2). Register values from IMX307LQD-C datasheet pp.66-68
 * "All-pixel HD720p" section. VMAX=0x02EE=750, HMAX=0x0CE4=3300,
 * Y_OUT_SIZE=0x02D9=729, X_OUT_SIZE=0x051C=1308. MIPI PHY values are
 * the middle tier between 1080p30 (445.5 Mbps) and 1080p60 (891 Mbps). */
void imx307_2l_linear_720p60_init(VI_PIPE ViPipe)
{
	imx307_2l_write_register(ViPipe, 0x3000, 0x01); // Standby
	imx307_2l_write_register(ViPipe, 0x3002, 0x00); // XMSTA
	imx307_2l_write_register(ViPipe, 0x3005, 0x00); // ADBIT 10-bit
	imx307_2l_write_register(ViPipe, 0x3007, 0x10); // WINMODE=1h (HD720p)
	imx307_2l_write_register(ViPipe, 0x3009, 0x01); // FRSEL=1h (60fps tier)
	imx307_2l_write_register(ViPipe, 0x300A, 0x3C); // BLKLEVEL
	imx307_2l_write_register(ViPipe, 0x3011, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3018, 0xEE); // VMAX[7:0]   (0x02EE=750)
	imx307_2l_write_register(ViPipe, 0x3019, 0x02); // VMAX[15:8]
	imx307_2l_write_register(ViPipe, 0x301C, 0xE4); // HMAX[7:0]   (0x0CE4=3300)
	imx307_2l_write_register(ViPipe, 0x301D, 0x0C); // HMAX[15:8]
	imx307_2l_write_register(ViPipe, 0x3046, 0x00);
	imx307_2l_write_register(ViPipe, 0x304B, 0x0A);
	imx307_2l_write_register(ViPipe, 0x305C, 0x18); // INCKSEL1 37.125MHz
	imx307_2l_write_register(ViPipe, 0x305D, 0x03);
	imx307_2l_write_register(ViPipe, 0x305E, 0x20);
	imx307_2l_write_register(ViPipe, 0x305F, 0x01);
	imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
	imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

	imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
	imx307_2l_write_register(ViPipe, 0x3128, 0x04);
	imx307_2l_write_register(ViPipe, 0x3129, 0x1D);
	imx307_2l_write_register(ViPipe, 0x313B, 0x41);
	imx307_2l_write_register(ViPipe, 0x315E, 0x1A);
	imx307_2l_write_register(ViPipe, 0x3164, 0x1A);
	imx307_2l_write_register(ViPipe, 0x317C, 0x12);
	imx307_2l_write_register(ViPipe, 0x31EC, 0x37);

	imx307_2l_write_register(ViPipe, 0x3405, 0x00); // REPETITION (60fps tier)
	imx307_2l_write_register(ViPipe, 0x3407, 0x01); // PHYSICAL_LANE_NUM
	imx307_2l_write_register(ViPipe, 0x3414, 0x04); // OPB_SIZE_V (720p)
	imx307_2l_write_register(ViPipe, 0x3418, 0xD9); // Y_OUT_SIZE[7:0] 0x02D9=729
	imx307_2l_write_register(ViPipe, 0x3419, 0x02);
	imx307_2l_write_register(ViPipe, 0x3441, 0x0A); // CSI_DT_FMT 10-bit
	imx307_2l_write_register(ViPipe, 0x3442, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3443, 0x01); // CSI_LANE_MODE
	imx307_2l_write_register(ViPipe, 0x3444, 0x20); // EXTCK_FREQ
	imx307_2l_write_register(ViPipe, 0x3445, 0x25);
	/* MIPI D-PHY HS timings for 594 Mbps/lane (720p60 column, p.68). */
	imx307_2l_write_register(ViPipe, 0x3446, 0x67); // TCLKPOST
	imx307_2l_write_register(ViPipe, 0x3447, 0x00);
	imx307_2l_write_register(ViPipe, 0x3448, 0x57); // THSZERO
	imx307_2l_write_register(ViPipe, 0x3449, 0x00);
	imx307_2l_write_register(ViPipe, 0x344A, 0x2F); // THSPREPARE
	imx307_2l_write_register(ViPipe, 0x344B, 0x00);
	imx307_2l_write_register(ViPipe, 0x344C, 0x27); // TCLKTRAIL
	imx307_2l_write_register(ViPipe, 0x344D, 0x00);
	imx307_2l_write_register(ViPipe, 0x344E, 0x2F); // THSTRAIL
	imx307_2l_write_register(ViPipe, 0x344F, 0x00);
	imx307_2l_write_register(ViPipe, 0x3450, 0xBF); // TCLKZERO
	imx307_2l_write_register(ViPipe, 0x3451, 0x00);
	imx307_2l_write_register(ViPipe, 0x3452, 0x2F); // TCLKPREPARE
	imx307_2l_write_register(ViPipe, 0x3453, 0x00);
	imx307_2l_write_register(ViPipe, 0x3454, 0x27); // TLPX
	imx307_2l_write_register(ViPipe, 0x3455, 0x00);
	imx307_2l_write_register(ViPipe, 0x3472, 0x1C); // X_OUT_SIZE[7:0] 0x051C=1308
	imx307_2l_write_register(ViPipe, 0x3473, 0x05);
	imx307_2l_write_register(ViPipe, 0x3480, 0x49); // INCKSEL7

	imx307_2l_default_reg_init(ViPipe);
	imx307_2l_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel

	printf("==============================================================\n");
	printf("=====Sony imx307_2l sensor 720P60fps(MIPI) init success!======\n");
	printf("==============================================================\n");
	return;
}

/* Flex window-crop, WINMODE=4h, FRSEL=1h locked (HMAX=0x0898, 891 Mbps/lane).
 * Crop W×H from g_au32Imx307_2l_CropW/H, set by cmos_set_image_mode from the
 * sensor INI's Isp_W/Isp_H when Isp_SnsMode=4. Datasheet p.63 worked examples:
 * VGA 640×480 sustains 129.8 fps, CIF 352×288 sustains 205.8 fps at this tier.
 * fps is controlled by VMAX from cmos_fps_set; init sets a 60 fps baseline VMAX
 * that AE may scale per requested f32Fps. */
void imx307_2l_flex_crop_init(VI_PIPE ViPipe)
{
	GK_U32 crop_w = 1920, crop_h = 1080;
	imx307_2l_get_crop(ViPipe, &crop_w, &crop_h);

	/* Snap to datasheet multiples-of-4 (WINPH, WINWH) and clamp. */
	crop_w = (crop_w / 4) * 4;
	if (crop_w < 368)
		crop_w = 368;
	if (crop_w > 1920)
		crop_w = 1920;
	crop_h = (crop_h / 2) * 2;
	if (crop_h < 304)
		crop_h = 304;
	if (crop_h > 1080)
		crop_h = 1080;

	/* Centre the crop in the 1944×1097 active area. WINPH mult of 4. */
	GK_U32 winph    = ((1944 - crop_w) / 2) & ~0x3U;
	GK_U32 winpv    = ((1096 - crop_h) / 2) & ~0x1U;
	GK_U32 winwh    = crop_w;
	GK_U32 winwv    = crop_h;
	GK_U32 winwv_ob = 2;

	/* CSI-2 output sizes: Y_OUT_SIZE = WINWV + WINWV_OB; X_OUT_SIZE = WINWH + 8. */
	GK_U32 y_out_size = winwv + winwv_ob;
	GK_U32 x_out_size = winwh + 8;

	/* Baseline VMAX for ~60 fps at HMAX=0x0898 (datasheet p.49 1H=14.8µs).
	 * cmos_fps_set scales further per AE. */
	GK_U32 vmax = 1126;
	if (vmax < y_out_size + 10)
		vmax = y_out_size + 10;

	imx307_2l_write_register(ViPipe, 0x3000, 0x01); // Standby
	imx307_2l_write_register(ViPipe, 0x3002, 0x00); // XMSTA
	imx307_2l_write_register(ViPipe, 0x3005, 0x00); // ADBIT 10-bit
	imx307_2l_write_register(ViPipe, 0x3007, 0x40); // WINMODE=4h
	imx307_2l_write_register(ViPipe, 0x3009, 0x01); // FRSEL=1h (60fps tier)
	imx307_2l_write_register(ViPipe, 0x300A, 0x3C);
	imx307_2l_write_register(ViPipe, 0x3011, 0x0A);

	imx307_2l_write_register(ViPipe, 0x3018, vmax & 0xFF);
	imx307_2l_write_register(ViPipe, 0x3019, (vmax >> 8) & 0xFF);
	imx307_2l_write_register(ViPipe, 0x301A, (vmax >> 16) & 0x03);
	imx307_2l_write_register(ViPipe, 0x301C, 0x98); // HMAX=0x0898
	imx307_2l_write_register(ViPipe, 0x301D, 0x08);

	imx307_2l_write_register(ViPipe, 0x303A, winwv_ob & 0x0F);
	imx307_2l_write_register(ViPipe, 0x303C, winpv & 0xFF);
	imx307_2l_write_register(ViPipe, 0x303D, (winpv >> 8) & 0x07);
	imx307_2l_write_register(ViPipe, 0x303E, winwv & 0xFF);
	imx307_2l_write_register(ViPipe, 0x303F, (winwv >> 8) & 0x07);
	imx307_2l_write_register(ViPipe, 0x3040, winph & 0xFF);
	imx307_2l_write_register(ViPipe, 0x3041, (winph >> 8) & 0x07);
	imx307_2l_write_register(ViPipe, 0x3042, winwh & 0xFF);
	imx307_2l_write_register(ViPipe, 0x3043, (winwh >> 8) & 0x07);

	imx307_2l_write_register(ViPipe, 0x3046, 0x00);
	imx307_2l_write_register(ViPipe, 0x304B, 0x0A);
	imx307_2l_write_register(ViPipe, 0x305C, 0x18);
	imx307_2l_write_register(ViPipe, 0x305D, 0x03);
	imx307_2l_write_register(ViPipe, 0x305E, 0x20);
	imx307_2l_write_register(ViPipe, 0x305F, 0x01);
	imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
	imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

	imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
	imx307_2l_write_register(ViPipe, 0x3128, 0x04);
	imx307_2l_write_register(ViPipe, 0x3129, 0x1D);
	imx307_2l_write_register(ViPipe, 0x313B, 0x41);
	imx307_2l_write_register(ViPipe, 0x315E, 0x1A);
	imx307_2l_write_register(ViPipe, 0x3164, 0x1A);
	imx307_2l_write_register(ViPipe, 0x317C, 0x12);
	imx307_2l_write_register(ViPipe, 0x31EC, 0x37);

	imx307_2l_write_register(ViPipe, 0x3405, 0x00); // REPETITION (60fps tier)
	imx307_2l_write_register(ViPipe, 0x3407, 0x01);
	imx307_2l_write_register(ViPipe, 0x3414, 0x0A); // OPB_SIZE_V (default)
	imx307_2l_write_register(ViPipe, 0x3418, y_out_size & 0xFF);
	imx307_2l_write_register(ViPipe, 0x3419, (y_out_size >> 8) & 0x1F);
	imx307_2l_write_register(ViPipe, 0x3441, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3442, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3443, 0x01);
	imx307_2l_write_register(ViPipe, 0x3444, 0x20);
	imx307_2l_write_register(ViPipe, 0x3445, 0x25);
	/* MIPI D-PHY HS timings for 891 Mbps/lane (60fps tier, p.62). */
	imx307_2l_write_register(ViPipe, 0x3446, 0x77);
	imx307_2l_write_register(ViPipe, 0x3447, 0x00);
	imx307_2l_write_register(ViPipe, 0x3448, 0x67);
	imx307_2l_write_register(ViPipe, 0x3449, 0x00);
	imx307_2l_write_register(ViPipe, 0x344A, 0x47);
	imx307_2l_write_register(ViPipe, 0x344B, 0x00);
	imx307_2l_write_register(ViPipe, 0x344C, 0x37);
	imx307_2l_write_register(ViPipe, 0x344D, 0x00);
	imx307_2l_write_register(ViPipe, 0x344E, 0x3F);
	imx307_2l_write_register(ViPipe, 0x344F, 0x00);
	imx307_2l_write_register(ViPipe, 0x3450, 0xFF);
	imx307_2l_write_register(ViPipe, 0x3451, 0x00);
	imx307_2l_write_register(ViPipe, 0x3452, 0x3F);
	imx307_2l_write_register(ViPipe, 0x3453, 0x00);
	imx307_2l_write_register(ViPipe, 0x3454, 0x37);
	imx307_2l_write_register(ViPipe, 0x3455, 0x00);
	imx307_2l_write_register(ViPipe, 0x3472, x_out_size & 0xFF);
	imx307_2l_write_register(ViPipe, 0x3473, (x_out_size >> 8) & 0x1F);
	imx307_2l_write_register(ViPipe, 0x3480, 0x49);

	imx307_2l_default_reg_init(ViPipe);
	imx307_2l_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel

	printf("=== Sony imx307_2l FLEX_CROP init: %ux%u (WINPH=%u WINWH=%u "
	       "WINPV=%u WINWV=%u VMAX=%u HMAX=0x0898) ===\n",
	       crop_w, crop_h, winph, winwh, winpv, winwv, vmax);
	return;
}

void imx307_2l_wdr_1080p30_2to1_init(VI_PIPE ViPipe)
{
	// 10bit
	imx307_2l_write_register(ViPipe, 0x3000, 0x01); //  standby

	imx307_2l_write_register(ViPipe, 0x3002, 0x00); //  XMSTA;
	imx307_2l_write_register(ViPipe, 0x3005, 0x00); //  ADBIT 0x00->10Bit;
	imx307_2l_write_register(ViPipe, 0x3007,
				 0x00); // VREVERSE & HREVERSE & WINMODE
	imx307_2l_write_register(ViPipe, 0x3009, 0x01); // FRSEL & FDG SEL
	imx307_2l_write_register(ViPipe, 0x300A, 0x3C); // BLKLEVEL
	imx307_2l_write_register(ViPipe, 0x300C, 0x11);
	imx307_2l_write_register(ViPipe, 0x3011, 0x0A); //  Change after reset;
	imx307_2l_write_register(ViPipe, 0x3018, 0x65); //  VMAX[7:0]
	imx307_2l_write_register(ViPipe, 0x3019, 0x04); //  VMAX[15:8]
	imx307_2l_write_register(ViPipe, 0x301C,
				 0x98); //  HMAX[7:0]      0x14a0->25fps;
	imx307_2l_write_register(ViPipe, 0x301D,
				 0x08); //  HMAX[15:8]     0x1130->30fps;
	imx307_2l_write_register(ViPipe, 0x3020, 0x02); //  SHS1
	imx307_2l_write_register(ViPipe, 0x3021, 0x00); //  SHS1
	imx307_2l_write_register(ViPipe, 0x3024, 0x49); //  SHS2
	imx307_2l_write_register(ViPipe, 0x3025, 0x08); //  SHS2
	imx307_2l_write_register(ViPipe, 0x3030, 0x0B); //  RHS1
	imx307_2l_write_register(ViPipe, 0x3031, 0x00); //  RHS1
	imx307_2l_write_register(ViPipe, 0x3045,
				 0x05); //  DOLSCDEN & DOLSYDINFOEN & HINFOEN
	imx307_2l_write_register(ViPipe, 0x3046, 0x00); //  OPORTSEL & ODBIT
	imx307_2l_write_register(ViPipe, 0x304B,
				 0x0A); //  XVSOUTSEL & XHSOUTSEL
	imx307_2l_write_register(
		ViPipe, 0x305C,
		0x18); //  INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C
	imx307_2l_write_register(
		ViPipe, 0x305D,
		0x03); //  INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03
	imx307_2l_write_register(
		ViPipe, 0x305E,
		0x20); //  INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10
	imx307_2l_write_register(
		ViPipe, 0x305F,
		0x01); //  INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01
	imx307_2l_write_register(ViPipe, 0x309E, 0x4A);
	imx307_2l_write_register(ViPipe, 0x309F, 0x4A);

	imx307_2l_write_register(ViPipe, 0x3106, 0x11);
	imx307_2l_write_register(ViPipe, 0x311C, 0x0E);
	imx307_2l_write_register(ViPipe, 0x3128, 0x04);
	imx307_2l_write_register(ViPipe, 0x3129,
				 0x1D); //  ADBIT1,12Bit; 0x1D->10Bit;
	imx307_2l_write_register(ViPipe, 0x313B, 0x41);
	imx307_2l_write_register(
		ViPipe, 0x315E,
		0x1A); //  INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
	imx307_2l_write_register(
		ViPipe, 0x3164,
		0x1A); //  INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
	imx307_2l_write_register(ViPipe, 0x317C,
				 0x12); //  ADBIT2,12Bit;0x12->10Bit;
	imx307_2l_write_register(ViPipe, 0x31EC,
				 0x37); //  ADBIT3,12Bit;0x37->10Bit;

	// MIPI setting
	imx307_2l_write_register(ViPipe, 0x3405, 0x00); //  REPETITION
	imx307_2l_write_register(ViPipe, 0x3407,
				 0x01); //  VREVERSE & HREVERSE & WINMODE
	imx307_2l_write_register(ViPipe, 0x3414, 0x0A); //  OPB_SIZE_V
	imx307_2l_write_register(ViPipe, 0x3415, 0x00);
	imx307_2l_write_register(ViPipe, 0x3418, 0x9C); //  Y_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3419, 0x08); //  Y_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3441, 0x0A); //  CSI_DT_FMT 10Bit
	imx307_2l_write_register(ViPipe, 0x3442, 0x0A);
	imx307_2l_write_register(ViPipe, 0x3443, 0x01); //  CSI_LANE_MODE
	imx307_2l_write_register(ViPipe, 0x3444, 0x20); //  EXTCK_FREQ
	imx307_2l_write_register(ViPipe, 0x3445, 0x25); //  EXTCK_FREQ
	imx307_2l_write_register(ViPipe, 0x3446, 0x77); //  TCLKPOST
	imx307_2l_write_register(ViPipe, 0x3447, 0x00);
	imx307_2l_write_register(ViPipe, 0x3448, 0x67); //  THSZERO
	imx307_2l_write_register(ViPipe, 0x3449, 0x00);
	imx307_2l_write_register(ViPipe, 0x344A, 0x47); //  THSPREPARE
	imx307_2l_write_register(ViPipe, 0x344B, 0x00);
	imx307_2l_write_register(ViPipe, 0x344C, 0x37); //  TCLKTRAIL
	imx307_2l_write_register(ViPipe, 0x344D, 0x00);
	imx307_2l_write_register(ViPipe, 0x344E, 0x3F); //  THSTRAIL
	imx307_2l_write_register(ViPipe, 0x344F, 0x00);
	imx307_2l_write_register(ViPipe, 0x3450, 0xFF); //  TCLKZERO
	imx307_2l_write_register(ViPipe, 0x3451, 0x00);
	imx307_2l_write_register(ViPipe, 0x3452, 0x3F); //  TCLKPREPARE
	imx307_2l_write_register(ViPipe, 0x3453, 0x00);
	imx307_2l_write_register(ViPipe, 0x3454, 0x37); //  TLPX
	imx307_2l_write_register(ViPipe, 0x3455, 0x00);
	imx307_2l_write_register(ViPipe, 0x3472, 0xA0); //  X_OUT_SIZE
	imx307_2l_write_register(ViPipe, 0x3473, 0x07);
	imx307_2l_write_register(ViPipe, 0x347B, 0x23);
	imx307_2l_write_register(
		ViPipe, 0x3480,
		0x49); //  INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92

	imx307_2l_default_reg_init(ViPipe);

	imx307_2l_write_register(ViPipe, 0x3000, 0x00); //  Standby Cancel

	printf("============================================================================\n");
	printf("===Imx307_2l sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!===\n");
	printf("============================================================================\n");

	return;
}
