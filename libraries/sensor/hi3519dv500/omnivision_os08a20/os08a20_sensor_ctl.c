/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef OT_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "ot_i2c.h"
#endif

#include "os08a20_cmos.h"
#include "ot_mpi_isp.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};

int os08a20_i2c_init(ot_vi_pipe vi_pipe)
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
    ot_isp_sns_commbus *os08a20businfo = TD_NULL;
    os08a20businfo = os08a20_get_bus_info(vi_pipe);
    dev_num = os08a20businfo->i2c_dev;
    (td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (OS08A20_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int os08a20_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 os08a20_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 os08a20_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = OS08A20_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OS08A20_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OS08A20_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (OS08A20_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (OS08A20_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, OS08A20_ADDR_BYTE + OS08A20_DATA_BYTE);
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

void os08a20_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
    return;
}

void os08a20_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

void os08a20_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

td_void os08a20_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp)
{
    td_s32 ret = TD_SUCCESS;

    os08a20_set_blc_clamp_value(vi_pipe, blc_clamp.blc_clamp_en);

    if (blc_clamp.blc_clamp_en == TD_TRUE) {
        ret += os08a20_write_register(vi_pipe, 0x4001, 0xeb);  /* clamp on */
    } else {
        ret += os08a20_write_register(vi_pipe, 0x4001, 0xea);  /* clamp off */
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init(ot_vi_pipe vi_pipe);
static td_s32 os08a20_linear_8m30_12bit_init(ot_vi_pipe vi_pipe);
static td_s32 os08a20_linear_2m60_10bit_init(ot_vi_pipe vi_pipe);
static td_s32 os08a20_flex_init(ot_vi_pipe vi_pipe, td_bool bin);

static td_s32 os08a20_default_reg_init(ot_vi_pipe vi_pipe)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *pastos08a20 = TD_NULL;

    pastos08a20 = os08a20_get_ctx(vi_pipe);
    for (i = 0; i < pastos08a20->regs_info[0].reg_num; i++) {
        if ((i == EXPO_L_IDX || i == WDR_EXPO_L_IDX) && ((pastos08a20->regs_info[0].i2c_data[i].data & 0x07) == 0x07)) {
            pastos08a20->regs_info[0].i2c_data[i].data = ((pastos08a20->regs_info[0].i2c_data[i].data >> 1) << 1);
        }
        ret += os08a20_write_register(vi_pipe,
            pastos08a20->regs_info[0].i2c_data[i].reg_addr,
            pastos08a20->regs_info[0].i2c_data[i].data);
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }

    return ret;
}

static td_bool os08a20_get_quick_start_flag(ot_vi_pipe vi_pipe)
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

void os08a20_init(ot_vi_pipe vi_pipe)
{
    ot_wdr_mode      wdr_mode;
    td_u8            img_mode;
    td_s32           ret;
    td_bool          quick_start_flag = TD_FALSE;
    td_s32           init_ret = TD_SUCCESS;
    ot_isp_sns_state *pastos08a20 = TD_NULL;

    pastos08a20 = os08a20_get_ctx(vi_pipe);
    wdr_mode    = pastos08a20->wdr_mode;
    img_mode    = pastos08a20->img_mode;

    ret = os08a20_i2c_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    quick_start_flag = os08a20_get_quick_start_flag(vi_pipe);
    /* When quick start mode, sensor init in boot, don't init sensor here */
    if (quick_start_flag == TD_FALSE) {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {  /* OS08A20_VMAX_8M_30FPS_10BIT_2TO1_WDR */
                init_ret = os08a20_vc_wdr_2t1_8m30_10bit_init(vi_pipe);
            } else {
            }
        } else if (img_mode == OS08A20_2M_60FPS_10BIT_LINEAR_MODE) {
            init_ret = os08a20_linear_2m60_10bit_init(vi_pipe);
        } else if (img_mode == OS08A20_FLEX_12BIT_LINEAR_MODE) {
            init_ret = os08a20_flex_init(vi_pipe, TD_FALSE);
        } else if (img_mode == OS08A20_FLEX_BIN_10BIT_LINEAR_MODE) {
            init_ret = os08a20_flex_init(vi_pipe, TD_TRUE);
        } else {
            init_ret = os08a20_linear_8m30_12bit_init(vi_pipe);
        }
    }

    /* Only a sensor that took every register is ready: marked initialised
     * after a failed write, it is never set up again. */
    if (init_ret != TD_SUCCESS) {
        isp_err_trace("pipe[%d] os08a20 init failed!\n", vi_pipe);
        return;
    }
    pastos08a20->init = TD_TRUE;
    return;
}

void os08a20_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;

    ret = os08a20_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("OS08A20 exit failed!\n");
    }

    return;
}

