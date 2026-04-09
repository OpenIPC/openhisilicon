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

#include "proc_ext.h"

#include "hi_errno.h"
#include "hi_debug.h"
#include "hifb_graphics_drv.h"
#include "hifb_vou_graphics.h"

#include "mm_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"

extern VO_GFXLAYER_CONTEXT_S s_astGfxLayerCtx[VO_MAX_GRAPHICS_LAYER_NUM];

#define VOU_INTMSK_G0WBC (0x400)
#define VOU_INTMSK_G4WBC (0x800)

#define VOU1_IRQ_NR 91

#define GFX_CSC_SCALE      0xd
#define GFX_CSC_CLIP_MIN   0x0
#define GFX_CSC_CLIP_MAX   0x3ff


HIFB_COEF_ADDR_S g_stHifbCoefBufAddr;

/* vou interrupt mask type */
typedef enum tagHIFB_INT_MASK_E
{
    HIFB_INTMSK_NONE         =   0,
    HIFB_INTMSK_DHD0_VTTHD1  = 0x1,
    HIFB_INTMSK_DHD0_VTTHD2  = 0x2,
    HIFB_INTMSK_DHD0_VTTHD3  = 0x4,
    HIFB_INTMSK_DHD0_UFINT   = 0x8,

    HIFB_INTMSK_DHD1_VTTHD1  = 0x10,
    HIFB_INTMSK_DHD1_VTTHD2  = 0x20,
    HIFB_INTMSK_DHD1_VTTHD3  = 0x40,
    HIFB_INTMSK_DHD1_UFINT   = 0x80,

    HIFB_INTMSK_DSD_VTTHD1   = 0x100,
    HIFB_INTMSK_DSD_VTTHD2   = 0x200,
    HIFB_INTMSK_DSD_VTTHD3   = 0x400,
    HIFB_INTMSK_DSD_UFINT    = 0x800,

    HIFB_INTMSK_B0_ERR       = 0x1000,
    HIFB_INTMSK_B1_ERR       = 0x2000,
    HIFB_INTMSK_B2_ERR       = 0x4000,

    HIFB_INTMSK_WBC_DHDOVER = 0x8000,
    HIFB_INTREPORT_ALL = 0xffffffff
} HIFB_INT_MASK_E;

/* RGB->YUV601 Constant coefficient matrix */
const CscCoef_S g_stCSC_RGB2YUV601_tv
    = {
     /*csc coef*/
     258, 504, 98, -148, -291, 439, 439, -368, -71,
     /*csc Input DC(IDC)*/
     0, 0, 0,
     /*csc Output DC(ODC)*/
     16, 128, 128,
    };
/* RGB->YUV601 Constant coefficient matrix */
const CscCoef_S g_stCSC_RGB2YUV601_pc
    = {
     /*csc coef*/
     299, 587, 114, -172, -339, 511, 511, -428, -83,
     /*csc Input DC(IDC)*/
     0, 0, 0,
     /*csc Output DC(ODC)*/
     0, 128, 128,/*64, 512, 512,*/
    };
/* RGB->YUV709 Constant coefficient matrix */
const CscCoef_S g_stCSC_RGB2YUV709_tv
    = {
     /*csc coef*/
     184, 614, 62, -101, -338, 439, 439, -399, -40,
     /*csc Input DC(IDC)*/
     0, 0, 0,
     /*csc Output DC(ODC)*/
     16, 128, 128,
    };

/* RGB->YUV709 Constant coefficient matrix, output full[0,255] */
const CscCoef_S g_stCSC_RGB2YUV709_pc
    = {
     /*csc coef*/
     213, 715, 72, -117, -394, 511, 511, -464, -47,
     /*csc Input DC(IDC)*/
     0, 0, 0,
     /*csc Output DC(ODC)*/
     0, 128, 128,
    };
/* RGB -> YUV BT.2020 Coefficient Matrix */
const CscCoef_S g_stCSC_RGB2YUV2020_pc =
{
    /*csc Multiplier coefficient*/
    263, 678, 59, -143, -368, 511, 511, -470, -41,
    /*csc Input DC component (IDC)*/
    0, 0, 0,
    /*csc Output DC component (ODC)*/
    0, 128, 128,
};


/* YUV601->RGB Constant coefficient matrix */
const CscCoef_S g_stCSC_YUV6012RGB_pc
    = {
     /*csc coef*/
     1164, 0, 1596, 1164, -391, -813, 1164, 2018, 0,
     /*csc Input DC(IDC)*/
     -16, -128, -128,
     /*csc Output DC(ODC)*/
     0, 0, 0,
    };
/* YUV709->RGB Constant coefficient matrix */
#if 1
// old
const CscCoef_S g_stCSC_YUV7092RGB_pc
    = {
     /*csc coef*/
     1164, 0, 1793, 1164, -213, -534, 1164, 2115, 0,
     /*csc Input DC(IDC)*/
     -16, -128, -128,
     /*csc Output DC(ODC)*/
     0, 0, 0,
    };
#else
const CscCoef_S g_stCSC_YUV7092RGB_pc=
{
     /*csc coef*/
     1024, 0, 1576, 1024, -187, -470, 1024, 1859, 0,
     /*csc Input DC(IDC)*/
     -64, -512, 1536,
     /*csc Output DC(ODC)*/
     0, 0, 0,
};

#endif

/* BT.2020 YUV -> RGB Coefficient Matrix */
const CscCoef_S g_stCSC_YUV20202RGB_pc =
{
    /*csc Multiplier coefficient*/
    1000, 0, 1442, 1000, -160, -560, 1000, 1841, 0,
    /*csc Input DC component (IDC)*/
    0, -128, -128,
    /*csc Output DC component (ODC)*/
    0, 0, 0,
};


/* YUV601->YUV709 Constant coefficient matrix, output full[0,255] */
const CscCoef_S g_stCSC_YUV2YUV_601_709
    = {
     /*csc coef*/
     1000, -116, 208, 0, 1017, 114, 0, 75, 1025,
     /*csc Input DC(IDC)*/
     -16, -128, -128,
     /*csc Output DC(ODC)*/
     16, 128, 128,
    };
/* YUV709->YUV601 Constant coefficient matrix,output full[0,255] */
const CscCoef_S g_stCSC_YUV2YUV_709_601
    = {
     /*csc coef*/
     1000, 99, 192, 0, 990, -111, 0, -72, 983,
     /*csc Input DC(IDC)*/
     -16, -128, -128,
     /*csc Output DC(ODC)*/
     16, 128, 128,
    };
/* YUV601->YUV709 Constant coefficient matrix */
const CscCoef_S g_stCSC_Init
    = {
     /*csc coef*/
     1000, 0, 0, 0, 1000, 0, 0, 0, 1000,
     /*csc Input DC(IDC)*/
     -16, -128, -128,
     /*csc Output DC(ODC)*/
     16, 128, 128,
    };


const int HIFB_SIN_TABLE[61] = {
  -500,  -485,  -469,  -454,  -438,  -422,  -407,  -391,  -374,  -358,
  -342,  -325,  -309,  -292,  -276,  -259,  -242,  -225,  -208,  -191,
  -174,  -156,  -139,  -122,  -104,   -87,   -70,   -52,   -35,   -17,
     0,    17,    35,    52,    70,    87,   104,   122,   139,   156,
   174,   191,   208,   225,   242,   259,   276,   292,   309,   325,
   342,   358,   374,   391,   407,   422,   438,   454,   469,   485,
   500};

