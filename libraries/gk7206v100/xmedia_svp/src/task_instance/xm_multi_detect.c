#include "xm_multi_detect.h"

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

// 包裹
static xmedia_svp_detect_anchors pkd_anchors_three = {
    .anchors = {{{28.0f, 24.0f}, {65.0f, 51.0f}, {108.0f, 92.0f}},
                {{167.0f, 110.0f}, {129.0f, 189.0f}, {208.0f, 162.0f}},
                {{263.0f, 207.0f}, {178.0f, 298.0f}, {324.0f, 294.0f}}},
    .layer_num = LAYER_MIN_NUM,
};

static inline xmedia_s32 base_check_attr(const xmedia_svp_multi_base_attr *task_attr)
{
    CHECK_SVP_COND_RETURN_ERROR(task_attr == XMEDIA_NULL, "task_attr NULL!\n");
    xmedia_s32 ret = XMEDIA_SUCCESS;
    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range[0-50] !! \n",
        task_attr->max_target_num);
    return ret;
}

static inline xmedia_s32 base_set_attr(yolov5_detect_param *param, const xmedia_svp_multi_base_attr *task_attr)
{
    CHECK_SVP_COND_RETURN_ERROR(param == XMEDIA_NULL, "param NULL!\n");
    CHECK_SVP_COND_RETURN_ERROR(task_attr == XMEDIA_NULL, "task_attr NULL!\n");
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret = base_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }
    param->detect_threshold = task_attr->detect_threshold;
    param->classifier_threshold = task_attr->classifier_threshold;
    param->iou_threshold = task_attr->iou_threshold;
    param->max_target_num = task_attr->max_target_num;
    return ret;
}

static inline xmedia_s32 base_get_attr(yolov5_detect_param *param, xmedia_svp_multi_base_attr *task_attr)
{
    CHECK_SVP_COND_RETURN_ERROR(param == XMEDIA_NULL, "param NULL!\n");
    CHECK_SVP_COND_RETURN_ERROR(task_attr == XMEDIA_NULL, "task_attr NULL!\n");
    task_attr->detect_threshold = param->detect_threshold;
    task_attr->classifier_threshold = param->classifier_threshold;
    task_attr->iou_threshold = param->iou_threshold;
    task_attr->max_target_num = param->max_target_num;
    return XMEDIA_SUCCESS;
}

static inline xmedia_s32 detect_get_class_id(xmedia_svp_yolov5_output *detect_result, xmedia_svp_alg_type type)
{
    CHECK_SVP_COND_RETURN_ERROR(detect_result == XMEDIA_NULL, "detect_result NULL!\n");
    xmedia_s32 ret = XMEDIA_SUCCESS;
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
    }
    return ret;
}

static inline xmedia_s32 set_default_base_info(svp_multi_base_info *param)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    CHECK_SVP_COND_RETURN_ERROR(param == XMEDIA_NULL, "param NULL!\n");
    if (param) {
        param->movement.enable = XMEDIA_FALSE;
        param->movement.stillness_thres = SVP_STILLNESS_THRES;
        param->movement.movement_fps_thres = SVP_MOVEMENT_FPS_THRES;
        param->movement.num = (XMEDIA_SVP_MAX_TARGET_NUM * SVP_MOVELESS_COUNT);
        param->tracklet.enable = XMEDIA_TRUE;
        param->tracklet.high_score_thres = DEFAULT_DETECT_THRESHOLD;
        param->tracklet.activated_tracker_thres = SVP_ACTIVATED_TRACKER_THRESHOLD;
        param->tracklet.tracked_tracker_thres = SVP_TRACKED_TRACKER_THRESHOLD;
        param->tracklet.unactivated_tracker_thres = SVP_UNACTIVATED_TRACKER_THRESHOLD;
        param->tracklet.track_age_thres = SVP_TRACK_AGE_THRESHOLD;
        memset(param->tarck_id_arry, 0, sizeof(xmedia_bool) * XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
        param->tarck_id_grow = -1;
    }
    return ret;
}

static inline xmedia_s32 set_default_flag(svp_multi_detect_context *multi_context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "multi_context NULL!\n");
    multi_context->use_count.person_flag = XMEDIA_FALSE;
    multi_context->info.person_detect_info = XMEDIA_NULL;
    multi_context->use_count.car_flag = XMEDIA_FALSE;
    multi_context->info.car_detect_info = XMEDIA_NULL;
    multi_context->use_count.nmv_flag = XMEDIA_FALSE;
    multi_context->info.nmv_detect_info = XMEDIA_NULL;
    multi_context->use_count.face_flag = XMEDIA_FALSE;
    multi_context->info.face_detect_info = XMEDIA_NULL;
    multi_context->use_count.pet_flag = XMEDIA_FALSE;
    multi_context->info.pet_detect_info = XMEDIA_NULL;
    multi_context->use_count.pkg_flag = XMEDIA_FALSE;
    multi_context->info.pkg_detect_info = XMEDIA_NULL;
    multi_context->use_count.rcnn_flag = XMEDIA_FALSE;
    return ret;
}