static td_s32 os08a20_linear_8m30_12bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x0100, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x0103, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x0303, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x0305, 0x5a);
    ret += os08a20_write_register(vi_pipe, 0x0306, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x0308, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x0309, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x032a, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x300f, 0x11);
    ret += os08a20_write_register(vi_pipe, 0x3010, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3011, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3012, 0x41);
    ret += os08a20_write_register(vi_pipe, 0x3016, 0xf0);
    ret += os08a20_write_register(vi_pipe, 0x301e, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x3031, 0xa9);
    ret += os08a20_write_register(vi_pipe, 0x3103, 0x92);
    ret += os08a20_write_register(vi_pipe, 0x3104, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3106, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3400, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3025, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x3425, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3428, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3406, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3408, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x340c, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x340d, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x031e, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0xe5);
    ret += os08a20_write_register(vi_pipe, 0x3505, 0x83);
    ret += os08a20_write_register(vi_pipe, 0x3508, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3509, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x350a, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x350b, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x350c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x350d, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x350e, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x350f, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3600, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3603, 0x2c);
    ret += os08a20_write_register(vi_pipe, 0x3605, 0x50);
    ret += os08a20_write_register(vi_pipe, 0x3609, 0xdb);
    ret += os08a20_write_register(vi_pipe, 0x3610, 0x39);
    ret += os08a20_write_register(vi_pipe, 0x360c, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3628, 0xa4);

    return ret;
}

static td_s32 os08a20_linear_8m30_12bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x362d, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3660, 0xd3);
    ret += os08a20_write_register(vi_pipe, 0x3661, 0x06);
    ret += os08a20_write_register(vi_pipe, 0x3662, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3663, 0x28);
    ret += os08a20_write_register(vi_pipe, 0x3664, 0x0d);
    ret += os08a20_write_register(vi_pipe, 0x366a, 0x38);
    ret += os08a20_write_register(vi_pipe, 0x366b, 0xa0);
    ret += os08a20_write_register(vi_pipe, 0x366d, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x366e, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3680, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x36c0, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3701, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x373b, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x373c, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3736, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3737, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3705, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3706, 0x72);
    ret += os08a20_write_register(vi_pipe, 0x370a, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x370b, 0x30);
    ret += os08a20_write_register(vi_pipe, 0x3709, 0x48);
    ret += os08a20_write_register(vi_pipe, 0x3714, 0x21);
    ret += os08a20_write_register(vi_pipe, 0x371c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x371d, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3740, 0x1b);
    ret += os08a20_write_register(vi_pipe, 0x3741, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x375e, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x3760, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3776, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3781, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3782, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3783, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3784, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3785, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3788, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3789, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3797, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3762, 0x11);
    ret += os08a20_write_register(vi_pipe, 0x3800, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3801, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3802, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3803, 0x00); /* was 0x0c: see 0x3813 */
    ret += os08a20_write_register(vi_pipe, 0x3804, 0x0e);
    ret += os08a20_write_register(vi_pipe, 0x3805, 0xff);

    return ret;
}

