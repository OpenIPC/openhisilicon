/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov12870_cmos_priv.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __OV9284_CMOS_PRIV_H_
#define __OV9284_CMOS_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* usefull macro */
#define HIGHER_8BITS(x) (((x)&0xff0000) >> 16)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

#define OV9284_RES_IS_1M(w, h) (((w) <= 1280) && ((h) <= 800))

#define CHECK_RET(express)                                                                \
    do {                                                                                  \
        HI_S32 s32Ret;                                                                    \
        s32Ret = express;                                                                 \
        if (s32Ret != HI_SUCCESS) {                                                       \
            printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, s32Ret); \
        }                                                                                 \
    } while (0)

#define     OV9284_GROUP_HOLD_START                   (0x3208)
#define     OV9284_GROUP_HOLD_END1                    (0x3208)
#define     OV9284_GROUP_HOLD_END2                    (0x3208)

#define     OV9284_VMAX_H                             (0x380e)
#define     OV9284_VMAX_L                             (0x380f)

#define     OV9284_HMAX_H                             (0x380c)
#define     OV9284_HMAX_L                             (0x380d)

#define     OV9284_COARSE_INTEG_TIME_L                (0x3502)
#define     OV9284_COARSE_INTEG_TIME_M                (0x3501)
#define     OV9284_COARSE_INTEG_TIME_H                (0x3500)

#define     OV9284_DEBUG_MODE                         (0x3508)
#define     OV9284_ANA_GAIN_GLOBAL                    (0x3509)

#define     OV9284_DIG_GAIN_RED_H        (0x3400)
#define     OV9284_DIG_GAIN_RED_L        (0x3401)
#define     OV9284_DIG_GAIN_GRN_H        (0x3402)
#define     OV9284_DIG_GAIN_GRN_L        (0x3403)
#define     OV9284_DIG_GAIN_BLU_H        (0x3404)
#define     OV9284_DIG_GAIN_BLU_L        (0x3405)

typedef struct hiOV9284_SENSOR_REG_S {
    HI_U16 u16Addr;
    HI_U8 u8Data;
} OV9284_SENSOR_REG_S;

typedef enum {
    OV9284_1M_60FPS_10BIT_LINEAR_MODE = 0,
    OV9284_MODE_BUTT
} OV9284_RES_MODE_E;

typedef struct hiOV12870_VIDEO_MODE_TBL_S {
    HI_U32 u32VMax;
    HI_U32 u32Offset;
    HI_U32 u32ExpLineMin;
    HI_U32 u32ExpLineLimit;
    HI_FLOAT f32MaxFps;
    HI_U8 u8SnsMode;
    const char *pszModeName;
} OV9284_VIDEO_MODE_TBL_S;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __OV9284_CMOS_PRIV_H_ */
