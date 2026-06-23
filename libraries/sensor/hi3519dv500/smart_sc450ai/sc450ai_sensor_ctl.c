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

#include "sc450ai_cmos.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};

int sc450ai_i2c_init(ot_vi_pipe vi_pipe)
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
    ot_isp_sns_commbus *sc450aibusinfo = TD_NULL;
    sc450aibusinfo = sc450ai_get_bus_info(vi_pipe);
    dev_num = sc450aibusinfo->i2c_dev;
    (td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (SC450AI_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int sc450ai_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 sc450ai_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 sc450ai_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = SC450AI_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = SC450AI_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = SC450AI_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (SC450AI_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (SC450AI_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, SC450AI_ADDR_BYTE + SC450AI_DATA_BYTE);
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

void sc450ai_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
    return;
}

void sc450ai_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

void sc450ai_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static td_s32 sc450ai_linear_4m30_10bit_init(ot_vi_pipe vi_pipe);
static td_s32 sc450ai_vc_wdr_2t1_4m30_10bit_init(ot_vi_pipe vi_pipe);

static void sc450ai_default_reg_init(ot_vi_pipe vi_pipe)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *pastsc450ai = TD_NULL;

    pastsc450ai = sc450ai_get_ctx(vi_pipe);
    for (i = 0; i < pastsc450ai->regs_info[0].reg_num; i++) {
        ret += sc450ai_write_register(vi_pipe,
            pastsc450ai->regs_info[0].i2c_data[i].reg_addr,
            pastsc450ai->regs_info[0].i2c_data[i].data);
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void sc450ai_init(ot_vi_pipe vi_pipe)
{
    ot_wdr_mode      wdr_mode;
    td_u8            img_mode;
    td_bool          init;
    td_s32           ret;
    ot_isp_sns_state *pastsc450ai = TD_NULL;

    pastsc450ai = sc450ai_get_ctx(vi_pipe);
    init        = pastsc450ai->init;
    wdr_mode    = pastsc450ai->wdr_mode;
    img_mode    = pastsc450ai->img_mode;

    ret = sc450ai_i2c_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    /* When sensor first init, config all registers */
    if (init == TD_FALSE) {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {
                sc450ai_vc_wdr_2t1_4m30_10bit_init(vi_pipe);
            } else {
            }
        } else {
            sc450ai_linear_4m30_10bit_init(vi_pipe);
        }
    } else {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
            if (SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {
                sc450ai_vc_wdr_2t1_4m30_10bit_init(vi_pipe);
            } else {
            }
        } else {
            sc450ai_linear_4m30_10bit_init(vi_pipe);
        }
    }

    pastsc450ai->init = TD_TRUE;
    return;
}

void sc450ai_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;

    ret = sc450ai_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("SC450AI exit failed!\n");
    }

    return;
}

static td_s32 sc450ai_linear_4m30_10bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x0103, 0x01);
    delay_ms(100); /* 100ms */
    ret += sc450ai_write_register(vi_pipe, 0x0100, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x36e9, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x36f9, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x301c, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x301f, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x302e, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3208, 0x0a);
    ret += sc450ai_write_register(vi_pipe, 0x3209, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x320a, 0x05);
    ret += sc450ai_write_register(vi_pipe, 0x320b, 0xf0);
    ret += sc450ai_write_register(vi_pipe, 0x320c, 0x02);
    ret += sc450ai_write_register(vi_pipe, 0x320d, 0xee);
    ret += sc450ai_write_register(vi_pipe, 0x320e, 0x06);
    ret += sc450ai_write_register(vi_pipe, 0x320f, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3214, 0x11);
    ret += sc450ai_write_register(vi_pipe, 0x3215, 0x11);
    ret += sc450ai_write_register(vi_pipe, 0x3220, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3223, 0xc0);
    ret += sc450ai_write_register(vi_pipe, 0x3253, 0x10);
    ret += sc450ai_write_register(vi_pipe, 0x325f, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x3274, 0x09);
    ret += sc450ai_write_register(vi_pipe, 0x3280, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x3301, 0x07);
    ret += sc450ai_write_register(vi_pipe, 0x3306, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x3308, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x330b, 0x58);
    ret += sc450ai_write_register(vi_pipe, 0x330e, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3315, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x335d, 0x60);
    ret += sc450ai_write_register(vi_pipe, 0x3364, 0x56);
    ret += sc450ai_write_register(vi_pipe, 0x338f, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x3390, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3391, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3392, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x3393, 0x07);
    ret += sc450ai_write_register(vi_pipe, 0x3394, 0x10);
    ret += sc450ai_write_register(vi_pipe, 0x3395, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3396, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3397, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3398, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x3399, 0x10);
    ret += sc450ai_write_register(vi_pipe, 0x339a, 0x13);
    return ret;
}

