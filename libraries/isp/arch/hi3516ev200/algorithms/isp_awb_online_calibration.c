/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "comm_isp.h"
#include "ot_mpi_isp.h"
#include "isp_math_utils.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_S32 ISP_GetLightboxGain(VI_PIPE ViPipe, ISP_AWB_Calibration_Gain_S *pstAWBCalibrationGain)
{
    GK_U32 u32Rsum = 0;
    GK_U32 u32Bsum = 0;
    GK_U32 u32Gsum = 0;
    GK_S32 i, j;
    GK_S32 s32ZoneCol;
    GK_S32 s32ZoneRow;
    GK_S32 s32ZoneColStart;
    GK_S32 s32ZoneColEnd;
    GK_S32 s32ZoneRowStart;
    GK_S32 s32ZoneRowEnd;
    GK_S32 s32Ret;
    ISP_WB_STATISTICS_S  *pstWBStat  = NULL;
    ISP_STATISTICS_CFG_S *pstStatCfg = NULL;

    pstWBStat = (ISP_WB_STATISTICS_S *)ISP_MALLOC(sizeof(ISP_WB_STATISTICS_S));
    if (pstWBStat == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "pstWBStat malloc failure !\n");
        return ERR_CODE_ISP_NOMEM;
    }
    pstStatCfg = (ISP_STATISTICS_CFG_S *)ISP_MALLOC(sizeof(ISP_STATISTICS_CFG_S));
    if (pstStatCfg == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "pstWBStat malloc failure !\n");
        ISP_FREE(pstWBStat);

        return ERR_CODE_ISP_NOMEM;
    }
    s32Ret = MPI_ISP_GetStatisticsConfig(ViPipe, pstStatCfg);
    if (s32Ret != GK_SUCCESS) {
        ISP_FREE(pstWBStat);
        ISP_FREE(pstStatCfg);

        return s32Ret;
    }
    if (pstStatCfg->stWBCfg.u16ZoneCol * pstStatCfg->stWBCfg.u16ZoneRow < 16) {
        ISP_TRACE(MODULE_DBG_ERR, "Not support zone number less than 16 !\n");
        ISP_FREE(pstWBStat);
        ISP_FREE(pstStatCfg);

        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    s32Ret = MPI_ISP_GetWBStatistics(ViPipe, pstWBStat);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Get WB statics failed!\n");
        ISP_FREE(pstWBStat);
        ISP_FREE(pstStatCfg);

        return s32Ret;
    }

    s32ZoneCol = pstStatCfg->stWBCfg.u16ZoneCol;
    s32ZoneRow = pstStatCfg->stWBCfg.u16ZoneRow;
    s32ZoneColStart = s32ZoneCol / 2 - 2;
    s32ZoneColEnd   = s32ZoneCol / 2 + 2;
    s32ZoneRowStart = s32ZoneRow / 2 - 2;
    s32ZoneRowEnd   = s32ZoneRow / 2 + 2;

    /* Get_statistics */
    for (j = s32ZoneRowStart; j < s32ZoneRowEnd; j++) {
        for (i = j * s32ZoneCol + s32ZoneColStart; i < j * s32ZoneCol + s32ZoneColEnd; i++) {
            u32Rsum += pstWBStat->au16ZoneAvgR[i];
            u32Bsum += pstWBStat->au16ZoneAvgB[i];
            u32Gsum += pstWBStat->au16ZoneAvgG[i];
        }
    }

    pstAWBCalibrationGain->u16AvgRgain = (u32Gsum << 8) / DIV_0_TO_1(u32Rsum); /* G/R*256 */
    pstAWBCalibrationGain->u16AvgBgain = (u32Gsum << 8) / DIV_0_TO_1(u32Bsum); /* avarage for 16 zones */

    ISP_FREE(pstWBStat);
    ISP_FREE(pstStatCfg);

    return GK_SUCCESS;
}
