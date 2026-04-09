/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_tx_hal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/11/15
Last Modified :
Description   :
Function List :
******************************************************************************/

#include "hi_osal.h"
#include "type.h"
#include "hi_mipi_tx.h"
#include "mipi_tx_hal.h"
#include "mipi_tx_reg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * macro definition
 */
#define MIPI_TX_REGS_ADDR   0x11270000
#define MIPI_TX_REGS_SIZE   0x10000

#define MIPI_TX_IRQ         120

#define MIPI_TX_CRG         0x1201010C

#define MIPI_TX_REF_CLK     27

#define TLPX                60
#define TCLK_PREPARE        60
#define TCLK_ZERO           250
#define TCLK_TRAIL          80
#define TPRE_DELAY          100
#define THS_PREPARE         80
#define THS_ZERO            180
#define THS_TRAIL           110

/* phy addr */
#define PLL_SET0            0x60
#define PLL_SET1            0x64
#define PLL_SET2            0x65
#ifdef HI_FPGA
#define PLL_SET3            0x17
#endif
#define PLL_SET4            0x66
#define PLL_SET5            0x67

#define DATA0_TPRE_DELAY    0x28
#define DATA1_TPRE_DELAY    0x38
#define DATA2_TPRE_DELAY    0x48
#define DATA3_TPRE_DELAY    0x58

#define CLK_TLPX            0x10
#define CLK_TCLK_PREPARE    0x11
#define CLK_TCLK_ZERO       0x12
#define CLK_TCLK_TRAIL      0x13

#define DATA0_TLPX          0x20
#define DATA0_THS_PREPARE   0x21
#define DATA0_THS_ZERO      0x22
#define DATA0_THS_TRAIL     0x23
#define DATA1_TLPX          0x30
#define DATA1_THS_PREPARE   0x31
#define DATA1_THS_ZERO      0x32
#define DATA1_THS_TRAIL     0x33
#define DATA2_TLPX          0x40
#define DATA2_THS_PREPARE   0x41
#define DATA2_THS_ZERO      0x42
#define DATA2_THS_TRAIL     0x43
#define DATA3_TLPX          0x50
#define DATA3_THS_PREPARE   0x51
#define DATA3_THS_ZERO      0x52
#define DATA3_THS_TRAIL     0x53

#define MIPI_TX_READ_TIMEOUT_CNT 1000

#define PREPARE_COMPENSATE    10
#define ROUNDUP_VALUE     7999
#define INNER_PEROID      8000   /* 8 * 1000 ,1000 is 1us = 1000ns, 8 is division ratio */

/*
 * global operation timing parameters
 */
typedef struct {
    unsigned char data_tpre_delay;
    unsigned char clk_tlpx;
    unsigned char clk_tclk_prepare;
    unsigned char clk_tclk_zero;
    unsigned char clk_tclk_trail;
    unsigned char data_tlpx;
    unsigned char data_ths_prepare;
    unsigned char data_ths_zero;
    unsigned char data_ths_trail;
} mipi_tx_phy_timing_parameters;

/*
 * global variables definition
 */
volatile  mipi_tx_regs_type_t *mipi_tx_regs_va = NULL;

unsigned int mipi_tx_irq_num = MIPI_TX_IRQ;

unsigned int actual_phy_data_rate;

static unsigned int regMapFlag = 0;

/*
 * function definition
 */
static void write_reg32(unsigned long addr,
                                 unsigned int value,
                                 unsigned int mask)
{
    unsigned int t;

    t = osal_readl((void*)addr);
    t &= ~mask;
    t |= value & mask;
    osal_writel(t, (void*)addr);
}

static void set_phy_reg(unsigned int addr, unsigned char value)
{
    osal_isb();
    osal_dsb();
    osal_dmb();
    mipi_tx_regs_va->PHY_TST_CTRL1.u32 = (0x10000 + addr);
    osal_isb();
    osal_dsb();
    osal_dmb();
    mipi_tx_regs_va->PHY_TST_CTRL0.u32 = 0x2;
    osal_isb();
    osal_dsb();
    osal_dmb();
    mipi_tx_regs_va->PHY_TST_CTRL0.u32 = 0x0;
    osal_isb();
    osal_dsb();
    osal_dmb();
    mipi_tx_regs_va->PHY_TST_CTRL1.u32 = value;
    osal_isb();
    osal_dsb();
    osal_dmb();
    mipi_tx_regs_va->PHY_TST_CTRL0.u32 = 0x2;
    osal_isb();
    osal_dsb();
    osal_dmb();
    mipi_tx_regs_va->PHY_TST_CTRL0.u32 = 0x0;
    osal_isb();
    osal_dsb();
    osal_dmb();
}

/*
 * get phy pll parameter set0.
 */
static unsigned char mipi_tx_drv_get_phy_pll_set0(unsigned int phy_data_rate)
{
    unsigned char pll_set0;

    /* pll_set0 */
    if (phy_data_rate > 750) {
        pll_set0 = 0x0;
    } else if (phy_data_rate > 375) {
        pll_set0 = 0x8;
    } else if (phy_data_rate > 188) {
        pll_set0 = 0xa;
    } else if (phy_data_rate > 94) {
        pll_set0 = 0xc;
    } else {
        pll_set0 = 0xe;
    }
    return pll_set0;
}

/*
 * get phy pll parameter set1.
 */
