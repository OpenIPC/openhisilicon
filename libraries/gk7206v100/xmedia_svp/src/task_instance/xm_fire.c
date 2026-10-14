#include "xm_fire.h"

static xmedia_s32 fire_grid_h_array[3] = { 48, 24, 12 };
static xmedia_s32 fire_grid_w_array[3] = { 80, 40, 20 };
static xmedia_s32 fire_stride_array[3] = { 8, 16, 32 };

inline static int clamp(xmedia_float val, int min, int max)
{
    return val > min ? (val < max ? val : max) : min;
}

inline static xmedia_float sigmoid(xmedia_float x) { return 1.0 / (1.0 + expf(-x)); }

inline static xmedia_float unsigmoid(xmedia_float y) { return -1.0 * logf((1.0 / y) - 1.0); }

inline static xmedia_s32 __clip(xmedia_float val, xmedia_float min, xmedia_float max)
{
    xmedia_float f = val <= min ? min : (val >= max ? max : val);
    return f;
}

inline static xmedia_float deqnt_affine_u8_to_f32(xmedia_u8 qnt, xmedia_s32 zp, xmedia_float scale)
{
    return ((xmedia_float)qnt - (xmedia_float)zp) * scale;
}

static void compute_dfl(xmedia_float* tensor, xmedia_float* box)
{
    for (int b = 0; b < 4; b++) {
        xmedia_float exp_t[DFL_LEN];
        xmedia_float exp_sum = 0;
        xmedia_float acc_sum = 0;
        for (int i = 0; i < DFL_LEN; i++) {
            exp_t[i] = exp(tensor[i + b * DFL_LEN]);
            exp_sum += exp_t[i];
        }

        for (int i = 0; i < DFL_LEN; i++) {
            acc_sum += exp_t[i] / exp_sum * i;
        }
        box[b] = acc_sum;
    }
}

static xmedia_u8 qnt_f32_to_affine_u8(xmedia_float f32, xmedia_s32 zp, xmedia_float scale)
{
    xmedia_float dst_val = (f32 / scale) + zp;
    xmedia_u8 res = (xmedia_u8)__clip(dst_val, 0, 255);
    return res;
}

static inline xmedia_s32 cmp_score_fall(const xmedia_void* lsh, const xmedia_void* rsh)
{
    xmedia_svp_yolov8_result* s1 = (xmedia_svp_yolov8_result*)lsh;
    xmedia_svp_yolov8_result* s2 = (xmedia_svp_yolov8_result*)rsh;
    return s1->detect_score > s2->detect_score ? -1 : 1;
}

