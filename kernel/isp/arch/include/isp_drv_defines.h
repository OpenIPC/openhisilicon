/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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
#define VICAP_HD_MASK 0x00F8
#define VICAP_HD_INTTERRUPT 0x00F0
#define VICAP_INT_MASK_PT(ViDev) (1 << (ViDev))
#define VICAP_INT_MASK_ISP(ViPipe) (0x10000 << (ViPipe))

#define VI_WCH_MASK 0x00F8
#define VI_WCH_INT 0xF0
#define VI_WCH_INT_FSTART (1 << 0)

#define VI_PT_BASE(ViDev) (0x1000 + (ViDev)*0x100)
#define VI_PT_INT_MASK 0xF8
#define VI_PT_INT 0xF0
#define VI_PT_INT_FSTART (1 << 0)
#define VI_PT_INT_ERR ((1 << 1) | (2 << 1))
#define VI_PT_FSTART_DLY 0x60

#define VI_ISP_MODE (0x58)
#define VI_ISP0_P2_EN_BIT (8)

#define ISP_INT_FE_MASK 0x00F8
#define ISP_INT_FE 0x00F0
#define ISP_INT_FE_FSTART (1 << 0)
#define ISP_INT_FE_FEND (1 << 4)

#define VIPROC_INT_BE_MASK 0x00F0
#define VIPROC_INT_BE 0x0310
#define VIPROC_INT_BE_FSTART (1 << 16)

#define VI_PT0_ID 0xE0
#define VI_PT0_ID_CFG 0x10A4

#define CH_REG_NEWER 0x1004
#define ISP_FE_REG_NEWER 0x01EC
#define ISP_BE_REG_NEWER 0x01EC

/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR 0x01A4

#define ISP_RESET 0x0078
#define CHN_SWITCH 0x10028

#define ISP_MAGIC_OFFSET 1
#define ISP_PROC_SIZE 0x2000 /* 8KB */
#define PROC_PRT_SLICE_SIZE 256

#define BUILT_IN_WDR_RATIO_VS_S 0x400
#define BUILT_IN_WDR_RATIO_S_M 0x400
#define BUILT_IN_WDR_RATIO_M_L 0x40

extern void *reg_vicap_base_va;
extern void *reg_ispfe_base_va[ISP_MAX_PIPE_NUM];
extern void *reg_vicap_ch_base_va[ISP_MAX_PIPE_NUM];
extern void *reg_ispbe_base_va[ISP_MAX_BE_NUM];
extern void *reg_viproc_base_va[ISP_MAX_BE_NUM];

#define IO_PT_ADDRESS(x) ((GK_UINTPTR_T)reg_vicap_base_va + (x))
#define IO_WCH_ADDRESS(PhyPipe, x) \
	((GK_UINTPTR_T)reg_vicap_ch_base_va[PhyPipe] + (x))
#define IO_FE_ADDRESS(ViPipe, x) ((GK_UINTPTR_T)reg_ispfe_base_va[ViPipe] + (x))
#define IO_BE_ADDRESS(PhyPipe, x) \
	((GK_UINTPTR_T)reg_ispbe_base_va[PhyPipe] + (x))

#define IO_RW_PT_ADDRESS(x) (*((volatile unsigned int *)IO_PT_ADDRESS(x)))
#define IO_RW_FE_ADDRESS(ViPipe, x) \
	(*((volatile unsigned long *)IO_FE_ADDRESS(ViPipe, x)))
#define IO_RW_BE_ADDRESS(PhyPipe, x) \
	(*((volatile unsigned long *)IO_BE_ADDRESS(PhyPipe, x)))
#define IO_RW_CH_ADDRESS(PhyPipe, x) \
	(*((volatile unsigned long *)IO_WCH_ADDRESS(PhyPipe, x)))

