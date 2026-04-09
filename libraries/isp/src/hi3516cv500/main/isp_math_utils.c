/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_math_utils.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_math_utils.h"
#include "hi_math.h"
#include "hi_isp_debug.h"
#include <stdio.h>
#include "isp_main.h"

hi_u8 sqrt16(hi_u32 arg)
{
    hi_u8 mask = 128;
    hi_u8 res  = 0;
    hi_u8 i = 0;
    hi_u8 loops = 8;

    for (i = 0; i < loops; i++) {
        if ((res + (mask >> i)) * (res + (mask >> i)) <= arg) {
            res = res + (mask >> i);
        }
    }

    return res;
}

hi_u8 log16(hi_u32 arg)
{
    hi_u8 k   = 0;
    hi_u8 res = 0;
    hi_u8 loops = 16;

    for (k = 0; k < loops; k++) {
        if (arg > (1 << k)) {
            res = (k << 4) + ((arg << 4) / (1 << (k))); /* left shift 4bits */
        }
    }
    return res;
}

hi_u16 sqrt32(hi_u32 arg)
{
    hi_u32 mask = (hi_u32)1 << 15;  /* left shift 15bits */
    hi_u16 res  = 0;
    hi_u32 i = 0;
    hi_u8 loops = 16;

    for (i = 0; i < loops; i++) {
        if ((res + (mask >> i)) * (res + (mask >> i)) <= arg) {
            res = res + (mask >> i);
        }
    }

    /* rounding */
    if (res * res + res < arg) {
        ++res;
    }

    return res;
}

hi_void get_linear_inter_info(hi_vi_pipe vi_pipe, linear_inter_info *inter_info)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    inter_info->cur_iso  = isp_ctx->linkage.iso;
    inter_info->idx_high = get_iso_index(inter_info->cur_iso);
    inter_info->idx_low  = MAX2((hi_s8)inter_info->idx_high - 1, 0);
    inter_info->iso_high = get_iso(inter_info->idx_high);
    inter_info->iso_low  = get_iso(inter_info->idx_low);
}

hi_s32 linear_inter(hi_s32 v, hi_s32 x0, hi_s32 y0, hi_s32 x1, hi_s32 y1)
{
    hi_s32 res;

    if (v <= x0) {
        return y0;
    }
    if (v >= x1) {
        return y1;
    }

    res = (hi_s64)(y1 - y0) * (hi_s64)(v - x0) / (x1 - x0) + y0;
    return res;
}

hi_s32 calc_mul_coef(hi_s32 x0, hi_s32 y0, hi_s32 x1, hi_s32 y1, hi_u8 sft)
{
    if (x0 == x1) {
        return 0;
    } else {
        return (signed_left_shift((y1 - y0), sft) / (x1 - x0));
    }
}

static hi_u8 leading_one_position(const hi_u32 in)
{
    hi_u8 pos = 0;
    hi_u32 val = in;
    if (val >= (1 << 16)) {  /* compare with 1 << 16 */
        val >>= 16;          /* right shift 16bits */
        pos += 16;           /* added to 16 */
    }
    if (val >= (1 << 8)) { /* compare with 1 << 8 */
        val >>= 8;         /* right shift 8bits */
        pos += 8;          /* added to 8 */
    }
    if (val >= (1 << 4)) {  /* compare with 1 << 4 */
        val >>= 4;          /* right shift 4bits */
        pos += 4;           /* added to 4 */
    }
    if (val >= (1 << 2)) {  /* compare with 1 << 2 */
        val >>= 2;          /* right shift 2bits */
        pos += 2;           /* added to 2 */
    }
    if (val >= (1 << 1)) {  /* compare with 1 << 1 */
        pos += 1;           /* added to 1 */
    }
    return pos;
}

/* y := log2(x) */
/* input:  integer: val */
/* output: fixed point x.y */
/* y: out_precision */
hi_u32 log2_int_to_fixed(const hi_u32 val, const hi_u8 out_precision, const hi_u8 shift_out)
{
    hi_u8  bit_shift;
    int i, pos;
    hi_u32 a;
    hi_u32 b = 0;
    hi_u32 in = val;
    hi_u32 result = 0;
    const unsigned char precision = out_precision;

    if (val == 0) {
        return 0;
    }
    /* integral part */
    pos = leading_one_position(val);
    /* fractional part */
    if (pos <= 15) { /* const value 15 */
        bit_shift = 15 - pos; /* const value 15 */
        a = in << bit_shift;
    } else {
        bit_shift = pos - 15; /* const value 15 */
        a = in >> bit_shift;
    }

    for (i = 0; i < precision; ++i) {
        b = a * a;
        if (b & (1U << 31)) {  /* left shift 31bits */
            result = (result << 1) + 1;
            a = b >> 16;       /* right shift 16bits */
        } else {
            result = (result << 1);
            a = b >> 15;       /* right shift 15bits */
        }
    }
    return (hi_u32)((((hi_u32)signed_left_shift(pos, precision) + result) << shift_out) |
                    ((a & 0x7fff) >> (15 - shift_out)));  /* right shift (15 - shift_out)bits */
}

