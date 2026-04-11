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
#include "hi_comm_vb.h"

#include "isp_metering_mem_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_config.h"
#include "isp.h"

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


/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define VICAP_BASE              0x11380000

#define IO_ADDRESS_PORT(x)      ((unsigned long)reg_vicap_base_va + x)
#define HW_REG(reg)             *((volatile unsigned int *)(reg))

#define ISP_BASE                0x113A0000
#define ISP_OFFSET              0x10000

/* 0x00F8[8]: int_isp_en; 0x00F8[16]: int_pt0_en; 0x00F8[0]: int_ch0_en; */
#define VICAP_HD_MASK           0x00F8
#define VICAP_HD_INTTERRUPT     0x00F0
#define ISP_AF_INT              0x123F0
#define ISP_AF_INT_MASK         0x123F8

#define VI_PT0_INT_MASK         0x01F8
#define VI_PT0_INT              0x01F0
#define VI_PT0_INT_FSTART       (1<<0)
#define VI_PT0_INT_ERR          (3<<1)

#define ISP_INT_FE_MASK         0x200F8
#define ISP_INT_FE              0x200F0
#define ISP_INT_BE_MASK         0x400F8
#define ISP_INT_BE              0x400F0
#define VI_PT0_ID               0x01E0
#define VI_PT0_ID_CFG           0x01A4

#define CH_REG_NEWER            0x1004
#define ISP_444_REG_NEWER       0x201EC
#define ISP_422_REG_NEWER       0x401EC
/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR             0x01A4

#define ISP_RESET               0x0078
#define CHN_SWITCH              0x10028

//#define ISP_MAGIC_OFFSET        1
#define ISP_PROC_SIZE           0x10000 /* 10k */

#define VI_PT0_DELAY_ADDR       0x0160
#if 0
#define ISP_VER_D               VER_D
#define ISP_VER_PRIX            "_ISP_V"
#define __MK_ISP_VER(x,y,z)     #x"."#y"."#z
#define MK_ISP_VER(x,y,z)       __MK_ISP_VER(x,y,z)
#define ISP_VERSION             CHIP_NAME ISP_VER_PRIX MK_ISP_VER(VER_X,VER_Y,VER_Z)
#endif

#define IO_ISP_ADDRESS(x)      ((unsigned long)reg_isp_base_va + (x))
#define IO_RD_ISP_ADDRESS(x)   (*((unsigned long *)IO_ISP_ADDRESS(x)))


int ISP_IntBottomHalf(ISP_DEV IspDev);
int  ISP_IntUserBottomHalf(int irq, void *id);



//extern HI_BOOL VB_IsSupplementSupport(HI_U32 u32Mask);
//#define CHIP_SUPPORT_JPEGEDCF()    (CALL_VB_IsSupplementSupport(VB_SUPPLEMENT_JPEG_MASK))

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
ISP_DRV_CTX_S           g_astIspDrvCtx[1] = {{0}};

//ISP_EXPORT_FUNC_S       g_stIspExpFunc = {0};
//ISP_PIRIS_EXPORT_FUNC_S  g_stIspPirisExpFunc = {0};
//static ISP_VERSION_S    gs_stIspLibInfo;
void             *reg_vicap_base_va = HI_NULL;
void             *reg_isp_base_va = HI_NULL;

HI_U32                  proc_param = 0;         /* 0: close proc; n: write proc info's interval int num */
HI_U32                  pwm_num = 2;
HI_U32                  update_pos = 0;         /* 0: frame start; 1: frame end */
HI_BOOL                 int_bottomhalf = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */

HI_BOOL                 use_bottomhalf = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */
HI_U32                  port_init_delay = 0;         /* Port intertupt delay value */

osal_spinlock_t g_stIspLock;

unsigned int isp_irq = ISP_IRQ_NR;

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
#endif

#if 0
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
        HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK)) = 0x43;
        HW_REG(IO_ADDRESS_PORT(ISP_INT_BE_MASK)) = 0x3;
        HW_REG(IO_ADDRESS_PORT(ISP_AF_INT_MASK)) = 0x1;    /*AF MASK*/
    }
    else
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK)) = 0x0;
        HW_REG(IO_ADDRESS_PORT(ISP_INT_BE_MASK)) = 0x0;
        HW_REG(IO_ADDRESS_PORT(ISP_AF_INT_MASK)) = 0x0;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_WaitConditionCallback(HI_VOID* pParam)
{
    HI_BOOL bCondition;

    bCondition = *(HI_BOOL*)pParam;

    return (HI_TRUE == bCondition);
}

HI_S32 ISP_WaitConditionCallbackEx(HI_VOID* pParam)
{
    HI_BOOL bCondition;
    HI_U32 *value = (HI_U32 *)pParam;

    bCondition = value[0] & value[1];

    return (HI_TRUE == bCondition);
}

HI_S32 ISP_GetFrameEdge(ISP_DEV IspDev, HI_U32 *pu32Status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWait, ISP_WaitConditionCallback,
                pstDrvCtx->abEdge,  (200));
    if (s32Ret < 0)
    {
        return -ERESTARTSYS;
    }
    else if (s32Ret == 0)
    {
        ISP_TRACE(HI_DBG_WARN, "Wait frame edge timeout!\n");
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->abEdge[0] = HI_FALSE;
    pstDrvCtx->abEdge[1] = HI_FALSE;
    *pu32Status = pstDrvCtx->u32Status;
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

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

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->abVd[0] = HI_FALSE;
    pstDrvCtx->abVd[1] = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    if(0 != u32MilliSec)
    {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVd, ISP_WaitConditionCallback,
            pstDrvCtx->abVd,  (u32MilliSec));
        if (s32Ret < 0)
        {
            return -ERESTARTSYS;
        }
        else if (s32Ret == 0)
        {
            ISP_TRACE(HI_DBG_WARN, "Wait Vd frame edge timeout!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVd, ISP_WaitConditionCallback, pstDrvCtx->abVd);
        if(s32Ret)
        {
            return -ERESTARTSYS;
        }
    }

    *pu32status = pstDrvCtx->u32Status;

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

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->abVdEnd[0] = HI_FALSE;
    pstDrvCtx->abVdEnd[1] = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    if(0 != u32MilliSec)
    {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_WaitConditionCallbackEx,
            pstDrvCtx->abVdEnd,  (u32MilliSec));
        if (s32Ret < 0)
        {
            return -ERESTARTSYS;
        }
        else if (s32Ret == 0)
        {
            ISP_TRACE(HI_DBG_WARN, "Wait Vd frame-end edge timeout!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_WaitConditionCallbackEx, pstDrvCtx->abVdEnd);
        if(s32Ret)
        {
            return -ERESTARTSYS;
        }
    }

    *pu32status = 1;

    return 0;
}


static HI_S32 ISP_SetRegNewer(ISP_DEV IspDev)
{
    HW_REG(IO_ADDRESS_PORT(ISP_444_REG_NEWER)) = 0x1;
    return HI_SUCCESS;
}

#if 0
HI_S32 ISP_DRV_StatisticsRead(ISP_DEV IspDev, ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_STAT_S *pstStat = HI_NULL;
    HI_S32 i, j;
    HI_U32 u32Value;
    HI_U32 u32AveMem;
    ISP_STAT_KEY_U unStatkey;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;
    if (HI_NULL == pstStat)
    {
        return HI_FAILURE;
    }

    unStatkey.u32Key = pstStatInfo->unKey.bit16IsrAccess;

#ifdef TEST_TIME
    {
        osal_printk("%x\n", g_test_pviraddr);

        osal_gettimeofday(&time1);
        for(i = 0; i < 1024; i++)
        {
            pstStat->stAeStat3.au32HistogramMemArray[i] = *(g_test_pviraddr + i);
        }
        osal_gettimeofday(&time2);
        osal_printk("read memory %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        osal_gettimeofday(&time1);
        osal_memcpy(pstStat->stAeStat3.au16HistogramMemArray, g_test_pviraddr, 256*2);
        osal_gettimeofday(&time2);
        osal_printk("memory copy %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        osal_gettimeofday(&time1);
        for(i = 0; i < 1024; i++)
        {
            pstStat->stAeStat3.au32HistogramMemArray[i] =
            IO_RD_ISP_ADDRESS(HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
        }
        osal_gettimeofday(&time2);
        osal_printk("read register %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
    }
#endif

#if 0
    if (unStatkey.bit1AeStat1)
    {
        pstStat->stAeStat1.au16MeteringHist[0] = hi_isp_metering_hist_0_read();
        pstStat->stAeStat1.au16MeteringHist[1] = hi_isp_metering_hist_1_read();
        pstStat->stAeStat1.au16MeteringHist[2] = hi_isp_metering_hist_3_read();
        pstStat->stAeStat1.au16MeteringHist[3] = hi_isp_metering_hist_4_read();
        pstStat->stAeStat1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
        pstStat->stAeStat1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
        pstStat->stAeStat1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
        pstStat->stAeStat1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();
    }
#endif

    if (unStatkey.bit1AwbStat3)
    {
        pstStat->stAwbStat3.u16MeteringAwbAvgR = hi_isp_metering_avg_r_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbAvgG = hi_isp_metering_avg_g_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbAvgB = hi_isp_metering_avg_b_read(IspDev);
        pstStat->stAwbStat3.u16MeteringAwbCountAll = hi_isp_metering_count_all_read(IspDev);
        //pstStat->stAwbStat3.u16MeteringAwbCountMin = hi_isp_metering_count_min_read();
        //pstStat->stAwbStat3.u16MeteringAwbCountMax = hi_isp_metering_count_max_read();
    }

    if (unStatkey.bit1AfStat)
    {
        HI_U32 u32Zone = hi_area_block_distribute_data_read_af();
        HI_U8 u8Col = (u32Zone & 0x1F);
        HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);
        HI_U32 u32V1H1, u32V2H2, u32Y;
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
                u32V1H1 = hi_area_stat_mem_array_data_read_af_v1h1(i * u8Col + j);
                pstStat->stAfStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & u32V1H1) >> 16);
                pstStat->stAfStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & u32V1H1);
                u32V2H2 = hi_area_stat_mem_array_data_read_af_v2h2(i * u8Col + j);
                pstStat->stAfStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & u32V2H2) >> 16);
                pstStat->stAfStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & u32V2H2);
                u32Y = hi_area_stat_mem_array_data_read_af_y(i * u8Col + j);
                pstStat->stAfStat.stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & u32Y);
                pstStat->stAfStat.stZoneMetrics[i][j].u16HlCnt = (HI_U16)((0xFFFF0000 & u32Y) >> 16);
            }
        }

    }

    if (unStatkey.bit1DpStat)
    {
        //pstStat->stDpStat.u16DefectPixelCount = hi_isp_defect_pixel_defect_pixel_count_read();
    }

    if (unStatkey.bit1CommStat)
    {
        pstStat->stCommStat.au16WhiteBalanceGain[0] = hi_isp_white_balance_gain_r_read(IspDev);
        pstStat->stCommStat.au16WhiteBalanceGain[1] = hi_isp_white_balance_gain_gr_read(IspDev);
        pstStat->stCommStat.au16WhiteBalanceGain[2] = hi_isp_white_balance_gain_gb_read(IspDev);
        pstStat->stCommStat.au16WhiteBalanceGain[3] = hi_isp_white_balance_gain_b_read(IspDev);
    }

