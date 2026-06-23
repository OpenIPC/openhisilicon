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

#include "gst412c_cmos.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
#define FILE_NAME_LEN 50
#define LEN 10
#define gst412c_check_fp_return(fp, file_name)                                   \
    do {                                                       \
        if (fp == NULL) {    \
            printf("Cannot open %s.\n", file_name); \
            return;                   \
        }                                                      \
    } while (0)

td_void gpio_export(td_u32 gpio_chip_num, td_u32 gpio_offset)
{
    td_s32 ret;
    FILE *fp = TD_NULL;
    td_char file_name[FILE_NAME_LEN];
    td_u32 gpio_num = gpio_chip_num * 8 + gpio_offset; // 8 pins

    ret = sprintf_s(file_name, FILE_NAME_LEN, "/sys/class/gpio/export");
    if (ret < 0) {
        printf("export sprintf_s failed with %d.\n", ret);
        return;
    }
    fp = fopen(file_name, "w");
    gst412c_check_fp_return(fp, file_name);
    ret = fprintf(fp, "%u", gpio_num);
    if (ret < 0) {
        printf("print gpio_num failed!!!\n");
    }

    ret = fclose(fp);
    if (ret != 0) {
        printf("export close fp err ret:%d\n", ret);
    }
    return;
}

td_void gpio_dir(td_u32 gpio_chip_num, td_u32 gpio_offset)
{
    td_s32 ret;
    FILE *fp = TD_NULL;
    char file_name[FILE_NAME_LEN];
    td_u32 gpio_num = gpio_chip_num * 8 + gpio_offset; // 8 pins

    ret = sprintf_s(file_name, FILE_NAME_LEN, "/sys/class/gpio/gpio%u/direction", gpio_num);
    if (ret < 0) {
        printf("dir sprintf_s failed with %d.\n", ret);
        return;
    }
    fp = fopen(file_name, "rb+");
    gst412c_check_fp_return(fp, file_name);
    ret = fprintf(fp, "out");
    if (ret < 0) {
        printf("print out failed!!!\n");
    }
    ret = fclose(fp);
    if (ret != 0) {
        printf("dir close fp err, ret:%d\n", ret);
    }

    return;
}

td_void gpio_write(td_u32 gpio_chip_num, td_u32 gpio_offset, td_u32 gpio_out_val)
{
    td_s32 ret;
    FILE *fp = TD_NULL;
    char buf[LEN];
    char file_name[FILE_NAME_LEN];
    td_u32 gpio_num = gpio_chip_num * 8 + gpio_offset; // 8 pins

    ret = sprintf_s(file_name, FILE_NAME_LEN, "/sys/class/gpio/gpio%u/value", gpio_num);
    if (ret < 0) {
        printf("write sprintf_s failed with %d.\n", ret);
        return;
    }

    fp = fopen(file_name, "rb+");
    gst412c_check_fp_return(fp, file_name);

    if (gpio_out_val) {
        strcpy_s(buf, LEN, "1");
    } else {
        strcpy_s(buf, LEN, "0");
    }
    ret = fwrite(buf, sizeof(char), sizeof(buf) - 1, fp);
    if (ret != (sizeof(buf) - 1)) {
        printf("%s: gpio%u_%u = %s, fwrite err ret:%d\n", __func__, gpio_chip_num, gpio_offset, buf, ret);
    }
    ret = fclose(fp);
    if (ret != 0) {
        printf("write close fp err, ret:%d\n", ret);
    }

    return;
}

td_void gpio_unexport(td_u32 gpio_chip_num, td_u32 gpio_offset)
{
    td_s32 ret;
    FILE *fp = TD_NULL;
    char file_name[FILE_NAME_LEN];
    td_u32 gpio_num = gpio_chip_num * 8 + gpio_offset; // 8 pins

    ret = sprintf_s(file_name, FILE_NAME_LEN, "/sys/class/gpio/unexport");
    if (ret < 0) {
        printf("unexport sprintf_s failed with %d.\n", ret);
        return;
    }

    fp = fopen(file_name, "w");
    gst412c_check_fp_return(fp, file_name);
    ret = fprintf(fp, "%u", gpio_num);
    if (ret < 0) {
        printf("print unexport gpio_num failed!!!\n");
    }
    ret = fclose(fp);
    if (ret != 0) {
        printf("unexport close fp err, ret:%d\n", ret);
    }

    return;
}

