#include "yolov5.h"

// 每层feature_map的下采样倍数
static const xmedia_s32 k_strides_3layer[LAYER_MIN_NUM] = {8, 16, 32};
static const xmedia_s32 k_strides_4layer[LAYER_NUM_FOUR] = {4, 8, 16, 32};

/**
 * @brief
 * @param width
 * @param height
 * @param stride yolov5最大下采样倍数=32
 * @return xmedia_s32
 */
static xmedia_s32 check_img_size(xmedia_u32 width, xmedia_u32 height, xmedia_s32 stride)
{
    if (((width % stride) != 0) && ((height % stride) != 0)) {
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 init_sigmoid_table(yolov5_detect_param* param)
{
    xmedia_float step = TABLE_200 / TABLE_SIZE;
    xmedia_float x = TABLE_MIN;
    xmedia_u32 i;
    param->sigmoid_table = (xmedia_float*)calloc(TABLE_SIZE, sizeof(xmedia_float));
    CHECK_CALLOC_RETURN_ERROR(param->sigmoid_table);

    for (i = 0; i < TABLE_SIZE; i++) {
        *(param->sigmoid_table + i) = 1.0f / (1.0f + exp(-x));
        x += step;
    }

    return XMEDIA_SUCCESS;
}

static inline xmedia_float sigmoid_table(xmedia_float* table, xmedia_float x)
{
    xmedia_u32 lower_index;
    xmedia_float index, fraction, lower_result, result;

    if ((x >= TABLE_MIN) && (x <= TABLE_MAX)) {
        index = (x + TABLE_MAX) * TABLE_500; // (x + TABLE_MAX) * TABLE_SIZE / TABLE_200
        lower_index = (xmedia_u32)index;
        fraction = index - lower_index;
        lower_result = *(table + lower_index);
        result = *(table + lower_index + 1);
        return (lower_result + fraction * (result - lower_result));
    }

    return (1.f / (1.f + exp(-x)));
}

xmedia_s32 init_sigmoid_dequantize_table(yolov5_detect_param *param)
{
    xmedia_s32 i;
    xmedia_float scale = param->quanlize[0].scale;
    xmedia_s32 zp = param->quanlize[0].zp;

    param->sigmoid_dequantize_table = (xmedia_float *)calloc(UNSIGNED_8_BIT_NUM, sizeof(xmedia_float));
    CHECK_CALLOC_GOTO_ERROR(param->sigmoid_dequantize_table, EXIT);
    param->sigmoid_dequantize_table_x2 = (xmedia_float *)calloc(UNSIGNED_8_BIT_NUM, sizeof(xmedia_float));
    CHECK_CALLOC_GOTO_ERROR(param->sigmoid_dequantize_table_x2, EXIT);
    param->sigmoid_dequantize_table_x6 = (xmedia_float *)calloc(UNSIGNED_8_BIT_NUM, sizeof(xmedia_float));
    CHECK_CALLOC_GOTO_ERROR(param->sigmoid_dequantize_table_x6, EXIT);
    param->sigmoid_dequantize_table_x2_sq = (xmedia_float *)calloc(UNSIGNED_8_BIT_NUM, sizeof(xmedia_float));
    CHECK_CALLOC_GOTO_ERROR(param->sigmoid_dequantize_table_x2_sq, EXIT);

    for (i = 0; i < 256; i++) {
        *(param->sigmoid_dequantize_table + i) = 1.f / (1.f + exp(-((xmedia_float)(scale * (i - zp)))));
        *(param->sigmoid_dequantize_table_x2 + i) = (1.f / (1.f + exp(-((xmedia_float)(scale * (i - zp)))))) * 2.0f;
        *(param->sigmoid_dequantize_table_x6 + i) = (1.f / (1.f + exp(-((xmedia_float)(scale * (i - zp)))))) * 6.0f;
        *(param->sigmoid_dequantize_table_x2_sq +i) =
            (xmedia_float)pow((1.f / (1.f + exp(-((xmedia_float)(scale * (i - zp)))))) * 2.0f, 2);
    }

    return XMEDIA_SUCCESS;
EXIT:
    CFREE(param->sigmoid_dequantize_table);
    CFREE(param->sigmoid_dequantize_table_x2);
    CFREE(param->sigmoid_dequantize_table_x6);
    CFREE(param->sigmoid_dequantize_table_x2_sq);
    return XMEDIA_FAILURE;
}

xmedia_void set_default_yolov5_detect_param(yolov5_detect_param *param)
{
    xmedia_s32 i;

    if (param) {
        param->w = INPUT_DEFAULT_WIDE;
        param->h = INPUT_DEFAULT_HIGH;
        param->num = DETECTION_MODEL_MAX;
        param->detect_threshold = DEFAULT_DETECT_THRESHOLD;
        param->iou_threshold = DEFAULT_IOU_THRESHOLD;
        param->max_target_num = XMEDIA_SVP_MAX_TARGET_NUM;
        param->classifier_threshold = DEFAULT_CLASSIFIER_THRESHOLD;
        param->thres_desig = desigmoid(param->detect_threshold);
        param->dup_flag = XMEDIA_FALSE;
        param->aov_flag = XMEDIA_FALSE;
        param->aov_only_target = XMEDIA_FALSE;
        param->movement.enable = XMEDIA_FALSE;
        param->movement.stillness_thres = SVP_STILLNESS_THRES;
        param->movement.movement_fps_thres = SVP_MOVEMENT_FPS_THRES;
        param->movement.num = (XMEDIA_SVP_MAX_TARGET_NUM * SVP_MOVELESS_COUNT);
        param->tracklet.enable = XMEDIA_FALSE;
        param->tracklet.high_score_thres = param->detect_threshold;
        param->tracklet.activated_tracker_thres = SVP_ACTIVATED_TRACKER_THRESHOLD;
        param->tracklet.tracked_tracker_thres = SVP_TRACKED_TRACKER_THRESHOLD;
        param->tracklet.unactivated_tracker_thres = SVP_UNACTIVATED_TRACKER_THRESHOLD;
        param->tracklet.track_age_thres = SVP_TRACK_AGE_THRESHOLD;
        memset(param->tarck_id_arry, 0, sizeof(xmedia_bool) * XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
        param->tarck_id_grow = -1;
        param->smart_ae_enable = XMEDIA_FALSE;
        param->smart_venc_enable = XMEDIA_FALSE;
        for (i = 0; i < XMEDIA_SVP_MAX_VENC_CHN_NUM; i++) {
            param->smart_venc_array[i] = XMEDIA_FALSE;
        }
        for (i = 0; i < XMEDIA_SVP_MAX_VI_PIPE_NUM; i++) {
            param->smart_ae_array[i] = XMEDIA_FALSE;
        }
        for (i = 0; i < XMEDIA_SVP_MAX_TARGET_NUM; i++) {
            memset(&param->record_result.stable_targets[i].rect, 0,
                sizeof(param->record_result.stable_targets[i].rect));
        }
    }
}

static xmedia_s32 svp_get_detect_result(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                                    const xmedia_svp_detect_anchors anchors_yaml)
{
    xmedia_s32 i, j, k, l, id;
    xmedia_u32 idx = 0, offset, out_box_len;
    xmedia_u8 *addr, thres_u8, conf_u8;
    xmedia_u32 xywh_offset_base, obj_offset_base, multi_cls_offset_base;
    xmedia_float conf, p_x, p_y, p_w, p_h, x, y, w, h;
    xmedia_float class_conf = 0;
    xmedia_float class_conf_tmp = 0;
    xmedia_u32 downsample_3layer, downsample_4layer;
    xmedia_u32 p_w_base, p_h_base;
#ifdef SVP_DEBUG_SAVE_FILE
    xmedia_s32 n;
    xmedia_s32 size;
    xmedia_char name[64];
#endif

#ifdef SVP_DEBUG_SAVE_FILE
    SVP_TRACE(MODULE_DBG_ERR, "################################# \n");
    for (n = 0; n < anchors_yaml.layer_num; n++) {
        size = param->feature[n] * 3 * (XYWHF_LEN + param->num);
        SVP_TRACE(MODULE_DBG_ERR, "size:%d ,n:%d\n",size ,n);
        sprintf(name, "output_data%d.bin", n);
        FILE *fp = fopen(name,"wb");
        if (!fp) {
            printf("cannot open %s\n", name);
        }
        fwrite((void*)param->model.output.tensor[n].addr, size, 1, fp);
        fclose(fp);
    }
    SVP_TRACE(MODULE_DBG_ERR, "################################# \n");
#endif

    for (i = 0; i < anchors_yaml.layer_num; i++) {
        downsample_3layer = param->w / k_strides_3layer[i];
        downsample_4layer = param->w / k_strides_4layer[i % 4];
        p_w_base = param->feature[i] * 2;
        p_h_base = param->feature[i] * 3;
#ifdef SVP_USE_LAST_THREE_LATER
        if ((anchors_yaml.layer_num == LAYER_NUM_FOUR) && (i == 0)) {
            continue;
        }
        if ((anchors_yaml.layer_num == LAYER_MAX_NUM) && (i == 0) && (i == 4)) {
            continue;
        }
#endif
        addr = (xmedia_u8 *)param->model.output.tensor[i].addr;
        thres_u8 = quantize(param->thres_desig, param->quanlize[i].scale, param->quanlize[i].zp);
#ifdef SVP_DEBUG_SAVE_FILE
        SVP_TRACE(MODULE_DBG_ERR, "output quantize data param: scale:%f, zp:%d, num:%d\n",
                                  param->quanlize[i].scale, param->quanlize[i].zp, param->num);
#endif
        for (j = 0; j < ANCHOR_NUM; j++) {
            xywh_offset_base = j * XYWH_LEN * param->feature[i];
            obj_offset_base = (ANCHOR_NUM * XYWH_LEN + j)* param->feature[i];
            multi_cls_offset_base = (ANCHOR_NUM * XYWHF_LEN + j * param->num - XYWHF_LEN ) * param->feature[i];
            for (k = 0; k < param->feature[i]; k++) {
                if(idx >= DEFAULT_BBOX_NUM) {
                    break;
                }
                if (addr[obj_offset_base + k] < thres_u8) {
                    continue;
                }

                conf_u8 = addr[obj_offset_base + k];
                // find max class confidence and id
                id = 0;
                class_conf_tmp = 0;
                if (param->type == XMEDIA_SVP_ALG_TYPE_PERSON || param->type == XMEDIA_SVP_ALG_TYPE_BOUNDARY_PERSON) {
                    conf = *(param->sigmoid_dequantize_table + conf_u8);
                } else {
                    if (param->num == DETECTION_MODEL_SINGLE) {
                        conf = sigmoid_table(param->sigmoid_table,
                            dequantize(conf_u8, param->quanlize[i].scale, param->quanlize[i].zp));
                    } else {
                        for (l = XYWHF_LEN; l < (XYWHF_LEN + param->num); l++) {
                            offset = multi_cls_offset_base + l * param->feature[i] + k;
                            class_conf = sigmoid_table(param->sigmoid_table,
                                dequantize(addr[offset], param->quanlize[i].scale, param->quanlize[i].zp));
                            if (class_conf > class_conf_tmp) {
                                class_conf_tmp = class_conf;
                                id = l - XYWHF_LEN;
                            }
                        }
                        conf = class_conf_tmp * sigmoid_table(param->sigmoid_table,
                            dequantize(conf_u8, param->quanlize[i].scale, param->quanlize[i].zp));
                    }
                }
                if (conf > param->detect_threshold) {
                    offset = xywh_offset_base + k;
                    /** 预测值 */
                    if (param->type == XMEDIA_SVP_ALG_TYPE_PERSON ||
                        param->type == XMEDIA_SVP_ALG_TYPE_BOUNDARY_PERSON) {
                        // param->feature[i] * 0 + offset
                        p_x = *(param->sigmoid_dequantize_table_x2 + addr[offset]);
                        // param->feature[i] * 1 + offset
                        p_y = *(param->sigmoid_dequantize_table_x6 + addr[param->feature[i] + offset]);
                        p_w = *(param->sigmoid_dequantize_table_x2_sq + addr[p_w_base + offset]);
                        p_h = *(param->sigmoid_dequantize_table_x2_sq + addr[p_h_base + offset]);
                        /** 真实值 */
                        if (anchors_yaml.layer_num == LAYER_MIN_NUM) {
                            x = (p_x + ((k % downsample_3layer) - 0.5f)) * k_strides_3layer[i];
                            y = (p_y + ((k / downsample_3layer) - 0.3f)) * k_strides_3layer[i];
                            w = p_w * anchors_yaml.anchors[i][j].x;
                            h = p_h * anchors_yaml.anchors[i][j].y;
                        } else {
                            x = (p_x + ((k % downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            y = (p_y + ((k / downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            w = p_w * anchors_yaml.anchors[i][j].x;
                            h = p_h * anchors_yaml.anchors[i][j].y;
                        }
                    } else {
                        /** 预测值 */
                        p_x = sigmoid_table(param->sigmoid_table, dequantize(addr[offset],
                            param->quanlize[i].scale, param->quanlize[i].zp));
                        p_y = sigmoid_table(param->sigmoid_table, dequantize(addr[param->feature[i] + offset],
                            param->quanlize[i].scale, param->quanlize[i].zp));
                        p_w = sigmoid_table(param->sigmoid_table, dequantize(addr[p_w_base + offset],
                            param->quanlize[i].scale, param->quanlize[i].zp));
                        p_h = sigmoid_table(param->sigmoid_table, dequantize(addr[p_h_base + offset],
                            param->quanlize[i].scale, param->quanlize[i].zp));

                        /** 真实值 */
                        if (anchors_yaml.layer_num == LAYER_MIN_NUM) {
                            x = (p_x * 2.0f + ((k % downsample_3layer) - 0.5f)) * k_strides_3layer[i];
                            if (param->type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE) {
                                y = (-p_y * 6.0f + ((k / downsample_3layer) + 1.5f)) * k_strides_3layer[i];
                            } else {
                                y = (p_y * 2.0f + ((k / downsample_3layer) - 0.5f)) * k_strides_3layer[i];
                            }
                            w = (p_w * p_w * 4.0f) * anchors_yaml.anchors[i][j].x; // (p_w * 2.0f) * (p_w * 2.0f)
                            h = (p_h * p_h * 4.0f) * anchors_yaml.anchors[i][j].y;
                        } else {
                            x = (p_x * 2.0f + ((k % downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            y = (p_y * 2.0f + ((k / downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            // (p_w * 2.0f) * (p_w * 2.0f)
                            w = (p_w * p_w * 4.0f) * anchors_yaml.anchors[i][j].x;
                            h = (p_h * p_h * 4.0f) * anchors_yaml.anchors[i][j].y;
                        }
                    }

#if 0
                    // 目标框面积大小限制
                    if (param->box_area_limit.enable) {
                        box_area = w * h;
                        if ((box_area < param->box_area_limit.min_area) || (box_area > param->box_area_limit.max_area)) {
                            continue;
                        }
                    }

                    // 边缘过滤
                    if (param->box_edge_limit.enable) {
                        left_edge = param->box_edge_limit.w_edge;
                        right_edge = param->w - param->box_edge_limit.w_edge;
                        top_edge = param->box_edge_limit.h_edge;
                        bottom_edge = param->h - param->box_edge_limit.h_edge;
                        if (param->type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE ||
                            param->type == XMEDIA_SVP_ALG_TYPE_HEAD) {
                            if (STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y + h * 0.5f)) > bottom_edge
                                && STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y - h * 0.5f)) > 180) {
                                continue;
                            }
                        }else{
                            if (STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x - w * 0.5f)) < left_edge
                                || STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x + w * 0.5f)) > right_edge
                                || STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y - h * 0.5f)) < top_edge
                                || STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y + h * 0.5f)) > bottom_edge) {
                                continue;
                            }
                        }
                    }
#endif
                    param->bbox[idx].detect_score = conf;
//                    bbox[idx].alg_type = param->type;
//                    bbox[idx].classfier_score = 0.0f;
                    // 双模型拼接时 前四层和后四层分别为一个类别
                    param->bbox[idx].class_type = (xmedia_svp_class_type)id + (i / 4);
                    param->bbox[idx].rect.x1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x - w * 0.5f));
                    param->bbox[idx].rect.y1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y - h * 0.5f));
                    param->bbox[idx].rect.x2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x + w * 0.5f));
                    param->bbox[idx].rect.y2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y + h * 0.5f));
                    idx++;
                }
            }
        }
    }

    if (idx == 0) {
        result->target_num = 0;
        return XMEDIA_SUCCESS;
    } else if (idx == 1) {
        result->targets[0].detect_score = param->bbox[0].detect_score;
        result->targets[0].rect         = param->bbox[0].rect;
        result->targets[0].class_type   = param->bbox[0].class_type;
        result->targets[0].alg_type     = param->type;
        result->target_num = idx;
        return XMEDIA_SUCCESS;
    }

    out_box_len = 0;
    det_nms(param->bbox, idx, param->iou_threshold, &out_box_len);
    // get result
    result->target_num = 0;
    for (i = 0; i < out_box_len; i++) {
        result->targets[result->target_num].detect_score = param->bbox[i].detect_score;
        result->targets[result->target_num].rect         = param->bbox[i].rect;
        result->targets[result->target_num].class_type   = param->bbox[i].class_type;
        result->targets[result->target_num].alg_type     = param->type;
        result->target_num += 1;
        if (result->target_num >= param->max_target_num) {
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 svp_get_aov_detect_result(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                                               const xmedia_svp_detect_anchors anchors_yaml)
{
    xmedia_s32 i, j, k, l, id;
    xmedia_u32 idx = 0, offset, out_box_len;
    xmedia_u8 *addr, thres_u8, conf_u8;
    xmedia_float conf, p_x, p_y, p_w, p_h, x, y, w, h;
    xmedia_u32 xywh_offset_base, obj_offset_base, multi_cls_offset_base;
    xmedia_float class_conf = 0;
    xmedia_float class_conf_tmp = 0;
    xmedia_u32 p_w_base, p_h_base;
    xmedia_u32 downsample_3layer, downsample_4layer;
#ifdef SVP_DEBUG_SAVE_FILE
    xmedia_s32 n;
    xmedia_s32 size;
    xmedia_char name[64];
#endif

#ifdef SVP_DEBUG_SAVE_FILE
    SVP_TRACE(MODULE_DBG_ERR, "################################# \n");
    for (n = 0; n < anchors_yaml.layer_num; n++) {
        size = param->feature[n] * 3 * (XYWHF_LEN + param->num);
        SVP_TRACE(MODULE_DBG_ERR, "size:%d ,n:%d\n",size ,n);
        sprintf(name, "output_data%d.bin", n);
        FILE *fp = fopen(name,"wb");
        if (!fp) {
            printf("cannot open %s\n", name);
        }
        fwrite((void*)param->model.output.tensor[n].addr, size, 1, fp);
        fclose(fp);
    }
    SVP_TRACE(MODULE_DBG_ERR, "################################# \n");
#endif

    for (i = 0; i < anchors_yaml.layer_num; i++) {
        addr = (xmedia_u8 *)param->model.output.tensor[i*2].addr;
        thres_u8 = quantize(param->thres_desig, param->quanlize[i*2].scale, param->quanlize[i*2].zp);
        p_w_base = param->feature[i] * 2;
        p_h_base = param->feature[i] * 3;
        downsample_3layer = param->w / k_strides_3layer[i];
        downsample_4layer = param->w / k_strides_4layer[i % 4];
#ifdef SVP_DEBUG_SAVE_FILE
        SVP_TRACE(MODULE_DBG_ERR, "output quantize data param: scale:%f, zp:%d, num:%d\n",
                                  param->quanlize[i*2].scale, param->quanlize[i*2].zp, param->num);
#endif
        for (j = 0; j < ANCHOR_NUM; j++) {
            xywh_offset_base = j * XYWH_LEN * param->feature[i];
            obj_offset_base = (ANCHOR_NUM * XYWH_LEN + j)* param->feature[i];
            multi_cls_offset_base = (ANCHOR_NUM * XYWHF_LEN + j * param->num - XYWHF_LEN ) * param->feature[i];
            for (k = 0; k < param->feature[i]; k++) {
                if(idx >= DEFAULT_BBOX_NUM) {
                    break;
                }
                if (addr[obj_offset_base + k] < thres_u8) {
                    continue;
                }
                // find max class confidence and id
                id = 0;
                class_conf_tmp = 0;
                conf_u8 = addr[obj_offset_base + k];
                if (param->type == XMEDIA_SVP_ALG_TYPE_PERSON ||
                    param->type == XMEDIA_SVP_ALG_TYPE_BOUNDARY_PERSON) {
                    conf = *(param->sigmoid_dequantize_table + conf_u8);
                } else {
                    for (l = XYWHF_LEN; l < (XYWHF_LEN + param->num); l++) {
                        offset = multi_cls_offset_base + l * param->feature[i] + k;
                        class_conf = sigmoid_table(param->sigmoid_table,
                            dequantize(addr[offset], param->quanlize[i*2].scale, param->quanlize[i*2].zp));
                        if (class_conf > class_conf_tmp) {
                            class_conf_tmp = class_conf;
                            id = l - XYWHF_LEN;
                        }
                    }
                    conf = class_conf_tmp * sigmoid_table(param->sigmoid_table,
                        dequantize(conf_u8, param->quanlize[i*2].scale, param->quanlize[i*2].zp));
                }
                if (conf > param->detect_threshold) {
                    offset = xywh_offset_base + k;
                    /** 预测值 */
                    if (param->type == XMEDIA_SVP_ALG_TYPE_PERSON ||
                        param->type == XMEDIA_SVP_ALG_TYPE_BOUNDARY_PERSON) {
                        p_x = *(param->sigmoid_dequantize_table_x2 + addr[offset]);
                        p_y = *(param->sigmoid_dequantize_table_x6 + addr[param->feature[i] + offset]);
                        p_w = *(param->sigmoid_dequantize_table_x2_sq + addr[p_w_base + offset]);
                        p_h = *(param->sigmoid_dequantize_table_x2_sq + addr[p_h_base + offset]);
                        /** 真实值 */
                        if (anchors_yaml.layer_num == LAYER_MIN_NUM) {
                            x = (p_x + ((k % downsample_3layer) - 0.5f)) * k_strides_3layer[i];
                            y = (p_y + ((k / downsample_3layer) - 0.3f)) * k_strides_3layer[i];
                            w = p_w * anchors_yaml.anchors[i][j].x;
                            h = p_h * anchors_yaml.anchors[i][j].y;
                        } else {
                            x = (p_x + ((k % downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            y = (p_y + ((k / downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            w = p_w * anchors_yaml.anchors[i][j].x;
                            h = p_h * anchors_yaml.anchors[i][j].y;
                        }
                    } else {
                        /** 预测值 */
                        p_x = sigmoid_table(param->sigmoid_table, dequantize(addr[offset],
                            param->quanlize[i*2].scale, param->quanlize[i*2].zp));
                        p_y = sigmoid_table(param->sigmoid_table, dequantize(addr[param->feature[i]  + offset],
                            param->quanlize[i*2].scale, param->quanlize[i*2].zp));
                        p_w = sigmoid_table(param->sigmoid_table, dequantize(addr[p_w_base + offset],
                            param->quanlize[i*2].scale, param->quanlize[i*2].zp));
                        p_h = sigmoid_table(param->sigmoid_table, dequantize(addr[p_h_base + offset],
                            param->quanlize[i*2].scale, param->quanlize[i*2].zp));

                        /** 真实值 */
                        if (anchors_yaml.layer_num == LAYER_MIN_NUM) {
                            x = (p_x * 2.0f + ((k % downsample_3layer) - 0.5f)) * k_strides_3layer[i];
                            if (param->type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE) {
                                y = (-p_y * 6.0f + ((k / downsample_3layer) + 1.5f)) * k_strides_3layer[i];
                            } else {
                                y = (p_y * 2.0f + ((k / downsample_3layer) - 0.5f)) * k_strides_3layer[i];
                            }
                            w = (p_w * 2.0f) * (p_w * 2.0f) * anchors_yaml.anchors[i][j].x;
                            h = (p_h * 2.0f) * (p_h * 2.0f) * anchors_yaml.anchors[i][j].y;
                        } else {
                            x = (p_x * 2.0f + ((k % downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            y = (p_y * 2.0f + ((k / downsample_4layer) - 0.5f)) * k_strides_4layer[i % 4];
                            w = (p_w * 2.0f) * (p_w * 2.0f) * anchors_yaml.anchors[i][j].x;
                            h = (p_h * 2.0f) * (p_h * 2.0f) * anchors_yaml.anchors[i][j].y;
                        }
                    }
#if 0
                    // 目标框面积大小限制
                    if (param->box_area_limit.enable) {
                        box_area = w * h;
                        if ((box_area < param->box_area_limit.min_area) || (box_area > param->box_area_limit.max_area)) {
                            continue;
                        }
                    }

                    // 边缘过滤
                    if (param->box_edge_limit.enable) {
                        left_edge = param->box_edge_limit.w_edge;
                        right_edge = param->w - param->box_edge_limit.w_edge;
                        top_edge = param->box_edge_limit.h_edge;
                        bottom_edge = param->h - param->box_edge_limit.h_edge;
                        if (param->type == XMEDIA_SVP_ALG_TYPE_NON_MOTORIZED_VEHICLE ||
                            param->type == XMEDIA_SVP_ALG_TYPE_HEAD) {
                            if (STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y + h * 0.5f)) > bottom_edge
                                && STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y - h * 0.5f)) > 180) {
                                continue;
                            }
                        }else{
                            if (STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x - w * 0.5f)) < left_edge
                                || STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x + w * 0.5f)) > right_edge
                                || STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y - h * 0.5f)) < top_edge
                                || STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y + h * 0.5f)) > bottom_edge) {
                                continue;
                            }
                        }
                    }
#endif
                    param->bbox[idx].detect_score = conf;
                    // 双模型拼接时 前四层和后四层分别为一个类别
                    param->bbox[idx].class_type = (xmedia_svp_class_type)id;
                    param->bbox[idx].rect.x1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x - w * 0.5f));
                    param->bbox[idx].rect.y1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y - h * 0.5f));
                    param->bbox[idx].rect.x2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x + w * 0.5f));
                    param->bbox[idx].rect.y2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y + h * 0.5f));
                    idx++;
                }
            }
        }
    }

    if (idx == 0) {
        result->target_num = 0;
        return XMEDIA_SUCCESS;
    } else if (idx == 1) {
        result->targets[0].detect_score = param->bbox[0].detect_score;
        result->targets[0].rect         = param->bbox[0].rect;
        result->targets[0].class_type   = param->bbox[0].class_type;
        result->targets[0].alg_type     = param->type;
        result->target_num = idx;
        return XMEDIA_SUCCESS;
    }
    out_box_len = 0;
    det_nms(param->bbox, idx, param->iou_threshold, &out_box_len);
    // get result
    result->target_num = 0;
    for (i = 0; i < out_box_len; i++) {
        result->targets[result->target_num].detect_score = param->bbox[i].detect_score;
        result->targets[result->target_num].rect         = param->bbox[i].rect;
        result->targets[result->target_num].class_type   = param->bbox[i].class_type;
        result->targets[result->target_num].alg_type     = param->type;
        result->target_num += 1;
        if (result->target_num >= param->max_target_num) {
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_get_detect_result_yolov5_bz(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
    const xmedia_svp_detect_anchors anchors_yaml)
{
    xmedia_s32 i, j, x, y;
    xmedia_s32 num_grid_x, num_grid_y;
    xmedia_s32 stride;
    xmedia_s32 num_grid;
    xmedia_u8 *addr;
    xmedia_u32 offset;
    xmedia_float cx, cy, w_b, h_b;
    xmedia_u32 idx = 0;
    xmedia_u32 out_box_len;

    for (i = 0; i < anchors_yaml.layer_num; i++) {
        stride = (xmedia_s32)pow(2, i) * 8;
        // h
        num_grid = param->model.output.tensor[i].shape.dims[2];
        if (param->w > param->h) {
            num_grid_x = param->w / stride;
            num_grid_y = num_grid / num_grid_x;
        } else {
            num_grid_y = param->h / stride;
            num_grid_x = num_grid / num_grid_y;
        }
        addr = (xmedia_u8*)param->model.output.tensor[i].addr;
        offset = 0;

        for (j = 0; j < ANCHOR_NUM; j++) {
            xmedia_float anchor_x = anchors_yaml.anchors[i][j].x;
            xmedia_float anchor_y = anchors_yaml.anchors[i][j].y;
            for (y = 0; y < num_grid_y; y++) {
                for (x = 0; x < num_grid_x; x++) {
                    if(idx >= DEFAULT_BBOX_NUM) {
                        break;
                    }
                    xmedia_float score = sigmoid_table(param->sigmoid_table, dequantize(addr[offset + 4],
                        param->quanlize[i].scale, param->quanlize[i].zp));
                    if (score >= param->detect_threshold) {
                        cx = (sigmoid_table(param->sigmoid_table, dequantize(addr[offset],
                            param->quanlize[i].scale, param->quanlize[i].zp)) * 2.f - 0.5f + x) * stride;
                        cy = (sigmoid_table(param->sigmoid_table, dequantize(addr[offset + 1],
                            param->quanlize[i].scale, param->quanlize[i].zp)) * 2.f - 0.5f + y) * stride;
                        w_b = pow(sigmoid_table(param->sigmoid_table, dequantize(addr[offset + 2],
                            param->quanlize[i].scale, param->quanlize[i].zp))* 2.f, 2) * anchor_x;
                        h_b = pow(sigmoid_table(param->sigmoid_table, dequantize(addr[offset + 3],
                            param->quanlize[i].scale, param->quanlize[i].zp))* 2.f, 2) * anchor_y;

                        if (w_b < 2 || h_b <= 2) {
                            continue;
                        }

                        param->bbox[idx].detect_score = score;
                        param->bbox[idx].rect.x1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, cx - w_b * 0.5f));
                        param->bbox[idx].rect.y1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, cy - h_b * 0.5f));
                        param->bbox[idx].rect.x2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, cx + w_b * 0.5f));
                        param->bbox[idx].rect.y2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, cy + h_b * 0.5f));
                        idx++;
                    }
                    // w
                    offset += param->model.output.tensor[i].shape.dims[3];
                }
            }

        }
    }

    if (idx == 0) {
        result->target_num = 0;
        return XMEDIA_SUCCESS;
    } else if (idx == 1) {
        result->targets[0].detect_score = param->bbox[0].detect_score;
        result->targets[0].rect         = param->bbox[0].rect;
        result->target_num = idx;
        return XMEDIA_SUCCESS;
    }

    out_box_len = 0;
    det_nms(param->bbox, idx, param->iou_threshold, &out_box_len);
    // get result
    result->target_num = 0;
    for (i = 0; i < out_box_len; i++) {
        result->targets[result->target_num].detect_score = param->bbox[i].detect_score;
        result->targets[result->target_num].rect         = param->bbox[i].rect;
        result->target_num += 1;
        if (result->target_num >= param->max_target_num) {
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_float svp_find_max_overlap(svp_stable_box history_box, xmedia_svp_rect cur_rect,
                                                    xmedia_svp_class_type class_type, xmedia_s32 *history_id,
                                                    xmedia_u32 history_box_num)
{
    xmedia_s32 i;
    xmedia_s32 id = -1;
    xmedia_float area1;
    xmedia_float area2;
    xmedia_float mix_area;
    xmedia_float ratio = 0.0f;
    xmedia_float max_ratio = 0.0f;

    for (i = 0; i < history_box_num; i++) {
        if (((history_box.stable_targets[i].rect.x2 - SVP_EPSILON) <= 0 &&
            (history_box.stable_targets[i].rect.x1 - SVP_EPSILON) <= 0) ||
            ((history_box.stable_targets[i].rect.y1 - SVP_EPSILON) <= 0 &&
            (history_box.stable_targets[i].rect.y2 - SVP_EPSILON) <= 0)) {
            continue;
        }
        if (history_box.stable_targets[i].class_type != class_type) {
            continue;
        }

        xmedia_svp_rect history_rect = history_box.stable_targets[i].rect;
        mix_area = STD_MAX(0.0f, STD_MIN(history_rect.x2, cur_rect.x2) - STD_MAX(history_rect.x1, cur_rect.x1)) *
                   STD_MAX(0.0f, STD_MIN(history_rect.y2, cur_rect.y2) - STD_MAX(history_rect.y1, cur_rect.y1));
        if ((mix_area - SVP_EPSILON) <= 0.0f) {
            continue;
        }
        area1 = (history_rect.x2 - history_rect.x1) * (history_rect.y2 - history_rect.y1);
        area2 = (cur_rect.x2 - cur_rect.x1) * (cur_rect.y2 - cur_rect.y1);
        // mix_area为area1与area2相交面积，所以area1 + area2 - mix_area恒大于0
        ratio = mix_area / (area1 + area2 - mix_area);

        if (max_ratio < ratio && ratio > SVP_IOU_STABLE_THRESH_MIDDLE) {
            max_ratio = ratio;
            id = i;
        }
    }
    *history_id = id;

    return max_ratio;
}

static xmedia_float svp_clear_record_box(xmedia_svp_rect rect1, xmedia_svp_rect rect2)
{
    xmedia_float area1;
    xmedia_float area2;
    xmedia_float mix_area;

    mix_area = STD_MAX(0.0f, STD_MIN(rect1.x2, rect2.x2) - STD_MAX(rect1.x1, rect2.x1)) *
               STD_MAX(0.0f, STD_MIN(rect1.y2, rect2.y2) - STD_MAX(rect1.y1, rect2.y1));
    if ((mix_area - SVP_EPSILON) <= 0.0f) {
        return 0.0f;
    }

    area1 = (rect1.x2 - rect1.x1) * (rect1.y2 - rect1.y1);
    area2 = (rect2.x2 - rect2.x1) * (rect2.y2 - rect2.y1);

    return (mix_area / (area1 + area2 - mix_area));
}

static xmedia_void svp_clear_record_by_idx(svp_stable_box *history_box, xmedia_u32 idx,
                                                        xmedia_float iou_threshold, xmedia_u32 history_box_num)
{
    xmedia_s32 i;
    xmedia_float value;

    for (i = 0; i < XMEDIA_SVP_MAX_TARGET_NUM; i++) {
        if (idx == i) {
            continue;
        }
        if (((history_box->stable_targets[i].rect.x2 - SVP_EPSILON) <= 0) &&
            ((history_box->stable_targets[i].rect.x1 - SVP_EPSILON) <= 0)) {
            continue;
        }
        if (history_box->stable_targets[i].class_type != history_box->stable_targets[idx].class_type) {
            continue;
        }

        value = svp_clear_record_box(history_box->stable_targets[i].rect, history_box->stable_targets[idx].rect);
        if (value > iou_threshold) {
            memset(&history_box->stable_targets[i].rect, 0, sizeof(history_box->stable_targets[i].rect));
        }
    }
}

static xmedia_void svp_side_stable_box(svp_stable_box *history_box, xmedia_s32 history_idx,
                                                xmedia_svp_rect *cur_rect, xmedia_u32 history_box_num)
{
    xmedia_float left_diff;
    xmedia_float right_diff;
    xmedia_float top_diff;
    xmedia_float bottom_diff;
    xmedia_float width_diff;
    xmedia_float height_diff;
    xmedia_float history_box_width;
    xmedia_float history_box_height;
    xmedia_float cur_box_width;
    xmedia_float cur_box_height;

    if ((history_idx < 0) || (history_idx > history_box_num)) {
        return;
    }

    history_box_width = fabs(history_box->stable_targets[history_idx].rect.x2 -
                    history_box->stable_targets[history_idx].rect.x1);
    history_box_height = fabs(history_box->stable_targets[history_idx].rect.y2 -
                    history_box->stable_targets[history_idx].rect.y1);
    cur_box_width = fabs(cur_rect->x2 - cur_rect->x1);
    cur_box_height = fabs(cur_rect->y2 - cur_rect->y1);

    left_diff = fabs(cur_rect->x1 - history_box->stable_targets[history_idx].rect.x1);
    right_diff = fabs(cur_rect->x2 - history_box->stable_targets[history_idx].rect.x2);
    top_diff = fabs(cur_rect->y1 - history_box->stable_targets[history_idx].rect.y1);
    bottom_diff = fabs(cur_rect->y2 - history_box->stable_targets[history_idx].rect.y2);
    width_diff = fabs(cur_box_width - history_box_width);
    height_diff = fabs(history_box_height - cur_box_height);

    if ((left_diff * SVP_STABLE_HORIZONTAL_FACTOR < history_box_width) ||
        (width_diff * SVP_STABLE_HORIZONTAL_FACTOR < history_box_width) ||
       ((right_diff * SVP_UNIFORM_FACTOR_A  < left_diff * SVP_UNIFORM_FACTOR_B) &&
        (left_diff * SVP_UNIFORM_FACTOR_A < right_diff * SVP_UNIFORM_FACTOR_B)) ||
       (((xmedia_s32)left_diff ^ (xmedia_s32)right_diff) < 0)) {
        cur_rect->x1 = cur_rect->x1 * SVP_STABLE_CURRENT_RATIO +
                      history_box->stable_targets[history_idx].rect.x1 * SVP_STABLE_HISTORY_RATIO;
        cur_rect->x2 = cur_rect->x2 * SVP_STABLE_CURRENT_RATIO +
                      history_box->stable_targets[history_idx].rect.x2 * SVP_STABLE_HISTORY_RATIO;
    }

    if ((top_diff * SVP_STABLE_VERTICAL_FACTOR < history_box_height) ||
        (height_diff * SVP_STABLE_VERTICAL_FACTOR < history_box_height) ||
       ((bottom_diff * SVP_UNIFORM_FACTOR_A < top_diff * SVP_UNIFORM_FACTOR_B) &&
        (top_diff * SVP_UNIFORM_FACTOR_A < bottom_diff * SVP_UNIFORM_FACTOR_B)) ||
       (((xmedia_s32)top_diff ^ (xmedia_s32)bottom_diff) < 0)) {
        cur_rect->y1 = cur_rect->y1 * SVP_STABLE_CURRENT_RATIO +
                    history_box->stable_targets[history_idx].rect.y1 * SVP_STABLE_HISTORY_RATIO;
        cur_rect->y2 = cur_rect->y2 * SVP_STABLE_CURRENT_RATIO +
                    history_box->stable_targets[history_idx].rect.y2 * SVP_STABLE_HISTORY_RATIO;
    }
}

static xmedia_void svp_stable_filter_rect(svp_stable_box *history_box, svp_stable_box *cur_box,
                                                    xmedia_float iou_threshold, xmedia_u32 history_box_num)
{
    xmedia_s32 i;
    xmedia_s32 idx = -1;

    for (i = 0;i < cur_box->num; i++) {
        xmedia_s32 id = -1;
        xmedia_svp_rect cur_rect = cur_box->stable_targets[i].rect;
        xmedia_svp_class_type class_type = cur_box->stable_targets[i].class_type;
        xmedia_float max_value = svp_find_max_overlap(*history_box, cur_rect, class_type, &id, history_box_num);

        if (max_value > SVP_IOU_STABLE_THRESH_UPPER) {
            cur_rect.x1 = cur_rect.x1 * SVP_STABLE_CURRENT_RATIO +
                         history_box->stable_targets[id].rect.x1 * SVP_STABLE_HISTORY_RATIO;
            cur_rect.x2 = cur_rect.x2 * SVP_STABLE_CURRENT_RATIO +
                         history_box->stable_targets[id].rect.x2 * SVP_STABLE_HISTORY_RATIO;
            cur_rect.y1 = cur_rect.y1 * SVP_STABLE_CURRENT_RATIO +
                         history_box->stable_targets[id].rect.y1 * SVP_STABLE_HISTORY_RATIO;
            cur_rect.y2 = cur_rect.y2 * SVP_STABLE_CURRENT_RATIO +
                         history_box->stable_targets[id].rect.y2 * SVP_STABLE_HISTORY_RATIO;
        } else if (max_value > SVP_IOU_STABLE_THRESH_MIDDLE) {
            svp_side_stable_box(history_box, id, &cur_rect, history_box_num);
        } else if (max_value > iou_threshold) {
            // do nothing
        } else {
            idx = (idx + 1) % history_box_num;
            history_box->stable_targets[idx].rect = cur_rect;
            history_box->stable_targets[idx].class_type = class_type;
            svp_clear_record_by_idx(history_box, idx, iou_threshold, history_box_num);
        }

        cur_box->stable_targets[i].rect = cur_rect;
    }
}

static xmedia_s32 svp_get_stable_result(svp_stable_box *history_box, xmedia_svp_yolov5_output* result,
                                                    yolov5_detect_param *param)
{
    xmedia_s32 i;
    svp_stable_box cur_box;

    if (result->target_num != 0) {
        cur_box.num = result->target_num;
        for (i = 0; i < result->target_num; i++) {
            cur_box.stable_targets[i].rect = result->targets[i].rect;
            cur_box.stable_targets[i].class_type = result->targets[i].class_type;
        }

        svp_stable_filter_rect(history_box, &cur_box, param->iou_threshold, param->max_target_num);
        for (i = 0; i < result->target_num; i++) {
            result->targets[i].rect = cur_box.stable_targets[i].rect;
        }
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 detect_init(yolov5_detect_param *param, const xmedia_svp_alg_type type, const xmedia_svp_modules *modules,
                            xmedia_s32 layer_num)
{
    xmedia_s32 i, ret = XMEDIA_FAILURE;

    if ((modules == XMEDIA_NULL) || (param == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector init get null ptr\n");
        return XMEDIA_FAILURE;
    }

    if (param->dup_flag == XMEDIA_FALSE) {
        ret = npu_load_model(modules, &param->model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu load model failed\n");
            return XMEDIA_FAILURE;
        }
    }

    /** set quanlize param */
    if (layer_num != param->model.output.num && param->aov_flag == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "output num is not %d.\n", layer_num);
        npu_unload_model(&param->model);
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < param->model.output.num; i++) {
        param->quanlize[i].scale = param->model.output.tensor[i].quant.scale;
        param->quanlize[i].zp = param->model.output.tensor[i].quant.zp;
    }

    if (type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        ret = init_sigmoid_dequantize_table(param);
    } else {
        ret = init_sigmoid_table(param);
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "init_sigmoid_table failed \n");
        return XMEDIA_FAILURE;
    }

    param->type = type;
    param->thres_desig = desigmoid(param->detect_threshold);
    param->output_num = 0;

    if (layer_num == LAYER_MIN_NUM) {
        for (i = 0; i < LAYER_MIN_NUM; i++) {
            param->feature[i] = (param->w / k_strides_3layer[i]) * ((param->h / k_strides_3layer[i]) + 3 - i);
            param->output_num += ANCHOR_NUM * param->feature[i];
        }
        // 直接使用最大采样倍数
        if (check_img_size(param->w, param->h, k_strides_3layer[LAYER_MIN_NUM - 1]) != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector input img_size error. w:%d h:%d\n", param->w, param->h);
            return XMEDIA_FAILURE;
        }
    } else if ((layer_num == LAYER_MAX_NUM) || (layer_num == LAYER_NUM_FOUR)) {
        // 下采样非整数时，按最大下采样的向上取整为基准
        xmedia_u32 ceil_w = (param->w + k_strides_4layer[LAYER_NUM_FOUR - 1] - 1) /
                          k_strides_4layer[LAYER_NUM_FOUR - 1];
        xmedia_u32 ceil_h = (param->h + k_strides_4layer[LAYER_NUM_FOUR - 1] - 1) /
                          k_strides_4layer[LAYER_NUM_FOUR - 1];
        ceil_w = ceil_w % 2 == 0 ? ceil_w : ceil_w + 1;
        ceil_h = ceil_h % 2 == 0 ? ceil_h : ceil_h + 1;
        for (i = 0; i < layer_num; i++) {
            param->feature[i] = ceil_w * (xmedia_u32)pow(2,LAYER_NUM_FOUR - 1 - (i % 4)) *
                                ceil_h * (xmedia_u32)pow(2,LAYER_NUM_FOUR - 1 - (i % 4));

            param->output_num += ANCHOR_NUM * param->feature[i];
        }
        if (check_img_size(param->w, param->h, k_strides_4layer[LAYER_NUM_FOUR - 1]) != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector input img_size error. w:%d h:%d\n", param->w, param->h);
            return XMEDIA_FAILURE;
        }
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "anchor layer num:%d invalid. w:%d h:%d\n", param->w, param->h, layer_num);
        return XMEDIA_FAILURE;
    }

    param->bbox = (svp_base_result*)calloc(1, sizeof(svp_base_result) * DEFAULT_BBOX_NUM);
    CHECK_CALLOC_RETURN_ERROR(param->bbox);

    *(param->model.user_count) += 1;

    return XMEDIA_SUCCESS;
}

xmedia_s32 detect_deinit(yolov5_detect_param *param)
{
    CFREE(param->bbox);
    CFREE(param->sigmoid_table);
    CFREE(param->sigmoid_dequantize_table);
    CFREE(param->sigmoid_dequantize_table_x2);
    CFREE(param->sigmoid_dequantize_table_x6);
    CFREE(param->sigmoid_dequantize_table_x2_sq);

    if (*(param->model.user_count) == 1) {
        *(param->model.user_count) -= 1;
        return npu_unload_model(&param->model);
    }

    SVP_TRACE(MODULE_DBG_DEBUG, "detect_deinit, detect_modle_user_count: %d\n", *(param->model.user_count));
    *(param->model.user_count) -= 1;

    return XMEDIA_SUCCESS;
}

xmedia_s32 detect_process(yolov5_detect_param *param, const xmedia_video_frame_info *frame_info,
                                xmedia_svp_yolov5_output *result, const xmedia_svp_detect_anchors anchors_yaml)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((param == XMEDIA_NULL) ||
        (result == XMEDIA_NULL) || (frame_info == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detect process err! frame:%p, result:%p\n", frame_info, result);
        return XMEDIA_FAILURE;
    }

    if ((frame_info->frame.addr.y_phy_addr == 0) ||
        (frame_info->frame.width != param->w) || (frame_info->frame.height != param->h)) {
        SVP_TRACE(MODULE_DBG_ERR, "picture size w:%d  h:%d \n",
                     frame_info->frame.width,
                     frame_info->frame.height);
        return XMEDIA_FAILURE;
    }

    ret = npu_run_model(&param->model, frame_info->frame.addr.y_phy_addr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu forward failed, ret=0x%x\n", ret);
        return XMEDIA_FAILURE;
    }
    if (param->aov_only_target == XMEDIA_FALSE) {
        if (param->aov_flag == XMEDIA_FALSE) {
            ret = svp_get_detect_result(param, result, anchors_yaml);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu process result failed, ret=0x%x\n", ret);
            }
        } else {
            ret = svp_get_aov_detect_result(param, result, anchors_yaml);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu process result failed, ret=0x%x\n", ret);
            }
        }
        ret = svp_get_stable_result(&param->record_result, result, param);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu process result failed, ret=0x%x\n", ret);
        }
    }
    return ret;
}
