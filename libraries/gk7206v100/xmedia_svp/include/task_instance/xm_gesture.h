#ifndef __XM_GESTURE_H__
#define __XM_GESTURE_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_SVP_GESTURE_MODEL_NUM 2
#define GESTURE_THRES 0.85// 手势识别阈值
#define GESTURE_CLASS 18 // 手势类别数量

/* 模型输出序号 */
#define GESTURE_OK 5
#define GESTURE_PALM 8
#define GESTURE_THUMB 14
#define GESTURE_V 17
/* 模型输入分辨率 */
#define INPUT_BIG_WIDE 1920
#define INPUT_BIG_HIGH 1080
#define GESTURE_RESIZE_OUTPUT_IMG_WIDE_192 192
#define GESTURE_RESIZE_OUTPUT_IMG_HIGH_192 192
#define GESTURE_BEST_OUTPUT_IMG_WIDE 96
#define GESTURE_BEST_OUTPUT_IMG_HIGH 96
#define PI 3.1415926
#define GESTURE_KEYPOINT_LEN 2
#define GESTURE_MAX_STORE_NUM 100

typedef struct {
    xmedia_float x_min, y_min, x_max, y_max;
} BoundingBox;

typedef struct {
    xmedia_npu_model npu_model;
} svp_gesture_info;

typedef struct {
    yolov8_detect_param *detect_info;
    svp_gesture_info *gesture_info;
    xmedia_void* priv;
} svp_gesture_context;

xmedia_s32 gesture_set_attr(xmedia_void* context, const xmedia_svp_yolov8_attr* task_attr);

xmedia_s32 gesture_get_attr(xmedia_void* context, xmedia_svp_yolov8_attr* task_attr);

xmedia_s32 gesture_uninit(xmedia_void* context);

xmedia_s32 gesture_init(xmedia_void* context, const xmedia_svp_task_cfg cfg);

xmedia_s32 gesture_process(xmedia_void* context, const xmedia_svp_task_input* input, xmedia_svp_gesture_result* output);

#ifdef __cplusplus
}
#endif

#endif
