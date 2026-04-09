/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef __HIFB_DEF_H__
#define __HIFB_DEF_H__

#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define VO_VHD_BASE_ADDR   0x1000            /* V0's base addr */
#define VO_VSD_BASE_ADDR   0x2000            /* V1's base addr */
#define VO_GFX_BASE_ADDR   0x7000            /* G0's base addr */
#define VO_WBC0_BASE_ADDR  0xC000            /* WBC_DHD0's base addr*/
#define VO_DHD_BASE_ADDR   0xD000            /* DHD0's base addr*/
#define VO_DSD_BASE_ADDR   0xE000            /* DHD1's base addr*/

#define VHD_REGS_LEN       0x1000            /* len of V0's regs*/
#define VSD_REGS_LEN       0x1000
#define GFX_REGS_LEN       0x800
#define GFX2_REGS_LEN      0x1000
#define WBC_REGS_LEN       0x400
#define DHD_REGS_LEN       0x1000
#define DSD_REGS_LEN       0x400
#define INTF_REGS_LEN      0x100
#define VID_REGS_LEN       0x200             /* len of VID regs */
#define GRF_REGS_LEN       0x200             /* len of GFX regs */
#define MRG_REGS_LEN       0xc00             /* len of v0 mrg regs */

#define MRG_OFFSET_ADDR    0x12000

// Offset define
#define FDR_VID_OFFSET     (0x200/4)         /* 0x200 bytes, 0x200/4 U32*/
#define FDR_GFX_OFFSET     (0x200/4)
#define CAP_WBC_OFFSET     (0x200/4)

// architecture define
#define VID_MAX            3
#define GFX_MAX            3
#define WBC_MAX            1
#define PARA_MAX           2

//for driver WBC resolution
#define VO_WBC_MIN_WIDTH        32            /* min WBC width */
#define VO_WBC_MIN_HEIGHT       32            /* min WBC height */
#define VO_WBC_MAX_WIDTH        1920          /* max WBC width */
#define VO_WBC_MAX_HEIGHT       1080          /* max WBC height */
/*1920*/      /* max WBC height for MIPI_Tx */
//--------------------
// Value define
//--------------------
#define  ZME_HPREC    (1<<20)
#define  ZME_VPREC    (1<<12)

//--------------------
// Defined for ZME
//--------------------
#define  MAX_OFFSET   3
#define  MIN_OFFSET   -1

/* vou device enumeration */
typedef enum hiVO_DEV_E
{
    VO_DEV_DHD0  = 0,                 /* ultra high definition device */
    VO_DEV_DHD1  = 1,                 /* high definition device */
    VO_DEV_BUTT
} VO_DEV_E;

typedef enum
{
    VO_LAYER_VHD0   = 0,    /*V0 layer*/
    VO_LAYER_VHD1   = 1,    /*V1 layer*/
    VO_LAYER_VHD2   = 2,    /*V2 layer*/

    VO_LAYER_G0     = 3,    /*G0 layer*/
    VO_LAYER_G1     = 4,    /*G1 layer*/
    VO_LAYER_G2     = 5,    /*G2 layer*/ // not used
    VO_LAYER_G3     = 6,    /*G3 layer*/

    VO_LAYER_WBC    = 7,    /*wbc layer*/

    VO_LAYER_BUTT
}VO_LAYER_E;

typedef enum
{
    VO_SW_LAYER_VHD0  = 0,
    VO_SW_LAYER_VHD1  = 1,
    VO_SW_LAYER_VHD2  = 2,

    VOU_SW_LAYER_BUTT
}VO_SW_LAYER_E;

typedef enum tagHAL_DISP_OUTPUTCHANNEL_E
{
    HAL_DISP_CHANNEL_DHD0 = 0,
    HAL_DISP_CHANNEL_DHD1 = 1,

    HAL_DISP_CHANNEL_WBC  = 2,

    HAL_DISP_CHANNEL_NONE = 3,

    HAL_DISP_CHANNEL_BUTT
} HAL_DISP_OUTPUTCHANNEL_E;


