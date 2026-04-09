/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os08a10_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "os08a10_cmos.h"
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

/* sensor fps mode */
#define OS08A10_SENSOR_4k2k_30FPS_LINEAR_MODE        1
#define OS08A10_SENSOR_1080P120FPS_LINEAR_MODE       2
#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
int OS08A10_i2c_init(VI_PIPE vi_pipe)
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
    ISP_SNS_COMMBUS_U *os08a10businfo = HI_NULL;
    os08a10businfo = os08a10_get_bus_Info(vi_pipe);
    u8DevNum = os08a10businfo->s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (OS08A10_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int OS08A10_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int OS08A10_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}
int OS08A10_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = OS08A10_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OS08A10_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OS08A10_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        SNS_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (OS08A10_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (OS08A10_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, OS08A10_ADDR_BYTE + OS08A10_DATA_BYTE);
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

void OS08A10_standby(VI_PIPE vi_pipe)
{
    return;
}

void OS08A10_restart(VI_PIPE vi_pipe)
{
    return;
}
static void delay_ms(int ms)
{
    usleep(ms * 1000);  /* 1ms: 1000us */
    return;
}

void OS08A10_wdr_4k2k30_2to1_init(VI_PIPE vi_pipe);
void OS08A10_wdr_4k2k25_2to1_init(VI_PIPE vi_pipe);
void OS08A10_linear_4k2k30_init(VI_PIPE vi_pipe);
void OS08A10_linear_1080P120_init(VI_PIPE vi_pipe);
void OS08A10_linear_4k2k25_init(VI_PIPE vi_pipe);
void OS08A10_default_reg_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastos08a10 = HI_NULL;
    pastos08a10 = os08a10_get_ctx(vi_pipe);
    for (i = 0; i < pastos08a10->astRegsInfo[0].u32RegNum; i++) {
        ret += OS08A10_write_register(vi_pipe,
                                      pastos08a10->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                      pastos08a10->astRegsInfo[0].astI2cData[i].u32Data);
    }
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
    }
    return;
}

void OS08A10_init(VI_PIPE vi_pipe)
{
    WDR_MODE_E       enWDRMode;
    HI_BOOL          bInit;
    ISP_SNS_STATE_S *pastos08a10 = HI_NULL;
    pastos08a10 = os08a10_get_ctx(vi_pipe);
    bInit       = pastos08a10->bInit;
    enWDRMode   = pastos08a10->enWDRMode;

    /* When sensor first init, config all registers */
    if (bInit == HI_FALSE) {
        /* sensor i2c init */
        OS08A10_i2c_init(vi_pipe);
    }

    /* When sensor first init, config all registers */
    if (WDR_MODE_2To1_LINE == enWDRMode) {
        {
            printf("OS08A10_init Not support this mode\n");
            return;
        }
    } else if (enWDRMode == WDR_MODE_NONE) {
        OS08A10_linear_4k2k30_init(vi_pipe);
    } else {
        printf("OS08A10_init Not support this mode\n");
        return;
    }

    pastos08a10->bInit = HI_TRUE;

    return;
}

void OS08A10_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = OS08A10_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
    }
    return;
}

