#ifndef __XMYOLOV8_H__
#define __XMYOLOV8_H__

#include <stdio.h>
#include <string.h>

#include "xmedia_svp_std.h"
#include "xmedia_svp.h"
#include "svp_nms.h"
#include "xmedia_svp_quantize.h"
#include "svp_core.h"
#include "yolov8.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    yolov8_detect_param *detect_info;
    xmedia_void *priv;
} svp_yolov8_context;

xmedia_s32 xmyolov8_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 xmyolov8_detect_uninit(xmedia_void *context);

xmedia_s32 xmyolov8_detect_set_attr(xmedia_void *context, const xmedia_svp_yolov8_attr *task_attr);

xmedia_s32 xmyolov8_detect_get_attr(xmedia_void *context, xmedia_svp_yolov8_attr *task_attr);

xmedia_s32 xmyolov8_detect_process(xmedia_void *context, const xmedia_svp_task_input *input, xmedia_svp_yolov8_output *output);

#ifdef __cplusplus
}
#endif

#endif
