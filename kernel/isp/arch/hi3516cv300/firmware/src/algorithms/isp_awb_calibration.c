/******************************************************************************

  Copyright (C), 2016-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_awb_calibration.c
  Version       : Initial Draft
  Author        : Hisilicon BVT PQ group
  Created       : 2016/12/15
  Description   : 
  History       :
  1.Date        : 2016/12/15
    Author      : h00372898
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hi_comm_isp.h"
#include "mpi_isp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define ROW_START 14
#define ROW_END 18
#define COLUMN_START 14
#define COLUMN_END 18

HI_S32 ISP_GetLightboxGain(ISP_DEV IspDev,ISP_AWB_Calibration_Gain_S *pstAWBCalibrationGain)
{
    HI_U32 u32Rsum = 0;
    HI_U32 u32Bsum = 0;
    HI_U32 u32Gsum = 0;
    HI_S32 i,j;
    ISP_STATISTICS_S *pstStatisticsAttr = NULL;
	
    pstStatisticsAttr = (ISP_STATISTICS_S *)malloc(sizeof(ISP_STATISTICS_S));	
	
    if(pstStatisticsAttr == HI_NULL)
    {
        printf("%s: LINE: %d pstStatisticsAttr malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    if (HI_SUCCESS != HI_MPI_ISP_GetStatistics(IspDev, pstStatisticsAttr))
    {
        printf("%s: LINE: %d Get Statistics failure !\n",__FUNCTION__,__LINE__);
        free(pstStatisticsAttr);
        pstStatisticsAttr = HI_NULL;
        return HI_FAILURE;
    }
	
    /*Get_statistics*/
    for (i = COLUMN_START; i < COLUMN_END; i++)
    {
        for (j = ROW_START; j < ROW_END; j++)
        {
            u32Rsum += pstStatisticsAttr->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j];
            u32Bsum += pstStatisticsAttr->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j];
            u32Gsum += pstStatisticsAttr->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j];
 #if 0
            printf("CurR =%8d, CurB = %8d, CurG = %8d\n", pstStatisticsAttr->stWBStat.stBayerStatistics.au16ZoneAvgR[i], pstStatisticsAttr->stWBStat.stBayerStatistics.au16ZoneAvgB[i], pstStatisticsAttr->stWBStat.stBayerStatistics.au16ZoneAvgG[i]);
 #endif
        }
    }

    pstAWBCalibrationGain->u16AvgRgain = (u32Gsum << 8) / DIV_0_TO_1(u32Rsum) ; /* G/R*256 */
    pstAWBCalibrationGain->u16AvgBgain = (u32Gsum << 8) / DIV_0_TO_1(u32Bsum) ; /*avarage for 16 zones*/

#if 0
    printf("u16AvgRgain =%8d, u16AvgBgain = %8d\n", pstAWBCalibrationGain->u16AvgRgain, pstAWBCalibrationGain->u16AvgBgain);
#endif
		
    free(pstStatisticsAttr);
    return HI_SUCCESS;
}

     
