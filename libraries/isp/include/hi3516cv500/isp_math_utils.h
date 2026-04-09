/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_math_utils.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_MATH_UTILS_H__
#define __ISP_MATH_UTILS_H__

#include "hi_type.h"
#include "hi_common_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_u8  idx_low;
    hi_u8  idx_high;
    hi_u32 cur_iso;
    hi_u32 iso_low;
    hi_u32 iso_high;
} linear_inter_info;

#define isp_bitmask(bit)      ((1 << (bit)) - 1)
#define isp_bitfix(bit)       ((1 << (bit)))
#define isp_sqr(x)            ((x) * (x))

hi_u8  sqrt16(hi_u32 arg);
hi_u8  log16(hi_u32 arg);
hi_u16 sqrt32(hi_u32 arg);
hi_void get_linear_inter_info(hi_vi_pipe vi_pipe, linear_inter_info *inter_info);
hi_s32 linear_inter(hi_s32 v, hi_s32 x0, hi_s32 y0, hi_s32 x1, hi_s32 y1);
hi_s32 calc_mul_coef(hi_s32 x0, hi_s32 y0, hi_s32 x1, hi_s32 y1, hi_u8 sft);
hi_u32 math_log2(const hi_u32 val, const hi_u8 out_precision, const hi_u8 shift_out);
hi_u32 log2_int_to_fixed(const hi_u32 val, const hi_u8 out_precision, const hi_u8 shift_out);
hi_u32 math_exp2(hi_u32 val, const unsigned char shift_in, const unsigned char shift_out);
hi_s32 solving_lin_equation_a(hi_s32 y1, hi_s32 y2, hi_s32 x1, hi_s32 x2, hi_u16 a_fraction_size);
hi_s32 solving_lin_equation_b(hi_s32 y1, hi_s32 a, hi_s32 x1, hi_u16 a_fraction_size);
hi_u32 div_fixed(hi_u32 a, hi_u32 b, const hi_u16 fraction_size);
hi_s32 solving_nth_root_045(hi_s32 x, const hi_u16 fraction_size);
hi_u32 transition(hi_u32 *lut_in, hi_u32 *lut_out, hi_u32 lut_size, hi_u32 value, hi_u32 value_fraction_size);
hi_s64 signed_right_shift(hi_s64 value, hi_u8 bit_shift);
hi_s64 signed_left_shift(hi_s64 value, hi_u8 bit_shift);
void memset_u16(hi_u16 *vir, hi_u16 temp, hi_u32 size);
void memset_u32(hi_u32 *vir, hi_u32 temp, hi_u32 size);
hi_u16 complement_to_direct_u16(hi_s16 value);
hi_s16 direct_to_complement_u16(hi_u16 value);

hi_u8  get_iso_index(hi_u32 iso);
hi_u32 get_iso(hi_u8 index);
const hi_u32 *get_iso_lut(hi_void);

void *isp_malloc(unsigned long size);
#define isp_free(ptr)         \
    do {                      \
        if ((ptr) != HI_NULL) { \
            free(ptr);        \
            (ptr) = HI_NULL;    \
        }                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  /* __ISP_MATH_UTILS_H__ */
