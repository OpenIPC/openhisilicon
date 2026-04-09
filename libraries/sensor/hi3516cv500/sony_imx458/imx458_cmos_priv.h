/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx458_cmos_priv.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX458_CMOS_PRIV_H_
#define __IMX458_CMOS_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* usefull macro */
#define HIGH_8BITS(x) (((x)&0xff00) >> 8)
#define LOW_8BITS(x) ((x)&0x00ff)
#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

#define IMX458_RES_IS_8M(w, h) (((w) <= 3840) && ((h) <= 2160))
#define IMX458_RES_IS_2M(w, h) (((w) <= 1920) && ((h) <= 1080))
#define IMX458_RES_IS_1M(w, h) (((w) <= 1280) && ((h) <= 720))
#define IMX458_RES_IS_12M(w, h) (((w) <= 4000) && ((h) <= 3000))
#define IMX458_RES_IS_4M(w, h) (((w) <= 2716) && ((h) <= 1524))

#define CHECK_RET(express)                                                                \
    do {                                                                                  \
        HI_S32 s32Ret;                                                                    \
        s32Ret = express;                                                                 \
        if (s32Ret != HI_SUCCESS) {                                                       \
            printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, s32Ret); \
        }                                                                                 \
    } while (0)

#define IMX458_VMAX_H 0x0340
#define IMX458_VMAX_L 0x0341

#define IMX458_COARSE_INTEG_TIME_L 0x0203
#define IMX458_COARSE_INTEG_TIME_H 0x0202
#define IMX458_ANA_GAIN_GLOBAL_L 0x0205
#define IMX458_ANA_GAIN_GLOBAL_H 0x0204
#define IMX458_DIG_GAIN_GLOBAL_L 0x020f
#define IMX458_DIG_GAIN_GLOBAL_H 0x020e
#define IMX458_CIT_LSHIFT 0x3002

typedef struct hiIMX458_SENSOR_REG_S {
    HI_U16 u16Addr;
    HI_U8 u8Data;
} IMX458_SENSOR_REG_S;

typedef enum {
    IMX458_1M_129FPS_10BIT_LINEAR_MODE = 0,
    IMX458_2M_90FPS_10BIT_LINEAR_MODE = 1,
    IMX458_4M_40FPS_10BIT_LINEAR_MODE = 2,
    IMX458_4M_60FPS_10BIT_LINEAR_MODE = 3,
    IMX458_8M_30FPS_10BIT_LINEAR_MODE = 4,
    IMX458_8M_30FPS_10BIT_LINEAR_SCALE_MODE = 5,
    IMX458_8M_30FPS_10BIT_LINEAR_804M_MODE = 6,
    IMX458_12M_20FPS_10BIT_LINEAR_MODE = 7,
    IMX458_MODE_BUTT
} IMX458_RES_MODE_E;

typedef struct hiIMX458_VIDEO_MODE_TBL_S {
    HI_U32 u32VMax;
    HI_U32 u32Offset;
    HI_U32 u32ExpLineMin;
    HI_U32 u32ExpLineLimit;
    HI_FLOAT f32MaxFps;
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U8  u8SnsMode;
    const char *pszModeName;
} IMX458_VIDEO_MODE_TBL_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __IMX458_CMOS_PRIV_H_ */
