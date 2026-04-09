/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#ifndef __HIFB_DEF_H__
#define __HIFB_DEF_H__

#include "hi_type.h"
#include "hifb_coef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define VO_VHD_BASE_ADDR  0x1000 /* V0's base addr */
#define VO_VSD_BASE_ADDR  0x2000 /* V1's base addr */
#define VO_GFX_BASE_ADDR  0x7000 /* G0's base addr */
#define VO_WBC0_BASE_ADDR 0xC000 /* WBC_DHD0's base addr */
#define VO_DHD_BASE_ADDR  0xD000 /* DHD0's base addr */
#define VO_DSD_BASE_ADDR  0xE000 /* DHD1's base addr */

#define VHD_REGS_LEN  0x1000 /* len of V0's regs */
#define VSD_REGS_LEN  0x1000
#define GFX_REGS_LEN  0x800
#define GFX2_REGS_LEN 0x1000
#define WBC_REGS_LEN  0x400
#define DHD_REGS_LEN  0x1000
#define DSD_REGS_LEN  0x400
#define INTF_REGS_LEN 0x100
#define VID_REGS_LEN  0x200 /* len of VID regs */
#define GRF_REGS_LEN  0x200 /* len of GFX regs */
#define MRG_REGS_LEN  0xc00 /* len of v0 mrg regs */

#define MRG_OFFSET_ADDR 0x12000

/* Offset define */
#define FDR_VID_OFFSET (0x200 / 4) /* 0x200 bytes, 0x200/4 U32 */
#define FDR_GFX_OFFSET (0x200 / 4)
#define CAP_WBC_OFFSET (0x200 / 4)

/* architecture define */
#define VID_MAX  3
#define GFX_MAX  3
#define WBC_MAX  1
#define PARA_MAX 2

/* for driver WBC resolution */
#define VO_WBC_MIN_WIDTH  32   /* min WBC width */
#define VO_WBC_MIN_HEIGHT 32   /* min WBC height */
#define VO_WBC_MAX_WIDTH  1920 /* max WBC width */
#define VO_WBC_MAX_HEIGHT 1080 /* max WBC height */
/* Value define */
#define ZME_HPREC (1 << 20)
#define ZME_VPREC (1 << 12)

/* Defined for ZME */
#define MAX_OFFSET 3
#define MIN_OFFSET (-1)
/* Defined for vou background color */
#define HIFB_BOUNDDARY_Y_RANGE  2
#define HIFB_BOUNDDARY_CB_RANGE 2
#define HIFB_BOUNDDARY_CR_RANGE 2

/* vou device enumeration */
typedef enum {
    VO_DEV_DHD0 = 0, /* ultra high definition device */
    VO_DEV_DHD1 = 1, /* high definition device */
    VO_DEV_BUTT
} hifb_vo_dev;

typedef enum {
    VO_LAYER_VHD0 = 0, /* V0 layer */
    VO_LAYER_VHD1 = 1, /* V1 layer */
    VO_LAYER_VHD2 = 2, /* V2 layer */

    VO_LAYER_G0 = 3, /* G0 layer */
    VO_LAYER_G1 = 4, /* G1 layer */
    VO_LAYER_G2 = 5, /* G2 layer */
    VO_LAYER_G3 = 6, /* G3 layer */

    VO_LAYER_WBC = 7, /* wbc layer */

    VO_LAYER_BUTT
} vo_layer;

typedef enum {
    VO_SW_LAYER_VHD0 = 0,
    VO_SW_LAYER_VHD1 = 1,
    VO_SW_LAYER_VHD2 = 2,

    VOU_SW_LAYER_BUTT
} vo_sw_layer;

typedef enum {
    HAL_DISP_CHANNEL_DHD0 = 0,
    HAL_DISP_CHANNEL_DHD1 = 1,

    HAL_DISP_CHANNEL_WBC = 2,

    HAL_DISP_CHANNEL_NONE = 3,

    HAL_DISP_CHANNEL_BUTT
} hal_disp_outputchannel;

