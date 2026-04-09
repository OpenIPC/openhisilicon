/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_BLIT_H__
#define __HIFB_BLIT_H__

#include "hi_type.h"
#include "hi_tde_type.h"
#include "hifb_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_BOOL hifb_TdeIsSupportFmt(HIFB_COLOR_FMT_E fmt);
HI_S32 HIFB_DRV_Fill(HIFB_BUFFER_S *pDstImg, HI_U32 u32FillData);
HI_S32 HIFB_DRV_Blit(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg, HIFB_BLIT_OPT_S *pstOpt,
                     HI_BOOL bRefreshScreen, TDE_HANDLE *refresh_handle);
HI_S32 HIFB_DRV_Rotate(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg, HIFB_ROTATE_OPT_S *pstRotOpt,
                       HI_BOOL bRefreshScreen);
HI_S32 HIFB_DRV_SetTdeCallBack(IntCallBack pTdeCallBack);
HI_S32 HIFB_DRV_SetTdeRotateCallBack(IntCallBack pTdeRotCallBack);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __HIFB_BLIT_H__ */
#endif /* __cplusplus */

#endif /* __HIFB_BLIT_H__ */

