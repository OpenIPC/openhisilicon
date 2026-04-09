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
#ifndef __HIFB_DRV_H__
#define __HIFB_DRV_H__
#ifdef __HuaweiLite__
#include <fb.h>
#else
#include <linux/fb.h>
#endif
#include "hifb.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __HuaweiLite__
typedef struct
{
    struct hifb_info *pstInfo;
    HI_UL ulLayerSize;
    HI_U32 u32CurosrBufSize;    /*For soft cursor*/
} HIFB_LAYER_S;

#else
typedef struct
{
    struct fb_info *pstInfo;
    HI_UL ulLayerSize;     /*ulLayerSize = fb.smem_len, For display buf, KB*/
    HI_U32 u32CurosrBufSize;    /*For soft cursor*/
} HIFB_LAYER_S;
#endif

extern HIFB_LAYER_S g_stLayer[];

extern char* apszLayerMmzNames[];

extern char video[];

#if(HICHIP == HI3516E_V200)
typedef HI_S32 (*IntVGSCallBack)(HI_U32 call_mod_id, HI_U32 call_dev_id, HI_U32 call_chn_id,
                            HI_VOID* pJobData);
#endif

typedef HI_S32 (*IntCallBack)(const HI_VOID *pParaml, HI_VOID *pParamr);

