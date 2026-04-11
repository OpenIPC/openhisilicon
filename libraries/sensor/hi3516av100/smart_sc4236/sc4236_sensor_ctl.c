
/******************************************************************************

  Copyright (C), 2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sc4236_sensor_ctl.c

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

#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else

#ifdef __HuaweiLite__
#include "i2c.h"
#else
#include "hi_i2c.h"
#endif


#endif

const unsigned char sc4236_i2c_addr     =    0x60;        /* I2C Address of SC4236 */
const unsigned int  sc4236_addr_byte    =    2;
const unsigned int  sc4236_data_byte    =    1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S       *g_pastSc4236[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U      g_aunSc4236BusInfo[];

int sc4236_i2c_init(VI_PIPE ViPipe)
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

    u8DevNum = g_aunSc4236BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(HI_DBG_ERR, "Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc4236_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(HI_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int sc4236_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int sc4236_read_register(VI_PIPE ViPipe, HI_U32 addr)
{
    HI_S32 s32RegVal = 0;
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(HI_DBG_ERR, "sc4236_read_register fd not opened!\n");
        return HI_FAILURE;
    }

    HI_S32 s32Ret = 0;
    HI_U32 u32RegWidth = sc4236_addr_byte;
    HI_U32 u32DataWidth = sc4236_data_byte;
    HI_U8 aRecvbuf[4];


#ifdef LOSCFG_HOST_TYPE_HIBVT
    HI_U32 u32SnsI2cAddr = (sc4236_i2c_addr >> 1);
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


int sc4236_write_register(VI_PIPE ViPipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[ViPipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = sc4236_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = sc4236_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = sc4236_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(HI_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (sc4236_addr_byte == 2) {
        buf[idx] = (((HI_U32)addr) >> 8) & 0xff;
        idx++;
        buf[idx] = ((HI_U32)addr) & 0xff;
        idx++;
    } else {
    }

    if (sc4236_data_byte == 2) {
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, sc4236_addr_byte + sc4236_data_byte);
    if (ret < 0) {
        ISP_TRACE(HI_DBG_ERR, "I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void sc4236_standby(VI_PIPE ViPipe)
{
    return;
}

void sc4236_restart(VI_PIPE ViPipe)
{
    return;
}

#define SC4236_SENSOR_1296P_30FPS_LINEAR_MODE (1)

void sc4236_linear_1296p30_init(VI_PIPE ViPipe);

void sc4236_init(VI_PIPE ViPipe)
{
    HI_U32           i;

    sc4236_i2c_init(ViPipe);

    sc4236_linear_1296p30_init(ViPipe);
    delay_ms(10);

    for (i = 0; i < g_pastSc4236[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        sc4236_write_register(ViPipe, g_pastSc4236[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr, g_pastSc4236[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }

    g_pastSc4236[ViPipe]->bInit = HI_TRUE;

    return;
}

void sc4236_exit(VI_PIPE ViPipe)
{
    sc4236_i2c_exit(ViPipe);

    return;
}

/* 1296P30 (2304X1296@30fps) */
void sc4236_linear_1296p30_init(VI_PIPE ViPipe)
{
    sc4236_write_register(ViPipe, 0x0100, 0x00);
    sc4236_write_register(ViPipe, 0x36e9, 0x82);  // pll1 bypass
    sc4236_write_register(ViPipe, 0x36f9, 0x81);  // pll2 bypass
    sc4236_write_register(ViPipe, 0x3001, 0xfe);
    sc4236_write_register(ViPipe, 0x3018, 0x33);
    sc4236_write_register(ViPipe, 0x301c, 0x78);
    sc4236_write_register(ViPipe, 0x3031, 0x0a);
    sc4236_write_register(ViPipe, 0x3037, 0x20);
    sc4236_write_register(ViPipe, 0x3038, 0xcc);
    sc4236_write_register(ViPipe, 0x303f, 0x01);
    sc4236_write_register(ViPipe, 0x3200, 0x00);
    sc4236_write_register(ViPipe, 0x3201, 0x04);
    sc4236_write_register(ViPipe, 0x3202, 0x00);
    sc4236_write_register(ViPipe, 0x3203, 0x7c);
    sc4236_write_register(ViPipe, 0x3204, 0x09);
    sc4236_write_register(ViPipe, 0x3205, 0x0b);
    sc4236_write_register(ViPipe, 0x3206, 0x05);
    sc4236_write_register(ViPipe, 0x3207, 0x93);
    sc4236_write_register(ViPipe, 0x3208, 0x09);
    sc4236_write_register(ViPipe, 0x3209, 0x00);
    sc4236_write_register(ViPipe, 0x320a, 0x05);
    sc4236_write_register(ViPipe, 0x320b, 0x10);
    sc4236_write_register(ViPipe, 0x320c, 0x09);
    sc4236_write_register(ViPipe, 0x320d, 0xc4);
    sc4236_write_register(ViPipe, 0x320e, 0x06);
    sc4236_write_register(ViPipe, 0x320f, 0x40);
    sc4236_write_register(ViPipe, 0x3211, 0x04);
    sc4236_write_register(ViPipe, 0x3213, 0x04);
    sc4236_write_register(ViPipe, 0x3222, 0x29);
    sc4236_write_register(ViPipe, 0x3235, 0x0c);
    sc4236_write_register(ViPipe, 0x3236, 0x7e);
    sc4236_write_register(ViPipe, 0x3301, 0x1e);
    sc4236_write_register(ViPipe, 0x3303, 0x30);
    sc4236_write_register(ViPipe, 0x3306, 0x60);
    sc4236_write_register(ViPipe, 0x3307, 0x18);
    sc4236_write_register(ViPipe, 0x3308, 0x18);
    sc4236_write_register(ViPipe, 0x3309, 0x80);
    sc4236_write_register(ViPipe, 0x330a, 0x01);
    sc4236_write_register(ViPipe, 0x330b, 0x10);
    sc4236_write_register(ViPipe, 0x330e, 0x50);
    sc4236_write_register(ViPipe, 0x330f, 0x04);
    sc4236_write_register(ViPipe, 0x3310, 0x20);
    sc4236_write_register(ViPipe, 0x3314, 0x04);
    sc4236_write_register(ViPipe, 0x331e, 0x21);
    sc4236_write_register(ViPipe, 0x331f, 0x71);
    sc4236_write_register(ViPipe, 0x3320, 0x06);
    sc4236_write_register(ViPipe, 0x3326, 0x00);
    sc4236_write_register(ViPipe, 0x3333, 0x20);
    sc4236_write_register(ViPipe, 0x3366, 0x78);
    sc4236_write_register(ViPipe, 0x3367, 0x08);
    sc4236_write_register(ViPipe, 0x3368, 0x04);
    sc4236_write_register(ViPipe, 0x3369, 0x00);
    sc4236_write_register(ViPipe, 0x336a, 0x00);
    sc4236_write_register(ViPipe, 0x336b, 0x00);
    sc4236_write_register(ViPipe, 0x337f, 0x03);
    sc4236_write_register(ViPipe, 0x33aa, 0x00);
    sc4236_write_register(ViPipe, 0x3620, 0x28);
    sc4236_write_register(ViPipe, 0x3622, 0xf6);
    sc4236_write_register(ViPipe, 0x3625, 0x03);
    sc4236_write_register(ViPipe, 0x3630, 0x80);
    sc4236_write_register(ViPipe, 0x3631, 0x88);
    sc4236_write_register(ViPipe, 0x3632, 0x18);
    sc4236_write_register(ViPipe, 0x3633, 0x23);
    sc4236_write_register(ViPipe, 0x3635, 0xe2);
    sc4236_write_register(ViPipe, 0x3636, 0x24);
    sc4236_write_register(ViPipe, 0x3637, 0x63);
    sc4236_write_register(ViPipe, 0x3638, 0x18);
    sc4236_write_register(ViPipe, 0x3639, 0x09);
    sc4236_write_register(ViPipe, 0x363a, 0x1f);
    sc4236_write_register(ViPipe, 0x363b, 0x0c);
    sc4236_write_register(ViPipe, 0x363c, 0x05);
    sc4236_write_register(ViPipe, 0x363d, 0x05);
    sc4236_write_register(ViPipe, 0x3640, 0x02);
    sc4236_write_register(ViPipe, 0x3641, 0x02);
    sc4236_write_register(ViPipe, 0x3650, 0x42);
    sc4236_write_register(ViPipe, 0x366e, 0x08);
    sc4236_write_register(ViPipe, 0x366f, 0x2f);
    sc4236_write_register(ViPipe, 0x36ea, 0x36);
    sc4236_write_register(ViPipe, 0x36eb, 0x06);
    sc4236_write_register(ViPipe, 0x36ec, 0x0e);
    sc4236_write_register(ViPipe, 0x36ed, 0x03);
    sc4236_write_register(ViPipe, 0x36fa, 0x8a);
    sc4236_write_register(ViPipe, 0x36fb, 0x00);
    sc4236_write_register(ViPipe, 0x3802, 0x00);
    sc4236_write_register(ViPipe, 0x3901, 0x02);
    sc4236_write_register(ViPipe, 0x3908, 0x15);
    sc4236_write_register(ViPipe, 0x3933, 0x0a);
    sc4236_write_register(ViPipe, 0x3934, 0x16);
    sc4236_write_register(ViPipe, 0x3940, 0x15);
    sc4236_write_register(ViPipe, 0x3941, 0x14);
    sc4236_write_register(ViPipe, 0x3942, 0x02);
    sc4236_write_register(ViPipe, 0x3943, 0x20);
    sc4236_write_register(ViPipe, 0x3946, 0x48);
    sc4236_write_register(ViPipe, 0x3947, 0x20);
    sc4236_write_register(ViPipe, 0x3948, 0x0a);
    sc4236_write_register(ViPipe, 0x3949, 0x10);
    sc4236_write_register(ViPipe, 0x394a, 0x28);
    sc4236_write_register(ViPipe, 0x394b, 0x48);
    sc4236_write_register(ViPipe, 0x394c, 0x08);
    sc4236_write_register(ViPipe, 0x394d, 0x14);
    sc4236_write_register(ViPipe, 0x394e, 0x28);
    sc4236_write_register(ViPipe, 0x394f, 0x40);
    sc4236_write_register(ViPipe, 0x3950, 0x20);
    sc4236_write_register(ViPipe, 0x3951, 0x10);
    sc4236_write_register(ViPipe, 0x3952, 0x68);
    sc4236_write_register(ViPipe, 0x3953, 0x38);
    sc4236_write_register(ViPipe, 0x3954, 0x18);
    sc4236_write_register(ViPipe, 0x3955, 0x18);
    sc4236_write_register(ViPipe, 0x3956, 0x38);
    sc4236_write_register(ViPipe, 0x3957, 0x78);
    sc4236_write_register(ViPipe, 0x3958, 0x10);
    sc4236_write_register(ViPipe, 0x3959, 0x20);
    sc4236_write_register(ViPipe, 0x395a, 0x38);
    sc4236_write_register(ViPipe, 0x395b, 0x38);
    sc4236_write_register(ViPipe, 0x395c, 0x20);
    sc4236_write_register(ViPipe, 0x395d, 0x10);
    sc4236_write_register(ViPipe, 0x395e, 0x24);
    sc4236_write_register(ViPipe, 0x395f, 0x00);
    sc4236_write_register(ViPipe, 0x3960, 0xc4);
    sc4236_write_register(ViPipe, 0x3961, 0xb1);
    sc4236_write_register(ViPipe, 0x3962, 0x09);
    sc4236_write_register(ViPipe, 0x3963, 0x80);
    sc4236_write_register(ViPipe, 0x3e01, 0x70);
    sc4236_write_register(ViPipe, 0x3e02, 0x60);
    sc4236_write_register(ViPipe, 0x3e06, 0x00);
    sc4236_write_register(ViPipe, 0x3e08, 0x03);
    sc4236_write_register(ViPipe, 0x3e09, 0x10);
    sc4236_write_register(ViPipe, 0x3e1e, 0x34);
    sc4236_write_register(ViPipe, 0x3f00, 0x0f);
    sc4236_write_register(ViPipe, 0x3f04, 0x04);
    sc4236_write_register(ViPipe, 0x3f05, 0xbe);
    sc4236_write_register(ViPipe, 0x4603, 0x00);
    sc4236_write_register(ViPipe, 0x4827, 0x46);
    sc4236_write_register(ViPipe, 0x4837, 0x20);
    sc4236_write_register(ViPipe, 0x5000, 0x00);  // 0x06
    sc4236_write_register(ViPipe, 0x5002, 0x00);  // 0x06
    sc4236_write_register(ViPipe, 0x5780, 0x7f);  // audo dpc
    sc4236_write_register(ViPipe, 0x5781, 0x04);
    sc4236_write_register(ViPipe, 0x5782, 0x03);
    sc4236_write_register(ViPipe, 0x5783, 0x02);
    sc4236_write_register(ViPipe, 0x5784, 0x01);
    sc4236_write_register(ViPipe, 0x5785, 0x18);
    sc4236_write_register(ViPipe, 0x5786, 0x10);
    sc4236_write_register(ViPipe, 0x5787, 0x08);
    sc4236_write_register(ViPipe, 0x5788, 0x02);
    sc4236_write_register(ViPipe, 0x5789, 0x20);
    sc4236_write_register(ViPipe, 0x578a, 0x30);
    sc4236_write_register(ViPipe, 0x36e9, 0x02);  // pll1 enable
    sc4236_write_register(ViPipe, 0x36f9, 0x01);  // pll2 enable
    sc4236_write_register(ViPipe, 0x0100, 0x01);

    sc4236_write_register(ViPipe, 0x3221, 0x66);  // mirror & filp

    printf("==============================================================\n");
    printf("===Smart sc4236 sensor 1296P30fps(MIPI) init success!=====\n");
    printf("==============================================================\n");
    return;
}
