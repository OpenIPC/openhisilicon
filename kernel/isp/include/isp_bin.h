/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_BIN_H__
#define __ISP_BIN_H__

#include "type.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define MAX_BIN_REG_NUM 5

/****************************************************************************
 * GENERAL STRUCTURES                                                       *
 ****************************************************************************/
typedef struct ISP_BIN_REG_ATTR_S {
	GK_U32 u32Addr; /* register addr */
	GK_U8 u8StartBit; /* start bit of register addr */
	GK_U8 u8EndBit; /* end bit of register addr */
} ISP_BIN_REG_ATTR_S;

/* -------------------------------------------*
 The base addr of ISP logic register
 The base addr of ISP ext register
 The base addr of AE ext register
 The base addr of AWB ext register
* ------------------------------------------- */

ISP_BIN_REG_ATTR_S g_astIspBinRegAttr[ISP_MAX_PIPE_NUM][MAX_BIN_REG_NUM] = {
	[0 ...(ISP_MAX_PIPE_NUM - 1)] = { 0 }
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_BIN_H__ */
