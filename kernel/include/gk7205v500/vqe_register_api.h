/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __VQE_REGISTER_API_H__
#define __VQE_REGISTER_API_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* VQE module function api, return pointer to handle of VQE module */
/* record */
XMEDIA_VOID *VQE_RECORD_GetHandle(XMEDIA_VOID);
/* HPF */
XMEDIA_VOID *VQE_HPF_GetHandle(XMEDIA_VOID);
/* AEC */
XMEDIA_VOID *VQE_AEC_GetHandle(XMEDIA_VOID);
/* ANR */
XMEDIA_VOID *VQE_ANR_GetHandle(XMEDIA_VOID);
/* RNR */
XMEDIA_VOID *VQE_RNR_GetHandle(XMEDIA_VOID);
/* HDR */
XMEDIA_VOID *VQE_HDR_GetHandle(XMEDIA_VOID);
/* DRC */
XMEDIA_VOID *VQE_DRC_GetHandle(XMEDIA_VOID);
/* PEQ */
XMEDIA_VOID *VQE_PEQ_GetHandle(XMEDIA_VOID);
/* AGC */
XMEDIA_VOID *VQE_AGC_GetHandle(XMEDIA_VOID);
/* EQ */
XMEDIA_VOID *VQE_EQ_GetHandle(XMEDIA_VOID);
/* RESAMPLE */
XMEDIA_VOID *VQE_RESAMPLE_GetHandle(XMEDIA_VOID);
/* GAIN */
XMEDIA_VOID *VQE_GAIN_GetHandle(XMEDIA_VOID);
/* TALKV2 */
XMEDIA_VOID *VQE_TALKV2_GetHandle(XMEDIA_VOID);
/* WNR */
XMEDIA_VOID *VQE_WNR_GetHandle(XMEDIA_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __VQE_REGISTER_API_H__ */

