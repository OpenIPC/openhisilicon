#include "xm_yolov5n_aov.h"

// 具体见训练的 .yaml文件中的锚点
// 人形AOV
static xmedia_svp_detect_anchors pd_anchors_aov = {
    .anchors = {{{5.0f, 10.0f}, {8.0f, 19.0f}, {14.0f, 28.0f}},
               {{22.0f, 48.0f}, {30.0f, 79.0f}, {53.0f, 128.0f}},
               {{61.0f, 70.0f}, {91.0f, 197.0f}, {128.0f, 284.0f}},
               {{144.0f, 86.0f}, {185.0f, 178.0f}, {227.0f, 302.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};

static xmedia_void detect_get_class_id(xmedia_svp_yolov5_output *detect_result, xmedia_svp_alg_type type)
{
    xmedia_u32 i;

    for (i = 0; i < detect_result->target_num; i++) {
        if (type == XMEDIA_SVP_ALG_TYPE_PERSON) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PERSON;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
    }
}

static xmedia_s32 xmyolov5n_aov_check_attr(const xmedia_svp_aov_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range(0-50) !! \n",
        task_attr->max_target_num);
    SVP_CHECK_ATTR_ENABLE(task_attr->aov_only_target, "aov_only_target must be 0 or 1 !! \n");

    return ret;
}

xmedia_s32 xmyolov5n_aov_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    svp_yolov5n_aov_context *xmyolov5n_context = (svp_yolov5n_aov_context *)context;
    if (xmyolov5n_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n detect init context err !\n");
        return XMEDIA_FAILURE;
    }

    xmyolov5n_context->detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    if (xmyolov5n_context->detect_info == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "yolov5_detect_info calloc error !\n");
        return XMEDIA_FAILURE;
    }
    // 设置默认参数
    set_default_yolov5_detect_param(xmyolov5n_context->detect_info);
    xmyolov5n_context->detect_info->w = INPUT_DEFAULT_WIDE;
    xmyolov5n_context->detect_info->h = INPUT_DEFAULT_HIGH;
    xmyolov5n_context->detect_info->num = DETECTION_MODEL_SINGLE;
    xmyolov5n_context->detect_info->aov_flag = XMEDIA_TRUE;   // aov model flag

   if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        xmyolov5n_context->detect_info->anchors = pd_anchors_aov;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "task_type error ! anchor not found!\n");
        CFREE(xmyolov5n_context->detect_info);
        return XMEDIA_FAILURE;
    }

    if (cfg.modules->load_mode == XMEDIA_SVP_MODEL_FILE &&
        check_file_exist(cfg.modules->pathname) != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "model pathname err!\n");
        CFREE(xmyolov5n_context->detect_info);
        return XMEDIA_FAILURE;
    }

    ret = detect_init(xmyolov5n_context->detect_info, cfg.modules->alg_type,
            cfg.modules, xmyolov5n_context->detect_info->anchors.layer_num);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "head init detect model failed !\n");
        CFREE(xmyolov5n_context->detect_info);
        return ret;
    }

    return ret;
}

xmedia_s32 xmyolov5n_aov_detect_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov5n_aov_context *xmyolov5n_context = (svp_yolov5n_aov_context *)context;
    if (xmyolov5n_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n detect uninit context err !\n");
        return XMEDIA_FAILURE;
    }

    if (xmyolov5n_context->detect_info != XMEDIA_NULL) {
        if (xmyolov5n_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(xmyolov5n_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "xm yolov5n detect deinit failed %#x!\n", ret);
            }
        }
        CFREE(xmyolov5n_context->detect_info);
    }

    return ret;
}

xmedia_s32 xmyolov5n_aov_detect_set_attr(xmedia_void *context, const xmedia_svp_aov_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov5n_aov_context *xmyolov5n_context = (svp_yolov5n_aov_context *)context;
    yolov5_detect_param *detect_info = xmyolov5n_context->detect_info;

    ret = xmyolov5n_aov_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    detect_info->detect_threshold = task_attr->detect_threshold;
    detect_info->iou_threshold = task_attr->iou_threshold;
    detect_info->max_target_num = task_attr->max_target_num;
    detect_info->classifier_threshold = task_attr->classifier_threshold;
    detect_info->aov_only_target = task_attr->aov_only_target;
    detect_info->thres_desig = desigmoid(detect_info->detect_threshold);

    return ret;
}

