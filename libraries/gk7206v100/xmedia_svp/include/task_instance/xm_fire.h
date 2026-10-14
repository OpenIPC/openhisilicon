#ifndef __XM_FIRE_H__
#define __XM_FIRE_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PROTO_CHANNEL 32
#define PROTO_HEIGHT 96
#define PROTO_WEIGHT 160
#define DFL_LEN 16

#define FIRE_FRAME_MAX_NUM 2
#define FIRE_SMOKE_CLASS 2
#define THRES_MIN_VAL 0
#define THRES_MAX_VAL 255
#define FIRE_THRES 10
#define SMOKE_THRES 5
#define SMOKE_RATIO_THRES 0.1f
#define TIMEOUT_THRES 1.0f
#define SMOKE_COUNT_TIMES ((1) / (2))

typedef struct {
    xmedia_ive_sub_ctrl_s stSubCtrl;
    xmedia_ive_dst_image_s pre_sub_img;
    xmedia_ive_dst_image_s dst_sub_img;
    xmedia_ive_dst_image_s dst_thre_img;
    xmedia_ive_dst_image_s smoke_thre_img;
} ive_sub_attr;

typedef struct {
    yolov8_detect_param* fire_info;
    xmedia_svp_yolov8_output pre_result;
    xmedia_svp_fire_attr fire_attr;
    xmedia_u32 fire_pixel_count[FIRE_FRAME_MAX_NUM];
    xmedia_u32 smoke_pixel_count[FIRE_FRAME_MAX_NUM];

    xmedia_bool diff_flag;
    xmedia_bool fire_alarm;
    xmedia_bool smoke_alarm;
    xmedia_bool smoke_gray;

    xmedia_u32 mask_index;

    xmedia_u32 smoke_frame_count;
    xmedia_u32 smoke_bigger_frame;
    xmedia_u32 smoke_loss_count;
    xmedia_u32 smoke_apear_count;
    xmedia_float smoke_change_ratio;

    xmedia_u32 fire_loss_count;
    xmedia_float fire_change_ratio;
    xmedia_float fire_ratio_sum;

    time_t fire_update_time; // the latest time of fire occur
    time_t smoke_update_time; // the latest time of smoke occur
} svp_fire_information;

typedef struct {
    svp_fire_information info;
    xmedia_void *priv;
} svp_fire_context;

xmedia_s32 fire_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 fire_uninit(xmedia_void *context);

xmedia_s32 fire_set_attr(xmedia_void *context, const xmedia_svp_fire_attr *task_attr);

xmedia_s32 fire_get_attr(xmedia_void *context, xmedia_svp_fire_attr *task_attr);

xmedia_s32 fire_process(xmedia_void *context, const xmedia_svp_task_input *input, xmedia_svp_yolov8_output *output);

#ifdef __cplusplus
}
#endif

#endif

