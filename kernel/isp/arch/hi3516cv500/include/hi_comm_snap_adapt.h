/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: hi_comm_snap_adapt.h
 * Author:
 * Create: 2017-06-19
 */

#ifndef __HI_COMM_SNAP_ADAPT_H__
#define __HI_COMM_SNAP_ADAPT_H__

#include "hi_common_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_snap.h"
#include "hi_comm_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef SNAP_TYPE_E hi_snap_type;

typedef struct {
    hi_u32 man_exp_time[PRO_MAX_FRAME_NUM]; /* RW;  Range: [0x0, 0xFFFFFFFF];sensor exposure time (unit: us),
                                                                    it's related to the specific sensor */
    hi_u32 man_sysgain[PRO_MAX_FRAME_NUM]; /* RW;  Range: [0x400, 0xFFFFFFFF];system gain (unit: times,
                                                                    10bit precision), it's related to the specific
                                                                    sensor and ISP dgain range */
} hi_snap_pro_manual_param;

typedef struct {
    hi_u16 pro_exp_step[PRO_MAX_FRAME_NUM]; /* RW; Range:[0x0, 0xFFFF];Professional exposure step
                                                                    (unit: times, 8bit precision) */
} hi_snap_pro_auto_param;

typedef struct {
    hi_operation_mode operation_mode;
    hi_snap_pro_auto_param auto_param;
    hi_snap_pro_manual_param manual_param;
} hi_snap_pro_param;

typedef struct {
    hi_u32  frame_cnt;         /* RW;Set capture frame counts */
    hi_u32  repeat_send_times;  /* [0, 2]; When FE-BE is offline, the first raw frame that is triggered should be sent
                                                    repeatedly to the BE. */

    /* ZSL struct */
    hi_bool zsl;           /* RW;statical attrs */
    hi_u32  frame_depth;  /* buffer depth: [1,8] */
    hi_u32  rollback_ms;  /* RW;Rollback time, unit(ms), invalid when zsl is HI_FALSE  */
    hi_u32  interval;    /* RW;For continuous capture, select frame every intercal frames */
} hi_snap_normal_attr;

typedef struct {
    hi_u32  frame_cnt;          /* [1, PRO_MAX_FRAME_NUM] */
    hi_u32  repeat_send_times;   /* [0, 2];When FE-BE is offline, the first raw frame that is triggered should be sent
                                                    repeatedly to the BE. */
    hi_snap_pro_param pro_param;
} hi_snap_pro_attr;

typedef struct {
    hi_u32 frame_depth;
} hi_snap_user_attr;

typedef struct {
    hi_snap_type snap_type;
    hi_bool load_ccm;    /* HI_TRUE:use CCM of snap_isp_info, HI_FALSE: Algorithm calculate */
    union {
        hi_snap_normal_attr     normal_attr;
        hi_snap_pro_attr        pro_attr;
    };
} hi_snap_attr;

typedef struct {
    hi_bool enable;
    hi_u32 param_num;
    ISP_NR_AUTO_ATTR_S *nr_auto_attr;
} hi_isp_pro_bnr_param;

typedef struct {
    hi_bool enable;
    hi_u32 param_num;
    ISP_SHARPEN_AUTO_ATTR_S *shp_auto_attr;
} hi_isp_pro_sharpen_param;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


