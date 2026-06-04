/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_CONFIG_H
#define ISP_CONFIG_H

#include "ot_debug.h"
#include "ot_isp_debug.h"
#include "isp_vreg.h"
#include "isp_reg_define.h"
#include "isp_regcfg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OT_ISP_WDR_WGT_SLOPE_DEFAULT 0x10
#define OT_ISP_WDR_FUSION_DATA_MODE 0x2
#define OT_ISP_WDR_MDT_NOSF_HIG_THR_DEFAULT           0xbd
#define OT_ISP_WDR_MDT_NOSF_LOW_THR_DEFAULT           0x7e
#define OT_ISP_WDR_GAIN_SUM_HIG_THR_DEFAULT           0x3
#define OT_ISP_WDR_GAIN_SUM_LOW_THR_DEFAULT           0x2
#define OT_ISP_WDR_FORCELONG_SLOPE_DEFAULT            0x5
#define OT_ISP_WDR_FUSION_F1_THR_R                    3000
#define OT_ISP_WDR_FUSION_F0_THR_R                    3500
#define OT_ISP_WDR_FUSION_F1_THR_B                    3000
#define OT_ISP_WDR_FUSION_F0_THR_B                    3500
#define OT_ISP_WDR_FUSION_F1_THR_G                    3000
#define OT_ISP_WDR_FUSION_F0_THR_G                    3500


#define OT_ISP_FUSION_MODE_DEFAULT 0
#define OT_ISP_WDR_EROSION_EN_DEFAULT 1
#define OT_ISP_WDR_MDT_EN_DEFAULT 1
#define OT_ISP_WDR_GRAYSCALE_DEFAULT 0
#define OT_ISP_WDR_SAVE_BLC_EN_DEFAULT 1
#define OT_ISP_WDR_SATURATE_THR_DEFAULT 3777
#define OT_ISP_WDR_STILL_THR0_DEFAULT 0
#define OT_ISP_WDR_MDTL_BLD_DEFAULT 0
#define OT_ISP_WDR_MDT_FULL_THR_DEFAULT 250
#define OT_ISP_WDR_MDT_STILL_THR_DEFAULT 20
#define OT_ISP_WDR_FORCELONG_HIGH_THD_DEFAULT 1600
#define OT_ISP_WDR_FORCELONG_LOW_THD_DEFAULT 1024
#define OT_ISP_WDR_FORCELONG_SMOOTH_EN 0x1
#define OT_ISP_WDR_FORCELONG_EN_DEFAULT 0
#define OT_ISP_WDR_MDT_NOS_FLOOR_DEFAULT 64
#define OT_ISP_WDR_MDTHR_HIG_GAIN_DEFAULT 16
#define OT_ISP_WDR_MDTHR_LOW_GAIN_DEFAULT 16
#define ISP_WDR_SHORTCHK_THD_DEFAULT 12
#define OT_ISP_WDR_SHORT_EXPO_CHK_DEFAULT 1
#define OT_ISP_WDR_PIXEL_AVG_MAX_DIFF_DEFAULT 70
#define OT_ISP_WDR_MASK_SIMILAR_CNT_DEFAULT 3
#define OT_ISP_WDR_MASK_SIMILAR_THR_DEFAULT 20
#define OT_ISP_WDR_BLDRLHFIDX_DEFAULT 4
#define OT_ISP_WDR_LONG_THR_DEFAULT 3008
#define OT_ISP_WDR_SHORT_THR_DEFAULT 4032
#define OT_ISP_WDR_DFTWGT_FL_DEFAULT 256
#define OT_ISP_WDR_SQRT_DGAIN_G_DEFAULT 1
#define OT_ISP_WDR_SQRT_AGAIN_G_DEFAULT 1
#define OT_ISP_WDR_FUSION_SATURATE_THD 0xed1
#define OT_ISP_WDR_FUSION_BLEND_EN_DEFAULT 1
#define OT_ISP_WDR_FUSION_BLEND_WGT        4
#define OT_ISP_WDR_FUSION_FORCE_GRAY_DEFAULT 1
#define OT_ISP_WDR_FUSION_FORCE_BLEND_DEFAULT 14


/* FE FE FE FE FE FE FE FE FE */
/*  description : set the value of the member isp_fe_module_pos.isp_fe_module_posl */
/*  input       : unsigned int uisp_fe_module_posl: 16 bits */
static __inline td_void isp_fe_module_posl_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_module_posl)
{
    u_isp_fe_module_pos o_isp_fe_module_pos;
    o_isp_fe_module_pos.u32 = fe_reg->isp_fe_module_pos.u32;
    o_isp_fe_module_pos.bits.isp_fe_module_posl = uisp_fe_module_posl;
    fe_reg->isp_fe_module_pos.u32 = o_isp_fe_module_pos.u32;
}

/*  description : set the value of the member isp_fe_module_pos.isp_fe_module_posh */
/*  input       : unsigned int uisp_fe_module_posh: 16 bits */
static __inline td_void isp_fe_module_posh_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_module_posh)
{
    u_isp_fe_module_pos o_isp_fe_module_pos;
    o_isp_fe_module_pos.u32 = fe_reg->isp_fe_module_pos.u32;
    o_isp_fe_module_pos.bits.isp_fe_module_posh = uisp_fe_module_posh;
    fe_reg->isp_fe_module_pos.u32 = o_isp_fe_module_pos.u32;
}

/*  description : set the value of the member isp_fe_fstart.isp_fe_fstart */
/*  input       : unsigned int uisp_fe_fstart: 32 bits */
static __inline td_void isp_fe_fstart_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_fstart)
{
    fe_reg->isp_fe_fstart.u32 = uisp_fe_fstart;
}


/*  description : set the value of the member isp_fe_user_define0.isp_fe_user_define0 */
/*  input       : unsigned int uisp_fe_user_define0: 32 bits */
static __inline td_void isp_fe_user_define0_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_user_define0)
{
    fe_reg->isp_fe_user_define0.u32 = uisp_fe_user_define0;
}


/*  description : set the value of the member isp_fe_user_define1.isp_fe_user_define1 */
/*  input       : unsigned int uisp_fe_user_define1: 32 bits */
static __inline td_void isp_fe_user_define1_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_user_define1)
{
    fe_reg->isp_fe_user_define1.u32 = uisp_fe_user_define1;
}


/*  description : set the value of the member isp_fe_format.isp_fe_format */
/*  input       : unsigned int uisp_fe_format: 32 bits */
static __inline td_void isp_fe_format_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_format)
{
    fe_reg->isp_fe_format.u32 = uisp_fe_format;
}


/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_fstart */
/*  input       : unsigned int uisp_fe_int_mask_fstart: 1 bits */
static __inline td_void isp_fe_int_mask_fstart_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_fstart)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_fstart = uisp_fe_int_mask_fstart;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_update_cfg */
/*  input       : unsigned int uisp_fe_int_mask_update_cfg: 1 bits */
static __inline td_void isp_fe_int_mask_update_cfg_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_update_cfg)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_update_cfg = uisp_fe_int_mask_update_cfg;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_cfg_loss */
/*  input       : unsigned int uisp_fe_int_mask_cfg_loss: 1 bits */
static __inline td_void isp_fe_int_mask_cfg_loss_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_cfg_loss)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_cfg_loss = uisp_fe_int_mask_cfg_loss;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_int_delay */
/*  input       : unsigned int uisp_fe_int_mask_int_delay: 1 bits */
static __inline td_void isp_fe_int_mask_int_delay_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_int_delay)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_int_delay = uisp_fe_int_mask_int_delay;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_ae1_int */
/*  input       : unsigned int uisp_fe_int_mask_ae1_int: 1 bits */
static __inline td_void isp_fe_int_mask_ae1_int_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_ae1_int)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_ae1_int = uisp_fe_int_mask_ae1_int;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_af1_int */
/*  input       : unsigned int uisp_fe_int_mask_af1_int: 1 bits */
static __inline td_void isp_fe_int_mask_af1_int_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_af1_int)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_af1_int = uisp_fe_int_mask_af1_int;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_int_mask.isp_fe_int_mask_dis1_int */
/*  input       : unsigned int uisp_fe_int_mask_dis1_int: 1 bits */
static __inline td_void isp_fe_int_mask_dis1_int_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_dis1_int)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_dis1_int = uisp_fe_int_mask_dis1_int;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description   set the value of the member isp_fe_int_mask.isp_fe_int_mask_done_int */
/*  input         unsigned int uisp_fe_int_mask_done_int  1 bits */
static __inline td_void isp_fe_int_mask_int_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_int_mask_done_int)
{
    u_isp_fe_int_mask o_isp_fe_int_mask;
    o_isp_fe_int_mask.u32 = fe_reg->isp_fe_int_mask.u32;
    o_isp_fe_int_mask.bits.isp_fe_int_mask_int = uisp_fe_int_mask_done_int;
    fe_reg->isp_fe_int_mask.u32 = o_isp_fe_int_mask.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_colorbar_en */
/*  input       : unsigned int uisp_colorbar_en: 1 bits */
static __inline td_void isp_fe_colorbar_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_colorbar_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_colorbar_en = uisp_colorbar_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_crop_en */
/*  input       : unsigned int uisp_crop_en: 1 bits */
static __inline td_void isp_fe_crop_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_crop_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_crop_en = uisp_crop_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_blc1_en */
/*  input       : unsigned int uisp_blc1_en: 1 bits */
static __inline td_void isp_fe_blc1_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_blc1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_blc1_en = uisp_blc1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_rc_en */
/*  input       : unsigned int uisp_rc_en: 1 bits */
static __inline td_void isp_fe_rc_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_rc_en = uisp_rc_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_hrs_en */
/*  input       : unsigned int uisp_hrs_en: 1 bits */
static __inline td_void isp_fe_hrs_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_hrs_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_hrs_en = uisp_hrs_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_rlsc1_en */
/*  input       : unsigned int uisp_rlsc1_en: 1 bits */
static __inline td_void isp_fe_rlsc1_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rlsc1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_rlsc1_en = uisp_rlsc1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_dg2_en */
/*  input       : unsigned int uisp_dg2_en: 1 bits */
static __inline td_void isp_fe_dg2_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_dg2_en = uisp_dg2_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_dis1_en */
/*  input       : unsigned int uisp_dis1_en: 1 bits */
static __inline td_void isp_fe_dis1_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dis1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_dis1_en = uisp_dis1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_af1_en */
/*  input       : unsigned int uisp_af1_en: 1 bits */
static __inline td_void isp_fe_af1_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_af1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_af1_en = uisp_af1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_wb1_en */
/*  input       : unsigned int uisp_wb1_en: 1 bits */
static __inline td_void isp_fe_wb_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_wb1_en = uisp_wb1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_drcs_en */
/*  input       : unsigned int uisp_drcs_en: 1 bits */
static __inline td_void isp_fe_drcs_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_drcs_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_drcs_en = uisp_drcs_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_ae1_en */
/*  input       : unsigned int uisp_ae1_en: 1 bits */
static __inline td_void isp_fe_ae1_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_ae1_en = uisp_ae1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_loglut1_en */
/*  input       : unsigned int uisp_loglut1_en: 1 bits */
static __inline td_void isp_fe_loglut1_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_loglut1_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_loglut1_en = uisp_loglut1_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_dg_en */
/*  input       : unsigned int uisp_dg_en: 1 bits */
static __inline td_void isp_fe_dg_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_dg_en = uisp_dg_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_ctrl.isp_gamma_en */
/*  input       : unsigned int uisp_gamma_en: 1 bits */
static __inline td_void isp_fe_gamma_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_gamma_en)
{
    u_isp_fe_ctrl o_isp_fe_ctrl;
    o_isp_fe_ctrl.u32 = fe_reg->isp_fe_ctrl.u32;
    o_isp_fe_ctrl.bits.isp_gamma_en = uisp_gamma_en;
    fe_reg->isp_fe_ctrl.u32 = o_isp_fe_ctrl.u32;
}

/*  description : set the value of the member isp_fe_adapter_cfg.fe_hsync_mode */
/*  input       : unsigned int ufe_hsync_mode: 1 bits */
static __inline td_void isp_fe_hsync_mode_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_hsync_mode)
{
    u_isp_fe_adapter_cfg o_isp_fe_adapter_cfg;
    o_isp_fe_adapter_cfg.u32 = fe_reg->isp_fe_adapter_cfg.u32;
    o_isp_fe_adapter_cfg.bits.fe_hsync_mode = uisp_fe_hsync_mode;
    fe_reg->isp_fe_adapter_cfg.u32 = o_isp_fe_adapter_cfg.u32;
}

/*  description : set the value of the member isp_fe_adapter_cfg.fe_vsync_mode */
/*  input       : unsigned int ufe_vsync_mode: 1 bits */
static __inline td_void isp_fe_vsync_mode_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_vsync_mode)
{
    u_isp_fe_adapter_cfg o_isp_fe_adapter_cfg;
    o_isp_fe_adapter_cfg.u32 = fe_reg->isp_fe_adapter_cfg.u32;
    o_isp_fe_adapter_cfg.bits.fe_vsync_mode = uisp_fe_vsync_mode;
    fe_reg->isp_fe_adapter_cfg.u32 = o_isp_fe_adapter_cfg.u32;
}

/*  description : set the value of the member isp_fe_fstart_delay.isp_fe_delay */
/*  input       : unsigned int uisp_fe_delay: 32 bits */
static __inline td_void isp_fe_delay_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_delay)
{
    fe_reg->isp_fe_fstart_delay.u32 = uisp_fe_delay;
}


/*  description : set the value of the member isp_fe_ctrl_f.isp_fe_rggb_cfg */
/*  input       : unsigned int uisp_fe_rggb_cfg: 2 bits */
static __inline td_void isp_fe_rggb_cfg_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_rggb_cfg)
{
    u_isp_fe_ctrl_f o_isp_fe_ctrl_f;
    o_isp_fe_ctrl_f.u32 = fe_reg->isp_fe_ctrl_f.u32;
    o_isp_fe_ctrl_f.bits.isp_fe_rggb_cfg = uisp_fe_rggb_cfg;
    fe_reg->isp_fe_ctrl_f.u32 = o_isp_fe_ctrl_f.u32;
}

/*  description : set the value of the member isp_fe_ctrl_i.isp_fe_update_mode */
/*  input       : unsigned int uisp_fe_update_mode: 1 bits */
static __inline td_void isp_fe_update_mode_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_update_mode)
{
    u_isp_fe_ctrl_i o_isp_fe_ctrl_i;
    o_isp_fe_ctrl_i.u32 = fe_reg->isp_fe_ctrl_i.u32;
    o_isp_fe_ctrl_i.bits.isp_fe_update_mode = uisp_fe_update_mode;
    fe_reg->isp_fe_ctrl_i.u32 = o_isp_fe_ctrl_i.u32;
}

#define OT_ISP_FE_FIX_TIMING_STAT 0x40
/*  description : set the value of the member isp_fe_timing_cfg.isp_fe_fix_timing */
/*  input       : unsigned int uisp_fe_fix_timing: 13 bits */
static __inline td_void isp_fe_fix_timing_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_fix_timing)
{
    u_isp_fe_timing_cfg o_isp_fe_timing_cfg;
    o_isp_fe_timing_cfg.u32 = fe_reg->isp_fe_timing_cfg.u32;
    o_isp_fe_timing_cfg.bits.isp_fe_fix_timing = uisp_fe_fix_timing;
    fe_reg->isp_fe_timing_cfg.u32 = o_isp_fe_timing_cfg.u32;
}

/*  description : set the value of the member isp_fe_reg_update.isp_fe_update */
/*  input       : unsigned int uisp_fe_update: 1 bits */
static __inline td_void isp_fe_update_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_update)
{
    u_isp_fe_reg_update o_isp_fe_reg_update;
    o_isp_fe_reg_update.u32 = fe_reg->isp_fe_reg_update.u32;
    o_isp_fe_reg_update.bits.isp_fe_update = uisp_fe_update;
    fe_reg->isp_fe_reg_update.u32 = o_isp_fe_reg_update.u32;
}

/*  description   set the value of the member isp_fe_size.isp_fe_width */
/*  input         unsigned int uisp_fe_width  16 bits */
static __inline td_void isp_fe_size_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_width, td_u32 uisp_fe_height)
{
    fe_reg->isp_fe_size.u32 = (uisp_fe_height << 16) + uisp_fe_width; /* 16~31 uisp_fe_height */
}

/*  description   set the value of the member isp_fe_blk_size.isp_fe_blk_width */
/*  input         unsigned int uisp_fe_blk_width  16 bits */
static __inline td_void isp_fe_blk_size_write(isp_fe_reg_type *fe_reg, td_u32 fe_blk_width, td_u32 fe_blk_height)
{
    fe_reg->isp_fe_blk_size.u32 = (fe_blk_height << 16) + fe_blk_width; /* 16~31 uisp_fe_blk_height */
}


/*  description : set the value of the member isp_fe_in_sum_cfg.isp_fe_csc_sum_en */
/*  input       : unsigned int uisp_fe_csc_sum_en: 1 bits */
static __inline td_void isp_fe_csc_sum_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_csc_sum_en)
{
    u_isp_fe_in_sum_cfg o_isp_fe_in_sum_cfg;
    o_isp_fe_in_sum_cfg.u32 = fe_reg->isp_fe_in_sum_cfg.u32;
    o_isp_fe_in_sum_cfg.bits.isp_fe_csc_sum_en = uisp_fe_csc_sum_en;
    fe_reg->isp_fe_in_sum_cfg.u32 = o_isp_fe_in_sum_cfg.u32;
}

/*  description : set the value of the member isp_fe_out_sum_cfg.isp_fe_yuv444_sum_en */
/*  input       : unsigned int uisp_fe_yuv444_sum_en: 1 bits */
static __inline td_void isp_fe_yuv444_sum_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_yuv444_sum_en)
{
    u_isp_fe_out_sum_cfg o_isp_fe_out_sum_cfg;
    o_isp_fe_out_sum_cfg.u32 = fe_reg->isp_fe_out_sum_cfg.u32;
    o_isp_fe_out_sum_cfg.bits.isp_fe_yuv444_sum_en = uisp_fe_yuv444_sum_en;
    fe_reg->isp_fe_out_sum_cfg.u32 = o_isp_fe_out_sum_cfg.u32;
}

/*  description : set the value of the member isp_fe_blk_size.isp_fe_blk_width */
/*  input       : unsigned int uisp_fe_blk_width: 16 bits */
static __inline td_void isp_fe_blk_width_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_blk_width)
{
    u_isp_fe_blk_size o_isp_fe_blk_size;
    o_isp_fe_blk_size.u32 = fe_reg->isp_fe_blk_size.u32;
    o_isp_fe_blk_size.bits.isp_fe_blk_width = uisp_fe_blk_width;
    fe_reg->isp_fe_blk_size.u32 = o_isp_fe_blk_size.u32;
}

/*  description : set the value of the member isp_fe_blk_size.isp_fe_blk_height */
/*  input       : unsigned int uisp_fe_blk_height: 16 bits */
static __inline td_void isp_fe_blk_height_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_blk_height)
{
    u_isp_fe_blk_size o_isp_fe_blk_size;
    o_isp_fe_blk_size.u32 = fe_reg->isp_fe_blk_size.u32;
    o_isp_fe_blk_size.bits.isp_fe_blk_height = uisp_fe_blk_height;
    fe_reg->isp_fe_blk_size.u32 = o_isp_fe_blk_size.u32;
}

/*  description : set the value of the member isp_fe_blk_hblank.isp_fe_blk_b_hblank */
/*  input       : unsigned int uisp_fe_blk_b_hblank: 16 bits */
static __inline td_void isp_fe_blk_b_hblank_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_blk_b_hblank)
{
    u_isp_fe_blk_hblank o_isp_fe_blk_hblank;
    o_isp_fe_blk_hblank.u32 = fe_reg->isp_fe_blk_hblank.u32;
    o_isp_fe_blk_hblank.bits.isp_fe_blk_b_hblank = uisp_fe_blk_b_hblank;
    fe_reg->isp_fe_blk_hblank.u32 = o_isp_fe_blk_hblank.u32;
}

/*  description : set the value of the member isp_fe_blk_hblank.isp_fe_blk_f_hblank */
/*  input       : unsigned int uisp_fe_blk_f_hblank: 16 bits */
static __inline td_void isp_fe_blk_f_hblank_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_blk_f_hblank)
{
    u_isp_fe_blk_hblank o_isp_fe_blk_hblank;
    o_isp_fe_blk_hblank.u32 = fe_reg->isp_fe_blk_hblank.u32;
    o_isp_fe_blk_hblank.bits.isp_fe_blk_f_hblank = uisp_fe_blk_f_hblank;
    fe_reg->isp_fe_blk_hblank.u32 = o_isp_fe_blk_hblank.u32;
}

/*  description : set the value of the member isp_fe_blk_vblank.isp_fe_blk_b_vblank */
/*  input       : unsigned int uisp_fe_blk_b_vblank: 16 bits */
static __inline td_void isp_fe_blk_b_vblank_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_blk_b_vblank)
{
    u_isp_fe_blk_vblank o_isp_fe_blk_vblank;
    o_isp_fe_blk_vblank.u32 = fe_reg->isp_fe_blk_vblank.u32;
    o_isp_fe_blk_vblank.bits.isp_fe_blk_b_vblank = uisp_fe_blk_b_vblank;
    fe_reg->isp_fe_blk_vblank.u32 = o_isp_fe_blk_vblank.u32;
}

/*  description : set the value of the member isp_fe_blk_vblank.isp_fe_blk_f_vblank */
/*  input       : unsigned int uisp_fe_blk_f_vblank: 16 bits */
static __inline td_void isp_fe_blk_f_vblank_write(isp_fe_reg_type *fe_reg, td_u32 uisp_fe_blk_f_vblank)
{
    u_isp_fe_blk_vblank o_isp_fe_blk_vblank;
    o_isp_fe_blk_vblank.u32 = fe_reg->isp_fe_blk_vblank.u32;
    o_isp_fe_blk_vblank.bits.isp_fe_blk_f_vblank = uisp_fe_blk_f_vblank;
    fe_reg->isp_fe_blk_vblank.u32 = o_isp_fe_blk_vblank.u32;
}

/*  description : set the value of the member isp_blc1_offset1.isp_blc1_ofsgr */
/*  input       : unsigned int uisp_blc1_ofsgr: 15 bits */
static __inline td_void isp_fe_blc1_ofsgr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_blc1_ofsgr)
{
    u_isp_blc1_offset1 o_isp_blc1_offset1;
    o_isp_blc1_offset1.u32 = fe_reg->isp_blc1_offset1.u32;
    o_isp_blc1_offset1.bits.isp_blc1_ofsgr = uisp_blc1_ofsgr;
    fe_reg->isp_blc1_offset1.u32 = o_isp_blc1_offset1.u32;
}

/*  description : set the value of the member isp_blc1_offset1.isp_blc1_ofsr */
/*  input       : unsigned int uisp_blc1_ofsr: 15 bits */
static __inline td_void isp_fe_blc1_ofsr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_blc1_ofsr)
{
    u_isp_blc1_offset1 o_isp_blc1_offset1;
    o_isp_blc1_offset1.u32 = fe_reg->isp_blc1_offset1.u32;
    o_isp_blc1_offset1.bits.isp_blc1_ofsr = uisp_blc1_ofsr;
    fe_reg->isp_blc1_offset1.u32 = o_isp_blc1_offset1.u32;
}

/*  description : set the value of the member isp_blc1_offset2.isp_blc1_ofsgb */
/*  input       : unsigned int uisp_blc1_ofsgb: 15 bits */
static __inline td_void isp_fe_blc1_ofsgb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_blc1_ofsgb)
{
    u_isp_blc1_offset2 o_isp_blc1_offset2;
    o_isp_blc1_offset2.u32 = fe_reg->isp_blc1_offset2.u32;
    o_isp_blc1_offset2.bits.isp_blc1_ofsgb = uisp_blc1_ofsgb;
    fe_reg->isp_blc1_offset2.u32 = o_isp_blc1_offset2.u32;
}

/*  description : set the value of the member isp_blc1_offset2.isp_blc1_ofsb */
/*  input       : unsigned int uisp_blc1_ofsb: 15 bits */
static __inline td_void isp_fe_blc1_ofsb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_blc1_ofsb)
{
    u_isp_blc1_offset2 o_isp_blc1_offset2;
    o_isp_blc1_offset2.u32 = fe_reg->isp_blc1_offset2.u32;
    o_isp_blc1_offset2.bits.isp_blc1_ofsb = uisp_blc1_ofsb;
    fe_reg->isp_blc1_offset2.u32 = o_isp_blc1_offset2.u32;
}

/*  description   set the value of the member isp_blc1_offset2.isp_blc1_ofsgb */
/*  input         unsigned int uisp_blc1_ofsgb  15 bits */
static __inline td_void isp_fe_blc1_offset_write(isp_fe_reg_type *fe_reg, const td_u16 *puisp_blc1_ofs)
{
    fe_reg->isp_blc1_offset1.u32 = ((td_u32)puisp_blc1_ofs[0] << 16) + puisp_blc1_ofs[1]; /* 16~31  r_blc */
    fe_reg->isp_blc1_offset2.u32 = ((td_u32)puisp_blc1_ofs[3] << 16) + puisp_blc1_ofs[2]; /* 16~31  b_blc, 2/3 gb/b */
}

static __inline td_void isp_fe_dg2_blc_en_write(isp_fe_reg_type *fe_reg,
    td_u32 uisp_dg2_en_in, td_u32 uisp_dg2_en_out)
{
    fe_reg->isp_dg2_blc_cfg.u32 = (uisp_dg2_en_out << 1) + uisp_dg2_en_in;
}

/*  description : set the value of the member isp_dg2_gain1.isp_dg2_grgain */
/*  input       : unsigned int uisp_dg2_grgain: 16 bits */
static __inline td_void isp_fe_dg2_grgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_grgain)
{
    u_isp_dg2_gain1 o_isp_dg2_gain1;
    o_isp_dg2_gain1.u32 = fe_reg->isp_dg2_gain1.u32;
    o_isp_dg2_gain1.bits.isp_dg2_grgain = uisp_dg2_grgain;
    fe_reg->isp_dg2_gain1.u32 = o_isp_dg2_gain1.u32;
}

/*  description : set the value of the member isp_dg2_gain1.isp_dg2_rgain */
/*  input       : unsigned int uisp_dg2_rgain: 16 bits */
static __inline td_void isp_fe_dg2_rgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_rgain)
{
    u_isp_dg2_gain1 o_isp_dg2_gain1;
    o_isp_dg2_gain1.u32 = fe_reg->isp_dg2_gain1.u32;
    o_isp_dg2_gain1.bits.isp_dg2_rgain = uisp_dg2_rgain;
    fe_reg->isp_dg2_gain1.u32 = o_isp_dg2_gain1.u32;
}

/*  description : set the value of the member isp_dg2_gain2.isp_dg2_gbgain */
/*  input       : unsigned int uisp_dg2_gbgain: 16 bits */
static __inline td_void isp_fe_dg2_gbgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_gbgain)
{
    u_isp_dg2_gain2 o_isp_dg2_gain2;
    o_isp_dg2_gain2.u32 = fe_reg->isp_dg2_gain2.u32;
    o_isp_dg2_gain2.bits.isp_dg2_gbgain = uisp_dg2_gbgain;
    fe_reg->isp_dg2_gain2.u32 = o_isp_dg2_gain2.u32;
}

/*  description : set the value of the member isp_dg2_gain2.isp_dg2_bgain */
/*  input       : unsigned int uisp_dg2_bgain: 16 bits */
static __inline td_void isp_fe_dg2_bgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_bgain)
{
    u_isp_dg2_gain2 o_isp_dg2_gain2;
    o_isp_dg2_gain2.u32 = fe_reg->isp_dg2_gain2.u32;
    o_isp_dg2_gain2.bits.isp_dg2_bgain = uisp_dg2_bgain;
    fe_reg->isp_dg2_gain2.u32 = o_isp_dg2_gain2.u32;
}

/*  description : set the value of the member isp_dg2_blc_offset1.isp_dg2_ofsgr */
/*  input       : unsigned int uisp_dg2_ofsgr: 15 bits */
static __inline td_void isp_fe_dg2_ofsgr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_ofsgr)
{
    u_isp_dg2_blc_offset1 o_isp_dg2_blc_offset1;
    o_isp_dg2_blc_offset1.u32 = fe_reg->isp_dg2_blc_offset1.u32;
    o_isp_dg2_blc_offset1.bits.isp_dg2_ofsgr = uisp_dg2_ofsgr;
    fe_reg->isp_dg2_blc_offset1.u32 = o_isp_dg2_blc_offset1.u32;
}

/*  description : set the value of the member isp_dg2_blc_offset1.isp_dg2_ofsr */
/*  input       : unsigned int uisp_dg2_ofsr: 15 bits */
static __inline td_void isp_fe_dg2_ofsr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_ofsr)
{
    u_isp_dg2_blc_offset1 o_isp_dg2_blc_offset1;
    o_isp_dg2_blc_offset1.u32 = fe_reg->isp_dg2_blc_offset1.u32;
    o_isp_dg2_blc_offset1.bits.isp_dg2_ofsr = uisp_dg2_ofsr;
    fe_reg->isp_dg2_blc_offset1.u32 = o_isp_dg2_blc_offset1.u32;
}

/*  description : set the value of the member isp_dg2_blc_offset2.isp_dg2_ofsgb */
/*  input       : unsigned int uisp_dg2_ofsgb: 15 bits */
static __inline td_void isp_fe_dg2_ofsgb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_ofsgb)
{
    u_isp_dg2_blc_offset2 o_isp_dg2_blc_offset2;
    o_isp_dg2_blc_offset2.u32 = fe_reg->isp_dg2_blc_offset2.u32;
    o_isp_dg2_blc_offset2.bits.isp_dg2_ofsgb = uisp_dg2_ofsgb;
    fe_reg->isp_dg2_blc_offset2.u32 = o_isp_dg2_blc_offset2.u32;
}

/*  description : set the value of the member isp_dg2_blc_offset2.isp_dg2_ofsb */
/*  input       : unsigned int uisp_dg2_ofsb: 15 bits */
static __inline td_void isp_fe_dg2_ofsb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_ofsb)
{
    u_isp_dg2_blc_offset2 o_isp_dg2_blc_offset2;
    o_isp_dg2_blc_offset2.u32 = fe_reg->isp_dg2_blc_offset2.u32;
    o_isp_dg2_blc_offset2.bits.isp_dg2_ofsb = uisp_dg2_ofsb;
    fe_reg->isp_dg2_blc_offset2.u32 = o_isp_dg2_blc_offset2.u32;
}

static __inline td_void isp_fe_dg2_offset_write(isp_fe_reg_type *fe_reg, const td_u16 *puisp_dg2_ofs)
{
    fe_reg->isp_dg2_blc_offset1.u32 = ((td_u32)puisp_dg2_ofs[0] << 16) + puisp_dg2_ofs[1]; /* 16~31  r_blc */
    fe_reg->isp_dg2_blc_offset2.u32 = ((td_u32)puisp_dg2_ofs[3] << 16) + puisp_dg2_ofs[2]; /* 16~31  b_blc, 2/3 gb/b */
}

/*  description : set the value of the member isp_dg2_clip_value.isp_dg2_clip_value */
/*  input       : unsigned int uisp_dg2_clip_value: 20 bits */
static __inline td_void isp_fe_dg2_clip_value_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg2_clip_value)
{
    u_isp_dg2_clip_value o_isp_dg2_clip_value;
    o_isp_dg2_clip_value.u32 = fe_reg->isp_dg2_clip_value.u32;
    o_isp_dg2_clip_value.bits.isp_dg2_clip_value = uisp_dg2_clip_value;
    fe_reg->isp_dg2_clip_value.u32 = o_isp_dg2_clip_value.u32;
}

/*  description   set the value of the member isp_wb1_blc_cfg.isp_wb1_en_in */
/*  input         unsigned int uisp_wb1_en_in  1 bits */
static __inline td_void isp_fe_wb_blc_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_en_in, td_u32 uisp_wb1_en_out)
{
    fe_reg->isp_wb1_blc_cfg.u32 = (uisp_wb1_en_out << 1) + uisp_wb1_en_in;
}

/*  description   set the value of the member isp_wb1_gain1.isp_wb1_rgain */
/*  input         unsigned int uisp_wb1_rgain  16 bits */
static __inline td_void isp_fe_wb_gain_write(isp_fe_reg_type *fe_reg, td_u32 *puisp_wb1_gain)
{
    fe_reg->isp_wb1_gain1.u32 = (puisp_wb1_gain[0] << 16) + puisp_wb1_gain[1]; /* bit16~31  rgain */
    fe_reg->isp_wb1_gain2.u32 = (puisp_wb1_gain[3] << 16) + puisp_wb1_gain[2]; /* bit16~31  bgain, 2/3 gb/b */
}

/*  description : set the value of the member isp_wb1_blc_offset1.isp_wb1_ofsgr */
/*  input       : unsigned int uisp_wb1_ofsgr: 15 bits */
static __inline td_void isp_fe_wb1_ofsgr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_ofsgr)
{
    u_isp_wb1_blc_offset1 o_isp_wb1_blc_offset1;
    o_isp_wb1_blc_offset1.u32 = fe_reg->isp_wb1_blc_offset1.u32;
    o_isp_wb1_blc_offset1.bits.isp_wb1_ofsgr = uisp_wb1_ofsgr;
    fe_reg->isp_wb1_blc_offset1.u32 = o_isp_wb1_blc_offset1.u32;
}

/*  description : set the value of the member isp_wb1_blc_offset1.isp_wb1_ofsr */
/*  input       : unsigned int uisp_wb1_ofsr: 15 bits */
static __inline td_void isp_fe_wb1_ofsr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_ofsr)
{
    u_isp_wb1_blc_offset1 o_isp_wb1_blc_offset1;
    o_isp_wb1_blc_offset1.u32 = fe_reg->isp_wb1_blc_offset1.u32;
    o_isp_wb1_blc_offset1.bits.isp_wb1_ofsr = uisp_wb1_ofsr;
    fe_reg->isp_wb1_blc_offset1.u32 = o_isp_wb1_blc_offset1.u32;
}

/*  description : set the value of the member isp_wb1_blc_offset2.isp_wb1_ofsgb */
/*  input       : unsigned int uisp_wb1_ofsgb: 15 bits */
static __inline td_void isp_fe_wb1_ofsgb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_ofsgb)
{
    u_isp_wb1_blc_offset2 o_isp_wb1_blc_offset2;
    o_isp_wb1_blc_offset2.u32 = fe_reg->isp_wb1_blc_offset2.u32;
    o_isp_wb1_blc_offset2.bits.isp_wb1_ofsgb = uisp_wb1_ofsgb;
    fe_reg->isp_wb1_blc_offset2.u32 = o_isp_wb1_blc_offset2.u32;
}

/*  description : set the value of the member isp_wb1_blc_offset2.isp_wb1_ofsb */
/*  input       : unsigned int uisp_wb1_ofsb: 15 bits */
static __inline td_void isp_fe_wb1_ofsb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_ofsb)
{
    u_isp_wb1_blc_offset2 o_isp_wb1_blc_offset2;
    o_isp_wb1_blc_offset2.u32 = fe_reg->isp_wb1_blc_offset2.u32;
    o_isp_wb1_blc_offset2.bits.isp_wb1_ofsb = uisp_wb1_ofsb;
    fe_reg->isp_wb1_blc_offset2.u32 = o_isp_wb1_blc_offset2.u32;
}

static __inline td_void isp_fe_wb_offset_write(isp_fe_reg_type *fe_reg, const td_u16 *puisp_wb1_ofs)
{
    fe_reg->isp_wb1_blc_offset1.u32 = ((td_u32)puisp_wb1_ofs[0] << 16) + puisp_wb1_ofs[1]; /* 16~31  r_blc */
    fe_reg->isp_wb1_blc_offset2.u32 = ((td_u32)puisp_wb1_ofs[3] << 16) + puisp_wb1_ofs[2]; /* 16~31  b_blc, 2/3 gb/b */
}

/*  description : set the value of the member isp_wb1_clip_value.isp_wb1_clip_value */
/*  input       : unsigned int uisp_wb1_clip_value: 20 bits */
static __inline td_void isp_fe_wb1_clip_value_write(isp_fe_reg_type *fe_reg, td_u32 uisp_wb1_clip_value)
{
    u_isp_wb1_clip_value o_isp_wb1_clip_value;
    o_isp_wb1_clip_value.u32 = fe_reg->isp_wb1_clip_value.u32;
    o_isp_wb1_clip_value.bits.isp_wb1_clip_value = uisp_wb1_clip_value;
    fe_reg->isp_wb1_clip_value.u32 = o_isp_wb1_clip_value.u32;
}

/*  description   set the value of the member isp_ae_zone.isp_ae_hnum */
/*  input         unsigned int uisp_ae_hnum  5 bits */
static __inline td_void isp_fe_ae_zone_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_hnum, td_u32 uisp_ae1_vnum)
{
    fe_reg->isp_ae_zone.u32 = (uisp_ae1_vnum << 8) + uisp_ae_hnum; /* bit8~12  ae1_vnum */
}

static __inline td_void isp_fe_ae_skip_crg_write(isp_fe_reg_type *fe_reg, td_u32 ae1_skip_x,
                                                 td_u32 ae1_offset_x, td_u32 ae1_skip_y, td_u32 ae1_offset_y)
{
    fe_reg->isp_ae_skip_crg.u32 = (ae1_offset_y << 7) + (ae1_skip_y << 4) + /* bit7  offset_y, bit4~6 skip_y */
                                  (ae1_offset_x << 3) + ae1_skip_x;         /* bit3  offset_x */
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_bitmove */
/*  input       : unsigned int uisp_ae_bitmove: 5 bits */
static __inline td_void isp_fe_ae_bitmove_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_bitmove)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = fe_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_bitmove = uisp_ae_bitmove;
    fe_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_hist_gamma_mode */
/*  input       : unsigned int uisp_ae_hist_gamma_mode: 2 bits */
static __inline td_void isp_fe_ae_hist_gamma_mode_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_hist_gamma_mode)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = fe_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_hist_gamma_mode = uisp_ae_hist_gamma_mode;
    fe_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_aver_gamma_mode */
/*  input       : unsigned int uisp_ae_aver_gamma_mode: 2 bits */
static __inline td_void isp_fe_ae_aver_gamma_mode_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_aver_gamma_mode)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = fe_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_aver_gamma_mode = uisp_ae_aver_gamma_mode;
    fe_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_blc_en */
/*  input       : unsigned int uisp_ae_blc_en: 1 bits */
static __inline td_void isp_fe_ae_blc_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_blc_en)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = fe_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_blc_en = uisp_ae_blc_en;
    fe_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_offset_r_gr.isp_ae_offset_r */
/*  input       : unsigned int uisp_ae_offset_r: 15 bits */
static __inline td_void isp_fe_ae_offset_r_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_offset_r)
{
    u_isp_ae_offset_r_gr o_isp_ae_offset_r_gr;
    o_isp_ae_offset_r_gr.u32 = fe_reg->isp_ae_offset_r_gr.u32;
    o_isp_ae_offset_r_gr.bits.isp_ae_offset_r = uisp_ae_offset_r;
    fe_reg->isp_ae_offset_r_gr.u32 = o_isp_ae_offset_r_gr.u32;
}

/*  description : set the value of the member isp_ae_offset_r_gr.isp_ae_offset_gr */
/*  input       : unsigned int uisp_ae_offset_gr: 15 bits */
static __inline td_void isp_fe_ae_offset_gr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_offset_gr)
{
    u_isp_ae_offset_r_gr o_isp_ae_offset_r_gr;
    o_isp_ae_offset_r_gr.u32 = fe_reg->isp_ae_offset_r_gr.u32;
    o_isp_ae_offset_r_gr.bits.isp_ae_offset_gr = uisp_ae_offset_gr;
    fe_reg->isp_ae_offset_r_gr.u32 = o_isp_ae_offset_r_gr.u32;
}

/*  description : set the value of the member isp_ae_offset_gb_b.isp_ae_offset_gb */
/*  input       : unsigned int uisp_ae_offset_gb: 15 bits */
static __inline td_void isp_fe_ae_offset_gb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_offset_gb)
{
    u_isp_ae_offset_gb_b o_isp_ae_offset_gb_b;
    o_isp_ae_offset_gb_b.u32 = fe_reg->isp_ae_offset_gb_b.u32;
    o_isp_ae_offset_gb_b.bits.isp_ae_offset_gb = uisp_ae_offset_gb;
    fe_reg->isp_ae_offset_gb_b.u32 = o_isp_ae_offset_gb_b.u32;
}

/*  description : set the value of the member isp_ae_offset_gb_b.isp_ae_offset_b */
/*  input       : unsigned int uisp_ae_offset_b: 15 bits */
static __inline td_void isp_fe_ae_offset_b_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_offset_b)
{
    u_isp_ae_offset_gb_b o_isp_ae_offset_gb_b;
    o_isp_ae_offset_gb_b.u32 = fe_reg->isp_ae_offset_gb_b.u32;
    o_isp_ae_offset_gb_b.bits.isp_ae_offset_b = uisp_ae_offset_b;
    fe_reg->isp_ae_offset_gb_b.u32 = o_isp_ae_offset_gb_b.u32;
}

static __inline td_void isp_fe_ae_offset_write(isp_fe_reg_type *fe_reg, const td_u16 *isp_ae_offset)
{
    fe_reg->isp_ae_offset_r_gr.u32 = ((td_u32)isp_ae_offset[1] << 16) + isp_ae_offset[0]; /* 16~31  r_blc */
    fe_reg->isp_ae_offset_gb_b.u32 = ((td_u32)isp_ae_offset[3] << 16) + isp_ae_offset[2]; /* 16~31  b_blc, 2/3 gb/b */
}

/*  description : set the value of the member isp_ae_lut_update.isp_ae_lut_update */
/*  input       : unsigned int uisp_ae_lut_update: 1 bits */
static __inline td_void isp_fe_ae_lut_update_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_lut_update)
{
    u_isp_ae_lut_update o_isp_ae_lut_update;
    o_isp_ae_lut_update.u32 = fe_reg->isp_ae_lut_update.u32;
    o_isp_ae_lut_update.bits.isp_ae_lut_update = uisp_ae_lut_update;
    fe_reg->isp_ae_lut_update.u32 = o_isp_ae_lut_update.u32;
}

/*  description : set the value of the member isp_ae_stt_bst.isp_ae_stt_size */
/*  input       : unsigned int uisp_ae_stt_size: 16 bits */
static __inline td_void isp_fe_ae_stt_size_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_stt_size)
{
    u_isp_ae_stt_bst o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = fe_reg->isp_ae_stt_bst.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_size = uisp_ae_stt_size;
    fe_reg->isp_ae_stt_bst.u32 = o_isp_ae_stt_bst.u32;
}

/*  description : set the value of the member isp_ae_stt_bst.isp_ae_stt_bst */
/*  input       : unsigned int uisp_ae_stt_bst: 4 bits */
static __inline td_void isp_fe_ae_stt_bst_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_stt_bst)
{
    u_isp_ae_stt_bst o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = fe_reg->isp_ae_stt_bst.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_bst = uisp_ae_stt_bst;
    fe_reg->isp_ae_stt_bst.u32 = o_isp_ae_stt_bst.u32;
}

/*  description : set the value of the member isp_ae_stt_bst.isp_ae_stt_en */
/*  input       : unsigned int uisp_ae_stt_en: 1 bits */
static __inline td_void isp_fe_ae_stt_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_stt_en)
{
    u_isp_ae_stt_bst o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = fe_reg->isp_ae_stt_bst.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_en = uisp_ae_stt_en;
    fe_reg->isp_ae_stt_bst.u32 = o_isp_ae_stt_bst.u32;
}

/*  description   set the value of the member isp_ae_crop_pos.isp_ae_crop_pos_x */
/*  input         unsigned int uisp_ae_crop_pos_x  16 bits */
static __inline td_void isp_fe_ae_crop_pos_write(isp_fe_reg_type *fe_reg, td_u32 ae_crop_pos_x, td_u32 ae_crop_pos_y)
{
    fe_reg->isp_ae_crop_pos.u32 = (ae_crop_pos_y << 16) + ae_crop_pos_x; /* bit16~31  crop_pos_y */
}

/*  description   set the value of the member isp_ae_crop_outsize.isp_ae_crop_out_width */
/*  input         unsigned int uisp_ae_crop_out_width  16 bits */
static __inline td_void isp_fe_ae_crop_outsize_write(isp_fe_reg_type *fe_reg, td_u32 ae_crop_out_width,
                                                     td_u32 ae_crop_out_height)
{
    fe_reg->isp_ae_crop_outsize.u32 = (ae_crop_out_height << 16) + ae_crop_out_width; /* bit16~31  height */
}

/*  description : set the value of the member isp_ae_hist_raddr.isp_ae_hist_raddr */
/*  input       : unsigned int uisp_ae_hist_raddr: 32 bits */
static __inline td_void isp_fe_ae_hist_raddr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_hist_raddr)
{
    fe_reg->isp_ae_hist_raddr.u32 = uisp_ae_hist_raddr;
}


/*  description : set the value of the member isp_ae_weight_waddr.isp_ae_wei_waddr */
/*  input       : unsigned int uisp_ae_wei_waddr: 32 bits */
static __inline td_void isp_fe_ae_wei_waddr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_wei_waddr)
{
    fe_reg->isp_ae_weight_waddr.u32 = uisp_ae_wei_waddr;
}

/*  description : set the value of the member isp_ae_weight_wdata.isp_ae_wei_wdata */
/*  input       : unsigned int uisp_ae_wei_wdata: 32 bits */
static __inline td_void isp_fe_ae_wei_wdata_write(isp_fe_reg_type *fe_reg, td_u32 uisp_ae_wei_wdata)
{
    fe_reg->isp_ae_weight_wdata.u32 = uisp_ae_wei_wdata;
}

static __inline td_void isp_fe_dg_blc_en_write(isp_fe_reg_type *fe_reg,
    td_u32 uisp_dg_en_in, td_u32 uisp_dg_en_out)
{
    fe_reg->isp_dg_blc_cfg.u32 = (uisp_dg_en_out << 1) + uisp_dg_en_in;
}

/*  description : set the value of the member isp_dg_gain1.isp_dg_grgain */
/*  input       : unsigned int uisp_dg_grgain: 16 bits */
static __inline td_void isp_fe_dg_grgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_grgain)
{
    u_isp_dg_gain1 o_isp_dg_gain1;
    o_isp_dg_gain1.u32 = fe_reg->isp_dg_gain1.u32;
    o_isp_dg_gain1.bits.isp_dg_grgain = uisp_dg_grgain;
    fe_reg->isp_dg_gain1.u32 = o_isp_dg_gain1.u32;
}

/*  description : set the value of the member isp_dg_gain1.isp_dg_rgain */
/*  input       : unsigned int uisp_dg_rgain: 16 bits */
static __inline td_void isp_fe_dg_rgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_rgain)
{
    u_isp_dg_gain1 o_isp_dg_gain1;
    o_isp_dg_gain1.u32 = fe_reg->isp_dg_gain1.u32;
    o_isp_dg_gain1.bits.isp_dg_rgain = uisp_dg_rgain;
    fe_reg->isp_dg_gain1.u32 = o_isp_dg_gain1.u32;
}

/*  description : set the value of the member isp_dg_gain2.isp_dg_gbgain */
/*  input       : unsigned int uisp_dg_gbgain: 16 bits */
static __inline td_void isp_fe_dg_gbgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_gbgain)
{
    u_isp_dg_gain2 o_isp_dg_gain2;
    o_isp_dg_gain2.u32 = fe_reg->isp_dg_gain2.u32;
    o_isp_dg_gain2.bits.isp_dg_gbgain = uisp_dg_gbgain;
    fe_reg->isp_dg_gain2.u32 = o_isp_dg_gain2.u32;
}

/*  description : set the value of the member isp_dg_gain2.isp_dg_bgain */
/*  input       : unsigned int uisp_dg_bgain: 16 bits */
static __inline td_void isp_fe_dg_bgain_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_bgain)
{
    u_isp_dg_gain2 o_isp_dg_gain2;
    o_isp_dg_gain2.u32 = fe_reg->isp_dg_gain2.u32;
    o_isp_dg_gain2.bits.isp_dg_bgain = uisp_dg_bgain;
    fe_reg->isp_dg_gain2.u32 = o_isp_dg_gain2.u32;
}

/*  description : set the value of the member isp_dg_blc_offset1.isp_dg_ofsgr */
/*  input       : unsigned int uisp_dg_ofsgr: 15 bits */
static __inline td_void isp_fe_dg_ofsgr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_ofsgr)
{
    u_isp_dg_blc_offset1 o_isp_dg_blc_offset1;
    o_isp_dg_blc_offset1.u32 = fe_reg->isp_dg_blc_offset1.u32;
    o_isp_dg_blc_offset1.bits.isp_dg_ofsgr = uisp_dg_ofsgr;
    fe_reg->isp_dg_blc_offset1.u32 = o_isp_dg_blc_offset1.u32;
}

/*  description : set the value of the member isp_dg_blc_offset1.isp_dg_ofsr */
/*  input       : unsigned int uisp_dgofsr: 15 bits */
static __inline td_void isp_fe_dg_ofsr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_ofsr)
{
    u_isp_dg_blc_offset1 o_isp_dg_blc_offset1;
    o_isp_dg_blc_offset1.u32 = fe_reg->isp_dg_blc_offset1.u32;
    o_isp_dg_blc_offset1.bits.isp_dg_ofsr = uisp_dg_ofsr;
    fe_reg->isp_dg_blc_offset1.u32 = o_isp_dg_blc_offset1.u32;
}

/*  description : set the value of the member isp_dg_blc_offset2.isp_dg_ofsgb */
/*  input       : unsigned int uisp_dg_ofsgb: 15 bits */
static __inline td_void isp_fe_dg_ofsgb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_ofsgb)
{
    u_isp_dg_blc_offset2 o_isp_dg_blc_offset2;
    o_isp_dg_blc_offset2.u32 = fe_reg->isp_dg_blc_offset2.u32;
    o_isp_dg_blc_offset2.bits.isp_dg_ofsgb = uisp_dg_ofsgb;
    fe_reg->isp_dg_blc_offset2.u32 = o_isp_dg_blc_offset2.u32;
}

/*  description : set the value of the member isp_dg_blc_offset2.isp_dg_ofsb */
/*  input       : unsigned int uisp_dg_ofsb: 15 bits */
static __inline td_void isp_fe_dg_ofsb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_ofsb)
{
    u_isp_dg_blc_offset2 o_isp_dg_blc_offset2;
    o_isp_dg_blc_offset2.u32 = fe_reg->isp_dg_blc_offset2.u32;
    o_isp_dg_blc_offset2.bits.isp_dg_ofsb = uisp_dg_ofsb;
    fe_reg->isp_dg_blc_offset2.u32 = o_isp_dg_blc_offset2.u32;
}

static __inline td_void isp_fe_dg_offset_write(isp_fe_reg_type *fe_reg, const td_u16 *puisp_dg_ofs)
{
    fe_reg->isp_dg_blc_offset1.u32 = ((td_u32)puisp_dg_ofs[0] << 16) + puisp_dg_ofs[1]; /* 16~31  r_blc */
    fe_reg->isp_dg_blc_offset2.u32 = ((td_u32)puisp_dg_ofs[3] << 16) + puisp_dg_ofs[2]; /* 16~31  b_blc, 2/3 gb/b */
}

/*  description : set the value of the member isp_dg2_clip_value.isp_dg2_clip_value */
/*  input       : unsigned int uisp_dg2_clip_value: 20 bits */
static __inline td_void isp_fe_dg_clip_value_write(isp_fe_reg_type *fe_reg, td_u32 uisp_dg_clip_value)
{
    u_isp_dg_clip_value o_isp_dg_clip_value;
    o_isp_dg_clip_value.u32 = fe_reg->isp_dg_clip_value.u32;
    o_isp_dg_clip_value.bits.isp_dg_clip_value = uisp_dg_clip_value;
    fe_reg->isp_dg_clip_value.u32 = o_isp_dg_clip_value.u32;
}

/*  description : set the value of the member isp_fe_gamma_lut_wdata.isp_fe_gamma_lut_wdata */
/*  input       : unsigned int uisp_fe_gamma_lut_wdata: 32 bits */
static __inline td_void isp_fe_gamma_bitw_write(isp_fe_reg_type *fe_reg, td_u32 uisp_gamma_bitw)
{
    u_isp_gamma_bitw isp_gamma_bitw;
    isp_gamma_bitw.u32 = fe_reg->isp_gamma_bitw.u32;
    isp_gamma_bitw.bits.isp_gamma_bitw_in = uisp_gamma_bitw;
    isp_gamma_bitw.bits.isp_gamma_bitw_out = uisp_gamma_bitw;
    fe_reg->isp_gamma_bitw.u32 = isp_gamma_bitw.u32;
}

/*  description : set the value of the member isp_fe_gamma_lut_wdata.isp_fe_gamma_lut_wdata */
/*  input       : unsigned int uisp_fe_gamma_lut_wdata: 32 bits */
static __inline td_void isp_fe_gamma_lut_wdata_write(isp_fe_reg_type *fe_reg, td_u32 uisp_gamma_lut_wdata)
{
    fe_reg->isp_gamma_lut_wdata.u32 = uisp_gamma_lut_wdata;
}

static __inline td_void isp_fe_gamma_lut_waddr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_gamma_lut_waddr)
{
    fe_reg->isp_gamma_lut_waddr.u32 = uisp_gamma_lut_waddr;
}

/*  description : set the value of the member isp_rc_blcen.isp_rc_blc_in_en */
/*  input       : unsigned int uisp_rc_blc_in_en: 1 bits */
static __inline td_void isp_fe_rc_blc_in_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_blc_in_en)
{
    u_isp_rc_blcen o_isp_rc_blcen;
    o_isp_rc_blcen.u32 = fe_reg->isp_rc_blcen.u32;
    o_isp_rc_blcen.bits.isp_rc_blc_in_en = uisp_rc_blc_in_en;
    fe_reg->isp_rc_blcen.u32 = o_isp_rc_blcen.u32;
}

/*  description : set the value of the member isp_rc_blcen.isp_rc_blc_out_en */
/*  input       : unsigned int uisp_rc_blc_out_en: 1 bits */
static __inline td_void isp_fe_rc_blc_out_en_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_blc_out_en)
{
    u_isp_rc_blcen o_isp_rc_blcen;
    o_isp_rc_blcen.u32 = fe_reg->isp_rc_blcen.u32;
    o_isp_rc_blcen.bits.isp_rc_blc_out_en = uisp_rc_blc_out_en;
    fe_reg->isp_rc_blcen.u32 = o_isp_rc_blcen.u32;
}

/*  description : set the value of the member isp_rc_blc0.isp_rc_blc_r */
/*  input       : unsigned int uisp_rc_blc_r: 15 bits */
static __inline td_void isp_fe_rc_blc_r_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_blc_r)
{
    u_isp_rc_blc0 o_isp_rc_blc0;
    o_isp_rc_blc0.u32 = fe_reg->isp_rc_blc0.u32;
    o_isp_rc_blc0.bits.isp_rc_blc_r = uisp_rc_blc_r;
    fe_reg->isp_rc_blc0.u32 = o_isp_rc_blc0.u32;
}

/*  description : set the value of the member isp_rc_blc0.isp_rc_blc_gr */
/*  input       : unsigned int uisp_rc_blc_gr: 15 bits */
static __inline td_void isp_fe_rc_blc_gr_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_blc_gr)
{
    u_isp_rc_blc0 o_isp_rc_blc0;
    o_isp_rc_blc0.u32 = fe_reg->isp_rc_blc0.u32;
    o_isp_rc_blc0.bits.isp_rc_blc_gr = uisp_rc_blc_gr;
    fe_reg->isp_rc_blc0.u32 = o_isp_rc_blc0.u32;
}

/*  description : set the value of the member isp_rc_blc1.isp_rc_blc_b */
/*  input       : unsigned int uisp_rc_blc_b: 15 bits */
static __inline td_void isp_fe_rc_blc_b_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_blc_b)
{
    u_isp_rc_blc1 o_isp_rc_blc1;
    o_isp_rc_blc1.u32 = fe_reg->isp_rc_blc1.u32;
    o_isp_rc_blc1.bits.isp_rc_blc_b = uisp_rc_blc_b;
    fe_reg->isp_rc_blc1.u32 = o_isp_rc_blc1.u32;
}

/*  description : set the value of the member isp_rc_blc1.isp_rc_blc_gb */
/*  input       : unsigned int uisp_rc_blc_gb: 15 bits */
static __inline td_void isp_fe_rc_blc_gb_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_blc_gb)
{
    u_isp_rc_blc1 o_isp_rc_blc1;
    o_isp_rc_blc1.u32 = fe_reg->isp_rc_blc1.u32;
    o_isp_rc_blc1.bits.isp_rc_blc_gb = uisp_rc_blc_gb;
    fe_reg->isp_rc_blc1.u32 = o_isp_rc_blc1.u32;
}

/*  description : set the value of the member isp_rc_sqradius.isp_rc_sqradius */
/*  input       : unsigned int uisp_rc_sqradius: 32 bits */
static __inline td_void isp_fe_rc_sqradius_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_sqradius)
{
    fe_reg->isp_rc_sqradius.u32 = uisp_rc_sqradius;
}


/*  description : set the value of the member isp_rc_center_coor.isp_rc_cenhor_coor */
/*  input       : unsigned int uisp_rc_cenhor_coor: 14 bits */
static __inline td_void isp_fe_rc_cenhor_coor_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_cenhor_coor)
{
    u_isp_rc_center_coor o_isp_rc_center_coor;
    o_isp_rc_center_coor.u32 = fe_reg->isp_rc_center_coor.u32;
    o_isp_rc_center_coor.bits.isp_rc_cenhor_coor = uisp_rc_cenhor_coor;
    fe_reg->isp_rc_center_coor.u32 = o_isp_rc_center_coor.u32;
}

/*  description : set the value of the member isp_rc_center_coor.isp_rc_cenver_coor */
/*  input       : unsigned int uisp_rc_cenver_coor: 14 bits */
static __inline td_void isp_fe_rc_cenver_coor_write(isp_fe_reg_type *fe_reg, td_u32 uisp_rc_cenver_coor)
{
    u_isp_rc_center_coor o_isp_rc_center_coor;
    o_isp_rc_center_coor.u32 = fe_reg->isp_rc_center_coor.u32;
    o_isp_rc_center_coor.bits.isp_rc_cenver_coor = uisp_rc_cenver_coor;
    fe_reg->isp_rc_center_coor.u32 = o_isp_rc_center_coor.u32;
}

/*  description   set the value of the member isp_crop_sizeout.isp_crop_width_out */
/*  input         unsigned int uisp_crop_width_out  16 bits */
static __inline td_void isp_fe_crop_size_out_write(isp_fe_reg_type *fe_reg, td_u32 uisp_crop_width_out,
                                                   td_u32 uisp_crop_height_out)
{
    fe_reg->isp_crop_sizeout.u32 = (uisp_crop_height_out << 16) + uisp_crop_width_out; /* 16~31  height */
}

/*  description   set the value of the member isp_crop_pos.isp_crop_pos_x */
/*  input         unsigned int uisp_crop_pos_x  16 bits */
static __inline td_void isp_fe_crop_pos_write(isp_fe_reg_type *fe_reg, td_u32 uisp_crop_pos_x, td_u32 uisp_crop_pos_y)
{
    fe_reg->isp_crop_pos.u32 = (uisp_crop_pos_y << 16) + uisp_crop_pos_x; /* 16~31  pos_y */
}

static __inline td_u8 isp_fe_ae_wei_raddr_write(ot_vi_pipe vi_pipe, td_u32 uisp_ae_wei_waddr)
{
    isp_fe_reg_type *fe_reg = (isp_fe_reg_type *)isp_get_fe_vir_addr(vi_pipe);
    isp_check_reg_nullptr_return(fe_reg);
    fe_reg->isp_ae_weight_raddr.u32 = uisp_ae_wei_waddr;
    return 1;
}

static __inline td_u32 isp_fe_ae_wei_rdata_read(td_u8 vi_pipe)
{
    isp_fe_reg_type *fe_reg = (isp_fe_reg_type *)isp_get_fe_vir_addr(vi_pipe);
    isp_check_reg_nullptr_return(fe_reg);
    return fe_reg->isp_ae_weight_rdata.u32;
}


/* BE BE BE BE BE BE BE BE BE */
static __inline td_void isp_pre_be_reg_list(td_void)
{
}

/*  description   set the value of the member isp_be_module_pos.isp_ae_sel */
/*  input         unsigned int uisp_ae_sel  2 bits */
static __inline td_void isp_ae_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_ae_sel = uisp_ae_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description   set the value of the member isp_be_module_pos.isp_awb_sel */
/*  input         unsigned int uisp_awb_sel  2 bits */
static __inline td_void isp_awb_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_awb_sel = uisp_awb_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description   set the value of the member isp_be_module_pos.isp_af_sel */
/*  input         unsigned int uisp_af_sel  2 bits */
static __inline td_void isp_af_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_af_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_af_sel = uisp_af_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description : set the value of the member isp_be_module_pos.isp_dis_sel */
/*  input       : unsigned int uisp_dis_sel: 1 bits */
static __inline td_void isp_dis_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_dis_sel = uisp_dis_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description   set the value of the member isp_be_module_pos.isp_clut_sel */
/*  input         unsigned int uisp_clut_sel  1 bits */
#define OT_ISP_CLUT_SEL_WRITE                         0x0
static __inline td_void isp_clut_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_clut_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_clut_sel = uisp_clut_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description : set the value of the member isp_be_module_pos.isp_dcg_sel */
/*  input       : unsigned int uisp_dcg_sel: 1 bits */
static __inline td_void isp_dcg_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_dcg_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_dcg_sel = uisp_dcg_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description : set the value of the member isp_be_module_pos.isp_mot_sel */
/*  input       : unsigned int uisp_mot_sel: 1 bits */
static __inline td_void isp_mot_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_mot_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_mot_sel = uisp_mot_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description : set the value of the member isp_be_module_pos.isp_wrnt_sel */
/*  input       : unsigned int uisp_wrnt_sel: 1 bits */
static __inline td_void isp_wrnt_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_wrnt_sel)
{
    u_isp_be_module_pos o_isp_be_module_pos;
    o_isp_be_module_pos.u32 = be_reg->isp_be_module_pos.u32;
    o_isp_be_module_pos.bits.isp_wrnt_sel = uisp_wrnt_sel;
    be_reg->isp_be_module_pos.u32 = o_isp_be_module_pos.u32;
}
/*  description   set the value of the member isp_be_fstart_delay.isp_delay */
/*  input         unsigned int uisp_delay  32 bits */
static __inline td_void isp_delay_write(isp_be_reg_type *be_reg, td_u32 uisp_delay)
{
    be_reg->isp_be_fstart_delay.u32 = uisp_delay;
}
/*  description   set the value of the member isp_be_input_mux.isp_input0_sel */
/*  input         unsigned int uisp_input0_sel  2 bits */
static __inline td_void isp_input_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_input_sel)
{
    be_reg->isp_be_input_mux.u32 = uisp_input_sel;
}


/*  description   set the value of the member isp_be_sys_ctrl.isp_stt_en */
/*  input         unsigned int uisp_stt_en  1 bits */
static __inline td_void isp_be_stt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_stt_en)
{
    u_isp_be_sys_ctrl o_isp_be_sys_ctrl;
    o_isp_be_sys_ctrl.u32 = be_reg->isp_be_sys_ctrl.u32;
    o_isp_be_sys_ctrl.bits.isp_stt_en = uisp_stt_en;
    be_reg->isp_be_sys_ctrl.u32 = o_isp_be_sys_ctrl.u32;
}
/*  description   set the value of the member isp_be_user_define0.isp_user_define0 */
/*  input         unsigned int uisp_user_define0  32 bits */
static __inline td_void isp_user_define0_write(isp_be_reg_type *be_reg, td_u32 uisp_user_define0)
{
    be_reg->isp_be_user_define0.u32 = uisp_user_define0;
}
/*  description   set the value of the member isp_be_user_define1.isp_user_define1 */
/*  input         unsigned int uisp_user_define1  32 bits */
static __inline td_void isp_user_define1_write(isp_be_reg_type *be_reg, td_u32 uisp_user_define1)
{
    be_reg->isp_be_user_define1.u32 = uisp_user_define1;
}
/*  description   set the value of the member isp_be_format.isp_format */
/*  input         unsigned int uisp_format  2 bits */
static __inline td_void isp_format_write(isp_be_reg_type *be_reg, td_u32 uisp_format)
{
    u_isp_be_format o_isp_be_format;
    o_isp_be_format.u32 = be_reg->isp_be_format.u32;
    o_isp_be_format.bits.isp_format = uisp_format;
    be_reg->isp_be_format.u32 = o_isp_be_format.u32;
}

/*  description : set the value of the member isp_be_mem_cfg.isp_mem_cfg */
/*  input       : unsigned int uisp_mem_cfg: 1 bits */
static __inline td_void isp_mem_cfg_write(isp_be_reg_type *be_reg, td_u32 uisp_mem_cfg)
{
    u_isp_be_mem_cfg o_isp_be_mem_cfg;
    o_isp_be_mem_cfg.u32 = be_reg->isp_be_mem_cfg.u32;
    o_isp_be_mem_cfg.bits.isp_mem_cfg = uisp_mem_cfg;
    be_reg->isp_be_mem_cfg.u32 = o_isp_be_mem_cfg.u32;
}

/*  description   set the value of the member isp_clip_y_cfg.isp_clip_y_min */
/*  input         unsigned int uisp_clip_y_min  16 bits */
#define ISP_CLIP_Y_MIN_DEFAULT                        0x0
static __inline td_void isp_clip_y_min_write(isp_be_reg_type *be_reg, td_u32 uisp_clip_y_min)
{
    u_isp_clip_y_cfg o_isp_clip_y_cfg;
    o_isp_clip_y_cfg.u32 = be_reg->isp_clip_y_cfg.u32;
    o_isp_clip_y_cfg.bits.isp_clip_y_min = uisp_clip_y_min;
    be_reg->isp_clip_y_cfg.u32 = o_isp_clip_y_cfg.u32;
}
/*  description   set the value of the member isp_clip_y_cfg.isp_clip_y_max */
/*  input         unsigned int uisp_clip_y_max  16 bits */
#define ISP_CLIP_Y_MAX_DEFAULT                        0xFFFF
static __inline td_void isp_clip_y_max_write(isp_be_reg_type *be_reg, td_u32 uisp_clip_y_max)
{
    u_isp_clip_y_cfg o_isp_clip_y_cfg;
    o_isp_clip_y_cfg.u32 = be_reg->isp_clip_y_cfg.u32;
    o_isp_clip_y_cfg.bits.isp_clip_y_max = uisp_clip_y_max;
    be_reg->isp_clip_y_cfg.u32 = o_isp_clip_y_cfg.u32;
}
/*  description   set the value of the member isp_clip_c_cfg.isp_clip_c_min */
/*  input         unsigned int uisp_clip_c_min  16 bits */
#define ISP_CLIP_C_MIN_DEFAULT                        0x0
static __inline td_void isp_clip_c_min_write(isp_be_reg_type *be_reg, td_u32 uisp_clip_c_min)
{
    u_isp_clip_c_cfg o_isp_clip_c_cfg;
    o_isp_clip_c_cfg.u32 = be_reg->isp_clip_c_cfg.u32;
    o_isp_clip_c_cfg.bits.isp_clip_c_min = uisp_clip_c_min;
    be_reg->isp_clip_c_cfg.u32 = o_isp_clip_c_cfg.u32;
}
/*  description   set the value of the member isp_clip_c_cfg.isp_clip_c_max */
/*  input         unsigned int uisp_clip_c_max  16 bits */
#define ISP_CLIP_C_MAX_DEFAULT                        0xFFFF
static __inline td_void isp_clip_c_max_write(isp_be_reg_type *be_reg, td_u32 uisp_clip_c_max)
{
    u_isp_clip_c_cfg o_isp_clip_c_cfg;
    o_isp_clip_c_cfg.u32 = be_reg->isp_clip_c_cfg.u32;
    o_isp_clip_c_cfg.bits.isp_clip_c_max = uisp_clip_c_max;
    be_reg->isp_clip_c_cfg.u32 = o_isp_clip_c_cfg.u32;
}

/*  description : set the value of the member isp_csc_sum_cfg.isp_csc_sum_en */
/*  input       : unsigned int uisp_csc_sum_en: 1 bits */
static __inline td_void isp_csc_sum_en_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_sum_en)
{
    u_isp_csc_sum_cfg o_isp_csc_sum_cfg;
    o_isp_csc_sum_cfg.u32 = be_reg->isp_csc_sum_cfg.u32;
    o_isp_csc_sum_cfg.bits.isp_csc_sum_en = uisp_csc_sum_en;
    be_reg->isp_csc_sum_cfg.u32 = o_isp_csc_sum_cfg.u32;
}

/*  description : set the value of the member isp_yuv444_sum_cfg.isp_yuv444_sum_en */
/*  input       : unsigned int uisp_yuv444_sum_en: 1 bits */
static __inline td_void isp_yuv444_sum_en_write(isp_be_reg_type *be_reg, td_u32 uisp_yuv444_sum_en)
{
    u_isp_yuv444_sum_cfg o_isp_yuv444_sum_cfg;
    o_isp_yuv444_sum_cfg.u32 = be_reg->isp_yuv444_sum_cfg.u32;
    o_isp_yuv444_sum_cfg.bits.isp_yuv444_sum_en = uisp_yuv444_sum_en;
    be_reg->isp_yuv444_sum_cfg.u32 = o_isp_yuv444_sum_cfg.u32;
}

/*  description : set the value of the member isp_yuv422_sum_cfg.isp_yuv422_sum_en */
/*  input       : unsigned int uisp_yuv422_sum_en: 1 bits */
static __inline td_void isp_yuv422_sum_en_write(isp_be_reg_type *be_reg, td_u32 uisp_yuv422_sum_en)
{
    u_isp_yuv422_sum_cfg o_isp_yuv422_sum_cfg;
    o_isp_yuv422_sum_cfg.u32 = be_reg->isp_yuv422_sum_cfg.u32;
    o_isp_yuv422_sum_cfg.bits.isp_yuv422_sum_en = uisp_yuv422_sum_en;
    be_reg->isp_yuv422_sum_cfg.u32 = o_isp_yuv422_sum_cfg.u32;
}

/*  description : set the value of the member isp_wdr_sum_cfg.isp_wdr_sum_en */
/*  input       : unsigned int uisp_wdr_sum_en: 1 bits */
static __inline td_void isp_wdr_sum_en_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_sum_en)
{
    u_isp_wdr_sum_cfg o_isp_wdr_sum_cfg;
    o_isp_wdr_sum_cfg.u32 = be_reg->isp_wdr_sum_cfg.u32;
    o_isp_wdr_sum_cfg.bits.isp_wdr_sum_en = uisp_wdr_sum_en;
    be_reg->isp_wdr_sum_cfg.u32 = o_isp_wdr_sum_cfg.u32;
}

/*  description : set the value of the member isp_demosaic_sum_cfg.isp_demosaic_sum_en */
/*  input       : unsigned int uisp_demosaic_sum_en: 1 bits */
static __inline td_void isp_demosaic_sum_en_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_sum_en)
{
    u_isp_demosaic_sum_cfg o_isp_demosaic_sum_cfg;
    o_isp_demosaic_sum_cfg.u32 = be_reg->isp_demosaic_sum_cfg.u32;
    o_isp_demosaic_sum_cfg.bits.isp_demosaic_sum_en = uisp_demosaic_sum_en;
    be_reg->isp_demosaic_sum_cfg.u32 = o_isp_demosaic_sum_cfg.u32;
}

/*  description : set the value of the member isp_dmnr_dither.isp_dmnr_dither_en */
/*  input       : unsigned int uisp_dmnr_dither_en: 1 bits */
static __inline td_void isp_dmnr_dither_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dmnr_dither_en)
{
    u_isp_dmnr_dither o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = be_reg->isp_dmnr_dither.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_en = uisp_dmnr_dither_en;
    be_reg->isp_dmnr_dither.u32 = o_isp_dmnr_dither.u32;
}
/*  description   set the value of the member isp_dmnr_dither.isp_dmnr_dither_round */
/*  input         unsigned int uisp_dmnr_dither_round  2 bits */
#define ISP_DMNR_DITHER_ROUND_DEFAULT              2
static __inline td_void isp_dmnr_dither_round_write(isp_be_reg_type *be_reg, td_u32 uisp_dmnr_dither_round)
{
    u_isp_dmnr_dither o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = be_reg->isp_dmnr_dither.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_round = uisp_dmnr_dither_round;
    be_reg->isp_dmnr_dither.u32 = o_isp_dmnr_dither.u32;
}
/*  description   set the value of the member isp_dmnr_dither.isp_dmnr_dither_spatial_mode */
/*  input         unsigned int uisp_dmnr_dither_spatial_mode  1 bits */
#define ISP_DMNR_DITHER_SPATIAL_MODE_DEFAULT       1
static __inline td_void isp_dmnr_dither_spatial_mode_write(isp_be_reg_type *be_reg,
    td_u32 uisp_dmnr_dither_spatial_mode)
{
    u_isp_dmnr_dither o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = be_reg->isp_dmnr_dither.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_spatial_mode = uisp_dmnr_dither_spatial_mode;
    be_reg->isp_dmnr_dither.u32 = o_isp_dmnr_dither.u32;
}
/*  description   set the value of the member isp_dmnr_dither.isp_dmnr_dither_out_bits */
/*  input         unsigned int uisp_dmnr_dither_out_bits  4 bits */
#define ISP_DMNR_DITHER_OUT_BITS_DEFAULT           10
static __inline td_void isp_dmnr_dither_out_bits_write(isp_be_reg_type *be_reg, td_u32 uisp_dmnr_dither_out_bits)
{
    u_isp_dmnr_dither o_isp_dmnr_dither;
    o_isp_dmnr_dither.u32 = be_reg->isp_dmnr_dither.u32;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_out_bits = uisp_dmnr_dither_out_bits;
    be_reg->isp_dmnr_dither.u32 = o_isp_dmnr_dither.u32;
}

/*  description : set the value of the member isp_acm_dither.isp_acm_dither_en */
/*  input       : unsigned int uisp_acm_dither_en: 1 bits */
static __inline td_void isp_acm_dither_en_write(isp_be_reg_type *be_reg, td_u32 uisp_acm_dither_en)
{
    u_isp_acm_dither o_isp_acm_dither;
    o_isp_acm_dither.u32 = be_reg->isp_acm_dither.u32;
    o_isp_acm_dither.bits.isp_acm_dither_en = uisp_acm_dither_en;
    be_reg->isp_acm_dither.u32 = o_isp_acm_dither.u32;
}

/*  description : set the value of the member isp_acm_dither.isp_acm_dither_round */
/*  input       : unsigned int uisp_acm_dither_round: 2 bits */
static __inline td_void isp_acm_dither_round_write(isp_be_reg_type *be_reg, td_u32 uisp_acm_dither_round)
{
    u_isp_acm_dither o_isp_acm_dither;
    o_isp_acm_dither.u32 = be_reg->isp_acm_dither.u32;
    o_isp_acm_dither.bits.isp_acm_dither_round = uisp_acm_dither_round;
    be_reg->isp_acm_dither.u32 = o_isp_acm_dither.u32;
}

/*  description : set the value of the member isp_acm_dither.isp_acm_dither_spatial_mode */
/*  input       : unsigned int uisp_acm_dither_spatial_mode: 1 bits */
static __inline td_void isp_acm_dither_spatial_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_acm_dither_spatial_mode)
{
    u_isp_acm_dither o_isp_acm_dither;
    o_isp_acm_dither.u32 = be_reg->isp_acm_dither.u32;
    o_isp_acm_dither.bits.isp_acm_dither_spatial_mode = uisp_acm_dither_spatial_mode;
    be_reg->isp_acm_dither.u32 = o_isp_acm_dither.u32;
}

/*  description : set the value of the member isp_acm_dither.isp_acm_dither_out_bits */
/*  input       : unsigned int uisp_acm_dither_out_bits: 4 bits */
static __inline td_void isp_acm_dither_out_bits_write(isp_be_reg_type *be_reg, td_u32 uisp_acm_dither_out_bits)
{
    u_isp_acm_dither o_isp_acm_dither;
    o_isp_acm_dither.u32 = be_reg->isp_acm_dither.u32;
    o_isp_acm_dither.bits.isp_acm_dither_out_bits = uisp_acm_dither_out_bits;
    be_reg->isp_acm_dither.u32 = o_isp_acm_dither.u32;
}
/*  description   set the value of the member isp_drc_dither.isp_drc_dither_en */
/*  input         unsigned int uisp_drc_dither_en  1 bits */
static __inline td_void isp_drc_dither_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dither_en)
{
    u_isp_drc_dither o_isp_drc_dither;
    o_isp_drc_dither.u32 = be_reg->isp_drc_dither.u32;
    o_isp_drc_dither.bits.isp_drc_dither_en = uisp_drc_dither_en;
    be_reg->isp_drc_dither.u32 = o_isp_drc_dither.u32;
}
/*  description   set the value of the member isp_drc_dither.isp_drc_dither_round */
/*  input         unsigned int uisp_drc_dither_round  2 bits */
#define ISP_DRC_DITHER_ROUND_DEFAULT               2
static __inline td_void isp_drc_dither_round_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dither_round)
{
    u_isp_drc_dither o_isp_drc_dither;
    o_isp_drc_dither.u32 = be_reg->isp_drc_dither.u32;
    o_isp_drc_dither.bits.isp_drc_dither_round = uisp_drc_dither_round;
    be_reg->isp_drc_dither.u32 = o_isp_drc_dither.u32;
}
/*  description   set the value of the member isp_drc_dither.isp_drc_dither_spatial_mode */
/*  input         unsigned int uisp_drc_dither_spatial_mode  1 bits */
#define ISP_DRC_DITHER_SPATIAL_MODE_DEFAULT        1
static __inline td_void isp_drc_dither_spatial_mode_write(isp_be_reg_type *be_reg, td_u32 drc_dither_spatial_mode)
{
    u_isp_drc_dither o_isp_drc_dither;
    o_isp_drc_dither.u32 = be_reg->isp_drc_dither.u32;
    o_isp_drc_dither.bits.isp_drc_dither_spatial_mode = drc_dither_spatial_mode;
    be_reg->isp_drc_dither.u32 = o_isp_drc_dither.u32;
}
/*  description   set the value of the member isp_drc_dither.isp_drc_dither_out_bits */
/*  input         unsigned int uisp_drc_dither_out_bits  4 bits */
#define ISP_DRC_DITHER_OUT_BITS_DEFAULT            12
static __inline td_void isp_drc_dither_out_bits_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dither_out_bits)
{
    u_isp_drc_dither o_isp_drc_dither;
    o_isp_drc_dither.u32 = be_reg->isp_drc_dither.u32;
    o_isp_drc_dither.bits.isp_drc_dither_out_bits = uisp_drc_dither_out_bits;
    be_reg->isp_drc_dither.u32 = o_isp_drc_dither.u32;
}

/*  description : set the value of the member isp_sqrt1_dither.isp_sqrt1_dither_en */
/*  input       : unsigned int uisp_sqrt1_dither_en: 1 bits */
static __inline td_void isp_sqrt1_dither_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sqrt1_dither_en)
{
    u_isp_sqrt1_dither o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = be_reg->isp_sqrt1_dither.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_en = uisp_sqrt1_dither_en;
    be_reg->isp_sqrt1_dither.u32 = o_isp_sqrt1_dither.u32;
}

/*  description : set the value of the member isp_sqrt1_dither.isp_sqrt1_dither_round */
/*  input       : unsigned int uisp_sqrt1_dither_round: 2 bits */
static __inline td_void isp_sqrt1_dither_round_write(isp_be_reg_type *be_reg, td_u32 uisp_sqrt1_dither_round)
{
    u_isp_sqrt1_dither o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = be_reg->isp_sqrt1_dither.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_round = uisp_sqrt1_dither_round;
    be_reg->isp_sqrt1_dither.u32 = o_isp_sqrt1_dither.u32;
}

/*  description : set the value of the member isp_sqrt1_dither.isp_sqrt1_dither_spatial_mode */
/*  input       : unsigned int uisp_sqrt1_dither_spatial_mode: 1 bits */
static __inline td_void isp_sqrt1_dither_spatial_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_sqrt1_dither_spatial_mode)
{
    u_isp_sqrt1_dither o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = be_reg->isp_sqrt1_dither.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_spatial_mode = uisp_sqrt1_dither_spatial_mode;
    be_reg->isp_sqrt1_dither.u32 = o_isp_sqrt1_dither.u32;
}

/*  description : set the value of the member isp_sqrt1_dither.isp_sqrt1_dither_out_bits */
/*  input       : unsigned int uisp_sqrt1_dither_out_bits: 4 bits */
static __inline td_void isp_sqrt1_dither_out_bits_write(isp_be_reg_type *be_reg, td_u32 uisp_sqrt1_dither_out_bits)
{
    u_isp_sqrt1_dither o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.u32 = be_reg->isp_sqrt1_dither.u32;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_out_bits = uisp_sqrt1_dither_out_bits;
    be_reg->isp_sqrt1_dither.u32 = o_isp_sqrt1_dither.u32;
}

/*  description   set the value of the member isp_blk_hblank.isp_blk_b_hblank */
/*  input         unsigned int uisp_blk_b_hblank  16 bits */
#define OT_ISP_BLK_B_HBLANK_DEFAULT                   0x20
static __inline td_void isp_blk_b_hblank_write(isp_be_reg_type *be_reg, td_u32 uisp_blk_b_hblank)
{
    u_isp_blk_hblank o_isp_blk_hblank;
    o_isp_blk_hblank.u32 = be_reg->isp_blk_hblank.u32;
    o_isp_blk_hblank.bits.isp_blk_b_hblank = uisp_blk_b_hblank;
    be_reg->isp_blk_hblank.u32 = o_isp_blk_hblank.u32;
}
/*  description   set the value of the member isp_blk_hblank.isp_blk_f_hblank */
/*  input         unsigned int uisp_blk_f_hblank  16 bits */
#define OT_ISP_BLK_F_HBLANK_DEFAULT                   0x18
static __inline td_void isp_blk_f_hblank_write(isp_be_reg_type *be_reg, td_u32 uisp_blk_f_hblank)
{
    u_isp_blk_hblank o_isp_blk_hblank;
    o_isp_blk_hblank.u32 = be_reg->isp_blk_hblank.u32;
    o_isp_blk_hblank.bits.isp_blk_f_hblank = uisp_blk_f_hblank;
    be_reg->isp_blk_hblank.u32 = o_isp_blk_hblank.u32;
}
/*  description   set the value of the member isp_blk_vblank.isp_blk_b_vblank */
/*  input         unsigned int uisp_blk_b_vblank  16 bits */
#define OT_ISP_BLK_B_VBLANK_DEFAULT                   0x0
static __inline td_void isp_blk_b_vblank_write(isp_be_reg_type *be_reg, td_u32 uisp_blk_b_vblank)
{
    u_isp_blk_vblank o_isp_blk_vblank;
    o_isp_blk_vblank.u32 = be_reg->isp_blk_vblank.u32;
    o_isp_blk_vblank.bits.isp_blk_b_vblank = uisp_blk_b_vblank;
    be_reg->isp_blk_vblank.u32 = o_isp_blk_vblank.u32;
}
/*  description   set the value of the member isp_blk_vblank.isp_blk_f_vblank */
/*  input         unsigned int uisp_blk_f_vblank  16 bits */
#define OT_ISP_BLK_F_VBLANK_DEFAULT                   0x0
static __inline td_void isp_blk_f_vblank_write(isp_be_reg_type *be_reg, td_u32 uisp_blk_f_vblank)
{
    u_isp_blk_vblank o_isp_blk_vblank;
    o_isp_blk_vblank.u32 = be_reg->isp_blk_vblank.u32;
    o_isp_blk_vblank.bits.isp_blk_f_vblank = uisp_blk_f_vblank;
    be_reg->isp_blk_vblank.u32 = o_isp_blk_vblank.u32;
}
/*  description   set the value of the member isp_blc_offset1.isp_blc_ofsgr */
/*  input         unsigned int uisp_blc_ofsgr  15 bits */
static __inline td_void isp_blc_ofsgr_write(isp_be_reg_type *be_reg, td_u32 uisp_blc_ofsgr)
{
    u_isp_blc_offset1 o_isp_blc_offset1;
    o_isp_blc_offset1.u32 = be_reg->isp_blc_offset1.u32;
    o_isp_blc_offset1.bits.isp_blc_ofsgr = uisp_blc_ofsgr;
    be_reg->isp_blc_offset1.u32 = o_isp_blc_offset1.u32;
}
/*  description   set the value of the member isp_blc_offset1.isp_blc_ofsr */
/*  input         unsigned int uisp_blc_ofsr  15 bits */
static __inline td_void isp_blc_ofsr_write(isp_be_reg_type *be_reg, td_u32 uisp_blc_ofsr)
{
    u_isp_blc_offset1 o_isp_blc_offset1;
    o_isp_blc_offset1.u32 = be_reg->isp_blc_offset1.u32;
    o_isp_blc_offset1.bits.isp_blc_ofsr = uisp_blc_ofsr;
    be_reg->isp_blc_offset1.u32 = o_isp_blc_offset1.u32;
}
/*  description   set the value of the member isp_blc_offset2.isp_blc_ofsgb */
/*  input         unsigned int uisp_blc_ofsgb  15 bits */
static __inline td_void isp_blc_ofsgb_write(isp_be_reg_type *be_reg, td_u32 uisp_blc_ofsgb)
{
    u_isp_blc_offset2 o_isp_blc_offset2;
    o_isp_blc_offset2.u32 = be_reg->isp_blc_offset2.u32;
    o_isp_blc_offset2.bits.isp_blc_ofsgb = uisp_blc_ofsgb;
    be_reg->isp_blc_offset2.u32 = o_isp_blc_offset2.u32;
}
/*  description   set the value of the member isp_blc_offset2.isp_blc_ofsb */
/*  input         unsigned int uisp_blc_ofsb  15 bits */
static __inline td_void isp_blc_ofsb_write(isp_be_reg_type *be_reg, td_u32 uisp_blc_ofsb)
{
    u_isp_blc_offset2 o_isp_blc_offset2;
    o_isp_blc_offset2.u32 = be_reg->isp_blc_offset2.u32;
    o_isp_blc_offset2.bits.isp_blc_ofsb = uisp_blc_ofsb;
    be_reg->isp_blc_offset2.u32 = o_isp_blc_offset2.u32;
}

/*  description   set the value of the member isp_wb_blc_cfg.isp_wb_en_in */
/*  input         unsigned int uisp_wb_en_in  1 bits */
static __inline td_void isp_wb_en_in_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_en_in)
{
    u_isp_wb_blc_cfg o_isp_wb_blc_cfg;
    o_isp_wb_blc_cfg.u32 = be_reg->isp_wb_blc_cfg.u32;
    o_isp_wb_blc_cfg.bits.isp_wb_en_in = uisp_wb_en_in;
    be_reg->isp_wb_blc_cfg.u32 = o_isp_wb_blc_cfg.u32;
}
/*  description   set the value of the member isp_wb_blc_cfg.isp_wb_en_out */
/*  input         unsigned int uisp_wb_en_out  1 bits */
static __inline td_void isp_wb_en_out_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_en_out)
{
    u_isp_wb_blc_cfg o_isp_wb_blc_cfg;
    o_isp_wb_blc_cfg.u32 = be_reg->isp_wb_blc_cfg.u32;
    o_isp_wb_blc_cfg.bits.isp_wb_en_out = uisp_wb_en_out;
    be_reg->isp_wb_blc_cfg.u32 = o_isp_wb_blc_cfg.u32;
}

/*  description   set the value of the member isp_wb_gain1.isp_wb_grgain */
/*  input         unsigned int uisp_wb_grgain  16 bits */
static __inline td_void isp_wb_grgain_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_grgain)
{
    u_isp_wb_gain1 o_isp_wb_gain1;
    o_isp_wb_gain1.u32 = be_reg->isp_wb_gain1.u32;
    o_isp_wb_gain1.bits.isp_wb_grgain = uisp_wb_grgain;
    be_reg->isp_wb_gain1.u32 = o_isp_wb_gain1.u32;
}
/*  description   set the value of the member isp_wb_gain1.isp_wb_rgain */
/*  input         unsigned int uisp_wb_rgain  16 bits */
static __inline td_void isp_wb_rgain_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_rgain)
{
    u_isp_wb_gain1 o_isp_wb_gain1;
    o_isp_wb_gain1.u32 = be_reg->isp_wb_gain1.u32;
    o_isp_wb_gain1.bits.isp_wb_rgain = uisp_wb_rgain;
    be_reg->isp_wb_gain1.u32 = o_isp_wb_gain1.u32;
}
/*  description   set the value of the member isp_wb_gain2.isp_wb_gbgain */
/*  input         unsigned int uisp_wb_gbgain  16 bits */
static __inline td_void isp_wb_gbgain_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_gbgain)
{
    u_isp_wb_gain2 o_isp_wb_gain2;
    o_isp_wb_gain2.u32 = be_reg->isp_wb_gain2.u32;
    o_isp_wb_gain2.bits.isp_wb_gbgain = uisp_wb_gbgain;
    be_reg->isp_wb_gain2.u32 = o_isp_wb_gain2.u32;
}
/*  description   set the value of the member isp_wb_gain2.isp_wb_bgain */
/*  input         unsigned int uisp_wb_bgain  16 bits */
static __inline td_void isp_wb_bgain_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_bgain)
{
    u_isp_wb_gain2 o_isp_wb_gain2;
    o_isp_wb_gain2.u32 = be_reg->isp_wb_gain2.u32;
    o_isp_wb_gain2.bits.isp_wb_bgain = uisp_wb_bgain;
    be_reg->isp_wb_gain2.u32 = o_isp_wb_gain2.u32;
}
/*  description   set the value of the member isp_wb_blc_offset1.isp_wb_ofsgr */
/*  input         unsigned int uisp_wb_ofsgr  15 bits */
static __inline td_void isp_wb_ofsgr_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_ofsgr)
{
    u_isp_wb_blc_offset1 o_isp_wb_blc_offset1;
    o_isp_wb_blc_offset1.u32 = be_reg->isp_wb_blc_offset1.u32;
    o_isp_wb_blc_offset1.bits.isp_wb_ofsgr = uisp_wb_ofsgr;
    be_reg->isp_wb_blc_offset1.u32 = o_isp_wb_blc_offset1.u32;
}
/*  description   set the value of the member isp_wb_blc_offset1.isp_wb_ofsr */
/*  input         unsigned int uisp_wb_ofsr  15 bits */
static __inline td_void isp_wb_ofsr_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_ofsr)
{
    u_isp_wb_blc_offset1 o_isp_wb_blc_offset1;
    o_isp_wb_blc_offset1.u32 = be_reg->isp_wb_blc_offset1.u32;
    o_isp_wb_blc_offset1.bits.isp_wb_ofsr = uisp_wb_ofsr;
    be_reg->isp_wb_blc_offset1.u32 = o_isp_wb_blc_offset1.u32;
}
/*  description   set the value of the member isp_wb_blc_offset2.isp_wb_ofsgb */
/*  input         unsigned int uisp_wb_ofsgb  15 bits */
static __inline td_void isp_wb_ofsgb_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_ofsgb)
{
    u_isp_wb_blc_offset2 o_isp_wb_blc_offset2;
    o_isp_wb_blc_offset2.u32 = be_reg->isp_wb_blc_offset2.u32;
    o_isp_wb_blc_offset2.bits.isp_wb_ofsgb = uisp_wb_ofsgb;
    be_reg->isp_wb_blc_offset2.u32 = o_isp_wb_blc_offset2.u32;
}
/*  description   set the value of the member isp_wb_blc_offset2.isp_wb_ofsb */
/*  input         unsigned int uisp_wb_ofsb  15 bits */
static __inline td_void isp_wb_ofsb_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_ofsb)
{
    u_isp_wb_blc_offset2 o_isp_wb_blc_offset2;
    o_isp_wb_blc_offset2.u32 = be_reg->isp_wb_blc_offset2.u32;
    o_isp_wb_blc_offset2.bits.isp_wb_ofsb = uisp_wb_ofsb;
    be_reg->isp_wb_blc_offset2.u32 = o_isp_wb_blc_offset2.u32;
}
/*  description   set the value of the member isp_wb_clip_value.isp_wb_clip_value */
/*  input         unsigned int uisp_wb_clip_value  20 bits */
static __inline td_void isp_wb_clip_value_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_clip_value)
{
    u_isp_wb_clip_value o_isp_wb_clip_value;
    o_isp_wb_clip_value.u32 = be_reg->isp_wb_clip_value.u32;
    o_isp_wb_clip_value.bits.isp_wb_clip_value = uisp_wb_clip_value;
    be_reg->isp_wb_clip_value.u32 = o_isp_wb_clip_value.u32;
}
/*  description : set the value of the member isp_wb_blc_offset_shift.isp_wb_blc_offset_shift */
/*  input       : unsigned int uisp_wb_blc_offset_shift: 4 bits */
static __inline td_void isp_wb_blc_offset_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_wb_blc_offset_shift)
{
    u_isp_wb_blc_offset_shift o_isp_wb_blc_offset_shift;
    o_isp_wb_blc_offset_shift.u32 = be_reg->isp_wb_blc_offset_shift.u32;
    o_isp_wb_blc_offset_shift.bits.isp_wb_blc_offset_shift = uisp_wb_blc_offset_shift;
    be_reg->isp_wb_blc_offset_shift.u32 = o_isp_wb_blc_offset_shift.u32;
}

/*  description   set the value of the member isp_dg_blc_cfg.isp_dg_en_in */
/*  input         unsigned int uisp_dg_en_in  1 bits */
static __inline td_void isp_dg_en_in_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_en_in)
{
    u_isp_dg_blc_cfg o_isp_dg_blc_cfg;
    o_isp_dg_blc_cfg.u32 = be_reg->isp_dg_blc_cfg.u32;
    o_isp_dg_blc_cfg.bits.isp_dg_en_in = uisp_dg_en_in;
    be_reg->isp_dg_blc_cfg.u32 = o_isp_dg_blc_cfg.u32;
}
/*  description   set the value of the member isp_dg_blc_cfg.isp_dg_en_out */
/*  input         unsigned int uisp_dg_en_out  1 bits */
static __inline td_void isp_dg_en_out_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_en_out)
{
    u_isp_dg_blc_cfg o_isp_dg_blc_cfg;
    o_isp_dg_blc_cfg.u32 = be_reg->isp_dg_blc_cfg.u32;
    o_isp_dg_blc_cfg.bits.isp_dg_en_out = uisp_dg_en_out;
    be_reg->isp_dg_blc_cfg.u32 = o_isp_dg_blc_cfg.u32;
}

/*  description   set the value of the member isp_dg_gain1.isp_dg_grgain */
/*  input         unsigned int uisp_dg_grgain  16 bits */
static __inline td_void isp_dg_grgain_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_grgain)
{
    u_isp_dg_gain1 o_isp_dg_gain1;
    o_isp_dg_gain1.u32 = be_reg->isp_dg_gain1.u32;
    o_isp_dg_gain1.bits.isp_dg_grgain = uisp_dg_grgain;
    be_reg->isp_dg_gain1.u32 = o_isp_dg_gain1.u32;
}
/*  description   set the value of the member isp_dg_gain1.isp_dg_rgain */
/*  input         unsigned int uisp_dg_rgain  16 bits */
static __inline td_void isp_dg_rgain_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_rgain)
{
    u_isp_dg_gain1 o_isp_dg_gain1;
    o_isp_dg_gain1.u32 = be_reg->isp_dg_gain1.u32;
    o_isp_dg_gain1.bits.isp_dg_rgain = uisp_dg_rgain;
    be_reg->isp_dg_gain1.u32 = o_isp_dg_gain1.u32;
}
/*  description   set the value of the member isp_dg_gain2.isp_dg_gbgain */
/*  input         unsigned int uisp_dg_gbgain  16 bits */
static __inline td_void isp_dg_gbgain_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_gbgain)
{
    u_isp_dg_gain2 o_isp_dg_gain2;
    o_isp_dg_gain2.u32 = be_reg->isp_dg_gain2.u32;
    o_isp_dg_gain2.bits.isp_dg_gbgain = uisp_dg_gbgain;
    be_reg->isp_dg_gain2.u32 = o_isp_dg_gain2.u32;
}
/*  description   set the value of the member isp_dg_gain2.isp_dg_bgain */
/*  input         unsigned int uisp_dg_bgain  16 bits */
static __inline td_void isp_dg_bgain_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_bgain)
{
    u_isp_dg_gain2 o_isp_dg_gain2;
    o_isp_dg_gain2.u32 = be_reg->isp_dg_gain2.u32;
    o_isp_dg_gain2.bits.isp_dg_bgain = uisp_dg_bgain;
    be_reg->isp_dg_gain2.u32 = o_isp_dg_gain2.u32;
}
/*  description   set the value of the member isp_dg_blc_offset1.isp_dg_ofsgr */
/*  input         unsigned int uisp_dg_ofsgr  15 bits */
static __inline td_void isp_dg_ofsgr_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_ofsgr)
{
    u_isp_dg_blc_offset1 o_isp_dg_blc_offset1;
    o_isp_dg_blc_offset1.u32 = be_reg->isp_dg_blc_offset1.u32;
    o_isp_dg_blc_offset1.bits.isp_dg_ofsgr = uisp_dg_ofsgr;
    be_reg->isp_dg_blc_offset1.u32 = o_isp_dg_blc_offset1.u32;
}
/*  description   set the value of the member isp_dg_blc_offset1.isp_dg_ofsr */
/*  input         unsigned int uisp_dg_ofsr  15 bits */
static __inline td_void isp_dg_ofsr_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_ofsr)
{
    u_isp_dg_blc_offset1 o_isp_dg_blc_offset1;
    o_isp_dg_blc_offset1.u32 = be_reg->isp_dg_blc_offset1.u32;
    o_isp_dg_blc_offset1.bits.isp_dg_ofsr = uisp_dg_ofsr;
    be_reg->isp_dg_blc_offset1.u32 = o_isp_dg_blc_offset1.u32;
}
/*  description   set the value of the member isp_dg_blc_offset2.isp_dg_ofsgb */
/*  input         unsigned int uisp_dg_ofsgb  15 bits */
static __inline td_void isp_dg_ofsgb_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_ofsgb)
{
    u_isp_dg_blc_offset2 o_isp_dg_blc_offset2;
    o_isp_dg_blc_offset2.u32 = be_reg->isp_dg_blc_offset2.u32;
    o_isp_dg_blc_offset2.bits.isp_dg_ofsgb = uisp_dg_ofsgb;
    be_reg->isp_dg_blc_offset2.u32 = o_isp_dg_blc_offset2.u32;
}
/*  description   set the value of the member isp_dg_blc_offset2.isp_dg_ofsb */
/*  input         unsigned int uisp_dg_ofsb  15 bits */
static __inline td_void isp_dg_ofsb_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_ofsb)
{
    u_isp_dg_blc_offset2 o_isp_dg_blc_offset2;
    o_isp_dg_blc_offset2.u32 = be_reg->isp_dg_blc_offset2.u32;
    o_isp_dg_blc_offset2.bits.isp_dg_ofsb = uisp_dg_ofsb;
    be_reg->isp_dg_blc_offset2.u32 = o_isp_dg_blc_offset2.u32;
}
/*  description   set the value of the member isp_dg_clip_value.isp_dg_clip_value */
/*  input         unsigned int uisp_dg_clip_value  20 bits */
static __inline td_void isp_dg_clip_value_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_clip_value)
{
    u_isp_dg_clip_value o_isp_dg_clip_value;
    o_isp_dg_clip_value.u32 = be_reg->isp_dg_clip_value.u32;
    o_isp_dg_clip_value.bits.isp_dg_clip_value = uisp_dg_clip_value;
    be_reg->isp_dg_clip_value.u32 = o_isp_dg_clip_value.u32;
}

/*  description : set the value of the member isp_dg_blc_offset_shift.isp_dg_blc_offset_shift */
/*  input       : unsigned int uisp_dg_blc_offset_shift: 4 bits */
static __inline td_void isp_dg_blc_offset_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_dg_blc_offset_shift)
{
    u_isp_dg_blc_offset_shift o_isp_dg_blc_offset_shift;
    o_isp_dg_blc_offset_shift.u32 = be_reg->isp_dg_blc_offset_shift.u32;
    o_isp_dg_blc_offset_shift.bits.isp_dg_blc_offset_shift = uisp_dg_blc_offset_shift;
    be_reg->isp_dg_blc_offset_shift.u32 = o_isp_dg_blc_offset_shift.u32;
}

/*  description   set the value of the member isp_cc_cfg.isp_cc_colortone_en */
/*  input         unsigned int uisp_cc_colortone_en  1 bits */
static __inline td_void isp_cc_colortone_en_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_colortone_en)
{
    u_isp_cc_cfg o_isp_cc_cfg;
    o_isp_cc_cfg.u32 = be_reg->isp_cc_cfg.u32;
    o_isp_cc_cfg.bits.isp_cc_colortone_en = uisp_cc_colortone_en;
    be_reg->isp_cc_cfg.u32 = o_isp_cc_cfg.u32;
}
/*  description   set the value of the member isp_cc_cfg.isp_cc_recover_en */
/*  input         unsigned int uisp_cc_recover_en  1 bits */
static __inline td_void isp_cc_recover_en_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_recover_en)
{
    u_isp_cc_cfg o_isp_cc_cfg;
    o_isp_cc_cfg.u32 = be_reg->isp_cc_cfg.u32;
    o_isp_cc_cfg.bits.isp_cc_recover_en = uisp_cc_recover_en;
    be_reg->isp_cc_cfg.u32 = o_isp_cc_cfg.u32;
}

/*  description   set the value of the member isp_cc_cfg.isp_cc_lutb_en */
/*  input         unsigned int uisp_cc_lutb_en  1 bits */
static __inline td_void isp_cc_lutb_en_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_lutb_en)
{
    u_isp_cc_cfg o_isp_cc_cfg;
    o_isp_cc_cfg.u32 = be_reg->isp_cc_cfg.u32;
    o_isp_cc_cfg.bits.isp_cc_lutb_en = uisp_cc_lutb_en;
    be_reg->isp_cc_cfg.u32 = o_isp_cc_cfg.u32;
}

static __inline td_void isp_cc_hue_en_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue_en)
{
    u_isp_cc_cfg o_isp_cc_cfg;
    o_isp_cc_cfg.u32 = be_reg->isp_cc_cfg.u32;
    o_isp_cc_cfg.bits.isp_cc_hue_en = uisp_cc_hue_en;
    be_reg->isp_cc_cfg.u32 = o_isp_cc_cfg.u32;
}

/*  description : set the value of the member isp_cc_coef00.isp_cc_coef000 */
/*  input       : unsigned int uisp_cc_coef000: 15 bits */
static __inline td_void isp_cc_coef000_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef000)
{
    u_isp_cc_coef00 o_isp_cc_coef00;
    o_isp_cc_coef00.u32 = be_reg->isp_cc_coef00.u32;
    o_isp_cc_coef00.bits.isp_cc_coef000 = uisp_cc_coef000;
    be_reg->isp_cc_coef00.u32 = o_isp_cc_coef00.u32;
}

/*  description : set the value of the member isp_cc_coef00.isp_cc_coef001 */
/*  input       : unsigned int uisp_cc_coef001: 15 bits */
static __inline td_void isp_cc_coef001_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef001)
{
    u_isp_cc_coef00 o_isp_cc_coef00;
    o_isp_cc_coef00.u32 = be_reg->isp_cc_coef00.u32;
    o_isp_cc_coef00.bits.isp_cc_coef001 = uisp_cc_coef001;
    be_reg->isp_cc_coef00.u32 = o_isp_cc_coef00.u32;
}

/*  description : set the value of the member isp_cc_coef01.isp_cc_coef002 */
/*  input       : unsigned int uisp_cc_coef002: 15 bits */
static __inline td_void isp_cc_coef002_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef002)
{
    u_isp_cc_coef01 o_isp_cc_coef01;
    o_isp_cc_coef01.u32 = be_reg->isp_cc_coef01.u32;
    o_isp_cc_coef01.bits.isp_cc_coef002 = uisp_cc_coef002;
    be_reg->isp_cc_coef01.u32 = o_isp_cc_coef01.u32;
}

/*  description : set the value of the member isp_cc_coef01.isp_cc_coef010 */
/*  input       : unsigned int uisp_cc_coef010: 15 bits */
static __inline td_void isp_cc_coef010_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef010)
{
    u_isp_cc_coef01 o_isp_cc_coef01;
    o_isp_cc_coef01.u32 = be_reg->isp_cc_coef01.u32;
    o_isp_cc_coef01.bits.isp_cc_coef010 = uisp_cc_coef010;
    be_reg->isp_cc_coef01.u32 = o_isp_cc_coef01.u32;
}

/*  description : set the value of the member isp_cc_coef02.isp_cc_coef011 */
/*  input       : unsigned int uisp_cc_coef011: 15 bits */
static __inline td_void isp_cc_coef011_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef011)
{
    u_isp_cc_coef02 o_isp_cc_coef02;
    o_isp_cc_coef02.u32 = be_reg->isp_cc_coef02.u32;
    o_isp_cc_coef02.bits.isp_cc_coef011 = uisp_cc_coef011;
    be_reg->isp_cc_coef02.u32 = o_isp_cc_coef02.u32;
}

/*  description : set the value of the member isp_cc_coef02.isp_cc_coef012 */
/*  input       : unsigned int uisp_cc_coef012: 15 bits */
static __inline td_void isp_cc_coef012_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef012)
{
    u_isp_cc_coef02 o_isp_cc_coef02;
    o_isp_cc_coef02.u32 = be_reg->isp_cc_coef02.u32;
    o_isp_cc_coef02.bits.isp_cc_coef012 = uisp_cc_coef012;
    be_reg->isp_cc_coef02.u32 = o_isp_cc_coef02.u32;
}

/*  description : set the value of the member isp_cc_coef03.isp_cc_coef020 */
/*  input       : unsigned int uisp_cc_coef020: 15 bits */
static __inline td_void isp_cc_coef020_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef020)
{
    u_isp_cc_coef03 o_isp_cc_coef03;
    o_isp_cc_coef03.u32 = be_reg->isp_cc_coef03.u32;
    o_isp_cc_coef03.bits.isp_cc_coef020 = uisp_cc_coef020;
    be_reg->isp_cc_coef03.u32 = o_isp_cc_coef03.u32;
}

/*  description : set the value of the member isp_cc_coef03.isp_cc_coef021 */
/*  input       : unsigned int uisp_cc_coef021: 15 bits */
static __inline td_void isp_cc_coef021_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef021)
{
    u_isp_cc_coef03 o_isp_cc_coef03;
    o_isp_cc_coef03.u32 = be_reg->isp_cc_coef03.u32;
    o_isp_cc_coef03.bits.isp_cc_coef021 = uisp_cc_coef021;
    be_reg->isp_cc_coef03.u32 = o_isp_cc_coef03.u32;
}

/*  description : set the value of the member isp_cc_coef04.isp_cc_coef022 */
/*  input       : unsigned int uisp_cc_coef022: 15 bits */
static __inline td_void isp_cc_coef022_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef022)
{
    u_isp_cc_coef04 o_isp_cc_coef04;
    o_isp_cc_coef04.u32 = be_reg->isp_cc_coef04.u32;
    o_isp_cc_coef04.bits.isp_cc_coef022 = uisp_cc_coef022;
    be_reg->isp_cc_coef04.u32 = o_isp_cc_coef04.u32;
}

/*  description : set the value of the member isp_cc_coef10.isp_cc_coef100 */
/*  input       : unsigned int uisp_cc_coef100: 15 bits */
static __inline td_void isp_cc_coef100_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef100)
{
    u_isp_cc_coef10 o_isp_cc_coef10;
    o_isp_cc_coef10.u32 = be_reg->isp_cc_coef10.u32;
    o_isp_cc_coef10.bits.isp_cc_coef100 = uisp_cc_coef100;
    be_reg->isp_cc_coef10.u32 = o_isp_cc_coef10.u32;
}

/*  description : set the value of the member isp_cc_coef10.isp_cc_coef101 */
/*  input       : unsigned int uisp_cc_coef101: 15 bits */
static __inline td_void isp_cc_coef101_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef101)
{
    u_isp_cc_coef10 o_isp_cc_coef10;
    o_isp_cc_coef10.u32 = be_reg->isp_cc_coef10.u32;
    o_isp_cc_coef10.bits.isp_cc_coef101 = uisp_cc_coef101;
    be_reg->isp_cc_coef10.u32 = o_isp_cc_coef10.u32;
}

/*  description : set the value of the member isp_cc_coef11.isp_cc_coef102 */
/*  input       : unsigned int uisp_cc_coef102: 15 bits */
static __inline td_void isp_cc_coef102_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef102)
{
    u_isp_cc_coef11 o_isp_cc_coef11;
    o_isp_cc_coef11.u32 = be_reg->isp_cc_coef11.u32;
    o_isp_cc_coef11.bits.isp_cc_coef102 = uisp_cc_coef102;
    be_reg->isp_cc_coef11.u32 = o_isp_cc_coef11.u32;
}

/*  description : set the value of the member isp_cc_coef11.isp_cc_coef110 */
/*  input       : unsigned int uisp_cc_coef110: 15 bits */
static __inline td_void isp_cc_coef110_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef110)
{
    u_isp_cc_coef11 o_isp_cc_coef11;
    o_isp_cc_coef11.u32 = be_reg->isp_cc_coef11.u32;
    o_isp_cc_coef11.bits.isp_cc_coef110 = uisp_cc_coef110;
    be_reg->isp_cc_coef11.u32 = o_isp_cc_coef11.u32;
}

/*  description : set the value of the member isp_cc_coef12.isp_cc_coef111 */
/*  input       : unsigned int uisp_cc_coef111: 15 bits */
static __inline td_void isp_cc_coef111_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef111)
{
    u_isp_cc_coef12 o_isp_cc_coef12;
    o_isp_cc_coef12.u32 = be_reg->isp_cc_coef12.u32;
    o_isp_cc_coef12.bits.isp_cc_coef111 = uisp_cc_coef111;
    be_reg->isp_cc_coef12.u32 = o_isp_cc_coef12.u32;
}

/*  description : set the value of the member isp_cc_coef12.isp_cc_coef112 */
/*  input       : unsigned int uisp_cc_coef112: 15 bits */
static __inline td_void isp_cc_coef112_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef112)
{
    u_isp_cc_coef12 o_isp_cc_coef12;
    o_isp_cc_coef12.u32 = be_reg->isp_cc_coef12.u32;
    o_isp_cc_coef12.bits.isp_cc_coef112 = uisp_cc_coef112;
    be_reg->isp_cc_coef12.u32 = o_isp_cc_coef12.u32;
}

/*  description : set the value of the member isp_cc_coef13.isp_cc_coef120 */
/*  input       : unsigned int uisp_cc_coef120: 15 bits */
static __inline td_void isp_cc_coef120_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef120)
{
    u_isp_cc_coef13 o_isp_cc_coef13;
    o_isp_cc_coef13.u32 = be_reg->isp_cc_coef13.u32;
    o_isp_cc_coef13.bits.isp_cc_coef120 = uisp_cc_coef120;
    be_reg->isp_cc_coef13.u32 = o_isp_cc_coef13.u32;
}

/*  description : set the value of the member isp_cc_coef13.isp_cc_coef121 */
/*  input       : unsigned int uisp_cc_coef121: 15 bits */
static __inline td_void isp_cc_coef121_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef121)
{
    u_isp_cc_coef13 o_isp_cc_coef13;
    o_isp_cc_coef13.u32 = be_reg->isp_cc_coef13.u32;
    o_isp_cc_coef13.bits.isp_cc_coef121 = uisp_cc_coef121;
    be_reg->isp_cc_coef13.u32 = o_isp_cc_coef13.u32;
}

/*  description : set the value of the member isp_cc_coef14.isp_cc_coef122 */
/*  input       : unsigned int uisp_cc_coef122: 15 bits */
static __inline td_void isp_cc_coef122_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef122)
{
    u_isp_cc_coef14 o_isp_cc_coef14;
    o_isp_cc_coef14.u32 = be_reg->isp_cc_coef14.u32;
    o_isp_cc_coef14.bits.isp_cc_coef122 = uisp_cc_coef122;
    be_reg->isp_cc_coef14.u32 = o_isp_cc_coef14.u32;
}

/*  description : set the value of the member isp_cc_coef20.isp_cc_coef200 */
/*  input       : unsigned int uisp_cc_coef200: 15 bits */
static __inline td_void isp_cc_coef200_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef200)
{
    u_isp_cc_coef20 o_isp_cc_coef20;
    o_isp_cc_coef20.u32 = be_reg->isp_cc_coef20.u32;
    o_isp_cc_coef20.bits.isp_cc_coef200 = uisp_cc_coef200;
    be_reg->isp_cc_coef20.u32 = o_isp_cc_coef20.u32;
}

/*  description : set the value of the member isp_cc_coef20.isp_cc_coef201 */
/*  input       : unsigned int uisp_cc_coef201: 15 bits */
static __inline td_void isp_cc_coef201_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef201)
{
    u_isp_cc_coef20 o_isp_cc_coef20;
    o_isp_cc_coef20.u32 = be_reg->isp_cc_coef20.u32;
    o_isp_cc_coef20.bits.isp_cc_coef201 = uisp_cc_coef201;
    be_reg->isp_cc_coef20.u32 = o_isp_cc_coef20.u32;
}

/*  description : set the value of the member isp_cc_coef21.isp_cc_coef202 */
/*  input       : unsigned int uisp_cc_coef202: 15 bits */
static __inline td_void isp_cc_coef202_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef202)
{
    u_isp_cc_coef21 o_isp_cc_coef21;
    o_isp_cc_coef21.u32 = be_reg->isp_cc_coef21.u32;
    o_isp_cc_coef21.bits.isp_cc_coef202 = uisp_cc_coef202;
    be_reg->isp_cc_coef21.u32 = o_isp_cc_coef21.u32;
}

/*  description : set the value of the member isp_cc_coef21.isp_cc_coef210 */
/*  input       : unsigned int uisp_cc_coef210: 15 bits */
static __inline td_void isp_cc_coef210_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef210)
{
    u_isp_cc_coef21 o_isp_cc_coef21;
    o_isp_cc_coef21.u32 = be_reg->isp_cc_coef21.u32;
    o_isp_cc_coef21.bits.isp_cc_coef210 = uisp_cc_coef210;
    be_reg->isp_cc_coef21.u32 = o_isp_cc_coef21.u32;
}

/*  description : set the value of the member isp_cc_coef22.isp_cc_coef211 */
/*  input       : unsigned int uisp_cc_coef211: 15 bits */
static __inline td_void isp_cc_coef211_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef211)
{
    u_isp_cc_coef22 o_isp_cc_coef22;
    o_isp_cc_coef22.u32 = be_reg->isp_cc_coef22.u32;
    o_isp_cc_coef22.bits.isp_cc_coef211 = uisp_cc_coef211;
    be_reg->isp_cc_coef22.u32 = o_isp_cc_coef22.u32;
}

/*  description : set the value of the member isp_cc_coef22.isp_cc_coef212 */
/*  input       : unsigned int uisp_cc_coef212: 15 bits */
static __inline td_void isp_cc_coef212_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef212)
{
    u_isp_cc_coef22 o_isp_cc_coef22;
    o_isp_cc_coef22.u32 = be_reg->isp_cc_coef22.u32;
    o_isp_cc_coef22.bits.isp_cc_coef212 = uisp_cc_coef212;
    be_reg->isp_cc_coef22.u32 = o_isp_cc_coef22.u32;
}

/*  description : set the value of the member isp_cc_coef23.isp_cc_coef220 */
/*  input       : unsigned int uisp_cc_coef220: 15 bits */
static __inline td_void isp_cc_coef220_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef220)
{
    u_isp_cc_coef23 o_isp_cc_coef23;
    o_isp_cc_coef23.u32 = be_reg->isp_cc_coef23.u32;
    o_isp_cc_coef23.bits.isp_cc_coef220 = uisp_cc_coef220;
    be_reg->isp_cc_coef23.u32 = o_isp_cc_coef23.u32;
}

/*  description : set the value of the member isp_cc_coef23.isp_cc_coef221 */
/*  input       : unsigned int uisp_cc_coef221: 15 bits */
static __inline td_void isp_cc_coef221_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef221)
{
    u_isp_cc_coef23 o_isp_cc_coef23;
    o_isp_cc_coef23.u32 = be_reg->isp_cc_coef23.u32;
    o_isp_cc_coef23.bits.isp_cc_coef221 = uisp_cc_coef221;
    be_reg->isp_cc_coef23.u32 = o_isp_cc_coef23.u32;
}

/*  description : set the value of the member isp_cc_coef24.isp_cc_coef222 */
/*  input       : unsigned int uisp_cc_coef222: 15 bits */
static __inline td_void isp_cc_coef222_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef222)
{
    u_isp_cc_coef24 o_isp_cc_coef24;
    o_isp_cc_coef24.u32 = be_reg->isp_cc_coef24.u32;
    o_isp_cc_coef24.bits.isp_cc_coef222 = uisp_cc_coef222;
    be_reg->isp_cc_coef24.u32 = o_isp_cc_coef24.u32;
}

/*  description : set the value of the member isp_cc_coef30.isp_cc_coef300 */
/*  input       : unsigned int uisp_cc_coef300: 15 bits */
static __inline td_void isp_cc_coef300_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef300)
{
    u_isp_cc_coef30 o_isp_cc_coef30;
    o_isp_cc_coef30.u32 = be_reg->isp_cc_coef30.u32;
    o_isp_cc_coef30.bits.isp_cc_coef300 = uisp_cc_coef300;
    be_reg->isp_cc_coef30.u32 = o_isp_cc_coef30.u32;
}

/*  description : set the value of the member isp_cc_coef30.isp_cc_coef301 */
/*  input       : unsigned int uisp_cc_coef301: 15 bits */
static __inline td_void isp_cc_coef301_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef301)
{
    u_isp_cc_coef30 o_isp_cc_coef30;
    o_isp_cc_coef30.u32 = be_reg->isp_cc_coef30.u32;
    o_isp_cc_coef30.bits.isp_cc_coef301 = uisp_cc_coef301;
    be_reg->isp_cc_coef30.u32 = o_isp_cc_coef30.u32;
}

/*  description : set the value of the member isp_cc_coef31.isp_cc_coef302 */
/*  input       : unsigned int uisp_cc_coef302: 15 bits */
static __inline td_void isp_cc_coef302_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef302)
{
    u_isp_cc_coef31 o_isp_cc_coef31;
    o_isp_cc_coef31.u32 = be_reg->isp_cc_coef31.u32;
    o_isp_cc_coef31.bits.isp_cc_coef302 = uisp_cc_coef302;
    be_reg->isp_cc_coef31.u32 = o_isp_cc_coef31.u32;
}

/*  description : set the value of the member isp_cc_coef31.isp_cc_coef310 */
/*  input       : unsigned int uisp_cc_coef310: 15 bits */
static __inline td_void isp_cc_coef310_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef310)
{
    u_isp_cc_coef31 o_isp_cc_coef31;
    o_isp_cc_coef31.u32 = be_reg->isp_cc_coef31.u32;
    o_isp_cc_coef31.bits.isp_cc_coef310 = uisp_cc_coef310;
    be_reg->isp_cc_coef31.u32 = o_isp_cc_coef31.u32;
}

/*  description : set the value of the member isp_cc_coef32.isp_cc_coef311 */
/*  input       : unsigned int uisp_cc_coef311: 15 bits */
static __inline td_void isp_cc_coef311_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef311)
{
    u_isp_cc_coef32 o_isp_cc_coef32;
    o_isp_cc_coef32.u32 = be_reg->isp_cc_coef32.u32;
    o_isp_cc_coef32.bits.isp_cc_coef311 = uisp_cc_coef311;
    be_reg->isp_cc_coef32.u32 = o_isp_cc_coef32.u32;
}

/*  description : set the value of the member isp_cc_coef32.isp_cc_coef312 */
/*  input       : unsigned int uisp_cc_coef312: 15 bits */
static __inline td_void isp_cc_coef312_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef312)
{
    u_isp_cc_coef32 o_isp_cc_coef32;
    o_isp_cc_coef32.u32 = be_reg->isp_cc_coef32.u32;
    o_isp_cc_coef32.bits.isp_cc_coef312 = uisp_cc_coef312;
    be_reg->isp_cc_coef32.u32 = o_isp_cc_coef32.u32;
}

/*  description : set the value of the member isp_cc_coef33.isp_cc_coef320 */
/*  input       : unsigned int uisp_cc_coef320: 15 bits */
static __inline td_void isp_cc_coef320_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef320)
{
    u_isp_cc_coef33 o_isp_cc_coef33;
    o_isp_cc_coef33.u32 = be_reg->isp_cc_coef33.u32;
    o_isp_cc_coef33.bits.isp_cc_coef320 = uisp_cc_coef320;
    be_reg->isp_cc_coef33.u32 = o_isp_cc_coef33.u32;
}

/*  description : set the value of the member isp_cc_coef33.isp_cc_coef321 */
/*  input       : unsigned int uisp_cc_coef321: 15 bits */
static __inline td_void isp_cc_coef321_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef321)
{
    u_isp_cc_coef33 o_isp_cc_coef33;
    o_isp_cc_coef33.u32 = be_reg->isp_cc_coef33.u32;
    o_isp_cc_coef33.bits.isp_cc_coef321 = uisp_cc_coef321;
    be_reg->isp_cc_coef33.u32 = o_isp_cc_coef33.u32;
}

/*  description : set the value of the member isp_cc_coef34.isp_cc_coef322 */
/*  input       : unsigned int uisp_cc_coef322: 15 bits */
static __inline td_void isp_cc_coef322_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef322)
{
    u_isp_cc_coef34 o_isp_cc_coef34;
    o_isp_cc_coef34.u32 = be_reg->isp_cc_coef34.u32;
    o_isp_cc_coef34.bits.isp_cc_coef322 = uisp_cc_coef322;
    be_reg->isp_cc_coef34.u32 = o_isp_cc_coef34.u32;
}

/*  description : set the value of the member isp_cc_coef40.isp_cc_coef400 */
/*  input       : unsigned int uisp_cc_coef400: 15 bits */
static __inline td_void isp_cc_coef400_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef400)
{
    u_isp_cc_coef40 o_isp_cc_coef40;
    o_isp_cc_coef40.u32 = be_reg->isp_cc_coef40.u32;
    o_isp_cc_coef40.bits.isp_cc_coef400 = uisp_cc_coef400;
    be_reg->isp_cc_coef40.u32 = o_isp_cc_coef40.u32;
}

/*  description : set the value of the member isp_cc_coef40.isp_cc_coef401 */
/*  input       : unsigned int uisp_cc_coef401: 15 bits */
static __inline td_void isp_cc_coef401_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef401)
{
    u_isp_cc_coef40 o_isp_cc_coef40;
    o_isp_cc_coef40.u32 = be_reg->isp_cc_coef40.u32;
    o_isp_cc_coef40.bits.isp_cc_coef401 = uisp_cc_coef401;
    be_reg->isp_cc_coef40.u32 = o_isp_cc_coef40.u32;
}

/*  description : set the value of the member isp_cc_coef41.isp_cc_coef402 */
/*  input       : unsigned int uisp_cc_coef402: 15 bits */
static __inline td_void isp_cc_coef402_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef402)
{
    u_isp_cc_coef41 o_isp_cc_coef41;
    o_isp_cc_coef41.u32 = be_reg->isp_cc_coef41.u32;
    o_isp_cc_coef41.bits.isp_cc_coef402 = uisp_cc_coef402;
    be_reg->isp_cc_coef41.u32 = o_isp_cc_coef41.u32;
}

/*  description : set the value of the member isp_cc_coef41.isp_cc_coef410 */
/*  input       : unsigned int uisp_cc_coef410: 15 bits */
static __inline td_void isp_cc_coef410_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef410)
{
    u_isp_cc_coef41 o_isp_cc_coef41;
    o_isp_cc_coef41.u32 = be_reg->isp_cc_coef41.u32;
    o_isp_cc_coef41.bits.isp_cc_coef410 = uisp_cc_coef410;
    be_reg->isp_cc_coef41.u32 = o_isp_cc_coef41.u32;
}

/*  description : set the value of the member isp_cc_coef42.isp_cc_coef411 */
/*  input       : unsigned int uisp_cc_coef411: 15 bits */
static __inline td_void isp_cc_coef411_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef411)
{
    u_isp_cc_coef42 o_isp_cc_coef42;
    o_isp_cc_coef42.u32 = be_reg->isp_cc_coef42.u32;
    o_isp_cc_coef42.bits.isp_cc_coef411 = uisp_cc_coef411;
    be_reg->isp_cc_coef42.u32 = o_isp_cc_coef42.u32;
}

/*  description : set the value of the member isp_cc_coef42.isp_cc_coef412 */
/*  input       : unsigned int uisp_cc_coef412: 15 bits */
static __inline td_void isp_cc_coef412_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef412)
{
    u_isp_cc_coef42 o_isp_cc_coef42;
    o_isp_cc_coef42.u32 = be_reg->isp_cc_coef42.u32;
    o_isp_cc_coef42.bits.isp_cc_coef412 = uisp_cc_coef412;
    be_reg->isp_cc_coef42.u32 = o_isp_cc_coef42.u32;
}

/*  description : set the value of the member isp_cc_coef43.isp_cc_coef420 */
/*  input       : unsigned int uisp_cc_coef420: 15 bits */
static __inline td_void isp_cc_coef420_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef420)
{
    u_isp_cc_coef43 o_isp_cc_coef43;
    o_isp_cc_coef43.u32 = be_reg->isp_cc_coef43.u32;
    o_isp_cc_coef43.bits.isp_cc_coef420 = uisp_cc_coef420;
    be_reg->isp_cc_coef43.u32 = o_isp_cc_coef43.u32;
}

/*  description : set the value of the member isp_cc_coef43.isp_cc_coef421 */
/*  input       : unsigned int uisp_cc_coef421: 15 bits */
static __inline td_void isp_cc_coef421_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef421)
{
    u_isp_cc_coef43 o_isp_cc_coef43;
    o_isp_cc_coef43.u32 = be_reg->isp_cc_coef43.u32;
    o_isp_cc_coef43.bits.isp_cc_coef421 = uisp_cc_coef421;
    be_reg->isp_cc_coef43.u32 = o_isp_cc_coef43.u32;
}

/*  description : set the value of the member isp_cc_coef44.isp_cc_coef422 */
/*  input       : unsigned int uisp_cc_coef422: 15 bits */
static __inline td_void isp_cc_coef422_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef422)
{
    u_isp_cc_coef44 o_isp_cc_coef44;
    o_isp_cc_coef44.u32 = be_reg->isp_cc_coef44.u32;
    o_isp_cc_coef44.bits.isp_cc_coef422 = uisp_cc_coef422;
    be_reg->isp_cc_coef44.u32 = o_isp_cc_coef44.u32;
}

/*  description : set the value of the member isp_cc_coef50.isp_cc_coef500 */
/*  input       : unsigned int uisp_cc_coef500: 15 bits */
static __inline td_void isp_cc_coef500_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef500)
{
    u_isp_cc_coef50 o_isp_cc_coef50;
    o_isp_cc_coef50.u32 = be_reg->isp_cc_coef50.u32;
    o_isp_cc_coef50.bits.isp_cc_coef500 = uisp_cc_coef500;
    be_reg->isp_cc_coef50.u32 = o_isp_cc_coef50.u32;
}

/*  description : set the value of the member isp_cc_coef50.isp_cc_coef501 */
/*  input       : unsigned int uisp_cc_coef501: 15 bits */
static __inline td_void isp_cc_coef501_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef501)
{
    u_isp_cc_coef50 o_isp_cc_coef50;
    o_isp_cc_coef50.u32 = be_reg->isp_cc_coef50.u32;
    o_isp_cc_coef50.bits.isp_cc_coef501 = uisp_cc_coef501;
    be_reg->isp_cc_coef50.u32 = o_isp_cc_coef50.u32;
}

/*  description : set the value of the member isp_cc_coef51.isp_cc_coef502 */
/*  input       : unsigned int uisp_cc_coef502: 15 bits */
static __inline td_void isp_cc_coef502_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef502)
{
    u_isp_cc_coef51 o_isp_cc_coef51;
    o_isp_cc_coef51.u32 = be_reg->isp_cc_coef51.u32;
    o_isp_cc_coef51.bits.isp_cc_coef502 = uisp_cc_coef502;
    be_reg->isp_cc_coef51.u32 = o_isp_cc_coef51.u32;
}

/*  description : set the value of the member isp_cc_coef51.isp_cc_coef510 */
/*  input       : unsigned int uisp_cc_coef510: 15 bits */
static __inline td_void isp_cc_coef510_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef510)
{
    u_isp_cc_coef51 o_isp_cc_coef51;
    o_isp_cc_coef51.u32 = be_reg->isp_cc_coef51.u32;
    o_isp_cc_coef51.bits.isp_cc_coef510 = uisp_cc_coef510;
    be_reg->isp_cc_coef51.u32 = o_isp_cc_coef51.u32;
}

/*  description : set the value of the member isp_cc_coef52.isp_cc_coef511 */
/*  input       : unsigned int uisp_cc_coef511: 15 bits */
static __inline td_void isp_cc_coef511_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef511)
{
    u_isp_cc_coef52 o_isp_cc_coef52;
    o_isp_cc_coef52.u32 = be_reg->isp_cc_coef52.u32;
    o_isp_cc_coef52.bits.isp_cc_coef511 = uisp_cc_coef511;
    be_reg->isp_cc_coef52.u32 = o_isp_cc_coef52.u32;
}

/*  description : set the value of the member isp_cc_coef52.isp_cc_coef512 */
/*  input       : unsigned int uisp_cc_coef512: 15 bits */
static __inline td_void isp_cc_coef512_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef512)
{
    u_isp_cc_coef52 o_isp_cc_coef52;
    o_isp_cc_coef52.u32 = be_reg->isp_cc_coef52.u32;
    o_isp_cc_coef52.bits.isp_cc_coef512 = uisp_cc_coef512;
    be_reg->isp_cc_coef52.u32 = o_isp_cc_coef52.u32;
}

/*  description : set the value of the member isp_cc_coef53.isp_cc_coef520 */
/*  input       : unsigned int uisp_cc_coef520: 15 bits */
static __inline td_void isp_cc_coef520_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef520)
{
    u_isp_cc_coef53 o_isp_cc_coef53;
    o_isp_cc_coef53.u32 = be_reg->isp_cc_coef53.u32;
    o_isp_cc_coef53.bits.isp_cc_coef520 = uisp_cc_coef520;
    be_reg->isp_cc_coef53.u32 = o_isp_cc_coef53.u32;
}

/*  description : set the value of the member isp_cc_coef53.isp_cc_coef521 */
/*  input       : unsigned int uisp_cc_coef521: 15 bits */
static __inline td_void isp_cc_coef521_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef521)
{
    u_isp_cc_coef53 o_isp_cc_coef53;
    o_isp_cc_coef53.u32 = be_reg->isp_cc_coef53.u32;
    o_isp_cc_coef53.bits.isp_cc_coef521 = uisp_cc_coef521;
    be_reg->isp_cc_coef53.u32 = o_isp_cc_coef53.u32;
}

/*  description : set the value of the member isp_cc_coef54.isp_cc_coef522 */
/*  input       : unsigned int uisp_cc_coef522: 15 bits */
static __inline td_void isp_cc_coef522_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef522)
{
    u_isp_cc_coef54 o_isp_cc_coef54;
    o_isp_cc_coef54.u32 = be_reg->isp_cc_coef54.u32;
    o_isp_cc_coef54.bits.isp_cc_coef522 = uisp_cc_coef522;
    be_reg->isp_cc_coef54.u32 = o_isp_cc_coef54.u32;
}

/*  description : set the value of the member isp_cc_coef60.isp_cc_coef600 */
/*  input       : unsigned int uisp_cc_coef600: 15 bits */
static __inline td_void isp_cc_coef600_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef600)
{
    u_isp_cc_coef60 o_isp_cc_coef60;
    o_isp_cc_coef60.u32 = be_reg->isp_cc_coef60.u32;
    o_isp_cc_coef60.bits.isp_cc_coef600 = uisp_cc_coef600;
    be_reg->isp_cc_coef60.u32 = o_isp_cc_coef60.u32;
}

/*  description : set the value of the member isp_cc_coef60.isp_cc_coef601 */
/*  input       : unsigned int uisp_cc_coef601: 15 bits */
static __inline td_void isp_cc_coef601_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef601)
{
    u_isp_cc_coef60 o_isp_cc_coef60;
    o_isp_cc_coef60.u32 = be_reg->isp_cc_coef60.u32;
    o_isp_cc_coef60.bits.isp_cc_coef601 = uisp_cc_coef601;
    be_reg->isp_cc_coef60.u32 = o_isp_cc_coef60.u32;
}

/*  description : set the value of the member isp_cc_coef61.isp_cc_coef602 */
/*  input       : unsigned int uisp_cc_coef602: 15 bits */
static __inline td_void isp_cc_coef602_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef602)
{
    u_isp_cc_coef61 o_isp_cc_coef61;
    o_isp_cc_coef61.u32 = be_reg->isp_cc_coef61.u32;
    o_isp_cc_coef61.bits.isp_cc_coef602 = uisp_cc_coef602;
    be_reg->isp_cc_coef61.u32 = o_isp_cc_coef61.u32;
}

/*  description : set the value of the member isp_cc_coef61.isp_cc_coef610 */
/*  input       : unsigned int uisp_cc_coef610: 15 bits */
static __inline td_void isp_cc_coef610_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef610)
{
    u_isp_cc_coef61 o_isp_cc_coef61;
    o_isp_cc_coef61.u32 = be_reg->isp_cc_coef61.u32;
    o_isp_cc_coef61.bits.isp_cc_coef610 = uisp_cc_coef610;
    be_reg->isp_cc_coef61.u32 = o_isp_cc_coef61.u32;
}

/*  description : set the value of the member isp_cc_coef62.isp_cc_coef611 */
/*  input       : unsigned int uisp_cc_coef611: 15 bits */
static __inline td_void isp_cc_coef611_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef611)
{
    u_isp_cc_coef62 o_isp_cc_coef62;
    o_isp_cc_coef62.u32 = be_reg->isp_cc_coef62.u32;
    o_isp_cc_coef62.bits.isp_cc_coef611 = uisp_cc_coef611;
    be_reg->isp_cc_coef62.u32 = o_isp_cc_coef62.u32;
}

/*  description : set the value of the member isp_cc_coef62.isp_cc_coef612 */
/*  input       : unsigned int uisp_cc_coef612: 15 bits */
static __inline td_void isp_cc_coef612_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef612)
{
    u_isp_cc_coef62 o_isp_cc_coef62;
    o_isp_cc_coef62.u32 = be_reg->isp_cc_coef62.u32;
    o_isp_cc_coef62.bits.isp_cc_coef612 = uisp_cc_coef612;
    be_reg->isp_cc_coef62.u32 = o_isp_cc_coef62.u32;
}

/*  description : set the value of the member isp_cc_coef63.isp_cc_coef620 */
/*  input       : unsigned int uisp_cc_coef620: 15 bits */
static __inline td_void isp_cc_coef620_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef620)
{
    u_isp_cc_coef63 o_isp_cc_coef63;
    o_isp_cc_coef63.u32 = be_reg->isp_cc_coef63.u32;
    o_isp_cc_coef63.bits.isp_cc_coef620 = uisp_cc_coef620;
    be_reg->isp_cc_coef63.u32 = o_isp_cc_coef63.u32;
}

/*  description : set the value of the member isp_cc_coef63.isp_cc_coef621 */
/*  input       : unsigned int uisp_cc_coef621: 15 bits */
static __inline td_void isp_cc_coef621_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef621)
{
    u_isp_cc_coef63 o_isp_cc_coef63;
    o_isp_cc_coef63.u32 = be_reg->isp_cc_coef63.u32;
    o_isp_cc_coef63.bits.isp_cc_coef621 = uisp_cc_coef621;
    be_reg->isp_cc_coef63.u32 = o_isp_cc_coef63.u32;
}

/*  description : set the value of the member isp_cc_coef64.isp_cc_coef622 */
/*  input       : unsigned int uisp_cc_coef622: 15 bits */
static __inline td_void isp_cc_coef622_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef622)
{
    u_isp_cc_coef64 o_isp_cc_coef64;
    o_isp_cc_coef64.u32 = be_reg->isp_cc_coef64.u32;
    o_isp_cc_coef64.bits.isp_cc_coef622 = uisp_cc_coef622;
    be_reg->isp_cc_coef64.u32 = o_isp_cc_coef64.u32;
}

/*  description : set the value of the member isp_cc_coef70.isp_cc_coef700 */
/*  input       : unsigned int uisp_cc_coef700: 15 bits */
static __inline td_void isp_cc_coef700_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef700)
{
    u_isp_cc_coef70 o_isp_cc_coef70;
    o_isp_cc_coef70.u32 = be_reg->isp_cc_coef70.u32;
    o_isp_cc_coef70.bits.isp_cc_coef700 = uisp_cc_coef700;
    be_reg->isp_cc_coef70.u32 = o_isp_cc_coef70.u32;
}

/*  description : set the value of the member isp_cc_coef70.isp_cc_coef701 */
/*  input       : unsigned int uisp_cc_coef701: 15 bits */
static __inline td_void isp_cc_coef701_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef701)
{
    u_isp_cc_coef70 o_isp_cc_coef70;
    o_isp_cc_coef70.u32 = be_reg->isp_cc_coef70.u32;
    o_isp_cc_coef70.bits.isp_cc_coef701 = uisp_cc_coef701;
    be_reg->isp_cc_coef70.u32 = o_isp_cc_coef70.u32;
}

/*  description : set the value of the member isp_cc_coef71.isp_cc_coef702 */
/*  input       : unsigned int uisp_cc_coef702: 15 bits */
static __inline td_void isp_cc_coef702_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef702)
{
    u_isp_cc_coef71 o_isp_cc_coef71;
    o_isp_cc_coef71.u32 = be_reg->isp_cc_coef71.u32;
    o_isp_cc_coef71.bits.isp_cc_coef702 = uisp_cc_coef702;
    be_reg->isp_cc_coef71.u32 = o_isp_cc_coef71.u32;
}

/*  description : set the value of the member isp_cc_coef71.isp_cc_coef710 */
/*  input       : unsigned int uisp_cc_coef710: 15 bits */
static __inline td_void isp_cc_coef710_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef710)
{
    u_isp_cc_coef71 o_isp_cc_coef71;
    o_isp_cc_coef71.u32 = be_reg->isp_cc_coef71.u32;
    o_isp_cc_coef71.bits.isp_cc_coef710 = uisp_cc_coef710;
    be_reg->isp_cc_coef71.u32 = o_isp_cc_coef71.u32;
}

/*  description : set the value of the member isp_cc_coef72.isp_cc_coef711 */
/*  input       : unsigned int uisp_cc_coef711: 15 bits */
static __inline td_void isp_cc_coef711_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef711)
{
    u_isp_cc_coef72 o_isp_cc_coef72;
    o_isp_cc_coef72.u32 = be_reg->isp_cc_coef72.u32;
    o_isp_cc_coef72.bits.isp_cc_coef711 = uisp_cc_coef711;
    be_reg->isp_cc_coef72.u32 = o_isp_cc_coef72.u32;
}

/*  description : set the value of the member isp_cc_coef72.isp_cc_coef712 */
/*  input       : unsigned int uisp_cc_coef712: 15 bits */
static __inline td_void isp_cc_coef712_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef712)
{
    u_isp_cc_coef72 o_isp_cc_coef72;
    o_isp_cc_coef72.u32 = be_reg->isp_cc_coef72.u32;
    o_isp_cc_coef72.bits.isp_cc_coef712 = uisp_cc_coef712;
    be_reg->isp_cc_coef72.u32 = o_isp_cc_coef72.u32;
}

/*  description : set the value of the member isp_cc_coef73.isp_cc_coef720 */
/*  input       : unsigned int uisp_cc_coef720: 15 bits */
static __inline td_void isp_cc_coef720_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef720)
{
    u_isp_cc_coef73 o_isp_cc_coef73;
    o_isp_cc_coef73.u32 = be_reg->isp_cc_coef73.u32;
    o_isp_cc_coef73.bits.isp_cc_coef720 = uisp_cc_coef720;
    be_reg->isp_cc_coef73.u32 = o_isp_cc_coef73.u32;
}

/*  description : set the value of the member isp_cc_coef73.isp_cc_coef721 */
/*  input       : unsigned int uisp_cc_coef721: 15 bits */
static __inline td_void isp_cc_coef721_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef721)
{
    u_isp_cc_coef73 o_isp_cc_coef73;
    o_isp_cc_coef73.u32 = be_reg->isp_cc_coef73.u32;
    o_isp_cc_coef73.bits.isp_cc_coef721 = uisp_cc_coef721;
    be_reg->isp_cc_coef73.u32 = o_isp_cc_coef73.u32;
}

/*  description : set the value of the member isp_cc_coef74.isp_cc_coef722 */
/*  input       : unsigned int uisp_cc_coef722: 15 bits */
static __inline td_void isp_cc_coef722_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_coef722)
{
    u_isp_cc_coef74 o_isp_cc_coef74;
    o_isp_cc_coef74.u32 = be_reg->isp_cc_coef74.u32;
    o_isp_cc_coef74.bits.isp_cc_coef722 = uisp_cc_coef722;
    be_reg->isp_cc_coef74.u32 = o_isp_cc_coef74.u32;
}

/*  description : set the value of the member isp_cc_hue_coef0.isp_cc_hue0 */
/*  input       : unsigned int uisp_cc_hue0: 11 bits */
static __inline td_void isp_cc_hue0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue0)
{
    u_isp_cc_hue_coef0 o_isp_cc_hue_coef0;
    o_isp_cc_hue_coef0.u32 = be_reg->isp_cc_hue_coef0.u32;
    o_isp_cc_hue_coef0.bits.isp_cc_hue0 = uisp_cc_hue0;
    be_reg->isp_cc_hue_coef0.u32 = o_isp_cc_hue_coef0.u32;
}

/*  description : set the value of the member isp_cc_hue_coef0.isp_cc_hue1 */
/*  input       : unsigned int uisp_cc_hue1: 11 bits */
static __inline td_void isp_cc_hue1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue1)
{
    u_isp_cc_hue_coef0 o_isp_cc_hue_coef0;
    o_isp_cc_hue_coef0.u32 = be_reg->isp_cc_hue_coef0.u32;
    o_isp_cc_hue_coef0.bits.isp_cc_hue1 = uisp_cc_hue1;
    be_reg->isp_cc_hue_coef0.u32 = o_isp_cc_hue_coef0.u32;
}

/*  description : set the value of the member isp_cc_hue_coef1.isp_cc_hue2 */
/*  input       : unsigned int uisp_cc_hue2: 11 bits */
static __inline td_void isp_cc_hue2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue2)
{
    u_isp_cc_hue_coef1 o_isp_cc_hue_coef1;
    o_isp_cc_hue_coef1.u32 = be_reg->isp_cc_hue_coef1.u32;
    o_isp_cc_hue_coef1.bits.isp_cc_hue2 = uisp_cc_hue2;
    be_reg->isp_cc_hue_coef1.u32 = o_isp_cc_hue_coef1.u32;
}

/*  description : set the value of the member isp_cc_hue_coef1.isp_cc_hue3 */
/*  input       : unsigned int uisp_cc_hue3: 11 bits */
static __inline td_void isp_cc_hue3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue3)
{
    u_isp_cc_hue_coef1 o_isp_cc_hue_coef1;
    o_isp_cc_hue_coef1.u32 = be_reg->isp_cc_hue_coef1.u32;
    o_isp_cc_hue_coef1.bits.isp_cc_hue3 = uisp_cc_hue3;
    be_reg->isp_cc_hue_coef1.u32 = o_isp_cc_hue_coef1.u32;
}

/*  description : set the value of the member isp_cc_hue_coef2.isp_cc_hue4 */
/*  input       : unsigned int uisp_cc_hue4: 11 bits */
static __inline td_void isp_cc_hue4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue4)
{
    u_isp_cc_hue_coef2 o_isp_cc_hue_coef2;
    o_isp_cc_hue_coef2.u32 = be_reg->isp_cc_hue_coef2.u32;
    o_isp_cc_hue_coef2.bits.isp_cc_hue4 = uisp_cc_hue4;
    be_reg->isp_cc_hue_coef2.u32 = o_isp_cc_hue_coef2.u32;
}

/*  description : set the value of the member isp_cc_hue_coef2.isp_cc_hue5 */
/*  input       : unsigned int uisp_cc_hue5: 11 bits */
static __inline td_void isp_cc_hue5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue5)
{
    u_isp_cc_hue_coef2 o_isp_cc_hue_coef2;
    o_isp_cc_hue_coef2.u32 = be_reg->isp_cc_hue_coef2.u32;
    o_isp_cc_hue_coef2.bits.isp_cc_hue5 = uisp_cc_hue5;
    be_reg->isp_cc_hue_coef2.u32 = o_isp_cc_hue_coef2.u32;
}

/*  description : set the value of the member isp_cc_hue_coef3.isp_cc_hue6 */
/*  input       : unsigned int uisp_cc_hue6: 11 bits */
static __inline td_void isp_cc_hue6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue6)
{
    u_isp_cc_hue_coef3 o_isp_cc_hue_coef3;
    o_isp_cc_hue_coef3.u32 = be_reg->isp_cc_hue_coef3.u32;
    o_isp_cc_hue_coef3.bits.isp_cc_hue6 = uisp_cc_hue6;
    be_reg->isp_cc_hue_coef3.u32 = o_isp_cc_hue_coef3.u32;
}

/*  description : set the value of the member isp_cc_hue_coef3.isp_cc_hue7 */
/*  input       : unsigned int uisp_cc_hue7: 11 bits */
static __inline td_void isp_cc_hue7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_hue7)
{
    u_isp_cc_hue_coef3 o_isp_cc_hue_coef3;
    o_isp_cc_hue_coef3.u32 = be_reg->isp_cc_hue_coef3.u32;
    o_isp_cc_hue_coef3.bits.isp_cc_hue7 = uisp_cc_hue7;
    be_reg->isp_cc_hue_coef3.u32 = o_isp_cc_hue_coef3.u32;
}

/*  description : set the value of the member isp_cc_hue_cfg.isp_cc_matrix_gray_en */
/*  input       : unsigned int uisp_cc_matrix_gray_en: 1 bits */
static __inline td_void isp_cc_matrix_gray_en_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_matrix_gray_en)
{
    u_isp_cc_hue_cfg o_isp_cc_hue_cfg;
    o_isp_cc_hue_cfg.u32 = be_reg->isp_cc_hue_cfg.u32;
    o_isp_cc_hue_cfg.bits.isp_cc_matrix_gray_en = uisp_cc_matrix_gray_en;
    be_reg->isp_cc_hue_cfg.u32 = o_isp_cc_hue_cfg.u32;
}

/*  description   set the value of the member isp_cc_rcv_ctrl0.isp_cc_soft_clip0_step */
/*  input         unsigned int uisp_cc_soft_clip0_step  6 bits */
static __inline td_void isp_cc_soft_clip0_step_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_soft_clip0_step)
{
    u_isp_cc_rcv_ctrl0 o_isp_cc_rcv_ctrl0;
    o_isp_cc_rcv_ctrl0.u32 = be_reg->isp_cc_rcv_ctrl0.u32;
    o_isp_cc_rcv_ctrl0.bits.isp_cc_soft_clip0_step = uisp_cc_soft_clip0_step;
    be_reg->isp_cc_rcv_ctrl0.u32 = o_isp_cc_rcv_ctrl0.u32;
}
/*  description   set the value of the member isp_cc_rcv_ctrl0.isp_cc_soft_clip1_step */
/*  input         unsigned int uisp_cc_soft_clip1_step  6 bits */
static __inline td_void isp_cc_soft_clip1_step_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_soft_clip1_step)
{
    u_isp_cc_rcv_ctrl0 o_isp_cc_rcv_ctrl0;
    o_isp_cc_rcv_ctrl0.u32 = be_reg->isp_cc_rcv_ctrl0.u32;
    o_isp_cc_rcv_ctrl0.bits.isp_cc_soft_clip1_step = uisp_cc_soft_clip1_step;
    be_reg->isp_cc_rcv_ctrl0.u32 = o_isp_cc_rcv_ctrl0.u32;
}
/*  description   set the value of the member isp_cc_rcv_ctrl1.isp_cc_darkprev */
/*  input         unsigned int uisp_cc_darkprev  5 bits */
static __inline td_void isp_cc_darkprev_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_darkprev)
{
    u_isp_cc_rcv_ctrl1 o_isp_cc_rcv_ctrl1;
    o_isp_cc_rcv_ctrl1.u32 = be_reg->isp_cc_rcv_ctrl1.u32;
    o_isp_cc_rcv_ctrl1.bits.isp_cc_darkprev = uisp_cc_darkprev;
    be_reg->isp_cc_rcv_ctrl1.u32 = o_isp_cc_rcv_ctrl1.u32;
}
/*  description   set the value of the member isp_cc_rcv_ctrl1.isp_cc_peaksupp_sat */
/*  input         unsigned int uisp_cc_peaksupp_sat  8 bits */
static __inline td_void isp_cc_peaksupp_sat_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_peaksupp_sat)
{
    u_isp_cc_rcv_ctrl1 o_isp_cc_rcv_ctrl1;
    o_isp_cc_rcv_ctrl1.u32 = be_reg->isp_cc_rcv_ctrl1.u32;
    o_isp_cc_rcv_ctrl1.bits.isp_cc_peaksupp_sat = uisp_cc_peaksupp_sat;
    be_reg->isp_cc_rcv_ctrl1.u32 = o_isp_cc_rcv_ctrl1.u32;
}
/*  description   set the value of the member isp_cc_rcv_ctrl1.isp_cc_peaksupp_max */
/*  input         unsigned int uisp_cc_peaksupp_max  8 bits */
static __inline td_void isp_cc_peaksupp_max_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_peaksupp_max)
{
    u_isp_cc_rcv_ctrl1 o_isp_cc_rcv_ctrl1;
    o_isp_cc_rcv_ctrl1.u32 = be_reg->isp_cc_rcv_ctrl1.u32;
    o_isp_cc_rcv_ctrl1.bits.isp_cc_peaksupp_max = uisp_cc_peaksupp_max;
    be_reg->isp_cc_rcv_ctrl1.u32 = o_isp_cc_rcv_ctrl1.u32;
}
/*  description   set the value of the member isp_cc_lumafact.isp_cc_luma_coefr */
/*  input         unsigned int uisp_cc_luma_coefr  5 bits */
static __inline td_void isp_cc_luma_coefr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_luma_coefr)
{
    u_isp_cc_lumafact o_isp_cc_lumafact;
    o_isp_cc_lumafact.u32 = be_reg->isp_cc_lumafact.u32;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefr = uisp_cc_luma_coefr;
    be_reg->isp_cc_lumafact.u32 = o_isp_cc_lumafact.u32;
}
/*  description   set the value of the member isp_cc_lumafact.isp_cc_luma_coefb */
/*  input         unsigned int uisp_cc_luma_coefb  5 bits */
static __inline td_void isp_cc_luma_coefb_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_luma_coefb)
{
    u_isp_cc_lumafact o_isp_cc_lumafact;
    o_isp_cc_lumafact.u32 = be_reg->isp_cc_lumafact.u32;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefb = uisp_cc_luma_coefb;
    be_reg->isp_cc_lumafact.u32 = o_isp_cc_lumafact.u32;
}
/*  description   set the value of the member isp_cc_lumafact.isp_cc_luma_coefr_up */
/*  input         unsigned int uisp_cc_luma_coefr_up  5 bits */
static __inline td_void isp_cc_luma_coefr_up_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_luma_coefr_up)
{
    u_isp_cc_lumafact o_isp_cc_lumafact;
    o_isp_cc_lumafact.u32 = be_reg->isp_cc_lumafact.u32;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefr_up = uisp_cc_luma_coefr_up;
    be_reg->isp_cc_lumafact.u32 = o_isp_cc_lumafact.u32;
}
/*  description   set the value of the member isp_cc_lumafact.isp_cc_luma_coefb_up */
/*  input         unsigned int uisp_cc_luma_coefb_up  5 bits */
static __inline td_void isp_cc_luma_coefb_up_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_luma_coefb_up)
{
    u_isp_cc_lumafact o_isp_cc_lumafact;
    o_isp_cc_lumafact.u32 = be_reg->isp_cc_lumafact.u32;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefb_up = uisp_cc_luma_coefb_up;
    be_reg->isp_cc_lumafact.u32 = o_isp_cc_lumafact.u32;
}
/*  description   set the value of the member isp_cc_colortone_rb_gain.isp_cc_b_gain */
/*  input         unsigned int uisp_cc_b_gain  12 bits */
static __inline td_void isp_cc_b_gain_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_b_gain)
{
    u_isp_cc_colortone_rb_gain o_isp_cc_colortone_rb_gain;
    o_isp_cc_colortone_rb_gain.u32 = be_reg->isp_cc_colortone_rb_gain.u32;
    o_isp_cc_colortone_rb_gain.bits.isp_cc_b_gain = uisp_cc_b_gain;
    be_reg->isp_cc_colortone_rb_gain.u32 = o_isp_cc_colortone_rb_gain.u32;
}
/*  description   set the value of the member isp_cc_colortone_rb_gain.isp_cc_r_gain */
/*  input         unsigned int uisp_cc_r_gain  12 bits */
static __inline td_void isp_cc_r_gain_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_r_gain)
{
    u_isp_cc_colortone_rb_gain o_isp_cc_colortone_rb_gain;
    o_isp_cc_colortone_rb_gain.u32 = be_reg->isp_cc_colortone_rb_gain.u32;
    o_isp_cc_colortone_rb_gain.bits.isp_cc_r_gain = uisp_cc_r_gain;
    be_reg->isp_cc_colortone_rb_gain.u32 = o_isp_cc_colortone_rb_gain.u32;
}
/*  description   set the value of the member isp_cc_colortone_g_gain.isp_cc_g_gain */
/*  input         unsigned int uisp_cc_g_gain  12 bits */
static __inline td_void isp_cc_g_gain_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_g_gain)
{
    u_isp_cc_colortone_g_gain o_isp_cc_colortone_g_gain;
    o_isp_cc_colortone_g_gain.u32 = be_reg->isp_cc_colortone_g_gain.u32;
    o_isp_cc_colortone_g_gain.bits.isp_cc_g_gain = uisp_cc_g_gain;
    be_reg->isp_cc_colortone_g_gain.u32 = o_isp_cc_colortone_g_gain.u32;
}
/*  description   set the value of the member isp_cc_in_dc0.isp_cc_in_dc0 */
/*  input         unsigned int uisp_cc_in_dc0  13 bits */
static __inline td_void isp_cc_in_dc0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_in_dc0)
{
    u_isp_cc_in_dc0 o_isp_cc_in_dc0;
    o_isp_cc_in_dc0.u32 = be_reg->isp_cc_in_dc0.u32;
    o_isp_cc_in_dc0.bits.isp_cc_in_dc0 = uisp_cc_in_dc0;
    be_reg->isp_cc_in_dc0.u32 = o_isp_cc_in_dc0.u32;
}
/*  description   set the value of the member isp_cc_in_dc1.isp_cc_in_dc1 */
/*  input         unsigned int uisp_cc_in_dc1  13 bits */
static __inline td_void isp_cc_in_dc1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_in_dc1)
{
    u_isp_cc_in_dc1 o_isp_cc_in_dc1;
    o_isp_cc_in_dc1.u32 = be_reg->isp_cc_in_dc1.u32;
    o_isp_cc_in_dc1.bits.isp_cc_in_dc1 = uisp_cc_in_dc1;
    be_reg->isp_cc_in_dc1.u32 = o_isp_cc_in_dc1.u32;
}
/*  description   set the value of the member isp_cc_in_dc2.isp_cc_in_dc2 */
/*  input         unsigned int uisp_cc_in_dc2  13 bits */
static __inline td_void isp_cc_in_dc2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_in_dc2)
{
    u_isp_cc_in_dc2 o_isp_cc_in_dc2;
    o_isp_cc_in_dc2.u32 = be_reg->isp_cc_in_dc2.u32;
    o_isp_cc_in_dc2.bits.isp_cc_in_dc2 = uisp_cc_in_dc2;
    be_reg->isp_cc_in_dc2.u32 = o_isp_cc_in_dc2.u32;
}
/*  description   set the value of the member isp_cc_out_dc0.isp_cc_out_dc0 */
/*  input         unsigned int uisp_cc_out_dc0  13 bits */
static __inline td_void isp_cc_out_dc0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_out_dc0)
{
    u_isp_cc_out_dc0 o_isp_cc_out_dc0;
    o_isp_cc_out_dc0.u32 = be_reg->isp_cc_out_dc0.u32;
    o_isp_cc_out_dc0.bits.isp_cc_out_dc0 = uisp_cc_out_dc0;
    be_reg->isp_cc_out_dc0.u32 = o_isp_cc_out_dc0.u32;
}
/*  description   set the value of the member isp_cc_out_dc1.isp_cc_out_dc1 */
/*  input         unsigned int uisp_cc_out_dc1  13 bits */
static __inline td_void isp_cc_out_dc1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_out_dc1)
{
    u_isp_cc_out_dc1 o_isp_cc_out_dc1;
    o_isp_cc_out_dc1.u32 = be_reg->isp_cc_out_dc1.u32;
    o_isp_cc_out_dc1.bits.isp_cc_out_dc1 = uisp_cc_out_dc1;
    be_reg->isp_cc_out_dc1.u32 = o_isp_cc_out_dc1.u32;
}
/*  description   set the value of the member isp_cc_out_dc2.isp_cc_out_dc2 */
/*  input         unsigned int uisp_cc_out_dc2  13 bits */
static __inline td_void isp_cc_out_dc2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_out_dc2)
{
    u_isp_cc_out_dc2 o_isp_cc_out_dc2;
    o_isp_cc_out_dc2.u32 = be_reg->isp_cc_out_dc2.u32;
    o_isp_cc_out_dc2.bits.isp_cc_out_dc2 = uisp_cc_out_dc2;
    be_reg->isp_cc_out_dc2.u32 = o_isp_cc_out_dc2.u32;
}


/*  description   set the value of the member isp_cc_dgain_pos0.isp_cc_dgain_pos_0 */
/*  input         unsigned int uisp_cc_dgain_pos_0  13 bits */
static __inline td_void isp_cc_dgain_pos_0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_0)
{
    u_isp_cc_dgain_pos0 o_isp_cc_dgain_pos0;
    o_isp_cc_dgain_pos0.u32 = be_reg->isp_cc_dgain_pos0.u32;
    o_isp_cc_dgain_pos0.bits.isp_cc_dgain_pos_0 = uisp_cc_dgain_pos_0;
    be_reg->isp_cc_dgain_pos0.u32 = o_isp_cc_dgain_pos0.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos0.isp_cc_dgain_pos_1 */
/*  input         unsigned int uisp_cc_dgain_pos_1  13 bits */
static __inline td_void isp_cc_dgain_pos_1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_1)
{
    u_isp_cc_dgain_pos0 o_isp_cc_dgain_pos0;
    o_isp_cc_dgain_pos0.u32 = be_reg->isp_cc_dgain_pos0.u32;
    o_isp_cc_dgain_pos0.bits.isp_cc_dgain_pos_1 = uisp_cc_dgain_pos_1;
    be_reg->isp_cc_dgain_pos0.u32 = o_isp_cc_dgain_pos0.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos1.isp_cc_dgain_pos_2 */
/*  input         unsigned int uisp_cc_dgain_pos_2  13 bits */
static __inline td_void isp_cc_dgain_pos_2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_2)
{
    u_isp_cc_dgain_pos1 o_isp_cc_dgain_pos1;
    o_isp_cc_dgain_pos1.u32 = be_reg->isp_cc_dgain_pos1.u32;
    o_isp_cc_dgain_pos1.bits.isp_cc_dgain_pos_2 = uisp_cc_dgain_pos_2;
    be_reg->isp_cc_dgain_pos1.u32 = o_isp_cc_dgain_pos1.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos1.isp_cc_dgain_pos_3 */
/*  input         unsigned int uisp_cc_dgain_pos_3  13 bits */
static __inline td_void isp_cc_dgain_pos_3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_3)
{
    u_isp_cc_dgain_pos1 o_isp_cc_dgain_pos1;
    o_isp_cc_dgain_pos1.u32 = be_reg->isp_cc_dgain_pos1.u32;
    o_isp_cc_dgain_pos1.bits.isp_cc_dgain_pos_3 = uisp_cc_dgain_pos_3;
    be_reg->isp_cc_dgain_pos1.u32 = o_isp_cc_dgain_pos1.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos2.isp_cc_dgain_pos_4 */
/*  input         unsigned int uisp_cc_dgain_pos_4  13 bits */
static __inline td_void isp_cc_dgain_pos_4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_4)
{
    u_isp_cc_dgain_pos2 o_isp_cc_dgain_pos2;
    o_isp_cc_dgain_pos2.u32 = be_reg->isp_cc_dgain_pos2.u32;
    o_isp_cc_dgain_pos2.bits.isp_cc_dgain_pos_4 = uisp_cc_dgain_pos_4;
    be_reg->isp_cc_dgain_pos2.u32 = o_isp_cc_dgain_pos2.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos2.isp_cc_dgain_pos_5 */
/*  input         unsigned int uisp_cc_dgain_pos_5  13 bits */
static __inline td_void isp_cc_dgain_pos_5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_5)
{
    u_isp_cc_dgain_pos2 o_isp_cc_dgain_pos2;
    o_isp_cc_dgain_pos2.u32 = be_reg->isp_cc_dgain_pos2.u32;
    o_isp_cc_dgain_pos2.bits.isp_cc_dgain_pos_5 = uisp_cc_dgain_pos_5;
    be_reg->isp_cc_dgain_pos2.u32 = o_isp_cc_dgain_pos2.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos3.isp_cc_dgain_pos_6 */
/*  input         unsigned int uisp_cc_dgain_pos_6  13 bits */
static __inline td_void isp_cc_dgain_pos_6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_6)
{
    u_isp_cc_dgain_pos3 o_isp_cc_dgain_pos3;
    o_isp_cc_dgain_pos3.u32 = be_reg->isp_cc_dgain_pos3.u32;
    o_isp_cc_dgain_pos3.bits.isp_cc_dgain_pos_6 = uisp_cc_dgain_pos_6;
    be_reg->isp_cc_dgain_pos3.u32 = o_isp_cc_dgain_pos3.u32;
}
/*  description   set the value of the member isp_cc_dgain_pos3.isp_cc_dgain_pos_7 */
/*  input         unsigned int uisp_cc_dgain_pos_7  13 bits */
static __inline td_void isp_cc_dgain_pos_7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_pos_7)
{
    u_isp_cc_dgain_pos3 o_isp_cc_dgain_pos3;
    o_isp_cc_dgain_pos3.u32 = be_reg->isp_cc_dgain_pos3.u32;
    o_isp_cc_dgain_pos3.bits.isp_cc_dgain_pos_7 = uisp_cc_dgain_pos_7;
    be_reg->isp_cc_dgain_pos3.u32 = o_isp_cc_dgain_pos3.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg0.isp_cc_dgain_inseg_0 */
/*  input         unsigned int uisp_cc_dgain_inseg_0  7 bits */
static __inline td_void isp_cc_dgain_inseg_0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_0)
{
    u_isp_cc_dgain_inseg0 o_isp_cc_dgain_inseg0;
    o_isp_cc_dgain_inseg0.u32 = be_reg->isp_cc_dgain_inseg0.u32;
    o_isp_cc_dgain_inseg0.bits.isp_cc_dgain_inseg_0 = uisp_cc_dgain_inseg_0;
    be_reg->isp_cc_dgain_inseg0.u32 = o_isp_cc_dgain_inseg0.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg0.isp_cc_dgain_inseg_1 */
/*  input         unsigned int uisp_cc_dgain_inseg_1  7 bits */
static __inline td_void isp_cc_dgain_inseg_1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_1)
{
    u_isp_cc_dgain_inseg0 o_isp_cc_dgain_inseg0;
    o_isp_cc_dgain_inseg0.u32 = be_reg->isp_cc_dgain_inseg0.u32;
    o_isp_cc_dgain_inseg0.bits.isp_cc_dgain_inseg_1 = uisp_cc_dgain_inseg_1;
    be_reg->isp_cc_dgain_inseg0.u32 = o_isp_cc_dgain_inseg0.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg0.isp_cc_dgain_inseg_2 */
/*  input         unsigned int uisp_cc_dgain_inseg_2  7 bits */
static __inline td_void isp_cc_dgain_inseg_2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_2)
{
    u_isp_cc_dgain_inseg0 o_isp_cc_dgain_inseg0;
    o_isp_cc_dgain_inseg0.u32 = be_reg->isp_cc_dgain_inseg0.u32;
    o_isp_cc_dgain_inseg0.bits.isp_cc_dgain_inseg_2 = uisp_cc_dgain_inseg_2;
    be_reg->isp_cc_dgain_inseg0.u32 = o_isp_cc_dgain_inseg0.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg0.isp_cc_dgain_inseg_3 */
/*  input         unsigned int uisp_cc_dgain_inseg_3  7 bits */
static __inline td_void isp_cc_dgain_inseg_3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_3)
{
    u_isp_cc_dgain_inseg0 o_isp_cc_dgain_inseg0;
    o_isp_cc_dgain_inseg0.u32 = be_reg->isp_cc_dgain_inseg0.u32;
    o_isp_cc_dgain_inseg0.bits.isp_cc_dgain_inseg_3 = uisp_cc_dgain_inseg_3;
    be_reg->isp_cc_dgain_inseg0.u32 = o_isp_cc_dgain_inseg0.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg1.isp_cc_dgain_inseg_4 */
/*  input         unsigned int uisp_cc_dgain_inseg_4  7 bits */
static __inline td_void isp_cc_dgain_inseg_4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_4)
{
    u_isp_cc_dgain_inseg1 o_isp_cc_dgain_inseg1;
    o_isp_cc_dgain_inseg1.u32 = be_reg->isp_cc_dgain_inseg1.u32;
    o_isp_cc_dgain_inseg1.bits.isp_cc_dgain_inseg_4 = uisp_cc_dgain_inseg_4;
    be_reg->isp_cc_dgain_inseg1.u32 = o_isp_cc_dgain_inseg1.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg1.isp_cc_dgain_inseg_5 */
/*  input         unsigned int uisp_cc_dgain_inseg_5  7 bits */
static __inline td_void isp_cc_dgain_inseg_5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_5)
{
    u_isp_cc_dgain_inseg1 o_isp_cc_dgain_inseg1;
    o_isp_cc_dgain_inseg1.u32 = be_reg->isp_cc_dgain_inseg1.u32;
    o_isp_cc_dgain_inseg1.bits.isp_cc_dgain_inseg_5 = uisp_cc_dgain_inseg_5;
    be_reg->isp_cc_dgain_inseg1.u32 = o_isp_cc_dgain_inseg1.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg1.isp_cc_dgain_inseg_6 */
/*  input         unsigned int uisp_cc_dgain_inseg_6  7 bits */
static __inline td_void isp_cc_dgain_inseg_6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_6)
{
    u_isp_cc_dgain_inseg1 o_isp_cc_dgain_inseg1;
    o_isp_cc_dgain_inseg1.u32 = be_reg->isp_cc_dgain_inseg1.u32;
    o_isp_cc_dgain_inseg1.bits.isp_cc_dgain_inseg_6 = uisp_cc_dgain_inseg_6;
    be_reg->isp_cc_dgain_inseg1.u32 = o_isp_cc_dgain_inseg1.u32;
}
/*  description   set the value of the member isp_cc_dgain_inseg1.isp_cc_dgain_inseg_7 */
/*  input         unsigned int uisp_cc_dgain_inseg_7  7 bits */
static __inline td_void isp_cc_dgain_inseg_7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_inseg_7)
{
    u_isp_cc_dgain_inseg1 o_isp_cc_dgain_inseg1;
    o_isp_cc_dgain_inseg1.u32 = be_reg->isp_cc_dgain_inseg1.u32;
    o_isp_cc_dgain_inseg1.bits.isp_cc_dgain_inseg_7 = uisp_cc_dgain_inseg_7;
    be_reg->isp_cc_dgain_inseg1.u32 = o_isp_cc_dgain_inseg1.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step0 */
/*  input         unsigned int uisp_cc_dgain_step0  3 bits */
static __inline td_void isp_cc_dgain_step0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step0)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step0 = uisp_cc_dgain_step0;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step1 */
/*  input         unsigned int uisp_cc_dgain_step1  3 bits */
static __inline td_void isp_cc_dgain_step1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step1)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step1 = uisp_cc_dgain_step1;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step2 */
/*  input         unsigned int uisp_cc_dgain_step2  3 bits */
static __inline td_void isp_cc_dgain_step2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step2)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step2 = uisp_cc_dgain_step2;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step3 */
/*  input         unsigned int uisp_cc_dgain_step3  3 bits */
static __inline td_void isp_cc_dgain_step3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step3)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step3 = uisp_cc_dgain_step3;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step4 */
/*  input         unsigned int uisp_cc_dgain_step4  3 bits */
static __inline td_void isp_cc_dgain_step4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step4)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step4 = uisp_cc_dgain_step4;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step5 */
/*  input         unsigned int uisp_cc_dgain_step5  3 bits */
static __inline td_void isp_cc_dgain_step5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step5)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step5 = uisp_cc_dgain_step5;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step6 */
/*  input         unsigned int uisp_cc_dgain_step6  3 bits */
static __inline td_void isp_cc_dgain_step6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step6)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step6 = uisp_cc_dgain_step6;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_dgain_step.isp_cc_dgain_step7 */
/*  input         unsigned int uisp_cc_dgain_step7  3 bits */
static __inline td_void isp_cc_dgain_step7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_step7)
{
    u_isp_cc_dgain_step o_isp_cc_dgain_step;
    o_isp_cc_dgain_step.u32 = be_reg->isp_cc_dgain_step.u32;
    o_isp_cc_dgain_step.bits.isp_cc_dgain_step7 = uisp_cc_dgain_step7;
    be_reg->isp_cc_dgain_step.u32 = o_isp_cc_dgain_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos0.isp_cc_gamma_pos_0 */
/*  input         unsigned int uisp_cc_gamma_pos_0  15 bits */
static __inline td_void isp_cc_gamma_pos_0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_0)
{
    u_isp_cc_gamma_pos0 o_isp_cc_gamma_pos0;
    o_isp_cc_gamma_pos0.u32 = be_reg->isp_cc_gamma_pos0.u32;
    o_isp_cc_gamma_pos0.bits.isp_cc_gamma_pos_0 = uisp_cc_gamma_pos_0;
    be_reg->isp_cc_gamma_pos0.u32 = o_isp_cc_gamma_pos0.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos0.isp_cc_gamma_pos_1 */
/*  input         unsigned int uisp_cc_gamma_pos_1  15 bits */
static __inline td_void isp_cc_gamma_pos_1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_1)
{
    u_isp_cc_gamma_pos0 o_isp_cc_gamma_pos0;
    o_isp_cc_gamma_pos0.u32 = be_reg->isp_cc_gamma_pos0.u32;
    o_isp_cc_gamma_pos0.bits.isp_cc_gamma_pos_1 = uisp_cc_gamma_pos_1;
    be_reg->isp_cc_gamma_pos0.u32 = o_isp_cc_gamma_pos0.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos1.isp_cc_gamma_pos_2 */
/*  input         unsigned int uisp_cc_gamma_pos_2  15 bits */
static __inline td_void isp_cc_gamma_pos_2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_2)
{
    u_isp_cc_gamma_pos1 o_isp_cc_gamma_pos1;
    o_isp_cc_gamma_pos1.u32 = be_reg->isp_cc_gamma_pos1.u32;
    o_isp_cc_gamma_pos1.bits.isp_cc_gamma_pos_2 = uisp_cc_gamma_pos_2;
    be_reg->isp_cc_gamma_pos1.u32 = o_isp_cc_gamma_pos1.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos1.isp_cc_gamma_pos_3 */
/*  input         unsigned int uisp_cc_gamma_pos_3  15 bits */
static __inline td_void isp_cc_gamma_pos_3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_3)
{
    u_isp_cc_gamma_pos1 o_isp_cc_gamma_pos1;
    o_isp_cc_gamma_pos1.u32 = be_reg->isp_cc_gamma_pos1.u32;
    o_isp_cc_gamma_pos1.bits.isp_cc_gamma_pos_3 = uisp_cc_gamma_pos_3;
    be_reg->isp_cc_gamma_pos1.u32 = o_isp_cc_gamma_pos1.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos2.isp_cc_gamma_pos_4 */
/*  input         unsigned int uisp_cc_gamma_pos_4  15 bits */
static __inline td_void isp_cc_gamma_pos_4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_4)
{
    u_isp_cc_gamma_pos2 o_isp_cc_gamma_pos2;
    o_isp_cc_gamma_pos2.u32 = be_reg->isp_cc_gamma_pos2.u32;
    o_isp_cc_gamma_pos2.bits.isp_cc_gamma_pos_4 = uisp_cc_gamma_pos_4;
    be_reg->isp_cc_gamma_pos2.u32 = o_isp_cc_gamma_pos2.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos2.isp_cc_gamma_pos_5 */
/*  input         unsigned int uisp_cc_gamma_pos_5  15 bits */
static __inline td_void isp_cc_gamma_pos_5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_5)
{
    u_isp_cc_gamma_pos2 o_isp_cc_gamma_pos2;
    o_isp_cc_gamma_pos2.u32 = be_reg->isp_cc_gamma_pos2.u32;
    o_isp_cc_gamma_pos2.bits.isp_cc_gamma_pos_5 = uisp_cc_gamma_pos_5;
    be_reg->isp_cc_gamma_pos2.u32 = o_isp_cc_gamma_pos2.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos3.isp_cc_gamma_pos_6 */
/*  input         unsigned int uisp_cc_gamma_pos_6  15 bits */
static __inline td_void isp_cc_gamma_pos_6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_6)
{
    u_isp_cc_gamma_pos3 o_isp_cc_gamma_pos3;
    o_isp_cc_gamma_pos3.u32 = be_reg->isp_cc_gamma_pos3.u32;
    o_isp_cc_gamma_pos3.bits.isp_cc_gamma_pos_6 = uisp_cc_gamma_pos_6;
    be_reg->isp_cc_gamma_pos3.u32 = o_isp_cc_gamma_pos3.u32;
}
/*  description   set the value of the member isp_cc_gamma_pos3.isp_cc_gamma_pos_7 */
/*  input         unsigned int uisp_cc_gamma_pos_7  15 bits */
static __inline td_void isp_cc_gamma_pos_7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_pos_7)
{
    u_isp_cc_gamma_pos3 o_isp_cc_gamma_pos3;
    o_isp_cc_gamma_pos3.u32 = be_reg->isp_cc_gamma_pos3.u32;
    o_isp_cc_gamma_pos3.bits.isp_cc_gamma_pos_7 = uisp_cc_gamma_pos_7;
    be_reg->isp_cc_gamma_pos3.u32 = o_isp_cc_gamma_pos3.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg0.isp_cc_gamma_inseg_0 */
/*  input         unsigned int uisp_cc_gamma_inseg_0  7 bits */
static __inline td_void isp_cc_gamma_inseg_0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_0)
{
    u_isp_cc_gamma_inseg0 o_isp_cc_gamma_inseg0;
    o_isp_cc_gamma_inseg0.u32 = be_reg->isp_cc_gamma_inseg0.u32;
    o_isp_cc_gamma_inseg0.bits.isp_cc_gamma_inseg_0 = uisp_cc_gamma_inseg_0;
    be_reg->isp_cc_gamma_inseg0.u32 = o_isp_cc_gamma_inseg0.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg0.isp_cc_gamma_inseg_1 */
/*  input         unsigned int uisp_cc_gamma_inseg_1  7 bits */
static __inline td_void isp_cc_gamma_inseg_1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_1)
{
    u_isp_cc_gamma_inseg0 o_isp_cc_gamma_inseg0;
    o_isp_cc_gamma_inseg0.u32 = be_reg->isp_cc_gamma_inseg0.u32;
    o_isp_cc_gamma_inseg0.bits.isp_cc_gamma_inseg_1 = uisp_cc_gamma_inseg_1;
    be_reg->isp_cc_gamma_inseg0.u32 = o_isp_cc_gamma_inseg0.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg0.isp_cc_gamma_inseg_2 */
/*  input         unsigned int uisp_cc_gamma_inseg_2  7 bits */
static __inline td_void isp_cc_gamma_inseg_2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_2)
{
    u_isp_cc_gamma_inseg0 o_isp_cc_gamma_inseg0;
    o_isp_cc_gamma_inseg0.u32 = be_reg->isp_cc_gamma_inseg0.u32;
    o_isp_cc_gamma_inseg0.bits.isp_cc_gamma_inseg_2 = uisp_cc_gamma_inseg_2;
    be_reg->isp_cc_gamma_inseg0.u32 = o_isp_cc_gamma_inseg0.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg0.isp_cc_gamma_inseg_3 */
/*  input         unsigned int uisp_cc_gamma_inseg_3  7 bits */
static __inline td_void isp_cc_gamma_inseg_3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_3)
{
    u_isp_cc_gamma_inseg0 o_isp_cc_gamma_inseg0;
    o_isp_cc_gamma_inseg0.u32 = be_reg->isp_cc_gamma_inseg0.u32;
    o_isp_cc_gamma_inseg0.bits.isp_cc_gamma_inseg_3 = uisp_cc_gamma_inseg_3;
    be_reg->isp_cc_gamma_inseg0.u32 = o_isp_cc_gamma_inseg0.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg1.isp_cc_gamma_inseg_4 */
/*  input         unsigned int uisp_cc_gamma_inseg_4  7 bits */
static __inline td_void isp_cc_gamma_inseg_4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_4)
{
    u_isp_cc_gamma_inseg1 o_isp_cc_gamma_inseg1;
    o_isp_cc_gamma_inseg1.u32 = be_reg->isp_cc_gamma_inseg1.u32;
    o_isp_cc_gamma_inseg1.bits.isp_cc_gamma_inseg_4 = uisp_cc_gamma_inseg_4;
    be_reg->isp_cc_gamma_inseg1.u32 = o_isp_cc_gamma_inseg1.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg1.isp_cc_gamma_inseg_5 */
/*  input         unsigned int uisp_cc_gamma_inseg_5  7 bits */
static __inline td_void isp_cc_gamma_inseg_5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_5)
{
    u_isp_cc_gamma_inseg1 o_isp_cc_gamma_inseg1;
    o_isp_cc_gamma_inseg1.u32 = be_reg->isp_cc_gamma_inseg1.u32;
    o_isp_cc_gamma_inseg1.bits.isp_cc_gamma_inseg_5 = uisp_cc_gamma_inseg_5;
    be_reg->isp_cc_gamma_inseg1.u32 = o_isp_cc_gamma_inseg1.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg1.isp_cc_gamma_inseg_6 */
/*  input         unsigned int uisp_cc_gamma_inseg_6  7 bits */
static __inline td_void isp_cc_gamma_inseg_6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_6)
{
    u_isp_cc_gamma_inseg1 o_isp_cc_gamma_inseg1;
    o_isp_cc_gamma_inseg1.u32 = be_reg->isp_cc_gamma_inseg1.u32;
    o_isp_cc_gamma_inseg1.bits.isp_cc_gamma_inseg_6 = uisp_cc_gamma_inseg_6;
    be_reg->isp_cc_gamma_inseg1.u32 = o_isp_cc_gamma_inseg1.u32;
}
/*  description   set the value of the member isp_cc_gamma_inseg1.isp_cc_gamma_inseg_7 */
/*  input         unsigned int uisp_cc_gamma_inseg_7  7 bits */
static __inline td_void isp_cc_gamma_inseg_7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_inseg_7)
{
    u_isp_cc_gamma_inseg1 o_isp_cc_gamma_inseg1;
    o_isp_cc_gamma_inseg1.u32 = be_reg->isp_cc_gamma_inseg1.u32;
    o_isp_cc_gamma_inseg1.bits.isp_cc_gamma_inseg_7 = uisp_cc_gamma_inseg_7;
    be_reg->isp_cc_gamma_inseg1.u32 = o_isp_cc_gamma_inseg1.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step0 */
/*  input         unsigned int uisp_cc_gamma_step0  4 bits */
static __inline td_void isp_cc_gamma_step0_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step0)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step0 = uisp_cc_gamma_step0;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step1 */
/*  input         unsigned int uisp_cc_gamma_step1  4 bits */
static __inline td_void isp_cc_gamma_step1_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step1)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step1 = uisp_cc_gamma_step1;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step2 */
/*  input         unsigned int uisp_cc_gamma_step2  4 bits */
static __inline td_void isp_cc_gamma_step2_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step2)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step2 = uisp_cc_gamma_step2;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step3 */
/*  input         unsigned int uisp_cc_gamma_step3  4 bits */
static __inline td_void isp_cc_gamma_step3_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step3)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step3 = uisp_cc_gamma_step3;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step4 */
/*  input         unsigned int uisp_cc_gamma_step4  4 bits */
static __inline td_void isp_cc_gamma_step4_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step4)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step4 = uisp_cc_gamma_step4;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step5 */
/*  input         unsigned int uisp_cc_gamma_step5  4 bits */
static __inline td_void isp_cc_gamma_step5_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step5)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step5 = uisp_cc_gamma_step5;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step6 */
/*  input         unsigned int uisp_cc_gamma_step6  4 bits */
static __inline td_void isp_cc_gamma_step6_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step6)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step6 = uisp_cc_gamma_step6;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_gamma_step.isp_cc_gamma_step7 */
/*  input         unsigned int uisp_cc_gamma_step7  4 bits */
static __inline td_void isp_cc_gamma_step7_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_step7)
{
    u_isp_cc_gamma_step o_isp_cc_gamma_step;
    o_isp_cc_gamma_step.u32 = be_reg->isp_cc_gamma_step.u32;
    o_isp_cc_gamma_step.bits.isp_cc_gamma_step7 = uisp_cc_gamma_step7;
    be_reg->isp_cc_gamma_step.u32 = o_isp_cc_gamma_step.u32;
}
/*  description   set the value of the member isp_cc_luma_wdr_coef.isp_cc_luma_wdr_coefr */
/*  input         unsigned int uisp_cc_luma_wdr_coefr  5 bits */
static __inline td_void isp_cc_luma_wdr_coefr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_luma_wdr_coefr)
{
    u_isp_cc_luma_wdr_coef o_isp_cc_luma_wdr_coef;
    o_isp_cc_luma_wdr_coef.u32 = be_reg->isp_cc_luma_wdr_coef.u32;
    o_isp_cc_luma_wdr_coef.bits.isp_cc_luma_wdr_coefr = uisp_cc_luma_wdr_coefr;
    be_reg->isp_cc_luma_wdr_coef.u32 = o_isp_cc_luma_wdr_coef.u32;
}
/*  description   set the value of the member isp_cc_luma_wdr_coef.isp_cc_luma_wdr_coefb */
/*  input         unsigned int uisp_cc_luma_wdr_coefb  5 bits */
static __inline td_void isp_cc_luma_wdr_coefb_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_luma_wdr_coefb)
{
    u_isp_cc_luma_wdr_coef o_isp_cc_luma_wdr_coef;
    o_isp_cc_luma_wdr_coef.u32 = be_reg->isp_cc_luma_wdr_coef.u32;
    o_isp_cc_luma_wdr_coef.bits.isp_cc_luma_wdr_coefb = uisp_cc_luma_wdr_coefb;
    be_reg->isp_cc_luma_wdr_coef.u32 = o_isp_cc_luma_wdr_coef.u32;
}

/*  description   set the value of the member isp_cc_stt2lut_cfg.isp_cc_stt2lut_en */
/*  input         unsigned int uisp_cc_stt2lut_en  1 bits */
static __inline td_void isp_cc_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_stt2lut_en)
{
    u_isp_cc_stt2lut_cfg o_isp_cc_stt2lut_cfg;
    o_isp_cc_stt2lut_cfg.u32 = be_reg->isp_cc_stt2lut_cfg.u32;
    o_isp_cc_stt2lut_cfg.bits.isp_cc_stt2lut_en = uisp_cc_stt2lut_en;
    be_reg->isp_cc_stt2lut_cfg.u32 = o_isp_cc_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_cc_stt2lut_regnew.isp_cc_stt2lut_regnew */
/*  input         unsigned int uisp_cc_stt2lut_regnew  1 bits */
static __inline td_void isp_cc_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_stt2lut_regnew)
{
    u_isp_cc_stt2lut_regnew o_isp_cc_stt2lut_regnew;
    o_isp_cc_stt2lut_regnew.u32 = be_reg->isp_cc_stt2lut_regnew.u32;
    o_isp_cc_stt2lut_regnew.bits.isp_cc_stt2lut_regnew = uisp_cc_stt2lut_regnew;
    be_reg->isp_cc_stt2lut_regnew.u32 = o_isp_cc_stt2lut_regnew.u32;
}
/*  description   set the value of the member isp_cc_stt2lut_abn.isp_cc_stt2lut_clr */
/*  input         unsigned int uisp_cc_stt2lut_clr  1 bits */
static __inline td_void isp_cc_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_stt2lut_clr)
{
    u_isp_cc_stt2lut_abn o_isp_cc_stt2lut_abn;
    o_isp_cc_stt2lut_abn.u32 = be_reg->isp_cc_stt2lut_abn.u32;
    o_isp_cc_stt2lut_abn.bits.isp_cc_stt2lut_clr = uisp_cc_stt2lut_clr;
    be_reg->isp_cc_stt2lut_abn.u32 = o_isp_cc_stt2lut_abn.u32;
}

static __inline td_u16 isp_cc_stt2lut_info_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_cc_stt2lut_abn.bits.isp_cc_stt2lut_info;
}

/*  description   set the value of the member isp_cc_dgain_lut_waddr.isp_cc_dgain_lut_waddr */
/*  input         unsigned int uisp_cc_dgain_lut_waddr  7 bits */
static __inline td_void isp_cc_dgain_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_lut_waddr)
{
    u_isp_cc_dgain_lut_waddr o_isp_cc_dgain_lut_waddr;
    o_isp_cc_dgain_lut_waddr.u32 = be_reg->isp_cc_dgain_lut_waddr.u32;
    o_isp_cc_dgain_lut_waddr.bits.isp_cc_dgain_lut_waddr = uisp_cc_dgain_lut_waddr;
    be_reg->isp_cc_dgain_lut_waddr.u32 = o_isp_cc_dgain_lut_waddr.u32;
}
/*  description   set the value of the member isp_cc_dgain_lut_wdata.isp_cc_dgain_lut_wdata */
/*  input         unsigned int uisp_cc_dgain_lut_wdata  14 bits */
static __inline td_void isp_cc_dgain_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_lut_wdata)
{
    u_isp_cc_dgain_lut_wdata o_isp_cc_dgain_lut_wdata;
    o_isp_cc_dgain_lut_wdata.u32 = be_reg->isp_cc_dgain_lut_wdata.u32;
    o_isp_cc_dgain_lut_wdata.bits.isp_cc_dgain_lut_wdata = uisp_cc_dgain_lut_wdata;
    be_reg->isp_cc_dgain_lut_wdata.u32 = o_isp_cc_dgain_lut_wdata.u32;
}
/*  description   set the value of the member isp_cc_dgain_lut_raddr.isp_cc_dgain_lut_raddr */
/*  input         unsigned int uisp_cc_dgain_lut_raddr  7 bits */
static __inline td_void isp_cc_dgain_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_lut_raddr)
{
    u_isp_cc_dgain_lut_raddr o_isp_cc_dgain_lut_raddr;
    o_isp_cc_dgain_lut_raddr.u32 = be_reg->isp_cc_dgain_lut_raddr.u32;
    o_isp_cc_dgain_lut_raddr.bits.isp_cc_dgain_lut_raddr = uisp_cc_dgain_lut_raddr;
    be_reg->isp_cc_dgain_lut_raddr.u32 = o_isp_cc_dgain_lut_raddr.u32;
}
/*  description   set the value of the member isp_cc_dgain_lut_rdata.isp_cc_dgain_lut_rdata */
/*  input         unsigned int uisp_cc_dgain_lut_rdata  14 bits */
static __inline td_void isp_cc_dgain_lut_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_dgain_lut_rdata)
{
    u_isp_cc_dgain_lut_rdata o_isp_cc_dgain_lut_rdata;
    o_isp_cc_dgain_lut_rdata.u32 = be_reg->isp_cc_dgain_lut_rdata.u32;
    o_isp_cc_dgain_lut_rdata.bits.isp_cc_dgain_lut_rdata = uisp_cc_dgain_lut_rdata;
    be_reg->isp_cc_dgain_lut_rdata.u32 = o_isp_cc_dgain_lut_rdata.u32;
}
/*  description   set the value of the member isp_cc_gamma_lut_waddr.isp_cc_gamma_lut_waddr */
/*  input         unsigned int uisp_cc_gamma_lut_waddr  7 bits */
static __inline td_void isp_cc_gamma_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_lut_waddr)
{
    u_isp_cc_gamma_lut_waddr o_isp_cc_gamma_lut_waddr;
    o_isp_cc_gamma_lut_waddr.u32 = be_reg->isp_cc_gamma_lut_waddr.u32;
    o_isp_cc_gamma_lut_waddr.bits.isp_cc_gamma_lut_waddr = uisp_cc_gamma_lut_waddr;
    be_reg->isp_cc_gamma_lut_waddr.u32 = o_isp_cc_gamma_lut_waddr.u32;
}
/*  description   set the value of the member isp_cc_gamma_lut_wdata.isp_cc_gamma_lut_wdata */
/*  input         unsigned int uisp_cc_gamma_lut_wdata  12 bits */
static __inline td_void isp_cc_gamma_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_lut_wdata)
{
    u_isp_cc_gamma_lut_wdata o_isp_cc_gamma_lut_wdata;
    o_isp_cc_gamma_lut_wdata.u32 = be_reg->isp_cc_gamma_lut_wdata.u32;
    o_isp_cc_gamma_lut_wdata.bits.isp_cc_gamma_lut_wdata = uisp_cc_gamma_lut_wdata;
    be_reg->isp_cc_gamma_lut_wdata.u32 = o_isp_cc_gamma_lut_wdata.u32;
}
/*  description   set the value of the member isp_cc_gamma_lut_raddr.isp_cc_gamma_lut_raddr */
/*  input         unsigned int uisp_cc_gamma_lut_raddr  7 bits */
static __inline td_void isp_cc_gamma_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_lut_raddr)
{
    u_isp_cc_gamma_lut_raddr o_isp_cc_gamma_lut_raddr;
    o_isp_cc_gamma_lut_raddr.u32 = be_reg->isp_cc_gamma_lut_raddr.u32;
    o_isp_cc_gamma_lut_raddr.bits.isp_cc_gamma_lut_raddr = uisp_cc_gamma_lut_raddr;
    be_reg->isp_cc_gamma_lut_raddr.u32 = o_isp_cc_gamma_lut_raddr.u32;
}
/*  description   set the value of the member isp_cc_gamma_lut_rdata.isp_cc_gamma_lut_rdata */
/*  input         unsigned int uisp_cc_gamma_lut_rdata  12 bits */
static __inline td_void isp_cc_gamma_lut_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_cc_gamma_lut_rdata)
{
    u_isp_cc_gamma_lut_rdata o_isp_cc_gamma_lut_rdata;
    o_isp_cc_gamma_lut_rdata.u32 = be_reg->isp_cc_gamma_lut_rdata.u32;
    o_isp_cc_gamma_lut_rdata.bits.isp_cc_gamma_lut_rdata = uisp_cc_gamma_lut_rdata;
    be_reg->isp_cc_gamma_lut_rdata.u32 = o_isp_cc_gamma_lut_rdata.u32;
}

/*  description   set the value of the member isp_csc_coef0.isp_csc_coef00 */
/*  input         unsigned int uisp_csc_coef00  15 bits */
static __inline td_void isp_csc_coef00_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef00)
{
    u_isp_csc_coef0 o_isp_csc_coef0;
    o_isp_csc_coef0.u32 = be_reg->isp_csc_coef0.u32;
    o_isp_csc_coef0.bits.isp_csc_coef00 = uisp_csc_coef00;
    be_reg->isp_csc_coef0.u32 = o_isp_csc_coef0.u32;
}
/*  description   set the value of the member isp_csc_coef0.isp_csc_coef01 */
/*  input         unsigned int uisp_csc_coef01  15 bits */
static __inline td_void isp_csc_coef01_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef01)
{
    u_isp_csc_coef0 o_isp_csc_coef0;
    o_isp_csc_coef0.u32 = be_reg->isp_csc_coef0.u32;
    o_isp_csc_coef0.bits.isp_csc_coef01 = uisp_csc_coef01;
    be_reg->isp_csc_coef0.u32 = o_isp_csc_coef0.u32;
}
/*  description   set the value of the member isp_csc_coef1.isp_csc_coef02 */
/*  input         unsigned int uisp_csc_coef02  15 bits */
static __inline td_void isp_csc_coef02_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef02)
{
    u_isp_csc_coef1 o_isp_csc_coef1;
    o_isp_csc_coef1.u32 = be_reg->isp_csc_coef1.u32;
    o_isp_csc_coef1.bits.isp_csc_coef02 = uisp_csc_coef02;
    be_reg->isp_csc_coef1.u32 = o_isp_csc_coef1.u32;
}
/*  description   set the value of the member isp_csc_coef1.isp_csc_coef10 */
/*  input         unsigned int uisp_csc_coef10  15 bits */
static __inline td_void isp_csc_coef10_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef10)
{
    u_isp_csc_coef1 o_isp_csc_coef1;
    o_isp_csc_coef1.u32 = be_reg->isp_csc_coef1.u32;
    o_isp_csc_coef1.bits.isp_csc_coef10 = uisp_csc_coef10;
    be_reg->isp_csc_coef1.u32 = o_isp_csc_coef1.u32;
}
/*  description   set the value of the member isp_csc_coef2.isp_csc_coef11 */
/*  input         unsigned int uisp_csc_coef11  15 bits */
static __inline td_void isp_csc_coef11_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef11)
{
    u_isp_csc_coef2 o_isp_csc_coef2;
    o_isp_csc_coef2.u32 = be_reg->isp_csc_coef2.u32;
    o_isp_csc_coef2.bits.isp_csc_coef11 = uisp_csc_coef11;
    be_reg->isp_csc_coef2.u32 = o_isp_csc_coef2.u32;
}
/*  description   set the value of the member isp_csc_coef2.isp_csc_coef12 */
/*  input         unsigned int uisp_csc_coef12  15 bits */
static __inline td_void isp_csc_coef12_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef12)
{
    u_isp_csc_coef2 o_isp_csc_coef2;
    o_isp_csc_coef2.u32 = be_reg->isp_csc_coef2.u32;
    o_isp_csc_coef2.bits.isp_csc_coef12 = uisp_csc_coef12;
    be_reg->isp_csc_coef2.u32 = o_isp_csc_coef2.u32;
}
/*  description   set the value of the member isp_csc_coef3.isp_csc_coef20 */
/*  input         unsigned int uisp_csc_coef20  15 bits */
static __inline td_void isp_csc_coef20_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef20)
{
    u_isp_csc_coef3 o_isp_csc_coef3;
    o_isp_csc_coef3.u32 = be_reg->isp_csc_coef3.u32;
    o_isp_csc_coef3.bits.isp_csc_coef20 = uisp_csc_coef20;
    be_reg->isp_csc_coef3.u32 = o_isp_csc_coef3.u32;
}
/*  description   set the value of the member isp_csc_coef3.isp_csc_coef21 */
/*  input         unsigned int uisp_csc_coef21  15 bits */
static __inline td_void isp_csc_coef21_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef21)
{
    u_isp_csc_coef3 o_isp_csc_coef3;
    o_isp_csc_coef3.u32 = be_reg->isp_csc_coef3.u32;
    o_isp_csc_coef3.bits.isp_csc_coef21 = uisp_csc_coef21;
    be_reg->isp_csc_coef3.u32 = o_isp_csc_coef3.u32;
}
/*  description   set the value of the member isp_csc_coef4.isp_csc_coef22 */
/*  input         unsigned int uisp_csc_coef22  15 bits */
static __inline td_void isp_csc_coef22_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_coef22)
{
    u_isp_csc_coef4 o_isp_csc_coef4;
    o_isp_csc_coef4.u32 = be_reg->isp_csc_coef4.u32;
    o_isp_csc_coef4.bits.isp_csc_coef22 = uisp_csc_coef22;
    be_reg->isp_csc_coef4.u32 = o_isp_csc_coef4.u32;
}
/*  description   set the value of the member isp_csc_in_dc0.isp_csc_in_dc0 */
/*  input         unsigned int uisp_csc_in_dc0  11 bits */
static __inline td_void isp_csc_in_dc0_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_in_dc0)
{
    u_isp_csc_in_dc0 o_isp_csc_in_dc0;
    o_isp_csc_in_dc0.u32 = be_reg->isp_csc_in_dc0.u32;
    o_isp_csc_in_dc0.bits.isp_csc_in_dc0 = uisp_csc_in_dc0;
    be_reg->isp_csc_in_dc0.u32 = o_isp_csc_in_dc0.u32;
}
/*  description   set the value of the member isp_csc_in_dc0.isp_csc_in_dc1 */
/*  input         unsigned int uisp_csc_in_dc1  11 bits */
static __inline td_void isp_csc_in_dc1_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_in_dc1)
{
    u_isp_csc_in_dc0 o_isp_csc_in_dc0;
    o_isp_csc_in_dc0.u32 = be_reg->isp_csc_in_dc0.u32;
    o_isp_csc_in_dc0.bits.isp_csc_in_dc1 = uisp_csc_in_dc1;
    be_reg->isp_csc_in_dc0.u32 = o_isp_csc_in_dc0.u32;
}
/*  description   set the value of the member isp_csc_in_dc1.isp_csc_in_dc2 */
/*  input         unsigned int uisp_csc_in_dc2  11 bits */
static __inline td_void isp_csc_in_dc2_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_in_dc2)
{
    u_isp_csc_in_dc1 o_isp_csc_in_dc1;
    o_isp_csc_in_dc1.u32 = be_reg->isp_csc_in_dc1.u32;
    o_isp_csc_in_dc1.bits.isp_csc_in_dc2 = uisp_csc_in_dc2;
    be_reg->isp_csc_in_dc1.u32 = o_isp_csc_in_dc1.u32;
}
/*  description   set the value of the member isp_csc_out_dc0.isp_csc_out_dc0 */
/*  input         unsigned int uisp_csc_out_dc0  11 bits */
static __inline td_void isp_csc_out_dc0_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_out_dc0)
{
    u_isp_csc_out_dc0 o_isp_csc_out_dc0;
    o_isp_csc_out_dc0.u32 = be_reg->isp_csc_out_dc0.u32;
    o_isp_csc_out_dc0.bits.isp_csc_out_dc0 = uisp_csc_out_dc0;
    be_reg->isp_csc_out_dc0.u32 = o_isp_csc_out_dc0.u32;
}
/*  description   set the value of the member isp_csc_out_dc0.isp_csc_out_dc1 */
/*  input         unsigned int uisp_csc_out_dc1  11 bits */
static __inline td_void isp_csc_out_dc1_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_out_dc1)
{
    u_isp_csc_out_dc0 o_isp_csc_out_dc0;
    o_isp_csc_out_dc0.u32 = be_reg->isp_csc_out_dc0.u32;
    o_isp_csc_out_dc0.bits.isp_csc_out_dc1 = uisp_csc_out_dc1;
    be_reg->isp_csc_out_dc0.u32 = o_isp_csc_out_dc0.u32;
}
/*  description   set the value of the member isp_csc_out_dc1.isp_csc_out_dc2 */
/*  input         unsigned int uisp_csc_out_dc2  11 bits */
static __inline td_void isp_csc_out_dc2_write(isp_be_reg_type *be_reg, td_u32 uisp_csc_out_dc2)
{
    u_isp_csc_out_dc1 o_isp_csc_out_dc1;
    o_isp_csc_out_dc1.u32 = be_reg->isp_csc_out_dc1.u32;
    o_isp_csc_out_dc1.bits.isp_csc_out_dc2 = uisp_csc_out_dc2;
    be_reg->isp_csc_out_dc1.u32 = o_isp_csc_out_dc1.u32;
}

/*  description   set the value of the member isp_hcds_coefh0.isp_hcds_coefh1 */
/*  input         unsigned int uisp_hcds_coefh1  11 bits */
static __inline td_void isp_hcds_coefh1_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh1)
{
    u_isp_hcds_coefh0 o_isp_hcds_coefh0;
    o_isp_hcds_coefh0.u32 = be_reg->isp_hcds_coefh0.u32;
    o_isp_hcds_coefh0.bits.isp_hcds_coefh1 = uisp_hcds_coefh1;
    be_reg->isp_hcds_coefh0.u32 = o_isp_hcds_coefh0.u32;
}
/*  description   set the value of the member isp_hcds_coefh0.isp_hcds_coefh0 */
/*  input         unsigned int uisp_hcds_coefh0  11 bits */
static __inline td_void isp_hcds_coefh0_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh0)
{
    u_isp_hcds_coefh0 o_isp_hcds_coefh0;
    o_isp_hcds_coefh0.u32 = be_reg->isp_hcds_coefh0.u32;
    o_isp_hcds_coefh0.bits.isp_hcds_coefh0 = uisp_hcds_coefh0;
    be_reg->isp_hcds_coefh0.u32 = o_isp_hcds_coefh0.u32;
}
/*  description   set the value of the member isp_hcds_coefh1.isp_hcds_coefh3 */
/*  input         unsigned int uisp_hcds_coefh3  11 bits */
static __inline td_void isp_hcds_coefh3_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh3)
{
    u_isp_hcds_coefh1 o_isp_hcds_coefh1;
    o_isp_hcds_coefh1.u32 = be_reg->isp_hcds_coefh1.u32;
    o_isp_hcds_coefh1.bits.isp_hcds_coefh3 = uisp_hcds_coefh3;
    be_reg->isp_hcds_coefh1.u32 = o_isp_hcds_coefh1.u32;
}
/*  description   set the value of the member isp_hcds_coefh1.isp_hcds_coefh2 */
/*  input         unsigned int uisp_hcds_coefh2  11 bits */
static __inline td_void isp_hcds_coefh2_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh2)
{
    u_isp_hcds_coefh1 o_isp_hcds_coefh1;
    o_isp_hcds_coefh1.u32 = be_reg->isp_hcds_coefh1.u32;
    o_isp_hcds_coefh1.bits.isp_hcds_coefh2 = uisp_hcds_coefh2;
    be_reg->isp_hcds_coefh1.u32 = o_isp_hcds_coefh1.u32;
}
/*  description   set the value of the member isp_hcds_coefh2.isp_hcds_coefh5 */
/*  input         unsigned int uisp_hcds_coefh5  11 bits */
static __inline td_void isp_hcds_coefh5_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh5)
{
    u_isp_hcds_coefh2 o_isp_hcds_coefh2;
    o_isp_hcds_coefh2.u32 = be_reg->isp_hcds_coefh2.u32;
    o_isp_hcds_coefh2.bits.isp_hcds_coefh5 = uisp_hcds_coefh5;
    be_reg->isp_hcds_coefh2.u32 = o_isp_hcds_coefh2.u32;
}
/*  description   set the value of the member isp_hcds_coefh2.isp_hcds_coefh4 */
/*  input         unsigned int uisp_hcds_coefh4  11 bits */
static __inline td_void isp_hcds_coefh4_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh4)
{
    u_isp_hcds_coefh2 o_isp_hcds_coefh2;
    o_isp_hcds_coefh2.u32 = be_reg->isp_hcds_coefh2.u32;
    o_isp_hcds_coefh2.bits.isp_hcds_coefh4 = uisp_hcds_coefh4;
    be_reg->isp_hcds_coefh2.u32 = o_isp_hcds_coefh2.u32;
}
/*  description   set the value of the member isp_hcds_coefh3.isp_hcds_coefh7 */
/*  input         unsigned int uisp_hcds_coefh7  11 bits */
static __inline td_void isp_hcds_coefh7_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh7)
{
    u_isp_hcds_coefh3 o_isp_hcds_coefh3;
    o_isp_hcds_coefh3.u32 = be_reg->isp_hcds_coefh3.u32;
    o_isp_hcds_coefh3.bits.isp_hcds_coefh7 = uisp_hcds_coefh7;
    be_reg->isp_hcds_coefh3.u32 = o_isp_hcds_coefh3.u32;
}
/*  description   set the value of the member isp_hcds_coefh3.isp_hcds_coefh6 */
/*  input         unsigned int uisp_hcds_coefh6  11 bits */
static __inline td_void isp_hcds_coefh6_write(isp_be_reg_type *be_reg, td_u32 uisp_hcds_coefh6)
{
    u_isp_hcds_coefh3 o_isp_hcds_coefh3;
    o_isp_hcds_coefh3.u32 = be_reg->isp_hcds_coefh3.u32;
    o_isp_hcds_coefh3.bits.isp_hcds_coefh6 = uisp_hcds_coefh6;
    be_reg->isp_hcds_coefh3.u32 = o_isp_hcds_coefh3.u32;
}
/*  description   set the value of the member isp_4dg_blc_cfg.isp_4dg_en_in */
/*  input         unsigned int uisp_4dg_en_in  1 bits */
static __inline td_void isp_4dg_en_in_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg_en_in)
{
    u_isp_4dg_blc_cfg o_isp_4dg_blc_cfg;
    o_isp_4dg_blc_cfg.u32 = be_reg->isp_4dg_blc_cfg.u32;
    o_isp_4dg_blc_cfg.bits.isp_4dg_en_in = uisp_4dg_en_in;
    be_reg->isp_4dg_blc_cfg.u32 = o_isp_4dg_blc_cfg.u32;
}
/*  description   set the value of the member isp_4dg_blc_cfg.isp_4dg_en_out */
/*  input         unsigned int uisp_4dg_en_out  1 bits */
static __inline td_void isp_4dg_en_out_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg_en_out)
{
    u_isp_4dg_blc_cfg o_isp_4dg_blc_cfg;
    o_isp_4dg_blc_cfg.u32 = be_reg->isp_4dg_blc_cfg.u32;
    o_isp_4dg_blc_cfg.bits.isp_4dg_en_out = uisp_4dg_en_out;
    be_reg->isp_4dg_blc_cfg.u32 = o_isp_4dg_blc_cfg.u32;
}
/*  description   set the value of the member isp_4dg_0_gain1.isp_4dg0_grgain */
/*  input         unsigned int uisp_4dg0_grgain  16 bits */
static __inline td_void isp_4dg0_grgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_grgain)
{
    u_isp_4dg_0_gain1 o_isp_4dg_0_gain1;
    o_isp_4dg_0_gain1.u32 = be_reg->isp_4dg_0_gain1.u32;
    o_isp_4dg_0_gain1.bits.isp_4dg0_grgain = uisp_4dg0_grgain;
    be_reg->isp_4dg_0_gain1.u32 = o_isp_4dg_0_gain1.u32;
}
/*  description   set the value of the member isp_4dg_0_gain1.isp_4dg0_rgain */
/*  input         unsigned int uisp_4dg0_rgain  16 bits */
static __inline td_void isp_4dg0_rgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_rgain)
{
    u_isp_4dg_0_gain1 o_isp_4dg_0_gain1;
    o_isp_4dg_0_gain1.u32 = be_reg->isp_4dg_0_gain1.u32;
    o_isp_4dg_0_gain1.bits.isp_4dg0_rgain = uisp_4dg0_rgain;
    be_reg->isp_4dg_0_gain1.u32 = o_isp_4dg_0_gain1.u32;
}
/*  description   set the value of the member isp_4dg_0_gain2.isp_4dg0_gbgain */
/*  input         unsigned int uisp_4dg0_gbgain  16 bits */
static __inline td_void isp_4dg0_gbgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_gbgain)
{
    u_isp_4dg_0_gain2 o_isp_4dg_0_gain2;
    o_isp_4dg_0_gain2.u32 = be_reg->isp_4dg_0_gain2.u32;
    o_isp_4dg_0_gain2.bits.isp_4dg0_gbgain = uisp_4dg0_gbgain;
    be_reg->isp_4dg_0_gain2.u32 = o_isp_4dg_0_gain2.u32;
}
/*  description   set the value of the member isp_4dg_0_gain2.isp_4dg0_bgain */
/*  input         unsigned int uisp_4dg0_bgain  16 bits */
static __inline td_void isp_4dg0_bgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_bgain)
{
    u_isp_4dg_0_gain2 o_isp_4dg_0_gain2;
    o_isp_4dg_0_gain2.u32 = be_reg->isp_4dg_0_gain2.u32;
    o_isp_4dg_0_gain2.bits.isp_4dg0_bgain = uisp_4dg0_bgain;
    be_reg->isp_4dg_0_gain2.u32 = o_isp_4dg_0_gain2.u32;
}
/*  description   set the value of the member isp_4dg_0_blc_offset1.isp_4dg0_ofsgr */
/*  input         unsigned int uisp_4dg0_ofsgr  15 bits */
static __inline td_void isp_4dg0_ofsgr_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_ofsgr)
{
    u_isp_4dg_0_blc_offset1 o_isp_4dg_0_blc_offset1;
    o_isp_4dg_0_blc_offset1.u32 = be_reg->isp_4dg_0_blc_offset1.u32;
    o_isp_4dg_0_blc_offset1.bits.isp_4dg0_ofsgr = uisp_4dg0_ofsgr;
    be_reg->isp_4dg_0_blc_offset1.u32 = o_isp_4dg_0_blc_offset1.u32;
}
/*  description   set the value of the member isp_4dg_0_blc_offset1.isp_4dg0_ofsr */
/*  input         unsigned int uisp_4dg0_ofsr  15 bits */
static __inline td_void isp_4dg0_ofsr_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_ofsr)
{
    u_isp_4dg_0_blc_offset1 o_isp_4dg_0_blc_offset1;
    o_isp_4dg_0_blc_offset1.u32 = be_reg->isp_4dg_0_blc_offset1.u32;
    o_isp_4dg_0_blc_offset1.bits.isp_4dg0_ofsr = uisp_4dg0_ofsr;
    be_reg->isp_4dg_0_blc_offset1.u32 = o_isp_4dg_0_blc_offset1.u32;
}
/*  description   set the value of the member isp_4dg_0_blc_offset2.isp_4dg0_ofsgb */
/*  input         unsigned int uisp_4dg0_ofsgb  15 bits */
static __inline td_void isp_4dg0_ofsgb_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_ofsgb)
{
    u_isp_4dg_0_blc_offset2 o_isp_4dg_0_blc_offset2;
    o_isp_4dg_0_blc_offset2.u32 = be_reg->isp_4dg_0_blc_offset2.u32;
    o_isp_4dg_0_blc_offset2.bits.isp_4dg0_ofsgb = uisp_4dg0_ofsgb;
    be_reg->isp_4dg_0_blc_offset2.u32 = o_isp_4dg_0_blc_offset2.u32;
}
/*  description   set the value of the member isp_4dg_0_blc_offset2.isp_4dg0_ofsb */
/*  input         unsigned int uisp_4dg0_ofsb  15 bits */
static __inline td_void isp_4dg0_ofsb_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_ofsb)
{
    u_isp_4dg_0_blc_offset2 o_isp_4dg_0_blc_offset2;
    o_isp_4dg_0_blc_offset2.u32 = be_reg->isp_4dg_0_blc_offset2.u32;
    o_isp_4dg_0_blc_offset2.bits.isp_4dg0_ofsb = uisp_4dg0_ofsb;
    be_reg->isp_4dg_0_blc_offset2.u32 = o_isp_4dg_0_blc_offset2.u32;
}
/*  description   set the value of the member isp_4dg_1_gain1.isp_4dg1_grgain */
/*  input         unsigned int uisp_4dg1_grgain  16 bits */
static __inline td_void isp_4dg1_grgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_grgain)
{
    u_isp_4dg_1_gain1 o_isp_4dg_1_gain1;
    o_isp_4dg_1_gain1.u32 = be_reg->isp_4dg_1_gain1.u32;
    o_isp_4dg_1_gain1.bits.isp_4dg1_grgain = uisp_4dg1_grgain;
    be_reg->isp_4dg_1_gain1.u32 = o_isp_4dg_1_gain1.u32;
}
/*  description   set the value of the member isp_4dg_1_gain1.isp_4dg1_rgain */
/*  input         unsigned int uisp_4dg1_rgain  16 bits */
static __inline td_void isp_4dg1_rgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_rgain)
{
    u_isp_4dg_1_gain1 o_isp_4dg_1_gain1;
    o_isp_4dg_1_gain1.u32 = be_reg->isp_4dg_1_gain1.u32;
    o_isp_4dg_1_gain1.bits.isp_4dg1_rgain = uisp_4dg1_rgain;
    be_reg->isp_4dg_1_gain1.u32 = o_isp_4dg_1_gain1.u32;
}
/*  description   set the value of the member isp_4dg_1_gain2.isp_4dg1_gbgain */
/*  input         unsigned int uisp_4dg1_gbgain  16 bits */
static __inline td_void isp_4dg1_gbgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_gbgain)
{
    u_isp_4dg_1_gain2 o_isp_4dg_1_gain2;
    o_isp_4dg_1_gain2.u32 = be_reg->isp_4dg_1_gain2.u32;
    o_isp_4dg_1_gain2.bits.isp_4dg1_gbgain = uisp_4dg1_gbgain;
    be_reg->isp_4dg_1_gain2.u32 = o_isp_4dg_1_gain2.u32;
}
/*  description   set the value of the member isp_4dg_1_gain2.isp_4dg1_bgain */
/*  input         unsigned int uisp_4dg1_bgain  16 bits */
static __inline td_void isp_4dg1_bgain_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_bgain)
{
    u_isp_4dg_1_gain2 o_isp_4dg_1_gain2;
    o_isp_4dg_1_gain2.u32 = be_reg->isp_4dg_1_gain2.u32;
    o_isp_4dg_1_gain2.bits.isp_4dg1_bgain = uisp_4dg1_bgain;
    be_reg->isp_4dg_1_gain2.u32 = o_isp_4dg_1_gain2.u32;
}
/*  description   set the value of the member isp_4dg_1_blc_offset1.isp_4dg1_ofsgr */
/*  input         unsigned int uisp_4dg1_ofsgr  15 bits */
static __inline td_void isp_4dg1_ofsgr_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_ofsgr)
{
    u_isp_4dg_1_blc_offset1 o_isp_4dg_1_blc_offset1;
    o_isp_4dg_1_blc_offset1.u32 = be_reg->isp_4dg_1_blc_offset1.u32;
    o_isp_4dg_1_blc_offset1.bits.isp_4dg1_ofsgr = uisp_4dg1_ofsgr;
    be_reg->isp_4dg_1_blc_offset1.u32 = o_isp_4dg_1_blc_offset1.u32;
}
/*  description   set the value of the member isp_4dg_1_blc_offset1.isp_4dg1_ofsr */
/*  input         unsigned int uisp_4dg1_ofsr  15 bits */
static __inline td_void isp_4dg1_ofsr_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_ofsr)
{
    u_isp_4dg_1_blc_offset1 o_isp_4dg_1_blc_offset1;
    o_isp_4dg_1_blc_offset1.u32 = be_reg->isp_4dg_1_blc_offset1.u32;
    o_isp_4dg_1_blc_offset1.bits.isp_4dg1_ofsr = uisp_4dg1_ofsr;
    be_reg->isp_4dg_1_blc_offset1.u32 = o_isp_4dg_1_blc_offset1.u32;
}
/*  description   set the value of the member isp_4dg_1_blc_offset2.isp_4dg1_ofsgb */
/*  input         unsigned int uisp_4dg1_ofsgb  15 bits */
static __inline td_void isp_4dg1_ofsgb_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_ofsgb)
{
    u_isp_4dg_1_blc_offset2 o_isp_4dg_1_blc_offset2;
    o_isp_4dg_1_blc_offset2.u32 = be_reg->isp_4dg_1_blc_offset2.u32;
    o_isp_4dg_1_blc_offset2.bits.isp_4dg1_ofsgb = uisp_4dg1_ofsgb;
    be_reg->isp_4dg_1_blc_offset2.u32 = o_isp_4dg_1_blc_offset2.u32;
}
/*  description   set the value of the member isp_4dg_1_blc_offset2.isp_4dg1_ofsb */
/*  input         unsigned int uisp_4dg1_ofsb  15 bits */
static __inline td_void isp_4dg1_ofsb_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_ofsb)
{
    u_isp_4dg_1_blc_offset2 o_isp_4dg_1_blc_offset2;
    o_isp_4dg_1_blc_offset2.u32 = be_reg->isp_4dg_1_blc_offset2.u32;
    o_isp_4dg_1_blc_offset2.bits.isp_4dg1_ofsb = uisp_4dg1_ofsb;
    be_reg->isp_4dg_1_blc_offset2.u32 = o_isp_4dg_1_blc_offset2.u32;
}

/*  description   set the value of the member isp_4dg_0_clip_value.isp_4dg0_clip_value */
/*  input         unsigned int uisp_4dg0_clip_value  20 bits */
static __inline td_void isp_4dg0_clip_value_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg0_clip_value)
{
    u_isp_4dg_0_clip_value o_isp_4dg_0_clip_value;
    o_isp_4dg_0_clip_value.u32 = be_reg->isp_4dg_0_clip_value.u32;
    o_isp_4dg_0_clip_value.bits.isp_4dg0_clip_value = uisp_4dg0_clip_value;
    be_reg->isp_4dg_0_clip_value.u32 = o_isp_4dg_0_clip_value.u32;
}
/*  description   set the value of the member isp_4dg_1_clip_value.isp_4dg1_clip_value */
/*  input         unsigned int uisp_4dg1_clip_value  20 bits */
static __inline td_void isp_4dg1_clip_value_write(isp_be_reg_type *be_reg, td_u32 uisp_4dg1_clip_value)
{
    u_isp_4dg_1_clip_value o_isp_4dg_1_clip_value;
    o_isp_4dg_1_clip_value.u32 = be_reg->isp_4dg_1_clip_value.u32;
    o_isp_4dg_1_clip_value.bits.isp_4dg1_clip_value = uisp_4dg1_clip_value;
    be_reg->isp_4dg_1_clip_value.u32 = o_isp_4dg_1_clip_value.u32;
}

/*  description   set the value of the member isp_ae_zone.isp_ae_hnum */
/*  input         unsigned int uisp_ae_hnum  5 bits */
static __inline td_void isp_ae_hnum_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_hnum)
{
    u_isp_ae_zone o_isp_ae_zone;
    o_isp_ae_zone.u32 = be_reg->isp_ae_zone.u32;
    o_isp_ae_zone.bits.isp_ae_hnum = uisp_ae_hnum;
    be_reg->isp_ae_zone.u32 = o_isp_ae_zone.u32;
}
/*  description   set the value of the member isp_ae_zone.isp_ae_vnum */
/*  input         unsigned int uisp_ae_vnum  5 bits */
static __inline td_void isp_ae_vnum_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_vnum)
{
    u_isp_ae_zone o_isp_ae_zone;
    o_isp_ae_zone.u32 = be_reg->isp_ae_zone.u32;
    o_isp_ae_zone.bits.isp_ae_vnum = uisp_ae_vnum;
    be_reg->isp_ae_zone.u32 = o_isp_ae_zone.u32;
}
/*  description   set the value of the member isp_ae_skip_crg.isp_ae_skip_x */
/*  input         unsigned int uisp_ae_skip_x  3 bits */
static __inline td_void isp_ae_skip_x_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_skip_x)
{
    u_isp_ae_skip_crg o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = be_reg->isp_ae_skip_crg.u32;
    o_isp_ae_skip_crg.bits.isp_ae_skip_x = uisp_ae_skip_x;
    be_reg->isp_ae_skip_crg.u32 = o_isp_ae_skip_crg.u32;
}
/*  description   set the value of the member isp_ae_skip_crg.isp_ae_offset_x */
/*  input         unsigned int uisp_ae_offset_x  1 bits */
static __inline td_void isp_ae_offset_x_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_offset_x)
{
    u_isp_ae_skip_crg o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = be_reg->isp_ae_skip_crg.u32;
    o_isp_ae_skip_crg.bits.isp_ae_offset_x = uisp_ae_offset_x;
    be_reg->isp_ae_skip_crg.u32 = o_isp_ae_skip_crg.u32;
}
/*  description   set the value of the member isp_ae_skip_crg.isp_ae_skip_y */
/*  input         unsigned int uisp_ae_skip_y  3 bits */
static __inline td_void isp_ae_skip_y_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_skip_y)
{
    u_isp_ae_skip_crg o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = be_reg->isp_ae_skip_crg.u32;
    o_isp_ae_skip_crg.bits.isp_ae_skip_y = uisp_ae_skip_y;
    be_reg->isp_ae_skip_crg.u32 = o_isp_ae_skip_crg.u32;
}
/*  description   set the value of the member isp_ae_skip_crg.isp_ae_offset_y */
/*  input         unsigned int uisp_ae_offset_y  1 bits */
static __inline td_void isp_ae_offset_y_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_offset_y)
{
    u_isp_ae_skip_crg o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.u32 = be_reg->isp_ae_skip_crg.u32;
    o_isp_ae_skip_crg.bits.isp_ae_offset_y = uisp_ae_offset_y;
    be_reg->isp_ae_skip_crg.u32 = o_isp_ae_skip_crg.u32;
}
/*  description   set the value of the member isp_ae_bitmove.isp_ae_bitmove */
/*  input         unsigned int uisp_ae_bitmove  5 bits */
static __inline td_void isp_ae_bitmove_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_bitmove)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = be_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_bitmove = uisp_ae_bitmove;
    be_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_hist_gamma_mode */
/*  input       : unsigned int uisp_ae_hist_gamma_mode: 2 bits */
static __inline td_void isp_ae_hist_gamma_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_hist_gamma_mode)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = be_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_hist_gamma_mode = uisp_ae_hist_gamma_mode;
    be_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_aver_gamma_mode */
/*  input       : unsigned int uisp_ae_aver_gamma_mode: 2 bits */
static __inline td_void isp_ae_aver_gamma_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_aver_gamma_mode)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = be_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_aver_gamma_mode = uisp_ae_aver_gamma_mode;
    be_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}
/*  description   set the value of the member isp_ae_bitmove.isp_ae_blc_en */
/*  input         unsigned int uisp_ae_blc_en  1 bits */
static __inline td_void isp_ae_blc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_blc_en)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = be_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_blc_en = uisp_ae_blc_en;
    be_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_gamma_limit */
/*  input       : unsigned int uisp_ae_gamma_limit: 4 bits */
static __inline td_void isp_ae_gamma_limit_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_gamma_limit)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = be_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_gamma_limit = uisp_ae_gamma_limit;
    be_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}

/*  description : set the value of the member isp_ae_bitmove.isp_ae_fourplanemode */
/*  input       : unsigned int uisp_ae_fourplanemode: 1 bits */
static __inline td_void isp_ae_fourplanemode_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_fourplanemode)
{
    u_isp_ae_bitmove o_isp_ae_bitmove;
    o_isp_ae_bitmove.u32 = be_reg->isp_ae_bitmove.u32;
    o_isp_ae_bitmove.bits.isp_ae_fourplanemode = uisp_ae_fourplanemode;
    be_reg->isp_ae_bitmove.u32 = o_isp_ae_bitmove.u32;
}
/*  description   set the value of the member isp_ae_offset_r_gr.isp_ae_offset_r */
/*  input         unsigned int uisp_ae_offset_r  15 bits */
static __inline td_void isp_ae_offset_r_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_offset_r)
{
    u_isp_ae_offset_r_gr o_isp_ae_offset_r_gr;
    o_isp_ae_offset_r_gr.u32 = be_reg->isp_ae_offset_r_gr.u32;
    o_isp_ae_offset_r_gr.bits.isp_ae_offset_r = uisp_ae_offset_r;
    be_reg->isp_ae_offset_r_gr.u32 = o_isp_ae_offset_r_gr.u32;
}
/*  description   set the value of the member isp_ae_offset_r_gr.isp_ae_offset_gr */
/*  input         unsigned int uisp_ae_offset_gr  15 bits */
static __inline td_void isp_ae_offset_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_offset_gr)
{
    u_isp_ae_offset_r_gr o_isp_ae_offset_r_gr;
    o_isp_ae_offset_r_gr.u32 = be_reg->isp_ae_offset_r_gr.u32;
    o_isp_ae_offset_r_gr.bits.isp_ae_offset_gr = uisp_ae_offset_gr;
    be_reg->isp_ae_offset_r_gr.u32 = o_isp_ae_offset_r_gr.u32;
}
/*  description   set the value of the member isp_ae_offset_gb_b.isp_ae_offset_gb */
/*  input         unsigned int uisp_ae_offset_gb  15 bits */
static __inline td_void isp_ae_offset_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_offset_gb)
{
    u_isp_ae_offset_gb_b o_isp_ae_offset_gb_b;
    o_isp_ae_offset_gb_b.u32 = be_reg->isp_ae_offset_gb_b.u32;
    o_isp_ae_offset_gb_b.bits.isp_ae_offset_gb = uisp_ae_offset_gb;
    be_reg->isp_ae_offset_gb_b.u32 = o_isp_ae_offset_gb_b.u32;
}
/*  description   set the value of the member isp_ae_offset_gb_b.isp_ae_offset_b */
/*  input         unsigned int uisp_ae_offset_b  15 bits */
static __inline td_void isp_ae_offset_b_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_offset_b)
{
    u_isp_ae_offset_gb_b o_isp_ae_offset_gb_b;
    o_isp_ae_offset_gb_b.u32 = be_reg->isp_ae_offset_gb_b.u32;
    o_isp_ae_offset_gb_b.bits.isp_ae_offset_b = uisp_ae_offset_b;
    be_reg->isp_ae_offset_gb_b.u32 = o_isp_ae_offset_gb_b.u32;
}
/*  description   set the value of the member isp_ae_lut_update.isp_ae_lut_update */
/*  input         unsigned int uisp_ae_lut_update  1 bits */
static __inline td_void isp_ae_lut_update_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_lut_update)
{
    u_isp_ae_lut_update o_isp_ae_lut_update;
    o_isp_ae_lut_update.u32 = be_reg->isp_ae_lut_update.u32;
    o_isp_ae_lut_update.bits.isp_ae_lut_update = uisp_ae_lut_update;
    be_reg->isp_ae_lut_update.u32 = o_isp_ae_lut_update.u32;
}
/*  description   set the value of the member isp_ae_stt_bst.isp_ae_stt_size */
/*  input         unsigned int uisp_ae_stt_size  16 bits */
static __inline td_void isp_ae_stt_size_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_stt_size)
{
    u_isp_ae_stt_bst o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = be_reg->isp_ae_stt_bst.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_size = uisp_ae_stt_size;
    be_reg->isp_ae_stt_bst.u32 = o_isp_ae_stt_bst.u32;
}
/*  description   set the value of the member isp_ae_stt_bst.isp_ae_stt_bst */
/*  input         unsigned int uisp_ae_stt_bst  4 bits */
static __inline td_void isp_ae_stt_bst_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_stt_bst)
{
    u_isp_ae_stt_bst o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = be_reg->isp_ae_stt_bst.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_bst = uisp_ae_stt_bst;
    be_reg->isp_ae_stt_bst.u32 = o_isp_ae_stt_bst.u32;
}
/*  description   set the value of the member isp_ae_stt_bst.isp_ae_stt_en */
/*  input         unsigned int uisp_ae_stt_en  1 bits */
static __inline td_void isp_ae_stt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_stt_en)
{
    u_isp_ae_stt_bst o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.u32 = be_reg->isp_ae_stt_bst.u32;
    o_isp_ae_stt_bst.bits.isp_ae_stt_en = uisp_ae_stt_en;
    be_reg->isp_ae_stt_bst.u32 = o_isp_ae_stt_bst.u32;
}
/*  description   set the value of the member isp_ae_crop_pos.isp_ae_crop_pos_x */
/*  input         unsigned int uisp_ae_crop_pos_x  16 bits */
static __inline td_void isp_ae_crop_pos_x_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_crop_pos_x)
{
    u_isp_ae_crop_pos o_isp_ae_crop_pos;
    o_isp_ae_crop_pos.u32 = be_reg->isp_ae_crop_pos.u32;
    o_isp_ae_crop_pos.bits.isp_ae_crop_pos_x = uisp_ae_crop_pos_x;
    be_reg->isp_ae_crop_pos.u32 = o_isp_ae_crop_pos.u32;
}
/*  description   set the value of the member isp_ae_crop_pos.isp_ae_crop_pos_y */
/*  input         unsigned int uisp_ae_crop_pos_y  16 bits */
static __inline td_void isp_ae_crop_pos_y_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_crop_pos_y)
{
    u_isp_ae_crop_pos o_isp_ae_crop_pos;
    o_isp_ae_crop_pos.u32 = be_reg->isp_ae_crop_pos.u32;
    o_isp_ae_crop_pos.bits.isp_ae_crop_pos_y = uisp_ae_crop_pos_y;
    be_reg->isp_ae_crop_pos.u32 = o_isp_ae_crop_pos.u32;
}

/*  description : set the value of the member isp_ae_crop_outsize.isp_ae_crop_out_width */
/*  input       : unsigned int uisp_ae_crop_out_width: 16 bits */
static __inline td_void isp_ae_crop_out_width_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_crop_out_width)
{
    u_isp_ae_crop_outsize o_isp_ae_crop_outsize;
    o_isp_ae_crop_outsize.u32 = be_reg->isp_ae_crop_outsize.u32;
    o_isp_ae_crop_outsize.bits.isp_ae_crop_out_width = uisp_ae_crop_out_width;
    be_reg->isp_ae_crop_outsize.u32 = o_isp_ae_crop_outsize.u32;
}

/*  description : set the value of the member isp_ae_crop_outsize.isp_ae_crop_out_height */
/*  input       : unsigned int uisp_ae_crop_out_height: 16 bits */
static __inline td_void isp_ae_crop_out_height_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_crop_out_height)
{
    u_isp_ae_crop_outsize o_isp_ae_crop_outsize;
    o_isp_ae_crop_outsize.u32 = be_reg->isp_ae_crop_outsize.u32;
    o_isp_ae_crop_outsize.bits.isp_ae_crop_out_height = uisp_ae_crop_out_height;
    be_reg->isp_ae_crop_outsize.u32 = o_isp_ae_crop_outsize.u32;
}
/*  description   set the value of the member isp_ae_hist_raddr.isp_ae_hist_raddr */
/*  input         unsigned int uisp_ae_hist_raddr  32 bits */
static __inline td_void isp_ae_hist_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_hist_raddr)
{
    be_reg->isp_ae_hist_raddr.u32 = uisp_ae_hist_raddr;
}


/*  description : set the value of the member isp_ae_aver_r_gr_raddr.isp_ae_aver_r_gr_raddr */
/*  input       : unsigned int uisp_ae_aver_r_gr_raddr: 32 bits */
static __inline td_void isp_ae_aver_r_gr_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_aver_r_gr_raddr)
{
    be_reg->isp_ae_aver_r_gr_raddr.u32 = uisp_ae_aver_r_gr_raddr;
}
/*  description : set the value of the member isp_ae_aver_gb_b_raddr.isp_ae_aver_gb_b_raddr */
/*  input       : unsigned int uisp_ae_aver_gb_b_raddr: 32 bits */
static __inline td_void isp_ae_aver_gb_b_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_aver_gb_b_raddr)
{
    be_reg->isp_ae_aver_gb_b_raddr.u32 = uisp_ae_aver_gb_b_raddr;
}
#define OT_ISP_AE_WEI_COMBIN_COUNT                    4
/*  description   set the value of the member isp_ae_weight_waddr.isp_ae_wei_waddr */
/*  input         unsigned int uisp_ae_wei_waddr  32 bits */
static __inline td_void isp_ae_wei_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_wei_waddr)
{
    be_reg->isp_ae_weight_waddr.u32 = uisp_ae_wei_waddr;
}
/*  description   set the value of the member isp_ae_weight_wdata.isp_ae_wei_wdata */
/*  input         unsigned int uisp_ae_wei_wdata  32 bits */
static __inline td_void isp_ae_wei_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_wei_wdata)
{
    be_reg->isp_ae_weight_wdata.u32 = uisp_ae_wei_wdata;
}

/*  description   set the value of the member isp_ae_weight_raddr.isp_ae_wei_raddr */
/*  input         unsigned int uisp_ae_wei_raddr  32 bits */
static __inline td_void isp_ae_wei_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ae_wei_raddr)
{
    be_reg->isp_ae_weight_raddr.u32 = uisp_ae_wei_raddr;
}
/*  description   get the value of the member isp_ae_weight_rdata.isp_ae_wei_rdata */
/*  input         unsigned int uisp_ae_wei_rdata  32 bits */
static __inline td_u32 isp_ae_wei_rdata_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_ae_weight_rdata.u32;
}

/*  description   set the value of the member isp_awb_zone.isp_awb_hnum */
/*  input         unsigned int uisp_awb_hnum  6 bits */
static __inline td_void isp_awb_hnum_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_hnum)
{
    u_isp_awb_zone o_isp_awb_zone;
    o_isp_awb_zone.u32 = be_reg->isp_awb_zone.u32;
    o_isp_awb_zone.bits.isp_awb_hnum = uisp_awb_hnum;
    be_reg->isp_awb_zone.u32 = o_isp_awb_zone.u32;
}
/*  description   set the value of the member isp_awb_zone.isp_awb_vnum */
/*  input         unsigned int uisp_awb_vnum  6 bits */
static __inline td_void isp_awb_vnum_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_vnum)
{
    u_isp_awb_zone o_isp_awb_zone;
    o_isp_awb_zone.u32 = be_reg->isp_awb_zone.u32;
    o_isp_awb_zone.bits.isp_awb_vnum = uisp_awb_vnum;
    be_reg->isp_awb_zone.u32 = o_isp_awb_zone.u32;
}
/*  description   set the value of the member isp_awb_ctrl.isp_awb_bitmove */
/*  input         unsigned int uisp_awb_bitmove  3 bits */
static __inline td_void isp_awb_bitmove_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_bitmove)
{
    u_isp_awb_bitmove o_isp_awb_bitmove;
    o_isp_awb_bitmove.u32 = be_reg->isp_awb_bitmove.u32;
    o_isp_awb_bitmove.bits.isp_awb_bitmove = uisp_awb_bitmove;
    be_reg->isp_awb_bitmove.u32 = o_isp_awb_bitmove.u32;
}

/*  description   set the value of the member isp_awb_thd_min.isp_awb_threshold_min */
/*  input         unsigned int uisp_awb_threshold_min  16 bits */
static __inline td_void isp_awb_threshold_min_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_threshold_min)
{
    u_isp_awb_thd_min o_isp_awb_thd_min;
    o_isp_awb_thd_min.u32 = be_reg->isp_awb_thd_min.u32;
    o_isp_awb_thd_min.bits.isp_awb_threshold_min = uisp_awb_threshold_min;
    be_reg->isp_awb_thd_min.u32 = o_isp_awb_thd_min.u32;
}
/*  description   set the value of the member isp_awb_thd_max.isp_awb_threshold_max */
/*  input         unsigned int uisp_awb_threshold_max  16 bits */
static __inline td_void isp_awb_threshold_max_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_threshold_max)
{
    u_isp_awb_thd_max o_isp_awb_thd_max;
    o_isp_awb_thd_max.u32 = be_reg->isp_awb_thd_max.u32;
    o_isp_awb_thd_max.bits.isp_awb_threshold_max = uisp_awb_threshold_max;
    be_reg->isp_awb_thd_max.u32 = o_isp_awb_thd_max.u32;
}
/*  description   set the value of the member isp_awb_cr_mm.isp_awb_cr_ref_max */
/*  input         unsigned int uisp_awb_cr_ref_max  12 bits */
static __inline td_void isp_awb_cr_ref_max_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_cr_ref_max)
{
    u_isp_awb_cr_mm o_isp_awb_cr_mm;
    o_isp_awb_cr_mm.u32 = be_reg->isp_awb_cr_mm.u32;
    o_isp_awb_cr_mm.bits.isp_awb_cr_ref_max = uisp_awb_cr_ref_max;
    be_reg->isp_awb_cr_mm.u32 = o_isp_awb_cr_mm.u32;
}
/*  description   set the value of the member isp_awb_cr_mm.isp_awb_cr_ref_min */
/*  input         unsigned int uisp_awb_cr_ref_min  12 bits */
static __inline td_void isp_awb_cr_ref_min_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_cr_ref_min)
{
    u_isp_awb_cr_mm o_isp_awb_cr_mm;
    o_isp_awb_cr_mm.u32 = be_reg->isp_awb_cr_mm.u32;
    o_isp_awb_cr_mm.bits.isp_awb_cr_ref_min = uisp_awb_cr_ref_min;
    be_reg->isp_awb_cr_mm.u32 = o_isp_awb_cr_mm.u32;
}
/*  description   set the value of the member isp_awb_cb_mm.isp_awb_cb_ref_max */
/*  input         unsigned int uisp_awb_cb_ref_max  12 bits */
static __inline td_void isp_awb_cb_ref_max_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_cb_ref_max)
{
    u_isp_awb_cb_mm o_isp_awb_cb_mm;
    o_isp_awb_cb_mm.u32 = be_reg->isp_awb_cb_mm.u32;
    o_isp_awb_cb_mm.bits.isp_awb_cb_ref_max = uisp_awb_cb_ref_max;
    be_reg->isp_awb_cb_mm.u32 = o_isp_awb_cb_mm.u32;
}
/*  description   set the value of the member isp_awb_cb_mm.isp_awb_cb_ref_min */
/*  input         unsigned int uisp_awb_cb_ref_min  12 bits */
static __inline td_void isp_awb_cb_ref_min_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_cb_ref_min)
{
    u_isp_awb_cb_mm o_isp_awb_cb_mm;
    o_isp_awb_cb_mm.u32 = be_reg->isp_awb_cb_mm.u32;
    o_isp_awb_cb_mm.bits.isp_awb_cb_ref_min = uisp_awb_cb_ref_min;
    be_reg->isp_awb_cb_mm.u32 = o_isp_awb_cb_mm.u32;
}
/*  description   set the value of the member isp_awb_offset_comp.isp_awb_offset_comp */
/*  input         unsigned int uisp_awb_offset_comp  16 bits */
static __inline td_void isp_awb_offset_comp_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_offset_comp)
{
    u_isp_awb_offset_comp o_isp_awb_offset_comp;
    o_isp_awb_offset_comp.u32 = be_reg->isp_awb_offset_comp.u32;
    o_isp_awb_offset_comp.bits.isp_awb_offset_comp = uisp_awb_offset_comp;
    be_reg->isp_awb_offset_comp.u32 = o_isp_awb_offset_comp.u32;
}
/*  description   set the value of the member isp_awb_stt_bst.isp_awb_stt_size */
/*  input         unsigned int uisp_awb_stt_size  16 bits */
static __inline td_void isp_awb_stt_size_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_stt_size)
{
    u_isp_awb_stt_bst o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.u32 = be_reg->isp_awb_stt_bst.u32;
    o_isp_awb_stt_bst.bits.isp_awb_stt_size = uisp_awb_stt_size;
    be_reg->isp_awb_stt_bst.u32 = o_isp_awb_stt_bst.u32;
}
/*  description   set the value of the member isp_awb_stt_bst.isp_awb_stt_bst */
/*  input         unsigned int uisp_awb_stt_bst  4 bits */
static __inline td_void isp_awb_stt_bst_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_stt_bst)
{
    u_isp_awb_stt_bst o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.u32 = be_reg->isp_awb_stt_bst.u32;
    o_isp_awb_stt_bst.bits.isp_awb_stt_bst = uisp_awb_stt_bst;
    be_reg->isp_awb_stt_bst.u32 = o_isp_awb_stt_bst.u32;
}
/*  description   set the value of the member isp_awb_stt_bst.isp_awb_stt_en */
/*  input         unsigned int uisp_awb_stt_en  1 bits */
static __inline td_void isp_awb_stt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_stt_en)
{
    u_isp_awb_stt_bst o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.u32 = be_reg->isp_awb_stt_bst.u32;
    o_isp_awb_stt_bst.bits.isp_awb_stt_en = uisp_awb_stt_en;
    be_reg->isp_awb_stt_bst.u32 = o_isp_awb_stt_bst.u32;
}
/*  description   set the value of the member isp_awb_stt_abn.isp_awb_stt_clr */
/*  input         unsigned int uisp_awb_stt_clr  1 bits */
static __inline td_void isp_awb_stt_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_stt_clr)
{
    u_isp_awb_stt_abn o_isp_awb_stt_abn;
    o_isp_awb_stt_abn.u32 = be_reg->isp_awb_stt_abn.u32;
    o_isp_awb_stt_abn.bits.isp_awb_stt_clr = uisp_awb_stt_clr;
    be_reg->isp_awb_stt_abn.u32 = o_isp_awb_stt_abn.u32;
}
/*  description   set the value of the member isp_awb_crop_pos.isp_awb_crop_pos_x */
/*  input         unsigned int uisp_awb_crop_pos_x  16 bits */
static __inline td_void isp_awb_crop_pos_x_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_crop_pos_x)
{
    u_isp_awb_crop_pos o_isp_awb_crop_pos;
    o_isp_awb_crop_pos.u32 = be_reg->isp_awb_crop_pos.u32;
    o_isp_awb_crop_pos.bits.isp_awb_crop_pos_x = uisp_awb_crop_pos_x;
    be_reg->isp_awb_crop_pos.u32 = o_isp_awb_crop_pos.u32;
}
/*  description   set the value of the member isp_awb_crop_pos.isp_awb_crop_pos_y */
/*  input         unsigned int uisp_awb_crop_pos_y  16 bits */
static __inline td_void isp_awb_crop_pos_y_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_crop_pos_y)
{
    u_isp_awb_crop_pos o_isp_awb_crop_pos;
    o_isp_awb_crop_pos.u32 = be_reg->isp_awb_crop_pos.u32;
    o_isp_awb_crop_pos.bits.isp_awb_crop_pos_y = uisp_awb_crop_pos_y;
    be_reg->isp_awb_crop_pos.u32 = o_isp_awb_crop_pos.u32;
}
/*  description   set the value of the member isp_awb_crop_outsize.isp_awb_crop_out_width */
/*  input         unsigned int uisp_awb_crop_out_width  16 bits */
static __inline td_void isp_awb_crop_out_width_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_crop_out_width)
{
    u_isp_awb_crop_outsize o_isp_awb_crop_outsize;
    o_isp_awb_crop_outsize.u32 = be_reg->isp_awb_crop_outsize.u32;
    o_isp_awb_crop_outsize.bits.isp_awb_crop_out_width = uisp_awb_crop_out_width;
    be_reg->isp_awb_crop_outsize.u32 = o_isp_awb_crop_outsize.u32;
}
/*  description   set the value of the member isp_awb_crop_outsize.isp_awb_crop_out_height */
/*  input         unsigned int uisp_awb_crop_out_height  16 bits */
static __inline td_void isp_awb_crop_out_height_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_crop_out_height)
{
    u_isp_awb_crop_outsize o_isp_awb_crop_outsize;
    o_isp_awb_crop_outsize.u32 = be_reg->isp_awb_crop_outsize.u32;
    o_isp_awb_crop_outsize.bits.isp_awb_crop_out_height = uisp_awb_crop_out_height;
    be_reg->isp_awb_crop_outsize.u32 = o_isp_awb_crop_outsize.u32;
}
/*  description   set the value of the member isp_awb_stat_raddr.isp_awb_stat_raddr */
/*  input         unsigned int uisp_awb_stat_raddr  32 bits */
static __inline td_void isp_awb_stat_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_stat_raddr)
{
    be_reg->isp_awb_stat_raddr.u32 = uisp_awb_stat_raddr;
}

/*  description   set the value of the member isp_af_cfg.isp_af_iir0_en0 */
/*  input         unsigned int uisp_af_iir0_en0  1 bits */
static __inline td_void isp_af_iir0_en0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir0_en0)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir0_en0 = uisp_af_iir0_en0;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir0_en1 */
/*  input         unsigned int uisp_af_iir0_en1  1 bits */
static __inline td_void isp_af_iir0_en1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir0_en1)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir0_en1 = uisp_af_iir0_en1;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir0_en2 */
/*  input         unsigned int uisp_af_iir0_en2  1 bits */
static __inline td_void isp_af_iir0_en2_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir0_en2)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir0_en2 = uisp_af_iir0_en2;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir1_en0 */
/*  input         unsigned int uisp_af_iir1_en0  1 bits */
static __inline td_void isp_af_iir1_en0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir1_en0)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir1_en0 = uisp_af_iir1_en0;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir1_en1 */
/*  input         unsigned int uisp_af_iir1_en1  1 bits */
static __inline td_void isp_af_iir1_en1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir1_en1)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir1_en1 = uisp_af_iir1_en1;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir1_en2 */
/*  input         unsigned int uisp_af_iir1_en2  1 bits */
static __inline td_void isp_af_iir1_en2_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir1_en2)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir1_en2 = uisp_af_iir1_en2;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_peak_mode */
/*  input         unsigned int uisp_af_peak_mode  1 bits */
static __inline td_void isp_af_peak_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_af_peak_mode)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_peak_mode = uisp_af_peak_mode;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_squ_mode */
/*  input         unsigned int uisp_af_squ_mode  1 bits */
static __inline td_void isp_af_squ_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_af_squ_mode)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_squ_mode = uisp_af_squ_mode;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_offset_en */
/*  input         unsigned int uisp_af_offset_en  1 bits */
static __inline td_void isp_af_offset_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_offset_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_offset_en = uisp_af_offset_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_crop_en */
/*  input         unsigned int uisp_af_crop_en  1 bits */
static __inline td_void isp_af_crop_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_crop_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_crop_en = uisp_af_crop_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_lpf_en */
/*  input         unsigned int uisp_af_lpf_en  1 bits */
#define OT_ISP_BE_AF_LPF_EN_DEFAULT                   0
static __inline td_void isp_af_lpf_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_lpf_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_lpf_en = uisp_af_lpf_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_mean_en */
/*  input         unsigned int uisp_af_mean_en  1 bits */
static __inline td_void isp_af_mean_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_mean_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_mean_en = uisp_af_mean_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_raw_mode */
/*  input         unsigned int uisp_af_raw_mode  1 bits */
static __inline td_void isp_af_raw_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_af_raw_mode)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_raw_mode = uisp_af_raw_mode;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_bayer_mode */
/*  input         unsigned int uisp_af_bayer_mode  2 bits */
static __inline td_void isp_af_bayer_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_af_bayer_mode)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_bayer_mode = uisp_af_bayer_mode;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir0_ds_en */
/*  input         unsigned int uisp_af_iir0_ds_en  1 bits */
static __inline td_void isp_af_iir0_ds_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir0_ds_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir0_ds_en = uisp_af_iir0_ds_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir1_ds_en */
/*  input         unsigned int uisp_af_iir1_ds_en  1 bits */
static __inline td_void isp_af_iir1_ds_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir1_ds_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir1_ds_en = uisp_af_iir1_ds_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_fir0_lpf_en */
/*  input         unsigned int uisp_af_fir0_lpf_en  1 bits */
#define OT_ISP_BE_AF_FIR0_LPF_EN_DEFAULT              0x1
static __inline td_void isp_af_fir0_lpf_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir0_lpf_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_fir0_lpf_en = uisp_af_fir0_lpf_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_fir1_lpf_en */
/*  input         unsigned int uisp_af_fir1_lpf_en  1 bits */
#define OT_ISP_BE_AF_FIR1_LPF_EN_DEFAULT              0x1
static __inline td_void isp_af_fir1_lpf_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir1_lpf_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_fir1_lpf_en = uisp_af_fir1_lpf_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir0_ldg_en */
/*  input         unsigned int uisp_af_iir0_ldg_en  1 bits */
static __inline td_void isp_af_iir0_ldg_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir0_ldg_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir0_ldg_en = uisp_af_iir0_ldg_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_iir1_ldg_en */
/*  input         unsigned int uisp_af_iir1_ldg_en  1 bits */
static __inline td_void isp_af_iir1_ldg_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir1_ldg_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_iir1_ldg_en = uisp_af_iir1_ldg_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_fir0_ldg_en */
/*  input         unsigned int uisp_af_fir0_ldg_en  1 bits */
static __inline td_void isp_af_fir0_ldg_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir0_ldg_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_fir0_ldg_en = uisp_af_fir0_ldg_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_fir1_ldg_en */
/*  input         unsigned int uisp_af_fir1_ldg_en  1 bits */
static __inline td_void isp_af_fir1_ldg_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir1_ldg_en)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_fir1_ldg_en = uisp_af_fir1_ldg_en;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_cfg.isp_af_gamma */
/*  input         unsigned int uisp_af_gamma  1 bits */
static __inline td_void isp_af_gamma_write(isp_be_reg_type *be_reg, td_u32 uisp_af_gamma)
{
    u_isp_af_cfg o_isp_af_cfg;
    o_isp_af_cfg.u32 = be_reg->isp_af_cfg.u32;
    o_isp_af_cfg.bits.isp_af_gamma = uisp_af_gamma;
    be_reg->isp_af_cfg.u32 = o_isp_af_cfg.u32;
}
/*  description   set the value of the member isp_af_zone.isp_af_hnum */
/*  input         unsigned int uisp_af_hnum  5 bits */
static __inline td_void isp_af_hnum_write(isp_be_reg_type *be_reg, td_u32 uisp_af_hnum)
{
    u_isp_af_zone o_isp_af_zone;
    o_isp_af_zone.u32 = be_reg->isp_af_zone.u32;
    o_isp_af_zone.bits.isp_af_hnum = uisp_af_hnum;
    be_reg->isp_af_zone.u32 = o_isp_af_zone.u32;
}
/*  description   set the value of the member isp_af_zone.isp_af_vnum */
/*  input         unsigned int uisp_af_vnum  5 bits */
static __inline td_void isp_af_vnum_write(isp_be_reg_type *be_reg, td_u32 uisp_af_vnum)
{
    u_isp_af_zone o_isp_af_zone;
    o_isp_af_zone.u32 = be_reg->isp_af_zone.u32;
    o_isp_af_zone.bits.isp_af_vnum = uisp_af_vnum;
    be_reg->isp_af_zone.u32 = o_isp_af_zone.u32;
}
/*  description   set the value of the member isp_af_crop_start.isp_af_pos_x */
/*  input         unsigned int uisp_af_pos_x  13 bits */
static __inline td_void isp_af_pos_x_write(isp_be_reg_type *be_reg, td_u32 uisp_af_pos_x)
{
    u_isp_af_crop_start o_isp_af_crop_start;
    o_isp_af_crop_start.u32 = be_reg->isp_af_crop_start.u32;
    o_isp_af_crop_start.bits.isp_af_pos_x = uisp_af_pos_x;
    be_reg->isp_af_crop_start.u32 = o_isp_af_crop_start.u32;
}
/*  description   set the value of the member isp_af_crop_start.isp_af_pos_y */
/*  input         unsigned int uisp_af_pos_y  13 bits */
static __inline td_void isp_af_pos_y_write(isp_be_reg_type *be_reg, td_u32 uisp_af_pos_y)
{
    u_isp_af_crop_start o_isp_af_crop_start;
    o_isp_af_crop_start.u32 = be_reg->isp_af_crop_start.u32;
    o_isp_af_crop_start.bits.isp_af_pos_y = uisp_af_pos_y;
    be_reg->isp_af_crop_start.u32 = o_isp_af_crop_start.u32;
}
/*  description   set the value of the member isp_af_crop_size.isp_af_crop_hsize */
/*  input         unsigned int uisp_af_crop_hsize  13 bits */
static __inline td_void isp_af_crop_hsize_write(isp_be_reg_type *be_reg, td_u32 uisp_af_crop_hsize)
{
    u_isp_af_crop_size o_isp_af_crop_size;
    o_isp_af_crop_size.u32 = be_reg->isp_af_crop_size.u32;
    o_isp_af_crop_size.bits.isp_af_crop_hsize = uisp_af_crop_hsize;
    be_reg->isp_af_crop_size.u32 = o_isp_af_crop_size.u32;
}
/*  description   set the value of the member isp_af_crop_size.isp_af_crop_vsize */
/*  input         unsigned int uisp_af_crop_vsize  13 bits */
static __inline td_void isp_af_crop_vsize_write(isp_be_reg_type *be_reg, td_u32 uisp_af_crop_vsize)
{
    u_isp_af_crop_size o_isp_af_crop_size;
    o_isp_af_crop_size.u32 = be_reg->isp_af_crop_size.u32;
    o_isp_af_crop_size.bits.isp_af_crop_vsize = uisp_af_crop_vsize;
    be_reg->isp_af_crop_size.u32 = o_isp_af_crop_size.u32;
}
/*  description   set the value of the member isp_af_mean_thres.isp_af_mean_thres */
/*  input         unsigned int uisp_af_mean_thres  16 bits */
static __inline td_void isp_af_mean_thres_write(isp_be_reg_type *be_reg, td_u32 uisp_af_mean_thres)
{
    u_isp_af_mean_thres o_isp_af_mean_thres;
    o_isp_af_mean_thres.u32 = be_reg->isp_af_mean_thres.u32;
    o_isp_af_mean_thres.bits.isp_af_mean_thres = uisp_af_mean_thres;
    be_reg->isp_af_mean_thres.u32 = o_isp_af_mean_thres.u32;
}
/*  description   set the value of the member isp_af_iirg0.isp_af_iirg0_0 */
/*  input         unsigned int uisp_af_iirg0_0  8 bits */
static __inline td_void isp_af_iirg0_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg0_0)
{
    u_isp_af_iirg0 o_isp_af_iirg0;
    o_isp_af_iirg0.u32 = be_reg->isp_af_iirg0.u32;
    o_isp_af_iirg0.bits.isp_af_iirg0_0 = uisp_af_iirg0_0;
    be_reg->isp_af_iirg0.u32 = o_isp_af_iirg0.u32;
}
/*  description   set the value of the member isp_af_iirg0.isp_af_iirg0_1 */
/*  input         unsigned int uisp_af_iirg0_1  8 bits */
static __inline td_void isp_af_iirg0_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg0_1)
{
    u_isp_af_iirg0 o_isp_af_iirg0;
    o_isp_af_iirg0.u32 = be_reg->isp_af_iirg0.u32;
    o_isp_af_iirg0.bits.isp_af_iirg0_1 = uisp_af_iirg0_1;
    be_reg->isp_af_iirg0.u32 = o_isp_af_iirg0.u32;
}
/*  description   set the value of the member isp_af_iirg1.isp_af_iirg1_0 */
/*  input         unsigned int uisp_af_iirg1_0  10 bits */
static __inline td_void isp_af_iirg1_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg1_0)
{
    u_isp_af_iirg1 o_isp_af_iirg1;
    o_isp_af_iirg1.u32 = be_reg->isp_af_iirg1.u32;
    o_isp_af_iirg1.bits.isp_af_iirg1_0 = uisp_af_iirg1_0;
    be_reg->isp_af_iirg1.u32 = o_isp_af_iirg1.u32;
}
/*  description   set the value of the member isp_af_iirg1.isp_af_iirg1_1 */
/*  input         unsigned int uisp_af_iirg1_1  10 bits */
static __inline td_void isp_af_iirg1_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg1_1)
{
    u_isp_af_iirg1 o_isp_af_iirg1;
    o_isp_af_iirg1.u32 = be_reg->isp_af_iirg1.u32;
    o_isp_af_iirg1.bits.isp_af_iirg1_1 = uisp_af_iirg1_1;
    be_reg->isp_af_iirg1.u32 = o_isp_af_iirg1.u32;
}
/*  description   set the value of the member isp_af_iirg2.isp_af_iirg2_0 */
/*  input         unsigned int uisp_af_iirg2_0  10 bits */
static __inline td_void isp_af_iirg2_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg2_0)
{
    u_isp_af_iirg2 o_isp_af_iirg2;
    o_isp_af_iirg2.u32 = be_reg->isp_af_iirg2.u32;
    o_isp_af_iirg2.bits.isp_af_iirg2_0 = uisp_af_iirg2_0;
    be_reg->isp_af_iirg2.u32 = o_isp_af_iirg2.u32;
}
/*  description   set the value of the member isp_af_iirg2.isp_af_iirg2_1 */
/*  input         unsigned int uisp_af_iirg2_1  10 bits */
static __inline td_void isp_af_iirg2_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg2_1)
{
    u_isp_af_iirg2 o_isp_af_iirg2;
    o_isp_af_iirg2.u32 = be_reg->isp_af_iirg2.u32;
    o_isp_af_iirg2.bits.isp_af_iirg2_1 = uisp_af_iirg2_1;
    be_reg->isp_af_iirg2.u32 = o_isp_af_iirg2.u32;
}
/*  description   set the value of the member isp_af_iirg3.isp_af_iirg3_0 */
/*  input         unsigned int uisp_af_iirg3_0  10 bits */
static __inline td_void isp_af_iirg3_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg3_0)
{
    u_isp_af_iirg3 o_isp_af_iirg3;
    o_isp_af_iirg3.u32 = be_reg->isp_af_iirg3.u32;
    o_isp_af_iirg3.bits.isp_af_iirg3_0 = uisp_af_iirg3_0;
    be_reg->isp_af_iirg3.u32 = o_isp_af_iirg3.u32;
}
/*  description   set the value of the member isp_af_iirg3.isp_af_iirg3_1 */
/*  input         unsigned int uisp_af_iirg3_1  10 bits */
static __inline td_void isp_af_iirg3_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg3_1)
{
    u_isp_af_iirg3 o_isp_af_iirg3;
    o_isp_af_iirg3.u32 = be_reg->isp_af_iirg3.u32;
    o_isp_af_iirg3.bits.isp_af_iirg3_1 = uisp_af_iirg3_1;
    be_reg->isp_af_iirg3.u32 = o_isp_af_iirg3.u32;
}
/*  description   set the value of the member isp_af_iirg4.isp_af_iirg4_0 */
/*  input         unsigned int uisp_af_iirg4_0  10 bits */
static __inline td_void isp_af_iirg4_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg4_0)
{
    u_isp_af_iirg4 o_isp_af_iirg4;
    o_isp_af_iirg4.u32 = be_reg->isp_af_iirg4.u32;
    o_isp_af_iirg4.bits.isp_af_iirg4_0 = uisp_af_iirg4_0;
    be_reg->isp_af_iirg4.u32 = o_isp_af_iirg4.u32;
}
/*  description   set the value of the member isp_af_iirg4.isp_af_iirg4_1 */
/*  input         unsigned int uisp_af_iirg4_1  10 bits */
static __inline td_void isp_af_iirg4_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg4_1)
{
    u_isp_af_iirg4 o_isp_af_iirg4;
    o_isp_af_iirg4.u32 = be_reg->isp_af_iirg4.u32;
    o_isp_af_iirg4.bits.isp_af_iirg4_1 = uisp_af_iirg4_1;
    be_reg->isp_af_iirg4.u32 = o_isp_af_iirg4.u32;
}
/*  description   set the value of the member isp_af_iirg5.isp_af_iirg5_0 */
/*  input         unsigned int uisp_af_iirg5_0  10 bits */
static __inline td_void isp_af_iirg5_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg5_0)
{
    u_isp_af_iirg5 o_isp_af_iirg5;
    o_isp_af_iirg5.u32 = be_reg->isp_af_iirg5.u32;
    o_isp_af_iirg5.bits.isp_af_iirg5_0 = uisp_af_iirg5_0;
    be_reg->isp_af_iirg5.u32 = o_isp_af_iirg5.u32;
}
/*  description   set the value of the member isp_af_iirg5.isp_af_iirg5_1 */
/*  input         unsigned int uisp_af_iirg5_1  10 bits */
static __inline td_void isp_af_iirg5_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg5_1)
{
    u_isp_af_iirg5 o_isp_af_iirg5;
    o_isp_af_iirg5.u32 = be_reg->isp_af_iirg5.u32;
    o_isp_af_iirg5.bits.isp_af_iirg5_1 = uisp_af_iirg5_1;
    be_reg->isp_af_iirg5.u32 = o_isp_af_iirg5.u32;
}
/*  description   set the value of the member isp_af_iirg6.isp_af_iirg6_0 */
/*  input         unsigned int uisp_af_iirg6_0  10 bits */
static __inline td_void isp_af_iirg6_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg6_0)
{
    u_isp_af_iirg6 o_isp_af_iirg6;
    o_isp_af_iirg6.u32 = be_reg->isp_af_iirg6.u32;
    o_isp_af_iirg6.bits.isp_af_iirg6_0 = uisp_af_iirg6_0;
    be_reg->isp_af_iirg6.u32 = o_isp_af_iirg6.u32;
}
/*  description   set the value of the member isp_af_iirg6.isp_af_iirg6_1 */
/*  input         unsigned int uisp_af_iirg6_1  10 bits */
static __inline td_void isp_af_iirg6_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirg6_1)
{
    u_isp_af_iirg6 o_isp_af_iirg6;
    o_isp_af_iirg6.u32 = be_reg->isp_af_iirg6.u32;
    o_isp_af_iirg6.bits.isp_af_iirg6_1 = uisp_af_iirg6_1;
    be_reg->isp_af_iirg6.u32 = o_isp_af_iirg6.u32;
}
/*  description   set the value of the member isp_af_iirpl.isp_af_iirplg_0 */
/*  input         unsigned int uisp_af_iirplg_0  8 bits */
#define OT_ISP_BE_AF_IIR0_PLG_DEFAULT                 0x96
static __inline td_void isp_af_iirplg_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirplg_0)
{
    u_isp_af_iirpl o_isp_af_iirpl;
    o_isp_af_iirpl.u32 = be_reg->isp_af_iirpl.u32;
    o_isp_af_iirpl.bits.isp_af_iirplg_0 = uisp_af_iirplg_0;
    be_reg->isp_af_iirpl.u32 = o_isp_af_iirpl.u32;
}
/*  description   set the value of the member isp_af_iirpl.isp_af_iirpls_0 */
/*  input         unsigned int uisp_af_iirpls_0  3 bits */
#define OT_ISP_BE_AF_IIR0_PLS_DEFAULT                 0x3
static __inline td_void isp_af_iirpls_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirpls_0)
{
    u_isp_af_iirpl o_isp_af_iirpl;
    o_isp_af_iirpl.u32 = be_reg->isp_af_iirpl.u32;
    o_isp_af_iirpl.bits.isp_af_iirpls_0 = uisp_af_iirpls_0;
    be_reg->isp_af_iirpl.u32 = o_isp_af_iirpl.u32;
}
/*  description   set the value of the member isp_af_iirpl.isp_af_iirplg_1 */
/*  input         unsigned int uisp_af_iirplg_1  8 bits */
#define OT_ISP_BE_AF_IIR1_PLG_DEFAULT                 0x99
static __inline td_void isp_af_iirplg_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirplg_1)
{
    u_isp_af_iirpl o_isp_af_iirpl;
    o_isp_af_iirpl.u32 = be_reg->isp_af_iirpl.u32;
    o_isp_af_iirpl.bits.isp_af_iirplg_1 = uisp_af_iirplg_1;
    be_reg->isp_af_iirpl.u32 = o_isp_af_iirpl.u32;
}
/*  description   set the value of the member isp_af_iirpl.isp_af_iirpls_1 */
/*  input         unsigned int uisp_af_iirpls_1  3 bits */
#define OT_ISP_BE_AF_IIR1_PLS_DEFAULT                 0x3
static __inline td_void isp_af_iirpls_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirpls_1)
{
    u_isp_af_iirpl o_isp_af_iirpl;
    o_isp_af_iirpl.u32 = be_reg->isp_af_iirpl.u32;
    o_isp_af_iirpl.bits.isp_af_iirpls_1 = uisp_af_iirpls_1;
    be_reg->isp_af_iirpl.u32 = o_isp_af_iirpl.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift0_0 */
/*  input         unsigned int uisp_af_iirshift0_0  3 bits */
static __inline td_void isp_af_iirshift0_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift0_0)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift0_0 = uisp_af_iirshift0_0;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift0_1 */
/*  input         unsigned int uisp_af_iirshift0_1  3 bits */
static __inline td_void isp_af_iirshift0_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift0_1)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift0_1 = uisp_af_iirshift0_1;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift0_2 */
/*  input         unsigned int uisp_af_iirshift0_2  3 bits */
static __inline td_void isp_af_iirshift0_2_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift0_2)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift0_2 = uisp_af_iirshift0_2;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift0_3 */
/*  input         unsigned int uisp_af_iirshift0_3  3 bits */
static __inline td_void isp_af_iirshift0_3_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift0_3)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift0_3 = uisp_af_iirshift0_3;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift1_0 */
/*  input         unsigned int uisp_af_iirshift1_0  3 bits */
static __inline td_void isp_af_iirshift1_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift1_0)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift1_0 = uisp_af_iirshift1_0;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift1_1 */
/*  input         unsigned int uisp_af_iirshift1_1  3 bits */
static __inline td_void isp_af_iirshift1_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift1_1)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift1_1 = uisp_af_iirshift1_1;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift1_2 */
/*  input         unsigned int uisp_af_iirshift1_2  3 bits */
static __inline td_void isp_af_iirshift1_2_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift1_2)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift1_2 = uisp_af_iirshift1_2;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_shift.isp_af_iirshift1_3 */
/*  input         unsigned int uisp_af_iirshift1_3  3 bits */
static __inline td_void isp_af_iirshift1_3_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iirshift1_3)
{
    u_isp_af_shift o_isp_af_shift;
    o_isp_af_shift.u32 = be_reg->isp_af_shift.u32;
    o_isp_af_shift.bits.isp_af_iirshift1_3 = uisp_af_iirshift1_3;
    be_reg->isp_af_shift.u32 = o_isp_af_shift.u32;
}
/*  description   set the value of the member isp_af_firh0.isp_af_firh0_0 */
/*  input         unsigned int uisp_af_firh0_0  6 bits */
static __inline td_void isp_af_firh0_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh0_0)
{
    u_isp_af_firh0 o_isp_af_firh0;
    o_isp_af_firh0.u32 = be_reg->isp_af_firh0.u32;
    o_isp_af_firh0.bits.isp_af_firh0_0 = uisp_af_firh0_0;
    be_reg->isp_af_firh0.u32 = o_isp_af_firh0.u32;
}
/*  description   set the value of the member isp_af_firh0.isp_af_firh0_1 */
/*  input         unsigned int uisp_af_firh0_1  6 bits */
static __inline td_void isp_af_firh0_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh0_1)
{
    u_isp_af_firh0 o_isp_af_firh0;
    o_isp_af_firh0.u32 = be_reg->isp_af_firh0.u32;
    o_isp_af_firh0.bits.isp_af_firh0_1 = uisp_af_firh0_1;
    be_reg->isp_af_firh0.u32 = o_isp_af_firh0.u32;
}
/*  description   set the value of the member isp_af_firh1.isp_af_firh1_0 */
/*  input         unsigned int uisp_af_firh1_0  6 bits */
static __inline td_void isp_af_firh1_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh1_0)
{
    u_isp_af_firh1 o_isp_af_firh1;
    o_isp_af_firh1.u32 = be_reg->isp_af_firh1.u32;
    o_isp_af_firh1.bits.isp_af_firh1_0 = uisp_af_firh1_0;
    be_reg->isp_af_firh1.u32 = o_isp_af_firh1.u32;
}
/*  description   set the value of the member isp_af_firh1.isp_af_firh1_1 */
/*  input         unsigned int uisp_af_firh1_1  6 bits */
static __inline td_void isp_af_firh1_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh1_1)
{
    u_isp_af_firh1 o_isp_af_firh1;
    o_isp_af_firh1.u32 = be_reg->isp_af_firh1.u32;
    o_isp_af_firh1.bits.isp_af_firh1_1 = uisp_af_firh1_1;
    be_reg->isp_af_firh1.u32 = o_isp_af_firh1.u32;
}
/*  description   set the value of the member isp_af_firh2.isp_af_firh2_0 */
/*  input         unsigned int uisp_af_firh2_0  6 bits */
static __inline td_void isp_af_firh2_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh2_0)
{
    u_isp_af_firh2 o_isp_af_firh2;
    o_isp_af_firh2.u32 = be_reg->isp_af_firh2.u32;
    o_isp_af_firh2.bits.isp_af_firh2_0 = uisp_af_firh2_0;
    be_reg->isp_af_firh2.u32 = o_isp_af_firh2.u32;
}
/*  description   set the value of the member isp_af_firh2.isp_af_firh2_1 */
/*  input         unsigned int uisp_af_firh2_1  6 bits */
static __inline td_void isp_af_firh2_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh2_1)
{
    u_isp_af_firh2 o_isp_af_firh2;
    o_isp_af_firh2.u32 = be_reg->isp_af_firh2.u32;
    o_isp_af_firh2.bits.isp_af_firh2_1 = uisp_af_firh2_1;
    be_reg->isp_af_firh2.u32 = o_isp_af_firh2.u32;
}
/*  description   set the value of the member isp_af_firh3.isp_af_firh3_0 */
/*  input         unsigned int uisp_af_firh3_0  6 bits */
static __inline td_void isp_af_firh3_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh3_0)
{
    u_isp_af_firh3 o_isp_af_firh3;
    o_isp_af_firh3.u32 = be_reg->isp_af_firh3.u32;
    o_isp_af_firh3.bits.isp_af_firh3_0 = uisp_af_firh3_0;
    be_reg->isp_af_firh3.u32 = o_isp_af_firh3.u32;
}
/*  description   set the value of the member isp_af_firh3.isp_af_firh3_1 */
/*  input         unsigned int uisp_af_firh3_1  6 bits */
static __inline td_void isp_af_firh3_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh3_1)
{
    u_isp_af_firh3 o_isp_af_firh3;
    o_isp_af_firh3.u32 = be_reg->isp_af_firh3.u32;
    o_isp_af_firh3.bits.isp_af_firh3_1 = uisp_af_firh3_1;
    be_reg->isp_af_firh3.u32 = o_isp_af_firh3.u32;
}
/*  description   set the value of the member isp_af_firh4.isp_af_firh4_0 */
/*  input         unsigned int uisp_af_firh4_0  6 bits */
static __inline td_void isp_af_firh4_0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh4_0)
{
    u_isp_af_firh4 o_isp_af_firh4;
    o_isp_af_firh4.u32 = be_reg->isp_af_firh4.u32;
    o_isp_af_firh4.bits.isp_af_firh4_0 = uisp_af_firh4_0;
    be_reg->isp_af_firh4.u32 = o_isp_af_firh4.u32;
}
/*  description   set the value of the member isp_af_firh4.isp_af_firh4_1 */
/*  input         unsigned int uisp_af_firh4_1  6 bits */
static __inline td_void isp_af_firh4_1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_firh4_1)
{
    u_isp_af_firh4 o_isp_af_firh4;
    o_isp_af_firh4.u32 = be_reg->isp_af_firh4.u32;
    o_isp_af_firh4.bits.isp_af_firh4_1 = uisp_af_firh4_1;
    be_reg->isp_af_firh4.u32 = o_isp_af_firh4.u32;
}
/*  description   set the value of the member isp_af_stt_bst.isp_af_stt_size */
/*  input         unsigned int uisp_af_stt_size  16 bits */
static __inline td_void isp_af_stt_size_write(isp_be_reg_type *be_reg, td_u32 uisp_af_stt_size)
{
    u_isp_af_stt_bst o_isp_af_stt_bst;
    o_isp_af_stt_bst.u32 = be_reg->isp_af_stt_bst.u32;
    o_isp_af_stt_bst.bits.isp_af_stt_size = uisp_af_stt_size;
    be_reg->isp_af_stt_bst.u32 = o_isp_af_stt_bst.u32;
}
/*  description   set the value of the member isp_af_stt_bst.isp_af_stt_bst */
/*  input         unsigned int uisp_af_stt_bst  4 bits */
static __inline td_void isp_af_stt_bst_write(isp_be_reg_type *be_reg, td_u32 uisp_af_stt_bst)
{
    u_isp_af_stt_bst o_isp_af_stt_bst;
    o_isp_af_stt_bst.u32 = be_reg->isp_af_stt_bst.u32;
    o_isp_af_stt_bst.bits.isp_af_stt_bst = uisp_af_stt_bst;
    be_reg->isp_af_stt_bst.u32 = o_isp_af_stt_bst.u32;
}
/*  description   set the value of the member isp_af_stt_bst.isp_af_stt_en */
/*  input         unsigned int uisp_af_stt_en  1 bits */
static __inline td_void isp_af_stt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_af_stt_en)
{
    u_isp_af_stt_bst o_isp_af_stt_bst;
    o_isp_af_stt_bst.u32 = be_reg->isp_af_stt_bst.u32;
    o_isp_af_stt_bst.bits.isp_af_stt_en = uisp_af_stt_en;
    be_reg->isp_af_stt_bst.u32 = o_isp_af_stt_bst.u32;
}
/*  description   set the value of the member isp_af_stt_abn.isp_af_stt_clr */
/*  input         unsigned int uisp_af_stt_clr  1 bits */
static __inline td_void isp_af_stt_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_af_stt_clr)
{
    u_isp_af_stt_abn o_isp_af_stt_abn;
    o_isp_af_stt_abn.u32 = be_reg->isp_af_stt_abn.u32;
    o_isp_af_stt_abn.bits.isp_af_stt_clr = uisp_af_stt_clr;
    be_reg->isp_af_stt_abn.u32 = o_isp_af_stt_abn.u32;
}
/*  description   set the value of the member isp_af_acc_shift.isp_af_acc_shift0_h */
/*  input         unsigned int uisp_af_acc_shift0_h  4 bits */
static __inline td_void isp_af_acc_shift0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_acc_shift0_h)
{
    u_isp_af_acc_shift o_isp_af_acc_shift;
    o_isp_af_acc_shift.u32 = be_reg->isp_af_acc_shift.u32;
    o_isp_af_acc_shift.bits.isp_af_acc_shift0_h = uisp_af_acc_shift0_h;
    be_reg->isp_af_acc_shift.u32 = o_isp_af_acc_shift.u32;
}
/*  description   set the value of the member isp_af_acc_shift.isp_af_acc_shift1_h */
/*  input         unsigned int uisp_af_acc_shift1_h  4 bits */
static __inline td_void isp_af_acc_shift1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_acc_shift1_h)
{
    u_isp_af_acc_shift o_isp_af_acc_shift;
    o_isp_af_acc_shift.u32 = be_reg->isp_af_acc_shift.u32;
    o_isp_af_acc_shift.bits.isp_af_acc_shift1_h = uisp_af_acc_shift1_h;
    be_reg->isp_af_acc_shift.u32 = o_isp_af_acc_shift.u32;
}
/*  description   set the value of the member isp_af_acc_shift.isp_af_acc_shift0_v */
/*  input         unsigned int uisp_af_acc_shift0_v  4 bits */
static __inline td_void isp_af_acc_shift0_v_write(isp_be_reg_type *be_reg, td_u32 uisp_af_acc_shift0_v)
{
    u_isp_af_acc_shift o_isp_af_acc_shift;
    o_isp_af_acc_shift.u32 = be_reg->isp_af_acc_shift.u32;
    o_isp_af_acc_shift.bits.isp_af_acc_shift0_v = uisp_af_acc_shift0_v;
    be_reg->isp_af_acc_shift.u32 = o_isp_af_acc_shift.u32;
}
/*  description   set the value of the member isp_af_acc_shift.isp_af_acc_shift1_v */
/*  input         unsigned int uisp_af_acc_shift1_v  4 bits */
static __inline td_void isp_af_acc_shift1_v_write(isp_be_reg_type *be_reg, td_u32 uisp_af_acc_shift1_v)
{
    u_isp_af_acc_shift o_isp_af_acc_shift;
    o_isp_af_acc_shift.u32 = be_reg->isp_af_acc_shift.u32;
    o_isp_af_acc_shift.bits.isp_af_acc_shift1_v = uisp_af_acc_shift1_v;
    be_reg->isp_af_acc_shift.u32 = o_isp_af_acc_shift.u32;
}
/*  description   set the value of the member isp_af_acc_shift.isp_af_acc_shift_y */
/*  input         unsigned int uisp_af_acc_shift_y  4 bits */
static __inline td_void isp_af_acc_shift_y_write(isp_be_reg_type *be_reg, td_u32 uisp_af_acc_shift_y)
{
    u_isp_af_acc_shift o_isp_af_acc_shift;
    o_isp_af_acc_shift.u32 = be_reg->isp_af_acc_shift.u32;
    o_isp_af_acc_shift.bits.isp_af_acc_shift_y = uisp_af_acc_shift_y;
    be_reg->isp_af_acc_shift.u32 = o_isp_af_acc_shift.u32;
}
/*  description   set the value of the member isp_af_cnt_shift.isp_af_cnt_shift0_h */
/*  input         unsigned int uisp_af_cnt_shift0_h  4 bits */
static __inline td_void isp_af_cnt_shift0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_cnt_shift0_h)
{
    u_isp_af_cnt_shift o_isp_af_cnt_shift;
    o_isp_af_cnt_shift.u32 = be_reg->isp_af_cnt_shift.u32;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift0_h = uisp_af_cnt_shift0_h;
    be_reg->isp_af_cnt_shift.u32 = o_isp_af_cnt_shift.u32;
}
/*  description   set the value of the member isp_af_cnt_shift.isp_af_cnt_shift1_h */
/*  input         unsigned int uisp_af_cnt_shift1_h  4 bits */
static __inline td_void isp_af_cnt_shift1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_cnt_shift1_h)
{
    u_isp_af_cnt_shift o_isp_af_cnt_shift;
    o_isp_af_cnt_shift.u32 = be_reg->isp_af_cnt_shift.u32;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift1_h = uisp_af_cnt_shift1_h;
    be_reg->isp_af_cnt_shift.u32 = o_isp_af_cnt_shift.u32;
}
/*  description   set the value of the member isp_af_cnt_shift.isp_af_cnt_shift0_v */
/*  input         unsigned int uisp_af_cnt_shift0_v  4 bits */
#define ISP_AF_CNT_SHIFT0_V_DEFAULT                   0x2
static __inline td_void isp_af_cnt_shift0_v_write(isp_be_reg_type *be_reg, td_u32 uisp_af_cnt_shift0_v)
{
    u_isp_af_cnt_shift o_isp_af_cnt_shift;
    o_isp_af_cnt_shift.u32 = be_reg->isp_af_cnt_shift.u32;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift0_v = uisp_af_cnt_shift0_v;
    be_reg->isp_af_cnt_shift.u32 = o_isp_af_cnt_shift.u32;
}
/*  description   set the value of the member isp_af_cnt_shift.isp_af_cnt_shift1_v */
/*  input         unsigned int uisp_af_cnt_shift1_v  4 bits */
static __inline td_void isp_af_cnt_shift1_v_write(isp_be_reg_type *be_reg, td_u32 uisp_af_cnt_shift1_v)
{
    u_isp_af_cnt_shift o_isp_af_cnt_shift;
    o_isp_af_cnt_shift.u32 = be_reg->isp_af_cnt_shift.u32;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift1_v = uisp_af_cnt_shift1_v;
    be_reg->isp_af_cnt_shift.u32 = o_isp_af_cnt_shift.u32;
}
/*  description   set the value of the member isp_af_cnt_shift.isp_af_cnt_shift_y */
/*  input         unsigned int uisp_af_cnt_shift_y  4 bits */
static __inline td_void isp_af_cnt_shift_y_write(isp_be_reg_type *be_reg, td_u32 uisp_af_cnt_shift_y)
{
    u_isp_af_cnt_shift o_isp_af_cnt_shift;
    o_isp_af_cnt_shift.u32 = be_reg->isp_af_cnt_shift.u32;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift_y = uisp_af_cnt_shift_y;
    be_reg->isp_af_cnt_shift.u32 = o_isp_af_cnt_shift.u32;
}
/*  description   set the value of the member isp_af_stat_raddr.isp_af_stat_raddr */
/*  input         unsigned int uisp_af_stat_raddr  32 bits */
static __inline td_void isp_af_stat_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_af_stat_raddr)
{
    be_reg->isp_af_stat_raddr.u32 = uisp_af_stat_raddr;
}
/*  description   set the value of the member isp_af_iirthre.isp_af_iir_thre0_l */
/*  input         unsigned int uisp_af_iir_thre0_l  8 bits */
static __inline td_void isp_af_iir_thre0_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_thre0_l)
{
    u_isp_af_iirthre o_isp_af_iirthre;
    o_isp_af_iirthre.u32 = be_reg->isp_af_iirthre.u32;
    o_isp_af_iirthre.bits.isp_af_iir_thre0_l = uisp_af_iir_thre0_l;
    be_reg->isp_af_iirthre.u32 = o_isp_af_iirthre.u32;
}
/*  description   set the value of the member isp_af_iirthre.isp_af_iir_thre0_h */
/*  input         unsigned int uisp_af_iir_thre0_h  8 bits */
static __inline td_void isp_af_iir_thre0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_thre0_h)
{
    u_isp_af_iirthre o_isp_af_iirthre;
    o_isp_af_iirthre.u32 = be_reg->isp_af_iirthre.u32;
    o_isp_af_iirthre.bits.isp_af_iir_thre0_h = uisp_af_iir_thre0_h;
    be_reg->isp_af_iirthre.u32 = o_isp_af_iirthre.u32;
}
/*  description   set the value of the member isp_af_iirthre.isp_af_iir_thre1_l */
/*  input         unsigned int uisp_af_iir_thre1_l  8 bits */
static __inline td_void isp_af_iir_thre1_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_thre1_l)
{
    u_isp_af_iirthre o_isp_af_iirthre;
    o_isp_af_iirthre.u32 = be_reg->isp_af_iirthre.u32;
    o_isp_af_iirthre.bits.isp_af_iir_thre1_l = uisp_af_iir_thre1_l;
    be_reg->isp_af_iirthre.u32 = o_isp_af_iirthre.u32;
}
/*  description   set the value of the member isp_af_iirthre.isp_af_iir_thre1_h */
/*  input         unsigned int uisp_af_iir_thre1_h  8 bits */
static __inline td_void isp_af_iir_thre1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_thre1_h)
{
    u_isp_af_iirthre o_isp_af_iirthre;
    o_isp_af_iirthre.u32 = be_reg->isp_af_iirthre.u32;
    o_isp_af_iirthre.bits.isp_af_iir_thre1_h = uisp_af_iir_thre1_h;
    be_reg->isp_af_iirthre.u32 = o_isp_af_iirthre.u32;
}
/*  description   set the value of the member isp_af_iirgain.isp_af_iir_gain0_l */
/*  input         unsigned int uisp_af_iir_gain0_l  8 bits */
static __inline td_void isp_af_iir_gain0_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_gain0_l)
{
    u_isp_af_iirgain o_isp_af_iirgain;
    o_isp_af_iirgain.u32 = be_reg->isp_af_iirgain.u32;
    o_isp_af_iirgain.bits.isp_af_iir_gain0_l = uisp_af_iir_gain0_l;
    be_reg->isp_af_iirgain.u32 = o_isp_af_iirgain.u32;
}
/*  description   set the value of the member isp_af_iirgain.isp_af_iir_gain0_h */
/*  input         unsigned int uisp_af_iir_gain0_h  8 bits */
static __inline td_void isp_af_iir_gain0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_gain0_h)
{
    u_isp_af_iirgain o_isp_af_iirgain;
    o_isp_af_iirgain.u32 = be_reg->isp_af_iirgain.u32;
    o_isp_af_iirgain.bits.isp_af_iir_gain0_h = uisp_af_iir_gain0_h;
    be_reg->isp_af_iirgain.u32 = o_isp_af_iirgain.u32;
}
/*  description   set the value of the member isp_af_iirgain.isp_af_iir_gain1_l */
/*  input         unsigned int uisp_af_iir_gain1_l  8 bits */
static __inline td_void isp_af_iir_gain1_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_gain1_l)
{
    u_isp_af_iirgain o_isp_af_iirgain;
    o_isp_af_iirgain.u32 = be_reg->isp_af_iirgain.u32;
    o_isp_af_iirgain.bits.isp_af_iir_gain1_l = uisp_af_iir_gain1_l;
    be_reg->isp_af_iirgain.u32 = o_isp_af_iirgain.u32;
}
/*  description   set the value of the member isp_af_iirgain.isp_af_iir_gain1_h */
/*  input         unsigned int uisp_af_iir_gain1_h  8 bits */
static __inline td_void isp_af_iir_gain1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_gain1_h)
{
    u_isp_af_iirgain o_isp_af_iirgain;
    o_isp_af_iirgain.u32 = be_reg->isp_af_iirgain.u32;
    o_isp_af_iirgain.bits.isp_af_iir_gain1_h = uisp_af_iir_gain1_h;
    be_reg->isp_af_iirgain.u32 = o_isp_af_iirgain.u32;
}
/*  description   set the value of the member isp_af_iirslope.isp_af_iir_slope0_l */
/*  input         unsigned int uisp_af_iir_slope0_l  4 bits */
static __inline td_void isp_af_iir_slope0_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_slope0_l)
{
    u_isp_af_iirslope o_isp_af_iirslope;
    o_isp_af_iirslope.u32 = be_reg->isp_af_iirslope.u32;
    o_isp_af_iirslope.bits.isp_af_iir_slope0_l = uisp_af_iir_slope0_l;
    be_reg->isp_af_iirslope.u32 = o_isp_af_iirslope.u32;
}
/*  description   set the value of the member isp_af_iirslope.isp_af_iir_slope0_h */
/*  input         unsigned int uisp_af_iir_slope0_h  4 bits */
static __inline td_void isp_af_iir_slope0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_slope0_h)
{
    u_isp_af_iirslope o_isp_af_iirslope;
    o_isp_af_iirslope.u32 = be_reg->isp_af_iirslope.u32;
    o_isp_af_iirslope.bits.isp_af_iir_slope0_h = uisp_af_iir_slope0_h;
    be_reg->isp_af_iirslope.u32 = o_isp_af_iirslope.u32;
}
/*  description   set the value of the member isp_af_iirslope.isp_af_iir_slope1_l */
/*  input         unsigned int uisp_af_iir_slope1_l  4 bits */
static __inline td_void isp_af_iir_slope1_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_slope1_l)
{
    u_isp_af_iirslope o_isp_af_iirslope;
    o_isp_af_iirslope.u32 = be_reg->isp_af_iirslope.u32;
    o_isp_af_iirslope.bits.isp_af_iir_slope1_l = uisp_af_iir_slope1_l;
    be_reg->isp_af_iirslope.u32 = o_isp_af_iirslope.u32;
}
/*  description   set the value of the member isp_af_iirslope.isp_af_iir_slope1_h */
/*  input         unsigned int uisp_af_iir_slope1_h  4 bits */
static __inline td_void isp_af_iir_slope1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_slope1_h)
{
    u_isp_af_iirslope o_isp_af_iirslope;
    o_isp_af_iirslope.u32 = be_reg->isp_af_iirslope.u32;
    o_isp_af_iirslope.bits.isp_af_iir_slope1_h = uisp_af_iir_slope1_h;
    be_reg->isp_af_iirslope.u32 = o_isp_af_iirslope.u32;
}
/*  description   set the value of the member isp_af_iirdilate.isp_af_iir_dilate0 */
/*  input         unsigned int uisp_af_iir_dilate0  3 bits */
static __inline td_void isp_af_iir_dilate0_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_dilate0)
{
    u_isp_af_iirdilate o_isp_af_iirdilate;
    o_isp_af_iirdilate.u32 = be_reg->isp_af_iirdilate.u32;
    o_isp_af_iirdilate.bits.isp_af_iir_dilate0 = uisp_af_iir_dilate0;
    be_reg->isp_af_iirdilate.u32 = o_isp_af_iirdilate.u32;
}
/*  description   set the value of the member isp_af_iirdilate.isp_af_iir_dilate1 */
/*  input         unsigned int uisp_af_iir_dilate1  3 bits */
static __inline td_void isp_af_iir_dilate1_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_dilate1)
{
    u_isp_af_iirdilate o_isp_af_iirdilate;
    o_isp_af_iirdilate.u32 = be_reg->isp_af_iirdilate.u32;
    o_isp_af_iirdilate.bits.isp_af_iir_dilate1 = uisp_af_iir_dilate1;
    be_reg->isp_af_iirdilate.u32 = o_isp_af_iirdilate.u32;
}
/*  description   set the value of the member isp_af_firthre.isp_af_fir_thre0_l */
/*  input         unsigned int uisp_af_fir_thre0_l  8 bits */
static __inline td_void isp_af_fir_thre0_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_thre0_l)
{
    u_isp_af_firthre o_isp_af_firthre;
    o_isp_af_firthre.u32 = be_reg->isp_af_firthre.u32;
    o_isp_af_firthre.bits.isp_af_fir_thre0_l = uisp_af_fir_thre0_l;
    be_reg->isp_af_firthre.u32 = o_isp_af_firthre.u32;
}
/*  description   set the value of the member isp_af_firthre.isp_af_fir_thre0_h */
/*  input         unsigned int uisp_af_fir_thre0_h  8 bits */
static __inline td_void isp_af_fir_thre0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_thre0_h)
{
    u_isp_af_firthre o_isp_af_firthre;
    o_isp_af_firthre.u32 = be_reg->isp_af_firthre.u32;
    o_isp_af_firthre.bits.isp_af_fir_thre0_h = uisp_af_fir_thre0_h;
    be_reg->isp_af_firthre.u32 = o_isp_af_firthre.u32;
}
/*  description   set the value of the member isp_af_firthre.isp_af_fir_thre1_l */
/*  input         unsigned int uisp_af_fir_thre1_l  8 bits */
static __inline td_void isp_af_fir_thre1_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_thre1_l)
{
    u_isp_af_firthre o_isp_af_firthre;
    o_isp_af_firthre.u32 = be_reg->isp_af_firthre.u32;
    o_isp_af_firthre.bits.isp_af_fir_thre1_l = uisp_af_fir_thre1_l;
    be_reg->isp_af_firthre.u32 = o_isp_af_firthre.u32;
}
/*  description   set the value of the member isp_af_firthre.isp_af_fir_thre1_h */
/*  input         unsigned int uisp_af_fir_thre1_h  8 bits */
static __inline td_void isp_af_fir_thre1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_thre1_h)
{
    u_isp_af_firthre o_isp_af_firthre;
    o_isp_af_firthre.u32 = be_reg->isp_af_firthre.u32;
    o_isp_af_firthre.bits.isp_af_fir_thre1_h = uisp_af_fir_thre1_h;
    be_reg->isp_af_firthre.u32 = o_isp_af_firthre.u32;
}
/*  description   set the value of the member isp_af_firgain.isp_af_fir_gain0_l */
/*  input         unsigned int uisp_af_fir_gain0_l  8 bits */
static __inline td_void isp_af_fir_gain0_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_gain0_l)
{
    u_isp_af_firgain o_isp_af_firgain;
    o_isp_af_firgain.u32 = be_reg->isp_af_firgain.u32;
    o_isp_af_firgain.bits.isp_af_fir_gain0_l = uisp_af_fir_gain0_l;
    be_reg->isp_af_firgain.u32 = o_isp_af_firgain.u32;
}
/*  description   set the value of the member isp_af_firgain.isp_af_fir_gain0_h */
/*  input         unsigned int uisp_af_fir_gain0_h  8 bits */
static __inline td_void isp_af_fir_gain0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_gain0_h)
{
    u_isp_af_firgain o_isp_af_firgain;
    o_isp_af_firgain.u32 = be_reg->isp_af_firgain.u32;
    o_isp_af_firgain.bits.isp_af_fir_gain0_h = uisp_af_fir_gain0_h;
    be_reg->isp_af_firgain.u32 = o_isp_af_firgain.u32;
}
/*  description   set the value of the member isp_af_firgain.isp_af_fir_gain1_l */
/*  input         unsigned int uisp_af_fir_gain1_l  8 bits */
static __inline td_void isp_af_fir_gain1_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_gain1_l)
{
    u_isp_af_firgain o_isp_af_firgain;
    o_isp_af_firgain.u32 = be_reg->isp_af_firgain.u32;
    o_isp_af_firgain.bits.isp_af_fir_gain1_l = uisp_af_fir_gain1_l;
    be_reg->isp_af_firgain.u32 = o_isp_af_firgain.u32;
}
/*  description   set the value of the member isp_af_firgain.isp_af_fir_gain1_h */
/*  input         unsigned int uisp_af_fir_gain1_h  8 bits */
static __inline td_void isp_af_fir_gain1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_gain1_h)
{
    u_isp_af_firgain o_isp_af_firgain;
    o_isp_af_firgain.u32 = be_reg->isp_af_firgain.u32;
    o_isp_af_firgain.bits.isp_af_fir_gain1_h = uisp_af_fir_gain1_h;
    be_reg->isp_af_firgain.u32 = o_isp_af_firgain.u32;
}
/*  description   set the value of the member isp_af_firslope.isp_af_fir_slope0_l */
/*  input         unsigned int uisp_af_fir_slope0_l  4 bits */
static __inline td_void isp_af_fir_slope0_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_slope0_l)
{
    u_isp_af_firslope o_isp_af_firslope;
    o_isp_af_firslope.u32 = be_reg->isp_af_firslope.u32;
    o_isp_af_firslope.bits.isp_af_fir_slope0_l = uisp_af_fir_slope0_l;
    be_reg->isp_af_firslope.u32 = o_isp_af_firslope.u32;
}
/*  description   set the value of the member isp_af_firslope.isp_af_fir_slope0_h */
/*  input         unsigned int uisp_af_fir_slope0_h  4 bits */
static __inline td_void isp_af_fir_slope0_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_slope0_h)
{
    u_isp_af_firslope o_isp_af_firslope;
    o_isp_af_firslope.u32 = be_reg->isp_af_firslope.u32;
    o_isp_af_firslope.bits.isp_af_fir_slope0_h = uisp_af_fir_slope0_h;
    be_reg->isp_af_firslope.u32 = o_isp_af_firslope.u32;
}
/*  description   set the value of the member isp_af_firslope.isp_af_fir_slope1_l */
/*  input         unsigned int uisp_af_fir_slope1_l  4 bits */
static __inline td_void isp_af_fir_slope1_l_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_slope1_l)
{
    u_isp_af_firslope o_isp_af_firslope;
    o_isp_af_firslope.u32 = be_reg->isp_af_firslope.u32;
    o_isp_af_firslope.bits.isp_af_fir_slope1_l = uisp_af_fir_slope1_l;
    be_reg->isp_af_firslope.u32 = o_isp_af_firslope.u32;
}
/*  description   set the value of the member isp_af_firslope.isp_af_fir_slope1_h */
/*  input         unsigned int uisp_af_fir_slope1_h  4 bits */
static __inline td_void isp_af_fir_slope1_h_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_slope1_h)
{
    u_isp_af_firslope o_isp_af_firslope;
    o_isp_af_firslope.u32 = be_reg->isp_af_firslope.u32;
    o_isp_af_firslope.bits.isp_af_fir_slope1_h = uisp_af_fir_slope1_h;
    be_reg->isp_af_firslope.u32 = o_isp_af_firslope.u32;
}
/*  description   set the value of the member isp_af_iirthre_coring.isp_af_iir_thre0_c */
/*  input         unsigned int uisp_af_iir_thre0_c  11 bits */
static __inline td_void isp_af_iir_thre0_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_thre0_c)
{
    u_isp_af_iirthre_coring o_isp_af_iirthre_coring;
    o_isp_af_iirthre_coring.u32 = be_reg->isp_af_iirthre_coring.u32;
    o_isp_af_iirthre_coring.bits.isp_af_iir_thre0_c = uisp_af_iir_thre0_c;
    be_reg->isp_af_iirthre_coring.u32 = o_isp_af_iirthre_coring.u32;
}
/*  description   set the value of the member isp_af_iirthre_coring.isp_af_iir_thre1_c */
/*  input         unsigned int uisp_af_iir_thre1_c  11 bits */
static __inline td_void isp_af_iir_thre1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_thre1_c)
{
    u_isp_af_iirthre_coring o_isp_af_iirthre_coring;
    o_isp_af_iirthre_coring.u32 = be_reg->isp_af_iirthre_coring.u32;
    o_isp_af_iirthre_coring.bits.isp_af_iir_thre1_c = uisp_af_iir_thre1_c;
    be_reg->isp_af_iirthre_coring.u32 = o_isp_af_iirthre_coring.u32;
}
/*  description   set the value of the member isp_af_iirpeak_coring.isp_af_iir_peak0_c */
/*  input         unsigned int uisp_af_iir_peak0_c  11 bits */
static __inline td_void isp_af_iir_peak0_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_peak0_c)
{
    u_isp_af_iirpeak_coring o_isp_af_iirpeak_coring;
    o_isp_af_iirpeak_coring.u32 = be_reg->isp_af_iirpeak_coring.u32;
    o_isp_af_iirpeak_coring.bits.isp_af_iir_peak0_c = uisp_af_iir_peak0_c;
    be_reg->isp_af_iirpeak_coring.u32 = o_isp_af_iirpeak_coring.u32;
}
/*  description   set the value of the member isp_af_iirpeak_coring.isp_af_iir_peak1_c */
/*  input         unsigned int uisp_af_iir_peak1_c  11 bits */
static __inline td_void isp_af_iir_peak1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_peak1_c)
{
    u_isp_af_iirpeak_coring o_isp_af_iirpeak_coring;
    o_isp_af_iirpeak_coring.u32 = be_reg->isp_af_iirpeak_coring.u32;
    o_isp_af_iirpeak_coring.bits.isp_af_iir_peak1_c = uisp_af_iir_peak1_c;
    be_reg->isp_af_iirpeak_coring.u32 = o_isp_af_iirpeak_coring.u32;
}
/*  description   set the value of the member isp_af_iirslope_coring.isp_af_iir_slope0_c */
/*  input         unsigned int uisp_af_iir_slope0_c  4 bits */
static __inline td_void isp_af_iir_slope0_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_slope0_c)
{
    u_isp_af_iirslope_coring o_isp_af_iirslope_coring;
    o_isp_af_iirslope_coring.u32 = be_reg->isp_af_iirslope_coring.u32;
    o_isp_af_iirslope_coring.bits.isp_af_iir_slope0_c = uisp_af_iir_slope0_c;
    be_reg->isp_af_iirslope_coring.u32 = o_isp_af_iirslope_coring.u32;
}
/*  description   set the value of the member isp_af_iirslope_coring.isp_af_iir_slope1_c */
/*  input         unsigned int uisp_af_iir_slope1_c  4 bits */
static __inline td_void isp_af_iir_slope1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_iir_slope1_c)
{
    u_isp_af_iirslope_coring o_isp_af_iirslope_coring;
    o_isp_af_iirslope_coring.u32 = be_reg->isp_af_iirslope_coring.u32;
    o_isp_af_iirslope_coring.bits.isp_af_iir_slope1_c = uisp_af_iir_slope1_c;
    be_reg->isp_af_iirslope_coring.u32 = o_isp_af_iirslope_coring.u32;
}
/*  description   set the value of the member isp_af_firthre_coring.isp_af_fir_thre0_c */
/*  input         unsigned int uisp_af_fir_thre0_c  11 bits */
static __inline td_void isp_af_fir_thre0_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_thre0_c)
{
    u_isp_af_firthre_coring o_isp_af_firthre_coring;
    o_isp_af_firthre_coring.u32 = be_reg->isp_af_firthre_coring.u32;
    o_isp_af_firthre_coring.bits.isp_af_fir_thre0_c = uisp_af_fir_thre0_c;
    be_reg->isp_af_firthre_coring.u32 = o_isp_af_firthre_coring.u32;
}
/*  description   set the value of the member isp_af_firthre_coring.isp_af_fir_thre1_c */
/*  input         unsigned int uisp_af_fir_thre1_c  11 bits */
static __inline td_void isp_af_fir_thre1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_thre1_c)
{
    u_isp_af_firthre_coring o_isp_af_firthre_coring;
    o_isp_af_firthre_coring.u32 = be_reg->isp_af_firthre_coring.u32;
    o_isp_af_firthre_coring.bits.isp_af_fir_thre1_c = uisp_af_fir_thre1_c;
    be_reg->isp_af_firthre_coring.u32 = o_isp_af_firthre_coring.u32;
}
/*  description   set the value of the member isp_af_firpeak_coring.isp_af_fir_peak0_c */
/*  input         unsigned int uisp_af_fir_peak0_c  11 bits */
static __inline td_void isp_af_fir_peak0_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_peak0_c)
{
    u_isp_af_firpeak_coring o_isp_af_firpeak_coring;
    o_isp_af_firpeak_coring.u32 = be_reg->isp_af_firpeak_coring.u32;
    o_isp_af_firpeak_coring.bits.isp_af_fir_peak0_c = uisp_af_fir_peak0_c;
    be_reg->isp_af_firpeak_coring.u32 = o_isp_af_firpeak_coring.u32;
}
/*  description   set the value of the member isp_af_firpeak_coring.isp_af_fir_peak1_c */
/*  input         unsigned int uisp_af_fir_peak1_c  11 bits */
static __inline td_void isp_af_fir_peak1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_peak1_c)
{
    u_isp_af_firpeak_coring o_isp_af_firpeak_coring;
    o_isp_af_firpeak_coring.u32 = be_reg->isp_af_firpeak_coring.u32;
    o_isp_af_firpeak_coring.bits.isp_af_fir_peak1_c = uisp_af_fir_peak1_c;
    be_reg->isp_af_firpeak_coring.u32 = o_isp_af_firpeak_coring.u32;
}
/*  description   set the value of the member isp_af_firslope_coring.isp_af_fir_slope0_c */
/*  input         unsigned int uisp_af_fir_slope0_c  4 bits */
static __inline td_void isp_af_fir_slope0_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_slope0_c)
{
    u_isp_af_firslope_coring o_isp_af_firslope_coring;
    o_isp_af_firslope_coring.u32 = be_reg->isp_af_firslope_coring.u32;
    o_isp_af_firslope_coring.bits.isp_af_fir_slope0_c = uisp_af_fir_slope0_c;
    be_reg->isp_af_firslope_coring.u32 = o_isp_af_firslope_coring.u32;
}
/*  description   set the value of the member isp_af_firslope_coring.isp_af_fir_slope1_c */
/*  input         unsigned int uisp_af_fir_slope1_c  4 bits */
static __inline td_void isp_af_fir_slope1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_af_fir_slope1_c)
{
    u_isp_af_firslope_coring o_isp_af_firslope_coring;
    o_isp_af_firslope_coring.u32 = be_reg->isp_af_firslope_coring.u32;
    o_isp_af_firslope_coring.bits.isp_af_fir_slope1_c = uisp_af_fir_slope1_c;
    be_reg->isp_af_firslope_coring.u32 = o_isp_af_firslope_coring.u32;
}
/*  description   set the value of the member isp_af_highlight.isp_af_highlight */
/*  input         unsigned int uisp_af_highlight  8 bits */
static __inline td_void isp_af_highlight_write(isp_be_reg_type *be_reg, td_u32 uisp_af_highlight)
{
    u_isp_af_hilight o_isp_af_highlight;
    o_isp_af_highlight.u32 = be_reg->isp_af_hilight.u32;
    o_isp_af_highlight.bits.isp_af_hilight = uisp_af_highlight;
    be_reg->isp_af_hilight.u32 = o_isp_af_highlight.u32;
}
/*  description   set the value of the member isp_af_offset.isp_af_offset_gr */
/*  input         unsigned int uisp_af_offset_gr  15 bits */
static __inline td_void isp_af_offset_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_af_offset_gr)
{
    u_isp_af_offset o_isp_af_offset;
    o_isp_af_offset.u32 = be_reg->isp_af_offset.u32;
    o_isp_af_offset.bits.isp_af_offset_gr = uisp_af_offset_gr;
    be_reg->isp_af_offset.u32 = o_isp_af_offset.u32;
}
/*  description   set the value of the member isp_af_offset.isp_af_offset_gb */
/*  input         unsigned int uisp_af_offset_gb  15 bits */
static __inline td_void isp_af_offset_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_af_offset_gb)
{
    u_isp_af_offset o_isp_af_offset;
    o_isp_af_offset.u32 = be_reg->isp_af_offset.u32;
    o_isp_af_offset.bits.isp_af_offset_gb = uisp_af_offset_gb;
    be_reg->isp_af_offset.u32 = o_isp_af_offset.u32;
}

/*  description : set the value of the member isp_dis_cfg.isp_dis_blc_en */
/*  input       : unsigned int uisp_dis_blc_en: 1 bits */
static __inline td_void isp_dis_blc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_blc_en)
{
    u_isp_dis_cfg o_isp_dis_cfg;
    o_isp_dis_cfg.u32 = be_reg->isp_dis_cfg.u32;
    o_isp_dis_cfg.bits.isp_dis_blc_en = uisp_dis_blc_en;
    be_reg->isp_dis_cfg.u32 = o_isp_dis_cfg.u32;
}

/*  description : set the value of the member isp_dis_blk.isp_dis_blk_size */
/*  input       : unsigned int uisp_dis_blk_size: 8 bits */
static __inline td_void isp_dis_blk_size_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_blk_size)
{
    u_isp_dis_blk o_isp_dis_blk;
    o_isp_dis_blk.u32 = be_reg->isp_dis_blk.u32;
    o_isp_dis_blk.bits.isp_dis_blk_size = uisp_dis_blk_size;
    be_reg->isp_dis_blk.u32 = o_isp_dis_blk.u32;
}

/*  description : set the value of the member isp_dis_blk.isp_dis_srch_range */
/*  input       : unsigned int uisp_dis_srch_range: 7 bits */
static __inline td_void isp_dis_srch_range_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_srch_range)
{
    u_isp_dis_blk o_isp_dis_blk;
    o_isp_dis_blk.u32 = be_reg->isp_dis_blk.u32;
    o_isp_dis_blk.bits.isp_dis_srch_range = uisp_dis_srch_range;
    be_reg->isp_dis_blk.u32 = o_isp_dis_blk.u32;
}

/*  description : set the value of the member isp_dis_v0pos.isp_dis_v0pos_hor */
/*  input       : unsigned int uisp_dis_v0pos_hor: 10 bits */
static __inline td_void isp_dis_v0pos_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v0pos_hor)
{
    u_isp_dis_v0pos o_isp_dis_v0pos;
    o_isp_dis_v0pos.u32 = be_reg->isp_dis_v0pos.u32;
    o_isp_dis_v0pos.bits.isp_dis_v0pos_hor = uisp_dis_v0pos_hor;
    be_reg->isp_dis_v0pos.u32 = o_isp_dis_v0pos.u32;
}

/*  description : set the value of the member isp_dis_v0pos.isp_dis_v0pos_ver */
/*  input       : unsigned int uisp_dis_v0pos_ver: 10 bits */
static __inline td_void isp_dis_v0pos_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v0pos_ver)
{
    u_isp_dis_v0pos o_isp_dis_v0pos;
    o_isp_dis_v0pos.u32 = be_reg->isp_dis_v0pos.u32;
    o_isp_dis_v0pos.bits.isp_dis_v0pos_ver = uisp_dis_v0pos_ver;
    be_reg->isp_dis_v0pos.u32 = o_isp_dis_v0pos.u32;
}

/*  description : set the value of the member isp_dis_v4pos.isp_dis_v4pos_hor */
/*  input       : unsigned int uisp_dis_v4pos_hor: 10 bits */
static __inline td_void isp_dis_v4pos_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v4pos_hor)
{
    u_isp_dis_v4pos o_isp_dis_v4pos;
    o_isp_dis_v4pos.u32 = be_reg->isp_dis_v4pos.u32;
    o_isp_dis_v4pos.bits.isp_dis_v4pos_hor = uisp_dis_v4pos_hor;
    be_reg->isp_dis_v4pos.u32 = o_isp_dis_v4pos.u32;
}

/*  description : set the value of the member isp_dis_v4pos.isp_dis_v4pos_ver */
/*  input       : unsigned int uisp_dis_v4pos_ver: 10 bits */
static __inline td_void isp_dis_v4pos_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v4pos_ver)
{
    u_isp_dis_v4pos o_isp_dis_v4pos;
    o_isp_dis_v4pos.u32 = be_reg->isp_dis_v4pos.u32;
    o_isp_dis_v4pos.bits.isp_dis_v4pos_ver = uisp_dis_v4pos_ver;
    be_reg->isp_dis_v4pos.u32 = o_isp_dis_v4pos.u32;
}

/*  description : set the value of the member isp_dis_v8pos.isp_dis_v8pos_hor */
/*  input       : unsigned int uisp_dis_v8pos_hor: 10 bits */
static __inline td_void isp_dis_v8pos_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v8pos_hor)
{
    u_isp_dis_v8pos o_isp_dis_v8pos;
    o_isp_dis_v8pos.u32 = be_reg->isp_dis_v8pos.u32;
    o_isp_dis_v8pos.bits.isp_dis_v8pos_hor = uisp_dis_v8pos_hor;
    be_reg->isp_dis_v8pos.u32 = o_isp_dis_v8pos.u32;
}

/*  description : set the value of the member isp_dis_v8pos.isp_dis_v8pos_ver */
/*  input       : unsigned int uisp_dis_v8pos_ver: 10 bits */
static __inline td_void isp_dis_v8pos_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v8pos_ver)
{
    u_isp_dis_v8pos o_isp_dis_v8pos;
    o_isp_dis_v8pos.u32 = be_reg->isp_dis_v8pos.u32;
    o_isp_dis_v8pos.bits.isp_dis_v8pos_ver = uisp_dis_v8pos_ver;
    be_reg->isp_dis_v8pos.u32 = o_isp_dis_v8pos.u32;
}

/*  description : set the value of the member isp_dis_v0pose.isp_dis_v0pose_hor */
/*  input       : unsigned int uisp_dis_v0pose_hor: 10 bits */
static __inline td_void isp_dis_v0pose_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v0pose_hor)
{
    u_isp_dis_v0pose o_isp_dis_v0pose;
    o_isp_dis_v0pose.u32 = be_reg->isp_dis_v0pose.u32;
    o_isp_dis_v0pose.bits.isp_dis_v0pose_hor = uisp_dis_v0pose_hor;
    be_reg->isp_dis_v0pose.u32 = o_isp_dis_v0pose.u32;
}

/*  description : set the value of the member isp_dis_v0pose.isp_dis_v0pose_ver */
/*  input       : unsigned int uisp_dis_v0pose_ver: 10 bits */
static __inline td_void isp_dis_v0pose_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v0pose_ver)
{
    u_isp_dis_v0pose o_isp_dis_v0pose;
    o_isp_dis_v0pose.u32 = be_reg->isp_dis_v0pose.u32;
    o_isp_dis_v0pose.bits.isp_dis_v0pose_ver = uisp_dis_v0pose_ver;
    be_reg->isp_dis_v0pose.u32 = o_isp_dis_v0pose.u32;
}

/*  description : set the value of the member isp_dis_v4pose.isp_dis_v4pose_hor */
/*  input       : unsigned int uisp_dis_v4pose_hor: 10 bits */
static __inline td_void isp_dis_v4pose_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v4pose_hor)
{
    u_isp_dis_v4pose o_isp_dis_v4pose;
    o_isp_dis_v4pose.u32 = be_reg->isp_dis_v4pose.u32;
    o_isp_dis_v4pose.bits.isp_dis_v4pose_hor = uisp_dis_v4pose_hor;
    be_reg->isp_dis_v4pose.u32 = o_isp_dis_v4pose.u32;
}

/*  description : set the value of the member isp_dis_v4pose.isp_dis_v4pose_ver */
/*  input       : unsigned int uisp_dis_v4pose_ver: 10 bits */
static __inline td_void isp_dis_v4pose_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v4pose_ver)
{
    u_isp_dis_v4pose o_isp_dis_v4pose;
    o_isp_dis_v4pose.u32 = be_reg->isp_dis_v4pose.u32;
    o_isp_dis_v4pose.bits.isp_dis_v4pose_ver = uisp_dis_v4pose_ver;
    be_reg->isp_dis_v4pose.u32 = o_isp_dis_v4pose.u32;
}

/*  description : set the value of the member isp_dis_v8pose.isp_dis_v8pose_hor */
/*  input       : unsigned int uisp_dis_v8pose_hor: 10 bits */
static __inline td_void isp_dis_v8pose_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v8pose_hor)
{
    u_isp_dis_v8pose o_isp_dis_v8pose;
    o_isp_dis_v8pose.u32 = be_reg->isp_dis_v8pose.u32;
    o_isp_dis_v8pose.bits.isp_dis_v8pose_hor = uisp_dis_v8pose_hor;
    be_reg->isp_dis_v8pose.u32 = o_isp_dis_v8pose.u32;
}

/*  description : set the value of the member isp_dis_v8pose.isp_dis_v8pose_ver */
/*  input       : unsigned int uisp_dis_v8pose_ver: 10 bits */
static __inline td_void isp_dis_v8pose_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v8pose_ver)
{
    u_isp_dis_v8pose o_isp_dis_v8pose;
    o_isp_dis_v8pose.u32 = be_reg->isp_dis_v8pose.u32;
    o_isp_dis_v8pose.bits.isp_dis_v8pose_ver = uisp_dis_v8pose_ver;
    be_reg->isp_dis_v8pose.u32 = o_isp_dis_v8pose.u32;
}

/*  description : set the value of the member isp_dis_h0pos.isp_dis_h0pos_hor */
/*  input       : unsigned int uisp_dis_h0pos_hor: 10 bits */
static __inline td_void isp_dis_h0pos_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h0pos_hor)
{
    u_isp_dis_h0pos o_isp_dis_h0pos;
    o_isp_dis_h0pos.u32 = be_reg->isp_dis_h0pos.u32;
    o_isp_dis_h0pos.bits.isp_dis_h0pos_hor = uisp_dis_h0pos_hor;
    be_reg->isp_dis_h0pos.u32 = o_isp_dis_h0pos.u32;
}

/*  description : set the value of the member isp_dis_h0pos.isp_dis_h0pos_ver */
/*  input       : unsigned int uisp_dis_h0pos_ver: 10 bits */
static __inline td_void isp_dis_h0pos_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h0pos_ver)
{
    u_isp_dis_h0pos o_isp_dis_h0pos;
    o_isp_dis_h0pos.u32 = be_reg->isp_dis_h0pos.u32;
    o_isp_dis_h0pos.bits.isp_dis_h0pos_ver = uisp_dis_h0pos_ver;
    be_reg->isp_dis_h0pos.u32 = o_isp_dis_h0pos.u32;
}

/*  description : set the value of the member isp_dis_h4pos.isp_dis_h4pos_hor */
/*  input       : unsigned int uisp_dis_h4pos_hor: 10 bits */
static __inline td_void isp_dis_h4pos_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h4pos_hor)
{
    u_isp_dis_h4pos o_isp_dis_h4pos;
    o_isp_dis_h4pos.u32 = be_reg->isp_dis_h4pos.u32;
    o_isp_dis_h4pos.bits.isp_dis_h4pos_hor = uisp_dis_h4pos_hor;
    be_reg->isp_dis_h4pos.u32 = o_isp_dis_h4pos.u32;
}

/*  description : set the value of the member isp_dis_h4pos.isp_dis_h4pos_ver */
/*  input       : unsigned int uisp_dis_h4pos_ver: 10 bits */
static __inline td_void isp_dis_h4pos_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h4pos_ver)
{
    u_isp_dis_h4pos o_isp_dis_h4pos;
    o_isp_dis_h4pos.u32 = be_reg->isp_dis_h4pos.u32;
    o_isp_dis_h4pos.bits.isp_dis_h4pos_ver = uisp_dis_h4pos_ver;
    be_reg->isp_dis_h4pos.u32 = o_isp_dis_h4pos.u32;
}

/*  description : set the value of the member isp_dis_h8pos.isp_dis_h8pos_hor */
/*  input       : unsigned int uisp_dis_h8pos_hor: 10 bits */
static __inline td_void isp_dis_h8pos_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h8pos_hor)
{
    u_isp_dis_h8pos o_isp_dis_h8pos;
    o_isp_dis_h8pos.u32 = be_reg->isp_dis_h8pos.u32;
    o_isp_dis_h8pos.bits.isp_dis_h8pos_hor = uisp_dis_h8pos_hor;
    be_reg->isp_dis_h8pos.u32 = o_isp_dis_h8pos.u32;
}

/*  description : set the value of the member isp_dis_h8pos.isp_dis_h8pos_ver */
/*  input       : unsigned int uisp_dis_h8pos_ver: 10 bits */
static __inline td_void isp_dis_h8pos_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h8pos_ver)
{
    u_isp_dis_h8pos o_isp_dis_h8pos;
    o_isp_dis_h8pos.u32 = be_reg->isp_dis_h8pos.u32;
    o_isp_dis_h8pos.bits.isp_dis_h8pos_ver = uisp_dis_h8pos_ver;
    be_reg->isp_dis_h8pos.u32 = o_isp_dis_h8pos.u32;
}

/*  description : set the value of the member isp_dis_h0pose.isp_dis_h0pose_hor */
/*  input       : unsigned int uisp_dis_h0pose_hor: 10 bits */
static __inline td_void isp_dis_h0pose_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h0pose_hor)
{
    u_isp_dis_h0pose o_isp_dis_h0pose;
    o_isp_dis_h0pose.u32 = be_reg->isp_dis_h0pose.u32;
    o_isp_dis_h0pose.bits.isp_dis_h0pose_hor = uisp_dis_h0pose_hor;
    be_reg->isp_dis_h0pose.u32 = o_isp_dis_h0pose.u32;
}

/*  description : set the value of the member isp_dis_h0pose.isp_dis_h0pose_ver */
/*  input       : unsigned int uisp_dis_h0pose_ver: 10 bits */
static __inline td_void isp_dis_h0pose_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h0pose_ver)
{
    u_isp_dis_h0pose o_isp_dis_h0pose;
    o_isp_dis_h0pose.u32 = be_reg->isp_dis_h0pose.u32;
    o_isp_dis_h0pose.bits.isp_dis_h0pose_ver = uisp_dis_h0pose_ver;
    be_reg->isp_dis_h0pose.u32 = o_isp_dis_h0pose.u32;
}

/*  description : set the value of the member isp_dis_h4pose.isp_dis_h4pose_hor */
/*  input       : unsigned int uisp_dis_h4pose_hor: 10 bits */
static __inline td_void isp_dis_h4pose_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h4pose_hor)
{
    u_isp_dis_h4pose o_isp_dis_h4pose;
    o_isp_dis_h4pose.u32 = be_reg->isp_dis_h4pose.u32;
    o_isp_dis_h4pose.bits.isp_dis_h4pose_hor = uisp_dis_h4pose_hor;
    be_reg->isp_dis_h4pose.u32 = o_isp_dis_h4pose.u32;
}

/*  description : set the value of the member isp_dis_h4pose.isp_dis_h4pose_ver */
/*  input       : unsigned int uisp_dis_h4pose_ver: 10 bits */
static __inline td_void isp_dis_h4pose_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h4pose_ver)
{
    u_isp_dis_h4pose o_isp_dis_h4pose;
    o_isp_dis_h4pose.u32 = be_reg->isp_dis_h4pose.u32;
    o_isp_dis_h4pose.bits.isp_dis_h4pose_ver = uisp_dis_h4pose_ver;
    be_reg->isp_dis_h4pose.u32 = o_isp_dis_h4pose.u32;
}

/*  description : set the value of the member isp_dis_h8pose.isp_dis_h8pose_hor */
/*  input       : unsigned int uisp_dis_h8pose_hor: 10 bits */
static __inline td_void isp_dis_h8pose_hor_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h8pose_hor)
{
    u_isp_dis_h8pose o_isp_dis_h8pose;
    o_isp_dis_h8pose.u32 = be_reg->isp_dis_h8pose.u32;
    o_isp_dis_h8pose.bits.isp_dis_h8pose_hor = uisp_dis_h8pose_hor;
    be_reg->isp_dis_h8pose.u32 = o_isp_dis_h8pose.u32;
}

/*  description : set the value of the member isp_dis_h8pose.isp_dis_h8pose_ver */
/*  input       : unsigned int uisp_dis_h8pose_ver: 10 bits */
static __inline td_void isp_dis_h8pose_ver_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h8pose_ver)
{
    u_isp_dis_h8pose o_isp_dis_h8pose;
    o_isp_dis_h8pose.u32 = be_reg->isp_dis_h8pose.u32;
    o_isp_dis_h8pose.bits.isp_dis_h8pose_ver = uisp_dis_h8pose_ver;
    be_reg->isp_dis_h8pose.u32 = o_isp_dis_h8pose.u32;
}

/*  description : set the value of the member isp_dis_raw_luma.isp_dis_raw_luma */
/*  input       : unsigned int uisp_dis_raw_luma: 1 bits */
static __inline td_void isp_dis_raw_luma_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_raw_luma)
{
    u_isp_dis_raw_luma o_isp_dis_raw_luma;
    o_isp_dis_raw_luma.u32 = be_reg->isp_dis_raw_luma.u32;
    o_isp_dis_raw_luma.bits.isp_dis_raw_luma = uisp_dis_raw_luma;
    be_reg->isp_dis_raw_luma.u32 = o_isp_dis_raw_luma.u32;
}

/*  description : set the value of the member isp_dis_gamma_en.isp_dis_gamma_en */
/*  input       : unsigned int uisp_dis_gamma_en: 1 bits */
static __inline td_void isp_dis_gamma_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_gamma_en)
{
    u_isp_dis_gamma_en o_isp_dis_gamma_en;
    o_isp_dis_gamma_en.u32 = be_reg->isp_dis_gamma_en.u32;
    o_isp_dis_gamma_en.bits.isp_dis_gamma_en = uisp_dis_gamma_en;
    be_reg->isp_dis_gamma_en.u32 = o_isp_dis_gamma_en.u32;
}

/*  description : set the value of the member isp_dis_offset_gr.isp_dis_offset_gr */
/*  input       : unsigned int uisp_dis_offset_gr: 20 bits */
static __inline td_void isp_dis_offset_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_offset_gr)
{
    u_isp_dis_offset_gr o_isp_dis_offset_gr;
    o_isp_dis_offset_gr.u32 = be_reg->isp_dis_offset_gr.u32;
    o_isp_dis_offset_gr.bits.isp_dis_offset_gr = uisp_dis_offset_gr;
    be_reg->isp_dis_offset_gr.u32 = o_isp_dis_offset_gr.u32;
}

/*  description : set the value of the member isp_dis_offset_gb.isp_dis_offset_gb */
/*  input       : unsigned int uisp_dis_offset_gb: 20 bits */
static __inline td_void isp_dis_offset_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_offset_gb)
{
    u_isp_dis_offset_gb o_isp_dis_offset_gb;
    o_isp_dis_offset_gb.u32 = be_reg->isp_dis_offset_gb.u32;
    o_isp_dis_offset_gb.bits.isp_dis_offset_gb = uisp_dis_offset_gb;
    be_reg->isp_dis_offset_gb.u32 = o_isp_dis_offset_gb.u32;
}

/*  description : set the value of the member isp_dis_scale.isp_dis_hor_scale */
/*  input       : unsigned int uisp_dis_hor_scale: 2 bits */
static __inline td_void isp_dis_hor_scale_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_hor_scale)
{
    u_isp_dis_scale o_isp_dis_scale;
    o_isp_dis_scale.u32 = be_reg->isp_dis_scale.u32;
    o_isp_dis_scale.bits.isp_dis_hor_scale = uisp_dis_hor_scale;
    be_reg->isp_dis_scale.u32 = o_isp_dis_scale.u32;
}

/*  description : set the value of the member isp_dis_scale.isp_dis_ver_scale */
/*  input       : unsigned int uisp_dis_ver_scale: 2 bits */
static __inline td_void isp_dis_ver_scale_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_ver_scale)
{
    u_isp_dis_scale o_isp_dis_scale;
    o_isp_dis_scale.u32 = be_reg->isp_dis_scale.u32;
    o_isp_dis_scale.bits.isp_dis_ver_scale = uisp_dis_ver_scale;
    be_reg->isp_dis_scale.u32 = o_isp_dis_scale.u32;
}

/*  description : set the value of the member isp_dis_refinfo_update.isp_dis_refinfo_update */
/*  input       : unsigned int uisp_dis_refinfo_update: 1 bits */
static __inline td_void isp_dis_refinfo_update_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_refinfo_update)
{
    u_isp_dis_refinfo_update o_isp_dis_refinfo_update;
    o_isp_dis_refinfo_update.u32 = be_reg->isp_dis_refinfo_update.u32;
    o_isp_dis_refinfo_update.bits.isp_dis_refinfo_update = uisp_dis_refinfo_update;
    be_reg->isp_dis_refinfo_update.u32 = o_isp_dis_refinfo_update.u32;
}

/*  description : set the value of the member isp_dis_prj_shift.isp_dis_h_prj_shift_bits */
/*  input       : unsigned int uisp_dis_h_prj_shift_bits: 4 bits */
static __inline td_void isp_dis_h_prj_shift_bits_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_h_prj_shift_bits)
{
    u_isp_dis_prj_shift o_isp_dis_prj_shift;
    o_isp_dis_prj_shift.u32 = be_reg->isp_dis_prj_shift.u32;
    o_isp_dis_prj_shift.bits.isp_dis_h_prj_shift_bits = uisp_dis_h_prj_shift_bits;
    be_reg->isp_dis_prj_shift.u32 = o_isp_dis_prj_shift.u32;
}

/*  description : set the value of the member isp_dis_prj_shift.isp_dis_v_prj_shift_bits */
/*  input       : unsigned int uisp_dis_v_prj_shift_bits: 4 bits */
static __inline td_void isp_dis_v_prj_shift_bits_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_v_prj_shift_bits)
{
    u_isp_dis_prj_shift o_isp_dis_prj_shift;
    o_isp_dis_prj_shift.u32 = be_reg->isp_dis_prj_shift.u32;
    o_isp_dis_prj_shift.bits.isp_dis_v_prj_shift_bits = uisp_dis_v_prj_shift_bits;
    be_reg->isp_dis_prj_shift.u32 = o_isp_dis_prj_shift.u32;
}

/*  description : set the value of the member isp_dis_stat_raddr.isp_dis_stat_raddr */
/*  input       : unsigned int uisp_dis_stat_raddr: 32 bits */
static __inline td_void isp_dis_stat_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_stat_raddr)
{
    be_reg->isp_dis_stat_raddr.u32 = uisp_dis_stat_raddr;
}


/*  description : set the value of the member isp_dis_refinfo_waddr.isp_dis_refinfo_waddr */
/*  input       : unsigned int uisp_dis_refinfo_waddr: 32 bits */
static __inline td_void isp_dis_refinfo_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_refinfo_waddr)
{
    be_reg->isp_dis_refinfo_waddr.u32 = uisp_dis_refinfo_waddr;
}


/*  description : set the value of the member isp_dis_refinfo_wdata.isp_dis_refinfo_wdata */
/*  input       : unsigned int uisp_dis_refinfo_wdata: 32 bits */
static __inline td_void isp_dis_refinfo_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_refinfo_wdata)
{
    be_reg->isp_dis_refinfo_wdata.u32 = uisp_dis_refinfo_wdata;
}


/*  description : set the value of the member isp_dis_refinfo_raddr.isp_dis_refinfo_raddr */
/*  input       : unsigned int uisp_dis_refinfo_raddr: 32 bits */
static __inline td_void isp_dis_refinfo_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dis_refinfo_raddr)
{
    be_reg->isp_dis_refinfo_raddr.u32 = uisp_dis_refinfo_raddr;
}

/*  description   set the value of the member isp_la_zone.isp_la_hnum */
/*  input         unsigned int uisp_la_hnum  5 bits */
static __inline td_void isp_la_hnum_write(isp_be_reg_type *be_reg, td_u32 uisp_la_hnum)
{
    u_isp_la_zone o_isp_la_zone;
    o_isp_la_zone.u32 = be_reg->isp_la_zone.u32;
    o_isp_la_zone.bits.isp_la_hnum = uisp_la_hnum;
    be_reg->isp_la_zone.u32 = o_isp_la_zone.u32;
}
/*  description   set the value of the member isp_la_zone.isp_la_vnum */
/*  input         unsigned int uisp_la_vnum  5 bits */
static __inline td_void isp_la_vnum_write(isp_be_reg_type *be_reg, td_u32 uisp_la_vnum)
{
    u_isp_la_zone o_isp_la_zone;
    o_isp_la_zone.u32 = be_reg->isp_la_zone.u32;
    o_isp_la_zone.bits.isp_la_vnum = uisp_la_vnum;
    be_reg->isp_la_zone.u32 = o_isp_la_zone.u32;
}
/*  description   set the value of the member isp_la_bitmove.isp_la_bitmove */
/*  input         unsigned int uisp_la_bitmove  5 bits */
static __inline td_void isp_la_bitmove_write(isp_be_reg_type *be_reg, td_u32 uisp_la_bitmove)
{
    u_isp_la_bitmove o_isp_la_bitmove;
    o_isp_la_bitmove.u32 = be_reg->isp_la_bitmove.u32;
    o_isp_la_bitmove.bits.isp_la_bitmove = uisp_la_bitmove;
    be_reg->isp_la_bitmove.u32 = o_isp_la_bitmove.u32;
}
/*  description   set the value of the member isp_la_bitmove.isp_la_gamma_en */
/*  input         unsigned int uisp_la_gamma_en  1 bits */
static __inline td_void isp_la_gamma_en_write(isp_be_reg_type *be_reg, td_u32 uisp_la_gamma_en)
{
    u_isp_la_bitmove o_isp_la_bitmove;
    o_isp_la_bitmove.u32 = be_reg->isp_la_bitmove.u32;
    o_isp_la_bitmove.bits.isp_la_gamma_en = uisp_la_gamma_en;
    be_reg->isp_la_bitmove.u32 = o_isp_la_bitmove.u32;
}
/*  description   set the value of the member isp_la_bitmove.isp_la_blc_en */
/*  input         unsigned int uisp_la_blc_en  1 bits */
static __inline td_void isp_la_blc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_la_blc_en)
{
    u_isp_la_bitmove o_isp_la_bitmove;
    o_isp_la_bitmove.u32 = be_reg->isp_la_bitmove.u32;
    o_isp_la_bitmove.bits.isp_la_blc_en = uisp_la_blc_en;
    be_reg->isp_la_bitmove.u32 = o_isp_la_bitmove.u32;
}
/*  description   set the value of the member isp_la_offset_r.isp_la_offset_r */
/*  input         unsigned int uisp_la_offset_r  15 bits */
static __inline td_void isp_la_offset_r_write(isp_be_reg_type *be_reg, td_u32 uisp_la_offset_r)
{
    u_isp_la_offset_r o_isp_la_offset_r;
    o_isp_la_offset_r.u32 = be_reg->isp_la_offset_r.u32;
    o_isp_la_offset_r.bits.isp_la_offset_r = uisp_la_offset_r;
    be_reg->isp_la_offset_r.u32 = o_isp_la_offset_r.u32;
}
/*  description   set the value of the member isp_la_offset_gr.isp_la_offset_gr */
/*  input         unsigned int uisp_la_offset_gr  15 bits */
static __inline td_void isp_la_offset_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_la_offset_gr)
{
    u_isp_la_offset_gr o_isp_la_offset_gr;
    o_isp_la_offset_gr.u32 = be_reg->isp_la_offset_gr.u32;
    o_isp_la_offset_gr.bits.isp_la_offset_gr = uisp_la_offset_gr;
    be_reg->isp_la_offset_gr.u32 = o_isp_la_offset_gr.u32;
}
/*  description   set the value of the member isp_la_offset_gb.isp_la_offset_gb */
/*  input         unsigned int uisp_la_offset_gb  15 bits */
static __inline td_void isp_la_offset_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_la_offset_gb)
{
    u_isp_la_offset_gb o_isp_la_offset_gb;
    o_isp_la_offset_gb.u32 = be_reg->isp_la_offset_gb.u32;
    o_isp_la_offset_gb.bits.isp_la_offset_gb = uisp_la_offset_gb;
    be_reg->isp_la_offset_gb.u32 = o_isp_la_offset_gb.u32;
}
/*  description   set the value of the member isp_la_offset_b.isp_la_offset_b */
/*  input         unsigned int uisp_la_offset_b  15 bits */
static __inline td_void isp_la_offset_b_write(isp_be_reg_type *be_reg, td_u32 uisp_la_offset_b)
{
    u_isp_la_offset_b o_isp_la_offset_b;
    o_isp_la_offset_b.u32 = be_reg->isp_la_offset_b.u32;
    o_isp_la_offset_b.bits.isp_la_offset_b = uisp_la_offset_b;
    be_reg->isp_la_offset_b.u32 = o_isp_la_offset_b.u32;
}
/*  description   set the value of the member isp_la_gamma_limit.isp_la_gamma_limit */
/*  input         unsigned int uisp_la_gamma_limit  4 bits */
static __inline td_void isp_la_gamma_limit_write(isp_be_reg_type *be_reg, td_u32 uisp_la_gamma_limit)
{
    u_isp_la_gamma_limit o_isp_la_gamma_limit;
    o_isp_la_gamma_limit.u32 = be_reg->isp_la_gamma_limit.u32;
    o_isp_la_gamma_limit.bits.isp_la_gamma_limit = uisp_la_gamma_limit;
    be_reg->isp_la_gamma_limit.u32 = o_isp_la_gamma_limit.u32;
}
/*  description   set the value of the member isp_la_crop_pos.isp_la_crop_pos_x */
/*  input         unsigned int uisp_la_crop_pos_x  16 bits */
static __inline td_void isp_la_crop_pos_x_write(isp_be_reg_type *be_reg, td_u32 uisp_la_crop_pos_x)
{
    u_isp_la_crop_pos o_isp_la_crop_pos;
    o_isp_la_crop_pos.u32 = be_reg->isp_la_crop_pos.u32;
    o_isp_la_crop_pos.bits.isp_la_crop_pos_x = uisp_la_crop_pos_x;
    be_reg->isp_la_crop_pos.u32 = o_isp_la_crop_pos.u32;
}
/*  description   set the value of the member isp_la_crop_pos.isp_la_crop_pos_y */
/*  input         unsigned int uisp_la_crop_pos_y  16 bits */
static __inline td_void isp_la_crop_pos_y_write(isp_be_reg_type *be_reg, td_u32 uisp_la_crop_pos_y)
{
    u_isp_la_crop_pos o_isp_la_crop_pos;
    o_isp_la_crop_pos.u32 = be_reg->isp_la_crop_pos.u32;
    o_isp_la_crop_pos.bits.isp_la_crop_pos_y = uisp_la_crop_pos_y;
    be_reg->isp_la_crop_pos.u32 = o_isp_la_crop_pos.u32;
}
/*  description   set the value of the member isp_la_crop_outsize.isp_la_crop_out_width */
/*  input         unsigned int uisp_la_crop_out_width  16 bits */
static __inline td_void isp_la_crop_out_width_write(isp_be_reg_type *be_reg, td_u32 uisp_la_crop_out_width)
{
    u_isp_la_crop_outsize o_isp_la_crop_outsize;
    o_isp_la_crop_outsize.u32 = be_reg->isp_la_crop_outsize.u32;
    o_isp_la_crop_outsize.bits.isp_la_crop_out_width = uisp_la_crop_out_width;
    be_reg->isp_la_crop_outsize.u32 = o_isp_la_crop_outsize.u32;
}
/*  description   set the value of the member isp_la_crop_outsize.isp_la_crop_out_height */
/*  input         unsigned int uisp_la_crop_out_height  16 bits */
static __inline td_void isp_la_crop_out_height_write(isp_be_reg_type *be_reg, td_u32 uisp_la_crop_out_height)
{
    u_isp_la_crop_outsize o_isp_la_crop_outsize;
    o_isp_la_crop_outsize.u32 = be_reg->isp_la_crop_outsize.u32;
    o_isp_la_crop_outsize.bits.isp_la_crop_out_height = uisp_la_crop_out_height;
    be_reg->isp_la_crop_outsize.u32 = o_isp_la_crop_outsize.u32;
}
/*  description   set the value of the member isp_la_stt_bst.isp_la_stt_size */
/*  input         unsigned int uisp_la_stt_size  16 bits */
static __inline td_void isp_la_stt_size_write(isp_be_reg_type *be_reg, td_u32 uisp_la_stt_size)
{
    u_isp_la_stt_bst o_isp_la_stt_bst;
    o_isp_la_stt_bst.u32 = be_reg->isp_la_stt_bst.u32;
    o_isp_la_stt_bst.bits.isp_la_stt_size = uisp_la_stt_size;
    be_reg->isp_la_stt_bst.u32 = o_isp_la_stt_bst.u32;
}
/*  description   set the value of the member isp_la_stt_bst.isp_la_stt_bst */
/*  input         unsigned int uisp_la_stt_bst  4 bits */
static __inline td_void isp_la_stt_bst_write(isp_be_reg_type *be_reg, td_u32 uisp_la_stt_bst)
{
    u_isp_la_stt_bst o_isp_la_stt_bst;
    o_isp_la_stt_bst.u32 = be_reg->isp_la_stt_bst.u32;
    o_isp_la_stt_bst.bits.isp_la_stt_bst = uisp_la_stt_bst;
    be_reg->isp_la_stt_bst.u32 = o_isp_la_stt_bst.u32;
}
/*  description   set the value of the member isp_la_stt_bst.isp_la_stt_en */
/*  input         unsigned int uisp_la_stt_en  1 bits */
static __inline td_void isp_la_stt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_la_stt_en)
{
    u_isp_la_stt_bst o_isp_la_stt_bst;
    o_isp_la_stt_bst.u32 = be_reg->isp_la_stt_bst.u32;
    o_isp_la_stt_bst.bits.isp_la_stt_en = uisp_la_stt_en;
    be_reg->isp_la_stt_bst.u32 = o_isp_la_stt_bst.u32;
}
/*  description   set the value of the member isp_la_stt_abn.isp_la_stt_clr */
/*  input         unsigned int uisp_la_stt_clr  1 bits */
static __inline td_void isp_la_stt_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_la_stt_clr)
{
    u_isp_la_stt_abn o_isp_la_stt_abn;
    o_isp_la_stt_abn.u32 = be_reg->isp_la_stt_abn.u32;
    o_isp_la_stt_abn.bits.isp_la_stt_clr = uisp_la_stt_clr;
    be_reg->isp_la_stt_abn.u32 = o_isp_la_stt_abn.u32;
}
/*  description   set the value of the member isp_la_aver_raddr.isp_la_aver_raddr */
/*  input         unsigned int uisp_la_aver_raddr  32 bits */
static __inline td_void isp_la_aver_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_la_aver_raddr)
{
    be_reg->isp_la_aver_raddr.u32 = uisp_la_aver_raddr;
}

/*  description   set the value of the member isp_lsc_mesh.isp_lsc_mesh_str */
/*  input         unsigned int uisp_lsc_mesh_str  10 bits */
static __inline td_void isp_lsc_mesh_str_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_mesh_str)
{
    u_isp_lsc_mesh o_isp_lsc_mesh;
    o_isp_lsc_mesh.u32 = be_reg->isp_lsc_mesh.u32;
    o_isp_lsc_mesh.bits.isp_lsc_mesh_str = uisp_lsc_mesh_str;
    be_reg->isp_lsc_mesh.u32 = o_isp_lsc_mesh.u32;
}
/*  description   set the value of the member isp_lsc_mesh.isp_lsc_mesh_scale */
/*  input         unsigned int uisp_lsc_mesh_scale  3 bits */
static __inline td_void isp_lsc_mesh_scale_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_mesh_scale)
{
    u_isp_lsc_mesh o_isp_lsc_mesh;
    o_isp_lsc_mesh.u32 = be_reg->isp_lsc_mesh.u32;
    o_isp_lsc_mesh.bits.isp_lsc_mesh_scale = uisp_lsc_mesh_scale;
    be_reg->isp_lsc_mesh.u32 = o_isp_lsc_mesh.u32;
}
/*  description   set the value of the member isp_lsc_blcen.isp_lsc_blc_in_en */
/*  input         unsigned int uisp_lsc_blc_in_en  1 bits */
static __inline td_void isp_lsc_blc_in_en_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_in_en)
{
    u_isp_lsc_blcen o_isp_lsc_blcen;
    o_isp_lsc_blcen.u32 = be_reg->isp_lsc_blcen.u32;
    o_isp_lsc_blcen.bits.isp_lsc_blc_in_en = uisp_lsc_blc_in_en;
    be_reg->isp_lsc_blcen.u32 = o_isp_lsc_blcen.u32;
}
/*  description   set the value of the member isp_lsc_blcen.isp_lsc_blc_out_en */
/*  input         unsigned int uisp_lsc_blc_out_en  1 bits */
static __inline td_void isp_lsc_blc_out_en_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_out_en)
{
    u_isp_lsc_blcen o_isp_lsc_blcen;
    o_isp_lsc_blcen.u32 = be_reg->isp_lsc_blcen.u32;
    o_isp_lsc_blcen.bits.isp_lsc_blc_out_en = uisp_lsc_blc_out_en;
    be_reg->isp_lsc_blcen.u32 = o_isp_lsc_blcen.u32;
}
/*  description   set the value of the member isp_lsc_blc0.isp_lsc_blc_r */
/*  input         unsigned int uisp_lsc_blc_r  15 bits */
static __inline td_void isp_lsc_blc_r_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_r)
{
    u_isp_lsc_blc0 o_isp_lsc_blc0;
    o_isp_lsc_blc0.u32 = be_reg->isp_lsc_blc0.u32;
    o_isp_lsc_blc0.bits.isp_lsc_blc_r = uisp_lsc_blc_r;
    be_reg->isp_lsc_blc0.u32 = o_isp_lsc_blc0.u32;
}
/*  description   set the value of the member isp_lsc_blc0.isp_lsc_blc_gr */
/*  input         unsigned int uisp_lsc_blc_gr  15 bits */
static __inline td_void isp_lsc_blc_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_gr)
{
    u_isp_lsc_blc0 o_isp_lsc_blc0;
    o_isp_lsc_blc0.u32 = be_reg->isp_lsc_blc0.u32;
    o_isp_lsc_blc0.bits.isp_lsc_blc_gr = uisp_lsc_blc_gr;
    be_reg->isp_lsc_blc0.u32 = o_isp_lsc_blc0.u32;
}
/*  description   set the value of the member isp_lsc_blc1.isp_lsc_blc_b */
/*  input         unsigned int uisp_lsc_blc_b  15 bits */
static __inline td_void isp_lsc_blc_b_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_b)
{
    u_isp_lsc_blc1 o_isp_lsc_blc1;
    o_isp_lsc_blc1.u32 = be_reg->isp_lsc_blc1.u32;
    o_isp_lsc_blc1.bits.isp_lsc_blc_b = uisp_lsc_blc_b;
    be_reg->isp_lsc_blc1.u32 = o_isp_lsc_blc1.u32;
}
/*  description   set the value of the member isp_lsc_blc1.isp_lsc_blc_gb */
/*  input         unsigned int uisp_lsc_blc_gb  15 bits */
static __inline td_void isp_lsc_blc_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_gb)
{
    u_isp_lsc_blc1 o_isp_lsc_blc1;
    o_isp_lsc_blc1.u32 = be_reg->isp_lsc_blc1.u32;
    o_isp_lsc_blc1.bits.isp_lsc_blc_gb = uisp_lsc_blc_gb;
    be_reg->isp_lsc_blc1.u32 = o_isp_lsc_blc1.u32;
}

/*  description : set the value of the member isp_lsc_right_shift.isp_lsc_blc_right_shift */
/*  input       : unsigned int uisp_lsc_blc_right_shift: 4 bits */
static __inline td_void isp_lsc_blc_right_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_blc_right_shift)
{
    u_isp_lsc_right_shift o_isp_lsc_right_shift;
    o_isp_lsc_right_shift.u32 = be_reg->isp_lsc_right_shift.u32;
    o_isp_lsc_right_shift.bits.isp_lsc_blc_right_shift = uisp_lsc_blc_right_shift;
    be_reg->isp_lsc_right_shift.u32 = o_isp_lsc_right_shift.u32;
}

/*  description   set the value of the member isp_ge_blc_en.isp_ge0_blc_offset_en */
/*  input         unsigned int uisp_ge0_blc_offset_en  1 bits */
static __inline td_void isp_ge0_blc_offset_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ge0_blc_offset_en)
{
    u_isp_ge_blc_en o_isp_ge_blc_en;
    o_isp_ge_blc_en.u32 = be_reg->isp_ge_blc_en.u32;
    o_isp_ge_blc_en.bits.isp_ge0_blc_offset_en = uisp_ge0_blc_offset_en;
    be_reg->isp_ge_blc_en.u32 = o_isp_ge_blc_en.u32;
}

/*  description   set the value of the member isp_ge0_ct_th1.isp_ge_ge0_ct_th1 */
/*  input         unsigned int uisp_ge_ge0_ct_th1  16 bits */
#define OT_ISP_GE_NPOFFSET_DEFAULT                    1024
static __inline td_void isp_ge_ge0_ct_th1_write(isp_be_reg_type *be_reg, td_u32 uisp_ge_ge0_ct_th1)
{
    u_isp_ge0_ct_th1 o_isp_ge0_ct_th1;
    o_isp_ge0_ct_th1.u32 = be_reg->isp_ge0_ct_th1.u32;
    o_isp_ge0_ct_th1.bits.isp_ge_ge0_ct_th1 = uisp_ge_ge0_ct_th1;
    be_reg->isp_ge0_ct_th1.u32 = o_isp_ge0_ct_th1.u32;
}
/*  description   set the value of the member isp_ge0_ct_th2.isp_ge_ge0_ct_th2 */
/*  input         unsigned int uisp_ge_ge0_ct_th2  16 bits */
#define OT_ISP_GE_SENSITHR_DEFAULT                    300
static __inline td_void isp_ge_ge0_ct_th2_write(isp_be_reg_type *be_reg, td_u32 uisp_ge_ge0_ct_th2)
{
    u_isp_ge0_ct_th2 o_isp_ge0_ct_th2;
    o_isp_ge0_ct_th2.u32 = be_reg->isp_ge0_ct_th2.u32;
    o_isp_ge0_ct_th2.bits.isp_ge_ge0_ct_th2 = uisp_ge_ge0_ct_th2;
    be_reg->isp_ge0_ct_th2.u32 = o_isp_ge0_ct_th2.u32;
}
/*  description   set the value of the member isp_ge0_ct_th3.isp_ge_ge0_ct_th3 */
/*  input         unsigned int uisp_ge_ge0_ct_th3  16 bits */
#define OT_ISP_GE_THRESHOLD_DEFAULT                   300
static __inline td_void isp_ge_ge0_ct_th3_write(isp_be_reg_type *be_reg, td_u32 uisp_ge_ge0_ct_th3)
{
    u_isp_ge0_ct_th3 o_isp_ge0_ct_th3;
    o_isp_ge0_ct_th3.u32 = be_reg->isp_ge0_ct_th3.u32;
    o_isp_ge0_ct_th3.bits.isp_ge_ge0_ct_th3 = uisp_ge_ge0_ct_th3;
    be_reg->isp_ge0_ct_th3.u32 = o_isp_ge0_ct_th3.u32;
}
/*  description   set the value of the member isp_ge0_ct_slope.isp_ge_ge0_ct_slope1 */
/*  input         unsigned int uisp_ge_ge0_ct_slope1  5 bits */
#define OT_ISP_GE_SENSISLOPE_DEFAULT                  9
static __inline td_void isp_ge_ge0_ct_slope1_write(isp_be_reg_type *be_reg, td_u32 uisp_ge_ge0_ct_slope1)
{
    u_isp_ge0_ct_slope o_isp_ge0_ct_slope;
    o_isp_ge0_ct_slope.u32 = be_reg->isp_ge0_ct_slope.u32;
    o_isp_ge0_ct_slope.bits.isp_ge_ge0_ct_slope1 = uisp_ge_ge0_ct_slope1;
    be_reg->isp_ge0_ct_slope.u32 = o_isp_ge0_ct_slope.u32;
}
/*  description   set the value of the member isp_ge0_ct_slope.isp_ge_ge0_ct_slope2 */
/*  input         unsigned int uisp_ge_ge0_ct_slope2  5 bits */
#define OT_ISP_GE_SLOPE_DEFAULT                       9
static __inline td_void isp_ge_ge0_ct_slope2_write(isp_be_reg_type *be_reg, td_u32 uisp_ge_ge0_ct_slope2)
{
    u_isp_ge0_ct_slope o_isp_ge0_ct_slope;
    o_isp_ge0_ct_slope.u32 = be_reg->isp_ge0_ct_slope.u32;
    o_isp_ge0_ct_slope.bits.isp_ge_ge0_ct_slope2 = uisp_ge_ge0_ct_slope2;
    be_reg->isp_ge0_ct_slope.u32 = o_isp_ge0_ct_slope.u32;
}
/*  description   set the value of the member isp_ge0_blc_offset.isp_ge0_blc_offset_gb */
/*  input         unsigned int uisp_ge0_blc_offset_gb  16 bits */
static __inline td_void isp_ge0_blc_offset_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_ge0_blc_offset_gb)
{
    u_isp_ge0_blc_offset o_isp_ge0_blc_offset;
    o_isp_ge0_blc_offset.u32 = be_reg->isp_ge0_blc_offset.u32;
    o_isp_ge0_blc_offset.bits.isp_ge0_blc_offset_gb = uisp_ge0_blc_offset_gb;
    be_reg->isp_ge0_blc_offset.u32 = o_isp_ge0_blc_offset.u32;
}
/*  description   set the value of the member isp_ge0_blc_offset.isp_ge0_blc_offset_gr */
/*  input         unsigned int uisp_ge0_blc_offset_gr  16 bits */
static __inline td_void isp_ge0_blc_offset_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_ge0_blc_offset_gr)
{
    u_isp_ge0_blc_offset o_isp_ge0_blc_offset;
    o_isp_ge0_blc_offset.u32 = be_reg->isp_ge0_blc_offset.u32;
    o_isp_ge0_blc_offset.bits.isp_ge0_blc_offset_gr = uisp_ge0_blc_offset_gr;
    be_reg->isp_ge0_blc_offset.u32 = o_isp_ge0_blc_offset.u32;
}

/*  description   set the value of the member isp_ge_strength.isp_ge_strength */
/*  input         unsigned int uisp_ge_strength  9 bits */
#define OT_ISP_GE_STRENGTH_DEFAULT                    128
static __inline td_void isp_ge_strength_write(isp_be_reg_type *be_reg, td_u32 uisp_ge_strength)
{
    u_isp_ge_strength o_isp_ge_strength;
    o_isp_ge_strength.u32 = be_reg->isp_ge_strength.u32;
    o_isp_ge_strength.bits.isp_ge_strength = uisp_ge_strength;
    be_reg->isp_ge_strength.u32 = o_isp_ge_strength.u32;
}

/*  description   set the value of the member isp_fpn_cfg.isp_fpn_calib_offline */
/*  input         unsigned int uisp_fpn_calib_offline  1 bits */
static __inline td_void isp_fpn_calib_offline_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_calib_offline)
{
    u_isp_fpn_cfg o_isp_fpn_cfg;
    o_isp_fpn_cfg.u32 = be_reg->isp_fpn_cfg.u32;
    o_isp_fpn_cfg.bits.isp_fpn_calib_offline = uisp_fpn_calib_offline;
    be_reg->isp_fpn_cfg.u32 = o_isp_fpn_cfg.u32;
}
/*  description   set the value of the member isp_fpn_calib_start.isp_fpn_calib_start */
/*  input         unsigned int uisp_fpn_calib_start  1 bits */
static __inline td_void isp_fpn_calib_start_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_calib_start)
{
    u_isp_fpn_calib_start o_isp_fpn_calib_start;
    o_isp_fpn_calib_start.u32 = be_reg->isp_fpn_calib_start.u32;
    o_isp_fpn_calib_start.bits.isp_fpn_calib_start = uisp_fpn_calib_start;
    be_reg->isp_fpn_calib_start.u32 = o_isp_fpn_calib_start.u32;
}
/*  description   set the value of the member isp_fpn_corr_cfg.isp_fpn_correct0_en */
/*  input         unsigned int uisp_fpn_correct0_en  1 bits */
static __inline td_void isp_fpn_correct0_en_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_correct0_en)
{
    u_isp_fpn_corr_cfg o_isp_fpn_corr_cfg;
    o_isp_fpn_corr_cfg.u32 = be_reg->isp_fpn_corr_cfg.u32;
    o_isp_fpn_corr_cfg.bits.isp_fpn_correct0_en = uisp_fpn_correct0_en;
    be_reg->isp_fpn_corr_cfg.u32 = o_isp_fpn_corr_cfg.u32;
}
/*  description   set the value of the member isp_fpn_white_level.isp_fpn_white_level */
/*  input         unsigned int uisp_fpn_white_level  14 bits */
static __inline td_void isp_fpn_white_level_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_white_level)
{
    u_isp_fpn_white_level o_isp_fpn_white_level;
    o_isp_fpn_white_level.u32 = be_reg->isp_fpn_white_level.u32;
    o_isp_fpn_white_level.bits.isp_fpn_white_level = uisp_fpn_white_level;
    be_reg->isp_fpn_white_level.u32 = o_isp_fpn_white_level.u32;
}
/*  description   set the value of the member isp_fpn_divcoef.isp_fpn_divcoef */
/*  input         unsigned int uisp_fpn_divcoef  13 bits */
static __inline td_void isp_fpn_divcoef_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_divcoef)
{
    u_isp_fpn_divcoef o_isp_fpn_divcoef;
    o_isp_fpn_divcoef.u32 = be_reg->isp_fpn_divcoef.u32;
    o_isp_fpn_divcoef.bits.isp_fpn_divcoef = uisp_fpn_divcoef;
    be_reg->isp_fpn_divcoef.u32 = o_isp_fpn_divcoef.u32;
}
/*  description   set the value of the member isp_fpn_framelog2.isp_fpn_framelog2 */
/*  input         unsigned int uisp_fpn_framelog2  3 bits */
static __inline td_void isp_fpn_framelog2_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_framelog2)
{
    u_isp_fpn_framelog2 o_isp_fpn_framelog2;
    o_isp_fpn_framelog2.u32 = be_reg->isp_fpn_framelog2.u32;
    o_isp_fpn_framelog2.bits.isp_fpn_framelog2 = uisp_fpn_framelog2;
    be_reg->isp_fpn_framelog2.u32 = o_isp_fpn_framelog2.u32;
}
/*  description   set the value of the member isp_fpn_corr0.isp_fpn_offset0 */
/*  input         unsigned int uisp_fpn_offset0  12 bits */
static __inline td_void isp_fpn_offset0_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_offset0)
{
    u_isp_fpn_corr0 o_isp_fpn_corr0;
    o_isp_fpn_corr0.u32 = be_reg->isp_fpn_corr0.u32;
    o_isp_fpn_corr0.bits.isp_fpn_offset0 = uisp_fpn_offset0;
    be_reg->isp_fpn_corr0.u32 = o_isp_fpn_corr0.u32;
}
/*  description   set the value of the member isp_fpn_corr0.isp_fpn_strength0 */
/*  input         unsigned int uisp_fpn_strength0  16 bits */
static __inline td_void isp_fpn_strength0_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_strength0)
{
    u_isp_fpn_corr0 o_isp_fpn_corr0;
    o_isp_fpn_corr0.u32 = be_reg->isp_fpn_corr0.u32;
    o_isp_fpn_corr0.bits.isp_fpn_strength0 = uisp_fpn_strength0;
    be_reg->isp_fpn_corr0.u32 = o_isp_fpn_corr0.u32;
}
static __inline td_u32 isp_fpn_strengthoffset_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_fpn_corr0.u32;
}

/*  description   set the value of the member isp_fpn_shift.isp_fpn_shift */
/*  input         unsigned int uisp_fpn_shift  4 bits */
static __inline td_void isp_fpn_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_shift)
{
    u_isp_fpn_shift o_isp_fpn_shift;
    o_isp_fpn_shift.u32 = be_reg->isp_fpn_shift.u32;
    o_isp_fpn_shift.bits.isp_fpn_shift = uisp_fpn_shift;
    be_reg->isp_fpn_shift.u32 = o_isp_fpn_shift.u32;
}
/*  description   set the value of the member isp_fpn_shift.isp_fpn_in_shift */
/*  input         unsigned int uisp_fpn_in_shift  4 bits */
static __inline td_void isp_fpn_in_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_in_shift)
{
    u_isp_fpn_shift o_isp_fpn_shift;
    o_isp_fpn_shift.u32 = be_reg->isp_fpn_shift.u32;
    o_isp_fpn_shift.bits.isp_fpn_in_shift = uisp_fpn_in_shift;
    be_reg->isp_fpn_shift.u32 = o_isp_fpn_shift.u32;
}
/*  description   set the value of the member isp_fpn_shift.isp_fpn_out_shift */
/*  input         unsigned int uisp_fpn_out_shift  4 bits */
static __inline td_void isp_fpn_out_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_out_shift)
{
    u_isp_fpn_shift o_isp_fpn_shift;
    o_isp_fpn_shift.u32 = be_reg->isp_fpn_shift.u32;
    o_isp_fpn_shift.bits.isp_fpn_out_shift = uisp_fpn_out_shift;
    be_reg->isp_fpn_shift.u32 = o_isp_fpn_shift.u32;
}
/*  description   set the value of the member isp_fpn_shift.isp_fpn_frame_calib_shift */
/*  input         unsigned int uisp_fpn_frame_calib_shift  4 bits */
static __inline td_void isp_fpn_frame_calib_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_frame_calib_shift)
{
    u_isp_fpn_shift o_isp_fpn_shift;
    o_isp_fpn_shift.u32 = be_reg->isp_fpn_shift.u32;
    o_isp_fpn_shift.bits.isp_fpn_frame_calib_shift = uisp_fpn_frame_calib_shift;
    be_reg->isp_fpn_shift.u32 = o_isp_fpn_shift.u32;
}
/*  description   set the value of the member isp_fpn_max_o.isp_fpn_max_o */
/*  input         unsigned int uisp_fpn_max_o  16 bits */
static __inline td_void isp_fpn_max_o_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_max_o)
{
    u_isp_fpn_max_o o_isp_fpn_max_o;
    o_isp_fpn_max_o.u32 = be_reg->isp_fpn_max_o.u32;
    o_isp_fpn_max_o.bits.isp_fpn_max_o = uisp_fpn_max_o;
    be_reg->isp_fpn_max_o.u32 = o_isp_fpn_max_o.u32;
}
/*  description   set the value of the member isp_fpn_overflowthr.isp_fpn_overflowthr */
/*  input         unsigned int uisp_fpn_overflowthr  14 bits */
static __inline td_void isp_fpn_overflowthr_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_overflowthr)
{
    u_isp_fpn_overflowthr o_isp_fpn_overflowthr;
    o_isp_fpn_overflowthr.u32 = be_reg->isp_fpn_overflowthr.u32;
    o_isp_fpn_overflowthr.bits.isp_fpn_overflowthr = uisp_fpn_overflowthr;
    be_reg->isp_fpn_overflowthr.u32 = o_isp_fpn_overflowthr.u32;
}
/*  description   set the value of the member isp_fpn_black_frame_out_sel.isp_fpn_data0_out_sel */
/*  input         unsigned int uisp_fpn_data0_out_sel  1 bits */
static __inline td_void isp_fpn_data0_out_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_data0_out_sel)
{
    u_isp_fpn_black_frame_out_sel o_isp_fpn_black_frame_out_sel;
    o_isp_fpn_black_frame_out_sel.u32 = be_reg->isp_fpn_black_frame_out_sel.u32;
    o_isp_fpn_black_frame_out_sel.bits.isp_fpn_data0_out_sel = uisp_fpn_data0_out_sel;
    be_reg->isp_fpn_black_frame_out_sel.u32 = o_isp_fpn_black_frame_out_sel.u32;
}
/*  description   set the value of the member isp_fpn_overlap_cfg.isp_fpn_overlap_width_l */
/*  input         unsigned int uisp_fpn_overlap_width_l  16 bits */
static __inline td_void isp_fpn_overlap_width_l_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_overlap_width_l)
{
    u_isp_fpn_overlap_cfg o_isp_fpn_overlap_cfg;
    o_isp_fpn_overlap_cfg.u32 = be_reg->isp_fpn_overlap_cfg.u32;
    o_isp_fpn_overlap_cfg.bits.isp_fpn_overlap_width_l = uisp_fpn_overlap_width_l;
    be_reg->isp_fpn_overlap_cfg.u32 = o_isp_fpn_overlap_cfg.u32;
}
/*  description   set the value of the member isp_fpn_overlap_cfg.isp_fpn_overlap_width_r */
/*  input         unsigned int uisp_fpn_overlap_width_r  16 bits */
static __inline td_void isp_fpn_overlap_width_r_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_overlap_width_r)
{
    u_isp_fpn_overlap_cfg o_isp_fpn_overlap_cfg;
    o_isp_fpn_overlap_cfg.u32 = be_reg->isp_fpn_overlap_cfg.u32;
    o_isp_fpn_overlap_cfg.bits.isp_fpn_overlap_width_r = uisp_fpn_overlap_width_r;
    be_reg->isp_fpn_overlap_cfg.u32 = o_isp_fpn_overlap_cfg.u32;
}
/*  description   set the value of the member isp_fpn_calib_offline_set.isp_fpn_calib_offline_set */
/*  input         unsigned int uisp_fpn_calib_offline_set  3 bits */
static __inline td_void isp_fpn_calib_offline_set_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_calib_offline_set)
{
    u_isp_fpn_calib_offline_set o_isp_fpn_calib_offline_set;
    o_isp_fpn_calib_offline_set.u32 = be_reg->isp_fpn_calib_offline_set.u32;
    o_isp_fpn_calib_offline_set.bits.isp_fpn_calib_offline_set = uisp_fpn_calib_offline_set;
    be_reg->isp_fpn_calib_offline_set.u32 = o_isp_fpn_calib_offline_set.u32;
}
/*  description   set the value of the member isp_fpn_calib_stat_clear.isp_fpn_calib_stat_clear */
/*  input         unsigned int uisp_fpn_calib_stat_clear  1 bits */
static __inline td_void isp_fpn_calib_stat_clear_write(isp_be_reg_type *be_reg, td_u32 uisp_fpn_calib_stat_clear)
{
    u_isp_fpn_calib_stat_clear o_isp_fpn_calib_stat_clear;
    o_isp_fpn_calib_stat_clear.u32 = be_reg->isp_fpn_calib_stat_clear.u32;
    o_isp_fpn_calib_stat_clear.bits.isp_fpn_calib_stat_clear = uisp_fpn_calib_stat_clear;
    be_reg->isp_fpn_calib_stat_clear.u32 = o_isp_fpn_calib_stat_clear.u32;
}


/*  description   set the value of the member isp_dpc_alpha.isp_dpc_alpha1_rb */
/*  input         unsigned int uisp_dpc_alpha1_rb  8 bits */
#define OT_ISP_DPC_DEFAULT_ALPHA 0
static __inline td_void isp_dpc_blend_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_alpha)
{
    be_reg->isp_dpc_alpha.u32 = uisp_dpc_alpha;
}

/*****************************************************
ISP_DPC_MODE:      DPC mode
                    31:10      reserved
                        9      ir_channel
                        8      ir_position      1:gr; 0: gb
                        7      dp_hightlight_en
                        6      defect_type_sel  0:hot  1:dead
                        5      bpt_cor_en
                        4      grayscale_mode   1:grayscale     0:raw data
                        3      reserved
                        2     reserved
                        1      cor_en
                        0      det_en
*****************************************************/
#define OT_ISP_DPC_DEFAULT_MODE                       0x23
static __inline td_void isp_dpc_mode_write(isp_be_reg_type *be_reg, td_u16  uisp_dpc_mode)
{
    be_reg->isp_dpc_mode.u32 = uisp_dpc_mode & 0x4ff;
}

/*  description : set the value of the member isp_dpc_mode.isp_dpc_quadref_en */
/*  input       : unsigned int uisp_dpc_quadref_en: 1 bits */
static __inline td_void isp_dpc_quadref_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_quadref_en)
{
    u_isp_dpc_mode o_isp_dpc_mode;
    o_isp_dpc_mode.u32 = be_reg->isp_dpc_mode.u32;
    o_isp_dpc_mode.bits.isp_dpc_quadref_en = uisp_dpc_quadref_en;
    be_reg->isp_dpc_mode.u32 = o_isp_dpc_mode.u32;
}

/*****************************************************
ISP_DPC_SET_USE:
                      31:4      reserved
                        3       stage1_use_fix_set
                       2:1      reserve
                        0       stage1_use_set1
*****************************************************/
#define OT_ISP_DPC_DEFAULT_SET_USE                    0x1
static __inline td_void isp_dpc_set_use_write(isp_be_reg_type *be_reg, td_u8 uisp_dpc_set_use)
{
    be_reg->isp_dpc_set_use.u32 = uisp_dpc_set_use & 0x1f;
}

/*****************************************************
ISP_DPC_METHODS_SET_1:
                      31:5  reserved
                         4  rg_enable
                         3  mid_enable
                         2  ro_enable
                         1  lc_enable
                         0  pg_enable
*****************************************************/
#define OT_ISP_DPC_DEFAULT_METHODS_SET_1              0x17
static __inline td_void isp_dpc_methods_set_1_write(isp_be_reg_type *be_reg, td_u16 uisp_dpc_method_set_1)
{
    be_reg->isp_dpc_methods_set_1.u32 = uisp_dpc_method_set_1 & 0x1f;
}

/*****************************************************
ISP_DPC_LINE_THRESH_1:
    31:16       reserved
    15: 08      line_thr1_rb
    07: 00      line_thr1_g
*****************************************************/
#define OT_ISP_DPC_DEFAULT_LINE_THRESH_1              0x5454
static __inline td_void isp_dpc_line_thresh_1_write(isp_be_reg_type *be_reg, td_u16 uisp_dpc_line_thresh_1)
{
    be_reg->isp_dpc_line_thresh_1.u32 = uisp_dpc_line_thresh_1 & 0xffff;
}

/*****************************************************
ISP_DPC_LINE_MAD_FAC_1:
                    31:14   reserved
                    13:08   line_mad_fac_1_rb
                    07:06   reserved
                    05:00   line_mad_fac_1_g
*****************************************************/
#define OT_ISP_DPC_DEFAULT_LINE_MAD_FAC_1             0x1B1B
static __inline td_void isp_dpc_line_mad_fac_1_write(isp_be_reg_type *be_reg, td_u16 uisp_dpc_line_mad_fac_1)
{
    u_isp_dpc_line_mad_fac_1 isp_dpc_line_mad_fac_1;
    isp_dpc_line_mad_fac_1.u32 = be_reg->isp_dpc_line_mad_fac_1.u32;
    isp_dpc_line_mad_fac_1.bits.isp_dpc_line_mad_fac_1_g  = uisp_dpc_line_mad_fac_1 & 0x3f;
    isp_dpc_line_mad_fac_1.bits.isp_dpc_line_mad_fac_1_rb = (uisp_dpc_line_mad_fac_1 >> 8) & 0x3f; /* 8~13 mad_fac_1 */
    be_reg->isp_dpc_line_mad_fac_1.u32 = isp_dpc_line_mad_fac_1.u32;
}

/*****************************************************
ISP_DPC_PG_FAC_1:
                       31:16   reserved
                       15:08   pg_fac_1_rb
                       07:00   pg_fac_1_g
*****************************************************/
#define OT_ISP_DPC_DEFAULT_PG_FAC_1                   0x2020
static __inline td_void isp_dpc_pg_fac_1_write(isp_be_reg_type *be_reg, td_u16 uisp_dpc_pg_fac_1)
{
    u_isp_dpc_pg_fac_1 isp_dpc_pg_fac_1;
    isp_dpc_pg_fac_1.u32 = be_reg->isp_dpc_pg_fac_1.u32;
    isp_dpc_pg_fac_1.bits.isp_dpc_pg_fac_1_g  = uisp_dpc_pg_fac_1 & 0xff;
    isp_dpc_pg_fac_1.bits.isp_dpc_pg_fac_1_rb = (uisp_dpc_pg_fac_1 >> 8) & 0xff; /* 8~15 pg_fac_1_rb */
    be_reg->isp_dpc_pg_fac_1.u32 = isp_dpc_pg_fac_1.u32;
}

/*****************************************************
ISP_DPC_DEFAULT_RG_FAC_1:
                        31:16   reserved
                        15:08   rg_fac_1_rb
                        07:00   rg_fac_1_g
*****************************************************/
#define OT_ISP_DPC_DEFAULT_RG_FAC_1                   0x9898
static __inline td_void isp_dpc_rg_fac_1_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_rg_fac_1)
{
    u_isp_dpc_rg_fac_1 isp_dpc_rg_fac_1;
    isp_dpc_rg_fac_1.u32 = be_reg->isp_dpc_rg_fac_1.u32;
    isp_dpc_rg_fac_1.bits.isp_dpc_rg_fac_1_g  = uisp_dpc_rg_fac_1 & 0xff;
    isp_dpc_rg_fac_1.bits.isp_dpc_rg_fac_1_rb = (uisp_dpc_rg_fac_1 >> 8) & 0xff; /* 8~15 rg_fac_1_rb */
    be_reg->isp_dpc_rg_fac_1.u32 = isp_dpc_rg_fac_1.u32;
}

/*****************************************************
isp_dpc_ro_limits:
                    31:04   reserved
                    03:02   ro_rb
                    01:00   ro_g
*****************************************************/
#define OT_ISP_DPC_DEFAULT_RO_LIMITS                  0x5
static __inline td_void isp_dpc_ro_limits_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_ro_limits)
{
    be_reg->isp_dpc_ro_limits.u32 = uisp_dpc_ro_limits & 0xf;
}

#define OT_ISP_DPCC_BPDATA_DEFAULT                    16383
#define OT_ISP_DPC_DEFAULT_BPT_CTRL                   0x3FFF0330

#define OT_ISP_DPC_DEFAULT_BPT_THRESH                 0xff800080

/*  description   set the value of the member isp_dpc_soft_thr.isp_dpc_ex_soft_thr_min */
/*  input         unsigned int uisp_dpc_ex_soft_thr_min  8 bits */
#define OT_ISP_DPC_DEFAULT_SOFT_THR_MIN               (-15)

/*  description   set the value of the member isp_dpc_soft_thr.isp_dpc_ex_soft_thr_max */
/*  input         unsigned int uisp_dpc_ex_soft_thr_max  8 bits */
#define OT_ISP_DPC_DEFAULT_SOFT_THR_MAX 6

/*  description   set the value of the member isp_dpc_bhardthr_en.isp_dpc_ex_hard_thr_en */
/*  input         unsigned int uisp_dpc_ex_hard_thr_en  1 bits */
#define OT_ISP_DPC_DEFAULT_HARD_THR_ENABLE 1
static __inline td_void isp_dpc_ex_hard_thr_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_ex_hard_thr_en)
{
    u_isp_dpc_bhardthr_en o_isp_dpc_bhardthr_en;
    o_isp_dpc_bhardthr_en.u32 = be_reg->isp_dpc_bhardthr_en.u32;
    o_isp_dpc_bhardthr_en.bits.isp_dpc_ex_hard_thr_en = uisp_dpc_ex_hard_thr_en;
    be_reg->isp_dpc_bhardthr_en.u32 = o_isp_dpc_bhardthr_en.u32;
}

/*  description   set the value of the member isp_dpc_lch_thr_1.isp_dpc_line_aver_fac_1 */
/*  input         unsigned int uisp_dpc_line_aver_fac_1  8 bits */
#define OT_ISP_DPC_DEFAULT_LINE_AVER_FAC_1            80
static __inline td_void isp_dpc_line_aver_fac_1_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_line_aver_fac_1)
{
    u_isp_dpc_lch_thr_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.u32 = be_reg->isp_dpc_lch_thr_1.u32;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_aver_fac_1 = uisp_dpc_line_aver_fac_1;
    be_reg->isp_dpc_lch_thr_1.u32 = o_isp_dpc_lch_thr_1.u32;
}
/*  description   set the value of the member isp_dpc_lch_thr_1.isp_dpc_line_diff_thr_1 */
/*  input         unsigned int uisp_dpc_line_diff_thr_1  7 bits */
#define OT_ISP_DPC_DEFAULT_LINE_DIFF_THR_1            18
static __inline td_void isp_dpc_line_diff_thr_1_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_line_diff_thr_1)
{
    u_isp_dpc_lch_thr_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.u32 = be_reg->isp_dpc_lch_thr_1.u32;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_diff_thr_1 = uisp_dpc_line_diff_thr_1;
    be_reg->isp_dpc_lch_thr_1.u32 = o_isp_dpc_lch_thr_1.u32;
}
/*  description   set the value of the member isp_dpc_lch_thr_1.isp_dpc_line_std_thr_1 */
/*  input         unsigned int uisp_dpc_line_std_thr_1  12 bits */
#define OT_ISP_DPC_DEFAULT_LINE_STD_THR_1             40
static __inline td_void isp_dpc_line_std_thr_1_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_line_std_thr_1)
{
    u_isp_dpc_lch_thr_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.u32 = be_reg->isp_dpc_lch_thr_1.u32;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_std_thr_1 = uisp_dpc_line_std_thr_1;
    be_reg->isp_dpc_lch_thr_1.u32 = o_isp_dpc_lch_thr_1.u32;
}


/*  description   set the value of the member isp_dpc_rg_fac_1_mtp.isp_dpc_rg_fac_1_g_mtp */
/*  input         unsigned int uisp_dpc_rg_fac_1_g_mtp  10 bits */
static __inline td_void isp_dpc_rg_fac_1_g_mtp_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_rg_fac_1_g_mtp)
{
    u_isp_dpc_rg_fac_1_mtp o_isp_dpc_rg_fac_1_mtp;
    o_isp_dpc_rg_fac_1_mtp.u32 = be_reg->isp_dpc_rg_fac_1_mtp.u32;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_g_mtp = uisp_dpc_rg_fac_1_g_mtp;
    be_reg->isp_dpc_rg_fac_1_mtp.u32 = o_isp_dpc_rg_fac_1_mtp.u32;
}
/*  description   set the value of the member isp_dpc_rg_fac_1_mtp.isp_dpc_rg_fac_1_rb_mtp */
/*  input         unsigned int uisp_dpc_rg_fac_1_rb_mtp  10 bits */
static __inline td_void isp_dpc_rg_fac_1_rb_mtp_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_rg_fac_1_rb_mtp)
{
    u_isp_dpc_rg_fac_1_mtp o_isp_dpc_rg_fac_1_mtp;
    o_isp_dpc_rg_fac_1_mtp.u32 = be_reg->isp_dpc_rg_fac_1_mtp.u32;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_rb_mtp = uisp_dpc_rg_fac_1_rb_mtp;
    be_reg->isp_dpc_rg_fac_1_mtp.u32 = o_isp_dpc_rg_fac_1_mtp.u32;
}

static __inline  td_void isp_dpc_rg_fac_1_mtp_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_rg_fac_1_mtp)
{
    u_isp_dpc_rg_fac_1_mtp o_isp_dpc_rg_fac_1_mtp;
    o_isp_dpc_rg_fac_1_mtp.u32 = be_reg->isp_dpc_rg_fac_1_mtp.u32;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_g_mtp = uisp_dpc_rg_fac_1_mtp & 0x3ff;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_rb_mtp = (uisp_dpc_rg_fac_1_mtp >> 10) & 0x3ff; /* 10~19 */
    be_reg->isp_dpc_rg_fac_1_mtp.u32 = o_isp_dpc_rg_fac_1_mtp.u32;
}

/*  description   set the value of the member isp_dpc_amp_coef.isp_dpc_amp_coef_k */
/*  input         unsigned int uisp_dpc_amp_coef_k  8 bits */
#define OT_ISP_DPC_DEFAULT_AMP_COEF_K           0x40
static __inline td_void isp_dpc_amp_coef_k_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_amp_coef_k)
{
    u_isp_dpc_amp_coef o_isp_dpc_amp_coef;
    o_isp_dpc_amp_coef.u32 = be_reg->isp_dpc_amp_coef.u32;
    o_isp_dpc_amp_coef.bits.isp_dpc_amp_coef_k = uisp_dpc_amp_coef_k;
    be_reg->isp_dpc_amp_coef.u32 = o_isp_dpc_amp_coef.u32;
}
/*  description   set the value of the member isp_dpc_amp_coef.isp_dpc_amp_coef_min */
/*  input         unsigned int uisp_dpc_amp_coef_min  7 bits */
#define OT_ISP_DPC_DEFAULT_AMP_COEF_MIN           0x40
static __inline td_void isp_dpc_amp_coef_min_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_amp_coef_min)
{
    u_isp_dpc_amp_coef o_isp_dpc_amp_coef;
    o_isp_dpc_amp_coef.u32 = be_reg->isp_dpc_amp_coef.u32;
    o_isp_dpc_amp_coef.bits.isp_dpc_amp_coef_min = uisp_dpc_amp_coef_min;
    be_reg->isp_dpc_amp_coef.u32 = o_isp_dpc_amp_coef.u32;
}

/*  description : set the value of the member isp_dpc_soft_thr.isp_dpc_soft_thr_min */
/*  input       : unsigned int uisp_dpc_soft_thr_min: 8 bits */
static __inline td_void isp_dpc_soft_thr_min_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_soft_thr_min)
{
    u_isp_dpc_soft_thr o_isp_dpc_soft_thr;
    o_isp_dpc_soft_thr.u32 = be_reg->isp_dpc_soft_thr.u32;
    o_isp_dpc_soft_thr.bits.isp_dpc_soft_thr_min = uisp_dpc_soft_thr_min;
    be_reg->isp_dpc_soft_thr.u32 = o_isp_dpc_soft_thr.u32;
}

/*  description : set the value of the member isp_dpc_soft_thr.isp_dpc_soft_thr_max */
/*  input       : unsigned int uisp_dpc_soft_thr_max: 8 bits */
static __inline td_void isp_dpc_soft_thr_max_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_soft_thr_max)
{
    u_isp_dpc_soft_thr o_isp_dpc_soft_thr;
    o_isp_dpc_soft_thr.u32 = be_reg->isp_dpc_soft_thr.u32;
    o_isp_dpc_soft_thr.bits.isp_dpc_soft_thr_max = uisp_dpc_soft_thr_max;
    be_reg->isp_dpc_soft_thr.u32 = o_isp_dpc_soft_thr.u32;
}
/*  description   set the value of the member isp_dpc_rakeratio.isp_dpc_ex_rake_ratio */
/*  input         unsigned int uisp_dpc_ex_rake_ratio  16 bits */
#define OT_ISP_DPC_DEFAULT_SOFT_RAKE_RATIO 0x600
static __inline td_void isp_dpc_ex_rake_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_ex_rake_ratio)
{
    u_isp_dpc_rakeratio o_isp_dpc_rakeratio;
    o_isp_dpc_rakeratio.u32 = be_reg->isp_dpc_rakeratio.u32;
    o_isp_dpc_rakeratio.bits.isp_dpc_ex_rake_ratio = uisp_dpc_ex_rake_ratio;
    be_reg->isp_dpc_rakeratio.u32 = o_isp_dpc_rakeratio.u32;
}

/*  description : set the value of the member isp_dpc_dark_enhance.isp_dpc_dark_enhance */
/*  input       : unsigned int uisp_dpc_dark_enhance: 7 bits */
static __inline td_void isp_dpc_dark_enhance_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_dark_enhance)
{
    u_isp_dpc_dark_enhance o_isp_dpc_dark_enhance;
    o_isp_dpc_dark_enhance.u32 = be_reg->isp_dpc_dark_enhance.u32;
    o_isp_dpc_dark_enhance.bits.isp_dpc_dark_enhance = uisp_dpc_dark_enhance;
    be_reg->isp_dpc_dark_enhance.u32 = o_isp_dpc_dark_enhance.u32;
}
/*  description   set the value of the member isp_dpc_line_kerdiff_fac.isp_dpc_line_kerdiff_fac */
/*  input         unsigned int uisp_dpc_line_kerdiff_fac  8 bits */
#define OT_ISP_DPC_DEFAULT_LINE_KERDIFF_FAC           64
static __inline td_void isp_dpc_line_kerdiff_fac_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_line_kerdiff_fac)
{
    u_isp_dpc_line_kerdiff_fac o_isp_dpc_line_kerdiff_fac;
    o_isp_dpc_line_kerdiff_fac.u32 = be_reg->isp_dpc_line_kerdiff_fac.u32;
    o_isp_dpc_line_kerdiff_fac.bits.isp_dpc_line_kerdiff_fac = uisp_dpc_line_kerdiff_fac;
    be_reg->isp_dpc_line_kerdiff_fac.u32 = o_isp_dpc_line_kerdiff_fac.u32;
}

static __inline td_void isp_demosaic_inner_bypass_en_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_inner_bypass_en)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_inner_bypass_en = uisp_demosaic_inner_bypass_en;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}

#define OT_ISP_DEMOSAIC_AHD_ENABLE_DEFAULT 1
static __inline td_void isp_demosaic_ahd_en_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_ahd_en)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_ahd_en = uisp_demosaic_ahd_en;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}

/*  description : set the value of the member isp_demosaic_cfg1.isp_demosaic_nddm_mode */
/*  input       : unsigned int uisp_demosaic_nddm_mode: 1 bits */
#define OT_ISP_DEMOSAIC_NDDM_MODE_DEFAULT 0
static __inline td_void isp_demosaic_nddm_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_nddm_mode)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_nddm_mode = uisp_demosaic_nddm_mode;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}

/*  description   set the value of the member isp_demosaic_cfg1.isp_demosaic_gcac_en */
/*  input         unsigned int uisp_demosaic_gcac_en  1 bits */
static __inline td_void isp_demosaic_gcac_en_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_en)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_gcac_en = uisp_demosaic_gcac_en;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}
/*  description   set the value of the member isp_demosaic_cfg1.isp_demosaic_local_cac_en */
/*  input         unsigned int uisp_demosaic_local_cac_en  1 bits */
static __inline td_void isp_demosaic_local_cac_en_write(isp_be_reg_type *be_reg, td_u32 demosaic_local_cac_en)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_lcac_en = demosaic_local_cac_en;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}
/*  description   set the value of the member isp_demosaic_cfg1.isp_demosaic_fcr_en */
/*  input         unsigned int uisp_demosaic_fcr_en  1 bits */
static __inline td_void isp_demosaic_fcr_en_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_en)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_fcr_en = uisp_demosaic_fcr_en;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}


/*  description   set the value of the member isp_demosaic_cfg1.isp_demosaic_desat_enable */
/*  input         unsigned int uisp_demosaic_desat_enable  1 bits */
#define OT_ISP_DEMOSAIC_DESAT_ENABLE_DEFAULT          1
static __inline td_void isp_demosaic_desat_enable_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_desat_enable)
{
    u_isp_demosaic_cfg1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.u32 = be_reg->isp_demosaic_cfg1.u32;
    o_isp_demosaic_cfg1.bits.isp_demosaic_desat_en = uisp_demosaic_desat_enable;
    be_reg->isp_demosaic_cfg1.u32 = o_isp_demosaic_cfg1.u32;
}


/*  description   set the value of the member isp_demosaic_coef0.isp_demosaic_bld_limit2 */
/*  input         unsigned int uisp_demosaic_bld_limit2  8 bits */
#define OT_ISP_DEMOSAIC_BLENDLIMIT2_DEFAULT           40
static __inline td_void isp_demosaic_bld_limit2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_bld_limit2)
{
    u_isp_demosaic_coef0 o_isp_demosaic_coef0;
    o_isp_demosaic_coef0.u32 = be_reg->isp_demosaic_coef0.u32;
    o_isp_demosaic_coef0.bits.isp_demosaic_bld_limit2 = uisp_demosaic_bld_limit2;
    be_reg->isp_demosaic_coef0.u32 = o_isp_demosaic_coef0.u32;
}
/*  description   set the value of the member isp_demosaic_coef0.isp_demosaic_bld_limit1 */
/*  input         unsigned int uisp_demosaic_bld_limit1  8 bits */
#define OT_ISP_DEMOSAIC_BLENDLIMIT1_DEFAULT           24
static __inline td_void isp_demosaic_bld_limit1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_bld_limit1)
{
    u_isp_demosaic_coef0 o_isp_demosaic_coef0;
    o_isp_demosaic_coef0.u32 = be_reg->isp_demosaic_coef0.u32;
    o_isp_demosaic_coef0.bits.isp_demosaic_bld_limit1 = uisp_demosaic_bld_limit1;
    be_reg->isp_demosaic_coef0.u32 = o_isp_demosaic_coef0.u32;
}

/*  description : set the value of the member isp_demosaic_coef0.isp_demosaic_grad_limit2 */
/*  input       : unsigned int uisp_demosaic_grad_limit2: 8 bits */
#define OT_ISP_DEMOSAIC_GRADLIMIT2_DEFAULT           80
static __inline td_void isp_demosaic_grad_limit2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_grad_limit2)
{
    u_isp_demosaic_coef0 o_isp_demosaic_coef0;
    o_isp_demosaic_coef0.u32 = be_reg->isp_demosaic_coef0.u32;
    o_isp_demosaic_coef0.bits.isp_demosaic_grad_limit2 = uisp_demosaic_grad_limit2;
    be_reg->isp_demosaic_coef0.u32 = o_isp_demosaic_coef0.u32;
}

/*  description : set the value of the member isp_demosaic_coef0.isp_demosaic_grad_limit1 */
/*  input       : unsigned int uisp_demosaic_grad_limit1: 8 bits */
#define OT_ISP_DEMOSAIC_GRADLIMIT1_DEFAULT           16
static __inline td_void isp_demosaic_grad_limit1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_grad_limit1)
{
    u_isp_demosaic_coef0 o_isp_demosaic_coef0;
    o_isp_demosaic_coef0.u32 = be_reg->isp_demosaic_coef0.u32;
    o_isp_demosaic_coef0.bits.isp_demosaic_grad_limit1 = uisp_demosaic_grad_limit1;
    be_reg->isp_demosaic_coef0.u32 = o_isp_demosaic_coef0.u32;
}

/*  description   set the value of the member isp_demosaic_coef2.isp_demosaic_fcr_limit2 */
/*  input         unsigned int uisp_demosaic_fcr_limit2  12 bits */
#define OT_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT            0xA0
static __inline td_void isp_demosaic_fcr_limit2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_limit2)
{
    u_isp_demosaic_coef2 o_isp_demosaic_coef2;
    o_isp_demosaic_coef2.u32 = be_reg->isp_demosaic_coef2.u32;
    o_isp_demosaic_coef2.bits.isp_demosaic_fcr_limit2 = uisp_demosaic_fcr_limit2;
    be_reg->isp_demosaic_coef2.u32 = o_isp_demosaic_coef2.u32;
}
/*  description   set the value of the member isp_demosaic_coef2.isp_demosaic_fcr_limit1 */
/*  input         unsigned int uisp_demosaic_fcr_limit1  12 bits */
#define OT_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT            0x10
static __inline td_void isp_demosaic_fcr_limit1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_limit1)
{
    u_isp_demosaic_coef2 o_isp_demosaic_coef2;
    o_isp_demosaic_coef2.u32 = be_reg->isp_demosaic_coef2.u32;
    o_isp_demosaic_coef2.bits.isp_demosaic_fcr_limit1 = uisp_demosaic_fcr_limit1;
    be_reg->isp_demosaic_coef2.u32 = o_isp_demosaic_coef2.u32;
}
/*  description   set the value of the member isp_demosaic_coef3.isp_demosaic_ahd_par2 */
/*  input         unsigned int uisp_demosaic_ahd_par2  8 bits */
#define OT_ISP_DEMOSAIC_AHDPART2_DEFAULT              8
static __inline td_void isp_demosaic_ahd_par2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_ahd_par2)
{
    u_isp_demosaic_coef3 o_isp_demosaic_coef3;
    o_isp_demosaic_coef3.u32 = be_reg->isp_demosaic_coef3.u32;
    o_isp_demosaic_coef3.bits.isp_demosaic_ahd_par2 = uisp_demosaic_ahd_par2;
    be_reg->isp_demosaic_coef3.u32 = o_isp_demosaic_coef3.u32;
}
/*  description   set the value of the member isp_demosaic_coef3.isp_demosaic_ahd_par1 */
/*  input         unsigned int uisp_demosaic_ahd_par1  10 bits */
#define OT_ISP_DEMOSAIC_AHDPART1_DEFAULT              400
static __inline td_void isp_demosaic_ahd_par1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_ahd_par1)
{
    u_isp_demosaic_coef3 o_isp_demosaic_coef3;
    o_isp_demosaic_coef3.u32 = be_reg->isp_demosaic_coef3.u32;
    o_isp_demosaic_coef3.bits.isp_demosaic_ahd_par1 = uisp_demosaic_ahd_par1;
    be_reg->isp_demosaic_coef3.u32 = o_isp_demosaic_coef3.u32;
}
/*  description   set the value of the member isp_demosaic_coef4.isp_demosaic_fcr_gain */
/*  input         unsigned int uisp_demosaic_fcr_gain  5 bits */
#define OT_ISP_DEMOSAIC_FCR_GAIN_DEFAULT              8
static __inline td_void isp_demosaic_fcr_gain_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_gain)
{
    u_isp_demosaic_coef4 o_isp_demosaic_coef4;
    o_isp_demosaic_coef4.u32 = be_reg->isp_demosaic_coef4.u32;
    o_isp_demosaic_coef4.bits.isp_demosaic_fcr_gain = uisp_demosaic_fcr_gain;
    be_reg->isp_demosaic_coef4.u32 = o_isp_demosaic_coef4.u32;
}

/*  description : set the value of the member isp_demosaic_coef4.isp_demosaic_var_thr_for_cac */
/*  input       : unsigned int uisp_demosaic_var_thr_for_cac: 10 bits */
#define OT_ISP_DEMOSAIC_VAR_THR_FOR_CAC_DEFAULT 6
static __inline td_void isp_demosaic_var_thr_for_cac_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_var_thr_for_cac)
{
    u_isp_demosaic_coef4 o_isp_demosaic_coef4;
    o_isp_demosaic_coef4.u32 = be_reg->isp_demosaic_coef4.u32;
    o_isp_demosaic_coef4.bits.isp_demosaic_var_thr_for_cac = uisp_demosaic_var_thr_for_cac;
    be_reg->isp_demosaic_coef4.u32 = o_isp_demosaic_coef4.u32;
}

/*  description : set the value of the member isp_demosaic_coef4.isp_demosaic_var_thr_for_ahd */
/*  input       : unsigned int uisp_demosaic_var_thr_for_ahd: 10 bits */
#define OT_ISP_DEMOSAIC_VAR_THR_FOR_AHD_DEFAULT 4
static __inline td_void isp_demosaic_var_thr_for_ahd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_var_thr_for_ahd)
{
    u_isp_demosaic_coef4 o_isp_demosaic_coef4;
    o_isp_demosaic_coef4.u32 = be_reg->isp_demosaic_coef4.u32;
    o_isp_demosaic_coef4.bits.isp_demosaic_var_thr_for_ahd = uisp_demosaic_var_thr_for_ahd;
    be_reg->isp_demosaic_coef4.u32 = o_isp_demosaic_coef4.u32;
}

/*  description   set the value of the member isp_demosaic_coef5.isp_demosaic_hv_ratio */
/*  input         unsigned int uisp_demosaic_hv_ratio  8 bits */
#define OT_ISP_DEMOSAIC_HV_DETG_RATIO_DEFAULT          13
static __inline td_void isp_demosaic_hv_detg_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_hv_detg_ratio)
{
    u_isp_demosaic_coef5 o_isp_demosaic_coef5;
    o_isp_demosaic_coef5.u32 = be_reg->isp_demosaic_coef5.u32;
    o_isp_demosaic_coef5.bits.isp_demosaic_hv_detg_ratio = uisp_demosaic_hv_detg_ratio;
    be_reg->isp_demosaic_coef5.u32 = o_isp_demosaic_coef5.u32;
}
/*  description : set the value of the member isp_demosaic_coef5.isp_demosaic_hv_color_ratio */
/*  input       : unsigned int uisp_demosaic_hv_color_ratio: 8 bits */
#define OT_ISP_DEMOSAIC_HV_COLOR_RATIO_DEFAULT         64
static __inline td_void isp_demosaic_hv_color_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_hv_color_ratio)
{
    u_isp_demosaic_coef5 o_isp_demosaic_coef5;
    o_isp_demosaic_coef5.u32 = be_reg->isp_demosaic_coef5.u32;
    o_isp_demosaic_coef5.bits.isp_demosaic_hv_color_ratio = uisp_demosaic_hv_color_ratio;
    be_reg->isp_demosaic_coef5.u32 = o_isp_demosaic_coef5.u32;
}

/*  description : set the value of the member isp_demosaic_coef5.isp_demosaic_wgd_limit2 */
/*  input       : unsigned int uisp_demosaic_wgd_limit2: 8 bits */
#define OT_ISP_DEMOSAIC_WGDLIMIT2_DEFAULT               100
static __inline td_void isp_demosaic_wgd_limit2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_wgd_limit2)
{
    u_isp_demosaic_coef5 o_isp_demosaic_coef5;
    o_isp_demosaic_coef5.u32 = be_reg->isp_demosaic_coef5.u32;
    o_isp_demosaic_coef5.bits.isp_demosaic_wgd_limit2 = uisp_demosaic_wgd_limit2;
    be_reg->isp_demosaic_coef5.u32 = o_isp_demosaic_coef5.u32;
}

/*  description : set the value of the member isp_demosaic_coef5.isp_demosaic_wgd_limit1 */
/*  input       : unsigned int uisp_demosaic_wgd_limit1: 8 bits */
#define OT_ISP_DEMOSAIC_WGDLIMIT1_DEFAULT               30
static __inline td_void isp_demosaic_wgd_limit1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_wgd_limit1)
{
    u_isp_demosaic_coef5 o_isp_demosaic_coef5;
    o_isp_demosaic_coef5.u32 = be_reg->isp_demosaic_coef5.u32;
    o_isp_demosaic_coef5.bits.isp_demosaic_wgd_limit1 = uisp_demosaic_wgd_limit1;
    be_reg->isp_demosaic_coef5.u32 = o_isp_demosaic_coef5.u32;
}

/*  description   set the value of the member isp_demosaic_coef6.isp_demosaic_fcr_ratio */
/*  input         unsigned int uisp_demosaic_fcr_ratio  6 bits */
#define OT_ISP_DEMOSAIC_FCR_RATIO_DEFAULT             8
static __inline td_void isp_demosaic_fcr_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_ratio)
{
    u_isp_demosaic_coef6 o_isp_demosaic_coef6;
    o_isp_demosaic_coef6.u32 = be_reg->isp_demosaic_coef6.u32;
    o_isp_demosaic_coef6.bits.isp_demosaic_fcr_ratio = uisp_demosaic_fcr_ratio;
    be_reg->isp_demosaic_coef6.u32 = o_isp_demosaic_coef6.u32;
}

/*  description : set the value of the member isp_demosaic_coef7.isp_demosaic_fcr_remove */
/*  input       : unsigned int uisp_demosaic_fcr_remove: 12 bits */
#define OT_ISP_DEMOSAIC_FCR_REMOVE_DEFAULT            1
static __inline td_void isp_demosaic_fcr_remove_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_remove)
{
    u_isp_demosaic_coef7 o_isp_demosaic_coef7;
    o_isp_demosaic_coef7.u32 = be_reg->isp_demosaic_coef7.u32;
    o_isp_demosaic_coef7.bits.isp_demosaic_fcr_remove = uisp_demosaic_fcr_remove;
    be_reg->isp_demosaic_coef7.u32 = o_isp_demosaic_coef7.u32;
}
/*  description   set the value of the member isp_demosaic_lcac_cnt_thr.isp_demosaic_r_counter_thr */
/*  input         unsigned int uisp_demosaic_r_counter_thr  5 bits */
#define OT_ISP_LCAC_COUNT_THR_R_DEFAULT               1
static __inline td_void isp_demosaic_r_counter_thr_write(isp_be_reg_type *be_reg, td_u32 demosaic_r_counter_thr)
{
    u_isp_demosaic_lcac_cnt_thr o_isp_demosaic_lcac_cnt_thr;
    o_isp_demosaic_lcac_cnt_thr.u32 = be_reg->isp_demosaic_lcac_cnt_thr.u32;
    o_isp_demosaic_lcac_cnt_thr.bits.isp_demosaic_r_counter_thr = demosaic_r_counter_thr;
    be_reg->isp_demosaic_lcac_cnt_thr.u32 = o_isp_demosaic_lcac_cnt_thr.u32;
}
/*  description   set the value of the member isp_demosaic_lcac_cnt_thr.isp_demosaic_b_counter_thr */
/*  input         unsigned int uisp_demosaic_b_counter_thr  5 bits */
#define OT_ISP_LCAC_COUNT_THR_B_DEFAULT               1
static __inline td_void isp_demosaic_b_counter_thr_write(isp_be_reg_type *be_reg, td_u32 demosaic_b_counter_thr)
{
    u_isp_demosaic_lcac_cnt_thr o_isp_demosaic_lcac_cnt_thr;
    o_isp_demosaic_lcac_cnt_thr.u32 = be_reg->isp_demosaic_lcac_cnt_thr.u32;
    o_isp_demosaic_lcac_cnt_thr.bits.isp_demosaic_b_counter_thr = demosaic_b_counter_thr;
    be_reg->isp_demosaic_lcac_cnt_thr.u32 = o_isp_demosaic_lcac_cnt_thr.u32;
}
/*  description   set the value of the member isp_demosaic_lcac_cnt_thr.isp_demosaic_g_counter_thr */
/*  input         unsigned int uisp_demosaic_g_counter_thr  6 bits */
#define OT_ISP_LCAC_COUNT_THR_G_DEFAULT               1
static __inline td_void isp_demosaic_g_counter_thr_write(isp_be_reg_type *be_reg, td_u32 demosaic_g_counter_thr)
{
    u_isp_demosaic_lcac_cnt_thr o_isp_demosaic_lcac_cnt_thr;
    o_isp_demosaic_lcac_cnt_thr.u32 = be_reg->isp_demosaic_lcac_cnt_thr.u32;
    o_isp_demosaic_lcac_cnt_thr.bits.isp_demosaic_g_counter_thr = demosaic_g_counter_thr;
    be_reg->isp_demosaic_lcac_cnt_thr.u32 = o_isp_demosaic_lcac_cnt_thr.u32;
}
/*  description   set the value of the member isp_demosaic_lcac_luma_rb_thr.isp_demosaic_r_luma_thr */
/*  input         unsigned int uisp_demosaic_r_luma_thr  12 bits */
static __inline td_void isp_demosaic_r_luma_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_r_luma_thr)
{
    u_isp_demosaic_lcac_luma_rb_thr o_isp_demosaic_lcac_luma_rb_thr;
    o_isp_demosaic_lcac_luma_rb_thr.u32 = be_reg->isp_demosaic_lcac_luma_rb_thr.u32;
    o_isp_demosaic_lcac_luma_rb_thr.bits.isp_demosaic_r_luma_thr = uisp_demosaic_r_luma_thr;
    be_reg->isp_demosaic_lcac_luma_rb_thr.u32 = o_isp_demosaic_lcac_luma_rb_thr.u32;
}
/*  description   set the value of the member isp_demosaic_lcac_luma_rb_thr.isp_demosaic_b_luma_thr */
/*  input         unsigned int uisp_demosaic_b_luma_thr  12 bits */
static __inline td_void isp_demosaic_b_luma_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_b_luma_thr)
{
    u_isp_demosaic_lcac_luma_rb_thr o_isp_demosaic_lcac_luma_rb_thr;
    o_isp_demosaic_lcac_luma_rb_thr.u32 = be_reg->isp_demosaic_lcac_luma_rb_thr.u32;
    o_isp_demosaic_lcac_luma_rb_thr.bits.isp_demosaic_b_luma_thr = uisp_demosaic_b_luma_thr;
    be_reg->isp_demosaic_lcac_luma_rb_thr.u32 = o_isp_demosaic_lcac_luma_rb_thr.u32;
}
/*  description   set the value of the member isp_demosaic_lcac_luma_g_thr.isp_demosaic_g_luma_thr */
/*  input         unsigned int uisp_demosaic_g_luma_thr  12 bits */
static __inline td_void isp_demosaic_g_luma_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_g_luma_thr)
{
    u_isp_demosaic_lcac_luma_g_thr o_isp_demosaic_lcac_luma_g_thr;
    o_isp_demosaic_lcac_luma_g_thr.u32 = be_reg->isp_demosaic_lcac_luma_g_thr.u32;
    o_isp_demosaic_lcac_luma_g_thr.bits.isp_demosaic_g_luma_thr = uisp_demosaic_g_luma_thr;
    be_reg->isp_demosaic_lcac_luma_g_thr.u32 = o_isp_demosaic_lcac_luma_g_thr.u32;
}

/*  description : set the value of the member isp_demosaic_lcac_ratio_thd.isp_demosaic_lcac_ratio_thd */
/*  input       : unsigned int uisp_demosaic_lcac_ratio_thd: 10 bits */
static __inline td_void isp_demosaic_lcac_ratio_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_lcac_ratio_thd)
{
    u_isp_demosaic_lcac_ratio_thd o_isp_demosaic_lcac_ratio_thd;
    o_isp_demosaic_lcac_ratio_thd.u32 = be_reg->isp_demosaic_lcac_ratio_thd.u32;
    o_isp_demosaic_lcac_ratio_thd.bits.isp_demosaic_lcac_ratio_thd = uisp_demosaic_lcac_ratio_thd;
    be_reg->isp_demosaic_lcac_ratio_thd.u32 = o_isp_demosaic_lcac_ratio_thd.u32;
}

/*  description : set the value of the member isp_demosaic_lcac_ratio_sft.isp_demosaic_lcac_ratio_sft */
/*  input       : unsigned int uisp_demosaic_lcac_ratio_sft: 3 bits */
static __inline td_void isp_demosaic_lcac_ratio_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_lcac_ratio_sft)
{
    u_isp_demosaic_lcac_ratio_sft o_isp_demosaic_lcac_ratio_sft;
    o_isp_demosaic_lcac_ratio_sft.u32 = be_reg->isp_demosaic_lcac_ratio_sft.u32;
    o_isp_demosaic_lcac_ratio_sft.bits.isp_demosaic_lcac_ratio_sft = uisp_demosaic_lcac_ratio_sft;
    be_reg->isp_demosaic_lcac_ratio_sft.u32 = o_isp_demosaic_lcac_ratio_sft.u32;
}

/*  description : set the value of the member isp_demosaic_lcac_g_avg_thd.isp_demosaic_lcac_g_avg_thd */
/*  input       : unsigned int uisp_demosaic_lcac_g_avg_thd: 10 bits */
static __inline td_void isp_demosaic_lcac_g_avg_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_lcac_g_avg_thd)
{
    u_isp_demosaic_lcac_g_avg_thd o_isp_demosaic_lcac_g_avg_thd;
    o_isp_demosaic_lcac_g_avg_thd.u32 = be_reg->isp_demosaic_lcac_g_avg_thd.u32;
    o_isp_demosaic_lcac_g_avg_thd.bits.isp_demosaic_lcac_g_avg_thd = uisp_demosaic_lcac_g_avg_thd;
    be_reg->isp_demosaic_lcac_g_avg_thd.u32 = o_isp_demosaic_lcac_g_avg_thd.u32;
}

/*  description : set the value of the member isp_demosaic_lcac_g_avg_sft.isp_demosaic_lcac_g_avg_sft */
/*  input       : unsigned int uisp_demosaic_lcac_g_avg_sft: 3 bits */
static __inline td_void isp_demosaic_lcac_g_avg_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_lcac_g_avg_sft)
{
    u_isp_demosaic_lcac_g_avg_sft o_isp_demosaic_lcac_g_avg_sft;
    o_isp_demosaic_lcac_g_avg_sft.u32 = be_reg->isp_demosaic_lcac_g_avg_sft.u32;
    o_isp_demosaic_lcac_g_avg_sft.bits.isp_demosaic_lcac_g_avg_sft = uisp_demosaic_lcac_g_avg_sft;
    be_reg->isp_demosaic_lcac_g_avg_sft.u32 = o_isp_demosaic_lcac_g_avg_sft.u32;
}

/*  description : set the value of the member isp_demosaic_plat_thd.isp_demosaic_plat_thd */
/*  input       : unsigned int uisp_demosaic_plat_thd: 10 bits */
static __inline td_void isp_demosaic_plat_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_plat_thd)
{
    u_isp_demosaic_plat_thd o_isp_demosaic_plat_thd;
    o_isp_demosaic_plat_thd.u32 = be_reg->isp_demosaic_plat_thd.u32;
    o_isp_demosaic_plat_thd.bits.isp_demosaic_plat_thd = uisp_demosaic_plat_thd;
    be_reg->isp_demosaic_plat_thd.u32 = o_isp_demosaic_plat_thd.u32;
}

/*  description : set the value of the member isp_demosaic_over_exp_thd.isp_demosaic_over_exp_thd */
/*  input       : unsigned int uisp_demosaic_over_exp_thd: 6 bits */
static __inline td_void isp_demosaic_over_exp_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_over_exp_thd)
{
    u_isp_demosaic_over_exp_thd o_isp_demosaic_over_exp_thd;
    o_isp_demosaic_over_exp_thd.u32 = be_reg->isp_demosaic_over_exp_thd.u32;
    o_isp_demosaic_over_exp_thd.bits.isp_demosaic_over_exp_thd = uisp_demosaic_over_exp_thd;
    be_reg->isp_demosaic_over_exp_thd.u32 = o_isp_demosaic_over_exp_thd.u32;
}

/*  description : set the value of the member isp_demosaic_b_diff_sft.isp_demosaic_b_diff_sft */
/*  input       : unsigned int uisp_demosaic_b_diff_sft: 3 bits */
static __inline td_void isp_demosaic_b_diff_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_b_diff_sft)
{
    u_isp_demosaic_b_diff_sft o_isp_demosaic_b_diff_sft;
    o_isp_demosaic_b_diff_sft.u32 = be_reg->isp_demosaic_b_diff_sft.u32;
    o_isp_demosaic_b_diff_sft.bits.isp_demosaic_b_diff_sft = uisp_demosaic_b_diff_sft;
    be_reg->isp_demosaic_b_diff_sft.u32 = o_isp_demosaic_b_diff_sft.u32;
}

/*  description : set the value of the member isp_demosaic_mix_ratio.isp_demosaic_mix_ratio */
/*  input       : unsigned int uisp_demosaic_mix_ratio: 4 bits */
static __inline td_void isp_demosaic_mix_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_mix_ratio)
{
    u_isp_demosaic_mix_ratio o_isp_demosaic_mix_ratio;
    o_isp_demosaic_mix_ratio.u32 = be_reg->isp_demosaic_mix_ratio.u32;
    o_isp_demosaic_mix_ratio.bits.isp_demosaic_mix_ratio = uisp_demosaic_mix_ratio;
    be_reg->isp_demosaic_mix_ratio.u32 = o_isp_demosaic_mix_ratio.u32;
}
/*  description   set the value of the member isp_demosaic_depurplectr.isp_demosaic_depurplectrcb */
/*  input         unsigned int uisp_demosaic_depurplectrcb  4 bits */
static __inline td_void isp_demosaic_depurplectrcb_write(isp_be_reg_type *be_reg, td_u32 demosaic_depurplectrcb)
{
    u_isp_demosaic_depurplectr o_isp_demosaic_depurplectr;
    o_isp_demosaic_depurplectr.u32 = be_reg->isp_demosaic_depurplectr.u32;
    o_isp_demosaic_depurplectr.bits.isp_demosaic_depurplectrcb = demosaic_depurplectrcb;
    be_reg->isp_demosaic_depurplectr.u32 = o_isp_demosaic_depurplectr.u32;
}
/*  description   set the value of the member isp_demosaic_depurplectr.isp_demosaic_depurplectrcr */
/*  input         unsigned int uisp_demosaic_depurplectrcr  4 bits */
static __inline td_void isp_demosaic_depurplectrcr_write(isp_be_reg_type *be_reg, td_u32 demosaic_depurplectrcr)
{
    u_isp_demosaic_depurplectr o_isp_demosaic_depurplectr;
    o_isp_demosaic_depurplectr.u32 = be_reg->isp_demosaic_depurplectr.u32;
    o_isp_demosaic_depurplectr.bits.isp_demosaic_depurplectrcr = demosaic_depurplectrcr;
    be_reg->isp_demosaic_depurplectr.u32 = o_isp_demosaic_depurplectr.u32;
}

/*  description : set the value of the member isp_demosaic_g_intp_ctrl.isp_demosaic_g_clip_sft_bit */
/*  input       : unsigned int uisp_demosaic_g_clip_sft_bit: 4 bits */
#define OT_ISP_DEMOSAIC_G_CLIP_SFT_BIT_DEFAULT       0x1
static __inline td_void isp_demosaic_g_clip_sft_bit_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_g_clip_sft_bit)
{
    u_isp_demosaic_g_intp_ctrl o_isp_demosaic_g_intp_ctrl;
    o_isp_demosaic_g_intp_ctrl.u32 = be_reg->isp_demosaic_g_intp_ctrl.u32;
    o_isp_demosaic_g_intp_ctrl.bits.isp_demosaic_g_clip_sft_bit = uisp_demosaic_g_clip_sft_bit;
    be_reg->isp_demosaic_g_intp_ctrl.u32 = o_isp_demosaic_g_intp_ctrl.u32;
}

/*  description   set the value of the member isp_demosaic_cc_hf_ratio.isp_demosaic_cc_hf_min_ratio */
/*  input         unsigned int uisp_demosaic_cc_hf_min_ratio  5 bits */
#define OT_ISP_DEMOSAIC_CC_HF_MIN_RATIO_DEFAULT       0x0
static __inline td_void isp_demosaic_cc_hf_min_ratio_write(isp_be_reg_type *be_reg,
    td_u32 uisp_demosaic_cc_hf_min_ratio)
{
    u_isp_demosaic_cc_hf_ratio o_isp_demosaic_cc_hf_ratio;
    o_isp_demosaic_cc_hf_ratio.u32 = be_reg->isp_demosaic_cc_hf_ratio.u32;
    o_isp_demosaic_cc_hf_ratio.bits.isp_demosaic_cc_hf_min_ratio = uisp_demosaic_cc_hf_min_ratio;
    be_reg->isp_demosaic_cc_hf_ratio.u32 = o_isp_demosaic_cc_hf_ratio.u32;
}
/*  description   set the value of the member isp_demosaic_cc_hf_ratio.isp_demosaic_cc_hf_max_ratio */
/*  input         unsigned int uisp_demosaic_cc_hf_max_ratio  5 bits */
#define OT_ISP_DEMOSAIC_CC_HF_MAX_RATIO_DEFAULT       0x8
static __inline td_void isp_demosaic_cc_hf_max_ratio_write(isp_be_reg_type *be_reg,
    td_u32 uisp_demosaic_cc_hf_max_ratio)
{
    u_isp_demosaic_cc_hf_ratio o_isp_demosaic_cc_hf_ratio;
    o_isp_demosaic_cc_hf_ratio.u32 = be_reg->isp_demosaic_cc_hf_ratio.u32;
    o_isp_demosaic_cc_hf_ratio.bits.isp_demosaic_cc_hf_max_ratio = uisp_demosaic_cc_hf_max_ratio;
    be_reg->isp_demosaic_cc_hf_ratio.u32 = o_isp_demosaic_cc_hf_ratio.u32;
}

/*  description : set the value of the member isp_demosaic_satu_thr.isp_demosaic_satu_thr */
/*  input       : unsigned int uisp_demosaic_satu_thr: 12 bits */
static __inline td_void isp_demosaic_satu_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_satu_thr)
{
    u_isp_demosaic_satu_thr o_isp_demosaic_satu_thr;
    o_isp_demosaic_satu_thr.u32 = be_reg->isp_demosaic_satu_thr.u32;
    o_isp_demosaic_satu_thr.bits.isp_demosaic_satu_thr = uisp_demosaic_satu_thr;
    be_reg->isp_demosaic_satu_thr.u32 = o_isp_demosaic_satu_thr.u32;
}

/*  description : set the value of the member isp_demosaic_satu_thr.isp_demosaic_g_var_thr_for_purple */
/*  input       : unsigned int uisp_demosaic_g_var_thr_for_purple: 12 bits */
static __inline td_void isp_demosaic_g_var_thr_for_purple_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_g_var_thr_for_purple)
{
    u_isp_demosaic_satu_thr o_isp_demosaic_satu_thr;
    o_isp_demosaic_satu_thr.u32 = be_reg->isp_demosaic_satu_thr.u32;
    o_isp_demosaic_satu_thr.bits.isp_demosaic_g_var_thr_for_purple = uisp_demosaic_g_var_thr_for_purple;
    be_reg->isp_demosaic_satu_thr.u32 = o_isp_demosaic_satu_thr.u32;
}

/*  description   set the value of the member isp_demosaic_cbcr_ratio_limit.isp_demosaic_cbcr_ratio_low_limit */
/*  input         unsigned int uisp_demosaic_cbcr_ratio_low_limit  12 bits */
static __inline td_void isp_demosaic_cbcr_ratio_low_limit_write(isp_be_reg_type *be_reg,
    td_u32 uisp_demosaic_cbcr_ratio_low_limit)
{
    u_isp_demosaic_cbcr_ratio_limit o_isp_demosaic_cbcr_ratio_limit;
    o_isp_demosaic_cbcr_ratio_limit.u32 = be_reg->isp_demosaic_cbcr_ratio_limit.u32;
    o_isp_demosaic_cbcr_ratio_limit.bits.isp_demosaic_cbcr_ratio_low_limit = uisp_demosaic_cbcr_ratio_low_limit;
    be_reg->isp_demosaic_cbcr_ratio_limit.u32 = o_isp_demosaic_cbcr_ratio_limit.u32;
}
/*  description   set the value of the member isp_demosaic_cbcr_ratio_limit.isp_demosaic_cbcr_ratio_high_limit */
/*  input         unsigned int uisp_demosaic_cbcr_ratio_high_limit  12 bits */
static __inline td_void isp_demosaic_cbcr_ratio_high_limit_write(isp_be_reg_type *be_reg,
    td_u32 uisp_demosaic_cbcr_ratio_high_limit)
{
    u_isp_demosaic_cbcr_ratio_limit o_isp_demosaic_cbcr_ratio_limit;
    o_isp_demosaic_cbcr_ratio_limit.u32 = be_reg->isp_demosaic_cbcr_ratio_limit.u32;
    o_isp_demosaic_cbcr_ratio_limit.bits.isp_demosaic_cbcr_ratio_high_limit = uisp_demosaic_cbcr_ratio_high_limit;
    be_reg->isp_demosaic_cbcr_ratio_limit.u32 = o_isp_demosaic_cbcr_ratio_limit.u32;
}
/*  description   set the value of the member isp_demosaic_fcr_gray_ratio.isp_demosaic_fcr_gray_ratio */
/*  input         unsigned int uisp_demosaic_fcr_gray_ratio  5 bits */
#define OT_ISP_DEMOSAIC_FCR_GRAY_RATIO_DEFAULT        0x4
static __inline td_void isp_demosaic_fcr_gray_ratio_write(isp_be_reg_type *be_reg, td_u32 demosaic_fcr_gray_ratio)
{
    u_isp_demosaic_fcr_gray_ratio o_isp_demosaic_fcr_gray_ratio;
    o_isp_demosaic_fcr_gray_ratio.u32 = be_reg->isp_demosaic_fcr_gray_ratio.u32;
    o_isp_demosaic_fcr_gray_ratio.bits.isp_demosaic_fcr_gray_ratio = demosaic_fcr_gray_ratio;
    be_reg->isp_demosaic_fcr_gray_ratio.u32 = o_isp_demosaic_fcr_gray_ratio.u32;
}

/*  description : set the value of the member isp_demosaic_cx_var_rate.isp_demosaic_cx_var_min_rate */
/*  input       : unsigned int uisp_demosaic_cx_var_min_rate: 4 bits */
#define OT_ISP_DEMOSAIC_CX_VAR_MIN_RATE_DEFAULT       0
static __inline td_void isp_demosaic_cx_var_min_rate_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_cx_var_min_rate)
{
    u_isp_demosaic_cx_var_rate o_isp_demosaic_cx_var_rate;
    o_isp_demosaic_cx_var_rate.u32 = be_reg->isp_demosaic_cx_var_rate.u32;
    o_isp_demosaic_cx_var_rate.bits.isp_demosaic_cx_var_min_rate = uisp_demosaic_cx_var_min_rate;
    be_reg->isp_demosaic_cx_var_rate.u32 = o_isp_demosaic_cx_var_rate.u32;
}

/*  description : set the value of the member isp_demosaic_cx_var_rate.isp_demosaic_cx_var_max_rate */
/*  input       : unsigned int uisp_demosaic_cx_var_max_rate: 4 bits */
#define OT_ISP_DEMOSAIC_CX_VAR_MAX_RATE_DEFAULT       8
static __inline td_void isp_demosaic_cx_var_max_rate_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_cx_var_max_rate)
{
    u_isp_demosaic_cx_var_rate o_isp_demosaic_cx_var_rate;
    o_isp_demosaic_cx_var_rate.u32 = be_reg->isp_demosaic_cx_var_rate.u32;
    o_isp_demosaic_cx_var_rate.bits.isp_demosaic_cx_var_max_rate = uisp_demosaic_cx_var_max_rate;
    be_reg->isp_demosaic_cx_var_rate.u32 = o_isp_demosaic_cx_var_rate.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_diffthd.isp_demosaic_gcac_out_diffthd */
/*  input       : unsigned int uisp_demosaic_gcac_out_diffthd: 12 bits */
static __inline td_void isp_demosaic_gcac_out_diffthd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_out_diffthd)
{
    u_isp_demosaic_gcac_diffthd o_isp_demosaic_gcac_diffthd;
    o_isp_demosaic_gcac_diffthd.u32 = be_reg->isp_demosaic_gcac_diffthd.u32;
    o_isp_demosaic_gcac_diffthd.bits.isp_demosaic_gcac_out_diffthd = uisp_demosaic_gcac_out_diffthd;
    be_reg->isp_demosaic_gcac_diffthd.u32 = o_isp_demosaic_gcac_diffthd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_diffthd.isp_demosaic_gcac_desatuthr */
/*  input       : unsigned int uisp_demosaic_gcac_desatuthr: 12 bits */
static __inline td_void isp_demosaic_gcac_desatuthr_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_desatuthr)
{
    u_isp_demosaic_gcac_diffthd o_isp_demosaic_gcac_diffthd;
    o_isp_demosaic_gcac_diffthd.u32 = be_reg->isp_demosaic_gcac_diffthd.u32;
    o_isp_demosaic_gcac_diffthd.bits.isp_demosaic_gcac_desatuthr = uisp_demosaic_gcac_desatuthr;
    be_reg->isp_demosaic_gcac_diffthd.u32 = o_isp_demosaic_gcac_diffthd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamda_reg.isp_demosaic_gcac_edgemode */
/*  input       : unsigned int uisp_demosaic_gcac_edgemode: 2 bits */
static __inline td_void isp_demosaic_gcac_edgemode_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_edgemode)
{
    u_isp_demosaic_gcac_lamda_reg o_isp_demosaic_gcac_lamda_reg;
    o_isp_demosaic_gcac_lamda_reg.u32 = be_reg->isp_demosaic_gcac_lamda_reg.u32;
    o_isp_demosaic_gcac_lamda_reg.bits.isp_demosaic_gcac_edgemode = uisp_demosaic_gcac_edgemode;
    be_reg->isp_demosaic_gcac_lamda_reg.u32 = o_isp_demosaic_gcac_lamda_reg.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamda_reg.isp_demosaic_gcac_globalstr */
/*  input       : unsigned int uisp_demosaic_gcac_globalstr: 10 bits */
static __inline td_void isp_demosaic_gcac_globalstr_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_globalstr)
{
    u_isp_demosaic_gcac_lamda_reg o_isp_demosaic_gcac_lamda_reg;
    o_isp_demosaic_gcac_lamda_reg.u32 = be_reg->isp_demosaic_gcac_lamda_reg.u32;
    o_isp_demosaic_gcac_lamda_reg.bits.isp_demosaic_gcac_globalstr = uisp_demosaic_gcac_globalstr;
    be_reg->isp_demosaic_gcac_lamda_reg.u32 = o_isp_demosaic_gcac_lamda_reg.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamda_reg.isp_demosaic_gcac_normfactor */
/*  input       : unsigned int uisp_demosaic_gcac_normfactor: 4 bits */
static __inline td_void isp_demosaic_gcac_normfactor_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_normfactor)
{
    u_isp_demosaic_gcac_lamda_reg o_isp_demosaic_gcac_lamda_reg;
    o_isp_demosaic_gcac_lamda_reg.u32 = be_reg->isp_demosaic_gcac_lamda_reg.u32;
    o_isp_demosaic_gcac_lamda_reg.bits.isp_demosaic_gcac_normfactor = uisp_demosaic_gcac_normfactor;
    be_reg->isp_demosaic_gcac_lamda_reg.u32 = o_isp_demosaic_gcac_lamda_reg.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamda_reg.isp_demosaic_gcac_upperlmtsft */
/*  input       : unsigned int uisp_demosaic_gcac_upperlmtsft: 3 bits */
static __inline td_void isp_demosaic_gcac_upperlmtsft_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_upperlmtsft)
{
    u_isp_demosaic_gcac_lamda_reg o_isp_demosaic_gcac_lamda_reg;
    o_isp_demosaic_gcac_lamda_reg.u32 = be_reg->isp_demosaic_gcac_lamda_reg.u32;
    o_isp_demosaic_gcac_lamda_reg.bits.isp_demosaic_gcac_upperlmtsft = uisp_demosaic_gcac_upperlmtsft;
    be_reg->isp_demosaic_gcac_lamda_reg.u32 = o_isp_demosaic_gcac_lamda_reg.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamda_reg.isp_demosaic_gcac_lowerlmtsft */
/*  input       : unsigned int uisp_demosaic_gcac_lowerlmtsft: 3 bits */
static __inline td_void isp_demosaic_gcac_lowerlmtsft_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_lowerlmtsft)
{
    u_isp_demosaic_gcac_lamda_reg o_isp_demosaic_gcac_lamda_reg;
    o_isp_demosaic_gcac_lamda_reg.u32 = be_reg->isp_demosaic_gcac_lamda_reg.u32;
    o_isp_demosaic_gcac_lamda_reg.bits.isp_demosaic_gcac_lowerlmtsft = uisp_demosaic_gcac_lowerlmtsft;
    be_reg->isp_demosaic_gcac_lamda_reg.u32 = o_isp_demosaic_gcac_lamda_reg.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamdathd.isp_demosaic_gcac_lamdathd1 */
/*  input       : unsigned int uisp_demosaic_gcac_lamdathd1: 12 bits */
static __inline td_void isp_demosaic_gcac_lamdathd1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_lamdathd1)
{
    u_isp_demosaic_gcac_lamdathd o_isp_demosaic_gcac_lamdathd;
    o_isp_demosaic_gcac_lamdathd.u32 = be_reg->isp_demosaic_gcac_lamdathd.u32;
    o_isp_demosaic_gcac_lamdathd.bits.isp_demosaic_gcac_lamdathd1 = uisp_demosaic_gcac_lamdathd1;
    be_reg->isp_demosaic_gcac_lamdathd.u32 = o_isp_demosaic_gcac_lamdathd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamdathd.isp_demosaic_gcac_lamdathd2 */
/*  input       : unsigned int uisp_demosaic_gcac_lamdathd2: 12 bits */
static __inline td_void isp_demosaic_gcac_lamdathd2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_lamdathd2)
{
    u_isp_demosaic_gcac_lamdathd o_isp_demosaic_gcac_lamdathd;
    o_isp_demosaic_gcac_lamdathd.u32 = be_reg->isp_demosaic_gcac_lamdathd.u32;
    o_isp_demosaic_gcac_lamdathd.bits.isp_demosaic_gcac_lamdathd2 = uisp_demosaic_gcac_lamdathd2;
    be_reg->isp_demosaic_gcac_lamdathd.u32 = o_isp_demosaic_gcac_lamdathd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_thrbthd.isp_demosaic_gcac_thrbthd1 */
/*  input       : unsigned int uisp_demosaic_gcac_thrbthd1: 12 bits */
static __inline td_void isp_demosaic_gcac_thrbthd1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_thrbthd1)
{
    u_isp_demosaic_gcac_thrbthd o_isp_demosaic_gcac_thrbthd;
    o_isp_demosaic_gcac_thrbthd.u32 = be_reg->isp_demosaic_gcac_thrbthd.u32;
    o_isp_demosaic_gcac_thrbthd.bits.isp_demosaic_gcac_thrbthd1 = uisp_demosaic_gcac_thrbthd1;
    be_reg->isp_demosaic_gcac_thrbthd.u32 = o_isp_demosaic_gcac_thrbthd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_thrbthd.isp_demosaic_gcac_thrbthd2 */
/*  input       : unsigned int uisp_demosaic_gcac_thrbthd2: 12 bits */
static __inline td_void isp_demosaic_gcac_thrbthd2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_thrbthd2)
{
    u_isp_demosaic_gcac_thrbthd o_isp_demosaic_gcac_thrbthd;
    o_isp_demosaic_gcac_thrbthd.u32 = be_reg->isp_demosaic_gcac_thrbthd.u32;
    o_isp_demosaic_gcac_thrbthd.bits.isp_demosaic_gcac_thrbthd2 = uisp_demosaic_gcac_thrbthd2;
    be_reg->isp_demosaic_gcac_thrbthd.u32 = o_isp_demosaic_gcac_thrbthd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamdamul.isp_demosaic_gcac_thrbmul */
/*  input       : unsigned int uisp_demosaic_gcac_thrbmul: 16 bits */
static __inline td_void isp_demosaic_gcac_thrbmul_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_thrbmul)
{
    u_isp_demosaic_gcac_lamdamul o_isp_demosaic_gcac_lamdamul;
    o_isp_demosaic_gcac_lamdamul.u32 = be_reg->isp_demosaic_gcac_lamdamul.u32;
    o_isp_demosaic_gcac_lamdamul.bits.isp_demosaic_gcac_thrbmul = uisp_demosaic_gcac_thrbmul;
    be_reg->isp_demosaic_gcac_lamdamul.u32 = o_isp_demosaic_gcac_lamdamul.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_lamdamul.isp_demosaic_gcac_lamdamul */
/*  input       : unsigned int uisp_demosaic_gcac_lamdamul: 16 bits */
static __inline td_void isp_demosaic_gcac_lamdamul_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_lamdamul)
{
    u_isp_demosaic_gcac_lamdamul o_isp_demosaic_gcac_lamdamul;
    o_isp_demosaic_gcac_lamdamul.u32 = be_reg->isp_demosaic_gcac_lamdamul.u32;
    o_isp_demosaic_gcac_lamdamul.bits.isp_demosaic_gcac_lamdamul = uisp_demosaic_gcac_lamdamul;
    be_reg->isp_demosaic_gcac_lamdamul.u32 = o_isp_demosaic_gcac_lamdamul.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_distance_thd.isp_demosaic_gcac_distance_thd1 */
/*  input       : unsigned int uisp_demosaic_gcac_distance_thd1: 12 bits */
static __inline td_void isp_demosaic_gcac_distance_thd1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_distance_thd1)
{
    u_isp_demosaic_gcac_distance_thd o_isp_demosaic_gcac_distance_thd;
    o_isp_demosaic_gcac_distance_thd.u32 = be_reg->isp_demosaic_gcac_distance_thd.u32;
    o_isp_demosaic_gcac_distance_thd.bits.isp_demosaic_gcac_distance_thd1 = uisp_demosaic_gcac_distance_thd1;
    be_reg->isp_demosaic_gcac_distance_thd.u32 = o_isp_demosaic_gcac_distance_thd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_distance_thd.isp_demosaic_gcac_distance_thd2 */
/*  input       : unsigned int uisp_demosaic_gcac_distance_thd2: 12 bits */
static __inline td_void isp_demosaic_gcac_distance_thd2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_distance_thd2)
{
    u_isp_demosaic_gcac_distance_thd o_isp_demosaic_gcac_distance_thd;
    o_isp_demosaic_gcac_distance_thd.u32 = be_reg->isp_demosaic_gcac_distance_thd.u32;
    o_isp_demosaic_gcac_distance_thd.bits.isp_demosaic_gcac_distance_thd2 = uisp_demosaic_gcac_distance_thd2;
    be_reg->isp_demosaic_gcac_distance_thd.u32 = o_isp_demosaic_gcac_distance_thd.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_distance_mul.isp_demosaic_gcac_distance_mul */
/*  input       : unsigned int uisp_demosaic_gcac_distance_mul: 16 bits */
static __inline td_void isp_demosaic_gcac_distance_mul_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_distance_mul)
{
    u_isp_demosaic_gcac_distance_mul o_isp_demosaic_gcac_distance_mul;
    o_isp_demosaic_gcac_distance_mul.u32 = be_reg->isp_demosaic_gcac_distance_mul.u32;
    o_isp_demosaic_gcac_distance_mul.bits.isp_demosaic_gcac_distance_mul = uisp_demosaic_gcac_distance_mul;
    be_reg->isp_demosaic_gcac_distance_mul.u32 = o_isp_demosaic_gcac_distance_mul.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_distance_mul.isp_demosaic_gcac_tao */
/*  input       : unsigned int uisp_demosaic_gcac_tao: 5 bits */
static __inline td_void isp_demosaic_gcac_tao_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_tao)
{
    u_isp_demosaic_gcac_distance_mul o_isp_demosaic_gcac_distance_mul;
    o_isp_demosaic_gcac_distance_mul.u32 = be_reg->isp_demosaic_gcac_distance_mul.u32;
    o_isp_demosaic_gcac_distance_mul.bits.isp_demosaic_gcac_tao = uisp_demosaic_gcac_tao;
    be_reg->isp_demosaic_gcac_distance_mul.u32 = o_isp_demosaic_gcac_distance_mul.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_distance_mul.isp_demosaic_gcac_purplealpha */
/*  input       : unsigned int uisp_demosaic_gcac_purplealpha: 6 bits */
static __inline td_void isp_demosaic_gcac_purplealpha_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_purplealpha)
{
    u_isp_demosaic_gcac_distance_mul o_isp_demosaic_gcac_distance_mul;
    o_isp_demosaic_gcac_distance_mul.u32 = be_reg->isp_demosaic_gcac_distance_mul.u32;
    o_isp_demosaic_gcac_distance_mul.bits.isp_demosaic_gcac_purplealpha = uisp_demosaic_gcac_purplealpha;
    be_reg->isp_demosaic_gcac_distance_mul.u32 = o_isp_demosaic_gcac_distance_mul.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_edge_alpha.isp_demosaic_gcac_edge_alpha */
/*  input       : unsigned int uisp_demosaic_gcac_edge_alpha: 6 bits */
static __inline td_void isp_demosaic_gcac_edge_alpha_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_edge_alpha)
{
    u_isp_demosaic_gcac_edge_alpha o_isp_demosaic_gcac_edge_alpha;
    o_isp_demosaic_gcac_edge_alpha.u32 = be_reg->isp_demosaic_gcac_edge_alpha.u32;
    o_isp_demosaic_gcac_edge_alpha.bits.isp_demosaic_gcac_edge_alpha = uisp_demosaic_gcac_edge_alpha;
    be_reg->isp_demosaic_gcac_edge_alpha.u32 = o_isp_demosaic_gcac_edge_alpha.u32;
}

/*  description : set the value of the member isp_demosaic_gcac_edge_alpha.isp_demosaic_gcac_desatuthr_hig */
/*  input       : unsigned int uisp_demosaic_gcac_desatuthr_hig: 14 bits */
static __inline td_void isp_demosaic_gcac_desatuthr_hig_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_desatuthr_hig)
{
    u_isp_demosaic_gcac_edge_alpha o_isp_demosaic_gcac_edge_alpha;
    o_isp_demosaic_gcac_edge_alpha.u32 = be_reg->isp_demosaic_gcac_edge_alpha.u32;
    o_isp_demosaic_gcac_edge_alpha.bits.isp_demosaic_gcac_desatuthr_hig = uisp_demosaic_gcac_desatuthr_hig;
    be_reg->isp_demosaic_gcac_edge_alpha.u32 = o_isp_demosaic_gcac_edge_alpha.u32;
}
/*  description : set the value of the member isp_demosaic_gcac_edge_alpha.isp_demosaic_gcac_rb_diff_thd */
/*  input       : unsigned int uisp_demosaic_gcac_rb_diff_thd: 12 bits */
static __inline td_void isp_demosaic_gcac_rb_diff_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_gcac_rb_diff_thd)
{
    u_isp_demosaic_gcac_edge_alpha o_isp_demosaic_gcac_edge_alpha;
    o_isp_demosaic_gcac_edge_alpha.u32 = be_reg->isp_demosaic_gcac_edge_alpha.u32;
    o_isp_demosaic_gcac_edge_alpha.bits.isp_demosaic_gcac_rb_diff_thd = uisp_demosaic_gcac_rb_diff_thd;
    be_reg->isp_demosaic_gcac_edge_alpha.u32 = o_isp_demosaic_gcac_edge_alpha.u32;
}

/*  description : set the value of the member isp_demosaic_filter_blur_str_1.isp_demosaic_filter_f3 */
/*  input       : unsigned int uisp_demosaic_filter_f3: 8 bits */
#define OT_ISP_DEMOSAIC_LPF_F3_DEFAULT                0x7
static __inline td_void isp_demosaic_filter_f3_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_filter_f3)
{
    u_isp_demosaic_filter_blur_str_1 o_isp_demosaic_filter_blur_str_1;
    o_isp_demosaic_filter_blur_str_1.u32 = be_reg->isp_demosaic_filter_blur_str_1.u32;
    o_isp_demosaic_filter_blur_str_1.bits.isp_demosaic_filter_f3 = uisp_demosaic_filter_f3;
    be_reg->isp_demosaic_filter_blur_str_1.u32 = o_isp_demosaic_filter_blur_str_1.u32;
}

/*  description : set the value of the member isp_demosaic_filter_blur_str_1.isp_demosaic_filter_f2 */
/*  input       : unsigned int uisp_demosaic_filter_f2: 6 bits */
#define OT_ISP_DEMOSAIC_LPF_F2_DEFAULT                0x18
static __inline td_void isp_demosaic_filter_f2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_filter_f2)
{
    u_isp_demosaic_filter_blur_str_1 o_isp_demosaic_filter_blur_str_1;
    o_isp_demosaic_filter_blur_str_1.u32 = be_reg->isp_demosaic_filter_blur_str_1.u32;
    o_isp_demosaic_filter_blur_str_1.bits.isp_demosaic_filter_f2 = uisp_demosaic_filter_f2;
    be_reg->isp_demosaic_filter_blur_str_1.u32 = o_isp_demosaic_filter_blur_str_1.u32;
}

/*  description : set the value of the member isp_demosaic_filter_blur_str_1.isp_demosaic_filter_f1 */
/*  input       : unsigned int uisp_demosaic_filter_f1: 5 bits */
#define OT_ISP_DEMOSAIC_LPF_F1_DEFAULT                0x4
static __inline td_void isp_demosaic_filter_f1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_filter_f1)
{
    u_isp_demosaic_filter_blur_str_1 o_isp_demosaic_filter_blur_str_1;
    o_isp_demosaic_filter_blur_str_1.u32 = be_reg->isp_demosaic_filter_blur_str_1.u32;
    o_isp_demosaic_filter_blur_str_1.bits.isp_demosaic_filter_f1 = uisp_demosaic_filter_f1;
    be_reg->isp_demosaic_filter_blur_str_1.u32 = o_isp_demosaic_filter_blur_str_1.u32;
}

/*  description : set the value of the member isp_demosaic_filter_blur_str_1.isp_demosaic_filter_f0 */
/*  input       : unsigned int uisp_demosaic_filter_f0: 3 bits */
#define OT_ISP_DEMOSAIC_LPF_F0_DEFAULT                0x4
static __inline td_void isp_demosaic_filter_f0_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_filter_f0)
{
    u_isp_demosaic_filter_blur_str_1 o_isp_demosaic_filter_blur_str_1;
    o_isp_demosaic_filter_blur_str_1.u32 = be_reg->isp_demosaic_filter_blur_str_1.u32;
    o_isp_demosaic_filter_blur_str_1.bits.isp_demosaic_filter_f0 = uisp_demosaic_filter_f0;
    be_reg->isp_demosaic_filter_blur_str_1.u32 = o_isp_demosaic_filter_blur_str_1.u32;
}

/*  description : set the value of the member isp_demosaic_filter_blur_str_2.isp_demosaic_filter_blur1 */
/*  input       : unsigned int uisp_demosaic_filter_blur1: 9 bits */
#define OT_ISP_DEMOSAIC_FILTER_BLUR_TH_LOW_DEFAULT    0xF0
static __inline td_void isp_demosaic_filter_blur1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_filter_blur1)
{
    u_isp_demosaic_filter_blur_str_2 o_isp_demosaic_filter_blur_str_2;
    o_isp_demosaic_filter_blur_str_2.u32 = be_reg->isp_demosaic_filter_blur_str_2.u32;
    o_isp_demosaic_filter_blur_str_2.bits.isp_demosaic_filter_blur1 = uisp_demosaic_filter_blur1;
    be_reg->isp_demosaic_filter_blur_str_2.u32 = o_isp_demosaic_filter_blur_str_2.u32;
}

/*  description : set the value of the member isp_demosaic_filter_blur_str_2.isp_demosaic_filter_blur2 */
/*  input       : unsigned int uisp_demosaic_filter_blur2: 9 bits */
#define OT_ISP_DEMOSAIC_FILTER_BLUR_TH_HIG_DEFAULT    0xF0
static __inline td_void isp_demosaic_filter_blur2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_filter_blur2)
{
    u_isp_demosaic_filter_blur_str_2 o_isp_demosaic_filter_blur_str_2;
    o_isp_demosaic_filter_blur_str_2.u32 = be_reg->isp_demosaic_filter_blur_str_2.u32;
    o_isp_demosaic_filter_blur_str_2.bits.isp_demosaic_filter_blur2 = uisp_demosaic_filter_blur2;
    be_reg->isp_demosaic_filter_blur_str_2.u32 = o_isp_demosaic_filter_blur_str_2.u32;
}

/*  description : set the value of the member isp_demosaic_fcr_hf_thresh.isp_demosaic_fcr_hfthresh1 */
/*  input       : unsigned int uisp_demosaic_fcr_hfthresh1: 10 bits */
#define OT_ISP_DEMOSAIC_FCR_HF_THRESH_LOW_DEFAULT      48
static __inline td_void isp_demosaic_fcr_hfthresh1_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_hfthresh1)
{
    u_isp_demosaic_fcr_hf_thresh o_isp_demosaic_fcr_hf_thresh;
    o_isp_demosaic_fcr_hf_thresh.u32 = be_reg->isp_demosaic_fcr_hf_thresh.u32;
    o_isp_demosaic_fcr_hf_thresh.bits.isp_demosaic_fcr_hfthresh1 = uisp_demosaic_fcr_hfthresh1;
    be_reg->isp_demosaic_fcr_hf_thresh.u32 = o_isp_demosaic_fcr_hf_thresh.u32;
}

/*  description : set the value of the member isp_demosaic_fcr_hf_thresh.isp_demosaic_fcr_hfthresh2 */
/*  input       : unsigned int uisp_demosaic_fcr_hfthresh2: 10 bits */
#define OT_ISP_DEMOSAIC_FCR_HF_THRESH_HIGH_DEFAULT    128
static __inline td_void isp_demosaic_fcr_hfthresh2_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_fcr_hfthresh2)
{
    u_isp_demosaic_fcr_hf_thresh o_isp_demosaic_fcr_hf_thresh;
    o_isp_demosaic_fcr_hf_thresh.u32 = be_reg->isp_demosaic_fcr_hf_thresh.u32;
    o_isp_demosaic_fcr_hf_thresh.bits.isp_demosaic_fcr_hfthresh2 = uisp_demosaic_fcr_hfthresh2;
    be_reg->isp_demosaic_fcr_hf_thresh.u32 = o_isp_demosaic_fcr_hf_thresh.u32;
}

/*  description : set the value of the member isp_demosaic_nddm_coef.isp_demosaic_nddm_str */
/*  input       : unsigned int uisp_demosaic_nddm_str: 9 bits */
#define OT_ISP_DEMOSAIC_NDDM_NDDMSTR_DEFAULT           64
static __inline td_void isp_demosaic_nddm_str_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_nddm_str)
{
    u_isp_demosaic_nddm_coef o_isp_demosaic_nddm_coef;
    o_isp_demosaic_nddm_coef.u32 = be_reg->isp_demosaic_nddm_coef.u32;
    o_isp_demosaic_nddm_coef.bits.isp_demosaic_nddm_str = uisp_demosaic_nddm_str;
    be_reg->isp_demosaic_nddm_coef.u32 = o_isp_demosaic_nddm_coef.u32;
}

/*  description : set the value of the member isp_demosaic_nddm_coef.isp_demosaic_nddm_eps_sft */
/*  input       : unsigned int uisp_demosaic_nddm_eps_sft: 4 bits */
#define OT_ISP_DEMOSAIC_NDDM_EPS_SFT_DEFAULT           5
static __inline td_void isp_demosaic_nddm_eps_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_nddm_eps_sft)
{
    u_isp_demosaic_nddm_coef o_isp_demosaic_nddm_coef;
    o_isp_demosaic_nddm_coef.u32 = be_reg->isp_demosaic_nddm_coef.u32;
    o_isp_demosaic_nddm_coef.bits.isp_demosaic_nddm_eps_sft = uisp_demosaic_nddm_eps_sft;
    be_reg->isp_demosaic_nddm_coef.u32 = o_isp_demosaic_nddm_coef.u32;
}

/*  description : set the value of the member isp_demosaic_nddm_coef.isp_demosaic_nddm_ehcgray */
/*  input       : unsigned int uisp_demosaic_nddm_ehcgray: 7 bits */
#define OT_ISP_DEMOSAIC_NDDM_EHCGRAY_DEFAULT           30
static __inline td_void isp_demosaic_nddm_ehcgray_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_nddm_ehcgray)
{
    u_isp_demosaic_nddm_coef o_isp_demosaic_nddm_coef;
    o_isp_demosaic_nddm_coef.u32 = be_reg->isp_demosaic_nddm_coef.u32;
    o_isp_demosaic_nddm_coef.bits.isp_demosaic_nddm_ehcgray = uisp_demosaic_nddm_ehcgray;
    be_reg->isp_demosaic_nddm_coef.u32 = o_isp_demosaic_nddm_coef.u32;
}
/*  description   set the value of the member isp_demosaic_desat_thr.isp_demosaic_desat_thresh1 */
/*  input         unsigned int uisp_demosaic_desat_thresh1  10 bits */
#define OT_ISP_DEMOSAIC_DESAT_THRESH1_DEFAULT         96
static __inline td_void isp_demosaic_desat_thresh1_write(isp_be_reg_type *be_reg, td_u32 demosaic_desat_thresh1)
{
    u_isp_demosaic_desat_thr o_isp_demosaic_desat_thr;
    o_isp_demosaic_desat_thr.u32 = be_reg->isp_demosaic_desat_thr.u32;
    o_isp_demosaic_desat_thr.bits.isp_demosaic_desat_thresh1 = demosaic_desat_thresh1;
    be_reg->isp_demosaic_desat_thr.u32 = o_isp_demosaic_desat_thr.u32;
}
/*  description   set the value of the member isp_demosaic_desat_thr.isp_demosaic_desat_thresh2 */
/*  input         unsigned int uisp_demosaic_desat_thresh2  10 bits */
#define OT_ISP_DEMOSAIC_DESAT_THRESH2_DEFAULT         192
static __inline td_void isp_demosaic_desat_thresh2_write(isp_be_reg_type *be_reg, td_u32 demosaic_desat_thresh2)
{
    u_isp_demosaic_desat_thr o_isp_demosaic_desat_thr;
    o_isp_demosaic_desat_thr.u32 = be_reg->isp_demosaic_desat_thr.u32;
    o_isp_demosaic_desat_thr.bits.isp_demosaic_desat_thresh2 = demosaic_desat_thresh2;
    be_reg->isp_demosaic_desat_thr.u32 = o_isp_demosaic_desat_thr.u32;
}
/*  description   set the value of the member isp_demosaic_desat_bldr.isp_demosaic_desat_hig */
/*  input         unsigned int uisp_demosaic_desat_hig  9 bits */
#define OT_ISP_DEMOSAIC_DESAT_HIG_DEFAULT             256
static __inline td_void isp_demosaic_desat_hig_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_desat_hig)
{
    u_isp_demosaic_desat_bldr o_isp_demosaic_desat_bldr;
    o_isp_demosaic_desat_bldr.u32 = be_reg->isp_demosaic_desat_bldr.u32;
    o_isp_demosaic_desat_bldr.bits.isp_demosaic_desat_hig = uisp_demosaic_desat_hig;
    be_reg->isp_demosaic_desat_bldr.u32 = o_isp_demosaic_desat_bldr.u32;
}
/*  description   set the value of the member isp_demosaic_desat_bldr.isp_demosaic_desat_low */
/*  input         unsigned int uisp_demosaic_desat_low  9 bits */
#define OT_ISP_DEMOSAIC_DESAT_LOW_DEFAULT             166
static __inline td_void isp_demosaic_desat_low_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_desat_low)
{
    u_isp_demosaic_desat_bldr o_isp_demosaic_desat_bldr;
    o_isp_demosaic_desat_bldr.u32 = be_reg->isp_demosaic_desat_bldr.u32;
    o_isp_demosaic_desat_bldr.bits.isp_demosaic_desat_low = uisp_demosaic_desat_low;
    be_reg->isp_demosaic_desat_bldr.u32 = o_isp_demosaic_desat_bldr.u32;
}
/*  description   set the value of the member isp_demosaic_desat_bldr_ratio.isp_demosaic_desat_ratio */
/*  input         unsigned int uisp_demosaic_desat_ratio  13 bits */
#define OT_ISP_DEMOSAIC_DESAT_RATIO_DEFAULT           (-15)
static __inline td_void isp_demosaic_desat_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_desat_ratio)
{
    u_isp_demosaic_desat_bldr_ratio o_isp_demosaic_desat_bldr_ratio;
    o_isp_demosaic_desat_bldr_ratio.u32 = be_reg->isp_demosaic_desat_bldr_ratio.u32;
    o_isp_demosaic_desat_bldr_ratio.bits.isp_demosaic_desat_ratio = uisp_demosaic_desat_ratio;
    be_reg->isp_demosaic_desat_bldr_ratio.u32 = o_isp_demosaic_desat_bldr_ratio.u32;
}
/*  description   set the value of the member isp_demosaic_desat_protect.isp_demosaic_desat_protect_sl */
/*  input         unsigned int uisp_demosaic_desat_protect_sl  10 bits */
#define OT_ISP_DEMOSAIC_DESAT_PROTECT_SL_DEFAULT      180
static __inline td_void isp_demosaic_desat_protect_sl_write(isp_be_reg_type *be_reg,
    td_u32 uisp_demosaic_desat_protect_sl)
{
    u_isp_demosaic_desat_protect o_isp_demosaic_desat_protect;
    o_isp_demosaic_desat_protect.u32 = be_reg->isp_demosaic_desat_protect.u32;
    o_isp_demosaic_desat_protect.bits.isp_demosaic_desat_protect_sl = uisp_demosaic_desat_protect_sl;
    be_reg->isp_demosaic_desat_protect.u32 = o_isp_demosaic_desat_protect.u32;
}
/*  description   set the value of the member isp_demosaic_desat_protect.isp_demosaic_desat_protect_th */
/*  input         unsigned int uisp_demosaic_desat_protect_th  10 bits */
#define OT_ISP_DEMOSAIC_DESAT_PROTECT_TH_DEFAULT      128
static __inline td_void isp_demosaic_desat_protect_th_write(isp_be_reg_type *be_reg,
    td_u32 uisp_demosaic_desat_protect_th)
{
    u_isp_demosaic_desat_protect o_isp_demosaic_desat_protect;
    o_isp_demosaic_desat_protect.u32 = be_reg->isp_demosaic_desat_protect.u32;
    o_isp_demosaic_desat_protect.bits.isp_demosaic_desat_protect_th = uisp_demosaic_desat_protect_th;
    be_reg->isp_demosaic_desat_protect.u32 = o_isp_demosaic_desat_protect.u32;
}


/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_benlumactrl */
/*  input         unsigned int uisp_sharpen_benlumactrl  1 bits */
static __inline td_void isp_sharpen_benlumactrl_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_benlumactrl)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benlumactrl = uisp_sharpen_benlumactrl;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}
/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_vcds_filterv */
/*  input         unsigned int uisp_sharpen_vcds_filterv  1 bits */
static __inline td_void isp_sharpen_vcds_filterv_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_vcds_filterv)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_vcds_filterv = uisp_sharpen_vcds_filterv;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}
/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_benshtvar_sel */
/*  input         unsigned int uisp_sharpen_benshtvar_sel  1 bits */
static __inline td_void isp_sharpen_benshtvar_sel_write(isp_be_reg_type *be_reg, td_u32 sharpen_benshtvar_sel)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benshtvar_sel = sharpen_benshtvar_sel;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}
/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_benshtctrlbyvar */
/*  input         unsigned int uisp_sharpen_benshtctrlbyvar  1 bits */
static __inline td_void isp_sharpen_benshtctrlbyvar_write(isp_be_reg_type *be_reg, td_u32 sharpen_benshtctrlbyvar)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benshtctrlbyvar = sharpen_benshtctrlbyvar;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}

/*  description : set the value of the member isp_sharpen_ctrl.isp_sharpen_benskinctrl */
/*  input       : unsigned int uisp_sharpen_benskinctrl: 1 bits */
static __inline td_void isp_sharpen_benskinctrl_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_benskinctrl)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benskinctrl = uisp_sharpen_benskinctrl;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}

/*  description : set the value of the member isp_sharpen_ctrl.isp_sharpen_benchrctrl */
/*  input       : unsigned int uisp_sharpen_benchrctrl: 1 bits */
static __inline td_void isp_sharpen_benchrctrl_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_benchrctrl)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benchrctrl = uisp_sharpen_benchrctrl;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}
/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_detailthd_sel */
/*  input         unsigned int uisp_sharpen_detailthd_sel  3 bits */
static __inline td_void isp_sharpen_detailthd_sel_write(isp_be_reg_type *be_reg, td_u32 sharpen_detailthd_sel)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_detailthd_sel = sharpen_detailthd_sel;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}
/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_bendetailctrl */
/*  input         unsigned int uisp_sharpen_bendetailctrl  1 bits */
static __inline td_void isp_sharpen_bendetailctrl_write(isp_be_reg_type *be_reg, td_u32 sharpen_bendetailctrl)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_bendetailctrl = sharpen_bendetailctrl;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}

/*  description   set the value of the member isp_sharpen_ctrl.isp_sharpen_ben8dir_sel */
/*  input         unsigned int isp_sharpen_ben8dir_sel  1 bits */
static __inline td_void isp_sharpen_ben8dir_sel_write(isp_be_reg_type *be_reg, td_u32 sharpen_ben8dir_sel)
{
    u_isp_sharpen_ctrl o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.u32 = be_reg->isp_sharpen_ctrl.u32;
    o_isp_sharpen_ctrl.bits.isp_sharpen_ben8dir_sel = sharpen_ben8dir_sel;
    be_reg->isp_sharpen_ctrl.u32 = o_isp_sharpen_ctrl.u32;
}

/*  description   set the value of the member isp_sharpen_mhfthd.isp_sharpen_mhfthdsftd */
/*  input         unsigned int uisp_sharpen_mhfthdsftd  4 bits */
static __inline td_void isp_sharpen_mhfthdsftd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mhfthdsftd)
{
    u_isp_sharpen_mhfthd o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = be_reg->isp_sharpen_mhfthd.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdsftd = uisp_sharpen_mhfthdsftd;
    be_reg->isp_sharpen_mhfthd.u32 = o_isp_sharpen_mhfthd.u32;
}
/*  description   set the value of the member isp_sharpen_mhfthd.isp_sharpen_mhfthdseld */
/*  input         unsigned int uisp_sharpen_mhfthdseld  2 bits */
static __inline td_void isp_sharpen_mhfthdseld_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mhfthdseld)
{
    u_isp_sharpen_mhfthd o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = be_reg->isp_sharpen_mhfthd.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdseld = uisp_sharpen_mhfthdseld;
    be_reg->isp_sharpen_mhfthd.u32 = o_isp_sharpen_mhfthd.u32;
}
/*  description   set the value of the member isp_sharpen_mhfthd.isp_sharpen_mhfthdsftud */
/*  input         unsigned int uisp_sharpen_mhfthdsftud  4 bits */
static __inline td_void isp_sharpen_mhfthdsftud_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mhfthdsftud)
{
    u_isp_sharpen_mhfthd o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = be_reg->isp_sharpen_mhfthd.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdsftud = uisp_sharpen_mhfthdsftud;
    be_reg->isp_sharpen_mhfthd.u32 = o_isp_sharpen_mhfthd.u32;
}
/*  description   set the value of the member isp_sharpen_mhfthd.isp_sharpen_mhfthdselud */
/*  input         unsigned int uisp_sharpen_mhfthdselud  2 bits */
static __inline td_void isp_sharpen_mhfthdselud_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mhfthdselud)
{
    u_isp_sharpen_mhfthd o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.u32 = be_reg->isp_sharpen_mhfthd.u32;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdselud = uisp_sharpen_mhfthdselud;
    be_reg->isp_sharpen_mhfthd.u32 = o_isp_sharpen_mhfthd.u32;
}

/*  description   set the value of the member isp_sharpen_dirvar.isp_sharpen_dirvarsft */
/*  input         unsigned int uisp_sharpen_dirvarsft  4 bits */
static __inline td_void isp_sharpen_dirvarsft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirvarsft)
{
    u_isp_sharpen_dirvar o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = be_reg->isp_sharpen_dirvar.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirvarsft = uisp_sharpen_dirvarsft;
    be_reg->isp_sharpen_dirvar.u32 = o_isp_sharpen_dirvar.u32;
}
/*  description   set the value of the member isp_sharpen_dirvar.isp_sharpen_dirvarscale */
/*  input         unsigned int uisp_sharpen_dirvarscale  4 bits */
static __inline td_void isp_sharpen_dirvarscale_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirvarscale)
{
    u_isp_sharpen_dirvar o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = be_reg->isp_sharpen_dirvar.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirvarscale = uisp_sharpen_dirvarscale;
    be_reg->isp_sharpen_dirvar.u32 = o_isp_sharpen_dirvar.u32;
}
/*  description   set the value of the member isp_sharpen_dirvar.isp_sharpen_dirrly0 */
/*  input         unsigned int uisp_sharpen_dirrly0  7 bits */
static __inline td_void isp_sharpen_dirrly0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirrly0)
{
    u_isp_sharpen_dirvar o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = be_reg->isp_sharpen_dirvar.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirrly0 = uisp_sharpen_dirrly0;
    be_reg->isp_sharpen_dirvar.u32 = o_isp_sharpen_dirvar.u32;
}
/*  description   set the value of the member isp_sharpen_dirvar.isp_sharpen_dirrly1 */
/*  input         unsigned int uisp_sharpen_dirrly1  7 bits */
static __inline td_void isp_sharpen_dirrly1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirrly1)
{
    u_isp_sharpen_dirvar o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.u32 = be_reg->isp_sharpen_dirvar.u32;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirrly1 = uisp_sharpen_dirrly1;
    be_reg->isp_sharpen_dirvar.u32 = o_isp_sharpen_dirvar.u32;
}
/*  description   set the value of the member isp_sharpen_dirdiff.isp_sharpen_dirdiffsft */
/*  input         unsigned int uisp_sharpen_dirdiffsft  6 bits */
static __inline td_void isp_sharpen_dirdiffsft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirdiffsft)
{
    u_isp_sharpen_dirdiff o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.u32 = be_reg->isp_sharpen_dirdiff.u32;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirdiffsft = uisp_sharpen_dirdiffsft;
    be_reg->isp_sharpen_dirdiff.u32 = o_isp_sharpen_dirdiff.u32;
}
/*  description   set the value of the member isp_sharpen_dirdiff.isp_sharpen_dirrt0 */
/*  input         unsigned int uisp_sharpen_dirrt0  5 bits */
static __inline td_void isp_sharpen_dirrt0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirrt0)
{
    u_isp_sharpen_dirdiff o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.u32 = be_reg->isp_sharpen_dirdiff.u32;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirrt0 = uisp_sharpen_dirrt0;
    be_reg->isp_sharpen_dirdiff.u32 = o_isp_sharpen_dirdiff.u32;
}
/*  description   set the value of the member isp_sharpen_dirdiff.isp_sharpen_dirrt1 */
/*  input         unsigned int uisp_sharpen_dirrt1  5 bits */
static __inline td_void isp_sharpen_dirrt1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirrt1)
{
    u_isp_sharpen_dirdiff o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.u32 = be_reg->isp_sharpen_dirdiff.u32;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirrt1 = uisp_sharpen_dirrt1;
    be_reg->isp_sharpen_dirdiff.u32 = o_isp_sharpen_dirdiff.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt0.isp_sharpen_lumawgt0 */
/*  input         unsigned int uisp_sharpen_lumawgt0  6 bits */
static __inline td_void isp_sharpen_lumawgt0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt0)
{
    u_isp_sharpen_lumawgt0 o_isp_sharpen_lumawgt0;
    o_isp_sharpen_lumawgt0.u32 = be_reg->isp_sharpen_lumawgt0.u32;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt0 = uisp_sharpen_lumawgt0;
    be_reg->isp_sharpen_lumawgt0.u32 = o_isp_sharpen_lumawgt0.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt0.isp_sharpen_lumawgt1 */
/*  input         unsigned int uisp_sharpen_lumawgt1  6 bits */
static __inline td_void isp_sharpen_lumawgt1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt1)
{
    u_isp_sharpen_lumawgt0 o_isp_sharpen_lumawgt0;
    o_isp_sharpen_lumawgt0.u32 = be_reg->isp_sharpen_lumawgt0.u32;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt1 = uisp_sharpen_lumawgt1;
    be_reg->isp_sharpen_lumawgt0.u32 = o_isp_sharpen_lumawgt0.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt0.isp_sharpen_lumawgt2 */
/*  input         unsigned int uisp_sharpen_lumawgt2  6 bits */
static __inline td_void isp_sharpen_lumawgt2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt2)
{
    u_isp_sharpen_lumawgt0 o_isp_sharpen_lumawgt0;
    o_isp_sharpen_lumawgt0.u32 = be_reg->isp_sharpen_lumawgt0.u32;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt2 = uisp_sharpen_lumawgt2;
    be_reg->isp_sharpen_lumawgt0.u32 = o_isp_sharpen_lumawgt0.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt0.isp_sharpen_lumawgt3 */
/*  input         unsigned int uisp_sharpen_lumawgt3  6 bits */
static __inline td_void isp_sharpen_lumawgt3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt3)
{
    u_isp_sharpen_lumawgt0 o_isp_sharpen_lumawgt0;
    o_isp_sharpen_lumawgt0.u32 = be_reg->isp_sharpen_lumawgt0.u32;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt3 = uisp_sharpen_lumawgt3;
    be_reg->isp_sharpen_lumawgt0.u32 = o_isp_sharpen_lumawgt0.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt0.isp_sharpen_lumawgt4 */
/*  input         unsigned int uisp_sharpen_lumawgt4  6 bits */
static __inline td_void isp_sharpen_lumawgt4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt4)
{
    u_isp_sharpen_lumawgt0 o_isp_sharpen_lumawgt0;
    o_isp_sharpen_lumawgt0.u32 = be_reg->isp_sharpen_lumawgt0.u32;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt4 = uisp_sharpen_lumawgt4;
    be_reg->isp_sharpen_lumawgt0.u32 = o_isp_sharpen_lumawgt0.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt1.isp_sharpen_lumawgt5 */
/*  input         unsigned int uisp_sharpen_lumawgt5  6 bits */
static __inline td_void isp_sharpen_lumawgt5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt5)
{
    u_isp_sharpen_lumawgt1 o_isp_sharpen_lumawgt1;
    o_isp_sharpen_lumawgt1.u32 = be_reg->isp_sharpen_lumawgt1.u32;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt5 = uisp_sharpen_lumawgt5;
    be_reg->isp_sharpen_lumawgt1.u32 = o_isp_sharpen_lumawgt1.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt1.isp_sharpen_lumawgt6 */
/*  input         unsigned int uisp_sharpen_lumawgt6  6 bits */
static __inline td_void isp_sharpen_lumawgt6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt6)
{
    u_isp_sharpen_lumawgt1 o_isp_sharpen_lumawgt1;
    o_isp_sharpen_lumawgt1.u32 = be_reg->isp_sharpen_lumawgt1.u32;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt6 = uisp_sharpen_lumawgt6;
    be_reg->isp_sharpen_lumawgt1.u32 = o_isp_sharpen_lumawgt1.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt1.isp_sharpen_lumawgt7 */
/*  input         unsigned int uisp_sharpen_lumawgt7  6 bits */
static __inline td_void isp_sharpen_lumawgt7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt7)
{
    u_isp_sharpen_lumawgt1 o_isp_sharpen_lumawgt1;
    o_isp_sharpen_lumawgt1.u32 = be_reg->isp_sharpen_lumawgt1.u32;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt7 = uisp_sharpen_lumawgt7;
    be_reg->isp_sharpen_lumawgt1.u32 = o_isp_sharpen_lumawgt1.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt1.isp_sharpen_lumawgt8 */
/*  input         unsigned int uisp_sharpen_lumawgt8  6 bits */
static __inline td_void isp_sharpen_lumawgt8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt8)
{
    u_isp_sharpen_lumawgt1 o_isp_sharpen_lumawgt1;
    o_isp_sharpen_lumawgt1.u32 = be_reg->isp_sharpen_lumawgt1.u32;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt8 = uisp_sharpen_lumawgt8;
    be_reg->isp_sharpen_lumawgt1.u32 = o_isp_sharpen_lumawgt1.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt1.isp_sharpen_lumawgt9 */
/*  input         unsigned int uisp_sharpen_lumawgt9  6 bits */
static __inline td_void isp_sharpen_lumawgt9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt9)
{
    u_isp_sharpen_lumawgt1 o_isp_sharpen_lumawgt1;
    o_isp_sharpen_lumawgt1.u32 = be_reg->isp_sharpen_lumawgt1.u32;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt9 = uisp_sharpen_lumawgt9;
    be_reg->isp_sharpen_lumawgt1.u32 = o_isp_sharpen_lumawgt1.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt2.isp_sharpen_lumawgt10 */
/*  input         unsigned int uisp_sharpen_lumawgt10  6 bits */
static __inline td_void isp_sharpen_lumawgt10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt10)
{
    u_isp_sharpen_lumawgt2 o_isp_sharpen_lumawgt2;
    o_isp_sharpen_lumawgt2.u32 = be_reg->isp_sharpen_lumawgt2.u32;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt10 = uisp_sharpen_lumawgt10;
    be_reg->isp_sharpen_lumawgt2.u32 = o_isp_sharpen_lumawgt2.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt2.isp_sharpen_lumawgt11 */
/*  input         unsigned int uisp_sharpen_lumawgt11  6 bits */
static __inline td_void isp_sharpen_lumawgt11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt11)
{
    u_isp_sharpen_lumawgt2 o_isp_sharpen_lumawgt2;
    o_isp_sharpen_lumawgt2.u32 = be_reg->isp_sharpen_lumawgt2.u32;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt11 = uisp_sharpen_lumawgt11;
    be_reg->isp_sharpen_lumawgt2.u32 = o_isp_sharpen_lumawgt2.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt2.isp_sharpen_lumawgt12 */
/*  input         unsigned int uisp_sharpen_lumawgt12  6 bits */
static __inline td_void isp_sharpen_lumawgt12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt12)
{
    u_isp_sharpen_lumawgt2 o_isp_sharpen_lumawgt2;
    o_isp_sharpen_lumawgt2.u32 = be_reg->isp_sharpen_lumawgt2.u32;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt12 = uisp_sharpen_lumawgt12;
    be_reg->isp_sharpen_lumawgt2.u32 = o_isp_sharpen_lumawgt2.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt2.isp_sharpen_lumawgt13 */
/*  input         unsigned int uisp_sharpen_lumawgt13  6 bits */
static __inline td_void isp_sharpen_lumawgt13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt13)
{
    u_isp_sharpen_lumawgt2 o_isp_sharpen_lumawgt2;
    o_isp_sharpen_lumawgt2.u32 = be_reg->isp_sharpen_lumawgt2.u32;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt13 = uisp_sharpen_lumawgt13;
    be_reg->isp_sharpen_lumawgt2.u32 = o_isp_sharpen_lumawgt2.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt2.isp_sharpen_lumawgt14 */
/*  input         unsigned int uisp_sharpen_lumawgt14  6 bits */
static __inline td_void isp_sharpen_lumawgt14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt14)
{
    u_isp_sharpen_lumawgt2 o_isp_sharpen_lumawgt2;
    o_isp_sharpen_lumawgt2.u32 = be_reg->isp_sharpen_lumawgt2.u32;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt14 = uisp_sharpen_lumawgt14;
    be_reg->isp_sharpen_lumawgt2.u32 = o_isp_sharpen_lumawgt2.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt3.isp_sharpen_lumawgt15 */
/*  input         unsigned int uisp_sharpen_lumawgt15  6 bits */
static __inline td_void isp_sharpen_lumawgt15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt15)
{
    u_isp_sharpen_lumawgt3 o_isp_sharpen_lumawgt3;
    o_isp_sharpen_lumawgt3.u32 = be_reg->isp_sharpen_lumawgt3.u32;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt15 = uisp_sharpen_lumawgt15;
    be_reg->isp_sharpen_lumawgt3.u32 = o_isp_sharpen_lumawgt3.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt3.isp_sharpen_lumawgt16 */
/*  input         unsigned int uisp_sharpen_lumawgt16  6 bits */
static __inline td_void isp_sharpen_lumawgt16_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt16)
{
    u_isp_sharpen_lumawgt3 o_isp_sharpen_lumawgt3;
    o_isp_sharpen_lumawgt3.u32 = be_reg->isp_sharpen_lumawgt3.u32;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt16 = uisp_sharpen_lumawgt16;
    be_reg->isp_sharpen_lumawgt3.u32 = o_isp_sharpen_lumawgt3.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt3.isp_sharpen_lumawgt17 */
/*  input         unsigned int uisp_sharpen_lumawgt17  6 bits */
static __inline td_void isp_sharpen_lumawgt17_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt17)
{
    u_isp_sharpen_lumawgt3 o_isp_sharpen_lumawgt3;
    o_isp_sharpen_lumawgt3.u32 = be_reg->isp_sharpen_lumawgt3.u32;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt17 = uisp_sharpen_lumawgt17;
    be_reg->isp_sharpen_lumawgt3.u32 = o_isp_sharpen_lumawgt3.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt3.isp_sharpen_lumawgt18 */
/*  input         unsigned int uisp_sharpen_lumawgt18  6 bits */
static __inline td_void isp_sharpen_lumawgt18_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt18)
{
    u_isp_sharpen_lumawgt3 o_isp_sharpen_lumawgt3;
    o_isp_sharpen_lumawgt3.u32 = be_reg->isp_sharpen_lumawgt3.u32;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt18 = uisp_sharpen_lumawgt18;
    be_reg->isp_sharpen_lumawgt3.u32 = o_isp_sharpen_lumawgt3.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt3.isp_sharpen_lumawgt19 */
/*  input         unsigned int uisp_sharpen_lumawgt19  6 bits */
static __inline td_void isp_sharpen_lumawgt19_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt19)
{
    u_isp_sharpen_lumawgt3 o_isp_sharpen_lumawgt3;
    o_isp_sharpen_lumawgt3.u32 = be_reg->isp_sharpen_lumawgt3.u32;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt19 = uisp_sharpen_lumawgt19;
    be_reg->isp_sharpen_lumawgt3.u32 = o_isp_sharpen_lumawgt3.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt4.isp_sharpen_lumawgt20 */
/*  input         unsigned int uisp_sharpen_lumawgt20  6 bits */
static __inline td_void isp_sharpen_lumawgt20_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt20)
{
    u_isp_sharpen_lumawgt4 o_isp_sharpen_lumawgt4;
    o_isp_sharpen_lumawgt4.u32 = be_reg->isp_sharpen_lumawgt4.u32;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt20 = uisp_sharpen_lumawgt20;
    be_reg->isp_sharpen_lumawgt4.u32 = o_isp_sharpen_lumawgt4.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt4.isp_sharpen_lumawgt21 */
/*  input         unsigned int uisp_sharpen_lumawgt21  6 bits */
static __inline td_void isp_sharpen_lumawgt21_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt21)
{
    u_isp_sharpen_lumawgt4 o_isp_sharpen_lumawgt4;
    o_isp_sharpen_lumawgt4.u32 = be_reg->isp_sharpen_lumawgt4.u32;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt21 = uisp_sharpen_lumawgt21;
    be_reg->isp_sharpen_lumawgt4.u32 = o_isp_sharpen_lumawgt4.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt4.isp_sharpen_lumawgt22 */
/*  input         unsigned int uisp_sharpen_lumawgt22  6 bits */
static __inline td_void isp_sharpen_lumawgt22_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt22)
{
    u_isp_sharpen_lumawgt4 o_isp_sharpen_lumawgt4;
    o_isp_sharpen_lumawgt4.u32 = be_reg->isp_sharpen_lumawgt4.u32;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt22 = uisp_sharpen_lumawgt22;
    be_reg->isp_sharpen_lumawgt4.u32 = o_isp_sharpen_lumawgt4.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt4.isp_sharpen_lumawgt23 */
/*  input         unsigned int uisp_sharpen_lumawgt23  6 bits */
static __inline td_void isp_sharpen_lumawgt23_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt23)
{
    u_isp_sharpen_lumawgt4 o_isp_sharpen_lumawgt4;
    o_isp_sharpen_lumawgt4.u32 = be_reg->isp_sharpen_lumawgt4.u32;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt23 = uisp_sharpen_lumawgt23;
    be_reg->isp_sharpen_lumawgt4.u32 = o_isp_sharpen_lumawgt4.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt4.isp_sharpen_lumawgt24 */
/*  input         unsigned int uisp_sharpen_lumawgt24  6 bits */
static __inline td_void isp_sharpen_lumawgt24_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt24)
{
    u_isp_sharpen_lumawgt4 o_isp_sharpen_lumawgt4;
    o_isp_sharpen_lumawgt4.u32 = be_reg->isp_sharpen_lumawgt4.u32;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt24 = uisp_sharpen_lumawgt24;
    be_reg->isp_sharpen_lumawgt4.u32 = o_isp_sharpen_lumawgt4.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt5.isp_sharpen_lumawgt25 */
/*  input         unsigned int uisp_sharpen_lumawgt25  6 bits */
static __inline td_void isp_sharpen_lumawgt25_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt25)
{
    u_isp_sharpen_lumawgt5 o_isp_sharpen_lumawgt5;
    o_isp_sharpen_lumawgt5.u32 = be_reg->isp_sharpen_lumawgt5.u32;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt25 = uisp_sharpen_lumawgt25;
    be_reg->isp_sharpen_lumawgt5.u32 = o_isp_sharpen_lumawgt5.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt5.isp_sharpen_lumawgt26 */
/*  input         unsigned int uisp_sharpen_lumawgt26  6 bits */
static __inline td_void isp_sharpen_lumawgt26_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt26)
{
    u_isp_sharpen_lumawgt5 o_isp_sharpen_lumawgt5;
    o_isp_sharpen_lumawgt5.u32 = be_reg->isp_sharpen_lumawgt5.u32;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt26 = uisp_sharpen_lumawgt26;
    be_reg->isp_sharpen_lumawgt5.u32 = o_isp_sharpen_lumawgt5.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt5.isp_sharpen_lumawgt27 */
/*  input         unsigned int uisp_sharpen_lumawgt27  6 bits */
static __inline td_void isp_sharpen_lumawgt27_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt27)
{
    u_isp_sharpen_lumawgt5 o_isp_sharpen_lumawgt5;
    o_isp_sharpen_lumawgt5.u32 = be_reg->isp_sharpen_lumawgt5.u32;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt27 = uisp_sharpen_lumawgt27;
    be_reg->isp_sharpen_lumawgt5.u32 = o_isp_sharpen_lumawgt5.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt5.isp_sharpen_lumawgt28 */
/*  input         unsigned int uisp_sharpen_lumawgt28  6 bits */
static __inline td_void isp_sharpen_lumawgt28_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt28)
{
    u_isp_sharpen_lumawgt5 o_isp_sharpen_lumawgt5;
    o_isp_sharpen_lumawgt5.u32 = be_reg->isp_sharpen_lumawgt5.u32;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt28 = uisp_sharpen_lumawgt28;
    be_reg->isp_sharpen_lumawgt5.u32 = o_isp_sharpen_lumawgt5.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt5.isp_sharpen_lumawgt29 */
/*  input         unsigned int uisp_sharpen_lumawgt29  6 bits */
static __inline td_void isp_sharpen_lumawgt29_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt29)
{
    u_isp_sharpen_lumawgt5 o_isp_sharpen_lumawgt5;
    o_isp_sharpen_lumawgt5.u32 = be_reg->isp_sharpen_lumawgt5.u32;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt29 = uisp_sharpen_lumawgt29;
    be_reg->isp_sharpen_lumawgt5.u32 = o_isp_sharpen_lumawgt5.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt6.isp_sharpen_lumawgt30 */
/*  input         unsigned int uisp_sharpen_lumawgt30  6 bits */
static __inline td_void isp_sharpen_lumawgt30_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt30)
{
    u_isp_sharpen_lumawgt6 o_isp_sharpen_lumawgt6;
    o_isp_sharpen_lumawgt6.u32 = be_reg->isp_sharpen_lumawgt6.u32;
    o_isp_sharpen_lumawgt6.bits.isp_sharpen_lumawgt30 = uisp_sharpen_lumawgt30;
    be_reg->isp_sharpen_lumawgt6.u32 = o_isp_sharpen_lumawgt6.u32;
}
/*  description   set the value of the member isp_sharpen_lumawgt6.isp_sharpen_lumawgt31 */
/*  input         unsigned int uisp_sharpen_lumawgt31  6 bits */
static __inline td_void isp_sharpen_lumawgt31_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lumawgt31)
{
    u_isp_sharpen_lumawgt6 o_isp_sharpen_lumawgt6;
    o_isp_sharpen_lumawgt6.u32 = be_reg->isp_sharpen_lumawgt6.u32;
    o_isp_sharpen_lumawgt6.bits.isp_sharpen_lumawgt31 = uisp_sharpen_lumawgt31;
    be_reg->isp_sharpen_lumawgt6.u32 = o_isp_sharpen_lumawgt6.u32;
}

/*  description : set the value of the member isp_sharpen_luma_thd.isp_sharpen_exluma_thd */
/*  input       : unsigned int uisp_sharpen_exluma_thd: 9 bits */
static __inline td_void isp_sharpen_exluma_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_exluma_thd)
{
    u_isp_sharpen_luma_thd o_isp_sharpen_luma_thd;
    o_isp_sharpen_luma_thd.u32 = be_reg->isp_sharpen_luma_thd.u32;
    o_isp_sharpen_luma_thd.bits.isp_sharpen_exluma_thd = uisp_sharpen_exluma_thd;
    be_reg->isp_sharpen_luma_thd.u32 = o_isp_sharpen_luma_thd.u32;
}

/*  description : set the value of the member isp_sharpen_luma_thd.isp_sharpen_exluma_out_thd */
/*  input       : unsigned int uisp_sharpen_exluma_out_thd: 9 bits */
static __inline td_void isp_sharpen_exluma_out_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_exluma_out_thd)
{
    u_isp_sharpen_luma_thd o_isp_sharpen_luma_thd;
    o_isp_sharpen_luma_thd.u32 = be_reg->isp_sharpen_luma_thd.u32;
    o_isp_sharpen_luma_thd.bits.isp_sharpen_exluma_out_thd = uisp_sharpen_exluma_out_thd;
    be_reg->isp_sharpen_luma_thd.u32 = o_isp_sharpen_luma_thd.u32;
}

/*  description   set the value of the member isp_sharpen_shoot_amt.isp_sharpen_selpixwgt */
/*  input         unsigned int uisp_sharpen_selpixwgt  5 bits */
static __inline td_void isp_sharpen_selpixwgt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_selpixwgt)
{
    u_isp_sharpen_shoot_amt o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.u32 = be_reg->isp_sharpen_shoot_amt.u32;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_selpixwgt = uisp_sharpen_selpixwgt;
    be_reg->isp_sharpen_shoot_amt.u32 = o_isp_sharpen_shoot_amt.u32;
}
/*  description   set the value of the member isp_sharpen_shoot_amt.isp_sharpen_oshtamt */
/*  input         unsigned int uisp_sharpen_oshtamt  7 bits */
static __inline td_void isp_sharpen_oshtamt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_oshtamt)
{
    u_isp_sharpen_shoot_amt o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.u32 = be_reg->isp_sharpen_shoot_amt.u32;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_oshtamt = uisp_sharpen_oshtamt;
    be_reg->isp_sharpen_shoot_amt.u32 = o_isp_sharpen_shoot_amt.u32;
}
/*  description   set the value of the member isp_sharpen_shoot_amt.isp_sharpen_ushtamt */
/*  input         unsigned int uisp_sharpen_ushtamt  7 bits */
static __inline td_void isp_sharpen_ushtamt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_ushtamt)
{
    u_isp_sharpen_shoot_amt o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.u32 = be_reg->isp_sharpen_shoot_amt.u32;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_ushtamt = uisp_sharpen_ushtamt;
    be_reg->isp_sharpen_shoot_amt.u32 = o_isp_sharpen_shoot_amt.u32;
}
/*  description   set the value of the member isp_sharpen_shoot_maxchg.isp_sharpen_omaxchg */
/*  input         unsigned int uisp_sharpen_omaxchg  10 bits */
static __inline td_void isp_sharpen_omaxchg_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_omaxchg)
{
    u_isp_sharpen_shoot_maxchg o_isp_sharpen_shoot_maxchg;
    o_isp_sharpen_shoot_maxchg.u32 = be_reg->isp_sharpen_shoot_maxchg.u32;
    o_isp_sharpen_shoot_maxchg.bits.isp_sharpen_omaxchg = uisp_sharpen_omaxchg;
    be_reg->isp_sharpen_shoot_maxchg.u32 = o_isp_sharpen_shoot_maxchg.u32;
}
/*  description   set the value of the member isp_sharpen_shoot_maxchg.isp_sharpen_umaxchg */
/*  input         unsigned int uisp_sharpen_umaxchg  10 bits */
static __inline td_void isp_sharpen_umaxchg_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_umaxchg)
{
    u_isp_sharpen_shoot_maxchg o_isp_sharpen_shoot_maxchg;
    o_isp_sharpen_shoot_maxchg.u32 = be_reg->isp_sharpen_shoot_maxchg.u32;
    o_isp_sharpen_shoot_maxchg.bits.isp_sharpen_umaxchg = uisp_sharpen_umaxchg;
    be_reg->isp_sharpen_shoot_maxchg.u32 = o_isp_sharpen_shoot_maxchg.u32;
}
/*  description   set the value of the member isp_sharpen_shtvar.isp_sharpen_shtvarsft */
/*  input         unsigned int uisp_sharpen_shtvarsft  3 bits */
static __inline td_void isp_sharpen_shtvarsft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtvarsft)
{
    u_isp_sharpen_shtvar o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = be_reg->isp_sharpen_shtvar.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtvarsft = uisp_sharpen_shtvarsft;
    be_reg->isp_sharpen_shtvar.u32 = o_isp_sharpen_shtvar.u32;
}
/*  description   set the value of the member isp_sharpen_shtvar.isp_sharpen_shtvar5x5_sft */
/*  input         unsigned int uisp_sharpen_shtvar5x5_sft  4 bits */
static __inline td_void isp_sharpen_shtvar5x5_sft_write(isp_be_reg_type *be_reg, td_u32 sharpen_shtvar5x5_sft)
{
    u_isp_sharpen_shtvar o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = be_reg->isp_sharpen_shtvar.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtvar5x5_sft = sharpen_shtvar5x5_sft;
    be_reg->isp_sharpen_shtvar.u32 = o_isp_sharpen_shtvar.u32;
}
/*  description   set the value of the member isp_sharpen_shtvar.isp_sharpen_shtbldrt */
/*  input         unsigned int uisp_sharpen_shtbldrt  4 bits */
static __inline td_void isp_sharpen_shtbldrt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtbldrt)
{
    u_isp_sharpen_shtvar o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = be_reg->isp_sharpen_shtvar.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtbldrt = uisp_sharpen_shtbldrt;
    be_reg->isp_sharpen_shtvar.u32 = o_isp_sharpen_shtvar.u32;
}
/*  description : set the value of the member isp_sharpen_shtvar.isp_sharpen_dirvar_thd */
/*  input       : unsigned int uisp_sharpen_dirvar_thd: 10 bits */
static __inline td_void isp_sharpen_dirvar_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dirvar_thd)
{
    u_isp_sharpen_shtvar o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.u32 = be_reg->isp_sharpen_shtvar.u32;
    o_isp_sharpen_shtvar.bits.isp_sharpen_dirvar_thd = uisp_sharpen_dirvar_thd;
    be_reg->isp_sharpen_shtvar.u32 = o_isp_sharpen_shtvar.u32;
}

/*  description : set the value of the member isp_sharpen_hard_luma_thd.isp_sharpen_hard_luma_thd */
/*  input       : unsigned int uisp_sharpen_hard_luma_thd: 9 bits */
static __inline td_void isp_sharpen_hard_luma_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hard_luma_thd)
{
    u_isp_sharpen_hard_luma_thd o_isp_sharpen_hard_luma_thd;
    o_isp_sharpen_hard_luma_thd.u32 = be_reg->isp_sharpen_hard_luma_thd.u32;
    o_isp_sharpen_hard_luma_thd.bits.isp_sharpen_hard_luma_thd = uisp_sharpen_hard_luma_thd;
    be_reg->isp_sharpen_hard_luma_thd.u32 = o_isp_sharpen_hard_luma_thd.u32;
}

/*  description   set the value of the member isp_sharpen_oshtvar.isp_sharpen_shtvarthd0 */
/*  input         unsigned int uisp_sharpen_shtvarthd0  8 bits */
static __inline td_void isp_sharpen_shtvarthd0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtvarthd0)
{
    u_isp_sharpen_oshtvar o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = be_reg->isp_sharpen_oshtvar.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarthd0 = uisp_sharpen_shtvarthd0;
    be_reg->isp_sharpen_oshtvar.u32 = o_isp_sharpen_oshtvar.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvar.isp_sharpen_shtvarthd1 */
/*  input         unsigned int uisp_sharpen_shtvarthd1  8 bits */
static __inline td_void isp_sharpen_shtvarthd1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtvarthd1)
{
    u_isp_sharpen_oshtvar o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = be_reg->isp_sharpen_oshtvar.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarthd1 = uisp_sharpen_shtvarthd1;
    be_reg->isp_sharpen_oshtvar.u32 = o_isp_sharpen_oshtvar.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvar.isp_sharpen_shtvarwgt0 */
/*  input         unsigned int uisp_sharpen_shtvarwgt0  7 bits */
static __inline td_void isp_sharpen_shtvarwgt0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtvarwgt0)
{
    u_isp_sharpen_oshtvar o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = be_reg->isp_sharpen_oshtvar.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarwgt0 = uisp_sharpen_shtvarwgt0;
    be_reg->isp_sharpen_oshtvar.u32 = o_isp_sharpen_oshtvar.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvar.isp_sharpen_shtvarwgt1 */
/*  input         unsigned int uisp_sharpen_shtvarwgt1  7 bits */
static __inline td_void isp_sharpen_shtvarwgt1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtvarwgt1)
{
    u_isp_sharpen_oshtvar o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.u32 = be_reg->isp_sharpen_oshtvar.u32;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarwgt1 = uisp_sharpen_shtvarwgt1;
    be_reg->isp_sharpen_oshtvar.u32 = o_isp_sharpen_oshtvar.u32;
}

/*  description : set the value of the member isp_sharpen_exluma_mul.isp_sharpen_exluma_mul */
/*  input       : unsigned int uisp_sharpen_exluma_mul: 8 bits */
static __inline td_void isp_sharpen_exluma_mul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_exluma_mul)
{
    u_isp_sharpen_exluma_mul o_isp_sharpen_exluma_mul;
    o_isp_sharpen_exluma_mul.u32 = be_reg->isp_sharpen_exluma_mul.u32;
    o_isp_sharpen_exluma_mul.bits.isp_sharpen_exluma_mul = uisp_sharpen_exluma_mul;
    be_reg->isp_sharpen_exluma_mul.u32 = o_isp_sharpen_exluma_mul.u32;
}

/*  description   set the value of the member isp_sharpen_shtvar_mul.isp_sharpen_shtvarmul */
/*  input         unsigned int uisp_sharpen_shtvarmul  12 bits */
static __inline td_void isp_sharpen_shtvarmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_shtvarmul)
{
    u_isp_sharpen_shtvar_mul o_isp_sharpen_shtvar_mul;
    o_isp_sharpen_shtvar_mul.u32 = be_reg->isp_sharpen_shtvar_mul.u32;
    o_isp_sharpen_shtvar_mul.bits.isp_sharpen_shtvarmul = uisp_sharpen_shtvarmul;
    be_reg->isp_sharpen_shtvar_mul.u32 = o_isp_sharpen_shtvar_mul.u32;
}
/*  description   set the value of the member isp_sharpen_shtvar_mul.isp_sharpen_shtvardiffmul */
/*  input         unsigned int uisp_sharpen_shtvardiffmul  12 bits */
static __inline td_void isp_sharpen_shtvardiffmul_write(isp_be_reg_type *be_reg, td_u32 sharpen_shtvardiffmul)
{
    u_isp_sharpen_shtvar_mul o_isp_sharpen_shtvar_mul;
    o_isp_sharpen_shtvar_mul.u32 = be_reg->isp_sharpen_shtvar_mul.u32;
    o_isp_sharpen_shtvar_mul.bits.isp_sharpen_shtvardiffmul = sharpen_shtvardiffmul;
    be_reg->isp_sharpen_shtvar_mul.u32 = o_isp_sharpen_shtvar_mul.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvardiff.isp_sharpen_shtvardiffthd0 */
/*  input         unsigned int uisp_sharpen_shtvardiffthd0  8 bits */
static __inline td_void isp_sharpen_shtvardiffthd0_write(isp_be_reg_type *be_reg, td_u32 sharpen_shtvardiffthd0)
{
    u_isp_sharpen_oshtvardiff o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = be_reg->isp_sharpen_oshtvardiff.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffthd0 = sharpen_shtvardiffthd0;
    be_reg->isp_sharpen_oshtvardiff.u32 = o_isp_sharpen_oshtvardiff.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvardiff.isp_sharpen_shtvardiffthd1 */
/*  input         unsigned int uisp_sharpen_shtvardiffthd1  8 bits */
static __inline td_void isp_sharpen_shtvardiffthd1_write(isp_be_reg_type *be_reg, td_u32 sharpen_shtvardiffthd1)
{
    u_isp_sharpen_oshtvardiff o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = be_reg->isp_sharpen_oshtvardiff.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffthd1 = sharpen_shtvardiffthd1;
    be_reg->isp_sharpen_oshtvardiff.u32 = o_isp_sharpen_oshtvardiff.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvardiff.isp_sharpen_shtvardiffwgt0 */
/*  input         unsigned int uisp_sharpen_shtvardiffwgt0  7 bits */
static __inline td_void isp_sharpen_shtvardiffwgt0_write(isp_be_reg_type *be_reg, td_u32 sharpen_shtvardiffwgt0)
{
    u_isp_sharpen_oshtvardiff o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = be_reg->isp_sharpen_oshtvardiff.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffwgt0 = sharpen_shtvardiffwgt0;
    be_reg->isp_sharpen_oshtvardiff.u32 = o_isp_sharpen_oshtvardiff.u32;
}
/*  description   set the value of the member isp_sharpen_oshtvardiff.isp_sharpen_shtvardiffwgt1 */
/*  input         unsigned int uisp_sharpen_shtvardiffwgt1  7 bits */
static __inline td_void isp_sharpen_shtvardiffwgt1_write(isp_be_reg_type *be_reg, td_u32 sharpen_shtvardiffwgt1)
{
    u_isp_sharpen_oshtvardiff o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.u32 = be_reg->isp_sharpen_oshtvardiff.u32;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffwgt1 = sharpen_shtvardiffwgt1;
    be_reg->isp_sharpen_oshtvardiff.u32 = o_isp_sharpen_oshtvardiff.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf0.isp_sharpen_lmtmf0 */
/*  input         unsigned int uisp_sharpen_lmtmf0  6 bits */
static __inline td_void isp_sharpen_lmtmf0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf0)
{
    u_isp_sharpen_lmtmf0 o_isp_sharpen_lmtmf0;
    o_isp_sharpen_lmtmf0.u32 = be_reg->isp_sharpen_lmtmf0.u32;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf0 = uisp_sharpen_lmtmf0;
    be_reg->isp_sharpen_lmtmf0.u32 = o_isp_sharpen_lmtmf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf0.isp_sharpen_lmtmf1 */
/*  input         unsigned int uisp_sharpen_lmtmf1  6 bits */
static __inline td_void isp_sharpen_lmtmf1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf1)
{
    u_isp_sharpen_lmtmf0 o_isp_sharpen_lmtmf0;
    o_isp_sharpen_lmtmf0.u32 = be_reg->isp_sharpen_lmtmf0.u32;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf1 = uisp_sharpen_lmtmf1;
    be_reg->isp_sharpen_lmtmf0.u32 = o_isp_sharpen_lmtmf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf0.isp_sharpen_lmtmf2 */
/*  input         unsigned int uisp_sharpen_lmtmf2  6 bits */
static __inline td_void isp_sharpen_lmtmf2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf2)
{
    u_isp_sharpen_lmtmf0 o_isp_sharpen_lmtmf0;
    o_isp_sharpen_lmtmf0.u32 = be_reg->isp_sharpen_lmtmf0.u32;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf2 = uisp_sharpen_lmtmf2;
    be_reg->isp_sharpen_lmtmf0.u32 = o_isp_sharpen_lmtmf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf0.isp_sharpen_lmtmf3 */
/*  input         unsigned int uisp_sharpen_lmtmf3  6 bits */
static __inline td_void isp_sharpen_lmtmf3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf3)
{
    u_isp_sharpen_lmtmf0 o_isp_sharpen_lmtmf0;
    o_isp_sharpen_lmtmf0.u32 = be_reg->isp_sharpen_lmtmf0.u32;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf3 = uisp_sharpen_lmtmf3;
    be_reg->isp_sharpen_lmtmf0.u32 = o_isp_sharpen_lmtmf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf1.isp_sharpen_lmtmf4 */
/*  input         unsigned int uisp_sharpen_lmtmf4  6 bits */
static __inline td_void isp_sharpen_lmtmf4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf4)
{
    u_isp_sharpen_lmtmf1 o_isp_sharpen_lmtmf1;
    o_isp_sharpen_lmtmf1.u32 = be_reg->isp_sharpen_lmtmf1.u32;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf4 = uisp_sharpen_lmtmf4;
    be_reg->isp_sharpen_lmtmf1.u32 = o_isp_sharpen_lmtmf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf1.isp_sharpen_lmtmf5 */
/*  input         unsigned int uisp_sharpen_lmtmf5  6 bits */
static __inline td_void isp_sharpen_lmtmf5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf5)
{
    u_isp_sharpen_lmtmf1 o_isp_sharpen_lmtmf1;
    o_isp_sharpen_lmtmf1.u32 = be_reg->isp_sharpen_lmtmf1.u32;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf5 = uisp_sharpen_lmtmf5;
    be_reg->isp_sharpen_lmtmf1.u32 = o_isp_sharpen_lmtmf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf1.isp_sharpen_lmtmf6 */
/*  input         unsigned int uisp_sharpen_lmtmf6  6 bits */
static __inline td_void isp_sharpen_lmtmf6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf6)
{
    u_isp_sharpen_lmtmf1 o_isp_sharpen_lmtmf1;
    o_isp_sharpen_lmtmf1.u32 = be_reg->isp_sharpen_lmtmf1.u32;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf6 = uisp_sharpen_lmtmf6;
    be_reg->isp_sharpen_lmtmf1.u32 = o_isp_sharpen_lmtmf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmtmf1.isp_sharpen_lmtmf7 */
/*  input         unsigned int uisp_sharpen_lmtmf7  6 bits */
static __inline td_void isp_sharpen_lmtmf7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmtmf7)
{
    u_isp_sharpen_lmtmf1 o_isp_sharpen_lmtmf1;
    o_isp_sharpen_lmtmf1.u32 = be_reg->isp_sharpen_lmtmf1.u32;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf7 = uisp_sharpen_lmtmf7;
    be_reg->isp_sharpen_lmtmf1.u32 = o_isp_sharpen_lmtmf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf0.isp_sharpen_lmthf0 */
/*  input         unsigned int uisp_sharpen_lmthf0  6 bits */
static __inline td_void isp_sharpen_lmthf0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf0)
{
    u_isp_sharpen_lmthf0 o_isp_sharpen_lmthf0;
    o_isp_sharpen_lmthf0.u32 = be_reg->isp_sharpen_lmthf0.u32;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf0 = uisp_sharpen_lmthf0;
    be_reg->isp_sharpen_lmthf0.u32 = o_isp_sharpen_lmthf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf0.isp_sharpen_lmthf1 */
/*  input         unsigned int uisp_sharpen_lmthf1  6 bits */
static __inline td_void isp_sharpen_lmthf1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf1)
{
    u_isp_sharpen_lmthf0 o_isp_sharpen_lmthf0;
    o_isp_sharpen_lmthf0.u32 = be_reg->isp_sharpen_lmthf0.u32;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf1 = uisp_sharpen_lmthf1;
    be_reg->isp_sharpen_lmthf0.u32 = o_isp_sharpen_lmthf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf0.isp_sharpen_lmthf2 */
/*  input         unsigned int uisp_sharpen_lmthf2  6 bits */
static __inline td_void isp_sharpen_lmthf2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf2)
{
    u_isp_sharpen_lmthf0 o_isp_sharpen_lmthf0;
    o_isp_sharpen_lmthf0.u32 = be_reg->isp_sharpen_lmthf0.u32;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf2 = uisp_sharpen_lmthf2;
    be_reg->isp_sharpen_lmthf0.u32 = o_isp_sharpen_lmthf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf0.isp_sharpen_lmthf3 */
/*  input         unsigned int uisp_sharpen_lmthf3  6 bits */
static __inline td_void isp_sharpen_lmthf3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf3)
{
    u_isp_sharpen_lmthf0 o_isp_sharpen_lmthf0;
    o_isp_sharpen_lmthf0.u32 = be_reg->isp_sharpen_lmthf0.u32;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf3 = uisp_sharpen_lmthf3;
    be_reg->isp_sharpen_lmthf0.u32 = o_isp_sharpen_lmthf0.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf1.isp_sharpen_lmthf4 */
/*  input         unsigned int uisp_sharpen_lmthf4  6 bits */
static __inline td_void isp_sharpen_lmthf4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf4)
{
    u_isp_sharpen_lmthf1 o_isp_sharpen_lmthf1;
    o_isp_sharpen_lmthf1.u32 = be_reg->isp_sharpen_lmthf1.u32;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf4 = uisp_sharpen_lmthf4;
    be_reg->isp_sharpen_lmthf1.u32 = o_isp_sharpen_lmthf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf1.isp_sharpen_lmthf5 */
/*  input         unsigned int uisp_sharpen_lmthf5  6 bits */
static __inline td_void isp_sharpen_lmthf5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf5)
{
    u_isp_sharpen_lmthf1 o_isp_sharpen_lmthf1;
    o_isp_sharpen_lmthf1.u32 = be_reg->isp_sharpen_lmthf1.u32;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf5 = uisp_sharpen_lmthf5;
    be_reg->isp_sharpen_lmthf1.u32 = o_isp_sharpen_lmthf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf1.isp_sharpen_lmthf6 */
/*  input         unsigned int uisp_sharpen_lmthf6  6 bits */
static __inline td_void isp_sharpen_lmthf6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf6)
{
    u_isp_sharpen_lmthf1 o_isp_sharpen_lmthf1;
    o_isp_sharpen_lmthf1.u32 = be_reg->isp_sharpen_lmthf1.u32;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf6 = uisp_sharpen_lmthf6;
    be_reg->isp_sharpen_lmthf1.u32 = o_isp_sharpen_lmthf1.u32;
}
/*  description   set the value of the member isp_sharpen_lmthf1.isp_sharpen_lmthf7 */
/*  input         unsigned int uisp_sharpen_lmthf7  6 bits */
static __inline td_void isp_sharpen_lmthf7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmthf7)
{
    u_isp_sharpen_lmthf1 o_isp_sharpen_lmthf1;
    o_isp_sharpen_lmthf1.u32 = be_reg->isp_sharpen_lmthf1.u32;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf7 = uisp_sharpen_lmthf7;
    be_reg->isp_sharpen_lmthf1.u32 = o_isp_sharpen_lmthf1.u32;
}
/*  description   set the value of the member isp_sharpen_mhfgaind_waddr.isp_sharpen_mhfgaind_waddr */
/*  input         unsigned int uisp_sharpen_mhfgaind_waddr  32 bits */
static __inline td_void isp_sharpen_mhfgaind_waddr_write(isp_be_reg_type *be_reg,
    td_u32 uisp_sharpen_mhfgaind_waddr)
{
    be_reg->isp_sharpen_mhfgaind_waddr.u32 = uisp_sharpen_mhfgaind_waddr;
}
/*  description   set the value of the member isp_sharpen_mhfgaind_wdata.isp_sharpen_mhfgaind_wdata */
/*  input         unsigned int uisp_sharpen_mhfgaind_wdata  32 bits */
static __inline td_void isp_sharpen_mhfgaind_wdata_write(isp_be_reg_type *be_reg,
    td_u32 uisp_sharpen_mhfgaind_wdata)
{
    be_reg->isp_sharpen_mhfgaind_wdata.u32 = uisp_sharpen_mhfgaind_wdata;
}
/*  description   set the value of the member isp_sharpen_mhfgaind_raddr.isp_sharpen_mhfgaind_raddr */
/*  input         unsigned int uisp_sharpen_mhfgaind_raddr  32 bits */
static __inline td_void isp_sharpen_mhfgaind_raddr_write(isp_be_reg_type *be_reg,
    td_u32 uisp_sharpen_mhfgaind_raddr)
{
    be_reg->isp_sharpen_mhfgaind_raddr.u32 = uisp_sharpen_mhfgaind_raddr;
}
/*  description   set the value of the member isp_sharpen_mhfgainud_waddr.isp_sharpen_mhfgainud_waddr */
/*  input         unsigned int uisp_sharpen_mhfgainud_waddr  32 bits */
static __inline td_void isp_sharpen_mhfgainud_waddr_write(isp_be_reg_type *be_reg,
    td_u32 uisp_sharpen_mhfgainud_waddr)
{
    be_reg->isp_sharpen_mhfgainud_waddr.u32 = uisp_sharpen_mhfgainud_waddr;
}
/*  description   set the value of the member isp_sharpen_mhfgainud_wdata.isp_sharpen_mhfgainud_wdata */
/*  input         unsigned int uisp_sharpen_mhfgainud_wdata  32 bits */
static __inline td_void isp_sharpen_mhfgainud_wdata_write(isp_be_reg_type *be_reg,
    td_u32 uisp_sharpen_mhfgainud_wdata)
{
    be_reg->isp_sharpen_mhfgainud_wdata.u32 = uisp_sharpen_mhfgainud_wdata;
}
/*  description   set the value of the member isp_sharpen_mhfgainud_raddr.isp_sharpen_mhfgainud_raddr */
/*  input         unsigned int uisp_sharpen_mhfgainud_raddr  32 bits */
static __inline td_void isp_sharpen_mhfgainud_raddr_write(isp_be_reg_type *be_reg,
    td_u32 uisp_sharpen_mhfgainud_raddr)
{
    be_reg->isp_sharpen_mhfgainud_raddr.u32 = uisp_sharpen_mhfgainud_raddr;
}


/*  description : set the value of the member isp_sharpen_skin_u.isp_sharpen_skinminu */
/*  input       : unsigned int uisp_sharpen_skinminu: 8 bits */
static __inline td_void isp_sharpen_skinminu_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinminu)
{
    u_isp_sharpen_skin_u o_isp_sharpen_skin_u;
    o_isp_sharpen_skin_u.u32 = be_reg->isp_sharpen_skin_u.u32;
    o_isp_sharpen_skin_u.bits.isp_sharpen_skinminu = uisp_sharpen_skinminu;
    be_reg->isp_sharpen_skin_u.u32 = o_isp_sharpen_skin_u.u32;
}

/*  description : set the value of the member isp_sharpen_skin_u.isp_sharpen_skinmaxu */
/*  input       : unsigned int uisp_sharpen_skinmaxu: 8 bits */
static __inline td_void isp_sharpen_skinmaxu_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinmaxu)
{
    u_isp_sharpen_skin_u o_isp_sharpen_skin_u;
    o_isp_sharpen_skin_u.u32 = be_reg->isp_sharpen_skin_u.u32;
    o_isp_sharpen_skin_u.bits.isp_sharpen_skinmaxu = uisp_sharpen_skinmaxu;
    be_reg->isp_sharpen_skin_u.u32 = o_isp_sharpen_skin_u.u32;
}

/*  description : set the value of the member isp_sharpen_skin_v.isp_sharpen_skinminv */
/*  input       : unsigned int uisp_sharpen_skinminv: 8 bits */
static __inline td_void isp_sharpen_skinminv_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinminv)
{
    u_isp_sharpen_skin_v o_isp_sharpen_skin_v;
    o_isp_sharpen_skin_v.u32 = be_reg->isp_sharpen_skin_v.u32;
    o_isp_sharpen_skin_v.bits.isp_sharpen_skinminv = uisp_sharpen_skinminv;
    be_reg->isp_sharpen_skin_v.u32 = o_isp_sharpen_skin_v.u32;
}

/*  description : set the value of the member isp_sharpen_skin_v.isp_sharpen_skinmaxv */
/*  input       : unsigned int uisp_sharpen_skinmaxv: 8 bits */
static __inline td_void isp_sharpen_skinmaxv_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinmaxv)
{
    u_isp_sharpen_skin_v o_isp_sharpen_skin_v;
    o_isp_sharpen_skin_v.u32 = be_reg->isp_sharpen_skin_v.u32;
    o_isp_sharpen_skin_v.bits.isp_sharpen_skinmaxv = uisp_sharpen_skinmaxv;
    be_reg->isp_sharpen_skin_v.u32 = o_isp_sharpen_skin_v.u32;
}

/*  description : set the value of the member isp_sharpen_skin_cnt.isp_sharpen_skincntthd0 */
/*  input       : unsigned int uisp_sharpen_skincntthd0: 4 bits */
static __inline td_void isp_sharpen_skincntthd0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skincntthd0)
{
    u_isp_sharpen_skin_cnt o_isp_sharpen_skin_cnt;
    o_isp_sharpen_skin_cnt.u32 = be_reg->isp_sharpen_skin_cnt.u32;
    o_isp_sharpen_skin_cnt.bits.isp_sharpen_skincntthd0 = uisp_sharpen_skincntthd0;
    be_reg->isp_sharpen_skin_cnt.u32 = o_isp_sharpen_skin_cnt.u32;
}

/*  description : set the value of the member isp_sharpen_skin_cnt.isp_sharpen_skincntthd1 */
/*  input       : unsigned int uisp_sharpen_skincntthd1: 4 bits */
static __inline td_void isp_sharpen_skincntthd1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skincntthd1)
{
    u_isp_sharpen_skin_cnt o_isp_sharpen_skin_cnt;
    o_isp_sharpen_skin_cnt.u32 = be_reg->isp_sharpen_skin_cnt.u32;
    o_isp_sharpen_skin_cnt.bits.isp_sharpen_skincntthd1 = uisp_sharpen_skincntthd1;
    be_reg->isp_sharpen_skin_cnt.u32 = o_isp_sharpen_skin_cnt.u32;
}

/*  description : set the value of the member isp_sharpen_skin_cnt.isp_sharpen_skincntmul */
/*  input       : unsigned int uisp_sharpen_skincntmul: 5 bits */
static __inline td_void isp_sharpen_skincntmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skincntmul)
{
    u_isp_sharpen_skin_cnt o_isp_sharpen_skin_cnt;
    o_isp_sharpen_skin_cnt.u32 = be_reg->isp_sharpen_skin_cnt.u32;
    o_isp_sharpen_skin_cnt.bits.isp_sharpen_skincntmul = uisp_sharpen_skincntmul;
    be_reg->isp_sharpen_skin_cnt.u32 = o_isp_sharpen_skin_cnt.u32;
}

/*  description : set the value of the member isp_sharpen_skin_edge.isp_sharpen_skinedgesft */
/*  input       : unsigned int uisp_sharpen_skinedgesft: 4 bits */
static __inline td_void isp_sharpen_skinedgesft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinedgesft)
{
    u_isp_sharpen_skin_edge o_isp_sharpen_skin_edge;
    o_isp_sharpen_skin_edge.u32 = be_reg->isp_sharpen_skin_edge.u32;
    o_isp_sharpen_skin_edge.bits.isp_sharpen_skinedgesft = uisp_sharpen_skinedgesft;
    be_reg->isp_sharpen_skin_edge.u32 = o_isp_sharpen_skin_edge.u32;
}

/*  description : set the value of the member isp_sharpen_skin_edge.isp_sharpen_skinedgemul */
/*  input       : unsigned int uisp_sharpen_skinedgemul: 10 bits */
static __inline td_void isp_sharpen_skinedgemul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinedgemul)
{
    u_isp_sharpen_skin_edge o_isp_sharpen_skin_edge;
    o_isp_sharpen_skin_edge.u32 = be_reg->isp_sharpen_skin_edge.u32;
    o_isp_sharpen_skin_edge.bits.isp_sharpen_skinedgemul = uisp_sharpen_skinedgemul;
    be_reg->isp_sharpen_skin_edge.u32 = o_isp_sharpen_skin_edge.u32;
}

/*  description : set the value of the member isp_sharpen_skin_edgethd.isp_sharpen_skinedgethd0 */
/*  input       : unsigned int uisp_sharpen_skinedgethd0: 8 bits */
static __inline td_void isp_sharpen_skinedgethd0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinedgethd0)
{
    u_isp_sharpen_skin_edgethd o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = be_reg->isp_sharpen_skin_edgethd.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgethd0 = uisp_sharpen_skinedgethd0;
    be_reg->isp_sharpen_skin_edgethd.u32 = o_isp_sharpen_skin_edgethd.u32;
}

/*  description : set the value of the member isp_sharpen_skin_edgethd.isp_sharpen_skinedgethd1 */
/*  input       : unsigned int uisp_sharpen_skinedgethd1: 8 bits */
static __inline td_void isp_sharpen_skinedgethd1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinedgethd1)
{
    u_isp_sharpen_skin_edgethd o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = be_reg->isp_sharpen_skin_edgethd.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgethd1 = uisp_sharpen_skinedgethd1;
    be_reg->isp_sharpen_skin_edgethd.u32 = o_isp_sharpen_skin_edgethd.u32;
}

/*  description : set the value of the member isp_sharpen_skin_edgethd.isp_sharpen_skinedgewgt0 */
/*  input       : unsigned int uisp_sharpen_skinedgewgt0: 5 bits */
static __inline td_void isp_sharpen_skinedgewgt0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinedgewgt0)
{
    u_isp_sharpen_skin_edgethd o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = be_reg->isp_sharpen_skin_edgethd.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgewgt0 = uisp_sharpen_skinedgewgt0;
    be_reg->isp_sharpen_skin_edgethd.u32 = o_isp_sharpen_skin_edgethd.u32;
}

/*  description : set the value of the member isp_sharpen_skin_edgethd.isp_sharpen_skinedgewgt1 */
/*  input       : unsigned int uisp_sharpen_skinedgewgt1: 5 bits */
static __inline td_void isp_sharpen_skinedgewgt1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_skinedgewgt1)
{
    u_isp_sharpen_skin_edgethd o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.u32 = be_reg->isp_sharpen_skin_edgethd.u32;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgewgt1 = uisp_sharpen_skinedgewgt1;
    be_reg->isp_sharpen_skin_edgethd.u32 = o_isp_sharpen_skin_edgethd.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_var.isp_sharpen_chrrsft0 */
/*  input       : unsigned int uisp_sharpen_chrrsft0: 3 bits */
static __inline td_void isp_sharpen_chrrsft0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrsft0)
{
    u_isp_sharpen_chrr_var o_isp_sharpen_chrr_var;
    o_isp_sharpen_chrr_var.u32 = be_reg->isp_sharpen_chrr_var.u32;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft0 = uisp_sharpen_chrrsft0;
    be_reg->isp_sharpen_chrr_var.u32 = o_isp_sharpen_chrr_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_var.isp_sharpen_chrrsft1 */
/*  input       : unsigned int uisp_sharpen_chrrsft1: 3 bits */
static __inline td_void isp_sharpen_chrrsft1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrsft1)
{
    u_isp_sharpen_chrr_var o_isp_sharpen_chrr_var;
    o_isp_sharpen_chrr_var.u32 = be_reg->isp_sharpen_chrr_var.u32;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft1 = uisp_sharpen_chrrsft1;
    be_reg->isp_sharpen_chrr_var.u32 = o_isp_sharpen_chrr_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_var.isp_sharpen_chrrsft2 */
/*  input       : unsigned int uisp_sharpen_chrrsft2: 3 bits */
static __inline td_void isp_sharpen_chrrsft2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrsft2)
{
    u_isp_sharpen_chrr_var o_isp_sharpen_chrr_var;
    o_isp_sharpen_chrr_var.u32 = be_reg->isp_sharpen_chrr_var.u32;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft2 = uisp_sharpen_chrrsft2;
    be_reg->isp_sharpen_chrr_var.u32 = o_isp_sharpen_chrr_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_var.isp_sharpen_chrrsft3 */
/*  input       : unsigned int uisp_sharpen_chrrsft3: 3 bits */
static __inline td_void isp_sharpen_chrrsft3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrsft3)
{
    u_isp_sharpen_chrr_var o_isp_sharpen_chrr_var;
    o_isp_sharpen_chrr_var.u32 = be_reg->isp_sharpen_chrr_var.u32;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft3 = uisp_sharpen_chrrsft3;
    be_reg->isp_sharpen_chrr_var.u32 = o_isp_sharpen_chrr_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_var.isp_sharpen_chrrvarshift */
/*  input       : unsigned int uisp_sharpen_chrrvarshift: 4 bits */
static __inline td_void isp_sharpen_chrrvarshift_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrvarshift)
{
    u_isp_sharpen_chrr_var o_isp_sharpen_chrr_var;
    o_isp_sharpen_chrr_var.u32 = be_reg->isp_sharpen_chrr_var.u32;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrvarshift = uisp_sharpen_chrrvarshift;
    be_reg->isp_sharpen_chrr_var.u32 = o_isp_sharpen_chrr_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_thd.isp_sharpen_chrrori0 */
/*  input       : unsigned int uisp_sharpen_chrrori0: 8 bits */
static __inline td_void isp_sharpen_chrrori0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrori0)
{
    u_isp_sharpen_chrr_thd o_isp_sharpen_chrr_thd;
    o_isp_sharpen_chrr_thd.u32 = be_reg->isp_sharpen_chrr_thd.u32;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrori0 = uisp_sharpen_chrrori0;
    be_reg->isp_sharpen_chrr_thd.u32 = o_isp_sharpen_chrr_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_thd.isp_sharpen_chrrori1 */
/*  input       : unsigned int uisp_sharpen_chrrori1: 8 bits */
static __inline td_void isp_sharpen_chrrori1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrori1)
{
    u_isp_sharpen_chrr_thd o_isp_sharpen_chrr_thd;
    o_isp_sharpen_chrr_thd.u32 = be_reg->isp_sharpen_chrr_thd.u32;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrori1 = uisp_sharpen_chrrori1;
    be_reg->isp_sharpen_chrr_thd.u32 = o_isp_sharpen_chrr_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_thd.isp_sharpen_chrrthd0 */
/*  input       : unsigned int uisp_sharpen_chrrthd0: 8 bits */
static __inline td_void isp_sharpen_chrrthd0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrthd0)
{
    u_isp_sharpen_chrr_thd o_isp_sharpen_chrr_thd;
    o_isp_sharpen_chrr_thd.u32 = be_reg->isp_sharpen_chrr_thd.u32;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrthd0 = uisp_sharpen_chrrthd0;
    be_reg->isp_sharpen_chrr_thd.u32 = o_isp_sharpen_chrr_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_thd.isp_sharpen_chrrthd1 */
/*  input       : unsigned int uisp_sharpen_chrrthd1: 8 bits */
static __inline td_void isp_sharpen_chrrthd1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrthd1)
{
    u_isp_sharpen_chrr_thd o_isp_sharpen_chrr_thd;
    o_isp_sharpen_chrr_thd.u32 = be_reg->isp_sharpen_chrr_thd.u32;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrthd1 = uisp_sharpen_chrrthd1;
    be_reg->isp_sharpen_chrr_thd.u32 = o_isp_sharpen_chrr_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_gain.isp_sharpen_chrrgain */
/*  input       : unsigned int uisp_sharpen_chrrgain: 6 bits */
static __inline td_void isp_sharpen_chrrgain_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrgain)
{
    u_isp_sharpen_chrr_gain o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.u32 = be_reg->isp_sharpen_chrr_gain.u32;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_chrrgain = uisp_sharpen_chrrgain;
    be_reg->isp_sharpen_chrr_gain.u32 = o_isp_sharpen_chrr_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_gain.isp_sharpen_rmfscale */
/*  input       : unsigned int uisp_sharpen_rmfscale: 6 bits */
static __inline td_void isp_sharpen_rmfscale_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rmfscale)
{
    u_isp_sharpen_chrr_gain o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.u32 = be_reg->isp_sharpen_chrr_gain.u32;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_rmfscale = uisp_sharpen_rmfscale;
    be_reg->isp_sharpen_chrr_gain.u32 = o_isp_sharpen_chrr_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrr_gain.isp_sharpen_chrrmul */
/*  input       : unsigned int uisp_sharpen_chrrmul: 10 bits */
static __inline td_void isp_sharpen_chrrmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrrmul)
{
    u_isp_sharpen_chrr_gain o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.u32 = be_reg->isp_sharpen_chrr_gain.u32;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_chrrmul = uisp_sharpen_chrrmul;
    be_reg->isp_sharpen_chrr_gain.u32 = o_isp_sharpen_chrr_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_mul.isp_sharpen_chrgmul */
/*  input       : unsigned int uisp_sharpen_chrgmul: 14 bits */
static __inline td_void isp_sharpen_chrgmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgmul)
{
    u_isp_sharpen_chrg_mul o_isp_sharpen_chrg_mul;
    o_isp_sharpen_chrg_mul.u32 = be_reg->isp_sharpen_chrg_mul.u32;
    o_isp_sharpen_chrg_mul.bits.isp_sharpen_chrgmul = uisp_sharpen_chrgmul;
    be_reg->isp_sharpen_chrg_mul.u32 = o_isp_sharpen_chrg_mul.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_mul.isp_sharpen_chrgmfmul */
/*  input       : unsigned int uisp_sharpen_chrgmfmul: 14 bits */
static __inline td_void isp_sharpen_chrgmfmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgmfmul)
{
    u_isp_sharpen_chrg_mul o_isp_sharpen_chrg_mul;
    o_isp_sharpen_chrg_mul.u32 = be_reg->isp_sharpen_chrg_mul.u32;
    o_isp_sharpen_chrg_mul.bits.isp_sharpen_chrgmfmul = uisp_sharpen_chrgmfmul;
    be_reg->isp_sharpen_chrg_mul.u32 = o_isp_sharpen_chrg_mul.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_sft.isp_sharpen_chrgsft0 */
/*  input       : unsigned int uisp_sharpen_chrgsft0: 3 bits */
static __inline td_void isp_sharpen_chrgsft0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgsft0)
{
    u_isp_sharpen_chrg_sft o_isp_sharpen_chrg_sft;
    o_isp_sharpen_chrg_sft.u32 = be_reg->isp_sharpen_chrg_sft.u32;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft0 = uisp_sharpen_chrgsft0;
    be_reg->isp_sharpen_chrg_sft.u32 = o_isp_sharpen_chrg_sft.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_sft.isp_sharpen_chrgsft1 */
/*  input       : unsigned int uisp_sharpen_chrgsft1: 3 bits */
static __inline td_void isp_sharpen_chrgsft1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgsft1)
{
    u_isp_sharpen_chrg_sft o_isp_sharpen_chrg_sft;
    o_isp_sharpen_chrg_sft.u32 = be_reg->isp_sharpen_chrg_sft.u32;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft1 = uisp_sharpen_chrgsft1;
    be_reg->isp_sharpen_chrg_sft.u32 = o_isp_sharpen_chrg_sft.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_sft.isp_sharpen_chrgsft2 */
/*  input       : unsigned int uisp_sharpen_chrgsft2: 3 bits */
static __inline td_void isp_sharpen_chrgsft2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgsft2)
{
    u_isp_sharpen_chrg_sft o_isp_sharpen_chrg_sft;
    o_isp_sharpen_chrg_sft.u32 = be_reg->isp_sharpen_chrg_sft.u32;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft2 = uisp_sharpen_chrgsft2;
    be_reg->isp_sharpen_chrg_sft.u32 = o_isp_sharpen_chrg_sft.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_sft.isp_sharpen_chrgsft3 */
/*  input       : unsigned int uisp_sharpen_chrgsft3: 3 bits */
static __inline td_void isp_sharpen_chrgsft3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgsft3)
{
    u_isp_sharpen_chrg_sft o_isp_sharpen_chrg_sft;
    o_isp_sharpen_chrg_sft.u32 = be_reg->isp_sharpen_chrg_sft.u32;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft3 = uisp_sharpen_chrgsft3;
    be_reg->isp_sharpen_chrg_sft.u32 = o_isp_sharpen_chrg_sft.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_thd.isp_sharpen_chrgori0 */
/*  input       : unsigned int uisp_sharpen_chrgori0: 8 bits */
static __inline td_void isp_sharpen_chrgori0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgori0)
{
    u_isp_sharpen_chrg_thd o_isp_sharpen_chrg_thd;
    o_isp_sharpen_chrg_thd.u32 = be_reg->isp_sharpen_chrg_thd.u32;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgori0 = uisp_sharpen_chrgori0;
    be_reg->isp_sharpen_chrg_thd.u32 = o_isp_sharpen_chrg_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_thd.isp_sharpen_chrgori1 */
/*  input       : unsigned int uisp_sharpen_chrgori1: 8 bits */
static __inline td_void isp_sharpen_chrgori1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgori1)
{
    u_isp_sharpen_chrg_thd o_isp_sharpen_chrg_thd;
    o_isp_sharpen_chrg_thd.u32 = be_reg->isp_sharpen_chrg_thd.u32;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgori1 = uisp_sharpen_chrgori1;
    be_reg->isp_sharpen_chrg_thd.u32 = o_isp_sharpen_chrg_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_thd.isp_sharpen_chrgthd0 */
/*  input       : unsigned int uisp_sharpen_chrgthd0: 8 bits */
static __inline td_void isp_sharpen_chrgthd0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgthd0)
{
    u_isp_sharpen_chrg_thd o_isp_sharpen_chrg_thd;
    o_isp_sharpen_chrg_thd.u32 = be_reg->isp_sharpen_chrg_thd.u32;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgthd0 = uisp_sharpen_chrgthd0;
    be_reg->isp_sharpen_chrg_thd.u32 = o_isp_sharpen_chrg_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_thd.isp_sharpen_chrgthd1 */
/*  input       : unsigned int uisp_sharpen_chrgthd1: 8 bits */
static __inline td_void isp_sharpen_chrgthd1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgthd1)
{
    u_isp_sharpen_chrg_thd o_isp_sharpen_chrg_thd;
    o_isp_sharpen_chrg_thd.u32 = be_reg->isp_sharpen_chrg_thd.u32;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgthd1 = uisp_sharpen_chrgthd1;
    be_reg->isp_sharpen_chrg_thd.u32 = o_isp_sharpen_chrg_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_gain.isp_sharpen_chrggain */
/*  input       : unsigned int uisp_sharpen_chrggain: 9 bits */
static __inline td_void isp_sharpen_chrggain_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrggain)
{
    u_isp_sharpen_chrg_gain o_isp_sharpen_chrg_gain;
    o_isp_sharpen_chrg_gain.u32 = be_reg->isp_sharpen_chrg_gain.u32;
    o_isp_sharpen_chrg_gain.bits.isp_sharpen_chrggain = uisp_sharpen_chrggain;
    be_reg->isp_sharpen_chrg_gain.u32 = o_isp_sharpen_chrg_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrg_gain.isp_sharpen_chrgmfgain */
/*  input       : unsigned int uisp_sharpen_chrgmfgain: 9 bits */
static __inline td_void isp_sharpen_chrgmfgain_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrgmfgain)
{
    u_isp_sharpen_chrg_gain o_isp_sharpen_chrg_gain;
    o_isp_sharpen_chrg_gain.u32 = be_reg->isp_sharpen_chrg_gain.u32;
    o_isp_sharpen_chrg_gain.bits.isp_sharpen_chrgmfgain = uisp_sharpen_chrgmfgain;
    be_reg->isp_sharpen_chrg_gain.u32 = o_isp_sharpen_chrg_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_var.isp_sharpen_chrbsft0 */
/*  input       : unsigned int uisp_sharpen_chrbsft0: 3 bits */
static __inline td_void isp_sharpen_chrbsft0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbsft0)
{
    u_isp_sharpen_chrb_var o_isp_sharpen_chrb_var;
    o_isp_sharpen_chrb_var.u32 = be_reg->isp_sharpen_chrb_var.u32;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft0 = uisp_sharpen_chrbsft0;
    be_reg->isp_sharpen_chrb_var.u32 = o_isp_sharpen_chrb_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_var.isp_sharpen_chrbsft1 */
/*  input       : unsigned int uisp_sharpen_chrbsft1: 3 bits */
static __inline td_void isp_sharpen_chrbsft1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbsft1)
{
    u_isp_sharpen_chrb_var o_isp_sharpen_chrb_var;
    o_isp_sharpen_chrb_var.u32 = be_reg->isp_sharpen_chrb_var.u32;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft1 = uisp_sharpen_chrbsft1;
    be_reg->isp_sharpen_chrb_var.u32 = o_isp_sharpen_chrb_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_var.isp_sharpen_chrbsft2 */
/*  input       : unsigned int uisp_sharpen_chrbsft2: 3 bits */
static __inline td_void isp_sharpen_chrbsft2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbsft2)
{
    u_isp_sharpen_chrb_var o_isp_sharpen_chrb_var;
    o_isp_sharpen_chrb_var.u32 = be_reg->isp_sharpen_chrb_var.u32;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft2 = uisp_sharpen_chrbsft2;
    be_reg->isp_sharpen_chrb_var.u32 = o_isp_sharpen_chrb_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_var.isp_sharpen_chrbsft3 */
/*  input       : unsigned int uisp_sharpen_chrbsft3: 3 bits */
static __inline td_void isp_sharpen_chrbsft3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbsft3)
{
    u_isp_sharpen_chrb_var o_isp_sharpen_chrb_var;
    o_isp_sharpen_chrb_var.u32 = be_reg->isp_sharpen_chrb_var.u32;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft3 = uisp_sharpen_chrbsft3;
    be_reg->isp_sharpen_chrb_var.u32 = o_isp_sharpen_chrb_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_var.isp_sharpen_chrbvarshift */
/*  input       : unsigned int uisp_sharpen_chrbvarshift: 4 bits */
static __inline td_void isp_sharpen_chrbvarshift_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbvarshift)
{
    u_isp_sharpen_chrb_var o_isp_sharpen_chrb_var;
    o_isp_sharpen_chrb_var.u32 = be_reg->isp_sharpen_chrb_var.u32;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbvarshift = uisp_sharpen_chrbvarshift;
    be_reg->isp_sharpen_chrb_var.u32 = o_isp_sharpen_chrb_var.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_thd.isp_sharpen_chrbori0 */
/*  input       : unsigned int uisp_sharpen_chrbori0: 8 bits */
static __inline td_void isp_sharpen_chrbori0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbori0)
{
    u_isp_sharpen_chrb_thd o_isp_sharpen_chrb_thd;
    o_isp_sharpen_chrb_thd.u32 = be_reg->isp_sharpen_chrb_thd.u32;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbori0 = uisp_sharpen_chrbori0;
    be_reg->isp_sharpen_chrb_thd.u32 = o_isp_sharpen_chrb_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_thd.isp_sharpen_chrbori1 */
/*  input       : unsigned int uisp_sharpen_chrbori1: 8 bits */
static __inline td_void isp_sharpen_chrbori1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbori1)
{
    u_isp_sharpen_chrb_thd o_isp_sharpen_chrb_thd;
    o_isp_sharpen_chrb_thd.u32 = be_reg->isp_sharpen_chrb_thd.u32;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbori1 = uisp_sharpen_chrbori1;
    be_reg->isp_sharpen_chrb_thd.u32 = o_isp_sharpen_chrb_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_thd.isp_sharpen_chrbthd0 */
/*  input       : unsigned int uisp_sharpen_chrbthd0: 8 bits */
static __inline td_void isp_sharpen_chrbthd0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbthd0)
{
    u_isp_sharpen_chrb_thd o_isp_sharpen_chrb_thd;
    o_isp_sharpen_chrb_thd.u32 = be_reg->isp_sharpen_chrb_thd.u32;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbthd0 = uisp_sharpen_chrbthd0;
    be_reg->isp_sharpen_chrb_thd.u32 = o_isp_sharpen_chrb_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_thd.isp_sharpen_chrbthd1 */
/*  input       : unsigned int uisp_sharpen_chrbthd1: 8 bits */
static __inline td_void isp_sharpen_chrbthd1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbthd1)
{
    u_isp_sharpen_chrb_thd o_isp_sharpen_chrb_thd;
    o_isp_sharpen_chrb_thd.u32 = be_reg->isp_sharpen_chrb_thd.u32;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbthd1 = uisp_sharpen_chrbthd1;
    be_reg->isp_sharpen_chrb_thd.u32 = o_isp_sharpen_chrb_thd.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_gain.isp_sharpen_chrbgain */
/*  input       : unsigned int uisp_sharpen_chrbgain: 6 bits */
static __inline td_void isp_sharpen_chrbgain_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbgain)
{
    u_isp_sharpen_chrb_gain o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.u32 = be_reg->isp_sharpen_chrb_gain.u32;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_chrbgain = uisp_sharpen_chrbgain;
    be_reg->isp_sharpen_chrb_gain.u32 = o_isp_sharpen_chrb_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_gain.isp_sharpen_bmfscale */
/*  input       : unsigned int uisp_sharpen_bmfscale: 6 bits */
static __inline td_void isp_sharpen_bmfscale_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_bmfscale)
{
    u_isp_sharpen_chrb_gain o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.u32 = be_reg->isp_sharpen_chrb_gain.u32;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_bmfscale = uisp_sharpen_bmfscale;
    be_reg->isp_sharpen_chrb_gain.u32 = o_isp_sharpen_chrb_gain.u32;
}

/*  description : set the value of the member isp_sharpen_chrb_gain.isp_sharpen_chrbmul */
/*  input       : unsigned int uisp_sharpen_chrbmul: 10 bits */
static __inline td_void isp_sharpen_chrbmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_chrbmul)
{
    u_isp_sharpen_chrb_gain o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.u32 = be_reg->isp_sharpen_chrb_gain.u32;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_chrbmul = uisp_sharpen_chrbmul;
    be_reg->isp_sharpen_chrb_gain.u32 = o_isp_sharpen_chrb_gain.u32;
}
/*  description   set the value of the member isp_sharpen_gain_sft.isp_sharpen_mfgain_sft */
/*  input         unsigned int uisp_sharpen_mfgain_sft  3 bits */
static __inline td_void isp_sharpen_mfgain_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mfgain_sft)
{
    u_isp_sharpen_gain_sft o_isp_sharpen_gain_sft;
    o_isp_sharpen_gain_sft.u32 = be_reg->isp_sharpen_gain_sft.u32;
    o_isp_sharpen_gain_sft.bits.isp_sharpen_mfgain_sft = uisp_sharpen_mfgain_sft;
    be_reg->isp_sharpen_gain_sft.u32 = o_isp_sharpen_gain_sft.u32;
}
/*  description   set the value of the member isp_sharpen_gain_sft.isp_sharpen_lfgainwgt */
/*  input         unsigned int uisp_sharpen_lfgainwgt  5 bits */
static __inline td_void isp_sharpen_lfgainwgt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lfgainwgt)
{
    u_isp_sharpen_gain_sft o_isp_sharpen_gain_sft;
    o_isp_sharpen_gain_sft.u32 = be_reg->isp_sharpen_gain_sft.u32;
    o_isp_sharpen_gain_sft.bits.isp_sharpen_lfgainwgt = uisp_sharpen_lfgainwgt;
    be_reg->isp_sharpen_gain_sft.u32 = o_isp_sharpen_gain_sft.u32;
}
/*  description   set the value of the member isp_sharpen_gain_sft.isp_sharpen_hfgain_sft */
/*  input         unsigned int uisp_sharpen_hfgain_sft  3 bits */
static __inline td_void isp_sharpen_hfgain_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hfgain_sft)
{
    u_isp_sharpen_gain_sft o_isp_sharpen_gain_sft;
    o_isp_sharpen_gain_sft.u32 = be_reg->isp_sharpen_gain_sft.u32;
    o_isp_sharpen_gain_sft.bits.isp_sharpen_hfgain_sft = uisp_sharpen_hfgain_sft;
    be_reg->isp_sharpen_gain_sft.u32 = o_isp_sharpen_gain_sft.u32;
}
/*  description   set the value of the member isp_sharpen_shoot_maxgain.isp_sharpen_omaxgain */
/*  input         unsigned int uisp_sharpen_omaxgain  11 bits */
static __inline td_void isp_sharpen_omaxgain_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_omaxgain)
{
    u_isp_sharpen_shoot_maxgain o_isp_sharpen_shoot_maxgain;
    o_isp_sharpen_shoot_maxgain.u32 = be_reg->isp_sharpen_shoot_maxgain.u32;
    o_isp_sharpen_shoot_maxgain.bits.isp_sharpen_omaxgain = uisp_sharpen_omaxgain;
    be_reg->isp_sharpen_shoot_maxgain.u32 = o_isp_sharpen_shoot_maxgain.u32;
}
/*  description   set the value of the member isp_sharpen_shoot_maxgain.isp_sharpen_umaxgain */
/*  input         unsigned int uisp_sharpen_umaxgain  11 bits */
static __inline td_void isp_sharpen_umaxgain_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_umaxgain)
{
    u_isp_sharpen_shoot_maxgain o_isp_sharpen_shoot_maxgain;
    o_isp_sharpen_shoot_maxgain.u32 = be_reg->isp_sharpen_shoot_maxgain.u32;
    o_isp_sharpen_shoot_maxgain.bits.isp_sharpen_umaxgain = uisp_sharpen_umaxgain;
    be_reg->isp_sharpen_shoot_maxgain.u32 = o_isp_sharpen_shoot_maxgain.u32;
}
/*  description   set the value of the member isp_sharpen_detail_mul.isp_sharpen_detl_oshtmul */
/*  input         unsigned int uisp_sharpen_detl_oshtmul  12 bits */
static __inline td_void isp_sharpen_detl_oshtmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_detl_oshtmul)
{
    u_isp_sharpen_detail_mul o_isp_sharpen_detail_mul;
    o_isp_sharpen_detail_mul.u32 = be_reg->isp_sharpen_detail_mul.u32;
    o_isp_sharpen_detail_mul.bits.isp_sharpen_detl_oshtmul = uisp_sharpen_detl_oshtmul;
    be_reg->isp_sharpen_detail_mul.u32 = o_isp_sharpen_detail_mul.u32;
}
/*  description   set the value of the member isp_sharpen_detail_mul.isp_sharpen_detl_ushtmul */
/*  input         unsigned int uisp_sharpen_detl_ushtmul  12 bits */
static __inline td_void isp_sharpen_detl_ushtmul_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_detl_ushtmul)
{
    u_isp_sharpen_detail_mul o_isp_sharpen_detail_mul;
    o_isp_sharpen_detail_mul.u32 = be_reg->isp_sharpen_detail_mul.u32;
    o_isp_sharpen_detail_mul.bits.isp_sharpen_detl_ushtmul = uisp_sharpen_detl_ushtmul;
    be_reg->isp_sharpen_detail_mul.u32 = o_isp_sharpen_detail_mul.u32;
}
/*  description   set the value of the member isp_sharpen_osht_detail.isp_sharpen_osht_dtl_thd0 */
/*  input         unsigned int uisp_sharpen_osht_dtl_thd0  8 bits */
static __inline td_void isp_sharpen_osht_dtl_thd0_write(isp_be_reg_type *be_reg, td_u32 sharpen_osht_dtl_thd0)
{
    u_isp_sharpen_osht_detail o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = be_reg->isp_sharpen_osht_detail.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_thd0 = sharpen_osht_dtl_thd0;
    be_reg->isp_sharpen_osht_detail.u32 = o_isp_sharpen_osht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_osht_detail.isp_sharpen_osht_dtl_thd1 */
/*  input         unsigned int uisp_sharpen_osht_dtl_thd1  8 bits */
static __inline td_void isp_sharpen_osht_dtl_thd1_write(isp_be_reg_type *be_reg, td_u32 sharpen_osht_dtl_thd1)
{
    u_isp_sharpen_osht_detail o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = be_reg->isp_sharpen_osht_detail.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_thd1 = sharpen_osht_dtl_thd1;
    be_reg->isp_sharpen_osht_detail.u32 = o_isp_sharpen_osht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_osht_detail.isp_sharpen_osht_dtl_wgt */
/*  input         unsigned int uisp_sharpen_osht_dtl_wgt  7 bits */
static __inline td_void isp_sharpen_osht_dtl_wgt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_osht_dtl_wgt)
{
    u_isp_sharpen_osht_detail o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = be_reg->isp_sharpen_osht_detail.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_wgt = uisp_sharpen_osht_dtl_wgt;
    be_reg->isp_sharpen_osht_detail.u32 = o_isp_sharpen_osht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_osht_detail.isp_sharpen_max_var_clip */
/*  input         unsigned int uisp_sharpen_max_var_clip  8 bits */
static __inline td_void isp_sharpen_max_var_clip_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_max_var_clip)
{
    u_isp_sharpen_osht_detail o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.u32 = be_reg->isp_sharpen_osht_detail.u32;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_max_var_clip = uisp_sharpen_max_var_clip;
    be_reg->isp_sharpen_osht_detail.u32 = o_isp_sharpen_osht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_usht_detail.isp_sharpen_usht_dtl_thd0 */
/*  input         unsigned int uisp_sharpen_usht_dtl_thd0  8 bits */
static __inline td_void isp_sharpen_usht_dtl_thd0_write(isp_be_reg_type *be_reg, td_u32 sharpen_usht_dtl_thd0)
{
    u_isp_sharpen_usht_detail o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = be_reg->isp_sharpen_usht_detail.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_thd0 = sharpen_usht_dtl_thd0;
    be_reg->isp_sharpen_usht_detail.u32 = o_isp_sharpen_usht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_usht_detail.isp_sharpen_usht_dtl_thd1 */
/*  input         unsigned int uisp_sharpen_usht_dtl_thd1  8 bits */
static __inline td_void isp_sharpen_usht_dtl_thd1_write(isp_be_reg_type *be_reg, td_u32 sharpen_usht_dtl_thd1)
{
    u_isp_sharpen_usht_detail o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = be_reg->isp_sharpen_usht_detail.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_thd1 = sharpen_usht_dtl_thd1;
    be_reg->isp_sharpen_usht_detail.u32 = o_isp_sharpen_usht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_usht_detail.isp_sharpen_usht_dtl_wgt */
/*  input         unsigned int uisp_sharpen_usht_dtl_wgt  7 bits */
static __inline td_void isp_sharpen_usht_dtl_wgt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_usht_dtl_wgt)
{
    u_isp_sharpen_usht_detail o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = be_reg->isp_sharpen_usht_detail.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_wgt = uisp_sharpen_usht_dtl_wgt;
    be_reg->isp_sharpen_usht_detail.u32 = o_isp_sharpen_usht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_usht_detail.isp_sharpen_dtl_thdsft */
/*  input         unsigned int uisp_sharpen_dtl_thdsft  4 bits */
static __inline td_void isp_sharpen_dtl_thdsft_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_dtl_thdsft)
{
    u_isp_sharpen_usht_detail o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.u32 = be_reg->isp_sharpen_usht_detail.u32;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_dtl_thdsft = uisp_sharpen_dtl_thdsft;
    be_reg->isp_sharpen_usht_detail.u32 = o_isp_sharpen_usht_detail.u32;
}
/*  description   set the value of the member isp_sharpen_stt2lut_cfg.isp_sharpen_stt2lut_en */
/*  input         unsigned int uisp_sharpen_stt2lut_en  1 bits */
static __inline td_void isp_sharpen_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_stt2lut_en)
{
    u_isp_sharpen_stt2lut_cfg o_isp_sharpen_stt2lut_cfg;
    o_isp_sharpen_stt2lut_cfg.u32 = be_reg->isp_sharpen_stt2lut_cfg.u32;
    o_isp_sharpen_stt2lut_cfg.bits.isp_sharpen_stt2lut_en = uisp_sharpen_stt2lut_en;
    be_reg->isp_sharpen_stt2lut_cfg.u32 = o_isp_sharpen_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_sharpen_stt2lut_regnew.isp_sharpen_stt2lut_regnew */
/*  input         unsigned int uisp_sharpen_stt2lut_regnew  1 bits */
static __inline td_void isp_sharpen_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 sharpen_stt2lut_regnew)
{
    u_isp_sharpen_stt2lut_regnew o_isp_sharpen_stt2lut_regnew;
    o_isp_sharpen_stt2lut_regnew.u32 = be_reg->isp_sharpen_stt2lut_regnew.u32;
    o_isp_sharpen_stt2lut_regnew.bits.isp_sharpen_stt2lut_regnew = sharpen_stt2lut_regnew;
    be_reg->isp_sharpen_stt2lut_regnew.u32 = o_isp_sharpen_stt2lut_regnew.u32;
}
/*  description   set the value of the member isp_sharpen_stt2lut_abn.isp_sharpen_stt2lut_clr */
/*  input         unsigned int uisp_sharpen_stt2lut_clr  1 bits */
static __inline td_void isp_sharpen_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_stt2lut_clr)
{
    u_isp_sharpen_stt2lut_abn o_isp_sharpen_stt2lut_abn;
    o_isp_sharpen_stt2lut_abn.u32 = be_reg->isp_sharpen_stt2lut_abn.u32;
    o_isp_sharpen_stt2lut_abn.bits.isp_sharpen_stt2lut_clr = uisp_sharpen_stt2lut_clr;
    be_reg->isp_sharpen_stt2lut_abn.u32 = o_isp_sharpen_stt2lut_abn.u32;
}

static __inline td_u16 isp_sharpen_stt2lut_info_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_sharpen_stt2lut_abn.bits.isp_sharpen_stt2lut_info;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot0.isp_sharpen_std_gain_bymot0 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot0: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot0)
{
    u_isp_sharpen_std_gain_bymot0 o_isp_sharpen_std_gain_bymot0;
    o_isp_sharpen_std_gain_bymot0.u32 = be_reg->isp_sharpen_std_gain_bymot0.u32;
    o_isp_sharpen_std_gain_bymot0.bits.isp_sharpen_std_gain_bymot0 = uisp_sharpen_std_gain_bymot0;
    be_reg->isp_sharpen_std_gain_bymot0.u32 = o_isp_sharpen_std_gain_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot0.isp_sharpen_std_gain_bymot1 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot1: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot1)
{
    u_isp_sharpen_std_gain_bymot0 o_isp_sharpen_std_gain_bymot0;
    o_isp_sharpen_std_gain_bymot0.u32 = be_reg->isp_sharpen_std_gain_bymot0.u32;
    o_isp_sharpen_std_gain_bymot0.bits.isp_sharpen_std_gain_bymot1 = uisp_sharpen_std_gain_bymot1;
    be_reg->isp_sharpen_std_gain_bymot0.u32 = o_isp_sharpen_std_gain_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot0.isp_sharpen_std_gain_bymot2 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot2: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot2)
{
    u_isp_sharpen_std_gain_bymot0 o_isp_sharpen_std_gain_bymot0;
    o_isp_sharpen_std_gain_bymot0.u32 = be_reg->isp_sharpen_std_gain_bymot0.u32;
    o_isp_sharpen_std_gain_bymot0.bits.isp_sharpen_std_gain_bymot2 = uisp_sharpen_std_gain_bymot2;
    be_reg->isp_sharpen_std_gain_bymot0.u32 = o_isp_sharpen_std_gain_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot0.isp_sharpen_std_gain_bymot3 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot3: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot3)
{
    u_isp_sharpen_std_gain_bymot0 o_isp_sharpen_std_gain_bymot0;
    o_isp_sharpen_std_gain_bymot0.u32 = be_reg->isp_sharpen_std_gain_bymot0.u32;
    o_isp_sharpen_std_gain_bymot0.bits.isp_sharpen_std_gain_bymot3 = uisp_sharpen_std_gain_bymot3;
    be_reg->isp_sharpen_std_gain_bymot0.u32 = o_isp_sharpen_std_gain_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot0.isp_sharpen_std_gain_bymot4 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot4: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot4)
{
    u_isp_sharpen_std_gain_bymot0 o_isp_sharpen_std_gain_bymot0;
    o_isp_sharpen_std_gain_bymot0.u32 = be_reg->isp_sharpen_std_gain_bymot0.u32;
    o_isp_sharpen_std_gain_bymot0.bits.isp_sharpen_std_gain_bymot4 = uisp_sharpen_std_gain_bymot4;
    be_reg->isp_sharpen_std_gain_bymot0.u32 = o_isp_sharpen_std_gain_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot1.isp_sharpen_std_gain_bymot5 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot5: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot5)
{
    u_isp_sharpen_std_gain_bymot1 o_isp_sharpen_std_gain_bymot1;
    o_isp_sharpen_std_gain_bymot1.u32 = be_reg->isp_sharpen_std_gain_bymot1.u32;
    o_isp_sharpen_std_gain_bymot1.bits.isp_sharpen_std_gain_bymot5 = uisp_sharpen_std_gain_bymot5;
    be_reg->isp_sharpen_std_gain_bymot1.u32 = o_isp_sharpen_std_gain_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot1.isp_sharpen_std_gain_bymot6 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot6: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot6)
{
    u_isp_sharpen_std_gain_bymot1 o_isp_sharpen_std_gain_bymot1;
    o_isp_sharpen_std_gain_bymot1.u32 = be_reg->isp_sharpen_std_gain_bymot1.u32;
    o_isp_sharpen_std_gain_bymot1.bits.isp_sharpen_std_gain_bymot6 = uisp_sharpen_std_gain_bymot6;
    be_reg->isp_sharpen_std_gain_bymot1.u32 = o_isp_sharpen_std_gain_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot1.isp_sharpen_std_gain_bymot7 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot7: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot7)
{
    u_isp_sharpen_std_gain_bymot1 o_isp_sharpen_std_gain_bymot1;
    o_isp_sharpen_std_gain_bymot1.u32 = be_reg->isp_sharpen_std_gain_bymot1.u32;
    o_isp_sharpen_std_gain_bymot1.bits.isp_sharpen_std_gain_bymot7 = uisp_sharpen_std_gain_bymot7;
    be_reg->isp_sharpen_std_gain_bymot1.u32 = o_isp_sharpen_std_gain_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot1.isp_sharpen_std_gain_bymot8 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot8: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot8)
{
    u_isp_sharpen_std_gain_bymot1 o_isp_sharpen_std_gain_bymot1;
    o_isp_sharpen_std_gain_bymot1.u32 = be_reg->isp_sharpen_std_gain_bymot1.u32;
    o_isp_sharpen_std_gain_bymot1.bits.isp_sharpen_std_gain_bymot8 = uisp_sharpen_std_gain_bymot8;
    be_reg->isp_sharpen_std_gain_bymot1.u32 = o_isp_sharpen_std_gain_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot1.isp_sharpen_std_gain_bymot9 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot9: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot9)
{
    u_isp_sharpen_std_gain_bymot1 o_isp_sharpen_std_gain_bymot1;
    o_isp_sharpen_std_gain_bymot1.u32 = be_reg->isp_sharpen_std_gain_bymot1.u32;
    o_isp_sharpen_std_gain_bymot1.bits.isp_sharpen_std_gain_bymot9 = uisp_sharpen_std_gain_bymot9;
    be_reg->isp_sharpen_std_gain_bymot1.u32 = o_isp_sharpen_std_gain_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot2.isp_sharpen_std_gain_bymot10 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot10: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot10)
{
    u_isp_sharpen_std_gain_bymot2 o_isp_sharpen_std_gain_bymot2;
    o_isp_sharpen_std_gain_bymot2.u32 = be_reg->isp_sharpen_std_gain_bymot2.u32;
    o_isp_sharpen_std_gain_bymot2.bits.isp_sharpen_std_gain_bymot10 = uisp_sharpen_std_gain_bymot10;
    be_reg->isp_sharpen_std_gain_bymot2.u32 = o_isp_sharpen_std_gain_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot2.isp_sharpen_std_gain_bymot11 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot11: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot11)
{
    u_isp_sharpen_std_gain_bymot2 o_isp_sharpen_std_gain_bymot2;
    o_isp_sharpen_std_gain_bymot2.u32 = be_reg->isp_sharpen_std_gain_bymot2.u32;
    o_isp_sharpen_std_gain_bymot2.bits.isp_sharpen_std_gain_bymot11 = uisp_sharpen_std_gain_bymot11;
    be_reg->isp_sharpen_std_gain_bymot2.u32 = o_isp_sharpen_std_gain_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot2.isp_sharpen_std_gain_bymot12 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot12: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot12)
{
    u_isp_sharpen_std_gain_bymot2 o_isp_sharpen_std_gain_bymot2;
    o_isp_sharpen_std_gain_bymot2.u32 = be_reg->isp_sharpen_std_gain_bymot2.u32;
    o_isp_sharpen_std_gain_bymot2.bits.isp_sharpen_std_gain_bymot12 = uisp_sharpen_std_gain_bymot12;
    be_reg->isp_sharpen_std_gain_bymot2.u32 = o_isp_sharpen_std_gain_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot2.isp_sharpen_std_gain_bymot13 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot13: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot13)
{
    u_isp_sharpen_std_gain_bymot2 o_isp_sharpen_std_gain_bymot2;
    o_isp_sharpen_std_gain_bymot2.u32 = be_reg->isp_sharpen_std_gain_bymot2.u32;
    o_isp_sharpen_std_gain_bymot2.bits.isp_sharpen_std_gain_bymot13 = uisp_sharpen_std_gain_bymot13;
    be_reg->isp_sharpen_std_gain_bymot2.u32 = o_isp_sharpen_std_gain_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot2.isp_sharpen_std_gain_bymot14 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot14: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot14)
{
    u_isp_sharpen_std_gain_bymot2 o_isp_sharpen_std_gain_bymot2;
    o_isp_sharpen_std_gain_bymot2.u32 = be_reg->isp_sharpen_std_gain_bymot2.u32;
    o_isp_sharpen_std_gain_bymot2.bits.isp_sharpen_std_gain_bymot14 = uisp_sharpen_std_gain_bymot14;
    be_reg->isp_sharpen_std_gain_bymot2.u32 = o_isp_sharpen_std_gain_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_bymot3.isp_sharpen_std_gain_bymot15 */
/*  input       : unsigned int uisp_sharpen_std_gain_bymot15: 6 bits */
static __inline td_void isp_sharpen_std_gain_bymot15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_bymot15)
{
    u_isp_sharpen_std_gain_bymot3 o_isp_sharpen_std_gain_bymot3;
    o_isp_sharpen_std_gain_bymot3.u32 = be_reg->isp_sharpen_std_gain_bymot3.u32;
    o_isp_sharpen_std_gain_bymot3.bits.isp_sharpen_std_gain_bymot15 = uisp_sharpen_std_gain_bymot15;
    be_reg->isp_sharpen_std_gain_bymot3.u32 = o_isp_sharpen_std_gain_bymot3.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy0.isp_sharpen_std_gain_byy0 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy0: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy0)
{
    u_isp_sharpen_std_gain_byy0 o_isp_sharpen_std_gain_byy0;
    o_isp_sharpen_std_gain_byy0.u32 = be_reg->isp_sharpen_std_gain_byy0.u32;
    o_isp_sharpen_std_gain_byy0.bits.isp_sharpen_std_gain_byy0 = uisp_sharpen_std_gain_byy0;
    be_reg->isp_sharpen_std_gain_byy0.u32 = o_isp_sharpen_std_gain_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy0.isp_sharpen_std_gain_byy1 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy1: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy1)
{
    u_isp_sharpen_std_gain_byy0 o_isp_sharpen_std_gain_byy0;
    o_isp_sharpen_std_gain_byy0.u32 = be_reg->isp_sharpen_std_gain_byy0.u32;
    o_isp_sharpen_std_gain_byy0.bits.isp_sharpen_std_gain_byy1 = uisp_sharpen_std_gain_byy1;
    be_reg->isp_sharpen_std_gain_byy0.u32 = o_isp_sharpen_std_gain_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy0.isp_sharpen_std_gain_byy2 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy2: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy2)
{
    u_isp_sharpen_std_gain_byy0 o_isp_sharpen_std_gain_byy0;
    o_isp_sharpen_std_gain_byy0.u32 = be_reg->isp_sharpen_std_gain_byy0.u32;
    o_isp_sharpen_std_gain_byy0.bits.isp_sharpen_std_gain_byy2 = uisp_sharpen_std_gain_byy2;
    be_reg->isp_sharpen_std_gain_byy0.u32 = o_isp_sharpen_std_gain_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy0.isp_sharpen_std_gain_byy3 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy3: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy3)
{
    u_isp_sharpen_std_gain_byy0 o_isp_sharpen_std_gain_byy0;
    o_isp_sharpen_std_gain_byy0.u32 = be_reg->isp_sharpen_std_gain_byy0.u32;
    o_isp_sharpen_std_gain_byy0.bits.isp_sharpen_std_gain_byy3 = uisp_sharpen_std_gain_byy3;
    be_reg->isp_sharpen_std_gain_byy0.u32 = o_isp_sharpen_std_gain_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy0.isp_sharpen_std_gain_byy4 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy4: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy4)
{
    u_isp_sharpen_std_gain_byy0 o_isp_sharpen_std_gain_byy0;
    o_isp_sharpen_std_gain_byy0.u32 = be_reg->isp_sharpen_std_gain_byy0.u32;
    o_isp_sharpen_std_gain_byy0.bits.isp_sharpen_std_gain_byy4 = uisp_sharpen_std_gain_byy4;
    be_reg->isp_sharpen_std_gain_byy0.u32 = o_isp_sharpen_std_gain_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy1.isp_sharpen_std_gain_byy5 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy5: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy5)
{
    u_isp_sharpen_std_gain_byy1 o_isp_sharpen_std_gain_byy1;
    o_isp_sharpen_std_gain_byy1.u32 = be_reg->isp_sharpen_std_gain_byy1.u32;
    o_isp_sharpen_std_gain_byy1.bits.isp_sharpen_std_gain_byy5 = uisp_sharpen_std_gain_byy5;
    be_reg->isp_sharpen_std_gain_byy1.u32 = o_isp_sharpen_std_gain_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy1.isp_sharpen_std_gain_byy6 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy6: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy6)
{
    u_isp_sharpen_std_gain_byy1 o_isp_sharpen_std_gain_byy1;
    o_isp_sharpen_std_gain_byy1.u32 = be_reg->isp_sharpen_std_gain_byy1.u32;
    o_isp_sharpen_std_gain_byy1.bits.isp_sharpen_std_gain_byy6 = uisp_sharpen_std_gain_byy6;
    be_reg->isp_sharpen_std_gain_byy1.u32 = o_isp_sharpen_std_gain_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy1.isp_sharpen_std_gain_byy7 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy7: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy7)
{
    u_isp_sharpen_std_gain_byy1 o_isp_sharpen_std_gain_byy1;
    o_isp_sharpen_std_gain_byy1.u32 = be_reg->isp_sharpen_std_gain_byy1.u32;
    o_isp_sharpen_std_gain_byy1.bits.isp_sharpen_std_gain_byy7 = uisp_sharpen_std_gain_byy7;
    be_reg->isp_sharpen_std_gain_byy1.u32 = o_isp_sharpen_std_gain_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy1.isp_sharpen_std_gain_byy8 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy8: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy8)
{
    u_isp_sharpen_std_gain_byy1 o_isp_sharpen_std_gain_byy1;
    o_isp_sharpen_std_gain_byy1.u32 = be_reg->isp_sharpen_std_gain_byy1.u32;
    o_isp_sharpen_std_gain_byy1.bits.isp_sharpen_std_gain_byy8 = uisp_sharpen_std_gain_byy8;
    be_reg->isp_sharpen_std_gain_byy1.u32 = o_isp_sharpen_std_gain_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy1.isp_sharpen_std_gain_byy9 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy9: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy9)
{
    u_isp_sharpen_std_gain_byy1 o_isp_sharpen_std_gain_byy1;
    o_isp_sharpen_std_gain_byy1.u32 = be_reg->isp_sharpen_std_gain_byy1.u32;
    o_isp_sharpen_std_gain_byy1.bits.isp_sharpen_std_gain_byy9 = uisp_sharpen_std_gain_byy9;
    be_reg->isp_sharpen_std_gain_byy1.u32 = o_isp_sharpen_std_gain_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy2.isp_sharpen_std_gain_byy10 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy10: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy10)
{
    u_isp_sharpen_std_gain_byy2 o_isp_sharpen_std_gain_byy2;
    o_isp_sharpen_std_gain_byy2.u32 = be_reg->isp_sharpen_std_gain_byy2.u32;
    o_isp_sharpen_std_gain_byy2.bits.isp_sharpen_std_gain_byy10 = uisp_sharpen_std_gain_byy10;
    be_reg->isp_sharpen_std_gain_byy2.u32 = o_isp_sharpen_std_gain_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy2.isp_sharpen_std_gain_byy11 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy11: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy11)
{
    u_isp_sharpen_std_gain_byy2 o_isp_sharpen_std_gain_byy2;
    o_isp_sharpen_std_gain_byy2.u32 = be_reg->isp_sharpen_std_gain_byy2.u32;
    o_isp_sharpen_std_gain_byy2.bits.isp_sharpen_std_gain_byy11 = uisp_sharpen_std_gain_byy11;
    be_reg->isp_sharpen_std_gain_byy2.u32 = o_isp_sharpen_std_gain_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy2.isp_sharpen_std_gain_byy12 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy12: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy12)
{
    u_isp_sharpen_std_gain_byy2 o_isp_sharpen_std_gain_byy2;
    o_isp_sharpen_std_gain_byy2.u32 = be_reg->isp_sharpen_std_gain_byy2.u32;
    o_isp_sharpen_std_gain_byy2.bits.isp_sharpen_std_gain_byy12 = uisp_sharpen_std_gain_byy12;
    be_reg->isp_sharpen_std_gain_byy2.u32 = o_isp_sharpen_std_gain_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy2.isp_sharpen_std_gain_byy13 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy13: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy13)
{
    u_isp_sharpen_std_gain_byy2 o_isp_sharpen_std_gain_byy2;
    o_isp_sharpen_std_gain_byy2.u32 = be_reg->isp_sharpen_std_gain_byy2.u32;
    o_isp_sharpen_std_gain_byy2.bits.isp_sharpen_std_gain_byy13 = uisp_sharpen_std_gain_byy13;
    be_reg->isp_sharpen_std_gain_byy2.u32 = o_isp_sharpen_std_gain_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy2.isp_sharpen_std_gain_byy14 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy14: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy14)
{
    u_isp_sharpen_std_gain_byy2 o_isp_sharpen_std_gain_byy2;
    o_isp_sharpen_std_gain_byy2.u32 = be_reg->isp_sharpen_std_gain_byy2.u32;
    o_isp_sharpen_std_gain_byy2.bits.isp_sharpen_std_gain_byy14 = uisp_sharpen_std_gain_byy14;
    be_reg->isp_sharpen_std_gain_byy2.u32 = o_isp_sharpen_std_gain_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_gain_byy3.isp_sharpen_std_gain_byy15 */
/*  input       : unsigned int uisp_sharpen_std_gain_byy15: 6 bits */
static __inline td_void isp_sharpen_std_gain_byy15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_gain_byy15)
{
    u_isp_sharpen_std_gain_byy3 o_isp_sharpen_std_gain_byy3;
    o_isp_sharpen_std_gain_byy3.u32 = be_reg->isp_sharpen_std_gain_byy3.u32;
    o_isp_sharpen_std_gain_byy3.bits.isp_sharpen_std_gain_byy15 = uisp_sharpen_std_gain_byy15;
    be_reg->isp_sharpen_std_gain_byy3.u32 = o_isp_sharpen_std_gain_byy3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot0.isp_sharpen_std_offset_bymot0 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot0: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot0)
{
    u_isp_sharpen_std_offset_bymot0 o_isp_sharpen_std_offset_bymot0;
    o_isp_sharpen_std_offset_bymot0.u32 = be_reg->isp_sharpen_std_offset_bymot0.u32;
    o_isp_sharpen_std_offset_bymot0.bits.isp_sharpen_std_offset_bymot0 = uisp_sharpen_std_offset_bymot0;
    be_reg->isp_sharpen_std_offset_bymot0.u32 = o_isp_sharpen_std_offset_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot0.isp_sharpen_std_offset_bymot1 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot1: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot1)
{
    u_isp_sharpen_std_offset_bymot0 o_isp_sharpen_std_offset_bymot0;
    o_isp_sharpen_std_offset_bymot0.u32 = be_reg->isp_sharpen_std_offset_bymot0.u32;
    o_isp_sharpen_std_offset_bymot0.bits.isp_sharpen_std_offset_bymot1 = uisp_sharpen_std_offset_bymot1;
    be_reg->isp_sharpen_std_offset_bymot0.u32 = o_isp_sharpen_std_offset_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot0.isp_sharpen_std_offset_bymot2 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot2: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot2)
{
    u_isp_sharpen_std_offset_bymot0 o_isp_sharpen_std_offset_bymot0;
    o_isp_sharpen_std_offset_bymot0.u32 = be_reg->isp_sharpen_std_offset_bymot0.u32;
    o_isp_sharpen_std_offset_bymot0.bits.isp_sharpen_std_offset_bymot2 = uisp_sharpen_std_offset_bymot2;
    be_reg->isp_sharpen_std_offset_bymot0.u32 = o_isp_sharpen_std_offset_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot0.isp_sharpen_std_offset_bymot3 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot3: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot3)
{
    u_isp_sharpen_std_offset_bymot0 o_isp_sharpen_std_offset_bymot0;
    o_isp_sharpen_std_offset_bymot0.u32 = be_reg->isp_sharpen_std_offset_bymot0.u32;
    o_isp_sharpen_std_offset_bymot0.bits.isp_sharpen_std_offset_bymot3 = uisp_sharpen_std_offset_bymot3;
    be_reg->isp_sharpen_std_offset_bymot0.u32 = o_isp_sharpen_std_offset_bymot0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot1.isp_sharpen_std_offset_bymot4 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot4: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot4)
{
    u_isp_sharpen_std_offset_bymot1 o_isp_sharpen_std_offset_bymot1;
    o_isp_sharpen_std_offset_bymot1.u32 = be_reg->isp_sharpen_std_offset_bymot1.u32;
    o_isp_sharpen_std_offset_bymot1.bits.isp_sharpen_std_offset_bymot4 = uisp_sharpen_std_offset_bymot4;
    be_reg->isp_sharpen_std_offset_bymot1.u32 = o_isp_sharpen_std_offset_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot1.isp_sharpen_std_offset_bymot5 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot5: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot5)
{
    u_isp_sharpen_std_offset_bymot1 o_isp_sharpen_std_offset_bymot1;
    o_isp_sharpen_std_offset_bymot1.u32 = be_reg->isp_sharpen_std_offset_bymot1.u32;
    o_isp_sharpen_std_offset_bymot1.bits.isp_sharpen_std_offset_bymot5 = uisp_sharpen_std_offset_bymot5;
    be_reg->isp_sharpen_std_offset_bymot1.u32 = o_isp_sharpen_std_offset_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot1.isp_sharpen_std_offset_bymot6 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot6: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot6)
{
    u_isp_sharpen_std_offset_bymot1 o_isp_sharpen_std_offset_bymot1;
    o_isp_sharpen_std_offset_bymot1.u32 = be_reg->isp_sharpen_std_offset_bymot1.u32;
    o_isp_sharpen_std_offset_bymot1.bits.isp_sharpen_std_offset_bymot6 = uisp_sharpen_std_offset_bymot6;
    be_reg->isp_sharpen_std_offset_bymot1.u32 = o_isp_sharpen_std_offset_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot1.isp_sharpen_std_offset_bymot7 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot7: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot7)
{
    u_isp_sharpen_std_offset_bymot1 o_isp_sharpen_std_offset_bymot1;
    o_isp_sharpen_std_offset_bymot1.u32 = be_reg->isp_sharpen_std_offset_bymot1.u32;
    o_isp_sharpen_std_offset_bymot1.bits.isp_sharpen_std_offset_bymot7 = uisp_sharpen_std_offset_bymot7;
    be_reg->isp_sharpen_std_offset_bymot1.u32 = o_isp_sharpen_std_offset_bymot1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot2.isp_sharpen_std_offset_bymot8 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot8: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot8)
{
    u_isp_sharpen_std_offset_bymot2 o_isp_sharpen_std_offset_bymot2;
    o_isp_sharpen_std_offset_bymot2.u32 = be_reg->isp_sharpen_std_offset_bymot2.u32;
    o_isp_sharpen_std_offset_bymot2.bits.isp_sharpen_std_offset_bymot8 = uisp_sharpen_std_offset_bymot8;
    be_reg->isp_sharpen_std_offset_bymot2.u32 = o_isp_sharpen_std_offset_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot2.isp_sharpen_std_offset_bymot9 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot9: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot9)
{
    u_isp_sharpen_std_offset_bymot2 o_isp_sharpen_std_offset_bymot2;
    o_isp_sharpen_std_offset_bymot2.u32 = be_reg->isp_sharpen_std_offset_bymot2.u32;
    o_isp_sharpen_std_offset_bymot2.bits.isp_sharpen_std_offset_bymot9 = uisp_sharpen_std_offset_bymot9;
    be_reg->isp_sharpen_std_offset_bymot2.u32 = o_isp_sharpen_std_offset_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot2.isp_sharpen_std_offset_bymot10 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot10: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot10)
{
    u_isp_sharpen_std_offset_bymot2 o_isp_sharpen_std_offset_bymot2;
    o_isp_sharpen_std_offset_bymot2.u32 = be_reg->isp_sharpen_std_offset_bymot2.u32;
    o_isp_sharpen_std_offset_bymot2.bits.isp_sharpen_std_offset_bymot10 = uisp_sharpen_std_offset_bymot10;
    be_reg->isp_sharpen_std_offset_bymot2.u32 = o_isp_sharpen_std_offset_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot2.isp_sharpen_std_offset_bymot11 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot11: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot11)
{
    u_isp_sharpen_std_offset_bymot2 o_isp_sharpen_std_offset_bymot2;
    o_isp_sharpen_std_offset_bymot2.u32 = be_reg->isp_sharpen_std_offset_bymot2.u32;
    o_isp_sharpen_std_offset_bymot2.bits.isp_sharpen_std_offset_bymot11 = uisp_sharpen_std_offset_bymot11;
    be_reg->isp_sharpen_std_offset_bymot2.u32 = o_isp_sharpen_std_offset_bymot2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot3.isp_sharpen_std_offset_bymot12 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot12: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot12)
{
    u_isp_sharpen_std_offset_bymot3 o_isp_sharpen_std_offset_bymot3;
    o_isp_sharpen_std_offset_bymot3.u32 = be_reg->isp_sharpen_std_offset_bymot3.u32;
    o_isp_sharpen_std_offset_bymot3.bits.isp_sharpen_std_offset_bymot12 = uisp_sharpen_std_offset_bymot12;
    be_reg->isp_sharpen_std_offset_bymot3.u32 = o_isp_sharpen_std_offset_bymot3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot3.isp_sharpen_std_offset_bymot13 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot13: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot13)
{
    u_isp_sharpen_std_offset_bymot3 o_isp_sharpen_std_offset_bymot3;
    o_isp_sharpen_std_offset_bymot3.u32 = be_reg->isp_sharpen_std_offset_bymot3.u32;
    o_isp_sharpen_std_offset_bymot3.bits.isp_sharpen_std_offset_bymot13 = uisp_sharpen_std_offset_bymot13;
    be_reg->isp_sharpen_std_offset_bymot3.u32 = o_isp_sharpen_std_offset_bymot3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot3.isp_sharpen_std_offset_bymot14 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot14: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot14)
{
    u_isp_sharpen_std_offset_bymot3 o_isp_sharpen_std_offset_bymot3;
    o_isp_sharpen_std_offset_bymot3.u32 = be_reg->isp_sharpen_std_offset_bymot3.u32;
    o_isp_sharpen_std_offset_bymot3.bits.isp_sharpen_std_offset_bymot14 = uisp_sharpen_std_offset_bymot14;
    be_reg->isp_sharpen_std_offset_bymot3.u32 = o_isp_sharpen_std_offset_bymot3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_bymot3.isp_sharpen_std_offset_bymot15 */
/*  input       : unsigned int uisp_sharpen_std_offset_bymot15: 8 bits */
static __inline td_void isp_sharpen_std_offset_bymot15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_bymot15)
{
    u_isp_sharpen_std_offset_bymot3 o_isp_sharpen_std_offset_bymot3;
    o_isp_sharpen_std_offset_bymot3.u32 = be_reg->isp_sharpen_std_offset_bymot3.u32;
    o_isp_sharpen_std_offset_bymot3.bits.isp_sharpen_std_offset_bymot15 = uisp_sharpen_std_offset_bymot15;
    be_reg->isp_sharpen_std_offset_bymot3.u32 = o_isp_sharpen_std_offset_bymot3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy0.isp_sharpen_std_offset_byy0 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy0: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy0)
{
    u_isp_sharpen_std_offset_byy0 o_isp_sharpen_std_offset_byy0;
    o_isp_sharpen_std_offset_byy0.u32 = be_reg->isp_sharpen_std_offset_byy0.u32;
    o_isp_sharpen_std_offset_byy0.bits.isp_sharpen_std_offset_byy0 = uisp_sharpen_std_offset_byy0;
    be_reg->isp_sharpen_std_offset_byy0.u32 = o_isp_sharpen_std_offset_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy0.isp_sharpen_std_offset_byy1 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy1: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy1)
{
    u_isp_sharpen_std_offset_byy0 o_isp_sharpen_std_offset_byy0;
    o_isp_sharpen_std_offset_byy0.u32 = be_reg->isp_sharpen_std_offset_byy0.u32;
    o_isp_sharpen_std_offset_byy0.bits.isp_sharpen_std_offset_byy1 = uisp_sharpen_std_offset_byy1;
    be_reg->isp_sharpen_std_offset_byy0.u32 = o_isp_sharpen_std_offset_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy0.isp_sharpen_std_offset_byy2 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy2: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy2)
{
    u_isp_sharpen_std_offset_byy0 o_isp_sharpen_std_offset_byy0;
    o_isp_sharpen_std_offset_byy0.u32 = be_reg->isp_sharpen_std_offset_byy0.u32;
    o_isp_sharpen_std_offset_byy0.bits.isp_sharpen_std_offset_byy2 = uisp_sharpen_std_offset_byy2;
    be_reg->isp_sharpen_std_offset_byy0.u32 = o_isp_sharpen_std_offset_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy0.isp_sharpen_std_offset_byy3 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy3: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy3)
{
    u_isp_sharpen_std_offset_byy0 o_isp_sharpen_std_offset_byy0;
    o_isp_sharpen_std_offset_byy0.u32 = be_reg->isp_sharpen_std_offset_byy0.u32;
    o_isp_sharpen_std_offset_byy0.bits.isp_sharpen_std_offset_byy3 = uisp_sharpen_std_offset_byy3;
    be_reg->isp_sharpen_std_offset_byy0.u32 = o_isp_sharpen_std_offset_byy0.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy1.isp_sharpen_std_offset_byy4 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy4: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy4)
{
    u_isp_sharpen_std_offset_byy1 o_isp_sharpen_std_offset_byy1;
    o_isp_sharpen_std_offset_byy1.u32 = be_reg->isp_sharpen_std_offset_byy1.u32;
    o_isp_sharpen_std_offset_byy1.bits.isp_sharpen_std_offset_byy4 = uisp_sharpen_std_offset_byy4;
    be_reg->isp_sharpen_std_offset_byy1.u32 = o_isp_sharpen_std_offset_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy1.isp_sharpen_std_offset_byy5 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy5: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy5)
{
    u_isp_sharpen_std_offset_byy1 o_isp_sharpen_std_offset_byy1;
    o_isp_sharpen_std_offset_byy1.u32 = be_reg->isp_sharpen_std_offset_byy1.u32;
    o_isp_sharpen_std_offset_byy1.bits.isp_sharpen_std_offset_byy5 = uisp_sharpen_std_offset_byy5;
    be_reg->isp_sharpen_std_offset_byy1.u32 = o_isp_sharpen_std_offset_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy1.isp_sharpen_std_offset_byy6 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy6: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy6)
{
    u_isp_sharpen_std_offset_byy1 o_isp_sharpen_std_offset_byy1;
    o_isp_sharpen_std_offset_byy1.u32 = be_reg->isp_sharpen_std_offset_byy1.u32;
    o_isp_sharpen_std_offset_byy1.bits.isp_sharpen_std_offset_byy6 = uisp_sharpen_std_offset_byy6;
    be_reg->isp_sharpen_std_offset_byy1.u32 = o_isp_sharpen_std_offset_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy1.isp_sharpen_std_offset_byy7 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy7: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy7)
{
    u_isp_sharpen_std_offset_byy1 o_isp_sharpen_std_offset_byy1;
    o_isp_sharpen_std_offset_byy1.u32 = be_reg->isp_sharpen_std_offset_byy1.u32;
    o_isp_sharpen_std_offset_byy1.bits.isp_sharpen_std_offset_byy7 = uisp_sharpen_std_offset_byy7;
    be_reg->isp_sharpen_std_offset_byy1.u32 = o_isp_sharpen_std_offset_byy1.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy2.isp_sharpen_std_offset_byy8 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy8: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy8)
{
    u_isp_sharpen_std_offset_byy2 o_isp_sharpen_std_offset_byy2;
    o_isp_sharpen_std_offset_byy2.u32 = be_reg->isp_sharpen_std_offset_byy2.u32;
    o_isp_sharpen_std_offset_byy2.bits.isp_sharpen_std_offset_byy8 = uisp_sharpen_std_offset_byy8;
    be_reg->isp_sharpen_std_offset_byy2.u32 = o_isp_sharpen_std_offset_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy2.isp_sharpen_std_offset_byy9 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy9: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy9)
{
    u_isp_sharpen_std_offset_byy2 o_isp_sharpen_std_offset_byy2;
    o_isp_sharpen_std_offset_byy2.u32 = be_reg->isp_sharpen_std_offset_byy2.u32;
    o_isp_sharpen_std_offset_byy2.bits.isp_sharpen_std_offset_byy9 = uisp_sharpen_std_offset_byy9;
    be_reg->isp_sharpen_std_offset_byy2.u32 = o_isp_sharpen_std_offset_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy2.isp_sharpen_std_offset_byy10 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy10: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy10)
{
    u_isp_sharpen_std_offset_byy2 o_isp_sharpen_std_offset_byy2;
    o_isp_sharpen_std_offset_byy2.u32 = be_reg->isp_sharpen_std_offset_byy2.u32;
    o_isp_sharpen_std_offset_byy2.bits.isp_sharpen_std_offset_byy10 = uisp_sharpen_std_offset_byy10;
    be_reg->isp_sharpen_std_offset_byy2.u32 = o_isp_sharpen_std_offset_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy2.isp_sharpen_std_offset_byy11 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy11: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy11)
{
    u_isp_sharpen_std_offset_byy2 o_isp_sharpen_std_offset_byy2;
    o_isp_sharpen_std_offset_byy2.u32 = be_reg->isp_sharpen_std_offset_byy2.u32;
    o_isp_sharpen_std_offset_byy2.bits.isp_sharpen_std_offset_byy11 = uisp_sharpen_std_offset_byy11;
    be_reg->isp_sharpen_std_offset_byy2.u32 = o_isp_sharpen_std_offset_byy2.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy3.isp_sharpen_std_offset_byy12 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy12: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy12)
{
    u_isp_sharpen_std_offset_byy3 o_isp_sharpen_std_offset_byy3;
    o_isp_sharpen_std_offset_byy3.u32 = be_reg->isp_sharpen_std_offset_byy3.u32;
    o_isp_sharpen_std_offset_byy3.bits.isp_sharpen_std_offset_byy12 = uisp_sharpen_std_offset_byy12;
    be_reg->isp_sharpen_std_offset_byy3.u32 = o_isp_sharpen_std_offset_byy3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy3.isp_sharpen_std_offset_byy13 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy13: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy13)
{
    u_isp_sharpen_std_offset_byy3 o_isp_sharpen_std_offset_byy3;
    o_isp_sharpen_std_offset_byy3.u32 = be_reg->isp_sharpen_std_offset_byy3.u32;
    o_isp_sharpen_std_offset_byy3.bits.isp_sharpen_std_offset_byy13 = uisp_sharpen_std_offset_byy13;
    be_reg->isp_sharpen_std_offset_byy3.u32 = o_isp_sharpen_std_offset_byy3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy3.isp_sharpen_std_offset_byy14 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy14: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy14)
{
    u_isp_sharpen_std_offset_byy3 o_isp_sharpen_std_offset_byy3;
    o_isp_sharpen_std_offset_byy3.u32 = be_reg->isp_sharpen_std_offset_byy3.u32;
    o_isp_sharpen_std_offset_byy3.bits.isp_sharpen_std_offset_byy14 = uisp_sharpen_std_offset_byy14;
    be_reg->isp_sharpen_std_offset_byy3.u32 = o_isp_sharpen_std_offset_byy3.u32;
}

/*  description : set the value of the member isp_sharpen_std_offset_byy3.isp_sharpen_std_offset_byy15 */
/*  input       : unsigned int uisp_sharpen_std_offset_byy15: 8 bits */
static __inline td_void isp_sharpen_std_offset_byy15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_offset_byy15)
{
    u_isp_sharpen_std_offset_byy3 o_isp_sharpen_std_offset_byy3;
    o_isp_sharpen_std_offset_byy3.u32 = be_reg->isp_sharpen_std_offset_byy3.u32;
    o_isp_sharpen_std_offset_byy3.bits.isp_sharpen_std_offset_byy15 = uisp_sharpen_std_offset_byy15;
    be_reg->isp_sharpen_std_offset_byy3.u32 = o_isp_sharpen_std_offset_byy3.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain0.isp_sharpen_lmf_mot_gain0 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain0: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain0)
{
    u_isp_sharpen_lmf_mot_gain0 o_isp_sharpen_lmf_mot_gain0;
    o_isp_sharpen_lmf_mot_gain0.u32 = be_reg->isp_sharpen_lmf_mot_gain0.u32;
    o_isp_sharpen_lmf_mot_gain0.bits.isp_sharpen_lmf_mot_gain0 = uisp_sharpen_lmf_mot_gain0;
    be_reg->isp_sharpen_lmf_mot_gain0.u32 = o_isp_sharpen_lmf_mot_gain0.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain0.isp_sharpen_lmf_mot_gain1 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain1: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain1)
{
    u_isp_sharpen_lmf_mot_gain0 o_isp_sharpen_lmf_mot_gain0;
    o_isp_sharpen_lmf_mot_gain0.u32 = be_reg->isp_sharpen_lmf_mot_gain0.u32;
    o_isp_sharpen_lmf_mot_gain0.bits.isp_sharpen_lmf_mot_gain1 = uisp_sharpen_lmf_mot_gain1;
    be_reg->isp_sharpen_lmf_mot_gain0.u32 = o_isp_sharpen_lmf_mot_gain0.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain0.isp_sharpen_lmf_mot_gain2 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain2: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain2)
{
    u_isp_sharpen_lmf_mot_gain0 o_isp_sharpen_lmf_mot_gain0;
    o_isp_sharpen_lmf_mot_gain0.u32 = be_reg->isp_sharpen_lmf_mot_gain0.u32;
    o_isp_sharpen_lmf_mot_gain0.bits.isp_sharpen_lmf_mot_gain2 = uisp_sharpen_lmf_mot_gain2;
    be_reg->isp_sharpen_lmf_mot_gain0.u32 = o_isp_sharpen_lmf_mot_gain0.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain0.isp_sharpen_lmf_mot_gain3 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain3: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain3)
{
    u_isp_sharpen_lmf_mot_gain0 o_isp_sharpen_lmf_mot_gain0;
    o_isp_sharpen_lmf_mot_gain0.u32 = be_reg->isp_sharpen_lmf_mot_gain0.u32;
    o_isp_sharpen_lmf_mot_gain0.bits.isp_sharpen_lmf_mot_gain3 = uisp_sharpen_lmf_mot_gain3;
    be_reg->isp_sharpen_lmf_mot_gain0.u32 = o_isp_sharpen_lmf_mot_gain0.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain1.isp_sharpen_lmf_mot_gain4 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain4: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain4)
{
    u_isp_sharpen_lmf_mot_gain1 o_isp_sharpen_lmf_mot_gain1;
    o_isp_sharpen_lmf_mot_gain1.u32 = be_reg->isp_sharpen_lmf_mot_gain1.u32;
    o_isp_sharpen_lmf_mot_gain1.bits.isp_sharpen_lmf_mot_gain4 = uisp_sharpen_lmf_mot_gain4;
    be_reg->isp_sharpen_lmf_mot_gain1.u32 = o_isp_sharpen_lmf_mot_gain1.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain1.isp_sharpen_lmf_mot_gain5 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain5: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain5)
{
    u_isp_sharpen_lmf_mot_gain1 o_isp_sharpen_lmf_mot_gain1;
    o_isp_sharpen_lmf_mot_gain1.u32 = be_reg->isp_sharpen_lmf_mot_gain1.u32;
    o_isp_sharpen_lmf_mot_gain1.bits.isp_sharpen_lmf_mot_gain5 = uisp_sharpen_lmf_mot_gain5;
    be_reg->isp_sharpen_lmf_mot_gain1.u32 = o_isp_sharpen_lmf_mot_gain1.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain1.isp_sharpen_lmf_mot_gain6 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain6: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain6)
{
    u_isp_sharpen_lmf_mot_gain1 o_isp_sharpen_lmf_mot_gain1;
    o_isp_sharpen_lmf_mot_gain1.u32 = be_reg->isp_sharpen_lmf_mot_gain1.u32;
    o_isp_sharpen_lmf_mot_gain1.bits.isp_sharpen_lmf_mot_gain6 = uisp_sharpen_lmf_mot_gain6;
    be_reg->isp_sharpen_lmf_mot_gain1.u32 = o_isp_sharpen_lmf_mot_gain1.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain1.isp_sharpen_lmf_mot_gain7 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain7: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain7)
{
    u_isp_sharpen_lmf_mot_gain1 o_isp_sharpen_lmf_mot_gain1;
    o_isp_sharpen_lmf_mot_gain1.u32 = be_reg->isp_sharpen_lmf_mot_gain1.u32;
    o_isp_sharpen_lmf_mot_gain1.bits.isp_sharpen_lmf_mot_gain7 = uisp_sharpen_lmf_mot_gain7;
    be_reg->isp_sharpen_lmf_mot_gain1.u32 = o_isp_sharpen_lmf_mot_gain1.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain2.isp_sharpen_lmf_mot_gain8 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain8: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain8)
{
    u_isp_sharpen_lmf_mot_gain2 o_isp_sharpen_lmf_mot_gain2;
    o_isp_sharpen_lmf_mot_gain2.u32 = be_reg->isp_sharpen_lmf_mot_gain2.u32;
    o_isp_sharpen_lmf_mot_gain2.bits.isp_sharpen_lmf_mot_gain8 = uisp_sharpen_lmf_mot_gain8;
    be_reg->isp_sharpen_lmf_mot_gain2.u32 = o_isp_sharpen_lmf_mot_gain2.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain2.isp_sharpen_lmf_mot_gain9 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain9: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain9)
{
    u_isp_sharpen_lmf_mot_gain2 o_isp_sharpen_lmf_mot_gain2;
    o_isp_sharpen_lmf_mot_gain2.u32 = be_reg->isp_sharpen_lmf_mot_gain2.u32;
    o_isp_sharpen_lmf_mot_gain2.bits.isp_sharpen_lmf_mot_gain9 = uisp_sharpen_lmf_mot_gain9;
    be_reg->isp_sharpen_lmf_mot_gain2.u32 = o_isp_sharpen_lmf_mot_gain2.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain2.isp_sharpen_lmf_mot_gain10 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain10: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain10)
{
    u_isp_sharpen_lmf_mot_gain2 o_isp_sharpen_lmf_mot_gain2;
    o_isp_sharpen_lmf_mot_gain2.u32 = be_reg->isp_sharpen_lmf_mot_gain2.u32;
    o_isp_sharpen_lmf_mot_gain2.bits.isp_sharpen_lmf_mot_gain10 = uisp_sharpen_lmf_mot_gain10;
    be_reg->isp_sharpen_lmf_mot_gain2.u32 = o_isp_sharpen_lmf_mot_gain2.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain2.isp_sharpen_lmf_mot_gain11 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain11: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain11)
{
    u_isp_sharpen_lmf_mot_gain2 o_isp_sharpen_lmf_mot_gain2;
    o_isp_sharpen_lmf_mot_gain2.u32 = be_reg->isp_sharpen_lmf_mot_gain2.u32;
    o_isp_sharpen_lmf_mot_gain2.bits.isp_sharpen_lmf_mot_gain11 = uisp_sharpen_lmf_mot_gain11;
    be_reg->isp_sharpen_lmf_mot_gain2.u32 = o_isp_sharpen_lmf_mot_gain2.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain3.isp_sharpen_lmf_mot_gain12 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain12: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain12)
{
    u_isp_sharpen_lmf_mot_gain3 o_isp_sharpen_lmf_mot_gain3;
    o_isp_sharpen_lmf_mot_gain3.u32 = be_reg->isp_sharpen_lmf_mot_gain3.u32;
    o_isp_sharpen_lmf_mot_gain3.bits.isp_sharpen_lmf_mot_gain12 = uisp_sharpen_lmf_mot_gain12;
    be_reg->isp_sharpen_lmf_mot_gain3.u32 = o_isp_sharpen_lmf_mot_gain3.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain3.isp_sharpen_lmf_mot_gain13 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain13: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain13)
{
    u_isp_sharpen_lmf_mot_gain3 o_isp_sharpen_lmf_mot_gain3;
    o_isp_sharpen_lmf_mot_gain3.u32 = be_reg->isp_sharpen_lmf_mot_gain3.u32;
    o_isp_sharpen_lmf_mot_gain3.bits.isp_sharpen_lmf_mot_gain13 = uisp_sharpen_lmf_mot_gain13;
    be_reg->isp_sharpen_lmf_mot_gain3.u32 = o_isp_sharpen_lmf_mot_gain3.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain3.isp_sharpen_lmf_mot_gain14 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain14: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain14)
{
    u_isp_sharpen_lmf_mot_gain3 o_isp_sharpen_lmf_mot_gain3;
    o_isp_sharpen_lmf_mot_gain3.u32 = be_reg->isp_sharpen_lmf_mot_gain3.u32;
    o_isp_sharpen_lmf_mot_gain3.bits.isp_sharpen_lmf_mot_gain14 = uisp_sharpen_lmf_mot_gain14;
    be_reg->isp_sharpen_lmf_mot_gain3.u32 = o_isp_sharpen_lmf_mot_gain3.u32;
}

/*  description : set the value of the member isp_sharpen_lmf_mot_gain3.isp_sharpen_lmf_mot_gain15 */
/*  input       : unsigned int uisp_sharpen_lmf_mot_gain15: 8 bits */
static __inline td_void isp_sharpen_lmf_mot_gain15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_lmf_mot_gain15)
{
    u_isp_sharpen_lmf_mot_gain3 o_isp_sharpen_lmf_mot_gain3;
    o_isp_sharpen_lmf_mot_gain3.u32 = be_reg->isp_sharpen_lmf_mot_gain3.u32;
    o_isp_sharpen_lmf_mot_gain3.bits.isp_sharpen_lmf_mot_gain15 = uisp_sharpen_lmf_mot_gain15;
    be_reg->isp_sharpen_lmf_mot_gain3.u32 = o_isp_sharpen_lmf_mot_gain3.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec0.isp_sharpen_mf_mot_dec0 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec0: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec0)
{
    u_isp_sharpen_mf_mot_dec0 o_isp_sharpen_mf_mot_dec0;
    o_isp_sharpen_mf_mot_dec0.u32 = be_reg->isp_sharpen_mf_mot_dec0.u32;
    o_isp_sharpen_mf_mot_dec0.bits.isp_sharpen_mf_mot_dec0 = uisp_sharpen_mf_mot_dec0;
    be_reg->isp_sharpen_mf_mot_dec0.u32 = o_isp_sharpen_mf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec0.isp_sharpen_mf_mot_dec1 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec1: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec1)
{
    u_isp_sharpen_mf_mot_dec0 o_isp_sharpen_mf_mot_dec0;
    o_isp_sharpen_mf_mot_dec0.u32 = be_reg->isp_sharpen_mf_mot_dec0.u32;
    o_isp_sharpen_mf_mot_dec0.bits.isp_sharpen_mf_mot_dec1 = uisp_sharpen_mf_mot_dec1;
    be_reg->isp_sharpen_mf_mot_dec0.u32 = o_isp_sharpen_mf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec0.isp_sharpen_mf_mot_dec2 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec2: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec2)
{
    u_isp_sharpen_mf_mot_dec0 o_isp_sharpen_mf_mot_dec0;
    o_isp_sharpen_mf_mot_dec0.u32 = be_reg->isp_sharpen_mf_mot_dec0.u32;
    o_isp_sharpen_mf_mot_dec0.bits.isp_sharpen_mf_mot_dec2 = uisp_sharpen_mf_mot_dec2;
    be_reg->isp_sharpen_mf_mot_dec0.u32 = o_isp_sharpen_mf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec0.isp_sharpen_mf_mot_dec3 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec3: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec3)
{
    u_isp_sharpen_mf_mot_dec0 o_isp_sharpen_mf_mot_dec0;
    o_isp_sharpen_mf_mot_dec0.u32 = be_reg->isp_sharpen_mf_mot_dec0.u32;
    o_isp_sharpen_mf_mot_dec0.bits.isp_sharpen_mf_mot_dec3 = uisp_sharpen_mf_mot_dec3;
    be_reg->isp_sharpen_mf_mot_dec0.u32 = o_isp_sharpen_mf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec0.isp_sharpen_mf_mot_dec4 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec4: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec4)
{
    u_isp_sharpen_mf_mot_dec0 o_isp_sharpen_mf_mot_dec0;
    o_isp_sharpen_mf_mot_dec0.u32 = be_reg->isp_sharpen_mf_mot_dec0.u32;
    o_isp_sharpen_mf_mot_dec0.bits.isp_sharpen_mf_mot_dec4 = uisp_sharpen_mf_mot_dec4;
    be_reg->isp_sharpen_mf_mot_dec0.u32 = o_isp_sharpen_mf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec0.isp_sharpen_mf_mot_dec5 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec5: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec5)
{
    u_isp_sharpen_mf_mot_dec1 o_isp_sharpen_mf_mot_dec1;
    o_isp_sharpen_mf_mot_dec1.u32 = be_reg->isp_sharpen_mf_mot_dec1.u32;
    o_isp_sharpen_mf_mot_dec1.bits.isp_sharpen_mf_mot_dec5 = uisp_sharpen_mf_mot_dec5;
    be_reg->isp_sharpen_mf_mot_dec1.u32 = o_isp_sharpen_mf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec1.isp_sharpen_mf_mot_dec6 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec6: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec6)
{
    u_isp_sharpen_mf_mot_dec1 o_isp_sharpen_mf_mot_dec1;
    o_isp_sharpen_mf_mot_dec1.u32 = be_reg->isp_sharpen_mf_mot_dec1.u32;
    o_isp_sharpen_mf_mot_dec1.bits.isp_sharpen_mf_mot_dec6 = uisp_sharpen_mf_mot_dec6;
    be_reg->isp_sharpen_mf_mot_dec1.u32 = o_isp_sharpen_mf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec1.isp_sharpen_mf_mot_dec7 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec7: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec7)
{
    u_isp_sharpen_mf_mot_dec1 o_isp_sharpen_mf_mot_dec1;
    o_isp_sharpen_mf_mot_dec1.u32 = be_reg->isp_sharpen_mf_mot_dec1.u32;
    o_isp_sharpen_mf_mot_dec1.bits.isp_sharpen_mf_mot_dec7 = uisp_sharpen_mf_mot_dec7;
    be_reg->isp_sharpen_mf_mot_dec1.u32 = o_isp_sharpen_mf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec1.isp_sharpen_mf_mot_dec8 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec8: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec8)
{
    u_isp_sharpen_mf_mot_dec1 o_isp_sharpen_mf_mot_dec1;
    o_isp_sharpen_mf_mot_dec1.u32 = be_reg->isp_sharpen_mf_mot_dec1.u32;
    o_isp_sharpen_mf_mot_dec1.bits.isp_sharpen_mf_mot_dec8 = uisp_sharpen_mf_mot_dec8;
    be_reg->isp_sharpen_mf_mot_dec1.u32 = o_isp_sharpen_mf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec1.isp_sharpen_mf_mot_dec9 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec9: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec9)
{
    u_isp_sharpen_mf_mot_dec1 o_isp_sharpen_mf_mot_dec1;
    o_isp_sharpen_mf_mot_dec1.u32 = be_reg->isp_sharpen_mf_mot_dec1.u32;
    o_isp_sharpen_mf_mot_dec1.bits.isp_sharpen_mf_mot_dec9 = uisp_sharpen_mf_mot_dec9;
    be_reg->isp_sharpen_mf_mot_dec1.u32 = o_isp_sharpen_mf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec1.isp_sharpen_mf_mot_dec10 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec10: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec10)
{
    u_isp_sharpen_mf_mot_dec2 o_isp_sharpen_mf_mot_dec2;
    o_isp_sharpen_mf_mot_dec2.u32 = be_reg->isp_sharpen_mf_mot_dec2.u32;
    o_isp_sharpen_mf_mot_dec2.bits.isp_sharpen_mf_mot_dec10 = uisp_sharpen_mf_mot_dec10;
    be_reg->isp_sharpen_mf_mot_dec2.u32 = o_isp_sharpen_mf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec1.isp_sharpen_mf_mot_dec11 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec11: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec11)
{
    u_isp_sharpen_mf_mot_dec2 o_isp_sharpen_mf_mot_dec2;
    o_isp_sharpen_mf_mot_dec2.u32 = be_reg->isp_sharpen_mf_mot_dec2.u32;
    o_isp_sharpen_mf_mot_dec2.bits.isp_sharpen_mf_mot_dec11 = uisp_sharpen_mf_mot_dec11;
    be_reg->isp_sharpen_mf_mot_dec2.u32 = o_isp_sharpen_mf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec2.isp_sharpen_mf_mot_dec12 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec12: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec12)
{
    u_isp_sharpen_mf_mot_dec2 o_isp_sharpen_mf_mot_dec2;
    o_isp_sharpen_mf_mot_dec2.u32 = be_reg->isp_sharpen_mf_mot_dec2.u32;
    o_isp_sharpen_mf_mot_dec2.bits.isp_sharpen_mf_mot_dec12 = uisp_sharpen_mf_mot_dec12;
    be_reg->isp_sharpen_mf_mot_dec2.u32 = o_isp_sharpen_mf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec2.isp_sharpen_mf_mot_dec13 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec13: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec13)
{
    u_isp_sharpen_mf_mot_dec2 o_isp_sharpen_mf_mot_dec2;
    o_isp_sharpen_mf_mot_dec2.u32 = be_reg->isp_sharpen_mf_mot_dec2.u32;
    o_isp_sharpen_mf_mot_dec2.bits.isp_sharpen_mf_mot_dec13 = uisp_sharpen_mf_mot_dec13;
    be_reg->isp_sharpen_mf_mot_dec2.u32 = o_isp_sharpen_mf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec2.isp_sharpen_mf_mot_dec14 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec14: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec14)
{
    u_isp_sharpen_mf_mot_dec2 o_isp_sharpen_mf_mot_dec2;
    o_isp_sharpen_mf_mot_dec2.u32 = be_reg->isp_sharpen_mf_mot_dec2.u32;
    o_isp_sharpen_mf_mot_dec2.bits.isp_sharpen_mf_mot_dec14 = uisp_sharpen_mf_mot_dec14;
    be_reg->isp_sharpen_mf_mot_dec2.u32 = o_isp_sharpen_mf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_mf_mot_dec2.isp_sharpen_mf_mot_dec15 */
/*  input       : unsigned int uisp_sharpen_mf_mot_dec15: 5 bits */
static __inline td_void isp_sharpen_mf_mot_dec15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mf_mot_dec15)
{
    u_isp_sharpen_mf_mot_dec3 o_isp_sharpen_mf_mot_dec3;
    o_isp_sharpen_mf_mot_dec3.u32 = be_reg->isp_sharpen_mf_mot_dec3.u32;
    o_isp_sharpen_mf_mot_dec3.bits.isp_sharpen_mf_mot_dec15 = uisp_sharpen_mf_mot_dec15;
    be_reg->isp_sharpen_mf_mot_dec3.u32 = o_isp_sharpen_mf_mot_dec3.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec0.isp_sharpen_hf_mot_dec0 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec0: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec0)
{
    u_isp_sharpen_hf_mot_dec0 o_isp_sharpen_hf_mot_dec0;
    o_isp_sharpen_hf_mot_dec0.u32 = be_reg->isp_sharpen_hf_mot_dec0.u32;
    o_isp_sharpen_hf_mot_dec0.bits.isp_sharpen_hf_mot_dec0 = uisp_sharpen_hf_mot_dec0;
    be_reg->isp_sharpen_hf_mot_dec0.u32 = o_isp_sharpen_hf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec0.isp_sharpen_hf_mot_dec1 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec1: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec1)
{
    u_isp_sharpen_hf_mot_dec0 o_isp_sharpen_hf_mot_dec0;
    o_isp_sharpen_hf_mot_dec0.u32 = be_reg->isp_sharpen_hf_mot_dec0.u32;
    o_isp_sharpen_hf_mot_dec0.bits.isp_sharpen_hf_mot_dec1 = uisp_sharpen_hf_mot_dec1;
    be_reg->isp_sharpen_hf_mot_dec0.u32 = o_isp_sharpen_hf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec0.isp_sharpen_hf_mot_dec2 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec2: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec2)
{
    u_isp_sharpen_hf_mot_dec0 o_isp_sharpen_hf_mot_dec0;
    o_isp_sharpen_hf_mot_dec0.u32 = be_reg->isp_sharpen_hf_mot_dec0.u32;
    o_isp_sharpen_hf_mot_dec0.bits.isp_sharpen_hf_mot_dec2 = uisp_sharpen_hf_mot_dec2;
    be_reg->isp_sharpen_hf_mot_dec0.u32 = o_isp_sharpen_hf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec0.isp_sharpen_hf_mot_dec3 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec3: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec3)
{
    u_isp_sharpen_hf_mot_dec0 o_isp_sharpen_hf_mot_dec0;
    o_isp_sharpen_hf_mot_dec0.u32 = be_reg->isp_sharpen_hf_mot_dec0.u32;
    o_isp_sharpen_hf_mot_dec0.bits.isp_sharpen_hf_mot_dec3 = uisp_sharpen_hf_mot_dec3;
    be_reg->isp_sharpen_hf_mot_dec0.u32 = o_isp_sharpen_hf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec0.isp_sharpen_hf_mot_dec4 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec4: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec4)
{
    u_isp_sharpen_hf_mot_dec0 o_isp_sharpen_hf_mot_dec0;
    o_isp_sharpen_hf_mot_dec0.u32 = be_reg->isp_sharpen_hf_mot_dec0.u32;
    o_isp_sharpen_hf_mot_dec0.bits.isp_sharpen_hf_mot_dec4 = uisp_sharpen_hf_mot_dec4;
    be_reg->isp_sharpen_hf_mot_dec0.u32 = o_isp_sharpen_hf_mot_dec0.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec0.isp_sharpen_hf_mot_dec5 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec5: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec5)
{
    u_isp_sharpen_hf_mot_dec1 o_isp_sharpen_hf_mot_dec1;
    o_isp_sharpen_hf_mot_dec1.u32 = be_reg->isp_sharpen_hf_mot_dec1.u32;
    o_isp_sharpen_hf_mot_dec1.bits.isp_sharpen_hf_mot_dec5 = uisp_sharpen_hf_mot_dec5;
    be_reg->isp_sharpen_hf_mot_dec1.u32 = o_isp_sharpen_hf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec1.isp_sharpen_hf_mot_dec6 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec6: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec6)
{
    u_isp_sharpen_hf_mot_dec1 o_isp_sharpen_hf_mot_dec1;
    o_isp_sharpen_hf_mot_dec1.u32 = be_reg->isp_sharpen_hf_mot_dec1.u32;
    o_isp_sharpen_hf_mot_dec1.bits.isp_sharpen_hf_mot_dec6 = uisp_sharpen_hf_mot_dec6;
    be_reg->isp_sharpen_hf_mot_dec1.u32 = o_isp_sharpen_hf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec1.isp_sharpen_hf_mot_dec7 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec7: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec7)
{
    u_isp_sharpen_hf_mot_dec1 o_isp_sharpen_hf_mot_dec1;
    o_isp_sharpen_hf_mot_dec1.u32 = be_reg->isp_sharpen_hf_mot_dec1.u32;
    o_isp_sharpen_hf_mot_dec1.bits.isp_sharpen_hf_mot_dec7 = uisp_sharpen_hf_mot_dec7;
    be_reg->isp_sharpen_hf_mot_dec1.u32 = o_isp_sharpen_hf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec1.isp_sharpen_hf_mot_dec8 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec8: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec8)
{
    u_isp_sharpen_hf_mot_dec1 o_isp_sharpen_hf_mot_dec1;
    o_isp_sharpen_hf_mot_dec1.u32 = be_reg->isp_sharpen_hf_mot_dec1.u32;
    o_isp_sharpen_hf_mot_dec1.bits.isp_sharpen_hf_mot_dec8 = uisp_sharpen_hf_mot_dec8;
    be_reg->isp_sharpen_hf_mot_dec1.u32 = o_isp_sharpen_hf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec1.isp_sharpen_hf_mot_dec9 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec9: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec9)
{
    u_isp_sharpen_hf_mot_dec1 o_isp_sharpen_hf_mot_dec1;
    o_isp_sharpen_hf_mot_dec1.u32 = be_reg->isp_sharpen_hf_mot_dec1.u32;
    o_isp_sharpen_hf_mot_dec1.bits.isp_sharpen_hf_mot_dec9 = uisp_sharpen_hf_mot_dec9;
    be_reg->isp_sharpen_hf_mot_dec1.u32 = o_isp_sharpen_hf_mot_dec1.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec1.isp_sharpen_hf_mot_dec10 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec10: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec10)
{
    u_isp_sharpen_hf_mot_dec2 o_isp_sharpen_hf_mot_dec2;
    o_isp_sharpen_hf_mot_dec2.u32 = be_reg->isp_sharpen_hf_mot_dec2.u32;
    o_isp_sharpen_hf_mot_dec2.bits.isp_sharpen_hf_mot_dec10 = uisp_sharpen_hf_mot_dec10;
    be_reg->isp_sharpen_hf_mot_dec2.u32 = o_isp_sharpen_hf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec1.isp_sharpen_hf_mot_dec11 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec11: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec11)
{
    u_isp_sharpen_hf_mot_dec2 o_isp_sharpen_hf_mot_dec2;
    o_isp_sharpen_hf_mot_dec2.u32 = be_reg->isp_sharpen_hf_mot_dec2.u32;
    o_isp_sharpen_hf_mot_dec2.bits.isp_sharpen_hf_mot_dec11 = uisp_sharpen_hf_mot_dec11;
    be_reg->isp_sharpen_hf_mot_dec2.u32 = o_isp_sharpen_hf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec2.isp_sharpen_hf_mot_dec12 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec12: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec12)
{
    u_isp_sharpen_hf_mot_dec2 o_isp_sharpen_hf_mot_dec2;
    o_isp_sharpen_hf_mot_dec2.u32 = be_reg->isp_sharpen_hf_mot_dec2.u32;
    o_isp_sharpen_hf_mot_dec2.bits.isp_sharpen_hf_mot_dec12 = uisp_sharpen_hf_mot_dec12;
    be_reg->isp_sharpen_hf_mot_dec2.u32 = o_isp_sharpen_hf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec2.isp_sharpen_hf_mot_dec13 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec13: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec13)
{
    u_isp_sharpen_hf_mot_dec2 o_isp_sharpen_hf_mot_dec2;
    o_isp_sharpen_hf_mot_dec2.u32 = be_reg->isp_sharpen_hf_mot_dec2.u32;
    o_isp_sharpen_hf_mot_dec2.bits.isp_sharpen_hf_mot_dec13 = uisp_sharpen_hf_mot_dec13;
    be_reg->isp_sharpen_hf_mot_dec2.u32 = o_isp_sharpen_hf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec2.isp_sharpen_hf_mot_dec14 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec14: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec14)
{
    u_isp_sharpen_hf_mot_dec2 o_isp_sharpen_hf_mot_dec2;
    o_isp_sharpen_hf_mot_dec2.u32 = be_reg->isp_sharpen_hf_mot_dec2.u32;
    o_isp_sharpen_hf_mot_dec2.bits.isp_sharpen_hf_mot_dec14 = uisp_sharpen_hf_mot_dec14;
    be_reg->isp_sharpen_hf_mot_dec2.u32 = o_isp_sharpen_hf_mot_dec2.u32;
}

/*  description : set the value of the member isp_sharpen_hf_mot_dec2.isp_sharpen_hf_mot_dec15 */
/*  input       : unsigned int uisp_sharpen_hf_mot_dec15: 5 bits */
static __inline td_void isp_sharpen_hf_mot_dec15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_hf_mot_dec15)
{
    u_isp_sharpen_hf_mot_dec3 o_isp_sharpen_hf_mot_dec3;
    o_isp_sharpen_hf_mot_dec3.u32 = be_reg->isp_sharpen_hf_mot_dec3.u32;
    o_isp_sharpen_hf_mot_dec3.bits.isp_sharpen_hf_mot_dec15 = uisp_sharpen_hf_mot_dec15;
    be_reg->isp_sharpen_hf_mot_dec3.u32 = o_isp_sharpen_hf_mot_dec3.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt0.isp_sharpen_rly_wgt0 */
/*  input       : unsigned int uisp_sharpen_rly_wgt0: 8 bits */
static __inline td_void isp_sharpen_rly_wgt0_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt0)
{
    u_isp_sharpen_rly_wgt0 o_isp_sharpen_rly_wgt0;
    o_isp_sharpen_rly_wgt0.u32 = be_reg->isp_sharpen_rly_wgt0.u32;
    o_isp_sharpen_rly_wgt0.bits.isp_sharpen_rly_wgt0 = uisp_sharpen_rly_wgt0;
    be_reg->isp_sharpen_rly_wgt0.u32 = o_isp_sharpen_rly_wgt0.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt0.isp_sharpen_rly_wgt1 */
/*  input       : unsigned int uisp_sharpen_rly_wgt1: 8 bits */
static __inline td_void isp_sharpen_rly_wgt1_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt1)
{
    u_isp_sharpen_rly_wgt0 o_isp_sharpen_rly_wgt0;
    o_isp_sharpen_rly_wgt0.u32 = be_reg->isp_sharpen_rly_wgt0.u32;
    o_isp_sharpen_rly_wgt0.bits.isp_sharpen_rly_wgt1 = uisp_sharpen_rly_wgt1;
    be_reg->isp_sharpen_rly_wgt0.u32 = o_isp_sharpen_rly_wgt0.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt0.isp_sharpen_rly_wgt2 */
/*  input       : unsigned int uisp_sharpen_rly_wgt2: 8 bits */
static __inline td_void isp_sharpen_rly_wgt2_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt2)
{
    u_isp_sharpen_rly_wgt0 o_isp_sharpen_rly_wgt0;
    o_isp_sharpen_rly_wgt0.u32 = be_reg->isp_sharpen_rly_wgt0.u32;
    o_isp_sharpen_rly_wgt0.bits.isp_sharpen_rly_wgt2 = uisp_sharpen_rly_wgt2;
    be_reg->isp_sharpen_rly_wgt0.u32 = o_isp_sharpen_rly_wgt0.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt0.isp_sharpen_rly_wgt3 */
/*  input       : unsigned int uisp_sharpen_rly_wgt3: 8 bits */
static __inline td_void isp_sharpen_rly_wgt3_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt3)
{
    u_isp_sharpen_rly_wgt0 o_isp_sharpen_rly_wgt0;
    o_isp_sharpen_rly_wgt0.u32 = be_reg->isp_sharpen_rly_wgt0.u32;
    o_isp_sharpen_rly_wgt0.bits.isp_sharpen_rly_wgt3 = uisp_sharpen_rly_wgt3;
    be_reg->isp_sharpen_rly_wgt0.u32 = o_isp_sharpen_rly_wgt0.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt1.isp_sharpen_rly_wgt4 */
/*  input       : unsigned int uisp_sharpen_rly_wgt4: 8 bits */
static __inline td_void isp_sharpen_rly_wgt4_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt4)
{
    u_isp_sharpen_rly_wgt1 o_isp_sharpen_rly_wgt1;
    o_isp_sharpen_rly_wgt1.u32 = be_reg->isp_sharpen_rly_wgt1.u32;
    o_isp_sharpen_rly_wgt1.bits.isp_sharpen_rly_wgt4 = uisp_sharpen_rly_wgt4;
    be_reg->isp_sharpen_rly_wgt1.u32 = o_isp_sharpen_rly_wgt1.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt1.isp_sharpen_rly_wgt5 */
/*  input       : unsigned int uisp_sharpen_rly_wgt5: 8 bits */
static __inline td_void isp_sharpen_rly_wgt5_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt5)
{
    u_isp_sharpen_rly_wgt1 o_isp_sharpen_rly_wgt1;
    o_isp_sharpen_rly_wgt1.u32 = be_reg->isp_sharpen_rly_wgt1.u32;
    o_isp_sharpen_rly_wgt1.bits.isp_sharpen_rly_wgt5 = uisp_sharpen_rly_wgt5;
    be_reg->isp_sharpen_rly_wgt1.u32 = o_isp_sharpen_rly_wgt1.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt1.isp_sharpen_rly_wgt6 */
/*  input       : unsigned int uisp_sharpen_rly_wgt6: 8 bits */
static __inline td_void isp_sharpen_rly_wgt6_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt6)
{
    u_isp_sharpen_rly_wgt1 o_isp_sharpen_rly_wgt1;
    o_isp_sharpen_rly_wgt1.u32 = be_reg->isp_sharpen_rly_wgt1.u32;
    o_isp_sharpen_rly_wgt1.bits.isp_sharpen_rly_wgt6 = uisp_sharpen_rly_wgt6;
    be_reg->isp_sharpen_rly_wgt1.u32 = o_isp_sharpen_rly_wgt1.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt1.isp_sharpen_rly_wgt7 */
/*  input       : unsigned int uisp_sharpen_rly_wgt7: 8 bits */
static __inline td_void isp_sharpen_rly_wgt7_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt7)
{
    u_isp_sharpen_rly_wgt1 o_isp_sharpen_rly_wgt1;
    o_isp_sharpen_rly_wgt1.u32 = be_reg->isp_sharpen_rly_wgt1.u32;
    o_isp_sharpen_rly_wgt1.bits.isp_sharpen_rly_wgt7 = uisp_sharpen_rly_wgt7;
    be_reg->isp_sharpen_rly_wgt1.u32 = o_isp_sharpen_rly_wgt1.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt2.isp_sharpen_rly_wgt8 */
/*  input       : unsigned int uisp_sharpen_rly_wgt8: 8 bits */
static __inline td_void isp_sharpen_rly_wgt8_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt8)
{
    u_isp_sharpen_rly_wgt2 o_isp_sharpen_rly_wgt2;
    o_isp_sharpen_rly_wgt2.u32 = be_reg->isp_sharpen_rly_wgt2.u32;
    o_isp_sharpen_rly_wgt2.bits.isp_sharpen_rly_wgt8 = uisp_sharpen_rly_wgt8;
    be_reg->isp_sharpen_rly_wgt2.u32 = o_isp_sharpen_rly_wgt2.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt2.isp_sharpen_rly_wgt9 */
/*  input       : unsigned int uisp_sharpen_rly_wgt9: 8 bits */
static __inline td_void isp_sharpen_rly_wgt9_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt9)
{
    u_isp_sharpen_rly_wgt2 o_isp_sharpen_rly_wgt2;
    o_isp_sharpen_rly_wgt2.u32 = be_reg->isp_sharpen_rly_wgt2.u32;
    o_isp_sharpen_rly_wgt2.bits.isp_sharpen_rly_wgt9 = uisp_sharpen_rly_wgt9;
    be_reg->isp_sharpen_rly_wgt2.u32 = o_isp_sharpen_rly_wgt2.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt2.isp_sharpen_rly_wgt10 */
/*  input       : unsigned int uisp_sharpen_rly_wgt10: 8 bits */
static __inline td_void isp_sharpen_rly_wgt10_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt10)
{
    u_isp_sharpen_rly_wgt2 o_isp_sharpen_rly_wgt2;
    o_isp_sharpen_rly_wgt2.u32 = be_reg->isp_sharpen_rly_wgt2.u32;
    o_isp_sharpen_rly_wgt2.bits.isp_sharpen_rly_wgt10 = uisp_sharpen_rly_wgt10;
    be_reg->isp_sharpen_rly_wgt2.u32 = o_isp_sharpen_rly_wgt2.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt2.isp_sharpen_rly_wgt11 */
/*  input       : unsigned int uisp_sharpen_rly_wgt11: 8 bits */
static __inline td_void isp_sharpen_rly_wgt11_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt11)
{
    u_isp_sharpen_rly_wgt2 o_isp_sharpen_rly_wgt2;
    o_isp_sharpen_rly_wgt2.u32 = be_reg->isp_sharpen_rly_wgt2.u32;
    o_isp_sharpen_rly_wgt2.bits.isp_sharpen_rly_wgt11 = uisp_sharpen_rly_wgt11;
    be_reg->isp_sharpen_rly_wgt2.u32 = o_isp_sharpen_rly_wgt2.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt3.isp_sharpen_rly_wgt12 */
/*  input       : unsigned int uisp_sharpen_rly_wgt12: 8 bits */
static __inline td_void isp_sharpen_rly_wgt12_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt12)
{
    u_isp_sharpen_rly_wgt3 o_isp_sharpen_rly_wgt3;
    o_isp_sharpen_rly_wgt3.u32 = be_reg->isp_sharpen_rly_wgt3.u32;
    o_isp_sharpen_rly_wgt3.bits.isp_sharpen_rly_wgt12 = uisp_sharpen_rly_wgt12;
    be_reg->isp_sharpen_rly_wgt3.u32 = o_isp_sharpen_rly_wgt3.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt3.isp_sharpen_rly_wgt13 */
/*  input       : unsigned int uisp_sharpen_rly_wgt13: 8 bits */
static __inline td_void isp_sharpen_rly_wgt13_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt13)
{
    u_isp_sharpen_rly_wgt3 o_isp_sharpen_rly_wgt3;
    o_isp_sharpen_rly_wgt3.u32 = be_reg->isp_sharpen_rly_wgt3.u32;
    o_isp_sharpen_rly_wgt3.bits.isp_sharpen_rly_wgt13 = uisp_sharpen_rly_wgt13;
    be_reg->isp_sharpen_rly_wgt3.u32 = o_isp_sharpen_rly_wgt3.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt3.isp_sharpen_rly_wgt14 */
/*  input       : unsigned int uisp_sharpen_rly_wgt14: 8 bits */
static __inline td_void isp_sharpen_rly_wgt14_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt14)
{
    u_isp_sharpen_rly_wgt3 o_isp_sharpen_rly_wgt3;
    o_isp_sharpen_rly_wgt3.u32 = be_reg->isp_sharpen_rly_wgt3.u32;
    o_isp_sharpen_rly_wgt3.bits.isp_sharpen_rly_wgt14 = uisp_sharpen_rly_wgt14;
    be_reg->isp_sharpen_rly_wgt3.u32 = o_isp_sharpen_rly_wgt3.u32;
}

/*  description : set the value of the member isp_sharpen_rly_wgt3.isp_sharpen_rly_wgt15 */
/*  input       : unsigned int uisp_sharpen_rly_wgt15: 8 bits */
static __inline td_void isp_sharpen_rly_wgt15_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_rly_wgt15)
{
    u_isp_sharpen_rly_wgt3 o_isp_sharpen_rly_wgt3;
    o_isp_sharpen_rly_wgt3.u32 = be_reg->isp_sharpen_rly_wgt3.u32;
    o_isp_sharpen_rly_wgt3.bits.isp_sharpen_rly_wgt15 = uisp_sharpen_rly_wgt15;
    be_reg->isp_sharpen_rly_wgt3.u32 = o_isp_sharpen_rly_wgt3.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_mot_ctrl_en */
/*  input       : unsigned int uisp_sharpen_mot_ctrl_en: 1 bits */
static __inline td_void isp_sharpen_mot_ctrl_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mot_ctrl_en)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_mot_ctrl_en = uisp_sharpen_mot_ctrl_en;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_mot_interp_mode */
/*  input       : unsigned int uisp_sharpen_mot_interp_mode: 1 bits */
static __inline td_void isp_sharpen_mot_interp_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mot_interp_mode)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_mot_interp_mode = uisp_sharpen_mot_interp_mode;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_std_adj_bymot_en */
/*  input       : unsigned int uisp_sharpen_std_adj_bymot_en: 1 bits */
static __inline td_void isp_sharpen_std_adj_bymot_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_adj_bymot_en)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_std_adj_bymot_en = uisp_sharpen_std_adj_bymot_en;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_std_adj_byy_en */
/*  input       : unsigned int uisp_sharpen_std_adj_byy_en: 1 bits */
static __inline td_void isp_sharpen_std_adj_byy_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_adj_byy_en)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_std_adj_byy_en = uisp_sharpen_std_adj_byy_en;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_std_comb_mode */
/*  input       : unsigned int uisp_sharpen_std_comb_mode: 2 bits */
static __inline td_void isp_sharpen_std_comb_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_comb_mode)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_std_comb_mode = uisp_sharpen_std_comb_mode;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_std_comb_alpha */
/*  input       : unsigned int uisp_sharpen_std_comb_alpha: 5 bits */
static __inline td_void isp_sharpen_std_comb_alpha_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_std_comb_alpha)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_std_comb_alpha = uisp_sharpen_std_comb_alpha;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_mot_thd */
/*  input       : unsigned int uisp_sharpen_mot_thd: 4 bits */
static __inline td_void isp_sharpen_mot_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mot_thd)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_mot_thd = uisp_sharpen_mot_thd;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

/*  description : set the value of the member isp_sharpen_mot_cfg.isp_sharpen_mot_en */
/*  input       : unsigned int uisp_sharpen_mot_en: 1 bits */
static __inline td_void isp_sharpen_mot_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_mot_en)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_mot_en = uisp_sharpen_mot_en;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}

static __inline td_void isp_sharpen_var7x9_calc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_var7x9_calc_en)
{
    u_isp_sharpen_mot_cfg o_isp_sharpen_mot_cfg;
    o_isp_sharpen_mot_cfg.u32 = be_reg->isp_sharpen_mot_cfg.u32;
    o_isp_sharpen_mot_cfg.bits.isp_sharpen_var7x9_calc_en = uisp_sharpen_var7x9_calc_en;
    be_reg->isp_sharpen_mot_cfg.u32 = o_isp_sharpen_mot_cfg.u32;
}
/*  description : set the value of the member isp_sharpen_var5_thd.isp_sharpen_var5_low_thd */
/*  input       : unsigned int uisp_sharpen_var5_low_thd: 10 bits */
static __inline td_void isp_sharpen_var5_low_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_var5_low_thd)
{
    u_isp_sharpen_var5_thd o_isp_sharpen_var5_thd;
    o_isp_sharpen_var5_thd.u32 = be_reg->isp_sharpen_var5_thd.u32;
    o_isp_sharpen_var5_thd.bits.isp_sharpen_var5_low_thd = uisp_sharpen_var5_low_thd;
    be_reg->isp_sharpen_var5_thd.u32 = o_isp_sharpen_var5_thd.u32;
}

/*  description : set the value of the member isp_sharpen_var5_thd.isp_sharpen_var5_mid_thd */
/*  input       : unsigned int uisp_sharpen_var5_mid_thd: 10 bits */
static __inline td_void isp_sharpen_var5_mid_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_var5_mid_thd)
{
    u_isp_sharpen_var5_thd o_isp_sharpen_var5_thd;
    o_isp_sharpen_var5_thd.u32 = be_reg->isp_sharpen_var5_thd.u32;
    o_isp_sharpen_var5_thd.bits.isp_sharpen_var5_mid_thd = uisp_sharpen_var5_mid_thd;
    be_reg->isp_sharpen_var5_thd.u32 = o_isp_sharpen_var5_thd.u32;
}

/*  description : set the value of the member isp_sharpen_var5_thd.isp_sharpen_var5_high_thd */
/*  input       : unsigned int uisp_sharpen_var5_high_thd: 10 bits */
static __inline td_void isp_sharpen_var5_high_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_var5_high_thd)
{
    u_isp_sharpen_var5_thd o_isp_sharpen_var5_thd;
    o_isp_sharpen_var5_thd.u32 = be_reg->isp_sharpen_var5_thd.u32;
    o_isp_sharpen_var5_thd.bits.isp_sharpen_var5_high_thd = uisp_sharpen_var5_high_thd;
    be_reg->isp_sharpen_var5_thd.u32 = o_isp_sharpen_var5_thd.u32;
}

/*  description : set the value of the member isp_sharpen_var7_thd.isp_sharpen_var7_low_thd */
/*  input       : unsigned int uisp_sharpen_var7_low_thd: 10 bits */
static __inline td_void isp_sharpen_var7_low_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_var7_low_thd)
{
    u_isp_sharpen_var7_thd o_isp_sharpen_var7_thd;
    o_isp_sharpen_var7_thd.u32 = be_reg->isp_sharpen_var7_thd.u32;
    o_isp_sharpen_var7_thd.bits.isp_sharpen_var7_low_thd = uisp_sharpen_var7_low_thd;
    be_reg->isp_sharpen_var7_thd.u32 = o_isp_sharpen_var7_thd.u32;
}

/*  description : set the value of the member isp_sharpen_var7_thd.isp_sharpen_var7_high_thd */
/*  input       : unsigned int uisp_sharpen_var7_high_thd: 10 bits */
static __inline td_void isp_sharpen_var7_high_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_var7_high_thd)
{
    u_isp_sharpen_var7_thd o_isp_sharpen_var7_thd;
    o_isp_sharpen_var7_thd.u32 = be_reg->isp_sharpen_var7_thd.u32;
    o_isp_sharpen_var7_thd.bits.isp_sharpen_var7_high_thd = uisp_sharpen_var7_high_thd;
    be_reg->isp_sharpen_var7_thd.u32 = o_isp_sharpen_var7_thd.u32;
}

/*  description : set the value of the member isp_sharpen_shoot_edge_amt.isp_sharpen_edge_usht_amt */
/*  input       : unsigned int uisp_sharpen_edge_usht_amt: 7 bits */
static __inline td_void isp_sharpen_edge_usht_amt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_edge_usht_amt)
{
    u_isp_sharpen_shoot_edge_amt o_isp_sharpen_shoot_edge_amt;
    o_isp_sharpen_shoot_edge_amt.u32 = be_reg->isp_sharpen_shoot_edge_amt.u32;
    o_isp_sharpen_shoot_edge_amt.bits.isp_sharpen_edge_usht_amt = uisp_sharpen_edge_usht_amt;
    be_reg->isp_sharpen_shoot_edge_amt.u32 = o_isp_sharpen_shoot_edge_amt.u32;
}

/*  description : set the value of the member isp_sharpen_shoot_edge_amt.isp_sharpen_edge_osht_amt */
/*  input       : unsigned int uisp_sharpen_edge_osht_amt: 7 bits */
static __inline td_void isp_sharpen_edge_osht_amt_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_edge_osht_amt)
{
    u_isp_sharpen_shoot_edge_amt o_isp_sharpen_shoot_edge_amt;
    o_isp_sharpen_shoot_edge_amt.u32 = be_reg->isp_sharpen_shoot_edge_amt.u32;
    o_isp_sharpen_shoot_edge_amt.bits.isp_sharpen_edge_osht_amt = uisp_sharpen_edge_osht_amt;
    be_reg->isp_sharpen_shoot_edge_amt.u32 = o_isp_sharpen_shoot_edge_amt.u32;
}


/*  description   set the value of the member isp_sharpen_ldci_dither_cfg.isp_sharpen_ldci_dither_round */
/*  input         unsigned int uisp_sharpen_ldci_dither_round  2 bits */
#define ISP_SHARPEN_LDCI_DITHER_ROUND_DEFAULT    2
static __inline td_void isp_sharpen_ldci_dither_round_write(isp_be_reg_type *be_reg, td_u32 sharpen_ldci_dither_round)
{
    u_isp_sharpen_ldci_dither_cfg o_isp_sharpen_ldci_dither_cfg;
    o_isp_sharpen_ldci_dither_cfg.u32 = be_reg->isp_sharpen_ldci_dither_cfg.u32;
    o_isp_sharpen_ldci_dither_cfg.bits.isp_sharpen_ldci_dither_round = sharpen_ldci_dither_round;
    be_reg->isp_sharpen_ldci_dither_cfg.u32 = o_isp_sharpen_ldci_dither_cfg.u32;
}

/*  description : set the value of the member isp_bnr_cfg.isp_bnr_ensptnr */
/*  input       : unsigned int uisp_bnr_ensptnr: 1 bits */
static __inline td_void isp_bnr_ensptnr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_ensptnr)
{
    u_isp_bnr_cfg o_isp_bnr_cfg;
    o_isp_bnr_cfg.u32 = be_reg->isp_bnr_cfg.u32;
    o_isp_bnr_cfg.bits.isp_bnr_ensptnr = uisp_bnr_ensptnr;
    be_reg->isp_bnr_cfg.u32 = o_isp_bnr_cfg.u32;
}

/*  description : set the value of the member isp_bnr_cfg.isp_bnr_entmpnr */
/*  input       : unsigned int uisp_bnr_entmpnr: 1 bits */
static __inline td_void isp_bnr_entmpnr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_entmpnr)
{
    u_isp_bnr_cfg o_isp_bnr_cfg;
    o_isp_bnr_cfg.u32 = be_reg->isp_bnr_cfg.u32;
    o_isp_bnr_cfg.bits.isp_bnr_entmpnr = uisp_bnr_entmpnr;
    be_reg->isp_bnr_cfg.u32 = o_isp_bnr_cfg.u32;
}

/*  description : set the value of the member isp_bnr_cfg.isp_bnr_isinitial */
/*  input       : unsigned int uisp_bnr_isinitial: 1 bits */
static __inline td_void isp_bnr_isinitial_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_isinitial)
{
    u_isp_bnr_cfg o_isp_bnr_cfg;
    o_isp_bnr_cfg.u32 = be_reg->isp_bnr_cfg.u32;
    o_isp_bnr_cfg.bits.isp_bnr_isinitial = uisp_bnr_isinitial;
    be_reg->isp_bnr_cfg.u32 = o_isp_bnr_cfg.u32;
}

/*  description : set the value of the member isp_bnr_cfg.isp_bnr_frm_phase */
/*  input       : unsigned int uisp_bnr_frm_phase: 2 bits */
static __inline td_void isp_bnr_frm_phase_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_frm_phase)
{
    u_isp_bnr_cfg o_isp_bnr_cfg;
    o_isp_bnr_cfg.u32 = be_reg->isp_bnr_cfg.u32;
    o_isp_bnr_cfg.bits.isp_bnr_frm_phase = uisp_bnr_frm_phase;
    be_reg->isp_bnr_cfg.u32 = o_isp_bnr_cfg.u32;
}

/*  description : set the value of the member isp_bnr_cfg.isp_bnr2d_en */
/*  input       : unsigned int uisp_bnr2d_en: 1 bits */
static __inline td_void isp_bnr2d_en_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr2d_en)
{
    u_isp_bnr_cfg o_isp_bnr_cfg;
    o_isp_bnr_cfg.u32 = be_reg->isp_bnr_cfg.u32;
    o_isp_bnr_cfg.bits.isp_bnr2d_en = uisp_bnr2d_en;
    be_reg->isp_bnr_cfg.u32 = o_isp_bnr_cfg.u32;
}
static __inline td_void isp_bnr_freqreconst_en_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_freqreconst_en)
{
    u_isp_bnr_cfg o_isp_bnr_cfg;
    o_isp_bnr_cfg.u32 = be_reg->isp_bnr_cfg.u32;
    o_isp_bnr_cfg.bits.isp_bnr_freqreconst_en = uisp_bnr_freqreconst_en;
    be_reg->isp_bnr_cfg.u32 = o_isp_bnr_cfg.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_win_size */
/*  input       : unsigned int uisp_bnr_bltev500_win_size: 2 bits */
static __inline td_void isp_bnr_bltev500_win_size_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_win_size)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_win_size = uisp_bnr_bltev500_win_size;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_n_limit_gain_sad_d */
/*  input       : unsigned int uisp_bnr_bltev500_n_limit_gain_sad_d: 8 bits */
static __inline td_void isp_bnr_bltev500_n_limit_gain_sad_d_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_n_limit_gain_sad_d)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_n_limit_gain_sad_d = uisp_bnr_bltev500_n_limit_gain_sad_d;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_pnr */
/*  input       : unsigned int uisp_bnr_bltev500_pnr: 2 bits */
static __inline td_void isp_bnr_bltev500_pnr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_pnr)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_pnr = uisp_bnr_bltev500_pnr;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_en_imp_nr */
/*  input       : unsigned int uisp_bnr_bltev500_en_imp_nr: 1 bits */
static __inline td_void isp_bnr_bltev500_en_imp_nr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_en_imp_nr)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_en_imp_nr = uisp_bnr_bltev500_en_imp_nr;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_imp_nr_str */
/*  input       : unsigned int uisp_bnr_bltev500_imp_nr_str: 6 bits */
static __inline td_void isp_bnr_bltev500_imp_nr_str_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_imp_nr_str)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_imp_nr_str = uisp_bnr_bltev500_imp_nr_str;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_wt_ccoef */
/*  input       : unsigned int uisp_bnr_bltev500_wt_ccoef: 2 bits */
static __inline td_void isp_bnr_bltev500_wt_ccoef_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_wt_ccoef)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_wt_ccoef = uisp_bnr_bltev500_wt_ccoef;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_bltev500_wt_cmax */
/*  input       : unsigned int uisp_bnr_bltev500_wt_cmax: 5 bits */
static __inline td_void isp_bnr_bltev500_wt_cmax_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bltev500_wt_cmax)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_bltev500_wt_cmax = uisp_bnr_bltev500_wt_cmax;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_sad_bit_reduction */
/*  input       : unsigned int uisp_bnr_sad_bit_reduction: 3 bits */
static __inline td_void isp_bnr_sad_bit_reduction_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_sad_bit_reduction)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_sad_bit_reduction = uisp_bnr_sad_bit_reduction;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_blt_enable_symm_sad */
/*  input       : unsigned int uisp_bnr_blt_enable_symm_sad: 1 bits */
static __inline td_void isp_bnr_blt_enable_symm_sad_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_blt_enable_symm_sad)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_blt_enable_symm_sad = uisp_bnr_blt_enable_symm_sad;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg0.isp_bnr_jnlm_lum_sel */
/*  input       : unsigned int uisp_bnr_jnlm_lum_sel: 2 bits */
static __inline td_void isp_bnr_jnlm_lum_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lum_sel)
{
    u_isp_bnr_cfg0 o_isp_bnr_cfg0;
    o_isp_bnr_cfg0.u32 = be_reg->isp_bnr_cfg0.u32;
    o_isp_bnr_cfg0.bits.isp_bnr_jnlm_lum_sel = uisp_bnr_jnlm_lum_sel;
    be_reg->isp_bnr_cfg0.u32 = o_isp_bnr_cfg0.u32;
}

/*  description : set the value of the member isp_bnr_cfg1.isp_bnr_jnlm_lmt_gain0 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain0: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain0_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain0)
{
    u_isp_bnr_cfg1 o_isp_bnr_cfg1;
    o_isp_bnr_cfg1.u32 = be_reg->isp_bnr_cfg1.u32;
    o_isp_bnr_cfg1.bits.isp_bnr_jnlm_lmt_gain0 = uisp_bnr_jnlm_lmt_gain0;
    be_reg->isp_bnr_cfg1.u32 = o_isp_bnr_cfg1.u32;
}

/*  description : set the value of the member isp_bnr_cfg2.isp_bnr_jnlm_lmt_gain1 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain1: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain1_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain1)
{
    u_isp_bnr_cfg2 o_isp_bnr_cfg2;
    o_isp_bnr_cfg2.u32 = be_reg->isp_bnr_cfg2.u32;
    o_isp_bnr_cfg2.bits.isp_bnr_jnlm_lmt_gain1 = uisp_bnr_jnlm_lmt_gain1;
    be_reg->isp_bnr_cfg2.u32 = o_isp_bnr_cfg2.u32;
}

/*  description : set the value of the member isp_bnr_cfg3.isp_bnr_jnlm_lmt_gain2 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain2: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain2_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain2)
{
    u_isp_bnr_cfg3 o_isp_bnr_cfg3;
    o_isp_bnr_cfg3.u32 = be_reg->isp_bnr_cfg3.u32;
    o_isp_bnr_cfg3.bits.isp_bnr_jnlm_lmt_gain2 = uisp_bnr_jnlm_lmt_gain2;
    be_reg->isp_bnr_cfg3.u32 = o_isp_bnr_cfg3.u32;
}

/*  description : set the value of the member isp_bnr_cfg4.isp_bnr_jnlm_lmt_gain3 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain3: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain3_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain3)
{
    u_isp_bnr_cfg4 o_isp_bnr_cfg4;
    o_isp_bnr_cfg4.u32 = be_reg->isp_bnr_cfg4.u32;
    o_isp_bnr_cfg4.bits.isp_bnr_jnlm_lmt_gain3 = uisp_bnr_jnlm_lmt_gain3;
    be_reg->isp_bnr_cfg4.u32 = o_isp_bnr_cfg4.u32;
}

/*  description : set the value of the member isp_bnr_cfg5.isp_bnr_jnlm_lmt_gain4 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain4: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain4_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain4)
{
    u_isp_bnr_cfg5 o_isp_bnr_cfg5;
    o_isp_bnr_cfg5.u32 = be_reg->isp_bnr_cfg5.u32;
    o_isp_bnr_cfg5.bits.isp_bnr_jnlm_lmt_gain4 = uisp_bnr_jnlm_lmt_gain4;
    be_reg->isp_bnr_cfg5.u32 = o_isp_bnr_cfg5.u32;
}

/*  description : set the value of the member isp_bnr_cfg6.isp_bnr_jnlm_lmt_gain5 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain5: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain5_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain5)
{
    u_isp_bnr_cfg6 o_isp_bnr_cfg6;
    o_isp_bnr_cfg6.u32 = be_reg->isp_bnr_cfg6.u32;
    o_isp_bnr_cfg6.bits.isp_bnr_jnlm_lmt_gain5 = uisp_bnr_jnlm_lmt_gain5;
    be_reg->isp_bnr_cfg6.u32 = o_isp_bnr_cfg6.u32;
}

/*  description : set the value of the member isp_bnr_cfg7.isp_bnr_jnlm_lmt_gain6 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain6: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain6_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain6)
{
    u_isp_bnr_cfg7 o_isp_bnr_cfg7;
    o_isp_bnr_cfg7.u32 = be_reg->isp_bnr_cfg7.u32;
    o_isp_bnr_cfg7.bits.isp_bnr_jnlm_lmt_gain6 = uisp_bnr_jnlm_lmt_gain6;
    be_reg->isp_bnr_cfg7.u32 = o_isp_bnr_cfg7.u32;
}

/*  description : set the value of the member isp_bnr_cfg8.isp_bnr_jnlm_lmt_gain7 */
/*  input       : unsigned int uisp_bnr_jnlm_lmt_gain7: 17 bits */
static __inline td_void isp_bnr_jnlm_lmt_gain7_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_lmt_gain7)
{
    u_isp_bnr_cfg8 o_isp_bnr_cfg8;
    o_isp_bnr_cfg8.u32 = be_reg->isp_bnr_cfg8.u32;
    o_isp_bnr_cfg8.bits.isp_bnr_jnlm_lmt_gain7 = uisp_bnr_jnlm_lmt_gain7;
    be_reg->isp_bnr_cfg8.u32 = o_isp_bnr_cfg8.u32;
}

/*  description : set the value of the member isp_bnr_cfg8.isp_bnr_en_area_alt */
/*  input       : unsigned int uisp_bnr_en_area_alt: 1 bits */
static __inline td_void isp_bnr_en_area_alt_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_en_area_alt)
{
    u_isp_bnr_cfg8 o_isp_bnr_cfg8;
    o_isp_bnr_cfg8.u32 = be_reg->isp_bnr_cfg8.u32;
    o_isp_bnr_cfg8.bits.isp_bnr_en_area_alt = uisp_bnr_en_area_alt;
    be_reg->isp_bnr_cfg8.u32 = o_isp_bnr_cfg8.u32;
}

/*  description : set the value of the member isp_bnr_cfg8.isp_bnr_mdet_size */
/*  input       : unsigned int uisp_bnr_mdet_size: 3 bits */
static __inline td_void isp_bnr_mdet_size_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mdet_size)
{
    u_isp_bnr_cfg8 o_isp_bnr_cfg8;
    o_isp_bnr_cfg8.u32 = be_reg->isp_bnr_cfg8.u32;
    o_isp_bnr_cfg8.bits.isp_bnr_mdet_size = uisp_bnr_mdet_size;
    be_reg->isp_bnr_cfg8.u32 = o_isp_bnr_cfg8.u32;
}

/*  description : set the value of the member isp_bnr_cfg8.isp_bnr_mdet_cor_level */
/*  input       : unsigned int uisp_bnr_mdet_cor_level: 8 bits */
static __inline td_void isp_bnr_mdet_cor_level_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mdet_cor_level)
{
    u_isp_bnr_cfg8 o_isp_bnr_cfg8;
    o_isp_bnr_cfg8.u32 = be_reg->isp_bnr_cfg8.u32;
    o_isp_bnr_cfg8.bits.isp_bnr_mdet_cor_level = uisp_bnr_mdet_cor_level;
    be_reg->isp_bnr_cfg8.u32 = o_isp_bnr_cfg8.u32;
}

/*  description : set the value of the member isp_bnr_cfg8.isp_bnr_b_deform_md_wim */
/*  input       : unsigned int uisp_bnr_b_deform_md_wim: 1 bits */
static __inline td_void isp_bnr_b_deform_md_wim_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_b_deform_md_wim)
{
    u_isp_bnr_cfg8 o_isp_bnr_cfg8;
    o_isp_bnr_cfg8.u32 = be_reg->isp_bnr_cfg8.u32;
    o_isp_bnr_cfg8.bits.isp_bnr_b_deform_md_wim = uisp_bnr_b_deform_md_wim;
    be_reg->isp_bnr_cfg8.u32 = o_isp_bnr_cfg8.u32;
}

/*  description : set the value of the member isp_bnr_cfg9.isp_bnr_rnt_th */
/*  input       : unsigned int uisp_bnr_rnt_th: 8 bits */
static __inline td_void isp_bnr_rnt_th_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_rnt_th)
{
    u_isp_bnr_cfg9 o_isp_bnr_cfg9;
    o_isp_bnr_cfg9.u32 = be_reg->isp_bnr_cfg9.u32;
    o_isp_bnr_cfg9.bits.isp_bnr_rnt_th = uisp_bnr_rnt_th;
    be_reg->isp_bnr_cfg9.u32 = o_isp_bnr_cfg9.u32;
}

/*  description : set the value of the member isp_bnr_cfg9.isp_bnr_rnt_wid */
/*  input       : unsigned int uisp_bnr_rnt_wid: 8 bits */
static __inline td_void isp_bnr_rnt_wid_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_rnt_wid)
{
    u_isp_bnr_cfg9 o_isp_bnr_cfg9;
    o_isp_bnr_cfg9.u32 = be_reg->isp_bnr_cfg9.u32;
    o_isp_bnr_cfg9.bits.isp_bnr_rnt_wid = uisp_bnr_rnt_wid;
    be_reg->isp_bnr_cfg9.u32 = o_isp_bnr_cfg9.u32;
}

/*  description : set the value of the member isp_bnr_cfg9.isp_bnr_nr_strength_st_int */
/*  input       : unsigned int uisp_bnr_nr_strength_st_int: 9 bits */
static __inline td_void isp_bnr_nr_strength_st_int_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_nr_strength_st_int)
{
    u_isp_bnr_cfg9 o_isp_bnr_cfg9;
    o_isp_bnr_cfg9.u32 = be_reg->isp_bnr_cfg9.u32;
    o_isp_bnr_cfg9.bits.isp_bnr_nr_strength_st_int = uisp_bnr_nr_strength_st_int;
    be_reg->isp_bnr_cfg9.u32 = o_isp_bnr_cfg9.u32;
}

/*  description : set the value of the member isp_bnr_cfg9.isp_bnr_inputraw_ratio */
/*  input       : unsigned int uisp_bnr_inputraw_ratio: 7 bits */
static __inline td_void isp_bnr_inputraw_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_inputraw_ratio)
{
    u_isp_bnr_cfg9 o_isp_bnr_cfg9;
    o_isp_bnr_cfg9.u32 = be_reg->isp_bnr_cfg9.u32;
    o_isp_bnr_cfg9.bits.isp_bnr_inputraw_ratio = uisp_bnr_inputraw_ratio;
    be_reg->isp_bnr_cfg9.u32 = o_isp_bnr_cfg9.u32;
}

/*  description : set the value of the member isp_bnr_cfg10.isp_bnr_nr_strength_mv_int */
/*  input       : unsigned int uisp_bnr_nr_strength_mv_int: 9 bits */
static __inline td_void isp_bnr_nr_strength_mv_int_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_nr_strength_mv_int)
{
    u_isp_bnr_cfg10 o_isp_bnr_cfg10;
    o_isp_bnr_cfg10.u32 = be_reg->isp_bnr_cfg10.u32;
    o_isp_bnr_cfg10.bits.isp_bnr_nr_strength_mv_int = uisp_bnr_nr_strength_mv_int;
    be_reg->isp_bnr_cfg10.u32 = o_isp_bnr_cfg10.u32;
}

/*  description : set the value of the member isp_bnr_cfg10.isp_bnr_nr_strength_slope */
/*  input       : unsigned int uisp_bnr_nr_strength_slope: 10 bits */
static __inline td_void isp_bnr_nr_strength_slope_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_nr_strength_slope)
{
    u_isp_bnr_cfg10 o_isp_bnr_cfg10;
    o_isp_bnr_cfg10.u32 = be_reg->isp_bnr_cfg10.u32;
    o_isp_bnr_cfg10.bits.isp_bnr_nr_strength_slope = uisp_bnr_nr_strength_slope;
    be_reg->isp_bnr_cfg10.u32 = o_isp_bnr_cfg10.u32;
}

/*  description : set the value of the member isp_bnr_cfg10.isp_bnr_en_mixing */
/*  input       : unsigned int uisp_bnr_en_mixing: 2 bits */
static __inline td_void isp_bnr_en_mixing_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_en_mixing)
{
    u_isp_bnr_cfg10 o_isp_bnr_cfg10;
    o_isp_bnr_cfg10.u32 = be_reg->isp_bnr_cfg10.u32;
    o_isp_bnr_cfg10.bits.isp_bnr_en_mixing = uisp_bnr_en_mixing;
    be_reg->isp_bnr_cfg10.u32 = o_isp_bnr_cfg10.u32;
}

/*  description : set the value of the member isp_bnr_cfg10.isp_bnr_target_noise_ratio */
/*  input       : unsigned int uisp_bnr_target_noise_ratio: 8 bits */
static __inline td_void isp_bnr_target_noise_ratio_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_target_noise_ratio)
{
    u_isp_bnr_cfg10 o_isp_bnr_cfg10;
    o_isp_bnr_cfg10.u32 = be_reg->isp_bnr_cfg10.u32;
    o_isp_bnr_cfg10.bits.isp_bnr_target_noise_ratio = uisp_bnr_target_noise_ratio;
    be_reg->isp_bnr_cfg10.u32 = o_isp_bnr_cfg10.u32;
}

/*  description : set the value of the member isp_bnr_cfg11.isp_bnr_mix_gain_0_r */
/*  input       : unsigned int uisp_bnr_mix_gain_0_r: 8 bits */
static __inline td_void isp_bnr_mix_gain_0_r_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mix_gain_0_r)
{
    u_isp_bnr_cfg11 o_isp_bnr_cfg11;
    o_isp_bnr_cfg11.u32 = be_reg->isp_bnr_cfg11.u32;
    o_isp_bnr_cfg11.bits.isp_bnr_mix_gain_0_r = uisp_bnr_mix_gain_0_r;
    be_reg->isp_bnr_cfg11.u32 = o_isp_bnr_cfg11.u32;
}

/*  description : set the value of the member isp_bnr_cfg11.isp_bnr_mix_gain_0_b */
/*  input       : unsigned int uisp_bnr_mix_gain_0_b: 8 bits */
static __inline td_void isp_bnr_mix_gain_0_b_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mix_gain_0_b)
{
    u_isp_bnr_cfg11 o_isp_bnr_cfg11;
    o_isp_bnr_cfg11.u32 = be_reg->isp_bnr_cfg11.u32;
    o_isp_bnr_cfg11.bits.isp_bnr_mix_gain_0_b = uisp_bnr_mix_gain_0_b;
    be_reg->isp_bnr_cfg11.u32 = o_isp_bnr_cfg11.u32;
}

/*  description : set the value of the member isp_bnr_cfg11.isp_bnr_wb_gain_r */
/*  input       : unsigned int uisp_bnr_wb_gain_r: 10 bits */
static __inline td_void isp_bnr_wb_gain_r_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wb_gain_r)
{
    u_isp_bnr_cfg11 o_isp_bnr_cfg11;
    o_isp_bnr_cfg11.u32 = be_reg->isp_bnr_cfg11.u32;
    o_isp_bnr_cfg11.bits.isp_bnr_wb_gain_r = uisp_bnr_wb_gain_r;
    be_reg->isp_bnr_cfg11.u32 = o_isp_bnr_cfg11.u32;
}

/*  description : set the value of the member isp_bnr_cfg11.isp_bnr_rb_diff_en */
/*  input       : unsigned int uisp_bnr_rb_diff_en: 1 bits */
static __inline td_void isp_bnr_rb_diff_en_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_rb_diff_en)
{
    u_isp_bnr_cfg11 o_isp_bnr_cfg11;
    o_isp_bnr_cfg11.u32 = be_reg->isp_bnr_cfg11.u32;
    o_isp_bnr_cfg11.bits.isp_bnr_rb_diff_en = uisp_bnr_rb_diff_en;
    be_reg->isp_bnr_cfg11.u32 = o_isp_bnr_cfg11.u32;
}

/*  description : set the value of the member isp_bnr_cfg12.isp_bnr_wb_gain_b */
/*  input       : unsigned int uisp_bnr_wb_gain_b: 10 bits */
static __inline td_void isp_bnr_wb_gain_b_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wb_gain_b)
{
    u_isp_bnr_cfg12 o_isp_bnr_cfg12;
    o_isp_bnr_cfg12.u32 = be_reg->isp_bnr_cfg12.u32;
    o_isp_bnr_cfg12.bits.isp_bnr_wb_gain_b = uisp_bnr_wb_gain_b;
    be_reg->isp_bnr_cfg12.u32 = o_isp_bnr_cfg12.u32;
}

/*  description : set the value of the member isp_bnr_cfg12.isp_bnr_wb_gain_inv_r */
/*  input       : unsigned int uisp_bnr_wb_gain_inv_r: 16 bits */
static __inline td_void isp_bnr_wb_gain_inv_r_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wb_gain_inv_r)
{
    u_isp_bnr_cfg12 o_isp_bnr_cfg12;
    o_isp_bnr_cfg12.u32 = be_reg->isp_bnr_cfg12.u32;
    o_isp_bnr_cfg12.bits.isp_bnr_wb_gain_inv_r = uisp_bnr_wb_gain_inv_r;
    be_reg->isp_bnr_cfg12.u32 = o_isp_bnr_cfg12.u32;
}

/*  description : set the value of the member isp_bnr_cfg13.isp_bnr_wb_gain_inv_b */
/*  input       : unsigned int uisp_bnr_wb_gain_inv_b: 16 bits */
static __inline td_void isp_bnr_wb_gain_inv_b_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wb_gain_inv_b)
{
    u_isp_bnr_cfg13 o_isp_bnr_cfg13;
    o_isp_bnr_cfg13.u32 = be_reg->isp_bnr_cfg13.u32;
    o_isp_bnr_cfg13.bits.isp_bnr_wb_gain_inv_b = uisp_bnr_wb_gain_inv_b;
    be_reg->isp_bnr_cfg13.u32 = o_isp_bnr_cfg13.u32;
}

/*  description : set the value of the member isp_bnr_cfg13.isp_bnr_bnrlscen */
/*  input       : unsigned int uisp_bnr_bnrlscen: 1 bits */
static __inline td_void isp_bnr_bnrlscen_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bnrlscen)
{
    u_isp_bnr_cfg13 o_isp_bnr_cfg13;
    o_isp_bnr_cfg13.u32 = be_reg->isp_bnr_cfg13.u32;
    o_isp_bnr_cfg13.bits.isp_bnr_bnrlscen = uisp_bnr_bnrlscen;
    be_reg->isp_bnr_cfg13.u32 = o_isp_bnr_cfg13.u32;
}

/*  description : set the value of the member isp_bnr_cfg13.isp_bnr_bnrlscratio */
/*  input       : unsigned int uisp_bnr_bnrlscratio: 4 bits */
static __inline td_void isp_bnr_bnrlscratio_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_bnrlscratio)
{
    u_isp_bnr_cfg13 o_isp_bnr_cfg13;
    o_isp_bnr_cfg13.u32 = be_reg->isp_bnr_cfg13.u32;
    o_isp_bnr_cfg13.bits.isp_bnr_bnrlscratio = uisp_bnr_bnrlscratio;
    be_reg->isp_bnr_cfg13.u32 = o_isp_bnr_cfg13.u32;
}

/*  description : set the value of the member isp_bnr_cfg13.isp_bnr_alphamax_st */
/*  input       : unsigned int uisp_bnr_alphamax_st: 8 bits */
static __inline td_void isp_bnr_alphamax_st_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_alphamax_st)
{
    u_isp_bnr_cfg13 o_isp_bnr_cfg13;
    o_isp_bnr_cfg13.u32 = be_reg->isp_bnr_cfg13.u32;
    o_isp_bnr_cfg13.bits.isp_bnr_alphamax_st = uisp_bnr_alphamax_st;
    be_reg->isp_bnr_cfg13.u32 = o_isp_bnr_cfg13.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_wdr_enable */
/*  input       : unsigned int uisp_bnr_wdr_enable: 2 bits */
static __inline td_void isp_bnr_wdr_enable_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wdr_enable)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_wdr_enable = uisp_bnr_wdr_enable;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_wdr_en_fusion */
/*  input       : unsigned int uisp_bnr_wdr_en_fusion: 2 bits */
static __inline td_void isp_bnr_wdr_en_fusion_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wdr_en_fusion)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_wdr_en_fusion = uisp_bnr_wdr_en_fusion;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_wdr_map_flt_mod */
/*  input       : unsigned int uisp_bnr_wdr_map_flt_mod: 1 bits */
static __inline td_void isp_bnr_wdr_map_flt_mod_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wdr_map_flt_mod)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_wdr_map_flt_mod = uisp_bnr_wdr_map_flt_mod;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_wdr_map_gain */
/*  input       : unsigned int uisp_bnr_wdr_map_gain: 5 bits */
static __inline td_void isp_bnr_wdr_map_gain_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_wdr_map_gain)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_wdr_map_gain = uisp_bnr_wdr_map_gain;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_en_2nd_tmp_nr */
/*  input       : unsigned int uisp_bnr_en_2nd_tmp_nr: 1 bits */
static __inline td_void isp_bnr_en_2nd_tmp_nr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_en_2nd_tmp_nr)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_en_2nd_tmp_nr = uisp_bnr_en_2nd_tmp_nr;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_ynet_alpha */
/*  input       : unsigned int uisp_bnr_ynet_alpha: 8 bits */
static __inline td_void isp_bnr_ynet_alpha_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_ynet_alpha)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_ynet_alpha = uisp_bnr_ynet_alpha;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg14.isp_bnr_mdet_mixratio */
/*  input       : unsigned int uisp_bnr_mdet_mixratio: 7 bits */
static __inline td_void isp_bnr_mdet_mixratio_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mdet_mixratio)
{
    u_isp_bnr_cfg14 o_isp_bnr_cfg14;
    o_isp_bnr_cfg14.u32 = be_reg->isp_bnr_cfg14.u32;
    o_isp_bnr_cfg14.bits.isp_bnr_mdet_mixratio = uisp_bnr_mdet_mixratio;
    be_reg->isp_bnr_cfg14.u32 = o_isp_bnr_cfg14.u32;
}

/*  description : set the value of the member isp_bnr_cfg15.isp_bnr_coring_hig */
/*  input       : unsigned int uisp_bnr_coring_hig: 16 bits */
static __inline td_void isp_bnr_coring_hig_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_hig)
{
    u_isp_bnr_cfg15 o_isp_bnr_cfg15;
    o_isp_bnr_cfg15.u32 = be_reg->isp_bnr_cfg15.u32;
    o_isp_bnr_cfg15.bits.isp_bnr_coring_hig = uisp_bnr_coring_hig;
    be_reg->isp_bnr_cfg15.u32 = o_isp_bnr_cfg15.u32;
}

/*  description : set the value of the member isp_bnr_cfg15.isp_bnr_coring_enable */
/*  input       : unsigned int uisp_bnr_coring_enable: 1 bits */
static __inline td_void isp_bnr_coring_enable_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_enable)
{
    u_isp_bnr_cfg15 o_isp_bnr_cfg15;
    o_isp_bnr_cfg15.u32 = be_reg->isp_bnr_cfg15.u32;
    o_isp_bnr_cfg15.bits.isp_bnr_coring_enable = uisp_bnr_coring_enable;
    be_reg->isp_bnr_cfg15.u32 = o_isp_bnr_cfg15.u32;
}

/*  description : set the value of the member isp_bnr_cfg15.isp_bnr_jnlm_gain */
/*  input       : unsigned int uisp_bnr_jnlm_gain: 8 bits */
static __inline td_void isp_bnr_jnlm_gain_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_jnlm_gain)
{
    u_isp_bnr_cfg15 o_isp_bnr_cfg15;
    o_isp_bnr_cfg15.u32 = be_reg->isp_bnr_cfg15.u32;
    o_isp_bnr_cfg15.bits.isp_bnr_jnlm_gain = uisp_bnr_jnlm_gain;
    be_reg->isp_bnr_cfg15.u32 = o_isp_bnr_cfg15.u32;
}

/*  description : set the value of the member isp_bnr_cfg15.isp_bnr_maxv_mode */
/*  input       : unsigned int uisp_bnr_maxv_mode: 1 bits */
static __inline td_void isp_bnr_maxv_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_maxv_mode)
{
    u_isp_bnr_cfg15 o_isp_bnr_cfg15;
    o_isp_bnr_cfg15.u32 = be_reg->isp_bnr_cfg15.u32;
    o_isp_bnr_cfg15.bits.isp_bnr_maxv_mode = uisp_bnr_maxv_mode;
    be_reg->isp_bnr_cfg15.u32 = o_isp_bnr_cfg15.u32;
}

/*  description : set the value of the member isp_bnr_cfg15.isp_bnr_mixgain_sprs_en */
/*  input       : unsigned int uisp_bnr_mixgain_sprs_en: 1 bits */
static __inline td_void isp_bnr_mixgain_sprs_en_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mixgain_sprs_en)
{
    u_isp_bnr_cfg15 o_isp_bnr_cfg15;
    o_isp_bnr_cfg15.u32 = be_reg->isp_bnr_cfg15.u32;
    o_isp_bnr_cfg15.bits.isp_bnr_mixgain_sprs_en = uisp_bnr_mixgain_sprs_en;
    be_reg->isp_bnr_cfg15.u32 = o_isp_bnr_cfg15.u32;
}

/*  description : set the value of the member isp_bnr_cfg16.isp_bnr_nr_strength_mv_int_mot2yuv */
/*  input       : unsigned int uisp_bnr_nr_strength_mv_int_mot2yuv: 9 bits */
static __inline td_void isp_bnr_nr_strength_mv_int_mot2yuv_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_nr_strength_mv_int_mot2yuv)
{
    u_isp_bnr_cfg16 o_isp_bnr_cfg16;
    o_isp_bnr_cfg16.u32 = be_reg->isp_bnr_cfg16.u32;
    o_isp_bnr_cfg16.bits.isp_bnr_nr_strength_mv_int_mot2yuv = uisp_bnr_nr_strength_mv_int_mot2yuv;
    be_reg->isp_bnr_cfg16.u32 = o_isp_bnr_cfg16.u32;
}

/*  description : set the value of the member isp_bnr_cfg16.isp_bnr_nr_strength_slope_mot2yuv */
/*  input       : unsigned int uisp_bnr_nr_strength_slope_mot2yuv: 10 bits */
static __inline td_void isp_bnr_nr_strength_slope_mot2yuv_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_nr_strength_slope_mot2yuv)
{
    u_isp_bnr_cfg16 o_isp_bnr_cfg16;
    o_isp_bnr_cfg16.u32 = be_reg->isp_bnr_cfg16.u32;
    o_isp_bnr_cfg16.bits.isp_bnr_nr_strength_slope_mot2yuv = uisp_bnr_nr_strength_slope_mot2yuv;
    be_reg->isp_bnr_cfg16.u32 = o_isp_bnr_cfg16.u32;
}

/*  description : set the value of the member isp_bnr_cfg16.isp_bnr_nr_strength_st_int_mot2yuv */
/*  input       : unsigned int uisp_bnr_nr_strength_st_int_mot2yuv: 9 bits */
static __inline td_void isp_bnr_nr_strength_st_int_mot2yuv_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_nr_strength_st_int_mot2yuv)
{
    u_isp_bnr_cfg16 o_isp_bnr_cfg16;
    o_isp_bnr_cfg16.u32 = be_reg->isp_bnr_cfg16.u32;
    o_isp_bnr_cfg16.bits.isp_bnr_nr_strength_st_int_mot2yuv = uisp_bnr_nr_strength_st_int_mot2yuv;
    be_reg->isp_bnr_cfg16.u32 = o_isp_bnr_cfg16.u32;
}

/*  description : set the value of the member isp_bnr_cfg17.isp_bnr_rnt_th_mot2yuv */
/*  input       : unsigned int uisp_bnr_rnt_th_mot2yuv: 8 bits */
static __inline td_void isp_bnr_rnt_th_mot2yuv_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_rnt_th_mot2yuv)
{
    u_isp_bnr_cfg17 o_isp_bnr_cfg17;
    o_isp_bnr_cfg17.u32 = be_reg->isp_bnr_cfg17.u32;
    o_isp_bnr_cfg17.bits.isp_bnr_rnt_th_mot2yuv = uisp_bnr_rnt_th_mot2yuv;
    be_reg->isp_bnr_cfg17.u32 = o_isp_bnr_cfg17.u32;
}

/*  description : set the value of the member isp_bnr_cfg17.isp_bnr_mdet_cor_level_mot2yuv */
/*  input       : unsigned int uisp_bnr_mdet_cor_level_mot2yuv: 8 bits */
static __inline td_void isp_bnr_mdet_cor_level_mot2yuv_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mdet_cor_level_mot2yuv)
{
    u_isp_bnr_cfg17 o_isp_bnr_cfg17;
    o_isp_bnr_cfg17.u32 = be_reg->isp_bnr_cfg17.u32;
    o_isp_bnr_cfg17.bits.isp_bnr_mdet_cor_level_mot2yuv = uisp_bnr_mdet_cor_level_mot2yuv;
    be_reg->isp_bnr_cfg17.u32 = o_isp_bnr_cfg17.u32;
}

/*  description : set the value of the member isp_bnr_cfg17.isp_bnr_mdet_size_mot2yuv */
/*  input       : unsigned int uisp_bnr_mdet_size_mot2yuv: 3 bits */
static __inline td_void isp_bnr_mdet_size_mot2yuv_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_mdet_size_mot2yuv)
{
    u_isp_bnr_cfg17 o_isp_bnr_cfg17;
    o_isp_bnr_cfg17.u32 = be_reg->isp_bnr_cfg17.u32;
    o_isp_bnr_cfg17.bits.isp_bnr_mdet_size_mot2yuv = uisp_bnr_mdet_size_mot2yuv;
    be_reg->isp_bnr_cfg17.u32 = o_isp_bnr_cfg17.u32;
}

/*  description : set the value of the member isp_bnr_cfg17.isp_bnr_tss */
/*  input       : unsigned int uisp_bnr_tss: 8 bits */
static __inline td_void isp_bnr_tss_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_tss)
{
    u_isp_bnr_cfg17 o_isp_bnr_cfg17;
    o_isp_bnr_cfg17.u32 = be_reg->isp_bnr_cfg17.u32;
    o_isp_bnr_cfg17.bits.isp_bnr_tss = uisp_bnr_tss;
    be_reg->isp_bnr_cfg17.u32 = o_isp_bnr_cfg17.u32;
}

/*  description : set the value of the member isp_bnr_stt2lut_cfg.isp_bnr_stt2lut_en */
/*  input       : unsigned int uisp_bnr_stt2lut_en: 1 bits */
static __inline td_void isp_bnr_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_stt2lut_en)
{
    u_isp_bnr_stt2lut_cfg o_isp_bnr_stt2lut_cfg;
    o_isp_bnr_stt2lut_cfg.u32 = be_reg->isp_bnr_stt2lut_cfg.u32;
    o_isp_bnr_stt2lut_cfg.bits.isp_bnr_stt2lut_en = uisp_bnr_stt2lut_en;
    be_reg->isp_bnr_stt2lut_cfg.u32 = o_isp_bnr_stt2lut_cfg.u32;
}

/*  description : set the value of the member isp_bnr_stt2lut_regnew.isp_bnr_stt2lut_regnew */
/*  input       : unsigned int uisp_bnr_stt2lut_regnew: 1 bits */
static __inline td_void isp_bnr_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_stt2lut_regnew)
{
    u_isp_bnr_stt2lut_regnew o_isp_bnr_stt2lut_regnew;
    o_isp_bnr_stt2lut_regnew.u32 = be_reg->isp_bnr_stt2lut_regnew.u32;
    o_isp_bnr_stt2lut_regnew.bits.isp_bnr_stt2lut_regnew = uisp_bnr_stt2lut_regnew;
    be_reg->isp_bnr_stt2lut_regnew.u32 = o_isp_bnr_stt2lut_regnew.u32;
}

/*  description : set the value of the member isp_bnr_stt2lut_abn.isp_bnr_stt2lut_clr */
/*  input       : unsigned int uisp_bnr_stt2lut_clr: 1 bits */
static __inline td_void isp_bnr_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_stt2lut_clr)
{
    u_isp_bnr_stt2lut_abn o_isp_bnr_stt2lut_abn;
    o_isp_bnr_stt2lut_abn.u32 = be_reg->isp_bnr_stt2lut_abn.u32;
    o_isp_bnr_stt2lut_abn.bits.isp_bnr_stt2lut_clr = uisp_bnr_stt2lut_clr;
    be_reg->isp_bnr_stt2lut_abn.u32 = o_isp_bnr_stt2lut_abn.u32;
}

static __inline td_u16 isp_bnr_stt2lut_info_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_bnr_stt2lut_abn.bits.isp_bnr_stt2lut_info;
}

/*  description : set the value of the member isp_bnr_noisesd_even_waddr.isp_bnr_noisesd_even_waddr */
/*  input       : unsigned int uisp_bnr_noisesd_even_waddr: 32 bits */
static __inline td_void isp_bnr_noisesd_even_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_even_waddr)
{
    be_reg->isp_bnr_noisesd_even_waddr.u32 = uisp_bnr_noisesd_even_waddr;
}

/*  description : set the value of the member isp_bnr_noisesd_even_wdata.isp_bnr_noisesd_even_wdata */
/*  input       : unsigned int uisp_bnr_noisesd_even_wdata: 15 bits */
static __inline td_void isp_bnr_noisesd_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_even_wdata)
{
    u_isp_bnr_noisesd_even_wdata o_isp_bnr_noisesd_even_wdata;
    o_isp_bnr_noisesd_even_wdata.u32 = be_reg->isp_bnr_noisesd_even_wdata.u32;
    o_isp_bnr_noisesd_even_wdata.bits.isp_bnr_noisesd_even_wdata = uisp_bnr_noisesd_even_wdata;
    be_reg->isp_bnr_noisesd_even_wdata.u32 = o_isp_bnr_noisesd_even_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_even_wdata.isp_bnr_noisesd_s_even_wdata */
/*  input       : unsigned int uisp_bnr_noisesd_s_even_wdata: 15 bits */
static __inline td_void isp_bnr_noisesd_s_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_s_even_wdata)
{
    u_isp_bnr_noisesd_even_wdata o_isp_bnr_noisesd_even_wdata;
    o_isp_bnr_noisesd_even_wdata.u32 = be_reg->isp_bnr_noisesd_even_wdata.u32;
    o_isp_bnr_noisesd_even_wdata.bits.isp_bnr_noisesd_s_even_wdata = uisp_bnr_noisesd_s_even_wdata;
    be_reg->isp_bnr_noisesd_even_wdata.u32 = o_isp_bnr_noisesd_even_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_even_raddr.isp_bnr_noisesd_even_raddr */
/*  input       : unsigned int uisp_bnr_noisesd_even_raddr: 32 bits */
static __inline td_void isp_bnr_noisesd_even_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_even_raddr)
{
    be_reg->isp_bnr_noisesd_even_raddr.u32 = uisp_bnr_noisesd_even_raddr;
}


/*  description : set the value of the member isp_bnr_noisesd_even_rdata.isp_bnr_noisesd_even_rdata */
/*  input       : unsigned int uisp_bnr_noisesd_even_rdata: 15 bits */
static __inline td_void isp_bnr_noisesd_even_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_even_rdata)
{
    u_isp_bnr_noisesd_even_rdata o_isp_bnr_noisesd_even_rdata;
    o_isp_bnr_noisesd_even_rdata.u32 = be_reg->isp_bnr_noisesd_even_rdata.u32;
    o_isp_bnr_noisesd_even_rdata.bits.isp_bnr_noisesd_even_rdata = uisp_bnr_noisesd_even_rdata;
    be_reg->isp_bnr_noisesd_even_rdata.u32 = o_isp_bnr_noisesd_even_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_even_rdata.isp_bnr_noisesd_s_even_rdata */
/*  input       : unsigned int uisp_bnr_noisesd_s_even_rdata: 15 bits */
static __inline td_void isp_bnr_noisesd_s_even_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_s_even_rdata)
{
    u_isp_bnr_noisesd_even_rdata o_isp_bnr_noisesd_even_rdata;
    o_isp_bnr_noisesd_even_rdata.u32 = be_reg->isp_bnr_noisesd_even_rdata.u32;
    o_isp_bnr_noisesd_even_rdata.bits.isp_bnr_noisesd_s_even_rdata = uisp_bnr_noisesd_s_even_rdata;
    be_reg->isp_bnr_noisesd_even_rdata.u32 = o_isp_bnr_noisesd_even_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_odd_waddr.isp_bnr_noisesd_odd_waddr */
/*  input       : unsigned int uisp_bnr_noisesd_odd_waddr: 32 bits */
static __inline td_void isp_bnr_noisesd_odd_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_odd_waddr)
{
    be_reg->isp_bnr_noisesd_odd_waddr.u32 = uisp_bnr_noisesd_odd_waddr;
}


/*  description : set the value of the member isp_bnr_noisesd_odd_wdata.isp_bnr_noisesd_odd_wdata */
/*  input       : unsigned int uisp_bnr_noisesd_odd_wdata: 15 bits */
static __inline td_void isp_bnr_noisesd_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_odd_wdata)
{
    u_isp_bnr_noisesd_odd_wdata o_isp_bnr_noisesd_odd_wdata;
    o_isp_bnr_noisesd_odd_wdata.u32 = be_reg->isp_bnr_noisesd_odd_wdata.u32;
    o_isp_bnr_noisesd_odd_wdata.bits.isp_bnr_noisesd_odd_wdata = uisp_bnr_noisesd_odd_wdata;
    be_reg->isp_bnr_noisesd_odd_wdata.u32 = o_isp_bnr_noisesd_odd_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_odd_wdata.isp_bnr_noisesd_s_odd_wdata */
/*  input       : unsigned int uisp_bnr_noisesd_s_odd_wdata: 15 bits */
static __inline td_void isp_bnr_noisesd_s_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_s_odd_wdata)
{
    u_isp_bnr_noisesd_odd_wdata o_isp_bnr_noisesd_odd_wdata;
    o_isp_bnr_noisesd_odd_wdata.u32 = be_reg->isp_bnr_noisesd_odd_wdata.u32;
    o_isp_bnr_noisesd_odd_wdata.bits.isp_bnr_noisesd_s_odd_wdata = uisp_bnr_noisesd_s_odd_wdata;
    be_reg->isp_bnr_noisesd_odd_wdata.u32 = o_isp_bnr_noisesd_odd_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_odd_raddr.isp_bnr_noisesd_odd_raddr */
/*  input       : unsigned int uisp_bnr_noisesd_odd_raddr: 32 bits */
static __inline td_void isp_bnr_noisesd_odd_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_odd_raddr)
{
    be_reg->isp_bnr_noisesd_odd_raddr.u32 = uisp_bnr_noisesd_odd_raddr;
}


/*  description : set the value of the member isp_bnr_noisesd_odd_rdata.isp_bnr_noisesd_odd_rdata */
/*  input       : unsigned int uisp_bnr_noisesd_odd_rdata: 15 bits */
static __inline td_void isp_bnr_noisesd_odd_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_odd_rdata)
{
    u_isp_bnr_noisesd_odd_rdata o_isp_bnr_noisesd_odd_rdata;
    o_isp_bnr_noisesd_odd_rdata.u32 = be_reg->isp_bnr_noisesd_odd_rdata.u32;
    o_isp_bnr_noisesd_odd_rdata.bits.isp_bnr_noisesd_odd_rdata = uisp_bnr_noisesd_odd_rdata;
    be_reg->isp_bnr_noisesd_odd_rdata.u32 = o_isp_bnr_noisesd_odd_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesd_odd_rdata.isp_bnr_noisesd_s_odd_rdata */
/*  input       : unsigned int uisp_bnr_noisesd_s_odd_rdata: 15 bits */
static __inline td_void isp_bnr_noisesd_s_odd_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesd_s_odd_rdata)
{
    u_isp_bnr_noisesd_odd_rdata o_isp_bnr_noisesd_odd_rdata;
    o_isp_bnr_noisesd_odd_rdata.u32 = be_reg->isp_bnr_noisesd_odd_rdata.u32;
    o_isp_bnr_noisesd_odd_rdata.bits.isp_bnr_noisesd_s_odd_rdata = uisp_bnr_noisesd_s_odd_rdata;
    be_reg->isp_bnr_noisesd_odd_rdata.u32 = o_isp_bnr_noisesd_odd_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noiseinv_even_waddr.isp_bnr_noiseinv_even_waddr */
/*  input       : unsigned int uisp_bnr_noiseinv_even_waddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_even_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_even_waddr)
{
    be_reg->isp_bnr_noiseinv_even_waddr.u32 = uisp_bnr_noiseinv_even_waddr;
}


/*  description : set the value of the member isp_bnr_noiseinv_even_wdata.isp_bnr_noiseinv_even_wdata */
/*  input       : unsigned int uisp_bnr_noiseinv_even_wdata: 32 bits */
static __inline td_void isp_bnr_noiseinv_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_even_wdata)
{
    be_reg->isp_bnr_noiseinv_even_wdata.u32 = uisp_bnr_noiseinv_even_wdata;
}


/*  description : set the value of the member isp_bnr_noiseinv_even_raddr.isp_bnr_noiseinv_even_raddr */
/*  input       : unsigned int uisp_bnr_noiseinv_even_raddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_even_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_even_raddr)
{
    be_reg->isp_bnr_noiseinv_even_raddr.u32 = uisp_bnr_noiseinv_even_raddr;
}


/*  description : set the value of the member isp_bnr_noiseinv_odd_waddr.isp_bnr_noiseinv_odd_waddr */
/*  input       : unsigned int uisp_bnr_noiseinv_odd_waddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_odd_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_odd_waddr)
{
    be_reg->isp_bnr_noiseinv_odd_waddr.u32 = uisp_bnr_noiseinv_odd_waddr;
}


/*  description : set the value of the member isp_bnr_noiseinv_odd_wdata.isp_bnr_noiseinv_odd_wdata */
/*  input       : unsigned int uisp_bnr_noiseinv_odd_wdata: 32 bits */
static __inline td_void isp_bnr_noiseinv_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_odd_wdata)
{
    be_reg->isp_bnr_noiseinv_odd_wdata.u32 = uisp_bnr_noiseinv_odd_wdata;
}


/*  description : set the value of the member isp_bnr_noiseinv_odd_raddr.isp_bnr_noiseinv_odd_raddr */
/*  input       : unsigned int uisp_bnr_noiseinv_odd_raddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_odd_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_odd_raddr)
{
    be_reg->isp_bnr_noiseinv_odd_raddr.u32 = uisp_bnr_noiseinv_odd_raddr;
}


/*  description : set the value of the member isp_bnr_fbratiotable_waddr.isp_bnr_fbratiotable_waddr */
/*  input       : unsigned int uisp_bnr_fbratiotable_waddr: 32 bits */
static __inline td_void isp_bnr_fbratiotable_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_fbratiotable_waddr)
{
    be_reg->isp_bnr_fbratiotable_waddr.u32 = uisp_bnr_fbratiotable_waddr;
}


/*  description : set the value of the member isp_bnr_fbratiotable_wdata.isp_bnr_fbratiotable_wdata */
/*  input       : unsigned int uisp_bnr_fbratiotable_wdata: 32 bits */
static __inline td_void isp_bnr_fbratiotable_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_fbratiotable_wdata)
{
    be_reg->isp_bnr_fbratiotable_wdata.u32 = uisp_bnr_fbratiotable_wdata;
}


/*  description : set the value of the member isp_bnr_fbratiotable_raddr.isp_bnr_fbratiotable_raddr */
/*  input       : unsigned int uisp_bnr_fbratiotable_raddr: 32 bits */
static __inline td_void isp_bnr_fbratiotable_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_fbratiotable_raddr)
{
    be_reg->isp_bnr_fbratiotable_raddr.u32 = uisp_bnr_fbratiotable_raddr;
}


/*  description : set the value of the member isp_bnr_noisesds_even_waddr.isp_bnr_noisesds_even_waddr */
/*  input       : unsigned int uisp_bnr_noisesds_even_waddr: 32 bits */
static __inline td_void isp_bnr_noisesds_even_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_even_waddr)
{
    be_reg->isp_bnr_noisesds_even_waddr.u32 = uisp_bnr_noisesds_even_waddr;
}


/*  description : set the value of the member isp_bnr_noisesds_even_wdata.isp_bnr_noisesds_even_wdata */
/*  input       : unsigned int uisp_bnr_noisesds_even_wdata: 15 bits */
static __inline td_void isp_bnr_noisesds_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_even_wdata)
{
    u_isp_bnr_noisesds_even_wdata o_isp_bnr_noisesds_even_wdata;
    o_isp_bnr_noisesds_even_wdata.u32 = be_reg->isp_bnr_noisesds_even_wdata.u32;
    o_isp_bnr_noisesds_even_wdata.bits.isp_bnr_noisesds_even_wdata = uisp_bnr_noisesds_even_wdata;
    be_reg->isp_bnr_noisesds_even_wdata.u32 = o_isp_bnr_noisesds_even_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_even_wdata.isp_bnr_noisesds_s_even_wdata */
/*  input       : unsigned int uisp_bnr_noisesds_s_even_wdata: 15 bits */
static __inline td_void isp_bnr_noisesds_s_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_s_even_wdata)
{
    u_isp_bnr_noisesds_even_wdata o_isp_bnr_noisesds_even_wdata;
    o_isp_bnr_noisesds_even_wdata.u32 = be_reg->isp_bnr_noisesds_even_wdata.u32;
    o_isp_bnr_noisesds_even_wdata.bits.isp_bnr_noisesds_s_even_wdata = uisp_bnr_noisesds_s_even_wdata;
    be_reg->isp_bnr_noisesds_even_wdata.u32 = o_isp_bnr_noisesds_even_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_even_raddr.isp_bnr_noisesds_even_raddr */
/*  input       : unsigned int uisp_bnr_noisesds_even_raddr: 32 bits */
static __inline td_void isp_bnr_noisesds_even_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_even_raddr)
{
    be_reg->isp_bnr_noisesds_even_raddr.u32 = uisp_bnr_noisesds_even_raddr;
}


/*  description : set the value of the member isp_bnr_noisesds_even_rdata.isp_bnr_noisesds_even_rdata */
/*  input       : unsigned int uisp_bnr_noisesds_even_rdata: 15 bits */
static __inline td_void isp_bnr_noisesds_even_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_even_rdata)
{
    u_isp_bnr_noisesds_even_rdata o_isp_bnr_noisesds_even_rdata;
    o_isp_bnr_noisesds_even_rdata.u32 = be_reg->isp_bnr_noisesds_even_rdata.u32;
    o_isp_bnr_noisesds_even_rdata.bits.isp_bnr_noisesds_even_rdata = uisp_bnr_noisesds_even_rdata;
    be_reg->isp_bnr_noisesds_even_rdata.u32 = o_isp_bnr_noisesds_even_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_even_rdata.isp_bnr_noisesds_s_even_rdata */
/*  input       : unsigned int uisp_bnr_noisesds_s_even_rdata: 15 bits */
static __inline td_void isp_bnr_noisesds_s_even_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_s_even_rdata)
{
    u_isp_bnr_noisesds_even_rdata o_isp_bnr_noisesds_even_rdata;
    o_isp_bnr_noisesds_even_rdata.u32 = be_reg->isp_bnr_noisesds_even_rdata.u32;
    o_isp_bnr_noisesds_even_rdata.bits.isp_bnr_noisesds_s_even_rdata = uisp_bnr_noisesds_s_even_rdata;
    be_reg->isp_bnr_noisesds_even_rdata.u32 = o_isp_bnr_noisesds_even_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_odd_waddr.isp_bnr_noisesds_odd_waddr */
/*  input       : unsigned int uisp_bnr_noisesds_odd_waddr: 32 bits */
static __inline td_void isp_bnr_noisesds_odd_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_odd_waddr)
{
    be_reg->isp_bnr_noisesds_odd_waddr.u32 = uisp_bnr_noisesds_odd_waddr;
}


/*  description : set the value of the member isp_bnr_noisesds_odd_wdata.isp_bnr_noisesds_odd_wdata */
/*  input       : unsigned int uisp_bnr_noisesds_odd_wdata: 15 bits */
static __inline td_void isp_bnr_noisesds_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_odd_wdata)
{
    u_isp_bnr_noisesds_odd_wdata o_isp_bnr_noisesds_odd_wdata;
    o_isp_bnr_noisesds_odd_wdata.u32 = be_reg->isp_bnr_noisesds_odd_wdata.u32;
    o_isp_bnr_noisesds_odd_wdata.bits.isp_bnr_noisesds_odd_wdata = uisp_bnr_noisesds_odd_wdata;
    be_reg->isp_bnr_noisesds_odd_wdata.u32 = o_isp_bnr_noisesds_odd_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_odd_wdata.isp_bnr_noisesds_s_odd_wdata */
/*  input       : unsigned int uisp_bnr_noisesds_s_odd_wdata: 15 bits */
static __inline td_void isp_bnr_noisesds_s_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_s_odd_wdata)
{
    u_isp_bnr_noisesds_odd_wdata o_isp_bnr_noisesds_odd_wdata;
    o_isp_bnr_noisesds_odd_wdata.u32 = be_reg->isp_bnr_noisesds_odd_wdata.u32;
    o_isp_bnr_noisesds_odd_wdata.bits.isp_bnr_noisesds_s_odd_wdata = uisp_bnr_noisesds_s_odd_wdata;
    be_reg->isp_bnr_noisesds_odd_wdata.u32 = o_isp_bnr_noisesds_odd_wdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_odd_raddr.isp_bnr_noisesds_odd_raddr */
/*  input       : unsigned int uisp_bnr_noisesds_odd_raddr: 32 bits */
static __inline td_void isp_bnr_noisesds_odd_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_odd_raddr)
{
    be_reg->isp_bnr_noisesds_odd_raddr.u32 = uisp_bnr_noisesds_odd_raddr;
}


/*  description : set the value of the member isp_bnr_noisesds_odd_rdata.isp_bnr_noisesds_odd_rdata */
/*  input       : unsigned int uisp_bnr_noisesds_odd_rdata: 15 bits */
static __inline td_void isp_bnr_noisesds_odd_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_odd_rdata)
{
    u_isp_bnr_noisesds_odd_rdata o_isp_bnr_noisesds_odd_rdata;
    o_isp_bnr_noisesds_odd_rdata.u32 = be_reg->isp_bnr_noisesds_odd_rdata.u32;
    o_isp_bnr_noisesds_odd_rdata.bits.isp_bnr_noisesds_odd_rdata = uisp_bnr_noisesds_odd_rdata;
    be_reg->isp_bnr_noisesds_odd_rdata.u32 = o_isp_bnr_noisesds_odd_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noisesds_odd_rdata.isp_bnr_noisesds_s_odd_rdata */
/*  input       : unsigned int uisp_bnr_noisesds_s_odd_rdata: 15 bits */
static __inline td_void isp_bnr_noisesds_s_odd_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noisesds_s_odd_rdata)
{
    u_isp_bnr_noisesds_odd_rdata o_isp_bnr_noisesds_odd_rdata;
    o_isp_bnr_noisesds_odd_rdata.u32 = be_reg->isp_bnr_noisesds_odd_rdata.u32;
    o_isp_bnr_noisesds_odd_rdata.bits.isp_bnr_noisesds_s_odd_rdata = uisp_bnr_noisesds_s_odd_rdata;
    be_reg->isp_bnr_noisesds_odd_rdata.u32 = o_isp_bnr_noisesds_odd_rdata.u32;
}

/*  description : set the value of the member isp_bnr_noiseinv_mag_even_waddr.isp_bnr_noiseinv_mag_even_waddr */
/*  input       : unsigned int uisp_bnr_noiseinv_mag_even_waddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_mag_even_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_mag_even_waddr)
{
    be_reg->isp_bnr_noiseinv_mag_even_waddr.u32 = uisp_bnr_noiseinv_mag_even_waddr;
}


/*  description : set the value of the member isp_bnr_noiseinv_mag_even_wdata.isp_bnr_noiseinv_mag_even_wdata */
/*  input       : unsigned int uisp_bnr_noiseinv_mag_even_wdata: 32 bits */
static __inline td_void isp_bnr_noiseinv_mag_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_mag_even_wdata)
{
    be_reg->isp_bnr_noiseinv_mag_even_wdata.u32 = uisp_bnr_noiseinv_mag_even_wdata;
}


/*  description : set the value of the member isp_bnr_noiseinv_mag_even_raddr.isp_bnr_noiseinv_mag_even_raddr */
/*  input       : unsigned int uisp_bnr_noiseinv_mag_even_raddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_mag_even_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_mag_even_raddr)
{
    be_reg->isp_bnr_noiseinv_mag_even_raddr.u32 = uisp_bnr_noiseinv_mag_even_raddr;
}


/*  description : set the value of the member isp_bnr_noiseinv_mag_odd_waddr.isp_bnr_noiseinv_mag_odd_waddr */
/*  input       : unsigned int uisp_bnr_noiseinv_mag_odd_waddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_mag_odd_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_mag_odd_waddr)
{
    be_reg->isp_bnr_noiseinv_mag_odd_waddr.u32 = uisp_bnr_noiseinv_mag_odd_waddr;
}


/*  description : set the value of the member isp_bnr_noiseinv_mag_odd_wdata.isp_bnr_noiseinv_mag_odd_wdata */
/*  input       : unsigned int uisp_bnr_noiseinv_mag_odd_wdata: 32 bits */
static __inline td_void isp_bnr_noiseinv_mag_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_mag_odd_wdata)
{
    be_reg->isp_bnr_noiseinv_mag_odd_wdata.u32 = uisp_bnr_noiseinv_mag_odd_wdata;
}


/*  description : set the value of the member isp_bnr_noiseinv_mag_odd_raddr.isp_bnr_noiseinv_mag_odd_raddr */
/*  input       : unsigned int uisp_bnr_noiseinv_mag_odd_raddr: 32 bits */
static __inline td_void isp_bnr_noiseinv_mag_odd_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_noiseinv_mag_odd_raddr)
{
    be_reg->isp_bnr_noiseinv_mag_odd_raddr.u32 = uisp_bnr_noiseinv_mag_odd_raddr;
}


/*  description : set the value of the member isp_bnr_coring_low_even_waddr.isp_bnr_coring_low_even_waddr */
/*  input       : unsigned int uisp_bnr_coring_low_even_waddr: 32 bits */
static __inline td_void isp_bnr_coring_low_even_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_low_even_waddr)
{
    be_reg->isp_bnr_coring_low_even_waddr.u32 = uisp_bnr_coring_low_even_waddr;
}


/*  description : set the value of the member isp_bnr_coring_low_even_wdata.isp_bnr_coring_low_even_wdata */
/*  input       : unsigned int uisp_bnr_coring_low_even_wdata: 32 bits */
static __inline td_void isp_bnr_coring_low_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_low_even_wdata)
{
    be_reg->isp_bnr_coring_low_even_wdata.u32 = uisp_bnr_coring_low_even_wdata;
}


/*  description : set the value of the member isp_bnr_coring_low_even_raddr.isp_bnr_coring_low_even_raddr */
/*  input       : unsigned int uisp_bnr_coring_low_even_raddr: 32 bits */
static __inline td_void isp_bnr_coring_low_even_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_low_even_raddr)
{
    be_reg->isp_bnr_coring_low_even_raddr.u32 = uisp_bnr_coring_low_even_raddr;
}


/*  description : set the value of the member isp_bnr_coring_low_odd_waddr.isp_bnr_coring_low_odd_waddr */
/*  input       : unsigned int uisp_bnr_coring_low_odd_waddr: 32 bits */
static __inline td_void isp_bnr_coring_low_odd_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_low_odd_waddr)
{
    be_reg->isp_bnr_coring_low_odd_waddr.u32 = uisp_bnr_coring_low_odd_waddr;
}


/*  description : set the value of the member isp_bnr_coring_low_odd_wdata.isp_bnr_coring_low_odd_wdata */
/*  input       : unsigned int uisp_bnr_coring_low_odd_wdata: 32 bits */
static __inline td_void isp_bnr_coring_low_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_low_odd_wdata)
{
    be_reg->isp_bnr_coring_low_odd_wdata.u32 = uisp_bnr_coring_low_odd_wdata;
}


/*  description : set the value of the member isp_bnr_coring_low_odd_raddr.isp_bnr_coring_low_odd_raddr */
/*  input       : unsigned int uisp_bnr_coring_low_odd_raddr: 32 bits */
static __inline td_void isp_bnr_coring_low_odd_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_coring_low_odd_raddr)
{
    be_reg->isp_bnr_coring_low_odd_raddr.u32 = uisp_bnr_coring_low_odd_raddr;
}


/*  description : set the value of the member isp_bnr_fbra_mot2yuv_waddr.isp_bnr_fbra_mot2yuv_waddr */
/*  input       : unsigned int uisp_bnr_fbra_mot2yuv_waddr: 32 bits */
static __inline td_void isp_bnr_fbra_mot2yuv_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_fbra_mot2yuv_waddr)
{
    be_reg->isp_bnr_fbra_mot2yuv_waddr.u32 = uisp_bnr_fbra_mot2yuv_waddr;
}


/*  description : set the value of the member isp_bnr_fbra_mot2yuv_wdata.isp_bnr_fbra_mot2yuv_wdata */
/*  input       : unsigned int uisp_bnr_fbra_mot2yuv_wdata: 32 bits */
static __inline td_void isp_bnr_fbra_mot2yuv_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_fbra_mot2yuv_wdata)
{
    be_reg->isp_bnr_fbra_mot2yuv_wdata.u32 = uisp_bnr_fbra_mot2yuv_wdata;
}


/*  description : set the value of the member isp_bnr_fbra_mot2yuv_raddr.isp_bnr_fbra_mot2yuv_raddr */
/*  input       : unsigned int uisp_bnr_fbra_mot2yuv_raddr: 32 bits */
static __inline td_void isp_bnr_fbra_mot2yuv_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bnr_fbra_mot2yuv_raddr)
{
    be_reg->isp_bnr_fbra_mot2yuv_raddr.u32 = uisp_bnr_fbra_mot2yuv_raddr;
}


/*  description   set the value of the member isp_mlsc_stt2lut_cfg.isp_mlsc_stt2lut_en */
/*  input         unsigned int uisp_mlsc_stt2lut_en  1 bits */
static __inline td_void isp_mlsc_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_stt2lut_en)
{
    u_isp_mlsc_stt2lut_cfg o_isp_mlsc_stt2lut_cfg;
    o_isp_mlsc_stt2lut_cfg.u32 = be_reg->isp_mlsc_stt2lut_cfg.u32;
    o_isp_mlsc_stt2lut_cfg.bits.isp_mlsc_stt2lut_en = uisp_mlsc_stt2lut_en;
    be_reg->isp_mlsc_stt2lut_cfg.u32 = o_isp_mlsc_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_mlsc_stt2lut_regnew.isp_mlsc_stt2lut_regnew */
/*  input         unsigned int uisp_mlsc_stt2lut_regnew  1 bits */
static __inline td_void isp_mlsc_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_stt2lut_regnew)
{
    u_isp_mlsc_stt2lut_regnew o_isp_mlsc_stt2lut_regnew;
    o_isp_mlsc_stt2lut_regnew.u32 = be_reg->isp_mlsc_stt2lut_regnew.u32;
    o_isp_mlsc_stt2lut_regnew.bits.isp_mlsc_stt2lut_regnew = uisp_mlsc_stt2lut_regnew;
    be_reg->isp_mlsc_stt2lut_regnew.u32 = o_isp_mlsc_stt2lut_regnew.u32;
}
/*  description   set the value of the member isp_mlsc_stt2lut_abn.isp_mlsc_stt2lut_clr */
/*  input         unsigned int uisp_mlsc_stt2lut_clr  1 bits */
static __inline td_void isp_mlsc_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_stt2lut_clr)
{
    u_isp_mlsc_stt2lut_abn o_isp_mlsc_stt2lut_abn;
    o_isp_mlsc_stt2lut_abn.u32 = be_reg->isp_mlsc_stt2lut_abn.u32;
    o_isp_mlsc_stt2lut_abn.bits.isp_mlsc_stt2lut_clr = uisp_mlsc_stt2lut_clr;
    be_reg->isp_mlsc_stt2lut_abn.u32 = o_isp_mlsc_stt2lut_abn.u32;
}

static __inline td_u16 isp_mlsc_stt2lut_info_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_mlsc_stt2lut_abn.bits.isp_mlsc_stt2lut_info;
}
/*  description   set the value of the member isp_mlsc_winnum.isp_mlsc_numh */
/*  input         unsigned int uisp_mlsc_numh  6 bits */
static __inline td_void isp_mlsc_numh_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_numh)
{
    u_isp_mlsc_winnum o_isp_mlsc_winnum;
    o_isp_mlsc_winnum.u32 = be_reg->isp_mlsc_winnum.u32;
    o_isp_mlsc_winnum.bits.isp_mlsc_numh = uisp_mlsc_numh;
    be_reg->isp_mlsc_winnum.u32 = o_isp_mlsc_winnum.u32;
}
/*  description   set the value of the member isp_mlsc_winnum.isp_mlsc_numv */
/*  input         unsigned int uisp_mlsc_numv  6 bits */
static __inline td_void isp_mlsc_numv_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_numv)
{
    u_isp_mlsc_winnum o_isp_mlsc_winnum;
    o_isp_mlsc_winnum.u32 = be_reg->isp_mlsc_winnum.u32;
    o_isp_mlsc_winnum.bits.isp_mlsc_numv = uisp_mlsc_numv;
    be_reg->isp_mlsc_winnum.u32 = o_isp_mlsc_winnum.u32;
}

static __inline td_void isp_mlsc_x0_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax0, td_u32 uisp_mlsc_invx0)
{
    u_isp_mlsc_winx0 o_isp_mlsc_winx0;
    o_isp_mlsc_winx0.u32 = be_reg->isp_mlsc_winx0.u32;
    o_isp_mlsc_winx0.bits.isp_mlsc_deltax0 = uisp_mlsc_deltax0;
    o_isp_mlsc_winx0.bits.isp_mlsc_invx0 = uisp_mlsc_invx0;
    be_reg->isp_mlsc_winx0.u32 = o_isp_mlsc_winx0.u32;
}

static __inline td_void isp_mlsc_x1_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax1, td_u32 uisp_mlsc_invx1)
{
    u_isp_mlsc_winx1 o_isp_mlsc_winx1;
    o_isp_mlsc_winx1.u32 = be_reg->isp_mlsc_winx1.u32;
    o_isp_mlsc_winx1.bits.isp_mlsc_deltax1 = uisp_mlsc_deltax1;
    o_isp_mlsc_winx1.bits.isp_mlsc_invx1 = uisp_mlsc_invx1;
    be_reg->isp_mlsc_winx1.u32 = o_isp_mlsc_winx1.u32;
}

static __inline td_void isp_mlsc_x2_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax2, td_u32 uisp_mlsc_invx2)
{
    u_isp_mlsc_winx2 o_isp_mlsc_winx2;
    o_isp_mlsc_winx2.u32 = be_reg->isp_mlsc_winx2.u32;
    o_isp_mlsc_winx2.bits.isp_mlsc_deltax2 = uisp_mlsc_deltax2;
    o_isp_mlsc_winx2.bits.isp_mlsc_invx2 = uisp_mlsc_invx2;
    be_reg->isp_mlsc_winx2.u32 = o_isp_mlsc_winx2.u32;
}

static __inline td_void isp_mlsc_x3_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax3, td_u32 uisp_mlsc_invx3)
{
    u_isp_mlsc_winx3 o_isp_mlsc_winx3;
    o_isp_mlsc_winx3.u32 = be_reg->isp_mlsc_winx3.u32;
    o_isp_mlsc_winx3.bits.isp_mlsc_deltax3 = uisp_mlsc_deltax3;
    o_isp_mlsc_winx3.bits.isp_mlsc_invx3 = uisp_mlsc_invx3;
    be_reg->isp_mlsc_winx3.u32 = o_isp_mlsc_winx3.u32;
}

static __inline td_void isp_mlsc_x4_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax4, td_u32 uisp_mlsc_invx4)
{
    u_isp_mlsc_winx4 o_isp_mlsc_winx4;
    o_isp_mlsc_winx4.u32 = be_reg->isp_mlsc_winx4.u32;
    o_isp_mlsc_winx4.bits.isp_mlsc_deltax4 = uisp_mlsc_deltax4;
    o_isp_mlsc_winx4.bits.isp_mlsc_invx4 = uisp_mlsc_invx4;
    be_reg->isp_mlsc_winx4.u32 = o_isp_mlsc_winx4.u32;
}

static __inline td_void isp_mlsc_x5_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax5, td_u32 uisp_mlsc_invx5)
{
    u_isp_mlsc_winx5 o_isp_mlsc_winx5;
    o_isp_mlsc_winx5.u32 = be_reg->isp_mlsc_winx5.u32;
    o_isp_mlsc_winx5.bits.isp_mlsc_deltax5 = uisp_mlsc_deltax5;
    o_isp_mlsc_winx5.bits.isp_mlsc_invx5 = uisp_mlsc_invx5;
    be_reg->isp_mlsc_winx5.u32 = o_isp_mlsc_winx5.u32;
}

static __inline td_void isp_mlsc_x6_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax6, td_u32 uisp_mlsc_invx6)
{
    u_isp_mlsc_winx6 o_isp_mlsc_winx6;
    o_isp_mlsc_winx6.u32 = be_reg->isp_mlsc_winx6.u32;
    o_isp_mlsc_winx6.bits.isp_mlsc_deltax6 = uisp_mlsc_deltax6;
    o_isp_mlsc_winx6.bits.isp_mlsc_invx6 = uisp_mlsc_invx6;
    be_reg->isp_mlsc_winx6.u32 = o_isp_mlsc_winx6.u32;
}

static __inline td_void isp_mlsc_x7_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax7, td_u32 uisp_mlsc_invx7)
{
    u_isp_mlsc_winx7 o_isp_mlsc_winx7;
    o_isp_mlsc_winx7.u32 = be_reg->isp_mlsc_winx7.u32;
    o_isp_mlsc_winx7.bits.isp_mlsc_deltax7 = uisp_mlsc_deltax7;
    o_isp_mlsc_winx7.bits.isp_mlsc_invx7 = uisp_mlsc_invx7;
    be_reg->isp_mlsc_winx7.u32 = o_isp_mlsc_winx7.u32;
}

static __inline td_void isp_mlsc_x8_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax8, td_u32 uisp_mlsc_invx8)
{
    u_isp_mlsc_winx8 o_isp_mlsc_winx8;
    o_isp_mlsc_winx8.u32 = be_reg->isp_mlsc_winx8.u32;
    o_isp_mlsc_winx8.bits.isp_mlsc_deltax8 = uisp_mlsc_deltax8;
    o_isp_mlsc_winx8.bits.isp_mlsc_invx8 = uisp_mlsc_invx8;
    be_reg->isp_mlsc_winx8.u32 = o_isp_mlsc_winx8.u32;
}

static __inline td_void isp_mlsc_x9_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax9, td_u32 uisp_mlsc_invx9)
{
    u_isp_mlsc_winx9 o_isp_mlsc_winx9;
    o_isp_mlsc_winx9.u32 = be_reg->isp_mlsc_winx9.u32;
    o_isp_mlsc_winx9.bits.isp_mlsc_deltax9 = uisp_mlsc_deltax9;
    o_isp_mlsc_winx9.bits.isp_mlsc_invx9 = uisp_mlsc_invx9;
    be_reg->isp_mlsc_winx9.u32 = o_isp_mlsc_winx9.u32;
}

static __inline td_void isp_mlsc_x10_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax10, td_u32 uisp_mlsc_invx10)
{
    u_isp_mlsc_winx10 o_isp_mlsc_winx10;
    o_isp_mlsc_winx10.u32 = be_reg->isp_mlsc_winx10.u32;
    o_isp_mlsc_winx10.bits.isp_mlsc_deltax10 = uisp_mlsc_deltax10;
    o_isp_mlsc_winx10.bits.isp_mlsc_invx10 = uisp_mlsc_invx10;
    be_reg->isp_mlsc_winx10.u32 = o_isp_mlsc_winx10.u32;
}

static __inline td_void isp_mlsc_x11_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax11, td_u32 uisp_mlsc_invx11)
{
    u_isp_mlsc_winx11 o_isp_mlsc_winx11;
    o_isp_mlsc_winx11.u32 = be_reg->isp_mlsc_winx11.u32;
    o_isp_mlsc_winx11.bits.isp_mlsc_deltax11 = uisp_mlsc_deltax11;
    o_isp_mlsc_winx11.bits.isp_mlsc_invx11 = uisp_mlsc_invx11;
    be_reg->isp_mlsc_winx11.u32 = o_isp_mlsc_winx11.u32;
}

static __inline td_void isp_mlsc_x12_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax12, td_u32 uisp_mlsc_invx12)
{
    u_isp_mlsc_winx12 o_isp_mlsc_winx12;
    o_isp_mlsc_winx12.u32 = be_reg->isp_mlsc_winx12.u32;
    o_isp_mlsc_winx12.bits.isp_mlsc_deltax12 = uisp_mlsc_deltax12;
    o_isp_mlsc_winx12.bits.isp_mlsc_invx12 = uisp_mlsc_invx12;
    be_reg->isp_mlsc_winx12.u32 = o_isp_mlsc_winx12.u32;
}

static __inline td_void isp_mlsc_x13_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax13, td_u32 uisp_mlsc_invx13)
{
    u_isp_mlsc_winx13 o_isp_mlsc_winx13;
    o_isp_mlsc_winx13.u32 = be_reg->isp_mlsc_winx13.u32;
    o_isp_mlsc_winx13.bits.isp_mlsc_deltax13 = uisp_mlsc_deltax13;
    o_isp_mlsc_winx13.bits.isp_mlsc_invx13 = uisp_mlsc_invx13;
    be_reg->isp_mlsc_winx13.u32 = o_isp_mlsc_winx13.u32;
}

static __inline td_void isp_mlsc_x14_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax14, td_u32 uisp_mlsc_invx14)
{
    u_isp_mlsc_winx14 o_isp_mlsc_winx14;
    o_isp_mlsc_winx14.u32 = be_reg->isp_mlsc_winx14.u32;
    o_isp_mlsc_winx14.bits.isp_mlsc_deltax14 = uisp_mlsc_deltax14;
    o_isp_mlsc_winx14.bits.isp_mlsc_invx14 = uisp_mlsc_invx14;
    be_reg->isp_mlsc_winx14.u32 = o_isp_mlsc_winx14.u32;
}

static __inline td_void isp_mlsc_x15_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax15, td_u32 uisp_mlsc_invx15)
{
    u_isp_mlsc_winx15 o_isp_mlsc_winx15;
    o_isp_mlsc_winx15.u32 = be_reg->isp_mlsc_winx15.u32;
    o_isp_mlsc_winx15.bits.isp_mlsc_deltax15 = uisp_mlsc_deltax15;
    o_isp_mlsc_winx15.bits.isp_mlsc_invx15 = uisp_mlsc_invx15;
    be_reg->isp_mlsc_winx15.u32 = o_isp_mlsc_winx15.u32;
}

static __inline td_void isp_mlsc_deltay0_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay0)
{
    u_isp_mlsc_winy0 o_isp_mlsc_winy0;
    o_isp_mlsc_winy0.u32 = be_reg->isp_mlsc_winy0.u32;
    o_isp_mlsc_winy0.bits.isp_mlsc_deltay0 = uisp_mlsc_deltay0;
    be_reg->isp_mlsc_winy0.u32 = o_isp_mlsc_winy0.u32;
}
/*  description   set the value of the member isp_mlsc_winy0.isp_mlsc_invy0 */
/*  input         unsigned int uisp_mlsc_invy0  11 bits */
static __inline td_void isp_mlsc_invy0_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy0)
{
    u_isp_mlsc_winy0 o_isp_mlsc_winy0;
    o_isp_mlsc_winy0.u32 = be_reg->isp_mlsc_winy0.u32;
    o_isp_mlsc_winy0.bits.isp_mlsc_invy0 = uisp_mlsc_invy0;
    be_reg->isp_mlsc_winy0.u32 = o_isp_mlsc_winy0.u32;
}
/*  description   set the value of the member isp_mlsc_winy1.isp_mlsc_deltay1 */
/*  input         unsigned int uisp_mlsc_deltay1  8 bits */
static __inline td_void isp_mlsc_deltay1_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay1)
{
    u_isp_mlsc_winy1 o_isp_mlsc_winy1;
    o_isp_mlsc_winy1.u32 = be_reg->isp_mlsc_winy1.u32;
    o_isp_mlsc_winy1.bits.isp_mlsc_deltay1 = uisp_mlsc_deltay1;
    be_reg->isp_mlsc_winy1.u32 = o_isp_mlsc_winy1.u32;
}
/*  description   set the value of the member isp_mlsc_winy1.isp_mlsc_invy1 */
/*  input         unsigned int uisp_mlsc_invy1  11 bits */
static __inline td_void isp_mlsc_invy1_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy1)
{
    u_isp_mlsc_winy1 o_isp_mlsc_winy1;
    o_isp_mlsc_winy1.u32 = be_reg->isp_mlsc_winy1.u32;
    o_isp_mlsc_winy1.bits.isp_mlsc_invy1 = uisp_mlsc_invy1;
    be_reg->isp_mlsc_winy1.u32 = o_isp_mlsc_winy1.u32;
}
/*  description   set the value of the member isp_mlsc_winy2.isp_mlsc_deltay2 */
/*  input         unsigned int uisp_mlsc_deltay2  8 bits */
static __inline td_void isp_mlsc_deltay2_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay2)
{
    u_isp_mlsc_winy2 o_isp_mlsc_winy2;
    o_isp_mlsc_winy2.u32 = be_reg->isp_mlsc_winy2.u32;
    o_isp_mlsc_winy2.bits.isp_mlsc_deltay2 = uisp_mlsc_deltay2;
    be_reg->isp_mlsc_winy2.u32 = o_isp_mlsc_winy2.u32;
}
/*  description   set the value of the member isp_mlsc_winy2.isp_mlsc_invy2 */
/*  input         unsigned int uisp_mlsc_invy2  11 bits */
static __inline td_void isp_mlsc_invy2_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy2)
{
    u_isp_mlsc_winy2 o_isp_mlsc_winy2;
    o_isp_mlsc_winy2.u32 = be_reg->isp_mlsc_winy2.u32;
    o_isp_mlsc_winy2.bits.isp_mlsc_invy2 = uisp_mlsc_invy2;
    be_reg->isp_mlsc_winy2.u32 = o_isp_mlsc_winy2.u32;
}
/*  description   set the value of the member isp_mlsc_winy3.isp_mlsc_deltay3 */
/*  input         unsigned int uisp_mlsc_deltay3  8 bits */
static __inline td_void isp_mlsc_deltay3_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay3)
{
    u_isp_mlsc_winy3 o_isp_mlsc_winy3;
    o_isp_mlsc_winy3.u32 = be_reg->isp_mlsc_winy3.u32;
    o_isp_mlsc_winy3.bits.isp_mlsc_deltay3 = uisp_mlsc_deltay3;
    be_reg->isp_mlsc_winy3.u32 = o_isp_mlsc_winy3.u32;
}
/*  description   set the value of the member isp_mlsc_winy3.isp_mlsc_invy3 */
/*  input         unsigned int uisp_mlsc_invy3  11 bits */
static __inline td_void isp_mlsc_invy3_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy3)
{
    u_isp_mlsc_winy3 o_isp_mlsc_winy3;
    o_isp_mlsc_winy3.u32 = be_reg->isp_mlsc_winy3.u32;
    o_isp_mlsc_winy3.bits.isp_mlsc_invy3 = uisp_mlsc_invy3;
    be_reg->isp_mlsc_winy3.u32 = o_isp_mlsc_winy3.u32;
}
/*  description   set the value of the member isp_mlsc_winy4.isp_mlsc_deltay4 */
/*  input         unsigned int uisp_mlsc_deltay4  8 bits */
static __inline td_void isp_mlsc_deltay4_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay4)
{
    u_isp_mlsc_winy4 o_isp_mlsc_winy4;
    o_isp_mlsc_winy4.u32 = be_reg->isp_mlsc_winy4.u32;
    o_isp_mlsc_winy4.bits.isp_mlsc_deltay4 = uisp_mlsc_deltay4;
    be_reg->isp_mlsc_winy4.u32 = o_isp_mlsc_winy4.u32;
}
/*  description   set the value of the member isp_mlsc_winy4.isp_mlsc_invy4 */
/*  input         unsigned int uisp_mlsc_invy4  11 bits */
static __inline td_void isp_mlsc_invy4_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy4)
{
    u_isp_mlsc_winy4 o_isp_mlsc_winy4;
    o_isp_mlsc_winy4.u32 = be_reg->isp_mlsc_winy4.u32;
    o_isp_mlsc_winy4.bits.isp_mlsc_invy4 = uisp_mlsc_invy4;
    be_reg->isp_mlsc_winy4.u32 = o_isp_mlsc_winy4.u32;
}
/*  description   set the value of the member isp_mlsc_winy5.isp_mlsc_deltay5 */
/*  input         unsigned int uisp_mlsc_deltay5  8 bits */
static __inline td_void isp_mlsc_deltay5_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay5)
{
    u_isp_mlsc_winy5 o_isp_mlsc_winy5;
    o_isp_mlsc_winy5.u32 = be_reg->isp_mlsc_winy5.u32;
    o_isp_mlsc_winy5.bits.isp_mlsc_deltay5 = uisp_mlsc_deltay5;
    be_reg->isp_mlsc_winy5.u32 = o_isp_mlsc_winy5.u32;
}
/*  description   set the value of the member isp_mlsc_winy5.isp_mlsc_invy5 */
/*  input         unsigned int uisp_mlsc_invy5  11 bits */
static __inline td_void isp_mlsc_invy5_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy5)
{
    u_isp_mlsc_winy5 o_isp_mlsc_winy5;
    o_isp_mlsc_winy5.u32 = be_reg->isp_mlsc_winy5.u32;
    o_isp_mlsc_winy5.bits.isp_mlsc_invy5 = uisp_mlsc_invy5;
    be_reg->isp_mlsc_winy5.u32 = o_isp_mlsc_winy5.u32;
}
/*  description   set the value of the member isp_mlsc_winy6.isp_mlsc_deltay6 */
/*  input         unsigned int uisp_mlsc_deltay6  8 bits */
static __inline td_void isp_mlsc_deltay6_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay6)
{
    u_isp_mlsc_winy6 o_isp_mlsc_winy6;
    o_isp_mlsc_winy6.u32 = be_reg->isp_mlsc_winy6.u32;
    o_isp_mlsc_winy6.bits.isp_mlsc_deltay6 = uisp_mlsc_deltay6;
    be_reg->isp_mlsc_winy6.u32 = o_isp_mlsc_winy6.u32;
}
/*  description   set the value of the member isp_mlsc_winy6.isp_mlsc_invy6 */
/*  input         unsigned int uisp_mlsc_invy6  11 bits */
static __inline td_void isp_mlsc_invy6_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy6)
{
    u_isp_mlsc_winy6 o_isp_mlsc_winy6;
    o_isp_mlsc_winy6.u32 = be_reg->isp_mlsc_winy6.u32;
    o_isp_mlsc_winy6.bits.isp_mlsc_invy6 = uisp_mlsc_invy6;
    be_reg->isp_mlsc_winy6.u32 = o_isp_mlsc_winy6.u32;
}
/*  description   set the value of the member isp_mlsc_winy7.isp_mlsc_deltay7 */
/*  input         unsigned int uisp_mlsc_deltay7  8 bits */
static __inline td_void isp_mlsc_deltay7_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay7)
{
    u_isp_mlsc_winy7 o_isp_mlsc_winy7;
    o_isp_mlsc_winy7.u32 = be_reg->isp_mlsc_winy7.u32;
    o_isp_mlsc_winy7.bits.isp_mlsc_deltay7 = uisp_mlsc_deltay7;
    be_reg->isp_mlsc_winy7.u32 = o_isp_mlsc_winy7.u32;
}
/*  description   set the value of the member isp_mlsc_winy7.isp_mlsc_invy7 */
/*  input         unsigned int uisp_mlsc_invy7  11 bits */
static __inline td_void isp_mlsc_invy7_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy7)
{
    u_isp_mlsc_winy7 o_isp_mlsc_winy7;
    o_isp_mlsc_winy7.u32 = be_reg->isp_mlsc_winy7.u32;
    o_isp_mlsc_winy7.bits.isp_mlsc_invy7 = uisp_mlsc_invy7;
    be_reg->isp_mlsc_winy7.u32 = o_isp_mlsc_winy7.u32;
}
/*  description   set the value of the member isp_mlsc_width_offset.isp_mlsc_width_offset */
/*  input         unsigned int uisp_mlsc_width_offset  12 bits */
static __inline td_void isp_mlsc_width_offset_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_width_offset)
{
    u_isp_mlsc_width_offset o_isp_mlsc_width_offset;
    o_isp_mlsc_width_offset.u32 = be_reg->isp_mlsc_width_offset.u32;
    o_isp_mlsc_width_offset.bits.isp_mlsc_width_offset = uisp_mlsc_width_offset;
    be_reg->isp_mlsc_width_offset.u32 = o_isp_mlsc_width_offset.u32;
}
/*  description   set the value of the member isp_mlsc_mesh.isp_mlsc_mesh_str */
/*  input         unsigned int uisp_mlsc_mesh_str  10 bits */
static __inline td_void isp_mlsc_mesh_str_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_mesh_str)
{
    u_isp_mlsc_mesh o_isp_mlsc_mesh;
    o_isp_mlsc_mesh.u32 = be_reg->isp_mlsc_mesh.u32;
    o_isp_mlsc_mesh.bits.isp_mlsc_mesh_str = uisp_mlsc_mesh_str;
    be_reg->isp_mlsc_mesh.u32 = o_isp_mlsc_mesh.u32;
}
/*  description   set the value of the member isp_mlsc_mesh.isp_mlsc_mesh_scale */
/*  input         unsigned int uisp_mlsc_mesh_scale  3 bits */
static __inline td_void isp_mlsc_mesh_scale_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_mesh_scale)
{
    u_isp_mlsc_mesh o_isp_mlsc_mesh;
    o_isp_mlsc_mesh.u32 = be_reg->isp_mlsc_mesh.u32;
    o_isp_mlsc_mesh.bits.isp_mlsc_mesh_scale = uisp_mlsc_mesh_scale;
    be_reg->isp_mlsc_mesh.u32 = o_isp_mlsc_mesh.u32;
}
/*  description   set the value of the member isp_mlsc_rgain_waddr.isp_mlsc_rgain_waddr */
/*  input         unsigned int uisp_mlsc_rgain_waddr  32 bits */
static __inline td_void isp_mlsc_rgain_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_rgain_waddr)
{
    be_reg->isp_mlsc_rgain_waddr.u32 = uisp_mlsc_rgain_waddr;
}

/*  description   set the value of the member isp_mlsc_rgain_wdata.isp_mlsc_rgain_wdata */
/*  input         unsigned int uisp_mlsc_rgain_wdata  10 bits */
static __inline td_void isp_mlsc_rgain_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_rgain_wdata)
{
    u_isp_mlsc_rgain_wdata o_isp_mlsc_rgain_wdata;
    o_isp_mlsc_rgain_wdata.u32 = be_reg->isp_mlsc_rgain_wdata.u32;
    o_isp_mlsc_rgain_wdata.bits.isp_mlsc_rgain_wdata = uisp_mlsc_rgain_wdata;
    be_reg->isp_mlsc_rgain_wdata.u32 = o_isp_mlsc_rgain_wdata.u32;
}
/*  description   set the value of the member isp_mlsc_rgain_raddr.isp_mlsc_rgain_raddr */
/*  input         unsigned int uisp_mlsc_rgain_raddr  32 bits */
static __inline td_void isp_mlsc_rgain_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_rgain_raddr)
{
    be_reg->isp_mlsc_rgain_raddr.u32 = uisp_mlsc_rgain_raddr;
}

/*  description   set the value of the member isp_mlsc_grgain_waddr.isp_mlsc_grgain_waddr */
/*  input         unsigned int uisp_mlsc_grgain_waddr  32 bits */
static __inline td_void isp_mlsc_grgain_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_grgain_waddr)
{
    be_reg->isp_mlsc_grgain_waddr.u32 = uisp_mlsc_grgain_waddr;
}

/*  description   set the value of the member isp_mlsc_grgain_wdata.isp_mlsc_grgain_wdata */
/*  input         unsigned int uisp_mlsc_grgain_wdata  10 bits */
static __inline td_void isp_mlsc_grgain_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_grgain_wdata)
{
    u_isp_mlsc_grgain_wdata o_isp_mlsc_grgain_wdata;
    o_isp_mlsc_grgain_wdata.u32 = be_reg->isp_mlsc_grgain_wdata.u32;
    o_isp_mlsc_grgain_wdata.bits.isp_mlsc_grgain_wdata = uisp_mlsc_grgain_wdata;
    be_reg->isp_mlsc_grgain_wdata.u32 = o_isp_mlsc_grgain_wdata.u32;
}
/*  description   set the value of the member isp_mlsc_grgain_raddr.isp_mlsc_grgain_raddr */
/*  input         unsigned int uisp_mlsc_grgain_raddr  32 bits */
static __inline td_void isp_mlsc_grgain_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_grgain_raddr)
{
    be_reg->isp_mlsc_grgain_raddr.u32 = uisp_mlsc_grgain_raddr;
}

/*  description   set the value of the member isp_mlsc_bgain_waddr.isp_mlsc_bgain_waddr */
/*  input         unsigned int uisp_mlsc_bgain_waddr  32 bits */
static __inline td_void isp_mlsc_bgain_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_bgain_waddr)
{
    be_reg->isp_mlsc_bgain_waddr.u32 = uisp_mlsc_bgain_waddr;
}

/*  description   set the value of the member isp_mlsc_bgain_wdata.isp_mlsc_bgain_wdata */
/*  input         unsigned int uisp_mlsc_bgain_wdata  10 bits */
static __inline td_void isp_mlsc_bgain_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_bgain_wdata)
{
    u_isp_mlsc_bgain_wdata o_isp_mlsc_bgain_wdata;
    o_isp_mlsc_bgain_wdata.u32 = be_reg->isp_mlsc_bgain_wdata.u32;
    o_isp_mlsc_bgain_wdata.bits.isp_mlsc_bgain_wdata = uisp_mlsc_bgain_wdata;
    be_reg->isp_mlsc_bgain_wdata.u32 = o_isp_mlsc_bgain_wdata.u32;
}
/*  description   set the value of the member isp_mlsc_bgain_raddr.isp_mlsc_bgain_raddr */
/*  input         unsigned int uisp_mlsc_bgain_raddr  32 bits */
static __inline td_void isp_mlsc_bgain_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_bgain_raddr)
{
    be_reg->isp_mlsc_bgain_raddr.u32 = uisp_mlsc_bgain_raddr;
}

/*  description   set the value of the member isp_mlsc_gbgain_waddr.isp_mlsc_gbgain_waddr */
/*  input         unsigned int uisp_mlsc_gbgain_waddr  32 bits */
static __inline td_void isp_mlsc_gbgain_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_gbgain_waddr)
{
    be_reg->isp_mlsc_gbgain_waddr.u32 = uisp_mlsc_gbgain_waddr;
}

/*  description   set the value of the member isp_mlsc_gbgain_wdata.isp_mlsc_gbgain_wdata */
/*  input         unsigned int uisp_mlsc_gbgain_wdata  10 bits */
static __inline td_void isp_mlsc_gbgain_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_gbgain_wdata)
{
    u_isp_mlsc_gbgain_wdata o_isp_mlsc_gbgain_wdata;
    o_isp_mlsc_gbgain_wdata.u32 = be_reg->isp_mlsc_gbgain_wdata.u32;
    o_isp_mlsc_gbgain_wdata.bits.isp_mlsc_gbgain_wdata = uisp_mlsc_gbgain_wdata;
    be_reg->isp_mlsc_gbgain_wdata.u32 = o_isp_mlsc_gbgain_wdata.u32;
}
/*  description   set the value of the member isp_mlsc_gbgain_raddr.isp_mlsc_gbgain_raddr */
/*  input         unsigned int uisp_mlsc_gbgain_raddr  32 bits */
static __inline td_void isp_mlsc_gbgain_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_gbgain_raddr)
{
    be_reg->isp_mlsc_gbgain_raddr.u32 = uisp_mlsc_gbgain_raddr;
}


static __inline td_void isp_mlsc_x16_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax16, td_u32 uisp_mlsc_invx16)
{
    u_isp_mlsc_winx16 o_isp_mlsc_winx16;
    o_isp_mlsc_winx16.u32 = be_reg->isp_mlsc_winx16.u32;
    o_isp_mlsc_winx16.bits.isp_mlsc_deltax16 = uisp_mlsc_deltax16;
    o_isp_mlsc_winx16.bits.isp_mlsc_invx16 = uisp_mlsc_invx16;
    be_reg->isp_mlsc_winx16.u32 = o_isp_mlsc_winx16.u32;
}

static __inline td_void isp_mlsc_x17_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax17, td_u32 uisp_mlsc_invx17)
{
    u_isp_mlsc_winx17 o_isp_mlsc_winx17;
    o_isp_mlsc_winx17.u32 = be_reg->isp_mlsc_winx17.u32;
    o_isp_mlsc_winx17.bits.isp_mlsc_deltax17 = uisp_mlsc_deltax17;
    o_isp_mlsc_winx17.bits.isp_mlsc_invx17 = uisp_mlsc_invx17;
    be_reg->isp_mlsc_winx17.u32 = o_isp_mlsc_winx17.u32;
}

static __inline td_void isp_mlsc_x18_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax18, td_u32 uisp_mlsc_invx18)
{
    u_isp_mlsc_winx18 o_isp_mlsc_winx18;
    o_isp_mlsc_winx18.u32 = be_reg->isp_mlsc_winx18.u32;
    o_isp_mlsc_winx18.bits.isp_mlsc_deltax18 = uisp_mlsc_deltax18;
    o_isp_mlsc_winx18.bits.isp_mlsc_invx18 = uisp_mlsc_invx18;
    be_reg->isp_mlsc_winx18.u32 = o_isp_mlsc_winx18.u32;
}

static __inline td_void isp_mlsc_x19_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax19, td_u32 uisp_mlsc_invx19)
{
    u_isp_mlsc_winx19 o_isp_mlsc_winx19;
    o_isp_mlsc_winx19.u32 = be_reg->isp_mlsc_winx19.u32;
    o_isp_mlsc_winx19.bits.isp_mlsc_deltax19 = uisp_mlsc_deltax19;
    o_isp_mlsc_winx19.bits.isp_mlsc_invx19 = uisp_mlsc_invx19;
    be_reg->isp_mlsc_winx19.u32 = o_isp_mlsc_winx19.u32;
}

static __inline td_void isp_mlsc_x20_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax20, td_u32 uisp_mlsc_invx20)
{
    u_isp_mlsc_winx20 o_isp_mlsc_winx20;
    o_isp_mlsc_winx20.u32 = be_reg->isp_mlsc_winx20.u32;
    o_isp_mlsc_winx20.bits.isp_mlsc_deltax20 = uisp_mlsc_deltax20;
    o_isp_mlsc_winx20.bits.isp_mlsc_invx20 = uisp_mlsc_invx20;
    be_reg->isp_mlsc_winx20.u32 = o_isp_mlsc_winx20.u32;
}

static __inline td_void isp_mlsc_x21_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax21, td_u32 uisp_mlsc_invx21)
{
    u_isp_mlsc_winx21 o_isp_mlsc_winx21;
    o_isp_mlsc_winx21.u32 = be_reg->isp_mlsc_winx21.u32;
    o_isp_mlsc_winx21.bits.isp_mlsc_deltax21 = uisp_mlsc_deltax21;
    o_isp_mlsc_winx21.bits.isp_mlsc_invx21 = uisp_mlsc_invx21;
    be_reg->isp_mlsc_winx21.u32 = o_isp_mlsc_winx21.u32;
}


static __inline td_void isp_mlsc_x22_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax22, td_u32 uisp_mlsc_invx22)
{
    u_isp_mlsc_winx22 o_isp_mlsc_winx22;
    o_isp_mlsc_winx22.u32 = be_reg->isp_mlsc_winx22.u32;
    o_isp_mlsc_winx22.bits.isp_mlsc_deltax22 = uisp_mlsc_deltax22;
    o_isp_mlsc_winx22.bits.isp_mlsc_invx22 = uisp_mlsc_invx22;
    be_reg->isp_mlsc_winx22.u32 = o_isp_mlsc_winx22.u32;
}

static __inline td_void isp_mlsc_x23_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax23, td_u32 uisp_mlsc_invx23)
{
    u_isp_mlsc_winx23 o_isp_mlsc_winx23;
    o_isp_mlsc_winx23.u32 = be_reg->isp_mlsc_winx23.u32;
    o_isp_mlsc_winx23.bits.isp_mlsc_deltax23 = uisp_mlsc_deltax23;
    o_isp_mlsc_winx23.bits.isp_mlsc_invx23 = uisp_mlsc_invx23;
    be_reg->isp_mlsc_winx23.u32 = o_isp_mlsc_winx23.u32;
}

static __inline td_void isp_mlsc_x24_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax24, td_u32 uisp_mlsc_invx24)
{
    u_isp_mlsc_winx24 o_isp_mlsc_winx24;
    o_isp_mlsc_winx24.u32 = be_reg->isp_mlsc_winx24.u32;
    o_isp_mlsc_winx24.bits.isp_mlsc_deltax24 = uisp_mlsc_deltax24;
    o_isp_mlsc_winx24.bits.isp_mlsc_invx24 = uisp_mlsc_invx24;
    be_reg->isp_mlsc_winx24.u32 = o_isp_mlsc_winx24.u32;
}

static __inline td_void isp_mlsc_x25_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax25, td_u32 uisp_mlsc_invx25)
{
    u_isp_mlsc_winx25 o_isp_mlsc_winx25;
    o_isp_mlsc_winx25.u32 = be_reg->isp_mlsc_winx25.u32;
    o_isp_mlsc_winx25.bits.isp_mlsc_deltax25 = uisp_mlsc_deltax25;
    o_isp_mlsc_winx25.bits.isp_mlsc_invx25 = uisp_mlsc_invx25;
    be_reg->isp_mlsc_winx25.u32 = o_isp_mlsc_winx25.u32;
}

static __inline td_void isp_mlsc_x26_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax26, td_u32 uisp_mlsc_invx26)
{
    u_isp_mlsc_winx26 o_isp_mlsc_winx26;
    o_isp_mlsc_winx26.u32 = be_reg->isp_mlsc_winx26.u32;
    o_isp_mlsc_winx26.bits.isp_mlsc_deltax26 = uisp_mlsc_deltax26;
    o_isp_mlsc_winx26.bits.isp_mlsc_invx26 = uisp_mlsc_invx26;
    be_reg->isp_mlsc_winx26.u32 = o_isp_mlsc_winx26.u32;
}

static __inline td_void isp_mlsc_x27_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax27, td_u32 uisp_mlsc_invx27)
{
    u_isp_mlsc_winx27 o_isp_mlsc_winx27;
    o_isp_mlsc_winx27.u32 = be_reg->isp_mlsc_winx27.u32;
    o_isp_mlsc_winx27.bits.isp_mlsc_deltax27 = uisp_mlsc_deltax27;
    o_isp_mlsc_winx27.bits.isp_mlsc_invx27 = uisp_mlsc_invx27;
    be_reg->isp_mlsc_winx27.u32 = o_isp_mlsc_winx27.u32;
}

static __inline td_void isp_mlsc_x28_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax28, td_u32 uisp_mlsc_invx28)
{
    u_isp_mlsc_winx28 o_isp_mlsc_winx28;
    o_isp_mlsc_winx28.u32 = be_reg->isp_mlsc_winx28.u32;
    o_isp_mlsc_winx28.bits.isp_mlsc_deltax28 = uisp_mlsc_deltax28;
    o_isp_mlsc_winx28.bits.isp_mlsc_invx28 = uisp_mlsc_invx28;
    be_reg->isp_mlsc_winx28.u32 = o_isp_mlsc_winx28.u32;
}

static __inline td_void isp_mlsc_x29_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax29, td_u32 uisp_mlsc_invx29)
{
    u_isp_mlsc_winx29 o_isp_mlsc_winx29;
    o_isp_mlsc_winx29.u32 = be_reg->isp_mlsc_winx29.u32;
    o_isp_mlsc_winx29.bits.isp_mlsc_deltax29 = uisp_mlsc_deltax29;
    o_isp_mlsc_winx29.bits.isp_mlsc_invx29 = uisp_mlsc_invx29;
    be_reg->isp_mlsc_winx29.u32 = o_isp_mlsc_winx29.u32;
}

static __inline td_void isp_mlsc_x30_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax30, td_u32 uisp_mlsc_invx30)
{
    u_isp_mlsc_winx30 o_isp_mlsc_winx30;
    o_isp_mlsc_winx30.u32 = be_reg->isp_mlsc_winx30.u32;
    o_isp_mlsc_winx30.bits.isp_mlsc_deltax30 = uisp_mlsc_deltax30;
    o_isp_mlsc_winx30.bits.isp_mlsc_invx30 = uisp_mlsc_invx30;
    be_reg->isp_mlsc_winx30.u32 = o_isp_mlsc_winx30.u32;
}

static __inline td_void isp_mlsc_x31_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltax31, td_u32 uisp_mlsc_invx31)
{
    u_isp_mlsc_winx31 o_isp_mlsc_winx31;
    o_isp_mlsc_winx31.u32 = be_reg->isp_mlsc_winx31.u32;
    o_isp_mlsc_winx31.bits.isp_mlsc_deltax31 = uisp_mlsc_deltax31;
    o_isp_mlsc_winx31.bits.isp_mlsc_invx31 = uisp_mlsc_invx31;
    be_reg->isp_mlsc_winx31.u32 = o_isp_mlsc_winx31.u32;
}

/*  description : set the value of the member isp_mlsc_winy8.isp_mlsc_deltay8 */
/*  input       : unsigned int uisp_mlsc_deltay8: 8 bits */
static __inline td_void isp_mlsc_deltay8_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_deltay8)
{
    u_isp_mlsc_winy8 o_isp_mlsc_winy8;
    o_isp_mlsc_winy8.u32 = be_reg->isp_mlsc_winy8.u32;
    o_isp_mlsc_winy8.bits.isp_mlsc_deltay8 = uisp_mlsc_deltay8;
    be_reg->isp_mlsc_winy8.u32 = o_isp_mlsc_winy8.u32;
}

/*  description : set the value of the member isp_mlsc_winy8.isp_mlsc_invy8 */
/*  input       : unsigned int uisp_mlsc_invy8: 11 bits */
static __inline td_void isp_mlsc_invy8_write(isp_be_reg_type *be_reg, td_u32 uisp_mlsc_invy8)
{
    u_isp_mlsc_winy8 o_isp_mlsc_winy8;
    o_isp_mlsc_winy8.u32 = be_reg->isp_mlsc_winy8.u32;
    o_isp_mlsc_winy8.bits.isp_mlsc_invy8 = uisp_mlsc_invy8;
    be_reg->isp_mlsc_winy8.u32 = o_isp_mlsc_winy8.u32;
}

/*  description : set the value of the member isp_wdr_ctrl.isp_wdr_fusion_data_mode */
/*  input       : unsigned int uisp_wdr_fusion_data_mode: 2 bits */
static __inline td_void isp_wdr_fusion_data_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_data_mode)
{
    u_isp_wdr_ctrl o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = be_reg->isp_wdr_ctrl.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_fusion_data_mode = uisp_wdr_fusion_data_mode;
    be_reg->isp_wdr_ctrl.u32 = o_isp_wdr_ctrl.u32;
}

/*  description : set the value of the member isp_wdr_ctrl.isp_wdr_fusionmode */
/*  input       : unsigned int uisp_wdr_fusionmode: 1 bits */
static __inline td_void isp_wdr_fusionmode_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusionmode)
{
    u_isp_wdr_ctrl o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = be_reg->isp_wdr_ctrl.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_fusionmode = uisp_wdr_fusionmode;
    be_reg->isp_wdr_ctrl.u32 = o_isp_wdr_ctrl.u32;
}

/*  description : set the value of the member isp_wdr_ctrl.isp_wdr_erosion_en */
/*  input       : unsigned int uisp_wdr_erosion_en: 1 bits */
static __inline td_void isp_wdr_erosion_en_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_erosion_en)
{
    u_isp_wdr_ctrl o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = be_reg->isp_wdr_ctrl.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_erosion_en = uisp_wdr_erosion_en;
    be_reg->isp_wdr_ctrl.u32 = o_isp_wdr_ctrl.u32;
}

/*  description : set the value of the member isp_wdr_ctrl.isp_wdr_mdt_en */
/*  input       : unsigned int uisp_wdr_mdt_en: 1 bits */
static __inline td_void isp_wdr_mdt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mdt_en)
{
    u_isp_wdr_ctrl o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = be_reg->isp_wdr_ctrl.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_mdt_en = uisp_wdr_mdt_en;
    be_reg->isp_wdr_ctrl.u32 = o_isp_wdr_ctrl.u32;
}

/*  description : set the value of the member isp_wdr_ctrl.isp_wdr_grayscale_mode */
/*  input       : unsigned int uisp_wdr_grayscale_mode: 1 bits */
static __inline td_void isp_wdr_grayscale_mode_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_grayscale_mode)
{
    u_isp_wdr_ctrl o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.u32 = be_reg->isp_wdr_ctrl.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_grayscale_mode = uisp_wdr_grayscale_mode;
    be_reg->isp_wdr_ctrl.u32 = o_isp_wdr_ctrl.u32;
}

/*  description : set the value of the member isp_wdr_f0_inblc0.isp_wdr_f0_inblc_gr */
/*  input       : unsigned int uisp_wdr_f0_inblc_gr: 13 bits */
static __inline td_void isp_wdr_f0_inblc_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f0_inblc_gr)
{
    u_isp_wdr_f0_inblc0 o_isp_wdr_f0_inblc0;
    o_isp_wdr_f0_inblc0.u32 = be_reg->isp_wdr_f0_inblc0.u32;
    o_isp_wdr_f0_inblc0.bits.isp_wdr_f0_inblc_gr = uisp_wdr_f0_inblc_gr;
    be_reg->isp_wdr_f0_inblc0.u32 = o_isp_wdr_f0_inblc0.u32;
}

/*  description : set the value of the member isp_wdr_f0_inblc0.isp_wdr_f0_inblc_r */
/*  input       : unsigned int uisp_wdr_f0_inblc_r: 13 bits */
static __inline td_void isp_wdr_f0_inblc_r_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f0_inblc_r)
{
    u_isp_wdr_f0_inblc0 o_isp_wdr_f0_inblc0;
    o_isp_wdr_f0_inblc0.u32 = be_reg->isp_wdr_f0_inblc0.u32;
    o_isp_wdr_f0_inblc0.bits.isp_wdr_f0_inblc_r = uisp_wdr_f0_inblc_r;
    be_reg->isp_wdr_f0_inblc0.u32 = o_isp_wdr_f0_inblc0.u32;
}

/*  description : set the value of the member isp_wdr_f0_inblc1.isp_wdr_f0_inblc_b */
/*  input       : unsigned int uisp_wdr_f0_inblc_b: 13 bits */
static __inline td_void isp_wdr_f0_inblc_b_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f0_inblc_b)
{
    u_isp_wdr_f0_inblc1 o_isp_wdr_f0_inblc1;
    o_isp_wdr_f0_inblc1.u32 = be_reg->isp_wdr_f0_inblc1.u32;
    o_isp_wdr_f0_inblc1.bits.isp_wdr_f0_inblc_b = uisp_wdr_f0_inblc_b;
    be_reg->isp_wdr_f0_inblc1.u32 = o_isp_wdr_f0_inblc1.u32;
}

/*  description : set the value of the member isp_wdr_f0_inblc1.isp_wdr_f0_inblc_gb */
/*  input       : unsigned int uisp_wdr_f0_inblc_gb: 13 bits */
static __inline td_void isp_wdr_f0_inblc_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f0_inblc_gb)
{
    u_isp_wdr_f0_inblc1 o_isp_wdr_f0_inblc1;
    o_isp_wdr_f0_inblc1.u32 = be_reg->isp_wdr_f0_inblc1.u32;
    o_isp_wdr_f0_inblc1.bits.isp_wdr_f0_inblc_gb = uisp_wdr_f0_inblc_gb;
    be_reg->isp_wdr_f0_inblc1.u32 = o_isp_wdr_f0_inblc1.u32;
}

/*  description : set the value of the member isp_wdr_f1_inblc0.isp_wdr_f1_inblc_gr */
/*  input       : unsigned int uisp_wdr_f1_inblc_gr: 13 bits */
static __inline td_void isp_wdr_f1_inblc_gr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f1_inblc_gr)
{
    u_isp_wdr_f1_inblc0 o_isp_wdr_f1_inblc0;
    o_isp_wdr_f1_inblc0.u32 = be_reg->isp_wdr_f1_inblc0.u32;
    o_isp_wdr_f1_inblc0.bits.isp_wdr_f1_inblc_gr = uisp_wdr_f1_inblc_gr;
    be_reg->isp_wdr_f1_inblc0.u32 = o_isp_wdr_f1_inblc0.u32;
}

/*  description : set the value of the member isp_wdr_f1_inblc0.isp_wdr_f1_inblc_r */
/*  input       : unsigned int uisp_wdr_f1_inblc_r: 13 bits */
static __inline td_void isp_wdr_f1_inblc_r_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f1_inblc_r)
{
    u_isp_wdr_f1_inblc0 o_isp_wdr_f1_inblc0;
    o_isp_wdr_f1_inblc0.u32 = be_reg->isp_wdr_f1_inblc0.u32;
    o_isp_wdr_f1_inblc0.bits.isp_wdr_f1_inblc_r = uisp_wdr_f1_inblc_r;
    be_reg->isp_wdr_f1_inblc0.u32 = o_isp_wdr_f1_inblc0.u32;
}

/*  description : set the value of the member isp_wdr_f1_inblc1.isp_wdr_f1_inblc_b */
/*  input       : unsigned int uisp_wdr_f1_inblc_b: 13 bits */
static __inline td_void isp_wdr_f1_inblc_b_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f1_inblc_b)
{
    u_isp_wdr_f1_inblc1 o_isp_wdr_f1_inblc1;
    o_isp_wdr_f1_inblc1.u32 = be_reg->isp_wdr_f1_inblc1.u32;
    o_isp_wdr_f1_inblc1.bits.isp_wdr_f1_inblc_b = uisp_wdr_f1_inblc_b;
    be_reg->isp_wdr_f1_inblc1.u32 = o_isp_wdr_f1_inblc1.u32;
}

/*  description : set the value of the member isp_wdr_f1_inblc1.isp_wdr_f1_inblc_gb */
/*  input       : unsigned int uisp_wdr_f1_inblc_gb: 13 bits */
static __inline td_void isp_wdr_f1_inblc_gb_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f1_inblc_gb)
{
    u_isp_wdr_f1_inblc1 o_isp_wdr_f1_inblc1;
    o_isp_wdr_f1_inblc1.u32 = be_reg->isp_wdr_f1_inblc1.u32;
    o_isp_wdr_f1_inblc1.bits.isp_wdr_f1_inblc_gb = uisp_wdr_f1_inblc_gb;
    be_reg->isp_wdr_f1_inblc1.u32 = o_isp_wdr_f1_inblc1.u32;
}

/*  description : set the value of the member isp_wdr_outblc.isp_wdr_outblc */
/*  input       : unsigned int uisp_wdr_outblc: 18 bits */
static __inline td_void isp_wdr_outblc_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_outblc)
{
    u_isp_wdr_outblc o_isp_wdr_outblc;
    o_isp_wdr_outblc.u32 = be_reg->isp_wdr_outblc.u32;
    o_isp_wdr_outblc.bits.isp_wdr_outblc = uisp_wdr_outblc;
    be_reg->isp_wdr_outblc.u32 = o_isp_wdr_outblc.u32;
}

/*  description : set the value of the member isp_wdr_outblc.isp_wdr_bsaveblc */
/*  input       : unsigned int uisp_wdr_bsaveblc: 1 bits */
static __inline td_void isp_wdr_bsaveblc_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_bsaveblc)
{
    u_isp_wdr_outblc o_isp_wdr_outblc;
    o_isp_wdr_outblc.u32 = be_reg->isp_wdr_outblc.u32;
    o_isp_wdr_outblc.bits.isp_wdr_bsaveblc = uisp_wdr_bsaveblc;
    be_reg->isp_wdr_outblc.u32 = o_isp_wdr_outblc.u32;
}

/*  description : set the value of the member isp_wdr_expovalue.isp_wdr_expovalue1 */
/*  input       : unsigned int uisp_wdr_expovalue1: 12 bits */
static __inline td_void isp_wdr_expovalue1_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_expovalue1)
{
    u_isp_wdr_expovalue o_isp_wdr_expovalue;
    o_isp_wdr_expovalue.u32 = be_reg->isp_wdr_expovalue.u32;
    o_isp_wdr_expovalue.bits.isp_wdr_expovalue1 = uisp_wdr_expovalue1;
    be_reg->isp_wdr_expovalue.u32 = o_isp_wdr_expovalue.u32;
}

/*  description : set the value of the member isp_wdr_expovalue.isp_wdr_expovalue0 */
/*  input       : unsigned int uisp_wdr_expovalue0: 12 bits */
static __inline td_void isp_wdr_expovalue0_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_expovalue0)
{
    u_isp_wdr_expovalue o_isp_wdr_expovalue;
    o_isp_wdr_expovalue.u32 = be_reg->isp_wdr_expovalue.u32;
    o_isp_wdr_expovalue.bits.isp_wdr_expovalue0 = uisp_wdr_expovalue0;
    be_reg->isp_wdr_expovalue.u32 = o_isp_wdr_expovalue.u32;
}

/*  description : set the value of the member isp_wdr_exporratio.isp_wdr_exporratio0 */
/*  input       : unsigned int uisp_wdr_exporratio0: 10 bits */
static __inline td_void isp_wdr_exporratio0_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_exporratio0)
{
    u_isp_wdr_exporratio o_isp_wdr_exporratio;
    o_isp_wdr_exporratio.u32 = be_reg->isp_wdr_exporratio.u32;
    o_isp_wdr_exporratio.bits.isp_wdr_exporratio0 = uisp_wdr_exporratio0;
    be_reg->isp_wdr_exporratio.u32 = o_isp_wdr_exporratio.u32;
}

/*  description : set the value of the member isp_wdr_blc_comp.isp_wdr_blc_comp0 */
/*  input       : unsigned int uisp_wdr_blc_comp0: 26 bits */
static __inline td_void isp_wdr_blc_comp0_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_blc_comp0)
{
    u_isp_wdr_blc_comp o_isp_wdr_blc_comp;
    o_isp_wdr_blc_comp.u32 = be_reg->isp_wdr_blc_comp.u32;
    o_isp_wdr_blc_comp.bits.isp_wdr_blc_comp0 = uisp_wdr_blc_comp0;
    be_reg->isp_wdr_blc_comp.u32 = o_isp_wdr_blc_comp.u32;
}

/*  description : set the value of the member isp_wdr_maxratio.isp_wdr_maxratio */
/*  input       : unsigned int uisp_wdr_maxratio: 15 bits */
static __inline td_void isp_wdr_maxratio_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_maxratio)
{
    u_isp_wdr_maxratio o_isp_wdr_maxratio;
    o_isp_wdr_maxratio.u32 = be_reg->isp_wdr_maxratio.u32;
    o_isp_wdr_maxratio.bits.isp_wdr_maxratio = uisp_wdr_maxratio;
    be_reg->isp_wdr_maxratio.u32 = o_isp_wdr_maxratio.u32;
}

/*  description : set the value of the member isp_wdr_saturate_thr.isp_wdr_saturate_thr */
/*  input       : unsigned int uisp_wdr_saturate_thr: 12 bits */
static __inline td_void isp_wdr_saturate_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_saturate_thr)
{
    u_isp_wdr_saturate_thr o_isp_wdr_saturate_thr;
    o_isp_wdr_saturate_thr.u32 = be_reg->isp_wdr_saturate_thr.u32;
    o_isp_wdr_saturate_thr.bits.isp_wdr_saturate_thr = uisp_wdr_saturate_thr;
    be_reg->isp_wdr_saturate_thr.u32 = o_isp_wdr_saturate_thr.u32;
}

/*  description : set the value of the member isp_wdr_still_thr.isp_wdr_f0_still_thr */
/*  input       : unsigned int uisp_wdr_f0_still_thr: 12 bits */
static __inline td_void isp_wdr_f0_still_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_f0_still_thr)
{
    u_isp_wdr_still_thr o_isp_wdr_still_thr;
    o_isp_wdr_still_thr.u32 = be_reg->isp_wdr_still_thr.u32;
    o_isp_wdr_still_thr.bits.isp_wdr_f0_still_thr = uisp_wdr_f0_still_thr;
    be_reg->isp_wdr_still_thr.u32 = o_isp_wdr_still_thr.u32;
}

/*  description : set the value of the member isp_wdr_mdtlbld.isp_wdr_mdtlbld */
/*  input       : unsigned int uisp_wdr_mdtlbld: 8 bits */
static __inline td_void isp_wdr_mdtlbld_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mdtlbld)
{
    u_isp_wdr_mdtlbld o_isp_wdr_mdtlbld;
    o_isp_wdr_mdtlbld.u32 = be_reg->isp_wdr_mdtlbld.u32;
    o_isp_wdr_mdtlbld.bits.isp_wdr_mdtlbld = uisp_wdr_mdtlbld;
    be_reg->isp_wdr_mdtlbld.u32 = o_isp_wdr_mdtlbld.u32;
}

/*  description : set the value of the member isp_wdr_mdt_thr.isp_wdr_mdt_full_thr */
/*  input       : unsigned int uisp_wdr_mdt_full_thr: 8 bits */
static __inline td_void isp_wdr_mdt_full_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mdt_full_thr)
{
    u_isp_wdr_mdt_thr o_isp_wdr_mdt_thr;
    o_isp_wdr_mdt_thr.u32 = be_reg->isp_wdr_mdt_thr.u32;
    o_isp_wdr_mdt_thr.bits.isp_wdr_mdt_full_thr = uisp_wdr_mdt_full_thr;
    be_reg->isp_wdr_mdt_thr.u32 = o_isp_wdr_mdt_thr.u32;
}

/*  description : set the value of the member isp_wdr_mdt_thr.isp_wdr_mdt_still_thr */
/*  input       : unsigned int uisp_wdr_mdt_still_thr: 8 bits */
static __inline td_void isp_wdr_mdt_still_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mdt_still_thr)
{
    u_isp_wdr_mdt_thr o_isp_wdr_mdt_thr;
    o_isp_wdr_mdt_thr.u32 = be_reg->isp_wdr_mdt_thr.u32;
    o_isp_wdr_mdt_thr.bits.isp_wdr_mdt_still_thr = uisp_wdr_mdt_still_thr;
    be_reg->isp_wdr_mdt_thr.u32 = o_isp_wdr_mdt_thr.u32;
}

/*  description : set the value of the member isp_wdr_forcelong_para.isp_wdr_forcelong_high_thd */
/*  input       : unsigned int uisp_wdr_forcelong_high_thd: 12 bits */
static __inline td_void isp_wdr_forcelong_high_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_forcelong_high_thd)
{
    u_isp_wdr_forcelong_para o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = be_reg->isp_wdr_forcelong_para.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_high_thd = uisp_wdr_forcelong_high_thd;
    be_reg->isp_wdr_forcelong_para.u32 = o_isp_wdr_forcelong_para.u32;
}

/*  description : set the value of the member isp_wdr_forcelong_para.isp_wdr_forcelong_low_thd */
/*  input       : unsigned int uisp_wdr_forcelong_low_thd: 12 bits */
static __inline td_void isp_wdr_forcelong_low_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_forcelong_low_thd)
{
    u_isp_wdr_forcelong_para o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = be_reg->isp_wdr_forcelong_para.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_low_thd = uisp_wdr_forcelong_low_thd;
    be_reg->isp_wdr_forcelong_para.u32 = o_isp_wdr_forcelong_para.u32;
}

/*  description : set the value of the member isp_wdr_forcelong_para.isp_wdr_forcelong_smooth_en */
/*  input       : unsigned int uisp_wdr_forcelong_smooth_en: 1 bits */
static __inline td_void isp_wdr_forcelong_smooth_en_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_forcelong_smooth_en)
{
    u_isp_wdr_forcelong_para o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = be_reg->isp_wdr_forcelong_para.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_smooth_en = uisp_wdr_forcelong_smooth_en;
    be_reg->isp_wdr_forcelong_para.u32 = o_isp_wdr_forcelong_para.u32;
}

/*  description : set the value of the member isp_wdr_forcelong_para.isp_wdr_forcelong_en */
/*  input       : unsigned int uisp_wdr_forcelong_en: 1 bits */
static __inline td_void isp_wdr_forcelong_en_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_forcelong_en)
{
    u_isp_wdr_forcelong_para o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.u32 = be_reg->isp_wdr_forcelong_para.u32;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_en = uisp_wdr_forcelong_en;
    be_reg->isp_wdr_forcelong_para.u32 = o_isp_wdr_forcelong_para.u32;
}

/*  description : set the value of the member isp_wdr_forcelong_slope.isp_wdr_forcelong_slope */
/*  input       : unsigned int uisp_wdr_forcelong_slope: 14 bits */
static __inline td_void isp_wdr_forcelong_slope_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_forcelong_slope)
{
    u_isp_wdr_forcelong_slope o_isp_wdr_forcelong_slope;
    o_isp_wdr_forcelong_slope.u32 = be_reg->isp_wdr_forcelong_slope.u32;
    o_isp_wdr_forcelong_slope.bits.isp_wdr_forcelong_slope = uisp_wdr_forcelong_slope;
    be_reg->isp_wdr_forcelong_slope.u32 = o_isp_wdr_forcelong_slope.u32;
}

/*  description : set the value of the member isp_wdr_mdt_nosf_thr.isp_wdr_mdt_nosf_hig_thr */
/*  input       : unsigned int uisp_wdr_mdt_nosf_hig_thr: 11 bits */
static __inline td_void isp_wdr_mdt_nosf_hig_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mdt_nosf_hig_thr)
{
    u_isp_wdr_mdt_nosf_thr o_isp_wdr_mdt_nosf_thr;
    o_isp_wdr_mdt_nosf_thr.u32 = be_reg->isp_wdr_mdt_nosf_thr.u32;
    o_isp_wdr_mdt_nosf_thr.bits.isp_wdr_mdt_nosf_hig_thr = uisp_wdr_mdt_nosf_hig_thr;
    be_reg->isp_wdr_mdt_nosf_thr.u32 = o_isp_wdr_mdt_nosf_thr.u32;
}

/*  description : set the value of the member isp_wdr_mdt_nosf_thr.isp_wdr_mdt_nosf_low_thr */
/*  input       : unsigned int uisp_wdr_mdt_nosf_low_thr: 11 bits */
static __inline td_void isp_wdr_mdt_nosf_low_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mdt_nosf_low_thr)
{
    u_isp_wdr_mdt_nosf_thr o_isp_wdr_mdt_nosf_thr;
    o_isp_wdr_mdt_nosf_thr.u32 = be_reg->isp_wdr_mdt_nosf_thr.u32;
    o_isp_wdr_mdt_nosf_thr.bits.isp_wdr_mdt_nosf_low_thr = uisp_wdr_mdt_nosf_low_thr;
    be_reg->isp_wdr_mdt_nosf_thr.u32 = o_isp_wdr_mdt_nosf_thr.u32;
}

/*  description : set the value of the member isp_wdr_gain_sum_thr.isp_wdr_gain_sum_hig_thr */
/*  input       : unsigned int uisp_wdr_gain_sum_hig_thr: 8 bits */
static __inline td_void isp_wdr_gain_sum_hig_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_gain_sum_hig_thr)
{
    u_isp_wdr_gain_sum_thr o_isp_wdr_gain_sum_thr;
    o_isp_wdr_gain_sum_thr.u32 = be_reg->isp_wdr_gain_sum_thr.u32;
    o_isp_wdr_gain_sum_thr.bits.isp_wdr_gain_sum_hig_thr = uisp_wdr_gain_sum_hig_thr;
    be_reg->isp_wdr_gain_sum_thr.u32 = o_isp_wdr_gain_sum_thr.u32;
}

/*  description : set the value of the member isp_wdr_gain_sum_thr.isp_wdr_gain_sum_low_thr */
/*  input       : unsigned int uisp_wdr_gain_sum_low_thr: 8 bits */
static __inline td_void isp_wdr_gain_sum_low_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_gain_sum_low_thr)
{
    u_isp_wdr_gain_sum_thr o_isp_wdr_gain_sum_thr;
    o_isp_wdr_gain_sum_thr.u32 = be_reg->isp_wdr_gain_sum_thr.u32;
    o_isp_wdr_gain_sum_thr.bits.isp_wdr_gain_sum_low_thr = uisp_wdr_gain_sum_low_thr;
    be_reg->isp_wdr_gain_sum_thr.u32 = o_isp_wdr_gain_sum_thr.u32;
}

/*  description : set the value of the member isp_wdr_shortexpo_chk.isp_wdr_shortchk_thd */
/*  input       : unsigned int uisp_wdr_shortchk_thd: 12 bits */
static __inline td_void isp_wdr_shortchk_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_shortchk_thd)
{
    u_isp_wdr_shortexpo_chk o_isp_wdr_shortexpo_chk;
    o_isp_wdr_shortexpo_chk.u32 = be_reg->isp_wdr_shortexpo_chk.u32;
    o_isp_wdr_shortexpo_chk.bits.isp_wdr_shortchk_thd = uisp_wdr_shortchk_thd;
    be_reg->isp_wdr_shortexpo_chk.u32 = o_isp_wdr_shortexpo_chk.u32;
}

/*  description : set the value of the member isp_wdr_shortexpo_chk.isp_wdr_shortexpo_chk */
/*  input       : unsigned int uisp_wdr_shortexpo_chk: 1 bits */
static __inline td_void isp_wdr_shortexpo_chk_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_shortexpo_chk)
{
    u_isp_wdr_shortexpo_chk o_isp_wdr_shortexpo_chk;
    o_isp_wdr_shortexpo_chk.u32 = be_reg->isp_wdr_shortexpo_chk.u32;
    o_isp_wdr_shortexpo_chk.bits.isp_wdr_shortexpo_chk = uisp_wdr_shortexpo_chk;
    be_reg->isp_wdr_shortexpo_chk.u32 = o_isp_wdr_shortexpo_chk.u32;
}

/*  description : set the value of the member isp_wdr_pix_avg_diff.isp_wdr_pixel_avg_max_diff */
/*  input       : unsigned int uisp_wdr_pixel_avg_max_diff: 12 bits */
static __inline td_void isp_wdr_pixel_avg_max_diff_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_pixel_avg_max_diff)
{
    u_isp_wdr_pix_avg_diff o_isp_wdr_pix_avg_diff;
    o_isp_wdr_pix_avg_diff.u32 = be_reg->isp_wdr_pix_avg_diff.u32;
    o_isp_wdr_pix_avg_diff.bits.isp_wdr_pixel_avg_max_diff = uisp_wdr_pixel_avg_max_diff;
    be_reg->isp_wdr_pix_avg_diff.u32 = o_isp_wdr_pix_avg_diff.u32;
}

/*  description : set the value of the member isp_wdr_mask_similar.isp_wdr_mask_similar_cnt */
/*  input       : unsigned int uisp_wdr_mask_similar_cnt: 4 bits */
static __inline td_void isp_wdr_mask_similar_cnt_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mask_similar_cnt)
{
    u_isp_wdr_mask_similar o_isp_wdr_mask_similar;
    o_isp_wdr_mask_similar.u32 = be_reg->isp_wdr_mask_similar.u32;
    o_isp_wdr_mask_similar.bits.isp_wdr_mask_similar_cnt = uisp_wdr_mask_similar_cnt;
    be_reg->isp_wdr_mask_similar.u32 = o_isp_wdr_mask_similar.u32;
}

/*  description : set the value of the member isp_wdr_mask_similar.isp_wdr_mask_similar_thr */
/*  input       : unsigned int uisp_wdr_mask_similar_thr: 6 bits */
static __inline td_void isp_wdr_mask_similar_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_mask_similar_thr)
{
    u_isp_wdr_mask_similar o_isp_wdr_mask_similar;
    o_isp_wdr_mask_similar.u32 = be_reg->isp_wdr_mask_similar.u32;
    o_isp_wdr_mask_similar.bits.isp_wdr_mask_similar_thr = uisp_wdr_mask_similar_thr;
    be_reg->isp_wdr_mask_similar.u32 = o_isp_wdr_mask_similar.u32;
}

/*  description : set the value of the member isp_wdr_wgtidx_blendratio.isp_wdr_bldrlhfidx */
/*  input       : unsigned int uisp_wdr_bldrlhfidx: 5 bits */
static __inline td_void isp_wdr_bldrlhfidx_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_bldrlhfidx)
{
    u_isp_wdr_wgtidx_blendratio o_isp_wdr_wgtidx_blendratio;
    o_isp_wdr_wgtidx_blendratio.u32 = be_reg->isp_wdr_wgtidx_blendratio.u32;
    o_isp_wdr_wgtidx_blendratio.bits.isp_wdr_bldrlhfidx = uisp_wdr_bldrlhfidx;
    be_reg->isp_wdr_wgtidx_blendratio.u32 = o_isp_wdr_wgtidx_blendratio.u32;
}

/*  description : set the value of the member isp_wdr_wgtidx_thr.isp_wdr_long_thr */
/*  input       : unsigned int uisp_wdr_long_thr: 12 bits */
static __inline td_void isp_wdr_long_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_long_thr)
{
    u_isp_wdr_wgtidx_thr o_isp_wdr_wgtidx_thr;
    o_isp_wdr_wgtidx_thr.u32 = be_reg->isp_wdr_wgtidx_thr.u32;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_long_thr = uisp_wdr_long_thr;
    be_reg->isp_wdr_wgtidx_thr.u32 = o_isp_wdr_wgtidx_thr.u32;
}

/*  description : set the value of the member isp_wdr_wgtidx_thr.isp_wdr_short_thr */
/*  input       : unsigned int uisp_wdr_short_thr: 12 bits */
static __inline td_void isp_wdr_short_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_short_thr)
{
    u_isp_wdr_wgtidx_thr o_isp_wdr_wgtidx_thr;
    o_isp_wdr_wgtidx_thr.u32 = be_reg->isp_wdr_wgtidx_thr.u32;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_short_thr = uisp_wdr_short_thr;
    be_reg->isp_wdr_wgtidx_thr.u32 = o_isp_wdr_wgtidx_thr.u32;
}

/*  description : set the value of the member isp_wdr_dftwgt_fl.isp_wdr_dftwgt_fl */
/*  input       : unsigned int uisp_wdr_dftwgt_fl: 9 bits */
static __inline td_void isp_wdr_dftwgt_fl_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_dftwgt_fl)
{
    u_isp_wdr_dftwgt_fl o_isp_wdr_dftwgt_fl;
    o_isp_wdr_dftwgt_fl.u32 = be_reg->isp_wdr_dftwgt_fl.u32;
    o_isp_wdr_dftwgt_fl.bits.isp_wdr_dftwgt_fl = uisp_wdr_dftwgt_fl;
    be_reg->isp_wdr_dftwgt_fl.u32 = o_isp_wdr_dftwgt_fl.u32;
}

/*  description : set the value of the member isp_wdr_wgt_slope.isp_wdr_wgt_slope */
/*  input       : unsigned int uisp_wdr_wgt_slope: 12 bits */
static __inline td_void isp_wdr_wgt_slope_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_wgt_slope)
{
    u_isp_wdr_wgt_slope o_isp_wdr_wgt_slope;
    o_isp_wdr_wgt_slope.u32 = be_reg->isp_wdr_wgt_slope.u32;
    o_isp_wdr_wgt_slope.bits.isp_wdr_wgt_slope = uisp_wdr_wgt_slope;
    be_reg->isp_wdr_wgt_slope.u32 = o_isp_wdr_wgt_slope.u32;
}

/*  description : set the value of the member isp_wdr_fusion_thr_r.isp_wdr_fusion_f1_thr_r */
/*  input       : unsigned int uisp_wdr_fusion_f1_thr_r: 12 bits */
static __inline td_void isp_wdr_fusion_f1_thr_r_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_f1_thr_r)
{
    u_isp_wdr_fusion_thr_r o_isp_wdr_fusion_thr_r;
    o_isp_wdr_fusion_thr_r.u32 = be_reg->isp_wdr_fusion_thr_r.u32;
    o_isp_wdr_fusion_thr_r.bits.isp_wdr_fusion_f1_thr_r = uisp_wdr_fusion_f1_thr_r;
    be_reg->isp_wdr_fusion_thr_r.u32 = o_isp_wdr_fusion_thr_r.u32;
}

/*  description : set the value of the member isp_wdr_fusion_thr_r.isp_wdr_fusion_f0_thr_r */
/*  input       : unsigned int uisp_wdr_fusion_f0_thr_r: 12 bits */
static __inline td_void isp_wdr_fusion_f0_thr_r_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_f0_thr_r)
{
    u_isp_wdr_fusion_thr_r o_isp_wdr_fusion_thr_r;
    o_isp_wdr_fusion_thr_r.u32 = be_reg->isp_wdr_fusion_thr_r.u32;
    o_isp_wdr_fusion_thr_r.bits.isp_wdr_fusion_f0_thr_r = uisp_wdr_fusion_f0_thr_r;
    be_reg->isp_wdr_fusion_thr_r.u32 = o_isp_wdr_fusion_thr_r.u32;
}

/*  description : set the value of the member isp_wdr_fusion_thr_b.isp_wdr_fusion_f1_thr_b */
/*  input       : unsigned int uisp_wdr_fusion_f1_thr_b: 12 bits */
static __inline td_void isp_wdr_fusion_f1_thr_b_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_f1_thr_b)
{
    u_isp_wdr_fusion_thr_b o_isp_wdr_fusion_thr_b;
    o_isp_wdr_fusion_thr_b.u32 = be_reg->isp_wdr_fusion_thr_b.u32;
    o_isp_wdr_fusion_thr_b.bits.isp_wdr_fusion_f1_thr_b = uisp_wdr_fusion_f1_thr_b;
    be_reg->isp_wdr_fusion_thr_b.u32 = o_isp_wdr_fusion_thr_b.u32;
}

/*  description : set the value of the member isp_wdr_fusion_thr_b.isp_wdr_fusion_f0_thr_b */
/*  input       : unsigned int uisp_wdr_fusion_f0_thr_b: 12 bits */
static __inline td_void isp_wdr_fusion_f0_thr_b_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_f0_thr_b)
{
    u_isp_wdr_fusion_thr_b o_isp_wdr_fusion_thr_b;
    o_isp_wdr_fusion_thr_b.u32 = be_reg->isp_wdr_fusion_thr_b.u32;
    o_isp_wdr_fusion_thr_b.bits.isp_wdr_fusion_f0_thr_b = uisp_wdr_fusion_f0_thr_b;
    be_reg->isp_wdr_fusion_thr_b.u32 = o_isp_wdr_fusion_thr_b.u32;
}

/*  description : set the value of the member isp_wdr_fusion_thr_g.isp_wdr_fusion_f1_thr_g */
/*  input       : unsigned int uisp_wdr_fusion_f1_thr_g: 12 bits */
static __inline td_void isp_wdr_fusion_f1_thr_g_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_f1_thr_g)
{
    u_isp_wdr_fusion_thr_g o_isp_wdr_fusion_thr_g;
    o_isp_wdr_fusion_thr_g.u32 = be_reg->isp_wdr_fusion_thr_g.u32;
    o_isp_wdr_fusion_thr_g.bits.isp_wdr_fusion_f1_thr_g = uisp_wdr_fusion_f1_thr_g;
    be_reg->isp_wdr_fusion_thr_g.u32 = o_isp_wdr_fusion_thr_g.u32;
}

/*  description : set the value of the member isp_wdr_fusion_thr_g.isp_wdr_fusion_f0_thr_g */
/*  input       : unsigned int uisp_wdr_fusion_f0_thr_g: 12 bits */
static __inline td_void isp_wdr_fusion_f0_thr_g_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_f0_thr_g)
{
    u_isp_wdr_fusion_thr_g o_isp_wdr_fusion_thr_g;
    o_isp_wdr_fusion_thr_g.u32 = be_reg->isp_wdr_fusion_thr_g.u32;
    o_isp_wdr_fusion_thr_g.bits.isp_wdr_fusion_f0_thr_g = uisp_wdr_fusion_f0_thr_g;
    be_reg->isp_wdr_fusion_thr_g.u32 = o_isp_wdr_fusion_thr_g.u32;
}

/*  description : set the value of the member isp_wdr_fusion_sat_thd.isp_wdr_fusion_saturate_thd */
/*  input       : unsigned int uisp_wdr_fusion_saturate_thd: 12 bits */
static __inline td_void isp_wdr_fusion_saturate_thd_write(isp_be_reg_type *be_reg, td_u32 uisp_wdr_fusion_saturate_thd)
{
    u_isp_wdr_fusion_sat_thd o_isp_wdr_fusion_sat_thd;
    o_isp_wdr_fusion_sat_thd.u32 = be_reg->isp_wdr_fusion_sat_thd.u32;
    o_isp_wdr_fusion_sat_thd.bits.isp_wdr_fusion_saturate_thd = uisp_wdr_fusion_saturate_thd;
    be_reg->isp_wdr_fusion_sat_thd.u32 = o_isp_wdr_fusion_sat_thd.u32;
}

/*  description   set the value of the member isp_dehaze_blk_size.isp_dehaze_block_sizev */
/*  input         unsigned int uisp_dehaze_block_sizev  9 bits */
static __inline td_void isp_dehaze_block_sizev_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_block_sizev)
{
    u_isp_dehaze_blk_size o_isp_dehaze_blk_size;
    o_isp_dehaze_blk_size.u32 = be_reg->isp_dehaze_blk_size.u32;
    o_isp_dehaze_blk_size.bits.isp_dehaze_block_sizev = uisp_dehaze_block_sizev;
    be_reg->isp_dehaze_blk_size.u32 = o_isp_dehaze_blk_size.u32;
}
/*  description   set the value of the member isp_dehaze_blk_size.isp_dehaze_block_sizeh */
/*  input         unsigned int uisp_dehaze_block_sizeh  9 bits */
static __inline td_void isp_dehaze_block_sizeh_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_block_sizeh)
{
    u_isp_dehaze_blk_size o_isp_dehaze_blk_size;
    o_isp_dehaze_blk_size.u32 = be_reg->isp_dehaze_blk_size.u32;
    o_isp_dehaze_blk_size.bits.isp_dehaze_block_sizeh = uisp_dehaze_block_sizeh;
    be_reg->isp_dehaze_blk_size.u32 = o_isp_dehaze_blk_size.u32;
}
/*  description   set the value of the member isp_dehaze_blk_sum.isp_dehaze_block_sum */
/*  input         unsigned int uisp_dehaze_block_sum  11 bits */
static __inline td_void isp_dehaze_block_sum_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_block_sum)
{
    u_isp_dehaze_blk_sum o_isp_dehaze_blk_sum;
    o_isp_dehaze_blk_sum.u32 = be_reg->isp_dehaze_blk_sum.u32;
    o_isp_dehaze_blk_sum.bits.isp_dehaze_block_sum = uisp_dehaze_block_sum;
    be_reg->isp_dehaze_blk_sum.u32 = o_isp_dehaze_blk_sum.u32;
}
/*  description   set the value of the member isp_dehaze_dc_size.isp_dehaze_dc_numv */
/*  input         unsigned int uisp_dehaze_dc_numv  5 bits */
static __inline td_void isp_dehaze_dc_numv_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_dc_numv)
{
    u_isp_dehaze_dc_size o_isp_dehaze_dc_size;
    o_isp_dehaze_dc_size.u32 = be_reg->isp_dehaze_dc_size.u32;
    o_isp_dehaze_dc_size.bits.isp_dehaze_dc_numv = uisp_dehaze_dc_numv;
    be_reg->isp_dehaze_dc_size.u32 = o_isp_dehaze_dc_size.u32;
}
/*  description   set the value of the member isp_dehaze_dc_size.isp_dehaze_dc_numh */
/*  input         unsigned int uisp_dehaze_dc_numh  5 bits */
static __inline td_void isp_dehaze_dc_numh_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_dc_numh)
{
    u_isp_dehaze_dc_size o_isp_dehaze_dc_size;
    o_isp_dehaze_dc_size.u32 = be_reg->isp_dehaze_dc_size.u32;
    o_isp_dehaze_dc_size.bits.isp_dehaze_dc_numh = uisp_dehaze_dc_numh;
    be_reg->isp_dehaze_dc_size.u32 = o_isp_dehaze_dc_size.u32;
}
/*  description   set the value of the member isp_dehaze_x.isp_dehaze_phase_x */
/*  input         unsigned int uisp_dehaze_phase_x  24 bits */
static __inline td_void isp_dehaze_phase_x_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_phase_x)
{
    u_isp_dehaze_x o_isp_dehaze_x;
    o_isp_dehaze_x.u32 = be_reg->isp_dehaze_x.u32;
    o_isp_dehaze_x.bits.isp_dehaze_phase_x = uisp_dehaze_phase_x;
    be_reg->isp_dehaze_x.u32 = o_isp_dehaze_x.u32;
}
/*  description   set the value of the member isp_dehaze_y.isp_dehaze_phase_y */
/*  input         unsigned int uisp_dehaze_phase_y  24 bits */
static __inline td_void isp_dehaze_phase_y_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_phase_y)
{
    u_isp_dehaze_y o_isp_dehaze_y;
    o_isp_dehaze_y.u32 = be_reg->isp_dehaze_y.u32;
    o_isp_dehaze_y.bits.isp_dehaze_phase_y = uisp_dehaze_phase_y;
    be_reg->isp_dehaze_y.u32 = o_isp_dehaze_y.u32;
}
/*  description   set the value of the member isp_dehaze_air.isp_dehaze_air_b */
/*  input         unsigned int uisp_dehaze_air_b  10 bits */
static __inline td_void isp_dehaze_air_b_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_air_b)
{
    u_isp_dehaze_air o_isp_dehaze_air;
    o_isp_dehaze_air.u32 = be_reg->isp_dehaze_air.u32;
    o_isp_dehaze_air.bits.isp_dehaze_air_b = uisp_dehaze_air_b;
    be_reg->isp_dehaze_air.u32 = o_isp_dehaze_air.u32;
}
/*  description   set the value of the member isp_dehaze_air.isp_dehaze_air_g */
/*  input         unsigned int uisp_dehaze_air_g  10 bits */
static __inline td_void isp_dehaze_air_g_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_air_g)
{
    u_isp_dehaze_air o_isp_dehaze_air;
    o_isp_dehaze_air.u32 = be_reg->isp_dehaze_air.u32;
    o_isp_dehaze_air.bits.isp_dehaze_air_g = uisp_dehaze_air_g;
    be_reg->isp_dehaze_air.u32 = o_isp_dehaze_air.u32;
}
/*  description   set the value of the member isp_dehaze_air.isp_dehaze_air_r */
/*  input         unsigned int uisp_dehaze_air_r  10 bits */
static __inline td_void isp_dehaze_air_r_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_air_r)
{
    u_isp_dehaze_air o_isp_dehaze_air;
    o_isp_dehaze_air.u32 = be_reg->isp_dehaze_air.u32;
    o_isp_dehaze_air.bits.isp_dehaze_air_r = uisp_dehaze_air_r;
    be_reg->isp_dehaze_air.u32 = o_isp_dehaze_air.u32;
}
/*  description   set the value of the member isp_dehaze_thld.isp_dehaze_thld_tb */
/*  input         unsigned int uisp_dehaze_thld_tb  10 bits */
static __inline td_void isp_dehaze_thld_tb_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_thld_tb)
{
    u_isp_dehaze_thld o_isp_dehaze_thld;
    o_isp_dehaze_thld.u32 = be_reg->isp_dehaze_thld.u32;
    o_isp_dehaze_thld.bits.isp_dehaze_thld_tb = uisp_dehaze_thld_tb;
    be_reg->isp_dehaze_thld.u32 = o_isp_dehaze_thld.u32;
}
/*  description   set the value of the member isp_dehaze_thld.isp_dehaze_thld_tg */
/*  input         unsigned int uisp_dehaze_thld_tg  10 bits */
static __inline td_void isp_dehaze_thld_tg_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_thld_tg)
{
    u_isp_dehaze_thld o_isp_dehaze_thld;
    o_isp_dehaze_thld.u32 = be_reg->isp_dehaze_thld.u32;
    o_isp_dehaze_thld.bits.isp_dehaze_thld_tg = uisp_dehaze_thld_tg;
    be_reg->isp_dehaze_thld.u32 = o_isp_dehaze_thld.u32;
}
/*  description   set the value of the member isp_dehaze_thld.isp_dehaze_thld_tr */
/*  input         unsigned int uisp_dehaze_thld_tr  10 bits */
static __inline td_void isp_dehaze_thld_tr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_thld_tr)
{
    u_isp_dehaze_thld o_isp_dehaze_thld;
    o_isp_dehaze_thld.u32 = be_reg->isp_dehaze_thld.u32;
    o_isp_dehaze_thld.bits.isp_dehaze_thld_tr = uisp_dehaze_thld_tr;
    be_reg->isp_dehaze_thld.u32 = o_isp_dehaze_thld.u32;
}
/*  description   set the value of the member isp_dehaze_gstrth.isp_dehaze_gstrth */
/*  input         unsigned int uisp_dehaze_gstrth  8 bits */
static __inline td_void isp_dehaze_gstrth_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_gstrth)
{
    u_isp_dehaze_gstrth o_isp_dehaze_gstrth;
    o_isp_dehaze_gstrth.u32 = be_reg->isp_dehaze_gstrth.u32;
    o_isp_dehaze_gstrth.bits.isp_dehaze_gstrth = uisp_dehaze_gstrth;
    be_reg->isp_dehaze_gstrth.u32 = o_isp_dehaze_gstrth.u32;
}
/*  description   set the value of the member isp_dehaze_blthld.isp_dehaze_blthld */
/*  input         unsigned int uisp_dehaze_blthld  10 bits */
static __inline td_void isp_dehaze_blthld_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_blthld)
{
    u_isp_dehaze_blthld o_isp_dehaze_blthld;
    o_isp_dehaze_blthld.u32 = be_reg->isp_dehaze_blthld.u32;
    o_isp_dehaze_blthld.bits.isp_dehaze_blthld = uisp_dehaze_blthld;
    be_reg->isp_dehaze_blthld.u32 = o_isp_dehaze_blthld.u32;
}
/*  description   set the value of the member isp_dehaze_stt_bst.isp_dehaze_stt_size */
/*  input         unsigned int uisp_dehaze_stt_size  16 bits */
static __inline td_void isp_dehaze_stt_size_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stt_size)
{
    u_isp_dehaze_stt_bst o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.u32 = be_reg->isp_dehaze_stt_bst.u32;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_size = uisp_dehaze_stt_size;
    be_reg->isp_dehaze_stt_bst.u32 = o_isp_dehaze_stt_bst.u32;
}
/*  description   set the value of the member isp_dehaze_stt_bst.isp_dehaze_stt_bst */
/*  input         unsigned int uisp_dehaze_stt_bst  4 bits */
static __inline td_void isp_dehaze_stt_bst_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stt_bst)
{
    u_isp_dehaze_stt_bst o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.u32 = be_reg->isp_dehaze_stt_bst.u32;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_bst = uisp_dehaze_stt_bst;
    be_reg->isp_dehaze_stt_bst.u32 = o_isp_dehaze_stt_bst.u32;
}
/*  description   set the value of the member isp_dehaze_stt_bst.isp_dehaze_stt_en */
/*  input         unsigned int uisp_dehaze_stt_en  1 bits */
static __inline td_void isp_dehaze_stt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stt_en)
{
    u_isp_dehaze_stt_bst o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.u32 = be_reg->isp_dehaze_stt_bst.u32;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_en = uisp_dehaze_stt_en;
    be_reg->isp_dehaze_stt_bst.u32 = o_isp_dehaze_stt_bst.u32;
}
/*  description   set the value of the member isp_dehaze_stt_abn.isp_dehaze_stt_clr */
/*  input         unsigned int uisp_dehaze_stt_clr  1 bits */
static __inline td_void isp_dehaze_stt_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stt_clr)
{
    u_isp_dehaze_stt_abn o_isp_dehaze_stt_abn;
    o_isp_dehaze_stt_abn.u32 = be_reg->isp_dehaze_stt_abn.u32;
    o_isp_dehaze_stt_abn.bits.isp_dehaze_stt_clr = uisp_dehaze_stt_clr;
    be_reg->isp_dehaze_stt_abn.u32 = o_isp_dehaze_stt_abn.u32;
}
/*  description   set the value of the member isp_dehaze_stt2lut_cfg.isp_dehaze_stt2lut_en */
/*  input         unsigned int uisp_dehaze_stt2lut_en  1 bits */
static __inline td_void isp_dehaze_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stt2lut_en)
{
    u_isp_dehaze_stt2lut_cfg o_isp_dehaze_stt2lut_cfg;
    o_isp_dehaze_stt2lut_cfg.u32 = be_reg->isp_dehaze_stt2lut_cfg.u32;
    o_isp_dehaze_stt2lut_cfg.bits.isp_dehaze_stt2lut_en = uisp_dehaze_stt2lut_en;
    be_reg->isp_dehaze_stt2lut_cfg.u32 = o_isp_dehaze_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_dehaze_stt2lut_regnew.isp_dehaze_stt2lut_regnew */
/*  input         unsigned int uisp_dehaze_stt2lut_regnew  1 bits */
static __inline td_void isp_dehaze_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 dehaze_stt2lut_regnew)
{
    u_isp_dehaze_stt2lut_regnew o_isp_dehaze_stt2lut_regnew;
    o_isp_dehaze_stt2lut_regnew.u32 = be_reg->isp_dehaze_stt2lut_regnew.u32;
    o_isp_dehaze_stt2lut_regnew.bits.isp_dehaze_stt2lut_regnew = dehaze_stt2lut_regnew;
    be_reg->isp_dehaze_stt2lut_regnew.u32 = o_isp_dehaze_stt2lut_regnew.u32;
}
/*  description   set the value of the member isp_dehaze_stt2lut_abn.isp_dehaze_stt2lut_clr */
/*  input         unsigned int uisp_dehaze_stt2lut_clr  1 bits */
static __inline td_void isp_dehaze_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stt2lut_clr)
{
    u_isp_dehaze_stt2lut_abn o_isp_dehaze_stt2lut_abn;
    o_isp_dehaze_stt2lut_abn.u32 = be_reg->isp_dehaze_stt2lut_abn.u32;
    o_isp_dehaze_stt2lut_abn.bits.isp_dehaze_stt2lut_clr = uisp_dehaze_stt2lut_clr;
    be_reg->isp_dehaze_stt2lut_abn.u32 = o_isp_dehaze_stt2lut_abn.u32;
}
/*  description   set the value of the member isp_dehaze_smlmapoffset.isp_dehaze_smlmapoffset */
/*  input         unsigned int uisp_dehaze_smlmapoffset  20 bits */
static __inline td_void isp_dehaze_smlmapoffset_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_smlmapoffset)
{
    u_isp_dehaze_smlmapoffset o_isp_dehaze_smlmapoffset;
    o_isp_dehaze_smlmapoffset.u32 = be_reg->isp_dehaze_smlmapoffset.u32;
    o_isp_dehaze_smlmapoffset.bits.isp_dehaze_smlmapoffset = uisp_dehaze_smlmapoffset;
    be_reg->isp_dehaze_smlmapoffset.u32 = o_isp_dehaze_smlmapoffset.u32;
}
/*  description   set the value of the member isp_dehaze_stat_point.isp_dehaze_statstartx */
/*  input         unsigned int uisp_dehaze_statstartx  13 bits */
static __inline td_void isp_dehaze_statstartx_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_statstartx)
{
    u_isp_dehaze_stat_point o_isp_dehaze_stat_point;
    o_isp_dehaze_stat_point.u32 = be_reg->isp_dehaze_stat_point.u32;
    o_isp_dehaze_stat_point.bits.isp_dehaze_statstartx = uisp_dehaze_statstartx;
    be_reg->isp_dehaze_stat_point.u32 = o_isp_dehaze_stat_point.u32;
}
/*  description   set the value of the member isp_dehaze_stat_point.isp_dehaze_statendx */
/*  input         unsigned int uisp_dehaze_statendx  13 bits */
static __inline td_void isp_dehaze_statendx_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_statendx)
{
    u_isp_dehaze_stat_point o_isp_dehaze_stat_point;
    o_isp_dehaze_stat_point.u32 = be_reg->isp_dehaze_stat_point.u32;
    o_isp_dehaze_stat_point.bits.isp_dehaze_statendx = uisp_dehaze_statendx;
    be_reg->isp_dehaze_stat_point.u32 = o_isp_dehaze_stat_point.u32;
}
/*  description   set the value of the member isp_dehaze_stat_num.isp_dehaze_stat_numv */
/*  input         unsigned int uisp_dehaze_stat_numv  5 bits */
static __inline td_void isp_dehaze_stat_numv_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stat_numv)
{
    u_isp_dehaze_stat_num o_isp_dehaze_stat_num;
    o_isp_dehaze_stat_num.u32 = be_reg->isp_dehaze_stat_num.u32;
    o_isp_dehaze_stat_num.bits.isp_dehaze_stat_numv = uisp_dehaze_stat_numv;
    be_reg->isp_dehaze_stat_num.u32 = o_isp_dehaze_stat_num.u32;
}
/*  description   set the value of the member isp_dehaze_stat_num.isp_dehaze_stat_numh */
/*  input         unsigned int uisp_dehaze_stat_numh  5 bits */
static __inline td_void isp_dehaze_stat_numh_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_stat_numh)
{
    u_isp_dehaze_stat_num o_isp_dehaze_stat_num;
    o_isp_dehaze_stat_num.u32 = be_reg->isp_dehaze_stat_num.u32;
    o_isp_dehaze_stat_num.bits.isp_dehaze_stat_numh = uisp_dehaze_stat_numh;
    be_reg->isp_dehaze_stat_num.u32 = o_isp_dehaze_stat_num.u32;
}
/*  description   set the value of the member isp_dehaze_minstat_waddr.isp_dehaze_minstat_waddr */
/*  input         unsigned int uisp_dehaze_minstat_waddr  9 bits */
static __inline td_void isp_dehaze_minstat_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_minstat_waddr)
{
    u_isp_dehaze_minstat_waddr o_isp_dehaze_minstat_waddr;
    o_isp_dehaze_minstat_waddr.u32 = be_reg->isp_dehaze_minstat_waddr.u32;
    o_isp_dehaze_minstat_waddr.bits.isp_dehaze_minstat_waddr = uisp_dehaze_minstat_waddr;
    be_reg->isp_dehaze_minstat_waddr.u32 = o_isp_dehaze_minstat_waddr.u32;
}
/*  description   set the value of the member isp_dehaze_minstat_wdata.isp_dehaze_minstat_wdata_l */
/*  input         unsigned int uisp_dehaze_minstat_wdata_l  10 bits */
static __inline td_void isp_dehaze_minstat_wdata_l_write(isp_be_reg_type *be_reg, td_u32 dehaze_minstat_wdata_l)
{
    u_isp_dehaze_minstat_wdata o_isp_dehaze_minstat_wdata;
    o_isp_dehaze_minstat_wdata.u32 = be_reg->isp_dehaze_minstat_wdata.u32;
    o_isp_dehaze_minstat_wdata.bits.isp_dehaze_minstat_wdata_l = dehaze_minstat_wdata_l;
    be_reg->isp_dehaze_minstat_wdata.u32 = o_isp_dehaze_minstat_wdata.u32;
}
/*  description   set the value of the member isp_dehaze_minstat_wdata.isp_dehaze_minstat_wdata_h */
/*  input         unsigned int uisp_dehaze_minstat_wdata_h  10 bits */
static __inline td_void isp_dehaze_minstat_wdata_h_write(isp_be_reg_type *be_reg, td_u32 dehaze_minstat_wdata_h)
{
    u_isp_dehaze_minstat_wdata o_isp_dehaze_minstat_wdata;
    o_isp_dehaze_minstat_wdata.u32 = be_reg->isp_dehaze_minstat_wdata.u32;
    o_isp_dehaze_minstat_wdata.bits.isp_dehaze_minstat_wdata_h = dehaze_minstat_wdata_h;
    be_reg->isp_dehaze_minstat_wdata.u32 = o_isp_dehaze_minstat_wdata.u32;
}
/*  description   set the value of the member isp_dehaze_minstat_raddr.isp_dehaze_minstat_raddr */
/*  input         unsigned int uisp_dehaze_minstat_raddr  9 bits */
static __inline td_void isp_dehaze_minstat_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_minstat_raddr)
{
    u_isp_dehaze_minstat_raddr o_isp_dehaze_minstat_raddr;
    o_isp_dehaze_minstat_raddr.u32 = be_reg->isp_dehaze_minstat_raddr.u32;
    o_isp_dehaze_minstat_raddr.bits.isp_dehaze_minstat_raddr = uisp_dehaze_minstat_raddr;
    be_reg->isp_dehaze_minstat_raddr.u32 = o_isp_dehaze_minstat_raddr.u32;
}
/*  description   set the value of the member isp_dehaze_maxstat_waddr.isp_dehaze_maxstat_waddr */
/*  input         unsigned int uisp_dehaze_maxstat_waddr  10 bits */
static __inline td_void isp_dehaze_maxstat_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_maxstat_waddr)
{
    u_isp_dehaze_maxstat_waddr o_isp_dehaze_maxstat_waddr;
    o_isp_dehaze_maxstat_waddr.u32 = be_reg->isp_dehaze_maxstat_waddr.u32;
    o_isp_dehaze_maxstat_waddr.bits.isp_dehaze_maxstat_waddr = uisp_dehaze_maxstat_waddr;
    be_reg->isp_dehaze_maxstat_waddr.u32 = o_isp_dehaze_maxstat_waddr.u32;
}
/*  description   set the value of the member isp_dehaze_maxstat_wdata.isp_dehaze_maxstat_wdata */
/*  input         unsigned int uisp_dehaze_maxstat_wdata  30 bits */
static __inline td_void isp_dehaze_maxstat_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_maxstat_wdata)
{
    u_isp_dehaze_maxstat_wdata o_isp_dehaze_maxstat_wdata;
    o_isp_dehaze_maxstat_wdata.u32 = be_reg->isp_dehaze_maxstat_wdata.u32;
    o_isp_dehaze_maxstat_wdata.bits.isp_dehaze_maxstat_wdata = uisp_dehaze_maxstat_wdata;
    be_reg->isp_dehaze_maxstat_wdata.u32 = o_isp_dehaze_maxstat_wdata.u32;
}
/*  description   set the value of the member isp_dehaze_maxstat_raddr.isp_dehaze_maxstat_raddr */
/*  input         unsigned int uisp_dehaze_maxstat_raddr  10 bits */
static __inline td_void isp_dehaze_maxstat_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_maxstat_raddr)
{
    u_isp_dehaze_maxstat_raddr o_isp_dehaze_maxstat_raddr;
    o_isp_dehaze_maxstat_raddr.u32 = be_reg->isp_dehaze_maxstat_raddr.u32;
    o_isp_dehaze_maxstat_raddr.bits.isp_dehaze_maxstat_raddr = uisp_dehaze_maxstat_raddr;
    be_reg->isp_dehaze_maxstat_raddr.u32 = o_isp_dehaze_maxstat_raddr.u32;
}
/*  description   set the value of the member isp_dehaze_prestat_waddr.isp_dehaze_prestat_waddr */
/*  input         unsigned int uisp_dehaze_prestat_waddr  10 bits */
static __inline td_void isp_dehaze_prestat_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_prestat_waddr)
{
    u_isp_dehaze_prestat_waddr o_isp_dehaze_prestat_waddr;
    o_isp_dehaze_prestat_waddr.u32 = be_reg->isp_dehaze_prestat_waddr.u32;
    o_isp_dehaze_prestat_waddr.bits.isp_dehaze_prestat_waddr = uisp_dehaze_prestat_waddr;
    be_reg->isp_dehaze_prestat_waddr.u32 = o_isp_dehaze_prestat_waddr.u32;
}
/*  description   set the value of the member isp_dehaze_prestat_wdata.isp_dehaze_prestat_wdata */
/*  input         unsigned int uisp_dehaze_prestat_wdata  10 bits */
static __inline td_void isp_dehaze_prestat_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_prestat_wdata)
{
    u_isp_dehaze_prestat_wdata o_isp_dehaze_prestat_wdata;
    o_isp_dehaze_prestat_wdata.u32 = be_reg->isp_dehaze_prestat_wdata.u32;
    o_isp_dehaze_prestat_wdata.bits.isp_dehaze_prestat_wdata = uisp_dehaze_prestat_wdata;
    be_reg->isp_dehaze_prestat_wdata.u32 = o_isp_dehaze_prestat_wdata.u32;
}
/*  description   set the value of the member isp_dehaze_prestat_raddr.isp_dehaze_prestat_raddr */
/*  input         unsigned int uisp_dehaze_prestat_raddr  10 bits */
static __inline td_void isp_dehaze_prestat_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_prestat_raddr)
{
    u_isp_dehaze_prestat_raddr o_isp_dehaze_prestat_raddr;
    o_isp_dehaze_prestat_raddr.u32 = be_reg->isp_dehaze_prestat_raddr.u32;
    o_isp_dehaze_prestat_raddr.bits.isp_dehaze_prestat_raddr = uisp_dehaze_prestat_raddr;
    be_reg->isp_dehaze_prestat_raddr.u32 = o_isp_dehaze_prestat_raddr.u32;
}
/*  description   set the value of the member isp_dehaze_lut_waddr.isp_dehaze_lut_waddr */
/*  input         unsigned int uisp_dehaze_lut_waddr  8 bits */
static __inline td_void isp_dehaze_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_lut_waddr)
{
    u_isp_dehaze_lut_waddr o_isp_dehaze_lut_waddr;
    o_isp_dehaze_lut_waddr.u32 = be_reg->isp_dehaze_lut_waddr.u32;
    o_isp_dehaze_lut_waddr.bits.isp_dehaze_lut_waddr = uisp_dehaze_lut_waddr;
    be_reg->isp_dehaze_lut_waddr.u32 = o_isp_dehaze_lut_waddr.u32;
}
/*  description   set the value of the member isp_dehaze_lut_wdata.isp_dehaze_lut_wdata */
/*  input         unsigned int uisp_dehaze_lut_wdata  8 bits */
static __inline td_void isp_dehaze_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_lut_wdata)
{
    u_isp_dehaze_lut_wdata o_isp_dehaze_lut_wdata;
    o_isp_dehaze_lut_wdata.u32 = be_reg->isp_dehaze_lut_wdata.u32;
    o_isp_dehaze_lut_wdata.bits.isp_dehaze_lut_wdata = uisp_dehaze_lut_wdata;
    be_reg->isp_dehaze_lut_wdata.u32 = o_isp_dehaze_lut_wdata.u32;
}
/*  description   set the value of the member isp_dehaze_lut_raddr.isp_dehaze_lut_raddr */
/*  input         unsigned int uisp_dehaze_lut_raddr  8 bits */
static __inline td_void isp_dehaze_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_dehaze_lut_raddr)
{
    u_isp_dehaze_lut_raddr o_isp_dehaze_lut_raddr;
    o_isp_dehaze_lut_raddr.u32 = be_reg->isp_dehaze_lut_raddr.u32;
    o_isp_dehaze_lut_raddr.bits.isp_dehaze_lut_raddr = uisp_dehaze_lut_raddr;
    be_reg->isp_dehaze_lut_raddr.u32 = o_isp_dehaze_lut_raddr.u32;
}
/*  description : set the value of the member isp_expander_bitw.isp_expander_bitw_out */
/*  input       : unsigned int uisp_expander_bitw_out: 5 bits */
static __inline td_void isp_expander_bitw_out_write(isp_be_reg_type *be_reg, td_u32 uisp_expander_bitw_out)
{
    u_isp_expander_bitw o_isp_expander_bitw;
    o_isp_expander_bitw.u32 = be_reg->isp_expander_bitw.u32;
    o_isp_expander_bitw.bits.isp_expander_bitw_out = uisp_expander_bitw_out;
    be_reg->isp_expander_bitw.u32 = o_isp_expander_bitw.u32;
}

/*  description : set the value of the member isp_expander_bitw.isp_expander_bitw_in */
/*  input       : unsigned int uisp_expander_bitw_in: 5 bits */
static __inline td_void isp_expander_bitw_in_write(isp_be_reg_type *be_reg, td_u32 uisp_expander_bitw_in)
{
    u_isp_expander_bitw o_isp_expander_bitw;
    o_isp_expander_bitw.u32 = be_reg->isp_expander_bitw.u32;
    o_isp_expander_bitw.bits.isp_expander_bitw_in = uisp_expander_bitw_in;
    be_reg->isp_expander_bitw.u32 = o_isp_expander_bitw.u32;
}

/*  description : set the value of the member isp_expander_lut_waddr.isp_expander_lut_waddr */
/*  input       : unsigned int uisp_expander_lut_waddr: 9 bits */
static __inline td_void isp_expander_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_expander_lut_waddr)
{
    u_isp_expander_lut_waddr o_isp_expander_lut_waddr;
    o_isp_expander_lut_waddr.u32 = be_reg->isp_expander_lut_waddr.u32;
    o_isp_expander_lut_waddr.bits.isp_expander_lut_waddr = uisp_expander_lut_waddr;
    be_reg->isp_expander_lut_waddr.u32 = o_isp_expander_lut_waddr.u32;
}

/*  description : set the value of the member isp_expander_lut_wdata.isp_expander_lut_wdata */
/*  input       : unsigned int uisp_expander_lut_wdata: 18 bits */
static __inline td_void isp_expander_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_expander_lut_wdata)
{
    u_isp_expander_lut_wdata o_isp_expander_lut_wdata;
    o_isp_expander_lut_wdata.u32 = be_reg->isp_expander_lut_wdata.u32;
    o_isp_expander_lut_wdata.bits.isp_expander_lut_wdata = uisp_expander_lut_wdata;
    be_reg->isp_expander_lut_wdata.u32 = o_isp_expander_lut_wdata.u32;
}

/*  description : set the value of the member isp_expander_lut_raddr.isp_expander_lut_raddr */
/*  input       : unsigned int uisp_expander_lut_raddr: 9 bits */
static __inline td_void isp_expander_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_expander_lut_raddr)
{
    u_isp_expander_lut_raddr o_isp_expander_lut_raddr;
    o_isp_expander_lut_raddr.u32 = be_reg->isp_expander_lut_raddr.u32;
    o_isp_expander_lut_raddr.bits.isp_expander_lut_raddr = uisp_expander_lut_raddr;
    be_reg->isp_expander_lut_raddr.u32 = o_isp_expander_lut_raddr.u32;
}

/*  description   set the value of the member isp_gamma_pos0.isp_gamma_pos_0 */
/*  input         unsigned int uisp_gamma_pos_0  13 bits */
static __inline td_void isp_gamma_pos_0_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_0)
{
    u_isp_gamma_pos0 o_isp_gamma_pos0;
    o_isp_gamma_pos0.u32 = be_reg->isp_gamma_pos0.u32;
    o_isp_gamma_pos0.bits.isp_gamma_pos_0 = uisp_gamma_pos_0;
    be_reg->isp_gamma_pos0.u32 = o_isp_gamma_pos0.u32;
}
/*  description   set the value of the member isp_gamma_pos0.isp_gamma_pos_1 */
/*  input         unsigned int uisp_gamma_pos_1  13 bits */
static __inline td_void isp_gamma_pos_1_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_1)
{
    u_isp_gamma_pos0 o_isp_gamma_pos0;
    o_isp_gamma_pos0.u32 = be_reg->isp_gamma_pos0.u32;
    o_isp_gamma_pos0.bits.isp_gamma_pos_1 = uisp_gamma_pos_1;
    be_reg->isp_gamma_pos0.u32 = o_isp_gamma_pos0.u32;
}
/*  description   set the value of the member isp_gamma_pos1.isp_gamma_pos_2 */
/*  input         unsigned int uisp_gamma_pos_2  13 bits */
static __inline td_void isp_gamma_pos_2_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_2)
{
    u_isp_gamma_pos1 o_isp_gamma_pos1;
    o_isp_gamma_pos1.u32 = be_reg->isp_gamma_pos1.u32;
    o_isp_gamma_pos1.bits.isp_gamma_pos_2 = uisp_gamma_pos_2;
    be_reg->isp_gamma_pos1.u32 = o_isp_gamma_pos1.u32;
}
/*  description   set the value of the member isp_gamma_pos1.isp_gamma_pos_3 */
/*  input         unsigned int uisp_gamma_pos_3  13 bits */
static __inline td_void isp_gamma_pos_3_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_3)
{
    u_isp_gamma_pos1 o_isp_gamma_pos1;
    o_isp_gamma_pos1.u32 = be_reg->isp_gamma_pos1.u32;
    o_isp_gamma_pos1.bits.isp_gamma_pos_3 = uisp_gamma_pos_3;
    be_reg->isp_gamma_pos1.u32 = o_isp_gamma_pos1.u32;
}
/*  description   set the value of the member isp_gamma_pos2.isp_gamma_pos_4 */
/*  input         unsigned int uisp_gamma_pos_4  13 bits */
static __inline td_void isp_gamma_pos_4_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_4)
{
    u_isp_gamma_pos2 o_isp_gamma_pos2;
    o_isp_gamma_pos2.u32 = be_reg->isp_gamma_pos2.u32;
    o_isp_gamma_pos2.bits.isp_gamma_pos_4 = uisp_gamma_pos_4;
    be_reg->isp_gamma_pos2.u32 = o_isp_gamma_pos2.u32;
}
/*  description   set the value of the member isp_gamma_pos2.isp_gamma_pos_5 */
/*  input         unsigned int uisp_gamma_pos_5  13 bits */
static __inline td_void isp_gamma_pos_5_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_5)
{
    u_isp_gamma_pos2 o_isp_gamma_pos2;
    o_isp_gamma_pos2.u32 = be_reg->isp_gamma_pos2.u32;
    o_isp_gamma_pos2.bits.isp_gamma_pos_5 = uisp_gamma_pos_5;
    be_reg->isp_gamma_pos2.u32 = o_isp_gamma_pos2.u32;
}
/*  description   set the value of the member isp_gamma_pos3.isp_gamma_pos_6 */
/*  input         unsigned int uisp_gamma_pos_6  13 bits */
static __inline td_void isp_gamma_pos_6_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_6)
{
    u_isp_gamma_pos3 o_isp_gamma_pos3;
    o_isp_gamma_pos3.u32 = be_reg->isp_gamma_pos3.u32;
    o_isp_gamma_pos3.bits.isp_gamma_pos_6 = uisp_gamma_pos_6;
    be_reg->isp_gamma_pos3.u32 = o_isp_gamma_pos3.u32;
}
/*  description   set the value of the member isp_gamma_pos3.isp_gamma_pos_7 */
/*  input         unsigned int uisp_gamma_pos_7  13 bits */
static __inline td_void isp_gamma_pos_7_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_pos_7)
{
    u_isp_gamma_pos3 o_isp_gamma_pos3;
    o_isp_gamma_pos3.u32 = be_reg->isp_gamma_pos3.u32;
    o_isp_gamma_pos3.bits.isp_gamma_pos_7 = uisp_gamma_pos_7;
    be_reg->isp_gamma_pos3.u32 = o_isp_gamma_pos3.u32;
}
/*  description   set the value of the member isp_gamma_inseg0.isp_gamma_inseg_0 */
/*  input         unsigned int uisp_gamma_inseg_0  9 bits */
static __inline td_void isp_gamma_inseg_0_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_0)
{
    u_isp_gamma_inseg0 o_isp_gamma_inseg0;
    o_isp_gamma_inseg0.u32 = be_reg->isp_gamma_inseg0.u32;
    o_isp_gamma_inseg0.bits.isp_gamma_inseg_0 = uisp_gamma_inseg_0;
    be_reg->isp_gamma_inseg0.u32 = o_isp_gamma_inseg0.u32;
}
/*  description   set the value of the member isp_gamma_inseg0.isp_gamma_inseg_1 */
/*  input         unsigned int uisp_gamma_inseg_1  9 bits */
static __inline td_void isp_gamma_inseg_1_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_1)
{
    u_isp_gamma_inseg0 o_isp_gamma_inseg0;
    o_isp_gamma_inseg0.u32 = be_reg->isp_gamma_inseg0.u32;
    o_isp_gamma_inseg0.bits.isp_gamma_inseg_1 = uisp_gamma_inseg_1;
    be_reg->isp_gamma_inseg0.u32 = o_isp_gamma_inseg0.u32;
}
/*  description   set the value of the member isp_gamma_inseg1.isp_gamma_inseg_2 */
/*  input         unsigned int uisp_gamma_inseg_2  9 bits */
static __inline td_void isp_gamma_inseg_2_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_2)
{
    u_isp_gamma_inseg1 o_isp_gamma_inseg1;
    o_isp_gamma_inseg1.u32 = be_reg->isp_gamma_inseg1.u32;
    o_isp_gamma_inseg1.bits.isp_gamma_inseg_2 = uisp_gamma_inseg_2;
    be_reg->isp_gamma_inseg1.u32 = o_isp_gamma_inseg1.u32;
}
/*  description   set the value of the member isp_gamma_inseg1.isp_gamma_inseg_3 */
/*  input         unsigned int uisp_gamma_inseg_3  9 bits */
static __inline td_void isp_gamma_inseg_3_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_3)
{
    u_isp_gamma_inseg1 o_isp_gamma_inseg1;
    o_isp_gamma_inseg1.u32 = be_reg->isp_gamma_inseg1.u32;
    o_isp_gamma_inseg1.bits.isp_gamma_inseg_3 = uisp_gamma_inseg_3;
    be_reg->isp_gamma_inseg1.u32 = o_isp_gamma_inseg1.u32;
}
/*  description   set the value of the member isp_gamma_inseg2.isp_gamma_inseg_4 */
/*  input         unsigned int uisp_gamma_inseg_4  9 bits */
static __inline td_void isp_gamma_inseg_4_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_4)
{
    u_isp_gamma_inseg2 o_isp_gamma_inseg2;
    o_isp_gamma_inseg2.u32 = be_reg->isp_gamma_inseg2.u32;
    o_isp_gamma_inseg2.bits.isp_gamma_inseg_4 = uisp_gamma_inseg_4;
    be_reg->isp_gamma_inseg2.u32 = o_isp_gamma_inseg2.u32;
}
/*  description   set the value of the member isp_gamma_inseg2.isp_gamma_inseg_5 */
/*  input         unsigned int uisp_gamma_inseg_5  9 bits */
static __inline td_void isp_gamma_inseg_5_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_5)
{
    u_isp_gamma_inseg2 o_isp_gamma_inseg2;
    o_isp_gamma_inseg2.u32 = be_reg->isp_gamma_inseg2.u32;
    o_isp_gamma_inseg2.bits.isp_gamma_inseg_5 = uisp_gamma_inseg_5;
    be_reg->isp_gamma_inseg2.u32 = o_isp_gamma_inseg2.u32;
}
/*  description   set the value of the member isp_gamma_inseg3.isp_gamma_inseg_6 */
/*  input         unsigned int uisp_gamma_inseg_6  9 bits */
static __inline td_void isp_gamma_inseg_6_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_6)
{
    u_isp_gamma_inseg3 o_isp_gamma_inseg3;
    o_isp_gamma_inseg3.u32 = be_reg->isp_gamma_inseg3.u32;
    o_isp_gamma_inseg3.bits.isp_gamma_inseg_6 = uisp_gamma_inseg_6;
    be_reg->isp_gamma_inseg3.u32 = o_isp_gamma_inseg3.u32;
}
/*  description   set the value of the member isp_gamma_inseg3.isp_gamma_inseg_7 */
/*  input         unsigned int uisp_gamma_inseg_7  9 bits */
static __inline td_void isp_gamma_inseg_7_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_inseg_7)
{
    u_isp_gamma_inseg3 o_isp_gamma_inseg3;
    o_isp_gamma_inseg3.u32 = be_reg->isp_gamma_inseg3.u32;
    o_isp_gamma_inseg3.bits.isp_gamma_inseg_7 = uisp_gamma_inseg_7;
    be_reg->isp_gamma_inseg3.u32 = o_isp_gamma_inseg3.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step0 */
/*  input         unsigned int uisp_gamma_step0  3 bits */
static __inline td_void isp_gamma_step0_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step0)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step0 = uisp_gamma_step0;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step1 */
/*  input         unsigned int uisp_gamma_step1  3 bits */
static __inline td_void isp_gamma_step1_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step1)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step1 = uisp_gamma_step1;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step2 */
/*  input         unsigned int uisp_gamma_step2  3 bits */
static __inline td_void isp_gamma_step2_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step2)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step2 = uisp_gamma_step2;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step3 */
/*  input         unsigned int uisp_gamma_step3  3 bits */
static __inline td_void isp_gamma_step3_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step3)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step3 = uisp_gamma_step3;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step4 */
/*  input         unsigned int uisp_gamma_step4  3 bits */
static __inline td_void isp_gamma_step4_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step4)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step4 = uisp_gamma_step4;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step5 */
/*  input         unsigned int uisp_gamma_step5  3 bits */
static __inline td_void isp_gamma_step5_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step5)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step5 = uisp_gamma_step5;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step6 */
/*  input         unsigned int uisp_gamma_step6  3 bits */
static __inline td_void isp_gamma_step6_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step6)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step6 = uisp_gamma_step6;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_step.isp_gamma_step7 */
/*  input         unsigned int uisp_gamma_step7  3 bits */
static __inline td_void isp_gamma_step7_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_step7)
{
    u_isp_gamma_step o_isp_gamma_step;
    o_isp_gamma_step.u32 = be_reg->isp_gamma_step.u32;
    o_isp_gamma_step.bits.isp_gamma_step7 = uisp_gamma_step7;
    be_reg->isp_gamma_step.u32 = o_isp_gamma_step.u32;
}
/*  description   set the value of the member isp_gamma_stt2lut_cfg.isp_gamma_stt2lut_en */
/*  input         unsigned int uisp_gamma_stt2lut_en  1 bits */
static __inline td_void isp_gamma_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_stt2lut_en)
{
    u_isp_gamma_stt2lut_cfg o_isp_gamma_stt2lut_cfg;
    o_isp_gamma_stt2lut_cfg.u32 = be_reg->isp_gamma_stt2lut_cfg.u32;
    o_isp_gamma_stt2lut_cfg.bits.isp_gamma_stt2lut_en = uisp_gamma_stt2lut_en;
    be_reg->isp_gamma_stt2lut_cfg.u32 = o_isp_gamma_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_gamma_stt2lut_regnew.isp_gamma_stt2lut_regnew */
/*  input         unsigned int uisp_gamma_stt2lut_regnew  1 bits */
static __inline td_void isp_gamma_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_stt2lut_regnew)
{
    u_isp_gamma_stt2lut_regnew o_isp_gamma_stt2lut_regnew;
    o_isp_gamma_stt2lut_regnew.u32 = be_reg->isp_gamma_stt2lut_regnew.u32;
    o_isp_gamma_stt2lut_regnew.bits.isp_gamma_stt2lut_regnew = uisp_gamma_stt2lut_regnew;
    be_reg->isp_gamma_stt2lut_regnew.u32 = o_isp_gamma_stt2lut_regnew.u32;
}
/*  description   set the value of the member isp_gamma_stt2lut_abn.isp_gamma_stt2lut_clr */
/*  input         unsigned int uisp_gamma_stt2lut_clr  1 bits */
static __inline td_void isp_gamma_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_stt2lut_clr)
{
    u_isp_gamma_stt2lut_abn o_isp_gamma_stt2lut_abn;
    o_isp_gamma_stt2lut_abn.u32 = be_reg->isp_gamma_stt2lut_abn.u32;
    o_isp_gamma_stt2lut_abn.bits.isp_gamma_stt2lut_clr = uisp_gamma_stt2lut_clr;
    be_reg->isp_gamma_stt2lut_abn.u32 = o_isp_gamma_stt2lut_abn.u32;
}

static __inline td_u16 isp_gamma_stt2lut_info_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_gamma_stt2lut_abn.bits.isp_gamma_stt2lut_info;
}
/*  description   set the value of the member isp_gamma_lut_waddr.isp_gamma_lut_waddr */
/*  input         unsigned int uisp_gamma_lut_waddr  32 bits */
static __inline td_void isp_gamma_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_lut_waddr)
{
    be_reg->isp_gamma_lut_waddr.u32 = uisp_gamma_lut_waddr;
}

/*  description   set the value of the member isp_gamma_lut_wdata.isp_gamma_lut_wdata */
/*  input         unsigned int uisp_gamma_lut_wdata  32 bits */
static __inline td_void isp_gamma_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_lut_wdata)
{
    be_reg->isp_gamma_lut_wdata.u32 = uisp_gamma_lut_wdata;
}

/*  description   set the value of the member isp_gamma_lut_raddr.isp_gamma_lut_raddr */
/*  input         unsigned int uisp_gamma_lut_raddr  32 bits */
static __inline td_void isp_gamma_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_lut_raddr)
{
    be_reg->isp_gamma_lut_raddr.u32 = uisp_gamma_lut_raddr;
}

/*  description : set the value of the member isp_bcom_lut_step0.isp_bcom_lut_step0 */
/*  input       : unsigned int uisp_bcom_lut_step0: 4 bits */
static __inline td_void isp_bcom_lut_step0_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step0)
{
    u_isp_bcom_lut_step0 o_isp_bcom_lut_step0;
    o_isp_bcom_lut_step0.u32 = be_reg->isp_bcom_lut_step0.u32;
    o_isp_bcom_lut_step0.bits.isp_bcom_lut_step0 = uisp_bcom_lut_step0;
    be_reg->isp_bcom_lut_step0.u32 = o_isp_bcom_lut_step0.u32;
}

/*  description : set the value of the member isp_bcom_lut_step0.isp_bcom_lut_step1 */
/*  input       : unsigned int uisp_bcom_lut_step1: 4 bits */
static __inline td_void isp_bcom_lut_step1_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step1)
{
    u_isp_bcom_lut_step0 o_isp_bcom_lut_step0;
    o_isp_bcom_lut_step0.u32 = be_reg->isp_bcom_lut_step0.u32;
    o_isp_bcom_lut_step0.bits.isp_bcom_lut_step1 = uisp_bcom_lut_step1;
    be_reg->isp_bcom_lut_step0.u32 = o_isp_bcom_lut_step0.u32;
}

/*  description : set the value of the member isp_bcom_lut_step0.isp_bcom_lut_step2 */
/*  input       : unsigned int uisp_bcom_lut_step2: 4 bits */
static __inline td_void isp_bcom_lut_step2_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step2)
{
    u_isp_bcom_lut_step0 o_isp_bcom_lut_step0;
    o_isp_bcom_lut_step0.u32 = be_reg->isp_bcom_lut_step0.u32;
    o_isp_bcom_lut_step0.bits.isp_bcom_lut_step2 = uisp_bcom_lut_step2;
    be_reg->isp_bcom_lut_step0.u32 = o_isp_bcom_lut_step0.u32;
}

/*  description : set the value of the member isp_bcom_lut_step0.isp_bcom_lut_step3 */
/*  input       : unsigned int uisp_bcom_lut_step3: 4 bits */
static __inline td_void isp_bcom_lut_step3_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step3)
{
    u_isp_bcom_lut_step0 o_isp_bcom_lut_step0;
    o_isp_bcom_lut_step0.u32 = be_reg->isp_bcom_lut_step0.u32;
    o_isp_bcom_lut_step0.bits.isp_bcom_lut_step3 = uisp_bcom_lut_step3;
    be_reg->isp_bcom_lut_step0.u32 = o_isp_bcom_lut_step0.u32;
}

/*  description : set the value of the member isp_bcom_lut_step1.isp_bcom_lut_step4 */
/*  input       : unsigned int uisp_bcom_lut_step4: 4 bits */
static __inline td_void isp_bcom_lut_step4_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step4)
{
    u_isp_bcom_lut_step1 o_isp_bcom_lut_step1;
    o_isp_bcom_lut_step1.u32 = be_reg->isp_bcom_lut_step1.u32;
    o_isp_bcom_lut_step1.bits.isp_bcom_lut_step4 = uisp_bcom_lut_step4;
    be_reg->isp_bcom_lut_step1.u32 = o_isp_bcom_lut_step1.u32;
}

/*  description : set the value of the member isp_bcom_lut_step1.isp_bcom_lut_step5 */
/*  input       : unsigned int uisp_bcom_lut_step5: 4 bits */
static __inline td_void isp_bcom_lut_step5_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step5)
{
    u_isp_bcom_lut_step1 o_isp_bcom_lut_step1;
    o_isp_bcom_lut_step1.u32 = be_reg->isp_bcom_lut_step1.u32;
    o_isp_bcom_lut_step1.bits.isp_bcom_lut_step5 = uisp_bcom_lut_step5;
    be_reg->isp_bcom_lut_step1.u32 = o_isp_bcom_lut_step1.u32;
}

/*  description : set the value of the member isp_bcom_lut_step1.isp_bcom_lut_step6 */
/*  input       : unsigned int uisp_bcom_lut_step6: 4 bits */
static __inline td_void isp_bcom_lut_step6_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step6)
{
    u_isp_bcom_lut_step1 o_isp_bcom_lut_step1;
    o_isp_bcom_lut_step1.u32 = be_reg->isp_bcom_lut_step1.u32;
    o_isp_bcom_lut_step1.bits.isp_bcom_lut_step6 = uisp_bcom_lut_step6;
    be_reg->isp_bcom_lut_step1.u32 = o_isp_bcom_lut_step1.u32;
}

/*  description : set the value of the member isp_bcom_lut_step1.isp_bcom_lut_step7 */
/*  input       : unsigned int uisp_bcom_lut_step7: 4 bits */
static __inline td_void isp_bcom_lut_step7_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_step7)
{
    u_isp_bcom_lut_step1 o_isp_bcom_lut_step1;
    o_isp_bcom_lut_step1.u32 = be_reg->isp_bcom_lut_step1.u32;
    o_isp_bcom_lut_step1.bits.isp_bcom_lut_step7 = uisp_bcom_lut_step7;
    be_reg->isp_bcom_lut_step1.u32 = o_isp_bcom_lut_step1.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg0.isp_bcom_lut_inseg0 */
/*  input       : unsigned int uisp_bcom_lut_inseg0: 9 bits */
static __inline td_void isp_bcom_lut_inseg0_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg0)
{
    u_isp_bcom_lut_inseg0 o_isp_bcom_lut_inseg0;
    o_isp_bcom_lut_inseg0.u32 = be_reg->isp_bcom_lut_inseg0.u32;
    o_isp_bcom_lut_inseg0.bits.isp_bcom_lut_inseg0 = uisp_bcom_lut_inseg0;
    be_reg->isp_bcom_lut_inseg0.u32 = o_isp_bcom_lut_inseg0.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg0.isp_bcom_lut_inseg1 */
/*  input       : unsigned int uisp_bcom_lut_inseg1: 9 bits */
static __inline td_void isp_bcom_lut_inseg1_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg1)
{
    u_isp_bcom_lut_inseg0 o_isp_bcom_lut_inseg0;
    o_isp_bcom_lut_inseg0.u32 = be_reg->isp_bcom_lut_inseg0.u32;
    o_isp_bcom_lut_inseg0.bits.isp_bcom_lut_inseg1 = uisp_bcom_lut_inseg1;
    be_reg->isp_bcom_lut_inseg0.u32 = o_isp_bcom_lut_inseg0.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg0.isp_bcom_lut_inseg2 */
/*  input       : unsigned int uisp_bcom_lut_inseg2: 9 bits */
static __inline td_void isp_bcom_lut_inseg2_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg2)
{
    u_isp_bcom_lut_inseg0 o_isp_bcom_lut_inseg0;
    o_isp_bcom_lut_inseg0.u32 = be_reg->isp_bcom_lut_inseg0.u32;
    o_isp_bcom_lut_inseg0.bits.isp_bcom_lut_inseg2 = uisp_bcom_lut_inseg2;
    be_reg->isp_bcom_lut_inseg0.u32 = o_isp_bcom_lut_inseg0.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg1.isp_bcom_lut_inseg3 */
/*  input       : unsigned int uisp_bcom_lut_inseg3: 9 bits */
static __inline td_void isp_bcom_lut_inseg3_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg3)
{
    u_isp_bcom_lut_inseg1 o_isp_bcom_lut_inseg1;
    o_isp_bcom_lut_inseg1.u32 = be_reg->isp_bcom_lut_inseg1.u32;
    o_isp_bcom_lut_inseg1.bits.isp_bcom_lut_inseg3 = uisp_bcom_lut_inseg3;
    be_reg->isp_bcom_lut_inseg1.u32 = o_isp_bcom_lut_inseg1.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg1.isp_bcom_lut_inseg4 */
/*  input       : unsigned int uisp_bcom_lut_inseg4: 9 bits */
static __inline td_void isp_bcom_lut_inseg4_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg4)
{
    u_isp_bcom_lut_inseg1 o_isp_bcom_lut_inseg1;
    o_isp_bcom_lut_inseg1.u32 = be_reg->isp_bcom_lut_inseg1.u32;
    o_isp_bcom_lut_inseg1.bits.isp_bcom_lut_inseg4 = uisp_bcom_lut_inseg4;
    be_reg->isp_bcom_lut_inseg1.u32 = o_isp_bcom_lut_inseg1.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg1.isp_bcom_lut_inseg5 */
/*  input       : unsigned int uisp_bcom_lut_inseg5: 9 bits */
static __inline td_void isp_bcom_lut_inseg5_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg5)
{
    u_isp_bcom_lut_inseg1 o_isp_bcom_lut_inseg1;
    o_isp_bcom_lut_inseg1.u32 = be_reg->isp_bcom_lut_inseg1.u32;
    o_isp_bcom_lut_inseg1.bits.isp_bcom_lut_inseg5 = uisp_bcom_lut_inseg5;
    be_reg->isp_bcom_lut_inseg1.u32 = o_isp_bcom_lut_inseg1.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg2.isp_bcom_lut_inseg6 */
/*  input       : unsigned int uisp_bcom_lut_inseg6: 9 bits */
static __inline td_void isp_bcom_lut_inseg6_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg6)
{
    u_isp_bcom_lut_inseg2 o_isp_bcom_lut_inseg2;
    o_isp_bcom_lut_inseg2.u32 = be_reg->isp_bcom_lut_inseg2.u32;
    o_isp_bcom_lut_inseg2.bits.isp_bcom_lut_inseg6 = uisp_bcom_lut_inseg6;
    be_reg->isp_bcom_lut_inseg2.u32 = o_isp_bcom_lut_inseg2.u32;
}

/*  description : set the value of the member isp_bcom_lut_inseg2.isp_bcom_lut_inseg7 */
/*  input       : unsigned int uisp_bcom_lut_inseg7: 9 bits */
static __inline td_void isp_bcom_lut_inseg7_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_inseg7)
{
    u_isp_bcom_lut_inseg2 o_isp_bcom_lut_inseg2;
    o_isp_bcom_lut_inseg2.u32 = be_reg->isp_bcom_lut_inseg2.u32;
    o_isp_bcom_lut_inseg2.bits.isp_bcom_lut_inseg7 = uisp_bcom_lut_inseg7;
    be_reg->isp_bcom_lut_inseg2.u32 = o_isp_bcom_lut_inseg2.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos0.isp_bcom_lut_pos0 */
/*  input       : unsigned int uisp_bcom_lut_pos0: 18 bits */
static __inline td_void isp_bcom_lut_pos0_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos0)
{
    u_isp_bcom_lut_pos0 o_isp_bcom_lut_pos0;
    o_isp_bcom_lut_pos0.u32 = be_reg->isp_bcom_lut_pos0.u32;
    o_isp_bcom_lut_pos0.bits.isp_bcom_lut_pos0 = uisp_bcom_lut_pos0;
    be_reg->isp_bcom_lut_pos0.u32 = o_isp_bcom_lut_pos0.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos1.isp_bcom_lut_pos1 */
/*  input       : unsigned int uisp_bcom_lut_pos1: 18 bits */
static __inline td_void isp_bcom_lut_pos1_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos1)
{
    u_isp_bcom_lut_pos1 o_isp_bcom_lut_pos1;
    o_isp_bcom_lut_pos1.u32 = be_reg->isp_bcom_lut_pos1.u32;
    o_isp_bcom_lut_pos1.bits.isp_bcom_lut_pos1 = uisp_bcom_lut_pos1;
    be_reg->isp_bcom_lut_pos1.u32 = o_isp_bcom_lut_pos1.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos2.isp_bcom_lut_pos2 */
/*  input       : unsigned int uisp_bcom_lut_pos2: 18 bits */
static __inline td_void isp_bcom_lut_pos2_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos2)
{
    u_isp_bcom_lut_pos2 o_isp_bcom_lut_pos2;
    o_isp_bcom_lut_pos2.u32 = be_reg->isp_bcom_lut_pos2.u32;
    o_isp_bcom_lut_pos2.bits.isp_bcom_lut_pos2 = uisp_bcom_lut_pos2;
    be_reg->isp_bcom_lut_pos2.u32 = o_isp_bcom_lut_pos2.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos3.isp_bcom_lut_pos3 */
/*  input       : unsigned int uisp_bcom_lut_pos3: 18 bits */
static __inline td_void isp_bcom_lut_pos3_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos3)
{
    u_isp_bcom_lut_pos3 o_isp_bcom_lut_pos3;
    o_isp_bcom_lut_pos3.u32 = be_reg->isp_bcom_lut_pos3.u32;
    o_isp_bcom_lut_pos3.bits.isp_bcom_lut_pos3 = uisp_bcom_lut_pos3;
    be_reg->isp_bcom_lut_pos3.u32 = o_isp_bcom_lut_pos3.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos4.isp_bcom_lut_pos4 */
/*  input       : unsigned int uisp_bcom_lut_pos4: 18 bits */
static __inline td_void isp_bcom_lut_pos4_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos4)
{
    u_isp_bcom_lut_pos4 o_isp_bcom_lut_pos4;
    o_isp_bcom_lut_pos4.u32 = be_reg->isp_bcom_lut_pos4.u32;
    o_isp_bcom_lut_pos4.bits.isp_bcom_lut_pos4 = uisp_bcom_lut_pos4;
    be_reg->isp_bcom_lut_pos4.u32 = o_isp_bcom_lut_pos4.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos5.isp_bcom_lut_pos5 */
/*  input       : unsigned int uisp_bcom_lut_pos5: 18 bits */
static __inline td_void isp_bcom_lut_pos5_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos5)
{
    u_isp_bcom_lut_pos5 o_isp_bcom_lut_pos5;
    o_isp_bcom_lut_pos5.u32 = be_reg->isp_bcom_lut_pos5.u32;
    o_isp_bcom_lut_pos5.bits.isp_bcom_lut_pos5 = uisp_bcom_lut_pos5;
    be_reg->isp_bcom_lut_pos5.u32 = o_isp_bcom_lut_pos5.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos6.isp_bcom_lut_pos6 */
/*  input       : unsigned int uisp_bcom_lut_pos6: 18 bits */
static __inline td_void isp_bcom_lut_pos6_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos6)
{
    u_isp_bcom_lut_pos6 o_isp_bcom_lut_pos6;
    o_isp_bcom_lut_pos6.u32 = be_reg->isp_bcom_lut_pos6.u32;
    o_isp_bcom_lut_pos6.bits.isp_bcom_lut_pos6 = uisp_bcom_lut_pos6;
    be_reg->isp_bcom_lut_pos6.u32 = o_isp_bcom_lut_pos6.u32;
}

/*  description : set the value of the member isp_bcom_lut_pos7.isp_bcom_lut_pos7 */
/*  input       : unsigned int uisp_bcom_lut_pos7: 18 bits */
static __inline td_void isp_bcom_lut_pos7_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_lut_pos7)
{
    u_isp_bcom_lut_pos7 o_isp_bcom_lut_pos7;
    o_isp_bcom_lut_pos7.u32 = be_reg->isp_bcom_lut_pos7.u32;
    o_isp_bcom_lut_pos7.bits.isp_bcom_lut_pos7 = uisp_bcom_lut_pos7;
    be_reg->isp_bcom_lut_pos7.u32 = o_isp_bcom_lut_pos7.u32;
}

/*  description : set the value of the member isp_bcom_gamma_lut_waddr.isp_bcom_gamma_lut_waddr */
/*  input       : unsigned int uisp_bcom_gamma_lut_waddr: 9 bits */
static __inline td_void isp_bcom_gamma_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_gamma_lut_waddr)
{
    u_isp_bcom_gamma_lut_waddr o_isp_bcom_gamma_lut_waddr;
    o_isp_bcom_gamma_lut_waddr.u32 = be_reg->isp_bcom_gamma_lut_waddr.u32;
    o_isp_bcom_gamma_lut_waddr.bits.isp_bcom_gamma_lut_waddr = uisp_bcom_gamma_lut_waddr;
    be_reg->isp_bcom_gamma_lut_waddr.u32 = o_isp_bcom_gamma_lut_waddr.u32;
}

/*  description : set the value of the member isp_bcom_gamma_lut_wdata.isp_bcom_gamma_lut_wdata */
/*  input       : unsigned int uisp_bcom_gamma_lut_wdata: 13 bits */
static __inline td_void isp_bcom_gamma_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_gamma_lut_wdata)
{
    u_isp_bcom_gamma_lut_wdata o_isp_bcom_gamma_lut_wdata;
    o_isp_bcom_gamma_lut_wdata.u32 = be_reg->isp_bcom_gamma_lut_wdata.u32;
    o_isp_bcom_gamma_lut_wdata.bits.isp_bcom_gamma_lut_wdata = uisp_bcom_gamma_lut_wdata;
    be_reg->isp_bcom_gamma_lut_wdata.u32 = o_isp_bcom_gamma_lut_wdata.u32;
}

/*  description : set the value of the member isp_bcom_gamma_lut_raddr.isp_bcom_gamma_lut_raddr */
/*  input       : unsigned int uisp_bcom_gamma_lut_raddr: 9 bits */
static __inline td_void isp_bcom_gamma_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bcom_gamma_lut_raddr)
{
    u_isp_bcom_gamma_lut_raddr o_isp_bcom_gamma_lut_raddr;
    o_isp_bcom_gamma_lut_raddr.u32 = be_reg->isp_bcom_gamma_lut_raddr.u32;
    o_isp_bcom_gamma_lut_raddr.bits.isp_bcom_gamma_lut_raddr = uisp_bcom_gamma_lut_raddr;
    be_reg->isp_bcom_gamma_lut_raddr.u32 = o_isp_bcom_gamma_lut_raddr.u32;
}

/*  description : set the value of the member isp_bdec_lut_step0.isp_bdec_lut_step0 */
/*  input       : unsigned int uisp_bdec_lut_step0: 4 bits */
static __inline td_void isp_bdec_lut_step0_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step0)
{
    u_isp_bdec_lut_step0 o_isp_bdec_lut_step0;
    o_isp_bdec_lut_step0.u32 = be_reg->isp_bdec_lut_step0.u32;
    o_isp_bdec_lut_step0.bits.isp_bdec_lut_step0 = uisp_bdec_lut_step0;
    be_reg->isp_bdec_lut_step0.u32 = o_isp_bdec_lut_step0.u32;
}

/*  description : set the value of the member isp_bdec_lut_step0.isp_bdec_lut_step1 */
/*  input       : unsigned int uisp_bdec_lut_step1: 4 bits */
static __inline td_void isp_bdec_lut_step1_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step1)
{
    u_isp_bdec_lut_step0 o_isp_bdec_lut_step0;
    o_isp_bdec_lut_step0.u32 = be_reg->isp_bdec_lut_step0.u32;
    o_isp_bdec_lut_step0.bits.isp_bdec_lut_step1 = uisp_bdec_lut_step1;
    be_reg->isp_bdec_lut_step0.u32 = o_isp_bdec_lut_step0.u32;
}

/*  description : set the value of the member isp_bdec_lut_step0.isp_bdec_lut_step2 */
/*  input       : unsigned int uisp_bdec_lut_step2: 4 bits */
static __inline td_void isp_bdec_lut_step2_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step2)
{
    u_isp_bdec_lut_step0 o_isp_bdec_lut_step0;
    o_isp_bdec_lut_step0.u32 = be_reg->isp_bdec_lut_step0.u32;
    o_isp_bdec_lut_step0.bits.isp_bdec_lut_step2 = uisp_bdec_lut_step2;
    be_reg->isp_bdec_lut_step0.u32 = o_isp_bdec_lut_step0.u32;
}

/*  description : set the value of the member isp_bdec_lut_step0.isp_bdec_lut_step3 */
/*  input       : unsigned int uisp_bdec_lut_step3: 4 bits */
static __inline td_void isp_bdec_lut_step3_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step3)
{
    u_isp_bdec_lut_step0 o_isp_bdec_lut_step0;
    o_isp_bdec_lut_step0.u32 = be_reg->isp_bdec_lut_step0.u32;
    o_isp_bdec_lut_step0.bits.isp_bdec_lut_step3 = uisp_bdec_lut_step3;
    be_reg->isp_bdec_lut_step0.u32 = o_isp_bdec_lut_step0.u32;
}

/*  description : set the value of the member isp_bdec_lut_step1.isp_bdec_lut_step4 */
/*  input       : unsigned int uisp_bdec_lut_step4: 4 bits */
static __inline td_void isp_bdec_lut_step4_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step4)
{
    u_isp_bdec_lut_step1 o_isp_bdec_lut_step1;
    o_isp_bdec_lut_step1.u32 = be_reg->isp_bdec_lut_step1.u32;
    o_isp_bdec_lut_step1.bits.isp_bdec_lut_step4 = uisp_bdec_lut_step4;
    be_reg->isp_bdec_lut_step1.u32 = o_isp_bdec_lut_step1.u32;
}

/*  description : set the value of the member isp_bdec_lut_step1.isp_bdec_lut_step5 */
/*  input       : unsigned int uisp_bdec_lut_step5: 4 bits */
static __inline td_void isp_bdec_lut_step5_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step5)
{
    u_isp_bdec_lut_step1 o_isp_bdec_lut_step1;
    o_isp_bdec_lut_step1.u32 = be_reg->isp_bdec_lut_step1.u32;
    o_isp_bdec_lut_step1.bits.isp_bdec_lut_step5 = uisp_bdec_lut_step5;
    be_reg->isp_bdec_lut_step1.u32 = o_isp_bdec_lut_step1.u32;
}

/*  description : set the value of the member isp_bdec_lut_step1.isp_bdec_lut_step6 */
/*  input       : unsigned int uisp_bdec_lut_step6: 4 bits */
static __inline td_void isp_bdec_lut_step6_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step6)
{
    u_isp_bdec_lut_step1 o_isp_bdec_lut_step1;
    o_isp_bdec_lut_step1.u32 = be_reg->isp_bdec_lut_step1.u32;
    o_isp_bdec_lut_step1.bits.isp_bdec_lut_step6 = uisp_bdec_lut_step6;
    be_reg->isp_bdec_lut_step1.u32 = o_isp_bdec_lut_step1.u32;
}

/*  description : set the value of the member isp_bdec_lut_step1.isp_bdec_lut_step7 */
/*  input       : unsigned int uisp_bdec_lut_step7: 4 bits */
static __inline td_void isp_bdec_lut_step7_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_step7)
{
    u_isp_bdec_lut_step1 o_isp_bdec_lut_step1;
    o_isp_bdec_lut_step1.u32 = be_reg->isp_bdec_lut_step1.u32;
    o_isp_bdec_lut_step1.bits.isp_bdec_lut_step7 = uisp_bdec_lut_step7;
    be_reg->isp_bdec_lut_step1.u32 = o_isp_bdec_lut_step1.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg0.isp_bdec_lut_inseg0 */
/*  input       : unsigned int uisp_bdec_lut_inseg0: 9 bits */
static __inline td_void isp_bdec_lut_inseg0_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg0)
{
    u_isp_bdec_lut_inseg0 o_isp_bdec_lut_inseg0;
    o_isp_bdec_lut_inseg0.u32 = be_reg->isp_bdec_lut_inseg0.u32;
    o_isp_bdec_lut_inseg0.bits.isp_bdec_lut_inseg0 = uisp_bdec_lut_inseg0;
    be_reg->isp_bdec_lut_inseg0.u32 = o_isp_bdec_lut_inseg0.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg0.isp_bdec_lut_inseg1 */
/*  input       : unsigned int uisp_bdec_lut_inseg1: 9 bits */
static __inline td_void isp_bdec_lut_inseg1_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg1)
{
    u_isp_bdec_lut_inseg0 o_isp_bdec_lut_inseg0;
    o_isp_bdec_lut_inseg0.u32 = be_reg->isp_bdec_lut_inseg0.u32;
    o_isp_bdec_lut_inseg0.bits.isp_bdec_lut_inseg1 = uisp_bdec_lut_inseg1;
    be_reg->isp_bdec_lut_inseg0.u32 = o_isp_bdec_lut_inseg0.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg0.isp_bdec_lut_inseg2 */
/*  input       : unsigned int uisp_bdec_lut_inseg2: 9 bits */
static __inline td_void isp_bdec_lut_inseg2_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg2)
{
    u_isp_bdec_lut_inseg0 o_isp_bdec_lut_inseg0;
    o_isp_bdec_lut_inseg0.u32 = be_reg->isp_bdec_lut_inseg0.u32;
    o_isp_bdec_lut_inseg0.bits.isp_bdec_lut_inseg2 = uisp_bdec_lut_inseg2;
    be_reg->isp_bdec_lut_inseg0.u32 = o_isp_bdec_lut_inseg0.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg1.isp_bdec_lut_inseg3 */
/*  input       : unsigned int uisp_bdec_lut_inseg3: 9 bits */
static __inline td_void isp_bdec_lut_inseg3_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg3)
{
    u_isp_bdec_lut_inseg1 o_isp_bdec_lut_inseg1;
    o_isp_bdec_lut_inseg1.u32 = be_reg->isp_bdec_lut_inseg1.u32;
    o_isp_bdec_lut_inseg1.bits.isp_bdec_lut_inseg3 = uisp_bdec_lut_inseg3;
    be_reg->isp_bdec_lut_inseg1.u32 = o_isp_bdec_lut_inseg1.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg1.isp_bdec_lut_inseg4 */
/*  input       : unsigned int uisp_bdec_lut_inseg4: 9 bits */
static __inline td_void isp_bdec_lut_inseg4_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg4)
{
    u_isp_bdec_lut_inseg1 o_isp_bdec_lut_inseg1;
    o_isp_bdec_lut_inseg1.u32 = be_reg->isp_bdec_lut_inseg1.u32;
    o_isp_bdec_lut_inseg1.bits.isp_bdec_lut_inseg4 = uisp_bdec_lut_inseg4;
    be_reg->isp_bdec_lut_inseg1.u32 = o_isp_bdec_lut_inseg1.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg1.isp_bdec_lut_inseg5 */
/*  input       : unsigned int uisp_bdec_lut_inseg5: 9 bits */
static __inline td_void isp_bdec_lut_inseg5_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg5)
{
    u_isp_bdec_lut_inseg1 o_isp_bdec_lut_inseg1;
    o_isp_bdec_lut_inseg1.u32 = be_reg->isp_bdec_lut_inseg1.u32;
    o_isp_bdec_lut_inseg1.bits.isp_bdec_lut_inseg5 = uisp_bdec_lut_inseg5;
    be_reg->isp_bdec_lut_inseg1.u32 = o_isp_bdec_lut_inseg1.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg2.isp_bdec_lut_inseg6 */
/*  input       : unsigned int uisp_bdec_lut_inseg6: 9 bits */
static __inline td_void isp_bdec_lut_inseg6_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg6)
{
    u_isp_bdec_lut_inseg2 o_isp_bdec_lut_inseg2;
    o_isp_bdec_lut_inseg2.u32 = be_reg->isp_bdec_lut_inseg2.u32;
    o_isp_bdec_lut_inseg2.bits.isp_bdec_lut_inseg6 = uisp_bdec_lut_inseg6;
    be_reg->isp_bdec_lut_inseg2.u32 = o_isp_bdec_lut_inseg2.u32;
}

/*  description : set the value of the member isp_bdec_lut_inseg2.isp_bdec_lut_inseg7 */
/*  input       : unsigned int uisp_bdec_lut_inseg7: 9 bits */
static __inline td_void isp_bdec_lut_inseg7_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_inseg7)
{
    u_isp_bdec_lut_inseg2 o_isp_bdec_lut_inseg2;
    o_isp_bdec_lut_inseg2.u32 = be_reg->isp_bdec_lut_inseg2.u32;
    o_isp_bdec_lut_inseg2.bits.isp_bdec_lut_inseg7 = uisp_bdec_lut_inseg7;
    be_reg->isp_bdec_lut_inseg2.u32 = o_isp_bdec_lut_inseg2.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos0.isp_bdec_lut_pos0 */
/*  input       : unsigned int uisp_bdec_lut_pos0: 13 bits */
static __inline td_void isp_bdec_lut_pos0_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos0)
{
    u_isp_bdec_lut_pos0 o_isp_bdec_lut_pos0;
    o_isp_bdec_lut_pos0.u32 = be_reg->isp_bdec_lut_pos0.u32;
    o_isp_bdec_lut_pos0.bits.isp_bdec_lut_pos0 = uisp_bdec_lut_pos0;
    be_reg->isp_bdec_lut_pos0.u32 = o_isp_bdec_lut_pos0.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos0.isp_bdec_lut_pos1 */
/*  input       : unsigned int uisp_bdec_lut_pos1: 13 bits */
static __inline td_void isp_bdec_lut_pos1_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos1)
{
    u_isp_bdec_lut_pos0 o_isp_bdec_lut_pos0;
    o_isp_bdec_lut_pos0.u32 = be_reg->isp_bdec_lut_pos0.u32;
    o_isp_bdec_lut_pos0.bits.isp_bdec_lut_pos1 = uisp_bdec_lut_pos1;
    be_reg->isp_bdec_lut_pos0.u32 = o_isp_bdec_lut_pos0.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos1.isp_bdec_lut_pos2 */
/*  input       : unsigned int uisp_bdec_lut_pos2: 13 bits */
static __inline td_void isp_bdec_lut_pos2_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos2)
{
    u_isp_bdec_lut_pos1 o_isp_bdec_lut_pos1;
    o_isp_bdec_lut_pos1.u32 = be_reg->isp_bdec_lut_pos1.u32;
    o_isp_bdec_lut_pos1.bits.isp_bdec_lut_pos2 = uisp_bdec_lut_pos2;
    be_reg->isp_bdec_lut_pos1.u32 = o_isp_bdec_lut_pos1.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos1.isp_bdec_lut_pos3 */
/*  input       : unsigned int uisp_bdec_lut_pos3: 13 bits */
static __inline td_void isp_bdec_lut_pos3_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos3)
{
    u_isp_bdec_lut_pos1 o_isp_bdec_lut_pos1;
    o_isp_bdec_lut_pos1.u32 = be_reg->isp_bdec_lut_pos1.u32;
    o_isp_bdec_lut_pos1.bits.isp_bdec_lut_pos3 = uisp_bdec_lut_pos3;
    be_reg->isp_bdec_lut_pos1.u32 = o_isp_bdec_lut_pos1.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos2.isp_bdec_lut_pos4 */
/*  input       : unsigned int uisp_bdec_lut_pos4: 13 bits */
static __inline td_void isp_bdec_lut_pos4_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos4)
{
    u_isp_bdec_lut_pos2 o_isp_bdec_lut_pos2;
    o_isp_bdec_lut_pos2.u32 = be_reg->isp_bdec_lut_pos2.u32;
    o_isp_bdec_lut_pos2.bits.isp_bdec_lut_pos4 = uisp_bdec_lut_pos4;
    be_reg->isp_bdec_lut_pos2.u32 = o_isp_bdec_lut_pos2.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos2.isp_bdec_lut_pos5 */
/*  input       : unsigned int uisp_bdec_lut_pos5: 13 bits */
static __inline td_void isp_bdec_lut_pos5_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos5)
{
    u_isp_bdec_lut_pos2 o_isp_bdec_lut_pos2;
    o_isp_bdec_lut_pos2.u32 = be_reg->isp_bdec_lut_pos2.u32;
    o_isp_bdec_lut_pos2.bits.isp_bdec_lut_pos5 = uisp_bdec_lut_pos5;
    be_reg->isp_bdec_lut_pos2.u32 = o_isp_bdec_lut_pos2.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos3.isp_bdec_lut_pos6 */
/*  input       : unsigned int uisp_bdec_lut_pos6: 13 bits */
static __inline td_void isp_bdec_lut_pos6_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos6)
{
    u_isp_bdec_lut_pos3 o_isp_bdec_lut_pos3;
    o_isp_bdec_lut_pos3.u32 = be_reg->isp_bdec_lut_pos3.u32;
    o_isp_bdec_lut_pos3.bits.isp_bdec_lut_pos6 = uisp_bdec_lut_pos6;
    be_reg->isp_bdec_lut_pos3.u32 = o_isp_bdec_lut_pos3.u32;
}

/*  description : set the value of the member isp_bdec_lut_pos3.isp_bdec_lut_pos7 */
/*  input       : unsigned int uisp_bdec_lut_pos7: 13 bits */
static __inline td_void isp_bdec_lut_pos7_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_lut_pos7)
{
    u_isp_bdec_lut_pos3 o_isp_bdec_lut_pos3;
    o_isp_bdec_lut_pos3.u32 = be_reg->isp_bdec_lut_pos3.u32;
    o_isp_bdec_lut_pos3.bits.isp_bdec_lut_pos7 = uisp_bdec_lut_pos7;
    be_reg->isp_bdec_lut_pos3.u32 = o_isp_bdec_lut_pos3.u32;
}

/*  description : set the value of the member isp_bdec_gamma_lut_waddr.isp_bdec_gamma_lut_waddr */
/*  input       : unsigned int uisp_bdec_gamma_lut_waddr: 9 bits */
static __inline td_void isp_bdec_gamma_lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_gamma_lut_waddr)
{
    u_isp_bdec_gamma_lut_waddr o_isp_bdec_gamma_lut_waddr;
    o_isp_bdec_gamma_lut_waddr.u32 = be_reg->isp_bdec_gamma_lut_waddr.u32;
    o_isp_bdec_gamma_lut_waddr.bits.isp_bdec_gamma_lut_waddr = uisp_bdec_gamma_lut_waddr;
    be_reg->isp_bdec_gamma_lut_waddr.u32 = o_isp_bdec_gamma_lut_waddr.u32;
}

/*  description : set the value of the member isp_bdec_gamma_lut_wdata.isp_bdec_gamma_lut_wdata */
/*  input       : unsigned int uisp_bdec_gamma_lut_wdata: 18 bits */
static __inline td_void isp_bdec_gamma_lut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_gamma_lut_wdata)
{
    u_isp_bdec_gamma_lut_wdata o_isp_bdec_gamma_lut_wdata;
    o_isp_bdec_gamma_lut_wdata.u32 = be_reg->isp_bdec_gamma_lut_wdata.u32;
    o_isp_bdec_gamma_lut_wdata.bits.isp_bdec_gamma_lut_wdata = uisp_bdec_gamma_lut_wdata;
    be_reg->isp_bdec_gamma_lut_wdata.u32 = o_isp_bdec_gamma_lut_wdata.u32;
}

/*  description : set the value of the member isp_bdec_gamma_lut_raddr.isp_bdec_gamma_lut_raddr */
/*  input       : unsigned int uisp_bdec_gamma_lut_raddr: 9 bits */
static __inline td_void isp_bdec_gamma_lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_bdec_gamma_lut_raddr)
{
    u_isp_bdec_gamma_lut_raddr o_isp_bdec_gamma_lut_raddr;
    o_isp_bdec_gamma_lut_raddr.u32 = be_reg->isp_bdec_gamma_lut_raddr.u32;
    o_isp_bdec_gamma_lut_raddr.bits.isp_bdec_gamma_lut_raddr = uisp_bdec_gamma_lut_raddr;
    be_reg->isp_bdec_gamma_lut_raddr.u32 = o_isp_bdec_gamma_lut_raddr.u32;
}

/*  description   set the value of the member isp_ca_ctrl.isp_ca_satvssat_en */
/*  input         unsigned int uisp_ca_satvssat_en  1 bits */
static __inline td_void isp_ca_satvssat_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_satvssat_en)
{
    u_isp_ca_ctrl o_isp_ca_ctrl;
    o_isp_ca_ctrl.u32 = be_reg->isp_ca_ctrl.u32;
    o_isp_ca_ctrl.bits.isp_ca_satvssat_en = uisp_ca_satvssat_en;
    be_reg->isp_ca_ctrl.u32 = o_isp_ca_ctrl.u32;
}
/*  description   set the value of the member isp_ca_ctrl.isp_ca_skinproc_en */
/*  input         unsigned int uisp_ca_skinproc_en  1 bits */
static __inline td_void isp_ca_skinproc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_skinproc_en)
{
    u_isp_ca_ctrl o_isp_ca_ctrl;
    o_isp_ca_ctrl.u32 = be_reg->isp_ca_ctrl.u32;
    o_isp_ca_ctrl.bits.isp_ca_skinproc_en = uisp_ca_skinproc_en;
    be_reg->isp_ca_ctrl.u32 = o_isp_ca_ctrl.u32;
}
/*  description   set the value of the member isp_ca_ctrl.isp_ca_llhcproc_en */
/*  input         unsigned int uisp_ca_llhcproc_en  1 bits */
static __inline td_void isp_ca_llhcproc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_llhcproc_en)
{
    u_isp_ca_ctrl o_isp_ca_ctrl;
    o_isp_ca_ctrl.u32 = be_reg->isp_ca_ctrl.u32;
    o_isp_ca_ctrl.bits.isp_ca_llhcproc_en = uisp_ca_llhcproc_en;
    be_reg->isp_ca_ctrl.u32 = o_isp_ca_ctrl.u32;
}

/*  description   set the value of the member isp_ca_ctrl.isp_ca_satadj_en */
/*  input         unsigned int uisp_ca_satadj_en  1 bits */
static __inline td_void isp_ca_satadj_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_satadj_en)
{
    u_isp_ca_ctrl o_isp_ca_ctrl;
    o_isp_ca_ctrl.u32 = be_reg->isp_ca_ctrl.u32;
    o_isp_ca_ctrl.bits.isp_ca_satadj_en = uisp_ca_satadj_en;
    be_reg->isp_ca_ctrl.u32 = o_isp_ca_ctrl.u32;
}
/*  description   set the value of the member isp_ca_des.isp_ca_des_mix */
/*  input         unsigned int uisp_ca_des_mix  8 bits */
static __inline td_void isp_ca_des_mix_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_des_mix)
{
    u_isp_ca_des o_isp_ca_des;
    o_isp_ca_des.u32 = be_reg->isp_ca_des.u32;
    o_isp_ca_des.bits.isp_ca_des_mix = uisp_ca_des_mix;
    be_reg->isp_ca_des.u32 = o_isp_ca_des.u32;
}

/*  description   set the value of the member isp_ca_des.isp_ca_des_point */
/*  input         unsigned int uisp_ca_des_point  11 bits */
static __inline td_void isp_ca_des_point_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_des_point)
{
    u_isp_ca_des o_isp_ca_des;
    o_isp_ca_des.u32 = be_reg->isp_ca_des.u32;
    o_isp_ca_des.bits.isp_ca_des_point = uisp_ca_des_point;
    be_reg->isp_ca_des.u32 = o_isp_ca_des.u32;
}

/*  description   set the value of the member isp_ca_isoratio.isp_ca_isoratio */
/*  input         unsigned int uisp_ca_isoratio  11 bits */
static __inline td_void isp_ca_isoratio_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_isoratio)
{
    u_isp_ca_isoratio o_isp_ca_isoratio;
    o_isp_ca_isoratio.u32 = be_reg->isp_ca_isoratio.u32;
    o_isp_ca_isoratio.bits.isp_ca_isoratio = uisp_ca_isoratio;
    be_reg->isp_ca_isoratio.u32 = o_isp_ca_isoratio.u32;
}

/*  description   set the value of the member isp_ca_sat_coef.isp_ca_sat_c1 */
/*  input         unsigned int uisp_ca_sat_c1  7 bits */
static __inline td_void isp_ca_sat_c1_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_sat_c1)
{
    u_isp_ca_sat_coef o_isp_ca_sat_coef;
    o_isp_ca_sat_coef.u32 = be_reg->isp_ca_sat_coef.u32;
    o_isp_ca_sat_coef.bits.isp_ca_sat_c1 = uisp_ca_sat_c1;
    be_reg->isp_ca_sat_coef.u32 = o_isp_ca_sat_coef.u32;
}

/*  description   set the value of the member isp_ca_sat_coef.isp_ca_sat_c2 */
/*  input         unsigned int uisp_ca_sat_c2  7 bits */
static __inline td_void isp_ca_sat_c2_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_sat_c2)
{
    u_isp_ca_sat_coef o_isp_ca_sat_coef;
    o_isp_ca_sat_coef.u32 = be_reg->isp_ca_sat_coef.u32;
    o_isp_ca_sat_coef.bits.isp_ca_sat_c2 = uisp_ca_sat_c2;
    be_reg->isp_ca_sat_coef.u32 = o_isp_ca_sat_coef.u32;
}

/*  description   set the value of the member isp_ca_yratiolut_waddr.isp_ca_yratiolut_waddr */
/*  input         unsigned int uisp_ca_yratiolut_waddr  8 bits */
static __inline td_void isp_ca_yratiolut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yratiolut_waddr)
{
    u_isp_ca_yratiolut_waddr o_isp_ca_yratiolut_waddr;
    o_isp_ca_yratiolut_waddr.u32 = be_reg->isp_ca_yratiolut_waddr.u32;
    o_isp_ca_yratiolut_waddr.bits.isp_ca_yratiolut_waddr = uisp_ca_yratiolut_waddr;
    be_reg->isp_ca_yratiolut_waddr.u32 = o_isp_ca_yratiolut_waddr.u32;
}

/*  description   set the value of the member isp_ca_yratiolut_wdata.isp_ca_yratiolut_wdata */
/*  input         unsigned int uisp_ca_yratiolut_wdata  24 bits */
static __inline td_void isp_ca_yratiolut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yratiolut_wdata)
{
    u_isp_ca_yratiolut_wdata o_isp_ca_yratiolut_wdata;
    o_isp_ca_yratiolut_wdata.u32 = be_reg->isp_ca_yratiolut_wdata.u32;
    o_isp_ca_yratiolut_wdata.bits.isp_ca_yratiolut_wdata = uisp_ca_yratiolut_wdata;
    be_reg->isp_ca_yratiolut_wdata.u32 = o_isp_ca_yratiolut_wdata.u32;
}

/*  description   set the value of the member isp_ca_yratiolut_raddr.isp_ca_yratiolut_raddr */
/*  input         unsigned int uisp_ca_yratiolut_raddr  8 bits */
static __inline td_void isp_ca_yratiolut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yratiolut_raddr)
{
    u_isp_ca_yratiolut_raddr o_isp_ca_yratiolut_raddr;
    o_isp_ca_yratiolut_raddr.u32 = be_reg->isp_ca_yratiolut_raddr.u32;
    o_isp_ca_yratiolut_raddr.bits.isp_ca_yratiolut_raddr = uisp_ca_yratiolut_raddr;
    be_reg->isp_ca_yratiolut_raddr.u32 = o_isp_ca_yratiolut_raddr.u32;
}

/*  description   set the value of the member isp_ca_ysatlut_waddr.isp_ca_ysatlut_waddr */
/*  input         unsigned int uisp_ca_ysatlut_waddr  8 bits */
static __inline td_void isp_ca_ysatlut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ysatlut_waddr)
{
    u_isp_ca_ysatlut_waddr o_isp_ca_ysatlut_waddr;
    o_isp_ca_ysatlut_waddr.u32 = be_reg->isp_ca_ysatlut_waddr.u32;
    o_isp_ca_ysatlut_waddr.bits.isp_ca_ysatlut_waddr = uisp_ca_ysatlut_waddr;
    be_reg->isp_ca_ysatlut_waddr.u32 = o_isp_ca_ysatlut_waddr.u32;
}

/*  description   set the value of the member isp_ca_ysatlut_wdata.isp_ca_ysatlut_wdata */
/*  input         unsigned int uisp_ca_ysatlut_wdata  11 bits */
static __inline td_void isp_ca_ysatlut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ysatlut_wdata)
{
    u_isp_ca_ysatlut_wdata o_isp_ca_ysatlut_wdata;
    o_isp_ca_ysatlut_wdata.u32 = be_reg->isp_ca_ysatlut_wdata.u32;
    o_isp_ca_ysatlut_wdata.bits.isp_ca_ysatlut_wdata = uisp_ca_ysatlut_wdata;
    be_reg->isp_ca_ysatlut_wdata.u32 = o_isp_ca_ysatlut_wdata.u32;
}

/*  description   set the value of the member isp_ca_ysatlut_raddr.isp_ca_ysatlut_raddr */
/*  input         unsigned int uisp_ca_ysatlut_raddr  8 bits */
static __inline td_void isp_ca_ysatlut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ysatlut_raddr)
{
    u_isp_ca_ysatlut_raddr o_isp_ca_ysatlut_raddr;
    o_isp_ca_ysatlut_raddr.u32 = be_reg->isp_ca_ysatlut_raddr.u32;
    o_isp_ca_ysatlut_raddr.bits.isp_ca_ysatlut_raddr = uisp_ca_ysatlut_raddr;
    be_reg->isp_ca_ysatlut_raddr.u32 = o_isp_ca_ysatlut_raddr.u32;
}

/*  description   set the value of the member isp_ca_ych1lut_waddr.isp_ca_ych1lut_waddr */
/*  input         unsigned int uisp_ca_ych1lut_waddr  3 bits */
static __inline td_void isp_ca_ych1lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych1lut_waddr)
{
    u_isp_ca_ych1lut_waddr o_isp_ca_ych1lut_waddr;
    o_isp_ca_ych1lut_waddr.u32 = be_reg->isp_ca_ych1lut_waddr.u32;
    o_isp_ca_ych1lut_waddr.bits.isp_ca_ych1lut_waddr = uisp_ca_ych1lut_waddr;
    be_reg->isp_ca_ych1lut_waddr.u32 = o_isp_ca_ych1lut_waddr.u32;
}

/*  description   set the value of the member isp_ca_ych1lut_wdata.isp_ca_ych1lut_even_wdata */
/*  input         unsigned int uisp_ca_ych1lut_even_wdata  10 bits */
static __inline td_void isp_ca_ych1lut_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych1lut_even_wdata)
{
    u_isp_ca_ych1lut_wdata o_isp_ca_ych1lut_wdata;
    o_isp_ca_ych1lut_wdata.u32 = be_reg->isp_ca_ych1lut_wdata.u32;
    o_isp_ca_ych1lut_wdata.bits.isp_ca_ych1lut_even_wdata = uisp_ca_ych1lut_even_wdata;
    be_reg->isp_ca_ych1lut_wdata.u32 = o_isp_ca_ych1lut_wdata.u32;
}

/*  description   set the value of the member isp_ca_ych1lut_wdata.isp_ca_ych1lut_odd_wdata */
/*  input         unsigned int uisp_ca_ych1lut_odd_wdata  10 bits */
static __inline td_void isp_ca_ych1lut_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych1lut_odd_wdata)
{
    u_isp_ca_ych1lut_wdata o_isp_ca_ych1lut_wdata;
    o_isp_ca_ych1lut_wdata.u32 = be_reg->isp_ca_ych1lut_wdata.u32;
    o_isp_ca_ych1lut_wdata.bits.isp_ca_ych1lut_odd_wdata = uisp_ca_ych1lut_odd_wdata;
    be_reg->isp_ca_ych1lut_wdata.u32 = o_isp_ca_ych1lut_wdata.u32;
}

/*  description   set the value of the member isp_ca_ych1lut_raddr.isp_ca_ych1lut_raddr */
/*  input         unsigned int uisp_ca_ych1lut_raddr  3 bits */
static __inline td_void isp_ca_ych1lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych1lut_raddr)
{
    u_isp_ca_ych1lut_raddr o_isp_ca_ych1lut_raddr;
    o_isp_ca_ych1lut_raddr.u32 = be_reg->isp_ca_ych1lut_raddr.u32;
    o_isp_ca_ych1lut_raddr.bits.isp_ca_ych1lut_raddr = uisp_ca_ych1lut_raddr;
    be_reg->isp_ca_ych1lut_raddr.u32 = o_isp_ca_ych1lut_raddr.u32;
}

/*  description   set the value of the member isp_ca_ych2lut_waddr.isp_ca_ych2lut_waddr */
/*  input         unsigned int uisp_ca_ych2lut_waddr  3 bits */
static __inline td_void isp_ca_ych2lut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych2lut_waddr)
{
    u_isp_ca_ych2lut_waddr o_isp_ca_ych2lut_waddr;
    o_isp_ca_ych2lut_waddr.u32 = be_reg->isp_ca_ych2lut_waddr.u32;
    o_isp_ca_ych2lut_waddr.bits.isp_ca_ych2lut_waddr = uisp_ca_ych2lut_waddr;
    be_reg->isp_ca_ych2lut_waddr.u32 = o_isp_ca_ych2lut_waddr.u32;
}

/*  description   set the value of the member isp_ca_ych2lut_wdata.isp_ca_ych2lut_even_wdata */
/*  input         unsigned int uisp_ca_ych2lut_even_wdata  10 bits */
static __inline td_void isp_ca_ych2lut_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych2lut_even_wdata)
{
    u_isp_ca_ych2lut_wdata o_isp_ca_ych2lut_wdata;
    o_isp_ca_ych2lut_wdata.u32 = be_reg->isp_ca_ych2lut_wdata.u32;
    o_isp_ca_ych2lut_wdata.bits.isp_ca_ych2lut_even_wdata = uisp_ca_ych2lut_even_wdata;
    be_reg->isp_ca_ych2lut_wdata.u32 = o_isp_ca_ych2lut_wdata.u32;
}

/*  description   set the value of the member isp_ca_ych2lut_wdata.isp_ca_ych2lut_odd_wdata */
/*  input         unsigned int uisp_ca_ych2lut_odd_wdata  10 bits */
static __inline td_void isp_ca_ych2lut_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych2lut_odd_wdata)
{
    u_isp_ca_ych2lut_wdata o_isp_ca_ych2lut_wdata;
    o_isp_ca_ych2lut_wdata.u32 = be_reg->isp_ca_ych2lut_wdata.u32;
    o_isp_ca_ych2lut_wdata.bits.isp_ca_ych2lut_odd_wdata = uisp_ca_ych2lut_odd_wdata;
    be_reg->isp_ca_ych2lut_wdata.u32 = o_isp_ca_ych2lut_wdata.u32;
}

/*  description   set the value of the member isp_ca_ych2lut_raddr.isp_ca_ych2lut_raddr */
/*  input         unsigned int uisp_ca_ych2lut_raddr  3 bits */
static __inline td_void isp_ca_ych2lut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_ych2lut_raddr)
{
    u_isp_ca_ych2lut_raddr o_isp_ca_ych2lut_raddr;
    o_isp_ca_ych2lut_raddr.u32 = be_reg->isp_ca_ych2lut_raddr.u32;
    o_isp_ca_ych2lut_raddr.bits.isp_ca_ych2lut_raddr = uisp_ca_ych2lut_raddr;
    be_reg->isp_ca_ych2lut_raddr.u32 = o_isp_ca_ych2lut_raddr.u32;
}

/*  description   set the value of the member isp_ca_yslulut_waddr.isp_ca_yslulut_waddr */
/*  input         unsigned int uisp_ca_yslulut_waddr  3 bits */
static __inline td_void isp_ca_yslulut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslulut_waddr)
{
    u_isp_ca_yslulut_waddr o_isp_ca_yslulut_waddr;
    o_isp_ca_yslulut_waddr.u32 = be_reg->isp_ca_yslulut_waddr.u32;
    o_isp_ca_yslulut_waddr.bits.isp_ca_yslulut_waddr = uisp_ca_yslulut_waddr;
    be_reg->isp_ca_yslulut_waddr.u32 = o_isp_ca_yslulut_waddr.u32;
}

/*  description   set the value of the member isp_ca_yslulut_wdata.isp_ca_yslulut_even_wdata */
/*  input         unsigned int uisp_ca_yslulut_even_wdata  10 bits */
static __inline td_void isp_ca_yslulut_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslulut_even_wdata)
{
    u_isp_ca_yslulut_wdata o_isp_ca_yslulut_wdata;
    o_isp_ca_yslulut_wdata.u32 = be_reg->isp_ca_yslulut_wdata.u32;
    o_isp_ca_yslulut_wdata.bits.isp_ca_yslulut_even_wdata = uisp_ca_yslulut_even_wdata;
    be_reg->isp_ca_yslulut_wdata.u32 = o_isp_ca_yslulut_wdata.u32;
}

/*  description   set the value of the member isp_ca_yslulut_wdata.isp_ca_yslulut_odd_wdata */
/*  input         unsigned int uisp_ca_yslulut_odd_wdata  10 bits */
static __inline td_void isp_ca_yslulut_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslulut_odd_wdata)
{
    u_isp_ca_yslulut_wdata o_isp_ca_yslulut_wdata;
    o_isp_ca_yslulut_wdata.u32 = be_reg->isp_ca_yslulut_wdata.u32;
    o_isp_ca_yslulut_wdata.bits.isp_ca_yslulut_odd_wdata = uisp_ca_yslulut_odd_wdata;
    be_reg->isp_ca_yslulut_wdata.u32 = o_isp_ca_yslulut_wdata.u32;
}

/*  description   set the value of the member isp_ca_yslulut_raddr.isp_ca_yslulut_raddr */
/*  input         unsigned int uisp_ca_yslulut_raddr  3 bits */
static __inline td_void isp_ca_yslulut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslulut_raddr)
{
    u_isp_ca_yslulut_raddr o_isp_ca_yslulut_raddr;
    o_isp_ca_yslulut_raddr.u32 = be_reg->isp_ca_yslulut_raddr.u32;
    o_isp_ca_yslulut_raddr.bits.isp_ca_yslulut_raddr = uisp_ca_yslulut_raddr;
    be_reg->isp_ca_yslulut_raddr.u32 = o_isp_ca_yslulut_raddr.u32;
}

/*  description   set the value of the member isp_ca_yslvlut_waddr.isp_ca_yslvlut_waddr */
/*  input         unsigned int uisp_ca_yslvlut_waddr  3 bits */
static __inline td_void isp_ca_yslvlut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslvlut_waddr)
{
    u_isp_ca_yslvlut_waddr o_isp_ca_yslvlut_waddr;
    o_isp_ca_yslvlut_waddr.u32 = be_reg->isp_ca_yslvlut_waddr.u32;
    o_isp_ca_yslvlut_waddr.bits.isp_ca_yslvlut_waddr = uisp_ca_yslvlut_waddr;
    be_reg->isp_ca_yslvlut_waddr.u32 = o_isp_ca_yslvlut_waddr.u32;
}

/*  description   set the value of the member isp_ca_yslvlut_wdata.isp_ca_yslvlut_even_wdata */
/*  input         unsigned int uisp_ca_yslvlut_even_wdata  10 bits */
static __inline td_void isp_ca_yslvlut_even_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslvlut_even_wdata)
{
    u_isp_ca_yslvlut_wdata o_isp_ca_yslvlut_wdata;
    o_isp_ca_yslvlut_wdata.u32 = be_reg->isp_ca_yslvlut_wdata.u32;
    o_isp_ca_yslvlut_wdata.bits.isp_ca_yslvlut_even_wdata = uisp_ca_yslvlut_even_wdata;
    be_reg->isp_ca_yslvlut_wdata.u32 = o_isp_ca_yslvlut_wdata.u32;
}

/*  description   set the value of the member isp_ca_yslvlut_wdata.isp_ca_yslvlut_odd_wdata */
/*  input         unsigned int uisp_ca_yslvlut_odd_wdata  10 bits */
static __inline td_void isp_ca_yslvlut_odd_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslvlut_odd_wdata)
{
    u_isp_ca_yslvlut_wdata o_isp_ca_yslvlut_wdata;
    o_isp_ca_yslvlut_wdata.u32 = be_reg->isp_ca_yslvlut_wdata.u32;
    o_isp_ca_yslvlut_wdata.bits.isp_ca_yslvlut_odd_wdata = uisp_ca_yslvlut_odd_wdata;
    be_reg->isp_ca_yslvlut_wdata.u32 = o_isp_ca_yslvlut_wdata.u32;
}

/*  description   set the value of the member isp_ca_yslvlut_raddr.isp_ca_yslvlut_raddr */
/*  input         unsigned int uisp_ca_yslvlut_raddr  3 bits */
static __inline td_void isp_ca_yslvlut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_yslvlut_raddr)
{
    u_isp_ca_yslvlut_raddr o_isp_ca_yslvlut_raddr;
    o_isp_ca_yslvlut_raddr.u32 = be_reg->isp_ca_yslvlut_raddr.u32;
    o_isp_ca_yslvlut_raddr.bits.isp_ca_yslvlut_raddr = uisp_ca_yslvlut_raddr;
    be_reg->isp_ca_yslvlut_raddr.u32 = o_isp_ca_yslvlut_raddr.u32;
}
/*  description   set the value of the member isp_ca_stt2lut_cfg.isp_ca_stt2lut_en */
/*  input         unsigned int uisp_ca_stt2lut_en  1 bits */
static __inline td_void isp_ca_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_stt2lut_en)
{
    u_isp_ca_stt2lut_cfg o_isp_ca_stt2lut_cfg;
    o_isp_ca_stt2lut_cfg.u32 = be_reg->isp_ca_stt2lut_cfg.u32;
    o_isp_ca_stt2lut_cfg.bits.isp_ca_stt2lut_en = uisp_ca_stt2lut_en;
    be_reg->isp_ca_stt2lut_cfg.u32 = o_isp_ca_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_ca_stt2lut_regnew.isp_ca_stt2lut_regnew */
/*  input         unsigned int uisp_ca_stt2lut_regnew  1 bits */
static __inline td_void isp_ca_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_stt2lut_regnew)
{
    u_isp_ca_stt2lut_regnew o_isp_ca_stt2lut_regnew;
    o_isp_ca_stt2lut_regnew.u32 = be_reg->isp_ca_stt2lut_regnew.u32;
    o_isp_ca_stt2lut_regnew.bits.isp_ca_stt2lut_regnew = uisp_ca_stt2lut_regnew;
    be_reg->isp_ca_stt2lut_regnew.u32 = o_isp_ca_stt2lut_regnew.u32;
}

/*  description   set the value of the member isp_ca_stt2lut_abn.isp_ca_stt2lut_clr */
/*  input         unsigned int uisp_ca_stt2lut_clr  1 bits */
static __inline td_void isp_ca_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_stt2lut_clr)
{
    u_isp_ca_stt2lut_abn o_isp_ca_stt2lut_abn;
    o_isp_ca_stt2lut_abn.u32 = be_reg->isp_ca_stt2lut_abn.u32;
    o_isp_ca_stt2lut_abn.bits.isp_ca_stt2lut_clr = uisp_ca_stt2lut_clr;
    be_reg->isp_ca_stt2lut_abn.u32 = o_isp_ca_stt2lut_abn.u32;
}

static __inline td_u16 isp_ca_stt2lut_info_read(isp_be_reg_type *be_reg)
{
    return be_reg->isp_ca_stt2lut_abn.bits.isp_ca_stt2lut_info;
}


/*  description   set the value of the member isp_ldci_cfg.isp_ldci_calc_en */
/*  input         unsigned int uisp_ldci_calc_en  1 bits */
static __inline td_void isp_ldci_calc_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_calc_en)
{
    u_isp_ldci_cfg o_isp_ldci_cfg;
    o_isp_ldci_cfg.u32 = be_reg->isp_ldci_cfg.u32;
    o_isp_ldci_cfg.bits.isp_ldci_calc_en = uisp_ldci_calc_en;
    be_reg->isp_ldci_cfg.u32 = o_isp_ldci_cfg.u32;
}
/*  description   set the value of the member isp_ldci_cfg.isp_ldci_wrstat_en */
/*  input         unsigned int uisp_ldci_wrstat_en  1 bits */
static __inline td_void isp_ldci_wrstat_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_wrstat_en)
{
    u_isp_ldci_cfg o_isp_ldci_cfg;
    o_isp_ldci_cfg.u32 = be_reg->isp_ldci_cfg.u32;
    o_isp_ldci_cfg.bits.isp_ldci_wrstat_en = uisp_ldci_wrstat_en;
    be_reg->isp_ldci_cfg.u32 = o_isp_ldci_cfg.u32;
}
/*  description   set the value of the member isp_ldci_cfg.isp_ldci_rdstat_en */
/*  input         unsigned int uisp_ldci_rdstat_en  1 bits */
static __inline td_void isp_ldci_rdstat_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_rdstat_en)
{
    u_isp_ldci_cfg o_isp_ldci_cfg;
    o_isp_ldci_cfg.u32 = be_reg->isp_ldci_cfg.u32;
    o_isp_ldci_cfg.bits.isp_ldci_rdstat_en = uisp_ldci_rdstat_en;
    be_reg->isp_ldci_cfg.u32 = o_isp_ldci_cfg.u32;
}

static __inline td_void isp_ldci_lpf_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpf_en)
{
    u_isp_ldci_cfg o_isp_ldci_cfg;
    o_isp_ldci_cfg.u32 = be_reg->isp_ldci_cfg.u32;
    o_isp_ldci_cfg.bits.isp_ldci_lpf_en = uisp_ldci_lpf_en;
    be_reg->isp_ldci_cfg.u32 = o_isp_ldci_cfg.u32;
}
static __inline td_void isp_ldci_stat_offset_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_stat_offset)
{
    u_isp_ldci_stat_offset o_isp_ldci_stat_offset;
    o_isp_ldci_stat_offset.u32 = be_reg->isp_ldci_stat_offset.u32;
    o_isp_ldci_stat_offset.bits.isp_ldci_stat_offset = uisp_ldci_stat_offset;
    be_reg->isp_ldci_stat_offset.u32 = o_isp_ldci_stat_offset.u32;
}

/*  description   set the value of the member isp_ldci_calc_small_offset.isp_ldci_calc_map_offsetx */
/*  input         unsigned int uisp_ldci_calc_map_offsetx  20 bits */
static __inline td_void isp_ldci_calc_map_offsetx_write(isp_be_reg_type *be_reg, td_u32 ldci_calc_map_offsetx)
{
    u_isp_ldci_calc_small_offset o_isp_ldci_calc_small_offset;
    o_isp_ldci_calc_small_offset.u32 = be_reg->isp_ldci_calc_small_offset.u32;
    o_isp_ldci_calc_small_offset.bits.isp_ldci_calc_map_offsetx = ldci_calc_map_offsetx;
    be_reg->isp_ldci_calc_small_offset.u32 = o_isp_ldci_calc_small_offset.u32;
}
/*  description   set the value of the member isp_ldci_zone.isp_ldci_smlmapstride */
/*  input         unsigned int uisp_ldci_smlmapstride  6 bits */
static __inline td_void isp_ldci_smlmapstride_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_smlmapstride)
{
    u_isp_ldci_zone o_isp_ldci_zone;
    o_isp_ldci_zone.u32 = be_reg->isp_ldci_zone.u32;
    o_isp_ldci_zone.bits.isp_ldci_smlmapstride = uisp_ldci_smlmapstride;
    be_reg->isp_ldci_zone.u32 = o_isp_ldci_zone.u32;
}
/*  description   set the value of the member isp_ldci_zone.isp_ldci_smlmapheight */
/*  input         unsigned int uisp_ldci_smlmapheight  6 bits */
static __inline td_void isp_ldci_smlmapheight_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_smlmapheight)
{
    u_isp_ldci_zone o_isp_ldci_zone;
    o_isp_ldci_zone.u32 = be_reg->isp_ldci_zone.u32;
    o_isp_ldci_zone.bits.isp_ldci_smlmapheight = uisp_ldci_smlmapheight;
    be_reg->isp_ldci_zone.u32 = o_isp_ldci_zone.u32;
}
/*  description   set the value of the member isp_ldci_zone.isp_ldci_total_zone */
/*  input         unsigned int uisp_ldci_total_zone  16 bits */
static __inline td_void isp_ldci_total_zone_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_total_zone)
{
    u_isp_ldci_zone o_isp_ldci_zone;
    o_isp_ldci_zone.u32 = be_reg->isp_ldci_zone.u32;
    o_isp_ldci_zone.bits.isp_ldci_total_zone = uisp_ldci_total_zone;
    be_reg->isp_ldci_zone.u32 = o_isp_ldci_zone.u32;
}
/*  description   set the value of the member isp_ldci_lpfstt_bst.isp_ldci_lpfstt_size */
/*  input         unsigned int uisp_ldci_lpfstt_size  16 bits */
static __inline td_void isp_ldci_lpfstt_size_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfstt_size)
{
    u_isp_ldci_lpfstt_bst o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.u32 = be_reg->isp_ldci_lpfstt_bst.u32;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_size = uisp_ldci_lpfstt_size;
    be_reg->isp_ldci_lpfstt_bst.u32 = o_isp_ldci_lpfstt_bst.u32;
}
/*  description   set the value of the member isp_ldci_lpfstt_bst.isp_ldci_lpfstt_bst */
/*  input         unsigned int uisp_ldci_lpfstt_bst  4 bits */
static __inline td_void isp_ldci_lpfstt_bst_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfstt_bst)
{
    u_isp_ldci_lpfstt_bst o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.u32 = be_reg->isp_ldci_lpfstt_bst.u32;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_bst = uisp_ldci_lpfstt_bst;
    be_reg->isp_ldci_lpfstt_bst.u32 = o_isp_ldci_lpfstt_bst.u32;
}
/*  description   set the value of the member isp_ldci_lpfstt_bst.isp_ldci_lpfstt_en */
/*  input         unsigned int uisp_ldci_lpfstt_en  1 bits */
static __inline td_void isp_ldci_lpfstt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfstt_en)
{
    u_isp_ldci_lpfstt_bst o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.u32 = be_reg->isp_ldci_lpfstt_bst.u32;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_en = uisp_ldci_lpfstt_en;
    be_reg->isp_ldci_lpfstt_bst.u32 = o_isp_ldci_lpfstt_bst.u32;
}
/*  description   set the value of the member isp_ldci_lpfstt_abn.isp_ldci_lpfstt_clr */
/*  input         unsigned int uisp_ldci_lpfstt_clr  1 bits */
static __inline td_void isp_ldci_lpfstt_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfstt_clr)
{
    u_isp_ldci_lpfstt_abn o_isp_ldci_lpfstt_abn;
    o_isp_ldci_lpfstt_abn.u32 = be_reg->isp_ldci_lpfstt_abn.u32;
    o_isp_ldci_lpfstt_abn.bits.isp_ldci_lpfstt_clr = uisp_ldci_lpfstt_clr;
    be_reg->isp_ldci_lpfstt_abn.u32 = o_isp_ldci_lpfstt_abn.u32;
}
/*  description   set the value of the member isp_ldci_scale.isp_ldci_scalex */
/*  input         unsigned int uisp_ldci_scalex  15 bits */
static __inline td_void isp_ldci_scalex_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_scalex)
{
    u_isp_ldci_scale o_isp_ldci_scale;
    o_isp_ldci_scale.u32 = be_reg->isp_ldci_scale.u32;
    o_isp_ldci_scale.bits.isp_ldci_scalex = uisp_ldci_scalex;
    be_reg->isp_ldci_scale.u32 = o_isp_ldci_scale.u32;
}
/*  description   set the value of the member isp_ldci_scale.isp_ldci_scaley */
/*  input         unsigned int uisp_ldci_scaley  15 bits */
static __inline td_void isp_ldci_scaley_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_scaley)
{
    u_isp_ldci_scale o_isp_ldci_scale;
    o_isp_ldci_scale.u32 = be_reg->isp_ldci_scale.u32;
    o_isp_ldci_scale.bits.isp_ldci_scaley = uisp_ldci_scaley;
    be_reg->isp_ldci_scale.u32 = o_isp_ldci_scale.u32;
}
/*  description   set the value of the member isp_ldci_luma_sel.isp_ldci_luma_sel */
/*  input         unsigned int uisp_ldci_luma_sel  1 bits */
static __inline td_void isp_ldci_luma_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_luma_sel)
{
    u_isp_ldci_luma_sel o_isp_ldci_luma_sel;
    o_isp_ldci_luma_sel.u32 = be_reg->isp_ldci_luma_sel.u32;
    o_isp_ldci_luma_sel.bits.isp_ldci_luma_sel = uisp_ldci_luma_sel;
    be_reg->isp_ldci_luma_sel.u32 = o_isp_ldci_luma_sel.u32;
}
/*  description   set the value of the member isp_ldci_blc_ctrl.isp_ldci_blc_ctrl */
/*  input         unsigned int uisp_ldci_blc_ctrl  9 bits */
static __inline td_void isp_ldci_blc_ctrl_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_blc_ctrl)
{
    u_isp_ldci_blc_ctrl o_isp_ldci_blc_ctrl;
    o_isp_ldci_blc_ctrl.u32 = be_reg->isp_ldci_blc_ctrl.u32;
    o_isp_ldci_blc_ctrl.bits.isp_ldci_blc_ctrl = uisp_ldci_blc_ctrl;
    be_reg->isp_ldci_blc_ctrl.u32 = o_isp_ldci_blc_ctrl.u32;
}
/*  description   set the value of the member isp_ldci_lpf_lpfcoef0.isp_ldci_lpfcoef0 */
/*  input         unsigned int uisp_ldci_lpfcoef0  9 bits */
static __inline td_void isp_ldci_lpfcoef0_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfcoef0)
{
    u_isp_ldci_lpf_lpfcoef0 o_isp_ldci_lpf_lpfcoef0;
    o_isp_ldci_lpf_lpfcoef0.u32 = be_reg->isp_ldci_lpf_lpfcoef0.u32;
    o_isp_ldci_lpf_lpfcoef0.bits.isp_ldci_lpfcoef0 = uisp_ldci_lpfcoef0;
    be_reg->isp_ldci_lpf_lpfcoef0.u32 = o_isp_ldci_lpf_lpfcoef0.u32;
}
/*  description   set the value of the member isp_ldci_lpf_lpfcoef0.isp_ldci_lpfcoef1 */
/*  input         unsigned int uisp_ldci_lpfcoef1  9 bits */
static __inline td_void isp_ldci_lpfcoef1_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfcoef1)
{
    u_isp_ldci_lpf_lpfcoef0 o_isp_ldci_lpf_lpfcoef0;
    o_isp_ldci_lpf_lpfcoef0.u32 = be_reg->isp_ldci_lpf_lpfcoef0.u32;
    o_isp_ldci_lpf_lpfcoef0.bits.isp_ldci_lpfcoef1 = uisp_ldci_lpfcoef1;
    be_reg->isp_ldci_lpf_lpfcoef0.u32 = o_isp_ldci_lpf_lpfcoef0.u32;
}
/*  description   set the value of the member isp_ldci_lpf_lpfcoef1.isp_ldci_lpfcoef2 */
/*  input         unsigned int uisp_ldci_lpfcoef2  9 bits */
static __inline td_void isp_ldci_lpfcoef2_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfcoef2)
{
    u_isp_ldci_lpf_lpfcoef1 o_isp_ldci_lpf_lpfcoef1;
    o_isp_ldci_lpf_lpfcoef1.u32 = be_reg->isp_ldci_lpf_lpfcoef1.u32;
    o_isp_ldci_lpf_lpfcoef1.bits.isp_ldci_lpfcoef2 = uisp_ldci_lpfcoef2;
    be_reg->isp_ldci_lpf_lpfcoef1.u32 = o_isp_ldci_lpf_lpfcoef1.u32;
}
/*  description   set the value of the member isp_ldci_lpf_lpfcoef1.isp_ldci_lpfcoef3 */
/*  input         unsigned int uisp_ldci_lpfcoef3  9 bits */
static __inline td_void isp_ldci_lpfcoef3_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfcoef3)
{
    u_isp_ldci_lpf_lpfcoef1 o_isp_ldci_lpf_lpfcoef1;
    o_isp_ldci_lpf_lpfcoef1.u32 = be_reg->isp_ldci_lpf_lpfcoef1.u32;
    o_isp_ldci_lpf_lpfcoef1.bits.isp_ldci_lpfcoef3 = uisp_ldci_lpfcoef3;
    be_reg->isp_ldci_lpf_lpfcoef1.u32 = o_isp_ldci_lpf_lpfcoef1.u32;
}
/*  description   set the value of the member isp_ldci_lpf_lpfcoef2.isp_ldci_lpfcoef4 */
/*  input         unsigned int uisp_ldci_lpfcoef4  9 bits */
static __inline td_void isp_ldci_lpfcoef4_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfcoef4)
{
    u_isp_ldci_lpf_lpfcoef2 o_isp_ldci_lpf_lpfcoef2;
    o_isp_ldci_lpf_lpfcoef2.u32 = be_reg->isp_ldci_lpf_lpfcoef2.u32;
    o_isp_ldci_lpf_lpfcoef2.bits.isp_ldci_lpfcoef4 = uisp_ldci_lpfcoef4;
    be_reg->isp_ldci_lpf_lpfcoef2.u32 = o_isp_ldci_lpf_lpfcoef2.u32;
}
/*  description   set the value of the member isp_ldci_lpf_lpfsft.isp_ldci_lpfsft */
/*  input         unsigned int uisp_ldci_lpfsft  4 bits */
static __inline td_void isp_ldci_lpfsft_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfsft)
{
    u_isp_ldci_lpf_lpfsft o_isp_ldci_lpf_lpfsft;
    o_isp_ldci_lpf_lpfsft.u32 = be_reg->isp_ldci_lpf_lpfsft.u32;
    o_isp_ldci_lpf_lpfsft.bits.isp_ldci_lpfsft = uisp_ldci_lpfsft;
    be_reg->isp_ldci_lpf_lpfsft.u32 = o_isp_ldci_lpf_lpfsft.u32;
}
/*  description   set the value of the member isp_ldci_he_waddr.isp_ldci_he_waddr */
/*  input         unsigned int uisp_ldci_he_waddr  32 bits */
static __inline td_void isp_ldci_he_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_he_waddr)
{
    be_reg->isp_ldci_he_waddr.u32 = uisp_ldci_he_waddr;
}
static __inline td_void isp_ldci_he_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_hepos_wdata,
                                                td_u32 uisp_ldci_heneg_wdata)
{
    u_isp_ldci_he_wdata o_isp_ldci_he_wdata;
    o_isp_ldci_he_wdata.u32 = be_reg->isp_ldci_he_wdata.u32;
    o_isp_ldci_he_wdata.bits.isp_ldci_hepos_wdata = uisp_ldci_hepos_wdata;
    o_isp_ldci_he_wdata.bits.isp_ldci_heneg_wdata = uisp_ldci_heneg_wdata;
    be_reg->isp_ldci_he_wdata.u32 = o_isp_ldci_he_wdata.u32;
}
/*  description   set the value of the member isp_ldci_he_raddr.isp_ldci_he_raddr */
/*  input         unsigned int uisp_ldci_he_raddr  32 bits */
static __inline td_void isp_ldci_he_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_he_raddr)
{
    be_reg->isp_ldci_he_raddr.u32 = uisp_ldci_he_raddr;
}

/*  description   set the value of the member isp_ldci_he_rdata.isp_ldci_hepos_rdata */
/*  input         unsigned int uisp_ldci_hepos_rdata  9 bits */
static __inline td_void isp_ldci_hepos_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_hepos_rdata)
{
    u_isp_ldci_he_rdata o_isp_ldci_he_rdata;
    o_isp_ldci_he_rdata.u32 = be_reg->isp_ldci_he_rdata.u32;
    o_isp_ldci_he_rdata.bits.isp_ldci_hepos_rdata = uisp_ldci_hepos_rdata;
    be_reg->isp_ldci_he_rdata.u32 = o_isp_ldci_he_rdata.u32;
}
/*  description   set the value of the member isp_ldci_he_rdata.isp_ldci_heneg_rdata */
/*  input         unsigned int uisp_ldci_heneg_rdata  9 bits */
static __inline td_void isp_ldci_heneg_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_heneg_rdata)
{
    u_isp_ldci_he_rdata o_isp_ldci_he_rdata;
    o_isp_ldci_he_rdata.u32 = be_reg->isp_ldci_he_rdata.u32;
    o_isp_ldci_he_rdata.bits.isp_ldci_heneg_rdata = uisp_ldci_heneg_rdata;
    be_reg->isp_ldci_he_rdata.u32 = o_isp_ldci_he_rdata.u32;
}

/*  description   set the value of the member isp_ldci_cgain_waddr.isp_ldci_cgain_waddr */
/*  input         unsigned int uisp_ldci_cgain_waddr  32 bits */
static __inline td_void isp_ldci_cgain_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_cgain_waddr)
{
    be_reg->isp_ldci_cgain_waddr.u32 = uisp_ldci_cgain_waddr;
}
/*  description   set the value of the member isp_ldci_cgain_wdata.isp_ldci_cgain_wdata */
/*  input         unsigned int uisp_ldci_cgain_wdata  12 bits */
static __inline td_void isp_ldci_cgain_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_cgain_wdata)
{
    u_isp_ldci_cgain_wdata o_isp_ldci_cgain_wdata;
    o_isp_ldci_cgain_wdata.u32 = be_reg->isp_ldci_cgain_wdata.u32;
    o_isp_ldci_cgain_wdata.bits.isp_ldci_cgain_wdata = uisp_ldci_cgain_wdata;
    be_reg->isp_ldci_cgain_wdata.u32 = o_isp_ldci_cgain_wdata.u32;
}
/*  description   set the value of the member isp_ldci_cgain_raddr.isp_ldci_cgain_raddr */
/*  input         unsigned int uisp_ldci_cgain_raddr  32 bits */
static __inline td_void isp_ldci_cgain_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_cgain_raddr)
{
    be_reg->isp_ldci_cgain_raddr.u32 = uisp_ldci_cgain_raddr;
}

/*  description   set the value of the member isp_ldci_cgain_rdata.isp_ldci_cgain_rdata */
/*  input         unsigned int uisp_ldci_cgain_rdata  12 bits */
static __inline td_void isp_ldci_cgain_rdata_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_cgain_rdata)
{
    u_isp_ldci_cgain_rdata o_isp_ldci_cgain_rdata;
    o_isp_ldci_cgain_rdata.u32 = be_reg->isp_ldci_cgain_rdata.u32;
    o_isp_ldci_cgain_rdata.bits.isp_ldci_cgain_rdata = uisp_ldci_cgain_rdata;
    be_reg->isp_ldci_cgain_rdata.u32 = o_isp_ldci_cgain_rdata.u32;
}
/*  description   set the value of the member isp_ldci_stat_hpos.isp_ldci_hstart */
/*  input         unsigned int uisp_ldci_hstart  13 bits */
static __inline td_void isp_ldci_hstart_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_hstart)
{
    u_isp_ldci_stat_hpos o_isp_ldci_stat_hpos;
    o_isp_ldci_stat_hpos.u32 = be_reg->isp_ldci_stat_hpos.u32;
    o_isp_ldci_stat_hpos.bits.isp_ldci_hstart = uisp_ldci_hstart;
    be_reg->isp_ldci_stat_hpos.u32 = o_isp_ldci_stat_hpos.u32;
}
/*  description   set the value of the member isp_ldci_stat_hpos.isp_ldci_hend */
/*  input         unsigned int uisp_ldci_hend  13 bits */
static __inline td_void isp_ldci_hend_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_hend)
{
    u_isp_ldci_stat_hpos o_isp_ldci_stat_hpos;
    o_isp_ldci_stat_hpos.u32 = be_reg->isp_ldci_stat_hpos.u32;
    o_isp_ldci_stat_hpos.bits.isp_ldci_hend = uisp_ldci_hend;
    be_reg->isp_ldci_stat_hpos.u32 = o_isp_ldci_stat_hpos.u32;
}
/*  description   set the value of the member isp_ldci_stat_vpos.isp_ldci_vstart */
/*  input         unsigned int uisp_ldci_vstart  13 bits */
static __inline td_void isp_ldci_vstart_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_vstart)
{
    u_isp_ldci_stat_vpos o_isp_ldci_stat_vpos;
    o_isp_ldci_stat_vpos.u32 = be_reg->isp_ldci_stat_vpos.u32;
    o_isp_ldci_stat_vpos.bits.isp_ldci_vstart = uisp_ldci_vstart;
    be_reg->isp_ldci_stat_vpos.u32 = o_isp_ldci_stat_vpos.u32;
}
/*  description   set the value of the member isp_ldci_stat_vpos.isp_ldci_vend */
/*  input         unsigned int uisp_ldci_vend  13 bits */
static __inline td_void isp_ldci_vend_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_vend)
{
    u_isp_ldci_stat_vpos o_isp_ldci_stat_vpos;
    o_isp_ldci_stat_vpos.u32 = be_reg->isp_ldci_stat_vpos.u32;
    o_isp_ldci_stat_vpos.bits.isp_ldci_vend = uisp_ldci_vend;
    be_reg->isp_ldci_stat_vpos.u32 = o_isp_ldci_stat_vpos.u32;
}
/*  description   set the value of the member isp_ldci_stat_evratio.isp_ldci_stat_evratio */
/*  input         unsigned int uisp_ldci_stat_evratio  16 bits */
static __inline td_void isp_ldci_stat_evratio_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_stat_evratio)
{
    u_isp_ldci_stat_evratio o_isp_ldci_stat_evratio;
    o_isp_ldci_stat_evratio.u32 = be_reg->isp_ldci_stat_evratio.u32;
    o_isp_ldci_stat_evratio.bits.isp_ldci_stat_evratio = uisp_ldci_stat_evratio;
    be_reg->isp_ldci_stat_evratio.u32 = o_isp_ldci_stat_evratio.u32;
}
/*  description   set the value of the member isp_ldci_stat_zone.isp_ldci_stat_smlmapwidth */
/*  input         unsigned int uisp_ldci_stat_smlmapwidth  6 bits */
static __inline td_void isp_ldci_stat_smlmapwidth_write(isp_be_reg_type *be_reg, td_u32 ldci_stat_smlmapwidth)
{
    u_isp_ldci_stat_zone o_isp_ldci_stat_zone;
    o_isp_ldci_stat_zone.u32 = be_reg->isp_ldci_stat_zone.u32;
    o_isp_ldci_stat_zone.bits.isp_ldci_stat_smlmapwidth = ldci_stat_smlmapwidth;
    be_reg->isp_ldci_stat_zone.u32 = o_isp_ldci_stat_zone.u32;
}
/*  description   set the value of the member isp_ldci_stat_zone.isp_ldci_stat_smlmapheight */
/*  input         unsigned int uisp_ldci_stat_smlmapheight  6 bits */
static __inline td_void isp_ldci_stat_smlmapheight_write(isp_be_reg_type *be_reg, td_u32 ldci_stat_smlmapheight)
{
    u_isp_ldci_stat_zone o_isp_ldci_stat_zone;
    o_isp_ldci_stat_zone.u32 = be_reg->isp_ldci_stat_zone.u32;
    o_isp_ldci_stat_zone.bits.isp_ldci_stat_smlmapheight = ldci_stat_smlmapheight;
    be_reg->isp_ldci_stat_zone.u32 = o_isp_ldci_stat_zone.u32;
}
/*  description   set the value of the member isp_ldci_stat_zone.isp_ldci_stat_total_zone */
/*  input         unsigned int uisp_ldci_stat_total_zone  16 bits */
static __inline td_void isp_ldci_stat_total_zone_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_stat_total_zone)
{
    u_isp_ldci_stat_zone o_isp_ldci_stat_zone;
    o_isp_ldci_stat_zone.u32 = be_reg->isp_ldci_stat_zone.u32;
    o_isp_ldci_stat_zone.bits.isp_ldci_stat_total_zone = uisp_ldci_stat_total_zone;
    be_reg->isp_ldci_stat_zone.u32 = o_isp_ldci_stat_zone.u32;
}
/*  description   set the value of the member isp_ldci_blk_smlmapwidth0.isp_ldci_blk_smlmapwidth0 */
/*  input         unsigned int uisp_ldci_blk_smlmapwidth0  6 bits */
static __inline td_void isp_ldci_blk_smlmapwidth0_write(isp_be_reg_type *be_reg, td_u32 ldci_blk_smlmapwidth0)
{
    u_isp_ldci_blk_smlmapwidth0 o_isp_ldci_blk_smlmapwidth0;
    o_isp_ldci_blk_smlmapwidth0.u32 = be_reg->isp_ldci_blk_smlmapwidth0.u32;
    o_isp_ldci_blk_smlmapwidth0.bits.isp_ldci_blk_smlmapwidth0 = ldci_blk_smlmapwidth0;
    be_reg->isp_ldci_blk_smlmapwidth0.u32 = o_isp_ldci_blk_smlmapwidth0.u32;
}
/*  description   set the value of the member isp_ldci_blk_smlmapwidth1.isp_ldci_blk_smlmapwidth1 */
/*  input         unsigned int uisp_ldci_blk_smlmapwidth1  6 bits */
static __inline td_void isp_ldci_blk_smlmapwidth1_write(isp_be_reg_type *be_reg, td_u32 ldci_blk_smlmapwidth1)
{
    u_isp_ldci_blk_smlmapwidth1 o_isp_ldci_blk_smlmapwidth1;
    o_isp_ldci_blk_smlmapwidth1.u32 = be_reg->isp_ldci_blk_smlmapwidth1.u32;
    o_isp_ldci_blk_smlmapwidth1.bits.isp_ldci_blk_smlmapwidth1 = ldci_blk_smlmapwidth1;
    be_reg->isp_ldci_blk_smlmapwidth1.u32 = o_isp_ldci_blk_smlmapwidth1.u32;
}
/*  description   set the value of the member isp_ldci_blk_smlmapwidth2.isp_ldci_blk_smlmapwidth2 */
/*  input         unsigned int uisp_ldci_blk_smlmapwidth2  6 bits */
static __inline td_void isp_ldci_blk_smlmapwidth2_write(isp_be_reg_type *be_reg, td_u32 ldci_blk_smlmapwidth2)
{
    u_isp_ldci_blk_smlmapwidth2 o_isp_ldci_blk_smlmapwidth2;
    o_isp_ldci_blk_smlmapwidth2.u32 = be_reg->isp_ldci_blk_smlmapwidth2.u32;
    o_isp_ldci_blk_smlmapwidth2.bits.isp_ldci_blk_smlmapwidth2 = ldci_blk_smlmapwidth2;
    be_reg->isp_ldci_blk_smlmapwidth2.u32 = o_isp_ldci_blk_smlmapwidth2.u32;
}
/*  description   set the value of the member isp_ldci_chrdamp.isp_ldci_chrposdamp */
/*  input         unsigned int uisp_ldci_chrposdamp  8 bits */
static __inline td_void isp_ldci_chrposdamp_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_chrposdamp)
{
    u_isp_ldci_chrdamp o_isp_ldci_chrdamp;
    o_isp_ldci_chrdamp.u32 = be_reg->isp_ldci_chrdamp.u32;
    o_isp_ldci_chrdamp.bits.isp_ldci_chrposdamp = uisp_ldci_chrposdamp;
    be_reg->isp_ldci_chrdamp.u32 = o_isp_ldci_chrdamp.u32;
}
/*  description   set the value of the member isp_ldci_chrdamp.isp_ldci_chrnegdamp */
/*  input         unsigned int uisp_ldci_chrnegdamp  8 bits */
static __inline td_void isp_ldci_chrnegdamp_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_chrnegdamp)
{
    u_isp_ldci_chrdamp o_isp_ldci_chrdamp;
    o_isp_ldci_chrdamp.u32 = be_reg->isp_ldci_chrdamp.u32;
    o_isp_ldci_chrdamp.bits.isp_ldci_chrnegdamp = uisp_ldci_chrnegdamp;
    be_reg->isp_ldci_chrdamp.u32 = o_isp_ldci_chrdamp.u32;
}
/*  description   set the value of the member isp_ldci_stt2lut_cfg.isp_ldci_stt2lut_en */
/*  input         unsigned int uisp_ldci_stt2lut_en  1 bits */
static __inline td_void isp_ldci_stt2lut_en_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_stt2lut_en)
{
    u_isp_ldci_stt2lut_cfg o_isp_ldci_stt2lut_cfg;
    o_isp_ldci_stt2lut_cfg.u32 = be_reg->isp_ldci_stt2lut_cfg.u32;
    o_isp_ldci_stt2lut_cfg.bits.isp_ldci_stt2lut_en = uisp_ldci_stt2lut_en;
    be_reg->isp_ldci_stt2lut_cfg.u32 = o_isp_ldci_stt2lut_cfg.u32;
}
/*  description   set the value of the member isp_ldci_stt2lut_regnew.isp_ldci_stt2lut_regnew */
/*  input         unsigned int uisp_ldci_stt2lut_regnew  1 bits */
static __inline td_void isp_ldci_stt2lut_regnew_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_stt2lut_regnew)
{
    u_isp_ldci_stt2lut_regnew o_isp_ldci_stt2lut_regnew;
    o_isp_ldci_stt2lut_regnew.u32 = be_reg->isp_ldci_stt2lut_regnew.u32;
    o_isp_ldci_stt2lut_regnew.bits.isp_ldci_stt2lut_regnew = uisp_ldci_stt2lut_regnew;
    be_reg->isp_ldci_stt2lut_regnew.u32 = o_isp_ldci_stt2lut_regnew.u32;
}
/*  description   set the value of the member isp_ldci_stt2lut_abn.isp_ldci_stt2lut_clr */
/*  input         unsigned int uisp_ldci_stt2lut_clr  1 bits */
static __inline td_void isp_ldci_stt2lut_clr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_stt2lut_clr)
{
    u_isp_ldci_stt2lut_abn o_isp_ldci_stt2lut_abn;
    o_isp_ldci_stt2lut_abn.u32 = be_reg->isp_ldci_stt2lut_abn.u32;
    o_isp_ldci_stt2lut_abn.bits.isp_ldci_stt2lut_clr = uisp_ldci_stt2lut_clr;
    be_reg->isp_ldci_stt2lut_abn.u32 = o_isp_ldci_stt2lut_abn.u32;
}
/*  description   set the value of the member isp_ldci_lpf_map_raddr.isp_ldci_lpfmap_raddr */
/*  input         unsigned int uisp_ldci_lpfmap_raddr  32 bits */
static __inline td_void isp_ldci_lpfmap_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_ldci_lpfmap_raddr)
{
    be_reg->isp_ldci_lpf_map_raddr.u32 = uisp_ldci_lpfmap_raddr;
}


/*  description : set the value of the member isp_drc_cfg.isp_drc_wrstat_en */
/*  input       : unsigned int uisp_drc_wrstat_en: 1 bits */
static __inline td_void isp_drc_wrstat_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_wrstat_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_wrstat_en = uisp_drc_wrstat_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_rdstat_en */
/*  input       : unsigned int uisp_drc_rdstat_en: 1 bits */
static __inline td_void isp_drc_rdstat_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_rdstat_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_rdstat_en = uisp_drc_rdstat_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_vbiflt_en */
/*  input       : unsigned int uisp_drc_vbiflt_en: 1 bits */
static __inline td_void isp_drc_vbiflt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_vbiflt_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_vbiflt_en = uisp_drc_vbiflt_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_color_corr_en */
/*  input       : unsigned int uisp_drc_color_corr_en: 1 bits */
static __inline td_void isp_drc_color_corr_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_color_corr_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_color_corr_en = uisp_drc_color_corr_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_mono_chroma_en */
/*  input       : unsigned int uisp_drc_mono_chroma_en: 1 bits */
static __inline td_void isp_drc_mono_chroma_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_mono_chroma_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_mono_chroma_en = uisp_drc_mono_chroma_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}
/*  description : set the value of the member isp_drc_cfg.isp_drc_flt_en */
/*  input       : unsigned int uisp_drc_flt_en: 1 bits */
static __inline td_void isp_drc_flt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_flt_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_flt_en = uisp_drc_flt_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_pregamma_en */
/*  input       : unsigned int uisp_drc_pregamma_en: 1 bits */

/*  description : set the value of the member isp_drc_cfg.isp_drc_dp_det_en */
/*  input       : unsigned int uisp_drc_dp_det_en: 1 bits */
static __inline td_void isp_drc_dp_det_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_dp_det_en = uisp_drc_dp_det_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_inadpt_en */
/*  input       : unsigned int uisp_drc_inadpt_en: 1 bits */
static __inline td_void isp_drc_inadpt_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_inadpt_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_inadpt_en = uisp_drc_inadpt_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_line_detect_en */
/*  input       : unsigned int uisp_drc_line_detect_en: 1 bits */
static __inline td_void isp_drc_line_detect_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_line_detect_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_line_detect_en = uisp_drc_line_detect_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_nr_en */
/*  input       : unsigned int uisp_drc_nr_en: 1 bits */
static __inline td_void isp_drc_nr_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_nr_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_nr_en = uisp_drc_nr_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}
/*  description : set the value of the member isp_drc_cfg.isp_drc_inner_bypass_en */
/*  input       : unsigned int uisp_drc_inner_bypass_en: 1 bits */
static __inline td_void isp_drc_inner_bypass_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_inner_bypass_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_inner_bypass_en = uisp_drc_inner_bypass_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_cfg.isp_drc_strp_en */
/*  input       : unsigned int uisp_drc_strp_en: 1 bits */
static __inline td_void isp_drc_strp_en_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_strp_en)
{
    u_isp_drc_cfg o_isp_drc_cfg;
    o_isp_drc_cfg.u32 = be_reg->isp_drc_cfg.u32;
    o_isp_drc_cfg.bits.isp_drc_strp_en = uisp_drc_strp_en;
    be_reg->isp_drc_cfg.u32 = o_isp_drc_cfg.u32;
}

/*  description : set the value of the member isp_drc_lut_update0.isp_drc_lut_update0 */
/*  input       : unsigned int uisp_drc_lut_update0: 1 bits */
static __inline td_void isp_drc_lut_update0_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_lut_update0)
{
    u_isp_drc_lut_update0 o_isp_drc_lut_update0;
    o_isp_drc_lut_update0.u32 = be_reg->isp_drc_lut_update0.u32;
    o_isp_drc_lut_update0.bits.isp_drc_lut_update0 = uisp_drc_lut_update0;
    be_reg->isp_drc_lut_update0.u32 = o_isp_drc_lut_update0.u32;
}

/*  description : set the value of the member isp_drc_lut_update0.isp_drc_lut_update1 */
/*  input       : unsigned int uisp_drc_lut_update1: 1 bits */
static __inline td_void isp_drc_lut_update1_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_lut_update1)
{
    u_isp_drc_lut_update0 o_isp_drc_lut_update0;
    o_isp_drc_lut_update0.u32 = be_reg->isp_drc_lut_update0.u32;
    o_isp_drc_lut_update0.bits.isp_drc_lut_update1 = uisp_drc_lut_update1;
    be_reg->isp_drc_lut_update0.u32 = o_isp_drc_lut_update0.u32;
}

/*  description : set the value of the member isp_drc_lut_update0.isp_drc_lut_update2 */
/*  input       : unsigned int uisp_drc_lut_update2: 1 bits */
static __inline td_void isp_drc_lut_update2_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_lut_update2)
{
    u_isp_drc_lut_update0 o_isp_drc_lut_update0;
    o_isp_drc_lut_update0.u32 = be_reg->isp_drc_lut_update0.u32;
    o_isp_drc_lut_update0.bits.isp_drc_lut_update2 = uisp_drc_lut_update2;
    be_reg->isp_drc_lut_update0.u32 = o_isp_drc_lut_update0.u32;
}

/*  description : set the value of the member isp_drc_zone.isp_drc_hnum */
/*  input       : unsigned int uisp_drc_hnum: 6 bits */
static __inline td_void isp_drc_hnum_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_hnum)
{
    u_isp_drc_zone o_isp_drc_zone;
    o_isp_drc_zone.u32 = be_reg->isp_drc_zone.u32;
    o_isp_drc_zone.bits.isp_drc_hnum = uisp_drc_hnum;
    be_reg->isp_drc_zone.u32 = o_isp_drc_zone.u32;
}

/*  description : set the value of the member isp_drc_zone.isp_drc_vnum */
/*  input       : unsigned int uisp_drc_vnum: 6 bits */
static __inline td_void isp_drc_vnum_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_vnum)
{
    u_isp_drc_zone o_isp_drc_zone;
    o_isp_drc_zone.u32 = be_reg->isp_drc_zone.u32;
    o_isp_drc_zone.bits.isp_drc_vnum = uisp_drc_vnum;
    be_reg->isp_drc_zone.u32 = o_isp_drc_zone.u32;
}

/*  description : set the value of the member isp_drc_zone_size.isp_drc_zone_hsize */
/*  input       : unsigned int uisp_drc_zone_hsize: 9 bits */
static __inline td_void isp_drc_zone_hsize_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_zone_hsize)
{
    u_isp_drc_zone_size o_isp_drc_zone_size;
    o_isp_drc_zone_size.u32 = be_reg->isp_drc_zone_size.u32;
    o_isp_drc_zone_size.bits.isp_drc_zone_hsize = uisp_drc_zone_hsize;
    be_reg->isp_drc_zone_size.u32 = o_isp_drc_zone_size.u32;
}

/*  description : set the value of the member isp_drc_zone_size.isp_drc_chk_x */
/*  input       : unsigned int uisp_drc_chk_x: 6 bits */
static __inline td_void isp_drc_chk_x_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_chk_x)
{
    u_isp_drc_zone_size o_isp_drc_zone_size;
    o_isp_drc_zone_size.u32 = be_reg->isp_drc_zone_size.u32;
    o_isp_drc_zone_size.bits.isp_drc_chk_x = uisp_drc_chk_x;
    be_reg->isp_drc_zone_size.u32 = o_isp_drc_zone_size.u32;
}

/*  description : set the value of the member isp_drc_zone_size.isp_drc_zone_vsize */
/*  input       : unsigned int uisp_drc_zone_vsize: 9 bits */
static __inline td_void isp_drc_zone_vsize_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_zone_vsize)
{
    u_isp_drc_zone_size o_isp_drc_zone_size;
    o_isp_drc_zone_size.u32 = be_reg->isp_drc_zone_size.u32;
    o_isp_drc_zone_size.bits.isp_drc_zone_vsize = uisp_drc_zone_vsize;
    be_reg->isp_drc_zone_size.u32 = o_isp_drc_zone_size.u32;
}

/*  description : set the value of the member isp_drc_zone_size.isp_drc_chk_y */
/*  input       : unsigned int uisp_drc_chk_y: 6 bits */
static __inline td_void isp_drc_chk_y_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_chk_y)
{
    u_isp_drc_zone_size o_isp_drc_zone_size;
    o_isp_drc_zone_size.u32 = be_reg->isp_drc_zone_size.u32;
    o_isp_drc_zone_size.bits.isp_drc_chk_y = uisp_drc_chk_y;
    be_reg->isp_drc_zone_size.u32 = o_isp_drc_zone_size.u32;
}

/*  description : set the value of the member isp_drc_zone_div0.isp_drc_div_x0 */
/*  input       : unsigned int uisp_drc_div_x0: 15 bits */
static __inline td_void isp_drc_div_x0_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_div_x0)
{
    u_isp_drc_zone_div0 o_isp_drc_zone_div0;
    o_isp_drc_zone_div0.u32 = be_reg->isp_drc_zone_div0.u32;
    o_isp_drc_zone_div0.bits.isp_drc_div_x0 = uisp_drc_div_x0;
    be_reg->isp_drc_zone_div0.u32 = o_isp_drc_zone_div0.u32;
}

/*  description : set the value of the member isp_drc_zone_div0.isp_drc_div_y0 */
/*  input       : unsigned int uisp_drc_div_y0: 15 bits */
static __inline td_void isp_drc_div_y0_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_div_y0)
{
    u_isp_drc_zone_div0 o_isp_drc_zone_div0;
    o_isp_drc_zone_div0.u32 = be_reg->isp_drc_zone_div0.u32;
    o_isp_drc_zone_div0.bits.isp_drc_div_y0 = uisp_drc_div_y0;
    be_reg->isp_drc_zone_div0.u32 = o_isp_drc_zone_div0.u32;
}

/*  description : set the value of the member isp_drc_zone_div1.isp_drc_div_x1 */
/*  input       : unsigned int uisp_drc_div_x1: 15 bits */
static __inline td_void isp_drc_div_x1_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_div_x1)
{
    u_isp_drc_zone_div1 o_isp_drc_zone_div1;
    o_isp_drc_zone_div1.u32 = be_reg->isp_drc_zone_div1.u32;
    o_isp_drc_zone_div1.bits.isp_drc_div_x1 = uisp_drc_div_x1;
    be_reg->isp_drc_zone_div1.u32 = o_isp_drc_zone_div1.u32;
}

/*  description : set the value of the member isp_drc_zone_div1.isp_drc_div_y1 */
/*  input       : unsigned int uisp_drc_div_y1: 15 bits */
static __inline td_void isp_drc_div_y1_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_div_y1)
{
    u_isp_drc_zone_div1 o_isp_drc_zone_div1;
    o_isp_drc_zone_div1.u32 = be_reg->isp_drc_zone_div1.u32;
    o_isp_drc_zone_div1.bits.isp_drc_div_y1 = uisp_drc_div_y1;
    be_reg->isp_drc_zone_div1.u32 = o_isp_drc_zone_div1.u32;
}

/*  description : set the value of the member isp_drc_bin.isp_drc_bin */
/*  input       : unsigned int uisp_drc_bin: 4 bits */
static __inline td_void isp_drc_bin_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_bin)
{
    u_isp_drc_bin o_isp_drc_bin;
    o_isp_drc_bin.u32 = be_reg->isp_drc_bin.u32;
    o_isp_drc_bin.bits.isp_drc_bin = uisp_drc_bin;
    be_reg->isp_drc_bin.u32 = o_isp_drc_bin.u32;
}

/*  description : set the value of the member isp_drc_zone_init.isp_drc_acc_x_init */
/*  input       : unsigned int uisp_drc_acc_x_init: 16 bits */
static __inline td_void isp_drc_acc_x_init_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_acc_x_init)
{
    u_isp_drc_zone_init o_isp_drc_zone_init;
    o_isp_drc_zone_init.u32 = be_reg->isp_drc_zone_init.u32;
    o_isp_drc_zone_init.bits.isp_drc_acc_x_init = uisp_drc_acc_x_init;
    be_reg->isp_drc_zone_init.u32 = o_isp_drc_zone_init.u32;
}

/*  description : set the value of the member isp_drc_zone_init.isp_drc_cnt_x_init */
/*  input       : unsigned int uisp_drc_cnt_x_init: 9 bits */
static __inline td_void isp_drc_cnt_x_init_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_cnt_x_init)
{
    u_isp_drc_zone_init o_isp_drc_zone_init;
    o_isp_drc_zone_init.u32 = be_reg->isp_drc_zone_init.u32;
    o_isp_drc_zone_init.bits.isp_drc_cnt_x_init = uisp_drc_cnt_x_init;
    be_reg->isp_drc_zone_init.u32 = o_isp_drc_zone_init.u32;
}

/*  description : set the value of the member isp_drc_zone_init.isp_drc_idx_x_init */
/*  input       : unsigned int uisp_drc_idx_x_init: 6 bits */
static __inline td_void isp_drc_idx_x_init_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idx_x_init)
{
    u_isp_drc_zone_init o_isp_drc_zone_init;
    o_isp_drc_zone_init.u32 = be_reg->isp_drc_zone_init.u32;
    o_isp_drc_zone_init.bits.isp_drc_idx_x_init = uisp_drc_idx_x_init;
    be_reg->isp_drc_zone_init.u32 = o_isp_drc_zone_init.u32;
}

/*  description : set the value of the member isp_drc_zone_init.isp_drc_big_x_init */
/*  input       : unsigned int uisp_drc_big_x_init: 1 bits */
static __inline td_void isp_drc_big_x_init_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_big_x_init)
{
    u_isp_drc_zone_init o_isp_drc_zone_init;
    o_isp_drc_zone_init.u32 = be_reg->isp_drc_zone_init.u32;
    o_isp_drc_zone_init.bits.isp_drc_big_x_init = uisp_drc_big_x_init;
    be_reg->isp_drc_zone_init.u32 = o_isp_drc_zone_init.u32;
}

/*  description : set the value of the member isp_drc_bin_scale.isp_drc_bin_scale */
/*  input       : unsigned int uisp_drc_bin_scale: 3 bits */
static __inline td_void isp_drc_bin_scale_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_bin_scale)
{
    u_isp_drc_bin_scale o_isp_drc_bin_scale;
    o_isp_drc_bin_scale.u32 = be_reg->isp_drc_bin_scale.u32;
    o_isp_drc_bin_scale.bits.isp_drc_bin_scale = uisp_drc_bin_scale;
    be_reg->isp_drc_bin_scale.u32 = o_isp_drc_bin_scale.u32;
}

/*  description : set the value of the member isp_drc_gain_cfg.isp_drc_colorgain_thr */
/*  input       : unsigned int uisp_drc_colorgain_thr: 8 bits */
static __inline td_void isp_drc_colorgain_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_colorgain_thr)
{
    u_isp_drc_gain_cfg o_isp_drc_gain_cfg;
    o_isp_drc_gain_cfg.u32 = be_reg->isp_drc_gain_cfg.u32;
    o_isp_drc_gain_cfg.bits.isp_drc_colorgain_thr = uisp_drc_colorgain_thr;
    be_reg->isp_drc_gain_cfg.u32 = o_isp_drc_gain_cfg.u32;
}

/*  description : set the value of the member isp_drc_gain_cfg.isp_drc_colorgain_slope */
/*  input       : unsigned int uisp_drc_colorgain_slope: 8 bits */
static __inline td_void isp_drc_colorgain_slope_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_colorgain_slope)
{
    u_isp_drc_gain_cfg o_isp_drc_gain_cfg;
    o_isp_drc_gain_cfg.u32 = be_reg->isp_drc_gain_cfg.u32;
    o_isp_drc_gain_cfg.bits.isp_drc_colorgain_slope = uisp_drc_colorgain_slope;
    be_reg->isp_drc_gain_cfg.u32 = o_isp_drc_gain_cfg.u32;
}

/*  description : set the value of the member isp_drc_gain_cfg.isp_drc_detailgain_slope */
/*  input       : unsigned int uisp_drc_detailgain_slope: 8 bits */
static __inline td_void isp_drc_detailgain_slope_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_detailgain_slope)
{
    u_isp_drc_gain_cfg o_isp_drc_gain_cfg;
    o_isp_drc_gain_cfg.u32 = be_reg->isp_drc_gain_cfg.u32;
    o_isp_drc_gain_cfg.bits.isp_drc_detailgain_slope = uisp_drc_detailgain_slope;
    be_reg->isp_drc_gain_cfg.u32 = o_isp_drc_gain_cfg.u32;
}

/*  description : set the value of the member isp_drc_strength.isp_drc_strength */
/*  input       : unsigned int uisp_drc_strength: 10 bits */
static __inline td_void isp_drc_strength_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_strength)
{
    u_isp_drc_strength o_isp_drc_strength;
    o_isp_drc_strength.u32 = be_reg->isp_drc_strength.u32;
    o_isp_drc_strength.bits.isp_drc_strength = uisp_drc_strength;
    be_reg->isp_drc_strength.u32 = o_isp_drc_strength.u32;
}

/*  description : set the value of the member isp_drc_stat_width.isp_drc_stat_width */
/*  input       : unsigned int uisp_drc_stat_width: 16 bits */
static __inline td_void isp_drc_stat_width_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_stat_width)
{
    u_isp_drc_stat_width o_isp_drc_stat_width;
    o_isp_drc_stat_width.u32 = be_reg->isp_drc_stat_width.u32;
    o_isp_drc_stat_width.bits.isp_drc_stat_width = uisp_drc_stat_width;
    be_reg->isp_drc_stat_width.u32 = o_isp_drc_stat_width.u32;
}

/*  description : set the value of the member isp_drc_stat_width.isp_drc_total_width */
/*  input       : unsigned int uisp_drc_total_width: 16 bits */
static __inline td_void isp_drc_total_width_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_total_width)
{
    u_isp_drc_stat_width o_isp_drc_stat_width;
    o_isp_drc_stat_width.u32 = be_reg->isp_drc_stat_width.u32;
    o_isp_drc_stat_width.bits.isp_drc_total_width = uisp_drc_total_width;
    be_reg->isp_drc_stat_width.u32 = o_isp_drc_stat_width.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_y.isp_drc_val1_y */
/*  input       : unsigned int uisp_drc_val1_y: 8 bits */
static __inline td_void isp_drc_val1_y_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_val1_y)
{
    u_isp_drc_dark_gain_lmt_y o_isp_drc_dark_gain_lmt_y;
    o_isp_drc_dark_gain_lmt_y.u32 = be_reg->isp_drc_dark_gain_lmt_y.u32;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_val1_y = uisp_drc_val1_y;
    be_reg->isp_drc_dark_gain_lmt_y.u32 = o_isp_drc_dark_gain_lmt_y.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_y.isp_drc_sft1_y */
/*  input       : unsigned int uisp_drc_sft1_y: 4 bits */
static __inline td_void isp_drc_sft1_y_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_sft1_y)
{
    u_isp_drc_dark_gain_lmt_y o_isp_drc_dark_gain_lmt_y;
    o_isp_drc_dark_gain_lmt_y.u32 = be_reg->isp_drc_dark_gain_lmt_y.u32;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_sft1_y = uisp_drc_sft1_y;
    be_reg->isp_drc_dark_gain_lmt_y.u32 = o_isp_drc_dark_gain_lmt_y.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_y.isp_drc_val2_y */
/*  input       : unsigned int uisp_drc_val2_y: 4 bits */
static __inline td_void isp_drc_val2_y_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_val2_y)
{
    u_isp_drc_dark_gain_lmt_y o_isp_drc_dark_gain_lmt_y;
    o_isp_drc_dark_gain_lmt_y.u32 = be_reg->isp_drc_dark_gain_lmt_y.u32;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_val2_y = uisp_drc_val2_y;
    be_reg->isp_drc_dark_gain_lmt_y.u32 = o_isp_drc_dark_gain_lmt_y.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_y.isp_drc_sft2_y */
/*  input       : unsigned int uisp_drc_sft2_y: 4 bits */
static __inline td_void isp_drc_sft2_y_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_sft2_y)
{
    u_isp_drc_dark_gain_lmt_y o_isp_drc_dark_gain_lmt_y;
    o_isp_drc_dark_gain_lmt_y.u32 = be_reg->isp_drc_dark_gain_lmt_y.u32;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_sft2_y = uisp_drc_sft2_y;
    be_reg->isp_drc_dark_gain_lmt_y.u32 = o_isp_drc_dark_gain_lmt_y.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_c.isp_drc_val1_c */
/*  input       : unsigned int uisp_drc_val1_c: 8 bits */
static __inline td_void isp_drc_val1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_val1_c)
{
    u_isp_drc_dark_gain_lmt_c o_isp_drc_dark_gain_lmt_c;
    o_isp_drc_dark_gain_lmt_c.u32 = be_reg->isp_drc_dark_gain_lmt_c.u32;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_val1_c = uisp_drc_val1_c;
    be_reg->isp_drc_dark_gain_lmt_c.u32 = o_isp_drc_dark_gain_lmt_c.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_c.isp_drc_sft1_c */
/*  input       : unsigned int uisp_drc_sft1_c: 4 bits */
static __inline td_void isp_drc_sft1_c_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_sft1_c)
{
    u_isp_drc_dark_gain_lmt_c o_isp_drc_dark_gain_lmt_c;
    o_isp_drc_dark_gain_lmt_c.u32 = be_reg->isp_drc_dark_gain_lmt_c.u32;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_sft1_c = uisp_drc_sft1_c;
    be_reg->isp_drc_dark_gain_lmt_c.u32 = o_isp_drc_dark_gain_lmt_c.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_c.isp_drc_val2_c */
/*  input       : unsigned int uisp_drc_val2_c: 4 bits */
static __inline td_void isp_drc_val2_c_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_val2_c)
{
    u_isp_drc_dark_gain_lmt_c o_isp_drc_dark_gain_lmt_c;
    o_isp_drc_dark_gain_lmt_c.u32 = be_reg->isp_drc_dark_gain_lmt_c.u32;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_val2_c = uisp_drc_val2_c;
    be_reg->isp_drc_dark_gain_lmt_c.u32 = o_isp_drc_dark_gain_lmt_c.u32;
}

/*  description : set the value of the member isp_drc_dark_gain_lmt_c.isp_drc_sft2_c */
/*  input       : unsigned int uisp_drc_sft2_c: 4 bits */
static __inline td_void isp_drc_sft2_c_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_sft2_c)
{
    u_isp_drc_dark_gain_lmt_c o_isp_drc_dark_gain_lmt_c;
    o_isp_drc_dark_gain_lmt_c.u32 = be_reg->isp_drc_dark_gain_lmt_c.u32;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_sft2_c = uisp_drc_sft2_c;
    be_reg->isp_drc_dark_gain_lmt_c.u32 = o_isp_drc_dark_gain_lmt_c.u32;
}

/*  description : set the value of the member isp_drc_bright_gain_lmt.isp_drc_val */
/*  input       : unsigned int uisp_drc_val: 8 bits */
static __inline td_void isp_drc_val_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_val)
{
    u_isp_drc_bright_gain_lmt o_isp_drc_bright_gain_lmt;
    o_isp_drc_bright_gain_lmt.u32 = be_reg->isp_drc_bright_gain_lmt.u32;
    o_isp_drc_bright_gain_lmt.bits.isp_drc_val = uisp_drc_val;
    be_reg->isp_drc_bright_gain_lmt.u32 = o_isp_drc_bright_gain_lmt.u32;
}

/*  description : set the value of the member isp_drc_bright_gain_lmt.isp_drc_sft */
/*  input       : unsigned int uisp_drc_sft: 4 bits */
static __inline td_void isp_drc_sft_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_sft)
{
    u_isp_drc_bright_gain_lmt o_isp_drc_bright_gain_lmt;
    o_isp_drc_bright_gain_lmt.u32 = be_reg->isp_drc_bright_gain_lmt.u32;
    o_isp_drc_bright_gain_lmt.bits.isp_drc_sft = uisp_drc_sft;
    be_reg->isp_drc_bright_gain_lmt.u32 = o_isp_drc_bright_gain_lmt.u32;
}

/*  description : set the value of the member isp_drc_rgb_wgt.isp_drc_r_wgt */
/*  input       : unsigned int uisp_drc_r_wgt: 4 bits */
static __inline td_void isp_drc_r_wgt_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_r_wgt)
{
    u_isp_drc_rgb_wgt o_isp_drc_rgb_wgt;
    o_isp_drc_rgb_wgt.u32 = be_reg->isp_drc_rgb_wgt.u32;
    o_isp_drc_rgb_wgt.bits.isp_drc_r_wgt = uisp_drc_r_wgt;
    be_reg->isp_drc_rgb_wgt.u32 = o_isp_drc_rgb_wgt.u32;
}

/*  description : set the value of the member isp_drc_rgb_wgt.isp_drc_g_wgt */
/*  input       : unsigned int uisp_drc_g_wgt: 5 bits */
static __inline td_void isp_drc_g_wgt_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_g_wgt)
{
    u_isp_drc_rgb_wgt o_isp_drc_rgb_wgt;
    o_isp_drc_rgb_wgt.u32 = be_reg->isp_drc_rgb_wgt.u32;
    o_isp_drc_rgb_wgt.bits.isp_drc_g_wgt = uisp_drc_g_wgt;
    be_reg->isp_drc_rgb_wgt.u32 = o_isp_drc_rgb_wgt.u32;
}

/*  description : set the value of the member isp_drc_rgb_wgt.isp_drc_b_wgt */
/*  input       : unsigned int uisp_drc_b_wgt: 4 bits */
static __inline td_void isp_drc_b_wgt_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_b_wgt)
{
    u_isp_drc_rgb_wgt o_isp_drc_rgb_wgt;
    o_isp_drc_rgb_wgt.u32 = be_reg->isp_drc_rgb_wgt.u32;
    o_isp_drc_rgb_wgt.bits.isp_drc_b_wgt = uisp_drc_b_wgt;
    be_reg->isp_drc_rgb_wgt.u32 = o_isp_drc_rgb_wgt.u32;
}

/*  description : set the value of the member isp_drc_dp_dtc.isp_drc_dp_det_thrmin */
/*  input       : unsigned int uisp_drc_dp_det_thrmin: 16 bits */
static __inline td_void isp_drc_dp_det_thrmin_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_thrmin)
{
    u_isp_drc_dp_dtc o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.u32 = be_reg->isp_drc_dp_dtc.u32;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_thrmin = uisp_drc_dp_det_thrmin;
    be_reg->isp_drc_dp_dtc.u32 = o_isp_drc_dp_dtc.u32;
}

/*  description : set the value of the member isp_drc_dp_dtc.isp_drc_dp_det_thrslo */
/*  input       : unsigned int uisp_drc_dp_det_thrslo: 5 bits */
static __inline td_void isp_drc_dp_det_thrslo_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_thrslo)
{
    u_isp_drc_dp_dtc o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.u32 = be_reg->isp_drc_dp_dtc.u32;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_thrslo = uisp_drc_dp_det_thrslo;
    be_reg->isp_drc_dp_dtc.u32 = o_isp_drc_dp_dtc.u32;
}

/*  description : set the value of the member isp_drc_dp_dtc.isp_drc_dp_det_g2rb */
/*  input       : unsigned int uisp_drc_dp_det_g2rb: 1 bits */
static __inline td_void isp_drc_dp_det_g2rb_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_g2rb)
{
    u_isp_drc_dp_dtc o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.u32 = be_reg->isp_drc_dp_dtc.u32;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_g2rb = uisp_drc_dp_det_g2rb;
    be_reg->isp_drc_dp_dtc.u32 = o_isp_drc_dp_dtc.u32;
}

/*  description : set the value of the member isp_drc_dp_dtc.isp_drc_dp_det_rb2rb */
/*  input       : unsigned int uisp_drc_dp_det_rb2rb: 1 bits */
static __inline td_void isp_drc_dp_det_rb2rb_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_rb2rb)
{
    u_isp_drc_dp_dtc o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.u32 = be_reg->isp_drc_dp_dtc.u32;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_rb2rb = uisp_drc_dp_det_rb2rb;
    be_reg->isp_drc_dp_dtc.u32 = o_isp_drc_dp_dtc.u32;
}

/*  description : set the value of the member isp_drc_dp_dtc.isp_drc_dp_det_replctr */
/*  input       : unsigned int uisp_drc_dp_det_replctr: 1 bits */
static __inline td_void isp_drc_dp_det_replctr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_replctr)
{
    u_isp_drc_dp_dtc o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.u32 = be_reg->isp_drc_dp_dtc.u32;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_replctr = uisp_drc_dp_det_replctr;
    be_reg->isp_drc_dp_dtc.u32 = o_isp_drc_dp_dtc.u32;
}

/*  description : set the value of the member isp_drc_dp_dtc.isp_drc_dp_det_rngrto */
/*  input       : unsigned int uisp_drc_dp_det_rngrto: 5 bits */
static __inline td_void isp_drc_dp_det_rngrto_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dp_det_rngrto)
{
    u_isp_drc_dp_dtc o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.u32 = be_reg->isp_drc_dp_dtc.u32;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_rngrto = uisp_drc_dp_det_rngrto;
    be_reg->isp_drc_dp_dtc.u32 = o_isp_drc_dp_dtc.u32;
}

/*  description : set the value of the member isp_drc_gain_clip.isp_drc_gain_clip_step */
/*  input       : unsigned int uisp_drc_gain_clip_step: 4 bits */
static __inline td_void isp_drc_gain_clip_step_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_gain_clip_step)
{
    u_isp_drc_gain_clip o_isp_drc_gain_clip;
    o_isp_drc_gain_clip.u32 = be_reg->isp_drc_gain_clip.u32;
    o_isp_drc_gain_clip.bits.isp_drc_gain_clip_step = uisp_drc_gain_clip_step;
    be_reg->isp_drc_gain_clip.u32 = o_isp_drc_gain_clip.u32;
}

/*  description : set the value of the member isp_drc_gain_clip.isp_drc_gain_clip_knee */
/*  input       : unsigned int uisp_drc_gain_clip_knee: 4 bits */
static __inline td_void isp_drc_gain_clip_knee_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_gain_clip_knee)
{
    u_isp_drc_gain_clip o_isp_drc_gain_clip;
    o_isp_drc_gain_clip.u32 = be_reg->isp_drc_gain_clip.u32;
    o_isp_drc_gain_clip.bits.isp_drc_gain_clip_knee = uisp_drc_gain_clip_knee;
    be_reg->isp_drc_gain_clip.u32 = o_isp_drc_gain_clip.u32;
}

/*  description : set the value of the member isp_drc_color_ctrl.isp_drc_cc_lut_ctrl */
/*  input       : unsigned int uisp_drc_cc_lut_ctrl: 1 bits */
static __inline td_void isp_drc_cc_lut_ctrl_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_cc_lut_ctrl)
{
    u_isp_drc_color_ctrl o_isp_drc_color_ctrl;
    o_isp_drc_color_ctrl.u32 = be_reg->isp_drc_color_ctrl.u32;
    o_isp_drc_color_ctrl.bits.isp_drc_cc_lut_ctrl = uisp_drc_cc_lut_ctrl;
    be_reg->isp_drc_color_ctrl.u32 = o_isp_drc_color_ctrl.u32;
}

/*  description : set the value of the member isp_drc_global_corr.isp_drc_cc_global_corr */
/*  input       : unsigned int uisp_drc_cc_global_corr: 12 bits */
static __inline td_void isp_drc_cc_global_corr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_cc_global_corr)
{
    u_isp_drc_global_corr o_isp_drc_global_corr;
    o_isp_drc_global_corr.u32 = be_reg->isp_drc_global_corr.u32;
    o_isp_drc_global_corr.bits.isp_drc_cc_global_corr = uisp_drc_cc_global_corr;
    be_reg->isp_drc_global_corr.u32 = o_isp_drc_global_corr.u32;
}

/*  description : set the value of the member isp_drc_mixing_coring.isp_drc_mixing_coring */
/*  input       : unsigned int uisp_drc_mixing_coring: 8 bits */
static __inline td_void isp_drc_mixing_coring_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_mixing_coring)
{
    u_isp_drc_mixing_coring o_isp_drc_mixing_coring;
    o_isp_drc_mixing_coring.u32 = be_reg->isp_drc_mixing_coring.u32;
    o_isp_drc_mixing_coring.bits.isp_drc_mixing_coring = uisp_drc_mixing_coring;
    be_reg->isp_drc_mixing_coring.u32 = o_isp_drc_mixing_coring.u32;
}

/*  description : set the value of the member isp_drc_mixing_dark.isp_drc_dark_min */
/*  input       : unsigned int uisp_drc_dark_min: 8 bits */
static __inline td_void isp_drc_dark_min_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dark_min)
{
    u_isp_drc_mixing_dark o_isp_drc_mixing_dark;
    o_isp_drc_mixing_dark.u32 = be_reg->isp_drc_mixing_dark.u32;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_min = uisp_drc_dark_min;
    be_reg->isp_drc_mixing_dark.u32 = o_isp_drc_mixing_dark.u32;
}

/*  description : set the value of the member isp_drc_mixing_dark.isp_drc_dark_max */
/*  input       : unsigned int uisp_drc_dark_max: 8 bits */
static __inline td_void isp_drc_dark_max_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dark_max)
{
    u_isp_drc_mixing_dark o_isp_drc_mixing_dark;
    o_isp_drc_mixing_dark.u32 = be_reg->isp_drc_mixing_dark.u32;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_max = uisp_drc_dark_max;
    be_reg->isp_drc_mixing_dark.u32 = o_isp_drc_mixing_dark.u32;
}

/*  description : set the value of the member isp_drc_mixing_dark.isp_drc_dark_thr */
/*  input       : unsigned int uisp_drc_dark_thr: 8 bits */
static __inline td_void isp_drc_dark_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dark_thr)
{
    u_isp_drc_mixing_dark o_isp_drc_mixing_dark;
    o_isp_drc_mixing_dark.u32 = be_reg->isp_drc_mixing_dark.u32;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_thr = uisp_drc_dark_thr;
    be_reg->isp_drc_mixing_dark.u32 = o_isp_drc_mixing_dark.u32;
}

/*  description : set the value of the member isp_drc_mixing_dark.isp_drc_dark_slo */
/*  input       : unsigned int uisp_drc_dark_slo: 4 bits */
static __inline td_void isp_drc_dark_slo_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_dark_slo)
{
    u_isp_drc_mixing_dark o_isp_drc_mixing_dark;
    o_isp_drc_mixing_dark.u32 = be_reg->isp_drc_mixing_dark.u32;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_slo = uisp_drc_dark_slo;
    be_reg->isp_drc_mixing_dark.u32 = o_isp_drc_mixing_dark.u32;
}

/*  description : set the value of the member isp_drc_mixing_bright.isp_drc_bright_min */
/*  input       : unsigned int uisp_drc_bright_min: 8 bits */
static __inline td_void isp_drc_bright_min_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_bright_min)
{
    u_isp_drc_mixing_bright o_isp_drc_mixing_bright;
    o_isp_drc_mixing_bright.u32 = be_reg->isp_drc_mixing_bright.u32;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_min = uisp_drc_bright_min;
    be_reg->isp_drc_mixing_bright.u32 = o_isp_drc_mixing_bright.u32;
}

/*  description : set the value of the member isp_drc_mixing_bright.isp_drc_bright_max */
/*  input       : unsigned int uisp_drc_bright_max: 8 bits */
static __inline td_void isp_drc_bright_max_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_bright_max)
{
    u_isp_drc_mixing_bright o_isp_drc_mixing_bright;
    o_isp_drc_mixing_bright.u32 = be_reg->isp_drc_mixing_bright.u32;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_max = uisp_drc_bright_max;
    be_reg->isp_drc_mixing_bright.u32 = o_isp_drc_mixing_bright.u32;
}

/*  description : set the value of the member isp_drc_mixing_bright.isp_drc_bright_thr */
/*  input       : unsigned int uisp_drc_bright_thr: 8 bits */
static __inline td_void isp_drc_bright_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_bright_thr)
{
    u_isp_drc_mixing_bright o_isp_drc_mixing_bright;
    o_isp_drc_mixing_bright.u32 = be_reg->isp_drc_mixing_bright.u32;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_thr = uisp_drc_bright_thr;
    be_reg->isp_drc_mixing_bright.u32 = o_isp_drc_mixing_bright.u32;
}

/*  description : set the value of the member isp_drc_mixing_bright.isp_drc_bright_slo */
/*  input       : unsigned int uisp_drc_bright_slo: 4 bits */
static __inline td_void isp_drc_bright_slo_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_bright_slo)
{
    u_isp_drc_mixing_bright o_isp_drc_mixing_bright;
    o_isp_drc_mixing_bright.u32 = be_reg->isp_drc_mixing_bright.u32;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_slo = uisp_drc_bright_slo;
    be_reg->isp_drc_mixing_bright.u32 = o_isp_drc_mixing_bright.u32;
}

/*  description : set the value of the member isp_drc_tmlut0_waddr.isp_drc_tmlut0_waddr */
/*  input       : unsigned int uisp_drc_tmlut0_waddr: 32 bits */
static __inline td_void isp_drc_tmlut0_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_tmlut0_waddr)
{
    be_reg->isp_drc_tmlut0_waddr.u32 = uisp_drc_tmlut0_waddr;
}


/*  description : set the value of the member isp_drc_tmlut0_wdata.isp_drc_tmlut0_wdata */
/*  input       : unsigned int uisp_drc_tmlut0_wdata: 32 bits */
static __inline td_void isp_drc_tmlut0_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_tmlut0_wdata)
{
    be_reg->isp_drc_tmlut0_wdata.u32 = uisp_drc_tmlut0_wdata;
}


/*  description : set the value of the member isp_drc_tmlut0_raddr.isp_drc_tmlut0_raddr */
/*  input       : unsigned int uisp_drc_tmlut0_raddr: 32 bits */
static __inline td_void isp_drc_tmlut0_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_tmlut0_raddr)
{
    be_reg->isp_drc_tmlut0_raddr.u32 = uisp_drc_tmlut0_raddr;
}


/*  description : set the value of the member isp_drc_cclut_waddr.isp_drc_cclut_waddr */
/*  input       : unsigned int uisp_drc_cclut_waddr: 32 bits */
static __inline td_void isp_drc_cclut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_cclut_waddr)
{
    be_reg->isp_drc_cclut_waddr.u32 = uisp_drc_cclut_waddr;
}


/*  description : set the value of the member isp_drc_cclut_wdata.isp_drc_cclut_wdata */
/*  input       : unsigned int uisp_drc_cclut_wdata: 32 bits */
static __inline td_void isp_drc_cclut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_cclut_wdata)
{
    be_reg->isp_drc_cclut_wdata.u32 = uisp_drc_cclut_wdata;
}


/*  description : set the value of the member isp_drc_cclut_raddr.isp_drc_cclut_raddr */
/*  input       : unsigned int uisp_drc_cclut_raddr: 32 bits */
static __inline td_void isp_drc_cclut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_cclut_raddr)
{
    be_reg->isp_drc_cclut_raddr.u32 = uisp_drc_cclut_raddr;
}

/*  description : set the value of the member isp_drc_coefalut_waddr.isp_drc_coefalut_waddr */
/*  input       : unsigned int uisp_drc_coefalut_waddr: 32 bits */
static __inline td_void isp_drc_coefalut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_coefalut_waddr)
{
    be_reg->isp_drc_coefalut_waddr.u32 = uisp_drc_coefalut_waddr;
}


/*  description : set the value of the member isp_drc_coefalut_wdata.isp_drc_coefalut_wdata */
/*  input       : unsigned int uisp_drc_coefalut_wdata: 32 bits */
static __inline td_void isp_drc_coefalut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_coefalut_wdata)
{
    be_reg->isp_drc_coefalut_wdata.u32 = uisp_drc_coefalut_wdata;
}


/*  description : set the value of the member isp_drc_coefalut_raddr.isp_drc_coefalut_raddr */
/*  input       : unsigned int uisp_drc_coefalut_raddr: 32 bits */
static __inline td_void isp_drc_coefalut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_coefalut_raddr)
{
    be_reg->isp_drc_coefalut_raddr.u32 = uisp_drc_coefalut_raddr;
}


/*  description : set the value of the member isp_drc_delut_waddr.isp_drc_delut_waddr */
/*  input       : unsigned int uisp_drc_delut_waddr: 32 bits */
static __inline td_void isp_drc_delut_waddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_delut_waddr)
{
    be_reg->isp_drc_delut_waddr.u32 = uisp_drc_delut_waddr;
}


/*  description : set the value of the member isp_drc_delut_wdata.isp_drc_delut_wdata */
/*  input       : unsigned int uisp_drc_delut_wdata: 32 bits */
static __inline td_void isp_drc_delut_wdata_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_delut_wdata)
{
    be_reg->isp_drc_delut_wdata.u32 = uisp_drc_delut_wdata;
}


/*  description : set the value of the member isp_drc_delut_raddr.isp_drc_delut_raddr */
/*  input       : unsigned int uisp_drc_delut_raddr: 32 bits */
static __inline td_void isp_drc_delut_raddr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_delut_raddr)
{
    be_reg->isp_drc_delut_raddr.u32 = uisp_drc_delut_raddr;
}

/*  description : set the value of the member isp_drc_grad_rev.isp_drc_grad_rev_thres */
/*  input       : unsigned int uisp_drc_grad_rev_thres: 8 bits */
static __inline td_void isp_drc_grad_rev_thres_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_grad_rev_thres)
{
    u_isp_drc_grad_rev o_isp_drc_grad_rev;
    o_isp_drc_grad_rev.u32 = be_reg->isp_drc_grad_rev.u32;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_thres = uisp_drc_grad_rev_thres;
    be_reg->isp_drc_grad_rev.u32 = o_isp_drc_grad_rev.u32;
}

/*  description : set the value of the member isp_drc_grad_rev.isp_drc_grad_rev_max */
/*  input       : unsigned int uisp_drc_grad_rev_max: 7 bits */
static __inline td_void isp_drc_grad_rev_max_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_grad_rev_max)
{
    u_isp_drc_grad_rev o_isp_drc_grad_rev;
    o_isp_drc_grad_rev.u32 = be_reg->isp_drc_grad_rev.u32;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_max = uisp_drc_grad_rev_max;
    be_reg->isp_drc_grad_rev.u32 = o_isp_drc_grad_rev.u32;
}

/*  description : set the value of the member isp_drc_grad_rev.isp_drc_grad_rev_slope */
/*  input       : unsigned int uisp_drc_grad_rev_slope: 4 bits */
static __inline td_void isp_drc_grad_rev_slope_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_grad_rev_slope)
{
    u_isp_drc_grad_rev o_isp_drc_grad_rev;
    o_isp_drc_grad_rev.u32 = be_reg->isp_drc_grad_rev.u32;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_slope = uisp_drc_grad_rev_slope;
    be_reg->isp_drc_grad_rev.u32 = o_isp_drc_grad_rev.u32;
}

/*  description : set the value of the member isp_drc_grad_rev.isp_drc_grad_rev_shift */
/*  input       : unsigned int uisp_drc_grad_rev_shift: 3 bits */
static __inline td_void isp_drc_grad_rev_shift_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_grad_rev_shift)
{
    u_isp_drc_grad_rev o_isp_drc_grad_rev;
    o_isp_drc_grad_rev.u32 = be_reg->isp_drc_grad_rev.u32;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_shift = uisp_drc_grad_rev_shift;
    be_reg->isp_drc_grad_rev.u32 = o_isp_drc_grad_rev.u32;
}

/*  description : set the value of the member isp_drc_vbi_strength.isp_drc_var_rng_fine */
/*  input       : unsigned int uisp_drc_var_rng_fine: 2 bits */
static __inline td_void isp_drc_var_rng_fine_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_var_rng_fine)
{
    u_isp_drc_vbi_strength o_isp_drc_vbi_strength;
    o_isp_drc_vbi_strength.u32 = be_reg->isp_drc_vbi_strength.u32;
    o_isp_drc_vbi_strength.bits.isp_drc_var_rng_fine = uisp_drc_var_rng_fine;
    be_reg->isp_drc_vbi_strength.u32 = o_isp_drc_vbi_strength.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_0.isp_drc_prev_luma_0 */
/*  input       : unsigned int uisp_drc_prev_luma_0: 24 bits */
static __inline td_void isp_drc_prev_luma_0_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_0)
{
    u_isp_drc_prev_luma_0 o_isp_drc_prev_luma_0;
    o_isp_drc_prev_luma_0.u32 = be_reg->isp_drc_prev_luma_0.u32;
    o_isp_drc_prev_luma_0.bits.isp_drc_prev_luma_0 = uisp_drc_prev_luma_0;
    be_reg->isp_drc_prev_luma_0.u32 = o_isp_drc_prev_luma_0.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_1.isp_drc_prev_luma_1 */
/*  input       : unsigned int uisp_drc_prev_luma_1: 24 bits */
static __inline td_void isp_drc_prev_luma_1_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_1)
{
    u_isp_drc_prev_luma_1 o_isp_drc_prev_luma_1;
    o_isp_drc_prev_luma_1.u32 = be_reg->isp_drc_prev_luma_1.u32;
    o_isp_drc_prev_luma_1.bits.isp_drc_prev_luma_1 = uisp_drc_prev_luma_1;
    be_reg->isp_drc_prev_luma_1.u32 = o_isp_drc_prev_luma_1.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_2.isp_drc_prev_luma_2 */
/*  input       : unsigned int uisp_drc_prev_luma_2: 24 bits */
static __inline td_void isp_drc_prev_luma_2_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_2)
{
    u_isp_drc_prev_luma_2 o_isp_drc_prev_luma_2;
    o_isp_drc_prev_luma_2.u32 = be_reg->isp_drc_prev_luma_2.u32;
    o_isp_drc_prev_luma_2.bits.isp_drc_prev_luma_2 = uisp_drc_prev_luma_2;
    be_reg->isp_drc_prev_luma_2.u32 = o_isp_drc_prev_luma_2.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_3.isp_drc_prev_luma_3 */
/*  input       : unsigned int uisp_drc_prev_luma_3: 24 bits */
static __inline td_void isp_drc_prev_luma_3_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_3)
{
    u_isp_drc_prev_luma_3 o_isp_drc_prev_luma_3;
    o_isp_drc_prev_luma_3.u32 = be_reg->isp_drc_prev_luma_3.u32;
    o_isp_drc_prev_luma_3.bits.isp_drc_prev_luma_3 = uisp_drc_prev_luma_3;
    be_reg->isp_drc_prev_luma_3.u32 = o_isp_drc_prev_luma_3.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_4.isp_drc_prev_luma_4 */
/*  input       : unsigned int uisp_drc_prev_luma_4: 24 bits */
static __inline td_void isp_drc_prev_luma_4_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_4)
{
    u_isp_drc_prev_luma_4 o_isp_drc_prev_luma_4;
    o_isp_drc_prev_luma_4.u32 = be_reg->isp_drc_prev_luma_4.u32;
    o_isp_drc_prev_luma_4.bits.isp_drc_prev_luma_4 = uisp_drc_prev_luma_4;
    be_reg->isp_drc_prev_luma_4.u32 = o_isp_drc_prev_luma_4.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_5.isp_drc_prev_luma_5 */
/*  input       : unsigned int uisp_drc_prev_luma_5: 24 bits */
static __inline td_void isp_drc_prev_luma_5_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_5)
{
    u_isp_drc_prev_luma_5 o_isp_drc_prev_luma_5;
    o_isp_drc_prev_luma_5.u32 = be_reg->isp_drc_prev_luma_5.u32;
    o_isp_drc_prev_luma_5.bits.isp_drc_prev_luma_5 = uisp_drc_prev_luma_5;
    be_reg->isp_drc_prev_luma_5.u32 = o_isp_drc_prev_luma_5.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_6.isp_drc_prev_luma_6 */
/*  input       : unsigned int uisp_drc_prev_luma_6: 24 bits */
static __inline td_void isp_drc_prev_luma_6_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_6)
{
    u_isp_drc_prev_luma_6 o_isp_drc_prev_luma_6;
    o_isp_drc_prev_luma_6.u32 = be_reg->isp_drc_prev_luma_6.u32;
    o_isp_drc_prev_luma_6.bits.isp_drc_prev_luma_6 = uisp_drc_prev_luma_6;
    be_reg->isp_drc_prev_luma_6.u32 = o_isp_drc_prev_luma_6.u32;
}

/*  description : set the value of the member isp_drc_prev_luma_7.isp_drc_prev_luma_7 */
/*  input       : unsigned int uisp_drc_prev_luma_7: 24 bits */
static __inline td_void isp_drc_prev_luma_7_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_prev_luma_7)
{
    u_isp_drc_prev_luma_7 o_isp_drc_prev_luma_7;
    o_isp_drc_prev_luma_7.u32 = be_reg->isp_drc_prev_luma_7.u32;
    o_isp_drc_prev_luma_7.bits.isp_drc_prev_luma_7 = uisp_drc_prev_luma_7;
    be_reg->isp_drc_prev_luma_7.u32 = o_isp_drc_prev_luma_7.u32;
}

/*  description : set the value of the member isp_drc_suppress_bright.isp_drc_suppress_bright_max */
/*  input       : unsigned int uisp_drc_suppress_bright_max: 8 bits */
static __inline td_void isp_drc_suppress_bright_max_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_bright_max)
{
    u_isp_drc_suppress_bright o_isp_drc_suppress_bright;
    o_isp_drc_suppress_bright.u32 = be_reg->isp_drc_suppress_bright.u32;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_max = uisp_drc_suppress_bright_max;
    be_reg->isp_drc_suppress_bright.u32 = o_isp_drc_suppress_bright.u32;
}

/*  description : set the value of the member isp_drc_suppress_bright.isp_drc_suppress_bright_min */
/*  input       : unsigned int uisp_drc_suppress_bright_min: 8 bits */
static __inline td_void isp_drc_suppress_bright_min_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_bright_min)
{
    u_isp_drc_suppress_bright o_isp_drc_suppress_bright;
    o_isp_drc_suppress_bright.u32 = be_reg->isp_drc_suppress_bright.u32;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_min = uisp_drc_suppress_bright_min;
    be_reg->isp_drc_suppress_bright.u32 = o_isp_drc_suppress_bright.u32;
}

/*  description : set the value of the member isp_drc_suppress_bright.isp_drc_suppress_bright_thr */
/*  input       : unsigned int uisp_drc_suppress_bright_thr: 8 bits */
static __inline td_void isp_drc_suppress_bright_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_bright_thr)
{
    u_isp_drc_suppress_bright o_isp_drc_suppress_bright;
    o_isp_drc_suppress_bright.u32 = be_reg->isp_drc_suppress_bright.u32;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_thr = uisp_drc_suppress_bright_thr;
    be_reg->isp_drc_suppress_bright.u32 = o_isp_drc_suppress_bright.u32;
}

/*  description : set the value of the member isp_drc_suppress_bright.isp_drc_suppress_bright_slo */
/*  input       : unsigned int uisp_drc_suppress_bright_slo: 4 bits */
static __inline td_void isp_drc_suppress_bright_slo_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_bright_slo)
{
    u_isp_drc_suppress_bright o_isp_drc_suppress_bright;
    o_isp_drc_suppress_bright.u32 = be_reg->isp_drc_suppress_bright.u32;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_slo = uisp_drc_suppress_bright_slo;
    be_reg->isp_drc_suppress_bright.u32 = o_isp_drc_suppress_bright.u32;
}

/*  description : set the value of the member isp_drc_suppress_dark.isp_drc_suppress_dark_max */
/*  input       : unsigned int uisp_drc_suppress_dark_max: 8 bits */
static __inline td_void isp_drc_suppress_dark_max_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_dark_max)
{
    u_isp_drc_suppress_dark o_isp_drc_suppress_dark;
    o_isp_drc_suppress_dark.u32 = be_reg->isp_drc_suppress_dark.u32;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_max = uisp_drc_suppress_dark_max;
    be_reg->isp_drc_suppress_dark.u32 = o_isp_drc_suppress_dark.u32;
}

/*  description : set the value of the member isp_drc_suppress_dark.isp_drc_suppress_dark_min */
/*  input       : unsigned int uisp_drc_suppress_dark_min: 8 bits */
static __inline td_void isp_drc_suppress_dark_min_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_dark_min)
{
    u_isp_drc_suppress_dark o_isp_drc_suppress_dark;
    o_isp_drc_suppress_dark.u32 = be_reg->isp_drc_suppress_dark.u32;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_min = uisp_drc_suppress_dark_min;
    be_reg->isp_drc_suppress_dark.u32 = o_isp_drc_suppress_dark.u32;
}

/*  description : set the value of the member isp_drc_suppress_dark.isp_drc_suppress_dark_thr */
/*  input       : unsigned int uisp_drc_suppress_dark_thr: 8 bits */
static __inline td_void isp_drc_suppress_dark_thr_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_dark_thr)
{
    u_isp_drc_suppress_dark o_isp_drc_suppress_dark;
    o_isp_drc_suppress_dark.u32 = be_reg->isp_drc_suppress_dark.u32;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_thr = uisp_drc_suppress_dark_thr;
    be_reg->isp_drc_suppress_dark.u32 = o_isp_drc_suppress_dark.u32;
}

/*  description : set the value of the member isp_drc_suppress_dark.isp_drc_suppress_dark_slo */
/*  input       : unsigned int uisp_drc_suppress_dark_slo: 4 bits */
static __inline td_void isp_drc_suppress_dark_slo_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_suppress_dark_slo)
{
    u_isp_drc_suppress_dark o_isp_drc_suppress_dark;
    o_isp_drc_suppress_dark.u32 = be_reg->isp_drc_suppress_dark.u32;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_slo = uisp_drc_suppress_dark_slo;
    be_reg->isp_drc_suppress_dark.u32 = o_isp_drc_suppress_dark.u32;
}

/*  description : set the value of the member isp_drc_idxbase0.isp_drc_idxbase0 */
/*  input       : unsigned int uisp_drc_idxbase0: 8 bits */
static __inline td_void isp_drc_idxbase0_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase0)
{
    u_isp_drc_idxbase0 o_isp_drc_idxbase0;
    o_isp_drc_idxbase0.u32 = be_reg->isp_drc_idxbase0.u32;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase0 = uisp_drc_idxbase0;
    be_reg->isp_drc_idxbase0.u32 = o_isp_drc_idxbase0.u32;
}

/*  description : set the value of the member isp_drc_idxbase0.isp_drc_idxbase1 */
/*  input       : unsigned int uisp_drc_idxbase1: 8 bits */
static __inline td_void isp_drc_idxbase1_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase1)
{
    u_isp_drc_idxbase0 o_isp_drc_idxbase0;
    o_isp_drc_idxbase0.u32 = be_reg->isp_drc_idxbase0.u32;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase1 = uisp_drc_idxbase1;
    be_reg->isp_drc_idxbase0.u32 = o_isp_drc_idxbase0.u32;
}

/*  description : set the value of the member isp_drc_idxbase0.isp_drc_idxbase2 */
/*  input       : unsigned int uisp_drc_idxbase2: 8 bits */
static __inline td_void isp_drc_idxbase2_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase2)
{
    u_isp_drc_idxbase0 o_isp_drc_idxbase0;
    o_isp_drc_idxbase0.u32 = be_reg->isp_drc_idxbase0.u32;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase2 = uisp_drc_idxbase2;
    be_reg->isp_drc_idxbase0.u32 = o_isp_drc_idxbase0.u32;
}

/*  description : set the value of the member isp_drc_idxbase0.isp_drc_idxbase3 */
/*  input       : unsigned int uisp_drc_idxbase3: 8 bits */
static __inline td_void isp_drc_idxbase3_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase3)
{
    u_isp_drc_idxbase0 o_isp_drc_idxbase0;
    o_isp_drc_idxbase0.u32 = be_reg->isp_drc_idxbase0.u32;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase3 = uisp_drc_idxbase3;
    be_reg->isp_drc_idxbase0.u32 = o_isp_drc_idxbase0.u32;
}

/*  description : set the value of the member isp_drc_idxbase1.isp_drc_idxbase4 */
/*  input       : unsigned int uisp_drc_idxbase4: 8 bits */
static __inline td_void isp_drc_idxbase4_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase4)
{
    u_isp_drc_idxbase1 o_isp_drc_idxbase1;
    o_isp_drc_idxbase1.u32 = be_reg->isp_drc_idxbase1.u32;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase4 = uisp_drc_idxbase4;
    be_reg->isp_drc_idxbase1.u32 = o_isp_drc_idxbase1.u32;
}

/*  description : set the value of the member isp_drc_idxbase1.isp_drc_idxbase5 */
/*  input       : unsigned int uisp_drc_idxbase5: 8 bits */
static __inline td_void isp_drc_idxbase5_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase5)
{
    u_isp_drc_idxbase1 o_isp_drc_idxbase1;
    o_isp_drc_idxbase1.u32 = be_reg->isp_drc_idxbase1.u32;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase5 = uisp_drc_idxbase5;
    be_reg->isp_drc_idxbase1.u32 = o_isp_drc_idxbase1.u32;
}

/*  description : set the value of the member isp_drc_idxbase1.isp_drc_idxbase6 */
/*  input       : unsigned int uisp_drc_idxbase6: 8 bits */
static __inline td_void isp_drc_idxbase6_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase6)
{
    u_isp_drc_idxbase1 o_isp_drc_idxbase1;
    o_isp_drc_idxbase1.u32 = be_reg->isp_drc_idxbase1.u32;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase6 = uisp_drc_idxbase6;
    be_reg->isp_drc_idxbase1.u32 = o_isp_drc_idxbase1.u32;
}

/*  description : set the value of the member isp_drc_idxbase1.isp_drc_idxbase7 */
/*  input       : unsigned int uisp_drc_idxbase7: 8 bits */
static __inline td_void isp_drc_idxbase7_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_idxbase7)
{
    u_isp_drc_idxbase1 o_isp_drc_idxbase1;
    o_isp_drc_idxbase1.u32 = be_reg->isp_drc_idxbase1.u32;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase7 = uisp_drc_idxbase7;
    be_reg->isp_drc_idxbase1.u32 = o_isp_drc_idxbase1.u32;
}

/*  description : set the value of the member isp_drc_maxval0.isp_drc_maxval0 */
/*  input       : unsigned int uisp_drc_maxval0: 8 bits */
static __inline td_void isp_drc_maxval0_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval0)
{
    u_isp_drc_maxval0 o_isp_drc_maxval0;
    o_isp_drc_maxval0.u32 = be_reg->isp_drc_maxval0.u32;
    o_isp_drc_maxval0.bits.isp_drc_maxval0 = uisp_drc_maxval0;
    be_reg->isp_drc_maxval0.u32 = o_isp_drc_maxval0.u32;
}

/*  description : set the value of the member isp_drc_maxval0.isp_drc_maxval1 */
/*  input       : unsigned int uisp_drc_maxval1: 8 bits */
static __inline td_void isp_drc_maxval1_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval1)
{
    u_isp_drc_maxval0 o_isp_drc_maxval0;
    o_isp_drc_maxval0.u32 = be_reg->isp_drc_maxval0.u32;
    o_isp_drc_maxval0.bits.isp_drc_maxval1 = uisp_drc_maxval1;
    be_reg->isp_drc_maxval0.u32 = o_isp_drc_maxval0.u32;
}

/*  description : set the value of the member isp_drc_maxval0.isp_drc_maxval2 */
/*  input       : unsigned int uisp_drc_maxval2: 8 bits */
static __inline td_void isp_drc_maxval2_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval2)
{
    u_isp_drc_maxval0 o_isp_drc_maxval0;
    o_isp_drc_maxval0.u32 = be_reg->isp_drc_maxval0.u32;
    o_isp_drc_maxval0.bits.isp_drc_maxval2 = uisp_drc_maxval2;
    be_reg->isp_drc_maxval0.u32 = o_isp_drc_maxval0.u32;
}

/*  description : set the value of the member isp_drc_maxval0.isp_drc_maxval3 */
/*  input       : unsigned int uisp_drc_maxval3: 8 bits */
static __inline td_void isp_drc_maxval3_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval3)
{
    u_isp_drc_maxval0 o_isp_drc_maxval0;
    o_isp_drc_maxval0.u32 = be_reg->isp_drc_maxval0.u32;
    o_isp_drc_maxval0.bits.isp_drc_maxval3 = uisp_drc_maxval3;
    be_reg->isp_drc_maxval0.u32 = o_isp_drc_maxval0.u32;
}

/*  description : set the value of the member isp_drc_maxval1.isp_drc_maxval4 */
/*  input       : unsigned int uisp_drc_maxval4: 8 bits */
static __inline td_void isp_drc_maxval4_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval4)
{
    u_isp_drc_maxval1 o_isp_drc_maxval1;
    o_isp_drc_maxval1.u32 = be_reg->isp_drc_maxval1.u32;
    o_isp_drc_maxval1.bits.isp_drc_maxval4 = uisp_drc_maxval4;
    be_reg->isp_drc_maxval1.u32 = o_isp_drc_maxval1.u32;
}

/*  description : set the value of the member isp_drc_maxval1.isp_drc_maxval5 */
/*  input       : unsigned int uisp_drc_maxval5: 8 bits */
static __inline td_void isp_drc_maxval5_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval5)
{
    u_isp_drc_maxval1 o_isp_drc_maxval1;
    o_isp_drc_maxval1.u32 = be_reg->isp_drc_maxval1.u32;
    o_isp_drc_maxval1.bits.isp_drc_maxval5 = uisp_drc_maxval5;
    be_reg->isp_drc_maxval1.u32 = o_isp_drc_maxval1.u32;
}

/*  description : set the value of the member isp_drc_maxval1.isp_drc_maxval6 */
/*  input       : unsigned int uisp_drc_maxval6: 8 bits */
static __inline td_void isp_drc_maxval6_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval6)
{
    u_isp_drc_maxval1 o_isp_drc_maxval1;
    o_isp_drc_maxval1.u32 = be_reg->isp_drc_maxval1.u32;
    o_isp_drc_maxval1.bits.isp_drc_maxval6 = uisp_drc_maxval6;
    be_reg->isp_drc_maxval1.u32 = o_isp_drc_maxval1.u32;
}

/*  description : set the value of the member isp_drc_maxval1.isp_drc_maxval7 */
/*  input       : unsigned int uisp_drc_maxval7: 8 bits */
static __inline td_void isp_drc_maxval7_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_maxval7)
{
    u_isp_drc_maxval1 o_isp_drc_maxval1;
    o_isp_drc_maxval1.u32 = be_reg->isp_drc_maxval1.u32;
    o_isp_drc_maxval1.bits.isp_drc_maxval7 = uisp_drc_maxval7;
    be_reg->isp_drc_maxval1.u32 = o_isp_drc_maxval1.u32;
}

/*  description : set the value of the member isp_drc_flt_cfg.isp_drc_flt_spa_fine */
/*  input       : unsigned int uisp_drc_flt_spa_fine: 4 bits */
static __inline td_void isp_drc_flt_spa_fine_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_flt_spa_fine)
{
    u_isp_drc_flt_cfg o_isp_drc_flt_cfg;
    o_isp_drc_flt_cfg.u32 = be_reg->isp_drc_flt_cfg.u32;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_spa_fine = uisp_drc_flt_spa_fine;
    be_reg->isp_drc_flt_cfg.u32 = o_isp_drc_flt_cfg.u32;
}

/*  description : set the value of the member isp_drc_flt_cfg.isp_drc_flt_rng_fine */
/*  input       : unsigned int uisp_drc_flt_rng_fine: 4 bits */
static __inline td_void isp_drc_flt_rng_fine_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_flt_rng_fine)
{
    u_isp_drc_flt_cfg o_isp_drc_flt_cfg;
    o_isp_drc_flt_cfg.u32 = be_reg->isp_drc_flt_cfg.u32;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_rng_fine = uisp_drc_flt_rng_fine;
    be_reg->isp_drc_flt_cfg.u32 = o_isp_drc_flt_cfg.u32;
}

/*  description : set the value of the member isp_drc_detail_sub_factor.isp_drc_detail_sub_factor */
/*  input       : unsigned int uisp_drc_detail_sub_factor: 5 bits */
static __inline td_void isp_drc_detail_sub_factor_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_detail_sub_factor)
{
    u_isp_drc_detail_sub_factor o_isp_drc_detail_sub_factor;
    o_isp_drc_detail_sub_factor.u32 = be_reg->isp_drc_detail_sub_factor.u32;
    o_isp_drc_detail_sub_factor.bits.isp_drc_detail_sub_factor = uisp_drc_detail_sub_factor;
    be_reg->isp_drc_detail_sub_factor.u32 = o_isp_drc_detail_sub_factor.u32;
}

/*  description : set the value of the member isp_drc_wgt_box_tri_sel.isp_drc_wgt_box_tri_sel */
/*  input       : unsigned int uisp_drc_wgt_box_tri_sel: 1 bits */
static __inline td_void isp_drc_wgt_box_tri_sel_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_wgt_box_tri_sel)
{
    u_isp_drc_wgt_box_tri_sel o_isp_drc_wgt_box_tri_sel;
    o_isp_drc_wgt_box_tri_sel.u32 = be_reg->isp_drc_wgt_box_tri_sel.u32;
    o_isp_drc_wgt_box_tri_sel.bits.isp_drc_wgt_box_tri_sel = uisp_drc_wgt_box_tri_sel;
    be_reg->isp_drc_wgt_box_tri_sel.u32 = o_isp_drc_wgt_box_tri_sel.u32;
}

/*  description : set the value of the member isp_drc_blk_wgt_init.isp_drc_blk_wgt_init */
/*  input       : unsigned int uisp_drc_blk_wgt_init: 15 bits */
static __inline td_void isp_drc_blk_wgt_init_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_blk_wgt_init)
{
    u_isp_drc_blk_wgt_init o_isp_drc_blk_wgt_init;
    o_isp_drc_blk_wgt_init.u32 = be_reg->isp_drc_blk_wgt_init.u32;
    o_isp_drc_blk_wgt_init.bits.isp_drc_blk_wgt_init = uisp_drc_blk_wgt_init;
    be_reg->isp_drc_blk_wgt_init.u32 = o_isp_drc_blk_wgt_init.u32;
}

/*  description : set the value of the member isp_drc_shp_cfg.isp_drc_shp_log */
/*  input       : unsigned int uisp_drc_shp_log: 4 bits */
static __inline td_void isp_drc_shp_log_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_shp_log)
{
    u_isp_drc_shp_cfg o_isp_drc_shp_cfg;
    o_isp_drc_shp_cfg.u32 = be_reg->isp_drc_shp_cfg.u32;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_log = uisp_drc_shp_log;
    be_reg->isp_drc_shp_cfg.u32 = o_isp_drc_shp_cfg.u32;
}

/*  description : set the value of the member isp_drc_shp_cfg.isp_drc_shp_exp */
/*  input       : unsigned int uisp_drc_shp_exp: 4 bits */
static __inline td_void isp_drc_shp_exp_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_shp_exp)
{
    u_isp_drc_shp_cfg o_isp_drc_shp_cfg;
    o_isp_drc_shp_cfg.u32 = be_reg->isp_drc_shp_cfg.u32;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_exp = uisp_drc_shp_exp;
    be_reg->isp_drc_shp_cfg.u32 = o_isp_drc_shp_cfg.u32;
}

/*  description : set the value of the member isp_drc_div_denom_log.isp_drc_div_denom_log */
/*  input       : unsigned int uisp_drc_div_denom_log: 18 bits */
static __inline td_void isp_drc_div_denom_log_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_div_denom_log)
{
    u_isp_drc_div_denom_log o_isp_drc_div_denom_log;
    o_isp_drc_div_denom_log.u32 = be_reg->isp_drc_div_denom_log.u32;
    o_isp_drc_div_denom_log.bits.isp_drc_div_denom_log = uisp_drc_div_denom_log;
    be_reg->isp_drc_div_denom_log.u32 = o_isp_drc_div_denom_log.u32;
}

/*  description : set the value of the member isp_drc_denom_exp.isp_drc_denom_exp */
/*  input       : unsigned int uisp_drc_denom_exp: 21 bits */
static __inline td_void isp_drc_denom_exp_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_denom_exp)
{
    u_isp_drc_denom_exp o_isp_drc_denom_exp;
    o_isp_drc_denom_exp.u32 = be_reg->isp_drc_denom_exp.u32;
    o_isp_drc_denom_exp.bits.isp_drc_denom_exp = uisp_drc_denom_exp;
    be_reg->isp_drc_denom_exp.u32 = o_isp_drc_denom_exp.u32;
}

/*  description : set the value of the member isp_drc_debug_info.isp_drc_debug_info */
/*  input       : unsigned int uisp_drc_debug_info: 32 bits */
static __inline td_void isp_drc_debug_info_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_debug_info)
{
    be_reg->isp_drc_debug_info.u32 = uisp_drc_debug_info;
}


/*  be reg in viproc */
#define VIPROC_REG_START
static __inline td_void viproc_reg_list(td_void)
{
}


static __inline td_void isp_be_manual_reg_up_write(isp_viproc_reg_type *vi_proc_reg, td_u32 umanual_reg_up)
{
    u_isp_viproc_regup o_viproc_regup;
    o_viproc_regup.u32 = vi_proc_reg->viproc_regup.u32;
    o_viproc_regup.bits.manual_reg_up = umanual_reg_up;
    vi_proc_reg->viproc_regup.u32 = o_viproc_regup.u32;
}

/*  description   set the value of the member viproc_ispbe_regup.ispbe_reg_up */
/*  input         unsigned int uispbe_reg_up  1 bits */
static __inline td_void isp_be_reg_up_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uispbe_reg_up)
{
    u_isp_viproc_ispbe_regup o_viproc_ispbe_regup;
    o_viproc_ispbe_regup.u32 = vi_proc_reg->viproc_ispbe_regup.u32;
    o_viproc_ispbe_regup.bits.ispbe_reg_up = uispbe_reg_up;
    vi_proc_reg->viproc_ispbe_regup.u32 = o_viproc_ispbe_regup.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_wdrsplit1_en */
/*  input       : unsigned int uisp_wdrsplit1_en: 1 bits */
static __inline td_void isp_wdrsplit1_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_wdrsplit1_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_wdrsplit1_en = uisp_wdrsplit1_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_fpn_en */
/*  input       : unsigned int uisp_fpn_en: 1 bits */
static __inline td_void isp_fpn_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_fpn_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_fpn_en = uisp_fpn_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

static __inline td_void isp_fpn_mode_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_fpn_mode)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_fpn_mode = uisp_fpn_mode;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

static __inline td_void isp_dpc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dpc_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dpc_en = uisp_dpc_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_dpc_stat_en */
/*  input         unsigned int uisp_dpc_stat_en  1 bits */
static __inline td_void isp_dpc_stat_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dpc_stat_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dpc_stat_en = uisp_dpc_stat_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_ge_en */
/*  input         unsigned int uisp_ge_en  1 bits */
static __inline td_void isp_ge_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ge_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_ge_en = uisp_ge_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_4dg_en */
/*  input         unsigned int uisp_4dg_en  1 bits */
static __inline td_void isp_4dg_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_4dg_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_4dg_en = uisp_4dg_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_flicker_en */
/*  input         unsigned int uisp_flicker_en  1 bits */
static __inline td_void isp_flicker_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_flicker_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_flicker_en = uisp_flicker_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_wdr_en */
/*  input         unsigned int uisp_wdr_en  1 bits */
static __inline td_void isp_wdr_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_wdr_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_wdr_en = uisp_wdr_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_expander_en */
/*  input         unsigned int uisp_expander_en  1 bits */
static __inline td_void isp_expander_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_expander_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_expander_en = uisp_expander_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_bcom_en */
/*  input         unsigned int uisp_bcom_en  1 bits */
static __inline td_void isp_bcom_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_bcom_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_bcom_en = uisp_bcom_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_bnr_en */
/*  input       : unsigned int uisp_bnr_en: 1 bits */
static __inline td_void isp_bnr_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_bnr_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_bnr_en = uisp_bnr_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_bdec_en */
/*  input         unsigned int uisp_bdec_en  1 bits */
static __inline td_void isp_bdec_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_bdec_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_bdec_en = uisp_bdec_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_lsc_en */
/*  input         unsigned int uisp_lsc_en  1 bits */
static __inline td_void isp_lsc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_lsc_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_lsc_en = uisp_lsc_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_dg_en */
/*  input         unsigned int uisp_dg_en  1 bits */
static __inline td_void isp_dg_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dg_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dg_en = uisp_dg_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_af_en */
/*  input         unsigned int uisp_af_en  1 bits */
static __inline td_void isp_af_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_af_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_af_en = uisp_af_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_awb_en */
/*  input         unsigned int uisp_awb_en  1 bits */
static __inline td_void isp_awb_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_awb_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_awb_en = uisp_awb_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_ae_en */
/*  input         unsigned int uisp_ae_en  1 bits */
static __inline td_void isp_ae_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ae_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_ae_en = uisp_ae_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_dis_en */
/*  input       : unsigned int uisp_dis_en: 1 bits */
static __inline td_void isp_dis_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dis_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dis_en = uisp_dis_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_wb_en */
/*  input         unsigned int uisp_wb_en  1 bits */
static __inline td_void isp_wb_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_wb_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_wb_en = uisp_wb_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description   set the value of the member viproc_ispbe_ctrl0.isp_blc_en */
/*  input         unsigned int uisp_blc_en  1 bits */
static __inline td_void isp_blc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_blc_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_blc_en = uisp_blc_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_pregamma_en */
/*  input       : unsigned int uisp_pregamma_en: 1 bits */
static __inline td_void isp_pregamma_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_pregamma_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_pregamma_en = uisp_pregamma_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_drc_en */
/*  input       : unsigned int uisp_drc_en: 1 bits */
static __inline td_void isp_drc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_drc_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_drc_en = uisp_drc_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_la_en */
/*  input       : unsigned int uisp_la_en: 1 bits */
static __inline td_void isp_la_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_la_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_la_en = uisp_la_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_sqrt_en */
/*  input       : unsigned int uisp_sqrt_en: 1 bits */
static __inline td_void isp_sqrt_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sqrt_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_sqrt_en = uisp_sqrt_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_dmnr_vhdm_en */
/*  input       : unsigned int uisp_dmnr_vhdm_en: 1 bits */
static __inline td_void isp_dmnr_vhdm_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dmnr_vhdm_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dmnr_vhdm_en = uisp_dmnr_vhdm_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_dmnr_nddm_en */
/*  input       : unsigned int uisp_dmnr_nddm_en: 1 bits */
static __inline td_void isp_dmnr_nddm_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dmnr_nddm_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_dmnr_nddm_en = uisp_dmnr_nddm_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_loglut_en */
/*  input       : unsigned int uisp_loglut_en: 1 bits */
static __inline td_void isp_loglut_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_loglut_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_loglut_en = uisp_loglut_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_rlsc_en */
/*  input       : unsigned int uisp_rlsc_en: 1 bits */
static __inline td_void isp_rlsc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_rlsc_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_rlsc_en = uisp_rlsc_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl0.isp_rgbir_en */
/*  input       : unsigned int uisp_rgbir_en: 1 bits */
static __inline td_void isp_rgbir_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_rgbir_en)
{
    u_isp_viproc_ispbe_ctrl0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = vi_proc_reg->viproc_ispbe_ctrl0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_rgbir_en = uisp_rgbir_en;
    vi_proc_reg->viproc_ispbe_ctrl0.u32 = o_viproc_ispbe_ctrl0.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_sq_en */
/*  input       : unsigned int uisp_sq_en: 1 bits */
static __inline td_void isp_sq_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sq_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sq_en = uisp_sq_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_cc_en */
/*  input       : unsigned int uisp_cc_en: 1 bits */
static __inline td_void isp_cc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_cc_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_cc_en = uisp_cc_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_clut_en */
/*  input       : unsigned int uisp_clut_en: 1 bits */
static __inline td_void isp_clut_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_clut_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_clut_en = uisp_clut_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_sqrt1_en */
/*  input       : unsigned int uisp_sqrt1_en: 1 bits */
static __inline td_void isp_sqrt1_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sqrt1_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sqrt1_en = uisp_sqrt1_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_gamma_en */
/*  input       : unsigned int uisp_gamma_en: 1 bits */
static __inline td_void isp_gamma_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_gamma_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_gamma_en = uisp_gamma_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_dehaze_en */
/*  input       : unsigned int uisp_dehaze_en: 1 bits */
static __inline td_void isp_dehaze_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dehaze_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_dehaze_en = uisp_dehaze_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_csc_en */
/*  input       : unsigned int uisp_csc_en: 1 bits */
static __inline td_void isp_csc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_csc_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_csc_en = uisp_csc_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_sumy_en */
/*  input       : unsigned int uisp_sumy_en: 1 bits */
static __inline td_void isp_sumy_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sumy_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sumy_en = uisp_sumy_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_ldci_en */
/*  input       : unsigned int uisp_ldci_en: 1 bits */
static __inline td_void isp_ldci_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ldci_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_ldci_en = uisp_ldci_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_ca_en */
/*  input       : unsigned int uisp_ca_en: 1 bits */
static __inline td_void isp_ca_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ca_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_ca_en = uisp_ca_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_split_en */
/*  input       : unsigned int uisp_split_en: 1 bits */
static __inline td_void isp_split_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_split_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_split_en = uisp_split_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_sharpen_en */
/*  input       : unsigned int uisp_sharpen_en: 1 bits */
static __inline td_void isp_sharpen_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sharpen_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_sharpen_en = uisp_sharpen_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_hcds_en */
/*  input       : unsigned int uisp_hcds_en: 1 bits */
static __inline td_void isp_hcds_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_hcds_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_hcds_en = uisp_hcds_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_hlc_en */
/*  input       : unsigned int uisp_hlc_en: 1 bits */
static __inline td_void isp_hlc_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_hlc_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_hlc_en = uisp_hlc_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_de_en */
/*  input       : unsigned int uisp_de_en: 1 bits */
static __inline td_void isp_de_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_de_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_de_en = uisp_de_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_vcds_en */
/*  input       : unsigned int uisp_vcds_en: 1 bits */
static __inline td_void isp_vcds_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_vcds_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_vcds_en = uisp_vcds_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_ctrl1.isp_nry0_en */
/*  input       : unsigned int uisp_nry0_en: 1 bits */
static __inline td_void isp_nry0_en_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_nry0_en)
{
    u_isp_viproc_ispbe_ctrl1 o_viproc_ispbe_ctrl1;
    o_viproc_ispbe_ctrl1.u32 = vi_proc_reg->viproc_ispbe_ctrl1.u32;
    o_viproc_ispbe_ctrl1.bits.isp_nry0_en = uisp_nry0_en;
    vi_proc_reg->viproc_ispbe_ctrl1.u32 = o_viproc_ispbe_ctrl1.u32;
}

/*  description : set the value of the member viproc_ispbe_cfg.isp_be_rggb_cfg */
/*  input       : unsigned int uisp_be_rggb_cfg: 2 bits */
static __inline td_void isp_be_rggb_cfg_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_be_rggb_cfg)
{
    u_isp_viproc_ispbe_cfg o_viproc_ispbe_cfg;
    o_viproc_ispbe_cfg.u32 = vi_proc_reg->viproc_ispbe_cfg.u32;
    o_viproc_ispbe_cfg.bits.isp_be_rggb_cfg = uisp_be_rggb_cfg;
    vi_proc_reg->viproc_ispbe_cfg.u32 = o_viproc_ispbe_cfg.u32;
}

/*  description : set the value of the member viproc_ispbe_cfg.isp_be_timing_cfg */
/*  input       : unsigned int uisp_be_timing_cfg: 16 bits */
static __inline td_void isp_be_timing_cfg_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_be_timing_cfg)
{
    u_isp_viproc_ispbe_cfg o_viproc_ispbe_cfg;
    o_viproc_ispbe_cfg.u32 = vi_proc_reg->viproc_ispbe_cfg.u32;
    o_viproc_ispbe_cfg.bits.isp_be_timing_cfg = uisp_be_timing_cfg;
    vi_proc_reg->viproc_ispbe_cfg.u32 = o_viproc_ispbe_cfg.u32;
}

#define OT_ISP_BNR_LUT_WIDTH_WORD_DEFAULT             194
#define OT_ISP_LSC_LUT_WIDTH_WORD_DEFAULT             627
#define OT_ISP_CC_LUT_WIDTH_WORD_DEFAULT              34
#define OT_ISP_GAMMA_LUT_WIDTH_WORD_DEFAULT           65
#define OT_ISP_DEHAZE_LUT_WIDTH_WORD_DEFAULT          160
#define OT_ISP_LDCI_LUT_WIDTH_WORD_DEFAULT            65
#define OT_ISP_CA_LUT_WIDTH_WORD_DEFAULT              136
#define OT_ISP_SHARPEN_LUT_WIDTH_WORD_DEFAULT         32

/*  description : set the value of the member viproc_para_drc_addr_low.para_drc_addr_low */
/*  input       : unsigned int upara_drc_addr_low: 32 bits */
static __inline td_void viproc_para_drc_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 upara_drc_addr_low)
{
    vi_proc_reg->viproc_para_drc_addr_low.u32 = upara_drc_addr_low;
}


/*  description : set the value of the member viproc_para_dci_addr_low.para_dci_addr_low */
/*  input       : unsigned int upara_dci_addr_low: 32 bits */
static __inline td_void viproc_para_dci_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 upara_dci_addr_low)
{
    vi_proc_reg->viproc_para_dci_addr_low.u32 = upara_dci_addr_low;
}


/*  description : set the value of the member viproc_out_para_drc_addr_low.out_para_drc_addr_low */
/*  input       : unsigned int uout_para_drc_addr_low: 32 bits */
static __inline td_void out_para_drc_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uout_para_drc_addr_low)
{
    vi_proc_reg->viproc_out_para_drc_addr_low.u32 = uout_para_drc_addr_low;
}


/*  description : set the value of the member viproc_out_para_dci_addr_low.out_para_dci_addr_low */
/*  input       : unsigned int uout_para_dci_addr_low: 32 bits */
static __inline td_void out_para_dci_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uout_para_dci_addr_low)
{
    vi_proc_reg->viproc_out_para_dci_addr_low.u32 = uout_para_dci_addr_low;
}


/*  description : set the value of the member viproc_isp_ae_hist_addr_low.isp_ae_hist_addr_low */
/*  input       : unsigned int uisp_ae_hist_addr_low: 32 bits */
static __inline td_void isp_ae_hist_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ae_hist_addr_low)
{
    vi_proc_reg->ae_hist_addr_low.u32 = uisp_ae_hist_addr_low;
}


/*  description : set the value of the member viproc_isp_ae_aver_r_gr_addr_low.isp_ae_aver_r_gr_addr_low */
/*  input       : unsigned int uisp_ae_aver_r_gr_addr_low: 32 bits */
static __inline td_void isp_ae_aver_r_gr_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ae_aver_r_gr_addr_low)
{
    vi_proc_reg->ae_aver_r_gr_addr_low.u32 = uisp_ae_aver_r_gr_addr_low;
}


/*  description : set the value of the member viproc_isp_ae_aver_gb_b_addr_low.isp_ae_aver_gb_b_addr_low */
/*  input       : unsigned int uisp_ae_aver_gb_b_addr_low: 32 bits */
static __inline td_void isp_ae_aver_gb_b_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ae_aver_gb_b_addr_low)
{
    vi_proc_reg->ae_aver_gb_b_addr_low.u32 = uisp_ae_aver_gb_b_addr_low;
}


/*  description : set the value of the member viproc_isp_awb_stat_addr_low.isp_awb_stat_addr_low */
/*  input       : unsigned int uisp_awb_stat_addr_low: 32 bits */
static __inline td_void isp_awb_stat_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_awb_stat_addr_low)
{
    vi_proc_reg->awb_stat_addr_low.u32 = uisp_awb_stat_addr_low;
}


/*  description : set the value of the member viproc_isp_af_stat_addr_low.isp_af_stat_addr_low */
/*  input       : unsigned int uisp_af_stat_addr_low: 32 bits */
static __inline td_void isp_af_stat_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_af_stat_addr_low)
{
    vi_proc_reg->af_stat_addr_low.u32 = uisp_af_stat_addr_low;
}


/*  description : set the value of the member viproc_isp_dis_h_stat_addr_low.isp_dis_h_stat_addr_low */
/*  input       : unsigned int uisp_dis_h_stat_addr_low: 32 bits */
static __inline td_void isp_dis_h_stat_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dis_h_stat_addr_low)
{
    vi_proc_reg->dis_h_stat_addr_low.u32 = uisp_dis_h_stat_addr_low;
}


/*  description : set the value of the member viproc_isp_dis_v_stat_addr_low.isp_dis_v_stat_addr_low */
/*  input       : unsigned int uisp_dis_v_stat_addr_low: 32 bits */
static __inline td_void isp_dis_v_stat_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dis_v_stat_addr_low)
{
    vi_proc_reg->dis_v_stat_addr_low.u32 = uisp_dis_v_stat_addr_low;
}


/*  description : set the value of the member viproc_isp_la_aver_addr_low.isp_la_aver_addr_low */
/*  input       : unsigned int uisp_la_aver_addr_low: 32 bits */
static __inline td_void isp_la_aver_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_la_aver_addr_low)
{
    vi_proc_reg->la_aver_addr_low.u32 = uisp_la_aver_addr_low;
}


/*  description : set the value of the member viproc_isp_ge_aver0_addr_low.isp_ge_aver0_addr_low */
/*  input       : unsigned int uisp_ge_aver0_addr_low: 32 bits */
static __inline td_void isp_ge_aver0_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ge_aver0_addr_low)
{
    vi_proc_reg->ge_aver0_addr_low.u32 = uisp_ge_aver0_addr_low;
}


/*  description : set the value of the member viproc_isp_ge_aver1_addr_low.isp_ge_aver1_addr_low */
/*  input       : unsigned int uisp_ge_aver1_addr_low: 32 bits */
static __inline td_void isp_ge_aver1_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ge_aver1_addr_low)
{
    vi_proc_reg->ge_aver1_addr_low.u32 = uisp_ge_aver1_addr_low;
}


/*  description : set the value of the member viproc_isp_dehaze_min_stat_addr_low.isp_dehaze_min_stat_addr_low */
/*  input       : unsigned int uisp_dehaze_min_stat_addr_low: 32 bits */
static __inline td_void isp_dehaze_min_stat_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dehaze_min_stat_addr_low)
{
    vi_proc_reg->dehaze_min_stat_addr_low.u32 = uisp_dehaze_min_stat_addr_low;
}


/*  description : set the value of the member viproc_isp_dehaze_max_stat_addr_low.isp_dehaze_max_stat_addr_low */
/*  input       : unsigned int uisp_dehaze_max_stat_addr_low: 32 bits */
static __inline td_void isp_dehaze_max_stat_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dehaze_max_stat_addr_low)
{
    vi_proc_reg->dehaze_max_stat_addr_low.u32 = uisp_dehaze_max_stat_addr_low;
}


/*  description : set the value of the member viproc_isp_flick_gmean_addr_low.isp_flick_gmean_addr_low */
/*  input       : unsigned int uisp_flick_gmean_addr_low: 32 bits */
static __inline td_void isp_flick_gmean_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_flick_gmean_addr_low)
{
    vi_proc_reg->flick_gmean_addr_low.u32 = uisp_flick_gmean_addr_low;
}


/*  description : set the value of the member viproc_isp_ldci_lpf_addr_low.isp_ldci_lpf_addr_low */
/*  input       : unsigned int uisp_ldci_lpf_addr_low: 32 bits */
static __inline td_void isp_ldci_lpf_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ldci_lpf_addr_low)
{
    vi_proc_reg->ldci_lpf_addr_low.u32 = uisp_ldci_lpf_addr_low;
}


/*  description : set the value of the member viproc_out1_finish_line.out1_finish_line */
/*  input       : unsigned int uout1_finish_line: 16 bits */
static __inline td_void out1_finish_line_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uout1_finish_line)
{
    u_isp_viproc_out1_finish_line o_viproc_out1_finish_line;
    o_viproc_out1_finish_line.u32 = vi_proc_reg->viproc_out1_finish_line.u32;
    o_viproc_out1_finish_line.bits.out1_finish_line = uout1_finish_line;
    vi_proc_reg->viproc_out1_finish_line.u32 = o_viproc_out1_finish_line.u32;
}

/*  description : set the value of the member viproc_isp_dcg_lut_addr_low.isp_dcg_lut_addr_low */
/*  input       : unsigned int uisp_dcg_lut_addr_low: 32 bits */
static __inline td_void isp_dcg_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dcg_lut_addr_low)
{
    vi_proc_reg->dcg_lut_addr_low.u32 = uisp_dcg_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_dcg_lut_width_word.isp_dcg_lut_width_word */
/*  input       : unsigned int uisp_dcg_lut_width_word: 16 bits */
static __inline td_void isp_dcg_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dcg_lut_width_word)
{
    u_isp_viproc_isp_dcg_lut_width_word o_viproc_isp_dcg_lut_width_word;
    o_viproc_isp_dcg_lut_width_word.u32 = vi_proc_reg->dcg_lut_width_word.u32;
    o_viproc_isp_dcg_lut_width_word.bits.isp_dcg_lut_width_word = uisp_dcg_lut_width_word;
    vi_proc_reg->dcg_lut_width_word.u32 = o_viproc_isp_dcg_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_bnr_lut_addr_low.isp_bnr_lut_addr_low */
/*  input       : unsigned int uisp_bnr_lut_addr_low: 32 bits */
static __inline td_void isp_bnr_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_bnr_lut_addr_low)
{
    vi_proc_reg->bnr_lut_addr_low.u32 = uisp_bnr_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_bnr_lut_width_word.isp_bnr_lut_width_word */
/*  input       : unsigned int uisp_bnr_lut_width_word: 16 bits */
static __inline td_void isp_bnr_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_bnr_lut_width_word)
{
    u_isp_viproc_isp_bnr_lut_width_word o_viproc_isp_bnr_lut_width_word;
    o_viproc_isp_bnr_lut_width_word.u32 = vi_proc_reg->bnr_lut_width_word.u32;
    o_viproc_isp_bnr_lut_width_word.bits.isp_bnr_lut_width_word = uisp_bnr_lut_width_word;
    vi_proc_reg->bnr_lut_width_word.u32 = o_viproc_isp_bnr_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_lsc_lut_addr_low.isp_lsc_lut_addr_low */
/*  input       : unsigned int uisp_lsc_lut_addr_low: 32 bits */
static __inline td_void isp_lsc_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_lsc_lut_addr_low)
{
    vi_proc_reg->lsc_lut_addr_low.u32 = uisp_lsc_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_lsc_lut_width_word.isp_lsc_lut_width_word */
/*  input       : unsigned int uisp_lsc_lut_width_word: 16 bits */
static __inline td_void isp_lsc_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_lsc_lut_width_word)
{
    u_isp_viproc_isp_lsc_lut_width_word o_viproc_isp_lsc_lut_width_word;
    o_viproc_isp_lsc_lut_width_word.u32 = vi_proc_reg->lsc_lut_width_word.u32;
    o_viproc_isp_lsc_lut_width_word.bits.isp_lsc_lut_width_word = uisp_lsc_lut_width_word;
    vi_proc_reg->lsc_lut_width_word.u32 = o_viproc_isp_lsc_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_clut_lut_addr_low.isp_clut_lut_addr_low */
/*  input       : unsigned int uisp_clut_lut_addr_low: 32 bits */
static __inline td_void isp_clut_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_clut_lut_addr_low)
{
    vi_proc_reg->clut_lut_addr_low.u32 = uisp_clut_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_clut_lut_width_word.isp_clut_lut_width_word */
/*  input       : unsigned int uisp_clut_lut_width_word: 16 bits */
static __inline td_void isp_clut_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_clut_lut_width_word)
{
    u_isp_viproc_isp_clut_lut_width_word o_viproc_isp_clut_lut_width_word;
    o_viproc_isp_clut_lut_width_word.u32 = vi_proc_reg->clut_lut_width_word.u32;
    o_viproc_isp_clut_lut_width_word.bits.isp_clut_lut_width_word = uisp_clut_lut_width_word;
    vi_proc_reg->clut_lut_width_word.u32 = o_viproc_isp_clut_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_gamma_lut_addr_low.isp_gamma_lut_addr_low */
/*  input       : unsigned int uisp_gamma_lut_addr_low: 32 bits */
static __inline td_void isp_gamma_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_gamma_lut_addr_low)
{
    vi_proc_reg->gamma_lut_addr_low.u32 = uisp_gamma_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_gamma_lut_width_word.isp_gamma_lut_width_word */
/*  input       : unsigned int uisp_gamma_lut_width_word: 16 bits */
static __inline td_void isp_gamma_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_gamma_lut_width_word)
{
    u_isp_viproc_isp_gamma_lut_width_word o_viproc_isp_gamma_lut_width_word;
    o_viproc_isp_gamma_lut_width_word.u32 = vi_proc_reg->gamma_lut_width_word.u32;
    o_viproc_isp_gamma_lut_width_word.bits.isp_gamma_lut_width_word = uisp_gamma_lut_width_word;
    vi_proc_reg->gamma_lut_width_word.u32 = o_viproc_isp_gamma_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_dehaze_lut_addr_low.isp_dehaze_lut_addr_low */
/*  input       : unsigned int uisp_dehaze_lut_addr_low: 32 bits */
static __inline td_void isp_dehaze_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dehaze_lut_addr_low)
{
    vi_proc_reg->dehaze_lut_addr_low.u32 = uisp_dehaze_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_dehaze_lut_width_word.isp_dehaze_lut_width_word */
/*  input       : unsigned int uisp_dehaze_lut_width_word: 16 bits */
static __inline td_void isp_dehaze_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_dehaze_lut_width_word)
{
    u_isp_viproc_isp_dehaze_lut_width_word o_viproc_isp_dehaze_lut_width_word;
    o_viproc_isp_dehaze_lut_width_word.u32 = vi_proc_reg->dehaze_lut_width_word.u32;
    o_viproc_isp_dehaze_lut_width_word.bits.isp_dehaze_lut_width_word = uisp_dehaze_lut_width_word;
    vi_proc_reg->dehaze_lut_width_word.u32 = o_viproc_isp_dehaze_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_ldci_lut_addr_low.isp_ldci_lut_addr_low */
/*  input       : unsigned int uisp_ldci_lut_addr_low: 32 bits */
static __inline td_void isp_ldci_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ldci_lut_addr_low)
{
    vi_proc_reg->ldci_lut_addr_low.u32 = uisp_ldci_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_ldci_lut_width_word.isp_ldci_lut_width_word */
/*  input       : unsigned int uisp_ldci_lut_width_word: 16 bits */
static __inline td_void isp_ldci_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ldci_lut_width_word)
{
    u_isp_viproc_isp_ldci_lut_width_word o_viproc_isp_ldci_lut_width_word;
    o_viproc_isp_ldci_lut_width_word.u32 = vi_proc_reg->ldci_lut_width_word.u32;
    o_viproc_isp_ldci_lut_width_word.bits.isp_ldci_lut_width_word = uisp_ldci_lut_width_word;
    vi_proc_reg->ldci_lut_width_word.u32 = o_viproc_isp_ldci_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_ca_lut_addr_low.isp_ca_lut_addr_low */
/*  input       : unsigned int uisp_ca_lut_addr_low: 32 bits */
static __inline td_void isp_ca_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ca_lut_addr_low)
{
    vi_proc_reg->ca_lut_addr_low.u32 = uisp_ca_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_ca_lut_width_word.isp_ca_lut_width_word */
/*  input       : unsigned int uisp_ca_lut_width_word: 16 bits */
static __inline td_void isp_ca_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_ca_lut_width_word)
{
    u_isp_viproc_isp_ca_lut_width_word o_viproc_isp_ca_lut_width_word;
    o_viproc_isp_ca_lut_width_word.u32 = vi_proc_reg->ca_lut_width_word.u32;
    o_viproc_isp_ca_lut_width_word.bits.isp_ca_lut_width_word = uisp_ca_lut_width_word;
    vi_proc_reg->ca_lut_width_word.u32 = o_viproc_isp_ca_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_sharpen_lut_addr_low.isp_sharpen_lut_addr_low */
/*  input       : unsigned int uisp_sharpen_lut_addr_low: 32 bits */
static __inline td_void isp_sharpen_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sharpen_lut_addr_low)
{
    vi_proc_reg->sharpen_lut_addr_low.u32 = uisp_sharpen_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_sharpen_lut_width_word.isp_sharpen_lut_width_word */
/*  input       : unsigned int uisp_sharpen_lut_width_word: 16 bits */
static __inline td_void isp_sharpen_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_sharpen_lut_width_word)
{
    u_isp_viproc_isp_sharpen_lut_width_word o_viproc_isp_sharpen_lut_width_word;
    o_viproc_isp_sharpen_lut_width_word.u32 = vi_proc_reg->sharpen_lut_width_word.u32;
    o_viproc_isp_sharpen_lut_width_word.bits.isp_sharpen_lut_width_word = uisp_sharpen_lut_width_word;
    vi_proc_reg->sharpen_lut_width_word.u32 = o_viproc_isp_sharpen_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_pregamma_lut_addr_low.isp_pregamma_lut_addr_low */
/*  input       : unsigned int uisp_pregamma_lut_addr_low: 32 bits */
static __inline td_void isp_pregamma_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_pregamma_lut_addr_low)
{
    vi_proc_reg->pregamma_lut_addr_low.u32 = uisp_pregamma_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_pregamma_lut_width_word.isp_pregamma_lut_width_word */
/*  input       : unsigned int uisp_pregamma_lut_width_word: 16 bits */
static __inline td_void isp_pregamma_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_pregamma_lut_width_word)
{
    u_isp_viproc_isp_pregamma_lut_width_word o_viproc_isp_pregamma_lut_width_word;
    o_viproc_isp_pregamma_lut_width_word.u32 = vi_proc_reg->pregamma_lut_width_word.u32;
    o_viproc_isp_pregamma_lut_width_word.bits.isp_pregamma_lut_width_word = uisp_pregamma_lut_width_word;
    vi_proc_reg->pregamma_lut_width_word.u32 = o_viproc_isp_pregamma_lut_width_word.u32;
}

/*  description : set the value of the member viproc_isp_cc_lut_addr_low.isp_cc_lut_addr_low */
/*  input       : unsigned int uisp_cc_lut_addr_low: 32 bits */
static __inline td_void isp_cc_lut_addr_low_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_cc_lut_addr_low)
{
    vi_proc_reg->cc_lut_addr_low.u32 = uisp_cc_lut_addr_low;
}


/*  description : set the value of the member viproc_isp_cc_lut_width_word.isp_cc_lut_width_word */
/*  input       : unsigned int uisp_cc_lut_width_word: 16 bits */
static __inline td_void isp_cc_lut_width_word_write(isp_viproc_reg_type *vi_proc_reg, td_u32 uisp_cc_lut_width_word)
{
    u_isp_viproc_isp_cc_lut_width_word o_viproc_isp_cc_lut_width_word;
    o_viproc_isp_cc_lut_width_word.u32 = vi_proc_reg->cc_lut_width_word.u32;
    o_viproc_isp_cc_lut_width_word.bits.isp_cc_lut_width_word = uisp_cc_lut_width_word;
    vi_proc_reg->cc_lut_width_word.u32 = o_viproc_isp_cc_lut_width_word.u32;
}

#define VIPROC_REG_END


/* post be */
static __inline td_void isp_af_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_af_update_index)
{
    be_reg->reserved_0[0] = uisp_af_update_index; /* index0  af_update_index */
}

static __inline td_u32 isp_af_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[0]; /* index0  af_update_index */
}

static __inline td_void isp_awb_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_awb_update_index)
{
    be_reg->reserved_0[1] = uisp_awb_update_index; /* index1  awb_update_index */
}

static __inline td_u32 isp_awb_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[1]; /* index1  awb_update_index */
}

static __inline td_void isp_sharpen_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_sharpen_update_index)
{
    be_reg->reserved_0[2] = uisp_sharpen_update_index; /* index2  sharpen_update_index */
}

static __inline td_u32 isp_sharpen_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[2]; /* index2  sharpen_update_index */
}

static __inline td_void isp_lcac_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_lcac_update_index)
{
    be_reg->reserved_0[3] = uisp_lcac_update_index; /* index3  lcac_update_index */
}

static __inline td_u32 isp_lcac_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[3]; /* index3  lcac_update_index */
}

static __inline td_void isp_gcac_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_gcac_update_index)
{
    be_reg->reserved_0[4] = uisp_gcac_update_index; /* index4  gcac_update_index */
}

static __inline td_u32 isp_gcac_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[4]; /* index4  gcac_update_index */
}

static __inline td_void isp_lsc_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_lsc_update_index)
{
    be_reg->reserved_0[5] = uisp_lsc_update_index; /* index5  lsc_update_index */
}

static __inline td_u32 isp_lsc_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[5]; /* index5  lsc_update_index */
}

static __inline td_void isp_gamma_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_gamma_update_index)
{
    be_reg->reserved_0[6] = uisp_gamma_update_index; /* index6  gamma_update_index */
}

static __inline td_u32 isp_gamma_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[6]; /* index6  gamma_update_index */
}

static __inline td_void isp_ca_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_ca_update_index)
{
    be_reg->reserved_0[7] = uisp_ca_update_index; /* index7  ca_update_index */
}

static __inline td_u32 isp_ca_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[7]; /* index7  ca_update_index */
}

static __inline td_void isp_drc_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_drc_update_index)
{
    be_reg->reserved_0[8] = uisp_drc_update_index; /* index8  drc_update_index */
}

static __inline td_u32 isp_drc_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[8]; /* index8  drc_update_index */
}

static __inline td_void isp_rlsc_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_rlsc_update_index)
{
    be_reg->reserved_0[9] = uisp_rlsc_update_index; /* index9  rlsc_update_index */
}

static __inline td_u32 isp_rlsc_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[9]; /* index9  rlsc_update_index */
}

static __inline td_void isp_pregamma_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_pregamma_update_index)
{
    be_reg->reserved_0[10] = uisp_pregamma_update_index; /* index10  pregamma_update_index */
}

static __inline td_u32 isp_pregamma_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[10]; /* index10  pregamma_update_index */
}

static __inline td_void isp_clut_update_index_write(isp_be_reg_type *be_reg, td_u32  uisp_clut_update_index)
{
    be_reg->reserved_0[11] = uisp_clut_update_index; /* index11  clut_update_index */
}

static __inline td_u32 isp_clut_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[11]; /* index11  clut_update_index */
}

/* pret be */
static __inline td_void isp_dpc_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_dpc_update_index)
{
    be_reg->reserved_0[12] = uisp_dpc_update_index; /* index12  dpc_update_index */
}

static __inline td_u32 isp_dpc_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[12]; /* index12  dpc_update_index */
}

static __inline td_void isp_wdr_update_index_write(isp_be_reg_type *be_reg, td_u32  uisp_wdr_update_index)
{
    be_reg->reserved_0[13] = uisp_wdr_update_index; /* index13  wdr_update_index */
}

static __inline td_u32 isp_wdr_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[13]; /* index13  wdr_update_index */
}

static __inline td_void isp_crb_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_crb_update_index)
{
    be_reg->reserved_0[14] = uisp_crb_update_index; /* index14  crb_update_index */
}

static __inline td_u32 isp_crb_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[14]; /* index14  crb_update_index */
}

static __inline td_void isp_expander_update_index_write(isp_be_reg_type *be_reg, td_u32 expander_update_index)
{
    be_reg->reserved_0[15] = expander_update_index; /* index15  expander_update_index */
}

static __inline td_u32 isp_expander_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[15]; /* index15  expander_update_index */
}

static __inline td_void isp_csp_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_csp_update_index)
{
    be_reg->reserved_0[16] = uisp_csp_update_index; /* index16  csp_update_index */
}

static __inline td_u32 isp_csp_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[16]; /* index16  csp_update_index */
}

static __inline td_void isp_demosaic_update_index_write(isp_be_reg_type *be_reg, td_u32 uisp_demosaic_update_index)
{
    be_reg->reserved_0[17] = uisp_demosaic_update_index; /* index17  demosaic_update_index */
}

static __inline td_u32 isp_demosaic_update_index_read(isp_be_reg_type *be_reg)
{
    return be_reg->reserved_0[17]; /* index17  demosaic_update_index */
}


/* useful macro for bit operation */
#define bits_mask(msb, lsb) (td_u32)(((1ULL << ((msb) + 1)) - 1) & ~((1ULL << (lsb)) - 1))
#define bits_shift(data, msb, lsb) (td_u32)(((data) & (bits_mask(msb, lsb) >> (lsb))) << (lsb))

#define reg_bits_modify(wrfunc, rdfunc, offset, msb, lsb, data) \
    do {                                                        \
        td_u32 cfg = rdfunc(offset);                         \
        wrfunc(offset, ((bits_shift(data, msb, lsb)) | (cfg & ~(bits_mask(msb, lsb))))); \
    } while (0)

#define reg_bits_read(wrfunc, rdfunc, offset, msb, lsb) \
    ((rdfunc(offset) & bits_mask(msb, lsb)) >> (lsb))

#define complement_align32(data, signbit) \
    ((data) & (1ULL << (signbit)) ? ((data) | bits_mask(31, ((signbit) + 1))) : (data))

/* Register  ot_isp_slave_mode_configs */
/*  Slave mode sensor sync signal  */
/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_configs_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x0, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_configs_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x0, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_vstime_low_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x8, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_vstime_low_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x8, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_vstime_high_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x4, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_vstime_high_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x4, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_hstime_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0xc, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_hstime_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0xc, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_vscyc_low_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x1c, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_vscyc_low_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x1c, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_vscyc_high_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x18, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_vscyc_high_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x18, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_hscyc_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x20, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_hscyc_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x20, 31, 0);
}

/* args  data (32-bit) */
static __inline td_void ot_isp_slave_mode_hsdly_write(ot_slave_dev slave_dev, td_u32 data)
{
    reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x2c, 31, 0, data);
}

static __inline td_u32 ot_isp_slave_mode_hsdly_read(ot_slave_dev slave_dev)
{
    return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, cap_slave_reg_base(slave_dev) + 0x2c, 31, 0);
}

static __inline td_void ot_isp_slave_mode_sync_write(ot_slave_dev slave_dev)
{
    switch (slave_dev) {
        case 0: /* slave 0 */
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 0, 0, 1);
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 0, 0, 0);
            break;
        case 1: /* slave 1 */
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 1, 1, 1);
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 1, 1, 0);
            break;
        case 2: /* slave 2 */
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 2, 2, 1);
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 2, 2, 0);
            break;
        case 3: /* slave 3 */
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 3, 3, 1);
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_SYNC, 3, 3, 0);
            break;
        default:
            break;
    }
}

static __inline td_void ot_isp_slave_mode_time_cfg_select_write(ot_slave_dev slave_dev, td_u32 data)
{
    switch (slave_dev) {
        case 0: /* slave 0 */
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_BASE, 1, 0, data);
            break;
        case 1: /* slave 1 */
            reg_bits_modify(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_BASE, 3, 2, data);
            break;

        default:
            break;
    }
}

static __inline td_u32 ot_isp_slave_mode_time_cfg_select_read(ot_slave_dev slave_dev)
{
    switch (slave_dev) {
        case 0: /* slave 0 */
            return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_BASE, 1, 0);
        case 1: /* slave 1 */
            return reg_bits_read(iowr_32direct_slave, iord_32direct_slave, SLAVE_MODE_REG_BASE, 3, 2);

        default:
            return 0;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
