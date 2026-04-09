/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: tde kernel API define
 */

#ifndef _HI_TDE_EXT_H_
#define _HI_TDE_EXT_H_

#include "drv_tde_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef hi_s32 (*tde_module_open)(hi_void);
typedef hi_s32 (*tde_module_close)(hi_void);
typedef hi_s32 (*tde_module_begin_job)(hi_s32 *);
typedef hi_s32 (*tde_module_end_job)(drv_tde_end_job_cmd *, tde_func_cb, hi_void *);
typedef hi_s32 (*tde_module_cancel_job)(hi_s32);
typedef hi_s32 (*tde_module_wait_for_done)(hi_s32, hi_u32);
typedef hi_s32 (*tde_module_wait_all_done)(hi_bool);
typedef hi_s32 (*tde_module_quick_copy)(hi_s32, hi_tde_single_src *);
typedef hi_s32 (*tde_module_quick_fill)(hi_s32, hi_tde_surface *, hi_tde_rect *, hi_u32);
typedef hi_s32 (*tde_module_quick_resize)(hi_s32, hi_tde_surface *, hi_tde_rect *, hi_tde_surface *, hi_tde_rect *);
typedef hi_s32 (*tde_module_quick_flicker)(hi_s32, hi_tde_surface *, hi_tde_rect *, hi_tde_surface *, hi_tde_rect *);
typedef hi_s32 (*tde_module_blit)(hi_s32, hi_tde_double_src *, hi_tde_opt *);
typedef hi_s32 (*tde_module_mbblit)(hi_s32, hi_tde_mb_src *, hi_tde_mb_opt *);
typedef hi_s32 (*tde_module_solid_draw)(hi_s32, hi_tde_single_src *, hi_tde_fill_color *, hi_tde_opt *);
typedef hi_s32 (*tde_module_set_deflicker_level)(hi_tde_deflicker_level);
typedef hi_s32 (*tde_module_enable_region_deflicker)(hi_bool);
typedef hi_s32 (*tde_module_rotate)(hi_s32, hi_tde_single_src *, hi_tde_rotate_angle);

typedef hi_s32 (*tde_module_set_alpha_threshold_value)(hi_u8);
typedef hi_s32 (*tde_module_set_alpha_threshold_state)(hi_bool);
typedef hi_s32 (*tde_module_get_alpha_threshold_state)(hi_bool *);
typedef hi_s32 (*tde_module_calc_scale_rect)(const hi_tde_rect *, const hi_tde_rect *, hi_tde_rect *, hi_tde_rect *);
typedef struct {
    tde_module_open drv_tde_module_open;
    tde_module_close drv_tde_module_close;
    tde_module_begin_job drv_tde_module_begin_job;
    tde_module_end_job drv_tde_module_end_job;
    tde_module_cancel_job drv_tde_module_cancel_job;
    tde_module_wait_for_done drv_tde_module_wait_for_done;
    tde_module_wait_all_done drv_tde_module_wait_all_done;
    tde_module_quick_copy drv_tde_module_quick_copy;
    tde_module_quick_fill drv_tde_module_quick_fill;
    tde_module_quick_resize drv_tde_module_quick_resize;
    tde_module_quick_flicker drv_tde_module_quick_flicker;
    tde_module_blit drv_tde_module_blit;
    tde_module_mbblit drv_tde_module_mb_blit;
    tde_module_solid_draw drv_tde_module_solid_draw;
    tde_module_set_deflicker_level drv_tde_module_set_deflicker_level;
    tde_module_enable_region_deflicker drv_tde_module_enable_region_deflicker;
    tde_module_rotate drv_tde_module_rotate;
    tde_module_set_alpha_threshold_value drv_tde_module_set_alpha_threshold_value;
    tde_module_set_alpha_threshold_state drv_tde_module_set_alpha_threshold_state;
    tde_module_get_alpha_threshold_state drv_tde_module_get_alpha_threshold_state;
    tde_module_calc_scale_rect drv_tde_module_cal_scale_rect;
} hi_tde_export_func;

