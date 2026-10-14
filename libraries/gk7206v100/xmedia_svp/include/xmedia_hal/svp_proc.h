#ifndef __SVP_PROC_H__
#define __SVP_PROC_H__

#include "xmedia_svp.h"
#include "svp_comm.h"
#include "task_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_PROC_SIZE (4096 * 4)
#define SVP_PROC_NAME_SIZE (16)

typedef struct {
    xmedia_svp_alg_type alg_type;            // 算法类型
    xmedia_float detect_threshold;           // 置信度阈值，建议值0.55f
    xmedia_float classifier_threshold;       // 分类器阈值，建议值0.01f
    xmedia_float iou_threshold;              // iou相交比阈值，建议值0.5f
    xmedia_u32 max_target_num;               // 最大目标数，最大值10
    svp_movement movement;                   // 静止过滤相关参数
    xmedia_bool track_enable;                // 追踪相关参数
} proc_detect_info;

typedef struct {
    xmedia_u8 framerate;                     // 帧率
    xmedia_u64 cost_time;                    // 消耗的时间
    xmedia_u8 fps_count;                     // 一秒内运行的次数
    struct timeval last_time;                // 上次统计时间
} svp_dbg_info;

typedef struct {
    xmedia_u32  buff_len_total;
    xmedia_u32  remain_buf;
    xmedia_char *vir_addr;
} svp_proc_info;

typedef struct {
    svp_proc_info proc_info;
    svp_dbg_info dbg_info[XMEDIA_SVP_MAX_TASK_NUM];
    svp_task_handle *handle[XMEDIA_SVP_MAX_TASK_NUM];
} svp_proc_context;

xmedia_s32 svp_update_proc_time(svp_proc_context* proc, xmedia_s32 handle, xmedia_u64 cost_time);
xmedia_s32 svp_update_proc_framerate(svp_proc_context* proc, xmedia_s32 handle);
xmedia_s32 svp_update_proc_info(svp_proc_context* proc, xmedia_s32 handle);
xmedia_s32 svp_init_proc(xmedia_svp_context* context);
xmedia_s32 svp_deinit_proc(xmedia_svp_context* context);

#ifdef __cplusplus
}
#endif

#endif
