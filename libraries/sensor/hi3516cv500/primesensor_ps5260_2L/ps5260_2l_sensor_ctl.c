/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ps5260_2l_sensor_ctl.c
 * Author: ISP SW
 * Create: 2020/02/05
 */

#include "ps5260_2l_cmos.h"
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

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

int ps5260_2l_i2c_init(VI_PIPE vi_pipe)
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
    ISP_SNS_COMMBUS_U *ps52602lbusinfo = HI_NULL;
    ps52602lbusinfo = ps5260_2l_get_bus_Info(vi_pipe);
    u8DevNum = ps52602lbusinfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (PS5260_2L_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int ps5260_2l_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int ps5260_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("PS5260_read_register fd not opened!\n");
        return HI_FAILURE;
    }

    int idx = 0;
    int ret;
    int data;
    unsigned char buf[I2C_BUF_NUM];

    buf[idx++] = addr & 0xFF;
    if (PS5260_2L_ADDR_BYTE == 2) { /* 2 byte */
        buf[idx++] = addr >> 8; /* shift 8 */
    }
    ret = write(g_fd[vi_pipe], buf, PS5260_2L_ADDR_BYTE);
    ret += read(g_fd[vi_pipe], buf, PS5260_2L_DATA_BYTE);
    if (ret < 0) {
        printf("I2C_READ error!\n");
        return -1;
    }

    if (PS5260_2L_DATA_BYTE == 2) { /* 2 byte */
        data = buf[0] | (buf[1] << 8); /* shift 8 */
    } else {
        data = buf[0];
    }

    return data;
}

int ps5260_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = PS5260_2L_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = PS5260_2L_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = PS5260_2L_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (PS5260_2L_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (PS5260_2L_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, PS5260_2L_ADDR_BYTE + PS5260_2L_DATA_BYTE);
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

void ps5260_2l_standby(VI_PIPE vi_pipe)
{
    return;
}

void ps5260_2l_restart(VI_PIPE vi_pipe)
{
    delay_ms(20); /* delay 20 ms */
    return;
}

void ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080(VI_PIPE vi_pipe);
void ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080(VI_PIPE vi_pipe);
void ps5260_2l_default_reg_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastps5260 = HI_NULL;
    pastps5260 = ps5260_2l_get_ctx(vi_pipe);
    for (i = 0; i < pastps5260->astRegsInfo[0].u32RegNum; i++) {
        ret += ps5260_2l_write_register(vi_pipe,
            pastps5260->astRegsInfo[0].astI2cData[i].u32RegAddr,
            pastps5260->astRegsInfo[0].astI2cData[i].u32Data);
    }
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void ps5260_2l_init(VI_PIPE vi_pipe)
{
    WDR_MODE_E       enWDRMode;
    HI_U8            u8ImgMode;
    HI_S32           ret;
    HI_U32 sensor_nep_min, sign;
    HI_U32 sen_con, sen_con1;
    ISP_SNS_STATE_S *pastps5260 = HI_NULL;
    HI_U32 *sensor_const = HI_NULL;
    sensor_const = g_sensor_const1_ctx(vi_pipe);
    pastps5260 = ps5260_2l_get_ctx(vi_pipe);
    enWDRMode   = pastps5260->enWDRMode;
    u8ImgMode   = pastps5260->u8ImgMode;

    ret = ps5260_2l_i2c_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c init failed!\n");
    }
    /* When sensor first init, config all registers */
    if (enWDRMode == WDR_MODE_BUILT_IN) {
        if (u8ImgMode == PS5260_SENSOR_1080P_30FPS_BuiltinWDR_MODE) {  /* PS5260_SENSOR_1080P_30FPS_2t1_WDR_MODE */
            ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080(vi_pipe);
        } else {
        }
    } else {
        ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080(vi_pipe);
    }
    pastps5260->bInit = HI_TRUE;

    ret = ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c write failed!\n");
    }
    sign = ps5260_2l_read_register(vi_pipe, 0x10);
    sensor_nep_min = (sign & 0x04) << 6; /* shift 6 */
    sensor_nep_min = sensor_nep_min | ps5260_2l_read_register(vi_pipe, 0x12);
    sign = ps5260_2l_read_register(vi_pipe, 0x0E);
    sen_con = (sign & 0x01F) << 8; /* shift 8 */
    sen_con1 = ps5260_2l_read_register(vi_pipe, 0x0F);
    *sensor_const = (sen_con | sen_con1) + sensor_nep_min;

    return;
}

