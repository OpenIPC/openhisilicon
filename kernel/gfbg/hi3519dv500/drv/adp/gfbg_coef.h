/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_COEF_H
#define GFBG_COEF_H

#include "ot_type.h"
#include "ot_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* COLOR SPACE CONVERT DEFINITION */
typedef struct {
    td_s32 csc_coef00;
    td_s32 csc_coef01;
    td_s32 csc_coef02;

    td_s32 csc_coef10;
    td_s32 csc_coef11;
    td_s32 csc_coef12;

    td_s32 csc_coef20;
    td_s32 csc_coef21;
    td_s32 csc_coef22;
} vdp_csc_coef;

typedef struct {
    td_s32 csc_in_dc0;
    td_s32 csc_in_dc1;
    td_s32 csc_in_dc2;

    td_s32 csc_out_dc0;
    td_s32 csc_out_dc1;
    td_s32 csc_out_dc2;
} vdp_csc_dc_coef;

typedef struct {
    // for old version csc
    td_s32 csc_coef00;
    td_s32 csc_coef01;
    td_s32 csc_coef02;

    td_s32 csc_coef10;
    td_s32 csc_coef11;
    td_s32 csc_coef12;

    td_s32 csc_coef20;
    td_s32 csc_coef21;
    td_s32 csc_coef22;

    td_s32 csc_in_dc0;
    td_s32 csc_in_dc1;
    td_s32 csc_in_dc2;

    td_s32 csc_out_dc0;
    td_s32 csc_out_dc1;
    td_s32 csc_out_dc2;
} csc_coef;

typedef struct {
    td_s32 csc_scale2p;
    td_s32 csc_clip_min;
    td_s32 csc_clip_max;
} csc_coef_param;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_COEF_H */