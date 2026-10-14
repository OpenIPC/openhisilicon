#include "xm_gesture.h"

static xmedia_float fminf_t(xmedia_float a, xmedia_float b)
{
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

static xmedia_float fmaxf_t(xmedia_float a, xmedia_float b)
{
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

static xmedia_s32 max(xmedia_s32 a, xmedia_s32 b)
{
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

static xmedia_s32 min(xmedia_s32 a, xmedia_s32 b)
{
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

BoundingBox bbox_rotate(BoundingBox bbox, xmedia_float angle, xmedia_s32 rows, xmedia_s32 cols)
{
    xmedia_float x_min = bbox.x_min;
    xmedia_float y_min = bbox.y_min;
    xmedia_float x_max = bbox.x_max;
    xmedia_float y_max = bbox.y_max;

    xmedia_float scale = (float)cols / rows;

    xmedia_float c_x = (x_min + x_max) / 2;
    xmedia_float c_y = (y_min + y_max) / 2;

    xmedia_float x[4] = { x_min - c_x, x_max - c_x, x_max - c_x, x_min - c_x };
    xmedia_float y[4] = { y_min - c_y, y_min - c_y, y_max - c_y, y_max - c_y };

    xmedia_float angle_rad = angle * M_PI / 180.0;
    xmedia_float cos_angle = cos(angle_rad);
    xmedia_float sin_angle = sin(angle_rad);

    xmedia_float x_t[4], y_t[4];
    for (xmedia_s32 i = 0; i < 4; ++i) {
        x_t[i] = (cos_angle * x[i] * scale + sin_angle * y[i]) / scale;
        y_t[i] = -sin_angle * x[i] * scale + cos_angle * y[i];
    }

    xmedia_float new_x_min = c_x + fminf_t(fminf_t(x_t[0], x_t[1]), fminf_t(x_t[2], x_t[3]));
    xmedia_float new_x_max = c_x + fmaxf_t(fmaxf_t(x_t[0], x_t[1]), fmaxf_t(x_t[2], x_t[3]));
    xmedia_float new_y_min = c_y + fminf_t(fminf_t(y_t[0], y_t[1]), fminf_t(y_t[2], y_t[3]));
    xmedia_float new_y_max = c_y + fmaxf_t(fmaxf_t(y_t[0], y_t[1]), fmaxf_t(y_t[2], y_t[3]));

    BoundingBox rotated_bbox = { new_x_min, new_y_min, new_x_max, new_y_max };
    return rotated_bbox;
}

BoundingBox square_bbox(BoundingBox bbox)
{
    BoundingBox square_bbox = bbox; // 复制 bbox 到 square_bbox

    xmedia_float h = bbox.y_max - bbox.y_min + 1; // 计算高度
    xmedia_float w = bbox.x_max - bbox.x_min + 1; // 计算宽度
    xmedia_float l = (h > w) ? h : w; // 取较大的值作为正方形的边长

    square_bbox.x_min = bbox.x_min + w * 0.5 - l * 0.5; // 计算正方形的新左上角 x 坐标
    square_bbox.y_min = bbox.y_min + h * 0.5 - l * 0.5; // 计算正方形的新左上角 y 坐标
    square_bbox.x_max = square_bbox.x_min + l - 1; // 计算正方形的新右下角 x 坐标
    square_bbox.y_max = square_bbox.y_min + l - 1; // 计算正方形的新右下角 y 坐标

    return square_bbox; // 返回正方形的边界框及其边长
}

xmedia_float calculate_angle(xmedia_s32 cx1, xmedia_s32 cy1, xmedia_s32 cx2, xmedia_s32 cy2)
{
    // 获取第一个关键点坐标（注意y坐标取反）
    xmedia_s32 x1 = cx1;
    xmedia_s32 y1 = -cy1;

    // 获取第二个关键点坐标（注意y坐标取反）
    xmedia_s32 x2 = cx2;
    xmedia_s32 y2 = -cy2;

    // 计算两个关键点之间的水平和垂直距离
    xmedia_s32 dx = x2 - x1;
    xmedia_s32 dy = y2 - y1;

    // 根据dx, dy计算弧度值
    xmedia_float radian_angle = atan2(dy, dx);

    // 将弧度值转换为角度值（-pi 到 pi 范围内）
    xmedia_float degree_angle = radian_angle * (180.0 / M_PI);

    return degree_angle;
}

xmedia_s32 alg_crop_gesture_fast(const xmedia_video_frame_info* input_img, xmedia_svp_rect* gesture_rect,
                    xmedia_video_frame_info* output_frame, xmedia_float angle, xmedia_s32 index, xmedia_u32 target_num)
{

    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_s32 x1, y1, x2, y2;
    xmedia_s32 x_side, y_side;
    xmedia_s32 c_x1, c_y1, c_x2, c_y2;
    xmedia_u64 align_phyaddr;
    xmedia_u64* align_viraddr = XMEDIA_NULL;
    xmedia_u64 crop_phyaddr;
    xmedia_void* crop_viraddr = XMEDIA_NULL;
    svp_ive_img input_crop_img = { 0 };
    svp_ive_img output_crop_img = { 0 };
    C_ALG_IMAGE_INFO src_img = { 0 };
    C_ALG_IMAGE_INFO dst_img = { 0 };
    xmedia_video_frame_info resize_output = { 0 };
    xmedia_video_frame_info resize_frame = { 0 };
    xmedia_video_frame_info gesture_input = { 0 };

    // 小图坐标转换成大图坐标
    c_x1 = (xmedia_u32)(gesture_rect->x1 / INPUT_DEFAULT_WIDE * INPUT_BIG_WIDE);
    c_y1 = (xmedia_u32)(gesture_rect->y1 / INPUT_DEFAULT_HIGH * INPUT_BIG_HIGH);
    c_x2 = (xmedia_u32)(gesture_rect->x2 / INPUT_DEFAULT_WIDE * INPUT_BIG_WIDE);
    c_y2 = (xmedia_u32)(gesture_rect->y2 / INPUT_DEFAULT_HIGH * INPUT_BIG_HIGH);

    xmedia_s32 width_ex = c_x2 - c_x1;
    xmedia_s32 height_ex = c_y2 - c_y1;
    xmedia_s32 cx = (c_x1 + c_x2) / 2;
    xmedia_s32 cy = (c_y1 + c_y2) / 2;

    xmedia_s32 max_dimension = max(width_ex, height_ex);
    xmedia_s32 crop_x1 = cx - max_dimension;
    xmedia_s32 crop_y1 = cy - max_dimension;
    xmedia_s32 crop_x2 = cx + max_dimension;
    xmedia_s32 crop_y2 = cy + max_dimension;
    if (crop_x1 % 2 != 0) {
        crop_x1 = crop_x1 - 1;
    }
    if (crop_x2 % 2 != 0) {
        crop_x2 = crop_x2 + 1;
    }
    if (crop_y1 % 2 != 0) {
        crop_y1 = crop_y1 - 1;
    }
    if (crop_y2 % 2 != 0) {
        crop_y2 = crop_y2 + 1;
    }

    if (crop_x1 < 0 || crop_y1 < 0 || crop_x2 > INPUT_BIG_WIDE || crop_y2 > INPUT_BIG_HIGH) {
        x1 = max(0, crop_x1);
        y1 = max(0, crop_y1);
        x2 = min(INPUT_BIG_WIDE, crop_x2);
        y2 = min(INPUT_BIG_HIGH, crop_y2);

    } else {
        x1 = crop_x1;
        y1 = crop_y1;
        x2 = crop_x2;
        y2 = crop_y2;
    }

    // 将目标检测框的坐标转换为裁剪后的相对坐标。获得目标检测框在裁剪后图片中的位置，因为后面旋转图片也要对目标检测框进行旋转。
    xmedia_s32 normalized_x1 = c_x1 - x1;
    xmedia_s32 normalized_y1 = c_y1 - y1;
    xmedia_s32 normalized_x2 = c_x2 - x1;
    xmedia_s32 normalized_y2 = c_y2 - y1;

    input_crop_img.width = INPUT_BIG_WIDE;
    input_crop_img.hight = INPUT_BIG_HIGH;
    input_crop_img.img_addr = input_img->frame.addr.y_phy_addr;

    output_crop_img.width = (x2 - x1 + 15) / 16 * 16;
    output_crop_img.hight = (y2 - y1);
    output_crop_img.img_addr = xmedia_mmz_alloc(XMEDIA_NULL, "output_crop_img", (y2 - y1) * (x2 - x1) * 3 / 2);
    if (output_crop_img.img_addr == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_NOTICE, "gesture output crop  buffer , alloc failed !\n");
        goto RELEASE_FRAME_MMZ;
    }

    ret = svp_ive_crop(&input_crop_img, &output_crop_img, x1, y1);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "gesture svp_ive_crop failed! \n");
        goto RELEASE_FRAME_MMZ;
    }

    resize_output.frame.width = GESTURE_RESIZE_OUTPUT_IMG_WIDE_192;
    resize_output.frame.height = GESTURE_RESIZE_OUTPUT_IMG_HIGH_192;
    resize_output.frame.stride.y_stride = GESTURE_RESIZE_OUTPUT_IMG_WIDE_192;
    resize_output.frame.stride.c_stride = GESTURE_RESIZE_OUTPUT_IMG_WIDE_192;
    resize_output.frame.color_info.quantify_range = XMEDIA_VIDEO_COLOR_FULL_RANGE;
    resize_output.frame.color_info.color_gamut = XMEDIA_VIDEO_COLOR_GAMUT_BT709;

    ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "resize_output", &resize_output.frame.addr.y_phy_addr,
        (xmedia_void**)&src_img.image_buffer, resize_output.frame.width * resize_output.frame.height * 3 / 2);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "resize_output alloc failed! \n");
        goto RELEASE_FRAME_MMZ;
    }

    resize_output.frame.addr.c_phy_addr = resize_output.frame.addr.y_phy_addr +
        GESTURE_RESIZE_OUTPUT_IMG_WIDE_192 * GESTURE_RESIZE_OUTPUT_IMG_HIGH_192;

    resize_frame.frame.height = output_crop_img.hight;
    resize_frame.frame.width = output_crop_img.width;
    resize_frame.frame.stride.y_stride = (output_crop_img.width + 15) / 16 * 16;
    resize_frame.frame.stride.c_stride = (output_crop_img.width + 15) / 16 * 16;
    resize_frame.frame.addr.y_phy_addr = output_crop_img.img_addr;
    resize_frame.frame.addr.c_phy_addr = output_crop_img.img_addr +
        resize_frame.frame.stride.c_stride * resize_frame.frame.height;

    ret = svp_vgs_resize(&resize_frame, &resize_output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "gesture svp_vgs_resize failed! \n");
        goto RELEASE_FRAME_MMZ;
    }

    BoundingBox box;
    box.x_min = (xmedia_float)normalized_x1 / output_crop_img.width;
    box.y_min = (xmedia_float)normalized_y1 / output_crop_img.hight;
    box.x_max = (xmedia_float)normalized_x2 / output_crop_img.width;
    box.y_max = (xmedia_float)normalized_y2 / output_crop_img.hight;
    output_crop_img.width = GESTURE_RESIZE_OUTPUT_IMG_WIDE_192;
    output_crop_img.hight = GESTURE_RESIZE_OUTPUT_IMG_HIGH_192;

    svp_warpaffine_info warpaffine_info = { 0 };
    xmedia_float warp_center_x = output_crop_img.width / 2;
    xmedia_float warp_center_y = output_crop_img.hight / 2;
    xmedia_float warp_up_x = warp_center_x;
    xmedia_float warp_up_y = warp_center_y + 60;
    xmedia_float warp_down_x = warp_center_x + 60;
    xmedia_float warp_down_y = warp_center_y;

