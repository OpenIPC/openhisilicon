#ifndef __SVP_COMM__
#define __SVP_COMM__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "svp_nms.h"
#include "svp_core.h"
#include "svp_bytetracker.h"
#include "libalg_api.h"
#include "persptrans.h"
#include "yolov5.h"
#include "yolov8.h"

#include "xmedia_venc.h"
#include "xmedia_sys.h"
#include "xmedia_vgs.h"
#include "xmedia_tde.h"
#include "xmedia_isp.h"
#include "xmedia_ive.h"
#include "xmedia_svp.h"
#include "xmedia_svp_std.h"
#include "xmedia_svp_quantize.h"

#ifdef __cplusplus
extern "C" {
#endif

// VGS大小限制
#define VGS_LIMIT_BILINEAR_INIMG_W 8
#define VGS_LIMIT_BILINEAR_INIMG_H 8
#define VGS_LIMIT_MIN_INIMG_W 16
#define VGS_LIMIT_MIN_INIMG_H 16
#define VGS_LUMA_LIMIT 2
#define SVP_SKIN_COLOR_MIN_V_VALUE 131
#define SVP_SKIN_COLOR_MAX_V_VALUE 155
#define SVP_SKIN_COLOR_MIN_U_VALUE 107
#define SVP_SKIN_COLOR_MAX_U_VALUE 127
#define SVP_MAX_CAL_LUMA_RECT_NUM 5   /* 最大支持计算5个框的亮度信息 */
#define SVP_MIN_RECT_WIDTH 16
#define SVP_MIN_RECT_HEIGHT 16
#define SVP_MOV_ACCURACY_ONE 1
// cal luma rate
#define SVP_FACE_RECT_FD_X_COEF 4
#define SVP_FACE_RECT_FD_Y_COEF 2
#define SVP_FACE_RECT_FD_W_COEF 12
#define SVP_FACE_RECT_FD_H_COEF 16
#define SVP_FACE_RECT_PD_X_COEF 4
#define SVP_FACE_RECT_PD_Y_COEF 5
#define SVP_FACE_RECT_PD_W_COEF 12
#define SVP_FACE_RECT_PD_H_COEF 10
#define SVP_FACE_RECT_BASE_COEF 20
// smartAE class
#define SVP_SMART_CLASS_MAX 2
#define SVP_MODEL_TYPE_PEOPLE 0
#define SVP_MODEL_TYPE_FACE 1
#define SVP_PEOPLE_INDEX 0
#define SVP_FACE_INDEX 1
#define RCNN_DEBUG_STRING 64
// 模型大图分辨率
#define INPUT_BIG_WIDE 1920
#define INPUT_BIG_HIGH 1080
/** 万分比坐标比例  */
#define SVP_SCALE_RATIO 10000.0f
#define XMEDIA_SVP_FR_KEYPOINT_CNT ((XMEDIA_SVP_FR_KEYPOINT_NUM) * (2)) // 人脸关键点总数[x和y]

#define SVP_DIV_0_TO_1(a)   ((0 == (a)) ? 1 : (a))

typedef struct {
    xmedia_u64 img_addr;             // 物理地址
    xmedia_void *vir_addr;           // 虚拟地址
    xmedia_u32 width;                // 宽度
    xmedia_u32 hight;                // 长度
} svp_ive_img;

typedef struct {
    xmedia_float targetpoints[XMEDIA_SVP_FR_KEYPOINT_CNT];
    xmedia_float keypoints[XMEDIA_SVP_FR_KEYPOINT_CNT];
    xmedia_char *warpaffine_output;
} svp_warpaffine_pairpack;

typedef struct {
    xmedia_u32 left;
    xmedia_u32 top;
    xmedia_u32 right;
    xmedia_u32 bottom;
} cal_svp_rect;

typedef struct {
    xmedia_u8 x_coef;
    xmedia_u8 y_coef;
    xmedia_u8 w_coef;
    xmedia_u8 h_coef;
} svp_rect_cut_coef;

typedef struct {
    svp_warpaffine_pairpack warpaffine_pairpack[XMEDIA_SVP_MAX_TARGET_NUM];
} svp_warpaffine_info;

xmedia_void softmax(xmedia_float *arr, xmedia_s32 size, xmedia_float *result_arr);
xmedia_s32 svp_vgs_resize(const xmedia_video_frame_info *input_frame, xmedia_video_frame_info *output_frame);
xmedia_s32 svp_ive_crop(const svp_ive_img *src_img, svp_ive_img *dst_img, xmedia_u32 w_offset, xmedia_u32 h_offset);
xmedia_s32 svp_ive_dma_image_copy(const xmedia_video_frame_info *pst_frame_info, xmedia_ive_image_s *pst_dst);
xmedia_s32 svp_ive_padding(svp_ive_img *src_img, svp_ive_img *dst_img, xmedia_u32 right, xmedia_u32 top);
xmedia_s32 svp_send_info_to_venc(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                 const xmedia_svp_task_input *input_image);
xmedia_s32 svp_update_luma(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                           const xmedia_svp_task_input *input_image);
xmedia_s32 svp_padding_resize(xmedia_video_frame_info *ref_frame, xmedia_svp_rect *rect, xmedia_u64 resize_phyaddr,
                            xmedia_void *resize_viraddr, xmedia_u32 dst_width, xmedia_u32 dst_height);
xmedia_float svp_iou_with_box1(xmedia_svp_yolov8_result *box1, xmedia_svp_dms_result *box2);
xmedia_s32 svp_detect_zone(const xmedia_svp_zone* zone, const xmedia_svp_rect* rect,
    const xmedia_u32 width, const xmedia_u32 height);
xmedia_s32 clip_rect_to_frame(xmedia_svp_rect *rect, xmedia_u32 frame_width, xmedia_u32 frame_height);

#ifdef __cplusplus
}
#endif

#endif

