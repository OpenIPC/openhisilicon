/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/08/17
  Description   :
  History       :
  1.Date        : 2010/08/17
    Author      : n00168968
    Modification: Created file

******************************************************************************/


#include "hi_osal.h"

#include "hi_common.h"
#include "hi_comm_isp.h"
#include "mkp_isp.h"
#include "isp.h"
#include "hi_drv_vreg.h"
#include "mm_ext.h"
#include "proc_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"
#include "isp_ext.h"
#include "dev_ext.h"
#include "acm_ext.h"
#include "vb_ext.h"
#include "hi_comm_vb.h"


//#include "piris_ext.h"

#include "isp_metering_mem_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_config.h"
#include "isp.h"
#include "isp_ext_config.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#include "hi_spi.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


//#define CHIP_SUPPORT_JPEGEDCF()    (CALL_VB_IsSupplementSupport(VB_SUPPLEMENT_JPEG_MASK))


/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define IO_ISP_ADDRESS(IspDev, x)      ((unsigned long)reg_vi_isp_base_va[IspDev] + (x))
#define IO_RD_ISP_ADDRESS(IspDev, x)   (*((volatile unsigned long *)IO_ISP_ADDRESS(IspDev, x)))


/* 0x00F8[8]: int_isp_en; 0x00F8[16]: int_pt0_en; 0x00F8[0]: int_ch0_en; */
#define VICAP_HD_MASK           0x00F8
#define VICAP_HD_INTTERRUPT     0x00F0

#define VI_PT0_INT_MASK         0x01F8
#define VI_PT0_INT              0x01F0
#define VI_PT0_INT_FSTART       (1<<0)
#define VI_PT0_INT_ERR          (3<<1)

#define VI_DES0_INT_MASK         0x02F8
#define VI_DES0_INT              0x02F0
#define VI_DES0_INT_FSTART       (1<<0)
#define VI_DES0_INT_FEND         (1<<1)

#define VI_PT0_ID               0x01E0

#define ISP_INT_MASK            0x600F8
#define ISP_INT                 0x600F0

/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR             0x01A4

#define ISP_RESET               0x0078
#define CHN_SWITCH              0x10028

#define ISP_MAGIC_VERSION       20130305
#define ISP_MAGIC_OFFSET        1
#define ISP_PROC_SIZE           0x10000 /* 10k */
#define PROC_PRT_SLICE_SIZE     256

#define BUILT_IN_WDR_RATIO_VS_S 0x400
#define BUILT_IN_WDR_RATIO_S_M  0x400
#define BUILT_IN_WDR_RATIO_M_L  0x40


int  ISP_IntBottomHalf(ISP_DEV IspDev);
int  ISP_IntUserBottomHalf(int irq, void *id);


/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
ISP_DRV_CTX_S           g_astIspDrvCtx[ISP_MAX_DEV_NUM] = {{0}};

extern void  *g_psViuAllReg[VIU_MAX_DEV_NUM];
void             *reg_vi_isp_base_va[ISP_MAX_DEV_NUM] = {HI_NULL};
unsigned int      isp_irq[ISP_MAX_DEV_NUM] = {ISP0_IRQ_NR,ISP1_IRQ_NR};

osal_spinlock_t g_stIspLock[ISP_MAX_DEV_NUM];


//ISP_EXPORT_FUNC_S       g_stIspExpFunc = {0};
//ISP_PIRIS_EXPORT_FUNC_S  g_stIspPirisExpFunc = {0};
static ISP_VERSION_S    gs_stIspLibInfo;


HI_U32                  proc_param = 0;         /* 0: close proc; n: write proc info's interval int num */
HI_U32                  pwm_num = (0x4 << 16) | 0x4;
HI_U32                  update_pos = 0;         /* 0: frame start; 1: frame end */
HI_U32                  int_timeout = 200;          /* The time(unit:ms) of interrupt timeout */
HI_BOOL                 int_bottomhalf = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */
HI_U32                  stat_intvl = 1;             /* update isp statistic information per STAT_INTVAL frame, purpose to reduce CPU load */

HI_BOOL                 use_bottomhalf = HI_FALSE;  /* 1 to use interrupt processing at bottom half */
HI_BOOL                 g_bPrimStaReaded[ISP_MAX_DEV_NUM] = {HI_FALSE, HI_FALSE};/*HI_TRUE represent the PrimStatistic have readed from logic*/
HI_BOOL                 g_bHaveEndInt[ISP_MAX_DEV_NUM] = {HI_TRUE, HI_TRUE}; /*HI_TRUE represent last frame is whole frame, statistic will be update*/

//#define TEST_TIME

#ifdef TEST_TIME
HI_U32  g_test_phyaddr;
HI_U16 *g_test_pviraddr;
struct osal_timeval time1;
struct osal_timeval time2;
#endif
/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/
#if 0
static HI_U16 Sqrt32(HI_U32 u32Arg)
{
    HI_U32 u32Mask = (HI_U32)1 << 15;
    HI_U16 u16Res  = 0;
    HI_U32 i = 0;

    for(i=0; i<16; i++)
    {
        if((u16Res + (u32Mask >> i))*(u16Res + (u32Mask >> i)) <= u32Arg)
        {
            u16Res = u16Res + (u32Mask >> i);
        }
    }

    /* rounding */
    if (u16Res * u16Res + u16Res < u32Arg)
    {
        ++u16Res;
    }

    return u16Res;
}


/*Alpha if fixed to 0x100, input is 16 bit width, output is 16 bit width*/
static HI_S32 InvGammaFe(HI_U16 u16y)
{
    return ((HI_U32)u16y << 16) / ((1 << 20) - 15 * (HI_U32)u16y);
}
#endif

HI_S32 ISP_SetIntEnable(ISP_DEV IspDev, HI_BOOL bEn)
{
    if (bEn)
    {
        IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK) = 0x3;
    }
    else
    {
        IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK) = 0x0;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_WaitConditionCallback(HI_VOID* pParam)
{
	HI_BOOL bCondition;

	bCondition = *(HI_BOOL*)pParam;

	return (HI_TRUE == bCondition);
}

HI_S32 ISP_GetFrameEdge(ISP_DEV IspDev, HI_U32 *pu32Status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (0 != int_timeout)
    {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWait, ISP_WaitConditionCallback,
                    &(pstDrvCtx->bEdge), int_timeout);
        if (s32Ret <= 0)
        {
            #if 1
            //osal_printk(  "Wait frame edge timeout!\n");
            #endif
            return -1;
        }
    }
    else
    {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWait, ISP_WaitConditionCallback, &(pstDrvCtx->bEdge));
        if(s32Ret)
        {
            //osal_up(&pstDrvCtx->stIspSem);
            return -1;
        }
    }

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->bEdge = HI_FALSE;
    *pu32Status = pstDrvCtx->u32Status;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}

static int ISP_GetVdTimeOut(ISP_DEV IspDev, ISP_VD_INFO_S  *pIspVdInfo,
    HI_U32 u32MilliSec, HI_U32 *pu32status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if(osal_down_interruptible(&pstDrvCtx->stIspSem))
    {
       return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->bVd = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    if(0 != u32MilliSec)
    {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVd, ISP_WaitConditionCallback,
            &(pstDrvCtx->bVd), (u32MilliSec));
        if (s32Ret <= 0)
        {
            #if 0
            osal_printk(  "Wait frame edge timeout!\n");
            #endif
            osal_up(&pstDrvCtx->stIspSem);
            return -1;
        }
    }
    else
    {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVd, ISP_WaitConditionCallback, &(pstDrvCtx->bVd));
        if(s32Ret)
        {
            osal_up(&pstDrvCtx->stIspSem);
            return -1;
        }

    }

    *pu32status = pstDrvCtx->u32Status;

    osal_up(&pstDrvCtx->stIspSem);

    return 0;
}

static int ISP_GetVdEndTimeOut(ISP_DEV IspDev, ISP_VD_INFO_S  *pIspVdInfo,
    HI_U32 u32MilliSec, HI_U32 *pu32status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if(osal_down_interruptible(&pstDrvCtx->stIspSem))
    {
       return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->bVdEnd = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    if(0 != u32MilliSec)
    {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_WaitConditionCallback,
            &(pstDrvCtx->bVdEnd), (u32MilliSec));
        if (s32Ret <= 0)
        {
            #if 0
            osal_printk(  "Wait frame edge timeout!\n");
            #endif
            osal_up(&pstDrvCtx->stIspSem);
            return -1;
        }
    }
    else
    {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_WaitConditionCallback, &(pstDrvCtx->bVdEnd));
        if(s32Ret)
        {
            osal_up(&pstDrvCtx->stIspSem);
            return -1;
        }

    }

    *pu32status = 1;

    osal_up(&pstDrvCtx->stIspSem);

    return 0;
}

HI_S32 ISP_DRV_PrimStatisticsRead(ISP_DEV IspDev, HI_VOID *pstPrimStat)
{

    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_PRIM_STAT_S stStat;
    ISP_PRIM_STAT_S *pstPrimSt = (ISP_PRIM_STAT_S *)pstPrimStat;
    HI_S32 i, j;
    HI_U32 u32Value, u32HighBit, u32LowBit;
    HI_U32 u32ZoneAvg1, u32ZoneAvg2;
    ISP_STAT_KEY_U unStatkey;
    ISP_AE_STAT_3_S  *pstAeStat3 = HI_NULL;
    ISP_AE_STAT_5_S  *pstAeStat5 = HI_NULL;
    ISP_AWB_STAT_4_S *pstAwbStat4 = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if ((HI_NULL == pstPrimSt) || (HI_NULL == pstPrimSt->pstAeStat3)
        || (HI_NULL == pstPrimSt->pstAeStat5)
        || (HI_NULL == pstPrimSt->pstAwbStat4))
    {
        return HI_FAILURE;
    }

    /********************** pPrimVirAddr: |___AE_STAT_3__|__AE_STAT_5__|__AWB_STAT_4__| ************************/
    pstAeStat3  = (ISP_AE_STAT_3_S *)((HI_U8 *)pstDrvCtx->stPrimStatisticsBuf.pPrimVirAddr);
    pstAeStat5  = (ISP_AE_STAT_5_S *)((HI_U8 *)pstDrvCtx->stPrimStatisticsBuf.pPrimVirAddr + sizeof(ISP_AE_STAT_3_S));
    pstAwbStat4 = (ISP_AWB_STAT_4_S *)((HI_U8 *)pstDrvCtx->stPrimStatisticsBuf.pPrimVirAddr + sizeof(ISP_AE_STAT_3_S)
                   + sizeof(ISP_AE_STAT_5_S));

    if (HI_TRUE == g_bPrimStaReaded[IspDev])
    {
        osal_memcpy(pstPrimSt->pstAeStat3, pstAeStat3, sizeof(ISP_AE_STAT_3_S));
        osal_memcpy(pstPrimSt->pstAeStat5, pstAeStat5, sizeof(ISP_AE_STAT_5_S));
        osal_memcpy(pstPrimSt->pstAwbStat4, pstAwbStat4, sizeof(ISP_AWB_STAT_4_S));
        return HI_SUCCESS;
    }

    stStat.pstAeStat3  = pstAeStat3;
    stStat.pstAeStat5  = pstAeStat5;
    stStat.pstAwbStat4 = pstAwbStat4;
    if ((HI_NULL == stStat.pstAeStat3) || (HI_NULL == stStat.pstAeStat5)
        || (HI_NULL == stStat.pstAwbStat4))
    {
        return HI_FAILURE;
    }

    unStatkey.u32Key = pstDrvCtx->u32bit16IsrAccess;

    if (unStatkey.bit1AeStat3)
    {
        for(i = 0; i < 1024; i++)
        {
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            stStat.pstAeStat3->au32HistogramMemArray[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));
        }
    }

    if (unStatkey.bit1AeStat5)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                u32ZoneAvg1 = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AE_SUM_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 0) << 2));
                u32ZoneAvg2 = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AE_SUM_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 1) << 2));

                stStat.pstAeStat5->au16ZoneAvg[i][j][0] = (HI_U16)(u32ZoneAvg1 & 0xffff);              /*R */
                stStat.pstAeStat5->au16ZoneAvg[i][j][1] = (HI_U16)((u32ZoneAvg1 & 0xffff0000) >> 16);  /*Gr*/
                stStat.pstAeStat5->au16ZoneAvg[i][j][2] = (HI_U16)(u32ZoneAvg2 & 0xffff);              /*Gb*/
                stStat.pstAeStat5->au16ZoneAvg[i][j][3] = (HI_U16)((u32ZoneAvg2 & 0xffff0000) >> 16);  /*B */
            }
        }
    }

    if (unStatkey.bit1AwbStat4)
    {
        if (IS_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode))
        {
            HI_U32 u32AWBRGain = hi_isp_white_balance_gain_00_read(IspDev);
            HI_U32 u32AWBGGain = hi_isp_white_balance_gain_01_read(IspDev);
            HI_U32 u32AWBBGain = hi_isp_white_balance_gain_11_read(IspDev);

            u32AWBRGain = (u32AWBGGain << 8) / DIV_0_TO_1(u32AWBRGain);
            u32AWBBGain = (u32AWBGGain << 8) / DIV_0_TO_1(u32AWBBGain);

            for (i=0; i<AWB_ZONE_NUM; i++)
            {
                HI_U32 u32AvgR, u32AvgG, u32AvgB;
                HI_U16 CountAll, CountMin, CountMax;

                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12));
                u32AvgR = (u32Value & 0xffff);
                u32AvgG = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 4);
                u32AvgB = (u32Value & 0xffff);
                CountAll = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 8);
                CountMax = u32Value & 0xffff;
                CountMin = (u32Value>>16) & 0xffff;

                if ((0 == CountAll) || (u32AvgG < 0x100))
                {
                    HI_U32 u32Rg, u32Bg, u32Sum;

                    u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_METERING_MEM_BASE_ADDR + (i << 3));
                    u32Rg = u32Value & 0xfff;
                    u32Bg = (u32Value >> 16) & 0xfff;

                    u32Sum = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_METERING_MEM_BASE_ADDR + (i << 3) + 4);

                    if (u32Sum >= 0x40)
                    {
                        u32AvgG = 0x100;
                        u32AvgR = (u32AWBRGain * u32Rg >> 8);
                        u32AvgB = (u32AWBBGain * u32Bg >> 8);

                        CountAll = 0xFFFF;
                        CountMax = 0x0;
                        CountMin = 0x0;
                    }
                }

                stStat.pstAwbStat4->au16MeteringMemArrayAvgR[i] = (HI_U16)u32AvgR;
                stStat.pstAwbStat4->au16MeteringMemArrayAvgG[i] = (HI_U16)u32AvgG;
                stStat.pstAwbStat4->au16MeteringMemArrayAvgB[i] = (HI_U16)u32AvgB;
                stStat.pstAwbStat4->au16MeteringMemArrayCountAll[i] = CountAll;
                stStat.pstAwbStat4->au16MeteringMemArrayCountMax[i] = CountMax;
                stStat.pstAwbStat4->au16MeteringMemArrayCountMin[i] = CountMin;
            }
        }
        else
        {
            for (i=0; i<AWB_ZONE_NUM; i++)
            {
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12));
                stStat.pstAwbStat4->au16MeteringMemArrayAvgR[i] = (u32Value & 0xffff);
                stStat.pstAwbStat4->au16MeteringMemArrayAvgG[i] = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 4);
                stStat.pstAwbStat4->au16MeteringMemArrayAvgB[i] = (u32Value & 0xffff);
                stStat.pstAwbStat4->au16MeteringMemArrayCountAll[i] = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 8);
                stStat.pstAwbStat4->au16MeteringMemArrayCountMax[i] = u32Value & 0xffff;
                stStat.pstAwbStat4->au16MeteringMemArrayCountMin[i] = (u32Value>>16) & 0xffff;
            }
		}
    }

    osal_memcpy(pstPrimSt->pstAeStat3, pstAeStat3, sizeof(ISP_AE_STAT_3_S));
    osal_memcpy(pstPrimSt->pstAeStat5, pstAeStat5, sizeof(ISP_AE_STAT_5_S));
    osal_memcpy(pstPrimSt->pstAwbStat4, pstAwbStat4, sizeof(ISP_AWB_STAT_4_S));
    g_bPrimStaReaded[IspDev] = HI_TRUE;

    return HI_SUCCESS;

}
HI_S32 ISP_DRV_StatisticsRead(ISP_DEV IspDev, ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_STAT_S *pstStat = HI_NULL;
    HI_S32 i, j;
    HI_U32 u32Value, u32HighBit, u32LowBit;
    //HI_U32 u32Sum;
    //HI_U32 u32Hist10, u32Hist43;
    //HI_U32 u32ZoneAvg1, u32ZoneAvg2;
    ISP_STAT_KEY_U unStatkey;
    ISP_PRIM_STAT_S stPrimStat;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (((SNAP_TYPE_SYNC == pstDrvCtx->stSnapAttr.enSnapType) || (SNAP_TYPE_SYNC_PRO == pstDrvCtx->stSnapAttr.enSnapType) || (SNAP_TYPE_PRO == pstDrvCtx->stSnapAttr.enSnapType)) && (ISP_SNAP_PICTURE == pstDrvCtx->stSnapAttr.enSnapPipeMode))
    {
        return HI_SUCCESS;
    }

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;
    if (HI_NULL == pstStat)
    {
        return HI_FAILURE;
    }

#ifdef TEST_TIME
    {
        osal_printk("%x\n", g_test_pviraddr);

        osal_gettimeofday(&time1);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au16HistogramMemArray[i] = *(g_test_pviraddr + i);
        }
        osal_gettimeofday(&time2);
        osal_printk("read memory %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        osal_gettimeofday(&time1);
        osal_memcpy(pstStat->stAeStat3.au16HistogramMemArray, g_test_pviraddr, 256*2);
        osal_gettimeofday(&time2);
        osal_printk("memory copy %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        osal_gettimeofday(&time1);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au16HistogramMemArray[i] =
            IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
        }
        osal_gettimeofday(&time2);
        osal_printk("read register %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
    }
#endif
    if (HI_FALSE == g_bHaveEndInt[IspDev])
    {
        return HI_SUCCESS;
    }

    unStatkey.u32Key = pstStatInfo->unKey.bit16IsrAccess;
    stPrimStat.pstAeStat3 = &pstStat->stAeStat3;
    stPrimStat.pstAeStat5 = &pstStat->stAeStat5;
    stPrimStat.pstAwbStat4 = &pstStat->stAwbStat4;
    ISP_DRV_PrimStatisticsRead(IspDev, (HI_VOID *)&stPrimStat);

    if (unStatkey.bit1AeStat1)
    {
        pstStat->stAeStat1.au16MeteringHist[0] = hi_isp_metering_hist_0_read(IspDev);
        pstStat->stAeStat1.au16MeteringHist[1] = hi_isp_metering_hist_1_read(IspDev);
        pstStat->stAeStat1.au16MeteringHist[2] = 0;
        pstStat->stAeStat1.au16MeteringHist[3] = hi_isp_metering_hist_3_read(IspDev);
        pstStat->stAeStat1.au16MeteringHist[4] = hi_isp_metering_hist_4_read(IspDev);
        pstStat->stAeStat1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read(IspDev);
        pstStat->stAeStat1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read(IspDev);
        pstStat->stAeStat1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read(IspDev);
        pstStat->stAeStat1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read(IspDev);

        u32Value = 0;
        for(i=0; i<HIST_5_NUM; ++i)
        {
            u32Value += pstStat->stAeStat1.au16MeteringHist[i];
        }

        if (u32Value > 0xFFFF)
        {
            u32Value = 0xFFFF;
        }
        pstStat->stAeStat1.au16MeteringHist[2] = 0xFFFF - u32Value;
    }

    if (unStatkey.bit1AwbStat1)
    {
        pstStat->stAwbStat1.u16MeteringAwbRg = hi_isp_metering_awb_rg_read(IspDev);
        pstStat->stAwbStat1.u16MeteringAwbBg = hi_isp_metering_awb_bg_read(IspDev);
        pstStat->stAwbStat1.u32MeteringAwbSum = hi_isp_metering_awb_sum_read(IspDev);
    }

    if (unStatkey.bit1AwbStat3)
    {
#if 0
        HI_U16 u16BlackLevelR = (HI_U16)(hi_isp_offset_black_00_read(IspDev) >> 4);
        HI_U16 u16BlackLevelG = (HI_U16)(hi_isp_offset_black_01_read(IspDev) >> 4);
        HI_U16 u16BlackLevelB = (HI_U16)(hi_isp_offset_black_11_read(IspDev) >> 4);
#endif
        pstStat->stAwbStat3.u16MeteringAwbAvgR = hi_isp_metering_avg_r_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbAvgG = hi_isp_metering_avg_g_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbAvgB = hi_isp_metering_avg_b_read(IspDev);

#if 0
        if (IS_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode))
        {
            pstStat->stAwbStat3.u16MeteringAwbAvgR = InvGammaFe(pstStat->stAwbStat3.u16MeteringAwbAvgR);
            pstStat->stAwbStat3.u16MeteringAwbAvgG = InvGammaFe(pstStat->stAwbStat3.u16MeteringAwbAvgG);
            pstStat->stAwbStat3.u16MeteringAwbAvgB = InvGammaFe(pstStat->stAwbStat3.u16MeteringAwbAvgB);
        }
#endif

        pstStat->stAwbStat3.u16MeteringAwbCountAll = hi_isp_metering_count_all_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbCountMin = hi_isp_metering_count_min_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbCountMax = hi_isp_metering_count_max_read(IspDev);
#if 0
        if (0 != pstStat->stAwbStat3.u16MeteringAwbCountAll)
        {
            if ((pstStat->stAwbStat3.u16MeteringAwbAvgR >= u16BlackLevelR) &&
                (pstStat->stAwbStat3.u16MeteringAwbAvgG >= u16BlackLevelG) &&
                (pstStat->stAwbStat3.u16MeteringAwbAvgB >= u16BlackLevelB))
            {
                pstStat->stAwbStat3.u16MeteringAwbAvgR -= u16BlackLevelR;
                pstStat->stAwbStat3.u16MeteringAwbAvgG -= u16BlackLevelG;
                pstStat->stAwbStat3.u16MeteringAwbAvgB -= u16BlackLevelB;
            }
            else
            {
                pstStat->stAwbStat3.u16MeteringAwbAvgR = 0;
                pstStat->stAwbStat3.u16MeteringAwbAvgG = 0;
                pstStat->stAwbStat3.u16MeteringAwbAvgB = 0;
                pstStat->stAwbStat3.u16MeteringAwbCountAll = 0;
                pstStat->stAwbStat3.u16MeteringAwbCountMin = 0xFFFF;
                pstStat->stAwbStat3.u16MeteringAwbCountMax = 0x0;
            }
        }
#endif
    }

    if (unStatkey.bit1AfStat)
    {
        HI_U32 u32Zone = hi_area_block_distribute_data_read_af(IspDev);
        HI_U8 u8Col = (u32Zone & 0x1F);
        HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);
        HI_U32 u32V1H1, u32V2H2, u32Y;

        u8Col = (u8Col > AF_ZONE_COLUMN) ? AF_ZONE_COLUMN : u8Col;
        u8Row = (u8Row > AF_ZONE_ROW) ? AF_ZONE_ROW : u8Row;

        for (i = 0; i < u8Row; i++)
        {
            for (j = 0; j < u8Col; j++)
            {
                u32V1H1 = hi_area_stat_mem_array_data_read_af_v1h1(IspDev, i * u8Col + j);
                pstStat->stAfStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & u32V1H1) >> 16);
                pstStat->stAfStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & u32V1H1);
                u32V2H2 = hi_area_stat_mem_array_data_read_af_v2h2(IspDev, i * u8Col + j);
                pstStat->stAfStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & u32V2H2) >> 16);
                pstStat->stAfStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & u32V2H2);
                u32Y = hi_area_stat_mem_array_data_read_af_y(IspDev, i * u8Col + j);
                pstStat->stAfStat.stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & u32Y);
                pstStat->stAfStat.stZoneMetrics[i][j].u16HlCnt = (HI_U16)((0xFFFF0000 & u32Y) >> 16);
            }
        }
    }

    if (unStatkey.bit1DpStat)
    {
        pstStat->stDpStat.u16DefectPixelCount = hi_isp_defect_pixel_defect_pixel_count_read(IspDev);
    }

    if (unStatkey.bit1CommStat)
    {
        pstStat->stCommStat.au16WhiteBalanceGain[0] = hi_isp_white_balance_gain_00_read(IspDev);
        pstStat->stCommStat.au16WhiteBalanceGain[1] = hi_isp_white_balance_gain_01_read(IspDev);
        pstStat->stCommStat.au16WhiteBalanceGain[2] = hi_isp_white_balance_gain_10_read(IspDev);
        pstStat->stCommStat.au16WhiteBalanceGain[3] = hi_isp_white_balance_gain_11_read(IspDev);
    }


    if (unStatkey.bit1AeStat2)
    {
        #if 0
        pstStat->stAeStat2.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read(IspDev);
        pstStat->stAeStat2.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read(IspDev);
        pstStat->stAeStat2.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read(IspDev);
        pstStat->stAeStat2.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read(IspDev);

        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                u32Hist10 = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AE_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 0) << 2));
                u32Hist43 = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AE_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 1) << 2));

                /* todo : optimize,read first, then calculate */
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][0] = (HI_U16)(u32Hist10 & 0xffff);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][1] = (HI_U16)((u32Hist10 & 0xffff0000) >> 16);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][3] = (HI_U16)(u32Hist43 & 0xffff);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][4] = (HI_U16)((u32Hist43 & 0xffff0000) >> 16);
            }
        }
        #endif
    }
