/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "isp_regcfg.h"
#include "isp_config.h"
#include "isp_config_u32.h"
#include "isp_lut_config.h"
#include "isp_ext_config.h"
#include "isp_main.h"
#include "ot_mpi_sys.h"
#include "math_fun.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

ISP_BE_BUF_S g_astBeBufCtx[ISP_MAX_PIPE_NUM] = { { 0 } };
ISP_REGCFG_S *g_pastRegCfgCtx[ISP_MAX_PIPE_NUM] = { GK_NULL };
ISP_BE_LUT_BUF_S g_astBeLutBufCtx[ISP_MAX_PIPE_NUM] = { 0 };
ISP_LDCI_READ_STT_BUF_S g_astLdciReadSttBufCtx[ISP_MAX_PIPE_NUM] = { 0 };
const GK_U32 g_au32IsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {
	100,   200,   400,    800,    1600,   3200,   6400,    12800,
	25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800
};

#define BE_REG_GET_CTX(dev, pstCtx) pstCtx = &g_astBeBufCtx[dev]
#define BE_LUT_BUF_GET_CTX(dev, pstCtx) pstCtx = &g_astBeLutBufCtx[dev]
#define LDCI_BUF_GET_CTX(dev, pstCtx) pstCtx = &g_astLdciReadSttBufCtx[dev]

extern GK_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

GK_S32 ISP_ModParamGet(VI_PIPE ViPipe, ISP_MOD_PARAM_S *pstModParam)
{
	ISP_CHECK_POINTER(pstModParam);

	if (ioctl(g_as32IspFd[ViPipe], ISP_GET_MOD_PARAM, pstModParam) !=
	    GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "Get ModParam failed!\n");
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_ClutBufInit(VI_PIPE ViPipe)
{
	return GK_SUCCESS;
}

GK_S32 ISP_ClutBufExit(VI_PIPE ViPipe)
{
	return GK_SUCCESS;
}

GK_S32 ISP_SpecAwbBufInit(VI_PIPE ViPipe)
{
	if (ioctl(g_as32IspFd[ViPipe], ISP_SPECAWB_BUF_INIT) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] specawb buffer init failed\n", ViPipe);
		return ERR_CODE_ISP_MEM_NOT_INIT;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SpecAwbBufExit(VI_PIPE ViPipe)
{
	if (ioctl(g_as32IspFd[ViPipe], ISP_SPECAWB_BUF_EXIT) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] exit specawb bufs failed\n",
			  ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_UpdateBeLutSttBufCtx(VI_PIPE ViPipe, GK_U64 u64PhyAddr)
{
	GK_U8 i;
	GK_U64 u64Size;
	GK_VOID *pVirtAddr = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_FAILURE;
	}

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);

	pVirtAddr = MPI_SYS_Mmap(u64PhyAddr, u64Size * 2 * ISP_MAX_BE_NUM);

	if (pVirtAddr == GK_NULL) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d get be lut stt bufs address failed!\n",
			  ViPipe);
		return GK_FAILURE;
	}

	for (i = 0; i < ISP_MAX_BE_NUM; i++) {
		pstBeLutBuf->astLutSttBuf[i].u64PhyAddr =
			u64PhyAddr + 2 * i * u64Size;
		pstBeLutBuf->astLutSttBuf[i].pVirAddr =
			(GK_VOID *)((GK_U8 *)pVirtAddr + 2 * i * u64Size);
		;
		pstBeLutBuf->astLutSttBuf[i].u64Size = u64Size;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_BeLutBufAddrInit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	GK_U64 u64PhyAddr;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_SUCCESS;
	}

	s32Ret =
		ioctl(g_as32IspFd[ViPipe], ISP_BE_LUT_STT_BUF_GET, &u64PhyAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d get be lut2stt bufs address failed%x!\n",
			  ViPipe, s32Ret);
		return s32Ret;
	}

	ext_system_be_lut_stt_buffergh_addr_write(ViPipe, (u64PhyAddr >> 32));
	ext_system_be_lut_stt_buffer_low_addr_write(ViPipe,
						    (u64PhyAddr & 0xFFFFFFFF));

	s32Ret = ISP_UpdateBeLutSttBufCtx(ViPipe, u64PhyAddr);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_BeLutBufAddrExit(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_SUCCESS;
	}

	if (pstBeLutBuf->astLutSttBuf[0].pVirAddr != GK_NULL) {
		MPI_SYS_Munmap(pstBeLutBuf->astLutSttBuf[0].pVirAddr,
			       sizeof(S_ISP_LUT_WSTT_TYPE) * 2 *
				       ISP_MAX_BE_NUM);
		for (i = 0; i < ISP_MAX_BE_NUM; i++) {
			pstBeLutBuf->astLutSttBuf[i].pVirAddr = GK_NULL;
		}
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SttBufInit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;

	if (ioctl(g_as32IspFd[ViPipe], ISP_STT_BUF_INIT) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] stt buffer init failed\n",
			  ViPipe);
		return GK_FAILURE;
	}

	s32Ret = ISP_BeLutBufAddrInit(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] be lut2stt buffer address init failed\n",
			  ViPipe);
		ISP_SttBufExit(ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SttBufExit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;

	s32Ret = ISP_BeLutBufAddrExit(ViPipe);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] be lut stt buffer exit failed\n", ViPipe);
		return GK_FAILURE;
	}

	if (ioctl(g_as32IspFd[ViPipe], ISP_STT_BUF_EXIT) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "exit stt bufs failed\n");
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SttAddrInit(VI_PIPE ViPipe)
{
	if (ioctl(g_as32IspFd[ViPipe], ISP_STT_ADDR_INIT) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] stt address init failed\n",
			  ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_UpdateLdciReadSttBufCtx(VI_PIPE ViPipe, GK_U64 u64PhyAddr)
{
	GK_U8 i;
	GK_U64 u64Size;
	GK_VOID *pVirtAddr = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LDCI_READ_STT_BUF_S *pstLdciReadSttBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->bLdciTprFltEn == GK_FALSE) {
		return GK_SUCCESS;
	}

	LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

	u64Size = sizeof(ISP_LDCI_STAT_S);
	pVirtAddr =
		MPI_SYS_Mmap(u64PhyAddr, u64Size * pstLdciReadSttBuf->u8BufNum);

	if (pVirtAddr == GK_NULL) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d]:map ldci read stt buffer failed\n", ViPipe);
		return GK_FAILURE;
	}

	for (i = 0; i < pstLdciReadSttBuf->u8BufNum; i++) {
		pstLdciReadSttBuf->astReadBuf[i].u64PhyAddr =
			u64PhyAddr + i * u64Size;
		pstLdciReadSttBuf->astReadBuf[i].pVirAddr =
			(GK_VOID *)((GK_U8 *)pVirtAddr + i * u64Size);
		pstLdciReadSttBuf->astReadBuf[i].u64Size = u64Size;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_LdciReadSttBufAddInit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	GK_U64 u64PhyAddr;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LDCI_READ_STT_BUF_S *pstLdciReadSttBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->bLdciTprFltEn == GK_FALSE) {
		return GK_SUCCESS;
	}

	LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_LDCI_READ_STT_BUF_GET,
		       pstLdciReadSttBuf);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d]:Get ldci read stt buffer address failed\n",
			  ViPipe);
		return s32Ret;
	}

	u64PhyAddr = pstLdciReadSttBuf->u64HeadPhyAddr;
	ext_system_ldci_read_stt_buffer_low_addr_write(ViPipe,
						       u64PhyAddr & 0xFFFFFFFF);
	ext_system_ldci_read_stt_buffergh_addr_write(ViPipe, u64PhyAddr >> 32);

	s32Ret = ISP_UpdateLdciReadSttBufCtx(ViPipe, u64PhyAddr);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_LdciReadSttBufAddExit(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LDCI_READ_STT_BUF_S *pstLdciReadSttBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->bLdciTprFltEn == GK_FALSE) {
		return GK_SUCCESS;
	}

	LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

	if (pstLdciReadSttBuf->astReadBuf[0].pVirAddr != GK_NULL) {
		MPI_SYS_Munmap(pstLdciReadSttBuf->astReadBuf[0].pVirAddr,
			       sizeof(ISP_LDCI_STAT_S) *
				       pstLdciReadSttBuf->u8BufNum);

		for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
			pstLdciReadSttBuf->astReadBuf[i].pVirAddr = GK_NULL;
		}
	}

	return GK_SUCCESS;
}

GK_S32 ISP_LdciBufInit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_CTRL_PARAM_S stIspCtrlParam = { 0 };
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	s32Ret =
		ioctl(g_as32IspFd[ViPipe], ISP_GET_CTRL_PARAM, &stIspCtrlParam);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get ctrlparam failed\n",
			  ViPipe);
		return s32Ret;
	}

	pstIspCtx->bLdciTprFltEn = stIspCtrlParam.bLdciTprFltEn;

	if ((pstIspCtx->bLdciTprFltEn == GK_FALSE) &&
	    (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))) {
		return GK_SUCCESS;
	}

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_LDCI_BUF_INIT);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] ldci buffer init failed\n",
			  ViPipe);
		return GK_FAILURE;
	}

	if (pstIspCtx->bLdciTprFltEn == GK_TRUE) {
		s32Ret = ISP_LdciReadSttBufAddInit(ViPipe);

		if (s32Ret != GK_SUCCESS) {
			ISP_TRACE(
				MODULE_DBG_ERR,
				"ISP[%d] ldci read stt buffer address init failed\n",
				ViPipe);
			ISP_LdciBufExit(ViPipe);
			return GK_FAILURE;
		}
	}

	return GK_SUCCESS;
}

GK_S32 ISP_LdciBufExit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	if ((pstIspCtx->bLdciTprFltEn == GK_FALSE) &&
	    IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_SUCCESS;
	}

	if (pstIspCtx->bLdciTprFltEn == GK_TRUE) {
		s32Ret = ISP_LdciReadSttBufAddExit(ViPipe);
		if (s32Ret != GK_SUCCESS) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "ISP[%d] exit readstt bufaddr failed\n",
				  ViPipe);
			return GK_FAILURE;
		}
	}

	if (ioctl(g_as32IspFd[ViPipe], ISP_LDCI_BUF_EXIT) != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] exit ldci bufs failed\n",
			  ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_CfgBeBufInit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	GK_U32 u32BeBufSize;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_CHECK_OFFLINE_MODE(ViPipe);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_INIT,
		       &pstBeBuf->u64BePhyAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d init be config bufs failed %x!\n", ViPipe,
			  s32Ret);
		return s32Ret;
	}

	pstBeBuf->pBeVirtAddr = MPI_SYS_MmapCache(pstBeBuf->u64BePhyAddr,
						  sizeof(ISP_BE_WO_REG_CFG_S) *
							  MAX_ISP_BE_BUF_NUM);

	if (pstBeBuf->pBeVirtAddr == GK_NULL) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d init be config bufs failed!\n", ViPipe);
		s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_EXIT);

		if (s32Ret != GK_SUCCESS) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Pipe:%d exit be config bufs failed %x!\n",
				  ViPipe, s32Ret);
			return s32Ret;
		}

		return ERR_CODE_ISP_NOMEM;
	}

	pstBeBuf->stBeWoCfgBuf.u64PhyAddr = pstBeBuf->u64BePhyAddr;

	/* Get be buffer start address & size */
	u32BeBufSize = sizeof(ISP_BE_WO_REG_CFG_S) * MAX_ISP_BE_BUF_NUM;
	ext_system_be_buffer_addressgh_write(ViPipe,
					     (pstBeBuf->u64BePhyAddr >> 32));
	ext_system_be_buffer_address_low_write(ViPipe, (pstBeBuf->u64BePhyAddr &
							0xFFFFFFFF));
	ext_system_be_buffer_size_write(ViPipe, u32BeBufSize);

	return GK_SUCCESS;
}

GK_S32 ISP_UpdateBeBufAddr(VI_PIPE ViPipe, GK_VOID *pVirtAddr)
{
	GK_U16 i;
	GK_U64 u64BufSize;
	ISP_RUNNING_MODE_E enIspRuningMode;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	enIspRuningMode = pstIspCtx->stBlockAttr.enIspRunningMode;
	u64BufSize = sizeof(ISP_BE_WO_REG_CFG_S) / ISP_STRIPING_MAX_NUM;

	switch (enIspRuningMode) {
	case ISP_MODE_RUNNING_STRIPING:
		for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
			pstIspCtx->pIspBeVirtAddr[i] =
				(GK_VOID *)((GK_U8 *)pVirtAddr +
					    i * u64BufSize);
			pstIspCtx->pViProcVirtAddr[i] =
				(GK_VOID *)((GK_U8 *)pstIspCtx
						    ->pIspBeVirtAddr[i] +
					    VIPROC_OFFLINE_OFFSET);
		}
		break;

	case ISP_MODE_RUNNING_OFFLINE:
		for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
			if (i == 0) {
				pstIspCtx->pIspBeVirtAddr[i] = pVirtAddr;
				pstIspCtx->pViProcVirtAddr[i] =
					(GK_VOID *)((GK_U8 *)pVirtAddr +
						    VIPROC_OFFLINE_OFFSET);
			} else {
				pstIspCtx->pIspBeVirtAddr[i] = GK_NULL;
				pstIspCtx->pViProcVirtAddr[i] = GK_NULL;
			}
		}
		break;

	default:
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_CfgBeBufMmap(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	GK_U64 u64BePhyAddr;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	u64BePhyAddr = pstBeBuf->stBeWoCfgBuf.u64PhyAddr;
	ext_system_be_free_buffergh_addr_write(ViPipe, (u64BePhyAddr >> 32));
	ext_system_be_free_buffer_low_addr_write(ViPipe,
						 (u64BePhyAddr & 0xFFFFFFFF));

	if (pstBeBuf->pBeVirtAddr != GK_NULL) {
		pstBeBuf->stBeWoCfgBuf.pVirAddr =
			(GK_VOID *)((GK_U8 *)pstBeBuf->pBeVirtAddr +
				    (pstBeBuf->stBeWoCfgBuf.u64PhyAddr -
				     pstBeBuf->u64BePhyAddr));
	} else {
		pstBeBuf->stBeWoCfgBuf.pVirAddr = GK_NULL;
	}

	if (pstBeBuf->stBeWoCfgBuf.pVirAddr == GK_NULL) {
		return GK_FAILURE;
	}

	s32Ret = ISP_UpdateBeBufAddr(ViPipe, pstBeBuf->stBeWoCfgBuf.pVirAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d isp update BE bufs failed %x!\n", ViPipe,
			  s32Ret);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_GetBeBufFirst(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_CHECK_OFFLINE_MODE(ViPipe);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_BE_BUF_FIRST,
		       &pstBeBuf->stBeWoCfgBuf.u64PhyAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d Get be free bufs failed %x!\n", ViPipe,
			  s32Ret);
		return s32Ret;
	}

	s32Ret = ISP_CfgBeBufMmap(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d ISP_CfgBeBufMmap failed %x!\n", ViPipe,
			  s32Ret);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_GetBeFreeBuf(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_CHECK_OFFLINE_MODE(ViPipe);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_FREE_BUF_GET,
		       &pstBeBuf->stBeWoCfgBuf);

	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = ISP_CfgBeBufMmap(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d ISP_CfgBeBufMmap failed %x!\n", ViPipe,
			  s32Ret);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_GetBeLastBuf(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_CHECK_OFFLINE_MODE(ViPipe);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_LAST_BUF_GET,
		       &pstBeBuf->stBeWoCfgBuf.u64PhyAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d Get be busy bufs failed %x!\n", ViPipe,
			  s32Ret);
		return s32Ret;
	}

	s32Ret = ISP_CfgBeBufMmap(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d ISP_CfgBeBufMmap failed %x!\n", ViPipe,
			  s32Ret);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_CfgBeBufExit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_CHECK_OFFLINE_MODE(ViPipe);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	if (pstBeBuf->pBeVirtAddr != GK_NULL) {
		MPI_SYS_Munmap(pstBeBuf->pBeVirtAddr,
			       sizeof(ISP_BE_WO_REG_CFG_S) *
				       MAX_ISP_BE_BUF_NUM);
		pstBeBuf->pBeVirtAddr = GK_NULL;
	}

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_EXIT);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d exit be config bufs failed %x!\n", ViPipe,
			  s32Ret);
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_CfgBeBufCtl(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_CHECK_OFFLINE_MODE(ViPipe);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_CTL,
		       &pstBeBuf->stBeWoCfgBuf);
	if (s32Ret) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SetCfgBeBufState(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;

	ISP_CHECK_OFFLINE_MODE(ViPipe);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_RUNNING);
	if (s32Ret) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

/* init isp be cfgs all buffer */
GK_S32 ISP_AllCfgsBeBufInit(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;

	ISP_CHECK_OFFLINE_MODE(ViPipe);

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_All_BUF_INIT);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] init be all bufs Failed with ec %#x!\n",
			  ViPipe, s32Ret);
		return s32Ret;
	}

	return GK_SUCCESS;
}

static GK_S8 ISP_GetBlockIdByPipe(VI_PIPE ViPipe)
{
	GK_S8 s8BlockId = 0;

	switch (ViPipe) {
	case ISP_BE0_PIPE_ID:
		s8BlockId = 0;
		break;

	case ISP_BE1_PIPE_ID:
		s8BlockId = 1;
		break;

	default:
		return GK_FAILURE;
	}

	return s8BlockId;
}

GK_S32 ISP_BeVregAddrInit(VI_PIPE ViPipe)
{
	GK_U8 k = 0;
	GK_S8 s8BlkDev = 0;
	GK_U8 u8BlockId = 0;
	GK_U64 u64BufSize;
	ISP_RUNNING_MODE_E enIspRuningMode;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	enIspRuningMode = pstIspCtx->stBlockAttr.enIspRunningMode;
	u64BufSize = sizeof(ISP_BE_WO_REG_CFG_S) / ISP_STRIPING_MAX_NUM;

	switch (enIspRuningMode) {
	case ISP_MODE_RUNNING_ONLINE:
		s8BlkDev = ISP_GetBlockIdByPipe(ViPipe);

		if (s8BlkDev == -1) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "ISP[%d] init Online Mode Pipe Err!\n",
				  ViPipe);
			return GK_FAILURE;
		}

		u8BlockId = (GK_U8)s8BlkDev;

		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k == 0) {
				pstIspCtx->pIspBeVirtAddr[k] =
					VReg_GetVirtAddrBase(
						ISP_BE_REG_BASE(u8BlockId));
				pstIspCtx->pViProcVirtAddr[k] =
					VReg_GetVirtAddrBase(
						ISP_VIPROC_REG_BASE(u8BlockId));
			} else {
				pstIspCtx->pIspBeVirtAddr[k] = GK_NULL;
				pstIspCtx->pViProcVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_OFFLINE:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k == 0) {
				pstIspCtx->pIspBeVirtAddr[k] =
					pstBeBuf->stBeWoCfgBuf.pVirAddr;
				pstIspCtx->pViProcVirtAddr[k] =
					(GK_VOID *)((GK_U8 *)pstBeBuf
							    ->stBeWoCfgBuf
							    .pVirAddr +
						    VIPROC_OFFLINE_OFFSET);
			} else {
				pstIspCtx->pIspBeVirtAddr[k] = GK_NULL;
				pstIspCtx->pViProcVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_SIDEBYSIDE:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k < ISP_MAX_BE_NUM) {
				pstIspCtx->pIspBeVirtAddr[k] =
					VReg_GetVirtAddrBase(
						ISP_BE_REG_BASE(k));
				pstIspCtx->pViProcVirtAddr[k] =
					VReg_GetVirtAddrBase(
						ISP_VIPROC_REG_BASE(k));
			} else {
				pstIspCtx->pIspBeVirtAddr[k] = GK_NULL;
				pstIspCtx->pViProcVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_STRIPING:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			pstIspCtx->pIspBeVirtAddr[k] =
				(GK_VOID *)((GK_U8 *)pstBeBuf->stBeWoCfgBuf
						    .pVirAddr +
					    k * u64BufSize);
			pstIspCtx->pViProcVirtAddr[k] =
				(GK_VOID *)((GK_U8 *)pstIspCtx
						    ->pIspBeVirtAddr[k] +
					    VIPROC_OFFLINE_OFFSET);
		}
		break;

	default:
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init Running Mode Err!\n",
			  ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_VOID *ISP_VRegCfgBufAddr(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
	GK_U64 u64Size;
	GK_U64 u64PhyAddrHigh;
	GK_U64 u64PhyAddrTemp;
	ISP_BE_BUF_S *pstBeBuf = GK_NULL;

	BE_REG_GET_CTX(ViPipe, pstBeBuf);

	u64Size = sizeof(ISP_BE_WO_REG_CFG_S) / ISP_STRIPING_MAX_NUM;

	if (pstBeBuf->stBeWoCfgBuf.pVirAddr != GK_NULL) {
		return ((GK_U8 *)pstBeBuf->stBeWoCfgBuf.pVirAddr +
			BlkDev * u64Size);
	}

	u64PhyAddrHigh = (GK_U64)ext_system_be_free_buffergh_addr_read(ViPipe);
	u64PhyAddrTemp =
		(GK_U64)ext_system_be_free_buffer_low_addr_read(ViPipe);
	u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

	pstBeBuf->stBeWoCfgBuf.u64PhyAddr = u64PhyAddrTemp;
	pstBeBuf->stBeWoCfgBuf.pVirAddr = MPI_SYS_MmapCache(
		pstBeBuf->stBeWoCfgBuf.u64PhyAddr, sizeof(ISP_BE_WO_REG_CFG_S));

	return ((GK_U8 *)pstBeBuf->stBeWoCfgBuf.pVirAddr + BlkDev * u64Size);
}

GK_S32 ISP_GetBeVregCfgAddr(VI_PIPE ViPipe, GK_VOID *pVirtAddr[])
{
	GK_U8 k = 0;
	GK_S8 s8BlkDev = 0;
	GK_U8 u8BlockId = 0;
	GK_S32 s32Ret;
	ISP_WORKING_MODE_S stIspWorkMode;

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode);

	if (s32Ret) {
		ISP_TRACE(MODULE_DBG_ERR, "get isp work mode failed!\n");
		return s32Ret;
	}

	switch (stIspWorkMode.enIspRunningMode) {
	case ISP_MODE_RUNNING_ONLINE:
		s8BlkDev = ISP_GetBlockIdByPipe(ViPipe);

		if (s8BlkDev == -1) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "ISP[%d] Online Mode Pipe Err!\n", ViPipe);
			return GK_FAILURE;
		}

		u8BlockId = (GK_U8)s8BlkDev;

		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k == 0) {
				pVirtAddr[k] = VReg_GetVirtAddrBase(
					ISP_BE_REG_BASE(u8BlockId));
			} else {
				pVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_OFFLINE:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k == 0) {
				pVirtAddr[k] =
					ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
			} else {
				pVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_SIDEBYSIDE:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k < ISP_MAX_BE_NUM) {
				pVirtAddr[k] = VReg_GetVirtAddrBase(
					ISP_BE_REG_BASE(k));
			} else {
				pVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_STRIPING:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			pVirtAddr[k] = ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
		}
		break;

	default:
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] GetBe Running Mode Err!\n",
			  ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_GetViProcCfgAddr(VI_PIPE ViPipe, GK_VOID *pVirtAddr[])
{
	GK_U8 k = 0;
	GK_S8 s8BlkDev = 0;
	GK_U8 u8BlockId = 0;
	GK_S32 s32Ret;
	GK_VOID *pBeVirtAddr;
	ISP_WORKING_MODE_S stIspWorkMode;

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode);

	if (s32Ret) {
		ISP_TRACE(MODULE_DBG_ERR, "get isp work mode failed!\n");
		return s32Ret;
	}

	switch (stIspWorkMode.enIspRunningMode) {
	case ISP_MODE_RUNNING_ONLINE:
		s8BlkDev = ISP_GetBlockIdByPipe(ViPipe);

		if (s8BlkDev == -1) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "ISP[%d] Online Mode Pipe Err!\n", ViPipe);
			return GK_FAILURE;
		}

		u8BlockId = (GK_U8)s8BlkDev;

		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k == 0) {
				pVirtAddr[k] = VReg_GetVirtAddrBase(
					ISP_VIPROC_REG_BASE(u8BlockId));
			} else {
				pVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_OFFLINE:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k == 0) {
				pBeVirtAddr =
					ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
				pVirtAddr[k] =
					(GK_VOID *)((GK_U8 *)pBeVirtAddr +
						    VIPROC_OFFLINE_OFFSET);
			} else {
				pVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_SIDEBYSIDE:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			if (k < ISP_MAX_BE_NUM) {
				pVirtAddr[k] = VReg_GetVirtAddrBase(
					ISP_VIPROC_REG_BASE(k));
			} else {
				pVirtAddr[k] = GK_NULL;
			}
		}
		break;

	case ISP_MODE_RUNNING_STRIPING:
		for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
			pBeVirtAddr = ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
			pVirtAddr[k] = (GK_VOID *)((GK_U8 *)pBeVirtAddr +
						   VIPROC_OFFLINE_OFFSET);
		}
		break;

	default:
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] GetBe Running Mode Err!\n",
			  ViPipe);
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

GK_VOID *ISP_GetLdciReadSttVirAddr(VI_PIPE ViPipe, GK_U8 u8BufIdx)
{
	GK_S32 s32Ret;
	GK_U64 u64PhyAddrHigh, u64PhyAddrTmp;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LDCI_READ_STT_BUF_S *pstLdciReadSttBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->bLdciTprFltEn == GK_FALSE) {
		return GK_NULL;
	}

	LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

	if (pstLdciReadSttBuf->astReadBuf[u8BufIdx].pVirAddr != GK_NULL) {
		return pstLdciReadSttBuf->astReadBuf[u8BufIdx].pVirAddr;
	}

	u64PhyAddrHigh =
		(GK_U64)ext_system_ldci_read_stt_buffergh_addr_read(ViPipe);
	u64PhyAddrTmp =
		(GK_U64)ext_system_ldci_read_stt_buffer_low_addr_read(ViPipe);
	u64PhyAddrTmp |= (u64PhyAddrHigh << 32);

	s32Ret = ISP_UpdateLdciReadSttBufCtx(ViPipe, u64PhyAddrTmp);
	if (s32Ret != GK_SUCCESS) {
		return GK_NULL;
	}

	return pstLdciReadSttBuf->astReadBuf[u8BufIdx].pVirAddr;
}

GK_VOID *ISP_GetFeVirAddr(VI_PIPE ViPipe)
{
	ISP_CHECK_FE_PIPE(ViPipe);

	return VReg_GetVirtAddrBase(ISP_FE_REG_BASE(ViPipe));
}

GK_VOID *ISP_GetBeLut2SttVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev, GK_U8 U8BufId)
{
	GK_S32 s32Ret;
	GK_U64 u64Size;
	GK_U64 u64PhyAddrHigh, u64PhyAddrTemp;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_CHECK_FE_PIPE(ViPipe);
	ISP_CHECK_BE_DEV(BlkDev);
	ISP_GET_CTX(ViPipe, pstIspCtx);
	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_NULL;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);

	if (pstBeLutBuf->astLutSttBuf[BlkDev].pVirAddr != GK_NULL) {
		return (GK_VOID *)((GK_U8 *)pstBeLutBuf->astLutSttBuf[BlkDev]
					   .pVirAddr +
				   u64Size * U8BufId);
	}

	u64PhyAddrHigh =
		(GK_U64)ext_system_be_lut_stt_buffergh_addr_read(ViPipe);
	u64PhyAddrTemp =
		(GK_U64)ext_system_be_lut_stt_buffer_low_addr_read(ViPipe);
	u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

	s32Ret = ISP_UpdateBeLutSttBufCtx(ViPipe, u64PhyAddrTemp);

	if (s32Ret != GK_SUCCESS) {
		return GK_NULL;
	}

	return (GK_VOID *)((GK_U8 *)pstBeLutBuf->astLutSttBuf[BlkDev].pVirAddr +
			   u64Size * U8BufId);
}

GK_VOID *ISP_GetBeVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
	GK_S32 s32Ret;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	GK_VOID *pVirtAddr[ISP_STRIPING_MAX_NUM] = { GK_NULL };

	ISP_CHECK_FE_PIPE(ViPipe);
	ISP_CHECK_BE_DEV(BlkDev);
	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->pIspBeVirtAddr[BlkDev]) {
		return pstIspCtx->pIspBeVirtAddr[BlkDev];
	}

	s32Ret = ISP_GetBeVregCfgAddr(ViPipe, pVirtAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get Be CfgAddr Failed!\n",
			  ViPipe);
		return GK_NULL;
	}

	ISP_CHECK_NULLPTR(pVirtAddr[BlkDev]);

	return pVirtAddr[BlkDev];
}

GK_VOID *ISP_GetViProcVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
	GK_S32 s32Ret;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	GK_VOID *pVirtAddr[ISP_STRIPING_MAX_NUM] = { GK_NULL };

	ISP_CHECK_FE_PIPE(ViPipe);
	ISP_CHECK_BE_DEV(BlkDev);
	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->pViProcVirtAddr[BlkDev]) {
		return pstIspCtx->pViProcVirtAddr[BlkDev];
	}

	s32Ret = ISP_GetViProcCfgAddr(ViPipe, pVirtAddr);

	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] Get viproc CfgAddr Failed!\n", ViPipe);
		return GK_NULL;
	}

	ISP_CHECK_NULLPTR(pVirtAddr[BlkDev]);

	return pVirtAddr[BlkDev];
}

static GK_S32 isp_sharpen_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i,
					      GK_U8 u8BufId,
					      S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;

	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_sharpen_lut_addr_low_write(pstViProcReg,
				       GetLowAddr(u64PhyAddr +
						  ISP_SHARPEN_LUT_WSTT_OFFSET +
						  u8BufId * u64Size));

	return GK_SUCCESS;
}

static GK_S32 isp_dehaze_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i,
					     GK_U8 u8BufId,
					     S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;

	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_dehaze_lut_addr_low_write(pstViProcReg,
				      GetLowAddr(u64PhyAddr +
						 ISP_DEHAZE_LUT_WSTT_OFFSET +
						 u8BufId * u64Size));

	return GK_SUCCESS;
}

static GK_S32 isp_gamma_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i,
					    GK_U8 u8BufId,
					    S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;

	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_gamma_lut_addr_low_write(pstViProcReg,
				     GetLowAddr(u64PhyAddr +
						ISP_GAMMA_LUT_WSTT_OFFSET +
						u8BufId * u64Size));

	return GK_SUCCESS;
}

static GK_S32 isp_bnr_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i,
					  GK_U8 u8BufId,
					  S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;
	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);
	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;
	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}
	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_bnr_lut_addr_low_write(
		pstViProcReg, GetLowAddr(u64PhyAddr + ISP_BNR_LUT_WSTT_OFFSET +
					 u8BufId * u64Size));

	return GK_SUCCESS;
}

static GK_S32 isp_lsc_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i,
					  GK_U8 u8BufId,
					  S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;

	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_lsc_lut_addr_low_write(
		pstViProcReg, GetLowAddr(u64PhyAddr + ISP_LSC_LUT_WSTT_OFFSET +
					 u8BufId * u64Size));

	return GK_SUCCESS;
}

#ifdef CONFIG_ISP_CA_SUPPORT
static GK_S32 isp_ca_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i, GK_U8 u8BufId,
					 S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;

	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_ca_lut_addr_low_write(
		pstViProcReg, GetLowAddr(u64PhyAddr + ISP_CA_LUT_WSTT_OFFSET +
					 u8BufId * u64Size));

	return GK_SUCCESS;
}
#endif
static GK_S32 isp_ldci_lut_wstt_addr_write(VI_PIPE ViPipe, GK_U8 i,
					   GK_U8 u8BufId,
					   S_VIPROC_REGS_TYPE *pstViProcReg)
{
	GK_U64 u64PhyAddr, u64Size;
	ISP_BE_LUT_BUF_S *pstBeLutBuf = GK_NULL;

	BE_LUT_BUF_GET_CTX(ViPipe, pstBeLutBuf);

	u64PhyAddr = pstBeLutBuf->astLutSttBuf[i].u64PhyAddr;

	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	u64Size = sizeof(S_ISP_LUT_WSTT_TYPE);
	isp_ldci_lut_addr_low_write(
		pstViProcReg, GetLowAddr(u64PhyAddr + ISP_LDCI_LUT_WSTT_OFFSET +
					 u8BufId * u64Size));

	return GK_SUCCESS;
}