hi_u32 math_log2(const hi_u32 val, const hi_u8 out_precision, const hi_u8 shift_out)
{
    int i;
    int pos;
    hi_u32 a;
    hi_u32 b = 0;
    hi_u32 in = val;
    hi_u32 result = 0;
    hi_u8  bit_shift;
    const unsigned char precision = out_precision;

    if (val == 0) {
        return 0;
    }

    /* integral part */
    pos = leading_one_position(val);
    /* fractional part */
    if (pos <= 15) { /* const value 15 */
        bit_shift = 15 - pos; /* const value 15 */
        a = in << bit_shift;
    } else {
        bit_shift = pos - 15; /* const value 15 */
        a = in >> bit_shift;
    }

    for (i = 0; i < precision; ++i) {
        b = a * a;
        if (b & (1U << 31)) {   /* left shift 31bits */
            result = (result << 1) + 1;
            a = b >> 16;   /* right shift 16bits */
        } else {
            result = (result << 1);
            a = b >> 15;   /* right shift 15bits */
        }
    }

    return (hi_u32)(((hi_u32)signed_left_shift(pos, precision) + result) << shift_out);
}
#define POW2_LUT_NUM 33
static hi_u32 g_pow2_lut[POW2_LUT_NUM] = {
    1073741824, 1097253708, 1121280436, 1145833280, 1170923762, 1196563654, 1222764986, 1249540052,
    1276901417, 1304861917, 1333434672, 1362633090, 1392470869, 1422962010, 1454120821, 1485961921,
    1518500250, 1551751076, 1585730000, 1620452965, 1655936265, 1692196547, 1729250827, 1767116489,
    1805811301, 1845353420, 1885761398, 1927054196, 1969251188, 2012372174, 2056437387, 2101467502,
    2147483647
};

hi_u32 math_exp2(hi_u32 val, const unsigned char shift_in, const unsigned char shift_out)
{
    unsigned int fract_part = (val & ((1 << shift_in) - 1));
    unsigned int int_part = val >> shift_in;
    if (shift_in <= 5) {  /* const value 5 */
        unsigned int lut_index = fract_part << (5 - shift_in); /* left shift (5-shift_in)bits */
        return g_pow2_lut[lut_index] >> (30 - shift_out - int_part); /* right shift (30 - shift_out - int_part)bits */
    } else {
        unsigned int lut_index = fract_part >> (shift_in - 5); /* right shift (shift_in-5)bits */
        unsigned int lut_fract = fract_part & ((1 << (shift_in - 5)) - 1); /* left shift (shift_in-5)bits */
        unsigned int a = g_pow2_lut[lut_index];
        unsigned int b = g_pow2_lut[lut_index + 1];
        unsigned int res = ((unsigned long long)(b - a) * lut_fract) >> (shift_in - 5); /* right shift (shift_in-5) */
        res = (res + a) >> (30 - shift_out - int_part); /* right shift (30 - shift_out - int_part)bits */
        return res;
    }
}

/* linear equation solving */
/* y1 : a * x1 + b */
/* y2 : a * x2 + b */
/* y1 - (a * x1) := y2 - (a * x2) */
/* y1 - y2 := (a * x1) - (a * x2) */
/* a : (y1 - y2) / (x1 - x2) */
/* b : y1 - (a * x1) */
/* result is coefficient "a" in fixed format   x.a_fraction_size */
/* y1, y2, x1, x2 - real value format */
hi_s32 solving_lin_equation_a(hi_s32 y1, hi_s32 y2, hi_s32 x1, hi_s32 x2, hi_u16 a_fraction_size)
{
    return ((y1 - y2) * (1 << a_fraction_size)) / div_0_to_1(x1 - x2);
}

/*    result is coefficient "b" in fixed format   x.a_fraction_size
 *    y1, x1 - real value format
 *    "a" in fixed format   x.a_fraction_size
 */
hi_s32 solving_lin_equation_b(hi_s32 y1, hi_s32 a, hi_s32 x1, hi_u16 a_fraction_size)
{
    return (y1 * (1 << a_fraction_size)) - (a * x1);
}

/*    y := a / b
 *    a: fixed xxx.fraction_size
 *    b: fixed xxx.fraction_size
 */
hi_u32 div_fixed(hi_u32 a, hi_u32 b, const hi_u16 fraction_size)
{
    return (a << fraction_size) / div_0_to_1(b);
}
/*  nth root finding y = x^0.45
 *  not a precise equation - for speed issue
 *  result is coefficient "y" in fixed format   xxx.fraction_size
 */
hi_s32 solving_nth_root_045(hi_s32 x, const hi_u16 fraction_size)
{
    hi_s32 value = (hi_s32)signed_left_shift(x, fraction_size);
    hi_s32 y = (1 << fraction_size) + (value / 4) - (value / 8); /* const value 4 and 8 */
    return y;
}

