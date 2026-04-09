/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx327_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx327_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

int imx327_i2c_init(VI_PIPE vi_pipe)
{
    char acDevFile[I2C_DEV_FILE_NUM] = {0};
    HI_U8 u8DevNum;

    if (g_fd[vi_pipe] >= 0) {
        return HI_SUCCESS;
    }
#ifdef HI_GPIO_I2C
    int ret;

    g_fd[vi_pipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open gpioi2c_ex error!\n");
        return HI_FAILURE;
    }
#else
    int ret;
    ISP_SNS_COMMBUS_U *imx327businfo = HI_NULL;
    imx327businfo = imx327_get_bus_Info(vi_pipe);
    u8DevNum = imx327businfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (IMX327_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int imx327_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int imx327_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}

int imx327_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = IMX327_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX327_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX327_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (IMX327_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (IMX327_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, IMX327_ADDR_BYTE + IMX327_DATA_BYTE);
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000); /* 1ms: 1000us */
    return;
}

void imx327_prog(VI_PIPE vi_pipe, const HI_U32 *rom)
{
    int i = 0;
    while (1) {
        HI_U32 lookup = rom[i++];
        HI_U32 addr = (lookup >> 16) & 0xFFFF; /* shift 16 */
        HI_U32 data = lookup & 0xFFFF;
        if (addr == 0xFFFE) {
            delay_ms(data);
        } else if (addr == 0xFFFF) {
            return;
        } else {
            imx327_write_register(vi_pipe, addr, data);
        }
    }
}

void imx327_standby(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += imx327_write_register(vi_pipe, 0x3000, 0x01);  /* STANDBY */
    ret += imx327_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void imx327_restart(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += imx327_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(20); /* 20ms */
    ret += imx327_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    ret += imx327_write_register(vi_pipe, 0x304b, 0x0a);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void imx327_wdr_1080p30_2to1_init(VI_PIPE vi_pipe);
void imx327_linear_1080p30_init(VI_PIPE vi_pipe);

void imx327_default_reg_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastimx327 = HI_NULL;
    pastimx327 = imx327_get_ctx(vi_pipe);
    for (i = 0; i < pastimx327->astRegsInfo[0].u32RegNum; i++) {
        ret += imx327_write_register(vi_pipe,
                                     pastimx327->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                     pastimx327->astRegsInfo[0].astI2cData[i].u32Data);
    }
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void imx327_init(VI_PIPE vi_pipe)
{
    WDR_MODE_E       enWDRMode;
    HI_BOOL          bInit;
    HI_U8            u8ImgMode;
    HI_S32 ret;
    ISP_SNS_STATE_S *pastimx327 = HI_NULL;
    pastimx327 = imx327_get_ctx(vi_pipe);
    bInit       = pastimx327->bInit;
    enWDRMode   = pastimx327->enWDRMode;
    u8ImgMode   = pastimx327->u8ImgMode;

    ret = imx327_i2c_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c init failed!\n");
        return;
    }
    /* When sensor first init, config all registers */
    if (bInit == HI_FALSE) {
        if (WDR_MODE_2To1_LINE == enWDRMode) {
            if (IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE == u8ImgMode) {  /* IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE */
                imx327_wdr_1080p30_2to1_init(vi_pipe);
            } else {
            }
        } else {
            imx327_linear_1080p30_init(vi_pipe);
        }
    } else {
        /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        if (WDR_MODE_2To1_LINE == enWDRMode) {
            if (IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE == u8ImgMode) {  /* IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE */
                imx327_wdr_1080p30_2to1_init(vi_pipe);
            } else {
            }
        } else {
            imx327_linear_1080p30_init(vi_pipe);
        }
    }

    pastimx327->bInit = HI_TRUE;
    return;
}

void imx327_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx327_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("IMX327 exit failed!\n");
    }
    return;
}

