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


#ifndef __GRAPHICS_DRV_H__
#define __GRAPHICS_DRV_H__

#include "hi_osal.h"
#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_vo.h"
#include "hifb_graphic_hal.h"
//#include "gf_ghdr_lut.h"
#include "hifb_coef_org.h"


HI_S32 GRAPHICS_DRV_CheckDevId(VO_DEV VoDev);


#define  COEF_SIZE_G0ZME          (4096*128/8)

/* debug print format                                                       */
#define HIFB_GRAPHICS_TRACE(level, fmt...)\
    do{ \
            HI_TRACE(level,HI_ID_FB,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
            HI_TRACE(level,HI_ID_FB,##fmt);\
    }while(0)

typedef HI_S32 (*FB_IntCallBack)(HI_VOID *pParaml, HI_VOID *pParamr);

#define GRAPHIC_ALPHA_TRANSPARENT   0x0
#define GRAPHIC_ALPHA_OPACITY       0xff

#define GFX_SPIN_LOCK_FLAG                      unsigned long
#define GFX_SPIN_LOCK_S                         osal_spinlock_t
#define GFX_SPIN_LOCK_INIT(pLock)               osal_spin_lock_init(pLock)
#define GFX_SPIN_LOCK_DEINIT(pLock)             osal_spin_lock_destory(pLock)
#define GFX_SPIN_LOCK_IRQSAVE(pLock, flag)      osal_spin_lock_irqsave(pLock, flag)
#define GFX_SPIN_UNLOCK_IRQRESTORE(pLock, flag) osal_spin_unlock_irqrestore(pLock, flag)

/* debug print format                                                       */
#define GRAPHICS_DRV_TRACE(level, fmt...)\
    do{ \
            HI_TRACE(level,HI_ID_FB,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
            HI_TRACE(level,HI_ID_FB,##fmt);\
    }while(0)

#define GRAPHICS_CHECK_DEV_ID(VoDev)\
        do {\
            HI_S32 s32Ret;\
            s32Ret = GRAPHICS_DRV_CheckDevId(VoDev);\
            if (HI_SUCCESS != s32Ret)\
            {\
                GRAPHICS_DRV_TRACE(HI_DBG_ERR, "DEV ID %d invalid!\n", VoDev);\
                return s32Ret;\
            }\
        }while(0)

#define GRAPHICS_CHECK_NULL_PTR(ptr)\
    do {\
        if (NULL == ptr)\
        {\
            GRAPHICS_DRV_TRACE(HI_DBG_ERR, "Null ptr!\n");\
            return HI_ERR_VO_NULL_PTR;\
        }\
    }while(0)

typedef struct
{
    HI_U32 u32StartPhyAddr;
    HI_U32 u32EndPhyAddr;
}MDDRC_ZONE_ADDR_S;

typedef struct
{
    HI_U32 u32AR_PhyAddr;
    HI_U32 u32GB_PhyAddr;
    HI_U32 u32Offset;
    HI_U32 u32Stride;
    HIFB_RECT stInRect;
    HIFB_RECT stOutRect;
}GRAPHIC_WBC_INFO_S;

typedef struct
{
    HAL_DISP_PIXEL_FORMAT_E enPixelFmt; // new
    HI_U64 u64AR_PhyAddr;
    HI_U64 u64GB_PhyAddr;   // new
    HI_U32 u32Frame_Size0;  // new
    HI_U32 u32Frame_Size1;  // new
    HI_U32 u32Width;        // new
    HI_U32 u32Height;       // new
    HI_BOOL IsLosslessA;    // new
    HI_BOOL IsLossless;     // new
    HI_U32 u32Offset;
    HI_U32 u32Stride;
}GRAPHIC_DCMP_INFO_S;


typedef struct hiVO_GFXLAYER_CONTEXT_S
{
    HAL_DISP_LAYER_E  enLayerId;
    HI_BOOL           bOpened;
    HI_BOOL           bBinded;
    VO_DEV            s32BindedDev;

    FB_IntCallBack    pfVoCallBack;     /*VSYNC interrupt callback function */
    HI_VOID *         pVoCallBackArg;

    FB_IntCallBack    pfWbcCallBack;    /*VSYNC interrupt callback function */
    HI_VOID *         pWbcCallBackArg;

    VO_CSC_S          stGfxCsc;
    CscCoefParam_S    stCscCoefParam;
    GFX_SPIN_LOCK_S   spinLock;

    /* DDR detection area used in this layer */
    HI_U32            u32StartSection;
    HI_U32            u32ZoneNums;
}VO_GFXLAYER_CONTEXT_S;


/********************************************************************
*     Begin   : Graphic Layer hdr data structures.
********************************************************************/
typedef enum
{
    GF_HIHDR_G_TYP =0,
    GF_HIHDR_G_TYP1 ,
    GF_HIHDR_G_RAND ,
    GF_HIHDR_G_MAX ,
    GF_HIHDR_G_MIN ,
    GF_HIHDR_G_ZERO ,
    GF_HIHDR_G_BUTT
}GF_HIHDR_G_MODE_E;

typedef enum
{
    GF_CSC_G0_TYP =0,
    GF_CSC_G0_TYP1 ,
    GF_CSC_G0_RAND ,
    GF_CSC_G0_MAX ,
    GF_CSC_G0_MIN ,
    GF_CSC_G0_ZERO ,
    GF_CSC_G0_BUTT
}GF_CSC_G0_MODE_E;

typedef enum
{
    GF_SDR_IN_HDR10_OUT = 0, //hifonev500 test ok!
    GF_SDR_IN_HLG_OUT      , //hifonev500 test ok!
    GF_SDR_PREMULT         , //hifonev500 test ok!
    GF_SDR709_IN_2020_OUT  , //reserved!
    GF_BUTT
}HIGHDR_SCENE_MODE_E;


typedef struct
{
    HI_U32 ghdr_en                 ;
    HI_U32 ghdr_degmm_en           ;
    HI_U32 ghdr_gamut_en           ;
    HI_U32 ghdr_tmap_en            ;
    HI_U32 ghdr_gmm_en             ;
    HI_U32 ghdr_dither_en          ;

    // dither ctrl
    HI_U32 ghdr_dither_round       ;
    HI_U32 ghdr_dither_domain_mode ;
    HI_U32 ghdr_dither_tap_mode    ;
    HI_U32 ghdr_dither_round_unlim ;

    //ck_gt_en
    HI_U32 ghdr_ck_gt_en           ;

    //demo
    HI_U32 ghdr_demo_en            ;
    HI_U32 ghdr_demo_mode          ;
    HI_U32 ghdr_demo_pos           ;

    //HiHDR Scene
    HIGHDR_SCENE_MODE_E ghdr_scene_mode;
}GF_HIHDR_G_CFG_S;

typedef struct
{
    HI_U32 hihdr_r2y_en            ;

    //HiHDR Scene
    HIGHDR_SCENE_MODE_E ghdr_scene_mode;
}GF_CSC_G0_CFG_S;
/********************************************************************
*     End    : Graphic Layer hdr data structures.
********************************************************************/


/********************************************************************
*     Begin  : Graphic Layer DCMP data structures.
********************************************************************/
typedef enum tagVDP_GP_ORDER_E
{
    VDP_GP_ORDER_NULL     = 0x0,
    VDP_GP_ORDER_CSC      = 0x1,
    VDP_GP_ORDER_ZME      = 0x2,
    VDP_GP_ORDER_CSC_ZME  = 0x3,
    VDP_GP_ORDER_ZME_CSC  = 0x4,

    VDP_GP_ORDER_BUTT
} VDP_GP_ORDER_E;

typedef enum tagVDP_GFX_BITEXTEND_E
{
    VDP_GFX_BITEXTEND_1ST =   0,
    VDP_GFX_BITEXTEND_2ND = 0x2,
    VDP_GFX_BITEXTEND_3RD = 0x3,

    VDP_GFX_BITEXTEND_BUTT
}VDP_GFX_BITEXTEND_E;
typedef struct
{
    HI_U32  uAData  ;
    HI_U32  uRData  ;
    HI_U32  uGData  ;
    HI_U32  uBData  ;
} VDP_GFX_LUT_DATA_S;

typedef struct tagVDP_GFX_CKEY_S
{
    HI_U32 u32Key_r_min;
    HI_U32 u32Key_g_min;
    HI_U32 u32Key_b_min;

    HI_U32 u32Key_r_max;
    HI_U32 u32Key_g_max;
    HI_U32 u32Key_b_max;

    HI_U32 u32Key_r_msk;
    HI_U32 u32Key_g_msk;
    HI_U32 u32Key_b_msk;

    HI_U32 bKeyMode;
} VDP_GFX_CKEY_S;

typedef struct tagVDP_GFX_MASK_S
{
    HI_U32 u32Mask_r;
    HI_U32 u32Mask_g;
    HI_U32 u32Mask_b;
} VDP_GFX_MASK_S;

typedef enum tagVDP_GFX_PARA_E
{
    VDP_GFX_PARA_LUT = 0,

    VDP_GFX_PARA_BUTT
} VDP_GFX_PARA_E;

typedef enum tagVDP_GP_PARA_E
{
    VDP_GP_PARA_ZME_HORL = 0,
    VDP_GP_PARA_ZME_HORC  ,
    VDP_GP_PARA_ZME_VERL  ,
    VDP_GP_PARA_ZME_VERC  ,

    VDP_GP_GTI_PARA_ZME_HORL  ,
    VDP_GP_GTI_PARA_ZME_HORC  ,
    VDP_GP_GTI_PARA_ZME_VERL  ,
    VDP_GP_GTI_PARA_ZME_VERC  ,

    VDP_GP_PARA_ZME_HOR   ,
    VDP_GP_PARA_ZME_VER   ,
    VDP_GP_PARA_HDR_DEGAMM    ,
    VDP_GP_PARA_HDR_GAMM    ,

    VDP_GP_PARA_BUTT
} VDP_GP_PARA_E;

typedef enum
{
    VDP_REQ_CTRL_16BURST_1,
    VDP_REQ_CTRL_16BURST_2,
    VDP_REQ_CTRL_16BURST_4,
    VDP_REQ_CTRL_16BURST_8,
    VDP_REQ_CTRL_BUTT
}VDP_REQ_CTRL_E;
/********************************************************************
*     End    : Graphic Layer DCMP data structures.
********************************************************************/


/********************************************************************
*  [Begin]   : Graphic Layer ZME relative data structures.
********************************************************************/
typedef enum
{
    VDP_G0_ZME_TYP =0,
    VDP_G0_ZME_TYP1 ,
    VDP_G0_ZME_RAND ,
    VDP_G0_ZME_MAX ,
    VDP_G0_ZME_MIN ,
    VDP_G0_ZME_ZERO ,
    VDP_G0_ZME_BUTT
}GF_G0_ZME_MODE_E;

typedef enum
{
    VDP_G0_ZME_FIR_COPY = 0,
    VDP_G0_ZME_FIR_FILT = 1
}GF_G0_ZME_FIR_MODE_E;

typedef struct
{
    HI_U32 ck_gt_en         ;
    HI_U32 in_width         ;
    HI_U32 out_width        ;
    HI_U32 out_pro          ;

    HI_U32 hfir_en          ;
    HI_U32 lhmid_en         ;
    HI_U32 ahmid_en         ;
    HI_U32 lhfir_mode       ;
    HI_U32 ahfir_mode       ;

    HI_U32 in_height        ;
    HI_U32 out_height       ;

    HI_U32  vfir_en         ;
    HI_U32 lvmid_en         ;
    HI_U32 avmid_en         ;
    HI_U32 lvfir_mode       ;
    HI_U32 avfir_mode       ;

}GF_G0_ZME_CFG_S;


typedef enum tagGF_RM_COEF_MODE_E
{
    GF_RM_COEF_MODE_TYP  = 0x0,
    GF_RM_COEF_MODE_RAN  = 0x1,
    GF_RM_COEF_MODE_MIN  = 0x2,
    GF_RM_COEF_MODE_MAX  = 0x3,
    GF_RM_COEF_MODE_ZRO  = 0x4,
    GF_RM_COEF_MODE_CUS  = 0x5,
    GF_RM_COEF_MODE_UP   = 0x6,
    GF_RM_COEF_MODE_BUTT
} GF_RM_COEF_MODE_E;

typedef enum
{
    GFX_DCMP_SRC_MODE_ARGB8888 = 0,
    GFX_DCMP_SRC_MODE_ARGB6888    ,
    GFX_DCMP_SRC_MODE_ARGB1555    ,
    GFX_DCMP_SRC_MODE_ARGB4444    ,

    GFX_DCMP_SRC_MODE_BUTT
}GFX_DCMP_SRC_MODE_E;


typedef struct hiHIFB_MMZ_BUFFER_S
{
    HI_U64   u64StartPhyAddr;
    HI_VOID* pStartVirAddr;
    HI_U32   u32Size;
}HIFB_MMZ_BUFFER_S;

typedef enum
{
    HIFB_COEF_BUF_V0_HZME   = 0 ,
    HIFB_COEF_BUF_V0_VZME   = 1 ,
    HIFB_COEF_BUF_V0_HDRD   = 2 ,
    HIFB_COEF_BUF_V0_HDRS   = 3 ,
    HIFB_COEF_BUF_G0_HDR_TMAP  = 4 ,
    HIFB_COEF_BUF_G0_HDR_GMM   = 5 ,
    HIFB_COEF_BUF_G0ZME        = 6 ,
    HIFB_COEF_BUF_WD_HZME   = 7 ,
    HIFB_COEF_BUF_WD_VZME   = 8 ,
    HIFB_COEF_BUF_WD_HDRD   = 9 ,
    HIFB_COEF_BUF_WD_HDRS   = 10,

    HIFB_COEF_BUF_REGION_V0 = 12,
    HIFB_COEF_BUF_REGION_V1 = 13,
    HIFB_COEF_BUF_SHARPEN   ,
    HIFB_COEF_BUF_DCI       ,
    HIFB_COEF_BUF_ACM       ,
    HIFB_COEF_BUF_VZME      ,
    HIFB_COEF_BUF_HZME      ,
    HIFB_COEF_BUF_GPZME     ,
    HIFB_COEF_BUF_DIM       ,
    HIFB_COEF_BUF_GMM       ,
    HIFB_COEF_BUF_OD        ,
    HIFB_COEF_BUF_CLUT      ,
    HIFB_COEF_BUF_PCID      ,

    HIFB_COEF_BUF_ALL       = 32 ,
    HIFB_COEF_BUF_BUTT      = 33

}HIFB_COEF_BUF_E;

typedef struct hiHIFB_COEF_ADDR_S
{
    HIFB_MMZ_BUFFER_S       stBufBaseAddr;
    HI_U32                  u32size;

    HI_U8*                  pu8CoefVirAddr[HIFB_COEF_BUF_BUTT];
    HI_U64                  u64CoefPhyAddr[HIFB_COEF_BUF_BUTT];
}HIFB_COEF_ADDR_S;

typedef enum
{
    HIFB_DCMP_SRC_MODE_ARGB8888 = 0 ,
    HIFB_DCMP_SRC_MODE_ARGB1555     ,
    HIFB_DCMP_SRC_MODE_ARGB4444     ,
    HIFB_DCMP_SRC_MODE_RGB565       ,
    HIFB_DCMP_SRC_MODE_RGB888       ,
    HIFB_DCMP_SRC_MODE_YUV444       ,
    HIFB_DCMP_SRC_MODE_BUTT
}HIFB_DCMP_SRC_MODE_E;

typedef enum tagVOU_MIX_PRIO_E
{
    VOU_MIX_PRIO0 = 0,
    VOU_MIX_PRIO1,
    VOU_MIX_PRIO2,
    VOU_MIX_PRIO3,
    VOU_MIX_PRIO4,
    VOU_MIX_PRIO5,
    VOU_MIX_PRIO_CLR,
    VOU_MIX_BUTT
}VOU_MIX_PRIO_E;

typedef struct
{
    HI_U32 data3;
    HI_U32 data2;
    HI_U32 data1;
    HI_U32 data0;
    HI_U32 depth;
}HIFB_DRV_U128_S;

typedef enum tagHIFB_DRV_COEF_DATA_TYPE
{
    DRV_COEF_DATA_TYPE_U8 = 0,
    DRV_COEF_DATA_TYPE_S8  ,
    DRV_COEF_DATA_TYPE_U16 ,
    DRV_COEF_DATA_TYPE_S16 ,
    DRV_COEF_DATA_TYPE_U32 ,
    DRV_COEF_DATA_TYPE_S32 ,
    DRV_COEF_DATA_TYPE_BUTT
} HIFB_DRV_COEF_DATA_TYPE_E;

typedef struct
{
    HI_U8 *                     coef_addr;
    HI_U32                      lut_num;
    HI_U32                      burst_num;
    HI_U32                      cycle_num;
    void * *                    p_coef_array;
    HI_U32 *                    lut_length;
    HI_U32 *                    coef_bit_length;
    HIFB_DRV_COEF_DATA_TYPE_E     data_type;
}HIFB_DRV_COEF_SEND_CFG;

typedef enum tagHIFB_RM_COEF_MODE_E
{
    HIFB_RM_COEF_MODE_TYP  = 0x0,
    HIFB_RM_COEF_MODE_RAN  = 0x1,
    HIFB_RM_COEF_MODE_MIN  = 0x2,
    HIFB_RM_COEF_MODE_MAX  = 0x3,
    HIFB_RM_COEF_MODE_ZRO  = 0x4,
    HIFB_RM_COEF_MODE_CUS  = 0x5,
    HIFB_RM_COEF_MODE_UP   = 0x6,
    HIFB_RM_COEF_MODE_BUTT
} HIFB_RM_COEF_MODE_E;


typedef struct
{
    void*                       p_coef;
    void*                       p_coef_new;
    HIFB_DRV_COEF_DATA_TYPE_E   coef_data_type;
    HI_U32                      length;
    HIFB_RM_COEF_MODE_E         coef_data_mode;
    HI_S32                      coef_max;
    HI_S32                      coef_min;
}HIFB_DRV_COEF_GEN_CFG;

typedef enum hiHIFB_CSC_RANGE_E
{
    HIFB_CSC_DEF_VAL          = 50,
    HIFB_CSC_LUMA_MAX         = 100,
    HIFB_CSC_LUMA_MIN         = 0,

    HIFB_CSC_CONT_MAX         = 100,
    HIFB_CSC_CONT_MIN         = 0,

    HIFB_CSC_HUE_MAX         = 100,
    HIFB_CSC_HUE_MIN         = 0,

    HIFB_CSC_SAT_MAX         = 100,
    HIFB_CSC_SAT_MIN         = 0,

}HIFB_CSC_RANGE_E;

/********************************************************************
*  [End  ]   : Graphic Layer ZME relative data structures.
********************************************************************/



//for compress
HI_VOID Graphics_DRV_GetMDDRStatus(HI_U32 u32StartID, HI_U32 u32Cnt, HI_U32 *pu32Status);
HI_VOID MDDRC_DRV_SetZoneAddr(HI_U32 u32StartID, HI_U32 u32Cnt, MDDRC_ZONE_ADDR_S *pstZoneAddr);
HI_BOOL GRAPHIC_DRV_SetMDDRDetectZone(HAL_DISP_LAYER_E enLayer, HI_U32 u32StartSection, HI_U32 u32ZoneNums);
HI_VOID MDDRC_DRV_InitMDDRDetect(HI_VOID);
//Being : new feature decompress
HI_BOOL GRAPHIC_DRV_EnableDcmp(HAL_DISP_LAYER_E enLayer, HI_BOOL bEnable);
HI_BOOL GRAPHIC_DRV_GetDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable);
//End  : new feature decompress
HI_S32  GRAPHIC_DRV_GetHalLayer(HI_U32 u32Layer, HAL_DISP_LAYER_E *penLayer);
HI_S32  GRAPHIC_DRV_GetLayerIndex(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32Layer);
HI_S32  GRAPHIC_DRV_GetLayerID(HI_U32 u32Layer, HAL_DISP_LAYER_E *penLayer);


HI_BOOL GRAPHIC_DRV_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut);
HI_BOOL GRAPHIC_DRV_SetGfxExt(HAL_DISP_LAYER_E enLayer, HAL_GFX_BITEXTEND_E enMode);
HI_BOOL GRAPHIC_DRV_SetGfxPalpha(HAL_DISP_LAYER_E enLayer, HI_U32 bAlphaEn, HI_U32 bArange, HI_U8 u8Alpha0, HI_U8 u8Alpha1);
HI_BOOL GRAPHIC_DRV_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,HI_U8 u8Alpha0);
HI_BOOL GRAPHIC_DRV_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn);
HI_BOOL GRAPHIC_DRV_SetLayerAddr(HAL_DISP_LAYER_E u32LayerId, HI_U64 u64Addr);
HI_BOOL GRAPHIC_DRV_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch);
HI_BOOL GRAPHIC_DRV_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 *pbEnable);
HI_BOOL GRAPHIC_DRV_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 pbEnable);
HI_BOOL GRAPHIC_DRV_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer, HAL_DISP_PIXEL_FORMAT_E  enDataFmt);
HI_BOOL GRAPHIC_DRV_SetLayerInRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL GRAPHIC_DRV_SetSrcImageResolution(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL GRAPHIC_DRV_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL GRAPHIC_DRV_SetColorKeyValue(HAL_DISP_LAYER_E enLayer, HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin);
HI_BOOL GRAPHIC_DRV_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk);
HI_BOOL GRAPHIC_DRV_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable);
HI_BOOL GRAPHIC_DRV_SetRegUp(HAL_DISP_LAYER_E enLayer);
HI_BOOL GRAPHIC_DRV_GetLayerGalpha(HAL_DISP_LAYER_E enLayer,HI_U8 *pu8Alpha0);
HI_BOOL GRAPHIC_DRV_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer,HI_U32 *pu32Fmt);
HI_BOOL GRAPHIC_DRV_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 *pu64GfxAddr);
HI_BOOL GRAPHIC_DRV_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32GfxStride);
HI_BOOL GRAPHIC_DRV_GetScanMode(VO_DEV VoDev,HI_BOOL *pbIop);