static xmedia_s32 fire_det_nms(svp_base_yolov8_result* vec_bbox, xmedia_u32 vec_bbox_len,
    xmedia_float threshold, xmedia_u32* count)
{
    xmedia_u32 i;
    xmedia_u32 j;
    xmedia_u32 nms_count = vec_bbox_len;
    qsort(vec_bbox, vec_bbox_len, sizeof(svp_base_yolov8_result), cmp_score_fall);
    for (i = 0; i < vec_bbox_len; i++) {
        if ((vec_bbox[i].detect_score - SVP_EPSILON) <= 0.0) {
            continue;
        }
        for (j = i + 1; j < vec_bbox_len; j++) {
            if ((vec_bbox[j].detect_score - SVP_EPSILON) <= 0.0) {
                continue;
            }
            xmedia_float box_iou = iouv8(&vec_bbox[i], &vec_bbox[j]);
            if (box_iou >= threshold) {
                vec_bbox[j].detect_score = 0.0;
                nms_count--;
            }
        }
    }
    *count = nms_count;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 is_point_in_rect(xmedia_s32 point_index, xmedia_s32 col_size, const xmedia_svp_rect* rect)
{
    xmedia_float x, y;
    y = point_index / col_size;
    x = point_index % col_size;

    // xy normal resize to 640 * 360
    x = x * ((xmedia_float)INPUT_DEFAULT_WIDE / PROTO_WEIGHT); // 640 / 160 = 4
    y = y * ((xmedia_float)INPUT_DEFAULT_HIGH / PROTO_HEIGHT); // 360 / 96 = 3.75
    if (x >= rect->x1 && x < rect->x2 && y >= rect->y1 && y < rect->y2) {
        return 1;
    } else {
        return 0;
    }
}

static xmedia_s32 get_seg_detect_result(svp_fire_context* fire_context, yolov8_detect_param* param,
    xmedia_svp_yolov8_output* result)
{
    xmedia_s32 ret;
    xmedia_u32 index;
    xmedia_u32 seg_size;
    xmedia_u32 idx = 0;
    xmedia_u32 segments_idx = 0;
    xmedia_u32 target_num = param->max_target_num * 3;
    xmedia_float *proto, *segments;

    seg_size = (fire_grid_h_array[0] * fire_grid_w_array[0]) + (fire_grid_h_array[1] * fire_grid_w_array[1]) +
               (fire_grid_h_array[2] * fire_grid_w_array[2]);
    segments = (xmedia_float*)malloc(PROTO_CHANNEL * seg_size * sizeof(xmedia_float));
    proto = (xmedia_float*)malloc(PROTO_CHANNEL * PROTO_HEIGHT * PROTO_WEIGHT * sizeof(xmedia_float));

    for (index = 0; index < LAYER_MIN_NUM; index++) {
        xmedia_s32 grid_h = fire_grid_h_array[index];
        xmedia_s32 grid_w = fire_grid_w_array[index];
        xmedia_s32 stride = fire_stride_array[index];
        xmedia_s32 box_idx = index * 2;
        xmedia_s32 score_idx = index * 2 + 1;
        xmedia_s32 seg_idx = index + 6;

        xmedia_s32 box_zp = param->quanlize[box_idx].zp;
        xmedia_float box_scale = param->quanlize[box_idx].scale;
        xmedia_u8* box_tensor = param->model.output.tensor[box_idx].addr;

        xmedia_s32 score_zp = param->quanlize[score_idx].zp;
        xmedia_float score_scale = param->quanlize[score_idx].scale;
        xmedia_u8* score_tensor = param->model.output.tensor[score_idx].addr;

        xmedia_s32 seg_zp = param->quanlize[seg_idx].zp;
        xmedia_float seg_scale = param->quanlize[seg_idx].scale;
        xmedia_u8* seg_tensor = param->model.output.tensor[seg_idx].addr;

        xmedia_s32 grid_len = grid_h * grid_w;
        xmedia_u8 score_thres_u8 = qnt_f32_to_affine_u8(unsigmoid(param->detect_threshold),
                                   score_zp, score_scale);
        for (xmedia_s32 i = 0; i < grid_h; i++) {
            for (xmedia_s32 j = 0; j < grid_w; j++) {
                xmedia_s32 score_offset = i * grid_w + j;
                xmedia_s32 seg_offset = i * grid_w + j;
                xmedia_s32 max_class_id = -1;
                xmedia_u32 bbox_offset = i * grid_w + j;
                xmedia_u8 max_score = -score_zp;
                xmedia_u8* in_ptr_seg = seg_tensor + seg_offset;
                xmedia_float box[4]; // left right top botom
                xmedia_float before_dfl[DFL_LEN * 4];
                xmedia_float x1, y1, x2, y2;

                if (idx >= target_num) {
                    break;
                }

                for (xmedia_s32 c = 0; c < FIRE_SMOKE_CLASS; c++) {
                    if ((score_tensor[score_offset] > score_thres_u8) && (score_tensor[score_offset] > max_score)) {
                        max_score = score_tensor[score_offset];
                        max_class_id = c;
                    }
                    score_offset += grid_len;
                }
                // for quick filtering through "detect_score"
                if (max_score < score_thres_u8) {
                    continue;
                }
                // compute box
                for (int k = 0; k < PROTO_CHANNEL; k++) {
                    segments[segments_idx] = deqnt_affine_u8_to_f32(in_ptr_seg[k * grid_len],
                        seg_zp, seg_scale);
                    segments_idx++;
                }

                for (int k = 0; k < DFL_LEN * 4; k++) {
                    before_dfl[k] = deqnt_affine_u8_to_f32(box_tensor[bbox_offset], box_zp, box_scale);
                    bbox_offset += grid_len;
                }
                compute_dfl(before_dfl, box);

                x1 = (-box[0] + j + 0.5) * stride;
                y1 = (-box[1] + i + 0.5) * stride;
                x2 = (box[2] + j + 0.5) * stride;
                y2 = (box[3] + i + 0.5) * stride;

                param->bbox[idx].rect.x1 = clamp(x1, 0, param->w);
                param->bbox[idx].rect.y1 = clamp(y1, 0, param->h);
                param->bbox[idx].rect.x2 = clamp(x2, 0, param->w);
                param->bbox[idx].rect.y2 = clamp(y2, 0, param->h);

                param->bbox[idx].detect_score = sigmoid(deqnt_affine_u8_to_f32(max_score, score_zp, score_scale));
                param->bbox[idx].class_type = max_class_id;
                idx++;
            }
        }
    }

    // clear fire_pixel_count
    fire_context->info.fire_pixel_count[fire_context->info.mask_index] = 0;
    fire_context->info.smoke_pixel_count[fire_context->info.mask_index] = 0;

    if (idx == 0) {
        result->target_num = 0;
        fire_context->info.pre_result.target_num = 0;
        CFREE(segments);
        CFREE(proto);
        return XMEDIA_SUCCESS;
    }

    xmedia_u8* input_proto = param->model.output.tensor[9].addr;
    xmedia_s32 zp_proto = param->quanlize[9].zp;
    xmedia_float scale_proto = param->quanlize[9].scale;
    xmedia_u32 nms_idx = 0;
    xmedia_u32 Segments_nms_idx = 0;
    xmedia_u32 fire_num = 0;
    xmedia_u32 smoke_num = 0;

    for (xmedia_s32 i = 0; i < PROTO_CHANNEL * PROTO_HEIGHT * PROTO_WEIGHT; i++) {
        proto[i] = deqnt_affine_u8_to_f32(input_proto[i], zp_proto, scale_proto);
        // proto[i] = input_proto[i] - zp_proto;
    }

    ret = fire_det_nms(param->bbox, idx, param->iou_threshold, &nms_idx);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "fire_det_nms  failed ! \n");
        return ret;
    }

    // get segments and NMS result
    xmedia_float* filterSegments_by_nms = (xmedia_float*)malloc(PROTO_CHANNEL * nms_idx * sizeof(xmedia_float));
    if (filterSegments_by_nms == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "malloc failed !\n");
        CFREE(segments);
        CFREE(proto);
        return XMEDIA_FAILURE;
    }

    for (xmedia_u32 i = 0; i < idx; i++) {
        if (param->bbox[i].detect_score - SVP_EPSILON <= 0) {
            continue;
        }
        // save amount of fire/smoke only one
        if (fire_num == 1 && param->bbox[i].class_type == 0) {
            continue;
        }
        if (smoke_num == 1 && param->bbox[i].class_type == 1) {
            continue;
        }
        for (int k = 0; k < PROTO_CHANNEL; k++) {
            filterSegments_by_nms[Segments_nms_idx] = segments[i * PROTO_CHANNEL + k];
            Segments_nms_idx++;
        }
        result->targets[result->target_num].detect_score = param->bbox[i].detect_score;
        result->targets[result->target_num].rect         = param->bbox[i].rect;
        result->targets[result->target_num].class_type   = param->bbox[i].class_type;
        result->targets[result->target_num].alg_type     = param->type;
        result->target_num += 1;

        if (param->bbox[i].class_type == 0) {
            fire_num++;
        }
        if (param->bbox[i].class_type == 1) {
            smoke_num++;
        }
    }

    // compute the mask (binary matrix) through Matmul
    xmedia_u32 ROWS_A = result->target_num; // n
    xmedia_u32 COLS_A = PROTO_CHANNEL; // 32
    xmedia_u32 COLS_B = PROTO_HEIGHT * PROTO_WEIGHT; // 96 * 160
    xmedia_u8* matmul_out = (xmedia_u8*)malloc(ROWS_A * COLS_B);
    if (matmul_out == XMEDIA_NULL) {
        CFREE(filterSegments_by_nms);
        CFREE(segments);
        CFREE(proto);
        SVP_TRACE(MODULE_DBG_ERR, "malloc failed !\n");

        return XMEDIA_FAILURE;
    }

    for (xmedia_u32 i = 0; i < ROWS_A; i++) {
        for (xmedia_u32 j = 0; j < COLS_B; j++) {
            xmedia_double mat_val = 0;
            for (xmedia_u32 k = 0; k < COLS_A; k++) {
                mat_val += filterSegments_by_nms[i * COLS_A + k] * proto[k * COLS_B + j];
            }
            if (mat_val > 0 && is_point_in_rect(j, PROTO_WEIGHT, &result->targets[i].rect)) {
                if (result->targets[i].class_type == 0) {
                    fire_context->info.fire_pixel_count[fire_context->info.mask_index]++;
                } else if (result->targets[i].class_type == 1) {
                    fire_context->info.smoke_pixel_count[fire_context->info.mask_index]++;
                }
            }
        }
    }

    CFREE(matmul_out);
    CFREE(filterSegments_by_nms);
    CFREE(segments);
    CFREE(proto);

    return XMEDIA_SUCCESS;
}