void ps5260_2l_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = ps5260_2l_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
    }
    return;
}

HI_S32 ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_1(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x44, 0x00); /* T_pll_enh 0 */
    ret += ps5260_2l_write_register(vi_pipe, 0x43, 0x02); /* T_MIPI_sel 1 */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xF5, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x10, 0x80); /* Cmd_BYTECLK_InvSel 0 for PS308 */
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x80); /* GatedAllClk enable */
    ret += ps5260_2l_write_register(vi_pipe, 0x13, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x16, 0xBC);
    ret += ps5260_2l_write_register(vi_pipe, 0x37, 0x0E);
    ret += ps5260_2l_write_register(vi_pipe, 0x38, 0x28);
    ret += ps5260_2l_write_register(vi_pipe, 0x39, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x3C, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x5F, 0x64);
    ret += ps5260_2l_write_register(vi_pipe, 0x61, 0xDA);
    ret += ps5260_2l_write_register(vi_pipe, 0x62, 0x14);
    ret += ps5260_2l_write_register(vi_pipe, 0x69, 0x10);
    ret += ps5260_2l_write_register(vi_pipe, 0x75, 0x0B);
    ret += ps5260_2l_write_register(vi_pipe, 0x77, 0x06);
    ret += ps5260_2l_write_register(vi_pipe, 0x7E, 0x19);
    ret += ps5260_2l_write_register(vi_pipe, 0x85, 0x88);
    ret += ps5260_2l_write_register(vi_pipe, 0x9E, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA0, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xA2, 0x09);
    ret += ps5260_2l_write_register(vi_pipe, 0xBE, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xBF, 0x80);
    ret += ps5260_2l_write_register(vi_pipe, 0xDF, 0x1E);
    ret += ps5260_2l_write_register(vi_pipe, 0xE1, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xE2, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0xE3, 0x20);
    ret += ps5260_2l_write_register(vi_pipe, 0xE4, 0x0F);
    ret += ps5260_2l_write_register(vi_pipe, 0xE5, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0xE6, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xF3, 0xB1); /* B16A: Version */
    ret += ps5260_2l_write_register(vi_pipe, 0xF8, 0x6A); /* B16A: Version */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x05, 0x0B);
    return ret;
}

HI_S32 ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_2(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0x07, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x08, 0x46);
    ret += ps5260_2l_write_register(vi_pipe, 0x0A, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0x0B, 0x64);
    ret += ps5260_2l_write_register(vi_pipe, 0x0C, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x0D, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0x0E, 0x10);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0xCC);
    ret += ps5260_2l_write_register(vi_pipe, 0x18, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x19, 0x23);
    ret += ps5260_2l_write_register(vi_pipe, 0x27, 0x11);
    ret += ps5260_2l_write_register(vi_pipe, 0x28, 0x30);
    ret += ps5260_2l_write_register(vi_pipe, 0x2A, 0x3C);
    ret += ps5260_2l_write_register(vi_pipe, 0x37, 0x90);
    ret += ps5260_2l_write_register(vi_pipe, 0x3A, 0xF0);
    ret += ps5260_2l_write_register(vi_pipe, 0x3B, 0x20);
    ret += ps5260_2l_write_register(vi_pipe, 0x40, 0xF0);
    ret += ps5260_2l_write_register(vi_pipe, 0x42, 0xD6);
    ret += ps5260_2l_write_register(vi_pipe, 0x43, 0x20);
    ret += ps5260_2l_write_register(vi_pipe, 0x5C, 0x1E);
    ret += ps5260_2l_write_register(vi_pipe, 0x5D, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0x5E, 0x32);
    ret += ps5260_2l_write_register(vi_pipe, 0x67, 0x11);
    ret += ps5260_2l_write_register(vi_pipe, 0x68, 0xF4);
    ret += ps5260_2l_write_register(vi_pipe, 0x69, 0xC2);
    ret += ps5260_2l_write_register(vi_pipe, 0x75, 0x3C);
    ret += ps5260_2l_write_register(vi_pipe, 0x7B, 0x08);
    ret += ps5260_2l_write_register(vi_pipe, 0x7C, 0x08);
    ret += ps5260_2l_write_register(vi_pipe, 0x7D, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x83, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x8F, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0x90, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x92, 0x80);
    ret += ps5260_2l_write_register(vi_pipe, 0xA3, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA4, 0x0E);
    ret += ps5260_2l_write_register(vi_pipe, 0xA5, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0xA6, 0x40);
    ret += ps5260_2l_write_register(vi_pipe, 0xA7, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA8, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA9, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0xAA, 0x88);
    ret += ps5260_2l_write_register(vi_pipe, 0xAB, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xAE, 0x28);
    ret += ps5260_2l_write_register(vi_pipe, 0xB0, 0x28);
    ret += ps5260_2l_write_register(vi_pipe, 0xB3, 0x0A);
    return ret;
}