static inline xmedia_s32 combine_outputs(xmedia_svp_yolov5_output *multi_out,xmedia_svp_yolov5_output *tmp_reslut)
{
    CHECK_SVP_COND_RETURN_ERROR(multi_out == XMEDIA_NULL, "multi_out NULL!\n");
    CHECK_SVP_COND_RETURN_ERROR(tmp_reslut == XMEDIA_NULL, "tmp_reslut NULL!\n");
    xmedia_s32 ret = XMEDIA_SUCCESS;
    if (tmp_reslut->target_num > 0) {
        for (xmedia_u8 i = 0; i < tmp_reslut->target_num; i++) {
            if (multi_out->target_num >= XMEDIA_SVP_MAX_TARGET_NUM) {
                SVP_TRACE(MODULE_DBG_ERR, "index out of range [%d] \n", XMEDIA_SVP_MAX_TARGET_NUM);
                break;
            }
            multi_out->targets[multi_out->target_num] = tmp_reslut->targets[i];
            multi_out->target_num ++;
        }
    }
    tmp_reslut->target_num = 0;
    return ret;
}

static inline xmedia_s32 multi_algtype_init(xmedia_svp_modules *tmp_module, svp_multi_detect_context *multi_context)
{
    CHECK_SVP_COND_RETURN_ERROR(tmp_module == XMEDIA_NULL, "tmp_module NULL!\n");
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "multi_context NULL!\n");
    xmedia_s32 ret = XMEDIA_SUCCESS;
    if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE) {
        multi_context->info.nmv_detect_info = (yolov5_detect_param*)calloc(1, sizeof(yolov5_detect_param));
        CHECK_CALLOC_RETURN_ERROR(multi_context->info.nmv_detect_info);
        set_default_yolov5_detect_param(multi_context->info.nmv_detect_info);
        multi_context->info.nmv_detect_info->w = INPUT_DEFAULT_WIDE;
        multi_context->info.nmv_detect_info->h = INPUT_DEFAULT_HIGH;
        multi_context->info.nmv_detect_info->num = DETECTION_MODEL_SIX;
        multi_context->info.nmv_detect_info->anchors = ncd_anchors_three;
        ret = detect_init(multi_context->info.nmv_detect_info, tmp_module->alg_type,
            tmp_module, multi_context->info.nmv_detect_info->anchors.layer_num);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "nmv detect init failed\n");
        multi_context->use_count.nmv_flag = XMEDIA_TRUE;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_CAR) {
        multi_context->info.car_detect_info = (yolov5_detect_param*)calloc(1, sizeof(yolov5_detect_param));
        CHECK_CALLOC_RETURN_ERROR(multi_context->info.car_detect_info);
        set_default_yolov5_detect_param(multi_context->info.car_detect_info);
        multi_context->info.car_detect_info->w = INPUT_DEFAULT_WIDE;
        multi_context->info.car_detect_info->h = INPUT_DEFAULT_HIGH;
        multi_context->info.car_detect_info->num = DETECTION_MODEL_SINGLE;
        multi_context->info.car_detect_info->anchors = cd_anchors_four;
        ret = detect_init(multi_context->info.car_detect_info, tmp_module->alg_type,
            tmp_module, multi_context->info.car_detect_info->anchors.layer_num);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "car detect init failed\n");
        multi_context->use_count.car_flag = XMEDIA_TRUE;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        multi_context->info.person_detect_info = (yolov5_detect_param*)calloc(1, sizeof(yolov5_detect_param));
        CHECK_CALLOC_RETURN_ERROR(multi_context->info.person_detect_info);
        set_default_yolov5_detect_param(multi_context->info.person_detect_info);
        multi_context->info.person_detect_info->w = INPUT_DEFAULT_WIDE;
        multi_context->info.person_detect_info->h = INPUT_DEFAULT_HIGH;
        multi_context->info.person_detect_info->num = DETECTION_MODEL_SINGLE;
        multi_context->info.person_detect_info->anchors = pd_anchors_four;
        ret = detect_init(multi_context->info.person_detect_info, tmp_module->alg_type,
            tmp_module, multi_context->info.person_detect_info->anchors.layer_num);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "person detect init failed\n");
        multi_context->use_count.person_flag = XMEDIA_TRUE;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_FACE) {
        multi_context->info.face_detect_info = (yolov5_detect_param*)calloc(1, sizeof(yolov5_detect_param));
        CHECK_CALLOC_RETURN_ERROR(multi_context->info.face_detect_info);
        set_default_yolov5_detect_param(multi_context->info.face_detect_info);
        multi_context->info.face_detect_info->w = INPUT_DEFAULT_WIDE;
        multi_context->info.face_detect_info->h = INPUT_DEFAULT_HIGH;
        multi_context->info.face_detect_info->num = DETECTION_MODEL_SINGLE;
        multi_context->info.face_detect_info->anchors = fd_anchors_four;
        ret = detect_init(multi_context->info.face_detect_info, tmp_module->alg_type,
            tmp_module, multi_context->info.face_detect_info->anchors.layer_num);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "face detect init failed\n");
        multi_context->use_count.face_flag = XMEDIA_TRUE;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PET) {
        multi_context->info.pet_detect_info = (yolov5_detect_param*)calloc(1, sizeof(yolov5_detect_param));
        CHECK_CALLOC_RETURN_ERROR(multi_context->info.pet_detect_info);
        set_default_yolov5_detect_param(multi_context->info.pet_detect_info);
        multi_context->info.pet_detect_info->w = INPUT_DEFAULT_WIDE;
        multi_context->info.pet_detect_info->h = INPUT_DEFAULT_HIGH;
        multi_context->info.pet_detect_info->num = DETECTION_MODEL_SINGLE;
        multi_context->info.pet_detect_info->anchors = ptd_anchors_four;
        ret = detect_init(multi_context->info.pet_detect_info, tmp_module->alg_type,
            tmp_module, multi_context->info.pet_detect_info->anchors.layer_num);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "pet detect init failed\n");
        multi_context->use_count.pet_flag = XMEDIA_TRUE;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PACKAGE) {
        multi_context->info.pkg_detect_info = (yolov5_detect_param*)calloc(1, sizeof(yolov5_detect_param));
        CHECK_CALLOC_RETURN_ERROR(multi_context->info.pkg_detect_info);
        set_default_yolov5_detect_param(multi_context->info.pkg_detect_info);
        multi_context->info.pkg_detect_info->w = INPUT_DEFAULT_WIDE;
        multi_context->info.pkg_detect_info->h = INPUT_DEFAULT_HIGH;
        multi_context->info.pkg_detect_info->num = DETECTION_MODEL_SINGLE;
        multi_context->info.pkg_detect_info->anchors = pkd_anchors_three;
        ret = detect_init(multi_context->info.pkg_detect_info, tmp_module->alg_type,
            tmp_module, multi_context->info.pkg_detect_info->anchors.layer_num);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "package detect init failed\n");
        multi_context->use_count.pkg_flag = XMEDIA_TRUE;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_SECOND_STAGE) {
        multi_context->rcnn_info = (svp_multi_rcnn_info*)calloc(1, sizeof(svp_multi_rcnn_info));
        CHECK_CALLOC_RETURN_ERROR(multi_context->rcnn_info);
        ret = npu_load_model(tmp_module, &multi_context->rcnn_info->model);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "second stage detect init failed\n");
        multi_context->rcnn_info->resize_phyaddr = xmedia_mmz_alloc(XMEDIA_NULL, "img_resize",
            RCNN_RECO_FRAME_SIZE * RCNN_RECO_FRAME_SIZE * 3 / 2);
        CHECK_SVP_COND_RETURN_ERROR(multi_context->rcnn_info->resize_phyaddr == 0, "resize mmz alloc fail !\n");
        multi_context->base_info->tracklet.high_score_thres = RCNN_TRACKER_THR;
        multi_context->use_count.rcnn_flag = XMEDIA_TRUE;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "error alg type[%d] !\n", tmp_module->alg_type);
    }

    return ret;
}

