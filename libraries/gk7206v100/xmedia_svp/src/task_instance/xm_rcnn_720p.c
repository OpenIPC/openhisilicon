#include "xm_rcnn_720p.h"

static xmedia_svp_detect_anchors cd_anchors = {
    .anchors = {{{4.0f, 3.0f}, {9.0f, 7.0f}, {12.0f, 9.0f}},
               {{20.0f, 14.0f}, {24.0f, 18.0f}, {41.0f, 28.0f}},
               {{60.0f, 44.0f}, {91.0f, 94.0f}, {104.0f, 51.0f}},
               {{174.0f, 90.0f}, {219.0f, 181.0f}, {457.0f, 310.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};

static xmedia_svp_detect_anchors pd_anchors_four = {
    .anchors = {{{5.0f, 10.0f}, {8.0f, 19.0f}, {14.0f, 28.0f}},
               {{22.0f, 48.0f}, {30.0f, 79.0f}, {53.0f, 128.0f}},
               {{61.0f, 70.0f}, {91.0f, 197.0f}, {128.0f, 284.0f}},
               {{144.0f, 86.0f}, {185.0f, 178.0f}, {227.0f, 302.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};

static xmedia_svp_detect_anchors pet_anchors = {
    .anchors = {{{8.0f, 8.0f}, {16.0f, 16.0f}, {25.0f, 26.0f}},
               {{48.0f, 44.0f}, {62.0f, 76.0f}, {96.0f, 80.0f}},
               {{112.0f, 154.0f}, {208.0f, 148.0f}, {175.0f, 249.0f}},
               {{247.0f, 331.0f}, {380.0f, 333.0f}, {531.0f, 315.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};

// 后面放svp_context里面
typedef struct {
    xmedia_u8 rcnn_count;
    xmedia_npu_model model;
} xmedia_svp_rcnn_model;

static xmedia_svp_rcnn_model rcnn_model = {0};

static xmedia_void detect_get_rcnn_class_id(xmedia_svp_yolov5_output *detect_result,
    xmedia_svp_alg_type type)
{
    for (xmedia_s32 i = 0; i < detect_result->target_num; i++) {
        if (type == XMEDIA_SVP_ALG_TYPE_PET) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PET;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        } else if (type == XMEDIA_SVP_ALG_TYPE_PERSON) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PERSON;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        } else if (type == XMEDIA_SVP_ALG_TYPE_CAR) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_CAR;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "rcnn does not support this type[%d]\n", type);
        }
    }
}

xmedia_s32 rcnn_720p_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_svp_modules *tmp_module = XMEDIA_NULL;
    svp_rcnn_720p_context *rcnn_context = (svp_rcnn_720p_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(rcnn_context == XMEDIA_NULL, "rcnn_init context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(cfg.module_num != RCNN_MODEL_NUM, "module num err !\n");

    // 检测模型
    tmp_module = cfg.modules;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT0, "detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT0, "detect model pathname err!\n");

    rcnn_context->detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_GOTO_ERROR(rcnn_context->detect_info, EXIT0);

    rcnn_context->detect_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    CHECK_SVP_COND_GOTO_ERROR(rcnn_context->detect_info->tracklet.tracker == XMEDIA_NULL,
        EXIT1, "tracker calloc failed !\n");

    set_default_yolov5_detect_param(rcnn_context->detect_info);
    rcnn_context->detect_info->w = INPUT_MID_WIDE;
    rcnn_context->detect_info->h = INPUT_MID_HIGH;
    rcnn_context->detect_info->num = DETECTION_MODEL_SINGLE;
    if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        rcnn_context->detect_info->anchors = pd_anchors_four;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_CAR) {
        rcnn_context->detect_info->anchors = cd_anchors;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PET) {
        rcnn_context->detect_info->anchors = pet_anchors;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "rcnn does not support this type[%d]\n", tmp_module->alg_type);
        goto EXIT2;
    }
    ret = detect_init(rcnn_context->detect_info, tmp_module->alg_type,
        tmp_module, rcnn_context->detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT2, "detect init model failed !\n");

    // 二阶段模型
    tmp_module = cfg.modules + 1;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT3, "second stage model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT3, "rcnn_model pathname err!\n");

    if (rcnn_model.rcnn_count == 0) {
        ret = npu_load_model(tmp_module, &rcnn_model.model);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT3, "second stage model load err!\n");
    }

    rcnn_model.rcnn_count += 1;
    rcnn_context->second_stage_flag = XMEDIA_TRUE;
    rcnn_context->detect_info->classifier_threshold = RCNN_CLASSIFY_THR;
    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "img_resize", &rcnn_context->resize_phyaddr, &rcnn_context->resize_viraddr,
        RCNN_RECO_FRAME_SIZE * RCNN_RECO_FRAME_SIZE * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(rcnn_context->resize_phyaddr == 0, EXIT4, "img_resize mmz alloc fail !\n");
    return ret;

EXIT4:
    ret = npu_unload_model(&rcnn_model.model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "rcnn unload model failed ! \n");
    }
EXIT3:
    ret = detect_deinit(rcnn_context->detect_info);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
    }
EXIT2:
    CFREE(rcnn_context->detect_info->tracklet.tracker);
EXIT1:
    CFREE(rcnn_context->detect_info);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 rcnn_720p_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_rcnn_720p_context *rcnn_context = (svp_rcnn_720p_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(rcnn_context == XMEDIA_NULL, "rcnn_uninit context err !\n");
    if (rcnn_context->resize_phyaddr != 0) {
        svp_mmz_unmap_and_free(rcnn_context->resize_phyaddr, rcnn_context->resize_viraddr);
    }

    if (rcnn_model.rcnn_count > 0 && rcnn_context->second_stage_flag == XMEDIA_TRUE) {
        rcnn_model.rcnn_count -= 1;
    }

    if (rcnn_model.rcnn_count == 0) {
        ret = npu_unload_model(&rcnn_model.model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "rcnn unload model failed ! \n");
        }
    }

    if (rcnn_context->detect_info != XMEDIA_NULL) {
        if (rcnn_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(rcnn_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(rcnn_context->detect_info->tracklet.tracker);
        CFREE(rcnn_context->detect_info);
    }
    return ret;
}

xmedia_s32 rcnn_720p_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 i;
    svp_rcnn_720p_context *rcnn_context = (svp_rcnn_720p_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(rcnn_context == XMEDIA_NULL, "rcnn_set_attr context err !\n");

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range(0-50) !! \n",
        task_attr->max_target_num);
    SVP_CHECK_ATTR_ENABLE(task_attr->bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");
    SVP_CHECK_ATTR_ENABLE(task_attr->motionless_filter_enable, "motionless_filter_enable must be 0 or 1 !! \n");
    if (task_attr->motionless_filter_enable == XMEDIA_TRUE && task_attr->bytetrack_enable == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "motionless_filter[%d] depends on bytetrack[%d] ! \n", task_attr->bytetrack_enable,
            task_attr->motionless_filter_enable);
        return XMEDIA_FAILURE;
    }
    if (task_attr->motionless_filter_enable == XMEDIA_TRUE) {
        SVP_CHECK_ATTR_STILLNESS_THRES(task_attr->stillness_thres, "stillness_thres[%.2f] out of range(0.6-1) !! \n",
            task_attr->stillness_thres);
        SVP_CHECK_U8(task_attr->movement_fps_thres, "movement_fps_thres[%.d] out of range(0-255) !! \n",
            task_attr->movement_fps_thres);
    }
    SVP_CHECK_ATTR_ENABLE(task_attr->smart_ae_enable, "smart_ae_enable must be 0 or 1 !! \n");
    SVP_CHECK_ATTR_ENABLE(task_attr->smart_venc_enable, "smart_venc_enable must be 0 or 1 !! \n");

    rcnn_context->detect_info->detect_threshold = task_attr->detect_threshold;
    rcnn_context->detect_info->iou_threshold = task_attr->iou_threshold;
    rcnn_context->detect_info->max_target_num = task_attr->max_target_num;
    rcnn_context->detect_info->classifier_threshold = task_attr->classifier_threshold;
    rcnn_context->detect_info->tracklet.enable = task_attr->bytetrack_enable;
    rcnn_context->detect_info->tracklet.high_score_thres = task_attr->detect_threshold;
    rcnn_context->detect_info->thres_desig = desigmoid(rcnn_context->detect_info->detect_threshold);
    rcnn_context->detect_info->movement.enable = task_attr->motionless_filter_enable;
    rcnn_context->detect_info->movement.stillness_thres = task_attr->stillness_thres;
    rcnn_context->detect_info->movement.movement_fps_thres = task_attr->movement_fps_thres;
    rcnn_context->detect_info->smart_ae_enable = task_attr->smart_ae_enable;
    rcnn_context->detect_info->smart_venc_enable = task_attr->smart_venc_enable;
    if (task_attr->smart_venc_enable == XMEDIA_TRUE) {
        for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
            SVP_CHECK_ATTR_ENABLE(task_attr->smart_venc_array[i], "smart_venc_array must be 0 or 1 !! \n");
            rcnn_context->detect_info->smart_venc_array[i] = task_attr->smart_venc_array[i];
        }
    } else {
        for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
            rcnn_context->detect_info->smart_venc_array[i] = XMEDIA_FALSE;
        }
    }
    if (task_attr->smart_ae_enable == XMEDIA_TRUE) {
        for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
            SVP_CHECK_ATTR_ENABLE(task_attr->smart_ae_array[i], "smart_ae_array must be 0 or 1 !! \n");
            rcnn_context->detect_info->smart_ae_array[i] = task_attr->smart_ae_array[i];
        }
    } else {
        for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
            rcnn_context->detect_info->smart_ae_array[i] = XMEDIA_FALSE;
        }
    }

    return ret;
}

