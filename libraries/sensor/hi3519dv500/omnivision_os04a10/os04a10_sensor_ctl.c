/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef OT_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "ot_i2c.h"
#endif

#include "os04a10_cmos.h"
#include "ot_mpi_isp.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};

int os04a10_i2c_init(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        return TD_SUCCESS;
    }
#ifdef OT_GPIO_I2C
    g_fd[vi_pipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open gpioi2c_ex error!\n");
        return TD_FAILURE;
    }
#else
    int ret;
    char dev_file[I2C_DEV_FILE_NUM] = {0};
    td_u8 dev_num;
    ot_isp_sns_commbus *os04a10businfo = TD_NULL;
    os04a10businfo = os04a10_get_bus_info(vi_pipe);
    dev_num = os04a10businfo->i2c_dev;
    (td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);
    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (OS04A10_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int os04a10_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 os04a10_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 os04a10_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = OS04A10_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OS04A10_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OS04A10_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (OS04A10_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (OS04A10_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, OS04A10_ADDR_BYTE + OS04A10_DATA_BYTE);
    if (ret < 0) {
        isp_err_trace("I2C_WRITE error!\n");
        return TD_FAILURE;
    }

#endif
    return TD_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000); /* 1ms: 1000us */
    return;
}

void os04a10_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
    return;
}

void os04a10_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

void os04a10_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static void os04a10_linear_4m30_12bit_init(ot_vi_pipe vi_pipe);
static void os04a10_wdr_2to1_4m30_12bit_init(ot_vi_pipe vi_pipe);

static void os04a10_default_reg_init(ot_vi_pipe vi_pipe)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *pastos04a10 = TD_NULL;

    pastos04a10 = os04a10_get_ctx(vi_pipe);
    for (i = 0; i < pastos04a10->regs_info[0].reg_num; i++) {
        ret += os04a10_write_register(vi_pipe,
            pastos04a10->regs_info[0].i2c_data[i].reg_addr,
            pastos04a10->regs_info[0].i2c_data[i].data);
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }

    return;
}

static td_bool os04a10_get_quick_start_flag(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    ot_isp_ctrl_param isp_ctrl_param = {};

    ret = ot_mpi_isp_get_ctrl_param(vi_pipe, &isp_ctrl_param);
    if (ret != TD_SUCCESS) {
        isp_err_trace("pipe[%d] call ot_mpi_isp_get_ctrl_param failed! ret = 0x%x, quick_start_flag force set to 0!\n",
            vi_pipe, ret);
        return TD_FALSE;
    }

    return isp_ctrl_param.quick_start_en;
}

void os04a10_init(ot_vi_pipe vi_pipe)
{
    ot_wdr_mode      wdr_mode;
    td_bool          init;
    td_u8            img_mode;
    td_s32           ret;
    td_bool          quick_start_flag = TD_FALSE;
    ot_isp_sns_state *pastos04a10 = TD_NULL;

    pastos04a10 = os04a10_get_ctx(vi_pipe);
    init        = pastos04a10->init;
    wdr_mode    = pastos04a10->wdr_mode;
    img_mode    = pastos04a10->img_mode;

    ret = os04a10_i2c_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    quick_start_flag = os04a10_get_quick_start_flag(vi_pipe);
    /* When sensor first init, config all registers */
    if (init == TD_FALSE) {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (OS04A10_4M_30FPS_12BIT_2TO1_VC_MODE == img_mode) {  /* OS04A10_4M_30FPS_12BIT_2TO1_VC_MODE */
                os04a10_wdr_2to1_4m30_12bit_init(vi_pipe);
            } else {
            }
        } else if (quick_start_flag == TD_FALSE) {
            os04a10_linear_4m30_12bit_init(vi_pipe);
        }
    } else {
        /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (OS04A10_4M_30FPS_12BIT_2TO1_VC_MODE == img_mode) {  /* OS04A10_4M_30FPS_12BIT_2TO1_VC_MODE */
                os04a10_wdr_2to1_4m30_12bit_init(vi_pipe);
            } else {
            }
        } else if (quick_start_flag == TD_FALSE) {
            os04a10_linear_4m30_12bit_init(vi_pipe);
        }
    }

    pastos04a10->init = TD_TRUE;
    return;
}

