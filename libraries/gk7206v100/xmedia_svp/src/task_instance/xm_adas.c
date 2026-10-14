#include "xm_adas.h"

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

static xmedia_svp_detect_anchors nmv_anchors_three = {
    .layer_num = LAYER_MIN_NUM,
    .anchors = {{{16.0f, 13.0f}, {30.0f, 24.0f}, {48.0f, 37.0f}},
                {{75.0f, 59.0f}, {117.0f, 99.0f}, {181.0f, 161.0f}},
                {{298.0f, 243.0f}, {197.0f, 384.0f}, {327.0f, 579.0f}}},
};

static xmedia_svp_detect_anchors plate_anchors = {
    .anchors = {{{4,5}, {8,10}, {13,16}},
               {{23,29}, {43,55}, {73,105}},
               {{146,217}, {231,300}, {335,433}}},
    .layer_num = LAYER_MIN_NUM,
};


// 数据过滤函数
static xmedia_s32 data_filter(xmedia_float *x, xmedia_float *y, xmedia_s32 n, xmedia_float *coeffs,
    xmedia_float **rediduals, xmedia_float *mean_residuals)
{
    *rediduals = (xmedia_float *)malloc(n * sizeof(xmedia_float));
    CHECK_CALLOC_RETURN_ERROR(*rediduals);
    xmedia_float *fitted_y = (xmedia_float *)malloc(n * sizeof(xmedia_float));
    CHECK_CALLOC_GOTO_ERROR(fitted_y, EXIT);

    *mean_residuals = 0;
    for (xmedia_s32 i = 0; i < n; i++) {
        fitted_y[i] = coeffs[2] * x[i] *x[i] + coeffs[1] * x[i] + coeffs[0];
        (*rediduals)[i] = fabsf(y[i] - fitted_y[i]);
        *mean_residuals += (*rediduals)[i];
    }
    *mean_residuals = *mean_residuals / n;
    CFREE(fitted_y);
    // CFREE(rediduals);
    return XMEDIA_SUCCESS;

EXIT:
    CFREE(*rediduals);
    return XMEDIA_FAILURE;
}

static xmedia_s32 polyfit(xmedia_float *x, xmedia_float *y, xmedia_s32 n, xmedia_float *coeffs)
{
    xmedia_float sum_x = 0, sum_x2 = 0, sum_x3 = 0, sum_x4 = 0;
    xmedia_float sum_y = 0, sum_xy = 0, sum_x2y = 0;

    for (xmedia_s32 i = 0; i < n; i++) {
        xmedia_float xi = x[i];
        sum_x += xi;
        sum_x2 += xi * xi;
        sum_x3 += xi * xi * xi;
        sum_x4 += xi * xi * xi * xi;
        sum_y += y[i];
        sum_xy += xi * y[i];
        sum_x2y += xi * xi * y[i];
    }

    xmedia_float A[SVP_FIT_ORDER][SVP_FIT_ORDER] = {
        {n, sum_x, sum_x2},
        {sum_x, sum_x2, sum_x3},
        {sum_x2, sum_x3, sum_x4}
    };
    xmedia_float B[SVP_FIT_ORDER] = {sum_y, sum_xy, sum_x2y};

    // 高斯消元法解方程组
    for (xmedia_s32 k = 0; k < SVP_FIT_ORDER; k++) {
        for (xmedia_s32 i = k + 1; i < SVP_FIT_ORDER; i++) {
            xmedia_float factor = A[i][k] / A[k][k];
            for (xmedia_s32 j = k; j < SVP_FIT_ORDER; j++)
                A[i][j] -= factor * A[k][j];
            B[i] -= factor * B[k];
        }
    }

    coeffs[2] = B[2] / A[2][2];                                               // x²系数
    coeffs[1] = (B[1] - A[1][2] * coeffs[2]) / A[1][1];                       // x系数
    coeffs[0] = (B[0] - A[0][1] * coeffs[1] - A[0][2] * coeffs[2]) / A[0][0]; // 常数项

    return XMEDIA_SUCCESS;
}

static xmedia_bool in_lane(xmedia_float x, xmedia_float y, xmedia_float *left_coeffs, xmedia_float *right_coeffs)
{

    xmedia_float x_left, x_right;
    x_left = left_coeffs[2] * y * y + left_coeffs[1] * y + left_coeffs[0];
    x_right = right_coeffs[2] * y * y + right_coeffs[1] * y + right_coeffs[0];

    if (x_left >= x_right || x_left == 0 || x_right == 0) {
        return XMEDIA_FALSE;
    }

    return x >= x_left && x <= x_right;
}

static inline xmedia_void get_detect_class_id(xmedia_svp_yolov5_output *detect_result, xmedia_svp_alg_type type)
{
    for (xmedia_s32 i = 0; i < detect_result->target_num; i++) {
        switch (type) {
            case XMEDIA_SVP_ALG_TYPE_PERSON: {
                if (detect_result->targets[i].class_type == 0) {
                    detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PERSON;
                } else {
                    detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
                }
                break;
            }
            case XMEDIA_SVP_ALG_TYPE_CAR: {
                if (detect_result->targets[i].class_type == 0) {
                    detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_CAR;
                } else {
                    detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
                }
                break;
            }
            case XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE: {
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
                break;
            }
            case XMEDIA_SVP_ALG_TYPE_LICENSE_DETECT: {
                if (detect_result->targets[i].class_type == 0) {
                    detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_LICENSE;
                } else {
                    detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
                }
                break;
            }
            default:
                SVP_TRACE(MODULE_DBG_ERR, "detect does not support this type[%d]\n", type);
        }

    }
}

