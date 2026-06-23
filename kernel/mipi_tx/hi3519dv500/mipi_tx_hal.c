/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "mipi_tx_hal.h"
#include "mipi_tx.h"
#include "ot_osal.h"
#include "type.h"
#include "mipi_tx_reg.h"
#include "mipi_tx_phy_reg.h"
#include "mipi_tx_def.h"
#include "ot_mipi_tx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if mipi_tx_desc("pub")

#define mipi_tx_readl(x)        (*(const volatile unsigned int *)(x))
#define mipi_tx_writel(v, x)    (*(volatile unsigned int *)(x) = (v))

static volatile mipi_tx_regs_type_t *g_mipi_tx_regs_va = NULL;
static volatile mipi_tx_phy_regs_type_t *g_mipi_tx_phy_regs_va = NULL;
static unsigned int g_mipi_tx_irq_num = MIPI_TX_IRQ;
static unsigned int g_actual_phy_data_rate;
static unsigned int g_reg_map_flag = 0;
static unsigned int g_phy_reg_map_flag = 0;

unsigned int hal_read_reg(const unsigned int *address)
{
    return *(volatile unsigned int *)(address);
}

void hal_write_reg(unsigned int *address, unsigned int value)
{
    *(volatile unsigned int *)address = value;
}

void mipi_tx_drv_set_irq_num(unsigned int irq_num)
{
    g_mipi_tx_irq_num = irq_num;
}

void mipi_tx_drv_set_regs(const mipi_tx_regs_type_t *regs)
{
    g_mipi_tx_regs_va = (mipi_tx_regs_type_t *)regs;
}

static void mipi_tx_set_actual_phy_data_rate(unsigned int actual_date_rate)
{
    g_actual_phy_data_rate = actual_date_rate;
}

static unsigned int mipi_tx_get_actual_phy_data_rate(void)
{
    return g_actual_phy_data_rate;
}

static void write_reg32(unsigned long addr, unsigned int value, unsigned int mask)
{
    unsigned int t;

    t = mipi_tx_readl((const volatile void *)(uintptr_t)addr);
    t &= ~mask;
    t |= value & mask;
    mipi_tx_writel(t, (volatile void *)(uintptr_t)addr);
}

#ifdef OT_FPGA
static void set_phy_reg(unsigned int addr, unsigned char value)
{
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = (0x10000 + addr);
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = value;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    osal_isb();
    osal_dsb();
    osal_dmb();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    osal_isb();
    osal_dsb();
    osal_dmb();
}
#endif

void mipi_tx_drv_get_dev_status(mipi_tx_dev_phy_t *mipi_tx_phy_ctx)
{
    volatile reg_hori0_det hori0_det;
    volatile reg_hori1_det hori1_det;
    volatile reg_vert_det vert_det;
    volatile reg_vsa_det vsa_det;

    hori0_det.u32 = g_mipi_tx_regs_va->hori0_det.u32;
    hori1_det.u32 = g_mipi_tx_regs_va->hori1_det.u32;
    vert_det.u32 = g_mipi_tx_regs_va->vert_det.u32;
    vsa_det.u32 = g_mipi_tx_regs_va->vsa_det.u32;

    mipi_tx_phy_ctx->hact_det = hori0_det.bits.hact_det;
    mipi_tx_phy_ctx->hall_det = hori0_det.bits.hline_det;
    mipi_tx_phy_ctx->hbp_det  = hori1_det.bits.hbp_det;
    mipi_tx_phy_ctx->hsa_det  = hori1_det.bits.hsa_det;

    mipi_tx_phy_ctx->vact_det = vert_det.bits.vact_det;
    mipi_tx_phy_ctx->vall_det = vert_det.bits.vall_det;
    mipi_tx_phy_ctx->vsa_det  = vsa_det.bits.vsa_det;
}

#endif

#if mipi_tx_desc("phy pll")

#ifdef OT_FPGA
static void mipi_tx_drv_set_phy_pll_setx(unsigned int phy_data_rate)
{
    unsigned char pll_set0;
    unsigned char pll_set1;
    unsigned char pll_set2;
    unsigned char pll_set3;
    unsigned char pll_set4;

    /* step2 : */
    /* pll_set0 */
    pll_set0 = 0x18;
    set_phy_reg(PLL_SET_0X14, pll_set0);

    /* pll_set1 */
    pll_set1 = 0x8;
    set_phy_reg(PLL_SET_0X15, pll_set1);

    /* pll_set2 */
    pll_set2 = 0x21;
    set_phy_reg(PLL_SET_0X16, pll_set2);

    /* pll_set3 */
    pll_set3 = 0x1;
    set_phy_reg(PLL_SET_0X17, pll_set3);

    /* pll_set4 */
    pll_set4 = 0x28;
    set_phy_reg(PLL_SET_0X1E, pll_set4);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy pll info=======\n");
    osal_printk("pll_set0(0x14): 0x%x\n", pll_set0);
    osal_printk("pll_set1(0x15): 0x%x\n", pll_set1);
    osal_printk("pll_set2(0x16): 0x%x\n", pll_set2);
    osal_printk("pll_set3(0x17): 0x%x\n", pll_set3);
    osal_printk("pll_set4(0x1e): 0x%x\n", pll_set4);
    osal_printk("=========================\n");
#endif
}

#else

static void mipi_tx_drv_set_phy_ictrl(unsigned int loop_div)
{
    unsigned int phy_ictrl = 0;
    unsigned int phy_vco = MIPI_TX_REF_CLK * loop_div / 10; /* 10: phy_vco MHZ */

    if (phy_vco > 1770) { /* VCO: 1770 MHZ */
        phy_ictrl = 14; /* 14: phy params after optimization */
    } else if (phy_vco > 1680) { /* VCO: 1680 MHZ */
        phy_ictrl = 13; /* 13: phy params after optimization */
    } else if (phy_vco > 1560) { /* VCO: 1560 MHZ */
        phy_ictrl = 12; /* 12: phy params after optimization */
    } else if (phy_vco > 1450) { /* VCO: 1450 MHZ */
        phy_ictrl = 11; /* 11: phy params after optimization */
    } else if (phy_vco > 1330) { /* VCO: 1330 MHZ */
        phy_ictrl = 10; /* 10: phy params after optimization */
    } else if (phy_vco > 1220) { /* VCO: 1220 MHZ */
        phy_ictrl = 9; /* 9: phy params after optimization */
    } else if (phy_vco > 1100) { /* VCO: 1100 MHZ */
        phy_ictrl = 8; /* 8: phy params after optimization */
    } else {
        phy_ictrl = 7; /* 7: phy params after optimization */
    }
    g_mipi_tx_phy_regs_va->aphy_txpll_inctrl.u32 = phy_ictrl;

#ifdef MIPI_TX_DEBUG
    osal_printk("pll ctrl (0x10): 0x%x\n", g_mipi_tx_phy_regs_va->aphy_txpll_inctrl.u32);
#endif
}


static int mipi_tx_drv_get_phy_pll_post_div(unsigned int phy_data_rate, unsigned int *conf_post_div)
{
    int post_div = 1;

    if (phy_data_rate < 62) { /* below 62: need 32 div */
        post_div = 32; /* 62: need 32 div */
        *conf_post_div = 0b101;
    } else if (phy_data_rate < 124) { /* 62-124: need 16 div */
        post_div = 16; /* 62-124: need 16 div */
        *conf_post_div = 0b100;
    } else if (phy_data_rate < 249) { /* 124-249: need 8 div */
        post_div = 8; /* 124-249: need 8 div */
        *conf_post_div = 0b11;
    } else if (phy_data_rate < 499) { /* 249-499: need 4 div */
        post_div = 4; /* 249-499: need 4 div */
        *conf_post_div = 0b10;
    } else if (phy_data_rate < 998) { /* 499-998: need 2 div */
        post_div = 2; /* 499-998: need 2 div */
        *conf_post_div = 0b1;
    } else {
        post_div = 1; /* larger 998: need 1 div */
        *conf_post_div = 0b0;
    }

    return post_div;
}