static td_s32 sc450ai_linear_4m30_10bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x339b, 0x15);
    ret += sc450ai_write_register(vi_pipe, 0x339c, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x33af, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x360f, 0x13);
    ret += sc450ai_write_register(vi_pipe, 0x3621, 0xec);
    ret += sc450ai_write_register(vi_pipe, 0x3622, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3625, 0x0b);
    ret += sc450ai_write_register(vi_pipe, 0x3627, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x3630, 0x90);
    ret += sc450ai_write_register(vi_pipe, 0x3633, 0x56);
    ret += sc450ai_write_register(vi_pipe, 0x3637, 0x1d);
    ret += sc450ai_write_register(vi_pipe, 0x3638, 0x12);
    ret += sc450ai_write_register(vi_pipe, 0x363c, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x363d, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x363e, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3670, 0x4a);
    ret += sc450ai_write_register(vi_pipe, 0x3671, 0xe0);
    ret += sc450ai_write_register(vi_pipe, 0x3672, 0xe0);
    ret += sc450ai_write_register(vi_pipe, 0x3673, 0xe1);
    ret += sc450ai_write_register(vi_pipe, 0x3674, 0xc0);
    ret += sc450ai_write_register(vi_pipe, 0x3675, 0x87);
    ret += sc450ai_write_register(vi_pipe, 0x3676, 0x8c);
    ret += sc450ai_write_register(vi_pipe, 0x367a, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x367b, 0x58);
    ret += sc450ai_write_register(vi_pipe, 0x367c, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x367d, 0x58);
    ret += sc450ai_write_register(vi_pipe, 0x3690, 0x22);
    ret += sc450ai_write_register(vi_pipe, 0x3691, 0x33);
    ret += sc450ai_write_register(vi_pipe, 0x3692, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x3699, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x369a, 0x07);
    ret += sc450ai_write_register(vi_pipe, 0x369b, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x369c, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x369d, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x36a2, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x36a3, 0x58);
    ret += sc450ai_write_register(vi_pipe, 0x36b0, 0x53);
    ret += sc450ai_write_register(vi_pipe, 0x36b1, 0x73);
    ret += sc450ai_write_register(vi_pipe, 0x36b2, 0x34);
    ret += sc450ai_write_register(vi_pipe, 0x36b3, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x36b4, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x36b7, 0xa0);
    ret += sc450ai_write_register(vi_pipe, 0x36b8, 0xa0);
    ret += sc450ai_write_register(vi_pipe, 0x36b9, 0x20);
    return ret;
}

static td_s32 sc450ai_linear_4m30_10bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x36bd, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x36be, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x36d0, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x36e0, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x36e1, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x36e2, 0x12);
    ret += sc450ai_write_register(vi_pipe, 0x36e3, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x36e4, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x36ec, 0x53);
    ret += sc450ai_write_register(vi_pipe, 0x36fc, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3907, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3908, 0x41);
    ret += sc450ai_write_register(vi_pipe, 0x391e, 0xf1);
    ret += sc450ai_write_register(vi_pipe, 0x391f, 0x11);
    ret += sc450ai_write_register(vi_pipe, 0x3933, 0x82);
    ret += sc450ai_write_register(vi_pipe, 0x3934, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x3935, 0x02);
    ret += sc450ai_write_register(vi_pipe, 0x3936, 0xc7);
    ret += sc450ai_write_register(vi_pipe, 0x3937, 0x76);
    ret += sc450ai_write_register(vi_pipe, 0x3938, 0x76);
    ret += sc450ai_write_register(vi_pipe, 0x3939, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x393a, 0x28);
    ret += sc450ai_write_register(vi_pipe, 0x393b, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x393c, 0x23);
    ret += sc450ai_write_register(vi_pipe, 0x3e01, 0xc2);
    ret += sc450ai_write_register(vi_pipe, 0x3e02, 0x60);
    ret += sc450ai_write_register(vi_pipe, 0x3e03, 0x0b);
    ret += sc450ai_write_register(vi_pipe, 0x3e08, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x3e1b, 0x2a);
    ret += sc450ai_write_register(vi_pipe, 0x440e, 0x02);
    ret += sc450ai_write_register(vi_pipe, 0x4509, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x4837, 0x2c);
    ret += sc450ai_write_register(vi_pipe, 0x5000, 0x0e);
    ret += sc450ai_write_register(vi_pipe, 0x5001, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x5799, 0x06);
    ret += sc450ai_write_register(vi_pipe, 0x59e0, 0xfe);
    ret += sc450ai_write_register(vi_pipe, 0x59e1, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x59e2, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59e3, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59e4, 0x30);
    return ret;
}

