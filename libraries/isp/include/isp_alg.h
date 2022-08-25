/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_ALG_H__
#define __ISP_ALG_H__

#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "comm_isp.h"
#include "isp_main.h"
#include "math_fun.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_S32 ISP_AlgRegisterAe(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterAwb(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterAf(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDp(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterGe(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterFrameWDR(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterBayernr(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterFlicker(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterHrs(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDg(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterFeLsc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterLsc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterRLsc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterRc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDrc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDehaze(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterLCac(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterGCac(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterSplit(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterExpander(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDemosaic(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterFcr(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterGamma(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterCsc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterSharpen(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterEdgeMark(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterMcds(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDci(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterLdci(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterCa(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterFPN(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterPreGamma(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterBlc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterFeLogLUT(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterLogLUT(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterClut(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterHlc(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterDetail(VI_PIPE ViPipe);
GK_S32 ISP_AlgRegisterRgbir(VI_PIPE ViPipe);

/* find the specified library */
static inline GK_S32 ISP_FindLib(const ISP_LIB_NODE_S *astLibs,
				 const ALG_LIB_S *pstLib)
{
	GK_S32 i;

	for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
		/* can't use memcmp, if user not fill the string. */
		if ((strncmp(astLibs[i].stAlgLib.acLibName, pstLib->acLibName,
			     ALG_LIB_NAME_SIZE_MAX) == 0) &&
		    (astLibs[i].stAlgLib.s32Id == pstLib->s32Id)) {
			return i;
		}
	}

	return -1;
}

/* search the empty pos of libs */
static inline ISP_LIB_NODE_S *ISP_SearchLib(ISP_LIB_NODE_S *astLibs)
{
	GK_S32 i;

	for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
		if (!astLibs[i].bUsed) {
			return &astLibs[i];
		}
	}

	return GK_NULL;
}

/* search the empty pos of algs */
static inline ISP_ALG_NODE_S *ISP_SearchAlg(ISP_ALG_NODE_S *astAlgs)
{
	GK_S32 i;

	for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
		if (!astAlgs[i].bUsed) {
			return &astAlgs[i];
		}
	}

	return GK_NULL;
}

static inline GK_VOID ISP_AlgsInit(const ISP_ALG_NODE_S *astAlgs,
				   VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_S32 i;

	for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
		if (astAlgs[i].bUsed) {
			if (astAlgs[i].stAlgFunc.pfn_alg_init != GK_NULL) {
				astAlgs[i].stAlgFunc.pfn_alg_init(ViPipe,
								  pRegCfg);
			}
		}
	}

	return;
}

static inline GK_VOID ISP_AlgsRun(const ISP_ALG_NODE_S *astAlgs, VI_PIPE ViPipe,
				  const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
				  GK_S32 s32Rsv)
{
	GK_S32 i;

	for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
		if (astAlgs[i].bUsed) {
			if (astAlgs[i].stAlgFunc.pfn_alg_run != GK_NULL) {
				astAlgs[i].stAlgFunc.pfn_alg_run(
					ViPipe, pStatInfo, pRegCfg, s32Rsv);
			}
		}
	}

	return;
}

static inline GK_VOID ISP_AlgsCtrl(const ISP_ALG_NODE_S *astAlgs,
				   VI_PIPE ViPipe, GK_U32 u32Cmd,
				   GK_VOID *pValue)
{
	GK_S32 i;

	for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
		if (astAlgs[i].bUsed) {
			if (astAlgs[i].stAlgFunc.pfn_alg_ctrl != GK_NULL) {
				astAlgs[i].stAlgFunc.pfn_alg_ctrl(
					ViPipe, u32Cmd, pValue);
			}
		}
	}

	return;
}

static inline GK_VOID ISP_AlgsExit(VI_PIPE ViPipe,
				   const ISP_ALG_NODE_S *astAlgs)
{
	GK_S32 i;

	for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
		if (astAlgs[i].bUsed) {
			if (astAlgs[i].stAlgFunc.pfn_alg_exit != GK_NULL) {
				astAlgs[i].stAlgFunc.pfn_alg_exit(ViPipe);
			}
		}
	}

	return;
}

