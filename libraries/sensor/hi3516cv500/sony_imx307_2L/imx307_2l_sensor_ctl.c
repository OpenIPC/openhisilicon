/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx307_2l_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx307_2l_cmos.h"
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

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};
static unsigned char g_serdes_i2c_addr[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 0x34};

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

unsigned char imx307_2l_get_serdes_i2c_addr(VI_PIPE vi_pipe)
{
    return g_serdes_i2c_addr[vi_pipe];
}

int imx307_2l_set_bus_ex_info(VI_PIPE vi_pipe, ISP_SNS_BUS_EX_S *serdes_info)
{
    g_serdes_i2c_addr[vi_pipe] = serdes_info->bus_addr;

    SNS_ERR_TRACE("---->pipe[%d] bus_ex_addr[0x%x]!\n", vi_pipe, serdes_info->bus_addr);

    return HI_SUCCESS;
}

int imx307_2l_i2c_init(VI_PIPE vi_pipe)
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
    ISP_SNS_COMMBUS_U *imx3072lbusinfo = HI_NULL;
    imx3072lbusinfo = imx307_2l_get_bus_Info(vi_pipe);
    u8DevNum = imx3072lbusinfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (g_serdes_i2c_addr[vi_pipe] >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int imx307_2l_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int imx307_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}

int imx307_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = IMX307_2L_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX307_2L_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX307_2L_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (IMX307_2L_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (IMX307_2L_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, IMX307_2L_ADDR_BYTE + IMX307_2L_DATA_BYTE);
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);  /* 1ms: 1000us */
    return;
}

void imx307_2l_standby(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x01);  /* STANDBY */
    ret += imx307_2l_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void imx307_2l_restart(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(20);  /* delay 20 ms */
    ret += imx307_2l_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    ret += imx307_2l_write_register(vi_pipe, 0x304b, 0x0a);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

#define IMX307_SENSOR_1080P_30FPS_LINEAR_MODE  0
#define IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE 1
#define IMX307_SENSOR_1080P_60FPS_LINEAR_MODE  2


void imx307_2l_wdr_1080p30_2to1_init(VI_PIPE vi_pipe);
void imx307_2l_linear_1080p30_init(VI_PIPE vi_pipe);
void imx307_2l_linear_1080p60_init(VI_PIPE vi_pipe);

void imx307_2l_default_reg_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastimx3072l = HI_NULL;
    pastimx3072l = imx307_2l_get_ctx(vi_pipe);
    for (i = 0; i < pastimx3072l->astRegsInfo[0].u32RegNum; i++) {
        ret += imx307_2l_write_register(vi_pipe,
                                        pastimx3072l->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                        pastimx3072l->astRegsInfo[0].astI2cData[i].u32Data);
    }

    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void imx307_2l_init(VI_PIPE vi_pipe)
{
    WDR_MODE_E       enWDRMode;
    HI_U8            u8ImgMode;
    HI_S32           ret;
    ISP_SNS_STATE_S *pastimx3072l = HI_NULL;
    pastimx3072l = imx307_2l_get_ctx(vi_pipe);
    enWDRMode   = pastimx3072l->enWDRMode;
    u8ImgMode   = pastimx3072l->u8ImgMode;

    ret = imx307_2l_i2c_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c init failed!\n");
    }
    /* When sensor first init, config all registers */
    if (WDR_MODE_2To1_LINE == enWDRMode) {
        if (u8ImgMode == IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE) {  /* IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE */
            imx307_2l_wdr_1080p30_2to1_init(vi_pipe);
        } else {
        }
    } else {
        if (u8ImgMode == IMX307_SENSOR_1080P_30FPS_LINEAR_MODE) {
            imx307_2l_linear_1080p30_init(vi_pipe);
        } else if (u8ImgMode == IMX307_SENSOR_1080P_60FPS_LINEAR_MODE) {
            imx307_2l_linear_1080p60_init(vi_pipe);
        }
    }

    pastimx3072l->bInit = HI_TRUE;
    return;
}

void imx307_2l_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx307_2l_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
    }
    return;
}

