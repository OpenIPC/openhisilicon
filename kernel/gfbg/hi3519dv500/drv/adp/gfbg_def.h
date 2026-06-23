/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_DEF_H
#define GFBG_DEF_H

#include "ot_type.h"
#include "gfbg_coef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VHD_REGS_LEN       0x1000 /* len of V0's regs */
#define VSD_REGS_LEN       0x1000
#define GFX_REGS_LEN       0x800
#define DHD_REGS_LEN       0x1000
#define GRF_REGS_LEN       0x200 /* len of GFX regs */

/* Offset define */
#define FDR_GFX_OFFSET     0x200

/* architecture define */
#define GFBG_MAX_LAYER_NUM 5
#define WBC_MAX            1

/* Value define */
#define ZME_HPREC          (1 << 20)
#define ZME_VPREC          (1 << 12)

/* Defined for ZME */
#define MAX_OFFSET         3
#define MIN_OFFSET         (-1)

/* vou device enumeration */
typedef td_u32 vo_intf_type;

typedef enum {
    VO_DEV_DHD0 = 0, /* ultra high definition device */
    VO_DEV_DHD1 = 1, /* high definition device */
    VO_DEV_DSD0 = 2,
    VO_DEV_BUTT
} hal_vo_dev;

typedef enum {
    HAL_DISP_CHANNEL_DHD0 = 0,
    HAL_DISP_CHANNEL_DHD1 = 1,
    HAL_DISP_CHANNEL_DSD0 = 2,
    HAL_DISP_CHANNEL_WBC = 3,
    HAL_DISP_CHANNEL_NONE = 4,
    HAL_DISP_CHANNEL_BUTT
} hal_disp_outputchannel;

typedef enum {
    HAL_DISP_LAYER_VHD0 = 0,
    HAL_DISP_LAYER_VHD1 = 1,
    HAL_DISP_LAYER_VHD2 = 2,
    HAL_DISP_LAYER_VSD0 = 3,

    HAL_DISP_LAYER_GFX0 = 4,
    HAL_DISP_LAYER_GFX1 = 5,
    HAL_DISP_LAYER_GFX2 = 6,
    HAL_DISP_LAYER_GFX3 = 7,
    HAL_DISP_LAYER_GFX4 = 8,

    HAL_DISP_LAYER_WBC = 9,
    HAL_DISP_LAYER_BUTT,
} hal_disp_layer;

#define LAYER_GFX_START    HAL_DISP_LAYER_GFX0  /* GFX0 */
#define LAYER_GFX_END      HAL_DISP_LAYER_GFX4  /* GFX4 */

typedef enum {
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_400 = 0x1,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_420 = 0x2,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_422 = 0x3,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_444 = 0x4,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_411_4X1 = 0x6,
    HAL_INPUTFMT_YCBCR_SEMIPLANAR_422_2X1 = 0x7,

    HAL_INPUTFMT_CBYCRY_PACKAGE_422 = 0x9,
    HAL_INPUTFMT_YCBYCR_PACKAGE_422 = 0xa,
    HAL_INPUTFMT_YCRYCB_PACKAGE_422 = 0xb,
    HAL_INPUTFMT_YCBCR_PACKAGE_444 = 0x1000,

    HAL_INPUTFMT_CLUT_1BPP = 0x00,
    HAL_INPUTFMT_CLUT_2BPP = 0x10,
    HAL_INPUTFMT_CLUT_4BPP = 0x20,
    HAL_INPUTFMT_CLUT_8BPP = 0x30,
    HAL_INPUTFMT_ACLUT_44 = 0x38,

    HAL_INPUTFMT_RGB_444 = 0x40,
    HAL_INPUTFMT_RGB_555 = 0x41,
    HAL_INPUTFMT_RGB_565 = 0x42,
    HAL_INPUTFMT_CBYCRY_PACKAGE_422_GRC = 0x43,
    HAL_INPUTFMT_YCBYCR_PACKAGE_422_GRC = 0x44,
    HAL_INPUTFMT_YCRYCB_PACKAGE_422_GRC = 0x45,
    HAL_INPUTFMT_ACLUT_88 = 0x46,
    HAL_INPUTFMT_ARGB_4444 = 0x48,
    HAL_INPUTFMT_ARGB_1555 = 0x49,

    HAL_INPUTFMT_RGB_888 = 0x50,
    HAL_INPUTFMT_YCBCR_888 = 0x51,
    HAL_INPUTFMT_ARGB_8565 = 0x5a,
    HAL_INPUTFMT_ARGB_6666 = 0x5b,

    HAL_INPUTFMT_KRGB_888 = 0x60,
    HAL_INPUTFMT_ARGB_8888 = 0x68,
    HAL_INPUTFMT_AYCBCR_8888 = 0x69,

    HAL_INPUTFMT_RGBA_4444 = 0xc8,
    HAL_INPUTFMT_RGBA_5551 = 0xc9,

    HAL_INPUTFMT_RGBA_6666 = 0xd8,
    HAL_INPUTFMT_RGBA_5658 = 0xda,

    HAL_INPUTFMT_RGBA_8888 = 0xe8,
    HAL_INPUTFMT_YCBCRA_8888 = 0xe9,

    HAL_DISP_PIXELFORMAT_BUTT
} hal_disp_pixel_format;