const int HIFB_COS_TABLE[61] = {
   866,   875,   883,   891,   899,   906,   914,   921,   927,   934,
   940,   946,   951,   956,   961,   966,   970,   974,   978,   982,
   985,   988,   990,   993,   995,   996,   998,   999,   999,  1000,
  1000,  1000,   999,   999,   998,   996,   995,   993,   990,   988,
   985,   982,   978,   974,   970,   966,   961,   956,   951,   946,
   940,   934,   927,   921,   914,   906,   899,   891,   883,   875,
   866};



HI_S32 GRAPHIC_DRV_GetHalLayer(HI_U32 u32Layer, HAL_DISP_LAYER_E *penLayer)
{
    switch (u32Layer)
    {
        case 0 :
        {
            *penLayer = HAL_DISP_LAYER_GFX0;
            break;
        }
        case 1 :
        {
            *penLayer = HAL_DISP_LAYER_GFX1;
            break;
        }
        case 2 :
        {
            *penLayer = HAL_DISP_LAYER_GFX3;
            break;
        }
        default:
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 GRAPHIC_DRV_GetLayerIndex(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32Layer)
{
    switch (enLayer)
    {
        case HAL_DISP_LAYER_GFX0:
        {
            *pu32Layer = 0;
            break;
        }
        case HAL_DISP_LAYER_GFX1:
        {
            *pu32Layer = 1;
            break;
        }
        case HAL_DISP_LAYER_GFX3:
        {
            *pu32Layer = 2;
            break;
        }
        default:
        {
            return HI_ERR_VO_GFX_INVALID_ID;
        }
    }

    return HI_SUCCESS;
}

HI_S32 GRAPHIC_DRV_GetLayerID(HI_U32 u32Layer, HAL_DISP_LAYER_E *penLayer)
{
    switch (u32Layer)
    {
        case VO_LAYER_G0:
        {
            *penLayer = HAL_DISP_LAYER_GFX0;
            break;
        }
        case VO_LAYER_G1:
        {
            *penLayer = HAL_DISP_LAYER_GFX1;
            break;
        }
        case VO_LAYER_G3:
        {
            *penLayer = HAL_DISP_LAYER_GFX3;
            break;
        }
        default:
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


extern HI_S32 VOU_DRV_CalcCscMatrix(HI_U32 u32Luma, HI_U32 u32Contrast,
    HI_U32 u32Hue, HI_U32 u32Satuature, HAL_CSC_MODE_E enCscMode, CscCoef_S *pstCstCoef);


HI_BOOL GRAPHIC_DRV_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut)
{
    if (HI_FALSE == HAL_GRAPHIC_SetGfxKeyMode(enLayer, u32KeyOut))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                                HAL_GFX_BITEXTEND_E enMode)
{
    if (HI_FALSE == HAL_GRAPHIC_SetGfxExt(enLayer, enMode))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                   HI_U32 bAlphaEn, HI_U32 bArange,
                                   HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
    if (HI_FALSE == HAL_GRAPHIC_SetGfxPalpha(enLayer, bAlphaEn, HI_TRUE,
                    u8Alpha0, u8Alpha1))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,
                                     HI_U8 u8Alpha0)
{
    if (HI_FALSE == HAL_LAYER_SetLayerGAlpha(enLayer, u8Alpha0))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    if (HI_FALSE == HAL_LAYER_SetCscEn(enLayer, bCscEn))
    {
        return HI_FALSE;
    }

    // TODO,G0's csc is in HIGHDR
    if (HAL_DISP_LAYER_GFX0 == enLayer)
    {
        return HI_TRUE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetLayerAddr(HAL_DISP_LAYER_E u32LayerId, HI_U64 u64Addr)
{
    HI_U32 u32LayerIndex;

    if (HI_SUCCESS != GRAPHIC_DRV_GetLayerIndex(u32LayerId, &u32LayerIndex))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "gfxLayer(%u) is invalid!\n", (HI_U32)u32LayerId);
        return HI_FALSE;
    }

    if (HI_FALSE == HAL_GRAPHIC_SetGfxAddr(u32LayerId, u64Addr))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch)
{
    HI_U32 u32LayerIndex;

    if (HI_SUCCESS != GRAPHIC_DRV_GetLayerIndex(enLayer, &u32LayerIndex))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "gfxLayer(%u) is invalid!\n", (HI_U32)enLayer);
        return HI_FALSE;
    }
    /* Stride need be divided by 16 before setting the register */
    if (HI_FALSE == HAL_GRAPHIC_SetGfxStride(enLayer, u16pitch>>4))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 *pbEnable)
{
    if (HI_FALSE == HAL_GRAPHIC_GetGfxPreMult(enLayer, pbEnable))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 pbEnable)
{
    if (HI_FALSE == HAL_GRAPHIC_SetGfxPreMult(enLayer, pbEnable))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                        HAL_DISP_PIXEL_FORMAT_E  enDataFmt)
{
    HI_U32 u32LayerIndex;
    if (HI_SUCCESS != GRAPHIC_DRV_GetLayerIndex(enLayer, &u32LayerIndex))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "gfxLayer(%u) is invalid!\n", (HI_U32)enLayer);
        return HI_FALSE;
    }

    if (HI_FALSE == HAL_LAYER_SetLayerDataFmt(enLayer, enDataFmt))
    {
        return HI_FALSE;
    }
    GRAPHICS_DRV_TRACE(HI_DBG_INFO, "Set Layer%d DataFmt: %d!\n", (HI_U32)enLayer,enDataFmt);
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetLayerInRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{
    HI_U32 u32LayerIndex;

    if (HI_SUCCESS != GRAPHIC_DRV_GetLayerIndex(enLayer, &u32LayerIndex))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "gfxLayer(%u) is invalid!\n", (HI_U32)enLayer);
        return HI_FALSE;
    }

    if (HI_FALSE == HAL_LAYER_SetLayerInRect(enLayer, pstRect))
    {
        return HI_FALSE;
    }

    if (HI_FALSE == HAL_VIDEO_SetLayerDispRect(enLayer, pstRect))
    {
        return HI_FALSE;
    }
    if (HI_FALSE == HAL_VIDEO_SetLayerVideoRect(enLayer, pstRect))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*
