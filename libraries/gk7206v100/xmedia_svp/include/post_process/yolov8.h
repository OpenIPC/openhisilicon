#ifndef __YOLOV8__
#define __YOLOV8__

#include <stdio.h>
#include <string.h>

#include "xmedia_svp.h"
#include "xmedia_svp_std.h"
#include "xmedia_svp_quantize.h"

#include "svp_nms.h"
#include "svp_core.h"
#include "yolov5.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PEOPLE_KEYPOINT_LEN 17
#define SVP_PLATE_KPS 4

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
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.01f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
    svp_stable_box record_result;            // 记录历史框,用于稳定算法
    xmedia_bool dup_flag;                    // 是否已加载完模型
    svp_movement movement;                   // 静止过滤相关参数
    svp_tracklet tracklet;
    xmedia_bool tarck_id_arry[XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT]; // 追踪id数组
    xmedia_s32  tarck_id_grow;                                                 // 追踪id增加
    xmedia_u8 keypoint_len;                  // 关键点数量
    xmedia_void *private_data;               // 实例私有数据
    svp_base_yolov8_result *bbox;
} yolov8_detect_param;

xmedia_void set_default_yolov8_detect_param(yolov8_detect_param *param);

xmedia_s32 yolov8_init(yolov8_detect_param *param, const xmedia_svp_modules *model);

xmedia_s32 yolov8_uninit(yolov8_detect_param *param);

xmedia_s32 detect_yolov8_process(yolov8_detect_param *param, const xmedia_video_frame_info *frame_info,
    xmedia_svp_yolov8_output *result);

xmedia_float iouv8(svp_base_yolov8_result *box1, svp_base_yolov8_result *box2);
xmedia_s32 svp_get_detect_result_bz(yolov5_detect_param *param, xmedia_svp_yolov8_output *result,
    const xmedia_svp_detect_anchors anchors_yaml);
#ifdef __cplusplus
}
#endif

#endif