static void mipi_tx_drv_set_phy_pll_setx(unsigned int phy_data_rate)
{
    volatile u_common_reg_48 common_reg_48;
    volatile u_common_reg_49 common_reg_49;
    volatile u_common_reg_4a common_reg_4a;
    volatile u_common_reg_4b common_reg_4b;
    unsigned int *addr_reg;
    unsigned int conf_post_div = 0;
    unsigned int loop_div;
    unsigned int input_div = 0;
    /* 10: unit trans to MHZ */
    unsigned int post_div = mipi_tx_drv_get_phy_pll_post_div(phy_data_rate / 10, &conf_post_div);

    loop_div = (phy_data_rate * post_div) /
        (MIPI_TX_REF_CLK * (input_div + 1)); /* phy_data_rate & MIPI_TX_REF_CLK unit is MHZ*10 */

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->common_reg_48));
    common_reg_48.u32 = hal_read_reg(addr_reg);
    common_reg_48.bits.reg_pll_phy_posdiv = conf_post_div;
    hal_write_reg(addr_reg, common_reg_48.u32);

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->common_reg_49));
    common_reg_49.u32 = hal_read_reg(addr_reg);
    common_reg_49.bits.reg_pll_phy_prediv = input_div;
    common_reg_49.bits.reg_pll_phy_div_int_bit10to8 = 0x0;
    hal_write_reg(addr_reg, common_reg_49.u32);

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->common_reg_4a));
    common_reg_4a.u32 = hal_read_reg(addr_reg);
    common_reg_4a.bits.reg_pll_phy_div_int_bit7to0 = loop_div;
    hal_write_reg(addr_reg, common_reg_4a.u32);

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->common_reg_4b));
    common_reg_4b.u32 = hal_read_reg(addr_reg);
    common_reg_4b.bits.reg_pll_phy_div_update = 1;
    hal_write_reg(addr_reg, common_reg_4b.u32);

    mipi_tx_drv_set_phy_ictrl(loop_div);
#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy pll info=======\n");
    osal_printk("loop_div: %d\n", loop_div);
    osal_printk("post_div: %d\n", post_div);
    osal_printk("conf_post_div: 0x%d\n", conf_post_div);
    osal_printk("phy_data_rate: %dMHZ\n", phy_data_rate / 10); /* phy_data_rate unit is MHZ*10 */
    osal_printk("=========================\n");
#endif
    return;
}
#endif /* end of #ifdef OT_FPGA */

#endif /* end of #if mipi_tx_desc("phy pll") */

#if mipi_tx_desc("phy time parameter")

#ifdef OT_FPGA

static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    tp->data_tpre_delay = DATA0_TPRE_DELAY_VALUE;
    tp->clk_post_delay = CLK_POST_DELAY_VALUE;
    tp->clk_tlpx = CLK_TLPX_VALUE;
    tp->clk_tclk_prepare = CLK_TCLK_PREPARE_VALUE;
    tp->clk_tclk_zero = CLK_TCLK_ZERO_VALUE;
    tp->clk_tclk_trail = CLK_TCLK_TRAIL_VALUE;
    tp->data_tlpx = DATA0_TLPX_VALUE;
    tp->data_ths_prepare = DATA0_THS_PREPARE_VALUE;
    tp->data_ths_zero = DATA0_THS_ZERO_VALUE;
    tp->data_ths_trail = DATA0_THS_TRAIL_VALUE;
}

#else

static inline unsigned char mipi_tx_drv_get_phy_timing_parameters_by_type(unsigned int timing_type,
    unsigned char round_type)
{
    unsigned int actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();
    unsigned int round_value = (round_type == 0 ? ROUNDUP_VALUE : (INNER_PEROID / 2)); /* 2: half */
    /* actual_phy_data_rate unit is MHZ*10 */
    return ((actual_phy_data_rate * timing_type / PREPARE_COMPENSATE + round_value) /
        INNER_PEROID - 1); /* -1: logical add 1 itself */
}

static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    unsigned int ths_prepare;
    unsigned int ths_zero;
    unsigned int ths_trail;
    unsigned int actual_data_rate = mipi_tx_get_actual_phy_data_rate() / PREPARE_COMPENSATE; /* converted to Mbps */

    ths_prepare = (125 / 2) + (5000 / actual_data_rate); /* 125, 2, 5000 phy timing params after optimization */
    ths_zero = 195 + (10000 / actual_data_rate) - ths_prepare; /* 195, 10000 phy timing params after optimization */
    ths_trail = 61 + (4000 / actual_data_rate); /* 61, 4000 phy timing params after optimization */

    /* CLK_TLPX */
    tp->clk_tlpx = mipi_tx_drv_get_phy_timing_parameters_by_type(TLPX, 0);

    /* CLK_TCLK_PREPARE */
    tp->clk_tclk_prepare = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_PREPARE, 1);

    /* CLK_TCLK_ZERO */
    tp->clk_tclk_zero = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_ZERO, 0) + 1; /* +1: logical param */

    /* CLK_TCLK_TRAIL */
    tp->clk_tclk_trail = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_TRAIL, 0);

    /* DATA_TLPX */
    tp->data_tlpx = mipi_tx_drv_get_phy_timing_parameters_by_type(TLPX, 0);

    /* DATA_TPRE_DELAY */
    tp->data_tpre_delay = tp->data_tlpx + tp->clk_tclk_prepare + tp->clk_tclk_zero +
        mipi_tx_drv_get_phy_timing_parameters_by_type(TPRE_DELAY, 1) + 8; /* +8: logical param */

    /* DATA_THS_PREPARE */
    tp->data_ths_prepare = mipi_tx_drv_get_phy_timing_parameters_by_type(ths_prepare, 1);

    /* DATA_THS_ZERO */
    tp->data_ths_zero = mipi_tx_drv_get_phy_timing_parameters_by_type(ths_zero, 0) + 1; /* +1: logical param */

    /* DATA_THS_TRAIL */
    tp->data_ths_trail = mipi_tx_drv_get_phy_timing_parameters_by_type(ths_trail, 0) - 1; /* -1: logical param */

    /* CLK POST DELAY */
    tp->clk_post_delay = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_POST, 0) +
        tp->data_ths_trail + 10; /* +10: logical param */
}
#endif

#ifdef OT_FPGA

/*
 * set global operation timing parameters.
 */
static void mipi_tx_drv_set_phy_timing_parameters(const mipi_tx_phy_timing_parameters *tp)
{
    /* DATA0~3 TPRE-DELAY */
    set_phy_reg(DATA0_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA1_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA2_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA3_TPRE_DELAY, tp->data_tpre_delay);

    /* CLK_POST_DELAY */
    set_phy_reg(CLK_POST_DELAY, tp->clk_post_delay);

    /* CLK_TLPX */
    set_phy_reg(CLK_TLPX, tp->clk_tlpx);

    /* CLK_TCLK_PREPARE */
    set_phy_reg(CLK_TCLK_PREPARE, tp->clk_tclk_prepare);

    /* CLK_TCLK_ZERO */
    set_phy_reg(CLK_TCLK_ZERO, tp->clk_tclk_zero);

    /* CLK_TCLK_TRAIL */
    set_phy_reg(CLK_TCLK_TRAIL, tp->clk_tclk_trail);

    /*
     * DATA_TLPX
     * DATA_THS_PREPARE
     * DATA_THS_ZERO
     * DATA_THS_TRAIL
     */
    set_phy_reg(DATA0_TLPX, tp->data_tlpx);
    set_phy_reg(DATA0_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA0_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA0_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA1_TLPX, tp->data_tlpx);
    set_phy_reg(DATA1_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA1_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA1_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA2_TLPX, tp->data_tlpx);
    set_phy_reg(DATA2_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA2_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA2_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA3_TLPX, tp->data_tlpx);
    set_phy_reg(DATA3_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA3_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA3_THS_TRAIL, tp->data_ths_trail);

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy timing parameters=======\n");
    osal_printk("data_tpre_delay(0x30/40/50/60): 0x%x\n", tp->data_tpre_delay);
    osal_printk("clk_post_delay(0x21): 0x%x\n", tp->clk_post_delay);
    osal_printk("clk_tlpx(0x22): 0x%x\n", tp->clk_tlpx);
    osal_printk("clk_tclk_prepare(0x23): 0x%x\n", tp->clk_tclk_prepare);
    osal_printk("clk_tclk_zero(0x24): 0x%x\n", tp->clk_tclk_zero);
    osal_printk("clk_tclk_trail(0x25): 0x%x\n", tp->clk_tclk_trail);
    osal_printk("data_tlpx(0x32/42/52/62): 0x%x\n", tp->data_tlpx);
    osal_printk("data_ths_prepare(0x33/43/53/63): 0x%x\n", tp->data_ths_prepare);
    osal_printk("data_ths_zero(0x34/44/54/64): 0x%x\n", tp->data_ths_zero);
    osal_printk("data_ths_trail(0x35/45/55/65): 0x%x\n", tp->data_ths_trail);
    osal_printk("=========================\n");
#endif
}

#else

