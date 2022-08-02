/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_REGCFG_H__
#define __ISP_REGCFG_H__

#include "comm_isp.h"
#include "ot_mpi_isp.h"
#include "isp_sensor.h"
#include "mkp_isp.h"
#include "vi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IS_HRS_ON(ViPipe) (g_astIspCtx[ViPipe].bIsp0P2En == GK_TRUE)
#define ISP_ALIGNUP(x, g) ((((x) + (g)-1) / (g)) * (g))

typedef struct ISP_BE_BUF_S {
    GK_U64 u64BePhyAddr; /* head addr */
    GK_VOID *pBeVirtAddr;

    ISP_BE_WO_CFG_BUF_S stBeWoCfgBuf;
} ISP_BE_BUF_S;

typedef struct ISP_BE_LUT_BUF_S {
    ISP_MMZ_BUF_EX_S astLutSttBuf[ISP_MAX_BE_NUM];
} ISP_BE_LUT_BUF_S;

GK_S32 ISP_ModParamGet(VI_PIPE ViPipe, ISP_MOD_PARAM_S *pstModParam);
GK_S32 ISP_ResetFeSttEn(VI_PIPE ViPipe);
GK_S32 ISP_AlgEnExit(VI_PIPE ViPipe);
GK_S32 ISP_LdciBufInit(VI_PIPE ViPipe);
GK_S32 ISP_LdciBufExit(VI_PIPE ViPipe);
GK_S32 ISP_ClutBufInit(VI_PIPE ViPipe);
GK_S32 ISP_ClutBufExit(VI_PIPE ViPipe);
GK_S32 ISP_SpecAwbBufInit(VI_PIPE ViPipe);
GK_S32 ISP_SpecAwbBufExit(VI_PIPE ViPipe);
GK_S32 ISP_SttBufInit(VI_PIPE ViPipe);
GK_S32 ISP_SttBufExit(VI_PIPE ViPipe);
GK_S32 ISP_SttAddrInit(VI_PIPE ViPipe);
GK_S32 ISP_CfgBeBufInit(VI_PIPE ViPipe);
GK_S32 ISP_CfgBeBufExit(VI_PIPE ViPipe);
GK_S32 ISP_RegCfgInit(VI_PIPE ViPipe);
GK_S32 ISP_GetRegCfgCtx(VI_PIPE ViPipe, GK_VOID **ppCfg);
GK_S32 ISP_RegCfgCtrl(VI_PIPE ViPipe);
GK_S32 ISP_RegCfgExit(VI_PIPE ViPipe);
GK_S32 ISP_SwitchRegSet(VI_PIPE ViPipe);
GK_S32 ISP_GetBeLastBuf(VI_PIPE ViPipe);
GK_S32 ISP_BeVregAddrInit(VI_PIPE ViPipe);
GK_S32 ISP_GetBeVregCfgAddr(VI_PIPE ViPipe, GK_VOID *pVirtAddr[]);
GK_S32 ISP_GetBeBufFirst(VI_PIPE ViPipe);
GK_S32 ISP_GetBeFreeBuf(VI_PIPE ViPipe);
GK_S32 ISP_AllCfgsBeBufInit(VI_PIPE ViPipe);
GK_S32 ISP_SetCfgBeBufState(VI_PIPE ViPipe);
GK_S32 ISP_RegCfgInfoSet(VI_PIPE ViPipe);
GK_S32 ISP_RegCfgInfoInit(VI_PIPE ViPipe);
GK_S32 ISP_Lut2SttApbReg(VI_PIPE ViPipe);
GK_S32 ISP_SyncCfgSet(VI_PIPE ViPipe);
GK_S32 ISP_SnapRegCfgSet(VI_PIPE ViPipe, ISP_CONFIG_INFO_S *pstSnapInfo);
GK_S32 ISP_SnapRegCfgGet(VI_PIPE ViPipe, ISP_SNAP_INFO_S *pstSnapInfo);
GK_BOOL ISP_ProTriggerGet(VI_PIPE ViPipe);
GK_VOID *ISP_GetBeVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev);
GK_VOID *ISP_GetBeLut2SttVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev, GK_U8 U8BufId);
GK_VOID *ISP_GetViProcVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev);
GK_VOID *ISP_GetFeVirAddr(VI_PIPE ViPipe);
GK_S32 ISP_SwitchStateSet(VI_PIPE ViPipe);

#define ISP_CHECK_FE_PIPE(pipe)                              \
    do {                                                     \
        if (((pipe) < 0) || ((pipe) >= ISP_MAX_PIPE_NUM)) {  \
            ISP_TRACE(MODULE_DBG_ERR, "Invalid ISP-FE Pipe!\n"); \
            return GK_NULL;                                  \
        }                                                    \
    } while (0)

#define ISP_CHECK_BE_DEV(dev)                                 \
    do {                                                      \
        if (((dev) < 0) || ((dev) >= ISP_STRIPING_MAX_NUM)) { \
            ISP_TRACE(MODULE_DBG_ERR, "Err ISP-BE dev!\n");       \
            return GK_NULL;                                   \
        }                                                     \
    } while (0)

#define ISP_CHECK_NULLPTR(ptr)                                                \
    do {                                                                      \
        if (ptr == GK_NULL) {                                                 \
            ISP_TRACE(MODULE_DBG_ERR, "Null point when writing isp register!\n"); \
            return GK_NULL;                                                   \
        }                                                                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
