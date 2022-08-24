/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_math_utils.h"
#include "math_fun.h"
#include "isp_debug.h"
#include <stdio.h>
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_U8 sqrt16(GK_U32 arg)
{
	GK_U8 mask = 128;
	GK_U8 res = 0;
	GK_U8 i = 0;

	for (i = 0; i < 8; i++) {
		if ((res + (mask >> i)) * (res + (mask >> i)) <= arg) {
			res = res + (mask >> i);
		}
	}

	return res;
}

GK_U8 log16(GK_U32 arg)
{
	GK_U8 k = 0;
	GK_U8 res = 0;

	for (k = 0; k < 16; k++) {
		if (arg > (1 << k)) {
			res = (k << 4) + ((arg << 4) / (1 << (k)));
		}
	}
	return res;
}

GK_U16 Sqrt32(GK_U32 u32Arg)
{
	GK_U32 u32Mask = (GK_U32)1 << 15;
	GK_U16 u16Res = 0;
	GK_U32 i = 0;

	for (i = 0; i < 16; i++) {
		if ((u16Res + (u32Mask >> i)) * (u16Res + (u32Mask >> i)) <=
		    u32Arg) {
			u16Res = u16Res + (u32Mask >> i);
		}
	}

	/* rounding */
	if (u16Res * u16Res + u16Res < u32Arg) {
		++u16Res;
	}

	return u16Res;
}

GK_S32 LinearInter(GK_S32 v, GK_S32 x0, GK_S32 y0, GK_S32 x1, GK_S32 y1)
{
	GK_S32 res;

	if (v <= x0) {
		return y0;
	}
	if (v >= x1) {
		return y1;
	}

	res = (GK_S64)(y1 - y0) * (GK_S64)(v - x0) / (x1 - x0) + y0;
	return res;
}

GK_S32 CalcMulCoef(GK_S32 x0, GK_S32 y0, GK_S32 x1, GK_S32 y1, GK_U8 sft)
{
	if (x0 == x1) {
		return 0;
	} else {
		return (SignedLeftShift((y1 - y0), sft) / (x1 - x0));
	}
}

static GK_U8 leading_one_position(const GK_U32 in)
{
	GK_U8 pos = 0;
	GK_U32 val = in;
	if (val >= 1 << 16) {
		val >>= 16;
		pos += 16;
	}
	if (val >= 1 << 8) {
		val >>= 8;
		pos += 8;
	}
	if (val >= 1 << 4) {
		val >>= 4;
		pos += 4;
	}
	if (val >= 1 << 2) {
		val >>= 2;
		pos += 2;
	}
	if (val >= 1 << 1) {
		pos += 1;
	}
	return pos;
}

//  y = log2(x)
//  input:  Integer: val
//  output: Fixed point x.y
//  y: out_precision

GK_U32 log2_int_to_fixed(const GK_U32 val, const GK_U8 out_precision,
			 const GK_U8 shift_out)
{
	int i;
	int pos = 0;
	GK_U32 a = 0;
	GK_U32 b = 0;
	GK_U32 in = val;
	GK_U32 result = 0;
	const unsigned char precision = out_precision;

	if (val == 0) {
		return 0;
	}
	// integral part
	pos = leading_one_position(val);
	// fractional part
	a = (pos <= 15) ? (in << (15 - pos)) : (in >> (pos - 15));
	for (i = 0; i < precision; ++i) {
		b = a * a;
		if (b & (1U << 31)) {
			result = (result << 1) + 1;
			a = b >> 16;
		} else {
			result = (result << 1);
			a = b >> 15;
		}
	}
	return (GK_U32)((((GK_U32)SignedLeftShift(pos, precision) + result)
			 << shift_out) |
			((a & 0x7fff) >> (15 - shift_out)));
}