/* 4k2k30 10bit */
void OS08A10_linear_4k2k30_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += OS08A10_write_register(vi_pipe, 0x0103, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0303, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0305, 0x5c);
    ret += OS08A10_write_register(vi_pipe, 0x0306, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0307, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0308, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x0309, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x032a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x300f, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3010, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3011, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3012, 0x41);
    ret += OS08A10_write_register(vi_pipe, 0x3016, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x301e, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3031, 0xa9);
    ret += OS08A10_write_register(vi_pipe, 0x3603, 0x2c);
    ret += OS08A10_write_register(vi_pipe, 0x3103, 0x92);
    ret += OS08A10_write_register(vi_pipe, 0x3104, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3106, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3400, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3025, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x3425, 0x51);
    ret += OS08A10_write_register(vi_pipe, 0x3428, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3406, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3408, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x340c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x340d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x031e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3501, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3502, 0x6e);
    ret += OS08A10_write_register(vi_pipe, 0x3505, 0x83);
    ret += OS08A10_write_register(vi_pipe, 0x3508, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3509, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350d, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x350e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350f, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3600, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3605, 0x50);
    ret += OS08A10_write_register(vi_pipe, 0x3609, 0xb5);
    ret += OS08A10_write_register(vi_pipe, 0x3610, 0x39);
    ret += OS08A10_write_register(vi_pipe, 0x360c, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x360e, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3628, 0xa4);
    ret += OS08A10_write_register(vi_pipe, 0x362d, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3660, 0x43);
    ret += OS08A10_write_register(vi_pipe, 0x3661, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x3662, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3663, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3664, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x366a, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x366b, 0xa0);
    ret += OS08A10_write_register(vi_pipe, 0x366d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x366e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3680, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3705, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3706, 0x35);
    ret += OS08A10_write_register(vi_pipe, 0x370a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x370b, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3709, 0x49);
    ret += OS08A10_write_register(vi_pipe, 0x3714, 0x21);
    ret += OS08A10_write_register(vi_pipe, 0x371c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x371d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x375e, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3762, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3776, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3781, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3782, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3783, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3784, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3785, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3788, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3789, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3797, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3800, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3801, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3802, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3803, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x3804, 0x0e);
    ret += OS08A10_write_register(vi_pipe, 0x3805, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3806, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3807, 0x6f);
    ret += OS08A10_write_register(vi_pipe, 0x3808, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x3809, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x380a, 0x08);

    ret += OS08A10_write_register(vi_pipe, 0x380b, 0x70);
    ret += OS08A10_write_register(vi_pipe, 0x380c, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x380d, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x380e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x380f, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x3813, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3814, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3815, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3816, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3817, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x381c, 0x00);
    /* mirror */
    ret += OS08A10_write_register(vi_pipe, 0x3820, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3821, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3823, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3826, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3827, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x382d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3832, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x383c, 0x48);
    ret += OS08A10_write_register(vi_pipe, 0x383d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3d84, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += OS08A10_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += OS08A10_write_register(vi_pipe, 0x4000, 0xf8);
    ret += OS08A10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += OS08A10_write_register(vi_pipe, 0x400a, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x400f, 0xa1);
    ret += OS08A10_write_register(vi_pipe, 0x4010, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x4018, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4008, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x4009, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x401a, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4050, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4051, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4028, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x4052, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4053, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4054, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4055, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4056, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4057, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4058, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4059, 0x80);

    ret += OS08A10_write_register(vi_pipe, 0x430b, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x430e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4501, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4502, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4643, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4640, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4641, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4800, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4809, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4813, 0x90);
    ret += OS08A10_write_register(vi_pipe, 0x4817, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4833, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4837, 0x15);
    ret += OS08A10_write_register(vi_pipe, 0x483b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x484b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4850, 0x7c);
    ret += OS08A10_write_register(vi_pipe, 0x4852, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x4856, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4857, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x4862, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x4869, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x486a, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x486e, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x486f, 0x55);
    ret += OS08A10_write_register(vi_pipe, 0x4875, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x5000, 0x89);
    ret += OS08A10_write_register(vi_pipe, 0x5001, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x5004, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x5005, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5180, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5181, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x580b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4700, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4e00, 0x2b);
    OS08A10_default_reg_init(vi_pipe);
    delay_ms(5);   /* delay 5 ms */
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("=++++++++==OS08A10 4k2k 30fps 10bit LINE Init OK!===\n");
    return;
}