static xmedia_void detect_firesmoke_get_class_id(xmedia_svp_yolov8_result* detect_result)
{
    if (detect_result->class_type == 0) {
        detect_result->class_type = XMEDIA_SVP_CLASS_TYPE_FIRE;
    } else if (detect_result->class_type == 1) {
        detect_result->class_type = XMEDIA_SVP_CLASS_TYPE_SMOKE;
    } else {
        detect_result->class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
    }
}

static xmedia_s32 ai_cal_fire_luma_by_vgs(const xmedia_video_frame* input_image, xmedia_video_rect* rect,
                                                            xmedia_u32 rect_num, xmedia_u64* luma_single)
{
    xmedia_s32 ret;
    xmedia_s32 vgs_handle;
    xmedia_vgs_frame_info vgs_task;

    ret = xmedia_vgs_init();
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_vgs_init failed !\n");
        return ret;
    }

    ret = xmedia_vgs_create_job(&vgs_handle);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "Vgs begin job fail, Error(%#x)\n", ret);
    }
    memcpy(&(vgs_task.img_in.frame), input_image, sizeof(xmedia_video_frame));
    memcpy(&(vgs_task.img_out.frame), input_image, sizeof(xmedia_video_frame));

    ret = xmedia_vgs_add_task_luma(vgs_handle, &vgs_task, rect, rect_num, luma_single);
    if (ret != XMEDIA_SUCCESS) {
        xmedia_vgs_cancel_job(vgs_handle);
        SVP_TRACE(MODULE_DBG_ERR, "VGS AddLumaTask fail, Error(%#x)\n", ret);

        return ret;
    }

    ret = xmedia_vgs_submit_job(vgs_handle);
    if (XMEDIA_SUCCESS != ret) {
        (xmedia_void) xmedia_vgs_cancel_job(vgs_handle);
        SVP_TRACE(MODULE_DBG_ERR, "vgs submit failed %#x!\n", ret);
        return XMEDIA_FAILURE;
    }

    ret = xmedia_vgs_wait_job(vgs_handle, 2000);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "vgs wait job failed %#x!\n", ret);
        return ret;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ive_thres_proc(xmedia_ive_src_image_s* src_img, xmedia_ive_src_image_s* dst_img,
                                                             xmedia_ive_thresh_ctrl_s* stThreCtrl)
{
    xmedia_s32 ret;
    xmedia_s32 ive_handle;
    xmedia_bool bBlock = XMEDIA_TRUE;
    xmedia_bool bFinish = XMEDIA_FALSE;
    xmedia_bool bInstant = XMEDIA_TRUE;

    ret = xmedia_ive_thresh(&ive_handle, src_img, dst_img, stThreCtrl, bInstant);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "XMEDIA_API_IVE_Thres failed !\n");
    }

    ret = xmedia_ive_query(ive_handle, &bFinish, bBlock);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret) {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &bFinish, bBlock);
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "XMEDIA_API_IVE_Thres failed !\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ive_sub_proc(ive_sub_attr* sub_attr, xmedia_ive_src_image_s* cur_img)
{
    xmedia_s32 ret;
    xmedia_s32 ive_handle;
    xmedia_bool bBlock = XMEDIA_TRUE;
    xmedia_bool bFinish = XMEDIA_FALSE;
    xmedia_bool bInstant = XMEDIA_TRUE;

    ret = xmedia_ive_sub(&ive_handle, &sub_attr->pre_sub_img, cur_img,
              &sub_attr->dst_sub_img, &sub_attr->stSubCtrl, bInstant);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "XMEDIA_API_IVE_Sub failed !\n");
    }

    ret = xmedia_ive_query(ive_handle, &bFinish, bBlock);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret) {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &bFinish, bBlock);
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "XMEDIA_API_IVE_Sub failed !\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ive_init(ive_sub_attr* sub_attr)
{
    xmedia_s32 ret;
    xmedia_s32 output_size;

    sub_attr->dst_sub_img.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C1;
    sub_attr->dst_sub_img.u32width = INPUT_DEFAULT_WIDE;
    sub_attr->dst_sub_img.u32height = INPUT_DEFAULT_HIGH;
    sub_attr->dst_sub_img.au32stride[0] = INPUT_DEFAULT_WIDE;

    sub_attr->pre_sub_img.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C1;
    sub_attr->pre_sub_img.u32width = INPUT_DEFAULT_WIDE;
    sub_attr->pre_sub_img.u32height = INPUT_DEFAULT_HIGH;
    sub_attr->pre_sub_img.au32stride[0] = INPUT_DEFAULT_WIDE;

    sub_attr->dst_thre_img.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C1;
    sub_attr->dst_thre_img.u32width = INPUT_DEFAULT_WIDE;
    sub_attr->dst_thre_img.u32height = INPUT_DEFAULT_HIGH;
    sub_attr->dst_thre_img.au32stride[0] = INPUT_DEFAULT_WIDE;

    sub_attr->smoke_thre_img.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C1;
    sub_attr->smoke_thre_img.u32width = INPUT_DEFAULT_WIDE;
    sub_attr->smoke_thre_img.u32height = INPUT_DEFAULT_HIGH;
    sub_attr->smoke_thre_img.au32stride[0] = INPUT_DEFAULT_WIDE;

    output_size = sub_attr->dst_sub_img.u32width * sub_attr->dst_sub_img.u32height;

    sub_attr->dst_sub_img.au64phyaddr[0] = xmedia_mmz_alloc(XMEDIA_NULL, "ive_dstSubImg", output_size);
    if (sub_attr->dst_sub_img.au64phyaddr[0] == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_mmz_map filed, size %d \n", output_size);
        xmedia_mmz_free(sub_attr->dst_sub_img.au64phyaddr[0]);
        return XMEDIA_FAILURE;
    }

    sub_attr->pre_sub_img.au64phyaddr[0] = xmedia_mmz_alloc(XMEDIA_NULL, "ive_preSubImg2", output_size);
    CHECK_SVP_COND_GOTO_ERROR(sub_attr->pre_sub_img.au64phyaddr[0] == XMEDIA_NULL,
                                        RELEASE_MMZ1, "xmedia_mmz_alloc fail!\n");

    sub_attr->dst_thre_img.au64phyaddr[0] = xmedia_mmz_alloc(XMEDIA_NULL, "ive_fireThres", output_size);
    CHECK_SVP_COND_GOTO_ERROR(sub_attr->dst_thre_img.au64phyaddr[0] == XMEDIA_NULL,
                                         RELEASE_MMZ2, "xmedia_mmz_alloc fail!\n");

    sub_attr->smoke_thre_img.au64phyaddr[0] = xmedia_mmz_alloc(XMEDIA_NULL, "ive_smokeThres", output_size);
    CHECK_SVP_COND_GOTO_ERROR(sub_attr->smoke_thre_img.au64phyaddr[0] == XMEDIA_NULL,
                                           RELEASE_MMZ3, "xmedia_mmz_alloc fail!\n");

    return XMEDIA_SUCCESS;

RELEASE_MMZ3:

    ret = xmedia_mmz_free(sub_attr->dst_thre_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

RELEASE_MMZ2:

    ret = xmedia_mmz_free(sub_attr->pre_sub_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

RELEASE_MMZ1:

    ret = xmedia_mmz_free(sub_attr->dst_sub_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

    return XMEDIA_FAILURE;
}

static xmedia_s32 ive_uninit(ive_sub_attr* sub_attr)
{
    xmedia_s32 ret;

    ret = xmedia_mmz_free(sub_attr->pre_sub_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

    ret = xmedia_mmz_free(sub_attr->dst_sub_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

    ret = xmedia_mmz_free(sub_attr->dst_thre_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

    ret = xmedia_mmz_free(sub_attr->smoke_thre_img.au64phyaddr[0]);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "xmedia_mmz_free  failed ! \n");
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 fire_check_attr(const xmedia_svp_fire_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_attr.detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_attr.detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_attr.iou_threshold, "iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_attr.iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_attr.classifier_threshold,
        "classifier_threshold[%.2f] out of range[0-1] !! \n", task_attr->detect_attr.classifier_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->detect_attr.max_target_num, "max_target_num[%d] out of range[0-50] !! \n",
        task_attr->detect_attr.max_target_num);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->fire_change_ratio,
        "fire_change_ratio[%.2f] out of range[0-1] !! \n", task_attr->fire_change_ratio);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->smoke_change_ratio,
        "smoke_change_ratio[%.2f] out of range[0-1] !! \n", task_attr->smoke_change_ratio);
    if (task_attr->fire_ratio_sum <= 0 || task_attr->fire_ratio_sum >= 10) {
        return XMEDIA_FAILURE;
    }
    SVP_CHECK_U8(task_attr->fire_loss_count, "fire_loss_count[%.d] out of range(0-255) !! \n",
        task_attr->fire_loss_count);
    SVP_CHECK_U8(task_attr->smoke_apear_count, "smoke_apear_count[%.d] out of range(0-255) !! \n",
        task_attr->smoke_apear_count);
    SVP_CHECK_U8(task_attr->smoke_loss_count, "smoke_loss_count[%.d] out of range(0-255) !! \n",
        task_attr->smoke_loss_count);
    return ret;
}

xmedia_s32 fire_init(xmedia_void* context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fire_context* fire_context = (svp_fire_context*)context;
    if (fire_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "fire detect init context err !\n");
        return XMEDIA_FAILURE;
    }

    fire_context->info.fire_info = (yolov8_detect_param*)calloc(1, sizeof(yolov8_detect_param));
    if (fire_context->info.fire_info == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "fire_info calloc error !\n");
        return XMEDIA_FAILURE;
    }

    // 设置默认参数
    set_default_yolov8_detect_param(fire_context->info.fire_info);
    fire_context->info.fire_info->w = INPUT_DEFAULT_WIDE;
    fire_context->info.fire_info->h = INPUT_DEFAULT_HIGH;
    fire_context->info.fire_info->num = DETECTION_MODEL_DOUBLE;
    fire_context->info.fire_info->type = cfg.modules->alg_type;

    if (cfg.modules->load_mode == XMEDIA_SVP_MODEL_FILE &&
        check_file_exist(cfg.modules->pathname) != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "model pathname err!\n");
        CFREE(fire_context->info.fire_info);
        return XMEDIA_FAILURE;
    }

    ret = yolov8_init(fire_context->info.fire_info, cfg.modules);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "fire init detect model failed !\n");
        CFREE(fire_context->info.fire_info);
        return ret;
    }

    fire_context->info.fire_info->private_data = (ive_sub_attr*)calloc(1, sizeof(ive_sub_attr));
    if (fire_context->info.fire_info->private_data == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_NOTICE, "ive_sub_attr calloc failed !\n");
        yolov8_uninit(fire_context->info.fire_info);
        CFREE(fire_context->info.fire_info);
        return XMEDIA_FAILURE;
    }

    ret = ive_init(fire_context->info.fire_info->private_data);
    if (ret != XMEDIA_SUCCESS) {
        CFREE(fire_context->info.fire_info->private_data);
        yolov8_uninit(fire_context->info.fire_info);
        CFREE(fire_context->info.fire_info);
        SVP_TRACE(MODULE_DBG_NOTICE, "ive_init failed ! \n");
        return XMEDIA_FAILURE;
    }

    return ret;
}

