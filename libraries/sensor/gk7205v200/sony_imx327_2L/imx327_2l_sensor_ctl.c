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

const unsigned char imx327_2l_i2c_addr     =    0x34;        /* I2C Address of IMX327 */
const unsigned int  imx327_2l_addr_byte    =    2;
const unsigned int  imx327_2l_data_byte    =    1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S       *g_pastImx327_2l[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U      g_aunImx327_2l_BusInfo[];

int imx327_2l_i2c_init(VI_PIPE ViPipe)
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

    u8DevNum = g_aunImx327_2l_BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx327_2l_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int imx327_2l_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int imx327_2l_read_register(VI_PIPE ViPipe, int addr)
{
    return GK_SUCCESS;
}


int imx327_2l_write_register(VI_PIPE ViPipe, int addr, int data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = imx327_2l_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx327_2l_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx327_2l_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (imx327_2l_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (imx327_2l_data_byte == 2) {
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, imx327_2l_addr_byte + imx327_2l_data_byte);
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

void imx327_2l_prog(VI_PIPE ViPipe, int *rom)
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
            imx327_2l_write_register(ViPipe, addr, data);
        }
    }
}

void imx327_2l_standby(VI_PIPE ViPipe)
{
    imx327_2l_write_register(ViPipe, 0x3000, 0x01);  /* STANDBY */
    imx327_2l_write_register(ViPipe, 0x3002, 0x01);  /* XTMSTA */

    return;
}

void imx327_2l_restart(VI_PIPE ViPipe)
{
    imx327_2l_write_register(ViPipe, 0x3000, 0x00);  /* standby */
    delay_ms(20);
    imx327_2l_write_register(ViPipe, 0x3002, 0x00);  /* master mode start */
    imx327_2l_write_register(ViPipe, 0x304b, 0x0a);

    return;
}

#define IMX327_SENSOR_1080P_30FPS_LINEAR_MODE         (1)
#define IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE       (2)
#define IMX327_SENSOR_1080P_30FPS_FRAME_WDR_2to1_MODE (3)

void imx327_2l_wdr_1080p30_2to1_init(VI_PIPE ViPipe);
void imx327_2l_linear_1080p30_init(VI_PIPE ViPipe);