HI_VOID GRAPHIC_DRV_ClrWbcMask(HAL_DISP_LAYER_E enLayer);
HI_VOID GRAPHIC_DRV_ClearWbcIntStatus(HAL_DISP_LAYER_E enLayer);
HI_BOOL GRAPHIC_DRV_GetVtThdMode(VO_DEV VoDev, HI_BOOL *pbFeildUpdate);

HI_BOOL GRAPHIC_DRV_GetIntState(HAL_DISP_LAYER_E gfxLayer, HI_BOOL *pbBottom);

HI_S32 GRAPHIC_DRV_CheckGraphicBindSupport(GRAPHIC_LAYER gfxLayer, VO_DEV VoDev);
HI_S32 GRAPHIC_DRV_GraphicBindDev(GRAPHIC_LAYER gfxLayer, VO_DEV VoDev);
HI_S32 GRAPHIC_DRV_GraphicUnBindDev(GRAPHIC_LAYER gfxLayer, VO_DEV VoDev);

HI_VOID GRAPHIC_ZME_COEF_INIT(HI_VOID);
HI_S32 GRAPHIC_DRV_GetBindDev(HI_S32 s32LayaerId);
HI_S32 GRAPHIC_DRV_Init(HI_VOID);
HI_S32 GRAPHIC_DRV_Exit(HI_VOID);
HI_S32 GRAPHIC_DRV_Resource_Init(HI_VOID);
HI_S32 GRAPHIC_DRV_Resource_Exit(HI_VOID);

