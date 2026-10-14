#include "yolov8.h"

#define MODEL_IN_H 360
#define MODEL_IN_W 640
#define DFL_LEN 16
#define MODEL_LAYER_NUM 3
#define BOX_COORDINATE_NUM 4


static xmedia_s32 grid_h_array[MODEL_LAYER_NUM] = {48, 24, 12};
static xmedia_s32 grid_w_array[MODEL_LAYER_NUM] = {80, 40, 20};
static xmedia_s32 stride_array[MODEL_LAYER_NUM] = {8, 16, 32};

xmedia_void set_default_yolov8_detect_param(yolov8_detect_param *param)
{
    xmedia_s32 i;
    if (param) {
        param->w = INPUT_DEFAULT_WIDE;
        param->h = INPUT_DEFAULT_HIGH;
        param->num = DETECTION_MODEL_MAX;
        param->detect_threshold = DEFAULT_DETECT_THRESHOLD;
        param->iou_threshold = DEFAULT_IOU_THRESHOLD;
        param->max_target_num = XMEDIA_SVP_MAX_TARGET_NUM;
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
        param->thres_desig = desigmoid(param->detect_threshold);
        param->dup_flag = XMEDIA_FALSE;
        for (i = 0; i < XMEDIA_SVP_MAX_TARGET_NUM; i++) {
            memset(&param->record_result.stable_targets[i].rect, 0,
                sizeof(param->record_result.stable_targets[i].rect));
        }
    }
}

xmedia_s32 yolov8_init(yolov8_detect_param *param, const xmedia_svp_modules *model)
{
    xmedia_s32 ret, i;

    if (param->dup_flag == XMEDIA_FALSE) {
        ret = npu_load_model(model, &param->model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu load model failed\n");
            return XMEDIA_FAILURE;
        }
    }
    for (i = 0; i < param->model.output.num; i++) {
        param->quanlize[i].scale = param->model.output.tensor[i].quant.scale;
        param->quanlize[i].zp = param->model.output.tensor[i].quant.zp;
    }

    param->thres_desig = desigmoid(param->detect_threshold);
    param->output_num = 0;
    param->bbox = (svp_base_yolov8_result *)calloc(1, sizeof(svp_base_yolov8_result) * DEFAULT_BBOX_NUM);
    CHECK_CALLOC_RETURN_ERROR(param->bbox);

    *(param->model.user_count) += 1;
    /*for (i = 0; i < MODEL_LAYER_NUM; i++) {
      param->output_num += grid_h_array[i] * grid_w_array[i];
    }*/

    return ret;
}

xmedia_s32 yolov8_uninit(yolov8_detect_param *param)
{
    CFREE(param->bbox);
    if (*(param->model.user_count) == 1) {
        *(param->model.user_count) -= 1;
        return npu_unload_model(&param->model);
    }

    SVP_TRACE(MODULE_DBG_DEBUG, "detect_deinit, detect_modle_user_count: %d\n", *(param->model.user_count));
    *(param->model.user_count) -= 1;

    return XMEDIA_SUCCESS;
}

static inline xmedia_s32 cmp_score_fall(const xmedia_void *lsh, const xmedia_void *rsh)
{
    // 降序排列, 令score最大的box在队列末端
    svp_base_yolov8_result *s1 = (svp_base_yolov8_result *)lsh;
    svp_base_yolov8_result *s2 = (svp_base_yolov8_result *)rsh;
    return s1->detect_score > s2->detect_score ? -1 : 1;
}