static inline xmedia_s32 multi_algtype_uninit(svp_multi_detect_context *multi_context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "multi_context NULL!\n");
    if (multi_context->rcnn_info != XMEDIA_NULL) {
        if (multi_context->use_count.rcnn_flag == XMEDIA_TRUE) {
            ret = npu_unload_model(&multi_context->rcnn_info->model);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "rcnn unload model failed ! \n");
            }
        }
        if (multi_context->rcnn_info->resize_phyaddr != XMEDIA_NULL) {
            xmedia_mmz_free(multi_context->rcnn_info->resize_phyaddr);
        }
        CFREE(multi_context->rcnn_info);
    }

    if (multi_context->info.person_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.person_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.person_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "person detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.person_detect_info);
    }

    if (multi_context->info.car_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.car_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.car_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "car detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.car_detect_info);
    }

    if (multi_context->info.nmv_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.nmv_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.nmv_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "nmv detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.nmv_detect_info);
    }

    if (multi_context->info.face_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.face_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.face_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "face detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.face_detect_info);
    }

    if (multi_context->info.pet_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.pet_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.pet_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "pet detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.nmv_detect_info);
    }

    if (multi_context->info.pkg_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.pkg_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.pkg_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "pkg detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.pkg_detect_info);
    }

    return ret;
}

