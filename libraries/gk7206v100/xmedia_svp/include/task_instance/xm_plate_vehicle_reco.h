#ifndef __XM_PLATE_VEHICLE_RECO_H__
#define __XM_PLATE_VEHICLE_RECO_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_VEHICLE_DETECT_WIDTH 384
#define SVP_VEHICLE_DETECT_HIGHT 224
#define SVP_VEHICLE_RECO_WIDTH 224
#define SVP_VEHICLE_RECO_HEIGHT 224

#define SVP_PLATE_DETECT_WIDTH 512
#define SVP_PLATE_DETECT_HEIGHT 288
#define SVP_PLATE_RECO_WIDTH 192
#define SVP_PLATE_RECO_HIGHT 48
#define XMEDAI_SVP_PLATE_MAX_NUM 8
#define SVP_PLATE_STR_NUM 73
#define SVP_PLATE_T_STRIDE 21

#define SVP_PLATE_CHAR_MIN 7
#define SVP_PLATE_CHAR_MAX 8

#define SVP_PLATE_COLOR_THRES 0.5f
#define SVP_PLATE_REPORT_TIME ((120) * (30)) // 120 秒

#define SVP_VEHICLE_TYPE_CLASS 6
#define SVP_VEHICLE_COLOR_CLASS 6
#define SVP_VEHICLE_TYPE_THRES 0.5f
#define SVP_VEHICLE_COLOR_THRES 0.5f
#define SVP_VEHICLE_STATUS_CHANGE_CNT 5

typedef struct {
    xmedia_s32               track_id;
    xmedia_svp_rect          veh_rect;
    xmedia_svp_vehicle_color veh_color;
    xmedia_float             veh_color_score;
    xmedia_svp_vehicle_type  veh_type;
    xmedia_float             veh_type_score;
    xmedia_svp_rect          plate_rect;
    xmedia_svp_plate_color   plate_color;
    xmedia_float             plate_color_score;
    xmedia_float             char_score;
    xmedia_char              reco_char[XMEDIA_SVP_PLATE_CHAR_MAX_NUM];
} svp_plate_vehicle_info;

typedef struct {
    svp_plate_vehicle_info base_info;
    xmedia_u8      loss_cnt;
    xmedia_u32     report_cnt;
    xmedia_u32     exist_cnt;
    xmedia_bool    is_report;
} svp_plate_vehicle_status;

typedef struct svp_plate_vehicle_status_node {
    svp_plate_vehicle_status             data;
    struct svp_plate_vehicle_status_node *next;
} svp_plate_vehicle_status_node;

typedef struct {
    xmedia_s32   index;
    xmedia_float char_score;
} svp_plate_vehicle_char_t;

typedef struct {
    xmedia_s32       num;
    svp_plate_vehicle_char_t char_arr[SVP_PLATE_T_STRIDE];
} svp_plate_vehicle_char_vec;

typedef struct {
    xmedia_u8   char_num;
    xmedia_char reco_char[SVP_PLATE_T_STRIDE];
} svp_plate_vehicle_reco_char;

typedef struct {
    xmedia_svp_point point_arry[SVP_PLATE_KPS];
} svp_plate_vehicle_keypoint;

typedef struct {
    xmedia_npu_model       *reco_model;
    xmedia_npu_model       *color_model;
    xmedia_video_frame_info reco_frame;
    xmedia_void            *frame_viraddr;
} svp_plate_vehicle_reco_info;

typedef struct {
    yolov5_detect_param     param;
    xmedia_void             *frame_viraddr;
    xmedia_video_frame_info detect_frame;
} svp_plate_vehicle_detect_info;

typedef struct {
    svp_plate_vehicle_detect_info *plate_detect;
    svp_plate_vehicle_detect_info *veh_detect;
    svp_plate_vehicle_reco_info   *plate_reco;
    svp_plate_vehicle_reco_info   *veh_reco;
    xmedia_u32             plate_min_width;
    xmedia_u32             report_count;
    xmedia_float           report_thres;
    xmedia_svp_zone        detect_zone;
    xmedia_u32             node_num;
    svp_plate_vehicle_status_node  *head;
    svp_plate_vehicle_status_node  *tail;
} svp_plate_vehicle_information;

typedef struct {
    svp_plate_vehicle_information info;
    xmedia_void *priv;
} svp_plate_vehicle_context;

xmedia_s32 plate_vehicle_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 plate_vehicle_recognition_uninit(xmedia_void *context);

xmedia_s32 plate_vehicle_recognition_set_attr(xmedia_void *context, const xmedia_svp_plate_vehicle_attr *task_attr);

xmedia_s32 plate_vehicle_recognition_get_attr(xmedia_void *context, xmedia_svp_plate_vehicle_attr *task_attr);

xmedia_s32 plate_vehicle_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
                            xmedia_svp_plate_vehicle_output *output);

#ifdef __cplusplus
}
#endif

#endif

