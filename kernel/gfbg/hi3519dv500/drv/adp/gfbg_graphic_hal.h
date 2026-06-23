/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_GRAPHIC_HAL_H
#define GFBG_GRAPHIC_HAL_H

#include "gfbg.h"
#include "gfbg_reg.h"
#include "gfbg_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 fb_hal_gfbg_init(td_void);
td_void fb_hal_gfbg_deinit(td_void);

/* Prototype       : device Relative */
td_s32 fb_hal_disp_get_intf_enable(hal_disp_outputchannel chan, td_bool *intf_en);

td_s32 fb_hal_disp_get_intf_sync(hal_disp_outputchannel chan, hal_disp_syncinfo *sync_info);
td_s32 fb_hal_disp_get_disp_iop(hal_disp_outputchannel chan, td_bool *iop);
td_s32 fb_hal_disp_get_intf_mux_sel(hal_disp_outputchannel chan, vo_intf_type *intf_type);
td_s32 fb_hal_disp_get_vt_thd_mode(hal_disp_outputchannel chan, td_bool *field_mode);

td_u32 fb_hal_disp_get_int_status(td_u32 int_msk);
td_s32 fb_hal_disp_clear_int_status(td_u32 int_msk);

td_s32 fb_hal_video_set_layer_disp_rect(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 fb_hal_video_set_layer_video_rect(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 fb_hal_disp_set_int_mask1(td_u32 mask_en);
td_s32 fb_hal_disp_clr_int_mask1(td_u32 mask_en);

/* Video layer CSC relative hal functions. */
td_void fb_hal_link_get_hc_link(hal_disp_layer gfx_layer, td_u32 *data);

td_s32 fb_hal_set_layer_enable(hal_disp_layer layer, td_u32 enable);
td_void fb_hal_set_layer_ck_gt_en(hal_disp_layer layer, td_u32 ck_gt_en);
td_s32 fb_hal_get_layer_enable(hal_disp_layer layer, td_u32 *enable);
td_s32 fb_hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
td_s32 fb_hal_layer_get_layer_data_fmt(hal_disp_layer layer, td_u32 *format);

td_s32 fb_hal_layer_set_csc_coef(hal_disp_layer layer, const csc_coef *coef, const csc_coef_param *coef_param);
td_s32 fb_hal_layer_set_csc_en(hal_disp_layer layer, td_bool cscen);

td_s32 fb_hal_layer_set_src_resolution(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 fb_hal_layer_set_layer_in_rect(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 fb_hal_layer_set_layer_out_rect(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 fb_hal_layer_set_layer_galpha(hal_disp_layer layer, td_u8 alpha0);
td_s32 fb_hal_layer_get_layer_galpha(hal_disp_layer layer, td_u8 *alpha0);

td_s32 fb_hal_layer_set_reg_up(hal_disp_layer layer);

/* Prototype       : graphic layer Relative */
td_s32 fb_hal_graphic_set_gfx_addr(hal_disp_layer layer, td_phys_addr_t laddr);
td_s32 fb_hal_graphic_get_gfx_addr(hal_disp_layer layer, td_phys_addr_t *gfx_addr);
td_s32 fb_hal_graphic_set_gfx_stride(hal_disp_layer layer, td_u16 pitch);
td_s32 fb_hal_graphic_get_gfx_stride(hal_disp_layer layer, td_u32 *gfx_stride);
td_s32 fb_hal_graphic_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode);
td_s32 fb_hal_graphic_set_gfx_pre_mult(hal_disp_layer layer, td_u32 enable);
td_s32 fb_hal_graphic_set_gfx_palpha(hal_disp_layer layer, td_u32 alpha_en, td_u32 arange, td_u8 alpha0, td_u8 alpha1);
td_s32 fb_hal_graphic_get_gfx_palpha(hal_disp_layer layer, td_u32 *alpha_en, td_u8 *alpha0, td_u8 *alpha1);

td_s32 fb_hal_graphic_set_gfx_key_en(hal_disp_layer layer, td_u32 key_enable);
td_s32 fb_hal_graphic_set_gfx_key_mode(hal_disp_layer layer, td_u32 key_out);
td_s32 fb_hal_graphic_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min);
td_s32 fb_hal_graphic_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk);

td_s32 fb_hal_graphic_get_gfx_premult(hal_disp_layer layer, td_u32 *enable);

/* for compress */
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_hal_graphic_set_gfx_dcmp_addr(hal_disp_layer layer, td_phys_addr_t addr_ar);
td_s32 fb_hal_graphic_get_gfx_dcmp_enable_state(hal_disp_layer layer, td_bool *enable);
#endif
td_s32 fb_hal_graphic_set_gfx_dcmp_enable(hal_disp_layer layer, td_u32 enable);

td_void fb_hal_mddrc_get_status(td_u32 *status);
td_void fb_hal_mddrc_clear_status(td_u32 status);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_void fb_hal_para_set_para_addr_vhd_chn06(td_phys_addr_t para_addr_vhd_chn06);
td_void fb_hal_para_set_para_up_vhd_chn(td_u32 chn_num);
#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
td_void fb_hal_fdr_gfx_set_source_mode(td_u32 data, td_u32 source_mode);
td_void fb_hal_fdr_gfx_set_cmp_mode(td_u32 data, td_u32 cmp_mode);
td_void fb_hal_fdr_gfx_set_is_loss_lessa(td_u32 data, td_u32 is_lossless_a);
td_void fb_hal_fdr_gfx_set_is_loss_less(td_u32 data, td_u32 is_lossless);
#endif
#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
td_void fb_hal_v3r2_gfx_set_source_mode(td_u32 data, td_u32 source_mode);
td_void fb_hal_v3r2_gfx_set_cmp_mode(td_u32 data, td_u32 cmp_mode);
td_void fb_hal_v3r2_gfx_set_is_loss_less(td_u32 data, td_u32 is_lossless);
td_void fb_hal_v3r2_gfx_set_is_conv_en(td_u32 layer, td_u32 conv_en);
td_void fb_hal_v3r2_gfx_set_is_ice_en(td_u32 layer, td_u32 ice_en);
td_void fb_hal_v3r2_gfx_set_frame_size(td_u32 layer_index, td_u32 width, td_u32 height);
#endif
#endif

td_void hal_g0_zme_set_ck_gt_en(td_u32 ck_gt_en);
td_void hal_g0_zme_set_out_width(td_u32 out_width);
td_void hal_g0_zme_set_hfir_en(td_u32 hfir_en);
td_void hal_g0_zme_set_ahfir_mid_en(td_u32 ahfir_mid_en);
td_void hal_g0_zme_set_lhfir_mid_en(td_u32 lhfir_mid_en);
td_void hal_g0_zme_set_chfir_mid_en(td_u32 chfir_mid_en);
td_void hal_g0_zme_set_lhfir_mode(td_u32 lhfir_mode);
td_void hal_g0_zme_set_ahfir_mode(td_u32 ahfir_mode);
td_void hal_g0_zme_set_hfir_order(td_u32 hfir_order);
td_void hal_g0_zme_set_hratio(td_u32 hratio);
td_void hal_g0_zme_set_lhfir_offset(td_u32 lhfir_offset);
td_void hal_g0_zme_set_chfir_offset(td_u32 chfir_offset);
td_void hal_g0_zme_set_out_pro(td_u32 out_pro);
td_void hal_g0_zme_set_out_height(td_u32 out_height);
td_void hal_g0_zme_set_vfir_en(td_u32 vfir_en);
td_void hal_g0_zme_set_avfir_mid_en(td_u32 avfir_mid_en);
td_void hal_g0_zme_set_lvfir_mid_en(td_u32 lvfir_mid_en);
td_void hal_g0_zme_set_cvfir_mid_en(td_u32 cvfir_mid_en);
td_void hal_g0_zme_set_lvfir_mode(td_u32 lvfir_mode);
td_void hal_g0_zme_set_vafir_mode(td_u32 vafir_mode);
td_void hal_g0_zme_set_vratio(td_u32 vratio);
td_void hal_g0_zme_set_vtp_offset(td_u32 vtp_offset);
td_void hal_g0_zme_set_vbtm_offset(td_u32 vbtm_offset);

#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
td_void hal_g1_zme_set_ck_gt_en(td_u32 ck_gt_en);
td_void hal_g1_zme_set_out_width(td_u32 out_width);
td_void hal_g1_zme_set_hfir_en(td_u32 hfir_en);
td_void hal_g1_zme_set_ahfir_mid_en(td_u32 ahfir_mid_en);
td_void hal_g1_zme_set_lhfir_mid_en(td_u32 lhfir_mid_en);
td_void hal_g1_zme_set_chfir_mid_en(td_u32 chfir_mid_en);
td_void hal_g1_zme_set_lhfir_mode(td_u32 lhfir_mode);
td_void hal_g1_zme_set_ahfir_mode(td_u32 ahfir_mode);
td_void hal_g1_zme_set_hfir_order(td_u32 hfir_order);
td_void hal_g1_zme_set_hratio(td_u32 hratio);
td_void hal_g1_zme_set_lhfir_offset(td_u32 lhfir_offset);
td_void hal_g1_zme_set_chfir_offset(td_u32 chfir_offset);
td_void hal_g1_zme_set_out_pro(td_u32 out_pro);
td_void hal_g1_zme_set_out_height(td_u32 out_height);
td_void hal_g1_zme_set_vfir_en(td_u32 vfir_en);
td_void hal_g1_zme_set_avfir_mid_en(td_u32 avfir_mid_en);
td_void hal_g1_zme_set_lvfir_mid_en(td_u32 lvfir_mid_en);
td_void hal_g1_zme_set_cvfir_mid_en(td_u32 cvfir_mid_en);
td_void hal_g1_zme_set_lvfir_mode(td_u32 lvfir_mode);
td_void hal_g1_zme_set_vafir_mode(td_u32 vafir_mode);
td_void hal_g1_zme_set_vratio(td_u32 vratio);
td_void hal_g1_zme_set_vtp_offset(td_u32 vtp_offset);
td_void hal_g1_zme_set_vbtm_offset(td_u32 vbtm_offset);
#endif

td_void hal_clut_set_up_param(td_u32 layer_id, td_phys_addr_t clut_phy_addr);
td_void hal_clut_set_up(td_u32 layer_id);
/* for draw smart rect */
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_void hal_smart_rect_set_up(td_u32 layer_id, td_phys_addr_t phys_addr);
td_void hal_smart_rect_up_param(td_u32 layer_id);
td_void hal_smart_rect_disable(td_u32 layer_id);
#endif
td_void fb_hal_disp_get_int_state_vcnt(hal_disp_outputchannel chan, td_u32 *vcnt);

/* for low delay */
td_void hal_gfx_set_bind_mode(td_bool is_sync);
td_void hal_gfx_set_hardware_mute_clr(td_bool is_hw_mute_clr_en);
td_void hal_gfx_set_tde_safe_dis(td_u32 safe_dist);
td_void hal_gfx_set_ld_mute_en(td_bool is_mute_en);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_GRAPHIC_HAL_H */