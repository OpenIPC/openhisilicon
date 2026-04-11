/******************************************************************************

  Copyright (C), 20015-2025, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_flicker.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/12/09
  Description   : 
  History       :
  1.Date        : 2015/12/09
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_FLICKER_GROUP (16)
#define HI_ISP_FLICKER_MINBANDNUM (3)
#define HI_ISP_FLICKER_MINVALIDBANDPCNT (6)
#define HI_ISP_FLICKER_WAVEDIFF1 (3)
#define HI_ISP_FLICKER_WAVEDIFF2 (4)
#define HI_ISP_FLICKER_PERIOD (3)
#define HI_ISP_FLICKER_GRCNT (5)
#define HI_ISP_FLICKER_GBCNT (5)
#define HI_ISP_FLICKER_OVERTH (50)

typedef struct hiHI_ISP_FLICKER_EXTERN_REG_S
{
	HI_U8 	u8MinBandNum;
	HI_U8 	u8MinValidBandPcnt;
	HI_U8 	u8WaveDiff1;
	HI_U8 	u8WaveDiff2;
	HI_U8 	u8Period;
	HI_U8 	u8GrCnt;
	HI_U8 	u8GbCnt;	
	HI_S32	s32GbAvgcnt;
	HI_S32	s32GrAvgcnt;
}ISP_FLICKER_EXTERN_REG_S;


typedef struct hiHI_ISP_FLICKER_POINT_ATTR
{
	/*Alloc Mem when Initial*/
    HI_S32 *ps32GrMean;
    HI_S32 *ps32GbMean;
    HI_S32 *ps32GrCros0;
    HI_S32 *ps32GbCros0;
    HI_S32 *ps32GrCnt;
    HI_S32 *ps32GbCnt;
}ISP_FLICKER_POINT_ATTR;

typedef struct hiHI_ISP_FLICKER_STAT_INFO
{
	// flicker reg
	HI_S32 s32CurFrameDiffSumGr; //s24.0, current Frame Gr Sum
	HI_S32 s32CurFrameDiffSumGb; //s24.0, current Frame Gb Sum	  
	//WDR reg	
	HI_U32 u32CurFrameAbsSumGr;  //u23.0
	HI_U32 u32CurFrameAbsSumGb;  //u23.0
	HI_S32 *ps32GrMean;    //s11.0
	HI_S32 *ps32GbMean;	//s11.0
}ISP_FLICKER_STAT_INFO;

typedef struct hiHI_ISP_FLICKER_ATTR
{
	HI_BOOL bEnable;
	HI_BOOL bMalloc;
	HI_U16 u16FckInx;
	HI_U16 u16Height;
	HI_U32 u32FrameNum;
	HI_S32 s32PreFrameAvgGr;  //s15.0, previous Frame Gr Avg 
	HI_S32 s32PreFrameAvgGb;  //s15.0, previous Frame Gb Avg	
	HI_S32 s32CurFrameAvgGr;  //s15.0, previous Frame Gr Avg 
	HI_S32 s32CurFrameAvgGb;  //s15.0, previous Frame Gb Avg	
	HI_U32 u32CurFrameVarGr;
	HI_U32 u32CurFrameVarGb;

	HI_S32 s32CurFlicker;                                  //u1.0,  current frame flicker,1 is flicker,0 is no flicker
	HI_S32 s32RetFlickResult;                              //u1.0,  the result,1 is flicker,0 is no flicker 	
	HI_S32 s32CurFreq;                                    //u1.0,  current frame flicker ,1 is 50Hz,0 is 60Hz
	HI_S32 s32PreFreq;
	HI_S32 s32RetFreqResult; 
	HI_S32 s32FlickerHist;
	HI_S32 s32FreqHist;
	
	ISP_FLICKER_EXTERN_REG_S stFckExtReg;
	ISP_FLICKER_POINT_ATTR   stFckPoint;
	ISP_FLICKER_STAT_INFO    stFckStatInfo;
}ISP_FLICKER_ATTR;

ISP_FLICKER_ATTR g_astFlickerCtx[ISP_MAX_DEV_NUM];