static inline GK_VOID ISP_AlgsRegister(VI_PIPE ViPipe)
{
	ISP_AlgRegisterAe(ViPipe);
	ISP_AlgRegisterAwb(ViPipe);
#ifdef CONFIG_ISP_AF_SUPPORT
	ISP_AlgRegisterAf(ViPipe);
#endif
	ISP_AlgRegisterDp(ViPipe);

#ifdef CONFIG_ISP_CR_SUPPORT
	ISP_AlgRegisterGe(ViPipe);
#endif
	ISP_AlgRegisterFrameWDR(ViPipe);
	ISP_AlgRegisterExpander(ViPipe);
	ISP_AlgRegisterBlc(ViPipe);
	ISP_AlgRegisterBayernr(ViPipe);
	ISP_AlgRegisterSplit(ViPipe);
	ISP_AlgRegisterFlicker(ViPipe);
	ISP_AlgRegisterDg(ViPipe);
	ISP_AlgRegisterHrs(ViPipe);
	ISP_AlgRegisterFeLsc(ViPipe);
	ISP_AlgRegisterLsc(ViPipe);
	ISP_AlgRegisterRLsc(ViPipe);
	ISP_AlgRegisterRc(ViPipe);
	ISP_AlgRegisterDrc(ViPipe);
	ISP_AlgRegisterDehaze(ViPipe);
	ISP_AlgRegisterLCac(ViPipe);

#ifdef CONFIG_ISP_GCAC_SUPPORT
	ISP_AlgRegisterGCac(ViPipe);
#endif

	ISP_AlgRegisterDemosaic(ViPipe);
	ISP_AlgRegisterFcr(ViPipe);
	ISP_AlgRegisterGamma(ViPipe);
	ISP_AlgRegisterCsc(ViPipe);

#ifdef CONFIG_ISP_CA_SUPPORT
	ISP_AlgRegisterCa(ViPipe);
#endif

#ifdef CONFIG_VI_FPN_SUPPORT
	ISP_AlgRegisterFPN(ViPipe);
#endif
	ISP_AlgRegisterSharpen(ViPipe);

#ifdef CONFIG_ISP_EDGEMARK_SUPPORT
	ISP_AlgRegisterEdgeMark(ViPipe);
#endif
	ISP_AlgRegisterMcds(ViPipe);
	ISP_AlgRegisterLdci(ViPipe);

#ifdef CONFIG_ISP_PREGAMMA_SUPPORT
	ISP_AlgRegisterPreGamma(ViPipe);
#endif
	ISP_AlgRegisterFeLogLUT(ViPipe);
	ISP_AlgRegisterLogLUT(ViPipe);
	ISP_AlgRegisterClut(ViPipe);

#ifdef CONFIG_ISP_HLC_SUPPORT
	ISP_AlgRegisterHlc(ViPipe);
#endif
	ISP_AlgRegisterDetail(ViPipe);
	ISP_AlgRegisterRgbir(ViPipe);
	return;
}

static inline GK_VOID ISP_YUVAlgsRegister(VI_PIPE ViPipe)
{
#ifdef CONFIG_ISP_AF_SUPPORT
	ISP_AlgRegisterAf(ViPipe);
#endif

#ifdef CONFIG_ISP_CA_SUPPORT
	ISP_AlgRegisterCa(ViPipe);
#endif

	ISP_AlgRegisterSharpen(ViPipe);
#ifdef CONFIG_ISP_EDGEMARK_SUPPORT
	ISP_AlgRegisterEdgeMark(ViPipe);
#endif
	ISP_AlgRegisterMcds(ViPipe);
	ISP_AlgRegisterLdci(ViPipe);

	return;
}

/* resolve problem:
isp error: Null Pointer in ISP_AlgRegisterxxx when return isp_init without app exit */
static inline GK_VOID ISP_AlgsUnRegister(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	GK_S32 i;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
		if (pstIspCtx->astAlgs[i].bUsed) {
			pstIspCtx->astAlgs[i].bUsed = GK_FALSE;
		}
	}
}

/* resolve problem:
isp error: MPI_XX_Register failed when return 3a lib register without app exit */
static inline GK_VOID ISP_LibsUnRegister(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	GK_S32 i;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
		pstIspCtx->stAeLibInfo.astLibs[i].bUsed = GK_FALSE;
		pstIspCtx->stAwbLibInfo.astLibs[i].bUsed = GK_FALSE;
		pstIspCtx->stAfLibInfo.astLibs[i].bUsed = GK_FALSE;
	}
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