/* transition matching. */
/* lut_int  - LUT for search,                    values in real format */
/* lut_out  - LUT for transition matching,        values in real format */
/* lut_size - size of LUT,                        value in real format */
/* value    - value for search,                value in fixed format size. xxx.value_fraction_size */
/* ret      - output value,                    value in real format */
/* a0             ax(value)          a1 */
/* |--------------|-----------------|    - lut_int */
/*  */
/* b0             bx(ret)            b1 */
/* |--------------|-----------------|    - lut_out */
/* ax : (a1 - a0) * x + a0 */
/* bx : (b1 - b0) * x + b0 */
/*  */
/* x : (ax - a0) / (a1 - a0) */
/* bx : ((b1 - b0) * (ax - a0) / (a1 - a0)) + b0 */
hi_u32 transition(hi_u32 *lut_in, hi_u32 *lut_out, hi_u32 lut_size, hi_u32 value, hi_u32 value_fraction_size)
{
    hi_u32 ret;
    hi_s32 i;
    hi_u32 fast_search = value >> value_fraction_size;
    hi_s32 a0, a1, b0, b1;
    hi_s32 b10, ax0, a10, result;

    if (fast_search < lut_in[0]) {
        return lut_out[0];
    }

    if (fast_search >= lut_in[lut_size - 1]) {
        return lut_out[lut_size - 1];
    }

    /* transition matching. */
    /* searching */
    a0 = lut_in[0];
    a1 = lut_in[0];
    b0 = lut_out[0];
    b1 = lut_out[0];
    for (i = 1; i < lut_size; i++) {
        if (lut_in[i] > fast_search) {
            a0 = lut_in[i - 1];
            a1 = lut_in[i];
            b0 = lut_out[i - 1];
            b1 = lut_out[i];
            break;
        }
    }
    /* interpolation */
    b10 = b1 - b0;
    ax0 = value - (hi_s32)signed_left_shift(a0, value_fraction_size);
    a10 = a1 - a0;
    if (a10 == 0) {
        result = 0;
    } else {
        result = b10 * ax0 / a10;
    }
    ret = (hi_u32)(result + (hi_s32)signed_left_shift(b0, value_fraction_size));
    ret = ret >> value_fraction_size;

    return ret;
}

hi_s64 signed_right_shift(hi_s64 value, hi_u8 bit_shift)
{
    hi_u64 value_pos;
    hi_u64 tmp = 0;
    value_pos = (hi_u64)value;
    if (value < 0) {
        value_pos = value_pos >> bit_shift;
        tmp = (((0x1LL << (bit_shift)) - 1LL)) << (64 - bit_shift); /* left shift (64 - bit_shift)bits */
        value_pos = value_pos | tmp;
        return (hi_s64)value_pos;
    } else {
        return (hi_s64)(value_pos >> bit_shift);
    }
}

hi_s64 signed_left_shift(hi_s64 value, hi_u8 bit_shift)
{
    hi_u64 abs_value = ABS(value);

    if (value < 0) {
        return (hi_s64)((0x1ULL << 63) | ((~(abs_value << bit_shift)) + 1)); /* left shift 63bits */
    } else {
        return (hi_s64)(abs_value << bit_shift);
    }
}

void *isp_malloc(unsigned long size)
{
    if (size == 0) {
        return HI_NULL;
    }

    return malloc(size);
}

void memset_u16(hi_u16 *vir, hi_u16 temp, hi_u32 size)
{
    int i;
    for (i = 0; i < size; i++) {
        *(vir + i) = temp;
    }
}

void memset_u32(hi_u32 *vir, hi_u32 temp, hi_u32 size)
{
    int i;
    for (i = 0; i < size; i++) {
        *(vir + i) = temp;
    }
}

hi_u16 complement_to_direct_u16(hi_s16 value)
{
    hi_u16 result;

    if (value >= 0) {
        result = value;
    } else {
        result = -value;
        result |= (1 << 15);  /* left shift 15bits */
    }

    return result;
}

hi_s16 direct_to_complement_u16(hi_u16 value)
{
    hi_s16 result;

    result = value & (~(1 << 15));  /* left shift 15bits */

    if (value & (1 << 15)) {   /* left shift 15bits */
        result = -result;
    }

    return result;
}

const hi_u32 g_iso_lut[ISP_AUTO_ISO_STRENGTH_NUM] = {
    100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800
};

hi_u8 get_iso_index(hi_u32 iso)
{
    hi_u8 index;

    for (index = 0; index < ISP_AUTO_ISO_STRENGTH_NUM - 1; index++) {
        if (iso <= g_iso_lut[index]) {
            break;
        }
    }

    return index;
}

hi_u32 get_iso(hi_u8 index)
{
    return g_iso_lut[index];
}

const hi_u32 *get_iso_lut(hi_void)
{
    return &g_iso_lut[0];
}

