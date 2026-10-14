/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef MATH_FUN_H
#define MATH_FUN_H

#include "xmedia_type.h"

#ifdef __cplusplus
extern "C"{
#endif

#define ABS(x)            ((x) >= 0 ? (x) : (-(x)))
#define _SIGN(x)          ((x) >= 0 ? 1 : -1)
#define CMP(x, y)         (((x) == (y)) ? 0 : (((x) > (y)) ? 1 : -1))

#define MAX2(x, y)        ((x) > (y) ? (x) : (y))
#define MIN2(x, y)        ((x) < (y) ? (x) : (y))
#define MAX3(x, y, z)     ((x) > (y) ? MAX2(x, z) : MAX2(y, z))
#define MIN3(x, y, z)     ((x) < (y) ? MIN2(x, z) : MIN2(y, z))
#define MEDIAN(x, y, z)   (((x) + (y) + (z) - MAX3(x, y, z)) - MIN3(x, y, z))
#define MEAN2(x, y)       (((x) + (y)) >> 1)

#define CLIP_MIN(x, min)           (((x) >= (min)) ? (x) : (min))
#define CLIP3(x, min, max)         ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define CLIP_MAX(x, max)           ((x) > (max) ? (max) : (x))
#define WRAP_MAX(x, max, min)      ((x) >= (max) ? (min) : (x))
#define WRAP_MIN(x, min, max)      ((x) <= (min) ? (max) : (x))
#define VALUE_BETWEEN(x, min, max) (((x) >= (min)) && ((x) <= (max)))

#define MULTI_OF_2_POWER(x, a)     (!((x) & ((a) - 1)))
#define CEILING(x, a)              (((x) + (a) - 1) / (a))

#define ALIGN_UP(x, a)             ((((x) + ((a) - 1)) / (a)) * (a))
#define XM_ALIGN_DOWN(x, a)        (((x) / (a)) * (a))

#define DIV_UP(x, a)               (((x) + ((a) - 1) ) / (a))

#define MPP_ALIGN_UP(x, a)         ALIGN_UP(x, a)
#define MPP_ALIGN_DOWN(x, a)       XM_ALIGN_DOWN(x, a)

/******************************************************************************
** Get the span between two unsinged number, such as
** SPAN(xmedia_u32, 100, 200) is 200 - 100 = 100
** SPAN(xmedia_u32, 200, 100) is 0xFFFFFFFF - 200 + 100
** SPAN(xmedia_u64, 200, 100) is 0xFFFFFFFFFFFFFFFF - 200 + 100
******************************************************************************/
#define SPAN(type, begin, end) \
({                             \
   type b = (begin);           \
   type e = (end);             \
   (type)((b >= e) ? (b - e) : (b + ((~((type)0)) - e))); \
})

#define ENDIAN32(x)                    \
        (((x) << 24) |                 \
        (((x) & 0x0000ff00) << 8) |    \
        (((x) & 0x00ff0000) >> 8) |    \
        (((x) >> 24) & 0x000000ff))

#define ENDIAN16(x) ((((x) << 8) & 0xff00) | (((x) >> 8) & 255))

__inline static xmedia_bool is_little_end(void)
{
    union {
        xmedia_char test[4];
        xmedia_u32  u32Test;
    } end_test;

    end_test.test[0] = 0x01;
    end_test.test[1] = 0x02;
    end_test.test[2] = 0x03;
    end_test.test[3] = 0x04;

    return (end_test.u32Test > 0x01020304) ? (XMEDIA_TRUE) : (XMEDIA_FALSE);
}

#define FRACTION32(de, nu)      (((de) << 16) | (nu))
#define NUMERATOR32(x)          ((x) & 0xffff)
#define DENOMINATOR32(x)        ((x) >> 16)

#define RGB(r, g, b) ((((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define RGB_R(c)     (((c) & 0xff0000) >> 16)
#define RGB_G(c)     (((c) & 0xff00) >> 8)
#define RGB_B(c)     ((c) & 0xff)

#define YUV(y, u, v) ((((y) & 0x03ff) << 20) | (((u) & 0x03ff) << 10) | ((v) & 0x03ff))
#define YUV_Y(c)     (((c) & 0x3ff00000) >> 20)
#define YUV_U(c)     (((c) & 0x000ffc00) >> 10)
#define YUV_V(c)     ((c) & 0x000003ff)

#define YUV_8BIT(y, u, v) ((((y) & 0xff) << 16) | (((u) & 0xff) << 8) | ((v) & 0xff))
#define YUV_8BIT_Y(c)     (((c) & 0xff0000) >> 16)
#define YUV_8BIT_U(c)     (((c) & 0xff00) >> 8)
#define YUV_8BIT_V(c)     ((c) & 0xff)

__inline static xmedia_void rgb_to_yc(xmedia_u16 r, xmedia_u16 g, xmedia_u16 b, xmedia_u16 * py, xmedia_u16 * pcb, xmedia_u16 * pcr)
{
    *py = (xmedia_u16)((((r * 66 + g * 129 + b * 25) >> 8) + 16) << 2);

    *pcb = (xmedia_u16)(((((b * 112 - r * 38) - g * 74) >> 8) + 128) << 2);

    *pcr = (xmedia_u16)(((((r * 112 - g * 94) - b * 18) >> 8) + 128) << 2);
}

__inline static xmedia_u32 rgb_to_yuv(xmedia_u32 rgb)
{
    xmedia_u16 y,u,v;

    rgb_to_yc(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV(y,u,v);
}

__inline static xmedia_void rgb_to_yc_full(xmedia_u16 r, xmedia_u16 g, xmedia_u16 b, xmedia_u16 *py, xmedia_u16 *pcb, xmedia_u16 *pcr)
{
    xmedia_u16 py_temp, pcb_temp, pcr_temp;

    py_temp  = (xmedia_u16)(((r * 76 + g * 150 + b * 29) >> 8) * 4);
    pcb_temp = (xmedia_u16)(CLIP_MIN(((((b * 130 - r * 44 ) - g * 86) >> 8) + 128),0) * 4);
    pcr_temp = (xmedia_u16)(CLIP_MIN(((((r * 130 - g * 109) - b * 21) >> 8) + 128),0) * 4);

    *py = MAX2(MIN2(py_temp, 1023), 0);
    *pcb = MAX2(MIN2(pcb_temp, 1023), 0);
    *pcr = MAX2(MIN2(pcr_temp, 1023), 0);
}

__inline static xmedia_u32 rgb_to_yuv_full(xmedia_u32 rgb)
{
    xmedia_u16 y,u,v;

    rgb_to_yc_full(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV(y,u,v);
}

__inline static xmedia_void rgb_to_yc_8bit(xmedia_u8 r, xmedia_u8 g, xmedia_u8 b, xmedia_u8 *py, xmedia_u8 *pcb, xmedia_u8 *pcr)
{
    *py = (xmedia_u8)(((r * 66 + g * 129 + b * 25) >> 8) + 16);

    *pcb = (xmedia_u8)((((b * 112 - r * 38) - g * 74) >> 8) + 128);

    *pcr = (xmedia_u8)((((r * 112 - g * 94) - b * 18) >> 8) + 128);
}

__inline static xmedia_u32 rgb_to_yuv_8bit(xmedia_u32 rgb)
{
    xmedia_u8 y,u,v;

    rgb_to_yc_8bit(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV_8BIT(y,u,v);
}

__inline static xmedia_void rgb_to_yc_full_8bit(xmedia_u8 r, xmedia_u8 g, xmedia_u8 b, xmedia_u8 *py, xmedia_u8 *pcb, xmedia_u8 *pcr)
{
    xmedia_s16 py_temp, pcb_temp, pcr_temp;

    py_temp = (r * 76 + g * 150 + b * 29) >> 8;
    pcb_temp = (((b * 130 - r * 44) - g * 86) >> 8) + 128;
    pcr_temp = (((r * 130 - g * 109) - b * 21) >> 8) + 128;

    *py = MAX2(MIN2(py_temp, 255), 0);
    *pcb = MAX2(MIN2(pcb_temp, 255), 0);
    *pcr = MAX2(MIN2(pcr_temp, 255), 0);
}

__inline static xmedia_u32 rgb_to_yuv_full_8bit(xmedia_u32 rgb)
{
    xmedia_u8 y,u,v;

    rgb_to_yc_full_8bit(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV_8BIT(y,u,v);
}

typedef struct {
    xmedia_u32 full_fps;
    xmedia_u32 tag_fps;
    xmedia_u32 frm_key;
} fps_ctrl;

__inline static xmedia_void init_fps(fps_ctrl *frm_ctrl, xmedia_u32 full_fps, xmedia_u32 tag_fps)
{
    frm_ctrl->full_fps = full_fps;
    frm_ctrl->tag_fps = tag_fps;
    frm_ctrl->frm_key = 0;
}

__inline static xmedia_bool fps_control(fps_ctrl *frm_ctrl)
{
    xmedia_bool ret = XMEDIA_FALSE;

    frm_ctrl->frm_key += frm_ctrl->tag_fps;
    if (frm_ctrl->frm_key >= frm_ctrl->full_fps) {
        frm_ctrl->frm_key -= frm_ctrl->full_fps;
        ret = XMEDIA_TRUE;
    }

    return ret;
}

__inline static xmedia_u32 get_low_addr(xmedia_u64  phy_addr)
{
    return (xmedia_u32)phy_addr;
}

__inline static xmedia_u32 get_high_addr(xmedia_u64  phy_addr)
{
    return (xmedia_u32)(phy_addr>>32);
}

#ifdef __cplusplus
}
#endif

#endif