#define FLICKER_GET_CTX(dev, pstCtx)   pstCtx = &g_astFlickerCtx[dev]

static HI_VOID FlickerFreeMem(ISP_DEV IspDev)
{
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;    
    FLICKER_GET_CTX(IspDev, pstFlickerCtx);

	if (HI_TRUE == pstFlickerCtx->bMalloc && pstFlickerCtx->stFckPoint.ps32GrMean)
	{
		free(pstFlickerCtx->stFckPoint.ps32GrMean);
		pstFlickerCtx->stFckPoint.ps32GrMean = HI_NULL;
		pstFlickerCtx->bMalloc = HI_FALSE;
	}
}


static HI_VOID FlickerExtRegsInitialize(HI_VOID)
{
	hi_ext_system_flicker_result_write(0);
	hi_ext_system_freq_result_write(0x2);
	hi_ext_system_flicker_min_band_num_write(HI_ISP_FLICKER_MINBANDNUM);
	hi_ext_system_flicker_min_valid_band_pcnt_write(HI_ISP_FLICKER_MINVALIDBANDPCNT);
	hi_ext_system_flicker_wave_diff1_write(HI_ISP_FLICKER_WAVEDIFF1);
	hi_ext_system_flicker_wave_diff2_write(HI_ISP_FLICKER_WAVEDIFF2);
	hi_ext_system_flicker_period_write(HI_ISP_FLICKER_PERIOD);
	hi_ext_system_flicker_gr_cnt_write(HI_ISP_FLICKER_GRCNT);
	hi_ext_system_flicker_gb_cnt_write(HI_ISP_FLICKER_GBCNT);	
}

static HI_VOID FlickerRegsInitialize(ISP_DEV IspDev)
{
	HI_U8  u8MemCnt;
	HI_U8 u8WDRMode;
	HI_U32 u32MemSize;
	HI_S32 *pS32Mem = HI_NULL;
	HI_U16 u16Width, u16Height;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;    
    	FLICKER_GET_CTX(IspDev, pstFlickerCtx);
	
    	ISP_GET_CTX(IspDev, pstIspCtx);	
	u8WDRMode = pstIspCtx->u8SnsWDRMode;
	pstFlickerCtx->u32FrameNum = 0;

	u16Width		 = hi_ext_sync_total_width_read();
	u16Height	 = hi_ext_sync_total_height_read();

	hi_isp_flick_width_write(IspDev, u16Width-1);
	hi_isp_flick_height_write(IspDev, u16Height-1);
	hi_isp_flick_over_th_write(IspDev, HI_ISP_FLICKER_OVERTH);
	pstFlickerCtx->u16Height = u16Height;
	pstFlickerCtx->u16FckInx = u16Height >> 4;

	pstFlickerCtx->s32PreFrameAvgGb = 0;
	pstFlickerCtx->s32PreFrameAvgGr = 0;
	
	if (IS_LINEAR_MODE(u8WDRMode))
	{
		pstFlickerCtx->bEnable= HI_FALSE;
		return;
	}
		
	if (pstFlickerCtx->bMalloc)
	{
		return;
	}
	
	u32MemSize = (u16Height/HI_ISP_FLICKER_GROUP)*sizeof(HI_S32);
	u8MemCnt   = sizeof(ISP_FLICKER_POINT_ATTR)/sizeof(HI_S32);
    if(0 == u32MemSize)
    {
       return;
	}
	pS32Mem = (HI_S32*)malloc(u32MemSize*u8MemCnt);
	if (HI_NULL == pS32Mem)
	{
		printf("Alloc Mem for Flicker Failure!\n");
		pstFlickerCtx->bEnable = HI_FALSE;
		pstFlickerCtx->bMalloc = HI_FALSE;
		pstFlickerCtx->stFckPoint.ps32GrMean = HI_NULL;
		return;
	}
	
	pstFlickerCtx->stFckPoint.ps32GrMean = pS32Mem;
	pstFlickerCtx->stFckPoint.ps32GbMean = (HI_S32*)((HI_U32)pstFlickerCtx->stFckPoint.ps32GrMean + u32MemSize);
	pstFlickerCtx->stFckPoint.ps32GrCros0 = (HI_S32*)((HI_U32)pstFlickerCtx->stFckPoint.ps32GbMean + u32MemSize);
	pstFlickerCtx->stFckPoint.ps32GbCros0 = (HI_S32*)((HI_U32)pstFlickerCtx->stFckPoint.ps32GrCros0 + u32MemSize);
	pstFlickerCtx->stFckPoint.ps32GrCnt = (HI_S32*)((HI_U32)pstFlickerCtx->stFckPoint.ps32GbCros0 + u32MemSize);
	pstFlickerCtx->stFckPoint.ps32GbCnt = (HI_S32*)((HI_U32)pstFlickerCtx->stFckPoint.ps32GrCnt + u32MemSize);
	
    	memset(pstFlickerCtx->stFckPoint.ps32GrMean,0,u32MemSize);
    	memset(pstFlickerCtx->stFckPoint.ps32GbMean,0,u32MemSize);
    	memset(pstFlickerCtx->stFckPoint.ps32GrCros0,0,u32MemSize);
    	memset(pstFlickerCtx->stFckPoint.ps32GbCros0,0,u32MemSize);
    	memset(pstFlickerCtx->stFckPoint.ps32GrCnt,-1,u32MemSize);
    	memset(pstFlickerCtx->stFckPoint.ps32GbCnt,-1,u32MemSize);
	
    	pstFlickerCtx->stFckStatInfo.ps32GrMean = pstFlickerCtx->stFckPoint.ps32GrMean;
    	pstFlickerCtx->stFckStatInfo.ps32GbMean = pstFlickerCtx->stFckPoint.ps32GbMean;
	
	pstFlickerCtx->bMalloc = HI_TRUE;
	pstFlickerCtx->bEnable= HI_TRUE;
}