typedef enum {
    HAL_DISP_LAYER_VHD0 = 0,
    HAL_DISP_LAYER_VHD1 = 1,
    HAL_DISP_LAYER_VHD2 = 2,

    HAL_DISP_LAYER_GFX0 = 3,
    HAL_DISP_LAYER_GFX1 = 4,
    HAL_DISP_LAYER_GFX2 = 5, /* not used */
    HAL_DISP_LAYER_GFX3 = 6, /* for hardware cursor */

    HAL_DISP_LAYER_WBC = 7,

    HAL_DISP_LAYER_TT = 8,
    HAL_DISP_LAYER_BUTT,
    HAL_DISP_INVALID_LAYER = -1
} hal_disp_layer;

typedef enum {
    HAL_LAYER_WBC_HD0 = 0,
    HAL_LAYER_WBC_BUTT
} hal_layer_wbc;

#define LAYER_VHD_START HAL_DISP_LAYER_VHD0 /* VHD0 */
#define LAYER_VHD_END   HAL_DISP_LAYER_VHD0 /* VHD0 */

#define LAYER_MRG_START HAL_DISP_LAYER_VHD0
#define LAYER_MRG_END   HAL_DISP_LAYER_VHD0

#define LAYER_GFX_START HAL_DISP_LAYER_GFX0 /* GFX0 */
#define LAYER_GFX_END   HAL_DISP_LAYER_GFX0 /* GFX0 */
#define LAYER_WBC_START HAL_DISP_LAYER_WBC  /* WBC */
#define LAYER_WBC_END   HAL_DISP_LAYER_WBC  /* WBC */

/* for multi area */
#define MULTI_AREA_CFG_LEN (4 * 8)
#define MULTI_AREA_1RGN    1
#define MULTI_AREA_16RGN   16
#define MULTI_AREA_32RGN   32
#define MULTI_AREA_64RGN   64

typedef enum {
    HAL_DISP_INTERLACE = 0,
    HAL_DISP_PROGRESSIVE = 1,
    HAL_DISP_TOP,
    HAL_DISP_BOTTOM,

    HAL_DISP_DATARMODE_BUTT
} hal_disp_datarmode;

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

/* WBC2 output data format. */
typedef enum {
    HAL_DISP_INTFDATAFMT_SPCBYCRY_422 = 0x0,
    HAL_DISP_INTFDATAFMT_SPYCBYCR_422 = 0x1,
    HAL_DISP_INTFDATAFMT_SPYCRYCB_422 = 0x2,

    HAL_DISP_INTFDATAFMT_ARGB8888 = 0x3,
    HAL_DISP_INTFDATAFMT_YCBCR420 = 0x4,
    HAL_DISP_INTFDATAFMT_YCBCR422 = 0x5,

    HAL_DISP_INTFDATAFMT_BUTT
} hal_disp_intfdatafmt;

typedef enum {
    HAL_CS_UNKNOWN = 0,
    HAL_CS_BT601,
    HAL_CS_BT709,
    HAL_CS_BT2020,
    HAL_CS_RGB,
    HAL_CS_BUTT
} hal_cs;

/*
 * Name : hal_csc_mode
 * Desc : HAL Csc mode,CSC: color space convert.
 */
typedef enum {
    HAL_CSC_MODE_NONE = 0,

    HAL_CSC_MODE_BT601_TO_BT601,
    HAL_CSC_MODE_BT709_TO_BT709,
    HAL_CSC_MODE_RGB_TO_RGB,

    HAL_CSC_MODE_BT601_TO_BT709,
    HAL_CSC_MODE_BT709_TO_BT601,

    HAL_CSC_MODE_BT601_TO_RGB_PC,
    HAL_CSC_MODE_BT709_TO_RGB_PC,
    HAL_CSC_MODE_BT2020_TO_RGB_PC,
    HAL_CSC_MODE_RGB_TO_BT601_PC,
    HAL_CSC_MODE_RGB_TO_BT709_PC,
    HAL_CSC_MODE_RGB_TO_BT2020_PC,

    HAL_CSC_MODE_BT601_TO_RGB_TV,
    HAL_CSC_MODE_BT709_TO_RGB_TV,
    HAL_CSC_MODE_RGB_TO_BT601_TV,
    HAL_CSC_MODE_RGB_TO_BT709_TV,

    HAL_CSC_MODE_BUTT
} hal_csc_mode;

