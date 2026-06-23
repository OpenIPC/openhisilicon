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

#include "imx515_cmos.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};

int imx515_i2c_init(ot_vi_pipe vi_pipe)
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
    ot_isp_sns_commbus *imx515businfo = TD_NULL;
    imx515businfo = imx515_get_bus_info(vi_pipe);
    dev_num = imx515businfo->i2c_dev;
    (td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (IMX515_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int imx515_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 imx515_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 imx515_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = IMX515_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX515_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX515_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (IMX515_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (IMX515_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, IMX515_ADDR_BYTE + IMX515_DATA_BYTE);
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

void imx515_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
}

void imx515_standby(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx515_write_register(vi_pipe, 0x3000, 0x01);  /* STANDBY */
    ret += imx515_write_register(vi_pipe, 0x3002, 0x01);  /* XTMSTA */
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx515_restart(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx515_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(20); /* 20ms */
    ret += imx515_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx515_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip)
{
    switch (sns_mirror_flip) {
        case ISP_SNS_NORMAL:
            imx515_write_register(vi_pipe, 0x3030, 0x00);
            break;
        case ISP_SNS_MIRROR:
            imx515_write_register(vi_pipe, 0x3030, 0x01);
            break;
        case ISP_SNS_FLIP:
            imx515_write_register(vi_pipe, 0x3030, 0x02);
            break;
        case ISP_SNS_MIRROR_FLIP:
            imx515_write_register(vi_pipe, 0x3030, 0x03);
            break;
        default:
            break;
    }
    return;
}

td_void imx515_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp)
{
    td_s32 ret = TD_SUCCESS;

    imx515_set_blc_clamp_value(vi_pipe, blc_clamp.blc_clamp_en);

    if (blc_clamp.blc_clamp_en == TD_TRUE) {
        ret += imx515_write_register(vi_pipe, 0x300e, 0x01);
    } else {
        ret += imx515_write_register(vi_pipe, 0x300e, 0x00);
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx515_comm_init(ot_vi_pipe vi_pipe);
void imx515_linear_8m_init(ot_vi_pipe vi_pipe);

void imx515_default_reg_init(ot_vi_pipe vi_pipe)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *pastimx515 = TD_NULL;
    pastimx515 = imx515_get_ctx(vi_pipe);
    for (i = 0; i < pastimx515->regs_info[0].reg_num; i++) {
        ret += imx515_write_register(vi_pipe,
                                     pastimx515->regs_info[0].i2c_data[i].reg_addr,
                                     pastimx515->regs_info[0].i2c_data[i].data);
    }
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

void imx515_init(ot_vi_pipe vi_pipe)
{
    ot_wdr_mode wdr_mode;
    td_bool          init;
    td_s32 ret;
    ot_isp_sns_state *pastimx515 = TD_NULL;
    pastimx515 = imx515_get_ctx(vi_pipe);
    init       = pastimx515->init;
    wdr_mode   = pastimx515->wdr_mode;

    ret = imx515_i2c_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }
    /* When sensor first init, config all registers */
    if (init == TD_FALSE) {
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
        } else {
            imx515_linear_8m_init(vi_pipe);
        }
    } else {
        /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
        } else {
            imx515_linear_8m_init(vi_pipe);
        }
    }

    pastimx515->init = TD_TRUE;
    return;
}

void imx515_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    ret = imx515_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("IMX515 exit failed!\n");
    }
    return;
}

