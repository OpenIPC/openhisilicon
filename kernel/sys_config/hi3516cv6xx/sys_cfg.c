/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "sys_cfg.h"
#include <asm/io.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_osal.h"
#include "sys_ctrl.h"
#include "clk_cfg.h"
#include "pin_mux.h"
#include "securec.h"

#define SENSOR_LIST_CMDLINE_LEN 256
#define SENSOR_NAME_LEN 64
#define SENSOR_NAME_MAX 8
#define SENSOR_NUM 2
#define BOARD_NAME_LEN 64

#define SENSOR_NAME_SC4336P "sc4336p"
#define SENSOR_NAME_GC4023 "gc4023"
#define SENSOR_NAME_SC450AI "sc450ai"
#define SENSOR_NAME_SC500AI "sc500ai"
#define SENSOR_NAME_SC431HAI "sc431hai"
#define SENSOR_NAME_OS04D10 "os04d10"
#define MIPI_AD "mipi_ad"

static void *g_reg_crg_base = NULL;
static void *g_reg_sys_base = NULL;
static void *g_reg_ddr_base = NULL;
static void *g_reg_misc_base = NULL;
static void *g_reg_vivo_misc_base = NULL;
static void *g_reg_iocfg_base = NULL;
static void *g_reg_iocfg2_base = NULL;
static void *g_reg_iocfg3_base = NULL;
static void *g_reg_gpio_base = NULL;

void *sys_config_get_reg_crg(void)
{
    return g_reg_crg_base;
}

void *sys_config_get_reg_sys(void)
{
    return g_reg_sys_base;
}

void *sys_config_get_reg_ddr(void)
{
    return g_reg_ddr_base;
}

void *sys_config_get_reg_misc(void)
{
    return g_reg_misc_base;
}

void *sys_config_get_reg_iocfg(void)
{
    return g_reg_iocfg_base;
}

void *sys_config_get_reg_iocfg2(void)
{
    return g_reg_iocfg2_base;
}

void *sys_config_get_reg_iocfg3(void)
{
    return g_reg_iocfg3_base;
}

void *sys_config_get_reg_gpio(void)
{
    return g_reg_gpio_base;
}

static char g_sensor_list[SENSOR_LIST_CMDLINE_LEN] = "sns0=sc4336p,sns1=sc4336p";
static int g_vi_intf_type = VI_MIPI_RX_MODE;
static char g_vi_sensor_name[SENSOR_NUM][SENSOR_NAME_LEN] = {SENSOR_NAME_SC4336P, SENSOR_NAME_SC4336P};
static int g_board = BOARD_DMEB_QFN;
static char g_board_name[BOARD_NAME_LEN] = BOARD_NAME_DMEB_QFN;        /* sck dmeb_qfn dmeb_bga */
static bool g_ir_auto_en = 0;

#ifndef MODULE
osal_setup_str_param(sensors, g_sensor_list);
osal_setup_str_param(board, g_board_name);
osal_setup_num_param(ir_auto, g_ir_auto_en);
#else
module_param_string(sensors, g_sensor_list, SENSOR_LIST_CMDLINE_LEN, 0600);
MODULE_PARM_DESC(sensors, "sns0=sc4336p,sns1=sc4336p");
module_param_string(board, g_board_name, BOARD_NAME_LEN, 0600);
module_param_named(ir_auto, g_ir_auto_en, bool, 0600);
#endif


bool sys_config_is_hi3516cv610_10b(void)
{
    unsigned long chip_id = 0x0;

    if (g_reg_sys_base != 0) {
        chip_id = sys_readl(g_reg_sys_base, 0xEE0);
    }

    return (chip_id == 0x3516C610 || chip_id == 0x24A10); // chip id 0x3516CV610_10B, 0x24A10
}

bool sys_config_is_hi3516cv608(void)
{
    unsigned long chip_id = 0x0;

    if (g_reg_sys_base != 0) {
        chip_id = sys_readl(g_reg_sys_base, 0xEE0);
    }

    return (chip_id == 0x3516C608 || chip_id == 0x24A08); // chip id 0x3516CV608, 0x24A08
}

