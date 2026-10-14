#ifndef __SVP_NMS_H__
#define __SVP_NMS_H__
#include <math.h>
#include <stdlib.h>
#include "xmedia_svp_std.h"
#include "xmedia_svp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_EPSILON 0.000001f

typedef struct {
    xmedia_svp_rect rect;
    xmedia_float detect_score;
    xmedia_svp_class_type class_type;
} svp_base_result;

typedef struct {
    xmedia_svp_rect rect;
    xmedia_float detect_score;
    xmedia_svp_class_type class_type;
    xmedia_svp_keypoint kpt[MAX_DETECT_KEYPOINT_NUM];
} svp_base_yolov8_result;

xmedia_s32 det_nms(svp_base_result* vec_bbox, xmedia_u32 vec_bbox_len,
                   xmedia_float threshold, xmedia_u32* picked_bbox_len);

xmedia_float iou(svp_base_result* box1, svp_base_result* box2);

xmedia_s32 det_nms_cross(xmedia_svp_detect_result* vec_bbox, xmedia_u32 vec_bbox_len,
                         xmedia_float threshold, xmedia_u32* picked_bbox_len);

#ifdef __cplusplus
}
#endif

#endif
