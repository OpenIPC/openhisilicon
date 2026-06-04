/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "clk_cfg.h"
#include "sys_cfg.h"

static void mipirx_vi_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* MIPIRX */
    sys_writel(crg_base, 0x8540, 0x38); /* mipi_cal/mipi_cbar/mipi_cil/mipi reset&cken */
    sys_writel(crg_base, 0x8560, 0x10); /* mipirx pix0 reset&cken */
    sys_writel(crg_base, 0x8580, 0x10); /* mipirx pix1 reset&cken */

    /* VI */
if (sys_config_is_hi3516cv610_10b()) {
#if (HI3516CV610_10B_WDR == 0)
        sys_writel(crg_base, 0x9140, 0x3001); /* vicap ppc&bus reset&cken, ppc 198M */
        sys_writel(crg_base, 0x9164, 0x3001); /* vi port0 reset&cken, 198M */
        sys_writel(crg_base, 0x9184, 0x3001); /* vi port1 reset&cken, 198M */
        sys_writel(crg_base, 0x9740, 0x3001); /* viproc reset&cken, 198M */
#else
        sys_writel(crg_base, 0x9140, 0x6001); /* vicap ppc&bus reset&cken, ppc 380M */
        sys_writel(crg_base, 0x9164, 0x6001); /* vi port0 reset&cken, 380M */
        sys_writel(crg_base, 0x9184, 0x6001); /* vi port1 reset&cken, 380M */
#endif
    } else if (sys_config_is_hi3516cv608()) {
        sys_writel(crg_base, 0x9140, 0x2001); /* vicap ppc&bus reset&cken, ppc 148.5M */
        sys_writel(crg_base, 0x9164, 0x2001); /* vi port0 reset&cken, 148.5M */
        sys_writel(crg_base, 0x9184, 0x2001); /* vi port1 reset&cken, 148.5M */
        sys_writel(crg_base, 0x9740, 0x2001); /* viproc reset&cken, 148.5M */
    } else {
        sys_writel(crg_base, 0x9140, 0x7001); /* vicap ppc&bus reset&cken, ppc 475M */
        sys_writel(crg_base, 0x9164, 0x7001); /* vi port0 reset&cken, 475M */
        sys_writel(crg_base, 0x9184, 0x7001); /* vi port1 reset&cken, 475M */
        sys_writel(crg_base, 0x9740, 0x4001); /* viproc reset&cken, 264M */
    }
}

void isp_clk_ctrl(void)
{
    void *crg_base = sys_config_get_reg_crg();

    if (sys_config_is_hi3516cv610_10b()) {
#if (HI3516CV610_10B_WDR == 0)
        sys_writel(crg_base, 0x9150, 0x3003); /* isp0 reset&cken, 198M */
#else
        sys_writel(crg_base, 0x9150, 0x4003); /* isp0 reset&cken, 264M */
        sys_writel(crg_base, 0x9740, 0x4001); /* viproc reset&cken, 264M */
#endif
        sys_writel(crg_base, 0x9154, 0x3);    /* isp1 reset&cken */
    } else if (sys_config_is_hi3516cv608()) {
        sys_writel(crg_base, 0x9150, 0x2003); /* isp0 reset&cken, 148.5M */
        sys_writel(crg_base, 0x9154, 0x3);    /* isp1 reset&cken */
    } else {
        sys_writel(crg_base, 0x9150, 0x4003); /* isp0 reset&cken, 264M */
        sys_writel(crg_base, 0x9154, 0x3);    /* isp1 reset&cken */
    }
}

void clk_cfg(void)
{
    mipirx_vi_clk_cfg();
    isp_clk_ctrl();
}
