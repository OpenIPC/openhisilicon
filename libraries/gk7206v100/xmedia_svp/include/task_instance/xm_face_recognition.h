#ifndef __XMFACE_RECOGNITION_H__
#define __XMFACE_RECOGNITION_H__

#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FR_ADAPT_SOBEL_RESIZE 64
#define FR_ADAPT_SOBEL_INPUT 48
#define FR_IVE_SOBLE_DEFAULT 20
/** fr face pose区间 */
#define FR_MIN_FACE_POSE 1.0f
#define FR_MAX_FACE_POSE 200.0f
#define FR_DEFAULT_FACE_POSE 120.0f
/** 关键点默认阈值 */
#define FR_DEFAULT_KEYPOINT_THRES 0.8f
/** 最大人脸面积 */
#define FR_MAX_FACE  360 * 360
/** 人脸识别删除质量模型暂定3模型 */
#define FR_MODELS_NUM 3
/** 计算人脸亮度 人脸框最大个数 */
#define FR_MAX_CAL_LUMA_TECT_NUM 10
/** 人脸luma区间 */
#define FR_MIN_LUMA 20
#define FR_MAX_LUMA 200
/** ive sobel区间 */
#define FR_MIN_IVE_SOBLE 0
#define FR_MAX_IVE_SOBLE 200
/** 人脸luma默认值 */
#define FR_DEFAULT_LUMA  60
/** 人脸识别最大数量 */
#define FR_MAX_NUM 10
/** 模型分辨率  */
#define FACE_ALIGN_OUTPUT_IMG_WIDE 112
#define FACE_ALIGN_OUTPUT_IMG_HIGH 112
#define FACE_ALIGN_INPUT_IMG_WIDE 1920
#define FACE_ALIGN_INPUT_IMG_HIGH 1080
#define FACE_REIZE_OUTPUT_IMG_WIDE 48
#define FACE_RESIZE_OUTUT_IMG_HIGH 48
#define FACE_RECOGN_RESULT_VECTOR_OS 512
#define FACE_RECOGN_RESULT_VECTOR_BZ 256
#define FACE_S_TO_MS 1000LLU
#define FACE_NS_TO_MS 1000000LLU
/** 人脸属性结构化清理时间  */
#define FACE_TIMEOUT_RECONFIRM 3000LLU
/** 人脸属性类别确定计数  */
#define FACE_CONFIRM_COUNT 3

typedef struct {
    xmedia_svp_point point_arry[XMEDIA_SVP_FR_KEYPOINT_NUM];
} svp_keypoint_point;

typedef struct {
    xmedia_npu_model npu_model;
    xmedia_float thres;
} fr_module_info;

typedef struct {
    xmedia_s32 sobel_thres;
    xmedia_ive_sobel_ctrl_s stSobelCtrl;
    xmedia_ive_dst_image_s dst_img;
} ive_sobel_attr;

typedef struct {
    xmedia_svp_point keypoint_arry[XMEDIA_SVP_FR_KEYPOINT_NUM];
    xmedia_svp_rect rect;
    xmedia_s32 tracker_id;
    xmedia_s32 tracker_age;
    xmedia_float score;
    xmedia_float roll;
    xmedia_float yaw;
    xmedia_float pitch;
    xmedia_bool is_pose_filter_out;
    xmedia_float pupil_distance_thre;
} svp_keypoint_targets;

typedef struct {
    svp_keypoint_targets keypoint_targets[FR_MAX_NUM];
    xmedia_u32 after_filter_num;
} svp_keypoint_result_info;

typedef struct {
    xmedia_bool used_flag;
    xmedia_u32 tracker_age;
    xmedia_u32  lost_num;
    xmedia_s8 emotion_class;
    xmedia_u64  retime;
    xmedia_float face_score;
} svp_face_history_result;

typedef struct {
    xmedia_svp_task_cfg cfg;
    yolov5_detect_param *detect_info;
    fr_module_info *keypoint_info;
    fr_module_info *recognition_info;
    ive_sobel_attr *sobel_attr;
    xmedia_svp_face_pose_attr pose_attr;
    xmedia_u64 luma_thres;
    xmedia_u64 keypoint_phyaddr;
    xmedia_u64 resize_phyaddr;
    xmedia_u64 sobel_phyaddr;
    xmedia_u64 align_phyaddr;
    xmedia_void *align_viraddr;
    svp_face_history_result fva_history_arry[SVP_MAX_LOST_COUNT * XMEDIA_SVP_MAX_TARGET_NUM];
    xmedia_void *priv;
} svp_fr_context;

xmedia_s32 xmface_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 xmface_recognition_uninit(xmedia_void *context);

xmedia_s32 xmface_recognition_set_attr(xmedia_void *context, const xmedia_svp_fr_attr *task_attr);

xmedia_s32 xmface_recognition_get_attr(xmedia_void *context, xmedia_svp_fr_attr *task_attr);

xmedia_s32 xmface_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
    const xmedia_void *output);

#ifdef __cplusplus
}
#endif

#endif