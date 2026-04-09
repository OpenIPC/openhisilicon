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

#ifndef __HIFB_VOU_GRAPHICS_H__
#define __HIFB_VOU_GRAPHICS_H__

#include "hi_type.h"
#include "hifb.h"
#include "hifb_vou_drv.h"
#include "hifb_graphic_hal.h"
#include "hifb_graphics_drv.h"


#define MDDRC_ZONE_MAX_NUM          32
#define HIFB_LINE_BUF          1920
#define HIFB_MAX_LAYER_NUM 1
typedef enum hiVOU_BITEXT_MODE_E
{
    VOU_BITEXT_LOW_ZERO         = 0x0,
    VOU_BITEXT_LOW_HIGHBIT      = 0x2,
    VOU_BITEXT_LOW_HIGHBITS     = 0x3,
    VOU_BITEXT_BUTT
} VOU_BITEXT_MODE_E;


typedef enum hiVOU_COLORKEY_MODE_E
{
    VOU_COLORKEY_IN     = 0x0,
    VOU_COLORKEY_OUT    = 0x1,
    VOU_COLORKEY_BUTT
} VOU_COLORKEY_MODE_E;
typedef struct tagVO_GFX_KEY_MAX_S
{
    HI_U8 u8KeyMax_R;
    HI_U8 u8KeyMax_G;
    HI_U8 u8KeyMax_B;

}VO_GFX_KEY_MAX_S;

typedef struct tagVO_GFX_KEY_MIN_S
{
    HI_U8 u8KeyMin_R;
    HI_U8 u8KeyMin_G;
    HI_U8 u8KeyMin_B;

} VO_GFX_KEY_MIN_S;

typedef struct tagVO_GFX_MASK_S
{
    HI_U8 u8Mask_r;
    HI_U8 u8Mask_g;
    HI_U8 u8Mask_b;

} VO_GFX_MASK_S;

typedef enum hiVOU_SCAN_MODE_E
{
    VOU_SCAN_MODE_INTERLACE = 0x0,
    VOU_SCAN_MODE_PROGRESSIVE = 0x1,
    VOU_SCAN_MODE_BUTT
}VOU_SCAN_MODE_E;

typedef enum
{
    VO_GFX_BITEXTEND_1ST =   0,
    VO_GFX_BITEXTEND_2ND = 0x2,
    VO_GFX_BITEXTEND_3RD = 0x3,

    VO_GFX_BITEXTEND_BUTT
}VO_GFX_BITEXTEND_E;
typedef HI_S32 (*VO_FB_IntCallBack)(const HI_VOID *pParaml, HI_VOID *pParamr);
//typedef struct hiVO_GRAPHIC_LAYER_

HI_BOOL VOU_GraphicsEnableDcmp(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);
HI_BOOL VOU_GraphicsGetDcmpEnableState(GRAPHIC_LAYER gfxLayer, HI_BOOL *pbEnable);
HI_VOID VOU_GraphicsEnableWbc(HAL_DISP_LAYER_E gfxLayer, HI_BOOL bEnable);
HI_VOID VOU_GraphicsSetMDDRDetectZone(HI_U32 u32StartID, HI_U32 u32Cnt, VO_MDDRC_ZONE_ADDR_S *pstZoneAddr);
HI_VOID VOU_GraphicsGetMDDRStatus(HI_U32 u32StartID, HI_U32 u32Cnt, HI_U32 *pu32Status);
HI_VOID VOU_GraphicsClearDDRDectectZone(HI_U32 u32StartID, HI_U32 u32Cnt);
HI_S32 VOU_GraphicsCheckDDRDectectZone(GRAPHIC_LAYER gfxLayer, HI_U32 u32NewStartID, HI_U32 u32NewCnt, HI_U32 u32PreStartID, HI_U32 u32PreCnt);
HI_VOID VOU_GraphicsGetDDRZoneCfg(GRAPHIC_LAYER gfxLayer, HI_U32 *pu32StartID, HI_U32 *pu32Cnt);
HI_VOID VOU_GraphicsInitMDDRDetect(HI_VOID);

HI_BOOL  VOU_GraphicsSetGfxExt(GRAPHIC_LAYER gfxLayer, VO_GFX_BITEXTEND_E enMode);
HI_BOOL VOU_GraphicsSetGfxPalpha(GRAPHIC_LAYER gfxLayer, HI_U32 bAlphaEn,HI_U32 bArange,HI_U8 u8Alpha0,HI_U8 u8Alpha1);
HI_BOOL VOU_GraphicsSetLayerGalpha(GRAPHIC_LAYER gfxLayer, HI_U8 u8Alpha0);
HI_BOOL VOU_GraphicsSetCscEn(GRAPHIC_LAYER gfxLayer, HI_BOOL bCscEn);
HI_BOOL VOU_GraphicsSetGfxAddr(GRAPHIC_LAYER gfxLayer, HI_U64 u64LAddr);
HI_BOOL VOU_GraphicsSetGfxStride(GRAPHIC_LAYER gfxLayer, HI_U16 u16pitch);

