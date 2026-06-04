/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include <linux/delay.h>
#include "sys_ctrl.h"
#include "clk_cfg.h"
#include "sys_cfg.h"

static void set_vi_online_video_norm_vpss_online_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x300:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x304:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x308:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0300, 0x55477705);
    sys_writel(misc_base, 0x0304, 0x74057054);
    sys_writel(misc_base, 0x0308, 0x44404447);

    /*
     * read_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x310:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x314:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x318:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0310, 0x66477706);
    sys_writel(misc_base, 0x0314, 0x74057054);
    sys_writel(misc_base, 0x0318, 0x55505547);
}

static void set_vi_online_video_norm_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x300:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x304:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x308:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0300, 0x55447705);
    sys_writel(misc_base, 0x0304, 0x74057054);
    sys_writel(misc_base, 0x0308, 0x44404447);

    /*
     * read_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x310:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x314:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x318:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0310, 0x66457706);
    sys_writel(misc_base, 0x0314, 0x75057054);
    sys_writel(misc_base, 0x0318, 0x55505547);
}

static void set_vi_offline_video_norm_vpss_online_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x300:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x304:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x308:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0300, 0x55446705);
    sys_writel(misc_base, 0x0304, 0x74057054);
    sys_writel(misc_base, 0x0308, 0x44404447);

    /*
     * read_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x310:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x314:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x318:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0310, 0x66456706);
    sys_writel(misc_base, 0x0314, 0x74057054);
    sys_writel(misc_base, 0x0318, 0x55505547);
}

static void set_vi_offline_video_norm_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x300:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x304:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x308:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0300, 0x55447705);
    sys_writel(misc_base, 0x0304, 0x74057054);
    sys_writel(misc_base, 0x0308, 0x44404447);

    /*
     * read_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x310:  cpu_inst     cpu_data     coresight    vpss         viproc       vicap       ddrt       vedu
     * 0x314:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     * 0x318:  eth          sdio1        sdio0        edma         spacc        fmc         usb2       usb2_uvc
     */
    sys_writel(misc_base, 0x0310, 0x66457706);
    sys_writel(misc_base, 0x0314, 0x75057054);
    sys_writel(misc_base, 0x0318, 0x55505547);
}

void sys_ctl(int online_flag)
{
    isp_clk_ctrl();
    switch (online_flag) {
        case VI_ONLINE_VIDEO_NORM_VPSS_ONLINE:
            return set_vi_online_video_norm_vpss_online_qos();
        case VI_ONLINE_VIDEO_NORM_VPSS_OFFLINE:
            return set_vi_online_video_norm_vpss_offline_qos();
        case VI_OFFLINE_VIDEO_NORM_VPSS_ONLINE:
            return set_vi_offline_video_norm_vpss_online_qos();
        case VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE:
            return set_vi_offline_video_norm_vpss_offline_qos();
        default:
            return;
    }
}
EXPORT_SYMBOL(sys_ctl);

static void set_aiisp_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();
    unsigned long t;

    /*
     * write_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x304:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     */
    t = sys_readl(misc_base, 0x0304);
    t &= 0xFFFFFF00;
    t |= 0x65;
    sys_writel(misc_base, 0x0304, t);

    /*
     * read_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x314:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     */
    t = sys_readl(misc_base, 0x0314);
    t &= 0xFFFFFF00;
    t |= 0x65;
    sys_writel(misc_base, 0x0314, t);
}

static void set_normal_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();
    unsigned long t;

    /*
     * write_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x304:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     */
    t = sys_readl(misc_base, 0x0304);
    t &= 0xFFFFFF00;
    t |= 0x54;
    sys_writel(misc_base, 0x0304, t);

    /*
     * read_qos:
     *         30:28        26:24        22:20        18:16        14:12        10:8        6:4        2:0
     * 0x314:  aiao         vgs          gzip         jpge      npu_instr     npu_smmu  npu_high_data  npu_low_data
     */
    t = sys_readl(misc_base, 0x0314);
    t &= 0xFFFFFF00;
    t |= 0x54;
    sys_writel(misc_base, 0x0314, t);
}

void update_ctl_by_aiisp(int aiisp_neable)
{
    if (aiisp_neable == 1) {
        set_aiisp_qos();
    } else {
        set_normal_qos();
    }
}
EXPORT_SYMBOL(update_ctl_by_aiisp);