typedef enum tagHAL_DISP_LAYER_E
{
    HAL_DISP_LAYER_VHD0   = 0 ,
    HAL_DISP_LAYER_VHD1   = 1 ,
    HAL_DISP_LAYER_VHD2   = 2 ,

    HAL_DISP_LAYER_GFX0   = 3 ,
    HAL_DISP_LAYER_GFX1   = 4 ,
    HAL_DISP_LAYER_GFX2   = 5 ,   // not used
    HAL_DISP_LAYER_GFX3   = 6 ,   // for hardware cursor

    HAL_DISP_LAYER_WBC    = 7 ,

    HAL_DISP_LAYER_TT     = 8 ,
    HAL_DISP_LAYER_BUTT       ,
    HAL_DISP_INVALID_LAYER = -1
} HAL_DISP_LAYER_E;


typedef enum tagHAL_LAYER_WBC_E
{
    HAL_LAYER_WBC_HD0  = 0,
    HAL_LAYER_WBC_BUTT

} HAL_LAYER_WBC_E;


//#define HAL_DISP_LAYER_GFX0  HAL_DISP_LAYER_HC0


#define LAYER_VHD_START  HAL_DISP_LAYER_VHD0  //VHD0
#define LAYER_VHD_END    HAL_DISP_LAYER_VHD0  //VHD0

#define LAYER_MRG_START  HAL_DISP_LAYER_VHD0
#define LAYER_MRG_END    HAL_DISP_LAYER_VHD0

//#define LAYER_VSD_START  HAL_DISP_LAYER_VSD0  //VSD0
//#define LAYER_VSD_END    HAL_DISP_LAYER_VSD0  //VSD0

#define LAYER_GFX_START  HAL_DISP_LAYER_GFX0  //GFX0
#define LAYER_GFX_END    HAL_DISP_LAYER_GFX0  //GFX0
#define LAYER_WBC_START  HAL_DISP_LAYER_WBC //WBC
#define LAYER_WBC_END    HAL_DISP_LAYER_WBC //WBC

//for multi area
#define MULTI_AREA_CFG_LEN 4*8
#define MULTI_AREA_1RGN    1
#define MULTI_AREA_16RGN   16
#define MULTI_AREA_32RGN   32
#define MULTI_AREA_64RGN   64

typedef enum tagHAL_DISP_DATARMODE_E
{
    HAL_DISP_INTERLACE   = 0,
    HAL_DISP_PROGRESSIVE = 1,
    HAL_DISP_TOP,
    HAL_DISP_BOTTOM,

    HAL_DISP_DATARMODE_BUTT
} HAL_DISP_DATARMODE_E;

typedef enum tagHAL_DISP_PIXEL_FORMAT_E
{
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_400     = 0x1,
    //HAL_INPUTFMT_YCbCr_SEMIPLANAR_411_1X4 = 0x2,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_420     = 0x2,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_422     = 0x3,
    //HAL_INPUTFMT_YCbCr_SEMIPLANAR_422_1X2 = 0x4,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_444     = 0x4,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_411_4X1 = 0x6,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_422_2X1 = 0x7,

    HAL_INPUTFMT_CbYCrY_PACKAGE_422       = 0x9,
    HAL_INPUTFMT_YCbYCr_PACKAGE_422       = 0xa,
    HAL_INPUTFMT_YCrYCb_PACKAGE_422       = 0xb,
    HAL_INPUTFMT_YCbCr_PACKAGE_444        = 0x1000,

    HAL_INPUTFMT_CLUT_1BPP                = 0x00,
    HAL_INPUTFMT_CLUT_2BPP                = 0x10,
    HAL_INPUTFMT_CLUT_4BPP                = 0x20,
    HAL_INPUTFMT_CLUT_8BPP                = 0x30,
    HAL_INPUTFMT_ACLUT_44                 = 0x38,

    HAL_INPUTFMT_RGB_444                  = 0x40,
    HAL_INPUTFMT_RGB_555                  = 0x41,
    HAL_INPUTFMT_RGB_565                  = 0x42,
    HAL_INPUTFMT_CbYCrY_PACKAGE_422_GRC   = 0x43,
    HAL_INPUTFMT_YCbYCr_PACKAGE_422_GRC   = 0x44,
    HAL_INPUTFMT_YCrYCb_PACKAGE_422_GRC   = 0x45,
    HAL_INPUTFMT_ACLUT_88                 = 0x46,
    HAL_INPUTFMT_ARGB_4444                = 0x48,
    HAL_INPUTFMT_ARGB_1555                = 0x49,

    HAL_INPUTFMT_RGB_888                  = 0x50,
    HAL_INPUTFMT_YCbCr_888                = 0x51,
    HAL_INPUTFMT_ARGB_8565                = 0x5a,
    HAL_INPUTFMT_ARGB_6666                = 0x5b,

    HAL_INPUTFMT_KRGB_888                 = 0x60,
    HAL_INPUTFMT_ARGB_8888                = 0x68,
    HAL_INPUTFMT_AYCbCr_8888              = 0x69,

    HAL_INPUTFMT_RGBA_4444                = 0xc8,
    HAL_INPUTFMT_RGBA_5551                = 0xc9,

    HAL_INPUTFMT_RGBA_6666                = 0xd8,
    HAL_INPUTFMT_RGBA_5658                = 0xda,

    HAL_INPUTFMT_RGBA_8888                = 0xe8,
    HAL_INPUTFMT_YCbCrA_8888              = 0xe9,

    HAL_DISP_PIXELFORMAT_BUTT

}HAL_DISP_PIXEL_FORMAT_E;