static HI_VOID FlickerReadHWStatInfo(ISP_DEV IspDev)
{
	HI_U32 i;
	HI_S32 s32Val, s32Tmp;
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;    
    	FLICKER_GET_CTX(IspDev, pstFlickerCtx);	
	
	/***HW statout to FW*************/	
	s32Val = hi_isp_flick_cur_sum_gr_read(IspDev);//Diff	
	s32Val = (s32Val&0x400000)?(0xff800000|s32Val):s32Val;	
	pstFlickerCtx->stFckStatInfo.s32CurFrameDiffSumGr = s32Val;
	
	s32Val = hi_isp_flick_cur_sum_gb_read(IspDev);//Diff
	s32Val = (s32Val&0x400000)?(0xff800000|s32Val):s32Val;
	pstFlickerCtx->stFckStatInfo.s32CurFrameDiffSumGb = s32Val;

	pstFlickerCtx->stFckStatInfo.u32CurFrameAbsSumGr = hi_isp_flick_cur_abs_gr_read(IspDev);//ABS
	pstFlickerCtx->stFckStatInfo.u32CurFrameAbsSumGb = hi_isp_flick_cur_abs_gb_read(IspDev);//ABS
	
	pstFlickerCtx->s32CurFrameAvgGr = (2 * pstFlickerCtx->stFckStatInfo.s32CurFrameDiffSumGr)/(HI_S32)DIV_0_TO_1(pstFlickerCtx->u16Height);
	pstFlickerCtx->s32CurFrameAvgGb = (2 * pstFlickerCtx->stFckStatInfo.s32CurFrameDiffSumGb)/(HI_S32)DIV_0_TO_1(pstFlickerCtx->u16Height);

	hi_isp_flick_gmean_raddr_write(IspDev, 0);
	for(i = 0; i < pstFlickerCtx->u16FckInx ; i++)
	{
		s32Val = (HI_S32)hi_isp_flick_mean_rdata_read(IspDev);
		s32Tmp = ((s32Val&0x3FF800)>>11);
		s32Tmp = (s32Tmp&0x400)?(s32Tmp|0xfffff800):s32Tmp;
		pstFlickerCtx->stFckStatInfo.ps32GrMean[i] = s32Tmp;

		s32Tmp = s32Val&0X7FF;
		s32Tmp = (s32Tmp&0x400)?(s32Tmp|0xfffff800):s32Tmp;
		pstFlickerCtx->stFckStatInfo.ps32GbMean[i] = s32Tmp;		
	}	

	pstFlickerCtx->u32CurFrameVarGr  = (2 * pstFlickerCtx->stFckStatInfo.u32CurFrameAbsSumGr)/DIV_0_TO_1(pstFlickerCtx->u16Height);
	pstFlickerCtx->u32CurFrameVarGb  = (2 * pstFlickerCtx->stFckStatInfo.u32CurFrameAbsSumGb)/DIV_0_TO_1(pstFlickerCtx->u16Height);	
	/************end ***************/
}