#if 0
    if (unStatkey.bit1AeStat2)
    {
        pstStat->stAeStat2.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
        pstStat->stAeStat2.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
        pstStat->stAeStat2.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
        pstStat->stAeStat2.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();

        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
            #if 0
                u32Hist10 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 0);
                u32Hist43 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 1);
            #else
                u32Hist10 = IO_RD_ISP_ADDRESS(HI_AE_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 0) << 2));
                u32Hist43 = IO_RD_ISP_ADDRESS(HI_AE_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 1) << 2));
            #endif

                /* todo : optimize,read first, then calculate */
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][0] = (HI_U16)(u32Hist10 & 0xffff);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][1] = (HI_U16)((u32Hist10 & 0xffff0000) >> 16);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][3] = (HI_U16)(u32Hist43 & 0xffff);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][4] = (HI_U16)((u32Hist43 & 0xffff0000) >> 16);
            }
        }
    }
#endif



    if (unStatkey.bit1AeStat3)
    {
        hi_isp_ae_mem_hist_raddr_write(IspDev, 0);

        for(i = 0; i < 1024; i++)
        {
            pstStat->stAeStat3.au32HistogramMemArray[i] = hi_isp_ae_mem_hist_rdata_read(IspDev);
        }
        pstStat->stAeStat3.u32PixelCount  = hi_isp_ae_pixel_selected_count_read(IspDev);
        pstStat->stAeStat3.u32PixelWeight = hi_isp_ae_pixel_selected_weight_read(IspDev);
    }

    if (unStatkey.bit1AeStat4)
    {
        pstStat->stAeStat4.u16GlobalAvgR  = hi_isp_ae_total_r_aver_read(IspDev);
        pstStat->stAeStat4.u16GlobalAvgGr = hi_isp_ae_total_gr_aver_read(IspDev);
        pstStat->stAeStat4.u16GlobalAvgGb = hi_isp_ae_total_gb_aver_read(IspDev);
        pstStat->stAeStat4.u16GlobalAvgB  = hi_isp_ae_total_b_aver_read(IspDev);
    }

    if (unStatkey.bit1AeStat5)
    {
        hi_isp_ae_mem_aver_r_gr_raddr_write(IspDev, 0);
        hi_isp_ae_mem_aver_gb_b_raddr_write(IspDev, 0);
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                u32AveMem = hi_isp_ae_mem_aver_r_gr_rdata_read(IspDev);

                pstStat->stAeStat5.au16ZoneAvg[i][j][0] = (HI_U16)((u32AveMem & 0xffff0000) >> 16);
                pstStat->stAeStat5.au16ZoneAvg[i][j][1] = (HI_U16)((u32AveMem & 0xffff));

                u32AveMem = hi_isp_ae_mem_aver_gb_b_rdata_read(IspDev);

                pstStat->stAeStat5.au16ZoneAvg[i][j][2] = (HI_U16)((u32AveMem & 0xffff0000) >> 16);
                pstStat->stAeStat5.au16ZoneAvg[i][j][3] = (HI_U16)((u32AveMem & 0xffff));
            }
        }
    }

    if (unStatkey.bit1AeStat6)
    {
        hi_isp_ae_wdr_hist_raddr_write(IspDev, 0, 0);
        hi_isp_ae_wdr_hist_raddr_write(IspDev, 1, 0);
        hi_isp_ae_wdr_hist_raddr_write(IspDev, 2, 0);
        hi_isp_ae_wdr_hist_raddr_write(IspDev, 3, 0);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat6.au32PreHistogram1[i] = hi_isp_ae_wdr_hist_rdata_read(IspDev, 0);
            pstStat->stAeStat6.au32PreHistogram2[i] = hi_isp_ae_wdr_hist_rdata_read(IspDev, 1);
            pstStat->stAeStat6.au32PreHistogram3[i] = hi_isp_ae_wdr_hist_rdata_read(IspDev, 2);
            pstStat->stAeStat6.au32PreHistogram4[i] = hi_isp_ae_wdr_hist_rdata_read(IspDev, 3);
        }
        for(i = 0; i < 4; i++)
        {
            pstStat->stAeStat6.au32PixelCount[i]  = hi_isp_ae_wdr_total_pixels_read(IspDev, i);
            pstStat->stAeStat6.au32PixelWeight[i] = hi_isp_ae_wdr_count_pixels_read(IspDev, i);
        }
    }

    if (unStatkey.bit1AwbStat4)
    {
        hi_isp_awb_stat_raddr_write(IspDev, 0);

        for (i=0; i<AWB_ZONE_COLUMN*AWB_ZONE_ROW; i++)
        {
            u32Value = hi_isp_awb_stat_rdata_read(IspDev);
            pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] = (u32Value & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] = ((u32Value >> 16) & 0xffff);
            u32Value = hi_isp_awb_stat_rdata_read(IspDev);
            pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] = (u32Value & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i] = ((u32Value >> 16) & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayCountMin[i] = 0;
            pstStat->stAwbStat4.au16MeteringMemArrayCountMax[i] = 0;
        }
    }

    if(unStatkey.bit1Dehaze)
    {

         j = DEFOG_ZONE_NUM / 2;
         hi_isp_dehaze_minstat_raddr_write(IspDev, 0);
         for(i = 0; i < j; i++)
        {
            //pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_vi_dehaze_min_stat_read(i);
            pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_isp_dehaze_minstat_rdata_read(IspDev);
        }
        #if 0
        for(i = 0; i < j; i++)
        {
            HI_U32 u32Value = hi_vi_dehaze_min_stat_read(i);
            pstStat->stDehazeStat.au8DehazeMinDout[4*i]   = (u32Value >> 24) & 0xff;
            pstStat->stDehazeStat.au8DehazeMinDout[4*i+1] = (u32Value >> 16) & 0xff;
            pstStat->stDehazeStat.au8DehazeMinDout[4*i+2] = (u32Value >>  8) & 0xff;
            pstStat->stDehazeStat.au8DehazeMinDout[4*i+3] = (u32Value      ) & 0xff;
        }
       for(i = 0; i < 1024; i++)
        {
            pstStat->stDehazeStat.au32DehazeMaxStatDout[i] = hi_vi_dehaze_max_stat_read(i);
        }
        for(i = 0; i < 32; i++)
        {
            pstStat->stDehazeStat.au32Hist[i] = hi_vi_dehaze_hist_read(i);
        }
        #endif
    }

    if (unStatkey.bit1MgStat)
    {
        hi_isp_mg_mem_aver_raddr_write(IspDev, 0);
        for(i = 0;i < MG_ZONE_ROW ; i++)
        {
            for(j=0;j< MG_ZONE_COLUMN ;j++)
            {
                u32AveMem = hi_isp_mg_mem_aver_rdata_read(IspDev);

                pstStat->stMgStat.au16ZoneAvg[i][j][0] = (HI_U16)((u32AveMem & 0xff000000) >> 24);
                pstStat->stMgStat.au16ZoneAvg[i][j][1] = (HI_U16)((u32AveMem & 0xff0000) >> 16);
                pstStat->stMgStat.au16ZoneAvg[i][j][2] = (HI_U16)((u32AveMem & 0xff00) >> 8);
                pstStat->stMgStat.au16ZoneAvg[i][j][3] = (HI_U16)((u32AveMem & 0xff));
            }
        }
    }

    /* copy stat to shadow mem */
    // abandon this frame while user were operating
    if (HI_TRUE != pstDrvCtx->stStatShadowMem.bUsrAccess)
    {
        osal_memcpy(pstDrvCtx->stStatShadowMem.pVirtAddr, pstStat, sizeof(ISP_STAT_S));
    }

    return HI_SUCCESS;
}
#endif

HI_S32 ISP_DRV_StatDMABufInit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    HI_U32 source[10]={0x113a208c,0x113a218c,0x113a209c,0x113a20ac,0x113a908c,0x113a918c,0x113a928c,0x113a938c,0x113a259c,0x1139228c};

   pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

   osal_memcpy(pstDrvCtx->stStatDMABuf.source, source, sizeof(HI_U32)*10);

   return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatDMABufExit(HI_VOID)
{
    return 0;
}


HI_S32 ISP_DRV_StatBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret, i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;
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

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = u32PhyAddr;
    pstDrvCtx->stStatisticsBuf.pVirAddr = (HI_VOID *)pu8VirAddr;
    pstDrvCtx->stStatisticsBuf.u32Size  = u32Size;

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

    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->stStatisticsBuf.u32PhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stStatisticsBuf.pVirAddr;

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->stStatisticsBuf.pVirAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.u32PhyAddr = 0;

    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);



    if (0 != u32PhyAddr)
    {
        hil_mmb_invalid_cache_byaddr(pu8VirAddr, u32PhyAddr, pstDrvCtx->stStatisticsBuf.u32Size);
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
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

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        ISP_TRACE(HI_DBG_WARN, "busy list empty\n");
        *ppstStatInfo = HI_NULL;
        osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);
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
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    return HI_SUCCESS;
}

