/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "clk_cfg.h"
#include "sys_cfg.h"

static void spi_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* SPI */
    sys_writel(crg_base, 0x4380, 0x1010); /* spi_tft reset&cken 100M */
    sys_writel(crg_base, 0x4480, 0x10);   /* spi0 reset&cken         */
    sys_writel(crg_base, 0x4488, 0x10);   /* spi1 reset&cken         */
    sys_writel(crg_base, 0x4490, 0x10);   /* spi2 reset&cken         */
    sys_writel(crg_base, 0x4498, 0x10);   /* spi3 reset&cken         */
    sys_writel(crg_base, 0x44a0, 0x10);   /* spi_3wire reset&cken    */
}

static void mipirx_vi_isp_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* MIPIRX */
    sys_writel(crg_base, 0x8540, 0x3); /* mipi_cal/mipi_cbar/mipi_cil/mipi reset&cken */
    sys_writel(crg_base, 0x8560, 0x1); /* mipirx pix0 reset&cken */
    sys_writel(crg_base, 0x8580, 0x1); /* mipirx pix1 reset&cken */
    sys_writel(crg_base, 0x85a0, 0x1); /* mipirx pix2 reset&cken */
    sys_writel(crg_base, 0x85c0, 0x1); /* mipirx pix3 reset&cken */

    /* VI & ISP */
    if (chip_is_hi3516dv500()) {
        sys_writel(crg_base, 0x9140, 0x4003); /* vicap ppc&bus reset&cken, ppc 475M */
        sys_writel(crg_base, 0x9164, 0x6001); /* vi port0 reset&cken, 475M */
        sys_writel(crg_base, 0x9184, 0x6001); /* vi port1 reset&cken, 475M */
        sys_writel(crg_base, 0x91a4, 0x6001); /* vi port2 reset&cken, 475M */
        sys_writel(crg_base, 0x91c4, 0x6001); /* vi port3 reset&cken, 475M */
        sys_writel(crg_base, 0x9150, 0x4003); /* isp0 reset&cken, 475M offline */
    } else { /* 19dv500 */
        sys_writel(crg_base, 0x9140, 0x6003); /* vicap ppc&bus reset&cken, ppc 594M */
        sys_writel(crg_base, 0x9164, 0x7001); /* vi port0 reset&cken, 594M */
        sys_writel(crg_base, 0x9184, 0x7001); /* vi port1 reset&cken, 594M */
        sys_writel(crg_base, 0x91a4, 0x7001); /* vi port2 reset&cken, 594M */
        sys_writel(crg_base, 0x91c4, 0x7001); /* vi port3 reset&cken, 594M */
        sys_writel(crg_base, 0x9150, 0x7003); /* isp0 reset&cken, 594M offline */
    }
    sys_writel(crg_base, 0x9148, 0x0);   /* vi_ppc_ch0~6_cken/vi_ppc_pt0~3 cken */
    sys_writel(crg_base, 0x9160, 0x0);   /* vi_cmos ck */
    sys_writel(crg_base, 0x9154, 0x3);   /* isp1 reset&cken */
    sys_writel(crg_base, 0x9158, 0x3);   /* isp2 reset&cken */
    sys_writel(crg_base, 0x915c, 0x3);   /* isp3 reset&cken */
}

static void aiao_audio_gzip_clk_cfg(void)
{
    void *crg_base = sys_config_get_reg_crg();

    /* AIAO */
    sys_writel(crg_base, 0xa880, 0x2030);   /* aiao reset&cken, 786.432M */
    sys_writel(crg_base, 0xa884, 0x10);   /* aiao_dmic reset&cken */

    /* AUDIO */
    sys_writel(crg_base, 0xaa80, 0x0); /* audio_codec reset&cken */
}

void clk_cfg(void)
{
    spi_clk_cfg();
    mipirx_vi_isp_clk_cfg();
    aiao_audio_gzip_clk_cfg();
}