static void mipi_tx_drv_get_phy_pll_set1_set5(unsigned int phy_data_rate,
    unsigned char pll_set0,
    unsigned char *pll_set1,
    unsigned char *pll_set5)
{
    int datarate_clk;
    int pll_ref;

    datarate_clk = (phy_data_rate + MIPI_TX_REF_CLK - 1) / MIPI_TX_REF_CLK;

    if (pll_set0 / 2 == 4) {
        pll_ref = 2;
    } else if (pll_set0 / 2 == 5) {
        pll_ref = 4;
    } else if (pll_set0 / 2 == 6) {
        pll_ref = 8;
    } else if (pll_set0 / 2 == 7) {
        pll_ref = 16;
    } else {
        pll_ref = 1;
    }
    if ((datarate_clk * pll_ref) % 2) {
        *pll_set1 = 0x10;
        *pll_set5 = (datarate_clk * pll_ref - 1) / 2;
    } else {
        *pll_set1 = 0x20;
        *pll_set5 = datarate_clk * pll_ref / 2 - 1;
    }

    return;
}

/*
 * set phy pll parameters set0, set1, set2, set4.
 */
static void mipi_tx_drv_set_phy_pll_setx(unsigned int phy_data_rate)
{
    unsigned char pll_set0;
    unsigned char pll_set1;
    unsigned char pll_set2;
#ifdef HI_FPGA
    unsigned char pll_set3;
#endif
    unsigned char pll_set4;
    unsigned char pll_set5;

    /* pll_set0 */
    pll_set0 = mipi_tx_drv_get_phy_pll_set0(phy_data_rate);
    set_phy_reg(PLL_SET0, pll_set0);

    /* pll_set1 */
    mipi_tx_drv_get_phy_pll_set1_set5(phy_data_rate, pll_set0, &pll_set1, &pll_set5);
    set_phy_reg(PLL_SET1, pll_set1);

    /* pll_set2 */
    pll_set2 = 0x2;
    set_phy_reg(PLL_SET2, pll_set2);

    /* pll_set4 */
    pll_set4 = 0x0;
    set_phy_reg(PLL_SET4, pll_set4);

#ifdef HI_FPGA
    pll_set3 = 0x1;
    set_phy_reg(PLL_SET3, pll_set3);
#endif

    /* pll_set5 */
    set_phy_reg(PLL_SET5, pll_set5);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy pll info=======\n");
    osal_printk("pll_set0(0x14): 0x%x\n", pll_set0);
    osal_printk("pll_set1(0x15): 0x%x\n", pll_set1);
    osal_printk("pll_set2(0x16): 0x%x\n", pll_set2);
#ifdef HI_FPGA
    osal_printk("pll_set3(0x17): 0x%x\n", pll_set3);
#endif
    osal_printk("pll_set4(0x1e): 0x%x\n", pll_set4);
    osal_printk("=========================\n");
#endif
}

static void mipi_tx_drv_get_phy_clk_prepare(unsigned char *clk_prepare)
{
    unsigned char temp0;
    unsigned char temp1;

    temp0 = ((actual_phy_data_rate * TCLK_PREPARE + ROUNDUP_VALUE) / INNER_PEROID - 1 +
                ((actual_phy_data_rate * PREPARE_COMPENSATE + ROUNDUP_VALUE) / INNER_PEROID) -
                ((((actual_phy_data_rate * TCLK_PREPARE + ROUNDUP_VALUE) / INNER_PEROID +
                ((actual_phy_data_rate * PREPARE_COMPENSATE + ROUNDUP_VALUE) / INNER_PEROID)) * INNER_PEROID -
                PREPARE_COMPENSATE * actual_phy_data_rate - TCLK_PREPARE * actual_phy_data_rate) / INNER_PEROID));
    if (temp0 > 0) {     /* 0 is the minmum  */
        temp1 = temp0;
    } else {
        temp1 = 0;       /* 0 is the minmum  */
    }

    if (((temp1 + 1) * INNER_PEROID - PREPARE_COMPENSATE * actual_phy_data_rate) /* temp + 1 is next level period */
        > 94 * actual_phy_data_rate) {          /* 94 is the  maximum in mipi protocol */
        if (temp0 > 0) {
            *clk_prepare = temp0 - 1;
        } else {
            *clk_prepare = 255;                 /* set 255 will easy to found mistake */
            HI_MIPI_TX_ERR("err when calc phy timing \n");
        }
    } else {
        if (temp0 > 0) {    /* 0 is the minmum  */
            *clk_prepare = temp0;
        } else {
            *clk_prepare = 0;  /* 0 is the minmum  */
        }
    }
}

static void mipi_tx_drv_get_phy_data_prepare(unsigned char *data_prepare)
{
    unsigned char temp0;
    unsigned char temp1;

    /* DATA_THS_PREPARE */
    temp0 = ((actual_phy_data_rate * THS_PREPARE + ROUNDUP_VALUE) / INNER_PEROID - 1 +
            ((actual_phy_data_rate * PREPARE_COMPENSATE + ROUNDUP_VALUE) / INNER_PEROID) -
            ((((actual_phy_data_rate * THS_PREPARE + ROUNDUP_VALUE) / INNER_PEROID +
            ((actual_phy_data_rate * PREPARE_COMPENSATE + ROUNDUP_VALUE) / INNER_PEROID)) * INNER_PEROID -
            PREPARE_COMPENSATE * actual_phy_data_rate - THS_PREPARE * actual_phy_data_rate) / INNER_PEROID));
    if (temp0 > 0) {
        temp1 = temp0;
    } else {
        temp1 = 0;
    }

    if ((actual_phy_data_rate > 105) &&
        (((temp1 + 1) * INNER_PEROID - PREPARE_COMPENSATE * actual_phy_data_rate) >
        (85 * actual_phy_data_rate + 6 * 1000))) { /* 85 + 6UI is the  maximum in mipi protocol */
    if (temp0 > 0) {
            *data_prepare = temp0 - 1;
        } else {
            *data_prepare = 255;                 /* set 255 will easy to found mistake */
            HI_MIPI_TX_ERR("err when calc phy timing \n");
        }
    } else {
        if (temp0 > 0) {
            *data_prepare = temp0;
        } else {
            *data_prepare = 0;
        }
    }
}