xmedia_s32 rcnn_720p_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 i;
    svp_rcnn_720p_context *rcnn_context = (svp_rcnn_720p_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(rcnn_context == XMEDIA_NULL, "rcnn_get_attr context err !\n");

    task_attr->detect_threshold = rcnn_context->detect_info->detect_threshold;
    task_attr->iou_threshold = rcnn_context->detect_info->iou_threshold;
    task_attr->max_target_num = rcnn_context->detect_info->max_target_num;
    task_attr->classifier_threshold = rcnn_context->detect_info->classifier_threshold;
    task_attr->bytetrack_enable = rcnn_context->detect_info->tracklet.enable;
    task_attr->motionless_filter_enable = rcnn_context->detect_info->movement.enable;
    task_attr->stillness_thres = rcnn_context->detect_info->movement.stillness_thres;
    task_attr->movement_fps_thres = rcnn_context->detect_info->movement.movement_fps_thres;
    task_attr->smart_ae_enable = rcnn_context->detect_info->smart_ae_enable;
    task_attr->smart_venc_enable = rcnn_context->detect_info->smart_venc_enable;
    for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
        task_attr->smart_ae_array[i] = rcnn_context->detect_info->smart_ae_array[i];
    }
    for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
        task_attr->smart_venc_array[i] = rcnn_context->detect_info->smart_venc_array[i];
    }

    return ret;
}