void os04a10_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;

    ret = os04a10_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("OS04A10 exit failed!\n");
    }

    return;
}

static td_s32 os04a10_linear_4m30_12bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x0102, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0305, 0x42);
    ret += os04a10_write_register(vi_pipe, 0x0306, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0307, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0308, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x030a, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x0317, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x0322, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x0323, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x0324, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0325, 0x90);
    ret += os04a10_write_register(vi_pipe, 0x0327, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x0329, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x032c, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x032d, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x032e, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x300f, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x3012, 0x41);
    ret += os04a10_write_register(vi_pipe, 0x3026, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3027, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x302d, 0x24);
    ret += os04a10_write_register(vi_pipe, 0x3104, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3106, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x3400, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3408, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x340c, 0x0c);
    ret += os04a10_write_register(vi_pipe, 0x340d, 0xb0);
    ret += os04a10_write_register(vi_pipe, 0x3425, 0x51);
    ret += os04a10_write_register(vi_pipe, 0x3426, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3427, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x3428, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3429, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x342a, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x342b, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x3501, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3504, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3508, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3509, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x350a, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3544, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3548, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3549, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3584, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3588, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3589, 0x00);

    return ret;
}

static td_s32 os04a10_linear_4m30_12bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x3601, 0x70);
    ret += os04a10_write_register(vi_pipe, 0x3604, 0xe3);
    ret += os04a10_write_register(vi_pipe, 0x3605, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x3606, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3608, 0xa8);
    ret += os04a10_write_register(vi_pipe, 0x360a, 0xd0);
    ret += os04a10_write_register(vi_pipe, 0x360b, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x360e, 0xc8);
    ret += os04a10_write_register(vi_pipe, 0x360f, 0x66);
    ret += os04a10_write_register(vi_pipe, 0x3610, 0x89);
    ret += os04a10_write_register(vi_pipe, 0x3611, 0x8a);
    ret += os04a10_write_register(vi_pipe, 0x3612, 0x4e);
    ret += os04a10_write_register(vi_pipe, 0x3613, 0xbd);
    ret += os04a10_write_register(vi_pipe, 0x3614, 0x9b);
    ret += os04a10_write_register(vi_pipe, 0x362a, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362b, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362c, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362d, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x362e, 0x1a);
    ret += os04a10_write_register(vi_pipe, 0x362f, 0x34);
    ret += os04a10_write_register(vi_pipe, 0x3630, 0x67);
    ret += os04a10_write_register(vi_pipe, 0x3631, 0x7f);
    ret += os04a10_write_register(vi_pipe, 0x3638, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3643, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3644, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3645, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3646, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3647, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3648, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3649, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x364a, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x364c, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x364d, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x364e, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x364f, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x3650, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x3651, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x365a, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x365b, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x365c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x365d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3661, 0x07);
    ret += os04a10_write_register(vi_pipe, 0x3662, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3663, 0x20);
    ret += os04a10_write_register(vi_pipe, 0x3665, 0x12);

    return ret;
}