#if 0
    if (unStatkey.bit1AeStat3)
    {
        for(i = 0; i < 1024; i++)
        {
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            pstStat->stAeStat3.au32HistogramMemArray[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));
        }
    }
#endif
    if (unStatkey.bit1AeStat4)
    {
        pstStat->stAeStat4.u16GlobalAvgR  = hi_isp_metering_sum_r_read(IspDev);
        pstStat->stAeStat4.u16GlobalAvgGr = hi_isp_metering_sum_rg_read(IspDev);
        pstStat->stAeStat4.u16GlobalAvgGb = hi_isp_metering_sum_bg_read(IspDev);
        pstStat->stAeStat4.u16GlobalAvgB  = hi_isp_metering_sum_b_read(IspDev);
    }
#if 0
    if (unStatkey.bit1AeStat5)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                u32ZoneAvg1 = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AE_SUM_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 0) << 2));
                u32ZoneAvg2 = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AE_SUM_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 1) << 2));

                pstStat->stAeStat5.au16ZoneAvg[i][j][0] = (HI_U16)(u32ZoneAvg1 & 0xffff);              /*R */
                pstStat->stAeStat5.au16ZoneAvg[i][j][1] = (HI_U16)((u32ZoneAvg1 & 0xffff0000) >> 16);  /*Gr*/
                pstStat->stAeStat5.au16ZoneAvg[i][j][2] = (HI_U16)(u32ZoneAvg2 & 0xffff);              /*Gb*/
                pstStat->stAeStat5.au16ZoneAvg[i][j][3] = (HI_U16)((u32ZoneAvg2 & 0xffff0000) >> 16);  /*B */
            }
        }
    }
#endif

    if (unStatkey.bit1AeStat6)
    {
        for(i = 0; i < HIST_256_NUM; i++)
        {
            /* Pre 256 bin histogram1, channel 1 */
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_1_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            pstStat->stAeStat6.au32PreHistogram0[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));

            /* Pre 256 bin histogram2, channel 2 */
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_2_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            pstStat->stAeStat6.au32PreHistogram1[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));

            /* Pre 256 bin histogram3, channel 3 */
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_3_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            pstStat->stAeStat6.au32PreHistogram2[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));

            /* Pre 256 bin histogram4, channel 4 */
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_HISTOGRAM_4_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            pstStat->stAeStat6.au32PreHistogram3[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));
        }
    }

    if (unStatkey.bit1AwbStat2)
    {
        for (i=0; i< AWB_ZONE_NUM; i++)
        {
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_METERING_MEM_BASE_ADDR + (i << 3));
            pstStat->stAwbStat2.au16MeteringMemArrayRg[i] = u32Value & 0xfff;
            pstStat->stAwbStat2.au16MeteringMemArrayBg[i] = (u32Value >> 16) & 0xfff;
            u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_METERING_MEM_BASE_ADDR + (i << 3) + 4);
            pstStat->stAwbStat2.au16MeteringMemArraySum[i] = u32Value;
        }
    }
#if 0
    if (unStatkey.bit1AwbStat4)
    {
        if (IS_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode))
        {
            HI_U32 u32AWBRGain = hi_isp_white_balance_gain_00_read(IspDev);
            HI_U32 u32AWBGGain = hi_isp_white_balance_gain_01_read(IspDev);
            HI_U32 u32AWBBGain = hi_isp_white_balance_gain_11_read(IspDev);

            u32AWBRGain = (u32AWBGGain << 8) / DIV_0_TO_1(u32AWBRGain);
            u32AWBBGain = (u32AWBGGain << 8) / DIV_0_TO_1(u32AWBBGain);

            for (i=0; i<AWB_ZONE_NUM; i++)
            {
                HI_U32 u32AvgR, u32AvgG, u32AvgB;
                HI_U16 CountAll, CountMin, CountMax;

                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12));
                u32AvgR = (u32Value & 0xffff);
                u32AvgG = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 4);
                u32AvgB = (u32Value & 0xffff);
                CountAll = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 8);
                CountMax = u32Value & 0xffff;
                CountMin = (u32Value>>16) & 0xffff;

                if ((0 == CountAll) || (u32AvgG < 0x100))
                {
                    HI_U32 u32Rg, u32Bg, u32Sum;

                    u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_METERING_MEM_BASE_ADDR + (i << 3));
                    u32Rg = u32Value & 0xfff;
                    u32Bg = (u32Value >> 16) & 0xfff;

                    u32Sum = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_METERING_MEM_BASE_ADDR + (i << 3) + 4);

                    if (u32Sum >= 0x40)
                    {
                        u32AvgG = 0x100;
                        u32AvgR = (u32AWBRGain * u32Rg >> 8);
                        u32AvgB = (u32AWBBGain * u32Bg >> 8);

                        CountAll = 0xFFFF;
                        CountMax = 0x0;
                        CountMin = 0x0;
                    }
                }

                pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] = (HI_U16)u32AvgR;
                pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] = (HI_U16)u32AvgG;
                pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] = (HI_U16)u32AvgB;
                pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i] = CountAll;
                pstStat->stAwbStat4.au16MeteringMemArrayCountMax[i] = CountMax;
                pstStat->stAwbStat4.au16MeteringMemArrayCountMin[i] = CountMin;
            }
        }
        else
        {
            for (i=0; i<AWB_ZONE_NUM; i++)
            {
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12));
                pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] = (u32Value & 0xffff);
                pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 4);
                pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] = (u32Value & 0xffff);
                pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i] = ((u32Value >> 16) & 0xffff);
                u32Value = IO_RD_ISP_ADDRESS(IspDev, VI_ISP_OFFSET + HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 8);
                pstStat->stAwbStat4.au16MeteringMemArrayCountMax[i] = u32Value & 0xffff;
                pstStat->stAwbStat4.au16MeteringMemArrayCountMin[i] = (u32Value>>16) & 0xffff;
            }
		}
    }
