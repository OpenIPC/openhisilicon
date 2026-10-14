#include "xm_face_recognition.h"

// 人脸
static xmedia_svp_detect_anchors fd_anchors_four = {
    .anchors = {{{6.0f, 7.0f}, {9.0f, 11.0f}, {13.0f, 16.0f}},
               {{18.0f, 23.0f}, {26.0f, 33.0f}, {37.0f, 47.0f}},
               {{54.0f, 67.0f}, {77.0f, 104.0f}, {112.0f, 154.0f}},
               {{174.0f, 238.0f}, {258.0f, 355.0f}, {455.0f, 568.0f}}},
    .layer_num = LAYER_NUM_FOUR,
 };

// 人脸关键点 预设标准坐标
static svp_keypoint_point persets_point = {
    .point_arry = {{38.2946, 51.6963}, {73.5318, 51.5014},
                   {56.0252, 71.7366}, {41.5493, 92.3655},
                   {70.7299, 92.2041}},
};

static xmedia_void svp_get_face_rect(xmedia_svp_yolov5_output *result, xmedia_video_rect *rect,
    xmedia_u32 *rect_num)
{
    xmedia_u32 j = 0;
    xmedia_u32 x1;
    xmedia_u32 y1;
    xmedia_u32 x2;
    xmedia_u32 y2;
    xmedia_u32 face_num = 0;
    xmedia_video_rect *rect_cfg;

    rect_cfg = rect;
    for (j = 0; j < result->target_num; j++) {
        if (face_num >= FR_MAX_CAL_LUMA_TECT_NUM) {
            break;
        }
        // 取整
        x1 = (xmedia_u32)roundf(result->targets[j].rect.x1 / 2) * 2;
        y1 = (xmedia_u32)roundf(result->targets[j].rect.y1 / 2) * 2;
        x2 = (xmedia_u32)roundf(result->targets[j].rect.x2 / 2) * 2;
        y2 = (xmedia_u32)roundf(result->targets[j].rect.y2 / 2) * 2;
        rect_cfg->x = x1;
        rect_cfg->y = y1;
        rect_cfg->width = (abs(x1 - x2));
        rect_cfg->height = (abs(y1 - y2));
        rect_cfg++;
        face_num++;
    }

    *rect_num = face_num;
}

static xmedia_s32 svp_cal_face_luma_by_vgs(const xmedia_video_frame_info *input_image,
    xmedia_video_rect *rect, xmedia_u32 rect_num, xmedia_u64 *luma_single)
{
    xmedia_s32 ret;
    xmedia_s32 handle = -1;
    xmedia_vgs_frame_info vgs_task;
    xmedia_video_rect *rect_cfg;
    xmedia_u64 *luma_single_cfg;

    rect_cfg = rect;
    luma_single_cfg = luma_single;

    memcpy(&(vgs_task.img_in), input_image, sizeof(xmedia_video_frame_info));

    ret = xmedia_vgs_create_job(&handle);
    if (ret != XMEDIA_SUCCESS) {
        xmedia_vgs_cancel_job(handle);
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_vgs_create_job fail, Error(%#x)\n", ret);
        return ret;
    }

    ret = xmedia_vgs_add_task_luma(handle,  &vgs_task, rect_cfg, rect_num, luma_single_cfg);
    if (ret != XMEDIA_SUCCESS) {
        xmedia_vgs_cancel_job(handle);
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_vgs_add_task_luma fail, Error(%#x)\n", ret);
        return ret;
    }

    ret = xmedia_vgs_submit_job(handle);
    if (ret != XMEDIA_SUCCESS) {
        xmedia_vgs_cancel_job(handle);
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_vgs_submit_job fail, Error(%#x)\n", ret);
        return ret;
    }
    ret = xmedia_vgs_wait_job(handle, 2000);
    if (ret != XMEDIA_SUCCESS) {
        xmedia_vgs_cancel_job(handle);
        SVP_TRACE(MODULE_DBG_ERR, "vgs wait job fail, Error(%#x)\n", ret);
        return ret;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_cal_face_luma(const xmedia_video_frame_info *input_image,
    xmedia_svp_yolov5_output *result, xmedia_u64 luma_thres)
{
    xmedia_u32 i = 0;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_video_rect *rect = XMEDIA_NULL;
    xmedia_u32 rect_num = 0;
    xmedia_u64 *rect_luma = XMEDIA_NULL;
    xmedia_svp_yolov5_output tmp_result = {0};

    rect = (xmedia_video_rect *)malloc(FR_MAX_CAL_LUMA_TECT_NUM * sizeof(xmedia_video_rect));
    CHECK_CALLOC_RETURN_ERROR(rect);
    rect_luma = (xmedia_u64 *)malloc(FR_MAX_CAL_LUMA_TECT_NUM * sizeof(xmedia_u64));
    CHECK_CALLOC_GOTO_ERROR(rect_luma, EXIT);

    svp_get_face_rect(result, rect, &rect_num);
    if  (rect_num == 0) {
        CFREE(rect_luma);
        CFREE(rect);
        return ret;
    }

    ret = svp_cal_face_luma_by_vgs(input_image, rect, rect_num, rect_luma);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "vgs cal luma fail, Error\n")

    // 筛选出高亮度的框
    for (i = 0; i < rect_num; i++) {
        if ((rect_luma[i]/(rect[i].width * rect[i].height)) >= luma_thres) {// pick
            memcpy(&tmp_result.targets[tmp_result.target_num], &result->targets[i], sizeof(xmedia_svp_detect_result));
            tmp_result.target_num++;
        }
    }

    memset(result, 0x00, sizeof(xmedia_svp_yolov5_output));
    if (tmp_result.target_num != 0) {
        memcpy(result, &tmp_result, sizeof(xmedia_svp_yolov5_output));
    }

    CFREE(rect_luma);
    CFREE(rect);
    return ret;

EXIT:
    CFREE(rect_luma);
    CFREE(rect);
    return XMEDIA_FAILURE;
}

xmedia_s32 ive_sobel_uninit(ive_sobel_attr *sobel_attr)
{
    if (sobel_attr == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "null ptr, ive_sobel_uninit failed ! \n");
    }

    if (sobel_attr->dst_img.au64viraddr[0] != 0) {
        (xmedia_void)svp_mmz_unmap_and_free(sobel_attr->dst_img.au64phyaddr[0],
                                           (xmedia_void *)(xmedia_ulong)sobel_attr->dst_img.au64viraddr[0]);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 ive_sobel_init(ive_sobel_attr *sobel_attr)
{
    xmedia_s32 ret;
    xmedia_u32 sobel_output_size = 0;
    xmedia_s8 as8Mask[25] = {0, 0, 0, 0, 0,
                             0, -1, 0, 1, 0,
                             0, -2, 0, 2, 0,
                             0, -1, 0, 1, 0,
                             0, 0, 0, 0, 0};

    sobel_attr->stSobelCtrl.en_out_ctrl = XMEDIA_IVE_SOBEL_OUT_CTRL_HOR;
    memcpy(sobel_attr->stSobelCtrl.as8_mask, as8Mask, 25);
    sobel_attr->dst_img.en_type = XMEDIA_IVE_IMAGE_TYPE_S16C1;
    sobel_attr->dst_img.u32width = FR_ADAPT_SOBEL_INPUT;
    sobel_attr->dst_img.u32height = FR_ADAPT_SOBEL_INPUT;
    sobel_attr->dst_img.au32stride[0] = FR_ADAPT_SOBEL_INPUT;
    sobel_attr->sobel_thres = FR_IVE_SOBLE_DEFAULT;
    sobel_output_size = sobel_attr->dst_img.u32width * sobel_attr->dst_img.u32height * 2;
    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "sobel_buf", &sobel_attr->dst_img.au64phyaddr[0],
                                (xmedia_void **)&sobel_attr->dst_img.au64viraddr[0], sobel_output_size);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "svp_mmz_alloc_and_map  failed ! \n");
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 ive_sobel_proc(ive_sobel_attr *sobel_attr, xmedia_u64 src_phyaddr, xmedia_s32 *ave_gradient)
{
    xmedia_s32 ret;
    xmedia_ive_handle ive_handle;
    xmedia_bool bBlock = XMEDIA_TRUE;
    xmedia_bool bFinish = XMEDIA_FALSE;
    xmedia_bool bInstant = XMEDIA_TRUE;
    xmedia_ive_src_image_s src_img;
    xmedia_s16 *p;
    xmedia_s32 sum = 0;
    xmedia_s32 area_size;

    src_img.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C1;
    src_img.u32width = FR_ADAPT_SOBEL_INPUT;
    src_img.u32height = FR_ADAPT_SOBEL_INPUT;
    src_img.au32stride[0] = FR_ADAPT_SOBEL_INPUT;
    src_img.au64phyaddr[0] = src_phyaddr;
    area_size = FR_ADAPT_SOBEL_INPUT * FR_ADAPT_SOBEL_INPUT;

    ret = xmedia_ive_sobel(&ive_handle, &src_img, &sobel_attr->dst_img, NULL, &sobel_attr->stSobelCtrl, bInstant);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "XMEDIA_API_IVE_Sobel failed !\n");
        return XMEDIA_FAILURE;
    }

    ret = xmedia_ive_query(ive_handle, &bFinish, bBlock);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret)
    {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &bFinish, bBlock);
    }
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "XMEDIA_API_IVE_Sobel failed !\n");
        return XMEDIA_FAILURE;
    }

    p = (xmedia_s16 *)(xmedia_ulong)sobel_attr->dst_img.au64viraddr[0];
    for (int i = 0; i < src_img.u32width * src_img.u32height; i++) {
        sum += abs(p[i]);
    }
    *ave_gradient = sum / area_size;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 get_keypoint_result(svp_keypoint_targets *keypoint_targets, xmedia_npu_model* npu_model,
    xmedia_float *keypoint_score)
{
    xmedia_u8 *addr;
    xmedia_s32 i;
    xmedia_float arr[2] = {0};
    xmedia_float result_arr[2] = {0};

    if (keypoint_targets == XMEDIA_NULL || npu_model == XMEDIA_NULL ||
        npu_model->output.tensor[0].addr == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR," param abnormal,  get_keypoint_result failed !\n");
        return XMEDIA_FAILURE;
    }

    addr = (xmedia_u8 *)npu_model->output.tensor[0].addr;
    for(i = 0; i < XMEDIA_SVP_FR_KEYPOINT_NUM; i++) {
        keypoint_targets->keypoint_arry[i].x = dequantize(addr[i * 2], npu_model->output.tensor[0].quant.scale,
                                               npu_model->output.tensor[0].quant.zp);
        keypoint_targets->keypoint_arry[i].y = dequantize(addr[i * 2 + 1], npu_model->output.tensor[0].quant.scale,
                                               npu_model->output.tensor[0].quant.zp);
    }

    addr = (xmedia_u8 *)npu_model->output.tensor[1].addr;
    arr[0] = dequantize(addr[0], npu_model->output.tensor[1].quant.scale, npu_model->output.tensor[1].quant.zp);
    arr[1] = dequantize(addr[1], npu_model->output.tensor[1].quant.scale, npu_model->output.tensor[1].quant.zp);
    softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);
    *keypoint_score = result_arr[0];

    return XMEDIA_SUCCESS;
}


