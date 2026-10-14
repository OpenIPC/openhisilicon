#include "svp_comm.h"

xmedia_void softmax(xmedia_float *arr, xmedia_s32 size, xmedia_float *result_arr)
{

    xmedia_double sum = 0.0;

    for (xmedia_s32 i = 0; i < size; i++) {
        sum += exp(arr[i]);
    }

    for (xmedia_s32 i = 0; i < size; i++) {
        result_arr[i] = exp(arr[i]) / sum;
    }
}

xmedia_s32 svp_vgs_resize(const xmedia_video_frame_info *input_frame, xmedia_video_frame_info *output_frame)
{
    xmedia_s32 ret;
    xmedia_s32 vgs_handle;
    xmedia_vgs_frame_info vgs_task;

    memset(&vgs_task, 0, sizeof(vgs_task));
    ret = xmedia_vgs_init();
    if(ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_vgs_init failed !\n");
        return ret;
    }

    ret = xmedia_vgs_create_job(&vgs_handle);
    if (XMEDIA_SUCCESS != ret) {
        SVP_TRACE(MODULE_DBG_ERR, "XMEDIA_API_VGS_BeginJob failed %#x!\n", ret);
        return XMEDIA_FAILURE;
    }

    output_frame->mod_id = MOD_ID_USER;
    output_frame->pool_id = input_frame->pool_id;
    output_frame->frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    output_frame->frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
    output_frame->frame.bit_width = XMEDIA_VIDEO_DATA_WIDTH_8;
    memcpy(&vgs_task.img_in, input_frame, sizeof(xmedia_video_frame_info));
    memcpy(&vgs_task.img_out, output_frame, sizeof(xmedia_video_frame_info));

    ret = xmedia_vgs_add_task_scale(vgs_handle, &vgs_task, XMEDIA_VIDEO_SCALE_MODE_BILINEAR);
    if (XMEDIA_SUCCESS != ret) {
        (xmedia_void)xmedia_vgs_cancel_job(vgs_handle);
        SVP_TRACE(MODULE_DBG_ERR, "XMEDIA_API_VGS_AddScaleTask failed %#x!\n", ret);
        return XMEDIA_FAILURE;
    }

    ret = xmedia_vgs_submit_job(vgs_handle);
    if (XMEDIA_SUCCESS != ret) {
        (xmedia_void)xmedia_vgs_cancel_job(vgs_handle);
        SVP_TRACE(MODULE_DBG_ERR, "vgs submit failed %#x!\n", ret);
        return XMEDIA_FAILURE;
    }

    ret = xmedia_vgs_wait_job(vgs_handle, 2000); // 2000ms超时时间
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "vgs wait job failed %#x!\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_ive_crop(const svp_ive_img *src_img, svp_ive_img *dst_img, xmedia_u32 w_offset, xmedia_u32 h_offset)
{
    xmedia_s32 ret;
    xmedia_ive_handle ive_handle;
    xmedia_ive_data_s     st_src_dma;
    xmedia_ive_dst_data_s st_dst_dma;
    xmedia_ive_dma_ctrl_s dma_ctrl;
    xmedia_bool b_instant = XMEDIA_FALSE;
    xmedia_bool b_block = XMEDIA_TRUE;
    xmedia_bool b_finish = XMEDIA_FALSE;

    // 向上取整，保证地址偏移量2对齐
    w_offset = SVP_ALIGN_UP(w_offset, 2);

    // dma y
    st_src_dma.u32width = dst_img->width;
    st_src_dma.u32height = dst_img->hight;
    st_src_dma.u32stride = SVP_ALIGN_UP(src_img->width, SVP_ALIGN_BYTE);
    st_src_dma.u64phyaddr = src_img->img_addr + (w_offset + h_offset * st_src_dma.u32stride);
    st_src_dma.u64viraddr = 0;

    st_dst_dma.u32width = dst_img->width;
    st_dst_dma.u32height = dst_img->hight;
    st_dst_dma.u32stride = SVP_ALIGN_UP(dst_img->width, SVP_ALIGN_BYTE);
    st_dst_dma.u64phyaddr = dst_img->img_addr;
    st_dst_dma.u64viraddr = 0;

    dma_ctrl.en_mode = XMEDIA_IVE_DMA_MODE_DIRECT_COPY;
    b_instant = XMEDIA_TRUE;
    ret = xmedia_ive_dma(&ive_handle, &st_src_dma, &st_dst_dma, &dma_ctrl, b_instant);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_ive_dma failed !\n");
        return -1;
    }

    ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret)
    {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    }

    xmedia_ive_data_s     st_src_dma_2;
    xmedia_ive_dst_data_s st_dst_dma_2;
    // dma uv
    st_src_dma_2.u32width = dst_img->width;
    st_src_dma_2.u32height = dst_img->hight / 2;
    st_src_dma_2.u32stride = SVP_ALIGN_UP(src_img->width, SVP_ALIGN_BYTE);
    st_src_dma_2.u64phyaddr = src_img->img_addr + (st_src_dma_2.u32stride * src_img->hight) +
        (w_offset + h_offset / 2 * st_src_dma.u32stride);
    st_src_dma_2.u64viraddr = 0;

    st_dst_dma_2.u32width = dst_img->width;
    st_dst_dma_2.u32height = dst_img->hight / 2;
    st_dst_dma_2.u32stride = SVP_ALIGN_UP(dst_img->width, SVP_ALIGN_BYTE);
    st_dst_dma_2.u64phyaddr = dst_img->img_addr + (st_dst_dma_2.u32stride * dst_img->hight);
    st_dst_dma_2.u64viraddr = 0;

    dma_ctrl.en_mode = XMEDIA_IVE_DMA_MODE_DIRECT_COPY;
    b_instant = XMEDIA_TRUE;
    ret = xmedia_ive_dma(&ive_handle, &st_src_dma_2, &st_dst_dma_2, &dma_ctrl, b_instant);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_ive_dma failed !\n");
        return -1;
    }

    ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret)
    {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_ive_dma_image_copy(const xmedia_video_frame_info *pst_frame_info, xmedia_ive_image_s *pst_dst)
{
    xmedia_s32 s32_ret;
    xmedia_ive_handle h_ive_handle;
    xmedia_ive_src_data_s st_src_data;
    xmedia_ive_dst_data_s st_dst_data;
    xmedia_ive_dma_ctrl_s st_ctrl;
    xmedia_bool b_finish = XMEDIA_FALSE;
    xmedia_bool b_block = XMEDIA_TRUE;
    st_ctrl.en_mode = XMEDIA_IVE_DMA_MODE_DIRECT_COPY;

    st_src_data.u64phyaddr = pst_frame_info->frame.addr.y_phy_addr;
    st_src_data.u32width = pst_frame_info->frame.width;
    st_src_data.u32height = pst_frame_info->frame.height;
    st_src_data.u32stride = SVP_ALIGN_UP(pst_frame_info->frame.width, SVP_ALIGN_BYTE);

    st_dst_data.u64phyaddr = pst_dst->au64phyaddr[0];
    st_dst_data.u32width = pst_dst->u32width;
    st_dst_data.u32height = pst_dst->u32height;
    st_dst_data.u32stride = SVP_ALIGN_UP(pst_dst->u32width, SVP_ALIGN_BYTE);

    s32_ret = xmedia_ive_dma(&h_ive_handle, &st_src_data, &st_dst_data, &st_ctrl, XMEDIA_TRUE);
    if (s32_ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "Error(%#x),xmedia_ive_dma failed!\n", s32_ret);
        return XMEDIA_FAILURE;
    }
    s32_ret = xmedia_ive_query(h_ive_handle, &b_finish, b_block);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == s32_ret) {
        usleep(100);
        s32_ret = xmedia_ive_query(h_ive_handle, &b_finish, b_block);
    }
    if (s32_ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "Error(%#x),xmedia_ive_query failed!\n", s32_ret);
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}