static td_s32 sc450ai_linear_4m30_10bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x5784, 0x8);
    ret += sc450ai_write_register(vi_pipe, 0x5785, 0x4);
    ret += sc450ai_write_register(vi_pipe, 0x5787, 0xa);
    ret += sc450ai_write_register(vi_pipe, 0x5788, 0xa);
    ret += sc450ai_write_register(vi_pipe, 0x5789, 0xa);
    ret += sc450ai_write_register(vi_pipe, 0x578a, 0xa);
    ret += sc450ai_write_register(vi_pipe, 0x578b, 0xa);
    ret += sc450ai_write_register(vi_pipe, 0x578c, 0xa);
    ret += sc450ai_write_register(vi_pipe, 0x578d, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x5790, 0x8);
    ret += sc450ai_write_register(vi_pipe, 0x5791, 0x4);
    ret += sc450ai_write_register(vi_pipe, 0x5792, 0x4);
    ret += sc450ai_write_register(vi_pipe, 0x5793, 0x8);
    ret += sc450ai_write_register(vi_pipe, 0x5794, 0x4);
    ret += sc450ai_write_register(vi_pipe, 0x5795, 0x4);
    ret += sc450ai_write_register(vi_pipe, 0x57aa, 0x28);
    ret += sc450ai_write_register(vi_pipe, 0x57ab, 0x0);
    ret += sc450ai_write_register(vi_pipe, 0x57ac, 0x0);
    ret += sc450ai_write_register(vi_pipe, 0x57ad, 0x0);

    ret += sc450ai_write_register(vi_pipe, 0x5000, 0x0); /* sensor dpc off */
    ret += sc450ai_write_register(vi_pipe, 0x5002, 0x0);
    ret += sc450ai_write_register(vi_pipe, 0x33b0, 0x0);
    return ret;
}

static td_s32 sc450ai_linear_4m30_10bit_init_part5(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x59e5, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59e6, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59e7, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59e8, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59e9, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59ea, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59eb, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59ec, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59ed, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59ee, 0xfe);
    ret += sc450ai_write_register(vi_pipe, 0x59ef, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x59f4, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59f5, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59f6, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59f7, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59f8, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59f9, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59fa, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59fb, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59fc, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59fd, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59fe, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59ff, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x36e9, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x36f9, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x3221, 0x66); /* mirror and flip */
    sc450ai_default_reg_init(vi_pipe);
    ret += sc450ai_write_register(vi_pipe, 0x0100, 0x01);
    return ret;
}

static td_s32 sc450ai_linear_4m30_10bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_linear_4m30_10bit_init_part1(vi_pipe);
    ret += sc450ai_linear_4m30_10bit_init_part2(vi_pipe);
    ret += sc450ai_linear_4m30_10bit_init_part3(vi_pipe);
    ret += sc450ai_linear_4m30_10bit_init_part4(vi_pipe); /* sensor dpc */
    ret += sc450ai_linear_4m30_10bit_init_part5(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sc450ai write register failed!\n");
        return ret;
    }

    printf("===================================================================================\n");
    printf("vi_pipe:%d,== SC450AI_MIPI_27MInput_4lane_360Mbps_10bit_30fps_2688x1520 Init OK! ==\n", vi_pipe);
    printf("===================================================================================\n");
    return ret;
}