#endif

    if (unStatkey.bit1AeStat2)
    {
        #if 0
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                u32Sum = pstStat->stAeStat2.au16MeteringMemArrary[i][j][0]
                    + pstStat->stAeStat2.au16MeteringMemArrary[i][j][1]
                    + pstStat->stAeStat2.au16MeteringMemArrary[i][j][3]
                    + pstStat->stAeStat2.au16MeteringMemArrary[i][j][4];
                if(u32Sum > 0xFFFF)
                {
                    u32Sum = 0xFFFF;
                }

                pstStat->stAeStat2.au16MeteringMemArrary[i][j][2] = 0xFFFF - u32Sum;
            }
        }
        #endif
    }


    if (unStatkey.bit1Dehaze)
    {

        j = DEFOG_ZONE_NUM / 2;
		hi_isp_dehaze_minstat_raddr_write(IspDev, 0);
        for(i = 0; i < j; i++)
        {
            pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_isp_dehaze_minstat_rdata_read(IspDev);
        }

    }

    g_bHaveEndInt[IspDev] = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret, i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;
    HI_U32 u32PrimPhyAddr, u32PrimSize;
    HI_U8  *pu8PrimVirAddr;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM;
    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ISPStat", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u32Size);

    u32PrimSize = sizeof(ISP_AE_STAT_3_S) + sizeof(ISP_AE_STAT_5_S) + sizeof(ISP_AWB_STAT_4_S);
    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ISPPrimStat", &u32PrimPhyAddr, (HI_VOID**)&pu8PrimVirAddr, u32PrimSize);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc primary statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }
    osal_memset(pu8PrimVirAddr, 0, u32PrimSize);
    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = u32PhyAddr;
    pstDrvCtx->stStatisticsBuf.pVirAddr = (HI_VOID *)pu8VirAddr;
    pstDrvCtx->stStatisticsBuf.u32Size  = u32Size;
    pstDrvCtx->stPrimStatisticsBuf.u32PrimPhyAddr = u32PrimPhyAddr;
    pstDrvCtx->stPrimStatisticsBuf.pPrimVirAddr = (HI_VOID *)pu8PrimVirAddr;
    pstDrvCtx->stPrimStatisticsBuf.u32Size      = u32PrimSize;

    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);

    for (i = 0; i < MAX_ISP_STAT_BUF_NUM; i++)
    {
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.u32PhyAddr =
            u32PhyAddr + i * sizeof(ISP_STAT_S);
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.pVirtAddr =
            (HI_VOID *)(pu8VirAddr + i * sizeof(ISP_STAT_S));

        osal_list_add_tail(&pstDrvCtx->stStatisticsBuf.astNode[i].list,
            &pstDrvCtx->stStatisticsBuf.stFreeList);
    }

    pstDrvCtx->stStatisticsBuf.bInit = HI_TRUE;
    pstDrvCtx->stStatisticsBuf.u32BusyNum = 0;
    pstDrvCtx->stStatisticsBuf.u32UserNum = 0;
    pstDrvCtx->stStatisticsBuf.u32FreeNum = MAX_ISP_STAT_BUF_NUM;

    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    HI_U32 u32PrimPhyAddr;
    HI_U8  *pu8PrimVirAddr;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->stStatisticsBuf.u32PhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stStatisticsBuf.pVirAddr;
    u32PrimPhyAddr = pstDrvCtx->stPrimStatisticsBuf.u32PrimPhyAddr;
    pu8PrimVirAddr = pstDrvCtx->stPrimStatisticsBuf.pPrimVirAddr;

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->stStatisticsBuf.pVirAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.bInit = HI_FALSE;
    pstDrvCtx->stPrimStatisticsBuf.pPrimVirAddr = HI_NULL;
    pstDrvCtx->stPrimStatisticsBuf.u32PrimPhyAddr = 0;
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    if (0 != u32PhyAddr)
    {
        hil_mmb_invalid_cache_byaddr(pu8VirAddr, u32PhyAddr, pstDrvCtx->stStatisticsBuf.u32Size);
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }
    if (0 != u32PrimPhyAddr)
    {
        hil_mmb_invalid_cache_byaddr(pu8PrimVirAddr, u32PrimPhyAddr, pstDrvCtx->stPrimStatisticsBuf.u32Size);
        CMPI_MmzFree(u32PrimPhyAddr, pu8PrimVirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserGet(ISP_DEV IspDev, ISP_STAT_INFO_S **ppstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_STATBUFF_INIT(IspDev);
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        ISP_TRACE(HI_DBG_WARN, "busy list empty\n");
        *ppstStatInfo = HI_NULL;
        osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
        return HI_FAILURE;
    }

    /* get busy */
    plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
    HI_ASSERT(plist != HI_NULL);
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32BusyNum--;

    /* return info */
    pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);
    *ppstStatInfo = &pstNode->stStatInfo;

    /* put user */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stUserList);
    pstDrvCtx->stStatisticsBuf.u32UserNum++;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserPut(ISP_DEV IspDev, ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    HI_BOOL bValid = HI_FALSE;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_STATBUFF_INIT(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    osal_list_for_each(plist, &pstDrvCtx->stStatisticsBuf.stUserList)
    {
        pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);
        HI_ASSERT(HI_NULL != pstNode);
        if (pstNode->stStatInfo.u32PhyAddr == pstStatInfo->u32PhyAddr)
        {
            bValid = HI_TRUE;
            pstNode->stStatInfo.unKey.u32Key = pstStatInfo->unKey.u32Key;
            pstDrvCtx->u32bit16IsrAccess = pstStatInfo->unKey.bit16IsrAccess;
            break;
        }
    }

    if (!bValid)
    {
        ISP_TRACE(HI_DBG_ERR, "invalid stat info, phy 0x%x\n", pstStatInfo->u32PhyAddr);
        osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
        return HI_FAILURE;
    }

    /* get user */
    HI_ASSERT(plist != HI_NULL);
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32UserNum--;

    /* put free */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
    pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufBusyPut(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);

    /* There should be one frame of the newest statistics info in busy list. */
    while (!osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
        HI_ASSERT(plist != HI_NULL);
        osal_list_del(plist);
        pstDrvCtx->stStatisticsBuf.u32BusyNum--;

        osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
        pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    }

    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stFreeList))
    {
        ISP_TRACE(HI_DBG_WARN, "free list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

        return HI_FAILURE;
    }

    /* get free */
    plist = pstDrvCtx->stStatisticsBuf.stFreeList.next;
    HI_ASSERT(plist != HI_NULL);
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32FreeNum--;

    /* read statistics */
    pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);

    pstDrvCtx->stStatisticsBuf.pstActStat = &pstNode->stStatInfo;
    //pstNode->stStatInfo.unKey.u32Key = 0xffff;
    ISP_DRV_StatisticsRead(IspDev, &pstNode->stStatInfo);

    /* put busy */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stBusyList);
    pstDrvCtx->stStatisticsBuf.u32BusyNum++;

    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StaKernelGet(ISP_DEV IspDev, ISP_DRV_FOCUS_STATISTICS_S *pstFocusStat)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_STAT_S *pstStat = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (HI_NULL == pstFocusStat)
    {
        ISP_TRACE(HI_DBG_ERR, "get statistic active buffer err, pstFocusStat is NULL!\n");
        return HI_FAILURE;
    }

    if (!pstDrvCtx->stStatisticsBuf.pstActStat)
    {
        ISP_TRACE(HI_DBG_INFO, "get statistic active buffer err, stat not ready!\n");
        return HI_FAILURE;
    }

    if (!pstDrvCtx->stStatisticsBuf.pstActStat->pVirtAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "get statistic active buffer err, pVirtAddr is NULL!\n");
        return HI_FAILURE;
    }

    pstStat = (ISP_STAT_S *)pstDrvCtx->stStatisticsBuf.pstActStat->pVirtAddr;

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    osal_memcpy(pstFocusStat, &pstStat->stAfStat, sizeof(ISP_DRV_FOCUS_STATISTICS_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_SwitchMode(ISP_DEV IspDev, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_U8 i = 0, j = 0;
    ISP_SYNC_CFG_S      *pstSyncCfg;
    HI_U32  u32PTIntMask = 0;
    HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[5] = {0};
    HI_U32  u32IspFEInputMode = 0;

    pstSyncCfg = &pstDrvCtx->stSyncCfg;
    pstSyncCfg->u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;

    /* pt_int_mask */
    u32PTIntMask = IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT_MASK);
    if ((IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)) || (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode)))
    {
        u32PTIntMask = u32PTIntMask | (HI_U32)VI_PT0_INT_FSTART;
        //u32PTIntMask = VI_PT0_INT_FSTART;
    }
    else
    {
        u32PTIntMask = u32PTIntMask & (~(HI_U32)VI_PT0_INT_FSTART);
        //u32PTIntMask = 0;
    }
    IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT_MASK) = u32PTIntMask;

    /* init cfg when modes change */
    {
        osal_memset(&pstDrvCtx->stSyncCfg.stSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
        osal_memset(&pstDrvCtx->stSyncCfg.apstNode, 0, sizeof(pstDrvCtx->stSyncCfg.apstNode));

        pstSyncCfg->u8VCNum = 0;
        pstSyncCfg->u8VCCfgNum = 0;
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;

        /* get N (N to 1 WDR) */
        switch(pstSyncCfg->u8WDRMode)
        {
            default:
            case WDR_MODE_NONE:
            case WDR_MODE_BUILT_IN:
            case WDR_MODE_2To1_LINE:
            case WDR_MODE_3To1_LINE:
            case WDR_MODE_4To1_LINE:
                pstSyncCfg->u8VCNumMax = 0;
                break;
            case WDR_MODE_2To1_FRAME :
            case WDR_MODE_2To1_FRAME_FULL_RATE:
                pstSyncCfg->u8VCNumMax = 1;
                break;
            case WDR_MODE_3To1_FRAME :
            case WDR_MODE_3To1_FRAME_FULL_RATE:
                pstSyncCfg->u8VCNumMax = 2;
                break;
            case WDR_MODE_4To1_FRAME :
            case WDR_MODE_4To1_FRAME_FULL_RATE:
                pstSyncCfg->u8VCNumMax = 3;
                break;
        }

        /* Channel Switch config */
        if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            au32ChnSwitch[0] = HI_VI_TOP_CHN_SWITCH0_DEFAULT;
            au32ChnSwitch[1] = HI_VI_TOP_CHN_SWITCH1_DEFAULT;
            au32ChnSwitch[2] = HI_VI_TOP_CHN_SWITCH2_DEFAULT;
            au32ChnSwitch[3] = HI_VI_TOP_CHN_SWITCH3_DEFAULT;
            bChnSwitchEnable = HI_TRUE;
            u32IspFEInputMode = 1;
        }
        else if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            au32ChnSwitch[0] = 1 % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            bChnSwitchEnable = HI_TRUE;
            u32IspFEInputMode = 1;
        }
        else if (IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            if(IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 1;
                au32ChnSwitch[2] = 2;
                au32ChnSwitch[3] = 3;
                bChnSwitchEnable = HI_TRUE;
                u32IspFEInputMode = 1;
            }
            else if(IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 2;
                au32ChnSwitch[2] = 1;
                au32ChnSwitch[3] = 3;
                bChnSwitchEnable = HI_TRUE;
                u32IspFEInputMode = 1;
            }
            else if(IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 3;
                au32ChnSwitch[2] = 2;
                au32ChnSwitch[3] = 1;
                bChnSwitchEnable = HI_TRUE;
                u32IspFEInputMode = 1;
            }
            else
            {
            }

        }
        else
        {
            au32ChnSwitch[0] = HI_VI_TOP_CHN_SWITCH0_DEFAULT;
            au32ChnSwitch[1] = HI_VI_TOP_CHN_SWITCH1_DEFAULT;
            au32ChnSwitch[2] = HI_VI_TOP_CHN_SWITCH2_DEFAULT;
            au32ChnSwitch[3] = HI_VI_TOP_CHN_SWITCH3_DEFAULT;
            bChnSwitchEnable = HI_FALSE;
            u32IspFEInputMode = 0;
        }
		au32ChnSwitch[4] = 4;
        hi_vi_top_chn_switch0_write(IspDev, au32ChnSwitch[0]);
        hi_vi_top_chn_switch1_write(IspDev, au32ChnSwitch[1]);
        hi_vi_top_chn_switch2_write(IspDev, au32ChnSwitch[2]);
        hi_vi_top_chn_switch3_write(IspDev, au32ChnSwitch[3]);
        hi_vi_top_chn_switch4_write(IspDev, au32ChnSwitch[4]);
        hi_vi_top_chn_switch_enble_write(IspDev, bChnSwitchEnable);
        hi_isp_fe_input_mode_select_write(IspDev, u32IspFEInputMode);

        /* VC config */
        if ((IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode)) || (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)))
        {
            hi_vi_top_vc_enable_write(IspDev, 1);
            hi_vi_top_vc_max_write(IspDev, pstSyncCfg->u8VCNumMax);
            //hi_vi_top_vc_reset_write(IspDev, 1);
        }
        else
        {
            hi_vi_top_vc_enable_write(IspDev, 0);
            hi_vi_top_vc_max_write(IspDev, 0);
            //hi_vi_top_vc_reset_write(IspDev, 1);
        }

        /* ratio config */
        for (j=0; j<3; j++)
        {
            pstDrvCtx->stWDRCfg.au32ExpRatio[j] = CLIP3(pstDrvCtx->stWDRCfg.au32ExpRatio[j], EXP_RATIO_MIN, EXP_RATIO_MAX);
            for (i=0; i<CFG2VLD_DLY_LIMIT; i++)
            {
                pstSyncCfg->u32ExpRatio[j][i] = pstDrvCtx->stWDRCfg.au32ExpRatio[j];
            }
        }
        hi_isp_frame_stitch_short_exposure_ratio_write(IspDev, pstDrvCtx->stWDRCfg.au32ExpRatio[0]);
        hi_isp_frame_stitch_med_exposure_ratio_write(IspDev, pstDrvCtx->stWDRCfg.au32ExpRatio[1]);
        hi_isp_frame_stitch_long_exposure_ratio_write(IspDev, pstDrvCtx->stWDRCfg.au32ExpRatio[2]);

        /* drc_comp config */
        for (i=0; i<CFG2VLD_DLY_LIMIT; i++)
        {
            pstSyncCfg->u32DRCComp[i] = (1 << DRC_COMP_SHIFT);
        }
        hi_isp_drc_collection_correction_write(IspDev, pstSyncCfg->u32DRCComp[0]);

        /* isp_dgain config */
        //hi_isp_digital_gain_gain_write(u32IspDgain);

        /* drc_strength config */
        //hi_isp_drc_strength_inroi_write(u32DRCStrength);
        //hi_isp_drc_strength_outroi_write(u32DRCStrength);
    }

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_GetSyncControlnfo(ISP_DEV IspDev, ISP_SYNC_CFG_S *pstSyncCfg)
{
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = NULL;

    /* get VC number form logic */
    // pstSyncCfg->u8VCNum++;
    // pstSyncCfg->u8VCNum = pstSyncCfg->u8VCNum % DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);
    pstSyncCfg->u8VCNum = (IO_RD_ISP_ADDRESS(IspDev, VI_PT0_ID) & 0x30) >> 4;
    if (0 == pstSyncCfg->u8VCNumMax)
    {
        pstSyncCfg->u8VCNum = 0;
    }

    if (pstSyncCfg->u8VCNum > pstSyncCfg->u8VCNumMax)
    {
        ISP_TRACE(HI_DBG_ERR, "err VC number(%d), can't be large than VC total(%d)!\n", pstSyncCfg->u8VCNum, pstSyncCfg->u8VCNumMax);
    }

    /* get Cfg2VldDlyMAX */
    if (!ISPSyncBufIsEmpty(&pstSyncCfg->stSyncCfgBuf))
    {
        pstCurNode = &pstSyncCfg->stSyncCfgBuf.stSyncCfgBufNode[pstSyncCfg->stSyncCfgBuf.u8BufRDFlag];
        if (pstCurNode != HI_NULL)
        {
            if (pstCurNode->bValid)
            {
                pstSyncCfg->u8Cfg2VldDlyMAX = pstCurNode->stSnsRegsInfo.u8Cfg2ValidDelayMax;
            }
        }
    }

    if ((pstSyncCfg->u8Cfg2VldDlyMAX > CFG2VLD_DLY_LIMIT) ||(pstSyncCfg->u8Cfg2VldDlyMAX < 1))
    {
        ISP_TRACE(HI_DBG_WARN, "Delay of config to valid is:0x%x\n", pstSyncCfg->u8Cfg2VldDlyMAX);
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;
    }

    /* calc VCCfgNum =  (Cfg2VldDlyMAX * (VCNumMax + 1) + VCNum - Cfg2VldDlyMAX) % (VCNumMax + 1) */
    pstSyncCfg->u8VCCfgNum = (pstSyncCfg->u8VCNum + pstSyncCfg->u8VCNumMax * pstSyncCfg->u8Cfg2VldDlyMAX) % DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_CalcSyncCfg(ISP_SYNC_CFG_S *pstSyncCfg)
{
    HI_U32 i,j;
    HI_U64 au64Exp[4] = {0};
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreNode = HI_NULL;
    HI_U64 u64CurExp = 0;
    HI_U64 u64PreExp = 0;
    HI_U64 u64Ratio = 0;
    HI_U64 u64CurSnsGain = 0, u64PreSnsGain = 0;
	HI_U64 u32CurDrcGain = 0, u32PreDrcGain = 0;
    HI_BOOL bErr = HI_FALSE;
    HI_U64 u64Tmp = 0;
    HI_U32 u32WDRFrmNum = 0;
    HI_U8 u8WDRMode = 0;

    u8WDRMode = pstSyncCfg->u8WDRMode;
    u32WDRFrmNum = (IS_2to1_WDR_MODE(u8WDRMode)) ? 1 : \
        (IS_3to1_WDR_MODE(u8WDRMode)) ? 2 : \
        (IS_4to1_WDR_MODE(u8WDRMode)) ? 3 :0;

    /* update node when VCCfgNum is 0 */
    if (pstSyncCfg->u8VCCfgNum == 0)
    {
        for (i=CFG2VLD_DLY_LIMIT; i>=1; i--)
        {
            pstSyncCfg->apstNode[i] = pstSyncCfg->apstNode[i-1];
        }

        //ISPSyncBufRead(&pstSyncCfg->stSyncCfgBuf, &pstSyncCfg->apstNode[0]);

        /* avoid skip effective AE results */
        if (ISPSyncBufIsErr(&pstSyncCfg->stSyncCfgBuf))
        {
            bErr = HI_TRUE;
        }

        /* read the newest information */
        ISPSyncBufRead2(&pstSyncCfg->stSyncCfgBuf, &pstSyncCfg->apstNode[0]);
    }

    pstCurNode = pstSyncCfg->apstNode[0];
    if (HI_NULL == pstCurNode)
    {
        return HI_SUCCESS;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        return HI_SUCCESS;
    }

    if (HI_TRUE == bErr)
    {
        if (ISP_SNS_I2C_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
            {
                pstCurNode->stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
            }
        }
        else if (ISP_SNS_SSP_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
            {
                pstCurNode->stSnsRegsInfo.astSspData[i].bUpdate = HI_TRUE;
            }
        }
        else
        {
            /* do nothing */
        }

        pstCurNode->stSnsRegsInfo.stSlvSync.bUpdate = HI_TRUE;
    }

    if (HI_NULL == pstSyncCfg->apstNode[1])
    {
        pstPreNode = pstSyncCfg->apstNode[0];
    }
    else
    {
        if (HI_FALSE == pstSyncCfg->apstNode[1]->bValid)
        {
            pstPreNode = pstSyncCfg->apstNode[0];
        }
        else
        {
            pstPreNode = pstSyncCfg->apstNode[1];
        }
    }

    if (pstSyncCfg->u8VCCfgNum == 0)
    {
        u64PreSnsGain = pstPreNode->stAERegCfg.u64Exposure;
        u64PreSnsGain = osal_do_div64(u64PreSnsGain, DIV_0_TO_1(pstPreNode->stAERegCfg.u32IntTime[0]));
        u64PreSnsGain = u64PreSnsGain << 8;
        u64PreSnsGain = osal_do_div64(u64PreSnsGain, DIV_0_TO_1(pstPreNode->stAERegCfg.u32IspDgain));
        u64PreSnsGain = u64PreSnsGain << 8;
        u64PreSnsGain = osal_do_div64(u64PreSnsGain, DIV_0_TO_1(pstPreNode->stAERegCfg.u32WDRGain[u32WDRFrmNum]));

        u64CurSnsGain = pstCurNode->stAERegCfg.u64Exposure;
        u64CurSnsGain = osal_do_div64(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[0]));
        u64CurSnsGain = u64CurSnsGain << 8;
        u64CurSnsGain = osal_do_div64(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32IspDgain));
        u64CurSnsGain = u64CurSnsGain << 8;
        u64CurSnsGain = osal_do_div64(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32WDRGain[u32WDRFrmNum]));

        pstSyncCfg->u64PreSnsGain = u64PreSnsGain;
        pstSyncCfg->u64CurSnsGain = u64CurSnsGain;
    }

    u64PreSnsGain = pstSyncCfg->u64PreSnsGain;
    u64CurSnsGain = pstSyncCfg->u64CurSnsGain;

    /* calculate exposure ratio */
    if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode))
    {
        switch(pstSyncCfg->u8VCCfgNum)
        {
            case 0:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * u64CurSnsGain;
                au64Exp[1] = pstPreNode->stAERegCfg.u32IntTime[1] * u64PreSnsGain;
                au64Exp[2] = pstPreNode->stAERegCfg.u32IntTime[2] * u64PreSnsGain;
                au64Exp[3] = pstPreNode->stAERegCfg.u32IntTime[3] * u64PreSnsGain;
                break;
            case 1:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * u64CurSnsGain;
                au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * u64CurSnsGain;
                au64Exp[2] = pstPreNode->stAERegCfg.u32IntTime[2] * u64PreSnsGain;
                au64Exp[3] = pstPreNode->stAERegCfg.u32IntTime[3] * u64PreSnsGain;
                break;
            case 2:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * u64CurSnsGain;
                au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * u64CurSnsGain;
                au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * u64CurSnsGain;
                au64Exp[3] = pstPreNode->stAERegCfg.u32IntTime[3] * u64PreSnsGain;
                break;
            case 3:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * u64CurSnsGain;
                au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * u64CurSnsGain;
                au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * u64CurSnsGain;
                au64Exp[3] = pstCurNode->stAERegCfg.u32IntTime[3] * u64CurSnsGain;
                break;
        }
    }
    else
    {
        au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * u64CurSnsGain;
        au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * u64CurSnsGain;
        au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * u64CurSnsGain;
        au64Exp[3] = pstCurNode->stAERegCfg.u32IntTime[3] * u64CurSnsGain;
    }