xmedia_s32 svp_ive_padding(svp_ive_img *src_img, svp_ive_img *dst_img, xmedia_u32 right, xmedia_u32 top)
{
    xmedia_s32 ret;
    xmedia_ive_handle ive_handle;
    xmedia_ive_dma_ctrl_s dma_ctrl;
    xmedia_bool b_instant = XMEDIA_FALSE;
    xmedia_bool b_block = XMEDIA_TRUE;
    xmedia_bool b_finish = XMEDIA_FALSE;

    // 向上取整，保证地址偏移量2对齐
    right = SVP_ALIGN_UP(right, 2);

    // dma y
    xmedia_ive_data_s     st_src_dma;
    xmedia_ive_dst_data_s st_dst_dma;

    st_src_dma.u32width  = src_img->width;
    st_src_dma.u32height = src_img->hight;
    st_src_dma.u32stride  = SVP_ALIGN_UP(src_img->width, SVP_ALIGN_BYTE);
    st_src_dma.u64phyaddr = src_img->img_addr;
    st_src_dma.u64viraddr = 0;

    st_dst_dma.u32width  = src_img->width;
    st_dst_dma.u32height = src_img->hight;
    st_dst_dma.u32stride = SVP_ALIGN_UP(dst_img->width, SVP_ALIGN_BYTE);
    st_dst_dma.u64phyaddr = dst_img->img_addr + (top * st_dst_dma.u32stride + right);
    st_dst_dma.u64viraddr = 0;

    dma_ctrl.en_mode = XMEDIA_IVE_DMA_MODE_DIRECT_COPY;
    b_instant = XMEDIA_TRUE;
    ret = xmedia_ive_dma(&ive_handle, &st_src_dma, &st_dst_dma, &dma_ctrl, b_instant);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_ive failed!\n");
        return XMEDIA_FAILURE;
    }

    ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret)
    {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    }

    // dma uv
    xmedia_ive_data_s     st_src_dma_2;
    xmedia_ive_dst_data_s st_dst_dma_2;

    st_src_dma_2.u32width  = src_img->width;
    st_src_dma_2.u32height = src_img->hight / 2;
    st_src_dma_2.u32stride  = SVP_ALIGN_UP(src_img->width, SVP_ALIGN_BYTE);
    st_src_dma_2.u64phyaddr = src_img->img_addr + (src_img->hight * st_src_dma_2.u32stride);
    st_src_dma_2.u64viraddr = 0;

    st_dst_dma_2.u32width  = src_img->width;
    st_dst_dma_2.u32height = src_img->hight / 2;
    st_dst_dma_2.u32stride = SVP_ALIGN_UP(dst_img->width, SVP_ALIGN_BYTE);
    st_dst_dma_2.u64phyaddr = dst_img->img_addr + (dst_img->hight * st_dst_dma_2.u32stride) +
        (right + top / 2 * st_dst_dma_2.u32stride);
    st_dst_dma_2.u64viraddr = 0;

    dma_ctrl.en_mode = XMEDIA_IVE_DMA_MODE_DIRECT_COPY;
    b_instant = XMEDIA_TRUE;
    ret = xmedia_ive_dma(&ive_handle, &st_src_dma_2, &st_dst_dma_2, &dma_ctrl, b_instant);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_ive failed!\n");
        return XMEDIA_FAILURE;
    }

    ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret)
    {
        usleep(100);
        ret = xmedia_ive_query(ive_handle, &b_finish, b_block);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_void svp_cal_venc_info(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                            xmedia_venc_svc_rect_info *obj_array,
                                            const xmedia_venc_chn_attr *pstchnattr,
                                            const xmedia_svp_task_input *input_image)
{
    xmedia_s32 i, j;
    xmedia_u32 x, y, w, h;

    for(i = 0, j = 0;i < result->target_num; i++) {
        if (result->targets[i].class_type == XMEDIA_SVP_CLASS_TYPE_PERSON) {
            obj_array->detect_type[j] = SVC_RECT_TYPE_PEOPLE;
        } else if (result->targets[i].class_type == XMEDIA_SVP_CLASS_TYPE_FACE) {
            obj_array->detect_type[j] = SVC_RECT_TYPE_FACE;
        } else {
            continue;
        }

        w = (xmedia_u32)fabs(result->targets[i].rect.x1 - result->targets[i].rect.x2);
        h = (xmedia_u32)fabs(result->targets[i].rect.y1 - result->targets[i].rect.y2);
        x = (xmedia_u32)(STD_MIN(fabs(result->targets[i].rect.x1), fabs(result->targets[i].rect.x2)));
        y = (xmedia_u32)(STD_MIN(fabs(result->targets[i].rect.y1), fabs(result->targets[i].rect.y2)));

        obj_array->rect_attr[j].width =  w * (pstchnattr->venc_attr.pic_width / param->w);
        obj_array->rect_attr[j].height = h * (pstchnattr->venc_attr.pic_height / param->h);
        obj_array->rect_attr[j].x =      x * (pstchnattr->venc_attr.pic_width / param->w);
        obj_array->rect_attr[j].y =      y * (pstchnattr->venc_attr.pic_height / param->h);
        j++;
    }

    obj_array->rect_num = result->target_num;
    obj_array->base_resolution.width = pstchnattr->venc_attr.pic_width;
    obj_array->base_resolution.height = pstchnattr->venc_attr.pic_height;
    obj_array->pts = input_image->frame->frame.pts;
    return;
}

xmedia_s32 svp_send_info_to_venc(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                            const xmedia_svp_task_input *input_image)
{
    xmedia_venc_svc_rect_info venc_rect;
    xmedia_venc_chn_attr chn_attr;
    xmedia_s32 venc_chn;
    xmedia_s32 ret;

    for (venc_chn = 0; venc_chn < XMEDIA_SVP_MAX_VENC_CHN_NUM; venc_chn++) {
        if (param->smart_venc_array[venc_chn] == XMEDIA_TRUE) {
            ret = xmedia_venc_get_chn_attr(venc_chn, &chn_attr);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_NOTICE, "VENC_CHN_GETCHNATTR fail! ret=0x%x \n", ret);
                return  XMEDIA_FAILURE;
            }
            svp_cal_venc_info(param, result, &venc_rect, &chn_attr, input_image);
            ret = xmedia_venc_send_svc_region(venc_chn, &venc_rect);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_NOTICE, "VENC_CHN_SEND_IVPREGION fail! ret=0x%x \n", ret);
                return  XMEDIA_FAILURE;
            }
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_void svp_cut_rect(xmedia_svp_yolov5_output *result, svp_rect_cut_coef *rect_cut_coef,
                                xmedia_video_rect *rect, xmedia_u32 *rect_num,
                                xmedia_u64 *size_total, xmedia_u8 type)
{
    xmedia_u32 j = 0;
    xmedia_u32 x = 0;
    xmedia_u32 y = 0;
    xmedia_u32 w = 0;
    xmedia_u32 h = 0;
    xmedia_u64 face_size_total = 0;
    xmedia_u32 face_num = 0;
    xmedia_video_rect *rect_cfg;

    rect_cfg = rect;
    for (j = 0; j < result->target_num; j++) {
        if (result->targets->class_type != type) {
            continue;
        }
        if (face_num >= SVP_MAX_CAL_LUMA_RECT_NUM) {
            break;
        }
        w = (xmedia_u32)(fabs(result->targets[j].rect.x1 - result->targets[j].rect.x2));
        h = (xmedia_u32)(fabs(result->targets[j].rect.y1 - result->targets[j].rect.y2));
        x = (xmedia_u32)result->targets[j].rect.x1;
        y = (xmedia_u32)result->targets[j].rect.y1;

        if ((w < SVP_MIN_RECT_WIDTH) || (h < SVP_MIN_RECT_HEIGHT)) {
            continue;
        }

        rect_cfg->x = ((x + w * rect_cut_coef->x_coef / SVP_FACE_RECT_BASE_COEF)
            >> SVP_MOV_ACCURACY_ONE) << SVP_MOV_ACCURACY_ONE;
        rect_cfg->y = ((y + h * rect_cut_coef->y_coef / SVP_FACE_RECT_BASE_COEF)
            >> SVP_MOV_ACCURACY_ONE) << SVP_MOV_ACCURACY_ONE;
        rect_cfg->width = ((w * rect_cut_coef->w_coef / SVP_FACE_RECT_BASE_COEF)
            >> SVP_MOV_ACCURACY_ONE) << SVP_MOV_ACCURACY_ONE;
        rect_cfg->height = ((h * rect_cut_coef->h_coef / SVP_FACE_RECT_BASE_COEF)
            >> SVP_MOV_ACCURACY_ONE) << SVP_MOV_ACCURACY_ONE;

        face_size_total += rect_cfg->width * rect_cfg->height;
        rect_cfg++;
        face_num++;
    }
    *size_total = face_size_total;
    *rect_num = face_num;
}