void OS08A10_linear_1080P120_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += OS08A10_write_register(vi_pipe, 0x0103, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0303, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0305, 0x5c);
    ret += OS08A10_write_register(vi_pipe, 0x0306, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0308, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x0309, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x032a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x300f, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3010, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3011, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3012, 0x41);
    ret += OS08A10_write_register(vi_pipe, 0x3016, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x301e, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3031, 0xa9);
    ret += OS08A10_write_register(vi_pipe, 0x3603, 0x2c);
    ret += OS08A10_write_register(vi_pipe, 0x3103, 0x92);
    ret += OS08A10_write_register(vi_pipe, 0x3104, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3106, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3400, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3025, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x3425, 0x51);
    ret += OS08A10_write_register(vi_pipe, 0x3428, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3406, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3408, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x340c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x340d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x031e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3501, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3502, 0x62);
    ret += OS08A10_write_register(vi_pipe, 0x3505, 0x83);
    ret += OS08A10_write_register(vi_pipe, 0x3508, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3509, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350d, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x350e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350f, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3600, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3605, 0x50);
    ret += OS08A10_write_register(vi_pipe, 0x3609, 0xb5);
    ret += OS08A10_write_register(vi_pipe, 0x3610, 0x39);
    ret += OS08A10_write_register(vi_pipe, 0x360c, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x360e, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3628, 0xa4);
    ret += OS08A10_write_register(vi_pipe, 0x362d, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3660, 0x43);
    ret += OS08A10_write_register(vi_pipe, 0x3661, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x3662, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3663, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3664, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x366a, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x366b, 0xa0);
    ret += OS08A10_write_register(vi_pipe, 0x366d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x366e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3680, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3701, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x373b, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x373c, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3736, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3737, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3705, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3706, 0x35);
    ret += OS08A10_write_register(vi_pipe, 0x370a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x370b, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3709, 0x49);
    ret += OS08A10_write_register(vi_pipe, 0x3714, 0x22);
    ret += OS08A10_write_register(vi_pipe, 0x371c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x371d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x375e, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3762, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3776, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3781, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3782, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3783, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3784, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3785, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3788, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3789, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3797, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3800, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3801, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3802, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3803, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x3804, 0x0e);
    ret += OS08A10_write_register(vi_pipe, 0x3805, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3806, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3807, 0x6f);
    ret += OS08A10_write_register(vi_pipe, 0x3808, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x3809, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x380a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x380b, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x380c, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x380d, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x380e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x380f, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3813, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3814, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x3815, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3816, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x3817, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x381c, 0x00);

    ret += OS08A10_write_register(vi_pipe, 0x3820, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3821, 0x04);

    ret += OS08A10_write_register(vi_pipe, 0x3823, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3826, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3827, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x382d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3832, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x383c, 0x48);
    ret += OS08A10_write_register(vi_pipe, 0x383d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3d84, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += OS08A10_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += OS08A10_write_register(vi_pipe, 0x4000, 0xf8);
    ret += OS08A10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += OS08A10_write_register(vi_pipe, 0x400a, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x400f, 0xa1);
    ret += OS08A10_write_register(vi_pipe, 0x4010, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x4018, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4008, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x4009, 0x05);
    ret += OS08A10_write_register(vi_pipe, 0x401a, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4050, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4051, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4028, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x4052, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4053, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4054, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4055, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4056, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4057, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4058, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4059, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x430b, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x430e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4501, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x4502, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4643, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4640, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4641, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4800, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4809, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4813, 0x90);
    ret += OS08A10_write_register(vi_pipe, 0x4817, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4833, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4837, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x483b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x484b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4850, 0x7c);
    ret += OS08A10_write_register(vi_pipe, 0x4852, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x4856, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4857, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x4862, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x4869, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x486a, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x486e, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x486f, 0x55);
    ret += OS08A10_write_register(vi_pipe, 0x4875, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x5000, 0x89);
    ret += OS08A10_write_register(vi_pipe, 0x5001, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x5004, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x5005, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5180, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5181, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x580b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4700, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4e00, 0x2b);
    OS08A10_default_reg_init(vi_pipe);
    delay_ms(5);   /* delay 5 ms */
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("===OS08A10 1080P 120fps 10bit LINE Init OK!===---------\n");
    return;
}

