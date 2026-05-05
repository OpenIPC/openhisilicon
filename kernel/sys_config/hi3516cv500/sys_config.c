/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2018. All rights reserved.
 * Description: sys config file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-07-10
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#ifndef __HuaweiLite__
#include <linux/of_platform.h>
#endif
#include <linux/types.h>
#include <asm/io.h>
#include "hi_osal.h"
#include "../../compat/kernel_compat.h"

#define VO_BT1120_EN      0
#define VO_BT656_EN       0
#define VO_MIPI_TX_EN     0
#define VO_LCD_8BIT_EN    0
#define VO_LCD_24BIT_EN   0

#define I2S0_EN           0

#define SENSOR_LIST_CMDLINE_LEN     256
#define SENSOR_NAME_LEN             64
#define SENSOR_MAX_NUM              2
#define CHIP_NAME_LEN               64

#define sys_writel(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define sys_read(addr)          (*((volatile int *)(addr)))

#ifdef __HuaweiLite__
#define ARRAY_SIZE(a) ((sizeof(a) / sizeof(a[0])))
#endif

static char* g_reg_crg_base    = 0;
static char* g_reg_ddr_base    = 0;
static char* g_reg_misc_base   = 0;
static char* g_reg_sysctl_base = 0;
static char* g_reg_iocfg_base  = 0;
static char* g_reg_iocfg1_base = 0;
static char* g_reg_iocfg2_base = 0;
static char* g_reg_gpio_base   = 0;

#ifndef __HuaweiLite__
static int g_online_flag = 0;
static int g_cmos_yuv_flag = 0; /* vi: 0--RAW, 1--BT1120/DC, 2--BT656 */
/* imx335 imx327 imx327_2l imx307 imx307_2l imx307_2l_slave imx415
   os05a imx290 imx377 imx458 ov12870 imx206 imx34220 ov2775 ov9284 */
static char g_sensor_list[SENSOR_LIST_CMDLINE_LEN] = "sns0=imx327,sns1=imx327";
static char g_chip_list[CHIP_NAME_LEN] = "hi3516dv300";  /* hi3516cv500  hi3516dv300 hi3516av300 */

module_param(g_online_flag, int, S_IRUGO);
module_param(g_cmos_yuv_flag, int, 0600);
module_param_string(sensors, g_sensor_list, SENSOR_LIST_CMDLINE_LEN, 0600);
module_param_string(chip, g_chip_list, CHIP_NAME_LEN, 0600);

MODULE_PARM_DESC(sensors, "sns0=imx327,sns1=imx327");
#endif

typedef enum {
    BUS_TYPE_I2C = 0,
    BUS_TYPE_SPI = 1,
} bus_type;

int parse_sensor_index(char *s)
{
    char tmp[8];  /* 8 -- accept string length of 'sns0' */
    int i;
    char* line = NULL;
    int index = -1;

    line = strsep(&s, "=");
    if (line == NULL) {
        osal_printk("FUNC:%s line:%d  err sensor index: [%s] \n", __FUNCTION__, __LINE__, s);
        return index;
    }

    if (sizeof(tmp) <= sizeof("sns0")) {
        return index;
    }

    for (i = 0; i < SENSOR_MAX_NUM; i++) {
        snprintf(tmp, sizeof(tmp), "sns%d", i);

        if (strncmp(tmp, line, sizeof(tmp)) == 0) {
            index = i;
            return index;
        }
    }

    osal_printk("FUNC:%s line:%d  SNS prefix:[%s] is not supported !\n", __FUNCTION__, __LINE__, line);

    return index;
}

int parse_sensor_name(char *s, char *name)
{
    unsigned int len;
    char *line = NULL;

    line = strsep(&s, "=");
    if (line == NULL) {
        return -1;
    }

    line = strsep(&s, "="); /* sensor0 & sensor1, need strsep twice */
    if (line == NULL) {
        return -1;
    }

    len = strlen(line);
    if (len >= SENSOR_NAME_LEN) {
        osal_printk("FUNC:%s line:%d  name:[%s] is too long, can not longer than %d\n",
            __FUNCTION__, __LINE__, line, SENSOR_NAME_LEN);
        return -1;
    }

    strncpy(name, line, SENSOR_NAME_LEN - 1);

    return 0;
}

static inline void reg_write32(unsigned long value, unsigned long mask, unsigned long addr)
{
    unsigned long t;

    t = sys_read((const volatile void*)addr);
    t &= ~mask;
    t |= value & mask;
    sys_writel((volatile void*)addr, t);
}

void sensor_clock_config(unsigned int index, unsigned int clock)
{
    reg_write32(clock << ((6 * index) + 2), 0xF << ((6 * index) + 2), /* 2 -- Sns0_clk [5:2], 6 -- Sns1[11:8] */
        (uintptr_t)g_reg_crg_base + 0x00F0);
}

void set_cmos_flag(unsigned int index)
{
    reg_write32(index << 5, 0x1 << 5, (uintptr_t)g_reg_misc_base + 0x0018); /* 5 -- pt1_sel: mipi or cmos */
}

