#ifndef __XMEDIA_SVP_H__
#define __XMEDIA_SVP_H__

#include "xmedia_type.h"
#include "xmedia_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_SVP_MAX_TASK_NUM (16u)             // 支持最多任务个数
#define XMEDIA_SVP_MAX_TARGET_NUM (50u)           // 单帧最多输出的目标个数
#define MAX_DETECT_KEYPOINT_NUM (68u)             // yolov8关键点数量
#define XMEDIA_SVP_DMS_MAX_TARGET_NUM (5u)        // DMS单帧输出目标最大个数
#define XMEDIA_SVP_DMS_SPECIAL_MAX_NUM (3u)       // DMS手机和香烟最多输出的目标个数
#define XMEDIA_SVP_LANE_MAX_NUM 4                 // 车道线最大数量
#define XMEDIA_SVP_LANE_AXIS_NUM 2                // 车道线方向数量
#define XMEDIA_SVP_LANE_MAX_ROW_POINT 18          // 车道线行方向关键点数量
#define XMEDIA_SVP_LANE_MAX_COL_POINT 40          // 车道线列方向关键点数量
#define XMEDIA_SVP_LANE_FIT_ORDER 3               // 车道线拟合因子数量
#define XMEDIA_SVP_FEATURE_COL 512                // 人脸特征向量纬度
#define XMEDIA_SVP_FR_KEYPOINT_NUM 5              // 人脸关键点数量
#define XMEDIA_SVP_MAX_VENC_CHN_NUM 16            // venc最大通道数
#define XMEDIA_SVP_MAX_VI_PIPE_NUM 8              // vi最大通道数
#define XMEDIA_SVP_PLATE_CHAR_MAX_NUM 21          // 车牌字符最大数量
#define XMEDIA_SVP_MAX_REGION 5                   // 区域最大数量
#define XMEDIA_SVP_MAX_EDGE 10                    // 区域最大边长
#define XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM (10u) // 车牌车辆单帧输出目标最大个数

// 单模型算法类别
typedef enum {
    XMEDIA_SVP_ALG_TYPE_PERSON,                 // 人形检测
    XMEDIA_SVP_ALG_TYPE_FACE,                   // 人脸检测
    XMEDIA_SVP_ALG_TYPE_CAR,                    // 车形检测
    XMEDIA_SVP_ALG_TYPE_PET,                    // 宠物检测
    XMEDIA_SVP_ALG_TYPE_HEAD,                   // 人头检测
    XMEDIA_SVP_ALG_TYPE_ELECTRIC_BICYCLE,       // 电瓶车检测
    XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE,  // 非机动车检测
    XMEDIA_SVP_ALG_TYPE_FACE_RECOGNITION,       // 人脸识别
    XMEDIA_SVP_ALG_TYPE_PERSON_KEYPOINT,        // 人体关键点检测
    XMEDIA_SVP_ALG_TYPE_FACE_ATTRIBUTE,         // 人脸属性
    XMEDIA_SVP_ALG_TYPE_GESTURE,                // 手势识别
    XMEDIA_SVP_ALG_TYPE_BOUNDARY_PERSON,        // 周界检测(人)
    XMEDIA_SVP_ALG_TYPE_BOUNDARY_CAR,           // 周界检测(车)
    XMEDIA_SVP_ALG_TYPE_OFF_DUTY,               // 离岗检测
    XMEDIA_SVP_ALG_TYPE_PFS,                    // 客流量检测
    XMEDIA_SVP_ALG_TYPE_FIREWORKS,              // 烟火检测
    XMEDIA_SVP_ALG_TYPE_PACKAGE,                // 包裹检测
    XMEDIA_SVP_ALG_TYPE_FIRESMOKE,              // 烟火检测YOLOV8
    XMEDIA_SVP_ALG_TYPE_SECOND_STAGE,           // 二阶段检测
    XMEDIA_SVP_ALG_TYPE_PERSON_CAR,             // 人车检测
    XMEDIA_SVP_ALG_TYPE_DMS,                    // DMS检测
    XMEDIA_SVP_ALG_TYPE_LICENSE_DETECT,         // 车牌检测
    XMEDIA_SVP_ALG_TYPE_LICENSE_RECO,           // 车牌字符识别
    XMEDIA_SVP_ALG_TYPE_LICENSE_COLOR,          // 车牌颜色识别
    XMEDIA_SVP_ALG_TYPE_VEHICLE_DETECT,         // 车辆检测
    XMEDIA_SVP_ALG_TYPE_VEHICLE_RECO,           // 车辆识别
    XMEDIA_SVP_ALG_TYPE_MAX,
} xmedia_svp_alg_type;