// 以下180、60，分别是仿射变换的特定转换角度
    warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] = warp_center_x;
    warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] = warp_center_y;

    warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 1] = warp_up_x;
    warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 1) + 1)] = warp_up_y;

    warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 2] = warp_down_x;
    warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 2) + 1)] = warp_down_y;

    warpaffine_info.warpaffine_pairpack[index].keypoints[6] = warp_center_x;
    warpaffine_info.warpaffine_pairpack[index].keypoints[7] = warp_center_y - 60;

    warpaffine_info.warpaffine_pairpack[index].keypoints[8] = warp_center_x - 60;
    warpaffine_info.warpaffine_pairpack[index].keypoints[9] = warp_center_y;

    warpaffine_info.warpaffine_pairpack[index].targetpoints[0] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0];
    warpaffine_info.warpaffine_pairpack[index].targetpoints[1] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)];

    warpaffine_info.warpaffine_pairpack[index].targetpoints[2] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] + cos(PI / 2 - (angle / 180 * PI)) * 60;
    warpaffine_info.warpaffine_pairpack[index].targetpoints[3] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] - sin(PI / 2 - (angle / 180 * PI)) * 60;

    warpaffine_info.warpaffine_pairpack[index].targetpoints[4] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] + 60 * sin(PI / 2 - (angle / 180 * PI));
    warpaffine_info.warpaffine_pairpack[index].targetpoints[5] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] + 60 * cos(PI / 2 - (angle / 180 * PI));

    warpaffine_info.warpaffine_pairpack[index].targetpoints[6] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] - 60 * sin(PI / 2 - (angle / 180 * PI));
    warpaffine_info.warpaffine_pairpack[index].targetpoints[7] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] - 60 * cos(PI / 2 - (angle / 180 * PI));

    warpaffine_info.warpaffine_pairpack[index].targetpoints[8] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] + 60 * cos(PI / 2 - (angle / 180 * PI));
    warpaffine_info.warpaffine_pairpack[index].targetpoints[9] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] + 60 * sin(PI / 2 - (angle / 180 * PI));

    warpaffine_info.warpaffine_pairpack[index].targetpoints[0] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0];
    warpaffine_info.warpaffine_pairpack[index].targetpoints[1] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)];

    warpaffine_info.warpaffine_pairpack[index].targetpoints[2] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] + cos((angle / 180 * PI)) * 60;
    warpaffine_info.warpaffine_pairpack[index].targetpoints[3] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] + sin((angle / 180 * PI)) * 60;

    warpaffine_info.warpaffine_pairpack[index].targetpoints[4] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] + 60 * sin((angle / 180 * PI));
    warpaffine_info.warpaffine_pairpack[index].targetpoints[5] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] - 60 * cos((angle / 180 * PI));

    warpaffine_info.warpaffine_pairpack[index].targetpoints[6] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] - 60 * cos((angle / 180 * PI));
    warpaffine_info.warpaffine_pairpack[index].targetpoints[7] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] - 60 * sin((angle / 180 * PI));

    warpaffine_info.warpaffine_pairpack[index].targetpoints[8] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[2 * 0] - 60 * sin((angle / 180 * PI));
    warpaffine_info.warpaffine_pairpack[index].targetpoints[9] =
        warpaffine_info.warpaffine_pairpack[index].keypoints[((2 * 0) + 1)] + 60 * cos((angle / 180 * PI));

    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "gesture_align_buf", &align_phyaddr, (xmedia_void**)&align_viraddr,
        target_num * output_crop_img.width * output_crop_img.hight * 3 / 2);

    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "gesture_align mmz alloc fail !\n");
        goto RELEASE_FRAME_MMZ;
    }

    warpaffine_info.warpaffine_pairpack[index].warpaffine_output = (xmedia_char*)(align_viraddr) +
        (index * output_crop_img.width * output_crop_img.hight * 3 / 2);

    src_img.image_width = output_crop_img.width;
    src_img.image_height = output_crop_img.hight;
    src_img.image_stride = output_crop_img.width;

    dst_img.image_width = output_crop_img.width;
    dst_img.image_height = output_crop_img.hight;
    dst_img.image_stride = output_crop_img.width;
    dst_img.image_buffer = (unsigned char*)warpaffine_info.warpaffine_pairpack[index].warpaffine_output;

    memset(dst_img.image_buffer, 255, dst_img.image_height * dst_img.image_stride * 3 / 2);

    C_ALG_RESULT alg_result = alg_api_face_feature_warpaffine(&src_img, &dst_img,
        (xmedia_float*)&warpaffine_info.warpaffine_pairpack[index].targetpoints,
        (xmedia_float*)&warpaffine_info.warpaffine_pairpack[index].keypoints);

    if (alg_result != 0) {
        SVP_TRACE(MODULE_DBG_NOTICE, "alg_api_face_feature_warpaffine failed, error:%d !\n", alg_result);
        goto RELEASE_FRAME_MMZ;
    }

    angle = 90 - angle;
    BoundingBox box_out = bbox_rotate(box, angle, output_crop_img.width, output_crop_img.hight);
    BoundingBox transformed_bbox;
    transformed_bbox.x_min = box_out.x_min * output_crop_img.width;
    transformed_bbox.y_min = box_out.y_min * output_crop_img.hight;
    transformed_bbox.x_max = box_out.x_max * output_crop_img.width;
    transformed_bbox.y_max = box_out.y_max * output_crop_img.hight;

    BoundingBox bi;
    bi.x_min = (xmedia_s32)transformed_bbox.x_min;
    bi.y_min = (xmedia_s32)transformed_bbox.y_min;
    bi.x_max = (xmedia_s32)transformed_bbox.x_max;
    bi.y_max = (xmedia_s32)transformed_bbox.y_max;

    BoundingBox b = square_bbox(bi);
    xmedia_s32 x_left = (xmedia_s32)b.x_min;
    xmedia_s32 y_top = (xmedia_s32)b.y_min;
    xmedia_s32 x_right = (xmedia_s32)b.x_max;
    xmedia_s32 y_bottom = (xmedia_s32)b.y_max;

    if (x_left % 2 != 0) {
        x_left = x_left - 1;
    }
    if (x_right % 2 == 0) {
        x_right = x_right + 1;
    }
    if (y_top % 2 != 0) {
        y_top = y_top - 1;
    }
    if (y_bottom % 2 == 0) {
        y_bottom = y_bottom + 1;
    }
    xmedia_s32 stride_crop = ((x_right + 1 - x_left) + 15) / 16 * 16;

    ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "resize_input_buf", &crop_phyaddr, &crop_viraddr,
        stride_crop * (y_bottom - y_top + 1) * 3 / 2);

    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "bbox_crop_image alloc failed! \n");
        goto RELEASE_FRAME_MMZ;
    }

    memset(crop_viraddr, 255, stride_crop * (y_bottom - y_top + 1) * 3 / 2);

    xmedia_s32 h_c, w_c;
    if (x_left < 0 || y_top < 0 || x_right > INPUT_BIG_WIDE || y_bottom > INPUT_BIG_HIGH) {
        xmedia_s32 x1_img = max(0, x_left);
        xmedia_s32 y1_img = max(0, y_top);
        xmedia_s32 x2_img = min(output_crop_img.width, x_right);
        xmedia_s32 y2_img = min(output_crop_img.hight, y_bottom);

        h_c = min(dst_img.image_height - y1_img, (y2_img - y1_img + 1));
        w_c = min(dst_img.image_width - x1_img, (x2_img - x1_img + 1));
        for (xmedia_s32 m = 0; m < h_c; m++) {
            memcpy(crop_viraddr + m * stride_crop,
                (xmedia_void*)dst_img.image_buffer + (y1_img + m) * dst_img.image_stride + x1_img, w_c);
            memcpy(crop_viraddr + (y_bottom - y_top + 1) * stride_crop + m / 2 * stride_crop,
                (xmedia_void*)dst_img.image_buffer + dst_img.image_stride * dst_img.image_height +
                    (y1_img + m) / 2 * dst_img.image_stride + x1_img, w_c);
        }
    } else {
        h_c = min(dst_img.image_height - y_top, (y_bottom - y_top + 1));
        w_c = min(dst_img.image_width - x_left, (x_right - x_left + 1));
        for (int m = 0; m < h_c; m++) {
            memcpy(crop_viraddr + m * stride_crop,
                (xmedia_void*)dst_img.image_buffer + (y_top + m) * dst_img.image_stride + x_left, w_c);
            memcpy(crop_viraddr + (y_bottom - y_top + 1) * stride_crop + m / 2 * stride_crop,
                (xmedia_void*)dst_img.image_buffer + dst_img.image_stride * dst_img.image_height +
                    (y_top + m) / 2 * dst_img.image_stride + x_left, w_c);
        }
    }

    x_side = y_bottom - y_top + 1;
    y_side = x_right - x_left + 1;

    gesture_input.frame.addr.y_phy_addr = crop_phyaddr;
    gesture_input.frame.addr.c_phy_addr = crop_phyaddr + stride_crop * y_side;
    gesture_input.frame.stride.y_stride = stride_crop;
    gesture_input.frame.stride.c_stride = stride_crop;
    gesture_input.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    gesture_input.frame.width = x_side;
    gesture_input.frame.height = y_side;

    ret = svp_vgs_resize(&gesture_input, output_frame);
    if (XMEDIA_SUCCESS != ret) {
        SVP_TRACE(MODULE_DBG_NOTICE, "svp_vgs_resize failed:%d\n", ret);
        goto RELEASE_FRAME_MMZ;
    }

    if (crop_viraddr != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(crop_phyaddr, crop_viraddr);
    }

    if (align_viraddr != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(align_phyaddr, align_viraddr);
    }

    if (src_img.image_buffer != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(resize_output.frame.addr.y_phy_addr, src_img.image_buffer);
    }

    if (output_crop_img.img_addr != XMEDIA_NULL) {
        xmedia_mmz_free(output_crop_img.img_addr);
    }

    return XMEDIA_SUCCESS;

