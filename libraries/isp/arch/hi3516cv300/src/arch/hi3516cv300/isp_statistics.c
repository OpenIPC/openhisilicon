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
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_statistics.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_metering_mem_config.h"
#include <sys/time.h>
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

    HI_U32          u32StatInfoPhyaddr[2];
    HI_VOID        *pStatInfoVirtAddr[2];
    HI_BOOL         bFinish;

    HI_BOOL         bRead;
    //HI_BOOL         bFinish;
    //HI_BOOL         bReady;
    ISP_STAT_INFO_S stStatInfo;
} ISP_STA_S;

ISP_STA_S g_astStatCtx[ISP_MAX_DEV_NUM] = {{0}};
ISP_STA_S g_shadowmemStat[ISP_MAX_DEV_NUM] = {{0}};
#define STATISTICS_GET_CTX(dev, pstCtx)   pstCtx = &g_astStatCtx[dev]
#define SHADOW_STATISTICS_GET_CTX(dev, pstCtx)   pstCtx = &g_shadowmemStat[dev]

extern HI_S32 g_as32IspFd[ISP_MAX_DEV_NUM];

HI_S32 ISP_StatisticsInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;
    HI_U32 u32Phyaddr;
    HI_VOID* pVirtAddr;
    //ISP_STA_S *pstShadowStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);
    //SHADOW_STATISTICS_GET_CTX(IspDev, pstShadowStat);

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
        if(HI_SUCCESS != s32Ret)
        {
          printf("Isp statistics bufs exit failed!\n");
        }
        return HI_ERR_ISP_NOMEM;
    }

    s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&u32Phyaddr, (void**)&pVirtAddr, "isp_stat_user", HI_NULL, sizeof(ISP_STAT_S)*2);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Isp statistics bufs malloc failed!\n");
        HI_MPI_SYS_Munmap(pstStat->pStatVirtAddr,
            sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);
        pstStat->pStatVirtAddr = HI_NULL;
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_EXIT);
        if(HI_SUCCESS != s32Ret)
        {
          printf("Isp statistics bufs exit failed!\n");
        }
        return HI_ERR_ISP_NOMEM;
    }
    pstStat->u32StatInfoPhyaddr[0] = u32Phyaddr;
    pstStat->u32StatInfoPhyaddr[1] = u32Phyaddr + sizeof(ISP_STAT_S);
    pstStat->pStatInfoVirtAddr[0] = pVirtAddr;
    pstStat->pStatInfoVirtAddr[1] = (HI_VOID *)((unsigned long)pVirtAddr + sizeof(ISP_STAT_S));
    pstStat->bFinish = 0;

    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsExit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

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

    HI_MPI_SYS_MmzFree(pstStat->u32StatInfoPhyaddr[0], pstStat->pStatInfoVirtAddr[0]);

    return HI_SUCCESS;
}