bus_type parse_sensor_bus_type(const char *name)
{
    unsigned int len;
    bus_type type;

    len = SENSOR_NAME_LEN;

    if ((strncmp("imx290", name, len) == 0) ||
        (strncmp("imx327", name, len) == 0) ||
        (strncmp("imx327_2l", name, len) == 0) ||
        (strncmp("imx390", name, len) == 0) ||
        (strncmp("sc4210", name, len) == 0) ||
        (strncmp("imx34220", name, len) == 0) ||
        (strncmp("imx377", name, len) == 0) ||
        (strncmp("imx307", name, len) == 0) ||
        (strncmp("imx307_2l", name, len) == 0) ||
        (strncmp("imx307_2l_slave", name, len) == 0) ||
        (strncmp("imx458", name, len) == 0) ||
        (strncmp("ov12870", name, len) == 0) ||
        (strncmp("imx415", name, len) == 0) ||
        (strncmp("imx335", name, len) == 0) ||
        (strncmp("gc2053", name, len) == 0) ||
        (strncmp("os08a10", name, len) == 0) ||
        (strncmp("os04b10", name, len) == 0) ||
        (strncmp("os05a", name, len) == 0) ||
        (strncmp("ov9284", name, len) == 0) ||
        (strncmp("ps5260", name, len) == 0) ||
        (strncmp("ov2775", name, len) == 0)) {
        type = BUS_TYPE_I2C;
    } else if (strncmp("imx206", name, len) == 0) {
        type = BUS_TYPE_SPI;
    } else {
        osal_printk("FUNC:%s line:%d  SNS:[%s] is not supported !\n", __FUNCTION__, __LINE__, name);
        type = BUS_TYPE_I2C;
    }

    return type;
}

/*
 * 0x0: 74.25MHz; 0x1: 72MHz;0x2: 54MHz;0x3: 50MHz;0x4/0x5/0x6: 24MHz;
 * 0x8: 37.125MHz;0x9: 36MHz;0xA: 27MHz;0xB: 25MHz;0xC/0xD/0xE/0xF: 12MHz;
 */
unsigned int parse_sensor_clock(const char *name)
{
    unsigned int clock = 0x0;
    unsigned int len;

    len = SENSOR_NAME_LEN;

    if ((strncmp("imx290", name, len) == 0) ||
        (strncmp("imx327", name, len) == 0) ||
        (strncmp("imx327_2l", name, len) == 0) ||
        (strncmp("imx307", name, len) == 0) ||
        (strncmp("imx307_2l", name, len) == 0) ||
        (strncmp("imx307_2l_slave", name, len) == 0) ||
        (strncmp("imx415", name, len) == 0) ||
        (strncmp("imx34220", name, len) == 0) ||
        (strncmp("imx335", name, len) == 0)) {
        clock = 0x8;
    } else if ((strncmp("imx458", name, len) == 0) ||
               (strncmp("imx377", name, len) == 0) ||
               (strncmp("ov12870", name, len) == 0) ||
               (strncmp("os08a10", name, len) == 0) ||
               (strncmp("os04b10", name, len) == 0) ||
               (strncmp("os05a", name, len) == 0) ||
               (strncmp("ov9284", name, len) == 0) ||
               (strncmp("ov2775", name, len) == 0)) {
        clock = 0x4;
    } else if ((strncmp("sc4210", name, len) == 0) ||
               (strncmp("gc2053", name, len) == 0) ||
               (strncmp("ps5260", name, len) == 0) ||
               (strncmp("imx390", name, len) == 0)) {
        clock = 0xA;
    } else if (strncmp("imx206", name, len) == 0) {
        clock = 0x1;
    } else {
        osal_printk("FUNC:%s line:%d  SNS:[%s] is not supported !\n", __FUNCTION__, __LINE__, name);
    }

    return clock;
}

int is_coms(const char *name)
{
    unsigned int len;

    len = SENSOR_NAME_LEN;

    if ((strncmp("bt1120", name, len) == 0) ||
        (strncmp("bt656", name, len) == 0) ||
        (strncmp("bt601", name, len) == 0)) {
        return 1;
    } else {
        return 0;
    }
}

static void set_hi3516av300_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x00582c00);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x025d03ff);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x024903ff);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00f00492);
    sys_writel(g_reg_crg_base + 0x00FC, 0x10);
}

static void set_hi3516dv300_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x00582c00);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x027d83FF);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x026d83FF);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00F006DB);
    sys_writel(g_reg_crg_base + 0x00FC, 0x11);
}

static void set_hi3559v200_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x00582000);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x025d03ff);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x024903ff);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00f00492);
    sys_writel(g_reg_crg_base + 0x00FC, 0x10);
}

static void set_hi3556v200_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x007828f0);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x025d03ff);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x024903ff);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00f00492);
    sys_writel(g_reg_crg_base + 0x00FC, 0x10);
}

static void set_hi3566v100_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x00792cf6);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x027d83FF);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x026d83FF);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00F006DB);
    sys_writel(g_reg_crg_base + 0x00FC, 0x11);
}

static void set_hi3562v100_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x00792cf4);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x029e03ff);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x29203ff);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00f00924);
    sys_writel(g_reg_crg_base + 0x00FC, 0x12);
}