typedef struct {
    hal_csc_mode csc_mode;
    const csc_coef *coef;
} vo_drv_csc_mode_coef;

typedef enum {
    HAL_DISP_BIT_WIDTH_1 = 0x0,
    HAL_DISP_BIT_WIDTH_2 = 0x1,
    HAL_DISP_BIT_WIDTH_3 = 0x3,
    HAL_DISP_BIT_WIDTH_BUTT
} hal_disp_bit_width;

typedef enum {
    HAL_CCD_IMGID_INVALID,
    HAL_CCD_IMGID_1080P60,
    HAL_CCD_IMGID_1080P30_ODD,
    HAL_CCD_IMGID_1080P30_EVEN,
} hal_ccd_imgid;

typedef enum {
    HAL_T_FIRST = 0,
    HAL_B_FIRST = 1,

    HAL_VHD_FOD_BUTT
} hal_vhd_fod;

/* vou coef load mode */
typedef enum {
    HAL_DISP_COEFMODE_HORL = 0,
    HAL_DISP_COEFMODE_HORC,
    HAL_DISP_COEFMODE_VERL,
    HAL_DISP_COEFMODE_VERC,
    HAL_DISP_COEFMODE_LUT,
    HAL_DISP_COEFMODE_GAM,
    HAL_DISP_COEFMODE_ACC,
    HAL_DISP_COEFMODE_ALL
} hal_disp_coefmode;

/* vou zoom mode */
typedef enum {
    HAL_DISP_ZMEMODE_HORL = 0,
    HAL_DISP_ZMEMODE_HORC,
    HAL_DISP_ZMEMODE_VERL,
    HAL_DISP_ZMEMODE_VERC,

    HAL_DISP_ZMEMODE_HOR,
    HAL_DISP_ZMEMODE_VER,
    HAL_DISP_ZMEMODE_ALPHA,
    HAL_DISP_ZMEMODE_ALPHAV,
    HAL_DISP_ZMEMODE_VERT,
    HAL_DISP_ZMEMODE_VERB,

    HAL_DISP_ZMEMODE_ALL,
    HAL_DISP_ZMEMODE_NONL,
    HAL_DISP_ZMEMODE_BUTT
} hal_disp_zmemode;

/* vou zoom format : 420 or 422 or 444 */
typedef enum {
    HAL_DISP_ZME_OUTFMT420 = 0,
    HAL_DISP_ZME_OUTFMT422,
    HAL_DISP_ZME_OUTFMT444,
    HAL_DISP_ZME_OUTFMT_BUTT
} hal_disp_zme_outfmt;

/* vou CBM MIXER */
typedef enum {
    HAL_CBMMIX1 = 0,
    HAL_CBMMIX2 = 1,
    HAL_CBMMIX3 = 2,

    HAL_CBMMIX1_BUTT
} hal_cbmmix;

/* vou graphic layer data extend mode */
typedef enum {
    HAL_GFX_BITEXTEND_1ST = 0,
    HAL_GFX_BITEXTEND_2ND = 0x2,
    HAL_GFX_BITEXTEND_3RD = 0x3,

    HAL_GFX_BITEXTEND_BUTT
} hal_gfx_bitextend;

typedef enum {
    HAL_DISP_SYNC_MODE_TIMING = 0x0,
    HAL_DISP_SYNC_MODE_SIGNAL = 0x1,
    HAL_DISP_SYNC_MODE_BUTT
} hal_disp_sync_mode;

/*
 * name : hal_ifirmode
 * Desc : IFIR,Interpolated Finite Impulse Response filter.
 */
typedef enum {
    HAL_IFIRMODE_DISEN = 0,
    HAL_IFIRMODE_COPY,
    HAL_IFIRMODE_DOUBLE,
    HAL_IFIRMODE_6TAPFIR,

    HAL_IFIRMODE_BUTT
} hal_ifirmode;

/*
 * name : hal_hfirmode
 * Desc : HFIR,Horizontal Finite Impulse Response filter.
 */
