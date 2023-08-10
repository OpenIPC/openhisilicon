
/******************************************************************************

  Copyright (C), 2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sc3336_sensor_ctl.c

  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2018/07/23
  Description   :
  History       :
  1.Date        : 2018/07/23
    Author      :
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "comm_video.h"
#include "sns_ctrl.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#ifdef __HuaweiLite__
#include "i2c.h"
#else
#include "hi_i2c.h"
#endif


#endif

#define LOSCFG_HOST_TYPE_HIBVT
#ifdef LOSCFG_HOST_TYPE_HIBVT
#include <linux/fb.h>
#endif

const unsigned char smartsens_i2c_addr     =    0x60;        /* I2C Address of SC3336 */
const unsigned int  smartsens_addr_byte    =    2;
const unsigned int  smartsens_data_byte    =    1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S       *g_pastSc3336[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U      g_aunSc3336BusInfo[];

int smartsens_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum;

    if (g_fd[ViPipe] >= 0) {
        return HI_SUCCESS;
    }
#ifdef HI_GPIO_I2C
    int ret;

    g_fd[ViPipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(HI_DBG_ERR, "Open gpioi2c_ex error!\n");
        return HI_FAILURE;
    }
#else
    int ret;

    u8DevNum = g_aunSc3336BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(HI_DBG_ERR, "Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (smartsens_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(HI_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int smartsens_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

struct i2c_rdwr_ioctl_data {
	struct i2c_msg  *msgs;
	unsigned int nmsgs;
};

int smartsens_read_register(VI_PIPE ViPipe, HI_U32 addr)
{
    HI_S32 s32RegVal = 0;
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(HI_DBG_ERR, "smartsens_read_register fd not opened!\n");
        return HI_FAILURE;
    }

    HI_S32 s32Ret = 0;
    HI_U32 u32RegWidth = smartsens_addr_byte;
    HI_U32 u32DataWidth = smartsens_data_byte;
    HI_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_HIBVT
    HI_U32 u32SnsI2cAddr = (smartsens_i2c_addr >> 1);
    struct i2c_rdwr_ioctl_data stRdwr;
    struct i2c_msg astMsg[2];
    memset(&stRdwr, 0x0, sizeof(stRdwr));
    memset(astMsg, 0x0, sizeof(astMsg));
#endif

    memset(aRecvbuf, 0x0, sizeof(aRecvbuf));

#ifdef LOSCFG_HOST_TYPE_HIBVT
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

#ifdef LOSCFG_HOST_TYPE_HIBVT
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
        return HI_FAILURE;
    }

    if (u32DataWidth == 2) {
        s32RegVal = aRecvbuf[0] | (aRecvbuf[1] << 8);
    } else {
        s32RegVal = aRecvbuf[0];
    }

    return s32RegVal;
}


