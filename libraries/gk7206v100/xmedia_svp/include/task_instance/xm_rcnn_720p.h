#ifndef __XMRCNN_720_H__
#define __XMRCNN_720_H__

#include "xm_rcnn.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INPUT_MID_WIDE 1280
#define INPUT_MID_HIGH 720

typedef struct {
    yolov5_detect_param *detect_info;
    xmedia_u64 resize_phyaddr;
    xmedia_void *resize_viraddr;
    xmedia_bool second_stage_flag;
} svp_rcnn_720p_context;

xmedia_s32 rcnn_720p_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 rcnn_720p_uninit(xmedia_void *context);

xmedia_s32 rcnn_720p_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 rcnn_720p_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 rcnn_720p_process(xmedia_void *context, const xmedia_svp_task_input *input,
                             xmedia_svp_yolov5_output *output);

#ifdef __cplusplus
}
#endif

#endif