typedef enum {
    HAL_HFIRMODE_MEDEN = 0, /* median filtering enable */
    HAL_HFIRMODE_COPY,      /* chroma HFIR copy */
    HAL_HFIRMODE_DOUBLE,    /* bilinear interpolation */
    HAL_HFIRMODE_6TAPFIR,   /* 6 order FIR */

    HAL_HFIRMODE_BUTT
} hal_hfirmode;

/* vou mixer prio id */
typedef enum {
    HAL_DISP_MIX_PRIO0 = 0,
    HAL_DISP_MIX_PRIO1,
    HAL_DISP_MIX_PRIO2,
    HAL_DISP_MIX_PRIO3,
    HAL_DISP_MIX_BUTT
} hal_disp_mix_prio;

/* vou layer select ddr id */
typedef enum {
    HAL_LAYER_DDR0 = 0,
    HAL_LAYER_DDR_BUTT
} hal_layer_ddr_sel;

/* LTI/CTI mode */
typedef enum {
    HAL_DISP_TIMODE_LUM = 0,
    HAL_DISP_TIMODE_CHM,

    HAL_DISP_TIMODE_ALL,
    HAL_DISP_TIMODE_NON,
    HAL_DISP_TIMODE_BUTT,
} hal_disp_timode;

typedef struct {
    hi_u32 synm;
    hi_u32 iop;
    hi_u8 intfb;

    hi_u16 vact;
    hi_u16 vbb;
    hi_u16 vfb;

    hi_u16 hact;
    hi_u16 hbb;
    hi_u16 hfb;
    hi_u16 hmid;

    hi_u16 bvact;
    hi_u16 bvbb;
    hi_u16 bvfb;

    hi_u16 hpw;
    hi_u16 vpw;

    hi_u32 idv;
    hi_u32 ihs;
    hi_u32 ivs;
} hal_disp_syncinfo;

typedef enum {
    HAL_DISP_INTF_CVBS = (0x01L << 0),
    HAL_DISP_INTF_HDDATE = (0x01L << 1),
    HAL_DISP_INTF_VGA = (0x01L << 2),         /* 2 bit vga */
    HAL_DISP_INTF_BT656 = (0x01L << 3),       /* 3 bit bt565 */
    HAL_DISP_INTF_BT1120 = (0x01L << 4),      /* 4 bit bt1120 */
    HAL_DISP_INTF_HDMI = (0x01L << 5),        /* 5 bit hdmi */
    HAL_DISP_INTF_LCD = (0x01L << 6),         /* 6 bit lcd */
    HAL_DISP_INTF_DATE = (0x01L << 7),        /* 7 bit data */
    HAL_DISP_INTF_LCD_6BIT = (0x01L << 9),    /* 9 bit 6bit */
    HAL_DISP_INTF_LCD_8BIT = (0x01L << 10),   /* 10 bit 8bit */
    HAL_DISP_INTF_LCD_16BIT = (0x01L << 11),  /* 11 bit 16bit */
    HAL_DISP_INTF_LCD_18BIT = (0x01L << 12),  /* 12 bit 18bit */
    HAL_DISP_INTF_LCD_24BIT = (0x01L << 13),  /* 13 bit 24bit */
    HAL_DISP_INTF_MIPI = (0x01L << 14),       /* 14 bit mipi */
    HAL_DISP_INTF_MIPI_SLAVE = (0x01L << 15), /* 15 bit mipi slave */
    HAL_DISP_INTF_BUTT = (0x01L << 16),       /* 16 bit max */
} hal_disp_intf;

typedef struct {
    hi_u32 f_inv;
    hi_u32 vs_inv;
    hi_u32 hs_inv;
    hi_u32 dv_inv;
} hal_disp_syncinv;

typedef enum {
    HAL_MULTICHN_EN_1P1C = 0x1, /* 1 pixel / 1 cycle */
    HAL_MULTICHN_EN_2P1C = 0x2, /* 2 pixel / 1 cycle */
} hal_multichn_en;