static HI_VOID FlickerDetectResult(ISP_DEV IspDev)
{
	
	ISP_FLICKER_EXTERN_REG_S *pstExtReg = HI_NULL;
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;  
	
    	FLICKER_GET_CTX(IspDev, pstFlickerCtx);	
	pstExtReg = &pstFlickerCtx->stFckExtReg;	
	HI_S32	s32GbAvgcnt	=	pstExtReg->s32GbAvgcnt;
	HI_S32	s32GrAvgcnt	=	pstExtReg->s32GrAvgcnt;

	pstFlickerCtx->s32PreFrameAvgGr = pstFlickerCtx->s32CurFrameAvgGr;
	pstFlickerCtx->s32PreFrameAvgGb = pstFlickerCtx->s32CurFrameAvgGb;
		/***********flicker result***********/
	
	if(pstFlickerCtx->u32FrameNum == 1)
	{
		if(pstFlickerCtx->s32CurFlicker == 1)
		{
			pstFlickerCtx->s32RetFlickResult = 1;
		}
		else
		{
			pstFlickerCtx->s32RetFlickResult = 0;
		}
		pstFlickerCtx->s32FlickerHist = 0;
	}
	else
	{
		if(pstFlickerCtx->s32FlickerHist == pstExtReg->u8Period)
		{
			pstFlickerCtx->s32RetFlickResult = !(pstFlickerCtx->s32RetFlickResult);
			pstFlickerCtx->s32FlickerHist = 0;
		}
		if(pstFlickerCtx->s32CurFlicker == pstFlickerCtx->s32RetFlickResult)
		{
			pstFlickerCtx->s32FlickerHist = 0;
		}
		else
		{
			pstFlickerCtx->s32FlickerHist++;
		}
	}
	/************50Hz/60Hz******************/	
	if(pstFlickerCtx->s32CurFlicker == 1)
	{
		if((s32GrAvgcnt <= pstExtReg->u8GrCnt)&&(s32GbAvgcnt <= pstExtReg->u8GbCnt))
		{
			pstFlickerCtx->s32CurFreq = 1;//50Hz
		}
		else
		{
			pstFlickerCtx->s32CurFreq = 0;//60Hz
		}
	}
	else
	{
		pstFlickerCtx->s32CurFreq     = 2;//light off
	}

	if(pstFlickerCtx->u32FrameNum == 1)
	{
		if(pstFlickerCtx->s32CurFreq == 1)
		{
			pstFlickerCtx->s32RetFreqResult = 1;
		}
		else if(pstFlickerCtx->s32CurFreq == 0)
		{
			pstFlickerCtx->s32RetFreqResult = 0;
		}	
		else
		{
			pstFlickerCtx->s32RetFreqResult = 2;
		}
		pstFlickerCtx->s32FreqHist = 0;
	}
	else
	{
		if(pstFlickerCtx->s32FreqHist == pstExtReg->u8Period)
		{
			pstFlickerCtx->s32RetFreqResult = pstFlickerCtx->s32PreFreq;
			pstFlickerCtx->s32FreqHist = 0;
		}
		if(pstFlickerCtx->s32CurFreq == pstFlickerCtx->s32RetFreqResult)
		{
			pstFlickerCtx->s32FreqHist = 0;
		}
		else
		{
			pstFlickerCtx->s32FreqHist++;
		}
	}
	
	pstFlickerCtx->s32PreFreq = pstFlickerCtx->s32CurFreq;
}