/*
 * get global operation timing parameters.
 */
static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    /* DATA0~3 TPRE-DELAY */
    tp->data_tpre_delay = (actual_phy_data_rate * TPRE_DELAY + ROUNDUP_VALUE) / INNER_PEROID - 1; /* 1 is compensate */
    /* CLK_TLPX */
    tp->clk_tlpx = (actual_phy_data_rate * TLPX + ROUNDUP_VALUE) / INNER_PEROID - 1; /* 1 is compensate */
    /* CLK_TCLK_PREPARE */
    mipi_tx_drv_get_phy_clk_prepare(&tp->clk_tclk_prepare);

    /* CLK_TCLK_ZERO */
    if ((actual_phy_data_rate * TCLK_ZERO + ROUNDUP_VALUE) / INNER_PEROID > 4) {         /* 4 is compensate */
        tp->clk_tclk_zero = (actual_phy_data_rate * TCLK_ZERO + ROUNDUP_VALUE) / INNER_PEROID - 4;
    } else {
        tp->clk_tclk_zero = 0;       /* 0 is minimum */
    }

    /* CLK_TCLK_TRAIL */
    tp->clk_tclk_trail = (actual_phy_data_rate * TCLK_TRAIL + ROUNDUP_VALUE) / INNER_PEROID;

    /* DATA_TLPX */
    tp->data_tlpx = (actual_phy_data_rate * TLPX + ROUNDUP_VALUE) / INNER_PEROID - 1;    /* 1 is compensate */

    /* DATA_THS_PREPARE */
    mipi_tx_drv_get_phy_data_prepare(&tp->data_ths_prepare);


    /* DATA_THS_ZERO */
    if ((actual_phy_data_rate * THS_ZERO + ROUNDUP_VALUE) / INNER_PEROID > 4) {             /* 4 is compensate */
        tp->data_ths_zero = (actual_phy_data_rate * THS_ZERO + ROUNDUP_VALUE) / INNER_PEROID - 4;
    } else {
        tp->data_ths_zero = 0;       /* 0 is minimum */
    }

    /* DATA_THS_TRAIL */
    tp->data_ths_trail = (actual_phy_data_rate * THS_TRAIL + ROUNDUP_VALUE) / INNER_PEROID + 1;   /* 1 is compensate */
}

/*
 * set global operation timing parameters.
 */
static void mipi_tx_drv_set_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    unsigned char data_tpre_delay = tp->data_tpre_delay;
    unsigned char clk_tlpx = tp->clk_tlpx;
    unsigned char clk_tclk_prepare = tp->clk_tclk_prepare;
    unsigned char clk_tclk_zero = tp->clk_tclk_zero;
    unsigned char clk_tclk_trail = tp->clk_tclk_trail;
    unsigned char data_tlpx = tp->data_tlpx;
    unsigned char data_ths_prepare = tp->data_ths_prepare;
    unsigned char data_ths_zero = tp->data_ths_zero;
    unsigned char data_ths_trail = tp->data_ths_trail;

    /* DATA0~3 TPRE-DELAY */
    set_phy_reg(DATA0_TPRE_DELAY, data_tpre_delay);
    set_phy_reg(DATA1_TPRE_DELAY, data_tpre_delay);
    set_phy_reg(DATA2_TPRE_DELAY, data_tpre_delay);
    set_phy_reg(DATA3_TPRE_DELAY, data_tpre_delay);

    /* CLK_TLPX */
    set_phy_reg(CLK_TLPX, clk_tlpx);

    /* CLK_TCLK_PREPARE */
    set_phy_reg(CLK_TCLK_PREPARE, clk_tclk_prepare);

    /* CLK_TCLK_ZERO */
    set_phy_reg(CLK_TCLK_ZERO, clk_tclk_zero);

    /* CLK_TCLK_TRAIL */
    set_phy_reg(CLK_TCLK_TRAIL, clk_tclk_trail);

    /*
     * DATA_TLPX
     * DATA_THS_PREPARE
     * DATA_THS_ZERO
     * DATA_THS_TRAIL
     */
    set_phy_reg(DATA0_TLPX, data_tlpx);
    set_phy_reg(DATA0_THS_PREPARE, data_ths_prepare);
    set_phy_reg(DATA0_THS_ZERO, data_ths_zero);
    set_phy_reg(DATA0_THS_TRAIL, data_ths_trail);
    set_phy_reg(DATA1_TLPX, data_tlpx);
    set_phy_reg(DATA1_THS_PREPARE, data_ths_prepare);
    set_phy_reg(DATA1_THS_ZERO, data_ths_zero);
    set_phy_reg(DATA1_THS_TRAIL, data_ths_trail);
    set_phy_reg(DATA2_TLPX, data_tlpx);
    set_phy_reg(DATA2_THS_PREPARE, data_ths_prepare);
    set_phy_reg(DATA2_THS_ZERO, data_ths_zero);
    set_phy_reg(DATA2_THS_TRAIL, data_ths_trail);
    set_phy_reg(DATA3_TLPX, data_tlpx);
    set_phy_reg(DATA3_THS_PREPARE, data_ths_prepare);
    set_phy_reg(DATA3_THS_ZERO, data_ths_zero);
    set_phy_reg(DATA3_THS_TRAIL, data_ths_trail);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy timing parameters=======\n");
    osal_printk("data_tpre_delay(0x30/40/50/60): 0x%x\n", data_tpre_delay);
    osal_printk("clk_tlpx(0x22): 0x%x\n", clk_tlpx);
    osal_printk("clk_tclk_prepare(0x23): 0x%x\n", clk_tclk_prepare);
    osal_printk("clk_tclk_zero(0x24): 0x%x\n", clk_tclk_zero);
    osal_printk("clk_tclk_trail(0x25): 0x%x\n", clk_tclk_trail);
    osal_printk("data_tlpx(0x32/42/52/62): 0x%x\n", data_tlpx);
    osal_printk("data_ths_prepare(0x33/43/53/63): 0x%x\n", data_ths_prepare);
    osal_printk("data_ths_zero(0x34/44/54/64): 0x%x\n", data_ths_zero);
    osal_printk("data_ths_trail(0x35/45/55/65): 0x%x\n", data_ths_trail);
    osal_printk("=========================\n");