HI_BOOL VOU_GraphicsGetGfxPreMult(GRAPHIC_LAYER gfxLayer, HI_U32 *pbEnable);
HI_BOOL VOU_GraphicsSetGfxPreMult(GRAPHIC_LAYER gfxLayer, HI_U32 bEnable);
HI_BOOL VOU_GraphicsSetLayerDataFmt(GRAPHIC_LAYER gfxLayer, VO_DISP_PIXEL_FORMAT_E  enDataFmt);
HI_BOOL VOU_GraphicsSetLayerInRect(GRAPHIC_LAYER gfxLayer, HIFB_RECT *pstRect);
HI_BOOL VOU_GraphicsSetLayerSrcImageReso(GRAPHIC_LAYER gfxLayer, HIFB_RECT *pstRect);
HI_BOOL VOU_GraphicsSetLayerOutRect(GRAPHIC_LAYER gfxLayer, HIFB_RECT *pstRect);
HI_BOOL VOU_GraphicsSetColorKeyValue(GRAPHIC_LAYER gfxLayer, VO_GFX_KEY_MAX_S stKeyMax, VO_GFX_KEY_MIN_S stKeyMin);
HI_BOOL VOU_GraphicsSetColorKeyMask(GRAPHIC_LAYER gfxLayer, VO_GFX_MASK_S stMsk);
HI_BOOL GRAPHIC_DRV_GetDevEnable(VO_DEV VoDev, HI_BOOL *pbIntfEn);
HI_BOOL GRAPHIC_DRV_GetIntfSync(VO_DEV VoDev,HAL_DISP_SYNCINFO_S *pstSyncInfo);
HI_BOOL GRAPHIC_DRV_GetIntfMuxSel(VO_DEV VoDev,VO_INTF_TYPE_E *pbenIntfType);
HI_BOOL VOU_GraphicsSetGfxKeyEn(GRAPHIC_LAYER gfxLayer, HI_U32 u32KeyEnable);
HI_BOOL VOU_GraphicsSetGfxKeyMode(GRAPHIC_LAYER gfxLayer, HI_U32 u32KeyOut);

HI_BOOL  VOU_GraphicsSetRegUp(GRAPHIC_LAYER gfxLayer);
HI_BOOL VOU_GraphicsGetLayerGalpha(GRAPHIC_LAYER gfxLayer,HI_U8 *pu8Alpha0);
HI_BOOL VOU_GraphicsGetLayerDataFmt(GRAPHIC_LAYER gfxLayer,HI_U32 *pu32Fmt);
HI_BOOL VOU_GraphicsGetGfxStride(GRAPHIC_LAYER gfxLayer, HI_U32 *pu32GfxStride);
HI_BOOL VOU_GraphicsGetGfxAddr(GRAPHIC_LAYER gfxLayer, HI_U64 *pu64GfxAddr);

HI_S32 VOU_GraphicsGetDevMode(HAL_DISP_LAYER_E u32Layer, VOU_SCAN_MODE_E *pScanMode, HI_BOOL *pbFeildUpdate);

HI_VOID VO_GraphicsVtthIntProcess(VO_DEV VoDev);
HI_VOID VO_GraphicsWbcIntProcess(HI_U32 IntStatus);

HI_S32 VOU_GraphicsBindDev(GRAPHIC_LAYER gfxLayer, VO_DEV VoDev);
HI_S32 VOU_GraphicsUnBindDev(GRAPHIC_LAYER gfxLayer, VO_DEV VoDev);

HI_S32 VOU_GraphicsSetCSC(HI_U32 u32Layer, VO_CSC_S *pstCsc);
HI_S32 VOU_GraphicsGetCSC(HI_U32 u32Layer, VO_CSC_S * pstCsc);
HI_S32 VO_GraphicsInit(HI_VOID);
HI_S32 VO_GraphicsDeInit(HI_VOID);
HI_S32 VOU_GraphicsResourceInit(HI_VOID);
HI_S32 VOU_GraphicsResourceDeInit(HI_VOID);

HI_S32 VOU_GraphicsOpenLayer(GRAPHIC_LAYER  gfxLayer);
HI_S32 VOU_GraphicsCloseLayer(GRAPHIC_LAYER gfxLayer);
HI_S32 VOU_GraphicsEnableLayer(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);
HI_S32 VOU_GraphicsSetCallback(GRAPHIC_LAYER gfxLayer, VO_FB_INT_TYPE_E enIntType, VO_FB_IntCallBack call_back, HI_VOID *pArg);
HI_S32 VOU_GraphicsGetIntfSize(GRAPHIC_LAYER gfxLayer, HI_U32 *pu32Width, HI_U32 *pu32Height);
HI_S32 VOU_GraphicsGetIntfType(GRAPHIC_LAYER gfxLayer, VO_INTF_TYPE_E *penIntfType);


HI_S32 VOU_GraphicsSetCscCoef(GRAPHIC_LAYER gfxLayer);

HI_S32  VOU_GraphicsShowProc(osal_proc_entry_t *s);
HI_S32  VOU_GraphicsEnableInt(HI_U32 u32LayerIndex, HI_BOOL bEnable);
HI_BOOL VOU_GraphicsClearInt(HI_U32 u32IntClear, HI_S32 s32Irq);
HI_BOOL VOU_GraphicsGetInt(HI_U32 * pu32IntStaus);
HI_BOOL VOU_GraphicsClearIntStatus(HI_U32 u32IntStatus);
HI_S32 VOU_GraphicsGetInterruptDev(HI_U32 IntStatus, VO_DEV* pVoDev);
// for GHDR
HI_BOOL VOU_GraphicsEnableGHDR(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);

// for ZME
HI_BOOL VOU_GraphicsEnableZME(GRAPHIC_LAYER gfxLayer, RECT_S *stInRect,RECT_S *stOutRect,HI_BOOL bEnable);



#endif /* __VOU_GRAPHICS_H__ */