/* 4k2k25 10bit */
void OS08A10_linear_4k2k25_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += OS08A10_write_register(vi_pipe, 0x0103, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0303, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0305, 0x5c);
    ret += OS08A10_write_register(vi_pipe, 0x0306, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0307, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0308, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x0309, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x032a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x300f, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3010, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3011, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3012, 0x41);
    ret += OS08A10_write_register(vi_pipe, 0x3016, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x301e, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3031, 0xa9);
    ret += OS08A10_write_register(vi_pipe, 0x3603, 0x2c);
    ret += OS08A10_write_register(vi_pipe, 0x3103, 0x92);
    ret += OS08A10_write_register(vi_pipe, 0x3104, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3106, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3400, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3025, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x3425, 0x51);
    ret += OS08A10_write_register(vi_pipe, 0x3428, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3406, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3408, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x340c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x340d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x031e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3501, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3502, 0x6e);
    ret += OS08A10_write_register(vi_pipe, 0x3505, 0x83);
    ret += OS08A10_write_register(vi_pipe, 0x3508, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3509, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350d, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x350e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350f, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3600, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3605, 0x50);
    ret += OS08A10_write_register(vi_pipe, 0x3609, 0xb5);
    ret += OS08A10_write_register(vi_pipe, 0x3610, 0x39);
    ret += OS08A10_write_register(vi_pipe, 0x360c, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x360e, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3628, 0xa4);
    ret += OS08A10_write_register(vi_pipe, 0x362d, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3660, 0x43);
    ret += OS08A10_write_register(vi_pipe, 0x3661, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x3662, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3663, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3664, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x366a, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x366b, 0xa0);
    ret += OS08A10_write_register(vi_pipe, 0x366d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x366e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3680, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3705, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3706, 0x35);
    ret += OS08A10_write_register(vi_pipe, 0x370a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x370b, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3709, 0x49);
    ret += OS08A10_write_register(vi_pipe, 0x3714, 0x21);
    ret += OS08A10_write_register(vi_pipe, 0x371c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x371d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x375e, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3762, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3776, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3781, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3782, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3783, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3784, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3785, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3788, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3789, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3797, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3800, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3801, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3802, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3803, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x3804, 0x0e);
    ret += OS08A10_write_register(vi_pipe, 0x3805, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3806, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3807, 0x6f);
    ret += OS08A10_write_register(vi_pipe, 0x3808, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x3809, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x380a, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x380b, 0x70);
    ret += OS08A10_write_register(vi_pipe, 0x380c, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x380d, 0xb6);
    ret += OS08A10_write_register(vi_pipe, 0x380e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x380f, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x3813, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3814, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3815, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3816, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3817, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x381c, 0x00);
    /* mirror */
    ret += OS08A10_write_register(vi_pipe, 0x3820, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3821, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3823, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3826, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3827, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x382d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3832, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x383c, 0x48);
    ret += OS08A10_write_register(vi_pipe, 0x383d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3d84, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += OS08A10_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += OS08A10_write_register(vi_pipe, 0x4000, 0xf8);
    ret += OS08A10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += OS08A10_write_register(vi_pipe, 0x400a, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x400f, 0xa1);
    ret += OS08A10_write_register(vi_pipe, 0x4010, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x4018, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4008, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x4009, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x401a, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4050, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4051, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4028, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x4052, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4053, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4054, 0x00);

    ret += OS08A10_write_register(vi_pipe, 0x4055, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4056, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4057, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4058, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4059, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x430b, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x430e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4501, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4502, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4643, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4640, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4641, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4800, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4809, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4813, 0x90);
    ret += OS08A10_write_register(vi_pipe, 0x4817, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4833, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4837, 0x15);
    ret += OS08A10_write_register(vi_pipe, 0x483b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x484b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4850, 0x7c);
    ret += OS08A10_write_register(vi_pipe, 0x4852, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x4856, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4857, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x4862, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x4869, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x486a, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x486e, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x486f, 0x55);
    ret += OS08A10_write_register(vi_pipe, 0x4875, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x5000, 0x89);
    ret += OS08A10_write_register(vi_pipe, 0x5001, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x5004, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x5005, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5180, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5181, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x580b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4700, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4e00, 0x2b);
    OS08A10_default_reg_init(vi_pipe);
    delay_ms(5);  /* delay 5 ms */
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("===OS08A10 4k2k 25fps 10bit LINE Init OK!===\n");
    printf("===MIPI data rate = 736Mbps/lane =====\n");
    return;
}

