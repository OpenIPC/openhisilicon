/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_drv.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_drv.h"
#include "isp_drv_defines.h"
#include "isp_reg_define.h"
#include "isp_stt_define.h"
#include "hi_common.h"
#include "hi_osal.h"
#include "hi_math_adapt.h"
#include "mkp_isp.h"
#include "isp.h"
#include "mm_ext.h"
#include "sys_ext.h"
#include "vi_ext.h"

static hi_void  *g_reg_vicap_base_va = HI_NULL;
static hi_void  *g_reg_vicap_ch_base_va[ISP_MAX_PIPE_NUM] = { HI_NULL };
static hi_void  *g_reg_ispfe_base_va[ISP_MAX_PIPE_NUM]    = { HI_NULL };
static hi_void  *g_reg_ispbe_base_va[ISP_MAX_BE_NUM]      = { HI_NULL };
static hi_void  *g_reg_viproc_base_va[ISP_MAX_BE_NUM]     = { HI_NULL };

#define ENABLE_CLOCK  1

static hi_u32 g_drc_cur_luma_lut[HI_ISP_DRC_SHP_LOG_CONFIG_NUM][HI_ISP_DRC_EXP_COMP_SAMPLE_NUM - 1] = {
    {1,     1,      5,      31,     180,    1023,   32767},
    {1,     3,      8,      52,     277,    1446,   38966},
    {2,     5,      15,     87,     427,    2044,   46337},
    {4,     9,      27,     144,    656,    2888,   55101},
    {7,     16,     48,     240,    1008,   4080,   65521},
    {12,    29,     85,     399,    1547,   5761,   77906},
    {23,    53,     151,    660,    2372,   8128,   92622},
    {42,    97,     267,    1090,   3628,   11458,  110100},
    {76,    175,    468,    1792,   5537,   16130,  130840},
    {138,   313,    816,    2933,   8423,   22664,  155417},
    {258,   555,    1412,   4770,   12758,  31760,  184476},
    {441,   977,    2420,   7699,   19215,  44338,  218711},
    {776,   1698,   4100,   12304,  28720,  61568,  258816},
    {1344,  2907,   6847,   19416,  42491,  84851,  305376},
    {2283,  4884,   11224,  30137,  62006,  115708, 358680},
    {3783,  8004,   17962,  45770,  88821,  155470, 418391},
};

static hi_u32 g_drc_div_denom_log[DRC_DENOM_LOG_EXP_NUM] = {
    52429, 55188,  58254,  61681,  65536,  69905,  74898, 80659,
    87379, 95319, 104843, 116472, 130980, 149557, 174114, 207870
};

static hi_u32 g_drc_denom_exp[DRC_DENOM_LOG_EXP_NUM] = {
    1310720, 1245184, 1179648, 1114113, 1048577, 983043, 917510, 851980,
    786455,  720942,  655452,  590008,  524657, 459488, 394682, 330589
};

static hi_u8 g_drc_shp_log[ISP_MAX_PIPE_NUM][ISP_STRIPING_MAX_NUM] = { [0 ... ISP_MAX_PIPE_NUM - 1] = { 8, 8, 8 } };
static hi_u8 g_drc_shp_exp[ISP_MAX_PIPE_NUM][ISP_STRIPING_MAX_NUM] = { [0 ... ISP_MAX_PIPE_NUM - 1] = { 8, 8, 8 } };

hi_void *isp_drv_get_reg_vicap_base_va(hi_void)
{
    return g_reg_vicap_base_va;
}

hi_void *isp_drv_get_vicap_ch_base_va(hi_vi_pipe vi_pipe)
{
    return g_reg_vicap_ch_base_va[vi_pipe];
}

hi_void *isp_drv_get_ispfe_base_va(hi_vi_pipe vi_pipe)
{
    return g_reg_ispfe_base_va[vi_pipe];
}

hi_void *isp_drv_get_ispbe_base_va(hi_u8 blk_dev)
{
    return g_reg_ispbe_base_va[blk_dev];
}

hi_void *isp_drv_get_viproc_base_va(hi_u8 blk_dev)
{
    return g_reg_viproc_base_va[blk_dev];
}

static hi_u16 sqrt32(hi_u32 arg)
{
    hi_u32 mask = (hi_u32)1 << 15; /* left shift 15 */
    hi_u16 res = 0;
    hi_u32 i = 0;

    for (i = 0; i < 16; i++) {  /* max value 16 */
        if ((res + (mask >> i)) * (res + (mask >> i)) <= arg) {
            res = res + (mask >> i);
        }
    }

    /* rounding */
    if (res * res + res < arg) {
        ++res;
    }

    return res;
}

/* isp drv FHY regs define */
hi_void isp_drv_set_input_sel(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 *input_sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    isp_check_pointer_void_return(isp_be_regs);

    o_isp_be_input_mux.u32 = isp_be_regs->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input0_sel = input_sel[0]; /* array index 0 */
    o_isp_be_input_mux.bits.isp_input1_sel = input_sel[1]; /* array index 1 */
    o_isp_be_input_mux.bits.isp_input2_sel = input_sel[2]; /* array index 2 */
    o_isp_be_input_mux.bits.isp_input3_sel = input_sel[3]; /* array index 3 */
    o_isp_be_input_mux.bits.isp_input4_sel = input_sel[4]; /* array index 4 */
    isp_be_regs->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}
static __inline hi_void isp_drv_set_be_dgain(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_dgain)
{
    U_ISP_DG_GAIN1 o_isp_dg_gain1;
    U_ISP_DG_GAIN2 o_isp_dg_gain2;

    o_isp_dg_gain1.u32 = (isp_dgain << 16) + isp_dgain; /* left shift 16 */
    isp_be_regs->ISP_DG_GAIN1.u32 = o_isp_dg_gain1.u32;

    o_isp_dg_gain2.u32 = (isp_dgain << 16) + isp_dgain; /* left shift 16 */
    isp_be_regs->ISP_DG_GAIN2.u32 = o_isp_dg_gain2.u32;
}

static __inline hi_void isp_drv_set_fe_dgain(S_ISPFE_REGS_TYPE *isp_fe_regs, hi_u32 isp_dgain)
{
    U_ISP_DG2_GAIN1 o_isp_dg_gain1;
    U_ISP_DG2_GAIN2 o_isp_dg_gain2;

    isp_check_pointer_void_return(isp_fe_regs);

    o_isp_dg_gain1.u32 = (isp_dgain << 16) + isp_dgain; /* left shift 16 */
    isp_fe_regs->ISP_DG2_GAIN1.u32 = o_isp_dg_gain1.u32;

    o_isp_dg_gain2.u32 = (isp_dgain << 16) + isp_dgain; /* left shift 16 */
    isp_fe_regs->ISP_DG2_GAIN2.u32 = o_isp_dg_gain2.u32;

    return;
}

static __inline hi_void isp_drv_awb_enable(S_VIPROC_REGS_TYPE *viproc_reg, hi_u32 isp_awb_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = viproc_reg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_awb_en = isp_awb_en;
    viproc_reg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;
}

static __inline hi_void isp_drv_set_isp_4dgain0(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp4_dgain0)
{
    U_ISP_4DG_0_GAIN1 o_isp_4dg_0_gain1;
    U_ISP_4DG_0_GAIN2 o_isp_4dg_0_gain2;

    o_isp_4dg_0_gain1.u32 = (isp4_dgain0 << 16) + isp4_dgain0; /* left shift 16 */
    isp_be_regs->ISP_4DG_0_GAIN1.u32 = o_isp_4dg_0_gain1.u32;

    o_isp_4dg_0_gain2.u32 = (isp4_dgain0 << 16) + isp4_dgain0; /* left shift 16 */
    isp_be_regs->ISP_4DG_0_GAIN2.u32 = o_isp_4dg_0_gain2.u32;
}

static __inline hi_void isp_drv_set_isp_4dgain1(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp4_dgain1)
{
    U_ISP_4DG_1_GAIN1 o_isp_4dg_1_gain1;
    U_ISP_4DG_1_GAIN2 o_isp_4dg_1_gain2;

    o_isp_4dg_1_gain1.u32 = (isp4_dgain1 << 16) + isp4_dgain1; /* left shift 16 */
    isp_be_regs->ISP_4DG_1_GAIN1.u32 = o_isp_4dg_1_gain1.u32;

    o_isp_4dg_1_gain2.u32 = (isp4_dgain1 << 16) + isp4_dgain1; /* left shift 16 */
    isp_be_regs->ISP_4DG_1_GAIN2.u32 = o_isp_4dg_1_gain2.u32;
}

static __inline hi_void isp_drv_set_wdr_exporratio0(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_exporatio0)
{
    U_ISP_WDR_EXPORRATIO o_isp_wdr_exporatio0;

    o_isp_wdr_exporatio0.u32 = isp_be_regs->ISP_WDR_EXPORRATIO.u32;
    o_isp_wdr_exporatio0.bits.isp_wdr_exporratio0 = isp_wdr_exporatio0;
    isp_be_regs->ISP_WDR_EXPORRATIO.u32 = o_isp_wdr_exporatio0.u32;
}

static __inline hi_void isp_drv_set_wdr_expo_value0(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_expovalue0)
{
    U_ISP_WDR_EXPOVALUE o_isp_wdr_expovalue0;

    o_isp_wdr_expovalue0.u32 = isp_be_regs->ISP_WDR_EXPOVALUE.u32;
    o_isp_wdr_expovalue0.bits.isp_wdr_expovalue0 = isp_wdr_expovalue0;
    isp_be_regs->ISP_WDR_EXPOVALUE.u32 = o_isp_wdr_expovalue0.u32;
}

static __inline hi_void isp_drv_set_wdr_expo_value1(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_expovalue1)
{
    U_ISP_WDR_EXPOVALUE o_isp_wdr_expovalue0;

    o_isp_wdr_expovalue0.u32 = isp_be_regs->ISP_WDR_EXPOVALUE.u32;
    o_isp_wdr_expovalue0.bits.isp_wdr_expovalue1 = isp_wdr_expovalue1;
    isp_be_regs->ISP_WDR_EXPOVALUE.u32 = o_isp_wdr_expovalue0.u32;
}

static __inline hi_void isp_drv_set_flick_exporatio0(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_flick_exporatio)
{
    U_ISP_FLICK_EXPORATIO o_isp_flick_exporatio;

    o_isp_flick_exporatio.u32 = isp_be_regs->ISP_FLICK_EXPORATIO.u32;
    o_isp_flick_exporatio.bits.isp_flick_exporatio = isp_flick_exporatio;
    isp_be_regs->ISP_FLICK_EXPORATIO.u32 = o_isp_flick_exporatio.u32;
}

static __inline hi_void isp_drv_set_wdr_blc_comp0(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_blc_comp0)
{
    U_ISP_WDR_BLC_COMP o_isp_wdr_blc_comp0;

    o_isp_wdr_blc_comp0.u32 = isp_be_regs->ISP_WDR_BLC_COMP.u32;
    o_isp_wdr_blc_comp0.bits.isp_wdr_blc_comp0 = isp_wdr_blc_comp0;
    isp_be_regs->ISP_WDR_BLC_COMP.u32 = o_isp_wdr_blc_comp0.u32;
}

static __inline hi_void isp_drv_set_wdr_max_ratio(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_maxratio)
{
    U_ISP_WDR_MAXRATIO o_isp_wdr_maxratio;

    o_isp_wdr_maxratio.u32 = isp_be_regs->ISP_WDR_MAXRATIO.u32;
    o_isp_wdr_maxratio.bits.isp_wdr_maxratio = isp_wdr_maxratio;
    isp_be_regs->ISP_WDR_MAXRATIO.u32 = o_isp_wdr_maxratio.u32;
}

static __inline hi_void isp_drv_set_wdr_long_thr(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_long_thr)
{
    U_ISP_WDR_WGTIDX_THR o_isp_wdr_wgtidx_thr;

    o_isp_wdr_wgtidx_thr.u32 = isp_be_regs->ISP_WDR_WGTIDX_THR.u32;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_long_thr  = isp_wdr_long_thr;
    isp_be_regs->ISP_WDR_WGTIDX_THR.u32 = o_isp_wdr_wgtidx_thr.u32;
}

static __inline hi_void isp_drv_set_wdr_short_thr(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_short_thr)
{
    U_ISP_WDR_WGTIDX_THR o_isp_wdr_wgtidx_thr;

    o_isp_wdr_wgtidx_thr.u32 = isp_be_regs->ISP_WDR_WGTIDX_THR.u32;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_short_thr  = isp_wdr_short_thr;
    isp_be_regs->ISP_WDR_WGTIDX_THR.u32 = o_isp_wdr_wgtidx_thr.u32;
}

static __inline hi_void isp_drv_set_wdr_mdt_en(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_mdt_en)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;

    o_isp_wdr_ctrl.u32 = isp_be_regs->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_mdt_en = isp_wdr_mdt_en;
    isp_be_regs->ISP_WDR_CTRL.u32      = o_isp_wdr_ctrl.u32;
}

static __inline hi_void isp_drv_set_wdr_fusion_mode(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_wdr_fusion_mode)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;

    o_isp_wdr_ctrl.u32 = isp_be_regs->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_fusionmode = isp_wdr_fusion_mode;
    isp_be_regs->ISP_WDR_CTRL.u32     = o_isp_wdr_ctrl.u32;
}