RELEASE_FRAME_MMZ:

    if (crop_viraddr != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(crop_phyaddr, crop_viraddr);
    }

    if (align_viraddr != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(align_phyaddr, align_viraddr);
    }

    if (src_img.image_buffer != XMEDIA_NULL) {
        svp_mmz_unmap_and_free(resize_output.frame.addr.y_phy_addr, src_img.image_buffer);
    }

    if (output_crop_img.img_addr != XMEDIA_NULL) {
        xmedia_mmz_free(output_crop_img.img_addr);
    }

    return XMEDIA_FAILURE;
}

static xmedia_s32 gesture_check_attr(const xmedia_svp_yolov8_attr *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SVP_CHECK_ATTR_THRESHOLD(task_attr->detect_threshold, "detect_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->detect_threshold);
    SVP_CHECK_ATTR_THRESHOLD(task_attr->iou_threshold, "iou_threshold[%.2f] out of range[0-1] !! \n",
        task_attr->iou_threshold);
    SVP_CHECK_ATTR_NUM(task_attr->max_target_num, "max_target_num[%d] out of range[0-50] !! \n",
        task_attr->max_target_num);
    SVP_CHECK_ATTR_ENABLE(task_attr->bytetrack_enable, "bytetrack_enable must be 0 or 1 !! \n");

    return ret;
}

xmedia_s32 gesture_get_attr(xmedia_void* context, xmedia_svp_yolov8_attr* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_gesture_context* gesture_context = (svp_gesture_context*)context;
    yolov8_detect_param* detect_info = gesture_context->detect_info;

    task_attr->detect_threshold = detect_info->detect_threshold;
    task_attr->classifier_threshold = detect_info->classifier_threshold;
    task_attr->iou_threshold = detect_info->iou_threshold;
    task_attr->max_target_num = detect_info->max_target_num;
    task_attr->bytetrack_enable = detect_info->tracklet.enable;

    return ret;
}

xmedia_s32 gesture_set_attr(xmedia_void* context, const xmedia_svp_yolov8_attr* task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_gesture_context* gesture_context = (svp_gesture_context*)context;
    yolov8_detect_param* detect_info = gesture_context->detect_info;

    ret = gesture_check_attr(task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "check_attr failed !!\n");
    }

    detect_info->detect_threshold = task_attr->detect_threshold;
    detect_info->classifier_threshold = task_attr->classifier_threshold;
    detect_info->iou_threshold = task_attr->iou_threshold;
    detect_info->max_target_num = task_attr->max_target_num;
    detect_info->thres_desig = desigmoid(detect_info->detect_threshold);
    detect_info->tracklet.enable = task_attr->bytetrack_enable;

    return ret;
}