static inline  xmedia_s32 base_check_attr(const xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range[0-50] !! \n",
        task_attr->max_target_num);
    SVP_CHECK_ATTR_ENABLE(task_attr->bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");

    return ret;
}

static inline xmedia_s32 base_set_attr(yolov5_detect_param *param, const xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret = base_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }
    param->detect_threshold = task_attr->detect_threshold;
    param->iou_threshold = task_attr->iou_threshold;
    param->max_target_num = task_attr->max_target_num;
    param->classifier_threshold = task_attr->classifier_threshold;
    param->tracklet.enable = task_attr->bytetrack_enable;
    return ret;
}

static inline xmedia_s32 base_get_attr(yolov5_detect_param *param, xmedia_svp_yolov5_attr *task_attr)
{
    task_attr->detect_threshold = param->detect_threshold;
    task_attr->iou_threshold = param->iou_threshold;
    task_attr->max_target_num = param->max_target_num;
    task_attr->classifier_threshold = param->classifier_threshold;
    task_attr->bytetrack_enable = param->tracklet.enable;
    return XMEDIA_SUCCESS;
}


xmedia_s32 adas_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_svp_modules *tmp_module = XMEDIA_NULL;
    svp_adas_context *adas_context = (svp_adas_context *)context;
    if (adas_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "adas detect init context err !\n");
        return XMEDIA_FAILURE;
    }
    if (cfg.module_num != 5) {
        SVP_TRACE(MODULE_DBG_ERR, "detect model num err!\n");
        return XMEDIA_FAILURE;
    }

    // 车牌检测模型
    adas_context->info.plate_detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_GOTO_ERROR(adas_context->info.plate_detect_info, EXIT0);
    set_default_yolov5_detect_param(adas_context->info.plate_detect_info);
    adas_context->info.plate_detect_info->w = SVP_PLATE_IMG_WIGTH;
    adas_context->info.plate_detect_info->h = SVP_PLATE_IMG_HIGHT;
    adas_context->info.plate_detect_info->num = DETECTION_MODEL_SINGLE;
    adas_context->info.plate_detect_info->anchors = plate_anchors;
    tmp_module = cfg.modules;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT1, "plate detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT1, "plate detect model pathname err!\n");
    ret = detect_init(adas_context->info.plate_detect_info, tmp_module->alg_type,
        tmp_module, adas_context->info.plate_detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT1, "plate detect init model failed !\n");

    // 非机动车检测模型
    adas_context->info.nmv_detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_GOTO_ERROR(adas_context->info.nmv_detect_info, EXIT2);
    set_default_yolov5_detect_param(adas_context->info.nmv_detect_info);
    adas_context->info.nmv_detect_info->w = INPUT_DEFAULT_WIDE;
    adas_context->info.nmv_detect_info->h = INPUT_DEFAULT_HIGH;
    adas_context->info.nmv_detect_info->num = DETECTION_MODEL_SIX;
    adas_context->info.nmv_detect_info->anchors = nmv_anchors_three;
    tmp_module = cfg.modules + 1;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT3, "nmv detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT3, "nmv model pathname err!\n");
    ret = detect_init(adas_context->info.nmv_detect_info, tmp_module->alg_type,
        tmp_module, adas_context->info.nmv_detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT3, "nmv detect init model failed !\n");

    // 车形检测模型
    adas_context->info.car_detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_GOTO_ERROR(adas_context->info.car_detect_info, EXIT4);
    set_default_yolov5_detect_param(adas_context->info.car_detect_info);
    adas_context->info.car_detect_info->w = INPUT_DEFAULT_WIDE;
    adas_context->info.car_detect_info->h = INPUT_DEFAULT_HIGH;
    adas_context->info.car_detect_info->num = DETECTION_MODEL_SINGLE;
    adas_context->info.car_detect_info->anchors = cd_anchors;
    tmp_module = cfg.modules + 2;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT5, "car detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT5, "car detect model pathname err!\n");
    ret = detect_init(adas_context->info.car_detect_info, tmp_module->alg_type,
        tmp_module, adas_context->info.car_detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT5, "car detect init model failed !\n");

    // 车道线模型
    adas_context->info.lane_info = (svp_lane_info *)calloc(1, sizeof(svp_lane_info));
    CHECK_CALLOC_GOTO_ERROR(adas_context->info.lane_info, EXIT6);
    tmp_module = cfg.modules + 3;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT7, "lane_model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT7, "lane detect model pathname err!\n");
    ret = npu_load_model(tmp_module, &adas_context->info.lane_info->npu_model);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT7, "lane_model load err!\n");
    *(adas_context->info.lane_info->npu_model.user_count) += 1;
    adas_context->info.dist_attr.focal_length_px = 500; // 像素焦距，正确值需要根据镜头实际参数进行计算，此为参数默认值
    adas_context->info.dist_attr.car_limit_angle = 10;  // 车辆限制角度，此为参数默认值
    adas_context->info.dist_attr.camera_height = 120;   // 相机高度，实际值会根据车牌距离实时计算，此为参数默认值
    adas_context->info.dist_attr.plate_width = 14;      // 车牌宽度，参数默认值

    // 人形检测模型
    adas_context->info.person_detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_GOTO_ERROR(adas_context->info.person_detect_info, EXIT8);
    set_default_yolov5_detect_param(adas_context->info.person_detect_info);
    adas_context->info.person_detect_info->w = INPUT_DEFAULT_WIDE;
    adas_context->info.person_detect_info->h = INPUT_DEFAULT_HIGH;
    adas_context->info.person_detect_info->num = DETECTION_MODEL_SINGLE;
    adas_context->info.person_detect_info->anchors = pd_anchors_four;
    tmp_module = cfg.modules + 4;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT9, "person detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), EXIT9, "person detect model pathname err!\n");
    ret = detect_init(adas_context->info.person_detect_info, tmp_module->alg_type,
        tmp_module, adas_context->info.person_detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT9, "person detect init model failed !\n");
    return ret;