HI_VOID start_dma(HI_U32 IspDev, HI_U32 u32PhyAddr)
{
    HI_U32 dest[10];
    HI_U32 length[10]={1024,2048,256,256,256,256,256,256,256,1024};
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    dest[0]= u32PhyAddr;
    dest[1]= u32PhyAddr + 1024*4;
    dest[2]= u32PhyAddr + 3072*4;
    dest[3]= u32PhyAddr + 3328*4;
    dest[4]= u32PhyAddr + 3584*4;
    dest[5]= u32PhyAddr + 3840*4;
    dest[6]= u32PhyAddr + 4096*4;
    dest[7]= u32PhyAddr + 4352*4;
    dest[8]= u32PhyAddr + 4608*4;
    dest[9]= u32PhyAddr + 4864*4;

    hi_isp_awb_stat_raddr_write(IspDev, 0);

    hi_isp_ae_mem_hist_raddr_write(IspDev, 0);

    hi_isp_ae_mem_aver_r_gr_raddr_write(IspDev, 0);
    hi_isp_ae_mem_aver_gb_b_raddr_write(IspDev, 0);


    hi_isp_ae_wdr_hist_raddr_write(IspDev, 0, 0);
    hi_isp_ae_wdr_hist_raddr_write(IspDev, 1, 0);
    hi_isp_ae_wdr_hist_raddr_write(IspDev, 2, 0);
    hi_isp_ae_wdr_hist_raddr_write(IspDev, 3, 0);

    hi_isp_mg_mem_aver_raddr_write(IspDev, 0);
    hi_area_stat_mem_array_data_af_raddr_write(0);

    osal_do_dma_llim2m_isp(pstDrvCtx->stStatDMABuf.source, dest, length, 10);
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

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    osal_list_for_each(plist, &pstDrvCtx->stStatisticsBuf.stUserList)
    {
        pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);
        HI_ASSERT(HI_NULL != pstNode);
        if (pstNode->stStatInfo.u32PhyAddr == pstStatInfo->u32PhyAddr)
        {
            bValid = HI_TRUE;
            pstNode->stStatInfo.unKey.u32Key = pstStatInfo->unKey.u32Key;
            break;
        }
    }

    if (!bValid)
    {
        ISP_TRACE(HI_DBG_ERR, "invalid stat info, phy 0x%x\n", pstStatInfo->u32PhyAddr);
        osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);
        return HI_FAILURE;
    }

    /* get user */
    HI_ASSERT(plist != HI_NULL);
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32UserNum--;

    /* put free */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
    pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufBusyPut(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

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
        return HI_FAILURE;
    }

    /* get free */
    plist = pstDrvCtx->stStatisticsBuf.stFreeList.next;
    HI_ASSERT(plist != HI_NULL);
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32FreeNum--;

    /* read statistics */
    pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);

    //pstNode->stStatInfo.unKey.u32Key = 0xffff;
    //ISP_DRV_StatisticsRead(IspDev, &pstNode->stStatInfo);

    pstDrvCtx->stStatisticsBuf.pstActStat = &pstNode->stStatInfo;

    pstDrvCtx->u32DMAVirAddr = (HI_U32)pstNode->stStatInfo.pVirtAddr;

    start_dma(IspDev, pstNode->stStatInfo.u32PhyAddr);

    /* put busy */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stBusyList);
    pstDrvCtx->stStatisticsBuf.u32BusyNum++;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_TransAF(ISP_AF_STAT_S *pstAFStat, HI_U32 pVirAddr)
{
    HI_U32*  pAFVirAddr;
    HI_U32 i, j;
    HI_U32 u32Zone = hi_area_block_distribute_data_read_af();
    HI_U8 u8Col = (u32Zone & 0x1F);
    HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);

    if ( u8Col > AF_ZONE_COLUMN )
    {
        u8Col = AF_ZONE_COLUMN;
    }

    if (u8Row > AF_ZONE_ROW)
    {
        u8Row = AF_ZONE_ROW;
    }

    pAFVirAddr = (HI_U32*)(pVirAddr + 4864 * 4);


    for (i = 0; i < u8Row; i++)
    {
        for (j = 0; j < u8Col; j++)
        {

            HI_U32 tmp1, tmp2, tmp3, k;
            k = (i * u8Col + j) * 4;

            tmp1 = pAFVirAddr[k];
            tmp2 = pAFVirAddr[k + 1];
            tmp3 = pAFVirAddr[k + 2];

            pstAFStat->stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & tmp1) >> 16);
            pstAFStat->stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & tmp1);
            pstAFStat->stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & tmp2) >> 16);
            pstAFStat->stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & tmp2);
            pstAFStat->stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & tmp3);
            pstAFStat->stZoneMetrics[i][j].u16HlCnt = (HI_U16)((0xFFFF0000 & tmp3) >> 16);

        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StaKernelGet(ISP_DEV IspDev, ISP_DRV_FOCUS_STATISTICS_S *pstFocusStat)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_AF_STAT_S *pstAFStat = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (HI_NULL == pstFocusStat)
    {
        ISP_TRACE(HI_DBG_ERR, "get statistic active buffer err, pstFocusStat is NULL!\n");
        return HI_FAILURE;
    }

    if (!pstDrvCtx->stStatShadowMem.pVirtAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "get statistic active buffer err, pVirtAddr is NULL!\n");
        return HI_FAILURE;
    }

   // pstStat = (ISP_STAT_S *)pstDrvCtx->stStatShadowMem.pVirtAddr;
    pstAFStat = (ISP_AF_STAT_S *)(pstDrvCtx->stStatShadowMem.pVirtAddr);


    ISP_DRV_TransAF(pstAFStat, pstDrvCtx->u32DMAVirAddr);

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    osal_memcpy(pstFocusStat, pstAFStat, sizeof(ISP_DRV_FOCUS_STATISTICS_S));
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_SwitchMode(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_U8 i = 0, j = 0;
    ISP_DEV IspDev = 0;
    ISP_SYNC_CFG_S      *pstSyncCfg;
    HI_U32  u32Ratio = 0x40;
    HI_U32  u32ExpoRatio = 0x3FF;
    HI_U32  u32PTIntMask = 0;
    //HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};
    //HI_U32  u32IspFEInputMode = 0;

    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    pstSyncCfg->u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;
    for (j=0; j<3; j++)
    {
        for (i=0; i<CFG2VLD_DLY_LIMIT; i++)
        {
            pstSyncCfg->u32ExpRatio[j][i] = pstDrvCtx->stWDRCfg.au32ExpRatio[j];
        }
    }
    u32Ratio = pstSyncCfg->u32ExpRatio[0][0];
    u32Ratio = (u32Ratio > EXP_RATIO_MAX) ? EXP_RATIO_MAX : u32Ratio;
    u32Ratio = (u32Ratio < EXP_RATIO_MIN) ? EXP_RATIO_MIN : u32Ratio;
    hi_isp_wdr_exporatio0_write(IspDev, MIN2(u32Ratio, 0XFFF));
    hi_isp_flick_exp_ratios0_write(IspDev, MIN2(u32Ratio, 0XFFF));
    u32ExpoRatio = (1024 * 64 / DIV_0_TO_1(MIN2(u32Ratio, 0XFFF)));

    u32ExpoRatio =  MIN2(u32ExpoRatio, 0X3FF);

    hi_isp_wdr_exporatio_r_write(IspDev, u32ExpoRatio);

    for (i=0; i<CFG2VLD_DLY_LIMIT; i++)
    {
        pstSyncCfg->u32DRCComp[i] = 0x1000;
    }
    hi_isp_drc_exposure_write(IspDev, pstSyncCfg->u32DRCComp[0]);
    //hi_isp_dg_rgain_write(IspDev, 0X100);
    //hi_isp_dg_bgain_write(IspDev, 0X100);
    //hi_isp_dg_grgain_write(IspDev, 0X100);
    //hi_isp_dg_gbgain_write(IspDev, 0X100);

    //osal_printk("SwitchMode Ratio: %d DRCComp: %d\n", pstSyncCfg->u32ExpRatio[0][0], pstSyncCfg->u32DRCComp[0]);

    /* init cfg when modes change */
    //if (pstSyncCfg->u8WDRMode != pstSyncCfg->u8PreWDRMode)
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
            au32ChnSwitch[0] = 0;
            au32ChnSwitch[1] = 1;
            au32ChnSwitch[2] = 2;
            au32ChnSwitch[3] = 3;
            //bChnSwitchEnable = HI_TRUE;
            //u32IspFEInputMode = 1;
        }
        else if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            au32ChnSwitch[0] = 1 % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            //bChnSwitchEnable = HI_TRUE;
            //u32IspFEInputMode = 1;
        }
        else if (IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            if(IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 1;
                au32ChnSwitch[2] = 2;
                au32ChnSwitch[3] = 3;
                //bChnSwitchEnable = HI_TRUE;
                //u32IspFEInputMode = 1;
            }
            else if(IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 2;
                au32ChnSwitch[2] = 1;
                au32ChnSwitch[3] = 3;
                //bChnSwitchEnable = HI_TRUE;
                //u32IspFEInputMode = 1;
            }
            else if(IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 3;
                au32ChnSwitch[2] = 2;
                au32ChnSwitch[3] = 1;
                //bChnSwitchEnable = HI_TRUE;
                //u32IspFEInputMode = 1;
            }
            else
            {
            }

        }
        else
        {
            au32ChnSwitch[0] = 0;
            au32ChnSwitch[1] = 1;
            au32ChnSwitch[2] = 2;
            au32ChnSwitch[3] = 3;
            //bChnSwitchEnable = HI_FALSE;
            //u32IspFEInputMode = 0;
        }
        hi_isp_top_chn_switch0_write(IspDev, au32ChnSwitch[0]);
        hi_isp_top_chn_switch1_write(IspDev, au32ChnSwitch[1]);
        hi_isp_top_chn_switch2_write(IspDev, au32ChnSwitch[2]);
        hi_isp_top_chn_switch3_write(IspDev, au32ChnSwitch[3]);

        /* VC config */
        if ((IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode)) || (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)))
        {
            hi_vi_top_vc_enable_write(1);
            hi_vi_top_vc_max_write(pstSyncCfg->u8VCNumMax);
            hi_vi_top_vc_reset_write(1);
        }
        else
        {
            hi_vi_top_vc_enable_write(0);
            hi_vi_top_vc_max_write(0);
            //hi_vi_top_vc_reset_write(1);
        }

        /* pt_int_mask */
        u32PTIntMask = HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK));
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
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = u32PTIntMask;
    }

    pstSyncCfg->u8PreWDRMode = pstSyncCfg->u8WDRMode;

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_GetSyncControlnfo(ISP_SYNC_CFG_S *pstSyncCfg)
{
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = NULL;

    /* get VC number form logic */
    // pstSyncCfg->u8VCNum++;
    // pstSyncCfg->u8VCNum = pstSyncCfg->u8VCNum % DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);
    pstSyncCfg->u8VCNum = (HW_REG(IO_ADDRESS_PORT(VI_PT0_ID)) & 0x30) >> 4;
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
    static HI_U64 u64CurSnsGain = 0, u64PreSnsGain = 0;
    HI_BOOL bErr = HI_FALSE;
    HI_U64 u64Tmp = 0;

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

        u64CurSnsGain = pstCurNode->stAERegCfg.u64Exposure;
        u64CurSnsGain = osal_do_div64(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[0]));
        u64CurSnsGain = u64CurSnsGain << 8;
        u64CurSnsGain = osal_do_div64(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32IspDgain));
    }
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

        for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
        {
            pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i-1];
        }
        pstSyncCfg->u8LFMode[0] = 1;
        pstSyncCfg->u32WDRGain[0][0] = 0x100;
    }
    else if((IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) && (ISP_FSWDR_AUTO_LONG_FRAME_MODE == pstCurNode->stAERegCfg.enFSWDRMode))
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
            u64Tmp = (u64Tmp * pstCurNode->stAERegCfg.u32WDRGain[0]) >> 8;
            while (u64Tmp > (0x1LL << 31))
            {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }

            u64Ratio = osal_do_div64(u64Ratio, DIV_0_TO_1(u64Tmp));
            pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
        }
		for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
		{
			pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i-1];
			pstSyncCfg->u32WDRGain[0][i] = pstSyncCfg->u32WDRGain[0][i-1];
		}
		pstSyncCfg->u8LFMode[0] = 2;
		pstSyncCfg->u32WDRGain[0][0] = pstCurNode->stAERegCfg.u32WDRGain[0];
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
            u64Tmp = (u64Tmp * pstCurNode->stAERegCfg.u32WDRGain[0]) >> 8;
            while (u64Tmp > (0x1LL << 31))
            {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }
            u64Ratio = osal_do_div64(u64Ratio, DIV_0_TO_1(u64Tmp));
            pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
        }
        for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
        {
            pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i-1];
            pstSyncCfg->u32WDRGain[0][i] = pstSyncCfg->u32WDRGain[0][i-1];
        }
        pstSyncCfg->u8LFMode[0] = 0;
        pstSyncCfg->u32WDRGain[0][0] = pstCurNode->stAERegCfg.u32WDRGain[0];
    }

    /* calculate AlgProc */
    if(IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode))
    {
        for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
        {
            pstSyncCfg->u8AlgProc[i] = pstSyncCfg->u8AlgProc[i-1];
        }
        pstSyncCfg->u8AlgProc[0] = pstCurNode->stWDRRegCfg.bAlgProcMDT;
    }

    /* calculate DRC compensation */
	u64CurExp = pstCurNode->stAERegCfg.u64Exposure;
    u64PreExp = pstPreNode->stAERegCfg.u64Exposure;
	while (u64CurExp > (0x1LL << 31) || u64PreExp > (0x1LL << 31))
    {
        u64CurExp >>= 1;
        u64PreExp >>= 1;
    }
    u64CurExp = u64CurExp * pstCurNode->stAERegCfg.u32WDRGain[0];
    u64PreExp = u64PreExp * pstPreNode->stAERegCfg.u32WDRGain[0];

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