static GK_S32 ISP_FeDgRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U16 i;
	VI_PIPE ViPipeBind;
	ISP_FE_DG_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;

	ISP_CHECK_PIPE(ViPipe);
	ISP_GET_CTX(ViPipe, pstIspCtx);

	pstDynaRegCfg =
		&pstRegCfgInfo->stAlgRegCfg[0].stFeDgRegCfg.stDynaRegCfg;

	if (pstIspCtx->stWdrAttr.bMastPipe) {
		for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num;
		     i++) {
			ViPipeBind =
				pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
			ISP_CHECK_PIPE(ViPipeBind);
			pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(
				ViPipeBind);
			ISP_CHECK_POINTER(pstFeReg);

			if (pstRegCfgInfo->unKey.bit1FeDgCfg) {
				isp_fe_dg2_en_write(
					pstFeReg, pstRegCfgInfo->stAlgRegCfg[0]
							  .stFeDgRegCfg.bDgEn);

				if (pstDynaRegCfg->bResh) {
					isp_dg2_rgain_write(
						pstFeReg,
						pstDynaRegCfg->au16GainR[i]);
					isp_dg2_grgain_write(
						pstFeReg,
						pstDynaRegCfg->au16GainGR[i]);
					isp_dg2_gbgain_write(
						pstFeReg,
						pstDynaRegCfg->au16GainGB[i]);
					isp_dg2_bgain_write(
						pstFeReg,
						pstDynaRegCfg->au16GainB[i]);
					isp_dg2_clip_value_write(
						pstFeReg,
						pstDynaRegCfg->u32ClipValue);
				}
			}
		}

		pstDynaRegCfg->bResh = GK_FALSE;
		pstRegCfgInfo->unKey.bit1FeDgCfg = 0;
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_FeBlcRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U16 i;
	VI_PIPE ViPipeBind;
	isp_fe_blc_static_cfg *static_cfg = GK_NULL;
	isp_fe_blc_dyna_cfg *dyna_cfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;
	GK_U32 u32RegValue;
	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->stWdrAttr.bMastPipe != GK_TRUE) {
		return GK_SUCCESS;
	}

	static_cfg = &pstRegCfgInfo->stAlgRegCfg[0].stFeBlcCfg.static_blc;
	dyna_cfg = &pstRegCfgInfo->stAlgRegCfg[0].stFeBlcCfg.dyna_blc;

	for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num; i++) {
		ViPipeBind = pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
		ISP_CHECK_PIPE(ViPipeBind);
		pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
		ISP_CHECK_POINTER(pstFeReg);

		if (pstRegCfgInfo->unKey.bit1FeBlcCfg) {
			if (static_cfg->bReshStatic) {
				/* Fe Dg */
				isp_dg2_en_in_write(
					pstFeReg, static_cfg->stFeDgBlc.blc_in);
				isp_dg2_en_out_write(
					pstFeReg,
					static_cfg->stFeDgBlc.blc_out);
				/* Fe Wb */
				isp_wb1_en_in_write(
					pstFeReg, static_cfg->stFeWbBlc.blc_in);
				isp_wb1_en_out_write(
					pstFeReg,
					static_cfg->stFeWbBlc.blc_out);
				/* Fe Ae */
				isp_ae1_blc_en_write(
					pstFeReg, static_cfg->stFeAeBlc.blc_in);

				/* Fe RC */
				isp_rc_blc_in_en_write(
					pstFeReg, static_cfg->stRcBlc.blc_in);
				isp_rc_blc_out_en_write(
					pstFeReg, static_cfg->stRcBlc.blc_out);
				/* FE BLC */
				isp_fe_blc1_en_write(
					pstFeReg, static_cfg->stFeBlc.blc_in);
			}

			if (pstRegCfgInfo->stAlgRegCfg[0]
				    .stFeBlcCfg.resh_dyna_init == GK_TRUE) {
				/* FE BLC */
				u32RegValue = (dyna_cfg->stFeBlc.blc[0] << 16) +
					      dyna_cfg->stFeBlc.blc[1];
				isp_blc1_ofset1_write(pstFeReg, u32RegValue);
				u32RegValue = (dyna_cfg->stFeBlc.blc[3] << 16) +
					      dyna_cfg->stFeBlc.blc[2];
				isp_blc1_ofset2_write(pstFeReg, u32RegValue);

				/* Fe Dg */
				isp_dg2_ofsr_write(pstFeReg,
						   dyna_cfg->stFeDgBlc.blc[0]);
				isp_dg2_ofsgr_write(pstFeReg,
						    dyna_cfg->stFeDgBlc.blc[1]);
				isp_dg2_ofsgb_write(pstFeReg,
						    dyna_cfg->stFeDgBlc.blc[2]);
				isp_dg2_ofsb_write(pstFeReg,
						   dyna_cfg->stFeDgBlc.blc[3]);

				/* Fe WB */
				isp_wb1_ofsr_write(pstFeReg,
						   dyna_cfg->stFeWbBlc.blc[0]);
				isp_wb1_ofsgr_write(pstFeReg,
						    dyna_cfg->stFeWbBlc.blc[1]);
				isp_wb1_ofsgb_write(pstFeReg,
						    dyna_cfg->stFeWbBlc.blc[2]);
				isp_wb1_ofsb_write(pstFeReg,
						   dyna_cfg->stFeWbBlc.blc[3]);

				/* Fe AE */
				isp_ae1_offset_r_write(
					pstFeReg, dyna_cfg->stFeAeBlc.blc[0]);
				isp_ae1_offset_gr_write(
					pstFeReg, dyna_cfg->stFeAeBlc.blc[1]);
				isp_ae1_offset_gb_write(
					pstFeReg, dyna_cfg->stFeAeBlc.blc[2]);
				isp_ae1_offset_b_write(
					pstFeReg, dyna_cfg->stFeAeBlc.blc[3]);
				/* Fe Rc */
				isp_rc_blc_r_write(pstFeReg,
						   dyna_cfg->stRcBlc.blc[0]);
				isp_rc_blc_gr_write(pstFeReg,
						    dyna_cfg->stRcBlc.blc[1]);
				isp_rc_blc_gb_write(pstFeReg,
						    dyna_cfg->stRcBlc.blc[2]);
				isp_rc_blc_b_write(pstFeReg,
						   dyna_cfg->stRcBlc.blc[3]);
			}
		}
	}

	static_cfg->bReshStatic = GK_FALSE;
	pstRegCfgInfo->stAlgRegCfg[0].stFeBlcCfg.resh_dyna_init = GK_FALSE;
	pstRegCfgInfo->unKey.bit1FeBlcCfg = 0;

	return GK_SUCCESS;
}
static GK_S32 ISP_FeRcRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U16 i;
	VI_PIPE ViPipeBind;
	ISP_RC_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	pstUsrRegCfg = &pstRegCfgInfo->stAlgRegCfg[0].stRcRegCfg.stUsrRegCfg;

	if (pstIspCtx->stWdrAttr.bMastPipe) {
		for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num;
		     i++) {
			ViPipeBind =
				pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
			ISP_CHECK_PIPE(ViPipeBind);

			if (pstRegCfgInfo->unKey.bit1RcCfg) {
				pstFeReg =
					(S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(
						ViPipeBind);
				ISP_CHECK_POINTER(pstFeReg);

				isp_fe_rc_en_write(pstFeReg,
						   pstRegCfgInfo->stAlgRegCfg[0]
							   .stRcRegCfg.bRcEn);

				if (pstUsrRegCfg->bResh) {
					isp_rc_sqradius_write(
						pstFeReg,
						pstUsrRegCfg->u32SquareRadius);
					isp_rc_cenhor_coor_write(
						pstFeReg,
						pstUsrRegCfg->u16CenterHorCoor);
					isp_rc_cenver_coor_write(
						pstFeReg,
						pstUsrRegCfg->u16CenterVerCoor);
				}
			}
		}

		pstUsrRegCfg->bResh = GK_FALSE;
		pstRegCfgInfo->unKey.bit1RcCfg = 0;
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_FeAeRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_BOOL bLutUpdate = GK_FALSE;
	GK_U16 i, j, k;
	GK_U32 u32TableWeightTmp = 0;
	GK_U32 u32CombinWeight = 0;
	GK_U32 u32CombinWeightNum = 0;
	VI_PIPE ViPipeBind;
	ISP_AE_STATIC_CFG_S *pstStaticRegFeCfg = GK_NULL;
	ISP_AE_DYNA_CFG_S *pstDynaRegFeCfg = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->stWdrAttr.bMastPipe && pstRegCfgInfo->unKey.bit1AeCfg1) {
		for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num;
		     i++) {
			ViPipeBind =
				pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
			ISP_CHECK_PIPE(ViPipeBind);
			pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(
				ViPipeBind);
			ISP_CHECK_POINTER(pstFeReg);
			// ae fe static
			pstStaticRegFeCfg = &pstRegCfgInfo->stAlgRegCfg[0]
						     .stAeRegCfg.stStaticRegCfg;
			pstDynaRegFeCfg = &pstRegCfgInfo->stAlgRegCfg[0]
						   .stAeRegCfg.stDynaRegCfg;

			isp_fe_ae1_en_write(pstFeReg,
					    pstStaticRegFeCfg->u8FEEnable);
			isp_ae1_crop_pos_x_write(
				pstFeReg, pstStaticRegFeCfg->u16FECropPosX);
			isp_ae1_crop_pos_y_write(
				pstFeReg, pstStaticRegFeCfg->u16FECropPosY);
			isp_ae1_crop_out_width_write(
				pstFeReg,
				pstStaticRegFeCfg->u16FECropOutWidth - 1);
			isp_ae1_crop_out_height_write(
				pstFeReg,
				pstStaticRegFeCfg->u16FECropOutHeight - 1);

			// ae fe dynamic
			isp_ae1_hnum_write(
				pstFeReg,
				pstDynaRegFeCfg->u8FEWeightTableWidth);
			isp_ae1_vnum_write(
				pstFeReg,
				pstDynaRegFeCfg->u8FEWeightTableHeight);
			isp_ae1_skip_x_write(pstFeReg,
					     pstDynaRegFeCfg->u8FEHistSkipX);
			isp_ae1_offset_x_write(
				pstFeReg, pstDynaRegFeCfg->u8FEHistOffsetX);
			isp_ae1_skip_y_write(pstFeReg,
					     pstDynaRegFeCfg->u8FEHistSkipY);
			isp_ae1_offset_y_write(
				pstFeReg, pstDynaRegFeCfg->u8FEHistOffsetY);
			isp_ae1_bitmove_write(pstFeReg,
					      pstDynaRegFeCfg->u8FEBitMove);

			u32CombinWeight = 0;
			u32CombinWeightNum = 0;

			isp_ae1_wei_waddr_write(pstFeReg, 0);

			for (j = 0; j < AE_ZONE_ROW; j++) {
				for (k = 0; k < AE_ZONE_COLUMN; k++) {
					u32TableWeightTmp =
						(GK_U32)pstDynaRegFeCfg
							->au8FEWeightTable[j][k];
					u32CombinWeight |=
						(u32TableWeightTmp
						 << (8 * u32CombinWeightNum));
					u32CombinWeightNum++;

					if (u32CombinWeightNum ==
					    ISP_AE_WEI_COMBIN_COUNT) {
						isp_ae1_wei_wdata_write(
							pstFeReg,
							u32CombinWeight);
						u32CombinWeightNum = 0;
						u32CombinWeight = 0;
					}
				}
			}

			if ((u32CombinWeightNum != ISP_AE_WEI_COMBIN_COUNT) &&
			    (u32CombinWeightNum != 0)) {
				isp_ae1_wei_wdata_write(pstFeReg,
							u32CombinWeight);
			}

			bLutUpdate = pstDynaRegFeCfg->u8FEWightTableUpdate;
		}
	}

	pstRegCfgInfo->stAlgRegCfg[0].stFeLutUpdateCfg.bAe1LutUpdate =
		bLutUpdate;
	return GK_SUCCESS;
}

static GK_S32 ISP_AeRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			      GK_U8 i)
{
	GK_BOOL bLutUpdate = GK_FALSE;
	GK_BOOL bIsOfflineMode;
	GK_U16 j, k, m;
	GK_U32 u32CombinWeight = 0;
	GK_U32 u32CombinWeightNum = 0;
	ISP_AE_STATIC_CFG_S *pstStaticRegBeCfg = GK_NULL;
	ISP_AE_DYNA_CFG_S *pstDynaRegBeCfg = GK_NULL;
	ISP_MG_STATIC_CFG_S *pstMgStaticRegCfg = GK_NULL;
	ISP_MG_DYNA_CFG_S *pstMgDynaRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	GK_U32 au32CombineWgt[64] = { 0 };

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1AeCfg1) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		// ae be static
		pstStaticRegBeCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stAeRegCfg.stStaticRegCfg;
		pstDynaRegBeCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stAeRegCfg.stDynaRegCfg;

		isp_ae_crop_pos_x_write(pstBeReg,
					pstStaticRegBeCfg->u16BECropPosX);
		isp_ae_crop_pos_y_write(pstBeReg,
					pstStaticRegBeCfg->u16BECropPosY);
		isp_ae_crop_out_width_write(
			pstBeReg, pstStaticRegBeCfg->u16BECropOutWidth - 1);
		isp_ae_crop_out_height_write(
			pstBeReg, pstStaticRegBeCfg->u16BECropOutHeight - 1);
		// ae be dynamic
		isp_ae_sel_write(pstBeReg, pstDynaRegBeCfg->u8BEAESel);
		isp_ae_hnum_write(pstBeReg,
				  pstDynaRegBeCfg->u8BEWeightTableWidth);
		isp_ae_vnum_write(pstBeReg,
				  pstDynaRegBeCfg->u8BEWeightTableHeight);
		isp_ae_skip_x_write(pstBeReg, pstDynaRegBeCfg->u8BEHistSkipX);
		isp_ae_offset_x_write(pstBeReg,
				      pstDynaRegBeCfg->u8BEHistOffsetX);
		isp_ae_skip_y_write(pstBeReg, pstDynaRegBeCfg->u8BEHistSkipY);
		isp_ae_offset_y_write(pstBeReg,
				      pstDynaRegBeCfg->u8BEHistOffsetY);
		isp_ae_bitmove_write(pstBeReg, pstDynaRegBeCfg->u8BEBitMove);
		isp_aest_gamma_mode_write(pstBeReg,
					  pstDynaRegBeCfg->u8BEHistGammaMode);
		isp_ae_aver_gamma_mode_write(
			pstBeReg, pstDynaRegBeCfg->u8BEAverGammaMode);
		isp_ae_gamma_limit_write(pstBeReg,
					 pstDynaRegBeCfg->u8BEGammaLimit);
		isp_ae_four_plane_mode_write(
			pstBeReg, pstDynaRegBeCfg->u8BEFourPlaneMode);

		m = 0;
		u32CombinWeight = 0;
		u32CombinWeightNum = 0;

		for (j = 0; j < pstDynaRegBeCfg->u8BEWeightTableHeight; j++) {
			for (k = 0; k < pstDynaRegBeCfg->u8BEWeightTableWidth;
			     k++) {
				u32CombinWeight |=
					((GK_U32)pstDynaRegBeCfg
						 ->au8BEWeightTable[j][k]
					 << (8 * u32CombinWeightNum));
				u32CombinWeightNum++;

				if (u32CombinWeightNum ==
				    ISP_AE_WEI_COMBIN_COUNT) {
					au32CombineWgt[m++] = u32CombinWeight;
					u32CombinWeightNum = 0;
					u32CombinWeight = 0;
				}
			}
		}

		if (u32CombinWeightNum != ISP_AE_WEI_COMBIN_COUNT &&
		    u32CombinWeightNum != 0) {
			au32CombineWgt[m++] = u32CombinWeight;
		}

		if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		    IS_SIDEBYSIDE_MODE(
			    pstIspCtx->stBlockAttr.enIspRunningMode)) {
			isp_ae_weight_waddr_u32_write(pstBeReg, 0);

			for (m = 0; m < 64; m++) {
				isp_ae_weight_wdata_u32_write(
					pstBeReg, au32CombineWgt[m]);
			}
		} else {
			isp_ae_weight_write(pstBeReg, au32CombineWgt);
		}

		bLutUpdate = pstDynaRegBeCfg->u8BEWightTableUpdate;

		/* mg static */
		pstMgStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stMgRegCfg.stStaticRegCfg;
		pstMgDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stMgRegCfg.stDynaRegCfg;

		isp_la_crop_pos_x_write(pstBeReg,
					pstMgStaticRegCfg->u16CropPosX);
		isp_la_crop_pos_y_write(pstBeReg,
					pstMgStaticRegCfg->u16CropPosY);
		isp_la_crop_out_width_write(
			pstBeReg, pstMgStaticRegCfg->u16CropOutWidth - 1);
		isp_la_crop_out_height_write(
			pstBeReg, pstMgStaticRegCfg->u16CropOutHeight - 1);

		/* mg dynamic */
		isp_la_hnum_write(pstBeReg, pstMgDynaRegCfg->u8ZoneWidth);
		isp_la_vnum_write(pstBeReg, pstMgDynaRegCfg->u8ZoneHeight);
		isp_la_bitmove_write(pstBeReg, pstMgDynaRegCfg->u8BitMove);
		isp_la_gamma_en_write(pstBeReg, pstMgDynaRegCfg->u8GammaMode);
		isp_la_gamma_limit_write(pstBeReg,
					 pstMgDynaRegCfg->u8GammaLimit);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stBeLutUpdateCfg.bAeLutUpdate =
		bLutUpdate | bIsOfflineMode;

	return GK_SUCCESS;
}

static GK_S32 ISP_AfRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			      GK_U8 i)
{
#ifdef CONFIG_ISP_AF_SUPPORT
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh;
	GK_BOOL bIdxResh;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	ISP_AF_REG_CFG_S *pstAfRegBeCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	pstAfRegBeCfg = &pstRegCfgInfo->stAlgRegCfg[i].stBEAfRegCfg;
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);

	bIdxResh = (isp_af_update_index_read(pstBeReg) !=
		    pstAfRegBeCfg->u32UpdateIndex);
	bUsrResh = (bIsOfflineMode) ?
			   (pstRegCfgInfo->unKey.bit1AfBeCfg & bIdxResh) :
			   (pstRegCfgInfo->unKey.bit1AfBeCfg);

	if (bUsrResh) {
		isp_af_update_index_write(pstBeReg,
					  pstAfRegBeCfg->u32UpdateIndex);

		isp_af_iirgain_u32_write(pstBeReg,
					 pstAfRegBeCfg->u16IirGain0Low,
					 pstAfRegBeCfg->u16IirGain0High,
					 pstAfRegBeCfg->u16IirGain1Low,
					 pstAfRegBeCfg->u16IirGain1High);

		isp_af_shift_u32_write(pstBeReg,
				       pstAfRegBeCfg->u16Iir0ShiftGroup0,
				       pstAfRegBeCfg->u16Iir1ShiftGroup0,
				       pstAfRegBeCfg->u16Iir2ShiftGroup0,
				       pstAfRegBeCfg->u16Iir3ShiftGroup0,
				       pstAfRegBeCfg->u16Iir0ShiftGroup1,
				       pstAfRegBeCfg->u16Iir1ShiftGroup1,
				       pstAfRegBeCfg->u16Iir2ShiftGroup1,
				       pstAfRegBeCfg->u16Iir3ShiftGroup1);

		isp_af_firslope_coring_u32_write(
			pstBeReg, pstAfRegBeCfg->u16FirSlope0Coring,
			pstAfRegBeCfg->u16FirSlope1Coring);
		isp_af_firgain_u32_write(pstBeReg,
					 pstAfRegBeCfg->u16FirGain0Low,
					 pstAfRegBeCfg->u16FirGain0High,
					 pstAfRegBeCfg->u16FirGain1Low,
					 pstAfRegBeCfg->u16FirGain1High);
		isp_af_firh4_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16FirHGain4Group0,
			(GK_U32)pstAfRegBeCfg->s16FirHGain4Group1);
		isp_af_firthre_u32_write(pstBeReg,
					 pstAfRegBeCfg->u16FirThre0Low,
					 pstAfRegBeCfg->u16FirThre0High,
					 pstAfRegBeCfg->u16FirThre1Low,
					 pstAfRegBeCfg->u16FirThre1High);
		isp_af_firh0_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16FirHGain0Group0,
			(GK_U32)pstAfRegBeCfg->s16FirHGain0Group1);
		isp_af_firh1_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16FirHGain1Group0,
			(GK_U32)pstAfRegBeCfg->s16FirHGain1Group1);
		isp_af_firh2_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16FirHGain2Group0,
			(GK_U32)pstAfRegBeCfg->s16FirHGain2Group1);
		isp_af_firh3_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16FirHGain3Group0,
			(GK_U32)pstAfRegBeCfg->s16FirHGain3Group1);
		isp_af_firslope_u32_write(pstBeReg,
					  pstAfRegBeCfg->u16FirSlope0Low,
					  pstAfRegBeCfg->u16FirSlope0High,
					  pstAfRegBeCfg->u16FirSlope1Low,
					  pstAfRegBeCfg->u16FirSlope1High);
		isp_af_iirg5_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain5Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain5Group1);
		isp_af_iirslope_u32_write(pstBeReg,
					  pstAfRegBeCfg->u16IirSlope0Low,
					  pstAfRegBeCfg->u16IirSlope0High,
					  pstAfRegBeCfg->u16IirSlope1Low,
					  pstAfRegBeCfg->u16IirSlope1High);
		isp_af_iirg2_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain2Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain2Group1);
		isp_af_iirg3_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain3Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain3Group1);
		isp_af_iirg0_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain0Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain0Group1);
		isp_af_iirg1_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain1Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain1Group1);
		isp_af_iirg6_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain6Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain6Group1);
		isp_af_iirg4_u32_write(
			pstBeReg, (GK_U32)pstAfRegBeCfg->s16IirGain4Group0,
			(GK_U32)pstAfRegBeCfg->s16IirGain4Group1);

		isp_af_cfg_u32_write(
			pstBeReg, pstAfRegBeCfg->bIir0Enable0,
			pstAfRegBeCfg->bIir0Enable1,
			pstAfRegBeCfg->bIir0Enable2,
			pstAfRegBeCfg->bIir1Enable0,
			pstAfRegBeCfg->bIir1Enable1,
			pstAfRegBeCfg->bIir1Enable2, pstAfRegBeCfg->enPeakMode,
			pstAfRegBeCfg->enSquMode, pstAfRegBeCfg->bOffsetEnable,
			pstAfRegBeCfg->bCropEnable, pstAfRegBeCfg->bLpfEnable,
			pstAfRegBeCfg->bMeanEnable, pstAfRegBeCfg->bRawMode,
			pstAfRegBeCfg->u8BayerMode,
			pstAfRegBeCfg->bIir0DsEnable,
			pstAfRegBeCfg->bIir1DsEnable,
			pstAfRegBeCfg->bFir0LpfEnable,
			pstAfRegBeCfg->bFir1LpfEnable,
			pstAfRegBeCfg->bIir0LdgEnable,
			pstAfRegBeCfg->bIir1LdgEnable,
			pstAfRegBeCfg->bFir0LdgEnable,
			pstAfRegBeCfg->bFir1LdgEnable, pstAfRegBeCfg->u8Gamma);

		isp_af_iirpeak_coring_u32_write(
			pstBeReg, pstAfRegBeCfg->u16IirPeak0Coring,
			pstAfRegBeCfg->u16IirPeak1Coring);
		isp_af_mean_thres_u32_write(
			pstBeReg, 0xffff - pstAfRegBeCfg->u16MeanThres);
		isp_af_crop_start_u32_write(pstBeReg,
					    pstAfRegBeCfg->u16CropPosX,
					    pstAfRegBeCfg->u16CropPosY);
		isp_af_firthre_coring_u32_write(
			pstBeReg, pstAfRegBeCfg->u16FirThre0Coring,
			pstAfRegBeCfg->u16FirThre1Coring);
		isp_af_iirpl_u32_write(pstBeReg, pstAfRegBeCfg->u8IirPlgGroup0,
				       pstAfRegBeCfg->u8IirPlsGroup0,
				       pstAfRegBeCfg->u8IirPlgGroup1,
				       pstAfRegBeCfg->u8IirPlsGroup1);
		isp_af_iirthre_u32_write(pstBeReg,
					 pstAfRegBeCfg->u16IirThre0Low,
					 pstAfRegBeCfg->u16IirThre0High,
					 pstAfRegBeCfg->u16IirThre1Low,
					 pstAfRegBeCfg->u16IirThre1High);
		isp_af_iirslope_coring_u32_write(
			pstBeReg, pstAfRegBeCfg->u16IirSlope0Coring,
			pstAfRegBeCfg->u16IirSlope1Coring);
		isp_af_acc_shift_u32_write(pstBeReg,
					   pstAfRegBeCfg->u16AccShift0H,
					   pstAfRegBeCfg->u16AccShift1H,
					   pstAfRegBeCfg->u16AccShift0V,
					   pstAfRegBeCfg->u16AccShift1V,
					   pstAfRegBeCfg->u16AccShiftY);
		isp_af_iirthre_coring_u32_write(
			pstBeReg, pstAfRegBeCfg->u16IirThre0Coring,
			pstAfRegBeCfg->u16IirThre1Coring);
		isp_af_offset_u32_write(pstBeReg, pstAfRegBeCfg->u16OffsetGr,
					pstAfRegBeCfg->u16OffsetGb);
		isp_af_cnt_shift_u32_write(pstBeReg, 0x0, 0x0,
					   ISP_AF_CNT_SHIFT0_V_DEFAULT, 0x0,
					   pstAfRegBeCfg->u16ShiftCountY);
		isp_af_firpeak_coring_u32_write(
			pstBeReg, pstAfRegBeCfg->u16FirPeak0Coring,
			pstAfRegBeCfg->u16FirPeak1Coring);
		isp_aflight_u32_write(pstBeReg, pstAfRegBeCfg->u8HilighThre);
		isp_af_crop_size_u32_write(pstBeReg,
					   pstAfRegBeCfg->u16CropHsize - 1,
					   pstAfRegBeCfg->u16CropVsize - 1);
		isp_af_iirdilate_u32_write(pstBeReg,
					   pstAfRegBeCfg->u8IirDilate0,
					   pstAfRegBeCfg->u8IirDilate1);
		isp_af_zone_u32_write(pstBeReg, pstAfRegBeCfg->u16WindowHnum,
				      pstAfRegBeCfg->u16WindowVnum);

		isp_af_sel_write(pstBeReg, pstAfRegBeCfg->u8AfPosSel);

		pstRegCfgInfo->unKey.bit1AfBeCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}
#endif

	return GK_SUCCESS;
}

static GK_S32 ISP_FeAwbRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U32 i;
	VI_PIPE ViPipeBind;
	ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = GK_NULL;
	ISP_AWB_REG_STA_CFG_S *pstAwbRegStaCfg = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->stWdrAttr.bMastPipe) {
		for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num;
		     i++) {
			ViPipeBind =
				pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
			ISP_CHECK_PIPE(ViPipeBind);
			pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(
				ViPipeBind);
			ISP_CHECK_POINTER(pstFeReg);

			if (pstIspCtx->stLinkage.bSnapState == GK_TRUE) {
				isp_wb1_rgain_write(
					pstFeReg,
					pstIspCtx->stSnapIspInfo.stIspCfgInfo
						.au32WhiteBalanceGain[0]);
				isp_wb1_grgain_write(
					pstFeReg,
					pstIspCtx->stSnapIspInfo.stIspCfgInfo
						.au32WhiteBalanceGain[1]);
				isp_wb1_gbgain_write(
					pstFeReg,
					pstIspCtx->stSnapIspInfo.stIspCfgInfo
						.au32WhiteBalanceGain[2]);
				isp_wb1_bgain_write(
					pstFeReg,
					pstIspCtx->stSnapIspInfo.stIspCfgInfo
						.au32WhiteBalanceGain[3]);
			}

			if (pstRegCfgInfo->unKey.bit1AwbDynCfg) {
				pstAwbRegDynCfg =
					&pstRegCfgInfo->stAlgRegCfg[0]
						 .stAwbRegCfg.stAwbRegDynCfg;
				isp_wb1_rgain_write(
					pstFeReg,
					pstAwbRegDynCfg
						->au32FEWhiteBalanceGain[0]);
				isp_wb1_grgain_write(
					pstFeReg,
					pstAwbRegDynCfg
						->au32FEWhiteBalanceGain[1]);
				isp_wb1_gbgain_write(
					pstFeReg,
					pstAwbRegDynCfg
						->au32FEWhiteBalanceGain[2]);
				isp_wb1_bgain_write(
					pstFeReg,
					pstAwbRegDynCfg
						->au32FEWhiteBalanceGain[3]);
				isp_fe_wb1_en_write(
					pstFeReg,
					pstAwbRegDynCfg->u8FEWbWorkEn);
			}

			pstAwbRegStaCfg = &pstRegCfgInfo->stAlgRegCfg[0]
						   .stAwbRegCfg.stAwbRegStaCfg;

			if (pstAwbRegStaCfg->bFEAwbStaCfg) {
				pstAwbRegStaCfg =
					&pstRegCfgInfo->stAlgRegCfg[0]
						 .stAwbRegCfg.stAwbRegStaCfg;
				isp_wb1_clip_value_write(
					pstFeReg,
					pstAwbRegStaCfg->u32FEClipValue);
			}
		}
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_AwbCCSet(S_ISPBE_REGS_TYPE *pstBeReg, GK_U16 *pu16BeCC)
{
	isp_cc_coef1_u32_write(pstBeReg,
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[2])),
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[3])));
	isp_cc_coef0_u32_write(pstBeReg,
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[0])),
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[1])));
	isp_cc_coef3_u32_write(pstBeReg,
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[6])),
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[7])));
	isp_cc_coef2_u32_write(pstBeReg,
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[4])),
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[5])));
	isp_cc_coef4_u32_write(pstBeReg,
			       CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[8])));
	return GK_SUCCESS;
}

static GK_S32 ISP_AwbGainSet(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 *pu32BeWBGain)
{
	isp_wb_gain2_u32_write(pstBeReg, pu32BeWBGain[2], pu32BeWBGain[3]);
	isp_wb_gain1_u32_write(pstBeReg, pu32BeWBGain[1], pu32BeWBGain[0]);
	return GK_SUCCESS;
}