static void mipi_tx_drv_set_phy_timing_cfg(int offset, const mipi_tx_phy_timing_parameters *tp)
{
    volatile u_lane_reg_10 lane_reg_10;
    volatile u_lane_reg_11 lane_reg_11;
    volatile u_lane_reg_12 lane_reg_12;
    volatile u_lane_reg_13 lane_reg_13;
    volatile u_lane_reg_14 lane_reg_14;
    volatile u_lane_reg_15 lane_reg_15;
    volatile u_lane_reg_16 lane_reg_16;
    unsigned int *addr_reg;
    unsigned char data_tpre_delay;
    unsigned char tlpx;
    unsigned char ths_prepare;
    unsigned char ths_zero;
    unsigned char ths_trail;

    if (offset == 2 * 0x80) { /* clk lane is 2 */
        tlpx = tp->clk_tlpx;
        ths_prepare = tp->clk_tclk_prepare;
        ths_zero = tp->clk_tclk_zero;
        ths_trail = tp->clk_tclk_trail;
    } else {
        data_tpre_delay = tp->data_tpre_delay;
        tlpx = tp->data_tlpx;
        ths_prepare = tp->data_ths_prepare;
        ths_zero = tp->data_ths_zero;
        ths_trail = tp->data_ths_trail;
    }

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_12) + offset);
    lane_reg_12.u32 = hal_read_reg(addr_reg);
    lane_reg_12.bits.reg_tlpx = tlpx;
    hal_write_reg(addr_reg, lane_reg_12.u32);

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_13) + offset);
    lane_reg_13.u32 = hal_read_reg(addr_reg);
    lane_reg_13.bits.reg_tprepare = ths_prepare;
    hal_write_reg(addr_reg, lane_reg_13.u32);

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_14) + offset);
    lane_reg_14.u32 = hal_read_reg(addr_reg);
    lane_reg_14.bits.reg_tzero = ths_zero;
    hal_write_reg(addr_reg, lane_reg_14.u32);

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_15) + offset);
    lane_reg_15.u32 = hal_read_reg(addr_reg);
    lane_reg_15.bits.reg_thstrail = ths_trail;
    hal_write_reg(addr_reg, lane_reg_15.u32);

    if (offset == 2 * 0x80) { /* clk lane is 2 */
        addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_11) + offset);
        lane_reg_11.u32 = hal_read_reg(addr_reg);
        lane_reg_11.bits.reg_pos_req_dy = tp->clk_post_delay;
        hal_write_reg(addr_reg, lane_reg_11.u32);

        addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_16) + offset);
        lane_reg_16.u32 = hal_read_reg(addr_reg);
        lane_reg_16.bits.reg_clock_val = 0x55;
        hal_write_reg(addr_reg, lane_reg_16.u32);
    } else {
        addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_10) + offset);
        lane_reg_10.u32 = hal_read_reg(addr_reg);
        lane_reg_10.bits.reg_pre_req_dy = data_tpre_delay;
        hal_write_reg(addr_reg, lane_reg_10.u32);
    }

    return;
}

static void mipi_tx_drv_set_phy_timing_parameters(const mipi_tx_phy_timing_parameters *tp)
{
    /*
     * DATA_TLPX
     * DATA_THS_PREPARE
     * DATA_THS_ZERO
     * DATA_THS_TRAIL
     * DATA0/DATA1/CLK/DATA2/DATA3
     */
    mipi_tx_drv_set_phy_timing_cfg(0, tp);             /* 0: DATA0 */
    mipi_tx_drv_set_phy_timing_cfg(1 * 0x80, tp);      /* 1: DATA1 */
    mipi_tx_drv_set_phy_timing_cfg(2 * 0x80, tp);      /* 2: CLK */
    mipi_tx_drv_set_phy_timing_cfg(3 * 0x80, tp);      /* 3: DATA2 */
    mipi_tx_drv_set_phy_timing_cfg(4 * 0x80, tp);      /* 4: DATA3 */

#ifdef MIPI_TX_DEBUG
    osal_printk("\n==========phy timing parameters=======\n");
    osal_printk("clk_post_delay: 0x%x\n", tp->clk_post_delay);
    osal_printk("clk_tlpx: 0x%x\n", tp->clk_tlpx);
    osal_printk("clk_tclk_prepare: 0x%x\n", tp->clk_tclk_prepare);
    osal_printk("clk_tclk_zero: 0x%x\n", tp->clk_tclk_zero);
    osal_printk("clk_tclk_trail: 0x%x\n", tp->clk_tclk_trail);
    osal_printk("data_tlpx: 0x%x\n", tp->data_tlpx);
    osal_printk("data_ths_prepare: 0x%x\n", tp->data_ths_prepare);
    osal_printk("data_ths_zero: 0x%x\n", tp->data_ths_zero);
    osal_printk("data_ths_trail: 0x%x\n", tp->data_ths_trail);
    osal_printk("=========================\n");
#endif
}
#endif

#endif /* end of #if mipi_tx_desc("phy time parameter") */

#if mipi_tx_desc("phy cfg")

#ifdef OT_FPGA
static void mipi_tx_drv_set_phy_hs_lp_switch_time(const mipi_tx_phy_timing_parameters *tp)
{
    /* data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = 0x6001D;
    /* clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = 0x26001E;

#ifdef MIPI_TX_DEBUG
    osal_printk("datalane_time(0x9C): 0x%x\n", g_mipi_tx_regs_va->datalane_time.u32);
    osal_printk("clklane_time(0x98): 0x%x\n", g_mipi_tx_regs_va->clklane_time.u32);
#endif
}

#else

/*
 * set data lp2hs,hs2lp time
 * set clk lp2hs,hs2lp time
 * unit: hsclk
 */
static void mipi_tx_drv_set_phy_hs_lp_switch_time(const mipi_tx_phy_timing_parameters *tp)
{
    unsigned char clk_post_delay = tp->clk_post_delay + 1; /* 1: logical add 1 itself */
    unsigned char tlpx = tp->clk_tlpx + 1; /* 1: logical add 1 itself */
    unsigned char clk_tclk_prepare = tp->clk_tclk_prepare + 1; /* 1: logical add 1 itself */
    unsigned char clk_tclk_zero = tp->clk_tclk_zero + 1; /* 1: logical add 1 itself */
    unsigned char data_tpre_delay = tp->data_tpre_delay + 1; /* 1: logical add 1 itself */
    unsigned char data_ths_prepare = tp->data_ths_prepare + 1; /* 1: logical add 1 itself */
    unsigned char data_ths_zero = tp->data_ths_zero + 1; /* 1: logical add 1 itself */
    unsigned char data_ths_trail = tp->data_ths_trail + 1; /* 1: logical add 1 itself */

    /* data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = data_tpre_delay + tlpx + data_ths_prepare + data_ths_zero +
        data_ths_trail * 65536; /* 65536: phy timing params */

    /* clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = tlpx + clk_tclk_prepare + clk_tclk_zero +
        (clk_post_delay + data_ths_trail - 1) * 65536; /* 1, 65536: phy timing params */

#ifdef MIPI_TX_DEBUG
    osal_printk("datalane_time(0x9C): 0x%x\n", g_mipi_tx_regs_va->datalane_time.u32);
    osal_printk("clklane_time(0x98): 0x%x\n", g_mipi_tx_regs_va->clklane_time.u32);
#endif
}
#endif


static void mipi_tx_drv_set_phy_default_setting(void)
{
    g_mipi_tx_phy_regs_va->aphy_test_atop_1.u32 = 0x3000080; /* 0x10 TEST_ATOP_H LP */
    g_mipi_tx_phy_regs_va->common_reg_52.u32 = 0xfc; /* 0xd48 clk lane pattern */
    g_mipi_tx_phy_regs_va->common_reg_53.u32 = 0x8; /* 0xd4c ser_psave */
    g_mipi_tx_phy_regs_va->common_reg_56.u32 = 0x7; /* 0xd58 lprxcd en */
    g_mipi_tx_phy_regs_va->common_reg_57.u32 = 0x0; /* 0xd5c cphy pre de drv */
    g_mipi_tx_phy_regs_va->aphy_bg_cfg.u32 = 0x3; /* 0x1c phy gate clk */

#ifdef MIPI_TX_DEBUG
    osal_printk("atop h lp (0x10): 0x%x\n", g_mipi_tx_phy_regs_va->aphy_test_atop_1.u32);
    osal_printk("clk lane pattern (0xd48): 0x%x\n", g_mipi_tx_phy_regs_va->common_reg_52.u32);
    osal_printk("ser psave (0xd4c): 0x%x\n", g_mipi_tx_phy_regs_va->common_reg_53.u32);
    osal_printk("lp rx cd en (0xd58): 0x%x\n", g_mipi_tx_phy_regs_va->common_reg_56.u32);
    osal_printk("cphy pre de drv(0xd5c): 0x%x\n", g_mipi_tx_phy_regs_va->common_reg_57.u32);
#endif
}


static void mipi_tx_drv_set_input_enable(unsigned char input_en)
{
    volatile reg_operation_mode operation_mode = {0};

    operation_mode.u32 = g_mipi_tx_regs_va->operation_mode.u32;
    if (input_en == 0) {
        operation_mode.u32 = 0;
        g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;
        return;
    }

    operation_mode.bits.mem_ck_en = 1;
    operation_mode.bits.input_en = 1;
    operation_mode.bits.hss_abnormal_rst = 1;
    operation_mode.bits.read_empty_vsync_en = 1;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;
}

void mipi_tx_drv_set_phy_cfg(void)
{
    unsigned int actual_data_rate = mipi_tx_get_actual_phy_data_rate();
    mipi_tx_phy_timing_parameters tp = {0};

    /*
     * reset phy before setting phy:
     * forcepll: pll enable
     * enableclk: enable clk lane
     * rstz: unreset
     * shutdownz: powerup
     */
    g_mipi_tx_regs_va->phy_ctrl.u32 = 0x0;
    osal_udelay(1000); /* 1000: 1000us */

    /* disable input, done in mipi_tx_drv_set_controller_cfg */
    mipi_tx_drv_set_input_enable(0);

    /* pwr_up unreset */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    osal_udelay(1);
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;

    /* set phy pll parameters setx to generate data rate */
    mipi_tx_drv_set_phy_pll_setx(actual_data_rate);

    /* get global operation timing parameters */
    mipi_tx_drv_get_phy_timing_parameters(&tp);

    /* set global operation timing parameters */
    mipi_tx_drv_set_phy_timing_parameters(&tp);

    /*
     * dsi controller
     * set hs switch to lp and lp switch to hs time
     */
    mipi_tx_drv_set_phy_hs_lp_switch_time(&tp);

    /* mipitx clk default */
    mipi_tx_drv_set_phy_default_setting();

    /*
     * unreset phy after setting phy:
     * forcepll: pll enable
     * enableclk: enable clk lane
     * rstz: unreset
     * shutdownz: powerup
     */

    /* pwr_up unreset */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = 0xF;
    osal_udelay(200); /* 200: 200us */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;
    osal_msleep(1);
}

#endif /* #if mipi_tx_desc("phy cfg") */

#if mipi_tx_desc("controller cfg")

static void set_output_format(const combo_dev_cfg_t *dev_cfg)
{
    int color_coding = 0;

    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_NORMAL) {
            color_coding = MIPI_TX_CSI_YUV420_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_LEGACY) {
            color_coding = MIPI_TX_CSI_LEGACY_YUV420_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_8BIT) {
            color_coding = MIPI_TX_CSI_YUV422_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_888) {
            color_coding = MIPI_TX_CSI_RGB888;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_8BIT) {
            color_coding = MIPI_TX_CSI_RAW8;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_10BIT) {
            color_coding = MIPI_TX_CSI_RAW10;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_12BIT) {
            color_coding = MIPI_TX_CSI_RAW12;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_14BIT) {
            color_coding = MIPI_TX_CSI_RAW14;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_16BIT) {
            color_coding = MIPI_TX_CSI_RAW16;
        }
    } else {
        if (dev_cfg->out_format == OUT_FORMAT_RGB_16BIT) {
            color_coding = MIPI_TX_DSI_RGB_16BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_18BIT) {
            color_coding = MIPI_TX_DSI_RGB_18BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_18BIT_LOOSELY) {
            color_coding = MIPI_TX_DSI_RGB_18BIT_LOOSELY;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_24BIT) {
            color_coding = MIPI_TX_DSI_RGB_24BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_12BIT) {
            color_coding = MIPI_TX_DSI_YCBCR420_12BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_16BIT) {
            color_coding = MIPI_TX_DSI_YCBCR422_16BIT;
        }
    }

    g_mipi_tx_regs_va->data_type.u32 = color_coding;
