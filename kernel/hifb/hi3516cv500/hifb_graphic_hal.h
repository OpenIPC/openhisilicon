/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#ifndef __HIFB_HAL_H__
#define __HIFB_HAL_H__

#include "hifb.h"
#include "hifb_reg.h"
#include "hifb_def.h"
#include "hi_comm_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_s32 fb_hal_vou_init(hi_void);
hi_void fb_hal_vou_exit(hi_void);
hi_void fb_hal_write_reg(hi_u32 *address, hi_u32 value);

/* Prototype       : sys Relative */
hi_void hal_sys_set_outstanding(hi_void);
hi_void hal_sys_set_arb_mode(hi_u32 mode);
hi_void hal_sys_set_rd_bus_id(hi_u32 mode);
hi_void hal_sys_vdp_reset_clk(hi_u32 sel);

/* Prototype       : device Relative */
hi_bool hal_disp_set_intf_enable(hal_disp_outputchannel chan, hi_bool intf);
hi_bool fb_hal_disp_get_intf_enable(hal_disp_outputchannel chan, hi_bool *intf_en);
hi_void hifb_hal_soft_int_en(hi_bool soft_int_en);

hi_bool fb_hal_disp_get_int_state(hal_disp_outputchannel chan, hi_bool *bottom);
hi_bool hal_disp_get_int_state_vblank(hal_disp_outputchannel chan, hi_bool *vblank);
hi_bool hal_disp_get_int_state_vback_blank(hal_disp_outputchannel chan, hi_bool *vback_blank);
hi_bool hal_disp_get_int_state_vcnt(hal_disp_outputchannel chan, hi_u32 *vcnt);
hi_bool hal_disp_set_intf_sync(hal_disp_outputchannel chan, hal_disp_syncinfo *sync_info);
hi_bool hal_disp_set_intf_sync_inv(hal_disp_intf intf, hal_disp_syncinv *inv);
hi_bool hal_disp_set_intf_multi_chn_en(hal_disp_intf intf, hal_multichn_en multi_chn_en);
hi_bool hal_disp_set_intf_ctrl(hal_disp_intf intf, hi_u32 *ctrl_info);
hi_bool fb_hal_disp_get_disp_iop(hal_disp_outputchannel chan, hi_bool *iop);
hi_bool hal_disp_set_intf_mux_sel(hal_disp_outputchannel chan, hal_disp_intf intf);
hi_bool hal_disp_set_bt1120_sel(hal_disp_outputchannel chan);
hi_bool hal_disp_set_intf_clip_enable(hal_disp_intf intf, hi_bool clip);
hi_bool hal_disp_set_intf_clip(hal_disp_intf chan, hi_bool clip, hal_disp_clip *clip_data);
hi_bool hal_disp_set_intvsync_te_mode(hal_disp_outputchannel chan, hi_bool slave);
hi_bool hal_disp_set_vt_thd_mode(hal_disp_outputchannel chan, hi_u32 field_mode);
hi_bool hal_disp_get_vt_thd_mode(hal_disp_outputchannel chan, hi_bool *field_mode);
hi_bool hal_disp_set_vt_thd(hal_disp_outputchannel chan, hi_u32 vtthd);

hi_bool fb_hal_disp_set_int_mask(hi_u32 mask_en);
hi_bool fb_hal_disp_clr_int_mask(hi_u32 mask_en);
hi_u32 fb_hal_disp_get_int_status(hi_u32 int_msk);
hi_u32 fb_hal_disp_get_ori_int_status(hi_u32 int_status);
hi_bool fb_hal_disp_clear_int_status(hi_u32 int_msk);
hi_u32 hal_disp_get_raw_int_status(hi_u32 int_msk);
hi_u32 hal_disp_get_raw_int_status1(hi_u32 int_msk);

