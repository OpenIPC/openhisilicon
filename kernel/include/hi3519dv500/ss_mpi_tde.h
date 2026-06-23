/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_TDE_H
#define SS_MPI_TDE_H

#include "ot_common_tde.h"


#ifdef __cplusplus
extern "C" {
#endif

/* API Declaration */
td_s32 ss_tde_open(td_void);
td_void ss_tde_close(td_void);
td_s32 ss_tde_begin_job(td_void);
td_s32 ss_tde_end_job(td_s32 handle, td_bool is_sync, td_bool is_block, td_u32 time_out);
td_s32 ss_tde_cancel_job(td_s32 handle);
td_s32 ss_tde_wait_the_task_done(td_s32 handle);
td_s32 ss_tde_wait_all_task_done(td_void);
td_s32 ss_tde_quick_fill(td_s32 handle, const ot_tde_none_src *none_src, td_u32 fill_data);
td_s32 ss_tde_draw_corner_box(td_s32 handle, const ot_tde_surface *dst_surface, const ot_tde_corner_rect *corner_rect,
    td_u32 num);
td_s32 ss_tde_draw_line(td_s32 handle, const ot_tde_surface *dst_surface, const ot_tde_line *line, td_u32 num);
td_s32 ss_tde_quick_copy(td_s32 handle, const ot_tde_single_src *single_src);
td_s32 ss_tde_quick_resize(td_s32 handle, const ot_tde_single_src *single_src);
td_s32 ss_tde_solid_draw(td_s32 handle, const ot_tde_single_src *single_src, const ot_tde_fill_color *fill_color,
    const ot_tde_opt *opt);
td_s32 ss_tde_rotate(td_s32 handle, const ot_tde_single_src *single_src, ot_tde_rotate_angle rotate);
td_s32 ss_tde_bit_blit(td_s32 handle, const ot_tde_double_src *double_src, const ot_tde_opt *opt);
td_s32 ss_tde_pattern_fill(td_s32 handle, const ot_tde_double_src *double_src,
    const ot_tde_pattern_fill_opt *fill_opt);
td_s32 ss_tde_mb_blit(td_s32 handle, const ot_tde_mb_src *mb_src, const ot_tde_mb_opt *opt);
td_s32 ss_tde_set_alpha_threshold_value(td_u8 threshold_value);
td_s32 ss_tde_get_alpha_threshold_value(td_u8 *threshold_value);
td_s32 ss_tde_set_alpha_threshold_state(td_bool threshold_en);
td_s32 ss_tde_get_alpha_threshold_state(td_bool *threshold_en);

#ifdef __cplusplus
}
#endif

#endif /* __SOURCE_MSP_API_TDE_SRC_LINUX_API__ */