static __inline hi_void isp_drv_set_ldci_stat_evratio(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_ldci_stat_evratio)
{
    U_ISP_LDCI_STAT_EVRATIO o_isp_ldci_stat_evratio;

    o_isp_ldci_stat_evratio.u32 = isp_be_regs->ISP_LDCI_STAT_EVRATIO.u32;
    o_isp_ldci_stat_evratio.bits.isp_ldci_stat_evratio = isp_ldci_stat_evratio;
    isp_be_regs->ISP_LDCI_STAT_EVRATIO.u32 = o_isp_ldci_stat_evratio.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma0(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_0)
{
    U_ISP_DRC_PREV_LUMA_0 o_isp_drc_prev_luma_0;

    o_isp_drc_prev_luma_0.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_0.u32;
    o_isp_drc_prev_luma_0.bits.isp_drc_prev_luma_0 = isp_drc_prev_luma_0;
    isp_be_regs->ISP_DRC_PREV_LUMA_0.u32 = o_isp_drc_prev_luma_0.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma1(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_1)
{
    U_ISP_DRC_PREV_LUMA_1 o_isp_drc_prev_luma_1;

    o_isp_drc_prev_luma_1.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_1.u32;
    o_isp_drc_prev_luma_1.bits.isp_drc_prev_luma_1 = isp_drc_prev_luma_1;
    isp_be_regs->ISP_DRC_PREV_LUMA_1.u32 = o_isp_drc_prev_luma_1.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma2(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_2)
{
    U_ISP_DRC_PREV_LUMA_2 o_isp_drc_prev_luma_2;

    o_isp_drc_prev_luma_2.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_2.u32;
    o_isp_drc_prev_luma_2.bits.isp_drc_prev_luma_2 = isp_drc_prev_luma_2;
    isp_be_regs->ISP_DRC_PREV_LUMA_2.u32 = o_isp_drc_prev_luma_2.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma3(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_3)
{
    U_ISP_DRC_PREV_LUMA_3 o_isp_drc_prev_luma_3;

    o_isp_drc_prev_luma_3.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_3.u32;
    o_isp_drc_prev_luma_3.bits.isp_drc_prev_luma_3 = isp_drc_prev_luma_3;
    isp_be_regs->ISP_DRC_PREV_LUMA_3.u32 = o_isp_drc_prev_luma_3.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma4(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_4)
{
    U_ISP_DRC_PREV_LUMA_4 o_isp_drc_prev_luma_4;

    o_isp_drc_prev_luma_4.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_4.u32;
    o_isp_drc_prev_luma_4.bits.isp_drc_prev_luma_4 = isp_drc_prev_luma_4;
    isp_be_regs->ISP_DRC_PREV_LUMA_4.u32 = o_isp_drc_prev_luma_4.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma5(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_5)
{
    U_ISP_DRC_PREV_LUMA_5 o_isp_drc_prev_luma_5;

    o_isp_drc_prev_luma_5.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_5.u32;
    o_isp_drc_prev_luma_5.bits.isp_drc_prev_luma_5 = isp_drc_prev_luma_5;
    isp_be_regs->ISP_DRC_PREV_LUMA_5.u32 = o_isp_drc_prev_luma_5.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma6(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_6)
{
    U_ISP_DRC_PREV_LUMA_6 o_isp_drc_prev_luma_6;

    o_isp_drc_prev_luma_6.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_6.u32;
    o_isp_drc_prev_luma_6.bits.isp_drc_prev_luma_6 = isp_drc_prev_luma_6;
    isp_be_regs->ISP_DRC_PREV_LUMA_6.u32 = o_isp_drc_prev_luma_6.u32;
}

static __inline hi_void isp_drv_set_drc_prev_luma7(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_prev_luma_7)
{
    U_ISP_DRC_PREV_LUMA_7 o_isp_drc_prev_luma_7;

    o_isp_drc_prev_luma_7.u32 = isp_be_regs->ISP_DRC_PREV_LUMA_7.u32;
    o_isp_drc_prev_luma_7.bits.isp_drc_prev_luma_7 = isp_drc_prev_luma_7;
    isp_be_regs->ISP_DRC_PREV_LUMA_7.u32 = o_isp_drc_prev_luma_7.u32;
}

static __inline hi_void isp_drv_set_drc_shp_cfg(S_ISPBE_REGS_TYPE *isp_be_regs,
                                                hi_u32 isp_drc_shp_log,
                                                hi_u32 isp_drc_shp_exp)
{
    U_ISP_DRC_SHP_CFG o_isp_drc_shp_cfg;

    o_isp_drc_shp_cfg.u32 = isp_be_regs->ISP_DRC_SHP_CFG.u32;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_log = isp_drc_shp_log;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_exp = isp_drc_shp_exp;
    isp_be_regs->ISP_DRC_SHP_CFG.u32 = o_isp_drc_shp_cfg.u32;
}

static __inline hi_void isp_drv_set_drc_div_denom_log(S_ISPBE_REGS_TYPE *isp_be_regs,
                                                      hi_u32 isp_drc_div_denom_log)
{
    U_ISP_DRC_DIV_DENOM_LOG o_isp_drc_div_denom_log;

    o_isp_drc_div_denom_log.u32 = isp_be_regs->ISP_DRC_DIV_DENOM_LOG.u32;
    o_isp_drc_div_denom_log.bits.isp_drc_div_denom_log = isp_drc_div_denom_log;
    isp_be_regs->ISP_DRC_DIV_DENOM_LOG.u32 = o_isp_drc_div_denom_log.u32;
}

static __inline hi_void isp_drv_set_drc_denom_exp(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 isp_drc_denom_exp)
{
    U_ISP_DRC_DENOM_EXP o_isp_drc_denom_exp;

    o_isp_drc_denom_exp.u32 = isp_be_regs->ISP_DRC_DENOM_EXP.u32;
    o_isp_drc_denom_exp.bits.isp_drc_denom_exp = isp_drc_denom_exp;
    isp_be_regs->ISP_DRC_DENOM_EXP.u32 = o_isp_drc_denom_exp.u32;
}

hi_void isp_drv_set_ldci_write_stt_addr(S_VIPROC_REGS_TYPE *viproc_reg, hi_u64 write_addr)
{
    viproc_reg->OUT_PARA_DCI_ADDR_LOW.u32 = get_low_addr(write_addr);
    return;
}

hi_void isp_drv_set_ldci_read_stt_addr(S_VIPROC_REGS_TYPE *viproc_reg, hi_u64 read_addr)
{
    viproc_reg->PARA_DCI_ADDR_LOW.u32 = get_low_addr(read_addr);
}

/* Fe Blc */
static __inline hi_void isp_drv_fe_dg_offset_write(S_ISPFE_REGS_TYPE *fe_reg, hi_u16 *dg_blc)
{
    fe_reg->ISP_DG2_BLC_OFFSET1.u32 = ((hi_u32)dg_blc[BAYER_CHN_R] << 16) + dg_blc[BAYER_CHN_GR]; /* bit16~30: r_blc */
    fe_reg->ISP_DG2_BLC_OFFSET2.u32 = ((hi_u32)dg_blc[BAYER_CHN_B] << 16) + dg_blc[BAYER_CHN_GB]; /* bit16~30: b_blc */
}

static __inline hi_void isp_drv_fe_wb_offset_write(S_ISPFE_REGS_TYPE *fe_reg, hi_u16 *wb_blc)
{
    fe_reg->ISP_WB1_BLC_OFFSET1.u32 = ((hi_u32)wb_blc[BAYER_CHN_R] << 16) + wb_blc[BAYER_CHN_GR]; /* bit16~30: r_blc */
    fe_reg->ISP_WB1_BLC_OFFSET2.u32 = ((hi_u32)wb_blc[BAYER_CHN_B] << 16) + wb_blc[BAYER_CHN_GB]; /* bit16~30: b_blc */
}

static __inline hi_void isp_drv_fe_ae_offset_write(S_ISPFE_REGS_TYPE *fe_reg, hi_u16 *ae_blc)
{
    fe_reg->ISP_AE1_OFFSET_R.u32  = ae_blc[BAYER_CHN_R];
    fe_reg->ISP_AE1_OFFSET_GR.u32 = ae_blc[BAYER_CHN_GR];
    fe_reg->ISP_AE1_OFFSET_GB.u32 = ae_blc[BAYER_CHN_GB];
    fe_reg->ISP_AE1_OFFSET_B.u32  = ae_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_fe_blc_offset_write(S_ISPFE_REGS_TYPE *fe_reg, hi_u16 *fe_blc)
{
    fe_reg->ISP_BLC1_OFFSET1.u32 = ((hi_u32)fe_blc[BAYER_CHN_R] << 16) + fe_blc[BAYER_CHN_GR]; /* bit16~30: r_blc */
    fe_reg->ISP_BLC1_OFFSET2.u32 = ((hi_u32)fe_blc[BAYER_CHN_B] << 16) + fe_blc[BAYER_CHN_GB]; /* bit16~30: b_blc */
}

static __inline hi_void isp_drv_be_f0_4dg_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *f0_4dg_blc)
{
    be_reg->ISP_4DG_0_BLC_OFFSET1.u32 = ((hi_u32)f0_4dg_blc[BAYER_CHN_R] << 16) + /* bit16~30: r_blc */
                                        f0_4dg_blc[BAYER_CHN_GR];

    be_reg->ISP_4DG_0_BLC_OFFSET2.u32 = ((hi_u32)f0_4dg_blc[BAYER_CHN_B] << 16) + /* bit16~30: b_blc */
                                        f0_4dg_blc[BAYER_CHN_GB];
}

static __inline hi_void isp_drv_be_f1_4dg_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *f1_4dg_blc)
{
    be_reg->ISP_4DG_1_BLC_OFFSET1.u32 = ((hi_u32)f1_4dg_blc[BAYER_CHN_R] << 16) + /* bit16~30: r_blc */
                                        f1_4dg_blc[BAYER_CHN_GR];

    be_reg->ISP_4DG_1_BLC_OFFSET2.u32 = ((hi_u32)f1_4dg_blc[BAYER_CHN_B] << 16) + /* bit16~30: b_blc */
                                        f1_4dg_blc[BAYER_CHN_GB];
}

static __inline hi_void isp_drv_wdr_out_blc_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 out_blc)
{
    U_ISP_WDR_OUTBLC o_isp_wdr_outblc;

    o_isp_wdr_outblc.u32 = be_reg->ISP_WDR_OUTBLC.u32;
    o_isp_wdr_outblc.bits.isp_wdr_outblc = out_blc;
    be_reg->ISP_WDR_OUTBLC.u32 = o_isp_wdr_outblc.u32;
}

static __inline hi_void isp_drv_be_f0_wdr_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *f0_wdr_blc)
{
    be_reg->ISP_WDR_F0_INBLC0.u32 = ((hi_u32)f0_wdr_blc[BAYER_CHN_R] << 16) +  /* bit16~30: r_blc */
                                    f0_wdr_blc[BAYER_CHN_GR];

    be_reg->ISP_WDR_F0_INBLC1.u32 = ((hi_u32)f0_wdr_blc[BAYER_CHN_GB] << 16) + /* bit16~30: gb_blc */
                                    f0_wdr_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_f1_wdr_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *f1_wdr_blc)
{
    be_reg->ISP_WDR_F1_INBLC0.u32 = ((hi_u32)f1_wdr_blc[BAYER_CHN_R] << 16) +  /* bit16~30: r_blc */
                                    f1_wdr_blc[BAYER_CHN_GR];

    be_reg->ISP_WDR_F1_INBLC1.u32 = ((hi_u32)f1_wdr_blc[BAYER_CHN_GB] << 16) + /* bit16~30: gb_blc */
                                    f1_wdr_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_f0_flick_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *f0_flick_blc)
{
    be_reg->ISP_FLICK_F0_INBLC0.u32 = ((hi_u32)f0_flick_blc[BAYER_CHN_R] << 16) +  /* bit16~30: r_blc */
                                      f0_flick_blc[BAYER_CHN_GR];

    be_reg->ISP_FLICK_F0_INBLC1.u32 = ((hi_u32)f0_flick_blc[BAYER_CHN_GB] << 16) + /* bit16~30: gb_blc */
                                      f0_flick_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_f1_flick_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *f1_flick_blc)
{
    be_reg->ISP_FLICK_F1_INBLC0.u32 = ((hi_u32)f1_flick_blc[BAYER_CHN_R] << 16) +  /* bit16~30: r_blc */
                                      f1_flick_blc[BAYER_CHN_GR];

    be_reg->ISP_FLICK_F1_INBLC1.u32 = ((hi_u32)f1_flick_blc[BAYER_CHN_GB] << 16) + /* bit16~30: gb_blc */
                                      f1_flick_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_bnr_rlmt_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 bnr_rlmt_blc)
{
    U_ISP_BNR_CHROMA o_isp_bnr_chroma;
    o_isp_bnr_chroma.u32 = be_reg->ISP_BNR_CHROMA.u32;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_blc = bnr_rlmt_blc;
    be_reg->ISP_BNR_CHROMA.u32 = o_isp_bnr_chroma.u32;
}

static __inline hi_void isp_drv_be_lsc_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *lsc_blc)
{
    be_reg->ISP_LSC_BLC0.u32  = ((hi_u32)lsc_blc[BAYER_CHN_GR] << 16) + lsc_blc[BAYER_CHN_R]; /* bit16~30: gr_blc */
    be_reg->ISP_LSC_BLC1.u32  = ((hi_u32)lsc_blc[BAYER_CHN_GB] << 16) + lsc_blc[BAYER_CHN_B]; /* bit16~30: gb_blc */
}

static __inline hi_void isp_drv_be_dgain_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *dg_blc)
{
    be_reg->ISP_DG_BLC_OFFSET1.u32  = ((hi_u32)dg_blc[BAYER_CHN_R] << 16) + dg_blc[BAYER_CHN_GR]; /* bit16~30: r_blc */
    be_reg->ISP_DG_BLC_OFFSET2.u32  = ((hi_u32)dg_blc[BAYER_CHN_B] << 16) + dg_blc[BAYER_CHN_GB]; /* bit16~30: b_blc */
}

static __inline hi_void isp_drv_be_ae_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *ae_blc)
{
    be_reg->ISP_AE_OFFSET_R.u32  = ae_blc[BAYER_CHN_R];
    be_reg->ISP_AE_OFFSET_GR.u32 = ae_blc[BAYER_CHN_GR];
    be_reg->ISP_AE_OFFSET_GB.u32 = ae_blc[BAYER_CHN_GB];
    be_reg->ISP_AE_OFFSET_B.u32  = ae_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_la_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *la_blc)
{
    be_reg->ISP_LA_OFFSET_R.u32  = la_blc[BAYER_CHN_R];
    be_reg->ISP_LA_OFFSET_GR.u32 = la_blc[BAYER_CHN_GR];
    be_reg->ISP_LA_OFFSET_GB.u32 = la_blc[BAYER_CHN_GB];
    be_reg->ISP_LA_OFFSET_B.u32  = la_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_wb_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *wb_blc)
{
    be_reg->ISP_WB_BLC_OFFSET1.u32  = ((hi_u32)wb_blc[BAYER_CHN_R] << 16) + wb_blc[BAYER_CHN_GR]; /* bit16~30: r_blc */
    be_reg->ISP_WB_BLC_OFFSET2.u32  = ((hi_u32)wb_blc[BAYER_CHN_B] << 16) + wb_blc[BAYER_CHN_GB]; /* bit16~30: b_blc */
}

static __inline hi_void isp_drv_be_expander_offset_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *expander_blc)
{
    be_reg->ISP_EXPANDER_OFFSETR.u32  = expander_blc[BAYER_CHN_R];
    be_reg->ISP_EXPANDER_OFFSETGR.u32 = expander_blc[BAYER_CHN_GR];
    be_reg->ISP_EXPANDER_OFFSETGB.u32 = expander_blc[BAYER_CHN_GB];
    be_reg->ISP_EXPANDER_OFFSETB.u32  = expander_blc[BAYER_CHN_B];
}

static __inline hi_void isp_drv_be_bnr_de_blcvalue_write(S_ISPBE_REGS_TYPE *be_reg, hi_u16 bnr_de_blcvalue)
{
    U_ISP_BNR_DETAIL o_isp_bnr_detail;
    o_isp_bnr_detail.u32 = be_reg->ISP_BNR_DETAIL.u32;
    o_isp_bnr_detail.bits.isp_bnr_de_blcvalue = bnr_de_blcvalue;
    be_reg->ISP_BNR_DETAIL.u32 = o_isp_bnr_detail.u32;
}

hi_s32 isp_drv_be_remap(void)
{
    hi_u32 isp_be_phy_pipe = 0;

    for (isp_be_phy_pipe = 0; isp_be_phy_pipe < ISP_MAX_BE_NUM; isp_be_phy_pipe++) {
        g_reg_ispbe_base_va[isp_be_phy_pipe] =
            (void *)osal_ioremap(isp_be_reg_base(isp_be_phy_pipe), (hi_u32)VI_ISP_BE_REG_SIZE);

        if (g_reg_ispbe_base_va[isp_be_phy_pipe] == HI_NULL) {
            osal_printk("Remap ISP BE[%d] failed!\n", isp_be_phy_pipe);
            return HI_FAILURE;
        }

        g_reg_viproc_base_va[isp_be_phy_pipe] =
            (void *)osal_ioremap(isp_viproc_reg_base(isp_be_phy_pipe), (hi_u32)VIPROC_REG_SIZE);

        if (g_reg_viproc_base_va[isp_be_phy_pipe] == HI_NULL) {
            osal_printk("Remap isp viproc[%d] failed!\n", isp_be_phy_pipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

hi_void isp_drv_be_unmap(void)
{
    hi_u32 isp_be_phy_pipe = 0;

    for (isp_be_phy_pipe = 0; isp_be_phy_pipe < ISP_MAX_BE_NUM; isp_be_phy_pipe++) {
        if (g_reg_ispbe_base_va[isp_be_phy_pipe] != HI_NULL) {
            osal_iounmap(g_reg_ispbe_base_va[isp_be_phy_pipe]);
            g_reg_ispbe_base_va[isp_be_phy_pipe] = HI_NULL;
        }

        if (g_reg_viproc_base_va[isp_be_phy_pipe] != HI_NULL) {
            osal_iounmap(g_reg_viproc_base_va[isp_be_phy_pipe]);
            g_reg_viproc_base_va[isp_be_phy_pipe] = HI_NULL;
        }
    }
}

hi_s32 isp_drv_vicap_remap(void)
{
    g_reg_vicap_base_va = (void *)osal_ioremap(CAP_REG_BASE, (hi_u32)CAP_REG_SIZE_ALIGN);
    if (g_reg_vicap_base_va == HI_NULL) {
        osal_printk("Remap ISP PT failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_drv_vicap_unmap(void)
{
    if (g_reg_vicap_base_va != HI_NULL) {
        osal_iounmap(g_reg_vicap_base_va);
        g_reg_vicap_base_va = HI_NULL;
    }
}

static hi_void isp_drv_sys_init(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_memset(drv_ctx, 0, sizeof(isp_drv_ctx));
    osal_snprintf(drv_ctx->name, sizeof(drv_ctx->name), "ISP[%d]", vi_pipe);

    drv_ctx->work_mode.running_mode = ISP_MODE_RUNNING_OFFLINE;
    drv_ctx->work_mode.block_num    = 1;

    drv_ctx->ai_info.pq_ai_en     = HI_FALSE;
    drv_ctx->ai_info.ai_pipe_id   = -1;
    drv_ctx->ai_info.base_pipe_id = -1;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        drv_ctx->chn_sel_attr[i].channel_sel = 0;
    }

    drv_ctx->running_state = ISP_BE_BUF_STATE_INIT;
    drv_ctx->exit_state    = ISP_BE_BUF_EXIT;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        drv_ctx->be_sync_para.isp_dgain[i]   = 0x100;
    }
}

static hi_void isp_drv_stitch_init(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    drv_ctx->stitch_attr.stitch_enable      = HI_FALSE;
    drv_ctx->stitch_attr.stitch_pipe_num    = 2; /* default: stitch num is 2 for online mode */
    drv_ctx->stitch_attr.stitch_bind_id[0]  = 0;
    drv_ctx->stitch_attr.stitch_bind_id[1]  = 3; /* default: stitch pipe 0 & 3 */
    drv_ctx->stitch_attr.stitch_bind_id[2]  = -1; /* id[2] Not use stitch pipe is -1 */
    drv_ctx->stitch_attr.stitch_bind_id[3]  = -1; /* id[3] Not use stitch pipe is -1 */
}

static hi_void isp_drv_snap_init(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    drv_ctx->snap_attr.snap_type = SNAP_TYPE_NORMAL;
    drv_ctx->snap_attr.picture_pipe_id = -1;
    drv_ctx->snap_attr.preview_pipe_id = -1;
    drv_ctx->snap_attr.load_ccm        = HI_TRUE;
    drv_ctx->snap_attr.pro_param.operation_mode = OPERATION_MODE_AUTO;

    for (i = 0; i < PRO_MAX_FRAME_NUM; i++) {
        drv_ctx->snap_attr.pro_param.auto_param.pro_exp_step[i]   = 0x100;
        drv_ctx->snap_attr.pro_param.manual_param.man_exp_time[i] = 10000; /* default value 10000 */
        drv_ctx->snap_attr.pro_param.manual_param.man_sysgain[i]  = 0x400;
    }

    drv_ctx->snap_attr.pro_param.pro_frame_num = 3; /* default frame num is 3 */
}

static hi_void isp_drv_wait_sema_init(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    drv_ctx->use_node          = HI_NULL;
    drv_ctx->run_once_ok       = HI_FALSE;
    drv_ctx->run_once_flag     = HI_FALSE;
    drv_ctx->yuv_run_once_ok   = HI_FALSE;
    drv_ctx->yuv_run_once_flag = HI_FALSE;

    osal_wait_init(&drv_ctx->isp_wait);
    osal_wait_init(&drv_ctx->isp_wait_vd_start);
    osal_wait_init(&drv_ctx->isp_wait_vd_end);
    osal_wait_init(&drv_ctx->isp_wait_vd_be_end);
    osal_wait_init(&drv_ctx->isp_exit_wait);
    drv_ctx->edge             = HI_FALSE;
    drv_ctx->vd_start         = HI_FALSE;
    drv_ctx->vd_end           = HI_FALSE;
    drv_ctx->vd_be_end        = HI_FALSE;
    drv_ctx->mem_init         = HI_FALSE;
    drv_ctx->isp_init         = HI_FALSE;
    drv_ctx->pub_attr_ok      = HI_FALSE;
    drv_ctx->isp_run_flag     = HI_FALSE;
    drv_ctx->pro_trig_flag    = 0;
    osal_sema_init(&drv_ctx->isp_sem, 1);
    osal_sema_init(&drv_ctx->isp_sem_vd, 1);
    osal_sema_init(&drv_ctx->isp_sem_be_vd, 1);
    osal_sema_init(&drv_ctx->proc_sem, 1);
    osal_sema_init(&drv_ctx->pro_info_sem, 1);
}

hi_s32 isp_drv_fe_remap(void)
{
    hi_vi_pipe vi_pipe;

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PHY_PIPE_NUM; vi_pipe++) {
        g_reg_ispfe_base_va[vi_pipe] = (void *)osal_ioremap(isp_fe_reg_base(vi_pipe), (hi_u32)VI_ISP_FE_REG_SIZE);

        if (g_reg_ispfe_base_va[vi_pipe] == HI_NULL) {
            osal_printk("Remap ISP[%d] FE failed!\n", vi_pipe);
            return HI_FAILURE;
        }

        g_reg_vicap_ch_base_va[vi_pipe] =
            (void *)osal_ioremap(isp_vicap_ch_reg_base(vi_pipe), (hi_u32)VICAP_CH_REG_SIZE_ALIGN);

        if (g_reg_vicap_ch_base_va[vi_pipe] == HI_NULL) {
            osal_printk("Remap ISP[%d] FE STT failed!\n", vi_pipe);
            return HI_FAILURE;
        }

        /* enable port int & isp int */
        io_rw_pt_address(VICAP_HD_MASK) |= vicap_int_mask_pt((hi_u32)vi_pipe);
        io_rw_pt_address(VICAP_HD_MASK) |= vicap_int_mask_isp((hi_u32)vi_pipe);

        io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK) = (0x0);
    }

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        isp_drv_sys_init(vi_pipe);

        isp_drv_stitch_init(vi_pipe);

        isp_drv_snap_init(vi_pipe);

        isp_drv_wait_sema_init(vi_pipe);

        sync_task_init(vi_pipe);
    }

    return HI_SUCCESS;
}

hi_void isp_drv_fe_unmap(void)
{
    hi_vi_pipe vi_pipe;
    isp_drv_ctx *drv_ctx = HI_NULL;

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PHY_PIPE_NUM; vi_pipe++) {
        if (g_reg_ispfe_base_va[vi_pipe] != HI_NULL) {
            osal_iounmap(g_reg_ispfe_base_va[vi_pipe]);
            g_reg_ispfe_base_va[vi_pipe] = HI_NULL;
        }

        if (g_reg_vicap_ch_base_va[vi_pipe] != HI_NULL) {
            osal_iounmap(g_reg_vicap_ch_base_va[vi_pipe]);
            g_reg_vicap_ch_base_va[vi_pipe] = HI_NULL;
        }
    }

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        drv_ctx = isp_drv_get_ctx(vi_pipe);

        osal_sema_destory(&drv_ctx->isp_sem);
        osal_sema_destory(&drv_ctx->isp_sem_vd);
        osal_sema_destory(&drv_ctx->isp_sem_be_vd);
        osal_sema_destory(&drv_ctx->proc_sem);
        osal_sema_destory(&drv_ctx->pro_info_sem);

        osal_wait_destory(&drv_ctx->isp_wait);
        osal_wait_destory(&drv_ctx->isp_wait_vd_start);
        osal_wait_destory(&drv_ctx->isp_wait_vd_end);
        osal_wait_destory(&drv_ctx->isp_wait_vd_be_end);
        osal_wait_destory(&drv_ctx->isp_exit_wait);

        sync_task_exit(vi_pipe);
    }
}

