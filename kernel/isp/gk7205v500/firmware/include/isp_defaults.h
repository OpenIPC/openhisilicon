/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_DEFAULTS_H_
#define __ISP_DEFAULTS_H_

#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* ext register default value */
XMEDIA_VOID ISP_ExtRegsDefault(VI_PIPE ViPipe);

/* initialize ext register */
XMEDIA_VOID ISP_ExtRegsInitialize(VI_PIPE ViPipe);

/* isp register default value */
XMEDIA_VOID ISP_RegsDefault(VI_PIPE ViPipe);

/* initialize isp register */
XMEDIA_VOID ISP_RegsInitialize(VI_PIPE ViPipe);

/* This function initialises an instance of ISP_CTX_S */
XMEDIA_VOID ISP_GlobalInitialize(VI_PIPE ViPipe);

XMEDIA_VOID ISP_DngExtRegsInitialize(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_DEFAULTS_H_ */