#endif
}

/*
 * set data lp2hs,hs2lp time
 * set clk lp2hs,hs2lp time
 * unit: hsclk
 */
static void mipi_tx_drv_set_phy_hs_lp_switch_time(mipi_tx_phy_timing_parameters *tp)
{
    unsigned char data_tpre_delay = tp->data_tpre_delay;
    unsigned char clk_tlpx = tp->clk_tlpx;
    unsigned char clk_tclk_prepare = tp->clk_tclk_prepare;
    unsigned char clk_tclk_zero = tp->clk_tclk_zero;
    unsigned char data_tlpx = tp->data_tlpx;
    unsigned char data_ths_prepare = tp->data_ths_prepare;
    unsigned char data_ths_zero = tp->data_ths_zero;
    unsigned char data_ths_trail = tp->data_ths_trail;

    /* data lp2hs,hs2lp time */
    mipi_tx_regs_va->PHY_TMR_CFG.u32 = ((data_ths_trail - 1) << 16) +
        data_tpre_delay + data_tlpx + data_ths_prepare + data_ths_zero + 7;
    /* clk lp2hs,hs2lp time */
    mipi_tx_regs_va->PHY_TMR_LPCLK_CFG.u32 = ((31 + data_ths_trail) << 16) +
        clk_tlpx + clk_tclk_prepare + clk_tclk_zero + 6;

#ifdef MIPI_TX_DEBUG
        osal_printk("PHY_TMR_CFG(0x9C): 0x%x\n", mipi_tx_regs_va->PHY_TMR_CFG.u32);
        osal_printk("PHY_TMR_LPCLK_CFG(0x98): 0x%x\n", mipi_tx_regs_va->PHY_TMR_LPCLK_CFG.u32);
#endif
}

void mipi_tx_drv_set_phy_cfg(combo_dev_cfg_t *dev_cfg)
{
    mipi_tx_phy_timing_parameters tp = {0};

    /* set phy pll parameters setx */
    mipi_tx_drv_set_phy_pll_setx(dev_cfg->phy_data_rate);

    /* get global operation timing parameters */
    mipi_tx_drv_get_phy_timing_parameters(&tp);

    /* set global operation timing parameters */
    mipi_tx_drv_set_phy_timing_parameters(&tp);

    /* set hs switch to lp and lp switch to hs time  */
    mipi_tx_drv_set_phy_hs_lp_switch_time(&tp);

    /* edpi_cmd_size */
    mipi_tx_regs_va->EDPI_CMD_SIZE.u32 = 0xF0;

    /* phy enable */
    mipi_tx_regs_va->PHY_RSTZ.u32 = 0xf;

    if (dev_cfg->output_mode == OUTPUT_MODE_CSI) {
        if (dev_cfg->output_format == OUT_FORMAT_YUV420_8_BIT_NORMAL) {
            mipi_tx_regs_va->DATATYPE0.u32 = 0x10218;
            mipi_tx_regs_va->CSI_CTRL.u32 = 0x1111;
        } else if (dev_cfg->output_format == OUT_FORMAT_YUV422_8_BIT) {
            mipi_tx_regs_va->DATATYPE0.u32 = 0x1021A;
            mipi_tx_regs_va->CSI_CTRL.u32 = 0x1111;
        }
    } else {
        if (dev_cfg->output_format == OUT_FORMAT_RGB_16_BIT) {
            mipi_tx_regs_va->DATATYPE0.u32 = 0x111213D;
            mipi_tx_regs_va->CSI_CTRL.u32 = 0x10100;
        } else if (dev_cfg->output_format == OUT_FORMAT_RGB_18_BIT) {
            mipi_tx_regs_va->DATATYPE0.u32 = 0x111213D;
            mipi_tx_regs_va->CSI_CTRL.u32 = 0x10100;
        } else if (dev_cfg->output_format == OUT_FORMAT_RGB_24_BIT) {
            mipi_tx_regs_va->DATATYPE0.u32 = 0x111213D;
            mipi_tx_regs_va->CSI_CTRL.u32 = 0x10100;
        }
    }

    mipi_tx_regs_va->PHY_RSTZ.u32 = 0XF;

    osal_msleep(1);

    mipi_tx_regs_va->LPCLK_CTRL.u32 = 0x0;
}

