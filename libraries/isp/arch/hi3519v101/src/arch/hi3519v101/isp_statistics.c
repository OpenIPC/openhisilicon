/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_statistics.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/06
  Description   : 
  History       :
  1.Date        : 2013/05/06
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_statistics.h"
#include "isp_ext_config.h"
#include "isp_main.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_STA_S
{
    HI_U32          u32StatPhyaddr;
    HI_VOID        *pStatVirtAddr;

    HI_BOOL         bRead;
    ISP_STAT_INFO_S stStatInfo;
} ISP_STA_S;

typedef struct hiISP_STITCH_BUF_S
{    
    HI_U32  u32PhyAddr;
    HI_VOID *pVirtAddr;
    HI_U32  u32Size;
} ISP_STITCH_BUF_S;

static ISP_STITCH_BUF_S g_stStatBufInfo = {0};
ISP_STA_S g_astStatCtx[ISP_MAX_DEV_NUM] = {{0}};
#define STATISTICS_GET_CTX(dev, pstCtx)   pstCtx = &g_astStatCtx[dev]

extern HI_S32 g_as32IspFd[ISP_MAX_DEV_NUM];

HI_S32 ISP_StatisticsInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;
    HI_U32 u32DevSta = 0, u32DevEnd = 0;
    ISP_CHECK_DEV(IspDev);

    if(ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
    {
        if(IS_MAIN_CHANNAL(IspDev))    
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev + 1;
           
            g_stStatBufInfo.u32Size = sizeof(ISP_STAT_S);
            s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&g_stStatBufInfo.u32PhyAddr, &g_stStatBufInfo.pVirtAddr,
                                                "ISPStitchBuf", HI_NULL, g_stStatBufInfo.u32Size);
            if(HI_SUCCESS != s32Ret)
            {
                printf("Mmz alloc Stitch statistics bufs failed!\n");
                return s32Ret;
            }
            
            memset(g_stStatBufInfo.pVirtAddr, 0, g_stStatBufInfo.u32Size);                  
        }
        else
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev - 1;
        }

        hi_ext_isp_stitch_buffer_phyaddr_write(IspDev, g_stStatBufInfo.u32PhyAddr);
        hi_ext_isp_stitch_buffer_size_write(IspDev, g_stStatBufInfo.u32Size);     
    }
    else
    {
        u32DevSta = IspDev;
        u32DevEnd = IspDev;
    }

    while(u32DevSta <= u32DevEnd)
    {
        IspDev = u32DevSta;
        STATISTICS_GET_CTX(IspDev, pstStat);
        ISP_CHECK_OPEN(IspDev);

        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_INIT, &pstStat->u32StatPhyaddr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("init statistics bufs failed %x!\n", s32Ret);
            return s32Ret;
        }

        pstStat->pStatVirtAddr = HI_MPI_SYS_MmapCache(pstStat->u32StatPhyaddr,
            sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);

        if (HI_NULL == pstStat->pStatVirtAddr)
        {
            printf("mmap statistics bufs failed!\n");
            s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_EXIT);
            if (HI_SUCCESS != s32Ret)
            {
                printf("exit statistics bufs failed %x!\n", s32Ret);
                return s32Ret;
            }

            return HI_ERR_ISP_NOMEM;
        }
        
        u32DevSta++;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsExit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;
    HI_U32 u32DevSta = 0, u32DevEnd = 0;
    ISP_CHECK_DEV(IspDev);

    if(ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
    {
        if(IS_MAIN_CHANNAL(IspDev)) 
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev + 1; 

            if(HI_NULL != g_stStatBufInfo.u32PhyAddr)
            {
                s32Ret = HI_MPI_SYS_MmzFree(g_stStatBufInfo.u32PhyAddr, g_stStatBufInfo.pVirtAddr);
                if (HI_SUCCESS != s32Ret)
                {
                    printf("Mmz Stitch statistics cache failed!\n");
                    return s32Ret;
                }
                g_stStatBufInfo.u32PhyAddr = 0;
                g_stStatBufInfo.pVirtAddr = NULL;
                g_stStatBufInfo.u32Size = 0;     
            }            
        }
        else
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev - 1;
        }
    }    
    else
    {
        u32DevSta = IspDev;
        u32DevEnd = IspDev;
    }

    while(u32DevSta <= u32DevEnd)
    {
        IspDev = u32DevSta;
        STATISTICS_GET_CTX(IspDev, pstStat);
        ISP_CHECK_OPEN(IspDev);       
        
        if (HI_NULL != pstStat->pStatVirtAddr)
        {
            HI_MPI_SYS_Munmap(pstStat->pStatVirtAddr,
                sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);
            pstStat->pStatVirtAddr = HI_NULL;
        }
        
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_EXIT);
        if (HI_SUCCESS != s32Ret)
        {
            printf("exit statistics bufs failed %x!\n", s32Ret);
            return s32Ret;
        }

        u32DevSta++;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsGetBuf(ISP_DEV IspDev, HI_VOID **ppStat)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    STATISTICS_GET_CTX(IspDev, pstStat);
    ISP_CHECK_OPEN(IspDev); 

    if (!pstStat->bRead)
    {
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_GET, &pstStat->stStatInfo);
        if (s32Ret)
        {
            return s32Ret;
        }

        if (HI_NULL != pstStat->pStatVirtAddr)
        {
            pstStat->stStatInfo.pVirtAddr = pstStat->pStatVirtAddr +
                (pstStat->stStatInfo.u32PhyAddr - pstStat->u32StatPhyaddr);
        }
        else
        {        
            pstStat->stStatInfo.pVirtAddr = HI_NULL;
        }
        pstStat->bRead = HI_TRUE;
    }

    if (HI_NULL == pstStat->stStatInfo.pVirtAddr)
    {
        return HI_FAILURE;
    }
    
    *ppStat = pstStat->stStatInfo.pVirtAddr;
    
    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsPutBuf(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;
    HI_U32 u32DevSta = 0, u32DevEnd = 0;
    ISP_CHECK_DEV(IspDev);

    if(ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
    {
        if(IS_MAIN_CHANNAL(IspDev)) 
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev + 1;           
        }
        else
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev - 1;
        }
    }    
    else
    {
        u32DevSta = IspDev;
        u32DevEnd = IspDev;
    }

    while(u32DevSta <= u32DevEnd)
    {
        IspDev = u32DevSta;
        STATISTICS_GET_CTX(IspDev, pstStat);
        ISP_CHECK_OPEN(IspDev);

        pstStat->stStatInfo.unKey.u32Key = hi_ext_system_statistics_ctrl_read(IspDev);

        if (pstStat->bRead)
        {
            s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_PUT, &pstStat->stStatInfo);
            if (s32Ret)
            {
                printf("Release ISP stat buf Failed with ec %#x!\n", s32Ret);
            }
            pstStat->stStatInfo.pVirtAddr = HI_NULL;
            pstStat->bRead = HI_FALSE;
        }

        u32DevSta++;
    }
    
    return HI_SUCCESS;
}