static HI_VOID FlickerDetect(ISP_DEV IspDev)
{
	HI_U16 u16Inx;
	HI_U32 i, j;
	HI_S32 s32Cnt, s32Grindx, s32Gbindx, s32Tmp1, s32Tmp2;
	HI_S32 *pS32GrCros0, *pS32GbCros0, *pS32GrCnt, *pS32GbCnt;	
	HI_S32 s32GrDiff3cnt, s32GbDiff3cnt, s32GrAvgcnt, s32GbAvgcnt;
	HI_S32 s32GrAvg, s32GbAvg, s32GrPat, s32GbPat, s32GrflickerThd, s32GbflickerThd;

	ISP_FLICKER_EXTERN_REG_S *pstExtReg = HI_NULL;
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;  
	
    	FLICKER_GET_CTX(IspDev, pstFlickerCtx);	
	pstExtReg = &pstFlickerCtx->stFckExtReg;
		
	u16Inx = pstFlickerCtx->u16FckInx;
		
	pS32GrCros0 = pstFlickerCtx->stFckPoint.ps32GrCros0;
	pS32GbCros0 = pstFlickerCtx->stFckPoint.ps32GbCros0;

	pS32GrCnt   = pstFlickerCtx->stFckPoint.ps32GrCnt;
	pS32GbCnt   = pstFlickerCtx->stFckPoint.ps32GbCnt;

	/* previous frame-based mean is the zero-value axis.                */
	/* if group mean value is above this zero-value, it is marked as 1; */
	/* otherwise, it is marked as 0.                                    */
	for (i = 0; i < u16Inx; i++) 
	{
		if(pstFlickerCtx->stFckStatInfo.ps32GrMean[i] >= pstFlickerCtx->s32PreFrameAvgGr)
		{
			*(pS32GrCros0 + i) = 1;
		}
		else
		{
		       *(pS32GrCros0 + i) = 0;
		}

		if(pstFlickerCtx->stFckStatInfo.ps32GbMean[i] >= pstFlickerCtx->s32PreFrameAvgGb)
		{
			*(pS32GbCros0 + i) = 1;
		}
		else
		{
		       *(pS32GbCros0 + i) = 0;
		}
	}

	/* pGr_cnt, pGb_cnt: the width of each wave                              */
	/* pGr_avg, pGb_avg: the average width of wave                           */
	/* Gr_pat, Gb_pat: the total number of valid wave                        */
	/* Gr/Gbflicker_thd: min. number of valid wave to indicate flickering    */
	/* Comparing the wave's avg. width with each wave's width,               */
	/* if the difference is less than 2, this wave is valid flicker wave.    */
	/* We allow one time of difference being 3 (from the video we have had.) */
	s32Cnt    = 1;			
	s32Grindx  = 0;
	j      = 1;
	for(i = 0; i < u16Inx; ) 
	{
		s32Tmp1 = *(pS32GrCros0 + i);
		for ( ; j < u16Inx; )	
		{
			s32Tmp2 = *(pS32GrCros0 + j);
			if (s32Tmp1 == s32Tmp2) 
			{				
				s32Cnt++;  
				j++;
			}
			else
			{
				break;
			}
		}
		
		if (i == u16Inx - 1)
		{				
			*(pS32GrCnt + s32Grindx) = s32Cnt;
			break;
		}
		else if (j == u16Inx)	
		{
			*(pS32GrCnt + s32Grindx) = s32Cnt;
			i = u16Inx;
		}
		else 
		{
			*(pS32GrCnt + s32Grindx) = s32Cnt;
			s32Cnt = 1;				
			i   = j;
			j++;				
			s32Grindx++;
		}			
	}
	s32Cnt     = 1;			
	s32Gbindx  = 0;
	j       = 1;
	for(i = 0; i < u16Inx; ) 
	{
		s32Tmp1 = *(pS32GbCros0 + i);
		for ( ; j < u16Inx; )	
		{
			s32Tmp2 = *(pS32GbCros0 + j);
			if (s32Tmp1 == s32Tmp2) 
			{					
				s32Cnt++;  
				j++;
			}
			else
			{
				break;
			}
		}
		if (i == u16Inx - 1)
		{				
			*(pS32GbCnt + s32Gbindx) = s32Cnt;
			break;
		}
		else if (j == u16Inx)	
		{				
			*(pS32GbCnt + s32Gbindx) = s32Cnt;
			i = u16Inx;
		}
		else 
		{
			*(pS32GbCnt + s32Gbindx) = s32Cnt;
			s32Cnt = 1;			
			i   = j;
			j++;
			s32Gbindx++;
		}			
	}	
	/**********Gr avg************/
	//remove first and last wave
	s32GrDiff3cnt = s32GbDiff3cnt = 0;
	s32GrAvgcnt   = s32GbAvgcnt   = 0;
	if (s32Grindx  > pstExtReg->u8MinBandNum && s32Gbindx > pstExtReg->u8MinBandNum)	
	{
		s32GrAvg = 0;
		for (i = 1; i < s32Grindx ; i++)
		{
			if ( *(pS32GrCnt + i) > 2 ) 
			{
				s32GrAvg += *(pS32GrCnt + i);
				s32GrAvgcnt++;
			}
		}
		if (s32GrAvgcnt)
		{
			s32GrAvg = ( s32GrAvg + (s32GrAvgcnt >> 1) ) / s32GrAvgcnt;
		}
		else
		{
			s32GrAvg = -1;
		}				
		/********* Gb avg *******************/
		s32GbAvg = 0;
		//for (i = 0; i < s32Gbindx + 1; i++)
		for (i = 1; i < s32Gbindx; i++)
		{
			if ( *(pS32GbCnt + i) > 2 )
			{
				s32GbAvg += *(pS32GbCnt + i);
				s32GbAvgcnt++;
			}
		}
		if (s32GbAvgcnt)
		{
			s32GbAvg = ( s32GbAvg + (s32GbAvgcnt >> 1) ) / s32GbAvgcnt;
		}
		else
		{
			s32GbAvg = -1;
		}
		/**************end ****************/					
		s32GrPat = 0;
		for (i = 1; i < s32Grindx  ; i++) 
		{
			if ( abs(s32GrAvg - *(pS32GrCnt + i)) <= pstExtReg->u8WaveDiff1)
			{
				s32GrPat++;
			}
			else if ( abs(s32GrAvg - *(pS32GrCnt + i)) == pstExtReg->u8WaveDiff2)
			{
				s32GrDiff3cnt++;
			}
		}

		s32GbPat = 0;
		for (i = 1; i < s32Gbindx ; i++)
		{
			if (abs(s32GbAvg - *(pS32GbCnt + i)) <= pstExtReg->u8WaveDiff1)
			{
				s32GbPat++;
			}
			else if (abs(s32GbAvg - *(pS32GbCnt + i)) == pstExtReg->u8WaveDiff2)	
			{
				s32GbDiff3cnt++;
			}
		}
		if (s32GrDiff3cnt == 1)
		{
			s32GrPat++;
		}

		if (s32GbDiff3cnt == 1)
		{
			s32GbPat++;
		}

		if (s32GrAvg == -1 || s32GbAvg == -1)
		{
			s32GrPat = -1;  
			s32GbPat = -1;
		}
	}
	else
	{
		s32GrPat = -1;  
		s32GbPat = -1;
	}
	s32GrflickerThd = ((s32Grindx * pstExtReg->u8MinValidBandPcnt) + 5) / 10;
	s32GbflickerThd = ((s32Gbindx * pstExtReg->u8MinValidBandPcnt) + 5) / 10;
	// result
	if( s32GrPat >= s32GrflickerThd && s32GbPat >= s32GbflickerThd )	
	{
		pstFlickerCtx->s32CurFlicker = 1;
	}
	else		
	{
		pstFlickerCtx->s32CurFlicker = 0;
	}
	pstExtReg->s32GbAvgcnt	=	s32GbAvgcnt;
	pstExtReg->s32GrAvgcnt	=	s32GrAvgcnt;

	FlickerDetectResult( IspDev);
}

