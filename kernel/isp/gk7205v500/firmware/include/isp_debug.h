/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_FMWR_DEBUG_H__
#define __ISP_FMWR_DEBUG_H__

#include <stdio.h>
#include "type.h"
#include "errcode.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_DBG_CTRL_S {
    XMEDIA_BOOL bDebugEn;
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_U32  u32Depth;

    XMEDIA_U32  u32Size;
    ISP_DBG_ATTR_S   *pstDbgAttr;
    ISP_DBG_STATUS_S *pstDbgStatus;
} ISP_DBG_CTRL_S;

XMEDIA_S32 ISP_DbgSet(VI_PIPE ViPipe, const ISP_DEBUG_INFO_S *pstDbgInfo);
XMEDIA_S32 ISP_DbgGet(VI_PIPE ViPipe, ISP_DEBUG_INFO_S *pstDbgInfo);
XMEDIA_S32 ISP_DbgRunBgn(ISP_DBG_CTRL_S *pstDbg, XMEDIA_U32 u32FrmCnt);
XMEDIA_S32 ISP_DbgRunEnd(ISP_DBG_CTRL_S *pstDbg, XMEDIA_U32 u32FrmCnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __ISP_FMWR_DEBUG_H__ */