static xmedia_void fr_cal_facepose(svp_keypoint_targets *kp_target)
{
    const xmedia_float PI_VALUE = 3.1415926f;

    xmedia_svp_point eye_l   = kp_target->keypoint_arry[0];
    xmedia_svp_point eye_r   = kp_target->keypoint_arry[1];
    xmedia_svp_point nose    = kp_target->keypoint_arry[2];
    xmedia_float dx_eye    = fmax((eye_r.x  - eye_l.x), 1.0f);
    xmedia_float dy_eye    = fmax((eye_r.y  - eye_l.y), 1.0f);
    xmedia_double angle    = atan(dy_eye / dx_eye);
    xmedia_double alpha    = cos(angle);
    xmedia_double beta     = sin(angle);

    xmedia_float LM_nose_x = nose.x;
    xmedia_float LM_nose_y = nose.y;

    xmedia_float lmx_rot[XMEDIA_SVP_FR_KEYPOINT_NUM] ={0.f};
    xmedia_float lmy_rot[XMEDIA_SVP_FR_KEYPOINT_NUM] ={0.f};

    for(int i=0; i < XMEDIA_SVP_FR_KEYPOINT_NUM; i++){
        xmedia_float LMx = kp_target->keypoint_arry[i].x;
        xmedia_float LMy = kp_target->keypoint_arry[i].y;
        lmx_rot[i] = alpha * LMx + beta * LMy + (1 - alpha) * LM_nose_x / 2 - beta * LM_nose_y / 2;
        lmy_rot[i] = -beta * LMx + alpha * LMy + beta * LM_nose_x / 2 + (1 - alpha) * LM_nose_y / 2;
    }

    xmedia_float dXtot = (lmx_rot[1] - lmx_rot[0] + lmx_rot[4] - lmx_rot[3])/2;
    xmedia_float dYtot = (lmy_rot[3] - lmy_rot[0] + lmy_rot[4] - lmy_rot[1])/2;

    xmedia_float dXnose = (lmx_rot[1] - lmx_rot[2] + lmx_rot[4] - lmx_rot[2])/2;
    xmedia_float dYnose = (lmy_rot[3] - lmy_rot[2] + lmy_rot[4] - lmy_rot[2])/2;

    xmedia_float Xfrontal = 0.f;
    xmedia_float Yfrontal = 0.f;
    // 线性映射变换，将原始数据范围映射到[-90,90]的角度区间
    if(dXtot != 0){
        Xfrontal = -90 + 90 / 0.5 * dXnose / dXtot;
    }
    if(dYtot != 0){
        Yfrontal = -90 + 90 / 0.5 * dYnose / dYtot;
    }

    // Roll: -90 to 90 (0: no rotatdraw_colorion, positive: clock-wise rotation, negative: anti-clock-wise rotation)
    // Yaw: -90 to 90 (0: no rotation, positive: looking left, negative: looking right)
    // Pitch: -90 to 90 (0: no rotation, positive: looking upward, negative: looking downward)
    kp_target->roll     = angle * 180 / PI_VALUE;
    kp_target->yaw      = Xfrontal;
    kp_target->pitch    = Yfrontal;
}

