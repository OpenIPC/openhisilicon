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


#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/errno.h>
#include <linux/kallsyms.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "hi_common.h"
#include "hi_comm_isp.h"
#include "hi_comm_vb.h"
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

#include "piris_ext.h"

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
#define VICAP_BASE              0x20580000

#define IO_ADDRESS_PORT(x)      (reg_vicap_base_va + x)
#define HW_REG(reg)             *((volatile unsigned int *)(reg))

#define ISP_BASE                0x205A0000
#define ISP_OFFSET              0x10000

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

#define VI_PT0_ID               0x01E0

#define ISP_INT_MASK            0x100F8
#define ISP_INT                 0x100F0

/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR             0x01A4

#define ISP_RESET               0x0078
#define CHN_SWITCH              0x10028

#define ISP_MAGIC_OFFSET        1
#define ISP_PROC_SIZE           0x10000 /* 10k */

#define IO_ISP_ADDRESS(x)      ((unsigned long)reg_isp_base_va + (x))
#define IO_RD_ISP_ADDRESS(x)   (*((unsigned long *)IO_ISP_ADDRESS(x)))


extern HI_BOOL VB_IsSupplementSupport(HI_U32 u32Mask);
#define CHIP_SUPPORT_JPEGEDCF()    (VB_IsSupplementSupport(VB_SUPPLEMENT_JPEG_MASK))

void ISP_IntBottomHalf(unsigned long data);

int isp_irq = -1;

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
ISP_DRV_CTX_S           g_astIspDrvCtx[1] = {{0}};

ISP_EXPORT_FUNC_S       g_stIspExpFunc = {0};
ISP_PIRIS_EXPORT_FUNC_S  g_stIspPirisExpFunc = {0};
static ISP_VERSION_S    gs_stIspLibInfo;
void __iomem            *reg_vicap_base_va = HI_NULL;
void __iomem            *reg_isp_base_va = HI_NULL;

HI_U32                  proc_param = 0;        /* 0: close proc; n: write proc info's interval int num */
HI_U32                  pwm_num = 5;
HI_U32                  int_timeout = 200;   /* The time(unit:ms) of interrupt timeout */
bool                    int_bottomhalf = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */

spinlock_t g_stIspLock;

//#define TEST_TIME

#ifdef TEST_TIME
HI_U32  g_test_phyaddr;
HI_U16 *g_test_pviraddr;
struct timeval time1;
struct timeval time2;
#endif
/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/

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

HI_S32 ISP_SetIntEnable(ISP_DEV IspDev, HI_BOOL bEn)
{
    if (bEn)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = 0x3;
    }
    else
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = 0x0;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetFrameEdge(ISP_DEV IspDev, HI_U32 *pu32Status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    s32Ret = wait_event_interruptible_timeout(pstDrvCtx->stIspWait,
                (pstDrvCtx->bEdge == HI_TRUE), msecs_to_jiffies(int_timeout));
    if (s32Ret <= 0)
    {
        #if 0
        printk(KERN_INFO  "Wait frame edge timeout!\n");
        #endif
        return -1;
    }

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->bEdge = HI_FALSE;
    *pu32Status = pstDrvCtx->u32Status;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

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

    if(down_interruptible(&pstDrvCtx->stIspSem))
    {
       return -ERESTARTSYS;
    }

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->bVd = HI_FALSE;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    if(0 != u32MilliSec)
    {
        s32Ret = wait_event_interruptible_timeout(pstDrvCtx->stIspWaitVd,
            (HI_TRUE == pstDrvCtx->bVd), msecs_to_jiffies(u32MilliSec));
        if (s32Ret <= 0)
        {
            #if 0
            printk(KERN_INFO  "Wait frame edge timeout!\n");
            #endif
            up(&pstDrvCtx->stIspSem);
            return -1;
        }
    }
    else
    {
        s32Ret = wait_event_interruptible(pstDrvCtx->stIspWaitVd, (HI_TRUE == pstDrvCtx->bVd));
        if(s32Ret)
        {
            up(&pstDrvCtx->stIspSem);
            return -1;
        }

    }

    *pu32status = pstDrvCtx->u32Status;

    up(&pstDrvCtx->stIspSem);

    return 0;
}

