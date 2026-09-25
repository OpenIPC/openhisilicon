/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_MATH_UTILS_H__
#define __ISP_MATH_UTILS_H__

#include "type.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_BITMASK(bit)      ((1 << (bit)) - 1)
#define ISP_BITFIX(bit)       ((1 << (bit)))
#define ISP_SQR(x)            ((x) * (x))

XMEDIA_U8  sqrt16(XMEDIA_U32 arg);
XMEDIA_U8  log16(XMEDIA_U32 arg);
XMEDIA_U16 Sqrt32(XMEDIA_U32 u32Arg);
XMEDIA_S32 LinearInter(XMEDIA_S32 v, XMEDIA_S32 x0, XMEDIA_S32 y0, XMEDIA_S32 x1, XMEDIA_S32 y1);
XMEDIA_S32 CalcMulCoef(XMEDIA_S32 x0, XMEDIA_S32 y0, XMEDIA_S32 x1, XMEDIA_S32 y1, XMEDIA_U8 sft);
XMEDIA_U32 math_log2(const XMEDIA_U32 val, const XMEDIA_U8 out_precision, const XMEDIA_U8 shift_out);
XMEDIA_U32 log2_int_to_fixed(const XMEDIA_U32 val, const XMEDIA_U8 out_precision, const XMEDIA_U8 shift_out);
XMEDIA_U32 math_exp2(XMEDIA_U32 val, const unsigned char shift_in, const unsigned char shift_out);
XMEDIA_S32 solving_lin_equation_a(XMEDIA_S32 y1, XMEDIA_S32 y2, XMEDIA_S32 x1, XMEDIA_S32 x2, XMEDIA_S16 a_fraction_size);
XMEDIA_S32 solving_lin_equation_b(XMEDIA_S32 y1, XMEDIA_S32 a, XMEDIA_S32 x1, XMEDIA_S16 a_fraction_size);
XMEDIA_U32 div_fixed(XMEDIA_U32 a, XMEDIA_U32 b, const XMEDIA_U16 fraction_size);
XMEDIA_S32 solving_nth_root_045(XMEDIA_S32 x, const XMEDIA_U16 fraction_size);
XMEDIA_U32 transition(XMEDIA_U32 *lut_in, XMEDIA_U32 *lut_out, XMEDIA_U32 lut_size, XMEDIA_U32 value, XMEDIA_U32 value_fraction_size);
XMEDIA_S64 SignedRightShift(XMEDIA_S64 s64Value, XMEDIA_U8 u8BitShift);
XMEDIA_S64 SignedLeftShift(XMEDIA_S64 s64Value, XMEDIA_U8 u8BitShift);
void MemsetU16(XMEDIA_U16 *pVir, XMEDIA_U16 temp, XMEDIA_U32 size);
void MemsetU32(XMEDIA_U32 *pVir, XMEDIA_U32 temp, XMEDIA_U32 size);

XMEDIA_U8 GetIsoIndex(XMEDIA_U32 u32Iso);

void *ISP_MALLOC(unsigned long size);
#define ISP_FREE(ptr)         \
    do {                      \
        if (ptr != XMEDIA_NULL) { \
            free(ptr);        \
            ptr = XMEDIA_NULL;    \
        }                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // __ISP_MATH_UTILS_H__
