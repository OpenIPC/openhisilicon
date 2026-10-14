#include "xm_yolov5n.h"

// 具体见训练的 .yaml文件中的锚点
// 人形
static xmedia_svp_detect_anchors pd_anchors_four = {
    .anchors = {{{5.0f, 10.0f}, {8.0f, 19.0f}, {14.0f, 28.0f}},
               {{22.0f, 48.0f}, {30.0f, 79.0f}, {53.0f, 128.0f}},
               {{61.0f, 70.0f}, {91.0f, 197.0f}, {128.0f, 284.0f}},
               {{144.0f, 86.0f}, {185.0f, 178.0f}, {227.0f, 302.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};
// 人脸
static xmedia_svp_detect_anchors fd_anchors_four = {
    .anchors = {{{6.0f, 7.0f}, {9.0f, 11.0f}, {13.0f, 16.0f}},
               {{18.0f, 23.0f}, {26.0f, 33.0f}, {37.0f, 47.0f}},
               {{54.0f, 67.0f}, {77.0f, 104.0f}, {112.0f, 154.0f}},
               {{174.0f, 238.0f}, {258.0f, 355.0f}, {455.0f, 568.0f}}},
    .layer_num = LAYER_NUM_FOUR,
 };
// 机动车
static xmedia_svp_detect_anchors cd_anchors_four = {
    .anchors = {{{4.0f, 3.0f}, {9.0f, 7.0f}, {12.0f, 9.0f}},
               {{20.0f, 14.0f}, {24.0f, 18.0f}, {41.0f, 28.0f}},
               {{60.0f, 44.0f}, {91.0f, 94.0f}, {104.0f, 51.0f}},
               {{174.0f, 90.0f}, {219.0f, 181.0f}, {457.0f, 310.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};
// 非机动车
static xmedia_svp_detect_anchors ncd_anchors_three = {
    .layer_num = LAYER_MIN_NUM,
    .anchors = {{{16.0f, 13.0f}, {30.0f, 24.0f}, {48.0f, 37.0f}},
                {{75.0f, 59.0f}, {117.0f, 99.0f}, {181.0f, 161.0f}},
                {{298.0f, 243.0f}, {197.0f, 384.0f}, {327.0f, 579.0f}}},
};
// 宠物
static xmedia_svp_detect_anchors ptd_anchors_four = {
    .anchors = {{{19.0f, 16.0f}, {38.0f, 33.0f}, {49.0f, 36.0f}},
               {{67.0f, 68.0f}, {98.0f, 73.0f}, {134.0f, 137.0f}},
               {{262.0f, 116.0f}, {218.0f, 219.0f}, {374.0f, 187.0f}},
               {{357.0f, 318.0f}, {543.0f, 229.0f}, {598.0f, 345.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};
// 头肩检测
static xmedia_svp_detect_anchors hd_anchors_four = {
    .anchors = {{{5.0f, 6.0f}, {8.0f, 9.0f}, {16.0f, 18.0f}},
               {{25.0f, 25.0f}, {36.0f, 37.0f}, {53.0f, 53.0f}},
               {{84.0f, 83.0f}, {112.0f, 92.0f}, {174.0f, 143.0f}},
               {{147.0f, 137.0f}, {162.0f, 194.0f}, {258.0f, 213.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};
// 烟火
static xmedia_svp_detect_anchors fwd_anchors_four = {
    .anchors = {{{5.0f, 10.0f}, {8.0f, 19.0f}, {14.0f, 28.0f}},
                {{22.0f, 48.0f}, {30.0f, 79.0f}, {53.0f, 128.0f}},
                {{61.0f, 70.0f}, {91.0f, 97.0f}, {128.0f, 284.0f}},
                {{144.0f, 86.0f}, {185.0f, 178.0f}, {227.0f, 302.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};
// 包裹
static xmedia_svp_detect_anchors pkd_anchors_three = {
    .anchors = {{{28.0f, 24.0f}, {65.0f, 51.0f}, {108.0f, 92.0f}},
                {{167.0f, 110.0f}, {129.0f, 189.0f}, {208.0f, 162.0f}},
                {{263.0f, 207.0f}, {178.0f, 298.0f}, {324.0f, 294.0f}}},
    .layer_num = LAYER_MIN_NUM,
};

// 人车
static xmedia_svp_detect_anchors pcd_anchors_four = {
    .anchors = {{{13.0f, 15.0f}, {27.0f, 21.0f}, {43.0f, 31.0f}},
                {{43.0f, 31.0f}, {64.0f, 43.0f}, {36.0f, 83.0f}},
                {{93.0f, 62.0f}, {140.0f, 89.0f}, {88.0f, 150.0f}},
                {{206.0f, 143.0f}, {141.0f, 261.0f}, {298.0f, 285.0f}}},
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
        if (type == XMEDIA_SVP_ALG_TYPE_FACE) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_FACE;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_CAR) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_CAR;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_PET) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PET;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_HEAD) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_HEAD;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_BIKE;
            } else if (detect_result->targets[i].class_type == 1) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_BIKER;
            } else if (detect_result->targets[i].class_type == 2) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MOTOR;
            } else if (detect_result->targets[i].class_type == 3) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MOTORER;
            } else if (detect_result->targets[i].class_type == 4) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_TRICYCLE;
            } else if (detect_result->targets[i].class_type == 5) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_TRICYCLER;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_PACKAGE) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PACKAGE;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_FIREWORKS) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_FIREWORKS_FIRE;
            } else if (detect_result->targets[i].class_type == 1) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_FIREWORKS_SMOKE;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
        if (type == XMEDIA_SVP_ALG_TYPE_PERSON_CAR) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PERSON;
            } else if (detect_result->targets[i].class_type == 1) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_CAR;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        }
    }
}

static xmedia_s32 xmyolov5n_check_attr(const xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_STILLNESS_THRES(task_attr->stillness_thres, "stillness_thres[%.2f] out of range(0.6-1) !! \n",
        task_attr->stillness_thres);
    SVP_CHECK_U8(task_attr->movement_fps_thres, "movement_fps_thres[%.d] out of range(0-255) !! \n",
        task_attr->movement_fps_thres);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range(0-50) !! \n",
        task_attr->max_target_num);
    SVP_CHECK_ATTR_ENABLE(task_attr->bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");
    SVP_CHECK_ATTR_ENABLE(task_attr->motionless_filter_enable, "motionless_filter_enable must be 0 or 1 !! \n");
    if (task_attr->motionless_filter_enable == XMEDIA_TRUE && task_attr->bytetrack_enable == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "motionless_filter[%d] depends on bytetrack[%d] ! \n",task_attr->bytetrack_enable,
                  task_attr->motionless_filter_enable);
        return XMEDIA_FAILURE;
    }
    SVP_CHECK_ATTR_ENABLE(task_attr->smart_ae_enable, "smart_ae_enable must be 0 or 1 !! \n");
    SVP_CHECK_ATTR_ENABLE(task_attr->smart_venc_enable, "smart_venc_enable must be 0 or 1 !! \n");

    return ret;
}

