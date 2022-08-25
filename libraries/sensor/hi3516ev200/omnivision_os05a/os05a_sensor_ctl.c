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

const unsigned char os05a_i2c_addr = 0x6c; /* I2C Address */
const unsigned int os05a_addr_byte = 2;
const unsigned int os05a_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = -1 };

extern ISP_SNS_STATE_S *g_pastOs05a[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunOs05aBusInfo[];

#define Os05a_5M_2688x1944_12bit_linear30 (0)
#define Os05a_5M_2688x1944_10bit_wdr30 (1)

/* VI 297M */
void os05a_init_5M_2688x1944_10bit_wdr30(VI_PIPE ViPipe);
/* VI 250M */
void os05a_init_5M_2688x1944_12bit_linear30(VI_PIPE ViPipe);

int os05a_i2c_init(VI_PIPE ViPipe)
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

	u8DevNum = g_aunOs05aBusInfo[ViPipe].s8I2cDev;
	snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

	g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

	if (g_fd[ViPipe] < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n",
			  u8DevNum);
		return GK_FAILURE;
	}

	ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (os05a_i2c_addr >> 1));
	if (ret < 0) {
		ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return ret;
	}
#endif

	return GK_SUCCESS;
}

int os05a_i2c_exit(VI_PIPE ViPipe)
{
	if (g_fd[ViPipe] >= 0) {
		close(g_fd[ViPipe]);
		g_fd[ViPipe] = -1;
		return GK_SUCCESS;
	}
	return GK_FAILURE;
}

int os05a_read_register(VI_PIPE ViPipe, int addr)
{
	return GK_SUCCESS;
}