HI_S32 ISP_DRV_RegConfigIsp(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i;
    ISP_DEV IspDev = 0;
    HI_U8  u8CfgNodeIdx, u8CfgNodeVC;
    HI_U32 u32IspDgain;
    //HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};
    HI_U32  u32Ratio[3] = {0x40};
    HI_U32  u32ExpoRatio;
    HI_U32  u32DrcComp = 0x100;
    //HI_U32  u32IspFEInputMode = 0;
    HI_U8 u8LFMode = 0;
    HI_U16 u32WDRGain = 0x100;
    static HI_U16 u16AlgProc = 1;
    static HI_U32 u32flrgtthHigh = HI_ISP_WDR_RGTHIGTHFL_DEFAULT;
    static HI_U32 u32flrgtthLow = HI_ISP_WDR_RGTLOWTHFL_DEFAULT;
    static HI_BOOL bLFM1st = HI_TRUE;
    static HI_BOOL bWDR1st = HI_TRUE;
    static HI_BOOL bMDT1st = HI_TRUE;
	static HI_BOOL bRatio1st = HI_TRUE;
	static HI_BOOL bRatio2nd = HI_TRUE;
    
    static ISP_AE_REG_CFG_2_S *pstAERegCfg = HI_NULL, *pstAERegCfg2 = HI_NULL;
    static ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL, *pstCfgNode2 = HI_NULL;
    //ISP_REG_CFG_S *pstRegCfgInfo = &pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegsInfoFlag];

    /* config Chn Switch / Exp Ratio / ISP Dgain */
    /* delay of config2valid of isp reg is 1 */
    if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            /* Channal Switch */
            au32ChnSwitch[0] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            //bChnSwitchEnable = HI_TRUE;

            {
                hi_isp_top_chn_switch0_write(IspDev, au32ChnSwitch[0]);
                hi_isp_top_chn_switch1_write(IspDev, au32ChnSwitch[1]);

            }

            //hi_isp_top_chn_switch2_write(au32ChnSwitch[2]);
            //hi_isp_top_chn_switch3_write(au32ChnSwitch[3]);
            //hi_vi_top_chn_switch_enble_write(bChnSwitchEnable);
        }
        else if (IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            if(IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 1;
                au32ChnSwitch[2] = 2;
                au32ChnSwitch[3] = 3;
                //bChnSwitchEnable = HI_TRUE;
                //u32IspFEInputMode = 1;
            }
            else if(IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 2;
                au32ChnSwitch[2] = 1;
                au32ChnSwitch[3] = 3;
                //bChnSwitchEnable = HI_TRUE;
                //u32IspFEInputMode = 1;
            }
            else if(IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                au32ChnSwitch[0] = 0;
                au32ChnSwitch[1] = 3;
                au32ChnSwitch[2] = 2;
                au32ChnSwitch[3] = 1;
                //bChnSwitchEnable = HI_TRUE;
                //u32IspFEInputMode = 1;
            }
            else
            {
            }
//
//            hi_isp_top_chn_switch0_write(au32ChnSwitch[0]);
//            hi_isp_top_chn_switch1_write(au32ChnSwitch[1]);
//            hi_isp_top_chn_switch2_write(au32ChnSwitch[2]);
//            hi_isp_top_chn_switch3_write(au32ChnSwitch[3]);
        }
        else
        {
        }

        /* Ratio */
        for (i=0; i<3; i++)
        {
            u32Ratio[i] = pstDrvCtx->stSyncCfg.u32ExpRatio[i][pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
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

    for (i=0; i<3; i++)
    {
        u32Ratio[i] = (u32Ratio[i] > EXP_RATIO_MAX) ? EXP_RATIO_MAX : u32Ratio[i];
        u32Ratio[i] = (u32Ratio[i] < EXP_RATIO_MIN) ? EXP_RATIO_MIN : u32Ratio[i];
    }

    hi_isp_wdr_exporatio0_write(IspDev, MIN2(u32Ratio[0], 0XFFF));
    hi_isp_flick_exp_ratios0_write(IspDev, MIN2(u32Ratio[0], 0XFFF));
    u32ExpoRatio = (1024 * 64 / DIV_0_TO_1(MIN2(u32Ratio[0], 0XFFF)));

    u32ExpoRatio =  CLIP_MAX(u32ExpoRatio,  0X3FF);

    hi_isp_wdr_exporatio_r_write(IspDev, u32ExpoRatio);

    u8LFMode = pstDrvCtx->stSyncCfg.u8LFMode[pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
    u32WDRGain = pstDrvCtx->stSyncCfg.u32WDRGain[0][pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
		
	if(u8LFMode == 1)
	{
		if(bLFM1st)
		{
			u16AlgProc = hi_isp_wdr_balgprocmdt_read(IspDev);
			u32flrgtthLow = hi_isp_wdr_flrgtth_low_read(IspDev);
			u32flrgtthHigh = hi_isp_wdr_flrgtth_high_read(IspDev);
			bLFM1st = HI_FALSE;
			bWDR1st = HI_TRUE;
		}
		hi_isp_wdr_flrgtth_low_write (IspDev, 0X3FFF);
		hi_isp_wdr_flrgtth_high_write(IspDev, 0X3FFF);
		hi_isp_wdr_balgprocmdt_write(IspDev, 0);
	}
	else if(u8LFMode == 2)
	{
		if(bLFM1st)
		{
			u16AlgProc = hi_isp_wdr_balgprocmdt_read(IspDev);
			u32flrgtthLow = hi_isp_wdr_flrgtth_low_read(IspDev);
			u32flrgtthHigh = hi_isp_wdr_flrgtth_high_read(IspDev);
			bLFM1st = HI_FALSE;
			bWDR1st = HI_TRUE;
		}

		if(u32Ratio[0] <= 0x44)
		{
			if(bRatio1st)
			{
				u16AlgProc = hi_isp_wdr_balgprocmdt_read(IspDev);
				u32flrgtthLow = hi_isp_wdr_flrgtth_low_read(IspDev);
				u32flrgtthHigh = hi_isp_wdr_flrgtth_high_read(IspDev);
				bRatio1st = HI_FALSE;
				bRatio2nd = HI_TRUE;
			}
			hi_isp_wdr_flrgtth_low_write (IspDev, 0X3FFF);
			hi_isp_wdr_flrgtth_high_write(IspDev, 0X3FFF);
			hi_isp_wdr_balgprocmdt_write(IspDev, 0);
		}
		else
		{
			if(bRatio2nd)
			{
				hi_isp_wdr_flrgtth_low_write (IspDev, u32flrgtthLow);
				hi_isp_wdr_flrgtth_high_write(IspDev, u32flrgtthHigh);
				if(u32WDRGain < 0x200)
				{	
					hi_isp_wdr_balgprocmdt_write(IspDev, u16AlgProc);
					bRatio1st = HI_TRUE;
					bRatio2nd = HI_FALSE;
				}
				else
				{
					hi_isp_wdr_balgprocmdt_write(IspDev, 0);
				}
			}
			else
			{				
				u16AlgProc = hi_isp_wdr_balgprocmdt_read(IspDev);
				u32flrgtthLow = hi_isp_wdr_flrgtth_low_read(IspDev);
				u32flrgtthHigh = hi_isp_wdr_flrgtth_high_read(IspDev);
			}
		}
	}
	else
	{	
		if(bWDR1st)
		{
			hi_isp_wdr_flrgtth_low_write (IspDev, u32flrgtthLow);
			hi_isp_wdr_flrgtth_high_write(IspDev, u32flrgtthHigh);
			hi_isp_wdr_balgprocmdt_write(IspDev, u16AlgProc);
			bWDR1st = HI_FALSE;
			bLFM1st = HI_TRUE;
		}
		if(u32WDRGain < 0x200)
		{
			if(!bMDT1st)
			{
				hi_isp_wdr_balgprocmdt_write(IspDev, u16AlgProc);
				bMDT1st = HI_TRUE;
			}
		}
		else
		{
			if(bMDT1st)
			{	
				u16AlgProc = hi_isp_wdr_balgprocmdt_read(IspDev);
				bMDT1st = HI_FALSE;
			}
			hi_isp_wdr_balgprocmdt_write(IspDev, 0);
		}

	}

    /* config drc compensation */
    u32DrcComp = pstDrvCtx->stSyncCfg.u32DRCComp[pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
    u32DrcComp = MIN2(u32DrcComp, 0xFFFF);

    hi_isp_drc_exposure_write(IspDev, u32DrcComp);

    /* config isp_dgain */
    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
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

    u32IspDgain = pstAERegCfg->u32IspDgain;
    u32IspDgain = CLIP3(u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    u32WDRGain = CLIP3(u32WDRGain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);

    hi_isp_dg_rgain_write(IspDev, u32IspDgain);
    hi_isp_dg_bgain_write(IspDev, u32IspDgain);
    hi_isp_dg_grgain_write(IspDev, u32IspDgain);
    hi_isp_dg_gbgain_write(IspDev, u32IspDgain);

    hi_isp_4dg_0_rgain_write(IspDev,u32WDRGain);
    hi_isp_4dg_0_grgain_write(IspDev,u32WDRGain);
    hi_isp_4dg_0_gbgain_write(IspDev,u32WDRGain);
    hi_isp_4dg_0_bgain_write(IspDev,u32WDRGain);
    hi_isp_4dg_1_rgain_write(IspDev,ISP_DIGITAL_GAIN_MIN);
    hi_isp_4dg_1_grgain_write(IspDev,ISP_DIGITAL_GAIN_MIN);
    hi_isp_4dg_1_gbgain_write(IspDev,ISP_DIGITAL_GAIN_MIN);
    hi_isp_4dg_1_bgain_write(IspDev,ISP_DIGITAL_GAIN_MIN);

    pstCfgNode2 = pstDrvCtx->stSyncCfg.apstNode[0];
    if (HI_NULL == pstCfgNode2)
    {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
    {
        pstAERegCfg2 = &pstCfgNode->stAERegCfg;
    }

    if (HI_NULL == pstAERegCfg2)
    {
        return HI_SUCCESS;
    }

    if(pstAERegCfg2->bPirisValid == HI_TRUE)
    {
        if (HI_NULL != pstDrvCtx->stPirisCb.pfn_piris_gpio_update)
        {
            pstDrvCtx->stPirisCb.pfn_piris_gpio_update(&pstAERegCfg2->s32PirisPos);
        }
    }

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_RegConfigSensor(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i = 0;
    HI_U8 u8CfgNodeIdx = 0, u8CfgNodeVC = 0;
    HI_U8 u8WDRMode = WDR_MODE_NONE;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreCfgNode = HI_NULL;
    ISP_I2C_DATA_S *pstI2cData = HI_NULL;
    ISP_SSP_DATA_S *pstSspData = HI_NULL;

    u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;
    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];
    if (HI_NULL == pstCurNode)
    {
        ISP_TRACE(HI_DBG_WARN, "NULL point!\n");
        return HI_SUCCESS;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        ISP_TRACE(HI_DBG_WARN, "Invalid Node!\n");
        return HI_SUCCESS;
    }

    if (0 == pstCurNode->stSnsRegsInfo.u32RegNum)
    {
        ISP_TRACE(HI_DBG_WARN, "Err u32RegNum!\n");
        return HI_SUCCESS;
    }

    for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
    {
        /* config i2c */
        if (ISP_SNS_I2C_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            u8CfgNodeIdx = (pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            u8CfgNodeVC = (pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
            {
                if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT-1)
                {
                    ISP_TRACE(HI_DBG_WARN, "DelayFrmNum error!\n");
                    return HI_FAILURE;
                }

                pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
                pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

                if (HI_NULL == pstCfgNode)
                {
                    continue;
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
                            continue;
                        }
                    }
                }

                if (HI_TRUE == pstCfgNode->bValid)
                {
                    pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];
                    if ((pstI2cData->bUpdate == HI_TRUE)&& (pstDrvCtx->u32IntPos == pstI2cData->u8IntPos))
                    {
                        if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteSensorData)
                        {
                            pstDrvCtx->stBusCb.pfnISPWriteSensorData(pstI2cData);
                        }
                    }
                }
                else
                {
                    ;
                }
            }
        }
        /* config ssp */
        else if (ISP_SNS_SSP_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            u8CfgNodeIdx = (pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            u8CfgNodeVC = (pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
            {
                if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT-1)
                {
                    ISP_TRACE(HI_DBG_WARN, "DelayFrmNum error!\n");
                    return HI_FAILURE;
                }

                pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
                pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

                if (HI_NULL == pstCfgNode)
                {
                    continue;
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
                            continue;
                        }
                    }
                }

                if (HI_TRUE == pstCfgNode->bValid)
                {
                    pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];
                    if ((pstSspData->bUpdate == HI_TRUE)&& (pstDrvCtx->u32IntPos == pstSspData->u8IntPos))
                    {
                        if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteSensorData)
                        {
                            pstDrvCtx->stBusCb.pfnISPWriteSensorData(pstSspData);
                        }
                    }
                }
                else
                {
                    ;
                }
            }
        }
        else
        {

        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegisterBusCallBack (ISP_DEV IspDev, ISP_BUS_CALLBACK_S *pstBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstBusCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->stBusCb.pfnISPWriteSensorData = pstBusCb->pfnISPWriteSensorData;
    pstDrvCtx->stBusCb.pfnISPReadSensorData = pstBusCb->pfnISPReadSensorData;
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);
    return HI_SUCCESS;
}

HI_S32 ISP_RegisterPirisCallBack (ISP_DEV IspDev, ISP_PIRIS_CALLBACK_S *pstPirisCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstPirisCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
    pstDrvCtx->stPirisCb.pfn_piris_gpio_update = pstPirisCb->pfn_piris_gpio_update;
    osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_ProcInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ISPProc", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, ISP_PROC_SIZE);
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
        osal_seq_printf(s, "%s", (HI_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr);
    }
    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

#ifdef ENABLE_JPEGEDCF
HI_S32 ISP_GetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_UPDATE_INFO_S *pstIspUpdateInfo = HI_NULL;
    ISP_DCF_CONST_INFO_S *pstIspDCFConstInfo = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (NULL == pstDrvCtx->pUpdateInfoVirAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "UpdateInfo buf don't init ok!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    pstIspUpdateInfo = pstDrvCtx->pUpdateInfoVirAddr;
    pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S *)(pstDrvCtx->pUpdateInfoVirAddr + 1);

    osal_memcpy(pstIspDCF, pstIspDCFConstInfo, sizeof(HI_U8)*DCF_DRSCRIPTION_LENGTH*4);

    pstIspDCF->u32FNumber               = pstIspDCFConstInfo->u32FNumber;
    pstIspDCF->u32MaxApertureValue      = pstIspDCFConstInfo->u32MaxApertureValue;
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

    pstIspDCF->u16ISOSpeedRatings       = pstIspUpdateInfo->u16ISOSpeedRatings;
    pstIspDCF->u32ExposureTime          = pstIspUpdateInfo->u32ExposureTime;
    pstIspDCF->u32ExposureBiasValue     = pstIspUpdateInfo->u32ExposureBiasValue;
    pstIspDCF->u8ExposureProgram        = pstIspUpdateInfo->u8ExposureProgram;
    pstIspDCF->u8MeteringMode           = pstIspUpdateInfo->u8MeteringMode;
    pstIspDCF->u8ExposureMode           = pstIspUpdateInfo->u8ExposureMode;
    pstIspDCF->u8WhiteBalance           = pstIspUpdateInfo->u8WhiteBalance;

    return HI_SUCCESS;
}
#endif

HI_S32 ISP_GetIspUpdateInfo(ISP_DEV IspDev,ISP_UPDATE_INFO_S *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (NULL == pstDrvCtx->pUpdateInfoVirAddr)
    {
        ISP_TRACE(HI_DBG_ERR, "UpdateInfo buf don't init ok!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    osal_memcpy(pstIspUpdateInfo, pstDrvCtx->pUpdateInfoVirAddr, sizeof(ISP_UPDATE_INFO_S));

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

    u32Size = sizeof(ISP_UPDATE_INFO_S) + sizeof(ISP_DCF_CONST_INFO_S);
    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ISPUpdateInfo", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
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

    pstDrvCtx->u32UpdateInfoPhyAddr = u32PhyAddr;
    pstDrvCtx->pUpdateInfoVirAddr = (ISP_UPDATE_INFO_S *)pu8VirAddr;

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
    osal_up(&pstDrvCtx->stProcSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}



HI_S32 ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    if ( ((0 == proc_param) && (pstIspModParam->proc_param != 0)) ||
         ((0 != proc_param) && (pstIspModParam->proc_param == 0)) )
    {
        ISP_TRACE(HI_DBG_ERR, "proc_param do not support to change! %d to %d.\n",
            proc_param, pstIspModParam->proc_param);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if((pstIspModParam->u32UpdatePos!=0)&&(pstIspModParam->u32UpdatePos!=1))
    {
        ISP_TRACE(HI_DBG_ERR, "update_pos value do not support %d.\n",
        pstIspModParam->u32UpdatePos);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    proc_param = pstIspModParam->proc_param;
    update_pos = pstIspModParam->u32UpdatePos;
    port_init_delay = pstIspModParam->port_init_delay;

    return HI_SUCCESS;
}

HI_S32 ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    pstIspModParam->proc_param = proc_param;
    pstIspModParam->u32UpdatePos = update_pos;
    pstIspModParam->port_init_delay= port_init_delay;

    return HI_SUCCESS;
}

static long ISP_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    unsigned int   *argp = (unsigned int   *)arg;
    ISP_SYNC_CFG_BUF_S  *pstSyncCfgBuf = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S  *pstCurNode = HI_NULL;
    HI_S32  s32Ret;
    ISP_DEV IspDev = ISP_GET_DEV(private_data);
    unsigned long u32Flags;

    switch (cmd)
    {
        case ISP_DEV_SET_FD :
        {
            *((HI_U32*)(private_data)) = *(HI_U32*)arg;
            return 0;
        }

        #if 0
        case ISP_GET_VERSION :
        {
            osal_memcpy(&gs_stIspLibInfo, argp, sizeof(ISP_VERSION_S));
            gs_stIspLibInfo.u32Magic = VERSION_MAGIC + ISP_MAGIC_OFFSET;
            osal_memcpy(argp, &gs_stIspLibInfo, sizeof(ISP_VERSION_S));
            return 0;
        }
        #endif

        case ISP_GET_FRAME_EDGE :
        {
            HI_U32 u32Status = 0x0;
            ISP_CHECK_DEV(IspDev);
            s32Ret = ISP_GetFrameEdge(IspDev, &u32Status);
            if (s32Ret)
            {
                ISP_TRACE(HI_DBG_WARN, "Get Interrupt failed!\n");
                return HI_FAILURE;
            }

            osal_memcpy(argp, &u32Status, sizeof(HI_U32));
            return 0;
        }
        case ISP_GET_VD_TIMEOUT:
        {
            ISP_VD_TIMEOUT_S stVdTimeOut;
            ISP_CHECK_DEV(IspDev);

            osal_memcpy(&stVdTimeOut, argp, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdTimeOut(IspDev, &stVdTimeOut.stVdInfo,
                stVdTimeOut.u32MilliSec, (HI_U32*)&stVdTimeOut.s32IntStatus);

            stVdTimeOut.s32IntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK));

            osal_memcpy(argp, &stVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));
            return 0;
        }
        case ISP_GET_VD_END_TIMEOUT:
        {
            ISP_VD_TIMEOUT_S stVdTimeOut;
            ISP_CHECK_DEV(IspDev);

            osal_memcpy(&stVdTimeOut, argp, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdEndTimeOut(IspDev, &stVdTimeOut.stVdInfo,
                stVdTimeOut.u32MilliSec, (HI_U32*)&stVdTimeOut.s32IntStatus);

            //stVdTimeOut.s32IntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK));

            osal_memcpy(argp, &stVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));
            return 0;
        }
        case ISP_SET_REGNEWER:
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_SetRegNewer(IspDev);
        }
        case ISP_UPDATEINFO_BUF_INIT :
        {
            ISP_CHECK_DEV(IspDev);
            s32Ret = ISP_DRV_UpdateInfoBufInit(IspDev);

            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].u32UpdateInfoPhyAddr, sizeof(HI_U32));
            return s32Ret;
        }
        case ISP_UPDATEINFO_BUF_EXIT :
        {
            ISP_CHECK_DEV(IspDev);
            return ISP_DRV_UpdateInfoBufExit(IspDev);
        }
        case ISP_SET_INT_ENABLE :
        {
            HI_BOOL bEn;
            ISP_CHECK_DEV(IspDev);
            osal_memcpy(&bEn, argp, sizeof(HI_BOOL));
            return ISP_SetIntEnable(IspDev, bEn);
        }
        /* There should be two ioctl:init & get base phyaddr. */
        case ISP_STAT_BUF_INIT :
        {
            ISP_CHECK_DEV(IspDev);
            s32Ret = ISP_DRV_StatBufInit(IspDev);
            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].stStatisticsBuf.u32PhyAddr, sizeof(HI_U32));
            ISP_DRV_StatDMABufInit(IspDev);
            return s32Ret;
        }
        case ISP_STAT_BUF_EXIT :
        {
            ISP_CHECK_DEV(IspDev);
            ISP_DRV_StatDMABufExit();
            return ISP_DRV_StatBufExit(IspDev);
        }
        case ISP_STAT_BUF_GET :
        {
            ISP_STAT_INFO_S *pstStatInfo = HI_NULL;
            ISP_CHECK_DEV(IspDev);
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

            osal_memcpy(&stStatInfo, argp, sizeof(ISP_STAT_INFO_S));
            return ISP_DRV_StatBufUserPut(IspDev, &stStatInfo);
        }
        case ISP_STAT_ACT_GET:
        {
            ISP_CHECK_DEV(IspDev);
            
            if (!g_astIspDrvCtx[IspDev].stStatisticsBuf.pstActStat)
            {
                ISP_TRACE(HI_DBG_WARN, "get statistic active buffer err, stat not ready!\n");
                return HI_FAILURE;
            }
            osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
            osal_memcpy(argp, g_astIspDrvCtx[IspDev].stStatisticsBuf.pstActStat, sizeof(ISP_STAT_INFO_S));
            osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

            return 0;
        }
        case ISP_STAT_SHADOW_MEMSTATE_SET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_memcpy(&g_astIspDrvCtx[IspDev].stStatShadowMem.bUsrAccess, argp, sizeof(HI_BOOL));


            return 0;
        }

        case ISP_STAT_SHADOW_MEMPHY_GET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].stStatShadowMem.u32PhyAddr, sizeof(HI_U32));

            return 0;
        }

        case ISP_SYNC_CFG_SET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
            pstSyncCfgBuf = &g_astIspDrvCtx[IspDev].stSyncCfg.stSyncCfgBuf;
            if (ISPSyncBufIsFull(pstSyncCfgBuf))
            {
                ISP_TRACE(HI_DBG_ERR, "isp sync buffer is full\n");
                //osal_printk("isp sync buffer is full!\n");
                osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);
                return HI_FAILURE;
            }

            pstCurNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag];
            osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

            osal_memcpy(pstCurNode, argp, sizeof(ISP_SYNC_CFG_BUF_NODE_S));

            osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
            pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;
            osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

            return 0;
        }
        case ISP_WDR_CFG_SET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
            osal_memcpy(&g_astIspDrvCtx[IspDev].stWDRCfg, argp, sizeof(ISP_WDR_CFG_S));
            ISP_DRV_SwitchMode(&g_astIspDrvCtx[IspDev]);
            osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

            return 0;
        }

        case ISP_RES_SWITCH_SET:
        {
            ISP_RES_SWITCH_MODE_S   stSnsImageMode;
            ISP_CHECK_DEV(IspDev);
            osal_memcpy(&stSnsImageMode, argp, sizeof(ISP_RES_SWITCH_MODE_S));
            osal_spin_lock_irqsave(&g_stIspLock, &u32Flags);
            ISP_DRV_SwitchMode(&g_astIspDrvCtx[IspDev]);
            osal_spin_unlock_irqrestore(&g_stIspLock, &u32Flags);

            return 0;
        }

        case ISP_ACM_ATTR_GET:
        case ISP_ACM_ATTR_SET:
        case ISP_ACM_LUT_PHY_ADDR_GET:
        case ISP_ACM_COEF_SET:
        {
            return ISP_ACM_DRV_Ioctrl(cmd,arg,private_data);
        }

        case ISP_PROC_INIT:
        {
            ISP_CHECK_DEV(IspDev);
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
            HI_U32   *argp = (HI_U32   *)arg;

            osal_memcpy(argp, &proc_param, sizeof(HI_U32));
            return 0;
        }
        case ISP_MEM_INFO_SET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_memcpy(&g_astIspDrvCtx[IspDev].bMemInit, argp, sizeof(HI_BOOL));
            return 0;
        }
        case ISP_MEM_INFO_GET:
        {
            ISP_CHECK_DEV(IspDev);
            osal_memcpy(argp, &g_astIspDrvCtx[IspDev].bMemInit, sizeof(HI_BOOL));
            return 0;
        }
        case ISP_PWM_NUM_GET:
        {
            HI_U32   *argp = (HI_U32   *)arg;

            osal_memcpy(argp, &pwm_num, sizeof(HI_U32));
            return 0;
        }
        case ISP_SET_MOD_PARAM:
        {
           return ISP_SetModParam((ISP_MOD_PARAM_S *) argp);
        }
        case ISP_GET_MOD_PARAM:
        {
            ISP_MOD_PARAM_S stIspModParam;

            ISP_GetModParam(&stIspModParam);
            *(ISP_MOD_PARAM_S *)argp = stIspModParam;
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

#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_FALSE;


    /* For power saving. Open ISP clock and cancel ISP soft reset when module init */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif

    /* enable port0 int & isp int */
    u32VicapIntMask = HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK));
    HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK)) = (u32VicapIntMask | 0x10100);
    HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = (VI_PT0_INT_FSTART);
    HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK)) = (0x0);
    HW_REG(IO_ADDRESS_PORT(ISP_INT_BE_MASK)) = (0x0);

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
    .unlocked_ioctl = ISP_ioctl,
    .open       = ISP_open,
    .release    = ISP_close
};

