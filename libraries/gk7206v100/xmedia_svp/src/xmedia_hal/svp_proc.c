#include "svp_proc.h"
#include "mpi_sys.h"

static xmedia_u32 g_sys_proc;
static svp_proc_context *proc_context = XMEDIA_NULL;

const char* alg_type[] = {
    "PERSON",             // XMEDIA_SVP_ALG_TYPE_PERSON
    "FACE",               // XMEDIA_SVP_ALG_TYPE_FACE
    "CAR",                // XMEDIA_SVP_ALG_TYPE_CAR
    "PET",                // XMEDIA_SVP_ALG_TYPE_PET
    "HEAD",               // XMEDIA_SVP_ALG_TYPE_HEAD
    "E_BICYCLE",          // XMEDIA_SVP_ALG_TYPE_ELECTRIC_BICYCLE
    "NMV",                // XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE
    "FACE_RECOG",         // XMEDIA_SVP_ALG_TYPE_FACE_RECOGNITION
    "PERSON_KPT",         // XMEDIA_SVP_ALG_TYPE_PERSON_KEYPOINT
    "FACE_ATTR",          // XMEDIA_SVP_ALG_TYPE_FACE_ATTRIBUTE
    "GESTURE",            // XMEDIA_SVP_ALG_TYPE_GESTURE
    "BOUNDARY_PERSON",    // XMEDIA_SVP_ALG_TYPE_BOUNDARY_PERSON
    "BOUNDARY_CAR",       // XMEDIA_SVP_ALG_TYPE_BOUNDARY_CAR
    "OFF_DUTY",           // XMEDIA_SVP_ALG_TYPE_OFF_DUTY
    "PFS",                // XMEDIA_SVP_ALG_TYPE_PFS
    "FIREWORKS",          // XMEDIA_SVP_ALG_TYPE_FIREWORKS
    "PACKAGE",            // XMEDIA_SVP_ALG_TYPE_PACKAGE
    "FIRESMOKE",          // XMEDIA_SVP_ALG_TYPE_FIRESMOKE
    "SECOND_STAGE",       // XMEDIA_SVP_ALG_TYPE_SECOND_STAGE
    "PERSON_CAR",         // XMEDIA_SVP_ALG_TYPE_PERSON_CAR
    "DMS",                // XMEDIA_SVP_ALG_TYPE_DMS
    "PLATE_DET",          // XMEDIA_SVP_ALG_TYPE_LICENSE_DETECT
    "PLATE_RECO",         // XMEDIA_SVP_ALG_TYPE_LICENSE_RECO
    "PLATE_COLOR",        // XMEDIA_SVP_ALG_TYPE_LICENSE_COLOR
    "VEH_DET",            // XMEDIA_SVP_ALG_TYPE_VEHICLE_DETECT
    "VEH_RECO",           // XMEDIA_SVP_ALG_TYPE_VEHICLE_RECO
    "MAX",                // XMEDIA_SVP_ALG_TYPE_MAX
};

const char* task_type[] = {
    "UAV",                  // XMEDIA_SVP_TASK_UAV
    "DETECT",               // XMEDIA_SVP_TASK_DETECT
    "AOV",                  // XMEDIA_SVP_TASK_AOV
    "PERSON_KPT",           // XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT
    "ADAS",                 // XMEDIA_SVP_TASK_ADAS
    "FIRESMOKE",            // XMEDIA_SVP_TASK_FIRE
    "GESTURE",              // XMEDIA_SVP_TASK_GESTURE
    "FACE_RECOG",           // XMEDIA_SVP_TASK_FACE_RECOGNITON
    "EMOTION",              // XMEDIA_SVP_TASK_EMOTION_CLASSIFITION
    "RCNN",                 // XMEDIA_SVP_TASK_RCNN
    "RCNN_720P",            // XMEDIA_SVP_TASK_RCNN_720P
    "PET_ROBOT",            // XMEDIA_SVP_TASK_COMPANION_ROBOT
    "MULTI_DETECT",         // XMEDIA_SVP_TASK_MULTI_DETECT
    "DMS",                  // XMEDIA_SVP_TASK_DMS
    "PLATE",                // XMEDIA_SVP_TASK_PLATE
    "VEHICLE",              // XMEDIA_SVP_TASK_VEHICLE
    "PLATE_VEH",            // XMEDIA_SVP_TASK_PLATE_VEHICLE
    "MAX",                  // XMEDIA_SVP_TASK_MAX
};

