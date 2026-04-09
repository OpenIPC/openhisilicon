/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx335_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx335_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <memory.h>

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};
#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
int IMX335_i2c_init(VI_PIPE vi_pipe)
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
    ISP_SNS_COMMBUS_U *imx335businfo = HI_NULL;
    imx335businfo = imx335_get_bus_Info(vi_pipe);
    u8DevNum = imx335businfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (IMX335_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int IMX335_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int IMX335_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}
int IMX335_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = IMX335_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX335_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX335_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (IMX335_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (IMX335_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, IMX335_ADDR_BYTE + IMX335_DATA_BYTE);
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

void IMX335_standby(VI_PIPE vi_pipe)
{
    return;
}

void IMX335_restart(VI_PIPE vi_pipe)
{
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);  /* 1ms: 1000us */
    return;
}

void IMX335_linear_5M30_12bit_init(VI_PIPE vi_pipe);
void IMX335_wdr_5M30_10bit_init(VI_PIPE vi_pipe);

void IMX335_default_reg_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastimx335 = HI_NULL;
    pastimx335 = imx335_get_ctx(vi_pipe);
    for (i = 0; i < pastimx335->astRegsInfo[0].u32RegNum; i++) {
        ret += IMX335_write_register(vi_pipe,
                                     pastimx335->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                     pastimx335->astRegsInfo[0].astI2cData[i].u32Data);
    }

    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void IMX335_init(VI_PIPE vi_pipe)
{
    WDR_MODE_E       enWDRMode;
    HI_BOOL          bInit;
    HI_U8            u8ImgMode;
    ISP_SNS_STATE_S *pastimx335 = HI_NULL;
    pastimx335 = imx335_get_ctx(vi_pipe);
    bInit       = pastimx335->bInit;
    enWDRMode   = pastimx335->enWDRMode;
    u8ImgMode   = pastimx335->u8ImgMode;

    if (bInit == HI_FALSE) {
        /* sensor i2c init */
        IMX335_i2c_init(vi_pipe);
    }

    /* When sensor first init, config all registers */
    if (WDR_MODE_2To1_LINE == enWDRMode) {
        if (u8ImgMode == IMX335_5M_30FPS_10BIT_WDR_MODE) {
            IMX335_wdr_5M30_10bit_init(vi_pipe);
        } else {
            printf("IMX335_init Not support this mode\n");
            return;
        }
    } else if (enWDRMode == WDR_MODE_NONE) {
        IMX335_linear_5M30_12bit_init(vi_pipe);
    } else {
        printf("IMX335_init Not support this mode\n");
        return;
    }

    pastimx335->bInit = HI_TRUE;

    return;
}

void IMX335_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = IMX335_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
    }
    return;
}