static td_s32 os08a20_linear_8m30_12bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x3806, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3807, 0x8f); /* was 0x73: the whole array, rows 0..2191 */
    ret += os08a20_write_register(vi_pipe, 0x3808, 0x0f);
    ret += os08a20_write_register(vi_pipe, 0x3809, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x380a, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x380b, 0x88);
    ret += os08a20_write_register(vi_pipe, 0x380c, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x380d, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x380e, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x380f, 0x0a);
    /* Was 0x00, and the first ~18 rows out were black: the output has to
     * start 24 rows into the window (measured: 16 left 4 dark rows, 24 none).
     * With the window from row 0, the 2160 rows VI keeps are array rows
     * 24..2183. */
    ret += os08a20_write_register(vi_pipe, 0x3813, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x3814, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3815, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3816, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3817, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x381c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3820, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3821, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3823, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3826, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3827, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x382d, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3832, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3833, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x383c, 0x48);
    ret += os08a20_write_register(vi_pipe, 0x383d, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x3d84, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += os08a20_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += os08a20_write_register(vi_pipe, 0x4000, 0xf8);
    ret += os08a20_write_register(vi_pipe, 0x4001, 0xeb);
    ret += os08a20_write_register(vi_pipe, 0x4004, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4005, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x400a, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x400f, 0xa0);
    ret += os08a20_write_register(vi_pipe, 0x4010, 0x12);
    ret += os08a20_write_register(vi_pipe, 0x4018, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4008, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x4009, 0x0d);
    ret += os08a20_write_register(vi_pipe, 0x401a, 0x58);
    ret += os08a20_write_register(vi_pipe, 0x4050, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4051, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4028, 0x2f);

    return ret;
}

static td_s32 os08a20_linear_8m30_12bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x4052, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4053, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4054, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4055, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4056, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4057, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4058, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4059, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x430b, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x430c, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x430d, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x430e, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4501, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x4502, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4600, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4601, 0x20);
    ret += os08a20_write_register(vi_pipe, 0x4603, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4643, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4640, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4641, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x4800, 0x64);
    ret += os08a20_write_register(vi_pipe, 0x4809, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4813, 0x90);
    ret += os08a20_write_register(vi_pipe, 0x4817, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x4833, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x4837, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x483b, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x484b, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4850, 0x7c);
    ret += os08a20_write_register(vi_pipe, 0x4852, 0x06);
    ret += os08a20_write_register(vi_pipe, 0x4856, 0x58);
    ret += os08a20_write_register(vi_pipe, 0x4857, 0xaa);
    ret += os08a20_write_register(vi_pipe, 0x4862, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x4869, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x486a, 0xaa);
    ret += os08a20_write_register(vi_pipe, 0x486e, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x486f, 0x55);
    ret += os08a20_write_register(vi_pipe, 0x4875, 0xf0);
    ret += os08a20_write_register(vi_pipe, 0x5000, 0x89);
    ret += os08a20_write_register(vi_pipe, 0x5001, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x5004, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x5005, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x5180, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x5181, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x580b, 0x03);

    return ret;
}

static td_s32 os08a20_linear_8m30_12bit_init_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x4d00, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4d01, 0xc9);
    ret += os08a20_write_register(vi_pipe, 0x4d02, 0xbc);
    ret += os08a20_write_register(vi_pipe, 0x4d03, 0xc6);
    ret += os08a20_write_register(vi_pipe, 0x4d04, 0x4a);
    ret += os08a20_write_register(vi_pipe, 0x4d05, 0x25);
    ret += os08a20_write_register(vi_pipe, 0x4700, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4e00, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0x01);

    os08a20_default_reg_init(vi_pipe);

    delay_ms(5);   /* delay 5 ms */
    ret += os08a20_write_register(vi_pipe, 0x0100, 0x01);

    return ret;
}


