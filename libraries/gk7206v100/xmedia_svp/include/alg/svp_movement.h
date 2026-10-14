#ifndef __SVP_MOVEMENT__
#define __SVP_MOVEMENT__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "svp_nms.h"

#include "xmedia_svp.h"
#include "xmedia_svp_std.h"
#include "xmedia_svp_quantize.h"

#ifdef __cplusplus
extern "C" {
#endif


// 静态目标过滤
#define SVP_MOTION_THRES          0.6f
#define SVP_STILLNESS_THRES       0.9f
#define SVP_MOVEMENT_FPS_THRES    5
#define SVP_MOVELESS_COUNT        20
#define SVP_MOTION_REFRESH_TIME   20000
#define SVP_S_TO_MS_UNIT          1000LLU
#define SVP_NS_TO_MS_UNIT         1000000LLU

typedef enum {
    SVP_MOTION_STATUS_UNKNOWN = 0,
    SVP_MOTION_STATUS_STATIC,
    SVP_MOTION_STATUS_MOVEING,
} svp_motion_status;

typedef struct {
    xmedia_svp_rect rect;
    xmedia_svp_class_type class_type;
    svp_motion_status status;
    xmedia_u8 count;
    xmedia_bool flag;
    xmedia_s32 tracker_id;
    xmedia_u64 last_time;
} svp_movement_box;

typedef struct {
    xmedia_bool enable;
    xmedia_float stillness_thres;
    xmedia_u8 movement_fps_thres;
    svp_movement_box movement_targets[XMEDIA_SVP_MAX_TARGET_NUM * SVP_MOVELESS_COUNT];
    xmedia_u32 num;
} svp_movement;

typedef struct {
    xmedia_svp_class_type class_type;        // 检测结果类别
    xmedia_s32 tracker_id;                   // 检测结果追踪id
    xmedia_svp_rect rect;                    // 检测结果目标框
    xmedia_svp_motion_state motion_state;    // 运动状态
} movement_detect_result;

typedef struct {
    xmedia_u32 target_num;                                        // 检测结果数量
    movement_detect_result targets[XMEDIA_SVP_MAX_TARGET_NUM];    // 检测结果
} svp_movement_result;

xmedia_s32 svp_detect_movement(svp_movement* movement, svp_movement_result* result);

#ifdef __cplusplus
}
#endif

#endif