HI_U32  ISP_CalcStatisticsStit(const HI_VOID *pStatIsp0, const HI_VOID *pStatIsp1, HI_VOID **ppstStit)
{
    HI_U32 i, j, k;
    HI_S32 s32Dev0GainDiff[4];
    HI_S32 s32Dev1GainDiff[4];

    if(HI_NULL == g_stStatBufInfo.u32PhyAddr)
    {
        printf("No alloc Stitch statistics buffer!\n");
        return HI_FAILURE;
    }
    
    //TO: AE/AWB/DEHAZE Statistics Stitch
    memcpy(&((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAfStat, &(((ISP_STAT_S *)pStatIsp0)->stAfStat), sizeof(ISP_AF_STAT_S));
    memcpy(&((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stCommStat, &(((ISP_STAT_S *)pStatIsp0)->stCommStat), sizeof(ISP_COMM_STAT_S));
    memcpy(&((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stDpStat, &(((ISP_STAT_S *)pStatIsp0)->stDpStat), sizeof(ISP_DP_STAT_S));

    /* AE global 5 bin histogram */
    for(i = 0; i < 5; i++)
    {
        ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat1.au16MeteringHist[i] = (((ISP_STAT_S *)pStatIsp0)->stAeStat1.au16MeteringHist[i] + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat1.au16MeteringHist[i]) / 2;
    }

    /* AE global 1024 bin histogram */
    for(i = 0; i < 1024; i++)
    {
        ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat3.au32HistogramMemArray[i] = (((ISP_STAT_S *)pStatIsp0)->stAeStat3.au32HistogramMemArray[i] + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat3.au32HistogramMemArray[i]) / 2;
    }
    
    /* AE global avg */
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat4.u16GlobalAvgR = ((HI_U32)((ISP_STAT_S *)pStatIsp0)->stAeStat4.u16GlobalAvgR + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat4.u16GlobalAvgR) / 2;
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat4.u16GlobalAvgGr = ((HI_U32)((ISP_STAT_S *)pStatIsp0)->stAeStat4.u16GlobalAvgGr + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat4.u16GlobalAvgGr) / 2;    
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat4.u16GlobalAvgGb = ((HI_U32)((ISP_STAT_S *)pStatIsp0)->stAeStat4.u16GlobalAvgGb + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat4.u16GlobalAvgGb) / 2;    
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat4.u16GlobalAvgB = ((HI_U32)((ISP_STAT_S *)pStatIsp0)->stAeStat4.u16GlobalAvgB + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat4.u16GlobalAvgB) / 2;

    /* AE 255 zone avg */
    for (i = 0; i < AE_ZONE_ROW; i++)
    {
        for (j = 0; j < AE_ZONE_COLUMN; j++)
        {
            for(k = 0; k < 4; k++)
            {
                if(j < AE_ZONE_COLUMN/2)
                {
                    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat5.au16ZoneAvg[i][j][k] = ((HI_U32)((ISP_STAT_S *)pStatIsp0)->stAeStat5.au16ZoneAvg[i][2*j][k] + \
                        ((ISP_STAT_S *)pStatIsp0)->stAeStat5.au16ZoneAvg[i][2*j+1][k]) / 2;
                }
                else if(j == AE_ZONE_COLUMN/2)
                {
                    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat5.au16ZoneAvg[i][j][k] = ((HI_U32)((ISP_STAT_S *)pStatIsp0)->stAeStat5.au16ZoneAvg[i][AE_ZONE_COLUMN-1][k] + \
                        ((ISP_STAT_S *)pStatIsp1)->stAeStat5.au16ZoneAvg[i][0][k]) / 2;
                }
                else
                {
                    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat5.au16ZoneAvg[i][j][k] = ((HI_U32)((ISP_STAT_S *)pStatIsp1)->stAeStat5.au16ZoneAvg[i][2*(j-AE_ZONE_COLUMN/2)-1][k] + \
                        ((ISP_STAT_S *)pStatIsp1)->stAeStat5.au16ZoneAvg[i][2*(j-AE_ZONE_COLUMN/2)][k]) / 2;
                }
            }
        }
    }

    // AE global 256 bin histogram
    for(i = 0; i < 256; i++)
    {
        ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat6.au32PreHistogram0[i] = (((ISP_STAT_S *)pStatIsp0)->stAeStat6.au32PreHistogram0[i] + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat6.au32PreHistogram0[i]) / 2;
        ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat6.au32PreHistogram1[i] = (((ISP_STAT_S *)pStatIsp0)->stAeStat6.au32PreHistogram1[i] + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat6.au32PreHistogram1[i]) / 2;
        ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat6.au32PreHistogram2[i] = (((ISP_STAT_S *)pStatIsp0)->stAeStat6.au32PreHistogram2[i] + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat6.au32PreHistogram2[i]) / 2;
        ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAeStat6.au32PreHistogram3[i] = (((ISP_STAT_S *)pStatIsp0)->stAeStat6.au32PreHistogram3[i] + \
            ((ISP_STAT_S *)pStatIsp1)->stAeStat6.au32PreHistogram3[i]) / 2;
    }

    /*AWB Bayer Statistic*/
    for (i = 0; i < 4; i++)
    {
        s32Dev0GainDiff[i] = hi_ext_system_isp_pipe_diff_gain_read(0, i);
        s32Dev1GainDiff[i] = hi_ext_system_isp_pipe_diff_gain_read(1, i);
    }
    s32Dev0GainDiff[1] = (s32Dev0GainDiff[1] + s32Dev0GainDiff[2]) >> 1;
    s32Dev1GainDiff[1] = (s32Dev1GainDiff[1] + s32Dev1GainDiff[2]) >> 1;
    s32Dev0GainDiff[2] = s32Dev0GainDiff[1];
    s32Dev1GainDiff[2] = s32Dev1GainDiff[1];
   
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat3.u16MeteringAwbAvgR = (((ISP_STAT_S *)pStatIsp0)->stAwbStat3.u16MeteringAwbAvgR * s32Dev0GainDiff[0] + \
        ((ISP_STAT_S *)pStatIsp1)->stAwbStat3.u16MeteringAwbAvgR * s32Dev1GainDiff[0] + 0x100) >> 9;
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat3.u16MeteringAwbAvgG = (((ISP_STAT_S *)pStatIsp0)->stAwbStat3.u16MeteringAwbAvgG * s32Dev0GainDiff[1] + \
        ((ISP_STAT_S *)pStatIsp1)->stAwbStat3.u16MeteringAwbAvgG * s32Dev1GainDiff[1] + 0x100) >> 9;
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat3.u16MeteringAwbAvgB = (((ISP_STAT_S *)pStatIsp0)->stAwbStat3.u16MeteringAwbAvgB * s32Dev0GainDiff[2] + \
        ((ISP_STAT_S *)pStatIsp1)->stAwbStat3.u16MeteringAwbAvgB * s32Dev1GainDiff[2] + 0x100) >> 9;
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat3.u16MeteringAwbCountAll = (((ISP_STAT_S *)pStatIsp0)->stAwbStat3.u16MeteringAwbCountAll + \
        ((ISP_STAT_S *)pStatIsp1)->stAwbStat3.u16MeteringAwbCountAll) / 2;
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat3.u16MeteringAwbCountMax = (((ISP_STAT_S *)pStatIsp0)->stAwbStat3.u16MeteringAwbCountMax + \
        ((ISP_STAT_S *)pStatIsp1)->stAwbStat3.u16MeteringAwbCountMax) / 2;
    ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat3.u16MeteringAwbCountMin = (((ISP_STAT_S *)pStatIsp0)->stAwbStat3.u16MeteringAwbCountMin + \
        ((ISP_STAT_S *)pStatIsp1)->stAwbStat3.u16MeteringAwbCountMin) / 2;

    for (i = 0; i < AWB_ZONE_ROW_STITCH; i++)
    {
        for (j = 0; j < AWB_ZONE_COLUMN_STITCH / 2; j++)
        {
             ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgR[i * AWB_ZONE_COLUMN_STITCH + j] = \
                 (((ISP_STAT_S *)pStatIsp0)->stAwbStat4.au16MeteringMemArrayAvgR[i * AWB_ZONE_COLUMN_STITCH / 2 + j] * s32Dev0GainDiff[0] + 0x80) >> 8;

              ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgG[i * AWB_ZONE_COLUMN_STITCH + j] = \
                 (((ISP_STAT_S *)pStatIsp0)->stAwbStat4.au16MeteringMemArrayAvgG[i * AWB_ZONE_COLUMN_STITCH / 2 + j] * s32Dev0GainDiff[1] + 0x80) >> 8;
            
              ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgB[i * AWB_ZONE_COLUMN_STITCH + j] = \
                 (((ISP_STAT_S *)pStatIsp0)->stAwbStat4.au16MeteringMemArrayAvgB[i * AWB_ZONE_COLUMN_STITCH / 2 + j] * s32Dev0GainDiff[3] + 0x80) >> 8;

              ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgR[i * AWB_ZONE_COLUMN_STITCH + AWB_ZONE_COLUMN_STITCH / 2 + j] = \
                 (((ISP_STAT_S *)pStatIsp1)->stAwbStat4.au16MeteringMemArrayAvgR[i * AWB_ZONE_COLUMN_STITCH / 2 + j] * s32Dev1GainDiff[0] + 0x80) >> 8;

              ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgG[i * AWB_ZONE_COLUMN_STITCH + AWB_ZONE_COLUMN_STITCH / 2 + j] = \
                 (((ISP_STAT_S *)pStatIsp1)->stAwbStat4.au16MeteringMemArrayAvgG[i * AWB_ZONE_COLUMN_STITCH / 2 + j] * s32Dev1GainDiff[1] + 0x80) >> 8;

              ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgB[i * AWB_ZONE_COLUMN_STITCH + AWB_ZONE_COLUMN_STITCH / 2 + j] = \
                 (((ISP_STAT_S *)pStatIsp1)->stAwbStat4.au16MeteringMemArrayAvgB[i * AWB_ZONE_COLUMN_STITCH / 2 + j] * s32Dev1GainDiff[3] + 0x80) >> 8;
        }

        memcpy(&(((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountAll[i * AWB_ZONE_COLUMN_STITCH]), 
            &(((ISP_STAT_S *)pStatIsp0)->stAwbStat4.au16MeteringMemArrayCountAll[i * AWB_ZONE_COLUMN_STITCH / 2]), sizeof(HI_U16) * AWB_ZONE_COLUMN_STITCH / 2);
        memcpy(&(((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountAll[i * AWB_ZONE_COLUMN_STITCH + AWB_ZONE_COLUMN_STITCH / 2]), 
            &(((ISP_STAT_S *)pStatIsp1)->stAwbStat4.au16MeteringMemArrayCountAll[i * AWB_ZONE_COLUMN_STITCH / 2]), sizeof(HI_U16) * AWB_ZONE_COLUMN_STITCH / 2);

        memcpy(&(((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountMax[i * AWB_ZONE_COLUMN_STITCH]), 
            &(((ISP_STAT_S *)pStatIsp0)->stAwbStat4.au16MeteringMemArrayCountMax[i * AWB_ZONE_COLUMN_STITCH / 2]), sizeof(HI_U16) * AWB_ZONE_COLUMN_STITCH / 2);
        memcpy(&(((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountMax[i * AWB_ZONE_COLUMN_STITCH + AWB_ZONE_COLUMN_STITCH / 2]), 
            &(((ISP_STAT_S *)pStatIsp1)->stAwbStat4.au16MeteringMemArrayCountMax[i * AWB_ZONE_COLUMN_STITCH / 2]), sizeof(HI_U16) * AWB_ZONE_COLUMN_STITCH / 2);
    
        memcpy(&(((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountMin[i * AWB_ZONE_COLUMN_STITCH]), 
            &(((ISP_STAT_S *)pStatIsp0)->stAwbStat4.au16MeteringMemArrayCountMin[i * AWB_ZONE_COLUMN_STITCH / 2]), sizeof(HI_U16) * AWB_ZONE_COLUMN_STITCH / 2);
        memcpy(&(((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountMin[i * AWB_ZONE_COLUMN_STITCH + AWB_ZONE_COLUMN_STITCH / 2]), 
            &(((ISP_STAT_S *)pStatIsp1)->stAwbStat4.au16MeteringMemArrayCountMin[i * AWB_ZONE_COLUMN_STITCH / 2]), sizeof(HI_U16) * AWB_ZONE_COLUMN_STITCH / 2);

    }

    /*Dehaze Statistic*/     
    for(i = 0; i < DEFOG_ZONE_ROW_STITCH; i++)
    {
        for(j = 0; j < DEFOG_ZONE_COL_STITCH/2; j++)
        {
            ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stDehazeStat.au32DehazeMinDout[i*DEFOG_ZONE_COL_STITCH + j] = ((ISP_STAT_S *)pStatIsp0)->stDehazeStat.au32DehazeMinDout[i * DEFOG_ZONE_COL_STITCH / 2 + j];
            ((ISP_STAT_S *)g_stStatBufInfo.pVirtAddr)->stDehazeStat.au32DehazeMinDout[i*DEFOG_ZONE_COL_STITCH + j + DEFOG_ZONE_COL_STITCH / 2] = ((ISP_STAT_S *)pStatIsp1)->stDehazeStat.au32DehazeMinDout[i * DEFOG_ZONE_COL_STITCH / 2 + j];
        }
    }

    *ppstStit = (HI_VOID *)g_stStatBufInfo.pVirtAddr;

    return HI_SUCCESS;
}

#if 0
HI_S32 ISP_StatisticsKeyMap(HI_VOID *pStat, ISP_STAT_KEY_U unKey, HI_VOID **ppKeyStat)
{
    ISP_STAT_S *pstStatInfo = HI_NULL;
    
    pstStatInfo = (ISP_STAT_S *)pStat;
    if (unKey.bit1AeStat1)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAeStat1;
    }
    else if (unKey.bit1AeStat2)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAeStat2;
    }
    else if (unKey.bit1AeStat3)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAeStat3;
    }
    else if (unKey.bit1AwbStat1)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAwbStat1;
    }
    else if (unKey.bit1AwbStat2)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAwbStat2;
    }
    else if (unKey.bit1AfStat)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAfStat;
    }
    else if (unKey.bit1CommStat)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stCommStat;
    }
    else if (unKey.bit1DpStat)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stDpStat;
    }
    else
    {
        *ppKeyStat = pStat;
    }

    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