static void set_output_format(combo_dev_cfg_t *dev_cfg)
{
    int color_coding = 0;

    if (dev_cfg->output_mode == OUTPUT_MODE_CSI) {
        if (dev_cfg->output_format == OUT_FORMAT_YUV420_8_BIT_NORMAL) {
            color_coding = 0xd;
        } else if (dev_cfg->output_format == OUT_FORMAT_YUV422_8_BIT) {
            color_coding = 0x1;
        }
    } else {
        if (dev_cfg->output_format == OUT_FORMAT_RGB_16_BIT) {
            color_coding = 0x0;
        } else if (dev_cfg->output_format == OUT_FORMAT_RGB_18_BIT) {
            color_coding = 0x3;
        } else if (dev_cfg->output_format == OUT_FORMAT_RGB_24_BIT) {
            color_coding = 0x5;
        }
    }

    mipi_tx_regs_va->COLOR_CODING.u32 = color_coding;
#ifdef MIPI_TX_DEBUG
    osal_printk("set_output_format: 0x%x\n", color_coding);
#endif
}

static void set_video_mode_cfg(combo_dev_cfg_t *dev_cfg)
{
    int video_mode;

    if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) {
        video_mode = 0;
    } else if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS) {
        video_mode = 1;
    } else {
        video_mode = 2;
    }

    if ((dev_cfg->output_mode == OUTPUT_MODE_CSI) || (dev_cfg->output_mode == OUTPUT_MODE_DSI_CMD)) {
        video_mode = 2;
    }

    mipi_tx_regs_va->VID_MODE_CFG.u32 = 0x3f00 + video_mode;
}

static void set_timing_config(combo_dev_cfg_t *dev_cfg)
{
    unsigned int hsa_time;
    unsigned int hbp_time;
    unsigned int hline_time;

    if (dev_cfg->pixel_clk == 0) {
        osal_printk("dev_cfg->pixel_clk is 0, illegal.\n");
        return;
    }

    hsa_time = actual_phy_data_rate * dev_cfg->sync_info.vid_hsa_pixels * 125 / dev_cfg->pixel_clk;
    hbp_time = actual_phy_data_rate * dev_cfg->sync_info.vid_hbp_pixels  * 125 / dev_cfg->pixel_clk;
    hline_time = actual_phy_data_rate * dev_cfg->sync_info.vid_hline_pixels  * 125 / dev_cfg->pixel_clk;

    mipi_tx_regs_va->VID_HSA_TIME.u32 = hsa_time;
    mipi_tx_regs_va->VID_HBP_TIME.u32 = hbp_time;
    mipi_tx_regs_va->VID_HLINE_TIME.u32 = hline_time;

    mipi_tx_regs_va->VID_VSA_LINES.u32 = dev_cfg->sync_info.vid_vsa_lines;
    mipi_tx_regs_va->VID_VBP_LINES.u32 = dev_cfg->sync_info.vid_vbp_lines;
    mipi_tx_regs_va->VID_VFP_LINES.u32 = dev_cfg->sync_info.vid_vfp_lines;
    mipi_tx_regs_va->VID_VACTIVE_LINES.u32 = dev_cfg->sync_info.vid_active_lines;

#ifdef MIPI_TX_DEBUG
    osal_printk("VID_HSA_TIME(0x48): 0x%x\n", hsa_time);
    osal_printk("VID_HBP_TIME(0x4c): 0x%x\n", hbp_time);
    osal_printk("VID_HLINE_TIME(0x50): 0x%x\n", hline_time);
    osal_printk("VID_VSA_LINES(0x54): 0x%x\n", dev_cfg->sync_info.vid_vsa_lines);
    osal_printk("VID_VBP_LINES(0x58): 0x%x\n", dev_cfg->sync_info.vid_vbp_lines);
    osal_printk("VID_VFP_LINES(0x5c): 0x%x\n", dev_cfg->sync_info.vid_vfp_lines);
    osal_printk("VID_VACTIVE_LINES(0x60): 0x%x\n", dev_cfg->sync_info.vid_active_lines);
#endif
}

void set_lane_config(short lane_id[])
{
    int lane_num = 0;
    int i;

    for (i = 0; i < LANE_MAX_NUM; i++) {
        if (-1 != lane_id[i]) {
            lane_num++;
        }
    }

    mipi_tx_regs_va->PHY_IF_CFG.u32 = lane_num - 1;
}

void mipi_tx_drv_set_clkmgr_cfg(void)
{
    if (actual_phy_data_rate / 160 < 2) {
        mipi_tx_regs_va->CLKMGR_CFG.u32 = 0x102;
    } else {
        mipi_tx_regs_va->CLKMGR_CFG.u32 = 0x100 + (actual_phy_data_rate + 159) / 160;
    }
}

