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

const unsigned char ar0237_i2c_addr = 0x20; /* I2C Address of AR0237 */
const unsigned int ar0237_addr_byte = 2; /* ADDR byte of AR0237 */
const unsigned int ar0237_data_byte = 2; /* DATA byte of AR0237 */
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern GK_U8 gu8SensorImageMode;
extern WDR_MODE_E genSensorMode;
extern GK_BOOL bSensorInit;

extern ISP_SNS_STATE_S *g_pastAr0237[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunAr0237BusInfo[];

int ar0237_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunAr0237BusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (ar0237_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int ar0237_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int ar0237_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int ar0237_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}
#ifdef GPIO_I2C
	i2c_data.dev_addr = ar0237_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = ar0237_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = ar0237_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (ar0237_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
		buf[idx] = addr & 0xff;
		idx++;
	}

	if (ar0237_data_byte == 2) {
		buf[idx] = (data >> 8) & 0xff;
		idx++;
		buf[idx] = data & 0xff;
		idx++;
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, (ar0237_addr_byte + ar0237_data_byte));
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

#define AR0237_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define AR0237_SENSOR_1080P_30FPS_3t1_WDR_MODE (2)
#define AR0237_SENSOR_1080P_30FPS_2t1_WDR_MODE (3)
#define AR0237_SENSOR_720P_30FPS_2t1_WDR_MODE (4)
void ar0237_linear_1080p30_init(VI_PIPE ViPipe);
void ar0237_wdr_1080p30_init(VI_PIPE ViPipe);

void ar0237_init(VI_PIPE ViPipe)
{
	WDR_MODE_E enWDRMode;
	GK_BOOL bInit;

	bInit = g_pastAr0237[ViPipe]->bInit;
	enWDRMode = g_pastAr0237[ViPipe]->enWDRMode;
	ar0237_i2c_init(ViPipe);
	if (bInit == GK_FALSE) {
		if (WDR_MODE_2To1_LINE == enWDRMode) {
			ar0237_wdr_1080p30_init(ViPipe);

		} else if (enWDRMode == WDR_MODE_NONE) {
			ar0237_linear_1080p30_init(ViPipe);

		} else {
			printf("Not support!\n");
			return;
		}
	}
	/* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
	else {
		if (WDR_MODE_2To1_LINE == enWDRMode) {
			ar0237_wdr_1080p30_init(ViPipe);
		} else if (enWDRMode == WDR_MODE_NONE) {
			ar0237_linear_1080p30_init(ViPipe);
		} else {
			printf("Not support!\n");
			return;
		}
	}

	g_pastAr0237[ViPipe]->bInit = GK_TRUE;
	return;
}

void ar0237_exit(VI_PIPE ViPipe)
{
	ar0237_i2c_exit(ViPipe);

	return;
}

void ar0237_standby(VI_PIPE ViPipe)
{
	return;
}

void ar0237_restart(VI_PIPE ViPipe)
{
	return;
}

// INCLK=27M
void ar0237_linear_1080p30_init(VI_PIPE ViPipe)
{
	// #[HiSPi Linear 1080p30 Low Power Mode - 4 Lane]
	// #Load = Reset
	ar0237_write_register(ViPipe, 0x301A, 0x0001);
	delay_ms(200);
	ar0237_write_register(ViPipe, 0x301A, 0x10D8);
	// #LOAD = Linear_mode_sequencer
	// #[Linear_mode_sequencer]
	ar0237_write_register(ViPipe, 0x3088, 0x8000);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x72A6);
	ar0237_write_register(ViPipe, 0x3086, 0x4A31);
	ar0237_write_register(ViPipe, 0x3086, 0x4342);
	ar0237_write_register(ViPipe, 0x3086, 0x8E03);
	ar0237_write_register(ViPipe, 0x3086, 0x2A14);
	ar0237_write_register(ViPipe, 0x3086, 0x4578);
	ar0237_write_register(ViPipe, 0x3086, 0x7B3D);
	ar0237_write_register(ViPipe, 0x3086, 0xFF3D);
	ar0237_write_register(ViPipe, 0x3086, 0xFF3D);
	ar0237_write_register(ViPipe, 0x3086, 0xEA2A);
	ar0237_write_register(ViPipe, 0x3086, 0x043D);
	ar0237_write_register(ViPipe, 0x3086, 0x102A);
	ar0237_write_register(ViPipe, 0x3086, 0x052A);
	ar0237_write_register(ViPipe, 0x3086, 0x1535);
	ar0237_write_register(ViPipe, 0x3086, 0x2A05);
	ar0237_write_register(ViPipe, 0x3086, 0x3D10);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x2A04);
	ar0237_write_register(ViPipe, 0x3086, 0x2A14);
	ar0237_write_register(ViPipe, 0x3086, 0x3DFF);
	ar0237_write_register(ViPipe, 0x3086, 0x3DFF);
	ar0237_write_register(ViPipe, 0x3086, 0x3DEA);
	ar0237_write_register(ViPipe, 0x3086, 0x2A04);
	ar0237_write_register(ViPipe, 0x3086, 0x622A);
	ar0237_write_register(ViPipe, 0x3086, 0x288E);
	ar0237_write_register(ViPipe, 0x3086, 0x0036);
	ar0237_write_register(ViPipe, 0x3086, 0x2A08);
	ar0237_write_register(ViPipe, 0x3086, 0x3D64);
	ar0237_write_register(ViPipe, 0x3086, 0x7A3D);
	ar0237_write_register(ViPipe, 0x3086, 0x0444);
	ar0237_write_register(ViPipe, 0x3086, 0x2C4B);
	ar0237_write_register(ViPipe, 0x3086, 0xA403);
	ar0237_write_register(ViPipe, 0x3086, 0x430D);
	ar0237_write_register(ViPipe, 0x3086, 0x2D46);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x2A90);
	ar0237_write_register(ViPipe, 0x3086, 0x3E06);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x5F16);
	ar0237_write_register(ViPipe, 0x3086, 0x530D);
	ar0237_write_register(ViPipe, 0x3086, 0x1660);
	ar0237_write_register(ViPipe, 0x3086, 0x3E4C);
	ar0237_write_register(ViPipe, 0x3086, 0x2904);
	ar0237_write_register(ViPipe, 0x3086, 0x2984);
	ar0237_write_register(ViPipe, 0x3086, 0x8E03);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFC);
	ar0237_write_register(ViPipe, 0x3086, 0x5C1D);
	ar0237_write_register(ViPipe, 0x3086, 0x5754);
	ar0237_write_register(ViPipe, 0x3086, 0x495F);
	ar0237_write_register(ViPipe, 0x3086, 0x5305);
	ar0237_write_register(ViPipe, 0x3086, 0x5307);
	ar0237_write_register(ViPipe, 0x3086, 0x4D2B);
	ar0237_write_register(ViPipe, 0x3086, 0xF810);
	ar0237_write_register(ViPipe, 0x3086, 0x164C);
	ar0237_write_register(ViPipe, 0x3086, 0x0955);
	ar0237_write_register(ViPipe, 0x3086, 0x562B);
	ar0237_write_register(ViPipe, 0x3086, 0xB82B);
	ar0237_write_register(ViPipe, 0x3086, 0x984E);
	ar0237_write_register(ViPipe, 0x3086, 0x1129);
	ar0237_write_register(ViPipe, 0x3086, 0x9460);
	ar0237_write_register(ViPipe, 0x3086, 0x5C19);
	ar0237_write_register(ViPipe, 0x3086, 0x5C1B);
	ar0237_write_register(ViPipe, 0x3086, 0x4548);
	ar0237_write_register(ViPipe, 0x3086, 0x4508);
	ar0237_write_register(ViPipe, 0x3086, 0x4588);
	ar0237_write_register(ViPipe, 0x3086, 0x29B6);
	ar0237_write_register(ViPipe, 0x3086, 0x8E01);
	ar0237_write_register(ViPipe, 0x3086, 0x2AF8);
	ar0237_write_register(ViPipe, 0x3086, 0x3E02);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFA);
	ar0237_write_register(ViPipe, 0x3086, 0x3F09);
	ar0237_write_register(ViPipe, 0x3086, 0x5C1B);
	ar0237_write_register(ViPipe, 0x3086, 0x29B2);
	ar0237_write_register(ViPipe, 0x3086, 0x3F0C);
	ar0237_write_register(ViPipe, 0x3086, 0x3E03);
	ar0237_write_register(ViPipe, 0x3086, 0x3E15);
	ar0237_write_register(ViPipe, 0x3086, 0x5C13);
	ar0237_write_register(ViPipe, 0x3086, 0x3F11);
	ar0237_write_register(ViPipe, 0x3086, 0x3E0F);
	ar0237_write_register(ViPipe, 0x3086, 0x5F2B);
	ar0237_write_register(ViPipe, 0x3086, 0x902B);
	ar0237_write_register(ViPipe, 0x3086, 0x803E);
	ar0237_write_register(ViPipe, 0x3086, 0x062A);
	ar0237_write_register(ViPipe, 0x3086, 0xF23F);
	ar0237_write_register(ViPipe, 0x3086, 0x103E);
	ar0237_write_register(ViPipe, 0x3086, 0x0160);
	ar0237_write_register(ViPipe, 0x3086, 0x29A2);
	ar0237_write_register(ViPipe, 0x3086, 0x29A3);
	ar0237_write_register(ViPipe, 0x3086, 0x5F4D);
	ar0237_write_register(ViPipe, 0x3086, 0x1C2A);
	ar0237_write_register(ViPipe, 0x3086, 0xFA29);
	ar0237_write_register(ViPipe, 0x3086, 0x8345);
	ar0237_write_register(ViPipe, 0x3086, 0xA83E);
	ar0237_write_register(ViPipe, 0x3086, 0x072A);
	ar0237_write_register(ViPipe, 0x3086, 0xFB3E);
	ar0237_write_register(ViPipe, 0x3086, 0x2945);
	ar0237_write_register(ViPipe, 0x3086, 0x8824);
	ar0237_write_register(ViPipe, 0x3086, 0x3E08);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFA);
	ar0237_write_register(ViPipe, 0x3086, 0x5D29);
	ar0237_write_register(ViPipe, 0x3086, 0x9288);
	ar0237_write_register(ViPipe, 0x3086, 0x102B);
	ar0237_write_register(ViPipe, 0x3086, 0x048B);
	ar0237_write_register(ViPipe, 0x3086, 0x1686);
	ar0237_write_register(ViPipe, 0x3086, 0x8D48);
	ar0237_write_register(ViPipe, 0x3086, 0x4D4E);
	ar0237_write_register(ViPipe, 0x3086, 0x2B80);
	ar0237_write_register(ViPipe, 0x3086, 0x4C0B);
	ar0237_write_register(ViPipe, 0x3086, 0x3F36);
	ar0237_write_register(ViPipe, 0x3086, 0x2AF2);
	ar0237_write_register(ViPipe, 0x3086, 0x3F10);
	ar0237_write_register(ViPipe, 0x3086, 0x3E01);
	ar0237_write_register(ViPipe, 0x3086, 0x6029);
	ar0237_write_register(ViPipe, 0x3086, 0x8229);
	ar0237_write_register(ViPipe, 0x3086, 0x8329);
	ar0237_write_register(ViPipe, 0x3086, 0x435C);
	ar0237_write_register(ViPipe, 0x3086, 0x155F);
	ar0237_write_register(ViPipe, 0x3086, 0x4D1C);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFA);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x8E00);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x3F0A);
	ar0237_write_register(ViPipe, 0x3086, 0x4A0A);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x0B43);
	ar0237_write_register(ViPipe, 0x3086, 0x168E);
	ar0237_write_register(ViPipe, 0x3086, 0x032A);
	ar0237_write_register(ViPipe, 0x3086, 0x9C45);
	ar0237_write_register(ViPipe, 0x3086, 0x783F);
	ar0237_write_register(ViPipe, 0x3086, 0x072A);
	ar0237_write_register(ViPipe, 0x3086, 0x9D3E);
	ar0237_write_register(ViPipe, 0x3086, 0x305D);
	ar0237_write_register(ViPipe, 0x3086, 0x2944);
	ar0237_write_register(ViPipe, 0x3086, 0x8810);
	ar0237_write_register(ViPipe, 0x3086, 0x2B04);
	ar0237_write_register(ViPipe, 0x3086, 0x530D);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x3E08);
	ar0237_write_register(ViPipe, 0x3086, 0x8E01);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x8E00);
	ar0237_write_register(ViPipe, 0x3086, 0x76A7);
	ar0237_write_register(ViPipe, 0x3086, 0x77A7);
	ar0237_write_register(ViPipe, 0x3086, 0x4644);
	ar0237_write_register(ViPipe, 0x3086, 0x1616);
	ar0237_write_register(ViPipe, 0x3086, 0xA57A);
	ar0237_write_register(ViPipe, 0x3086, 0x1244);
	ar0237_write_register(ViPipe, 0x3086, 0x4B18);
	ar0237_write_register(ViPipe, 0x3086, 0x4A04);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x0643);
	ar0237_write_register(ViPipe, 0x3086, 0x1605);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x0743);
	ar0237_write_register(ViPipe, 0x3086, 0x1658);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x5A43);
	ar0237_write_register(ViPipe, 0x3086, 0x1645);
	ar0237_write_register(ViPipe, 0x3086, 0x588E);
	ar0237_write_register(ViPipe, 0x3086, 0x032A);
	ar0237_write_register(ViPipe, 0x3086, 0x9C45);
	ar0237_write_register(ViPipe, 0x3086, 0x787B);
	ar0237_write_register(ViPipe, 0x3086, 0x3F07);
	ar0237_write_register(ViPipe, 0x3086, 0x2A9D);
	ar0237_write_register(ViPipe, 0x3086, 0x530D);
	ar0237_write_register(ViPipe, 0x3086, 0x8B16);
	ar0237_write_register(ViPipe, 0x3086, 0x863E);
	ar0237_write_register(ViPipe, 0x3086, 0x2345);
	ar0237_write_register(ViPipe, 0x3086, 0x5825);
	ar0237_write_register(ViPipe, 0x3086, 0x3E10);
	ar0237_write_register(ViPipe, 0x3086, 0x8E01);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x8E00);
	ar0237_write_register(ViPipe, 0x3086, 0x3E10);
	ar0237_write_register(ViPipe, 0x3086, 0x8D60);
	ar0237_write_register(ViPipe, 0x3086, 0x1244);
	ar0237_write_register(ViPipe, 0x3086, 0x4BB9);
	ar0237_write_register(ViPipe, 0x3086, 0x2C2C);
	ar0237_write_register(ViPipe, 0x3086, 0x2C2C);

	// #LOAD= Default_Settings_12_Aug
	// #[Default_Settings_12_Aug]
	ar0237_write_register(ViPipe, 0x30F0, 0x1283);
	ar0237_write_register(ViPipe, 0x3064, 0x1802);
	ar0237_write_register(ViPipe, 0x3EEE, 0xA0AA);
	ar0237_write_register(ViPipe, 0x30BA, 0x762C);
	ar0237_write_register(ViPipe, 0x3FA4, 0x0F70);
	ar0237_write_register(ViPipe, 0x309E, 0x016A);
	ar0237_write_register(ViPipe, 0x3096, 0xF880);
	ar0237_write_register(ViPipe, 0x3F32, 0xF880);
	ar0237_write_register(ViPipe, 0x3092, 0x006F);

	ar0237_write_register(ViPipe, 0x301A, 0x0058);
	ar0237_write_register(ViPipe, 0x30B0, 0x0118);
	ar0237_write_register(ViPipe, 0x31AC, 0x0C0C);

	// #LOAD= PLL_settings - 4 Lane 12-bit HiSPi Low Power Mode
	// #[PLL_settings - 4 Lane 12-bit HiSPi Low Power Mode]
	ar0237_write_register(ViPipe, 0x302A, 0x000C);
	ar0237_write_register(ViPipe, 0x302C, 0x0001);
	ar0237_write_register(ViPipe, 0x302E, 0x0004);
	ar0237_write_register(ViPipe, 0x3030, 0x0042);
	ar0237_write_register(ViPipe, 0x3036, 0x000C);
	ar0237_write_register(ViPipe, 0x3038, 0x0002);

	ar0237_write_register(ViPipe, 0x3002, 0x0000);
	ar0237_write_register(ViPipe, 0x3004, 0x0000);
	ar0237_write_register(ViPipe, 0x3006, 0x0437);
	ar0237_write_register(ViPipe, 0x3008, 0x0787);
	ar0237_write_register(ViPipe, 0x300A, 0x0454);
	ar0237_write_register(ViPipe, 0x300C, 0x045E);
	ar0237_write_register(ViPipe, 0x3012, 0x0451);
	ar0237_write_register(ViPipe, 0x30A2, 0x0001);
	ar0237_write_register(ViPipe, 0x30A6, 0x0001);
	ar0237_write_register(ViPipe, 0x30AE, 0x0001);
	ar0237_write_register(ViPipe, 0x30A8, 0x0001);
	ar0237_write_register(ViPipe, 0x3040, 0x0000);

	// #LOAD= Linear Mode Setup
	ar0237_write_register(ViPipe, 0x3082, 0x0009);

	ar0237_write_register(ViPipe, 0x3096, 0x0080);
	ar0237_write_register(ViPipe, 0x3098, 0x0080);
	// #LOAD= Linear Mode Low Conversion Gain
	// #LOAD= 1.5x analog Gain
	ar0237_write_register(ViPipe, 0x3060, 0x0010);

	ar0237_write_register(ViPipe, 0x3100, 0x0000);
	// #LOAD= Companding Disabled
	ar0237_write_register(ViPipe, 0x31D0, 0x0000);
	// #LOAD= AR0230_Rev1_AWB_CCM
	// #LOAD= Enable Embedded Data and Stats

	ar0237_write_register(ViPipe, 0x31AE, 0x0304);
	ar0237_write_register(ViPipe, 0x31C6, 0x0400);
	ar0237_write_register(ViPipe, 0x306E, 0x9010);
	// #patch start###########################
	ar0237_write_register(ViPipe, 0x3ECC, 0x6E2D);
	ar0237_write_register(ViPipe, 0x3ED2, 0xFE96);
	ar0237_write_register(ViPipe, 0x3ED6, 0x34B3);
	ar0237_write_register(ViPipe, 0x3EEA, 0x2818);
	// #patch end ############################
	ar0237_write_register(ViPipe, 0x301A, 0x005C);
	delay_ms(33);

	printf("Aptina AR0237 sensor linear 2M-1080p 30fps init success!\n");

	return;
}

// INCLK=27M
void ar0237_wdr_1080p30_init(VI_PIPE ViPipe)
{
	// #[HiSPi Linear 1080p30 Low Power Mode - 4 Lane]
	// #Load = Reset
	ar0237_write_register(ViPipe, 0x301A, 0x0001);
	delay_ms(200);
	ar0237_write_register(ViPipe, 0x301A, 0x10D8);

	// #LOAD = Line_Interleaved_Sequencer
	// #[Line_Interleaved_Sequencer]
	ar0237_write_register(ViPipe, 0x3088, 0x816A);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x729B);
	ar0237_write_register(ViPipe, 0x3086, 0x4A31);
	ar0237_write_register(ViPipe, 0x3086, 0x4342);
	ar0237_write_register(ViPipe, 0x3086, 0x8E03);
	ar0237_write_register(ViPipe, 0x3086, 0x2A14);
	ar0237_write_register(ViPipe, 0x3086, 0x4578);
	ar0237_write_register(ViPipe, 0x3086, 0x7B3D);
	ar0237_write_register(ViPipe, 0x3086, 0xFF3D);
	ar0237_write_register(ViPipe, 0x3086, 0xFF3D);
	ar0237_write_register(ViPipe, 0x3086, 0xEA2A);
	ar0237_write_register(ViPipe, 0x3086, 0x043D);
	ar0237_write_register(ViPipe, 0x3086, 0x102A);
	ar0237_write_register(ViPipe, 0x3086, 0x052A);
	ar0237_write_register(ViPipe, 0x3086, 0x1535);
	ar0237_write_register(ViPipe, 0x3086, 0x2A05);
	ar0237_write_register(ViPipe, 0x3086, 0x3D10);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x2A04);
	ar0237_write_register(ViPipe, 0x3086, 0x2A14);
	ar0237_write_register(ViPipe, 0x3086, 0x3DFF);
	ar0237_write_register(ViPipe, 0x3086, 0x3DFF);
	ar0237_write_register(ViPipe, 0x3086, 0x3DEA);
	ar0237_write_register(ViPipe, 0x3086, 0x2A04);
	ar0237_write_register(ViPipe, 0x3086, 0x622A);
	ar0237_write_register(ViPipe, 0x3086, 0x288E);
	ar0237_write_register(ViPipe, 0x3086, 0x0036);
	ar0237_write_register(ViPipe, 0x3086, 0x2A08);
	ar0237_write_register(ViPipe, 0x3086, 0x3D64);
	ar0237_write_register(ViPipe, 0x3086, 0x7A3D);
	ar0237_write_register(ViPipe, 0x3086, 0x0444);
	ar0237_write_register(ViPipe, 0x3086, 0x2C4B);
	ar0237_write_register(ViPipe, 0x3086, 0x8F00);
	ar0237_write_register(ViPipe, 0x3086, 0x430C);
	ar0237_write_register(ViPipe, 0x3086, 0x2D63);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x2A90);
	ar0237_write_register(ViPipe, 0x3086, 0x3E06);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x168E);
	ar0237_write_register(ViPipe, 0x3086, 0x032A);
	ar0237_write_register(ViPipe, 0x3086, 0xFC5C);
	ar0237_write_register(ViPipe, 0x3086, 0x1D57);
	ar0237_write_register(ViPipe, 0x3086, 0x5449);
	ar0237_write_register(ViPipe, 0x3086, 0x5F53);
	ar0237_write_register(ViPipe, 0x3086, 0x0553);
	ar0237_write_register(ViPipe, 0x3086, 0x074D);
	ar0237_write_register(ViPipe, 0x3086, 0x2BF8);
	ar0237_write_register(ViPipe, 0x3086, 0x1016);
	ar0237_write_register(ViPipe, 0x3086, 0x4C08);
	ar0237_write_register(ViPipe, 0x3086, 0x5556);
	ar0237_write_register(ViPipe, 0x3086, 0x2BB8);
	ar0237_write_register(ViPipe, 0x3086, 0x2B98);
	ar0237_write_register(ViPipe, 0x3086, 0x4E11);
	ar0237_write_register(ViPipe, 0x3086, 0x2904);
	ar0237_write_register(ViPipe, 0x3086, 0x2984);
	ar0237_write_register(ViPipe, 0x3086, 0x2994);
	ar0237_write_register(ViPipe, 0x3086, 0x605C);
	ar0237_write_register(ViPipe, 0x3086, 0x195C);
	ar0237_write_register(ViPipe, 0x3086, 0x1B45);
	ar0237_write_register(ViPipe, 0x3086, 0x4845);
	ar0237_write_register(ViPipe, 0x3086, 0x0845);
	ar0237_write_register(ViPipe, 0x3086, 0x8829);
	ar0237_write_register(ViPipe, 0x3086, 0xB68E);
	ar0237_write_register(ViPipe, 0x3086, 0x012A);
	ar0237_write_register(ViPipe, 0x3086, 0xF83E);
	ar0237_write_register(ViPipe, 0x3086, 0x022A);
	ar0237_write_register(ViPipe, 0x3086, 0xFA3F);
	ar0237_write_register(ViPipe, 0x3086, 0x095C);
	ar0237_write_register(ViPipe, 0x3086, 0x1B29);
	ar0237_write_register(ViPipe, 0x3086, 0xB23F);
	ar0237_write_register(ViPipe, 0x3086, 0x0C3E);
	ar0237_write_register(ViPipe, 0x3086, 0x023E);
	ar0237_write_register(ViPipe, 0x3086, 0x135C);
	ar0237_write_register(ViPipe, 0x3086, 0x133F);
	ar0237_write_register(ViPipe, 0x3086, 0x113E);
	ar0237_write_register(ViPipe, 0x3086, 0x0B5F);
	ar0237_write_register(ViPipe, 0x3086, 0x2B90);
	ar0237_write_register(ViPipe, 0x3086, 0x2B80);
	ar0237_write_register(ViPipe, 0x3086, 0x3E06);
	ar0237_write_register(ViPipe, 0x3086, 0x162A);
	ar0237_write_register(ViPipe, 0x3086, 0xF23F);
	ar0237_write_register(ViPipe, 0x3086, 0x103E);
	ar0237_write_register(ViPipe, 0x3086, 0x0160);
	ar0237_write_register(ViPipe, 0x3086, 0x29A2);
	ar0237_write_register(ViPipe, 0x3086, 0x29A3);
	ar0237_write_register(ViPipe, 0x3086, 0x5F4D);
	ar0237_write_register(ViPipe, 0x3086, 0x192A);
	ar0237_write_register(ViPipe, 0x3086, 0xFA29);
	ar0237_write_register(ViPipe, 0x3086, 0x8345);
	ar0237_write_register(ViPipe, 0x3086, 0xA83E);
	ar0237_write_register(ViPipe, 0x3086, 0x072A);
	ar0237_write_register(ViPipe, 0x3086, 0xFB3E);
	ar0237_write_register(ViPipe, 0x3086, 0x2945);
	ar0237_write_register(ViPipe, 0x3086, 0x8821);
	ar0237_write_register(ViPipe, 0x3086, 0x3E08);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFA);
	ar0237_write_register(ViPipe, 0x3086, 0x5D29);
	ar0237_write_register(ViPipe, 0x3086, 0x9288);
	ar0237_write_register(ViPipe, 0x3086, 0x102B);
	ar0237_write_register(ViPipe, 0x3086, 0x048B);
	ar0237_write_register(ViPipe, 0x3086, 0x1685);
	ar0237_write_register(ViPipe, 0x3086, 0x8D48);
	ar0237_write_register(ViPipe, 0x3086, 0x4D4E);
	ar0237_write_register(ViPipe, 0x3086, 0x2B80);
	ar0237_write_register(ViPipe, 0x3086, 0x4C0B);
	ar0237_write_register(ViPipe, 0x3086, 0x3F2B);
	ar0237_write_register(ViPipe, 0x3086, 0x2AF2);
	ar0237_write_register(ViPipe, 0x3086, 0x3F10);
	ar0237_write_register(ViPipe, 0x3086, 0x3E01);
	ar0237_write_register(ViPipe, 0x3086, 0x6029);
	ar0237_write_register(ViPipe, 0x3086, 0x8229);
	ar0237_write_register(ViPipe, 0x3086, 0x8329);
	ar0237_write_register(ViPipe, 0x3086, 0x435C);
	ar0237_write_register(ViPipe, 0x3086, 0x155F);
	ar0237_write_register(ViPipe, 0x3086, 0x4D19);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFA);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x8E00);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x3F06);
	ar0237_write_register(ViPipe, 0x3086, 0x1244);
	ar0237_write_register(ViPipe, 0x3086, 0x4A04);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x0543);
	ar0237_write_register(ViPipe, 0x3086, 0x1658);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x5A43);
	ar0237_write_register(ViPipe, 0x3086, 0x1606);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x0743);
	ar0237_write_register(ViPipe, 0x3086, 0x168E);
	ar0237_write_register(ViPipe, 0x3086, 0x032A);
	ar0237_write_register(ViPipe, 0x3086, 0x9C45);
	ar0237_write_register(ViPipe, 0x3086, 0x787B);
	ar0237_write_register(ViPipe, 0x3086, 0x3F07);
	ar0237_write_register(ViPipe, 0x3086, 0x2A9D);
	ar0237_write_register(ViPipe, 0x3086, 0x3E2E);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x253E);
	ar0237_write_register(ViPipe, 0x3086, 0x068E);
	ar0237_write_register(ViPipe, 0x3086, 0x012A);
	ar0237_write_register(ViPipe, 0x3086, 0x988E);
	ar0237_write_register(ViPipe, 0x3086, 0x0012);
	ar0237_write_register(ViPipe, 0x3086, 0x444B);
	ar0237_write_register(ViPipe, 0x3086, 0x0343);
	ar0237_write_register(ViPipe, 0x3086, 0x2D46);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0xA343);
	ar0237_write_register(ViPipe, 0x3086, 0x165D);
	ar0237_write_register(ViPipe, 0x3086, 0x0D29);
	ar0237_write_register(ViPipe, 0x3086, 0x4488);
	ar0237_write_register(ViPipe, 0x3086, 0x102B);
	ar0237_write_register(ViPipe, 0x3086, 0x0453);
	ar0237_write_register(ViPipe, 0x3086, 0x0D8B);
	ar0237_write_register(ViPipe, 0x3086, 0x1685);
	ar0237_write_register(ViPipe, 0x3086, 0x448E);
	ar0237_write_register(ViPipe, 0x3086, 0x032A);
	ar0237_write_register(ViPipe, 0x3086, 0xFC5C);
	ar0237_write_register(ViPipe, 0x3086, 0x1D8D);
	ar0237_write_register(ViPipe, 0x3086, 0x6057);
	ar0237_write_register(ViPipe, 0x3086, 0x5417);
	ar0237_write_register(ViPipe, 0x3086, 0xFF17);
	ar0237_write_register(ViPipe, 0x3086, 0x4B2A);
	ar0237_write_register(ViPipe, 0x3086, 0xF43E);
	ar0237_write_register(ViPipe, 0x3086, 0x062A);
	ar0237_write_register(ViPipe, 0x3086, 0xFC49);
	ar0237_write_register(ViPipe, 0x3086, 0x5F53);
	ar0237_write_register(ViPipe, 0x3086, 0x0553);
	ar0237_write_register(ViPipe, 0x3086, 0x074D);
	ar0237_write_register(ViPipe, 0x3086, 0x2BF8);
	ar0237_write_register(ViPipe, 0x3086, 0x1016);
	ar0237_write_register(ViPipe, 0x3086, 0x4C08);
	ar0237_write_register(ViPipe, 0x3086, 0x5556);
	ar0237_write_register(ViPipe, 0x3086, 0x2BB8);
	ar0237_write_register(ViPipe, 0x3086, 0x2B98);
	ar0237_write_register(ViPipe, 0x3086, 0x4E11);
	ar0237_write_register(ViPipe, 0x3086, 0x2904);
	ar0237_write_register(ViPipe, 0x3086, 0x2984);
	ar0237_write_register(ViPipe, 0x3086, 0x2994);
	ar0237_write_register(ViPipe, 0x3086, 0x605C);
	ar0237_write_register(ViPipe, 0x3086, 0x195C);
	ar0237_write_register(ViPipe, 0x3086, 0x1B45);
	ar0237_write_register(ViPipe, 0x3086, 0x4845);
	ar0237_write_register(ViPipe, 0x3086, 0x0845);
	ar0237_write_register(ViPipe, 0x3086, 0x8829);
	ar0237_write_register(ViPipe, 0x3086, 0xB68E);
	ar0237_write_register(ViPipe, 0x3086, 0x012A);
	ar0237_write_register(ViPipe, 0x3086, 0xF83E);
	ar0237_write_register(ViPipe, 0x3086, 0x022A);
	ar0237_write_register(ViPipe, 0x3086, 0xFA3F);
	ar0237_write_register(ViPipe, 0x3086, 0x095C);
	ar0237_write_register(ViPipe, 0x3086, 0x1B29);
	ar0237_write_register(ViPipe, 0x3086, 0xB23F);
	ar0237_write_register(ViPipe, 0x3086, 0x0C3E);
	ar0237_write_register(ViPipe, 0x3086, 0x023E);
	ar0237_write_register(ViPipe, 0x3086, 0x135C);
	ar0237_write_register(ViPipe, 0x3086, 0x133F);
	ar0237_write_register(ViPipe, 0x3086, 0x113E);
	ar0237_write_register(ViPipe, 0x3086, 0x0B5F);
	ar0237_write_register(ViPipe, 0x3086, 0x2B90);
	ar0237_write_register(ViPipe, 0x3086, 0x2B80);
	ar0237_write_register(ViPipe, 0x3086, 0x3E10);
	ar0237_write_register(ViPipe, 0x3086, 0x2AF2);
	ar0237_write_register(ViPipe, 0x3086, 0x3F10);
	ar0237_write_register(ViPipe, 0x3086, 0x3E01);
	ar0237_write_register(ViPipe, 0x3086, 0x6029);
	ar0237_write_register(ViPipe, 0x3086, 0xA229);
	ar0237_write_register(ViPipe, 0x3086, 0xA35F);
	ar0237_write_register(ViPipe, 0x3086, 0x4D1C);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFA);
	ar0237_write_register(ViPipe, 0x3086, 0x2983);
	ar0237_write_register(ViPipe, 0x3086, 0x45A8);
	ar0237_write_register(ViPipe, 0x3086, 0x3E07);
	ar0237_write_register(ViPipe, 0x3086, 0x2AFB);
	ar0237_write_register(ViPipe, 0x3086, 0x3E29);
	ar0237_write_register(ViPipe, 0x3086, 0x4588);
	ar0237_write_register(ViPipe, 0x3086, 0x243E);
	ar0237_write_register(ViPipe, 0x3086, 0x082A);
	ar0237_write_register(ViPipe, 0x3086, 0xFA5D);
	ar0237_write_register(ViPipe, 0x3086, 0x2992);
	ar0237_write_register(ViPipe, 0x3086, 0x8810);
	ar0237_write_register(ViPipe, 0x3086, 0x2B04);
	ar0237_write_register(ViPipe, 0x3086, 0x8B16);
	ar0237_write_register(ViPipe, 0x3086, 0x868D);
	ar0237_write_register(ViPipe, 0x3086, 0x484D);
	ar0237_write_register(ViPipe, 0x3086, 0x4E2B);
	ar0237_write_register(ViPipe, 0x3086, 0x804C);
	ar0237_write_register(ViPipe, 0x3086, 0x0B3F);
	ar0237_write_register(ViPipe, 0x3086, 0x332A);
	ar0237_write_register(ViPipe, 0x3086, 0xF23F);
	ar0237_write_register(ViPipe, 0x3086, 0x103E);
	ar0237_write_register(ViPipe, 0x3086, 0x0160);
	ar0237_write_register(ViPipe, 0x3086, 0x2982);
	ar0237_write_register(ViPipe, 0x3086, 0x2983);
	ar0237_write_register(ViPipe, 0x3086, 0x2943);
	ar0237_write_register(ViPipe, 0x3086, 0x5C15);
	ar0237_write_register(ViPipe, 0x3086, 0x5F4D);
	ar0237_write_register(ViPipe, 0x3086, 0x1C2A);
	ar0237_write_register(ViPipe, 0x3086, 0xFA45);
	ar0237_write_register(ViPipe, 0x3086, 0x588E);
	ar0237_write_register(ViPipe, 0x3086, 0x002A);
	ar0237_write_register(ViPipe, 0x3086, 0x983F);
	ar0237_write_register(ViPipe, 0x3086, 0x064A);
	ar0237_write_register(ViPipe, 0x3086, 0x739D);
	ar0237_write_register(ViPipe, 0x3086, 0x0A43);
	ar0237_write_register(ViPipe, 0x3086, 0x160B);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x8E03);
	ar0237_write_register(ViPipe, 0x3086, 0x2A9C);
	ar0237_write_register(ViPipe, 0x3086, 0x4578);
	ar0237_write_register(ViPipe, 0x3086, 0x3F07);
	ar0237_write_register(ViPipe, 0x3086, 0x2A9D);
	ar0237_write_register(ViPipe, 0x3086, 0x3E12);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x3F04);
	ar0237_write_register(ViPipe, 0x3086, 0x8E01);
	ar0237_write_register(ViPipe, 0x3086, 0x2A98);
	ar0237_write_register(ViPipe, 0x3086, 0x8E00);
	ar0237_write_register(ViPipe, 0x3086, 0x9176);
	ar0237_write_register(ViPipe, 0x3086, 0x9C77);
	ar0237_write_register(ViPipe, 0x3086, 0x9C46);
	ar0237_write_register(ViPipe, 0x3086, 0x4416);
	ar0237_write_register(ViPipe, 0x3086, 0x1690);
	ar0237_write_register(ViPipe, 0x3086, 0x7A12);
	ar0237_write_register(ViPipe, 0x3086, 0x444B);
	ar0237_write_register(ViPipe, 0x3086, 0x4A00);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x6343);
	ar0237_write_register(ViPipe, 0x3086, 0x1608);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x5043);
	ar0237_write_register(ViPipe, 0x3086, 0x1665);
	ar0237_write_register(ViPipe, 0x3086, 0x4316);
	ar0237_write_register(ViPipe, 0x3086, 0x6643);
	ar0237_write_register(ViPipe, 0x3086, 0x168E);
	ar0237_write_register(ViPipe, 0x3086, 0x032A);
	ar0237_write_register(ViPipe, 0x3086, 0x9C45);
	ar0237_write_register(ViPipe, 0x3086, 0x783F);
	ar0237_write_register(ViPipe, 0x3086, 0x072A);
	ar0237_write_register(ViPipe, 0x3086, 0x9D5D);
	ar0237_write_register(ViPipe, 0x3086, 0x0C29);
	ar0237_write_register(ViPipe, 0x3086, 0x4488);
	ar0237_write_register(ViPipe, 0x3086, 0x102B);
	ar0237_write_register(ViPipe, 0x3086, 0x0453);
	ar0237_write_register(ViPipe, 0x3086, 0x0D8B);
	ar0237_write_register(ViPipe, 0x3086, 0x1686);
	ar0237_write_register(ViPipe, 0x3086, 0x3E1F);
	ar0237_write_register(ViPipe, 0x3086, 0x4558);
	ar0237_write_register(ViPipe, 0x3086, 0x283E);
	ar0237_write_register(ViPipe, 0x3086, 0x068E);
	ar0237_write_register(ViPipe, 0x3086, 0x012A);
	ar0237_write_register(ViPipe, 0x3086, 0x988E);
	ar0237_write_register(ViPipe, 0x3086, 0x008D);
	ar0237_write_register(ViPipe, 0x3086, 0x6012);
	ar0237_write_register(ViPipe, 0x3086, 0x444B);
	ar0237_write_register(ViPipe, 0x3086, 0xB92C);
	ar0237_write_register(ViPipe, 0x3086, 0x2C2C);

	// #LOAD= Default_Settings_12_Aug
	// #[Default_Settings_12_Aug]
	ar0237_write_register(ViPipe, 0x30F0, 0x1283);
	ar0237_write_register(ViPipe, 0x3064, 0x1802);
	ar0237_write_register(ViPipe, 0x3EEE, 0xA0AA);
	ar0237_write_register(ViPipe, 0x30BA, 0x762C);
	ar0237_write_register(ViPipe, 0x3FA4, 0x0F70);
	ar0237_write_register(ViPipe, 0x309E, 0x016A);
	ar0237_write_register(ViPipe, 0x3096, 0xF880);
	ar0237_write_register(ViPipe, 0x3F32, 0xF880);
	ar0237_write_register(ViPipe, 0x3092, 0x006F);
	// ##
	ar0237_write_register(ViPipe, 0x301A, 0x0058);
	ar0237_write_register(ViPipe, 0x30B0, 0x0118);

	ar0237_write_register(ViPipe, 0x31AC, 0x0C0C);
	ar0237_write_register(ViPipe, 0x31C0, 0x0000);

	// #LOAD= PLL_settings - 4 Lane 12-bit HiSPi
	// #[PLL_settings - 4 Lane 12-bit HiSPi]
	ar0237_write_register(ViPipe, 0x302A, 0x0006);
	ar0237_write_register(ViPipe, 0x302C, 0x0001);
	ar0237_write_register(ViPipe, 0x302E, 0x0004);
	ar0237_write_register(ViPipe, 0x3030, 0x0042);
	ar0237_write_register(ViPipe, 0x3036, 0x000C);
	ar0237_write_register(ViPipe, 0x3038, 0x0001);
	// ##
	ar0237_write_register(ViPipe, 0x3002, 0x0000);
	ar0237_write_register(ViPipe, 0x3004, 0x0000);
	ar0237_write_register(ViPipe, 0x3006, 0x0437);
	ar0237_write_register(ViPipe, 0x3008, 0x0787);
	ar0237_write_register(ViPipe, 0x300A, 0x04C4);
	ar0237_write_register(ViPipe, 0x300C, 0x07F0);
	ar0237_write_register(ViPipe, 0x3012, 0x007d); // LTime
	ar0237_write_register(ViPipe, 0x30A2, 0x0001);
	ar0237_write_register(ViPipe, 0x30A6, 0x0001);
	ar0237_write_register(ViPipe, 0x30AE, 0x0001);
	ar0237_write_register(ViPipe, 0x30A8, 0x0001);
	ar0237_write_register(ViPipe, 0x3040, 0x0000);

	// #LOAD= HDR Mode 16x Setup
	// #[HDR Mode 16x Setup]
	ar0237_write_register(ViPipe, 0x3082, 0x0008);
	//     #LOAD= HDR Mode Setup
	//     #LOAD= HDR Mode Low Conversion Gain
	//  #LOAD= 1.5x analog Gain
	ar0237_write_register(ViPipe, 0x3060, 0x000f);
	// ##
	ar0237_write_register(ViPipe, 0x3096, 0x0480);
	ar0237_write_register(ViPipe, 0x3098, 0x0480);
	ar0237_write_register(ViPipe, 0x3100, 0x0004);

	ar0237_write_register(ViPipe, 0x30BA, 0x772c);

	ar0237_write_register(ViPipe, 0x31AE, 0x0304);
	ar0237_write_register(ViPipe, 0x31C6, 0x0400);
	ar0237_write_register(ViPipe, 0x306E, 0x9218);

	// #LOAD= Disable Embedded Data and Stats
	ar0237_write_register(ViPipe, 0x318E, 0x1200);
	// #patch start###########################
	ar0237_write_register(ViPipe, 0x3ECC, 0x6E2D);
	ar0237_write_register(ViPipe, 0x3ED2, 0xFE96);
	ar0237_write_register(ViPipe, 0x3ED6, 0x34B3);
	ar0237_write_register(ViPipe, 0x3EEA, 0x2818);
	// #patch end ############################

	ar0237_write_register(ViPipe, 0x301A, 0x005C);
	ar0237_write_register(ViPipe, 0x31D0, 0x0000);

	ar0237_write_register(ViPipe, 0x3100,
			      0x0004); // T1&T2 use same AGain Reg
	ar0237_write_register(ViPipe, 0x3060, 0x0004); // Again
	ar0237_write_register(ViPipe, 0x312C, 0x0030);
	ar0237_write_register(ViPipe, 0x3082, 0x0808);
	ar0237_write_register(ViPipe, 0x3212, 0x007d); // STime
	delay_ms(33);

	printf("Aptina AR0237 sensor wdr 2M-1080p 30fps init success!\n");
	return;
}