void IMX335_linear_5M30_12bit_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret +=  IMX335_write_register(vi_pipe, 0x3000, 0x01); /* standby */
    ret +=  IMX335_write_register(vi_pipe, 0x3001, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3002, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3003, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x300C, 0x5B);
    ret +=  IMX335_write_register(vi_pipe, 0x300D, 0x40);
    ret +=  IMX335_write_register(vi_pipe, 0x3030, 0x94); /* VMAX_LOW */
    ret +=  IMX335_write_register(vi_pipe, 0x3031, 0x11); /* VMAX_MIDDLE */
    ret +=  IMX335_write_register(vi_pipe, 0x3032, 0x00); /* VMAX_HIGH */

    ret +=  IMX335_write_register(vi_pipe, 0x3034, 0x26); /* HMAX */
    ret +=  IMX335_write_register(vi_pipe, 0x3035, 0x02);

    ret +=  IMX335_write_register(vi_pipe, 0x3058, 0x09); /* SHR0 */
    ret +=  IMX335_write_register(vi_pipe, 0x3059, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x305A, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x30E8, 0x00); /* Gain */
    ret +=  IMX335_write_register(vi_pipe, 0x30E9, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x315A, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x316A, 0x7E);

    ret +=  IMX335_write_register(vi_pipe, 0x319D, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x319E, 0x01);

    ret +=  IMX335_write_register(vi_pipe, 0x3288, 0x21);
    ret +=  IMX335_write_register(vi_pipe, 0x328A, 0x02);

    ret +=  IMX335_write_register(vi_pipe, 0x3414, 0x05);
    ret +=  IMX335_write_register(vi_pipe, 0x3416, 0x18);

    ret +=  IMX335_write_register(vi_pipe, 0x341C, 0x47);
    ret +=  IMX335_write_register(vi_pipe, 0x341D, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x3648, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x364A, 0x04);
    ret +=  IMX335_write_register(vi_pipe, 0x364C, 0x04);

    ret +=  IMX335_write_register(vi_pipe, 0x3678, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x367C, 0x31);
    ret +=  IMX335_write_register(vi_pipe, 0x367E, 0x31);

    ret +=  IMX335_write_register(vi_pipe, 0x3706, 0x10);
    ret +=  IMX335_write_register(vi_pipe, 0x3708, 0x03);

    ret +=  IMX335_write_register(vi_pipe, 0x3714, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3715, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3716, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3717, 0x03);
    ret +=  IMX335_write_register(vi_pipe, 0x371C, 0x3D);
    ret +=  IMX335_write_register(vi_pipe, 0x371D, 0x3F);

    ret +=  IMX335_write_register(vi_pipe, 0x372C, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x372D, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x372E, 0x46);
    ret +=  IMX335_write_register(vi_pipe, 0x372F, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3730, 0x89);
    ret +=  IMX335_write_register(vi_pipe, 0x3731, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3732, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x3733, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3734, 0xFE);
    ret +=  IMX335_write_register(vi_pipe, 0x3735, 0x05);

    ret +=  IMX335_write_register(vi_pipe, 0x3740, 0x02);

    ret +=  IMX335_write_register(vi_pipe, 0x375D, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x375E, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x375F, 0x11);
    ret +=  IMX335_write_register(vi_pipe, 0x3760, 0x01);

    ret +=  IMX335_write_register(vi_pipe, 0x3768, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x3769, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x376A, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x376B, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x376C, 0x1A);
    ret +=  IMX335_write_register(vi_pipe, 0x376D, 0x17);
    ret +=  IMX335_write_register(vi_pipe, 0x376E, 0x0F);

    ret +=  IMX335_write_register(vi_pipe, 0x3776, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3777, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3778, 0x46);
    ret +=  IMX335_write_register(vi_pipe, 0x3779, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x377A, 0x89);
    ret +=  IMX335_write_register(vi_pipe, 0x377B, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x377C, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x377D, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x377E, 0x23);
    ret +=  IMX335_write_register(vi_pipe, 0x377F, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3780, 0xD9);
    ret +=  IMX335_write_register(vi_pipe, 0x3781, 0x03);
    ret +=  IMX335_write_register(vi_pipe, 0x3782, 0xF5);
    ret +=  IMX335_write_register(vi_pipe, 0x3783, 0x06);
    ret +=  IMX335_write_register(vi_pipe, 0x3784, 0xA5);
    ret +=  IMX335_write_register(vi_pipe, 0x3788, 0x0F);
    ret +=  IMX335_write_register(vi_pipe, 0x378A, 0xD9);
    ret +=  IMX335_write_register(vi_pipe, 0x378B, 0x03);
    ret +=  IMX335_write_register(vi_pipe, 0x378C, 0xEB);
    ret +=  IMX335_write_register(vi_pipe, 0x378D, 0x05);
    ret +=  IMX335_write_register(vi_pipe, 0x378E, 0x87);
    ret +=  IMX335_write_register(vi_pipe, 0x378F, 0x06);
    ret +=  IMX335_write_register(vi_pipe, 0x3790, 0xF5);
    ret +=  IMX335_write_register(vi_pipe, 0x3792, 0x43);
    ret +=  IMX335_write_register(vi_pipe, 0x3794, 0x7A);
    ret +=  IMX335_write_register(vi_pipe, 0x3796, 0xA1);

    IMX335_default_reg_init(vi_pipe);

    ret +=  IMX335_write_register(vi_pipe, 0x3000, 0x00); /* Standby Cancel */
    delay_ms(18);  /* delay 18 ms */
    ret +=  IMX335_write_register(vi_pipe, 0x3002, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("-------Sony IMX335_init_5M_2592x1944_12bit_linear30 Initial OK!-------\n");
    return;
}