struct osal_pmops stHiISPDrvOps = {
    .pm_freeze = ISP_Freeze,
    .pm_restore  = ISP_Restore
};

static osal_dev_t *s_stHiISPDevice = NULL;

static inline int ISP_ISR(int irq, void *id)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S* pstDrvCtx = HI_NULL;
    HI_U32 u32PortIntStatus = 0, u32IspIntFeStatus = 0, u32IspIntBeStatus = 0, u32PortTempIntStatus = 0;
    HI_U32 u32AFIntStatus = 0;

    ISP_CHECK_DEV(IspDev);

    u32PortIntStatus = HW_REG(IO_ADDRESS_PORT(VI_PT0_INT));
    u32PortTempIntStatus = u32PortIntStatus & VI_PT0_INT_ERR;

    u32PortIntStatus &= VI_PT0_INT_FSTART;

    u32IspIntFeStatus = HW_REG(IO_ADDRESS_PORT(ISP_INT_FE));
    u32IspIntFeStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK));

    u32AFIntStatus = HW_REG(IO_ADDRESS_PORT(ISP_AF_INT));

    u32AFIntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_AF_INT_MASK));

    u32AFIntStatus = (u32AFIntStatus & 0x1);


    u32IspIntBeStatus = HW_REG(IO_ADDRESS_PORT(ISP_INT_BE));
    u32IspIntBeStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_BE_MASK));

    HW_REG(IO_ADDRESS_PORT(0x0504)) = 1;
    HW_REG(IO_ADDRESS_PORT(0x1004)) = 1;

    if ((!u32PortIntStatus) && (!u32IspIntFeStatus) && (!u32IspIntBeStatus) && (!u32PortTempIntStatus))
    {
        return OSAL_IRQ_NONE;
    }

    if (u32IspIntFeStatus)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_FE)) = u32IspIntFeStatus;
    }

    if (u32IspIntBeStatus)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_BE)) = u32IspIntBeStatus;
    }

    if (u32AFIntStatus)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_AF_INT)) = u32AFIntStatus;
    }

    if (u32PortIntStatus)
    {
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT)) = VI_PT0_INT_FSTART;
    }

    /*When detect vi port's width&height changed,then reset isp*/
    if (u32PortTempIntStatus)
    {
        //printk("\nVI WIDTH&HEIGTH= 0x%x\n",HW_REG(IO_ADDRESS_PORT(0x01ec)));
        pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
        pstDrvCtx->stDrvDbgInfo.u32IspResetCnt++;
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT)) = u32PortTempIntStatus;
        HW_REG(IO_ISP_ADDRESS(ISP_RESET)) = 0x1;
        HW_REG(IO_ISP_ADDRESS(ISP_RESET)) = 0;

        return OSAL_IRQ_HANDLED;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    pstDrvCtx->stIntSch.u32IspIntFeStatus = u32IspIntFeStatus;
    pstDrvCtx->stIntSch.u32IspIntBeStatus = u32IspIntBeStatus;
    pstDrvCtx->stIntSch.u32IspIntAFStatus =  u32AFIntStatus;
    pstDrvCtx->stIntSch.u32PortIntStatus = u32PortIntStatus;

    if ((IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)))
    {
        HW_REG(IO_ADDRESS_PORT(VI_PT0_DELAY_ADDR)) = port_init_delay;
    }
    if (!use_bottomhalf)
    {
        ISP_IntUserBottomHalf(irq, id);
    }

    return (HI_TRUE == use_bottomhalf) ? OSAL_IRQ_WAKE_THREAD : OSAL_IRQ_HANDLED;
}