// 输出某个目标的类别
typedef enum {
    XMEDIA_SVP_CLASS_TYPE_PERSON,               // 人形类别
    XMEDIA_SVP_CLASS_TYPE_FACE,                 // 人脸类别
    XMEDIA_SVP_CLASS_TYPE_CAR,                  // 车形类别
    XMEDIA_SVP_CLASS_TYPE_PET,                  // 宠物类别
    XMEDIA_SVP_CLASS_TYPE_HEAD,                 // 人头类别
    XMEDIA_SVP_CLASS_TYPE_ELECTRIC_BICYCLE,     // 电瓶车类别
    XMEDIA_SVP_CLASS_TYPE_MASK,                 // 口罩类别
    XMEDIA_SVP_CLASS_TYPE_PERSON_KEYPOINT,      // 人体关键点类别
    XMEDIA_SVP_CLASS_TYPE_BIKE,                 // 自行车
    XMEDIA_SVP_CLASS_TYPE_BIKER,                // 骑自行车车的人
    XMEDIA_SVP_CLASS_TYPE_MOTOR,                // 摩托车
    XMEDIA_SVP_CLASS_TYPE_MOTORER,              // 骑摩托车的人
    XMEDIA_SVP_CLASS_TYPE_TRICYCLE,             // 三轮车
    XMEDIA_SVP_CLASS_TYPE_TRICYCLER,            // 骑三轮车的人
    XMEDIA_SVP_CLASS_TYPE_FIREWORKS_FIRE,       // 火
    XMEDIA_SVP_CLASS_TYPE_FIREWORKS_SMOKE,      // 烟
    XMEDIA_SVP_CLASS_TYPE_PACKAGE,              // 包裹
    XMEDIA_SVP_CLASS_TYPE_FIRE,                 // 火YOLOV8
    XMEDIA_SVP_CLASS_TYPE_SMOKE,                // 烟YOLOV8
    XMEDIA_SVP_CLASS_TYPE_GESTURE_V,            // 手势剪刀手
    XMEDIA_SVP_CLASS_TYPE_GESTURE_THUMB,        // 手势大拇指
    XMEDIA_SVP_CLASS_TYPE_GESTURE_OK,           // 手势OK
    XMEDIA_SVP_CLASS_TYPE_GESTURE_PALM,         // 手势手掌
    XMEDIA_SVP_CLASS_TYPE_NOGESTURE,            // 无手势
    XMEDIA_SVP_CLASS_TYPE_LICENSE,              // 车牌类别
    XMEDIA_SVP_CLASS_TYPE_MAX,
} xmedia_svp_class_type;

typedef enum {
    XMEDIA_SVP_INPUTDATA_FORMAT_YUV420SP,      // 输入图像格式YUV420SP
    XMEDIA_SVP_INPUTDATA_FORMAT_RGB888,        // 输入图像格式RGB888
    XMEDIA_SVP_INPUTDATA_FORMAT_MAX,
} xmedia_svp_inputdata_format;

typedef enum {
    XMEDIA_SVP_MODEL_FILE,                    // 模型文件输入
    XMEDIA_SVP_MODEL_MEM                      // 模型内存输入
} xmedia_svp_model_type;

// 目标的矩阵坐标点(坐标原点为图像左上角)
typedef struct {
    xmedia_float x1;
    xmedia_float y1;
    xmedia_float x2;
    xmedia_float y2;
} xmedia_svp_rect;

typedef enum {
    XMEDIA_SVP_MOTION_STATE_UNKNOWN = 0,         // 未知状态
    XMEDIA_SVP_MOTION_STATE_STATIC,              // 静止
    XMEDIA_SVP_MOTION_STATE_MOVING,              // 运动
} xmedia_svp_motion_state;