xmedia_s32 rcnn_720p_process(xmedia_void *context, const xmedia_svp_task_input *input, xmedia_svp_yolov5_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 i;
    xmedia_svp_yolov5_output tmp_result = {0};

    svp_rcnn_720p_context *rcnn_context = (svp_rcnn_720p_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(rcnn_context == XMEDIA_NULL, "rcnn_process context err !\n");

    ret = detect_process(rcnn_context->detect_info, input->frame,
        output, rcnn_context->detect_info->anchors);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "detect process err !\n");

#ifdef RCNN_DEBUG_SAVE_FILE
    SVP_TRACE(MODULE_DBG_ERR, "num[%d] \n", output->target_num);
#endif

    detect_get_rcnn_class_id(output, rcnn_context->detect_info->type);

    for(i = 0; i < output->target_num; i++) {
        ret = svp_padding_resize(input->frame, &output->targets[i].rect, rcnn_context->resize_phyaddr,
            rcnn_context->resize_viraddr, RCNN_RECO_FRAME_SIZE, RCNN_RECO_FRAME_SIZE);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "svp_padding_resize failed with [%d] \n", ret);
            goto EXIT;
        }

        ret = npu_run_model(&rcnn_model.model, rcnn_context->resize_phyaddr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "npu_run_model error !!\n");

        xmedia_u8 *addr;
        xmedia_float arr[RCNN_CLASSIFY_COUNT] = { 0 };
        xmedia_float result_arr[RCNN_CLASSIFY_COUNT] = { 0 };
        addr = (xmedia_u8*)rcnn_model.model.output.tensor[0].addr;
        for (xmedia_u8 m = 0; m < RCNN_CLASSIFY_COUNT; m++) {
            arr[m] = dequantize(addr[m], rcnn_model.model.output.tensor[0].quant.scale,
                rcnn_model.model.output.tensor[0].quant.zp);
        }
        softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);
#ifdef RCNN_DEBUG_SAVE_FILE
        for (xmedia_u8 j = 0; j < RCNN_CLASSIFY_COUNT; j++) {
            SVP_TRACE(MODULE_DBG_ERR, "rcnn_num[%d]result_arr[%.4f]\n", i, result_arr[j]);
        }