HI_S32 ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_3(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0xBE, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xBF, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xC0, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xC1, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xC4, 0x60);
    ret += ps5260_2l_write_register(vi_pipe, 0xC6, 0x60);
    ret += ps5260_2l_write_register(vi_pipe, 0xC7, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0xC8, 0xC8);
    ret += ps5260_2l_write_register(vi_pipe, 0xC9, 0x35);
    ret += ps5260_2l_write_register(vi_pipe, 0xCE, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0xCF, 0x82);
    ret += ps5260_2l_write_register(vi_pipe, 0xD0, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0xD1, 0x60);
    ret += ps5260_2l_write_register(vi_pipe, 0xD5, 0x49);
    ret += ps5260_2l_write_register(vi_pipe, 0xD7, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0xD8, 0xC8);
    ret += ps5260_2l_write_register(vi_pipe, 0xDA, 0xC0);
    ret += ps5260_2l_write_register(vi_pipe, 0xDD, 0x42);
    ret += ps5260_2l_write_register(vi_pipe, 0xDE, 0x43);
    ret += ps5260_2l_write_register(vi_pipe, 0xE2, 0x9A);
    ret += ps5260_2l_write_register(vi_pipe, 0xE4, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xF0, 0x7C);
    ret += ps5260_2l_write_register(vi_pipe, 0xF1, 0x16);
    ret += ps5260_2l_write_register(vi_pipe, 0xF2, 0x1F);
    ret += ps5260_2l_write_register(vi_pipe, 0xF3, 0x0C);
    ret += ps5260_2l_write_register(vi_pipe, 0xF4, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xF5, 0x19);
    ret += ps5260_2l_write_register(vi_pipe, 0xF6, 0x16);
    ret += ps5260_2l_write_register(vi_pipe, 0xF7, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xF8, 0x48);
    ret += ps5260_2l_write_register(vi_pipe, 0xF9, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xFA, 0x55);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x2E, 0x0C);
    ret += ps5260_2l_write_register(vi_pipe, 0x33, 0x8C);
    ret += ps5260_2l_write_register(vi_pipe, 0x3C, 0xC8);
    ret += ps5260_2l_write_register(vi_pipe, 0x4E, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0xB0, 0x81);
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x06, 0x06);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x09);
    ret += ps5260_2l_write_register(vi_pipe, 0x0A, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x0B, 0x06);
    return ret;
}

HI_S32 ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_4(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0x0C, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0x0D, 0x5E);
    ret += ps5260_2l_write_register(vi_pipe, 0x0E, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x00); /* MIPI CSI disable */
    ret += ps5260_2l_write_register(vi_pipe, 0x10, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x15, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0x17, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x18, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0x1B, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0x1C, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0x40, 0x19);
    ret += ps5260_2l_write_register(vi_pipe, 0x41, 0x1A);
    ret += ps5260_2l_write_register(vi_pipe, 0x43, 0x02); /* T_MIPI_sel[0], Bank5_67[1] 1 */
    ret += ps5260_2l_write_register(vi_pipe, 0x44, 0x01); /* MIPI PLL enable */
    ret += ps5260_2l_write_register(vi_pipe, 0x4A, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x4F, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x94, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0xB1, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x06);
    ret += ps5260_2l_write_register(vi_pipe, 0x00, 0x0C);
    ret += ps5260_2l_write_register(vi_pipe, 0x02, 0x13);
    ret += ps5260_2l_write_register(vi_pipe, 0x06, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x07, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x08, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x12);
    ret += ps5260_2l_write_register(vi_pipe, 0x10, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x12);
    ret += ps5260_2l_write_register(vi_pipe, 0x12, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x18, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x1A, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x28, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x2D, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x2E, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x2F, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x4A, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x4B, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x9E, 0x80);
    ret += ps5260_2l_write_register(vi_pipe, 0xDF, 0x04);
    return ret;
}