/* vou background color */
typedef struct {
    hi_u16 boundary_y[HIFB_BOUNDDARY_Y_RANGE];
    hi_u16 boundary_cb[HIFB_BOUNDDARY_CB_RANGE];
    hi_u16 boundary_cr[HIFB_BOUNDDARY_CR_RANGE];
} hal_disp_boundary_color;

typedef struct {
    hi_u16 bkg_a;
    hi_u16 bkg_y;
    hi_u16 bkg_cb;
    hi_u16 bkg_cr;
} hal_disp_bkcolor;

typedef struct {
    hi_u16 bkg_y;
    hi_u16 bkg_cb;
    hi_u16 bkg_cr;
} hal_video_layer_bkcolor;

typedef struct {
    hi_u16 clip_low_y;
    hi_u16 clip_low_cb;
    hi_u16 clip_low_cr;

    hi_u16 clip_high_y;
    hi_u16 clip_high_cb;
    hi_u16 clip_high_cr;
} hal_disp_clip;

typedef struct {
    hi_u32 acc_multi;
    hi_u32 thd_med_high;
    hi_u32 thd_med_low;
    hi_u32 thd_high;
    hi_u32 thd_low;
} accthd;

typedef struct {
    hi_s32 sx; /* source horizontal start position */
    hi_s32 sy; /* source vertical start position */

    hi_s32 dxs; /* dispaly horizontal start position */
    hi_s32 dys; /* display vertical start position */

    hi_s32 dxl; /* dispaly horizontal end position */
    hi_s32 dyl; /* display vertical end position */

    hi_s32 vx; /* video horizontal start position */
    hi_s32 vy; /* video vertical start position */

    hi_u32 in_width;   /* input width */
    hi_u32 in_height;  /* input height */
    hi_u32 out_width;  /* output width */
    hi_u32 out_height; /* output height */
} hal_disp_rect;

typedef struct {
    hi_u32 dither_sed_y0;
    hi_u32 dither_sed_u0;
    hi_u32 dither_sed_v0;
    hi_u32 dither_sed_w0;

    hi_u32 dither_sed_y1;
    hi_u32 dither_sed_u1;
    hi_u32 dither_sed_v1;
    hi_u32 dither_sed_w1;

    hi_u32 dither_sed_y2;
    hi_u32 dither_sed_u2;
    hi_u32 dither_sed_v2;
    hi_u32 dither_sed_w2;

    hi_u32 dither_sed_y3;
    hi_u32 dither_sed_u3;
    hi_u32 dither_sed_v3;
    hi_u32 dither_sed_w3;
} hal_disp_dihter_sed;

/* vou graphic layer mask */
typedef struct {
    hi_u8 mask_r;
    hi_u8 mask_g;
    hi_u8 mask_b;
} hal_gfx_mask;

typedef struct {
    hi_u8 key_max_r;
    hi_u8 key_max_g;
    hi_u8 key_max_b;
} hal_gfx_key_max;

typedef struct {
    hi_u8 key_min_r;
    hi_u8 key_min_g;
    hi_u8 key_min_b;
} hal_gfx_key_min;

typedef struct {
    hi_u32 sx;
    hi_u32 x;
    hi_u32 y;

    hi_u32 wth;
    hi_u32 hgt;
} vdp_rect;

typedef enum {
    VDP_REQ_LENGTH_16 = 0,
    VDP_REQ_LENGTH_8 = 1,
    VDP_REQ_LENGTH_4 = 2,
    VDP_REQ_LENGTH_BUTT
} vdp_req_length;

typedef enum {
    VDP_DATA_WTH_8 = 0,
    VDP_DATA_WTH_10 = 1,
    VDP_DATA_WTH_BUTT
} vdp_data_wth;

typedef enum {
    VDP_LAYER_VID0 = 0,
    VDP_LAYER_VID1 = 1,
    VDP_LAYER_VID2 = 2,
    VDP_LAYER_VID3 = 3,
    VDP_LAYER_VID4 = 4,
    VDP_LAYER_VID5 = 5,
    VDP_LAYER_VID6 = 6, /* for FI pixel */
    VDP_LAYER_VID7 = 7, /* for FI info */

    VDP_LAYER_VID_BUTT
} vdp_layer_vid;