static td_s32 os04a10_linear_4m30_12bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x3667, 0xd4);
    ret += os04a10_write_register(vi_pipe, 0x3668, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x366c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x366d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x366e, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x366f, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3671, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3673, 0x2a);
    ret += os04a10_write_register(vi_pipe, 0x3681, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x3700, 0x2d);
    ret += os04a10_write_register(vi_pipe, 0x3701, 0x22);
    ret += os04a10_write_register(vi_pipe, 0x3702, 0x25);
    ret += os04a10_write_register(vi_pipe, 0x3703, 0x28);
    ret += os04a10_write_register(vi_pipe, 0x3705, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3706, 0xf0);
    ret += os04a10_write_register(vi_pipe, 0x3707, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3708, 0x36);
    ret += os04a10_write_register(vi_pipe, 0x3709, 0x57);
    ret += os04a10_write_register(vi_pipe, 0x370a, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x370b, 0x15);
    ret += os04a10_write_register(vi_pipe, 0x3714, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3719, 0x24);
    ret += os04a10_write_register(vi_pipe, 0x371b, 0x1f);
    ret += os04a10_write_register(vi_pipe, 0x371c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x371d, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x373f, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3740, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3741, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3742, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3743, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3756, 0xe7);
    ret += os04a10_write_register(vi_pipe, 0x3757, 0xe7);
    ret += os04a10_write_register(vi_pipe, 0x3762, 0x1c);
    ret += os04a10_write_register(vi_pipe, 0x376c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3776, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x3777, 0x22);
    ret += os04a10_write_register(vi_pipe, 0x3779, 0x60);
    ret += os04a10_write_register(vi_pipe, 0x377c, 0x48);
    ret += os04a10_write_register(vi_pipe, 0x3784, 0x06);
    ret += os04a10_write_register(vi_pipe, 0x3785, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3790, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3793, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x3794, 0x07);
    ret += os04a10_write_register(vi_pipe, 0x3796, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3797, 0x02);

    return ret;
}

static td_s32 os04a10_linear_4m30_12bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x379c, 0x4d);
    ret += os04a10_write_register(vi_pipe, 0x37a1, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x37bb, 0x88);
    ret += os04a10_write_register(vi_pipe, 0x37be, 0x48);
    ret += os04a10_write_register(vi_pipe, 0x37bf, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37c0, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37c4, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37c5, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37c6, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37ca, 0x21);
    ret += os04a10_write_register(vi_pipe, 0x37cc, 0x15);
    ret += os04a10_write_register(vi_pipe, 0x37cd, 0x90);
    ret += os04a10_write_register(vi_pipe, 0x37cf, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x37d0, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37d1, 0xf0);
    ret += os04a10_write_register(vi_pipe, 0x37d2, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x37d3, 0x15);
    ret += os04a10_write_register(vi_pipe, 0x37d4, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37d5, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37d6, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x37d7, 0x15);
    ret += os04a10_write_register(vi_pipe, 0x37d8, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37dc, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37dd, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37da, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37db, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3800, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3801, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3802, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3803, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3804, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3805, 0x8f);
    ret += os04a10_write_register(vi_pipe, 0x3806, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x3807, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x3808, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3809, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x380a, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x380b, 0xf0);
    ret += os04a10_write_register(vi_pipe, 0x380c, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x380d, 0xc4);
    ret += os04a10_write_register(vi_pipe, 0x380e, 0x06);
    ret += os04a10_write_register(vi_pipe, 0x380f, 0x58);
    ret += os04a10_write_register(vi_pipe, 0x3811, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x3813, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x3814, 0x01);

    return ret;
}