int DMA_ISR(int *p_dma_chn, int *p_dma_status)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    //pstDrvCtx->abEdge[1] = HI_TRUE;
    //pstDrvCtx->abVd[1] = HI_TRUE;
    pstDrvCtx->abVdEnd[1] = HI_TRUE;

    //osal_wakeup(&pstDrvCtx->stIspWait);
    //osal_wakeup(&pstDrvCtx->stIspWaitVd);
    osal_wakeup(&pstDrvCtx->stIspWaitVdEnd);
    
    IspSyncTaskProcess(IspDev);

    return OSAL_IRQ_HANDLED;
}


int ISP_IntBottomHalf(ISP_DEV IspDev)
{

    ISP_DRV_CTX_S* pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S* pstSyncCfg = HI_NULL;

    HI_U64 u64PtTime1 = 0, u64PtTime2 = 0;
    HI_U64 u64IspTime1 = 0, u64IspTime2 = 0;
    HI_U64 u64SensorCfgTime1 = 0, u64SensorCfgTime2 = 0;

    HI_U32 u32PortIntFStart;
    HI_U32 u32IspIntFeStatus;
    HI_U32 u32SensorCfgInt;
    HI_U32 u32IspIntAFStatus;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;
    pstDrvCtx->u32IntPos = 0;

    u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
    u32IspIntFeStatus = pstDrvCtx->stIntSch.u32IspIntFeStatus;
    u32IspIntAFStatus = pstDrvCtx->stIntSch.u32IspIntAFStatus;

    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    /* * * * * * * * vi_pt_int_process begin * * * * * * * */
    if (u32PortIntFStart)
    {
        /* port int proc */
        {
            pstDrvCtx->stDrvDbgInfo.u32PtIntCnt++;
            u64PtTime1 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime = u64PtTime1 - pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime;

            if (pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime > pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime;
            }

            pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime = u64PtTime1;
        }

        /* N to 1 WDR mode */
        if ((IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            //ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }

        /* port int proc */
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

        //return IRQ_HANDLED;
    }

    /* * * * * * * * isp_int_process begin * * * * * * * */
    if (u32IspIntFeStatus & 0x2) /* ISP Input formatter int (Frame Start int) */
    {
        /* use ISP AF int in linear/Built-in WDR mode; use ISP Frame start int in line WDR mode */
        if (IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);
            ISP_DRV_RegConfigSensor(pstDrvCtx);
        }
    }

    if (0 == update_pos) /* frame start */
    {
        u32SensorCfgInt = (u32IspIntFeStatus & 0x1);
    }
    else                 /* frame end */
    {
        u32SensorCfgInt = (u32IspIntAFStatus & 0x1);
    }

    if (u32SensorCfgInt)
    {
        /* use ISP AF int in linear/Built-in WDR mode; use ISP Frame start int in line WDR mode */
        if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) || IS_BUILT_IN_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    //if (u32IspIntAFStatus & 0x1)
    if (u32IspIntFeStatus & 0x40) /* MG ready int */
    {
        pstDrvCtx->u32IntPos = 1;
        ISP_DRV_RegConfigSensor(pstDrvCtx);
        if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_RegConfigIsp(pstDrvCtx);
        }  

        ISP_DRV_StatBufBusyPut(IspDev);
        pstDrvCtx->abVdEnd[0]   = HI_TRUE;
        osal_wakeup(&pstDrvCtx->stIspWaitVdEnd);
    }

    if ((u32IspIntFeStatus & 0x1)) /* ISP Fe Frame Start Int */
    {
        /* isp int proc */
        {
            pstDrvCtx->stDrvDbgInfo.u32IspIntCnt++;
            u64IspTime1 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime = u64IspTime1 - pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime;

            if (pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime > pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime;
            }

            pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime = u64IspTime1;
        }

        /* N to 1 fullrate frame WDR mode, get statistics only in the last frame(N-1) */
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            if (pstDrvCtx->stSyncCfg.u8VCNum != pstDrvCtx->stSyncCfg.u8VCNumMax)
            {
                /* isp int proc */
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

                return OSAL_IRQ_HANDLED;
            }
        }
//        else    /* linear or Built-in WDR mode */
//        {

//            if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) || IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
//            {

//                ISP_DRV_GetSyncControlnfo(pstSyncCfg);
//                ISP_DRV_CalcSyncCfg(pstSyncCfg);
//                ISP_DRV_RegConfigIsp(pstDrvCtx);

//                u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
//                ISP_DRV_RegConfigSensor(pstDrvCtx);
//                u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
//                pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
//            }
//        }

        pstDrvCtx->u32Status = (u32IspIntFeStatus & 0x1);

        //ISP_DRV_StatBufBusyPut(IspDev);

        pstDrvCtx->abEdge[0] = HI_TRUE;
        pstDrvCtx->abVd[0]   = HI_TRUE;

        HW_REG(IO_ADDRESS_PORT(ISP_444_REG_NEWER)) = 1;
        HW_REG(IO_ADDRESS_PORT(CH_REG_NEWER)) = 1;
        HW_REG(IO_ADDRESS_PORT(ISP_422_REG_NEWER)) = 1;

        osal_wakeup(&pstDrvCtx->stIspWait);
        osal_wakeup(&pstDrvCtx->stIspWaitVd);


        /* isp int proc */
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
    }

    return OSAL_IRQ_HANDLED;
}