static xmedia_void svp_get_cal_luma_coef(xmedia_u8 type, svp_rect_cut_coef *rect_cut_coef,
                                                    xmedia_svp_alg_type *alg_type)
{
    if (type == SVP_MODEL_TYPE_FACE) {
        rect_cut_coef->x_coef = SVP_FACE_RECT_FD_X_COEF;
        rect_cut_coef->y_coef = SVP_FACE_RECT_FD_Y_COEF;
        rect_cut_coef->w_coef = SVP_FACE_RECT_FD_W_COEF;
        rect_cut_coef->h_coef = SVP_FACE_RECT_FD_H_COEF;
        *alg_type = XMEDIA_SVP_ALG_TYPE_FACE;
    } else {
        rect_cut_coef->x_coef = SVP_FACE_RECT_PD_X_COEF;
        rect_cut_coef->y_coef = SVP_FACE_RECT_PD_Y_COEF;
        rect_cut_coef->w_coef = SVP_FACE_RECT_PD_W_COEF;
        rect_cut_coef->h_coef = SVP_FACE_RECT_PD_H_COEF;
        *alg_type = XMEDIA_SVP_ALG_TYPE_PERSON;
    }
}

static xmedia_s32 svp_cal_luma_by_vgs(yolov5_detect_param *param, const xmedia_svp_task_input *input_image,
    xmedia_video_rect *rect, xmedia_u32 rect_num, xmedia_u64 *luma_single)
{
    xmedia_s32 ret;
    xmedia_s32 vgs_handle = -1;
    xmedia_vgs_frame_info vgs_task;
    xmedia_video_rect *rect_cfg;
    xmedia_u64 *luma_single_cfg;

    rect_cfg = rect;
    luma_single_cfg = luma_single;
    memcpy(&(vgs_task.img_in), input_image->frame, sizeof(xmedia_video_frame_info));
    memcpy(&(vgs_task.img_out), input_image->frame, sizeof(xmedia_video_frame_info));
    ret = xmedia_vgs_create_job(&vgs_handle);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "Vgs begin job fail, Error(%#x)\n", ret);
    }
    ret = xmedia_vgs_add_task_luma(vgs_handle,  &vgs_task, rect_cfg, rect_num, luma_single_cfg);
    if  (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "VGS AddLumaTask fail, Error(%#x)\n", ret);
        xmedia_vgs_cancel_job(vgs_handle);
    }

    ret = xmedia_vgs_submit_job(vgs_handle);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "VGS submit fail, Error(%#x)\n", ret);
        xmedia_vgs_cancel_job(vgs_handle);
        return ret;
    }
    ret = xmedia_vgs_wait_job(vgs_handle, 2000);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "VGS wait job fail, Error(%#x)\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 svp_cal_luma(yolov5_detect_param *param, const xmedia_svp_task_input *input_image,
                                    xmedia_svp_yolov5_output *result, xmedia_u8 type,  xmedia_isp_smart_info *roi_info)
{
    xmedia_u32 i = 0;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u64 luma_total = 0;
    xmedia_u64 size_total = 0;
    xmedia_video_rect *rect;
    xmedia_u64 *luma_single;
    xmedia_u64 *luma_single_cfg;
    xmedia_u32 rect_num = 0;
    svp_rect_cut_coef rect_cut_coef;
    xmedia_svp_alg_type alg_type;

    if (type >= SVP_SMART_CLASS_MAX) {
        return ret;
    }

    rect = (xmedia_video_rect *)malloc(SVP_MAX_CAL_LUMA_RECT_NUM * sizeof(xmedia_video_rect));
    CHECK_SVP_COND_GOTO_ERROR(rect == XMEDIA_NULL, EXIT0, "rect malloc failed !\n");
    luma_single = (xmedia_u64 *)malloc(SVP_MAX_CAL_LUMA_RECT_NUM * sizeof(xmedia_u64));
    CHECK_SVP_COND_GOTO_ERROR(luma_single == XMEDIA_NULL, EXIT1, "luma_single malloc failed !\n");
    luma_single_cfg = luma_single;

    svp_get_cal_luma_coef(type, &rect_cut_coef, &alg_type);

    if (param->type == alg_type) {
        svp_cut_rect(result, &rect_cut_coef, rect, &rect_num, &size_total, (xmedia_u8)alg_type);
    }

    if  (rect_num == 0) {
        free(luma_single);
        free(rect);
        return ret;
    }

    ret = svp_cal_luma_by_vgs(param, input_image, rect, rect_num, luma_single);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "vgs cal luma fail, Error(%#x)\n", ret);
    }
    for  (i = 0; i < rect_num; i++) {
        luma_total += *luma_single_cfg;
        luma_single_cfg++;
    }
    free(luma_single);
    free(rect);

    if (size_total != 0) {
        roi_info->roi[type].available = XMEDIA_TRUE;
        roi_info->roi[type].luma = (luma_total + (size_total >> SVP_MOV_ACCURACY_ONE)) / size_total;
    }
    return ret;