* Name : GRAPHIC_DRV_SetSrcImageResolution
* Desc : set the original resolution of the frame.
*/
HI_BOOL GRAPHIC_DRV_SetSrcImageResolution(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{

    if (HI_FALSE == HAL_LAYER_SetSrcResolution(enLayer, pstRect))
    {
        return HI_FALSE;
    }

    if (HI_FALSE == HAL_LAYER_SetLayerInRect(enLayer, pstRect))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{
    if (HI_FALSE == HAL_LAYER_SetLayerOutRect(enLayer, pstRect))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetColorKeyValue(HAL_DISP_LAYER_E enLayer,
        HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin)
{
    if (HI_FALSE == HAL_GRAPHIC_SetColorKeyValue(enLayer, stKeyMax, stKeyMin))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk)
{
    if (HI_FALSE == HAL_GRAPHIC_SetColorKeyMask(enLayer, stMsk))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable)
{
    if (HI_FALSE ==  HAL_GRAPHIC_SetGfxKeyEn(enLayer, u32KeyEnable))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL  GRAPHIC_DRV_SetRegUp(HAL_DISP_LAYER_E enLayer)
{
    if (HI_FALSE == HAL_LAYER_SetRegUp(enLayer))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetLayerGalpha(HAL_DISP_LAYER_E enLayer,HI_U8 *pu8Alpha0)
{
    if (HI_FALSE == HAL_LAYER_GetLayerGAlpha(enLayer, pu8Alpha0))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer,HI_U32 *pu32Fmt)
{
    if (HI_FALSE == HAL_LAYER_GetLayerDataFmt(enLayer, pu32Fmt))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 *pu64GfxAddr)
{
    if (HI_FALSE == HAL_GRAPHIC_GetGfxAddr(enLayer, pu64GfxAddr))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32GfxStride)
{
    if (HI_FALSE == HAL_GRAPHIC_GetGfxStride(enLayer, pu32GfxStride))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}


HI_BOOL GRAPHIC_DRV_GetScanMode(VO_DEV VoDev, HI_BOOL *pbIop)
{
    if (HI_FALSE == HAL_DISP_GetDispIoP(VoDev, pbIop))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_EnableDcmp(HAL_DISP_LAYER_E enLayer, HI_BOOL bEnable)
{
    HI_U32 u32LayerIndex;
    if (HI_SUCCESS != GRAPHIC_DRV_GetLayerIndex(enLayer, &u32LayerIndex))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "gfxLayer(%u) is invalid!\n", (HI_U32)enLayer);
        return HI_FALSE;
    }
    if (HI_FALSE == HAL_GRAPHIC_SetGfxDcmpEnable(enLayer, bEnable))
    {
        return HI_FALSE;
    }
    HAL_FDR_GFX_SetDcmpEn(u32LayerIndex,bEnable);
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable)
{
    if (HI_FALSE == HAL_GRAPHIC_GetGfxDcmpEnableState(enLayer, pbEnable))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetVtThdMode(VO_DEV VoDev, HI_BOOL *pbFeildUpdate)
{
    if (HI_FALSE == HAL_DISP_GetVtThdMode(VoDev, pbFeildUpdate))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}



HI_BOOL GRAPHIC_DRV_GetIntState(HAL_DISP_LAYER_E gfxLayer, HI_BOOL *pbBottom)
{
    if (HI_FALSE == HAL_DISP_GetIntState((HAL_DISP_OUTPUTCHANNEL_E)gfxLayer, pbBottom))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}


/*
* Name : VOU_DRV_CalcCscMatrix
* Desc : Calculate csc matrix.
*/
HI_S32 VOU_DRV_CalcCscMatrix(HI_U32 u32Luma, HI_U32 u32Contrast,
                             HI_U32 u32Hue, HI_U32 u32Satuature, HAL_CSC_MODE_E enCscMode, CscCoef_S* pstCstCoef)
{
    HI_S32 s32Luma     = 0;
    HI_S32 s32Contrast = 0;
    HI_S32 s32Hue      = 0;
    HI_S32 s32Satu     = 0;
    const CscCoef_S* pstCscTmp = NULL;

    s32Luma     = (HI_S32)u32Luma * 64 / 100 - 32;
    s32Contrast = ((HI_S32)u32Contrast - 50) * 2 + 100;
    s32Hue      = (HI_S32)u32Hue * 60 / 100;
    s32Satu     = ((HI_S32)u32Satuature - 50) * 2 + 100;

    switch (enCscMode)
    {
        case HAL_CSC_MODE_BT601_TO_BT601:
        case HAL_CSC_MODE_BT709_TO_BT709:
        case HAL_CSC_MODE_RGB_TO_RGB:
            pstCscTmp = &g_stCSC_Init;
            break;
        case HAL_CSC_MODE_BT709_TO_BT601:
            pstCscTmp = &g_stCSC_YUV2YUV_709_601;
            break;
        case HAL_CSC_MODE_BT601_TO_BT709:
            pstCscTmp = &g_stCSC_YUV2YUV_601_709;
            break;
        case HAL_CSC_MODE_BT601_TO_RGB_PC:
            pstCscTmp = &g_stCSC_YUV6012RGB_pc;
            break;
        case HAL_CSC_MODE_BT709_TO_RGB_PC:
            pstCscTmp = &g_stCSC_YUV7092RGB_pc;
            break;
        case HAL_CSC_MODE_RGB_TO_BT601_PC:
            pstCscTmp = &g_stCSC_RGB2YUV601_pc;
            break;
        case HAL_CSC_MODE_RGB_TO_BT709_PC:
            pstCscTmp = &g_stCSC_RGB2YUV709_pc;
            break;
        case HAL_CSC_MODE_RGB_TO_BT601_TV:
            pstCscTmp = &g_stCSC_RGB2YUV601_tv;
            break;
        case HAL_CSC_MODE_RGB_TO_BT709_TV:
            pstCscTmp = &g_stCSC_RGB2YUV709_tv;
            break;
        default:
            return HI_FAILURE;
    }

    pstCstCoef->csc_in_dc0  = pstCscTmp->csc_in_dc0;
    pstCstCoef->csc_in_dc1  = pstCscTmp->csc_in_dc1;
    pstCstCoef->csc_in_dc2  = pstCscTmp->csc_in_dc2;
    pstCstCoef->csc_out_dc0 = pstCscTmp->csc_out_dc0;
    pstCstCoef->csc_out_dc1 = pstCscTmp->csc_out_dc1;
    pstCstCoef->csc_out_dc2 = pstCscTmp->csc_out_dc2;

    /* C_ratio normally is 0~1.99, C_ratio=s32Contrast/100
    *  S normally is 0~1.99,S=s32Satu/100
    *  Hue -30~30, using the lut to get COS and SIN and then /1000
    */
    if ((HAL_CSC_MODE_BT601_TO_RGB_PC == enCscMode) || (HAL_CSC_MODE_BT709_TO_RGB_PC == enCscMode)
        || (HAL_CSC_MODE_BT601_TO_RGB_TV == enCscMode) || (HAL_CSC_MODE_BT709_TO_RGB_TV == enCscMode))
    {
        /* YUV->RGB convert, RGB->YUV convert */
        pstCstCoef->csc_coef00 = (s32Contrast * pstCscTmp->csc_coef00) / 100;
        pstCstCoef->csc_coef01 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef01 * HIFB_COS_TABLE[s32Hue] - pstCscTmp->csc_coef02 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef02 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef01 * HIFB_SIN_TABLE[s32Hue] + pstCscTmp->csc_coef02 * HIFB_COS_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef10 = (s32Contrast * pstCscTmp->csc_coef10) / 100;
        pstCstCoef->csc_coef11 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11 * HIFB_COS_TABLE[s32Hue] - pstCscTmp->csc_coef12 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef12 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11 * HIFB_SIN_TABLE[s32Hue] + pstCscTmp->csc_coef12 * HIFB_COS_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef20 = (s32Contrast * pstCscTmp->csc_coef20) / 100;
        pstCstCoef->csc_coef21 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21 * HIFB_COS_TABLE[s32Hue] - pstCscTmp->csc_coef22 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef22 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21 * HIFB_SIN_TABLE[s32Hue] + pstCscTmp->csc_coef22 * HIFB_COS_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_in_dc0 += (0 != s32Contrast) ? (s32Luma * 100 / s32Contrast) : s32Luma * 100;
    }
    else
    {
        /* RGB->YUV convert, YUV->RGB convert
        *  YUV->YUV */

        pstCstCoef->csc_coef00 = (s32Contrast * pstCscTmp->csc_coef00) / 100;
        pstCstCoef->csc_coef01 = (s32Contrast * pstCscTmp->csc_coef01) / 100;
        pstCstCoef->csc_coef02 = (s32Contrast * pstCscTmp->csc_coef02) / 100;
        pstCstCoef->csc_coef10 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef10 * HIFB_COS_TABLE[s32Hue] + pstCscTmp->csc_coef20 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef11 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11 * HIFB_COS_TABLE[s32Hue] + pstCscTmp->csc_coef21 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef12 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef12 * HIFB_COS_TABLE[s32Hue] + pstCscTmp->csc_coef22 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef20 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef20 * HIFB_COS_TABLE[s32Hue] - pstCscTmp->csc_coef10 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef21 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21 * HIFB_COS_TABLE[s32Hue] - pstCscTmp->csc_coef11 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef22 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef22 * HIFB_COS_TABLE[s32Hue] - pstCscTmp->csc_coef12 * HIFB_SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_out_dc0 += s32Luma;
    }
    return HI_SUCCESS;
}


HI_BOOL GRAPHIC_DRV_GetDevEnable(VO_DEV VoDev, HI_BOOL *pbIntfEn)
{
    if (HI_FALSE == HAL_DISP_GetIntfEnable(VoDev, pbIntfEn))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetIntfSync(VO_DEV VoDev,HAL_DISP_SYNCINFO_S *pstSyncInfo)
{
    if (HI_FALSE == HAL_DISP_GetIntfSync(VoDev, pstSyncInfo))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL GRAPHIC_DRV_GetIntfMuxSel(VO_DEV VoDev,VO_INTF_TYPE_E *pbenIntfType)
{
    if (HI_FALSE == HAL_DISP_GetIntfMuxSel(VoDev, pbenIntfType))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}


HI_S32 GRAPHIC_DRV_AllocateMem(HI_U32 u32Size, HIFB_MMZ_BUFFER_S* stVdpMmzBuffer)
{

#if 1
    HI_S32     nRet;

    HI_VOID*   pMmzName = HI_NULL;
    hi_mpp_chn  stChn;

    stChn.mod_id = HI_ID_FB;
    stChn.dev_id = 0;
    stChn.chn_id = 0;

    if (FUNC_ENTRY(sys_export_func, HI_ID_SYS)->pfn_get_mmz_name(&stChn,&pMmzName)){
        HIFB_GRAPHICS_TRACE(HI_DBG_ERR,"get mmz name fail!\n");
        return HI_FAILURE;
    }

    nRet = cmpi_mmz_malloc_nocache(pMmzName, "HIFB COEF", &stVdpMmzBuffer->u64StartPhyAddr,
                                 &stVdpMmzBuffer->pStartVirAddr, u32Size);

    if (0 != nRet)
    {
        HIFB_GRAPHICS_TRACE(HI_DBG_ERR,"HIFB DDR CFG  failed\n");
        return HI_FAILURE;
    }
    osal_memset(stVdpMmzBuffer->pStartVirAddr,0,u32Size);
#else
    stVdpMmzBuffer->u32StartVirAddr = 0x8000000;
    stVdpMmzBuffer->u32StartPhyAddr = 0x8000000;
#endif

    return HI_SUCCESS;
}


HI_S32 GRAPHIC_DRV_DeleteMem(HIFB_MMZ_BUFFER_S* stVdpMmzBuffer)
{
#if 1
    if(HI_NULL != stVdpMmzBuffer)
    {
        cmpi_mmz_free(stVdpMmzBuffer->u64StartPhyAddr, stVdpMmzBuffer->pStartVirAddr);
        stVdpMmzBuffer->u64StartPhyAddr = 0;
        stVdpMmzBuffer->pStartVirAddr   = NULL;
    }
#endif
    return HI_SUCCESS;
}

HI_S32 GRAPHIC_DRV_VhdCoefBufAddrDistribute(HIFB_COEF_ADDR_S* stVdpCoefBufAddr)
{
    stVdpCoefBufAddr->pu8CoefVirAddr[HIFB_COEF_BUF_G0ZME]  = stVdpCoefBufAddr->stBufBaseAddr.pStartVirAddr;
    stVdpCoefBufAddr->u64CoefPhyAddr[HIFB_COEF_BUF_G0ZME]  = stVdpCoefBufAddr->stBufBaseAddr.u64StartPhyAddr;

    HAL_PARA_SetParaAddrVhdChn06(stVdpCoefBufAddr->u64CoefPhyAddr[HIFB_COEF_BUF_G0ZME]);

    return HI_SUCCESS;
}


HI_VOID GRAPHIC_ZME_COEF_INIT(HI_VOID)
{
    /* hifb only need zme coef size */
    GRAPHIC_DRV_AllocateMem(COEF_SIZE_G0ZME, &g_stHifbCoefBufAddr.stBufBaseAddr);
    GRAPHIC_DRV_VhdCoefBufAddrDistribute(&g_stHifbCoefBufAddr);
}



HI_S32 GRAPHIC_DRV_Init(HI_VOID)
{
    HI_S32                 i           = 0;
    VO_GFXLAYER_CONTEXT_S* pstVoGfxCtx = NULL;

    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM /*1*/; ++i)
    {
        pstVoGfxCtx = &s_astGfxLayerCtx[i];

        pstVoGfxCtx->enLayerId     = HAL_DISP_LAYER_GFX0+i; /* HAL_DISP_LAYER_GFX0+1 */
        pstVoGfxCtx->s32BindedDev  = i;                     /* 0 */
        pstVoGfxCtx->bBinded       = HI_TRUE;
    }


    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i)
    {
        pstVoGfxCtx = &s_astGfxLayerCtx[i];
        pstVoGfxCtx->stGfxCsc.enCscMatrix  = VO_CSC_MATRIX_RGB_TO_BT601_TV;
        pstVoGfxCtx->stGfxCsc.u32Luma      = 50;
        pstVoGfxCtx->stGfxCsc.u32Contrast  = 50;
        pstVoGfxCtx->stGfxCsc.u32Hue       = 50;
        pstVoGfxCtx->stGfxCsc.u32Satuature = 50;

        //CSC extra coef
        pstVoGfxCtx->stCscCoefParam.csc_scale2p  = GFX_CSC_SCALE;
        pstVoGfxCtx->stCscCoefParam.csc_clip_min = GFX_CSC_CLIP_MIN;
        pstVoGfxCtx->stCscCoefParam.csc_clip_max = GFX_CSC_CLIP_MAX;
    }

    /* DDR detect coef */
    s_astGfxLayerCtx[0].u32StartSection = 0;
    s_astGfxLayerCtx[0].u32ZoneNums = 16;

    return HI_SUCCESS;
}

HI_S32 GRAPHIC_DRV_GetBindDev(HI_S32 s32LayaerId)
{
    return s_astGfxLayerCtx[s32LayaerId].s32BindedDev;
}

HI_S32 GRAPHIC_DRV_Exit(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 GRAPHIC_DRV_Resource_Init(HI_VOID)
{
    HI_S32 i = 0;

    HAL_VOU_Init();

    /* lock init */
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i)
    {
        osal_memset(&s_astGfxLayerCtx[i], 0, sizeof(VO_GFXLAYER_CONTEXT_S));
        GFX_SPIN_LOCK_INIT(&s_astGfxLayerCtx[i].spinLock);
    }

    /* mem alloc */
    GRAPHIC_ZME_COEF_INIT();

    return HI_SUCCESS;
}
HI_S32 GRAPHIC_DRV_Resource_Exit(HI_VOID)
{
    HI_S32                 i           = 0;
    VO_GFXLAYER_CONTEXT_S* pstVoGfxCtx = NULL;

    /* lock deinit */
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i)
    {
        pstVoGfxCtx = &s_astGfxLayerCtx[i];
        GFX_SPIN_LOCK_DEINIT(&pstVoGfxCtx->spinLock);
    }

    /* mem delete */
    GRAPHIC_DRV_DeleteMem(&g_stHifbCoefBufAddr.stBufBaseAddr);
    HAL_VOU_Exit();

    return HI_SUCCESS;
}

HI_S32 GRAPHIC_DRV_EnableLayer(HAL_DISP_LAYER_E gfxLayer, HI_BOOL bEnable)
{
    if( HI_FALSE == HAL_LAYER_EnableLayer(gfxLayer, bEnable))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "graphics layer %d Enable failed!\n", gfxLayer);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_S32 GRAPHIC_DRV_SetCscCoef(HAL_DISP_LAYER_E gfxLayer, VO_CSC_S *pstGfxCsc, CscCoefParam_S * pstCscCoefParam)
{
    CscCoef_S          stCscCoef;
    HAL_CSC_MODE_E     enCscMode;

    HI_U32 u32Pre = 8;
    HI_U32 u32DcPre = 4;

    HI_U32 u32LayerIndex;

    if (HI_SUCCESS != GRAPHIC_DRV_GetLayerIndex(gfxLayer, &u32LayerIndex))
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "gfxLayer(%u) is invalid!\n", (HI_U32)gfxLayer);
        return HI_FALSE;
    }

    if (VO_CSC_MATRIX_RGB_TO_BT601_PC == pstGfxCsc->enCscMatrix)
    {
        enCscMode = HAL_CSC_MODE_RGB_TO_BT601_PC;
    }
    else if (VO_CSC_MATRIX_RGB_TO_BT709_PC == pstGfxCsc->enCscMatrix)
    {
        enCscMode = HAL_CSC_MODE_RGB_TO_BT709_PC;
    }
    else if (VO_CSC_MATRIX_RGB_TO_BT601_TV == pstGfxCsc->enCscMatrix)
    {
        enCscMode = HAL_CSC_MODE_RGB_TO_BT601_TV;
    }
    else if (VO_CSC_MATRIX_RGB_TO_BT709_TV == pstGfxCsc->enCscMatrix)
    {
        enCscMode = HAL_CSC_MODE_RGB_TO_BT709_TV;
    }
    //else if (VO_CSC_MATRIX_IDENTITY == pstGfxCsc->enCscMatrix )
    //{
    //    enCscMode = HAL_CSC_MODE_RGB_TO_RGB;
    //}
    else
    {
        GRAPHICS_DRV_TRACE(HI_DBG_ERR, "enCscMatrix %d err, should only be RGB to YUV matrix\n", pstGfxCsc->enCscMatrix);
        return HI_ERR_VO_ILLEGAL_PARAM;
    }

    //cal CSC coef and CSC dc coef
    VOU_DRV_CalcCscMatrix(pstGfxCsc->u32Luma, pstGfxCsc->u32Contrast,
        pstGfxCsc->u32Hue, pstGfxCsc->u32Satuature, enCscMode, &stCscCoef);

    stCscCoef.new_csc_clip_max = GFX_CSC_CLIP_MAX;
    stCscCoef.new_csc_clip_min = GFX_CSC_CLIP_MIN;
    stCscCoef.new_csc_scale2p  = GFX_CSC_SCALE;

    stCscCoef.csc_coef00 =(HI_S32)u32Pre*stCscCoef.csc_coef00 * 1024 / 1000;
    stCscCoef.csc_coef01 =(HI_S32)u32Pre*stCscCoef.csc_coef01 * 1024 / 1000;
    stCscCoef.csc_coef02 =(HI_S32)u32Pre*stCscCoef.csc_coef02 * 1024 / 1000;
    stCscCoef.csc_coef10 =(HI_S32)u32Pre*stCscCoef.csc_coef10 * 1024 / 1000;
    stCscCoef.csc_coef11 =(HI_S32)u32Pre*stCscCoef.csc_coef11 * 1024 / 1000;
    stCscCoef.csc_coef12 =(HI_S32)u32Pre*stCscCoef.csc_coef12 * 1024 / 1000;
    stCscCoef.csc_coef20 =(HI_S32)u32Pre*stCscCoef.csc_coef20 * 1024 / 1000;
    stCscCoef.csc_coef21 =(HI_S32)u32Pre*stCscCoef.csc_coef21 * 1024 / 1000;
    stCscCoef.csc_coef22 =(HI_S32)u32Pre*stCscCoef.csc_coef22 * 1024 / 1000;

    stCscCoef.csc_in_dc0  = (HI_S32)u32DcPre*stCscCoef.csc_in_dc0;
    stCscCoef.csc_in_dc1  = (HI_S32)u32DcPre*stCscCoef.csc_in_dc1;
    stCscCoef.csc_in_dc2  = (HI_S32)u32DcPre*stCscCoef.csc_in_dc2;

    stCscCoef.csc_out_dc0 = (HI_S32)u32DcPre*stCscCoef.csc_out_dc0;
    stCscCoef.csc_out_dc1 = (HI_S32)u32DcPre*stCscCoef.csc_out_dc1;
    stCscCoef.csc_out_dc2 = (HI_S32)u32DcPre*stCscCoef.csc_out_dc2;


    //set CSC coef and CSC dc coef
    HAL_LAYER_SetCscCoef(gfxLayer, &stCscCoef);

    // copy to drv
    osal_memcpy(&s_astGfxLayerCtx[u32LayerIndex].stGfxCsc, pstGfxCsc, sizeof(VO_CSC_S));
    osal_memcpy(&s_astGfxLayerCtx[u32LayerIndex].stCscCoefParam, pstCscCoefParam, sizeof(CscCoefParam_S));

    return HI_SUCCESS;
}


HI_S32    GRAPHIC_DRV_ShowProc(osal_proc_entry_t *s)
{
    return HI_SUCCESS;
}

/********************************************************************************
* [Begin]  Graphic layer ZME functions
********************************************************************************/
HI_VOID GF_FUNC_SetG0ZmeMode(HI_U32 enLayer , GF_G0_ZME_MODE_E G0ZmeMode, GF_G0_ZME_CFG_S * pstCfg)
{
    //filed declare
    HI_U32 ck_gt_en      =  pstCfg->ck_gt_en;

    HI_U32 out_pro       =  pstCfg->out_pro;
    HI_U32 hfir_order    =  1;

    HI_U32 hfir_en          =  pstCfg->hfir_en   ;
    HI_U32 vfir_en          =  pstCfg->vfir_en   ;
    HI_U32 out_width        =  pstCfg->out_width ;
    HI_U32 out_height       =  pstCfg->out_height;

    HI_U32 ahfir_mid_en     =  pstCfg->ahmid_en  ;
    HI_U32 lhfir_mid_en     =  pstCfg->lhmid_en  ;
    HI_U32 chfir_mid_en     =  pstCfg->lhmid_en  ;
    HI_U32 lhfir_mode       =  pstCfg->lhfir_mode;
    HI_U32 ahfir_mode       =  pstCfg->ahfir_mode;
    HI_S32 lhfir_offset     =  0;
    HI_S32 chfir_offset     =  0;
    HI_U32 avfir_mid_en     =  pstCfg->avmid_en  ;
    HI_U32 lvfir_mid_en     =  pstCfg->lvmid_en  ;
    HI_U32 cvfir_mid_en     =  pstCfg->lvmid_en  ;
    HI_U32 lvfir_mode       =  pstCfg->lvfir_mode;
    HI_U32 avfir_mode       =  pstCfg->avfir_mode;
    HI_S32 vtp_offset       =  0;
    HI_S32 vbtm_offset      =  0;

    HI_UL  zme_hprec        =  (1<<20);
    HI_UL  zme_vprec        =  (1<<12);

    HI_U32 hratio           =  (pstCfg->in_width *zme_hprec)/pstCfg->out_width;
    HI_U32 vratio           =  (pstCfg->in_height*zme_vprec)/pstCfg->out_height;

    if(G0ZmeMode == VDP_G0_ZME_TYP)
    {
        //typ mode
        lhfir_offset = 0;
        chfir_offset = 0;
        vtp_offset   = 0;
        vbtm_offset  = (-1) * (HI_S64)zme_vprec/2;
    }

    //drv transfer
    HAL_G0_ZME_SetCkGtEn( ck_gt_en);
    HAL_G0_ZME_SetOutWidth( out_width);
    HAL_G0_ZME_SetHfirEn( hfir_en);
    HAL_G0_ZME_SetAhfirMidEn( ahfir_mid_en);
    HAL_G0_ZME_SetLhfirMidEn( lhfir_mid_en);
    HAL_G0_ZME_SetChfirMidEn( chfir_mid_en);
    HAL_G0_ZME_SetLhfirMode( lhfir_mode);
    HAL_G0_ZME_SetAhfirMode( ahfir_mode);
    HAL_G0_ZME_SetHfirOrder( hfir_order);
    HAL_G0_ZME_SetHratio( hratio);
    HAL_G0_ZME_SetLhfirOffset( lhfir_offset);
    HAL_G0_ZME_SetChfirOffset( chfir_offset);
    HAL_G0_ZME_SetOutPro( out_pro);
    HAL_G0_ZME_SetOutHeight( out_height);
    HAL_G0_ZME_SetVfirEn( vfir_en);
    HAL_G0_ZME_SetAvfirMidEn( avfir_mid_en);
    HAL_G0_ZME_SetLvfirMidEn( lvfir_mid_en);
    HAL_G0_ZME_SetCvfirMidEn( cvfir_mid_en);
    HAL_G0_ZME_SetLvfirMode( lvfir_mode);
    HAL_G0_ZME_SetVafirMode( avfir_mode);
    HAL_G0_ZME_SetVratio( vratio);
    HAL_G0_ZME_SetVtpOffset( vtp_offset);
    HAL_G0_ZME_SetVbtmOffset( vbtm_offset);

}


HI_U32 VO_DRV_WriteDDR(HI_U8* addr, HIFB_DRV_U128_S* pstData128 )
{
    HI_U32 ii = 0;
    HI_U32 u32DataArr[4] = {pstData128->data0, pstData128->data1, pstData128->data2, pstData128->data3};
    HI_U8* pu8AddrTmp = 0;

    HI_U32     uDataTemp = 0;

    for (ii = 0; ii < 4; ii++)
    {
        pu8AddrTmp = addr + ii * 4;
        uDataTemp = u32DataArr[ii];
        *(HI_U32*)pu8AddrTmp  = uDataTemp;
    }

    return 0;
}

HI_S32 VO_DRV_Push128(HIFB_DRV_U128_S* pstData128, HI_U32 coef_data, HI_U32 bit_len)
{

    coef_data = coef_data & (0xFFFFFFFF >> (32 - bit_len));

    if (pstData128->depth < 32)
    {
        if ((pstData128->depth + bit_len) <= 32)
        {
            pstData128->data0 = (coef_data << pstData128->depth) | pstData128->data0;
        }
        else
        {
            pstData128->data0 = (coef_data << pstData128->depth) | pstData128->data0;
            pstData128->data1 = coef_data >> (32 - pstData128->depth % 32);
        }
    }
    else if (pstData128->depth >= 32 && pstData128->depth < 64)
    {
        if ((pstData128->depth + bit_len) <= 64)
        {
            pstData128->data1 = (coef_data << pstData128->depth % 32) | pstData128->data1;
        }
        else
        {
            pstData128->data1 = (coef_data << pstData128->depth % 32) | pstData128->data1;
            pstData128->data2 = coef_data >> (32 - pstData128->depth % 32);
        }
    }
    else if (pstData128->depth >= 64 && pstData128->depth < 96)
    {
        if ((pstData128->depth + bit_len) <= 96)
        {
            pstData128->data2 = (coef_data << pstData128->depth % 32) | pstData128->data2;
        }
        else
        {
            pstData128->data2 = (coef_data << pstData128->depth % 32) | pstData128->data2;
            pstData128->data3 = coef_data >> (32 - pstData128->depth % 32);
        }
    }
    else if (pstData128->depth >= 96)
    {
        if ((pstData128->depth + bit_len) <= 128)
        {
            pstData128->data3 = (coef_data << (pstData128->depth % 32)) | pstData128->data3;
        }
    }

    pstData128->depth = pstData128->depth + bit_len;

    if (pstData128->depth <= 128)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }

}


HI_U32 VO_DRV_FindMax(HI_U32* u32Array, HI_U32 num)
{
    HI_U32 ii;
    HI_U32 u32TmpData = u32Array[0];

    for (ii = 1; ii < num; ii++)
    {
        if (u32TmpData < u32Array[ii])
        {
            u32TmpData = u32Array[ii];
        }
    }

    return u32TmpData;
}


HI_U8* VO_DRV_SendCoef(HIFB_DRV_COEF_SEND_CFG* stCfg)
{
    HI_U32 ii, kk, nn, mm;
    HI_U8* addr_base               = stCfg->coef_addr;
    HI_U32 addr_offset             = 0;
    HI_U8* addr                    = addr_base;

    HI_U32 cycle_num ;
    HI_U32 total_bit_len           = 0;

    HIFB_DRV_U128_S stData128        = {0};
    HI_U32 coef_cnt                = 0;

    HI_S32 tmp_data                = 0;
    HI_U32 total_burst_num         = 0;
    HI_U32 max_len                 = 0;

    addr_base  = stCfg->coef_addr;

    //----------------------------------------------------------------------
    //data type convertion
    //----------------------------------------------------------------------

    addr = addr_base;

    cycle_num = stCfg->cycle_num ;

    for (ii = 0; ii < stCfg->lut_num; ii++)
    {
        total_bit_len = total_bit_len + stCfg->coef_bit_length[ii];
    }

    //send data
    max_len = VO_DRV_FindMax( stCfg->lut_length , stCfg->lut_num) ;
    if (stCfg->burst_num == 1)
    {
        total_burst_num = (max_len + cycle_num - 1) / cycle_num;

        if (stCfg->data_type == DRV_COEF_DATA_TYPE_S16)
        {
            for (kk = 0; kk < total_burst_num; kk++)
            {
                osal_memset((void*)&stData128, 0 , sizeof(stData128));

                for (mm = 0 ; mm < cycle_num ; mm++)
                {
                    coef_cnt = kk * cycle_num + mm ;

                    for (nn = 0; nn < stCfg->lut_num ; nn++)
                    {
                        if (coef_cnt < stCfg->lut_length[nn])
                        {
                            tmp_data = ((HI_S16**)stCfg->p_coef_array)[nn][coef_cnt];
                        }
                        else
                        {
                            tmp_data = 0;
                        }

                        VO_DRV_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                    }
                }

                addr = addr_base + addr_offset;
                addr_offset = addr_offset + 16;
                VO_DRV_WriteDDR(addr, &stData128);
            }
        }
        else if (stCfg->data_type == DRV_COEF_DATA_TYPE_U16)
        {
            for (kk = 0; kk < total_burst_num; kk++)
            {
                osal_memset((void*)&stData128, 0 , sizeof(stData128));

                for (mm = 0 ; mm < cycle_num ; mm++)
                {
                    coef_cnt = kk * cycle_num + mm ;

                    for (nn = 0; nn < stCfg->lut_num ; nn++)
                    {
                        if (coef_cnt < stCfg->lut_length[nn])
                        {
                            tmp_data = ((HI_U16**)stCfg->p_coef_array)[nn][coef_cnt];
                        }
                        else
                        {
                            tmp_data = 0;
                        }

                        VO_DRV_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                    }
                }

                addr = addr_base + addr_offset;
                addr_offset = addr_offset + 16;
                VO_DRV_WriteDDR(addr, &stData128);
            }
        }
        else if (stCfg->data_type == DRV_COEF_DATA_TYPE_U32)
        {
            for (kk = 0; kk < total_burst_num; kk++)
            {
                osal_memset((void*)&stData128, 0 , sizeof(stData128));

                for (mm = 0 ; mm < cycle_num ; mm++)
                {
                    coef_cnt = kk * cycle_num + mm ;

                    for (nn = 0; nn < stCfg->lut_num ; nn++)
                    {
                        if (coef_cnt < stCfg->lut_length[nn])
                        {
                            tmp_data = ((HI_U32**)stCfg->p_coef_array)[nn][coef_cnt];
                        }
                        else
                        {
                            tmp_data = 0;
                        }

                        VO_DRV_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                    }
                }

                addr = addr_base + addr_offset;
                addr_offset = addr_offset + 16;
                VO_DRV_WriteDDR(addr, &stData128);
            }
        }
        else if (stCfg->data_type == DRV_COEF_DATA_TYPE_S32)
        {
            for (kk = 0; kk < total_burst_num; kk++)
            {
                osal_memset((void*)&stData128, 0 , sizeof(stData128));

                for (mm = 0 ; mm < cycle_num ; mm++)
                {
                    coef_cnt = kk * cycle_num + mm ;

                    for (nn = 0; nn < stCfg->lut_num ; nn++)
                    {
                        if (coef_cnt < stCfg->lut_length[nn])
                        {
                            tmp_data = ((HI_S32**)stCfg->p_coef_array)[nn][coef_cnt];
                        }
                        else
                        {
                            tmp_data = 0;
                        }

                        VO_DRV_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                    }
                }

                addr = addr_base + addr_offset;
                addr_offset = addr_offset + 16;
                VO_DRV_WriteDDR(addr, &stData128);
            }
        }
        else if (stCfg->data_type == DRV_COEF_DATA_TYPE_S8)
        {
            for (kk = 0; kk < total_burst_num; kk++)
            {
                osal_memset((void*)&stData128, 0 , sizeof(stData128));

                for (mm = 0 ; mm < cycle_num ; mm++)
                {
                    coef_cnt = kk * cycle_num + mm ;

                    for (nn = 0; nn < stCfg->lut_num ; nn++)
                    {
                        if (coef_cnt < stCfg->lut_length[nn])
                        {
                            tmp_data = ((HI_S8**)stCfg->p_coef_array)[nn][coef_cnt];
                        }
                        else
                        {
                            tmp_data = 0;
                        }

                        VO_DRV_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                    }
                }

                addr = addr_base + addr_offset;
                addr_offset = addr_offset + 16;
                VO_DRV_WriteDDR(addr, &stData128);
            }
        }
        else if (stCfg->data_type == DRV_COEF_DATA_TYPE_U8)
        {
            for (kk = 0; kk < total_burst_num; kk++)
            {
                osal_memset((void*)&stData128, 0 , sizeof(stData128));

                for (mm = 0 ; mm < cycle_num ; mm++)
                {
                    coef_cnt = kk * cycle_num + mm ;

                    for (nn = 0; nn < stCfg->lut_num ; nn++)
                    {
                        if (coef_cnt < stCfg->lut_length[nn])
                        {
                            tmp_data = ((HI_U8**)stCfg->p_coef_array)[nn][coef_cnt];
                        }
                        else
                        {
                            tmp_data = 0;
                        }

                        VO_DRV_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                    }
                }

                addr = addr_base + addr_offset;
                addr_offset = addr_offset + 16;
                VO_DRV_WriteDDR(addr, &stData128);
            }
        }

    }

    return (addr_base + addr_offset);

}


HI_S32 GF_DRV_SetG0zmeCoef(HI_S16 * pcoef_h,HI_S16 * pcoef_v)
{
    HIFB_DRV_COEF_SEND_CFG   stCoefSend;
    HI_U8*                 addr = 0;

    void*                   p_coef_array[1]    = {pcoef_h};
    HI_U32                  lut_length[1]      = {64};
    HI_U32                  coef_bit_length[1] = {16};

    addr = g_stHifbCoefBufAddr.pu8CoefVirAddr[HIFB_COEF_BUF_G0ZME];

    stCoefSend.coef_addr         = addr;
    stCoefSend.lut_num           = 1;
    stCoefSend.burst_num         = 1;
    stCoefSend.cycle_num         = 8;
    stCoefSend.p_coef_array      = p_coef_array;
    stCoefSend.lut_length        = lut_length;
    stCoefSend.coef_bit_length   = coef_bit_length;
    stCoefSend.data_type         = DRV_COEF_DATA_TYPE_S16;

    addr = VO_DRV_SendCoef(&stCoefSend);
    p_coef_array[0]    = pcoef_v;
    lut_length[0]      = 128;
    coef_bit_length[0] = 16;

    stCoefSend.coef_addr         = addr;
    stCoefSend.cycle_num         = 8;
    stCoefSend.p_coef_array      = p_coef_array;
    stCoefSend.lut_length        = lut_length;
    stCoefSend.coef_bit_length   = coef_bit_length;
    stCoefSend.data_type         = DRV_COEF_DATA_TYPE_S16;

    VO_DRV_SendCoef(&stCoefSend);

    return HI_SUCCESS;
}

HI_S16 coef_v[16][8] =
{
    {0, 0, 0, 0, 0,     63,  0,  0},
    {0, 0, 0, 0, 6,     51, 13, -6},
    {0, 0, 0, 0, 4,     51, 16, -7},
    {0, 0, 0, 0, 1,     50, 20, -7},
    {0, 0, 0, 0, -1, 49, 24, -8},
    {0, 0, 0, 0, -3, 47, 28, -8},
    {0, 0, 0, 0, -4, 45, 31, -8},
    {0, 0, 0, 0, -6, 42, 35, -7},
    {0, 0, 0, 0, -7, 39, 39, -7},
    {0, 0, 0, 0, -7, 35, 42, -6},
    {0, 0, 0, 0, -8, 31, 45, -4},
    {0, 0, 0, 0, -8, 28, 47, -3},
    {0, 0, 0, 0, -8, 24, 49, -1},
    {0, 0, 0, 0, -7, 20, 50,  1},
    {0, 0, 0, 0, -7, 16, 51,  4},
    {0, 0, 0, 0, -6, 13, 51,  6}
};

HI_S16 coef_h[8][8] =
{
    {0, 0,     0,     0,    63,     0,     0,    0},
    {0, 0,     -4,     4,    52,    16,    -6,    2},
    {0, 0,     -2,     -1,    48,    24,    -7,    2},
    {0, 0,     -1,     -4,    44,    31,    -7,    1},
    {0, 0,     1,     -7,    38,    38,    -7,    1},
    {0, 0,     1,     -7,    31,    44,    -4, -1},
    {0, 0,     2,     -7,    24,    48,    -1, -2},
    {0, 0,     2,     -6,    16,    52,     4, -4}
};


HI_S16 coef_h_New[64];
HI_S16 coef_v_New[128];

HI_VOID GF_vSetG0zmeCoef(GF_RM_COEF_MODE_E enCoefMode)
{
    HI_U32 ii = 0;

    if(enCoefMode == GF_RM_COEF_MODE_TYP)
    {

        for(ii = 0; ii < 8; ii++)
        {
            coef_h_New[ii*8+0] = coef_h[ii][7];
            coef_h_New[ii*8+1] = coef_h[ii][6];
            coef_h_New[ii*8+2] = coef_h[ii][5];
            coef_h_New[ii*8+3] = coef_h[ii][4];
            coef_h_New[ii*8+4] = coef_h[ii][3];
            coef_h_New[ii*8+5] = coef_h[ii][2];
            coef_h_New[ii*8+6] = coef_h[ii][1];
            coef_h_New[ii*8+7] = coef_h[ii][0];
        }

        for(ii = 0; ii < 16; ii++)
        {
            coef_v_New[ii*8+0] = coef_v[ii][7];
            coef_v_New[ii*8+1] = coef_v[ii][6];
            coef_v_New[ii*8+2] = coef_v[ii][5];
            coef_v_New[ii*8+3] = coef_v[ii][4];
            coef_v_New[ii*8+4] = coef_v[ii][3];
            coef_v_New[ii*8+5] = coef_v[ii][2];
            coef_v_New[ii*8+6] = coef_v[ii][1];
            coef_v_New[ii*8+7] = coef_v[ii][0];
        }

    }

    //send coef to DDR
    GF_DRV_SetG0zmeCoef(coef_h_New,coef_v_New);
}
/********************************************************************************
* [Begin]  Graphic layer ZME coef functions
********************************************************************************/
HI_BOOL GRAPHIC_DRV_EnableZME(HI_U32 enLayer, GF_G0_ZME_CFG_S* pst_zme_cfg, HI_BOOL bEnableZme)
{
    GF_G0_ZME_CFG_S            stZmeCfg    ;

    if ((pst_zme_cfg->in_width == 0) || (pst_zme_cfg->in_height == 0) ||
        (pst_zme_cfg->out_width == 0) || (pst_zme_cfg->out_height == 0)) {
        return HI_FALSE;
    }

    stZmeCfg.ck_gt_en         =  1;
    stZmeCfg.out_pro          =  VDP_RMODE_PROGRESSIVE;

    stZmeCfg.in_width         =  pst_zme_cfg->in_width;
    stZmeCfg.in_height        =  pst_zme_cfg->in_height;
    stZmeCfg.out_width        =  pst_zme_cfg->out_width;
    stZmeCfg.out_height       =  pst_zme_cfg->out_height;
    stZmeCfg.lhmid_en         =  1;
    stZmeCfg.ahmid_en         =  1;
    stZmeCfg.lhfir_mode       =  1;
    stZmeCfg.ahfir_mode       =  1;
    stZmeCfg.lvmid_en         =  1;
    stZmeCfg.avmid_en         =  1;
    stZmeCfg.lvfir_mode       =  1;
    stZmeCfg.avfir_mode       =  1;


    if(bEnableZme)
    {
        stZmeCfg.hfir_en          =  1;
        stZmeCfg.vfir_en          =  1;

        GF_FUNC_SetG0ZmeMode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &stZmeCfg);

        /* It will be reset when VO exit, so config again */
        GRAPHIC_DRV_VhdCoefBufAddrDistribute(&g_stHifbCoefBufAddr);
        GF_vSetG0zmeCoef(GF_RM_COEF_MODE_TYP);
        HAL_PARA_SetParaUpVhdChn(HIFB_COEF_BUF_G0ZME);
    }
    else
    {
        stZmeCfg.hfir_en     = 0;
        stZmeCfg.vfir_en     = 0;

        GF_FUNC_SetG0ZmeMode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &stZmeCfg);
    }

    return HI_TRUE;
}