typedef enum {
    HAL_CSC_MODE_BT601LIMIT_TO_BT601LIMIT, /* BT601LIMIT to BT601LIMIT */
    HAL_CSC_MODE_BT601FULL_TO_BT601LIMIT,  /* BT601FULL to BT601LIMIT */
    HAL_CSC_MODE_BT709LIMIT_TO_BT601LIMIT, /* BT709LIMIT to BT601LIMIT */
    HAL_CSC_MODE_BT709FULL_TO_BT601LIMIT,  /* BT709FULL to BT601LIMIT */

    HAL_CSC_MODE_BT601LIMIT_TO_BT709LIMIT, /* BT601LIMIT to BT709LIMIT */
    HAL_CSC_MODE_BT601FULL_TO_BT709LIMIT,  /* BT601FULL to BT709LIMIT */
    HAL_CSC_MODE_BT709LIMIT_TO_BT709LIMIT, /* BT709LIMIT to BT709LIMIT */
    HAL_CSC_MODE_BT709FULL_TO_BT709LIMIT,  /* BT709FULL to BT709LIMIT */

    HAL_CSC_MODE_BT601LIMIT_TO_BT601FULL,  /* BT601LIMIT to BT601FULL */
    HAL_CSC_MODE_BT601FULL_TO_BT601FULL,   /* BT601FULL to BT601FULL */
    HAL_CSC_MODE_BT709LIMIT_TO_BT601FULL,  /* BT709LIMIT to BT601FULL */
    HAL_CSC_MODE_BT709FULL_TO_BT601FULL,   /* BT709FULL to BT601FULL */

    HAL_CSC_MODE_BT601LIMIT_TO_BT709FULL,  /* BT601LIMIT to BT709FULL */
    HAL_CSC_MODE_BT709LIMIT_TO_BT709FULL,  /* BT709LIMIT to BT709FULL */
    HAL_CSC_MODE_BT601FULL_TO_BT709FULL,   /* BT601FULL to BT709FULL */
    HAL_CSC_MODE_BT709FULL_TO_BT709FULL,   /* BT709FULL to BT709FULL */

    HAL_CSC_MODE_BT601LIMIT_TO_RGBFULL,     /* BT601LIMIT to RGBFULL */
    HAL_CSC_MODE_BT601FULL_TO_RGBFULL,      /* BT601FULL to RGBFULL */
    HAL_CSC_MODE_BT709LIMIT_TO_RGBFULL,     /* BT709LIMIT to RGBFULL */
    HAL_CSC_MODE_BT709FULL_TO_RGBFULL,      /* BT709FULL to RGBFULL */

    HAL_CSC_MODE_BT601LIMIT_TO_RGBLIMIT,     /* BT601LIMIT to RGBLIMIT */
    HAL_CSC_MODE_BT601FULL_TO_RGBLIMIT,      /* BT601FULL to RGBLIMIT */
    HAL_CSC_MODE_BT709LIMIT_TO_RGBLIMIT,     /* BT709LIMIT to RGBLIMIT */
    HAL_CSC_MODE_BT709FULL_TO_RGBLIMIT,      /* BT709FULL to RGBLIMIT */

    HAL_CSC_MODE_RGBFULL_TO_BT601LIMIT,      /* RGBFULL to BT601LIMIT */
    HAL_CSC_MODE_RGBFULL_TO_BT601FULL,       /* RGBFULL to BT601FULL */
    HAL_CSC_MODE_RGBFULL_TO_BT709LIMIT,      /* RGBFULL to BT709LIMIT */
    HAL_CSC_MODE_RGBFULL_TO_BT709FULL,       /* RGBFULL to BT709FULL */
    HAL_CSC_MODE_RGBFULL_TO_RGBFULL,         /* RGBFULL to RGBFULL */
    HAL_CSC_MODE_RGBFULL_TO_RGBLIMIT,        /* RGBFULLto RGBLIMIT */

    HAL_CSC_MODE_BUTT
} hal_csc_mode;