xmedia_s32 face_recognition_process(fr_module_info *module, xmedia_u64 img_src_phy,
    xmedia_svp_fr_output *result, xmedia_u32 idx)
{
    xmedia_s32 ret, i;
    xmedia_u8 *addr;
    xmedia_float tmp = 0.0f;

    ret = npu_run_model(&module->npu_model, img_src_phy);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "recognition run model failed! \n");

    addr = (xmedia_u8 *)module->npu_model.output.tensor[0].addr;
    for (i = 0; i < FACE_RECOGN_RESULT_VECTOR_BZ; i++) {
        result->fr_result[idx].vector[i] = dequantize(addr[i], module->npu_model.output.tensor[0].quant.scale,
                                           module->npu_model.output.tensor[0].quant.zp);
        tmp += (result->fr_result[idx].vector[i] * result->fr_result[idx].vector[i]);
    }

    tmp = (xmedia_float)sqrt((xmedia_float)tmp);
    for (i = 0; i < FACE_RECOGN_RESULT_VECTOR_BZ; i++) {
        result->fr_result[idx].vector[i] /= tmp;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 face_emotion_process(fr_module_info *module, xmedia_u64 img_src_phy, xmedia_s8 *result)
{
    xmedia_s32 ret;
    xmedia_u8 *addr;

    ret = npu_run_model(&module->npu_model, img_src_phy);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "emotion classification run model failed! \n");

    //二分类  反量化 + softmax
    xmedia_float arr[2] = {0};
    xmedia_float result_arr[2] = {0};
    addr = (xmedia_u8 *)module->npu_model.output.tensor[0].addr;
    arr[0] = dequantize(addr[0], module->npu_model.output.tensor[0].quant.scale,
        module->npu_model.output.tensor[0].quant.zp);
    arr[1] = dequantize(addr[1], module->npu_model.output.tensor[0].quant.scale,
        module->npu_model.output.tensor[0].quant.zp);
    softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);

    // 返回1是二分类中类别1，返回-1是二分类中类别2
    *result = result_arr[0] >= result_arr[1] ? 1 : -1;

    return XMEDIA_SUCCESS;
}