static td_s32 os08a20_linear_8m30_12bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += os08a20_linear_8m30_12bit_init_part1(vi_pipe);
    ret += os08a20_linear_8m30_12bit_init_part2(vi_pipe);
    ret += os08a20_linear_8m30_12bit_init_part3(vi_pipe);
    ret += os08a20_linear_8m30_12bit_init_part4(vi_pipe);
    ret += os08a20_linear_8m30_12bit_init_part5(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os08a20 write register failed!\n");
        return ret;
    }

    printf("========================================================================\n");
    printf("vi_pipe:%d,== os08a20 24Mclk 8M30fps(MIPI) 12bit linear init success! ==\n", vi_pipe);
    printf("========================================================================\n");
    return TD_SUCCESS;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x0100, 0x00);

    ret += os08a20_write_register(vi_pipe, 0x0103, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x0303, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x0305, 0x5a);
    ret += os08a20_write_register(vi_pipe, 0x0306, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x0308, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x0309, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x032a, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x300f, 0x11);
    ret += os08a20_write_register(vi_pipe, 0x3010, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3011, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3012, 0x41);
    ret += os08a20_write_register(vi_pipe, 0x3016, 0xf0);
    ret += os08a20_write_register(vi_pipe, 0x301e, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x3031, 0xa9);
    ret += os08a20_write_register(vi_pipe, 0x3103, 0x92);
    ret += os08a20_write_register(vi_pipe, 0x3104, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3106, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x340c, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x340d, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x031e, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0xe5);
    ret += os08a20_write_register(vi_pipe, 0x3505, 0x83);
    ret += os08a20_write_register(vi_pipe, 0x3508, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3509, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x350a, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x350b, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x350c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x350d, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x350e, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x350f, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3600, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3603, 0x2c);
    ret += os08a20_write_register(vi_pipe, 0x3605, 0x50);
    ret += os08a20_write_register(vi_pipe, 0x3609, 0xb5);
    ret += os08a20_write_register(vi_pipe, 0x3610, 0x39);
    ret += os08a20_write_register(vi_pipe, 0x360c, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3628, 0xa4);
    ret += os08a20_write_register(vi_pipe, 0x362d, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3660, 0x42);
    ret += os08a20_write_register(vi_pipe, 0x3661, 0x07);
    ret += os08a20_write_register(vi_pipe, 0x3662, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3663, 0x28);
    ret += os08a20_write_register(vi_pipe, 0x3664, 0x0d);

    return ret;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x366a, 0x38);
    ret += os08a20_write_register(vi_pipe, 0x366b, 0xa0);
    ret += os08a20_write_register(vi_pipe, 0x366d, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x366e, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3680, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x36c0, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3701, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x373b, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x373c, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3736, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3737, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3705, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3706, 0x39);
    ret += os08a20_write_register(vi_pipe, 0x370a, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x370b, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x3709, 0x49);
    ret += os08a20_write_register(vi_pipe, 0x3714, 0x21);
    ret += os08a20_write_register(vi_pipe, 0x371c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x371d, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3740, 0x1b);
    ret += os08a20_write_register(vi_pipe, 0x3741, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x375e, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x3760, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3776, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3781, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3782, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3783, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3784, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3785, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3788, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3789, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3797, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3762, 0x11);
    ret += os08a20_write_register(vi_pipe, 0x3800, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3801, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3802, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3803, 0x0c);
    ret += os08a20_write_register(vi_pipe, 0x3804, 0x0e);
    ret += os08a20_write_register(vi_pipe, 0x3805, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x3806, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3807, 0x73);
    ret += os08a20_write_register(vi_pipe, 0x3808, 0x0f);
    ret += os08a20_write_register(vi_pipe, 0x3809, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x380a, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x380b, 0x88);

    return ret;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x380c, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x380d, 0x0c);
    ret += os08a20_write_register(vi_pipe, 0x380e, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x380f, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x3813, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3814, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3815, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3816, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3817, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x381c, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3820, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3821, 0x24);
    ret += os08a20_write_register(vi_pipe, 0x3823, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3826, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3827, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x382d, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3832, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3833, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x383c, 0x48);
    ret += os08a20_write_register(vi_pipe, 0x383d, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x3d84, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += os08a20_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += os08a20_write_register(vi_pipe, 0x4000, 0xf8);
    ret += os08a20_write_register(vi_pipe, 0x4001, 0xeb);
    ret += os08a20_write_register(vi_pipe, 0x4004, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4005, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x400a, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x400f, 0xa0);
    ret += os08a20_write_register(vi_pipe, 0x4010, 0x12);
    ret += os08a20_write_register(vi_pipe, 0x4018, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4008, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x4009, 0x0d);
    ret += os08a20_write_register(vi_pipe, 0x401a, 0x58);
    ret += os08a20_write_register(vi_pipe, 0x4050, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4051, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4028, 0x2f);
    ret += os08a20_write_register(vi_pipe, 0x4052, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4053, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4054, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4055, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4056, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4057, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4058, 0x00);

    return ret;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x4059, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x430b, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x430c, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x430d, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x430e, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4501, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x4502, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4643, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4640, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4641, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x4800, 0x64);
    ret += os08a20_write_register(vi_pipe, 0x4809, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4813, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x4817, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x4833, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x4837, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x483b, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x484b, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4850, 0x7c);
    ret += os08a20_write_register(vi_pipe, 0x4852, 0x06);
    ret += os08a20_write_register(vi_pipe, 0x4856, 0x58);
    ret += os08a20_write_register(vi_pipe, 0x4857, 0xaa);
    ret += os08a20_write_register(vi_pipe, 0x4862, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x4869, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x486a, 0xaa);
    ret += os08a20_write_register(vi_pipe, 0x486e, 0x07);
    ret += os08a20_write_register(vi_pipe, 0x486f, 0x55);
    ret += os08a20_write_register(vi_pipe, 0x4875, 0xf0);
    ret += os08a20_write_register(vi_pipe, 0x5000, 0x89);
    ret += os08a20_write_register(vi_pipe, 0x5001, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x5004, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x5005, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x5180, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x5181, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x580b, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4d00, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4d01, 0xc9);
    ret += os08a20_write_register(vi_pipe, 0x4d02, 0xbc);
    ret += os08a20_write_register(vi_pipe, 0x4d03, 0xc6);
    ret += os08a20_write_register(vi_pipe, 0x4d04, 0x4a);
    ret += os08a20_write_register(vi_pipe, 0x4d05, 0x25);
    ret += os08a20_write_register(vi_pipe, 0x4700, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4e00, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0xe1);

    return ret;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x3511, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3512, 0x20);
    ret += os08a20_write_register(vi_pipe, 0x3833, 0x01);

    os08a20_default_reg_init(vi_pipe);

    delay_ms(1);
    ret += os08a20_write_register(vi_pipe, 0x0100, 0x01);

    return ret;
}

