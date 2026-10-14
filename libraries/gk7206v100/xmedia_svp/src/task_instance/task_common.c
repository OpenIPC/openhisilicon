#include "task_common.h"

xmedia_s32 svp_task_common_create(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (type) {
        //case XMEDIA_SVP_TASK_UAV:
            // task->context = (xmedia_svp_uav_context *)calloc(1, sizeof(xmedia_svp_uav_context));
            // ret = uav_hover_init(task->context, cfg);
            // break;
        case XMEDIA_SVP_TASK_DETECT:
            task->context = (svp_yolov5n_context*)calloc(1, sizeof(svp_yolov5n_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = xmyolov5n_detect_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_AOV:
            task->context = (svp_yolov5n_aov_context*)calloc(1, sizeof(svp_yolov5n_aov_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = xmyolov5n_aov_detect_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT:
            task->context = (svp_yolov8_context*)calloc(1, sizeof(svp_yolov8_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = xmyolov8_detect_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_ADAS:
            task->context = (svp_adas_context*)calloc(1, sizeof(svp_adas_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = adas_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_FIRE:
            task->context = (svp_fire_context*)calloc(1, sizeof(svp_fire_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = fire_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_GESTURE:
            task->context = (svp_gesture_context*)calloc(1, sizeof(svp_gesture_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = gesture_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_FACE_RECOGNITON:
        case XMEDIA_SVP_TASK_EMOTION_CLASSIFITION:
            task->context = (svp_fr_context*)calloc(1, sizeof(svp_fr_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = xmface_recognition_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_RCNN:
            task->context = (svp_rcnn_context*)calloc(1, sizeof(svp_rcnn_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = rcnn_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_RCNN_720P:
            task->context = (svp_rcnn_720p_context*)calloc(1, sizeof(svp_rcnn_720p_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = rcnn_720p_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_COMPANION_ROBOT:
            task->context = (svp_companion_robot_context*)calloc(1, sizeof(svp_companion_robot_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = companion_robot_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_MULTI_DETECT:
            task->context = (svp_multi_detect_context*)calloc(1, sizeof(svp_multi_detect_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = multi_detect_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_DMS:
            task->context = (svp_dms_context*)calloc(1, sizeof(svp_dms_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = dms_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_PLATE:
            task->context = (svp_plate_context*)calloc(1, sizeof(svp_plate_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = plate_recognition_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_VEHICLE:
            task->context = (svp_vehicle_context*)calloc(1, sizeof(svp_vehicle_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = vehicle_recognition_init(task->context, cfg);
            break;
        case XMEDIA_SVP_TASK_PLATE_VEHICLE:
            task->context = (svp_plate_vehicle_context*)calloc(1, sizeof(svp_plate_vehicle_context));
            CHECK_CALLOC_RETURN_ERROR(task->context);
            ret = plate_vehicle_recognition_init(task->context, cfg);
            break;
        default:
            SVP_TRACE(MODULE_DBG_ERR, "create task_type[%u] not support\n", cfg.task_type);
            return XMEDIA_FAILURE;
    }
    if (ret != XMEDIA_SUCCESS) {
        CFREE(task->context);
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] common init failed!!!\n", cfg.task_type);
        return ret;
    }

    return ret;
}

xmedia_s32 svp_task_common_set(svp_task_handle* task, xmedia_svp_task_type type, const xmedia_void* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (type) {
        // case XMEDIA_SVP_TASK_UAV:
        //     xmedia_svp_uav_attr *uav_attr = (xmedia_svp_uav_attr *)task_attr;
        //     ret = uav_hover_set_attr(task->context, uav_attr);
        //     break;
        case XMEDIA_SVP_TASK_DETECT:
            xmedia_svp_yolov5_attr* attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = xmyolov5n_detect_set_attr(task->context, attr);
            break;
        case XMEDIA_SVP_TASK_AOV:
            xmedia_svp_aov_attr* aov_attr = (xmedia_svp_aov_attr*)task_attr;
            ret = xmyolov5n_aov_detect_set_attr(task->context, aov_attr);
            break;
        case XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT:
            xmedia_svp_yolov8_attr* yolov8_attr = (xmedia_svp_yolov8_attr*)task_attr;
            ret = xmyolov8_detect_set_attr(task->context, yolov8_attr);
            break;
        case XMEDIA_SVP_TASK_ADAS:
            xmedia_svp_adas_attr* adas_attr = (xmedia_svp_adas_attr*)task_attr;
            ret = adas_set_attr(task->context, adas_attr);
            break;
        case XMEDIA_SVP_TASK_FIRE:
            xmedia_svp_fire_attr* fire_attr = (xmedia_svp_fire_attr*)task_attr;
            ret = fire_set_attr(task->context, fire_attr);
            break;
        case XMEDIA_SVP_TASK_GESTURE:
            xmedia_svp_yolov8_attr* gesture_attr = (xmedia_svp_yolov8_attr*)task_attr;
            ret = gesture_set_attr(task->context, gesture_attr);
            break;
        case XMEDIA_SVP_TASK_FACE_RECOGNITON:
        case XMEDIA_SVP_TASK_EMOTION_CLASSIFITION:
            xmedia_svp_fr_attr* fr_attr = (xmedia_svp_fr_attr*)task_attr;
            ret = xmface_recognition_set_attr(task->context, fr_attr);
            break;
        case XMEDIA_SVP_TASK_RCNN:
            xmedia_svp_yolov5_attr* rcnn_attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = rcnn_set_attr(task->context, rcnn_attr);
            break;
        case XMEDIA_SVP_TASK_RCNN_720P:
            xmedia_svp_yolov5_attr* rcnn_720p_attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = rcnn_720p_set_attr(task->context, rcnn_720p_attr);
            break;
        case XMEDIA_SVP_TASK_COMPANION_ROBOT:
            xmedia_svp_yolov5_attr* robot_attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = companion_robot_set_attr(task->context, robot_attr);
            break;
        case XMEDIA_SVP_TASK_MULTI_DETECT:
            xmedia_svp_multi_detect_attr* multi_attr = (xmedia_svp_multi_detect_attr*)task_attr;
            ret = multi_detect_set_attr(task->context, multi_attr);
            break;
        case XMEDIA_SVP_TASK_DMS:
            xmedia_svp_dms_attr* dms_attr = (xmedia_svp_dms_attr*)task_attr;
            ret = dms_set_attr(task->context, dms_attr);
            break;
        case XMEDIA_SVP_TASK_PLATE:
            xmedia_svp_plate_reco_attr* plate_attr = (xmedia_svp_plate_reco_attr*)task_attr;
            ret = plate_recognition_set_attr(task->context, plate_attr);
            break;
        case XMEDIA_SVP_TASK_VEHICLE:
            xmedia_svp_vehicle_reco_attr* veh_attr = (xmedia_svp_vehicle_reco_attr*)task_attr;
            ret = vehicle_recognition_set_attr(task->context, veh_attr);
            break;
        case XMEDIA_SVP_TASK_PLATE_VEHICLE:
            xmedia_svp_plate_vehicle_attr* pv_attr = (xmedia_svp_plate_vehicle_attr*)task_attr;
            ret = plate_vehicle_recognition_set_attr(task->context, pv_attr);
            break;
        default:
            SVP_TRACE(MODULE_DBG_ERR, "set task_type[%u] not support\n", task->task_type);
            return XMEDIA_FAILURE;
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] common set failed!!!\n", task->task_type);
        return ret;
    }

    return ret;
}

xmedia_s32 svp_task_common_get(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_void *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (type) {
        // case XMEDIA_SVP_TASK_UAV:
        //     xmedia_svp_uav_attr *uav_attr = (xmedia_svp_uav_attr *)task_attr;
        //     memset(uav_attr, 0x00, sizeof(xmedia_svp_uav_attr));
        //     ret = uav_hover_get_attr(task->context, uav_attr);
        //     break;
        case XMEDIA_SVP_TASK_DETECT:
            xmedia_svp_yolov5_attr *attr = (xmedia_svp_yolov5_attr *)task_attr;
            ret = xmyolov5n_detect_get_attr(task->context, attr);
            break;
        case XMEDIA_SVP_TASK_AOV:
            xmedia_svp_aov_attr *aov_attr = (xmedia_svp_aov_attr *)task_attr;
            ret = xmyolov5n_aov_detect_get_attr(task->context, aov_attr);
            break;
        case XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT:
            xmedia_svp_yolov8_attr *yolov8_attr = (xmedia_svp_yolov8_attr *)task_attr;
            ret = xmyolov8_detect_get_attr(task->context, yolov8_attr);
            break;
        case XMEDIA_SVP_TASK_ADAS:
            xmedia_svp_adas_attr *adas_attr = (xmedia_svp_adas_attr *)task_attr;
            ret = adas_get_attr(task->context, adas_attr);
            break;
        case XMEDIA_SVP_TASK_FIRE:
            xmedia_svp_fire_attr *fire_attr = (xmedia_svp_fire_attr *)task_attr;
            ret = fire_get_attr(task->context, fire_attr);
            break;
        case XMEDIA_SVP_TASK_GESTURE:
            xmedia_svp_yolov8_attr *gesture_attr = (xmedia_svp_yolov8_attr *)task_attr;
            ret = gesture_get_attr(task->context, gesture_attr);
            break;
        case XMEDIA_SVP_TASK_FACE_RECOGNITON:
        case XMEDIA_SVP_TASK_EMOTION_CLASSIFITION:
            xmedia_svp_fr_attr* fr_attr = (xmedia_svp_fr_attr*)task_attr;
            ret = xmface_recognition_get_attr(task->context, fr_attr);
            break;
        case XMEDIA_SVP_TASK_RCNN:
            xmedia_svp_yolov5_attr* rcnn_attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = rcnn_get_attr(task->context, rcnn_attr);
            break;
        case XMEDIA_SVP_TASK_RCNN_720P:
            xmedia_svp_yolov5_attr* rcnn_720p_attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = rcnn_720p_get_attr(task->context, rcnn_720p_attr);
            break;
        case XMEDIA_SVP_TASK_COMPANION_ROBOT:
            xmedia_svp_yolov5_attr* robot_attr = (xmedia_svp_yolov5_attr*)task_attr;
            ret = companion_robot_get_attr(task->context, robot_attr);
            break;
        case XMEDIA_SVP_TASK_MULTI_DETECT:
            xmedia_svp_multi_detect_attr* multi_attr = (xmedia_svp_multi_detect_attr*)task_attr;
            ret = multi_detect_get_attr(task->context, multi_attr);
            break;
        case XMEDIA_SVP_TASK_DMS:
            xmedia_svp_dms_attr* dms_attr = (xmedia_svp_dms_attr*)task_attr;
            ret = dms_get_attr(task->context, dms_attr);
            break;
        case XMEDIA_SVP_TASK_PLATE:
            xmedia_svp_plate_reco_attr* plate_attr = (xmedia_svp_plate_reco_attr*)task_attr;
            ret = plate_recognition_get_attr(task->context, plate_attr);
            break;
        case XMEDIA_SVP_TASK_VEHICLE:
            xmedia_svp_vehicle_reco_attr* veh_attr = (xmedia_svp_vehicle_reco_attr*)task_attr;
            ret = vehicle_recognition_get_attr(task->context, veh_attr);
            break;
        case XMEDIA_SVP_TASK_PLATE_VEHICLE:
            xmedia_svp_plate_vehicle_attr* pv_attr = (xmedia_svp_plate_vehicle_attr*)task_attr;
            ret = plate_vehicle_recognition_get_attr(task->context, pv_attr);
            break;
            break;
        default:
            SVP_TRACE(MODULE_DBG_ERR, "get task_type[%u] not support\n", task->task_type);
            return XMEDIA_FAILURE;
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] common get failed!!!\n", task->task_type);
        return ret;
    }

    return ret;
}

xmedia_s32 svp_task_common_process(svp_task_handle *task, xmedia_svp_task_type type, const xmedia_svp_task_input *input,
                                   const xmedia_void *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (type) {
        // case XMEDIA_SVP_TASK_UAV:
        //     xmedia_svp_uav_output *uav_result = (xmedia_svp_uav_output *)output;
        //     ret = uav_hover_process(task->context, input, uav_result);
        //     break;
        case XMEDIA_SVP_TASK_DETECT:
            xmedia_svp_yolov5_output *detect_output = (xmedia_svp_yolov5_output *)output;
            ret = xmyolov5n_detect_process(task->context, input, detect_output);
            break;
        case XMEDIA_SVP_TASK_AOV:
            xmedia_svp_aov_detect_output *aov_output = (xmedia_svp_aov_detect_output *)output;
            ret = xmyolov5n_aov_detect_process(task->context, input, aov_output);
            break;
        case XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT:
            xmedia_svp_yolov8_output* detect_yolov8_output = (xmedia_svp_yolov8_output*)output;
            ret = xmyolov8_detect_process(task->context, input, detect_yolov8_output);
            break;
        case XMEDIA_SVP_TASK_ADAS:
            xmedia_svp_adas_result* adas_result = (xmedia_svp_adas_result*)output;
            ret = adas_process(task->context, input, adas_result);
            break;
        case XMEDIA_SVP_TASK_FIRE:
            xmedia_svp_yolov8_output* fire_result = (xmedia_svp_yolov8_output*)output;
            ret = fire_process(task->context, input, fire_result);
            break;
        case XMEDIA_SVP_TASK_GESTURE:
            xmedia_svp_gesture_result* gesture_result = (xmedia_svp_gesture_result*)output;
            ret = gesture_process(task->context, input, gesture_result);
            break;
        case XMEDIA_SVP_TASK_FACE_RECOGNITON:
        case XMEDIA_SVP_TASK_EMOTION_CLASSIFITION:
            ret = xmface_recognition_process(task->context, input, output);
            break;
        case XMEDIA_SVP_TASK_RCNN:
            xmedia_svp_yolov5_output* rcnn_output = (xmedia_svp_yolov5_output*)output;
            ret = rcnn_process(task->context, input, rcnn_output);
            break;
        case XMEDIA_SVP_TASK_RCNN_720P:
            xmedia_svp_yolov5_output* rcnn_720p_output = (xmedia_svp_yolov5_output*)output;
            ret = rcnn_720p_process(task->context, input, rcnn_720p_output);
            break;
        case XMEDIA_SVP_TASK_COMPANION_ROBOT:
            xmedia_svp_companion_robot_output *robot_output = (xmedia_svp_companion_robot_output *)output;
            ret = companion_robot_process(task->context, input, robot_output);
            break;
        case XMEDIA_SVP_TASK_MULTI_DETECT:
            xmedia_svp_yolov5_output *multi_output = (xmedia_svp_yolov5_output *)output;
            ret = multi_detect_process(task->context, input, multi_output);
            break;
        case XMEDIA_SVP_TASK_DMS:
            xmedia_svp_dms_output *dms_output = (xmedia_svp_dms_output *)output;
            ret = dms_process(task->context, input, dms_output);
            break;
        case XMEDIA_SVP_TASK_PLATE:
            xmedia_svp_plate_reco_output *plate_output = (xmedia_svp_plate_reco_output *)output;
            ret = plate_recognition_process(task->context, input, plate_output);
            break;
        case XMEDIA_SVP_TASK_VEHICLE:
            xmedia_svp_vehicle_reco_output *veh_output = (xmedia_svp_vehicle_reco_output *)output;
            ret = vehicle_recognition_process(task->context, input, veh_output);
            break;
        case XMEDIA_SVP_TASK_PLATE_VEHICLE:
            xmedia_svp_plate_vehicle_output *pv_output = (xmedia_svp_plate_vehicle_output *)output;
            ret = plate_vehicle_recognition_process(task->context, input, pv_output);
            break;
        default:
            SVP_TRACE(MODULE_DBG_ERR, "process task_type[%u] not support\n", task->task_type);
            return XMEDIA_FAILURE;
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] common process failed!!!\n", task->task_type);
        return ret;
    }

    return ret;
}

xmedia_s32 svp_task_common_destroy(svp_task_handle* task, xmedia_svp_task_type type)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (type) {
        // case XMEDIA_SVP_TASK_UAV:
        //     ret = uav_hover_uninit(task->context);
        //     break;
        case XMEDIA_SVP_TASK_DETECT:
            ret = xmyolov5n_detect_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_AOV:
            ret = xmyolov5n_aov_detect_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_DETECT_AND_KEYPOINT:
            ret = xmyolov8_detect_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_ADAS:
            ret = adas_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_FIRE:
            ret = fire_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_GESTURE:
            ret = gesture_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_FACE_RECOGNITON:
        case XMEDIA_SVP_TASK_EMOTION_CLASSIFITION:
            ret = xmface_recognition_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_RCNN:
            ret = rcnn_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_RCNN_720P:
            ret = rcnn_720p_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_COMPANION_ROBOT:
            ret = companion_robot_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_MULTI_DETECT:
            ret = multi_detect_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_DMS:
            ret = dms_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_PLATE:
            ret = plate_recognition_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_VEHICLE:
            ret = vehicle_recognition_uninit(task->context);
            break;
        case XMEDIA_SVP_TASK_PLATE_VEHICLE:
            ret = plate_vehicle_recognition_uninit(task->context);
            break;
        default:
            SVP_TRACE(MODULE_DBG_ERR, "destroy task_type[%u] not support\n", task->task_type);
            return XMEDIA_FAILURE;
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] common destroy failed!!!\n", task->task_type);
        return ret;
    }
    CFREE(task->context);

    return ret;
}