static td_s32 os04a10_linear_4m30_12bit_init_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x3815, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3816, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3817, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x381c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3820, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3821, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3822, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x3823, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x3826, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3827, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3833, 0x40);
    ret += os04a10_write_register(vi_pipe, 0x384c, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x384d, 0xc4);
    ret += os04a10_write_register(vi_pipe, 0x3858, 0x3c);
    ret += os04a10_write_register(vi_pipe, 0x3865, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3866, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3867, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3868, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3900, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x3940, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x3980, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x3c01, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x3c05, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3c0f, 0x1c);
    ret += os04a10_write_register(vi_pipe, 0x3c12, 0x0d);
    ret += os04a10_write_register(vi_pipe, 0x3c19, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3c21, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3c3a, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3c3b, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x3c3d, 0xc6);
    ret += os04a10_write_register(vi_pipe, 0x3c55, 0xcb);
    ret += os04a10_write_register(vi_pipe, 0x3c5a, 0xe5);
    ret += os04a10_write_register(vi_pipe, 0x3c5d, 0xcf);
    ret += os04a10_write_register(vi_pipe, 0x3c5e, 0xcf);
    ret += os04a10_write_register(vi_pipe, 0x3d8c, 0x70);
    ret += os04a10_write_register(vi_pipe, 0x3d8d, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x4000, 0xf9);
    ret += os04a10_write_register(vi_pipe, 0x4001, 0x2f);
    ret += os04a10_write_register(vi_pipe, 0x4004, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x4005, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4008, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x4009, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x400a, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x400b, 0x27);
    ret += os04a10_write_register(vi_pipe, 0x400e, 0x40);

    return ret;
}

static td_s32 os04a10_linear_4m30_12bit_init_part6(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x402e, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x402f, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4030, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4031, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x4032, 0x9f);
    ret += os04a10_write_register(vi_pipe, 0x4033, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x4050, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4051, 0x07);
    ret += os04a10_write_register(vi_pipe, 0x4011, 0xbb);
    ret += os04a10_write_register(vi_pipe, 0x410f, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x4288, 0xcf);
    ret += os04a10_write_register(vi_pipe, 0x4289, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x428a, 0x46);
    ret += os04a10_write_register(vi_pipe, 0x430b, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x430c, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x430d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x430e, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4314, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x4500, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x4501, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x4503, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x4504, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4506, 0x32);
    ret += os04a10_write_register(vi_pipe, 0x4507, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x4601, 0x30);
    ret += os04a10_write_register(vi_pipe, 0x4603, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x460a, 0x50);
    ret += os04a10_write_register(vi_pipe, 0x460c, 0x60);
    ret += os04a10_write_register(vi_pipe, 0x4640, 0x62);
    ret += os04a10_write_register(vi_pipe, 0x4646, 0xaa);
    ret += os04a10_write_register(vi_pipe, 0x4647, 0x55);
    ret += os04a10_write_register(vi_pipe, 0x4648, 0x99);
    ret += os04a10_write_register(vi_pipe, 0x4649, 0x66);
    ret += os04a10_write_register(vi_pipe, 0x464d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4654, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x4655, 0x22);
    ret += os04a10_write_register(vi_pipe, 0x4800, 0x44);
    ret += os04a10_write_register(vi_pipe, 0x480e, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4810, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x4811, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x4813, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x481f, 0x30);
    ret += os04a10_write_register(vi_pipe, 0x4837, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x484b, 0x27);
    ret += os04a10_write_register(vi_pipe, 0x4d00, 0x4d);

    return ret;
}

static td_s32 os04a10_linear_4m30_12bit_init_part7(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x4d01, 0x9d);
    ret += os04a10_write_register(vi_pipe, 0x4d02, 0xb9);
    ret += os04a10_write_register(vi_pipe, 0x4d03, 0x2e);
    ret += os04a10_write_register(vi_pipe, 0x4d04, 0x4a);
    ret += os04a10_write_register(vi_pipe, 0x4d05, 0x3d);
    ret += os04a10_write_register(vi_pipe, 0x4d09, 0x4f);
    ret += os04a10_write_register(vi_pipe, 0x5000, 0x17);
    ret += os04a10_write_register(vi_pipe, 0x5001, 0x0d);
    ret += os04a10_write_register(vi_pipe, 0x5080, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x50c0, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5100, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5200, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5201, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5202, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x5203, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x5780, 0x53);
    ret += os04a10_write_register(vi_pipe, 0x5782, 0x60);
    ret += os04a10_write_register(vi_pipe, 0x5783, 0xf0);
    ret += os04a10_write_register(vi_pipe, 0x5786, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x5788, 0x60);
    ret += os04a10_write_register(vi_pipe, 0x5789, 0xf0);
    ret += os04a10_write_register(vi_pipe, 0x5792, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x5793, 0x33);
    ret += os04a10_write_register(vi_pipe, 0x5857, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x5858, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x5859, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x58d7, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x58d8, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x58d9, 0xff);

    os04a10_default_reg_init(vi_pipe);

    delay_ms(0x5);
    ret += os04a10_write_register(vi_pipe, 0x0100, 0x01);
    return ret;
}