typedef struct {
    xmedia_svp_alg_type alg_type;            // 检测类别
    xmedia_svp_class_type class_type;        // 检测结果类别
    xmedia_float detect_score;               // 检测模型结果置信度
    xmedia_float classfier_score;            // 分类器结果置信度
    xmedia_s32 tracker_id;                   // 检测结果追踪id
    xmedia_u32 tracker_age;                  // 检测结果追踪age
    xmedia_svp_rect rect;                    // 检测结果目标框
    xmedia_bool special_target;              // 特殊目标
    xmedia_float distance;                   // 检测目标距离
    xmedia_svp_motion_state motion_state;    // 运动状态
} xmedia_svp_detect_result;

typedef struct {
    xmedia_float x;
    xmedia_float y;
} xmedia_svp_point;

typedef struct {
    xmedia_bool enable;                       // 框面积大小限制是否使能
    xmedia_float min_area;                    // 最小框面积
    xmedia_float max_area;                    // 最大框面积
} xmedia_svp_area_limit_attr;

typedef struct {
    xmedia_bool enable;                       // 边缘过滤是否使能
    xmedia_float w_edge;                      // 宽度过滤距离(框中心点至左右两边距离)
    xmedia_float h_edge;                      // 高度过滤距离(框中心点至上下两边距离)
} xmedia_svp_edge_limit_attr;

typedef enum {
    XMEDIA_SVP_TASK_UAV,
    XMEDIA_SVP_TASK_DETECT,
    XMEDIA_SVP_TASK_AOV,
    XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT,
    XMEDIA_SVP_TASK_ADAS,
    XMEDIA_SVP_TASK_FIRE,
    XMEDIA_SVP_TASK_GESTURE,
    XMEDIA_SVP_TASK_FACE_RECOGNITON,
    XMEDIA_SVP_TASK_EMOTION_CLASSIFITION,
    XMEDIA_SVP_TASK_RCNN,
    XMEDIA_SVP_TASK_RCNN_720P,
    XMEDIA_SVP_TASK_COMPANION_ROBOT,
    XMEDIA_SVP_TASK_MULTI_DETECT,
    XMEDIA_SVP_TASK_DMS,
    XMEDIA_SVP_TASK_PLATE,
    XMEDIA_SVP_TASK_VEHICLE,
    XMEDIA_SVP_TASK_PLATE_VEHICLE,
    XMEDIA_SVP_TASK_MAX,
} xmedia_svp_task_type;

typedef struct {
    xmedia_video_frame_info *frame;
    xmedia_u8 frame_num;
} xmedia_svp_task_input;

typedef struct {
    xmedia_svp_model_type load_mode;                 // 模型加载模式
    xmedia_svp_inputdata_format format;              // 模型输入图像格式
    xmedia_char *pathname;                           // 模型文件
    xmedia_u8 *buf;                                  // 模型存放的内存
    xmedia_u32 len;                                  // 模型的长度
    xmedia_svp_alg_type alg_type;                    // 模型检测类别
    xmedia_void *priv;                               // 模型私有数据
} xmedia_svp_modules;

typedef struct {
    xmedia_svp_task_type task_type;                  // 任务类型
    xmedia_svp_modules *modules;                     // 模型数据
    xmedia_u8 module_num;                            // 模型数量
    xmedia_void *priv;                               // 私有数据
} xmedia_svp_task_cfg;

typedef struct {
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.65f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.8f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
} xmedia_svp_detect_base_attr;

typedef struct {
    xmedia_bool enable;                                   // 追踪使能
    xmedia_float max_feature_distance;                    // 追踪特征匹配距离最大阈值
    xmedia_float min_feature_distance;                    // 追踪特征匹配距离最小阈值
    xmedia_float max_iou_distance;                        // 追踪iou匹配距离阈值
    xmedia_u32 iou2feature;                               // 追踪iou匹配变成确认态
    xmedia_u32 max_age;                                   // 追踪age阈值
} xmedia_svp_deepsort_attr;

typedef struct {
    xmedia_svp_detect_base_attr person_attr;
    xmedia_svp_detect_base_attr head_attr;
    xmedia_svp_detect_base_attr gesture_attr;
    xmedia_svp_deepsort_attr deepsort_attr;
    xmedia_u32 focal_length_px;
    xmedia_u32 head_width_cm;
    xmedia_u32 palm_width_cm;
} xmedia_svp_uav_attr;