hi_s32 tde_osi_open(hi_void);

hi_s32 tde_osi_close(hi_void);

hi_s32 tde_osi_begin_job(hi_s32 *handle, hi_void *private_data);

hi_s32 tde_osi_begin_job_ex(hi_s32 *handle);

/*
 * Function:      tde_osi_end_job
 * Description:   submit TDE task
 * Input:         handle: task handle
 *                is_block: if block
 *                time_out: timeout value(unit by 10ms)
 *                is_sync: if synchronization or not of sumbitting operate
 *                func_compl_cb: callback function of completing operate
 * Return:        success / error code
 */
hi_s32 tde_osi_end_job(drv_tde_end_job_cmd *end_job, tde_func_cb func_compl_cb, hi_void *func_para);

/*
 * Function:      tde_osi_cancel_job
 * Description:   delete TDE task created, only effectivly callede before endjob
 *                release software resource of list of its task
 * Input:         handle: task handle
 * Return:        success / error code
 */
hi_s32 tde_osi_cancel_job(hi_s32 handle);

hi_s32 tde_osi_wait_for_done(hi_s32 handle, hi_u32 time_out);

hi_s32 tde_osi_wait_all_done(hi_bool is_sync);

/*
 * Function:      tde_osi_quick_copy
 * Description:   quick blit source to target bitmap,no any functional operate,
 *                the size of source and target bitmap is the same, pixel format is the same too
 * Input:         handle: task handle pSrc: source bitmap info struct src_rect:
 * source bitmap  operate rect pDst: target bitmap information struct dst_rect: target bitmap operate rect
 * Return:        success / error code
 */
hi_s32 tde_osi_quick_copy(hi_s32 handle, hi_tde_single_src *single_src);

/*
 * Function:      tde_osi_quick_fill
 * Description:   quickly fill fixed value to target bitmap
 * Input:         handle: task handle
 *                dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate rect
 *                fill_data: fill data, pixel format is the same as target bitmap format
 * Return:        success / error code
 */
hi_s32 tde_osi_quick_fill(hi_s32 handle, hi_tde_surface *dst_surface, hi_tde_rect *dst_rect, hi_u32 fill_data);

/*
 * Function:      tde_osi_quick_resize
 * Description:   zoom the size of source bitmap to the size of target bitmap,
                  source and target are the same bitmap
 * Input:         handle: task handle
 *                src_surface: source bitmap info struct
 *                src_rect: source bitmap operate rect
 *                dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate rect
 * Return:        success / error code
 */
hi_s32 tde_osi_quick_resize(hi_s32 handle, hi_tde_surface *src_surface, hi_tde_rect *src_rect,
                            hi_tde_surface *dst_surface, hi_tde_rect *dst_rect);

/*
 * Function:      tde_osi_quick_flicker
 * Description:   deflicker source bitmap,output to target bitmap,
                  source and target can be the same bitmap
 * Input:         handle: task handle
 *                src_surface: source bitmap info struct
 *                src_rect: source bitmap operate rect
 *                dst_surface: target bitmap info struct
 *                dst_rect: target bitmap operate rect
 * Return:        success / error code
 */
hi_s32 tde_osi_quick_flicker(hi_s32 handle, hi_tde_surface *src_surface, hi_tde_rect *src_rect,
                             hi_tde_surface *dst_surface, hi_tde_rect *dst_rect);

/*
 * Function:      tde_osi_blit
 * Description:   operate pstBackGround with pstForeGround, which result output to pDst, operate setting is in pOpt
 * Input:         handle:task handle
 *                opt: operate parameter setting struct
 * Return:        success / error code
 */
hi_s32 tde_osi_blit(hi_s32 handle, hi_tde_double_src *double_src, hi_tde_opt *opt);