hi_s32 isp_drv_get_viproc_regs_attr(hi_vi_pipe vi_pipe, S_VIPROC_REGS_TYPE *viproc_reg[], isp_be_regs_attr *blk_attr)
{
    hi_u8 k, blk_dev, blk_num, striping_num;
    hi_s8 block_id;
    isp_running_mode running_mode;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_be_wo_reg_cfg *isp_be_reg_cfg = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(blk_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    running_mode = drv_ctx->work_mode.running_mode;

    blk_num = isp_drv_get_block_num(vi_pipe);
    blk_num = div_0_to_1(blk_num);
    block_id = isp_drv_get_block_id(vi_pipe, running_mode);
    if (-1 == block_id) {
        isp_err_trace("ISP[%d] Online Mode Pipe Err!\n", vi_pipe);
        return HI_FAILURE;
    }

    blk_dev = (hi_u8)block_id;
    blk_attr->block_dev = blk_dev;
    blk_attr->block_num = blk_num;

    striping_num = (running_mode == ISP_MODE_RUNNING_STRIPING) ? ISP_STRIPING_MAX_NUM : 1;

    switch (running_mode) {
        case ISP_MODE_RUNNING_ONLINE:
            isp_drv_viprocreg_ctx(blk_dev, viproc_reg[blk_dev]);
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_MAX_BE_NUM; k++) {
                isp_drv_viprocreg_ctx(k, viproc_reg[k]);
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE:
        case ISP_MODE_RUNNING_STRIPING:
            isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);
            if (drv_ctx->use_node == HI_NULL) {
                isp_warn_trace("ISP[%d] UseNode is Null!\n", vi_pipe);
                return HI_FAILURE;
            }
            isp_be_reg_cfg = (isp_be_wo_reg_cfg *)drv_ctx->use_node->be_cfg_buf.vir_addr;
            for (k = 0; k < striping_num; k++) {
                viproc_reg[k] = &isp_be_reg_cfg->be_reg_cfg[k].vi_proc_reg;
            }
            break;

        default:
            isp_err_trace("ISP[%d] Running Mode Err!\n", vi_pipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_set_online_stt_addr(hi_vi_pipe vi_pipe, hi_u64 phy_base_addr)
{
    hi_u8   k;
    hi_u8   blk_dev;
    hi_u8   blk_num;
    hi_u32  ret;
    hi_u64  stt_size;
    hi_u64  phy_addr = 0;
    isp_be_regs_attr  be_regs_attr = {0};
    S_VIPROC_REGS_TYPE *viproc_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_pipe_return(vi_pipe);
    ret = isp_drv_get_viproc_regs_attr(vi_pipe, viproc_reg, &be_regs_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Get ViprocRegs Attr Err!\n", vi_pipe);
        return HI_FAILURE;
    }

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;

    stt_size = sizeof(S_ISP_ONLINE_STT_REGS_TYPE);

    for (k = 0; k < blk_num; k++) {
        phy_addr = phy_base_addr + k * stt_size;

        viproc_reg[k + blk_dev]->VIPROC_ISP_AE_HIST_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_AE_HIST_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_AE_AVER_R_GR_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_AE_AVER_R_GR_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_AE_AVER_GB_B_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_AE_AVER_GB_B_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_AWB_STAT_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_AWB_STAT_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_AF_STAT_ADDR_LOW.u32  =
            get_low_addr(phy_addr + ISP_AF_STAT_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_LA_AVER_ADDR_LOW.u32  =
            get_low_addr(phy_addr + ISP_LA_AVER_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_DEHAZE_MIN_STAT_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_DEHAZE_MIN_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_DEHAZE_MAX_STAT_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_DEHAZE_MAX_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_LDCI_LPF_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_LDCI_LPF_MAP_ADDR_OFFSET);

        viproc_reg[k + blk_dev]->VIPROC_ISP_FLICK_GMEAN_ADDR_LOW.u32 =
            get_low_addr(phy_addr + ISP_FLICK_GMEAN_ADDR_OFFSET);
    }

    return HI_SUCCESS;
}

hi_void isp_drv_set_fe_stt_addr(hi_vi_pipe vi_pipe_bind, hi_u64 phy_addr)
{
    S_ISP_VICAP_CH_REGS_TYPE *fe_stt_addr_reg = HI_NULL;

    isp_drv_festtreg_ctx(vi_pipe_bind, fe_stt_addr_reg);

    fe_stt_addr_reg->CH_WCH_STT_AE_HIST_ADDR_L.u32      = get_low_addr(phy_addr + ISP_AE_HIST_ADDR_OFFSET);

    fe_stt_addr_reg->CH_WCH_STT_AE_AVER_R_GR_ADDR_L.u32 = get_low_addr(phy_addr + ISP_AE_AVER_R_GR_ADDR_OFFSET);

    fe_stt_addr_reg->CH_WCH_STT_AE_AVER_GB_B_ADDR_L.u32 = get_low_addr(phy_addr + ISP_AE_AVER_GB_B_ADDR_OFFSET);

    return ;
}

/* just called by isp_drv_reg_config_chn_sel */
static hi_void isp_drv_reg_config_chn_sel_subfunc0(hi_u32 channel_sel,
                                                   isp_drv_ctx *drv_ctx,  hi_u32 *chn_switch)
{
    switch (channel_sel & 0x3) {
        case ISP_CHN_SWITCH_NORMAL:
            chn_switch[0] = (drv_ctx->sync_cfg.vc_num_max - drv_ctx->sync_cfg.vc_num) %
                            div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
            chn_switch[1] = (chn_switch[0] + 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 1 */
            chn_switch[2] = (chn_switch[0] + 2) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 2 */
            chn_switch[3] = (chn_switch[0] + 3) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 3 */
            break;

        case ISP_CHN_SWITCH_2LANE:
            chn_switch[1] = (drv_ctx->sync_cfg.vc_num_max - drv_ctx->sync_cfg.vc_num) %
                            div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
            chn_switch[0] = (chn_switch[1] + 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 0 & 1 */
            chn_switch[2] = (chn_switch[1] + 2) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 2 & 1 */
            chn_switch[3] = (chn_switch[1] + 3) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 3 & 1 */
            break;

        case ISP_CHN_SWITCH_3LANE:
            chn_switch[2] = (drv_ctx->sync_cfg.vc_num_max - drv_ctx->sync_cfg.vc_num) %  /* index 2 */
                            div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
            chn_switch[1] = (chn_switch[2] + 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 1 & 2 */
            chn_switch[0] = (chn_switch[2] + 2) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 0 & 2 */
            chn_switch[3] = (chn_switch[2] + 3) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 3 & 2 */
            break;

        case ISP_CHN_SWITCH_4LANE:
            chn_switch[3] = (drv_ctx->sync_cfg.vc_num_max - drv_ctx->sync_cfg.vc_num) %  /* index 3 */
                            div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
            chn_switch[2] = (chn_switch[3] + 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 2 & 3+1 */
            chn_switch[1] = (chn_switch[3] + 2) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 1 & 3+2 */
            chn_switch[0] = (chn_switch[3] + 3) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* index 0 & 3+3 */
            break;
    }
}

static hi_void isp_drv_reg_config_chn_sel_subfunc1(hi_u32 channel_sel, isp_drv_ctx *drv_ctx,  hi_u32 *chn_switch)
{
    switch (channel_sel & 0x3) {
        case ISP_CHN_SWITCH_NORMAL:
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0]; /* array index 0 */
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1]; /* array index 1 */
            chn_switch[2] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[2]; /* array index 2 */
            chn_switch[3] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[3]; /* array index 3 */
            break;

        case ISP_CHN_SWITCH_2LANE:
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0]; /* array index 1 */
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1]; /* array index 0 */
            chn_switch[2] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[2]; /* array index 2 */
            chn_switch[3] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[3]; /* array index 3 */
            break;

        case ISP_CHN_SWITCH_3LANE:
            chn_switch[2] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0]; /* array index 2 */
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1]; /* array index 1 */
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[2]; /* array index 0 & 2 */
            chn_switch[3] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[3]; /* array index 3 */
            break;

        case ISP_CHN_SWITCH_4LANE:
            chn_switch[3] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0]; /* array index 3 */
            chn_switch[2] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1]; /* array index 2 */
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[2]; /* array index 1 & 2 */
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[3]; /* array index 0 & 3 */
            break;
    }
}

static __inline hi_s32 isp_drv_reg_config_chn_sel_subfunc2(hi_u8 blk_num,
                                                           S_ISPBE_REGS_TYPE *be_reg[],
                                                           hi_u8 blk_dev,
                                                           hi_u32 *chn_switch)
{
    hi_u8 k;
    for (k = 0; k < blk_num; k++) {
        isp_drv_set_input_sel(be_reg[k + blk_dev], chn_switch);
    }
    return 1;
}

hi_s32 isp_drv_reg_config_chn_sel(S_ISPBE_REGS_TYPE *be_reg[],
                                  isp_drv_ctx *drv_ctx, hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8   k;
    hi_u32  chn_switch[ISP_CHN_SWITCH_NUM]  = {0};
    hi_u32  channel_sel;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
    }

    isp_check_pointer_return(drv_ctx);

    channel_sel = drv_ctx->chn_sel_attr[0].channel_sel;
    chn_switch[4] = (drv_ctx->yuv_mode == HI_TRUE) ? 1 : 0; /* array index 4 */

    if (is_full_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        isp_drv_reg_config_chn_sel_subfunc0(channel_sel, drv_ctx, &chn_switch[0]);
        isp_drv_reg_config_chn_sel_subfunc2(blk_num, be_reg, blk_dev, &chn_switch[0]);
    } else if ((is_line_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) ||
               (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode))) {
        isp_drv_reg_config_chn_sel_subfunc1(channel_sel, drv_ctx, &chn_switch[0]);

        /* offline mode: isp BE buffer poll, so chn switch need each frame refres */
        if (is_offline_mode(drv_ctx->work_mode.running_mode) ||
            (is_striping_mode(drv_ctx->work_mode.running_mode))) {
            isp_drv_reg_config_chn_sel_subfunc2(blk_num, be_reg, blk_dev, &chn_switch[0]);
        }
    } else {
    }

    return HI_SUCCESS;
}

static hi_void isp_drv_reg_config_pq_ai_wb_gain(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, hi_u32 *wb_dgain)
{
    hi_u8 i;
    isp_drv_ctx *ai_drv_ctx = HI_NULL;
    if ((drv_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == drv_ctx->ai_info.ai_pipe_id)) {
        return;
    }

    if (vi_pipe == drv_ctx->ai_info.base_pipe_id) {
        if (drv_ctx->ai_info.ai_pipe_id < 0 || drv_ctx->ai_info.ai_pipe_id >= ISP_MAX_PIPE_NUM) {
            isp_err_trace("Err ai pipe %d!\n", drv_ctx->ai_info.ai_pipe_id);
            return;
        }

        ai_drv_ctx = isp_drv_get_ctx(drv_ctx->ai_info.ai_pipe_id);
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            ai_drv_ctx->be_sync_para.wb_gain[i] = wb_dgain[i];
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        drv_ctx->be_sync_para.wb_gain[i] = wb_dgain[i];
    }
}

hi_void isp_drv_reg_config_sync_awb(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, hi_u32 *be_white_balance_gain)
{
    hi_u8 i;
    hi_u32 wb_gain[ISP_BAYER_CHN_NUM] = { 0 };

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        wb_gain[i] = be_white_balance_gain[i];
    }

    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            wb_gain[i] = drv_ctx->snap_info_load.isp_cfg_info.white_balance_gain[i];
        }
    }

    isp_drv_reg_config_pq_ai_wb_gain(vi_pipe, drv_ctx, wb_gain);
}

static hi_void isp_drv_online_be_blc_reg(S_ISPBE_REGS_TYPE *be_reg[], isp_be_blc_dyna_cfg *be_blc_reg_cfg,
                                         isp_be_regs_attr *be_regs_attr)
{
    hi_u8 k, blk_dev, blk_num;

    isp_check_pointer_void_return(be_regs_attr);
    isp_check_pointer_void_return(be_blc_reg_cfg);

    blk_num = be_regs_attr->block_num;
    blk_dev = be_regs_attr->block_dev;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_void_return(be_reg[k + blk_dev]);
        /* 4dg */
        isp_drv_be_f0_4dg_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->wdr_dg_blc[0].blc); /* wdr0 */
        isp_drv_be_f1_4dg_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->wdr_dg_blc[1].blc); /* wdr1 */

        /* wdr */
        isp_drv_wdr_out_blc_write(be_reg[k + blk_dev], be_blc_reg_cfg->wdr_blc[0].out_blc);
        isp_drv_be_f0_wdr_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->wdr_blc[0].blc); /* wdr0 */
        isp_drv_be_f1_wdr_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->wdr_blc[1].blc); /* wdr1 */

        /* flicker */
        isp_drv_be_f0_flick_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->flicker_blc[0].blc); /* wdr0 */
        isp_drv_be_f1_flick_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->flicker_blc[1].blc); /* wdr1 */
        isp_drv_be_expander_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->expander_blc.blc); /* expander */
        isp_drv_be_bnr_rlmt_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->bnr_blc.blc[0] >> 2); /* shift 2bits */
        isp_drv_be_lsc_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->lsc_blc.blc); /* lsc */
        isp_drv_be_dgain_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->dg_blc.blc); /* isp_dgain */
        isp_drv_be_ae_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->ae_blc.blc); /* ae */
        isp_drv_be_la_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->mg_blc.blc); /* la */
        isp_drv_be_wb_offset_write(be_reg[k + blk_dev], be_blc_reg_cfg->wb_blc.blc); /* wb */
        isp_drv_be_bnr_de_blcvalue_write(be_reg[k + blk_dev], be_blc_reg_cfg->de_blc.blc[0]);
    }
}

hi_s32 isp_drv_reg_config_be_blc(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_drv_ctx *drv_ctx,
                                 isp_sync_cfg_buf_node *cfg_node, isp_be_regs_attr *be_regs_attr)
{
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(drv_ctx);
    isp_check_pointer_return(be_regs_attr);
    isp_check_pointer_return(cfg_node);

    if (cfg_node->be_blc_reg_cfg.resh_dyna != HI_TRUE) {
        return HI_SUCCESS;
    }

    if (is_online_mode(drv_ctx->work_mode.running_mode) ||
        is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        isp_drv_online_be_blc_reg(be_reg, &cfg_node->be_blc_reg_cfg, be_regs_attr);
    } else {
        osal_memcpy(&drv_ctx->be_sync_para.be_blc, &cfg_node->be_blc_reg_cfg, sizeof(isp_be_blc_dyna_cfg));
    }

    cfg_node->wdr_reg_cfg.offset0 = cfg_node->be_blc_reg_cfg.wdr_blc[0].blc[1];

    return HI_SUCCESS;
}

static hi_void isp_drv_set_fe_blc_reg(S_ISPFE_REGS_TYPE *fe_reg, isp_fe_blc_dyna_cfg *fe_blc_reg_cfg)
{
    isp_check_pointer_void_return(fe_reg);
    isp_check_pointer_void_return(fe_blc_reg_cfg);

    if (fe_blc_reg_cfg->resh_dyna == HI_TRUE) {
        isp_drv_fe_dg_offset_write(fe_reg, fe_blc_reg_cfg->fe_dg_blc.blc);
        isp_drv_fe_wb_offset_write(fe_reg, fe_blc_reg_cfg->fe_wb_blc.blc);
        isp_drv_fe_ae_offset_write(fe_reg, fe_blc_reg_cfg->fe_ae_blc.blc);
        isp_drv_fe_blc_offset_write(fe_reg, fe_blc_reg_cfg->fe_blc.blc);
    }
}

