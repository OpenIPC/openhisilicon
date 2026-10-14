#include "xm_yolov8.h"

static xmedia_void detect_get_class_id(xmedia_svp_yolov8_output *detect_result, xmedia_svp_alg_type type)
{
    xmedia_u32 i;

    for (i = 0; i < detect_result->target_num; i++) {
        if (type == XMEDIA_SVP_ALG_TYPE_PERSON_KEYPOINT) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PERSON_KEYPOINT;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
    }
}

static xmedia_s32 xmyolov8_check_attr(const xmedia_svp_yolov8_attr* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range[0-50] !! \n",
        task_attr->max_target_num);
    return ret;
}

xmedia_s32 xmyolov8_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    svp_yolov8_context *xmyolov8_context = (svp_yolov8_context *)context;
    if (xmyolov8_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 detect init context err !\n");
        return XMEDIA_FAILURE;
    }

    xmyolov8_context->detect_info = (yolov8_detect_param *)calloc(1, sizeof(yolov8_detect_param));
    if (xmyolov8_context->detect_info == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8_detect_info calloc error !\n");
        return XMEDIA_FAILURE;
    }

    // 设置默认参数
    set_default_yolov8_detect_param(xmyolov8_context->detect_info);
    xmyolov8_context->detect_info->w = INPUT_DEFAULT_WIDE;
    xmyolov8_context->detect_info->h = INPUT_DEFAULT_HIGH;
    xmyolov8_context->detect_info->num = DETECTION_MODEL_SINGLE;
    xmyolov8_context->detect_info->keypoint_len = PEOPLE_KEYPOINT_LEN;
    xmyolov8_context->detect_info->type = cfg.modules->alg_type;

    if (cfg.modules->load_mode == XMEDIA_SVP_MODEL_FILE &&
        check_file_exist(cfg.modules->pathname) != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "model pathname err!\n");
        CFREE(xmyolov8_context->detect_info);
        return XMEDIA_FAILURE;
    }

    ret = yolov8_init(xmyolov8_context->detect_info, cfg.modules);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 init detect model failed !\n");
        CFREE(xmyolov8_context->detect_info);
        return ret;
    }

    return ret;
}

xmedia_s32 xmyolov8_detect_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov8_context *xmyolov8_context = (svp_yolov8_context *)context;
    if (xmyolov8_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 detect uninit context err !\n");
        return XMEDIA_FAILURE;
    }

    if (xmyolov8_context->detect_info != XMEDIA_NULL) {
        if (xmyolov8_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = yolov8_uninit(xmyolov8_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 detect deinit failed %#x!\n", ret);
            }
        }
        CFREE(xmyolov8_context->detect_info);
    }

    return ret;
}

xmedia_s32 xmyolov8_detect_set_attr(xmedia_void *context, const xmedia_svp_yolov8_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov8_context *xmyolov8_context = (svp_yolov8_context *)context;
    yolov8_detect_param *detect_info = xmyolov8_context->detect_info;

    ret = xmyolov8_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    detect_info->detect_threshold = task_attr->detect_threshold;
    detect_info->iou_threshold = task_attr->iou_threshold;
    detect_info->max_target_num = task_attr->max_target_num;
    detect_info->thres_desig = desigmoid(detect_info->detect_threshold);
    return ret;
}

xmedia_s32 xmyolov8_detect_get_attr(xmedia_void *context, xmedia_svp_yolov8_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov8_context *xmyolov8_context = (svp_yolov8_context *)context;
    yolov8_detect_param *detect_info = xmyolov8_context->detect_info;

    task_attr->detect_threshold = detect_info->detect_threshold;
    task_attr->iou_threshold = detect_info->iou_threshold;
    task_attr->max_target_num = detect_info->max_target_num;
    return ret;
}