#endif
        if (rcnn_context->detect_info->type == XMEDIA_SVP_ALG_TYPE_PERSON) {
            if (result_arr[SVP_CLASS_RCNN_PERSON] >= rcnn_context->detect_info->classifier_threshold) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.targets[tmp_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_PERSON];
                tmp_result.target_num++;
            } else if (result_arr[SVP_CLASS_RCNN_BIKER] >= rcnn_context->detect_info->classifier_threshold) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.targets[tmp_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_BIKER];
                tmp_result.target_num++;
            } else if (result_arr[SVP_CLASS_RCNN_MOTORER] >= rcnn_context->detect_info->classifier_threshold) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.targets[tmp_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_MOTORER];
                tmp_result.target_num++;
            } else if (result_arr[SVP_CLASS_RCNN_TRICYCLER] >= rcnn_context->detect_info->classifier_threshold) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.targets[tmp_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_TRICYCLER];
                tmp_result.target_num++;
            }
        } else if (rcnn_context->detect_info->type == XMEDIA_SVP_ALG_TYPE_PET) {
            if (result_arr[SVP_CLASS_RCNN_PET] >= rcnn_context->detect_info->classifier_threshold) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.targets[tmp_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_PET];
                tmp_result.target_num++;
            }
        } else if (rcnn_context->detect_info->type == XMEDIA_SVP_ALG_TYPE_CAR) {
            if (result_arr[SVP_CLASS_RCNN_CAR] >= rcnn_context->detect_info->classifier_threshold) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.targets[tmp_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_CAR];
                tmp_result.target_num++;
            }
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "error detect type[%d] !\n", rcnn_context->detect_info->type);
        }
    }

    // 第二次iou过滤
    xmedia_u32 endnum = 0;
    det_nms_cross(tmp_result.targets, tmp_result.target_num, RCNN_IOU_THR, &endnum);
    output->target_num = 0;
    for (xmedia_u8 i = 0; i < endnum; i++) {
        output->targets[output->target_num] = tmp_result.targets[i];
        output->target_num += 1;
        if (output->target_num >= rcnn_context->detect_info->max_target_num) {
            break;
        }
    }

    if (rcnn_context->detect_info->tracklet.enable == XMEDIA_TRUE) {
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
        ret = svp_detect_bytetracker(&result,
                                     &rcnn_context->detect_info->tracklet,
                                     &rcnn_context->detect_info->tarck_id_grow,
                                     rcnn_context->detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        memset(output, 0, sizeof(xmedia_svp_yolov5_output));
        for(i = 0; i < result.target_num; i++) {
            output->targets[output->target_num].alg_type = rcnn_context->detect_info->type;
            output->targets[output->target_num].class_type = result.tracker_single[i].class_type;
            output->targets[output->target_num].detect_score = result.tracker_single[i].detect_score;
            output->targets[output->target_num].classfier_score = result.tracker_single[i].classfier_score;
            output->targets[output->target_num].tracker_id = result.tracker_single[i].tracker_id;
            output->targets[output->target_num].tracker_age = result.tracker_single[i].tracker_age;
            output->targets[output->target_num].rect = result.tracker_single[i].rect;
            output->target_num++;
        }
    }

    if ((rcnn_context->detect_info->movement.enable == XMEDIA_TRUE) && (output->target_num != 0)) {
        svp_movement_result result = { 0 };
        result.target_num = output->target_num;
        for (i = 0; i < result.target_num; i++) {
            result.targets[i].class_type = output->targets[i].class_type;
            result.targets[i].tracker_id = output->targets[i].tracker_id;
            result.targets[i].rect = output->targets[i].rect;
        }
        ret = svp_detect_movement(&rcnn_context->detect_info->movement, &result);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "movement filter failed, ret=0x%x\n", ret);
        }
        for (i = 0; i < result.target_num; i++) {
            output->targets[i].motion_state = result.targets[i].motion_state;
            output->targets[i].rect = result.targets[i].rect;
        }
    }

    if (output->target_num != 0 && rcnn_context->detect_info->type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        if (rcnn_context->detect_info->smart_venc_enable == XMEDIA_TRUE) {
            ret = svp_send_info_to_venc(rcnn_context->detect_info, output, input);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "svp smart venc failed! ret=0x%x \n", ret);
            }
        }

        if (rcnn_context->detect_info->smart_ae_enable == XMEDIA_TRUE) {
            ret = svp_update_luma(rcnn_context->detect_info, output, input);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "svp smart ae failed! ret=0x%x\n", ret);
            }
        }
    }

    return XMEDIA_SUCCESS;

EXIT:

    return XMEDIA_FAILURE;
}

