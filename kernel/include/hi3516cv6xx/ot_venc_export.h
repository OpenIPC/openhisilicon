/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_VENC_EXPORT_H
#define OT_VENC_EXPORT_H

#include "ot_common.h"
#include "ot_common_rc.h"
#include "ot_type.h"

typedef enum {
    VENC_EXPORT_FRAME_TYPE_I = 0, /* i frame */
    VENC_EXPORT_FRAME_TYPE_P, /* p frame  */
    VENC_EXPORT_FRAME_TYPE_B, /* b frame  */
    VENC_EXPORT_FRAME_TYPE_SP, /* special p frame in dual p and singlesp */
    VENC_EXPORT_FRAME_TYPE_VI, /* virtual i in smartp and smartcrr */
    VENC_EXPORT_FRAME_TYPE_BUTT,
} ot_venc_export_frame_type;

typedef enum {
    VENC_EXPORT_FRAME_STATUS_INIT = 0,
    VENC_EXPORT_FRAME_STATUS_NORMAL,
    VENC_EXPORT_FRAME_STATUS_LOST,
    VENC_EXPORT_FRAME_STATUS_REENCODE,
    VENC_EXPORT_FRAME_STATUS_BUTT
} ot_venc_export_frame_status;

typedef struct {
    ot_payload_type type;
    ot_venc_rc_attr venc_rc_attr;
} ot_venc_export_rc_attr;

typedef struct {
    ot_venc_export_frame_type cur_frame_type;
    ot_venc_export_frame_status last_frame_status;
    td_u32 last_frm_bits;
    td_u32 last_madi_val;
    td_u32 last_madp_val;
    td_u32 last_move_scene_sum;
} ot_venc_export_rc_frame_info;

typedef struct {
    td_u32 target_bits;
    td_u32 max_qp;
    td_u32 min_qp;
    td_u32 start_qp;
} ot_venc_export_rc_enc_info;

typedef struct {
    td_s32 (*venc_export_rc_init)(td_s32 chn, const ot_venc_export_rc_attr *user_rc_attr);
    td_s32 (*venc_export_rc_deinit)(td_s32 chn);
    td_s32 (*venc_export_rc_reset)(td_s32 chn);
    td_s32 (*venc_export_rc_change)(td_s32 chn, const ot_venc_export_rc_attr *user_rc_attr);
    td_s32 (*venc_export_rc_calc)(td_s32 chn, const ot_venc_export_rc_frame_info *rc_frame_info,
        ot_venc_export_rc_enc_info *rc_enc_info);
} ot_venc_export_callback;

typedef td_s32 ot_venc_register_export_callback(td_s32 chn, const ot_venc_export_callback *export_callback);

typedef struct {
    ot_venc_register_export_callback *register_export_callback;
} ot_venc_export_symbol;

ot_venc_export_symbol *ot_venc_get_export_symbol(td_void);

#endif /* OT_VENC_EXPORT_H */