void imx327_2l_default_reg_init(VI_PIPE ViPipe)
{
    GK_U32 i;

    for (i = 0; i < g_pastImx327_2l[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        imx327_2l_write_register(ViPipe, g_pastImx327_2l[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr, g_pastImx327_2l[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }
}

void imx327_2l_init(VI_PIPE ViPipe)
{
    WDR_MODE_E       enWDRMode;
    GK_U8            u8ImgMode;

    enWDRMode   = g_pastImx327_2l[ViPipe]->enWDRMode;
    u8ImgMode   = g_pastImx327_2l[ViPipe]->u8ImgMode;

    imx327_2l_i2c_init(ViPipe);

    /* When sensor first init, config all registers */

    if (WDR_MODE_2To1_LINE == enWDRMode) {
        if (IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE == u8ImgMode) {  /* IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE */
            imx327_2l_wdr_1080p30_2to1_init(ViPipe);
        } else {
        }
    } else {
        imx327_2l_linear_1080p30_init(ViPipe);
    }

    g_pastImx327_2l[ViPipe]->bInit = GK_TRUE;
    return;
}

void imx327_2l_exit(VI_PIPE ViPipe)
{
    imx327_2l_i2c_exit(ViPipe);

    return;
}

/* 1080P30 and 1080P25 */
void imx327_2l_linear_1080p30_init(VI_PIPE ViPipe)
{
    imx327_2l_write_register(ViPipe, 0x3000, 0x01);  /* STANDBY */
    imx327_2l_write_register(ViPipe, 0x3002, 0x00);  /* XTMSTA */
    imx327_2l_write_register(ViPipe, 0x3005, 0x01);
    imx327_2l_write_register(ViPipe, 0x3007, 0x00);
    imx327_2l_write_register(ViPipe, 0x3009, 0x02);
    imx327_2l_write_register(ViPipe, 0x300a, 0xf0);
    imx327_2l_write_register(ViPipe, 0x3011, 0x02);
    imx327_2l_write_register(ViPipe, 0x3018, 0x65);  /* Vmax */
    imx327_2l_write_register(ViPipe, 0x3019, 0x04);  /* Vmax */
    imx327_2l_write_register(ViPipe, 0x301c, 0x30);  /* Hmax */
    imx327_2l_write_register(ViPipe, 0x301d, 0x11);  /* Hmax */
    imx327_2l_write_register(ViPipe, 0x3046, 0x01);  /* ODBIT */
    imx327_2l_write_register(ViPipe, 0x304b, 0x0a);

    imx327_2l_write_register(ViPipe, 0x305c, 0x18);  /* inck1 */
    imx327_2l_write_register(ViPipe, 0x305d, 0x03);  /* inck2 */
    imx327_2l_write_register(ViPipe, 0x305e, 0x20);  /* inck3 */
    imx327_2l_write_register(ViPipe, 0x305f, 0x01);
    imx327_2l_write_register(ViPipe, 0x309e, 0x4a);
    imx327_2l_write_register(ViPipe, 0x309f, 0x4a);
    imx327_2l_write_register(ViPipe, 0x30d2, 0x19);
    imx327_2l_write_register(ViPipe, 0x30d7, 0x03);

    imx327_2l_write_register(ViPipe, 0x3129, 0x00);  /* ADBIT1 */
    imx327_2l_write_register(ViPipe, 0x313b, 0x61);
    imx327_2l_write_register(ViPipe, 0x315e, 0x1a);  /* inck5 */
    imx327_2l_write_register(ViPipe, 0x3164, 0x1a);  /* inck6 */
    imx327_2l_write_register(ViPipe, 0x317c, 0x00);  /* ADBIT2 */
    imx327_2l_write_register(ViPipe, 0x31ec, 0x0e);  /* ADBIT3 */

    imx327_2l_write_register(ViPipe, 0x3405, 0x10);
    imx327_2l_write_register(ViPipe, 0x3407, 0x01);
    imx327_2l_write_register(ViPipe, 0x3414, 0x0a);
    imx327_2l_write_register(ViPipe, 0x3418, 0x49);
    imx327_2l_write_register(ViPipe, 0x3419, 0x04);
    imx327_2l_write_register(ViPipe, 0x3441, 0x0c);
    imx327_2l_write_register(ViPipe, 0x3442, 0x0c);
    imx327_2l_write_register(ViPipe, 0x3443, 0x01);
    imx327_2l_write_register(ViPipe, 0x3444, 0x20);
    imx327_2l_write_register(ViPipe, 0x3445, 0x25);
    imx327_2l_write_register(ViPipe, 0x3446, 0x57);
    imx327_2l_write_register(ViPipe, 0x3447, 0x00);
    imx327_2l_write_register(ViPipe, 0x3448, 0x37);
    imx327_2l_write_register(ViPipe, 0x3449, 0x00);
    imx327_2l_write_register(ViPipe, 0x344a, 0x1f);
    imx327_2l_write_register(ViPipe, 0x344b, 0x00);
    imx327_2l_write_register(ViPipe, 0x344c, 0x1f);
    imx327_2l_write_register(ViPipe, 0x344d, 0x00);
    imx327_2l_write_register(ViPipe, 0x344e, 0x1f);
    imx327_2l_write_register(ViPipe, 0x344f, 0x00);
    imx327_2l_write_register(ViPipe, 0x3450, 0x77);
    imx327_2l_write_register(ViPipe, 0x3451, 0x00);
    imx327_2l_write_register(ViPipe, 0x3452, 0x1f);
    imx327_2l_write_register(ViPipe, 0x3453, 0x00);
    imx327_2l_write_register(ViPipe, 0x3454, 0x17);
    imx327_2l_write_register(ViPipe, 0x3455, 0x00);
    imx327_2l_write_register(ViPipe, 0x3472, 0x9c);
    imx327_2l_write_register(ViPipe, 0x3473, 0x07);
    imx327_2l_write_register(ViPipe, 0x3480, 0x49);

    imx327_2l_default_reg_init(ViPipe);

    imx327_2l_write_register(ViPipe, 0x3000, 0x00);  /* standby */

    printf("ViPipe:%d,===IMX327_2L 1080P 30fps 12bit LINE Init OK!===\n", ViPipe);
    return;
}

void imx327_2l_wdr_1080p30_2to1_init(VI_PIPE ViPipe)
{
    /* 10bit */
    imx327_2l_write_register(ViPipe, 0x3000, 0x01); /* standby */
    imx327_2l_write_register(ViPipe, 0x3002, 0x01);
    imx327_2l_write_register(ViPipe, 0x3005, 0x00); /* 12Bit, 0x01, 10Bit, 0x00 */
    imx327_2l_write_register(ViPipe, 0x3007, 0x40); /* WINMODE:HD1080p 0x0, HD720p 0x1 */
    imx327_2l_write_register(ViPipe, 0x3009, 0x01);
    imx327_2l_write_register(ViPipe, 0x300A, 0x3C);
    imx327_2l_write_register(ViPipe, 0x300C, 0x11);
    imx327_2l_write_register(ViPipe, 0x3011, 0x02);/*Change after reset;*/
    imx327_2l_write_register(ViPipe, 0x3018, 0xC4); /* VMAX[7:0] */
    imx327_2l_write_register(ViPipe, 0x3019, 0x04);/*VMAX[15:8]*/
    imx327_2l_write_register(ViPipe, 0x301c, 0xEC); /* HMAX[7:0] */
    imx327_2l_write_register(ViPipe, 0x301d, 0x07); /* HMAX[15:8] */

    imx327_2l_write_register(ViPipe, 0x3020, 0x02);
    imx327_2l_write_register(ViPipe, 0x3021, 0x00);
    imx327_2l_write_register(ViPipe, 0x3024, 0x53);
    imx327_2l_write_register(ViPipe, 0x3025, 0x04);
    imx327_2l_write_register(ViPipe, 0x3030, 0xE1); /* RHS1 */
    imx327_2l_write_register(ViPipe, 0x3031, 0x00);
    imx327_2l_write_register(ViPipe, 0x303A, 0x08);
    imx327_2l_write_register(ViPipe, 0x303C, 0x04); //  WINPV
    imx327_2l_write_register(ViPipe, 0x303D, 0x00);
    imx327_2l_write_register(ViPipe, 0x303E, 0x41); //  WINWV
    imx327_2l_write_register(ViPipe, 0x303F, 0x04);
    imx327_2l_write_register(ViPipe, 0x3045, 0x05);
    imx327_2l_write_register(ViPipe, 0x3046, 0x00); /* OPORTSE&ODBIT */
    //imx327_2l_write_register(ViPipe, 0x304b, 0x0a);
    imx327_2l_write_register(ViPipe, 0x305c, 0x18);/*INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C*/
    imx327_2l_write_register(ViPipe, 0x305d, 0x03);/*INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03*/
    imx327_2l_write_register(ViPipe, 0x305e, 0x20);/*INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10*/
    imx327_2l_write_register(ViPipe, 0x305f, 0x01);/*INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01*/

    imx327_2l_write_register(ViPipe, 0x309e, 0x4a);
    imx327_2l_write_register(ViPipe, 0x309f, 0x4a);
    imx327_2l_write_register(ViPipe, 0x30D2, 0x19);
    imx327_2l_write_register(ViPipe, 0x30D7, 0x03);
    imx327_2l_write_register(ViPipe, 0x3106, 0x11);
    imx327_2l_write_register(ViPipe, 0x3129, 0x1D); /* ADBIT1,12Bit;0x1D->10Bit */
    imx327_2l_write_register(ViPipe, 0x313b, 0x61);

    imx327_2l_write_register(ViPipe, 0x315e, 0x1a);/*INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B*/
    imx327_2l_write_register(ViPipe, 0x3164, 0x1a);/*INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B*/
    imx327_2l_write_register(ViPipe, 0x317c, 0x12); /* ADBIT2,12Bit;0x12->10Bit */
    imx327_2l_write_register(ViPipe, 0x31ec, 0x37); /* ADBIT3,12Bit;0x12->10Bit */

    imx327_2l_write_register(ViPipe, 0x3480, 0x49); /* INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92 */
    /*MIPI setting*/
    imx327_2l_write_register(ViPipe, 0x3405, 0x00);
    imx327_2l_write_register(ViPipe, 0x3407, 0x01);
    imx327_2l_write_register(ViPipe, 0x3414, 0x00);
    imx327_2l_write_register(ViPipe, 0x3415, 0x00);
    imx327_2l_write_register(ViPipe, 0x3418, 0x7A); /* Y_OUT_SIZE */
    imx327_2l_write_register(ViPipe, 0x3419, 0x09);
    imx327_2l_write_register(ViPipe, 0x3441, 0x0A);
    imx327_2l_write_register(ViPipe, 0x3442, 0x0A);
    imx327_2l_write_register(ViPipe, 0x3443, 0x01);
    imx327_2l_write_register(ViPipe, 0x3444, 0x20);
    imx327_2l_write_register(ViPipe, 0x3445, 0x25);
    imx327_2l_write_register(ViPipe, 0x3446, 0x77);
    imx327_2l_write_register(ViPipe, 0x3447, 0x00);
    imx327_2l_write_register(ViPipe, 0x3448, 0x67);
    imx327_2l_write_register(ViPipe, 0x3449, 0x00);
    imx327_2l_write_register(ViPipe, 0x344a, 0x47);
    imx327_2l_write_register(ViPipe, 0x344b, 0x00);
    imx327_2l_write_register(ViPipe, 0x344c, 0x37);
    imx327_2l_write_register(ViPipe, 0x344d, 0x00);
    imx327_2l_write_register(ViPipe, 0x344e, 0x3f);
    imx327_2l_write_register(ViPipe, 0x344f, 0x00);
    imx327_2l_write_register(ViPipe, 0x3450, 0xff);
    imx327_2l_write_register(ViPipe, 0x3451, 0x00);
    imx327_2l_write_register(ViPipe, 0x3452, 0x3f);
    imx327_2l_write_register(ViPipe, 0x3453, 0x00);
    imx327_2l_write_register(ViPipe, 0x3454, 0x37);
    imx327_2l_write_register(ViPipe, 0x3455, 0x00);
    imx327_2l_write_register(ViPipe, 0x3472, 0xa0);
    imx327_2l_write_register(ViPipe, 0x3473, 0x07);
    imx327_2l_write_register(ViPipe, 0x347b, 0x23);

    imx327_2l_default_reg_init(ViPipe);

    imx327_2l_write_register(ViPipe, 0x3000, 0x00); /* standby */
    delay_ms(20);
    imx327_2l_write_register(ViPipe, 0x3002, 0x00);  /* master mode start */
    imx327_2l_write_register(ViPipe, 0x304b, 0x0a);

    printf("===Imx327_2l sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;
}