typedef enum tagHAL_DISP_INTFDATAFMT_E
{
    // WBC2 output data format.

    HAL_DISP_INTFDATAFMT_SPCBYCRY_422   = 0x0,
    HAL_DISP_INTFDATAFMT_SPYCBYCR_422   = 0x1,
    HAL_DISP_INTFDATAFMT_SPYCRYCB_422   = 0x2,

    HAL_DISP_INTFDATAFMT_ARGB8888       = 0x3,
    HAL_DISP_INTFDATAFMT_YCBCR420       = 0x4,
    HAL_DISP_INTFDATAFMT_YCBCR422       = 0x5,

    HAL_DISP_INTFDATAFMT_BUTT
} HAL_DISP_INTFDATAFMT_E;

typedef enum hiHAL_CS_E
{
    HAL_CS_UNKNOWN = 0,
    HAL_CS_BT601,
    HAL_CS_BT709,
    HAL_CS_BT2020,
    HAL_CS_RGB,
    HAL_CS_BUTT
} HAL_CS_E;


/*
* Name : HAL_CSC_MODE_E
* Desc : HAL Csc mode,CSC: color space convert.
*/
typedef enum hiHAL_CSC_MODE_E
{
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
} HAL_CSC_MODE_E;

typedef enum hiHAL_DISP_BIT_WIDTH_E
{
    HAL_DISP_BIT_WIDTH_1 = 0x0,
    HAL_DISP_BIT_WIDTH_2 = 0x1,
    HAL_DISP_BIT_WIDTH_3 = 0x3,
    HAL_DISP_BIT_WIDTH_BUTT
} HAL_DISP_BIT_WIDTH_E;

typedef enum tagHAL_CCD_IMGID_E
{
    HAL_CCD_IMGID_INVALID,
    HAL_CCD_IMGID_1080P60,
    HAL_CCD_IMGID_1080P30_ODD,
    HAL_CCD_IMGID_1080P30_EVEN,

}HAL_CCD_IMGID_E;

typedef enum tagHAL_VHD_FOD_E
{
    HAL_T_FIRST = 0,
    HAL_B_FIRST = 1,

    HAL_VHD_FOD_BUTT
} HAL_VHD_FOD_E;

/* vou coef load mode */
typedef enum
{
    HAL_DISP_COEFMODE_HORL  = 0,
    HAL_DISP_COEFMODE_HORC,
    HAL_DISP_COEFMODE_VERL,
    HAL_DISP_COEFMODE_VERC,
    HAL_DISP_COEFMODE_LUT,
    HAL_DISP_COEFMODE_GAM,
    HAL_DISP_COEFMODE_ACC,
    HAL_DISP_COEFMODE_ALL
}HAL_DISP_COEFMODE_E;


/* vou zoom mode */
typedef enum
{
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

}HAL_DISP_ZMEMODE_E;

/*
* vou zoom format : 420 or 422 or 444
*/
typedef enum
{
    HAL_DISP_ZME_OUTFMT420 = 0,
    HAL_DISP_ZME_OUTFMT422 ,
    HAL_DISP_ZME_OUTFMT444 ,
    HAL_DISP_ZME_OUTFMT_BUTT

}HAL_DISP_ZME_OUTFMT_E;