#ifdef MIPI_TX_DEBUG
    osal_printk("set_output_format: 0x%x\n", color_coding);
#endif
}

static void mipi_tx_hal_set_cmd_video_mode(cmd_video_mode cmd_video)
{
    volatile reg_mode_cfg mode_cfg;
    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.cmd_video_mode = cmd_video;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void mipi_tx_hal_set_video_mode_type(unsigned int video_mode_type)
{
    volatile reg_mode_cfg mode_cfg;

    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.video_mode_type = video_mode_type;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void set_video_mode_cfg(const combo_dev_cfg_t *dev_cfg)
{
    int video_mode_type;
    volatile reg_read_memory_delay_ctrl read_memory_delay_ctrl;

    if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) {
        video_mode_type = 0; /* 0: Non-burst with sync pulse */
    } else if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS) {
        video_mode_type = 1; /* 1: Non-burst with sync */
    } else {
        video_mode_type = 2; /* 2: burst mode */
    }

    if ((dev_cfg->out_mode == OUT_MODE_CSI) ||
        (dev_cfg->out_mode == OUT_MODE_DSI_CMD)) {
        video_mode_type = 2; /* 2: burst mode */
    }

    /*
     * mode config
     * other: burst mode
     * 00: Non-burst with sync pulse
     * 01: Non-burst with sync
     */
    mipi_tx_hal_set_cmd_video_mode(COMMAND_MODE);
    mipi_tx_hal_set_video_mode_type(video_mode_type);

    g_mipi_tx_regs_va->video_lp_en.u32 = 0x3f;

    /* delay value setting. */
    read_memory_delay_ctrl.u32 = g_mipi_tx_regs_va->read_memory_delay_ctrl.u32;
    if ((dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) ||
        (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS)) {
        read_memory_delay_ctrl.bits.delay_abnormal = 0x1;
    } else if (dev_cfg->video_mode == BURST_MODE) {
        read_memory_delay_ctrl.bits.delay_abnormal = 0x0;
    }
    g_mipi_tx_regs_va->read_memory_delay_ctrl.u32 = read_memory_delay_ctrl.u32;

    if ((dev_cfg->out_mode == OUT_MODE_DSI_VIDEO) ||
        (dev_cfg->out_mode == OUT_MODE_CSI)) {
        g_mipi_tx_regs_va->videom_pkt_size.u32 = dev_cfg->sync_info.hact;
    } else {
        g_mipi_tx_regs_va->command_pkt_size.u32 = dev_cfg->sync_info.hact;
    }

    /* num_chunks/null_size */
    g_mipi_tx_regs_va->videom_num_chunks.u32 = 0x0;
    g_mipi_tx_regs_va->videom_null_size.u32 = 0x0;
}

static void set_timing_config(const combo_dev_cfg_t *dev_cfg)
{
    unsigned int hsa_time;
    unsigned int hbp_time;
    unsigned int hline_time;
    unsigned int actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();
    const unsigned int htime_tmp = 1000 / 8; /* 1000, 8: 1000Hz / 8bit */

    if (dev_cfg->pixel_clk == 0) {
        osal_printk("dev_cfg->pixel_clk is 0, illegal.\n");
        return;
    }

    actual_phy_data_rate = actual_phy_data_rate / 10; /* 10: actual_phy_data_rate unit trans to MHZ */

    hsa_time = actual_phy_data_rate * dev_cfg->sync_info.hpw * htime_tmp / dev_cfg->pixel_clk;
    hbp_time = actual_phy_data_rate * dev_cfg->sync_info.hbp  * htime_tmp / dev_cfg->pixel_clk;
    hline_time = actual_phy_data_rate * (dev_cfg->sync_info.hpw +
        dev_cfg->sync_info.hact +
        dev_cfg->sync_info.hbp +
        dev_cfg->sync_info.hfp) * htime_tmp / dev_cfg->pixel_clk;

    g_mipi_tx_regs_va->videom_hsa_size.u32 = hsa_time;
    g_mipi_tx_regs_va->videom_hbp_size.u32 = hbp_time;
    g_mipi_tx_regs_va->videom_hline_size.u32 = hline_time;

    g_mipi_tx_regs_va->videom_vsa_lines.u32 = dev_cfg->sync_info.vpw;
    g_mipi_tx_regs_va->videom_vbp_lines.u32 = dev_cfg->sync_info.vbp;
    g_mipi_tx_regs_va->videom_vfp_lines.u32 = dev_cfg->sync_info.vfp;
    g_mipi_tx_regs_va->videom_vactive_lines.u32 = dev_cfg->sync_info.vact;

#ifdef MIPI_TX_DEBUG
    osal_printk("hpw(0x48): 0x%x\n", dev_cfg->sync_info.hpw);
    osal_printk("hbp(0x4c): 0x%x\n", dev_cfg->sync_info.hbp);
    osal_printk("hact(0x50): 0x%x\n", dev_cfg->sync_info.hact);
    osal_printk("hfp(0x50): 0x%x\n", dev_cfg->sync_info.hfp);
    osal_printk("vact(0x54): 0x%x\n", dev_cfg->sync_info.vact);
    osal_printk("vbp(0x58): 0x%x\n", dev_cfg->sync_info.vbp);
    osal_printk("vfp(0x5c): 0x%x\n", dev_cfg->sync_info.vfp);
    osal_printk("vpw(0x60): 0x%x\n", dev_cfg->sync_info.vpw);
#endif
}