static void os04a10_linear_4m30_12bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += os04a10_linear_4m30_12bit_init_part1(vi_pipe);
    ret += os04a10_linear_4m30_12bit_init_part2(vi_pipe);
    ret += os04a10_linear_4m30_12bit_init_part3(vi_pipe);
    ret += os04a10_linear_4m30_12bit_init_part4(vi_pipe);
    ret += os04a10_linear_4m30_12bit_init_part5(vi_pipe);
    ret += os04a10_linear_4m30_12bit_init_part6(vi_pipe);
    ret += os04a10_linear_4m30_12bit_init_part7(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os04a10 write register failed!\n");
        return;
    }

    printf("========================================================================\n");
    printf("vi_pipe:%d,== os04a10 24Mclk 4M30fps(MIPI) 12bit linear init success! ==\n", vi_pipe);
    printf("========================================================================\n");
    return;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x0103, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x0109, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x0104, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x0102, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0305, 0x3c);
    ret += os04a10_write_register(vi_pipe, 0x0306, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0307, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0308, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x030a, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x0317, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x0322, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x0323, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x0324, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x0325, 0x90);
    ret += os04a10_write_register(vi_pipe, 0x0327, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x0329, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x032c, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x032d, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x032e, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x300f, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x3012, 0x41);
    ret += os04a10_write_register(vi_pipe, 0x3026, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3027, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x302d, 0x24);
    ret += os04a10_write_register(vi_pipe, 0x3104, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3106, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x3400, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3408, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x340c, 0x0c);
    ret += os04a10_write_register(vi_pipe, 0x340d, 0xb0);
    ret += os04a10_write_register(vi_pipe, 0x3425, 0x51);
    ret += os04a10_write_register(vi_pipe, 0x3426, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3427, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x3428, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3429, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x342a, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x342b, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x3501, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3504, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3508, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3509, 0x00);

    return ret;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x350a, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3544, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3548, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3549, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3584, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3588, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3589, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3601, 0x70);
    ret += os04a10_write_register(vi_pipe, 0x3604, 0xe3);
    ret += os04a10_write_register(vi_pipe, 0x3605, 0x7f);
    ret += os04a10_write_register(vi_pipe, 0x3606, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x3608, 0xa8);
    ret += os04a10_write_register(vi_pipe, 0x360a, 0xd0);
    ret += os04a10_write_register(vi_pipe, 0x360b, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x360e, 0xc8);
    ret += os04a10_write_register(vi_pipe, 0x360f, 0x66);
    ret += os04a10_write_register(vi_pipe, 0x3610, 0x89);
    ret += os04a10_write_register(vi_pipe, 0x3611, 0x8a);
    ret += os04a10_write_register(vi_pipe, 0x3612, 0x4e);
    ret += os04a10_write_register(vi_pipe, 0x3613, 0xbd);
    ret += os04a10_write_register(vi_pipe, 0x3614, 0x9b);
    ret += os04a10_write_register(vi_pipe, 0x362a, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362b, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362c, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362d, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x362e, 0x1a);
    ret += os04a10_write_register(vi_pipe, 0x362f, 0x34);
    ret += os04a10_write_register(vi_pipe, 0x3630, 0x67);
    ret += os04a10_write_register(vi_pipe, 0x3631, 0x7f);
    ret += os04a10_write_register(vi_pipe, 0x3638, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3643, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3644, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3645, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3646, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3647, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3648, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3649, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x364a, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x364c, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x364d, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x364e, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x364f, 0x0e);

    return ret;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x3650, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x3651, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x365a, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x365b, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x365c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x365d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3661, 0x07);
    ret += os04a10_write_register(vi_pipe, 0x3662, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3663, 0x20);
    ret += os04a10_write_register(vi_pipe, 0x3665, 0x12);
    ret += os04a10_write_register(vi_pipe, 0x3667, 0x54);
    ret += os04a10_write_register(vi_pipe, 0x3668, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x366c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x366d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x366e, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x366f, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3671, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x3673, 0x2a);
    ret += os04a10_write_register(vi_pipe, 0x3681, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x3700, 0x2d);
    ret += os04a10_write_register(vi_pipe, 0x3701, 0x22);
    ret += os04a10_write_register(vi_pipe, 0x3702, 0x25);
    ret += os04a10_write_register(vi_pipe, 0x3703, 0x20);
    ret += os04a10_write_register(vi_pipe, 0x3705, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3706, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x3707, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3708, 0x36);
    ret += os04a10_write_register(vi_pipe, 0x3709, 0x57);
    ret += os04a10_write_register(vi_pipe, 0x370a, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x370b, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x3714, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3719, 0x1f);
    ret += os04a10_write_register(vi_pipe, 0x371b, 0x16);
    ret += os04a10_write_register(vi_pipe, 0x371c, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x371d, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x373f, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3740, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3741, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3742, 0x63);
    ret += os04a10_write_register(vi_pipe, 0x3743, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3756, 0x9d);
    ret += os04a10_write_register(vi_pipe, 0x3757, 0x9d);

    return ret;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x3762, 0x1c);
    ret += os04a10_write_register(vi_pipe, 0x376c, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x3776, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x3777, 0x22);
    ret += os04a10_write_register(vi_pipe, 0x3779, 0x60);
    ret += os04a10_write_register(vi_pipe, 0x377c, 0x48);
    ret += os04a10_write_register(vi_pipe, 0x3784, 0x06);
    ret += os04a10_write_register(vi_pipe, 0x3785, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3790, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3793, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x3794, 0x07);
    ret += os04a10_write_register(vi_pipe, 0x3796, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3797, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x379c, 0x4d);
    ret += os04a10_write_register(vi_pipe, 0x37a1, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x37bb, 0x88);
    ret += os04a10_write_register(vi_pipe, 0x37be, 0x48);
    ret += os04a10_write_register(vi_pipe, 0x37bf, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37c0, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37c4, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37c5, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37c6, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37ca, 0x21);
    ret += os04a10_write_register(vi_pipe, 0x37cc, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x37cd, 0x90);
    ret += os04a10_write_register(vi_pipe, 0x37cf, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x37d0, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37d1, 0x72);
    ret += os04a10_write_register(vi_pipe, 0x37d2, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37d3, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x37d4, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37d5, 0x6c);
    ret += os04a10_write_register(vi_pipe, 0x37d6, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37d7, 0xf7);
    ret += os04a10_write_register(vi_pipe, 0x37d8, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x37dc, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37dd, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37da, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x37db, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3800, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3801, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3802, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3803, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3804, 0x0a);
    return ret;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += os04a10_write_register(vi_pipe, 0x3805, 0x8f);
    ret += os04a10_write_register(vi_pipe, 0x3806, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x3807, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x3808, 0x0a);
    ret += os04a10_write_register(vi_pipe, 0x3809, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x380a, 0x05);
    ret += os04a10_write_register(vi_pipe, 0x380b, 0xf0);
    ret += os04a10_write_register(vi_pipe, 0x380c, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x380d, 0xdc);
    ret += os04a10_write_register(vi_pipe, 0x380e, 0x06);
    ret += os04a10_write_register(vi_pipe, 0x380f, 0x58);
    ret += os04a10_write_register(vi_pipe, 0x3811, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x3813, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x3814, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3815, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3816, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x3817, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x381c, 0x08);
    ret += os04a10_write_register(vi_pipe, 0x3820, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x3821, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3822, 0x14);
    ret += os04a10_write_register(vi_pipe, 0x3823, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x3826, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3827, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3833, 0x41);
    ret += os04a10_write_register(vi_pipe, 0x384c, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x384d, 0xdc);
    ret += os04a10_write_register(vi_pipe, 0x3858, 0x3c);
    ret += os04a10_write_register(vi_pipe, 0x3865, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3866, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3867, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3868, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x3900, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x3940, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x3980, 0x13);
    ret += os04a10_write_register(vi_pipe, 0x3c01, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x3c05, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3c0f, 0x1c);
    ret += os04a10_write_register(vi_pipe, 0x3c12, 0x0d);
    ret += os04a10_write_register(vi_pipe, 0x3c19, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3c21, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x3c3a, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x3c3b, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x3c3d, 0xc6);
    ret += os04a10_write_register(vi_pipe, 0x3c55, 0xcb);
    return ret;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part6(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x3c5a, 0x55);
    ret += os04a10_write_register(vi_pipe, 0x3c5d, 0xcf);
    ret += os04a10_write_register(vi_pipe, 0x3c5e, 0xcf);
    ret += os04a10_write_register(vi_pipe, 0x3d8c, 0x70);
    ret += os04a10_write_register(vi_pipe, 0x3d8d, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x4000, 0xf9);
    ret += os04a10_write_register(vi_pipe, 0x4001, 0xef);
    ret += os04a10_write_register(vi_pipe, 0x4004, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4005, 0x40);
    ret += os04a10_write_register(vi_pipe, 0x4008, 0x02);
    ret += os04a10_write_register(vi_pipe, 0x4009, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x400a, 0x06);
    ret += os04a10_write_register(vi_pipe, 0x400b, 0x40);
    ret += os04a10_write_register(vi_pipe, 0x400e, 0x40);
    ret += os04a10_write_register(vi_pipe, 0x402e, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x402f, 0x40);
    ret += os04a10_write_register(vi_pipe, 0x4030, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4031, 0x40);
    ret += os04a10_write_register(vi_pipe, 0x4032, 0x0f);
    ret += os04a10_write_register(vi_pipe, 0x4033, 0x80);
    ret += os04a10_write_register(vi_pipe, 0x4050, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4051, 0x07);
    ret += os04a10_write_register(vi_pipe, 0x4011, 0xbb);
    ret += os04a10_write_register(vi_pipe, 0x410f, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x4288, 0xce);
    ret += os04a10_write_register(vi_pipe, 0x4289, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x428a, 0x46);
    ret += os04a10_write_register(vi_pipe, 0x430b, 0x0f);
    ret += os04a10_write_register(vi_pipe, 0x430c, 0xfc);
    ret += os04a10_write_register(vi_pipe, 0x430d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x430e, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4314, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x4500, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x4501, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x4503, 0x10);
    ret += os04a10_write_register(vi_pipe, 0x4504, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4506, 0x32);
    ret += os04a10_write_register(vi_pipe, 0x4507, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x4601, 0x30);
    ret += os04a10_write_register(vi_pipe, 0x4603, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x460a, 0x50);
    ret += os04a10_write_register(vi_pipe, 0x460c, 0x60);
    ret += os04a10_write_register(vi_pipe, 0x4640, 0x62);
    ret += os04a10_write_register(vi_pipe, 0x4646, 0xaa);

    return ret;
}