static td_s32 sc450ai_vc_wdr_2t1_4m30_10bit_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x0103, 0x01);
    delay_ms(100); /* 100ms */
    ret += sc450ai_write_register(vi_pipe, 0x0100, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x36e9, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x36f9, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x301c, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x301f, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x302e, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3208, 0x0a);
    ret += sc450ai_write_register(vi_pipe, 0x3209, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x320a, 0x05);
    ret += sc450ai_write_register(vi_pipe, 0x320b, 0xf0);
    ret += sc450ai_write_register(vi_pipe, 0x320c, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x320d, 0xa8);
    ret += sc450ai_write_register(vi_pipe, 0x320e, 0x0c);
    ret += sc450ai_write_register(vi_pipe, 0x320f, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x3214, 0x11);
    ret += sc450ai_write_register(vi_pipe, 0x3215, 0x11);
    ret += sc450ai_write_register(vi_pipe, 0x3220, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3223, 0xc0);
    ret += sc450ai_write_register(vi_pipe, 0x3250, 0xff);
    ret += sc450ai_write_register(vi_pipe, 0x3253, 0x10);
    ret += sc450ai_write_register(vi_pipe, 0x325f, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x3274, 0x09);
    ret += sc450ai_write_register(vi_pipe, 0x3280, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x3281, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x3301, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3306, 0x24);
    ret += sc450ai_write_register(vi_pipe, 0x3309, 0x60);
    ret += sc450ai_write_register(vi_pipe, 0x330b, 0x64);
    ret += sc450ai_write_register(vi_pipe, 0x330d, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x3314, 0x94);
    ret += sc450ai_write_register(vi_pipe, 0x3315, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x331f, 0x59);
    ret += sc450ai_write_register(vi_pipe, 0x335d, 0x60);
    ret += sc450ai_write_register(vi_pipe, 0x3364, 0x56);
    ret += sc450ai_write_register(vi_pipe, 0x338f, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x3390, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3391, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3392, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x3393, 0x0a);
    ret += sc450ai_write_register(vi_pipe, 0x3394, 0x10);

    return ret;
}

static td_s32 sc450ai_vc_wdr_2t1_4m30_10bit_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x3395, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3396, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3397, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3398, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x3399, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x339a, 0x12);
    ret += sc450ai_write_register(vi_pipe, 0x339b, 0x14);
    ret += sc450ai_write_register(vi_pipe, 0x339c, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x33af, 0x18);
    ret += sc450ai_write_register(vi_pipe, 0x3400, 0x04);
    ret += sc450ai_write_register(vi_pipe, 0x3410, 0x04);
    ret += sc450ai_write_register(vi_pipe, 0x360f, 0x13);
    ret += sc450ai_write_register(vi_pipe, 0x3621, 0xec);
    ret += sc450ai_write_register(vi_pipe, 0x3627, 0xa0);
    ret += sc450ai_write_register(vi_pipe, 0x3630, 0x90);
    ret += sc450ai_write_register(vi_pipe, 0x3633, 0x56);
    ret += sc450ai_write_register(vi_pipe, 0x3637, 0x1d);
    ret += sc450ai_write_register(vi_pipe, 0x3638, 0x0a);
    ret += sc450ai_write_register(vi_pipe, 0x363c, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x363d, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x363e, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x3670, 0x4a);
    ret += sc450ai_write_register(vi_pipe, 0x3671, 0xe0);
    ret += sc450ai_write_register(vi_pipe, 0x3672, 0xe0);
    ret += sc450ai_write_register(vi_pipe, 0x3673, 0xe1);
    ret += sc450ai_write_register(vi_pipe, 0x3674, 0xb0);
    ret += sc450ai_write_register(vi_pipe, 0x3675, 0x88);
    ret += sc450ai_write_register(vi_pipe, 0x3676, 0x8c);
    ret += sc450ai_write_register(vi_pipe, 0x367a, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x367b, 0x58);
    ret += sc450ai_write_register(vi_pipe, 0x367c, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x367d, 0x58);
    ret += sc450ai_write_register(vi_pipe, 0x3690, 0x34);
    ret += sc450ai_write_register(vi_pipe, 0x3691, 0x43);
    ret += sc450ai_write_register(vi_pipe, 0x3692, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x3699, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x369a, 0x07);
    ret += sc450ai_write_register(vi_pipe, 0x369b, 0x0f);
    ret += sc450ai_write_register(vi_pipe, 0x369c, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x369d, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x36a2, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x36a3, 0x58);
    return ret;
}