HI_S32 ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_5(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x00); /* GatedAllClk disable */
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x3B, 0x00); /* R_Cmd_Gated_MIPI_Clk 0 */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x02, 0xFB); /* ResetTG */
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x01); /* MIPI CSI enable */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    return ret;
}

void ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080(VI_PIPE vi_pipe)
{
    /* PNS310_MCLK27MHz_Imgsyn_1x16x_Synout_HQ_LCG_20181018_RAW12_560Mbps_B16a(1).asc.asc */
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_1(vi_pipe);
    ret += ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_2(vi_pipe);
    ret += ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_3(vi_pipe);
    ret += ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_4(vi_pipe);
    ret += ps5260_2l_Linear_1080p30_init_27M_B16A_1920_1080_5(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("=== Primesensor ps5260_B16A_1080P_30FPS_27m_12BIT_LINE_Init_OK!===\n");
    return;
}

HI_S32 ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_1(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x44, 0x00); /* T_pll_enh 0 */
    ret += ps5260_2l_write_register(vi_pipe, 0x43, 0x02); /* T_MIPI_sel 1 */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xF5, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x10, 0x80); /* Cmd_BYTECLK_InvSel 0 for PS308 */
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x80); /* GatedAllClk enable */
    ret += ps5260_2l_write_register(vi_pipe, 0x13, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x16, 0xBC);
    ret += ps5260_2l_write_register(vi_pipe, 0x38, 0x28);
    ret += ps5260_2l_write_register(vi_pipe, 0x3C, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x5F, 0x64);
    ret += ps5260_2l_write_register(vi_pipe, 0x61, 0xDA);
    ret += ps5260_2l_write_register(vi_pipe, 0x62, 0x14);
    ret += ps5260_2l_write_register(vi_pipe, 0x69, 0x10);
    ret += ps5260_2l_write_register(vi_pipe, 0x75, 0x0B);
    ret += ps5260_2l_write_register(vi_pipe, 0x77, 0x06);
    ret += ps5260_2l_write_register(vi_pipe, 0x7E, 0x19);
    ret += ps5260_2l_write_register(vi_pipe, 0x85, 0x88);
    ret += ps5260_2l_write_register(vi_pipe, 0x9E, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA0, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xA2, 0x09);
    ret += ps5260_2l_write_register(vi_pipe, 0xBE, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xBF, 0x80);
    ret += ps5260_2l_write_register(vi_pipe, 0xDF, 0x1E);
    ret += ps5260_2l_write_register(vi_pipe, 0xE1, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xE2, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0xE3, 0x20);
    ret += ps5260_2l_write_register(vi_pipe, 0xE4, 0x0F);
    ret += ps5260_2l_write_register(vi_pipe, 0xE5, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0xE6, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xF3, 0xB1); /* B17A: Version */
    ret += ps5260_2l_write_register(vi_pipe, 0xF8, 0x7A); /* B17A: Version */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x05, 0x0B);
    ret += ps5260_2l_write_register(vi_pipe, 0x07, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x08, 0x46);
    ret += ps5260_2l_write_register(vi_pipe, 0x0A, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0x0B, 0x64);
    ret += ps5260_2l_write_register(vi_pipe, 0x0C, 0x00);
    return ret;
}