hi_bool hal_disp_set_clk_gate_enable(hi_u32 data);
hi_void fb_hal_disp_set_reg_up(hal_disp_outputchannel chan);
hi_u32 fb_hal_disp_get_reg_up(hal_disp_outputchannel chan);
hi_bool hal_disp_get_intf_sync(hal_disp_outputchannel chan, hal_disp_syncinfo *sync_info);
hi_bool hal_disp_get_intf_mux_sel(hal_disp_outputchannel chan, VO_INTF_TYPE_E *intf_type);
hi_bool fb_hal_video_set_layer_disp_rect(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool fb_hal_video_set_layer_video_rect(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool hal_video_set_multi_area_laddr(hal_disp_layer layer, hi_u32 area_num, hi_ulong laddr, hi_u16 stride);
hi_bool hal_video_set_layer_rim_width(hal_disp_layer layer, hi_u32 rim_width);
hi_bool hal_video_set_layer_rim_col0(hal_disp_layer layer, vdp_bkg *rimCol0);
hi_bool hal_video_set_layer_rim_col1(hal_disp_layer layer, vdp_bkg *rim_col1);
hi_bool hal_video_set_multi_area_caddr(hal_disp_layer layer, hi_u32 area_num, hi_ulong caddr, hi_u16 stride);
hi_bool hal_video_get_multi_area_laddr(hal_disp_layer layer, hi_u32 area_num, hi_u32 *laddr_low,
                                       hi_u32 *laddr_high);
hi_bool hal_video_get_multi_area_caddr(hal_disp_layer layer, hi_u32 area_num, hi_u32 *addr_Low,
                                       hi_u32 *caddr_high);

hi_bool hal_video_set_multi_area_vdcmp_offset(hal_disp_layer layer, hi_u32 area_num, hi_u64 lh_offset,
                                              hi_u64 ch_offset);

hi_bool hal_video_set_multi_area_vdcmp_enable(hal_disp_layer layer, hi_u32 enable);
hi_bool hal_video_set_multi_area_reso(hal_disp_layer layer, hi_u32 area_num, hi_u32 width);
hi_bool hal_video_set_multi_area_rect(hal_disp_layer layer, hi_u32 area_num, RECT_S *video_area_rect);
hi_bool hal_video_get_mrg_state(hal_disp_layer layer, hi_u32 *mrg);
hi_bool hal_video_clr_mrg_state(hal_disp_layer layer);
hi_bool hal_video_get_vdcmp_luma_state(hal_disp_layer layer, hi_u32 *dcmp_luma_stat);
hi_bool hal_video_get_vdcmp_chroma_state(hal_disp_layer layer, hi_u32 *dcmp_chroma_stat);
hi_bool hal_video_clr_vdcmp_luma_state(hal_disp_layer layer);
hi_bool hal_video_clr_vdcmp_chroma_state(hal_disp_layer layer);

hi_bool hal_video_set_all_area_disable(hal_disp_layer layer);

/* Video layer CVFIR relative hal functions */
hi_void hal_video_cvfir_set_out_height(hal_disp_layer layer, hi_u32 out_height);
hi_void hal_video_cvfir_set_out_fmt(hal_disp_layer layer, hi_u32 out_fmt);
hi_void hal_video_cvfir_set_out_pro(hal_disp_layer layer, hi_u32 out_pro);
hi_void hal_video_cvfir_set_vzme_ck_gt_en(hal_disp_layer layer, hi_bool vzme_ck_gt_en);

hi_void hal_video_cvfir_set_cvfir_en(hal_disp_layer layer, hi_u32 cvfir_en);
hi_void hal_video_cvfir_set_cvmid_en(hal_disp_layer layer, hi_u32 cvmid_en);
hi_void hal_video_cvfir_set_cvfir_mode(hal_disp_layer layer, hi_u32 cvfir_mode);
hi_void hal_video_cvfir_set_vratio(hal_disp_layer layer, hi_u32 vratio);

hi_void hal_video_cvfir_set_vchroma_offset(hal_disp_layer layer, hi_u32 vchroma_offset);
hi_void hal_video_cvfir_set_vbchroma_offset(hal_disp_layer layer, hi_u32 vbchroma_offset);

hi_void hal_video_cvfir_set_coef(hal_disp_layer layer, cvfir_coef *cvfir_coef);

/* Video layer HFIR relative hal functions */
hi_void hal_video_hfir_set_ck_gt_en(hal_disp_layer layer, hi_u32 ck_gt_en);
hi_void hal_video_hfir_set_mid_en(hal_disp_layer layer, hi_u32 mid_en);
hi_void hal_video_hfir_set_hfir_mode(hal_disp_layer layer, hi_u32 hfir_mode);
hi_void hal_video_hfir_set_hfir_en(hal_disp_layer layer, hi_u32 hfir_en);
hi_void hal_video_hfir_set_coef(hal_disp_layer layer, hfir_coef *hfir_coef);

/* Video layer CSC relative hal functions. */
hi_void fb_hal_layer_csc_set_enable(hal_disp_layer layer, hi_bool csc_en);
hi_void fb_hal_layer_csc_set_ck_gt_en(hal_disp_layer layer, hi_bool ck_gt_en);
hi_void fb_hal_layer_csc_set_coef(hal_disp_layer layer, vdp_csc_coef *coef);
hi_void fb_hal_layer_csc_set_dc_coef(hal_disp_layer layer, vdp_csc_dc_coef *csc_dc_coef);
hi_bool hal_layer_set_link_ctrl(hal_disp_layer layer, hi_u8 cbm_id);

hi_bool fb_hal_layer_enable_layer(hal_disp_layer layer, hi_u32 enable);
hi_bool fb_hal_layer_get_layer_enable(hal_disp_layer layer, hi_u32 *enable);
hi_bool fb_hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
hi_bool fb_hal_layer_get_layer_data_fmt(hal_disp_layer layer, hi_u32 *pu32Fmt);
hi_bool fb_hal_layer_set_csc_coef(hal_disp_layer layer, csc_coef *coef);
hi_bool fb_hal_layer_set_csc_mode(hal_disp_layer layer, hi_bool is_hc_mode_by_709);
hi_bool fb_hal_layer_set_csc_en(hal_disp_layer layer, hi_bool cscen);

hi_bool fb_hal_layer_set_src_resolution(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool fb_hal_layer_set_layer_in_rect(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool fb_hal_layer_set_layer_out_rect(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool hal_layer_set_layer_galpha(hal_disp_layer layer, hi_u8 alpha0);
hi_bool hal_layer_get_layer_galpha(hal_disp_layer layer, hi_u8 *alpha0);
hi_bool hal_layer_set_zme_fir_enable(hal_disp_layer layer, hal_disp_zmemode mode, hi_u32 enable);
hi_bool hal_layer_set_zme_msc_enable(hal_disp_layer layer, hal_disp_zmemode mode, hi_u32 enable);
hi_bool hal_layer_set_hfir_order(hal_disp_layer layer, hi_u32 hfir_order);
hi_bool hal_layer_set_zme_ver_tap(hal_disp_layer layer, hal_disp_zmemode mode, hi_u32 ver_tap);
hi_bool hal_layer_set_hor_ratio(hal_disp_layer layer, hi_u32 ratio);
hi_bool fb_hal_layer_set_reg_up(hal_disp_layer layer);
hi_bool hal_layer_set_layer_bg_color(hal_disp_layer layer, hal_video_layer_bkcolor *bg_color);
hi_bool hal_disp_set_layer_boundary_color(hal_disp_layer layer, hal_disp_boundary_color *boundary_color);
hi_bool hal_disp_set_layer_boundary_width(hal_disp_layer layer, hi_u32 width);
hi_bool hal_disp_set_chn_boundary(hal_disp_layer layer, hi_u32 area_num, const VO_CHN_BOUNDARY_S *chn_boundary);

/* Prototype       : graphic layer Relative */
hi_bool hal_graphic_set_gfx_addr(hal_disp_layer layer, hi_u64 laddr);
hi_bool hal_graphic_get_gfx_addr(hal_disp_layer layer, hi_u64 *gfx_addr);
hi_bool hal_graphic_set_gfx_stride(hal_disp_layer layer, hi_u16 pitch);
hi_bool hal_graphic_get_gfx_stride(hal_disp_layer layer, hi_u32 *gfx_stride);
hi_bool hal_graphic_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode);
hi_bool hal_graphic_set_gfx_pre_mult(hal_disp_layer layer, hi_u32 enable);
hi_bool hal_graphic_set_gfx_palpha(hal_disp_layer layer, hi_u32 alphaEn, hi_u32 arange, hi_u8 alpha0,
                                   hi_u8 alpha1);
hi_bool hal_graphic_get_gfx_palpha(hal_disp_layer layer, hi_u32 *alpha_en, hi_u8 *alpha0, hi_u8 *alpha1);
hi_bool hal_graphic_set_gfx_palpha_range(hal_disp_layer layer, hi_u32 arange);

hi_bool hal_graphic_set_gfx_key_en(hal_disp_layer layer, hi_u32 key_enable);
hi_bool hal_graphic_set_gfx_key_mode(hal_disp_layer layer, hi_u32 key_out);
hi_bool hal_graphic_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min);
hi_bool hal_graphic_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk);

hi_bool hal_graphic_get_gfx_premult(hal_disp_layer layer, hi_u32 *enable);

/* for compress */
hi_bool hal_graphic_set_gfx_dcmp_enable(hal_disp_layer layer, hi_u32 enable);
hi_bool hal_graphic_get_gfx_dcmp_enable_state(hal_disp_layer layer, hi_bool *enable);

hi_bool hal_mddrc_init_mddr_detect(hi_void);
hi_void hal_mddrc_get_status(hi_u32 *status);
hi_void hal_mddrc_clear_status(hi_u32 status);
hi_void hal_mddrc_init(hi_void);
hi_void hal_mddrc_exit(hi_void);

hi_void hal_para_set_para_addr_vhd_chn06(hi_u64 para_addr_vhd_chn06);
hi_void hal_para_set_para_up_vhd_chn(hi_u32 chn_num);
hi_void hal_fdr_gfx_set_dcmp_en(hi_u32 data, hi_u32 dcmp_en);
hi_void hal_g0_zme_set_ck_gt_en(hi_u32 ck_gt_en);
hi_void hal_g0_zme_set_out_width(hi_u32 out_width);
hi_void hal_g0_zme_set_hfir_en(hi_u32 hfir_en);
hi_void hal_g0_zme_set_ahfir_mid_en(hi_u32 ahfir_mid_en);
hi_void hal_g0_zme_set_lhfir_mid_en(hi_u32 lhfir_mid_en);
hi_void hal_g0_zme_set_chfir_mid_en(hi_u32 chfir_mid_en);
hi_void hal_g0_zme_set_lhfir_mode(hi_u32 lhfir_mode);
hi_void hal_g0_zme_set_ahfir_mode(hi_u32 ahfir_mode);
hi_void hal_g0_zme_set_hfir_order(hi_u32 hfir_order);
hi_void hal_g0_zme_set_hratio(hi_u32 hratio);
hi_void hal_g0_zme_set_lhfir_offset(hi_u32 lhfir_offset);
hi_void hal_g0_zme_set_chfir_offset(hi_u32 chfir_offset);
hi_void hal_g0_zme_set_out_pro(hi_u32 out_pro);
hi_void hal_g0_zme_set_out_height(hi_u32 out_height);
hi_void hal_g0_zme_set_vfir_en(hi_u32 vfir_en);
hi_void hal_g0_zme_set_avfir_mid_en(hi_u32 avfir_mid_en);
hi_void hal_g0_zme_set_lvfir_mid_en(hi_u32 lvfir_mid_en);
hi_void hal_g0_zme_set_cvfir_mid_en(hi_u32 cvfir_mid_en);
hi_void hal_g0_zme_set_lvfir_mode(hi_u32 lvfir_mode);
hi_void hal_g0_zme_set_vafir_mode(hi_u32 vafir_mode);
hi_void hal_g0_zme_set_vratio(hi_u32 vratio);
hi_void hal_g0_zme_set_vtp_offset(hi_u32 vtp_offset);
hi_void hal_g0_zme_set_vbtm_offset(hi_u32 vbtm_offset);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __HIFB_HAL_H__ */