static td_s32 imx515_linear_2160p30_init_part1(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += imx515_write_register(vi_pipe, 0x3008, 0x7F);
    ret += imx515_write_register(vi_pipe, 0x300A, 0x5B);
    ret += imx515_write_register(vi_pipe, 0x3024, 0xE4);
    ret += imx515_write_register(vi_pipe, 0x3025, 0x0C);
    ret += imx515_write_register(vi_pipe, 0x3028, 0xEE);
    ret += imx515_write_register(vi_pipe, 0x3033, 0x08);
    ret += imx515_write_register(vi_pipe, 0x3050, 0x06);
    ret += imx515_write_register(vi_pipe, 0x3051, 0x09);
    ret += imx515_write_register(vi_pipe, 0x3090, 0x14);
    ret += imx515_write_register(vi_pipe, 0x30C1, 0x00);
    ret += imx515_write_register(vi_pipe, 0x3116, 0x24);
    ret += imx515_write_register(vi_pipe, 0x3118, 0xA0);
    ret += imx515_write_register(vi_pipe, 0x311E, 0x24);
    ret += imx515_write_register(vi_pipe, 0x32D4, 0x21);
    ret += imx515_write_register(vi_pipe, 0x32EC, 0xA1);
    ret += imx515_write_register(vi_pipe, 0x344C, 0x2B);
    ret += imx515_write_register(vi_pipe, 0x344D, 0x01);
    ret += imx515_write_register(vi_pipe, 0x344E, 0xED);
    ret += imx515_write_register(vi_pipe, 0x344F, 0x01);
    ret += imx515_write_register(vi_pipe, 0x3450, 0xF6);
    ret += imx515_write_register(vi_pipe, 0x3451, 0x02);
    ret += imx515_write_register(vi_pipe, 0x3452, 0x7F);
    ret += imx515_write_register(vi_pipe, 0x3453, 0x03);
    ret += imx515_write_register(vi_pipe, 0x358A, 0x04);
    ret += imx515_write_register(vi_pipe, 0x35A1, 0x02);
    ret += imx515_write_register(vi_pipe, 0x35EC, 0x27);
    ret += imx515_write_register(vi_pipe, 0x35EE, 0x8D);
    ret += imx515_write_register(vi_pipe, 0x35F0, 0x8D);
    ret += imx515_write_register(vi_pipe, 0x35F2, 0x29);
    ret += imx515_write_register(vi_pipe, 0x36BC, 0x0C);
    ret += imx515_write_register(vi_pipe, 0x36CC, 0x53);
    ret += imx515_write_register(vi_pipe, 0x36CD, 0x00);
    ret += imx515_write_register(vi_pipe, 0x36CE, 0x3C);
    ret += imx515_write_register(vi_pipe, 0x36D0, 0x8C);
    ret += imx515_write_register(vi_pipe, 0x36D1, 0x00);
    ret += imx515_write_register(vi_pipe, 0x36D2, 0x71);
    ret += imx515_write_register(vi_pipe, 0x36D4, 0x3C);
    ret += imx515_write_register(vi_pipe, 0x36D6, 0x53);
    ret += imx515_write_register(vi_pipe, 0x36D7, 0x00);
    ret += imx515_write_register(vi_pipe, 0x36D8, 0x71);
    ret += imx515_write_register(vi_pipe, 0x36DA, 0x8C);
    ret += imx515_write_register(vi_pipe, 0x36DB, 0x00);

    return ret;
}

static td_s32 imx515_linear_2160p30_init_part2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += imx515_write_register(vi_pipe, 0x3720, 0x00);
    ret += imx515_write_register(vi_pipe, 0x3724, 0x02);
    ret += imx515_write_register(vi_pipe, 0x3726, 0x02);
    ret += imx515_write_register(vi_pipe, 0x3732, 0x02);
    ret += imx515_write_register(vi_pipe, 0x3734, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3736, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3742, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3862, 0xE0);
    ret += imx515_write_register(vi_pipe, 0x38CC, 0x30);
    ret += imx515_write_register(vi_pipe, 0x38CD, 0x2F);
    ret += imx515_write_register(vi_pipe, 0x395C, 0x0C);
    ret += imx515_write_register(vi_pipe, 0x39A4, 0x07);
    ret += imx515_write_register(vi_pipe, 0x39A8, 0x32);
    ret += imx515_write_register(vi_pipe, 0x39AA, 0x32);
    ret += imx515_write_register(vi_pipe, 0x39AC, 0x32);
    ret += imx515_write_register(vi_pipe, 0x39AE, 0x32);
    ret += imx515_write_register(vi_pipe, 0x39B0, 0x32);
    ret += imx515_write_register(vi_pipe, 0x39B2, 0x2F);
    ret += imx515_write_register(vi_pipe, 0x39B4, 0x2D);
    ret += imx515_write_register(vi_pipe, 0x39B6, 0x28);
    ret += imx515_write_register(vi_pipe, 0x39B8, 0x30);
    ret += imx515_write_register(vi_pipe, 0x39BA, 0x30);
    ret += imx515_write_register(vi_pipe, 0x39BC, 0x30);
    ret += imx515_write_register(vi_pipe, 0x39BE, 0x30);
    ret += imx515_write_register(vi_pipe, 0x39C0, 0x30);
    ret += imx515_write_register(vi_pipe, 0x39C2, 0x2E);
    ret += imx515_write_register(vi_pipe, 0x39C4, 0x2B);
    ret += imx515_write_register(vi_pipe, 0x39C6, 0x25);
    ret += imx515_write_register(vi_pipe, 0x3A42, 0xD1);
    ret += imx515_write_register(vi_pipe, 0x3A4C, 0x77);
    ret += imx515_write_register(vi_pipe, 0x3AE0, 0x02);
    ret += imx515_write_register(vi_pipe, 0x3AEC, 0x0C);
    ret += imx515_write_register(vi_pipe, 0x3B00, 0x2E);
    ret += imx515_write_register(vi_pipe, 0x3B06, 0x29);
    ret += imx515_write_register(vi_pipe, 0x3B98, 0x25);
    ret += imx515_write_register(vi_pipe, 0x3B99, 0x21);
    ret += imx515_write_register(vi_pipe, 0x3B9B, 0x13);
    ret += imx515_write_register(vi_pipe, 0x3B9C, 0x13);
    ret += imx515_write_register(vi_pipe, 0x3B9D, 0x13);
    ret += imx515_write_register(vi_pipe, 0x3B9E, 0x13);

    return ret;
}