xmedia_s32 fire_uninit(xmedia_void* context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fire_context* fire_context = (svp_fire_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(fire_context == XMEDIA_NULL, "fire_uninit context err !\n");
    ret = ive_uninit((ive_sub_attr*)fire_context->info.fire_info->private_data);
    CFREE(fire_context->info.fire_info->private_data);
    if (fire_context->info.fire_info != XMEDIA_NULL) {
        if (fire_context->info.fire_info->model.user_count != XMEDIA_NULL) {
            ret = yolov8_uninit(fire_context->info.fire_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "fire detect deinit failed %#x!\n", ret);
            }
        }
        CFREE(fire_context->info.fire_info);
    }
    return ret;
}

xmedia_s32 fire_set_attr(xmedia_void* context, const xmedia_svp_fire_attr* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fire_context* fire_context = (svp_fire_context*)context;
    yolov8_detect_param* detect_info = fire_context->info.fire_info;

    ret = fire_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    detect_info->detect_threshold = task_attr->detect_attr.detect_threshold;
    detect_info->iou_threshold = task_attr->detect_attr.iou_threshold;
    detect_info->max_target_num = task_attr->detect_attr.max_target_num;
    detect_info->classifier_threshold = task_attr->detect_attr.classifier_threshold;

    fire_context->info.fire_attr.fire_change_ratio = task_attr->fire_change_ratio;
    fire_context->info.fire_attr.fire_loss_count = task_attr->fire_loss_count;
    fire_context->info.fire_attr.fire_ratio_sum = task_attr->fire_ratio_sum;
    fire_context->info.fire_attr.smoke_apear_count = task_attr->smoke_apear_count;
    fire_context->info.fire_attr.smoke_change_ratio = task_attr->smoke_change_ratio;
    fire_context->info.fire_attr.smoke_loss_count = task_attr->smoke_loss_count;
    return ret;
}

