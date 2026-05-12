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
    IMX415_8M_30FPS_10BIT_LINEAR_MODE                = 0, /* 4K all-pixel, 891 Mbps */
    IMX415_8M_20FPS_10BIT_LINEAR_MODE                = 1, /* 4K all-pixel, 594 Mbps */
    IMX415_2M_60FPS_12BIT_LINEAR_MODE                = 2, /* 1080p binning, 891 Mbps */
    IMX415_8M_60FPS_10BIT_LINEAR_MODE                = 3, /* 4K @ 60 fps, 1485 Mbps */
    IMX415_CROP_FLEX_LINEAR_MODE                     = 4, /* Window crop (full T1H),
                                                           * flexible W×H, fps≤54 */
    IMX415_CROP_BIN_FLEX_LINEAR_MODE                 = 5, /* Window crop on 2x2 binned
                                                           * sensor: 1H=7.26µs gives
                                                           * fps_max≈120; flexible W×H */
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

/* IMX415 timing constants (per Sony IMX415-AAQR-C datasheet and Linux mainline
 * drivers/media/i2c/imx415.c).
 *
 * Frame-rate formula:
 *   fps = 1 / (VMAX × T1H)
 * where VMAX is the per-frame line count (register 0x3024-0x3026, 20-bit)
 * and T1H is the per-line period set by the operating mode (SYS_MODE,
 * INCKSEL*, HMAX). Within a single SYS_MODE the T1H is fixed by the lane
 * rate — only VMAX (vertical blanking) is adjustable for fps tuning.
 *
 * VMAX has a per-mode minimum equal to (output_height + min_vblank). The
 * mainline kernel driver uses min_vblank = 58 (IMX415_PIXEL_ARRAY_VBLANK).
 * Below this, the sensor truncates the active area or stalls. */
#define IMX415_VBLANK_MIN           58
#define IMX415_FULL_LINES_MAX       0xFFFFF /* VMAX is 20-bit; SHR0 caps lower */

/* T1H period per Sony datasheet page 54-55 (CSI-2 4-lane), in nanoseconds.
 * These are the line periods at the configured lane rate. They are FIXED
 * per SYS_MODE — changing them requires a full mode switch (different
 * INCKSEL + D-PHY timing block). */
#define IMX415_T1H_NS_891MBPS_4K    14900   /* SYS_MODE=0x05, all-pixel 4K */
#define IMX415_T1H_NS_594MBPS_4K    22300   /* SYS_MODE=0x07, all-pixel 4K */
#define IMX415_T1H_NS_891MBPS_2M    7260    /* SYS_MODE=0x05, 1080p binning */
#define IMX415_T1H_NS_1485MBPS_4K   7500    /* SYS_MODE=0x08, 4K @ 60 fps */

/* Window-cropping mode (WINMODE=4h) constraints per IMX415-AAQR-C datasheet
 * page 63. Cropping reuses the operating-mode 1H period and HMAX; only the
 * effective active area shrinks.
 *
 *   VTTL ≥ max((PIX_VWIDTH / 2) + 46, 1222)
 *
 * The 1222-line floor dominates for any crop height ≤ 2352, so window-crop
 * mode caps at fps_max = 1 / (1222 × T1H) regardless of crop height.
 * At 891 Mbps (T1H = 14.9 µs): fps_max ≈ 54.9.
 *
 * Crop register multiples:
 *   PIX_HST    multiple of 2  (pixel units, 13-bit)
 *   PIX_HWIDTH multiple of 24 (pixel units, 13-bit)
 *   PIX_VST    multiple of 4  (V units = lines × 2, 13-bit)
 *   PIX_VWIDTH multiple of 4  (V units = lines × 2, 13-bit)
 *
 * Compile-time default = centred 1920×1080 crop of the 3840×2160 active
 * sensor area. Future work: expose a sensor extension API to retarget at
 * runtime (PIX_HST / PIX_HWIDTH / PIX_VST / PIX_VWIDTH are mid-stream
 * writable on the IMX415; one invalid frame is generated when crop
 * geometry changes per page 62). */
#define IMX415_VMAX_MIN_CROP        1222
#define IMX415_CROP_W_DEFAULT       1920
#define IMX415_CROP_H_DEFAULT       1080
#define IMX415_SENSOR_W             3840
#define IMX415_SENSOR_H             2160

/* Window cropping registers (datasheet page 63) */
#define IMX415_WINMODE_ADDR         0x301C  /* [3:0] = 4h for crop mode */
#define IMX415_PIX_HST_LOW          0x3040  /* horiz start, pixel units */
#define IMX415_PIX_HST_HIGH         0x3041
#define IMX415_PIX_HWIDTH_LOW       0x3042  /* horiz width, pixel units */
#define IMX415_PIX_HWIDTH_HIGH      0x3043
#define IMX415_PIX_VST_LOW          0x3044  /* vert start, V units */
#define IMX415_PIX_VST_HIGH         0x3045
#define IMX415_PIX_VWIDTH_LOW       0x3046  /* vert width, V units */
#define IMX415_PIX_VWIDTH_HIGH      0x3047

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __IMX415_CMOS_PRIV_H_ */