typedef struct {
    xmedia_svp_detect_result track_result;
    xmedia_svp_detect_result head_result;
    xmedia_svp_detect_result gesture_result;
    xmedia_float head_distance;
    xmedia_bool is_hover;
    xmedia_float gesture_distance;
} xmedia_svp_uav_output;

typedef struct {
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.65f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.8f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
    xmedia_bool bytetrack_enable;            // 追踪开关
    xmedia_bool motionless_filter_enable;    // 运动状态检测开关
    xmedia_float stillness_thres;            // 由运动转为静止状态的灵敏度阈值，建议值0.9f
    xmedia_u8 movement_fps_thres;            // 由运动转为静止状态的连续帧阈值，建议值5
    xmedia_bool smart_venc_enable;
    xmedia_bool smart_ae_enable;
    xmedia_bool smart_venc_array[XMEDIA_SVP_MAX_VENC_CHN_NUM];
    xmedia_bool smart_ae_array[XMEDIA_SVP_MAX_VI_PIPE_NUM];
} xmedia_svp_yolov5_attr;

typedef struct {
    xmedia_u32 target_num;                                          // 检测结果数量
    xmedia_svp_detect_result targets[XMEDIA_SVP_MAX_TARGET_NUM];    // 检测结果
} xmedia_svp_yolov5_output;

typedef struct {
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.65f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.8f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
    xmedia_bool bytetrack_enable;            // 追踪开关
    xmedia_bool motionless_filter_enable;    // 运动状态检测开关
    xmedia_bool aov_only_target;             // 只获取目标有无
} xmedia_svp_aov_attr;

typedef struct {
    xmedia_bool is_exist;
    xmedia_svp_yolov5_output detect_output;
} xmedia_svp_aov_detect_output;

typedef struct {
    xmedia_float x;
    xmedia_float y;
    xmedia_float score;
} xmedia_svp_keypoint;

typedef struct {
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.65f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.8f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
    xmedia_bool bytetrack_enable;            // 追踪开关
    xmedia_bool motionless_filter_enable;    // 运动状态检测开关
    xmedia_float stillness_thres;            // 由运动转为静止状态的灵敏度阈值，建议值0.9f
    xmedia_u8 movement_fps_thres;            // 由运动转为静止状态的连续帧阈值，建议值5
} xmedia_svp_yolov8_attr;

typedef struct {
    xmedia_svp_alg_type alg_type;                     // 检测类别
    xmedia_svp_class_type class_type;                 // 检测结果类别
    xmedia_float detect_score;                        // 检测模型结果置信度
    xmedia_float classfier_score;                     // 分类器结果置信度
    xmedia_s32 tracker_id;                            // 检测结果追踪id
    xmedia_u32 tracker_age;                           // 检测结果追踪age
    xmedia_svp_rect rect;                             // 检测结果目标框
    xmedia_svp_keypoint kpt[MAX_DETECT_KEYPOINT_NUM]; // 关键点坐标
    xmedia_svp_motion_state motion_state;             // 运动状态
} xmedia_svp_yolov8_result;

typedef struct {
    xmedia_u32 target_num;                                          // 检测结果数量
    xmedia_svp_yolov8_result targets[XMEDIA_SVP_MAX_TARGET_NUM];    // 检测结果
} xmedia_svp_yolov8_output;

typedef struct {
    xmedia_u32 focal_length_px;
    xmedia_u32 car_limit_angle;
    xmedia_float camera_height;
    xmedia_u32 plate_width;
} xmedia_svp_adas_distance_attr;

typedef struct {
    xmedia_svp_yolov5_attr car_attr;
    xmedia_svp_yolov5_attr plate_attr;
    xmedia_svp_yolov5_attr person_attr;
    xmedia_svp_yolov5_attr nmv_attr;
    xmedia_svp_adas_distance_attr distance_attr;
} xmedia_svp_adas_attr;

