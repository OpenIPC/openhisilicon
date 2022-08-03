/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __MPI_SYS_H__
#define __MPI_SYS_H__

#include "type.h"
#include "common.h"
#include "comm_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_S32 MPI_SYS_Init(GK_VOID);
GK_S32 MPI_SYS_Exit(GK_VOID);

GK_S32 MPI_SYS_SetConfig(const MPP_SYS_CONFIG_S *pstSysConfig);
GK_S32 MPI_SYS_GetConfig(MPP_SYS_CONFIG_S *pstSysConfig);

GK_S32 MPI_SYS_Bind(const MPP_CHN_S *pstSrcChn, const MPP_CHN_S *pstDestChn);
GK_S32 MPI_SYS_UnBind(const MPP_CHN_S *pstSrcChn, const MPP_CHN_S *pstDestChn);
GK_S32 MPI_SYS_GetBindbyDest(const MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn);
GK_S32 MPI_SYS_GetBindbySrc(const MPP_CHN_S *pstSrcChn,
			    MPP_BIND_DEST_S *pstBindDest);

GK_S32 MPI_SYS_GetVersion(MPP_VERSION_S *pstVersion);

GK_S32 MPI_SYS_GetChipId(GK_U32 *pu32ChipId);

GK_S32 MPI_SYS_GetUniqueId(UNIQUE_ID_S *pstUniqueId);

GK_S32 MPI_SYS_GetCustomCode(GK_U32 *pu32CustomCode);

/*
 * u64Base is the global PTS of the system.
 * ADVICE:
 * 1. Better to call MPI_SYS_GetCurPTS on the host board to get u64Base.
 * 2. When os start up, call MPI_SYS_InitPTSBase to set the init PTS.
 * 3. When media bussines is running, synchronize the PTS one time per minute
 *     by calling MPI_SYS_SyncPTS.
 */
GK_S32 MPI_SYS_GetCurPTS(GK_U64 *pu64CurPTS);
GK_S32 MPI_SYS_InitPTSBase(GK_U64 u64PTSBase);
GK_S32 MPI_SYS_SyncPTS(GK_U64 u64PTSBase);

/* alloc mmz memory in user context */
GK_S32 MPI_SYS_MmzAlloc(GK_U64 *pu64PhyAddr, GK_VOID **ppVirAddr,
			const GK_CHAR *strMmb, const GK_CHAR *strZone,
			GK_U32 u32Len);

/* alloc mmz memory with cache */
GK_S32 MPI_SYS_MmzAlloc_Cached(GK_U64 *pu64PhyAddr, GK_VOID **ppVirAddr,
			       const GK_CHAR *pstrMmb, const GK_CHAR *pstrZone,
			       GK_U32 u32Len);

/* free mmz memory in user context */
GK_S32 MPI_SYS_MmzFree(GK_U64 u64PhyAddr, GK_VOID *pVirAddr);

/* fulsh cache */
GK_S32 MPI_SYS_MmzFlushCache(GK_U64 u64PhyAddr, GK_VOID *pVirAddr,
			     GK_U32 u32Size);

/*
 * Call the mmap function to map physical address to virtual address
 * The system function mmap is too complicated, so we packge it.
 */
GK_VOID *MPI_SYS_Mmap(GK_U64 u64PhyAddr, GK_U32 u32Size);
GK_VOID *MPI_SYS_MmapCache(GK_U64 u64PhyAddr, GK_U32 u32Size);
GK_S32 MPI_SYS_Munmap(GK_VOID *pVirAddr, GK_U32 u32Size);
GK_S32 MPI_SYS_MflushCache(GK_U64 u64PhyAddr, GK_VOID *pVirAddr,
			   GK_U32 u32Size);

GK_S32 MPI_SYS_SetMemConfig(const MPP_CHN_S *pstMppChn,
			    const GK_CHAR *pcMmzName);
GK_S32 MPI_SYS_GetMemConfig(const MPP_CHN_S *pstMppChn, GK_CHAR *pcMmzName);

/* Close all the FD which is used by sys module */
GK_S32 MPI_SYS_CloseFd(GK_VOID);

/* Get virtual meminfo according to virtual addr, should be in one process */
GK_S32 MPI_SYS_GetVirMemInfo(const void *pVirAddr,
			     SYS_VIRMEM_INFO_S *pstMemInfo);

/* Set/get Scale coefficient level for VPSS/VGS */
GK_S32 MPI_SYS_SetScaleCoefLevel(const SCALE_RANGE_S *pstScaleRange,
				 const SCALE_COEFF_LEVEL_S *pstScaleCoeffLevel);
GK_S32 MPI_SYS_GetScaleCoefLevel(const SCALE_RANGE_S *pstScaleRange,
				 SCALE_COEFF_LEVEL_S *pstScaleCoeffLevel);

/* Set/Get local timezone, range: [-86400, 86400] seconds (that is: [-24, 24] hours)  */
GK_S32 MPI_SYS_SetTimeZone(GK_S32 s32TimeZone);
GK_S32 MPI_SYS_GetTimeZone(GK_S32 *ps32TimeZone);

GK_S32 MPI_SYS_SetGPSInfo(const GPS_INFO_S *pstGPSInfo);
GK_S32 MPI_SYS_GetGPSInfo(GPS_INFO_S *pstGPSInfo);

GK_S32 MPI_SYS_SetTuningConnect(GK_S32 s32Connect);
GK_S32 MPI_SYS_GetTuningConnect(GK_S32 *ps32Connect);

GK_S32 MPI_SYS_SetVIVPSSMode(const VI_VPSS_MODE_S *pstVIVPSSMode);
GK_S32 MPI_SYS_GetVIVPSSMode(VI_VPSS_MODE_S *pstVIVPSSMode);

GK_S32 MPI_SYS_GetVPSSVENCWrapBufferLine(VPSS_VENC_WRAP_PARAM_S *pWrapParam,
					 GK_U32 *pu32BufLine);

GK_S32 MPI_LOG_SetLevelConf(LOG_LEVEL_CONF_S *pstConf);
GK_S32 MPI_LOG_GetLevelConf(LOG_LEVEL_CONF_S *pstConf);

GK_S32 MPI_SYS_SetRawFrameCompressParam(
	const RAW_FRAME_COMPRESS_PARAM_S *pstCompressParam);
GK_S32
MPI_SYS_GetRawFrameCompressParam(RAW_FRAME_COMPRESS_PARAM_S *pstCompressParam);

GK_S32 MPI_LOG_SetWaitFlag(GK_BOOL bWait);

GK_S32 MPI_LOG_Read(GK_CHAR *pBuf, GK_U32 u32Size);

GK_VOID MPI_LOG_Close(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __MPI_SYS_H__ */
