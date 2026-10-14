#ifndef __XMYOLOV5N_H__
#define __XMYOLOV5N_H__

#include <stdio.h>
#include <string.h>

#include "xmedia_svp_std.h"
#include "xmedia_svp.h"
#include "svp_nms.h"
#include "xmedia_svp_quantize.h"
#include "svp_core.h"
#include "yolov5.h"
#include "svp_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    yolov5_detect_param *detect_info;
    xmedia_void *priv;
} svp_yolov5n_context;

xmedia_s32 xmyolov5n_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg);

xmedia_s32 xmyolov5n_detect_uninit(xmedia_void *context);

xmedia_s32 xmyolov5n_detect_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 xmyolov5n_detect_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr);

xmedia_s32 xmyolov5n_detect_process(xmedia_void *context, const xmedia_svp_task_input *input, xmedia_svp_yolov5_output *output);

#ifdef __cplusplus
}
#endif

#endif
