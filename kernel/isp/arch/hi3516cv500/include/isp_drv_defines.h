/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#ifndef __ISP_DRV_DEFINES_H__
#define __ISP_DRV_DEFINES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#include "isp.h"
#include "mkp_isp.h"
#include "isp_stt_define.h"

/* 0x00F8[16]: int_isp0_en; 0x00F8[0]: int_pt0_en; 0x00F8[8]: int_ch0_en; */
#define VICAP_HD_MASK                   0x00F8
#define VICAP_HD_INTTERRUPT             0x00F0
#define VICAP_INT_MASK_PT(ViDev)        (1 << (ViDev))
#define VICAP_INT_MASK_ISP(ViPipe)      (0x10000 << (ViPipe))

#define VI_WCH_MASK             0x00F8
#define VI_WCH_INT              0xF0
#define VI_WCH_INT_FSTART       (1<<0)

#define VI_PT_BASE(ViDev)      (0x1000 + (ViDev) * 0x100)
#define VI_PT_INT_MASK          0xF8
#define VI_PT_INT               0xF0
#define VI_PT_INT_FSTART        (1<<0)
#define VI_PT_INT_ERR           ((1<<1)|(2<<1))
#define VI_PT_FSTART_DLY        0x60

#define VI_ISP_MODE             (0x58)
#define VI_ISP0_P2_EN_BIT       (8)

#define ISP_INT_FE_MASK         0x00F8
#define ISP_INT_FE              0x00F0
#define ISP_INT_FE_FSTART       (1<<0)
#define ISP_INT_FE_FEND         (1<<4)

#define VIPROC_INT_BE_MASK      0x00F0
#define VIPROC_INT_BE           0x0310
#define VIPROC_INT_BE_FSTART    (1<<16)

#define VI_PT0_ID               0xE0
#define VI_PT0_ID_CFG           0x10A4

#define CH_REG_NEWER            0x1004
#define ISP_FE_REG_NEWER        0x01EC
#define ISP_BE_REG_NEWER        0x01EC

/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR             0x01A4

#define ISP_RESET               0x0078
#define CHN_SWITCH              0x10028

#define ISP_MAGIC_OFFSET        1
#define ISP_PROC_SIZE           0x2000 /* 8KB */
#define PROC_PRT_SLICE_SIZE     256

#define BUILT_IN_WDR_RATIO_VS_S 0x400
#define BUILT_IN_WDR_RATIO_S_M  0x400
#define BUILT_IN_WDR_RATIO_M_L  0x40


extern void                   *reg_vicap_base_va;
extern void                   *reg_ispfe_base_va[ISP_MAX_PIPE_NUM];
extern void                   *reg_vicap_ch_base_va[ISP_MAX_PIPE_NUM];
extern void                   *reg_ispbe_base_va[ISP_MAX_BE_NUM];
extern void                   *reg_viproc_base_va[ISP_MAX_BE_NUM];

#define IO_PT_ADDRESS(x)                    ((HI_UINTPTR_T)reg_vicap_base_va + (x))
#define IO_WCH_ADDRESS(PhyPipe,x)           ((HI_UINTPTR_T)reg_vicap_ch_base_va[PhyPipe] + (x))
#define IO_FE_ADDRESS(ViPipe, x)            ((HI_UINTPTR_T)reg_ispfe_base_va[ViPipe] + (x))
#define IO_BE_ADDRESS(PhyPipe, x)           ((HI_UINTPTR_T)reg_ispbe_base_va[PhyPipe] + (x))

#define IO_RW_PT_ADDRESS(x)                 (*((volatile unsigned int *)IO_PT_ADDRESS(x)))
#define IO_RW_FE_ADDRESS(ViPipe, x)         (*((volatile unsigned long *)IO_FE_ADDRESS(ViPipe, x)))
#define IO_RW_BE_ADDRESS(PhyPipe, x)        (*((volatile unsigned long *)IO_BE_ADDRESS(PhyPipe, x)))
#define IO_RW_CH_ADDRESS(PhyPipe, x)        (*((volatile unsigned long *)IO_WCH_ADDRESS(PhyPipe, x)))

#define ISP_DRV_FEREG_CTX(ViPipe, pstCtx)        pstCtx = (S_ISPFE_REGS_TYPE *)(reg_ispfe_base_va[ViPipe])
#define ISP_DRV_FESTTREG_CTX(ViPipe, pstCtx)     pstCtx = (S_ISP_VICAP_CH_REGS_TYPE *)(reg_vicap_ch_base_va[ViPipe])
#define ISP_DRV_BEREG_CTX(PhyPipe, pstCtx)       pstCtx = (S_ISPBE_REGS_TYPE *)(reg_ispbe_base_va[PhyPipe])
#define ISP_DRV_VIPROCREG_CTX(PhyPipe, pstCtx)   pstCtx = (S_VIPROC_REGS_TYPE *)(reg_viproc_base_va[PhyPipe])

HI_S32  ISP_DRV_BeRemap(void);
HI_VOID ISP_DRV_BeUnmap(void);

HI_S32  ISP_DRV_VicapRemap(void);
HI_VOID ISP_DRV_VicapUnmap(void);

HI_S32  ISP_DRV_FeRemap(void);
HI_VOID ISP_DRV_FeUnmap(void);

HI_S32 ISP_CHECK_ModParam(VI_PIPE ViPipe);

HI_U8 ISP_DRV_GetBlockNum(VI_PIPE ViPipe);
HI_S8 ISP_DRV_GetBlockId(VI_PIPE ViPipe, isp_running_mode enRunningMode);