#define SVP_PROC_PRINTF(proc, write_len, fmt...)                                   \
    do {                                                                           \
        int _len = snprintf((proc)->vir_addr, (proc)->remain_buf, ##fmt);          \
        if (_len < 0) {                                                            \
            (write_len) = 0;                                                       \
            break;                                                                 \
        }                                                                          \
        (write_len) = (_len < (proc)->remain_buf) ? _len : (proc)->remain_buf - 1; \
        if ((proc)->remain_buf > 0) {                                              \
            (proc)->vir_addr[(write_len)] = '\0';                                  \
        }                                                                          \
        (proc)->vir_addr += (write_len);                                           \
        (proc)->remain_buf -= (write_len);                                         \
        proc_context->proc_info.remain_buf = (proc)->remain_buf;                   \
    } while (0)

xmedia_s32 svp_update_proc_time(svp_proc_context *proc, xmedia_s32 handle, xmedia_u64 cost_time)
{
    svp_proc_context* proc_context = (svp_proc_context*)proc;
    svp_dbg_info* dbg_info = &proc_context->dbg_info[handle];
    dbg_info->cost_time = cost_time;
    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_update_proc_framerate(svp_proc_context *proc, xmedia_s32 handle)
{
    svp_proc_context* proc_context = (svp_proc_context*)proc;
    svp_dbg_info* dbg_info = &proc_context->dbg_info[handle];
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    dbg_info->fps_count++;

    xmedia_u64 time_diff = (current_time.tv_sec - dbg_info->last_time.tv_sec) * 1000 +
                           (current_time.tv_usec - dbg_info->last_time.tv_usec) / 1000;

    if (time_diff >= 1000) {
        dbg_info->framerate = dbg_info->fps_count;
        dbg_info->fps_count = 0;
        dbg_info->last_time = current_time;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 proc_update_base_info(xmedia_s32 handle,xmedia_svp_task_type task, svp_proc_info *proc_cur,
    proc_detect_info *proc_base_info, svp_dbg_info *dbg_info)
{
    xmedia_u32 write_len = 0;
    SVP_PROC_PRINTF(proc_cur, write_len,
        "%-8d"
        "%-12s"
        "%-12s"
        "%-8.2f"
        "%-8.2f"
        "%-8.2f"
        "%-8d"
        "%-10lld"
        "%-8d"
        "%-10d"
        "%-10d"
        "%-10.2f"
        "%-8d"
        "\n",
        handle,
        task_type[task],
        alg_type[proc_base_info->alg_type],
        proc_base_info->detect_threshold, proc_base_info->iou_threshold,
        proc_base_info->classifier_threshold, proc_base_info->max_target_num,
        dbg_info->cost_time, dbg_info->framerate,
        proc_base_info->track_enable, proc_base_info->movement.enable,
        proc_base_info->movement.stillness_thres, proc_base_info->movement.movement_fps_thres);
    return XMEDIA_SUCCESS;
}

xmedia_s32 proc_update_adas_info(xmedia_s32 handle, xmedia_svp_task_type task,
    svp_proc_info *proc_cur, yolov5_detect_param *detect_info)
{
    xmedia_u32 write_len = 0;
    SVP_PROC_PRINTF(proc_cur, write_len,
        "%-8d"
        "%-12s"
        "%-12s"
        "%-8.2f"
        "%-8.2f"
        "%-8.2f"
        "%-8d"
        "%-10s"
        "%-8s"
        "%-10d"
        "%-10d"
        "%-10.2f"
        "%-8d"
        "\n",
        handle,
        task_type[task],
        alg_type[detect_info->type],
        detect_info->detect_threshold, detect_info->iou_threshold,
        detect_info->classifier_threshold, detect_info->max_target_num,
        "NA", "NA",
        detect_info->tracklet.enable, detect_info->movement.enable,
        detect_info->movement.stillness_thres, detect_info->movement.movement_fps_thres);
    return XMEDIA_SUCCESS;
}

xmedia_s32 proc_update_multi_info(xmedia_s32 handle, xmedia_svp_task_type task,
    svp_proc_info *proc_cur, yolov5_detect_param *detect_info, xmedia_bool flag)
{
    xmedia_u32 write_len = 0;
    SVP_PROC_PRINTF(proc_cur, write_len,
        "%-8d"
        "%-12s"
        "%-14s"
        "%-16d"
        "%-10.2f"
        "%-10.2f"
        "%-10.2f"
        "%-10d"
        "\n",
        handle, task_type[task], alg_type[detect_info->type], flag,
        detect_info->detect_threshold, detect_info->iou_threshold,
        detect_info->classifier_threshold, detect_info->max_target_num);
    return XMEDIA_SUCCESS;
}

xmedia_s32  svp_sys_write_proc(xmedia_handle handle, xmedia_u64 buffer_addr_va, xmedia_u32 buffer_len)
{
    return mpi_sys_write_proc(handle, buffer_addr_va, buffer_len);
}

xmedia_s32 svp_update_proc_info(svp_proc_context* proc, xmedia_s32 handle)
{
    svp_proc_info proc_cur;
    xmedia_u32 write_len = 0;
    proc_detect_info proc_base_info = { 0 };
    if (handle < 0 || handle >= XMEDIA_SVP_MAX_TASK_NUM) {
        SVP_TRACE(MODULE_DBG_ERR, "Invalid handle %d\n", handle);
        return XMEDIA_FAILURE;
    }

    // 获取 proc_context
    svp_proc_context* proc_context = (svp_proc_context*)proc;
    if (proc_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "proc_context is null! \n");
        return XMEDIA_FAILURE;
    }
    proc_context->proc_info.remain_buf = SVP_PROC_SIZE;
    memcpy(&proc_cur, &proc_context->proc_info, sizeof(svp_proc_info));

    SVP_PROC_PRINTF(&proc_cur, write_len,
        "\n------------------------------------------------------SVP_TASK_INFO"
        "-----------------------------------------------------\n");
    SVP_PROC_PRINTF(&proc_cur, write_len,
        "%-8s"
        "%-12s"
        "%-12s"
        "%-8s"
        "%-8s"
        "%-8s"
        "%-8s"
        "%-10s"
        "%-8s"
        "%-10s"
        "%-10s"
        "%-10s"
        "%-8s"
        "\n",
        "Handle", "TaskType", "AlgType", "DetThr", "IouThr", "ClsThr", "MaxNum",
        "CostTime", "FPS", "TrkEn", "MovEn", "StillThr", "MovThr");

    for (xmedia_s32 i = 0; i < XMEDIA_SVP_MAX_TASK_NUM; i++) {
        if (proc_context->handle[i] == XMEDIA_NULL) {
            continue;
        }
        svp_task_handle* task_handle = proc_context->handle[i];
        if (task_handle->used == XMEDIA_TRUE) {
            // 根据任务类型，设置基础信息
            if (task_handle->task_type == XMEDIA_SVP_TASK_DETECT) {
                svp_yolov5n_context* task_context = (svp_yolov5n_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_AOV) {
                svp_yolov5n_aov_context* task_context = (svp_yolov5n_aov_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT) {
                svp_yolov8_context* task_context = (svp_yolov8_context*)task_handle->context;
                yolov8_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_ADAS) {
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-12s"
                    "%-12s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-10lld"
                    "%-8d"
                    "%-10s"
                    "%-10s"
                    "%-10s"
                    "%-8s"
                    "\n",
                    i, "ADAS", "ADAS", "NA", "NA", "NA", "NA",
                    proc_context->dbg_info[i].cost_time,
                    proc_context->dbg_info[i].framerate,
                    "NA", "NA", "NA", "NA");
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_FIRE) {
                svp_fire_context* fire_context = (svp_fire_context*)task_handle->context;
                yolov8_detect_param* detect_info = fire_context->info.fire_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_GESTURE) {
                svp_gesture_context* task_context = (svp_gesture_context*)task_handle->context;
                yolov8_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_FACE_RECOGNITON) {
                svp_fr_context* task_context = (svp_fr_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_EMOTION_CLASSIFITION) {
                svp_fr_context* task_context = (svp_fr_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_RCNN ||
                       task_handle->task_type == XMEDIA_SVP_TASK_RCNN_720P) {
                svp_rcnn_context* task_context = (svp_rcnn_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_COMPANION_ROBOT) {
                svp_companion_robot_context* task_context = (svp_companion_robot_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_MULTI_DETECT) {
                svp_multi_detect_context* task_context = (svp_multi_detect_context*)task_handle->context;
                svp_multi_base_info* base_info = task_context->base_info;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-12s"
                    "%-12s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-10lld"
                    "%-8d"
                    "%-10d"
                    "%-10d"
                    "%-10.2f"
                    "%-8d"
                    "\n",
                    i, "DETECT",  "MULTI", "NA", "NA", "NA", "NA",
                    proc_context->dbg_info[i].cost_time,
                    proc_context->dbg_info[i].framerate,
                    base_info->tracklet.enable, base_info->movement.enable,
                    base_info->movement.stillness_thres, base_info->movement.movement_fps_thres);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_DMS) {
                svp_dms_context *task_context = (svp_dms_context*)task_handle->context;
                yolov8_detect_param *detect_info = task_context->detect_info;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_PLATE) {
                svp_plate_context *task_context = (svp_plate_context*)task_handle->context;
                yolov5_detect_param *detect_info = &task_context->info.detect_info->param;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_VEHICLE) {
                svp_vehicle_context *task_context = (svp_vehicle_context*)task_handle->context;
                yolov5_detect_param *detect_info = &task_context->info.detect_info->param;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_PLATE_VEHICLE) {
                svp_plate_vehicle_context *task_context = (svp_plate_vehicle_context*)task_handle->context;
                yolov5_detect_param *detect_info = &task_context->info.veh_detect->param;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);

                detect_info = &task_context->info.plate_detect->param;
                proc_base_info.alg_type = detect_info->type;
                proc_base_info.classifier_threshold = detect_info->classifier_threshold;
                proc_base_info.detect_threshold = detect_info->detect_threshold;
                proc_base_info.iou_threshold = detect_info->iou_threshold;
                proc_base_info.max_target_num = detect_info->max_target_num;
                proc_base_info.movement = detect_info->movement;
                proc_base_info.track_enable = detect_info->tracklet.enable;
                proc_update_base_info(i, task_handle->task_type, &proc_cur,
                    &proc_base_info, &proc_context->dbg_info[i]);
            } else {
            }
        }
    }

    for (xmedia_s32 i = 0; i < XMEDIA_SVP_MAX_TASK_NUM; i++) {
        if (proc_context->handle[i] == XMEDIA_NULL) {
            continue;
        }
        svp_task_handle* task_handle = proc_context->handle[i];
        if (task_handle->used == XMEDIA_TRUE) {
            // 根据任务类型，设置独有信息
            if (task_handle->task_type == XMEDIA_SVP_TASK_AOV) {
                svp_yolov5n_aov_context* task_context = (svp_yolov5n_aov_context*)task_handle->context;
                yolov5_detect_param* detect_info = task_context->detect_info;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n------------------------------------------------------SVP_AOV_INFO"
                    "-----------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-10s"
                    "\n",
                    "Handle", "Aov target");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-10d"
                    "\n",
                    i, detect_info->aov_only_target);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_ADAS) {
                svp_adas_context* adas_context = (svp_adas_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n-----------------------------------------------------ADAS_DETECT_INFO"
                    "---------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-12s"
                    "%-12s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-10s"
                    "%-8s"
                    "%-10s"
                    "%-10s"
                    "%-10s"
                    "%-8s"
                    "\n",
                    "Handle", "TaskType", "AlgType", "DetThr", "IouThr", "ClsThr", "MaxNum",
                    "CostTime", "FPS", "TrkEn", "MovEn", "StillThr", "MovThr");
                proc_update_adas_info(i, task_handle->task_type, &proc_cur, adas_context->info.car_detect_info);
                proc_update_adas_info(i, task_handle->task_type, &proc_cur, adas_context->info.plate_detect_info);
                proc_update_adas_info(i, task_handle->task_type, &proc_cur, adas_context->info.person_detect_info);
                proc_update_adas_info(i, task_handle->task_type, &proc_cur, adas_context->info.nmv_detect_info);
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n---------------------------------------------------ADAS_DISTANCE_ATTR"
                    "---------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-8s"
                    "%-12s"
                    "%-12s"
                    "%-12s"
                    "\n",
                    "Handle",
                    "Focal",
                    "CarAngle",
                    "CamHeight",
                    "PlateWidth");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-8d"
                    "%-12d"
                    "%-12.2f"
                    "%-12d"
                    "\n",
                    i,
                    adas_context->info.dist_attr.focal_length_px,
                    adas_context->info.dist_attr.car_limit_angle,
                    adas_context->info.dist_attr.camera_height,
                    adas_context->info.dist_attr.plate_width);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_FIRE) {
                svp_fire_context* fire_context = (svp_fire_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n---------------------------------------------------SVP_FIRE&SMOKE_INFO"
                    "--------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-8s"
                    "%-14s"
                    "%-14s"
                    "%-18s"
                    "%-18s"
                    "\n",
                    "Handle", "Type", "change_ratio", "loss_count", "fire_ratio_sum", "smoke_apear_count");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-8s"
                    "%-14.2f"
                    "%-14d"
                    "%-18.2f"
                    "%-18s"
                    "\n",
                    i, "Fire",
                    fire_context->info.fire_attr.fire_change_ratio,
                    fire_context->info.fire_attr.fire_loss_count,
                    fire_context->info.fire_attr.fire_ratio_sum,
                    "NA");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-8s"
                    "%-14.2f"
                    "%-14d"
                    "%-18s"
                    "%-18d"
                    "\n",
                    i, "Smoke",
                    fire_context->info.fire_attr.smoke_change_ratio,
                    fire_context->info.fire_attr.smoke_loss_count,
                    "NA",
                    fire_context->info.fire_attr.smoke_apear_count);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_FACE_RECOGNITON) {
                svp_fr_context* task_context = (svp_fr_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n------------------------------------------------SVP_FACE_RECOGNITON_INFO"
                    "------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-10s"
                    "%-12s"
                    "%-10s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "\n",
                    "Handle", "LumaThr", "Sobel_attr", "KeyPtTHr",
                    "ACWThr", "CWThr", "DWThr", "LThr", "RThr", "UPThre");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-10lld"
                    "%-12d"
                    "%-10.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "\n",
                    i,
                    task_context->luma_thres,
                    task_context->sobel_attr->sobel_thres,
                    task_context->keypoint_info->thres,
                    task_context->pose_attr.anticlockwise_thres,
                    task_context->pose_attr.clockwise_thres,
                    task_context->pose_attr.downward_thres,
                    task_context->pose_attr.left_thres,
                    task_context->pose_attr.right_thres,
                    task_context->pose_attr.upward_thres);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_EMOTION_CLASSIFITION) {
                svp_fr_context* task_context = (svp_fr_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n----------------------------------------------SVP_EMOTION_CLASSIFITION_INFO"
                    "---------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-10s"
                    "%-12s"
                    "%-10s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "%-8s"
                    "\n",
                    "Handle", "LumaThr", "Sobel_attr", "KeyPtTHr",
                    "ACWThr", "CWThr", "DWThr", "LThr", "RThr", "UPThre");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-10lld"
                    "%-12d"
                    "%-10.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "%-8.2f"
                    "\n",
                    i,
                    task_context->luma_thres,
                    task_context->sobel_attr->sobel_thres,
                    task_context->keypoint_info->thres,
                    task_context->pose_attr.anticlockwise_thres,
                    task_context->pose_attr.clockwise_thres,
                    task_context->pose_attr.downward_thres,
                    task_context->pose_attr.left_thres,
                    task_context->pose_attr.right_thres,
                    task_context->pose_attr.upward_thres);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_MULTI_DETECT) {
                svp_multi_detect_context *multi_context = (svp_multi_detect_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n----------------------------------------------------MULTI_DETECT_INFO"
                    "---------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-12s"
                    "%-14s"
                    "%-16s"
                    "%-10s"
                    "%-10s"
                    "%-10s"
                    "%-10s"
                    "\n",
                    "Handle", "TaskType", "AlgType", "SECOND_STAGE",
                    "DetThr", "IouThr", "ClsThr", "MaxNum");
                if (multi_context->use_count.car_flag == XMEDIA_TRUE &&
                    multi_context->info.car_detect_info != XMEDIA_NULL) {
                    proc_update_multi_info(i, task_handle->task_type, &proc_cur,
                        multi_context->info.car_detect_info, multi_context->use_count.rcnn_flag);
                }
                if (multi_context->use_count.person_flag == XMEDIA_TRUE &&
                    multi_context->info.person_detect_info != XMEDIA_NULL) {
                    proc_update_multi_info(i, task_handle->task_type, &proc_cur,
                        multi_context->info.person_detect_info, multi_context->use_count.rcnn_flag);
                }
                if (multi_context->use_count.nmv_flag == XMEDIA_TRUE &&
                    multi_context->info.nmv_detect_info != XMEDIA_NULL) {
                    proc_update_multi_info(i, task_handle->task_type, &proc_cur,
                        multi_context->info.nmv_detect_info, XMEDIA_FALSE);
                }
                if (multi_context->use_count.face_flag == XMEDIA_TRUE &&
                    multi_context->info.face_detect_info != XMEDIA_NULL) {
                    proc_update_multi_info(i, task_handle->task_type, &proc_cur,
                        multi_context->info.face_detect_info, XMEDIA_FALSE);
                }
                if (multi_context->use_count.pet_flag == XMEDIA_TRUE &&
                    multi_context->info.pet_detect_info != XMEDIA_NULL) {
                    proc_update_multi_info(i, task_handle->task_type, &proc_cur,
                        multi_context->info.pet_detect_info, multi_context->use_count.rcnn_flag);
                }
                if (multi_context->use_count.pkg_flag == XMEDIA_TRUE &&
                    multi_context->info.pkg_detect_info != XMEDIA_NULL) {
                    proc_update_multi_info(i, task_handle->task_type, &proc_cur,
                        multi_context->info.pkg_detect_info, XMEDIA_FALSE);
                }
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_DMS) {
                 svp_dms_context *task_context = (svp_dms_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n------------------------------------------------------SVP_DMS_INFO"
                    "------------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-12s"
                    "%-12s"
                    "\n",
                    "Handle", "Warn_Frame", "Iou_Thres");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-12d"
                    "%-12.2f"
                    "\n",
                    i,
                    task_context->warn_frame,
                    task_context->iou_thres);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_PLATE) {
                 svp_plate_context *task_context = (svp_plate_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n-----------------------------------------------------SVP_PLATE_INFO"
                    "-----------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-12s"
                    "%-12s"
                    "%-12s"
                    "\n",
                    "Handle", "Plate_min_W", "Report_Cnt", "Report_Thres");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-12d"
                    "%-12d"
                    "%-12.2f"
                    "\n",
                    i,
                    task_context->info.plate_min_width,
                    task_context->info.report_count,
                    task_context->info.report_thres);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_VEHICLE) {
                 svp_vehicle_context *task_context = (svp_vehicle_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n----------------------------------------------------SVP_VEHICLE_INFO"
                    "----------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-12s"
                    "%-12s"
                    "\n",
                    "Handle", "Report_Cnt", "Report_Thres");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-12d"
                    "%-12.2f"
                    "\n",
                    i,
                    task_context->info.report_count,
                    task_context->info.report_thres);
            } else if (task_handle->task_type == XMEDIA_SVP_TASK_PLATE_VEHICLE) {
                 svp_plate_vehicle_context *task_context = (svp_plate_vehicle_context*)task_handle->context;
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "\n-------------------------------------------------SVP_PLATE_VEHICLE_INFO"
                    "-------------------------------------------------\n");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8s"
                    "%-12s"
                    "%-12s"
                    "%-12s"
                    "\n",
                    "Handle", "Plate_min_W", "Report_Cnt", "Report_Thres");
                SVP_PROC_PRINTF(&proc_cur, write_len,
                    "%-8d"
                    "%-12d"
                    "%-12d"
                    "%-12.2f"
                    "\n",
                    i,
                    task_context->info.plate_min_width,
                    task_context->info.report_count,
                    task_context->info.report_thres);
            }  else {
            }
        }
    }

    // 写入sys proc节点
    svp_sys_write_proc(g_sys_proc, (xmedia_u64)(xmedia_uintptr_t)proc_context->proc_info.vir_addr,
                strlen(proc_context->proc_info.vir_addr));
    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_init_proc(xmedia_svp_context* context)
{
    xmedia_s32 ret;
    xmedia_u32 proc_size = SVP_PROC_SIZE;
    xmedia_char str[SVP_PROC_NAME_SIZE];
    proc_context = (svp_proc_context*)calloc(1, sizeof(svp_proc_context));
    if (proc_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "proc_context calloc failed! \n");
        return XMEDIA_FAILURE;
    }
    pid_t pid = getpid();
    if (pid == -1) {
        SVP_TRACE(MODULE_DBG_ERR, "get pid failed! \n");
        return XMEDIA_FAILURE;
    }
    sprintf(str, "svp_%d", pid);
    *(str + strlen(str)) = '\0';
    ret = mpi_sys_create_proc_entry(str, proc_size, &g_sys_proc);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "get proc handle failed! \n");
        CFREE(proc_context);
        return XMEDIA_FAILURE;
    }
    proc_context->proc_info.vir_addr = malloc(SVP_PROC_SIZE);
    if (proc_context->proc_info.vir_addr == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "alloc proc buf failed!");
        CFREE(proc_context);
        return XMEDIA_FAILURE;
    }
    proc_context->proc_info.remain_buf = SVP_PROC_SIZE;
    proc_context->proc_info.buff_len_total = SVP_PROC_SIZE;
    context->proc = (xmedia_void*)proc_context;
    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_deinit_proc(xmedia_svp_context* context)
{
    svp_proc_context* proc_context = (svp_proc_context*)context->proc;
    if (context == XMEDIA_NULL || context->proc == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }
    (void)mpi_sys_destory_proc(g_sys_proc);
    CFREE(proc_context->proc_info.vir_addr);
    CFREE(proc_context);
    return XMEDIA_SUCCESS;
}