static td_s32 os08a20_vc_wdr_2t1_8m30_10bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += os08a20_vc_wdr_2t1_8m30_10bit_init_part1(vi_pipe);
    ret += os08a20_vc_wdr_2t1_8m30_10bit_init_part2(vi_pipe);
    ret += os08a20_vc_wdr_2t1_8m30_10bit_init_part3(vi_pipe);
    ret += os08a20_vc_wdr_2t1_8m30_10bit_init_part4(vi_pipe);
    ret += os08a20_vc_wdr_2t1_8m30_10bit_init_part5(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os08a20 write register failed!\n");
        return ret;
    }

    printf("========================================================================\n");
    printf("vi_pipe:%d,== os08a20 24Mclk 8M30fps(MIPI) 10bit vc-wdr init success! ==\n", vi_pipe);
    printf("========================================================================\n");
    return TD_SUCCESS;
}

/* 1920x1080 10-bit linear, 2x2 binning, 60 fps, 4-lane MIPI, 24 MHz MCLK.
 * GK7206V100 OS08A20 driver (OpenIPC opensdk): its 10-bit linear common init plus its
 * 1920x1080 binning block, with VTS (0x380e/0x380f) halved from 0x1214 (30 fps) to
 * 0x090a (60 fps) at the same HTS 0x040c and PLL. */