/* 1080P30 and 1080P25 */
void imx327_linear_1080p30_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += imx327_write_register(vi_pipe, 0x3000, 0x01);  /* STANDBY */
    ret += imx327_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */

    ret += imx327_write_register(vi_pipe, 0x3009, 0x02);
    ret += imx327_write_register(vi_pipe, 0x300a, 0xf0);
    ret += imx327_write_register(vi_pipe, 0x3011, 0x02);
    ret += imx327_write_register(vi_pipe, 0x3018, 0x65);  /* Vmax */
    ret += imx327_write_register(vi_pipe, 0x3019, 0x04);  /* Vmax */
    ret += imx327_write_register(vi_pipe, 0x301c, 0x30);  /* Hmax */
    ret += imx327_write_register(vi_pipe, 0x301d, 0x11);  /* Hmax */
    ret += imx327_write_register(vi_pipe, 0x3046, 0x01);  /* ODBIT */

    ret += imx327_write_register(vi_pipe, 0x305c, 0x18);  /* inck1 */
    ret += imx327_write_register(vi_pipe, 0x305d, 0x03);  /* inck2 */
    ret += imx327_write_register(vi_pipe, 0x305e, 0x20);  /* inck3 */
    ret += imx327_write_register(vi_pipe, 0x309e, 0x4a);
    ret += imx327_write_register(vi_pipe, 0x309f, 0x4a);
    ret += imx327_write_register(vi_pipe, 0x30d2, 0x19);
    ret += imx327_write_register(vi_pipe, 0x30d7, 0x03);

    ret += imx327_write_register(vi_pipe, 0x3129, 0x00);  /* ADBIT1 */
    ret += imx327_write_register(vi_pipe, 0x313b, 0x61);

    ret += imx327_write_register(vi_pipe, 0x315e, 0x1a);  /* inck5 */
    ret += imx327_write_register(vi_pipe, 0x3164, 0x1a);  /* inck6 */
    ret += imx327_write_register(vi_pipe, 0x317c, 0x00);  /* ADBIT2 */
    ret += imx327_write_register(vi_pipe, 0x31ec, 0x0e);  /* ADBIT3 */

    ret += imx327_write_register(vi_pipe, 0x3644, 0x20);
    ret += imx327_write_register(vi_pipe, 0x3645, 0x25);
    ret += imx327_write_register(vi_pipe, 0x3672, 0xa0);
    ret += imx327_write_register(vi_pipe, 0x3673, 0x07);
    ret += imx327_write_register(vi_pipe, 0x367b, 0x23);
    ret += imx327_write_register(vi_pipe, 0x3680, 0x49);

    imx327_default_reg_init(vi_pipe);

    ret += imx327_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(20); /* 20ms */
    ret += imx327_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    ret += imx327_write_register(vi_pipe, 0x304b, 0x0a);  /* XVSOUTSEL XHSOUTSEL */
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("===IMX327 1080P 30fps 12bit LINE Init OK!===\n");
    return;
}

