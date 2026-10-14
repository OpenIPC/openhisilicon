#include "xm_plate_vehicle_reco.h"

static xmedia_svp_detect_anchors vehicle_anchors = {
    .anchors = {{{2.623046875, 2.466796875}, {4.77734375, 4.1484375}, {8.71875, 6.953125}},
               {{15.296875, 11.328125},      {26.03125, 19.984375},   {53.125, 33.90625}},
               {{101.0625, 63.125},          {217.0, 125.6875},       {236.75, 212.0}}},
    .layer_num = LAYER_MIN_NUM,
};

static xmedia_svp_detect_anchors plate_anchors = {
    .anchors = {{{4,5},    {8,10},    {13,16}},
               {{23,29},   {43,55},   {73,105}},
               {{146,217}, {231,300}, {335,433}}},
    .layer_num = LAYER_MIN_NUM,
};

static svp_plate_vehicle_keypoint reco_keypoints = {
    .point_arry = {{15.9, 3.9},
                   {176.1, 3.9},
                   {176.1, 44.1},
                   {15.9, 44.1}},
};

static const xmedia_char* g_character[] = {
    "#", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M",
    "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "桂", "贵", "冀", "吉", "京", "琼", "陕", "苏", "湘", "渝",
    "豫", "藏", "川", "鄂", "甘", "赣", "黑", "沪", "津", "晋",
    "鲁", "蒙", "闽", "宁", "青", "挂", "皖", "新", "粤", "云",
    "浙", "辽", "学", "警", "领", "使", "澳", "港"
};

#if 0
#include <sys/time.h>
static struct timeval plate_stTimeCostStart;
static struct timeval plate_stTimeCostEnd;

#define PLATE_TIME_COST_START()   do {    \
    gettimeofday(&plate_stTimeCostStart,NULL); \
} while(0)

#define PLATE_TIME_COST_END()   do {    \
        gettimeofday(&plate_stTimeCostEnd,NULL); \
} while(0)

#define PLATE_TIME_COST_PRINT(info)   do { \
    xmedia_u64 u64TimeUseMs = (plate_stTimeCostEnd.tv_sec - plate_stTimeCostStart.tv_sec) * 1000000 \
                      + (plate_stTimeCostEnd.tv_usec - plate_stTimeCostStart.tv_usec);   \
    printf("%s cost time:%llu us\n",info,u64TimeUseMs);  \
} while(0)
#endif