HI_S32 GRAPHIC_DRV_EnableLayer(HAL_DISP_LAYER_E gfxLayer, HI_BOOL bEnable);

HI_S32 GRAPHIC_DRV_SetCscCoef(HAL_DISP_LAYER_E gfxLayer, VO_CSC_S *pstGfxCsc, CscCoefParam_S * pstCscCoefParam);
HI_BOOL GRAPHIC_DRV_IsChipSupportCsc(HI_VOID);
HI_S32 GRAPHIC_DRV_CheckCscCoef(HAL_DISP_LAYER_E gfxLayer, VO_CSC_S *pstGfxCsc);
HI_S32    GRAPHIC_DRV_ShowProc(osal_proc_entry_t *s);
HI_VOID GRAPHIC_DRV_DevIntEnable(VO_DEV VoDev, HI_BOOL Enable);
HI_VOID GRAPHIC_DRV_IntClear(HI_U32 u32IntClear, HI_S32 s32Irq);
HI_U32 GRAPHIC_DRV_IntGetStatus(HI_VOID);
HI_VOID GRAPHIC_DRV_ClrIntStatus(HI_U32 u32IntStatus);
HI_S32 GRAPHIC_DRV_GetInterruptDev(HI_U32 IntStatus, VO_DEV* pVoDev);
HI_S32 VOU_GraphicsInit(HI_VOID);
HI_S32 VOU_GraphicsDeInit(HI_VOID);

/********************************************************************
*  [Begin]   : Graphic Layer ZME relative drv funtions.
********************************************************************/
//function declare
HI_VOID GF_FUNC_SetG0ZmeMode(HI_U32 enLayer , GF_G0_ZME_MODE_E G0ZmeMode, GF_G0_ZME_CFG_S * pstCfg);
/********************************************************************
*  [End  ]   : Graphic Layer ZME relative drv functions.
********************************************************************/

/********************************************************************
*  [Begin]   : Graphic Layer ZME relative drv coef functions.
********************************************************************/
//function declare
HI_S32  GF_DRV_SetG0zmeCoef(HI_S16 * pcoef_h,HI_S16 * pcoef_v);
HI_VOID GF_vSetG0zmeCoef(GF_RM_COEF_MODE_E enCoefMode);
/********************************************************************
*  [End  ]   : Graphic Layer ZME relative drv coeffunctions.
********************************************************************/
HI_BOOL GRAPHIC_DRV_EnableZME(HI_U32 enLayer, GF_G0_ZME_CFG_S* pst_zme_cfg, HI_BOOL bEnableZme);


#endif