xmedia_s32 xmyolov5n_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    svp_yolov5n_context *xmyolov5n_context = (svp_yolov5n_context *)context;
    if (xmyolov5n_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n detect init context err !\n");
        return XMEDIA_FAILURE;
    }
    if (cfg.module_num != 1) {
        SVP_TRACE(MODULE_DBG_ERR, "detect model num err!\n");
        return XMEDIA_FAILURE;
    }

    xmyolov5n_context->detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    if (xmyolov5n_context->detect_info == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "yolov5_detect_info calloc error !\n");
        return XMEDIA_FAILURE;
    }

    xmyolov5n_context->detect_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    if (xmyolov5n_context->detect_info->tracklet.tracker == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "tracker calloc error !\n");
        CFREE(xmyolov5n_context->detect_info);
        return XMEDIA_FAILURE;
    }

    // 设置默认参数
    set_default_yolov5_detect_param(xmyolov5n_context->detect_info);
    xmyolov5n_context->detect_info->w = INPUT_DEFAULT_WIDE;
    xmyolov5n_context->detect_info->h = INPUT_DEFAULT_HIGH;
    xmyolov5n_context->detect_info->num = DETECTION_MODEL_SINGLE;

   if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        xmyolov5n_context->detect_info->anchors = pd_anchors_four;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_CAR) {
        xmyolov5n_context->detect_info->anchors = cd_anchors_four;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE) {
        xmyolov5n_context->detect_info->num =DETECTION_MODEL_SIX;
        xmyolov5n_context->detect_info->anchors = ncd_anchors_three;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_PET) {
        xmyolov5n_context->detect_info->anchors = ptd_anchors_four;
    } else  if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_HEAD) {
        xmyolov5n_context->detect_info->anchors = hd_anchors_four;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_FACE) {
        xmyolov5n_context->detect_info->anchors = fd_anchors_four;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_FIREWORKS) {
        xmyolov5n_context->detect_info->num = DETECTION_MODEL_DOUBLE;
        xmyolov5n_context->detect_info->anchors = fwd_anchors_four;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_PACKAGE) {
        xmyolov5n_context->detect_info->anchors = pkd_anchors_three;
    } else if (cfg.modules->alg_type == XMEDIA_SVP_ALG_TYPE_PERSON_CAR) {
        xmyolov5n_context->detect_info->num = DETECTION_MODEL_DOUBLE;
        xmyolov5n_context->detect_info->anchors = pcd_anchors_four;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "task_type error ! anchor not found!\n");
        CFREE(xmyolov5n_context->detect_info->tracklet.tracker);
        CFREE(xmyolov5n_context->detect_info);
        return XMEDIA_FAILURE;
    }

    if (cfg.modules == XMEDIA_SVP_MODEL_FILE &&
        check_file_exist(cfg.modules->pathname) != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "model pathname err!\n");
        CFREE(xmyolov5n_context->detect_info->tracklet.tracker);
        CFREE(xmyolov5n_context->detect_info);
        return XMEDIA_FAILURE;
    }

    ret = detect_init(xmyolov5n_context->detect_info, cfg.modules->alg_type,
            cfg.modules, xmyolov5n_context->detect_info->anchors.layer_num);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "head init detect model failed !\n");
        CFREE(xmyolov5n_context->detect_info->tracklet.tracker);
        CFREE(xmyolov5n_context->detect_info);
        return ret;
    }

    return ret;
}