#ifdef CONFIG_OT_SYS_KOL_SUPPORT
typedef struct {
    unsigned int offset;
    unsigned int mask;
    unsigned int value;
    unsigned int backup;
} syskol_reg_info;

static syskol_reg_info g_syskol_regs_info[] = {
    {0x35c0, 0x0003, 0x0000, 0}, /* SDIO0: cken */
    {0x36c0, 0x0003, 0x0000, 0}, /* SDIO1: cken */
    {0x37cc, 0x0006, 0x0000, 0}, /* ETH/FEPHY: cken */
    {0x38c0, 0x0030, 0x0000, 0}, /* USB2 PHY: cken */
    {0x38c8, 0x0130, 0x0000, 0}, /* USB2 BUS/REF/UTMI: cken */
    {0x3f40, 0x7000, 0x0000, 0}, /* FMC: use 24M */
    {0x4180, 0x2000, 0x2000, 0}, /* UART0: use 3M/24M */
    {0x4188, 0x2000, 0x2000, 0}, /* UART1: use 3M/24M */
    {0x4190, 0x2000, 0x2000, 0}, /* UART2: use 3M/24M */
    {0x4280, 0x0010, 0x0000, 0}, /* I2C0: cken */
    {0x4288, 0x0010, 0x0000, 0}, /* I2C1: cken */
    {0x4290, 0x0010, 0x0000, 0}, /* I2C2: cken */
    {0x4480, 0x0010, 0x0000, 0}, /* SSP0: cken */
    {0x4488, 0x0010, 0x0000, 0}, /* SSP1: cken */
    {0x4580, 0x3000, 0x1000, 0}, /* svb_pwm: need fix 24M(use 24M boot) */
    {0x4598, 0x2000, 0x0000, 0}, /* pwm: use 1M/24M */
    {0x4984, 0x0010, 0x0000, 0}, /* hpm: cken */
    {0x4988, 0x0011, 0x0000, 0}, /* hpm_ctrl: cken */
    {0x4c84, 0x0010, 0x0000, 0}, /* CPM: cken */
    {0x6678, 0x0310, 0x0000, 0}, /* IVE: cken */
    {0x667c, 0x0310, 0x0000, 0}, /* NPU: cken */
    {0x7140, 0x0002, 0x0000, 0}, /* VEDU: cken */
    {0x7340, 0x0002, 0x0000, 0}, /* JPGE: cken */
    {0x8440, 0x0010, 0x0000, 0}, /* SENSOR0: cken */
    {0x8460, 0x0010, 0x0000, 0}, /* SENSOR1: cken */
    {0x8540, 0x0018, 0x0000, 0}, /* CBAR/CIL: cken */
    {0x8560, 0x0010, 0x0000, 0}, /* MIPI_RX0: cken */
    {0x8580, 0x0010, 0x0000, 0}, /* MIPI_RX1: cken */
    {0x9164, 0x0010, 0x0000, 0}, /* VI_PORT0: cken */
    {0x9184, 0x0010, 0x0000, 0}, /* VI_PORT1: cken */
    {0x9150, 0x0010, 0x0000, 0}, /* VI_ISP0: cken */
    {0x9154, 0x0010, 0x0000, 0}, /* VI_ISP1: cken */
    {0x9140, 0x0030, 0x0000, 0}, /* VICAP: cken */
    {0x9740, 0x0010, 0x0000, 0}, /* VI_PROC: cken */
    {0x9b40, 0x0010, 0x0000, 0}, /* VPSS: cken */
    {0x9ec0, 0x0010, 0x0000, 0}, /* VGS: cken */
    {0xa880, 0x0030, 0x0000, 0}, /* aiao: cken */
    {0xaa80, 0x0010, 0x0000, 0}, /* codec: cken */
};

static unsigned int g_syskol_cpu_value;
static unsigned int g_syskol_bus_value;

static inline void syskol_reg_write32(unsigned long value, unsigned long mask, unsigned long offset)
{
    void *crg_base = sys_config_get_reg_crg();
    unsigned long t;

    t = sys_readl(crg_base, offset);
    t &= ~mask;
    t |= value & mask;
    sys_writel(crg_base, offset, t);
}

static inline void syskol_reg_read(unsigned int *pvalue, unsigned long offset)
{
    void *crg_base = sys_config_get_reg_crg();
    *pvalue = sys_readl(crg_base, offset);
}

