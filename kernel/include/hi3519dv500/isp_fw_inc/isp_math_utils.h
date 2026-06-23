/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_MATH_UTILS_H
#define ISP_MATH_UTILS_H

#include "ot_type.h"
#include "ot_common.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    td_u8  idx_low;
    td_u8  idx_high;
    td_u32 cur_iso;
    td_u32 iso_low;
    td_u32 iso_high;
} linear_inter_info;

#define isp_bitmask(bit)      ((1 << (bit)) - 1)
#define isp_bitfix(bit)       ((1 << (bit)))
#define isp_sqr(x)            ((x) * (x))

#define U32_UP                          0x7fffffff
#define S32_UP                          0x7fffffff
#define S32_LOW                         (-0x7fffffff)

td_u8  sqrt16(td_u32 arg);
td_u8  log16(td_u32 arg);
td_u16 sqrt32(td_u32 arg);
td_void get_linear_inter_info(ot_vi_pipe vi_pipe, linear_inter_info *inter_info);
td_s32 linear_inter(td_s32 v, td_s32 x0, td_s32 y0, td_s32 x1, td_s32 y1);
td_s32 calc_mul_coef(td_s32 x0, td_s32 y0, td_s32 x1, td_s32 y1, td_u8 sft);
td_u32 math_log2(const td_u32 val, const td_u8 out_precision, const td_u8 shift_out);
td_u32 log2_int_to_fixed(const td_u32 val, const td_u8 out_precision, const td_u8 shift_out);
td_u32 math_exp2(td_u32 val, const unsigned char shift_in, const unsigned char shift_out);
td_s32 solving_lin_equation_a(td_s32 y1, td_s32 y2, td_s32 x1, td_s32 x2, td_u16 a_fraction_size);
td_s32 solving_lin_equation_b(td_s32 y1, td_s32 a, td_s32 x1, td_u16 a_fraction_size);
td_u32 div_fixed(td_u32 a, td_u32 b, const td_u16 fraction_size);
td_s32 solving_nth_root_045(td_s32 x, const td_u16 fraction_size);
td_u32 transition(td_u32 *lut_in, td_u32 *lut_out, td_u32 lut_size, td_u32 value, td_u32 value_fraction_size);
td_s64 signed_right_shift(td_s64 value, td_u8 bit_shift);
td_s64 signed_left_shift(td_s64 value, td_u8 bit_shift);
void memset_u16(td_u16 *vir, td_u16 temp, td_u32 size);
void memset_u32(td_u32 *vir, td_u32 temp, td_u32 size);
td_u16 complement_to_direct_u16(td_s16 value);
td_s16 direct_to_complement_u16(td_u16 value);

td_u8  get_iso_index(td_u32 iso);
td_u32 get_iso(td_u8 index);
const td_u32 *get_iso_lut(td_void);

void *isp_malloc(unsigned long size);
#define isp_free(ptr)         \
    do {                      \
        if ((ptr) != TD_NULL) { \
            free(ptr);        \
            (ptr) = TD_NULL;    \
        }                     \
    } while (0)

#define isp_mul_u32_limit(result, a, b) \
    do { \
        if ((a) > 0 && (b) > (U32_UP / (a))) { \
            isp_err_trace( "%u * %u upper limit of the multiplie\n", (td_u32)(a), (td_u32)(b)); \
            (result) = U32_UP; \
        } else { \
            (result) = (a) * (b); \
        } \
    } while (0)

#define isp_mul_s32_limit(result, a, b) \
    do { \
        if (((a) > 0 && (b) > 0 && (b) > (S32_UP / (a))) || ((a) < 0 && (b) < 0 && (b) < (S32_UP / (a))))  { \
            (result) = S32_UP; \
        } else if (((a) < 0 && (b) > 0 && (b) > (S32_LOW / (a))) || ((a) > 0 && (b) < 0 && (b) < (S32_LOW / (a)))) { \
            (result) = S32_LOW; \
        } else { \
            (result) = (a) * (b); \
        } \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  /* ISP_MATH_UTILS_H */
