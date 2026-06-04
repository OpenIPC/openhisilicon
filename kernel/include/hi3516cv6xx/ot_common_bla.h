/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_COMMON_BLA_H
#define OT_COMMON_BLA_H

#include "ot_common_video.h"
#include "ot_type.h"
#include "ot_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    td_float x;
    td_float y;
} ot_point_f;

typedef struct {
    ot_point_f *master_coords;
    ot_point_f *slave_coords;
} ot_bla_calibration_info;

#define OT_ERR_BLA_NULL_PTR         OT_DEFINE_ERR(OT_ID_BLA, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
#define OT_ERR_BLA_ILLEGAL_PARAM    OT_DEFINE_ERR(OT_ID_BLA, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
#define OT_ERR_BLA_NOT_SUPPORT      OT_DEFINE_ERR(OT_ID_BLA, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)

#ifdef __cplusplus
}
#endif
#endif /* OT_COMMON_BLA_H */