/* vou CBM MIXER */
typedef enum
{
    HAL_CBMMIX1 = 0,
    HAL_CBMMIX2 = 1,
    HAL_CBMMIX3 = 2,

    HAL_CBMMIX1_BUTT
}HAL_CBMMIX_E;

/* vou graphic layer data extend mode */
typedef enum
{
    HAL_GFX_BITEXTEND_1ST =   0,
    HAL_GFX_BITEXTEND_2ND = 0x2,
    HAL_GFX_BITEXTEND_3RD = 0x3,

    HAL_GFX_BITEXTEND_BUTT
}HAL_GFX_BITEXTEND_E;

typedef enum hiHAL_DISP_SYNC_MODE_E
{
    HAL_DISP_SYNC_MODE_TIMING = 0x0,
    HAL_DISP_SYNC_MODE_SIGNAL = 0x1,
    HAL_DISP_SYNC_MODE_BUTT
} HAL_DISP_SYNC_MODE_E;

/* RM frame or field infomation */
/*
* name : HAL_IFIRMODE_E
* Desc : IFIR,Interpolated Finite Impulse Response filter.
*/
typedef enum tagHAL_IFIRMODE_E
{
    HAL_IFIRMODE_DISEN = 0,
    HAL_IFIRMODE_COPY,
    HAL_IFIRMODE_DOUBLE,
    HAL_IFIRMODE_6TAPFIR,

    HAL_IFIRMODE_BUTT
}HAL_IFIRMODE_E;

/*
* name : HAL_HFIRMODE_E
* Desc : HFIR,Horizontal Finite Impulse Response filter.
*/
typedef enum tagHAL_HFIRMODE_E
{
    HAL_HFIRMODE_MEDEN = 0,    // median filtering enable
    HAL_HFIRMODE_COPY,         // chroma HFIR copy
    HAL_HFIRMODE_DOUBLE,       // bilinear interpolation
    HAL_HFIRMODE_6TAPFIR,      // 6 order FIR

    HAL_HFIRMODE_BUTT
}HAL_HFIRMODE_E;


/* vou mixer prio id */
typedef enum tagHAL_DISP_MIX_PRIO_E
{
    HAL_DISP_MIX_PRIO0 = 0,
    HAL_DISP_MIX_PRIO1,
    HAL_DISP_MIX_PRIO2,
    HAL_DISP_MIX_PRIO3,
    //HAL_DISP_MIX_PRIO4,
    //HAL_DISP_MIX_PRIO5,
    //HAL_DISP_MIX_PRIO6,
    HAL_DISP_MIX_BUTT
}HAL_DISP_MIX_PRIO_E;


/* vou layer select ddr id */
typedef enum tagHAL_LAYER_DDR_SEL_E
{
    HAL_LAYER_DDR0 = 0,
    //HAL_LAYER_DDR1 = 1,
    HAL_LAYER_DDR_BUTT
}HAL_LAYER_DDR_SEL_E;

/* LTI/CTI mode */
typedef enum tagHAL_DISP_TIMODE_E
{
    HAL_DISP_TIMODE_LUM = 0,
    HAL_DISP_TIMODE_CHM,

    HAL_DISP_TIMODE_ALL,
    HAL_DISP_TIMODE_NON,
    HAL_DISP_TIMODE_BUTT,
} HAL_DISP_TIMODE_E;

typedef struct tagHAL_DISP_SYNCINFO_S
{
    HI_U32    bSynm;
    HI_U32    bIop;
    HI_U8     u8Intfb;

    HI_U16    u16Vact ;
    HI_U16    u16Vbb;
    HI_U16    u16Vfb;

    HI_U16    u16Hact;
    HI_U16    u16Hbb;
    HI_U16    u16Hfb;
    HI_U16    u16Hmid;

    HI_U16    u16Bvact;
    HI_U16    u16Bvbb;
    HI_U16    u16Bvfb;

    HI_U16    u16Hpw;
    HI_U16    u16Vpw;

    HI_U32    bIdv;
    HI_U32    bIhs;
    HI_U32    bIvs;
} HAL_DISP_SYNCINFO_S;

