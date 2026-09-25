/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __XMEDIA_API_REGION_H__
#define __XMEDIA_API_REGION_H__

#include "comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

XMEDIA_S32 XMEDIA_API_RGN_Create(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
XMEDIA_S32 XMEDIA_API_RGN_Destroy(RGN_HANDLE Handle);

XMEDIA_S32 XMEDIA_API_RGN_GetAttr(RGN_HANDLE Handle, RGN_ATTR_S *pstRegion);
XMEDIA_S32 XMEDIA_API_RGN_SetAttr(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);

XMEDIA_S32 XMEDIA_API_RGN_SetBitMap(RGN_HANDLE Handle, const BITMAP_S *pstBitmap);

XMEDIA_S32 XMEDIA_API_RGN_AttachToChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
XMEDIA_S32 XMEDIA_API_RGN_DetachFromChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn);

XMEDIA_S32 XMEDIA_API_RGN_SetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
XMEDIA_S32 XMEDIA_API_RGN_GetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, RGN_CHN_ATTR_S *pstChnAttr);

XMEDIA_S32 XMEDIA_API_RGN_GetCanvasInfo(RGN_HANDLE Handle, RGN_CANVAS_INFO_S *pstCanvasInfo);
XMEDIA_S32 XMEDIA_API_RGN_UpdateCanvas(RGN_HANDLE Handle);

XMEDIA_S32 XMEDIA_API_RGN_BatchBegin(RGN_HANDLEGROUP *pu32Group, XMEDIA_U32 u32Num, const RGN_HANDLE handle[]);
XMEDIA_S32 XMEDIA_API_RGN_BatchEnd(RGN_HANDLEGROUP u32Group);

XMEDIA_S32 XMEDIA_API_RGN_GetFd(XMEDIA_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