static GK_S32 ISP_AwbRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = GK_NULL;
	ISP_AWB_REG_STA_CFG_S *pstAwbRegStaCfg = GK_NULL;
	ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeRegEx = GK_NULL;
	GK_BOOL bIsOfflineMode;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	GK_BOOL bIdxResh, bUsrResh;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);
	pstAwbRegDynCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].stAwbRegCfg.stAwbRegDynCfg;
	pstAwbRegStaCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].stAwbRegCfg.stAwbRegStaCfg;
	pstAwbRegUsrCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].stAwbRegCfg.stAwbRegUsrCfg;

	if (pstIspCtx->stLinkage.bSnapState == GK_TRUE) {
		if (pstIspCtx->stLinkage.bLoadCCM == GK_TRUE) {
			ISP_AwbCCSet(pstBeReg,
				     pstIspCtx->stSnapIspInfo.stIspCfgInfo
					     .au16CapCCM);
		} else {
			ISP_AwbCCSet(pstBeReg,
				     pstAwbRegDynCfg->au16BEColorMatrix);
		}

		ISP_AwbGainSet(pstBeReg, pstIspCtx->stSnapIspInfo.stIspCfgInfo
						 .au32WhiteBalanceGain);
	}

	if (pstRegCfgInfo->unKey.bit1AwbDynCfg) {
		if (pstIspCtx->stLinkage.bSnapState != GK_TRUE) {
			if (pstIspCtx->stLinkage.enSnapPipeMode !=
			    ISP_SNAP_PICTURE) {
				ISP_AwbCCSet(
					pstBeReg,
					pstAwbRegDynCfg->au16BEColorMatrix);
				ISP_AwbGainSet(
					pstBeReg,
					pstAwbRegDynCfg->au32BEWhiteBalanceGain);
			}
		}

		if ((IS_ONLINE_MODE(pstIspCtx->stLinkage.enPictureRunningMode) ||
		     IS_SIDEBYSIDE_MODE(
			     pstIspCtx->stLinkage.enPictureRunningMode)) &&
		    (pstIspCtx->stLinkage.enSnapPipeMode == ISP_SNAP_PREVIEW)) {
			ISP_CHECK_PIPE(pstIspCtx->stLinkage.s32PicturePipeId);
			pstBeRegEx = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(
				pstIspCtx->stLinkage.s32PicturePipeId, i);
			ISP_CHECK_POINTER(pstBeRegEx);
			ISP_AwbCCSet(pstBeRegEx,
				     pstAwbRegDynCfg->au16BEColorMatrix);
			ISP_AwbGainSet(pstBeRegEx,
				       pstAwbRegDynCfg->au32BEWhiteBalanceGain);
		}
		isp_cc_colortone_g_gain_u32_write(
			pstBeReg, pstAwbRegDynCfg->u16BECcGGain);
		isp_awb_thd_max_u32_write(
			pstBeReg, pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb);
		isp_awb_cb_mm_u32_write(
			pstBeReg, pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb,
			pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb);
		isp_awb_crop_outsize_u32_write(
			pstBeReg, pstAwbRegDynCfg->u32BECropOutWidth - 1,
			pstAwbRegDynCfg->u32BECropOutHeight - 1);
		isp_cc_colortone_rb_gain_u32_write(
			pstBeReg, pstAwbRegDynCfg->u16BECcBGain,
			pstAwbRegDynCfg->u16BECcRGain);
		isp_awb_thd_min_u32_write(
			pstBeReg, pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb);
		isp_awb_cr_mm_u32_write(
			pstBeReg, pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb,
			pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb);
		isp_awb_crop_pos_u32_write(pstBeReg,
					   pstAwbRegDynCfg->u32BECropPosX,
					   pstAwbRegDynCfg->u32BECropPosY);
	}

	if (pstAwbRegStaCfg->bBEAwbStaCfg) {
		isp_wb_clip_value_u32_write(pstBeReg,
					    pstAwbRegStaCfg->u32BEWbClipValue);
		isp_awb_stat_raddr_u32_write(
			pstBeReg, pstAwbRegStaCfg->u32BEAwbStatRaddr);
		isp_cc_out_dc2_u32_write(pstBeReg,
					 pstAwbRegStaCfg->u32BECcOutDc2);
		isp_awb_offset_comp_u32_write(
			pstBeReg, pstAwbRegStaCfg->u16BEAwbOffsetComp);
		isp_cc_lumafact_u32_write(pstBeReg, ISP_CCM_LUMA_COEFR_DEFAULT,
					  ISP_CCM_LUMA_COEFB_DEFAULT,
					  ISP_CCM_LUMA_COEFR_UP_DEFAULT,
					  ISP_CCM_LUMA_COEFB_UP_DEFAULT);
		isp_cc_out_dc0_u32_write(pstBeReg,
					 pstAwbRegStaCfg->u32BECcOutDc0);
		isp_cc_rcv_ctrl0_u32_write(pstBeReg,
					   ISP_CCM_SOFT_CLIP0_STEP_DEFAULT,
					   ISP_CCM_SOFT_CLIP1_STEP_DEFAULT);
		isp_cc_rcv_ctrl1_u32_write(pstBeReg, ISP_CCM_DARKPREV_DEFAULT,
					   ISP_CCM_PEAKSUPP_SAT_DEFAULT,
					   ISP_CCM_PEAKSUPP_MAX_DEFAULT);
		isp_awb_bitmove_u32_write(pstBeReg,
					  pstAwbRegStaCfg->u8BEAwbBitmove);
		isp_cc_in_dc0_u32_write(pstBeReg,
					pstAwbRegStaCfg->u32BECcInDc0);
		isp_cc_in_dc1_u32_write(pstBeReg,
					pstAwbRegStaCfg->u32BECcInDc1);
		isp_cc_in_dc2_u32_write(pstBeReg,
					pstAwbRegStaCfg->u32BECcInDc2);
		isp_cc_out_dc1_u32_write(pstBeReg,
					 pstAwbRegStaCfg->u32BECcOutDc1);

		isp_cc_recover_en_write(pstBeReg, ISP_CCM_RECOVER_EN_DEFAULT);

		pstAwbRegStaCfg->bBEAwbStaCfg = 0;
	}

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	bIdxResh = (isp_awb_update_index_read(pstBeReg) !=
		    pstAwbRegUsrCfg->u32UpdateIndex);
	bUsrResh = (bIsOfflineMode) ? (pstAwbRegUsrCfg->bResh & bIdxResh) :
				      (pstAwbRegUsrCfg->bResh);

	if (bUsrResh) {
		isp_awb_update_index_write(pstBeReg,
					   pstAwbRegUsrCfg->u32UpdateIndex);
		isp_awb_zone_u32_write(pstBeReg, pstAwbRegUsrCfg->u16BEZoneCol,
				       pstAwbRegUsrCfg->u16BEZoneRow);

		isp_awb_sel_write(pstBeReg, pstAwbRegUsrCfg->enBEAWBSwitch);
		pstAwbRegUsrCfg->bResh =
			bIsOfflineMode; // if online mode, bResh=0; if offline mode, bResh=1; but only index != will resh
	}

	return GK_SUCCESS;
}

static GK_S32
ISP_SharpenDynaRegConfig(S_ISPBE_REGS_TYPE *pstBeReg,
			 ISP_SHARPEN_MPI_DYNA_REG_CFG_S *pstMpiDynaRegCfg,
			 ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstDefDynaRegCfg)
{
	isp_sharpen_lumawgt3_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[15],
				       pstMpiDynaRegCfg->au8LumaWgt[16],
				       pstMpiDynaRegCfg->au8LumaWgt[17],
				       pstMpiDynaRegCfg->au8LumaWgt[18],
				       pstMpiDynaRegCfg->au8LumaWgt[19]);

	isp_sharpen_lumawgt4_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[20],
				       pstMpiDynaRegCfg->au8LumaWgt[21],
				       pstMpiDynaRegCfg->au8LumaWgt[22],
				       pstMpiDynaRegCfg->au8LumaWgt[23],
				       pstMpiDynaRegCfg->au8LumaWgt[24]);

	isp_sharpen_lumawgt5_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[25],
				       pstMpiDynaRegCfg->au8LumaWgt[26],
				       pstMpiDynaRegCfg->au8LumaWgt[27],
				       pstMpiDynaRegCfg->au8LumaWgt[28],
				       pstMpiDynaRegCfg->au8LumaWgt[29]);

	isp_sharpen_lumawgt2_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[10],
				       pstMpiDynaRegCfg->au8LumaWgt[11],
				       pstMpiDynaRegCfg->au8LumaWgt[12],
				       pstMpiDynaRegCfg->au8LumaWgt[13],
				       pstMpiDynaRegCfg->au8LumaWgt[14]);

	isp_sharpen_lumawgt0_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[0],
				       pstMpiDynaRegCfg->au8LumaWgt[1],
				       pstMpiDynaRegCfg->au8LumaWgt[2],
				       pstMpiDynaRegCfg->au8LumaWgt[3],
				       pstMpiDynaRegCfg->au8LumaWgt[4]);

	isp_sharpen_lumawgt1_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[5],
				       pstMpiDynaRegCfg->au8LumaWgt[6],
				       pstMpiDynaRegCfg->au8LumaWgt[7],
				       pstMpiDynaRegCfg->au8LumaWgt[8],
				       pstMpiDynaRegCfg->au8LumaWgt[9]);

	isp_sharpen_lumawgt6_u32_write(pstBeReg,
				       pstMpiDynaRegCfg->au8LumaWgt[30],
				       pstMpiDynaRegCfg->au8LumaWgt[31]);
	isp_sharpen_detail_mul_u32_write(pstBeReg,
					 pstMpiDynaRegCfg->s16detailOshtMul,
					 pstMpiDynaRegCfg->s16detailUshtMul);
	isp_sharpen_shoot_maxgain_u32_write(pstBeReg,
					    pstMpiDynaRegCfg->u16oMaxGain,
					    pstMpiDynaRegCfg->u16uMaxGain);
	isp_sharpen_skin_v_u32_write(pstBeReg, pstMpiDynaRegCfg->u8skinMinV,
				     pstMpiDynaRegCfg->u8skinMaxV);
	isp_sharpen_skin_u_u32_write(pstBeReg, pstMpiDynaRegCfg->u8skinMinU,
				     pstMpiDynaRegCfg->u8skinMaxU);
	isp_sharpen_chrg_mul_u32_write(pstBeReg, pstMpiDynaRegCfg->s16chrGMul,
				       pstMpiDynaRegCfg->s16chrGmfMul);
	isp_sharpen_chrg_gain_u32_write(pstBeReg, pstMpiDynaRegCfg->u16chrGGain,
					pstMpiDynaRegCfg->u16chrGmfGain);

	isp_sharpen_chrbgain_write(pstBeReg, pstMpiDynaRegCfg->u8chrBGain);
	isp_sharpen_chrbmul_write(pstBeReg, pstMpiDynaRegCfg->s16chrBMul);
	isp_sharpen_chrrgain_write(pstBeReg, pstMpiDynaRegCfg->u8chrRGain);
	isp_sharpen_chrrmul_write(pstBeReg, pstMpiDynaRegCfg->s16chrRMul);
	isp_sharpen_benchrctrl_write(pstBeReg, pstMpiDynaRegCfg->u8bEnChrCtrl);
	isp_sharpen_bendetailctrl_write(pstBeReg,
					pstMpiDynaRegCfg->bEnDetailCtrl);
	isp_sharpen_benlumactrl_write(pstBeReg,
				      pstMpiDynaRegCfg->u8bEnLumaCtrl);
	isp_sharpen_benshtctrlbyvar_write(pstBeReg,
					  pstMpiDynaRegCfg->u8bEnShtCtrlByVar);
	isp_sharpen_benskinctrl_write(pstBeReg,
				      pstMpiDynaRegCfg->u8bEnSkinCtrl);
	isp_sharpen_weakdetailadj_write(pstBeReg,
					pstMpiDynaRegCfg->bEnWeakDetailAdj);
	isp_sharpen_dirdiffsft_write(pstBeReg, pstMpiDynaRegCfg->u8dirDiffSft);
	isp_sharpen_dirrt0_write(pstBeReg, pstMpiDynaRegCfg->u8dirRt[0]);
	isp_sharpen_dirrt1_write(pstBeReg, pstMpiDynaRegCfg->u8dirRt[1]);
	isp_sharpen_osht_dtl_thd0_write(pstBeReg,
					pstMpiDynaRegCfg->u8detailOshtThr[0]);
	isp_sharpen_osht_dtl_thd1_write(pstBeReg,
					pstMpiDynaRegCfg->u8detailOshtThr[1]);
	isp_sharpen_osht_dtl_wgt_write(pstBeReg,
				       pstMpiDynaRegCfg->u8detailOshtAmt);
	isp_sharpen_shtvarthd1_write(pstBeReg, pstMpiDynaRegCfg->u8shtVarThd1);
	isp_sharpen_oshtamt_write(pstBeReg, pstMpiDynaRegCfg->u8oshtAmt);
	isp_sharpen_ushtamt_write(pstBeReg, pstMpiDynaRegCfg->u8ushtAmt);
	isp_sharpen_shtbldrt_write(pstBeReg, pstMpiDynaRegCfg->u8shtBldRt);
	isp_sharpen_shtvarmul_write(pstBeReg, pstMpiDynaRegCfg->u16shtVarMul);
	isp_sharpen_skinedgemul_write(pstBeReg,
				      pstMpiDynaRegCfg->s16skinEdgeMul);
	isp_sharpen_skinedgewgt0_write(pstBeReg,
				       pstMpiDynaRegCfg->u8skinEdgeWgt[0]);
	isp_sharpen_skinedgewgt1_write(pstBeReg,
				       pstMpiDynaRegCfg->u8skinEdgeWgt[1]);
	isp_sharpen_usht_dtl_thd0_write(pstBeReg,
					pstMpiDynaRegCfg->u8detailUshtThr[0]);
	isp_sharpen_usht_dtl_thd1_write(pstBeReg,
					pstMpiDynaRegCfg->u8detailUshtThr[1]);
	isp_sharpen_usht_dtl_wgt_write(pstBeReg,
				       pstMpiDynaRegCfg->u8detailUshtAmt);
	isp_sharpen_weakdetailgain_write(pstBeReg,
					 pstMpiDynaRegCfg->u8WeakDetailGain);

	// sharpen defalut iso
	isp_sharpen_mhfthdsftd_write(pstBeReg, pstDefDynaRegCfg->u8gainThdSftD);
	isp_sharpen_mhfthdselud_write(pstBeReg,
				      pstDefDynaRegCfg->u8gainThdSelUD);
	isp_sharpen_mhfthdsftud_write(pstBeReg,
				      pstDefDynaRegCfg->u8gainThdSftUD);
	isp_sharpen_dirvarsft_write(pstBeReg, pstDefDynaRegCfg->u8dirVarSft);
	isp_sharpen_shtvarwgt0_write(pstBeReg, pstDefDynaRegCfg->u8shtVarWgt0);
	isp_sharpen_shtvardiffthd0_write(pstBeReg,
					 pstDefDynaRegCfg->u8shtVarDiffThd[0]);
	isp_sharpen_selpixwgt_write(pstBeReg, pstDefDynaRegCfg->u8selPixWgt);
	isp_sharpen_shtvardiffthd1_write(pstBeReg,
					 pstDefDynaRegCfg->u8shtVarDiffThd[1]);
	isp_sharpen_shtvardiffwgt1_write(pstBeReg,
					 pstDefDynaRegCfg->u8shtVarDiffWgt1);
	isp_sharpen_shtvardiffmul_write(pstBeReg,
					pstDefDynaRegCfg->s16shtVarDiffMul);
	isp_sharpen_rmfscale_write(pstBeReg, pstDefDynaRegCfg->u8RmfGainScale);
	isp_sharpen_bmfscale_write(pstBeReg, pstDefDynaRegCfg->u8BmfGainScale);
	isp_sharpen_dirrlythrlow_write(pstBeReg,
				       pstDefDynaRegCfg->u8dirRlyThrLow);
	isp_sharpen_dirrlythrhih_write(pstBeReg,
				       pstDefDynaRegCfg->u8dirRlyThrhigh);

	return GK_SUCCESS;
}

static GK_S32 ISP_SharpenRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				   GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bCurRegNewEn = GK_FALSE;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U8 u8BufId;
	GK_BOOL bStt2LutRegnew = GK_FALSE;

	ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstSharpenDefaultDynaRegCfg =
		GK_NULL;
	ISP_SHARPEN_MPI_DYNA_REG_CFG_S *pstSharpenMpiDynaRegCfg = GK_NULL;
	ISP_SHARPEN_STATIC_REG_CFG_S *pstSharpenStaticRegCfg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1SharpenCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstViProcReg);
		pstSharpenStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i]
				 .stSharpenRegCfg.stStaticRegCfg;
		pstSharpenDefaultDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i]
				 .stSharpenRegCfg.stDynaRegCfg
				 .stDefaultDynaRegCfg;
		pstSharpenMpiDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i]
				 .stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg;

		if (pstSharpenStaticRegCfg->bStaticResh) {
			isp_sharpen_lut_width_word_write(
				pstViProcReg,
				ISP_SHARPEN_LUT_WIDTH_WORD_DEFAULT);

			isp_sharpen_chrr_var_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8chrRSft[0],
				pstSharpenStaticRegCfg->u8chrRSft[1],
				pstSharpenStaticRegCfg->u8chrRSft[2],
				pstSharpenStaticRegCfg->u8chrRSft[3],
				pstSharpenStaticRegCfg->u8chrRVarSft);

			isp_sharpen_dhsf_2dcoef0_47_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD0[4],
				pstSharpenStaticRegCfg->s8hsfCoefD0[5],
				pstSharpenStaticRegCfg->s8hsfCoefD0[6],
				pstSharpenStaticRegCfg->s8hsfCoefD0[7]);

			isp_sharpen_skin_cnt_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8skinCntThd[0],
				pstSharpenStaticRegCfg->u8skinCntThd[1],
				pstSharpenStaticRegCfg->u8skinCntMul);

			isp_sharpen_dhsf_2dcoef0_811_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD0[8],
				pstSharpenStaticRegCfg->s8hsfCoefD0[9],
				pstSharpenStaticRegCfg->s8hsfCoefD0[10],
				pstSharpenStaticRegCfg->s8hsfCoefD0[11]);

			isp_sharpen_ldci_dither_cfg_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8ditherMode);

			isp_sharpen_dhsf_2dcoef2_1215_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD2[12]);

			isp_sharpen_dhsf_2dcoef0_03_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD0[0],
				pstSharpenStaticRegCfg->s8hsfCoefD0[1],
				pstSharpenStaticRegCfg->s8hsfCoefD0[2],
				pstSharpenStaticRegCfg->s8hsfCoefD0[3]);

			isp_sharpen_dhsf_2dcoef3_1215_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD3[12]);

			isp_sharpen_gain_sft_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8mfGainSft,
				pstSharpenStaticRegCfg->u8lfGainWgt,
				pstSharpenStaticRegCfg->u8hfGainSft);
			isp_sharpen_lmthf0_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[0],
				pstSharpenStaticRegCfg->u8lmtHF[1],
				pstSharpenStaticRegCfg->u8lmtHF[2],
				pstSharpenStaticRegCfg->u8lmtHF[3]);
			isp_sharpen_lmthf1_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[4],
				pstSharpenStaticRegCfg->u8lmtHF[5],
				pstSharpenStaticRegCfg->u8lmtHF[6],
				pstSharpenStaticRegCfg->u8lmtHF[7]);
			isp_sharpen_dhsf_2dcoef1_811_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD1[8],
				pstSharpenStaticRegCfg->s8hsfCoefD1[9],
				pstSharpenStaticRegCfg->s8hsfCoefD1[10],
				pstSharpenStaticRegCfg->s8hsfCoefD1[11]);

			isp_sharpen_udhsf_coef_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefUD[0],
				pstSharpenStaticRegCfg->s8hsfCoefUD[1],
				pstSharpenStaticRegCfg->s8hsfCoefUD[2],
				pstSharpenStaticRegCfg->u8hsfSftUD);

			isp_sharpen_shoot_maxchg_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u16oMaxChg,
				pstSharpenStaticRegCfg->u16uMaxChg);
			isp_sharpen_dhsf_2dshift_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8hsfSftD);

			isp_sharpen_chrb_var_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8chrBSft[0],
				pstSharpenStaticRegCfg->u8chrBSft[1],
				pstSharpenStaticRegCfg->u8chrBSft[2],
				pstSharpenStaticRegCfg->u8chrBSft[3],
				pstSharpenStaticRegCfg->u8chrBVarSft);

			isp_sharpen_dhsf_2dcoef3_03_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD3[0],
				pstSharpenStaticRegCfg->s8hsfCoefD3[1],
				pstSharpenStaticRegCfg->s8hsfCoefD3[2],
				pstSharpenStaticRegCfg->s8hsfCoefD3[3]);

			isp_sharpen_dhsf_2dcoef0_1215_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD0[12]);

			isp_sharpen_chrg_sft_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8chrGSft[0],
				pstSharpenStaticRegCfg->u8chrGSft[1],
				pstSharpenStaticRegCfg->u8chrGSft[2],
				pstSharpenStaticRegCfg->u8chrGSft[3]);

			isp_sharpen_dhsf_2dcoef3_47_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD3[4],
				pstSharpenStaticRegCfg->s8hsfCoefD3[5],
				pstSharpenStaticRegCfg->s8hsfCoefD3[6],
				pstSharpenStaticRegCfg->s8hsfCoefD3[7]);

			isp_sharpen_dlpf_coef_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->s8lpfCoefD[0],
				pstSharpenStaticRegCfg->s8lpfCoefD[1],
				pstSharpenStaticRegCfg->s8lpfCoefD[2],
				pstSharpenStaticRegCfg->u8lpfSftD);

			isp_sharpen_udlpf_coef_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8lpfCoefUD[0],
				pstSharpenStaticRegCfg->s8lpfCoefUD[1],
				pstSharpenStaticRegCfg->s8lpfCoefUD[2],
				pstSharpenStaticRegCfg->u8lpfSftUD);

			isp_sharpen_dhsf_2dcoef2_47_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD2[4],
				pstSharpenStaticRegCfg->s8hsfCoefD2[5],
				pstSharpenStaticRegCfg->s8hsfCoefD2[6],
				pstSharpenStaticRegCfg->s8hsfCoefD2[7]);

			isp_sharpen_chrg_thd_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8chrGOriCb,
				pstSharpenStaticRegCfg->u8chrGOriCr,
				pstSharpenStaticRegCfg->u8chrGThd[0],
				pstSharpenStaticRegCfg->u8chrGThd[1]);

			isp_sharpen_lmtmf1_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[4],
				pstSharpenStaticRegCfg->u8lmtMF[5],
				pstSharpenStaticRegCfg->u8lmtMF[6],
				pstSharpenStaticRegCfg->u8lmtMF[7]);

			isp_sharpen_chrr_thd_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8chrROriCb,
				pstSharpenStaticRegCfg->u8chrROriCr,
				pstSharpenStaticRegCfg->u8chrRThd[0],
				pstSharpenStaticRegCfg->u8chrRThd[1]);

			isp_sharpen_dhsf_2dcoef2_03_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD2[0],
				pstSharpenStaticRegCfg->s8hsfCoefD2[1],
				pstSharpenStaticRegCfg->s8hsfCoefD2[2],
				pstSharpenStaticRegCfg->s8hsfCoefD2[3]);

			isp_sharpen_dhsf_2dcoef3_811_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD3[8],
				pstSharpenStaticRegCfg->s8hsfCoefD3[9],
				pstSharpenStaticRegCfg->s8hsfCoefD3[10],
				pstSharpenStaticRegCfg->s8hsfCoefD3[11]);

			isp_sharpen_chrb_thd_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8chrBOriCb,
				pstSharpenStaticRegCfg->u8chrBOriCr,
				pstSharpenStaticRegCfg->u8chrBThd[0],
				pstSharpenStaticRegCfg->u8chrBThd[1]);

			isp_sharpen_dhsf_2dcoef2_811_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD2[8],
				pstSharpenStaticRegCfg->s8hsfCoefD2[9],
				pstSharpenStaticRegCfg->s8hsfCoefD2[10],
				pstSharpenStaticRegCfg->s8hsfCoefD2[11]);

			isp_sharpen_dhsf_2dcoef1_1215_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD1[12]);

			isp_sharpen_dhsf_2dcoef1_03_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD1[0],
				pstSharpenStaticRegCfg->s8hsfCoefD1[1],
				pstSharpenStaticRegCfg->s8hsfCoefD1[2],
				pstSharpenStaticRegCfg->s8hsfCoefD1[3]);

			isp_sharpen_lmtmf0_u32_write(
				pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[0],
				pstSharpenStaticRegCfg->u8lmtMF[1],
				pstSharpenStaticRegCfg->u8lmtMF[2],
				pstSharpenStaticRegCfg->u8lmtMF[3]);
			isp_sharpen_dhsf_2dcoef1_47_u32_write(
				pstBeReg,
				pstSharpenStaticRegCfg->s8hsfCoefD1[4],
				pstSharpenStaticRegCfg->s8hsfCoefD1[5],
				pstSharpenStaticRegCfg->s8hsfCoefD1[6],
				pstSharpenStaticRegCfg->s8hsfCoefD1[7]);

			isp_sharpen_ben8dir_sel_write(
				pstBeReg, pstSharpenStaticRegCfg->bEnShp8Dir);
			isp_sharpen_benshtvar_sel_write(
				pstBeReg, pstSharpenStaticRegCfg->u8shtVarSel);
			isp_sharpen_detailthd_sel_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8detailThdSel);
			//isp_sharpen_dirrt0_write(pstBeReg,pstSharpenStaticRegCfg->u8dirRt[0]);
			//isp_sharpen_dirrt1_write(pstBeReg,pstSharpenStaticRegCfg->u8dirRt[1]);
			isp_sharpen_dirrly0_write(
				pstBeReg, pstSharpenStaticRegCfg->u8dirRly[0]);
			isp_sharpen_dirrly1_write(
				pstBeReg, pstSharpenStaticRegCfg->u8dirRly[1]);
			isp_sharpen_dirvarscale_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8dirVarScale);
			isp_sharpen_mhfthdseld_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8gainThdSelD);
			isp_sharpen_max_var_clip_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8MaxVarClipMin);
			isp_sharpen_shtvarthd0_write(
				pstBeReg, pstSharpenStaticRegCfg->u8shtVarThd0);
			isp_sharpen_shtvarwgt1_write(
				pstBeReg, pstSharpenStaticRegCfg->u8shtVarWgt1);
			isp_sharpen_shtvardiffwgt0_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8shtVarDiffWgt0);
			isp_sharpen_shtvar5x5_sft_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8shtVar5x5Sft);
			isp_sharpen_shtvarsft_write(
				pstBeReg, pstSharpenStaticRegCfg->u8shtVarSft);
			isp_sharpen_skinedgesft_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8skinEdgeSft);
			isp_sharpen_skinedgethd0_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8skinEdgeThd[0]);
			isp_sharpen_skinedgethd1_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8skinEdgeThd[1]);
			isp_sharpen_dtl_thdsft_write(
				pstBeReg,
				pstSharpenStaticRegCfg->u8detailThdSft);

			pstSharpenStaticRegCfg->bStaticResh = GK_FALSE;
		}

		if (pstSharpenDefaultDynaRegCfg->bResh) {
			pstSharpenDefaultDynaRegCfg->bResh = bIsOfflineMode;
		}

		bIdxResh = (isp_sharpen_update_index_read(pstBeReg) !=
			    pstSharpenMpiDynaRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ?
				   (pstSharpenMpiDynaRegCfg->bResh & bIdxResh) :
				   (pstSharpenMpiDynaRegCfg->bResh);

		if (bUsrResh) {
			isp_sharpen_update_index_write(
				pstBeReg,
				pstSharpenMpiDynaRegCfg->u32UpdateIndex);

			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				/* online Lut2stt regconfig */
				u8BufId = pstSharpenMpiDynaRegCfg->u8BufId;
				pstBeLutSttReg = (S_ISP_LUT_WSTT_TYPE *)
					ISP_GetBeLut2SttVirAddr(ViPipe, i,
								u8BufId);
				ISP_CHECK_NULLPTR(pstBeLutSttReg);

				isp_sharpen_lut_wstt_write(
					pstBeLutSttReg,
					pstSharpenMpiDynaRegCfg->u16mfGainD,
					pstSharpenMpiDynaRegCfg->u16mfGainUD,
					pstSharpenMpiDynaRegCfg->u16hfGainD,
					pstSharpenMpiDynaRegCfg->u16hfGainUD);

				isp_sharpen_lut_wstt_addr_write(
					ViPipe, i, u8BufId, pstViProcReg);

				isp_sharpen_stt2lut_en_write(pstBeReg, GK_TRUE);
				pstSharpenMpiDynaRegCfg->u8BufId = 1 - u8BufId;
				bCurRegNewEn = GK_TRUE;
			} else {
				isp_sharpen_lut_wstt_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstSharpenMpiDynaRegCfg->u16mfGainD,
					pstSharpenMpiDynaRegCfg->u16mfGainUD,
					pstSharpenMpiDynaRegCfg->u16hfGainD,
					pstSharpenMpiDynaRegCfg->u16hfGainUD);

				isp_sharpen_stt2lut_en_write(pstBeReg, GK_TRUE);
				bCurRegNewEn = GK_TRUE;
			}
			bStt2LutRegnew = GK_TRUE;
			pstSharpenMpiDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstSharpenMpiDynaRegCfg->bPreRegNewEn = bCurRegNewEn;

		pstRegCfgInfo->unKey.bit1SharpenCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}
	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bSharpenStt2LutRegnew =
		bStt2LutRegnew;
	return GK_SUCCESS;
}

static GK_S32 ISP_DemRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bGfLutUpdate = GK_FALSE;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 j;

	ISP_DEMOSAIC_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_DEMOSAIC_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1DemCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stDemRegCfg.stStaticRegCfg;
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDemRegCfg.stDynaRegCfg;

		if (pstStaticRegCfg->bResh) { /* static */
			isp_demosaic_desat_thr_u32_write(
				pstBeReg, pstStaticRegCfg->u16DeSatThresh1,
				pstStaticRegCfg->u16DeSatThresh2);
			isp_demosaic_cbcravgthld_u32_write(
				pstBeReg, pstStaticRegCfg->u16CbCrAvgThr);
			isp_demosaic_cc_var_thresh_u32_write(
				pstBeReg, pstStaticRegCfg->s16CcVarThresh);
			isp_nddm_gf_th_u32_write(pstBeReg,
						 pstStaticRegCfg->u16GFThLow,
						 pstStaticRegCfg->u16GFThHig);
			isp_demosaic_coef3_u32_write(
				pstBeReg, pstStaticRegCfg->u16AhdPart2,
				pstStaticRegCfg->u16AhdPart1);
			isp_demosaic_coef5_u32_write(
				pstBeReg, pstStaticRegCfg->u8hvColorRatio);
			isp_demosaic_coef0_u32_write(
				pstBeReg, pstStaticRegCfg->u8hvBlendLimit2,
				pstStaticRegCfg->u8hvBlendLimit1);
			isp_demosaic_sel_u32_write(
				pstBeReg, pstStaticRegCfg->u8hvSelection);
			isp_demosaic_g_intp_ctrl_u32_write(
				pstBeReg, pstStaticRegCfg->u8GClipBitSft);

			isp_demosaic_ahd_en_write(pstBeReg,
						  pstStaticRegCfg->bAHDEnable);
			isp_demosaic_de_fake_en_write(
				pstBeReg, pstStaticRegCfg->bDeFakeEnable);
			isp_demosaic_desat_enable_write(
				pstBeReg, pstStaticRegCfg->bDeSatEnable);
			isp_demosaic_desatg_write(pstBeReg,
						  pstStaticRegCfg->u16DeSatHig);
			isp_demosaic_desat_protect_sl_write(
				pstBeReg, pstStaticRegCfg->u16DeSatProtSl);
			isp_demosaic_lpf_f3_write(pstBeReg,
						  pstStaticRegCfg->u8Lpff3);
			isp_nddm_dith_mask_write(pstBeReg,
						 pstStaticRegCfg->u8DitherMask);
			isp_nddm_dith_ratio_write(
				pstBeReg, pstStaticRegCfg->u8DitherRatio);
			pstRegCfgInfo->stAlgRegCfg[i]
				.stDemRegCfg.stStaticRegCfg.bResh = GK_FALSE;
		}

		if (pstDynaRegCfg->bResh) { /* dynamic */
			isp_demosaic_defcolor_coef_u32_write(
				pstBeReg, pstDynaRegCfg->u8DeFColorCr,
				pstDynaRegCfg->u8DeFColorCb);
			isp_demosaic_fake_cr_var_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16FakeCrVarThrLow,
				pstDynaRegCfg->u16FakeCrVarThrHigh);
			isp_demosaic_hf_intp_blur_th_u32_write(
				pstBeReg, pstDynaRegCfg->u16HfIntpBlurThLow,
				pstDynaRegCfg->u16HfIntpBlurThHig);
			isp_nddm_awb_gf_cfg_u32_write(
				pstBeReg, pstDynaRegCfg->u8AwbGFGainLow,
				pstDynaRegCfg->u8AwbGFGainHig,
				pstDynaRegCfg->u8AwbGFGainMax);
			isp_nddm_fcr_gain_u32_write(
				pstBeReg, pstDynaRegCfg->u8FcrGFGain,
				pstDynaRegCfg->u16FcrDetLow);
			isp_demosaic_desat_bldr_ratio_u32_write(
				pstBeReg, pstDynaRegCfg->s16DeSatRatio);
			isp_nddm_ehc_gray_u32_write(pstBeReg,
						    pstDynaRegCfg->u8EhcGray);
			isp_demosaic_hf_intp_blur_ratio_u32_write(
				pstBeReg, pstDynaRegCfg->u16HfIntpBlurRatio);
			isp_demosaic_cc_hf_ratio_u32_write(
				pstBeReg, pstDynaRegCfg->u8CcHFMinRatio,
				pstDynaRegCfg->u8CcHFMaxRatio);
			isp_demosaic_desat_low_write(
				pstBeReg, pstDynaRegCfg->u16DeSatLow);
			isp_demosaic_desat_protect_th_write(
				pstBeReg, pstDynaRegCfg->u16DeSatProtTh);
			isp_demosaic_lpf_f0_write(pstBeReg,
						  pstDynaRegCfg->u8Lpff0);
			isp_demosaic_lpf_f1_write(pstBeReg,
						  pstDynaRegCfg->u8Lpff1);
			isp_demosaic_lpf_f2_write(pstBeReg,
						  pstDynaRegCfg->u8Lpff2);
			isp_nddm_dith_max_write(pstBeReg,
						pstDynaRegCfg->u8DitherMax);
			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				isp_nddm_gf_lut_waddr_u32_write(pstBeReg, 0);

				for (j = 0; j < ISP_DEMOSAIC_LUT_LENGTH; j++) {
					isp_nddm_gf_lut_wdata_u32_write(
						pstBeReg,
						pstDynaRegCfg->au16GFBlurLut[j]);
				}
			} else {
				isp_nddm_gflut_write(
					pstBeReg, pstDynaRegCfg->au16GFBlurLut);
			}
			isp_cc_rcv_ctrl0_u32_write(
				pstBeReg, pstDynaRegCfg->u8ColorNoiseStrY,
				ISP_CCM_SOFT_CLIP1_STEP_DEFAULT);
			isp_cc_rcv_ctrl1_u32_write(
				pstBeReg,
				ISP_CCM_DARK_RANGE_MAX -
					pstDynaRegCfg->u8ColorNoiseThdY,
				ISP_CCM_PEAKSUPP_SAT_DEFAULT,
				ISP_CCM_PEAKSUPP_MAX_DEFAULT);
			bGfLutUpdate = pstDynaRegCfg->bUpdateGF;
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1DemCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stBeLutUpdateCfg.bNddmGfLutUpdate =
		bGfLutUpdate | bIsOfflineMode;

	return GK_SUCCESS;
}

