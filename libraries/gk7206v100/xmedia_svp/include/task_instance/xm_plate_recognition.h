#ifndef __XM_PLATE_RECO_H__
#define __XM_PLATE_RECO_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_VEHICLE_DETECT_WIDTH 384
#define SVP_VEHICLE_DETECT_HIGHT 224

#define SVP_PLATE_DETECT_WIDTH 512
#define SVP_PLATE_DETECT_HEIGHT 288
#define SVP_PLATE_RECO_WIDTH 192
#define SVP_PLATE_RECO_HIGHT 48
#define XMEDAI_SVP_PLATE_MAX_NUM 8
#define SVP_PLATE_STR_NUM 73
#define SVP_PLATE_T_STRIDE 21 // tensor跨度

#define SVP_PLATE_CHAR_MIN 7
#define SVP_PLATE_CHAR_MAX 8

#define SVP_PLATE_COLOR_THRES 0.5f
#define SVP_PLATE_REPORT_TIME ((120) * (30)) // 120 秒

#define SVP_PLATE_MIN_WIDTH 85
typedef struct {
    xmedia_s32             track_id;
    xmedia_svp_rect        rect;
    xmedia_svp_plate_color color;
    xmedia_float           color_score;
    xmedia_float           char_score;
    xmedia_char            reco_char[XMEDIA_SVP_PLATE_CHAR_MAX_NUM];
} svp_plate_info;

typedef struct {
    svp_plate_info base_info;
    xmedia_u8      loss_cnt;
    xmedia_u32     report_cnt;
    xmedia_u32     exist_cnt;
    xmedia_bool    is_report;
} svp_plate_status;

typedef struct svp_plate_status_node {
    svp_plate_status             data;
    struct svp_plate_status_node *next;
} svp_plate_status_node;

typedef struct {
    xmedia_s32   index;
    xmedia_float char_score;
} svp_plate_char_t;

typedef struct {
    xmedia_s32       num;
    svp_plate_char_t char_arr[SVP_PLATE_T_STRIDE];
} svp_plate_char_vec;

typedef struct {
    xmedia_u8   char_num;
    xmedia_char reco_char[SVP_PLATE_T_STRIDE];
} svp_plate_reco_char;

typedef struct {
    xmedia_svp_point point_arry[SVP_PLATE_KPS];
} svp_plate_keypoint;

typedef struct {
    xmedia_npu_model       *reco_model;
    xmedia_npu_model       *color_model;
    xmedia_video_frame_info reco_frame;
    xmedia_void            *frame_viraddr;
} svp_plate_reco_info;

typedef struct {
    yolov5_detect_param     param;
    xmedia_void             *frame_viraddr;
    xmedia_video_frame_info detect_frame;
} svp_plate_detect_info;

typedef struct {
    svp_plate_detect_info *detect_info;
    svp_plate_reco_info   *reco_info;
    svp_plate_detect_info *veh_info;
    xmedia_u32             node_num;
    svp_plate_status_node  *head;
    svp_plate_status_node  *tail;
    xmedia_u32             plate_min_width;
    xmedia_u32             report_count;
    xmedia_float           report_thres;
    xmedia_svp_zone        detect_zone;
} svp_plate_information;

typedef struct {
    svp_plate_information info;
    xmedia_void *priv;
} svp_plate_context;

xmedia_s32 plate_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 plate_recognition_uninit(xmedia_void *context);

xmedia_s32 plate_recognition_set_attr(xmedia_void *context, const xmedia_svp_plate_reco_attr *task_attr);

xmedia_s32 plate_recognition_get_attr(xmedia_void *context, xmedia_svp_plate_reco_attr *task_attr);

xmedia_s32 plate_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
                            xmedia_svp_plate_reco_output *output);

#ifdef __cplusplus
}
#endif

#endif