static void set_hi3516cv500_clk(int cmos_yuv_flag)
{
    sys_writel(g_reg_crg_base + 0x00A0, 0x00782cf0);

    if (cmos_yuv_flag > 0) {
        sys_writel(g_reg_crg_base + 0x00F4, 0x029e03ff);
    } else {
        sys_writel(g_reg_crg_base + 0x00F4, 0x29203ff);
    }

    sys_writel(g_reg_crg_base + 0x0100, 0x00f00924);
    sys_writel(g_reg_crg_base + 0x00FC, 0x12);
}

void clk_cfg_on_chip(char *s, int cmos_yuv_flag)
{
    if (strncmp("hi3516av300", s, CHIP_NAME_LEN) == 0) {
        set_hi3516av300_clk(cmos_yuv_flag);
    } else if (strncmp("hi3516dv300", s, CHIP_NAME_LEN) == 0) {
        set_hi3516dv300_clk(cmos_yuv_flag);
    } else if (strncmp("hi3559v200", s, CHIP_NAME_LEN) == 0) {
        set_hi3559v200_clk(cmos_yuv_flag);
    } else if (strncmp("hi3556v200", s, CHIP_NAME_LEN) == 0) {
        set_hi3556v200_clk(cmos_yuv_flag);
    } else if (strncmp("hi3566v100", s, CHIP_NAME_LEN) == 0) {
        set_hi3566v100_clk(cmos_yuv_flag);
    }  else if (strncmp("hi3562v100", s, CHIP_NAME_LEN) == 0) {
        set_hi3562v100_clk(cmos_yuv_flag);
    } else {
        set_hi3516cv500_clk(cmos_yuv_flag);
    }
}

int clkcfg(char *s, int cmos_yuv_flag)
{
    osal_printk("\n==========chip: %s==========", s);

    sys_writel(g_reg_crg_base + 0x01B8, 0x0007f81f);
    sys_writel(g_reg_crg_base + 0x00F0, 0x00000861);
    sys_writel(g_reg_crg_base + 0x00F8, 0x0000000F);

    clk_cfg_on_chip(s, cmos_yuv_flag);

    if (cmos_yuv_flag > 0) {
        set_cmos_flag(1);
    } else {
        set_cmos_flag(0);
    }

    sys_writel(g_reg_misc_base + 0x0034, 0x1);
    sys_writel(g_reg_misc_base + 0x0000, 0x0);

    return 0;
}

void set_vi_workmode(int online_flag, int vpss_online_flag)
{
    if (online_flag == 1) {  /* vi_online_vpss_ */
        /* 30:28         26:24       22:20      18:16     14:12      10:8       6:4         2:0 */
        /* nnie_l_wqos  nnie_h_wqos gdc_wqos viproc_wqos vpss_wqos  aio_wqos  vdp_wqos   vicap_wqos */
        if (vpss_online_flag == 1) {
            sys_writel(g_reg_misc_base + 0x0080, 0x46577777);
        } else {
            sys_writel(g_reg_misc_base + 0x0080, 0x46576777);
        }
        /* scd_wqos     spacc_wqos  fmc_wqos emmc_wqos   sdio0_wqos edma_wqos usb_wqos   eth_wqos */
        sys_writel(g_reg_misc_base + 0x0084, 0x33333376);
        /* ddrt_wqos    gzip_wqos   tde_wqos ive_wqos    jpge_wqos  vgs_wqos  vedu_wqos  cpu_wqos */
        sys_writel(g_reg_misc_base + 0x0088, 0x03444455);
        /* ---          ---         ---      ---         ---        ---       sdio1_wqos jpgd_wqos */
        sys_writel(g_reg_misc_base + 0x008c, 0x00000033);

        /* nnie_l_rqos  nnie_h_rqos gdc_rqos viproc_rqos vpss_rqos  aio_rqos  vdp_rqos   vicap_rqos */
        if (vpss_online_flag == 1) {
            sys_writel(g_reg_misc_base + 0x0090, 0x46577777);
        } else {
            sys_writel(g_reg_misc_base + 0x0090, 0x46576777);
        }
        /* scd_rqos     spacc_rqos  fmc_rqos emmc_rqos   sdio0_rqos edma_rqos usb_rqos   eth_rqos */
        sys_writel(g_reg_misc_base + 0x0094, 0x33333376);
        /* ddrt_rqos    gzip_rqos   tde_rqos ive_rqos    jpge_rqos  vgs_rqos  vedu_rqos  cpu_rqos */
        sys_writel(g_reg_misc_base + 0x0098, 0x03444456);
        /* ---          ---         ---      ---         ---        ---       sdio1_rqos jpgd_rqos */
        sys_writel(g_reg_misc_base + 0x009c, 0x00000033);
    } else  { /* vi_offline_vpss_ */
        /* 30:28          26:24        22:20     18:16      14:12     10:8       6:4         2:0 */
        /* nnie_l_wqos   nnie_h_wqos gdc_wqos viproc_wqos vpss_wqos  aio_wqos  vdp_wqos     vicap_wqos */
        sys_writel(g_reg_misc_base + 0x0080, 0x46565667);
        /* scd_wqos      spacc_wqos  fmc_wqos emmc_wqos   sdio0_wqos edma_wqos usb_wqos     eth_wqos */
        sys_writel(g_reg_misc_base + 0x0084, 0x33333376);
        /* ddrt_wqos     gzip_wqos   tde_wqos ive_wqos    jpge_wqos  vgs_wqos  vedu_wqos    cpu_wqos */
        sys_writel(g_reg_misc_base + 0x0088, 0x03444455);
        /* ---           ---         ---      ---         ---        ---       sdio1_wqos   jpgd_wqos */
        sys_writel(g_reg_misc_base + 0x008c, 0x00000033);

        /* nnie_l_rqos   nnie_h_rqos gdc_rqos viproc_rqos vpss_rqos  aio_rqos  vdp_rqos     vicap_rqos */
        sys_writel(g_reg_misc_base + 0x0090, 0x46575677);
        /* scd_rqos      spacc_rqos  fmc_rqos emmc_rqos   sdio0_rqos edma_rqos usb_rqos     eth_rqos */
        sys_writel(g_reg_misc_base + 0x0094, 0x33333376);
        /* ddrt_rqos     gzip_rqos   tde_rqos ive_rqos    jpge_rqos  vgs_rqos  vedu_rqos    cpu_rqos */
        sys_writel(g_reg_misc_base + 0x0098, 0x03445556);
        /*  ---           ---         ---      ---        ---        ---       sdio1_rqos   jpgd_rqos */
        sys_writel(g_reg_misc_base + 0x009c, 0x00000033);
    }
}