void mipi_tx_drv_set_controller_cfg(combo_dev_cfg_t *dev_cfg)
{
    /* disable input */
    mipi_tx_regs_va->OPERATION_MODE.u32 = 0x0;

    /* vc_id */
    mipi_tx_regs_va->VCID.u32 = 0x0;

    /* output format,color coding */
    set_output_format(dev_cfg);

    /* txescclk,timeout */
    actual_phy_data_rate = ((dev_cfg->phy_data_rate + MIPI_TX_REF_CLK - 1) / MIPI_TX_REF_CLK) * MIPI_TX_REF_CLK;
    mipi_tx_drv_set_clkmgr_cfg();

    /* cmd transmission mode */
    mipi_tx_regs_va->CMD_MODE_CFG.u32 = 0xffffff00;

    /* crc,ecc,eotp tran */
    mipi_tx_regs_va->PCKHDL_CFG.u32 = 0x1e;
    /* gen_vcid_rx */
    mipi_tx_regs_va->GEN_VCID.u32 = 0x0;

    /* mode config */
    mipi_tx_regs_va->MODE_CFG.u32 = 0x1;

    /* video mode cfg */
    set_video_mode_cfg(dev_cfg);
    if ((dev_cfg->output_mode == OUTPUT_MODE_DSI_VIDEO) || (dev_cfg->output_mode == OUTPUT_MODE_CSI)) {
        mipi_tx_regs_va->VID_PKT_SIZE.u32 = dev_cfg->sync_info.vid_pkt_size;
    } else {
        mipi_tx_regs_va->EDPI_CMD_SIZE.u32 = dev_cfg->sync_info.edpi_cmd_size;
    }

    /* num_chunks/null_size */
    mipi_tx_regs_va->VID_NUM_CHUNKS.u32 = 0x0;
    mipi_tx_regs_va->VID_NULL_SIZE.u32 = 0x0;

    /* timing config */
    set_timing_config(dev_cfg);

    /* invact,outvact time */
    mipi_tx_regs_va->LP_CMD_TIM.u32 = 0x0;

    mipi_tx_regs_va->PHY_TMR_CFG.u32 = 0x9002D;

    mipi_tx_regs_va->PHY_TMR_LPCLK_CFG.u32 = 0x29002E;

    mipi_tx_regs_va->EDPI_CMD_SIZE.u32 = 0xF0;

    /* lp_wr_to_cnt */
    mipi_tx_regs_va->LP_WR_TO_CNT.u32 = 0x0;
    /* bta_to_cnt */
    mipi_tx_regs_va->BTA_TO_CNT.u32 = 0x0;

    /* lanes */
    set_lane_config(dev_cfg->lane_id);

    /* phy_txrequlpsclk */
    mipi_tx_regs_va->PHY_ULPS_CTRL.u32 = 0x0;

    /* int msk0 */
    mipi_tx_regs_va->INT_MSK0.u32 = 0x0;

    /* pwr_up unreset */
    mipi_tx_regs_va->PWR_UP.u32 = 0x0;
    mipi_tx_regs_va->PWR_UP.u32 = 0xf;
}

static int mipi_tx_wait_cmd_fifo_empty(void)
{
    U_CMD_PKT_STATUS cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = mipi_tx_regs_va->CMD_PKT_STATUS.u32;

        wait_count++;

        osal_udelay(1);

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            HI_MIPI_TX_ERR("timeout when send cmd buffer \n");
            return -1;
        }
    } while (cmd_pkt_status.bits.gen_cmd_empty == 0);

    return 0;
}

static int mipi_tx_wait_write_fifo_empty(void)
{
    U_CMD_PKT_STATUS cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = mipi_tx_regs_va->CMD_PKT_STATUS.u32;

        wait_count++;

        osal_udelay(1);

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            HI_MIPI_TX_ERR("timeout when send data buffer \n");
            return -1;
        }
    } while (cmd_pkt_status.bits.gen_pld_w_empty == 0);

    return 0;
}

static int mipi_tx_wait_write_fifo_not_full(void)
{
    U_CMD_PKT_STATUS cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = mipi_tx_regs_va->CMD_PKT_STATUS.u32;
        if(wait_count > 0) {
            osal_udelay(1);
            HI_MIPI_TX_ERR("write fifo full happened wait count = %d\n",wait_count);
        }
        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            HI_MIPI_TX_ERR("timeout when wait write fifo not full buffer \n");
            return -1;
        }
        wait_count++;
    } while (cmd_pkt_status.bits.gen_pld_w_full == 1);

    return 0;
}

/*
 * set payloads data by writing register
 * each 4 bytes in cmd corresponds to one register
 */
static void mipi_tx_drv_set_payload_data(unsigned char *cmd, unsigned short cmd_size)
{
    U_GEN_PLD_DATA gen_pld_data;
    int i, j;

    gen_pld_data.u32 = mipi_tx_regs_va->GEN_PLD_DATA.u32;

    for (i = 0; i < (cmd_size / 4); i++) {
        gen_pld_data.bits.gen_pld_b1 = cmd[i * 4];
        gen_pld_data.bits.gen_pld_b2 = cmd[i * 4 + 1];
        gen_pld_data.bits.gen_pld_b3 = cmd[i * 4 + 2];
        gen_pld_data.bits.gen_pld_b4 = cmd[i * 4 + 3];

        mipi_tx_wait_write_fifo_not_full();
        mipi_tx_regs_va->GEN_PLD_DATA.u32 = gen_pld_data.u32;
    }

    j = cmd_size % 4;
    if (j != 0) {
        if (j > 0) {
            gen_pld_data.bits.gen_pld_b1 = cmd[i * 4];
        }
        if (j > 1) {
            gen_pld_data.bits.gen_pld_b2 = cmd[i * 4 + 1];
        }
        if (j > 2) {
            gen_pld_data.bits.gen_pld_b3 = cmd[i * 4 + 2];
        }

        mipi_tx_wait_write_fifo_not_full();
        mipi_tx_regs_va->GEN_PLD_DATA.u32 = gen_pld_data.u32;
    }

#ifdef MIPI_TX_DEBUG
        osal_printk("\n=====set cmd=======\n");
        osal_printk("GEN_PLD_DATA(0x70): 0x%x\n", gen_pld_data);
#endif

    return;
}