#define ISP_DRV_FEREG_CTX(ViPipe, pstCtx) \
	pstCtx = (S_ISPFE_REGS_TYPE *)(reg_ispfe_base_va[ViPipe])
#define ISP_DRV_FESTTREG_CTX(ViPipe, pstCtx) \
	pstCtx = (S_ISP_VICAP_CH_REGS_TYPE *)(reg_vicap_ch_base_va[ViPipe])
#define ISP_DRV_BEREG_CTX(PhyPipe, pstCtx) \
	pstCtx = (S_ISPBE_REGS_TYPE *)(reg_ispbe_base_va[PhyPipe])
#define ISP_DRV_VIPROCREG_CTX(PhyPipe, pstCtx) \
	pstCtx = (S_VIPROC_REGS_TYPE *)(reg_viproc_base_va[PhyPipe])

GK_S32 ISP_DRV_BeRemap(void);
GK_VOID ISP_DRV_BeUnmap(void);

GK_S32 ISP_DRV_VicapRemap(void);
GK_VOID ISP_DRV_VicapUnmap(void);

GK_S32 ISP_DRV_FeRemap(void);
GK_VOID ISP_DRV_FeUnmap(void);

GK_S32 ISP_CHECK_ModParam(VI_PIPE ViPipe);

GK_U8 ISP_DRV_GetBlockNum(VI_PIPE ViPipe);
GK_S8 ISP_DRV_GetBlockId(VI_PIPE ViPipe, ISP_RUNNING_MODE_E enRunningMode);

GK_S32 ISP_DRV_GetBeRegsAttr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[],
			     ISP_BE_REGS_ATTR_S *pstBlkAttr);
GK_S32 ISP_DRV_FE_StitchStatisticsRead(VI_PIPE ViPipe,
				       ISP_STAT_INFO_S *pstStatInfo);

GK_S32 ISP_DRV_BE_APB_StatisticsRead(ISP_STAT_S *pstStat,
				     S_ISPBE_REGS_TYPE *apstBeReg[],
				     GK_U8 u8BlkNum, GK_U8 u8BlkDev,
				     ISP_STAT_KEY_U unStatkey);
GK_S32 ISP_DRV_BE_STT_StatisticsRead(VI_PIPE ViPipe, ISP_STAT_S *pstStat,
				     S_ISPBE_REGS_TYPE *apstBeReg[],
				     GK_U8 u8BlkNum, GK_U8 u8BlkDev,
				     ISP_STAT_KEY_U unStatkey);

GK_S32 ISP_DRV_BE_StitchStatisticsRead(VI_PIPE ViPipe,
				       ISP_STAT_INFO_S *pstStatInfo);
GK_S32 ISP_DRV_BE_AE_OfflineStatisticsRead(ISP_STAT_S *pstStat,
					   S_ISP_STT_REGS_TYPE *apstBeStt[],
					   GK_U8 u8BlkNum);
GK_S32 ISP_DRV_BE_AE_Global_OfflineStatisticsRead(
	ISP_STAT_S *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], GK_U8 u8BlkNum);
GK_S32 ISP_DRV_BE_AE_Local_OfflineStatisticsRead(
	ISP_STAT_S *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], GK_U8 u8BlkNum);
GK_S32 ISP_DRV_BE_MG_OfflineStatisticsRead(ISP_STAT_S *pstStat,
					   S_ISP_STT_REGS_TYPE *apstBeStt[],
					   GK_U8 u8BlkNum);
GK_S32 ISP_DRV_BE_AWB_OfflineStatisticsRead(VI_PIPE ViPipe, ISP_STAT_S *pstStat,
					    S_ISP_STT_REGS_TYPE *apstBeStt[],
					    GK_U8 u8BlkNum, GK_U8 u8BlkDev,
					    ISP_STAT_KEY_U unStatkey);
GK_S32 ISP_DRV_BE_AF_OfflineStatisticsRead(VI_PIPE ViPipe, ISP_STAT_S *pstStat,
					   S_ISP_STT_REGS_TYPE *apstBeStt[],
					   GK_U8 u8BlkNum);
