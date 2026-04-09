/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx415_cmos_priv.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX415_CMOS_PRIV_H_
#define __IMX415_CMOS_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* usefull macro */
#define IMX415_HIG_4BITS(x) (((x) & 0x0f0000) >> 16)
#define IMX415_MID_8BITS(x) (((x) & 0x00ff00) >> 8)
#define IMX415_LOW_8BITS(x) ((x) & 0x0000ff)
#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define IMX415_RES_IS_8M(w, h)    ((3840 >= (w)) && (2160 >= (h)))
#define imx415_res_is_2m(w, h)    ((1920 >= (w)) && (1080 >= (h)))

#define CHECK_RET(express)\
    do{\
        HI_S32 s32Ret;\
        s32Ret = express;\
        if (HI_SUCCESS != s32Ret) {\
            printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, s32Ret);\
        }\
    }while (0)

#define  IMX415_VMAX_ADDR_L             0x3024
#define  IMX415_VMAX_ADDR_M             0x3025
#define  IMX415_VMAX_ADDR_H             0x3026
#define  IMX415_SHR0_LOW                0x3050
#define  IMX415_SHR0_MIDDLE             0x3051
#define  IMX415_SHR0_HIGH               0x3052
#define  IMX415_GAIN_LONG_LOW           0x3090
#define  IMX415_GAIN_LONG_HIGH          0x3091

/* sensor gain */
#define IMX415_AGAIN_MIN    1024
#define IMX415_AGAIN_MAX    32381    /* the max again is 32381 */

#define IMX415_DGAIN_MIN    1024
#define IMX415_DGAIN_MAX    128914

#define IMX415_AD_GAIN_TBL_RANGE  241
#define IMX415_AGAIN_TBL_RANGE  100
#define IMX415_DGAIN_TBL_RANGE  140

typedef struct hiIMX415_SENSOR_REG_S {
    HI_U16 u16Addr;
    HI_U8  u8Data;
} IMX415_SENSOR_REG_S;

typedef enum {
    IMX415_8M_30FPS_10BIT_LINEAR_MODE                = 0,
    IMX415_8M_20FPS_10BIT_LINEAR_MODE                = 1,
    IMX415_2M_60FPS_12BIT_LINEAR_MODE                = 2,
    IMX415_MODE_BUTT
} IMX415_RES_MODE_E;

typedef struct hiIMX415_VIDEO_MODE_TBL_S {
    HI_U32 u32VMax;
    HI_U32 u32Offset;
    HI_U32 u32ExpLineMin;
    HI_U32 u32ExpLineLimit;
    HI_FLOAT f32MaxFps;
    HI_U8 u8SnsMode;
    const char *pszModeName;
} IMX415_VIDEO_MODE_TBL_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __IMX415_CMOS_PRIV_H_ */