GK_U32 math_log2(const GK_U32 val, const GK_U8 out_precision,
		 const GK_U8 shift_out)
{
	int i;
	int pos = 0;
	GK_U32 a = 0;
	GK_U32 b = 0;
	GK_U32 in = val;
	GK_U32 result = 0;
	const unsigned char precision = out_precision;

	if (val == 0) {
		return 0;
	}

	/* integral part */
	pos = leading_one_position(val);

	/* fractional part */
	a = (pos <= 15) ? (in << (15 - pos)) : (in >> (pos - 15));
	for (i = 0; i < precision; ++i) {
		b = a * a;
		if (b & (1U << 31)) {
			result = (result << 1) + 1;
			a = b >> 16;
		} else {
			result = (result << 1);
			a = b >> 15;
		}
	}

	return (GK_U32)(((GK_U32)SignedLeftShift(pos, precision) + result)
			<< shift_out);
}

static GK_U32 _pow2_lut[33] = {
	1073741824, 1097253708, 1121280436, 1145833280, 1170923762, 1196563654,
	1222764986, 1249540052, 1276901417, 1304861917, 1333434672, 1362633090,
	1392470869, 1422962010, 1454120821, 1485961921, 1518500250, 1551751076,
	1585730000, 1620452965, 1655936265, 1692196547, 1729250827, 1767116489,
	1805811301, 1845353420, 1885761398, 1927054196, 1969251188, 2012372174,
	2056437387, 2101467502, 2147483647
};

GK_U32 math_exp2(GK_U32 val, const unsigned char shift_in,
		 const unsigned char shift_out)
{
	unsigned int fract_part = (val & ((1 << shift_in) - 1));
	unsigned int int_part = val >> shift_in;
	if (shift_in <= 5) {
		unsigned int lut_index = fract_part << (5 - shift_in);
		return _pow2_lut[lut_index] >> (30 - shift_out - int_part);
	} else {
		unsigned int lut_index = fract_part >> (shift_in - 5);
		unsigned int lut_fract = fract_part &
					 ((1 << (shift_in - 5)) - 1);
		unsigned int a = _pow2_lut[lut_index];
		unsigned int b = _pow2_lut[lut_index + 1];
		unsigned int res = ((unsigned long long)(b - a) * lut_fract) >>
				   (shift_in - 5);
		res = (res + a) >> (30 - shift_out - int_part);
		return res;
	}
}

/*  Linear equation solving
 *
 *     y1 = a * x1 + b
 *     y2 = a * x2 + b
 *     y1 - (a * x1) = y2 - (a * x2)
 *     y1 - y2 = (a * x1) - (a * x2)
 *
 *     a = (y1 - y2) / (x1 - x2)
 *     b = y1 - (a * x1)
 *
 *    Result is coefficient "a" in fixed format   x.a_fraction_size
 *    y1, y2, x1, x2 - real value format
 */
GK_S32 solving_lin_equation_a(GK_S32 y1, GK_S32 y2, GK_S32 x1, GK_S32 x2,
			      GK_S16 a_fraction_size)
{
	return ((y1 - y2) * (1 << a_fraction_size)) / DIV_0_TO_1(x1 - x2);
}
/*    Result is coefficient "b" in fixed format   x.a_fraction_size
 *    y1, x1 - real value format
 *    "a" in fixed format   x.a_fraction_size
 */
GK_S32 solving_lin_equation_b(GK_S32 y1, GK_S32 a, GK_S32 x1,
			      GK_S16 a_fraction_size)
{
	return (y1 * (1 << a_fraction_size)) - (a * x1);
}

/*     y = a / b
 *    a: fixed xxx.fraction_size
 *    b: fixed xxx.fraction_size
 */
GK_U32 div_fixed(GK_U32 a, GK_U32 b, const GK_U16 fraction_size)
{
	return (a << fraction_size) / DIV_0_TO_1(b);
}
/*    nth root finding y = x^0.45
 *  not a precise equation - for speed issue
 *    Result is coefficient "y" in fixed format   xxx.fraction_size
 */
GK_S32 solving_nth_root_045(GK_S32 x, const GK_U16 fraction_size)
{
	GK_S32 s32Value = (GK_S32)SignedLeftShift(x, fraction_size);
	GK_S32 y = (1 << fraction_size) + (s32Value / 4) - (s32Value / 8);
	return y;
}