EXPORT_SYMBOL(set_vi_workmode);

static void set_ddr_axi_qos(void)
{
    /* set axi qos map mode */
    sys_writel(g_reg_ddr_base + 0x0200, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0210, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0220, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0230, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0240, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0250, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0260, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0270, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0280, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x0290, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x02a0, 0x00110000);
    sys_writel(g_reg_ddr_base + 0x02b0, 0x00110000);

    /* set axi qos write priority map table */
    sys_writel(g_reg_ddr_base + 0x0204, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0214, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0224, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0234, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0244, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0254, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0264, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0274, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0284, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0294, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x02a4, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x02b4, 0x01234567);

    /* set axi qos read priority map table */
    sys_writel(g_reg_ddr_base + 0x0208, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0218, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0228, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0238, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0248, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0258, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0268, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0278, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0288, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x0298, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x02a8, 0x01234567);
    sys_writel(g_reg_ddr_base + 0x02b8, 0x01234567);
}

static void set_ddr_qos_buf(void)
{
    sys_writel(g_reg_ddr_base + 0x4000, 0x00000002);
    sys_writel(g_reg_ddr_base + 0x410c, 0x0000000b);
    sys_writel(g_reg_ddr_base + 0x4110, 0x0000000b);
    /* comment: sys_writel(g_reg_ddr_base+0x4088, 0x00000000); */
    sys_writel(g_reg_ddr_base + 0x408c, 0x90b20906);
    sys_writel(g_reg_ddr_base + 0x4090, 0x90620906);
    sys_writel(g_reg_ddr_base + 0x40f4, 0x00000033);
    sys_writel(g_reg_ddr_base + 0x40ec, 0x00000011);
    sys_writel(g_reg_ddr_base + 0x40f0, 0x00001111);
    sys_writel(g_reg_ddr_base + 0x41f4, 0x00000000);

    sys_writel(g_reg_ddr_base + 0x41f0, 0x1);
    sys_writel(g_reg_ddr_base + 0x40ac, 0x00000080);
    sys_writel(g_reg_ddr_base + 0x41f8, 0x800002);
    sys_writel(g_reg_ddr_base + 0x4068, 0x51);
    sys_writel(g_reg_ddr_base + 0x406c, 0x51);

    sys_writel(g_reg_ddr_base + 0x4300, 0x20040);
    reg_write32(0 << 2, 0x1 << 2, (unsigned long)g_reg_ddr_base + 0x4088); /* 2 register offset. */
}

void sys_ctl(int online_flag)
{
    set_vi_workmode(online_flag, 0);

    set_ddr_axi_qos();
    set_ddr_qos_buf();
}

#ifdef CONFIG_HI_MOTIONFUSION_SUPPORT
/* spi 2 pin mux for motion sensor */
void motionsensor_cfg_mux(void)
{
    sys_writel(g_reg_crg_base + 0x01bc, 0x01807882);
    sys_writel(g_reg_iocfg_base + 0x0050, 0x474);
    sys_writel(g_reg_iocfg_base + 0x0054, 0x474);
    sys_writel(g_reg_iocfg_base + 0x0058, 0x474);
    sys_writel(g_reg_iocfg_base + 0x005c, 0x474);
}
#endif

void i2c0_pin_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0074, 0x422);
    sys_writel(g_reg_iocfg_base + 0x0078, 0x422);
}

void i2c1_pin_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x007C, 0x422);
    sys_writel(g_reg_iocfg_base + 0x0080, 0x522);
}

void i2c2_pin_mux(void)
{
    sys_writel(g_reg_iocfg1_base + 0x0018, 0x521);
    sys_writel(g_reg_iocfg1_base + 0x001C, 0x521);
}

void i2c3_pin_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0090, 0x521);
    sys_writel(g_reg_iocfg2_base + 0x008C, 0x521);
    sys_writel(g_reg_iocfg2_base + 0x0034, 0x470);
}

void i2c6_pin_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0048, 0x423);
    sys_writel(g_reg_iocfg_base + 0x004C, 0x423);
}