typedef enum tagHAL_DISP_INTF_E
{

    HAL_DISP_INTF_CVBS      = (0x01L<< 0),
    HAL_DISP_INTF_HDDATE    = (0x01L<< 1),
    HAL_DISP_INTF_VGA       = (0x01L<< 2),
    HAL_DISP_INTF_BT656     = (0x01L<< 3),
    HAL_DISP_INTF_BT1120    = (0x01L<< 4),
    HAL_DISP_INTF_HDMI      = (0x01L<< 5),
    HAL_DISP_INTF_LCD       = (0x01L<< 6),
    HAL_DISP_INTF_DATE      = (0x01L<< 7),
    HAL_DISP_INTF_LCD_6BIT  = (0x01L<< 9),
    HAL_DISP_INTF_LCD_8BIT  = (0x01L<<10),
    HAL_DISP_INTF_LCD_16BIT = (0x01L<<11),
    HAL_DISP_INTF_LCD_18BIT = (0x01L<<12),
    HAL_DISP_INTF_LCD_24BIT = (0x01L<<13),
    HAL_DISP_INTF_MIPI      = (0x01L<<14),
    HAL_DISP_INTF_MIPI_SLAVE      = (0x01L<<15),
    HAL_DISP_INTF_BUTT      = (0x01L<<16),

} HAL_DISP_INTF_E;

typedef struct tagHAL_DISP_SYNCINV_S
{
    HI_U32  u32f_inv;
    HI_U32  u32vs_inv;
    HI_U32  u32hs_inv;
    HI_U32  u32dv_inv;

} HAL_DISP_SYNCINV_S;

typedef enum tagHAL_MULTICHN_EN
{
    HAL_MULTICHN_EN_1P1C = 0x1,   // 1 pixel / 1 cycle
    HAL_MULTICHN_EN_2P1C = 0x2,  // 2 pixel / 1 cycle
}HAL_MULTICHN_EN;


/* vou background color */

typedef struct tagHAL_DISP_BOUNDARY_COLOR_S
{
    HI_U16 u16Boundary_y[2];
    HI_U16 u16Boundary_cb[2];
    HI_U16 u16Boundary_cr[2];
} HAL_DISP_BOUNDARY_COLOR_S;


typedef struct tagHAL_DISP_BKCOLOR_S
{
    HI_U16 u16Bkg_a;
    HI_U16 u16Bkg_y;
    HI_U16 u16Bkg_cb;
    HI_U16 u16Bkg_cr;
} HAL_DISP_BKCOLOR_S;

typedef struct tagHAL_VIDEO_LAYER_BKCOLOR_S
{
    HI_U16 u16Bkg_y;
    HI_U16 u16Bkg_cb;
    HI_U16 u16Bkg_cr;
} HAL_VIDEO_LAYER_BKCOLOR_S;


typedef struct tagHAL_DISP_CLIP_S
{
    HI_U16 u16ClipLow_y;
    HI_U16 u16ClipLow_cb;
    HI_U16 u16ClipLow_cr;

    HI_U16 u16ClipHigh_y;
    HI_U16 u16ClipHigh_cb;
    HI_U16 u16ClipHigh_cr;

} HAL_DISP_CLIP_S;

typedef struct
{
    HI_U32 acc_multi    ;
    HI_U32 thd_med_high ;
    HI_U32 thd_med_low  ;
    HI_U32 thd_high     ;
    HI_U32 thd_low      ;
} ACCTHD_S;

typedef struct tagHAL_DISP_RECT_S
{
    HI_S32 s32SX;       /*source horizontal start position*/
    HI_S32 s32SY;       /*source vertical start position*/

    HI_S32 s32DXS;       /*dispaly horizontal start position*/
    HI_S32 s32DYS;       /*display vertical start position*/

    HI_S32 s32DXL;       /*dispaly horizontal end position*/
    HI_S32 s32DYL;       /*display vertical end position*/

    HI_S32 s32VX;       /*video horizontal start position*/
    HI_S32 s32VY;       /*video vertical start position*/

    HI_U32 u32InWidth;   /*input width*/
    HI_U32 u32InHeight;  /*input height*/
    HI_U32 u32OutWidth;   /*output width*/
    HI_U32 u32OutHeight;  /*output height*/
} HAL_DISP_RECT_S;