EXIT1:
    free(rect);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 svp_update_luma(yolov5_detect_param *param, xmedia_svp_yolov5_output *result,
                                    const xmedia_svp_task_input *input_image)
{
    xmedia_s32 ret;
    xmedia_s32 i = 0;
    xmedia_u32 svp_vipipe = 0;
    xmedia_isp_smart_info smart_info;

    if (input_image == NULL) {
        return XMEDIA_SUCCESS;
    }

    for (svp_vipipe = 0; svp_vipipe < XMEDIA_SVP_MAX_VI_PIPE_NUM; svp_vipipe++) {
        if (param->smart_ae_array[svp_vipipe] == XMEDIA_TRUE) {
            if ((input_image->frame->frame.width < param->w) || (input_image->frame->frame.height < param->h)
                || (input_image->frame->frame.stride.y_stride < param->w)
                || (input_image->frame->frame.addr.y_phy_addr == 0)) {
                SVP_TRACE(MODULE_DBG_ERR, "Frame data w:%d  h:%d s:%u \n", input_image->frame->frame.width,
                            input_image->frame->frame.height, input_image->frame->frame.stride.y_stride);
                return XMEDIA_FAILURE;
            }
            for (i = 0; i < SVP_SMART_CLASS_MAX; i++) {
                smart_info.roi[i].enable = XMEDIA_FALSE;
                smart_info.roi[i].available = XMEDIA_FALSE;
                smart_info.roi[i].luma = 0;
            }

            if (param->type == XMEDIA_SVP_ALG_TYPE_PERSON) {
                ret = svp_cal_luma(param, input_image, result, SVP_MODEL_TYPE_PEOPLE, &smart_info);
                if (ret != XMEDIA_SUCCESS) {
                    SVP_TRACE(MODULE_DBG_NOTICE, "cal isp info fail\n");
                    return XMEDIA_FAILURE;
                }
                smart_info.roi[SVP_FACE_INDEX].enable = XMEDIA_FALSE;
                smart_info.roi[SVP_PEOPLE_INDEX].enable = XMEDIA_TRUE;
                xmedia_isp_set_smart_info(svp_vipipe, &smart_info);
            } else if (param->type == XMEDIA_SVP_ALG_TYPE_FACE) {
                ret = svp_cal_luma(param, input_image, result, SVP_MODEL_TYPE_FACE, &smart_info);
                if (ret != XMEDIA_SUCCESS) {
                    SVP_TRACE(MODULE_DBG_NOTICE, "cal isp info fail\n");
                    return XMEDIA_FAILURE;
                }
                smart_info.roi[SVP_FACE_INDEX].enable = XMEDIA_TRUE;
                smart_info.roi[SVP_PEOPLE_INDEX].enable = XMEDIA_FALSE;
                xmedia_isp_set_smart_info(svp_vipipe, &smart_info);
            } else {
                SVP_TRACE(MODULE_DBG_NOTICE, "type not supported\n");
                return XMEDIA_FAILURE;
            }
        }
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_padding_resize(xmedia_video_frame_info *ref_frame, xmedia_svp_rect *rect, xmedia_u64 resize_phyaddr,
    xmedia_void *resize_viraddr, xmedia_u32 dst_width, xmedia_u32 dst_height)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 x1, y1, x2, y2, w, h;
    xmedia_float ratio_w, ratio_h;
    xmedia_video_frame_info input_resize_frame = {0};
    xmedia_video_frame_info output_resize_frame = {0};

    // Y:0xff 背景全部涂为白色
    memset(resize_viraddr, 0xff, dst_width * dst_height);
    // UV:0x80 固定透明色
    memset(resize_viraddr + dst_width * dst_height, 0x80, dst_width * dst_height / 2);

    x1 = (xmedia_s32)roundf(rect->x1 / 2) * 2;
    y1 = (xmedia_s32)roundf(rect->y1 / 2) * 2;
    x2 = (xmedia_s32)roundf(rect->x2 / 2) * 2;
    y2 = (xmedia_s32)roundf(rect->y2 / 2) * 2;

    if (x1 < 0 || y1 < 0 || x1 > x2 || y1 > y2) {
        return XMEDIA_SUCCESS;
    }

    if ((abs(x2 - x1) < VGS_LIMIT_BILINEAR_INIMG_W) || (abs(y2 - y1) < VGS_LIMIT_BILINEAR_INIMG_H)) {
        return XMEDIA_SUCCESS;
    }

    w = ABS(x2 - x1) / 2 * 2;
    h = ABS(y2 - y1) / 2 * 2;

    if (((x1 + w) > ref_frame->frame.stride.y_stride) || ((y1 + h) > ref_frame->frame.height)) {
        return XMEDIA_SUCCESS;
    }

#ifdef RCNN_DEBUG_SAVE_FILE
    SVP_TRACE(MODULE_DBG_ERR, "origin x1.y1:[%d,%d] x2,y2:[%d,%d] w[%d] h[%d]\n", x1, y1, x2, y2, w, h);
#endif

    memcpy(&input_resize_frame, ref_frame, sizeof(xmedia_video_frame_info));
    input_resize_frame.frame.width = w;
    input_resize_frame.frame.height = h;
    input_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;

    input_resize_frame.frame.addr.y_phy_addr = ref_frame->frame.addr.y_phy_addr +
        y1 * ref_frame->frame.stride.y_stride + x1;

    input_resize_frame.frame.addr.c_phy_addr = ref_frame->frame.addr.c_phy_addr +
        y1 * ref_frame->frame.stride.c_stride / 2 + x1;

    ratio_w = (xmedia_float)dst_width / w;
    ratio_h = (xmedia_float)dst_height / h;

    if (ratio_w == ratio_h) {
        output_resize_frame.frame.width = dst_width;
        output_resize_frame.frame.height = dst_height;
    } else if (ratio_w < ratio_h) {
        output_resize_frame.frame.width = dst_width;
        output_resize_frame.frame.height = (xmedia_s32)(ratio_w * h) / 2 * 2;
    } else {
        output_resize_frame.frame.width = (xmedia_s32)(ratio_h * w) / 2 * 2;
        output_resize_frame.frame.height = dst_height;
    }

    if (output_resize_frame.frame.width < VGS_LIMIT_BILINEAR_INIMG_W ||
        output_resize_frame.frame.height < VGS_LIMIT_BILINEAR_INIMG_H) {
        return XMEDIA_SUCCESS;
    }

    xmedia_s32 offset_x = (dst_width - output_resize_frame.frame.width) / 2;
    xmedia_s32 offset_y = (dst_height - output_resize_frame.frame.height) / 2;

    offset_x = offset_x / 2 * 2;
    offset_y = offset_y / 2 * 2;

    output_resize_frame.frame.addr.y_phy_addr = resize_phyaddr + dst_width * offset_y + offset_x;
    output_resize_frame.frame.addr.c_phy_addr = resize_phyaddr + dst_width * dst_height +
                                                dst_width * offset_y / 2 + offset_x;
    output_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    output_resize_frame.frame.stride.y_stride = dst_width;
    output_resize_frame.frame.stride.c_stride = dst_width;

    ret = svp_vgs_resize(&input_resize_frame, &output_resize_frame);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "svp_vgs_resize failed with %d\n", ret);
        return XMEDIA_FAILURE;
    }

