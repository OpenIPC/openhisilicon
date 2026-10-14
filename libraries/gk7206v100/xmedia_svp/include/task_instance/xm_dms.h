#ifndef __XM_DMS_H__
#define __XM_DMS_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif
#define DMS_EPSILON 1e-6f
#define DMS_DETECTION_MODEL 3
#define DMS_KEYPOINT 68
#define DMS_INT_FRAME 10
#define DMS_EYES_THRE 0.25
#define DMS_OUT_MOUTH_THRES 1.3
#define DMS_IN_MOUTH_THRES 0.7
#define DMS_IOU_THRES 0.1

typedef struct {
    xmedia_bool close_eyes;
    xmedia_bool yawn;
    xmedia_bool phone;
    xmedia_bool cigar;
} svp_dms_flag;

typedef struct {
    xmedia_u8 close_eyes_frame;
    xmedia_u8 yawn_frame;
    xmedia_u8 phone_frame;
    xmedia_u8 cigar_frame;
    xmedia_u8 id_frame;
} svp_dms_frame;

typedef struct {
    xmedia_float left_ear;                           // 左眼宽高比
    xmedia_float right_ear;                          // 右眼宽高比
    xmedia_float out_mar;                            // 外嘴宽高比
    xmedia_float in_mar;                             // 内嘴宽高比
    xmedia_float pitch;                              // 俯仰角
    xmedia_float yaw;                                // 偏航角
    xmedia_float roll;                               // 翻滚角
} svp_dms_state;

typedef struct {
    svp_dms_flag flag;
    svp_dms_frame frame;
    svp_dms_state state;
    xmedia_s32 id;
} svp_dms_alarm_report;

typedef struct {
    yolov8_detect_param *detect_info;
    svp_dms_alarm_report alarm_report;
    xmedia_u8 warn_frame;                           // 警告帧数
    xmedia_float iou_thres;                         // 相交阈值，建议值0.1f
} svp_dms_context;

xmedia_s32 dms_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 dms_uninit(xmedia_void *context);

xmedia_s32 dms_set_attr(xmedia_void *context, const xmedia_svp_dms_attr *task_attr);

xmedia_s32 dms_get_attr(xmedia_void *context, xmedia_svp_dms_attr *task_attr);

xmedia_s32 dms_process(xmedia_void *context, const xmedia_svp_task_input *input, xmedia_svp_dms_output *output);

#ifdef __cplusplus
}
#endif

#endif