typedef struct {
    xmedia_s32 lane_point_num[XMEDIA_SVP_LANE_MAX_NUM];
    xmedia_s32 lane_fitted_point_num[XMEDIA_SVP_LANE_MAX_NUM];
    xmedia_svp_keypoint row_result[XMEDIA_SVP_LANE_AXIS_NUM][XMEDIA_SVP_LANE_MAX_ROW_POINT];
    xmedia_svp_keypoint col_result[XMEDIA_SVP_LANE_AXIS_NUM][XMEDIA_SVP_LANE_MAX_COL_POINT];
    xmedia_svp_keypoint row_fitted_result[XMEDIA_SVP_LANE_AXIS_NUM][XMEDIA_SVP_LANE_MAX_ROW_POINT];
    xmedia_svp_keypoint col_fitted_result[XMEDIA_SVP_LANE_AXIS_NUM][XMEDIA_SVP_LANE_MAX_COL_POINT];
    xmedia_svp_yolov5_output car_result;
    xmedia_svp_yolov5_output plate_result;
    xmedia_svp_yolov5_output person_result;
    xmedia_svp_yolov5_output nmv_result;
    xmedia_float coeffs[XMEDIA_SVP_LANE_MAX_NUM][XMEDIA_SVP_LANE_FIT_ORDER];
} xmedia_svp_adas_result;

typedef struct {
    xmedia_u8 smoke_apear_count;
    xmedia_u8 smoke_loss_count;
    xmedia_float smoke_change_ratio;
    xmedia_u8 fire_loss_count;
    xmedia_float fire_change_ratio;
    xmedia_float fire_ratio_sum;
    xmedia_svp_yolov8_attr detect_attr;
} xmedia_svp_fire_attr;

typedef struct {
    xmedia_svp_rect gesture_rect;
    xmedia_s32 gesture_tracker_id;
    xmedia_float select_score[XMEDIA_SVP_MAX_TARGET_NUM];
    xmedia_svp_yolov8_output result;
} xmedia_svp_gesture_result;

typedef enum {
    XMEDIA_SVP_CLASS_EMOTION_SMILE,
    XMEDIA_SVP_CLASS_EMOTION_NORMAL,
    XMEDIA_SVP_CLASS_EMOTION_UNKNOWN,
} xmedia_svp_class_emotion_type;

typedef struct {
    xmedia_float clockwise_thres;
    xmedia_float anticlockwise_thres;
    xmedia_float left_thres;
    xmedia_float right_thres;
    xmedia_float upward_thres;
    xmedia_float downward_thres;
} xmedia_svp_face_pose_attr;

typedef struct {
    xmedia_svp_yolov5_attr detect_attr;
    xmedia_u64 luma_score;
    xmedia_s32 ive_sobel_score;
    xmedia_svp_face_pose_attr fr_pose_attr;
    xmedia_float keypoint_score;
} xmedia_svp_fr_attr;

typedef struct {
    xmedia_float vector[XMEDIA_SVP_FEATURE_COL];
    xmedia_svp_rect rect;
    xmedia_s32 fr_tracker_id;
    xmedia_float score;
    xmedia_svp_point keypoint_arry[XMEDIA_SVP_FR_KEYPOINT_NUM];
} xmedia_svp_fr_result;

typedef struct {
    xmedia_svp_rect rect;
    xmedia_s32 face_tracker_id;
    xmedia_float score;
    xmedia_svp_point keypoint_arry[XMEDIA_SVP_FR_KEYPOINT_NUM];
    xmedia_svp_class_emotion_type emotion_class;
} xmedia_svp_face_attribute_result;

typedef struct {
    xmedia_s32 face_num;
    xmedia_svp_fr_result fr_result[XMEDIA_SVP_MAX_TARGET_NUM];
} xmedia_svp_fr_output;

typedef struct {
    xmedia_s32 face_num;
    xmedia_svp_face_attribute_result attribute_result[XMEDIA_SVP_MAX_TARGET_NUM];
} xmedia_svp_face_attribute_output;

typedef struct {
    xmedia_u32 target_num;
    xmedia_svp_detect_result targets[XMEDIA_SVP_MAX_TARGET_NUM];
} xmedia_svp_companion_robot_output;