typedef enum {
    VDP_VID_IFMT_SP_400 = 0x1,
    VDP_VID_IFMT_SP_420 = 0x3,
    VDP_VID_IFMT_SP_422 = 0x4,
    VDP_VID_IFMT_SP_444 = 0x5,
    VDP_VID_IFMT_SP_TILE = 0x6,
    VDP_VID_IFMT_SP_TILE_64 = 0x7,
    VDP_VID_IFMT_PKG_UYVY = 0x9,
    VDP_VID_IFMT_PKG_YUYV = 0xa,
    VDP_VID_IFMT_PKG_YVYU = 0xb,
    VDP_VID_IFMT_PKG_12 = 0xc,
    VDP_VID_IFMT_RGB_888 = 0x8,
    VDP_VID_IFMT_PKG_YUV444 = 0xe,
    VDP_VID_IFMT_ARGB_8888 = 0xf,
    VDP_VID_IFMT_BUTT
} vdp_vid_ifmt;

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

/* graphics layer */
typedef enum {
    VDP_GFX_IFMT_CLUT_1BPP = 0x00,
    VDP_GFX_IFMT_CLUT_2BPP = 0x10,
    VDP_GFX_IFMT_CLUT_4BPP = 0x20,
    VDP_GFX_IFMT_CLUT_8BPP = 0x30,

    VDP_GFX_IFMT_ACLUT_44 = 0x38,

    VDP_GFX_IFMT_RGB_444 = 0x40,
    VDP_GFX_IFMT_RGB_555 = 0x41,
    VDP_GFX_IFMT_RGB_565 = 0x42,

    VDP_GFX_IFMT_PKG_UYVY = 0x43,
    VDP_GFX_IFMT_PKG_YUYV = 0x44,
    VDP_GFX_IFMT_PKG_YVYU = 0x45,

    VDP_GFX_IFMT_ACLUT_88 = 0x46,
    VDP_GFX_IFMT_ARGB_4444 = 0x48,
    VDP_GFX_IFMT_ARGB_1555 = 0x49,

    VDP_GFX_IFMT_RGB_888 = 0x50,   /* 24bpp */
    VDP_GFX_IFMT_YCBCR_888 = 0x51, /* 24bpp */
    VDP_GFX_IFMT_ARGB_8565 = 0x5a, /* 24bpp */

    VDP_GFX_IFMT_KRGB_888 = 0x60,
    VDP_GFX_IFMT_ARGB_8888 = 0x68,
    VDP_GFX_IFMT_AYCBCR_8888 = 0x69,

    VDP_GFX_IFMT_RGBA_4444 = 0xc8,
    VDP_GFX_IFMT_RGBA_5551 = 0xc9,
    VDP_GFX_IFMT_RGBA_5658 = 0xda, /* 24bpp */
    VDP_GFX_IFMT_RGBA_8888 = 0xe8,
    VDP_GFX_IFMT_YCBCRA_8888 = 0xe9,
    VDP_GFX_IFMT_ABGR_8888 = 0xef,

    VDP_GFX_IFMT_STILL_888 = 0x100, /* only use by environment */

    VDP_GFX_IFMT_BUTT
} vdp_gfx_ifmt;

typedef enum {
    VDP_PROC_FMT_SP_420 = 0x0,
    VDP_PROC_FMT_SP_422 = 0x1,
    VDP_PROC_FMT_SP_444 = 0x2,  /* plannar,in YUV color domain */
    VDP_PROC_FMT_RGB_888 = 0x3, /* package,in RGB color domain */
    VDP_PROC_FMT_RGB_444 = 0x4, /* plannar,in RGB color domain */

    VDP_PROC_FMT_BUTT
} vdp_proc_fmt;

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

typedef struct {
    hi_u32 bkg_y;
    hi_u32 bkg_u;
    hi_u32 bkg_v;

    hi_u32 bkg_a;

    hi_u32 bk_type;
} vdp_bkg;

typedef enum {
    ALIGN_MODE_32BURST = 0x0,
    ALIGN_MODE_16BURST = 0x1,

    ALIGN_MODE_BUTT
} vo_wbc_align_mode;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __HIFB_DEF_H__ */