xmedia_s32 gesture_uninit(xmedia_void* context)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_gesture_context* gesture_context = (svp_gesture_context*)context;

    if (gesture_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_NOTICE, "gesture_uninit param err ! \n");
        return XMEDIA_FAILURE;
    }

    if (gesture_context->gesture_info != XMEDIA_NULL) {
        if (gesture_context->gesture_info->npu_model.user_count != XMEDIA_NULL &&
                *(gesture_context->gesture_info->npu_model.user_count) == 1) {
            ret = npu_unload_model(&gesture_context->gesture_info->npu_model);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_NOTICE, "gesture_recognition unload modle failed ! \n");
                return XMEDIA_FAILURE;
            }
        }
        if (gesture_context->gesture_info->npu_model.user_count != XMEDIA_NULL) {
            *(gesture_context->gesture_info->npu_model.user_count) =
                *(gesture_context->gesture_info->npu_model.user_count) - 1;
        }
        CFREE(gesture_context->gesture_info);
    }

    if (gesture_context->detect_info != XMEDIA_NULL) {
        if (gesture_context->detect_info->model.user_count != XMEDIA_NULL) {
            ret = yolov8_uninit(gesture_context->detect_info);
            if (ret != XMEDIA_SUCCESS) {
                SVP_TRACE(MODULE_DBG_ERR, "gesture detect deinit failed %#x!\n", ret);
            }
        }
        CFREE(gesture_context->detect_info->tracklet.tracker);
        CFREE(gesture_context->detect_info);
    }
    SVP_TRACE(MODULE_DBG_NOTICE, "gesture_uninit successfully ! \n");
    return XMEDIA_SUCCESS;
}