static td_s32 os08a20_linear_2m60_10bit_regs(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += os08a20_write_register(vi_pipe, 0x0103, 0x01);
    delay_ms(10); /* soft reset */
    ret += os08a20_write_register(vi_pipe, 0x0303, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x0305, 0x5a);
    ret += os08a20_write_register(vi_pipe, 0x0306, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x0308, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x0309, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x032a, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x300f, 0x11);
    ret += os08a20_write_register(vi_pipe, 0x3010, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3011, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3012, 0x41);
    ret += os08a20_write_register(vi_pipe, 0x3016, 0xf0);
    ret += os08a20_write_register(vi_pipe, 0x301e, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x3031, 0xa9);
    ret += os08a20_write_register(vi_pipe, 0x3103, 0x92);
    ret += os08a20_write_register(vi_pipe, 0x3104, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3106, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3400, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3025, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x3425, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3428, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3406, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3408, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x340c, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x340d, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x031e, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0xe5);
    ret += os08a20_write_register(vi_pipe, 0x3505, 0x83);
    ret += os08a20_write_register(vi_pipe, 0x3508, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3509, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x350a, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x350b, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x350c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x350d, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x350e, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x350f, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3600, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3603, 0x2c);
    ret += os08a20_write_register(vi_pipe, 0x3605, 0x50);
    ret += os08a20_write_register(vi_pipe, 0x3609, 0xb5);
    ret += os08a20_write_register(vi_pipe, 0x3610, 0x39);
    ret += os08a20_write_register(vi_pipe, 0x360c, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3628, 0xa4);
    ret += os08a20_write_register(vi_pipe, 0x362d, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3660, 0x43);
    ret += os08a20_write_register(vi_pipe, 0x3661, 0x06);
    ret += os08a20_write_register(vi_pipe, 0x3662, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3663, 0x28);
    ret += os08a20_write_register(vi_pipe, 0x3664, 0x0d);
    ret += os08a20_write_register(vi_pipe, 0x366a, 0x38);
    ret += os08a20_write_register(vi_pipe, 0x366b, 0xa0);
    ret += os08a20_write_register(vi_pipe, 0x366d, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x366e, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3680, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x36c0, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3701, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x373b, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x373c, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3736, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3737, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3705, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3706, 0x39);
    ret += os08a20_write_register(vi_pipe, 0x370a, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x370b, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x3709, 0x49);
    ret += os08a20_write_register(vi_pipe, 0x3714, 0x21);
    ret += os08a20_write_register(vi_pipe, 0x371c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x371d, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3740, 0x1b);
    ret += os08a20_write_register(vi_pipe, 0x3741, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x375e, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x3760, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3776, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3781, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3782, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3783, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3784, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3785, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3788, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3789, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3797, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3762, 0x11);
    ret += os08a20_write_register(vi_pipe, 0x380c, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x380d, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x380e, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x380f, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x3813, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x3814, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3815, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3816, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3817, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x381c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3820, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3821, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3823, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3826, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3827, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x382d, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3832, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x3833, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x383c, 0x48);
    ret += os08a20_write_register(vi_pipe, 0x383d, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x3d85, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x3d84, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x3d8c, 0x63);
    ret += os08a20_write_register(vi_pipe, 0x3d8d, 0xd7);
    ret += os08a20_write_register(vi_pipe, 0x4000, 0xf8);
    ret += os08a20_write_register(vi_pipe, 0x4001, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4004, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4005, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x400a, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x400f, 0xa0);
    ret += os08a20_write_register(vi_pipe, 0x4010, 0x12);
    ret += os08a20_write_register(vi_pipe, 0x4018, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4008, 0x02);
    ret += os08a20_write_register(vi_pipe, 0x4009, 0x0d);
    ret += os08a20_write_register(vi_pipe, 0x401a, 0x58);
    ret += os08a20_write_register(vi_pipe, 0x4050, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4051, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4028, 0x2f);
    ret += os08a20_write_register(vi_pipe, 0x4052, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4053, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4054, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4055, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4056, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4057, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x4058, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4059, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x430b, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x430c, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x430d, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x430e, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4501, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x4502, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4643, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x4640, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4641, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x4800, 0x64);
    ret += os08a20_write_register(vi_pipe, 0x4809, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4813, 0x90);
    ret += os08a20_write_register(vi_pipe, 0x4817, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x4833, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x4837, 0x0b);
    ret += os08a20_write_register(vi_pipe, 0x483b, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x484b, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4850, 0x7c);
    ret += os08a20_write_register(vi_pipe, 0x4852, 0x06);
    ret += os08a20_write_register(vi_pipe, 0x4856, 0x58);
    ret += os08a20_write_register(vi_pipe, 0x4857, 0xaa);
    ret += os08a20_write_register(vi_pipe, 0x4862, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x4869, 0x18);
    ret += os08a20_write_register(vi_pipe, 0x486a, 0xaa);
    ret += os08a20_write_register(vi_pipe, 0x486e, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x486f, 0x55);
    ret += os08a20_write_register(vi_pipe, 0x4875, 0xf0);
    ret += os08a20_write_register(vi_pipe, 0x5000, 0x89);
    ret += os08a20_write_register(vi_pipe, 0x5001, 0x42);
    ret += os08a20_write_register(vi_pipe, 0x5004, 0x40);
    ret += os08a20_write_register(vi_pipe, 0x5005, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x5180, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x5181, 0x10);
    ret += os08a20_write_register(vi_pipe, 0x580b, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4d00, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x4d01, 0xc9);
    ret += os08a20_write_register(vi_pipe, 0x4d02, 0xbc);
    ret += os08a20_write_register(vi_pipe, 0x4d03, 0xc6);
    ret += os08a20_write_register(vi_pipe, 0x4d04, 0x4a);
    ret += os08a20_write_register(vi_pipe, 0x4d05, 0x25);
    ret += os08a20_write_register(vi_pipe, 0x4700, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x4e00, 0x2b);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x4028, 0x4f);
    ret += os08a20_write_register(vi_pipe, 0x4029, 0x1f);
    ret += os08a20_write_register(vi_pipe, 0x402a, 0x7f);
    ret += os08a20_write_register(vi_pipe, 0x402b, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x0305, 0x32);
    ret += os08a20_write_register(vi_pipe, 0x3501, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x3502, 0x62);
    ret += os08a20_write_register(vi_pipe, 0x3600, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x3714, 0x22);
    ret += os08a20_write_register(vi_pipe, 0x3800, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3801, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3802, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3803, 0x0c);
    ret += os08a20_write_register(vi_pipe, 0x3804, 0x0e);
    ret += os08a20_write_register(vi_pipe, 0x3805, 0xff);
    ret += os08a20_write_register(vi_pipe, 0x3806, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3807, 0x6f);
    ret += os08a20_write_register(vi_pipe, 0x3808, 0x07);
    ret += os08a20_write_register(vi_pipe, 0x3809, 0x80);
    ret += os08a20_write_register(vi_pipe, 0x380a, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x380b, 0x38);
    ret += os08a20_write_register(vi_pipe, 0x380c, 0x04);
    ret += os08a20_write_register(vi_pipe, 0x380d, 0x0c);
    ret += os08a20_write_register(vi_pipe, 0x380e, 0x09);
    ret += os08a20_write_register(vi_pipe, 0x380f, 0x0a);
    ret += os08a20_write_register(vi_pipe, 0x3813, 0x08);
    ret += os08a20_write_register(vi_pipe, 0x3814, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x3815, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3816, 0x03);
    ret += os08a20_write_register(vi_pipe, 0x3817, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x381c, 0x00);
    ret += os08a20_write_register(vi_pipe, 0x3820, 0x01);
    ret += os08a20_write_register(vi_pipe, 0x3821, 0x05);
    ret += os08a20_write_register(vi_pipe, 0x4009, 0x05);
    ret += os08a20_write_register(vi_pipe, 0x4501, 0x98);
    ret += os08a20_write_register(vi_pipe, 0x4837, 0x14);
    return ret;
}