xmedia_s32 xmyolov5n_detect_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_yolov5n_context *xmyolov5n_context = (svp_yolov5n_context *)context;
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
        CFREE(xmyolov5n_context->detect_info->tracklet.tracker);
        CFREE(xmyolov5n_context->detect_info);
    }

    return ret;
}

xmedia_s32 xmyolov5n_detect_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 i;
    svp_yolov5n_context *xmyolov5n_context = (svp_yolov5n_context *)context;
    yolov5_detect_param *detect_info = xmyolov5n_context->detect_info;

    ret = xmyolov5n_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    detect_info->detect_threshold = task_attr->detect_threshold;
    detect_info->iou_threshold = task_attr->iou_threshold;
    detect_info->max_target_num = task_attr->max_target_num;
    detect_info->classifier_threshold = task_attr->classifier_threshold;
    detect_info->tracklet.enable = task_attr->bytetrack_enable;

    detect_info->thres_desig = desigmoid(detect_info->detect_threshold);
    detect_info->movement.enable = task_attr->motionless_filter_enable;
    if (task_attr->motionless_filter_enable == XMEDIA_TRUE) {
        SVP_CHECK_ATTR_STILLNESS_THRES(task_attr->stillness_thres, "stillness_thres[%.2f] out of range(0.6-1) !! \n",
            task_attr->stillness_thres);
        SVP_CHECK_U8(task_attr->movement_fps_thres, "movement_fps_thres[%.d] out of range(0-255) !! \n",
            task_attr->movement_fps_thres);
    }
    detect_info->movement.stillness_thres = task_attr->stillness_thres;
    detect_info->movement.movement_fps_thres = task_attr->movement_fps_thres;
    detect_info->smart_ae_enable = task_attr->smart_ae_enable;
    detect_info->smart_venc_enable = task_attr->smart_venc_enable;
    if (task_attr->smart_venc_enable == XMEDIA_TRUE) {
        for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
            detect_info->smart_venc_array[i] = task_attr->smart_venc_array[i];
        }
    } else {
        for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
            detect_info->smart_venc_array[i] = XMEDIA_FALSE;
        }
    }
    if (task_attr->smart_ae_enable == XMEDIA_TRUE) {
        for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
            detect_info->smart_ae_array[i] = task_attr->smart_ae_array[i];
        }
    } else {
        for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
            detect_info->smart_ae_array[i] = XMEDIA_FALSE;
        }
    }

    return ret;
}