HI_S32 ISP_DRV_StatisticsRead(ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_STAT_S *pstStat = HI_NULL;
    HI_S32 i, j;
    HI_U32 u32Value, u32HighBit, u32LowBit;
    HI_U32 u32Sum;
    HI_U32 u32Hist10, u32Hist43;
    HI_U32 u32ZoneAvg1, u32ZoneAvg2;

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;
    if (HI_NULL == pstStat)
    {
        return HI_FAILURE;
    }

#ifdef TEST_TIME
    {
        printk("%x\n", g_test_pviraddr);

        do_gettimeofday(&time1);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au16HistogramMemArray[i] = *(g_test_pviraddr + i);
        }
        do_gettimeofday(&time2);
        printk("read memory %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        do_gettimeofday(&time1);
        memcpy(pstStat->stAeStat3.au16HistogramMemArray, g_test_pviraddr, 256*2);
        do_gettimeofday(&time2);
        printk("memory copy %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        do_gettimeofday(&time1);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au16HistogramMemArray[i] =
            IO_RD_ISP_ADDRESS(HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
        }
        do_gettimeofday(&time2);
        printk("read register %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
    }
#endif

    if (pstStatInfo->unKey.bit1AeStat1)
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

    if (pstStatInfo->unKey.bit1AwbStat1)
    {
        pstStat->stAwbStat1.u16MeteringAwbRg = hi_isp_metering_awb_rg_read();
        pstStat->stAwbStat1.u16MeteringAwbBg = hi_isp_metering_awb_bg_read();
        pstStat->stAwbStat1.u32MeteringAwbSum = hi_isp_metering_awb_sum_read();
    }

    if (pstStatInfo->unKey.bit1AwbStat3)
    {
        HI_U16 u16BlackLevelR = ((HI_U16)hi_isp_offset_black_00_read() << 4);
        HI_U16 u16BlackLevelG = ((HI_U16)hi_isp_offset_black_01_read() << 4);
        HI_U16 u16BlackLevelB = ((HI_U16)hi_isp_offset_black_11_read() << 4);

        pstStat->stAwbStat3.u16MeteringAwbAvgR = hi_isp_metering_avg_r_read();
        pstStat->stAwbStat3.u16MeteringAwbAvgG = hi_isp_metering_avg_g_read();
        pstStat->stAwbStat3.u16MeteringAwbAvgB = hi_isp_metering_avg_b_read();
        pstStat->stAwbStat3.u16MeteringAwbCountAll = hi_isp_metering_count_all_read();
        pstStat->stAwbStat3.u16MeteringAwbCountMin = hi_isp_metering_count_min_read();
        pstStat->stAwbStat3.u16MeteringAwbCountMax = hi_isp_metering_count_max_read();

        if ((pstStat->stAwbStat3.u16MeteringAwbAvgR >= u16BlackLevelR) &&
            (pstStat->stAwbStat3.u16MeteringAwbAvgG >= u16BlackLevelG) &&
            (pstStat->stAwbStat3.u16MeteringAwbAvgB >= u16BlackLevelB))
        {
            pstStat->stAwbStat3.u16MeteringAwbAvgR -= u16BlackLevelR;
            pstStat->stAwbStat3.u16MeteringAwbAvgG -= u16BlackLevelG;
            pstStat->stAwbStat3.u16MeteringAwbAvgB -= u16BlackLevelB;
        }
    }

    if (pstStatInfo->unKey.bit1AfStat)
    {
        HI_U32 u32Zone = hi_area_block_distribute_data_read_af();
        HI_U8 u8Col = ((u32Zone & 0x1F) > AF_ZONE_COLUMN) ? AF_ZONE_COLUMN : (u32Zone & 0x1F);
        HI_U8 u8Row = (((u32Zone & 0x1F00) >> 8) > AF_ZONE_ROW) ? AF_ZONE_ROW : ((u32Zone & 0x1F00) >> 8);
        for (i = 0; i < u8Row; i++)
        {
            for (j = 0; j < u8Col; j++)
            {
                pstStat->stAfStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & hi_area_stat_mem_array_data_read_af_v1h1(i * u8Col + j)) >> 16);
                pstStat->stAfStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & hi_area_stat_mem_array_data_read_af_v1h1(i * u8Col + j));
                pstStat->stAfStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & hi_area_stat_mem_array_data_read_af_v2h2(i * u8Col + j)) >> 16);
                pstStat->stAfStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & hi_area_stat_mem_array_data_read_af_v2h2(i * u8Col + j));
                pstStat->stAfStat.stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & hi_area_stat_mem_array_data_read_af_y(i * u8Col + j));
            }
        }
    }

    if (pstStatInfo->unKey.bit1DpStat)
    {
        pstStat->stDpStat.u16DefectPixelCount = hi_isp_defect_pixel_defect_pixel_count_read();
    }

    if (pstStatInfo->unKey.bit1CommStat)
    {
        pstStat->stCommStat.au16WhiteBalanceGain[0] = hi_isp_white_balance_gain_00_read();
        pstStat->stCommStat.au16WhiteBalanceGain[1] = hi_isp_white_balance_gain_01_read();
        pstStat->stCommStat.au16WhiteBalanceGain[2] = hi_isp_white_balance_gain_10_read();
        pstStat->stCommStat.au16WhiteBalanceGain[3] = hi_isp_white_balance_gain_11_read();
    }


    if (pstStatInfo->unKey.bit1AeStat2)
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

    if (pstStatInfo->unKey.bit1AeStat3)
    {
        for(i = 0; i < 256; i++)
        {
        #if 0
            pstStat->stAeStat3.au16HistogramMemArray[i] = (HI_U16)(hi_histogram_mem_array_data_read(i) & 0xffff);
        #else
            u32Value = IO_RD_ISP_ADDRESS(HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
            u32HighBit = u32Value >> 12;
            u32LowBit  = u32Value & 0xfff;
            if(u32HighBit == 0)
            {
                pstStat->stAeStat3.au32HistogramMemArray[i] = u32LowBit;
            }
            else
            {
                pstStat->stAeStat3.au32HistogramMemArray[i] = (4096 + u32LowBit) << (u32HighBit - 1);
            }
        #endif
        }
    }

    if (pstStatInfo->unKey.bit1AeStat4)
    {
        pstStat->stAeStat4.u16GlobalAvgR  = hi_isp_metering_sum_r_read();
        pstStat->stAeStat4.u16GlobalAvgGr = hi_isp_metering_sum_rg_read();
        pstStat->stAeStat4.u16GlobalAvgGb = hi_isp_metering_sum_bg_read();
        pstStat->stAeStat4.u16GlobalAvgB  = hi_isp_metering_sum_b_read();
    }

    if (pstStatInfo->unKey.bit1AeStat5)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                u32ZoneAvg1 = IO_RD_ISP_ADDRESS(HI_AE_SUM_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 0) << 2));
                u32ZoneAvg2 = IO_RD_ISP_ADDRESS(HI_AE_SUM_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 1) << 2));

                pstStat->stAeStat5.au16ZoneAvg[i][j][0] = (HI_U16)(u32ZoneAvg1 & 0xffff);              /*R */
                pstStat->stAeStat5.au16ZoneAvg[i][j][1] = (HI_U16)((u32ZoneAvg1 & 0xffff0000) >> 16);  /*Gr*/
                pstStat->stAeStat5.au16ZoneAvg[i][j][2] = (HI_U16)(u32ZoneAvg2 & 0xffff);              /*Gb*/
                pstStat->stAeStat5.au16ZoneAvg[i][j][3] = (HI_U16)((u32ZoneAvg2 & 0xffff0000) >> 16);  /*B */
            }
        }
    }

    if (pstStatInfo->unKey.bit1AwbStat2)
    {
        for (i=0; i<AWB_ZONE_ROW * AWB_ZONE_COLUMN; i++)
        {
            u32Value = IO_RD_ISP_ADDRESS(HI_AWB_METERING_MEM_BASE_ADDR + (i << 3));
            pstStat->stAwbStat2.au16MeteringMemArrayRg[i] = u32Value & 0xfff;
            pstStat->stAwbStat2.au16MeteringMemArrayBg[i] = (u32Value >> 16) & 0xfff;
            u32Value = IO_RD_ISP_ADDRESS(HI_AWB_METERING_MEM_BASE_ADDR + (i << 3) + 4);
            pstStat->stAwbStat2.au16MeteringMemArraySum[i] = u32Value;
        }
    }

    if (pstStatInfo->unKey.bit1AwbStat4)
    {
        HI_U16 u16BlackLevelR = ((HI_U16)hi_isp_offset_black_00_read() << 4);
        HI_U16 u16BlackLevelG = ((HI_U16)hi_isp_offset_black_01_read() << 4);
        HI_U16 u16BlackLevelB = ((HI_U16)hi_isp_offset_black_11_read() << 4);

        for (i=0; i<AWB_ZONE_ROW * AWB_ZONE_COLUMN; i++)
        {
            u32Value = IO_RD_ISP_ADDRESS(HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12));
            pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] = (u32Value & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] = ((u32Value >> 16) & 0xffff);
            u32Value = IO_RD_ISP_ADDRESS(HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 4);
            pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] = (u32Value & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i] = ((u32Value >> 16) & 0xffff);
            u32Value = IO_RD_ISP_ADDRESS(HI_AWB_SUM_METERING_MEM_BASE_ADDR + (i * 12) + 8);
            pstStat->stAwbStat4.au16MeteringMemArrayCountMax[i] = u32Value & 0xffff;
            pstStat->stAwbStat4.au16MeteringMemArrayCountMin[i] = (u32Value>>16) & 0xffff;

            if ((pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] >= u16BlackLevelR) &&
                (pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] >= u16BlackLevelG) &&
                (pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] >= u16BlackLevelB) &&
                (0 != pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i]))
            {
                pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] -= u16BlackLevelR;
                pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] -= u16BlackLevelG;
                pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] -= u16BlackLevelB;
            }
            else
            {
                pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] = 0;
                pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] = 0;
                pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] = 0;
                pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i] = 0;
            }
        }
    }


    if (pstStatInfo->unKey.bit1AeStat2)
    {
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
    }


    if(pstStatInfo->unKey.bit1Dehaze)
    {

         j = DEFOG_ZONE_NUM / 4;
         for(i = 0; i < j; i++)
        {
            pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_vi_dehaze_min_stat_read(i);
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
    return HI_SUCCESS;
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
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPStat", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    memset(pu8VirAddr, 0, u32Size);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = u32PhyAddr;
    pstDrvCtx->stStatisticsBuf.pVirAddr = (HI_VOID *)pu8VirAddr;
    pstDrvCtx->stStatisticsBuf.u32Size  = u32Size;

    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);

    for (i = 0; i < MAX_ISP_STAT_BUF_NUM; i++)
    {
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.u32PhyAddr =
            u32PhyAddr + i * sizeof(ISP_STAT_S);
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.pVirtAddr =
            (HI_VOID *)(pu8VirAddr + i * sizeof(ISP_STAT_S));

        list_add_tail(&pstDrvCtx->stStatisticsBuf.astNode[i].list,
            &pstDrvCtx->stStatisticsBuf.stFreeList);
    }

    pstDrvCtx->stStatisticsBuf.u32BusyNum = 0;
    pstDrvCtx->stStatisticsBuf.u32UserNum = 0;
    pstDrvCtx->stStatisticsBuf.u32FreeNum = MAX_ISP_STAT_BUF_NUM;
    pstDrvCtx->stStatisticsBuf.bInit = HI_TRUE;

    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

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

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->stStatisticsBuf.pVirAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.u32PhyAddr = 0;

    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

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
    struct list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_STATBUFF_INIT(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    if (list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        ISP_TRACE(HI_DBG_WARN, "busy list empty\n");
        *ppstStatInfo = HI_NULL;
        spin_unlock_irqrestore(&g_stIspLock, u32Flags);
        return HI_FAILURE;
    }

    /* get busy */
    plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
    HI_ASSERT(plist != HI_NULL);
    list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32BusyNum--;

    /* return info */
    pstNode = list_entry(plist, ISP_STAT_NODE_S, list);
    *ppstStatInfo = &pstNode->stStatInfo;

    /* put user */
    list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stUserList);
    pstDrvCtx->stStatisticsBuf.u32UserNum++;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserPut(ISP_DEV IspDev, ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct list_head* plist = HI_NULL;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    HI_BOOL bValid = HI_FALSE;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_ISP_STATBUFF_INIT(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    list_for_each(plist, &pstDrvCtx->stStatisticsBuf.stUserList)
    {
        pstNode = list_entry(plist, ISP_STAT_NODE_S, list);
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
        spin_unlock_irqrestore(&g_stIspLock, u32Flags);
        return HI_FAILURE;
    }

    /* get user */
    HI_ASSERT(plist != HI_NULL);
    list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32UserNum--;

    /* put free */
    list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
    pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufBusyPut(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    /* There should be one frame of the newest statistics info in busy list. */
    while (!list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
        HI_ASSERT(plist != HI_NULL);
        list_del(plist);
        pstDrvCtx->stStatisticsBuf.u32BusyNum--;

        list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
        pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    }

    if (list_empty(&pstDrvCtx->stStatisticsBuf.stFreeList))
    {
        ISP_TRACE(HI_DBG_WARN, "free list empty\n");
        return HI_FAILURE;
    }

    /* get free */
    plist = pstDrvCtx->stStatisticsBuf.stFreeList.next;
    HI_ASSERT(plist != HI_NULL);
    list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32FreeNum--;

    /* read statistics */
    pstNode = list_entry(plist, ISP_STAT_NODE_S, list);

    //pstNode->stStatInfo.unKey.u32Key = 0xffff;
    ISP_DRV_StatisticsRead(&pstNode->stStatInfo);

    /* put busy */
    list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stBusyList);
    pstDrvCtx->stStatisticsBuf.u32BusyNum++;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfig(ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_U32 m;

    if (pstRegCfgInfo->stKernelCfg.unKey.bit1GammaCfg)
    {
        hi_isp_gamma_mcu_priority_write(1);
        for(m = 0; m < 257; m++)
        {
            hi_gamma_rgb_mem_array_data_write(m, (HI_U32)pstRegCfgInfo->stKernelCfg.stGammaRegCfg.au16GammaLut[m]);
        }
        hi_isp_gamma_mcu_priority_write(0);
        pstRegCfgInfo->stKernelCfg.unKey.bit1GammaCfg = 0;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SwitchMode(ISP_DRV_CTX_S *pstDrvCtx)
{
    ISP_SYNC_CFG_S      *pstSyncCfg;
    HI_U32  u32PTIntMask = 0;
    HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};

    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    pstSyncCfg->u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;

    /* init cfg when modes change */
    {
        memset(&pstDrvCtx->stSyncCfg.stSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
        memset(&pstDrvCtx->stSyncCfg.apstNode, 0, sizeof(pstDrvCtx->stSyncCfg.apstNode));

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
        }
        else if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            au32ChnSwitch[0] = 1 % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            bChnSwitchEnable = HI_TRUE;
        }
        else
        {
            au32ChnSwitch[0] = HI_VI_TOP_CHN_SWITCH0_DEFAULT;
            au32ChnSwitch[1] = HI_VI_TOP_CHN_SWITCH1_DEFAULT;
            au32ChnSwitch[2] = HI_VI_TOP_CHN_SWITCH2_DEFAULT;
            au32ChnSwitch[3] = HI_VI_TOP_CHN_SWITCH3_DEFAULT;
            bChnSwitchEnable = HI_FALSE;
        }
        hi_vi_top_chn_switch0_write(au32ChnSwitch[0]);
        hi_vi_top_chn_switch1_write(au32ChnSwitch[1]);
        hi_vi_top_chn_switch2_write(au32ChnSwitch[2]);
        hi_vi_top_chn_switch3_write(au32ChnSwitch[3]);
        hi_vi_top_chn_switch_enble_write(bChnSwitchEnable);

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
        do_div(u64PreSnsGain, DIV_0_TO_1(pstPreNode->stAERegCfg.u32IntTime[0]));
        u64PreSnsGain = u64PreSnsGain << 8;
        do_div(u64PreSnsGain, DIV_0_TO_1(pstPreNode->stAERegCfg.u32IspDgain));

        u64CurSnsGain = pstCurNode->stAERegCfg.u64Exposure;
        do_div(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[0]));
        u64CurSnsGain = u64CurSnsGain << 8;
        do_div(u64CurSnsGain, DIV_0_TO_1(pstCurNode->stAERegCfg.u32IspDgain));
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
            do_div(u64Ratio, u64Tmp);
            pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
        }
    }

    /* calculate DRC compensation */
    u64CurExp = pstCurNode->stAERegCfg.u64Exposure;
    u64PreExp = pstPreNode->stAERegCfg.u64Exposure;

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
    do_div(u64CurExp, u64Tmp);
    pstSyncCfg->u32DRCComp[0] = (HI_U32)u64CurExp;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigIsp(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i;
    HI_U8  u8CfgNodeIdx, u8CfgNodeVC;
    HI_U32 u32IspDgain;
    HI_U32 u32DRCStrength;
    HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};
    HI_U32  u32Ratio[3] = {0x40};
    HI_U32  u32DrcComp = 0x100;
    HI_U32  u32DrcCompIndex = 2;

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
            if (0x40 == u32Ratio[0])    /* ExpRatio == 1 */
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

                hi_vi_top_chn_switch0_write(au32ChnSwitch[0]);
                hi_vi_top_chn_switch1_write(au32ChnSwitch[1]);
                hi_vi_top_chn_switch2_write(au32ChnSwitch[2]);
                hi_vi_top_chn_switch3_write(au32ChnSwitch[3]);
                hi_vi_top_chn_switch_enble_write(bChnSwitchEnable);
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

    for (i=0; i<3; i++)
    {
        u32Ratio[i] = (u32Ratio[i] > EXP_RATIO_MAX) ? EXP_RATIO_MAX : u32Ratio[i];
        u32Ratio[i] = (u32Ratio[i] < EXP_RATIO_MIN) ? EXP_RATIO_MIN : u32Ratio[i];
    }

    hi_isp_wdr_exposure_ratio_write(u32Ratio[0]);

    /* config drc compensation:
        if the parameter is changed prior to frame X,
        then it will compensate the statistics collected in frame X,
        and the results will be applied and seen in frame X+1 */
    u32DrcCompIndex = pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX;
    u32DrcCompIndex = (u32DrcCompIndex > 2) ? (u32DrcCompIndex - 2) : 0;
    u32DrcComp = pstDrvCtx->stSyncCfg.u32DRCComp[u32DrcCompIndex];

    if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        /* do nothing */
    }
    else if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        u32DrcComp = Sqrt32(u32DrcComp << DRC_COMP_SHIFT);
    }
    else
    {
        /* unknow u8WDRMode */
    }

    u32DrcComp = MIN2(u32DrcComp, 0xFFF);
    hi_isp_drc_collection_correction_write(u32DrcComp);

    /* config isp_dgain & drc strength */
    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
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
    if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        /* do nothing */
    }
    else if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        u32IspDgain = Sqrt32(u32IspDgain << ISP_DIGITAL_GAIN_SHIFT);
    }
    else
    {
        /* unknow u8WDRMode */
    }

    /* avoid overflow */
    u32IspDgain = CLIP3(u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    hi_isp_digital_gain_gain_write(u32IspDgain);

    u32DRCStrength = CLIP3(pstDRCRegCfg->u32Strength, DRC_STRENGTH_MIN, DRC_STRENGTH_MAX);
    hi_isp_drc_strength_write(u32DRCStrength);

    /* config Piris */
    if(pstAERegCfg->bPirisValid == HI_TRUE)
    {
        if (HI_NULL != pstDrvCtx->stPirisCb.pfn_piris_gpio_update)
        {
            pstDrvCtx->stPirisCb.pfn_piris_gpio_update(&pstAERegCfg->s32PirisPos);
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
                    if (pstI2cData->bUpdate == HI_TRUE)
                    {
                        if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteI2CData)
                        {
                            pstDrvCtx->stBusCb.pfnISPWriteI2CData(pstI2cData->u8DevAddr,
                                pstI2cData->u32RegAddr, pstI2cData->u32AddrByteNum,
                                pstI2cData->u32Data, pstI2cData->u32DataByteNum);
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
                    if (pstSspData->bUpdate == HI_TRUE)
                    {
                        if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteSSPData)
                        {
                            pstDrvCtx->stBusCb.pfnISPWriteSSPData(pstSspData->u32DevAddr,
                                pstSspData->u32DevAddrByteNum, pstSspData->u32RegAddr,
                                pstSspData->u32RegAddrByteNum, pstSspData->u32Data,
                                pstSspData->u32DataByteNum);
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

HI_S32 ISP_RegisterBusCallBack (ISP_DEV IspDev,
    ISP_BUS_TYPE_E enType, ISP_BUS_CALLBACK_S *pstBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstBusCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
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
        spin_unlock_irqrestore(&g_stIspLock, u32Flags);
        printk("The bus type %d registerd to isp is err!", enType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);
    return HI_SUCCESS;
}

HI_S32 ISP_RegisterPirisCallBack (ISP_DEV IspDev, ISP_PIRIS_CALLBACK_S *pstPirisCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstPirisCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->stPirisCb.pfn_piris_gpio_update = pstPirisCb->pfn_piris_gpio_update;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

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

    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->stPorcMem.u32ProcPhyAddr = u32PhyAddr;
    pstDrvCtx->stPorcMem.u32ProcSize = ISP_PROC_SIZE;
    pstDrvCtx->stPorcMem.pProcVirtAddr = pu8VirAddr;
    up(&pstDrvCtx->stProcSem);

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
    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->stPorcMem.u32ProcPhyAddr = 0;
    pstDrvCtx->stPorcMem.u32ProcSize = 0;
    pstDrvCtx->stPorcMem.pProcVirtAddr = HI_NULL;
    up(&pstDrvCtx->stProcSem);


    if (0 != u32PhyAddr)
    {
        hil_mmb_invalid_cache_byaddr(pu8VirAddr, u32PhyAddr, ISP_PROC_SIZE);
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcPrintf(ISP_DEV IspDev, struct seq_file *s)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }

    //ISP_CHECK_DEV(IspDev);    /* don't defensive check */
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    if (HI_NULL != pstDrvCtx->stPorcMem.pProcVirtAddr)
    {
        seq_printf(s, "%s", (HI_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr);
    }
    up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

#ifdef ENABLE_JPEGEDCF
HI_S32 ISP_SetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    memcpy(pstDrvCtx->pDCFInfoVirAddr,pstIspDCF,sizeof(ISP_DCF_INFO_S));
    return HI_SUCCESS;
}

HI_S32 ISP_GetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    if (pstIspDCF && pstDrvCtx->pDCFInfoVirAddr)
    {
        memcpy(pstIspDCF,pstDrvCtx->pDCFInfoVirAddr,sizeof(ISP_DCF_INFO_S));
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DCFInfoBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(ISP_DCF_INFO_S);
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPDCFInfo", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc DCFInfo buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    memset(pu8VirAddr, 0, u32Size);

    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }

    pstDrvCtx->u32DCFInfoPhyAddr = u32PhyAddr;
    pstDrvCtx->pDCFInfoVirAddr = (ISP_DCF_INFO_S *)pu8VirAddr;

    up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DCFInfoBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->u32DCFInfoPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->pDCFInfoVirAddr;
    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->pDCFInfoVirAddr = HI_NULL;
    pstDrvCtx->u32DCFInfoPhyAddr = 0;
    up(&pstDrvCtx->stProcSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}


#endif

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

HI_S32 ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    if ( ((0 == proc_param) && (pstIspModParam->proc_param != 0)) ||
		 ((0 != proc_param) && (pstIspModParam->proc_param == 0)) )
    {
        ISP_TRACE(HI_DBG_ERR, "proc_param do not support to change! %d to %d.\n",
			proc_param, pstIspModParam->proc_param);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

	proc_param = pstIspModParam->proc_param;

    return HI_SUCCESS;
}

HI_S32 ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
	pstIspModParam->proc_param = proc_param;

    return HI_SUCCESS;
}

static long ISP_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    ISP_SYNC_CFG_BUF_S  *pstSyncCfgBuf = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S  *pstCurNode = HI_NULL;
    HI_S32  s32Ret;
    ISP_DEV IspDev = ISP_GET_DEV(file);
    unsigned long u32Flags;

    s32Ret = ISP_DRV_FuncPreFilter(cmd);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    switch (cmd)
    {
        case ISP_DEV_SET_FD :
        {
            if (copy_from_user(&file->private_data, argp, sizeof(HI_S32)))
            {
                return -EFAULT;
            }
            return 0;
        }
        case ISP_GET_VERSION :
        {
            if (copy_from_user(&gs_stIspLibInfo, argp, sizeof(ISP_VERSION_S)))
            {
                printk("copy isp lib info from user failed!\n");
                return -EFAULT;
            }
            gs_stIspLibInfo.u32Magic = VERSION_MAGIC + ISP_MAGIC_OFFSET;
            if (copy_to_user(argp, &gs_stIspLibInfo, sizeof(ISP_VERSION_S)))
            {
                printk(KERN_INFO "copy lib infor to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_GET_FRAME_EDGE :
        {
            HI_U32 u32Status = 0x0;
            s32Ret = ISP_GetFrameEdge(IspDev, &u32Status);
            if (s32Ret)
            {
                //printk(KERN_INFO "Get Interrupt failed!\n");
                ISP_TRACE(HI_DBG_WARN, "Get Interrupt failed!\n");
                return -EFAULT;
            }
            u32Status &= HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK));
            if (copy_to_user(argp, &u32Status, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_GET_VD_TIMEOUT:
        {
            ISP_VD_TIMEOUT_S stVdTimeOut;

            if (copy_from_user(&stVdTimeOut, argp, sizeof(ISP_VD_TIMEOUT_S)))
            {
                printk(KERN_INFO "copy from user failed!\n");
                return -EFAULT;
            }

            ISP_GetVdTimeOut(IspDev, &stVdTimeOut.stVdInfo,
                stVdTimeOut.u32MilliSec, &stVdTimeOut.s32IntStatus);

            stVdTimeOut.s32IntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK));

            if (copy_to_user(argp, &stVdTimeOut, sizeof(ISP_VD_TIMEOUT_S)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
#ifdef ENABLE_JPEGEDCF
        case ISP_DCF_INFO_SET:
        {
            ISP_DCF_INFO_S stIspDCF;

            if (copy_from_user(&stIspDCF, argp, sizeof(ISP_DCF_INFO_S)))
            {
                printk(KERN_INFO "copy from user failed!\n");
                return -EFAULT;
            }

            ISP_SetDCFInfo(IspDev, &stIspDCF);

            return HI_SUCCESS;
        }
        case ISP_DCF_INFO_GET:
        {
            ISP_DCF_INFO_S stIspDCF;

            ISP_GetDCFInfo(IspDev, &stIspDCF);

            if (copy_to_user(argp, &stIspDCF, sizeof(ISP_DCF_INFO_S)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }
            return HI_SUCCESS;
        }
        case ISP_DCFINFO_BUF_INIT :
        {
            s32Ret = ISP_DRV_DCFInfoBufInit(IspDev);
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].u32DCFInfoPhyAddr, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy base phyaddr to user failed!\n");
                return -EFAULT;
            }
            return s32Ret;
        }
        case ISP_DCFINFO_BUF_EXIT :
        {
            return ISP_DRV_DCFInfoBufExit(IspDev);
        }
#endif
        case ISP_SET_INT_ENABLE :
        {
            HI_BOOL bEn;
            if (copy_from_user(&bEn, argp, sizeof(bEn)))
            {
                printk(KERN_INFO  "copy arg from user failed!\n");
                return -EFAULT;
            }
            return ISP_SetIntEnable(IspDev, bEn);
        }
        /* There should be two ioctl:init & get base phyaddr. */
        case ISP_STAT_BUF_INIT :
        {
            s32Ret = ISP_DRV_StatBufInit(IspDev);
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].stStatisticsBuf.u32PhyAddr, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy base phyaddr to user failed!\n");
                return -EFAULT;
            }
            return s32Ret;
        }
        case ISP_STAT_BUF_EXIT :
        {
            return ISP_DRV_StatBufExit(IspDev);
        }
        case ISP_STAT_BUF_GET :
        {
            ISP_STAT_INFO_S *pstStatInfo = HI_NULL;
            s32Ret = ISP_DRV_StatBufUserGet(IspDev, &pstStatInfo);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
            if (HI_NULL == pstStatInfo)
            {
                return s32Ret;
            }

            if (copy_to_user(argp, pstStatInfo, sizeof(ISP_STAT_INFO_S)))
            {
                printk(KERN_INFO "copy statistics to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_STAT_BUF_PUT :
        {
            ISP_STAT_INFO_S stStatInfo;
            if (copy_from_user(&stStatInfo, argp, sizeof(ISP_STAT_INFO_S)))
            {
                printk(KERN_INFO "copy statistics from user failed!\n");
                return -EFAULT;
            }

            return ISP_DRV_StatBufUserPut(IspDev, &stStatInfo);
        }
        case ISP_REG_CFG_SET:
        {
            HI_U32 u32Flag;

            ISP_CHECK_DEV(IspDev);

            u32Flag= 1 - g_astIspDrvCtx[IspDev].u32RegCfgInfoFlag;

            if (copy_from_user(&g_astIspDrvCtx[IspDev].stRegCfgInfo[u32Flag].stKernelCfg, argp, sizeof(ISP_REG_KERNEL_CFG_S)))
            {
                printk(KERN_INFO "copy register config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            g_astIspDrvCtx[IspDev].u32RegCfgInfoFlag = u32Flag;
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);
            return 0;
        }
        case ISP_SYNC_CFG_SET:
        {
            ISP_CHECK_DEV(IspDev);
            spin_lock_irqsave(&g_stIspLock, u32Flags);
            pstSyncCfgBuf = &g_astIspDrvCtx[IspDev].stSyncCfg.stSyncCfgBuf;
            if (ISPSyncBufIsFull(pstSyncCfgBuf))
            {
                ISP_TRACE(HI_DBG_ERR, "isp sync buffer is full\n");
                //printk("isp sync buffer is full!\n");
                spin_unlock_irqrestore(&g_stIspLock, u32Flags);
                return -EFAULT;
            }

            pstCurNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag];
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);

            if (copy_from_user(pstCurNode, argp, sizeof(ISP_SYNC_CFG_BUF_NODE_S)))
            {
                printk("copy isp sync config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);

            return 0;
        }
        case ISP_WDR_CFG_SET:
        {
            ISP_WDR_CFG_S       stTempWDRCfg;

            ISP_CHECK_DEV(IspDev);

            if (copy_from_user(&stTempWDRCfg, argp, sizeof(ISP_WDR_CFG_S)))
            {
                printk(KERN_INFO "copy WDR config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            memcpy(&g_astIspDrvCtx[IspDev].stWDRCfg, &stTempWDRCfg, sizeof(ISP_WDR_CFG_S));
            ISP_DRV_SwitchMode(&g_astIspDrvCtx[IspDev]);
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);

            return 0;
        }

        case ISP_RES_SWITCH_SET:
        {
            ISP_RES_SWITCH_MODE_S   stSnsImageMode;

            ISP_CHECK_DEV(IspDev);

            if (copy_from_user(&stSnsImageMode, argp, sizeof(ISP_RES_SWITCH_MODE_S)))
            {
                printk(KERN_INFO "copy SnsImageMode config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            ISP_DRV_SwitchMode(&g_astIspDrvCtx[IspDev]);
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);

            return 0;
        }

        case ISP_ACM_ATTR_GET:
        case ISP_ACM_ATTR_SET:
        case ISP_ACM_COEF_GET:
        case ISP_ACM_COEF_SET:
        {
            return ISP_ACM_DRV_Ioctrl(file,cmd,arg);
        }

        case ISP_PROC_INIT:
        {
            s32Ret = ISP_DRV_ProcInit(IspDev);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].stPorcMem, sizeof(ISP_PROC_MEM_S)))
            {
                printk(KERN_INFO "copy proc mem to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_PROC_WRITE_ING:
        {
            ISP_CHECK_DEV(IspDev);
            if (down_interruptible(&g_astIspDrvCtx[IspDev].stProcSem))
            {
                return -ERESTARTSYS;
            }
            return 0;
        }
        case ISP_PROC_WRITE_OK:
        {
            ISP_CHECK_DEV(IspDev);
            up(&g_astIspDrvCtx[IspDev].stProcSem);
            return 0;
        }
        case ISP_PROC_EXIT:
        {
            return ISP_DRV_ProcExit(IspDev);
        }
        case ISP_PROC_PARAM_GET:
        {
            HI_U32 __user *argp = (HI_U32 __user *)arg;

            if (copy_to_user(argp, &proc_param, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy proc_param to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_MEM_INFO_SET:
        {
            ISP_CHECK_DEV(IspDev);
            if (copy_from_user(&g_astIspDrvCtx[IspDev].bMemInit, argp, sizeof(HI_BOOL)))
            {
                printk(KERN_INFO "copy arg from user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_MEM_INFO_GET:
        {
            ISP_CHECK_DEV(IspDev);
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].bMemInit, sizeof(HI_BOOL)))
            {
                printk(KERN_INFO "copy arg to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_PWM_NUM_GET:
        {
            HI_U32 __user *argp = (HI_U32 __user *)arg;

            if (copy_to_user(argp, &pwm_num, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy pwm number to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_SET_MOD_PARAM:
        {
            ISP_MOD_PARAM_S stIspModParam;

            if (copy_from_user(&stIspModParam, argp, sizeof(ISP_MOD_PARAM_S)))
            {
                printk(KERN_INFO "copy from user failed!\n");
                return -EFAULT;
            }

            return ISP_SetModParam(&stIspModParam);
        }
        case ISP_GET_MOD_PARAM:
        {
            ISP_MOD_PARAM_S stIspModParam;

            ISP_GetModParam(&stIspModParam);

            if (copy_to_user(argp, &stIspModParam, sizeof(ISP_MOD_PARAM_S)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }
            return HI_SUCCESS;
        }


        default:
        {
            return VREG_DRV_ioctl(file, cmd, arg);
        }
    }

    return 0;
}

static int ISP_open(struct inode * inode, struct file * file)
{
    return 0;
}

static int ISP_close(struct inode * inode, struct file * file)
{
    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static HI_S32 ISP_Freeze(UMAP_DEVICE_S *pdev)
{
    printk(KERN_DEBUG "%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(UMAP_DEVICE_S *pdev)
{
    HI_U32 u32VicapIntMask;

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

    /* enable port0 int & isp int */
    u32VicapIntMask = HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK));
    HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK)) = (u32VicapIntMask | 0x10100);
    HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = ((VI_PT0_INT_FSTART) | (VI_PT0_INT_ERR));
    HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = (0x0);

    printk(KERN_DEBUG "%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}
#else
static HI_S32 ISP_Freeze(UMAP_DEVICE_S *pdev)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(UMAP_DEVICE_S *pdev)
{
    return HI_SUCCESS;
}
#endif

static struct file_operations stIspFileOp =
{
    .owner      = THIS_MODULE,
    .unlocked_ioctl      = ISP_ioctl,
    .open       = ISP_open,
    .release    = ISP_close
};

struct himedia_ops stHiISPDrvOps = {
    .pm_freeze = ISP_Freeze,
    .pm_restore  = ISP_Restore
};

static UMAP_DEVICE_S s_stHiISPDevice = {
    .devfs_name = "isp_dev",
    .minor      = UMAP_ISP_MINOR_BASE,
    .fops       = &stIspFileOp,
    .drvops     = &stHiISPDrvOps,
    .owner      = THIS_MODULE,
};

static inline irqreturn_t ISP_ISR(int irq, void *id)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    HI_U32 u32PortIntStatus = 0;
    HI_U32 u32PortIntFStart = 0, u32PortIntErr = 0;
    HI_U32 u32IspIntStatus = 0;
    HI_U32 u32DesIntStatus = 0;
    HI_U32 u32DesIntFStart = 0;

    ISP_CHECK_DEV(IspDev);

    u32PortIntStatus = HW_REG(IO_ADDRESS_PORT(VI_PT0_INT));
    u32PortIntStatus &= HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK));

    u32PortIntFStart = u32PortIntStatus & VI_PT0_INT_FSTART;
    u32PortIntErr = u32PortIntStatus & VI_PT0_INT_ERR;

    u32DesIntStatus = HW_REG(IO_ADDRESS_PORT(VI_DES0_INT));
    u32DesIntStatus &= HW_REG(IO_ADDRESS_PORT(VI_DES0_INT_MASK));
    u32DesIntFStart = u32DesIntStatus & VI_DES0_INT_FSTART;

    u32IspIntStatus = HW_REG(IO_ADDRESS_PORT(ISP_INT));
    u32IspIntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK));

    if ((!u32PortIntStatus) && (!u32IspIntStatus) && (!u32DesIntStatus))
    {
        return IRQ_NONE;
    }

    if (u32PortIntStatus)
    {
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT)) = u32PortIntStatus;
    }
    if (u32IspIntStatus)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT)) = u32IspIntStatus;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    /*When detect vi port's width&height changed,then reset isp*/
    if(u32PortIntErr)
    {
        //printk("\nVI WIDTH&HEIGTH= 0x%x\n",HW_REG(IO_ADDRESS_PORT(0x01ec)));
        pstDrvCtx->stDrvDbgInfo.u32IspResetCnt++;
        HW_REG(IO_ISP_ADDRESS(ISP_RESET)) = 0x1;
        HW_REG(IO_ISP_ADDRESS(ISP_RESET)) = 0;

        return IRQ_HANDLED;
    }

    pstDrvCtx->stIntSch.u32IspIntStatus = u32IspIntStatus;
    pstDrvCtx->stIntSch.u32PortIntStatus= u32PortIntStatus;

    if ( !int_bottomhalf )
    {
        ISP_IntBottomHalf((unsigned long)pstDrvCtx);
    }
    else
    {
        tasklet_schedule(&pstDrvCtx->stIntSch.tsklet);
    }

    return IRQ_HANDLED;
}

void ISP_IntBottomHalf(unsigned long data)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
    HI_U64 u64PtTime1 = 0, u64PtTime2 = 0;
    HI_U64 u64IspTime1 = 0, u64IspTime2 = 0;
    HI_U64 u64SensorCfgTime1 = 0, u64SensorCfgTime2 = 0;
    HI_U32 u32PortIntFStart;
    HI_U32 u32IspIntStatus;
    ISP_DEV IspDev = 0;

    pstDrvCtx = (ISP_DRV_CTX_S *)data;
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
    u32IspIntStatus = pstDrvCtx->stIntSch.u32IspIntStatus;

    /* * * * * * * * vi_pt_int_process * * * * * * * */
    if (u32PortIntFStart)
    {
        /* port int proc */
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

        /* In full rate frame WDR mode, config sensor and vi(isp) register with vi_port_int(frame start interrupt) */
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
            /* isp sync task process */
            IspSyncTaskProcess(IspDev);
        }

        /* In half rate frame WDR mode, config sensor with vi_port_int(frame start interrupt) */
        if(IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            //ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
            /* isp sync task process */
            IspSyncTaskProcess(IspDev);
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
    }

    /* * * * * * * * isp_int1_process * * * * * * * */
    if (u32IspIntStatus & ISP_1ST_INT)
    {
        /* In line WDR mode, config sensor and vi(isp) register with vi_des_int(long expodure frame start interrupt) */
        if (IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
			/* isp sync task process */
            IspSyncTaskProcess(IspDev);
        }

        /* In half rate frame WDR mode, config ISP register with ISP frame end interrupt */
        if(IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_RegConfigIsp(pstDrvCtx);
        }
    }

    /* * * * * * * * isp_int2_process * * * * * * * */
    if (u32IspIntStatus & ISP_2ND_INT)
    {
        /* In linear mode or built-in WDR mode, config sensor and vi(isp) register with isp_int(frame start interrupt) */
        if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) || IS_BUILT_IN_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
            /* isp sync task process */
            IspSyncTaskProcess(IspDev);
        }

    }

    if (pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime > pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax)
    {
        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax = pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime;
    }

    /* * * * * * * * isp_int_process * * * * * * * */
    if (u32IspIntStatus & ISP_1ST_INT) /* ISP int */
    {
        /* isp int proc */
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

                return ;
            }
        }

        pstDrvCtx->u32Status = (u32IspIntStatus & 0x1);

        /* first config register. */
        ISP_DRV_RegConfig(&pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegCfgInfoFlag]);

        ISP_DRV_StatBufBusyPut(IspDev);

        pstDrvCtx->bEdge = HI_TRUE;
        pstDrvCtx->bVd   = HI_TRUE;

        wake_up_interruptible(&pstDrvCtx->stIspWait);
        wake_up_interruptible(&pstDrvCtx->stIspWaitVd);

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

    return ;
}

static int ISP_DRV_Init(void)
{
    HI_U32 u32VicapIntMask;

    memset(g_astIspDrvCtx, 0, sizeof(ISP_DRV_CTX_S) * 1);

    reg_vicap_base_va = (void __iomem*)ioremap_nocache(VICAP_BASE, (HI_U32)0x10000);
    if (reg_vicap_base_va == HI_NULL)
    {
        printk(KERN_ERR "Remap VI base failed!\n");
        return -ENOMEM;
    }

    reg_isp_base_va = (void __iomem*)ioremap_nocache(ISP_BASE, (HI_U32)0x20000);
    if (reg_isp_base_va == HI_NULL)
    {
        printk(KERN_ERR "Remap isp base failed!\n");
        return -ENOMEM;
    }

    /* enable port0 int & isp int */
    u32VicapIntMask = HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK));
    HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK)) = (u32VicapIntMask | 0x10100);
    HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = ((VI_PT0_INT_FSTART) | (VI_PT0_INT_ERR));

    HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = (0x0);

    if (request_irq(isp_irq, ISP_ISR, IRQF_SHARED, "ISP", (void*)&g_astIspDrvCtx))
    {
        printk(KERN_ERR  "ISP Register Interrupt Failed!\n");
        iounmap(reg_vicap_base_va);
        reg_vicap_base_va = HI_NULL;
        iounmap(reg_isp_base_va);
        reg_isp_base_va = HI_NULL;
        return -EAGAIN;
    }

    init_waitqueue_head(&g_astIspDrvCtx[0].stIspWait);
    init_waitqueue_head(&g_astIspDrvCtx[0].stIspWaitVd);
    g_astIspDrvCtx[0].bEdge = HI_FALSE;
    g_astIspDrvCtx[0].bVd = HI_FALSE;
    g_astIspDrvCtx[0].bMemInit = HI_FALSE;
    sema_init(&g_astIspDrvCtx[0].stIspSem,1);
    sema_init(&g_astIspDrvCtx[0].stProcSem,1);

    if (int_bottomhalf)
    {
        tasklet_init(&g_astIspDrvCtx[0].stIntSch.tsklet, ISP_IntBottomHalf, (unsigned long)&g_astIspDrvCtx[0]);
    }

    ISP_ACM_DRV_Init();
    SyncTaskInit(0);

    return 0;
}

static int ISP_DRV_Exit(void)
{
    free_irq(isp_irq, (void*)&g_astIspDrvCtx);
    //iounmap((void*)reg_vicap_base_va);
    //iounmap((void*)reg_isp_base_va);
    ISP_ACM_DRV_Exit();
    iounmap(reg_vicap_base_va);
    reg_vicap_base_va = HI_NULL;
    iounmap(reg_isp_base_va);
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

static int ISP_ProcShow(struct seq_file *s, void *pArg)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U8 u8Bayer;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    seq_printf(s, "\n[ISP] Version: [%s], Build Time[%s, %s]\n",
        ISP_VERSION,
        gs_stIspLibInfo.au8Date, gs_stIspLibInfo.au8Time);
    seq_printf(s, "\n");

	seq_printf(s, "-----MODULE PARAM--------------------------------------------------------------\n");
	seq_printf(s, " %15s" " %15s" "\n",  "proc_param", "bottomhalf");
	seq_printf(s, " %15u" " %15u" "\n",  proc_param, int_bottomhalf);

    seq_printf(s, "\n-----DRV INFO-------------------------------------------------------------------------------------\n");

    seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%12s\n"
            ,"IspDev","IntCnt","IntT","MaxIntT","IntGapT","MaxGapT","IntRat","IspResetCnt");

    seq_printf(s, "%11d" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%12d\n",
            0,
            pstDrvCtx->stDrvDbgInfo.u32IspIntCnt,
            pstDrvCtx->stDrvDbgInfo.u32IspIntTime,
            pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime,
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32IspRate,
            pstDrvCtx->stDrvDbgInfo.u32IspResetCnt);

    seq_printf(s, "\n");

    seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%11s" "%12s\n"
            ,"","PtIntCnt","PtIntT","PtMaxIntT","PtIntGapT","PtMaxGapT","PtIntRat","SensorCfgT", "SensorMaxT");

    seq_printf(s, "%11s" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%11d" "%12d\n",
            "",
            pstDrvCtx->stDrvDbgInfo.u32PtIntCnt,
            pstDrvCtx->stDrvDbgInfo.u32PtIntTime,
            pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime,
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32PtRate,
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime,
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax);

    seq_printf(s, "\n");

    u8Bayer = hi_isp_top_rggb_start_read();
    seq_printf(s, "-----PubAttr INFO---------------------------------------------------------------------------------\n");

    seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s\n"
            ,"WndX","WndY","WndW","WndH","Bayer");

    seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12s\n\n",
            hi_isp_input_port_hc_start0_read(),
            hi_isp_input_port_vc_start_read(),
            hi_isp_input_port_hc_size0_read(),
            hi_isp_input_port_vc_size_read(),
            (0 == u8Bayer) ? "RGGB" :
                (1 == u8Bayer) ? "GRBG" :
                (2 == u8Bayer) ? "GBRG" :
                (3 == u8Bayer) ? "BGGR" : "BUTT");

    ISP_DRV_ProcPrintf(0, s);

    return 0;
}

HI_S32 ISP_Init(void *p)
{
    memset(&g_astIspDrvCtx[0].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    return HI_SUCCESS;
}

HI_VOID ISP_Exit(void)
{
#if 0
    VREG_DRV_ReleaseAll();

    ISP_DRV_StatBufExit(0);
    ISP_DRV_ProcExit(0);
#endif
    return ;
}

static HI_U32 ISP_GetVerMagic(HI_VOID)
{
    return VERSION_MAGIC;
}

static UMAP_MODULE_S s_stModule =
{
    .pstOwner      = THIS_MODULE,
    .enModId       = HI_ID_ISP,
    .aModName      = "isp",

    .pfnInit       = ISP_Init,
    .pfnExit       = ISP_Exit,
    .pfnVerChecker = ISP_GetVerMagic,
    .pData         = HI_NULL,
};

static int __init ISP_ModInit(void)
{
    HI_S32  s32Ret;
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

    s32Ret = CMPI_RegisterDevice(&s_stHiISPDevice);
    if (s32Ret)
    {
        printk("Kernel: Could not register isp devices\n");
        return s32Ret;
    }

    if(HI_NULL == CMPI_CreateProc(PROC_ENTRY_ISP, ISP_ProcShow, 0))
    {
        printk("Kernel: Register isp proc failed!\n");
        goto OUT3;
    }

    if (CMPI_RegisterModule(&s_stModule))
    {
        goto OUT2;
    }

    spin_lock_init(&g_stIspLock);

    if(0 != ISP_DRV_Init())
    {
        printk("isp init failed\n");
        goto OUT1;
    }

    g_stIspExpFunc.pfnISPRegisterBusCallBack = ISP_RegisterBusCallBack;
    g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = ISP_RegisterPirisCallBack;

#ifdef TEST_TIME
    ISP_Test_Init();
#endif
    printk(KERN_INFO  "ISP Mod init!\n");
    return HI_SUCCESS;
OUT1:
    CMPI_UnRegisterModule(HI_ID_ISP);

OUT2:
    CMPI_RemoveProc(PROC_ENTRY_ISP);

OUT3:
    CMPI_UnRegisterDevice(&s_stHiISPDevice);

    printk(KERN_INFO  "ISP Mod init failed!\n");
    return HI_FAILURE;
}

static void __exit ISP_ModExit(void)
{
#ifndef HI_FPGA
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_TRUE;
    HI_BOOL bClkEn = HI_FALSE;
#endif

    g_stIspExpFunc.pfnISPRegisterBusCallBack = HI_NULL;
    g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = HI_NULL;
    ISP_DRV_Exit();

    CMPI_UnRegisterModule(HI_ID_ISP);
    CMPI_RemoveProc(PROC_ENTRY_ISP);

    CMPI_UnRegisterDevice(&s_stHiISPDevice);

#ifdef TEST_TIME
    ISP_Test_Exit();
#endif
#ifndef HI_FPGA
    /* For power saving. Reset ISP and close ISP clock when module exit */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
    printk(KERN_INFO  "ISP Mod Exit!\n");
}
#if 0
module_init(ISP_ModInit);
module_exit(ISP_ModExit);
#else
static int hi35xx_isp_probe(struct platform_device *pdev)
{
    isp_irq = himedia_platform_get_irq(pdev, 0);
    if (isp_irq <= 0) {
            dev_err(&pdev->dev, "cannot find isp IRQ\n");
    }

    if(HI_SUCCESS != ISP_ModInit())
    {
        return HI_FAILURE;
    }

    return 0;
}

static int hi35xx_isp_remove(struct platform_device *pdev)
{
    ISP_ModExit();
    return 0;
}

static const struct of_device_id hi35xx_isp_match[] = {
        { .compatible = "hisilicon,hi35xx_isp" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_isp_match);

static struct platform_driver hi35xx_isp_driver = {
        .probe          = hi35xx_isp_probe,
        .remove         = hi35xx_isp_remove,
        .driver         = {
                .name   = "hi35xx_isp",
                .of_match_table = hi35xx_isp_match,
        },
};

himedia_module_platform_driver(hi35xx_isp_driver);
#endif
module_param(proc_param, uint, S_IRUGO);
module_param(pwm_num, uint, S_IRUGO);
module_param(int_timeout, uint, S_IRUGO);
module_param(int_bottomhalf, bool, S_IRUGO);


EXPORT_SYMBOL(g_stIspExpFunc);
EXPORT_SYMBOL(g_stIspPirisExpFunc);

#ifdef ENABLE_JPEGEDCF
EXPORT_SYMBOL(ISP_GetDCFInfo);
#endif
MODULE_AUTHOR(MPP_AUTHOR);
MODULE_LICENSE(MPP_LICENSE);
MODULE_VERSION(MPP_VERSION);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