void i2c3_reset(void)
{
    sys_writel(g_reg_gpio_base + 0x0400, 0xff);
    sys_writel(g_reg_gpio_base + 0x03fc, 0xff);
}
void spi0_pin_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0074, 0x451);
    sys_writel(g_reg_iocfg_base + 0x0078, 0x411);
    sys_writel(g_reg_iocfg_base + 0x007C, 0x411);
    sys_writel(g_reg_iocfg_base + 0x0080, 0x541);
}

void spi1_pin_mux(void)
{
    reg_write32(0x1 << 13, 0x1 << 13, (uintptr_t)(g_reg_crg_base + 0x01BC)); /* 13 Register bit */
    sys_writel(g_reg_iocfg2_base + 0x0020, 0x651);
    sys_writel(g_reg_iocfg2_base + 0x0024, 0x611);
    sys_writel(g_reg_iocfg2_base + 0x002c, 0x411);
    sys_writel(g_reg_iocfg2_base + 0x0028, 0x541);
}

void spi2_pin_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0050, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x0054, 0x414);
    sys_writel(g_reg_iocfg2_base + 0x0058, 0x414);
    sys_writel(g_reg_iocfg2_base + 0x005c, 0x444);
}

void sensor_cfg_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x006C, 0x461);
    sys_writel(g_reg_iocfg_base + 0x0070, 0x671);

    sys_writel(g_reg_iocfg_base + 0x0060, 0x662);
    sys_writel(g_reg_iocfg_base + 0x0064, 0x672);
}

void vi_mipi_rx_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0028, 0x400);
    sys_writel(g_reg_iocfg_base + 0x002C, 0x400);
    sys_writel(g_reg_iocfg_base + 0x0030, 0x400);
    sys_writel(g_reg_iocfg_base + 0x0034, 0x400);
    sys_writel(g_reg_iocfg_base + 0x0038, 0x400);
    sys_writel(g_reg_iocfg_base + 0x003C, 0x400);
    sys_writel(g_reg_iocfg_base + 0x0018, 0x400);
    sys_writel(g_reg_iocfg_base + 0x001C, 0x400);
    sys_writel(g_reg_iocfg_base + 0x0020, 0x400);
    sys_writel(g_reg_iocfg_base + 0x0024, 0x400);
}

void vi_bt1120_mode_mux(void)
{
    osal_printk("\n==== vi_bt1120_mode_mux.");
    sys_writel(g_reg_iocfg_base + 0x0010, 0x401);
    sys_writel(g_reg_iocfg_base + 0x0014, 0x401);
    sys_writel(g_reg_iocfg_base + 0x0018, 0x401);
    sys_writel(g_reg_iocfg_base + 0x001C, 0x401);
    sys_writel(g_reg_iocfg_base + 0x0020, 0x401);
    sys_writel(g_reg_iocfg_base + 0x0024, 0x401);
    sys_writel(g_reg_iocfg_base + 0x0028, 0x401);
    sys_writel(g_reg_iocfg_base + 0x002C, 0x401);
    sys_writel(g_reg_iocfg_base + 0x0040, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0044, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0048, 0x471);
    sys_writel(g_reg_iocfg_base + 0x004C, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0050, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0054, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0058, 0x471);
    sys_writel(g_reg_iocfg_base + 0x005C, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0060, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0064, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0068, 0x461);
}

void vi_bt656_mode_mux(void)
{
    osal_printk("\n==== vi_bt656_mode_mux.");
    sys_writel(g_reg_iocfg_base + 0x0040, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0044, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0048, 0x471);
    sys_writel(g_reg_iocfg_base + 0x004C, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0050, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0054, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0058, 0x471);
    sys_writel(g_reg_iocfg_base + 0x005C, 0x471);
    sys_writel(g_reg_iocfg_base + 0x0068, 0x461);
    sys_writel(g_reg_gpio_base + 0x4400, 0xff);
    sys_writel(g_reg_gpio_base + 0x43fc, 0xff);
}

void vi_slave_mode0_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0060, 0x474);
    sys_writel(g_reg_iocfg_base + 0x0064, 0x474);
}

void vi_slave_mode1_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x007C, 0x474);
    sys_writel(g_reg_iocfg_base + 0x0080, 0x474);
}

void vo_bt656_mode_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0068, 0x462);
    sys_writel(g_reg_iocfg_base + 0x0040, 0x472);
    sys_writel(g_reg_iocfg_base + 0x0044, 0x472);
    sys_writel(g_reg_iocfg_base + 0x0048, 0x472);
    sys_writel(g_reg_iocfg_base + 0x004C, 0x472);
    sys_writel(g_reg_iocfg_base + 0x0050, 0x472);
    sys_writel(g_reg_iocfg_base + 0x0054, 0x472);
    sys_writel(g_reg_iocfg_base + 0x0058, 0x472);
    sys_writel(g_reg_iocfg_base + 0x005C, 0x472);
}

void vo_bt1120_mode_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0034, 0x063);
    sys_writel(g_reg_iocfg2_base + 0x0060, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x0050, 0x623);
    sys_writel(g_reg_iocfg2_base + 0x0038, 0x423);
    sys_writel(g_reg_iocfg2_base + 0x003c, 0x623);
    sys_writel(g_reg_iocfg2_base + 0x005c, 0x663);
    sys_writel(g_reg_iocfg2_base + 0x0044, 0x623);
    sys_writel(g_reg_iocfg2_base + 0x0040, 0x623);
    sys_writel(g_reg_iocfg2_base + 0x0048, 0x423);

    sys_writel(g_reg_iocfg2_base + 0x0070, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x006c, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x0078, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x0074, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x0080, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x007c, 0x463);
    sys_writel(g_reg_iocfg2_base + 0x0088, 0x663);
    sys_writel(g_reg_iocfg2_base + 0x0084, 0x663);
}

