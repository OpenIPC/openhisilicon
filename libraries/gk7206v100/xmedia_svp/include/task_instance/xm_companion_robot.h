#ifndef __XMCOMPANION_ROBOT_H__
#define __XMCOMPANION_ROBOT_H__

#include "xm_rcnn.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SVP_ROBOT_MIN_BOX_AREA (32 * 32)

typedef struct {
    yolov5_detect_param *detect_info;
    xmedia_u64 resize_phyaddr;
    xmedia_void *priv;
} svp_companion_robot_context;

xmedia_s32 companion_robot_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 companion_robot_uninit(xmedia_void *context);

xmedia_s32 companion_robot_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 companion_robot_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 companion_robot_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_companion_robot_output *output);

#ifdef __cplusplus
}
#endif

#endif

