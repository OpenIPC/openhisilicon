/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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

GK_U8  sqrt16(GK_U32 arg);
GK_U8  log16(GK_U32 arg);
GK_U16 Sqrt32(GK_U32 u32Arg);
GK_S32 LinearInter(GK_S32 v, GK_S32 x0, GK_S32 y0, GK_S32 x1, GK_S32 y1);
GK_S32 CalcMulCoef(GK_S32 x0, GK_S32 y0, GK_S32 x1, GK_S32 y1, GK_U8 sft);
GK_U32 math_log2(const GK_U32 val, const GK_U8 out_precision, const GK_U8 shift_out);
GK_U32 log2_int_to_fixed(const GK_U32 val, const GK_U8 out_precision, const GK_U8 shift_out);
GK_U32 math_exp2(GK_U32 val, const unsigned char shift_in, const unsigned char shift_out);
GK_S32 solving_lin_equation_a(GK_S32 y1, GK_S32 y2, GK_S32 x1, GK_S32 x2, GK_S16 a_fraction_size);
GK_S32 solving_lin_equation_b(GK_S32 y1, GK_S32 a, GK_S32 x1, GK_S16 a_fraction_size);
GK_U32 div_fixed(GK_U32 a, GK_U32 b, const GK_U16 fraction_size);
GK_S32 solving_nth_root_045(GK_S32 x, const GK_U16 fraction_size);
GK_U32 transition(GK_U32 *lut_in, GK_U32 *lut_out, GK_U32 lut_size, GK_U32 value, GK_U32 value_fraction_size);
GK_S64 SignedRightShift(GK_S64 s64Value, GK_U8 u8BitShift);
GK_S64 SignedLeftShift(GK_S64 s64Value, GK_U8 u8BitShift);
void MemsetU16(GK_U16 *pVir, GK_U16 temp, GK_U32 size);
void MemsetU32(GK_U32 *pVir, GK_U32 temp, GK_U32 size);

GK_U8 GetIsoIndex(GK_U32 u32Iso);

void *ISP_MALLOC(unsigned long size);
#define ISP_FREE(ptr)         \
    do {                      \
        if (ptr != GK_NULL) { \
            free(ptr);        \
            ptr = GK_NULL;    \
        }                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // __ISP_MATH_UTILS_H__
