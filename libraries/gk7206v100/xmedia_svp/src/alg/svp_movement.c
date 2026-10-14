#include "svp_movement.h"

xmedia_float movement_iou(movement_detect_result *box1, xmedia_svp_detect_result *box2)
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

static xmedia_s32 svp_find_trackid(movement_detect_result cur_result, svp_movement *movement,
                                            xmedia_s32 *idx)
{
    xmedia_s32 i;

    for (i = 0; i < movement->num; i++) {
        if (cur_result.class_type != movement->movement_targets[i].class_type) {
            continue;
        }
        if ((cur_result.tracker_id == movement->movement_targets[i].tracker_id) &&
            (movement->movement_targets[i].flag == XMEDIA_TRUE)) {
            *idx = i;
            return XMEDIA_SUCCESS;
        }
    }
    return XMEDIA_FAILURE;
}

static xmedia_s32 svp_query_idle_movement_arr(svp_movement *movement, svp_movement_result* result, xmedia_s32 cur_num)
{
    xmedia_s32 i;
    xmedia_s32 idx = -1;
    for (i = 0; i < movement->num; i++) {
        if (movement->movement_targets[i].flag == XMEDIA_FALSE) {
            movement->movement_targets[i].flag = XMEDIA_TRUE;
            movement->movement_targets[i].rect = result->targets[cur_num].rect;
            movement->movement_targets[i].count = 0;
            movement->movement_targets[i].tracker_id = result->targets[cur_num].tracker_id;
            movement->movement_targets[i].status = SVP_MOTION_STATUS_UNKNOWN;
            movement->movement_targets[i].class_type = result->targets[cur_num].class_type;
            idx = i;
            break;
        }
    }
    return idx;
}

static xmedia_void clear_movement_history(svp_movement *movement, xmedia_u64 cur_time)
{
    xmedia_s32 i;

    for (i = 0; i < movement->num; i++) {
        if (cur_time - movement->movement_targets[i].last_time >= SVP_MOTION_REFRESH_TIME) {
            movement->movement_targets[i].flag = XMEDIA_FALSE;
            movement->movement_targets[i].count = 0;
            movement->movement_targets[i].tracker_id = -1;
        }
    }
    return;
}

xmedia_s32 svp_detect_movement(svp_movement *movement, svp_movement_result* result)
{
    xmedia_s32 i;
    xmedia_s32 ret;
    xmedia_float box_iou;
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);
    xmedia_u64 cur_time = tp.tv_sec * SVP_S_TO_MS_UNIT  + tp.tv_nsec / SVP_NS_TO_MS_UNIT;
    (xmedia_void)clear_movement_history(movement, cur_time);
    for (i = 0; i < result->target_num; i++) {
        xmedia_s32 idx = -1;
        ret = svp_find_trackid(result->targets[i], movement, &idx); // 当前目标在历史组中寻找相同trackid
        if (ret == XMEDIA_FAILURE) {
            idx = svp_query_idle_movement_arr(movement, result, i);
            if (idx == -1) {
                SVP_TRACE(MODULE_DBG_ERR, "query idle movement_arr failed, is full\n");
                continue;
            }
        }
        xmedia_svp_detect_result history_box;
        history_box.rect = movement->movement_targets[idx].rect;
        history_box.class_type = movement->movement_targets[idx].class_type;
        box_iou = movement_iou(&result->targets[i], &history_box);
        movement->movement_targets[idx].last_time = cur_time;

        if (movement->movement_targets[idx].status == SVP_MOTION_STATUS_UNKNOWN) {
            if (box_iou >= movement->stillness_thres) {
                movement->movement_targets[idx].count++;
            } else {
                movement->movement_targets[idx].status = SVP_MOTION_STATUS_MOVEING;
                movement->movement_targets[idx].count = 0;
            }
            if (movement->movement_targets[idx].count >= movement->movement_fps_thres) {
                movement->movement_targets[idx].status = SVP_MOTION_STATUS_STATIC;
                movement->movement_targets[idx].count = 0;
            }
        } else if (movement->movement_targets[idx].status == SVP_MOTION_STATUS_STATIC) {
            if (box_iou < SVP_MOTION_THRES) { // 小于阈值则转为运动状态
                movement->movement_targets[idx].status = SVP_MOTION_STATUS_MOVEING;
            }
        } else {
            movement->movement_targets[idx].rect = result->targets[i].rect;
            if (box_iou >= movement->stillness_thres) {
                movement->movement_targets[idx].count++;
                if (movement->movement_targets[idx].count >= movement->movement_fps_thres) {
                    movement->movement_targets[idx].status = SVP_MOTION_STATUS_STATIC;
                    movement->movement_targets[idx].count = 0;
                }
            } else {
                movement->movement_targets[idx].count = 0;
            }
        }
        result->targets[i].motion_state = movement->movement_targets[idx].status;
    }
    return XMEDIA_SUCCESS;
}