xmedia_s32 face_keypoint_process(xmedia_svp_yolov5_output detect_result, svp_fr_context *fr_context,
    svp_keypoint_result_info *keypoint_result, const xmedia_video_frame_info *input_frame)
{
    xmedia_s32 ret, i, j;
    xmedia_u32 c_x1, c_y1, c_x2, c_y2;
    xmedia_u32 w, h, x_center, y_center;
    xmedia_u32 i_x1, i_y1, i_x2, i_y2;
    xmedia_u32 index = 0;
    xmedia_s32 ave_gradient;
    svp_keypoint_targets keypoint_targets = {0};
    xmedia_float one_keypoint_score;
    svp_ive_img input_ive_img = {0};
    svp_ive_img output_ive_img = {0};

    for (i = 0; i < detect_result.target_num; i++) {
        // 追踪限制条件
        c_x1 = (xmedia_u32)roundf(detect_result.targets[i].rect.x1 / 2) * 2;
        c_y1 = (xmedia_u32)roundf(detect_result.targets[i].rect.y1 / 2) * 2;
        c_x2 = (xmedia_u32)roundf(detect_result.targets[i].rect.x2 / 2) * 2;
        c_y2 = (xmedia_u32)roundf(detect_result.targets[i].rect.y2 / 2) * 2;
        w = abs(c_x2 - c_x1);
        h = abs(c_y2 - c_y1);
        x_center = c_x1 + w / 2;
        y_center = c_y1 + h / 2;
        if (w > h) {
            h = w;
        } else {
            w = h;
        }

        i_x1 = STD_MIN(INPUT_DEFAULT_WIDE, STD_MAX(0, x_center - w / 2));
        i_x2 = STD_MIN(INPUT_DEFAULT_WIDE, STD_MAX(0, x_center + w / 2));
        i_y1 = STD_MIN(INPUT_DEFAULT_HIGH, STD_MAX(0, y_center - h / 2));
        i_y2 = STD_MIN(INPUT_DEFAULT_HIGH, STD_MAX(0, y_center + h / 2));

        w = STD_MIN(INPUT_DEFAULT_WIDE, SVP_ALIGN_UP(abs(i_x2 - i_x1), SVP_ALIGN_BYTE));
        h = STD_MIN(INPUT_DEFAULT_HIGH, SVP_ALIGN_UP(abs(i_y2 - i_y1), SVP_ALIGN_BYTE));
        input_ive_img.img_addr = input_frame->frame.addr.y_phy_addr;
        input_ive_img.width = input_frame->frame.width;
        input_ive_img.hight = input_frame->frame.height;

        output_ive_img.img_addr = xmedia_mmz_alloc(XMEDIA_NULL, "svp_crop_buf", w * h * 3 / 2);
        CHECK_SVP_COND_RETURN_ERROR(output_ive_img.img_addr == 0, "svp_crop_outbuf  mmz alloc fail !\n");
        output_ive_img.width = w;
        output_ive_img.hight = h;
        ret = svp_ive_crop(&input_ive_img, &output_ive_img, i_x1, i_y1);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, RELEASE_MMZ, "ive_crop failed !\n");

        xmedia_video_frame_info input_resize_frame = {0};
        xmedia_video_frame_info output_resize_frame1 = {0};
        xmedia_video_frame_info output_resize_frame2 = {0};

        input_resize_frame.frame.width = output_ive_img.width;
        input_resize_frame.frame.height = output_ive_img.hight;
        input_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
        input_resize_frame.frame.stride.y_stride = output_ive_img.width;
        input_resize_frame.frame.stride.c_stride = output_ive_img.width;
        input_resize_frame.frame.addr.y_phy_addr = output_ive_img.img_addr;
        input_resize_frame.frame.addr.c_phy_addr =
            output_ive_img.img_addr + output_ive_img.width * output_ive_img.hight;

        output_resize_frame1.frame.width = FACE_REIZE_OUTPUT_IMG_WIDE;
        output_resize_frame1.frame.height = FACE_RESIZE_OUTUT_IMG_HIGH;
        output_resize_frame1.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
        output_resize_frame1.frame.stride.y_stride = FACE_REIZE_OUTPUT_IMG_WIDE;
        output_resize_frame1.frame.stride.c_stride = FACE_REIZE_OUTPUT_IMG_WIDE;
        output_resize_frame1.frame.addr.y_phy_addr = fr_context->keypoint_phyaddr;
        output_resize_frame1.frame.addr.c_phy_addr =
            fr_context->keypoint_phyaddr + FACE_REIZE_OUTPUT_IMG_WIDE * FACE_RESIZE_OUTUT_IMG_HIGH;
        // 送关键点模型
        ret = svp_vgs_resize(&input_resize_frame, &output_resize_frame1);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, RELEASE_MMZ, "svp_vgs_resize failed !\n");

        output_resize_frame2.frame.width = FR_ADAPT_SOBEL_RESIZE;
        output_resize_frame2.frame.height = FR_ADAPT_SOBEL_RESIZE;
        output_resize_frame2.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
        output_resize_frame2.frame.stride.y_stride = FR_ADAPT_SOBEL_RESIZE;
        output_resize_frame2.frame.stride.c_stride = FR_ADAPT_SOBEL_RESIZE;
        output_resize_frame2.frame.addr.y_phy_addr = fr_context->resize_phyaddr;
        output_resize_frame2.frame.addr.c_phy_addr =
            fr_context->resize_phyaddr + FR_ADAPT_SOBEL_RESIZE * FR_ADAPT_SOBEL_RESIZE;
        // 送sobel
        ret = svp_vgs_resize(&input_resize_frame, &output_resize_frame2);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, RELEASE_MMZ, "svp_vgs_resize failed !\n");

        // 释放抠图的内存
        if (output_ive_img.img_addr != 0) {
            xmedia_mmz_free(output_ive_img.img_addr);
            output_ive_img.img_addr = 0;
        }

        svp_ive_img resize_ive_img = {0};
        svp_ive_img sobel_ive_img = {0};
        resize_ive_img.img_addr = output_resize_frame2.frame.addr.y_phy_addr;
        resize_ive_img.width = output_resize_frame2.frame.width;
        resize_ive_img.hight = output_resize_frame2.frame.height;

        sobel_ive_img.img_addr = fr_context->sobel_phyaddr;
        sobel_ive_img.width = FR_ADAPT_SOBEL_INPUT;
        sobel_ive_img.hight = FR_ADAPT_SOBEL_INPUT;
        // 人脸框去除背景、头发等影响因素，64x64四边分别缩小8，输出为48x48
        ret = svp_ive_crop(&resize_ive_img, &sobel_ive_img, 8, 8);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "ive_crop failed !\n");

        // 计算梯度
        ret = ive_sobel_proc(fr_context->sobel_attr, fr_context->sobel_phyaddr, &ave_gradient);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "ive_sobel_proc failed !\n");

        if (ave_gradient < fr_context->sobel_attr->sobel_thres) {
            continue;
        }
        ret = npu_run_model(&fr_context->keypoint_info->npu_model, fr_context->keypoint_phyaddr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "keypoint run model failed !\n");

        memset(&keypoint_targets, 0x00, sizeof(svp_keypoint_targets));
        ret = get_keypoint_result(&keypoint_targets, &fr_context->keypoint_info->npu_model, &one_keypoint_score);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "get keypoint result failed! \n");

        if (one_keypoint_score < fr_context->keypoint_info->thres) {
            continue;
        }

        index = keypoint_result->after_filter_num;
        // 转换为原图坐标点
        for(j = 0; j < XMEDIA_SVP_FR_KEYPOINT_NUM; j++) {
            keypoint_result->keypoint_targets[index].keypoint_arry[j].x =
                keypoint_targets.keypoint_arry[j].x * w + i_x1;
            keypoint_result->keypoint_targets[index].keypoint_arry[j].y =
                keypoint_targets.keypoint_arry[j].y * h + i_y1;
        }
        keypoint_result->keypoint_targets[index].rect.x1 = i_x1;
        keypoint_result->keypoint_targets[index].rect.x2 = i_x2;
        keypoint_result->keypoint_targets[index].rect.y1 = i_y1;
        keypoint_result->keypoint_targets[index].rect.y2 = i_y2;
        keypoint_result->keypoint_targets[index].tracker_id =detect_result.targets[i].tracker_id;
        keypoint_result->keypoint_targets[index].tracker_age = detect_result.targets[i].tracker_age;

        // 推测人脸姿态
        (xmedia_void)fr_cal_facepose(&keypoint_result->keypoint_targets[index]);

        if (keypoint_result->keypoint_targets[index].roll > fr_context->pose_attr.clockwise_thres ||
            keypoint_result->keypoint_targets[index].roll < -(fr_context->pose_attr.anticlockwise_thres) ||
            keypoint_result->keypoint_targets[index].yaw > fr_context->pose_attr.left_thres ||
            keypoint_result->keypoint_targets[index].yaw < -(fr_context->pose_attr.right_thres) ||
            keypoint_result->keypoint_targets[index].pitch > fr_context->pose_attr.upward_thres ||
            keypoint_result->keypoint_targets[index].pitch < -(fr_context->pose_attr.downward_thres)) {
            continue;
        }

#ifdef FR_DEBUG_SAVE_FILE
        SVP_TRACE(MODULE_DBG_NOTICE, "%d face show 5 key_points in 640x360p face \n", i);
        for (j = 0;j < XMEDIA_SVP_FR_KEYPOINT_NUM; j++) {
            SVP_TRACE(MODULE_DBG_NOTICE, "show %d points (%.2f, %.2f) \n",
                j, keypoint_result->keypoint_targets[index].keypoint_arry[j].x,
                   keypoint_result->keypoint_targets[index].keypoint_arry[j].y);
        }
#endif
        keypoint_result->after_filter_num++;  // 没有被过滤的目标计数
    }

    return XMEDIA_SUCCESS;

RELEASE_MMZ:
    if (output_ive_img.img_addr != 0) {
        xmedia_mmz_free(output_ive_img.img_addr);
        output_ive_img.img_addr = 0;
    }

    return XMEDIA_FAILURE;
}


static xmedia_s32 xmface_recognition_check_attr(const xmedia_svp_fr_attr *task_attr)
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
    SVP_CHECK_ATTR_ENABLE(task_attr->detect_attr.bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");

    return ret;
}

static xmedia_void timeout_reconfirm_once_attr(xmedia_u64 ctime, svp_face_history_result *history_result,
    xmedia_u8 face_attr_cnt_thd, xmedia_u64 timeout)
{
    // 确认状态只重新确认一次，未确认状态忽略
    if ((ctime - history_result->retime) > timeout) {
        history_result->retime = ctime;
        // 表情
        if (history_result->emotion_class >= face_attr_cnt_thd) {
            history_result->emotion_class = face_attr_cnt_thd - 1;
        }
        if (history_result->emotion_class <= -(face_attr_cnt_thd)) {
            history_result->emotion_class = 1 -(face_attr_cnt_thd);
        }
    }
}