static td_s32 imx515_linear_2160p30_init_part3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += imx515_write_register(vi_pipe, 0x3BA1, 0x00);
    ret += imx515_write_register(vi_pipe, 0x3BA2, 0x06);
    ret += imx515_write_register(vi_pipe, 0x3BA3, 0x0B);
    ret += imx515_write_register(vi_pipe, 0x3BA4, 0x10);
    ret += imx515_write_register(vi_pipe, 0x3BA5, 0x14);
    ret += imx515_write_register(vi_pipe, 0x3BA6, 0x18);
    ret += imx515_write_register(vi_pipe, 0x3BA7, 0x1A);
    ret += imx515_write_register(vi_pipe, 0x3BA8, 0x1A);
    ret += imx515_write_register(vi_pipe, 0x3BA9, 0x1A);
    ret += imx515_write_register(vi_pipe, 0x3BAC, 0xED);
    ret += imx515_write_register(vi_pipe, 0x3BAD, 0x01);
    ret += imx515_write_register(vi_pipe, 0x3BAE, 0xF6);
    ret += imx515_write_register(vi_pipe, 0x3BAF, 0x02);
    ret += imx515_write_register(vi_pipe, 0x3BB0, 0xA2);
    ret += imx515_write_register(vi_pipe, 0x3BB1, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3BB2, 0xE0);
    ret += imx515_write_register(vi_pipe, 0x3BB3, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3BB4, 0xE0);
    ret += imx515_write_register(vi_pipe, 0x3BB5, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3BB6, 0xE0);
    ret += imx515_write_register(vi_pipe, 0x3BB7, 0x03);
    ret += imx515_write_register(vi_pipe, 0x3BB8, 0xE0);
    ret += imx515_write_register(vi_pipe, 0x3BBA, 0xE0);
    ret += imx515_write_register(vi_pipe, 0x3BBC, 0xDA);
    ret += imx515_write_register(vi_pipe, 0x3BBE, 0x88);
    ret += imx515_write_register(vi_pipe, 0x3BC0, 0x44);
    ret += imx515_write_register(vi_pipe, 0x3BC2, 0x7B);
    ret += imx515_write_register(vi_pipe, 0x3BC4, 0xA2);
    ret += imx515_write_register(vi_pipe, 0x3BC8, 0xBD);
    ret += imx515_write_register(vi_pipe, 0x3BCA, 0xBD);
    ret += imx515_write_register(vi_pipe, 0x4004, 0x48);
    ret += imx515_write_register(vi_pipe, 0x4005, 0x09);
    ret += imx515_write_register(vi_pipe, 0x4018, 0xA7);
    ret += imx515_write_register(vi_pipe, 0x401A, 0x57);
    ret += imx515_write_register(vi_pipe, 0x401C, 0x5F);
    ret += imx515_write_register(vi_pipe, 0x401E, 0x97);
    ret += imx515_write_register(vi_pipe, 0x4020, 0x5F);
    ret += imx515_write_register(vi_pipe, 0x4022, 0xAF);
    ret += imx515_write_register(vi_pipe, 0x4024, 0x5F);
    ret += imx515_write_register(vi_pipe, 0x4026, 0x9F);
    ret += imx515_write_register(vi_pipe, 0x4028, 0x4F);

    return ret;
}

void imx515_linear_8m_init(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;

    ret += imx515_linear_2160p30_init_part1(vi_pipe);
    ret += imx515_linear_2160p30_init_part2(vi_pipe);
    ret += imx515_linear_2160p30_init_part3(vi_pipe);

    imx515_default_reg_init(vi_pipe);

    ret += imx515_write_register(vi_pipe, 0x3000, 0x00);  /* standby */
    delay_ms(20); /* 20ms */
    ret += imx515_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    if (ret != TD_SUCCESS) {
        isp_err_trace("imx515 write register failed!\n");
        return;
    }
    printf("===IMX515 8M 30fps 12bit LINE Init OK!===\n");
    return;
}