HI_S32 ISP_DRV_GetBeRegsAttr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], isp_be_regs_attr *pstBlkAttr);
HI_S32  ISP_DRV_FE_StitchStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo);

HI_S32 ISP_DRV_BE_APB_StatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey);
HI_S32 ISP_DRV_BE_STT_StatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey);

HI_S32 ISP_DRV_BE_StitchStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo);
HI_S32 ISP_DRV_BE_AE_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_BE_AE_Global_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_BE_AE_Local_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_BE_MG_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_BE_AWB_OfflineStatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey);
HI_S32 ISP_DRV_BE_AF_OfflineStatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_BE_Dehaze_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_DPC_OfflineCalibInfoRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);
HI_S32 ISP_DRV_Flicker_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum);

HI_S32 ISP_DRV_BeOfflineAEStitchStatisticsRead(isp_stat *pstStat, ISP_DRV_CTX_S *pstDrvCtx, isp_stat_key unStatkey);
HI_S32 ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead(isp_stat *pstStat, ISP_DRV_CTX_S *pstDrvCtx);
HI_S32 ISP_DRV_BeOfflineAEStitchLocalStatisticsRead(isp_stat *pstStat, ISP_DRV_CTX_S *pstDrvCtx);
HI_S32 ISP_DRV_BeOfflineAWBStitchStatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8BlkDev, isp_stat_key unStatkey);


HI_S32 ISP_DRV_RegConfigChnSel(S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8BlkNum, HI_U8 u8BlkDev);
HI_S32 ISP_DRV_RegConfigWdr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, ISP_FSWDR_SYNC_CFG_S *pstWDRRegCfg, HI_U8 u8BlkNum, HI_U8 u8BlkDev, HI_U32 *u32Ratio);
HI_S32 ISP_DRV_RegConfigLdci(S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8BlkNum, HI_U8 u8BlkDev);
HI_S32 ISP_DRV_RegConfigDe(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], isp_sync_cfg_buf_node *pstCfgNode);
HI_S32 ISP_DRV_RegConfigDrc(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, ISP_DRC_REG_CFG_2_S *pstDRCRegCfg, HI_U8 u8BlkNum, HI_U8 u8BlkDev, HI_U32 *u32Ratio);
HI_S32 ISP_DRV_RegConfigDgain(S_ISPBE_REGS_TYPE *apstBeReg[], HI_U32  u32IspDgain, HI_U8 u8BlkNum, HI_U8 u8BlkDev);
HI_S32 ISP_DRV_RegConfig4Dgain(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U32 *au32WDRGain, HI_U8 u8BlkNum, HI_U8 u8BlkDev, HI_U32 *au32WDRWBGain, HI_U32 *au32WBViGain);
HI_S32 ISP_DRV_SetInputSel(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 *pu32InputSel);
HI_S32 ISP_DRV_ConfigCCM(ISP_DRV_CTX_S *pstDrvCtx, HI_U16 *au16CCM);
HI_S32 ISP_DRV_RegConfigAwbEn(VI_PIPE ViPipe, HI_BOOL bAwbEn);

HI_S32 ISP_DRV_FE_APB_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, ISP_DRV_CTX_S *pstDrvCtx, VI_PIPE ViPipeBind, HI_U32 k, isp_stat_key unStatkey);
HI_S32 ISP_DRV_FE_STT_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, ISP_DRV_CTX_S *pstDrvCtx, VI_PIPE ViPipeBind, HI_U32 k, isp_stat_key unStatkey);

HI_S32 ISP_DRV_FeSttAddrInit(VI_PIPE ViPipe);
HI_S32 ISP_DRV_FeStitchSttAddrInit(VI_PIPE ViPipe);

HI_S32 ISP_DRV_SttBufInit(VI_PIPE ViPipe);
HI_S32 ISP_DRV_SttBufExit(VI_PIPE ViPipe);

HI_S32 ISP_DRV_GetP2EnInfo(VI_PIPE ViPipe, HI_BOOL *pbP2En);
HI_S32 ISP_DRV_FE_StitchNonStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo);
HI_S32 ISP_DRV_StaKernelGet(VI_PIPE ViPipe, ISP_DRV_AF_STATISTICS_S *pstFocusStat);

HI_S32 ISP_DRV_StitchSyncCtrlInit(VI_PIPE ViPipe);
HI_S32 ISP_DRV_GetStitchBeSyncPara(VI_PIPE ViPipe, isp_be_sync_para *pstBeSyncPara);
HI_S32 ISP_DRV_ResetFeCfg(VI_PIPE ViPipe);

HI_S32 ISP_DRV_LDCI_OfflineStatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat);
HI_S32 ISP_DRV_LdciOnlineAttrUpdate(VI_PIPE ViPipe, isp_stat *pstStat);
HI_S32 ISP_DRV_SetLdciWriteSttAddr(S_VIPROC_REGS_TYPE *apstViprocReg, HI_U64 u64WriteAddr);
HI_S32 ISP_DRV_SetLdciReadSttAddr(S_VIPROC_REGS_TYPE *apstViprocReg, HI_U64 u64ReadAddr);
HI_S32 ISP_DRV_GetViprocRegsAttr(VI_PIPE ViPipe, S_VIPROC_REGS_TYPE *apstViprocReg[],
                                 isp_be_regs_attr *pstBlkAttr);
HI_S32 ISP_DRV_UpdateLdciTprOfflineStat(VI_PIPE ViPipe, isp_be_wo_reg_cfg *pstBeCfg);
HI_S32 ISP_DRV_UpdateLdciTprSttAddr(VI_PIPE ViPipe, isp_be_wo_reg_cfg *pstBeCfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