static void mipi_tx_set_lane_id(int lane_idx, short lane_id)
{
    reg_lane_id lane_id_reg;
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;

    switch (lane_id) {
        case 0:
            lane_id_reg.bits.lane0_id = lane_idx;
            break;

        case 1:
            lane_id_reg.bits.lane1_id = lane_idx;
            break;

        case 2: /* 2: lane2 */
            lane_id_reg.bits.lane2_id = lane_idx;
            break;

        case 3: /* 3: lane3 */
            lane_id_reg.bits.lane3_id = lane_idx;
            break;

        default:
            break;
    }
    g_mipi_tx_regs_va->lane_id.u32 = lane_id_reg.u32;
}

static void mipi_tx_set_disable_lane_id(int lane_idx, short invalid_lane_num)
{
    reg_lane_id lane_id_reg;
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;

    switch (invalid_lane_num) {
        case 1:
            lane_id_reg.bits.lane3_id = lane_idx;
            break;

        case 2: /* 2: 2lanes */
            lane_id_reg.bits.lane2_id = lane_idx;
            break;

        case 3: /* 3: 3lanes */
            lane_id_reg.bits.lane1_id = lane_idx;
            break;

        default:
            break;
    }

    g_mipi_tx_regs_va->lane_id.u32 = lane_id_reg.u32;
}

static void set_lane_config(const short lane_id[], int lane_id_len)
{
    const int max_lane_num = LANE_MAX_NUM;
    int i;
    int invalid_lane_num = 0;
    const int unused_id = 3; /* 3, for unused lane */
#ifdef MIPI_TX_DEBUG
    reg_lane_id lane_id_reg;
#endif

    g_mipi_tx_regs_va->lane_num.u32 = mipi_tx_get_lane_num(lane_id, lane_id_len) - 1;

    for (i = 0; i < max_lane_num; i++) {
        if (lane_id[i] == MIPI_TX_DISABLE_LANE_ID) {
            invalid_lane_num++;
            mipi_tx_set_disable_lane_id(unused_id, invalid_lane_num);
            continue;
        }
        mipi_tx_set_lane_id(i, lane_id[i]);
    }

#ifdef MIPI_TX_DEBUG
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;
    osal_printk("LANE_ID: 0x%x\n", lane_id_reg.u32);
#endif
}

static void mipi_tx_hal_set_request_hsclk_en(unsigned char clk_en)
{
    volatile reg_clklane_ctrl clk_lane_ctrl;

    clk_lane_ctrl.u32 = g_mipi_tx_regs_va->clklane_ctrl.u32;
    clk_lane_ctrl.bits.txrequesthsclk = clk_en;
    g_mipi_tx_regs_va->clklane_ctrl.u32 = clk_lane_ctrl.u32;
}

static void mipi_tx_hal_set_clklane_continue_en(unsigned char clk_en)
{
    volatile reg_clklane_ctrl clk_lane_ctrl;

    clk_lane_ctrl.u32 = g_mipi_tx_regs_va->clklane_ctrl.u32;
    clk_lane_ctrl.bits.clklane_continue = clk_en;
    g_mipi_tx_regs_va->clklane_ctrl.u32 = clk_lane_ctrl.u32;
}

static void mipi_tx_drv_phy_reset(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

static void mipi_tx_drv_phy_unreset(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x1;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

void mipi_tx_drv_phy_power_down(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.shutdownz = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

static void mipi_tx_drv_set_clkmgr_cfg(void)
{
    unsigned int actual_phy_data_rate;

    actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate() / 10; /* 10: actual_phy_data_rate unit trans to MHZ */
    if (actual_phy_data_rate / 160 < 2) { /* 160: half of 320, 2: half */
        g_mipi_tx_regs_va->crg_cfg.u32 = 0x102; /* 258 */
    } else {
        g_mipi_tx_regs_va->crg_cfg.u32 = 0x100 +
            (actual_phy_data_rate + 160 - 1) / 160; /* align to 160 */
    }
#ifdef MIPI_TX_DEBUG
    osal_printk("crg_cfg: 0x%x\n", g_mipi_tx_regs_va->crg_cfg.u32);
#endif
}

static void mipi_tx_drv_set_controller_cmd_tran_mode(command_trans_mode trans_mode)
{
    volatile reg_command_tran_mode command_tran_mode;

    command_tran_mode.u32 = g_mipi_tx_regs_va->command_tran_mode.u32;
    command_tran_mode.bits.max_rd_pkt_size_tran = trans_mode;
    command_tran_mode.bits.gen_sw_0p = trans_mode;
    command_tran_mode.bits.gen_sw_1p = trans_mode;
    command_tran_mode.bits.gen_sw_2p = trans_mode;
    command_tran_mode.bits.gen_sr_0p = trans_mode;
    command_tran_mode.bits.gen_sr_1p = trans_mode;
    command_tran_mode.bits.gen_sr_2p = trans_mode;
    command_tran_mode.bits.gen_lw = trans_mode;
    command_tran_mode.bits.dcs_sw_0p = trans_mode;
    command_tran_mode.bits.dcs_sw_1p = trans_mode;
    command_tran_mode.bits.dcs_sr_0p = trans_mode;
    command_tran_mode.bits.dcs_lw = trans_mode;
    g_mipi_tx_regs_va->command_tran_mode.u32 = command_tran_mode.u32;
}

static void mipi_tx_drv_set_controller_video_tran_mode(command_trans_mode trans_mode)
{
    volatile reg_mode_cfg mode_cfg;

    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.cmd_mode_tran_type = trans_mode;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void mipi_tx_drv_set_controller_cmd_datatype(const combo_dev_cfg_t *dev_cfg)
{
    volatile reg_datatype0 datatype0;
    unsigned int dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_16BIT;
    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_NORMAL) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_NORMAL;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_8BIT_LEGACY) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_YUV420_8BIT_LEGACY;
        } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_8BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_YUV422_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RGB_888) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RBG_888;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_8BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_8BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_10BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_10BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_12BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_12BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_14BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_14BIT;
        } else if (dev_cfg->out_format == OUT_FORMAT_RAW_16BIT) {
            dt_pixel = MIPI_TX_CSI_DATA_TYPE_RAW_16BIT;
        }
        datatype0.u32 = g_mipi_tx_regs_va->datatype0.u32;
        datatype0.bits.dt_pixel = dt_pixel;
        datatype0.bits.dt_hss = MIPI_TX_CSI_DATA_TYPE_LINE_START;
        datatype0.bits.dt_vse = MIPI_TX_CSI_DATA_TYPE_FRAME_END;
        datatype0.bits.dt_vss = MIPI_TX_CSI_DATA_TYPE_FRAME_START;
        g_mipi_tx_regs_va->datatype0.u32 = datatype0.u32;
        g_mipi_tx_regs_va->datatype1.u32 = 0x31081909;
        g_mipi_tx_regs_va->csi_ctrl.u32 = 0x1111;
    } else {
        g_mipi_tx_regs_va->datatype0.u32 = 0x111213D;
        g_mipi_tx_regs_va->datatype1.u32 = 0x31081909;
        g_mipi_tx_regs_va->csi_ctrl.u32 = 0x10100;
    }
}

static void mipi_tx_cal_actual_phy_data_rate(unsigned int input_data_rate)
{
    unsigned int data_rate;
    unsigned int actual_data_rate;
    unsigned int ratio_data_rate;

    data_rate = input_data_rate * 110 / 100; /* 110, 100: data_rate * 1.1 for datatype, ECC, Checksum, lp~hs turn */

    data_rate = data_rate * 10; /* data_rate unit is MHZ*10 */
    actual_data_rate = mipi_tx_align_up(data_rate, MIPI_TX_REF_CLK);
    if (actual_data_rate > (MIPI_TX_MAX_PHY_DATA_RATE * 10)) { /* data_rate unit is MHZ * 10 */
        ratio_data_rate = (MIPI_TX_MAX_PHY_DATA_RATE * 10) / MIPI_TX_REF_CLK; /* data_rate unit is MHZ * 10 */
        actual_data_rate = ratio_data_rate * MIPI_TX_REF_CLK;
    }

    mipi_tx_set_actual_phy_data_rate(actual_data_rate);
}

