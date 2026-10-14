#ifndef __XMRCNN_H__
#define __XMRCNN_H__

#include <stdio.h>
#include <string.h>

#include "xmedia_svp_std.h"
#include "xmedia_svp.h"
#include "svp_nms.h"
#include "xmedia_svp_quantize.h"
#include "svp_core.h"
#include "yolov5.h"
#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RCNN_RECO_FRAME_SIZE 128
#define RCNN_MODEL_NUM 2
#define RCNN_TRACKER_THR 0.1
#define RCNN_CLASSIFY_THR 0.5
#define RCNN_CLASSIFY_COUNT 7
#define RCNN_IOU_THR 0.45

typedef enum {
    SVP_CLASS_RCNN_PERSON = 1,
    SVP_CLASS_RCNN_CAR = 2,
    SVP_CLASS_RCNN_PET = 3,
    SVP_CLASS_RCNN_BIKER = 4,
    SVP_CLASS_RCNN_MOTORER = 5,
    SVP_CLASS_RCNN_TRICYCLER = 6,
    SVP_CLASS_RCNN_MAX
} svp_class_rcnn_type;

typedef struct {
    yolov5_detect_param *detect_info;
    xmedia_u64 resize_phyaddr;
    xmedia_void *resize_viraddr;
    xmedia_bool second_stage_flag;
} svp_rcnn_context;

xmedia_s32 rcnn_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 rcnn_uninit(xmedia_void *context);

xmedia_s32 rcnn_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 rcnn_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 rcnn_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_yolov5_output *output);

#ifdef __cplusplus
}
#endif

#endif