static GK_S32 ISP_FpnRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
#ifdef CONFIG_VI_FPN_SUPPORT
	ISP_FPN_DYNA_CFG_S *pstDynaRegCfg;

	if (pstRegCfgInfo->unKey.bit1FpnCfg) {
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stFpnRegCfg.stDynaRegCfg;

		isp_fpn_overflowthr_write(ViPipe, i,
					  pstDynaRegCfg->u32IspFpnOverflowThr);
		isp_fpn_strength0_write(ViPipe, i,
					pstDynaRegCfg->u32IspFpnStrength[0]);

		pstRegCfgInfo->unKey.bit1FpnCfg = 0;
	}
#endif
	return GK_SUCCESS;
}

static GK_S32 isp_ldci_read_stt_addr_write(S_VIPROC_REGS_TYPE *pstViProcReg,
					   GK_U64 u64PhyAddr)
{
	if (u64PhyAddr == 0) {
		return GK_FAILURE;
	}

	viproc_para_dci_addr_low_write(pstViProcReg, GetLowAddr(u64PhyAddr));

	return GK_SUCCESS;
}

static GK_S32 ISP_LdciRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				GK_U8 i)
{
	GK_BOOL bIsOfflineMode, bRdstatEn;
	GK_U8 u8BufId, u8ReadBufIdx;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 k;
	GK_S32 s32Ret = GK_SUCCESS;
	GK_BOOL bStt2LutRegnew = GK_FALSE;
	ISP_LDCI_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_LDCI_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LDCI_STAT_S *pstReadSttBuf = GK_NULL;
	ISP_LDCI_READ_STT_BUF_S *pstLdciReadSttBuf = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1LdciCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stLdciRegCfg.stStaticRegCfg;
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stLdciRegCfg.stDynaRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			/* static */
			isp_ldci_stat_evratio_u32_write(pstBeReg, 0x1000);
			isp_ldci_lut_width_word_write(
				pstViProcReg, ISP_LDCI_LUT_WIDTH_WORD_DEFAULT);
			isp_ldci_lpf_lpfsft_u32_write(
				pstBeReg, pstStaticRegCfg->u32LpfSft);
			isp_ldci_chrdamp_u32_write(
				pstBeReg, pstStaticRegCfg->u32ChrPosDamp,
				pstStaticRegCfg->u32ChrNegDamp);
			isp_ldci_luma_sel_u32_write(
				pstBeReg, pstStaticRegCfg->u32CalcLumaSel);
			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				for (k = 0; k < 2; k++) {
					pstBeLutSttReg = (S_ISP_LUT_WSTT_TYPE *)
						ISP_GetBeLut2SttVirAddr(ViPipe,
									i, k);
					ISP_CHECK_POINTER(pstBeLutSttReg);
					isp_ldci_cgain_lut_wstt_write(
						pstBeLutSttReg,
						pstDynaRegCfg->u32ColorGainLut);
					isp_ldci_usm_lut_wstt_write(
						pstBeLutSttReg,
						pstDynaRegCfg->u32UsmPosLut,
						pstDynaRegCfg->u32UsmNegLut);
				}

			} else {
				isp_ldci_cgain_lut_wstt_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstDynaRegCfg->u32ColorGainLut);
				isp_ldci_usm_lut_wstt_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstDynaRegCfg->u32UsmPosLut,
					pstDynaRegCfg->u32UsmNegLut);
			}

			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		/* dynamic */
		if (pstIspCtx->bLdciTprFltEn == GK_TRUE) {
			LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);
			u8ReadBufIdx = pstLdciReadSttBuf->u8BufIdx;

			if (i == 0) {
				pstReadSttBuf = (ISP_LDCI_STAT_S *)
					ISP_GetLdciReadSttVirAddr(ViPipe,
								  u8ReadBufIdx);
				ISP_CHECK_POINTER(pstReadSttBuf);
				memcpy(pstReadSttBuf, &pstDynaRegCfg->stTprStat,
				       sizeof(ISP_LDCI_STAT_S));
			}

			if ((i + 1) == u8BlkNum) {
				pstLdciReadSttBuf->u8BufIdx =
					(u8ReadBufIdx + 1) %
					DIV_0_TO_1(pstLdciReadSttBuf->u8BufNum);
			}

			/* Set ReadStt Addr */
			s32Ret = isp_ldci_read_stt_addr_write(
				pstViProcReg,
				pstLdciReadSttBuf->astReadBuf[u8ReadBufIdx]
					.u64PhyAddr);
			bRdstatEn = (s32Ret == GK_SUCCESS) ?
					    (pstDynaRegCfg->bRdstatEn) :
					    (GK_FALSE);
		} else {
			bRdstatEn = pstDynaRegCfg->bRdstatEn;
		}

		isp_ldci_en_write(pstViProcReg, pstDynaRegCfg->bEnable);
		isp_ldci_cfg_u32_write(pstBeReg, pstDynaRegCfg->bCalcEnable,
				       pstDynaRegCfg->bWrstatEn, bRdstatEn);
		isp_ldci_stat_vpos_u32_write(pstBeReg,
					     pstDynaRegCfg->u32StatVStart,
					     pstDynaRegCfg->u32StatVEnd);
		isp_ldci_stat_hpos_u32_write(pstBeReg,
					     pstDynaRegCfg->u32StatHStart,
					     pstDynaRegCfg->u32StatHEnd);
		isp_ldci_lpf_lpfcoef1_u32_write(pstBeReg,
						pstDynaRegCfg->u32LpfCoef[2],
						pstDynaRegCfg->u32LpfCoef[3]);
		isp_ldci_lpf_lpfcoef0_u32_write(pstBeReg,
						pstDynaRegCfg->u32LpfCoef[0],
						pstDynaRegCfg->u32LpfCoef[1]);
		isp_ldci_scale_u32_write(pstBeReg, pstDynaRegCfg->u32CalcScaleX,
					 pstDynaRegCfg->u32CalcScaleY);
		isp_ldci_lpf_lpfcoef2_u32_write(pstBeReg,
						pstDynaRegCfg->u32LpfCoef[4]);
		isp_ldci_zone_u32_write(pstBeReg,
					pstDynaRegCfg->u32CalcSmlMapStride,
					pstDynaRegCfg->u32CalcSmlMapHeight,
					pstDynaRegCfg->u32CalcTotalZone);
		isp_ldci_stat_zone_u32_write(pstBeReg,
					     pstDynaRegCfg->u32StatSmlMapWidth,
					     pstDynaRegCfg->u32StatSmlMapHeight,
					     pstDynaRegCfg->u32StatTotalZone);
		isp_ldci_calc_small_offset_u32_write(
			pstBeReg, pstDynaRegCfg->u32CalcMapOffsetX);
		isp_ldci_blk_smlmapwidth1_u32_write(
			pstBeReg, pstDynaRegCfg->u32BlkSmlMapWidth[1]);
		isp_ldci_blk_smlmapwidth0_u32_write(
			pstBeReg, pstDynaRegCfg->u32BlkSmlMapWidth[0]);
		isp_ldci_blk_smlmapwidth2_u32_write(
			pstBeReg, pstDynaRegCfg->u32BlkSmlMapWidth[2]);
		if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		    IS_SIDEBYSIDE_MODE(
			    pstIspCtx->stBlockAttr.enIspRunningMode)) {
			/* online Lut2stt regconfig */
			u8BufId = pstDynaRegCfg->u8BufId;
			pstBeLutSttReg =
				(S_ISP_LUT_WSTT_TYPE *)ISP_GetBeLut2SttVirAddr(
					ViPipe, i, u8BufId);
			ISP_CHECK_POINTER(pstBeLutSttReg);
			isp_ldci_he_lut_lut_wstt_write(
				pstBeLutSttReg, pstDynaRegCfg->u32HePosLut,
				pstDynaRegCfg->u32HeNegLut);
			isp_ldci_lut_wstt_addr_write(ViPipe, i, u8BufId,
						     pstViProcReg);
			pstDynaRegCfg->u8BufId = 1 - u8BufId;
		} else {
			isp_ldci_he_lut_lut_wstt_write(
				&pstBeReg->stIspBeLut.stBeLut2Stt,
				pstDynaRegCfg->u32HePosLut,
				pstDynaRegCfg->u32HeNegLut);
		}
		isp_ldci_stt2lut_en_write(pstBeReg, GK_TRUE);
		bStt2LutRegnew = GK_TRUE;
		pstRegCfgInfo->unKey.bit1LdciCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}
	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bLdciStt2LutRegnew =
		bStt2LutRegnew;
	return GK_SUCCESS;
}

static GK_S32 ISP_LcacRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bLutUpdate = GK_FALSE;
	GK_U8 j;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	ISP_LOCAL_CAC_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	ISP_LOCAL_CAC_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_LOCAL_CAC_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1LocalCacCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		/* static */
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stLCacRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_nddm_cfg_u32_write(
				pstBeReg, pstStaticRegCfg->bNddmCacBlendEn);
			//isp_demosaic_fake_cr_var_thr_u32_write(pstBeReg, pstStaticRegCfg->u16FakeCrVarThrLow, pstStaticRegCfg->u16FakeCrVarThrHigh);
			isp_demosaic_satu_thr_u32_write(
				pstBeReg, pstStaticRegCfg->u16SatuThr);
			isp_nddm_dm_bldrate_u32_write(
				pstBeReg, pstStaticRegCfg->u16NddmCacBlendRate);
			isp_demosaic_lcac_cnt_thr_u32_write(
				pstBeReg, pstStaticRegCfg->u8RCounterThr,
				pstStaticRegCfg->u8BCounterThr,
				pstStaticRegCfg->u8GCounterThr);
			isp_demosaic_cac_cor_mode_write(
				pstBeReg, pstStaticRegCfg->bCacCorMode);
			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				isp_demosaic_depurplut_waddr_u32_write(pstBeReg,
								       0);

				for (j = 0; j < 16; j++) {
					isp_demosaic_depurplut_wdata_u32_write(
						pstBeReg,
						pstStaticRegCfg
							->au8DePurpleStr[j]);
				}
			} else {
				isp_demosaic_depurp_lut_write(
					pstBeReg,
					pstStaticRegCfg->au8DePurpleStr);
			}

			bLutUpdate = GK_TRUE;
			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		/* Usr */
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stLCacRegCfg.stUsrRegCfg;
		bIdxResh = (isp_lcac_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bResh & bIdxResh) :
					      (pstUsrRegCfg->bResh);

		if (bUsrResh) {
			isp_lcac_update_index_write(
				pstBeReg, pstUsrRegCfg->u32UpdateIndex);
			isp_demosaic_cac_cnt_cfg_u32_write(
				pstBeReg, pstUsrRegCfg->u16CbCntRatio,
				pstUsrRegCfg->u8CbCntLowThr,
				pstUsrRegCfg->u8CbCntHighThr);
			isp_demosaic_cac_lumagh_cnt_thr_u32_write(
				pstBeReg, pstUsrRegCfg->u8LumaHighCntThr);
			//isp_demosaic_defcolor_coef_u32_write(pstBeReg, pstUsrRegCfg->u8DeFColorCr, pstUsrRegCfg->u8DeFColorCb);
			isp_demosaic_purple_var_thr_u32_write(
				pstBeReg, pstUsrRegCfg->u16VarThr);
			isp_demosaic_cac_bld_avg_u32_write(
				pstBeReg, pstUsrRegCfg->u8BldAvgCur);

			isp_demosaic_cb_thr_write(pstBeReg,
						  pstUsrRegCfg->u16CbThr);
			isp_demosaic_cbcr_ratiogh_limit_write(
				pstBeReg, pstUsrRegCfg->s16CbCrRatioLmtHigh);
			pstUsrRegCfg->bResh = bIsOfflineMode;
		}

		/* dynamic */
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stLCacRegCfg.stDynaRegCfg;

		if (pstDynaRegCfg->bResh) {
			isp_demosaic_lcac_luma_rb_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16RLumaThr,
				pstDynaRegCfg->u16BLumaThr);
			isp_demosaic_lcac_luma_g_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16GLumaThr);
			isp_demosaic_depurplectr_u32_write(
				pstBeReg, pstDynaRegCfg->u8DePurpleCtrCb,
				pstDynaRegCfg->u8DePurpleCtrCr);
			isp_demosaic_luma_thr_write(pstBeReg,
						    pstDynaRegCfg->u16LumaThr);
			isp_demosaic_cbcr_ratio_low_limit_write(
				pstBeReg, pstDynaRegCfg->s16CbCrRatioLmtLow);
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1LocalCacCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stBeLutUpdateCfg.bLCacLutUpdate =
		bLutUpdate | bIsOfflineMode;

	return GK_SUCCESS;
}

static GK_S32 ISP_FcrRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	ISP_ANTIFALSECOLOR_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_ANTIFALSECOLOR_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1FcrCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		pstStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i]
				 .stAntiFalseColorRegCfg.stStaticRegCfg;
		pstDynaRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					 .stAntiFalseColorRegCfg.stDynaRegCfg;

		/* static */
		if (pstStaticRegCfg->bResh) {
			isp_demosaic_coef2_u32_write(
				pstBeReg, pstStaticRegCfg->u16FcrLimit2,
				pstStaticRegCfg->u16FcrLimit1);
			pstStaticRegCfg->bResh = GK_FALSE;
		}

		/* dynamic */
		if (pstDynaRegCfg->bResh) {
			isp_demosaic_fcr_hf_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16FcrHfThreshLow,
				pstDynaRegCfg->u16FcrHfThreshHig);
			isp_demosaic_fcr_gray_ratio_u32_write(
				pstBeReg, pstDynaRegCfg->u8FcrGrayRatio);
			isp_demosaic_coef4_u32_write(pstBeReg,
						     pstDynaRegCfg->u8FcrGain);
			isp_demosaic_coef6_u32_write(pstBeReg,
						     pstDynaRegCfg->u8FcrRatio);
			isp_demosaic_fcr_sel_u32_write(
				pstBeReg, pstDynaRegCfg->u8FcrDetgSel,
				pstDynaRegCfg->u8FcrCmaxSel);
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1FcrCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_DpcRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;

	GK_BOOL bIsOfflineMode;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	ISP_DPC_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_DPC_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_DPC_USR_CFG_S *pstUsrRegCfg = GK_NULL;

	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1DpCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		/* static */
		pstStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDpRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_dpc_output_mode_write(
				pstBeReg, pstStaticRegCfg->u8DpccOutputMode);
			isp_dpc_bpt_ctrl_write(pstBeReg,
					       pstStaticRegCfg->u32DpccBptCtrl);
			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		/* usr */
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDpRegCfg.stUsrRegCfg;

		if (pstUsrRegCfg->stUsrDynaCorRegCfg.bResh) {
			isp_dpc_soft_thr_u32_write(
				pstBeReg,
				pstUsrRegCfg->stUsrDynaCorRegCfg
					.s8DpccSupTwinkleThrMin,
				pstUsrRegCfg->stUsrDynaCorRegCfg
					.s8DpccSupTwinkleThrMax);
			isp_dpc_bhardthr_en_u32_write(
				pstBeReg,
				pstUsrRegCfg->stUsrDynaCorRegCfg.bDpccHardThrEn);
			isp_dpc_rakeratio_u32_write(
				pstBeReg, pstUsrRegCfg->stUsrDynaCorRegCfg
						  .u16DpccRakeRatio);
			isp_dcg_sel_write(
				pstBeReg,
				pstUsrRegCfg->stUsrDynaCorRegCfg.u8DPCPos);
			pstUsrRegCfg->stUsrDynaCorRegCfg.bResh = bIsOfflineMode;
		}

		bIdxResh = (isp_dpc_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->stUsrStaCorRegCfg.u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ?
				   (pstUsrRegCfg->stUsrStaCorRegCfg.bResh &
				    bIdxResh) :
				   (pstUsrRegCfg->stUsrStaCorRegCfg.bResh);

		if (bUsrResh) {
			isp_dpc_update_index_write(
				pstBeReg,
				pstUsrRegCfg->stUsrStaCorRegCfg.u32UpdateIndex);
			pstUsrRegCfg->stUsrStaCorRegCfg.bResh = bIsOfflineMode;
		}

		/* dynamic */
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg;

		if (pstDynaRegCfg->bResh) {
			isp_dpc_stat_en_write(pstViProcReg,
					      pstDynaRegCfg->bDpcStatEn);

			isp_dpc_blend_write(pstBeReg,
					    pstDynaRegCfg->u32DpccAlpha);
			isp_dpc_mode_write(pstBeReg,
					   pstDynaRegCfg->u16DpccMode);
			isp_dpc_set_use_write(pstBeReg,
					      pstDynaRegCfg->u8DpccSetUse);
			isp_dpc_methods_set_1_write(
				pstBeReg, pstDynaRegCfg->u16DpccMethodsSet1);
			isp_dpc_methods_set_2_write(
				pstBeReg, pstDynaRegCfg->u16DpccMethodsSet2);
			isp_dpc_methods_set_3_write(
				pstBeReg, pstDynaRegCfg->u16DpccMethodsSet3);
			isp_dpc_line_thresh_1_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineThr[0]);
			isp_dpc_line_mad_fac_1_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineMadFac[0]);
			isp_dpc_pg_fac_1_write(pstBeReg,
					       pstDynaRegCfg->au16DpccPgFac[0]);
			isp_dpc_rnd_thresh_1_write(
				pstBeReg, pstDynaRegCfg->au16DpccRndThr[0]);
			isp_dpc_rg_fac_1_write(pstBeReg,
					       pstDynaRegCfg->au16DpccRgFac[0]);
			isp_dpc_line_thresh_2_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineThr[1]);
			isp_dpc_line_mad_fac_2_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineMadFac[1]);
			isp_dpc_pg_fac_2_write(pstBeReg,
					       pstDynaRegCfg->au16DpccPgFac[1]);
			isp_dpc_rnd_thresh_2_write(
				pstBeReg, pstDynaRegCfg->au16DpccRndThr[1]);
			isp_dpc_rg_fac_2_write(pstBeReg,
					       pstDynaRegCfg->au16DpccRgFac[1]);
			isp_dpc_line_thresh_3_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineThr[2]);
			isp_dpc_line_mad_fac_3_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineMadFac[2]);
			isp_dpc_pg_fac_3_write(pstBeReg,
					       pstDynaRegCfg->au16DpccPgFac[2]);
			isp_dpc_rnd_thresh_3_write(
				pstBeReg, pstDynaRegCfg->au16DpccRndThr[2]);
			isp_dpc_rg_fac_3_write(pstBeReg,
					       pstDynaRegCfg->au16DpccRgFac[2]);
			isp_dpc_ro_limits_write(pstBeReg,
						pstDynaRegCfg->u16DpccRoLimits);
			isp_dpc_rnd_offs_write(pstBeReg,
					       pstDynaRegCfg->u16DpccRndOffs);

			isp_dpc_line_std_thr_1_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineStdThr[0]);
			isp_dpc_line_std_thr_2_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineStdThr[1]);
			isp_dpc_line_std_thr_3_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineStdThr[2]);
			isp_dpc_line_std_thr_4_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineStdThr[3]);
			isp_dpc_line_std_thr_5_write(
				pstBeReg, pstDynaRegCfg->au16DpccLineStdThr[4]);

			isp_dpc_line_diff_thr_1_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[0]);
			isp_dpc_line_diff_thr_2_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[1]);
			isp_dpc_line_diff_thr_3_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[2]);
			isp_dpc_line_diff_thr_4_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[3]);
			isp_dpc_line_diff_thr_5_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[4]);

			isp_dpc_line_aver_fac_1_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[0]);
			isp_dpc_line_aver_fac_2_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[1]);
			isp_dpc_line_aver_fac_3_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[2]);
			isp_dpc_line_aver_fac_4_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[3]);
			isp_dpc_line_aver_fac_5_write(
				pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[4]);

			isp_dpc_line_kerdiff_fac_write(
				pstBeReg, pstDynaRegCfg->u8DpccLineKerdiffFac);
			isp_dpc_blend_mode_write(
				pstBeReg, pstDynaRegCfg->u8DpccBlendMode);
			isp_dpc_bit_depth_sel_write(
				pstBeReg, pstDynaRegCfg->u8DpccBitDepthSel);

			isp_dpc_rnd_thresh_1_mtp_write(
				pstBeReg, pstDynaRegCfg->au32DpccRndThrMtp[0]);
			isp_dpc_rnd_thresh_2_mtp_write(
				pstBeReg, pstDynaRegCfg->au32DpccRndThrMtp[1]);
			isp_dpc_rnd_thresh_3_mtp_write(
				pstBeReg, pstDynaRegCfg->au32DpccRndThrMtp[2]);

			isp_dpc_rg_fac_1_mtp_write(
				pstBeReg, pstDynaRegCfg->au32DpccRgFacMtp[0]);
			isp_dpc_rg_fac_2_mtp_write(
				pstBeReg, pstDynaRegCfg->au32DpccRgFacMtp[1]);
			isp_dpc_rg_fac_3_mtp_write(
				pstBeReg, pstDynaRegCfg->au32DpccRgFacMtp[2]);

			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1DpCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_GeRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			      GK_U8 i)
{
#ifdef CONFIG_ISP_CR_SUPPORT
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	ISP_GE_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_GE_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_GE_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1GeCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		/* static */
		pstStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stGeRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_ge_mode_u32_write(pstBeReg,
					      pstStaticRegCfg->bGeGrEn,
					      pstStaticRegCfg->bGeGbEn,
					      pstStaticRegCfg->bGeGrGbEn);
			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		/* usr */
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stGeRegCfg.stUsrRegCfg;

		if (pstUsrRegCfg->bResh) {
			isp_ge0_ct_slope_u32_write(pstBeReg,
						   pstUsrRegCfg->au8GeCtSlope1,
						   pstUsrRegCfg->au8GeCtSlope2);
			isp_ge0_ct_th2_u32_write(pstBeReg,
						 pstUsrRegCfg->au16GeCtTh2);
			pstUsrRegCfg->bResh = bIsOfflineMode;
		}

		/* dynamic */
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stGeRegCfg.stDynaRegCfg;

		if (pstDynaRegCfg->bResh) {
			isp_ge0_ct_th3_u32_write(pstBeReg,
						 pstDynaRegCfg->au16GeCtTh3);
			isp_ge_strength_u32_write(pstBeReg,
						  pstDynaRegCfg->u16GeStrength);
			isp_ge0_ct_th1_u32_write(pstBeReg,
						 pstDynaRegCfg->au16GeCtTh1);
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1GeCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}
#endif

	return GK_SUCCESS;
}

static GK_S32 ISP_LscRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bStt2LutRegnew = GK_FALSE;
	GK_U8 u8BufId;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 j;
	ISP_LSC_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	ISP_LSC_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1LscCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);
		/* static */
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stLscRegCfg.stStaticRegCfg;
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stLscRegCfg.stUsrRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_mlsc_winnum_u32_write(pstBeReg,
						  pstStaticRegCfg->u8WinNumH,
						  pstStaticRegCfg->u8WinNumV);
			isp_lsc_lut_width_word_write(
				pstViProcReg, ISP_MLSC_LUT_WIDTH_WORD_DEFAULT);
			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		/* usr */
		if (pstUsrRegCfg->bCoefUpdate) {
			isp_lsc_mesh_str_write(pstBeReg,
					       pstUsrRegCfg->u16MeshStr);
			isp_mlsc_mesh_str_write(pstBeReg,
						pstUsrRegCfg->u16MeshStr);
			pstUsrRegCfg->bCoefUpdate = bIsOfflineMode;
		}

		bIdxResh = (isp_lsc_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ?
				   (pstUsrRegCfg->bLutUpdate & bIdxResh) :
				   (pstUsrRegCfg->bLutUpdate);

		if (bUsrResh) {
			isp_lsc_update_index_write(
				pstBeReg, pstUsrRegCfg->u32UpdateIndex);

			isp_mlsc_width_offset_write(
				pstBeReg, pstUsrRegCfg->u16WidthOffset);

			for (j = 0; j < (ISP_LSC_GRID_ROW - 1) / 2; j++) {
				isp_mlsc_winy_info_write(
					pstBeReg, j,
					pstUsrRegCfg->au16DeltaY[j],
					pstUsrRegCfg->au16InvY[j]);
			}

			for (j = 0; j < (ISP_LSC_GRID_COL - 1); j++) {
				isp_mlsc_winx_info_write(
					pstBeReg, j,
					pstUsrRegCfg->au16DeltaX[j],
					pstUsrRegCfg->au16InvX[j]);
			}

			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				if (pstRegCfgInfo->stAlgRegCfg[i]
					    .stLscRegCfg.bLut2SttEn ==
				    GK_TRUE) { /* online Lut2stt regconfig */
					u8BufId = pstUsrRegCfg->u8BufId;
					pstBeLutSttReg = (S_ISP_LUT_WSTT_TYPE *)
						ISP_GetBeLut2SttVirAddr(
							ViPipe, i, u8BufId);
					ISP_CHECK_NULLPTR(pstBeLutSttReg);
					isp_mlsc_lut_wstt_write(
						pstBeLutSttReg,
						pstUsrRegCfg->au32RGain,
						pstUsrRegCfg->au32GrGain,
						pstUsrRegCfg->au32GbGain,
						pstUsrRegCfg->au32BGain);

					isp_lsc_lut_wstt_addr_write(
						ViPipe, i, u8BufId,
						pstViProcReg);

					isp_mlsc_stt2lut_en_write(pstBeReg,
								  GK_TRUE);
					pstUsrRegCfg->u8BufId = 1 - u8BufId;
				}
			} else {
				isp_mlsc_lut_wstt_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstUsrRegCfg->au32RGain,
					pstUsrRegCfg->au32GrGain,
					pstUsrRegCfg->au32GbGain,
					pstUsrRegCfg->au32BGain);
				isp_mlsc_stt2lut_en_write(pstBeReg, GK_TRUE);
				bStt2LutRegnew = GK_TRUE;
			}
			bStt2LutRegnew = GK_TRUE;

			pstUsrRegCfg->bLutUpdate = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1LscCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bLscStt2LutRegnew =
		bStt2LutRegnew;

	return GK_SUCCESS;
}

static GK_S32 ISP_RgbirRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				 GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	ISP_RGBIR_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1RgbirCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stRgbirCfg.stUsrRegCfg;

		if (pstUsrRegCfg->bResh) {
			isp_rgbir_reci_ctl_u32_write(pstBeReg,
						     pstUsrRegCfg->u16ReciExp1,
						     pstUsrRegCfg->u16ReciExp2);
			isp_rgbir_gain_u32_write(pstBeReg,
						 pstUsrRegCfg->u16Gain1,
						 pstUsrRegCfg->u16Gain2);
			isp_rgbir_cvt89_u32_write(pstBeReg,
						  pstUsrRegCfg->s16Matrix[8],
						  pstUsrRegCfg->s16Matrix[9]);
			isp_rgbir_cvt67_u32_write(pstBeReg,
						  pstUsrRegCfg->s16Matrix[6],
						  pstUsrRegCfg->s16Matrix[7]);
			isp_rgbir_cvt45_u32_write(pstBeReg,
						  pstUsrRegCfg->s16Matrix[4],
						  pstUsrRegCfg->s16Matrix[5]);
			isp_rgbir_exp_thr_u32_write(pstBeReg,
						    pstUsrRegCfg->u16ExpCtrl1,
						    pstUsrRegCfg->u16ExpCtrl2);
			isp_rgbir_cvt01_u32_write(pstBeReg,
						  pstUsrRegCfg->s16Matrix[0],
						  pstUsrRegCfg->s16Matrix[1]);
			isp_rgbir_cvt1011_u32_write(
				pstBeReg, pstUsrRegCfg->s16Matrix[10],
				pstUsrRegCfg->s16Matrix[11]);
			isp_rgbir_thre_u32_write(pstBeReg,
						 pstUsrRegCfg->u8ThresH,
						 pstUsrRegCfg->u8ThresV);
			isp_rgbir_cvt23_u32_write(pstBeReg,
						  pstUsrRegCfg->s16Matrix[2],
						  pstUsrRegCfg->s16Matrix[3]);

			isp_rgbir_ir_pattern_in_write(
				pstBeReg, pstUsrRegCfg->u8InPattern);
			isp_rgbir_pattern_out_write(pstBeReg,
						    pstUsrRegCfg->u8OutPattern);
			pstUsrRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1RgbirCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_GammaRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				 GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bStt2LutRegnew = GK_FALSE;
	GK_U8 u8BufId;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	ISP_GAMMA_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1GammaCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stGammaCfg.stUsrRegCfg;

		bIdxResh = (isp_gamma_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bGammaLutUpdateEn &
					       bIdxResh) :
					      (pstUsrRegCfg->bGammaLutUpdateEn);

		// LUT update
		if (bUsrResh) {
			isp_gamma_update_index_write(
				pstBeReg, pstUsrRegCfg->u32UpdateIndex);
			isp_gamma_lut_width_word_write(
				pstViProcReg, ISP_GAMMA_LUT_WIDTH_WORD_DEFAULT);

			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				u8BufId = pstUsrRegCfg->u8BufId;

				pstBeLutSttReg = (S_ISP_LUT_WSTT_TYPE *)
					ISP_GetBeLut2SttVirAddr(ViPipe, i,
								u8BufId);
				ISP_CHECK_POINTER(pstBeLutSttReg);

				isp_gamma_lut_wstt_write(
					pstBeLutSttReg,
					pstUsrRegCfg->au16GammaLUT);
				isp_gamma_lut_wstt_addr_write(
					ViPipe, i, u8BufId, pstViProcReg);
				isp_gamma_stt2lut_en_write(pstBeReg, GK_TRUE);
				pstUsrRegCfg->u8BufId = 1 - u8BufId;
			} else {
				isp_gamma_lut_wstt_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstUsrRegCfg->au16GammaLUT);
				isp_gamma_stt2lut_en_write(pstBeReg, GK_TRUE);
			}
			bStt2LutRegnew = GK_TRUE;
			isp_gamma_inseg1_u32_write(
				pstBeReg, pstUsrRegCfg->au16GammaInSeg[2],
				pstUsrRegCfg->au16GammaInSeg[3]);
			isp_gamma_inseg0_u32_write(
				pstBeReg, pstUsrRegCfg->au16GammaInSeg[0],
				pstUsrRegCfg->au16GammaInSeg[1]);
			isp_gamma_inseg3_u32_write(
				pstBeReg, pstUsrRegCfg->au16GammaInSeg[6],
				pstUsrRegCfg->au16GammaInSeg[7]);
			isp_gamma_inseg2_u32_write(
				pstBeReg, pstUsrRegCfg->au16GammaInSeg[4],
				pstUsrRegCfg->au16GammaInSeg[5]);
			isp_gamma_pos1_u32_write(pstBeReg,
						 pstUsrRegCfg->au16GammaPos[2],
						 pstUsrRegCfg->au16GammaPos[3]);
			isp_gamma_pos0_u32_write(pstBeReg,
						 pstUsrRegCfg->au16GammaPos[0],
						 pstUsrRegCfg->au16GammaPos[1]);
			isp_gamma_pos3_u32_write(pstBeReg,
						 pstUsrRegCfg->au16GammaPos[6],
						 pstUsrRegCfg->au16GammaPos[7]);
			isp_gamma_pos2_u32_write(pstBeReg,
						 pstUsrRegCfg->au16GammaPos[4],
						 pstUsrRegCfg->au16GammaPos[5]);
			isp_gamma_step_u32_write(pstBeReg,
						 pstUsrRegCfg->au8GammaStep[0],
						 pstUsrRegCfg->au8GammaStep[1],
						 pstUsrRegCfg->au8GammaStep[2],
						 pstUsrRegCfg->au8GammaStep[3],
						 pstUsrRegCfg->au8GammaStep[4],
						 pstUsrRegCfg->au8GammaStep[5],
						 pstUsrRegCfg->au8GammaStep[6],
						 pstUsrRegCfg->au8GammaStep[7]);

			pstUsrRegCfg->bGammaLutUpdateEn = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1GammaCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bGammaStt2LutRegnew =
		bStt2LutRegnew;

	return GK_SUCCESS;
}
static GK_S32 ISP_CscRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	ISP_CSC_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1CscCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stCscCfg.stDynaRegCfg;

		/* Dynamic */
		if (pstDynaRegCfg->bResh) {
			isp_csc_out_dc0_u32_write(pstBeReg,
						  pstDynaRegCfg->s16CscOutDC0,
						  pstDynaRegCfg->s16CscOutDC1);
			isp_csc_out_dc1_u32_write(pstBeReg,
						  pstDynaRegCfg->s16CscOutDC2);
			isp_csc_coef4_u32_write(pstBeReg,
						pstDynaRegCfg->s16CscCoef22);
			isp_csc_in_dc0_u32_write(pstBeReg,
						 pstDynaRegCfg->s16CscInDC0,
						 pstDynaRegCfg->s16CscInDC1);
			isp_csc_coef0_u32_write(pstBeReg,
						pstDynaRegCfg->s16CscCoef00,
						pstDynaRegCfg->s16CscCoef01);
			isp_csc_coef3_u32_write(pstBeReg,
						pstDynaRegCfg->s16CscCoef20,
						pstDynaRegCfg->s16CscCoef21);
			isp_csc_coef2_u32_write(pstBeReg,
						pstDynaRegCfg->s16CscCoef11,
						pstDynaRegCfg->s16CscCoef12);
			isp_csc_coef1_u32_write(pstBeReg,
						pstDynaRegCfg->s16CscCoef02,
						pstDynaRegCfg->s16CscCoef10);
			isp_csc_in_dc1_u32_write(pstBeReg,
						 pstDynaRegCfg->s16CscInDC2);
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1CscCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_CaRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			      GK_U8 i)
{
#ifdef CONFIG_ISP_CA_SUPPORT
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bStt2LutRegnew = GK_FALSE;
	GK_U8 u8BufId;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	ISP_CA_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_CA_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_CA_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1CaCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		/* usr */
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stCaRegCfg.stUsrRegCfg;
		bIdxResh = (isp_ca_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bResh & bIdxResh) :
					      (pstUsrRegCfg->bResh);

		if (bUsrResh) {
			isp_ca_update_index_write(pstBeReg,
						  pstUsrRegCfg->u32UpdateIndex);
			isp_ca_lumathgh_write(pstBeReg,
					      pstUsrRegCfg->u16CaLumaThrHigh);
			isp_ca_skin_betaratio_write(
				pstBeReg, pstUsrRegCfg->s16CaSkinBetaRatio);
			if (pstUsrRegCfg->bCaLutUpdateEn) {
				if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr
							   .enIspRunningMode) ||
				    IS_SIDEBYSIDE_MODE(
					    pstIspCtx->stBlockAttr
						    .enIspRunningMode)) {
					u8BufId = pstUsrRegCfg->u8BufId;
					pstBeLutSttReg = (S_ISP_LUT_WSTT_TYPE *)
						ISP_GetBeLut2SttVirAddr(
							ViPipe, i, u8BufId);
					ISP_CHECK_POINTER(pstBeLutSttReg);
					isp_ca_lut_wstt_write(
						pstBeLutSttReg,
						pstUsrRegCfg->au16YRatioLUT);
					isp_ca_lut_wstt_addr_write(
						ViPipe, i, u8BufId,
						pstViProcReg);
					isp_ca_stt2lut_en_write(pstBeReg,
								GK_TRUE);
					pstUsrRegCfg->u8BufId = 1 - u8BufId;

				} else {
					isp_ca_lut_wstt_write(
						&pstBeReg->stIspBeLut
							 .stBeLut2Stt,
						pstUsrRegCfg->au16YRatioLUT);
					isp_ca_stt2lut_en_write(pstBeReg,
								GK_TRUE);
				}
			}
			bStt2LutRegnew = GK_TRUE;
			pstUsrRegCfg->bResh = bIsOfflineMode;
		}

		/* dynamic */
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stCaRegCfg.stDynaRegCfg;

		if (pstDynaRegCfg->bResh) {
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		/* static */
		pstStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stCaRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_ca_ctrl_u32_write(pstBeReg,
					      pstStaticRegCfg->bCaSatuAdjEn,
					      pstStaticRegCfg->bCaSkinProcEn,
					      pstStaticRegCfg->bCaLlhcProcEn);
			isp_ca_darkchroma_th_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaDarkChromaThrHigh,
				pstStaticRegCfg->u16CaDarkChromaThrLow);
			isp_ca_skinlluma_uth_u32_write(
				pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMaxU,
				pstStaticRegCfg->u16CaSkinLowLumaMinU);
			isp_ca_skin_ratioth1_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinRatioThrHigh);
			isp_ca_skin_ratioth0_u32_write(
				pstBeReg, pstStaticRegCfg->u16CaSkinRatioThrMid,
				pstStaticRegCfg->u16CaSkinRatioThrLow);
			isp_ca_skin_uvdiff_u32_write(
				pstBeReg, pstStaticRegCfg->s16CaSkinUvDiff);
			isp_ca_skinhluma_vyth_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinHighLumaMaxVy,
				pstStaticRegCfg->u16CaSkinHighLumaMinVy);
			isp_ca_skinlluma_uyth_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinLowLumaMaxUy,
				pstStaticRegCfg->u16CaSkinLowLumaMinUy);
			isp_ca_skinlluma_vth_u32_write(
				pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMaxV,
				pstStaticRegCfg->u16CaSkinLowLumaMinV);
			isp_ca_skinhluma_uth_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinHighLumaMaxU,
				pstStaticRegCfg->u16CaSkinHighLumaMinU);
			isp_ca_sdarkchroma_th_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSDarkChromaThrHigh,
				pstStaticRegCfg->u16CaSDarkChromaThrLow);
			isp_ca_skinlluma_vyth_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinLowLumaMaxVy,
				pstStaticRegCfg->u16CaSkinLowLumaMinVy);
			isp_ca_skinhluma_uyth_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinHighLumaMaxUy,
				pstStaticRegCfg->u16CaSkinHighLumaMinUy);
			isp_ca_skinhluma_vth_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16CaSkinHighLumaMaxV,
				pstStaticRegCfg->u16CaSkinHighLumaMinV);

			isp_ca_lumaratio_low_write(
				pstBeReg, pstStaticRegCfg->s16CaLumaRatioLow);
			isp_ca_lumath_low_write(
				pstBeReg, pstStaticRegCfg->u16CaLumaThrLow);

			isp_ca_lut_width_word_write(
				pstViProcReg, ISP_CA_LUT_WIDTH_WORD_DEFAULT);
			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		pstRegCfgInfo->unKey.bit1CaCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bCaStt2LutRegnew =
		bStt2LutRegnew;