#if 0
    if((IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) && (ISP_FSWDR_LONG_FRAME_MODE == pstCurNode->stAERegCfg.enFSWDRMode))
    {
        for (j=0; j<3; j++)
        {
            for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
            {
                pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i-1];
            }
            pstSyncCfg->u32ExpRatio[j][0] = 0x40;
        }
    }
    else
    {
        for (j=0; j<3; j++)
        {
            for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
            {
                pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i-1];
            }

            u64Ratio = au64Exp[j+1] << WDR_EXP_RATIO_SHIFT;
            u64Tmp = DIV_0_TO_1(au64Exp[j]);
            while (u64Tmp > (0x1LL << 31))
            {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }
            u64Ratio= osal_do_div64(u64Ratio, u64Tmp);
            pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
        }
    }

    /* calculate DRC compensation */
    u64CurExp = pstCurNode->stAERegCfg.u64Exposure;
    u64PreExp = pstPreNode->stAERegCfg.u64Exposure;
#else
	if((IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) && (ISP_FSWDR_LONG_FRAME_MODE == pstCurNode->stAERegCfg.enFSWDRMode))
    {
 	   for (j=0; j<3; j++)
 	   {
 		   for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
 		   {
 			   pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i-1];
			   pstSyncCfg->u32WDRGain[j][i] = pstSyncCfg->u32WDRGain[j][i-1];
 		   }
 		   pstSyncCfg->u32ExpRatio[j][0] = 0x40;
		   pstSyncCfg->u32WDRGain[j][0] = 0x100;
 	   }

 	   for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
 	   {
 		   pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i-1];
 	   }
 	   pstSyncCfg->u8LFMode[0] = 1;
 	   pstSyncCfg->u32WDRGain[3][0] = 0x100;
    }
    else if((IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) && (ISP_FSWDR_AUTO_LONG_FRAME_MODE == pstCurNode->stAERegCfg.enFSWDRMode))
    {
 	   for (j=0; j<3; j++)
 	   {
 		   for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
 		   {
 			   pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i-1];
			   pstSyncCfg->u32WDRGain[j][i] = pstSyncCfg->u32WDRGain[j][i-1];
 		   }
		   u64Ratio = au64Exp[j+1];
 		   u64Tmp = au64Exp[j];
		   u64Tmp = DIV_0_TO_1(u64Tmp);
		   while (u64Ratio > (0x1LL << 31) || u64Tmp > (0x1LL << 31))
           {
               u64Tmp >>= 1;
               u64Ratio >>= 1;
           }
		   u64Ratio = ((u64Ratio * pstCurNode->stAERegCfg.u32WDRGain[j+1]) >> 8) << WDR_EXP_RATIO_SHIFT;
 		   u64Tmp = (u64Tmp * pstCurNode->stAERegCfg.u32WDRGain[j]) >> 8;
 		   while (u64Tmp > (0x1LL << 31))
 		   {
 			   u64Tmp >>= 1;
 			   u64Ratio >>= 1;
 		   }

 		   u64Ratio = osal_do_div64(u64Ratio, DIV_0_TO_1(u64Tmp));
		   u64Ratio = u64Ratio < 0x45? 0x40 : u64Ratio;
 		   pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
		   pstSyncCfg->u32WDRGain[j][0] = pstCurNode->stAERegCfg.u32WDRGain[j];
 	   }
 	   for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
 	   {
 		   pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i-1];
		   pstSyncCfg->u32WDRGain[3][i] = pstSyncCfg->u32WDRGain[3][i-1];
 	   }
 	   pstSyncCfg->u8LFMode[0] = 2;
	   pstSyncCfg->u32WDRGain[3][0] = 0x100;

    }
    else
    {
 	   for (j=0; j<3; j++)
 	   {
 		   for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
 		   {
 			   pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i-1];
			   pstSyncCfg->u32WDRGain[j][i] = pstSyncCfg->u32WDRGain[j][i-1];
 		   }
		   u64Ratio = au64Exp[j+1];
 		   u64Tmp = au64Exp[j];
		   u64Tmp = DIV_0_TO_1(u64Tmp);
		   while (u64Ratio > (0x1LL << 31) || u64Tmp > (0x1LL << 31))
           {
               u64Tmp >>= 1;
               u64Ratio >>= 1;
           }
		   u64Ratio = ((u64Ratio * pstCurNode->stAERegCfg.u32WDRGain[j+1]) >> 8) << WDR_EXP_RATIO_SHIFT;
 		   u64Tmp = (u64Tmp * pstCurNode->stAERegCfg.u32WDRGain[j]) >> 8;
 		   while (u64Tmp > (0x1LL << 31))
 		   {
 			   u64Tmp >>= 1;
 			   u64Ratio >>= 1;
 		   }

 		   u64Ratio = osal_do_div64(u64Ratio, DIV_0_TO_1(u64Tmp));
 		   pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
		   pstSyncCfg->u32WDRGain[j][0] = pstCurNode->stAERegCfg.u32WDRGain[j];
 	   }
 	   for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
 	   {
 		   pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i-1];
		   pstSyncCfg->u32WDRGain[3][i] = pstSyncCfg->u32WDRGain[3][i-1];
 	   }
 	   pstSyncCfg->u8LFMode[0] = 0;
 	   pstSyncCfg->u32WDRGain[3][0] = 0x100;
    }

	/* calculate DRC compensation */
	u64CurExp = pstCurNode->stAERegCfg.u32IntTime[0] * u64CurSnsGain;;
    u64PreExp = pstPreNode->stAERegCfg.u32IntTime[0] * u64PreSnsGain;;
	while (u64CurExp > (0x1LL << 31) || u64PreExp > (0x1LL << 31))
    {
        u64CurExp >>= 1;
        u64PreExp >>= 1;
    }

    u32CurDrcGain = CLIP3(pstCurNode->stAERegCfg.u32WDRGain[0], ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    u32CurDrcGain = ((HI_U64)u32CurDrcGain * CLIP3(pstCurNode->stAERegCfg.u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX)) >> 8;
    u32PreDrcGain = CLIP3(pstPreNode->stAERegCfg.u32WDRGain[0], ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    u32PreDrcGain = ((HI_U64)u32PreDrcGain * CLIP3(pstPreNode->stAERegCfg.u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX)) >> 8;
    u64CurExp = u64CurExp * u32CurDrcGain;
    u64PreExp = u64PreExp * u32PreDrcGain;
#endif

    for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
    {
        pstSyncCfg->u32DRCComp[i] = pstSyncCfg->u32DRCComp[i-1];
    }
    u64CurExp = u64CurExp << DRC_COMP_SHIFT;
    u64Tmp = DIV_0_TO_1(u64PreExp);
    while (u64Tmp > (0x1LL << 31))
    {
        u64Tmp >>= 1;
        u64CurExp >>= 1;
    }
    u64CurExp = osal_do_div64(u64CurExp, DIV_0_TO_1(u64Tmp));
    pstSyncCfg->u32DRCComp[0] = (HI_U32)u64CurExp;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigIsp(ISP_DEV IspDev, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32  i;
    HI_U8   u8CfgNodeIdx, u8CfgNodeVC;
    HI_U32  u32IspDgain;
    HI_U32  u32DRCStrength;
    HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};
    HI_U32  u32Ratio[3] = {0x40, 0x40, 0x40};
    HI_U32  u32DrcComp = 0x100;
    HI_U32  u32DrcCompIndex = 2;
	HI_U8   u8LFMode = 0;
	HI_U32  au32WDRGain[4] = {0x100, 0x100, 0x100, 0x100};

    ISP_AE_REG_CFG_2_S *pstAERegCfg = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_DRC_REG_CFG_S *pstDRCRegCfg = HI_NULL;

    /* config Chn Switch / Exp Ratio / ISP Dgain */
        /* delay of config2valid of isp reg is 1 */
    if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        /* Ratio */
        for (i=0; i<3; i++)
        {
            u32Ratio[i] = pstDrvCtx->stSyncCfg.u32ExpRatio[i][pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
        }

        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            if (((IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) && (0x40 == u32Ratio[0])) ||
                ((IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) && (0x40 == u32Ratio[0]) && (0x40 == u32Ratio[1])) ||
                ((IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) && (0x40 == u32Ratio[0]) && (0x40 == u32Ratio[1]) && (0x40 == u32Ratio[2])))    /* ExpRatio == 1 */
            {
                /* do NOT switch channel, ouput every frame */
            }
            else
            {
                /* Channel Switch */
                au32ChnSwitch[0] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                bChnSwitchEnable = HI_TRUE;

                hi_vi_top_chn_switch0_write(IspDev, au32ChnSwitch[0]);
                hi_vi_top_chn_switch1_write(IspDev, au32ChnSwitch[1]);
                hi_vi_top_chn_switch2_write(IspDev, au32ChnSwitch[2]);
                hi_vi_top_chn_switch3_write(IspDev, au32ChnSwitch[3]);
                hi_vi_top_chn_switch_enble_write(IspDev, bChnSwitchEnable);
            }
        }

        /*  */
        u8CfgNodeIdx = ((pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
        u8CfgNodeVC = ((pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
    }
    else
    {
        u8CfgNodeIdx = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1);
        u8CfgNodeVC = 0;
    }

    /* when the data of sensor built-in WDR after decompand is 16bit, the ratio value is as follow. */
    if (IS_BUILT_IN_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        u32Ratio[0] = BUILT_IN_WDR_RATIO_VS_S;
        u32Ratio[1] = BUILT_IN_WDR_RATIO_S_M;
        u32Ratio[2] = BUILT_IN_WDR_RATIO_M_L;
    }

    for (i=0; i<3; i++)
    {
        u32Ratio[i] = (u32Ratio[i] > EXP_RATIO_MAX) ? EXP_RATIO_MAX : u32Ratio[i];
        u32Ratio[i] = (u32Ratio[i] < EXP_RATIO_MIN) ? EXP_RATIO_MIN : u32Ratio[i];
    }

    hi_isp_frame_stitch_short_exposure_ratio_write(IspDev, u32Ratio[0]);
    hi_isp_frame_stitch_med_exposure_ratio_write(IspDev, u32Ratio[1]);
    hi_isp_frame_stitch_long_exposure_ratio_write(IspDev, u32Ratio[2]);

    /* config drc compensation:
        if the parameter is changed prior to frame X,
        then it will compensate the statistics collected in frame X,
        and the results will be applied and seen in frame X+1 */
    u32DrcCompIndex = pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX;
    u32DrcCompIndex = (u32DrcCompIndex > 2) ? (u32DrcCompIndex - 2) : 0;
    u32DrcComp = pstDrvCtx->stSyncCfg.u32DRCComp[u32DrcCompIndex];
    u32DrcComp = MIN2(u32DrcComp, 0xFFF);
    if ((ISP_SNAP_PICTURE == pstDrvCtx->stSnapAttr.enSnapPipeMode) && (SNAP_TYPE_NONE != pstDrvCtx->stSnapAttr.enSnapType))
    {
        u32DrcComp = 0x100;
    }
    hi_isp_drc_collection_correction_write(IspDev, u32DrcComp);

    /* config isp_dgain & drc strength */
    if (SNAP_STATE_CFG == pstDrvCtx->stCapInfoLoad.enCapState)
    {
        pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[0];
    }
    else
    {
        pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
    }
    //pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
    if (HI_NULL == pstCfgNode)
    {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
    {
        pstAERegCfg = &pstCfgNode->stAERegCfg;
        pstDRCRegCfg = &pstCfgNode->stDRCRegCfg;
    }

    if ((HI_NULL == pstAERegCfg) || (HI_NULL == pstDRCRegCfg))
    {
        return HI_SUCCESS;
    }

    u32IspDgain = pstAERegCfg->u32IspDgain;
	u32IspDgain = CLIP3(u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
	u8LFMode = pstDrvCtx->stSyncCfg.u8LFMode[pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
	for(i = 0; i < 4; i++)
	{
		au32WDRGain[i] = pstDrvCtx->stSyncCfg.u32WDRGain[i][pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
	    au32WDRGain[i] = CLIP3(au32WDRGain[i], ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
	}

    if(IS_FS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        hi_isp_frame_stitch_gain_gain_wdr1_write(IspDev, au32WDRGain[0]);
        hi_isp_frame_stitch_gain_gain_wdr2_write(IspDev, au32WDRGain[1]);
        hi_isp_frame_stitch_gain_gain_wdr3_write(IspDev, au32WDRGain[2]);
        hi_isp_frame_stitch_gain_gain_wdr4_write(IspDev, au32WDRGain[3]);
        hi_isp_digital_gain_gain_write(IspDev, u32IspDgain);
    }
    else
    {
        if (SNAP_STATE_CFG == pstDrvCtx->stCapInfoLoad.enCapState)
        {
            hi_isp_digital_gain_gain_write(IspDev, pstDrvCtx->stCapInfoLoad.u32IspDgain);
            if (1 == update_pos)
            {
                /*isp current frame end intterupter and next frame start intterupter arrived at the same time,
                  enCapstate should not be clear*/
                if (3 != pstDrvCtx->stIntSch.u32IspIntStatus)
                {
                    pstDrvCtx->stCapInfoLoad.enCapState = SNAP_STATE_NULL;
                }
            }
        }
        else
        {
            hi_isp_frame_stitch_gain_gain_wdr1_write(IspDev, ISP_DIGITAL_GAIN_MIN);
            hi_isp_frame_stitch_gain_gain_wdr2_write(IspDev, ISP_DIGITAL_GAIN_MIN);
            hi_isp_frame_stitch_gain_gain_wdr3_write(IspDev, ISP_DIGITAL_GAIN_MIN);
            hi_isp_frame_stitch_gain_gain_wdr4_write(IspDev, ISP_DIGITAL_GAIN_MIN);

            if (((SNAP_TYPE_SYNC_PRO == pstDrvCtx->stSnapAttr.enSnapType) || (SNAP_TYPE_SYNC == pstDrvCtx->stSnapAttr.enSnapType)) && (ISP_SNAP_PREVIEW == pstDrvCtx->stSnapAttr.enSnapPipeMode))
            {
                for (i=0; i<ISP_MAX_DEV_NUM; i++)
                {
                    hi_isp_digital_gain_gain_write(i, u32IspDgain);
                }
            }
            else if ((ISP_SNAP_PICTURE != pstDrvCtx->stSnapAttr.enSnapPipeMode) || ((SNAP_TYPE_SYNC != pstDrvCtx->stSnapAttr.enSnapType) && (SNAP_TYPE_SYNC_PRO != pstDrvCtx->stSnapAttr.enSnapType)))
            {
                hi_isp_digital_gain_gain_write(IspDev, u32IspDgain);
            }
        }
    }

    //u32DRCStrength = CLIP3(pstDRCRegCfg->u32Strength, DRC_STRENGTH_MIN, DRC_STRENGTH_MAX);
    if (((SNAP_TYPE_SYNC_PRO == pstDrvCtx->stSnapAttr.enSnapType) || (SNAP_TYPE_SYNC == pstDrvCtx->stSnapAttr.enSnapType)) && (ISP_SNAP_PREVIEW == pstDrvCtx->stSnapAttr.enSnapPipeMode))
    {
        for (i=0; i<ISP_MAX_DEV_NUM; i++)
        {
            u32DRCStrength = MIN2(pstDRCRegCfg->u32Strength, DRC_STRENGTH_MAX);
            hi_isp_drc_strength_inroi_write(i, u32DRCStrength);
            hi_isp_drc_strength_outroi_write(i, u32DRCStrength);
            u32DrcComp = 0x100;
            hi_isp_drc_collection_correction_write(i, u32DrcComp);
        }
    }
    else if ((ISP_SNAP_PICTURE != pstDrvCtx->stSnapAttr.enSnapPipeMode) || ((SNAP_TYPE_SYNC != pstDrvCtx->stSnapAttr.enSnapType) && (SNAP_TYPE_SYNC_PRO != pstDrvCtx->stSnapAttr.enSnapType)))
    {
        u32DRCStrength = MIN2(pstDRCRegCfg->u32Strength, DRC_STRENGTH_MAX);
        hi_isp_drc_strength_inroi_write(IspDev, u32DRCStrength);
        hi_isp_drc_strength_outroi_write(IspDev, u32DRCStrength);
    }

	pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[0];

	if (HI_NULL == pstCfgNode)
    {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
    {
        pstAERegCfg = &pstCfgNode->stAERegCfg;
    }

	if (HI_NULL == pstAERegCfg)
    {
        return HI_SUCCESS;
    }

    /* config Piris */
    if(pstAERegCfg->bPirisValid == HI_TRUE)
    {
        if (HI_NULL != pstDrvCtx->stPirisCb.pfn_piris_gpio_update)
        {
            pstDrvCtx->stPirisCb.pfn_piris_gpio_update(IspDev, &pstAERegCfg->s32PirisPos);
        }
    }

    return HI_SUCCESS;

}

ISP_SYNC_CFG_BUF_NODE_S *ISP_DRV_GetSnsRegConfigNode(ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8DelayFrmNum)
{
    HI_U8 u8WDRMode = WDR_MODE_NONE;
    HI_U8 u8CfgNodeIdx = 0, u8CfgNodeVC = 0;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreCfgNode = HI_NULL;

    u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;

    u8CfgNodeIdx = u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
    u8CfgNodeVC =  u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
    {
        if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT-1)
        {
            ISP_TRACE(HI_DBG_WARN, "DelayFrmNum error!\n");
            return HI_NULL;
        }

        pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
        pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

        if (HI_NULL == pstCfgNode)
        {
            return HI_NULL;
        }

        if (HI_NULL == pstPreCfgNode)
        {
        }
        else
        {
            if ((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode)))
            {
                /* not config sensor when cur == pre */
                if (pstCfgNode == pstPreCfgNode)
                {
                    return HI_NULL;
                }
            }
        }
    }

    return pstCfgNode;
}


HI_S32 ISP_DRV_RegConfigSensor(ISP_DEV IspDev, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i = 0;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_I2C_DATA_S *pstI2cData = HI_NULL;
    ISP_SSP_DATA_S *pstSspData = HI_NULL;
    HI_S8 s8I2cDev, s8SspDev, s8SspCs;

    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (HI_NULL == pstCurNode)
    {
        ISP_TRACE(HI_DBG_WARN, "NULL point!\n");
        return HI_FAILURE;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        ISP_TRACE(HI_DBG_WARN, "Invalid Node!\n");
        return HI_FAILURE;
    }

    if (0 == pstCurNode->stSnsRegsInfo.u32RegNum)
    {
        ISP_TRACE(HI_DBG_WARN, "Err u32RegNum!\n");
        return HI_FAILURE;
    }


    if (ISP_SNS_I2C_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
    {
        for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
        {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum);

            if (!pstCfgNode)
            {
                continue;
            }

			s8I2cDev = pstCfgNode->stSnsRegsInfo.unComBus.s8I2cDev;
	        if (-1 == s8I2cDev)
	        {
	            return 0;
	        }

            if (HI_TRUE == pstCfgNode->bValid)
            {
                pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];
                if ((pstI2cData->bUpdate == HI_TRUE) && (pstDrvCtx->u32IntPos == pstI2cData->u8IntPos))
                {
                    if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteI2CData)
                    {
                            pstDrvCtx->stBusCb.pfnISPWriteI2CData(s8I2cDev, pstI2cData->u8DevAddr,
                                pstI2cData->u32RegAddr, pstI2cData->u32AddrByteNum,
                                pstI2cData->u32Data, pstI2cData->u32DataByteNum);
                    }
                }
            }
        }
    }
    else if (ISP_SNS_SSP_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
    {
        for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
        {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum);

            if (!pstCfgNode)
            {
                continue;
            }

			s8SspDev = pstCfgNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspDev;

	        if (-1 == s8SspDev)
	        {
	            return 0;
	        }

            s8SspDev = pstCfgNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspDev;
            s8SspCs = pstCfgNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspCs;

            if (HI_TRUE == pstCfgNode->bValid)
            {
                pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];
                if ((pstSspData->bUpdate == HI_TRUE) && (pstDrvCtx->u32IntPos == pstSspData->u8IntPos))
                {
                    if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteSSPData)
                    {
                            pstDrvCtx->stBusCb.pfnISPWriteSSPData(s8SspDev, s8SspCs, pstSspData->u32DevAddr,
                                pstSspData->u32DevAddrByteNum, pstSspData->u32RegAddr,
                                pstSspData->u32RegAddrByteNum, pstSspData->u32Data,
                                pstSspData->u32DataByteNum);
                    }
                }
            }
        }
    }

    /* write slave sns vmax sync*/
    pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.stSlvSync.u8DelayFrmNum);

    if (HI_NULL == pstCfgNode)
    {
        return HI_FAILURE;
    }

    if ((HI_TRUE == pstCfgNode->bValid) && (HI_TRUE == pstCfgNode->stSnsRegsInfo.stSlvSync.bUpdate))
    {
        hi_isp_slave_mode_vstime_write(IspDev, pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveVsTime);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegisterBusCallBack (ISP_DEV IspDev,
    ISP_BUS_TYPE_E enType, ISP_BUS_CALLBACK_S *pstBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstBusCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    if (ISP_BUS_TYPE_I2C == enType)
    {
        pstDrvCtx->stBusCb.pfnISPWriteI2CData = pstBusCb->pfnISPWriteI2CData;
    }
    else if (ISP_BUS_TYPE_SSP == enType)
    {
        pstDrvCtx->stBusCb.pfnISPWriteSSPData = pstBusCb->pfnISPWriteSSPData;
    }
    else
    {
        osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
        osal_printk("The bus type %d registerd to isp is err!", enType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
    return HI_SUCCESS;
}

HI_S32 ISP_RegisterPirisCallBack (ISP_DEV IspDev, ISP_PIRIS_CALLBACK_S *pstPirisCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstPirisCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->stPirisCb.pfn_piris_gpio_update = pstPirisCb->pfn_piris_gpio_update;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_RegisterSnapCallBack (ISP_DEV IspDev, ISP_SNAP_CALLBACK_S *pstSnapCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    ISP_CHECK_POINTER(pstSnapCb);
    ISP_CHECK_DEV(IspDev);
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
    pstDrvCtx->stSnapCb.pfnSnapIspInfoUpdate = pstSnapCb->pfnSnapIspInfoUpdate;
    osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
    return HI_SUCCESS;
}
/* vi get isp_config from isp*/
HI_S32 ISP_SaveCaptureConfig(ISP_DEV IspDev, ISP_CONFIG_INFO_S *pstCapInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

	if (0 == update_pos) /* frame start */
    {
        osal_memcpy(pstCapInfo, &pstDrvCtx->stCapInfoLastSave, sizeof(ISP_CONFIG_INFO_S));
    }
    else                 /* frame end */
    {
        osal_memcpy(pstCapInfo, &pstDrvCtx->stCapInfoSave, sizeof(ISP_CONFIG_INFO_S));
    }

    return HI_SUCCESS;
}

/* vi send isp_config to isp*/
HI_S32 ISP_LoadCaptureConfig(ISP_DEV IspDev, ISP_CONFIG_INFO_S *pstCapInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    osal_memcpy(&pstDrvCtx->stCapInfoLoad, pstCapInfo, sizeof(ISP_CONFIG_INFO_S));
    pstDrvCtx->stCapInfoLoad.enCapState = SNAP_STATE_CFG;

    return HI_SUCCESS;
}

/* vi send Proenable*/
HI_S32 ISP_SetProEnable(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    pstDrvCtx->bProEnable = HI_TRUE;
    //pstDrvCtx->bProStart = HI_FALSE;
    return HI_SUCCESS;
}

/* vi set ProCtrl*/
HI_S32 ISP_SetProCtrl(ISP_DEV IspDev, HI_BOOL bProEnd)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

	pstDrvCtx->bProStart = bProEnd;

    return HI_SUCCESS;

}

/* vi get ProCtrl*/
HI_BOOL ISP_GetProCtrl(ISP_DEV IspDev, ISP_PRO_CTRL_S *pstProCtrl)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

	if (0 == update_pos)
	{
		pstProCtrl->u8Vcnum = 2;
	}
	else
	{
		pstProCtrl->u8Vcnum = 1;
	}
    osal_memcpy(&pstProCtrl->stProParam, &pstDrvCtx->stSnapAttr.stProParam, sizeof(ISP_PRO_PARAM_S));
    pstProCtrl->u32ExpTime = pstDrvCtx->stCapInfoSave.u32ExposureTime;
    pstProCtrl->u32SysGain = (pstDrvCtx->stCapInfoSave.u32Iso<<10)/100;

    return pstDrvCtx->bProStart;
}

HI_S32 ISP_DRV_ProcInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    char acProcName[16] = "proc_";

    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    osal_strcat(acProcName, pstDrvCtx->acName);
    s32Ret = CMPI_MmzMallocCached(HI_NULL, acProcName, &u32PhyAddr, (HI_VOID**)&pu8VirAddr, ISP_PROC_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc proc buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    ((HI_CHAR *)pu8VirAddr)[0] = '\0';
    ((HI_CHAR *)pu8VirAddr)[ISP_PROC_SIZE-1] = '\0';

    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->stPorcMem.u32ProcPhyAddr = u32PhyAddr;
    pstDrvCtx->stPorcMem.u32ProcSize = ISP_PROC_SIZE;
    pstDrvCtx->stPorcMem.pProcVirtAddr = pu8VirAddr;
    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;

    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }

    ISP_CHECK_DEV(IspDev);
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->stPorcMem.u32ProcPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stPorcMem.pProcVirtAddr;
    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->stPorcMem.u32ProcPhyAddr = 0;
    pstDrvCtx->stPorcMem.u32ProcSize = 0;
    pstDrvCtx->stPorcMem.pProcVirtAddr = HI_NULL;
    osal_up(&pstDrvCtx->stProcSem);


    if (0 != u32PhyAddr)
    {
        hil_mmb_invalid_cache_byaddr(pu8VirAddr, u32PhyAddr, ISP_PROC_SIZE);
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcPrintf(ISP_DEV IspDev, osal_proc_entry_t *s)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32ProcBufLen;
    const HI_CHAR *pszStr;
    HI_CHAR *pszBuf;

    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }

    //ISP_CHECK_DEV(IspDev);    /* don't defensive check */
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    if (HI_NULL != pstDrvCtx->stPorcMem.pProcVirtAddr)
    {
        pszBuf = osal_kmalloc(PROC_PRT_SLICE_SIZE+1,osal_gfp_kernel);
        if (!pszBuf)
        {
            ISP_TRACE(HI_DBG_ERR, "ISP_DRV_ProcPrintf malloc slice buf err\n");
            osal_up(&pstDrvCtx->stProcSem);
            return HI_ERR_ISP_NULL_PTR;
        }
        pszBuf[PROC_PRT_SLICE_SIZE] = '\0';
        pszStr = (HI_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr;
        u32ProcBufLen = osal_strlen((HI_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr);

        while (u32ProcBufLen)
        {
            osal_strncpy(pszBuf, pszStr, PROC_PRT_SLICE_SIZE);
            osal_seq_printf(s, "%s", pszBuf);
            pszStr += PROC_PRT_SLICE_SIZE;
            if (u32ProcBufLen < PROC_PRT_SLICE_SIZE)
            {
                u32ProcBufLen = 0;
            }
            else
            {
                u32ProcBufLen -= PROC_PRT_SLICE_SIZE;
            }
        }

        osal_kfree((HI_VOID *)pszBuf);
    }

    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

#ifdef ENABLE_JPEGEDCF
HI_S32 ISP_GetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo = HI_NULL;
    ISP_DCF_CONST_INFO_S *pstIspDCFConstInfo = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (NULL == pstDrvCtx->pUpdateInfoVirAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "DCF buf don't init ok!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    if (0 == update_pos)
    {
        pstIspUpdateInfo = pstDrvCtx->pUpdateInfoVirAddr+1;
    }
    else
    {
        pstIspUpdateInfo = pstDrvCtx->pUpdateInfoVirAddr;
    }
    pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S *)(pstDrvCtx->pUpdateInfoVirAddr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(pstIspDCF, pstIspDCFConstInfo, sizeof(HI_U8)*DCF_DRSCRIPTION_LENGTH*4);


    pstIspDCF->u8LightSource            = pstIspDCFConstInfo->u8LightSource;
    pstIspDCF->u32FocalLength           = pstIspDCFConstInfo->u32FocalLength;
    pstIspDCF->u8SceneType              = pstIspDCFConstInfo->u8SceneType;
    pstIspDCF->u8CustomRendered         = pstIspDCFConstInfo->u8CustomRendered;
    pstIspDCF->u8FocalLengthIn35mmFilm  = pstIspDCFConstInfo->u8FocalLengthIn35mmFilm;
    pstIspDCF->u8SceneCaptureType       = pstIspDCFConstInfo->u8SceneCaptureType;
    pstIspDCF->u8GainControl            = pstIspDCFConstInfo->u8GainControl;
    pstIspDCF->u8Contrast               = pstIspDCFConstInfo->u8Contrast;
    pstIspDCF->u8Saturation             = pstIspDCFConstInfo->u8Saturation;
    pstIspDCF->u8Sharpness              = pstIspDCFConstInfo->u8Sharpness;
    pstIspDCF->u8MeteringMode           = pstIspDCFConstInfo->u8MeteringMode;

    pstIspDCF->u16ISOSpeedRatings       = pstIspUpdateInfo->u16ISOSpeedRatings;
    pstIspDCF->u32ExposureTime          = pstIspUpdateInfo->u32ExposureTime;
    pstIspDCF->u32ExposureBiasValue     = pstIspUpdateInfo->u32ExposureBiasValue;
    pstIspDCF->u8ExposureProgram        = pstIspUpdateInfo->u8ExposureProgram;
    pstIspDCF->u8ExposureMode           = pstIspUpdateInfo->u8ExposureMode;
    pstIspDCF->u32FNumber               = pstIspUpdateInfo->u32FNumber;
    pstIspDCF->u32MaxApertureValue      = pstIspUpdateInfo->u32MaxApertureValue;
    pstIspDCF->u8WhiteBalance           = pstIspUpdateInfo->u8WhiteBalance;

    return HI_SUCCESS;
}

HI_S32 ISP_SetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo = HI_NULL;
    ISP_DCF_CONST_INFO_S *pstIspDCFConstInfo = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (NULL == pstDrvCtx->pUpdateInfoVirAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "DCF Info buf don't init ok!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    if (0 == update_pos)
    {
        pstIspUpdateInfo = pstDrvCtx->pUpdateInfoVirAddr+1;
    }
    else
    {
        pstIspUpdateInfo = pstDrvCtx->pUpdateInfoVirAddr;
    }

    pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S *)(pstDrvCtx->pUpdateInfoVirAddr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(pstIspDCFConstInfo, pstIspDCF, sizeof(HI_U8)*DCF_DRSCRIPTION_LENGTH*4);

    pstIspDCFConstInfo->u8LightSource            = pstIspDCF->u8LightSource;
    pstIspDCFConstInfo->u32FocalLength           = pstIspDCF->u32FocalLength;
    pstIspDCFConstInfo->u8SceneType              = pstIspDCF->u8SceneType;
    pstIspDCFConstInfo->u8CustomRendered         = pstIspDCF->u8CustomRendered;
    pstIspDCFConstInfo->u8FocalLengthIn35mmFilm  = pstIspDCF->u8FocalLengthIn35mmFilm;
    pstIspDCFConstInfo->u8SceneCaptureType       = pstIspDCF->u8SceneCaptureType;
    pstIspDCFConstInfo->u8GainControl            = pstIspDCF->u8GainControl;
    pstIspDCFConstInfo->u8Contrast               = pstIspDCF->u8Contrast;
    pstIspDCFConstInfo->u8Saturation             = pstIspDCF->u8Saturation;
    pstIspDCFConstInfo->u8Sharpness              = pstIspDCF->u8Sharpness;
    pstIspDCFConstInfo->u8MeteringMode           = pstIspDCF->u8MeteringMode;

    pstIspUpdateInfo->u16ISOSpeedRatings       = pstIspDCF->u16ISOSpeedRatings;
    pstIspUpdateInfo->u32ExposureTime          = pstIspDCF->u32ExposureTime;
    pstIspUpdateInfo->u32ExposureBiasValue     = pstIspDCF->u32ExposureBiasValue;
    pstIspUpdateInfo->u8ExposureProgram        = pstIspDCF->u8ExposureProgram;
    pstIspUpdateInfo->u8ExposureMode           = pstIspDCF->u8ExposureMode;
    pstIspUpdateInfo->u32FNumber               = pstIspDCF->u32FNumber;
    pstIspUpdateInfo->u32MaxApertureValue      = pstIspDCF->u32MaxApertureValue;
    pstIspUpdateInfo->u8WhiteBalance           = pstIspDCF->u8WhiteBalance;

    return HI_SUCCESS;
}
#endif

HI_S32 ISP_GetIspUpdateInfo(ISP_DEV IspDev,ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_UPDATE_INFO_INIT(IspDev);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (NULL == pstDrvCtx->pUpdateInfoVirAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "UpdateInfo buf don't init ok!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    if (0 == update_pos) /* frame start */
    {
        osal_memcpy(pstIspUpdateInfo, pstDrvCtx->pUpdateInfoVirAddr+1, sizeof(ISP_DCF_UPDATE_INFO_S));
    }
    else
    {
        osal_memcpy(pstIspUpdateInfo, pstDrvCtx->pUpdateInfoVirAddr, sizeof(ISP_DCF_UPDATE_INFO_S));
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetIspUpdateInfo(ISP_DEV IspDev,ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_UPDATE_INFO_INIT(IspDev);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (NULL == pstDrvCtx->pUpdateInfoVirAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "UpdateInfo buf don't init ok!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    if (0 == update_pos) /* frame start */
    {
        osal_memcpy(pstDrvCtx->pUpdateInfoVirAddr+1, pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));
    }
    else
    {
        osal_memcpy(pstDrvCtx->pUpdateInfoVirAddr, pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateInfoBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(ISP_DCF_UPDATE_INFO_S) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(ISP_DCF_CONST_INFO_S);
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPUpdateInfo", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc UpdateInfo buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u32Size);

    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }

    pstDrvCtx->bUpdateInfoInit = HI_TRUE;
    pstDrvCtx->u32UpdateInfoPhyAddr = u32PhyAddr;
    pstDrvCtx->pUpdateInfoVirAddr = (ISP_DCF_UPDATE_INFO_S *)pu8VirAddr;

    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateInfoBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->u32UpdateInfoPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->pUpdateInfoVirAddr;
    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->pUpdateInfoVirAddr = HI_NULL;
    pstDrvCtx->u32UpdateInfoPhyAddr = 0;
    pstDrvCtx->bUpdateInfoInit = HI_FALSE;
    osal_up(&pstDrvCtx->stProcSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}



HI_S32 ISP_SetFrameInfo(ISP_DEV IspDev,FRAME_SUPPLEMENT_INFO_S *pstIspFrame)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_FRAME_INFO_INIT(IspDev);
    ISP_CHECK_POINTER(pstIspFrame);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    ISP_CHECK_POINTER(pstDrvCtx->pFrameInfoVirAddr);

    osal_memcpy(pstDrvCtx->pFrameInfoVirAddr,pstIspFrame,sizeof(FRAME_SUPPLEMENT_INFO_S));

    return HI_SUCCESS;
}

HI_S32 ISP_GetFrameInfo(ISP_DEV IspDev,FRAME_SUPPLEMENT_INFO_S *pstIspFrame)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_FRAME_INFO_INIT(IspDev);
    ISP_CHECK_POINTER(pstIspFrame);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    ISP_CHECK_POINTER(pstDrvCtx->pFrameInfoVirAddr);

    if (pstIspFrame && pstDrvCtx->pFrameInfoVirAddr)
    {
        if (0 == update_pos) /* frame start */
        {
            osal_memcpy(pstIspFrame,pstDrvCtx->pFrameInfoVirAddr + 1,sizeof(FRAME_SUPPLEMENT_INFO_S));
        }
        else
        {
            osal_memcpy(pstIspFrame,pstDrvCtx->pFrameInfoVirAddr,sizeof(FRAME_SUPPLEMENT_INFO_S));
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FrameInfoBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(FRAME_SUPPLEMENT_INFO_S) * ISP_MAX_FRAMEINFO_BUF_NUM;
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPFrameInfo", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc FrameInfo buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u32Size);

    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }

    pstDrvCtx->bFrameInfoInit = HI_TRUE;
    pstDrvCtx->u32FrameInfoPhyAddr = u32PhyAddr;
    pstDrvCtx->pFrameInfoVirAddr = (FRAME_SUPPLEMENT_INFO_S *)pu8VirAddr;

    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FrameInfoBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->u32FrameInfoPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->pFrameInfoVirAddr;
    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->pFrameInfoVirAddr = HI_NULL;
    pstDrvCtx->u32FrameInfoPhyAddr = 0;
    pstDrvCtx->bFrameInfoInit = HI_FALSE;
    osal_up(&pstDrvCtx->stProcSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DngInfoBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(DNG_IMAGE_STATIC_INFO_S);
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPDngInfo", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc DngInfo buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u32Size);

    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    
    pstDrvCtx->bDngInfoInit = HI_TRUE;
    pstDrvCtx->u32DngInfoPhyAddr = u32PhyAddr;
    pstDrvCtx->pDngInfoVirAddr = (DNG_IMAGE_STATIC_INFO_S *)pu8VirAddr;

    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DngInfoBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->u32DngInfoPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->pDngInfoVirAddr;
    if (osal_down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->pDngInfoVirAddr = HI_NULL;
    pstDrvCtx->u32DngInfoPhyAddr = 0;
    pstDrvCtx->bDngInfoInit = HI_FALSE;
    osal_up(&pstDrvCtx->stProcSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetDngInfo(ISP_DEV IspDev, DNG_IMAGE_STATIC_INFO_S *pstDngInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_DNG_INFO_INIT(IspDev);
    ISP_CHECK_POINTER(pstDngInfo);
    
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    ISP_CHECK_POINTER(pstDrvCtx->pDngInfoVirAddr);
    
    if (pstDngInfo && pstDrvCtx->pDngInfoVirAddr)
    {
        osal_memcpy(pstDngInfo, pstDrvCtx->pDngInfoVirAddr,sizeof(DNG_IMAGE_STATIC_INFO_S));
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetDngImageDynamicInfo(ISP_DEV IspDev, DNG_IMAGE_DYNAMIC_INFO_S *pstDngImageDynamicInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (0 == update_pos) /* frame start */
    {
        osal_memcpy(pstDngImageDynamicInfo, &pstDrvCtx->stDngImageDynamicInfo[1], sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
    }
    else
    {
        osal_memcpy(pstDngImageDynamicInfo, &pstDrvCtx->stDngImageDynamicInfo[0], sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
    }

    return HI_SUCCESS;
}

#if 0
HI_S32 ISP_DRV_FuncPreFilter(unsigned int cmd)
{
    /* not support jpegedcf */
    if (!CHIP_SUPPORT_JPEGEDCF()
        && ((ISP_DCF_INFO_SET == cmd) || (ISP_DCF_INFO_GET == cmd)
        || (ISP_DCFINFO_BUF_INIT == cmd) || (ISP_DCFINFO_BUF_EXIT == cmd)))
    {
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    return HI_SUCCESS;
}
#endif

HI_S32 ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    if ( ((0 == proc_param) && (pstIspModParam->proc_param != 0)) ||
		 ((0 != proc_param) && (pstIspModParam->proc_param == 0)) )
    {
        ISP_TRACE(HI_DBG_ERR, "proc_param do not support to change! %d to %d.\n",
			proc_param, pstIspModParam->proc_param);
        return HI_ERR_ISP_NOT_SUPPORT;
    }
    if (!pstIspModParam->u32StatIntvl)
    {
        ISP_TRACE(HI_DBG_ERR, "u32StatIntvl must be larger than 0.\n");
        return HI_ERR_ISP_NOT_SUPPORT;
    }
#if 0
    if ((0 != pstIspModParam->u32UpdatePos) && (1 != pstIspModParam->u32UpdatePos))
    {
        ISP_TRACE(HI_DBG_ERR, "u32UpdatePos must be 0 or 1.\n");
        return HI_ERR_ISP_NOT_SUPPORT;
    }
    if ((0 != pstIspModParam->u32IntBottomHalf) && (1 != pstIspModParam->u32IntBottomHalf))
    {
        ISP_TRACE(HI_DBG_ERR, "u32IntBottomHalf must be 0 or 1.\n");
        return HI_ERR_ISP_NOT_SUPPORT;
    }
#endif

    proc_param      = pstIspModParam->proc_param;
    stat_intvl      = pstIspModParam->u32StatIntvl;
    update_pos      = pstIspModParam->u32UpdatePos;
    //int_bottomhalf  = pstIspModParam->u32IntBottomHalf;
    int_timeout     = pstIspModParam->u32IntTimeOut;
    //pwm_num         = pstIspModParam->u32PwmNum;

    return HI_SUCCESS;
}

HI_S32 ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    pstIspModParam->proc_param          = proc_param;
    pstIspModParam->u32StatIntvl        = stat_intvl;
    pstIspModParam->u32UpdatePos        = update_pos;
    pstIspModParam->u32IntBottomHalf    = int_bottomhalf;
    pstIspModParam->u32IntTimeOut       = int_timeout;
    pstIspModParam->u32PwmNum           = pwm_num;

    return HI_SUCCESS;
}

static long ISP_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    HI_U32 *argp = (HI_U32 *)arg;
    ISP_SYNC_CFG_BUF_S  *pstSyncCfgBuf = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S  *pstCurNode = HI_NULL;
    HI_S32  s32Ret;
    ISP_DEV IspDev = ISP_GET_DEV(private_data);
    unsigned long u32Flags;

    switch (cmd)
    {
        case ISP_DEV_SET_FD :
        {
            ISP_CHECK_POINTER(argp);
            *((HI_U32*)(private_data)) = *(HI_U32*)arg;
            return 0;
        }
        case ISP_GET_VERSION :
        {
            ISP_VERSION_S *pVersion = HI_NULL;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            pVersion = (ISP_VERSION_S *)argp;
            osal_memcpy(&gs_stIspLibInfo, pVersion, sizeof(ISP_VERSION_S));

            gs_stIspLibInfo.u32Magic = ISP_MAGIC_VERSION + ISP_MAGIC_OFFSET;
            osal_memcpy(pVersion, &gs_stIspLibInfo, sizeof(ISP_VERSION_S));
            return 0;
        }
        case ISP_GET_FRAME_EDGE :
        {
            HI_U32 u32Status = 0x0;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_GetFrameEdge(IspDev, &u32Status);
            if (s32Ret)
            {
                ISP_TRACE(HI_DBG_WARN, "Get Interrupt failed!\n");
                return HI_FAILURE;
            }
            //u32Status &= IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK);

            *argp = u32Status;

            return 0;
        }
        case ISP_GET_VD_TIMEOUT:
        {
            ISP_VD_TIMEOUT_S stVdTimeOut;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);

            osal_memcpy(&stVdTimeOut, argp, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdTimeOut(IspDev, &stVdTimeOut.stVdInfo,
            stVdTimeOut.u32MilliSec, (HI_U32*)&stVdTimeOut.s32IntStatus);

            //stVdTimeOut.s32IntStatus &= IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK);

			osal_memcpy(argp, &stVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));

            return 0;
        }
        case ISP_GET_VD_END_TIMEOUT:
        {
            ISP_VD_TIMEOUT_S stVdTimeOut;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);

            osal_memcpy(&stVdTimeOut, argp, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdEndTimeOut(IspDev, &stVdTimeOut.stVdInfo,
            stVdTimeOut.u32MilliSec, (HI_U32*)&stVdTimeOut.s32IntStatus);

            //stVdTimeOut.s32IntStatus &= IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK);

			osal_memcpy(argp, &stVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));

            return 0;
        }
        case ISP_UPDATEINFO_BUF_INIT :
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_DRV_UpdateInfoBufInit(IspDev);

            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].u32UpdateInfoPhyAddr, sizeof(HI_U32));
            return s32Ret;
        }
        case ISP_UPDATEINFO_BUF_EXIT :
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_DRV_UpdateInfoBufExit(IspDev);
        }
        case ISP_FRAME_INFO_SET:
        {
            FRAME_SUPPLEMENT_INFO_S stIspFrame;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);

            osal_memcpy(&stIspFrame, argp, sizeof(FRAME_SUPPLEMENT_INFO_S));

            ISP_SetFrameInfo(IspDev, &stIspFrame);

            return HI_SUCCESS;
        }
        case ISP_FRAME_INFO_GET:
        {
            FRAME_SUPPLEMENT_INFO_S stIspFrame;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);

            ISP_GetFrameInfo(IspDev, &stIspFrame);

			osal_memcpy(argp, &stIspFrame, sizeof(FRAME_SUPPLEMENT_INFO_S));

            return HI_SUCCESS;
        }
        case ISP_FRAME_INFO_BUF_INIT :
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_DRV_FrameInfoBufInit(IspDev);
			//*arg = g_astIspDrvCtx[IspDev].u32FrameInfoPhyAddr;
			osal_memcpy(argp, &g_astIspDrvCtx[IspDev].u32FrameInfoPhyAddr, sizeof(HI_U32));
            return s32Ret;
        }
        case ISP_FRAME_INFO_BUF_EXIT :
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_DRV_FrameInfoBufExit(IspDev);
        }
        case ISP_DNG_INFO_GET:
        {
            DNG_IMAGE_STATIC_INFO_S stDngInfo;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);

            ISP_GetDngInfo(IspDev, &stDngInfo);

			osal_memcpy(argp, &stDngInfo, sizeof(DNG_IMAGE_STATIC_INFO_S));
             
            return HI_SUCCESS;
        }
        case ISP_DNG_INFO_BUF_INIT :
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_DRV_DngInfoBufInit(IspDev);
			osal_memcpy(argp, &g_astIspDrvCtx[IspDev].u32DngInfoPhyAddr, sizeof(HI_U32));
            return s32Ret;
        }
        case ISP_DNG_INFO_BUF_EXIT :
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_DRV_DngInfoBufExit(IspDev);
        }
        case ISP_SET_INT_ENABLE :
        {
            HI_BOOL bEn;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            bEn = *(HI_BOOL*)argp;
            ISP_CHECK_DEV(IspDev);
            return ISP_SetIntEnable(IspDev, bEn);
        }
        /* There should be two ioctl:init & get base phyaddr. */
        case ISP_STAT_BUF_INIT :
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_DRV_StatBufInit(IspDev);
            *argp = g_astIspDrvCtx[IspDev].stStatisticsBuf.u32PhyAddr;
            return s32Ret;
        }
        case ISP_STAT_BUF_EXIT :
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_DRV_StatBufExit(IspDev);
        }
        case ISP_STAT_BUF_GET :
        {
            ISP_STAT_INFO_S *pstStatInfo = HI_NULL;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_DRV_StatBufUserGet(IspDev, &pstStatInfo);
            if (HI_NULL == pstStatInfo)
            {
                return s32Ret;
            }

            osal_memcpy(argp, pstStatInfo, sizeof(ISP_STAT_INFO_S));
            return 0;
        }
        case ISP_STAT_BUF_PUT :
        {
            ISP_STAT_INFO_S stStatInfo;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            osal_memcpy(&stStatInfo, argp, sizeof(ISP_STAT_INFO_S));
			return ISP_DRV_StatBufUserPut(IspDev, &stStatInfo);
        }
        case ISP_STAT_ACT_GET:
        {
            ISP_STAT_INFO_S stActStat;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);

            if (!g_astIspDrvCtx[IspDev].stStatisticsBuf.pstActStat)
            {
                ISP_TRACE(HI_DBG_WARN, "get statistic active buffer err, stat not ready!\n");
                return HI_FAILURE;
            }
            osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
            osal_memcpy(&stActStat, g_astIspDrvCtx[IspDev].stStatisticsBuf.pstActStat, sizeof(ISP_STAT_INFO_S));
            osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

			osal_memcpy(argp, &stActStat, sizeof(ISP_STAT_INFO_S));

            return 0;
        }
        case ISP_REG_CFG_SET:
        {
            HI_U32 u32Flag;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            u32Flag = 1 - g_astIspDrvCtx[IspDev].u32RegCfgInfoFlag;
            osal_memcpy(&g_astIspDrvCtx[IspDev].stRegCfgInfo[u32Flag].stKernelCfg, argp, sizeof(ISP_REG_KERNEL_CFG_S));

            osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
            g_astIspDrvCtx[IspDev].u32RegCfgInfoFlag = u32Flag;
            osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
            return 0;
        }
        case ISP_SYNC_CFG_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
            pstSyncCfgBuf = &g_astIspDrvCtx[IspDev].stSyncCfg.stSyncCfgBuf;
            if (ISPSyncBufIsFull(pstSyncCfgBuf))
            {
                ISP_TRACE(HI_DBG_ERR, "isp sync buffer is full\n");
                //printk("isp sync buffer is full!\n");
                osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);
                return HI_FAILURE;
            }

            pstCurNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag];
            osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

            osal_memcpy(pstCurNode, argp, sizeof(ISP_SYNC_CFG_BUF_NODE_S));

            osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
            pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;
            osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

            return 0;
        }
        case ISP_WDR_CFG_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
            osal_memcpy(&g_astIspDrvCtx[IspDev].stWDRCfg, argp, sizeof(ISP_WDR_CFG_S));
            ISP_DRV_SwitchMode(IspDev, &g_astIspDrvCtx[IspDev]);
            osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

            return 0;
        }

        case ISP_RES_SWITCH_SET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_spin_lock_irqsave(&g_stIspLock[IspDev], &u32Flags);
            ISP_DRV_SwitchMode(IspDev, &g_astIspDrvCtx[IspDev]);
            osal_spin_unlock_irqrestore(&g_stIspLock[IspDev], &u32Flags);

            return 0;
        }

        case ISP_ACM_ATTR_GET:
        case ISP_ACM_ATTR_SET:
        case ISP_ACM_LUT_PHY_ADDR_GET:
        case ISP_ACM_COEF_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            return ISP_ACM_DRV_Ioctrl(cmd,arg,private_data);
        }

        case ISP_PROC_INIT:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            s32Ret = ISP_DRV_ProcInit(IspDev);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].stPorcMem, sizeof(ISP_PROC_MEM_S));
            return 0;
        }
        case ISP_PROC_WRITE_ING:
        {
            ISP_CHECK_DEV(IspDev);
            if (osal_down_interruptible(&g_astIspDrvCtx[IspDev].stProcSem))
            {
                return -ERESTARTSYS;
            }
            return 0;
        }
        case ISP_PROC_WRITE_OK:
        {
            ISP_CHECK_DEV(IspDev);
            osal_up(&g_astIspDrvCtx[IspDev].stProcSem);
            return 0;
        }
        case ISP_PROC_EXIT:
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_DRV_ProcExit(IspDev);
        }
        case ISP_PROC_PARAM_GET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            *argp = proc_param;
            return 0;
        }
        case ISP_UPDATE_POS_GET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            *argp = update_pos;
            return 0;
        }
        case ISP_MEM_INFO_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            g_astIspDrvCtx[IspDev].bMemInit = *(HI_BOOL*)argp;
            return 0;
        }
        case ISP_MEM_INFO_GET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
			*(HI_BOOL *)argp = g_astIspDrvCtx[IspDev].bMemInit;
            return 0;
        }
        case ISP_MODE_INFO:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            g_astIspDrvCtx[IspDev].enIspMode = *(ISP_MODE_E*)argp;
            return 0;
        }
        case ISP_CAP_INFO_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
			osal_memcpy(&g_astIspDrvCtx[IspDev].stCapInfoLastSave, &g_astIspDrvCtx[IspDev].stCapInfoSave,sizeof(ISP_CONFIG_INFO_S));
            osal_memcpy(&g_astIspDrvCtx[IspDev].stCapInfoSave, argp, sizeof(ISP_CONFIG_INFO_S));

			//g_astIspDrvCtx[IspDev].stCapInfoSave.u64Pts = CALL_SYS_GetTimeStamp();
            return 0;
        }
        case ISP_CAP_INFO_GET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].stCapInfoLoad, sizeof(ISP_CONFIG_INFO_S));

            if (SNAP_STATE_CFG == g_astIspDrvCtx[IspDev].stCapInfoLoad.enCapState)
            {
                if (0 == update_pos)
                {
                    g_astIspDrvCtx[IspDev].stCapInfoLoad.enCapState = SNAP_STATE_NULL;
                }
            }

            return 0;
        }
        case ISP_DNG_INFO_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
			osal_memcpy(&g_astIspDrvCtx[IspDev].stDngImageDynamicInfo[1], &g_astIspDrvCtx[IspDev].stDngImageDynamicInfo[0],sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
            osal_memcpy(&g_astIspDrvCtx[IspDev].stDngImageDynamicInfo[0], argp, sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
            
            return 0;
        }
        case ISP_PRO_TRIGGER_GET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].bProEnable, sizeof(HI_BOOL));

            if (HI_TRUE == g_astIspDrvCtx[IspDev].bProEnable)
            {
                g_astIspDrvCtx[IspDev].bProEnable = HI_FALSE;
                g_astIspDrvCtx[IspDev].bProStart = HI_TRUE;
            }

            return 0;
        }
        case ISP_PRO_EXP_ATTR_SET:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            osal_memcpy(&g_astIspDrvCtx[IspDev].stSnapAttr, argp, sizeof(ISP_SNAP_ATTR_S));

            return 0;
        }
        case ISP_PWM_NUM_GET:
        {
            // TODO: need to check IspDev
            //ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
			*argp = pwm_num;

            return 0;
        }
        case ISP_SET_MOD_PARAM:
        {
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            return ISP_SetModParam((ISP_MOD_PARAM_S *)argp);
        }
        case ISP_GET_MOD_PARAM:
        {
            ISP_MOD_PARAM_S stIspModParam;
            ISP_CHECK_DEV(IspDev);
            ISP_CHECK_POINTER(argp);
            ISP_GetModParam(&stIspModParam);
            osal_memcpy(argp, &stIspModParam, sizeof(ISP_MOD_PARAM_S));

            return HI_SUCCESS;
        }
        default:
        {
            return VREG_DRV_ioctl(cmd, arg, private_data);
        }
    }

    return 0;
}