static HI_S32 FlickerReadExtregs(ISP_DEV IspDev)
{   
	ISP_FLICKER_EXTERN_REG_S *pstExtReg = HI_NULL;
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;
	
	FLICKER_GET_CTX(IspDev, pstFlickerCtx);	
	pstExtReg = &pstFlickerCtx->stFckExtReg;

	pstExtReg->u8MinBandNum		= hi_ext_system_flicker_min_band_num_read();
	pstExtReg->u8MinValidBandPcnt 	= hi_ext_system_flicker_min_valid_band_pcnt_read();
	pstExtReg->u8WaveDiff1		= hi_ext_system_flicker_wave_diff1_read();
	pstExtReg->u8WaveDiff2		= hi_ext_system_flicker_wave_diff2_read();
	pstExtReg->u8Period			= hi_ext_system_flicker_period_read();
	pstExtReg->u8GrCnt			= hi_ext_system_flicker_gr_cnt_read();
	pstExtReg->u8GbCnt			= hi_ext_system_flicker_gr_cnt_read();    
    return 0;
}

HI_S32 FlickerProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{       
    return HI_SUCCESS;
}

HI_S32 ISP_FlickerInit(ISP_DEV IspDev)
{
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL;	  
	FLICKER_GET_CTX(IspDev, pstFlickerCtx);
	pstFlickerCtx->bEnable = HI_FALSE;
	pstFlickerCtx->bMalloc = HI_FALSE;
	FlickerExtRegsInitialize();	
    	FlickerRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_FlickerRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_REG_CFG_S *pstRegCfg  = HI_NULL;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_FLICKER_ATTR *pstFlickerCtx = HI_NULL; 
	ISP_GET_CTX(IspDev, pstIspCtx);
    	FLICKER_GET_CTX(IspDev, pstFlickerCtx);	

	pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
	pstFlickerCtx->u32FrameNum = pstIspCtx->u32FrameCnt;

	if (!pstFlickerCtx->bEnable)
	{
		return HI_SUCCESS;
	}

	FlickerReadExtregs(IspDev);
	FlickerReadHWStatInfo(IspDev);
	if (pstFlickerCtx->u32FrameNum)
	{
		FlickerDetect(IspDev);
	}
	
	pstIspCtx->stLinkage.bFlicker     = (HI_BOOL)pstFlickerCtx->s32RetFlickResult;
	pstIspCtx->stLinkage.u8Freqresult = (HI_U8)pstFlickerCtx->s32RetFreqResult;

	hi_ext_system_flicker_result_write(pstFlickerCtx->s32RetFlickResult);
	hi_ext_system_freq_result_write(pstFlickerCtx->s32RetFreqResult);	

	pstRegCfg->stFlickerRegCfg.s32PreFrameAvgGr = pstFlickerCtx->s32PreFrameAvgGr;
	pstRegCfg->stFlickerRegCfg.s32PreFrameAvgGb = pstFlickerCtx->s32PreFrameAvgGb;

	pstRegCfg->unKey.bit1FlickerCfg = 1;
	
    return HI_SUCCESS;
}

static __inline HI_S32 FlickerImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
	// update Wdr width and height
	hi_isp_flick_width_write (IspDev, pstRes->u16Width - 1);
	hi_isp_flick_height_write(IspDev, pstRes->u16Height - 1);

	return HI_SUCCESS;
}

HI_S32 ISP_FlickerCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
	//HI_U8 u8WDRMode;
	
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :			
			FlickerExtRegsInitialize();			
			FlickerRegsInitialize(IspDev);			
        	break;
        case ISP_PROC_WRITE:
            		FlickerProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            	break;
	 case ISP_CHANGE_IMAGE_MODE_SET:
	      		FlickerImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);
	      break;
        default :
            	break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_FlickerExit(ISP_DEV IspDev)
{
	FlickerFreeMem(IspDev);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterFlicker(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_FLICKER;
    pstAlgs->stAlgFunc.pfn_alg_init	= ISP_FlickerInit;
    pstAlgs->stAlgFunc.pfn_alg_run  	= ISP_FlickerRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl 	= ISP_FlickerCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit 	= ISP_FlickerExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