#ifdef RCNN_DEBUG_SAVE_FILE
    // 保存debug文件
    xmedia_char resize[RCNN_DEBUG_STRING];
    xmedia_char origin[RCNN_DEBUG_STRING];

    xmedia_void* output_vir = XMEDIA_NULL;
    output_vir = xmedia_mmz_map(resize_phyaddr, dst_width * dst_height * 3 / 2, XMEDIA_FALSE);
    sprintf(resize, "resize_offsetxy%u_%u_wh%u_%u.yuv",
        offset_x, offset_y, output_resize_frame.frame.width, output_resize_frame.frame.height);
    printf("precess %s\n", resize);
    if (access(resize, 0) == 0) {
        remove(resize);
    }
    FILE* fp_002 = fopen(resize, "wb");
    fwrite(output_vir, dst_width * dst_height * 3 / 2, 1, fp_002);
    fclose(fp_002);
    xmedia_mmz_unmap(output_vir);

    xmedia_void* output_vir1 = XMEDIA_NULL;
    output_vir1 = xmedia_mmz_map(ref_frame->frame.addr.y_phy_addr,
        ref_frame->frame.stride.y_stride * ref_frame->frame.height * 3 / 2, XMEDIA_TRUE);
    sprintf(origin, "origin.yuv");
    if (access(origin, 0) == 0) {
        remove(origin);
    }
    FILE* fp_00 = fopen(origin, "wb");
    fwrite(output_vir1, ref_frame->frame.stride.y_stride * ref_frame->frame.height * 3 / 2, 1, fp_00);
    fclose(fp_00);

    xmedia_mmz_unmap(output_vir1);