hi_s32 isp_drv_reg_config_fe_blc(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_u8 k, fe_cfg_node_idx, cfg_node_vc, chn_num_max;
    hi_vi_pipe vi_pipe_bind;
    S_ISPFE_REGS_TYPE     *fe_reg   = HI_NULL;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_vir_pipe_return(vi_pipe);
    isp_check_pointer_return(drv_ctx);

    if (drv_ctx->wdr_attr.mast_pipe == HI_FALSE) {
        return HI_SUCCESS;
    }

    fe_cfg_node_idx = isp_drv_get_fe_sync_index(drv_ctx);
    fe_cfg_node_idx = MIN2(fe_cfg_node_idx, CFG2VLD_DLY_LIMIT - 1);

    if (IS_WDR_MODE(drv_ctx->sync_cfg.wdr_mode)) {
        cfg_node_vc  = (drv_ctx->sync_cfg.cfg2_vld_dly_max - 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
    } else {
        cfg_node_vc  = 0;
    }

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    cfg_node    = drv_ctx->sync_cfg.node[fe_cfg_node_idx];

    if (cfg_node != HI_NULL) {
        if (drv_ctx->sync_cfg.vc_cfg_num == cfg_node_vc) {
            for (k = 0; k < chn_num_max; k++) {
                vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
                isp_check_pipe_return(vi_pipe_bind);
                isp_drv_fereg_ctx(vi_pipe_bind, fe_reg);
                isp_drv_set_fe_blc_reg(fe_reg, &cfg_node->fe_blc_reg_cfg);
            }
        }
    }

    if (drv_ctx->sync_cfg.node[0] != HI_NULL) {
        if (drv_ctx->sync_cfg.vc_cfg_num == cfg_node_vc) {
            isp_drv_reg_config_sync_awb(vi_pipe, drv_ctx, drv_ctx->sync_cfg.node[0]->awb_reg_cfg.be_white_balance_gain);
        }
    }
    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_fe_dgain(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_u8 k, fe_cfg_node_idx, cfg_node_vc, chn_num_max;
    hi_vi_pipe vi_pipe_bind;
    hi_u32 wdr_gain;
    S_ISPFE_REGS_TYPE     *fe_reg   = HI_NULL;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(drv_ctx);

    if (drv_ctx->wdr_attr.mast_pipe == HI_FALSE) {
        return HI_SUCCESS;
    }

    fe_cfg_node_idx = isp_drv_get_fe_sync_index(drv_ctx);
    fe_cfg_node_idx = MIN2(fe_cfg_node_idx, CFG2VLD_DLY_LIMIT - 1);

    if (IS_WDR_MODE(drv_ctx->sync_cfg.wdr_mode)) {
        cfg_node_vc  = (drv_ctx->sync_cfg.cfg2_vld_dly_max - 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
    } else {
        cfg_node_vc  = 0;
    }

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    cfg_node    = drv_ctx->sync_cfg.node[fe_cfg_node_idx];

    if (cfg_node != HI_NULL) {
        if (drv_ctx->sync_cfg.vc_cfg_num == cfg_node_vc) {
            for (k = 0; k < chn_num_max; k++) {
                vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
                isp_check_pipe_return(vi_pipe_bind);
                isp_drv_fereg_ctx(vi_pipe_bind, fe_reg);

                wdr_gain = drv_ctx->sync_cfg.wdr_gain[k][fe_cfg_node_idx];
                wdr_gain = (cfg_node->ae_reg_cfg.isp_dgain * wdr_gain) >> 0x8;
                wdr_gain = CLIP3(wdr_gain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
                isp_drv_set_fe_dgain(fe_reg, wdr_gain);
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_be_regup(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}

static hi_void isp_drv_regs_wdr_2to1(S_ISPBE_REGS_TYPE *be_reg_offset, isp_drv_ctx *drv_ctx,
                                     isp_fswdr_sync_cfg *wdr_reg_cfg, hi_u32 *ratio, hi_u32 *expo_value0)
{
    hi_s32  max_ratio;
    hi_u32  expo_value[EXP_RATIO_NUM]  = {0};
    hi_u32  blc_comp[EXP_RATIO_NUM]    = {0};

    *expo_value0 = expo_value[0];

    if (is_2to1_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        isp_drv_set_wdr_exporratio0(be_reg_offset,
            MIN2((isp_bitmask(10) * 64 / div_0_to_1(ratio[0])), 0x3FF)); /* ratio precision is 64, and const 10 */
        isp_drv_set_flick_exporatio0(be_reg_offset, MIN2(ratio[0], 0X3FFF));

        drv_ctx->be_sync_para.wdr_exp_ratio   =
            MIN2((isp_bitmask(10) * 64 / div_0_to_1(ratio[0])), 0x3FF); /* ratio precision is 64, and const 10 */
        drv_ctx->be_sync_para.flick_exp_ratio = MIN2(ratio[0], 0X3FFF);

        expo_value[0] = MIN2(ratio[0], isp_bitmask(14)); /* const value 14 */
        expo_value[1] = MIN2(64, isp_bitmask(14)); /* ratio precision is 64, and const 14 */

        *expo_value0 = expo_value[0];

        if (wdr_reg_cfg->fusion_mode == 0) {
            isp_drv_set_wdr_expo_value0(be_reg_offset, expo_value[0]);
            isp_drv_set_wdr_expo_value1(be_reg_offset, expo_value[1]);

            drv_ctx->be_sync_para.wdr_exp_val[0] = expo_value[0];
            drv_ctx->be_sync_para.wdr_exp_val[1] = expo_value[1];

            max_ratio = ((1 << 22) - 1) / div_0_to_1(expo_value[0]); /* left shift 22 */
        } else {
            isp_drv_set_wdr_expo_value0(be_reg_offset,
                MIN2((expo_value[0] + 64), isp_bitmask(14))); /* ratio precision is 64, and const 14 */
            isp_drv_set_wdr_expo_value1(be_reg_offset, expo_value[1]);

            drv_ctx->be_sync_para.wdr_exp_val[0] =
                MIN2((expo_value[0] + 64), isp_bitmask(14)); /* ratio precision is 64, and const 14 */
            drv_ctx->be_sync_para.wdr_exp_val[1] = expo_value[1];

            max_ratio = ((1 << 22) - 1) / div_0_to_1(expo_value[0] + 64); /* precision 64, left shift 22 */
        }

        blc_comp[0] = (expo_value[0] - expo_value[1]) * wdr_reg_cfg->offset0;

        isp_drv_set_wdr_blc_comp0(be_reg_offset, blc_comp[0]);
        drv_ctx->be_sync_para.wdr_blc_comp = blc_comp[0];

        isp_drv_set_wdr_max_ratio(be_reg_offset, max_ratio);
        drv_ctx->be_sync_para.wdr_max_ratio = max_ratio;

        isp_drv_set_wdr_fusion_mode(be_reg_offset, wdr_reg_cfg->fusion_mode);
        drv_ctx->be_sync_para.fusion_mode = wdr_reg_cfg->fusion_mode;
    }
}

hi_s32 isp_drv_reg_config_wdr(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
                              isp_fswdr_sync_cfg *wdr_reg_cfg, isp_be_regs_attr *be_regs_attr, hi_u32 *ratio)
{
    hi_u8   k, lf_mode;
    hi_u16  long_thr, short_thr;
    hi_bool wdr_mdt_en;
    hi_u8   cfg_dly_max;
    hi_u32  expo_value0;
    isp_drv_ctx *drv_ctx = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg_offset = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(wdr_reg_cfg);
    isp_check_pointer_return(be_regs_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (k = 0; k < be_regs_attr->block_num; k++) {
        isp_check_pointer_return(be_reg[k + be_regs_attr->block_dev]);
    }

    cfg_dly_max = isp_drv_get_be_sync_index(drv_ctx);
    cfg_dly_max = MIN2(cfg_dly_max, CFG2VLD_DLY_LIMIT - 1);
    lf_mode = drv_ctx->sync_cfg.lf_mode[cfg_dly_max];

    long_thr   = wdr_reg_cfg->long_thr;
    short_thr  = wdr_reg_cfg->short_thr;
    wdr_mdt_en = wdr_reg_cfg->wdr_mdt_en;

    for (k = 0; k < be_regs_attr->block_num; k++) {
        be_reg_offset = be_reg[k + be_regs_attr->block_dev];
        isp_drv_regs_wdr_2to1(be_reg_offset, drv_ctx, wdr_reg_cfg, ratio, &expo_value0);

        if ((lf_mode != 0) && (expo_value0 < 0x44)) {
            long_thr   = 0xFFF;
            short_thr  = 0xFFF;
            wdr_mdt_en = 0;
        }

        isp_drv_set_wdr_long_thr(be_reg_offset,  long_thr);
        isp_drv_set_wdr_short_thr(be_reg_offset, short_thr);
        isp_drv_set_wdr_mdt_en(be_reg_offset,    wdr_mdt_en);
    }

    drv_ctx->be_sync_para.long_thr  = long_thr;
    drv_ctx->be_sync_para.short_thr = short_thr;
    drv_ctx->be_sync_para.wdr_mdt_en  = wdr_mdt_en;

    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_de(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_sync_cfg_buf_node *cfg_node)
{
    hi_bool nry0_en, de_en;
    hi_u8   k;
    hi_u8   blk_dev, blk_num;
    hi_s32  ret;
    isp_be_regs_attr  be_regs_attr = {0};
    S_VIPROC_REGS_TYPE *viproc_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(cfg_node);

    ret = isp_drv_get_viproc_regs_attr(vi_pipe, viproc_reg, &be_regs_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Get ViprocRegs Attr Err!\n", vi_pipe);
        return HI_FAILURE;
    }

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;

    de_en   = cfg_node->de_reg_cfg.de_enable;
    nry0_en = HI_FALSE;

    if (ckfn_vi_get_pipe_nrx_status()) {
        ret = call_vi_get_pipe_nrx_status(vi_pipe, &nry0_en);
        if (ret != HI_SUCCESS) {
            isp_err_trace("Pipe:%d call_vi_get_pipe_nrx_status failed 0x%x!\n", vi_pipe, ret);

            for (k = 0; k < blk_num; k++) {
                isp_check_pointer_return(be_reg[k + blk_dev]);
                be_reg[k + blk_dev]->ISP_BNR_DETAIL.bits.isp_bnr_de_enable   = de_en;
                viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL1.bits.isp_de_en   = de_en;
                viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL1.bits.isp_nry0_en = HI_FALSE;
                viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL0.bits.isp_bnr_en  = cfg_node->de_reg_cfg.bnr_enable;
            }

            return ret;
        }

        if (nry0_en == HI_TRUE) {
            de_en = HI_FALSE;
        }
    }

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
        be_reg[k + blk_dev]->ISP_BNR_DETAIL.bits.isp_bnr_de_enable   = de_en;
        viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL1.bits.isp_de_en   = de_en;
        viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL1.bits.isp_nry0_en = nry0_en;
        viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL0.bits.isp_bnr_en  = cfg_node->de_reg_cfg.bnr_enable;
    }

    return HI_SUCCESS;
}

static hi_void isp_drv_reg_config_pq_ai_ldci(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, hi_u32 ldci_comp)
{
    isp_drv_ctx *ai_drv_ctx = HI_NULL;

    if (vi_pipe == drv_ctx->ai_info.base_pipe_id) {
        if (drv_ctx->ai_info.ai_pipe_id < 0 || drv_ctx->ai_info.ai_pipe_id >= ISP_MAX_PIPE_NUM) {
            isp_err_trace("Err ai pipe %d!\n", drv_ctx->ai_info.ai_pipe_id);
            return;
        }

        ai_drv_ctx = isp_drv_get_ctx(drv_ctx->ai_info.ai_pipe_id);
        ai_drv_ctx->be_sync_para.ldci_comp = ldci_comp;
    }
}

hi_s32 isp_drv_reg_config_ldci(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_drv_ctx *drv_ctx,
                               hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8   k;
    hi_u32  ldci_comp;
    hi_u32  ldci_comp_index;

    isp_check_pointer_return(drv_ctx);
    if ((drv_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == drv_ctx->ai_info.ai_pipe_id)) {
        return HI_SUCCESS;
    }

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
    }

    ldci_comp_index = isp_drv_get_be_sync_index(drv_ctx);
    if (ldci_comp_index >= 1) {
        ldci_comp_index = ldci_comp_index - 1; /* ldci compensate is earlier tham drc one frame */
    } else {
        ldci_comp_index = 0;
    }
    ldci_comp_index = MIN2(ldci_comp_index, CFG2VLD_DLY_LIMIT - 1);

    ldci_comp = drv_ctx->sync_cfg.drc_comp[ldci_comp_index];
    ldci_comp = sqrt32(ldci_comp << DRC_COMP_SHIFT);
    ldci_comp = MIN2(ldci_comp, 0xFFFF);
    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        ldci_comp = 0x1000;
    }

    for (k = 0; k < blk_num; k++) {
        isp_drv_set_ldci_stat_evratio(be_reg[k + blk_dev], ldci_comp);
    }

    drv_ctx->be_sync_para.ldci_comp = ldci_comp;
    if (drv_ctx->ai_info.pq_ai_en == HI_TRUE) {
        isp_drv_reg_config_pq_ai_ldci(vi_pipe, drv_ctx, ldci_comp);
    }

    return HI_SUCCESS;
}

static hi_void isp_drv_calc_drc_exp_ratio(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, hi_u32 *exp_ratio)
{
    hi_u8 cfg_dly_max;
    hi_u32 drc_exp_ratio;
    isp_drv_ctx *base_drv_ctx = HI_NULL;

    cfg_dly_max = isp_drv_get_be_sync_index(drv_ctx);
    cfg_dly_max = MIN2(cfg_dly_max, CFG2VLD_DLY_LIMIT - 1);

    if ((drv_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == drv_ctx->ai_info.ai_pipe_id)) {
        if (drv_ctx->ai_info.base_pipe_id < 0 || drv_ctx->ai_info.base_pipe_id >= ISP_MAX_PIPE_NUM) {
            isp_err_trace("Err base pipe %d!\n", drv_ctx->ai_info.base_pipe_id);
            return;
        }
        base_drv_ctx = isp_drv_get_ctx(drv_ctx->ai_info.base_pipe_id);
        drc_exp_ratio = base_drv_ctx->sync_cfg.drc_comp[cfg_dly_max];
    } else {
        drc_exp_ratio = drv_ctx->sync_cfg.drc_comp[cfg_dly_max];
    }

    if (drc_exp_ratio != 0x1000) { /* do division only when drc_exp_ratio != 4096 */
        drc_exp_ratio = div_0_to_1(drc_exp_ratio);
        drc_exp_ratio = osal_div_u64((1 << (DRC_COMP_SHIFT + DRC_COMP_SHIFT)), drc_exp_ratio);
        drc_exp_ratio = MIN2(drc_exp_ratio, (15 << DRC_COMP_SHIFT)); /* Maximum supported ratio is 15 */
    }

    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        drc_exp_ratio = 0x1000;
    }

    *exp_ratio = drc_exp_ratio;
}

hi_void isp_drv_calc_drc_prev_luma(isp_drc_sync_cfg *drc_reg_cfg, hi_bool update_log_param,
                                   hi_u8 drc_shp_log, hi_u32 drc_exp_ratio, hi_u32 *drc_prev_luma)
{
    hi_u32 i;

    /* Compensate on PrevLuma when ShpLog/ShpExp is modified, but no compensation under offline repeat mode */
    if (update_log_param && (!drc_reg_cfg->is_offline_repeat_mode)) {
        for (i = 0; i < SYNC_DRC_PRELUMA_NUM - 1; i++) {
            drc_prev_luma[i] =
                (hi_u32)((hi_s32)g_drc_cur_luma_lut[drc_shp_log][i] + drc_reg_cfg->prev_luma_delta[i]);
        }
    } else {
        for (i = 0; i < SYNC_DRC_PRELUMA_NUM - 1; i++) {
            drc_prev_luma[i] = g_drc_cur_luma_lut[drc_shp_log][i];
        }
    }
    drc_prev_luma[SYNC_DRC_PRELUMA_NUM - 1] = (1 << 20); /* left shift 20 */

    if ((drc_exp_ratio != 0x1000) && (!drc_reg_cfg->is_offline_repeat_mode)) {
        for (i = 0; i < SYNC_DRC_PRELUMA_NUM; i++) {
            drc_prev_luma[i] = (hi_u32)(((hi_u64)drc_exp_ratio * drc_prev_luma[i]) >> DRC_COMP_SHIFT);
        }
    }
}

hi_s32 isp_drv_reg_config_drc(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
                              isp_drc_sync_cfg *drc_reg_cfg, isp_be_regs_attr *be_regs_attr, hi_u32 *ratio)
{
    hi_u8  i, k;
    hi_u8  blk_num, blk_dev;
    hi_u32 drc_div_denom_log, drc_denom_exp;
    hi_u32 drc_exp_ratio = 0x1000;
    hi_u32 drc_prev_luma[SYNC_DRC_PRELUMA_NUM] = {0};
    hi_bool update_log_param;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pointer_return(drc_reg_cfg);
    isp_check_pointer_return(ratio);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    blk_num = be_regs_attr->block_num;
    blk_dev = be_regs_attr->block_dev;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
    }

    isp_drv_calc_drc_exp_ratio(vi_pipe, drv_ctx, &drc_exp_ratio);

    update_log_param = (g_drc_shp_log[vi_pipe][blk_dev] != drc_reg_cfg->shp_log ||
                        g_drc_shp_exp[vi_pipe][blk_dev] != drc_reg_cfg->shp_exp) ? HI_TRUE : HI_FALSE;

    g_drc_shp_log[vi_pipe][blk_dev] = drc_reg_cfg->shp_log;
    g_drc_shp_exp[vi_pipe][blk_dev] = drc_reg_cfg->shp_exp;

    isp_drv_calc_drc_prev_luma(drc_reg_cfg, update_log_param,
                               g_drc_shp_log[vi_pipe][blk_dev], drc_exp_ratio, drc_prev_luma);

    drc_div_denom_log = g_drc_div_denom_log[g_drc_shp_log[vi_pipe][blk_dev]];
    drc_denom_exp    = g_drc_denom_exp[g_drc_shp_exp[vi_pipe][blk_dev]];

    for (k = 0; k < blk_num; k++) {
        isp_drv_set_drc_shp_cfg(be_reg[k + blk_dev], g_drc_shp_log[vi_pipe][blk_dev], g_drc_shp_exp[vi_pipe][blk_dev]);
        isp_drv_set_drc_div_denom_log(be_reg[k + blk_dev], drc_div_denom_log);
        isp_drv_set_drc_denom_exp(be_reg[k + blk_dev], drc_denom_exp);

        isp_drv_set_drc_prev_luma0(be_reg[k + blk_dev], drc_prev_luma[0]); /* array index 0 */
        isp_drv_set_drc_prev_luma1(be_reg[k + blk_dev], drc_prev_luma[1]); /* array index 1 */
        isp_drv_set_drc_prev_luma2(be_reg[k + blk_dev], drc_prev_luma[2]); /* array index 2 */
        isp_drv_set_drc_prev_luma3(be_reg[k + blk_dev], drc_prev_luma[3]); /* array index 3 */
        isp_drv_set_drc_prev_luma4(be_reg[k + blk_dev], drc_prev_luma[4]); /* array index 4 */
        isp_drv_set_drc_prev_luma5(be_reg[k + blk_dev], drc_prev_luma[5]); /* array index 5 */
        isp_drv_set_drc_prev_luma6(be_reg[k + blk_dev], drc_prev_luma[6]); /* array index 6 */
        isp_drv_set_drc_prev_luma7(be_reg[k + blk_dev], drc_prev_luma[7]); /* array index 7 */
    }

    for (i = 0; i < SYNC_DRC_PRELUMA_NUM; i++) {
        drv_ctx->be_sync_para.drc_prev_luma[i] = drc_prev_luma[i];
    }
    drv_ctx->be_sync_para.drc_shp_log       = g_drc_shp_log[vi_pipe][blk_dev];
    drv_ctx->be_sync_para.drc_div_denom_log = drc_div_denom_log;
    drv_ctx->be_sync_para.drc_denom_exp     = drc_denom_exp;

    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_dgain(S_ISPBE_REGS_TYPE *be_reg[], hi_u32  isp_dgain, hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8 k;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
        isp_drv_set_be_dgain(be_reg[k + blk_dev], isp_dgain);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_config_ccm(isp_drv_ctx *drv_ctx, hi_u16 *ccm)
{
    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config4_dgain(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
                                 isp_drv_ctx *drv_ctx, isp_sync_4dgain_cfg *sync_4dgain_cfg)
{
    hi_s32 ret;
    hi_u8 k, blk_num, blk_dev;
    hi_u32 *wdr_gain = HI_NULL;
    isp_be_regs_attr  be_regs_attr = {0};
    S_VIPROC_REGS_TYPE *viproc_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    ret = isp_drv_get_viproc_regs_attr(vi_pipe, viproc_reg, &be_regs_attr);
    isp_check_ret(vi_pipe, ret, "isp_drv_get_viproc_regs_attr");

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;
    wdr_gain = sync_4dgain_cfg->wdr_gain;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(viproc_reg[k + blk_dev]);

        viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL0.bits.isp_4dg_en = 0;
        if (is_fs_wdr_mode(drv_ctx->wdr_attr.wdr_mode)) {
            viproc_reg[k + blk_dev]->VIPROC_ISPBE_CTRL0.bits.isp_4dg_en = 1;
        }

        isp_check_pointer_return(be_reg[k + blk_dev]);
        isp_drv_set_isp_4dgain0(be_reg[k + blk_dev], wdr_gain[0]);
        isp_drv_set_isp_4dgain1(be_reg[k + blk_dev], wdr_gain[1]);
    }

    drv_ctx->be_sync_para.wdr_gain[0] = wdr_gain[0];
    drv_ctx->be_sync_para.wdr_gain[1] = wdr_gain[1];

    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_awb_en(hi_vi_pipe vi_pipe, hi_bool awb_en)
{
    hi_s32 ret;
    hi_u8 k;
    isp_be_regs_attr  be_regs_attr = {0};
    S_VIPROC_REGS_TYPE *viproc_reg[ISP_STRIPING_MAX_NUM] = {HI_NULL};

    ret = isp_drv_get_viproc_regs_attr(vi_pipe, viproc_reg, &be_regs_attr);
    isp_check_ret(vi_pipe, ret, "isp_drv_get_viproc_regs_attr");

    for (k = 0; k < be_regs_attr.block_num; k++) {
        isp_check_pointer_return(viproc_reg[k + be_regs_attr.block_dev]);
        isp_drv_awb_enable(viproc_reg[k + be_regs_attr.block_dev], awb_en);
    }

    return HI_SUCCESS;
}

/* read FE statistics information */
hi_s32 isp_drv_fe_ae_global_statistics_read(isp_stat *stat,
    S_ISP_FE_STT_REGS_TYPE *fe_stt_reg, isp_drv_ctx *drv_ctx, hi_u32 k)
{
    hi_s32 i;
    hi_u8 chn_num_max;
    isp_check_pointer_return(stat);
    isp_check_pointer_return(fe_stt_reg);
    isp_check_pointer_return(drv_ctx);

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    if (is_half_wdr_mode(drv_ctx->wdr_attr.wdr_mode)) {
        k = chn_num_max - 1 - k;
    }

    for (i = 0; i < HIST_NUM; i++) {
        stat->fe_ae_stat1.histogram_mem_array[k][i] = fe_stt_reg->ISP_AE_HIST[i].u32;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_ae_local_statistics_read(isp_stat *stat,
    S_ISP_FE_STT_REGS_TYPE *fe_stt_reg, isp_drv_ctx *drv_ctx, hi_u32 k)
{
    hi_s32 i, j;
    hi_u32 ave_mem = 0;
    hi_u8 chn_num_max;
    isp_check_pointer_return(stat);
    isp_check_pointer_return(fe_stt_reg);
    isp_check_pointer_return(drv_ctx);

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    if (is_half_wdr_mode(drv_ctx->wdr_attr.wdr_mode)) {
        k = chn_num_max - 1 - k;
    }

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            ave_mem = fe_stt_reg->ISP_AE_AVER_R_GR[i * AE_ZONE_COLUMN + j].u32;
            stat->fe_ae_stat3.zone_avg[k][i][j][0] = (hi_u16)((ave_mem & 0xFFFF0000) >> 16); /* Rshift 16bit */
            stat->fe_ae_stat3.zone_avg[k][i][j][1] = (hi_u16)((ave_mem & 0xFFFF)); /* index[1], low 16bit */
            ave_mem = fe_stt_reg->ISP_AE_AVER_GB_B[i * AE_ZONE_COLUMN + j].u32;
            stat->fe_ae_stat3.zone_avg[k][i][j][2] = (hi_u16)((ave_mem & 0xFFFF0000) >> 16); /* index[2],Rshift 16bit */
            stat->fe_ae_stat3.zone_avg[k][i][j][3] = (hi_u16)((ave_mem & 0xFFFF)); /* index[3], low 16bit */
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_apb_statistics_read(isp_stat_info *stat_info, S_ISPFE_REGS_TYPE *fe_reg, isp_drv_ctx *drv_ctx,
                                      hi_vi_pipe vi_pipe_bind, hi_u32 k)
{
    hi_u8 chn_num_max;
    isp_stat *stat = HI_NULL;
    isp_stat_key un_statkey;
    isp_check_pipe_return(vi_pipe_bind);
    isp_check_pointer_return(stat_info);
    isp_check_pointer_return(fe_reg);
    isp_check_pointer_return(drv_ctx);

    stat = (isp_stat *)stat_info->virt_addr;
    isp_check_pointer_return(stat);
    un_statkey.key = stat_info->stat_key.bit32_isr_access;

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    if (is_half_wdr_mode(drv_ctx->wdr_attr.wdr_mode)) {
        k = chn_num_max - 1 - k;
    }

    if (un_statkey.bit1_fe_ae_glo_stat) {
        stat->fe_ae_stat1.pixel_weight[k]  = fe_reg->ISP_AE1_COUNT_STAT.u32;
        stat->fe_ae_stat1.pixel_count[k]   = fe_reg->ISP_AE1_TOTAL_STAT.u32;

        stat->fe_ae_stat2.global_avg_r[k]  = fe_reg->ISP_AE1_TOTAL_R_AVER.u32;
        stat->fe_ae_stat2.global_avg_gr[k] = fe_reg->ISP_AE1_TOTAL_GR_AVER.u32;
        stat->fe_ae_stat2.global_avg_gb[k] = fe_reg->ISP_AE1_TOTAL_GB_AVER.u32;
        stat->fe_ae_stat2.global_avg_b[k]  = fe_reg->ISP_AE1_TOTAL_B_AVER.u32;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_af_stt_statistics_read(isp_stat *stat,
                                         S_ISPFE_REGS_TYPE *fe_reg, S_ISP_FE_STT_REGS_TYPE *fe_stt_reg,
                                         hi_vi_pipe vi_pipe_bind, hi_u32 k)
{
    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_switch_stt_addr(hi_vi_pipe vi_pipe_bind, hi_u8 wdr_index, isp_drv_ctx *drv_ctx)
{
    hi_u32  cur_read_flag;
    isp_check_pipe_return(vi_pipe_bind);
    isp_check_pointer_return(drv_ctx);

    if ((drv_ctx->fe_stt_attr.fe_stt[wdr_index].cur_write_flag != 0) &&
        (drv_ctx->fe_stt_attr.fe_stt[wdr_index].cur_write_flag != 1)) {
        isp_err_trace("Err FE u32CurWriteFlag != 0/1 !!!\n");
        return HI_FAILURE;
    }

    cur_read_flag = drv_ctx->fe_stt_attr.fe_stt[wdr_index].cur_write_flag;

    if (drv_ctx->fe_stt_attr.fe_stt[wdr_index].first_frame == HI_TRUE) {
        osal_memcpy(drv_ctx->fe_stt_attr.fe_stt[wdr_index].fe_stt_buf[1].vir_addr,
                    drv_ctx->fe_stt_attr.fe_stt[wdr_index].fe_stt_buf[0].vir_addr, sizeof(S_ISP_FE_STT_REGS_TYPE));

        cur_read_flag = 1;
        drv_ctx->fe_stt_attr.fe_stt[wdr_index].first_frame = HI_FALSE;
    }

    drv_ctx->fe_stt_attr.fe_stt[wdr_index].cur_write_flag = 1 - cur_read_flag;

    isp_drv_set_fe_stt_addr(vi_pipe_bind, drv_ctx->fe_stt_attr.fe_stt[wdr_index].fe_stt_buf[cur_read_flag].phy_addr);

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_stt_statistics_read(isp_stat_info *stat_info, S_ISPFE_REGS_TYPE *fe_reg,
                                      isp_drv_ctx *drv_ctx, hi_vi_pipe vi_pipe_bind, hi_u32 k)
{
    hi_s32 ret;
    hi_u32 cur_read_flag;
    S_ISP_FE_STT_REGS_TYPE *fe_stt_reg = HI_NULL;
    isp_stat *stat = HI_NULL;
    isp_stat_key un_statkey;

    isp_check_pipe_return(vi_pipe_bind);
    isp_check_pointer_return(stat_info);
    isp_check_pointer_return(fe_reg);

    stat = (isp_stat *)stat_info->virt_addr;
    isp_check_pointer_return(stat);

    un_statkey.key = stat_info->stat_key.bit32_isr_access;

    ret = isp_drv_fe_switch_stt_addr(vi_pipe_bind, k, drv_ctx);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp_drv_fe_switch_stt_addr Failed !!!\n");
        return HI_FAILURE;
    }

    cur_read_flag = 1 - drv_ctx->fe_stt_attr.fe_stt[k].cur_write_flag;

    fe_stt_reg = (S_ISP_FE_STT_REGS_TYPE *)drv_ctx->fe_stt_attr.fe_stt[k].fe_stt_buf[cur_read_flag].vir_addr;

    if (un_statkey.bit1_fe_ae_glo_stat) {
        isp_drv_fe_ae_global_statistics_read(stat, fe_stt_reg, drv_ctx, k);
    }

    if (un_statkey.bit1_fe_ae_loc_stat) {
        isp_drv_fe_ae_local_statistics_read(stat, fe_stt_reg, drv_ctx, k);
    }

    if (un_statkey.bit1_fe_af_stat) {
        isp_drv_fe_af_stt_statistics_read(stat, fe_reg, fe_stt_reg, vi_pipe_bind, k);
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32  isp_drv_fe_stitch_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    return HI_SUCCESS;
}
#endif

/* read BE statistics information from phy:online */
hi_s32 isp_drv_be_ae_stt_global_statistics_read(isp_stat *stat,
                                                S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[],
                                                hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u32 i, k;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_online_stt[k + blk_dev]);

        for (i = 0; i < HIST_NUM; i++) {
            if (k == 0) {
                stat->be_ae_stat1.histogram_mem_array[i]  = be_online_stt[k + blk_dev]->ISP_AE_HIST[i].u32;
            } else {
                stat->be_ae_stat1.histogram_mem_array[i] += be_online_stt[k + blk_dev]->ISP_AE_HIST[i].u32;
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_ae_stt_local_statistics_read(isp_stat *stat,
                                               S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[],
                                               hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u32 i, j, k;
    hi_u8  block_offset    = 0;
    hi_u8  block_zone_width = 0;
    hi_u32 ave_mem;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_online_stt[k + blk_dev]);

        for (i = 0; i < AE_ZONE_ROW; i++) {
            if (k < (AE_ZONE_COLUMN % div_0_to_1(blk_num))) {
                block_zone_width = (AE_ZONE_COLUMN / div_0_to_1(blk_num)) + 1;
            } else {
                block_zone_width = AE_ZONE_COLUMN / div_0_to_1(blk_num);
            }

            for (j = 0; j < block_zone_width; j++) {
                ave_mem = be_online_stt[k + blk_dev]->ISP_AE_AVER_R_GR[i * block_zone_width + j].u32;
                stat->be_ae_stat3.zone_avg[i][j + block_offset][0] =
                    (hi_u16)((ave_mem & 0xFFFF0000) >> 16); /* Rshift 16bit */
                stat->be_ae_stat3.zone_avg[i][j + block_offset][1] = /* array index 1 */
                    (hi_u16)((ave_mem & 0xFFFF)); /* low 16bit */
                ave_mem = be_online_stt[k + blk_dev]->ISP_AE_AVER_GB_B[i * block_zone_width + j].u32;
                stat->be_ae_stat3.zone_avg[i][j + block_offset][2] = /* array index 2 */
                    (hi_u16)((ave_mem & 0xFFFF0000) >> 16); /* Rshift 16bit */
                stat->be_ae_stat3.zone_avg[i][j + block_offset][3] = /* array index 3 */
                    (hi_u16)((ave_mem & 0xFFFF)); /* low 16bit */
            }
        }

        block_offset += block_zone_width;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_mg_stt_statistics_read(isp_stat *stat,
                                         S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[],
                                         hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8  k;
    hi_u8  block_offset     = 0;
    hi_u8  block_zone_width  = 0;
    hi_u32 i, j;
    hi_u32 ave_mem;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_online_stt[k + blk_dev]);

        for (i = 0; i < MG_ZONE_ROW; i++) {
            if (k < (MG_ZONE_COLUMN % div_0_to_1(blk_num))) {
                block_zone_width = (MG_ZONE_COLUMN / div_0_to_1(blk_num)) + 1;
            } else {
                block_zone_width = MG_ZONE_COLUMN / div_0_to_1(blk_num);
            }

            for (j = 0; j < block_zone_width; j++) {
                ave_mem = be_online_stt[k + blk_dev]->ISP_LA_AVER[i * block_zone_width + j].u32;
                stat->mg_stat.zone_avg[i][j + block_offset][0] =
                    (hi_u16)((ave_mem & 0xFF000000) >> 24); /* Rshift 24bit */
                stat->mg_stat.zone_avg[i][j + block_offset][1] = /* array index 1 */
                    (hi_u16)((ave_mem & 0xFF0000) >> 16); /* Rshift 16bit */
                stat->mg_stat.zone_avg[i][j + block_offset][2] = /* array index 2 */
                    (hi_u16)((ave_mem & 0xFF00) >> 8); /* Rshift 8bit */
                stat->mg_stat.zone_avg[i][j + block_offset][3] = /* array index 3 */
                    (hi_u16)((ave_mem & 0xFF)); /* low 8bit */
            }
        }

        block_offset += block_zone_width;
    }

    return HI_SUCCESS;
}

hi_void isp_drv_cal_awb_gain_conv(isp_drv_ctx *drv_ctx, hi_u16 *rgb_gain_conv)
{
    hi_s32 i;
    hi_u32 wb_gain_bf_stat[ISP_BAYER_CHN_NUM] = { 0x100, 0x100, 0x100, 0x100 };
    hi_u32 min_gain, norm_gain;
    hi_u16 gain_conv_r, gain_conv_g, gain_conv_b;
    isp_sync_cfg_buf_node *node = HI_NULL;

    node = &(drv_ctx->sync_cfg.sync_cfg_buf.sync_cfg_buf_node[0]);

    if (node->awb_reg_cfg.be_awb_switch == ISP_AWB_AFTER_DRC) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            wb_gain_bf_stat[i] = node->awb_reg_cfg.be_white_balance_gain[i];
        }
    }

    min_gain = 0xFFFF;
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        min_gain = MIN2(min_gain, wb_gain_bf_stat[i]);
    }

    norm_gain = 0x10000 / div_0_to_1(min_gain);
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        wb_gain_bf_stat[i] = (wb_gain_bf_stat[i] * norm_gain + 0x80) >> 8; /* Rshift 8bit */
    }

    gain_conv_r = (0x10000 + (wb_gain_bf_stat[0] >> 1)) / div_0_to_1(wb_gain_bf_stat[0]); /* array index 0 */
    gain_conv_g = (0x10000 + (wb_gain_bf_stat[1] >> 1)) / div_0_to_1(wb_gain_bf_stat[1]); /* array index 1 */
    gain_conv_b = (0x10000 + (wb_gain_bf_stat[3] >> 1)) / div_0_to_1(wb_gain_bf_stat[3]); /* array index 3 */

    rgb_gain_conv[0] = gain_conv_r; /* array index 0 */
    rgb_gain_conv[1] = gain_conv_g; /* array index 1 */
    rgb_gain_conv[2] = gain_conv_b; /* array index 2 */
}

hi_void isp_drv_be_awb_stt_stat2_read(isp_stat *stat, S_ISPBE_REGS_TYPE *be_reg_offset,
    S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt_offset, hi_u32 whole_col, hi_u16 *rgb_gain_conv)
{
    hi_s32 i, j, m;
    hi_u32 zone_bin = 1;
    hi_u32 value, zone, col, row;
    hi_u8  block_offset = 0;
    hi_u16 read_r_avg, read_g_avg, read_avg;
    hi_u16 w_start_addr, r_start_addr;

    zone = be_reg_offset->ISP_AWB_ZONE.u32;
    col  = MIN2((zone & 0x3F), AWB_ZONE_ORIG_COLUMN);
    row  = MIN2(((zone & 0x3F00) >> 8), AWB_ZONE_ORIG_ROW); /* Rshift 8bit */

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            w_start_addr = (i * whole_col + j + block_offset) * zone_bin;
            r_start_addr = (i * whole_col + j) * zone_bin * 2;  /* get array index(*2) */

            for (m = 0; m < zone_bin; m++) {
                value = be_online_stt_offset->ISP_AWB_STAT[r_start_addr + 2 * m + 0].u32; /* index(*2) */
                read_r_avg = (value & 0xFFFF);
                read_g_avg = ((value >> 16) & 0xFFFF); /* shift 16bit, get high 16bit */
                value = be_online_stt_offset->ISP_AWB_STAT[r_start_addr + 2 * m + 1].u32; /* index(*2) */
                read_avg   = (value & 0xFFFF);

                stat->awb_stat2.metering_mem_array_count_all[w_start_addr + m] =
                    ((value >> 16) & 0xFFFF); /* shift 16bit */
                stat->awb_stat2.metering_mem_array_avg_r[w_start_addr + m] =
                    (read_r_avg * rgb_gain_conv[0] + 0x80) >> 8; /* shift 8bit, index[0] */
                stat->awb_stat2.metering_mem_array_avg_g[w_start_addr + m] =
                    (read_g_avg * rgb_gain_conv[1] + 0x80) >> 8; /* shift 8bit, index[1] */
                stat->awb_stat2.metering_mem_array_avg_b[w_start_addr + m] =
                    (read_avg   * rgb_gain_conv[2] + 0x80) >> 8; /* shift 8bit, index[2] */
            }
        }
    }
    block_offset += col;
}

hi_s32 isp_drv_be_awb_stt_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISPBE_REGS_TYPE *be_reg[],
                                          S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[], hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_s32 k;
    hi_u32 zone;
    hi_u32 whole_col  = 0;
    hi_u16 rgb_gain_conv[3] = {0x100, 0x100, 0x100}; /* array max 3 for R,G,B channel */
    S_ISPBE_REGS_TYPE *be_reg_offset = HI_NULL;
    S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt_offset = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
        zone = be_reg[k + blk_dev]->ISP_AWB_ZONE.u32;
        whole_col  += MIN2((zone & 0x3F), AWB_ZONE_ORIG_COLUMN);
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_drv_cal_awb_gain_conv(drv_ctx, rgb_gain_conv);

    for (k = 0; k < blk_num; k++) {
        be_online_stt_offset = be_online_stt[k + blk_dev];
        isp_check_pointer_return(be_online_stt_offset);

        be_reg_offset = be_reg[k + blk_dev];
        isp_check_pointer_return(be_reg_offset);

        isp_drv_be_awb_stt_stat2_read(stat, be_reg_offset, be_online_stt_offset, whole_col, rgb_gain_conv);
    }

    stat->awb_stat1.metering_awb_avg_r =
        (stat->awb_stat1.metering_awb_avg_r * rgb_gain_conv[0] + 0x80) >> 8; /* index[0] shift 8 */
    stat->awb_stat1.metering_awb_avg_g =
        (stat->awb_stat1.metering_awb_avg_g * rgb_gain_conv[1] + 0x80) >> 8; /* index[1] shift 8 */
    stat->awb_stat1.metering_awb_avg_b =
        (stat->awb_stat1.metering_awb_avg_b * rgb_gain_conv[2] + 0x80) >> 8; /* index[2] shift 8 */

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_af_stt_statistics_read(isp_stat *stat,
                                         S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[],
                                         hi_u8 blk_dev, hi_u32 zone)
{
#ifdef CONFIG_HI_ISP_AF_SUPPORT
    hi_u32 i, j;
    hi_u32 be_af_stat_data = 0;
    hi_u32 col = (zone & 0x1F);
    hi_u32 row = ((zone & 0x1F00) >> 8); /* shift 8bit */

    isp_check_pointer_return(stat);
    isp_check_pointer_return(be_online_stt[blk_dev]);

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            be_af_stat_data = be_online_stt[blk_dev]->ISP_AF_STAT[(i * col + j) * 4].u32; /* array index(*4) */
            stat->be_af_stat.zone_metrics[i][j].v1    =
                (hi_u16)((0xFFFF0000 & be_af_stat_data) >> 16); /* shift 16bit, get high 16bit */
            stat->be_af_stat.zone_metrics[i][j].h1    =
                (hi_u16)(0xFFFF & be_af_stat_data);
            be_af_stat_data = be_online_stt[blk_dev]->ISP_AF_STAT[(i * col + j) * 4 + 1].u32; /* array index(*4)+1 */
            stat->be_af_stat.zone_metrics[i][j].v2    =
                (hi_u16)((0xFFFF0000 & be_af_stat_data) >> 16); /* shift 16bit, get high 16bit */
            stat->be_af_stat.zone_metrics[i][j].h2    =
                (hi_u16)(0xFFFF & be_af_stat_data);
            be_af_stat_data = be_online_stt[blk_dev]->ISP_AF_STAT[(i * col + j) * 4 + 2].u32; /* array index(*4)+2 */
            stat->be_af_stat.zone_metrics[i][j].hl_cnt =
                (hi_u16)((0xFFFF0000 & be_af_stat_data) >> 16); /* shift 16bit, get high 16bit */
            stat->be_af_stat.zone_metrics[i][j].y     =
                (hi_u16)(0xFFFF & be_af_stat_data);
        }
    }
#endif
    return HI_SUCCESS;
}

hi_s32 isp_drv_be_dehaze_stt_statistics_read(isp_stat *stat,
                                             S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[],
                                             hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u32 i, j, m;

    isp_check_pointer_return(stat);

    j = DEFOG_ZONE_NUM >> 1;

    for (i = 0; i < blk_num; i++) {
        isp_check_pointer_return(be_online_stt[i + blk_dev]);

        for (m = 0; m < j; m++) {
            stat->dehaze_stat.min_dout[i][m] = be_online_stt[i + blk_dev]->ISP_DEHAZE_MINSTAT[m].u32;
        }

        for (m = 0; m < DEFOG_ZONE_NUM; m++) {
            stat->dehaze_stat.max_stat_dout[i][m] = be_online_stt[i + blk_dev]->ISP_DEHAZE_MAXSTAT[m].u32;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_flicker_stt_statistics_read(isp_stat *stat,
                                              S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[],
                                              hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u16  k, j;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_online_stt[k + blk_dev]);

        for (j = 0; j < ISP_FLICKER_G_MEAN_LENGTH; j++) {
            stat->flicker_stat.g_mean[k][j] = be_online_stt[k + blk_dev]->ISP_FLICK_GMEAN[j].u32;
        }
    }

    return HI_SUCCESS;
}

hi_void isp_drv_be_apb_comm_statistics_read(isp_stat *stat,
                                            S_ISPBE_REGS_TYPE *be_reg[], hi_u8 blk_dev)
{
    stat->comm_stat.white_balance_gain[0] = be_reg[blk_dev]->ISP_WB_GAIN1.bits.isp_wb_rgain;  /* array index[0] */
    stat->comm_stat.white_balance_gain[1] = be_reg[blk_dev]->ISP_WB_GAIN1.bits.isp_wb_grgain; /* array index[1] */
    stat->comm_stat.white_balance_gain[2] = be_reg[blk_dev]->ISP_WB_GAIN2.bits.isp_wb_gbgain; /* array index[2] */
    stat->comm_stat.white_balance_gain[3] = be_reg[blk_dev]->ISP_WB_GAIN2.bits.isp_wb_bgain;  /* array index[3] */
}

hi_void isp_drv_be_apb_flicker_statistics_read(isp_stat *stat,
                                               S_ISPBE_REGS_TYPE *be_reg[],
                                               hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8  k = 0;

    for (k = 0; k < blk_num; k++) {
        stat->flicker_stat.cur_diff_gr[k] = be_reg[k + blk_dev]->ISP_FLICK_GR_DIFF_CUR.bits.isp_flick_gr_diff_cur;
        stat->flicker_stat.cur_diff_gb[k] = be_reg[k + blk_dev]->ISP_FLICK_GB_DIFF_CUR.bits.isp_flick_gb_diff_cur;
        stat->flicker_stat.cur_abs_gr[k]  = be_reg[k + blk_dev]->ISP_FLICK_GR_ABS_CUR.bits.isp_flick_gr_abs_cur;
        stat->flicker_stat.cur_abs_gb[k]  = be_reg[k + blk_dev]->ISP_FLICK_GB_ABS_CUR.bits.isp_flick_gb_abs_cur;
    }
}

hi_void isp_drv_be_apb_ae_statistics_read(isp_stat *stat,
                                          S_ISPBE_REGS_TYPE *be_reg[],
                                          hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8  k = 0;
    hi_u32 pixel_weight;
    hi_u32 pixel_weight_tmp  = 0;
    hi_u64 global_avg_sum_r  = 0;
    hi_u64 global_avg_sum_gr = 0;
    hi_u64 global_avg_sum_gb = 0;
    hi_u64 global_avg_sum_b  = 0;

    for (k = 0; k < blk_num; k++) {
        if (k == 0) {
            pixel_weight_tmp = be_reg[k + blk_dev]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
            stat->be_ae_stat1.pixel_weight = pixel_weight_tmp;
            stat->be_ae_stat1.pixel_count  = be_reg[k + blk_dev]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
        } else {
            pixel_weight_tmp = be_reg[k + blk_dev]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
            stat->be_ae_stat1.pixel_weight += pixel_weight_tmp;
            stat->be_ae_stat1.pixel_count  += be_reg[k + blk_dev]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
        }

        global_avg_sum_r  += ((hi_u64)be_reg[k + blk_dev]->ISP_AE_TOTAL_R_AVER.u32)  * ((hi_u64)pixel_weight_tmp);
        global_avg_sum_gr += ((hi_u64)be_reg[k + blk_dev]->ISP_AE_TOTAL_GR_AVER.u32) * ((hi_u64)pixel_weight_tmp);
        global_avg_sum_gb += ((hi_u64)be_reg[k + blk_dev]->ISP_AE_TOTAL_GB_AVER.u32) * ((hi_u64)pixel_weight_tmp);
        global_avg_sum_b  += ((hi_u64)be_reg[k + blk_dev]->ISP_AE_TOTAL_B_AVER.u32)  * ((hi_u64)pixel_weight_tmp);
    }

    pixel_weight = div_0_to_1(stat->be_ae_stat1.pixel_weight);

    stat->be_ae_stat2.global_avg_r  = osal_div_u64(global_avg_sum_r,  pixel_weight);
    stat->be_ae_stat2.global_avg_gr = osal_div_u64(global_avg_sum_gr, pixel_weight);
    stat->be_ae_stat2.global_avg_gb = osal_div_u64(global_avg_sum_gb, pixel_weight);
    stat->be_ae_stat2.global_avg_b  = osal_div_u64(global_avg_sum_b,  pixel_weight);
}

hi_void isp_drv_be_apb_awb_statistics_read(isp_stat *stat,
                                           S_ISPBE_REGS_TYPE *be_reg[],
                                           hi_u8 blk_num, hi_u8 blk_dev)
{
    hi_u8  k = 0;
    hi_u64 metering_awb_avg_r = 0;
    hi_u64 metering_awb_avg_g = 0;
    hi_u64 metering_awb_avg_b = 0;
    hi_u64 metering_awb_count_all = 0;

    for (k = 0; k < blk_num; k++) {
        metering_awb_avg_r += (hi_u64)be_reg[k + blk_dev]->ISP_AWB_AVG_R.bits.isp_awb_avg_r *
                              be_reg[k + blk_dev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
        metering_awb_avg_g += (hi_u64)be_reg[k + blk_dev]->ISP_AWB_AVG_G.bits.isp_awb_avg_g *
                              be_reg[k + blk_dev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
        metering_awb_avg_b += (hi_u64)be_reg[k + blk_dev]->ISP_AWB_AVG_B.bits.isp_awb_avg_b *
                              be_reg[k + blk_dev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
        metering_awb_count_all += be_reg[k + blk_dev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
    }

    stat->awb_stat1.metering_awb_avg_r =
        (hi_u16)(osal_div_u64(metering_awb_avg_r, div_0_to_1(metering_awb_count_all)));
    stat->awb_stat1.metering_awb_avg_g =
        (hi_u16)(osal_div_u64(metering_awb_avg_g, div_0_to_1(metering_awb_count_all)));
    stat->awb_stat1.metering_awb_avg_b =
        (hi_u16)(osal_div_u64(metering_awb_avg_b, div_0_to_1(metering_awb_count_all)));
    stat->awb_stat1.metering_awb_count_all =
        (hi_u16)(osal_div_u64(metering_awb_count_all, div_0_to_1(blk_num)));
}

hi_s32 isp_drv_be_apb_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISPBE_REGS_TYPE *be_reg[],
                                      isp_be_regs_attr be_regs_attr, isp_stat_key un_statkey)
{
    hi_u8 k = 0;
    hi_u8  blk_dev, blk_num;

    isp_check_pointer_return(stat);

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_reg[k + blk_dev]);
    }

    if (un_statkey.bit1_comm_stat) {
        isp_drv_be_apb_comm_statistics_read(stat, be_reg, blk_dev);
    }

    /* Flicker */
    if (un_statkey.bit1_flicker) {
        isp_drv_be_apb_flicker_statistics_read(stat, be_reg, blk_num, blk_dev);
    }

    /* AE */
    if (un_statkey.bit1_be_ae_glo_stat) {
        isp_drv_be_apb_ae_statistics_read(stat, be_reg, blk_num, blk_dev);
    }

    /* awb */
    if (un_statkey.bit1_awb_stat1) {
        isp_drv_be_apb_awb_statistics_read(stat, be_reg, blk_num, blk_dev);
    }

    return HI_SUCCESS;
}

hi_void isp_drv_be_stt_get_virt_addr(hi_vi_pipe vi_pipe, hi_void **stt_buf_addr)
{
    hi_u32 cur_read_flag;
    hi_void *vir_addr = HI_NULL;
    hi_u64  phy_addr, size;
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if ((drv_ctx->be_online_stt_buf.cur_write_flag != 0) && (drv_ctx->be_online_stt_buf.cur_write_flag != 1)) {
        isp_err_trace("Err cur_write_flag != 0/1 !!!\n");
        drv_ctx->be_online_stt_buf.cur_write_flag = 0;
    }

    /* switch ping pong buffer */
    cur_read_flag = drv_ctx->be_online_stt_buf.cur_write_flag;
    if (drv_ctx->be_online_stt_buf.get_first_stat_info_flag == HI_FALSE) {
        osal_memcpy(drv_ctx->be_online_stt_buf.be_stt_buf[1].vir_addr,
                    drv_ctx->be_online_stt_buf.be_stt_buf[0].vir_addr, sizeof(isp_be_rw_online_stt_reg));
        cur_read_flag = 1;
        drv_ctx->be_online_stt_buf.get_first_stat_info_flag = HI_TRUE;
    }

    drv_ctx->be_online_stt_buf.cur_write_flag = 1 - cur_read_flag;

    vir_addr = drv_ctx->be_online_stt_buf.be_stt_buf[cur_read_flag].vir_addr;
    phy_addr = drv_ctx->be_online_stt_buf.be_stt_buf[cur_read_flag].phy_addr;
    size     = drv_ctx->be_online_stt_buf.be_stt_buf[cur_read_flag].size;

    osal_flush_dcache_area(vir_addr, phy_addr, size);

    *stt_buf_addr = vir_addr;
}

hi_s32 isp_drv_be_stt_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISPBE_REGS_TYPE *be_reg[],
                                      isp_be_regs_attr be_regs_attr, isp_stat_key un_statkey)
{
    hi_u8  k, idx;
    hi_u8  blk_dev, blk_num;
    isp_be_rw_online_stt_reg *be_rw_online_stt_reg = HI_NULL;
    S_ISP_ONLINE_STT_REGS_TYPE *be_online_stt[ISP_MAX_BE_NUM] = { HI_NULL };
    hi_void *vir_addr = HI_NULL;

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat);
    isp_check_pointer_return(be_reg[blk_dev]);

    isp_drv_be_stt_get_virt_addr(vi_pipe, &vir_addr);

    be_rw_online_stt_reg = (isp_be_rw_online_stt_reg *)vir_addr;

    for (k = 0; k < blk_num; k++) {
        idx = MIN2(ISP_MAX_BE_NUM - 1, k + blk_dev);
        be_online_stt[idx] = &be_rw_online_stt_reg->be_online_stt_reg[idx];
    }

    /* BE AE statistics */
    if (un_statkey.bit1_be_ae_glo_stat) {
        isp_drv_be_ae_stt_global_statistics_read(stat, be_online_stt, blk_num, blk_dev);
    }

    if (un_statkey.bit1_be_ae_loc_stat) {
        isp_drv_be_ae_stt_local_statistics_read(stat, be_online_stt, blk_num, blk_dev);
    }

    if (un_statkey.bit1_mg_stat) {
        isp_drv_be_mg_stt_statistics_read(stat, be_online_stt, blk_num, blk_dev);
    }

    /* BE AWB statistics */
    if (un_statkey.bit1_awb_stat2) {
        isp_drv_be_awb_stt_statistics_read(vi_pipe, stat, be_reg, be_online_stt, blk_num, blk_dev);
    }

    /* BE AF statistics */
    if (un_statkey.bit1_be_af_stat) {
        isp_drv_be_af_stt_statistics_read(stat, be_online_stt, blk_dev, be_reg[blk_dev]->ISP_AF_ZONE.u32);
    }

    /* BE dehaze statistics */
    if (un_statkey.bit1_dehaze) {
        isp_drv_be_dehaze_stt_statistics_read(stat, be_online_stt, blk_num, blk_dev);
    }

    if (un_statkey.bit1_flicker) {
        isp_drv_be_flicker_stt_statistics_read(stat, be_online_stt, blk_num, blk_dev);
    }

    isp_drv_ldci_online_attr_update(vi_pipe, stat);

    return HI_SUCCESS;
}

/* read BE statistics information:offline */
hi_s32 isp_drv_be_ae_global_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num)
{
    hi_u32 i, k;
    hi_u64 global_avg_sum_r  = 0;
    hi_u64 global_avg_sum_gr = 0;
    hi_u64 global_avg_sum_gb = 0;
    hi_u64 global_avg_sum_b  = 0;
    hi_u32 pixel_weight_tmp = 0;
    hi_u32 pixel_weight;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        for (i = 0; i < HIST_NUM; i++) {
            if (k == 0) {
                stat->be_ae_stat1.histogram_mem_array[i] = be_stt[k]->ISP_AE_HIST[i].u32;
            } else {
                stat->be_ae_stat1.histogram_mem_array[i] += be_stt[k]->ISP_AE_HIST[i].u32;
            }
        }

        if (k == 0) {
            pixel_weight_tmp = be_stt[k]->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            stat->be_ae_stat1.pixel_weight = pixel_weight_tmp;
            stat->be_ae_stat1.pixel_count  = be_stt[k]->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        } else {
            pixel_weight_tmp = be_stt[k]->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            stat->be_ae_stat1.pixel_weight += pixel_weight_tmp;
            stat->be_ae_stat1.pixel_count  += be_stt[k]->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        }

        global_avg_sum_r  += ((hi_u64)be_stt[k]->ISP_AE_TOTAL_R_AVER_RSTT.u32)  * ((hi_u64)pixel_weight_tmp);
        global_avg_sum_gr += ((hi_u64)be_stt[k]->ISP_AE_TOTAL_GR_AVER_RSTT.u32) * ((hi_u64)pixel_weight_tmp);
        global_avg_sum_gb += ((hi_u64)be_stt[k]->ISP_AE_TOTAL_GB_AVER_RSTT.u32) * ((hi_u64)pixel_weight_tmp);
        global_avg_sum_b  += ((hi_u64)be_stt[k]->ISP_AE_TOTAL_B_AVER_RSTT.u32)  * ((hi_u64)pixel_weight_tmp);
    }

    pixel_weight = div_0_to_1(stat->be_ae_stat1.pixel_weight);

    stat->be_ae_stat2.global_avg_r  = osal_div_u64(global_avg_sum_r,  pixel_weight);
    stat->be_ae_stat2.global_avg_gr = osal_div_u64(global_avg_sum_gr, pixel_weight);
    stat->be_ae_stat2.global_avg_gb = osal_div_u64(global_avg_sum_gb, pixel_weight);
    stat->be_ae_stat2.global_avg_b  = osal_div_u64(global_avg_sum_b,  pixel_weight);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_ae_local_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num)
{
    hi_u32 i, j, k;
    hi_u8  block_offset    = 0;
    hi_u8  block_zone_width = 0;
    hi_u32 ave_mem;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        for (i = 0; i < AE_ZONE_ROW; i++) {
            if (k < (AE_ZONE_COLUMN % div_0_to_1(blk_num))) {
                block_zone_width = (AE_ZONE_COLUMN / div_0_to_1(blk_num)) + 1;
            } else {
                block_zone_width = AE_ZONE_COLUMN / div_0_to_1(blk_num);
            }

            for (j = 0; j < block_zone_width; j++) {
                ave_mem = be_stt[k]->ISP_AE_AVER_R_GR[i * block_zone_width + j].u32;
                stat->be_ae_stat3.zone_avg[i][j + block_offset][0] =
                    (hi_u16)((ave_mem & 0xFFFF0000) >> 16); /* Rshift 16bit */
                stat->be_ae_stat3.zone_avg[i][j + block_offset][1] = /* array index 1 */
                    (hi_u16)((ave_mem & 0xFFFF));
                ave_mem = be_stt[k]->ISP_AE_AVER_GB_B[i * block_zone_width + j].u32;
                stat->be_ae_stat3.zone_avg[i][j + block_offset][2] = /* array index 2 */
                    (hi_u16)((ave_mem & 0xFFFF0000) >> 16); /* Rshift 16bit */
                stat->be_ae_stat3.zone_avg[i][j + block_offset][3] = /* array index 3 */
                    (hi_u16)((ave_mem & 0xFFFF));
            }
        }

        block_offset += block_zone_width;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_mg_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num)
{
    hi_u8  k;
    hi_u8  block_offset    = 0;
    hi_u8  block_zone_width = 0;
    hi_u32 i, j;
    hi_u32 ave_mem;

    isp_check_pointer_return(stat);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        for (i = 0; i < MG_ZONE_ROW; i++) {
            if (k < (MG_ZONE_COLUMN % div_0_to_1(blk_num))) {
                block_zone_width = (MG_ZONE_COLUMN / div_0_to_1(blk_num)) + 1;
            } else {
                block_zone_width = MG_ZONE_COLUMN / div_0_to_1(blk_num);
            }

            for (j = 0; j < block_zone_width; j++) {
                ave_mem = be_stt[k]->ISP_LA_AVER[i * block_zone_width + j].u32;
                stat->mg_stat.zone_avg[i][j + block_offset][0] =
                    (hi_u16)((ave_mem & 0xFF000000) >> 24); /* Rshift 24bit */
                stat->mg_stat.zone_avg[i][j + block_offset][1] = /* array index 1 */
                    (hi_u16)((ave_mem & 0xFF0000) >> 16); /* Rshift 16bit */
                stat->mg_stat.zone_avg[i][j + block_offset][2] = /* array index 2 */
                    (hi_u16)((ave_mem & 0xFF00) >> 8); /* Rshift 8bit */
                stat->mg_stat.zone_avg[i][j + block_offset][3] = /* array index 3 */
                    (hi_u16)((ave_mem & 0xFF)); /* low 8bit */
            }
        }

        block_offset += block_zone_width;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_awb_stat1_read(S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num,
                                 isp_stat *stat, hi_u16 *rgb_gain_conv)
{
    hi_s32 k;
    hi_u64 metering_awb_avg_r = 0;
    hi_u64 metering_awb_avg_g = 0;
    hi_u64 metering_awb_avg_b = 0;
    hi_u64 metering_awb_count_all = 0;

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        metering_awb_avg_r += (hi_u64)be_stt[k]->ISP_AWB_AVG_R_RSTT.bits.isp_awb_avg_r_stt *
                              be_stt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
        metering_awb_avg_g += (hi_u64)be_stt[k]->ISP_AWB_AVG_G_RSTT.bits.isp_awb_avg_g_stt *
                              be_stt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
        metering_awb_avg_b += (hi_u64)be_stt[k]->ISP_AWB_AVG_B_RSTT.bits.isp_awb_avg_b_stt *
                              be_stt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
        metering_awb_count_all += be_stt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
    }

    stat->awb_stat1.metering_awb_avg_r =
        (hi_u16)(osal_div_u64(metering_awb_avg_r, div_0_to_1(metering_awb_count_all)));
    stat->awb_stat1.metering_awb_avg_g =
        (hi_u16)(osal_div_u64(metering_awb_avg_g, div_0_to_1(metering_awb_count_all)));
    stat->awb_stat1.metering_awb_avg_b =
        (hi_u16)(osal_div_u64(metering_awb_avg_b, div_0_to_1(metering_awb_count_all)));
    stat->awb_stat1.metering_awb_count_all =
        (hi_u16)(osal_div_u64(metering_awb_count_all, div_0_to_1(blk_num)));
    stat->awb_stat1.metering_awb_avg_r =
        (stat->awb_stat1.metering_awb_avg_r * rgb_gain_conv[0] + 0x80) >> 8; /* Rshift 8bit, index[0] */
    stat->awb_stat1.metering_awb_avg_g =
        (stat->awb_stat1.metering_awb_avg_g * rgb_gain_conv[1] + 0x80) >> 8; /* Rshift 8bit, index[1] */
    stat->awb_stat1.metering_awb_avg_b =
        (stat->awb_stat1.metering_awb_avg_b * rgb_gain_conv[2] + 0x80) >> 8; /* Rshift 8bit, index[2] */

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_awb_stat2_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[],
                                 hi_u16 *rgb_gain_conv, hi_u8 blk_num)
{
    hi_s32 i, j, k;
    hi_u8  block_offset     = 0;
    hi_u8  block_zone_width = 0;
    hi_u32 col, row, value;
    hi_u16 read_r_avg, read_g_avg, read_avg;
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        col  = drv_ctx->kernel_cfg[drv_ctx->reg_cfg_info_flag].alg_kernel_cfg[k].be_awb_zone_cfg.colum;
        row  = drv_ctx->kernel_cfg[drv_ctx->reg_cfg_info_flag].alg_kernel_cfg[k].be_awb_zone_cfg.row;
        col  = MIN2(col, AWB_ZONE_ORIG_COLUMN);
        row  = MIN2(row, AWB_ZONE_ORIG_ROW);

        for (i = 0; i < row; i++) {
            if (k < (col % div_0_to_1(blk_num))) {
                block_zone_width = (col / div_0_to_1(blk_num)) + 1;
            } else {
                block_zone_width = col / div_0_to_1(blk_num);
            }

            for (j = 0; j < block_zone_width; j++) {
                value = be_stt[k]->ISP_AWB_STAT[(i * block_zone_width + j) * 2].u32; /* get array index(*2) */
                read_r_avg = (value & 0xFFFF);
                read_g_avg = ((value >> 16) & 0xFFFF); /* Rshift 16bit */
                value = be_stt[k]->ISP_AWB_STAT[(i * block_zone_width + j) * 2 + 1].u32; /* get array index(*2) */
                read_avg = (value & 0xFFFF);

                stat->awb_stat2.metering_mem_array_count_all[i * col + j + block_offset] =
                    ((value >> 16) & 0xFFFF); /* Rshift 16bit */
                stat->awb_stat2.metering_mem_array_avg_r[i * col + j + block_offset] =
                    (read_r_avg * rgb_gain_conv[0] + 0x80) >> 8; /* index[0] Rshift 8bit */
                stat->awb_stat2.metering_mem_array_avg_g[i * col + j + block_offset] =
                    (read_g_avg * rgb_gain_conv[1] + 0x80) >> 8; /* index[1] Rshift 8bit */
                stat->awb_stat2.metering_mem_array_avg_b[i * col + j + block_offset] =
                    (read_avg   * rgb_gain_conv[2] + 0x80) >> 8; /* index[2] Rshift 8bit */
            }
        }
        block_offset += block_zone_width;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_awb_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[],
                                              hi_u8 blk_num, isp_stat_key un_statkey)
{
    hi_s32 ret = HI_SUCCESS;
    hi_u16 rgb_gain_conv[3] = {0}; /* 3 : max index for R,G,B */
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_drv_cal_awb_gain_conv(drv_ctx, rgb_gain_conv);

    if (un_statkey.bit1_awb_stat1) {
        ret  = isp_drv_be_awb_stat1_read(be_stt, blk_num, stat, rgb_gain_conv);
        isp_check_ret(vi_pipe, ret, "isp_drv_be_awb_stat1_read");
    }

    if (un_statkey.bit1_awb_stat2) {
        ret = isp_drv_be_awb_stat2_read(vi_pipe, stat, be_stt, rgb_gain_conv, blk_num);
        isp_check_ret(vi_pipe, ret, "isp_drv_be_awb_stat2_read");
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_af_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat,
                                             S_ISP_STT_REGS_TYPE *be_stt[],
                                             hi_u8 blk_num)
{
#ifdef CONFIG_HI_ISP_AF_SUPPORT
    hi_u8  k;
    hi_u8  col_index    = 0;
    hi_u8  block_offset = 0;
    hi_u32 i, j;
    hi_u32 col;
    hi_u32 row;
    hi_u32 be_af_stat_data   = 0;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pointer_return(stat);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        col = drv_ctx->kernel_cfg[drv_ctx->reg_cfg_info_flag].alg_kernel_cfg[k].be_af_zone_cfg.colum;
        row = drv_ctx->kernel_cfg[drv_ctx->reg_cfg_info_flag].alg_kernel_cfg[k].be_af_zone_cfg.row;
        col = MIN2(col, AF_ZONE_COLUMN);
        row = MIN2(row, AF_ZONE_ROW);

        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                col_index = MIN2(j + block_offset, AF_ZONE_COLUMN);

                be_af_stat_data = be_stt[k]->ISP_AF_STAT[(i * col + j) * 4].u32; /* get array index(*4) */
                stat->be_af_stat.zone_metrics[i][col_index].v1 =
                    (hi_u16)((0xFFFF0000 & be_af_stat_data) >> 16); /* Rshift 16bit, get high 16bit */
                stat->be_af_stat.zone_metrics[i][col_index].h1 = (hi_u16)(0xFFFF & be_af_stat_data);
                be_af_stat_data = be_stt[k]->ISP_AF_STAT[(i * col + j) * 4 + 1].u32; /* get array index(*4)+1 */
                stat->be_af_stat.zone_metrics[i][col_index].v2 =
                    (hi_u16)((0xFFFF0000 & be_af_stat_data) >> 16); /* Rshift 16bit, get high 16bit */
                stat->be_af_stat.zone_metrics[i][col_index].h2 = (hi_u16)(0xFFFF & be_af_stat_data);
                be_af_stat_data = be_stt[k]->ISP_AF_STAT[(i * col + j) * 4 + 2].u32; /* get array index(*4)+2 */
                stat->be_af_stat.zone_metrics[i][col_index].hl_cnt =
                    (hi_u16)((0xFFFF0000 & be_af_stat_data) >> 16); /* Rshift 16bit, get high 16bit */
                stat->be_af_stat.zone_metrics[i][col_index].y     = (hi_u16)(0xFFFF & be_af_stat_data);
            }
        }

        block_offset += col;
    }
#endif
    return HI_SUCCESS;
}

hi_s32 isp_drv_be_dehaze_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num)
{
    hi_u32 i, j, m;

    j = DEFOG_ZONE_NUM >> 1;

    isp_check_pointer_return(stat);

    for (i = 0; i < blk_num; i++) {
        isp_check_pointer_return(be_stt[i]);

        for (m = 0; m < j; m++) {
            stat->dehaze_stat.min_dout[i][m] = be_stt[i]->ISP_DEHAZE_MINSTAT[m].u32;
        }

        for (m = 0; m < DEFOG_ZONE_NUM; m++) {
            stat->dehaze_stat.max_stat_dout[i][m] = be_stt[i]->ISP_DEHAZE_MAXSTAT[m].u32;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_dpc_offline_calib_info_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num,
                                           hi_u64 bit1_dp_stat)
{
    hi_u8  k;
    hi_u16 j;
    isp_check_pointer_return(stat);

    if (bit1_dp_stat != 1) {
        return HI_SUCCESS;
    }

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        stat->dp_stat.defect_pixel_count[k] =
            be_stt[k]->ISP_DPC_BPT_CALIB_NUMBER_RSTT.bits.isp_dpc_bpt_calib_number_rstt;

        for (j = 0; j < stat->dp_stat.defect_pixel_count[k]; j++) {
            stat->dp_stat.defect_pixel_lut[k][j] = be_stt[k]->ISP_DPC_BPT_RSTT[j].u32;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_flicker_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num,
                                               hi_u64 bit1_flicker)
{
    hi_u8  k;
    hi_u16 j;
    isp_check_pointer_return(stat);

    if (bit1_flicker != 1) {
        return HI_SUCCESS;
    }

    for (k = 0; k < blk_num; k++) {
        isp_check_pointer_return(be_stt[k]);

        stat->flicker_stat.cur_diff_gr[k] = be_stt[k]->ISP_FLICK_GR_DIFF_CUR_RSTT.bits.isp_flick_gr_diff_cur_rstt;
        stat->flicker_stat.cur_diff_gb[k] = be_stt[k]->ISP_FLICK_GB_DIFF_CUR_RSTT.bits.isp_flick_gb_diff_cur_rstt;
        stat->flicker_stat.cur_abs_gr[k]  = be_stt[k]->ISP_FLICK_GR_ABS_CUR_RSTT.bits.isp_flick_gr_abs_cur_rstt;
        stat->flicker_stat.cur_abs_gb[k]  = be_stt[k]->ISP_FLICK_GB_ABS_CUR_RSTT.bits.isp_flick_gb_abs_cur_rstt;

        for (j = 0; j < ISP_FLICKER_G_MEAN_LENGTH; j++) {
            stat->flicker_stat.g_mean[k][j] = be_stt[k]->ISP_FLICK_GMEAN[j].u32;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_ldci_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_memcpy(&stat->ldci_stat, &drv_ctx->ldci_stt_addr.ldci_stat, sizeof(isp_ldci_stat));

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_stitch_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    return HI_SUCCESS;
}

hi_s32 isp_drv_be_offline_stitch_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32 isp_drv_fe_stitch_non_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_be_lut_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    hi_s32 ret;
    hi_u8  *vir_addr = HI_NULL;
    hi_u64 phy_addr, size;
    unsigned long flags;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = {0};
    isp_drv_ctx *drv_ctx        = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->be_lut2stt_attr.init);

    osal_snprintf(ac_name, sizeof(ac_name), "BeLutStt[%d]", vi_pipe);
    size = sizeof(S_ISP_LUT_WSTT_TYPE);

    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr,
                                  (hi_void **)&vir_addr, size * 2 * ISP_MAX_BE_NUM); /* get memory size (*2) */
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc ISP be Lut stt buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size * 2 * ISP_MAX_BE_NUM); /* get memory size (*2) */
    isp_spin_lock   = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->be_lut2stt_attr.init = HI_TRUE;

    for (i = 0; i < ISP_MAX_BE_NUM; i++) {
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[0].phy_addr =
            phy_addr + 2 * i * size; /* get phyaddr index(*2) */
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[0].vir_addr =
            (hi_void *)(vir_addr + 2 * i * size); /* get viraddr index(*2) */
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[0].size     = size;

        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[1].phy_addr =
            phy_addr + (2 * i + 1) * size; /* get phyaddr index(*2) */
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[1].vir_addr =
            (hi_void *)(vir_addr + (2 * i + 1) * size); /* get viraddr index(*2) */
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[1].size     = size;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_lut_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    hi_u64 phy_addr;
    hi_void *vir_addr = HI_NULL;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->be_lut2stt_attr.init);

    phy_addr = drv_ctx->be_lut2stt_attr.be_lut_stt_buf[0].lut_stt_buf[0].phy_addr;
    vir_addr = drv_ctx->be_lut2stt_attr.be_lut_stt_buf[0].lut_stt_buf[0].vir_addr;

    isp_spin_lock = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->be_lut2stt_attr.init = HI_FALSE;

    for (i = 0; i < ISP_MAX_BE_NUM; i++) {
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[0].phy_addr = 0;
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[0].size     = 0;
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[0].vir_addr = HI_NULL;

        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[1].phy_addr = 0;
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[1].size     = 0;
        drv_ctx->be_lut2stt_attr.be_lut_stt_buf[i].lut_stt_buf[1].vir_addr = HI_NULL;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_online_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    hi_s32 ret;
    hi_u8  *vir_addr = HI_NULL;
    hi_u64 phy_addr, size;
    unsigned long flags;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = {0};
    isp_drv_ctx *drv_ctx           = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->be_online_stt_buf.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].BeSttOn", vi_pipe);
    size = sizeof(isp_be_rw_online_stt_reg);

    ret = cmpi_mmz_malloc_cached(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size * PING_PONG_NUM);
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc ISP be online stt buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size * PING_PONG_NUM);
    isp_spin_lock = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->be_online_stt_buf.init = HI_TRUE;

    for (i = 0; i < PING_PONG_NUM; i++) {
        drv_ctx->be_online_stt_buf.be_stt_buf[i].phy_addr = phy_addr + i * size;
        drv_ctx->be_online_stt_buf.be_stt_buf[i].vir_addr = (hi_void *)(vir_addr + i * size);
        drv_ctx->be_online_stt_buf.be_stt_buf[i].size     = size;
    }

    drv_ctx->be_online_stt_buf.cur_write_flag = 0;
    drv_ctx->be_online_stt_buf.get_first_stat_info_flag = HI_FALSE;
    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    ret = isp_drv_set_online_stt_addr(vi_pipe, drv_ctx->be_online_stt_buf.be_stt_buf[0].phy_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Set ISP online stt addr Err!\n", vi_pipe);
        goto fail0;
    }

    return HI_SUCCESS;

fail0:
    drv_ctx->be_online_stt_buf.init = HI_FALSE;

    for (i = 0; i < PING_PONG_NUM; i++) {
        drv_ctx->be_online_stt_buf.be_stt_buf[i].phy_addr = 0;
        drv_ctx->be_online_stt_buf.be_stt_buf[i].vir_addr = HI_NULL;
        drv_ctx->be_online_stt_buf.be_stt_buf[i].size     = 0;
    }

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, (hi_void *)vir_addr);
    }

    return HI_FAILURE;
}