static xmedia_s32 plate_vehicle_reco_init(const xmedia_svp_modules *modules, xmedia_npu_model *model)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret = npu_load_model(modules, model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "npu_load_model failed !\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 plate_vehicle_reco_uninit(xmedia_npu_model *model)
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

static xmedia_s32 decode_char(xmedia_s32 *max_value, xmedia_float *score, svp_plate_vehicle_char_vec *char_vec,
    xmedia_char *res, xmedia_s32 max_length)
{
    for (xmedia_s32 i = 0; i < max_length; i++) {
        if (max_value[i] != 0 && !(i > 0 && max_value[i - 1] == max_value[i])) {
            if (char_vec->num >= max_length) {
                break;
            }
            char_vec->char_arr[char_vec->num].char_score = score[i];
            char_vec->char_arr[char_vec->num].index      = max_value[i];
            char_vec->num++;
            strncat(res, g_character[max_value[i]], max_length - strlen(res) - 1);
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_bool new_energy_rule(svp_plate_vehicle_char_vec *chars_vec)
{
    if (chars_vec->num != SVP_PLATE_CHAR_MAX) {
        return XMEDIA_FALSE;
    }
    // 判断是否为武警车牌WJ
    if (chars_vec->char_arr[0].index == 31 && chars_vec->char_arr[1].index == 19) {
        return XMEDIA_TRUE;
    }
    // 第一个字符必须为汉字
    if (chars_vec->char_arr[0].index < 35) {
        return XMEDIA_FALSE;
    }
    // 第二个字符必须为字母
    if (chars_vec->char_arr[1].index < 11 || chars_vec->char_arr[1].index > 34) {
        return XMEDIA_FALSE;
    }

    // 索引和g_character中一样
    if (chars_vec->char_arr[2].index >= 11 && chars_vec->char_arr[2].index <= 34) {
        // 小型新能源车牌  第3位是英文字母，第3位可以为英文字母，其他位是阿拉伯数字 川A D12345
        if (chars_vec->char_arr[4].index > 10 || chars_vec->char_arr[5].index > 10 ||
            chars_vec->char_arr[6].index > 10 || chars_vec->char_arr[7].index > 10) {
            return XMEDIA_FALSE;
        }
    } else if (chars_vec->char_arr[7].index >= 11 && chars_vec->char_arr[7].index <= 34) {
        // 大型新能源车牌  第8位是英文字母，其他位是阿拉伯数字 川A 12345D
        if (chars_vec->char_arr[2].index > 10 || chars_vec->char_arr[3].index > 10 ||
            chars_vec->char_arr[4].index > 10 || chars_vec->char_arr[5].index > 10 ||
            chars_vec->char_arr[6].index > 10) {
            return XMEDIA_FALSE;
        }
    } else {
        return XMEDIA_FALSE;
    }

    return XMEDIA_TRUE;
}

static xmedia_bool normal_rule(svp_plate_vehicle_char_vec *chars_vec)
{
    if (chars_vec->num != SVP_PLATE_CHAR_MIN) {
        return XMEDIA_FALSE;
    }
    // 第一个字符必须为汉字
    if (chars_vec->char_arr[0].index < 35) {
        return XMEDIA_FALSE;
    }
    // 第二个字符必须为字母
    if (chars_vec->char_arr[1].index < 11 || chars_vec->char_arr[1].index > 34) {
        return XMEDIA_FALSE;
    }
    return XMEDIA_TRUE;
}

static xmedia_s32 svp_plate_vehicle_plate_char_result_bz(xmedia_npu_model *model,
    svp_plate_vehicle_char_vec *chars_vec, xmedia_char *res, xmedia_float *total_score)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u8 *addr;
    xmedia_s32 i, j;
    xmedia_float max_value;
    xmedia_float scale;
    xmedia_s32 zp;

    xmedia_s32 *max_idx = (xmedia_s32 *)calloc(1, sizeof(xmedia_s32) * SVP_PLATE_T_STRIDE);
    CHECK_CALLOC_RETURN_ERROR(max_idx);
    xmedia_float *score_arr = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_PLATE_T_STRIDE);
    CHECK_CALLOC_RETURN_ERROR(score_arr);
    xmedia_float *score_tmp = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_PLATE_STR_NUM);
    CHECK_CALLOC_RETURN_ERROR(score_tmp);
    xmedia_float * dequan_arr = (xmedia_float *)calloc(1, sizeof(xmedia_float) * SVP_PLATE_STR_NUM);
    CHECK_CALLOC_RETURN_ERROR(dequan_arr);

    addr  = model->output.tensor[0].addr;
    scale = model->output.tensor[0].quant.scale;
    zp    = model->output.tensor[0].quant.zp;

    for (i = 0; i < SVP_PLATE_T_STRIDE; i++) {
        xmedia_s32 index = 0;
        max_value = dequantize(addr[i], scale, zp);
        for (j = 0; j < SVP_PLATE_STR_NUM; j++) {
            dequan_arr[j] = dequantize(addr[i + SVP_PLATE_T_STRIDE * j], scale, zp);
            if (dequan_arr[j] > max_value) {
                index = j;
                max_value = dequan_arr[j];
            }
        }
        max_idx[i] = index;

        softmax(dequan_arr, SVP_PLATE_STR_NUM, score_tmp);
        score_arr[i] = score_tmp[index];
    }

    ret = decode_char(max_idx, score_arr, chars_vec, res, SVP_PLATE_T_STRIDE);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate decode_char failed %#x!\n", ret);

    for (i = 0; i < SVP_PLATE_T_STRIDE; i++) {
        *total_score += score_arr[i];
    }

    *total_score = *total_score / SVP_PLATE_T_STRIDE;

    CFREE(score_tmp);
    CFREE(score_arr);
    CFREE(max_idx);
    CFREE(dequan_arr);

    return XMEDIA_SUCCESS;

EXIT:
    CFREE(score_tmp);
    CFREE(score_arr);
    CFREE(max_idx);
    CFREE(dequan_arr);

    return XMEDIA_FAILURE;
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

static xmedia_s32 svp_get_platecolor_result_bz(xmedia_npu_model *model, xmedia_float *table,
    xmedia_svp_plate_color *color, xmedia_float *color_score)
{
    xmedia_u8 *addr;
    xmedia_s32 i;
    xmedia_float scale;
    xmedia_s32 zp;
    xmedia_s32 max_index = 0;
    xmedia_float max_value = 0.0f;

    addr  = model->output.tensor[0].addr;
    scale = model->output.tensor[0].quant.scale;
    zp    = model->output.tensor[0].quant.zp;

    for (i = 0; i < model->output.tensor[0].size; i++) {
        xmedia_float cur_value = sigmoid_table(table, dequantize(addr[i], scale, zp));
        if (cur_value > max_value) {
            max_value = cur_value;
            max_index = i;
        }
    }

    if (max_value >= SVP_VEHICLE_COLOR_THRES) {
        *color = (xmedia_svp_plate_color)max_index;
        *color_score = max_value;
    } else {
        *color = XMEDIA_SVP_PLATE_COLOR_UNKNOWN;
        *color_score = 0.0f;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 svp_get_vehicle_typecolor_result_bz(xmedia_npu_model *model, svp_plate_vehicle_info *base_info)
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
        base_info->veh_type       = (xmedia_svp_vehicle_type)type_idx;
        base_info->veh_type_score = type_score[type_idx];
    } else {
        base_info->veh_type       = XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN;
        base_info->veh_type_score = 0.0f;
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
        base_info->veh_color       = (xmedia_svp_vehicle_color)color_idx;
        base_info->veh_color_score = color_score[color_idx];
    } else {
        base_info->veh_color       = XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN;
        base_info->veh_color_score = 0.0f;
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

static xmedia_s32 plate_vehicle_tag_reported_node(svp_plate_vehicle_information* plate_vehicle_info,
    xmedia_s32 track_id)
{
    if (plate_vehicle_info == XMEDIA_NULL || plate_vehicle_info->head == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    svp_plate_vehicle_status_node* cur = plate_vehicle_info->head;
    while (cur != XMEDIA_NULL) {
        if (cur->data.base_info.track_id == track_id) {
            cur->data.is_report = XMEDIA_TRUE;
        }
        cur = cur->next;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_bool plate_vehicle_update_status(svp_plate_vehicle_information* plate_vehicle_info,
    svp_plate_vehicle_status_node** cur_node, svp_plate_vehicle_info *base_info)
{
    if (plate_vehicle_info == XMEDIA_NULL || plate_vehicle_info->head == XMEDIA_NULL) {
        return XMEDIA_FALSE;
    }

    svp_plate_vehicle_status_node* cur = plate_vehicle_info->head;
    while (cur != XMEDIA_NULL) {
        if ((cur->data.base_info.track_id == base_info->track_id) && (cur->data.is_report == XMEDIA_TRUE)) {
            *cur_node = cur;
            return XMEDIA_TRUE; // 此id已上报过车牌
        }
         // 比较字符串 不包括串首中文字符
        if (!strncmp(base_info->reco_char + 3, cur->data.base_info.reco_char + 3, SVP_PLATE_T_STRIDE - 3)) {
            *cur_node = cur;
            cur->data.loss_cnt = 0;
            if (base_info->char_score - cur->data.base_info.char_score > 0.03) { // 分数差值大于0.03 更新数据
                memcpy(cur->data.base_info.reco_char, base_info->reco_char,
                    sizeof(xmedia_char) * XMEDIA_SVP_PLATE_CHAR_MAX_NUM);
            }

            if (base_info->plate_color_score - cur->data.base_info.plate_color_score > 0.03) { // 分数差值大于0.03
                cur->data.base_info.plate_color = base_info->plate_color;
                cur->data.base_info.plate_color_score = base_info->plate_color_score;
            }

            if (cur->data.is_report == XMEDIA_FALSE) {
                cur->data.exist_cnt++;
            }
            if ((cur->data.base_info.veh_color != base_info->veh_color) &&
                (cur->data.base_info.veh_color == XMEDIA_SVP_VEHICLE_COLOR_UNKNOWN)) {
                cur->data.base_info.veh_color = base_info->veh_color;
                cur->data.base_info.veh_color_score = base_info->veh_color_score;
            }
            if ((cur->data.base_info.veh_type != base_info->veh_type) &&
                (cur->data.base_info.veh_type == XMEDIA_SVP_VEHICLE_TYPE_UNKNOWN)) {
                cur->data.base_info.veh_type = base_info->veh_type;
                cur->data.base_info.veh_type_score = base_info->veh_type_score;
            }
            return XMEDIA_TRUE;
        }
        cur = cur->next;
    }

    return XMEDIA_FALSE;
}

// 创建新节点函数
static svp_plate_vehicle_status_node* plate_vehicle_create_status_node(svp_plate_vehicle_info *base_info)
{
    svp_plate_vehicle_status_node* new_node =
        (svp_plate_vehicle_status_node*)malloc(sizeof(svp_plate_vehicle_status_node));

    if (new_node == XMEDIA_NULL) {
        return XMEDIA_NULL;
    }

    // 显式初始化所有字段
    snprintf(new_node->data.base_info.reco_char, SVP_PLATE_T_STRIDE, "%s", base_info->reco_char);
    new_node->data.base_info.char_score        = base_info->char_score;
    new_node->data.base_info.track_id          = base_info->track_id;
    new_node->data.base_info.veh_rect          = base_info->veh_rect;
    new_node->data.base_info.veh_color         = base_info->veh_color;
    new_node->data.base_info.veh_color_score   = base_info->veh_color_score;
    new_node->data.base_info.veh_type          = base_info->veh_type;
    new_node->data.base_info.veh_type_score    = base_info->veh_type_score;
    new_node->data.base_info.plate_color       = base_info->plate_color;
    new_node->data.base_info.plate_color_score = base_info->plate_color_score;
    new_node->data.base_info.plate_rect        = base_info->plate_rect;

    new_node->data.loss_cnt   = 0;
    new_node->data.report_cnt = 0;
    new_node->data.exist_cnt  = 1;
    new_node->data.is_report  = XMEDIA_FALSE;
    new_node->next            = XMEDIA_NULL;

    return new_node;
}

static xmedia_s32 plate_vehicle_insert_status_node(svp_plate_vehicle_information* plate_vehicle_info,
    svp_plate_vehicle_status_node** cur_node, svp_plate_vehicle_info *base_info)
{
    if (plate_vehicle_info == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    if (plate_vehicle_update_status(plate_vehicle_info, cur_node, base_info) == XMEDIA_TRUE) {
        return XMEDIA_SUCCESS;
    }

    svp_plate_vehicle_status_node* new_node = plate_vehicle_create_status_node(base_info);
    if (new_node == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    if (plate_vehicle_info->head == XMEDIA_NULL) {
        plate_vehicle_info->head = new_node;
        plate_vehicle_info->tail = new_node;
    } else {
        plate_vehicle_info->tail->next = new_node;
        plate_vehicle_info->tail = new_node;
    }

    plate_vehicle_info->node_num++;
    *cur_node = new_node;

    return XMEDIA_SUCCESS;
}

static xmedia_void plate_vehicle_delete_expired_nodes(svp_plate_vehicle_information* plate_vehicle_info)
{
    if (plate_vehicle_info == XMEDIA_NULL) {
        return;
    }

    svp_plate_vehicle_status_node dummy;
    dummy.next = plate_vehicle_info->head;
    svp_plate_vehicle_status_node* prev = &dummy;
    svp_plate_vehicle_status_node* cur = plate_vehicle_info->head;

    while (cur != XMEDIA_NULL) {
        if ((cur->data.loss_cnt > SVP_MAX_LOST_COUNT && cur->data.is_report == XMEDIA_FALSE) ||
            (cur->data.is_report == XMEDIA_TRUE && cur->data.loss_cnt > SVP_MAX_LOST_COUNT &&
             cur->data.report_cnt > SVP_PLATE_REPORT_TIME)) {  // 上报后 消失且达到report_cnt阈值清空
            svp_plate_vehicle_status_node* to_delete = cur;
            prev->next = cur->next;
            if (cur == plate_vehicle_info->tail) {
                plate_vehicle_info->tail = prev;
            }
            cur = cur->next;
            CFREE(to_delete);
            plate_vehicle_info->node_num--;
        } else {
            cur->data.loss_cnt++;
            if (cur->data.is_report == XMEDIA_TRUE) {
                cur->data.report_cnt++;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    plate_vehicle_info->head = dummy.next;

    if (plate_vehicle_info->head == XMEDIA_NULL) {
        plate_vehicle_info->tail = XMEDIA_NULL;
    }
}

static xmedia_void plate_vehicle_delete_all_nodes(svp_plate_vehicle_information* plate_vehicle_info)
{
    if (plate_vehicle_info == XMEDIA_NULL) {
        return;
    }

    svp_plate_vehicle_status_node* status_cur = plate_vehicle_info->head;

    while (status_cur != XMEDIA_NULL) {
        svp_plate_vehicle_status_node* to_delete = status_cur;
        status_cur = status_cur->next;
        CFREE(to_delete);
    }

    plate_vehicle_info->head = XMEDIA_NULL;
    plate_vehicle_info->tail = XMEDIA_NULL;
    plate_vehicle_info->node_num = 0;
}

xmedia_s32 plate_vehicle_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_plate_vehicle_context *pv_ctx = (svp_plate_vehicle_context *)context;
    if (pv_ctx == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_recognition_init context err !\n");
        return XMEDIA_FAILURE;
    }

    if (cfg.module_num != 5) { // 车牌车辆 5个模型
        SVP_TRACE(MODULE_DBG_ERR, "detect model num err!\n");
        return XMEDIA_FAILURE;
    }

    // 车牌检测模型
    pv_ctx->info.plate_detect =
        (svp_plate_vehicle_detect_info *)calloc(1, sizeof(svp_plate_vehicle_detect_info));
    CHECK_CALLOC_GOTO_ERROR(pv_ctx->info.plate_detect, EXIT0);
    set_default_yolov5_detect_param(&pv_ctx->info.plate_detect->param);
    pv_ctx->info.plate_detect->param.w       = SVP_PLATE_DETECT_WIDTH;
    pv_ctx->info.plate_detect->param.h       = SVP_PLATE_DETECT_HEIGHT;
    pv_ctx->info.plate_detect->param.num     = DETECTION_MODEL_SINGLE;
    pv_ctx->info.plate_detect->param.anchors = plate_anchors;

    pv_ctx->info.plate_detect->param.tracklet.tracker =
        (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) * XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    if (pv_ctx->info.plate_detect->param.tracklet.tracker == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "tracker calloc error !\n");
        goto EXIT1;
    }

    CHECK_SVP_COND_GOTO_ERROR(&cfg.modules[0] == XMEDIA_NULL, EXIT2, "plate detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((cfg.modules[0].load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(cfg.modules[0].pathname) != XMEDIA_SUCCESS), EXIT2, "plate detect model pathname err!\n");
    ret = detect_init(&pv_ctx->info.plate_detect->param, cfg.modules[0].alg_type,
        &cfg.modules[0], pv_ctx->info.plate_detect->param.anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT2, "plate detect init model failed !\n");

    xmedia_video_frame_info *plate_frame = &pv_ctx->info.plate_detect->detect_frame;
    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "PLATE_DEECT", &plate_frame->frame.addr.y_phy_addr,
        &pv_ctx->info.plate_detect->frame_viraddr, SVP_PLATE_DETECT_WIDTH * SVP_PLATE_DETECT_HEIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT3, "svp_mmz_alloc_and_map failed !\n");

    plate_frame->mod_id                          = MOD_ID_USER;
    plate_frame->frame.compress_mode             = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    plate_frame->frame.video_fmt                 = XMEDIA_VIDEO_FMT_LINEAR;
    plate_frame->frame.bit_width                 = XMEDIA_VIDEO_DATA_WIDTH_8;
    plate_frame->frame.width                     = SVP_PLATE_DETECT_WIDTH;
    plate_frame->frame.height                    = SVP_PLATE_DETECT_HEIGHT;
    plate_frame->frame.pixel_fmt                 = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    plate_frame->frame.stride.y_stride           = SVP_PLATE_DETECT_WIDTH;
    plate_frame->frame.stride.c_stride           = SVP_PLATE_DETECT_WIDTH;
    plate_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    plate_frame->frame.color_info.color_gamut    = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    plate_frame->frame.addr.c_phy_addr           = plate_frame->frame.addr.y_phy_addr +
                                                   SVP_PLATE_DETECT_WIDTH * SVP_PLATE_DETECT_HEIGHT;

    // 车牌识别模型
    pv_ctx->info.plate_reco = (svp_plate_vehicle_reco_info *)calloc(1, sizeof(svp_plate_vehicle_reco_info));
    CHECK_CALLOC_GOTO_ERROR(pv_ctx->info.plate_reco, EXIT3);
    CHECK_SVP_COND_GOTO_ERROR(&cfg.modules[1] == XMEDIA_NULL, EXIT4, "plate reco model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((cfg.modules[1].load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(cfg.modules[1].pathname) != XMEDIA_SUCCESS), EXIT4, "plate reco model pathname err!\n");

    pv_ctx->info.plate_reco->reco_model = (xmedia_npu_model *)calloc(1, sizeof(xmedia_npu_model));
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT4, "reco_model calloc failed !\n");

    ret = plate_vehicle_reco_init(&cfg.modules[1], pv_ctx->info.plate_reco->reco_model);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT5, "plate reco init model failed !\n");

    xmedia_video_frame_info *reco_frame = &pv_ctx->info.plate_reco->reco_frame;
    ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "PLATE_RECO", &reco_frame->frame.addr.y_phy_addr,
        &pv_ctx->info.plate_reco->frame_viraddr, SVP_PLATE_RECO_WIDTH * SVP_PLATE_RECO_HIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT6, "svp_mmz_alloc_and_map_cache failed !\n");

    reco_frame->mod_id                          = MOD_ID_USER;
    reco_frame->frame.compress_mode             = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    reco_frame->frame.video_fmt                 = XMEDIA_VIDEO_FMT_LINEAR;
    reco_frame->frame.bit_width                 = XMEDIA_VIDEO_DATA_WIDTH_8;
    reco_frame->frame.width                     = SVP_PLATE_RECO_WIDTH;
    reco_frame->frame.height                    = SVP_PLATE_RECO_HIGHT;
    reco_frame->frame.pixel_fmt                 = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    reco_frame->frame.stride.y_stride           = SVP_PLATE_RECO_WIDTH;
    reco_frame->frame.stride.c_stride           = SVP_PLATE_RECO_WIDTH;
    reco_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    reco_frame->frame.color_info.color_gamut    = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    reco_frame->frame.addr.c_phy_addr           = reco_frame->frame.addr.y_phy_addr +
                                                  SVP_PLATE_RECO_WIDTH * SVP_PLATE_RECO_HIGHT;

    // 车牌颜色模型
    pv_ctx->info.plate_reco->color_model = (xmedia_npu_model *)calloc(1, sizeof(xmedia_npu_model));
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT6, "color_model calloc failed !\n");
    ret = plate_vehicle_reco_init(&cfg.modules[2], pv_ctx->info.plate_reco->color_model);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT7, "color reco init model failed !\n");

    // 车辆检测模型
    pv_ctx->info.veh_detect = (svp_plate_vehicle_detect_info *)calloc(1, sizeof(svp_plate_vehicle_detect_info));
    CHECK_CALLOC_GOTO_ERROR(pv_ctx->info.veh_detect, EXIT8);
    set_default_yolov5_detect_param(&pv_ctx->info.veh_detect->param);
    pv_ctx->info.veh_detect->param.w       = SVP_VEHICLE_DETECT_WIDTH;
    pv_ctx->info.veh_detect->param.h       = SVP_VEHICLE_DETECT_HIGHT;
    pv_ctx->info.veh_detect->param.num     = DETECTION_MODEL_SINGLE;
    pv_ctx->info.veh_detect->param.anchors = vehicle_anchors;

    pv_ctx->info.veh_detect->param.tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    if (pv_ctx->info.veh_detect->param.tracklet.tracker == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "tracker calloc error !\n");
        goto EXIT9;
    }

    CHECK_SVP_COND_GOTO_ERROR(&cfg.modules[3] == XMEDIA_NULL, EXIT10, "vehicle detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((cfg.modules[3].load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(cfg.modules[3].pathname) != XMEDIA_SUCCESS),
        EXIT10, "vehicle  detect model pathname err!\n");
    ret = detect_init(&pv_ctx->info.veh_detect->param, cfg.modules[3].alg_type,
        &cfg.modules[3], pv_ctx->info.veh_detect->param.anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT10, "vehicle  detect init model failed !\n");

    xmedia_video_frame_info *vehicle_frame = &pv_ctx->info.veh_detect->detect_frame;
    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "VEHICLE_DETECT", &vehicle_frame->frame.addr.y_phy_addr,
        &pv_ctx->info.veh_detect->frame_viraddr, SVP_VEHICLE_DETECT_WIDTH * SVP_VEHICLE_DETECT_HIGHT * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT11, "svp_mmz_alloc_and_map failed !\n");

    vehicle_frame->mod_id                          = MOD_ID_USER;
    vehicle_frame->frame.compress_mode             = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    vehicle_frame->frame.video_fmt                 = XMEDIA_VIDEO_FMT_LINEAR;
    vehicle_frame->frame.bit_width                 = XMEDIA_VIDEO_DATA_WIDTH_8;
    vehicle_frame->frame.width                     = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_frame->frame.height                    = SVP_VEHICLE_DETECT_HIGHT;
    vehicle_frame->frame.pixel_fmt                 = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    vehicle_frame->frame.stride.y_stride           = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_frame->frame.stride.c_stride           = SVP_VEHICLE_DETECT_WIDTH;
    vehicle_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    vehicle_frame->frame.color_info.color_gamut    = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
    vehicle_frame->frame.addr.c_phy_addr           = vehicle_frame->frame.addr.y_phy_addr +
                                                     SVP_VEHICLE_DETECT_WIDTH * SVP_VEHICLE_DETECT_HIGHT;

// 车辆识别模型
   pv_ctx->info.veh_reco = (svp_plate_vehicle_reco_info *)calloc(1, sizeof(svp_plate_vehicle_reco_info));
   CHECK_CALLOC_GOTO_ERROR(pv_ctx->info.veh_reco, EXIT11);
   CHECK_SVP_COND_GOTO_ERROR(&cfg.modules[4] == XMEDIA_NULL, EXIT12, "vehicle reco model is NULL !\n");
   CHECK_SVP_COND_GOTO_ERROR((cfg.modules[4].load_mode == XMEDIA_SVP_MODEL_FILE) &&
       (check_file_exist(cfg.modules[4].pathname) != XMEDIA_SUCCESS), EXIT12, "vehicle  reco model pathname err!\n");

   pv_ctx->info.veh_reco->reco_model = (xmedia_npu_model *)calloc(1, sizeof(xmedia_npu_model));
   CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT12, "reco_model calloc failed !\n");

   ret = plate_vehicle_reco_init(&cfg.modules[4], pv_ctx->info.veh_reco->reco_model);
   CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT13, "vehicle reco init model failed !\n");

   xmedia_video_frame_info *veh_reco_frame = &pv_ctx->info.veh_reco->reco_frame;
   ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "VEHICLE_RECO", &veh_reco_frame->frame.addr.y_phy_addr,
       &pv_ctx->info.veh_reco->frame_viraddr, SVP_VEHICLE_RECO_WIDTH * SVP_VEHICLE_RECO_HEIGHT * 3 / 2);
   CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT14, "svp_mmz_alloc_and_map failed !\n");

   veh_reco_frame->mod_id                          = MOD_ID_USER;
   veh_reco_frame->frame.compress_mode             = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
   veh_reco_frame->frame.video_fmt                 = XMEDIA_VIDEO_FMT_LINEAR;
   veh_reco_frame->frame.bit_width                 = XMEDIA_VIDEO_DATA_WIDTH_8;
   veh_reco_frame->frame.width                     = SVP_VEHICLE_RECO_WIDTH;
   veh_reco_frame->frame.height                    = SVP_VEHICLE_RECO_HEIGHT;
   veh_reco_frame->frame.pixel_fmt                 = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
   veh_reco_frame->frame.stride.y_stride           = SVP_VEHICLE_RECO_WIDTH;
   veh_reco_frame->frame.stride.c_stride           = SVP_VEHICLE_RECO_WIDTH;
   veh_reco_frame->frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
   veh_reco_frame->frame.color_info.color_gamut    = XMEDIA_VIDEO_COLOR_GAMUT_BT709;
   veh_reco_frame->frame.addr.c_phy_addr           = veh_reco_frame->frame.addr.y_phy_addr +
                                                     SVP_VEHICLE_RECO_WIDTH * SVP_VEHICLE_RECO_HEIGHT;

    return XMEDIA_SUCCESS;

EXIT14:
    ret = plate_vehicle_reco_uninit(pv_ctx->info.veh_reco->reco_model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_reco_uninit failed %#x!\n", ret);
    }
EXIT13:
    CFREE(pv_ctx->info.veh_reco->reco_model);
EXIT12:
    CFREE(pv_ctx->info.veh_reco);
EXIT11:
    ret = detect_deinit(&pv_ctx->info.veh_detect->param);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_reco_uninit failed %#x!\n", ret);
    }