xmedia_s32 xmface_recognition_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_svp_modules *tmp_module = XMEDIA_NULL;
    svp_fr_context *fr_context = (svp_fr_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(fr_context == XMEDIA_NULL, "xmface_recognition_init context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(cfg.module_num != FR_MODELS_NUM, "module num err !\n");
    fr_context->cfg.task_type = cfg.task_type;

    // 人脸检测模型
    tmp_module = cfg.modules;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT1, "detect model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS),
        EXIT1, "face detect model pathname err!\n");

    fr_context->detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_GOTO_ERROR(fr_context->detect_info, EXIT1);

    fr_context->detect_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    CHECK_SVP_COND_GOTO_ERROR(fr_context->detect_info->tracklet.tracker == XMEDIA_NULL,
        EXIT2, "tracker calloc failed !\n");

    set_default_yolov5_detect_param(fr_context->detect_info);
    fr_context->detect_info->w = INPUT_DEFAULT_WIDE;
    fr_context->detect_info->h = INPUT_DEFAULT_HIGH;
    fr_context->detect_info->num = DETECTION_MODEL_SINGLE;
    fr_context->detect_info->anchors = fd_anchors_four;

    ret = detect_init(fr_context->detect_info, tmp_module->alg_type,
        tmp_module, fr_context->detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT3, "detect init model failed !\n");

    // 关键点模型
    tmp_module = cfg.modules + 1;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT4, "keypoint model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS),
        EXIT4, "keypoint model pathname err!\n");

    fr_context->keypoint_info = (fr_module_info *)calloc(1, sizeof(fr_module_info));
    CHECK_CALLOC_GOTO_ERROR(fr_context->keypoint_info, EXIT4);

    ret = npu_load_model(tmp_module, &fr_context->keypoint_info->npu_model);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT5, "keypoint model load err!\n");

    *(fr_context->keypoint_info->npu_model.user_count) += 1;
    fr_context->keypoint_info->thres = FR_DEFAULT_KEYPOINT_THRES;

    // 识别模型/分类模型
    tmp_module = cfg.modules + 2;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT6, "recognition model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS),
        EXIT6, "recognition model pathname err!\n");

    fr_context->recognition_info = (fr_module_info *)calloc(1, sizeof(fr_module_info));
    CHECK_CALLOC_GOTO_ERROR(fr_context->recognition_info, EXIT6);

    ret = npu_load_model(tmp_module, &fr_context->recognition_info->npu_model);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT7, "recognition model load err!\n");

    *(fr_context->recognition_info->npu_model.user_count) += 1;

    fr_context->sobel_attr = (ive_sobel_attr*)calloc(1, sizeof(ive_sobel_attr));
    CHECK_CALLOC_GOTO_ERROR(fr_context->sobel_attr, EXIT8);

    ret = ive_sobel_init(fr_context->sobel_attr);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT9, "ive_sobel_init err!\n");
    fr_context->pose_attr.clockwise_thres = FR_DEFAULT_FACE_POSE;
    fr_context->pose_attr.anticlockwise_thres = FR_DEFAULT_FACE_POSE;
    fr_context->pose_attr.left_thres = FR_DEFAULT_FACE_POSE;
    fr_context->pose_attr.right_thres = FR_DEFAULT_FACE_POSE;
    fr_context->pose_attr.upward_thres = FR_DEFAULT_FACE_POSE;
    fr_context->pose_attr.downward_thres = FR_DEFAULT_FACE_POSE;
    fr_context->luma_thres = FR_DEFAULT_LUMA;

    fr_context->keypoint_phyaddr = xmedia_mmz_alloc(XMEDIA_NULL, "svp_keypoint_buf",
                                    FACE_REIZE_OUTPUT_IMG_WIDE * FACE_RESIZE_OUTUT_IMG_HIGH * 3/2);
    CHECK_SVP_COND_GOTO_ERROR(fr_context->keypoint_phyaddr == 0,EXIT10, "svp_keypoint_outbuf  mmz alloc fail !\n");
    fr_context->resize_phyaddr = xmedia_mmz_alloc(XMEDIA_NULL,"svp_resize_buf",
                                    FR_ADAPT_SOBEL_RESIZE * FR_ADAPT_SOBEL_RESIZE * 3/2);
    CHECK_SVP_COND_GOTO_ERROR(fr_context->resize_phyaddr == 0, EXIT11, "svp_resize_outbuf  mmz alloc fail !\n");
    fr_context->sobel_phyaddr = xmedia_mmz_alloc(XMEDIA_NULL,"svp_sobel_buf",
                                    FR_ADAPT_SOBEL_INPUT * FR_ADAPT_SOBEL_INPUT * 3/2);
    CHECK_SVP_COND_GOTO_ERROR(fr_context->sobel_phyaddr == 0, EXIT12, "svp_sobel_outbuf  mmz alloc fail !\n");
    ret = svp_mmz_alloc_and_map(XMEDIA_NULL,"svp_fr_align", &fr_context->align_phyaddr,
        (xmedia_void **)&fr_context->align_viraddr,
        FACE_ALIGN_OUTPUT_IMG_WIDE * FACE_ALIGN_OUTPUT_IMG_HIGH * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT13, "svp_fr_align mmz alloc fail !\n");

    return ret;

EXIT13:
    xmedia_mmz_free(fr_context->sobel_phyaddr);
EXIT12:
    xmedia_mmz_free(fr_context->resize_phyaddr);
EXIT11:
    xmedia_mmz_free(fr_context->keypoint_phyaddr);
EXIT10:
    ive_sobel_uninit(fr_context->sobel_attr);
EXIT9:
    CFREE(fr_context->sobel_attr);
EXIT8:
    npu_unload_model(&fr_context->recognition_info->npu_model);
EXIT7:
    CFREE(fr_context->recognition_info);
EXIT6:
    npu_unload_model(&fr_context->keypoint_info->npu_model);
EXIT5:
    CFREE(fr_context->keypoint_info);
EXIT4:
    detect_deinit(fr_context->detect_info);
EXIT3:
    CFREE(fr_context->detect_info->tracklet.tracker);
EXIT2:
    CFREE(fr_context->detect_info);
EXIT1:
    return XMEDIA_FAILURE;
}