HI_VOID GRAPHIC_DRV_DevIntEnable(VO_DEV VoDev, HI_BOOL Enable)
{
    HIFB_INT_MASK_E IntType;

    switch ( VoDev )
    {
        case VO_DEV_DHD0:
            IntType = HIFB_INTMSK_DHD0_VTTHD2;
            break;

        case VO_DEV_DHD1 :
            IntType = HIFB_INTMSK_DHD1_VTTHD2;
            break;

        default:
            return;
    }

    if (HI_TRUE == Enable)
    {
        HAL_DISP_SetIntMask(IntType);
    }
    else
    {
        HAL_DISP_ClrIntMask(IntType);
    }


    return;
}


HI_VOID GRAPHIC_DRV_IntClear(HI_U32 u32IntClear, HI_S32 s32Irq)
{
    /* vo double interrupt, read and mask use interrupt 1, but clear use interrupt 0 as video */
    HAL_DISP_ClearIntStatus(u32IntClear);
    return;
}

HI_VOID GRAPHIC_DRV_IntDisableAll(HI_VOID)
{
    HAL_DISP_ClrIntMask(HIFB_INTREPORT_ALL);

    return;
}

HI_U32 GRAPHIC_DRV_IntGetStatus(HI_VOID)
{
    return HAL_DISP_GetIntStatus(HIFB_INTREPORT_ALL);
}