EXIT9:
    CFREE(adas_context->info.person_detect_info);
EXIT8:
    if (adas_context->info.lane_info->npu_model.user_count != XMEDIA_NULL) {
        *(adas_context->info.lane_info->npu_model.user_count) =
            *(adas_context->info.lane_info->npu_model.user_count) - 1;
    }
    if (adas_context->info.lane_info->npu_model.user_count != XMEDIA_NULL &&
        *(adas_context->info.lane_info->npu_model.user_count) == 0) {
        ret = npu_unload_model(&adas_context->info.lane_info->npu_model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "adas unload lane modle failed ! \n");
        }
    }
EXIT7:
    CFREE(adas_context->info.lane_info);
EXIT6:
    ret = detect_deinit(adas_context->info.car_detect_info);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "car detect_deinit failed %#x!\n", ret);
    }
EXIT5:
    CFREE(adas_context->info.car_detect_info);
EXIT4:
    ret = detect_deinit(adas_context->info.nmv_detect_info);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "nmv detect_deinit failed %#x!\n", ret);
    }
EXIT3:
    CFREE(adas_context->info.nmv_detect_info);
EXIT2:
    ret = detect_deinit(adas_context->info.plate_detect_info);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "plate detect_deinit failed %#x!\n", ret);
    }
EXIT1:
    CFREE(adas_context->info.plate_detect_info);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 adas_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_adas_context *adas_context = (svp_adas_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(adas_context == XMEDIA_NULL, "adas_uninit context err !\n");

    if (adas_context->info.lane_info != XMEDIA_NULL) {
        if (adas_context->info.lane_info->npu_model.user_count != XMEDIA_NULL) {
            *(adas_context->info.lane_info->npu_model.user_count) =
                *(adas_context->info.lane_info->npu_model.user_count) - 1;
        }
        if (adas_context->info.lane_info->npu_model.user_count != XMEDIA_NULL &&
            *(adas_context->info.lane_info->npu_model.user_count) == 0) {
            ret = npu_unload_model(&adas_context->info.lane_info->npu_model);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "adas unload lane modle failed ! \n");
            }
        }
        CFREE(adas_context->info.lane_info);
    }

    if (adas_context->info.person_detect_info != XMEDIA_NULL) {
        if (adas_context->info.person_detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(adas_context->info.person_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "person detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(adas_context->info.person_detect_info);
    }

    if (adas_context->info.plate_detect_info != XMEDIA_NULL) {
        if (adas_context->info.plate_detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(adas_context->info.plate_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "plate detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(adas_context->info.plate_detect_info);
    }

    if (adas_context->info.car_detect_info != XMEDIA_NULL) {
        if (adas_context->info.car_detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(adas_context->info.car_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "car detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(adas_context->info.car_detect_info);
    }

    if (adas_context->info.nmv_detect_info != XMEDIA_NULL) {
        if (adas_context->info.nmv_detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(adas_context->info.nmv_detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "nmv detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(adas_context->info.nmv_detect_info);
    }

    return ret;
}

xmedia_s32 adas_set_attr(xmedia_void *context, const xmedia_svp_adas_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_adas_context* adas_context = (svp_adas_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(adas_context == XMEDIA_NULL, "adas_set_attr context err !\n");

    ret = base_set_attr(adas_context->info.car_detect_info, &task_attr->car_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "car detect_set_attr failed %#x!\n", ret);
    ret = base_set_attr(adas_context->info.plate_detect_info, &task_attr->plate_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "plate detect_set_attr failed %#x!\n", ret);
    ret = base_set_attr(adas_context->info.person_detect_info, &task_attr->person_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "person detect_set_attr failed %#x!\n", ret);
    ret = base_set_attr(adas_context->info.nmv_detect_info, &task_attr->nmv_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "nmv detect_set_attr failed %#x!\n", ret);
    adas_context->info.dist_attr = task_attr->distance_attr;

    return ret;
}

xmedia_s32 adas_get_attr(xmedia_void *context, xmedia_svp_adas_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_adas_context* adas_context = (svp_adas_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(adas_context == XMEDIA_NULL, "adas_get_attr context err !\n");

    ret = base_get_attr(adas_context->info.car_detect_info, &task_attr->car_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "car detect_set_attr failed %#x!\n", ret);
    ret = base_get_attr(adas_context->info.plate_detect_info, &task_attr->plate_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "plate detect_set_attr failed %#x!\n", ret);
    ret = base_get_attr(adas_context->info.person_detect_info, &task_attr->person_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "person detect_set_attr failed %#x!\n", ret);
    ret = base_get_attr(adas_context->info.nmv_detect_info, &task_attr->nmv_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "nmv detect_set_attr failed %#x!\n", ret);
    task_attr->distance_attr = adas_context->info.dist_attr;

    return ret;
}

static xmedia_s32 adas_lane_post_process(const xmedia_npu_model npu_model, xmedia_svp_adas_result *result)
{
    CHECK_SVP_COND_RETURN_ERROR(result == XMEDIA_NULL, "xmedia_svp_adas_result is NULL !\n");

    xmedia_s32 i, j, k;
    xmedia_u8 *addr;
    xmedia_float max_score, tmp_score;
    xmedia_s32 max_idx;
    xmedia_float rate = 0;
    xmedia_s32 max_idx_row[XMEDIA_SVP_LANE_MAX_ROW_POINT * XMEDIA_SVP_LANE_MAX_NUM] = {0};
    xmedia_s32 max_idx_col[XMEDIA_SVP_LANE_MAX_COL_POINT * XMEDIA_SVP_LANE_MAX_NUM] = {0};
    xmedia_s32 valid_row[XMEDIA_SVP_LANE_MAX_ROW_POINT * XMEDIA_SVP_LANE_MAX_NUM] = {0};
    xmedia_s32 valid_col[XMEDIA_SVP_LANE_MAX_COL_POINT * XMEDIA_SVP_LANE_MAX_NUM] = {0};
    xmedia_u8 local_width = 3;

    // (n, x, y ,z) n:batch_size, x:每个点不同位置的概率 y:每条车道线点数 z:车道线数量
    addr = (xmedia_u8 *)npu_model.output.tensor[0].addr; // (1,100,18,4)
    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM; i++) {
        for (j = 0; j < XMEDIA_SVP_LANE_MAX_ROW_POINT; j++) {
            max_score = dequantize(addr[j * XMEDIA_SVP_LANE_MAX_NUM + i], npu_model.output.tensor[0].quant.scale,
                npu_model.output.tensor[0].quant.zp);
            max_idx = 0;
            for (k = 1; k < SVP_LANE_ROW_SCORE_NUM; k++) {
                tmp_score = dequantize(
                    addr[k * XMEDIA_SVP_LANE_MAX_ROW_POINT * XMEDIA_SVP_LANE_MAX_NUM + j * XMEDIA_SVP_LANE_MAX_NUM + i],
                    npu_model.output.tensor[0].quant.scale,
                    npu_model.output.tensor[0].quant.zp);
                if (tmp_score > max_score) {
                    max_idx = k;
                    max_score = tmp_score;
                }
            }
            max_idx_row[i * XMEDIA_SVP_LANE_MAX_ROW_POINT + j] = max_idx;
        }
    }

    addr = (xmedia_u8 *)npu_model.output.tensor[1].addr; // (1,50,40,4)
    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM; i++) {
        for (j = 0; j < XMEDIA_SVP_LANE_MAX_COL_POINT; j++) {
            max_score = dequantize(addr[j * XMEDIA_SVP_LANE_MAX_NUM + i], npu_model.output.tensor[1].quant.scale,
                npu_model.output.tensor[1].quant.zp);
            max_idx = 0;
            for (k = 1; k < SVP_LANE_COL_SCORE_NUM; k++) {
                tmp_score = dequantize(
                    addr[k * XMEDIA_SVP_LANE_MAX_COL_POINT * XMEDIA_SVP_LANE_MAX_NUM + j * XMEDIA_SVP_LANE_MAX_NUM + i],
                    npu_model.output.tensor[1].quant.scale,
                    npu_model.output.tensor[1].quant.zp);
                if (tmp_score > max_score) {
                    max_idx = k;
                    max_score = tmp_score;
                }
            }
            max_idx_col[i * XMEDIA_SVP_LANE_MAX_COL_POINT + j] = max_idx;
        }
    }

    addr = (xmedia_u8 *)npu_model.output.tensor[2].addr; // (1,2,18,4)
    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM; i++) {
        for (j = 0; j < XMEDIA_SVP_LANE_MAX_ROW_POINT; j++) {
            xmedia_float invalid_score = dequantize(addr[j * XMEDIA_SVP_LANE_MAX_NUM + i],
                npu_model.output.tensor[2].quant.scale,
                npu_model.output.tensor[2].quant.zp);
            xmedia_float valid_score = dequantize(
                addr[XMEDIA_SVP_LANE_MAX_ROW_POINT * XMEDIA_SVP_LANE_MAX_NUM + j * XMEDIA_SVP_LANE_MAX_NUM + i],
                npu_model.output.tensor[2].quant.scale,
                npu_model.output.tensor[2].quant.zp);
            rate = exp(valid_score) / (exp(invalid_score) + exp(valid_score));
            valid_row[i * XMEDIA_SVP_LANE_MAX_ROW_POINT + j] = rate > 0.5 ? 1 :0;
        }
    }

    addr = (xmedia_u8 *)npu_model.output.tensor[3].addr; // (1,2,40,4)
    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM; i++) {
        for (j = 0; j < XMEDIA_SVP_LANE_MAX_COL_POINT; j++) {
            xmedia_float invalid_score = dequantize(addr[j * XMEDIA_SVP_LANE_MAX_NUM + i],
                npu_model.output.tensor[3].quant.scale,
                npu_model.output.tensor[3].quant.zp);
            xmedia_float valid_score = dequantize(
                addr[XMEDIA_SVP_LANE_MAX_COL_POINT * XMEDIA_SVP_LANE_MAX_NUM + j * XMEDIA_SVP_LANE_MAX_NUM + i],
                npu_model.output.tensor[3].quant.scale,
                npu_model.output.tensor[3].quant.zp);
            rate = exp(valid_score) / (exp(invalid_score) + exp(valid_score));
            valid_col[i * XMEDIA_SVP_LANE_MAX_COL_POINT + j] = rate > 0.5 ? 1 :0;
        }
    }

    // 车道线序号
    xmedia_u8 r_l[2] = {1, 2};
    xmedia_u8 c_l[2] = {0, 3};
    xmedia_float tmp_row[XMEDIA_SVP_LANE_MAX_ROW_POINT][2];
    xmedia_float tmp_col[XMEDIA_SVP_LANE_MAX_COL_POINT][2];
    xmedia_s32 best_idx, left_idx, right_idx;
    xmedia_s32 valid_point = 0;

    // k最大值邻近点，i第几条车道线
    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM / 2; i++) {
        valid_point = 0;
        addr = (xmedia_u8 *)npu_model.output.tensor[0].addr;
        for (j = 0; j < XMEDIA_SVP_LANE_MAX_ROW_POINT; j++) {
            if (valid_row[r_l[i] * XMEDIA_SVP_LANE_MAX_ROW_POINT + j] == 1) {
                best_idx = max_idx_row[r_l[i] * XMEDIA_SVP_LANE_MAX_ROW_POINT + j];
                left_idx = STD_MAX(best_idx - local_width, 0);
                right_idx = STD_MIN(best_idx + local_width, (SVP_LANE_ROW_SCORE_NUM - 1));
                xmedia_float arr[right_idx - left_idx + 1];
                xmedia_float result_arr[right_idx - left_idx + 1];
                xmedia_float tmp_out = 0;
                for (k = left_idx; k <= right_idx; k++) {
                    arr[k - left_idx] = dequantize(
                        addr[k * XMEDIA_SVP_LANE_MAX_ROW_POINT * XMEDIA_SVP_LANE_MAX_NUM +
                        j * XMEDIA_SVP_LANE_MAX_NUM + r_l[i]],
                        npu_model.output.tensor[0].quant.scale,
                        npu_model.output.tensor[0].quant.zp);
                }
                softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);
                for (k = left_idx; k <= right_idx; k++) {
                    tmp_out += k * result_arr[k - left_idx];
                }

                // 0.44为anchor值
                tmp_row[valid_point][0] = (tmp_out + 0.5) / (SVP_LANE_ROW_SCORE_NUM - 1) * INPUT_DEFAULT_WIDE;
                tmp_row[valid_point][1] = (0.44 + (1 - 0.44) / XMEDIA_SVP_LANE_MAX_ROW_POINT * j) * INPUT_DEFAULT_HIGH;
                valid_point++;
            }
        }
        if (valid_point > XMEDIA_SVP_LANE_MAX_ROW_POINT / 3) {
            result->lane_point_num[r_l[i]] = valid_point;
            for (k = 0; k < valid_point; k++) {
                result->row_result[i][k].x = tmp_row[k][0];
                result->row_result[i][k].y = tmp_row[k][1];
            }
        }
    }

    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM / 2; i++) {
        valid_point = 0;
        addr = (xmedia_u8 *)npu_model.output.tensor[1].addr;
        for (j = 0; j < XMEDIA_SVP_LANE_MAX_COL_POINT; j++) {
            if (valid_col[c_l[i] * XMEDIA_SVP_LANE_MAX_COL_POINT + j] == 1) {
                best_idx = max_idx_col[c_l[i] * XMEDIA_SVP_LANE_MAX_COL_POINT + j];
                left_idx = STD_MAX(best_idx - local_width, 0);
                right_idx = STD_MIN(best_idx + local_width, (SVP_LANE_COL_SCORE_NUM - 1));
                xmedia_float arr[right_idx - left_idx + 1];
                xmedia_float result_arr[right_idx - left_idx + 1];
                xmedia_float tmp_out = 0;
                for (k = left_idx; k <= right_idx; k++) {
                    arr[k - left_idx] = dequantize(
                        addr[k * XMEDIA_SVP_LANE_MAX_COL_POINT * XMEDIA_SVP_LANE_MAX_NUM +
                        j * XMEDIA_SVP_LANE_MAX_NUM + c_l[i]],
                        npu_model.output.tensor[1].quant.scale,
                        npu_model.output.tensor[1].quant.zp);
                }
                softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);
                for (k = left_idx; k <= right_idx; k++) {
                    tmp_out += k * result_arr[k - left_idx];
                }

                tmp_col[valid_point][0] = INPUT_DEFAULT_WIDE * j / (XMEDIA_SVP_LANE_MAX_COL_POINT - 1);
                tmp_col[valid_point][1] = (tmp_out + 0.5) / (SVP_LANE_COL_SCORE_NUM - 1) * INPUT_DEFAULT_HIGH;
                valid_point++;
            }
        }
        if (valid_point > XMEDIA_SVP_LANE_MAX_COL_POINT / 4) {
            for (k = 0; k < valid_point; k++) {
                result->lane_point_num[c_l[i]] = valid_point;
                result->col_result[i][k].x = tmp_col[k][0];
                result->col_result[i][k].y = tmp_col[k][1];
            }
        }
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 adas_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_adas_result *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_adas_context *adas_context = (svp_adas_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(adas_context == XMEDIA_NULL, "adas_process context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(input->frame_num != 2, "input frame num is err !!\n");
    CHECK_SVP_COND_RETURN_ERROR((input->frame == XMEDIA_NULL) || (input->frame + 1 == XMEDIA_NULL), 
        "input frame is NULL !!\n");
    xmedia_video_frame_info *big_frame = XMEDIA_NULL;
    xmedia_u64 plate_input_img_phy = 0;
    svp_ive_img input_crop_img = {0}, output_crop_img = {0};
    xmedia_video_frame_info *resize_frame = XMEDIA_NULL;
    xmedia_video_frame_info *plate_input_frame = XMEDIA_NULL;
    xmedia_s32 tmp_id;
    xmedia_float coeffs[XMEDIA_SVP_LANE_MAX_NUM][SVP_FIT_ORDER] = {0};
    xmedia_float second_coeffs[XMEDIA_SVP_LANE_MAX_NUM][SVP_FIT_ORDER] = {0};
    xmedia_s32 i, j;
    xmedia_float *rediduals = XMEDIA_NULL;
    xmedia_float mean_residuals = 0;
    xmedia_s32 clean_count = 0;

    // 1.车道线检测
    input_crop_img.width = SVP_LANE_INPUT_IMG_WIGTH;
    input_crop_img.hight = SVP_LANE_INPUT_IMG_HIGHT;
    input_crop_img.img_addr = xmedia_mmz_alloc(XMEDIA_NULL, "lane_input_crop",
        SVP_LANE_INPUT_IMG_WIGTH * SVP_LANE_INPUT_IMG_HIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(input_crop_img.img_addr == 0, EXIT, "lane_input_crop mmz alloc fail !\n");

    resize_frame = (xmedia_video_frame_info *)calloc(1, sizeof(xmedia_video_frame_info));
    CHECK_CALLOC_GOTO_ERROR(resize_frame, EXIT);
    resize_frame->frame.width = SVP_LANE_INPUT_IMG_WIGTH;
    resize_frame->frame.height = SVP_LANE_INPUT_IMG_HIGHT;
    resize_frame->frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    resize_frame->frame.stride.y_stride = SVP_LANE_INPUT_IMG_WIGTH;
    resize_frame->frame.stride.c_stride = SVP_LANE_INPUT_IMG_WIGTH;
    resize_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    resize_frame->frame.color_info.color_gamut = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    resize_frame->frame.addr.y_phy_addr = input_crop_img.img_addr;
    resize_frame->frame.addr.c_phy_addr = input_crop_img.img_addr + SVP_LANE_INPUT_IMG_WIGTH * SVP_LANE_INPUT_IMG_HIGHT;

    ret = svp_vgs_resize(input->frame, resize_frame);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize fail!\n");

    output_crop_img.width = SVP_LANE_INPUT_IMG_WIGTH;
    output_crop_img.hight = SVP_LANE_CROP_IMG_HIGHT;
    output_crop_img.img_addr = xmedia_mmz_alloc(XMEDIA_NULL, "lane_output_crop",
        SVP_LANE_INPUT_IMG_WIGTH * SVP_LANE_CROP_IMG_HIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(output_crop_img.img_addr == 0, EXIT , "lane_output_crop mmz alloc failed! \n");

    // 1.1裁剪
    ret = svp_ive_crop(&input_crop_img, &output_crop_img, 0, SVP_LANE_INPUT_IMG_HIGHT - SVP_LANE_CROP_IMG_HIGHT);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT , "ive_crop failed! \n");

    ret = npu_run_model(&adas_context->info.lane_info->npu_model, output_crop_img.img_addr);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "run lane model failed! \n");

    // 1.2后处理
    ret = adas_lane_post_process(adas_context->info.lane_info->npu_model, output);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "adas_post_process failed! \n");

    // 1.3 iqr过滤数据+二次多项式拟合
    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM / 2; i++) {
        tmp_id = i + 1;
        if (output->lane_point_num[tmp_id] <= 0) continue;

        xmedia_float x_tmp[XMEDIA_SVP_LANE_MAX_ROW_POINT] = {0};
        xmedia_float y_tmp[XMEDIA_SVP_LANE_MAX_ROW_POINT] = {0};
        for (j = 0; j < output->lane_point_num[tmp_id]; j++) {
            x_tmp[j] = output->row_result[i][j].y;
            y_tmp[j] = output->row_result[i][j].x;
        }

        // 首次拟合
        ret = polyfit(x_tmp, y_tmp, output->lane_point_num[tmp_id], coeffs[tmp_id]);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "polyfit fail! \n");

        // 清理异常点
        clean_count = 0;
        ret = data_filter(x_tmp, y_tmp, output->lane_point_num[tmp_id], coeffs[tmp_id], &rediduals, &mean_residuals);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "data_filter fail! \n");

        for (j = 0; j < output->lane_point_num[tmp_id]; j++) {
            if (rediduals[j] <= mean_residuals * 3) {
                x_tmp[clean_count] = x_tmp[j];
                y_tmp[clean_count] = y_tmp[j];
                clean_count++;
            }
        }
        CFREE(rediduals);

        // 清理异常点后二次拟合
        ret = polyfit(x_tmp, y_tmp, clean_count, second_coeffs[tmp_id]);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "polyfit fail! \n");

        ret = data_filter(x_tmp, y_tmp, clean_count, second_coeffs[tmp_id], &rediduals, &mean_residuals);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "data_filter fail! \n");

        if (mean_residuals > SVP_FIT_MAX_RESIDUALS || clean_count < SVP_FIT_MIN_NUM) {
            continue;
        }

        for (j = 0; j < clean_count; j++) {
            output->row_fitted_result[i][j].y = STD_MIN(STD_MAX(x_tmp[j], 0), INPUT_DEFAULT_HIGH);
            output->row_fitted_result[i][j].x =
                STD_MIN(STD_MAX(second_coeffs[tmp_id][2] * x_tmp[j] * x_tmp[j] +
                second_coeffs[tmp_id][1] * x_tmp[j] + second_coeffs[tmp_id][0], 0),
                INPUT_DEFAULT_WIDE);
        }
        output->lane_fitted_point_num[tmp_id] = clean_count;

        CFREE(rediduals);
    }

    for (i = 0; i < XMEDIA_SVP_LANE_MAX_NUM / 2; i++) {
        tmp_id = (i == 0) ? 0 :3;
        if (output->lane_point_num[tmp_id] <= 0) continue;
        xmedia_float x_tmp[XMEDIA_SVP_LANE_MAX_COL_POINT] = {0};
        xmedia_float y_tmp[XMEDIA_SVP_LANE_MAX_COL_POINT] = {0};
        for (j = 0; j < output->lane_point_num[tmp_id]; j++) {
            x_tmp[j] = output->col_result[i][j].y;
            y_tmp[j] = output->col_result[i][j].x;
        }

        // 首次拟合
        ret = polyfit(x_tmp, y_tmp, output->lane_point_num[tmp_id], coeffs[tmp_id]);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "polyfit fail! \n");

        // 清理异常点
        clean_count = 0;
        ret = data_filter(x_tmp, y_tmp, output->lane_point_num[tmp_id], coeffs[tmp_id], &rediduals, &mean_residuals);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "data_filter fail! \n");

        for (j = 0; j < output->lane_point_num[tmp_id]; j++) {
            if (rediduals[j] <= mean_residuals * 3) {
                x_tmp[clean_count] = x_tmp[j];
                y_tmp[clean_count] = y_tmp[j];
                clean_count++;
            }
        }
        CFREE(rediduals);

        // 清理异常点后二次拟合
        ret = polyfit(x_tmp, y_tmp, clean_count, second_coeffs[tmp_id]);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "polyfit fail! \n");

        ret = data_filter(x_tmp, y_tmp, clean_count, second_coeffs[tmp_id], &rediduals, &mean_residuals);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "data_filter fail! \n");

        if (mean_residuals > SVP_FIT_MAX_RESIDUALS || clean_count < SVP_FIT_MIN_NUM) {
            continue;
        }

        for (j = 0; j < clean_count; j++) {
            output->col_fitted_result[i][j].y = STD_MIN(STD_MAX(x_tmp[j], 0), INPUT_DEFAULT_HIGH);
            output->col_fitted_result[i][j].x =
                STD_MIN(STD_MAX(second_coeffs[tmp_id][2] * x_tmp[j] * x_tmp[j] +
                second_coeffs[tmp_id][1] * x_tmp[j] + second_coeffs[tmp_id][0], 0),
                INPUT_DEFAULT_WIDE);
        }
        output->lane_fitted_point_num[tmp_id] = clean_count;

        CFREE(rediduals);
    }

    memcpy(output->coeffs, second_coeffs, sizeof(xmedia_float) * XMEDIA_SVP_LANE_MAX_NUM * SVP_FIT_ORDER);

    // 车形检测
    ret = detect_process(adas_context->info.car_detect_info, input->frame,
        &output->car_result, adas_context->info.car_detect_info->anchors);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "car detect process err !\n");
    get_detect_class_id(&output->car_result,adas_context->info.car_detect_info->type);
    // 人形检测
    ret = detect_process(adas_context->info.person_detect_info, input->frame,
        &output->person_result, adas_context->info.person_detect_info->anchors);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "person detect process err !\n");
    get_detect_class_id(&output->person_result, adas_context->info.person_detect_info->type);
    // 非机动车检测
    ret = detect_process(adas_context->info.nmv_detect_info, input->frame,
        &output->nmv_result, adas_context->info.nmv_detect_info->anchors);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "nmv detect process err !\n");
    get_detect_class_id(&output->nmv_result, adas_context->info.nmv_detect_info->type);
    // 车牌检测
    big_frame = input->frame + 1;
    xmedia_s32 x1, y1, x2, y2, center_x, center_y;
    xmedia_float scale_r;
    xmedia_float inside_w;
    xmedia_float inside_h;
    xmedia_u32 w,h;
    xmedia_video_frame_info roi_frame;
    plate_input_img_phy = xmedia_mmz_alloc(XMEDIA_NULL, "plate_input_buf",
        SVP_PLATE_IMG_WIGTH * SVP_PLATE_IMG_HIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(plate_input_img_phy == 0, EXIT , "plate_input_buf mmz alloc failed! \n");

    plate_input_frame = (xmedia_video_frame_info *)malloc(sizeof(xmedia_video_frame_info));
    CHECK_CALLOC_GOTO_ERROR(plate_input_frame, EXIT);

    plate_input_frame->mod_id = MOD_ID_USER;
    plate_input_frame->frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    plate_input_frame->frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
    plate_input_frame->frame.bit_width = XMEDIA_VIDEO_DATA_WIDTH_8;
    plate_input_frame->frame.width = SVP_PLATE_IMG_WIGTH;
    plate_input_frame->frame.height = SVP_PLATE_IMG_HIGHT;
    plate_input_frame->frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    plate_input_frame->frame.stride.y_stride = SVP_PLATE_IMG_WIGTH;
    plate_input_frame->frame.stride.c_stride = SVP_PLATE_IMG_WIGTH;
    plate_input_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    plate_input_frame->frame.color_info.color_gamut = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    plate_input_frame->frame.addr.y_phy_addr = plate_input_img_phy;
    plate_input_frame->frame.addr.c_phy_addr = plate_input_img_phy + SVP_PLATE_IMG_WIGTH * SVP_PLATE_IMG_HIGHT;

    for (i = 0; i < output->car_result.target_num; i++) {
        center_x = (output->car_result.targets[i].rect.x1 + output->car_result.targets[i].rect.x2) / 2;
        center_y = (output->car_result.targets[i].rect.y1 + output->car_result.targets[i].rect.y2) / 2;
        if (INPUT_DEFAULT_HIGH - output->car_result.targets[i].rect.y2 == 0) {
            continue;
        }

        // 图像底边中心，与目标底边中心
        xmedia_float angle = (xmedia_float)abs(center_x - INPUT_DEFAULT_WIDE / 2) /
            (INPUT_DEFAULT_HIGH - output->car_result.targets[i].rect.y2);
        angle = atan(angle) * (180.f / M_PI);

        // 既不在车道线内也不在图像正前方
        if (in_lane(center_x, center_y, second_coeffs[1], second_coeffs[2]) == XMEDIA_FALSE &&
            (output->car_result.targets[i].rect.y2 <= (INPUT_DEFAULT_HIGH - 40)
            && angle > adas_context->info.dist_attr.car_limit_angle)) {
            continue;
        }

        // 摄像头安装角度较小时, 分母为车辆底线和地平线的垂直坐标的差值
        output->car_result.targets[i].distance =adas_context->info.dist_attr.focal_length_px *
            adas_context->info.dist_attr.camera_height /
            STD_MAX((output->car_result.targets[i].rect.y2 - INPUT_DEFAULT_HIGH / 2), 2);

        output->car_result.targets[i].special_target = XMEDIA_TRUE;

        // 小图坐标转大图
        x1 = (xmedia_s32)roundf(output->car_result.targets[i].rect.x1 / 2) * 2 * 3;
        y1 = (xmedia_s32)roundf(output->car_result.targets[i].rect.y1 / 2) * 2 * 3;
        x2 = (xmedia_s32)roundf(output->car_result.targets[i].rect.x2 / 2) * 2 * 3;
        y2 = (xmedia_s32)roundf(output->car_result.targets[i].rect.y2 / 2) * 2 * 3;
        center_x = (x1 + x2) / 2;
        center_y = (y1 + y2) / 2;
        w = abs(x2 - x1);
        h = abs(y2 - y1);

        // 等比缩放
        scale_r = (SVP_PLATE_IMG_WIGTH / (xmedia_float)w) < (SVP_PLATE_IMG_HIGHT / (xmedia_float)h) ?
            (SVP_PLATE_IMG_WIGTH / (xmedia_float)w) : (SVP_PLATE_IMG_HIGHT / (xmedia_float)h);
        inside_w = SVP_PLATE_IMG_WIGTH / scale_r;
        inside_h = SVP_PLATE_IMG_HIGHT / scale_r;
        x1 = STD_MAX((xmedia_s32)(center_x - inside_w * 1 / 2), 0);
        y1 = STD_MAX((xmedia_s32)(center_y - inside_h * 1 / 2), 0);
        x2 = STD_MIN((xmedia_s32)(center_x + inside_w * 1 / 2), 1920);
        y2 = STD_MIN((xmedia_s32)(center_y + inside_h * 1 / 2), 1080);

        x1 = x1 / 2 * 2;
        y1 = y1 / 2 * 2;
        w = abs(x2 - x1) / 2 * 2;
        h = abs(y2 - y1) / 2 * 2;

        if (w < VGS_LIMIT_MIN_INIMG_W || h < VGS_LIMIT_MIN_INIMG_H) {
            continue;
        }

        memcpy(&roi_frame, big_frame, sizeof(xmedia_video_frame_info));
        roi_frame.frame.addr.y_phy_addr += roi_frame.frame.stride.y_stride * y1 + x1;
        roi_frame.frame.addr.c_phy_addr += roi_frame.frame.stride.y_stride * y1 / 2 + x1;
        roi_frame.frame.width = w;
        roi_frame.frame.height = h;
        ret = svp_vgs_resize(&roi_frame, plate_input_frame);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize fail!\n");

        ret = npu_run_model(&adas_context->info.plate_detect_info->model, plate_input_img_phy);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate npu_run_model fail!\n");

        xmedia_svp_yolov5_output plate_result;
        // 3.1 车牌后处理
        ret = svp_get_detect_result_yolov5_bz(adas_context->info.plate_detect_info, &plate_result, plate_anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_detect_result_yolov5_bz fail!\n");

        if (plate_result.target_num > 0) {
            // 返回原坐标
            xmedia_float scale_x = (xmedia_float)(roi_frame.frame.width) / SVP_PLATE_IMG_WIGTH;
            xmedia_float scale_y = (xmedia_float)(roi_frame.frame.height) / SVP_PLATE_IMG_HIGHT;
            xmedia_s32 id = output->plate_result.target_num;
            output->plate_result.targets[id].rect.x1 = (plate_result.targets[0].rect.x1 * scale_x + x1) / 3;
            output->plate_result.targets[id].rect.x2 = (plate_result.targets[0].rect.x2 * scale_x + x1) / 3;
            output->plate_result.targets[id].rect.y1 = (plate_result.targets[0].rect.y1 * scale_y + y1) / 3;
            output->plate_result.targets[id].rect.y2 = (plate_result.targets[0].rect.y2 * scale_y + y1) / 3;
            output->plate_result.target_num++;

            // 有车牌优先使用车牌距离，没有车牌使用车辆距离
            xmedia_float p_h = fabsf(output->plate_result.targets[id].rect.y2 -
                output->plate_result.targets[id].rect.y1);

            if (p_h > 0) {
                // 反推摄像头高度
                xmedia_float plate_distance = adas_context->info.dist_attr.plate_width *
                    adas_context->info.dist_attr.focal_length_px / p_h;
                adas_context->info.dist_attr.camera_height =
                    adas_context->info.dist_attr.camera_height *
                    plate_distance / output->car_result.targets[i].distance;
                output->car_result.targets[i].distance = plate_distance;
            } else {
                SVP_TRACE(MODULE_DBG_ERR, "plate h is zero!\n");
            }
        }
    }

    CFREE(plate_input_frame);
    CFREE(resize_frame);

    if (plate_input_img_phy != 0) {
        xmedia_mmz_free(plate_input_img_phy);
    }

    if (output_crop_img.img_addr != 0) {
        xmedia_mmz_free(output_crop_img.img_addr);
    }

    if (input_crop_img.img_addr != 0) {
        xmedia_mmz_free(input_crop_img.img_addr);
    }

    return ret;

EXIT:
    CFREE(plate_input_frame);
    CFREE(resize_frame);
    CFREE(rediduals);

    if (plate_input_img_phy != 0) {
        xmedia_mmz_free(plate_input_img_phy);
    }

    if (output_crop_img.img_addr != 0) {
        xmedia_mmz_free(output_crop_img.img_addr);
    }

    if (input_crop_img.img_addr != 0) {
        xmedia_mmz_free(input_crop_img.img_addr);
    }

    return XMEDIA_FAILURE;
}