static td_s32 os04a10_wdr_2to1_4m30_12bit_part7(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os04a10_write_register(vi_pipe, 0x4647, 0x55);
    ret += os04a10_write_register(vi_pipe, 0x4648, 0x99);
    ret += os04a10_write_register(vi_pipe, 0x4649, 0x66);
    ret += os04a10_write_register(vi_pipe, 0x464d, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x4654, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x4655, 0x22);
    ret += os04a10_write_register(vi_pipe, 0x4800, 0x44);
    ret += os04a10_write_register(vi_pipe, 0x480e, 0x04);
    ret += os04a10_write_register(vi_pipe, 0x4810, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x4811, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x4813, 0x84);
    ret += os04a10_write_register(vi_pipe, 0x481f, 0x30);
    ret += os04a10_write_register(vi_pipe, 0x4837, 0x0e);
    ret += os04a10_write_register(vi_pipe, 0x484b, 0x67);
    ret += os04a10_write_register(vi_pipe, 0x4d00, 0x4d);
    ret += os04a10_write_register(vi_pipe, 0x4d01, 0x9d);
    ret += os04a10_write_register(vi_pipe, 0x4d02, 0xb9);
    ret += os04a10_write_register(vi_pipe, 0x4d03, 0x2e);
    ret += os04a10_write_register(vi_pipe, 0x4d04, 0x4a);
    ret += os04a10_write_register(vi_pipe, 0x4d05, 0x3d);
    ret += os04a10_write_register(vi_pipe, 0x4d09, 0x4f);
    ret += os04a10_write_register(vi_pipe, 0x5000, 0x17);
    ret += os04a10_write_register(vi_pipe, 0x5001, 0x0c);
    ret += os04a10_write_register(vi_pipe, 0x5080, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x50c0, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5100, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5200, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5201, 0x00);
    ret += os04a10_write_register(vi_pipe, 0x5202, 0x03);
    ret += os04a10_write_register(vi_pipe, 0x5203, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x5780, 0x53);
    ret += os04a10_write_register(vi_pipe, 0x5782, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x5783, 0x3c);
    ret += os04a10_write_register(vi_pipe, 0x5786, 0x01);
    ret += os04a10_write_register(vi_pipe, 0x5788, 0x18);
    ret += os04a10_write_register(vi_pipe, 0x5789, 0x3c);
    ret += os04a10_write_register(vi_pipe, 0x5792, 0x11);
    ret += os04a10_write_register(vi_pipe, 0x5793, 0x33);
    ret += os04a10_write_register(vi_pipe, 0x5857, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x5858, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x5859, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x58d7, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x58d8, 0xff);
    ret += os04a10_write_register(vi_pipe, 0x58d9, 0xff);
    return ret;
}

