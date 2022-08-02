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

const unsigned char sc3335_i2c_addr     =    0x60;        /* I2C Address of SC3335 */
const unsigned int  sc3335_addr_byte    =    2;
const unsigned int  sc3335_data_byte    =    1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S       *g_pastSc3335[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U      g_aunSc3335BusInfo[];

int sc3335_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[16] = {0};
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

    u8DevNum = g_aunSc3335BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc3335_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int sc3335_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int sc3335_read_register(VI_PIPE ViPipe, GK_U32 addr)
{
    GK_S32 s32RegVal = 0;

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "sc3335_read_register fd not opened!\n");
        return GK_FAILURE;
    }

    GK_S32 s32Ret = 0;
    GK_U32 u32RegWidth = sc3335_addr_byte;
    GK_U32 u32DataWidth = sc3335_data_byte;
    GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
    GK_U32 u32SnsI2cAddr = (sc3335_i2c_addr >> 1);
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

int sc3335_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = sc3335_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = sc3335_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = sc3335_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (sc3335_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (sc3335_data_byte == 2) {
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, sc3335_addr_byte + sc3335_data_byte);
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }

#endif
    return GK_SUCCESS;
}

void sc3335_standby(VI_PIPE ViPipe)
{
    return;
}

void sc3335_restart(VI_PIPE ViPipe)
{
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

#define SC3335_SENSOR_1296P_30FPS_LINEAR_MODE (1)

void sc3335_linear_1296p30_init(VI_PIPE ViPipe);

void sc3335_init(VI_PIPE ViPipe)
{
    GK_U32           i;

    sc3335_i2c_init(ViPipe);
    sc3335_linear_1296p30_init(ViPipe);
    for (i = 0; i < g_pastSc3335[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        sc3335_write_register(ViPipe,
            g_pastSc3335[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr,
            g_pastSc3335[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }
    g_pastSc3335[ViPipe]->bInit = GK_TRUE;
    return;
}

void sc3335_exit(VI_PIPE ViPipe)
{
    sc3335_i2c_exit(ViPipe);
    return;
}

void sc3335_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            sc3335_write_register(ViPipe, 0x3221, 0x00);
            break;

        case ISP_SNS_MIRROR:
            sc3335_write_register(ViPipe, 0x3221, 0x06);
            break;

        case ISP_SNS_FLIP:
            sc3335_write_register(ViPipe, 0x3221, 0x60);
            break;

        case ISP_SNS_MIRROR_FLIP:
            sc3335_write_register(ViPipe, 0x3221, 0x66);
            break;
    }
    return;
}

void sc3335_linear_1296p30_init(VI_PIPE ViPipe)
{
	sc3335_write_register(ViPipe, 0x0103, 0x01);
	delay_ms(3);
	sc3335_write_register(ViPipe, 0x0100, 0x00);
	sc3335_write_register(ViPipe, 0x36e9, 0x80);
	sc3335_write_register(ViPipe, 0x36f9, 0x80);
	sc3335_write_register(ViPipe, 0x301f, 0x01);
	sc3335_write_register(ViPipe, 0x3253, 0x04);
	sc3335_write_register(ViPipe, 0x3301, 0x04);
	sc3335_write_register(ViPipe, 0x3302, 0x10);
	sc3335_write_register(ViPipe, 0x3304, 0x40);
	sc3335_write_register(ViPipe, 0x3306, 0x40);
	sc3335_write_register(ViPipe, 0x3309, 0x50);
	sc3335_write_register(ViPipe, 0x330b, 0xb6);
	sc3335_write_register(ViPipe, 0x330e, 0x29);
	sc3335_write_register(ViPipe, 0x3310, 0x06);
	sc3335_write_register(ViPipe, 0x3314, 0x96);
	sc3335_write_register(ViPipe, 0x331e, 0x39);
	sc3335_write_register(ViPipe, 0x331f, 0x49);
	sc3335_write_register(ViPipe, 0x3320, 0x09);
	sc3335_write_register(ViPipe, 0x3333, 0x10);
	sc3335_write_register(ViPipe, 0x334c, 0x01);
	sc3335_write_register(ViPipe, 0x3364, 0x17);
	sc3335_write_register(ViPipe, 0x3367, 0x01);
	sc3335_write_register(ViPipe, 0x3390, 0x04);
	sc3335_write_register(ViPipe, 0x3391, 0x08);
	sc3335_write_register(ViPipe, 0x3392, 0x38);
	sc3335_write_register(ViPipe, 0x3393, 0x05);
	sc3335_write_register(ViPipe, 0x3394, 0x09);
	sc3335_write_register(ViPipe, 0x3395, 0x16);
	sc3335_write_register(ViPipe, 0x33ac, 0x0c);
	sc3335_write_register(ViPipe, 0x33ae, 0x1c);
	sc3335_write_register(ViPipe, 0x3622, 0x16);
	sc3335_write_register(ViPipe, 0x3637, 0x22);
	sc3335_write_register(ViPipe, 0x363a, 0x1f);
	sc3335_write_register(ViPipe, 0x363c, 0x05);
	sc3335_write_register(ViPipe, 0x3670, 0x0e);
	sc3335_write_register(ViPipe, 0x3674, 0xb0);
	sc3335_write_register(ViPipe, 0x3675, 0x88);
	sc3335_write_register(ViPipe, 0x3676, 0x68);
	sc3335_write_register(ViPipe, 0x3677, 0x84);
	sc3335_write_register(ViPipe, 0x3678, 0x85);
	sc3335_write_register(ViPipe, 0x3679, 0x86);
	sc3335_write_register(ViPipe, 0x367c, 0x18);
	sc3335_write_register(ViPipe, 0x367d, 0x38);
	sc3335_write_register(ViPipe, 0x367e, 0x08);
	sc3335_write_register(ViPipe, 0x367f, 0x18);
	sc3335_write_register(ViPipe, 0x3690, 0x43);
	sc3335_write_register(ViPipe, 0x3691, 0x43);
	sc3335_write_register(ViPipe, 0x3692, 0x44);
	sc3335_write_register(ViPipe, 0x369c, 0x18);
	sc3335_write_register(ViPipe, 0x369d, 0x38);
	sc3335_write_register(ViPipe, 0x36ea, 0x3b);
	sc3335_write_register(ViPipe, 0x36eb, 0x0d);
	sc3335_write_register(ViPipe, 0x36ec, 0x1c);
	sc3335_write_register(ViPipe, 0x36ed, 0x24);
	sc3335_write_register(ViPipe, 0x36fa, 0x3b);
	sc3335_write_register(ViPipe, 0x36fb, 0x00);
	sc3335_write_register(ViPipe, 0x36fc, 0x10);
	sc3335_write_register(ViPipe, 0x36fd, 0x24);
	sc3335_write_register(ViPipe, 0x3908, 0x82);
	sc3335_write_register(ViPipe, 0x391f, 0x18);
	sc3335_write_register(ViPipe, 0x3e01, 0xa8);
	sc3335_write_register(ViPipe, 0x3e02, 0x20);
	sc3335_write_register(ViPipe, 0x3f09, 0x48);
	sc3335_write_register(ViPipe, 0x4505, 0x08);
	sc3335_write_register(ViPipe, 0x4509, 0x20);
	sc3335_write_register(ViPipe, 0x5799, 0x00);
	sc3335_write_register(ViPipe, 0x59e0, 0x60);
	sc3335_write_register(ViPipe, 0x59e1, 0x08);
	sc3335_write_register(ViPipe, 0x59e2, 0x3f);
	sc3335_write_register(ViPipe, 0x59e3, 0x18);
	sc3335_write_register(ViPipe, 0x59e4, 0x18);
	sc3335_write_register(ViPipe, 0x59e5, 0x3f);
	sc3335_write_register(ViPipe, 0x59e6, 0x06);
	sc3335_write_register(ViPipe, 0x59e7, 0x02);
	sc3335_write_register(ViPipe, 0x59e8, 0x38);
	sc3335_write_register(ViPipe, 0x59e9, 0x10);
	sc3335_write_register(ViPipe, 0x59ea, 0x0c);
	sc3335_write_register(ViPipe, 0x59eb, 0x10);
	sc3335_write_register(ViPipe, 0x59ec, 0x04);
	sc3335_write_register(ViPipe, 0x59ed, 0x02);
	sc3335_write_register(ViPipe, 0x36e9, 0x23);
	sc3335_write_register(ViPipe, 0x36f9, 0x23);

	sc3335_write_register(ViPipe, 0x3221, 0x00);
	sc3335_write_register(ViPipe, 0x0100, 0x01);

    delay_ms(10);
    printf("==============================================================\n");
    printf("===Smart sc3335 sensor 1296P30fps(MIPI) init success!===\n");
    printf("==============================================================\n");
    return;
}