int smartsens_write_register(VI_PIPE ViPipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[ViPipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = smartsens_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = smartsens_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = smartsens_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(HI_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (smartsens_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (smartsens_data_byte == 2) {
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, smartsens_addr_byte + smartsens_data_byte);
    if (ret < 0) {
        ISP_TRACE(HI_DBG_ERR, "I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

void smartsens_standby(VI_PIPE ViPipe)
{
    return;
}

void smartsens_restart(VI_PIPE ViPipe)
{
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

#define SC3336_SENSOR_1296P_30FPS_LINEAR_MODE (1)

void smartsens_linear_1296p30_init(VI_PIPE ViPipe);


void smartsens_init(VI_PIPE ViPipe)
{
    HI_U32           i;

    smartsens_i2c_init(ViPipe);

    smartsens_linear_1296p30_init(ViPipe);

    for (i = 0; i < g_pastSc3336[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        smartsens_write_register(ViPipe, g_pastSc3336[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr, g_pastSc3336[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }

    g_pastSc3336[ViPipe]->bInit = HI_TRUE;

    return;
}

void smartsens_exit(VI_PIPE ViPipe)
{
    smartsens_i2c_exit(ViPipe);

    return;
}



void smartsens_linear_1296p30_init(VI_PIPE ViPipe)
{
	smartsens_write_register(ViPipe, 0x0103, 0x01);
	delay_ms(3);
	smartsens_write_register(ViPipe, 0x0100, 0x00);
	smartsens_write_register(ViPipe, 0x36e9, 0x80);
	smartsens_write_register(ViPipe, 0x37f9, 0x80);
	smartsens_write_register(ViPipe, 0x301f, 0x60);
	smartsens_write_register(ViPipe, 0x30b8, 0x33);
	smartsens_write_register(ViPipe, 0x320c, 0x07);
	smartsens_write_register(ViPipe, 0x320d, 0x53);
	smartsens_write_register(ViPipe, 0x3253, 0x10);
	smartsens_write_register(ViPipe, 0x325f, 0x20);
	smartsens_write_register(ViPipe, 0x3301, 0x04);
	smartsens_write_register(ViPipe, 0x3302, 0xff);
	smartsens_write_register(ViPipe, 0x3306, 0x50);
	smartsens_write_register(ViPipe, 0x3309, 0xa8);
	smartsens_write_register(ViPipe, 0x330a, 0x00);
	smartsens_write_register(ViPipe, 0x330b, 0xd8);
	smartsens_write_register(ViPipe, 0x330d, 0xff);
	smartsens_write_register(ViPipe, 0x3314, 0x13);
	smartsens_write_register(ViPipe, 0x331f, 0x99);
	smartsens_write_register(ViPipe, 0x3333, 0x10);
	smartsens_write_register(ViPipe, 0x3334, 0x40);
	smartsens_write_register(ViPipe, 0x335e, 0x06);
	smartsens_write_register(ViPipe, 0x335f, 0x0a);
	smartsens_write_register(ViPipe, 0x3364, 0x5e);
	smartsens_write_register(ViPipe, 0x337c, 0x02);
	smartsens_write_register(ViPipe, 0x337d, 0x0e);
	smartsens_write_register(ViPipe, 0x3390, 0x01);
	smartsens_write_register(ViPipe, 0x3391, 0x03);
	smartsens_write_register(ViPipe, 0x3392, 0x07);
	smartsens_write_register(ViPipe, 0x3393, 0x04);
	smartsens_write_register(ViPipe, 0x3394, 0x04);
	smartsens_write_register(ViPipe, 0x3395, 0x04);
	smartsens_write_register(ViPipe, 0x3396, 0x08);
	smartsens_write_register(ViPipe, 0x3397, 0x0b);
	smartsens_write_register(ViPipe, 0x3398, 0x1f);
	smartsens_write_register(ViPipe, 0x3399, 0x04);
	smartsens_write_register(ViPipe, 0x339a, 0x0a);
	smartsens_write_register(ViPipe, 0x339b, 0x3a);
	smartsens_write_register(ViPipe, 0x339c, 0xff);
	smartsens_write_register(ViPipe, 0x33a2, 0x04);
	smartsens_write_register(ViPipe, 0x33ac, 0x08);
	smartsens_write_register(ViPipe, 0x33ad, 0x1c);
	smartsens_write_register(ViPipe, 0x33ae, 0x10);
	smartsens_write_register(ViPipe, 0x33af, 0x30);
	smartsens_write_register(ViPipe, 0x33b1, 0x80);
	smartsens_write_register(ViPipe, 0x33b3, 0x48);
	smartsens_write_register(ViPipe, 0x33f9, 0x60);
	smartsens_write_register(ViPipe, 0x33fb, 0x74);
	smartsens_write_register(ViPipe, 0x33fc, 0x4b);
	smartsens_write_register(ViPipe, 0x33fd, 0x5f);
	smartsens_write_register(ViPipe, 0x349f, 0x03);
	smartsens_write_register(ViPipe, 0x34a6, 0x4b);
	smartsens_write_register(ViPipe, 0x34a7, 0x5f);
	smartsens_write_register(ViPipe, 0x34a8, 0x20);
	smartsens_write_register(ViPipe, 0x34a9, 0x18);
	smartsens_write_register(ViPipe, 0x34ab, 0xe8);
	smartsens_write_register(ViPipe, 0x34ac, 0x01);
	smartsens_write_register(ViPipe, 0x34ad, 0x00);
	smartsens_write_register(ViPipe, 0x34f8, 0x5f);
	smartsens_write_register(ViPipe, 0x34f9, 0x18);
	smartsens_write_register(ViPipe, 0x3630, 0xc0);
	smartsens_write_register(ViPipe, 0x3631, 0x84);
	smartsens_write_register(ViPipe, 0x3632, 0x64);
	smartsens_write_register(ViPipe, 0x3633, 0x32);
	smartsens_write_register(ViPipe, 0x363b, 0x03);
	smartsens_write_register(ViPipe, 0x363c, 0x08);
	smartsens_write_register(ViPipe, 0x3641, 0x38);
	smartsens_write_register(ViPipe, 0x3670, 0x4e);
	smartsens_write_register(ViPipe, 0x3674, 0xc0);
	smartsens_write_register(ViPipe, 0x3675, 0xc0);
	smartsens_write_register(ViPipe, 0x3676, 0xc0);
	smartsens_write_register(ViPipe, 0x3677, 0x84);
	smartsens_write_register(ViPipe, 0x3678, 0x84);
	smartsens_write_register(ViPipe, 0x3679, 0x84);
	smartsens_write_register(ViPipe, 0x367c, 0x48);
	smartsens_write_register(ViPipe, 0x367d, 0x49);
	smartsens_write_register(ViPipe, 0x367e, 0x4b);
	smartsens_write_register(ViPipe, 0x367f, 0x5f);
	smartsens_write_register(ViPipe, 0x3690, 0x32);
	smartsens_write_register(ViPipe, 0x3691, 0x32);
	smartsens_write_register(ViPipe, 0x3692, 0x42);
	smartsens_write_register(ViPipe, 0x369c, 0x4b);
	smartsens_write_register(ViPipe, 0x369d, 0x5f);
	smartsens_write_register(ViPipe, 0x36b0, 0x87);
	smartsens_write_register(ViPipe, 0x36b1, 0x90);
	smartsens_write_register(ViPipe, 0x36b2, 0xa1);
	smartsens_write_register(ViPipe, 0x36b3, 0xd8);
	smartsens_write_register(ViPipe, 0x36b4, 0x49);
	smartsens_write_register(ViPipe, 0x36b5, 0x4b);
	smartsens_write_register(ViPipe, 0x36b6, 0x4f);
	smartsens_write_register(ViPipe, 0x370f, 0x01);
	smartsens_write_register(ViPipe, 0x3722, 0x09);
	smartsens_write_register(ViPipe, 0x3724, 0x41);
	smartsens_write_register(ViPipe, 0x3725, 0xc1);
	smartsens_write_register(ViPipe, 0x3771, 0x09);
	smartsens_write_register(ViPipe, 0x3772, 0x09);
	smartsens_write_register(ViPipe, 0x3773, 0x05);
	smartsens_write_register(ViPipe, 0x377a, 0x48);
	smartsens_write_register(ViPipe, 0x377b, 0x5f);
	smartsens_write_register(ViPipe, 0x3904, 0x04);
	smartsens_write_register(ViPipe, 0x3905, 0x8c);
	smartsens_write_register(ViPipe, 0x391d, 0x04);
	smartsens_write_register(ViPipe, 0x3921, 0x20);
	smartsens_write_register(ViPipe, 0x3926, 0x21);
	smartsens_write_register(ViPipe, 0x3933, 0x80);
	smartsens_write_register(ViPipe, 0x3934, 0x0a);
	smartsens_write_register(ViPipe, 0x3935, 0x00);
	smartsens_write_register(ViPipe, 0x3936, 0x2a);
	smartsens_write_register(ViPipe, 0x3937, 0x6a);
	smartsens_write_register(ViPipe, 0x3938, 0x6a);
	smartsens_write_register(ViPipe, 0x39dc, 0x02);
	smartsens_write_register(ViPipe, 0x3e01, 0x53);
	smartsens_write_register(ViPipe, 0x3e02, 0xe0);
	smartsens_write_register(ViPipe, 0x3e09, 0x00);
	smartsens_write_register(ViPipe, 0x440e, 0x02);
	smartsens_write_register(ViPipe, 0x4509, 0x20);
	smartsens_write_register(ViPipe, 0x5ae0, 0xfe);
	smartsens_write_register(ViPipe, 0x5ae1, 0x40);
	smartsens_write_register(ViPipe, 0x5ae2, 0x38);
	smartsens_write_register(ViPipe, 0x5ae3, 0x30);
	smartsens_write_register(ViPipe, 0x5ae4, 0x28);
	smartsens_write_register(ViPipe, 0x5ae5, 0x38);
	smartsens_write_register(ViPipe, 0x5ae6, 0x30);
	smartsens_write_register(ViPipe, 0x5ae7, 0x28);
	smartsens_write_register(ViPipe, 0x5ae8, 0x3f);
	smartsens_write_register(ViPipe, 0x5ae9, 0x34);
	smartsens_write_register(ViPipe, 0x5aea, 0x2c);
	smartsens_write_register(ViPipe, 0x5aeb, 0x3f);
	smartsens_write_register(ViPipe, 0x5aec, 0x34);
	smartsens_write_register(ViPipe, 0x5aed, 0x2c);
	smartsens_write_register(ViPipe, 0x36e9, 0x54);
	smartsens_write_register(ViPipe, 0x37f9, 0x27);
	smartsens_write_register(ViPipe, 0x0100, 0x01);

    delay_ms(10);

    printf("===============================================================================================\n");
    printf("===============================================================================================\n");
    printf("===============================================================================================\n");
    printf("===============================================================================================\n");
    printf("===cleaned_0x60_DN_SC3336_MIPI_27Minput_2Lane_10bit_506.25Mbps_2304x1296_20fps init success!===\n");
    printf("===============================================================================================\n");
    printf("===============================================================================================\n");
    printf("===============================================================================================\n");
    printf("===============================================================================================\n");

    return;
}
