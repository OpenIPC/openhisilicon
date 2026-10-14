#include "xm_vehicle_recognition.h"

static xmedia_svp_detect_anchors vehicle_anchors = {
    .anchors = {{{2.623046875, 2.466796875}, {4.77734375, 4.1484375}, {8.71875, 6.953125}},
               {{15.296875, 11.328125},      {26.03125, 19.984375},   {53.125, 33.90625}},
               {{101.0625, 63.125},          {217.0, 125.6875},       {236.75, 212.0}}},
    .layer_num = LAYER_MIN_NUM,
};

static xmedia_s32 vehicle_reco_init(const xmedia_svp_modules *modules, xmedia_npu_model *model)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret = npu_load_model(modules, model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "npu_load_model failed !\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 vehicle_reco_uninit(xmedia_npu_model *model)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret = npu_unload_model(model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "npu_unload_model failed !\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 base_check_attr(const xmedia_svp_yolov5_attr *task_attr)
{
    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_ENABLE(task_attr->bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");
    if (task_attr->max_target_num > XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM || task_attr->max_target_num <= 0) {
        SVP_TRACE(MODULE_DBG_ERR, "max_target_num[%d] out of range(0-%d] !!\n",
            task_attr->max_target_num, XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 base_set_attr(yolov5_detect_param *param, const xmedia_svp_yolov5_attr *task_attr)
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

static xmedia_s32 base_get_attr(yolov5_detect_param *param, xmedia_svp_yolov5_attr *task_attr)
{
    task_attr->detect_threshold = param->detect_threshold;
    task_attr->iou_threshold = param->iou_threshold;
    task_attr->max_target_num = param->max_target_num;
    task_attr->classifier_threshold = param->classifier_threshold;
    task_attr->bytetrack_enable = param->tracklet.enable;
    return XMEDIA_SUCCESS;
}

static inline xmedia_float sigmoid_table(xmedia_float* table, xmedia_float x)
{
    xmedia_u32 lower_index;
    xmedia_float index, fraction, lower_result, result;

    if ((x >= TABLE_MIN) && (x <= TABLE_MAX)) {
        index = (x + TABLE_MAX) * TABLE_SIZE / TABLE_200;
        lower_index = (xmedia_u32)index;
        fraction = index - lower_index;
        lower_result = *(table + lower_index);
        result = *(table + lower_index + 1);
        return (lower_result + fraction * (result - lower_result));
    }

    return (1.f / (1.f + exp(-x)));
}

static xmedia_bool vehicle_update_status_node(svp_vehicle_reco_info* vehicle_reco_info,
    svp_vehicle_status_node** cur_node, svp_vehicle_info *base_info)
{
    if (vehicle_reco_info == XMEDIA_NULL || vehicle_reco_info->head == XMEDIA_NULL) {
        return XMEDIA_FALSE;
    }

    svp_vehicle_status_node* cur = vehicle_reco_info->head;
    while (cur != XMEDIA_NULL) {
        if (cur->data.base_info.track_id == base_info->track_id) {
            *cur_node = cur;
            cur->data.loss_cnt = 0;
            cur->data.base_info.rect = base_info->rect;

            if ((cur->data.base_info.color != base_info->color) &&
                (cur->data.base_info.color == XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN)) {
                cur->data.base_info.color = base_info->color;
                cur->data.base_info.color_scr = base_info->color_scr;
            }

            if ((cur->data.base_info.type != base_info->type) &&
                (cur->data.base_info.type == XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN)) {
                cur->data.base_info.type = base_info->type;
                cur->data.base_info.type_scr = base_info->type_scr;
            }

            if (cur->data.base_info.color != XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN) {
                cur->data.exist_color_cnt++;
            } else {
                cur->data.exist_color_cnt = 0;
            }
            if (cur->data.base_info.type != XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN) {
                cur->data.exist_type_cnt++;
            } else {
                cur->data.exist_type_cnt = 0;
            }

            if ((cur->data.tmp_info.color == base_info->color) &&
                (base_info->color != XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN)) {
                cur->data.tmp_color_cnt++; // 待定颜色与当前帧颜色一致 累加
            } else {
                cur->data.tmp_info.color = base_info->color; // 不一致 则更新待定颜色
                cur->data.tmp_info.color_scr = base_info->color_scr;
            }

            if ((cur->data.tmp_info.type == base_info->type) &&
                ((base_info->type != XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN))) {
                cur->data.tmp_type_cnt++;
            } else {
                cur->data.tmp_info.type = base_info->type;
                cur->data.tmp_info.type_scr = base_info->type_scr;
            }

            if (cur->data.tmp_color_cnt > SVP_VEHICLE_STATUS_CHANGE_CNT) { // 待定超过阈值
                cur->data.base_info.color = cur->data.tmp_info.color; // 正式更新
                cur->data.base_info.color_scr = cur->data.tmp_info.color_scr;
                cur->data.tmp_color_cnt = 0;
                cur->data.exist_color_cnt = 0;
            }

            if (cur->data.tmp_type_cnt > SVP_VEHICLE_STATUS_CHANGE_CNT) {
                cur->data.base_info.type = cur->data.tmp_info.type;
                cur->data.base_info.type_scr = cur->data.tmp_info.type_scr;
                cur->data.tmp_type_cnt = 0;
                cur->data.exist_type_cnt = 0;
            }

            return XMEDIA_TRUE;
        }
        cur = cur->next;
    }

    return XMEDIA_FALSE;
}

// 创建新节点函数
static svp_vehicle_status_node* vehicle_create_status_node(svp_vehicle_info *base_info)
{
    svp_vehicle_status_node* new_node = (svp_vehicle_status_node*)malloc(sizeof(svp_vehicle_status_node));

    if (new_node == XMEDIA_NULL) {
        return XMEDIA_NULL;
    }

    // 显式初始化所有字段
    memcpy(&new_node->data.base_info, base_info, sizeof(svp_vehicle_info));
    new_node->data.loss_cnt   = 0;
    new_node->data.report_cnt = 0;
    new_node->data.exist_color_cnt  = 0;
    new_node->data.exist_type_cnt  = 0;
    new_node->data.is_report  = XMEDIA_FALSE;
    new_node->next            = XMEDIA_NULL;

    return new_node;
}

static xmedia_s32 vehicle_insert_status_node(svp_vehicle_reco_info* vehicle_reco_info,
    svp_vehicle_status_node** cur_node, svp_vehicle_info *base_info)
{
    if (vehicle_reco_info == XMEDIA_NULL || base_info == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    if (vehicle_update_status_node(vehicle_reco_info, cur_node, base_info) == XMEDIA_TRUE) {
        return XMEDIA_SUCCESS;
    }

    svp_vehicle_status_node* new_node = vehicle_create_status_node(base_info);
    if (new_node == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    if (vehicle_reco_info->head == XMEDIA_NULL) {
        vehicle_reco_info->head = new_node;
        vehicle_reco_info->tail = new_node;
    } else {
        vehicle_reco_info->tail->next = new_node;
        vehicle_reco_info->tail = new_node;
    }

    vehicle_reco_info->node_num++;
    *cur_node = new_node;

    return XMEDIA_SUCCESS;
}

static xmedia_void vehicle_delete_expired_nodes(svp_vehicle_reco_info* vehicle_reco_info)
{
    if (vehicle_reco_info == XMEDIA_NULL) {
        return;
    }

    svp_vehicle_status_node dummy;
    dummy.next = vehicle_reco_info->head;
    svp_vehicle_status_node* prev = &dummy;
    svp_vehicle_status_node* cur = vehicle_reco_info->head;

    while (cur != XMEDIA_NULL) {
        if ((cur->data.loss_cnt > SVP_MAX_LOST_COUNT && cur->data.is_report == XMEDIA_FALSE) ||
            (cur->data.is_report == XMEDIA_TRUE && cur->data.loss_cnt > SVP_MAX_LOST_COUNT &&
             cur->data.report_cnt > SVP_MAX_LOST_COUNT * 2)) {  // 上报后 消失且达到report_cnt阈值清空
            svp_vehicle_status_node* to_delete = cur;
            prev->next = cur->next;
            if (cur == vehicle_reco_info->tail) {
                vehicle_reco_info->tail = prev;
            }
            cur = cur->next;
            CFREE(to_delete);
            vehicle_reco_info->node_num--;
        } else {
            cur->data.loss_cnt++;
            if (cur->data.is_report == XMEDIA_TRUE) {
                cur->data.report_cnt++;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    vehicle_reco_info->head = dummy.next;

    if (vehicle_reco_info->head == XMEDIA_NULL) {
        vehicle_reco_info->tail = XMEDIA_NULL;
    }
}

static xmedia_void vehicle_delete_all_nodes(svp_vehicle_reco_info* vehicle_reco_info)
{
    if (vehicle_reco_info == XMEDIA_NULL) {
        return;
    }

    svp_vehicle_status_node* status_cur = vehicle_reco_info->head;

    while (status_cur != XMEDIA_NULL) {
        svp_vehicle_status_node* to_delete = status_cur;
        status_cur = status_cur->next;
        CFREE(to_delete);
    }

    vehicle_reco_info->head = XMEDIA_NULL;
    vehicle_reco_info->tail = XMEDIA_NULL;
    vehicle_reco_info->node_num = 0;
}

static xmedia_s32 vehicle_tag_reported_node(svp_vehicle_reco_info* vehicle_reco_info, xmedia_s32 track_id)
{
    if (vehicle_reco_info == XMEDIA_NULL || vehicle_reco_info->head == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    svp_vehicle_status_node* cur = vehicle_reco_info->head;
    while (cur != XMEDIA_NULL) {
        if (cur->data.base_info.track_id == track_id) {
            cur->data.is_report = XMEDIA_TRUE;
        }
        cur = cur->next;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 vehicle_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_vehicle_context *vehicle_ctx = (svp_vehicle_context *)context;
    if (vehicle_ctx == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "vehicle_recognition_init context err !\n");
        return XMEDIA_FAILURE;
    }

    if (cfg.module_num != 2) {
        SVP_TRACE(MODULE_DBG_ERR, "cfg.module_num != 2 err !\n");
        return XMEDIA_FAILURE;
    }

    // 车牌检测模型
    vehicle_ctx->info.detect_info = (svp_vehicle_detect_info *)calloc(1, sizeof(svp_vehicle_detect_info));
    CHECK_CALLOC_GOTO_ERROR(vehicle_ctx->info.detect_info, EXIT0);
    set_default_yolov5_detect_param(&vehicle_ctx->info.detect_info->param);
    vehicle_ctx->info.detect_info->param.w = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_ctx->info.detect_info->param.h = SVP_VEHICLE_DETECT_HIGHT;
    vehicle_ctx->info.detect_info->param.num = DETECTION_MODEL_SINGLE;
    vehicle_ctx->info.detect_info->param.anchors = vehicle_anchors;

    vehicle_ctx->info.detect_info->param.tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    if (vehicle_ctx->info.detect_info->param.tracklet.tracker == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "tracker calloc error !\n");
        goto EXIT1;
    }

    CHECK_SVP_COND_GOTO_ERROR(&cfg.modules[0] == XMEDIA_NULL, EXIT2, "vehicle detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((cfg.modules[0].load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(cfg.modules[0].pathname) != XMEDIA_SUCCESS), EXIT2, "vehicle  detect model pathname err!\n");
    ret = detect_init(&vehicle_ctx->info.detect_info->param, cfg.modules[0].alg_type,
        &cfg.modules[0], vehicle_ctx->info.detect_info->param.anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT2, "vehicle  detect init model failed !\n");

    xmedia_u64 vehicle_detect_addr = xmedia_mmz_alloc(XMEDIA_NULL, "VEHICLE_DETECT",
        SVP_VEHICLE_DETECT_WIDTH * SVP_VEHICLE_DETECT_HIGHT * 3 / 2);
    xmedia_video_frame_info *vehicle_frame = &vehicle_ctx->info.detect_info->detect_frame;
    vehicle_frame->mod_id = MOD_ID_USER;
    vehicle_frame->frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    vehicle_frame->frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
    vehicle_frame->frame.bit_width = XMEDIA_VIDEO_DATA_WIDTH_8;
    vehicle_frame->frame.width = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_frame->frame.height = SVP_VEHICLE_DETECT_HIGHT;
    vehicle_frame->frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    vehicle_frame->frame.stride.y_stride = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_frame->frame.stride.c_stride = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    vehicle_frame->frame.color_info.color_gamut = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    vehicle_frame->frame.addr.y_phy_addr = vehicle_detect_addr;
    vehicle_frame->frame.addr.c_phy_addr = vehicle_detect_addr + SVP_VEHICLE_DETECT_WIDTH * SVP_VEHICLE_DETECT_HIGHT;

    // 车辆识别模型
    vehicle_ctx->info.reco_info = (svp_vehicle_reco_info *)calloc(1, sizeof(svp_vehicle_reco_info));
    CHECK_CALLOC_GOTO_ERROR(vehicle_ctx->info.reco_info, EXIT3);
    CHECK_SVP_COND_GOTO_ERROR(&cfg.modules[1] == XMEDIA_NULL, EXIT4, "vehicle  reco model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((cfg.modules[1].load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(cfg.modules[1].pathname) != XMEDIA_SUCCESS), EXIT4, "vehicle  reco model pathname err!\n");

    vehicle_ctx->info.reco_info->reco_model = (xmedia_npu_model *)calloc(1, sizeof(xmedia_npu_model));
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT4, "reco_model calloc failed !\n");

    ret = vehicle_reco_init(&cfg.modules[1], vehicle_ctx->info.reco_info->reco_model);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT5, "vehicle  reco init model failed !\n");

    xmedia_video_frame_info *reco_frame = &vehicle_ctx->info.reco_info->reco_frame;
    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "vehicle_RECO", &reco_frame->frame.addr.y_phy_addr,
        &vehicle_ctx->info.reco_info->reco_viraddr, SVP_VEHICLE_RECO_WIDTH * SVP_VEHICLE_RECO_HEIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT5, "svp_mmz_alloc_and_map failed !\n");

    reco_frame->mod_id                          = MOD_ID_USER;
    reco_frame->frame.compress_mode             = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    reco_frame->frame.video_fmt                 = XMEDIA_VIDEO_FMT_LINEAR;
    reco_frame->frame.bit_width                 = XMEDIA_VIDEO_DATA_WIDTH_8;
    reco_frame->frame.width                     = SVP_VEHICLE_RECO_WIDTH;
    reco_frame->frame.height                    = SVP_VEHICLE_RECO_HEIGHT;
    reco_frame->frame.pixel_fmt                 = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    reco_frame->frame.stride.y_stride           = SVP_VEHICLE_RECO_WIDTH;
    reco_frame->frame.stride.c_stride           = SVP_VEHICLE_RECO_WIDTH;
    reco_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    reco_frame->frame.color_info.color_gamut    = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    reco_frame->frame.addr.c_phy_addr           = reco_frame->frame.addr.y_phy_addr +
                                                  SVP_VEHICLE_RECO_WIDTH * SVP_VEHICLE_RECO_HEIGHT;

    return XMEDIA_SUCCESS;

EXIT5:
    CFREE(vehicle_ctx->info.reco_info->reco_model);
EXIT4:
    CFREE(vehicle_ctx->info.reco_info);
EXIT3:
    ret = detect_deinit(&vehicle_ctx->info.detect_info->param);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "vehicle  detect_deinit failed %#x!\n", ret);
    }
EXIT2:
    CFREE(vehicle_ctx->info.detect_info->param.tracklet.tracker);
EXIT1:
    CFREE(vehicle_ctx->info.detect_info);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 vehicle_recognition_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    CHECK_SVP_COND_RETURN_ERROR(context == XMEDIA_NULL, "vehicle_recognition_uninit context err !\n");
    svp_vehicle_context *vehicle_ctx = (svp_vehicle_context *)context;

    if (vehicle_ctx->info.reco_info != XMEDIA_NULL) {
        vehicle_delete_all_nodes(vehicle_ctx->info.reco_info);

        if (vehicle_ctx->info.reco_info->reco_frame.frame.addr.y_phy_addr != XMEDIA_NULL) {
            svp_mmz_unmap_and_free(vehicle_ctx->info.reco_info->reco_frame.frame.addr.y_phy_addr,
                vehicle_ctx->info.reco_info->reco_viraddr);
        }

        ret = vehicle_reco_uninit(vehicle_ctx->info.reco_info->reco_model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "vehicle_reco_uninit failed %#x!\n", ret);
        }
        CFREE(vehicle_ctx->info.reco_info->reco_model);
        CFREE(vehicle_ctx->info.reco_info);
    }

    if (vehicle_ctx->info.detect_info != XMEDIA_NULL) {
        if (vehicle_ctx->info.detect_info->detect_frame.frame.addr.y_phy_addr != XMEDIA_NULL) {
            xmedia_mmz_free(vehicle_ctx->info.detect_info->detect_frame.frame.addr.y_phy_addr);
        }

        ret = detect_deinit(&vehicle_ctx->info.detect_info->param);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
        }
        CFREE(vehicle_ctx->info.detect_info->param.tracklet.tracker);
        CFREE(vehicle_ctx->info.detect_info);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 vehicle_recognition_set_attr(xmedia_void *context, const xmedia_svp_vehicle_reco_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_vehicle_context* vehicle_ctx = (svp_vehicle_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(vehicle_ctx == XMEDIA_NULL, "adas_set_attr context err !\n");

    ret = base_set_attr(&vehicle_ctx->info.detect_info->param, &task_attr->detect_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "vehicle  detect_set_attr failed %#x!\n", ret);

    vehicle_ctx->info.report_count = task_attr->report_count;
    vehicle_ctx->info.report_thres = task_attr->report_thres;
    memcpy(&vehicle_ctx->info.detect_zone, &task_attr->detect_zone, sizeof(xmedia_svp_zone));

    return XMEDIA_SUCCESS;
}

xmedia_s32 vehicle_recognition_get_attr(xmedia_void *context, xmedia_svp_vehicle_reco_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_vehicle_context* vehicle_ctx = (svp_vehicle_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(vehicle_ctx == XMEDIA_NULL, "adas_set_attr context err !\n");

    ret = base_get_attr(&vehicle_ctx->info.detect_info->param, &task_attr->detect_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "vehicle  detect_set_attr failed %#x!\n", ret);

    task_attr->report_count = vehicle_ctx->info.report_count;
    task_attr->report_thres = vehicle_ctx->info.report_thres;
    memcpy(&task_attr->detect_zone, &vehicle_ctx->info.detect_zone, sizeof(xmedia_svp_zone));

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_get_vehicle_reco_result_bz(xmedia_npu_model *model, svp_vehicle_info *base_info)
{
    xmedia_u8 *addr;
    xmedia_s32 i;
    xmedia_float scale;
    xmedia_s32 zp;
    xmedia_u32 type_idx, color_idx;
    xmedia_float max_value;

    xmedia_float *type_score = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_VEHICLE_TYPE_CLASS);
    CHECK_CALLOC_GOTO_ERROR(type_score, EXIT0);
    xmedia_float *color_score = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_VEHICLE_COLOR_CLASS);
    CHECK_CALLOC_GOTO_ERROR(color_score, EXIT1);
    xmedia_float *type_arr = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_VEHICLE_TYPE_CLASS);
    CHECK_CALLOC_GOTO_ERROR(type_arr, EXIT2);
    xmedia_float *color_arr = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_VEHICLE_TYPE_CLASS);
    CHECK_CALLOC_GOTO_ERROR(color_arr, EXIT3);

    addr  = model->output.tensor[0].addr;
    scale = model->output.tensor[0].quant.scale;
    zp    = model->output.tensor[0].quant.zp;

    max_value = dequantize(addr[0], scale, zp);
    type_idx = 0;
    for (i = 1; i < model->output.tensor[0].size; i++) {
        type_arr[i] = dequantize(addr[i], scale, zp);
        if (type_arr[i] > max_value) {
            type_idx = i;
        }
    }
    softmax(type_arr, SVP_VEHICLE_TYPE_CLASS, type_score);

    if (type_score[type_idx] >= SVP_VEHICLE_TYPE_THRES) {
        base_info->type     = (xmedia_svp_vehicle_type)type_idx;
        base_info->type_scr = type_score[type_idx];
    } else {
        base_info->type     = XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN;
        base_info->type_scr = 0.0f;
    }

    addr  = model->output.tensor[1].addr;
    scale = model->output.tensor[1].quant.scale;
    zp    = model->output.tensor[1].quant.zp;

    max_value = dequantize(addr[0], scale, zp);
    color_idx = 0;
    for (i = 1; i < model->output.tensor[1].size; i++) {
        color_arr[i] = dequantize(addr[i], scale, zp);
        if (color_arr[i] > max_value) {
            color_idx = i;
        }
    }
    softmax(color_arr, SVP_VEHICLE_COLOR_CLASS, color_score);

    if (color_score[color_idx] > SVP_VEHICLE_COLOR_THRES) {
        base_info->color     = (xmedia_svp_vehicle_color)color_idx;
        base_info->color_scr = color_score[color_idx];
    } else {
        base_info->color     = XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN;
        base_info->color_scr = 0.0f;
    }

    CFREE(type_score);
    CFREE(color_score);
    CFREE(type_arr);
    CFREE(color_arr);

    return XMEDIA_SUCCESS;

EXIT3:
    CFREE(type_arr);
EXIT2:
    CFREE(color_score);
EXIT1:
    CFREE(type_score);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 vehicle_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_vehicle_reco_output *output)
{
    CHECK_SVP_COND_RETURN_ERROR(context == XMEDIA_NULL, "vehicle_recognition_process context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(output == XMEDIA_NULL, "vehicle_recognition_process output err !\n");
    CHECK_SVP_COND_RETURN_ERROR(input == XMEDIA_NULL, "vehicle_recognition_process input err !\n");

    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 i;
    svp_vehicle_context *vehicle_ctx;
    xmedia_svp_rect std_rect;
    xmedia_s32 zone_index;
    xmedia_float scale_x, scale_y;
    xmedia_float pre_rect, cur_rect;
    xmedia_video_frame_info input_frame;
    xmedia_u32 x1, x2, y1, y2;
    xmedia_svp_yolov8_output detect_output;
    xmedia_video_frame_info *big_frame;
    xmedia_video_frame_info *detect_frame;
    xmedia_video_frame_info *reco_frame;
    xmedia_svp_tracker_result result = {0};
    xmedia_svp_rect base_rect;
    svp_vehicle_info base_info;
    svp_vehicle_status_node *cur_node;

    vehicle_ctx = (svp_vehicle_context *)context;
    if (vehicle_ctx == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "vehicle_recognition_init context err !\n");
        return XMEDIA_FAILURE;
    }

    big_frame = input->frame + 1;
    detect_frame = &vehicle_ctx->info.detect_info->detect_frame;
    reco_frame = &vehicle_ctx->info.reco_info->reco_frame;
    CHECK_SVP_COND_RETURN_ERROR((big_frame == XMEDIA_NULL) || (reco_frame == XMEDIA_NULL), "input frame is NULL!\n");

    ret = svp_vgs_resize(big_frame, detect_frame);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize failed!\n");

    ret = npu_run_model(&vehicle_ctx->info.detect_info->param.model, detect_frame->frame.addr.y_phy_addr);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vehicle  npu_run_model failed!\n");

    ret = svp_get_detect_result_bz(&vehicle_ctx->info.detect_info->param, &detect_output, vehicle_anchors);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_detect_result_yolov5_bz failed!\n");

    if (vehicle_ctx->info.detect_info->param.tracklet.enable == XMEDIA_TRUE) {
        result.target_num = detect_output.target_num;
        for(i = 0; i < result.target_num; i++) {
            if (i > XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM) {
                break;
            }
            result.tracker_single[i].detect_score    = detect_output.targets[i].detect_score;
            result.tracker_single[i].classfier_score = detect_output.targets[i].classfier_score;
            result.tracker_single[i].rect            = detect_output.targets[i].rect;
        }
        ret = svp_detect_bytetracker(&result,
                                     &vehicle_ctx->info.detect_info->param.tracklet,
                                     &vehicle_ctx->info.detect_info->param.tarck_id_grow,
                                     vehicle_ctx->info.detect_info->param.tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }

        memset(&detect_output, 0, sizeof(xmedia_svp_yolov8_output));
        for(i = 0; i < result.target_num; i++) {
            if (i > XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM) {
                break;
            }
            detect_output.targets[detect_output.target_num].detect_score    = result.tracker_single[i].detect_score;
            detect_output.targets[detect_output.target_num].classfier_score = result.tracker_single[i].classfier_score;
            detect_output.targets[detect_output.target_num].tracker_id      = result.tracker_single[i].tracker_id;
            detect_output.targets[detect_output.target_num].tracker_age     = result.tracker_single[i].tracker_age;
            detect_output.targets[detect_output.target_num].rect            = result.tracker_single[i].rect;
            detect_output.target_num++;
        }
    }

    vehicle_delete_expired_nodes(vehicle_ctx->info.reco_info);

    output->target_num = 0;
    for (i = 0; i < detect_output.target_num; i++) {
        if (output->target_num > XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM) {
            break;
        }

        // 返回原坐标 1080P
        scale_x = (xmedia_float)big_frame->frame.width / SVP_VEHICLE_DETECT_WIDTH;
        scale_y = (xmedia_float)big_frame->frame.height / SVP_VEHICLE_DETECT_HIGHT;
        if (vehicle_ctx->info.detect_zone.zone_num != 0) { // 未设置区域默认全屏检测
            std_rect.x1 = detect_output.targets[i].rect.x1 * scale_x;
            std_rect.x2 = detect_output.targets[i].rect.x2 * scale_x;
            std_rect.y1 = detect_output.targets[i].rect.y1 * scale_y;
            std_rect.y2 = detect_output.targets[i].rect.y2 * scale_y;
            ret = clip_rect_to_frame(&std_rect, big_frame->frame.width, big_frame->frame.height);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "clip_rect_to_frame failed!\n");

            zone_index = svp_detect_zone(&vehicle_ctx->info.detect_zone, &std_rect,
                big_frame->frame.width, big_frame->frame.height);
            if (zone_index == -1) { // 不在任何一个区域内
                continue;
            }
        }

        base_rect.x1 = scale_x * detect_output.targets[i].rect.x1;
        base_rect.x2 = scale_x * detect_output.targets[i].rect.x2;
        base_rect.y1 = scale_y * detect_output.targets[i].rect.y1;
        base_rect.y2 = scale_y * detect_output.targets[i].rect.y2;

        x1 = (xmedia_s32)base_rect.x1 / 2 * 2;
        x2 = (xmedia_s32)base_rect.x2 / 2 * 2;
        y1 = (xmedia_s32)base_rect.y1 / 2 * 2;
        y2 = (xmedia_s32)base_rect.y2 / 2 * 2;
        memcpy(&input_frame, big_frame, sizeof(xmedia_video_frame_info));
        input_frame.frame.addr.y_phy_addr  =
            big_frame->frame.addr.y_phy_addr + x1 + big_frame->frame.stride.y_stride * y1;
        input_frame.frame.addr.c_phy_addr  =
            big_frame->frame.addr.c_phy_addr + x1 + big_frame->frame.stride.c_stride * y1 / 2;
        input_frame.frame.width            = (xmedia_u32)abs(x2 - x1) / 2 * 2;
        input_frame.frame.height           = (xmedia_u32)abs(y2 - y1) / 2 * 2;
        input_frame.frame.stride.y_stride  = big_frame->frame.stride.y_stride;
        input_frame.frame.stride.c_stride  = big_frame->frame.stride.c_stride;

        ret = svp_vgs_resize(&input_frame, reco_frame);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize failed!\n");

        ret = npu_run_model(vehicle_ctx->info.reco_info->reco_model, reco_frame->frame.addr.y_phy_addr);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vehicle reco npu_run_model failed!\n");
        ret = svp_get_vehicle_reco_result_bz(vehicle_ctx->info.reco_info->reco_model, &base_info);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_reco_result_yolov5_bz failed!\n");

        base_info.rect     = base_rect;
        base_info.track_id = detect_output.targets[i].tracker_id;
        if (base_info.color_scr < vehicle_ctx->info.report_thres &&
            base_info.type_scr < vehicle_ctx->info.report_thres) {
            continue;
        }

        ret = vehicle_insert_status_node(vehicle_ctx->info.reco_info, &cur_node, &base_info);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vehicle_insert_status_node failed!\n");
        if (cur_node->data.is_report != XMEDIA_TRUE) {
            if (cur_node->data.exist_color_cnt > vehicle_ctx->info.report_count &&
                cur_node->data.exist_type_cnt > vehicle_ctx->info.report_count) {

                pre_rect = fabs(cur_node->data.base_info.rect.x2 - cur_node->data.base_info.rect.x1) *
                           fabs(cur_node->data.base_info.rect.y2 - cur_node->data.base_info.rect.y1);
                cur_rect = fabs(detect_output.targets[i].rect.x2 - detect_output.targets[i].rect.x1) *
                           fabs(detect_output.targets[i].rect.y2 - detect_output.targets[i].rect.y1);
                if (pre_rect < cur_rect) {
                    output->targets[output->target_num].vehicle_direction = XMEDIA_SVP_DIRECTION_FORWARD;
                } else if (pre_rect > cur_rect) {
                    output->targets[output->target_num].vehicle_direction = XMEDIA_SVP_DIRECTION_BACKWARD;
                } else {
                    output->targets[output->target_num].vehicle_direction = XMEDIA_SVP_DIRECTION_UNKOWN;
                }

                output->targets[output->target_num].color      = cur_node->data.base_info.color;
                output->targets[output->target_num].color_scr  = cur_node->data.base_info.color_scr;
                output->targets[output->target_num].type       = cur_node->data.base_info.type;
                output->targets[output->target_num].type_scr   = cur_node->data.base_info.type_scr;
                output->targets[output->target_num].tracker_id = cur_node->data.base_info.track_id;
                output->targets[output->target_num].rect       = cur_node->data.base_info.rect;
                output->target_num++;
            }
            if (cur_node->data.exist_color_cnt > vehicle_ctx->info.report_count &&
                cur_node->data.exist_type_cnt > vehicle_ctx->info.report_count) {
                ret = vehicle_tag_reported_node(vehicle_ctx->info.reco_info, cur_node->data.base_info.track_id);
                CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vehicle_tag_reported_node failed!\n");
            }
        }
    }

    return XMEDIA_SUCCESS;
EXIT:
    return XMEDIA_FAILURE;
}