#endif

    return XMEDIA_SUCCESS;
}

xmedia_float svp_iou_with_box1(xmedia_svp_yolov8_result *box1, xmedia_svp_dms_result *box2)
{
    xmedia_float area1;
    xmedia_float mix_area;

    mix_area = STD_MAX(0.0f, STD_MIN(box1->rect.x2, box2->rect.x2) -
                          STD_MAX(box1->rect.x1, box2->rect.x1)) *
               STD_MAX(0.0f, STD_MIN(box1->rect.y2, box2->rect.y2) -
                          STD_MAX(box1->rect.y1, box2->rect.y1));
    if ((mix_area - SVP_EPSILON) <= 0.0f) {
        return 0.0f;
    }
    area1 = (box1->rect.x2 - box1->rect.x1) * (box1->rect.y2 - box1->rect.y1);
    return (mix_area / area1);
}

static xmedia_bool is_point_in_polygon(xmedia_svp_vec2i point, const xmedia_svp_landmarks* polygon)
{
    if (polygon->points_count < 3) { // 小于3个点 不能构成多边形
        return XMEDIA_FAILURE;
    }

    xmedia_bool inside = XMEDIA_FALSE;
    xmedia_s32 n = polygon->points_count;
    xmedia_svp_vec2i p1, p2;
    xmedia_s32 intersect_x;

    for (xmedia_s32 i = 0, j = n - 1; i < n; j = i++) {
        p1 = polygon->points_array[i];
        p2 = polygon->points_array[j];

        if ((point.y == p1.y && point.x == p1.x) || (point.y == p2.y && point.x == p2.x)) {
            return XMEDIA_SUCCESS;
        }

        if ((p1.y > point.y) != (p2.y > point.y)) {
            intersect_x = p1.x + (point.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            if (point.x < intersect_x) {
                inside = !inside;
            }
        }
    }

    return inside;
}

static xmedia_svp_vec2i get_rect_center(const xmedia_svp_rect* rect, const xmedia_u32 width, const xmedia_u32 height)
{
    xmedia_svp_vec2i center;
    center.x = (xmedia_s32)((rect->x1 + rect->x2) / 2 * SVP_SCALE_RATIO) / width; // 转换为万分比
    center.y = (xmedia_s32)((rect->y1 + rect->y2) / 2 * SVP_SCALE_RATIO) / height; // 转换为万分比
    return center;
}

xmedia_s32 svp_detect_zone(const xmedia_svp_zone* zone, const xmedia_svp_rect* rect, const xmedia_u32 width,
    const xmedia_u32 height)
{
    if (zone->zone_num == 0) {
        return XMEDIA_FAILURE;
    }

    xmedia_svp_vec2i center = get_rect_center(rect, width, height);
    for (xmedia_s32 i = 0; i < zone->zone_num; i++) {
        const xmedia_svp_landmarks* current_zone = &zone->zones[i];
        if (is_point_in_polygon(center, current_zone) == XMEDIA_TRUE) {
            return i;
        }
    }

    return XMEDIA_FAILURE;
}

xmedia_s32 clip_rect_to_frame(xmedia_svp_rect *rect, xmedia_u32 frame_width, xmedia_u32 frame_height)
{
    if (!rect || frame_width == 0 || frame_height == 0) {
        return XMEDIA_FAILURE;
    }

    if (rect->x1 > rect->x2) {
        xmedia_float temp = rect->x1;
        rect->x1 = rect->x2;
        rect->x2 = temp;
    }
    if (rect->y1 > rect->y2) {
        xmedia_float temp = rect->y1;
        rect->y1 = rect->y2;
        rect->y2 = temp;
    }

    if (rect->x1 < 0) rect->x1 = 0;
    if (rect->x2 > frame_width) {
        rect->x2 = frame_width;
    }
    if (rect->y1 < 0) rect->y1 = 0;
    if (rect->y2 > frame_height) {
        rect->y2 = frame_height;
    }

    return XMEDIA_SUCCESS;
}