void IMX335_wdr_5M30_10bit_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret +=  IMX335_write_register(vi_pipe, 0x3000, 0x01); /* standby */
    ret +=  IMX335_write_register(vi_pipe, 0x3001, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3002, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3003, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3004, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x300C, 0x5B); /* BCWAIT_TIME */
    ret +=  IMX335_write_register(vi_pipe, 0x300D, 0x40); /* CPWAIT_TIME */
    ret +=  IMX335_write_register(vi_pipe, 0x3018, 0x00); /* WINMODE */
    ret +=  IMX335_write_register(vi_pipe, 0x302C, 0x30);  /* HTRIMMING_START */
    ret +=  IMX335_write_register(vi_pipe, 0x302D, 0x00);  /* HTRIMMING_START */
    ret +=  IMX335_write_register(vi_pipe, 0x302E, 0x38); /* HNUM */
    ret +=  IMX335_write_register(vi_pipe, 0x302F, 0x0A); /* HNUM */

    ret +=  IMX335_write_register(vi_pipe, 0x3030, 0x94); /* VMAX_LOW */
    ret +=  IMX335_write_register(vi_pipe, 0x3031, 0x11); /* VMAX_MIDDLE */
    ret +=  IMX335_write_register(vi_pipe, 0x3032, 0x00); /* VMAX_HIGH */

    ret +=  IMX335_write_register(vi_pipe, 0x3034, 0x13); /* HMAX */
    ret +=  IMX335_write_register(vi_pipe, 0x3035, 0x01);

    ret +=  IMX335_write_register(vi_pipe, 0x3048, 0x01); /* WDMODE */
    ret +=  IMX335_write_register(vi_pipe, 0x3049, 0x01); /* WDSEL */
    ret +=  IMX335_write_register(vi_pipe, 0x304A, 0x04); /* WD_SET1 */
    ret +=  IMX335_write_register(vi_pipe, 0x304B, 0x03); /* WD_SET2 */
    ret +=  IMX335_write_register(vi_pipe, 0x304C, 0x13); /* OPB_SIZE_V */
    ret +=  IMX335_write_register(vi_pipe, 0x304E, 0x00); /* HREVERSE */
    ret +=  IMX335_write_register(vi_pipe, 0x304F, 0x00); /* VREVERSE */

    ret +=  IMX335_write_register(vi_pipe, 0x3050, 0x00); /* ADBIT */

    ret +=  IMX335_write_register(vi_pipe, 0x3056, 0xAC); /* Y_OUT_SIZE_LOW */
    ret +=  IMX335_write_register(vi_pipe, 0x3057, 0x07); /* Y_OUT_SIZE_HIGH */

    ret +=  IMX335_write_register(vi_pipe, 0x3058, 0x60); /* SHR0_LOW */
    ret +=  IMX335_write_register(vi_pipe, 0x3059, 0x22); /* SHR0_MIDDLE */
    ret +=  IMX335_write_register(vi_pipe, 0x305A, 0x00); /* SHR0_HIGH */

    ret +=  IMX335_write_register(vi_pipe, 0x305C, 0x16); /* SHR1_LOW */
    ret +=  IMX335_write_register(vi_pipe, 0x305D, 0x00); /* SHR1_MIDDLE */
    ret +=  IMX335_write_register(vi_pipe, 0x305E, 0x00); /* SHR1_HIGH */

    ret +=  IMX335_write_register(vi_pipe, 0x3068, 0x12); /* RHS1_LOW */
    ret +=  IMX335_write_register(vi_pipe, 0x3069, 0x02); /* RHS1_MIDDLE */
    ret +=  IMX335_write_register(vi_pipe, 0x306A, 0x00); /* RHS1_HIGH */

    ret +=  IMX335_write_register(vi_pipe, 0x3072, 0x28); /* AREA2_WIDTH_1 */
    ret +=  IMX335_write_register(vi_pipe, 0x3073, 0x00); /* AREA2_WIDTH_1 */
    ret +=  IMX335_write_register(vi_pipe, 0x3074, 0xB0); /* AREA3_ST_ADR_1 */
    ret +=  IMX335_write_register(vi_pipe, 0x3075, 0x00); /* AREA3_ST_ADR_1 */
    ret +=  IMX335_write_register(vi_pipe, 0x3076, 0x58); /* AREA3_WIDTH_1 */
    ret +=  IMX335_write_register(vi_pipe, 0x3077, 0x0F); /* AREA3_WIDTH_1 */

    ret +=  IMX335_write_register(vi_pipe, 0x3078, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3079, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x307A, 0xFF);
    ret +=  IMX335_write_register(vi_pipe, 0x307B, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x307C, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x307D, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x307E, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x307F, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3080, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3081, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3082, 0xFF);
    ret +=  IMX335_write_register(vi_pipe, 0x3083, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3084, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3085, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3086, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3087, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x30A4, 0x33);
    ret +=  IMX335_write_register(vi_pipe, 0x30A8, 0x10);
    ret +=  IMX335_write_register(vi_pipe, 0x30A9, 0x04);
    ret +=  IMX335_write_register(vi_pipe, 0x30AC, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x30AD, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x30B0, 0x10);
    ret +=  IMX335_write_register(vi_pipe, 0x30B1, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x30B4, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x30B5, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x30B6, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x30B7, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x30E8, 0x00); /* GAIN_LOW_LONG */
    ret +=  IMX335_write_register(vi_pipe, 0x30E9, 0x00); /* GAIN_HIGH_LONG */

    ret +=  IMX335_write_register(vi_pipe, 0x30EA, 0x00); /* GAIN1_LOW_SHORT */
    ret +=  IMX335_write_register(vi_pipe, 0x30EB, 0x00); /* GAIN1_HIGH_SHORT */

    ret +=  IMX335_write_register(vi_pipe, 0x3112, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x3113, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3116, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x3117, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x314C, 0x80);
    ret +=  IMX335_write_register(vi_pipe, 0x314D, 0x00);

    ret +=  IMX335_write_register(vi_pipe, 0x315A, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3168, 0x68);
    ret +=  IMX335_write_register(vi_pipe, 0x316A, 0x7E);

    ret +=  IMX335_write_register(vi_pipe, 0x3199, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x319D, 0x00); /* MDBIT */
    ret +=  IMX335_write_register(vi_pipe, 0x319E, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x319F, 0x03); /* virtul channel mode */
    ret +=  IMX335_write_register(vi_pipe, 0x31A0, 0x2A); /* XH&VSOUTSEL */
    ret +=  IMX335_write_register(vi_pipe, 0x31D7, 0x01);

    ret +=  IMX335_write_register(vi_pipe, 0x3200, 0x00); /* FGAINEN */

    ret +=  IMX335_write_register(vi_pipe, 0x3288, 0x21);
    ret +=  IMX335_write_register(vi_pipe, 0x328A, 0x02);

    ret +=  IMX335_write_register(vi_pipe, 0x3300, 0x00); /* TCYCLE */
    ret +=  IMX335_write_register(vi_pipe, 0x3302, 0x32); /* BLKLEVEL */

    ret +=  IMX335_write_register(vi_pipe, 0x3414, 0x05);
    ret +=  IMX335_write_register(vi_pipe, 0x3416, 0x18);
    ret +=  IMX335_write_register(vi_pipe, 0x341C, 0xFF); /* ADBIT1 */
    ret +=  IMX335_write_register(vi_pipe, 0x341D, 0x01);

    ret +=  IMX335_write_register(vi_pipe, 0x3648, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x364A, 0x04);
    ret +=  IMX335_write_register(vi_pipe, 0x364C, 0x04);
    ret +=  IMX335_write_register(vi_pipe, 0x3678, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x367C, 0x31);
    ret +=  IMX335_write_register(vi_pipe, 0x367E, 0x31);

    ret +=  IMX335_write_register(vi_pipe, 0x3706, 0x10);
    ret +=  IMX335_write_register(vi_pipe, 0x3708, 0x03);

    ret +=  IMX335_write_register(vi_pipe, 0x3714, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3715, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3716, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3717, 0x03);

    ret +=  IMX335_write_register(vi_pipe, 0x371C, 0x3D);
    ret +=  IMX335_write_register(vi_pipe, 0x371D, 0x3F);

    ret +=  IMX335_write_register(vi_pipe, 0x372C, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x372D, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x372E, 0x46);
    ret +=  IMX335_write_register(vi_pipe, 0x372F, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3730, 0x89);
    ret +=  IMX335_write_register(vi_pipe, 0x3731, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3732, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x3733, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3734, 0xFE);
    ret +=  IMX335_write_register(vi_pipe, 0x3735, 0x05);

    ret +=  IMX335_write_register(vi_pipe, 0x3740, 0x02);

    ret +=  IMX335_write_register(vi_pipe, 0x375D, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x375E, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x375F, 0x11);
    ret +=  IMX335_write_register(vi_pipe, 0x3760, 0x01);

    ret +=  IMX335_write_register(vi_pipe, 0x3768, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x3769, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x376A, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x376B, 0x1B);
    ret +=  IMX335_write_register(vi_pipe, 0x376C, 0x1A);
    ret +=  IMX335_write_register(vi_pipe, 0x376D, 0x17);
    ret +=  IMX335_write_register(vi_pipe, 0x376E, 0x0F);

    ret +=  IMX335_write_register(vi_pipe, 0x3776, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3777, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3778, 0x46);
    ret +=  IMX335_write_register(vi_pipe, 0x3779, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x377A, 0x89);
    ret +=  IMX335_write_register(vi_pipe, 0x377B, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x377C, 0x08);
    ret +=  IMX335_write_register(vi_pipe, 0x377D, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x377E, 0x23);
    ret +=  IMX335_write_register(vi_pipe, 0x377F, 0x02);
    ret +=  IMX335_write_register(vi_pipe, 0x3780, 0xD9);
    ret +=  IMX335_write_register(vi_pipe, 0x3781, 0x03);
    ret +=  IMX335_write_register(vi_pipe, 0x3782, 0xF5);
    ret +=  IMX335_write_register(vi_pipe, 0x3783, 0x06);
    ret +=  IMX335_write_register(vi_pipe, 0x3784, 0xA5);

    ret +=  IMX335_write_register(vi_pipe, 0x3788, 0x0F);

    ret +=  IMX335_write_register(vi_pipe, 0x378A, 0xD9);
    ret +=  IMX335_write_register(vi_pipe, 0x378B, 0x03);
    ret +=  IMX335_write_register(vi_pipe, 0x378C, 0xEB);
    ret +=  IMX335_write_register(vi_pipe, 0x378D, 0x05);
    ret +=  IMX335_write_register(vi_pipe, 0x378E, 0x87);
    ret +=  IMX335_write_register(vi_pipe, 0x378F, 0x06);
    ret +=  IMX335_write_register(vi_pipe, 0x3790, 0xF5);

    ret +=  IMX335_write_register(vi_pipe, 0x3792, 0x43);
    ret +=  IMX335_write_register(vi_pipe, 0x3794, 0x7A);
    ret +=  IMX335_write_register(vi_pipe, 0x3796, 0xA1);
    ret +=  IMX335_write_register(vi_pipe, 0x37B0, 0x36);

    ret +=  IMX335_write_register(vi_pipe, 0x3A00, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3A01, 0x03);

    ret +=  IMX335_write_register(vi_pipe, 0x3A04, 0x48); /* add */
    ret +=  IMX335_write_register(vi_pipe, 0x3A05, 0x09); /* add */

    ret +=  IMX335_write_register(vi_pipe, 0x3A18, 0x8F);
    ret +=  IMX335_write_register(vi_pipe, 0x3A19, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A1A, 0x4F);
    ret +=  IMX335_write_register(vi_pipe, 0x3A1B, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A1C, 0x47);
    ret +=  IMX335_write_register(vi_pipe, 0x3A1D, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A1E, 0x37);
    ret +=  IMX335_write_register(vi_pipe, 0x3A1F, 0x01);
    ret +=  IMX335_write_register(vi_pipe, 0x3A20, 0x4F);
    ret +=  IMX335_write_register(vi_pipe, 0x3A21, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A22, 0x87);
    ret +=  IMX335_write_register(vi_pipe, 0x3A23, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A24, 0x4F);
    ret +=  IMX335_write_register(vi_pipe, 0x3A25, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A26, 0x7F);
    ret +=  IMX335_write_register(vi_pipe, 0x3A27, 0x00);
    ret +=  IMX335_write_register(vi_pipe, 0x3A28, 0x3F);
    ret +=  IMX335_write_register(vi_pipe, 0x3A29, 0x00);

    IMX335_default_reg_init(vi_pipe);

    ret +=  IMX335_write_register(vi_pipe, 0x3000, 0x00); /* Standby Cancel */
    delay_ms(20);  /* delay 20 ms */
    ret +=  IMX335_write_register(vi_pipe, 0x3002, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("-------Sony IMX335_init_5M_2592x1944_10bit_wdr30 Initial OK!-------\n");
    return;
}
