/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef _API_TDE2_H_
#define _API_TDE2_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "type.h"
#include "tde_type.h"

#define TDE_Open TDE2_Open
#define TDE_Close TDE2_Close
#define TDE_BeginJob TDE2_BeginJob





XMEDIA_S32      TDE2_Open(XMEDIA_VOID);


XMEDIA_VOID     TDE2_Close(XMEDIA_VOID);


TDE_HANDLE  TDE2_BeginJob(XMEDIA_VOID);



XMEDIA_S32      TDE2_EndJob(TDE_HANDLE s32Handle, XMEDIA_BOOL bSync, XMEDIA_BOOL bBlock, XMEDIA_U32 u32TimeOut);



XMEDIA_S32      TDE2_CancelJob(TDE_HANDLE s32Handle);




XMEDIA_S32      TDE2_WaitForDone(TDE_HANDLE s32Handle);



XMEDIA_S32 TDE2_WaitAllDone(XMEDIA_VOID);


XMEDIA_S32 TDE2_Reset(XMEDIA_VOID);



XMEDIA_S32      TDE2_QuickCopy(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstSrc, const TDE2_RECT_S  *pstSrcRect,
                              const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S *pstDstRect);



XMEDIA_S32      TDE2_QuickFill(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S *pstDstRect,
                              XMEDIA_U32 u32FillData);



XMEDIA_S32      TDE2_QuickResize(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstSrc, const TDE2_RECT_S  *pstSrcRect,
                                const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S  *pstDstRect);



XMEDIA_S32      TDE2_QuickDeflicker(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstSrc, const TDE2_RECT_S  *pstSrcRect,
                                   const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S  *pstDstRect);




XMEDIA_S32      TDE2_Bitblit(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstBackGround, const TDE2_RECT_S  *pstBackGroundRect,
                            const TDE2_SURFACE_S *pstForeGround, const TDE2_RECT_S  *pstForeGroundRect, const TDE2_SURFACE_S *pstDst,
                            const TDE2_RECT_S  *pstDstRect, const TDE2_OPT_S *pstOpt);



XMEDIA_S32      TDE2_SolidDraw(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstForeGround, const TDE2_RECT_S  *pstForeGroundRect,
                              const TDE2_SURFACE_S *pstDst,
                              const TDE2_RECT_S  *pstDstRect, const TDE2_FILLCOLOR_S *pstFillColor,
                              const TDE2_OPT_S *pstOpt);



XMEDIA_S32      TDE2_MbBlit(TDE_HANDLE s32Handle, const TDE2_MB_S *pstMB, const TDE2_RECT_S  *pstMbRect, const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S  *pstDstRect,
                           const TDE2_MBOPT_S *pstMbOpt);


XMEDIA_S32      TDE2_BitmapMaskRop(TDE_HANDLE s32Handle,
                                  const TDE2_SURFACE_S *pstBackGround, const TDE2_RECT_S  *pstBackGroundRect,
                                  const TDE2_SURFACE_S *pstForeGround, const TDE2_RECT_S  *pstForeGroundRect,
                                  const TDE2_SURFACE_S *pstMask, const TDE2_RECT_S  *pstMaskRect,
                                  const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S  *pstDstRect,
                                  TDE2_ROP_CODE_E enRopCode_Color, TDE2_ROP_CODE_E enRopCode_Alpha);



XMEDIA_S32      TDE2_BitmapMaskBlend(TDE_HANDLE s32Handle,
                                    const TDE2_SURFACE_S *pstBackGround, const TDE2_RECT_S  *pstBackGroundRect,
                                    const TDE2_SURFACE_S *pstForeGround, const TDE2_RECT_S  *pstForeGroundRect,
                                    const TDE2_SURFACE_S *pstMask, const TDE2_RECT_S  *pstMaskRect,
                                    const TDE2_SURFACE_S *pstDst, const TDE2_RECT_S  *pstDstRect,
                                    XMEDIA_U8 u8Alpha, TDE2_ALUCMD_E enBlendMode);



XMEDIA_S32 TDE2_Rotate(TDE_HANDLE s32Handle, const TDE2_SURFACE_S* pstSrc, const TDE2_RECT_S  *pstSrcRect, const TDE2_SURFACE_S* pstDst,
                                                const TDE2_RECT_S  *pstDstRect,TDE_ROTATE_ANGLE_E enRotateAngle);




XMEDIA_S32 TDE2_SetDeflickerLevel(TDE_DEFLICKER_LEVEL_E enDeflickerLevel);



XMEDIA_S32 TDE2_GetDeflickerLevel(TDE_DEFLICKER_LEVEL_E *pDeflickerLevel);



XMEDIA_S32 TDE2_SetAlphaThresholdValue(XMEDIA_U8 u8ThresholdValue);



XMEDIA_S32 TDE2_GetAlphaThresholdValue(XMEDIA_U8 *pu8ThresholdValue);


XMEDIA_S32 TDE2_SetAlphaThresholdState(XMEDIA_BOOL bEnAlphaThreshold);



XMEDIA_S32 TDE2_GetAlphaThresholdState(XMEDIA_BOOL *p_bEnAlphaThreshold);



XMEDIA_S32 TDE2_PatternFill(TDE_HANDLE s32Handle, const TDE2_SURFACE_S *pstBackGround,
                           const TDE2_RECT_S *pstBackGroundRect, const TDE2_SURFACE_S *pstForeGround,
                           const TDE2_RECT_S *pstForeGroundRect, const TDE2_SURFACE_S *pstDst,
                           const TDE2_RECT_S *pstDstRect, const TDE2_PATTERN_FILL_OPT_S *pstOpt);


XMEDIA_S32 TDE2_EnableRegionDeflicker(XMEDIA_BOOL bRegionDeflicker);

XMEDIA_S32 TDE2_MultiBlending(TDE_HANDLE s32Handle, const TDE_SURFACE_LIST_S *pstSurfaceList);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
