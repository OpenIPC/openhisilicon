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
#include "securec.h"

#include "imx347_slave_cmos.h"
#include "ot_mpi_isp.h"
#include "ot_sns_ctrl.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};

int imx347_slave_i2c_init(ot_vi_pipe vi_pipe)
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
    ot_isp_sns_commbus *imx347slavebusinfo = TD_NULL;
    imx347slavebusinfo = imx347_slave_get_bus_info(vi_pipe);
    dev_num = imx347slavebusinfo->i2c_dev;
    (td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (IMX347_SLAVE_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int imx347_slave_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 imx347_slave_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 imx347_slave_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = IMX347_SLAVE_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX347_SLAVE_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX347_SLAVE_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (IMX347_SLAVE_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (IMX347_SLAVE_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, IMX347_SLAVE_ADDR_BYTE + IMX347_SLAVE_DATA_BYTE);
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

void imx347_slave_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
    return;
}

void imx347_slave_standby(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx347_slave_write_register(vi_pipe, 0x3000, 0x01);  /* STANDBY */
    ret += imx347_slave_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx347_slave_restart(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx347_slave_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(18); /* 18ms */
    ret += imx347_slave_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx347_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip)
{
    switch (sns_mirror_flip) {
        case ISP_SNS_NORMAL:
            imx347_slave_write_register(vi_pipe, 0x304E, 0x0);
            break;
        case ISP_SNS_MIRROR:
            imx347_slave_write_register(vi_pipe, 0x304E, 0x1);
            break;
        default:
            break;
    }
    return;
}

void imx347_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp sns_blc_clamp)
{
    td_s32 ret = TD_SUCCESS;

    imx347_slave_set_blc_clamp_value(vi_pipe, sns_blc_clamp.blc_clamp_en);

    if (sns_blc_clamp.blc_clamp_en == TD_TRUE) {
        ret += imx347_slave_write_register(vi_pipe, 0x3280, 0x01);  /* clamp on */
    } else {
        ret += imx347_slave_write_register(vi_pipe, 0x3280, 0x00);  /* clamp off */
        ret += imx347_slave_write_register(vi_pipe, 0x3302, 0x36);
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

static void imx347_slave_linear_4m30_init(ot_vi_pipe vi_pipe);

static void imx347_slave_default_reg_init(ot_vi_pipe vi_pipe)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *pastimx347slave = TD_NULL;
    pastimx347slave = imx347_slave_get_ctx(vi_pipe);
    for (i = 0; i < pastimx347slave->regs_info[0].reg_num; i++) {
        ret += imx347_slave_write_register(vi_pipe, pastimx347slave->regs_info[0].i2c_data[i].reg_addr,
            pastimx347slave->regs_info[0].i2c_data[i].data);
    }
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx347_set_slave_registers(ot_vi_pipe vi_pipe)
{
    td_s32  slave_dev;
    td_u32  data;
    td_s32  ot_ret;
    td_u8   img_mode;
    ot_isp_sns_state *pastimx347slave = TD_NULL;
    imx347_slave_video_mode_tbl *imx347_slave_mode_tbl = TD_NULL;
    ot_isp_slave_sns_sync *imx347_slave_sync = imx347_get_slave_sync(vi_pipe);
    td_s32 imx347_slave_bind_dev = imx347_get_slave_bind_dev(vi_pipe);
    td_u32 imx347_slave_sensor_mode_time = imx347_get_slave_sensor_mode_time(vi_pipe);
    ot_isp_sns_state *imx347_slave_sns_state = imx347_slave_get_ctx(vi_pipe);

    pastimx347slave = imx347_slave_get_ctx(vi_pipe);
    img_mode   = pastimx347slave->img_mode;
    slave_dev  = imx347_slave_bind_dev;
    data       = imx347_slave_sensor_mode_time;

    imx347_slave_mode_tbl = imx347_get_slave_mode_tbl(img_mode);

    check_ret(ot_mpi_isp_get_sns_slave_attr(slave_dev, imx347_slave_sync));
    imx347_slave_sync->cfg.bits.bit_h_enable = 0;
    imx347_slave_sync->cfg.bits.bit_v_enable = 0;
    imx347_slave_sync->slave_mode_time = data;
    check_ret(ot_mpi_isp_set_sns_slave_attr(slave_dev, imx347_slave_sync));

    ot_ret = imx347_slave_i2c_init(vi_pipe);
    if (ot_ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }
    check_ret(ot_mpi_isp_get_sns_slave_attr(slave_dev, imx347_slave_sync));
    imx347_slave_sync->hs_time = imx347_slave_mode_tbl->inck_per_hs;

    if (imx347_slave_sns_state->regs_info[0].slv_sync.slave_vs_time == 0) {
        imx347_slave_sync->vs_time = imx347_slave_mode_tbl->inck_per_vs;
    } else {
        imx347_slave_sync->vs_time = imx347_slave_sns_state->regs_info[0].slv_sync.slave_vs_time;
    }
    imx347_slave_sync->cfg.bytes = 0xc0030000;
    imx347_slave_sync->hs_cyc = 0x3;
    imx347_slave_sync->vs_cyc = 0x3;
    check_ret(ot_mpi_isp_set_sns_slave_attr(slave_dev, imx347_slave_sync));

    return;
}

void imx347_slave_init(ot_vi_pipe vi_pipe)
{
    ot_wdr_mode     wdr_mode;
    td_bool         init;
    td_u8           img_mode;
    ot_isp_sns_state *pastimx347slave = TD_NULL;

    pastimx347slave = imx347_slave_get_ctx(vi_pipe);
    init       = pastimx347slave->init;
    wdr_mode   = pastimx347slave->wdr_mode;
    img_mode   = pastimx347slave->img_mode;

    imx347_set_slave_registers(vi_pipe);
    /* When sensor first init, config all registers */
    if (init == TD_FALSE) {
        if (OT_WDR_MODE_NONE == wdr_mode) {
            if (IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE == img_mode) {  /* IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE */
                imx347_slave_linear_4m30_init(vi_pipe);
            }
        }
    } else {
        /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        if (OT_WDR_MODE_NONE == wdr_mode) {
            if (IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE == img_mode) {  /* IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE */
                imx347_slave_linear_4m30_init(vi_pipe);
            }
        }
    }
    pastimx347slave->init = TD_TRUE;
    return;
}

void imx347_slave_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    ret = imx347_slave_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("IMX347 exit failed!\n");
    }
    return;
}

static td_s32 imx347_slave_linear_4m30_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx347_slave_write_register(vi_pipe, 0x3000, 0x01);  /* STANDBY */
    ret += imx347_slave_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */

    ret += imx347_slave_write_register(vi_pipe, 0x300C, 0x5B);
    ret += imx347_slave_write_register(vi_pipe, 0x300D, 0x40);
    ret += imx347_slave_write_register(vi_pipe, 0x30BE, 0x5E);
    ret += imx347_slave_write_register(vi_pipe, 0x3110, 0x02);
    ret += imx347_slave_write_register(vi_pipe, 0x314C, 0xC0);
    ret += imx347_slave_write_register(vi_pipe, 0x315A, 0x06);
    ret += imx347_slave_write_register(vi_pipe, 0x316A, 0x7E);
    ret += imx347_slave_write_register(vi_pipe, 0x319E, 0x02);
    ret += imx347_slave_write_register(vi_pipe, 0x3202, 0x02);
    ret += imx347_slave_write_register(vi_pipe, 0x3288, 0x22);
    ret += imx347_slave_write_register(vi_pipe, 0x328A, 0x02);
    ret += imx347_slave_write_register(vi_pipe, 0x328C, 0xA2);
    ret += imx347_slave_write_register(vi_pipe, 0x328E, 0x22);
    ret += imx347_slave_write_register(vi_pipe, 0x3415, 0x27);
    ret += imx347_slave_write_register(vi_pipe, 0x3418, 0x27);
    ret += imx347_slave_write_register(vi_pipe, 0x3428, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x349E, 0x6A);
    ret += imx347_slave_write_register(vi_pipe, 0x34A2, 0x9A);
    ret += imx347_slave_write_register(vi_pipe, 0x34A4, 0x8A);
    ret += imx347_slave_write_register(vi_pipe, 0x34A6, 0x8E);
    ret += imx347_slave_write_register(vi_pipe, 0x34AA, 0xD8);
    ret += imx347_slave_write_register(vi_pipe, 0x3648, 0x01);
    ret += imx347_slave_write_register(vi_pipe, 0x3678, 0x01);
    ret += imx347_slave_write_register(vi_pipe, 0x367C, 0x69);
    ret += imx347_slave_write_register(vi_pipe, 0x367E, 0x69);
    ret += imx347_slave_write_register(vi_pipe, 0x3680, 0x69);
    ret += imx347_slave_write_register(vi_pipe, 0x3682, 0x69);
    ret += imx347_slave_write_register(vi_pipe, 0x371D, 0x05);
    ret += imx347_slave_write_register(vi_pipe, 0x375D, 0x11);
    ret += imx347_slave_write_register(vi_pipe, 0x375E, 0x43);
    ret += imx347_slave_write_register(vi_pipe, 0x375F, 0x76);
    ret += imx347_slave_write_register(vi_pipe, 0x3760, 0x07);
    ret += imx347_slave_write_register(vi_pipe, 0x3768, 0x1B);
    ret += imx347_slave_write_register(vi_pipe, 0x3769, 0x1B);
    ret += imx347_slave_write_register(vi_pipe, 0x376A, 0x1A);
    ret += imx347_slave_write_register(vi_pipe, 0x376B, 0x19);
    ret += imx347_slave_write_register(vi_pipe, 0x376C, 0x17);
    return ret;
}

static td_s32 imx347_slave_linear_4m30_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx347_slave_write_register(vi_pipe, 0x376D, 0x0F);
    ret += imx347_slave_write_register(vi_pipe, 0x376E, 0x0B);
    ret += imx347_slave_write_register(vi_pipe, 0x376F, 0x0B);
    ret += imx347_slave_write_register(vi_pipe, 0x3770, 0x0B);
    ret += imx347_slave_write_register(vi_pipe, 0x3776, 0x89);
    ret += imx347_slave_write_register(vi_pipe, 0x3777, 0x00);
    ret += imx347_slave_write_register(vi_pipe, 0x3778, 0xCA);
    ret += imx347_slave_write_register(vi_pipe, 0x3779, 0x00);
    ret += imx347_slave_write_register(vi_pipe, 0x377A, 0x45);
    ret += imx347_slave_write_register(vi_pipe, 0x377B, 0x01);
    ret += imx347_slave_write_register(vi_pipe, 0x377C, 0x56);
    ret += imx347_slave_write_register(vi_pipe, 0x377D, 0x02);
    ret += imx347_slave_write_register(vi_pipe, 0x377E, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x377F, 0x03);
    ret += imx347_slave_write_register(vi_pipe, 0x3780, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x3781, 0x05);
    ret += imx347_slave_write_register(vi_pipe, 0x3782, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x3783, 0x06);
    ret += imx347_slave_write_register(vi_pipe, 0x3784, 0x7F);
    ret += imx347_slave_write_register(vi_pipe, 0x3788, 0x1F);
    ret += imx347_slave_write_register(vi_pipe, 0x378A, 0xCA);
    ret += imx347_slave_write_register(vi_pipe, 0x378B, 0x00);
    ret += imx347_slave_write_register(vi_pipe, 0x378C, 0x45);
    ret += imx347_slave_write_register(vi_pipe, 0x378D, 0x01);
    ret += imx347_slave_write_register(vi_pipe, 0x378E, 0x56);
    ret += imx347_slave_write_register(vi_pipe, 0x378F, 0x02);
    ret += imx347_slave_write_register(vi_pipe, 0x3790, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x3791, 0x03);
    ret += imx347_slave_write_register(vi_pipe, 0x3792, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x3793, 0x05);
    ret += imx347_slave_write_register(vi_pipe, 0x3794, 0xFE);
    ret += imx347_slave_write_register(vi_pipe, 0x3795, 0x06);
    ret += imx347_slave_write_register(vi_pipe, 0x3796, 0x7F);
    ret += imx347_slave_write_register(vi_pipe, 0x3798, 0xBF);
    ret += imx347_slave_write_register(vi_pipe, 0x3A18, 0x7F);
    ret += imx347_slave_write_register(vi_pipe, 0x3A1A, 0x37);
    ret += imx347_slave_write_register(vi_pipe, 0x3A1C, 0x37);
    ret += imx347_slave_write_register(vi_pipe, 0x3A1E, 0xF7);
    ret += imx347_slave_write_register(vi_pipe, 0x3A1F, 0x00);
    ret += imx347_slave_write_register(vi_pipe, 0x3A20, 0x3F);
    ret += imx347_slave_write_register(vi_pipe, 0x3A22, 0x6F);
    ret += imx347_slave_write_register(vi_pipe, 0x3A24, 0x3F);
    ret += imx347_slave_write_register(vi_pipe, 0x3A26, 0x5F);
    ret += imx347_slave_write_register(vi_pipe, 0x3A28, 0x2F);
    return ret;
}

/* 4M 30fps */
static void imx347_slave_linear_4m30_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx347_slave_linear_4m30_part1(vi_pipe);
    ret += imx347_slave_linear_4m30_part2(vi_pipe);

    imx347_slave_default_reg_init(vi_pipe);
    if (vi_pipe == 1) {
        ret += imx347_slave_write_register(vi_pipe, 0x3302, 0x4B);
    }

    ret += imx347_slave_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(18); /* 18ms */
    ret += imx347_slave_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
        return;
    }
    printf("===IMX347 Slave 4M 30fps 12bit LINE Init OK!===\n");
    return;
}

