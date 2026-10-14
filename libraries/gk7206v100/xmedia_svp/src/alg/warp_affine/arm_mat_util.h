#ifndef ARM_ARM_MAT_UTIL_H_
#define ARM_ARM_MAT_UTIL_H_

#include <string.h>
#include <sys/time.h>
#include <cstdlib>

#define TNN_USE_NEON

#ifdef TNN_USE_NEON
#include <arm_neon.h>

#ifdef __aarch64__
#define VQMOVN_HIGH_S32_T(lows16, highs32) vqmovn_high_s32((lows16), (highs32))
#define VMOVL_HIGH_S16_T(a) vmovl_high_s16(a)
#define VCVTAQ_S32_F32(a) vcvtaq_s32_f32(a)
#define VPADDQ_S32(a, b) vpaddq_s32(a, b)

#else
#define VQMOVN_HIGH_S32_T(lows16, highs32) vcombine_s16((lows16), vqmovn_s32(highs32))
#define VMOVL_HIGH_S16_T(a) vmovl_s16(vget_high_s16(a))

// fp32 round to nearest

// method1: trick convert for float, only accurate when abs(a) < 1.5 * 2^22, (problematic when fractional part is exactly 0.5)
// const float32x4_t kNeonClampNumberf = vdupq_n_f32(12582912.0f);
// const int32x4_t kNeonClampNumberi   = vdupq_n_s32(0x4B400000);
// #define VCVTAQ_S32_F32(a) (vsubq_s32(vreinterpretq_s32_f32(vaddq_f32(a, kNeonClampNumberf)), kNeonClampNumberi))

// method2: follow definition
const int32x4_t kNeonSigni = vdupq_n_s32(1 << 31);
const int32x4_t kNeon05i   = vreinterpretq_s32_f32(vdupq_n_f32(0.5f));
#define VCVTAQ_S32_F32(a)                                                                                               \
    vcvtq_s32_f32(vaddq_f32(a, vreinterpretq_f32_s32(vorrq_s32(kNeon05i, vandq_s32(kNeonSigni, vreinterpretq_s32_f32(a))))))

#define VPADDQ_S32(a, b)                                                                                                \
    vcombine_s32(vpadd_s32(vget_low_s32(a), vget_high_s32(a)), vpadd_s32(vget_low_s32(b), vget_high_s32(b)))

#endif

#endif

/* Unsigned.  */
typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;

// #include "tnn/core/blob.h"
// #include "tnn/core/macro.h"
// #include "tnn/utils/bfp16.h"

#define GET_OFFSET_PTR(ptr, offset) (reinterpret_cast<int8_t*>(ptr) + offset)

typedef enum {
    INTERP_TYPE_NEAREST = 0x00,
    INTERP_TYPE_LINEAR  = 0x01,
} InterpType;

typedef enum {
    BORDER_TYPE_CONSTANT = 0x00,
    BORDER_TYPE_REFLECT  = 0x01,
    BORDER_TYPE_EDGE     = 0x02,
} BorderType;

struct WarpAffineParam {
    float transform[2][3];
    InterpType interp_type = INTERP_TYPE_NEAREST;
    BorderType border_type = BORDER_TYPE_CONSTANT;
    float border_val       = 0.0f;
};

void MatMemcpy2D(void* src, void* dst, int width, int height, int src_stride, int dst_stride);

// resize
void ResizeBilinearC1(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeBilinearC2(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeBilinearC3(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeBilinearC4(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeBilinearYUV420sp(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);

void ResizeNearestC1(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeNearestC2(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeNearestC3(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeNearestC4(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);
void ResizeNearestYUV420sp(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h);

// warp affine
void WarpAffineBilinearC1(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h,
                          const float (*transform)[3], const float border_val = 0.0);
void WarpAffineBilinearC1_Stride(const uint8_t* src, int batch, int src_w, int src_s, int src_h, uint8_t* dst, int w, int s, int h,
                          const float (*transform)[3], const float border_val = 0.0);
void WarpAffineBilinearC2(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h,
                          const float (*transform)[3], const float border_val = 0.0);
void WarpAffineBilinearC3(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h,
                          const float (*transform)[3], const float border_val = 0.0);
void WarpAffineBilinearC3_Planar(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h,
                          const float (*transform)[3], const float border_val = 0.0);
void WarpAffineBilinearC4(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h,
                          const float (*transform)[3], const float border_val = 0.0);
void WarpAffineBilinearYUV420sp(const uint8_t* src, int batch, int src_w, int src_h, uint8_t* dst, int w, int h,
                                float (*transform)[3], const float border_val = 0.0);

#endif
