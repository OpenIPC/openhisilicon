#include "svp_nms.h"

static inline xmedia_s32 cmp_score_fall(const xmedia_void *lsh, const xmedia_void *rsh)
{
    // 降序排列, 令score最大的box在队列末端
    xmedia_svp_detect_result *s1 = (xmedia_svp_detect_result *)lsh;
    xmedia_svp_detect_result *s2 = (xmedia_svp_detect_result *)rsh;
    return s1->detect_score > s2->detect_score ? -1 : 1;
}

xmedia_float iou(svp_base_result *box1, svp_base_result *box2)
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

static xmedia_float iou_cross(xmedia_svp_detect_result* box1, xmedia_svp_detect_result* box2)
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

    return (mix_area / STD_MIN(area1, area2));
}

xmedia_s32 det_nms(svp_base_result *vec_bbox, xmedia_u32 vec_bbox_len,
                                                      xmedia_float threshold, xmedia_u32 *picked_bbox_len)
{
    xmedia_u32 i, j, k, m;
    svp_base_result *box_b;
    svp_base_result *box_a;

    qsort(vec_bbox, vec_bbox_len, sizeof(svp_base_result), cmp_score_fall);

    for (i = 0, j = vec_bbox_len; i < j; i++) {
        box_a = &vec_bbox[i];
        for (k = i + 1, m = i + 1; k < j; k++) {
            box_b = &vec_bbox[k];
            xmedia_float box_iou = iou(box_a, box_b);
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

xmedia_s32 det_nms_cross(xmedia_svp_detect_result* vec_bbox, xmedia_u32 vec_bbox_len,
                         xmedia_float threshold, xmedia_u32* picked_bbox_len)
{
    xmedia_u32 i, j, k, m;
    xmedia_svp_detect_result* box_b;
    xmedia_svp_detect_result* box_a;

    qsort(vec_bbox, vec_bbox_len, sizeof(xmedia_svp_detect_result), cmp_score_fall);

    for (i = 0, j = vec_bbox_len; i < j; i++) {
        box_a = &vec_bbox[i];
        for (k = i + 1, m = i + 1; k < j; k++) {
            box_b = &vec_bbox[k];
            xmedia_float box_iou = iou_cross(box_a, box_b);
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