static td_s32 sc450ai_vc_wdr_2t1_4m30_10bit_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x36b0, 0x54);
    ret += sc450ai_write_register(vi_pipe, 0x36b1, 0x74);
    ret += sc450ai_write_register(vi_pipe, 0x36b2, 0x35);
    ret += sc450ai_write_register(vi_pipe, 0x36b3, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x36b4, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x36b7, 0xa0);
    ret += sc450ai_write_register(vi_pipe, 0x36b8, 0xa0);
    ret += sc450ai_write_register(vi_pipe, 0x36b9, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x36bd, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x36be, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x36d0, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x36e0, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x36e1, 0x08);
    ret += sc450ai_write_register(vi_pipe, 0x36e2, 0x12);
    ret += sc450ai_write_register(vi_pipe, 0x36e3, 0x48);
    ret += sc450ai_write_register(vi_pipe, 0x36e4, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x36fa, 0x0d);
    ret += sc450ai_write_register(vi_pipe, 0x36fb, 0xa4);
    ret += sc450ai_write_register(vi_pipe, 0x36fc, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x36fd, 0x24);
    ret += sc450ai_write_register(vi_pipe, 0x3907, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3908, 0x41);
    ret += sc450ai_write_register(vi_pipe, 0x391e, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x391f, 0x11);
    ret += sc450ai_write_register(vi_pipe, 0x3e00, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x3e01, 0x6c);
    ret += sc450ai_write_register(vi_pipe, 0x3e02, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3e03, 0x0b);
    ret += sc450ai_write_register(vi_pipe, 0x3e04, 0x16);
    ret += sc450ai_write_register(vi_pipe, 0x3e05, 0xc0);
    ret += sc450ai_write_register(vi_pipe, 0x3e06, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3e07, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x3e08, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x3e09, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x3e10, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3e11, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x3e12, 0x03);
    ret += sc450ai_write_register(vi_pipe, 0x3e13, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x3e1b, 0x2a);
    ret += sc450ai_write_register(vi_pipe, 0x3e22, 0x00);
    ret += sc450ai_write_register(vi_pipe, 0x3e23, 0x01);
    ret += sc450ai_write_register(vi_pipe, 0x3e24, 0x78);
    ret += sc450ai_write_register(vi_pipe, 0x440e, 0x02);

    return ret;
}

static td_s32 sc450ai_vc_wdr_2t1_4m30_10bit_init_part4(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += sc450ai_write_register(vi_pipe, 0x4509, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x4837, 0x16);
    ret += sc450ai_write_register(vi_pipe, 0x4853, 0xf8);
    ret += sc450ai_write_register(vi_pipe, 0x5000, 0x0e);
    ret += sc450ai_write_register(vi_pipe, 0x5001, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x5011, 0x80);
    ret += sc450ai_write_register(vi_pipe, 0x5799, 0x06);
    ret += sc450ai_write_register(vi_pipe, 0x57a8, 0xd0);
    ret += sc450ai_write_register(vi_pipe, 0x59e0, 0xfe);
    ret += sc450ai_write_register(vi_pipe, 0x59e1, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x59e2, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59e3, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59e4, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59e5, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59e6, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59e7, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59e8, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59e9, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59ea, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59eb, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59ec, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59ed, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59ee, 0xfe);
    ret += sc450ai_write_register(vi_pipe, 0x59ef, 0x40);
    ret += sc450ai_write_register(vi_pipe, 0x59f4, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59f5, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59f6, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59f7, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59f8, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59f9, 0x30);
    ret += sc450ai_write_register(vi_pipe, 0x59fa, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59fb, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59fc, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x59fd, 0x3f);
    ret += sc450ai_write_register(vi_pipe, 0x59fe, 0x3c);
    ret += sc450ai_write_register(vi_pipe, 0x59ff, 0x38);
    ret += sc450ai_write_register(vi_pipe, 0x36e9, 0x44);
    ret += sc450ai_write_register(vi_pipe, 0x36f9, 0x20);
    ret += sc450ai_write_register(vi_pipe, 0x3221, 0x66); /* mirror and flip */
    sc450ai_default_reg_init(vi_pipe);
    ret += sc450ai_write_register(vi_pipe, 0x0100, 0x01);
    return ret;
}


static td_s32 sc450ai_vc_wdr_2t1_4m30_10bit_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += sc450ai_vc_wdr_2t1_4m30_10bit_init_part1(vi_pipe);
    ret += sc450ai_vc_wdr_2t1_4m30_10bit_init_part2(vi_pipe);
    ret += sc450ai_vc_wdr_2t1_4m30_10bit_init_part3(vi_pipe);
    ret += sc450ai_vc_wdr_2t1_4m30_10bit_init_part4(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sc450ai_cmos write register failed!\n");
        return ret;
    }
    printf("==========================================================================================\n");
    printf("vi_pipe:%d,== SC450AI_MIPI_27MInput_4lane_720Mbps_10bit_WDR2T1_30fps_2688x1520 Init OK! ==\n", vi_pipe);
    printf("==========================================================================================\n");
    return ret;
}