static void os04a10_wdr_2to1_4m30_12bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += os04a10_wdr_2to1_4m30_12bit_part1(vi_pipe);
    ret += os04a10_wdr_2to1_4m30_12bit_part2(vi_pipe);
    ret += os04a10_wdr_2to1_4m30_12bit_part3(vi_pipe);
    ret += os04a10_wdr_2to1_4m30_12bit_part4(vi_pipe);
    ret += os04a10_wdr_2to1_4m30_12bit_part5(vi_pipe);
    ret += os04a10_wdr_2to1_4m30_12bit_part6(vi_pipe);
    ret += os04a10_wdr_2to1_4m30_12bit_part7(vi_pipe);

    ret += os04a10_write_register(vi_pipe, 0x0305, 0x5c); // add WDR VTS Blank
    ret += os04a10_write_register(vi_pipe, 0x0325, 0xd8);
    ret += os04a10_write_register(vi_pipe, 0x380e, 0x09);
    ret += os04a10_write_register(vi_pipe, 0x380f, 0x84);

    os04a10_default_reg_init(vi_pipe);
    delay_ms(5);   /* delay 5 ms */
    ret += os04a10_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os04a10 write register failed!\n");
        return;
    }
    printf("========================================================================\n");
    printf("vi_pipe:%d,== os04a10 24Mclk 4M30fps(MIPI) 12bit wdr2to1 init success! ==\n", vi_pipe);
    printf("========================================================================\n");
    return;
}
