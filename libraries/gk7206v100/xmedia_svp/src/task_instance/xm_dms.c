#include "xm_dms.h"

static xmedia_s32 dms_check_attr(const xmedia_svp_dms_attr* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_attr.detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_attr.detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_attr.iou_threshold, "detect_iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_attr.iou_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_thres, "face_iou[%.2f] out of range(0-1) !! \n",
        task_attr->iou_thres);

    if ((task_attr->detect_attr.max_target_num) <= 0 ||
        (task_attr->detect_attr.max_target_num) > XMEDIA_SVP_DMS_MAX_TARGET_NUM) {
        SVP_TRACE(MODULE_DBG_ERR, "max_target_num[%d] out of range[0-5] !! \n",
            task_attr->detect_attr.max_target_num);
        return (XMEDIA_FAILURE);
    }

    if ((task_attr->warn_frame) <= 0 || (task_attr->warn_frame) >= UNSIGNED_8_BIT_NUM) {
        SVP_TRACE(MODULE_DBG_ERR, "warn_frame_num[%d] out of range[0-255] !! \n",
            task_attr->warn_frame);
        return (XMEDIA_FAILURE);
    }

    SVP_CHECK_ATTR_ENABLE(task_attr->detect_attr.bytetrack_enable,
        "bytetrack_enable must be 0 or 1 !! \n");
    SVP_CHECK_ATTR_ENABLE(task_attr->detect_attr.motionless_filter_enable,
        "motionless_filter_enable must be 0 or 1 !! \n");
    if (task_attr->detect_attr.motionless_filter_enable == XMEDIA_TRUE) {
        SVP_CHECK_ATTR_STILLNESS_THRES(task_attr->detect_attr.stillness_thres,
            "stillness_thres[%.2f] out of range(0.6-1) !! \n",
            task_attr->detect_attr.stillness_thres);
        SVP_CHECK_U8(task_attr->detect_attr.movement_fps_thres, "movement_fps_thres[%.d] out of range(0-255) !! \n",
            task_attr->detect_attr.movement_fps_thres);
    }
    return ret;
}

xmedia_s32 dms_init(xmedia_void *context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_dms_context *dms_context = (svp_dms_context *)context;

    CHECK_SVP_COND_RETURN_ERROR(dms_context == XMEDIA_NULL, "detect_dms_init context err !\n");
    dms_context->alarm_report.id = -1;
    dms_context->detect_info = (yolov8_detect_param *)calloc(1, sizeof(yolov8_detect_param));
    CHECK_CALLOC_GOTO_ERROR(dms_context->detect_info, EXIT0);
    dms_context->detect_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    CHECK_SVP_COND_GOTO_ERROR(dms_context->detect_info->tracklet.tracker == XMEDIA_NULL,
        EXIT1, "tracker calloc failed !\n");

    set_default_yolov8_detect_param(dms_context->detect_info);
    dms_context->detect_info->w = INPUT_DEFAULT_WIDE;
    dms_context->detect_info->h = INPUT_DEFAULT_HIGH;
    dms_context->detect_info->num = DETECTION_MODEL_SINGLE;
    dms_context->detect_info->keypoint_len = DMS_KEYPOINT;
    dms_context->detect_info->type = cfg.modules->alg_type;
    dms_context->detect_info->num = DMS_DETECTION_MODEL;
    dms_context->warn_frame = DMS_INT_FRAME;
    dms_context->iou_thres = DMS_IOU_THRES;
    if (cfg.modules->load_mode == XMEDIA_SVP_MODEL_FILE &&
        check_file_exist(cfg.modules->pathname) != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "model pathname err!\n");
        CFREE(dms_context->detect_info);
        return XMEDIA_FAILURE;
    }

    ret = yolov8_init(dms_context->detect_info, cfg.modules);
    CHECK_SVP_COND_GOTO_ERROR(ret != XMEDIA_SUCCESS, EXIT2, "yolov8 init failed %#x!\n", ret);

    return ret;

EXIT2:
    CFREE(dms_context->detect_info->tracklet.tracker);
EXIT1:
    CFREE(dms_context->detect_info);
EXIT0:
    return XMEDIA_FAILURE;
}