static void syskol_check_reg_value(unsigned int offset, unsigned int value)
{
    unsigned int temp;
    int max_cnt = 1000;

    syskol_reg_read(&temp, offset);
    while (max_cnt >= 0 && temp != value) {
        syskol_reg_read(&temp, offset);
        udelay(10); /* 10: 10us */
        max_cnt--;
    }
}

static void syskol_sleep_light_suspend(void)
{
    syskol_reg_read(&g_syskol_cpu_value, 0x2040);
    syskol_reg_write32(0, 0xffffffff, 0x2040);

    syskol_reg_read(&g_syskol_bus_value, 0x2000);
    syskol_reg_write32(0, 0xffffffff, 0x2000);
}

static void syskol_sleep_user_suspend(const syskol_sleep_user_param *user_param)
{
    int i;
    int cnt = sizeof(g_syskol_regs_info) / sizeof(g_syskol_regs_info[0]);

    for (i = 0; i < cnt; i++) {
        if (user_param->bits.audio_cken == 1 &&
            (g_syskol_regs_info[i].offset == 0xa880 || g_syskol_regs_info[i].offset == 0xaa80)) {
            continue;
        }
#ifdef CONFIG_CLOCK_DEBUG_INFO
        if ((g_syskol_regs_info[i].backup & g_syskol_regs_info[i].mask) != g_syskol_regs_info[i].value) {
            printk("offset = 0x%x, value = 0x%x, may be unexpected\n",
                g_syskol_regs_info[i].offset, g_syskol_regs_info[i].backup);
        }
#endif
        syskol_reg_read(&g_syskol_regs_info[i].backup, g_syskol_regs_info[i].offset);
        syskol_reg_write32(g_syskol_regs_info[i].value, g_syskol_regs_info[i].mask, g_syskol_regs_info[i].offset);
    }

    syskol_sleep_light_suspend();

    syskol_reg_write32(0x00100000, 0x00100000, 0x0004);
    if (user_param->bits.audio_cken == 0) {
        syskol_reg_write32(0x00100000, 0x00100000, 0x0084);
    }
}

static void syskol_sleep_light_resume(void)
{
    syskol_reg_write32(g_syskol_bus_value, 0xffffffff, 0x2000);
    syskol_reg_write32(g_syskol_cpu_value, 0xffffffff, 0x2040);
}

static void syskol_sleep_user_resume(const syskol_sleep_user_param *user_param)
{
    int i;
    int cnt = sizeof(g_syskol_regs_info) / sizeof(g_syskol_regs_info[0]);

    if (user_param->bits.audio_cken == 0) {
        syskol_reg_write32(0x00000000, 0x00100000, 0x0084);
    }
    syskol_reg_write32(0x00000000, 0x00100000, 0x0004);
    syskol_check_reg_value(0x0038, 0x11); /* wait apll effect */
    if (user_param->bits.audio_cken == 0) {
        syskol_check_reg_value(0x00b8, 0x11); /* wait vpll effect */
    }

    syskol_sleep_light_resume();

    for (i = cnt - 1; i >= 0; i--) {
        if (user_param->bits.audio_cken == 1 &&
            (g_syskol_regs_info[i].offset == 0xa880 || g_syskol_regs_info[i].offset == 0xaa80)) {
            continue;
        }
        syskol_reg_write32(g_syskol_regs_info[i].backup, 0xffffffff, g_syskol_regs_info[i].offset);
    }
}

void syskol_sleep_suspend(syskol_sleep_mode sleep_mode, const syskol_sleep_user_param *user_param)
{
    switch (sleep_mode) {
        case SYSKOL_SLEEP_MODE_NONE:
            return;
        case SYSKOL_SLEEP_MODE_LIGHT:
            syskol_sleep_light_suspend();
            break;
        case SYSKOL_SLEEP_MODE_USER:
            syskol_sleep_user_suspend(user_param);
            break;
        default:
            printk("unknow sleep mode\n");
            return;
    }
}

void syskol_sleep_resume(syskol_sleep_mode sleep_mode, const syskol_sleep_user_param *user_param)
{
    switch (sleep_mode) {
        case SYSKOL_SLEEP_MODE_NONE:
            return;
        case SYSKOL_SLEEP_MODE_LIGHT:
            syskol_sleep_light_resume();
            return;
        case SYSKOL_SLEEP_MODE_USER:
            syskol_sleep_user_resume(user_param);
            return;
        default:
            printk("unknow sleep mode\n");
            return;
    }
}

EXPORT_SYMBOL(syskol_sleep_suspend);
EXPORT_SYMBOL(syskol_sleep_resume);

#endif