xmedia_s32 fire_get_attr(xmedia_void* context, xmedia_svp_fire_attr* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fire_context* fire_context = (svp_fire_context*)context;
    yolov8_detect_param* detect_info = fire_context->info.fire_info;
    task_attr->detect_attr.detect_threshold = detect_info->detect_threshold;
    task_attr->detect_attr.iou_threshold = detect_info->iou_threshold;
    task_attr->detect_attr.max_target_num = detect_info->max_target_num;
    task_attr->detect_attr.classifier_threshold = detect_info->classifier_threshold;
    task_attr->fire_change_ratio = fire_context->info.fire_attr.fire_change_ratio;
    task_attr->fire_loss_count = fire_context->info.fire_attr.fire_loss_count;
    task_attr->fire_ratio_sum = fire_context->info.fire_attr.fire_ratio_sum;
    task_attr->smoke_apear_count = fire_context->info.fire_attr.smoke_apear_count;
    task_attr->smoke_change_ratio = fire_context->info.fire_attr.smoke_change_ratio;
    task_attr->smoke_loss_count = fire_context->info.fire_attr.smoke_loss_count;
    return ret;
}

static xmedia_s32 fire_detect_process(svp_fire_context* fire_context, yolov8_detect_param* param,
                    const xmedia_video_frame_info* input_image, xmedia_svp_yolov8_output* result)
{
    xmedia_s32 ret;

    ret = npu_run_model(&param->model, input_image->frame.addr.y_phy_addr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "fire detector npu forward failed, ret=0x%x\n", ret);
        return XMEDIA_FAILURE;
    }

    ret = get_seg_detect_result(fire_context, param, result);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "fire detector npu process result failed, ret=0x%x\n", ret);
    }
    return ret;
}

