/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_COMMON_AIBNR_H
#define OT_COMMON_AIBNR_H

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_aiisp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OT_AIBNR_NLC_LUT_NUM 32

typedef enum {
    OT_AIBNR_REF_MODE_NORM = 0,
    OT_AIBNR_REF_MODE_NONE,
    OT_AIBNR_REF_MODE_BUTT
} ot_aibnr_ref_mode;

typedef struct {
    td_bool enable; /* RW; Range:[0x0, 0x1]; Format:1.0 */
    td_u8 step_bit;  /* RW; Range:[0x0, 0x4]; Format:3.0 */
    td_u16 r_fact[OT_AIBNR_NLC_LUT_NUM]; /* RW; Range:[500, 2000]; Format:11.0 */
    td_u16 b_fact[OT_AIBNR_NLC_LUT_NUM]; /* RW; Range:[500, 2000]; Format:11.0 */
} ot_aibnr_nlc;

typedef struct {
    ot_aiisp_model model;
    td_bool is_wdr_mode; /* W; is wdr mode or not. Range:[0, 0x1];Format:1.0; not support for Hi3516CV610 */
    ot_aibnr_ref_mode ref_mode;
    td_bool nlc_en; /* W; enable/disable nlc. Range:[0, 0x1];Format:1.0; not support for Hi3516CV610 */
} ot_aibnr_model;

typedef struct {
    td_bool load_ref_en;          /* RW; Range:[0x0, 0x1]; Format:1.0 */
    td_u8   motion_gain;          /* RW; Range:[0, 31];Format:5.0; motion detection sensitivity */
    td_u8   motion_thr;              /* RW; Range:[0, 255];Format:8.0; motion threshold */
    td_u8   motion_noise_control; /* RW; Range:[0, 255];Format:8.0; suppress motion noise */
    td_u8   tf_speed_fine;    /* RW; Range:[0, 255];Format:8.0; background converge speed(fine) */
    td_u8   tf_speed_coarse;  /* RW; Range:[0, 255];Format:8.0; background converge speed(coarse) */
} ot_aibnr_tnr;      /* only support for Hi3516CV610 pro model */

typedef struct {
    td_u32 sfs; /* RW; spatial filter strength. Range:[0, 31]; Format:5.0 */
    td_u32 tfs; /* RW; temporal filter strength. Range:[0, 255]; Format:8.0. only support for Hi3516CV610 pro model */
    td_u32 bright_denoise_strength; /* RW; Range:[0, 255]; Format:8.0 */
    td_u32 dark_denoise_strength; /* RW; Range:[0, 255]; Format:8.0 */
    ot_aibnr_nlc nlc; /* not support for Hi3516CV610 */
    ot_aibnr_tnr tnr; /* only support for Hi3516CV610 pro model */
} ot_aibnr_manual_attr;

typedef struct {
    td_u32 sfs[OT_AIISP_AUTO_ISO_NUM]; /* RW; spatial filter strength. Range:[0, 31]; Format:5.0 */
    td_u32 tfs[OT_AIISP_AUTO_ISO_NUM]; /* RW; temporal filter strength. Range:[0, 31]; Format:5.0. not support */
} ot_aibnr_auto_attr;

typedef struct {
    td_bool enable;     /* RW; Range:[0x0, 0x1]; Format:1.0 */
} ot_aibnr_status;

typedef struct {
    td_bool enable;     /* RW; Range:[0x0, 0x1]; Format:1.0 */
    td_bool bnr_bypass; /* RW; Range:[0x0, 0x1]; Format:1.0 */
    td_bool blend;      /* RW; Range:[0x0, 0x1]; Format:1.0; not support for Hi3516CV610 */
    td_u32 input_depth; /* RW; Range:[0, 8]; Format:4.0 */
    td_u32 output_depth; /* RW; Range:[0, 8]; Format:4.0 */
    ot_op_mode op_type;
    ot_aibnr_manual_attr manual_attr;
    ot_aibnr_auto_attr auto_attr; /* not support */
} ot_aibnr_attr;

typedef struct {
    ot_aibnr_ref_mode ref_mode;
} ot_aibnr_cfg;

typedef struct {
    ot_op_mode op_type;
    td_u8 manual_id; /* RW; Range:[0, 15]; Format:4.0 */
    td_u8 auto_id[OT_AIISP_AUTO_ISO_NUM]; /* RW; Range:[0, 15]; Format:4.0 */
} ot_aibnr_model_attr;

typedef struct {
    td_u32 id; /* R; model id. Range:[0, 15]; Format:4.0 */
    td_bool is_wdr_mode; /* R; is wdr mode or not. Range:[0, 0x1];Format:1.0; not support for Hi3516CV610 */
} ot_aibnr_model_info;

typedef struct {
    td_u32 num; /* R; model num. Range:[0, 16]; Format:5.0 */
    ot_aibnr_model_info info[OT_AIISP_MAX_MODEL_NUM];
} ot_aibnr_model_list;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* OT_COMMON_AIBNR_H */
