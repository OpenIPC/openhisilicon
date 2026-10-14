#ifndef __XMEDIA_SVP_QUANTIZE_H__
#define __XMEDIA_SVP_QUANTIZE_H__

#include <math.h>
#include "xmedia_svp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    xmedia_float scale;
    xmedia_s32 zp;
} quanlize_param;

/**
 * @brief 量化(浮点到定点)
 *
 * @param data
 * @param scale
 * @param zp
 * @return xmedia_u8
 */
static inline xmedia_u8 quantize(xmedia_float data, xmedia_float scale, xmedia_s32 zp)
{
    return STD_MIN(255, STD_MAX(0, round(data / scale + zp)));
}

/**
 * @brief 反量化(定点到浮点)
 *
 * @param data
 * @param scale
 * @param zp
 * @return xmedia_float
 */
static inline xmedia_float dequantize(xmedia_u8 data, xmedia_float scale, xmedia_s32 zp)
{
    return (xmedia_float)(scale * (data - zp));
}

static inline xmedia_float desigmoid(xmedia_float x)
{
    return (-log((1.f / (x + 1e-8)) - 1.f));
}

#ifdef __cplusplus
}
#endif

#endif