void OS08A10_wdr_4k2k30_2to1_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
#if 1 /* stone */
    ret += OS08A10_write_register(vi_pipe, 0x0103, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0303, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0305, 0x5c);
    ret += OS08A10_write_register(vi_pipe, 0x0306, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0308, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x0309, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x032a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x300f, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3010, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3012, 0x41);
    ret += OS08A10_write_register(vi_pipe, 0x3016, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x301e, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3031, 0xa9);
    ret += OS08A10_write_register(vi_pipe, 0x3103, 0x92);
    ret += OS08A10_write_register(vi_pipe, 0x3104, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3106, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x340c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x340d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x031e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3501, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3502, 0xe5);
    ret += OS08A10_write_register(vi_pipe, 0x3505, 0x83);
    ret += OS08A10_write_register(vi_pipe, 0x3508, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3509, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350d, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x350e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350f, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3600, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3605, 0x50);
    ret += OS08A10_write_register(vi_pipe, 0x3609, 0xb5);
    ret += OS08A10_write_register(vi_pipe, 0x3610, 0x69);
    ret += OS08A10_write_register(vi_pipe, 0x360c, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x360e, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3628, 0xa4);
    ret += OS08A10_write_register(vi_pipe, 0x362d, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3660, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x3661, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x3662, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3663, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3664, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x366a, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x366b, 0xa0);
    ret += OS08A10_write_register(vi_pipe, 0x366d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x366e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3680, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3701, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x373b, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x373c, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3736, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3737, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3705, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3706, 0x35);
    ret += OS08A10_write_register(vi_pipe, 0x370a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x370b, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3709, 0x49);
    ret += OS08A10_write_register(vi_pipe, 0x3714, 0x21);
    ret += OS08A10_write_register(vi_pipe, 0x371c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x371d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x375e, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3776, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3781, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3782, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3783, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3784, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3785, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3788, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3789, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3797, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3800, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3801, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3802, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3803, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x3804, 0x0e);
    ret += OS08A10_write_register(vi_pipe, 0x3805, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3806, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3807, 0x6f);
    ret += OS08A10_write_register(vi_pipe, 0x3808, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x3809, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x380a, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x380b, 0x70);
    ret += OS08A10_write_register(vi_pipe, 0x380c, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x380d, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x380e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x380f, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x3813, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3814, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3815, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3816, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3817, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x381c, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3820, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3821, 0x24);  /* mirror */
    ret += OS08A10_write_register(vi_pipe, 0x3823, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3826, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3827, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x382d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3832, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x383c, 0x48);
    ret += OS08A10_write_register(vi_pipe, 0x383d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3d84, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += OS08A10_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += OS08A10_write_register(vi_pipe, 0x4000, 0xf8);
    ret += OS08A10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += OS08A10_write_register(vi_pipe, 0x400a, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x400f, 0xa1);
    ret += OS08A10_write_register(vi_pipe, 0x4010, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x4018, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4008, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x4009, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x401a, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4050, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4051, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4028, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x4052, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4053, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4054, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4055, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4056, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4057, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4058, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4059, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x430b, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x430e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4501, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4502, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4643, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4640, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4641, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4800, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4809, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4813, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x4817, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4833, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4837, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x483b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x484b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4850, 0x7c);
    ret += OS08A10_write_register(vi_pipe, 0x4852, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x4856, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4857, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x4862, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x4869, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x486a, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x486e, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x486f, 0x55);
    ret += OS08A10_write_register(vi_pipe, 0x4875, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x5000, 0x89);
    ret += OS08A10_write_register(vi_pipe, 0x5001, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x5004, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x5005, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5180, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5181, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x580b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4700, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4e00, 0x2b);
    OS08A10_default_reg_init(vi_pipe);
    delay_ms(5);   /* delay 5 ms */
#else
    ret += OS08A10_write_register(vi_pipe, 0x0103, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0303, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0305, 0x5c);
    ret += OS08A10_write_register(vi_pipe, 0x0306, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0307, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0308, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x0309, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x032a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x300f, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3010, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3011, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3012, 0x41);
    ret += OS08A10_write_register(vi_pipe, 0x3016, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x301e, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3031, 0xa9);
    ret += OS08A10_write_register(vi_pipe, 0x3603, 0x2c);
    ret += OS08A10_write_register(vi_pipe, 0x3103, 0x92);
    ret += OS08A10_write_register(vi_pipe, 0x3104, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3106, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3400, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3025, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x3425, 0x51);
    ret += OS08A10_write_register(vi_pipe, 0x3428, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3406, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3408, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x340c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x340d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x031e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3501, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3502, 0xe5);
    ret += OS08A10_write_register(vi_pipe, 0x3505, 0x83);
    ret += OS08A10_write_register(vi_pipe, 0x3508, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3509, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350d, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x350e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350f, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3600, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3605, 0x50);
    ret += OS08A10_write_register(vi_pipe, 0x3609, 0xb5);
    ret += OS08A10_write_register(vi_pipe, 0x3610, 0x69);

    ret += OS08A10_write_register(vi_pipe, 0x360c, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x360e, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3628, 0xa4);
    ret += OS08A10_write_register(vi_pipe, 0x362d, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3660, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x3661, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x3662, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3663, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3664, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x366a, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x366b, 0xa0);
    ret += OS08A10_write_register(vi_pipe, 0x366d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x366e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3680, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3705, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3706, 0x35);
    ret += OS08A10_write_register(vi_pipe, 0x370a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x370b, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3709, 0x49);
    ret += OS08A10_write_register(vi_pipe, 0x3714, 0x21);
    ret += OS08A10_write_register(vi_pipe, 0x371c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x371d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x375e, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3762, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3776, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3781, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3782, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3783, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3784, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3785, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3788, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3789, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3797, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3800, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3801, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3802, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3803, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x3804, 0x0e);
    ret += OS08A10_write_register(vi_pipe, 0x3805, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3806, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3807, 0x6f);
    ret += OS08A10_write_register(vi_pipe, 0x3808, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x3809, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x380a, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x380b, 0x70);
    ret += OS08A10_write_register(vi_pipe, 0x380c, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x380d, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x380e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x380f, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x3813, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3814, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3815, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3816, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3817, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x381c, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3820, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3821, 0x20);
    ret += OS08A10_write_register(vi_pipe, 0x3823, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3826, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3827, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x382d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3832, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x383c, 0x48);
    ret += OS08A10_write_register(vi_pipe, 0x383d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3d84, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += OS08A10_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += OS08A10_write_register(vi_pipe, 0x4000, 0xf8);
    ret += OS08A10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += OS08A10_write_register(vi_pipe, 0x400a, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x400f, 0xa1);
    ret += OS08A10_write_register(vi_pipe, 0x4010, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x4018, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4008, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x4009, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x401a, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4050, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4051, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4028, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x4052, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4053, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4054, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4055, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4056, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4057, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4058, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4059, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x430b, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x430e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4501, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4502, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4643, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4640, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4641, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4800, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4809, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4813, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x4817, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4833, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4837, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x483b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x484b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4850, 0x7c);
    ret += OS08A10_write_register(vi_pipe, 0x4852, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x4856, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4857, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x4862, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x4869, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x486a, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x486e, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x486f, 0x55);
    ret += OS08A10_write_register(vi_pipe, 0x4875, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x5000, 0x89);
    ret += OS08A10_write_register(vi_pipe, 0x5001, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x5004, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x5005, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5180, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5181, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x580b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4700, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4e00, 0x2b);
    OS08A10_default_reg_init(vi_pipe);
    delay_ms(5);   /* delay 5 ms */

#endif
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("===OS08A10 sensor 4k2k30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;
}

void OS08A10_wdr_4k2k25_2to1_init(VI_PIPE vi_pipe)
{
    HI_S32 ret = HI_SUCCESS;
    ret += OS08A10_write_register(vi_pipe, 0x0103, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0303, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x0305, 0x4b);
    ret += OS08A10_write_register(vi_pipe, 0x0306, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0307, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x0308, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x0309, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x032a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x300f, 0x11);
    ret += OS08A10_write_register(vi_pipe, 0x3010, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3011, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3012, 0x41);
    ret += OS08A10_write_register(vi_pipe, 0x3016, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x301e, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3031, 0xa9);
    ret += OS08A10_write_register(vi_pipe, 0x3603, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3103, 0x92);
    ret += OS08A10_write_register(vi_pipe, 0x3104, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3106, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3400, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3025, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3425, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3428, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3406, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3408, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x340c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x340d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x031e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x3501, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3502, 0xe5);
    ret += OS08A10_write_register(vi_pipe, 0x3505, 0x83);
    ret += OS08A10_write_register(vi_pipe, 0x3508, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3509, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350a, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x350d, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x350e, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x350f, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3600, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3605, 0x50);
    ret += OS08A10_write_register(vi_pipe, 0x3609, 0xb5);
    ret += OS08A10_write_register(vi_pipe, 0x3610, 0x69);
    ret += OS08A10_write_register(vi_pipe, 0x360c, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x360e, 0x86);
    ret += OS08A10_write_register(vi_pipe, 0x3628, 0xa4);
    ret += OS08A10_write_register(vi_pipe, 0x362d, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3660, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x3661, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x3662, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3663, 0x28);
    ret += OS08A10_write_register(vi_pipe, 0x3664, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x366a, 0x38);
    ret += OS08A10_write_register(vi_pipe, 0x366b, 0xa0);
    ret += OS08A10_write_register(vi_pipe, 0x366d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x366e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3680, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3705, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3706, 0x35);
    ret += OS08A10_write_register(vi_pipe, 0x370a, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x370b, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x3709, 0x49);
    ret += OS08A10_write_register(vi_pipe, 0x3714, 0x21);
    ret += OS08A10_write_register(vi_pipe, 0x371c, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x371d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x375e, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3762, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x3776, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3781, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3782, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3783, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x3784, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3785, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3788, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3789, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3797, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x3800, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3801, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3802, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3803, 0x0c);
    ret += OS08A10_write_register(vi_pipe, 0x3804, 0x0e);
    ret += OS08A10_write_register(vi_pipe, 0x3805, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3806, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3807, 0x6f);
    ret += OS08A10_write_register(vi_pipe, 0x3808, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x3809, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x380a, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x380b, 0x70);
    ret += OS08A10_write_register(vi_pipe, 0x380c, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x380d, 0xdb);
    ret += OS08A10_write_register(vi_pipe, 0x380e, 0x09);
    ret += OS08A10_write_register(vi_pipe, 0x380f, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x3813, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x3814, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3815, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3816, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x3817, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x381c, 0x08);
    /* mirror */
    ret += OS08A10_write_register(vi_pipe, 0x3820, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3821, 0x24);
    ret += OS08A10_write_register(vi_pipe, 0x3823, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3826, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x3827, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x382d, 0x08);
    ret += OS08A10_write_register(vi_pipe, 0x3832, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x383c, 0x48);
    ret += OS08A10_write_register(vi_pipe, 0x383d, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += OS08A10_write_register(vi_pipe, 0x3d84, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += OS08A10_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += OS08A10_write_register(vi_pipe, 0x4000, 0xf8);
    ret += OS08A10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += OS08A10_write_register(vi_pipe, 0x400a, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x400f, 0xa1);
    ret += OS08A10_write_register(vi_pipe, 0x4010, 0x12);
    ret += OS08A10_write_register(vi_pipe, 0x4018, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4008, 0x02);
    ret += OS08A10_write_register(vi_pipe, 0x4009, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x401a, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4050, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4051, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4028, 0x0f);
    ret += OS08A10_write_register(vi_pipe, 0x4052, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4053, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4054, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4055, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4056, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4057, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x4058, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4059, 0x80);
    ret += OS08A10_write_register(vi_pipe, 0x430b, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430c, 0xff);
    ret += OS08A10_write_register(vi_pipe, 0x430d, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x430e, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4501, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4502, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4643, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x4640, 0x01);
    ret += OS08A10_write_register(vi_pipe, 0x4641, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4800, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4809, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4813, 0x98);
    ret += OS08A10_write_register(vi_pipe, 0x4817, 0x04);
    ret += OS08A10_write_register(vi_pipe, 0x4833, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x4837, 0x0d);
    ret += OS08A10_write_register(vi_pipe, 0x483b, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x484b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4850, 0x7c);
    ret += OS08A10_write_register(vi_pipe, 0x4852, 0x06);
    ret += OS08A10_write_register(vi_pipe, 0x4856, 0x58);
    ret += OS08A10_write_register(vi_pipe, 0x4857, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x4862, 0x0a);
    ret += OS08A10_write_register(vi_pipe, 0x4869, 0x18);
    ret += OS08A10_write_register(vi_pipe, 0x486a, 0xaa);
    ret += OS08A10_write_register(vi_pipe, 0x486e, 0x07);
    ret += OS08A10_write_register(vi_pipe, 0x486f, 0x55);
    ret += OS08A10_write_register(vi_pipe, 0x4875, 0xf0);
    ret += OS08A10_write_register(vi_pipe, 0x5000, 0x89);
    ret += OS08A10_write_register(vi_pipe, 0x5001, 0x42);
    ret += OS08A10_write_register(vi_pipe, 0x5004, 0x40);
    ret += OS08A10_write_register(vi_pipe, 0x5005, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5180, 0x00);
    ret += OS08A10_write_register(vi_pipe, 0x5181, 0x10);
    ret += OS08A10_write_register(vi_pipe, 0x580b, 0x03);
    ret += OS08A10_write_register(vi_pipe, 0x4700, 0x2b);
    ret += OS08A10_write_register(vi_pipe, 0x4e00, 0x2b);
    OS08A10_default_reg_init(vi_pipe);
    delay_ms(5);    /* delay 5 ms */
    ret += OS08A10_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    printf("===OS08A10 sensor 4k2k25fps 10bit 2to1 WDR(50fps->25fps) init success!=====\n");
    printf("===MIPI data rate = 1200Mbps/lane =====\n");

    return;
}
