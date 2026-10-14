#ifndef __XMYOLOV5N_AOV_H__
#define __XMYOLOV5N_AOV_H__

#include <stdio.h>
#include <string.h>

#include "xmedia_svp_std.h"
#include "xmedia_svp.h"
#include "svp_nms.h"
#include "xmedia_svp_quantize.h"
#include "svp_core.h"
#include "yolov5.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    yolov5_detect_param *detect_info;
    xmedia_void *priv;
} svp_yolov5n_aov_context;

xmedia_s32 xmyolov5n_aov_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 xmyolov5n_aov_detect_uninit(xmedia_void *context);

xmedia_s32 xmyolov5n_aov_detect_set_attr(xmedia_void *context, const xmedia_svp_aov_attr *task_attr);

xmedia_s32 xmyolov5n_aov_detect_get_attr(xmedia_void *context, xmedia_svp_aov_attr *task_attr);

xmedia_s32 xmyolov5n_aov_detect_process(xmedia_void *context, const xmedia_svp_task_input *input, xmedia_svp_aov_detect_output *output);

#ifdef __cplusplus
}
#endif

#endif