//    Transition matching.
//
//    lut_int  - LUT for search,                    values in real format
//    lut_out  - LUT for transition matching,        values in real format
//    lut_size - size of LUT,                        value in real format
//    value    - Value for search,                value in fixed format size. xxx.value_fraction_size
//
//    ret      - output value,                    value in real format
//
//
//
//    a0             ax(value)          a1
//   |--------------|-----------------|    - lut_int
//
//  b0             bx(ret)            b1
//   |--------------|-----------------|    - lut_out
//
//
//    ax = (a1 - a0) * x + a0
//    bx = (b1 - b0) * x + b0
//
//    x = (ax - a0) / (a1 - a0)
//  bx = ((b1 - b0) * (ax - a0) / (a1 - a0)) + b0
//
GK_U32 transition(GK_U32 *lut_in, GK_U32 *lut_out, GK_U32 lut_size,
		  GK_U32 value, GK_U32 value_fraction_size)
{
	GK_U32 ret = 0;
	GK_U32 fast_search = value >> value_fraction_size;

	if (fast_search < lut_in[0]) { //    fast search for minimum.
		ret = lut_out[0];
	} else { //    fast search for maximum.
		if (fast_search >= lut_in[lut_size - 1]) {
			ret = lut_out[lut_size - 1];
		} else { //    transition matching.
			//    searching
			GK_S32 i;
			GK_S32 a0 = lut_in[0];
			GK_S32 a1 = lut_in[0];
			GK_S32 b0 = lut_out[0];
			GK_S32 b1 = lut_out[0];
			for (i = 1; i < lut_size; i++) {
				if (lut_in[i] > fast_search) {
					a0 = lut_in[i - 1];
					a1 = lut_in[i];
					b0 = lut_out[i - 1];
					b1 = lut_out[i];
					break;
				}
			}
			//    interpolation
			GK_S32 b10 = b1 - b0;
			GK_S32 ax0 = value - (GK_S32)SignedLeftShift(
						     a0, value_fraction_size);
			GK_S32 a10 = a1 - a0;
			GK_S32 result;
			if (a10 == 0) {
				result = 0;
			} else {
				result = b10 * ax0 / a10;
			}
			ret = (GK_U32)(result +
				       (GK_S32)SignedLeftShift(
					       b0, value_fraction_size));
			ret = ret >> value_fraction_size;
		}
	}
	return ret;
}

GK_S64 SignedRightShift(GK_S64 s64Value, GK_U8 u8BitShift)
{
	GK_U64 u64Value = 0;
	GK_U64 tmp = 0;
	u64Value = (GK_U64)s64Value;
	if (s64Value < 0) {
		u64Value = u64Value >> u8BitShift;
		tmp = (((0x1LL << (u8BitShift)) - 1LL)) << (64 - u8BitShift);
		u64Value = u64Value | tmp;
		return (GK_S64)u64Value;
	} else {
		return (GK_S64)(u64Value >> u8BitShift);
	}
}

GK_S64 SignedLeftShift(GK_S64 s64Value, GK_U8 u8BitShift)
{
	GK_U64 u64AbsValue = ABS(s64Value);

	if (s64Value < 0) {
		return (GK_S64)((0x1ULL << 63) |
				((~(u64AbsValue << u8BitShift)) + 1));
	} else {
		return (GK_S64)(u64AbsValue << u8BitShift);
	}
}

void *ISP_MALLOC(unsigned long size)
{
	if (size == 0) {
		return GK_NULL;
	}

	return malloc(size);
}

void MemsetU16(GK_U16 *pVir, GK_U16 temp, GK_U32 size)
{
	int i;
	for (i = 0; i < size; i++) {
		*(pVir + i) = temp;
	}
}

void MemsetU32(GK_U32 *pVir, GK_U32 temp, GK_U32 size)
{
	int i;
	for (i = 0; i < size; i++) {
		*(pVir + i) = temp;
	}
}

GK_U8 GetIsoIndex(GK_U32 u32Iso)
{
	GK_U8 u8Index;

	for (u8Index = 0; u8Index < ISP_AUTO_ISO_STRENGTH_NUM - 1; u8Index++) {
		if (u32Iso <= g_au32IsoLut[u8Index]) {
			break;
		}
	}

	return u8Index;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
