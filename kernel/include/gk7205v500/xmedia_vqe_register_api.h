/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_VQE_REGISTER_API_H__
#define __XMEDIA_VQE_REGISTER_API_H__

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

XMEDIA_VOID *XMEDIA_VQE_HPF_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_AEC_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_ANR_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_AGC_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_EQ_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_RESAMPLE_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_GAIN_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_TALKV2_GetHandle(XMEDIA_VOID);
XMEDIA_VOID *XMEDIA_VQE_WNR_GetHandle(XMEDIA_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