int sys_config_get_vi_intf_type(void)
{
    return g_vi_intf_type;
}

int sys_config_get_board_type(void)
{
    return g_board;
}

bool sys_config_get_ir_auto(void)
{
    return g_ir_auto_en;
}

#define sys_config_ioremap_return(reg_name, addr, reg_len) \
    do {                                                   \
        (reg_name) = (void *)ioremap((addr), (reg_len));   \
        if ((reg_name) == NULL) {                          \
            return (-1);                                   \
        }                                                  \
    } while (0)

#define sys_config_iounmap(reg_name) \
    do {                             \
        if ((reg_name) != NULL) {    \
            iounmap((reg_name));     \
            (reg_name) = 0;          \
        }                            \
    } while (0)

static int sysconfig_init(void)
{
    sys_config_ioremap_return(g_reg_crg_base, 0x11010000, 0x10000);
    sys_config_ioremap_return(g_reg_sys_base, 0x11020000, 0x4000);
    sys_config_ioremap_return(g_reg_misc_base, 0x11024000, 0x2000);
    sys_config_ioremap_return(g_reg_vivo_misc_base, 0x17950000, 0x2000);
    sys_config_ioremap_return(g_reg_ddr_base, 0x11140000, 0x10000);
    sys_config_ioremap_return(g_reg_iocfg_base, 0x10260000, 0x10000);   /* io0_cfg  AHB_IOCFG */
    sys_config_ioremap_return(g_reg_iocfg2_base, 0x17940000, 0x10000);  /* io2_cfg  VIVO_AIAO_IOCFG */
    sys_config_ioremap_return(g_reg_iocfg3_base, 0x11130000, 0x10000);  /* io1_cfg  CORE_IOCFG */
    sys_config_ioremap_return(g_reg_gpio_base, 0x11090000, 0x10000);

    return 0;
}

static void sysconfig_exit(bool is_early_release)
{
    if (is_early_release != true) {
        sys_config_iounmap(g_reg_misc_base);
        sys_config_iounmap(g_reg_crg_base);
        sys_config_iounmap(g_reg_sys_base);
    }
    sys_config_iounmap(g_reg_vivo_misc_base);
    sys_config_iounmap(g_reg_ddr_base);
    sys_config_iounmap(g_reg_iocfg_base);
    sys_config_iounmap(g_reg_iocfg2_base);
    sys_config_iounmap(g_reg_iocfg3_base);
    sys_config_iounmap(g_reg_gpio_base);
}

static void sys_config_parse_board(void)
{
    sys_config_print("==========board_type: %s==========\n", g_board_name);
    /* parse board */
    if (strncmp(BOARD_NAME_DMEB_BGA, g_board_name, strlen(BOARD_NAME_DMEB_BGA)) == 0) {
        g_board  = BOARD_DMEB_BGA;
        sys_config_print("g_board %u\n", BOARD_DMEB_BGA);
    } else if (strncmp(BOARD_NAME_DMEB_QFN, g_board_name, strlen(BOARD_NAME_DMEB_QFN)) == 0) {
        g_board  = BOARD_DMEB_QFN;
        sys_config_print("g_board %u\n", BOARD_DMEB_QFN);
    } else if (strncmp(BOARD_NAME_KOLB, g_board_name, strlen(BOARD_NAME_KOLB)) == 0) {
        g_board  = BOARD_KOLB;
        sys_config_print("g_board %u\n", BOARD_KOLB);
    } else {
        g_board  = BOARD_SCK;
        sys_config_print("g_board default %u\n", BOARD_SCK);
    }
}