/*
 * Function:      tde_osi_mb_blit
 * Description:   MB blit interface
 * Input:         handle: task handle
 *                mb_opt:  operate parameter setting struct
 * Return:        >0: return task id of current operate ; <0: fail
 */
hi_s32 tde_osi_mb_blit(hi_s32 handle, hi_tde_mb_src *mb_src, hi_tde_mb_opt *mb_opt);

/*
 * Function:      tde_osi_bitmap_mask_rop
 * Description:   Ropmask source with mask bitmap at first, and then ropmask source1 with middle bitmap
 *                output result into target bitmap
 * Input:         handle: task handle
 * Return:        success / error code
 */
hi_s32 tde_osi_bitmap_mask_rop(hi_s32 handle, hi_tde_triple_src *triple_src, hi_tde_rop_mode rop_color,
                               hi_tde_rop_mode rop_alpha);

/*
 * Function:      tde_osi_bitmap_mask_blend
 * Description:   blendmask source2 with mask bitmap at first, and then blend source1 with middle bitmap
 *                output result to target bitmap
 * Input:         handle: task handle
 *                alpha:  alpha value of operate
 * Return:        success / error code
 */
hi_s32 tde_osi_bitmap_mask_blend(hi_s32 handle, hi_tde_triple_src *triple_src, hi_u8 alpha,
                                 hi_tde_alpha_blending blend_mode);

/*
 * Function:      tde_osi_solid_draw
 * Description:   output result of operate single color value with source bitmap,operate setting is in pOpt
 * Input:         handle: task handle
 *                dst_surface: target bitmap info struct
 *                fill_color: color info
 *                opt: operate parameter setting struct
 * Return:        success / error code
 */
hi_s32 tde_osi_solid_draw(hi_s32 handle, hi_tde_single_src *single_src, hi_tde_fill_color *fill_color,
                          hi_tde_opt *opt);

hi_s32 tde_osi_set_deflicker_level(hi_tde_deflicker_level deflicker_level);

hi_s32 tde_osi_get_deflicker_level(hi_tde_deflicker_level *deflicker_level);

hi_s32 tde_osi_set_alpha_threshold_value(hi_u8 threshold_value);

/*
 * Function:      tde_osi_get_alpha_threshold_value
 * Description:   get alpha threshold value
 * Output:        threshold_value: alpha threshold valu
 * Return:        success / error code
 */
hi_s32 tde_osi_get_alpha_threshold_value(hi_u8 *threshold_value);

/*
 * Function:      tde_osi_set_alpha_threshold_state
 * Description:   set alpha threshold switch
 * Input:         alpha_threshold_en:if open alpha threshold flag or not
 * Return:        success / error code
 */
hi_s32 tde_osi_set_alpha_threshold_state(hi_bool alpha_threshold_en);

/*
 * Function:      tde_osi_get_alpha_threshold_state
 * Description:   get alpha threshold switch
 * Input:         alpha_threshold_en:if open alpha threshold flag or not
 * Return:        success / error code
 */
hi_s32 tde_osi_get_alpha_threshold_state(hi_bool *alpha_threshold_en);

/*
 * Function:      tde_osi_pattern_fill
 * Description:   pattern fill
 * Output:        handle:task handle
                  opt:pattern fill operate option
 * Return:        success/error code
 * Others:        pattern width is 256 at most, height is not litmited
 */
hi_s32 tde_osi_pattern_fill(hi_s32 handle, hi_tde_double_src *double_src, hi_tde_pattern_fill_opt *opt);

/*
 * Function:      tde_osi_enable_region_deflicker
 * Description:   enable/unabel to local deflicker
 * Output:        region_deflicker:deflick enable flag
 * Return:        success/error code
 */
hi_s32 tde_osi_enable_region_deflicker(hi_bool region_deflicker);

hi_s32 tde_cal_scale_rect(const hi_tde_rect *src_rect, const hi_tde_rect *dst_rect, hi_tde_rect *rect_in_src,
                          hi_tde_rect *rect_in_dst);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _TDE_OSICTL_H_ */