xmedia_s32 gesture_init(xmedia_void* context, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    svp_gesture_context* gesture_context = (svp_gesture_context*)context;
    xmedia_svp_modules* tmp_module = XMEDIA_NULL;
    if (gesture_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "gesture detect init context err !\n");
        return XMEDIA_FAILURE;
    }
    if (cfg.module_num != 2) {
        SVP_TRACE(MODULE_DBG_ERR, "detect model num err!\n");
        return XMEDIA_FAILURE;
    }

    // 检测模型
    gesture_context->detect_info = (yolov8_detect_param*)calloc(1, sizeof(yolov8_detect_param));
    if (gesture_context->detect_info == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "gesture_context->detect_info calloc error !\n");
        return XMEDIA_FAILURE;
    }

    gesture_context->detect_info->tracklet.tracker = (svp_bytetracker*)calloc(1, sizeof(svp_bytetracker) *
        XMEDIA_SVP_MAX_TARGET_NUM * SVP_MAX_LOST_COUNT);
    if (gesture_context->detect_info->tracklet.tracker == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "tracker calloc error !\n");
        return XMEDIA_FAILURE;
    }

    set_default_yolov8_detect_param(gesture_context->detect_info);
    gesture_context->detect_info->w = INPUT_DEFAULT_WIDE;
    gesture_context->detect_info->h = INPUT_DEFAULT_HIGH;
    gesture_context->detect_info->num = DETECTION_MODEL_SINGLE;
    gesture_context->detect_info->keypoint_len = GESTURE_KEYPOINT_LEN;
    gesture_context->detect_info->type = cfg.modules->alg_type;
    tmp_module = cfg.modules;

    if (tmp_module->load_mode == XMEDIA_SVP_MODEL_FILE && check_file_exist(tmp_module->pathname) != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "model pathname err!\n");
        CFREE(gesture_context->detect_info);
        return XMEDIA_FAILURE;
    }

    ret = yolov8_init(gesture_context->detect_info, tmp_module);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "gesture detect model failed !\n");
        CFREE(gesture_context->detect_info);
        return ret;
    }

    // 分类模型
    gesture_context->gesture_info = (svp_gesture_info*)calloc(1, sizeof(svp_gesture_info));
    tmp_module = cfg.modules + 1;
    if (gesture_context->gesture_info == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "gesture_context->gesture_info calloc error !\n");
        yolov8_uninit(gesture_context->detect_info);
        CFREE(gesture_context->detect_info);
        return XMEDIA_FAILURE;
    }

    ret = npu_load_model(tmp_module, &gesture_context->gesture_info->npu_model);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "gesture recognition load model failed ! \n");
        CFREE(gesture_context->gesture_info);
        yolov8_uninit(gesture_context->detect_info);
        CFREE(gesture_context->detect_info);
        return XMEDIA_FAILURE;
    }
    *(gesture_context->gesture_info->npu_model.user_count) += 1;

    return ret;
}

