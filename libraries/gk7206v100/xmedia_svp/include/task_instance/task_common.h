#ifndef __TASK_COMMON_H__
#define __TASK_COMMON_H__

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#include "xmedia_vgs.h"
#include "xmedia_type.h"
#include "xmedia_svp.h"
#include "xm_yolov5n.h"
#include "xm_yolov5n_aov.h"
#include "xm_yolov8.h"
#include "xm_adas.h"
#include "xm_fire.h"
#include "xm_gesture.h"
#include "xm_face_recognition.h"
#include "xm_rcnn.h"
#include "xm_rcnn_720p.h"
#include "xm_companion_robot.h"
#include "xm_multi_detect.h"
#include "xm_dms.h"
#include "xm_plate_recognition.h"
#include "xm_vehicle_recognition.h"
#include "xm_plate_vehicle_reco.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    xmedia_void *context;
    xmedia_svp_task_type task_type;
    xmedia_bool used;
    xmedia_bool load_finish;
} svp_task_handle;

typedef struct {
    pthread_mutex_t g_svp_mutex;
    svp_task_handle g_task_handle[XMEDIA_SVP_MAX_TASK_NUM];
    xmedia_s32 g_svp_init_flag;
    xmedia_void *proc;
} xmedia_svp_context;

xmedia_s32 svp_task_common_create(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_svp_task_cfg cfg);
xmedia_s32 svp_task_common_set(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_void *task_attr);
xmedia_s32 svp_task_common_get(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_void *task_attr);
xmedia_s32 svp_task_common_process(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_svp_task_input *input,
                                   const xmedia_void *output);
xmedia_s32 svp_task_common_destroy(svp_task_handle *task, xmedia_svp_task_type type);


#ifdef __cplusplus
}
#endif

#endif