xmedia_s32 xmyolov5n_aov_detect_get_attr(xmedia_void *context, xmedia_svp_aov_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov5n_aov_context *xmyolov5n_context = (svp_yolov5n_aov_context *)context;
    yolov5_detect_param *detect_info = xmyolov5n_context->detect_info;

    task_attr->detect_threshold = detect_info->detect_threshold;
    task_attr->iou_threshold = detect_info->iou_threshold;
    task_attr->max_target_num = detect_info->max_target_num;
    task_attr->classifier_threshold = detect_info->classifier_threshold;
    task_attr->aov_only_target = detect_info->aov_only_target;

    return ret;
}

static xmedia_s32 svp_get_aov_only_target(yolov5_detect_param *param, xmedia_bool *xmedia_is_exist)
{
    xmedia_s32 i, j;
    xmedia_u8 *addr;
    xmedia_u8 thres_u8;
    for (i = 0; i < param->anchors.layer_num; i++) {
        addr = (xmedia_u8 *)param->model.output.tensor[i*2+1].addr;
        thres_u8 = quantize(param->thres_desig, param->quanlize[i*2].scale, param->quanlize[i*2].zp);
        for (j = 0; j < 3; j++) {
            if (addr[j] > thres_u8) {
                *xmedia_is_exist = XMEDIA_TRUE;
                break;
            }
        }
        if (*xmedia_is_exist == XMEDIA_TRUE) {
            break;
        }
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 xmyolov5n_aov_detect_process(xmedia_void *context, const xmedia_svp_task_input *input,
                                    xmedia_svp_aov_detect_output *aov_output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    // xmedia_s32 i = 0;
    // xmedia_float w = 0;
    xmedia_bool xmedia_is_exist = XMEDIA_FALSE;

    svp_yolov5n_aov_context *xmyolov5n_context = (svp_yolov5n_aov_context *)context;
    yolov5_detect_param *detect_info = xmyolov5n_context->detect_info;
    memset(aov_output, 0x0, sizeof(xmedia_svp_aov_detect_output));

    if (xmyolov5n_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n_detect_process context err !\n");
        return ret;
    }

    ret = detect_process(detect_info, input->frame, &aov_output->detect_output, detect_info->anchors);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n detect process err !\n");
        return ret;
    }

    ret = svp_get_aov_only_target(detect_info, &xmedia_is_exist);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "svp_get_aov_only_target result failed, ret=0x%x\n", ret);
    }
    aov_output->is_exist = xmedia_is_exist;

    if (detect_info->aov_only_target == XMEDIA_FALSE) {
        detect_get_class_id(&aov_output->detect_output, detect_info->type);
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

        if (detect_info->tracklet.enable == XMEDIA_TRUE) {
            ret = svp_bytetracker(output, &detect_info->tracklet, detect_info, &g_pd_tracker_idx, g_pd_id_flag);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
            }
        }

        memset(&result_temp, 0, sizeof(xmedia_svp_yolov5_output));
        if ((output->target_num != 0) && (detect_info->movement_result.svp_movement_enable == XMEDIA_TRUE)) {
            ret = svp_detect_movement(&detect_info->movement_result, output, detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_NOTICE, "movement filter failed, ret=0x%x\n", ret);
            }
            for (i = 0, j = 0; i < output->target_num; i++) {
                if (output->targets[i].detect_score - SVP_EPSILON <= 0) {
                    continue;
                }
                result_temp.targets[j++] = output->targets[i];
            }
            result_temp.target_num = j;
            memcpy(output, &result_temp, sizeof(xmedia_svp_yolov5_output));
        }

        if ((resuoutputlt->target_num != 0) && (detect_info->roi.enable == XMEDIA_TRUE)) {
            ret = svp_get_roi_result(detect_info, output, XMEDIA_NULL);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_NOTICE, "roi filter failed, ret=0x%x\n", ret);
            }
        }
#endif
    }

    return ret;
}