HI_S32 ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_2(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0x0D, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0x0E, 0x10);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0xCC);
    ret += ps5260_2l_write_register(vi_pipe, 0x18, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x19, 0x23);
    ret += ps5260_2l_write_register(vi_pipe, 0x27, 0x11);
    ret += ps5260_2l_write_register(vi_pipe, 0x28, 0x30);
    ret += ps5260_2l_write_register(vi_pipe, 0x2A, 0x3C);
    ret += ps5260_2l_write_register(vi_pipe, 0x37, 0x90);
    ret += ps5260_2l_write_register(vi_pipe, 0x3A, 0xF0);
    ret += ps5260_2l_write_register(vi_pipe, 0x3B, 0x20);
    ret += ps5260_2l_write_register(vi_pipe, 0x40, 0xF0);
    ret += ps5260_2l_write_register(vi_pipe, 0x42, 0xD6);
    ret += ps5260_2l_write_register(vi_pipe, 0x43, 0x20);
    ret += ps5260_2l_write_register(vi_pipe, 0x5C, 0x1E);
    ret += ps5260_2l_write_register(vi_pipe, 0x5D, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0x5E, 0x32);
    ret += ps5260_2l_write_register(vi_pipe, 0x67, 0x11);
    ret += ps5260_2l_write_register(vi_pipe, 0x68, 0xF4);
    ret += ps5260_2l_write_register(vi_pipe, 0x69, 0xC2);
    ret += ps5260_2l_write_register(vi_pipe, 0x75, 0x3C);
    ret += ps5260_2l_write_register(vi_pipe, 0x7B, 0x08);
    ret += ps5260_2l_write_register(vi_pipe, 0x7C, 0x08);
    ret += ps5260_2l_write_register(vi_pipe, 0x7D, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x83, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x8F, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0x90, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x92, 0x80);
    ret += ps5260_2l_write_register(vi_pipe, 0xA3, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA4, 0x0E);
    ret += ps5260_2l_write_register(vi_pipe, 0xA5, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0xA6, 0x40);
    ret += ps5260_2l_write_register(vi_pipe, 0xA7, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA8, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xA9, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0xAA, 0x88);
    ret += ps5260_2l_write_register(vi_pipe, 0xAB, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xAE, 0x28);
    ret += ps5260_2l_write_register(vi_pipe, 0xB0, 0x28);
    ret += ps5260_2l_write_register(vi_pipe, 0xB3, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0xBE, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xBF, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xC0, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xC1, 0x00);
    return ret;
}

HI_S32 ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_3(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0xC4, 0x60);
    ret += ps5260_2l_write_register(vi_pipe, 0xC6, 0x60);
    ret += ps5260_2l_write_register(vi_pipe, 0xC7, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0xC8, 0xC8);
    ret += ps5260_2l_write_register(vi_pipe, 0xC9, 0x35);
    ret += ps5260_2l_write_register(vi_pipe, 0xCE, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0xCF, 0x82);
    ret += ps5260_2l_write_register(vi_pipe, 0xD0, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0xD1, 0x60);
    ret += ps5260_2l_write_register(vi_pipe, 0xD5, 0x49);
    ret += ps5260_2l_write_register(vi_pipe, 0xD7, 0x0A);
    ret += ps5260_2l_write_register(vi_pipe, 0xD8, 0xC8);
    ret += ps5260_2l_write_register(vi_pipe, 0xDA, 0xC0);
    ret += ps5260_2l_write_register(vi_pipe, 0xDD, 0x42);
    ret += ps5260_2l_write_register(vi_pipe, 0xDE, 0x43);
    ret += ps5260_2l_write_register(vi_pipe, 0xE2, 0x9A);
    ret += ps5260_2l_write_register(vi_pipe, 0xE4, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xF0, 0x7C);
    ret += ps5260_2l_write_register(vi_pipe, 0xF1, 0x16);
    ret += ps5260_2l_write_register(vi_pipe, 0xF2, 0x1F);
    ret += ps5260_2l_write_register(vi_pipe, 0xF3, 0x0C);
    ret += ps5260_2l_write_register(vi_pipe, 0xF4, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xF5, 0x19);
    ret += ps5260_2l_write_register(vi_pipe, 0xF6, 0x16);
    ret += ps5260_2l_write_register(vi_pipe, 0xF7, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xF8, 0x48);
    ret += ps5260_2l_write_register(vi_pipe, 0xF9, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0xFA, 0x55);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x2E, 0x0C);
    ret += ps5260_2l_write_register(vi_pipe, 0x33, 0x8C);
    ret += ps5260_2l_write_register(vi_pipe, 0x36, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x3C, 0xC8);
    ret += ps5260_2l_write_register(vi_pipe, 0x4E, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0xB0, 0x81);
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x06, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x09);
    ret += ps5260_2l_write_register(vi_pipe, 0x0A, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x0B, 0x06);
    ret += ps5260_2l_write_register(vi_pipe, 0x0C, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0x0D, 0x5E);
    ret += ps5260_2l_write_register(vi_pipe, 0x0E, 0x01);
    return ret;
}

