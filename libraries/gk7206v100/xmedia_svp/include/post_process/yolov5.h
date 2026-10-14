#ifndef __YOLOV5__
#define __YOLOV5__
#include <stdio.h>
#include <string.h>

#include "xmedia_svp_std.h"
#include "xmedia_svp.h"
#include "svp_nms.h"
#include "xmedia_svp_quantize.h"
#include "svp_core.h"
#include "svp_movement.h"
#include "svp_bytetracker.h"

#ifdef __cplusplus
extern "C" {
#endif

// sigmoid 查表法
#define TABLE_SIZE 100000
#define TABLE_MIN   (-100.0f)
#define TABLE_MAX   (100.0f)
#define TABLE_200   (200.0f)
#define UNSIGNED_8_BIT_NUM 256
#define TABLE_500 (500.0f)

// 每个特征中的x、y、w、h、socre长度
#define XYWHF_LEN 5
#define XYWH_LEN 4
#define LAYER_MAX_NUM  10
#define LAYER_MIN_NUM  3
#define LAYER_NUM_FOUR 4

// 每层feature_map对应的anchor
#define ANCHOR_NUM 3

/** 模型输入默认分辨率  */
#define INPUT_DEFAULT_WIDE 640
#define INPUT_DEFAULT_HIGH 360

/** 默认检测阈值 */
#define DEFAULT_DETECT_THRESHOLD 0.45f
/** 默认IOU阈值 */
#define DEFAULT_IOU_THRESHOLD 0.5f
/** 分类器默认阈值*/
#define DEFAULT_CLASSIFIER_THRESHOLD 0.01f
#define DEFAULT_BBOX_NUM 500

/** 模型检测类型数量  */
#define DETECTION_MODEL_SINGLE 1
#define DETECTION_MODEL_DOUBLE 2
#define DETECTION_MODEL_MULTI  4
#define DETECTION_MODEL_SIX 6
#define DETECTION_MODEL_MAX  80

// 稳定框算法系数
#define SVP_IOU_STABLE_THRESH_UPPER   0.85f
#define SVP_IOU_STABLE_THRESH_MIDDLE  0.7f
#define SVP_STABLE_HORIZONTAL_FACTOR 6
#define SVP_STABLE_VERTICAL_FACTOR 5
#define SVP_UNIFORM_FACTOR_A 2
#define SVP_UNIFORM_FACTOR_B 3
#define SVP_STABLE_HISTORY_RATIO 0.8f
#define SVP_STABLE_CURRENT_RATIO 0.2f

typedef struct {
    xmedia_svp_rect rect;
    xmedia_svp_class_type class_type;
} svp_stable_rect;

typedef struct {
    svp_stable_rect stable_targets[XMEDIA_SVP_MAX_TARGET_NUM];
    xmedia_u32 num;
} svp_stable_box;

typedef struct {
    xmedia_svp_point anchors[LAYER_MAX_NUM][ANCHOR_NUM];   // 锚点值
    xmedia_s32 layer_num;                                 // 锚点层数
} xmedia_svp_detect_anchors;

typedef struct {
    xmedia_u32 w;                            // 模型支持的图像宽度
    xmedia_u32 h;                            // 模型支持的图像高度
    xmedia_u32 num;                          // 模型的检测类别数量
    xmedia_u32 feature[LAYER_MAX_NUM];       // 每个feature_map的大小
    xmedia_u32 output_num;                   // npu输出所有的特征数量
    xmedia_float thres_desig;                // 检测阈值的反sigmoid值
    quanlize_param quanlize[LAYER_MAX_NUM];  // 反量化需要的参数
    xmedia_npu_model model;                  // npu参数
    xmedia_svp_alg_type type;                // 算法类型
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.55f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.01f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
    xmedia_bool smart_venc_enable;
    xmedia_bool smart_ae_enable;
    xmedia_bool smart_venc_array[XMEDIA_SVP_MAX_VENC_CHN_NUM];
    xmedia_bool smart_ae_array[XMEDIA_SVP_MAX_VI_PIPE_NUM];
    svp_stable_box record_result;            // 记录历史框,用于稳定算法
    xmedia_svp_detect_anchors anchors;       // 锚点值
    xmedia_u64 cost_time;                    // process接口耗时
    xmedia_bool dup_flag;                    // 是否已加载完模型
    xmedia_bool aov_flag;                    // 是否为aov模型
    xmedia_bool aov_only_target;             // 是否仅判断有无目标
    svp_movement movement;                   // 静止过滤相关参数
    svp_tracklet tracklet;
    xmedia_bool tarck_id_arry[XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT]; // 追踪id数组
    xmedia_s32  tarck_id_grow;                                                 // 追踪id增加
    xmedia_void *private_data;               // 实例私有数据
    xmedia_float *sigmoid_table;             // sigmoid查表
    xmedia_float *sigmoid_dequantize_table;
    xmedia_float *sigmoid_dequantize_table_x2;
    xmedia_float *sigmoid_dequantize_table_x6;
    xmedia_float *sigmoid_dequantize_table_x2_sq;
    svp_base_result *bbox;
} yolov5_detect_param;

xmedia_void set_default_yolov5_detect_param(yolov5_detect_param *param);

xmedia_s32 detect_process(yolov5_detect_param *param, const xmedia_video_frame_info *input_image,
                                xmedia_svp_yolov5_output *result, const xmedia_svp_detect_anchors anchors_yaml);
xmedia_s32 detect_init(yolov5_detect_param *param, const xmedia_svp_alg_type type,
                          const xmedia_svp_modules *model, xmedia_s32 layer_num);
xmedia_s32 detect_deinit(yolov5_detect_param *param);

xmedia_s32 svp_get_detect_result_yolov5_bz(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                            const xmedia_svp_detect_anchors anchors_yaml);

#ifdef __cplusplus
}
#endif

#endif
