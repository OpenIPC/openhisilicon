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

const unsigned char imx290_i2c_addr = 0x34; /* I2C Address of IMX290 */
const unsigned int imx290_addr_byte = 2;
const unsigned int imx290_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastImx290[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunImx290BusInfo[];

int imx290_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunImx290BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx290_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int imx290_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int imx290_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int imx290_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = imx290_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = imx290_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = imx290_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (imx290_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (imx290_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, imx290_addr_byte + imx290_data_byte);
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

void imx290_standby(VI_PIPE ViPipe)
{
	return;
}

void imx290_restart(VI_PIPE ViPipe)
{
	return;
}

#define IMX290_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define IMX290_SENSOR_1080P_30FPS_3t1_WDR_MODE (2)
#define IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE (3)
#define IMX290_SENSOR_720P_30FPS_2t1_WDR_MODE (4)
#define IMX290_SENSOR_1080P_30FPS_2t1_FRAME_WDR_MODE (5)

void imx290_wdr_1080p30_2to1_init(VI_PIPE ViPipe);
void imx290_wdr_1080p60_2to1_init(VI_PIPE ViPipe);
void imx290_wdr_1080p120_2to1_init(VI_PIPE ViPipe);
void imx290_wdr_720p60_2to1_init(VI_PIPE ViPipe);
void imx290_wdr_1080p30_3to1_init(VI_PIPE ViPipe);
void imx290_wdr_1080p120_3to1_init(VI_PIPE ViPipe);
void imx290_wdr_720p60_3to1_init(VI_PIPE ViPipe);
void imx290_linear_1080p30_init(VI_PIPE ViPipe);

void imx290_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;
	for (i = 0; i < g_pastImx290[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		imx290_write_register(ViPipe,
				      g_pastImx290[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32RegAddr,
				      g_pastImx290[ViPipe]
					      ->astRegsInfo[0]
					      .astI2cData[i]
					      .u32Data);
	}
}

void imx290_init(VI_PIPE ViPipe)
{
	WDR_MODE_E enWDRMode;
	GK_U8 u8ImgMode;

	enWDRMode = g_pastImx290[ViPipe]->enWDRMode;
	u8ImgMode = g_pastImx290[ViPipe]->u8ImgMode;

	imx290_i2c_init(ViPipe);
	/* When sensor first init, config all registers */
	if (WDR_MODE_2To1_LINE == enWDRMode) {
		if (IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE ==
		    u8ImgMode) { /* IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE */
			imx290_wdr_1080p60_2to1_init(ViPipe);
		} else if (IMX290_SENSOR_720P_30FPS_2t1_WDR_MODE ==
			   u8ImgMode) { /* IMX290_SENSOR_720P_30FPS_2t1_WDR_MODE */
			imx290_wdr_720p60_2to1_init(ViPipe);
		} else {
			imx290_wdr_1080p60_2to1_init(ViPipe);
		}
	} else {
		imx290_linear_1080p30_init(ViPipe);
	}
	g_pastImx290[ViPipe]->bInit = GK_TRUE;
	return;
}

void imx290_exit(VI_PIPE ViPipe)
{
	imx290_i2c_exit(ViPipe);

	return;
}

/* 1080P30 and 1080P25 */
void imx290_linear_1080p30_init(VI_PIPE ViPipe)
{
	imx290_write_register(ViPipe, 0x3000, 0x01); /* standby */
	imx290_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

	imx290_write_register(ViPipe, 0x3005, 0x01); // ADBIT
	imx290_write_register(ViPipe, 0x3129, 0x00); // ADBIT1
	imx290_write_register(ViPipe, 0x317c, 0x00); // ADBIT2
	imx290_write_register(ViPipe, 0x31ec, 0x0e); // ADBIT3
	imx290_write_register(ViPipe, 0x3441, 0x0c); // CSI_DT_FMT
	imx290_write_register(ViPipe, 0x3442, 0x0c); // CSI_DT_FMT

	imx290_write_register(ViPipe, 0x3007, 0x00);
	imx290_write_register(ViPipe, 0x300c, 0x00);
	imx290_write_register(ViPipe, 0x300f, 0x00);
	imx290_write_register(ViPipe, 0x3010, 0x21);
	imx290_write_register(ViPipe, 0x3012, 0x64);
	imx290_write_register(ViPipe, 0x3016, 0x09);
	imx290_write_register(ViPipe, 0x3017, 0x00);

#if 1
	imx290_write_register(ViPipe, 0x3020, 0x01); /* SHS1 */
	imx290_write_register(ViPipe, 0x3021, 0x00);
	imx290_write_register(ViPipe, 0x3024, 0x00); /* SHS2 */
	imx290_write_register(ViPipe, 0x3025, 0x00);
	imx290_write_register(ViPipe, 0x3028, 0x00); /* SHS3 */
	imx290_write_register(ViPipe, 0x3029, 0x00);
	imx290_write_register(ViPipe, 0x3030, 0x00); /* RHS1 */
	imx290_write_register(ViPipe, 0x3031, 0x00);
	imx290_write_register(ViPipe, 0x3034, 0x00); /* RHS2 */
	imx290_write_register(ViPipe, 0x3035, 0x00);
#else
	imx290_write_register(ViPipe, 0x3020, 0x02);
#endif

	imx290_write_register(ViPipe, 0x305c, 0x18);
	imx290_write_register(ViPipe, 0x305d, 0x03);
	imx290_write_register(ViPipe, 0x305e, 0x20);
	imx290_write_register(ViPipe, 0x305f, 0x01);
	imx290_write_register(ViPipe, 0x3070, 0x02);
	imx290_write_register(ViPipe, 0x3071, 0x11);
	imx290_write_register(ViPipe, 0x309b, 0x10);
	imx290_write_register(ViPipe, 0x309c, 0x22);
	imx290_write_register(ViPipe, 0x30a2, 0x02);
	imx290_write_register(ViPipe, 0x30a6, 0x20);
	imx290_write_register(ViPipe, 0x30a8, 0x20);
	imx290_write_register(ViPipe, 0x30aa, 0x20);
	imx290_write_register(ViPipe, 0x30ac, 0x20);

	imx290_write_register(ViPipe, 0x30b0, 0x43);
	imx290_write_register(ViPipe, 0x3119, 0x9e);
	imx290_write_register(ViPipe, 0x311c, 0x1e);
	imx290_write_register(ViPipe, 0x311e, 0x08);
	imx290_write_register(ViPipe, 0x3128, 0x05);
	imx290_write_register(ViPipe, 0x313d, 0x83);
	imx290_write_register(ViPipe, 0x3150, 0x03);

	imx290_write_register(ViPipe, 0x317e, 0x00);
	imx290_write_register(ViPipe, 0x315e, 0x1a);
	imx290_write_register(ViPipe, 0x3164, 0x1a);
	imx290_write_register(ViPipe, 0x32b8, 0x50);

	imx290_write_register(ViPipe, 0x32b9, 0x10);
	imx290_write_register(ViPipe, 0x32ba, 0x00);
	imx290_write_register(ViPipe, 0x32bb, 0x04);
	imx290_write_register(ViPipe, 0x32c8, 0x50);
	imx290_write_register(ViPipe, 0x32c9, 0x10);
	imx290_write_register(ViPipe, 0x32ca, 0x00);
	imx290_write_register(ViPipe, 0x32cb, 0x04);
	imx290_write_register(ViPipe, 0x332c, 0xd3);
	imx290_write_register(ViPipe, 0x332d, 0x10);
	imx290_write_register(ViPipe, 0x332e, 0x0d);
	imx290_write_register(ViPipe, 0x3358, 0x06);
	imx290_write_register(ViPipe, 0x3359, 0xe1);
	imx290_write_register(ViPipe, 0x335a, 0x11);
	imx290_write_register(ViPipe, 0x3360, 0x1e);

	imx290_write_register(ViPipe, 0x3361, 0x61);
	imx290_write_register(ViPipe, 0x3362, 0x10);
	imx290_write_register(ViPipe, 0x33b0, 0x50);
	imx290_write_register(ViPipe, 0x33b2, 0x1a);
	imx290_write_register(ViPipe, 0x33b3, 0x04);
	imx290_write_register(ViPipe, 0x3414, 0x0a);
	imx290_write_register(ViPipe, 0x3418, 0x49);
	imx290_write_register(ViPipe, 0x3419, 0x04);
	imx290_write_register(ViPipe, 0x3444, 0x20);
	imx290_write_register(ViPipe, 0x3445, 0x25);

	imx290_write_register(ViPipe, 0x3446, 0x47);
	imx290_write_register(ViPipe, 0x3447, 0x0);
	imx290_write_register(ViPipe, 0x3448, 0x1f);
	imx290_write_register(ViPipe, 0x3449, 0x0);
	imx290_write_register(ViPipe, 0x344a, 0x17);
	imx290_write_register(ViPipe, 0x344b, 0x0);
	imx290_write_register(ViPipe, 0x344c, 0x0f);
	imx290_write_register(ViPipe, 0x344d, 0x0);
	imx290_write_register(ViPipe, 0x344e, 0x17);
	imx290_write_register(ViPipe, 0x344f, 0x0);
	imx290_write_register(ViPipe, 0x3450, 0x47);
	imx290_write_register(ViPipe, 0x3451, 0x0);
	imx290_write_register(ViPipe, 0x3452, 0x0f);
	imx290_write_register(ViPipe, 0x3453, 0x0);
	imx290_write_register(ViPipe, 0x3454, 0x0f);
	imx290_write_register(ViPipe, 0x3455, 0x0);
	imx290_write_register(ViPipe, 0x3480, 0x49);

	imx290_default_reg_init(ViPipe);
	imx290_write_register(ViPipe, 0x3000, 0x00); /* standby */
	delay_ms(20);
	imx290_write_register(ViPipe, 0x3002, 0x00); /* master mode start */
	imx290_write_register(ViPipe, 0x304b, 0x0a); /* XVSOUTSEL XHSOUTSEL */

	printf("===IMX290 1080P 30fps 12bit LINE Init OK!===\n");
	return;
}

void imx290_wdr_1080p30_2to1_init(VI_PIPE ViPipe)
{
	imx290_write_register(ViPipe, 0x3000, 0x01); /* standby */
	imx290_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

	// 10bit
	imx290_write_register(ViPipe, 0x3005, 0x00);
	imx290_write_register(ViPipe, 0x3007, 0x00);
	imx290_write_register(ViPipe, 0x300a, 0x3c);
	imx290_write_register(ViPipe, 0x300c, 0x11);
	imx290_write_register(ViPipe, 0x300f, 0x00);
	imx290_write_register(ViPipe, 0x3010, 0x21);
	imx290_write_register(ViPipe, 0x3012, 0x64);
	imx290_write_register(ViPipe, 0x3016, 0x09);

	imx290_write_register(ViPipe, 0x3018, 0xA6); /**** VMAX ****/
	imx290_write_register(ViPipe, 0x3019, 0x04);
	imx290_write_register(ViPipe, 0x301A, 0x00);

	imx290_write_register(ViPipe, 0x301C, 0xD8); /***** HMAX ****/
	imx290_write_register(ViPipe, 0x301D, 0x0F);

	imx290_write_register(ViPipe, 0x3020, 0x02);
	imx290_write_register(ViPipe, 0x3024, 0xc9);
	imx290_write_register(ViPipe, 0x3030, 0x0b);
	imx290_write_register(ViPipe, 0x3045, 0x05);
	imx290_write_register(ViPipe, 0x3046, 0x00);
	imx290_write_register(ViPipe, 0x304b, 0x0a);
	imx290_write_register(ViPipe, 0x305c, 0x18);
	imx290_write_register(ViPipe, 0x305d, 0x03);
	imx290_write_register(ViPipe, 0x305e, 0x20);
	imx290_write_register(ViPipe, 0x305f, 0x01);
	imx290_write_register(ViPipe, 0x3070, 0x02);
	imx290_write_register(ViPipe, 0x3071, 0x11);
	imx290_write_register(ViPipe, 0x309b, 0x10);
	imx290_write_register(ViPipe, 0x309c, 0x22);
	imx290_write_register(ViPipe, 0x30a2, 0x02);
	imx290_write_register(ViPipe, 0x30a6, 0x20);
	imx290_write_register(ViPipe, 0x30a8, 0x20);
	imx290_write_register(ViPipe, 0x30aa, 0x20);
	imx290_write_register(ViPipe, 0x30ac, 0x20);
	imx290_write_register(ViPipe, 0x30b0, 0x43);
	imx290_write_register(ViPipe, 0x3106, 0x11);
	imx290_write_register(ViPipe, 0x3119, 0x9e);
	imx290_write_register(ViPipe, 0x311c, 0x1e);
	imx290_write_register(ViPipe, 0x311e, 0x08);
	imx290_write_register(ViPipe, 0x3128, 0x05);
	imx290_write_register(ViPipe, 0x3129, 0x1d);
	imx290_write_register(ViPipe, 0x313d, 0x83);
	imx290_write_register(ViPipe, 0x3150, 0x03);
	imx290_write_register(ViPipe, 0x315e, 0x1a);
	imx290_write_register(ViPipe, 0x3164, 0x1a);
	imx290_write_register(ViPipe, 0x317c, 0x12);
	imx290_write_register(ViPipe, 0x317e, 0x00);
	imx290_write_register(ViPipe, 0x31ec, 0x37);
	imx290_write_register(ViPipe, 0x32b8, 0x50);
	imx290_write_register(ViPipe, 0x32b9, 0x10);
	imx290_write_register(ViPipe, 0x32ba, 0x00);
	imx290_write_register(ViPipe, 0x32bb, 0x04);
	imx290_write_register(ViPipe, 0x32c8, 0x50);
	imx290_write_register(ViPipe, 0x32c9, 0x10);
	imx290_write_register(ViPipe, 0x32ca, 0x00);
	imx290_write_register(ViPipe, 0x32cb, 0x04);
	imx290_write_register(ViPipe, 0x332c, 0xd3);
	imx290_write_register(ViPipe, 0x332d, 0x10);
	imx290_write_register(ViPipe, 0x332e, 0x0d);
	imx290_write_register(ViPipe, 0x3358, 0x06);
	imx290_write_register(ViPipe, 0x3359, 0xe1);
	imx290_write_register(ViPipe, 0x335a, 0x11);
	imx290_write_register(ViPipe, 0x3360, 0x1e);
	imx290_write_register(ViPipe, 0x3361, 0x61);
	imx290_write_register(ViPipe, 0x3362, 0x10);
	imx290_write_register(ViPipe, 0x33b0, 0x50);
	imx290_write_register(ViPipe, 0x33b2, 0x1a);
	imx290_write_register(ViPipe, 0x33b3, 0x04);

	imx290_write_register(ViPipe, 0x3418, 0xb2); /**** Y_OUT_SIZE *****/
	imx290_write_register(ViPipe, 0x3419, 0x08);

	imx290_write_register(ViPipe, 0x3441, 0x0a);
	imx290_write_register(ViPipe, 0x3442, 0x0a);
	imx290_write_register(ViPipe, 0x3444, 0x20);
	imx290_write_register(ViPipe, 0x3445, 0x25);
	imx290_write_register(ViPipe, 0x3480, 0x49);

	imx290_default_reg_init(ViPipe);
	imx290_write_register(ViPipe, 0x3000, 0x00); /* standby */
	delay_ms(20);
	imx290_write_register(ViPipe, 0x3002, 0x00); /* master mode start */

	printf("===Imx290 sensor 1080P15fps 10bit 2to1 WDR(30fps->15fps) init success!=====\n");

	return;
}

void imx290_wdr_1080p60_2to1_init(VI_PIPE ViPipe)
{
	imx290_write_register(ViPipe, 0x3000, 0x01); /* standby */
	imx290_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

	imx290_write_register(ViPipe, 0x3005, 0x00);
	imx290_write_register(ViPipe, 0x3007, 0x00);
	imx290_write_register(ViPipe, 0x3009, 0x01);
	imx290_write_register(ViPipe, 0x300a, 0x3c);
	imx290_write_register(ViPipe, 0x300c, 0x11);
	imx290_write_register(ViPipe, 0x300f, 0x00);
	imx290_write_register(ViPipe, 0x3010, 0x21);
	imx290_write_register(ViPipe, 0x3012, 0x64);
	imx290_write_register(ViPipe, 0x3016, 0x09);
	imx290_write_register(ViPipe, 0x3018, 0xC4); /* VMAX */
	imx290_write_register(ViPipe, 0x3019, 0x04); /* VMAX */
	imx290_write_register(ViPipe, 0x301c, 0xEC); /* HMAX */
	imx290_write_register(ViPipe, 0x301d, 0x07); /* HMAX */

#if 1
	imx290_write_register(ViPipe, 0x3020, 0x03); /* SHS1 */
	imx290_write_register(ViPipe, 0x3021, 0x00);
	imx290_write_register(ViPipe, 0x3024, 0x99); /* SHS2 */
	imx290_write_register(ViPipe, 0x3025, 0x00);
	imx290_write_register(ViPipe, 0x3028, 0x00); /* SHS3 */
	imx290_write_register(ViPipe, 0x3029, 0x00);
	imx290_write_register(ViPipe, 0x3030, 0x93); /* RHS1 */
	imx290_write_register(ViPipe, 0x3031, 0x00);
	imx290_write_register(ViPipe, 0x3034, 0x00); /* RHS2 */
	imx290_write_register(ViPipe, 0x3035, 0x00);
#endif

	imx290_write_register(ViPipe, 0x3045, 0x05);
	imx290_write_register(ViPipe, 0x3046, 0x00);
	imx290_write_register(ViPipe, 0x304b, 0x0a);
	imx290_write_register(ViPipe, 0x305c, 0x18);
	imx290_write_register(ViPipe, 0x305d, 0x03);
	imx290_write_register(ViPipe, 0x305e, 0x20);
	imx290_write_register(ViPipe, 0x305f, 0x01);
	imx290_write_register(ViPipe, 0x3070, 0x02);
	imx290_write_register(ViPipe, 0x3071, 0x11);
	imx290_write_register(ViPipe, 0x309b, 0x10);
	imx290_write_register(ViPipe, 0x309c, 0x22);
	imx290_write_register(ViPipe, 0x30a2, 0x02);
	imx290_write_register(ViPipe, 0x30a6, 0x20);
	imx290_write_register(ViPipe, 0x30a8, 0x20);
	imx290_write_register(ViPipe, 0x30aa, 0x20);
	imx290_write_register(ViPipe, 0x30ac, 0x20);
	imx290_write_register(ViPipe, 0x30b0, 0x43);

	imx290_write_register(ViPipe, 0x3106, 0x11);
	imx290_write_register(ViPipe, 0x3119, 0x9e);
	imx290_write_register(ViPipe, 0x311c, 0x1e);
	imx290_write_register(ViPipe, 0x311e, 0x08);
	imx290_write_register(ViPipe, 0x3128, 0x05);
	imx290_write_register(ViPipe, 0x3129, 0x1d);
	imx290_write_register(ViPipe, 0x313d, 0x83);
	imx290_write_register(ViPipe, 0x3150, 0x03);
	imx290_write_register(ViPipe, 0x315e, 0x1a);
	imx290_write_register(ViPipe, 0x3164, 0x1a);
	imx290_write_register(ViPipe, 0x317c, 0x12);
	imx290_write_register(ViPipe, 0x317e, 0x00);
	imx290_write_register(ViPipe, 0x31ec, 0x37);

	imx290_write_register(ViPipe, 0x32b8, 0x50);
	imx290_write_register(ViPipe, 0x32b9, 0x10);
	imx290_write_register(ViPipe, 0x32ba, 0x00);
	imx290_write_register(ViPipe, 0x32bb, 0x04);
	imx290_write_register(ViPipe, 0x32c8, 0x50);
	imx290_write_register(ViPipe, 0x32c9, 0x10);
	imx290_write_register(ViPipe, 0x32ca, 0x00);
	imx290_write_register(ViPipe, 0x32cb, 0x04);

	imx290_write_register(ViPipe, 0x332c, 0xd3);
	imx290_write_register(ViPipe, 0x332d, 0x10);
	imx290_write_register(ViPipe, 0x332e, 0x0d);
	imx290_write_register(ViPipe, 0x3358, 0x06);
	imx290_write_register(ViPipe, 0x3359, 0xe1);
	imx290_write_register(ViPipe, 0x335a, 0x11);
	imx290_write_register(ViPipe, 0x3360, 0x1e);
	imx290_write_register(ViPipe, 0x3361, 0x61);
	imx290_write_register(ViPipe, 0x3362, 0x10);
	imx290_write_register(ViPipe, 0x33b0, 0x50);
	imx290_write_register(ViPipe, 0x33b2, 0x1a);
	imx290_write_register(ViPipe, 0x33b3, 0x04);

	imx290_write_register(ViPipe, 0x3405, 0x10);
	imx290_write_register(ViPipe, 0x3407, 0x03);
	imx290_write_register(ViPipe, 0x3414, 0x0a);
	imx290_write_register(ViPipe, 0x3415, 0x00);
	imx290_write_register(ViPipe, 0x3418, 0x32); /* Y_OUT_SIZE */
	imx290_write_register(ViPipe, 0x3419, 0x09); /* Y_OUT_SIZE */
	imx290_write_register(ViPipe, 0x3441, 0x0a);
	imx290_write_register(ViPipe, 0x3442, 0x0a);
	imx290_write_register(ViPipe, 0x3443, 0x03);
	imx290_write_register(ViPipe, 0x3444, 0x20);
	imx290_write_register(ViPipe, 0x3445, 0x25);
	imx290_write_register(ViPipe, 0x3446, 0x57);
	imx290_write_register(ViPipe, 0x3447, 0x00);
	imx290_write_register(ViPipe, 0x3448, 0x37);
	imx290_write_register(ViPipe, 0x3449, 0x00);
	imx290_write_register(ViPipe, 0x344a, 0x1f);
	imx290_write_register(ViPipe, 0x344b, 0x00);
	imx290_write_register(ViPipe, 0x344c, 0x1f);
	imx290_write_register(ViPipe, 0x344d, 0x00);
	imx290_write_register(ViPipe, 0x344e, 0x1f);
	imx290_write_register(ViPipe, 0x344f, 0x00);
	imx290_write_register(ViPipe, 0x3450, 0x77);
	imx290_write_register(ViPipe, 0x3451, 0x00);
	imx290_write_register(ViPipe, 0x3452, 0x1f);
	imx290_write_register(ViPipe, 0x3453, 0x00);
	imx290_write_register(ViPipe, 0x3454, 0x17);
	imx290_write_register(ViPipe, 0x3455, 0x00);
	imx290_write_register(ViPipe, 0x3472, 0x9c);
	imx290_write_register(ViPipe, 0x3473, 0x07);
	imx290_write_register(ViPipe, 0x3480, 0x49);
	imx290_default_reg_init(ViPipe);
	imx290_write_register(ViPipe, 0x3000, 0x00); /* standby */
	delay_ms(20);
	imx290_write_register(ViPipe, 0x3002, 0x00); /* master mode start */

	printf("===Imx290 sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");

	return;
}

void imx290_wdr_720p60_2to1_init(VI_PIPE ViPipe)
{
	imx290_write_register(ViPipe, 0x3000, 0x01); /* standby */
	imx290_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

	// 12bit
	imx290_write_register(ViPipe, 0x3005, 0x01);
	imx290_write_register(ViPipe, 0x3007, 0x10);
	imx290_write_register(ViPipe, 0x3009, 0x01);
	imx290_write_register(ViPipe, 0x300a, 0xf0);
	imx290_write_register(ViPipe, 0x300c, 0x11);
	imx290_write_register(ViPipe, 0x300f, 0x00);
	imx290_write_register(ViPipe, 0x3010, 0x21);
	imx290_write_register(ViPipe, 0x3012, 0x64);
	imx290_write_register(ViPipe, 0x3016, 0x09);
	imx290_write_register(ViPipe, 0x3018, 0xee);
	imx290_write_register(ViPipe, 0x3019, 0x02);
	imx290_write_register(ViPipe, 0x301c, 0xe4);
	imx290_write_register(ViPipe, 0x301d, 0x0c);
	imx290_write_register(ViPipe, 0x3045, 0x05);
	imx290_write_register(ViPipe, 0x3046, 0x01);
	imx290_write_register(ViPipe, 0x304b, 0x0a);

	imx290_write_register(ViPipe, 0x305c, 0x20); // INCKSEL1
	imx290_write_register(ViPipe, 0x305d, 0x03); // INCKSEL2
	imx290_write_register(ViPipe, 0x305e, 0x20); // INCKSEL3
	imx290_write_register(ViPipe, 0x305f, 0x01); // INCKSEL4

	imx290_write_register(ViPipe, 0x3070, 0x02);
	imx290_write_register(ViPipe, 0x3071, 0x11);
	imx290_write_register(ViPipe, 0x309b, 0x10);
	imx290_write_register(ViPipe, 0x309c, 0x22);
	imx290_write_register(ViPipe, 0x30a2, 0x02);
	imx290_write_register(ViPipe, 0x30a6, 0x20);
	imx290_write_register(ViPipe, 0x30a8, 0x20);
	imx290_write_register(ViPipe, 0x30aa, 0x20);
	imx290_write_register(ViPipe, 0x30ac, 0x20);
	imx290_write_register(ViPipe, 0x30b0, 0x43);

	// Add
	imx290_write_register(ViPipe, 0x3106, 0x11);

	imx290_write_register(ViPipe, 0x3119, 0x9e);
	imx290_write_register(ViPipe, 0x311c, 0x1e);
	imx290_write_register(ViPipe, 0x311e, 0x08);
	imx290_write_register(ViPipe, 0x3128, 0x05);
	imx290_write_register(ViPipe, 0x3129, 0x00);
	imx290_write_register(ViPipe, 0x313d, 0x83);
	imx290_write_register(ViPipe, 0x3150, 0x03);
	imx290_write_register(ViPipe, 0x315e, 0x1a);
	imx290_write_register(ViPipe, 0x3164, 0x1a);
	imx290_write_register(ViPipe, 0x317c, 0x00);
	imx290_write_register(ViPipe, 0x317e, 0x00);
	imx290_write_register(ViPipe, 0x31ec, 0x00);

	imx290_write_register(ViPipe, 0x32b8, 0x50);
	imx290_write_register(ViPipe, 0x32b9, 0x10);
	imx290_write_register(ViPipe, 0x32ba, 0x00);
	imx290_write_register(ViPipe, 0x32bb, 0x04);
	imx290_write_register(ViPipe, 0x32c8, 0x50);
	imx290_write_register(ViPipe, 0x32c9, 0x10);
	imx290_write_register(ViPipe, 0x32ca, 0x00);
	imx290_write_register(ViPipe, 0x32cb, 0x04);

	imx290_write_register(ViPipe, 0x332c, 0xd3);
	imx290_write_register(ViPipe, 0x332d, 0x10);
	imx290_write_register(ViPipe, 0x332e, 0x0d);
	imx290_write_register(ViPipe, 0x3358, 0x06);
	imx290_write_register(ViPipe, 0x3359, 0xe1);
	imx290_write_register(ViPipe, 0x335a, 0x11);
	imx290_write_register(ViPipe, 0x3360, 0x1e);
	imx290_write_register(ViPipe, 0x3361, 0x61);
	imx290_write_register(ViPipe, 0x3362, 0x10);
	imx290_write_register(ViPipe, 0x33b0, 0x50);
	imx290_write_register(ViPipe, 0x33b2, 0x1a);
	imx290_write_register(ViPipe, 0x33b3, 0x04);

	imx290_write_register(ViPipe, 0x3405, 0x10);
	imx290_write_register(ViPipe, 0x3407, 0x03);
	imx290_write_register(ViPipe, 0x3414, 0x04);
	imx290_write_register(ViPipe, 0x3418, 0xc6);
	imx290_write_register(ViPipe, 0x3419, 0x05);
	imx290_write_register(ViPipe, 0x3441, 0x0c);
	imx290_write_register(ViPipe, 0x3442, 0x0c);
	imx290_write_register(ViPipe, 0x3443, 0x03);
	imx290_write_register(ViPipe, 0x3444, 0x20);
	imx290_write_register(ViPipe, 0x3445, 0x25);
	imx290_write_register(ViPipe, 0x3446, 0x4f);
	imx290_write_register(ViPipe, 0x3447, 0x00);
	imx290_write_register(ViPipe, 0x3448, 0x2f);
	imx290_write_register(ViPipe, 0x3449, 0x00);
	imx290_write_register(ViPipe, 0x344a, 0x17);
	imx290_write_register(ViPipe, 0x344b, 0x00);
	imx290_write_register(ViPipe, 0x344c, 0x17);
	imx290_write_register(ViPipe, 0x344d, 0x00);
	imx290_write_register(ViPipe, 0x344e, 0x17);
	imx290_write_register(ViPipe, 0x344f, 0x00);
	imx290_write_register(ViPipe, 0x3450, 0x57);
	imx290_write_register(ViPipe, 0x3451, 0x00);
	imx290_write_register(ViPipe, 0x3452, 0x17);
	imx290_write_register(ViPipe, 0x3453, 0x00);
	imx290_write_register(ViPipe, 0x3454, 0x17);
	imx290_write_register(ViPipe, 0x3455, 0x00);
	imx290_write_register(ViPipe, 0x3472, 0x1c);
	imx290_write_register(ViPipe, 0x3473, 0x05);
	imx290_write_register(ViPipe, 0x3480, 0x49);
	imx290_default_reg_init(ViPipe);
	imx290_write_register(ViPipe, 0x3000, 0x00); /* standby */
	delay_ms(20);
	imx290_write_register(ViPipe, 0x3002, 0x00); /* master mode start */

	printf("===Imx290 imx290 720P30fps 12bit 2to1 WDR(60fps->30fps) init success!=====\n");

	return;
}

void imx290_wdr_1080p120_2to1_init(VI_PIPE ViPipe)
{
	imx290_write_register(ViPipe, 0x3000, 0x01); /* standby */
	imx290_write_register(ViPipe, 0x3002, 0x01); /* XTMSTA */

	imx290_write_register(ViPipe, 0x3005, 0x00);
	imx290_write_register(ViPipe, 0x3007, 0x00);
	imx290_write_register(ViPipe, 0x3009, 0x00);
	imx290_write_register(ViPipe, 0x300a, 0x3c);
	imx290_write_register(ViPipe, 0x300c, 0x11);
	imx290_write_register(ViPipe, 0x300f, 0x00);
	imx290_write_register(ViPipe, 0x3010, 0x21);
	imx290_write_register(ViPipe, 0x3012, 0x64);
	imx290_write_register(ViPipe, 0x3016, 0x09);
	imx290_write_register(ViPipe, 0x3018, 0x65);
	imx290_write_register(ViPipe, 0x3019, 0x04);
	imx290_write_register(ViPipe, 0x301c, 0xF6);
	imx290_write_register(ViPipe, 0x301d, 0x03);
	imx290_write_register(ViPipe, 0x3020, 0x02);
	imx290_write_register(ViPipe, 0x3024, 0xc9);
	imx290_write_register(ViPipe, 0x3030, 0x0b);
	imx290_write_register(ViPipe, 0x3045, 0x05);
	imx290_write_register(ViPipe, 0x3046, 0x00);
	imx290_write_register(ViPipe, 0x304b, 0x0a);
	imx290_write_register(ViPipe, 0x305c, 0x18);
	imx290_write_register(ViPipe, 0x305d, 0x03);
	imx290_write_register(ViPipe, 0x305e, 0x20);
	imx290_write_register(ViPipe, 0x305f, 0x01);
	imx290_write_register(ViPipe, 0x3070, 0x02);
	imx290_write_register(ViPipe, 0x3071, 0x11);
	imx290_write_register(ViPipe, 0x309b, 0x10);
	imx290_write_register(ViPipe, 0x309c, 0x22);
	imx290_write_register(ViPipe, 0x30a2, 0x02);
	imx290_write_register(ViPipe, 0x30a6, 0x20);
	imx290_write_register(ViPipe, 0x30a8, 0x20);
	imx290_write_register(ViPipe, 0x30aa, 0x20);
	imx290_write_register(ViPipe, 0x30ac, 0x20);
	imx290_write_register(ViPipe, 0x30b0, 0x43);
	imx290_write_register(ViPipe, 0x3106, 0x11);
	imx290_write_register(ViPipe, 0x3119, 0x9e);
	imx290_write_register(ViPipe, 0x311c, 0x1e);
	imx290_write_register(ViPipe, 0x311e, 0x08);
	imx290_write_register(ViPipe, 0x3128, 0x05);
	imx290_write_register(ViPipe, 0x3129, 0x1d);
	imx290_write_register(ViPipe, 0x313d, 0x83);
	imx290_write_register(ViPipe, 0x3150, 0x03);
	imx290_write_register(ViPipe, 0x315e, 0x1a);
	imx290_write_register(ViPipe, 0x3164, 0x1a);
	imx290_write_register(ViPipe, 0x317c, 0x12);
	imx290_write_register(ViPipe, 0x317e, 0x00);
	imx290_write_register(ViPipe, 0x31ec, 0x37);

	imx290_write_register(ViPipe, 0x32b8, 0x50);
	imx290_write_register(ViPipe, 0x32b9, 0x10);
	imx290_write_register(ViPipe, 0x32ba, 0x00);
	imx290_write_register(ViPipe, 0x32bb, 0x04);
	imx290_write_register(ViPipe, 0x32c8, 0x50);
	imx290_write_register(ViPipe, 0x32c9, 0x10);
	imx290_write_register(ViPipe, 0x32ca, 0x00);
	imx290_write_register(ViPipe, 0x32cb, 0x04);

	imx290_write_register(ViPipe, 0x332c, 0xd3);
	imx290_write_register(ViPipe, 0x332d, 0x10);
	imx290_write_register(ViPipe, 0x332e, 0x0d);
	imx290_write_register(ViPipe, 0x3358, 0x06);
	imx290_write_register(ViPipe, 0x3359, 0xe1);
	imx290_write_register(ViPipe, 0x335a, 0x11);
	imx290_write_register(ViPipe, 0x3360, 0x1e);
	imx290_write_register(ViPipe, 0x3361, 0x61);
	imx290_write_register(ViPipe, 0x3362, 0x10);
	imx290_write_register(ViPipe, 0x33b0, 0x50);
	imx290_write_register(ViPipe, 0x33b2, 0x1a);
	imx290_write_register(ViPipe, 0x33b3, 0x04);

	imx290_write_register(ViPipe, 0x3405, 0x00);
	imx290_write_register(ViPipe, 0x3407, 0x03);
	imx290_write_register(ViPipe, 0x3414, 0x0a);
	imx290_write_register(ViPipe, 0x3418, 0xb2);
	imx290_write_register(ViPipe, 0x3419, 0x08);
	imx290_write_register(ViPipe, 0x3441, 0x0a);
	imx290_write_register(ViPipe, 0x3442, 0x0a);
	imx290_write_register(ViPipe, 0x3443, 0x03);
	imx290_write_register(ViPipe, 0x3444, 0x20);
	imx290_write_register(ViPipe, 0x3445, 0x25);
	imx290_write_register(ViPipe, 0x3446, 0x77);
	imx290_write_register(ViPipe, 0x3447, 0x00);
	imx290_write_register(ViPipe, 0x3448, 0x67);
	imx290_write_register(ViPipe, 0x3449, 0x00);
	imx290_write_register(ViPipe, 0x344a, 0x47);
	imx290_write_register(ViPipe, 0x344b, 0x00);
	imx290_write_register(ViPipe, 0x344c, 0x37);
	imx290_write_register(ViPipe, 0x344d, 0x00);
	imx290_write_register(ViPipe, 0x344e, 0x3f);
	imx290_write_register(ViPipe, 0x344f, 0x00);
	imx290_write_register(ViPipe, 0x3450, 0xff);
	imx290_write_register(ViPipe, 0x3451, 0x00);
	imx290_write_register(ViPipe, 0x3452, 0x3f);
	imx290_write_register(ViPipe, 0x3453, 0x00);
	imx290_write_register(ViPipe, 0x3454, 0x37);
	imx290_write_register(ViPipe, 0x3455, 0x00);
	imx290_write_register(ViPipe, 0x3472, 0x9c);
	imx290_write_register(ViPipe, 0x3473, 0x07);
	imx290_write_register(ViPipe, 0x3480, 0x49);
	imx290_default_reg_init(ViPipe);
	imx290_write_register(ViPipe, 0x3000, 0x00); /* standby */
	delay_ms(20);
	imx290_write_register(ViPipe, 0x3002, 0x00); /* master mode start */

	printf("===Imx290 imx290 1080P60fps 10bit 2to1 WDR(120fps->60fps) init success!=====\n");
	return;
}