HI_S32 ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_4(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x00); /* MIPI CSI disable */
    ret += ps5260_2l_write_register(vi_pipe, 0x10, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x15, 0x07);
    ret += ps5260_2l_write_register(vi_pipe, 0x17, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x18, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0x1B, 0x03);
    ret += ps5260_2l_write_register(vi_pipe, 0x1C, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0x25, 0x01); /* B17A */
    ret += ps5260_2l_write_register(vi_pipe, 0x40, 0x19);
    ret += ps5260_2l_write_register(vi_pipe, 0x41, 0x1A);
    ret += ps5260_2l_write_register(vi_pipe, 0x43, 0x02); /* T_MIPI_sel[0], Bank5_67[1] 1 */
    ret += ps5260_2l_write_register(vi_pipe, 0x44, 0x01); /* MIPI PLL enable */
    ret += ps5260_2l_write_register(vi_pipe, 0x4A, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x4F, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x94, 0x04);
    ret += ps5260_2l_write_register(vi_pipe, 0xB1, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x06);
    ret += ps5260_2l_write_register(vi_pipe, 0x00, 0x0C);
    ret += ps5260_2l_write_register(vi_pipe, 0x02, 0x13);
    ret += ps5260_2l_write_register(vi_pipe, 0x06, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x07, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x08, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x02);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x12);
    ret += ps5260_2l_write_register(vi_pipe, 0x10, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x12);
    ret += ps5260_2l_write_register(vi_pipe, 0x12, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x18, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x1A, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x28, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x2D, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x2E, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x2F, 0x6C);
    ret += ps5260_2l_write_register(vi_pipe, 0x4A, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x4B, 0x26);
    ret += ps5260_2l_write_register(vi_pipe, 0x9E, 0x01); /* 0x01 for HDR-CRAW12 compression output */
    ret += ps5260_2l_write_register(vi_pipe, 0xDF, 0x00); /* 04 to 00 */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x00);
    ret += ps5260_2l_write_register(vi_pipe, 0x11, 0x00); /* GatedAllClk disable */
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x3B, 0x00); /* R_Cmd_Gated_MIPI_Clk 0 */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    return ret;
}

HI_S32 ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_5(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0x02, 0xFB); /* ResetTG */
    ret += ps5260_2l_write_register(vi_pipe, 0x09, 0x01);
    ret += ps5260_2l_write_register(vi_pipe, 0xEF, 0x05);
    ret += ps5260_2l_write_register(vi_pipe, 0x0F, 0x01); /* MIPI CSI enable */
    ret += ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    delay_ms(20); /* B17A, Delay time need bigger than Vblanking interval, Delay 20ms */
    ret += ps5260_2l_write_register(vi_pipe, 0x25, 0x00); /* B17A */
    return ret;
}

void ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080(VI_PIPE vi_pipe)
{
    /* PNS310_MCLK27MHz_Imgsyn_1x16x_Synout_HQ_LCG_20181018_RAW14_560Mbps_B17a.asc => compression */
    HI_S32 ret = HI_SUCCESS;
    ret += ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_1(vi_pipe);
    ret += ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_2(vi_pipe);
    ret += ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_3(vi_pipe);
    ret += ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_4(vi_pipe);
    ret += ps5260_2l_BuiltinWDR_1080p30_init_27M_1920_1080_5(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("=== Primesensor ps5260_Compression_WDR_1080p30_B17A_raw12_init_OK!===\n");
    return;
}