void mipi_tx_drv_set_ctrl_clk(unsigned int enable)
{
    unsigned long mipi_tx_crg_cfg1_addr;

    mipi_tx_crg_cfg1_addr = (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG, (unsigned long)0x4);
    /* mipi_tx gate clk */
    write_reg32(mipi_tx_crg_cfg1_addr, enable, 0x1); /* bit 1 */
    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_cfg1_addr, (unsigned long)0x4);
}

void mipi_tx_drv_set_controller_cfg(const combo_dev_cfg_t *dev_cfg)
{
    mipi_tx_drv_set_ctrl_clk(1);

    /* set 1st level int mask */
    g_mipi_tx_regs_va->gint_msk = 0x1ffff;

    /* OPERATION_MODE, disable input */
    mipi_tx_drv_set_input_enable(0);

    /* vc_id */
    g_mipi_tx_regs_va->video_vc.u32 = 0x0;

    /* output format */
    set_output_format(dev_cfg);

    /* txescclk,timeout */
    mipi_tx_cal_actual_phy_data_rate(dev_cfg->phy_data_rate);
    mipi_tx_drv_set_clkmgr_cfg();

    /* cmd transmission mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(LOW_POWER_MODE);

    /* crc,ecc,eotp tran */
    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        g_mipi_tx_regs_va->pck_en.u32 = 0x0C; /* csi */
    } else {
        g_mipi_tx_regs_va->pck_en.u32 = 0x1E; /* dsi */
    }
    /* gen_vcid_rx */
    g_mipi_tx_regs_va->gen_vc.u32 = 0x0;

    /* video mode cfg */
    set_video_mode_cfg(dev_cfg);

    /* timing config */
    set_timing_config(dev_cfg);

    /* invact,outvact time */
    g_mipi_tx_regs_va->lp_cmd_byte.u32 = 0xff0000;

    if (dev_cfg->out_mode == OUT_MODE_CSI) {
        g_mipi_tx_regs_va->command_pkt_size.u32 = 0;
        g_mipi_tx_regs_va->hsrd_to_set.u32 = 0x0;
    }
    g_mipi_tx_regs_va->hs_lp_to_set.u32 = 0x0;
    g_mipi_tx_regs_va->lprd_to_set.u32 = 0x0;
    g_mipi_tx_regs_va->hswr_to_set.u32 = 0x0;
    /* lp_wr_to_cnt */
    g_mipi_tx_regs_va->lpwr_to_set.u32 = 0x0;
    /* bta_to_cnt */
    g_mipi_tx_regs_va->bta_to_set.u32 = 0x0;
    /* read cmd hsclk */
    g_mipi_tx_regs_va->read_cmd_time.u32 = 0x7fff; /* 0x7fff hsclk */

    /* set data type */
    mipi_tx_drv_set_controller_cmd_datatype(dev_cfg);

    /* lanes num and id */
    set_lane_config(dev_cfg->lane_id, LANE_MAX_NUM);

    /* phy_txrequlpsclk */
    g_mipi_tx_regs_va->ulps_ctrl.u32 = 0x0;

    /* tx triggers */
    g_mipi_tx_regs_va->tx_triggers.u32 = 0x0;

    g_mipi_tx_regs_va->vid_shadow_ctrl.u32 = 0x0;

    /* int0 mask disable */
    g_mipi_tx_regs_va->int0_mask.u32 = 0x0;

    /* clk continue set */
    mipi_tx_hal_set_clklane_continue_en(dev_cfg->clklane_continue_mode);
}

#endif /* #if mipi_tx_desc("controller cfg") */

#if mipi_tx_desc("set cmd")

static int mipi_tx_wait_cmd_fifo_empty(void)
{
    reg_command_status cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;

        wait_count++;

        osal_udelay(1);

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when send cmd buffer \n");
            return -1;
        }
    } while (cmd_pkt_status.bits.command_empty == 0);

    return 0;
}

static int mipi_tx_wait_write_fifo_empty(void)
{
    reg_command_status cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;

        wait_count++;

        osal_udelay(1);

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when send data buffer \n");
            return -1;
        }
    } while (cmd_pkt_status.bits.pld_write_empty == 0);

    return 0;
}

static int mipi_tx_wait_write_fifo_not_full(void)
{
    reg_command_status cmd_pkt_status;
    unsigned int wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;
        if (wait_count > 0) {
            osal_udelay(1);
            mipi_tx_err("write fifo full happened wait count = %u\n", wait_count);
        }
        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when wait write fifo not full buffer \n");
            return -1;
        }
        wait_count++;
    } while (cmd_pkt_status.bits.pld_write_full == 1);

    return 0;
}

/*
 * set payloads data by writing register
 * each 4 bytes in cmd corresponds to one register
 */
static void mipi_tx_drv_set_payload_data(const unsigned char *cmd, unsigned short cmd_size)
{
    reg_command_payload gen_pld_data;
    int i, j;

    gen_pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;

    for (i = 0; i < (cmd_size / MIPI_TX_BYTES_PER_WORD); i++) {
        gen_pld_data.bits.command_pld_b1 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1];
        gen_pld_data.bits.command_pld_b2 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2];
        gen_pld_data.bits.command_pld_b3 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3];
        gen_pld_data.bits.command_pld_b4 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE4];

        mipi_tx_wait_write_fifo_not_full();
        g_mipi_tx_regs_va->command_payload.u32 = gen_pld_data.u32;
    }

    j = cmd_size % MIPI_TX_BYTES_PER_WORD;
    if (j != 0) {
        if (j > 0) {
            gen_pld_data.bits.command_pld_b1 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1];
        }
        if (j > 1) {
            gen_pld_data.bits.command_pld_b2 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2];
        }
        if (j > 2) { /* 2 bytes */
            gen_pld_data.bits.command_pld_b3 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3];
        }

        mipi_tx_wait_write_fifo_not_full();
        g_mipi_tx_regs_va->command_payload.u32 = gen_pld_data.u32;
    }

#ifdef MIPI_TX_DEBUG
        osal_printk("\n=====set cmd=======\n");
        osal_printk("gen_pld_data(0x70): 0x%x\n", gen_pld_data.u32);
#endif

    return;
}

static void mipi_tx_drv_set_cmd_work_param(mipi_tx_work_mode_t work_mode, unsigned char lp_clk_en)
{
    command_trans_mode trans_mode[MIPI_TX_WORK_MODE_BUTT] = {LOW_POWER_MODE, HIGH_SPEED_MODE};

    /* set cmd tran work mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(trans_mode[work_mode]);

    /* set video tran work mode */
    mipi_tx_drv_set_controller_video_tran_mode(trans_mode[work_mode]);

    /* set lp clk en */
    mipi_tx_hal_set_request_hsclk_en(lp_clk_en);
}

static void mipi_tx_drv_set_command_header(unsigned short data_type, unsigned short data_param)
{
    volatile reg_command_header gen_hdr;

    gen_hdr.u32 = g_mipi_tx_regs_va->command_header.u32;
    gen_hdr.bits.command_virtualchannel = 0; /* always 0 */
    gen_hdr.bits.command_datatype = data_type;
    gen_hdr.bits.command_wordcount_low = get_low_byte(data_param);
    gen_hdr.bits.command_wordcount_high = get_high_byte(data_param);
    g_mipi_tx_regs_va->command_header.u32 = gen_hdr.u32;
#ifdef MIPI_TX_DEBUG
    osal_printk("\n=====set cmd=======\n");
    osal_printk("gen_hdr(0x6C): 0x%x\n", gen_hdr.u32);
#endif
}