#endif

	return GK_SUCCESS;
}

static GK_S32 ISP_McdsRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				GK_U8 i)
{
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	ISP_MCDS_DYNA_REG_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_MCDS_STATIC_REG_CFG_S *pstStaticRegCfg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1McdsCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stMcdsRegCfg.stDynaRegCfg;
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stMcdsRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_hcds_en_write(pstViProcReg,
					  pstStaticRegCfg->bHcdsEn);
			isp_hcds_coefh2_u32_write(pstBeReg,
						  pstStaticRegCfg->s32HCoef5,
						  pstStaticRegCfg->s32HCoef4);
			isp_hcds_coefh3_u32_write(pstBeReg,
						  pstStaticRegCfg->s32HCoef7,
						  pstStaticRegCfg->s32HCoef6);
			isp_hcds_coefh0_u32_write(pstBeReg,
						  pstStaticRegCfg->s32HCoef1,
						  pstStaticRegCfg->s32HCoef0);
			isp_hcds_coefh1_u32_write(pstBeReg,
						  pstStaticRegCfg->s32HCoef3,
						  pstStaticRegCfg->s32HCoef2);

			isp_sharpen_vcds_filterv_write(
				pstBeReg, pstStaticRegCfg->s32EnFilterV);
			pstStaticRegCfg->bStaticResh = 0;
		}

		if (pstDynaRegCfg->bDynaResh) {
			pstDynaRegCfg->bDynaResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1McdsCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_WdrRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_U16 u16Offset0;
	GK_U32 u32BlcComp0;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	ISP_FSWDR_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_FSWDR_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_FSWDR_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1FsWdrCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		isp_wdr_en_write(
			pstViProcReg,
			pstRegCfgInfo->stAlgRegCfg[i].stWdrRegCfg.bWDREn);
		isp_bnr_wdr_enable_write(
			pstBeReg,
			pstRegCfgInfo->stAlgRegCfg[i].stWdrRegCfg.bWDREn);

		/* static */
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stWdrRegCfg.stStaticRegCfg;
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stWdrRegCfg.stUsrRegCfg;
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stWdrRegCfg.stDynaRegCfg;

		/* static */
		if (pstStaticRegCfg->bFirstFrame == GK_TRUE) {
			isp_wdr_expovalue_u32_write(
				pstBeReg, pstStaticRegCfg->au16ExpoValue[1],
				pstStaticRegCfg->au16ExpoValue[0]);
			isp_wdr_maxratio_u32_write(
				pstBeReg, pstStaticRegCfg->u32MaxRatio);
			isp_wdr_exporratio_u32_write(
				pstBeReg, pstStaticRegCfg->au16ExpoRRatio[0]);
			isp_wdr_fusionmode_write(pstBeReg,
						 pstUsrRegCfg->bFusionMode);
			isp_wdr_mdt_en_write(pstBeReg,
					     pstDynaRegCfg->bWDRMdtEn);
			isp_wdr_wgtidx_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16LongThr,
				pstDynaRegCfg->u16ShortThr);

			u16Offset0 =
				pstRegCfgInfo->stAlgRegCfg[i]
					.stWdrRegCfg.stSyncRegCfg.u16Offset0;
			u32BlcComp0 =
				pstStaticRegCfg->au32BlcComp[0] * u16Offset0;
			pstStaticRegCfg->au32BlcComp[0] = u32BlcComp0;
			isp_wdr_blc_comp_u32_write(pstBeReg, u32BlcComp0);
			isp_wdr_wgt_slope_u32_write(pstBeReg,
						    pstDynaRegCfg->u16WgtSlope);
			pstStaticRegCfg->bFirstFrame = GK_FALSE;
		}

		if (pstStaticRegCfg->bResh) {
			isp_wdr_expovalue_u32_write(
				pstBeReg, pstStaticRegCfg->au16ExpoValue[1],
				pstStaticRegCfg->au16ExpoValue[0]);
			isp_wdr_blc_comp_u32_write(
				pstBeReg, pstStaticRegCfg->au32BlcComp[0]);
			isp_wdr_mask_similar_u32_write(
				pstBeReg, pstStaticRegCfg->u8MaskSimilarCnt,
				pstStaticRegCfg->u8MaskSimilarThr);
			isp_wdr_maxratio_u32_write(
				pstBeReg, pstStaticRegCfg->u32MaxRatio);
			isp_wdr_dftwgt_fl_u32_write(
				pstBeReg, pstStaticRegCfg->u16dftWgtFL);
			isp_wdr_saturate_thr_u32_write(
				pstBeReg, pstStaticRegCfg->u16SaturateThr);
			isp_wdr_exporratio_u32_write(
				pstBeReg, pstStaticRegCfg->au16ExpoRRatio[0]);
			isp_wdr_wgtidx_blendratio_u32_write(
				pstBeReg, pstStaticRegCfg->u8bldrLHFIdx);
			isp_wdr_fusion_sat_thd_u32_write(
				pstBeReg,
				pstStaticRegCfg->u16FusionSaturateThd);

			isp_wdr_grayscale_mode_write(
				pstBeReg, pstStaticRegCfg->bGrayScaleMode);
			isp_wdr_forcelong_smooth_en_write(
				pstBeReg, pstStaticRegCfg->bForceLongSmoothEn);
			isp_wdr_bsaveblc_write(pstBeReg,
					       pstStaticRegCfg->bSaveBLC);
			pstStaticRegCfg->bResh = GK_FALSE;
		}

		/* usr */
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stWdrRegCfg.stUsrRegCfg;
		bIdxResh = (isp_wdr_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bResh & bIdxResh) :
					      (pstUsrRegCfg->bResh);

		if (bUsrResh) {
			isp_wdr_update_index_write(
				pstBeReg, pstUsrRegCfg->u32UpdateIndex);
			isp_wdr_mdt_thr_u32_write(pstBeReg,
						  pstUsrRegCfg->u8MdtFullThr,
						  pstUsrRegCfg->u8MdtStillThr);
			isp_wdr_mdtlbld_u32_write(pstBeReg,
						  pstUsrRegCfg->u8MdtLBld);
			isp_wdr_pix_avg_diff_u32_write(
				pstBeReg, pstUsrRegCfg->u16PixelAvgMaxDiff);

			isp_wdr_shortexpo_chk_write(
				pstBeReg, pstUsrRegCfg->bShortExpoChk);

			pstUsrRegCfg->bResh = bIsOfflineMode;
		}

		if (pstDynaRegCfg->bResh) {
			isp_bcom_en_write(pstViProcReg, pstDynaRegCfg->bBcomEn);
			isp_bdec_en_write(pstViProcReg, pstDynaRegCfg->bBdecEn);

			isp_wdr_mdt_nosf_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16MdtNFHighThr,
				pstDynaRegCfg->u16MdtNFLowThr);
			isp_bdec_alpha_u32_write(pstBeReg,
						 pstDynaRegCfg->u8bdec_alpha);
			isp_wdr_forcelong_slope_u32_write(
				pstBeReg, pstDynaRegCfg->u16ForceLongSlope);
			isp_wdr_fusion_thr_g_u32_write(
				pstBeReg, pstDynaRegCfg->au16FusionThrG[1],
				pstDynaRegCfg->au16FusionThrG[0]);
			isp_wdr_fusion_thr_b_u32_write(
				pstBeReg, pstDynaRegCfg->au16FusionThrB[1],
				pstDynaRegCfg->au16FusionThrB[0]);
			isp_wdr_still_thr_u32_write(
				pstBeReg, pstDynaRegCfg->au16StillThr[0]);
			isp_wdr_gain_sum_thr_u32_write(
				pstBeReg, pstDynaRegCfg->u16GainSumHighThr,
				pstDynaRegCfg->u16GainSumLowThr);
			isp_bcom_alpha_u32_write(pstBeReg,
						 pstDynaRegCfg->u8bcom_alpha);
			isp_wdr_fusion_thr_r_u32_write(
				pstBeReg, pstDynaRegCfg->au16FusionThrR[1],
				pstDynaRegCfg->au16FusionThrR[0]);

			isp_wdr_erosion_en_write(pstBeReg,
						 pstDynaRegCfg->bErosionEn);
			isp_wdr_fusion_data_mode_write(
				pstBeReg, pstDynaRegCfg->bFusionDataMode);
			isp_wdr_forcelong_en_write(pstBeReg,
						   pstDynaRegCfg->bForceLong);
			isp_wdr_forcelonggh_thd_write(
				pstBeReg, pstDynaRegCfg->u16ForceLongHigThr);
			isp_wdr_forcelong_low_thd_write(
				pstBeReg, pstDynaRegCfg->u16ForceLongLowThr);
			isp_wdr_shortchk_thd_write(
				pstBeReg, pstDynaRegCfg->u16ShortCheckThd);

			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1FsWdrCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}
	return GK_SUCCESS;
}
static GK_S32 ISP_DrcRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bTmLutUpdate = GK_FALSE;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 j;
	ISP_DRC_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_DRC_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_DRC_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1DrcCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);

		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		isp_drc_en_write(
			pstViProcReg,
			pstRegCfgInfo->stAlgRegCfg[i].stDrcRegCfg.bDrcEn);

		if (pstRegCfgInfo->stAlgRegCfg[i].stDrcRegCfg.bDrcEn ==
		    GK_TRUE) {
			isp_drc_dither_en_write(pstBeReg, GK_FALSE);
		} else {
			isp_drc_dither_en_write(
				pstBeReg,
				!(pstIspCtx->stHdrAttr.enDynamicRange ==
				  DYNAMIC_RANGE_XDR));
		}

		/* Static */
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stDrcRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bStaticResh) {
			isp_drc_rgb_wgt_u32_write(pstBeReg,
						  pstStaticRegCfg->u8RWgt,
						  pstStaticRegCfg->u8GWgt,
						  pstStaticRegCfg->u8BWgt);
			isp_drc_wgt_box_tri_sel_u32_write(
				pstBeReg, pstStaticRegCfg->bWgtBoxTriSel);
			isp_drc_color_ctrl_u32_write(
				pstBeReg,
				pstStaticRegCfg->u8ColorControlLUTCtrl);
			isp_drc_maxval1_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegMaxVal[4],
				pstStaticRegCfg->au8SegMaxVal[5],
				pstStaticRegCfg->au8SegMaxVal[6],
				pstStaticRegCfg->au8SegMaxVal[7]);
			isp_drc_idxbase1_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegIdxBase[4],
				pstStaticRegCfg->au8SegIdxBase[5],
				pstStaticRegCfg->au8SegIdxBase[6],
				pstStaticRegCfg->au8SegIdxBase[7]);
			isp_drc_idxbase0_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegIdxBase[0],
				pstStaticRegCfg->au8SegIdxBase[1],
				pstStaticRegCfg->au8SegIdxBase[2],
				pstStaticRegCfg->au8SegIdxBase[3]);
			isp_drc_bin_u32_write(pstBeReg,
					      pstStaticRegCfg->u8BinNumZ);
			isp_drc_global_corr_u32_write(
				pstBeReg, pstStaticRegCfg->u16GlobalColorCorr);
			isp_drc_maxval0_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegMaxVal[0],
				pstStaticRegCfg->au8SegMaxVal[1],
				pstStaticRegCfg->au8SegMaxVal[2],
				pstStaticRegCfg->au8SegMaxVal[3]);

			isp_drc_dp_det_en_write(
				pstBeReg, pstStaticRegCfg->bDpDetectEnable);
			isp_drc_mono_chroma_en_write(
				pstBeReg, pstStaticRegCfg->bMonochromeMode);
			isp_drc_rdstat_en_write(pstBeReg,
						pstStaticRegCfg->bRdstatEn);
			isp_drc_wrstat_en_write(pstBeReg,
						pstStaticRegCfg->bWrstatEn);

			if (pstStaticRegCfg->bFirstFrame) {
				isp_drc_shp_cfg_u32_write(
					pstBeReg, pstStaticRegCfg->u8ShpLog,
					pstStaticRegCfg->u8ShpExp);
				isp_drc_div_denom_log_u32_write(
					pstBeReg,
					pstStaticRegCfg->u32DivDenomLog);
				isp_drc_denom_exp_u32_write(
					pstBeReg, pstStaticRegCfg->u32DenomExp);
				isp_drc_prev_luma_0_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[0]);
				isp_drc_prev_luma_1_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[1]);
				isp_drc_prev_luma_2_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[2]);
				isp_drc_prev_luma_3_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[3]);
				isp_drc_prev_luma_4_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[4]);
				isp_drc_prev_luma_5_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[5]);
				isp_drc_prev_luma_6_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[6]);
				isp_drc_prev_luma_7_u32_write(
					pstBeReg,
					pstStaticRegCfg->au32PrevLuma[7]);

				pstStaticRegCfg->bFirstFrame = GK_FALSE;
			}

			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		/* User */
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDrcRegCfg.stUsrRegCfg;
		bIdxResh = (isp_drc_update_index_read(pstBeReg) !=
			    pstUsrRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ?
				   (pstUsrRegCfg->bUsrResh & bIdxResh) :
				   (pstUsrRegCfg->bUsrResh);

		if (bUsrResh) {
			isp_drc_update_index_write(
				pstBeReg, pstUsrRegCfg->u32UpdateIndex);
			isp_drc_grad_rev_u32_write(pstBeReg,
						   pstUsrRegCfg->u8GradThr,
						   pstUsrRegCfg->u8GradMax,
						   pstUsrRegCfg->u8GradSlope,
						   pstUsrRegCfg->u8GradShift);
			isp_drc_bin_factor_medium_0_u32_write(
				pstBeReg, pstUsrRegCfg->au8BinMixMedium[0],
				pstUsrRegCfg->au8BinMixMedium[1],
				pstUsrRegCfg->au8BinMixMedium[2],
				pstUsrRegCfg->au8BinMixMedium[3]);
			isp_drc_bin_factor_medium_1_u32_write(
				pstBeReg, pstUsrRegCfg->au8BinMixMedium[4],
				pstUsrRegCfg->au8BinMixMedium[5],
				pstUsrRegCfg->au8BinMixMedium[6],
				pstUsrRegCfg->au8BinMixMedium[7]);
			isp_drc_bright_gain_lmt_u32_write(pstBeReg,
							  pstUsrRegCfg->u8VAL,
							  pstUsrRegCfg->u8SFT);
			isp_drc_flt_cfg_u32_write(pstBeReg,
						  pstUsrRegCfg->u8FltSpaFine,
						  pstUsrRegCfg->u8FltSpaMedium,
						  pstUsrRegCfg->u8FltSpaCoarse,
						  pstUsrRegCfg->u8FltRngFine,
						  pstUsrRegCfg->u8FltRngMedium,
						  pstUsrRegCfg->u8FltRngCoarse);

			isp_drc_mixing_bright_u32_write(
				pstBeReg, pstUsrRegCfg->u8MixingBrightMin,
				pstUsrRegCfg->u8MixingBrightMax,
				pstUsrRegCfg->u8MixingBrightThr,
				pstUsrRegCfg->s8MixingBrightSlo);
			isp_drc_dark_gain_lmt_y_u32_write(
				pstBeReg, pstUsrRegCfg->u8YVAL1,
				pstUsrRegCfg->u8YSFT1, pstUsrRegCfg->u8YVAL2,
				pstUsrRegCfg->u8YSFT2);
			isp_drc_dp_dtc_u32_write(
				pstBeReg, pstUsrRegCfg->u16DpDetectThrMin,
				pstUsrRegCfg->u8DpDetectThrSlo,
				pstUsrRegCfg->bDpDetectG2RB,
				pstUsrRegCfg->bDpDetectRB2RB,
				pstUsrRegCfg->bDpDetectReplCtr,
				pstUsrRegCfg->u8DpDetectRngRatio);
			isp_drc_vbi_strength_u32_write(
				pstBeReg, pstUsrRegCfg->u8VarRngFine,
				pstUsrRegCfg->u8VarRngMedium,
				pstUsrRegCfg->u8VarRngCoarse,
				pstUsrRegCfg->u8VarSpaFine,
				pstUsrRegCfg->u8VarSpaMedium,
				pstUsrRegCfg->u8VarSpaCoarse);
			isp_drc_dark_gain_lmt_c_u32_write(
				pstBeReg, pstUsrRegCfg->u8CVAL1,
				pstUsrRegCfg->u8CSFT1, pstUsrRegCfg->u8CVAL2,
				pstUsrRegCfg->u8CSFT2);
			isp_drc_suppress_dark_u32_write(
				pstBeReg, pstUsrRegCfg->u8SuppressDarkMax,
				pstUsrRegCfg->u8SuppressDarkMin,
				pstUsrRegCfg->u8SuppressDarkThr,
				pstUsrRegCfg->u8SuppressDarkSlo);
			isp_drc_gain_clip_u32_write(
				pstBeReg, pstUsrRegCfg->u8GainClipStep,
				pstUsrRegCfg->u8GainClipKnee);
			isp_drc_bin_factor_coarse_0_u32_write(
				pstBeReg, pstUsrRegCfg->au8BinMixCoarse[0],
				pstUsrRegCfg->au8BinMixCoarse[1],
				pstUsrRegCfg->au8BinMixCoarse[2],
				pstUsrRegCfg->au8BinMixCoarse[3]);
			isp_drc_bin_factor_coarse_1_u32_write(
				pstBeReg, pstUsrRegCfg->au8BinMixCoarse[4],
				pstUsrRegCfg->au8BinMixCoarse[5],
				pstUsrRegCfg->au8BinMixCoarse[6],
				pstUsrRegCfg->au8BinMixCoarse[7]);
			isp_drc_rng_ada_coef_u32_write(
				pstBeReg, pstUsrRegCfg->u8FltRngAdaMax,
				pstUsrRegCfg->u8DisOffsetCoef,
				pstUsrRegCfg->u8DisThrCoefLow,
				pstUsrRegCfg->u8DisThrCoefHigh);
			isp_drc_suppress_bright_u32_write(
				pstBeReg, pstUsrRegCfg->u8SuppressBrightMax,
				pstUsrRegCfg->u8SuppressBrightMin,
				pstUsrRegCfg->u8SuppressBrightThr,
				pstUsrRegCfg->u8SuppressBrightSlo);
			isp_drc_mixing_dark_u32_write(
				pstBeReg, pstUsrRegCfg->u8MixingDarkMin,
				pstUsrRegCfg->u8MixingDarkMax,
				pstUsrRegCfg->u8MixingDarkThr,
				pstUsrRegCfg->s8MixingDarkSlo);
			isp_drc_detail_sub_factor_u32_write(
				pstBeReg, pstUsrRegCfg->s8DetailSubFactor);
			isp_drc_mixing_coring_u32_write(
				pstBeReg, pstUsrRegCfg->u8MixingCoring);
			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				isp_drc_cclut_waddr_u32_write(pstBeReg, 0);

				for (j = 0; j < ISP_DRC_CC_NODE_NUM; j++) {
					isp_drc_cclut_wdata_u32_write(
						pstBeReg,
						pstUsrRegCfg->u16CCLUT[j]);
				}
			} else {
				isp_drc_cclut_write(pstBeReg,
						    pstUsrRegCfg->u16CCLUT);
			}
			pstUsrRegCfg->bUsrResh = bIsOfflineMode;
		}

		/* Dynamic */
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDrcRegCfg.stDynaRegCfg;

		if (pstDynaRegCfg->bDynaResh) {
			isp_drc_vbiflt_en_write(pstBeReg,
						pstDynaRegCfg->bVbifltEn);
			isp_drc_color_corr_en_write(
				pstBeReg, pstDynaRegCfg->bColorCorrEnable);

			if (IS_OFFLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_STRIPING_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				isp_drc_wrstat_en_write(
					pstBeReg, pstRegCfgInfo->stAlgRegCfg[i]
							  .stDrcRegCfg.bDrcEn);
				isp_drc_rdstat_en_write(
					pstBeReg, pstRegCfgInfo->stAlgRegCfg[i]
							  .stDrcRegCfg.bDrcEn);
			}

			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				if (pstDynaRegCfg->bLutUpdate) {
					isp_drc_tmlut0_waddr_u32_write(pstBeReg,
								       0);
					for (j = 0; j < ISP_DRC_TM_NODE_NUM;
					     j++) {
						isp_drc_tmlut0_wdata_u32_write(
							pstBeReg,
							((pstDynaRegCfg
								  ->au16ToneMappingValue0
									  [j])
							 << 14) |
								(pstDynaRegCfg->au16ToneMappingDiff0
									 [j]));
					}
					bTmLutUpdate = GK_TRUE;
				}
			} else {
				isp_drc_tmlut0_value_write(
					pstBeReg,
					pstDynaRegCfg->au16ToneMappingValue0);
				isp_drc_tmlut0_diff_write(
					pstBeReg,
					pstDynaRegCfg->au16ToneMappingDiff0);
				bTmLutUpdate = GK_TRUE;
			}

			isp_drc_strength_write(pstBeReg,
					       pstDynaRegCfg->u16Strength);

			if (pstDynaRegCfg->bImgSizeChanged) {
				isp_drc_bin_scale_u32_write(
					pstBeReg, pstDynaRegCfg->u8BinScale);
				isp_drc_zone_u32_write(
					pstBeReg, pstDynaRegCfg->u8BlockHNum,
					pstDynaRegCfg->u8BlockVNum);
				isp_drc_stat_width_u32_write(
					pstBeReg,
					pstDynaRegCfg->u16StatWidth - 1,
					pstDynaRegCfg->u16TotalWidth - 1);
				isp_drc_zone_init_u32_write(
					pstBeReg, pstDynaRegCfg->u16AccXInit,
					pstDynaRegCfg->u16CntXInit,
					pstDynaRegCfg->u8IdxXInit,
					pstDynaRegCfg->u8BigXInit);
				isp_drc_blk_wgt_init_u32_write(
					pstBeReg, pstDynaRegCfg->u16WgtXInit);
				isp_drc_zone_div0_u32_write(
					pstBeReg, pstDynaRegCfg->u16DivX0,
					pstDynaRegCfg->u16DivY0);
				isp_drc_zone_div1_u32_write(
					pstBeReg, pstDynaRegCfg->u16DivX1,
					pstDynaRegCfg->u16DivY1);
				isp_drc_zone_size_u32_write(
					pstBeReg,
					pstDynaRegCfg->u16BlockHSize - 1,
					pstDynaRegCfg->u8BlockChkX,
					pstDynaRegCfg->u16BlockVSize - 1,
					pstDynaRegCfg->u8BlockChkY);
				pstDynaRegCfg->bImgSizeChanged = bIsOfflineMode;
			}

			pstDynaRegCfg->bDynaResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1DrcCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stBeLutUpdateCfg.bDrcTmLutUpdate =
		bTmLutUpdate | bIsOfflineMode;

	return GK_SUCCESS;
}

static GK_S32 ISP_DehazeRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				  GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_U8 u8BufId;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 u16BlkNum;
	GK_BOOL bStt2LutRegnew = GK_FALSE;
	ISP_DEHAZE_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_DEHAZE_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1DehazeCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stDehazeRegCfg.stStaticRegCfg;
		pstDynaRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					 .stDehazeRegCfg.stDynaRegCfg;

		u16BlkNum = ((pstStaticRegCfg->u8Dchnum + 1) *
				     (pstStaticRegCfg->u8Dcvnum + 1) +
			     1) /
			    2;
		/* Static Registers */
		if (pstStaticRegCfg->bResh) {
			isp_dehaze_blk_sum_u32_write(
				pstBeReg, pstStaticRegCfg->u16BlockSum);
			isp_dehaze_blthld_u32_write(
				pstBeReg, pstStaticRegCfg->u16DehazeBlthld);
			isp_dehaze_dc_size_u32_write(pstBeReg,
						     pstStaticRegCfg->u8Dcvnum,
						     pstStaticRegCfg->u8Dchnum);
			isp_dehaze_lut_width_word_write(
				pstViProcReg,
				ISP_DEHAZE_LUT_WIDTH_WORD_DEFAULT);
			pstStaticRegCfg->bResh = GK_FALSE;
		}

		/* Dynamic Regs */
		if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		    IS_SIDEBYSIDE_MODE(
			    pstIspCtx->stBlockAttr.enIspRunningMode)) {
			/* online Lut2stt regconfig */
			u8BufId = pstDynaRegCfg->u8BufId;
			pstBeLutSttReg =
				(S_ISP_LUT_WSTT_TYPE *)ISP_GetBeLut2SttVirAddr(
					ViPipe, i, u8BufId);
			ISP_CHECK_POINTER(pstBeLutSttReg);
			isp_dehaze_lut_wstt_write(pstBeLutSttReg, u16BlkNum,
						  pstDynaRegCfg->prestat,
						  pstDynaRegCfg->u8Lut);
			isp_dehaze_lut_wstt_addr_write(ViPipe, i, u8BufId,
						       pstViProcReg);
			isp_dehaze_stt2lut_en_write(pstBeReg, GK_TRUE);
			pstDynaRegCfg->u8BufId = 1 - u8BufId;
		} else {
			isp_dehaze_lut_wstt_write(
				&pstBeReg->stIspBeLut.stBeLut2Stt, u16BlkNum,
				pstDynaRegCfg->prestat, pstDynaRegCfg->u8Lut);
			isp_dehaze_stt2lut_en_write(pstBeReg, GK_TRUE);
		}
		bStt2LutRegnew = GK_TRUE;
		isp_dehaze_stat_num_u32_write(pstBeReg,
					      pstDynaRegCfg->u8StatnumV,
					      pstDynaRegCfg->u8StatnumH);
		isp_dehaze_thld_u32_write(pstBeReg,
					  pstDynaRegCfg->u16DehazeThldB,
					  pstDynaRegCfg->u16DehazeThldG,
					  pstDynaRegCfg->u16DehazeThldR);
		isp_dehaze_gstrth_u32_write(pstBeReg,
					    pstDynaRegCfg->u8Strength);
		isp_dehaze_blk_size_u32_write(pstBeReg,
					      pstDynaRegCfg->u16Blockvsize,
					      pstDynaRegCfg->u16Blockhsize);
		isp_dehaze_stat_point_u32_write(pstBeReg,
						pstDynaRegCfg->u32StatStartX,
						pstDynaRegCfg->u32StatEndX);
		isp_dehaze_y_u32_write(pstBeReg, pstDynaRegCfg->u32phasey);
		isp_dehaze_x_u32_write(pstBeReg, pstDynaRegCfg->u32phasex);
		isp_dehaze_smlmapoffset_u32_write(
			pstBeReg, pstDynaRegCfg->u32smlMapOffset);
		pstRegCfgInfo->unKey.bit1DehazeCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}
	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bDehazeStt2LutRegnew =
		bStt2LutRegnew;

	return GK_SUCCESS;
}