typedef enum {
    XMEDIA_SVP_MEM_TYPE_BLOCK,
    XMEDIA_SVP_MEM_TYPE_AINR_SHARE,
    XMEDIA_SVP_MEM_TYPE_COMPLETE,
    XMEDIA_SVP_MEM_TYPE_MAX,
} xmedia_svp_mem_reuse_type;

typedef struct {
    xmedia_u32 size;
    xmedia_u64 phyaddr;
    xmedia_void *viraddr;
} xmedia_svp_mem_info;

typedef struct {
    xmedia_svp_mem_reuse_type reuse_type;
    xmedia_svp_mem_info workbuf_reuse_mem;
    xmedia_svp_mem_info input_reuse_mem;
    xmedia_svp_mem_info output_reuse_mem;
} xmedia_svp_cfg;

typedef struct {
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.65f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.8f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u8 max_target_num;                // 单个检测任务的最大目标数，所有检测任务总和最大值50
} xmedia_svp_multi_base_attr;

typedef struct {
    xmedia_svp_multi_base_attr car_attr;
    xmedia_svp_multi_base_attr person_attr;
    xmedia_svp_multi_base_attr nmv_attr;
    xmedia_svp_multi_base_attr face_attr;
    xmedia_svp_multi_base_attr pet_attr;
    xmedia_svp_multi_base_attr pkg_attr;
    xmedia_bool bytetrack_enable;            // 追踪开关
    xmedia_bool motionless_filter_enable;    // 运动状态检测开关
    xmedia_float stillness_thres;            // 由运动转为静止状态的灵敏度阈值，建议值0.9f
    xmedia_u8 movement_fps_thres;            // 由运动转为静止状态的连续帧阈值，建议值5
} xmedia_svp_multi_detect_attr;

typedef struct {
    xmedia_svp_yolov8_attr detect_attr;
    xmedia_u8 warn_frame;                           // 警告帧数
    xmedia_float iou_thres;                         // 相交阈值，建议值0.1f
} xmedia_svp_dms_attr;

typedef struct {
    xmedia_bool flag;                                 // 状态标志位
    xmedia_float detect_score;                        // 检测模型结果置信度
    xmedia_svp_rect rect;                             // 检测结果目标框
} xmedia_svp_dms_state;

typedef struct {
    xmedia_bool fatigue_flag;                                    // 疲劳状态标志位
    xmedia_u8 phone_num;                                         // 手机检测数量，最大3
    xmedia_svp_dms_state phone[XMEDIA_SVP_DMS_SPECIAL_MAX_NUM];  // 手机状态
    xmedia_u8 cigar_num;                                         // 香烟检测数量，最大3
    xmedia_svp_dms_state cigar[XMEDIA_SVP_DMS_SPECIAL_MAX_NUM];  // 香烟状态
} xmedia_svp_dms_report;

typedef struct {
    xmedia_svp_alg_type alg_type;                     // 检测类别
    xmedia_svp_class_type class_type;                 // 检测结果类别
    xmedia_float detect_score;                        // 检测模型结果置信度
    xmedia_s32 tracker_id;                            // 检测结果追踪id
    xmedia_u32 tracker_age;                           // 检测结果追踪age
    xmedia_svp_rect rect;                             // 检测结果目标框
    xmedia_bool special_target;                       // 特殊目标
    xmedia_svp_keypoint kpt[MAX_DETECT_KEYPOINT_NUM]; // 关键点坐标
    xmedia_svp_dms_report dms_report;
} xmedia_svp_dms_result;

typedef struct {
    xmedia_u32 target_num;                                          // 检测结果数量
    xmedia_svp_dms_result targets[XMEDIA_SVP_DMS_MAX_TARGET_NUM];   // 最大目标数5
} xmedia_svp_dms_output;

typedef struct {
    xmedia_s32 x; // X坐标，采用万分比坐标
    xmedia_s32 y; // Y坐标，采用万分比坐标
} xmedia_svp_vec2i;

typedef struct {
    xmedia_s32 points_count;                            // 坐标点数量
    xmedia_svp_vec2i points_array[XMEDIA_SVP_MAX_EDGE]; // 坐标点数组
} xmedia_svp_landmarks;

typedef struct {
    xmedia_s32 zone_num;                               // 区域个数
    xmedia_svp_landmarks zones[XMEDIA_SVP_MAX_REGION]; // 区域数组
} xmedia_svp_zone;