static int ISP_open(void *data)
{
    return 0;
}

static int ISP_close(void *data)
{
    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static HI_S32 ISP_Freeze(osal_dev_t *pdev)
{
    osal_printk(  "%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(osal_dev_t *pdev)
{
    HI_U32 u32VicapIntMask;
	ISP_DEV IspDev;
#if 0
#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_FALSE;
    HI_BOOL bClkEn = HI_TRUE;

    /* For power saving. Open ISP clock and cancel ISP soft reset when module init */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
#endif
	for(IspDev=0;IspDev<ISP_MAX_DEV_NUM;IspDev++)
	{
	    /* enable port0 int & isp int */
	    u32VicapIntMask = IO_RD_ISP_ADDRESS(IspDev, VICAP_HD_MASK);
	    IO_RD_ISP_ADDRESS(IspDev, VICAP_HD_MASK) = (u32VicapIntMask | 0x10100);
	    IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT_MASK) = ((VI_PT0_INT_FSTART) | (VI_PT0_INT_ERR));
	    IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK) = (0x0);
	}

    osal_printk(  "%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}
#else
static HI_S32 ISP_Freeze(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}
#endif

static struct osal_fileops stIspFileOp =
{
    .unlocked_ioctl      = ISP_ioctl,
    .open       = ISP_open,
    .release    = ISP_close
};

struct osal_pmops stHiISPDrvOps = {
    .pm_freeze = ISP_Freeze,
    .pm_restore  = ISP_Restore
};

static osal_dev_t *s_pstHiISPDevice = NULL;

static inline int ISP_ISR(int irq, void *id)
{
    ISP_DEV IspDev;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    HI_U32 u32PortIntStatus = 0;
    HI_U32 u32PortIntFStart = 0, u32PortIntErr = 0;
    HI_U32 u32IspIntStatus = 0;
    HI_U32 u32DesIntStatus = 0;
    HI_U32 u32DesIntFStart = 0;
    HI_U32 u32DesIntFEnd = 0;
    //HI_U32 u32DesIntMask = 0;
    HI_U64 u64PtTime1 = 0, u64PtTime2 = 0;
    HI_U64 u64IspTime1 = 0, u64IspTime2 = 0;

    IspDev = (irq == ISP0_IRQ_NR) ? 0 : \
             (irq == ISP1_IRQ_NR) ? 1 : (-1);

    ISP_CHECK_DEV(IspDev);

    u32PortIntStatus = IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT);
    u32PortIntStatus &= IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT_MASK);

    u32PortIntFStart = u32PortIntStatus & VI_PT0_INT_FSTART;
    u32PortIntErr = u32PortIntStatus & VI_PT0_INT_ERR;

    u32DesIntStatus = IO_RD_ISP_ADDRESS(IspDev, VI_DES0_INT);
    //u32DesIntStatus &= HW_REG(IO_ADDRESS_PORT(VI_DES0_INT_MASK));
    u32DesIntFStart = u32DesIntStatus & VI_DES0_INT_FSTART;
    u32DesIntFEnd = u32DesIntStatus & VI_DES0_INT_FEND;

    u32IspIntStatus = IO_RD_ISP_ADDRESS(IspDev, ISP_INT);
    u32IspIntStatus &= IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK);

    if ((!u32PortIntStatus) && (!u32IspIntStatus) && (!u32DesIntStatus))
    {
        return OSAL_IRQ_NONE;
    }

    if (u32PortIntStatus)
    {
        IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT) = u32PortIntStatus;
    }
    if (u32IspIntStatus)
    {
        IO_RD_ISP_ADDRESS(IspDev, ISP_INT) = u32IspIntStatus;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (u32PortIntFStart) /* port int proc */
    {
        pstDrvCtx->stDrvDbgInfo.u32PtIntCnt++;
        u64PtTime1 = CALL_SYS_GetTimeStamp();
        if (pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime) //not first int
        {
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime = u64PtTime1 - pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime;
            if (pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime > pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime;
            }
        }
        pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime = u64PtTime1;
    }

    if (u32IspIntStatus & ISP_1ST_INT) /* isp int proc */
    {
        pstDrvCtx->stDrvDbgInfo.u32IspIntCnt++;
        u64IspTime1 = CALL_SYS_GetTimeStamp();

        if (pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime)  //not first int
        {
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime = u64IspTime1 - pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime;
            if (pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime > pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime;
            }
        }
        pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime = u64IspTime1;
    }

    pstDrvCtx->stIntSch.u32IspIntStatus = u32IspIntStatus;
    pstDrvCtx->stIntSch.u32PortIntStatus = u32PortIntFStart;
    pstDrvCtx->stIntSch.u32DesIntFEnd = u32DesIntFEnd;
    pstDrvCtx->stIntSch.u32PortIntErr = u32PortIntErr;

    if (!use_bottomhalf)
    {
        ISP_IntUserBottomHalf(irq, id);
    }

    if (u32PortIntFStart) /* port int proc */
    {
        u64PtTime2 = CALL_SYS_GetTimeStamp();
        pstDrvCtx->stDrvDbgInfo.u32PtIntTime = u64PtTime2 - u64PtTime1;

        if (pstDrvCtx->stDrvDbgInfo.u32PtIntTime > pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax)
        {
            pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntTime;
        }

        if ((u64PtTime2 - pstDrvCtx->stDrvDbgInfo.u64PtLastRateTime) >= 1000000ul)
        {
            pstDrvCtx->stDrvDbgInfo.u64PtLastRateTime = u64PtTime2;
            pstDrvCtx->stDrvDbgInfo.u32PtRate = pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt;
            pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt = 0;
        }

        pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt++;
    }

    if (u32IspIntStatus & ISP_1ST_INT) /* isp int proc */
    {
        u64IspTime2 = CALL_SYS_GetTimeStamp();
        pstDrvCtx->stDrvDbgInfo.u32IspIntTime = u64IspTime2 - u64IspTime1;

        if (pstDrvCtx->stDrvDbgInfo.u32IspIntTime > pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax)
        {
            pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntTime;
        }

        if ((u64IspTime2 - pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime) >= 1000000ul)
        {
            pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime = u64IspTime2;
            pstDrvCtx->stDrvDbgInfo.u32IspRate = pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt;
            pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt = 0;
        }

        pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt++;
    }

    return (HI_TRUE == use_bottomhalf)?OSAL_IRQ_WAKE_THREAD:OSAL_IRQ_HANDLED;

}

