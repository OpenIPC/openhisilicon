#ifndef __XM_MULTI_DETECT_H__
#define __XM_MULTI_DETECT_H__

#include "svp_comm.h"
#include "xm_rcnn.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    yolov5_detect_param *car_detect_info;
    yolov5_detect_param *person_detect_info;
    yolov5_detect_param *nmv_detect_info;
    yolov5_detect_param *face_detect_info;
    yolov5_detect_param *pet_detect_info;
    yolov5_detect_param *pkg_detect_info;
} svp_multi_detect_information;

typedef struct {
    xmedia_npu_model model;
    xmedia_u64 resize_phyaddr;
} svp_multi_rcnn_info;

typedef struct {
    svp_movement movement;
    svp_tracklet tracklet;
    xmedia_bool tarck_id_arry[XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT];
    xmedia_s32 tarck_id_grow;
} svp_multi_base_info;

typedef struct {
    xmedia_bool person_flag;
    xmedia_bool car_flag;
    xmedia_bool nmv_flag;
    xmedia_bool face_flag;
    xmedia_bool pet_flag;
    xmedia_bool pkg_flag;
    xmedia_bool rcnn_flag;
} svp_multi_use_count;

typedef struct {
    svp_multi_detect_information info;
    svp_multi_rcnn_info *rcnn_info;
    svp_multi_base_info *base_info;
    svp_multi_use_count use_count;
} svp_multi_detect_context;

xmedia_s32 multi_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 multi_detect_uninit(xmedia_void *context);

xmedia_s32 multi_detect_set_attr(xmedia_void *context, const xmedia_svp_multi_detect_attr *task_attr);

xmedia_s32 multi_detect_get_attr(xmedia_void *context, xmedia_svp_multi_detect_attr *task_attr);

xmedia_s32 multi_detect_process(xmedia_void *context, const xmedia_svp_task_input *input,
                                xmedia_svp_yolov5_output *output);

#ifdef __cplusplus
}
#endif

#endif