xmedia_s32 xmyolov8_detect_process(xmedia_void *context, const xmedia_svp_task_input *input,
                                   xmedia_svp_yolov8_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    // xmedia_s32 i = 0;
    // xmedia_float w = 0;

    svp_yolov8_context *xmyolov8_context = (svp_yolov8_context *)context;
    if (xmyolov8_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 context err !\n");
        return ret;
    }
    yolov8_detect_param *detect_info = xmyolov8_context->detect_info;
    ret = detect_yolov8_process(detect_info, input->frame, output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 detect process err !\n");
        return ret;
    }
    detect_get_class_id(output, detect_info->type);

#if 0
    printf("###################### [%d] \n", output->target_num);
    if (output->target_num > 0) {
       for (xmedia_s32 i = 0; i < output->target_num; i++) {
          int x1 = (xmedia_s32)roundf(output->targets[i].rect.x1 / 2) * 2;
          int y1 = (xmedia_s32)roundf(output->targets[i].rect.y1 / 2) * 2;
          int x2 = (xmedia_s32)roundf(output->targets[i].rect.x2 / 2) * 2;
          int y2 = (xmedia_s32)roundf(output->targets[i].rect.y2 / 2) * 2;
          printf("##################### x1[%d]y1[%d]x2[%d]y2[%d]s[%.2f]\n",x1,y1,x2,y2,output->targets[i].detect_score);
       }
    }
#endif

#if 0
    if (output->target_num != 0) {
        ret = svp_send_info_to_venc(detect_info, output, &input_image);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "send venc rect failed ! ret=0x%x \n", ret);
        }
        ret = svp_update_luma(detect_info, output, &input_image);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "ai update luma output failed, ret=0x%x\n", ret);
        }
    }
#endif

#if 0
    if (detect_info->tracklet.enable == XMEDIA_TRUE) {
        xmedia_svp_tracker_result result = {0};
        result.target_num = output->target_num;
        for(i = 0; i < result.target_num; i++) {
            result.tracker_single[i].class_type = output->targets[i].class_type;
            result.tracker_single[i].detect_score = output->targets[i].detect_score;
            result.tracker_single[i].classfier_score = output->targets[i].classfier_score;
            result.tracker_single[i].tracker_id = output->targets[i].tracker_id;
            result.tracker_single[i].tracker_age = output->targets[i].tracker_age;
            result.tracker_single[i].rect = output->targets[i].rect;
        }
        ret = svp_detect_bytetracker(&result, &detect_info->tracklet, &detect_info->tarck_id_grow,
                                     detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        memset(output, 0, sizeof(xmedia_svp_yolov8_output));
        for(i = 0; i < result.target_num; i++) {
            output->targets[output->target_num].alg_type = detect_info->type;
            output->targets[output->target_num].class_type = result.tracker_single[i].class_type;
            output->targets[output->target_num].detect_score = result.tracker_single[i].detect_score;
            output->targets[output->target_num].classfier_score = result.tracker_single[i].classfier_score;
            output->targets[output->target_num].tracker_id = result.tracker_single[i].tracker_id;
            output->targets[output->target_num].tracker_age = result.tracker_single[i].tracker_age;
            output->targets[output->target_num].rect = result.tracker_single[i].rect;
            output->target_num++;
        }
    }

    if ((detect_info->movement.enable == XMEDIA_TRUE) && (output->target_num != 0)) {
        svp_movement_result result = { 0 };
        result.target_num = output->target_num;
        for (i = 0; i < result.target_num; i++) {
            result.targets[i].class_type = output->targets[i].class_type;
            result.targets[i].tracker_id = output->targets[i].tracker_id;
            result.targets[i].rect = output->targets[i].rect;
        }
        ret = svp_detect_movement(&detect_info->movement, &result);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "movement filter failed, ret=0x%x\n", ret);
        }
        for (i = 0; i < result.target_num; i++) {
            output->targets[i].motion_state = result.targets[i].motion_state;
            output->targets[i].rect = result.targets[i].rect;
        }
    }
#endif

    return ret;
}
