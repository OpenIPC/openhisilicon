#ifndef __SVP_BYTETRACKER__
#define __SVP_BYTETRACKER__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdlib.h>

#include "xmedia_svp_std.h"
#include "xmedia_svp.h"
#include "xmedia_svp_quantize.h"


#ifdef __cplusplus
extern "C" {
#endif

#define SVP_TRACK_EPSILON 0.000001f

// bytetracker
#define SVP_STD_POSITION                  (1.0f / 20.0f)
#define SVP_STD_VELOCITY                  (1.0f / 160.0f)
#define SVP_MAX_LOST_COUNT                30
#define SVP_TRACKER_HIGH_SCORE_THRESHOLD 0.5f
#define SVP_ACTIVATED_TRACKER_THRESHOLD   0.1f
#define SVP_TRACKED_TRACKER_THRESHOLD     0.1f
#define SVP_UNACTIVATED_TRACKER_THRESHOLD 0.1f
#define SVP_TRACK_AGE_THRESHOLD 3
#define SVP_TRACK_AGE_LIMIT 240

// 卡尔曼滤波矩阵
#define SVP_KALMAN_FILTER_X 8
#define SVP_KALMAN_FILTER_Y 4
#define SVP_KALMAN_FILTER_STATE 1


typedef struct {
    xmedia_float x; /* 中心点坐标x */
    xmedia_float y; /* 中心点坐标y */
    xmedia_float a; /* 宽高比 */
    xmedia_float h; /* 高度 */
} svp_kalman_measure;

typedef struct {
    xmedia_float state[SVP_KALMAN_FILTER_X][SVP_KALMAN_FILTER_STATE];
    xmedia_float R[SVP_KALMAN_FILTER_Y][SVP_KALMAN_FILTER_Y]; // 测量噪声矩阵 不需要保存
    xmedia_float K[SVP_KALMAN_FILTER_X][SVP_KALMAN_FILTER_Y]; // 卡尔曼增益
    xmedia_float H[SVP_KALMAN_FILTER_Y][SVP_KALMAN_FILTER_X]; // 观测矩阵
    xmedia_float P[SVP_KALMAN_FILTER_X][SVP_KALMAN_FILTER_X]; // 协方差矩阵
    xmedia_float F[SVP_KALMAN_FILTER_X][SVP_KALMAN_FILTER_X]; // 状态转移矩阵
    xmedia_float Q[SVP_KALMAN_FILTER_X][SVP_KALMAN_FILTER_X]; // 运动噪声矩阵 不需要保存
} svp_kalman_filter;

typedef enum {
    SVP_TRACKER_STATE_NEW = 0,     // 新轨迹
    SVP_TRACKER_STATE_TRACKED,     // 跟踪成功
    SVP_TRACKER_STATE_LOST,        // 未跟踪成功
    SVP_TRACKER_STATE_REMOVED,
    SVP_TRACKER_STATE_UNDEFINED
} svp_tracker_state;

typedef struct {
    svp_tracker_state state;
    xmedia_svp_class_type class_type;
    xmedia_float score;
    xmedia_float classfier_score;
    xmedia_s32 id;
    xmedia_u8 age;
    xmedia_float iou;
    xmedia_u8 lost_count;
    xmedia_svp_rect rect;
    xmedia_svp_rect predicted_rect;
    svp_kalman_filter kalman_filter;
    xmedia_svp_keypoint kpt[MAX_DETECT_KEYPOINT_NUM]; // 关键点坐标
} svp_bytetracker;

typedef struct {
    xmedia_bool enable;
    xmedia_float high_score_thres;
    xmedia_float activated_tracker_thres;
    xmedia_float tracked_tracker_thres;
    xmedia_float unactivated_tracker_thres;
    xmedia_u8 track_age_thres;
    xmedia_u8 num;
    svp_bytetracker* tracker;
} svp_tracklet;

typedef struct {
    xmedia_svp_alg_type alg_type;                     // 检测类别
    xmedia_svp_class_type class_type;                 // 检测结果类别
    xmedia_float detect_score;                        // 检测模型结果置信度
    xmedia_float classfier_score;                     // 分类器结果置信度
    xmedia_s32 tracker_id;                            // 检测结果追踪id
    xmedia_u32 tracker_age;                           // 检测结果追踪age
    xmedia_svp_rect rect;                             // 检测结果目标框
    xmedia_svp_keypoint kpt[MAX_DETECT_KEYPOINT_NUM]; // 关键点坐标
} xmedia_svp_tracker_single_result;

typedef struct {
    xmedia_u32 target_num;
    xmedia_svp_tracker_single_result tracker_single[XMEDIA_SVP_MAX_TARGET_NUM];
} xmedia_svp_tracker_result;


xmedia_s32 svp_detect_bytetracker(xmedia_svp_tracker_result* result, svp_tracklet* tracklet,
                                             xmedia_s32* tracker_idx, xmedia_bool* id_flag);

/*
xmedia_s32 assignment_optimal(xmedia_s32* assignment, xmedia_float* dist_matrix_in, xmedia_u32 row,
    xmedia_u32 col);

xmedia_s32 svp_bytetracker_set_attr(detect_param *param, const xmedia_svp_bytetracker_attr attr);

xmedia_s32 svp_bytetracker_get_attr(detect_param *param, xmedia_svp_bytetracker_attr *attr);
*/


#ifdef __cplusplus
}
#endif

#endif