xmedia_s32 gesture_process(xmedia_void* context, const xmedia_svp_task_input* input,
                           xmedia_svp_gesture_result* output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_s32 i = 0;
    svp_gesture_context* gesture_context = (svp_gesture_context*)context;
    if (gesture_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "gesture context err !\n");
        return ret;
    }
    xmedia_float angle;
    xmedia_u64 u64PhyAddr = XMEDIA_NULL;
    xmedia_video_frame_info best_img_small = { 0 };
    xmedia_video_frame_info* input_img_small = XMEDIA_NULL;
    xmedia_video_frame_info* input_img_big = XMEDIA_NULL;
    // 360p 1080p码流
    input_img_small = input->frame;
    input_img_big = input->frame + 1;

    if ((input_img_small->frame.addr.y_phy_addr == 0) || (input_img_small->frame.width != INPUT_DEFAULT_WIDE) ||
        (input_img_small->frame.height != INPUT_DEFAULT_HIGH)) {
        SVP_TRACE(MODULE_DBG_ERR, "picture size w:%d  h:%d \n",
            input_img_small->frame.width,
            input_img_small->frame.height);
        return XMEDIA_FAILURE;
    }

    if ((input_img_big->frame.addr.y_phy_addr == 0) || (input_img_big->frame.width != INPUT_BIG_WIDE) ||
        (input_img_big->frame.height != INPUT_BIG_HIGH)) {
        SVP_TRACE(MODULE_DBG_ERR, "picture size w:%d  h:%d \n",
            input_img_big->frame.width,
            input_img_big->frame.height);
        return XMEDIA_FAILURE;
    }

    if (input_img_small->frame.stride.y_stride == 0 || input_img_big->frame.stride.y_stride == 0) {
        SVP_TRACE(MODULE_DBG_NOTICE, "Frame stride err\n");
        return XMEDIA_FAILURE;
    }

    // 1.检测
    ret = detect_yolov8_process(gesture_context->detect_info, input_img_small, &output->result);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_NOTICE, "detect_keypoint_process failed !\n");
        goto RELEASE_FRAME_MMZ;
    }

    // 追踪
    if (gesture_context->detect_info->tracklet.enable == XMEDIA_TRUE) {
        xmedia_svp_tracker_result result = { 0 };
        result.target_num = output->result.target_num;
        for (i = 0; i < result.target_num; i++) {
            result.tracker_single[i].class_type = output->result.targets[i].class_type;
            result.tracker_single[i].detect_score = output->result.targets[i].detect_score;
            result.tracker_single[i].classfier_score = output->result.targets[i].classfier_score;
            result.tracker_single[i].tracker_id = output->result.targets[i].tracker_id;
            result.tracker_single[i].tracker_age = output->result.targets[i].tracker_age;
            result.tracker_single[i].rect = output->result.targets[i].rect;
            memcpy(result.tracker_single[i].kpt, output->result.targets[i].kpt,
                sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
        }
        ret = svp_detect_bytetracker(&result,
                                     &gesture_context->detect_info->tracklet,
                                     &gesture_context->detect_info->tarck_id_grow,
                                     gesture_context->detect_info->tarck_id_arry);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_NOTICE, "yolo detector npu bytetracker failed, ret=0x%x\n", ret);
        }
        memset(output, 0, sizeof(xmedia_svp_gesture_result));
        for (i = 0; i < result.target_num; i++) {
            output->result.targets[output->result.target_num].alg_type = gesture_context->detect_info->type;
            output->result.targets[output->result.target_num].class_type = result.tracker_single[i].class_type;
            output->result.targets[output->result.target_num].detect_score = result.tracker_single[i].detect_score;
            output->result.targets[output->result.target_num].classfier_score =
                result.tracker_single[i].classfier_score;
            output->result.targets[output->result.target_num].tracker_id = result.tracker_single[i].tracker_id;
            output->result.targets[output->result.target_num].tracker_age = result.tracker_single[i].tracker_age;
            output->result.targets[output->result.target_num].rect = result.tracker_single[i].rect;
            memcpy(output->result.targets[output->result.target_num].kpt, result.tracker_single[i].kpt,
                sizeof(xmedia_svp_keypoint) * MAX_DETECT_KEYPOINT_NUM);
            output->result.target_num++;
        }
    }

    xmedia_s32 stride_t = (GESTURE_BEST_OUTPUT_IMG_WIDE + SVP_ALIGN_BYTE - 1) / SVP_ALIGN_BYTE * SVP_ALIGN_BYTE;
    u64PhyAddr = xmedia_mmz_alloc(XMEDIA_NULL, "best_frame_buf", stride_t * GESTURE_BEST_OUTPUT_IMG_WIDE * 3 / 2);
    best_img_small.frame.width = GESTURE_BEST_OUTPUT_IMG_WIDE;
    best_img_small.frame.height = GESTURE_BEST_OUTPUT_IMG_HIGH;
    best_img_small.frame.pixel_fmt = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    best_img_small.frame.stride.y_stride = stride_t;
    best_img_small.frame.stride.c_stride = stride_t;
    best_img_small.frame.addr.y_phy_addr = u64PhyAddr;
    best_img_small.frame.addr.c_phy_addr = u64PhyAddr + GESTURE_BEST_OUTPUT_IMG_WIDE * GESTURE_BEST_OUTPUT_IMG_HIGH;

    for (i = 0; i < output->result.target_num; i++) {
        angle = calculate_angle(output->result.targets[i].kpt[0].x,
            output->result.targets[i].kpt[0].y,
            output->result.targets[i].kpt[1].x,
            output->result.targets[i].kpt[1].y);

        ret = alg_crop_gesture_fast(input_img_big, &output->result.targets[i].rect,
            &best_img_small, angle, i, output->result.target_num);

        ret = npu_run_model(&gesture_context->gesture_info->npu_model, u64PhyAddr);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "yolo detector npu forward failed, ret=0x%x\n", ret);
            goto RELEASE_FRAME_MMZ;
        }

        xmedia_u8* addr;
        xmedia_float arr[GESTURE_CLASS] = { 0 };
        xmedia_float result_arr[GESTURE_CLASS] = { 0 };
        addr = (xmedia_u8*)gesture_context->gesture_info->npu_model.output.tensor[0].addr;

        for (xmedia_u8 m = 0; m < GESTURE_CLASS; m++) {
            arr[m] = dequantize(addr[m],
                gesture_context->gesture_info->npu_model.output.tensor[0].quant.scale,
                gesture_context->gesture_info->npu_model.output.tensor[0].quant.zp);
        }

        softmax(arr, sizeof(arr) / sizeof(xmedia_float), result_arr);

        xmedia_float max_v = 0;
        xmedia_u8 max_id = 0;

        for (xmedia_u8 m = 0; m < GESTURE_CLASS; m++) {
            output->select_score[i] = result_arr[m]; // 分数
            if (result_arr[m] > max_v) {
                max_v = result_arr[m];
                max_id = m;
            }
        }

        if (max_id == GESTURE_OK && max_v >= GESTURE_THRES) {
            output->result.targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_GESTURE_OK;
        } else if (max_id == GESTURE_THUMB && max_v >= GESTURE_THRES) {
            output->result.targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_GESTURE_THUMB;
        } else if (max_id == GESTURE_V && max_v >= GESTURE_THRES) {
            output->result.targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_GESTURE_V;
        } else if (max_id == GESTURE_PALM && max_v >= GESTURE_THRES) {
            output->result.targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_GESTURE_PALM;
        } else {
            output->result.targets[i].class_type = XMEDIA_SVP_CLASS_TYPE_NOGESTURE;
        }
    }

    if (u64PhyAddr != XMEDIA_NULL) {
        xmedia_mmz_free(u64PhyAddr);
    }
    return XMEDIA_SUCCESS;

RELEASE_FRAME_MMZ:

    if (u64PhyAddr != XMEDIA_NULL) {
        xmedia_mmz_free(u64PhyAddr);
    }

    return XMEDIA_FAILURE;
}