/* 1080P30 and 1080P25 */
void imx307_2l_linear_1080p30_init(VI_PIPE vi_pipe)
{
    /* Enter Standby */
    HI_S32 ret = HI_SUCCESS;
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x01); /* Standby mode */
    ret += imx307_2l_write_register(vi_pipe, 0x3002, 0x00); /* XMSTA */
    ret += imx307_2l_write_register(vi_pipe, 0x3005, 0x00); /* ADBIT */
    ret += imx307_2l_write_register(vi_pipe, 0x3007, 0x00); /* VREVERSE & HREVERSE & WINMODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3009, 0x02); /* FRSEL & FDG_SEL */
    ret += imx307_2l_write_register(vi_pipe, 0x300A, 0x3C); /* BLKLEVEL */
    ret += imx307_2l_write_register(vi_pipe, 0x3011, 0x0A);
    ret += imx307_2l_write_register(vi_pipe, 0x3018, 0x65); /* VMAX */
    ret += imx307_2l_write_register(vi_pipe, 0x3019, 0x04); /* VMAX */
    ret += imx307_2l_write_register(vi_pipe, 0x301C, 0x30); /* HMAX; */
    ret += imx307_2l_write_register(vi_pipe, 0x301D, 0x11); /* HMAX; */
    ret += imx307_2l_write_register(vi_pipe, 0x3046, 0x00); /* ODBIT & OPORTSEL */
    ret += imx307_2l_write_register(vi_pipe, 0x304B, 0x0A); /* XVSOUTSEL & XHSOUTSEL */
    ret += imx307_2l_write_register(vi_pipe, 0x305C, 0x18); /* INCKSEL1 37.125MHz */
    ret += imx307_2l_write_register(vi_pipe, 0x305D, 0x03); /* INCKSEL2 */
    ret += imx307_2l_write_register(vi_pipe, 0x305E, 0x20);
    ret += imx307_2l_write_register(vi_pipe, 0x305F, 0x01);
    ret += imx307_2l_write_register(vi_pipe, 0x309E, 0x4A);
    ret += imx307_2l_write_register(vi_pipe, 0x309F, 0x4A);

    ret += imx307_2l_write_register(vi_pipe, 0x311C, 0x0E);
    ret += imx307_2l_write_register(vi_pipe, 0x3128, 0x04);
    ret += imx307_2l_write_register(vi_pipe, 0x3129, 0x1D); /* ADBIT1 */
    ret += imx307_2l_write_register(vi_pipe, 0x313B, 0x41);
    ret += imx307_2l_write_register(vi_pipe, 0x315E, 0x1A); /* INCKSEL5 37.125MHz INCK5 Setting */
    ret += imx307_2l_write_register(vi_pipe, 0x3164, 0x1A); /* INCKSEL6 37.125MHz */
    ret += imx307_2l_write_register(vi_pipe, 0x317C, 0x12); /* ADBIT2 */
    ret += imx307_2l_write_register(vi_pipe, 0x31EC, 0x37); /* ADBIT3 */

    ret += imx307_2l_write_register(vi_pipe, 0x3405, 0x10); /* REPETITION */
    ret += imx307_2l_write_register(vi_pipe, 0x3407, 0x01); /* PHYSICAL_LANE_NUM */
    ret += imx307_2l_write_register(vi_pipe, 0x3414, 0x0A); /* OPB_SIZE_V */
    ret += imx307_2l_write_register(vi_pipe, 0x3418, 0x49); /* Y_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3419, 0x04); /* Y_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3441, 0x0A); /* CSI_DT_FMT */
    ret += imx307_2l_write_register(vi_pipe, 0x3442, 0x0A); /* CSI_DT_FMT */
    ret += imx307_2l_write_register(vi_pipe, 0x3443, 0x01); /* CSI_LANE_MODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3444, 0x20); /* EXTCK_FREQ */
    ret += imx307_2l_write_register(vi_pipe, 0x3445, 0x25); /* EXTCK_FREQ */
    ret += imx307_2l_write_register(vi_pipe, 0x3446, 0x57); /* TCLKPOST */
    ret += imx307_2l_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3448, 0x37); /* THSZERO */
    ret += imx307_2l_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344A, 0x1F); /* THSPREPARE */
    ret += imx307_2l_write_register(vi_pipe, 0x344B, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344C, 0x1F); /* TCLKTRAIL */
    ret += imx307_2l_write_register(vi_pipe, 0x344D, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344E, 0x1F); /* THSTRAIL */
    ret += imx307_2l_write_register(vi_pipe, 0x344F, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3450, 0x77); /* TCLKZERO */
    ret += imx307_2l_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3452, 0x1F); /* TCLKPREPARE */
    ret += imx307_2l_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3454, 0x17); /* TLPX */
    ret += imx307_2l_write_register(vi_pipe, 0x3455, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3472, 0x9C); /* X_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3473, 0x07); /* X_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3480, 0x49); /* INCKSEL7 */

    imx307_2l_default_reg_init(vi_pipe);

    /* Standby Cancel */
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x00);    /* standby */
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("==============================================================\n");
    printf("=====Sony imx307_2l sensor 1080P30fps(MIPI) init success!=====\n");
    printf("==============================================================\n");

    return;
}