GK_S32 ISP_DRV_BE_Dehaze_OfflineStatisticsRead(ISP_STAT_S *pstStat,
					       S_ISP_STT_REGS_TYPE *apstBeStt[],
					       GK_U8 u8BlkNum);
GK_S32 ISP_DRV_DPC_OfflineCalibInfoRead(ISP_STAT_S *pstStat,
					S_ISP_STT_REGS_TYPE *apstBeStt[],
					GK_U8 u8BlkNum);
GK_S32 ISP_DRV_Flicker_OfflineStatisticsRead(ISP_STAT_S *pstStat,
					     S_ISP_STT_REGS_TYPE *apstBeStt[],
					     GK_U8 u8BlkNum);

GK_S32 ISP_DRV_BeOfflineAEStitchStatisticsRead(ISP_STAT_S *pstStat,
					       ISP_DRV_CTX_S *pstDrvCtx,
					       ISP_STAT_KEY_U unStatkey);
GK_S32 ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead(ISP_STAT_S *pstStat,
						     ISP_DRV_CTX_S *pstDrvCtx);
GK_S32 ISP_DRV_BeOfflineAEStitchLocalStatisticsRead(ISP_STAT_S *pstStat,
						    ISP_DRV_CTX_S *pstDrvCtx);
GK_S32 ISP_DRV_BeOfflineAWBStitchStatisticsRead(ISP_STAT_S *pstStat,
						S_ISPBE_REGS_TYPE *apstBeReg[],
						ISP_DRV_CTX_S *pstDrvCtx,
						GK_U8 u8BlkDev,
						ISP_STAT_KEY_U unStatkey);

GK_U8 isp_drv_get_fe_sync_index(ISP_DRV_CTX_S *drv_ctx);
GK_S32 ISP_DRV_RegConfigChnSel(S_ISPBE_REGS_TYPE *apstBeReg[],
			       ISP_DRV_CTX_S *pstDrvCtx, GK_U8 u8BlkNum,
			       GK_U8 u8BlkDev);
GK_S32 ISP_DRV_RegConfigWdr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[],
			    ISP_DRV_CTX_S *pstDrvCtx,
			    ISP_FSWDR_SYNC_CFG_S *pstWDRRegCfg, GK_U8 u8BlkNum,
			    GK_U8 u8BlkDev, GK_U32 *u32Ratio);
GK_S32 ISP_DRV_RegConfigLdci(S_ISPBE_REGS_TYPE *apstBeReg[],
			     ISP_DRV_CTX_S *pstDrvCtx, GK_U8 u8BlkNum,
			     GK_U8 u8BlkDev);
GK_S32 ISP_DRV_RegConfigDe(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[],
			   ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode);
GK_S32 ISP_DRV_RegConfigDrc(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[],
			    ISP_DRV_CTX_S *pstDrvCtx,
			    ISP_DRC_REG_CFG_2_S *pstDRCRegCfg, GK_U8 u8BlkNum,
			    GK_U8 u8BlkDev, GK_U32 *u32Ratio);
GK_S32 isp_drv_reg_config_fe_blc(VI_PIPE vi_pipe, ISP_DRV_CTX_S *drv_ctx);
GK_S32 isp_drv_reg_config_be_blc(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *be_reg[],
				 ISP_DRV_CTX_S *drv_ctx,
				 ISP_SYNC_CFG_BUF_NODE_S *cfg_node,
				 ISP_BE_REGS_ATTR_S *be_regs_attr);
GK_S32 ISP_DRV_RegConfigDgain(S_ISPBE_REGS_TYPE *apstBeReg[],
			      GK_U32 u32IspDgain, GK_U8 u8BlkNum,
			      GK_U8 u8BlkDev);
