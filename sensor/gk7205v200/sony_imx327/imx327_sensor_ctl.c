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

const unsigned char imx327_i2c_addr     =    0x34;        /* I2C Address of IMX327 */
const unsigned int  imx327_addr_byte    =    2;
const unsigned int  imx327_data_byte    =    1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S       *g_pastImx327[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U      g_aunImx327BusInfo[];

int imx327_i2c_init(VI_PIPE ViPipe)
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

    u8DevNum = g_aunImx327BusInfo[ViPipe].s8I2cDev;
    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx327_i2c_addr >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int imx327_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

int imx327_read_register(VI_PIPE ViPipe, int addr)
{
    return GK_SUCCESS;
}


int imx327_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = imx327_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx327_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx327_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (imx327_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (imx327_data_byte == 2) {
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, imx327_addr_byte + imx327_data_byte);
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


void imx327_standby(VI_PIPE ViPipe)
{
    return;
}

void imx327_restart(VI_PIPE ViPipe)
{
    return;
}

#define IMX327_SENSOR_1080P_30FPS_LINEAR_MODE         (1)
#define IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE       (2)
#define IMX327_SENSOR_1080P_30FPS_FRAME_WDR_2to1_MODE (3)

void imx327_wdr_1080p30_2to1_init(VI_PIPE ViPipe);
void imx327_linear_1080p30_init(VI_PIPE ViPipe);

void imx327_default_reg_init(VI_PIPE ViPipe)
{
    GK_U32 i;
    for (i = 0; i < g_pastImx327[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        imx327_write_register(ViPipe, g_pastImx327[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr, g_pastImx327[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }
}
void imx327_init(VI_PIPE ViPipe)
{
    WDR_MODE_E       enWDRMode;
    GK_U8            u8ImgMode;

    enWDRMode   = g_pastImx327[ViPipe]->enWDRMode;
    u8ImgMode   = g_pastImx327[ViPipe]->u8ImgMode;

    imx327_i2c_init(ViPipe);

    /* When sensor first init, config all registers */
    if (WDR_MODE_2To1_LINE == enWDRMode) {
        if (IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE == u8ImgMode) {  /* IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE */
            imx327_wdr_1080p30_2to1_init(ViPipe);
        } else {
        }
    } else {
        imx327_linear_1080p30_init(ViPipe);
    }
    g_pastImx327[ViPipe]->bInit = GK_TRUE;
    return;
}

void imx327_exit(VI_PIPE ViPipe)
{
    imx327_i2c_exit(ViPipe);

    return;
}

/* 1080P30 and 1080P25 */
void imx327_linear_1080p30_init(VI_PIPE ViPipe)
{
    imx327_write_register(ViPipe, 0x3000, 0x01);  /* STANDBY */
    imx327_write_register(ViPipe, 0x3002, 0x01);  /* XTMSTA */

    imx327_write_register(ViPipe, 0x3009, 0x02);
    imx327_write_register(ViPipe, 0x300a, 0xf0);
    imx327_write_register(ViPipe, 0x3011, 0x02);
    imx327_write_register(ViPipe, 0x3018, 0x65);  /* Vmax */
    imx327_write_register(ViPipe, 0x3019, 0x04);  /* Vmax */
    imx327_write_register(ViPipe, 0x301c, 0x30);  /* Hmax */
    imx327_write_register(ViPipe, 0x301d, 0x11);  /* Hmax */
    imx327_write_register(ViPipe, 0x3046, 0x01);  /* ODBIT */

    imx327_write_register(ViPipe, 0x305c, 0x18);  /* inck1 */
    imx327_write_register(ViPipe, 0x305d, 0x03);  /* inck2 */
    imx327_write_register(ViPipe, 0x305e, 0x20);  /* inck3 */
    imx327_write_register(ViPipe, 0x309e, 0x4a);
    imx327_write_register(ViPipe, 0x309f, 0x4a);
    imx327_write_register(ViPipe, 0x30d2, 0x19);
    imx327_write_register(ViPipe, 0x30d7, 0x03);

    imx327_write_register(ViPipe, 0x3129, 0x00);  /* ADBIT1 */
    imx327_write_register(ViPipe, 0x313b, 0x61);

    imx327_write_register(ViPipe, 0x315e, 0x1a);  /* inck5 */
    imx327_write_register(ViPipe, 0x3164, 0x1a);  /* inck6 */
    imx327_write_register(ViPipe, 0x317c, 0x00);  /* ADBIT2 */
    imx327_write_register(ViPipe, 0x31ec, 0x0e);  /* ADBIT3 */

    imx327_write_register(ViPipe, 0x3644, 0x20);
    imx327_write_register(ViPipe, 0x3645, 0x25);
    imx327_write_register(ViPipe, 0x3672, 0xa0);
    imx327_write_register(ViPipe, 0x3673, 0x07);
    imx327_write_register(ViPipe, 0x367b, 0x23);
    imx327_write_register(ViPipe, 0x3680, 0x49);

    imx327_default_reg_init(ViPipe);
    imx327_write_register(ViPipe, 0x3000, 0x00);  /* standby */
    delay_ms(20);
    imx327_write_register(ViPipe, 0x3002, 0x00);  /* master mode start */
    imx327_write_register(ViPipe, 0x304b, 0x0a);  /* XVSOUTSEL XHSOUTSEL */

    printf("===IMX327 1080P 30fps 12bit LINE Init OK!===\n");
    return;
}

void imx327_wdr_1080p30_2to1_init(VI_PIPE ViPipe)
{
    imx327_write_register(ViPipe, 0x3000, 0x01);  /* standby */
    delay_ms(200);
    imx327_write_register(ViPipe, 0x3005, 0x01); /* 12Bit, 0x00, 10Bit */
    imx327_write_register(ViPipe, 0x3007, 0x00);
    imx327_write_register(ViPipe, 0x3009, 0x11);
    imx327_write_register(ViPipe, 0x300a, 0xf0);
    imx327_write_register(ViPipe, 0x300c, 0x11);

    imx327_write_register(ViPipe, 0x3010, 0x21);
    imx327_write_register(ViPipe, 0x3011, 0x02); /* Change after reset; */
    imx327_write_register(ViPipe, 0x3014, 0x02); /* Gain */

    imx327_write_register(ViPipe, 0x3020, 0x02); /* SHS1 */
    imx327_write_register(ViPipe, 0x3021, 0x00);
    imx327_write_register(ViPipe, 0x3022, 0x00);
    imx327_write_register(ViPipe, 0x3024, 0xC1); /* SHS2 */
    imx327_write_register(ViPipe, 0x3025, 0x08);
    imx327_write_register(ViPipe, 0x3026, 0x00);
    imx327_write_register(ViPipe, 0x3030, 0xCF); /* RHS1 */
    imx327_write_register(ViPipe, 0x3031, 0x01);
    imx327_write_register(ViPipe, 0x3032, 0x00);

    imx327_write_register(ViPipe, 0x3018, 0x46); /* VMAX[7:0] */
    imx327_write_register(ViPipe, 0x3019, 0x05); /* VMAX[15:8] */
    imx327_write_register(ViPipe, 0x301a, 0x00); /* VMAX[16] */
    imx327_write_register(ViPipe, 0x301c, 0x98); /* HMAX[7:0] */
    imx327_write_register(ViPipe, 0x301d, 0x08); /* HMAX[15:8] */

    imx327_write_register(ViPipe, 0x3045, 0x05);
    imx327_write_register(ViPipe, 0x3046, 0x01); /* OPORTSE&ODBIT */
    imx327_write_register(ViPipe, 0x305c, 0x18); /* INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C */
    imx327_write_register(ViPipe, 0x305d, 0x03); /* INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03 */
    imx327_write_register(ViPipe, 0x305e, 0x20); /* INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10 */
    imx327_write_register(ViPipe, 0x305f, 0x01); /* INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01 */

    imx327_write_register(ViPipe, 0x309e, 0x4a);
    imx327_write_register(ViPipe, 0x309f, 0x4a);
    imx327_write_register(ViPipe, 0x3106, 0x10);
    imx327_write_register(ViPipe, 0x311c, 0x0e);
    imx327_write_register(ViPipe, 0x3128, 0x04);
    imx327_write_register(ViPipe, 0x3129, 0x00); /* ADBIT1,12Bit;0x1D->10Bit */
    imx327_write_register(ViPipe, 0x313b, 0x41);

    imx327_write_register(ViPipe, 0x315e, 0x1a); /* INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B */
    imx327_write_register(ViPipe, 0x3164, 0x1a); /* INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B */
    imx327_write_register(ViPipe, 0x317c, 0x00); /* ADBIT2,12Bit;0x12->10Bit */
    imx327_write_register(ViPipe, 0x31ec, 0x0e); /* ADBIT3,12Bit;0x12->10Bit */
    imx327_write_register(ViPipe, 0x3480, 0x49); /* INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92 */

    /* MIPI setting */
    imx327_write_register(ViPipe, 0x3405, 0x10);
    imx327_write_register(ViPipe, 0x3407, 0x03);
    imx327_write_register(ViPipe, 0x3414, 0x0a);
    imx327_write_register(ViPipe, 0x3415, 0x00);

    imx327_write_register(ViPipe, 0x3418, 0x6e); /* Y_OUT_SIZE */
    imx327_write_register(ViPipe, 0x3419, 0x0a);
    imx327_write_register(ViPipe, 0x3441, 0x0c);
    imx327_write_register(ViPipe, 0x3442, 0x0c);
    imx327_write_register(ViPipe, 0x3443, 0x03);
    imx327_write_register(ViPipe, 0x3444, 0x20);
    imx327_write_register(ViPipe, 0x3445, 0x25);
    imx327_write_register(ViPipe, 0x3446, 0x57);
    imx327_write_register(ViPipe, 0x3447, 0x00);
    imx327_write_register(ViPipe, 0x3448, 0x37);
    imx327_write_register(ViPipe, 0x3449, 0x00);
    imx327_write_register(ViPipe, 0x344a, 0x1f);
    imx327_write_register(ViPipe, 0x344b, 0x00);
    imx327_write_register(ViPipe, 0x344c, 0x1f);
    imx327_write_register(ViPipe, 0x344d, 0x00);
    imx327_write_register(ViPipe, 0x344e, 0x1f);
    imx327_write_register(ViPipe, 0x344f, 0x00);
    imx327_write_register(ViPipe, 0x3450, 0x77);
    imx327_write_register(ViPipe, 0x3451, 0x00);
    imx327_write_register(ViPipe, 0x3452, 0x1e);
    imx327_write_register(ViPipe, 0x3453, 0x00);
    imx327_write_register(ViPipe, 0x3454, 0x17);
    imx327_write_register(ViPipe, 0x3455, 0x00);
    imx327_write_register(ViPipe, 0x3472, 0xa0);
    imx327_write_register(ViPipe, 0x3473, 0x07);
    imx327_write_register(ViPipe, 0x347b, 0x23);

    delay_ms(200);
    imx327_default_reg_init(ViPipe);
    imx327_write_register(ViPipe, 0x3000, 0x00);  /* standby */
    imx327_write_register(ViPipe, 0x3002, 0x00);  /* master mode start */
    imx327_write_register(ViPipe, 0x304b, 0x0a);

    printf("===Imx327 sensor 1080P30fps 12bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;
}