static GK_VOID ISP_BayerNR_DivideLut_U16(GK_U16 lut[], GK_U16 lut_even[],
					 GK_U16 lut_odd[], GK_U32 length)
{
	GK_U32 i;
	for (i = 0; i < (length + 1) / 2; i++) {
		lut_even[i] = lut[i * 2];
	}

	for (i = 0; i < (length - 1) / 2; i++) {
		lut_odd[i] = lut[i * 2 + 1];
	}
}

static GK_VOID ISP_BayerNR_DivideLut_U8(GK_U8 lut[], GK_U8 lut_even[],
					GK_U8 lut_odd[], GK_U32 length)
{
	GK_U32 i;
	for (i = 0; i < (length + 1) / 2; i++) {
		lut_even[i] = lut[i * 2];
	}

	for (i = 0; i < (length - 1) / 2; i++) {
		lut_odd[i] = lut[i * 2 + 1];
	}
}
static GK_S32 ISP_BayerNrRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				   GK_U8 i)
{
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	GK_BOOL bStt2LutRegnew = GK_FALSE;
	GK_U8 u8BufId;
	ISP_BAYERNR_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_BAYERNR_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_BAYERNR_USR_CFG_S *pstUsrRegCfg = GK_NULL;
	S_ISP_LUT_WSTT_TYPE *pstBeLutSttReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1BayernrCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);

		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stBnrRegCfg.stStaticRegCfg;
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stBnrRegCfg.stDynaRegCfg;
		pstUsrRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stBnrRegCfg.stUsrRegCfg;

		if (pstStaticRegCfg->bResh) { /* satic */
			isp_bnr_lut_width_word_write(
				pstViProcReg, ISP_BNR_LUT_WIDTH_WORD_DEFAULT);
			isp_bnr_jnlm2_u32_write(
				pstBeReg, pstStaticRegCfg->u16JnlmMaxWtCoef);

			isp_bnr_jnlm_sel_write(pstBeReg,
					       pstStaticRegCfg->u8JnlmSel);
			isp_bnr_bilateral_enable_write(
				pstBeReg, pstStaticRegCfg->u8BilateralEnable);
			isp_bnr_enablepostproc_write(
				pstBeReg, pstStaticRegCfg->u8EnablePostProc);
			isp_bnr_enablesymmsad_write(
				pstBeReg, pstStaticRegCfg->bEnableSymmsad);
			isp_bnr_gainsad_write(pstBeReg,
					      pstStaticRegCfg->u8Gainsad);
			isp_bnr_implsnrenable1_write(
				pstBeReg, pstStaticRegCfg->bImplsnrEnable1);
			isp_bnr_implsnrenable2_write(
				pstBeReg, pstStaticRegCfg->bImplsnrEnable2);
			isp_bnr_implsnrenable3_write(
				pstBeReg, pstStaticRegCfg->bImplsnrEnable3);
			isp_bnr_implsnrstrength_write(
				pstBeReg, pstStaticRegCfg->u8ImplsnrStrength);
			isp_bnr_lumasel_write(pstBeReg,
					      pstStaticRegCfg->bLumaSel);
			isp_bnr_pattern_noise_reduction_write(
				pstBeReg,
				pstStaticRegCfg->bPatternNoiseReduction);
			isp_bnr_windowsizesel_write(
				pstBeReg, pstStaticRegCfg->u8WindowSizeSel);
			pstStaticRegCfg->bResh = GK_FALSE;
		}

		if (pstDynaRegCfg->bResh) {
			isp_bnr_shotratio_u32_write(
				pstBeReg, pstDynaRegCfg->u16ShotRatio);

			isp_bnr_rlmt_bgain_write(pstBeReg,
						 pstDynaRegCfg->u16RLmtBgain);
			isp_bnr_rlmt_rgain_write(pstBeReg,
						 pstDynaRegCfg->u16RLmtRgain);
			isp_bnr_jnlm_coringhig_write(
				pstBeReg, pstDynaRegCfg->u16JnlmCoringHig);
			isp_bnr_wdr_enfusion_write(pstBeReg,
						   pstDynaRegCfg->bWdrFusionEn);
			isp_bnr_wdr_mapfltmod_write(
				pstBeReg, pstDynaRegCfg->u8WdrMapFltMode);
			isp_bnr_wdr_mapgain_write(pstBeReg,
						  pstDynaRegCfg->u8WdrMapGain);

			ISP_BayerNR_DivideLut_U8(
				pstDynaRegCfg->au8JnlmLimitSLut,
				pstDynaRegCfg->au8JnlmLimitSEvenLut,
				pstDynaRegCfg->au8JnlmLimitSOddLut, 129);
			ISP_BayerNR_DivideLut_U8(
				pstDynaRegCfg->au8JnlmLimitLut,
				pstDynaRegCfg->au8JnlmLimitEvenLut,
				pstDynaRegCfg->au8JnlmLimitOddLut, 129);
			ISP_BayerNR_DivideLut_U16(
				pstDynaRegCfg->au16JnlmCoringLowLut,
				pstDynaRegCfg->au16JnlmCoringLowEvenLut,
				pstDynaRegCfg->au16JnlmCoringLowOddLut, 33);
			ISP_BayerNR_DivideLut_U8(
				pstDynaRegCfg->au8GcoringLut,
				pstDynaRegCfg->au8GcoringEvenLut,
				pstDynaRegCfg->au8GcoringOddLut, 33);
			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				u8BufId = pstDynaRegCfg->u8BufId;

				pstBeLutSttReg = (S_ISP_LUT_WSTT_TYPE *)
					ISP_GetBeLut2SttVirAddr(ViPipe, i,
								u8BufId);
				ISP_CHECK_POINTER(pstBeLutSttReg);

				isp_bnr_lmt_even_wlut_write(
					pstBeLutSttReg,
					pstDynaRegCfg->au8JnlmLimitSEvenLut,
					pstDynaRegCfg->au8JnlmLimitEvenLut);
				isp_bnr_lmt_odd_wlut_write(
					pstBeLutSttReg,
					pstDynaRegCfg->au8JnlmLimitSOddLut,
					pstDynaRegCfg->au8JnlmLimitOddLut);

				isp_bnr_cor_even_wlut_write(
					pstBeLutSttReg,
					pstDynaRegCfg->au8GcoringEvenLut,
					pstDynaRegCfg->au16JnlmCoringLowEvenLut);
				isp_bnr_cor_odd_wlut_write(
					pstBeLutSttReg,
					pstDynaRegCfg->au8GcoringOddLut,
					pstDynaRegCfg->au16JnlmCoringLowOddLut);

				isp_bnr_lut_wstt_addr_write(ViPipe, i, u8BufId,
							    pstViProcReg);
				isp_bnr_stt2lut_en_write(pstBeReg, GK_TRUE);

				pstDynaRegCfg->u8BufId = 1 - u8BufId;
			} else {
				isp_bnr_lmt_even_wlut_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstDynaRegCfg->au8JnlmLimitSEvenLut,
					pstDynaRegCfg->au8JnlmLimitEvenLut);
				isp_bnr_lmt_odd_wlut_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstDynaRegCfg->au8JnlmLimitSOddLut,
					pstDynaRegCfg->au8JnlmLimitOddLut);
				isp_bnr_cor_even_wlut_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstDynaRegCfg->au8GcoringEvenLut,
					pstDynaRegCfg->au16JnlmCoringLowEvenLut);
				isp_bnr_cor_odd_wlut_write(
					&pstBeReg->stIspBeLut.stBeLut2Stt,
					pstDynaRegCfg->au8GcoringOddLut,
					pstDynaRegCfg->au16JnlmCoringLowOddLut);
				isp_bnr_stt2lut_en_write(pstBeReg, GK_TRUE);
			}

			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		if (pstUsrRegCfg->bResh) {
			isp_bnr_cfg_u32_write(pstBeReg,
					      pstUsrRegCfg->bBnrMonoSensorEn);

			isp_bnr_lsccmpstrength_write(
				pstBeReg, pstUsrRegCfg->u16BnrLscCmpStrength);
			isp_bnr_lscmaxgain_write(pstBeReg,
						 pstUsrRegCfg->u8BnrLscMaxGain);

			pstUsrRegCfg->bResh = bIsOfflineMode;
		}
		bStt2LutRegnew = GK_TRUE;

		pstRegCfgInfo->unKey.bit1BayernrCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg.bBnrStt2LutRegnew =
		bStt2LutRegnew;

	return GK_SUCCESS;
}

static GK_S32 ISP_DgRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			      GK_U8 i)
{
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	ISP_DG_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_DG_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1DgCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);
		isp_dg_en_write(pstViProcReg,
				pstRegCfgInfo->stAlgRegCfg[i].stDgRegCfg.bDgEn);

		// static
		pstStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDgRegCfg.stStaticRegCfg;
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stDgRegCfg.stDynaRegCfg;

		if (pstStaticRegCfg->bResh) {
			isp_dg_gain2_u32_write(pstBeReg,
					       pstDynaRegCfg->u16GainGB,
					       pstDynaRegCfg->u16GainB);
			isp_dg_gain1_u32_write(pstBeReg,
					       pstDynaRegCfg->u16GainGR,
					       pstDynaRegCfg->u16GainR);
			pstStaticRegCfg->bResh = GK_FALSE;
		}

		// dynamic
		if (pstDynaRegCfg->bResh) {
			isp_dg_clip_value_write(pstBeReg,
						pstDynaRegCfg->u32ClipValue);
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1DgCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_4DgRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			       GK_U8 i)
{
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_BOOL bIsOfflineMode;
	ISP_4DG_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	ISP_4DG_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1WDRDgCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		// static
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .st4DgRegCfg.stStaticRegCfg;

		if (pstStaticRegCfg->bResh) {
			isp_4dg_0_gain2_u32_write(pstBeReg,
						  pstStaticRegCfg->u16GainGB0,
						  pstStaticRegCfg->u16GainB0);
			isp_4dg_1_gain2_u32_write(pstBeReg,
						  pstStaticRegCfg->u16GainGB1,
						  pstStaticRegCfg->u16GainB1);
			isp_4dg_1_gain1_u32_write(pstBeReg,
						  pstStaticRegCfg->u16GainGR1,
						  pstStaticRegCfg->u16GainR1);
			isp_4dg_0_gain1_u32_write(pstBeReg,
						  pstStaticRegCfg->u16GainGR0,
						  pstStaticRegCfg->u16GainR0);
			pstStaticRegCfg->bResh = GK_FALSE;
		}

		// dynamic
		pstDynaRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].st4DgRegCfg.stDynaRegCfg;

		if (pstDynaRegCfg->bResh) {
			isp_4dg0_clip_value_write(pstBeReg,
						  pstDynaRegCfg->u32ClipValue0);
			isp_4dg1_clip_value_write(pstBeReg,
						  pstDynaRegCfg->u32ClipValue1);
			pstDynaRegCfg->bResh = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1WDRDgCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_PreGammaRegConfig(VI_PIPE ViPipe,
				    ISP_REG_CFG_S *pstRegCfgInfo, GK_U8 i)
{
#ifdef CONFIG_ISP_PREGAMMA_SUPPORT
	GK_BOOL bIsOfflineMode;
	GK_BOOL bUsrResh = GK_FALSE;
	GK_BOOL bIdxResh = GK_FALSE;
	GK_BOOL bLutUpdate = GK_FALSE;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 k;
	ISP_PREGAMMA_DYNA_CFG_S *pstDynaRegCfg = GK_NULL;
	ISP_PREGAMMA_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1PreGammaCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		ISP_CHECK_POINTER(pstViProcReg);
		isp_drc_pregamma_en_write(
			pstBeReg,
			pstRegCfgInfo->stAlgRegCfg[i].stPreGammaCfg.bPreGammaEn);

		pstDynaRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					 .stPreGammaCfg.stDynaRegCfg;
		pstStaticRegCfg = &pstRegCfgInfo->stAlgRegCfg[i]
					   .stPreGammaCfg.stStaticRegCfg;

		// Enable PreGamma
		if (pstStaticRegCfg->bStaticResh) {
			isp_drc_pregamma_maxval5_u32_write(
				pstBeReg, pstStaticRegCfg->au16SegMaxVal[10],
				pstStaticRegCfg->au16SegMaxVal[11]);
			isp_drc_pregamma_idxbase2_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegIdxBase[8],
				pstStaticRegCfg->au8SegIdxBase[9],
				pstStaticRegCfg->au8SegIdxBase[10],
				pstStaticRegCfg->au8SegIdxBase[11]);
			isp_drc_pregamma_idxbase1_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegIdxBase[4],
				pstStaticRegCfg->au8SegIdxBase[5],
				pstStaticRegCfg->au8SegIdxBase[6],
				pstStaticRegCfg->au8SegIdxBase[7]);
			isp_drc_pregamma_idxbase0_u32_write(
				pstBeReg, pstStaticRegCfg->au8SegIdxBase[0],
				pstStaticRegCfg->au8SegIdxBase[1],
				pstStaticRegCfg->au8SegIdxBase[2],
				pstStaticRegCfg->au8SegIdxBase[3]);
			isp_drc_pregamma_maxval1_u32_write(
				pstBeReg, pstStaticRegCfg->au16SegMaxVal[2],
				pstStaticRegCfg->au16SegMaxVal[3]);
			isp_drc_pregamma_maxval0_u32_write(
				pstBeReg, pstStaticRegCfg->au16SegMaxVal[0],
				pstStaticRegCfg->au16SegMaxVal[1]);
			isp_drc_pregamma_maxval3_u32_write(
				pstBeReg, pstStaticRegCfg->au16SegMaxVal[6],
				pstStaticRegCfg->au16SegMaxVal[7]);
			isp_drc_pregamma_maxval2_u32_write(
				pstBeReg, pstStaticRegCfg->au16SegMaxVal[4],
				pstStaticRegCfg->au16SegMaxVal[5]);
			isp_drc_pregamma_maxval4_u32_write(
				pstBeReg, pstStaticRegCfg->au16SegMaxVal[8],
				pstStaticRegCfg->au16SegMaxVal[9]);
			pstStaticRegCfg->bStaticResh = GK_FALSE;
		}

		bIdxResh = (isp_pregamma_update_index_read(pstBeReg) !=
			    pstDynaRegCfg->u32UpdateIndex);
		bUsrResh = (bIsOfflineMode) ?
				   (pstDynaRegCfg->bPreGammaLutUpdateEn &
				    bIdxResh) :
				   (pstDynaRegCfg->bPreGammaLutUpdateEn);

		// LUT update
		if (bUsrResh) {
			isp_pregamma_update_index_write(
				pstBeReg, pstDynaRegCfg->u32UpdateIndex);

			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				isp_drc_pregamma_waddr_u32_write(pstBeReg, 0);
				for (k = 0; k < PREGAMMA_NODE_NUM; k++) {
					isp_drc_pregamma_wdata_u32_write(
						pstBeReg,
						(GK_U32)pstDynaRegCfg
							->au16PreGammaLUT[k]);
				}
				bLutUpdate = GK_TRUE;
			} else {
				isp_drc_pregamma_write(
					pstBeReg,
					pstDynaRegCfg->au16PreGammaLUT);
				bLutUpdate = GK_TRUE;
			}

			pstDynaRegCfg->bPreGammaLutUpdateEn = bIsOfflineMode;
		}

		pstRegCfgInfo->unKey.bit1PreGammaCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	pstRegCfgInfo->stAlgRegCfg[i].stBeLutUpdateCfg.bPreGammaLutUpdate =
		bLutUpdate | bIsOfflineMode;

#endif
	return GK_SUCCESS;
}

static GK_S32 ISP_BeBlcRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				 GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	isp_be_blc_static_cfg *static_cfg = GK_NULL;
	isp_be_blc_dyna_cfg *dyna_cfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1BeBlcCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);
		static_cfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stBeBlcCfg.static_blc;
		dyna_cfg = &pstRegCfgInfo->stAlgRegCfg[i].stBeBlcCfg.dyna_blc;

		if (static_cfg->bReshStatic) {
			isp_wb_blc_cfg_u32_write(pstBeReg,
						 static_cfg->stWbBlc.blc_in,
						 static_cfg->stWbBlc.blc_out);
			isp_4dg_blc_cfg_u32_write(
				pstBeReg, static_cfg->st4DgBlc[0].blc_in,
				static_cfg->st4DgBlc[0].blc_out);
			isp_dg_blc_cfg_u32_write(pstBeReg,
						 static_cfg->stDgBlc.blc_in,
						 static_cfg->stDgBlc.blc_out);
			isp_lsc_blcen_u32_write(pstBeReg,
						static_cfg->stLscBlc.blc_in,
						static_cfg->stLscBlc.blc_out);

			isp_ae_blc_en_write(pstBeReg,
					    static_cfg->stAeBlc.blc_in);
			isp_la_blc_en_write(pstBeReg,
					    static_cfg->stMgBlc.blc_in);
			isp_rgbir_blc_in_en_write(
				pstBeReg, static_cfg->stRgbirBlc.blc_in);
			isp_rgbir_blc_out_en_write(
				pstBeReg, static_cfg->stRgbirBlc.blc_out);
			isp_wdr_bsaveblc_write(pstBeReg,
					       static_cfg->stWdrBlc[0].blc_out);
			static_cfg->bReshStatic = bIsOfflineMode;
		}

		if (pstRegCfgInfo->stAlgRegCfg[i].stBeBlcCfg.resh_dyna_init ==
		    GK_TRUE) {
			isp_la_offset_r_u32_write(pstBeReg,
						  dyna_cfg->stMgBlc.blc[0]);
			isp_la_offset_gr_u32_write(pstBeReg,
						   dyna_cfg->stMgBlc.blc[1]);
			isp_la_offset_gb_u32_write(pstBeReg,
						   dyna_cfg->stMgBlc.blc[2]);
			isp_la_offset_b_u32_write(pstBeReg,
						  dyna_cfg->stMgBlc.blc[3]);

			isp_lsc_blc0_u32_write(pstBeReg,
					       dyna_cfg->stLscBlc.blc[0],
					       dyna_cfg->stLscBlc.blc[1]);
			isp_lsc_blc1_u32_write(pstBeReg,
					       dyna_cfg->stLscBlc.blc[3],
					       dyna_cfg->stLscBlc.blc[2]);
			isp_dg_blc_offset1_u32_write(pstBeReg,
						     dyna_cfg->stDgBlc.blc[1],
						     dyna_cfg->stDgBlc.blc[0]);
			isp_dg_blc_offset2_u32_write(pstBeReg,
						     dyna_cfg->stDgBlc.blc[2],
						     dyna_cfg->stDgBlc.blc[3]);
			isp_ae_offset_r_gr_u32_write(pstBeReg,
						     dyna_cfg->stAeBlc.blc[0],
						     dyna_cfg->stAeBlc.blc[1]);
			isp_ae_offset_gb_b_u32_write(pstBeReg,
						     dyna_cfg->stAeBlc.blc[2],
						     dyna_cfg->stAeBlc.blc[3]);

			isp_wb_blc_offset1_u32_write(pstBeReg,
						     dyna_cfg->stWbBlc.blc[1],
						     dyna_cfg->stWbBlc.blc[0]);
			isp_wb_blc_offset2_u32_write(pstBeReg,
						     dyna_cfg->stWbBlc.blc[2],
						     dyna_cfg->stWbBlc.blc[3]);
			isp_rgbir_blc_offset_r_u32_write(
				pstBeReg, dyna_cfg->stRgbirBlc.blc[0]);
			isp_rgbir_blc_offset_g_u32_write(
				pstBeReg, dyna_cfg->stRgbirBlc.blc[1]);
			isp_rgbir_blc_offset_b_u32_write(
				pstBeReg, dyna_cfg->stRgbirBlc.blc[2]);
			isp_rgbir_blc_offset_ir_u32_write(
				pstBeReg, dyna_cfg->stRgbirBlc.blc[3]);

			isp_4dg_0_blc_offset1_u32_write(
				pstBeReg, dyna_cfg->st4DgBlc[0].blc[1],
				dyna_cfg->st4DgBlc[0].blc[0]);
			isp_4dg_0_blc_offset2_u32_write(
				pstBeReg, dyna_cfg->st4DgBlc[0].blc[2],
				dyna_cfg->st4DgBlc[0].blc[3]);
			isp_4dg_1_blc_offset1_u32_write(
				pstBeReg, dyna_cfg->st4DgBlc[1].blc[1],
				dyna_cfg->st4DgBlc[1].blc[0]);
			isp_4dg_1_blc_offset2_u32_write(
				pstBeReg, dyna_cfg->st4DgBlc[1].blc[2],
				dyna_cfg->st4DgBlc[1].blc[3]);

			isp_bnr_rlmt_blc_write(
				pstBeReg, (dyna_cfg->stBnrBlc.blc[0] >> 2));

			isp_wdr_f0_inblc0_u32_write(
				pstBeReg, dyna_cfg->stWdrBlc[0].blc[1],
				dyna_cfg->stWdrBlc[0].blc[0]);
			isp_wdr_f0_inblc1_u32_write(
				pstBeReg, dyna_cfg->stWdrBlc[0].blc[3],
				dyna_cfg->stWdrBlc[0].blc[2]);
			isp_wdr_f1_inblc0_u32_write(
				pstBeReg, dyna_cfg->stWdrBlc[1].blc[1],
				dyna_cfg->stWdrBlc[1].blc[0]);
			isp_wdr_f1_inblc1_u32_write(
				pstBeReg, dyna_cfg->stWdrBlc[1].blc[3],
				dyna_cfg->stWdrBlc[1].blc[2]);
			isp_wdr_outblc_write(pstBeReg,
					     dyna_cfg->stWdrBlc[0].out_blc);

			pstRegCfgInfo->stAlgRegCfg[i].stBeBlcCfg.resh_dyna_init =
				GK_FALSE;
		}

		pstRegCfgInfo->unKey.bit1BeBlcCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_ExpanderRegConfig(VI_PIPE ViPipe,
				    ISP_REG_CFG_S *pstRegCfgInfo, GK_U8 i)
{
	GK_BOOL bIsOfflineMode;
	GK_U8 u8BlkNum = pstRegCfgInfo->u8CfgNum;
	GK_U16 j;
	ISP_EXPANDER_STATIC_CFG_S *pstStaticRegCfg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	bIsOfflineMode =
		(IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
		 IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode));

	if (pstRegCfgInfo->unKey.bit1ExpanderCfg) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstBeReg);

		pstStaticRegCfg =
			&pstRegCfgInfo->stAlgRegCfg[i].stExpanderCfg.stStaticCfg;

		if (pstStaticRegCfg->bResh) {
			isp_expander_bitw_u32_write(
				pstBeReg, pstStaticRegCfg->u8BitDepthOut,
				pstStaticRegCfg->u8BitDepthIn);
			if (IS_ONLINE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode) ||
			    IS_SIDEBYSIDE_MODE(
				    pstIspCtx->stBlockAttr.enIspRunningMode)) {
				isp_expander_lut_waddr_u32_write(pstBeReg, 0);
				for (j = 0; j < EXPANDER_NODE_NUM; j++) {
					isp_expander_lut_wdata_u32_write(
						pstBeReg,
						pstStaticRegCfg->au16Lut[j]);
				}
			} else {
				isp_expander_lut_write(
					pstBeReg, pstStaticRegCfg->au16Lut);
			}

			pstStaticRegCfg->bResh = GK_FALSE;
		}

		pstRegCfgInfo->unKey.bit1ExpanderCfg =
			bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_FeUpdateRegConfig(VI_PIPE ViPipe,
				    ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U32 i;
	VI_PIPE ViPipeBind;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num; i++) {
		ViPipeBind = pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
		ISP_CHECK_PIPE(ViPipeBind);
		pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
		ISP_CHECK_POINTER(pstFeReg);

		isp_fe_update_mode_write(pstFeReg, GK_FALSE);
		isp_fe_update_write(pstFeReg, GK_TRUE);

		if (pstRegCfgInfo->stAlgRegCfg[0]
			    .stFeLutUpdateCfg.bAe1LutUpdate) {
			isp_ae1_lut_update_write(
				pstFeReg,
				pstRegCfgInfo->stAlgRegCfg[0]
					.stFeLutUpdateCfg.bAe1LutUpdate);
		}
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_FeSystemRegConfig(VI_PIPE ViPipe,
				    ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_BOOL bIspCropEn = GK_FALSE;
	GK_U8 u8RggbCfg;
	GK_U32 i;
	GK_S32 s32X, s32Y;
	GK_U32 u32Width, u32Height;
	GK_U32 u32PipeW, u32PipeH;
	VI_PIPE ViPipeBind, ViPipeId;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	ViPipeId = pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[0];
	u8RggbCfg = ext_system_rggb_cfg_read(ViPipeId);

	s32X = pstIspCtx->stSysRect.s32X;
	s32Y = pstIspCtx->stSysRect.s32Y;
	u32Width = pstIspCtx->stSysRect.u32Width;
	u32Height = pstIspCtx->stSysRect.u32Height;
	u32PipeW = pstIspCtx->stPipeSize.u32Width;
	u32PipeH = pstIspCtx->stPipeSize.u32Height;

	/* ISP crop low-power process */
	if ((s32X == 0) && (s32Y == 0) && (u32Width == u32PipeW) &&
	    (u32Height == u32PipeH)) {
		bIspCropEn = GK_FALSE;
	} else {
		bIspCropEn = GK_TRUE;
	}

	for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num; i++) {
		ViPipeBind = pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
		ISP_CHECK_PIPE(ViPipeBind);
		pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
		ISP_CHECK_POINTER(pstFeReg);

		/* ISP FE/BE Set Offline Mode */
		/* isp regs uptate mode:   0: update; 1:frame */

		isp_fe_fix_timing_write(pstFeReg, ISP_FE_FIX_TIMING_STAT);
		isp_fe_crop_en_write(pstFeReg, bIspCropEn);
		isp_fe_rggb_cfg_write(pstFeReg, u8RggbCfg);
		isp_fe_width_write(pstFeReg, u32PipeW - 1);
		isp_fe_height_write(pstFeReg, u32PipeH - 1);
		isp_fe_hsync_mode_write(pstFeReg, 0);
		isp_fe_vsync_mode_write(pstFeReg, 0);

		isp_crop_sizeout_u32_write(pstFeReg, u32Width - 1,
					   u32Height - 1);
		isp_crop_pos_u32_write(pstFeReg, s32X, s32Y);
		isp_fe_blk_size_u32_write(pstFeReg, u32PipeW - 1, u32PipeH - 1);

		isp_fe_blk_f_hblank_write(pstFeReg, 0);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_RegDefault(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
			     GK_U8 i)
{
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);
	ISP_CHECK_POINTER(pstViProcReg);
	isp_yuv444_sum_cfg_u32_write(pstBeReg, ISP_YUV444_SUM_EN_DEFAULT);
	isp_wdr_sum_cfg_u32_write(pstBeReg, ISP_WDR_SUM_EN_DEFAULT);
	isp_blk_hblank_u32_write(pstBeReg, ISP_BLK_B_HBLANK_DEFAULT,
				 ISP_BLK_F_HBLANK_DEFAULT);
	isp_clip_c_cfg_u32_write(pstBeReg, ISP_CLIP_C_MIN_DEFAULT,
				 ISP_CLIP_C_MAX_DEFAULT);
	isp_clip_y_cfg_u32_write(pstBeReg, ISP_CLIP_Y_MIN_DEFAULT,
				 ISP_CLIP_Y_MAX_DEFAULT);
	isp_yuv422_sum_cfg_u32_write(pstBeReg, ISP_YUV422_SUM_EN_DEFAULT);
	isp_demosaic_sum_cfg_u32_write(pstBeReg, ISP_DEMOSAIC_SUM_EN_DEFAULT);
	isp_blk_vblank_u32_write(pstBeReg, ISP_BLK_B_VBLANK_DEFAULT,
				 ISP_BLK_F_VBLANK_DEFAULT);
	isp_csc_sum_cfg_u32_write(pstBeReg, ISP_CSC_SUM_EN_DEFAULT);
	if (pstIspCtx->stHdrAttr.enDynamicRange == DYNAMIC_RANGE_XDR) {
		isp_sqrt_en_write(pstViProcReg, GK_TRUE); // sqrt in raw
		isp_sq_en_write(pstViProcReg, GK_TRUE); // sq in rgb
		isp_sqrt1_en_write(pstViProcReg, GK_TRUE); // sqrt in rgb
	} else {
		isp_sqrt_en_write(pstViProcReg, GK_FALSE); // sqrt in raw
		isp_sq_en_write(pstViProcReg, GK_FALSE); // sq in rgb
		isp_sqrt1_en_write(pstViProcReg, GK_FALSE); // sqrt in rgb
	}

	isp_blc_en_write(pstViProcReg, GK_FALSE);
	isp_split_en_write(pstViProcReg, GK_FALSE);

	return GK_SUCCESS;
}

static GK_S32 ISP_SystemRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				  GK_U8 i)
{
	GK_U32 u32RggbCfg;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	u32RggbCfg = ext_system_rggb_cfg_read(ViPipe);
	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);
	ISP_CHECK_POINTER(pstViProcReg);

	isp_be_rggb_cfg_write(pstViProcReg, u32RggbCfg);

	if ((IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) ||
	    (IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))) {
		isp_stt_en_write(pstBeReg, GK_TRUE);
	} else {
		isp_stt_en_write(pstBeReg, GK_FALSE);
	}

	if (pstRegCfgInfo->stAlgRegCfg[i]
		    .stFpnRegCfg.stDynaRegCfg.u32IspFpnCalibCorr ==
	    1) { // fpn calib mode
		isp_sumy_en_write(pstViProcReg, GK_FALSE);
	} else {
		isp_sumy_en_write(pstViProcReg, GK_TRUE);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_DitherRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				  GK_U8 i)
{
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);

	isp_acm_dither_u32_write(pstBeReg, GK_TRUE,
				 ISP_ACM_DITHER_ROUND_DEFAULT,
				 ISP_ACM_DITHER_SPATIAL_MODE_DEFAULT,
				 ISP_ACM_DITHER_OUT_BITS_DEFAULT);
	isp_dmnr_dither_u32_write(pstBeReg, GK_TRUE,
				  ISP_DMNR_DITHER_ROUND_DEFAULT,
				  ISP_DMNR_DITHER_SPATIAL_MODE_DEFAULT,
				  ISP_DMNR_DITHER_OUT_BITS_DEFAULT);

	isp_drc_dither_out_bits_write(pstBeReg,
				      ISP_DRC_DITHER_OUT_BITS_DEFAULT);
	isp_drc_dither_round_write(pstBeReg, ISP_DRC_DITHER_ROUND_DEFAULT);
	isp_drc_dither_spatial_mode_write(pstBeReg,
					  ISP_DRC_DITHER_SPATIAL_MODE_DEFAULT);
	isp_sqrt1_dither_en_write(pstBeReg, GK_FALSE);
	return GK_SUCCESS;
}

static GK_S32 ISP_FeSttRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U16 u16NumH, u16NumV;
	GK_U32 k;
	GK_BOOL bStt = GK_TRUE;
	VI_PIPE ViPipeBind;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	for (k = 0; k < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num; k++) {
		ViPipeBind = pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[k];
		ISP_CHECK_PIPE(ViPipeBind);
		pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
		ISP_CHECK_POINTER(pstFeReg);

		if (pstIspCtx->stSpecialOpt.bFeSttUpdate) {
			isp_ae1_stt_en_write(pstFeReg, bStt);
			isp_ae1_stt_bst_write(pstFeReg, 0xF);
		}

		/* ae */
		u16NumH = pstRegCfgInfo->stAlgRegCfg[0]
				  .stAeRegCfg.stDynaRegCfg.u8FEWeightTableWidth;
		u16NumV =
			pstRegCfgInfo->stAlgRegCfg[0]
				.stAeRegCfg.stDynaRegCfg.u8FEWeightTableHeight;
		isp_ae1_stt_size_write(pstFeReg, (u16NumH * u16NumV + 3) / 4);
	}

	pstIspCtx->stSpecialOpt.bFeSttUpdate = GK_FALSE;

	return GK_SUCCESS;
}