static td_s32 os08a20_linear_2m60_10bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = os08a20_linear_2m60_10bit_regs(vi_pipe);

    ret += os08a20_default_reg_init(vi_pipe);
    delay_ms(5); /* 5 ms before streaming */
    ret += os08a20_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os08a20 write register failed!\n");
        return ret;
    }
    printf("=== OS08A20 1080p 10bit 2x2 binning linear init OK (up to 120 fps) ===\n");
    return TD_SUCCESS;
}

static td_s32 os08a20_write_u16(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 value)
{
    td_s32 ret = os08a20_write_register(vi_pipe, addr, (value >> 8) & 0xff); /* high byte first */
    ret += os08a20_write_register(vi_pipe, addr + 1, value & 0xff);
    return ret;
}

/*
 * Flexible crop: the base mode's list, then, in standby, a window centred on
 * the array, its line (HTS) and the shortest frame its rows allow (VTS).
 *
 * The array is 3872 x 2192; the picture is its centre 3840 x 2160, from row
 * 12. The window moves in by a multiple of 8 columns and 4 rows so the Bayer
 * phase does not change, and the output is the window's size exactly.
 *
 * The sensor's output starts some rows into the window, at the ISP Y offset
 * (0x3812). In the native list that offset is 0 and the first ~18 rows out
 * are black: measured, offset 16 still left 4 dark rows and 24 none. So the
 * native window starts 24 rows above the picture's first row, with offset
 * 24, and runs 8 rows past its last. The binned list's offset of 8 (binned
 * rows) shows no dark rows, and its window is padded by 16 array rows.
 */