static int parse_sensor_index(char *s, int s_len)
{
    char tmp[SENSOR_NAME_MAX] = {0};
    int i;
    char *line = NULL;
    int index = -1;

    if (s_len == 0) {
        sys_config_print("name len is 0!\n");
        return index;
    }
    line = strsep(&s, "=");
    if (line == NULL) {
        printk("FUNC:%s line:%d  err sensor index: [%s] \n", __FUNCTION__, __LINE__, s);
        return index;
    }

    for (i = 0; i < 4; i++) { /* max sensor num 4 */
        (void)snprintf_s(tmp, sizeof(tmp), sizeof(tmp) - 1, "sns%d", i);

        if (strncmp(tmp, line, SENSOR_NAME_MAX) == 0) {
            index = i;
            return index;
        }
    }

    printk("FUNC:%s line:%d  SNS prefix:[%s] is not supported !\n", __FUNCTION__, __LINE__, line);
    return index;
}

static int parse_sensor_name(char *s, char *name, unsigned int name_len)
{
    int ret;
    unsigned int len;
    char *line = NULL;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return -1;
    }
    line = strsep(&s, "=");
    if (line == NULL) {
        return -1;
    }

    line = strsep(&s, "=");
    if (line != NULL) {
        len = strlen(line);
        if (len >= SENSOR_NAME_LEN) {
            printk("FUNC:%s line:%d  name:[%s] is too long, can not longer than %d\n", __FUNCTION__, __LINE__, line,
                SENSOR_NAME_LEN);
            return -1;
        }

        ret = strncpy_s(name, SENSOR_NAME_LEN, line, SENSOR_NAME_LEN - 1);
        if (ret != EOK) {
            return -1;
        }
        return 0;
    }

    return -1;
}

static void sensor_clock_config(int index, unsigned int clock)
{
    int offset = 0x8440;
    offset += index * (0x20);
    sys_writel(g_reg_crg_base, offset, clock);
}

static unsigned int parse_sensor_clock(const char *name, unsigned int name_len)
{
    unsigned int clock = 0x0;
    unsigned int len;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return clock;
    }
    len = SENSOR_NAME_LEN;

    /* get clock from sensor name. */
    if ((strncmp(SENSOR_NAME_SC4336P, name, len) == 0) ||
        (strncmp(SENSOR_NAME_GC4023, name, len) == 0) ||
        (strncmp(SENSOR_NAME_SC431HAI, name, len) == 0) ||
        (strncmp(SENSOR_NAME_SC450AI, name, len) == 0) ||
        (strncmp(SENSOR_NAME_SC500AI, name, len) == 0)) {
        clock = 0xA001;   /* 27M */
    } else if ((strncmp(SENSOR_NAME_OS04D10, name, len) == 0)) {
        clock = 0x4001;  /* 24M */
    } else if (strncmp(MIPI_AD, name, len) == 0) {
        clock = 0x8010; /* 37.125M */
    } else if ((strncmp("bt656", name, len) == 0) ||
               (strncmp("bt1120", name, len) == 0) ||
               (strncmp("bt601", name, len) == 0)) {
        return clock;
    } else {
        sys_config_print("parse sensor[%s] failed!\n", name);
    }
    return clock;
}

static int is_cmos_bt1120(const char *name, unsigned int name_len)
{
    unsigned int len;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return 0;
    }
    len = SENSOR_NAME_LEN;

    if ((strncmp("bt1120", name, len) == 0) || (strncmp("bt601", name, len) == 0)) {
        return 1;
    } else {
        return 0;
    }
}

static int is_cmos_bt656(const char *name, unsigned int name_len)
{
    unsigned int len;

    if (name_len == 0) {
        sys_config_print("name len is 0!\n");
        return 0;
    }
    len = SENSOR_NAME_LEN;

    if (strncmp("bt656", name, len) == 0) {
        return 1;
    } else {
        return 0;
    }
}