static int g_fd[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = -1};
int gst412c_i2c_init(ot_vi_pipe vi_pipe)
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
    ot_isp_sns_commbus *gst412cbusinfo = TD_NULL;
    gst412cbusinfo = gst412c_get_bus_info(vi_pipe);
    dev_num = gst412cbusinfo->i2c_dev;
    (td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_I2C_SLAVE_FORCE, (GST412C_I2C_ADDR >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int gst412c_i2c_exit(ot_vi_pipe vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

td_s32 gst412c_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 gst412c_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    td_s32 ret;
    if (g_fd[vi_pipe] < 0) {
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = GST412C_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = GST412C_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = GST412C_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (GST412C_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (GST412C_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, GST412C_ADDR_BYTE + GST412C_DATA_BYTE);
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

void gst412c_prog(ot_vi_pipe vi_pipe, const td_u32 *rom)
{
    ot_unused(vi_pipe);
    ot_unused(rom);
    return;
}

void gst412c_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

void gst412c_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static void gst412c_linear_4m30_12bit_init(ot_vi_pipe vi_pipe);

void gst412c_init(ot_vi_pipe vi_pipe)
{
    td_bool          init;
    td_s32           ret;
    ot_isp_sns_state *pastgst412c = TD_NULL;

    pastgst412c = gst412c_get_ctx(vi_pipe);
    init        = pastgst412c->init;

    ret = gst412c_i2c_init(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    /* When sensor first init, config all registers */
    if (init == TD_FALSE) {
        gst412c_linear_4m30_12bit_init(vi_pipe);
    } else {
        gst412c_linear_4m30_12bit_init(vi_pipe);
    }

    pastgst412c->init = TD_TRUE;
    return;
}

void gst412c_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;

    ret = gst412c_i2c_exit(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("GST412C exit failed!\n");
    }

    return;
}

static td_s32 gst412c_linear_4m30_12bit_init_1(ot_vi_pipe vi_pipe)
{
    td_u32 gpio_chip_num2 = 2; // gpio2
    gpio_export(gpio_chip_num2, 0x0);  // gpio2_0
    gpio_export(gpio_chip_num2, 0x2);  // gpio2_2
    gpio_dir(gpio_chip_num2, 0x0);  // gpio2_0 out
    gpio_dir(gpio_chip_num2, 0x2);  // gpio2_2 out
    gpio_write(gpio_chip_num2, 0x0, 1); // set GPIO2_0 1
    delay_ms(1000);                     // delay 1000ms
    gpio_write(gpio_chip_num2, 0x2, 1); // set GPIO2_2 1
    delay_ms(1000);                     // delay 1000ms

    td_s32 ret = TD_SUCCESS;

    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa9);
    ret += gst412c_write_register(vi_pipe,  0x0,  0x0);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa8);
    ret += gst412c_write_register(vi_pipe, 0x02, 0x90);
    ret += gst412c_write_register(vi_pipe, 0x03, 0x01);
    ret += gst412c_write_register(vi_pipe, 0x04, 0x34);
    ret += gst412c_write_register(vi_pipe, 0x05, 0x01);
    ret += gst412c_write_register(vi_pipe, 0x06, 0x48);
    ret += gst412c_write_register(vi_pipe, 0x07, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x08, 0x3d);
    ret += gst412c_write_register(vi_pipe, 0x09, 0x01);
    ret += gst412c_write_register(vi_pipe, 0x0a, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x0b, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x0c, 0x90);
    ret += gst412c_write_register(vi_pipe, 0x0d, 0x01);
    ret += gst412c_write_register(vi_pipe, 0x0e, 0x01);
    ret += gst412c_write_register(vi_pipe, 0x2F, 0x41);
    ret += gst412c_write_register(vi_pipe, 0x30, 0x44);
    ret += gst412c_write_register(vi_pipe, 0x31, 0x02);
    ret += gst412c_write_register(vi_pipe, 0x32, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x10, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x01, 0x01);
    return ret;
}

static td_s32 gst412c_linear_4m30_12bit_init_2(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    delay_ms(0x5);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xab);
    ret += gst412c_write_register(vi_pipe, 0x01, 0x03);
    ret += gst412c_write_register(vi_pipe, 0x05, 0x03);
    ret += gst412c_write_register(vi_pipe, 0x06, 0xff);
    ret += gst412c_write_register(vi_pipe, 0x09, 0x40);
    ret += gst412c_write_register(vi_pipe, 0x0a, 0xc0);
    ret += gst412c_write_register(vi_pipe, 0x0d, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x0f, 0x08);
    ret += gst412c_write_register(vi_pipe, 0x10, 0x20);
    ret += gst412c_write_register(vi_pipe, 0x11, 0x20);
    ret += gst412c_write_register(vi_pipe, 0x19, 0x07);
    ret += gst412c_write_register(vi_pipe, 0x1B, 0xFF);
    ret += gst412c_write_register(vi_pipe, 0x1d, 0x3e);
    ret += gst412c_write_register(vi_pipe, 0x1e, 0x7f);
    ret += gst412c_write_register(vi_pipe, 0x20, 0xff);
    ret += gst412c_write_register(vi_pipe, 0x21, 0xff);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa9);
    ret += gst412c_write_register(vi_pipe, 0x03, 0x01);
    ret += gst412c_write_register(vi_pipe, 0x06, 0x37);
    ret += gst412c_write_register(vi_pipe, 0x07, 0x12);
    ret += gst412c_write_register(vi_pipe, 0x0a, 0x88);
    ret += gst412c_write_register(vi_pipe, 0x0b, 0x13);
    ret += gst412c_write_register(vi_pipe, 0x27, 0x0E);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa8);
    ret += gst412c_write_register(vi_pipe, 0x20, 0x07);
    ret += gst412c_write_register(vi_pipe, 0x1f, 0x0d);
    ret += gst412c_write_register(vi_pipe, 0x2c, 0x02);
    ret += gst412c_write_register(vi_pipe, 0x35, 0x3d);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa9);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xab);
    ret += gst412c_write_register(vi_pipe, 0x12, 0x04);
    ret += gst412c_write_register(vi_pipe, 0x18, 0x24);
    ret += gst412c_write_register(vi_pipe, 0x7C, 0xA9);
    ret += gst412c_write_register(vi_pipe, 0x42, 0x05);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa8);
    return ret;
}