static GK_S32 ISP_OnlineSttRegConfig(VI_PIPE ViPipe,
				     ISP_REG_CFG_S *pstRegCfgInfo, GK_U8 i)
{
	GK_U16 u16NumH, u16NumV, u16BinNum;
	GK_BOOL bStt = GK_TRUE;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if ((IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) ||
	    (IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))) {
		return GK_SUCCESS;
	}

	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);

	if (pstIspCtx->stSpecialOpt.abBeOnSttUpdate[i]) {
		isp_ae_stt_en_write(pstBeReg, bStt);
		isp_ae_stt_bst_write(pstBeReg, 0xF);

		isp_la_stt_en_write(pstBeReg, bStt);
		isp_la_stt_bst_write(pstBeReg, 0xF);

		isp_awb_stt_en_write(pstBeReg, bStt);
		isp_awb_stt_bst_write(pstBeReg, 0xF);

		isp_af_stt_en_write(pstBeReg, bStt);
		isp_af_stt_bst_write(pstBeReg, 0xF);

		isp_dehaze_stt_en_write(pstBeReg, bStt);
		isp_dehaze_stt_bst_write(pstBeReg, 0xF);

		isp_ldci_lpfstt_en_write(pstBeReg, bStt);
		isp_ldci_lpfstt_bst_write(pstBeReg, 0xF);

		pstIspCtx->stSpecialOpt.abBeOnSttUpdate[i] = GK_FALSE;
	}

	/* AE */
	u16NumH = pstRegCfgInfo->stAlgRegCfg[i]
			  .stAeRegCfg.stDynaRegCfg.u8BEWeightTableWidth;
	u16NumV = pstRegCfgInfo->stAlgRegCfg[i]
			  .stAeRegCfg.stDynaRegCfg.u8BEWeightTableHeight;
	isp_ae_stt_size_write(pstBeReg, (u16NumH * u16NumV + 3) / 4);

	/* MG */
	u16NumH = pstRegCfgInfo->stAlgRegCfg[i]
			  .stMgRegCfg.stDynaRegCfg.u8ZoneWidth;
	u16NumV = pstRegCfgInfo->stAlgRegCfg[i]
			  .stMgRegCfg.stDynaRegCfg.u8ZoneHeight;
	isp_la_stt_size_write(pstBeReg, (u16NumH * u16NumV + 3) / 4);

	/* AWB */
	u16NumH = pstRegCfgInfo->stAlgRegCfg[i]
			  .stAwbRegCfg.stAwbRegUsrCfg.u16BEZoneCol;
	u16NumV = pstRegCfgInfo->stAlgRegCfg[i]
			  .stAwbRegCfg.stAwbRegUsrCfg.u16BEZoneRow;
	u16BinNum = 1;
	isp_awb_stt_size_write(pstBeReg,
			       (u16NumH * u16NumV * u16BinNum * 2 + 3) / 4);

	/* AF */
	u16NumH = pstRegCfgInfo->stAlgRegCfg[i].stBEAfRegCfg.u16WindowHnum;
	u16NumV = pstRegCfgInfo->stAlgRegCfg[i].stBEAfRegCfg.u16WindowVnum;
	isp_af_stt_size_write(pstBeReg, (u16NumH * u16NumV * 4 + 3) / 4);

	/* Dehaze */
	u16NumH = pstRegCfgInfo->stAlgRegCfg[i]
			  .stDehazeRegCfg.stStaticRegCfg.u8Dchnum;
	u16NumV = pstRegCfgInfo->stAlgRegCfg[i]
			  .stDehazeRegCfg.stStaticRegCfg.u8Dcvnum;
	isp_dehaze_stt_size_write(pstBeReg,
				  (((u16NumH + 1) * (u16NumV + 1)) + 3) / 4);

	/* Ldci */
	u16NumH = pstRegCfgInfo->stAlgRegCfg[i]
			  .stLdciRegCfg.stDynaRegCfg.u32StatSmlMapWidth;
	u16NumV = pstRegCfgInfo->stAlgRegCfg[i]
			  .stLdciRegCfg.stDynaRegCfg.u32StatSmlMapHeight;
	isp_ldci_lpfstt_size_write(pstBeReg, (u16NumH * u16NumV + 3) / 4);

	return GK_SUCCESS;
}

static GK_S32 ISP_BeAlgLut2SttRegnewRegConfig(VI_PIPE ViPipe,
					      ISP_REG_CFG_S *pstRegCfgInfo,
					      GK_U8 i)
{
	ISP_BE_STT2LUT_REGNEW_REG_CFG_S *pstBeStt2LutRegnewCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].stStt2LutRegnewCfg;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);

	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bSharpenStt2LutRegnew) {
		isp_sharpen_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}
	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bLdciStt2LutRegnew) {
		isp_ldci_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}
	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bLscStt2LutRegnew) {
		isp_mlsc_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}

	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bGammaStt2LutRegnew) {
		isp_gamma_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}

	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bCaStt2LutRegnew) {
		isp_ca_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}

	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bDehazeStt2LutRegnew) {
		isp_dehaze_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}

	if ((pstIspCtx->u32FrameCnt < 3) ||
	    pstBeStt2LutRegnewCfg->bBnrStt2LutRegnew) {
		isp_bnr_stt2lut_regnew_write(pstBeReg, GK_TRUE);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_BeAlgLutUpdateRegConfig(VI_PIPE ViPipe,
					  ISP_REG_CFG_S *pstRegCfgInfo, GK_U8 i)
{
	ISP_BE_LUT_UPDATE_REG_CFG_S *pstBeLutUpdateCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].stBeLutUpdateCfg;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);

	if (pstBeLutUpdateCfg->bAeLutUpdate) {
		isp_ae_lut_update_write(pstBeReg,
					pstBeLutUpdateCfg->bAeLutUpdate);
	}
	if (pstBeLutUpdateCfg->bDrcTmLutUpdate) {
		isp_drc_lut_update0_write(pstBeReg,
					  pstBeLutUpdateCfg->bDrcTmLutUpdate);
	}
	if (pstBeLutUpdateCfg->bLCacLutUpdate) {
		isp_demosaic_depurplut_update_write(
			pstBeReg, pstBeLutUpdateCfg->bLCacLutUpdate);
	}

	if (pstBeLutUpdateCfg->bNddmGfLutUpdate) {
		isp_nddm_gf_lut_update_write(
			pstBeReg, pstBeLutUpdateCfg->bNddmGfLutUpdate);
	}
	if (pstBeLutUpdateCfg->bPreGammaLutUpdate) {
		isp_drc_lut_update1_write(
			pstBeReg, pstBeLutUpdateCfg->bPreGammaLutUpdate);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_BeCurRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				 GK_U8 i)
{
	ISP_ALG_REG_CFG_S *pstAlgRegCfg = &pstRegCfgInfo->stAlgRegCfg[i];
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;

	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);
	ISP_CHECK_POINTER(pstViProcReg);
	isp_ae_en_write(pstViProcReg,
			pstAlgRegCfg->stAeRegCfg.stStaticRegCfg.u8BEEnable);
	isp_la_en_write(pstViProcReg,
			pstAlgRegCfg->stMgRegCfg.stStaticRegCfg.u8Enable);
	isp_wb_en_write(pstViProcReg,
			pstAlgRegCfg->stAwbRegCfg.stAwbRegDynCfg.u8BEWbWorkEn);
	isp_cc_en_write(pstViProcReg,
			pstAlgRegCfg->stAwbRegCfg.stAwbRegDynCfg.u8BECcEn);
	isp_cc_colortone_en_write(
		pstBeReg,
		pstAlgRegCfg->stAwbRegCfg.stAwbRegDynCfg.u16BECcColortoneEn);
	isp_awb_en_write(
		pstViProcReg,
		pstAlgRegCfg->stAwbRegCfg.stAwbRegStaCfg.u8BEAwbWorkEn);
	isp_af_en_write(pstViProcReg, pstAlgRegCfg->stBEAfRegCfg.bAfEnable);
	isp_sharpen_en_write(pstViProcReg,
			     pstAlgRegCfg->stSharpenRegCfg.bEnable);

	isp_dmnr_vhdm_en_write(pstViProcReg,
			       pstAlgRegCfg->stDemRegCfg.bVhdmEnable);
	isp_dmnr_nddm_en_write(pstViProcReg,
			       pstAlgRegCfg->stDemRegCfg.bNddmEnable);
	isp_demosaic_local_cac_en_write(pstBeReg,
					pstAlgRegCfg->stLCacRegCfg.bLocalCacEn);

	isp_demosaic_fcr_en_write(
		pstBeReg, pstAlgRegCfg->stAntiFalseColorRegCfg.bFcrEnable);
	isp_dpc_en_write(pstViProcReg, pstAlgRegCfg->stDpRegCfg.abDpcEn[0]);
	isp_ge_en_write(pstViProcReg, pstAlgRegCfg->stGeRegCfg.abGeEn);
	isp_lsc_en_write(pstViProcReg, pstAlgRegCfg->stLscRegCfg.bLscEn);
	isp_gamma_en_write(pstViProcReg, pstAlgRegCfg->stGammaCfg.bGammaEn);
	isp_csc_en_write(pstViProcReg, pstAlgRegCfg->stCscCfg.bEnable);
	isp_ca_en_write(pstViProcReg, pstAlgRegCfg->stCaRegCfg.bCaEn);

	isp_ca_llhc_ratio_u32_write(
		pstBeReg,
		pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.s16CaLumaRatioHigh,
		pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.s16CaLumaRatioLow);
	isp_ca_yraratio_u32_write(
		pstBeReg, pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.s16CaYLumaRatio);
	isp_ca_isoratio_write(
		pstBeReg, pstAlgRegCfg->stCaRegCfg.stDynaRegCfg.u16CaISORatio);
	isp_dehaze_en_write(pstViProcReg,
			    pstAlgRegCfg->stDehazeRegCfg.bDehazeEn);
	isp_bnr_en_write(pstViProcReg, pstAlgRegCfg->stBnrRegCfg.bBnrEnable);
	if (pstAlgRegCfg->stLscRegCfg.bLscEn) {
		isp_bnr_lscbnrenable_write(
			pstBeReg,
			pstAlgRegCfg->stBnrRegCfg.stUsrRegCfg.bBnrLscEn);
	} else {
		isp_bnr_lscbnrenable_write(pstBeReg, 0);
	}

	isp_rgbir_en_write(pstViProcReg, pstAlgRegCfg->stRgbirCfg.bEnable);

	isp_pregamma_en_write(pstViProcReg,
			      pstAlgRegCfg->stPreGammaCfg.bPreGammaEn);

	isp_expander_en_write(pstViProcReg,
			      pstAlgRegCfg->stExpanderCfg.bEnable);

	isp_bnr_jnlmgain_s3_u32_write(pstBeReg,
				      pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg
					      .au32JnlmLimitMultGainS[3]);
	isp_bnr_jnlmgain_s2_u32_write(pstBeReg,
				      pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg
					      .au32JnlmLimitMultGainS[2]);
	isp_bnr_jnlmgain_s1_u32_write(pstBeReg,
				      pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg
					      .au32JnlmLimitMultGainS[1]);
	isp_bnr_jnlmgain_s0_u32_write(pstBeReg,
				      pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg
					      .au32JnlmLimitMultGainS[0]);
	isp_bnr_jnlmgain3_u32_write(
		pstBeReg,
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[3]);
	isp_bnr_jnlmgain2_u32_write(
		pstBeReg,
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[2]);
	isp_bnr_jnlmgain1_u32_write(
		pstBeReg,
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[1]);
	isp_bnr_jnlmgain0_u32_write(
		pstBeReg,
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[0]);

	isp_bnr_jnlm_gain_write(
		pstBeReg, pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.u8JnlmGain);

	isp_ldci_blc_ctrl_write(
		pstBeReg,
		pstAlgRegCfg->stLdciRegCfg.stDynaRegCfg.u32CalcBlcCtrl);

	ISP_SharpenDynaRegConfig(
		pstBeReg,
		&pstAlgRegCfg->stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg,
		&pstAlgRegCfg->stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg);

	if (pstAlgRegCfg->stLscRegCfg.stUsrRegCfg.bLutUpdate) {
		isp_lsc_mesh_scale_write(
			pstBeReg,
			pstAlgRegCfg->stLscRegCfg.stUsrRegCfg.u8MeshScale);
		isp_mlsc_mesh_scale_write(
			pstBeReg,
			pstAlgRegCfg->stLscRegCfg.stUsrRegCfg.u8MeshScale);
	}

	isp_dehaze_air_u32_write(
		pstBeReg, pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u16AirB,
		pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u16AirG,
		pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u16AirR);

	isp_dehaze_gstrth_u32_write(
		pstBeReg, pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u8Strength);
	return GK_SUCCESS;
}

static GK_S32 ISP_BeLastRegConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				  GK_U8 i)
{
	ISP_LUT2STT_SYNC_REG_CFG_S *pstLut2SttSyncCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].astLut2SttSyncCfg[0];
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_GET_CTX(ViPipe, pstIspCtx);

	pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
	pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
	ISP_CHECK_POINTER(pstBeReg);
	ISP_CHECK_POINTER(pstViProcReg);
	isp_ae_en_write(pstViProcReg, pstLut2SttSyncCfg->bAeEn);
	isp_la_en_write(pstViProcReg, pstLut2SttSyncCfg->bLaEn);
	isp_wb_en_write(pstViProcReg, pstLut2SttSyncCfg->bWbEn);
	isp_cc_en_write(pstViProcReg, pstLut2SttSyncCfg->bCcmEn);
	isp_cc_colortone_en_write(pstBeReg, pstLut2SttSyncCfg->bCcmColorToneEn);
	isp_awb_en_write(pstViProcReg, pstLut2SttSyncCfg->bAwbEn);
	isp_af_en_write(pstViProcReg, pstLut2SttSyncCfg->bAfEn);
	isp_sharpen_en_write(pstViProcReg, pstLut2SttSyncCfg->bSharpenEn);
	isp_dmnr_vhdm_en_write(pstViProcReg, pstLut2SttSyncCfg->bVhdmEn);
	isp_dmnr_nddm_en_write(pstViProcReg, pstLut2SttSyncCfg->bNddmEn);
	isp_demosaic_local_cac_en_write(pstBeReg, pstLut2SttSyncCfg->bLcacEn);

	isp_demosaic_fcr_en_write(pstBeReg, pstLut2SttSyncCfg->bFcrEn);
	isp_dpc_en_write(pstViProcReg, pstLut2SttSyncCfg->abDpcEn[0]);
	isp_ge_en_write(pstViProcReg, pstLut2SttSyncCfg->abGeEn);
	if (pstIspCtx->u32FrameCnt == 0) {
		isp_lsc_en_write(pstViProcReg, 0);
	} else {
		isp_lsc_en_write(pstViProcReg, pstLut2SttSyncCfg->bLscEn);
	}

	isp_gamma_en_write(pstViProcReg, pstLut2SttSyncCfg->bGammaEn);

	isp_csc_en_write(pstViProcReg, pstLut2SttSyncCfg->bCscEn);
	isp_ca_en_write(pstViProcReg, pstLut2SttSyncCfg->bCaEn);

	isp_ca_llhc_ratio_u32_write(
		pstBeReg, pstLut2SttSyncCfg->stCaSyncCfg.s16CaLumaRatioHigh,
		pstLut2SttSyncCfg->stCaSyncCfg.s16CaLumaRatioLow);
	isp_ca_yraratio_u32_write(
		pstBeReg, pstLut2SttSyncCfg->stCaSyncCfg.s16CaYLumaRatio);
	isp_ca_isoratio_write(pstBeReg,
			      pstLut2SttSyncCfg->stCaSyncCfg.u16CaIsoRatio);
	isp_dehaze_en_write(pstViProcReg, pstLut2SttSyncCfg->bDehazeEn);
	isp_bnr_en_write(pstViProcReg, pstLut2SttSyncCfg->bBnrEn);

	if (pstLut2SttSyncCfg->bLscEn) {
		isp_bnr_lscbnrenable_write(pstBeReg,
					   pstLut2SttSyncCfg->bBnrLscEn);
	} else {
		isp_bnr_lscbnrenable_write(pstBeReg, 0);
	}
	isp_rgbir_en_write(pstViProcReg, pstLut2SttSyncCfg->bRgbirEn);

	isp_pregamma_en_write(pstViProcReg, pstLut2SttSyncCfg->bPregammaEn);

	isp_expander_en_write(pstViProcReg, pstLut2SttSyncCfg->bExpanderEn);
	isp_bnr_jnlmgain_s3_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[3]);
	isp_bnr_jnlmgain_s2_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[2]);
	isp_bnr_jnlmgain_s1_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[1]);
	isp_bnr_jnlmgain_s0_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[0]);
	isp_bnr_jnlmgain3_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[3]);
	isp_bnr_jnlmgain2_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[2]);
	isp_bnr_jnlmgain1_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[1]);
	isp_bnr_jnlmgain0_u32_write(
		pstBeReg,
		pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[0]);

	isp_bnr_jnlm_gain_write(pstBeReg,
				pstLut2SttSyncCfg->stBnrSyncCfg.u8JnlmGain);
	isp_ldci_blc_ctrl_write(
		pstBeReg, pstLut2SttSyncCfg->stLdciSyncCfg.u32CalcBlcCtrl);

	if (pstLut2SttSyncCfg->stLscSyncCfg.bResh) {
		isp_lsc_mesh_scale_write(
			pstBeReg, pstLut2SttSyncCfg->stLscSyncCfg.u8MeshScale);
		isp_mlsc_mesh_scale_write(
			pstBeReg, pstLut2SttSyncCfg->stLscSyncCfg.u8MeshScale);
	}

	if (pstRegCfgInfo->stAlgRegCfg[i].stDehazeRegCfg.bLut2SttEn ==
	    GK_TRUE) {
		isp_dehaze_air_u32_write(
			pstBeReg, pstLut2SttSyncCfg->stDehazeSyncCfg.u16AirB,
			pstLut2SttSyncCfg->stDehazeSyncCfg.u16AirG,
			pstLut2SttSyncCfg->stDehazeSyncCfg.u16AirR);
		isp_dehaze_gstrth_u32_write(
			pstBeReg,
			pstLut2SttSyncCfg->stDehazeSyncCfg.u8Strength);
	}

	ISP_SharpenDynaRegConfig(
		pstBeReg, &pstLut2SttSyncCfg->stSharpenSyncCfg.stMpiDynaRegCfg,
		&pstLut2SttSyncCfg->stSharpenSyncCfg.stDefaultDynaRegCfg);

	return GK_SUCCESS;
}

static GK_S32 ISP_BeAlgSyncRegConfig(VI_PIPE ViPipe,
				     ISP_REG_CFG_S *pstRegCfgInfo, GK_U8 i)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		ISP_BeCurRegConfig(ViPipe, pstRegCfgInfo, i);
	} else {
		ISP_BeLastRegConfig(ViPipe, pstRegCfgInfo, i);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_SaveBeSyncReg(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo,
				GK_U8 i)
{
	GK_U8 j;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LUT2STT_SYNC_REG_CFG_S *pstLut2SttSyncCfg =
		&pstRegCfgInfo->stAlgRegCfg[i].astLut2SttSyncCfg[0];
	ISP_ALG_REG_CFG_S *pstAlgRegCfg = &pstRegCfgInfo->stAlgRegCfg[i];

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_SUCCESS;
	}

	pstLut2SttSyncCfg->bAeEn =
		pstAlgRegCfg->stAeRegCfg.stStaticRegCfg.u8BEEnable;
	pstLut2SttSyncCfg->bLaEn =
		pstAlgRegCfg->stMgRegCfg.stStaticRegCfg.u8Enable;
	pstLut2SttSyncCfg->bAwbEn =
		pstAlgRegCfg->stAwbRegCfg.stAwbRegStaCfg.u8BEAwbWorkEn;
	pstLut2SttSyncCfg->bWbEn =
		pstAlgRegCfg->stAwbRegCfg.stAwbRegDynCfg.u8BEWbWorkEn;
	pstLut2SttSyncCfg->bCcmEn =
		pstAlgRegCfg->stAwbRegCfg.stAwbRegDynCfg.u8BECcEn;
	pstLut2SttSyncCfg->bCcmColorToneEn =
		pstAlgRegCfg->stAwbRegCfg.stAwbRegDynCfg.u16BECcColortoneEn;
	pstLut2SttSyncCfg->bAfEn = pstAlgRegCfg->stBEAfRegCfg.bAfEnable;
	pstLut2SttSyncCfg->bSharpenEn = pstAlgRegCfg->stSharpenRegCfg.bEnable;

	pstLut2SttSyncCfg->bVhdmEn = pstAlgRegCfg->stDemRegCfg.bVhdmEnable;
	pstLut2SttSyncCfg->bNddmEn = pstAlgRegCfg->stDemRegCfg.bNddmEnable;
	pstLut2SttSyncCfg->bLdciEn =
		pstAlgRegCfg->stLdciRegCfg.stDynaRegCfg.bEnable;
	pstLut2SttSyncCfg->bLcacEn = pstAlgRegCfg->stLCacRegCfg.bLocalCacEn;

	pstLut2SttSyncCfg->bFcrEn =
		pstAlgRegCfg->stAntiFalseColorRegCfg.bFcrEnable;
	pstLut2SttSyncCfg->bLscEn = pstAlgRegCfg->stLscRegCfg.bLscEn;
	pstLut2SttSyncCfg->bGammaEn = pstAlgRegCfg->stGammaCfg.bGammaEn;
	pstLut2SttSyncCfg->bCscEn = pstAlgRegCfg->stCscCfg.bEnable;
	pstLut2SttSyncCfg->bCaEn = pstAlgRegCfg->stCaRegCfg.bCaEn;

	pstLut2SttSyncCfg->stCaSyncCfg.s16CaLumaRatioHigh =
		pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.s16CaLumaRatioHigh;
	pstLut2SttSyncCfg->stCaSyncCfg.s16CaLumaRatioLow =
		pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.s16CaLumaRatioLow;
	pstLut2SttSyncCfg->stCaSyncCfg.s16CaYLumaRatio =
		pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.s16CaYLumaRatio;
	pstLut2SttSyncCfg->stCaSyncCfg.u16CaIsoRatio =
		pstAlgRegCfg->stCaRegCfg.stDynaRegCfg.u16CaISORatio;

	pstLut2SttSyncCfg->bWdrEn = pstAlgRegCfg->stWdrRegCfg.bWDREn;
	pstLut2SttSyncCfg->bDrcEn = pstAlgRegCfg->stDrcRegCfg.bDrcEn;
	pstLut2SttSyncCfg->bDehazeEn = pstAlgRegCfg->stDehazeRegCfg.bDehazeEn;
	pstLut2SttSyncCfg->bBnrEn = pstAlgRegCfg->stBnrRegCfg.bBnrEnable;
	pstLut2SttSyncCfg->bBnrLscEn =
		pstAlgRegCfg->stBnrRegCfg.stUsrRegCfg.bBnrLscEn;
	pstLut2SttSyncCfg->bRgbirEn = pstAlgRegCfg->stRgbirCfg.bEnable;
	pstLut2SttSyncCfg->bDgEn = pstAlgRegCfg->stDgRegCfg.bDgEn;
	pstLut2SttSyncCfg->b4DgEn = pstAlgRegCfg->st4DgRegCfg.bEnable;
	pstLut2SttSyncCfg->bPregammaEn =
		pstAlgRegCfg->stPreGammaCfg.bPreGammaEn;
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[0] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[0];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[1] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[1];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[2] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[2];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGain[3] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGain[3];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[0] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGainS[0];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[1] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGainS[1];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[2] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGainS[2];
	pstLut2SttSyncCfg->stBnrSyncCfg.au32JnlmLimitMultGainS[3] =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.au32JnlmLimitMultGainS[3];
	pstLut2SttSyncCfg->stBnrSyncCfg.u8JnlmGain =
		pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.u8JnlmGain;
	pstLut2SttSyncCfg->bExpanderEn = pstAlgRegCfg->stExpanderCfg.bEnable;

	for (j = 0; j < 4; j++) {
		pstLut2SttSyncCfg->abDpcEn[j] =
			pstAlgRegCfg->stDpRegCfg.abDpcEn[j];
		pstLut2SttSyncCfg->abGeEn = pstAlgRegCfg->stGeRegCfg.abGeEn;
	}

	pstLut2SttSyncCfg->stLscSyncCfg.bResh =
		pstAlgRegCfg->stBeLutUpdateCfg.bLscLutUpdate;
	pstLut2SttSyncCfg->stLscSyncCfg.u8MeshScale =
		pstAlgRegCfg->stLscRegCfg.stUsrRegCfg.u8MeshScale;

	pstLut2SttSyncCfg->stDehazeSyncCfg.u16AirR =
		pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u16AirR;
	pstLut2SttSyncCfg->stDehazeSyncCfg.u16AirG =
		pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u16AirG;
	pstLut2SttSyncCfg->stDehazeSyncCfg.u16AirB =
		pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u16AirB;
	pstLut2SttSyncCfg->stDehazeSyncCfg.u8Strength =
		pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u8Strength;

	pstLut2SttSyncCfg->stLdciSyncCfg.u32CalcBlcCtrl =
		pstAlgRegCfg->stLdciRegCfg.stDynaRegCfg.u32CalcBlcCtrl;

	memcpy(&(pstLut2SttSyncCfg->stSharpenSyncCfg.stMpiDynaRegCfg),
	       &(pstAlgRegCfg->stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg),
	       sizeof(ISP_SHARPEN_MPI_DYNA_REG_CFG_S));
	memcpy(&(pstLut2SttSyncCfg->stSharpenSyncCfg.stDefaultDynaRegCfg),
	       &(pstAlgRegCfg->stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg),
	       sizeof(ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S));

	return GK_SUCCESS;
}

GK_S32 ISP_BeReshCfg(ISP_ALG_REG_CFG_S *pstAlgRegCfg)
{
	pstAlgRegCfg->stAwbRegCfg.stAwbRegStaCfg.bBEAwbStaCfg = GK_TRUE;
	pstAlgRegCfg->stAwbRegCfg.stAwbRegUsrCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stDemRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stDemRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stLdciRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;

	pstAlgRegCfg->stLCacRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stLCacRegCfg.stUsrRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stLCacRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stAntiFalseColorRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stAntiFalseColorRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stDpRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stDpRegCfg.stUsrRegCfg.stUsrDynaCorRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stDpRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stGeRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stGeRegCfg.stUsrRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stGeRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stLscRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stLscRegCfg.stUsrRegCfg.bCoefUpdate = GK_TRUE;
	pstAlgRegCfg->stLscRegCfg.stUsrRegCfg.bLutUpdate = GK_TRUE;

	pstAlgRegCfg->stGammaCfg.stUsrRegCfg.bGammaLutUpdateEn = GK_TRUE;
	pstAlgRegCfg->stCscCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stCaRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stCaRegCfg.stDynaRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stCaRegCfg.stUsrRegCfg.bCaLutUpdateEn = GK_TRUE;

	pstAlgRegCfg->stMcdsRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stMcdsRegCfg.stDynaRegCfg.bDynaResh = GK_TRUE;

	pstAlgRegCfg->stWdrRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stWdrRegCfg.stUsrRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stWdrRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stDrcRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stDrcRegCfg.stUsrRegCfg.bUsrResh = GK_TRUE;
	pstAlgRegCfg->stDrcRegCfg.stDynaRegCfg.bDynaResh = GK_TRUE;

	pstAlgRegCfg->stDehazeRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stDehazeRegCfg.stDynaRegCfg.u32LutUpdate = 1;

	pstAlgRegCfg->stBnrRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stBnrRegCfg.stDynaRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stBnrRegCfg.stUsrRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->st4DgRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->st4DgRegCfg.stDynaRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stDgRegCfg.stStaticRegCfg.bResh = GK_TRUE;
	pstAlgRegCfg->stDgRegCfg.stDynaRegCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stPreGammaCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stPreGammaCfg.stDynaRegCfg.bPreGammaLutUpdateEn = GK_TRUE;

	pstAlgRegCfg->stBeBlcCfg.static_blc.bReshStatic = GK_TRUE;
	pstAlgRegCfg->stBeBlcCfg.resh_dyna_init = GK_TRUE;

	pstAlgRegCfg->stExpanderCfg.stStaticCfg.bResh = GK_TRUE;

	pstAlgRegCfg->stSharpenRegCfg.stStaticRegCfg.bStaticResh = GK_TRUE;
	pstAlgRegCfg->stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg.bResh =
		GK_TRUE;
	pstAlgRegCfg->stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bResh =
		GK_TRUE;

	return GK_SUCCESS;
}

GK_S32 ISP_ResetFeSttEn(VI_PIPE ViPipe)
{
	GK_U8 i;
	VI_PIPE ViPipeBind;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->stWdrAttr.bMastPipe) {
		for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num;
		     i++) {
			ViPipeBind =
				pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
			ISP_CHECK_PIPE(ViPipeBind);

			pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(
				ViPipeBind);
			ISP_CHECK_POINTER(pstFeReg);

			isp_ae1_stt_en_write(pstFeReg, GK_FALSE);
			isp_fe_update_write(pstFeReg, GK_TRUE);
		}
	}

	return GK_SUCCESS;
}