int mipi_tx_drv_set_cmd_info(const cmd_info_t *cmd_info)
{
    unsigned char *cmd = NULL;

    /* set work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(cmd_info->work_mode, cmd_info->lp_clk_en);

    if (cmd_info->cmd != NULL) {
        cmd = (unsigned char *)osal_kmalloc(cmd_info->cmd_size, OSAL_GFP_KERNEL);
        if (cmd == NULL) {
            mipi_tx_err("kmalloc fail,please check,need %d bytes\n", cmd_info->cmd_size);
            return  -1;
        }

        if (osal_copy_from_user(cmd, cmd_info->cmd, cmd_info->cmd_size)) {
            osal_kfree(cmd);
            cmd = NULL;
            return  -1;
        }

        mipi_tx_drv_set_payload_data(cmd, cmd_info->cmd_size);

        osal_kfree(cmd);
        cmd = NULL;
    }

    mipi_tx_drv_set_command_header(cmd_info->data_type, cmd_info->cmd_size);

    /* wait transfer end */
    osal_udelay(350); /* 350us */

    mipi_tx_wait_cmd_fifo_empty();
    mipi_tx_wait_write_fifo_empty();

#ifdef MIPI_TX_DEBUG
    osal_printk("cmd_info->data_type: 0x%x\n", cmd_info->data_type);
    osal_printk("cmd_info->cmd_size: 0x%x\n", cmd_info->cmd_size);
#endif

    /* resume work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(mipi_tx_get_work_mode(), mipi_tx_get_lp_clk_en());

    return 0;
}

#endif /* #if mipi_tx_desc("set cmd") */

#if mipi_tx_desc("get cmd")

static int mipi_tx_wait_read_fifo_not_empty(void)
{
    reg_int0_status int_st0;
    reg_int1_status int_st1;
    unsigned int wait_count;
    reg_command_status cmd_pkt_status;

    wait_count = 0;
    do {
        int_st1.u32 =  g_mipi_tx_regs_va->int1_status.u32;
        int_st0.u32 =  g_mipi_tx_regs_va->int0_status.u32;

        /*
         * 0x0000101e:
         * to_lp_rx, rxecc_multi_err, rxecc_single_err, rxcrc_err, rxpkt_size_err
         */
        if ((int_st1.u32 & 0x0000101e) != 0) {
            mipi_tx_err("err happened when read data, int_st1 = 0x%x,int_st0 = %x\n", int_st1.u32, int_st0.u32);
            return -1;
        }

        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when read data\n");
            return -1;
        }

        wait_count++;

        osal_udelay(1);

        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;
    } while (cmd_pkt_status.bits.pld_read_empty == 0x1);

    return 0;
}

static int mipi_tx_wait_read_fifo_empty(void)
{
    reg_command_payload pld_data;
    reg_int1_status int_st1;
    unsigned int wait_count;

    wait_count = 0;
    do {
        int_st1.u32 = g_mipi_tx_regs_va->int1_status.u32;
        if ((int_st1.bits.rxpld_rd_err) == 0x0) {
            pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        }
        wait_count++;
        osal_udelay(1);
        if (wait_count >  MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when clear cmd data buffer, the last read data is 0x%x\n", pld_data.u32);
            return -1;
        }
    } while ((int_st1.bits.rxpld_rd_err) == 0x0);

    return 0;
}

static int mipi_tx_send_short_packet(short unsigned data_type, unsigned short data_param)
{
    mipi_tx_drv_set_command_header(data_type, data_param);

    if (mipi_tx_wait_cmd_fifo_empty() != 0) {
        return -1;
    }

    return 0;
}

static int mipi_tx_get_read_fifo_data(unsigned int get_data_size, unsigned char *data_buf)
{
    reg_command_payload pld_data;
    unsigned int i, j;

    for (i = 0; i < get_data_size / MIPI_TX_BYTES_PER_WORD; i++) {
        if (mipi_tx_wait_read_fifo_not_empty() != 0) {
            return -1;
        }
        pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1] = pld_data.bits.command_pld_b1;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2] = pld_data.bits.command_pld_b2;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3] = pld_data.bits.command_pld_b3;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE4] = pld_data.bits.command_pld_b4;
    }

    j = get_data_size % MIPI_TX_BYTES_PER_WORD;

    if (j != 0) {
        if (mipi_tx_wait_read_fifo_not_empty() != 0) {
            return -1;
        }
        pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        if (j > 0) {
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1] = pld_data.bits.command_pld_b1;
        }
        if (j > 1) {
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2] = pld_data.bits.command_pld_b2;
        }
        if (j > 2) { /* 2 bytes */
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3] = pld_data.bits.command_pld_b3;
        }
    }

    return 0;
}

static void mipi_tx_reset(void)
{
    volatile reg_phy_ctrl phy_ctrl = {0};
    volatile reg_ctrl_reset ctrl_reset = {0};

    ctrl_reset.u32 = g_mipi_tx_regs_va->ctrl_reset.u32;
    ctrl_reset.bits.ctrl_reset = 0x0;
    g_mipi_tx_regs_va->ctrl_reset.u32 = ctrl_reset.u32;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
    osal_udelay(1);

    ctrl_reset.u32 = g_mipi_tx_regs_va->ctrl_reset.u32;
    ctrl_reset.bits.ctrl_reset = 0x1;
    g_mipi_tx_regs_va->ctrl_reset.u32 = ctrl_reset.u32;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x1;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
    osal_udelay(1);
    return;
}

int mipi_tx_drv_get_cmd_info(const get_cmd_info_t *get_cmd_info)
{
    unsigned char* data_buf = NULL;

    data_buf = (unsigned char*)osal_kmalloc(get_cmd_info->get_data_size, OSAL_GFP_KERNEL);
    if (data_buf == NULL) {
        return -1;
    }

    if (mipi_tx_wait_read_fifo_empty() != 0) {
        goto fail0;
    }

    /* set work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(get_cmd_info->work_mode, get_cmd_info->lp_clk_en);

    if (mipi_tx_send_short_packet(get_cmd_info->data_type, get_cmd_info->data_param) != 0) {
        goto fail0;
    }

    if (mipi_tx_get_read_fifo_data(get_cmd_info->get_data_size, data_buf) != 0) {
        /* fail will block mipi data lane ,so need reset */
        mipi_tx_reset();
        goto fail0;
    }

    /* resume work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(mipi_tx_get_work_mode(), mipi_tx_get_lp_clk_en());

    osal_copy_to_user(get_cmd_info->get_data, data_buf, get_cmd_info->get_data_size);

    osal_kfree(data_buf);
    data_buf = NULL;
    return 0;

fail0:
    osal_kfree(data_buf);
    data_buf = NULL;
    return -1;
}

#endif /* #if mipi_tx_desc("get cmd") */

#if mipi_tx_desc("set pn swap")

static void mipi_tx_hal_set_pn_swap(int offset, char swap)
{
    volatile u_lane_reg_06 lane_reg_06;
    unsigned int *addr_reg;

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_06) + offset);
    lane_reg_06.u32 = hal_read_reg(addr_reg);
    lane_reg_06.bits.reg_wire_swap = swap;
    hal_write_reg(addr_reg, lane_reg_06.u32);
}

void mipi_tx_drv_set_pn_swap(const combo_dev_pn_swap_t *pn_swap)
{
    /*
     * PN SWAP
     * DATA0/DATA1/DATA2/DATA3
     */
    mipi_tx_hal_set_pn_swap(0, pn_swap->pn_swap[0]);             /* 0: DATA0 */
    mipi_tx_hal_set_pn_swap(1 * 0x80, pn_swap->pn_swap[1]);      /* 1: DATA1 */
    mipi_tx_hal_set_pn_swap(3 * 0x80, pn_swap->pn_swap[2]);      /* 3: DATA2 */
    mipi_tx_hal_set_pn_swap(4 * 0x80, pn_swap->pn_swap[3]);      /* 4: DATA3 */
}

static char mipi_tx_hal_get_pn_swap(int offset)
{
    volatile u_lane_reg_06 lane_reg_06;
    unsigned int *addr_reg;
    char pn_sw_value;

    addr_reg = (unsigned int *)((uintptr_t)(void *)&(g_mipi_tx_phy_regs_va->lane_reg_06) + offset);
    lane_reg_06.u32 = hal_read_reg(addr_reg);
    pn_sw_value = lane_reg_06.bits.reg_wire_swap;
    return pn_sw_value;
}

void mipi_tx_drv_get_pn_swap(combo_dev_pn_swap_t *pn_swap)
{
    pn_swap->pn_swap[0] = mipi_tx_hal_get_pn_swap(0);             /* 0: DATA lane0 */
    pn_swap->pn_swap[1] = mipi_tx_hal_get_pn_swap(1 * 0x80);      /* 1: DATA lane1 */
    pn_swap->pn_swap[2] = mipi_tx_hal_get_pn_swap(3 * 0x80);      /* 3: DATA lane2 */
    pn_swap->pn_swap[3] = mipi_tx_hal_get_pn_swap(4 * 0x80);      /* 4: DATA lane3 */
}

#endif /* #if mipi_tx_desc("set pn swap") */


#if mipi_tx_desc("enable & disable")