typedef struct {
    td_s32 luma;
    td_s32 contrast;
    td_s32 hue;
    td_s32 satu;
} hal_csc_value;

typedef struct {
    ot_fb_layer_csc_matrix csc_matrix;
    hal_csc_mode csc_mode;
} hal_csc_matrix_mode;

typedef struct {
    hal_csc_mode csc_mode;
    const csc_coef *coef;
} hal_csc_mode_coef;

/* graphic layer data extend mode */
typedef enum {
    HAL_GFX_BITEXTEND_1ST = 0,
    HAL_GFX_BITEXTEND_2ND = 0x2,
    HAL_GFX_BITEXTEND_3RD = 0x3,

    HAL_GFX_BITEXTEND_BUTT
} hal_gfx_bitextend;

typedef struct {
    td_u32 synm;
    td_u32 iop;
    td_u8 intfb;

    td_u16 vact;
    td_u16 vbb;
    td_u16 vfb;

    td_u16 hact;
    td_u16 hbb;
    td_u16 hfb;
    td_u16 hmid;

    td_u16 bvact;
    td_u16 bvbb;
    td_u16 bvfb;

    td_u16 hpw;
    td_u16 vpw;

    td_u32 idv;
    td_u32 ihs;
    td_u32 ivs;
} hal_disp_syncinfo;

typedef enum {
    HAL_DISP_INTF_CVBS = (0x01L << 0),
    HAL_DISP_INTF_VGA = (0x01L << 1),
    HAL_DISP_INTF_BT656 = (0x01L << 2), /* 2 alg data */
    HAL_DISP_INTF_BT1120 = (0x01L << 3), /* 3 alg data */
    HAL_DISP_INTF_HDMI = (0x01L << 4), /* 4 alg data */
    HAL_DISP_INTF_RGB_6BIT = (0x01L << 5), /* 5 alg data */
    HAL_DISP_INTF_RGB_8BIT = (0x01L << 6), /* 6 alg data */
    HAL_DISP_INTF_RGB_16BIT = (0x01L << 7), /* 7 alg data */
    HAL_DISP_INTF_RGB_18BIT = (0x01L << 8), /* 8 alg data */
    HAL_DISP_INTF_RGB_24BIT = (0x01L << 9), /* 9 alg data */
    HAL_DISP_INTF_MIPI = (0x01L << 10), /* 10 alg data */
    HAL_DISP_INTF_MIPI_SLAVE = (0x01L << 11), /* 11 alg data */
    HAL_DISP_INTF_BUTT = (0x01L << 12), /* 12 alg data */
} hal_disp_intf;

typedef struct {
    td_u16 bkg_a;
    td_u16 bkg_y;
    td_u16 bkg_cb;
    td_u16 bkg_cr;
} hal_disp_bkcolor;

/* graphic layer mask  */
typedef struct {
    td_u8 mask_r;
    td_u8 mask_g;
    td_u8 mask_b;
} hal_gfx_mask;

typedef struct {
    td_u8 key_max_r;
    td_u8 key_max_g;
    td_u8 key_max_b;
} hal_gfx_key_max;

typedef struct {
    td_u8 key_min_r;
    td_u8 key_min_g;
    td_u8 key_min_b;
} hal_gfx_key_min;

typedef enum {
    VDP_RMODE_INTERFACE = 0,
    VDP_RMODE_INTERLACE = 0,
    VDP_RMODE_PROGRESSIVE = 1,
    VDP_RMODE_TOP = 2,
    VDP_RMODE_BOTTOM = 3,
    VDP_RMODE_PRO_TOP = 4,
    VDP_RMODE_PRO_BOTTOM = 5,
    VDP_RMODE_BUTT
} vdp_data_rmode;

typedef enum {
    VDP_CSC_YUV2YUV = 1,
    VDP_CSC_YUV2RGB_601,
    VDP_CSC_YUV2RGB_709,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_RGB2YUV_601,
    VDP_CSC_RGB2YUV_709,
    VDP_CSC_YUV2YUV_MAX,
    VDP_CSC_YUV2YUV_MIN,
    VDP_CSC_YUV2YUV_RAND,
    VDP_CSC_BUTT
} vdp_csc_mode;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* GFBG_DEF_H */