GK_S32 ISP_AlgEnExit(VI_PIPE ViPipe)
{
	GK_U8 i;
	VI_PIPE ViPipeBind;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	S_ISPFE_REGS_TYPE *pstFeReg = GK_NULL;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_CHECK_POINTER(pstIspCtx);

	if (pstIspCtx->stIspParaRec.bInit == GK_FALSE) {
		return GK_SUCCESS;
	}

	/* FE */
	if (pstIspCtx->stWdrAttr.bMastPipe) {
		for (i = 0; i < pstIspCtx->stWdrAttr.stDevBindPipe.u32Num;
		     i++) {
			ViPipeBind =
				pstIspCtx->stWdrAttr.stDevBindPipe.PipeId[i];
			ISP_CHECK_PIPE(ViPipeBind);

			pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(
				ViPipeBind);
			ISP_CHECK_POINTER(pstFeReg);

			isp_fe_ae1_en_write(pstFeReg, GK_FALSE);
			isp_fe_wb1_en_write(pstFeReg, GK_FALSE);
			isp_fe_dg2_en_write(pstFeReg, GK_FALSE);
			isp_ae1_stt_en_write(pstFeReg, GK_FALSE);
			isp_fe_update_write(pstFeReg, GK_TRUE);
		}
	}

	if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		for (i = 0; i < pstIspCtx->stBlockAttr.u8BlockNum; i++) {
			pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe,
									 i);
			pstViProcReg =
				(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(
					ViPipe, i);

			ISP_CHECK_POINTER(pstBeReg);
			ISP_CHECK_POINTER(pstViProcReg);
			// online stt out
			isp_ae_en_write(pstViProcReg, GK_FALSE);
			isp_la_en_write(pstViProcReg, GK_FALSE);
			isp_awb_en_write(pstViProcReg, GK_FALSE);
			isp_af_en_write(pstViProcReg, GK_FALSE);
			isp_dehaze_en_write(pstViProcReg, GK_FALSE);
			isp_ldci_en_write(pstViProcReg, GK_FALSE);
			isp_ldci_wrstat_en_write(pstBeReg, GK_FALSE);
			isp_ldci_rdstat_en_write(pstBeReg, GK_FALSE);
			isp_drc_en_write(pstViProcReg, GK_FALSE);
			// online lut to stt
			isp_dehaze_stt2lut_en_write(pstBeReg, GK_FALSE);
			isp_gamma_stt2lut_en_write(pstBeReg, GK_FALSE);
			isp_sharpen_stt2lut_en_write(pstBeReg, GK_FALSE);
			isp_mlsc_stt2lut_en_write(pstBeReg, GK_FALSE);
			isp_bnr_stt2lut_en_write(pstBeReg, GK_FALSE);
			isp_ca_stt2lut_en_write(pstBeReg, GK_FALSE);
			isp_ldci_stt2lut_en_write(pstBeReg, GK_FALSE);

			isp_be_reg_up_write(pstViProcReg, GK_TRUE);
		}
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_FeRegsConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_CHECK_VIR_PIPE(ViPipe);
	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->stWdrAttr.bMastPipe) {
		/* FE alg cfgs setting to register */
		ISP_FeAeRegConfig(ViPipe, pstRegCfgInfo); /* Ae */
		ISP_FeAwbRegConfig(ViPipe, pstRegCfgInfo); /* awb */
		ISP_FeDgRegConfig(ViPipe, pstRegCfgInfo); /* DG */
		ISP_FeRcRegConfig(ViPipe, pstRegCfgInfo); /* Rc */
		ISP_FeBlcRegConfig(ViPipe, pstRegCfgInfo);

		ISP_FeSystemRegConfig(ViPipe, pstRegCfgInfo);

		ISP_FeSttRegConfig(ViPipe, pstRegCfgInfo);
		ISP_FeUpdateRegConfig(ViPipe, pstRegCfgInfo);
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_BeSyncParamInit(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U8 j;
	GK_S32 s32Ret;
	ISP_BE_SYNC_PARA_S stBeSyncParam = { 0 };
	ISP_ALG_REG_CFG_S *pstAlgRegCfg = &pstRegCfgInfo->stAlgRegCfg[0];
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		return GK_SUCCESS;
	}

	/* DG */
	stBeSyncParam.au32IspDgain[0] =
		pstAlgRegCfg->stDgRegCfg.stDynaRegCfg.u16GainR;
	stBeSyncParam.au32IspDgain[1] =
		pstAlgRegCfg->stDgRegCfg.stDynaRegCfg.u16GainGR;
	stBeSyncParam.au32IspDgain[2] =
		pstAlgRegCfg->stDgRegCfg.stDynaRegCfg.u16GainGB;
	stBeSyncParam.au32IspDgain[3] =
		pstAlgRegCfg->stDgRegCfg.stDynaRegCfg.u16GainB;
	/* LDCI */
	stBeSyncParam.u32LdciComp = 0x1000;

	/* DRC */
	stBeSyncParam.u8DrcShpLog =
		pstAlgRegCfg->stDrcRegCfg.stStaticRegCfg.u8ShpLog;
	stBeSyncParam.u32DrcDivDenomLog =
		pstAlgRegCfg->stDrcRegCfg.stStaticRegCfg.u32DivDenomLog;
	stBeSyncParam.u32DrcDenomExp =
		pstAlgRegCfg->stDrcRegCfg.stStaticRegCfg.u32DenomExp;

	for (j = 0; j < SYNC_DRC_PRELUMA_NUM; j++) {
		stBeSyncParam.au32DrcPrevLuma[j] =
			pstAlgRegCfg->stDrcRegCfg.stStaticRegCfg.au32PrevLuma[j];
	}

	/* WDR */
	stBeSyncParam.u32WdrExpRatio =
		pstAlgRegCfg->stWdrRegCfg.stStaticRegCfg.au16ExpoRRatio[0];
	stBeSyncParam.u32FlickExpRatio =
		pstAlgRegCfg->stWdrRegCfg.stStaticRegCfg.flick_exp_ratio;

	for (j = 0; j < SYNC_WDR_EXP_VAL_NUM; j++) {
		stBeSyncParam.au32WdrExpVal[j] =
			pstAlgRegCfg->stWdrRegCfg.stStaticRegCfg
				.au16ExpoValue[j];
		stBeSyncParam.au32WDRGain[j] = 0x100;
	}

	stBeSyncParam.u32WdrBlcComp =
		pstAlgRegCfg->stWdrRegCfg.stStaticRegCfg.au32BlcComp[0];
	stBeSyncParam.bWDRMdtEn =
		pstAlgRegCfg->stWdrRegCfg.stDynaRegCfg.bWDRMdtEn;
	stBeSyncParam.bFusionMode =
		pstAlgRegCfg->stWdrRegCfg.stUsrRegCfg.bFusionMode;
	stBeSyncParam.u16ShortThr =
		pstAlgRegCfg->stWdrRegCfg.stDynaRegCfg.u16ShortThr;
	stBeSyncParam.u16LongThr =
		pstAlgRegCfg->stWdrRegCfg.stDynaRegCfg.u16LongThr;
	stBeSyncParam.u32WdrMaxRatio =
		pstAlgRegCfg->stWdrRegCfg.stStaticRegCfg.u32MaxRatio;
	stBeSyncParam.u16WgtSlope =
		pstAlgRegCfg->stWdrRegCfg.stDynaRegCfg.u16WgtSlope;

	/* blc */
	memcpy_s(&stBeSyncParam.be_blc, sizeof(isp_be_blc_dyna_cfg),
		 &pstAlgRegCfg->stBeBlcCfg.dyna_blc,
		 sizeof(isp_be_blc_dyna_cfg));

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_SYNC_PARAM_INIT,
		       &stBeSyncParam);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] Init BE Sync Param Failed with ec %#x!\n",
			  ViPipe, s32Ret);
		return s32Ret;
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_BeRegsConfig(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U32 i;
	GK_S32 s32Ret = 0;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	for (i = 0; i < pstRegCfgInfo->u8CfgNum; i++) {
		ISP_SystemRegConfig(ViPipe, pstRegCfgInfo, i); /* sys */
		ISP_DitherRegConfig(ViPipe, pstRegCfgInfo, i); /* dither */
		ISP_OnlineSttRegConfig(ViPipe, pstRegCfgInfo, i);
		/* Be alg cfgs setting to register */
		ISP_AeRegConfig(ViPipe, pstRegCfgInfo, i); /* ae */
		ISP_AwbRegConfig(ViPipe, pstRegCfgInfo, i); /* awb */
		ISP_AfRegConfig(ViPipe, pstRegCfgInfo, i); /* AF */
		ISP_SharpenRegConfig(ViPipe, pstRegCfgInfo, i); /* sharpen */
		ISP_DemRegConfig(ViPipe, pstRegCfgInfo, i); /* demosaic */
		ISP_FpnRegConfig(ViPipe, pstRegCfgInfo, i); /* FPN */
		ISP_LdciRegConfig(ViPipe, pstRegCfgInfo, i); /* ldci */
		ISP_LcacRegConfig(ViPipe, pstRegCfgInfo, i); /* Local cac */
		ISP_FcrRegConfig(ViPipe, pstRegCfgInfo, i); /* FCR */
		ISP_DpcRegConfig(ViPipe, pstRegCfgInfo, i); /* dpc */
		ISP_GeRegConfig(ViPipe, pstRegCfgInfo, i); /* ge */
		ISP_LscRegConfig(ViPipe, pstRegCfgInfo, i); /* BE LSC */
		ISP_GammaRegConfig(ViPipe, pstRegCfgInfo, i); /* gamma */
		ISP_CscRegConfig(ViPipe, pstRegCfgInfo, i); /* csc */
		ISP_CaRegConfig(ViPipe, pstRegCfgInfo, i); /* ca */
		ISP_McdsRegConfig(ViPipe, pstRegCfgInfo, i); /* mcds */
		ISP_WdrRegConfig(ViPipe, pstRegCfgInfo, i); /* wdr */
		ISP_DrcRegConfig(ViPipe, pstRegCfgInfo, i); /* drc */
		ISP_DehazeRegConfig(ViPipe, pstRegCfgInfo, i); /* Dehaze */
		ISP_BayerNrRegConfig(ViPipe, pstRegCfgInfo, i); /* BayerNR */
		ISP_DgRegConfig(ViPipe, pstRegCfgInfo, i); /* DG */
		ISP_4DgRegConfig(ViPipe, pstRegCfgInfo, i); /* 4DG */
		ISP_PreGammaRegConfig(ViPipe, pstRegCfgInfo, i); /* PreGamma */
		ISP_BeBlcRegConfig(ViPipe, pstRegCfgInfo, i);
		ISP_ExpanderRegConfig(ViPipe, pstRegCfgInfo, i); /* expander */
		ISP_RgbirRegConfig(ViPipe, pstRegCfgInfo, i); /* Rgbir */
	}

	for (i = 0; i < pstRegCfgInfo->u8CfgNum; i++) {
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstViProcReg);
		ISP_BeAlgSyncRegConfig(ViPipe, pstRegCfgInfo, i);
		ISP_BeAlgLut2SttRegnewRegConfig(ViPipe, pstRegCfgInfo, i);
		isp_be_reg_up_write(pstViProcReg, GK_TRUE);
		ISP_BeAlgLutUpdateRegConfig(ViPipe, pstRegCfgInfo, i);
	}

	for (i = 0; i < pstRegCfgInfo->u8CfgNum; i++) {
		ISP_SaveBeSyncReg(ViPipe, pstRegCfgInfo, i);
	}

	if ((IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) ||
	    (IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode))) {
		s32Ret = ISP_CfgBeBufCtl(ViPipe);

		if (s32Ret != GK_SUCCESS) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Pipe:%d Be config bufs ctl failed %x!\n",
				  ViPipe, s32Ret);
			return s32Ret;
		}
	}

	return GK_SUCCESS;
}

static GK_S32 ISP_BeRegsConfigInit(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfgInfo)
{
	GK_U32 i;
	S_VIPROC_REGS_TYPE *pstViProcReg = GK_NULL;

	for (i = 0; i < pstRegCfgInfo->u8CfgNum; i++) {
		ISP_SaveBeSyncReg(ViPipe, pstRegCfgInfo, i);
	}

	for (i = 0; i < pstRegCfgInfo->u8CfgNum; i++) {
		ISP_RegDefault(ViPipe, pstRegCfgInfo, i);
		ISP_SystemRegConfig(ViPipe, pstRegCfgInfo, i); /* sys */
		ISP_DitherRegConfig(ViPipe, pstRegCfgInfo, i); /* dither */
		ISP_OnlineSttRegConfig(ViPipe, pstRegCfgInfo, i);
		/* Be alg cfgs setting to register */
		ISP_AeRegConfig(ViPipe, pstRegCfgInfo, i); /* ae */
		ISP_AwbRegConfig(ViPipe, pstRegCfgInfo, i); /* awb */
		ISP_AfRegConfig(ViPipe, pstRegCfgInfo, i); /* AF */
		ISP_SharpenRegConfig(ViPipe, pstRegCfgInfo, i); /* sharpen */
		ISP_DemRegConfig(ViPipe, pstRegCfgInfo, i); /* demosaic */
		ISP_FpnRegConfig(ViPipe, pstRegCfgInfo, i); /* FPN */
		ISP_LdciRegConfig(ViPipe, pstRegCfgInfo, i); /* ldci */
		ISP_LcacRegConfig(ViPipe, pstRegCfgInfo, i); /* Local cac */
		ISP_FcrRegConfig(ViPipe, pstRegCfgInfo, i); /* FCR */
		ISP_DpcRegConfig(ViPipe, pstRegCfgInfo, i); /* dpc */
		ISP_GeRegConfig(ViPipe, pstRegCfgInfo, i); /* ge */
		ISP_LscRegConfig(ViPipe, pstRegCfgInfo, i); /* BE LSC */
		ISP_GammaRegConfig(ViPipe, pstRegCfgInfo, i); /* gamma */
		ISP_CscRegConfig(ViPipe, pstRegCfgInfo, i); /* csc */
		ISP_CaRegConfig(ViPipe, pstRegCfgInfo, i); /* ca */
		ISP_McdsRegConfig(ViPipe, pstRegCfgInfo, i); /* mcds */
		ISP_WdrRegConfig(ViPipe, pstRegCfgInfo, i); /* wdr */
		ISP_DrcRegConfig(ViPipe, pstRegCfgInfo, i); /* drc */
		ISP_DehazeRegConfig(ViPipe, pstRegCfgInfo, i); /* Dehaze */
		ISP_BayerNrRegConfig(ViPipe, pstRegCfgInfo, i); /* BayerNR */
		ISP_DgRegConfig(ViPipe, pstRegCfgInfo, i); /* DG */
		ISP_4DgRegConfig(ViPipe, pstRegCfgInfo, i); /* 4DG */
		ISP_PreGammaRegConfig(ViPipe, pstRegCfgInfo, i); /* PreGamma */
		ISP_BeBlcRegConfig(ViPipe, pstRegCfgInfo, i);
		ISP_ExpanderRegConfig(ViPipe, pstRegCfgInfo, i); /* expander */
		ISP_RgbirRegConfig(ViPipe, pstRegCfgInfo, i); /* Rgbir */
	}

	for (i = 0; i < pstRegCfgInfo->u8CfgNum; i++) {
		pstViProcReg =
			(S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
		ISP_CHECK_POINTER(pstViProcReg);
		ISP_BeAlgSyncRegConfig(ViPipe, pstRegCfgInfo, i);
		ISP_BeAlgLut2SttRegnewRegConfig(ViPipe, pstRegCfgInfo, i);
		isp_be_reg_up_write(pstViProcReg, GK_TRUE);
		ISP_BeAlgLutUpdateRegConfig(ViPipe, pstRegCfgInfo, i);
	}

	ISP_BeSyncParamInit(ViPipe, pstRegCfgInfo);

	return GK_SUCCESS;
}

GK_S32 ISP_RegCfgInit(VI_PIPE ViPipe)
{
	ISP_REGCFG_S *pstRegCfgCtx = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pstRegCfgCtx);

	if (pstRegCfgCtx == GK_NULL) {
		pstRegCfgCtx = (ISP_REGCFG_S *)ISP_MALLOC(sizeof(ISP_REGCFG_S));
		if (pstRegCfgCtx == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] RegCfgCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pstRegCfgCtx, 0, sizeof(ISP_REGCFG_S));

	ISP_REGCFG_SET_CTX(ViPipe, pstRegCfgCtx);

	return GK_SUCCESS;
}

GK_S32 ISP_RegCfgExit(VI_PIPE ViPipe)
{
	ISP_REGCFG_S *pstRegCfgCtx = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pstRegCfgCtx);
	ISP_FREE(pstRegCfgCtx);
	ISP_REGCFG_RESET_CTX(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_GetRegCfgCtx(VI_PIPE ViPipe, GK_VOID **ppCfg)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_REGCFG_S *pstRegCfg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
	ISP_CHECK_POINTER(pstRegCfg);

	if (!pstRegCfg->bInit) {
		pstRegCfg->stRegCfg.unKey.u64Key = 0;

		pstRegCfg->bInit = GK_TRUE;
	}

	pstRegCfg->stRegCfg.u8CfgNum = pstIspCtx->stBlockAttr.u8BlockNum;

	*ppCfg = &pstRegCfg->stRegCfg;

	return GK_SUCCESS;
}

GK_S32 ISP_RegCfgInfoInit(VI_PIPE ViPipe)
{
	ISP_REGCFG_S *pstRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
	ISP_CHECK_POINTER(pstRegCfg);

	ISP_FeRegsConfig(ViPipe, &pstRegCfg->stRegCfg);
	ISP_BeRegsConfigInit(ViPipe, &pstRegCfg->stRegCfg);
	return GK_SUCCESS;
}

GK_S32 ISP_RegCfgInfoSet(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_REGCFG_S *pstRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
	ISP_CHECK_POINTER(pstRegCfg);

	ISP_FeRegsConfig(ViPipe, &pstRegCfg->stRegCfg);
	ISP_BeRegsConfig(ViPipe, &pstRegCfg->stRegCfg);

	if (pstRegCfg->stRegCfg.stKernelRegCfg.unKey.u32Key) {
		s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_REG_CFG_SET,
			       &pstRegCfg->stRegCfg.stKernelRegCfg);

		if (s32Ret != GK_SUCCESS) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Config ISP register Failed with ec %#x!\n",
				  s32Ret);
			return s32Ret;
		}
	}

	return GK_SUCCESS;
}

GK_VOID ISP_SnsRegsInfoCheck(VI_PIPE ViPipe,
			     ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
	if ((pstSnsRegsInfo->enSnsType >= ISP_SNS_TYPE_BUTT)) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "senor's regs info invalid, enSnsType %d\n",
			  pstSnsRegsInfo->enSnsType);
		return;
	}

	if (pstSnsRegsInfo->u32RegNum > ISP_MAX_SNS_REGS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "senor's regs info invalid, u32RegNum %d\n",
			  pstSnsRegsInfo->u32RegNum);
		return;
	}

	return;
}

GK_S32 ISP_SyncCfgSet(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	GK_S32 s32PipeSt = 0;
	GK_S32 s32PipeEd = 0;
	GK_S8 s8StitchMainPipe;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_REGCFG_S *pstRegCfg = GK_NULL;
	ISP_REGCFG_S *pstRegCfgS = GK_NULL;
	ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
		s8StitchMainPipe = pstIspCtx->stStitchAttr.as8StitchBindId[0];

		if (IS_STITCH_MAIN_PIPE(ViPipe, s8StitchMainPipe)) {
			s32PipeSt = 0;
			s32PipeEd = pstIspCtx->stStitchAttr.u8StitchPipeNum - 1;
		} else {
			s32PipeSt = ViPipe;
			s32PipeEd = ViPipe - 1;
		}
	} else {
		s32PipeSt = ViPipe;
		s32PipeEd = ViPipe;
	}

	while (s32PipeSt <= s32PipeEd) {
		if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
			ViPipe = pstIspCtx->stStitchAttr
					 .as8StitchBindId[s32PipeSt];
		} else {
			ViPipe = s32PipeSt;
		}

		ISP_GET_CTX(ViPipe, pstIspCtx);
		ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
		ISP_CHECK_POINTER(pstRegCfg);
		ISP_CHECK_OPEN(ViPipe);

		if (ISP_SensorUpdateSnsReg(ViPipe) != GK_SUCCESS) {
			/* If Users need to config AE sync info themselves, they can set pfn_cmos_get_sns_reg_info to NULL in cmos.c */
			/* Then there will be NO AE sync configs in kernel of firmware */
			return GK_SUCCESS;
		}

		ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
		memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo, pstSnsRegsInfo,
		       sizeof(ISP_SNS_REGS_INFO_S));
		ISP_SnsRegsInfoCheck(ViPipe,
				     &pstRegCfg->stSyncCfgNode.stSnsRegsInfo);
		memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg,
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0].stAeRegCfg2,
		       sizeof(ISP_AE_REG_CFG_2_S));
		memcpy(&pstRegCfg->stSyncCfgNode.stAWBRegCfg.au32WDRWBGain[0],
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
				.stAwbRegCfg.stAwbRegDynCfg.au32WDRWBGain[0],
		       sizeof(GK_U32) * ISP_BAYER_CHN_NUM);
		memcpy(&pstRegCfg->stSyncCfgNode.stAWBRegCfg
				.au32BEWhiteBalanceGain[0],
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
				.stAwbRegCfg.stAwbRegDynCfg
				.au32BEWhiteBalanceGain[0],
		       sizeof(GK_U32) * ISP_BAYER_CHN_NUM);
		memcpy(&pstRegCfg->stSyncCfgNode.stAWBRegCfg.au16ColorMatrix[0],
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
				.stAwbRegCfg.stAwbRegDynCfg.au16BEColorMatrix[0],
		       sizeof(GK_U16) * CCM_MATRIX_SIZE);
		memcpy(&pstRegCfg->stSyncCfgNode.stDRCRegCfg,
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
				.stDrcRegCfg.stSyncRegCfg,
		       sizeof(ISP_DRC_REG_CFG_2_S));
		memcpy(&pstRegCfg->stSyncCfgNode.stWDRRegCfg,
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
				.stWdrRegCfg.stSyncRegCfg,
		       sizeof(ISP_FSWDR_SYNC_CFG_S));
		pstRegCfg->stSyncCfgNode.stAWBRegCfg.enBEAWBSwitch =
			pstRegCfg->stRegCfg.stAlgRegCfg[0]
				.stAwbRegCfg.stAwbRegUsrCfg.enBEAWBSwitch;
		memcpy_s(
			&pstRegCfg->stSyncCfgNode.be_blc_reg_cfg,
			sizeof(isp_be_blc_dyna_cfg),
			&pstRegCfg->stRegCfg.stAlgRegCfg[0].stBeBlcCfg.dyna_blc,
			sizeof(isp_be_blc_dyna_cfg));
		memcpy_s(
			&pstRegCfg->stSyncCfgNode.fe_blc_reg_cfg,
			sizeof(isp_fe_blc_dyna_cfg),
			&pstRegCfg->stRegCfg.stAlgRegCfg[0].stFeBlcCfg.dyna_blc,
			sizeof(isp_fe_blc_dyna_cfg));

		if (pstIspCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
			s8StitchMainPipe =
				pstIspCtx->stStitchAttr.as8StitchBindId[0];
			ISP_REGCFG_GET_CTX(s8StitchMainPipe, pstRegCfgS);
			if (pstRegCfgS == GK_NULL) {
				return GK_FAILURE;
			}

			if (!IS_STITCH_MAIN_PIPE(ViPipe, s8StitchMainPipe)) {
				memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo,
				       &pstRegCfgS->stSyncCfgNode.stSnsRegsInfo,
				       sizeof(ISP_SNS_REGS_INFO_S));
				memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo
						.unComBus,
				       &pstSnsRegsInfo->unComBus,
				       sizeof(ISP_SNS_COMMBUS_U));
				memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo
						.stSlvSync.u32SlaveBindDev,
				       &pstSnsRegsInfo->stSlvSync
						.u32SlaveBindDev,
				       sizeof(GK_U32));
				memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg,
				       &pstRegCfgS->stRegCfg.stAlgRegCfg[0]
						.stAeRegCfg2,
				       sizeof(ISP_AE_REG_CFG_2_S));
				memcpy(&pstRegCfg->stSyncCfgNode.stAWBRegCfg
						.au16ColorMatrix[0],
				       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
						.stAwbRegCfg.stAwbRegDynCfg
						.au16BEColorMatrix[0],
				       sizeof(GK_U16) * CCM_MATRIX_SIZE);
				memcpy(&pstRegCfg->stSyncCfgNode.stAWBRegCfg
						.au32BEWhiteBalanceGain[0],
				       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
						.stAwbRegCfg.stAwbRegDynCfg
						.au32BEWhiteBalanceGain[0],
				       sizeof(GK_U32) * ISP_BAYER_CHN_NUM);
				memcpy(&pstRegCfg->stSyncCfgNode.stAWBRegCfg
						.au32WDRWBGain[0],
				       &pstRegCfg->stRegCfg.stAlgRegCfg[0]
						.stAwbRegCfg.stAwbRegDynCfg
						.au32WDRWBGain[0],
				       sizeof(GK_U32) * ISP_BAYER_CHN_NUM);
				pstRegCfg->stSyncCfgNode.stAWBRegCfg
					.enBEAWBSwitch =
					pstRegCfg->stRegCfg.stAlgRegCfg[0]
						.stAwbRegCfg.stAwbRegUsrCfg
						.enBEAWBSwitch;
			}
		}

		pstRegCfg->stSyncCfgNode.bValid = GK_TRUE;

		s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_CFG_SET,
			       &pstRegCfg->stSyncCfgNode);

		if (s32Ret != GK_SUCCESS) {
			return s32Ret;
		}

		pstSnsRegsInfo->bConfig = GK_TRUE;

		s32PipeSt++;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SnapRegCfgSet(VI_PIPE ViPipe, ISP_CONFIG_INFO_S *pstSnapInfo)
{
	GK_S32 s32Ret;

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_CONFIG_INFO_SET, pstSnapInfo);

	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SnapRegCfgGet(VI_PIPE ViPipe, ISP_SNAP_INFO_S *pstSnapInfo)
{
	GK_S32 s32Ret;

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SNAP_INFO_GET, pstSnapInfo);

	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

GK_BOOL ISP_ProTriggerGet(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	GK_BOOL bEnable;

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRO_TRIGGER_GET, &bEnable);

	if (s32Ret != GK_SUCCESS) {
		return GK_FALSE;
	}

	return bEnable;
}

GK_S32 ISP_RegCfgCtrl(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_REGCFG_S *pstRegCfg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

	pstRegCfg->stRegCfg.unKey.u64Key = 0xFFFFFFFFFFFFFFFF;

	for (i = pstIspCtx->stBlockAttr.u8PreBlockNum;
	     i < pstIspCtx->stBlockAttr.u8BlockNum; i++) {
		memcpy(&pstRegCfg->stRegCfg.stAlgRegCfg[i],
		       &pstRegCfg->stRegCfg.stAlgRegCfg[0],
		       sizeof(ISP_ALG_REG_CFG_S));
	}

	if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		pstIspCtx->stSpecialOpt.abBeOnSttUpdate[0] = GK_TRUE;

		for (i = pstIspCtx->stBlockAttr.u8PreBlockNum;
		     i < pstIspCtx->stBlockAttr.u8BlockNum; i++) {
			ISP_BeReshCfg(&pstRegCfg->stRegCfg.stAlgRegCfg[i]);
		}
	}

	pstRegCfg->stRegCfg.u8CfgNum = pstIspCtx->stBlockAttr.u8BlockNum;

	return GK_SUCCESS;
}

static gk_void ISP_LdciLutApbRegConfig(S_ISPBE_REGS_TYPE *be_reg,
				       ISP_LDCI_DYNA_CFG_S *dyna_reg_cfg,
				       ISP_LDCI_STATIC_CFG_S *static_reg_cfg)
{
	gk_u16 j;

	isp_ldci_cgain_waddr_u32_write(be_reg, 0);
	isp_ldci_de_usm_waddr_u32_write(be_reg, 0);
	isp_ldci_he_waddr_u32_write(be_reg, 0);

	for (j = 0; j < LDCI_COLOR_GAIN_LUT_SIZE; j++) {
		isp_ldci_cgain_wdata_u32_write(
			be_reg, dyna_reg_cfg->u32ColorGainLut[j]);
	}

	for (j = 0; j < LDCI_DE_USM_LUT_SIZE; j++) {
		isp_ldci_de_usm_wdata_u32_write(be_reg,
						dyna_reg_cfg->u32UsmPosLut[j],
						dyna_reg_cfg->u32UsmNegLut[j]);
	}

	for (j = 0; j < LDCI_HE_LUT_SIZE; j++) {
		isp_ldci_he_wdata_u32_write(be_reg,
					    dyna_reg_cfg->u32HePosLut[j],
					    dyna_reg_cfg->u32HeNegLut[j]);
	}
}

static GK_S32 ISP_LutApbRegconfig(VI_PIPE ViPipe, ISP_REGCFG_S *pstRegCfg)
{
	GK_U8 i;
	GK_U16 j;
	GK_U32 u32SharpenLutData;
	S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;
	GK_U32 u32BnrLutData;
	ISP_ALG_REG_CFG_S *pstAlgRegCfg = GK_NULL;

	for (i = 0; i < pstRegCfg->stRegCfg.u8CfgNum; i++) {
		pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
		pstAlgRegCfg = &pstRegCfg->stRegCfg.stAlgRegCfg[i];

		ISP_CHECK_POINTER(pstBeReg);

		/* gamma */
		isp_gamma_lut_waddr_u32_write(pstBeReg, 0);

		for (j = 0; j < GAMMA_REG_NODE_NUM; j++) {
			isp_gamma_lut_wdata_u32_write(
				pstBeReg,
				pstRegCfg->stRegCfg.stAlgRegCfg[i]
					.stGammaCfg.stUsrRegCfg.au16GammaLUT[j]);
		}

		/* sharpen */
		isp_sharpen_mhfgaind_waddr_u32_write(pstBeReg, 0);
		isp_sharpen_mhfgainud_waddr_u32_write(pstBeReg, 0);

		for (j = 0; j < SHRP_GAIN_LUT_SIZE; j++) {
			u32SharpenLutData =
				(pstRegCfg->stRegCfg.stAlgRegCfg[i]
					 .stSharpenRegCfg.stDynaRegCfg
					 .stMpiDynaRegCfg.u16hfGainD[j]
				 << 12) +
				(pstRegCfg->stRegCfg.stAlgRegCfg[i]
					 .stSharpenRegCfg.stDynaRegCfg
					 .stMpiDynaRegCfg.u16mfGainD[j]);
			isp_sharpen_mhfgaind_wdata_u32_write(pstBeReg,
							     u32SharpenLutData);

			u32SharpenLutData =
				(pstRegCfg->stRegCfg.stAlgRegCfg[i]
					 .stSharpenRegCfg.stDynaRegCfg
					 .stMpiDynaRegCfg.u16hfGainUD[j]
				 << 12) +
				(pstRegCfg->stRegCfg.stAlgRegCfg[i]
					 .stSharpenRegCfg.stDynaRegCfg
					 .stMpiDynaRegCfg.u16mfGainUD[j]);

			isp_sharpen_mhfgainud_wdata_u32_write(
				pstBeReg, u32SharpenLutData);
		}

		/* PreGamma */
		isp_drc_pregamma_waddr_u32_write(pstBeReg, 0);
		for (j = 0; j < PREGAMMA_NODE_NUM; j++) {
			isp_drc_pregamma_wdata_u32_write(
				pstBeReg, pstRegCfg->stRegCfg.stAlgRegCfg[i]
						  .stPreGammaCfg.stDynaRegCfg
						  .au16PreGammaLUT[j]);
		}
		isp_drc_lut_update1_u32_write(pstBeReg, GK_TRUE);

		// bnr
		isp_bnr_lmt_even_waddr_u32_write(pstBeReg, 0);
		for (j = 0; j < ISP_BAYERNR_LMTLUT_EVEN_NUM; j++) {
			u32BnrLutData = (pstRegCfg->stRegCfg.stAlgRegCfg[i]
						 .stBnrRegCfg.stDynaRegCfg
						 .au8JnlmLimitSEvenLut[j]
					 << 8) +
					(pstRegCfg->stRegCfg.stAlgRegCfg[i]
						 .stBnrRegCfg.stDynaRegCfg
						 .au8JnlmLimitEvenLut[j] &
					 0xff);
			isp_bnr_lmt_even_wdata_u32_write(pstBeReg,
							 u32BnrLutData);
		}

		isp_bnr_lmt_odd_waddr_u32_write(pstBeReg, 0);
		for (j = 0; j < ISP_BAYERNR_LMTLUT_ODD_NUM; j++) {
			u32BnrLutData = (pstRegCfg->stRegCfg.stAlgRegCfg[i]
						 .stBnrRegCfg.stDynaRegCfg
						 .au8JnlmLimitSOddLut[j]
					 << 8) +
					(pstRegCfg->stRegCfg.stAlgRegCfg[i]
						 .stBnrRegCfg.stDynaRegCfg
						 .au8JnlmLimitOddLut[j] &
					 0xff);

			isp_bnr_lmt_odd_wdata_u32_write(pstBeReg,
							u32BnrLutData);
		}

		isp_bnr_cor_even_waddr_u32_write(pstBeReg, 0);
		isp_bnr_gcor_even_waddr_u32_write(pstBeReg, 0);
		for (j = 0; j < ISP_BAYERNR_LUT_EVEN_LENGTH; j++) {
			isp_bnr_cor_even_wdata_u32_write(
				pstBeReg, pstRegCfg->stRegCfg.stAlgRegCfg[i]
						  .stBnrRegCfg.stDynaRegCfg
						  .au16JnlmCoringLowEvenLut[j]);
			isp_bnr_gcor_even_wdata_u32_write(
				pstBeReg, pstRegCfg->stRegCfg.stAlgRegCfg[i]
						  .stBnrRegCfg.stDynaRegCfg
						  .au8GcoringEvenLut[j]);
		}

		isp_bnr_cor_odd_waddr_u32_write(pstBeReg, 0);
		isp_bnr_gcor_odd_waddr_u32_write(pstBeReg, 0);
		for (j = 0; j < ISP_BAYERNR_LUT_ODD_LENGTH; j++) {
			isp_bnr_cor_odd_wdata_u32_write(
				pstBeReg, pstRegCfg->stRegCfg.stAlgRegCfg[i]
						  .stBnrRegCfg.stDynaRegCfg
						  .au16JnlmCoringLowOddLut[j]);
			isp_bnr_gcor_odd_wdata_u32_write(
				pstBeReg, pstRegCfg->stRegCfg.stAlgRegCfg[i]
						  .stBnrRegCfg.stDynaRegCfg
						  .au8GcoringOddLut[j]);
		}

		/* LDCI */
		ISP_LdciLutApbRegConfig(
			pstBeReg, &pstAlgRegCfg->stLdciRegCfg.stDynaRegCfg,
			&pstAlgRegCfg->stLdciRegCfg.stStaticRegCfg);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SwitchRegSet(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_REGCFG_S *pstRegCfg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
		ISP_CHECK_POINTER(pstRegCfg);
		ISP_RegCfgInfoInit(ViPipe);
		ISP_LutApbRegconfig(ViPipe, pstRegCfg);
		if (pstRegCfg->stRegCfg.stKernelRegCfg.unKey.u32Key) {
			s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_REG_CFG_SET,
				       &pstRegCfg->stRegCfg.stKernelRegCfg);

			if (s32Ret != GK_SUCCESS) {
				ISP_TRACE(
					MODULE_DBG_ERR,
					"Config ISP register Failed with ec %#x!\n",
					s32Ret);
				return s32Ret;
			}
		}

		pstIspCtx->stBlockAttr.u8PreBlockNum =
			pstIspCtx->stBlockAttr.u8BlockNum;

		return GK_SUCCESS;
	}

	/* record the register config infomation to fhy and kernel,and be valid in next frame. */
	s32Ret = ISP_RegCfgInfoInit(ViPipe);
	if (s32Ret) {
		return s32Ret;
	}

	pstIspCtx->stIspParaRec.bStitchSync = GK_TRUE;
	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET,
		       &pstIspCtx->stIspParaRec.bStitchSync);
	if (s32Ret != GK_SUCCESS) {
		pstIspCtx->stIspParaRec.bStitchSync = GK_FALSE;
		ISP_TRACE(MODULE_DBG_ERR,
			  "ISP[%d] set isp stitch sync failed!\n", ViPipe);
	}

	s32Ret = ISP_AllCfgsBeBufInit(ViPipe);
	if (s32Ret != GK_SUCCESS) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Pipe:%d init all be bufs failed %x!\n", ViPipe,
			  s32Ret);
		return s32Ret;
	}

	pstIspCtx->stBlockAttr.u8PreBlockNum =
		pstIspCtx->stBlockAttr.u8BlockNum;

	return GK_SUCCESS;
}

GK_S32 ISP_Lut2SttApbReg(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_REGCFG_S *pstRegCfg = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) ||
	    IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
		ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
		ISP_CHECK_POINTER(pstRegCfg);

		ISP_LutApbRegconfig(ViPipe, pstRegCfg);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_SwitchStateSet(VI_PIPE ViPipe)
{
	GK_S32 s32Ret;

	s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_SWITCH_FINISH_STATE_SET);

	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