int  ISP_IntUserBottomHalf(int irq, void *id)
{
    ISP_DEV IspDev = 0;

    ISP_CHECK_DEV(IspDev);

    ISP_IntBottomHalf(IspDev);

    return OSAL_IRQ_HANDLED;
}


extern void  *g_psViuAllReg;

static int ISP_DRV_Init(void)
{
    HI_U32 u32VicapIntMask;
    HI_S32 s32Ret;

    osal_memset(g_astIspDrvCtx, 0, sizeof(ISP_DRV_CTX_S) * 1);

    reg_vicap_base_va = g_psViuAllReg;
    reg_isp_base_va = (void *)((unsigned long)reg_vicap_base_va + ISP_BASE - VICAP_BASE);
    //reg_isp_base_va = reg_vicap_base_va + ISP_BASE - VICAP_BASE;

    /* enable port0 int & isp int */
    u32VicapIntMask = HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK));
    HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK)) = (u32VicapIntMask | 0x10100);
    HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = (VI_PT0_INT_FSTART);

    HW_REG(IO_ADDRESS_PORT(ISP_INT_FE_MASK)) = (0x0);
    HW_REG(IO_ADDRESS_PORT(ISP_INT_BE_MASK)) = (0x0);

#ifndef __HuaweiLite__
    if (int_bottomhalf)
    {
        use_bottomhalf = HI_TRUE;
    }
