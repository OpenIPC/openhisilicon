/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "sys_ctrl.h"
#include "sys_cfg.h"

static void set_vi_online_video_norm_vpss_online_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x300:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x304:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x308:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x30c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0300, 0x64667705);
    sys_writel(misc_base, 0x0304, 0x64444664);
    sys_writel(misc_base, 0x0308, 0x30000000);
    sys_writel(misc_base, 0x030c, 0x00005437);

    /*
     * read_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x310:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x314:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x318:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x31c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0310, 0x64767705);
    sys_writel(misc_base, 0x0314, 0x65554664);
    sys_writel(misc_base, 0x0318, 0x30000040);
    sys_writel(misc_base, 0x031c, 0x00005437);
}

static void set_vi_online_video_norm_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x300:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x304:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x308:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x30c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0300, 0x64657705);
    sys_writel(misc_base, 0x0304, 0x64444664);
    sys_writel(misc_base, 0x0308, 0x30000000);
    sys_writel(misc_base, 0x030c, 0x00005437);

    /*
     * read_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x310:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x314:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x318:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x31c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0310, 0x64757705);
    sys_writel(misc_base, 0x0314, 0x65554664);
    sys_writel(misc_base, 0x0318, 0x30000040);
    sys_writel(misc_base, 0x031c, 0x00005437);
}

static void set_vi_online_video_advanced_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x300:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x304:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x308:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x30c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0300, 0x64656705);
    sys_writel(misc_base, 0x0304, 0x64444664);
    sys_writel(misc_base, 0x0308, 0x30000000);
    sys_writel(misc_base, 0x030c, 0x00005437);

    /*
     * read_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x310:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x314:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x318:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x31c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0310, 0x64756705);
    sys_writel(misc_base, 0x0314, 0x65554664);
    sys_writel(misc_base, 0x0318, 0x30000040);
    sys_writel(misc_base, 0x031c, 0x00005437);
}

static void set_vi_offline_video_norm_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();
    /*
     * write_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x300:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x304:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x308:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x30c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0300, 0x64656705);
    sys_writel(misc_base, 0x0304, 0x64444664);
    sys_writel(misc_base, 0x0308, 0x30000000);
    sys_writel(misc_base, 0x030c, 0x00005437);

    /*
     * read_qos:
     *         30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * 0x310:  cpu       coresight       vdp        vpss         viproc      vicap         scd          vedu
     * 0x314:  aiao         vgs         jpgd        jpge         dpu        npu_instr     npu_smmu       ive
     * 0x318:  gsf          sdio1       sdio0       edma         spacc        fmc           gzip          emmc
     * 0x31c:                                                 npu_high_data  npu_low_data    usb3      usb3_uvc
     */
    sys_writel(misc_base, 0x0310, 0x64756705);
    sys_writel(misc_base, 0x0314, 0x65554664);
    sys_writel(misc_base, 0x0318, 0x30000040);
    sys_writel(misc_base, 0x031c, 0x00005437);
}

static void vi_isp_clk_ctrl(int online_flag)
{
    void *crg_base = sys_config_get_reg_crg();
    unsigned long online_clk = 0x4003; /* isp0 reset&cken, 475M */
    unsigned long offline_clk = 0x7003; /* isp0 reset&cken, 594M */

    if (chip_is_hi3516dv500()) {
        online_clk = 0x2003; /* isp0 reset&cken, 300M */
        offline_clk = 0x4003; /* isp0 reset&cken, 475M */
    }

    switch (online_flag) {
        case VI_ONLINE_VIDEO_NORM_VPSS_ONLINE:
        case VI_ONLINE_VIDEO_NORM_VPSS_OFFLINE:
        case VI_ONLINE_VIDEO_ADVANCED_VPSS_OFFLINE:
        case VI_ONLINE_VIDEO_ADVANCED_VPSS_ONLINE:
            sys_writel(crg_base, 0x9150, online_clk);
            break;
        case VI_OFFLINE_VIDEO_NORM_VPSS_ONLINE:
        case VI_OFFLINE_VIDEO_ADVANCED_VPSS_OFFLINE:
        case VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE:
        case VI_OFFLINE_VIDEO_ADVANCED_VPSS_ONLINE:
            sys_writel(crg_base, 0x9150, offline_clk);
            break;
        default:
            return;
    }
}

void sys_ctl(int online_flag)
{
    vi_isp_clk_ctrl(online_flag);
    switch (online_flag) {
        case VI_ONLINE_VIDEO_NORM_VPSS_ONLINE:
            set_vi_online_video_norm_vpss_online_qos();
            break;
        case VI_ONLINE_VIDEO_NORM_VPSS_OFFLINE:
            set_vi_online_video_norm_vpss_offline_qos();
            break;
        case VI_ONLINE_VIDEO_ADVANCED_VPSS_OFFLINE:
        case VI_ONLINE_VIDEO_ADVANCED_VPSS_ONLINE:
            set_vi_online_video_advanced_vpss_offline_qos();
            break;
        case VI_OFFLINE_VIDEO_ADVANCED_VPSS_OFFLINE:
        case VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE:
        case VI_OFFLINE_VIDEO_ADVANCED_VPSS_ONLINE:
            set_vi_offline_video_norm_vpss_offline_qos();
            break;
        default:
            return;
    }
}

EXPORT_SYMBOL(sys_ctl);