void mipi_tx_lcd_mux(int lane_num)
{
    int i;

    for (i = 0; i < lane_num; i++) {
        switch (i) {
            case 0:  /* 0 -- lane0 */
                sys_writel(g_reg_iocfg2_base + 0x0088, 0x670);
                sys_writel(g_reg_iocfg2_base + 0x0084, 0x670);
                break;

            case 1:  /* 1 -- lane1 */
                sys_writel(g_reg_iocfg2_base + 0x007C, 0x470);
                sys_writel(g_reg_iocfg2_base + 0x0080, 0x470);
                break;

            case 2:  /* 2 -- lane2 */
                sys_writel(g_reg_iocfg2_base + 0x006C, 0x470);
                sys_writel(g_reg_iocfg2_base + 0x0070, 0x470);
                break;

            case 3:  /* 3 -- lane3 */
                sys_writel(g_reg_iocfg2_base + 0x0068, 0x670);
                sys_writel(g_reg_iocfg2_base + 0x0064, 0x670);
                break;

            default:
                break;
        }
    }

    sys_writel(g_reg_iocfg2_base + 0x0074, 0x460);
    sys_writel(g_reg_iocfg2_base + 0x0078, 0x460);
}

void mipi_tx_set_rest(void)
{
    sys_writel(g_reg_gpio_base + 0x0080, 0x20);
    sys_writel(g_reg_gpio_base + 0x0400, 0x20);
    sys_writel(g_reg_gpio_base + 0x0080, 0x00);
    sys_writel(g_reg_gpio_base + 0x0080, 0x20);
}
void hdmi_pin_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x0000, 0x431);
    sys_writel(g_reg_iocfg_base + 0x0004, 0x431);
    sys_writel(g_reg_iocfg_base + 0x0008, 0x631);
    sys_writel(g_reg_iocfg_base + 0x000C, 0x621);
}

static void vo_3562_6bit_lcd_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0068, 0x654);
    sys_writel(g_reg_iocfg2_base + 0x0084, 0x654);
    sys_writel(g_reg_iocfg2_base + 0x007c, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x0088, 0x654);
    sys_writel(g_reg_iocfg2_base + 0x0080, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x0074, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x0078, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x006C, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x0070, 0x454);
    sys_writel(g_reg_iocfg2_base + 0x0064, 0x654);
}

static void vo_6bit_lcd_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0068, 0x674);
    sys_writel(g_reg_iocfg2_base + 0x0084, 0x674);
    sys_writel(g_reg_iocfg2_base + 0x007c, 0x474);
    sys_writel(g_reg_iocfg2_base + 0x0088, 0x674);
    sys_writel(g_reg_iocfg2_base + 0x0080, 0x474);
    sys_writel(g_reg_iocfg2_base + 0x0074, 0x474);
    sys_writel(g_reg_iocfg2_base + 0x0078, 0x474);
    sys_writel(g_reg_iocfg2_base + 0x006C, 0x474);
    sys_writel(g_reg_iocfg2_base + 0x0070, 0x474);
    sys_writel(g_reg_iocfg2_base + 0x0064, 0x674);
}

void vo_8bit_lcd_reset(void)
{
    sys_writel(g_reg_iocfg2_base + 0x009C, 0x600);
    sys_writel(g_reg_iocfg2_base + 0x009C, 0x500);
}

void vo_8bit_lcd_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0034, 0x422);
    sys_writel(g_reg_iocfg2_base + 0x0058, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x004c, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x0054, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x0048, 0x422);
    sys_writel(g_reg_iocfg2_base + 0x0040, 0x622);
    sys_writel(g_reg_iocfg2_base + 0x0044, 0x622);
    sys_writel(g_reg_iocfg2_base + 0x005C, 0x622);
    sys_writel(g_reg_iocfg2_base + 0x003c, 0x622);
    sys_writel(g_reg_iocfg2_base + 0x0038, 0x422);
    sys_writel(g_reg_iocfg2_base + 0x0050, 0x622);
    sys_writel(g_reg_iocfg2_base + 0x0060, 0x462);
}