static int cmos_clock_config(int index)
{
    if (index == 0) { /* port0 */
        sys_writel(g_reg_crg_base, 0x9164, 0x1010);
        sys_writel(g_reg_vivo_misc_base, 0x0038, 0x101);
        sys_writel(g_reg_crg_base, 0x8440, 0x3);
        sys_writel(g_reg_crg_base, 0x8440, 0xA010); /* sensor0 24M */
    } else if (index == 1) { /* port1 */
        sys_writel(g_reg_crg_base, 0x9184, 0x1010);
        sys_writel(g_reg_vivo_misc_base, 0x0038, 0x110);
        sys_writel(g_reg_crg_base, 0x8440, 0x3);
        sys_writel(g_reg_crg_base, 0x8440, 0xA010); /* sensor0 24M */
    } else {
        sys_config_print("SNS%d is not supported by cmos!\n", index);
        return 0;
    }

    return 1;
}

static void prase_sensor(char *s)
{
    int ret, index;
    char sensor_name[SENSOR_NAME_LEN];

    ret = parse_sensor_name(s, sensor_name, SENSOR_NAME_LEN);
    if (ret >= 0) {
        index = parse_sensor_index(s, strlen(s));
        if (index >= 0 && index < SENSOR_NUM) {
            ret = strncpy_s(g_vi_sensor_name[index], SENSOR_NAME_LEN, sensor_name, SENSOR_NAME_LEN - 1);
            if (ret != EOK) {
                return;
            }
        }
        /* set vi_intf_type by sensor_name, default: VI_MIPI_RX_MODE */
        if (is_cmos_bt1120(sensor_name, SENSOR_NAME_LEN)) {
            g_vi_intf_type = VI_CMOS_BT1120_MODE;
        } else if (is_cmos_bt656(sensor_name, SENSOR_NAME_LEN)) {
            g_vi_intf_type = VI_CMOS_BT656_MODE;
        }
    }
}

static int prase_sensor_mode(char *s, unsigned int str_len)
{
    char *line = NULL;
    char *argv[SENSOR_NAME_MAX] = {NULL};
    int i;

    if (str_len == 0) {
        sys_config_print("name len is 0!\n");
        return 0;
    }

    line = strsep(&s, ":");
    while (line != NULL) {
        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL; i++) {
            prase_sensor(argv[i]);

            if (i == (ARRAY_SIZE(argv) - 1)) {
                break;
            }
        }
        line = strsep(&s, ":");
    }

    return 0;
}

static void sys_config_parse_mod_param(void)
{
    sys_config_parse_board();
    prase_sensor_mode(g_sensor_list, SENSOR_LIST_CMDLINE_LEN);
}

static void sensor_cfg(void)
{
    int i;
    unsigned int clock_index;
    int sensor2_clk_use_ad = 0;

    for (i = 0; i < SENSOR_NUM; i++) {
        sys_config_print("==========sensor%d: %s==========\n", i, g_vi_sensor_name[i]);
        clock_index = parse_sensor_clock(g_vi_sensor_name[i], SENSOR_NAME_LEN);

        if (is_cmos_bt1120(g_vi_sensor_name[i], SENSOR_NAME_LEN) ||
            is_cmos_bt656(g_vi_sensor_name[i], SENSOR_NAME_LEN)) {
            sensor2_clk_use_ad = cmos_clock_config(i);
        } else {
            if (i != 2 || sensor2_clk_use_ad == 0) { /* ad use sensor2 clk */
                sensor_clock_config(i, clock_index);
            }
        }
    }
}

static int __init ot_sysconfig_init(void)
{
    int ret;

    ret = sysconfig_init();
    if (ret != 0) {
        goto end;
    }

    sys_config_parse_mod_param();
    clk_cfg();
    pin_mux();
    sensor_cfg();
#ifndef CONFIG_OT_SYS_KOL_SUPPORT
    sysconfig_exit(true);
#endif
    return 0;

end:
    sysconfig_exit(false);
    return -1;
}

static void __exit ot_sysconfig_exit(void)
{
    sysconfig_exit(false);
    return;
}

module_init(ot_sysconfig_init);
module_exit(ot_sysconfig_exit);

MODULE_LICENSE("GPL");