HI_U32 trans_stat_info(HI_VOID **ppSrcVirAddr, HI_VOID **ppDestVirAddr)
{
    ISP_STAT_S* ptemp_stat_info = *ppDestVirAddr;
    ISP_CTX_S       *pstIspCtx   = HI_NULL;
    HI_U32 IspDev = 0;
    ISP_GET_CTX(IspDev, pstIspCtx);

    HI_S32 i, j;

    HI_U32 *pAWBVirAddr = NULL;
    HI_U32 *pAE3VirAddr = NULL;
    HI_U32 *pAE5VirAddr1 = NULL;
    HI_U32 *pAE5VirAddr2 = NULL;
    HI_U32 *pAE6VirAddr0 = NULL;
    HI_U32 *pAE6VirAddr1 = NULL;
    HI_U32 *pAE6VirAddr2 = NULL;
    HI_U32 *pAE6VirAddr3 = NULL;
    HI_U32 *pMGVirAddr = NULL;
    HI_U32 *pAFVirAddr = NULL;

    ISP_AE_STAT_5_S* pstAeStat5;
    ISP_AE_STAT_6_S* pstAeStat6;
    ISP_AF_STAT_S  * pstAfStat;
	ISP_STAT_KEY_U unStatkey;



	unStatkey.u32Key = hi_ext_system_statistics_ctrl_read();

    pAE3VirAddr = (HI_U32 *)(*ppSrcVirAddr);
    pAWBVirAddr = pAE3VirAddr + 1024;
    pAE5VirAddr1 = pAWBVirAddr + 2048;
    pAE5VirAddr2 = pAE5VirAddr1 + 256;
    pAE6VirAddr0 = pAE5VirAddr2 + 256;
    pAE6VirAddr1 = pAE6VirAddr0 + 256;
    pAE6VirAddr2 = pAE6VirAddr1 + 256;
    pAE6VirAddr3 = pAE6VirAddr2 + 256;
    pMGVirAddr = pAE6VirAddr3 + 256;
    pAFVirAddr = pMGVirAddr + 256;

    pstAeStat5 = &ptemp_stat_info->stAeStat5;
    pstAeStat6 = &ptemp_stat_info->stAeStat6;
    pstAfStat  = &ptemp_stat_info->stAfStat;


    if (HI_NULL == ptemp_stat_info)
    {
        return HI_FAILURE;
    }

	pstIspCtx->u8AWBRun = 0;

//if(pstIspCtx->u32FrameCnt % 2)
{
	if (unStatkey.bit1AwbStat3)
	{
		ptemp_stat_info->stAwbStat3.u16MeteringAwbAvgR = hi_isp_metering_avg_r_read(IspDev);
		ptemp_stat_info->stAwbStat3.u16MeteringAwbAvgG = hi_isp_metering_avg_g_read(IspDev);
		ptemp_stat_info->stAwbStat3.u16MeteringAwbAvgB = hi_isp_metering_avg_b_read(IspDev);
		ptemp_stat_info->stAwbStat3.u16MeteringAwbCountAll = hi_isp_metering_count_all_read(IspDev);

	}
	if(unStatkey.bit1AwbStat4)
	{
		for (i=0; i<AWB_ZONE_COLUMN*AWB_ZONE_ROW; i++)
		{
			HI_U32 tmp1,tmp2,j;
			j = i * 2;
			tmp1 = pAWBVirAddr[j];
			tmp2 = pAWBVirAddr[j + 1];
			ptemp_stat_info->stAwbStat4.au16MeteringMemArrayAvgR[i] = tmp1 & 0xffff;
			ptemp_stat_info->stAwbStat4.au16MeteringMemArrayAvgG[i] = (tmp1 >> 16) & 0xffff;
			ptemp_stat_info->stAwbStat4.au16MeteringMemArrayAvgB[i] = tmp2 & 0xffff;
			ptemp_stat_info->stAwbStat4.au16MeteringMemArrayCountAll[i] = (tmp2 >> 16) & 0xffff;
			ptemp_stat_info->stAwbStat4.au16MeteringMemArrayCountMin[i] = 0;
			ptemp_stat_info->stAwbStat4.au16MeteringMemArrayCountMax[i] = 0;

		}
	}
	pstIspCtx->u8AWBRun = 1;
}

#if 1
	if (unStatkey.bit1AfStat)
	{
        HI_U32 u32Zone = hi_area_block_distribute_data_read_af();
        HI_U8 u8Col = (u32Zone & 0x1F);
        HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);
        if( u8Col > AF_ZONE_COLUMN )
        {
          u8Col = AF_ZONE_COLUMN;
        }
        if(u8Row > AF_ZONE_ROW)
        {
          u8Row = AF_ZONE_ROW;
        }

        for (i = 0; i < u8Row; i++)
        {
            for (j = 0; j < u8Col; j++)
            {

                HI_U32 tmp1,tmp2,tmp3,k;
                k=(i*u8Col+j)*4;

                tmp1 = pAFVirAddr[k];
                tmp2 = pAFVirAddr[k+1];
                tmp3 = pAFVirAddr[k+2];

                pstAfStat->stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & tmp1) >> 16);
                pstAfStat->stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & tmp1);
                pstAfStat->stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & tmp2) >> 16);
                pstAfStat->stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & tmp2);
                pstAfStat->stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & tmp3);
                pstAfStat->stZoneMetrics[i][j].u16HlCnt = (HI_U16)((0xFFFF0000 & tmp3) >> 16);

            }
        }


    }