void vo_24bit_lcd_mux(void)
{
    sys_writel(g_reg_iocfg_base + 0x004c, 0x470);

    sys_writel(g_reg_gpio_base + 0x3400, 0x08);
    sys_writel(g_reg_gpio_base + 0x3020, 0x00);
    sys_writel(g_reg_gpio_base + 0x3020, 0x08);

    sys_writel(g_reg_iocfg2_base + 0x0034, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x0058, 0x432);
    sys_writel(g_reg_iocfg2_base + 0x004C, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x0054, 0x432);
    sys_writel(g_reg_iocfg2_base + 0x0048, 0x432);
    sys_writel(g_reg_iocfg2_base + 0x0040, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x0044, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x005C, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x003C, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x0038, 0x432);
    sys_writel(g_reg_iocfg2_base + 0x0050, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x0060, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x0084, 0x672);
    sys_writel(g_reg_iocfg2_base + 0x0088, 0x672);
    sys_writel(g_reg_iocfg2_base + 0x007C, 0x472);
    sys_writel(g_reg_iocfg2_base + 0x0080, 0x472);
    sys_writel(g_reg_iocfg2_base + 0x0074, 0x472);
    sys_writel(g_reg_iocfg2_base + 0x0078, 0x472);
    sys_writel(g_reg_iocfg2_base + 0x006C, 0x472);
    sys_writel(g_reg_iocfg2_base + 0x0070, 0x462);
    sys_writel(g_reg_iocfg2_base + 0x0064, 0x672);
    sys_writel(g_reg_iocfg2_base + 0x0068, 0x672);
    sys_writel(g_reg_iocfg2_base + 0x0094, 0x532);
    sys_writel(g_reg_iocfg2_base + 0x0090, 0x532);
    sys_writel(g_reg_iocfg2_base + 0x008C, 0x532);
    sys_writel(g_reg_iocfg2_base + 0x0098, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x009C, 0x632);
    sys_writel(g_reg_iocfg2_base + 0x0030, 0x532);
}

void i2s0_pin_mux(void)
{
    sys_writel(g_reg_iocfg2_base + 0x0020, 0x663);
    sys_writel(g_reg_iocfg2_base + 0x0024, 0x673);
    sys_writel(g_reg_iocfg2_base + 0x0028, 0x573);
    sys_writel(g_reg_iocfg2_base + 0x002C, 0x473);
    sys_writel(g_reg_iocfg2_base + 0x0030, 0x433);
}

int pinmux(char *s, int cmos_yuv_flag)
{
    i2c0_pin_mux();
    i2c1_pin_mux();
    /* comment: i2c2_pin_mux(); */
    /* comment: i2c3_pin_mux(); */
    /* comment: i2c3_reset(); */
    /* comment: spi0_pin_mux(); */
    i2c6_pin_mux();
    sensor_cfg_mux();
#ifdef CONFIG_HI_MOTIONFUSION_SUPPORT
    motionsensor_cfg_mux();
#endif
    vi_mipi_rx_mux();

    if (cmos_yuv_flag == 1) {        /* 1 -- BT1120 */
        vi_bt1120_mode_mux();
    } else if (cmos_yuv_flag == 2) { /* 2 -- BT656 */
        vi_bt656_mode_mux();
    } else {
    }

#if VO_BT1120_EN
    vo_bt1120_mode_mux();
#endif

#if VO_BT656_EN
    vo_bt656_mode_mux();
#endif

#if VO_MIPI_TX_EN
    mipi_tx_lcd_mux(4); /* 4lanes */
#endif

    hdmi_pin_mux();

#if VO_LCD_24BIT_EN
    vo_24bit_lcd_mux();
#endif

#if VO_MIPI_TX_EN
    mipi_tx_set_rest();
#endif

    spi1_pin_mux();

    if (strncmp("hi3562v100", s, CHIP_NAME_LEN) == 0) {
        vo_3562_6bit_lcd_mux();
    } else {
        vo_6bit_lcd_mux();
    }

#if VO_LCD_8BIT_EN
    vo_8bit_lcd_reset();
    vo_8bit_lcd_mux();
#endif

#if I2S0_EN
    i2s0_pin_mux();
#endif
    return 0;
}


void sensor_bus_pin_mux(int index, bus_type bus_type, const char *name)
{
    unsigned int len;

    len = SENSOR_NAME_LEN;

    /* if sensor is slave, please set vi_slave_mode0_mux. */
    if (strncmp("imx206", name, len) == 0) {
        vi_slave_mode0_mux();
    }
    if (strncmp("imx307_2l_slave", name, len) == 0) {
        vi_slave_mode1_mux();
    }

    if (bus_type == BUS_TYPE_I2C) {
        if (index == 0) {
            /* comment: i2c0_pin_mux(); */
        } else if (index == 1) {
            /* comment: i2c1_pin_mux(); */
        }
    } else if (bus_type == BUS_TYPE_SPI) {
        if (index == 0) {
            sys_writel(g_reg_misc_base + 0x0018, 0x1);
            spi0_pin_mux();
        } else if (index == 1) {
            sys_writel(g_reg_misc_base + 0x0018, 0x5);
            spi1_pin_mux();
        }
    }
}

static void sensor_clk_config(char *s)
{
    int ret;
    int index;
    unsigned int clock;
    bus_type bus_type;
    char sensor_name[SENSOR_NAME_LEN];

    ret = parse_sensor_name(s, sensor_name);
    if (ret >= 0) {
        index = parse_sensor_index(s);
        if (index >= 0) {
            osal_printk("\n==========sensor%d: %s==========", index, sensor_name);

            clock = parse_sensor_clock(sensor_name);
            bus_type = parse_sensor_bus_type(sensor_name);

            if (is_coms(sensor_name) == 0) {
                sensor_bus_pin_mux(index, bus_type, sensor_name);
                sensor_clock_config(index, clock);
            }
        }
    }
}