xmedia_s32 dms_uninit(xmedia_void* context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_dms_context *dms_context = (svp_dms_context*)context;
    CHECK_SVP_COND_RETURN_ERROR(dms_context == XMEDIA_NULL, "detect_dms_uninit context err !\n");

    if (dms_context->detect_info != XMEDIA_NULL) {
        if (dms_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = yolov8_uninit(dms_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "dms detect deinit failed %#x!\n", ret);
            }
        }
        CFREE(dms_context->detect_info->tracklet.tracker);
        CFREE(dms_context->detect_info);
    }
    return ret;
}

xmedia_s32 dms_set_attr(xmedia_void *context, const xmedia_svp_dms_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_dms_context *dms_context = (svp_dms_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(dms_context == XMEDIA_NULL, "detect_dms_set_attr context err !\n");
    yolov8_detect_param *detect_info = dms_context->detect_info;

    dms_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    detect_info->detect_threshold = task_attr->detect_attr.detect_threshold;
    detect_info->iou_threshold = task_attr->detect_attr.iou_threshold;
    detect_info->max_target_num = task_attr->detect_attr.max_target_num;
    detect_info->thres_desig = desigmoid(detect_info->detect_threshold);
    detect_info->tracklet.enable = task_attr->detect_attr.bytetrack_enable;
    dms_context->warn_frame = task_attr->warn_frame;
    dms_context->iou_thres = task_attr->iou_thres;
    return ret;
}

xmedia_s32 dms_get_attr(xmedia_void *context, xmedia_svp_dms_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_dms_context *dms_context = (svp_dms_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(dms_context == XMEDIA_NULL, "detect_dms_get_attr context err !\n");
    yolov8_detect_param *detect_info = dms_context->detect_info;

    task_attr->detect_attr.detect_threshold = detect_info->detect_threshold;
    task_attr->detect_attr.iou_threshold = detect_info->iou_threshold;
    task_attr->detect_attr.max_target_num = detect_info->max_target_num;
    task_attr->detect_attr.bytetrack_enable = detect_info->tracklet.enable;
    task_attr->warn_frame = dms_context->warn_frame;
    task_attr->iou_thres = dms_context->iou_thres;
    return ret;
}

xmedia_s32 dms_process(xmedia_void *context, const xmedia_svp_task_input *input,
                       xmedia_svp_dms_output *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_s32 i = 0, j = 0;
    xmedia_s32 x1, y1, x2, y2, w, h;
    xmedia_s32 dms_id = -1, box_area = 0;
    xmedia_bool change_id = XMEDIA_TRUE;

    svp_dms_context *dms_context = (svp_dms_context *)context;
    CHECK_SVP_COND_RETURN_ERROR(dms_context == XMEDIA_NULL, "detect_dms_process context err !\n");
    output->target_num = 0;

    // 分开存储不同类型的结果，人脸单独进追踪，逻辑判断完后汇总
    xmedia_svp_yolov8_output tmp_result = { 0 };
    xmedia_svp_yolov8_output phone_result = { 0 };
    xmedia_svp_yolov8_output cigar_result = { 0 };

    // 1.检测
    ret = detect_yolov8_process(dms_context->detect_info, input->frame, &tmp_result);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmyolov8 detect process err !\n");
        return ret;
    }

    // 2.先对结果分类保存，后续分开进行逻辑判断
    for (i = 0; i < tmp_result.target_num; i++) {
        if (tmp_result.targets[i].class_type == 0) {
            if (output->target_num >= XMEDIA_SVP_DMS_MAX_TARGET_NUM) {
                break;
            }
            output->targets[output->target_num].alg_type = tmp_result.targets[i].alg_type;
            output->targets[output->target_num].class_type = tmp_result.targets[i].class_type;
            output->targets[output->target_num].detect_score = tmp_result.targets[i].detect_score;
            memcpy(output->targets[output->target_num].kpt, tmp_result.targets[i].kpt,
                sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
            output->targets[output->target_num].rect = tmp_result.targets[i].rect;
            output->targets[output->target_num].tracker_age = tmp_result.targets[i].tracker_age;
            output->targets[output->target_num].tracker_id = tmp_result.targets[i].tracker_id;
            output->target_num++;
        } else if (tmp_result.targets[i].class_type == 1) {
            if (cigar_result.target_num >= XMEDIA_SVP_DMS_SPECIAL_MAX_NUM) {
                break;
            }
            cigar_result.targets[cigar_result.target_num] = tmp_result.targets[i];
            cigar_result.target_num++;
        } else {
            if (phone_result.target_num >= XMEDIA_SVP_DMS_SPECIAL_MAX_NUM) {
                break;
            }
            phone_result.targets[phone_result.target_num] = tmp_result.targets[i];
            phone_result.target_num++;
        }
    }

    // 3.人脸单独进追踪
    if (dms_context->detect_info->tracklet.enable == XMEDIA_TRUE) {
        xmedia_svp_tracker_result track_result = { 0 };
        track_result.target_num = output->target_num;
        for (i = 0; i < track_result.target_num; i++) {
            track_result.tracker_single[i].class_type = output->targets[i].class_type;
            track_result.tracker_single[i].detect_score = output->targets[i].detect_score;
            track_result.tracker_single[i].tracker_id = output->targets[i].tracker_id;
            track_result.tracker_single[i].tracker_age = output->targets[i].tracker_age;
            track_result.tracker_single[i].rect = output->targets[i].rect;
            memcpy(track_result.tracker_single[i].kpt, output->targets[i].kpt,
                sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
        }
        ret = svp_detect_bytetracker(&track_result,
                                     &dms_context->detect_info->tracklet,
                                     &dms_context->detect_info->tarck_id_grow,
                                      dms_context->detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        output->target_num = 0;
        for (i = 0; i < track_result.target_num; i++) {
            output->targets[output->target_num].alg_type = dms_context->detect_info->type;
            output->targets[output->target_num].class_type = track_result.tracker_single[i].class_type;
            output->targets[output->target_num].detect_score = track_result.tracker_single[i].detect_score;
            output->targets[output->target_num].tracker_id = track_result.tracker_single[i].tracker_id;
            output->targets[output->target_num].tracker_age = track_result.tracker_single[i].tracker_age;
            output->targets[output->target_num].rect = track_result.tracker_single[i].rect;
            memcpy(output->targets[output->target_num].kpt, track_result.tracker_single[i].kpt,
                sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
            output->target_num++;
        }
    }

    // 4.人脸状态分析
    for (i = 0; i < output->target_num; i++) {
        // 左眼纵横比
        if (fabsf(output->targets[i].kpt[39].x - output->targets[i].kpt[36].x) > DMS_EPSILON) {
            dms_context->alarm_report.state.left_ear =
                (fabsf(output->targets[i].kpt[37].y - output->targets[i].kpt[41].y) +
                 fabsf(output->targets[i].kpt[38].y - output->targets[i].kpt[40].y)) /
                (2 * fabsf(output->targets[i].kpt[39].x - output->targets[i].kpt[36].x));
        } else {
            // left EAR(eye aspect ratio) denominator is zero, not process
        }
        // 右眼纵横比
        if (fabsf(output->targets[i].kpt[45].x - output->targets[i].kpt[42].x) > DMS_EPSILON) {
            dms_context->alarm_report.state.right_ear =
                (fabsf(output->targets[i].kpt[43].y - output->targets[i].kpt[47].y) +
                 fabsf(output->targets[i].kpt[44].y - output->targets[i].kpt[46].y)) /
                (2 * fabsf(output->targets[i].kpt[45].x - output->targets[i].kpt[42].x));
        } else {
            // right EAR denominator is zero, not process
        }
        // 嘴唇外侧纵横比
        if (fabsf(output->targets[i].kpt[54].x - output->targets[i].kpt[48].x) > DMS_EPSILON) {
            dms_context->alarm_report.state.out_mar =
                (fabsf(output->targets[i].kpt[50].y - output->targets[i].kpt[58].y) +
                 fabsf(output->targets[i].kpt[52].y - output->targets[i].kpt[56].y)) /
                (fabsf(output->targets[i].kpt[54].x - output->targets[i].kpt[48].x));
        } else {
            // out MAR(mouth aspect ratio) denominator is zero, not process
        }
        // 嘴唇内侧纵横比
        if (fabsf(output->targets[i].kpt[64].x - output->targets[i].kpt[60].x) > DMS_EPSILON) {
            dms_context->alarm_report.state.in_mar =
                (fabsf(output->targets[i].kpt[61].y - output->targets[i].kpt[67].y) +
                 fabsf(output->targets[i].kpt[63].y - output->targets[i].kpt[65].y)) /
                (fabsf(output->targets[i].kpt[64].x - output->targets[i].kpt[60].x));
        } else {
            // in MAR(mouth aspect ratio) denominator is zero, not process
        }
    }

    // 5.筛选bbox最大的目标为特殊目标
    for (i = 0; i < output->target_num; i++) {
        x1 = (xmedia_s32)roundf(output->targets[i].rect.x1 / 2) * 2;
        y1 = (xmedia_s32)roundf(output->targets[i].rect.y1 / 2) * 2;
        x2 = (xmedia_s32)roundf(output->targets[i].rect.x2 / 2) * 2;
        y2 = (xmedia_s32)roundf(output->targets[i].rect.y2 / 2) * 2;
        w = ABS(x2 - x1);
        h = ABS(y2 - y1);
        if (w * h > box_area) {
            box_area = w * h;
            dms_id = output->targets[i].tracker_id;
            output->targets[i].special_target = XMEDIA_TRUE;
        }
        output->targets[i].dms_report.phone_num = 0;
        output->targets[i].dms_report.cigar_num = 0;
    }

    // 6.特殊目标做逻辑判断
    for (i = 0; i < output->target_num; i++) {
        if (output->targets[i].tracker_id != dms_context->alarm_report.id) {
            output->targets[i].special_target = XMEDIA_FALSE;
            continue;
        }

        xmedia_bool cur_frame_phone = XMEDIA_FALSE;
        xmedia_bool cur_frame_smoke = XMEDIA_FALSE;
        change_id = XMEDIA_FALSE;

        // 6.1 疲劳判断逻辑
        if (dms_context->alarm_report.state.left_ear < DMS_EYES_THRE &&
            dms_context->alarm_report.state.right_ear < DMS_EYES_THRE) {
            if (dms_context->alarm_report.flag.close_eyes == XMEDIA_FALSE &&
                dms_context->alarm_report.frame.close_eyes_frame >= dms_context->warn_frame) {
                dms_context->alarm_report.flag.close_eyes = XMEDIA_TRUE;
            }
            if (dms_context->alarm_report.frame.close_eyes_frame < dms_context->warn_frame) {
                dms_context->alarm_report.frame.close_eyes_frame++;
            }
        } else {
            if (dms_context->alarm_report.frame.close_eyes_frame <= 0) {
                dms_context->alarm_report.flag.close_eyes = XMEDIA_FALSE;
            }
            if (dms_context->alarm_report.frame.close_eyes_frame > 0) {
                dms_context->alarm_report.frame.close_eyes_frame--;
            }
        }

        if (dms_context->alarm_report.state.out_mar > DMS_OUT_MOUTH_THRES &&
            dms_context->alarm_report.state.in_mar > DMS_IN_MOUTH_THRES) {
            if (dms_context->alarm_report.flag.yawn == XMEDIA_FALSE &&
                dms_context->alarm_report.frame.yawn_frame >= dms_context->warn_frame) {
                dms_context->alarm_report.flag.yawn = XMEDIA_TRUE;
            }
            if (dms_context->alarm_report.frame.yawn_frame < dms_context->warn_frame) {
                dms_context->alarm_report.frame.yawn_frame++;
            }
        } else {
            if (dms_context->alarm_report.frame.yawn_frame <= 0) {
                dms_context->alarm_report.flag.yawn = XMEDIA_FALSE;
            }
            if (dms_context->alarm_report.frame.yawn_frame > 0) {
                dms_context->alarm_report.frame.yawn_frame--;
            }
        }

        if ((dms_context->alarm_report.flag.close_eyes == XMEDIA_TRUE) ||
            (dms_context->alarm_report.flag.yawn == XMEDIA_TRUE)) {
            output->targets[i].dms_report.fatigue_flag = XMEDIA_TRUE;
        }

        // 6.2 API输出68个关键点由外部计算俯仰角、偏航角、翻滚角用于判断分心逻辑

        // 6.3 手机判断逻辑
        for (j = 0; j < phone_result.target_num; j++) {
            if (output->targets[i].dms_report.phone_num >= XMEDIA_SVP_DMS_SPECIAL_MAX_NUM) {
                break;
            }
            output->targets[i].dms_report.phone[output->targets[i].dms_report.phone_num].detect_score =
                phone_result.targets[j].detect_score;
            output->targets[i].dms_report.phone[output->targets[i].dms_report.phone_num].rect =
                phone_result.targets[j].rect;
            output->targets[i].dms_report.phone_num++;
            if (svp_iou_with_box1(&phone_result.targets[j], &output->targets[i]) > dms_context->iou_thres) {
                if (dms_context->alarm_report.flag.phone == XMEDIA_FALSE &&
                    dms_context->alarm_report.frame.phone_frame >= dms_context->warn_frame) {
                    output->targets[i].dms_report.phone[j].flag = XMEDIA_TRUE;
                }
                if (dms_context->alarm_report.frame.phone_frame < dms_context->warn_frame) {
                    dms_context->alarm_report.frame.phone_frame++;
                }
                cur_frame_phone = XMEDIA_TRUE;
                break;
            }
        }
        // 没有手机目标需要把之前的状态置为FALSE
        if (cur_frame_phone == XMEDIA_FALSE) {
            if (dms_context->alarm_report.frame.phone_frame <= 0) {
                dms_context->alarm_report.flag.phone = XMEDIA_FALSE;
            }
            if (dms_context->alarm_report.frame.phone_frame > 0) {
                dms_context->alarm_report.frame.phone_frame--;
            }
        }

        // 6.4 香烟判断逻辑
        for (j = 0; j < cigar_result.target_num; j++) {
            if (output->targets[i].dms_report.cigar_num >= XMEDIA_SVP_DMS_SPECIAL_MAX_NUM) {
                break;
            }
            output->targets[i].dms_report.cigar[output->targets[i].dms_report.cigar_num].detect_score =
                cigar_result.targets[j].detect_score;
            output->targets[i].dms_report.cigar[output->targets[i].dms_report.cigar_num].rect =
                cigar_result.targets[j].rect;
            output->targets[i].dms_report.cigar_num++;
            if (svp_iou_with_box1(&cigar_result.targets[j], &output->targets[i]) > dms_context->iou_thres) {
                if (dms_context->alarm_report.flag.cigar == XMEDIA_FALSE &&
                    dms_context->alarm_report.frame.cigar_frame >= dms_context->warn_frame) {
                    output->targets[i].dms_report.cigar[j].flag = XMEDIA_TRUE;
                }
                if (dms_context->alarm_report.frame.cigar_frame < dms_context->warn_frame) {
                    dms_context->alarm_report.frame.cigar_frame++;
                }
                cur_frame_smoke = XMEDIA_TRUE;
                break;
            }
        }

        // 没有香烟需要把之前的状态置为FALSE
        if (cur_frame_smoke == XMEDIA_FALSE) {
            if (dms_context->alarm_report.frame.cigar_frame <= 0) {
                dms_context->alarm_report.flag.cigar = XMEDIA_FALSE;
            }
            if (dms_context->alarm_report.frame.cigar_frame > 0) {
                dms_context->alarm_report.frame.cigar_frame--;
            }
        }
    }

    // 监控目标id变换重置状态
    if (change_id || dms_id != dms_context->alarm_report.id) {
        if (dms_context->alarm_report.frame.id_frame > dms_context->detect_info->tracklet.track_age_thres) {
            dms_context->alarm_report.id = dms_id;
            dms_context->alarm_report.frame.id_frame =0;
            dms_context->alarm_report.frame.close_eyes_frame = 0;
            dms_context->alarm_report.flag.close_eyes = XMEDIA_FALSE;
            dms_context->alarm_report.frame.yawn_frame = 0;
            dms_context->alarm_report.flag.yawn = XMEDIA_FALSE;
            dms_context->alarm_report.frame.phone_frame = 0;
            dms_context->alarm_report.flag.phone = XMEDIA_FALSE;
            dms_context->alarm_report.frame.cigar_frame = 0;
            dms_context->alarm_report.flag.cigar = XMEDIA_FALSE;
        } else {
            dms_context->alarm_report.frame.id_frame++;
        }
    } else {
        dms_context->alarm_report.frame.id_frame = 0;
    }

    return ret;
}