typedef struct tagHAL_DISP_DIHTER_SED_S
{
    HI_U32 u32DitherSedY0;
    HI_U32 u32DitherSedU0;
    HI_U32 u32DitherSedV0;
    HI_U32 u32DitherSedW0;

    HI_U32 u32DitherSedY1;
    HI_U32 u32DitherSedU1;
    HI_U32 u32DitherSedV1;
    HI_U32 u32DitherSedW1;

    HI_U32 u32DitherSedY2;
    HI_U32 u32DitherSedU2;
    HI_U32 u32DitherSedV2;
    HI_U32 u32DitherSedW2;

    HI_U32 u32DitherSedY3;
    HI_U32 u32DitherSedU3;
    HI_U32 u32DitherSedV3;
    HI_U32 u32DitherSedW3;
}HAL_DISP_DIHTER_SED_S;

/* vou graphic layer mask  */
typedef struct tagHAL_GFX_MASK_S
{
    HI_U8 u8Mask_r;
    HI_U8 u8Mask_g;
    HI_U8 u8Mask_b;

} HAL_GFX_MASK_S;

typedef struct tagHAL_GFX_KEY_MAX_S
{
    HI_U8 u8KeyMax_R;
    HI_U8 u8KeyMax_G;
    HI_U8 u8KeyMax_B;

} HAL_GFX_KEY_MAX_S;

typedef struct tagHAL_GFX_KEY_MIN_S
{
    HI_U8 u8KeyMin_R;
    HI_U8 u8KeyMin_G;
    HI_U8 u8KeyMin_B;

} HAL_GFX_KEY_MIN_S;

/*******************************************************************
* Begin  :  For CMP(Compress) and DCMP(Decompress).
*******************************************************************/
typedef struct
{
    HI_U32 u32SX;
    HI_U32 u32X;
    HI_U32 u32Y;

    HI_U32 u32Wth;
    HI_U32 u32Hgt;

} VDP_RECT_S;

typedef enum tagVDP_REQ_LENGHT
{
    VDP_REQ_LENGTH_16 = 0,
    VDP_REQ_LENGTH_8  = 1,
    VDP_REQ_LENGTH_4  = 2,
    VDP_REQ_LENGTH_BUTT
}VDP_REQ_LENGTH_E;

typedef enum tagVDP_DATA_WTH
{
    VDP_DATA_WTH_8  = 0,
    VDP_DATA_WTH_10 = 1,
    VDP_DATA_WTH_BUTT
}VDP_DATA_WTH;

typedef enum tagVDP_LAYER_VID_E
{
    VDP_LAYER_VID0  = 0,
    VDP_LAYER_VID1  = 1,
    VDP_LAYER_VID2  = 2,
    VDP_LAYER_VID3  = 3,
    VDP_LAYER_VID4  = 4,
    VDP_LAYER_VID5  = 5,
    VDP_LAYER_VID6  = 6,//for FI pixel
    VDP_LAYER_VID7  = 7,//for FI info

    VDP_LAYER_VID_BUTT

} VDP_LAYER_VID_E;

typedef enum tagVDP_VID_IFMT_E
{
    VDP_VID_IFMT_SP_400      = 0x1,
    VDP_VID_IFMT_SP_420      = 0x3,
    VDP_VID_IFMT_SP_422      = 0x4,
    VDP_VID_IFMT_SP_444      = 0x5,
    VDP_VID_IFMT_SP_TILE     = 0x6,
    VDP_VID_IFMT_SP_TILE_64  = 0x7,
    VDP_VID_IFMT_PKG_UYVY    = 0x9,
    VDP_VID_IFMT_PKG_YUYV    = 0xa,
    VDP_VID_IFMT_PKG_YVYU    = 0xb,
    VDP_VID_IFMT_PKG_12      = 0xc,
    VDP_VID_IFMT_RGB_888     = 0x8,
    VDP_VID_IFMT_PKG_YUV444  = 0xe,
    VDP_VID_IFMT_ARGB_8888   = 0xf,
    VDP_VID_IFMT_BUTT

}VDP_VID_IFMT_E;

typedef enum tagVDP_DATA_RMODE_E
{
    VDP_RMODE_INTERFACE   = 0,
    VDP_RMODE_INTERLACE   = 0,
    VDP_RMODE_PROGRESSIVE = 1,
    VDP_RMODE_TOP         = 2,
    VDP_RMODE_BOTTOM      = 3,
    VDP_RMODE_PRO_TOP     = 4,
    VDP_RMODE_PRO_BOTTOM  = 5,
    VDP_RMODE_BUTT

} VDP_DATA_RMODE_E;