xmedia_s32 fire_process(xmedia_void* context, const xmedia_svp_task_input* input, xmedia_svp_yolov8_output* output)
{
    xmedia_s32 ret;
    xmedia_u32 i;
    xmedia_float fire_change_ratio = 0;
    xmedia_float smoke_change_ratio = 0;
    svp_fire_context* fire_context = (svp_fire_context*)context;
    ive_sub_attr* sub_attr = fire_context->info.fire_info->private_data;

    time_t end = time(XMEDIA_NULL);
    if (difftime(end, fire_context->info.fire_update_time) > TIMEOUT_THRES) {
        fire_context->info.fire_alarm = XMEDIA_FALSE;
    }

    if (difftime(end, fire_context->info.smoke_update_time) > TIMEOUT_THRES) {
        fire_context->info.smoke_alarm = XMEDIA_FALSE;
    }

    if (fire_context->info.diff_flag == XMEDIA_FALSE) {
        ret = fire_detect_process(fire_context, fire_context->info.fire_info, input->frame, output);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "detection process error.\n");
            return XMEDIA_FAILURE;
        }
        for (i = 0; i < output->target_num; i++) {
            detect_firesmoke_get_class_id(&output->targets[i]);
        }
    }

    if (fire_context->info.diff_flag == XMEDIA_FALSE) {
        xmedia_float cur_change_ratio = 0;
        xmedia_u32 pre_fire_pixel = fire_context->info.fire_pixel_count
                                        [(fire_context->info.mask_index + FIRE_FRAME_MAX_NUM - 1) % FIRE_FRAME_MAX_NUM];
        xmedia_u32 cur_fire_pixel = fire_context->info.fire_pixel_count
                                        [fire_context->info.mask_index % FIRE_FRAME_MAX_NUM];
        xmedia_u32 pre_smoke_pixel = fire_context->info.smoke_pixel_count
                                        [(fire_context->info.mask_index + FIRE_FRAME_MAX_NUM - 1) % FIRE_FRAME_MAX_NUM];
        xmedia_u32 cur_smoke_pixel = fire_context->info.smoke_pixel_count
                                        [fire_context->info.mask_index % FIRE_FRAME_MAX_NUM];

        fire_context->info.mask_index++;
        fire_context->info.mask_index = fire_context->info.mask_index % FIRE_FRAME_MAX_NUM;

        // disappear
        if (pre_smoke_pixel == cur_smoke_pixel) {
            fire_context->info.smoke_loss_count++;
            if (fire_context->info.smoke_loss_count >= fire_context->info.fire_attr.smoke_loss_count) {
                fire_context->info.smoke_alarm = XMEDIA_FALSE;
                fire_context->info.smoke_bigger_frame = 0;
                fire_context->info.smoke_gray = XMEDIA_FALSE;
                fire_context->info.smoke_frame_count = 0;
            }
        }

        if (cur_smoke_pixel > 0 && pre_smoke_pixel > 0) {
            fire_context->info.smoke_loss_count = 0;
            fire_context->info.smoke_frame_count++;
            if (cur_smoke_pixel > pre_smoke_pixel && pre_smoke_pixel > 0) {
                fire_context->info.smoke_bigger_frame++;
                if (fire_context->info.smoke_frame_count >= fire_context->info.fire_attr.smoke_apear_count &&
                    fire_context->info.smoke_bigger_frame >= fire_context->info.smoke_frame_count * SMOKE_COUNT_TIMES) {
                    fire_context->info.smoke_gray = XMEDIA_TRUE;
                }
            }
        }

        // fire frist disappear
        if (pre_fire_pixel > 0 && cur_fire_pixel == 0) {
            fire_context->info.fire_ratio_sum = 0;
        }

        if (pre_fire_pixel == cur_fire_pixel) {
            fire_context->info.fire_loss_count++;
            if (fire_context->info.fire_loss_count >= fire_context->info.fire_attr.fire_loss_count) {
                fire_context->info.fire_alarm = XMEDIA_FALSE;
                fire_context->info.diff_flag = XMEDIA_FALSE;
            }
        }

        if (pre_fire_pixel > 0 && cur_fire_pixel > 0) {
            cur_change_ratio = abs(cur_fire_pixel - pre_fire_pixel) / fmax(cur_fire_pixel, pre_fire_pixel);
            fire_context->info.fire_ratio_sum += cur_change_ratio;
        }
    }

    // TODO diff
    if (fire_context->info.diff_flag == XMEDIA_FALSE) {
        fire_context->info.diff_flag = XMEDIA_TRUE;
        memcpy(&fire_context->info.pre_result, output, sizeof(xmedia_svp_yolov8_output));
        ret = svp_ive_dma_image_copy(input->frame, &sub_attr->pre_sub_img);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, FRAME_RETURN, "svp_ive_dma_image_copy fail!\n");
    } else {
        fire_context->info.diff_flag = XMEDIA_FALSE;
        if (fire_context->info.pre_result.target_num <= 0) {
            goto FRAME_RETURN;
        }

        // TODO fire ive
        xmedia_ive_image_s cur_img;
        cur_img.au64phyaddr[0] = input->frame->frame.addr.y_phy_addr;
        cur_img.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C1;
        cur_img.u32height = INPUT_DEFAULT_HIGH;
        cur_img.u32width = INPUT_DEFAULT_WIDE;
        cur_img.au32stride[0] = INPUT_DEFAULT_WIDE;
        ret = ive_sub_proc(sub_attr, &cur_img);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, FRAME_RETURN, "ive_sub_proc fail, Error!\n");

        xmedia_ive_thresh_ctrl_s stThreCtrl;
        stThreCtrl.en_mode = XMEDIA_IVE_THRESH_MODE_BINARY;
        stThreCtrl.u8_low_thr = FIRE_THRES;
        stThreCtrl.u8_min_val = THRES_MIN_VAL;
        stThreCtrl.u8_max_val = THRES_MAX_VAL;
        ret = ive_thres_proc(&sub_attr->dst_sub_img, &sub_attr->dst_thre_img, &stThreCtrl);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, FRAME_RETURN, "ive_sub_proc fail, Error!\n");

        xmedia_u32 x1 = fire_context->info.pre_result.targets[0].rect.x1;
        xmedia_u32 x2 = fire_context->info.pre_result.targets[0].rect.x2;
        xmedia_u32 y1 = fire_context->info.pre_result.targets[0].rect.y1;
        xmedia_u32 y2 = fire_context->info.pre_result.targets[0].rect.y2;

        xmedia_video_rect luma_rect;
        luma_rect.x = roundf(x1 / 2) * 2;
        luma_rect.y = roundf(y1 / 2) * 2;
        luma_rect.width = roundf((x2 - x1) / 2) * 2;
        luma_rect.height = roundf((y2 - y1) / 2) * 2;
        if (luma_rect.height < VGS_LUMA_LIMIT) {
            luma_rect.height = VGS_LUMA_LIMIT;
        }

        if (luma_rect.width < VGS_LUMA_LIMIT) {
            luma_rect.width = VGS_LUMA_LIMIT;
        }
        if (luma_rect.x + luma_rect.width > INPUT_DEFAULT_WIDE || luma_rect.y +
            luma_rect.height > INPUT_DEFAULT_HIGH) {
            goto FRAME_RETURN;
        }

        xmedia_u64 rect_luma = 0;
        xmedia_video_frame luma_frame;
        memcpy(&luma_frame, &input->frame->frame, sizeof(xmedia_video_frame));
        luma_frame.addr.y_phy_addr = sub_attr->dst_thre_img.au64phyaddr[0];
        ret = ai_cal_fire_luma_by_vgs(&luma_frame, &luma_rect, 1, &rect_luma);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, FRAME_RETURN, "vgs cal smoke luma fail, Error!\n");

        fire_change_ratio = rect_luma / THRES_MAX_VAL;
        fire_change_ratio = fire_change_ratio / (luma_rect.width * luma_rect.height);

        if (fire_context->info.smoke_gray == XMEDIA_TRUE &&
            fire_context->info.pre_result.target_num >= FIRE_SMOKE_CLASS) {

            stThreCtrl.u8_low_thr = SMOKE_THRES;
            ret = ive_thres_proc(&sub_attr->dst_sub_img, &sub_attr->smoke_thre_img, &stThreCtrl);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, FRAME_RETURN, "ive_sub_proc fail, Error!\n");
            luma_frame.addr.y_phy_addr = sub_attr->smoke_thre_img.au64phyaddr[0];
            luma_frame.height = INPUT_DEFAULT_HIGH;
            luma_frame.width = INPUT_DEFAULT_WIDE;

            ret = ai_cal_fire_luma_by_vgs(&luma_frame, &luma_rect, 1, &rect_luma);
            CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, FRAME_RETURN, "vgs cal smoke luma fail, Error!\n");

            smoke_change_ratio = rect_luma / THRES_MAX_VAL;
            smoke_change_ratio = smoke_change_ratio / (luma_rect.width * luma_rect.height);

            if (smoke_change_ratio > SMOKE_RATIO_THRES) {
                fire_context->info.smoke_alarm = XMEDIA_TRUE;
                fire_context->info.smoke_update_time = time(XMEDIA_NULL);
            }
        }
    }

    if (fire_change_ratio >= fire_context->info.fire_attr.fire_change_ratio &&
        fire_context->info.fire_ratio_sum >= fire_context->info.fire_attr.fire_ratio_sum) {
        fire_context->info.fire_alarm = XMEDIA_TRUE;
        fire_context->info.fire_update_time = time(XMEDIA_NULL);
    }

FRAME_RETURN:
    output->target_num = 0;
    for (xmedia_u32 i = 0; i < fire_context->info.pre_result.target_num; i++) {
        if ((fire_context->info.fire_alarm == XMEDIA_TRUE) &&
            (fire_context->info.pre_result.targets[i].class_type == XMEDIA_SVP_CLASS_TYPE_FIRE)) {
            output->targets[output->target_num] = fire_context->info.pre_result.targets[i];
            output->target_num++;
        }
        if ((fire_context->info.smoke_alarm == XMEDIA_TRUE) &&
            (fire_context->info.pre_result.targets[i].class_type == XMEDIA_SVP_CLASS_TYPE_SMOKE)) {
            output->targets[output->target_num] = fire_context->info.pre_result.targets[i];
            output->target_num++;
        }
    }
    return XMEDIA_SUCCESS;
}