int  ISP_IntBottomHalf(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
    HI_U64 u64SensorCfgTime1 = 0, u64SensorCfgTime2 = 0;
    HI_U32 u32PortIntFStart;
    HI_U32 u32IspIntStatus;
    HI_U32 u32DesIntFEnd;
    HI_U32 u32SensorCfgInt = 0;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

 //   IspDev = ReturnIspDevNumByHandle(pstDrvCtx, ISP_DRV_GET_CTX(0));
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
    u32IspIntStatus = pstDrvCtx->stIntSch.u32IspIntStatus;
    u32DesIntFEnd = pstDrvCtx->stIntSch.u32DesIntFEnd;

    pstDrvCtx->u32IntPos = 0;

    if (u32IspIntStatus & ISP_1ST_INT)
    {
        if (HI_NULL != pstDrvCtx->stSnapCb.pfnSnapIspInfoUpdate)
        {
            pstDrvCtx->stSnapCb.pfnSnapIspInfoUpdate(IspDev, &pstDrvCtx->stCapInfoLoad);
        }
    }

    if (u32IspIntStatus & ISP_1ST_INT)
    {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(IspDev, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(IspDev, pstDrvCtx);
            ISP_DRV_RegConfigSensor(IspDev, pstDrvCtx);
        }
    }

    if (u32PortIntFStart)
    {
        if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(IspDev, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            //ISP_DRV_RegConfigIsp(IspDev, pstDrvCtx);
            ISP_DRV_RegConfigSensor(IspDev, pstDrvCtx);
        }
    }

    /* * * * * * * * isp_int1_process * * * * * * * */
    if (u32IspIntStatus & ISP_1ST_INT)
    {
        if (IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(IspDev, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(IspDev, pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(IspDev, pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    if (0 == update_pos) /* frame start */
    {
        u32SensorCfgInt = (u32IspIntStatus & ISP_1ST_INT);
    }
    else                 /* frame end */
    {
        u32SensorCfgInt = (u32IspIntStatus & ISP_2ND_INT);
    }

    /* * * * * * * * isp_int2_process * * * * * * * */
    if (u32SensorCfgInt)
    {
        /* In linear mode or built-in WDR mode, config sensor and vi(isp) register with isp_int(frame start interrupt) */
        if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) || IS_BUILT_IN_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(IspDev, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(IspDev, pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(IspDev, pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    if (u32IspIntStatus & ISP_2ND_INT)
    {
        pstDrvCtx->u32IntPos = 1;
        g_bPrimStaReaded[IspDev] = HI_FALSE;
        g_bHaveEndInt[IspDev] = HI_TRUE;
        //if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) || IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_RegConfigSensor(IspDev, pstDrvCtx);
        }

        if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_RegConfigIsp(IspDev, pstDrvCtx);
        }

        /* get stat at frame end int */
        if (0 == pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(stat_intvl))
        {
           ISP_DRV_StatBufBusyPut(IspDev);
        }

        pstDrvCtx->bVdEnd = HI_TRUE;
        osal_wakeup(&pstDrvCtx->stIspWaitVdEnd);
    }

    if (pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime > pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax)
    {
        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax = pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime;
    }

    /* * * * * * * * isp_int_process * * * * * * * */
    if (u32IspIntStatus & ISP_1ST_INT) /* ISP int */
    {
        /* N to 1 fullrate frame WDR mode, get statistics only in the last frame(N-1) */
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            if (pstDrvCtx->stSyncCfg.u8VCNum != pstDrvCtx->stSyncCfg.u8VCNumMax)
            {
                return OSAL_IRQ_HANDLED;
            }
        }

        pstDrvCtx->u32Status = u32IspIntStatus;

        /* first config register. */
        //ISP_DRV_RegConfig(&pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegCfgInfoFlag]);
#if 0
        if (0 == pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(stat_intvl))
        {
           ISP_DRV_StatBufBusyPut(IspDev);
        }
#endif
        pstDrvCtx->bEdge = HI_TRUE;
        pstDrvCtx->bVd   = HI_TRUE;

        osal_wakeup(&pstDrvCtx->stIspWait);
        osal_wakeup(&pstDrvCtx->stIspWaitVd);
        IspSyncTaskProcess(IspDev);

    }

    return OSAL_IRQ_HANDLED;
}

int  ISP_IntUserBottomHalf(int irq, void *id)
{
    ISP_DEV IspDev;
    HI_S32  IspDev_i = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PortIntFStart;
    HI_U32 u32IspIntStatus;
    HI_U32 u32DesIntFEnd;

    IspDev = (irq == ISP0_IRQ_NR) ? 0 : \
             (irq == ISP1_IRQ_NR) ? 1 : (-1);

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (ISP_MODE_STITCH == pstDrvCtx->enIspMode)
    {
        if(IS_MAIN_CHANNAL(IspDev))
        {
            IspDev_i = 0;
            u32IspIntStatus = pstDrvCtx->stIntSch.u32IspIntStatus;
            u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
            u32DesIntFEnd = pstDrvCtx->stIntSch.u32DesIntFEnd;
            while(IspDev_i <= 1)
            {
                pstDrvCtx = ISP_DRV_GET_CTX(IspDev_i);

                /*When detect vi port's width&height changed,then reset isp*/
                if(pstDrvCtx->stIntSch.u32PortIntErr)
                {
                    pstDrvCtx->stDrvDbgInfo.u32IspResetCnt++;
                    IO_RD_ISP_ADDRESS(IspDev_i, ISP_RESET) = 0x1;
                    IO_RD_ISP_ADDRESS(IspDev_i, ISP_RESET) = 0;

                    return OSAL_IRQ_HANDLED;
                }
                pstDrvCtx->stIntSch.u32IspIntStatus = u32IspIntStatus;
                pstDrvCtx->stIntSch.u32PortIntStatus = u32PortIntFStart;
                pstDrvCtx->stIntSch.u32DesIntFEnd = u32DesIntFEnd;

                ISP_IntBottomHalf(IspDev_i);

                IspDev_i++;
            }
        }
    }
    else
    {
        /*When detect vi port's width&height changed,then reset isp*/
        if(pstDrvCtx->stIntSch.u32PortIntErr)
        {
            pstDrvCtx->stDrvDbgInfo.u32IspResetCnt++;
            IO_RD_ISP_ADDRESS(IspDev, ISP_RESET) = 0x1;
            IO_RD_ISP_ADDRESS(IspDev, ISP_RESET) = 0;

            return OSAL_IRQ_HANDLED;
        }

        ISP_IntBottomHalf(IspDev);
    }

    return OSAL_IRQ_HANDLED;
}


static int ISP_DRV_Init(void)
{
    HI_U32 u32VicapIntMask;
    HI_U32 IspDev;

    for (IspDev = 0; IspDev < ISP_MAX_DEV_NUM; IspDev++)
    {
        osal_memset(&g_astIspDrvCtx[IspDev], 0, sizeof(ISP_DRV_CTX_S));
        osal_snprintf(g_astIspDrvCtx[IspDev].acName, sizeof(g_astIspDrvCtx[IspDev].acName), "isp%d", IspDev);

		#if 1
        reg_vi_isp_base_va[IspDev] = (void *)osal_ioremap(ISP_REG_BASE(IspDev),(HI_U32)0x10000);
        if (HI_NULL == reg_vi_isp_base_va[IspDev])
        {
            osal_printk( "Remap ISP[%d] base failed!\n", IspDev);
            return HI_FAILURE;
        }
		#else
		reg_vi_isp_base_va[IspDev] = g_psViuAllReg[IspDev];
        //reg_vicap_base_va[IspDev] = g_psViuAllReg[IspDev];
        // reg_isp_base_va[IspDev] = reg_vicap_base_va[IspDev] + ISP_BASE - VICAP_BASE;
		#endif

        /* enable port int & isp int */
        u32VicapIntMask = IO_RD_ISP_ADDRESS(IspDev, VICAP_HD_MASK);
        IO_RD_ISP_ADDRESS(IspDev, VICAP_HD_MASK) = (u32VicapIntMask | 0x10100);
        IO_RD_ISP_ADDRESS(IspDev, VI_PT0_INT_MASK) = ((VI_PT0_INT_FSTART) | (VI_PT0_INT_ERR));
        IO_RD_ISP_ADDRESS(IspDev, ISP_INT_MASK) = (0x0);

#ifndef __HuaweiLite__
        if (int_bottomhalf)
        {
            use_bottomhalf = HI_TRUE;
        }
#endif


	    if (use_bottomhalf)
	    {
         //   osal_printk( "\nyyyyyyyyyyyyyyyyyyyyyyyyyyyyy4ISP[%d] Register Interrupt &&&&&&&&&&&&&&&&&&&\n", IspDev);
	        if (osal_request_irq(isp_irq[IspDev], ISP_ISR, ISP_IntUserBottomHalf, "ISP", (void*)&g_astIspDrvCtx[IspDev]))
            {
                osal_printk( "ISP[%d] Register Interrupt Failed!\n", IspDev);
                return HI_FAILURE;
            }
	    }
        else
        {
         //   osal_printk( "\nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnISP[%d] Register Interrupt &&&&&&&&&&&&&&&&&&&\n", IspDev);
            if (osal_request_irq(isp_irq[IspDev], ISP_ISR, HI_NULL, "ISP", (void*)&g_astIspDrvCtx[IspDev]))
            {
                osal_printk( "ISP[%d] Register Interrupt Failed!\n", IspDev);
                return HI_FAILURE;
            }
        }

        osal_wait_init(&g_astIspDrvCtx[IspDev].stIspWait);
        osal_wait_init(&g_astIspDrvCtx[IspDev].stIspWaitVd);
        osal_wait_init(&g_astIspDrvCtx[IspDev].stIspWaitVdEnd);
        g_astIspDrvCtx[IspDev].bEdge = HI_FALSE;
        g_astIspDrvCtx[IspDev].bVd = HI_FALSE;
        g_astIspDrvCtx[IspDev].bVdEnd = HI_FALSE;
        g_astIspDrvCtx[IspDev].bMemInit = HI_FALSE;
        osal_sema_init(&g_astIspDrvCtx[IspDev].stIspSem, 1);
        osal_sema_init(&g_astIspDrvCtx[IspDev].stProcSem, 1);
        ISP_ACM_DRV_Init(IspDev);
        SyncTaskInit(IspDev);
    }

    return 0;
}

static int ISP_DRV_Exit(void)
{
    HI_U32 IspDev;

    for (IspDev = 0; IspDev < ISP_MAX_DEV_NUM; IspDev++)
    {
        osal_free_irq(isp_irq[IspDev], (void*)&g_astIspDrvCtx[IspDev]);
        SyncTaskExit(IspDev);
        ISP_ACM_DRV_Exit(IspDev);
        //reg_vi_isp_base_va[IspDev] = HI_NULL;

		osal_sema_destory(&g_astIspDrvCtx[IspDev].stIspSem);
    	osal_sema_destory(&g_astIspDrvCtx[IspDev].stProcSem);

        osal_wait_destory(&g_astIspDrvCtx[IspDev].stIspWait);
        osal_wait_destory(&g_astIspDrvCtx[IspDev].stIspWaitVd);
        osal_wait_destory(&g_astIspDrvCtx[IspDev].stIspWaitVdEnd);

        if (reg_vi_isp_base_va[IspDev] != HI_NULL)
        {
            osal_iounmap(reg_vi_isp_base_va[IspDev]);
            reg_vi_isp_base_va[IspDev] = HI_NULL;
        }
    }

    return 0;
}

#ifdef TEST_TIME
static int ISP_Test_Init(void)
{
    CMPI_MmzMallocNocache(HI_NULL, "ISPStatTest", &g_test_phyaddr, (HI_VOID**)&g_test_pviraddr, 256*2);
}

static int ISP_Test_Exit(void)
{
    CMPI_MmzFree(g_test_phyaddr, g_test_pviraddr);
}
#endif

#ifndef DISABLE_DEBUG_INFO
static int ISP_ProcShow(osal_proc_entry_t *s)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx0 = HI_NULL, *pstDrvCtx1 = HI_NULL;
    HI_U8 u8Bayer;
 //   extern char proc_title[1024];

    osal_seq_printf(s, "\n[ISP] Version: ["ISP_VERSION"], Build Time[%s, %s]\n",__DATE__,__TIME__);
 //   osal_seq_printf(s, "\n[ISP] Version: ["ISP_VERSION"], %s",proc_title);
    osal_seq_printf(s, "\n");

    pstDrvCtx0 = ISP_DRV_GET_CTX(0);
    pstDrvCtx1 = ISP_DRV_GET_CTX(1);
    if(pstDrvCtx0->bMemInit || pstDrvCtx1->bMemInit)
    {
    	osal_seq_printf(s, "-----MODULE PARAM--------------------------------------------------------------\n");
    	osal_seq_printf(s, " %15s" " %15s" " %15s" " %15s" " %15s" " %15s" "\n",  "proc_param", "StatIntvl", "UpdatePos", "IntBottomHalf", "IntTimeOut", "PwmNum");
    	osal_seq_printf(s, " %15u" " %15u" " %15u" " %15u" " %15u" " %15xH" "\n",  proc_param, stat_intvl, update_pos, int_bottomhalf, int_timeout, pwm_num);
        osal_seq_printf(s, "\n");
    }

    do
    {
        pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
        if(HI_FALSE == pstDrvCtx->bMemInit)
        {
            continue;
        }

    	osal_seq_printf(s, "-----ISP Mode-------------------------------------------------------------------------------------\n");
    	osal_seq_printf(s, " %15s" "\n",  "IspMode");
    	osal_seq_printf(s, " %15s" "\n",  pstDrvCtx->enIspMode ? "ISP_MODE_STITCH" : "ISP_MODE_NORMAL" );
        osal_seq_printf(s, "\n");
        osal_seq_printf(s, "-----DRV INFO-------------------------------------------------------------------------------------\n");

        osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%12s\n"
                ,"IspDev","IntCnt","IntT","MaxIntT","IntGapT","MaxGapT","IntRat","IspResetCnt");

        osal_seq_printf(s, "%11d" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%12d\n",
                IspDev,
                pstDrvCtx->stDrvDbgInfo.u32IspIntCnt,
                pstDrvCtx->stDrvDbgInfo.u32IspIntTime,
                pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax,
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime,
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax,
                pstDrvCtx->stDrvDbgInfo.u32IspRate,
                pstDrvCtx->stDrvDbgInfo.u32IspResetCnt);

        osal_seq_printf(s, "\n");

        osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%11s" "%12s\n"
                ,"","PtIntCnt","PtIntT","PtMaxIntT","PtIntGapT","PtMaxGapT","PtIntRat","SensorCfgT", "SensorMaxT");

        osal_seq_printf(s, "%11s" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%11d" "%12d\n",
                "",
                pstDrvCtx->stDrvDbgInfo.u32PtIntCnt,
                pstDrvCtx->stDrvDbgInfo.u32PtIntTime,
                pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax,
                pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime,
                pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax,
                pstDrvCtx->stDrvDbgInfo.u32PtRate,
                pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime,
                pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax);

        osal_seq_printf(s, "\n");

        /* TODO: show isp attribute here. width/height/bayer_format, etc..
          * Read parameter from memory directly.
          */
        u8Bayer = hi_isp_top_rggb_start_read(IspDev);
        osal_seq_printf(s, "-----PubAttr INFO---------------------------------------------------------------------------------\n");

        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s\n"
                ,"WndX","WndY","WndW","WndH","SnsW","SnsH","Bayer");

        osal_seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12s\n\n",
                hi_isp_fe_input_port_crop_x_start_read(IspDev),
                hi_isp_fe_input_port_crop_y_start_read(IspDev),
                hi_isp_fe_input_port_crop_width_read(IspDev) + 1,
                hi_isp_fe_input_port_crop_height_read(IspDev) + 1,
                hi_ext_top_sensor_width_read(IspDev),
                hi_ext_top_sensor_height_read(IspDev),
                (0 == u8Bayer) ? "RGGB" :
                    (1 == u8Bayer) ? "GRBG" :
                    (2 == u8Bayer) ? "GBRG" :
                    (3 == u8Bayer) ? "BGGR" : "BUTT");
        osal_seq_printf(s, "\n");

        /* TODO: show isp snap attribute here. SnapType/PipeMode/OPType/ProFrmNum, etc..
          */
        osal_seq_printf(s, "-----SNAPATTR INFO--------------------------------------------------------------------------------\n");

        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s\n"
                ,"SnapType","PipeMode","OPType","ProFrmNum");
        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12d\n\n",
               (0 == pstDrvCtx->stSnapAttr.enSnapType) ? "NONE" :
               (1 == pstDrvCtx->stSnapAttr.enSnapType) ? "NORMAL" :
               (2 == pstDrvCtx->stSnapAttr.enSnapType) ? "PRO" :
               (3 == pstDrvCtx->stSnapAttr.enSnapType) ? "USR" :
               (4 == pstDrvCtx->stSnapAttr.enSnapType) ? "SYNC" :
               (5 == pstDrvCtx->stSnapAttr.enSnapType) ? "SYNC_PRO" :  "BUTT",
               (0 == pstDrvCtx->stSnapAttr.enSnapPipeMode) ? "NONE" :
               (1 == pstDrvCtx->stSnapAttr.enSnapPipeMode) ? "PREVIEW" :
               (2 == pstDrvCtx->stSnapAttr.enSnapPipeMode) ? "PICTURE" :
               (3 == pstDrvCtx->stSnapAttr.enSnapPipeMode) ? "PRE_PIC" : "BUTT",
               (0 == pstDrvCtx->stSnapAttr.stProParam.enOpType) ? "Auto" : "Manul",
                    pstDrvCtx->stSnapAttr.stProParam.u8ProFrameNum);

        ISP_DRV_ProcPrintf(IspDev, s);
        osal_seq_printf(s, "\n");

    } while ( ++IspDev < ISP_MAX_DEV_NUM);

    return 0;
}
#endif
HI_S32 ISP_Init(void *p)
{
    HI_U32 IspDev;

    for (IspDev = 0; IspDev < ISP_MAX_DEV_NUM; IspDev++)
    {
        osal_memset(&g_astIspDrvCtx[IspDev].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    }

    return HI_SUCCESS;
}

HI_VOID ISP_Exit(void)
{
    HI_U32 IspDev;

    for (IspDev = 0; IspDev < ISP_MAX_DEV_NUM; IspDev++)
    {
        osal_memset(&g_astIspDrvCtx[IspDev].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    }

    return ;
}

ISP_EXPORT_FUNC_S g_stIspExpFunc =
{
    .pfnISPRegisterBusCallBack   = ISP_RegisterBusCallBack,
    .pfnISPRegisterPirisCallBack = ISP_RegisterPirisCallBack,
    .pfnISPRegisterSnapCallBack  = ISP_RegisterSnapCallBack,
#ifdef ENABLE_JPEGEDCF
    .pfnISP_GetDCFInfo           = ISP_GetDCFInfo,
    .pfnISP_SetDCFInfo           = ISP_SetDCFInfo,
#else
    .pfnISP_GetDCFInfo           = HI_NULL,
    .pfnISP_SetDCFInfo           = HI_NULL,
#endif
    .pfnISP_GetIspUpdateInfo     = ISP_GetIspUpdateInfo,
    .pfnISP_SetIspUpdateInfo     = ISP_SetIspUpdateInfo,
    .pfnISP_GetFrameInfo         = ISP_GetFrameInfo,
    .pfnISP_SetFrameInfo         = ISP_SetFrameInfo,
    .pfnISP_GetProCtrl           = ISP_GetProCtrl,
    .pfnISP_SetProCtrl           = ISP_SetProCtrl,
    .pfnISP_SaveCaptureConfig    = ISP_SaveCaptureConfig,
    .pfnISP_LoadCaptureConfig    = ISP_LoadCaptureConfig,
    .pfnISP_SetProEnable         = ISP_SetProEnable,
    .pfnISP_PrimStatisticsRead   = ISP_DRV_PrimStatisticsRead,
    .pfnISP_GetDngImageDynamicInfo = ISP_DRV_GetDngImageDynamicInfo,

    .pfnISPRegisterSyncTask      = hi_isp_sync_task_register,
    .pfnISPUnRegisterSyncTask    = hi_isp_sync_task_unregister,
};


static UMAP_MODULE_S s_stModule =
{
    .enModId       = HI_ID_ISP,
    .aModName      = "isp",

    .pfnInit       = ISP_Init,
    .pfnExit       = ISP_Exit,
    .pfnVerChecker = HI_NULL,
    .pstExportFuncs = &g_stIspExpFunc,
    .pData         = HI_NULL,
};

int ISP_ModInit(void)
{
    //HI_S32  s32Ret;
    HI_U32  IspDev;
#ifndef DISABLE_DEBUG_INFO
	osal_proc_entry_t *proc=NULL;
#endif

#if 0
#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_FALSE;
    HI_BOOL bClkEn = HI_TRUE;

    /* For power saving. Open ISP clock and cancel ISP soft reset when module init */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
#endif

    s_pstHiISPDevice = osal_createdev("isp_dev");
    s_pstHiISPDevice->fops = &stIspFileOp;
    s_pstHiISPDevice->osal_pmops = &stHiISPDrvOps;
    s_pstHiISPDevice->minor = UMAP_ISP_MINOR_BASE;
    if (osal_registerdevice(s_pstHiISPDevice) < 0)
    {
        HI_PRINT("Kernel: Could not register isp devices\n");
        osal_destroydev(s_pstHiISPDevice);
        return HI_FAILURE;
    }

#ifndef DISABLE_DEBUG_INFO
    proc = osal_create_proc_entry(PROC_ENTRY_ISP, NULL);
    if(HI_NULL == proc)
    {
        HI_PRINT("Kernel: Register isp proc failed!\n");
        goto OUT2;
    }
    proc->read = ISP_ProcShow;
#endif
    if (CMPI_RegisterModule(&s_stModule))
    {
        goto OUT1;;
    }

    for (IspDev = 0; IspDev < ISP_MAX_DEV_NUM; IspDev++)
    {
        osal_spin_lock_init(&g_stIspLock[IspDev]);
    }

    if(0 != ISP_DRV_Init())
    {
        HI_PRINT("isp init failed\n");
        goto OUT1;
    }

//    g_stIspExpFunc.pfnISPRegisterBusCallBack = ISP_RegisterBusCallBack;
//    g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = ISP_RegisterPirisCallBack;

#ifdef TEST_TIME
    ISP_Test_Init();
#endif
    HI_PRINT(  "ISP Mod init!\n");
    return HI_SUCCESS;
OUT1:
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, HI_NULL);
#endif
OUT2:
    osal_deregisterdevice(s_pstHiISPDevice);
    osal_destroydev(s_pstHiISPDevice);

    HI_PRINT("ISP Mod init failed!\n");
    return HI_FAILURE;
}

void ISP_ModExit(void)
{
	int i;
#if 0
#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_TRUE;
    HI_BOOL bClkEn = HI_FALSE;
#endif
#endif

    //g_stIspExpFunc.pfnISPRegisterBusCallBack = HI_NULL;
    //g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = HI_NULL;
    ISP_DRV_Exit();
	for(i=0;i<ISP_MAX_DEV_NUM;i++)
	{
		osal_spin_lock_destory(&g_stIspLock[i]);
	}

    CMPI_UnRegisterModule(HI_ID_ISP);
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP,NULL);
#endif
    osal_deregisterdevice(s_pstHiISPDevice);
    osal_destroydev(s_pstHiISPDevice);

#ifdef TEST_TIME
    ISP_Test_Exit();
#endif
#if 0
#ifndef HI_FPGA
    /* For power saving. Reset ISP and close ISP clock when module exit */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
#endif
    HI_PRINT("ISP Mod Exit!\n");
}

/*
module_init(ISP_ModInit);
module_exit(ISP_ModExit);
module_param(proc_param, uint, S_IRUGO);
module_param(pwm_num, uint, S_IRUGO);
module_param(update_pos, uint, S_IRUGO);


EXPORT_SYMBOL(g_stIspExpFunc);
EXPORT_SYMBOL(g_stIspPirisExpFunc);

#ifdef ENABLE_JPEGEDCF
EXPORT_SYMBOL(ISP_GetDCFInfo);
#endif
EXPORT_SYMBOL(ISP_GetFrameInfo);
MODULE_AUTHOR(MPP_AUTHOR);
MODULE_LICENSE(MPP_LICENSE);
MODULE_VERSION(MPP_VERSION);
*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