int os05a_write_register(VI_PIPE ViPipe, int addr, int data)
{
	if (g_fd[ViPipe] < 0) {
		return GK_SUCCESS;
	}

#ifdef GPIO_I2C
	i2c_data.dev_addr = os05a_i2c_addr;
	i2c_data.reg_addr = addr;
	i2c_data.addr_byte_num = os05a_addr_byte;
	i2c_data.data = data;
	i2c_data.data_byte_num = os05a_data_byte;

	ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

	if (ret) {
		ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
		return ret;
	}
#else
	int idx = 0;
	int ret;
	char buf[8];

	if (os05a_addr_byte == 2) {
		buf[idx] = (addr >> 8) & 0xff;
		idx++;
		buf[idx] = addr & 0xff;
		idx++;
	} else {
	}

	if (os05a_data_byte == 2) {
	} else {
		buf[idx] = data & 0xff;
		idx++;
	}

	ret = write(g_fd[ViPipe], buf, (os05a_addr_byte + os05a_data_byte));
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

void os05a_standby(VI_PIPE ViPipe)
{
	return;
}

void os05a_restart(VI_PIPE ViPipe)
{
	return;
}

void os05a_prog(VI_PIPE ViPipe, int *rom)
{
	int i = 0;
	while (1) {
		int lookup = rom[i++];
		int addr = (lookup >> 16) & 0xFFFF;
		int data = lookup & 0xFFFF;
		if (addr == 0xFFFE) {
			delay_ms(data);
		} else if (addr == 0xFFFF) {
			return;
		} else {
			os05a_write_register(ViPipe, addr, data);
		}
	}
}

void os05a_default_reg_init(VI_PIPE ViPipe)
{
	GK_U32 i;

	for (i = 0; i < g_pastOs05a[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
		os05a_write_register(ViPipe,
				     g_pastOs05a[ViPipe]
					     ->astRegsInfo[0]
					     .astI2cData[i]
					     .u32RegAddr,
				     g_pastOs05a[ViPipe]
					     ->astRegsInfo[0]
					     .astI2cData[i]
					     .u32Data);
	}
}

void os05a_init(VI_PIPE ViPipe)
{
	GK_BOOL bInit;
	GK_U8 u8ImgMode;
	bInit = g_pastOs05a[ViPipe]->bInit;
	u8ImgMode = g_pastOs05a[ViPipe]->u8ImgMode;

	os05a_i2c_init(ViPipe);

	/* When sensor first init, config all registers */
	if (bInit == GK_FALSE) {
		switch (u8ImgMode) {
		case Os05a_5M_2688x1944_12bit_linear30:
			os05a_init_5M_2688x1944_12bit_linear30(ViPipe);
			break;

		case Os05a_5M_2688x1944_10bit_wdr30:
			os05a_init_5M_2688x1944_10bit_wdr30(ViPipe);
			break;

		default:
			ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n",
				  u8ImgMode);
			break;
		}

	}
	/* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
	else {
		switch (u8ImgMode) {
		case Os05a_5M_2688x1944_12bit_linear30:
			os05a_init_5M_2688x1944_12bit_linear30(ViPipe);
			break;

		case Os05a_5M_2688x1944_10bit_wdr30:
			os05a_init_5M_2688x1944_10bit_wdr30(ViPipe);
			break;

		default:
			ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n",
				  u8ImgMode);
			break;
		}
	}

	g_pastOs05a[ViPipe]->bInit = GK_TRUE;

	return;
}

void os05a_exit(VI_PIPE ViPipe)
{
	os05a_i2c_exit(ViPipe);

	return;
}

void os05a_init_5M_2688x1944_10bit_wdr30(VI_PIPE ViPipe)
{
	os05a_write_register(ViPipe, 0x4600, 0x01);
	os05a_write_register(ViPipe, 0x4601, 0x04);
	os05a_write_register(ViPipe, 0x4603, 0x00);
	os05a_write_register(ViPipe, 0x0103, 0x01);
	os05a_write_register(ViPipe, 0x0303, 0x01);
	os05a_write_register(ViPipe, 0x0305, 0x5a);
	os05a_write_register(ViPipe, 0x0306, 0x00);
	os05a_write_register(ViPipe, 0x0307, 0x00);
	os05a_write_register(ViPipe, 0x0308, 0x03);
	os05a_write_register(ViPipe, 0x0309, 0x04);
	os05a_write_register(ViPipe, 0x032a, 0x00);
	os05a_write_register(ViPipe, 0x031e, 0x09);
	os05a_write_register(ViPipe, 0x0325, 0x48);
	os05a_write_register(ViPipe, 0x0328, 0x07);
	os05a_write_register(ViPipe, 0x300d, 0x11);
	os05a_write_register(ViPipe, 0x300e, 0x11);
	os05a_write_register(ViPipe, 0x300f, 0x11);
	os05a_write_register(ViPipe, 0x3026, 0x00); // update;
	os05a_write_register(ViPipe, 0x3027, 0x00); // update;
	os05a_write_register(ViPipe, 0x3010, 0x01);
	os05a_write_register(ViPipe, 0x3012, 0x41);
	os05a_write_register(ViPipe, 0x3016, 0xf0);
	os05a_write_register(ViPipe, 0x3018, 0xf0);
	os05a_write_register(ViPipe, 0x3028, 0xf0);
	os05a_write_register(ViPipe, 0x301e, 0x98);
	os05a_write_register(ViPipe, 0x3010, 0x01); // update; 0x04
	os05a_write_register(ViPipe, 0x3011, 0x04); // update; 0x06
	os05a_write_register(ViPipe, 0x3031, 0xa9);
	os05a_write_register(ViPipe, 0x3103, 0x48);
	os05a_write_register(ViPipe, 0x3104, 0x01);
	os05a_write_register(ViPipe, 0x3106, 0x10);
	os05a_write_register(ViPipe, 0x3501, 0x09);
	os05a_write_register(ViPipe, 0x3502, 0xa0);
	os05a_write_register(ViPipe, 0x3505, 0x83);
	os05a_write_register(ViPipe, 0x3508, 0x03);
	os05a_write_register(ViPipe, 0x3509, 0x00);
	os05a_write_register(ViPipe, 0x350a, 0x04);
	os05a_write_register(ViPipe, 0x350b, 0x00);
	os05a_write_register(ViPipe, 0x350c, 0x03);
	os05a_write_register(ViPipe, 0x350d, 0x00);
	os05a_write_register(ViPipe, 0x350e, 0x04);
	os05a_write_register(ViPipe, 0x350f, 0x00);
	os05a_write_register(ViPipe, 0x3600, 0x00);
	os05a_write_register(ViPipe, 0x3626, 0xff);
	os05a_write_register(ViPipe, 0x3605, 0x50);
	os05a_write_register(ViPipe, 0x3609, 0xb5);
	os05a_write_register(ViPipe, 0x3610, 0x69);
	os05a_write_register(ViPipe, 0x360c, 0x01);
	os05a_write_register(ViPipe, 0x3628, 0xa4);
	os05a_write_register(ViPipe, 0x3629, 0x6a);
	os05a_write_register(ViPipe, 0x362d, 0x10);
	os05a_write_register(ViPipe, 0x36c0, 0x00); // update;
	os05a_write_register(ViPipe, 0x3660, 0x42);
	os05a_write_register(ViPipe, 0x3661, 0x07);
	os05a_write_register(ViPipe, 0x3662, 0x00);
	os05a_write_register(ViPipe, 0x3663, 0x28);
	os05a_write_register(ViPipe, 0x3664, 0x0d);
	os05a_write_register(ViPipe, 0x366a, 0x38);
	os05a_write_register(ViPipe, 0x366b, 0xa0);
	os05a_write_register(ViPipe, 0x366d, 0x00);
	os05a_write_register(ViPipe, 0x366e, 0x00);
	os05a_write_register(ViPipe, 0x3680, 0x00);
	os05a_write_register(ViPipe, 0x3621, 0x81);
	os05a_write_register(ViPipe, 0x3634, 0x31);
	os05a_write_register(ViPipe, 0x3620, 0x00);
	os05a_write_register(ViPipe, 0x3622, 0x00);
	os05a_write_register(ViPipe, 0x362a, 0xd0);
	os05a_write_register(ViPipe, 0x362e, 0x8c);
	os05a_write_register(ViPipe, 0x362f, 0x98);
	os05a_write_register(ViPipe, 0x3630, 0xb0);
	os05a_write_register(ViPipe, 0x3631, 0xd7);
	os05a_write_register(ViPipe, 0x3701, 0x0f);
	os05a_write_register(ViPipe, 0x3737, 0x02);
	os05a_write_register(ViPipe, 0x3740, 0x18); // update;
	os05a_write_register(ViPipe, 0x3741, 0x04);
	os05a_write_register(ViPipe, 0x373c, 0x0f);
	os05a_write_register(ViPipe, 0x373b, 0x02);
	os05a_write_register(ViPipe, 0x3705, 0x00);
	os05a_write_register(ViPipe, 0x3706, 0x50);
	os05a_write_register(ViPipe, 0x370a, 0x00);
	os05a_write_register(ViPipe, 0x370b, 0xe4);
	os05a_write_register(ViPipe, 0x3709, 0x4a);
	os05a_write_register(ViPipe, 0x3714, 0x21);
	os05a_write_register(ViPipe, 0x371c, 0x00);
	os05a_write_register(ViPipe, 0x371d, 0x08);
	os05a_write_register(ViPipe, 0x375e, 0x0e); // update; 0x0b
	os05a_write_register(ViPipe, 0x3760, 0x13); // update;
	os05a_write_register(ViPipe, 0x3776, 0x10);
	os05a_write_register(ViPipe, 0x3781, 0x02);
	os05a_write_register(ViPipe, 0x3782, 0x04);
	os05a_write_register(ViPipe, 0x3783, 0x02);
	os05a_write_register(ViPipe, 0x3784, 0x08);
	os05a_write_register(ViPipe, 0x3785, 0x08);
	os05a_write_register(ViPipe, 0x3788, 0x01);
	os05a_write_register(ViPipe, 0x3789, 0x01);
	os05a_write_register(ViPipe, 0x3797, 0x04);
	os05a_write_register(ViPipe, 0x3798, 0x01); // update;
	os05a_write_register(ViPipe, 0x3799, 0x00); // update;
	os05a_write_register(ViPipe, 0x3761, 0x02);
	os05a_write_register(ViPipe, 0x3762, 0x0d);
	os05a_write_register(ViPipe, 0x3800, 0x00);
	os05a_write_register(ViPipe, 0x3801, 0x00);
	os05a_write_register(ViPipe, 0x3802, 0x00);
	os05a_write_register(ViPipe, 0x3803, 0x0c);
	os05a_write_register(ViPipe, 0x3804, 0x0e);
	os05a_write_register(ViPipe, 0x3805, 0xff);
	os05a_write_register(ViPipe, 0x3806, 0x08);
	os05a_write_register(ViPipe, 0x3807, 0x6f);
	os05a_write_register(ViPipe, 0x3808, 0x0a);
	os05a_write_register(ViPipe, 0x3809, 0x80);
	os05a_write_register(ViPipe, 0x380a, 0x07);
	os05a_write_register(ViPipe, 0x380b, 0x98);
	os05a_write_register(ViPipe, 0x380c, 0x02);
	os05a_write_register(ViPipe, 0x380d, 0xd0);
	os05a_write_register(ViPipe, 0x380e, 0x09);
	os05a_write_register(ViPipe, 0x380f, 0xc0);
	os05a_write_register(ViPipe, 0x3811, 0x10); // update;
	os05a_write_register(ViPipe, 0x3813, 0x04);
	os05a_write_register(ViPipe, 0x3814, 0x01);
	os05a_write_register(ViPipe, 0x3815, 0x01);
	os05a_write_register(ViPipe, 0x3816, 0x01);
	os05a_write_register(ViPipe, 0x3817, 0x01);
	os05a_write_register(ViPipe, 0x381c, 0x08);
	os05a_write_register(ViPipe, 0x3820, 0x00);
	os05a_write_register(ViPipe, 0x3821, 0x24);
	os05a_write_register(ViPipe, 0x3823, 0x08); // update; 0x18
	os05a_write_register(ViPipe, 0x3826, 0x00);
	os05a_write_register(ViPipe, 0x3827, 0x01);
	os05a_write_register(ViPipe, 0x3833, 0x01); // update;
	os05a_write_register(ViPipe, 0x3832, 0x02);
	os05a_write_register(ViPipe, 0x383c, 0x48);
	os05a_write_register(ViPipe, 0x383d, 0xff);
	os05a_write_register(ViPipe, 0x3843, 0x20);
	os05a_write_register(ViPipe, 0x382d, 0x08);
	os05a_write_register(ViPipe, 0x3d85, 0x0b);
	os05a_write_register(ViPipe, 0x3d84, 0x40);
	os05a_write_register(ViPipe, 0x3d8c, 0x63);
	os05a_write_register(ViPipe, 0x3d8d, 0x00);
	os05a_write_register(ViPipe, 0x4000, 0x78);
	os05a_write_register(ViPipe, 0x4001, 0x2b);
	os05a_write_register(ViPipe, 0x4004, 0x00); // update;
	os05a_write_register(ViPipe, 0x4005, 0x40);
	os05a_write_register(ViPipe, 0x4028, 0x2f);
	os05a_write_register(ViPipe, 0x400a, 0x01);
	os05a_write_register(ViPipe, 0x4010, 0x12);
	os05a_write_register(ViPipe, 0x4008, 0x02);
	os05a_write_register(ViPipe, 0x4009, 0x0d);
	os05a_write_register(ViPipe, 0x401a, 0x58);
	os05a_write_register(ViPipe, 0x4050, 0x00);
	os05a_write_register(ViPipe, 0x4051, 0x01);
	os05a_write_register(ViPipe, 0x4052, 0x00);
	os05a_write_register(ViPipe, 0x4053, 0x80);
	os05a_write_register(ViPipe, 0x4054, 0x00);
	os05a_write_register(ViPipe, 0x4055, 0x80);
	os05a_write_register(ViPipe, 0x4056, 0x00);
	os05a_write_register(ViPipe, 0x4057, 0x80);
	os05a_write_register(ViPipe, 0x4058, 0x00);
	os05a_write_register(ViPipe, 0x4059, 0x80);
	os05a_write_register(ViPipe, 0x430b, 0xff);
	os05a_write_register(ViPipe, 0x430c, 0xff);
	os05a_write_register(ViPipe, 0x430d, 0x00);
	os05a_write_register(ViPipe, 0x430e, 0x00);
	os05a_write_register(ViPipe, 0x4501, 0x18);
	os05a_write_register(ViPipe, 0x4502, 0x00);
	os05a_write_register(ViPipe, 0x4643, 0x00);
	os05a_write_register(ViPipe, 0x4640, 0x01);
	os05a_write_register(ViPipe, 0x4641, 0x04);
	os05a_write_register(ViPipe, 0x480e, 0x04);
	os05a_write_register(ViPipe, 0x4813, 0x98);
	os05a_write_register(ViPipe, 0x4815, 0x2b);
	os05a_write_register(ViPipe, 0x486e, 0x36);
	os05a_write_register(ViPipe, 0x486f, 0x84);
	os05a_write_register(ViPipe, 0x4860, 0x00);
	os05a_write_register(ViPipe, 0x4861, 0xa0);
	os05a_write_register(ViPipe, 0x484b, 0x05);
	os05a_write_register(ViPipe, 0x4850, 0x00);
	os05a_write_register(ViPipe, 0x4851, 0xaa);
	os05a_write_register(ViPipe, 0x4852, 0xff);
	os05a_write_register(ViPipe, 0x4853, 0x8a);
	os05a_write_register(ViPipe, 0x4854, 0x08);
	os05a_write_register(ViPipe, 0x4855, 0x30);
	os05a_write_register(ViPipe, 0x4800, 0x60); // update; 0x00
	os05a_write_register(ViPipe, 0x4837, 0x0b);
	os05a_write_register(ViPipe, 0x484a, 0x3f); // update;
	os05a_write_register(ViPipe, 0x5000, 0xc9);
	os05a_write_register(ViPipe, 0x5001, 0x43);
	os05a_write_register(ViPipe, 0x5002, 0x00); // update;
	os05a_write_register(ViPipe, 0x5211, 0x03);
	os05a_write_register(ViPipe, 0x5291, 0x03);
	os05a_write_register(ViPipe, 0x520d, 0x0f);
	os05a_write_register(ViPipe, 0x520e, 0xfd);
	os05a_write_register(ViPipe, 0x520f, 0xa5);
	os05a_write_register(ViPipe, 0x5210, 0xa5);
	os05a_write_register(ViPipe, 0x528d, 0x0f);
	os05a_write_register(ViPipe, 0x528e, 0xfd);
	os05a_write_register(ViPipe, 0x528f, 0xa5);
	os05a_write_register(ViPipe, 0x5290, 0xa5);
	os05a_write_register(ViPipe, 0x5004, 0x40);
	os05a_write_register(ViPipe, 0x5005, 0x00);
	os05a_write_register(ViPipe, 0x5180, 0x00);
	os05a_write_register(ViPipe, 0x5181, 0x10);
	os05a_write_register(ViPipe, 0x5182, 0x0f);
	os05a_write_register(ViPipe, 0x5183, 0xff);
	os05a_write_register(ViPipe, 0x580b, 0x03);

	os05a_write_register(ViPipe, 0x4d00, 0x03);
	os05a_write_register(ViPipe, 0x4d01, 0xe9);
	os05a_write_register(ViPipe, 0x4d02, 0xba);
	os05a_write_register(ViPipe, 0x4d03, 0x66);
	os05a_write_register(ViPipe, 0x4d04, 0x46);
	os05a_write_register(ViPipe, 0x4d05, 0xa5);
	os05a_write_register(ViPipe, 0x3603, 0x3c);
	os05a_write_register(ViPipe, 0x3703, 0x26);
	os05a_write_register(ViPipe, 0x3709, 0x49);

	os05a_write_register(ViPipe, 0x3708, 0x2d); // update;
	os05a_write_register(ViPipe, 0x3719, 0x1c); // update;
	os05a_write_register(ViPipe, 0x371a, 0x06); // update;

	os05a_write_register(ViPipe, 0x4000, 0x79);
	os05a_write_register(ViPipe, 0x380c, 0x02);
	os05a_write_register(ViPipe, 0x380d, 0xd0);
	os05a_write_register(ViPipe, 0x380e, 0x09);
	os05a_write_register(ViPipe, 0x380f, 0xc0);
	os05a_write_register(ViPipe, 0x3501, 0x09);
	os05a_write_register(ViPipe, 0x3502, 0x98);
	os05a_write_register(ViPipe, 0x3511, 0x00);
	os05a_write_register(ViPipe, 0x3512, 0x34);

	os05a_default_reg_init(ViPipe);
	os05a_write_register(ViPipe, 0x0100, 0x01);

	os05a_write_register(ViPipe, 0x0100, 0x00);

	os05a_write_register(ViPipe, 0x0100, 0x01);
	printf("-------OV os05a_init_5M_2688x1944_10bit_wdr30 Initial OK!-------\n");
}

void os05a_init_5M_2688x1944_12bit_linear30(VI_PIPE ViPipe)
{
	os05a_write_register(ViPipe, 0x4600, 0x00);
	os05a_write_register(ViPipe, 0x4601, 0x10);
	os05a_write_register(ViPipe, 0x4603, 0x01);

	os05a_write_register(ViPipe, 0x0103, 0x01);
	os05a_write_register(ViPipe, 0x0303, 0x01);
	os05a_write_register(ViPipe, 0x0305, 0x27);
	os05a_write_register(ViPipe, 0x0306, 0x00);
	os05a_write_register(ViPipe, 0x0307, 0x00);
	os05a_write_register(ViPipe, 0x0308, 0x03);
	os05a_write_register(ViPipe, 0x0309, 0x04);
	os05a_write_register(ViPipe, 0x032a, 0x00);
	os05a_write_register(ViPipe, 0x031e, 0x0a);
	os05a_write_register(ViPipe, 0x0325, 0x48);
	os05a_write_register(ViPipe, 0x0328, 0x07);
	os05a_write_register(ViPipe, 0x300d, 0x11);
	os05a_write_register(ViPipe, 0x300e, 0x11);
	os05a_write_register(ViPipe, 0x300f, 0x11);
	os05a_write_register(ViPipe, 0x3010, 0x01);
	os05a_write_register(ViPipe, 0x3012, 0x41);
	os05a_write_register(ViPipe, 0x3016, 0xf0);
	os05a_write_register(ViPipe, 0x3018, 0xf0);
	os05a_write_register(ViPipe, 0x3028, 0xf0);
	os05a_write_register(ViPipe, 0x301e, 0x98);
	os05a_write_register(ViPipe, 0x3010, 0x04);
	os05a_write_register(ViPipe, 0x3011, 0x06);
	os05a_write_register(ViPipe, 0x3031, 0xa9);
	os05a_write_register(ViPipe, 0x3103, 0x48);
	os05a_write_register(ViPipe, 0x3104, 0x01);
	os05a_write_register(ViPipe, 0x3106, 0x10);
	os05a_write_register(ViPipe, 0x3400, 0x04);
	os05a_write_register(ViPipe, 0x3025, 0x03);
	os05a_write_register(ViPipe, 0x3425, 0x51);
	os05a_write_register(ViPipe, 0x3428, 0x01);
	os05a_write_register(ViPipe, 0x3406, 0x08);
	os05a_write_register(ViPipe, 0x3408, 0x03);
	os05a_write_register(ViPipe, 0x3501, 0x08);
	os05a_write_register(ViPipe, 0x3502, 0x6f);
	os05a_write_register(ViPipe, 0x3505, 0x83);
	os05a_write_register(ViPipe, 0x3508, 0x00);
	os05a_write_register(ViPipe, 0x3509, 0x80);
	os05a_write_register(ViPipe, 0x350a, 0x04);
	os05a_write_register(ViPipe, 0x350b, 0x00);
	os05a_write_register(ViPipe, 0x350c, 0x00);
	os05a_write_register(ViPipe, 0x350d, 0x80);
	os05a_write_register(ViPipe, 0x350e, 0x04);
	os05a_write_register(ViPipe, 0x350f, 0x00);
	os05a_write_register(ViPipe, 0x3600, 0x00);
	os05a_write_register(ViPipe, 0x3626, 0xff);
	os05a_write_register(ViPipe, 0x3605, 0x50);
	os05a_write_register(ViPipe, 0x3609, 0xdb);
	os05a_write_register(ViPipe, 0x3610, 0x69);
	os05a_write_register(ViPipe, 0x360c, 0x01);
	os05a_write_register(ViPipe, 0x3628, 0xa4);
	os05a_write_register(ViPipe, 0x3629, 0x6a);
	os05a_write_register(ViPipe, 0x362d, 0x10);
	os05a_write_register(ViPipe, 0x3660, 0xd3);
	os05a_write_register(ViPipe, 0x3661, 0x06);
	os05a_write_register(ViPipe, 0x3662, 0x00);
	os05a_write_register(ViPipe, 0x3663, 0x28);
	os05a_write_register(ViPipe, 0x3664, 0x0d);
	os05a_write_register(ViPipe, 0x366a, 0x38);
	os05a_write_register(ViPipe, 0x366b, 0xa0);
	os05a_write_register(ViPipe, 0x366d, 0x00);
	os05a_write_register(ViPipe, 0x366e, 0x00);
	os05a_write_register(ViPipe, 0x3680, 0x00);
	os05a_write_register(ViPipe, 0x3621, 0x81);
	os05a_write_register(ViPipe, 0x3634, 0x31);
	os05a_write_register(ViPipe, 0x3620, 0x00);
	os05a_write_register(ViPipe, 0x3622, 0x00);
	os05a_write_register(ViPipe, 0x362a, 0xd0);
	os05a_write_register(ViPipe, 0x362e, 0x8c);
	os05a_write_register(ViPipe, 0x362f, 0x98);
	os05a_write_register(ViPipe, 0x3630, 0xb0);
	os05a_write_register(ViPipe, 0x3631, 0xd7);
	os05a_write_register(ViPipe, 0x3701, 0x0f);
	os05a_write_register(ViPipe, 0x3737, 0x02);
	os05a_write_register(ViPipe, 0x3741, 0x04);
	os05a_write_register(ViPipe, 0x373c, 0x0f);
	os05a_write_register(ViPipe, 0x373b, 0x02);
	os05a_write_register(ViPipe, 0x3705, 0x00);
	os05a_write_register(ViPipe, 0x3706, 0xa0);
	os05a_write_register(ViPipe, 0x370a, 0x01);
	os05a_write_register(ViPipe, 0x370b, 0xc8);
	os05a_write_register(ViPipe, 0x3709, 0x4a);
	os05a_write_register(ViPipe, 0x3714, 0x21);
	os05a_write_register(ViPipe, 0x371c, 0x00);
	os05a_write_register(ViPipe, 0x371d, 0x08);
	os05a_write_register(ViPipe, 0x375e, 0x0b);
	os05a_write_register(ViPipe, 0x3776, 0x10);
	os05a_write_register(ViPipe, 0x3781, 0x02);
	os05a_write_register(ViPipe, 0x3782, 0x04);
	os05a_write_register(ViPipe, 0x3783, 0x02);
	os05a_write_register(ViPipe, 0x3784, 0x08);
	os05a_write_register(ViPipe, 0x3785, 0x08);
	os05a_write_register(ViPipe, 0x3788, 0x01);
	os05a_write_register(ViPipe, 0x3789, 0x01);
	os05a_write_register(ViPipe, 0x3797, 0x04);
	os05a_write_register(ViPipe, 0x3761, 0x02);
	os05a_write_register(ViPipe, 0x3762, 0x0d);
	os05a_write_register(ViPipe, 0x3800, 0x00);
	os05a_write_register(ViPipe, 0x3801, 0x00);
	os05a_write_register(ViPipe, 0x3802, 0x00);
	os05a_write_register(ViPipe, 0x3803, 0x0c);
	os05a_write_register(ViPipe, 0x3804, 0x0e);
	os05a_write_register(ViPipe, 0x3805, 0xff);
	os05a_write_register(ViPipe, 0x3806, 0x08);
	os05a_write_register(ViPipe, 0x3807, 0x6f);
	os05a_write_register(ViPipe, 0x3808, 0x0a);
	os05a_write_register(ViPipe, 0x3809, 0x80);
	os05a_write_register(ViPipe, 0x380a, 0x07);
	os05a_write_register(ViPipe, 0x380b, 0x98);
	os05a_write_register(ViPipe, 0x380c, 0x04);
	os05a_write_register(ViPipe, 0x380d, 0xd0);
	os05a_write_register(ViPipe, 0x380e, 0x08);
	os05a_write_register(ViPipe, 0x380f, 0x8f);
	os05a_write_register(ViPipe, 0x3813, 0x04);
	os05a_write_register(ViPipe, 0x3814, 0x01);
	os05a_write_register(ViPipe, 0x3815, 0x01);
	os05a_write_register(ViPipe, 0x3816, 0x01);
	os05a_write_register(ViPipe, 0x3817, 0x01);
	os05a_write_register(ViPipe, 0x381c, 0x00);
	os05a_write_register(ViPipe, 0x3820, 0x00);
	os05a_write_register(ViPipe, 0x3821, 0x04);
	os05a_write_register(ViPipe, 0x3823, 0x18);
	os05a_write_register(ViPipe, 0x3826, 0x00);
	os05a_write_register(ViPipe, 0x3827, 0x01);
	os05a_write_register(ViPipe, 0x3832, 0x02);
	os05a_write_register(ViPipe, 0x383c, 0x48);
	os05a_write_register(ViPipe, 0x383d, 0xff);
	os05a_write_register(ViPipe, 0x3843, 0x20);
	os05a_write_register(ViPipe, 0x382d, 0x08);
	os05a_write_register(ViPipe, 0x3d85, 0x0b);
	os05a_write_register(ViPipe, 0x3d84, 0x40);
	os05a_write_register(ViPipe, 0x3d8c, 0x63);
	os05a_write_register(ViPipe, 0x3d8d, 0x00);
	os05a_write_register(ViPipe, 0x4000, 0x78);
	os05a_write_register(ViPipe, 0x4001, 0x2b);
	os05a_write_register(ViPipe, 0x4005, 0x40);
	os05a_write_register(ViPipe, 0x4028, 0x2f);
	os05a_write_register(ViPipe, 0x400a, 0x01);
	os05a_write_register(ViPipe, 0x4010, 0x12);
	os05a_write_register(ViPipe, 0x4008, 0x02);
	os05a_write_register(ViPipe, 0x4009, 0x0d);
	os05a_write_register(ViPipe, 0x401a, 0x58);
	os05a_write_register(ViPipe, 0x4050, 0x00);
	os05a_write_register(ViPipe, 0x4051, 0x01);
	os05a_write_register(ViPipe, 0x4052, 0x00);
	os05a_write_register(ViPipe, 0x4053, 0x80);
	os05a_write_register(ViPipe, 0x4054, 0x00);
	os05a_write_register(ViPipe, 0x4055, 0x80);
	os05a_write_register(ViPipe, 0x4056, 0x00);
	os05a_write_register(ViPipe, 0x4057, 0x80);
	os05a_write_register(ViPipe, 0x4058, 0x00);
	os05a_write_register(ViPipe, 0x4059, 0x80);
	os05a_write_register(ViPipe, 0x430b, 0xff);
	os05a_write_register(ViPipe, 0x430c, 0xff);
	os05a_write_register(ViPipe, 0x430d, 0x00);
	os05a_write_register(ViPipe, 0x430e, 0x00);
	os05a_write_register(ViPipe, 0x4501, 0x18);
	os05a_write_register(ViPipe, 0x4502, 0x00);
	os05a_write_register(ViPipe, 0x4600, 0x00);
	os05a_write_register(ViPipe, 0x4601, 0x10);
	os05a_write_register(ViPipe, 0x4603, 0x01);
	os05a_write_register(ViPipe, 0x4643, 0x00);
	os05a_write_register(ViPipe, 0x4640, 0x01);
	os05a_write_register(ViPipe, 0x4641, 0x04);
	os05a_write_register(ViPipe, 0x480e, 0x00);
	os05a_write_register(ViPipe, 0x4813, 0x00);
	os05a_write_register(ViPipe, 0x4815, 0x2b);
	os05a_write_register(ViPipe, 0x486e, 0x36);
	os05a_write_register(ViPipe, 0x486f, 0x84);
	os05a_write_register(ViPipe, 0x4860, 0x00);
	os05a_write_register(ViPipe, 0x4861, 0xa0);
	os05a_write_register(ViPipe, 0x484b, 0x05);
	os05a_write_register(ViPipe, 0x4850, 0x00);
	os05a_write_register(ViPipe, 0x4851, 0xaa);
	os05a_write_register(ViPipe, 0x4852, 0xff);
	os05a_write_register(ViPipe, 0x4853, 0x8a);
	os05a_write_register(ViPipe, 0x4854, 0x08);
	os05a_write_register(ViPipe, 0x4855, 0x30);
	os05a_write_register(ViPipe, 0x4800, 0x00);
	os05a_write_register(ViPipe, 0x4837, 0x19);
	os05a_write_register(ViPipe, 0x5000, 0xc9);
	os05a_write_register(ViPipe, 0x5001, 0x43);
	os05a_write_register(ViPipe, 0x5211, 0x03);
	os05a_write_register(ViPipe, 0x5291, 0x03);
	os05a_write_register(ViPipe, 0x520d, 0x0f);
	os05a_write_register(ViPipe, 0x520e, 0xfd);
	os05a_write_register(ViPipe, 0x520f, 0xa5);
	os05a_write_register(ViPipe, 0x5210, 0xa5);
	os05a_write_register(ViPipe, 0x528d, 0x0f);
	os05a_write_register(ViPipe, 0x528e, 0xfd);
	os05a_write_register(ViPipe, 0x528f, 0xa5);
	os05a_write_register(ViPipe, 0x5290, 0xa5);
	os05a_write_register(ViPipe, 0x5004, 0x40);
	os05a_write_register(ViPipe, 0x5005, 0x00);
	os05a_write_register(ViPipe, 0x5180, 0x00);
	os05a_write_register(ViPipe, 0x5181, 0x10);
	os05a_write_register(ViPipe, 0x5182, 0x0f);
	os05a_write_register(ViPipe, 0x5183, 0xff);
	os05a_write_register(ViPipe, 0x580b, 0x03);

	os05a_write_register(ViPipe, 0x4d00, 0x03);
	os05a_write_register(ViPipe, 0x4d01, 0xe9);
	os05a_write_register(ViPipe, 0x4d02, 0xba);
	os05a_write_register(ViPipe, 0x4d03, 0x66);
	os05a_write_register(ViPipe, 0x4d04, 0x46);
	os05a_write_register(ViPipe, 0x4d05, 0xa5);
	os05a_write_register(ViPipe, 0x3603, 0x3c);
	os05a_write_register(ViPipe, 0x3703, 0x26);
	os05a_write_register(ViPipe, 0x3709, 0x49);
	os05a_write_register(ViPipe, 0x4000, 0x79);
	os05a_write_register(ViPipe, 0x380c, 0x06);
	os05a_write_register(ViPipe, 0x380d, 0x04);
	os05a_write_register(ViPipe, 0x380e, 0x09);
	os05a_write_register(ViPipe, 0x380f, 0x21);
	os05a_write_register(ViPipe, 0x3501, 0x09);
	os05a_write_register(ViPipe, 0x3502, 0x19);

	os05a_default_reg_init(ViPipe);

	os05a_write_register(ViPipe, 0x0100, 0x01);

	os05a_write_register(ViPipe, 0x0100, 0x00);

	os05a_write_register(ViPipe, 0x0100, 0x01);
	printf("-------OV os05a_init_5M_2688x1944_12bit_linear30 Initial OK!-------\n");
}