int mipi_tx_drv_set_cmd_info(cmd_info_t *cmd_info)
{
    U_GEN_HDR gen_hdr;
    unsigned char *cmd;

    gen_hdr.u32 = mipi_tx_regs_va->GEN_HDR.u32;

    if (cmd_info->cmd != NULL) {
        if (cmd_info->cmd_size > 200 ||cmd_info->cmd_size == 0) {
            HI_MIPI_TX_ERR("set cmd size illegal, size =%d\n",cmd_info->cmd_size);
            return  -1;
        }

        cmd = (unsigned char *)osal_kmalloc(cmd_info->cmd_size, osal_gfp_kernel);
        if (cmd == NULL) {
            HI_MIPI_TX_ERR("kmalloc fail,please check,need %d bytes\n",cmd_info->cmd_size);
            return  -1;
        }

        if (osal_copy_from_user(cmd, cmd_info->cmd, cmd_info->cmd_size)) {
            osal_kfree(cmd);
            return  -1;
        }

        mipi_tx_drv_set_payload_data(cmd, cmd_info->cmd_size);

        osal_kfree(cmd);
    }

    gen_hdr.bits.gen_dt = cmd_info->data_type;
    gen_hdr.bits.gen_wc_lsbyte = cmd_info->cmd_size & 0xff;
    gen_hdr.bits.gen_wc_msbyte = (cmd_info->cmd_size & 0xff00) >> 8;
    mipi_tx_regs_va->GEN_HDR.u32 = gen_hdr.u32;

    /* wait transfer end */
    osal_udelay(350);

    mipi_tx_wait_cmd_fifo_empty();
    mipi_tx_wait_write_fifo_empty();

#ifdef MIPI_TX_DEBUG
    osal_printk("\n=====set cmd=======\n");
    osal_printk("cmd_info->cmd_size: 0x%x\n", cmd_info->cmd_size);
    osal_printk("cmd_info->data_type: 0x%x\n", cmd_info->data_type);
    osal_printk("GEN_HDR(0x6C): 0x%x\n", gen_hdr);
#endif

    return 0;
}

static int mipi_tx_wait_read_fifo_not_empty(void)
{
    U_INT_ST0 int_st0;
    U_INT_ST1 int_st1;
    unsigned int wait_count;
    U_CMD_PKT_STATUS cmd_pkt_status;

    wait_count = 0;
    do {
        int_st1.u32 =  mipi_tx_regs_va->INT_ST1.u32;
        int_st0.u32 =  mipi_tx_regs_va->INT_ST0.u32;

        if ((int_st1.u32 & 0x3e) != 0) {
            HI_MIPI_TX_ERR("err happened when read data, int_st1 = 0x%x,int_st0 = %x\n", int_st1.u32, int_st0.u32);
            return -1;
        }

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            HI_MIPI_TX_ERR("timeout when read data\n");
            return -1;
        }

        wait_count++;

        osal_udelay(1);

        cmd_pkt_status.u32 = mipi_tx_regs_va->CMD_PKT_STATUS.u32;
    } while (cmd_pkt_status.bits.gen_pld_r_empty == 0x1);

    return 0;
}

static int mipi_tx_wait_read_fifo_empty(void)
{
    U_GEN_PLD_DATA pld_data;
    U_INT_ST1 int_st1;
    unsigned int wait_count;

    wait_count = 0;
    do {
        int_st1.u32 = mipi_tx_regs_va->INT_ST1.u32;
        if ((int_st1.bits.gen_pld_rd_err) == 0x0) {
            pld_data.u32 = mipi_tx_regs_va->GEN_PLD_DATA.u32;
        }
        wait_count++;
        osal_udelay(1);
        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            HI_MIPI_TX_ERR("timeout when clear data buffer, the last read data is 0x%x \n", pld_data.u32);
            return -1;
        }
    } while ((int_st1.bits.gen_pld_rd_err) == 0x0);

    return 0;
}



static int mipi_tx_send_short_packet(unsigned char virtual_channel, short unsigned data_type, unsigned short  data_param)
{
    U_GEN_HDR gen_hdr;

    gen_hdr.bits.gen_vc = virtual_channel;
    gen_hdr.bits.gen_dt = data_type;
    gen_hdr.bits.gen_wc_lsbyte = (data_param & 0xff);
    gen_hdr.bits.gen_wc_msbyte = (data_param & 0xff00) >> 8;
    mipi_tx_regs_va->GEN_HDR.u32 = gen_hdr.u32;

    if(mipi_tx_wait_cmd_fifo_empty() != 0) {
        return -1;
    }

    return 0;
}

static int mipi_tx_get_read_fifo_data(unsigned int get_data_size, unsigned char *data_buf)
{
    U_GEN_PLD_DATA pld_data;
    unsigned int i, j;

    for (i = 0; i < get_data_size / 4; i++) {
        if (mipi_tx_wait_read_fifo_not_empty() != 0) {
            return -1;
        }
        pld_data.u32 = mipi_tx_regs_va->GEN_PLD_DATA.u32;
        data_buf[i * 4] = pld_data.bits.gen_pld_b1;
        data_buf[i * 4 + 1] = pld_data.bits.gen_pld_b2;
        data_buf[i * 4 + 2] = pld_data.bits.gen_pld_b3;
        data_buf[i * 4 + 3] = pld_data.bits.gen_pld_b4;
    }

    j = get_data_size % 4;

    if (j != 0) {
        if (mipi_tx_wait_read_fifo_not_empty() != 0) {
            return -1;
        }
        pld_data.u32 = mipi_tx_regs_va->GEN_PLD_DATA.u32;
        if (j > 0) {
            data_buf[i * 4] = pld_data.bits.gen_pld_b1;
        }
        if (j > 1) {
            data_buf[i * 4 + 1] = pld_data.bits.gen_pld_b2;
        }
        if (j > 2) {
            data_buf[i * 4 + 2] = pld_data.bits.gen_pld_b3;
        }
    }

    return 0;
}

