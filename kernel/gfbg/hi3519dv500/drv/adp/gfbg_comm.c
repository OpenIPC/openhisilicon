/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_comm.h"
#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
static td_u32 gfbg_node_max(td_u32 a, td_u32 b)
{
    return (a > b) ? a : b;
}

static td_u32 gfbg_node_min(td_u32 a, td_u32 b)
{
    return (a > b) ? b : a;
}

td_void gfbg_recalculate_stride(td_u32 *cmp_stride, td_u32 *uncmp_stride, const gfbg_stride_attr *attr)
{
    td_u32 bit_dep_a, bit_dep_rgb, osd_mode;
    td_u32 comp_ratio;
    td_bool is_continue;
    td_u32 mb_num_x, buffer_init_bits, delta, mb_ori_bits, budget_mb_bits, last_mb_width, budget_mb_bits_last;
    td_u32 last_mb_extra_bits;

    is_continue = ((attr->format != OT_FB_FORMAT_RGB888) && (attr->format != OT_FB_FORMAT_ARGB8888) &&
                   (attr->format != OT_FB_FORMAT_ARGB1555) && (attr->format != OT_FB_FORMAT_ARGB4444));

    if (is_continue == TD_TRUE) {
        return;
    }

    if (attr->format == OT_FB_FORMAT_ARGB8888) {
        osd_mode = 1;
        comp_ratio = 2000; /* 2000 for zme */
    } else if (attr->format == OT_FB_FORMAT_RGB888) {
        osd_mode = 0;
        comp_ratio = 1000; /* 1000 for zme */
    } else if (attr->format == OT_FB_FORMAT_ARGB1555) {
        osd_mode = 2; /* 2 for zme */
        comp_ratio = 1000; /* 1000 for zme */
    } else {
        osd_mode = 3; /* 3 for zme */
        comp_ratio = 1000; /* 1000 for zme */
    }

    bit_dep_a = (osd_mode <= 1) ? 8 : ((osd_mode == 2) ? 1 : 4); /* 8 2 4 alg data */
    bit_dep_rgb = (osd_mode <= 1) ? 8 : ((osd_mode == 2) ? 5 : 4); /* 8 2 5 4 alg data */

    /* for calculate budget_mb_bits */
    last_mb_width = (attr->width % 32) ? (attr->width % 32) : 32; /* 32 alg data */
    mb_num_x = (attr->width + 32 - 1) / 32; /* 32 alg data */
    /* 2 9 7946 7000 18 8082 7000 alg data */
    buffer_init_bits = (osd_mode < 2) ? ((mb_num_x < 9) ? 7946 : 7000) : ((mb_num_x < 18) ? 8082 : 7000);
    /* 9216 alg data */
    delta = gfbg_node_max(1, (9216 - buffer_init_bits + mb_num_x - 1) / (mb_num_x));
    mb_ori_bits = 32 * (bit_dep_rgb * 3 + bit_dep_a); /* 32 3 alg data */
    budget_mb_bits = mb_ori_bits * 1000 / comp_ratio - delta; /* 1000 alg data */
    budget_mb_bits = gfbg_node_min(gfbg_node_max(budget_mb_bits, 64), mb_ori_bits); /* 64 alg data */
    budget_mb_bits_last = (budget_mb_bits * last_mb_width) / 32; /* 32 alg data */
    last_mb_extra_bits = budget_mb_bits - budget_mb_bits_last;

    if (cmp_stride != NULL) {
        /* 3 1000 126 alg data */
        *cmp_stride = (attr->width * (bit_dep_a + bit_dep_rgb * 3) * 1000 / comp_ratio + last_mb_extra_bits + 126) /
                       127 + 1; /* 127 alg data */
        *cmp_stride = *cmp_stride * 16; /* 16 alg data */
    }

    if (uncmp_stride != NULL) {
        /* 3 8 16 alg data */
        *uncmp_stride = ((attr->width * (bit_dep_a + bit_dep_rgb * 3) / 8) + 16 - 1) & (~(16 - 1));
    }
    return;
}
#endif

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
static td_s32 gfbg_get_mode_and_ratio(ot_fb_color_format format, td_u32 *osd_mode, td_u32 *comp_ratio)
{
    td_bool is_continue;
    is_continue = ((format != OT_FB_FORMAT_ARGB8888) && (format != OT_FB_FORMAT_RGBA8888) &&
                   (format != OT_FB_FORMAT_ABGR8888) && (format != OT_FB_FORMAT_RGB888) &&
                   (format != OT_FB_FORMAT_BGR888) && (format != OT_FB_FORMAT_ARGB1555) &&
                   (format != OT_FB_FORMAT_ABGR1555) && (format != OT_FB_FORMAT_ARGB4444));

    if (is_continue == TD_TRUE) {
        return TD_FAILURE;
    }

    if (format == OT_FB_FORMAT_ARGB8888) {
        *osd_mode = 0;
        *comp_ratio = 2000; /* 2000 for zme */
    } else if (format == OT_FB_FORMAT_RGB888) {
        *osd_mode = 1;
        *comp_ratio = 1000; /* 1000 for zme */
    } else if (format == OT_FB_FORMAT_ARGB1555) {
        *osd_mode = 2; /* 2 for zme */
        *comp_ratio = 1000; /* 1000 for zme */
    } else {
        *osd_mode = 3; /* 3 for zme */
        *comp_ratio = 1000; /* 1000 for zme */
    }

    return TD_SUCCESS;
}
td_void gfbg_recalculate_stride(td_u32 *cmp_stride, td_u32 *uncmp_stride, const gfbg_stride_attr *attr)
{
    td_u32 exp_num, extend_width;
    td_u32 bit_dep_a, bit_dep_rgb, osd_mode;
    td_u32 comp_ratio;
    td_s32 ret;

    ret = gfbg_get_mode_and_ratio(attr->format, &osd_mode, &comp_ratio);
    if (ret != TD_SUCCESS) {
        return;
    }

    extend_width  = (attr->width + 31) / 32 * 32; /* 31 32 for align */
    bit_dep_a = (osd_mode == 0) ? 8 : ((osd_mode == 1) ? 6 : ((osd_mode == 2) ? 1 : 4)); /* 8 6 2 4 alg data */
    bit_dep_rgb = (osd_mode == 0) ? 8 : ((osd_mode == 1) ? 8 : ((osd_mode == 2) ? 5 : 4)); /* 8 2 5 4 alg data */

    if ((osd_mode == 0) || (osd_mode == 1)) {
        exp_num = (attr->width <= 320) ? 2 : ((attr->width <= 720) ? 2 : 14); /* 320 720 width, 2 14 alg data */
    } else {
        exp_num = (attr->width <= 720) ? 2 : 0; /* 720 width, 2 alg data */
    }

    if (cmp_stride != NULL) {
        /* 3 1000 127 128 alg data */
        *cmp_stride = (extend_width * (bit_dep_a + bit_dep_rgb * 3) * 1000 / comp_ratio + 127) / 128 + exp_num;

        if ((attr->is_lossless != TD_FALSE) || (attr->is_losslessa != TD_FALSE)) {
            /* 3 13 9 10 128 alg data */
            *cmp_stride = (((attr->width * (bit_dep_a + bit_dep_rgb * 3)) * 13 + 9) / 10) / 128;
        }
        /* 16 alg data */
        *cmp_stride = *cmp_stride * 16;
    }

    if (uncmp_stride != NULL) {
        /* 3 8 16 alg data */
        *uncmp_stride = ((attr->width * (bit_dep_a + bit_dep_rgb * 3) / 8) + 16 - 1) & (~(16 - 1));
    }
    return;
}
#endif