typedef struct {
    xmedia_svp_yolov5_attr plate_detect_attr;
    xmedia_u32             plate_min_width;
    xmedia_u32             report_count;
    xmedia_float           report_thres;
    xmedia_svp_zone        detect_zone;
} xmedia_svp_plate_reco_attr;

// 车牌车辆方向
typedef enum {
    XMEDIA_SVP_DIRECTION_FORWARD,          // 向前
    XMEDIA_SVP_DIRECTION_BACKWARD,         // 向后
    XMEDIA_SVP_DIRECTION_UNKOWN = 99,      // 未知
} xmedia_svp_direction;

// 车牌颜色
typedef enum {
    XMEDIA_SVP_PLATE_COLOR_BLACK   = 0, // 黑牌
    XMEDIA_SVP_PLATE_COLOR_BULE    = 1, // 蓝牌
    XMEDIA_SVP_PLATE_COLOR_GREEN   = 2, // 绿牌
    XMEDIA_SVP_PLATE_COLOR_RESEVER = 3, // 保留
    XMEDIA_SVP_PLATE_COLOR_WHITE   = 4, // 白牌
    XMEDIA_SVP_PLATE_COLOR_PURPLE  = 5, // 紫牌
    XMEDIA_SVP_PLATE_COLOR_YELLOW  = 6, // 黄牌
    XMEDIA_SVP_PLATE_COLOR_UNKNOWN = 7, // 未知
} xmedia_svp_plate_color;

typedef struct {
    xmedia_s32 tracker_id;
    xmedia_char plate_char[XMEDIA_SVP_PLATE_CHAR_MAX_NUM];
    xmedia_float char_score;
    xmedia_svp_plate_color color;         // 车牌颜色
    xmedia_float color_score;             // 车牌颜色置信度
    xmedia_svp_rect rect;                 // 车牌坐标
    xmedia_svp_direction lpr_direction;   // 车牌触发方向
} xmedia_svp_plate_result;

typedef struct {
    xmedia_s32 target_num;
    xmedia_svp_plate_result targets[XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM];
} xmedia_svp_plate_reco_output;

typedef struct {
    xmedia_svp_yolov5_attr detect_attr;
    xmedia_u32             report_count;
    xmedia_float           report_thres;
    xmedia_svp_zone        detect_zone;
} xmedia_svp_vehicle_reco_attr;

// 车辆类型
typedef enum {
    XMEDIA_SVP_VEHICLE_TYPE_SEDAN,            // 小车
    XMEDIA_SVP_VEHICLE_TYPE_MINIVAN,          // 小型货车
    XMEDIA_SVP_VEHICLE_TYPE_TRUCK,            // 卡车
    XMEDIA_SVP_VEHICLE_TYPE_TANKER,           // 罐车
    XMEDIA_SVP_VEHICLE_TYPE_MINIBUS,          // 巴士
    XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN = 99,     // 未知
} xmedia_svp_vehicle_type;

// 车辆颜色
typedef enum {
    XMEDIA_SVP_VEHICLE_COLOR_WHITE,           // 白色
    XMEDIA_SVP_VEHICLE_COLOR_RED,             // 红色
    XMEDIA_SVP_VEHICLE_COLOR_YELLOW,          // 黄色
    XMEDIA_SVP_VEHICLE_COLOR_BLACK,           // 黑色
    XMEDIA_SVP_VEHICLE_COLOR_BLUE,            // 蓝色
    XMEDIA_SVP_VEHICLE_COLOR_GREEN,           // 绿色
    XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN = 99,    // 未知
} xmedia_svp_vehicle_color;

typedef struct {
    xmedia_s32 tracker_id;
    xmedia_svp_vehicle_color color;         // 车辆颜色
    xmedia_svp_vehicle_type type;           // 车辆类型
    xmedia_float color_scr;                 // 车辆颜色置信度
    xmedia_float type_scr;                  // 车辆类型置信度
    xmedia_svp_rect rect;                   // 车辆坐标
    xmedia_svp_direction vehicle_direction; // 车辆触发方向
} xmedia_svp_vehicle_result;