void imx307_2l_wdr_1080p30_2to1_init(VI_PIPE vi_pipe)
{
    /* 10bit */
    HI_S32 ret = HI_SUCCESS;
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x01); /* Standby mode */
    ret += imx307_2l_write_register(vi_pipe, 0x3002, 0x01); /* XMSTA */
    ret += imx307_2l_write_register(vi_pipe, 0x3005, 0x00); /* ADBIT */
    ret += imx307_2l_write_register(vi_pipe, 0x3007, 0x40); /* VREVERSE & HREVERSE & WINMODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3009, 0x01); /* FRSEL & FDG_SEL */
    ret += imx307_2l_write_register(vi_pipe, 0x300A, 0x3C); /* BLKLEVEL */
    ret += imx307_2l_write_register(vi_pipe, 0x300C, 0x11);
    ret += imx307_2l_write_register(vi_pipe, 0x3011, 0x02); /*  Change after reset; */
    ret += imx307_2l_write_register(vi_pipe, 0x3018, 0xC4); /*  VMAX[7:0] */
    ret += imx307_2l_write_register(vi_pipe, 0x3019, 0x04); /*  VMAX[15:8] */
    ret += imx307_2l_write_register(vi_pipe, 0x301C, 0xEC); /*  HMAX[7:0]      0x14a0->25fps; */
    ret += imx307_2l_write_register(vi_pipe, 0x301D, 0x07); /*  HMAX[15:8]     0x1130->30fps; */

    ret += imx307_2l_write_register(vi_pipe, 0x3020, 0x02); /*  SHS1 */
    ret += imx307_2l_write_register(vi_pipe, 0x3021, 0x00); /*  SHS1 */
    ret += imx307_2l_write_register(vi_pipe, 0x3024, 0x53); /*  SHS2 */
    ret += imx307_2l_write_register(vi_pipe, 0x3025, 0x04); /*  SHS2 */
    ret += imx307_2l_write_register(vi_pipe, 0x3030, 0xE1); /*  RHS1 */
    ret += imx307_2l_write_register(vi_pipe, 0x3031, 0x00); /*  RHS1 */
    ret += imx307_2l_write_register(vi_pipe, 0x303A, 0x08);          /* add */
    ret += imx307_2l_write_register(vi_pipe, 0x303C, 0x04); /*  WINPV  add */
    ret += imx307_2l_write_register(vi_pipe, 0x303D, 0x00);          /* add */
    ret += imx307_2l_write_register(vi_pipe, 0x303E, 0x41); /*  WINWV   add */
    ret += imx307_2l_write_register(vi_pipe, 0x303F, 0x04);          /* add */
    ret += imx307_2l_write_register(vi_pipe, 0x3045, 0x05); /*  DOLSCDEN & DOLSYDINFOEN & HINFOEN */
    ret += imx307_2l_write_register(vi_pipe, 0x3046, 0x00); /*  OPORTSEL & ODBIT */

    ret += imx307_2l_write_register(vi_pipe, 0x305C, 0x18); /*  INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C */
    ret += imx307_2l_write_register(vi_pipe, 0x305D, 0x03); /*  INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03 */
    ret += imx307_2l_write_register(vi_pipe, 0x305E, 0x20); /*  INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10 */
    ret += imx307_2l_write_register(vi_pipe, 0x305F, 0x01); /*  INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01 */

    ret += imx307_2l_write_register(vi_pipe, 0x309E, 0x4A);
    ret += imx307_2l_write_register(vi_pipe, 0x309F, 0x4A);
    ret += imx307_2l_write_register(vi_pipe, 0x30D2, 0x19); /* add */
    ret += imx307_2l_write_register(vi_pipe, 0x30D7, 0x03); /* add */
    ret += imx307_2l_write_register(vi_pipe, 0x3106, 0x11);
    ret += imx307_2l_write_register(vi_pipe, 0x311C, 0x0E);
    ret += imx307_2l_write_register(vi_pipe, 0x3128, 0x04);
    ret += imx307_2l_write_register(vi_pipe, 0x3129, 0x1D); /*  ADBIT1,12Bit; 0x1D->10Bit; */
    ret += imx307_2l_write_register(vi_pipe, 0x313B, 0x41);

    ret += imx307_2l_write_register(vi_pipe, 0x315E, 0x1A); /*  INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B */
    ret += imx307_2l_write_register(vi_pipe, 0x3164, 0x1A); /*  INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B */
    ret += imx307_2l_write_register(vi_pipe, 0x317C, 0x12); /*  ADBIT2,12Bit;0x12->10Bit; */
    ret += imx307_2l_write_register(vi_pipe, 0x31EC, 0x37); /*  ADBIT3,12Bit;0x37->10Bit; */

    ret += imx307_2l_write_register(vi_pipe, 0x3480, 0x49); /*  INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92 */
    /* ##MIPI setting */
    ret += imx307_2l_write_register(vi_pipe, 0x3405, 0x00); /*  REPETITION */
    ret += imx307_2l_write_register(vi_pipe, 0x3407, 0x01); /*  VREVERSE & HREVERSE & WINMODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3414, 0x0A); /*  OPB_SIZE_V */
    ret += imx307_2l_write_register(vi_pipe, 0x3415, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3418, 0x9C); /*  Y_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3419, 0x08); /*  Y_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3441, 0x0A); /*  CSI_DT_FMT 10Bit */
    ret += imx307_2l_write_register(vi_pipe, 0x3442, 0x0A);
    ret += imx307_2l_write_register(vi_pipe, 0x3443, 0x01); /*  CSI_LANE_MODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3444, 0x20); /*  EXTCK_FREQ */
    ret += imx307_2l_write_register(vi_pipe, 0x3445, 0x25); /*  EXTCK_FREQ */
    ret += imx307_2l_write_register(vi_pipe, 0x3446, 0x77); /*  TCLKPOST */
    ret += imx307_2l_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3448, 0x67); /*  THSZERO */
    ret += imx307_2l_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344A, 0x47); /*  THSPREPARE */
    ret += imx307_2l_write_register(vi_pipe, 0x344B, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344C, 0x37); /*  TCLKTRAIL */
    ret += imx307_2l_write_register(vi_pipe, 0x344D, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344E, 0x3F); /*  THSTRAIL */
    ret += imx307_2l_write_register(vi_pipe, 0x344F, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3450, 0xFF); /*  TCLKZERO */
    ret += imx307_2l_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3452, 0x3F); /*  TCLKPREPARE */
    ret += imx307_2l_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3454, 0x37); /*  TLPX */
    ret += imx307_2l_write_register(vi_pipe, 0x3455, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3472, 0xA0); /*  X_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3473, 0x07);
    ret += imx307_2l_write_register(vi_pipe, 0x347B, 0x23);

    imx307_2l_default_reg_init(vi_pipe);

    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x00); /*  Standby Cancel */
    delay_ms(20);  /* delay 20 ms */
    ret += imx307_2l_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    ret += imx307_2l_write_register(vi_pipe, 0x304b, 0x0a);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("============================================================================\n");
    printf("===Imx307_2l sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!===\n");
    printf("============================================================================\n");

    return;
}