xmedia_float iouv8(svp_base_yolov8_result *box1, svp_base_yolov8_result *box2)
{
    xmedia_float area1;
    xmedia_float area2;
    xmedia_float mix_area;

    if (box1->class_type != box2->class_type) {
        return 0.0f;
    }

    mix_area = STD_MAX(0.0f, STD_MIN(box1->rect.x2, box2->rect.x2) -
                          STD_MAX(box1->rect.x1, box2->rect.x1)) *
               STD_MAX(0.0f, STD_MIN(box1->rect.y2, box2->rect.y2) -
                          STD_MAX(box1->rect.y1, box2->rect.y1));
    if ((mix_area - SVP_EPSILON) <= 0.0f) {
        return 0.0f;
    }
    area1 = (box1->rect.x2 - box1->rect.x1) * (box1->rect.y2 - box1->rect.y1);
    area2 = (box2->rect.x2 - box2->rect.x1) * (box2->rect.y2 - box2->rect.y1);

    return (mix_area / (area1 + area2 - mix_area));
}

xmedia_s32 yolov8_det_nms(svp_base_yolov8_result *vec_bbox, xmedia_u32 vec_bbox_len,
                            xmedia_float threshold, xmedia_u32 *picked_bbox_len)
{
    xmedia_u32 i, j, k, m;
    svp_base_yolov8_result *box_b;
    svp_base_yolov8_result *box_a;

    qsort(vec_bbox, vec_bbox_len, sizeof(svp_base_yolov8_result), cmp_score_fall);

    for (i = 0, j = vec_bbox_len; i < j; i++) {
        box_a = &vec_bbox[i];
        if (box_a->detect_score <= SVP_EPSILON) {
            continue;
        }
        for (k = i + 1, m = i + 1; k < j; k++) {
            box_b = &vec_bbox[k];
            if (box_a->detect_score <= SVP_EPSILON || box_a->class_type != box_b->class_type) {
                continue;
            }
            xmedia_float box_iou = iouv8(box_a, box_b);
            // 大于阈值过滤掉
            if (box_iou >= threshold) {
                continue;
            }
            if (m != k) {
                vec_bbox[m] = vec_bbox[k];
            }
            m++;
        }
        j = m;
    }
    *picked_bbox_len = i;

    return XMEDIA_SUCCESS;
}

inline static int clamp(xmedia_float val, int min, int max) { return val > min ? (val < max ? val : max) : min; }

static xmedia_float sigmoid(xmedia_float x) { return 1.0 / (1.0 + expf(-x)); }

static xmedia_float unsigmoid(xmedia_float y) { return -1.0 * logf((1.0 / y) - 1.0); }

inline static int32_t __clip(xmedia_float val, xmedia_float min, xmedia_float max)
{
    xmedia_float f = val <= min ? min : (val >= max ? max : val);
    return f;
}

static xmedia_u8 qnt_f32_to_affine_u8(xmedia_float f32, int32_t zp, xmedia_float scale)
{
    xmedia_float dst_val = (f32 / scale) + zp;
    xmedia_u8 res = (xmedia_u8)__clip(dst_val, 0, 255);
    return res;
}

static inline xmedia_float deqnt_affine_u8_to_f32(xmedia_u8 qnt, int32_t zp, xmedia_float scale)
{
    return ((xmedia_float)qnt - (xmedia_float)zp) * scale;
}