#define HIFB_TRACE(level, fmt...)\
    do{ \
        HI_TRACE(level,HI_ID_VO,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
        HI_TRACE(level,HI_ID_VO,##fmt);\
    }while(0)

#define HI_TRACE_FB(level, fmt...)  \
    do{\
        HI_TRACE(level,HI_ID_FB,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
        HI_TRACE(level,HI_ID_FB,##fmt);\
    } while(0)



#define HIFB_FATAL(fmt...)  HI_TRACE_FB(HI_DBG_EMERG, fmt)
#define HIFB_ERROR(fmt...)  HI_TRACE_FB(HI_DBG_ERR, fmt)
#define HIFB_WARNING(fmt...)  HI_TRACE_FB(HI_DBG_WARN, fmt)
#define HIFB_INFO(fmt...)  HI_TRACE_FB(HI_DBG_INFO, fmt)

typedef HI_U32 HIFB_LAYER_ID;   /* start from 0 */

HI_BOOL HIFB_DRV_SupportRotate(void);
HI_BOOL IS_HD_LAYER(HI_U32 LayerId);
HI_BOOL IS_SD_LAYER(HI_U32 LayerId);
HI_BOOL IS_AD_LAYER(HI_U32 LayerId);
HI_BOOL IS_CURSOR_LAYER(HI_U32 LayerId);


typedef struct
{
    HI_BOOL bKeyEnable;         /**<  colorkey */
    HI_BOOL bMaskEnable;        /**<  key mask */
    HI_U32 u32Key;
    HI_U8 u8RedMask;            /**<  red mask */
    HI_U8 u8GreenMask;          /**<  green mask */
    HI_U8 u8BlueMask;           /**<  blue mask */
    HI_U8 u8Reserved;
    HI_U32 u32KeyMode;          /**<  0:In region; 1:Out region*/
    HI_U8 u8RedMax;             /**< colorkey red max */
    HI_U8 u8GreenMax;           /**< colorkey green max */
    HI_U8 u8BlueMax;            /**< colorkey blue max*/
    HI_U8 u8Reserved1;
    HI_U8 u8RedMin;             /**< colorkey red min */
    HI_U8 u8GreenMin;           /**< colorkey green min */
    HI_U8 u8BlueMin;            /**< colorkey blue min */
    HI_U8 u8Reserved2;
}HIFB_COLORKEYEX_S;


typedef struct
{
    HIFB_RECT stClipRect;
    HI_BOOL bInRegionClip;
    HI_BOOL bClip;
}HIFB_CLIP_S;

typedef struct
{
    HIFB_ALPHA_S                   stAlpha;
    HIFB_COLORKEYEX_S              stCKey;
    HIFB_CLIP_S                    stClip;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel;
    HI_BOOL                        bScale;
    HI_BOOL                        bBlock;
    HI_BOOL                        bCallBack;
    HI_BOOL                        bRegionDeflicker;
    HI_VOID *                      pParam;
    HI_U64                         u64CmapAddr;
    HI_BOOL                        bSoftCursorUpdate;
    HIFB_MIRROR_MODE_E             enMirrorMode;
    HI_BOOL                        bCompress;
}HIFB_BLIT_OPT_S;

//for graphic rotation.
typedef struct
{
    HI_BOOL              bBlock;
    HI_BOOL              bCallBack;
    HI_VOID*             pParam;
    HIFB_ROTATE_MODE_E   enRotateMode;
}HIFB_ROTATE_OPT_S;

typedef enum
{
    HIFB_SCANMODE_P,
    HIFB_SCANMODE_I,
    HIFB_SCANMODE_BUTT,
}HIFB_SCAN_MODE_E;

/*layer state*/
typedef enum
{
    HIFB_LAYER_STATE_ENABLE = 0x0,

    HIFB_LAYER_STATE_DISABLE,

    HIFB_LAYER_STATE_INVALID,

    HIFB_LAYER_STATE_BUTT
} HIFB_LAYER_STATE_E;

typedef struct
{
    HIFB_SCAN_MODE_E enScanMode;
    HI_U32 u32ScreenWidth;
    HI_U32 u32ScreenHeight;
}DISP_INFO_S;

typedef struct
{
    HI_U64  u64BufferPhyAddr;
    HI_U32  u32Stride;
    HI_BOOL bFeildUpdate;  /* field update mode or not */
    HIFB_SCAN_MODE_E eScanMode;
    HI_U32 u32ScreenWidth; /* current sync width */
    HI_U32 u32ScreenHeight; /*current sync height*/
}HIFB_OSD_DATA_S;

/*mask bit*/
typedef enum
{
    HIFB_LAYER_PARAMODIFY_FMT              = 0x1,   /*color format*/
    HIFB_LAYER_PARAMODIFY_STRIDE           = 0x2,   /*stride, line spacing*/
    HIFB_LAYER_PARAMODIFY_ALPHA            = 0x4,   /*alpha*/
    HIFB_LAYER_PARAMODIFY_COLORKEY         = 0x8,   /*colorkey*/
    HIFB_LAYER_PARAMODIFY_INRECT           = 0x10,  /*input rect*/
    HIFB_LAYER_PARAMODIFY_OUTRECT          = 0x20,  /*output rect*/
    HIFB_LAYER_PARAMODIFY_DISPLAYADDR      = 0x40,  /*display addr*/
    HIFB_LAYER_PARAMODIFY_SHOW             = 0x80,  /*show or hide*/
    HIFB_LAYER_PARAMODIFY_BMUL             = 0x100, /*be pre-multi data*/
    HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL = 0x200, /*ANTIFLICKERLEVEL*/
    HIFB_LAYER_PARAMODIFY_DYNAMICRANGE     = 0x400, /* DYNAMICRANGE */
    HIFB_LAYER_PARAMODIFY_BUTT
}HIFB_LAYER_PARAMODIFY_MASKBIT_E;

typedef  struct
{
    HI_U32           u32PhyAddr;
    HIFB_COLOR_FMT_E enColorFmt;
    HIFB_SIZE_S      stResolution;
    HI_U32           u32Stride;
}HIFB_VCMP_INBUFINFO_S;

typedef struct
{
    HI_U32 u32BankWidth;
    HI_U32 u32Stride;
    HI_U32 u32AlphaAddr;
    HI_U32 u32RedAddr;
    HI_U32 u32GreenAddr;
    HI_U32 u32BlueAddr;
}HIFB_VCMP_OUTBUFINFO_S;

typedef enum tagVO_DISP_PIXEL_FORMAT_E
{
    VO_INPUTFMT_ARGB_4444                = 0x48,
    VO_INPUTFMT_ARGB_1555                = 0x49,
    VO_INPUTFMT_ARGB_8888                = 0x68,

    VO_DISP_PIXELFORMAT_BUTT

}VO_DISP_PIXEL_FORMAT_E;

typedef struct
{
    VO_DISP_PIXEL_FORMAT_E enPixelFmt;
    HI_U64  u64AR_PhyAddr;
    HI_U64  u64GB_PhyAddr;
    HI_U32  u32Frame_Size0;
    HI_U32  u32Frame_Size1;
    HI_U32  u32Width;
    HI_U32  u32Height;
    HI_BOOL IsLosslessA;
    HI_BOOL IsLossless;
    HI_U32  u32Offset;
    HI_U32  u32Stride;
}VO_GRAPHIC_DCMP_INFO_S;

typedef enum
{
    VO_HIFB_INTTYPE_VO,
    VO_HIFB_INTTYPE_VO_DISP,
    VO_HIFB_INTTYPE_WBC,
    VO_HIFB_INTTYPE_BUTT,
}VO_FB_INT_TYPE_E;

typedef struct hiVO_RECT_S
{
    HI_S32 x;
    HI_S32 y;
    HI_S32 w;
    HI_S32 h;
} VO_RECT_S;



typedef struct
{
    HI_U64    u64AR_PhyAddr;
    HI_U64    u64GB_PhyAddr;
    HI_U32    u32Offset;
    HI_U32    u32Stride;
    VO_RECT_S stInRect;
    VO_RECT_S stOutRect;
}VO_GRAPHIC_WBC_INFO_S;

typedef struct
{
    HI_U32 u32StartPhyAddr;
    HI_U32 u32EndPhyAddr;
}VO_MDDRC_ZONE_ADDR_S;

typedef enum
{
    HIFB_INTTYPE_VO,        /* Vertical timing interrupt */
    HIFB_INTTYPE_VO_DISP,   /* Timing switch notification */
    HIFB_INTTYPE_WBC,       /* Compression completion interrupt */
    HIFB_INTTYPE_BUTT,
}FB_INT_TPYE_E;


typedef struct {
    const char *pszChipName; /* chip name */
    HI_U32 u32LayerCount;   /* support how many layers */
    HIFB_CAPABILITY_S *pstCapability;    /* capability */

    /* init DRV*/
    HI_S32 (*HIFB_DRV_Init)(HI_VOID);

    /* De init DRV*/
    HI_S32 (*HIFB_DRV_DeInit)(HI_VOID);
    HI_S32 (*HIFB_DRV_LayerDefaultSetting)(HI_U32 u32LayerId);

    /* enable/disable layer */
    HI_S32 (*HIFB_DRV_EnableLayer)(HI_U32 u32LayerId, HI_BOOL bEnable);

    /* set layer address */
    HI_S32 (*HIFB_DRV_SetLayerAddr)(HI_U32 u32LayerId, HI_U64 u64Addr);

    /* get layer address */
    hi_s32 (*HIFB_DRV_GetLayerAddr)(hi_u32 layer_id, hi_u64 *phys_addr);

    /* set layer stride */
    HI_S32 (*HIFB_DRV_SetLayerStride)(HI_U32 u32LayerId, HI_U32 u32Stride);

    /* set layer pixel format */
    HI_S32 (*HIFB_DRV_SetLayerDataFmt)(HI_U32 u32LayerId, HIFB_COLOR_FMT_E enDataFmt);

    /* set color registers */
    HI_S32 (*HIFB_DRV_SetColorReg)(HI_U32 u32LayerId, HI_U32 u32OffSet, HI_U32 u32Color, HI_S32 UpFlag);

    /* wait until vblank, it's a block interface */
    HI_S32 (*HIFB_DRV_WaitVBlank)(HI_U32 u32LayerId);

    /* set layer deflicker */
    HI_S32 (*HIFB_DRV_SetLayerDeFlicker)(HI_U32 u32LayerId, HIFB_DEFLICKER_S stDeFlicker);

    /* set layer alpha */
    HI_S32 (*HIFB_DRV_SetLayerAlpha)(HI_U32 u32LayerId, HIFB_ALPHA_S stAlpha);

    /* set layer start position and size */
    HI_S32 (*HIFB_DRV_SetLayerRect)(HI_U32 u32LayerId, const HIFB_RECT *pstInputRect, const HIFB_RECT *pstOutputRect);

    /* set layer src image resolution */
    HI_S32 (*HIFB_DRV_SetLayerSrcImageReso)(HI_U32 u32LayerId, const HIFB_RECT *pstRect);

    /* other color format convert to RGB888 format */
#ifdef __HuaweiLite__
    HI_S32 (*HIFB_DRV_ColorConvert)(const struct hifb_info *info, HIFB_COLORKEYEX_S *pCkey);
#else
    HI_S32 (*HIFB_DRV_ColorConvert)(const struct fb_var_screeninfo *pstVar, HIFB_COLORKEYEX_S *pCkey);
#endif
    /* set layer colorkey */
    HI_S32 (*HIFB_DRV_SetLayerKeyMask)(HI_U32 u32LayerId, const HIFB_COLORKEYEX_S* pstColorkey);

    /* update layer register */
    HI_S32 (*HIFB_DRV_UpdataLayerReg)(HI_U32 u32LayerId);

    /*set premul data*/
    HI_S32 (*HIFB_DRV_SetPreMul)(HI_U32 u32LayerId, HI_BOOL bPreMul);

    HI_S32 (*HIFB_DRV_SetClutAddr)(HI_U32 u32LayerId, HI_U32 u32PhyAddr);
    /* get osd data */
    HI_S32 (*HIFB_DRV_GetOSDData)(HI_U32 u32LayerId, HIFB_OSD_DATA_S *pstLayerData);
    /* register call back function*/
    HI_S32 (*HIFB_DRV_SetIntCallback)(VO_FB_INT_TYPE_E eIntType, IntCallBack call_back,
                            HI_U32 u32LayerId, HI_VOID *pCallBackArg);
    /*open vo*/
    HI_S32 (*HIFB_DRV_OpenDisplay)(HI_VOID);

    /*close vou*/
    HI_S32 (*HIFB_DRV_CloseDisplay)(HI_VOID);

    HI_S32 (*HIFB_OpenLayer)(HI_U32 u32LayerId);
    HI_S32 (*HIFB_CloseLayer)(HI_U32 u32LayerId);

    //for compression
    HI_VOID (*HIFB_DRV_GetWbcEnState)(HI_U32 u32LayerId, HI_BOOL *pbWbcEn);
    HI_VOID (*HIFB_DRV_GetWbcStopState)(HI_U32 u32LayerId, HI_BOOL *pbWbcStop);
    HI_VOID (*HIFB_DRV_SetWbcInfo)(HI_U32 u32LayerId, VO_GRAPHIC_WBC_INFO_S *pstWbcInfo);
    HI_VOID (*HIFB_DRV_EnableWbcInt)(HI_U32 u32LayerId, HI_BOOL bEnable);
    HI_VOID (*HIFB_DRV_EnableWbc)(HI_U32 u32LayerId, HI_BOOL bEnable);
    HI_BOOL (*HIFB_DRV_EnableDcmp)(HI_U32 u32LayerId, HI_BOOL bEnable);
    HI_BOOL (*HIFB_DRV_GetDcmpEnableState)(HI_U32 u32LayerId, HI_BOOL *pbEnable);
    HI_BOOL (*HIFB_DRV_SetDcmpInfo)(HI_U32 u32LayerId, VO_GRAPHIC_DCMP_INFO_S *pstDcmpInfo);
    HI_BOOL (*HIFB_DRV_IsLayerSupportDDRDetect)(HI_U32 u32LayerId);
    HI_VOID (*HIFB_DRV_GetMDDR_Status)(HI_U32 u32StartID, HI_U32 u32Cnt, HI_U32 *pu32Status);
    HI_VOID (*HIFB_DRV_SetMDDRDetectZone)(HI_U32 u32StartID, HI_U32 u32Cnt, VO_MDDRC_ZONE_ADDR_S *pstZoneAddr);
    HI_VOID (*HIFB_DRV_ClearDDRDectectZone)(HI_U32 u32StartID, HI_U32 u32Cnt);
    HI_S32  (*HIFB_DRV_CheckDDR_DectectZone)(HI_U32 u32LayerId, HI_U32 u32NewStartID, HI_U32 u32NewCnt, HI_U32 u32PreStartID, HI_U32 u32PreCnt);
    HI_VOID  (*HIFB_DRV_GetDDR_ZoneCfg)(HI_U32 u32LayerId, HI_U32 *pu32StartID, HI_U32 *pu32Cnt);
    HI_VOID  (*HIFB_DRV_InitMDDR)(HI_VOID);

    // for GHDR
    HI_BOOL (*HIFB_DRV_EnableGHDR)(HI_U32 u32LayerId, HI_BOOL bEnable);

    // for ZME
    HI_BOOL (*HIFB_DRV_EnableZME)(HI_U32 u32LayerId, HIFB_RECT *pstInRect, HIFB_RECT *pstOutRect, HI_BOOL bEnable);
    HI_BOOL (*HIFB_DRV_IsLayerSupportZoomOut)(HI_U32 u32LayerId);
    // for int
    HI_BOOL (*HIFB_DRV_GraphicsEnableINT)(HI_U32 u32LayerId, HI_BOOL bEnable);
    HI_BOOL (*HIFB_DRV_GraphicsClearINT)(HI_U32 u32IntClear, HI_S32 s32Irq);
    HI_BOOL (*HIFB_DRV_GraphicsGetINT)(HI_U32 * pu32IntStatus);
    HI_BOOL (*HIFB_DRV_GraphicsClearINTStatus)(HI_U32 u32IntStatus);
    HI_S32  (*HIFB_DRV_GraphicsGetINTDev)(HI_U32 IntStatus, VO_DEV* pVoDev);
}HIFB_DRV_OPS_S;

HI_VOID HIFB_DRV_GetOps(HIFB_DRV_OPS_S *pstOps);
VO_DISP_PIXEL_FORMAT_E HIFB_DRV_ConvertPixelFormat(HIFB_COLOR_FMT_E enColFmt);
HI_U64 HIFB_DRV_GetDcmpOffsetAddr(HI_U64 u64DcmpAddr);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __HIFB_DRV_H__ */
#endif /* __cplusplus */

#endif  /* __HIFB_DRV_H__ */