/* 1080P60 */
void imx307_2l_linear_1080p60_init(VI_PIPE vi_pipe)
{
    /* Enter Standby */
    HI_S32 ret = HI_SUCCESS;
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x01); /* Standby mode */
    ret += imx307_2l_write_register(vi_pipe, 0x3002, 0x00); /* XMSTA */
    ret += imx307_2l_write_register(vi_pipe, 0x3005, 0x00); /* ADBIT */
    ret += imx307_2l_write_register(vi_pipe, 0x3007, 0x00); /* VREVERSE & HREVERSE & WINMODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3009, 0x01); /* FRSEL & FDG_SEL */
    ret += imx307_2l_write_register(vi_pipe, 0x300A, 0x3C); /* BLKLEVEL */
    ret += imx307_2l_write_register(vi_pipe, 0x3011, 0x0A);
    ret += imx307_2l_write_register(vi_pipe, 0x3018, 0x65); /* VMAX */
    ret += imx307_2l_write_register(vi_pipe, 0x3019, 0x04); /* VMAX */
    ret += imx307_2l_write_register(vi_pipe, 0x301C, 0x98); /* HMAX; */
    ret += imx307_2l_write_register(vi_pipe, 0x301D, 0x08); /* HMAX; */
    ret += imx307_2l_write_register(vi_pipe, 0x3046, 0x00); /* ODBIT & OPORTSEL */
    ret += imx307_2l_write_register(vi_pipe, 0x304B, 0x0A); /* XVSOUTSEL & XHSOUTSEL */
    ret += imx307_2l_write_register(vi_pipe, 0x305C, 0x18); /* INCKSEL1 37.125MHz */
    ret += imx307_2l_write_register(vi_pipe, 0x305D, 0x03); /* INCKSEL2 */
    ret += imx307_2l_write_register(vi_pipe, 0x305E, 0x20);
    ret += imx307_2l_write_register(vi_pipe, 0x305F, 0x01);
    ret += imx307_2l_write_register(vi_pipe, 0x309E, 0x4A);
    ret += imx307_2l_write_register(vi_pipe, 0x309F, 0x4A);

    ret += imx307_2l_write_register(vi_pipe, 0x311C, 0x0E);
    ret += imx307_2l_write_register(vi_pipe, 0x3128, 0x04);
    ret += imx307_2l_write_register(vi_pipe, 0x3129, 0x1D); /* ADBIT1 */
    ret += imx307_2l_write_register(vi_pipe, 0x313B, 0x41);
    ret += imx307_2l_write_register(vi_pipe, 0x315E, 0x1A); /* INCKSEL5 37.125MHz INCK5 Setting */
    ret += imx307_2l_write_register(vi_pipe, 0x3164, 0x1A); /* INCKSEL6 37.125MHz */
    ret += imx307_2l_write_register(vi_pipe, 0x317C, 0x12); /* ADBIT2 */
    ret += imx307_2l_write_register(vi_pipe, 0x31EC, 0x37); /* ADBIT3 */

    ret += imx307_2l_write_register(vi_pipe, 0x3405, 0x00); /* REPETITION */
    ret += imx307_2l_write_register(vi_pipe, 0x3407, 0x01); /* PHYSICAL_LANE_NUM */
    ret += imx307_2l_write_register(vi_pipe, 0x3414, 0x0A); /* OPB_SIZE_V */
    ret += imx307_2l_write_register(vi_pipe, 0x3418, 0x49); /* Y_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3419, 0x04); /* Y_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3441, 0x0A); /* CSI_DT_FMT */
    ret += imx307_2l_write_register(vi_pipe, 0x3442, 0x0A); /* CSI_DT_FMT */
    ret += imx307_2l_write_register(vi_pipe, 0x3443, 0x01); /* CSI_LANE_MODE */
    ret += imx307_2l_write_register(vi_pipe, 0x3444, 0x20); /* EXTCK_FREQ */
    ret += imx307_2l_write_register(vi_pipe, 0x3445, 0x25); /* EXTCK_FREQ */
    ret += imx307_2l_write_register(vi_pipe, 0x3446, 0x77); /* TCLKPOST */
    ret += imx307_2l_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3448, 0x67); /* THSZERO */
    ret += imx307_2l_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344A, 0x47); /* THSPREPARE */
    ret += imx307_2l_write_register(vi_pipe, 0x344B, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344C, 0x37); /* TCLKTRAIL */
    ret += imx307_2l_write_register(vi_pipe, 0x344D, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x344E, 0x3F); /* THSTRAIL */
    ret += imx307_2l_write_register(vi_pipe, 0x344F, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3450, 0xFF); /* TCLKZERO */
    ret += imx307_2l_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3452, 0x3F); /* TCLKPREPARE */
    ret += imx307_2l_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3454, 0x37); /* TLPX */
    ret += imx307_2l_write_register(vi_pipe, 0x3455, 0x00);
    ret += imx307_2l_write_register(vi_pipe, 0x3472, 0x9C); /* X_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3473, 0x07); /* X_OUT_SIZE */
    ret += imx307_2l_write_register(vi_pipe, 0x3480, 0x49); /* INCKSEL7 */

    imx307_2l_default_reg_init(vi_pipe);

    /* Standby Cancel */
    ret += imx307_2l_write_register(vi_pipe, 0x3000, 0x00);    /* standby */
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("==============================================================\n");
    printf("=====Sony imx307_2l sensor 1080P60fps(MIPI) init success!=====\n");
    printf("==============================================================\n");
    return;
}