void mipi_tx_reset(void)
{
    mipi_tx_regs_va->PWR_UP.u32 = 0x0;
    mipi_tx_regs_va->PHY_RSTZ.u32 = 0xd;
    osal_udelay(1);
    mipi_tx_regs_va->PWR_UP.u32 = 0x1;
    mipi_tx_regs_va->PHY_RSTZ.u32 = 0xf;
    osal_udelay(1);
    return;
}

int mipi_tx_drv_get_cmd_info(get_cmd_info_t *get_cmd_info)
{
    unsigned char* data_buf = NULL;

    if (get_cmd_info->get_data_size > 200 || get_cmd_info->get_data_size == 0) {
        HI_MIPI_TX_ERR("get_data_size illegal, size =%d\n",get_cmd_info->get_data_size);
        return -1;
    }

    if (get_cmd_info->get_data == NULL) {
        HI_MIPI_TX_ERR("get_data is NULL,illegal\n");
        return -1;
    }

    data_buf = (unsigned char*)osal_kmalloc(get_cmd_info->get_data_size, osal_gfp_kernel);
    if (data_buf == NULL) {
        return -1;
    }

    if (mipi_tx_wait_read_fifo_empty() != 0) {
        goto fail0;
    }

    if (mipi_tx_send_short_packet(0, get_cmd_info->data_type, get_cmd_info->data_param) != 0) {
        goto fail0;
    }

    if (mipi_tx_get_read_fifo_data(get_cmd_info->get_data_size, data_buf) != 0) {
        /* fail will block mipi data lane ,so need reset  */
        mipi_tx_reset();
        goto fail0;
    }

    osal_copy_to_user(get_cmd_info->get_data, data_buf, get_cmd_info->get_data_size);

    osal_kfree(data_buf);

    return 0;

fail0:
    osal_kfree(data_buf);
    return -1;
}

void mipi_tx_drv_enable_input(output_mode_t output_mode)
{
    if ((output_mode == OUTPUT_MODE_DSI_VIDEO) || (output_mode == OUTPUT_MODE_CSI)) {
        mipi_tx_regs_va->MODE_CFG.u32 = 0x0;
    }

    if (output_mode == OUTPUT_MODE_DSI_CMD) {
        mipi_tx_regs_va->CMD_MODE_CFG.u32 = 0x0;
    }

    /* enable input */
    mipi_tx_regs_va->OPERATION_MODE.u32 = 0x80150000;

    mipi_tx_regs_va->LPCLK_CTRL.u32 = 0x1;

    mipi_tx_reset();
}

void mipi_tx_drv_disable_input(void)
{
    /* disable input */
    mipi_tx_regs_va->OPERATION_MODE.u32 = 0x0;

    mipi_tx_regs_va->CMD_MODE_CFG.u32 = 0xffffff00;

    /* command mode */
    mipi_tx_regs_va->MODE_CFG.u32 = 0x1;

    mipi_tx_regs_va->LPCLK_CTRL.u32 = 0x0;

    mipi_tx_reset();
}

static int mipi_tx_drv_reg_init(void)
{
    if (!mipi_tx_regs_va) {
        mipi_tx_regs_va = (mipi_tx_regs_type_t *)osal_ioremap(MIPI_TX_REGS_ADDR, (unsigned int)MIPI_TX_REGS_SIZE);
        if (mipi_tx_regs_va == NULL) {
            HI_MIPI_TX_ERR("remap mipi_tx reg addr fail\n");
            return -1;
        }
        regMapFlag = 1;
    }

    return 0;
}

static void mipi_tx_drv_reg_exit(void)
{
    if (regMapFlag == 1) {
        if (mipi_tx_regs_va != NULL) {
            osal_iounmap((void *)mipi_tx_regs_va);
        }
        regMapFlag = 0;
    }
}

static void mipi_tx_drv_hw_init(int smooth)
{
    unsigned long mipi_tx_crg_addr;

    mipi_tx_crg_addr = (unsigned long)osal_ioremap(MIPI_TX_CRG, (unsigned long)0x4);

    /* mipi_tx gate clk enable */
    write_reg32(mipi_tx_crg_addr, 1, 0x1);

    /* reset */
    if (smooth == FALSE) {
        write_reg32(mipi_tx_crg_addr, 1 << 1, 0x1 << 1);
    }

    /* unreset */
    write_reg32(mipi_tx_crg_addr, 0 << 1, 0x1 << 1);

    /* ref clk */
    write_reg32(mipi_tx_crg_addr, 1 << 2, 0x1 << 2);

    osal_iounmap((void *)mipi_tx_crg_addr);
}

int mipi_tx_drv_init(int smooth)
{
    int ret;

    ret = mipi_tx_drv_reg_init();
    if (ret < 0) {
        HI_MIPI_TX_ERR("mipi_tx_drv_reg_init fail!\n");
        goto fail0;
    }

    mipi_tx_drv_hw_init(smooth);

    return 0;

fail0:
    return -1;
}

void mipi_tx_drv_exit(void)
{
    mipi_tx_drv_reg_exit();
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