void mipi_tx_drv_enable_input(const out_mode_t output_mode)
{
    if ((output_mode == OUT_MODE_DSI_VIDEO) ||
        (output_mode == OUT_MODE_CSI)) {
        mipi_tx_hal_set_cmd_video_mode(VIDEO_MODE);
    }

    /* set to high speed mode */
    if (output_mode == OUT_MODE_DSI_CMD) {
        mipi_tx_drv_set_controller_cmd_tran_mode(HIGH_SPEED_MODE);
    }

    osal_udelay(1);
    mipi_tx_hal_set_request_hsclk_en(1);
    osal_udelay(1);

    /* enable input */
    mipi_tx_drv_set_input_enable(1);

    /* reset mipi_tx controller */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    /* reset phy digital module by controller */
    mipi_tx_drv_phy_reset();
    osal_udelay(1);
    mipi_tx_drv_phy_unreset();
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;

#if MIPI_TX_INTERRUPTS_ENABLE
    /* enable int */
    mipi_tx_enable_int(0x1);
#endif
}

void mipi_tx_drv_reset_pn_swap(void)
{
    combo_dev_pn_swap_t tx_pn_swap = {0};

    tx_pn_swap.pn_swap[0] = 0;          /* 0: DATA0 */
    tx_pn_swap.pn_swap[1] = 0;          /* 1: DATA1 */
    tx_pn_swap.pn_swap[2] = 0;          /* 3: DATA2 */
    tx_pn_swap.pn_swap[3] = 0;          /* 4: DATA3 */
    mipi_tx_drv_set_pn_swap(&tx_pn_swap);
}

void mipi_tx_drv_disable_input(void)
{
    volatile reg_operation_mode operation_mode;

    /* disable int */
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_enable_int(0x0);
#endif

    /* set to disable input */
    operation_mode.u32 = g_mipi_tx_regs_va->operation_mode.u32;
    operation_mode.bits.input_en = 0x0;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;

    /* set to lp mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(LOW_POWER_MODE);

    /* set to command mode */
    mipi_tx_hal_set_cmd_video_mode(COMMAND_MODE);

    /* default: clklane disable */
    mipi_tx_hal_set_request_hsclk_en(0);
    mipi_tx_hal_set_clklane_continue_en(0);

    mipi_tx_drv_phy_power_down();
    mipi_tx_reset();
}

#endif /* #if mipi_tx_desc("enable & disable") */

#if mipi_tx_desc("init")

static int mipi_tx_drv_reg_init(void)
{
    if (g_mipi_tx_regs_va == NULL) {
        g_mipi_tx_regs_va = (volatile mipi_tx_regs_type_t *)osal_ioremap(
            MIPI_TX_REGS_ADDR, (unsigned int)MIPI_TX_REGS_SIZE);
        if (g_mipi_tx_regs_va == NULL) {
            mipi_tx_err("remap mipi_tx reg addr fail\n");
            return -1;
        }
        g_reg_map_flag = 1;
    }

    if (g_mipi_tx_phy_regs_va == NULL) {
        g_mipi_tx_phy_regs_va = (volatile mipi_tx_phy_regs_type_t *)osal_ioremap(
            MIPI_TX_PHY_REGS_ADDR, (unsigned int)MIPI_TX_PHY_REGS_SIZE);
        if (g_mipi_tx_phy_regs_va == NULL) {
            mipi_tx_err("remap mipi_tx phy reg addr fail\n");
            return -1;
        }
        g_phy_reg_map_flag = 1;
    }

    return 0;
}

static void mipi_tx_drv_reg_exit(void)
{
    if (g_reg_map_flag == 1) {
        if (g_mipi_tx_regs_va != NULL) {
            osal_iounmap((void *)g_mipi_tx_regs_va, (unsigned long)MIPI_TX_REGS_SIZE);
            g_mipi_tx_regs_va = NULL;
        }
        g_reg_map_flag = 0;
    }

    if (g_phy_reg_map_flag == 1) {
        if (g_mipi_tx_phy_regs_va != NULL) {
            osal_iounmap((void *)g_mipi_tx_phy_regs_va, (unsigned long)MIPI_TX_PHY_REGS_SIZE);
            g_mipi_tx_phy_regs_va = NULL;
        }
        g_phy_reg_map_flag = 0;
    }
}

#if MIPI_TX_INTERRUPTS_ENABLE
static void mipi_tx_enable_int(int enable)
{
    volatile reg_int1_mask int1_mask;

    int1_mask.u32 = g_mipi_tx_regs_va->int1_mask.u32;
    int1_mask.bits.mask_vss = enable & 0x1;
    g_mipi_tx_regs_va->int1_mask.u32 = int1_mask.u32;
}

static void mipi_tx_clear_int(void)
{
    volatile reg_int1_status int1_stat;
    volatile reg_int0_status int0_stat;

    /* warning: read register to clear interrupt status, do not delete it. */
    int1_stat.u32 = g_mipi_tx_regs_va->int1_status.u32;
    int0_stat.u32 = g_mipi_tx_regs_va->int0_status.u32;

    /* ignore compile warning: variable 'int_stat' set but not used */
    (void)int1_stat.u32;
    (void)int0_stat.u32;
}

static int mipi_tx_interrupt_route(int irq, void* dev_id)
{
    mipi_tx_clear_int();
    return OSAL_IRQ_HANDLED;
}

static int mipi_tx_register_irq(void)
{
    int ret;
    ret = osal_irq_request(g_mipi_tx_irq_num, mipi_tx_interrupt_route, NULL, "MIPI_TX", mipi_tx_interrupt_route);
    if (ret < 0) {
        mipi_tx_err("mipi_tx: failed to register irq.\n");
        return -1;
    }

    return 0;
}

static void mipi_tx_unregister_irq(void)
{
    osal_irq_free(g_mipi_tx_irq_num, mipi_tx_interrupt_route);
}
#endif

static void mipi_tx_drv_hw_init(int smooth)
{
#ifdef OT_FPGA
    /*
     * reset operation for fpga version.
     */
    unsigned long mipi_tx_crg_addr;

    mipi_tx_crg_addr = (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG, (unsigned long)0x4);
    if (mipi_tx_crg_addr == 0) {
        return;
    }

    write_reg32(mipi_tx_crg_addr, 0x3, 0x3);
    write_reg32(mipi_tx_crg_addr, 0x0, 0x3);

    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_addr, (unsigned long)0x4);

#else
    unsigned long mipi_tx_crg_cfg1_addr;
    unsigned long mipi_tx_crg_cfg2_addr;

    mipi_tx_crg_cfg1_addr = (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG, (unsigned long)0x4);
    mipi_tx_crg_cfg2_addr =
        (unsigned long)(uintptr_t)osal_ioremap(MIPI_TX_CRG + (unsigned long)0x8, (unsigned long)0x4);

    /* mipi_tx gate clk enable */
    write_reg32(mipi_tx_crg_cfg1_addr, 1, 0x1);

    /* reset */
    if (smooth == FALSE) {
        write_reg32(mipi_tx_crg_cfg1_addr, 1 << 1, 0x1 << 1); /* 1: bit1 */
        osal_udelay(1);
    }

    /* unreset */
    write_reg32(mipi_tx_crg_cfg1_addr, 0 << 1, 0x1 << 1); /* 1: bit1 */

    /* select ref clk 19.2MHz */
    write_reg32(mipi_tx_crg_cfg1_addr, 0 << 2, 0x3 << 2); /* 2: bit2 */

    /* reset mipitx */
    write_reg32(mipi_tx_crg_cfg2_addr, 0 << 8, 0x1 << 8); /* 8: bit8 */

    write_reg32(mipi_tx_crg_cfg2_addr, 1 << 4, 0x1 << 4); /* 4: bit4 */

    write_reg32(mipi_tx_crg_cfg2_addr, 0 << 0, 0x1 << 0); /* 0: bit0 */

    if (smooth == FALSE) {
        /* mipi_tx gate clk disable */
        write_reg32(mipi_tx_crg_cfg1_addr, 0, 0x1);
    }
    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_cfg1_addr, (unsigned long)0x4);
    osal_iounmap((void *)(uintptr_t)mipi_tx_crg_cfg2_addr, (unsigned long)0x4);
#endif
}

int mipi_tx_drv_init(int smooth)
{
    int ret;

    ret = mipi_tx_drv_reg_init();
    if (ret < 0) {
        mipi_tx_err("mipi_tx_drv_reg_init fail!\n");
        goto fail0;
    }

    mipi_tx_drv_hw_init(smooth);

#if MIPI_TX_INTERRUPTS_ENABLE
    ret = mipi_tx_register_irq();
    if (ret < 0) {
        mipi_tx_err("mipi_tx_register_irq fail!\n");
        goto fail1;
    }
#endif

    return 0;

#if MIPI_TX_INTERRUPTS_ENABLE
fail1:
    mipi_tx_drv_reg_exit();
#endif

fail0:
    return -1;
}

void mipi_tx_drv_exit(void)
{
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_unregister_irq();
#endif
    mipi_tx_drv_reg_exit();
}

#endif /* #if mipi_tx_desc("init") */

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif
