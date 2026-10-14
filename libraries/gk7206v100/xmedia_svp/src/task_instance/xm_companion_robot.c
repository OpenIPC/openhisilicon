#include "xm_companion_robot.h"

static xmedia_svp_detect_anchors pd_anchors_four = {
    .anchors = {{{5.0f, 10.0f}, {8.0f, 19.0f}, {14.0f, 28.0f}},
               {{22.0f, 48.0f}, {30.0f, 79.0f}, {53.0f, 128.0f}},
               {{61.0f, 70.0f}, {91.0f, 197.0f}, {128.0f, 284.0f}},
               {{144.0f, 86.0f}, {185.0f, 178.0f}, {227.0f, 302.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};

static xmedia_svp_detect_anchors pet_anchors = {
    .anchors = {{{8.0f, 8.0f}, {16.0f, 16.0f}, {25.0f, 26.0f}},
               {{48.0f, 44.0f}, {62.0f, 76.0f}, {96.0f, 80.0f}},
               {{112.0f, 154.0f}, {208.0f, 148.0f}, {175.0f, 249.0f}},
               {{247.0f, 331.0f}, {380.0f, 333.0f}, {531.0f, 315.0f}}},
    .layer_num = LAYER_NUM_FOUR,
};

// 后面放svp_context里面
typedef struct {
    xmedia_u8 rcnn_count;
    xmedia_npu_model model;
} xmedia_svp_rcnn_model;

static xmedia_svp_rcnn_model rcnn_model = {0};

static xmedia_void detect_get_rcnn_class_id(xmedia_svp_yolov5_output *detect_result,
    xmedia_svp_alg_type type)
{
    for (xmedia_s32 i = 0; i < detect_result->target_num; i++) {
        if (type == XMEDIA_SVP_ALG_TYPE_PET) {
            if (detect_result->targets[i].class_type == 0) {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_PET;
            } else {
                detect_result->targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_MAX;
            }
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "rcnn does not support this type[%d]\n", type);
        }
    }
}

xmedia_s32 companion_robot_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_svp_modules *tmp_module = XMEDIA_NULL;
    svp_companion_robot_context *robot_context = (svp_companion_robot_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(robot_context == XMEDIA_NULL, "companion_robot_init context err !\n");
    CHECK_SVP_COND_RETURN_ERROR(cfg.module_num != RCNN_MODEL_NUM, "module num err !\n");

    // 检测模型
    tmp_module = cfg.modules;
    CHECK_SVP_COND_RETURN_ERROR(tmp_module == XMEDIA_NULL, "detect model is NULL !\n");
    CHECK_SVP_COND_RETURN_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS), "detect model pathname err!\n");

    robot_context->detect_info = (yolov5_detect_param *)calloc(1, sizeof(yolov5_detect_param));
    CHECK_CALLOC_RETURN_ERROR(robot_context->detect_info);

    robot_context->detect_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    CHECK_SVP_COND_GOTO_ERROR(robot_context->detect_info->tracklet.tracker == XMEDIA_NULL,
        EXIT0, "calloc tracker failed !\n");

    set_default_yolov5_detect_param(robot_context->detect_info);
    robot_context->detect_info->w = INPUT_DEFAULT_WIDE;
    robot_context->detect_info->h = INPUT_DEFAULT_HIGH;
    robot_context->detect_info->num = DETECTION_MODEL_SINGLE;
    if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PERSON) {
        robot_context->detect_info->anchors = pd_anchors_four;
    } else if (tmp_module->alg_type == XMEDIA_SVP_ALG_TYPE_PET) {
        robot_context->detect_info->anchors = pet_anchors;
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "rcnn does not support this type[%d]\n", tmp_module->alg_type);
        goto EXIT1;
    }
    ret = detect_init(robot_context->detect_info, tmp_module->alg_type,
        tmp_module, robot_context->detect_info->anchors.layer_num);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT1, "detect init model failed !\n");

    // 二阶段模型
    tmp_module = cfg.modules + 1;
    CHECK_SVP_COND_GOTO_ERROR(tmp_module == XMEDIA_NULL, EXIT2, "rcnn_model is NULL !\n");
    CHECK_SVP_COND_GOTO_ERROR((tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE) &&
        (check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS),
        EXIT2, "rcnn_model pathname err!\n");

    if (rcnn_model.rcnn_count == 0) {
        ret = npu_load_model(tmp_module, &rcnn_model.model);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT2, "rcnn_model load err!\n");
    }

    rcnn_model.rcnn_count += 1;
    robot_context->detect_info->classifier_threshold = RCNN_CLASSIFY_THR;
    robot_context->resize_phyaddr = xmedia_mmz_alloc(XMEDIA_NULL, "img_resize",
        RCNN_RECO_FRAME_SIZE * RCNN_RECO_FRAME_SIZE * 3 / 2);
    CHECK_SVP_COND_GOTO_ERROR(robot_context->resize_phyaddr == 0, EXIT3, "img_resize mmz alloc fail !\n");

    return ret;

EXIT3:
    rcnn_model.rcnn_count -= 1;
    if (rcnn_model.rcnn_count == 0) {
        ret = npu_unload_model(&rcnn_model.model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "rcnn unload model failed ! \n");
        }
    }
EXIT2:
    ret = detect_deinit(robot_context->detect_info);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
    }
EXIT1:
    CFREE(robot_context->detect_info->tracklet.tracker);
EXIT0:
    CFREE(robot_context->detect_info);

    return XMEDIA_FAILURE;
}

xmedia_s32 companion_robot_uninit(xmedia_void *context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_companion_robot_context *robot_context = (svp_companion_robot_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(robot_context == XMEDIA_NULL, "companion_robot_uninit context err !\n");
    if (robot_context->resize_phyaddr != 0) {
        xmedia_mmz_free(robot_context->resize_phyaddr);
    }

    if (rcnn_model.rcnn_count > 0) {
        rcnn_model.rcnn_count -= 1;
    }

    if (rcnn_model.rcnn_count == 0) {
        ret = npu_unload_model(&rcnn_model.model);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "rcnn unload model failed ! \n");
        }
    }

    if (robot_context->detect_info != XMEDIA_NULL) {
        if (robot_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = detect_deinit(robot_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "detect_deinit failed %#x!\n", ret);
            }
        }
        CFREE(robot_context->detect_info->tracklet.tracker);
        CFREE(robot_context->detect_info);
    }

    return ret;
}

xmedia_s32 companion_robot_set_attr(xmedia_void *context, const xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_companion_robot_context *robot_context = (svp_companion_robot_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(robot_context == XMEDIA_NULL, "rcnn_detect_set_attr context err !\n");

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->classifier_threshold, "classifier_threshold[%.2f] out of range(0-1) !! \n",
        task_attr->classifier_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range(0-50) !! \n",
        task_attr->max_target_num);
    SVP_CHECK_ATTR_ENABLE(task_attr->bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");
    SVP_CHECK_ATTR_ENABLE(task_attr->motionless_filter_enable, "motionless_filter_enable must be 0 or 1 !! \n");
    if (task_attr->motionless_filter_enable == XMEDIA_TRUE && task_attr->bytetrack_enable == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "motionless_filter[%d] depends on bytetrack[%d] ! \n", task_attr->bytetrack_enable,
            task_attr->motionless_filter_enable);
        return XMEDIA_FAILURE;
    }
    if (task_attr->motionless_filter_enable == XMEDIA_TRUE) {
        SVP_CHECK_ATTR_STILLNESS_THRES(task_attr->stillness_thres, "stillness_thres[%.2f] out of range(0.6-1) !! \n",
            task_attr->stillness_thres);
        SVP_CHECK_U8(task_attr->movement_fps_thres, "movement_fps_thres[%.d] out of range(0-255) !! \n",
            task_attr->movement_fps_thres);
    }
    robot_context->detect_info->detect_threshold = task_attr->detect_threshold;
    robot_context->detect_info->iou_threshold = task_attr->iou_threshold;
    robot_context->detect_info->max_target_num = task_attr->max_target_num;
    robot_context->detect_info->classifier_threshold = task_attr->classifier_threshold;
    robot_context->detect_info->tracklet.enable = task_attr->bytetrack_enable;

    robot_context->detect_info->thres_desig = desigmoid(robot_context->detect_info->detect_threshold);
    robot_context->detect_info->movement.enable = task_attr->motionless_filter_enable;
    robot_context->detect_info->movement.stillness_thres = task_attr->stillness_thres;
    robot_context->detect_info->movement.movement_fps_thres = task_attr->movement_fps_thres;
    robot_context->detect_info->tracklet.activated_tracker_thres = -0.1;
    robot_context->detect_info->tracklet.tracked_tracker_thres = -0.1;
    robot_context->detect_info->tracklet.unactivated_tracker_thres = -0.1;
    robot_context->detect_info->tracklet.high_score_thres = 0.1;
    robot_context->detect_info->tracklet.track_age_thres = 10;
    return ret;
}