xmedia_s32 multi_detect_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_multi_detect_context *multi_context = (svp_multi_detect_context *)context;
    if (multi_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "adas detect init context err !\n");
        return XMEDIA_FAILURE;
    }

    set_default_flag(multi_context);
    multi_context->base_info = (svp_multi_base_info*)calloc(1, sizeof(svp_multi_base_info));
    CHECK_CALLOC_GOTO_ERROR(multi_context->base_info, EXIT0);
    set_default_base_info(multi_context->base_info);
    multi_context->base_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    CHECK_CALLOC_GOTO_ERROR(multi_context->base_info->tracklet.tracker, EXIT1);

    for (xmedia_u8 i = 0; i < cfg.module_num; i++) {
        ret = multi_algtype_init(&cfg.modules[i], multi_context);
        if (ret != XMEDIA_SUCCESS) {
            break;
        }
    }
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT2, "multi_algtype_init failed !\n");
    return ret;

EXIT2:
    multi_algtype_uninit(multi_context);
    CFREE(multi_context->base_info->tracklet.tracker);
EXIT1:
    CFREE(multi_context->base_info);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 multi_detect_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_multi_detect_context* multi_context = (svp_multi_detect_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "adas_uninit context err !\n");

    if (multi_context->rcnn_info != XMEDIA_NULL) {
        if (multi_context->rcnn_info->resize_phyaddr != XMEDIA_NULL) {
            xmedia_mmz_free(multi_context->rcnn_info->resize_phyaddr);
        }
        if (multi_context->use_count.rcnn_flag == XMEDIA_TRUE) {
            ret = npu_unload_model(&multi_context->rcnn_info->model);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "rcnn unload model failed ! \n");
            }
        }
        CFREE(multi_context->rcnn_info);
    }

    if (multi_context->info.person_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.person_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.person_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "person detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.person_detect_info);
    }

    if (multi_context->info.car_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.car_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.car_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "car detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.car_detect_info);
    }

    if (multi_context->info.nmv_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.nmv_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.nmv_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "nmv detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.nmv_detect_info);
    }

    if (multi_context->info.face_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.face_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.face_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "face detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.face_detect_info);
    }

    if (multi_context->info.pet_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.pet_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.pet_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "pet detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.nmv_detect_info);
    }

    if (multi_context->info.pkg_detect_info != XMEDIA_NULL) {
        if (multi_context->use_count.pkg_flag == XMEDIA_TRUE) {
            ret = detect_deinit(multi_context->info.pkg_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "pkg detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(multi_context->info.pkg_detect_info);
    }

    if (multi_context->base_info != XMEDIA_NULL) {
        CFREE(multi_context->base_info->tracklet.tracker);
        CFREE(multi_context->base_info);
    }

    return ret;
}

xmedia_s32 multi_detect_set_attr(xmedia_void *context, const xmedia_svp_multi_detect_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 max_detect_num = 0;
    svp_multi_detect_context* multi_context = (svp_multi_detect_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "adas_set_attr context err !\n");

    if (multi_context->use_count.person_flag == XMEDIA_TRUE) {
        ret = base_set_attr(multi_context->info.person_detect_info, &task_attr->person_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "person detect_set_attr failed %#x!\n", ret);
        max_detect_num += multi_context->info.person_detect_info->max_target_num;
        SVP_CHECK_ATTR_NUM(max_detect_num, "total max_target_num[%d] out of range(0-50) !! \n", max_detect_num);
    }
    if (multi_context->use_count.car_flag == XMEDIA_TRUE) {
        ret = base_set_attr(multi_context->info.car_detect_info, &task_attr->car_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "car detect_set_attr failed %#x!\n", ret);
        max_detect_num += multi_context->info.car_detect_info->max_target_num;
        SVP_CHECK_ATTR_NUM(max_detect_num, "total max_target_num[%d] out of range(0-50) !! \n", max_detect_num);
    }
    if (multi_context->use_count.nmv_flag == XMEDIA_TRUE) {
        ret = base_set_attr(multi_context->info.nmv_detect_info, &task_attr->nmv_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "nmv detect_set_attr failed %#x!\n", ret);
        max_detect_num += multi_context->info.nmv_detect_info->max_target_num;
        SVP_CHECK_ATTR_NUM(max_detect_num, "total max_target_num[%d] out of range(0-50) !! \n", max_detect_num);
    }
    if (multi_context->use_count.face_flag == XMEDIA_TRUE) {
        ret = base_set_attr(multi_context->info.face_detect_info, &task_attr->face_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "face detect_set_attr failed %#x!\n", ret);
        max_detect_num += multi_context->info.face_detect_info->max_target_num;
        SVP_CHECK_ATTR_NUM(max_detect_num, "total max_target_num[%d] out of range(0-50) !! \n", max_detect_num);
    }
    if (multi_context->use_count.pet_flag == XMEDIA_TRUE) {
        ret = base_set_attr(multi_context->info.pet_detect_info, &task_attr->pet_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "pet detect_set_attr failed %#x!\n", ret);
        max_detect_num += multi_context->info.pet_detect_info->max_target_num;
        SVP_CHECK_ATTR_NUM(max_detect_num, "total max_target_num[%d] out of range(0-50) !! \n", max_detect_num);
    }
    if (multi_context->use_count.pkg_flag == XMEDIA_TRUE) {
        ret = base_set_attr(multi_context->info.pkg_detect_info, &task_attr->pkg_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "pkg detect_set_attr failed %#x!\n", ret);
        max_detect_num += multi_context->info.pkg_detect_info->max_target_num;
        SVP_CHECK_ATTR_NUM(max_detect_num, "total max_target_num[%d] out of range(0-50) !! \n", max_detect_num);
    }

    multi_context->base_info->tracklet.enable = task_attr->bytetrack_enable;
    multi_context->base_info->movement.enable = task_attr->motionless_filter_enable;
    multi_context->base_info->movement.movement_fps_thres = task_attr->movement_fps_thres;
    multi_context->base_info->movement.stillness_thres = task_attr->stillness_thres;
    return ret;
}

xmedia_s32 multi_detect_get_attr(xmedia_void *context, xmedia_svp_multi_detect_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_multi_detect_context* multi_context = (svp_multi_detect_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "adas_get_attr context err !\n");

    if (multi_context->use_count.person_flag == XMEDIA_TRUE) {
        ret = base_get_attr(multi_context->info.person_detect_info, &task_attr->person_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "person detect_set_attr failed %#x!\n", ret);
    }
    if (multi_context->use_count.car_flag == XMEDIA_TRUE) {
        ret = base_get_attr(multi_context->info.car_detect_info, &task_attr->car_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "car detect_set_attr failed %#x!\n", ret);
    }
    if (multi_context->use_count.nmv_flag == XMEDIA_TRUE) {
        ret = base_get_attr(multi_context->info.nmv_detect_info, &task_attr->nmv_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "nmv detect_set_attr failed %#x!\n", ret);
    }
    if (multi_context->use_count.face_flag == XMEDIA_TRUE) {
        ret = base_get_attr(multi_context->info.face_detect_info, &task_attr->face_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "face detect_set_attr failed %#x!\n", ret);
    }
    if (multi_context->use_count.pet_flag == XMEDIA_TRUE) {
        ret = base_get_attr(multi_context->info.pet_detect_info, &task_attr->pet_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "pet detect_set_attr failed %#x!\n", ret);
    }
    if (multi_context->use_count.pkg_flag == XMEDIA_TRUE) {
        ret = base_get_attr(multi_context->info.pkg_detect_info, &task_attr->pkg_attr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "pkg detect_set_attr failed %#x!\n", ret);
    }
    return ret;
}

xmedia_s32 multi_detect_process(xmedia_void *context, const xmedia_svp_task_input *input,
                                xmedia_svp_yolov5_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 i;
    xmedia_u64 it_phyaddr = 0;
    xmedia_void* it_viraddr = XMEDIA_NULL;
    svp_multi_detect_context* multi_context = (svp_multi_detect_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(multi_context == XMEDIA_NULL, "multi_process context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(input == XMEDIA_NULL, "input err !\n");
    CHECK_SVP_COND_RETURN_ERROR(output == XMEDIA_NULL, "output err !\n");
    xmedia_svp_yolov5_output detect_result = { 0 };
    output->target_num = 0;

    if (multi_context->use_count.person_flag == XMEDIA_TRUE) {
        ret = detect_process(multi_context->info.person_detect_info, input->frame,
            &detect_result, multi_context->info.person_detect_info->anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "person detect process err !\n");
        detect_get_class_id(&detect_result, multi_context->info.person_detect_info->type);
        ret = combine_outputs(output, &detect_result);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "combine_outputs err !\n");
    }

    if (multi_context->use_count.car_flag == XMEDIA_TRUE) {
        ret = detect_process(multi_context->info.car_detect_info, input->frame,
            &detect_result, multi_context->info.car_detect_info->anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "car detect process err !\n");
        detect_get_class_id(&detect_result, multi_context->info.car_detect_info->type);
        ret = combine_outputs(output, &detect_result);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "combine_outputs err !\n");
    }

    if (multi_context->use_count.nmv_flag == XMEDIA_TRUE) {
        ret = detect_process(multi_context->info.nmv_detect_info, input->frame,
            &detect_result, multi_context->info.nmv_detect_info->anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "nmv detect process err !\n");
        detect_get_class_id(&detect_result, multi_context->info.nmv_detect_info->type);
        ret = combine_outputs(output, &detect_result);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "combine_outputs err !\n");
    }

    if (multi_context->use_count.face_flag == XMEDIA_TRUE) {
        ret = detect_process(multi_context->info.face_detect_info, input->frame,
            &detect_result, multi_context->info.face_detect_info->anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "face detect process err !\n");
        detect_get_class_id(&detect_result, multi_context->info.face_detect_info->type);
        ret = combine_outputs(output, &detect_result);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "combine_outputs err !\n");
    }

    if (multi_context->use_count.pet_flag == XMEDIA_TRUE) {
        ret = detect_process(multi_context->info.pet_detect_info, input->frame,
            &detect_result, multi_context->info.pet_detect_info->anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "pet detect process err !\n");
        detect_get_class_id(&detect_result, multi_context->info.pet_detect_info->type);
        ret = combine_outputs(output, &detect_result);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "combine_outputs err !\n");
    }

    if (multi_context->use_count.pkg_flag == XMEDIA_TRUE) {
        ret = detect_process(multi_context->info.pkg_detect_info, input->frame,
            &detect_result, multi_context->info.pkg_detect_info->anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "pkg detect process err !\n");
        detect_get_class_id(&detect_result, multi_context->info.pkg_detect_info->type);
        ret = combine_outputs(output, &detect_result);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "combine_outputs err !\n");
    }

    if (multi_context->use_count.rcnn_flag == XMEDIA_TRUE) {
        xmedia_u32 x1, y1, x2, y2, w, h, padding_size;
        xmedia_svp_yolov5_output rcnn_result = { 0 };
        xmedia_svp_yolov5_output tmp_result = { 0 };
        svp_ive_img crop_src_img;
        crop_src_img.width = input->frame->frame.width;
        crop_src_img.hight = input->frame->frame.height;
        crop_src_img.img_addr = input->frame->frame.addr.y_phy_addr;

        for (i = 0; i < output->target_num; i++) {
            // 过滤非RCNN结果
            if (output->targets[i].alg_type != XMEDIA_SVP_ALG_TYPE_PERSON &&
                output->targets[i].alg_type != XMEDIA_SVP_ALG_TYPE_PET &&
                output->targets[i].alg_type != XMEDIA_SVP_ALG_TYPE_CAR) {
                tmp_result.targets[tmp_result.target_num] = output->targets[i];
                tmp_result.target_num++;
                continue;
            }

            svp_ive_img crop_dst_img, padding_dst_img;
            x1 = (xmedia_s32)roundf(output->targets[i].rect.x1 / 2) * 2;
            y1 = (xmedia_s32)roundf(output->targets[i].rect.y1 / 2) * 2;
            x2 = (xmedia_s32)roundf(output->targets[i].rect.x2 / 2) * 2;
            y2 = (xmedia_s32)roundf(output->targets[i].rect.y2 / 2) * 2;
            if ((abs(x2 - x1) < VGS_LIMIT_BILINEAR_INIMG_W) || (abs(y2 - y1) < VGS_LIMIT_BILINEAR_INIMG_H)) {
                continue;
            }

            w = abs(x2 - x1);
            h = abs(y2 - y1);

            if (((x1 + w) > INPUT_DEFAULT_WIDE) || ((y1 + h) > INPUT_DEFAULT_HIGH)) {
                continue;
            }

            padding_size = STD_MAX(w, h);

            // 申请复合内存，多处使用
            ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "svp_it_buf", &it_phyaddr,
                (xmedia_void**)&it_viraddr,
                (SVP_ALIGN_UP(w, SVP_ALIGN_BYTE) * h * 3 / 2) +
                (SVP_ALIGN_UP(padding_size, SVP_ALIGN_BYTE) * padding_size * 3 / 2));
            CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "mmz_alloc and map error !!\n");
            crop_dst_img.width = w;
            crop_dst_img.hight = h;
            crop_dst_img.img_addr = it_phyaddr;
            crop_dst_img.vir_addr = it_viraddr;
            ret = svp_ive_crop(&crop_src_img, &crop_dst_img, x1, y1);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "ive_crop error !!\n");

            padding_dst_img.width = padding_size;
            padding_dst_img.hight = padding_size;
            padding_dst_img.img_addr = it_phyaddr + (SVP_ALIGN_UP(w, SVP_ALIGN_BYTE) * h * 3 / 2);
            padding_dst_img.vir_addr = it_viraddr + (SVP_ALIGN_UP(w, SVP_ALIGN_BYTE) * h * 3 / 2);

            // Y:0xff 背景全部涂为白色
            memset((xmedia_void*)padding_dst_img.vir_addr, 0xff,
                SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE) * padding_dst_img.hight);
            // UV:0x80 固定透明色
            memset(((xmedia_void*)padding_dst_img.vir_addr +
                SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE) * padding_dst_img.hight), 0x80,
                SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE) * padding_dst_img.hight / 2);

            x1 = (padding_size - w) / 2;
            y1 = (padding_size - h) / 2;

            xmedia_mmz_flush_cache(it_phyaddr, it_viraddr, (SVP_ALIGN_UP(w, SVP_ALIGN_BYTE) * h * 3 / 2) +
                (SVP_ALIGN_UP(padding_size, SVP_ALIGN_BYTE) * padding_size * 3 / 2));

            ret = svp_ive_padding(&crop_dst_img, &padding_dst_img, x1, y1);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_ive_padding error !!\n");

            xmedia_video_frame_info input_resize_frame = { 0 };
            xmedia_video_frame_info output_resize_frame = { 0 };
            input_resize_frame.frame.width = padding_dst_img.width;
            input_resize_frame.frame.height = padding_dst_img.hight;
            input_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
            input_resize_frame.frame.stride.y_stride = SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE);
            input_resize_frame.frame.stride.c_stride = SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE);
            input_resize_frame.frame.addr.y_phy_addr = padding_dst_img.img_addr;
            input_resize_frame.frame.addr.c_phy_addr = padding_dst_img.img_addr +
                input_resize_frame.frame.stride.c_stride * padding_dst_img.hight;
            output_resize_frame.frame.width = RCNN_RECO_FRAME_SIZE;
            output_resize_frame.frame.height = RCNN_RECO_FRAME_SIZE;
            output_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
            output_resize_frame.frame.stride.y_stride = RCNN_RECO_FRAME_SIZE;
            output_resize_frame.frame.stride.c_stride = RCNN_RECO_FRAME_SIZE;
            output_resize_frame.frame.addr.y_phy_addr = multi_context->rcnn_info->resize_phyaddr;
            output_resize_frame.frame.addr.c_phy_addr = multi_context->rcnn_info->resize_phyaddr +
                RCNN_RECO_FRAME_SIZE * RCNN_RECO_FRAME_SIZE;
            ret = svp_vgs_resize(&input_resize_frame, &output_resize_frame);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize error !!\n");

            if (it_viraddr != XMEDIA_NULL || it_phyaddr != 0) {
                svp_mmz_unmap_and_free(it_phyaddr, it_viraddr);
            }
            ret = npu_run_model(&multi_context->rcnn_info->model, multi_context->rcnn_info->resize_phyaddr);
            CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "npu_run_model error !!\n");

            xmedia_u8* addr;
            xmedia_float arr[RCNN_CLASSIFY_COUNT] = { 0 };
            xmedia_float result_arr[RCNN_CLASSIFY_COUNT] = { 0 };
            addr = (xmedia_u8*)multi_context->rcnn_info->model.output.tensor[0].addr;
            for (xmedia_u8 m = 0; m < RCNN_CLASSIFY_COUNT; m++) {
                arr[m] = dequantize(addr[m], multi_context->rcnn_info->model.output.tensor[0].quant.scale,
                    multi_context->rcnn_info->model.output.tensor[0].quant.zp);
            }
            softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);

            if (output->targets[i].alg_type == XMEDIA_SVP_ALG_TYPE_PERSON) {
                if (result_arr[SVP_CLASS_RCNN_PERSON] >= multi_context->info.person_detect_info->classifier_threshold) {
                    rcnn_result.targets[rcnn_result.target_num] = output->targets[i];
                    rcnn_result.targets[rcnn_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_PERSON];
                    rcnn_result.target_num++;
                } else if (result_arr[SVP_CLASS_RCNN_BIKER] >=
                    multi_context->info.person_detect_info->classifier_threshold) {
                    rcnn_result.targets[rcnn_result.target_num] = output->targets[i];
                    rcnn_result.targets[rcnn_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_BIKER];
                    rcnn_result.target_num++;
                } else if (result_arr[SVP_CLASS_RCNN_MOTORER] >=
                    multi_context->info.person_detect_info->classifier_threshold) {
                    rcnn_result.targets[rcnn_result.target_num] = output->targets[i];
                    rcnn_result.targets[rcnn_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_MOTORER];
                    rcnn_result.target_num++;
                } else if (result_arr[SVP_CLASS_RCNN_TRICYCLER] >=
                    multi_context->info.person_detect_info->classifier_threshold) {
                    rcnn_result.targets[rcnn_result.target_num] = output->targets[i];
                    rcnn_result.targets[rcnn_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_TRICYCLER];
                    rcnn_result.target_num++;
                }
            } else if (output->targets[i].alg_type == XMEDIA_SVP_ALG_TYPE_PET) {
                if (result_arr[SVP_CLASS_RCNN_PET] >= multi_context->info.pet_detect_info->classifier_threshold) {
                    rcnn_result.targets[rcnn_result.target_num] = output->targets[i];
                    rcnn_result.targets[rcnn_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_PET];
                    rcnn_result.target_num++;
                }
            } else if (output->targets[i].alg_type == XMEDIA_SVP_ALG_TYPE_CAR) {
                if (result_arr[SVP_CLASS_RCNN_CAR] >= multi_context->info.car_detect_info->classifier_threshold) {
                    rcnn_result.targets[rcnn_result.target_num] = output->targets[i];
                    rcnn_result.targets[rcnn_result.target_num].classfier_score = result_arr[SVP_CLASS_RCNN_CAR];
                    rcnn_result.target_num++;
                }
            } else {
                SVP_TRACE(MODULE_DBG_ERR, "error detect type[%d] !\n", output->targets[i].alg_type);
            }
        }

        // 第二次iou过滤
        xmedia_u32 endnum = 0;
        det_nms_cross(rcnn_result.targets, rcnn_result.target_num, RCNN_IOU_THR, &endnum);
        output->target_num = 0;
        for (xmedia_u8 j = 0; j < endnum; j++) {
            output->targets[output->target_num] = rcnn_result.targets[j];
            output->target_num++;
            if (output->target_num > XMEDIA_SVP_MAX_TARGET_NUM) {
                break;
            }
        }
        // 非RCNN结果总合
        for (xmedia_u8 k = 0; k < tmp_result.target_num; k++) {
            output->targets[output->target_num] = tmp_result.targets[k];
            output->target_num++;
            if (output->target_num > XMEDIA_SVP_MAX_TARGET_NUM) {
                break;
            }
        }
    }

    if (multi_context->base_info->tracklet.enable == XMEDIA_TRUE) {
        xmedia_svp_tracker_result result = { 0 };
        result.target_num = output->target_num;
        for (i = 0; i < result.target_num; i++) {
            result.tracker_single[i].alg_type = output->targets[i].alg_type;
            result.tracker_single[i].class_type = output->targets[i].class_type;
            result.tracker_single[i].detect_score = output->targets[i].detect_score;
            result.tracker_single[i].classfier_score = output->targets[i].classfier_score;
            result.tracker_single[i].tracker_id = output->targets[i].tracker_id;
            result.tracker_single[i].tracker_age = output->targets[i].tracker_age;
            result.tracker_single[i].rect = output->targets[i].rect;
        }
        ret = svp_detect_bytetracker(&result,
                                     &multi_context->base_info->tracklet,
                                     &multi_context->base_info->tarck_id_grow,
                                     multi_context->base_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        memset(output, 0, sizeof(xmedia_svp_yolov5_output));
        for (i = 0; i < result.target_num; i++) {
            output->targets[output->target_num].alg_type = result.tracker_single[i].alg_type;
            output->targets[output->target_num].class_type =
                result.tracker_single[i].class_type;
            output->targets[output->target_num].detect_score =
                result.tracker_single[i].detect_score;
            output->targets[output->target_num].classfier_score =
                result.tracker_single[i].classfier_score;
            output->targets[output->target_num].tracker_id =
                result.tracker_single[i].tracker_id;
            output->targets[output->target_num].tracker_age =
                result.tracker_single[i].tracker_age;
            output->targets[output->target_num].rect = result.tracker_single[i].rect;
            output->target_num++;
        }
    }

    if ((multi_context->base_info->movement.enable == XMEDIA_TRUE) && (output->target_num != 0)) {
        svp_movement_result result = { 0 };
        result.target_num = output->target_num;
        for (i = 0; i < result.target_num; i++) {
            result.targets[i].class_type = output->targets[i].class_type;
            result.targets[i].tracker_id = output->targets[i].tracker_id;
            result.targets[i].rect = output->targets[i].rect;
        }
        ret = svp_detect_movement(&multi_context->base_info->movement, &result);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "movement filter failed, ret=0x%x\n", ret);
        }
        for (i = 0; i < result.target_num; i++) {
            output->targets[i].motion_state = result.targets[i].motion_state;
            output->targets[i].rect = result.targets[i].rect;
        }
    }

    return ret;

EXIT:
    if (it_viraddr != XMEDIA_NULL || it_phyaddr != 0) {
        svp_mmz_unmap_and_free(it_phyaddr, it_viraddr);
    }
    return XMEDIA_FAILURE;
}