static void compute_dfl(xmedia_float* tensor, xmedia_float* box){
    for (int b=0; b<BOX_COORDINATE_NUM; b++){
        xmedia_float exp_t[DFL_LEN];
        xmedia_float exp_sum=0;
        xmedia_float acc_sum=0;
        for (int i=0; i< DFL_LEN; i++){
            exp_t[i] = exp(tensor[i+b*DFL_LEN]);
            exp_sum += exp_t[i];
        }

        for (int i=0; i< DFL_LEN; i++){
            acc_sum += exp_t[i]/exp_sum *i;
        }
        box[b] = acc_sum;
    }
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

xmedia_s32 svp_get_detect_result_bz(yolov5_detect_param *param, xmedia_svp_yolov8_output *result,
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
    svp_base_yolov8_result *bbox;
    bbox = (svp_base_yolov8_result *)calloc(1, DEFAULT_BBOX_NUM * sizeof(svp_base_yolov8_result));
    CHECK_CALLOC_RETURN_ERROR(bbox);

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
                    if (idx >= DEFAULT_BBOX_NUM) {
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

                        bbox[idx].detect_score = score;
                        bbox[idx].rect.x1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, cx - w_b * 0.5f));
                        bbox[idx].rect.y1 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, cy - h_b * 0.5f));
                        bbox[idx].rect.x2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, cx + w_b * 0.5f));
                        bbox[idx].rect.y2 = STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, cy + h_b * 0.5f));

                        for (xmedia_s32 kp_idx = 0; kp_idx < SVP_PLATE_KPS; kp_idx++) {
                            bbox[idx].kpt[kp_idx].x = dequantize(addr[offset + 2 * kp_idx + 5],
                                param->quanlize[i].scale, param->quanlize[i].zp) * anchor_x + x * stride;
                            bbox[idx].kpt[kp_idx].y = dequantize(addr[offset + 2 * kp_idx + 6],
                                param->quanlize[i].scale, param->quanlize[i].zp) * anchor_y + y * stride;
                            bbox[idx].kpt[kp_idx].x = STD_MAX(0.0f,
                                STD_MIN((xmedia_float)param->w, bbox[idx].kpt[kp_idx].x));
                            bbox[idx].kpt[kp_idx].y = STD_MAX(0.0f,
                                STD_MIN((xmedia_float)param->w, bbox[idx].kpt[kp_idx].y));
                        }
                        idx++;
                    }
                    // w
                    offset += param->model.output.tensor[i].shape.dims[3];
                }
            }

        }
    }

    if (idx == 0) {
        CFREE(bbox);
        result->target_num = 0;
        return XMEDIA_SUCCESS;
    } else if (idx == 1) {
        result->targets[0].detect_score = bbox[0].detect_score;
        result->targets[0].rect         = bbox[0].rect;
        result->targets[0].class_type   = bbox[0].class_type;
        result->targets[0].alg_type     = param->type;
        memcpy(result->targets[0].kpt, bbox[0].kpt, sizeof(xmedia_svp_keypoint) * SVP_PLATE_KPS);

        result->target_num = idx;
        CFREE(bbox);
        return XMEDIA_SUCCESS;
    }

    out_box_len = 0;
    yolov8_det_nms(bbox, idx, param->iou_threshold, &out_box_len);
    // get result
    result->target_num = 0;
    for (i = 0; i < out_box_len; i++) {
        result->targets[result->target_num].detect_score = bbox[i].detect_score;
        result->targets[result->target_num].rect         = bbox[i].rect;
        result->targets[result->target_num].class_type   = bbox[i].class_type;
        result->targets[result->target_num].alg_type     = param->type;
        memcpy(result->targets[result->target_num].kpt, bbox[i].kpt, sizeof(xmedia_svp_keypoint) * SVP_PLATE_KPS);

        result->target_num += 1;
        if (result->target_num >= param->max_target_num) {
            break;
        }
    }
    CFREE(bbox);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 svp_yolov8_get_detect_result(yolov8_detect_param *param, xmedia_svp_yolov8_output *output)
{
    xmedia_s32 idx = 0;
    xmedia_s32 index;
    xmedia_s32 i = 0;
    xmedia_u32 out_box_len = 0;

    for (index = 0; index < MODEL_LAYER_NUM; index++) {
        xmedia_s32 grid_h = grid_h_array[index];
        xmedia_s32 grid_w = grid_w_array[index];
        xmedia_s32 stride = stride_array[index];
        xmedia_s32 box_idx = index * 2;
        xmedia_s32 score_idx = index * 2 + 1;
        xmedia_s32 keypoint_idx = index + 6;

        xmedia_s32 score_zp = param->quanlize[score_idx].zp;
        xmedia_float score_scale = param->quanlize[score_idx].scale;
        xmedia_u8* score_tensor = param->model.output.tensor[score_idx].addr;

        xmedia_s32 box_zp = param->quanlize[box_idx].zp;
        xmedia_float box_scale = param->quanlize[box_idx].scale;
        xmedia_u8* box_tensor = param->model.output.tensor[box_idx].addr;

        xmedia_s32 keypoint_zp = param->quanlize[keypoint_idx].zp;
        xmedia_float keypoint_scale = param->quanlize[keypoint_idx].scale;
        xmedia_u8* keypoint_tensor = param->model.output.tensor[keypoint_idx].addr;

        xmedia_s32 grid_len = grid_h * grid_w;
        xmedia_u8 score_thres_u8 = qnt_f32_to_affine_u8(unsigmoid(param->detect_threshold), score_zp, score_scale);
        for (xmedia_s32 i = 0; i < grid_h; i++) {
            for (xmedia_s32 j = 0; j < grid_w; j++) {
                if (idx >= DEFAULT_BBOX_NUM) {
                    break;
                }
                xmedia_s32 offset = i * grid_w + j;
                xmedia_s32 max_class_id = -1;

                xmedia_u8 max_score = -score_zp;
                for (xmedia_u8 c = 0; c < param->num; c++) {
                    if ((score_tensor[offset] > score_thres_u8) && (score_tensor[offset] > max_score)) {
                        max_score = score_tensor[offset];
                        max_class_id = c;
                    }
                    offset += grid_len;
                }

                // compute box
                if (max_score > score_thres_u8) {
                    offset = i * grid_w + j;
                    xmedia_float box[BOX_COORDINATE_NUM];
                    xmedia_float before_dfl[DFL_LEN * BOX_COORDINATE_NUM];
                    for (xmedia_u8 k = 0; k < DFL_LEN * BOX_COORDINATE_NUM; k++) {
                        before_dfl[k] = deqnt_affine_u8_to_f32(box_tensor[offset], box_zp, box_scale);
                        offset += grid_len;
                    }
                    compute_dfl(before_dfl, box);

                    xmedia_float x1, y1, x2, y2; // w, h;
                    x1 = (-box[0] + j + 0.5) * stride;
                    y1 = (-box[1] + i + 0.5) * stride;
                    x2 = (box[2] + j + 0.5) * stride;
                    y2 = (box[3] + i + 0.5) * stride;
/*
                    // 目标框面积大小限制
                    if (param->box_area_limit.enable) {
                        xmedia_float box_area = (x2 - x1) * (y2 - y1); // w * h
                        if ((box_area < param->box_area_limit.min_area) || (box_area > param->box_area_limit.max_area)) {
                            continue;
                        }
                    }

                    // 边缘过滤
                    if (param->box_edge_limit.enable) {
                        xmedia_float left_edge, right_edge, top_edge, bottom_edge;
                        left_edge = param->box_edge_limit.w_edge;
                        right_edge = param->w - param->box_edge_limit.w_edge;
                        top_edge = param->box_edge_limit.h_edge;
                        bottom_edge = param->h - param->box_edge_limit.h_edge;

                        if (STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x1)) < left_edge
                            || STD_MAX(0.0f, STD_MIN((xmedia_float)param->w, x2)) > right_edge
                            || STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y1)) < top_edge
                            || STD_MAX(0.0f, STD_MIN((xmedia_float)param->h, y2)) > bottom_edge) {
                            continue;
                        }
                    }
*/
                    param->bbox[idx].rect.x1 = clamp(x1, 0, param->w);
                    param->bbox[idx].rect.y1 = clamp(y1, 0, param->h);
                    param->bbox[idx].rect.x2 = clamp(x2, 0, param->w);
                    param->bbox[idx].rect.y2 = clamp(y2, 0, param->h);
                    //w = x2 - x1;
                    //h = y2 - y1;

                    param->bbox[idx].detect_score = sigmoid(deqnt_affine_u8_to_f32(max_score, score_zp, score_scale));
                    param->bbox[idx].class_type = max_class_id;
                    //objProbs.push_back(sigmoid(deqnt_affine_u8_to_f32(max_score, score_zp, score_scale)));
                    //classId.push_back(max_class_id);

                    // for 17 keypoints (x, y, score)
                    offset = i * grid_w + j;
                    xmedia_float kp_x, kp_y, kp_score;
                    for (xmedia_u8 k = 0; k < param->keypoint_len; k++) {
                        kp_x = deqnt_affine_u8_to_f32(keypoint_tensor[offset], keypoint_zp, keypoint_scale);
                        kp_x = clamp((kp_x * 2.0 + j) * stride, 0, MODEL_IN_W);
                        offset += grid_len;
                        kp_y = deqnt_affine_u8_to_f32(keypoint_tensor[offset], keypoint_zp, keypoint_scale);
                        kp_y = clamp((kp_y * 2.0 + i) * stride, 0, MODEL_IN_H);
                        offset += grid_len;
                        kp_score = deqnt_affine_u8_to_f32(keypoint_tensor[offset], keypoint_zp, keypoint_scale);
                        kp_score = sigmoid(kp_score);
                        offset += grid_len;
                        param->bbox[idx].kpt[k].x = kp_x;
                        param->bbox[idx].kpt[k].y = kp_y;
                        param->bbox[idx].kpt[k].score = kp_score;
                    }
                    idx++;
                }
            }
        }
    }
    if (idx == 0) {
        output->target_num = 0;
        return XMEDIA_SUCCESS;
    } else if (idx == 1) {
        output->targets[0].detect_score = param->bbox[0].detect_score;
        output->targets[0].rect         = param->bbox[0].rect;
        output->targets[0].class_type   = param->bbox[0].class_type;
        output->targets[0].alg_type     = param->type;
        memcpy(output->targets[0].kpt, param->bbox[0].kpt, sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
        output->target_num = 1;
        return XMEDIA_SUCCESS;
    }

    yolov8_det_nms(param->bbox, idx, param->iou_threshold,&out_box_len);
    for (i = 0; i < out_box_len; i++) {
        output->targets[output->target_num].detect_score = param->bbox[i].detect_score;
        output->targets[output->target_num].rect         = param->bbox[i].rect;
        output->targets[output->target_num].class_type   = param->bbox[i].class_type;
        output->targets[output->target_num].alg_type     = param->type;
        memcpy(output->targets[output->target_num].kpt, param->bbox[i].kpt,
            sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
        output->target_num += 1;
        if(output->target_num >= param->max_target_num) {
           break;
        }
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 detect_yolov8_process(yolov8_detect_param *param, const xmedia_video_frame_info *frame_info,
                                 xmedia_svp_yolov8_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((param == XMEDIA_NULL) || (frame_info == XMEDIA_NULL) || (output == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector run get param:%p frame_info:%p, output:%p\n",
                  param, frame_info, output);
        return XMEDIA_FAILURE;
    }
    if ((frame_info->frame.width != param->w) || (frame_info->frame.height != param->h) ||
        (frame_info->frame.addr.y_phy_addr == 0)) {
        SVP_TRACE(MODULE_DBG_ERR, "picture size w:%d  h:%d \n",
                     frame_info->frame.width,
                     frame_info->frame.height);
        return XMEDIA_FAILURE;
    }
    if (frame_info->frame.stride.y_stride == 0) {
        SVP_TRACE(MODULE_DBG_NOTICE, "Frame stride:%d err\n",frame_info->frame.stride.y_stride);
        return XMEDIA_FAILURE;
    }

    ret = npu_run_model(&param->model, frame_info->frame.addr.y_phy_addr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu forward failed, ret=0x%x\n", ret);
        return XMEDIA_FAILURE;
    }

    ret = svp_yolov8_get_detect_result(param, output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu process output failed, ret=0x%x\n", ret);
        return XMEDIA_FAILURE;
    }
/*
    if ((output->target_num != 0) && (param->roi.enable == XMEDIA_TRUE)) {
        ret = svp_get_roi_result(param, output);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "roi filter failed, ret=0x%x\n", ret);
            return XMEDIA_FAILURE;
        }
    }
*/
    return ret;
}
