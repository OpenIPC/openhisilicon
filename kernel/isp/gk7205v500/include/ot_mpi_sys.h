/*
 * Copyright (c) XMEDIA. All rights reserved.
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

XMEDIA_S32 MPI_SYS_Init(XMEDIA_VOID);
XMEDIA_S32 MPI_SYS_Exit(XMEDIA_VOID);

XMEDIA_S32 MPI_SYS_SetConfig(const MPP_SYS_CONFIG_S *pstSysConfig);
XMEDIA_S32 MPI_SYS_GetConfig(MPP_SYS_CONFIG_S *pstSysConfig);

XMEDIA_S32 MPI_SYS_Bind(const MPP_CHN_S *pstSrcChn, const MPP_CHN_S *pstDestChn);
XMEDIA_S32 MPI_SYS_UnBind(const MPP_CHN_S *pstSrcChn, const MPP_CHN_S *pstDestChn);
XMEDIA_S32 MPI_SYS_GetBindbyDest(const MPP_CHN_S *pstDestChn, MPP_CHN_S *pstSrcChn);
XMEDIA_S32 MPI_SYS_GetBindbySrc(const MPP_CHN_S *pstSrcChn, MPP_BIND_DEST_S *pstBindDest);

XMEDIA_S32 MPI_SYS_GetVersion(MPP_VERSION_S *pstVersion);

XMEDIA_S32 MPI_SYS_GetChipId(XMEDIA_U32 *pu32ChipId);

XMEDIA_S32 MPI_SYS_GetUniqueId(UNIQUE_ID_S *pstUniqueId);

XMEDIA_S32 MPI_SYS_GetCustomCode(XMEDIA_U32 *pu32CustomCode);

/*
 * u64Base is the global PTS of the system.
 * ADVICE:
 * 1. Better to call MPI_SYS_GetCurPTS on the host board to get u64Base.
 * 2. When os start up, call MPI_SYS_InitPTSBase to set the init PTS.
 * 3. When media bussines is running, synchronize the PTS one time per minute
 *     by calling MPI_SYS_SyncPTS.
 */
XMEDIA_S32 MPI_SYS_GetCurPTS(XMEDIA_U64 *pu64CurPTS);
XMEDIA_S32 MPI_SYS_InitPTSBase(XMEDIA_U64 u64PTSBase);
XMEDIA_S32 MPI_SYS_SyncPTS(XMEDIA_U64 u64PTSBase);

/* alloc mmz memory in user context */
XMEDIA_S32 MPI_SYS_MmzAlloc(XMEDIA_U64 *pu64PhyAddr, XMEDIA_VOID **ppVirAddr,
                           const XMEDIA_CHAR *strMmb, const XMEDIA_CHAR *strZone, XMEDIA_U32 u32Len);

/* alloc mmz memory with cache */
XMEDIA_S32 MPI_SYS_MmzAlloc_Cached(XMEDIA_U64 *pu64PhyAddr, XMEDIA_VOID **ppVirAddr,
                                  const XMEDIA_CHAR *pstrMmb, const XMEDIA_CHAR *pstrZone, XMEDIA_U32 u32Len);

/* free mmz memory in user context */
XMEDIA_S32 MPI_SYS_MmzFree(XMEDIA_U64 u64PhyAddr, XMEDIA_VOID *pVirAddr);

/* fulsh cache */
XMEDIA_S32 MPI_SYS_MmzFlushCache(XMEDIA_U64 u64PhyAddr, XMEDIA_VOID *pVirAddr, XMEDIA_U32 u32Size);

/*
 * Call the mmap function to map physical address to virtual address
 * The system function mmap is too complicated, so we packge it.
 */
XMEDIA_VOID *MPI_SYS_Mmap(XMEDIA_U64 u64PhyAddr, XMEDIA_U32 u32Size);
XMEDIA_VOID *MPI_SYS_MmapCache(XMEDIA_U64 u64PhyAddr, XMEDIA_U32 u32Size);
XMEDIA_S32 MPI_SYS_Munmap(XMEDIA_VOID *pVirAddr, XMEDIA_U32 u32Size);
XMEDIA_S32 MPI_SYS_MflushCache(XMEDIA_U64 u64PhyAddr, XMEDIA_VOID *pVirAddr, XMEDIA_U32 u32Size);

XMEDIA_S32 MPI_SYS_SetMemConfig(const MPP_CHN_S *pstMppChn, const XMEDIA_CHAR *pcMmzName);
XMEDIA_S32 MPI_SYS_GetMemConfig(const MPP_CHN_S *pstMppChn, XMEDIA_CHAR *pcMmzName);

/* Close all the FD which is used by sys module */
XMEDIA_S32 MPI_SYS_CloseFd(XMEDIA_VOID);

/* Get virtual meminfo according to virtual addr, should be in one process */
XMEDIA_S32 MPI_SYS_GetVirMemInfo(const void *pVirAddr, SYS_VIRMEM_INFO_S *pstMemInfo);

/* Set/get Scale coefficient level for VPSS/VGS */
XMEDIA_S32 MPI_SYS_SetScaleCoefLevel(const SCALE_RANGE_S *pstScaleRange,
    const SCALE_COEFF_LEVEL_S *pstScaleCoeffLevel);
XMEDIA_S32 MPI_SYS_GetScaleCoefLevel(const SCALE_RANGE_S *pstScaleRange, SCALE_COEFF_LEVEL_S *pstScaleCoeffLevel);

/* Set/Get local timezone, range: [-86400, 86400] seconds (that is: [-24, 24] hours)  */
XMEDIA_S32 MPI_SYS_SetTimeZone(XMEDIA_S32 s32TimeZone);
XMEDIA_S32 MPI_SYS_GetTimeZone(XMEDIA_S32 *ps32TimeZone);

XMEDIA_S32 MPI_SYS_SetGPSInfo(const GPS_INFO_S *pstGPSInfo);
XMEDIA_S32 MPI_SYS_GetGPSInfo(GPS_INFO_S *pstGPSInfo);

XMEDIA_S32 MPI_SYS_SetTuningConnect(XMEDIA_S32 s32Connect);
XMEDIA_S32 MPI_SYS_GetTuningConnect(XMEDIA_S32 *ps32Connect);

XMEDIA_S32 MPI_SYS_SetVIVPSSMode(const VI_VPSS_MODE_S *pstVIVPSSMode);
XMEDIA_S32 MPI_SYS_GetVIVPSSMode(VI_VPSS_MODE_S *pstVIVPSSMode);

XMEDIA_S32 MPI_SYS_GetVPSSVENCWrapBufferLine(VPSS_VENC_WRAP_PARAM_S *pWrapParam, XMEDIA_U32 *pu32BufLine);

XMEDIA_S32 MPI_LOG_SetLevelConf(LOG_LEVEL_CONF_S *pstConf);
XMEDIA_S32 MPI_LOG_GetLevelConf(LOG_LEVEL_CONF_S *pstConf);

XMEDIA_S32 MPI_SYS_SetRawFrameCompressParam(const RAW_FRAME_COMPRESS_PARAM_S *pstCompressParam);
XMEDIA_S32 MPI_SYS_GetRawFrameCompressParam(RAW_FRAME_COMPRESS_PARAM_S *pstCompressParam);

XMEDIA_S32 MPI_LOG_SetWaitFlag(XMEDIA_BOOL bWait);

XMEDIA_S32 MPI_LOG_Read(XMEDIA_CHAR *pBuf, XMEDIA_U32 u32Size);

XMEDIA_VOID MPI_LOG_Close(XMEDIA_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __MPI_SYS_H__ */

