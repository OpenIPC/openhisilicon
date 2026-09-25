#ifndef __XMEDIA_API_AI_H__
#define __XMEDIA_API_AI_H__

#include "type.h"
#include "comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_AI_MAX_VENC_CHN_NUM  16
#define XMEDIA_AI_MAX_VI_PIPE_NUM   2

// 单帧最多输出的目标个数
#define XMEDIA_AI_MAX_TARGET_NUM    (10u)

// 设置检测类别
typedef enum {
    XMEDIA_AI_DETECT_TYPE_PERSON,
    XMEDIA_AI_DETECT_TYPE_FACE,
    XMEDIA_AI_DETECT_TYPE_CAR,
    XMEDIA_AI_DETECT_TYPE_PERSON_CAR,    // 人形和车形检测
    XMEDIA_AI_DETECT_TYPE_PERSON_FACE,   // 人形和人脸检测
    XMEDIA_AI_DETECT_TYPE_MAX,
} xmedia_ai_detect_type;

typedef enum {
    XMEDIA_AI_INPUTDATA_FORMAT_YUV420SP,
    XMEDIA_AI_INPUTDATA_FORMAT_RGB444,
    XMEDIA_AI_INPUTDATA_FORMAT_MAX,
} xmedia_ai_inputdata_format;

// 输出某个目标的类别
typedef enum {
    XMEDIA_AI_CLASS_TYPE_PERSON,
    XMEDIA_AI_CLASS_TYPE_FACE,
    XMEDIA_AI_CLASS_TYPE_CAR,
    XMEDIA_AI_CLASS_TYPE_MAX,
} xmedia_ai_class_type;

// 输入图像帧信息
typedef struct {
    VIDEO_FRAME_INFO_S *frame;
} xmedia_ai_input_image;

typedef struct {
    xmedia_ai_detect_type type;
    xmedia_ai_inputdata_format format;
    xmedia_char *pathname;              // AI模型文件
} xmedia_ai_model;

typedef struct {
    xmedia_float detect_threshold;     // 置信度阈值，建议值0.3f
    xmedia_float iou_threshold;        // iou相交比阈值，建议值0.5f
} xmedia_ai_detect_attr;

typedef struct {
    xmedia_ai_detect_attr detect_attr;
    xmedia_bool low_bitrate_enable[XMEDIA_AI_MAX_VENC_CHN_NUM];  // 使能VENC对应通道智能编码
    xmedia_bool advance_isp_enable[XMEDIA_AI_MAX_VI_PIPE_NUM];    // 使能VI对应通道智能曝光
} xmedia_ai_attr;

// 目标的矩阵坐标点
typedef struct {
    xmedia_float x1;
    xmedia_float y1;
    xmedia_float x2;
    xmedia_float y2;
} xmedia_ai_rect;

typedef struct {
    xmedia_ai_detect_type detect_type;
    xmedia_ai_class_type class_type;
    xmedia_float score;               // 目标的置信度
    xmedia_ai_rect rect;
} xmedia_ai_detect_result;

// 输出结果信息
typedef struct {
    xmedia_u32 target_num;
    xmedia_ai_detect_result targets[XMEDIA_AI_MAX_TARGET_NUM];
} xmedia_ai_result_info;

/*
 *  函数功能：初始化AI
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_ai_init(xmedia_void);

/*
 *  函数功能：去初始化AI
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_ai_uninit(xmedia_void);

/*
 *  函数功能：创建一路AI处理流程
 *  函数参数：
 *      handle  - 输出参数，创建成功返回AI句柄
 *      model   - 输入参数，AI模型信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      每个句柄对应一路AI处理流程
 */
xmedia_s32 xmedia_ai_create(xmedia_s32 *handle, const xmedia_ai_model *model);

/*
 *  函数功能：销毁一路AI处理流程
 *  函数参数：
 *      handle  - 输入参数，AI处理流程句柄
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_ai_destroy(xmedia_s32 handle);

/*
 *  函数功能：设置AI处理流程的属性
 *  函数参数：
 *      handle   - 输入参数，AI处理流程句柄
 *      ai_attr  - 输入参数，设置AI属性
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      创建后，销毁前的任意时间均可调用
 */
xmedia_s32 xmedia_ai_set_attr(const xmedia_s32 handle, const xmedia_ai_attr *ai_attr);

/*
 *  函数功能：获取AI处理流程的属性
 *  函数参数：
 *      handle   - 输入参数，AI处理流程句柄
 *      ai_attr  - 输出参数，当前设置的AI属性
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_ai_get_attr(const xmedia_s32 handle, xmedia_ai_attr *ai_attr);

/*
 *  函数功能：开始AI处理流程
 *  函数参数：
 *      handle     - 输入参数，AI处理流程句柄
 *      input_img  - 输入参数，输入图像帧信息
 *      result     - 输出参数，处理结果信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      创建后，销毁前的任意时间均可调用
 */
xmedia_s32 xmedia_ai_process(const xmedia_s32 handle, const xmedia_ai_input_image *input_img, xmedia_ai_result_info *result);

/*
 *  函数功能：获取当前AI版本信息
 *  函数参数：
 *      version   - 输出参数，用于保存AI版本信息
 *      len       - 输入参数，version的长度
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_ai_get_version(xmedia_char *version, const xmedia_s32 len);

#ifdef __cplusplus
}
#endif

#endif