int sensor_config(char *s)
{
    char *line;

    line = strsep(&s, ":");
    while (line != NULL) {
        int i;
        char *argv[8];  /* 8 -- accept string length of 'sns0' */

        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            sensor_clk_config(argv[i]);

            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }
        line = strsep(&s, ":");
    }

    return 0;
}

static int ampunmute(void)
{
    sys_writel(g_reg_iocfg1_base + 0x34, 0x00000431);

    sys_writel(g_reg_gpio_base + 0xa3fc, 0x000000ff);
    sys_writel(g_reg_gpio_base + 0xa400, 0x000000ff);
    sys_writel(g_reg_gpio_base + 0xa3fc, 0x000000ff);

    return 0;
}

static void hi_exit_unmap_comm_reg(void)
{
    /* when set pipe mode, system will call set_vi_workmode, so misc can't iounmap */
    if (g_reg_crg_base != NULL) {
        osal_iounmap(g_reg_crg_base);
        g_reg_crg_base = 0;
    }

    if (g_reg_ddr_base != NULL) {
        osal_iounmap(g_reg_ddr_base);
        g_reg_ddr_base = 0;
    }

    if (g_reg_sysctl_base != NULL) {
        osal_iounmap(g_reg_sysctl_base);
        g_reg_sysctl_base = 0;
    }

    if (g_reg_iocfg_base != NULL) {
        osal_iounmap(g_reg_iocfg_base);
        g_reg_iocfg_base = 0;
    }

    if (g_reg_iocfg1_base != NULL) {
        osal_iounmap(g_reg_iocfg1_base);
        g_reg_iocfg1_base = 0;
    }

    if (g_reg_iocfg2_base != NULL) {
        osal_iounmap(g_reg_iocfg2_base);
        g_reg_iocfg2_base = 0;
    }

    if (g_reg_gpio_base != NULL) {
        osal_iounmap(g_reg_gpio_base);
        g_reg_gpio_base = 0;
    }
}

static int hi_init_map_reg(void)
{
    g_reg_crg_base = (void*)osal_ioremap(0x12010000, 0x10000);
    if (g_reg_crg_base == NULL) {
        return -1;
    }

    g_reg_misc_base = (void*)osal_ioremap(0x12030000, 0x10000);
    if (g_reg_misc_base == NULL) {
        return -1;
    }

    g_reg_sysctl_base = (void*)osal_ioremap(0x12040000, 0x10000);
    if (g_reg_sysctl_base == NULL) {
        return -1;
    }

    g_reg_ddr_base = (void*)osal_ioremap(0x12060000, 0x10000);
    if (g_reg_ddr_base == NULL) {
        return -1;
    }

    g_reg_iocfg_base = (void*)osal_ioremap(0x114F0000, 0x10000);
    if (g_reg_iocfg_base == NULL) {
        return -1;
    }

    g_reg_iocfg1_base = (void*)osal_ioremap(0x111F0000, 0x10000);
    if (g_reg_iocfg1_base == NULL) {
        return -1;
    }

    g_reg_iocfg2_base = (void*)osal_ioremap(0x112F0000, 0x10000);
    if (g_reg_iocfg2_base == NULL) {
        return -1;
    }

    g_reg_gpio_base = (void*)osal_ioremap(0x120D0000, 0x10000);
    if (g_reg_gpio_base == NULL) {
        return -1;
    }

    return 0;
}

int hi_sysconfig_init(int cmos_yuv_flag, int online_flag, char* chip_list, char* sensor_list)
{
    int ret;

    ret = hi_init_map_reg();
    if (ret != 0) {
        goto end;
    }

    pinmux(chip_list, cmos_yuv_flag);

    clkcfg(chip_list, cmos_yuv_flag);

    sys_ctl(online_flag);

    ampunmute();

    sensor_config(sensor_list);

end:
    hi_exit_unmap_comm_reg();

    return 0;
}

void hi_sysconfig_exit(void)
{
    if (g_reg_misc_base != NULL) {
        iounmap(g_reg_misc_base);
        g_reg_misc_base = 0;
    }

    hi_exit_unmap_comm_reg();

    return;
}

#ifndef __HuaweiLite__
static int sys_config_probe(struct platform_device* pdev)
{
    osal_platform_get_module_param(pdev, "g_online_flag", int, &g_online_flag);
    osal_platform_get_module_param(pdev, "g_cmos_yuv_flag", int, &g_cmos_yuv_flag);
    osal_platform_get_modparam_string(pdev, "sensors", SENSOR_LIST_CMDLINE_LEN, g_sensor_list);
    osal_platform_get_modparam_string(pdev, "chip", CHIP_NAME_LEN, g_chip_list);

    return hi_sysconfig_init(g_cmos_yuv_flag, g_online_flag, g_chip_list, g_sensor_list);
}

static compat_platform_remove_ret sys_config_remove(struct platform_device* pdev)
{
    hi_sysconfig_exit();
    compat_platform_remove_return;
}

static const struct of_device_id g_sys_config_match[] = {
    { .compatible = "hisilicon,sys_config" },
    {},
};
MODULE_DEVICE_TABLE(of, g_sys_config_match);

static struct platform_driver g_sys_config_driver = {
    .probe  = sys_config_probe,
    .remove = sys_config_remove,
    .driver = {
        .name           = "sys_config",
        .of_match_table = g_sys_config_match,
    },
};

osal_module_platform_driver(g_sys_config_driver);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