static td_s32 gst412c_linear_4m30_12bit_init_3(ot_vi_pipe vi_pipe)
{
    td_s32 ret = TD_SUCCESS;
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa8);
    ret += gst412c_write_register(vi_pipe, 0x2d, 0x38);
    ret += gst412c_write_register(vi_pipe, 0x2e, 0x00);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xa8);
    ret += gst412c_write_register(vi_pipe, 0x28, 0x05);
    ret += gst412c_write_register(vi_pipe, 0x7c, 0xab);
    ret += gst412c_write_register(vi_pipe, 0x20, 0x1f);
    ret += gst412c_write_register(vi_pipe, 0x21, 0xff);
    ret += gst412c_write_register(vi_pipe, 0x09, 0x3e);
    return ret;
}

static void gst412c_linear_4m30_12bit_init(ot_vi_pipe vi_pipe)
{
    td_u32 gpio_chip_num2 = 2; // gpio2
    td_s32 ret = TD_SUCCESS;
    ret += gst412c_linear_4m30_12bit_init_1(vi_pipe);
    ret += gst412c_linear_4m30_12bit_init_2(vi_pipe);
    ret += gst412c_linear_4m30_12bit_init_3(vi_pipe);
    if (ret != TD_SUCCESS) {
        isp_err_trace("gst412c write register failed!\n");
        return;
    }

    gpio_export(gpio_chip_num2, 0x1);  // gpio2_1
    gpio_export(gpio_chip_num2, 0x3);  // gpio2_3
    gpio_dir(gpio_chip_num2, 0x1);  // gpio2_1 out
    gpio_dir(gpio_chip_num2, 0x3);  // gpio2_3 out
    gpio_write(gpio_chip_num2, 0x1, 0); // set GPIO2_1 0
    gpio_write(gpio_chip_num2, 0x3, 1); // set GPIO2_3 1
    delay_ms(50);                       // delay 50ms
    gpio_write(gpio_chip_num2, 0x1, 0); // set GPIO2_1 0
    gpio_write(gpio_chip_num2, 0x3, 0); // set GPIO2_3 0

    gpio_unexport(gpio_chip_num2, 0x0);
    gpio_unexport(gpio_chip_num2, 0x1);
    gpio_unexport(gpio_chip_num2, 0x2);
    gpio_unexport(gpio_chip_num2, 0x3);

    printf("========================================================================\n");
    printf("vi_pipe:%d,== gst412c 24Mclk 4M30fps(MIPI) 12bit linear init success! ==\n", vi_pipe);
    printf("========================================================================\n");
    return;
}