typedef struct {
    xmedia_s32 target_num;
    xmedia_svp_vehicle_result targets[XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM];
} xmedia_svp_vehicle_reco_output;

typedef struct {
    xmedia_svp_yolov5_attr plate_attr;
    xmedia_svp_yolov5_attr veh_attr;
    xmedia_u32             plate_min_width;
    xmedia_u32             report_count;
    xmedia_float           report_thres;
    xmedia_svp_zone        detect_zone;
} xmedia_svp_plate_vehicle_attr;

typedef struct {
    xmedia_s32 tracker_id;
    xmedia_svp_vehicle_color veh_color;         // 车辆颜色
    xmedia_svp_vehicle_type veh_type;           // 车辆类型
    xmedia_float veh_color_scr;                 // 车辆颜色置信度
    xmedia_float veh_type_scr;                  // 车辆类型置信度
    xmedia_svp_rect veh_rect;                   // 车辆坐标
    xmedia_svp_direction veh_direction;         // 车辆触发方向
    xmedia_char plate_char[XMEDIA_SVP_PLATE_CHAR_MAX_NUM];
    xmedia_float plate_score;
    xmedia_svp_plate_color plate_color;         // 车牌颜色
    xmedia_float plate_color_scr;               // 车牌颜色置信度
    xmedia_svp_rect plate_rect;                 // 车牌坐标
    xmedia_svp_direction plate_direction;       // 车牌触发方向
} xmedia_svp_plate_veh_result;

typedef struct {
    xmedia_s32 target_num;
    xmedia_svp_plate_veh_result targets[XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM];
} xmedia_svp_plate_vehicle_output;

/*
 *  函数功能：初始化SVP
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_init(xmedia_void);

/*
 *  函数功能：去初始化SVP
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_uninit(xmedia_void);

/*
 *  函数功能：设置svp配置信息
 *  函数参数：
 *      svp_cfg     - 输入参数，svp配置信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_set_config(xmedia_svp_cfg* svp_cfg);

/*
 *  函数功能：获取svp配置信息
 *  函数参数：
 *      svp_cfg     - 输入参数，svp配置信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_get_config(xmedia_svp_cfg* svp_cfg);

/*
 *  函数功能：创建一路svp_task处理流程
 *  函数参数：
 *      handle  - 输出参数，创建成功返回SVP句柄
 *      cfg     - 输入参数，svp_task配置信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      每个句柄对应一路svp_task处理流程
 */
xmedia_s32 xmedia_svp_task_create(xmedia_s32 *handle, const xmedia_svp_task_cfg cfg);

/*
 *  函数功能：设置svp_task处理流程的属性
 *  函数参数：
 *      handle    - 输入参数，svp_task处理流程句柄
 *      task_attr - 输入参数，设置svp_task属性
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      创建后，销毁前的任意时间均可调用
 */
xmedia_s32 xmedia_svp_task_set_attr(xmedia_s32 handle, const xmedia_void *task_attr);

/*
 *  函数功能：获取svp_task处理流程的属性
 *  函数参数：
 *      handle    - 输入参数，svp_task处理流程句柄
 *      task_attr - 输出参数，获取的svp_task属性
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_task_get_attr(xmedia_s32 handle, xmedia_void *task_attr);

/*
 *  函数功能：开始svp_task处理流程
 *  函数参数：
 *      handle     - 输入参数，SVP处理流程句柄
 *      input      - 输入参数，输入图像帧信息
 *      output     - 输出参数，处理结果信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 *  注意：
 *      创建后，销毁前的任意时间均可调用
 */
xmedia_s32 xmedia_svp_task_process(xmedia_s32 handle, const xmedia_svp_task_input *input, xmedia_void *output);

/*
 *  函数功能：销毁一路svp_task处理流程
 *  函数参数：
 *      handle  - 输入参数，SVP处理流程句柄
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_task_destroy(xmedia_s32 handle);

/*
 *  函数功能：获取当前SVP版本信息
 *  函数参数：
 *      version   - 输出参数，用于保存SVP版本信息
 *      len       - 输入参数，version的长度
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_svp_get_version(xmedia_char *version, const xmedia_s32 len);

#ifdef __cplusplus
}
#endif

#endif