/*******************************************************************
* End    :    For CMP(Compress) and DCMP(Decompress).
*******************************************************************/

//-----------------------------------
//graphics layer
//-----------------------------------
typedef enum tagVDP_GFX_IFMT_E
{
    VDP_GFX_IFMT_CLUT_1BPP   = 0x00,
    VDP_GFX_IFMT_CLUT_2BPP   = 0x10,
    VDP_GFX_IFMT_CLUT_4BPP   = 0x20,
    VDP_GFX_IFMT_CLUT_8BPP   = 0x30,

    VDP_GFX_IFMT_ACLUT_44    = 0x38,

    VDP_GFX_IFMT_RGB_444     = 0x40,
    VDP_GFX_IFMT_RGB_555     = 0x41,
    VDP_GFX_IFMT_RGB_565     = 0x42,

    VDP_GFX_IFMT_PKG_UYVY    = 0x43,
    VDP_GFX_IFMT_PKG_YUYV    = 0x44,
    VDP_GFX_IFMT_PKG_YVYU    = 0x45,

    VDP_GFX_IFMT_ACLUT_88    = 0x46,
    VDP_GFX_IFMT_ARGB_4444   = 0x48,
    VDP_GFX_IFMT_ARGB_1555   = 0x49,

    VDP_GFX_IFMT_RGB_888     = 0x50,//24bpp
    VDP_GFX_IFMT_YCBCR_888   = 0x51,//24bpp
    VDP_GFX_IFMT_ARGB_8565   = 0x5a,//24bpp

    VDP_GFX_IFMT_KRGB_888    = 0x60,
    VDP_GFX_IFMT_ARGB_8888   = 0x68,
    VDP_GFX_IFMT_AYCBCR_8888 = 0x69,

    VDP_GFX_IFMT_RGBA_4444   = 0xc8,
    VDP_GFX_IFMT_RGBA_5551   = 0xc9,
    VDP_GFX_IFMT_RGBA_5658   = 0xda,//24bpp
    VDP_GFX_IFMT_RGBA_8888   = 0xe8,
    VDP_GFX_IFMT_YCBCRA_8888 = 0xe9,
    VDP_GFX_IFMT_ABGR_8888   = 0xef,

    VDP_GFX_IFMT_STILL_888   = 0x100, // only use by environment

    VDP_GFX_IFMT_BUTT

}VDP_GFX_IFMT_E;

typedef enum tagVDP_PROC_FMT_E
{
    VDP_PROC_FMT_SP_420      = 0x0,
    VDP_PROC_FMT_SP_422      = 0x1,
    VDP_PROC_FMT_SP_444      = 0x2,//plannar,in YUV color domain
    VDP_PROC_FMT_RGB_888     = 0x3,//package,in RGB color domain
    VDP_PROC_FMT_RGB_444     = 0x4,//plannar,in RGB color domain

    VDP_PROC_FMT_BUTT

}VDP_PROC_FMT_E;

typedef enum tagVDP_CSC_MODE_E
{
    VDP_CSC_YUV2YUV     = 1,
    VDP_CSC_YUV2RGB_601    ,
    VDP_CSC_YUV2RGB_709    ,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_RGB2YUV_601    ,
    VDP_CSC_RGB2YUV_709    ,
    VDP_CSC_YUV2YUV_MAX    ,
    VDP_CSC_YUV2YUV_MIN    ,
    VDP_CSC_YUV2YUV_RAND   ,

    VDP_CSC_BUTT
} VDP_CSC_MODE_E;

typedef struct tagVDP_BKG_S
{
    HI_U32 u32BkgY;
    HI_U32 u32BkgU;
    HI_U32 u32BkgV;

    HI_U32 u32BkgA;

    HI_U32 bBkType;

} VDP_BKG_S;

typedef enum tagVO_WBC_ALIGN_MODE_E
{
    ALIGN_MODE_32BURST = 0x0,
    ALIGN_MODE_16BURST = 0x1,

    ALIGN_MODE_BUTT
}VO_WBC_ALIGN_MODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* End of __VOU_DEF_H__ */