hi_s32 isp_drv_be_online_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    hi_u64 phy_addr;
    hi_void *vir_addr = HI_NULL;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    phy_addr = drv_ctx->be_online_stt_buf.be_stt_buf[0].phy_addr;
    vir_addr = drv_ctx->be_online_stt_buf.be_stt_buf[0].vir_addr;
    isp_spin_lock = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->be_online_stt_buf.init = HI_FALSE;

    for (i = 0; i < PING_PONG_NUM; i++) {
        drv_ctx->be_online_stt_buf.be_stt_buf[i].phy_addr = 0;
        drv_ctx->be_online_stt_buf.be_stt_buf[i].size     = 0;
        drv_ctx->be_online_stt_buf.be_stt_buf[i].vir_addr = HI_NULL;
    }

    drv_ctx->be_online_stt_buf.cur_write_flag = 0;
    drv_ctx->be_online_stt_buf.get_first_stat_info_flag = HI_FALSE;
    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_stt_addr_init(hi_vi_pipe vi_pipe)
{
    hi_u8   chn_num_max, k;
    hi_vi_pipe vi_pipe_bind;
    isp_drv_ctx *drv_ctx = HI_NULL;
    S_ISP_VICAP_CH_REGS_TYPE *fe_stt_addr_reg = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_vir_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->fe_stt_attr.init);

    chn_num_max = drv_ctx->wdr_attr.dev_bind_pipe.num;

    if (chn_num_max > ISP_WDR_CHN_MAX) {
        isp_err_trace("unsupported %dTo1 WDR\n", chn_num_max);
        return HI_FAILURE;
    }

    for (k = 0; k < chn_num_max; k++) {
        drv_ctx->fe_stt_attr.fe_stt[k].first_frame = HI_TRUE;

        vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];

        isp_check_pipe_return(vi_pipe_bind);

        isp_drv_set_fe_stt_addr(vi_pipe_bind, drv_ctx->fe_stt_attr.fe_stt[k].fe_stt_buf[0].phy_addr);

        isp_drv_festtreg_ctx(vi_pipe_bind, fe_stt_addr_reg);
        fe_stt_addr_reg->VI_CH_REG_NEWER.bits.ch_reg_newer = 1;
        fe_stt_addr_reg->CH_UPDATE.bits.ch_update          = 1;
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32 isp_drv_fe_stitch_stt_addr_init(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_normal_fe_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u32 i, j;
    hi_u8  *vir_addr = HI_NULL;
    hi_u64 phy_addr, size, size_temp;
    unsigned long flags;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = {0};
    isp_drv_ctx *drv_ctx           = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->fe_stt_attr.init);

    if (drv_ctx->wdr_attr.mast_pipe == HI_FALSE) {
        return HI_SUCCESS;
    }

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].FeStt", vi_pipe);

    size = sizeof(S_ISP_FE_STT_REGS_TYPE);

    size_temp = size * 2 * ISP_WDR_CHN_MAX; /* get memory size (*2) */
    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size_temp);
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc ISP fe stt buf[%d] err\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size_temp);
    isp_spin_lock   = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->fe_stt_attr.init = HI_TRUE;

    for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
        for (j = 0; j < PING_PONG_NUM; j++) {
            drv_ctx->fe_stt_attr.fe_stt[i].fe_stt_buf[j].phy_addr =
                phy_addr + (2 * i + j) * size; /* get array index (*2) */
            drv_ctx->fe_stt_attr.fe_stt[i].fe_stt_buf[j].vir_addr =
                (hi_void *)(vir_addr + (2 * i + j) * size); /* get array index (*2) */
            drv_ctx->fe_stt_attr.fe_stt[i].fe_stt_buf[j].size     = size;
        }

        drv_ctx->fe_stt_attr.fe_stt[i].cur_write_flag = 0;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_check_pipe_return(vi_pipe);
    isp_check_vir_pipe_return(vi_pipe);

    ret = isp_drv_normal_fe_stt_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_warn_trace("isp_drv_normal_fe_stt_buf_init failure!\n");
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_fe_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u32  i, j;
    hi_u64  phy_addr;
    hi_void *vir_addr = HI_NULL;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_vir_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->fe_stt_attr.init);

    phy_addr = drv_ctx->fe_stt_attr.fe_stt[0].fe_stt_buf[0].phy_addr;
    vir_addr   = drv_ctx->fe_stt_attr.fe_stt[0].fe_stt_buf[0].vir_addr;
    isp_spin_lock = isp_drv_get_lock(vi_pipe);

    if (drv_ctx->wdr_attr.mast_pipe == HI_TRUE) {
        osal_spin_lock_irqsave(isp_spin_lock, &flags);

        drv_ctx->fe_stt_attr.init = HI_FALSE;

        for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
            for (j = 0; j < PING_PONG_NUM; j++) {
                drv_ctx->fe_stt_attr.fe_stt[i].fe_stt_buf[j].phy_addr = 0;
                drv_ctx->fe_stt_attr.fe_stt[i].fe_stt_buf[j].size     = 0;
                drv_ctx->fe_stt_attr.fe_stt[i].fe_stt_buf[j].vir_addr = HI_NULL;
            }

            drv_ctx->fe_stt_attr.fe_stt[i].cur_write_flag = 0;
            drv_ctx->fe_stt_attr.fe_stt[i].first_frame    = HI_FALSE;
        }

        osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
    }

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_offline_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u32 i;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr, size;
    hi_u8  *vir_addr = HI_NULL;
    unsigned long flags;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = {0};
    osal_spinlock_t *isp_spin_lock    = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->be_off_stt_attr.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].BeStt", vi_pipe);

    size = sizeof(S_ISP_STT_REGS_TYPE);

    size = (size + (256 - 1)) / 256 * 256; /* 256 bytes align, (x + (a-1))/a * a */

    ret = cmpi_mmz_malloc_cached(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size * ISP_STRIPING_MAX_NUM);
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc ISP BeSttBuf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size * ISP_STRIPING_MAX_NUM);
    isp_spin_lock = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->be_off_stt_attr.init = HI_TRUE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        drv_ctx->be_off_stt_attr.be_stt_buf[i].phy_addr = phy_addr + i * size;
        drv_ctx->be_off_stt_attr.be_stt_buf[i].vir_addr = (hi_void *)(vir_addr + i * size);
        drv_ctx->be_off_stt_attr.be_stt_buf[i].size     = size;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_offline_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    hi_u64 phy_addr;
    hi_void *vir_addr = HI_NULL;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    osal_spinlock_t *isp_spin_lock    = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->be_off_stt_attr.init);

    phy_addr = drv_ctx->be_off_stt_attr.be_stt_buf[0].phy_addr;
    vir_addr = drv_ctx->be_off_stt_attr.be_stt_buf[0].vir_addr;
    isp_spin_lock = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    drv_ctx->be_off_stt_attr.init = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        drv_ctx->be_off_stt_attr.be_stt_buf[i].phy_addr = 0;
        drv_ctx->be_off_stt_attr.be_stt_buf[i].size     = 0;
        drv_ctx->be_off_stt_attr.be_stt_buf[i].vir_addr = HI_NULL;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (is_online_mode(drv_ctx->work_mode.running_mode) ||  \
        is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        if (isp_drv_be_online_stt_buf_init(vi_pipe) != HI_SUCCESS) {
            return HI_FAILURE;
        }

        if (isp_drv_be_lut_stt_buf_init(vi_pipe) != HI_SUCCESS) {
            goto fail0;
        }
    } else if (is_offline_mode(drv_ctx->work_mode.running_mode) ||  \
               is_striping_mode(drv_ctx->work_mode.running_mode)) {
        if (isp_drv_be_offline_stt_buf_init(vi_pipe) != HI_SUCCESS) {
            return HI_FAILURE;
        }
    } else {
        isp_err_trace("enIspOnlineMode err 0x%x!\n", drv_ctx->work_mode.running_mode);
        return HI_FAILURE;
    }

    return HI_SUCCESS;