GK_S32 ISP_DRV_RegConfig4Dgain(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[],
			       ISP_DRV_CTX_S *pstDrvCtx, GK_U32 *au32WDRGain,
			       GK_U8 u8BlkNum, GK_U8 u8BlkDev,
			       GK_U32 *au32WDRWBGain, GK_U32 *au32WBViGain);
GK_S32 ISP_DRV_SetInputSel(S_ISPBE_REGS_TYPE *pstIspBeRegs,
			   GK_U32 *pu32InputSel);
GK_S32 ISP_DRV_ConfigCCM(ISP_DRV_CTX_S *pstDrvCtx, GK_U16 *au16CCM);

GK_S32 ISP_DRV_FE_APB_StatisticsRead(ISP_STAT_S *pstStat,
				     S_ISPFE_REGS_TYPE *pstFeReg,
				     VI_PIPE ViPipeBind, GK_U32 k,
				     ISP_STAT_KEY_U unStatkey);
GK_S32 ISP_DRV_FE_STT_StatisticsRead(ISP_STAT_S *pstStat,
				     S_ISPFE_REGS_TYPE *pstFeReg,
				     ISP_DRV_CTX_S *pstDrvCtx,
				     VI_PIPE ViPipeBind, GK_U32 k,
				     ISP_STAT_KEY_U unStatkey);

GK_S32 ISP_DRV_FeSttAddrInit(VI_PIPE ViPipe);
GK_S32 ISP_DRV_FeStitchSttAddrInit(VI_PIPE ViPipe);

GK_S32 ISP_DRV_SttBufInit(VI_PIPE ViPipe);
GK_S32 ISP_DRV_SttBufExit(VI_PIPE ViPipe);

GK_S32 ISP_DRV_GetP2EnInfo(VI_PIPE ViPipe, GK_BOOL *pbP2En);
GK_S32 ISP_DRV_FE_StitchNonStatisticsRead(VI_PIPE ViPipe,
					  ISP_STAT_INFO_S *pstStatInfo);
GK_S32 ISP_DRV_StaKernelGet(VI_PIPE ViPipe,
			    ISP_DRV_AF_STATISTICS_S *pstFocusStat);

GK_S32 ISP_DRV_StitchSyncCtrlInit(VI_PIPE ViPipe);
GK_S32 ISP_DRV_GetStitchBeSyncPara(VI_PIPE ViPipe,
				   ISP_BE_SYNC_PARA_S *pstBeSyncPara);
GK_S32 ISP_DRV_ResetFeCfg(VI_PIPE ViPipe);

GK_S32 ISP_DRV_LDCI_OfflineStatisticsRead(VI_PIPE ViPipe, ISP_STAT_S *pstStat);
GK_S32 ISP_DRV_LdciOnlineAttrUpdate(VI_PIPE ViPipe, ISP_STAT_S *pstStat);
GK_S32 ISP_DRV_SetLdciWriteSttAddr(S_VIPROC_REGS_TYPE *apstViprocReg,
				   GK_U64 u64WriteAddr);
GK_S32 ISP_DRV_SetLdciReadSttAddr(S_VIPROC_REGS_TYPE *apstViprocReg,
				  GK_U64 u64ReadAddr);
GK_S32 ISP_DRV_GetViprocRegsAttr(VI_PIPE ViPipe,
				 S_VIPROC_REGS_TYPE *apstViprocReg[],
				 ISP_BE_REGS_ATTR_S *pstBlkAttr);
GK_S32 ISP_DRV_UpdateLdciTprOfflineStat(VI_PIPE ViPipe,
					ISP_BE_WO_REG_CFG_S *pstBeCfg);
GK_S32 ISP_DRV_UpdateLdciTprSttAddr(VI_PIPE ViPipe,
				    ISP_BE_WO_REG_CFG_S *pstBeCfg);
GK_S32 isp_drv_reg_config_be_regup(VI_PIPE vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