#define OS08A20_ARRAY_ROWS    2192
#define OS08A20_PIC_Y0        12
#define OS08A20_NATIVE_YOFF   24

static td_s32 os08a20_flex_window(ot_vi_pipe vi_pipe, const os08a20_flex *flex, td_bool bin)
{
    td_u32 scale = bin ? 2 : 1;
    td_u32 rows = flex->height * scale;
    td_u32 dx = ((3840 - flex->width * scale) / 2) & ~7u;
    td_u32 dy = ((2160 - rows) / 2) & ~3u;
    td_u32 y_start, y_end;
    td_s32 ret = TD_SUCCESS;

    if (bin) {
        y_start = OS08A20_PIC_Y0 + dy;
        y_end = y_start + rows + 16 - 1;    /* 16: pad */
    } else {
        y_start = (OS08A20_PIC_Y0 + dy >= OS08A20_NATIVE_YOFF) ? OS08A20_PIC_Y0 + dy - OS08A20_NATIVE_YOFF : 0;
        y_end = y_start + OS08A20_NATIVE_YOFF + rows + 8 - 1; /* 8: pad */
        ret += os08a20_write_u16(vi_pipe, 0x3812, OS08A20_NATIVE_YOFF);
    }
    if (y_end > OS08A20_ARRAY_ROWS - 1) {
        y_end = OS08A20_ARRAY_ROWS - 1;
    }

    ret += os08a20_write_u16(vi_pipe, 0x3800, dx);                 /* x start */
    ret += os08a20_write_u16(vi_pipe, 0x3802, y_start);
    ret += os08a20_write_u16(vi_pipe, 0x3804, 3839 - dx);          /* x end */
    ret += os08a20_write_u16(vi_pipe, 0x3806, y_end);
    ret += os08a20_write_u16(vi_pipe, 0x3808, flex->width);        /* output */
    ret += os08a20_write_u16(vi_pipe, 0x380a, flex->height);
    ret += os08a20_write_u16(vi_pipe, 0x380c, flex->hts);
    ret += os08a20_write_u16(vi_pipe, 0x380e, flex->vts);
    ret += os08a20_write_u16(vi_pipe, 0x3501, flex->vts / 2);      /* exposure, until AE sets it */
    return ret;
}

static td_s32 os08a20_flex_init(ot_vi_pipe vi_pipe, td_bool bin)
{
    const os08a20_flex *flex = os08a20_get_flex(vi_pipe);
    td_s32 ret;

    if (bin) {
        ret = os08a20_linear_2m60_10bit_regs(vi_pipe);
    } else {
        ret = os08a20_linear_8m30_12bit_init(vi_pipe);
        ret += os08a20_write_register(vi_pipe, 0x0100, 0x00);   /* standby */
        delay_ms(5); /* 5 ms: let the frame in progress end */
    }
    ret += os08a20_flex_window(vi_pipe, flex, bin);
    ret += os08a20_default_reg_init(vi_pipe);
    delay_ms(5); /* 5 ms before streaming */
    ret += os08a20_write_register(vi_pipe, 0x0100, 0x01);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os08a20 write register failed!\n");
        return ret;
    }
    printf("=== OS08A20 flexible crop %ux%u (%s) init OK ===\n", flex->width, flex->height,
        bin ? "2x2 binned, 10bit" : "native, 12bit");
    return TD_SUCCESS;
}
