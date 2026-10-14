#ifndef __XM_VEHICLE_RECO_H__
#define __XM_VEHICLE_RECO_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_VEHICLE_DETECT_WIDTH 384
#define SVP_VEHICLE_DETECT_HIGHT 224
#define SVP_VEHICLE_RECO_WIDTH 224
#define SVP_VEHICLE_RECO_HEIGHT 224

#define SVP_VEHICLE_TYPE_CLASS 6
#define SVP_VEHICLE_COLOR_CLASS 6

#define SVP_VEHICLE_TYPE_THRES 0.5f
#define SVP_VEHICLE_COLOR_THRES 0.5f
#define SVP_VEHICLE_STATUS_CHANGE_CNT 5
typedef struct {
    xmedia_s32               track_id;
    xmedia_svp_rect          rect;
    xmedia_float             color_scr;
    xmedia_float             type_scr;
    xmedia_svp_vehicle_color color;
    xmedia_svp_vehicle_type  type;
} svp_vehicle_info;

typedef struct {
    svp_vehicle_info base_info;
    svp_vehicle_info tmp_info;
    xmedia_u8        tmp_type_cnt;
    xmedia_u8        tmp_color_cnt;
    xmedia_u8        loss_cnt;
    xmedia_u32       report_cnt;
    xmedia_u32       exist_type_cnt;
    xmedia_u32       exist_color_cnt;
    xmedia_bool      is_report;
} svp_vehicle_status;

typedef struct svp_vehicle_status_node {
    svp_vehicle_status data;
    struct svp_vehicle_status_node *next;
} svp_vehicle_status_node;

typedef struct {
    xmedia_npu_model *reco_model;
    xmedia_video_frame_info reco_frame;
    xmedia_void *reco_viraddr;
    xmedia_u32 node_num;
    svp_vehicle_status_node *head;
    svp_vehicle_status_node *tail;
} svp_vehicle_reco_info;

typedef struct {
    yolov5_detect_param     param;
    xmedia_video_frame_info detect_frame;
} svp_vehicle_detect_info;

typedef struct {
    svp_vehicle_detect_info *detect_info;
    svp_vehicle_reco_info   *reco_info;
    xmedia_u32             report_count;
    xmedia_float           report_thres;
    xmedia_svp_zone        detect_zone;
} svp_vehicle_information;

typedef struct {
    svp_vehicle_information info;
    xmedia_void *priv;
} svp_vehicle_context;

xmedia_s32 vehicle_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 vehicle_recognition_uninit(xmedia_void *context);

xmedia_s32 vehicle_recognition_set_attr(xmedia_void *context, const xmedia_svp_vehicle_reco_attr *task_attr);

xmedia_s32 vehicle_recognition_get_attr(xmedia_void *context, xmedia_svp_vehicle_reco_attr *task_attr);

xmedia_s32 vehicle_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
                            xmedia_svp_vehicle_reco_output *output);

#ifdef __cplusplus
}
#endif

#endif