#endif

  if (unStatkey.bit1CommStat)
    {
        ptemp_stat_info->stCommStat.au16WhiteBalanceGain[0] = hi_isp_white_balance_gain_r_read(IspDev);
        ptemp_stat_info->stCommStat.au16WhiteBalanceGain[1] = hi_isp_white_balance_gain_gr_read(IspDev);
        ptemp_stat_info->stCommStat.au16WhiteBalanceGain[2] = hi_isp_white_balance_gain_gb_read(IspDev);
        ptemp_stat_info->stCommStat.au16WhiteBalanceGain[3] = hi_isp_white_balance_gain_b_read(IspDev);

    }



	if(unStatkey.bit1AeStat3)
    {
        for(i = 0; i < 1024; i++)
        {
           ptemp_stat_info->stAeStat3.au32HistogramMemArray[i] = *((HI_U32 *)pAE3VirAddr+i);
        }

        ptemp_stat_info->stAeStat3.u32PixelCount  = hi_isp_ae_pixel_selected_count_read(IspDev);
        ptemp_stat_info->stAeStat3.u32PixelWeight = hi_isp_ae_pixel_selected_weight_read(IspDev);
    }


	if (unStatkey.bit1AeStat4)
    {
        ptemp_stat_info->stAeStat4.u16GlobalAvgR  = hi_isp_ae_total_r_aver_read(IspDev);
        ptemp_stat_info->stAeStat4.u16GlobalAvgGr = hi_isp_ae_total_gr_aver_read(IspDev);
        ptemp_stat_info->stAeStat4.u16GlobalAvgGb = hi_isp_ae_total_gb_aver_read(IspDev);
        ptemp_stat_info->stAeStat4.u16GlobalAvgB  = hi_isp_ae_total_b_aver_read(IspDev);

        //printf("stAeStat4  =%u   %u   %u \r\n",ptemp_stat_info->stAeStat4.u16GlobalAvgR,ptemp_stat_info->stAeStat4.u16GlobalAvgGr, ptemp_stat_info->stAeStat4.u16GlobalAvgB);
    }

	if (unStatkey.bit1AeStat5)
	{
		for(i = 0;i < AE_ZONE_ROW ; i++)
		{
			for(j=0;j< AE_ZONE_COLUMN ;j++)
			{
				HI_U32 tmp1,tmp2;
				tmp1 = pAE5VirAddr1[i * AE_ZONE_COLUMN+j];
				tmp2 = pAE5VirAddr2[i * AE_ZONE_COLUMN+j];

				pstAeStat5->au16ZoneAvg[i][j][0] = (HI_U16)((tmp1 & 0xffff0000) >> 16);
				pstAeStat5->au16ZoneAvg[i][j][1] = (HI_U16)((tmp1 & 0xffff));

				pstAeStat5->au16ZoneAvg[i][j][2] = (HI_U16)((tmp2 & 0xffff0000) >> 16);
				pstAeStat5->au16ZoneAvg[i][j][3] = (HI_U16)((tmp2 & 0xffff));
			}
		}
	}

	if(unStatkey.bit1AeStat6)
	{
		for(i = 0; i < 256; i++)
		{
			pstAeStat6->au32PreHistogram1[i] =	*((HI_U32 *)pAE6VirAddr0+i);
			pstAeStat6->au32PreHistogram2[i] = *((HI_U32 *)pAE6VirAddr1+i);
			pstAeStat6->au32PreHistogram3[i] = *((HI_U32 *)pAE6VirAddr2+i);
			pstAeStat6->au32PreHistogram4[i] = *((HI_U32 *)pAE6VirAddr3+i);
		}
	}

	if(unStatkey.bit1Dehaze)
	{

		j = DEFOG_ZONE_NUM / 2;
		hi_isp_dehaze_minstat_raddr_write(IspDev, 0);
		for(i = 0; i < j; i++)
		{
		  //pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_vi_dehaze_min_stat_read(i);
		  ptemp_stat_info->stDehazeStat.au32DehazeMinDout[i] = hi_isp_dehaze_minstat_rdata_read(IspDev);
		}

	}

	if(unStatkey.bit1MgStat)
	{
		for(i = 0;i < MG_ZONE_ROW ; i++)
		{
			for(j=0;j< MG_ZONE_COLUMN ;j++)
			{
				HI_U32 tmp;

				tmp = pMGVirAddr[i*MG_ZONE_ROW+j];
				ptemp_stat_info->stMgStat.au16ZoneAvg[i][j][0] = (HI_U16)((tmp & 0xff000000) >> 24);
				ptemp_stat_info->stMgStat.au16ZoneAvg[i][j][1] = (HI_U16)((tmp & 0xff0000) >> 16);
				ptemp_stat_info->stMgStat.au16ZoneAvg[i][j][2] = (HI_U16)((tmp & 0xff00) >> 8);
				ptemp_stat_info->stMgStat.au16ZoneAvg[i][j][3] = (HI_U16)(tmp & 0xff);
			}
		}
	}
    return HI_SUCCESS;

}


HI_S32 ISP_StatisticsGetBuf(ISP_DEV IspDev, HI_VOID **ppStat)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

    if (!pstStat->bRead)
    {
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_GET, &pstStat->stStatInfo);
        if (s32Ret)
        {
            return s32Ret;
        }

        if (HI_NULL != pstStat->pStatVirtAddr)
        {
            pstStat->stStatInfo.pVirtAddr = (HI_VOID *)((unsigned long)pstStat->pStatVirtAddr +
                (pstStat->stStatInfo.u32PhyAddr - pstStat->u32StatPhyaddr));
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

    trans_stat_info(&pstStat->stStatInfo.pVirtAddr, &pstStat->pStatInfoVirtAddr[pstStat->bFinish]);

    hi_ext_isp_stat_info_phyaddr_write(pstStat->u32StatInfoPhyaddr[pstStat->bFinish]);
    *ppStat = pstStat->pStatInfoVirtAddr[pstStat->bFinish];
    pstStat->bFinish = 1 - pstStat->bFinish;

    //invalid cache
    HI_MPI_SYS_MflushCache(pstStat->stStatInfo.u32PhyAddr, pstStat->stStatInfo.pVirtAddr, sizeof(ISP_STAT_S));

    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsPutBuf(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

    pstStat->stStatInfo.unKey.u32Key = hi_ext_system_statistics_ctrl_read();

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

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