void imx327_wdr_1080p30_2to1_init(VI_PIPE vi_pipe)
{
    /* 10bit Register */
    HI_S32 ret = HI_SUCCESS;
    ret += imx327_write_register(vi_pipe, 0x3000, 0x01);  /* standby */
    ret += imx327_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */
    ret += imx327_write_register(vi_pipe, 0x3005, 0x00);  /* 0: 10Bit; 0x01: 12Bit */
    ret += imx327_write_register(vi_pipe, 0x3007, 0x40);  /* WINMODE: 1080P */
    ret += imx327_write_register(vi_pipe, 0x3009, 0x01);
    ret += imx327_write_register(vi_pipe, 0x300a, 0x3C);  /* BLKLEVEL */
    ret += imx327_write_register(vi_pipe, 0x300c, 0x11);
    ret += imx327_write_register(vi_pipe, 0x3011, 0x02);  /* Change after reset; */
    ret += imx327_write_register(vi_pipe, 0x3014, 0x02);  /* Gain */
    ret += imx327_write_register(vi_pipe, 0x3018, 0xc4);  /* VMAX[7:0] */
    ret += imx327_write_register(vi_pipe, 0x3019, 0x04);  /* VMAX[15:8] */
    ret += imx327_write_register(vi_pipe, 0x301a, 0x00);  /* VMAX[17:16] */
    ret += imx327_write_register(vi_pipe, 0x301c, 0xec);  /* HMAX[7:0] */
    ret += imx327_write_register(vi_pipe, 0x301d, 0x07);  /* HMAX[15:8] */
    ret += imx327_write_register(vi_pipe, 0x3020, 0x02);  /* SHS1 */
    ret += imx327_write_register(vi_pipe, 0x3021, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3022, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3024, 0x53);  /* SHS2 */
    ret += imx327_write_register(vi_pipe, 0x3025, 0x04);
    ret += imx327_write_register(vi_pipe, 0x3026, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3030, 0xE1);  /* RHS1 */
    ret += imx327_write_register(vi_pipe, 0x3031, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3032, 0x00);
    ret += imx327_write_register(vi_pipe, 0x303A, 0x08);
    ret += imx327_write_register(vi_pipe, 0x303C, 0x04);
    ret += imx327_write_register(vi_pipe, 0x303D, 0x00);
    ret += imx327_write_register(vi_pipe, 0x303E, 0x41);
    ret += imx327_write_register(vi_pipe, 0x303F, 0x04);
    ret += imx327_write_register(vi_pipe, 0x3045, 0x05);
    ret += imx327_write_register(vi_pipe, 0x3046, 0x00); /* OPORTSE&ODBIT */
    ret += imx327_write_register(vi_pipe, 0x305c, 0x18); /* INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C */
    ret += imx327_write_register(vi_pipe, 0x305d, 0x03); /* INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03 */
    ret += imx327_write_register(vi_pipe, 0x305e, 0x20); /* INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10 */
    ret += imx327_write_register(vi_pipe, 0x305f, 0x01); /* INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01 */
    ret += imx327_write_register(vi_pipe, 0x309e, 0x4a);
    ret += imx327_write_register(vi_pipe, 0x309f, 0x4a);
    ret += imx327_write_register(vi_pipe, 0x30d2, 0x19);
    ret += imx327_write_register(vi_pipe, 0x30d7, 0x03);
    ret += imx327_write_register(vi_pipe, 0x3106, 0x11);
    ret += imx327_write_register(vi_pipe, 0x3129, 0x1D); /* ADBIT1,12Bit;0x1D->10Bit */
    ret += imx327_write_register(vi_pipe, 0x313b, 0x61);
    ret += imx327_write_register(vi_pipe, 0x315e, 0x1a); /* INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B */
    ret += imx327_write_register(vi_pipe, 0x3164, 0x1a); /* INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B */
    ret += imx327_write_register(vi_pipe, 0x317c, 0x12); /* ADBIT2,12Bit;0x12->10Bit */
    ret += imx327_write_register(vi_pipe, 0x31ec, 0x37); /* ADBIT3,12Bit;0x37->10Bit */
    ret += imx327_write_register(vi_pipe, 0x3480, 0x49); /* INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92 */

    /* MIPI setting */
    ret += imx327_write_register(vi_pipe, 0x3405, 0x10);
    ret += imx327_write_register(vi_pipe, 0x3407, 0x03);
    ret += imx327_write_register(vi_pipe, 0x3414, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3415, 0x00);

    ret += imx327_write_register(vi_pipe, 0x3418, 0x7a); /* Y_OUT_SIZE */
    ret += imx327_write_register(vi_pipe, 0x3419, 0x09);
    ret += imx327_write_register(vi_pipe, 0x3441, 0x0a);
    ret += imx327_write_register(vi_pipe, 0x3442, 0x0a);
    ret += imx327_write_register(vi_pipe, 0x3443, 0x03);
    ret += imx327_write_register(vi_pipe, 0x3444, 0x20);
    ret += imx327_write_register(vi_pipe, 0x3445, 0x25);
    ret += imx327_write_register(vi_pipe, 0x3446, 0x57);
    ret += imx327_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3448, 0x37);
    ret += imx327_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx327_write_register(vi_pipe, 0x344a, 0x1f);
    ret += imx327_write_register(vi_pipe, 0x344b, 0x00);
    ret += imx327_write_register(vi_pipe, 0x344c, 0x1f);
    ret += imx327_write_register(vi_pipe, 0x344d, 0x00);
    ret += imx327_write_register(vi_pipe, 0x344e, 0x1f);
    ret += imx327_write_register(vi_pipe, 0x344f, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3450, 0x77);
    ret += imx327_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3452, 0x1f);
    ret += imx327_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx327_write_register(vi_pipe, 0x3454, 0x17);
    ret += imx327_write_register(vi_pipe, 0x3455, 0x00);

    ret += imx327_write_register(vi_pipe, 0x3472, 0xA0);
    ret += imx327_write_register(vi_pipe, 0x3473, 0x07);
    ret += imx327_write_register(vi_pipe, 0x347b, 0x23);

    imx327_default_reg_init(vi_pipe);

    ret += imx327_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(20); /* 20ms */
    ret += imx327_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    ret += imx327_write_register(vi_pipe, 0x304b, 0x0a);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("===Imx327 sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;
}