xmedia_s32 xmface_recognition_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fr_context *fr_context = (svp_fr_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(fr_context == XMEDIA_NULL, "xmface_recognition_uninit context err !\n");

    if (fr_context->align_viraddr != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(fr_context->align_phyaddr, fr_context->align_viraddr);
    }

    if (fr_context->sobel_phyaddr != 0) {
        xmedia_mmz_free(fr_context->sobel_phyaddr);
        fr_context->sobel_phyaddr = 0;
    }

    if (fr_context->resize_phyaddr != 0) {
        xmedia_mmz_free(fr_context->resize_phyaddr);
        fr_context->resize_phyaddr = 0;
    }

    if (fr_context->keypoint_phyaddr != 0) {
        xmedia_mmz_free(fr_context->keypoint_phyaddr);
        fr_context->keypoint_phyaddr = 0;
    }

    if (fr_context->sobel_attr != XMEDIA_NULL) {
        ret = ive_sobel_uninit(fr_context->sobel_attr);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "ive_sobel_uninit failed ! \n");
        }
        CFREE(fr_context->sobel_attr);
    }

    if (fr_context->recognition_info != XMEDIA_NULL) {
        if (fr_context->recognition_info->npu_model.user_count != XMEDIA_NULL &&
            *(fr_context->recognition_info->npu_model.user_count) == 1) {
            ret = npu_unload_model(&fr_context->recognition_info->npu_model);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "recognition model failed ! \n");
            }
        }
        if (fr_context->recognition_info->npu_model.user_count != XMEDIA_NULL) {
            *(fr_context->recognition_info->npu_model.user_count) -= 1;
        }
        CFREE(fr_context->recognition_info);
    }

    if (fr_context->keypoint_info != XMEDIA_NULL) {
        if (fr_context->keypoint_info->npu_model.user_count != XMEDIA_NULL &&
            *(fr_context->keypoint_info->npu_model.user_count) == 1) {
            ret = npu_unload_model(&fr_context->keypoint_info->npu_model);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "keypoint model failed ! \n");
            }
        }
        if (fr_context->keypoint_info->npu_model.user_count != XMEDIA_NULL) {
            *(fr_context->keypoint_info->npu_model.user_count) -= 1;
        }
        CFREE(fr_context->keypoint_info);
    }

     if (fr_context->detect_info != XMEDIA_NULL) {
        if (fr_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(fr_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(fr_context->detect_info->tracklet.tracker);
        CFREE(fr_context->detect_info);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 xmface_recognition_set_attr(xmedia_void *context, const xmedia_svp_fr_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fr_context *fr_context = (svp_fr_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(fr_context == XMEDIA_NULL, "xmface_recognition_set_attr context err !\n");

    ret = xmface_recognition_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    fr_context->detect_info->detect_threshold = task_attr->detect_attr.detect_threshold;
    fr_context->detect_info->iou_threshold = task_attr->detect_attr.iou_threshold;
    fr_context->detect_info->max_target_num = task_attr->detect_attr.max_target_num;
    fr_context->detect_info->classifier_threshold = task_attr->detect_attr.classifier_threshold;
    fr_context->detect_info->tracklet.enable = task_attr->detect_attr.bytetrack_enable;

    if ((task_attr->luma_score >= FR_MIN_LUMA) &&
        (task_attr->luma_score <= FR_MAX_LUMA)) {
         fr_context->luma_thres = task_attr->luma_score;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "luma_score failed, use default value\n");
    }

    if ((task_attr->ive_sobel_score > FR_MIN_IVE_SOBLE) &&
        (task_attr->ive_sobel_score <= FR_MAX_IVE_SOBLE)) {
         fr_context->sobel_attr->sobel_thres = task_attr->ive_sobel_score;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "ive_sobel_score failed, use default value\n");
    }

    if (task_attr->fr_pose_attr.clockwise_thres < FR_MIN_FACE_POSE ||
        task_attr->fr_pose_attr.clockwise_thres > FR_MAX_FACE_POSE ||
        task_attr->fr_pose_attr.anticlockwise_thres < FR_MIN_FACE_POSE ||
        task_attr->fr_pose_attr.anticlockwise_thres > FR_MAX_FACE_POSE ||
        task_attr->fr_pose_attr.left_thres < FR_MIN_FACE_POSE ||
        task_attr->fr_pose_attr.left_thres > FR_MAX_FACE_POSE ||
        task_attr->fr_pose_attr.right_thres < FR_MIN_FACE_POSE ||
        task_attr->fr_pose_attr.right_thres > FR_MAX_FACE_POSE ||
        task_attr->fr_pose_attr.upward_thres < FR_MIN_FACE_POSE ||
        task_attr->fr_pose_attr.upward_thres > FR_MAX_FACE_POSE ||
        task_attr->fr_pose_attr.downward_thres < FR_MIN_FACE_POSE ||
        task_attr->fr_pose_attr.downward_thres > FR_MAX_FACE_POSE) {
        fr_context->pose_attr.clockwise_thres = FR_DEFAULT_FACE_POSE;
        fr_context->pose_attr.anticlockwise_thres = FR_DEFAULT_FACE_POSE;
        fr_context->pose_attr.left_thres = FR_DEFAULT_FACE_POSE;
        fr_context->pose_attr.right_thres = FR_DEFAULT_FACE_POSE;
        fr_context->pose_attr.upward_thres = FR_DEFAULT_FACE_POSE;
        fr_context->pose_attr.downward_thres = FR_DEFAULT_FACE_POSE;
        SVP_TRACE(MODULE_DBG_ERR, "fr pose attr failed, use default value\n");
    } else {
        fr_context->pose_attr = task_attr->fr_pose_attr;
    }

    if (task_attr->keypoint_score < 0 || task_attr->keypoint_score > 1) {
        fr_context->keypoint_info->thres = FR_DEFAULT_KEYPOINT_THRES;
    } else {
        fr_context->keypoint_info->thres = task_attr->keypoint_score;
    }

    return ret;
}

xmedia_s32 xmface_recognition_get_attr(xmedia_void *context, xmedia_svp_fr_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_fr_context *fr_context = (svp_fr_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(fr_context == XMEDIA_NULL, "xmface_recognition_get_attr context err !\n");

    task_attr->detect_attr.detect_threshold = fr_context->detect_info->detect_threshold;
    task_attr->detect_attr.iou_threshold = fr_context->detect_info->iou_threshold;
    task_attr->detect_attr.max_target_num = fr_context->detect_info->max_target_num;
    task_attr->detect_attr.classifier_threshold = fr_context->detect_info->classifier_threshold;
    task_attr->detect_attr.bytetrack_enable = fr_context->detect_info->tracklet.enable;
    task_attr->luma_score = fr_context->luma_thres;
    task_attr->ive_sobel_score = fr_context->sobel_attr->sobel_thres;
    task_attr->fr_pose_attr = fr_context->pose_attr;
    task_attr->keypoint_score = fr_context->keypoint_info->thres;

    return ret;
}

xmedia_s32 xmface_recognition_process(xmedia_void *context, const xmedia_svp_task_input *input,
    const xmedia_void *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_svp_yolov5_output detect_result = {0};
    xmedia_svp_yolov5_output detect_result_temp = {0};
    svp_keypoint_result_info keypoint_result = {0};
    xmedia_s32 i, j;
    xmedia_float w_ratio, h_ratio;
    struct timespec tp;
    svp_fr_context *fr_context = (svp_fr_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(fr_context == XMEDIA_NULL, "xmface_recognition_process context err !\n");
    xmedia_void *input_data_big = XMEDIA_NULL;
    xmedia_video_frame_info *small_frame = input->frame;
    xmedia_video_frame_info *big_frame = input->frame + 1;
    CHECK_SVP_COND_RETURN_ERROR(input->frame_num != 2, "input frame num is err !!\n");
    CHECK_SVP_COND_RETURN_ERROR((small_frame == XMEDIA_NULL) || (big_frame == XMEDIA_NULL), "input frame is NULL!\n");

    w_ratio = big_frame->frame.width / small_frame->frame.width;
    h_ratio = big_frame->frame.height / small_frame->frame.height;

    // 人脸检测
    ret = detect_process(fr_context->detect_info, small_frame, &detect_result, fr_context->detect_info->anchors);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "face detect_process err !\n");

    // 人脸追踪
    if (fr_context->detect_info->tracklet.enable == XMEDIA_TRUE) {
        xmedia_svp_tracker_result tracker_result = {0};
        tracker_result.target_num = detect_result.target_num;
        for(i = 0; i < tracker_result.target_num; i++) {
            tracker_result.tracker_single[i].class_type = detect_result.targets[i].class_type;
            tracker_result.tracker_single[i].detect_score = detect_result.targets[i].detect_score;
            tracker_result.tracker_single[i].classfier_score = detect_result.targets[i].classfier_score;
            tracker_result.tracker_single[i].tracker_id = detect_result.targets[i].tracker_id;
            tracker_result.tracker_single[i].tracker_age = detect_result.targets[i].tracker_age;
            tracker_result.tracker_single[i].rect = detect_result.targets[i].rect;
        }
        ret = svp_detect_bytetracker(&tracker_result,
                                     &fr_context->detect_info->tracklet,
                                     &fr_context->detect_info->tarck_id_grow,
                                     fr_context->detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }

        detect_result.target_num = 0;

        for(i = 0; i < tracker_result.target_num; i++) {
            detect_result.targets[detect_result.target_num].alg_type = fr_context->detect_info->type;
            detect_result.targets[detect_result.target_num].class_type = tracker_result.tracker_single[i].class_type;
            detect_result.targets[detect_result.target_num].detect_score =
                tracker_result.tracker_single[i].detect_score;
            detect_result.targets[detect_result.target_num].classfier_score =
                tracker_result.tracker_single[i].classfier_score;
            detect_result.targets[detect_result.target_num].tracker_id = tracker_result.tracker_single[i].tracker_id;
            detect_result.targets[detect_result.target_num].tracker_age = tracker_result.tracker_single[i].tracker_age;
            detect_result.targets[detect_result.target_num].rect = tracker_result.tracker_single[i].rect;
            detect_result.target_num++;
        }
    }

    // 清除符合条件的历史数据
    clock_gettime(CLOCK_MONOTONIC, &tp);
    xmedia_u64 cur_time = tp.tv_sec * FACE_S_TO_MS  + tp.tv_nsec / FACE_NS_TO_MS;
    for (i = 0; i < SVP_MAX_LOST_COUNT * XMEDIA_SVP_MAX_TARGET_NUM; i++) {
        if (fr_context->fva_history_arry[i].used_flag == XMEDIA_TRUE) {
            fr_context->fva_history_arry[i].lost_num++;
            if (fr_context->fva_history_arry[i].lost_num >= SVP_MAX_LOST_COUNT - 1) {
                memset(&fr_context->fva_history_arry[i], 0, sizeof(svp_face_history_result));
                fr_context->fva_history_arry[i].used_flag = XMEDIA_FALSE;
            } else {
                timeout_reconfirm_once_attr(cur_time, &fr_context->fva_history_arry[i],
                    FACE_CONFIRM_COUNT, FACE_TIMEOUT_RECONFIRM);
            }
        }
    }

    if (detect_result.target_num != 0) {
        for (i = 0, j = 0; i < detect_result.target_num; i++) {
            if ((detect_result.targets[i].rect.x2 - detect_result.targets[i].rect.x1) < VGS_LIMIT_BILINEAR_INIMG_W ||
                (detect_result.targets[i].rect.y2 - detect_result.targets[i].rect.y1) < VGS_LIMIT_BILINEAR_INIMG_H ||
               ((detect_result.targets[i].rect.x2 - detect_result.targets[i].rect.x1) *
                (detect_result.targets[i].rect.y2 - detect_result.targets[i].rect.y1)) > FR_MAX_FACE) {
                continue;
            }
            detect_result_temp.targets[j++] = detect_result.targets[i];
            fr_context->fva_history_arry[detect_result.targets[i].tracker_id].lost_num = 0;
            if (fr_context->fva_history_arry[detect_result.targets[i].tracker_id].used_flag == XMEDIA_FALSE) {
                fr_context->fva_history_arry[detect_result.targets[i].tracker_id].retime = cur_time;
            }
            fr_context->fva_history_arry[detect_result.targets[i].tracker_id].used_flag = XMEDIA_TRUE;
        }
        detect_result_temp.target_num = j;
        memcpy(&detect_result, &detect_result_temp, sizeof(xmedia_svp_yolov5_output));
    }

    if (detect_result.target_num == 0) {
        return XMEDIA_SUCCESS;
    }

    // 人脸亮度过滤
    ret = svp_cal_face_luma(small_frame, &detect_result, fr_context->luma_thres);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "svp_cal_face_luma error !\n");

    // 关键点检测
    ret = face_keypoint_process(detect_result, fr_context, &keypoint_result, small_frame);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "keypoint process failed !\n");
    if (keypoint_result.after_filter_num == 0) {
        return XMEDIA_SUCCESS;
    }

    // 仿射变换
    input_data_big = (xmedia_void*)xmedia_mmz_map(big_frame->frame.addr.y_phy_addr,
                     (big_frame->frame.stride.y_stride) *(big_frame->frame.height) * 3 / 2, XMEDIA_TRUE);
    CHECK_SVP_COND_RETURN_ERROR(input_data_big == XMEDIA_NULL, "process input data mmap failed\n");

    svp_warpaffine_info warpaffine_info = {0};
    for (i = 0; i < keypoint_result.after_filter_num; i++) {
        for (j = 0; j < XMEDIA_SVP_FR_KEYPOINT_NUM; j++) {
            warpaffine_info.warpaffine_pairpack[i].keypoints[2 * j] =
                keypoint_result.keypoint_targets[i].keypoint_arry[j].x * w_ratio;
            warpaffine_info.warpaffine_pairpack[i].keypoints[((2 * j) + 1)] =
                keypoint_result.keypoint_targets[i].keypoint_arry[j].y * h_ratio;
            warpaffine_info.warpaffine_pairpack[i].targetpoints[2 * j] = persets_point.point_arry[j].x;
            warpaffine_info.warpaffine_pairpack[i].targetpoints[((2 * j) + 1)] = persets_point.point_arry[j].y;

#ifdef FR_DEBUG_SAVE_FILE
            SVP_TRACE(MODULE_DBG_NOTICE, "1080p %d big face %d key_point (%f, %f)\n", i, j,
                warpaffine_info.warpaffine_pairpack[i].keypoints[2 * j],
                warpaffine_info.warpaffine_pairpack[i].keypoints[((2 * j) + 1)]);
#endif
        }
    }

    C_ALG_IMAGE_INFO src_img = {0};
    C_ALG_IMAGE_INFO dst_img = {0};

    src_img.image_buffer = (unsigned char *)input_data_big;
    src_img.image_width = big_frame->frame.width;
    src_img.image_height = big_frame->frame.height;
    src_img.image_stride = big_frame->frame.width;

    dst_img.image_buffer = (unsigned char *)fr_context->align_viraddr;
    dst_img.image_width = FACE_ALIGN_OUTPUT_IMG_WIDE;
    dst_img.image_height = FACE_ALIGN_OUTPUT_IMG_HIGH;
    dst_img.image_stride = FACE_ALIGN_OUTPUT_IMG_WIDE;

    xmedia_u32 idx = 0;
    if (fr_context->cfg.task_type == XMEDIA_SVP_TASK_FACE_RECOGNITON) {
        xmedia_svp_fr_output* fr_output = (xmedia_svp_fr_output*)output;
        for (i = 0; i < keypoint_result.after_filter_num; i++) {
            // 人脸识别
            C_ALG_RESULT alg_result = alg_api_face_feature_warpaffine(&src_img, &dst_img,
                                 (xmedia_float *)&warpaffine_info.warpaffine_pairpack[i].targetpoints,
                                 (xmedia_float *)&warpaffine_info.warpaffine_pairpack[i].keypoints);
            CHECK_SVP_COND_GOTO_ERROR(alg_result != 0, EXIT, "feature_warpaffine failed, error:%d !\n", alg_result);

            ret = face_recognition_process(fr_context->recognition_info, fr_context->align_phyaddr, fr_output, idx);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "get_recognition_result fail! \n");
            } else {
                fr_output->fr_result[idx].rect.x1 = keypoint_result.keypoint_targets[i].rect.x1;
                fr_output->fr_result[idx].rect.y1 = keypoint_result.keypoint_targets[i].rect.y1;
                fr_output->fr_result[idx].rect.x2 = keypoint_result.keypoint_targets[i].rect.x2;
                fr_output->fr_result[idx].rect.y2 = keypoint_result.keypoint_targets[i].rect.y2;
                fr_output->fr_result[idx].fr_tracker_id = keypoint_result.keypoint_targets[i].tracker_id;
                for(j = 0; j < XMEDIA_SVP_FR_KEYPOINT_NUM; j++){
                    fr_output->fr_result[idx].keypoint_arry[j].x =
                        keypoint_result.keypoint_targets[i].keypoint_arry[j].x;
                    fr_output->fr_result[idx].keypoint_arry[j].y =
                        keypoint_result.keypoint_targets[i].keypoint_arry[j].y;
                }
                idx++;
            }
        }
        fr_output->face_num = idx;
    } else if (fr_context->cfg.task_type == XMEDIA_SVP_TASK_EMOTION_CLASSIFITION) {
        xmedia_svp_face_attribute_output* attr_output = (xmedia_svp_face_attribute_output*)output;
        xmedia_s8 classify_result = 0;
        for (i = 0; i < keypoint_result.after_filter_num; i++) {
            // 人脸表情分类
            xmedia_s32 tracker_id = keypoint_result.keypoint_targets[i].tracker_id;
            if (abs(fr_context->fva_history_arry[tracker_id].emotion_class) < FACE_CONFIRM_COUNT) {
                C_ALG_RESULT alg_result = alg_api_face_feature_warpaffine(&src_img, &dst_img,
                                 (xmedia_float *)&warpaffine_info.warpaffine_pairpack[i].targetpoints,
                                 (xmedia_float *)&warpaffine_info.warpaffine_pairpack[i].keypoints);
                CHECK_SVP_COND_GOTO_ERROR(alg_result != 0, EXIT, "feature_warpaffine failed, error:%d !\n", alg_result);

                ret = face_emotion_process(fr_context->recognition_info, fr_context->align_phyaddr, &classify_result);
                if (ret != XMEDIA_SUCCESS) {
                    SVP_TRACE(MODULE_DBG_ERR, "get_emotion_result fail! \n");
                }
                fr_context->fva_history_arry[tracker_id].emotion_class += classify_result;
            }

            // 属性赋值
            if (fr_context->fva_history_arry[tracker_id].emotion_class >= FACE_CONFIRM_COUNT) {
                attr_output->attribute_result[i].emotion_class = XMEDIA_SVP_CLASS_EMOTION_SMILE;
            } else if (abs(fr_context->fva_history_arry[tracker_id].emotion_class) < FACE_CONFIRM_COUNT) {
                attr_output->attribute_result[i].emotion_class = XMEDIA_SVP_CLASS_EMOTION_UNKNOWN;
            } else {
                attr_output->attribute_result[i].emotion_class = XMEDIA_SVP_CLASS_EMOTION_NORMAL;
            }
            attr_output->attribute_result[idx].rect.x1 = keypoint_result.keypoint_targets[i].rect.x1;
            attr_output->attribute_result[idx].rect.y1 = keypoint_result.keypoint_targets[i].rect.y1;
            attr_output->attribute_result[idx].rect.x2 = keypoint_result.keypoint_targets[i].rect.x2;
            attr_output->attribute_result[idx].rect.y2 = keypoint_result.keypoint_targets[i].rect.y2;
            attr_output->attribute_result[idx].face_tracker_id = tracker_id;
            for(j = 0; j < XMEDIA_SVP_FR_KEYPOINT_NUM; j++){
                attr_output->attribute_result[idx].keypoint_arry[j].x =
                    keypoint_result.keypoint_targets[i].keypoint_arry[j].x;
                attr_output->attribute_result[idx].keypoint_arry[j].y =
                    keypoint_result.keypoint_targets[i].keypoint_arry[j].y;
            }
            idx++;
        }
        attr_output->face_num = idx;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "task type are not supported!!!");
    }

    if (input_data_big != XMEDIA_NULL) {
        ret  = xmedia_mmz_unmap(input_data_big);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "input_img_big mmz unmap fail\n");
        }
    }

    return XMEDIA_SUCCESS;

EXIT:

    if (input_data_big != XMEDIA_NULL) {
        ret  = xmedia_mmz_unmap(input_data_big);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "input_img_big mmz unmap fail\n");
        }
    }
    return XMEDIA_FAILURE;
}