#endif

    if (use_bottomhalf)
    {
        if (osal_request_irq(isp_irq, ISP_ISR, ISP_IntUserBottomHalf, "ISP", (void*)&g_astIspDrvCtx))
        {
            osal_printk( "ISP Register Interrupt Failed!\n");
            return HI_FAILURE;
        }

        osal_dmac_register_isr(2, DMA_ISR);
    }
    else
    {
        if (osal_request_irq(isp_irq, ISP_ISR, HI_NULL, "ISP", (void*)&g_astIspDrvCtx))
        {
            osal_printk( "ISP Register Interrupt Failed!\n");
            return HI_FAILURE;
        }

        osal_dmac_register_isr(2, DMA_ISR);
    }

    osal_wait_init(&g_astIspDrvCtx[0].stIspWait);
    osal_wait_init(&g_astIspDrvCtx[0].stIspWaitVd);
    osal_wait_init(&g_astIspDrvCtx[0].stIspWaitVdEnd);
    g_astIspDrvCtx[0].abEdge[0] = HI_FALSE;
    g_astIspDrvCtx[0].abEdge[1] = HI_FALSE;
    g_astIspDrvCtx[0].abVd[0] = HI_FALSE;
    g_astIspDrvCtx[0].abVd[1] = HI_FALSE;
    g_astIspDrvCtx[0].abVdEnd[0] = HI_FALSE;
    g_astIspDrvCtx[0].abVdEnd[1] = HI_FALSE;
    g_astIspDrvCtx[0].bMemInit = HI_FALSE;
    osal_sema_init(&g_astIspDrvCtx[0].stProcSem, 1);

    ISP_ACM_DRV_Init();

    /* alloc isp stat shandow mem for application use */
    g_astIspDrvCtx[0].stStatShadowMem.u32Size = sizeof(ISP_AF_STAT_S);
    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ISP AF mem", &g_astIspDrvCtx[0].stStatShadowMem.u32PhyAddr,
                                  (HI_VOID**)&g_astIspDrvCtx[0].stStatShadowMem.pVirtAddr, g_astIspDrvCtx[0].stStatShadowMem.u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc ISP shadow mem err\n");
        return HI_ERR_ISP_NOMEM;
    }

    SyncTaskInit(0);

    return 0;
}

static int ISP_DRV_Exit(void)
{
    osal_dmac_channel_free(2);
    osal_free_irq(isp_irq, (void*)&g_astIspDrvCtx);

    SyncTaskDestroy(0);

    if (0 != g_astIspDrvCtx[0].stStatShadowMem.u32PhyAddr)
    {
        CMPI_MmzFree(g_astIspDrvCtx[0].stStatShadowMem.u32PhyAddr, g_astIspDrvCtx[0].stStatShadowMem.pVirtAddr);
    }
    ISP_ACM_DRV_Exit();
    osal_sema_destory(&g_astIspDrvCtx[0].stProcSem);
    
    osal_wait_destory(&g_astIspDrvCtx[0].stIspWait);
    osal_wait_destory(&g_astIspDrvCtx[0].stIspWaitVd);
    osal_wait_destory(&g_astIspDrvCtx[0].stIspWaitVdEnd);    
    reg_vicap_base_va = HI_NULL;
    reg_isp_base_va = HI_NULL;

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

static int ISP_ProcShow(osal_proc_entry_t *s)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U8 u8Bayer;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    osal_seq_printf(s, "\n[ISP] Version: ["MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n");
    osal_seq_printf(s, "\n");

    if(pstDrvCtx->bMemInit)
    {
        osal_seq_printf(s, "-----MODULE PARAM--------------------------------------------------------------\n");
        osal_seq_printf(s, " %15s"   " %15s"  " %15s"  " %15s\n",  "proc_param",  "update_pos" ,"port_init_delay" ,"pwm_num");
        osal_seq_printf(s, " %15u"   " %15u"  " %15u"  " %15u\n",   proc_param,  update_pos, port_init_delay, pwm_num);
        osal_seq_printf(s, "\n");
    }


    osal_seq_printf(s, "-----DRV INFO-------------------------------------------------------------------------------------\n");

    osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%12s\n"
            ,"IspDev","IntCnt","IntT","MaxIntT","IntGapT","MaxGapT","IntRat","IspResetCnt");

    osal_seq_printf(s, "%11u" "%11u" "%11u" "%11u" "%11u" "%11u" "%9u" "%12u\n",
            0,
            pstDrvCtx->stDrvDbgInfo.u32IspIntCnt,
            pstDrvCtx->stDrvDbgInfo.u32IspIntTime,
            pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime,
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32IspRate,
            pstDrvCtx->stDrvDbgInfo.u32IspResetCnt);

    osal_seq_printf(s, "\n");

    osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%11s\n"
            ,"","PtIntCnt","PtIntT","PtMaxIntT","PtIntGapT","PtMaxGapT","PtIntRat","SensorCfgT");

    osal_seq_printf(s, "%11s" "%11u" "%11u" "%11u" "%11u" "%11u" "%9u" "%11u\n",
            "",
            pstDrvCtx->stDrvDbgInfo.u32PtIntCnt,
            pstDrvCtx->stDrvDbgInfo.u32PtIntTime,
            pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime,
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32PtRate,
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime);

    osal_seq_printf(s, "\n");

    u8Bayer = hi_isp_yuv444_rggb_start_read(IspDev);
    osal_seq_printf(s, "-----PubAttr INFO---------------------------------------------------------------------------------\n");

    osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s\n"
            ,"WndX","WndY","WndW","WndH","Bayer");

    osal_seq_printf(s, "%12u" "%12u" "%12u" "%12u" "%12s\n\n",
            hi_isp_crop_x_start_read(IspDev),
            hi_isp_crop_y_start_read(IspDev),
            hi_isp_crop_width_read(IspDev) + 1,
            hi_isp_crop_height_read(IspDev) + 1,
            (0 == u8Bayer) ? "RGGB" :
                (1 == u8Bayer) ? "GRBG" :
                (2 == u8Bayer) ? "GBRG" :
                (3 == u8Bayer) ? "BGGR" : "BUTT");

    ISP_DRV_ProcPrintf(0, s);

    return 0;
}
#if 0
/* ate all blanks in a line */
static char *StripString(char *s, char *d)
{
    char *p = d;
    do{
        if (*s == '\n')
            *s = '\0';
        if (*s != ' ')
            *p++ = *s;
    }while(*s++ != '\0');
    return d;
}
#endif

HI_VOID ISPCheckModParam(HI_VOID)
{
    if (3 < pwm_num)
    {
        osal_printk("-----ModuleParam: pwm_num %d is illegal,should be [0,3], it will be set to 2!\n", pwm_num);
        pwm_num = 2;
    }

    if (1 < update_pos)
    {
        osal_printk("-----ModuleParam: update_pos %d is illegal,should be [0, 1], it will be set to 0!\n", update_pos);
        update_pos = 0;
    }
}

HI_S32 ISP_Init(void *p)
{
    osal_memset(&g_astIspDrvCtx[0].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    return HI_SUCCESS;
}

HI_VOID ISP_Exit(void)
{
    VREG_DRV_ReleaseAll();

    ISP_DRV_StatBufExit(0);
    ISP_DRV_ProcExit(0);

    return ;
}

ISP_EXPORT_FUNC_S g_stIspExpFunc =
{
    .pfnISPRegisterBusCallBack   = ISP_RegisterBusCallBack,
    .pfnISPRegisterPirisCallBack = ISP_RegisterPirisCallBack,
#ifdef ENABLE_JPEGEDCF
    .pfnISP_GetDCFInfo           = ISP_GetDCFInfo,
#else
    .pfnISP_GetDCFInfo           = HI_NULL,
#endif
    .pfnISP_GetIspUpdateInfo     = ISP_GetIspUpdateInfo,
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
    osal_proc_entry_t *proc=NULL;
#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_FALSE;

    /*low power strategy, open the ISP clock after the initialization of the ISP module ,and cancel the  soft reset of ISP*/
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
    ISPCheckModParam();
    s_stHiISPDevice = osal_createdev("isp_dev");
    s_stHiISPDevice->fops = &stIspFileOp;
    s_stHiISPDevice->osal_pmops = &stHiISPDrvOps;
    s_stHiISPDevice->minor = UMAP_ISP_MINOR_BASE;
    if (osal_registerdevice(s_stHiISPDevice) < 0)
    {
        HI_PRINT("Kernel: Could not register isp devices\n");
        osal_destroydev(s_stHiISPDevice);
        return HI_FAILURE;
    }

    proc = osal_create_proc_entry(PROC_ENTRY_ISP, NULL);
    if(HI_NULL == proc)
    {
        HI_PRINT("Kernel: Register isp proc failed!\n");
        goto OUT2;
    }
    proc->read = ISP_ProcShow;

    if (CMPI_RegisterModule(&s_stModule))
    {
        goto OUT1;
    }

    osal_spin_lock_init(&g_stIspLock);

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
    HI_PRINT("ISP Mod init!\n");
    return 0;

OUT1:
    osal_remove_proc_entry(PROC_ENTRY_ISP, HI_NULL);

OUT2:
    osal_deregisterdevice(s_stHiISPDevice);
    osal_destroydev(s_stHiISPDevice);

    HI_PRINT("ISP Mod init failed!\n");
    return HI_FAILURE;
}


void ISP_ModExit(void)
{
#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_TRUE;

#endif

    //g_stIspExpFunc.pfnISPRegisterBusCallBack = HI_NULL;
    //g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = HI_NULL;

    CMPI_UnRegisterModule(HI_ID_ISP);

    ISP_DRV_Exit();

    osal_remove_proc_entry(PROC_ENTRY_ISP,NULL);

    osal_deregisterdevice(s_stHiISPDevice);
    osal_destroydev(s_stHiISPDevice);
    osal_spin_lock_destory(&g_stIspLock);

#ifdef TEST_TIME
    ISP_Test_Exit();
#endif
#ifndef HI_FPGA
    /*low power strategy,  reset the ISP when unload the ISP module and then close the clock of the ISP*/
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
    HI_PRINT("ISP Mod Exit!\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