xmedia_s32 xmyolov5n_detect_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 i;
    svp_yolov5n_context *xmyolov5n_context = (svp_yolov5n_context *)context;
    yolov5_detect_param *detect_info = xmyolov5n_context->detect_info;

    task_attr->detect_threshold = detect_info->detect_threshold;
    task_attr->iou_threshold = detect_info->iou_threshold;
    task_attr->max_target_num = detect_info->max_target_num;
    task_attr->classifier_threshold = detect_info->classifier_threshold;
    task_attr->bytetrack_enable = detect_info->tracklet.enable;
    task_attr->motionless_filter_enable = detect_info->movement.enable;
    task_attr->stillness_thres = detect_info->movement.stillness_thres;
    task_attr->movement_fps_thres = detect_info->movement.movement_fps_thres;
    task_attr->smart_ae_enable = detect_info->smart_ae_enable;
    task_attr->smart_venc_enable = detect_info->smart_venc_enable;
    for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
        task_attr->smart_ae_array[i] = detect_info->smart_ae_array[i];
    }
    for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
        task_attr->smart_venc_array[i] = detect_info->smart_venc_array[i];
    }

    return ret;
}

xmedia_s32 xmyolov5n_detect_process(xmedia_void *context, const xmedia_svp_task_input *input,
                                    xmedia_svp_yolov5_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_s32 i = 0;
    // xmedia_float w = 0;

    svp_yolov5n_context *xmyolov5n_context = (svp_yolov5n_context *)context;
    yolov5_detect_param *detect_info = xmyolov5n_context->detect_info;

    if (xmyolov5n_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n_detect_process context err !\n");
        return ret;
    }
    ret = detect_process(detect_info, input->frame, output, detect_info->anchors);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov5n detect process err !\n");
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
        ret = svp_detect_bytetracker(&result,
                                     &detect_info->tracklet,
                                     &detect_info->tarck_id_grow,
                                     detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        memset(output, 0, sizeof(xmedia_svp_yolov5_output));
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

    if (output->target_num != 0 &&
        (detect_info->type == XMEDIA_SVP_ALG_TYPE_PERSON ||
         detect_info->type == XMEDIA_SVP_ALG_TYPE_FACE)) {

        if (detect_info->smart_venc_enable == XMEDIA_TRUE) {
            ret = svp_send_info_to_venc(detect_info, output, input);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "svp smart venc failed! ret=0x%x \n", ret);
            }
        }

        if (detect_info->smart_ae_enable == XMEDIA_TRUE) {
            ret = svp_update_luma(detect_info, output, input);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "svp smart ae failed! ret=0x%x\n", ret);
            }
        }
    }

    return ret;
}