fail0:
    ret = isp_drv_be_online_stt_buf_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe[%d] isp_drv_be_online_stt_buf_exit failed!\n", vi_pipe);
    }

    return HI_FAILURE;
}

hi_s32 isp_drv_be_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (is_online_mode(drv_ctx->work_mode.running_mode) ||
        is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        if (isp_drv_be_online_stt_buf_exit(vi_pipe) != HI_SUCCESS) {
            isp_err_trace("pipe[%d] isp_drv_be_online_stt_buf_exit failed!\n", vi_pipe);
            return HI_FAILURE;
        }

        if (isp_drv_be_lut_stt_buf_exit(vi_pipe) != HI_SUCCESS) {
            isp_err_trace("pipe[%d] isp_drv_be_lut_stt_buf_exit failed!\n", vi_pipe);
            return HI_FAILURE;
        }
    } else if (is_offline_mode(drv_ctx->work_mode.running_mode) ||
               is_striping_mode(drv_ctx->work_mode.running_mode)) {
        if (isp_drv_be_offline_stt_buf_exit(vi_pipe) != HI_SUCCESS) {
            isp_err_trace("pipe[%d] isp_drv_be_offline_stt_buf_exit failed!\n", vi_pipe);
            return HI_FAILURE;
        }
    } else {
        isp_err_trace("enIspOnlineMode err 0x%x!\n", drv_ctx->work_mode.running_mode);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_stt_buf_init(hi_vi_pipe vi_pipe)
{
    isp_check_pipe_return(vi_pipe);

    if (isp_drv_be_stt_buf_init(vi_pipe) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    if (isp_drv_fe_stt_buf_init(vi_pipe) != HI_SUCCESS) {
        isp_drv_be_stt_buf_exit(vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    if (isp_drv_be_stt_buf_exit(vi_pipe) != HI_SUCCESS) {
        isp_err_trace("pipe[%d] isp_drv_be_stt_buf_exit failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    if (isp_drv_fe_stt_buf_exit(vi_pipe) != HI_SUCCESS) {
        isp_err_trace("pipe[%d] isp_drv_fe_stt_buf_exit failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_p2_en_info(hi_vi_pipe vi_pipe, hi_bool *p2_en)
{
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(p2_en);

    *p2_en = HI_FALSE;

    return HI_SUCCESS;
}

hi_s32 isp_drv_sta_kernel_get(hi_vi_pipe vi_pipe, isp_drv_af_statistics *focus_stat)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_stat *stat = HI_NULL;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    if (focus_stat == HI_NULL) {
        isp_err_trace("get statistic active buffer err, focus_stat is HI_NULL!\n");
        return HI_FAILURE;
    }

    if (!drv_ctx->statistics_buf.act_stat) {
        isp_info_trace("get statistic active buffer err, stat not ready!\n");
        return HI_FAILURE;
    }

    if (!drv_ctx->statistics_buf.act_stat->virt_addr) {
        isp_err_trace("get statistic active buffer err, virt_addr is HI_NULL!\n");
        return HI_FAILURE;
    }

    stat = (isp_stat *)drv_ctx->statistics_buf.act_stat->virt_addr;
    isp_spin_lock = isp_drv_get_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);
    osal_memset(&(focus_stat->fe_af_stat), 0, sizeof(isp_drv_fe_focus_statistics));
    osal_memcpy(&(focus_stat->be_af_stat), &stat->be_af_stat, sizeof(isp_drv_be_focus_statistics));
    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32 isp_drv_stitch_sync_ctrl_init(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_drv_get_stitch_be_sync_para(hi_vi_pipe vi_pipe, isp_be_sync_para *be_sync_para)
{
    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_reset_fe_cfg(hi_vi_pipe vi_pipe)
{
#ifdef ENABLE_CLOCK
    hi_u8   k;
    hi_bool reset = HI_TRUE;
    hi_vi_pipe vi_pipe_bind;
    hi_mpp_chn mpp_chn;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    mpp_chn.mod_id = HI_ID_ISP;

    for (k = 0; k < drv_ctx->wdr_attr.dev_bind_pipe.num; k++) {
        vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];

        isp_check_pipe_return(vi_pipe_bind);
        mpp_chn.dev_id = vi_pipe_bind;
        call_sys_drv_ioctrl(&mpp_chn, SYS_VI_ISP_CFG_RESET_SEL, &reset);
    }
    reset = HI_FALSE;
    for (k = 0; k < drv_ctx->wdr_attr.dev_bind_pipe.num; k++) {
        vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];

        isp_check_pipe_return(vi_pipe_bind);
        mpp_chn.dev_id = vi_pipe_bind;
        call_sys_drv_ioctrl(&mpp_chn, SYS_VI_ISP_CFG_RESET_SEL, &reset);
    }

#endif

    return HI_SUCCESS;
}

hi_s32 isp_drv_update_ldci_tpr_offline_stat(hi_vi_pipe vi_pipe, isp_be_wo_reg_cfg *be_cfg)
{
    hi_u8  i;
    hi_u64 phy_addr;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_cfg);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    phy_addr = be_cfg->be_reg_cfg[0].vi_proc_reg.OUT_PARA_DCI_ADDR_LOW.u32;

    for (i = 0; i < drv_ctx->ldci_read_buf_attr.buf_num; i++) {
        if (phy_addr == drv_ctx->ldci_write_buf_attr.ldci_buf[i].phy_addr) {
            osal_memcpy(&drv_ctx->ldci_stt_addr.ldci_stat, drv_ctx->ldci_write_buf_attr.ldci_buf[i].vir_addr,
                        sizeof(isp_ldci_stat));
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_update_ldci_tpr_stt_addr(hi_vi_pipe vi_pipe, isp_be_wo_reg_cfg *be_cfg)
{
    hi_bool ldci_tpr_flt_en;
    hi_u8  i;
    hi_u64 phy_addr;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_cfg);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    ldci_tpr_flt_en = isp_drv_get_ldci_tpr_flt_en(vi_pipe);
    if (ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < drv_ctx->work_mode.block_num; i++) {
        phy_addr  = be_cfg->be_reg_cfg[i].vi_proc_reg.OUT_PARA_DCI_ADDR_LOW.u32;
        drv_ctx->ldci_stt_addr.cur_write_stt_addr[i] = phy_addr;

        phy_addr = be_cfg->be_reg_cfg[i].vi_proc_reg.PARA_DCI_ADDR_LOW.u32;
        drv_ctx->ldci_stt_addr.read_stt_addr[i] = phy_addr;
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
hi_s32 isp_set_pq_ai_attr(pq_ai_attr *ai_attr)
{
    hi_bool change_flag = HI_FALSE;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_drv_ctx *base_drv_ctx = HI_NULL;

    isp_check_pointer_return(ai_attr);
    isp_check_pipe_return(ai_attr->ai_pipe_id);
    isp_check_pipe_return(ai_attr->base_pipe_id);
    isp_check_bool_return(ai_attr->pq_ai_en);

    drv_ctx = isp_drv_get_ctx(ai_attr->ai_pipe_id);
    if ((ai_attr->pq_ai_en != drv_ctx->ai_info.pq_ai_en) ||
        (ai_attr->ai_pipe_id != drv_ctx->ai_info.ai_pipe_id) ||
        (ai_attr->base_pipe_id != drv_ctx->ai_info.base_pipe_id)) {
        change_flag = HI_TRUE;
    }

    if ((drv_ctx->mem_init == HI_TRUE) && (change_flag == HI_TRUE)) {
        isp_err_trace("Does not support changed after isp init!\n");
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if (ai_attr->pq_ai_en == HI_TRUE) {
        if (ai_attr->ai_pipe_id == ai_attr->base_pipe_id) {
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        base_drv_ctx = isp_drv_get_ctx(ai_attr->base_pipe_id);
        if (base_drv_ctx->mem_init == HI_TRUE) {
            isp_err_trace("Base pipe has been init!\n");
            return HI_ERR_ISP_NOT_SUPPORT;
        }

        osal_memcpy(&base_drv_ctx->ai_info, ai_attr, sizeof(pq_ai_attr));
    }

    osal_memcpy(&drv_ctx->ai_info, ai_attr, sizeof(pq_ai_attr));

    return HI_SUCCESS;
}

hi_s32 isp_get_pq_ai_attr(hi_vi_pipe vi_pipe, pq_ai_attr *ai_attr)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(ai_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_memcpy(ai_attr, &drv_ctx->ai_info, sizeof(pq_ai_attr));

    return HI_SUCCESS;
}

hi_s32 isp_set_pq_ai_post_nr_attr(hi_vi_pipe vi_pipe, hi_pq_ai_noiseness_post_attr *ai_3dnr_attr)
{
    hi_s32 ret = HI_FAILURE;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(ai_3dnr_attr);
    if (check_func_entry(HI_ID_VI) && ckfn_vi_set_pq_3dnr_ai_attr()) {
        ret = call_vi_set_pq_3dnr_ai_att(vi_pipe, ai_3dnr_attr);
    }
    return ret;
}

hi_s32 isp_get_pq_ai_post_nr_attr(hi_vi_pipe vi_pipe, hi_pq_ai_noiseness_post_attr *ai_3dnr_attr)
{
    hi_s32 ret = HI_FAILURE;

    if (check_func_entry(HI_ID_VI) && ckfn_vi_get_pq_3dnr_ai_attr()) {
        ret = call_vi_get_pq_3dnr_ai_att(vi_pipe, ai_3dnr_attr);
    }
    return ret;
}
#endif