HI_VOID GRAPHIC_DRV_ClrIntStatus(HI_U32 u32IntStatus)
{
    if (u32IntStatus & HIFB_INTMSK_DHD0_VTTHD2)
    {
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD0_VTTHD2, VOU1_IRQ_NR);
    }

    if (u32IntStatus & HIFB_INTMSK_DHD0_VTTHD3)
    {
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD0_VTTHD3, VOU1_IRQ_NR);
    }


    if (u32IntStatus & HIFB_INTMSK_DHD1_VTTHD2)
    {
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD1_VTTHD2, VOU1_IRQ_NR);

    }

    if (u32IntStatus & HIFB_INTMSK_DHD1_VTTHD3)
    {
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD1_VTTHD3, VOU1_IRQ_NR);

    }


    return;
}

HI_S32 GRAPHIC_DRV_GetInterruptDev(HI_U32 IntStatus, VO_DEV* pVoDev)
{
    if (IntStatus & HIFB_INTMSK_DHD0_VTTHD2)
    {
        GRAPHICS_DRV_TRACE(HI_DBG_DEBUG, "Graphic: DHD0 INTTERRUPT\n");
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD0_VTTHD2, VOU1_IRQ_NR);
        *pVoDev = VO_DEV_DHD0;
    }
    else if (IntStatus & HIFB_INTMSK_DHD0_VTTHD3)
    {
        GRAPHICS_DRV_TRACE(HI_DBG_DEBUG, "Graphic: DHD0 INTTERRUPT\n");
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD0_VTTHD3, VOU1_IRQ_NR);
        *pVoDev = VO_DEV_DHD0;
    }
    else if (IntStatus & HIFB_INTMSK_DHD1_VTTHD2)
    {
        GRAPHICS_DRV_TRACE(HI_DBG_DEBUG, "Graphic: DHD1 INTTERRUPT\n");
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD1_VTTHD2, VOU1_IRQ_NR);
        *pVoDev = VO_DEV_DHD1;
    }
    else if (IntStatus & HIFB_INTMSK_DHD1_VTTHD3)
    {
        GRAPHICS_DRV_TRACE(HI_DBG_DEBUG, "Graphic: DHD1 INTTERRUPT\n");
        GRAPHIC_DRV_IntClear(HIFB_INTMSK_DHD1_VTTHD3, VOU1_IRQ_NR);
        *pVoDev = VO_DEV_DHD1;
    }
    else
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}