xmedia_s32 companion_robot_get_attr(xmedia_void *context, xmedia_svp_yolov5_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_companion_robot_context *robot_context = (svp_companion_robot_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(robot_context == XMEDIA_NULL, "companion_robot_get_attr context err !\n");

    task_attr->detect_threshold = robot_context->detect_info->detect_threshold;
    task_attr->iou_threshold = robot_context->detect_info->iou_threshold;
    task_attr->max_target_num = robot_context->detect_info->max_target_num;
    task_attr->classifier_threshold = robot_context->detect_info->classifier_threshold;
    task_attr->bytetrack_enable = robot_context->detect_info->tracklet.enable;
    task_attr->motionless_filter_enable = robot_context->detect_info->movement.enable;
    task_attr->stillness_thres = robot_context->detect_info->movement.stillness_thres;
    task_attr->movement_fps_thres = robot_context->detect_info->movement.movement_fps_thres;

    return ret;
}

xmedia_s32 companion_robot_process(xmedia_void *context, const xmedia_svp_task_input *input,
    xmedia_svp_companion_robot_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 x1, y1, x2, y2, w, h, padding_size;
    xmedia_u64 it_phyaddr = 0;
    xmedia_void *it_viraddr = XMEDIA_NULL;
    xmedia_u8 i;
    svp_companion_robot_context *robot_context = (svp_companion_robot_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(robot_context == XMEDIA_NULL, "companion_robot_process context err !\n");

    svp_ive_img crop_src_img;
    crop_src_img.width = input->frame->frame.width;
    crop_src_img.hight = input->frame->frame.height;
    crop_src_img.img_addr = input->frame->frame.addr.y_phy_addr;

    xmedia_svp_yolov5_output yolov5_output = {0};
    ret = detect_process(robot_context->detect_info, input->frame,
        &yolov5_output, robot_context->detect_info->anchors);
    CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "detect process err !\n");

    detect_get_rcnn_class_id(&yolov5_output, robot_context->detect_info->type);
    for(i = 0; i < yolov5_output.target_num; i++) {
        svp_ive_img crop_dst_img, padding_dst_img;
        x1 = (xmedia_s32)roundf(yolov5_output.targets[i].rect.x1 / 2) * 2;
        y1 = (xmedia_s32)roundf(yolov5_output.targets[i].rect.y1 / 2) * 2;
        x2 = (xmedia_s32)roundf(yolov5_output.targets[i].rect.x2 / 2) * 2;
        y2 = (xmedia_s32)roundf(yolov5_output.targets[i].rect.y2 / 2) * 2;

        if ((abs(x2 - x1) < VGS_LIMIT_BILINEAR_INIMG_W) || (abs(y2 - y1) < VGS_LIMIT_BILINEAR_INIMG_H) ||
            (abs(x2 - x1) * abs(y2 - y1) < SVP_ROBOT_MIN_BOX_AREA)) {
            continue;
        }

        w = ABS(x2 - x1);
        h = ABS(y2 - y1);
        padding_size = STD_MAX(w, h);

        // 申请复合内存，多处使用
        ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "svp_it_buf", &it_phyaddr,
                                   (xmedia_void **)&it_viraddr,
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
        memset((xmedia_void*)padding_dst_img.vir_addr, 0xff, SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE) *
                             padding_dst_img.hight);
        // UV:0x80 固定透明色
        memset(((xmedia_void*)padding_dst_img.vir_addr + SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE) *
                              padding_dst_img.hight), 0x80, SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE) *
                              padding_dst_img.hight / 2);

        x1 = (padding_size - w) / 2;
        y1 = (padding_size - h) / 2;

        xmedia_mmz_flush_cache(it_phyaddr, it_viraddr, (SVP_ALIGN_UP(w, SVP_ALIGN_BYTE) * h * 3 / 2) +
                              (SVP_ALIGN_UP(padding_size, SVP_ALIGN_BYTE) * padding_size * 3 / 2));

        ret = svp_ive_padding(&crop_dst_img, &padding_dst_img, x1, y1);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_ive_padding error !!\n");

        xmedia_video_frame_info input_resize_frame = {0};
        xmedia_video_frame_info output_resize_frame = {0};
        input_resize_frame.frame.width = padding_dst_img.width;
        input_resize_frame.frame.height = padding_dst_img.hight;
        input_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
        input_resize_frame.frame.stride.y_stride = SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE);
        input_resize_frame.frame.stride.c_stride = SVP_ALIGN_UP(padding_dst_img.width, SVP_ALIGN_BYTE);
        input_resize_frame.frame.addr.y_phy_addr = padding_dst_img.img_addr;
        input_resize_frame.frame.addr.c_phy_addr =
            padding_dst_img.img_addr + input_resize_frame.frame.stride.c_stride * padding_dst_img.hight;
        output_resize_frame.frame.width = RCNN_RECO_FRAME_SIZE;
        output_resize_frame.frame.height = RCNN_RECO_FRAME_SIZE;
        output_resize_frame.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
        output_resize_frame.frame.stride.y_stride = RCNN_RECO_FRAME_SIZE;
        output_resize_frame.frame.stride.c_stride = RCNN_RECO_FRAME_SIZE;
        output_resize_frame.frame.addr.y_phy_addr = robot_context->resize_phyaddr;
        output_resize_frame.frame.addr.c_phy_addr =
            robot_context->resize_phyaddr + RCNN_RECO_FRAME_SIZE * RCNN_RECO_FRAME_SIZE;
        ret = svp_vgs_resize(&input_resize_frame, &output_resize_frame);
        CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT, "svp_vgs_resize error !!\n");

        if (it_viraddr != XMEDIA_NULL || it_phyaddr != 0) {
            svp_mmz_unmap_and_free(it_phyaddr, it_viraddr);
        }
        ret = npu_run_model(&rcnn_model.model, robot_context->resize_phyaddr);
        CHECK_SVP_COND_RETURN_ERROR(ret != XMEDIA_SUCCESS, "npu_run_model error !!\n");

        xmedia_u8 *addr;
        xmedia_float arr[RCNN_CLASSIFY_COUNT] = { 0 };
        xmedia_float result_arr[RCNN_CLASSIFY_COUNT] = { 0 };
        addr = (xmedia_u8 *)rcnn_model.model.output.tensor[0].addr;
        for (xmedia_u8 m = 0; m < RCNN_CLASSIFY_COUNT; m++) {
            arr[m] = dequantize(addr[m], rcnn_model.model.output.tensor[0].quant.scale,
                rcnn_model.model.output.tensor[0].quant.zp);
        }
        softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);

        if (robot_context->detect_info->type == XMEDIA_SVP_ALG_TYPE_PET) {
            if (result_arr[SVP_CLASS_RCNN_PET] >= robot_context->detect_info->classifier_threshold) {
                output->targets[output->target_num] = yolov5_output.targets[i];
                output->targets[output->target_num].classfier_score = result_arr[SVP_CLASS_RCNN_PET];
                output->target_num++;
            }
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "error detect type[%d] !\n", robot_context->detect_info->type);
        }
    }

    xmedia_u32 endnum = 0;
    det_nms_cross(output->targets, output->target_num, RCNN_IOU_THR, &endnum);
    yolov5_output.target_num = 0;
    for (xmedia_u8 i = 0; i < endnum; i++) {
        yolov5_output.targets[yolov5_output.target_num] = output->targets[i];
        yolov5_output.target_num += 1;
        if (yolov5_output.target_num >= robot_context->detect_info->max_target_num) {
            break;
        }
    }

    if (robot_context->detect_info->tracklet.enable == XMEDIA_TRUE) {
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
                                     &robot_context->detect_info->tracklet,
                                     &robot_context->detect_info->tarck_id_grow,
                                     robot_context->detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        memset(output, 0, sizeof(xmedia_svp_companion_robot_output));
        for(i = 0; i < result.target_num; i++) {
            output->targets[i].alg_type = robot_context->detect_info->type;
            output->targets[i].class_type = result.tracker_single[i].class_type;
            output->targets[i].detect_score = result.tracker_single[i].detect_score;
            output->targets[i].classfier_score = result.tracker_single[i].classfier_score;
            output->targets[i].tracker_id = result.tracker_single[i].tracker_id;
            output->targets[i].tracker_age = result.tracker_single[i].tracker_age;
            output->targets[i].rect = result.tracker_single[i].rect;
            // 计算距离，需要转换系数转换成真实距离
            output->targets[i].distance = (xmedia_float)INPUT_DEFAULT_HIGH - output->targets[i].rect.y2;
            output->target_num++;
        }
    }

    xmedia_svp_detect_result max_score_result = {0};
    for (i = 0; i < output->target_num; i++) {
        if (output->targets[i].detect_score > max_score_result.detect_score) {
            max_score_result = output->targets[i];
        }
    }
    // 只输出一个目标
    if (output->target_num > 0) {
        output->target_num = 1;
        output->targets[0] = max_score_result;
    }

    return XMEDIA_SUCCESS;

EXIT:
    if (it_viraddr != XMEDIA_NULL || it_phyaddr != 0) {
        svp_mmz_unmap_and_free(it_phyaddr, it_viraddr);
    }
    return XMEDIA_FAILURE;
}
