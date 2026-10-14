#ifndef __XM_ADAS_H__
#define __XM_ADAS_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_LANE_ROW_SCORE_NUM 100
#define SVP_LANE_COL_SCORE_NUM 50
#define SVP_LANE_INPUT_IMG_WIGTH 800
#define SVP_LANE_INPUT_IMG_HIGHT 532
#define SVP_LANE_CROP_IMG_HIGHT 320
#define SVP_PLATE_IMG_WIGTH 512
#define SVP_PLATE_IMG_HIGHT 288
#define SVP_FIT_ORDER 3
#define SVP_FIT_MAX_RESIDUALS 15
#define SVP_FIT_MIN_NUM 7

typedef struct {
    xmedia_npu_model npu_model;
} svp_lane_info;

typedef struct {
    svp_lane_info *lane_info;
    yolov5_detect_param *plate_detect_info;
    yolov5_detect_param *car_detect_info;
    yolov5_detect_param *person_detect_info;
    yolov5_detect_param *nmv_detect_info;
    xmedia_svp_adas_distance_attr dist_attr;
} svp_adas_information;

typedef struct {
    svp_adas_information info;
    xmedia_void *priv;
} svp_adas_context;

xmedia_s32 adas_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 adas_uninit(xmedia_void *context);

xmedia_s32 adas_set_attr(xmedia_void *context, const xmedia_svp_adas_attr *task_attr);

xmedia_s32 adas_get_attr(xmedia_void *context, xmedia_svp_adas_attr *task_attr);

xmedia_s32 adas_process(xmedia_void *context, const xmedia_svp_task_input *input,
                            xmedia_svp_adas_result *output);

#ifdef __cplusplus
}
#endif

#endif