EXIT10:
    CFREE(pv_ctx->info.veh_detect->param.tracklet.tracker);
EXIT9:
    CFREE(pv_ctx->info.veh_detect);
EXIT8:
    ret = plate_vehicle_reco_uninit(pv_ctx->info.plate_reco->color_model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_reco_uninit failed %#x!\n", ret);
    }
EXIT7:
    CFREE(pv_ctx->info.plate_reco->color_model);
EXIT6:
    ret = plate_vehicle_reco_uninit(pv_ctx->info.plate_reco->reco_model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_reco_uninit failed %#x!\n", ret);
    }
EXIT5:
    CFREE(pv_ctx->info.plate_reco->reco_model);
EXIT4:
    CFREE(pv_ctx->info.plate_reco);
EXIT3:
    ret = detect_deinit(&pv_ctx->info.plate_detect->param);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "plate detect_deinit failed %#x!\n", ret);
    }
EXIT2:
    CFREE(pv_ctx->info.plate_detect->param.tracklet.tracker);
EXIT1:
    CFREE(pv_ctx->info.plate_detect);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 plate_vehicle_recognition_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    CHECK_SVP_COND_RETURN_ERROR(context == XMEDIA_NULL, "plate_vehicle_recognition_uninit context err !\n");
    svp_plate_vehicle_context *pv_ctx = (svp_plate_vehicle_context *)context;

    plate_vehicle_delete_all_nodes(&pv_ctx->info);

    if (pv_ctx->info.veh_reco != XMEDIA_NULL) {
        if (pv_ctx->info.veh_reco->reco_frame.frame.addr.y_phy_addr != XMEDIA_NULL) {
            svp_mmz_unmap_and_free(pv_ctx->info.veh_reco->reco_frame.frame.addr.y_phy_addr,
                pv_ctx->info.veh_reco->frame_viraddr);
        }
        ret = plate_vehicle_reco_uninit(pv_ctx->info.veh_reco->reco_model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "vehicle_reco_uninit failed %#x!\n", ret);
        }
        CFREE(pv_ctx->info.veh_reco->reco_model);
        CFREE(pv_ctx->info.veh_reco);
    }

    if (pv_ctx->info.veh_detect != XMEDIA_NULL) {
        if (pv_ctx->info.veh_detect->detect_frame.frame.addr.y_phy_addr != XMEDIA_NULL) {
            svp_mmz_unmap_and_free(pv_ctx->info.veh_detect->detect_frame.frame.addr.y_phy_addr,
                pv_ctx->info.veh_detect->frame_viraddr);
        }

        ret = detect_deinit(&pv_ctx->info.veh_detect->param);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
        }
        CFREE(pv_ctx->info.veh_detect);
    }

    if (pv_ctx->info.plate_reco != XMEDIA_NULL) {
        if (pv_ctx->info.plate_reco->reco_frame.frame.addr.y_phy_addr != XMEDIA_NULL) {
            svp_mmz_unmap_and_free(pv_ctx->info.plate_reco->reco_frame.frame.addr.y_phy_addr,
                pv_ctx->info.plate_reco->frame_viraddr);
        }
        ret = plate_vehicle_reco_uninit(pv_ctx->info.plate_reco->color_model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_reco_uninit failed %#x!\n", ret);
        }

        ret = plate_vehicle_reco_uninit(pv_ctx->info.plate_reco->reco_model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_reco_uninit failed %#x!\n", ret);
        }
        CFREE(pv_ctx->info.plate_reco->color_model);
        CFREE(pv_ctx->info.plate_reco->reco_model);
        CFREE(pv_ctx->info.plate_reco);
    }

    if (pv_ctx->info.plate_detect != XMEDIA_NULL) {
        if (pv_ctx->info.plate_detect->detect_frame.frame.addr.y_phy_addr != XMEDIA_NULL) {
            svp_mmz_unmap_and_free(pv_ctx->info.plate_detect->detect_frame.frame.addr.y_phy_addr,
                pv_ctx->info.plate_detect->frame_viraddr);
        }
        ret = detect_deinit(&pv_ctx->info.plate_detect->param);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
        }
        CFREE(pv_ctx->info.plate_detect);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 plate_vehicle_recognition_set_attr(xmedia_void *context, const xmedia_svp_plate_vehicle_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_plate_vehicle_context* pv_ctx = (svp_plate_vehicle_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(pv_ctx == XMEDIA_NULL, "adas_set_attr context err !\n");

    ret = base_set_attr(&pv_ctx->info.plate_detect->param, &task_attr->plate_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "plate detect_set_attr failed %#x!\n", ret);
    ret = base_set_attr(&pv_ctx->info.veh_detect->param, &task_attr->veh_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "vehicle detect_set_attr failed %#x!\n", ret);

    pv_ctx->info.report_count = task_attr->report_count;
    pv_ctx->info.report_thres = task_attr->report_thres;
    memcpy(&pv_ctx->info.detect_zone, &task_attr->detect_zone, sizeof(xmedia_svp_zone));

    return XMEDIA_SUCCESS;
}

xmedia_s32 plate_vehicle_recognition_get_attr(xmedia_void *context, xmedia_svp_plate_vehicle_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_plate_vehicle_context* pv_ctx = (svp_plate_vehicle_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(pv_ctx == XMEDIA_NULL, "adas_set_attr context err !\n");

    ret = base_get_attr(&pv_ctx->info.plate_detect->param, &task_attr->plate_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "plate detect_set_attr failed %#x!\n", ret);
    ret = base_get_attr(&pv_ctx->info.veh_detect->param, &task_attr->plate_attr);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "vehicle detect_set_attr failed %#x!\n", ret);

    task_attr->report_count = pv_ctx->info.report_count;
    task_attr->report_thres = pv_ctx->info.report_thres;
    memcpy(&task_attr->detect_zone, &pv_ctx->info.detect_zone, sizeof(xmedia_svp_zone));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 plate_vehicle_reco_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_plate_vehicle_output *output)
{
    CHECK_SVP_COND_RETURN_ERROR(context == XMEDIA_NULL, "plate_vehicle_vehicle_detect_process context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(output == XMEDIA_NULL, "plate_vehicle_vehicle_detect_process output err !\n");
    CHECK_SVP_COND_RETURN_ERROR(input == XMEDIA_NULL, "plate_vehicle_vehicle_detect_process input err !\n");

    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 i, j;
    xmedia_svp_yolov8_output veh_output, plate_output;
    xmedia_float scale_x, scale_y, plate_scale_x, plate_scale_y;
    xmedia_video_frame_info veh_frame;
    xmedia_u32 x1, y1, x2, y2;
    svp_plate_vehicle_status_node* cur_node;
    xmedia_svp_rect std_rect;
    xmedia_s32 zone_index;
    xmedia_float pre_rect, cur_rect;
    xmedia_video_frame_info *big_frame;
    svp_plate_vehicle_context *pv_ctx;
    xmedia_video_frame_info *veh_detect_frame;
    xmedia_video_frame_info *plate_detect_frame;
    xmedia_video_frame_info *plate_reco_frame;
    xmedia_video_frame_info *veh_reco_frame;
    xmedia_void *src_vir;
    C_ALG_IMAGE_INFO src_img = {0};
    C_ALG_IMAGE_INFO dst_img = {0};
    C_ALG_RESULT alg_result;
    svp_warpaffine_pairpack warp_pack;
    xmedia_video_frame_info input_frame;

    big_frame = input->frame + 1;
    pv_ctx = (svp_plate_vehicle_context *)context;
    if (pv_ctx == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_recognition_init context err !\n");
        return XMEDIA_FAILURE;
    }

    veh_detect_frame   = &pv_ctx->info.veh_detect->detect_frame;
    plate_detect_frame = &pv_ctx->info.plate_detect->detect_frame;
    plate_reco_frame   = &pv_ctx->info.plate_reco->reco_frame;
    veh_reco_frame     = &pv_ctx->info.veh_reco->reco_frame;

    ret = svp_vgs_resize(big_frame, veh_detect_frame);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize failed!\n");

    ret = npu_run_model(&pv_ctx->info.veh_detect->param.model, veh_detect_frame->frame.addr.y_phy_addr);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vehicle npu_run_model failed!\n");

    ret = svp_get_detect_result_bz(&pv_ctx->info.veh_detect->param, &veh_output, vehicle_anchors);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_detect_result_yolov5_bz failed!\n");

    if (pv_ctx->info.veh_detect->param.tracklet.enable == XMEDIA_TRUE) {
        xmedia_svp_tracker_result result = {0};
        result.target_num = veh_output.target_num;
        for(i = 0; i < result.target_num; i++) {
            if (i >= XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM) {
                break;
            }
            result.tracker_single[i].detect_score    = veh_output.targets[i].detect_score;
            result.tracker_single[i].classfier_score = veh_output.targets[i].classfier_score;
            result.tracker_single[i].rect            = veh_output.targets[i].rect;
        }
        ret = svp_detect_bytetracker(&result,
                                     &pv_ctx->info.veh_detect->param.tracklet,
                                     &pv_ctx->info.veh_detect->param.tarck_id_grow,
                                     pv_ctx->info.veh_detect->param.tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }

        memset(&veh_output, 0, sizeof(xmedia_svp_yolov8_output));
        for(i = 0; i < result.target_num; i++) {
            if (veh_output.target_num >= XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM) {
                break;
            }
            veh_output.targets[veh_output.target_num].detect_score    = result.tracker_single[i].detect_score;
            veh_output.targets[veh_output.target_num].classfier_score = result.tracker_single[i].classfier_score;
            veh_output.targets[veh_output.target_num].tracker_id      = result.tracker_single[i].tracker_id;
            veh_output.targets[veh_output.target_num].tracker_age     = result.tracker_single[i].tracker_age;
            veh_output.targets[veh_output.target_num].rect            = result.tracker_single[i].rect;
            veh_output.target_num++;
        }
    }

    src_vir = xmedia_mmz_map(big_frame->frame.addr.y_phy_addr,
        big_frame->frame.stride.y_stride * big_frame->frame.height * 3 >> 1, XMEDIA_TRUE);

    // 遍历删除超时节点
    plate_vehicle_delete_expired_nodes(&pv_ctx->info);

    //返回原坐标 1080P
    scale_x = (xmedia_float)big_frame->frame.width / SVP_VEHICLE_DETECT_WIDTH;
    scale_y = (xmedia_float)big_frame->frame.height / SVP_VEHICLE_DETECT_HIGHT;

    output->target_num = 0;
    for (i = 0; i < veh_output.target_num; i++) {
        if (output->target_num >= XMEDIA_SVP_PLATE_VEH_MAX_TARGET_NUM) {
            break;
        }
        std_rect.x1 = veh_output.targets[i].rect.x1 * scale_x;
        std_rect.x2 = veh_output.targets[i].rect.x2 * scale_x;
        std_rect.y1 = veh_output.targets[i].rect.y1 * scale_y;
        std_rect.y2 = veh_output.targets[i].rect.y2 * scale_y;
        ret = clip_rect_to_frame(&std_rect, big_frame->frame.width, big_frame->frame.height);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "clip_rect_to_frame failed!\n");

        if (pv_ctx->info.detect_zone.zone_num != 0) { // 未设置区域默认全屏检测
            zone_index = svp_detect_zone(&pv_ctx->info.detect_zone, &std_rect,
                big_frame->frame.width, big_frame->frame.height);
            if (zone_index == -1) { // 不在任何一个区域内
                continue;
            }
        }

        x1 = (xmedia_u32)(std_rect.x1) / 2 * 2;
        x2 = (xmedia_u32)(std_rect.x2) / 2 * 2;
        y1 = (xmedia_u32)(std_rect.y1) / 2 * 2;
        y2 = (xmedia_u32)(std_rect.y2) / 2 * 2;

        memcpy(&veh_frame, big_frame, sizeof(xmedia_video_frame_info));
        veh_frame.frame.addr.y_phy_addr  =
            big_frame->frame.addr.y_phy_addr + x1 + big_frame->frame.stride.y_stride * y1;
        veh_frame.frame.addr.c_phy_addr  =
            big_frame->frame.addr.c_phy_addr + x1 + big_frame->frame.stride.c_stride * y1 / 2;
        veh_frame.frame.width            = (xmedia_u32)abs(x2 - x1) / 2 * 2;
        veh_frame.frame.height           = (xmedia_u32)abs(y2 - y1) / 2 * 2;
        veh_frame.frame.stride.y_stride  = big_frame->frame.stride.y_stride;
        veh_frame.frame.stride.c_stride  = big_frame->frame.stride.c_stride;

        plate_scale_x = (xmedia_float)veh_frame.frame.width / SVP_PLATE_DETECT_WIDTH;
        plate_scale_y = (xmedia_float)veh_frame.frame.height / SVP_PLATE_DETECT_HEIGHT;

        ret = svp_vgs_resize(&veh_frame, plate_detect_frame);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize failed!\n");

        ret = npu_run_model(&pv_ctx->info.plate_detect->param.model,plate_detect_frame->frame.addr.y_phy_addr);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate npu_run_model failed!\n");

        ret = svp_get_detect_result_bz(&pv_ctx->info.plate_detect->param, &plate_output, plate_anchors);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_detect_result_yolov5_bz failed!\n");
        if (plate_output.target_num <= 0) {
            continue;
        }

        //车牌宽度返回原坐标 1080P
        xmedia_float plate_width = (plate_output.targets[0].rect.x2 - plate_output.targets[0].rect.x1) * plate_scale_x;
        if (plate_width < pv_ctx->info.plate_min_width) {
            continue;
        }

        //车牌关键点返回原坐标 1080P
        for (j = 0; j < SVP_PLATE_KPS; j++) { // 只需要一个最高分数的车牌目标
            warp_pack.keypoints[2 * j + 0]    = x1 + plate_output.targets[0].kpt[j].x * plate_scale_x;
            warp_pack.keypoints[2 * j + 1]    = y1 + plate_output.targets[0].kpt[j].y * plate_scale_y;
            warp_pack.targetpoints[2 * j + 0] = reco_keypoints.point_arry[j].x;
            warp_pack.targetpoints[2 * j + 1] = reco_keypoints.point_arry[j].y;
        }

        src_img.image_buffer = (unsigned char *)src_vir;
        src_img.image_width  = big_frame->frame.width;
        src_img.image_height = big_frame->frame.height;
        src_img.image_stride = big_frame->frame.width;

        dst_img.image_buffer = (unsigned char *)pv_ctx->info.plate_reco->frame_viraddr;
        dst_img.image_width  = SVP_PLATE_RECO_WIDTH;
        dst_img.image_height = SVP_PLATE_RECO_HIGHT;
        dst_img.image_stride = SVP_PLATE_RECO_WIDTH;

        alg_result = alg_api_plate_warpaffine(&src_img, &dst_img,
                                (xmedia_float *)&warp_pack.targetpoints,
                                (xmedia_float *)&warp_pack.keypoints);
        CHECK_SVP_COND_GOTO_ERROR(alg_result != 0, EXIT, "plate_warpaffine failed, error:%d !\n", alg_result);

        xmedia_mmz_flush_cache(plate_reco_frame->frame.addr.y_phy_addr, dst_img.image_buffer,
            plate_reco_frame->frame.stride.y_stride * plate_reco_frame->frame.height * 3 >> 1);

        ret = npu_run_model(pv_ctx->info.plate_reco->reco_model, plate_reco_frame->frame.addr.y_phy_addr);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate reco npu_run_model failed!\n");

        svp_plate_vehicle_char_vec chars_vec = {0};
        svp_plate_vehicle_info base_info = {0};
        ret = svp_plate_vehicle_plate_char_result_bz(pv_ctx->info.plate_reco->reco_model,
            &chars_vec, base_info.reco_char, &base_info.char_score);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_reco_result_bz failed!\n");

        if (chars_vec.num < SVP_PLATE_CHAR_MIN || chars_vec.num > SVP_PLATE_CHAR_MAX ||
            base_info.char_score < pv_ctx->info.report_thres) {
            continue;
        }
        if ((chars_vec.num == SVP_PLATE_CHAR_MAX) && (new_energy_rule(&chars_vec) == XMEDIA_FALSE)) {
            continue;
        }
        if ((chars_vec.num == SVP_PLATE_CHAR_MIN) && (normal_rule(&chars_vec) == XMEDIA_FALSE)) {
            continue;
        }
        // 车牌颜色
        ret = npu_run_model(pv_ctx->info.plate_reco->color_model, veh_reco_frame->frame.addr.y_phy_addr);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "veh reco npu_run_model failed!\n");

        ret = svp_get_platecolor_result_bz(pv_ctx->info.plate_reco->color_model,
            pv_ctx->info.plate_detect->param.sigmoid_table, &base_info.plate_color, &base_info.plate_color_score);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_color_result_yolov5_bz failed!\n");

        base_info.plate_rect.x1  = plate_output.targets[0].rect.x1 * plate_scale_x + x1;
        base_info.plate_rect.x2  = plate_output.targets[0].rect.x2 * plate_scale_x + x1;
        base_info.plate_rect.y1  = plate_output.targets[0].rect.y1 * plate_scale_y + y1;
        base_info.plate_rect.y2  = plate_output.targets[0].rect.y2 * plate_scale_y + y1;
        ret = clip_rect_to_frame(&base_info.plate_rect, big_frame->frame.width, big_frame->frame.height);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "clip_rect_to_frame failed!\n");

        // 车辆识别
        memcpy(&input_frame, big_frame, sizeof(xmedia_video_frame_info));
        input_frame.frame.addr.y_phy_addr  =
            big_frame->frame.addr.y_phy_addr + x1 + big_frame->frame.stride.y_stride * y1;
        input_frame.frame.addr.c_phy_addr  =
            big_frame->frame.addr.c_phy_addr + x1 + big_frame->frame.stride.c_stride * y1 / 2;
        input_frame.frame.width            = (xmedia_u32)abs(x2 - x1) / 2 * 2;
        input_frame.frame.height           = (xmedia_u32)abs(y2 - y1) / 2 * 2;
        input_frame.frame.stride.y_stride  = big_frame->frame.stride.y_stride;
        input_frame.frame.stride.c_stride  = big_frame->frame.stride.c_stride;

        ret = svp_vgs_resize(&input_frame, veh_reco_frame);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize failed!\n");

        ret = npu_run_model(pv_ctx->info.veh_reco->reco_model, veh_reco_frame->frame.addr.y_phy_addr);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vehicle reco npu_run_model failed!\n");

        base_info.veh_rect = std_rect;
        base_info.track_id = veh_output.targets[i].tracker_id;
        ret = svp_get_vehicle_typecolor_result_bz(pv_ctx->info.veh_reco->reco_model, &base_info);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_get_reco_result_yolov5_bz failed!\n");

        ret = plate_vehicle_insert_status_node(&pv_ctx->info, &cur_node, &base_info);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate_vehicle_insert_status_node failed!\n");

        if (cur_node->data.exist_cnt > pv_ctx->info.report_count && cur_node->data.is_report == XMEDIA_FALSE) {
            cur_node->data.is_report = XMEDIA_TRUE;
            ret = plate_vehicle_tag_reported_node(&pv_ctx->info, veh_output.targets[i].tracker_id);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate_vehicle_tag_reported_node failed!\n");
            pre_rect = fabs(cur_node->data.base_info.plate_rect.x2 - cur_node->data.base_info.plate_rect.x1) *
                       fabs(cur_node->data.base_info.plate_rect.y2 - cur_node->data.base_info.plate_rect.y1);
            cur_rect = fabs(base_info.plate_rect.x2 - base_info.plate_rect.x1) *
                       fabs(base_info.plate_rect.y2 - base_info.plate_rect.y1);
            if (pre_rect < cur_rect) {
                output->targets[output->target_num].plate_direction = XMEDIA_SVP_DIRECTION_FORWARD;
                output->targets[output->target_num].veh_direction   = XMEDIA_SVP_DIRECTION_FORWARD;
            } else if (pre_rect > cur_rect) {
                output->targets[output->target_num].plate_direction = XMEDIA_SVP_DIRECTION_BACKWARD;
                output->targets[output->target_num].veh_direction   = XMEDIA_SVP_DIRECTION_BACKWARD;
            } else {
                output->targets[output->target_num].plate_direction = XMEDIA_SVP_DIRECTION_UNKOWN;
                output->targets[output->target_num].veh_direction   = XMEDIA_SVP_DIRECTION_UNKOWN;
            }

            snprintf(output->targets[output->target_num].plate_char, XMEDIA_SVP_PLATE_CHAR_MAX_NUM,
                "%s", cur_node->data.base_info.reco_char);
            output->targets[output->target_num].plate_score     = cur_node->data.base_info.char_score;
            output->targets[output->target_num].plate_color     = cur_node->data.base_info.plate_color;
            output->targets[output->target_num].plate_color_scr = cur_node->data.base_info.plate_color_score;
            output->targets[output->target_num].plate_rect      = base_info.plate_rect;
            output->targets[output->target_num].veh_rect        = std_rect;
            output->targets[output->target_num].tracker_id      = cur_node->data.base_info.track_id;
            output->targets[output->target_num].veh_color       = cur_node->data.base_info.veh_color;
            output->targets[output->target_num].veh_color_scr   = cur_node->data.base_info.veh_color_score;
            output->targets[output->target_num].veh_type        = cur_node->data.base_info.veh_type;
            output->targets[output->target_num].veh_type_scr    = cur_node->data.base_info.veh_type_score;

            output->target_num++;
        }
    }

    xmedia_mmz_unmap(src_vir);

    return XMEDIA_SUCCESS;

EXIT:
    return XMEDIA_FAILURE;
}

xmedia_s32 plate_vehicle_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_plate_vehicle_output *output)
{
    CHECK_SVP_COND_RETURN_ERROR(context == XMEDIA_NULL, "plate_vehicle_recognition_process context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(output == XMEDIA_NULL, "plate_vehicle_recognition_process output err !\n");
    CHECK_SVP_COND_RETURN_ERROR(input == XMEDIA_NULL, "plate_vehicle_recognition_process input err !\n");

    xmedia_s32 ret = XMEDIA_SUCCESS;

    svp_plate_vehicle_context *pv_ctx = (svp_plate_vehicle_context *)context;
    if (pv_ctx == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "plate_vehicle_recognition_init context err !\n");
        return XMEDIA_FAILURE;
    }

    ret = plate_vehicle_reco_process(context, input, output);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "plate_vehicle_reco_road_process failed!\n");

    return XMEDIA_SUCCESS;

EXIT:
    return XMEDIA_FAILURE;
}
