/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __MKP_ISP_H__
#define __MKP_ISP_H__

#include "type.h"
#include "comm_3a.h"
#include "common.h"
#include "defines.h"
#include "isp_reg_define.h"
#include "isp_stt_define.h"
#include "comm_snap.h"
#include "isp_block.h"

#define IOC_TYPE_ISP 'I'

#define MAX_ISP_STAT_BUF_NUM 2
#define MAX_ISP_BE_BUF_NUM 4
#define ISP_LUT2STT_SYNC_BUF_MAX_NUM 2

#define ISP_VER_PRIX "ISP_V"
#define ISP_VERSION \
	ISP_VER_PRIX MK_VERSION(VER_X, VER_Y, VER_Z, VER_P, VER_B) VER_D

#define MAX_MMZ_NAMELEN 16
#define ISP_INIT_HAND_SIGNAL 0x66778899
#define ISP_EXIT_HAND_SIGNAL 0x99887766

typedef enum IOC_NR_ISP_E {
	IOC_NR_ISP_SET_FD = 0,
	IOC_NR_ISP_GET_FRAME_EDGE,
	IOC_NR_ISP_GET_VD_TIMEOUT,
	IOC_NR_ISP_GET_VD_END_TIMEOUT,
	IOC_NR_ISP_GET_VD_BEEND_TIMEOUT,
	IOC_NR_ISP_SET_INT_ENABLE,
	IOC_NR_ISP_GET_VERSION,

	IOC_NR_ISP_STAT_BUF_INIT,
	IOC_NR_ISP_STAT_BUF_EXIT,
	IOC_NR_ISP_STAT_BUF_GET,
	IOC_NR_ISP_STAT_BUF_PUT,
	IOC_NR_ISP_STAT_ACT_GET,
	IOC_NR_ISP_REG_CFG_INIT,
	IOC_NR_ISP_REG_CFG_SET,
	IOC_NR_ISP_BE_CFG_BUF_INIT,
	IOC_NR_ISP_GET_BE_BUF_FIRST,
	IOC_NR_ISP_BE_FREE_BUF_GET,
	IOC_NR_ISP_BE_LAST_BUF_GET,
	IOC_NR_ISP_BE_CFG_BUF_EXIT,
	IOC_NR_ISP_BE_CFG_BUF_CTL,
	IOC_NR_ISP_BE_CFG_BUF_RUNNING,
	IOC_NR_ISP_BE_All_BUF_INIT,
	IOC_NR_ISP_PROC_INIT,
	IOC_NR_ISP_PROC_WRITE_ING,
	IOC_NR_ISP_PROC_WRITE_OK,
	IOC_NR_ISP_PROC_EXIT,
	IOC_NR_ISP_PROC_PARAM_GET,
	IOC_NR_ISP_TRANS_BUF_INIT,
	IOC_NR_ISP_TRANS_BUF_EXIT,
	IOC_NR_ISP_PRO_BUF_INIT,
	IOC_NR_ISP_PRO_BUF_EXIT,
	IOC_NR_ISP_SYNC_CFG_SET,
	IOC_NR_ISP_SYNC_STITCH_PARAM_INIT,
	IOC_NR_WDR_SYNC_CFG_SET,
	IOC_NR_ISP_MEM_INFO_SET,
	IOC_NR_ISP_MEM_INFO_GET,
	IOC_NR_ISP_P2EN_INFO_GET,
	IOC_NR_ISP_INIT_INFO_SET,
	IOC_NR_ISP_SYNC_INIT_SET,
	IOC_NR_ISP_RUN_STATE_SET,
	IOC_NR_ISP_RESET_CTX,
	IOC_NR_ISP_CONFIG_INFO_SET,
	IOC_NR_ISP_SNAP_INFO_GET,
	IOC_NR_ISP_PRO_EN_GET,
	IOC_NR_ISP_UPDATE_POS_GET,
	IOC_NR_ISP_FRAME_CNT_GET,
	IOC_NR_ISP_SNAP_ATTR_GET,
	IOC_NR_ISP_PWM_NUM_GET,
	IOC_NR_ISP_RES_SWITCH_SET,
	IOC_NR_ISP_CHN_SELECT_CFG,
	IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET,
	IOC_NR_ISP_MODE_SWITCH_SET,

	IOC_NR_ISP_FRAME_INFO_SET,
	IOC_NR_ISP_FRAME_INFO_GET,
	IOC_NR_ISP_DNG_INFO_SET,
	IOC_NR_ISP_DNG_INFO_GET,
	IOC_NR_ISP_SET_MOD_PARAM,
	IOC_NR_ISP_GET_MOD_PARAM,
	IOC_NR_ISP_SET_CTRL_PARAM,
	IOC_NR_ISP_GET_CTRL_PARAM,

	IOC_NR_ISP_MODE_INFO,
	IOC_NR_ISP_WORK_MODE_INIT,
	IOC_NR_ISP_WORK_MODE_GET,
	IOC_NR_ISP_WORK_MODE_EXIT,
	IOC_NR_ISP_PRE_BLK_NUM_UPDATE,
	IOC_NR_ISP_GET_HDR_ATTR,
	IOC_NR_ISP_GET_WDR_ATTR,
	IOC_NR_ISP_GET_STITCH_ATTR,
	IOC_NR_ISP_GET_FUSION_ATTR,
	IOC_NR_ISP_GET_PIPE_SIZE,
	IOC_NR_ISP_PUB_ATTR_INFO,
	IOC_NR_ISP_CLUT_BUF_INIT,
	IOC_NR_ISP_CLUT_BUF_EXIT,
	IOC_NR_ISP_CLUT_BUF_GET,
	IOC_NR_ISP_STT_BUF_INIT,
	IOC_NR_ISP_STT_BUF_EXIT,
	IOC_NR_ISP_STT_ADDR_INIT,
	IOC_NR_ISP_BE_LUT_STT_BUF_GET,
	IOC_NR_ISP_KERNEL_RUNONCE,
	IOC_NR_ISP_YUV_RUNONCE_INFO,
	IOC_NR_ISP_KERNEL_YUV_RUNONCE,
	IOC_NR_ISP_OPT_RUNONCE_INFO,
	IOC_NR_ISP_SET_PROCALCDONE,
	IOC_NR_ISP_SPECAWB_BUF_INIT,
	IOC_NR_ISP_SPECAWB_BUF_EXIT,
	IOC_NR_ISP_SPECAWB_BUF_GET,
	IOC_NR_ISP_SET_RGBIR_FORMAT,
	IOC_NR_ISP_UPDATE_INFO_SET,
	IOC_NR_ISP_BE_SYNC_PARAM_INIT,
	IOC_NR_ISP_LDCI_BUF_INIT,
	IOC_NR_ISP_LDCI_BUF_EXIT,
	IOC_NR_ISP_LDCI_READ_STT_BUF_GET,
	IOC_NR_ISP_BUTT,
} IOC_NR_ISP_E;

typedef struct ISP_VD_TIMEOUT_S {
	GK_U32 u32MilliSec; /* RW, Range: [0x0, 0xFFFFFFFF], the number of the */
	GK_U32 u32IntStatus; /* RO, when there is ISP interrupt, then the s32IntStatus is 1 */
} ISP_VD_TIMEOUT_S;

typedef struct ISP_VERSION_S {
	GK_U8 au8MppVersion[4];
	GK_U8 au8Date[20];
	GK_U8 au8Time[20];
	GK_U32 u32Magic;
} ISP_VERSION_S;

/* the statistics of isp */
typedef struct ISP_DP_STAT_S {
	GK_U16 au16DefectPixelCount[ISP_STRIPING_MAX_NUM];
	GK_U32 au32DefectPixelLut[ISP_STRIPING_MAX_NUM][STATIC_DP_COUNT_NORMAL];
} ISP_DP_STAT_S;

#define ISP_FLICKER_G_MEAN_LENGTH (144)
typedef struct ISP_FLICKER_STAT_S {
	GK_U32 au32CurDiffGr[ISP_STRIPING_MAX_NUM]; // s24.0, current Frame Gr Sum
	GK_U32 au32CurDiffGb[ISP_STRIPING_MAX_NUM]; // s24.0, current Frame Gb Sum
	GK_U32 au32CurAbsGr[ISP_STRIPING_MAX_NUM]; // u23.0
	GK_U32 au32CurAbsGb[ISP_STRIPING_MAX_NUM]; // u23.0
	GK_U32 au32GMean[ISP_STRIPING_MAX_NUM]
			[ISP_FLICKER_G_MEAN_LENGTH]; // u22.0
} ISP_FLICKER_STAT_S;
typedef struct ISP_DEHAZE_STAT_S {
	GK_U32 au32DehazeMaxStatDout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM];
	GK_U32 au32DehazeMinDout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM / 2];
} ISP_DEHAZE_STAT_S;

typedef struct ISP_MG_STAT_S {
	GK_U16 au16ZoneAvg[MG_ZONE_ROW][MG_ZONE_COLUMN][4];
} ISP_MG_STAT_S;

typedef struct ISP_LDCI_STAT_S {
	GK_U32 au32GlobalMap[LDCI_STAT_WND_Y][LDCI_STAT_WND_X];
} ISP_LDCI_STAT_S;

typedef struct ISP_COMM_STAT_S {
	GK_U16 au16WhiteBalanceGain[4];
} ISP_COMM_STAT_S;

typedef struct ISP_BE_STAT_S {
	ISP_BE_AE_STAT_1_S stBEAeStat1;
	ISP_BE_AE_STAT_2_S stBEAeStat2;
	ISP_BE_AE_STAT_3_S stBEAeStat3;
	ISP_AWB_STAT_1_S stAwbStat1;
	ISP_AWB_STAT_2_S stAwbStat2;
	ISP_BE_AF_STAT_S stAfStat;
	ISP_COMM_STAT_S stCommStat;
	ISP_DEHAZE_STAT_S stDehazeStat;
	ISP_MG_STAT_S stMgStat;
} ISP_BE_STAT_S;

typedef struct ISP_STAT_S {
	ISP_FE_AE_STAT_1_S stFEAeStat1;
	ISP_FE_AE_STAT_2_S stFEAeStat2;
	ISP_FE_AE_STAT_3_S stFEAeStat3;
	ISP_BE_AE_STAT_1_S stBEAeStat1;
	ISP_BE_AE_STAT_2_S stBEAeStat2;
	ISP_BE_AE_STAT_3_S stBEAeStat3;
	ISP_AWB_STAT_1_S stAwbStat1;
	ISP_AWB_STAT_2_S stAwbStat2;
	ISP_BE_AF_STAT_S stBEAfStat;
	ISP_COMM_STAT_S stCommStat;
	ISP_DEHAZE_STAT_S stDehazeStat;
	ISP_MG_STAT_S stMgStat;
	ISP_LDCI_STAT_S stLdciStat;
	GK_BOOL bBEUpdate;
} ISP_STAT_S;

#define DP_STAT_KEY_BIT (14)
#define FLICKER_STAT_KEY_BIT (18)

typedef union ISP_STAT_KEY_U {
	GK_U64 u64Key;
	struct {
		GK_U64 bit1FEAeGloStat : 1; /* [0] */
		GK_U64 bit1FEAeLocStat : 1; /* [1] */
		GK_U64 bit1FEAeStiGloStat : 1; /* [2] */
		GK_U64 bit1FEAeStiLocStat : 1; /* [3] */
		GK_U64 bit1BEAeGloStat : 1; /* [4] */
		GK_U64 bit1BEAeLocStat : 1; /* [5] */
		GK_U64 bit1BEAeStiGloStat : 1; /* [6] */
		GK_U64 bit1BEAeStiLocStat : 1; /* [7] */
		GK_U64 bit1AwbStat1 : 1; /* [8] */
		GK_U64 bit1AwbStat2 : 1; /* [9] */
		GK_U64 bit1AwbStat3 : 1; /* [10] */
		GK_U64 bit1AwbStat4 : 1; /* [11] */
		GK_U64 bit1FEAfStat : 1; /* [12] */
		GK_U64 bit1BEAfStat : 1; /* [13] */
		GK_U64 bit1DpStat : 1; /* [14] */
		GK_U64 bit1CommStat : 1; /* [15] */
		GK_U64 bit1Dehaze : 1; /* [16] */
		GK_U64 bit1MgStat : 1; /* [17] */
		GK_U64 bit1Flicker : 1; /* [18] */
		GK_U64 bit13Rsv : 13; /* [19:31] */
		GK_U64 bit32IsrAccess : 32; /* [32:63] */
	};
} ISP_STAT_KEY_U;

typedef struct ISP_STAT_INFO_S {
	ISP_STAT_KEY_U unKey;
	GK_U64 u64PhyAddr;
	GK_VOID *pVirtAddr; /* (ISP_STAT_S *) */
} ISP_STAT_INFO_S;

typedef struct ISP_STAT_SHADOW_MEM_S {
	GK_U64 u64PhyAddr;
	GK_VOID *pVirtAddr;
	GK_U32 u32Size;
	GK_BOOL bUsrAccess;
} ISP_STAT_SHADOW_MEM_S;

/* the register config of isp */
typedef struct ISP_AE_REG_CFG_1_S {
	GK_U8 au8MeteringHistThresh[4];
	GK_U8 au8WeightTable[15][17];
} ISP_AE_REG_CFG_1_S;

typedef struct ISP_AE_REG_CFG_2_S {
	GK_U64 u64Exposure;
	GK_U64 u64ExposureSF;
	GK_U32 u32IntTime
		[4]; /* 0: Shortest shutter(SS), 1: Short shutter(S), 2: long shutter(L), 3: longest shutter(LL) */
	GK_U32 u32IspDgain;
	GK_U32 au32WDRGain[4];

	GK_BOOL bPirisValid;
	GK_S32 s32PirisPos;

	ISP_FSWDR_MODE_E enFSWDRMode;
} ISP_AE_REG_CFG_2_S;

typedef struct ISP_AE_STATIC_CFG_S {
	GK_U8 u8FEEnable;
	GK_U8 u8BEEnable;

	// Overlap Configs
	GK_U16 u16FECropPosX;
	GK_U16 u16FECropPosY;
	GK_U16 u16FECropOutWidth;
	GK_U16 u16FECropOutHeight;

	GK_U16 u16BECropPosX;
	GK_U16 u16BECropPosY;
	GK_U16 u16BECropOutWidth;
	GK_U16 u16BECropOutHeight;
} ISP_AE_STATIC_CFG_S;

typedef struct ISP_AE_DYNA_CFG_S {
	// ISP MPI Configs
	GK_U8 u8FEFourPlaneMode;
	GK_U8 u8FEHistOffsetX;
	GK_U8 u8FEHistOffsetY;
	GK_U8 u8FEHistSkipX;
	GK_U8 u8FEHistSkipY;

	GK_U8 u8BEAESel;
	GK_U8 u8BEFourPlaneMode;
	GK_U8 u8BEHistOffsetX;
	GK_U8 u8BEHistOffsetY;
	GK_U8 u8BEHistSkipX;
	GK_U8 u8BEHistSkipY;

	// WDR Configs
	GK_U8 u8FEGammaLimit;
	GK_U8 u8FEHistGammaMode;
	GK_U8 u8FEAverGammaMode;
	GK_U8 u8FEBitMove;

	GK_U8 u8BEGammaLimit;
	GK_U8 u8BEHistGammaMode;
	GK_U8 u8BEAverGammaMode;
	GK_U8 u8BEBitMove;

	// Weight Table Configs
	GK_U8 u8BEWightTableUpdate;
	GK_U8 u8BEWeightTableWidth;
	GK_U8 u8BEWeightTableHeight;
	GK_U8 au8BEWeightTable[15][17];

	GK_U8 u8FEWightTableUpdate;
	GK_U8 u8FEWeightTableWidth;
	GK_U8 u8FEWeightTableHeight;
	GK_U8 au8FEWeightTable[15][17];
} ISP_AE_DYNA_CFG_S;

typedef struct ISP_AE_MODULE_REG_CFG_S {
	ISP_AE_STATIC_CFG_S stStaticRegCfg;
	ISP_AE_DYNA_CFG_S stDynaRegCfg;
} ISP_AE_REG_CFG_S;

typedef struct ISP_MG_STATIC_CFG_S {
	GK_U8 u8Enable;

	// Overlap Configs
	GK_U16 u16CropPosX;
	GK_U16 u16CropPosY;
	GK_U16 u16CropOutWidth;
	GK_U16 u16CropOutHeight;
} ISP_MG_STATIC_CFG_S;

typedef struct ISP_MG_DYNA_CFG_S {
	// WDR Configs
	GK_U8 u8GammaLimit;
	GK_U8 u8GammaMode;
	GK_U8 u8BitMove;

	// Zone Configs
	GK_U8 u8ZoneWidth;
	GK_U8 u8ZoneHeight;
} ISP_MG_DYNA_CFG_S;

typedef struct ISP_MG_MODULE_REG_CFG_S {
	ISP_MG_STATIC_CFG_S stStaticRegCfg;
	ISP_MG_DYNA_CFG_S stDynaRegCfg;
} ISP_MG_REG_CFG_S;

typedef struct ISP_DG_STATIC_CFG_S {
	GK_BOOL bResh;
} ISP_DG_STATIC_CFG_S;

typedef struct ISP_DG_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_U16 u16GainR;
	GK_U16 u16GainGR;
	GK_U16 u16GainGB;
	GK_U16 u16GainB;
	GK_U32 u32ClipValue;
} ISP_DG_DYNA_CFG_S;

typedef struct ISP_FE_DG_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_U16 au16GainR[4];
	GK_U16 au16GainGR[4];
	GK_U16 au16GainGB[4];
	GK_U16 au16GainB[4];
	GK_U32 u32ClipValue;
} ISP_FE_DG_DYNA_CFG_S;

typedef struct ISP_DG_MODULE_REG_CFG_S {
	GK_BOOL bDgEn;
	ISP_DG_STATIC_CFG_S stStaticRegCfg;
	ISP_DG_DYNA_CFG_S stDynaRegCfg;
} ISP_DG_REG_CFG_S;

typedef struct ISP_FE_DG_MODULE_REG_CFG_S {
	GK_BOOL bDgEn;
	ISP_DG_STATIC_CFG_S stStaticRegCfg;
	ISP_FE_DG_DYNA_CFG_S stDynaRegCfg;
} ISP_FE_DG_REG_CFG_S;

typedef struct ISP_4DG_STATIC_CFG_S {
	GK_BOOL bResh;
	GK_U16 u16GainR0;
	GK_U16 u16GainGR0;
	GK_U16 u16GainGB0;
	GK_U16 u16GainB0;
	GK_U16 u16GainR1;
	GK_U16 u16GainGR1;
	GK_U16 u16GainGB1;
	GK_U16 u16GainB1;
	GK_U16 u16GainR2;
	GK_U16 u16GainGR2;
	GK_U16 u16GainGB2;
	GK_U16 u16GainB2;
	GK_U16 u16GainR3;
	GK_U16 u16GainGR3;
	GK_U16 u16GainGB3;
	GK_U16 u16GainB3;
} ISP_4DG_STATIC_CFG_S;

typedef struct ISP_4DG_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_U32 u32ClipValue0;
	GK_U32 u32ClipValue1;
	GK_U32 u32ClipValue2;
	GK_U32 u32ClipValue3;
} ISP_4DG_DYNA_CFG_S;

typedef struct ISP_4DG_MODULE_REG_CFG_S {
	GK_BOOL bEnable;
	ISP_4DG_STATIC_CFG_S stStaticRegCfg;
	ISP_4DG_DYNA_CFG_S stDynaRegCfg;
} ISP_4DG_REG_CFG_S;

typedef struct ISP_AWB_REG_CFG_1_S {
	// BE DYNAMIC
	GK_U32 au32BEWhiteBalanceGain[ISP_BAYER_CHN_NUM];
	GK_U16 au16BEColorMatrix[9];
	GK_U16 u16BEMeteringWhiteLevelAwb;
	GK_U16 u16BEMeteringBlackLevelAwb;
	GK_U16 u16BEMeteringCrRefMaxAwb;
	GK_U16 u16BEMeteringCbRefMaxAwb;
	GK_U16 u16BEMeteringCrRefMinAwb;
	GK_U16 u16BEMeteringCbRefMinAwb;
	GK_U8 u8BEWbWorkEn;
	GK_U8 u8BECcEn;
	GK_BOOL u16BECcColortoneEn;
	GK_U16 u16BECcRGain;
	GK_U16 u16BECcGGain;
	GK_U16 u16BECcBGain;

	GK_U32 u32BECropPosX;
	GK_U32 u32BECropPosY;
	GK_U32 u32BEWidth;
	GK_U32 u32BEHeight;
	GK_U32 u32BECropOutHeight;
	GK_U32 u32BECropOutWidth;

	// FE DYNAMIC
	GK_U32 au32FEWhiteBalanceGain[4];
	GK_U8 u8FEWbWorkEn;
	GK_U32 au32WDRWBGain[ISP_BAYER_CHN_NUM];
} ISP_AWB_REG_DYN_CFG_S;

typedef struct ISP_AWB_REG_CFG_2_S {
	// BESTATIC
	GK_BOOL bBEAwbStaCfg;
	GK_U8 u8BEAwbBitmove;
	GK_U8 u8BEAwbWorkEn;
	GK_U8 u8BEWbWorkEn;

	GK_U32 u32BEAwbStatRaddr;
	GK_U8 u8BECcColortoneEn;
	GK_U8 u8BECcEn;

	GK_U32 u32BETopK;
	GK_U32 u32BETopB;
	GK_U32 u32BEBotK;
	GK_U32 u32BEBotB;

	GK_U32 u32BECcInDc0;
	GK_U32 u32BECcInDc1;
	GK_U32 u32BECcInDc2;
	GK_U32 u32BECcOutDc0;
	GK_U32 u32BECcOutDc1;
	GK_U32 u32BECcOutDc2;
	GK_U32 u32BEWbClipValue;

	GK_U16 u16BEAwbOffsetComp;

	// FESTATIC
	GK_BOOL bFEAwbStaCfg;
	GK_U8 u8FEWbWorkEn;

	GK_U32 u32FEClipValue;
} ISP_AWB_REG_STA_CFG_S;

typedef struct ISP_AWB_REG_CFG_3_S {
	// BE USER
	GK_BOOL bResh;
	GK_U32 u32UpdateIndex;
	ISP_AWB_SWITCH_E enBEAWBSwitch;
	GK_U16 u16BEZoneRow;
	GK_U16 u16BEZoneCol;
	GK_U16 u16BEZoneBin;
	GK_U16 u16BEMeteringBinHist0;
	GK_U16 u16BEMeteringBinHist1;
	GK_U16 u16BEMeteringBinHist2;
	GK_U16 u16BEMeteringBinHist3;
} ISP_AWB_REG_USR_CFG_S;

typedef struct ISP_AWB_REG_CFG_S {
	ISP_AWB_REG_DYN_CFG_S stAwbRegDynCfg;
	ISP_AWB_REG_STA_CFG_S stAwbRegStaCfg;
	ISP_AWB_REG_USR_CFG_S stAwbRegUsrCfg;
} ISP_AWB_REG_CFG_S;

typedef struct ISP_AWB_SYNC_CFG_S {
	GK_U32 au32BEWhiteBalanceGain[ISP_BAYER_CHN_NUM];
	GK_U32 au32WDRWBGain[ISP_BAYER_CHN_NUM];
	GK_U16 au16ColorMatrix[CCM_MATRIX_SIZE];
	ISP_AWB_SWITCH_E enBEAWBSwitch;
} ISP_AWB_SYNC_CFG_S;

typedef ISP_AWB_SYNC_CFG_S ISP_AWB_REG_CFG_2_S;

typedef struct ISP_AF_REG_CFG_S {
	GK_U16 u16ThresholdWrite;
	GK_U8 u8MetricsShift;
	GK_U8 u8NpOffset;
	GK_BOOL bAfEnable;
	GK_BOOL bIir0Enable0;
	GK_BOOL bIir0Enable1;
	GK_BOOL bIir0Enable2;
	GK_BOOL bIir1Enable0;
	GK_BOOL bIir1Enable1;
	GK_BOOL bIir1Enable2;
	ISP_AF_PEAK_MODE_E enPeakMode;
	ISP_AF_SQU_MODE_E enSquMode;
	GK_BOOL bOffsetEnable;
	GK_BOOL bCropEnable;
	GK_BOOL bLpfEnable;
	GK_BOOL bMeanEnable;
	GK_BOOL bRawMode;
	GK_BOOL bIir0DsEnable;
	GK_BOOL bIir1DsEnable;
	GK_BOOL bFir0LpfEnable;
	GK_BOOL bFir1LpfEnable;
	GK_BOOL bIir0LdgEnable;
	GK_BOOL bIir1LdgEnable;
	GK_BOOL bFir0LdgEnable;
	GK_BOOL bFir1LdgEnable;
	GK_BOOL bUpdate;

	GK_U8 u8BayerMode;
	GK_U8 u8Gamma;
	GK_U8 u8GainLimit;
	GK_U8 u8IirPlgGroup0;
	GK_U8 u8IirPlsGroup0;
	GK_U8 u8IirPlgGroup1;
	GK_U8 u8IirPlsGroup1;
	GK_U8 u8CntShift0H;
	GK_U8 u8CntShift1H;
	GK_U8 u8CntShift0V;
	GK_U8 u8CntShift1V;
	GK_U8 u8CntShiftY;
	GK_U8 u8IirDilate0;
	GK_U8 u8IirDilate1;
	GK_U8 u8HilighThre;
	GK_U8 u8AfPosSel;

	GK_U16 u16MeanThres;
	GK_U16 u16WindowVnum;
	GK_U16 u16WindowHnum;
	GK_U16 u16IirThre0Coring;
	GK_U16 u16IirThre1Coring;
	GK_U16 u16IirPeak0Coring;
	GK_U16 u16IirPeak1Coring;
	GK_U16 u16FirThre0Coring;
	GK_U16 u16FirThre1Coring;
	GK_U16 u16FirPeak0Coring;
	GK_U16 u16FirPeak1Coring;
	GK_U16 u16IirSlope0Coring;
	GK_U16 u16IirSlope1Coring;
	GK_U16 u16FirSlope0Coring;
	GK_U16 u16FirSlope1Coring;
	GK_U16 u16OffsetGr;
	GK_U16 u16OffsetGb;
	GK_U16 u16InputVsize;
	GK_U16 u16InputHsize;
	GK_U16 u16Iir0ShiftGroup0;
	GK_U16 u16Iir1ShiftGroup0;
	GK_U16 u16Iir2ShiftGroup0;
	GK_U16 u16Iir3ShiftGroup0;
	GK_U16 u16Iir0ShiftGroup1;
	GK_U16 u16Iir1ShiftGroup1;
	GK_U16 u16Iir2ShiftGroup1;
	GK_U16 u16Iir3ShiftGroup1;
	GK_U16 u16IirThre0Low;
	GK_U16 u16IirThre0High;
	GK_U16 u16IirThre1Low;
	GK_U16 u16IirThre1High;
	GK_U16 u16IirGain0Low;
	GK_U16 u16IirGain0High;
	GK_U16 u16IirGain1Low;
	GK_U16 u16IirGain1High;
	GK_U16 u16IirSlope0Low;
	GK_U16 u16IirSlope0High;
	GK_U16 u16IirSlope1Low;
	GK_U16 u16IirSlope1High;
	GK_U16 u16FirThre0Low;
	GK_U16 u16FirThre0High;
	GK_U16 u16FirThre1Low;
	GK_U16 u16FirThre1High;
	GK_U16 u16FirGain0Low;
	GK_U16 u16FirGain0High;
	GK_U16 u16FirGain1Low;
	GK_U16 u16FirGain1High;
	GK_U16 u16FirSlope0Low;
	GK_U16 u16FirSlope0High;
	GK_U16 u16FirSlope1Low;
	GK_U16 u16FirSlope1High;
	GK_U16 u16AccShift0H;
	GK_U16 u16AccShift1H;
	GK_U16 u16AccShift0V;
	GK_U16 u16AccShift1V;
	GK_U16 u16AccShiftY;
	GK_U16 u16CropPosY;
	GK_U16 u16CropPosX;
	GK_U16 u16ShiftCountY;
	GK_U16 u16CropVsize;
	GK_U16 u16CropHsize;

	GK_S16 s16IirGain0Group0;
	GK_S16 s16IirGain0Group1;
	GK_S16 s16IirGain1Group0;
	GK_S16 s16IirGain1Group1;
	GK_S16 s16IirGain2Group0;
	GK_S16 s16IirGain2Group1;
	GK_S16 s16IirGain3Group0;
	GK_S16 s16IirGain3Group1;
	GK_S16 s16IirGain4Group0;
	GK_S16 s16IirGain4Group1;
	GK_S16 s16IirGain5Group0;
	GK_S16 s16IirGain5Group1;
	GK_S16 s16IirGain6Group0;
	GK_S16 s16IirGain6Group1;
	GK_S16 s16FirHGain0Group0;
	GK_S16 s16FirHGain0Group1;
	GK_S16 s16FirHGain1Group0;
	GK_S16 s16FirHGain1Group1;
	GK_S16 s16FirHGain2Group0;
	GK_S16 s16FirHGain2Group1;
	GK_S16 s16FirHGain3Group0;
	GK_S16 s16FirHGain3Group1;
	GK_S16 s16FirHGain4Group0;
	GK_S16 s16FirHGain4Group1;

	GK_U32 u32StatReadAddr;
	GK_U32 u32StatReadData;
	GK_U32 u32UpdateIndex;
} ISP_AF_REG_CFG_S;

typedef struct ISP_OFFSET_REG_CFG_S {
	GK_U16 au16BlackLevelOffset[4];
} ISP_OFFSET_REG_CFG_S;

typedef struct ISP_DRC_STATIC_CFG_S {
	GK_BOOL bStaticResh;

	GK_BOOL bRchEn;
	GK_BOOL bWchEn;

	GK_BOOL bRdstatEn; // DRC read statistics enable, u1.0
	GK_BOOL bWrstatEn; // DRC write statistics enable, u1.0

	GK_BOOL bMonochromeMode; // Monochrome mode enable; set to 1 if using monochrome sensor
	GK_BOOL bDetailBoostEnable;
	GK_BOOL bPdwSumEnable;
	GK_BOOL bWgtBoxTriSel;
	GK_BOOL bDpDetectEnable;

	GK_U8 u8OffLineMode; // Offline Mode, u1.0

	GK_U8 u8BinNumZ; // DRC bin num

	GK_U8 u8BitWidthOut; // Output bitwidth:12/20 select
	GK_U8 u8LocalEdgeLmt; // Limit of the difference between BLT filter's input & output, u8.0

	GK_U8 u8RWgt; // u4.0
	GK_U8 u8GWgt; // u5.0
	GK_U8 u8BWgt; // u4.0

	GK_U8 u8ColorControlMode; // 0 - Linear mode, 1 - Powerlaw mode, u1.0
	GK_U8 u8ColorControlLUTCtrl; // 0 - login LUT, 1 -logout LUT, u1.0
	GK_U16 u16GlobalColorCorr; // not in use now and should be fixed to 1024, u12.0

	GK_U8 u8PFRRGSlo; // Purple Fringe Reduce, R/G slope control, u3.0
	GK_U8 u8PFRRGWid; // Purple Fringe Reduce, R/G width, u7.0
	GK_U8 u8PFRRGCtr; // Purple Fringe Reduce, R/G central point, u8.0

	GK_U8 u8PFRBGSlo; // Purple Fringe Reduce, B/G slope control, u3.0
	GK_U8 u8PFRBGThr; // Purple Fringe Reduce, B/G Threshold detection, u8.0

	GK_U8 au8SegIdxBase[ISP_DRC_TM_SEG_NUM];
	GK_U8 au8SegMaxVal[ISP_DRC_TM_SEG_NUM];

	GK_U8 u8UpdateMode;
	GK_U16 u16TimingStat;
	GK_U8 u8DrcUpdate;

	GK_BOOL bFirstFrame;
	GK_U8 u8ShpLog;
	GK_U8 u8ShpExp;
	GK_U32 u32DivDenomLog;
	GK_U32 u32DenomExp;
	GK_U32 au32PrevLuma[ISP_DRC_EXP_COMP_SAMPLE_NUM];

	/* Regs for DRC STAT */
	GK_BOOL bDrcsEn; // DRCS enable (DRCS = DRC Statistics module in FE), u1.0
	GK_BOOL bDrcsVbiFltEn; // DRCS VBI filtering enable, u1.0
	GK_BOOL bDrcsWrtStatEn; // DRCS write statistics enable, u1.0

} ISP_DRC_STATIC_CFG_S;

typedef struct ISP_DRC_DYNA_CFG_S {
	GK_BOOL bDynaResh;
	GK_BOOL bImgSizeChanged;
	GK_BOOL bIsOfflineRepeatMode;
	GK_BOOL bVbifltEn; // DRC VBI filtering enable, u1.0
	GK_BOOL bColorCorrEnable;
	GK_BOOL bLutUpdate;

	GK_U16 u16StrengthBias;
	GK_U16 u16Strength; // DRC strength
	GK_U16 u16LutMixCtrl; // Mixing ctrl for LUT0 & LUT1
	GK_U16 u16AutoStrengthMax; // Maximum strength in auto mode
	GK_U16 u16AutoStrengthMin; // Minimum strength in auto mode

	GK_U16 au16ToneMappingValue0[ISP_DRC_TM_NODE_NUM];
	GK_U16 au16ToneMappingDiff0[ISP_DRC_TM_NODE_NUM];

	GK_U8 u8PFRHighThr; // Purple Fringe Reduce, High Threshold, u8.0

	GK_U16 u16VSize; // Image Height
	GK_U16 u16HSize; // Image Width
	GK_U8 u8OffLineMode; // Offline Mode, u1.0

	GK_U8 u8BlockVNum; // DRC block configure, Number, Vertical direction, u6.0
	GK_U8 u8BlockHNum; // DRC block configure, Number, Horizontal direction, u6.0
	GK_U8 u8BlockChkY; // DRC block configure, chk_y = vsize%num, u6.0
	GK_U8 u8BlockChkX; // DRC block configure, chk_x = hsize%num, u6.0
	GK_U16 u16BlockVSize; // DRC block configure, Size, Vertical direction, u9.0
	GK_U16 u16BlockHSize; // DRC block configure, Size, Horizontal direction, u9.0

	GK_U16 u16DivY0; // DRC block configure, div_y0 = 32768/vsize, u15.0
	GK_U16 u16DivX0; // DRC block configure, div_x0 = 32768/hsize, u15.0
	GK_U16 u16DivY1; // DRC block configure, div_y1 = 32768/vsize, u15.0
	GK_U16 u16DivX1; // DRC block configure, div_x1 = 32768/hsize, u15.0
	GK_U8 u8BinScale; // DRC bin scale (pixel weight), u3.0

	GK_U8 u8BigXInit; // DRC stripe configuration, initial is_big flag
	GK_U8 u8IdxXInit; // DRC stripe configuration, initial block index
	GK_U16 u16CntXInit; // DRC stripe configuration, initial pixel count
	GK_U16 u16AccXInit; // DRC stripe configuration, initial accumulation
	GK_U16 u16WgtXInit; // DRC stripe configuration, initial pixel position for calculating pixel weight
	GK_U16 u16TotalWidth; // DRC stripe configuration, total stripe width
	GK_U16 u16StatWidth; // DRC stripe configuration, active width for statistics

	GK_U8 u8DrcsBigXInit;
	GK_U8 u8DrcsIdxXInit;
	GK_U16 u16DrcsCntXInit;
	GK_U16 u16DrcsAccXInit;
	GK_U16 u16DrcsWgtXInit;
	GK_U16 u16DrcsTotalWidth;
	GK_U16 u16DrcsStatWidth;

	GK_U16 u16DrcsVSize;
	GK_U16 u16DrcsHSize;

} ISP_DRC_DYNA_CFG_S;

typedef struct ISP_DRC_USR_CFG_S {
	GK_BOOL bUsrResh;
	GK_U16 u16Exposure; // Exposure value, u16.0

	GK_U8 u8YSFT1; // Used for dark region luminance gain limitation, u3.0
	GK_U8 u8YVAL1; // Used for dark region luminance gain limitation, u7.0
	GK_U8 u8YSFT2; // Used for dark region luminance gain limitation, u2.0
	GK_U8 u8YVAL2; // Used for dark region luminance gain limitation, u4.0

	GK_U8 u8CSFT1; // Used for dark region chroma gain limitation, u3.0
	GK_U8 u8CVAL1; // Used for dark region chroma gain limitation, u7.0
	GK_U8 u8CSFT2; // Used for dark region chroma gain limitation, u2.0
	GK_U8 u8CVAL2; // Used for dark region chroma gain limitation, u4.0

	GK_U8 u8SFT; // u3.0
	GK_U8 u8VAL; // u7.0

	GK_U8 u8MixingCoring; // u8.0
	GK_U8 u8MixingDarkMin; // u8.0
	GK_U8 u8MixingDarkMax; // u8.0
	GK_S8 s8MixingDarkSlo; // u8.0
	GK_U8 u8MixingDarkThr; // u8.0

	GK_U8 u8MixingBrightMin; // u8.0
	GK_U8 u8MixingBrightMax; // u8.0
	GK_S8 s8MixingBrightSlo; // u8.0
	GK_U8 u8MixingBrightThr; // u8.0

	GK_U8 u8GainClipKnee; // Startup for bright region gain limit, u4.0
	GK_U8 u8GainClipStep; // Region for bright region gain limit, u4.0

	GK_U8 u8PFRHighSlo; // Purple Fringe Reduce, High Slope, u4.0
	GK_U8 u8PFRLowSlo; // Purple Fringe Reduce, Low Slope, u4.0
	GK_U8 u8PFRLowThr; // Purple Fringe Reduce, Low Slope, u8.0

	GK_U8 u8GradShift; // Grad Reverse shift, u3.0
	GK_U8 u8GradSlope; // Grad Reverse Slope, u4.0
	GK_U8 u8GradMax; // Grad Reverse, max value, u7.0
	GK_U8 u8GradThr; // Grad Reverse, Threshold Value, u8.0

	GK_U8 u8VarSpaCoarse;
	GK_U8 u8VarSpaMedium;
	GK_U8 u8VarSpaFine;

	GK_U8 u8VarRngCoarse;
	GK_U8 u8VarRngMedium;
	GK_U8 u8VarRngFine;

	GK_U8 au8BinMixCoarse[8];
	GK_U8 au8BinMixMedium[8];

	GK_U8 u8FltSpaCoarse;
	GK_U8 u8FltSpaMedium;
	GK_U8 u8FltSpaFine;
	GK_U8 u8FltRngCoarse;
	GK_U8 u8FltRngMedium;
	GK_U8 u8FltRngFine;

	GK_U8 u8DisThrCoefHigh;
	GK_U8 u8DisThrCoefLow;
	GK_U8 u8DisOffsetCoef;
	GK_U8 u8FltRngAdaMax;

	GK_U8 u8FltSpaAdaMax;
	GK_U8 u8DiffThrLow;
	GK_U8 u8DiffThrHigh;

	GK_S8 s8DetailSubFactor;

	GK_U16 u16PFRSum; // May not be used in fw;
	GK_U16 u16CCLUT[ISP_DRC_CC_NODE_NUM]; // Color Correction LUT

	/* New add for HDR mode */
	GK_U8 u8DetailCoring;
	GK_U8 u8DetailDarkStep;
	GK_U8 u8DetailBrightStep;

	GK_S8 s8DetailDarkSlo;
	GK_U8 u8DetailDarkThr;
	GK_U8 u8DetailDarkMax;
	GK_U8 u8DetailDarkMin;

	GK_S8 s8DetailBrightSlo;
	GK_U8 u8DetailBrightThr;
	GK_U8 u8DetailBrightMax;
	GK_U8 u8DetailBrightMin;

	GK_U8 u8DetailDarkCurve[16];
	GK_U8 u8DetailBrightCurve[16];

	GK_U8 u8SuppressBrightMax;
	GK_U8 u8SuppressBrightMin;
	GK_U8 u8SuppressBrightThr;
	GK_U8 u8SuppressBrightSlo;
	GK_U8 u8SuppressDarkMax;
	GK_U8 u8SuppressDarkMin;
	GK_U8 u8SuppressDarkThr;
	GK_U8 u8SuppressDarkSlo;

	GK_BOOL bDpDetectG2RB;
	GK_BOOL bDpDetectRB2RB;
	GK_BOOL bDpDetectReplCtr;
	GK_U8 u8DpDetectRngRatio;
	GK_U8 u8DpDetectThrSlo;
	GK_U16 u16DpDetectThrMin;

	/* Cubic TM Curve Related Parameters */
	GK_U16 u16CubicThres01;
	GK_U16 u16CubicThres10;
	GK_U16 u16CubicThres11;
	GK_BOOL bCubicCurveSel;

	GK_U8 u8CubicCoef00AExp;
	GK_U32 u32CubicCoef00A;
	GK_U8 u8CubicCoef01BExp;
	GK_U32 u32CubicCoef01B;
	GK_U8 u8CubicCoef02CExp;
	GK_U32 u32CubicCoef02C;
	GK_U32 u32CubicCoef03D;

	GK_U8 u8CubicCoef10AExp;
	GK_U32 u32CubicCoef10A;
	GK_U8 u8CubicCoef11BExp;
	GK_U32 u32CubicCoef11B;
	GK_U8 u8CubicCoef12CExp;
	GK_U32 u32CubicCoef12C;
	GK_U32 u32CubicCoef13D;

	GK_U8 u8CubicCoef20AExp;
	GK_U32 u32CubicCoef20A;
	GK_U8 u8CubicCoef21BExp;
	GK_U32 u32CubicCoef21B;
	GK_U8 u8CubicCoef22CExp;
	GK_U32 u32CubicCoef22C;
	GK_U32 u32CubicCoef23D;

	GK_U8 u8CubicCoef30AExp;
	GK_U32 u32CubicCoef30A;
	GK_U8 u8CubicCoef31BExp;
	GK_U32 u32CubicCoef31B;
	GK_U8 u8CubicCoef32CExp;
	GK_U32 u32CubicCoef32C;
	GK_U32 u32CubicCoef33D;
	GK_U32 u32UpdateIndex;
} ISP_DRC_USR_CFG_S;

typedef struct ISP_DRC_SYNC_CFG_S {
	GK_U8 u8ShpLog;
	GK_U8 u8ShpExp;
	GK_BOOL bIsOfflineRepeatMode;
	GK_S32 as32PrevLumaDelta[ISP_DRC_EXP_COMP_SAMPLE_NUM - 1];
} ISP_DRC_SYNC_CFG_S;

typedef ISP_DRC_SYNC_CFG_S ISP_DRC_REG_CFG_2_S;

typedef struct ISP_DRC_REG_CFG_S {
	GK_BOOL bDrcEn; // Enable
	ISP_DRC_STATIC_CFG_S stStaticRegCfg;
	ISP_DRC_DYNA_CFG_S stDynaRegCfg;
	ISP_DRC_USR_CFG_S stUsrRegCfg;
	ISP_DRC_SYNC_CFG_S stSyncRegCfg;
} ISP_DRC_REG_CFG_S;

#define ISP_PREGAMMA_SEG_NUM (12)
typedef struct ISP_PREGAMMA_STATIC_CFG_S {
	GK_BOOL bStaticResh;
	GK_U16 au16SegMaxVal[ISP_PREGAMMA_SEG_NUM];
	GK_U8 au8SegIdxBase[ISP_PREGAMMA_SEG_NUM];
} ISP_PREGAMMA_STATIC_CFG_S;

typedef struct ISP_PREGAMMA_DYNA_CFG_S {
	GK_BOOL bSwitchMode;
	GK_BOOL bPreGammaLutUpdateEn;
	GK_U8 u8BufId;
	GK_U32 u32UpdateIndex;
	GK_U16 au16PreGammaLUT[PREGAMMA_NODE_NUM]; // u16.0
} ISP_PREGAMMA_DYNA_CFG_S;

typedef struct ISP_PREGAMMA_REG_CFG_S {
	GK_BOOL bPreGammaEn;
	ISP_PREGAMMA_DYNA_CFG_S stDynaRegCfg;
	ISP_PREGAMMA_STATIC_CFG_S stStaticRegCfg;
} ISP_PREGAMMA_REG_CFG_S;

typedef struct ISP_DEHAZE_STATIC_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8MaxMode;

	GK_U16 u16DehazeBlthld;
	GK_U8 u8DehazeNegMode;

	GK_U16 u16BlockSum;
	GK_U8 u8Dchnum;
	GK_U8 u8Dcvnum;

	GK_U8 u8OfflineMode;
	GK_U8 u8UpdateMode;
	GK_U8 u8DehazeUpdate;
	GK_U16 u16fHblank;
	GK_U16 u16bHblank;
	GK_U16 u16fVblank;
	GK_U16 u16bVblank;

} ISP_DEHAZE_STATIC_CFG_S;

typedef struct ISP_DEHAZE_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8BufId;
	GK_U8 u8Strength;
	GK_U16 u16AirR;
	GK_U16 u16AirG;
	GK_U16 u16AirB;
	GK_U8 u8Lut[256];
	GK_U32 prestat[DEFOG_ZONE_NUM / 2];
	GK_U32 u32Update;
	GK_U32 u32LutUpdate;

	GK_U32 u32Width;
	GK_U32 u32Height;
	GK_U16 u16Blockhsize;
	GK_U16 u16Blockvsize;
	GK_U32 u32phasex;
	GK_U32 u32phasey;

	GK_U32 u32smlMapOffset; /* 20bit */
	GK_U32 u32StatStartX;
	GK_U32 u32StatEndX;
	GK_U8 u8StatnumH;
	GK_U8 u8StatnumV;

	GK_U16 u16DehazeThldR;
	GK_U16 u16DehazeThldG;
	GK_U16 u16DehazeThldB;

} ISP_DEHAZE_DYNA_CFG_S;

typedef struct ISP_DEHAZE_REG_CFG_S {
	GK_BOOL bDehazeEn;
	GK_BOOL bLut2SttEn;
	ISP_DEHAZE_DYNA_CFG_S stDynaRegCfg;
	ISP_DEHAZE_STATIC_CFG_S stStaticRegCfg;
} ISP_DEHAZE_REG_CFG_S;

typedef struct ISP_DPC_STATIC_CFG_S {
	GK_BOOL bStaticResh;
	GK_U8 u8DpccOutputMode; // U4.0
	GK_U32 u32DpccBptCtrl; // U12.0
} ISP_DPC_STATIC_CFG_S;

typedef struct ISP_DPC_USR_STATIC_COR_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8BufId;
	GK_U16 u16DpccBptNumber;
	GK_U32 u32UpdateIndex;
	GK_U32 au32DpccBpTable[STATIC_DP_COUNT_NORMAL];
} ISP_DPC_USR_STATIC_COR_CFG_S;

typedef struct ISP_DPC_USR_DYNA_COR_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bDpccHardThrEn;
	GK_S8 s8DpccSupTwinkleThrMax;
	GK_S8 s8DpccSupTwinkleThrMin;
	GK_U16 u16DpccRakeRatio;
	GK_U8 u8DPCPos; // U1.0
} ISP_DPC_USR_DYNA_COR_CFG_S;

typedef struct ISP_DPC_USR_CFG_S {
	ISP_DPC_USR_DYNA_COR_CFG_S stUsrDynaCorRegCfg;
	ISP_DPC_USR_STATIC_COR_CFG_S stUsrStaCorRegCfg;
} ISP_DPC_USR_CFG_S;
typedef struct ISP_DPC_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bDpcStatEn;
	GK_U8 u8DpccSetUse; // U4.0
	GK_U8 u8DpccLineKerdiffFac;
	GK_U8 u8DpccBlendMode;
	GK_U8 u8DpccBitDepthSel;
	GK_U16 au16DpccLineStdThr[5]; // u12.0
	GK_U8 au8DpccLineDiffThr[5];
	GK_U8 au8DpccLineAverFac[5];
	GK_U16 u16DpccMode; // U10.0
	GK_U16 u16DpccMethodsSet1; // U13.0
	GK_U16 u16DpccMethodsSet2; // U13.0
	GK_U16 u16DpccMethodsSet3; // U13.0
	GK_U16 au16DpccLineThr[3]; // U16.0
	GK_U16 au16DpccLineMadFac[3]; // U14.0
	GK_U16 au16DpccPgFac[3]; // U14.0
	GK_U16 au16DpccRndThr[3]; // U16.0
	GK_U32 au32DpccRndThrMtp[3]; // U32.0
	GK_U16 au16DpccRgFac[3]; // U14.0
	GK_U32 au32DpccRgFacMtp[3]; // U32.0
	GK_U16 u16DpccRoLimits; // U12.0
	GK_U16 u16DpccRndOffs; // U12.0
	GK_U32 u32DpccBadThresh; // U32.0
	GK_U32 u32DpccAlpha; // U32.0
} ISP_DPC_DYNA_CFG_S;

typedef struct ISP_DPC_MODULE_REG_CFG_S {
	GK_BOOL abDpcEn[4];
	GK_BOOL bLut2SttEn;
	GK_U8 u8ChnNum;
	ISP_DPC_STATIC_CFG_S stStaticRegCfg;
	ISP_DPC_DYNA_CFG_S stDynaRegCfg;
	ISP_DPC_USR_CFG_S stUsrRegCfg;
} ISP_DPC_REG_CFG_S;

typedef struct ISP_SHARPEN_STATIC_REG_CFG_S {
	GK_U8 u8ditherMode;
	GK_BOOL bStaticResh;
	GK_U8 u8gainThdSelD;
	GK_U8 u8dirVarScale;
	GK_U8 u8dirRly[2];

	GK_U8 u8MaxVarClipMin; //  8 bit
	GK_U16 u16oMaxChg;
	GK_U16 u16uMaxChg;
	GK_U8 u8shtVarWgt1;
	GK_U8 u8shtVarDiffWgt0;
	GK_U8 u8shtVarThd0;
	GK_U8 u8shtVarSft;
	GK_U8 u8lmtMF[ISP_SHARPEN_FREQ_CORING_LENGTH];
	GK_U8 u8lmtHF[ISP_SHARPEN_FREQ_CORING_LENGTH];

	GK_U8 u8skinSrcSel;
	GK_U8 u8skinCntThd[2];
	GK_U8 u8skinEdgeSft;
	GK_U8 u8skinEdgeThd[2];

	GK_U8 u8chrRVarSft;
	GK_U8 u8chrROriCb; // U8.0
	GK_U8 u8chrROriCr; // U8.0
	GK_U8 u8chrRSft[4]; // U0.3
	GK_U8 u8chrRThd[2];
	GK_U8 u8chrGOriCb; // U8.0
	GK_U8 u8chrGOriCr; // U8.0
	GK_U8 u8chrGSft[4]; // U0.3
	GK_U8 u8chrGThd[2]; // U8.0
	GK_U8 u8chrBVarSft;
	GK_U8 u8chrBOriCb; // U8.0
	GK_U8 u8chrBOriCr; // U8.0
	GK_U8 u8chrBSft[4]; // U0.3
	GK_U8 u8chrBThd[2];

	//GK_U8    u8dirRt[2] ;

	// filter
	GK_S8 s8lpfCoefUD[3]; // S8.0
	GK_S8 s8lpfCoefD[3]; // S8.0
	GK_S8 s8hsfCoefUD[3]; // S8.0
	GK_S8 s8hsfCoefD0[13]; // S8.0
	GK_S8 s8hsfCoefD1[13]; // S8.0
	GK_S8 s8hsfCoefD2[13]; // S8.0
	GK_S8 s8hsfCoefD3[13]; // S8.0

	GK_U8 u8lpfSftUD; // U4.0--[3, 15]
	GK_U8 u8lpfSftD; // U4.0--[3, 15]
	GK_U8 u8hsfSftUD; // U4.0--[3, 15]
	GK_U8 u8hsfSftD; // U4.0--[3, 15]

	GK_U8 bEnShp8Dir;

	// shoot ctrl
	GK_U8 u8lfGainWgt;
	GK_U8 u8hfGainSft;
	GK_U8 u8mfGainSft;
	GK_U8 u8shtVarSel;
	GK_U8 u8shtVar5x5Sft;
	GK_U8 u8detailThdSel;
	GK_U8 u8detailThdSft;

	GK_U8 u8skinCntMul; // convert 31/(skin_cnt_th2-skin_cnt_th1) to multiplication.        // 5-bit unsigned integer    // U5.0
} ISP_SHARPEN_STATIC_REG_CFG_S;

typedef struct ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8gainThdSftD;
	GK_U8 u8gainThdSelUD;
	GK_U8 u8gainThdSftUD;
	GK_U8 u8dirVarSft;

	GK_U8 u8dirRlyThrLow;
	GK_U8 u8dirRlyThrhigh;

	GK_U8 u8selPixWgt;
	GK_U8 u8shtVarWgt0;
	GK_U8 u8shtVarDiffThd[2];
	GK_U8 u8shtVarDiffWgt1;

	GK_U8 u8RmfGainScale;
	GK_U8 u8BmfGainScale;
	GK_S16 s16shtVarDiffMul; // Convert division to multiplication.           // 12-bit signed integer   // S8.4

} ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S;

typedef struct ISP_SHARPEN_MPI_DYNA_REG_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bPreRegNewEn;
	GK_BOOL bSwitchMode;
	GK_U8 u8BufId;
	GK_U16 u16mfGainD[SHRP_GAIN_LUT_SIZE];
	GK_U16 u16mfGainUD[SHRP_GAIN_LUT_SIZE];
	GK_U16 u16hfGainD[SHRP_GAIN_LUT_SIZE];
	GK_U16 u16hfGainUD[SHRP_GAIN_LUT_SIZE];
	GK_U8 u8oshtAmt;
	GK_U8 u8ushtAmt;

	GK_U8 u8bEnChrCtrl;
	GK_U8 u8chrRGain; // U1.5  [0 32]
	GK_U16 u16chrGGain; // U1.5  [0 32]
	GK_U16 u16chrGmfGain; // U1.5  [0 32]
	GK_U8 u8chrBGain; // U1.5    [0 32]
	GK_U8 u8bEnSkinCtrl;
	GK_U8 u8skinEdgeWgt[2]; // U0.5    [0 31]

	GK_U8 bEnWeakDetailAdj;
	GK_U8 u8WeakDetailGain;

	GK_U8 u8bEnShtCtrlByVar;
	GK_U8 u8shtBldRt;
	GK_U8 u8shtVarThd1;
	GK_U8 u8bEnLumaCtrl;
	GK_BOOL bEnDetailCtrl;
	GK_U8 u8detailOshtAmt;
	GK_U8 u8detailUshtAmt;
	GK_U8 au8LumaWgt[ISP_SHARPEN_LUMA_NUM];
	GK_U16 u16shtVarMul; // Convert division to multiplication.           // 11-bit unsigned integer // U7.4
	GK_S16 s16detailOshtMul;
	GK_S16 s16detailUshtMul;
	GK_U8 u8dirDiffSft;
	GK_U8 u8dirRt[2];
	GK_S16 s16chrRMul; // 9-bit signed integer     // S5.4
	GK_S16 s16chrGMul; // s10.4
	GK_S16 s16chrGmfMul; // s10.4
	GK_S16 s16chrBMul; // 9-bit signed integer     // S5.4
	GK_S16 s16skinEdgeMul; // convert (skin_edge_wgt2-skin_edge_wgt1)/(skin_edge_th2-skin_edge_th1) to multiplication.      // 9-bit signed integer // S5.4
	GK_U8 u8skinMaxU;
	GK_U8 u8skinMinU;
	GK_U8 u8skinMaxV;
	GK_U8 u8skinMinV;
	GK_U16 u16oMaxGain; // 11bit
	GK_U16 u16uMaxGain; // 11bit
	GK_U8 u8detailOshtThr[2];
	GK_U8 u8detailUshtThr[2];
	GK_U32 u32UpdateIndex;
} ISP_SHARPEN_MPI_DYNA_REG_CFG_S;

typedef struct ISP_SHARPEN_DYNA_REG_CFG_S {
	ISP_SHARPEN_MPI_DYNA_REG_CFG_S stMpiDynaRegCfg;
	ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S stDefaultDynaRegCfg;
} ISP_SHARPEN_DYNA_REG_CFG_S;

typedef struct ISP_SHARPEN_REG_CFG_S {
	GK_BOOL bEnable;
	GK_BOOL bLut2SttEn;
	ISP_SHARPEN_DYNA_REG_CFG_S stDynaRegCfg;
	ISP_SHARPEN_STATIC_REG_CFG_S stStaticRegCfg;
} ISP_SHARPEN_REG_CFG_S;

typedef struct ISP_GE_STATIC_CFG_S {
	GK_BOOL bStaticResh;
	GK_BOOL bGeGrGbEn; // u1.0
	GK_BOOL bGeGrEn; // u1.0
	GK_BOOL bGeGbEn; // u1.0
} ISP_GE_STATIC_CFG_S;

typedef struct ISP_GE_USR_CFG_S {
	GK_BOOL bResh;
	GK_U8 au8GeCtSlope1; // u4.0    0x9
	GK_U8 au8GeCtSlope2; // u4.0    0x9
	GK_U16 au16GeCtTh2; // u14.0   0x200
} ISP_GE_USR_CFG_S;

typedef struct ISP_GE_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_U16 au16GeCtTh1; // u14.0   0x400
	GK_U16 au16GeCtTh3; // u14.0   0x200
	GK_U16 u16GeStrength; // u9.0  0x80
} ISP_GE_DYNA_CFG_S;

typedef struct ISP_EXPANDER_STATIC_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8BitDepthIn;
	GK_U8 u8BitDepthOut;
	GK_U16 au16Lut[EXPANDER_NODE_NUM];
} ISP_EXPANDER_STATIC_CFG_S;

typedef struct ISP_EXPANDER_MODULE_REG_CFG_S {
	GK_BOOL bEnable;
	ISP_EXPANDER_STATIC_CFG_S stStaticCfg;
} ISP_EXPANDER_REG_CFG_S;

typedef struct ISP_GE_MODULE_REG_CFG_S {
	GK_BOOL abGeEn;
	ISP_GE_STATIC_CFG_S stStaticRegCfg;
	ISP_GE_DYNA_CFG_S stDynaRegCfg;
	ISP_GE_USR_CFG_S stUsrRegCfg;
} ISP_GE_REG_CFG_S;

typedef struct ISP_MCDS_STATIC_REG_CFG_S {
	GK_BOOL bStaticResh;
	GK_BOOL bHcdsEn; // UV Horizontal DS enable; 0: close; 1: open
	GK_S32 s32HCoef0;
	GK_S32 s32HCoef1;
	GK_S32 s32HCoef2;
	GK_S32 s32HCoef3;
	GK_S32 s32HCoef4;
	GK_S32 s32HCoef5;
	GK_S32 s32HCoef6;
	GK_S32 s32HCoef7;
	GK_S32 s32EnFilterV;
} ISP_MCDS_STATIC_REG_CFG_S;

typedef struct ISP_MCDS_DYNA_REG_CFG_S {
	GK_BOOL bDynaResh;
	GK_BOOL bVcdsEn; // UV vertical DS enable; 0: close; 1: open
} ISP_MCDS_DYNA_REG_CFG_S;

typedef struct ISP_MCDS_REG_CFG_S {
	GK_BOOL bMCDSen;
	ISP_MCDS_STATIC_REG_CFG_S stStaticRegCfg;
	ISP_MCDS_DYNA_REG_CFG_S stDynaRegCfg;

} ISP_MCDS_REG_CFG_S;
#define NLUT_LENGTH 129
#define WLUT_LENGTH 33
#define WDR_LUT_LENGTH 128
#define WDR_CHL_CHNLN 5

#define WDR_FRAME0 0
#define WDR_FRAME1 1
#define WDR_FRAME2 2
#define WDR_FRAME3 3

#define WDR_CHL_Rg 0
#define WDR_CHL_Gr 1
#define WDR_CHL_Gb 2
#define WDR_CHL_Bg 3
#define WDR_CHL_MAX 2

typedef struct ISP_FSWDR_STATIC_CFG_S {
	GK_BOOL bFirstFrame;
	GK_BOOL bResh;
	GK_BOOL bGrayScaleMode;

	GK_BOOL bSaveBLC;
	GK_BOOL bForceLongSmoothEn; // u1.0,[0,1]

	GK_U8 u8bldrLHFIdx; // u5.0, [0,16]
	GK_U8 u8MaskSimilarThr; // u6.0, [0,63],
	GK_U8 u8MaskSimilarCnt; // u3.0, [0,7]
	GK_U16 au16ExpoValue[WDR_CHL_MAX]; // u8.6, [0,16383]
	GK_U16 flick_exp_ratio;
	GK_U16 u32MaxRatio; // u17.0, [0,131071]
	GK_U16 u16dftWgtFL; // u9.0, [0,256]
	GK_U16 au16ExpoRRatio[WDR_CHL_MAX - 1]; // u10.0,[0,1023]
	GK_U32 au32BlcComp[WDR_CHL_MAX - 1]; // s27.0,[0,67108863]

	GK_U16 u16SaturateThr; // u14.0,[0,16383]
	GK_U16 u16FusionSaturateThd; // u12.0,[0,4095]
} ISP_FSWDR_STATIC_CFG_S;

typedef struct ISP_FSWDR_USR_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bFusionMode; // u1.0,  Fusion mode
	GK_BOOL bShortExpoChk; // u1.0, [0,1]
	GK_U8 u8MdtLBld; // u8.0, [0,254],the amount of long-expo is added in motion mask
	GK_U8 u8BnrFullMdtThr; // u8.0, [0,254]

	GK_U8 u8MdtStillThr; // u8.0, [0,254],less than this threshold, motion mask will be 0; u8MdtStillThr <= u8MdtFullThr
	GK_U8 u8MdtFullThr; // u8.0, [0,254],more than this threshold, motion mask will look for the next one
	GK_U8 u8FullMotSigWgt; // u5.0, [0,31]
	GK_U32 u32UpdateIndex;

	GK_U8 u8ShortSigmaGwgt; // U4.0  [0,15]
	GK_U8 u8ShortSigmaCwgt; // U4.0 [0,15]

	GK_BOOL bSFNR; // u1.0

	GK_U16 u16PixelAvgMaxDiff; // u11.0,[0,4095]

	GK_U8 u8ShortSigmaL1GWeight; // u4.4,[0.255]
	GK_U8 u8ShortSigmaL2GWeight; // u4.4,[0.255]
	GK_U8 u8ShortSigmaL1CWeight; // u4.4,[0.255]
	GK_U8 u8ShortSigmaL2CWeight; // u4.4,[0.255]

	GK_BOOL bFusionSmooth; // u1.0, [0,1]
	GK_U32 u32FusionSmoothDiffThd; // u16.0, [0,65535]

} ISP_FSWDR_USR_CFG_S;

typedef struct ISP_FSWDR_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bWDRMdtEn; // u1.0,  motion detection switch
	GK_U8 u8MdThrLowGain; // u4.2, [0,63]
	GK_U8 u8MdThrHigGain; // u4.2, [0,63]
	GK_U8 u8SqrtAgainG; // u3.0, [0,6]
	GK_U8 u8SqrtDgainG; // u3.0, [0,6]
	GK_U8 u8MdtNosFloor; // u7.0, [0,127]
	GK_U16 u16ShortThr; // u14.0,[0,16383],the threshold of beginning to adopt short exposure
	GK_U16 u16LongThr; // u14.0,[0,16383],below the threshold will adopt long exposure  u16LongThr <= u16ShortThr
	GK_U16 au16StillThr[WDR_CHL_MAX - 1]; // u14.0,[0,16383]

	GK_U16 u16TNosFloor; // u12.0,[0,4095]
	GK_BOOL bErosionEn;
	GK_U8 u8TextureThdWgt; // u4.4,[0,255]
	GK_U16 au16FusionThrR[WDR_CHL_MAX]; // u14.0,[0,16383]
	GK_U16 au16FusionThrG[WDR_CHL_MAX]; // u14.0,[0,16383]
	GK_U16 au16FusionThrB[WDR_CHL_MAX]; // u14.0,[0,16383]

	GK_BOOL bForceLong; // u1.0,[0,1]
	GK_U16 u16ForceLongLowThr; // u11.0,[0,4095]
	GK_U16 u16ForceLongHigThr; // u11.0,[0,4095]
	GK_U16 u16ShortCheckThd; // u11.0,[0,4095]

	GK_BOOL bFlickEn;
	GK_BOOL bBcomEn;
	GK_BOOL bBdecEn;
	GK_U8 u8bcom_alpha;
	GK_U8 u8bdec_alpha;
	GK_U8 u8FrmMerge;
	GK_BOOL bFusionDataMode;

	GK_U16 u16MdtNFLowThr; // u11.0,[0,2047]
	GK_U16 u16MdtNFHighThr; // u11.0,[0,2047]
	GK_U16 u16GainSumLowThr; // u8.0,[0,255]
	GK_U16 u16GainSumHighThr; // u8.0,[0,255]

	GK_U16 u16ForceLongSlope; // u10.4,[0,16383]
	GK_U16 u16WgtSlope; // u8.4,[0,4095]

} ISP_FSWDR_DYNA_CFG_S;

typedef struct ISP_FSWDR_SYNC_CFG_S {
	GK_BOOL bFusionMode;
	GK_BOOL bWDRMdtEn;
	GK_U16 u16ShortThr;
	GK_U16 u16LongThr;
	GK_U16 u16Offset0;
	GK_U16 u16WgtSlope;
} ISP_FSWDR_SYNC_CFG_S;

typedef struct ISP_WDR_REG_CFG_S {
	GK_BOOL bWDREn;
	ISP_FSWDR_STATIC_CFG_S stStaticRegCfg;
	ISP_FSWDR_DYNA_CFG_S stDynaRegCfg;
	ISP_FSWDR_USR_CFG_S stUsrRegCfg;
	ISP_FSWDR_SYNC_CFG_S stSyncRegCfg;
} ISP_WDR_REG_CFG_S;

typedef struct {
	GK_BOOL bStaticResh;
} ISP_RC_STATIC_CFG_S;

typedef struct {
	GK_BOOL bResh;
	GK_U16 u16CenterVerCoor;
	GK_U16 u16CenterHorCoor;
	GK_U32 u32SquareRadius;
} ISP_RC_USR_CFG_S;

typedef struct ISP_RADIAL_CROP_REG_CFG_S {
	GK_BOOL bRcEn;
	ISP_RC_STATIC_CFG_S stStaticRegCfg;
	ISP_RC_USR_CFG_S stUsrRegCfg;
} ISP_RADIAL_CROP_REG_CFG_S;

typedef struct ISP_LSC_STATIC_CFG_S {
	GK_BOOL bStaticResh;
	GK_U8 u8WinNumH;
	GK_U8 u8WinNumV;
} ISP_LSC_STATIC_CFG_S;

typedef struct ISP_LSC_USR_CFG_S {
	GK_BOOL bCoefUpdate;
	GK_BOOL bLutUpdate;
	GK_U8 u8BufId;
	GK_U8 u8MeshScale;
	GK_U16 u16MeshStr;
	GK_U16 u16WidthOffset;
	GK_U16 au16DeltaX[ISP_LSC_GRID_COL - 1];
	GK_U16 au16DeltaY[(ISP_LSC_GRID_ROW - 1) / 2];
	GK_U16 au16InvX[ISP_LSC_GRID_COL - 1];
	GK_U16 au16InvY[(ISP_LSC_GRID_ROW - 1) / 2];
	GK_U32 au32RGain[ISP_LSC_GRID_POINTS];
	GK_U32 au32GrGain[ISP_LSC_GRID_POINTS];
	GK_U32 au32GbGain[ISP_LSC_GRID_POINTS];
	GK_U32 au32BGain[ISP_LSC_GRID_POINTS];
	GK_U32 u32UpdateIndex;
} ISP_LSC_USR_CFG_S;

typedef struct ISP_LSC_MODULE_REG_CFG_S {
	GK_BOOL bLscEn;
	GK_BOOL bLut2SttEn;
	ISP_LSC_USR_CFG_S stUsrRegCfg;
	ISP_LSC_STATIC_CFG_S stStaticRegCfg;
} ISP_LSC_REG_CFG_S;

typedef struct ISP_RLSC_STATIC_CFG_S {
	GK_BOOL bStaticResh;

	GK_U16 u16NodeNum;

} ISP_RLSC_STATIC_CFG_S;

typedef struct ISP_RLSC_USR_CFG_S {
	GK_BOOL bUsrResh;
	GK_BOOL bRadialCropEn;
	GK_BOOL bRLscFuncEn;
	GK_BOOL bCoefUpdate;
	GK_BOOL bLutUpdate;
	GK_U8 u8BufId;

	GK_U16 u16WidthOffset;

	GK_U16 u16CenterRX;
	GK_U16 u16CenterRY;
	GK_U16 u16CenterGrX;
	GK_U16 u16CenterGrY;
	GK_U16 u16CenterGbX;
	GK_U16 u16CenterGbY;
	GK_U16 u16CenterBX;
	GK_U16 u16CenterBY;

	GK_U16 u16OffCenterR;
	GK_U16 u16OffCenterGr;
	GK_U16 u16OffCenterGb;
	GK_U16 u16OffCenterB;

	GK_U32 u32ValidRadius;

	GK_U16 u16GainStr;
	GK_U8 u8GainScale;

	GK_U32 au32Lut0Chn0[ISP_RLSC_POINTS + 1];
	GK_U32 au32Lut0Chn1[ISP_RLSC_POINTS + 1];
	GK_U32 au32Lut1Chn2[ISP_RLSC_POINTS + 1];
	GK_U32 au32Lut1Chn3[ISP_RLSC_POINTS + 1];
	GK_U32 u32UpdateIndex;

} ISP_RLSC_USR_CFG_S;

typedef struct ISP_RLSC_MODULE_REG_CFG_S {
	GK_BOOL bRLscEn;
	GK_BOOL bLut2SttEn;
	ISP_RLSC_USR_CFG_S stUsrRegCfg;
	ISP_RLSC_STATIC_CFG_S stStaticRegCfg;
} ISP_RLSC_REG_CFG_S;

typedef struct ISP_LOCAL_CAC_STATIC_CFG_S {
	GK_BOOL bStaticResh;
	GK_BOOL bNddmCacBlendEn;
	GK_U8 u8RCounterThr;
	GK_U8 u8GCounterThr;
	GK_U8 u8BCounterThr;
	GK_U8 au8DePurpleStr[LCAC_STRENGTH_NUM];
	GK_U16 u16SatuThr;
	GK_U16 u16NddmCacBlendRate;
	GK_BOOL bCacCorMode;
} ISP_LOCAL_CAC_STATIC_CFG_S;

typedef struct ISP_LOCAL_CAC_USR_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8LumaHighCntThr; // u8.0, [0,153]
	GK_U8 u8CbCntHighThr; // u7.0, [0,68]
	GK_U8 u8CbCntLowThr; // u7.0, [0,68]
	GK_U8 u8BldAvgCur; // u4.0, [0, 8]
	//GK_U8   u8DeFColorCr;
	//GK_U8   u8DeFColorCb;
	GK_U16 u16CbCntRatio; // u7.4, [0,1024]
	GK_U16 u16CbThr; // u12.0, [0,4095]
	GK_U16 u16VarThr;
	GK_S16 s16CbCrRatioLmtHigh;
	GK_U32 u32UpdateIndex;
} ISP_LOCAL_CAC_USR_CFG_S;

typedef struct ISP_LOCAL_CAC_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8DePurpleCtrCr;
	GK_U8 u8DePurpleCtrCb;
	GK_U16 u16LumaThr; // u12.0, [0,4095]
	GK_U16 u16RLumaThr;
	GK_U16 u16GLumaThr;
	GK_U16 u16BLumaThr;
	GK_S16 s16CbCrRatioLmtLow;
} ISP_LOCAL_CAC_DYNA_CFG_S;

typedef struct ISP_LOCAL_CAC_MODULE_REG_CFG_S {
	GK_BOOL bLocalCacEn;
	ISP_LOCAL_CAC_USR_CFG_S stUsrRegCfg;
	ISP_LOCAL_CAC_DYNA_CFG_S stDynaRegCfg;
	ISP_LOCAL_CAC_STATIC_CFG_S stStaticRegCfg;
} ISP_LCAC_REG_CFG_S;

typedef struct ISP_GLOBAL_CAC_STATIC_CFG_S {
	GK_BOOL bStaticResh;
	GK_BOOL bGCacVerFilEn;
	GK_U8 u8GCacBlendSel;
	GK_U8 u8GcacChrVerMode;
	GK_U8 u8GcacClipModeHor;
	GK_U8 u8GcacClipModeVer;
} ISP_GLOBAL_CAC_STATIC_CFG_S;

typedef struct ISP_GLOBAL_CAC_USR_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8VerNormShift;
	GK_U8 u8VerNormFactor;
	GK_U8 u8HorNormShift;
	GK_U8 u8HorNormFactor;
	GK_U16 u16StartCoorHor;
	GK_U16 u16StartCoorVer;
	GK_S16 s16ParamRedA;
	GK_S16 s16ParamRedB;
	GK_S16 s16ParamRedC;
	GK_S16 s16ParamBlueA;
	GK_S16 s16ParamBlueB;
	GK_S16 s16ParamBlueC;
	GK_U16 u16CenterCoorHor;
	GK_U16 u16CenterCoorVer;
	GK_U16 u16CorVarThr;
	GK_U32 u32UpdateIndex;
} ISP_GLOBAL_CAC_USR_CFG_S;

typedef struct ISP_GLOBAL_CAC_DYNA_CFG_S {
	GK_U32 u32Reserved;
} ISP_GLOBAL_CAC_DYNA_CFG_S;

typedef struct ISP_GLOBAL_CAC_MODULE_REG_CFG_S {
	GK_BOOL bGlobalCacEn;
	ISP_GLOBAL_CAC_USR_CFG_S stUsrRegCfg;
	ISP_GLOBAL_CAC_DYNA_CFG_S stDynaRegCfg;
	ISP_GLOBAL_CAC_STATIC_CFG_S stStaticRegCfg;
} ISP_GCAC_REG_CFG_S;

typedef struct ISP_DEMOSAIC_STATIC_CFG {
	GK_BOOL bResh;
	GK_BOOL bDeSatEnable; // u1.0
	GK_BOOL bAHDEnable; // u1.0
	GK_BOOL bDeFakeEnable; // u1.0
	GK_U8 u8Lpff3; // u8.0,
	GK_U8 u8hvBlendLimit1; // u8.0,
	GK_U8 u8hvBlendLimit2; // u8.0,
	GK_U8 u8hvColorRatio; // u8.0,
	GK_U8 u8hvSelection; // u2.0
	GK_U8 u8GClipBitSft; // u4.0,
	GK_U8 u8DitherMask; // u8.0,
	GK_U8 u8DitherRatio; // u8.0,
	GK_S16 s16CcVarThresh; // s13.0,
	GK_U16 u16DeSatThresh1; // u10.0,
	GK_U16 u16DeSatThresh2; // u10.0,
	GK_U16 u16DeSatHig; // u9.0,
	GK_U16 u16DeSatProtSl; // u10.0
	GK_U16 u16AhdPart1; // u10.0,
	GK_U16 u16AhdPart2; // u8.0,
	GK_U16 u16GFThLow; // u9.0,   gf low-fix strength
	GK_U16 u16GFThHig; // u9.0,   gf high-fix strength
	GK_U16 u16CbCrAvgThr; // u12.0,
} ISP_DEMOSAIC_STATIC_CFG_S;

typedef struct ISP_DEMOSAIC_DYNA_CFG {
	GK_BOOL bResh;
	GK_BOOL bUpdateGF;
	GK_U16 u16FakeCrVarThrLow; // u16.0
	GK_U16 u16FakeCrVarThrHigh; // u16.0
	GK_U8 u8DeFColorCr;
	GK_U8 u8DeFColorCb;
	GK_U8 u8CcHFMaxRatio; // u5.0,
	GK_U8 u8CcHFMinRatio; // u5.0,
	GK_U8 u8Lpff0; // s4.0,
	GK_U8 u8Lpff1; // u5.0,
	GK_U8 u8Lpff2; // u5.0,
	GK_U8 u8DitherMax; // u8.0,
	GK_U8 u8FcrGFGain; // s3.2,   fcr control
	GK_U8 u8AwbGFGainLow; // u3.4,
	GK_U8 u8AwbGFGainHig; // u3.4,
	GK_U8 u8AwbGFGainMax; // u4.0,
	GK_U8 u8EhcGray; // u3.4,
	GK_U16 u16DeSatLow; // u9.0,
	GK_S16 s16DeSatRatio; // s9.4,
	GK_U16 u16DeSatProtTh; // u10.0
	GK_U16 u16HfIntpBlurThLow; // u10.0,
	GK_U16 u16HfIntpBlurThHig; // u10.0,
	GK_U16 u16HfIntpBlurRatio; // u9.4,
	GK_U16 u16FcrDetLow; // u12.0,  fcr det thresh
	GK_U16 au16GFBlurLut[ISP_DEMOSAIC_LUT_LENGTH]; // u12.0,
	GK_U32 u32CoefK; // u18.6
	GK_U32 u32CoefB; // u13.6
	GK_U8 u8ColorNoiseThdY; // u4.0
	GK_U8 u8ColorNoiseStrY; // u6.0
} ISP_DEMOSAIC_DYNA_CFG_S;

typedef struct ISP_DEMOSAIC_MODULE_REG_CFG {
	GK_BOOL bVhdmEnable; // u1.0,
	GK_BOOL bNddmEnable; // u1.0,
	ISP_DEMOSAIC_STATIC_CFG_S stStaticRegCfg;
	ISP_DEMOSAIC_DYNA_CFG_S stDynaRegCfg;
} ISP_DEMOSAIC_REG_CFG_S;

typedef struct ISP_ANTIFALSECOLOR_STATIC_CFG {
	GK_BOOL bResh;
	GK_U16 u16FcrLimit1; // u12.0,
	GK_U16 u16FcrLimit2; // u12.0,
	GK_U16 u16FcrThr; // u12.0,
} ISP_ANTIFALSECOLOR_STATIC_CFG_S;

typedef struct ISP_ANTIFALSECOLOR_DYNA_CFG {
	GK_BOOL bResh;
	GK_U8 u8FcrGain; // u5.0,
	GK_U8 u8FcrRatio; // u6.0,
	GK_U8 u8FcrGrayRatio; // u5.0,
	GK_U8 u8FcrCmaxSel; // u4.0,
	GK_U8 u8FcrDetgSel; // u4.0,
	GK_U16 u16FcrHfThreshLow; // u10.0,  fcr hf thresh low
	GK_U16 u16FcrHfThreshHig; // u10.0,  fcr hf thresh high
} ISP_ANTIFALSECOLOR_DYNA_CFG_S;

typedef struct ISP_ANTIFALSECOLOR_MODULE_REG_CFG {
	GK_BOOL bFcrEnable; // u1.0,
	ISP_ANTIFALSECOLOR_STATIC_CFG_S stStaticRegCfg;
	ISP_ANTIFALSECOLOR_DYNA_CFG_S stDynaRegCfg;
} ISP_ANTIFALSECOLOR_REG_CFG_S;

typedef struct ISP_CA_USR_CFG {
	GK_BOOL bResh;
	GK_BOOL bCaLutUpdateEn; // u1.0
	GK_U8 u8BufId;
	GK_U16 u16CaLumaThrHigh; // u10.0
	GK_S16 s16CaLumaRatioHigh; // U1.10   High Lightness Thresh' Ratio in Y-Alpha
	GK_S16 s16CaLumaRatioLow;
	GK_U16 au16YRatioLUT[ISP_CA_YRATIO_LUT_LENGTH]; // U1.10
	GK_S16 s16CaYLumaRatio;
	GK_S16 s16CaSkinBetaRatio;
	GK_U32 u32UpdateIndex;
} ISP_CA_USR_CFG_S;

typedef struct ISP_CA_DYNA_CFG {
	GK_BOOL bResh;
	GK_U16 u16CaISORatio; // 1.10bit  ISO Ratio  For UV,Max = 2047 FW Limi
} ISP_CA_DYNA_CFG_S;

typedef struct ISP_CA_STATIC_CFG {
	GK_BOOL bStaticResh;
	GK_BOOL bCaLlhcProcEn; // u1.0
	GK_BOOL bCaSkinProcEn; // u1.0
	GK_BOOL bCaSatuAdjEn; // u1.0
	GK_U16 u16CaLumaThrLow; // u10.0
	GK_U16 u16CaDarkChromaThrLow; // u10.0
	GK_U16 u16CaDarkChromaThrHigh; // u10.0
	GK_U16 u16CaSDarkChromaThrLow; // u10.0
	GK_U16 u16CaSDarkChromaThrHigh; // u10.0
	GK_S16 s16CaLumaRatioLow; // U1.10
	GK_U16 u16CaSkinLowLumaMinU; // u10
	GK_U16 u16CaSkinLowLumaMaxU; // u10
	GK_U16 u16CaSkinLowLumaMinUy; // u10
	GK_U16 u16CaSkinLowLumaMaxUy; // u10
	GK_U16 u16CaSkinHighLumaMinU; // u10
	GK_U16 u16CaSkinHighLumaMaxU; // u10
	GK_U16 u16CaSkinHighLumaMinUy; // u10
	GK_U16 u16CaSkinHighLumaMaxUy; // u10
	GK_U16 u16CaSkinLowLumaMinV; // u10
	GK_U16 u16CaSkinLowLumaMaxV; // u10
	GK_U16 u16CaSkinLowLumaMinVy; // u10
	GK_U16 u16CaSkinLowLumaMaxVy; // u10
	GK_U16 u16CaSkinHighLumaMinV; // u10
	GK_U16 u16CaSkinHighLumaMaxV; // u10
	GK_U16 u16CaSkinHighLumaMinVy; // u10
	GK_U16 u16CaSkinHighLumaMaxVy; // u10
	GK_S16 s16CaSkinUvDiff; // S11
	GK_U16 u16CaSkinRatioThrLow; // u12.0
	GK_U16 u16CaSkinRatioThrMid; // u12.0
	GK_U16 u16CaSkinRatioThrHigh; // u12.0
} ISP_CA_STATIC_CFG_S;

typedef struct ISP_CA_REG_CFG {
	GK_BOOL bCaEn;
	GK_BOOL bLut2SttEn;
	ISP_CA_STATIC_CFG_S stStaticRegCfg;
	ISP_CA_DYNA_CFG_S stDynaRegCfg;
	ISP_CA_USR_CFG_S stUsrRegCfg;
} ISP_CA_REG_CFG_S;

typedef struct ISP_BAYERNR_STATIC_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8JnlmSel; // u2.0
	GK_U8 u8SADWinSizeSel; // u2.0
	GK_U16 u16JnlmMaxWtCoef; // u10.0
	GK_BOOL bLumaSel; // u1.0
	GK_U8 u8EnablePostProc; // u8.0
	GK_U8 u8BilateralEnable; // u8.0
	GK_BOOL bEnableSymmsad; // u1.0
	GK_U8 u8ImplsnrStrength; // u8.0
	GK_BOOL bImplsnrEnable3; // u1.0
	GK_BOOL bImplsnrEnable2; // u1.0
	GK_BOOL bImplsnrEnable1; // u1.0
	GK_U8 u8Gainsad; // u8.0
	GK_BOOL bPatternNoiseReduction; // u1.0
	GK_U8 u8WindowSizeSel; // u8.0

} ISP_BAYERNR_STATIC_CFG_S;

typedef struct ISP_BAYERNR_DYNA_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bMedcEnable; // u1.0
	GK_BOOL bWdrModeEn; // u1.0
	GK_BOOL bWdrFusionEn; // u1.0
	GK_BOOL bCenterWgtEn; // u1.0
	GK_U8 u8WdrMapFltMode; // u1.0
	GK_U8 au8BnrCRatio[ISP_BAYER_CHN_NUM]; // u6.0
	GK_U8 au8AmedMode[ISP_BAYER_CHN_NUM]; // u1.0
	GK_U8 au8AmedLevel[ISP_BAYER_CHN_NUM]; // u2.0
	GK_U8 u8JnlmSymCoef; // u8.0
	GK_U8 u8JnlmGain; // u8.0
	GK_U8 u8WdrMapGain; // u5.0
	GK_U16 u16ShotRatio; // u2.8
	GK_U16 u16JnlmCoringHig; // u16.0
	GK_U16 u16RLmtRgain; // u10.0
	GK_U16 u16RLmtBgain; // u10.0
	GK_U16 u16LmtOffset; // u10.0
	GK_U32 u32EdgeDetThr; // u17.0
	GK_U8 au8GCoringGainLut[ISP_BAYERNR_LUT_LENGTH]; // u4.4
	GK_U8 au8JnlmLimitLut[ISP_BAYERNR_LMTLUTNUM]; // u8.0
	GK_U8 au8JnlmLimitEvenLut[ISP_BAYERNR_LMTLUT_EVEN_NUM]; // u8.0
	GK_U8 au8JnlmLimitOddLut[ISP_BAYERNR_LMTLUT_ODD_NUM]; // u8.0

	GK_U8 au8JnlmLimitSLut[ISP_BAYERNR_LMTLUTNUM]; // u8.0
	GK_U8 au8JnlmLimitSEvenLut[ISP_BAYERNR_LMTLUT_EVEN_NUM]; // u8.0
	GK_U8 au8JnlmLimitSOddLut[ISP_BAYERNR_LMTLUT_ODD_NUM]; // u8.0

	GK_U16 au16JnlmCoringLowLut[ISP_BAYERNR_LUT_LENGTH]; // u14.0
	GK_U16 au16JnlmCoringLowEvenLut[ISP_BAYERNR_LUT_EVEN_LENGTH]; // u14.0
	GK_U16 au16JnlmCoringLowOddLut[ISP_BAYERNR_LUT_ODD_LENGTH]; // u14.0

	GK_U8 au8GcoringLut[ISP_BAYERNR_LUT_LENGTH]; // u8.0
	GK_U8 au8GcoringEvenLut[ISP_BAYERNR_LUT_EVEN_LENGTH]; // u8.0
	GK_U8 au8GcoringOddLut[ISP_BAYERNR_LUT_ODD_LENGTH]; // u8.0
	GK_U32 au32JnlmLimitMultGain[ISP_BAYER_CHN_NUM]; // u21.0
	GK_U32 au32JnlmLimitMultGainS[ISP_BAYER_CHN_NUM]; // u21.0
	GK_U8 u8BufId;
} ISP_BAYERNR_DYNA_CFG_S;

typedef struct ISP_BAYERNR_USR_CFG_S {
	GK_BOOL bResh;
	GK_U8 bBnrLscEn; // u1.0
	GK_BOOL bBnrMonoSensorEn; // u1.0
	GK_U8 u8BnrLscMaxGain; // u2.6
	GK_U16 u16BnrLscCmpStrength; // u1.8
} ISP_BAYERNR_USR_CFG_S;

typedef struct ISP_BAYERNR_MODULE_REG_CFG_S {
	GK_BOOL bBnrEnable; // u1.0
	ISP_BAYERNR_STATIC_CFG_S stStaticRegCfg;
	ISP_BAYERNR_DYNA_CFG_S stDynaRegCfg;
	ISP_BAYERNR_USR_CFG_S stUsrRegCfg;
} ISP_BAYERNR_REG_CFG_S;

typedef struct ISP_LDCI_STATIC_CFG_S {
	GK_BOOL bEnable;
	GK_BOOL bStaticResh;

	GK_U32 u32CalcLumaSel;
	GK_U32 u32LpfSft;
	GK_U32 u32ChrPosDamp;
	GK_U32 u32ChrNegDamp;

} ISP_LDCI_STATIC_CFG_S;

typedef struct ISP_LDCI_DYNA_CFG_S {
	GK_BOOL bEnable;
	GK_BOOL bWrstatEn;
	GK_BOOL bRdstatEn;
	GK_BOOL bCalcEnable;
	GK_BOOL bLut2SttEn;
	GK_U8 u8BufId;

	GK_U32 u32CalcBlcCtrl;
	GK_U32 u32HePosLut[LDCI_HE_LUT_SIZE];
	GK_U32 u32HeNegLut[LDCI_HE_LUT_SIZE];
	GK_U32 u32UsmPosLut[LDCI_DE_USM_LUT_SIZE];
	GK_U32 u32UsmNegLut[LDCI_DE_USM_LUT_SIZE];
	GK_U32 u32ColorGainLut[LDCI_COLOR_GAIN_LUT_SIZE];
	GK_U32 u32LpfCoef[LDCI_LPF_LUT_SIZE];

	GK_U32 u32CalcMapOffsetX;
	GK_U32 u32CalcSmlMapStride;
	GK_U32 u32CalcSmlMapHeight;
	GK_U32 u32CalcTotalZone;
	GK_U32 u32CalcScaleX;
	GK_U32 u32CalcScaleY;
	GK_U32 u32BlkSmlMapWidth[3];
	GK_U32 u32StatVStart;
	GK_U32 u32StatVEnd;
	GK_U32 u32StatHStart;
	GK_U32 u32StatHEnd;
	GK_U32 u32StatSmlMapWidth;
	GK_U32 u32StatSmlMapHeight;
	GK_U32 u32StatTotalZone;
	ISP_LDCI_STAT_S stTprStat;
} ISP_LDCI_DYNA_CFG_S;

typedef struct ISP_LDCI_REG_CFG {
	ISP_LDCI_STATIC_CFG_S stStaticRegCfg;
	ISP_LDCI_DYNA_CFG_S stDynaRegCfg;
} ISP_LDCI_REG_CFG_S;

typedef struct ISP_FPN_STATIC_CFG {
} ISP_FPN_STATIC_CFG_S;

typedef struct ISP_FPN_DYNA_CFG {
	GK_BOOL bIspFpnEnable;
	GK_BOOL bIspFpnOffline;

	GK_U32 u32IspFpnLineFrame;
	GK_U32 u32IspFpnCalibCorr;
	GK_U32 u32IspFpnFrameCalibShift;
	GK_U32 u32IspFpnOutShift;
	GK_U32 u32IspFpnInShift;
	GK_U32 u32IspFpnShift;
	GK_U32 u32IspFpnUpdateMode;
	GK_U32 u32IspFpnMaxO;
	GK_U32 u32IspFpnOverflowThr;
	GK_U32 u32IspFpnOffset[4];
	GK_U32 u32IspFpnStrength[4];
	GK_U32 u32IspFpnCorrectEnId[4];
	GK_U32 u32IspFpnHeight;
	GK_U32 u32IspFpnWidth;
	GK_U32 u32IspFpnUpdate;
	GK_U32 u32IspFpnLineWaddr;
	GK_U32 u32IspFpnLineRaddr;
} ISP_FPN_DYNA_CFG_S;

typedef struct ISP_FPN_REG_CFG {
	ISP_FPN_STATIC_CFG_S stStaticRegCfg;
	ISP_FPN_DYNA_CFG_S stDynaRegCfg;
} ISP_FPN_REG_CFG_S;

typedef struct ISP_GAMMA_STATIC_CFG_S {
	GK_BOOL bStaticResh;
} ISP_GAMMA_STATIC_CFG_S;

typedef struct ISP_GAMMA_USR_CFG_S {
	GK_BOOL bResh;
	GK_BOOL bGammaLutUpdateEn;
	GK_U8 u8BufId;
	GK_BOOL bSwitchMode;
	GK_U16 au16GammaLUT[GAMMA_REG_NODE_NUM]; // u12.0
	GK_U8 au8GammaStep[GAMMA_OUTSEG_NUM];
	GK_U16 au16GammaInSeg[GAMMA_OUTSEG_NUM];
	GK_U16 au16GammaPos[GAMMA_OUTSEG_NUM];
	GK_U32 u32UpdateIndex;
} ISP_GAMMA_USR_CFG_S;

typedef struct ISP_GAMMA_REG_CFG_S {
	GK_BOOL bGammaEn;
	GK_BOOL bLut2SttEn;
	ISP_GAMMA_STATIC_CFG_S stStaticRegCfg;
	ISP_GAMMA_USR_CFG_S stUsrRegCfg;
} ISP_GAMMA_REG_CFG_S;

typedef struct ISP_RGBIR_USR_CFG_S {
	GK_BOOL bResh;

	GK_U8 u8InPattern;
	GK_U8 u8OutPattern;
	GK_U8 u8ThresV;
	GK_U8 u8ThresH;
	GK_U16 u16Gain1; // u12.0
	GK_U16 u16Gain2; // u12.0

	GK_U16 u16ExpCtrl1; // u12.0
	GK_U16 u16ExpCtrl2; // u12.0
	GK_U16 u16ReciExp1; // u12.0
	GK_U16 u16ReciExp2; // u12.0

	GK_S16 s16Matrix[12]; // s15.0
	GK_U32 u32UpdateIndex;
} ISP_RGBIR_USR_CFG_S;

typedef struct ISP_RGBIR_REG_CFG_S {
	GK_BOOL bEnable;
	ISP_RGBIR_USR_CFG_S stUsrRegCfg;
} ISP_RGBIR_REG_CFG_S;

typedef struct ISP_CSC_DYNAMIC_CFG_S {
	// Multiplier coefficients
	GK_BOOL bResh;

	GK_S16 s16CscCoef00; // s5.10
	GK_S16 s16CscCoef01; // s5.10
	GK_S16 s16CscCoef02; // s5.10
	GK_S16 s16CscCoef10; // s5.10
	GK_S16 s16CscCoef11; // s5.10
	GK_S16 s16CscCoef12; // s5.10
	GK_S16 s16CscCoef20; // s5.10
	GK_S16 s16CscCoef21; // s5.10
	GK_S16 s16CscCoef22; // s5.10

	// DC components
	GK_S16 s16CscInDC0; // s11.0
	GK_S16 s16CscInDC1; // s11.0
	GK_S16 s16CscInDC2; // s11.0

	GK_S16 s16CscOutDC0; // s11.0
	GK_S16 s16CscOutDC1; // s11.0
	GK_S16 s16CscOutDC2; // s11.0
} ISP_CSC_DYNA_CFG_S;

typedef struct ISP_CSC_REG_CFG_S {
	GK_BOOL bEnable; // u1.0
	ISP_CSC_DYNA_CFG_S stDynaRegCfg;
} ISP_CSC_REG_CFG_S;

typedef struct {
	gk_bool blc_in;
	gk_bool blc_out;
} isp_blc_static_cfg;

typedef struct {
	gk_u16 blc[ISP_BAYER_CHN_NUM];
	gk_u16 out_blc;
} isp_blc_dyna_cfg;

typedef struct {
	GK_BOOL bReshStatic;
	/* BE module */
	isp_blc_static_cfg stLscBlc;
	isp_blc_static_cfg stDgBlc;
	isp_blc_static_cfg stWdrBlc[4];
	isp_blc_static_cfg stWbBlc;
	isp_blc_static_cfg st4DgBlc[4];
	isp_blc_static_cfg stAeBlc;
	isp_blc_static_cfg stMgBlc;
	isp_blc_static_cfg stBnrBlc;
	isp_blc_static_cfg stRgbirBlc;
} isp_be_blc_static_cfg;

typedef struct {
	GK_BOOL bReshDyna;
	/* BE module */
	isp_blc_dyna_cfg stLscBlc;
	isp_blc_dyna_cfg stDgBlc;
	isp_blc_dyna_cfg stWdrBlc[4];
	isp_blc_dyna_cfg stWbBlc;
	isp_blc_dyna_cfg st4DgBlc[4];
	isp_blc_dyna_cfg stAeBlc;
	isp_blc_dyna_cfg stMgBlc;
	isp_blc_dyna_cfg stBnrBlc;
	isp_blc_dyna_cfg stRgbirBlc;
} isp_be_blc_dyna_cfg;

typedef struct {
	GK_BOOL resh_dyna_init;
	isp_be_blc_static_cfg static_blc;
	isp_be_blc_dyna_cfg dyna_blc;
} isp_be_blc_cfg;

typedef struct {
	GK_BOOL bReshStatic;
	/* FE module */
	isp_blc_static_cfg stFeDgBlc;
	isp_blc_static_cfg stFeWbBlc;
	isp_blc_static_cfg stFeAeBlc;
	isp_blc_static_cfg stFeBlc;
	isp_blc_static_cfg stRcBlc;
} isp_fe_blc_static_cfg;

typedef struct {
	GK_BOOL bReshDyna;
	/* FE module */
	isp_blc_dyna_cfg stFeDgBlc;
	isp_blc_dyna_cfg stFeWbBlc;
	isp_blc_dyna_cfg stFeAeBlc;
	isp_blc_dyna_cfg stFeBlc;
	isp_blc_dyna_cfg stRcBlc;
} isp_fe_blc_dyna_cfg;

typedef struct {
	GK_BOOL resh_dyna_init;
	isp_fe_blc_static_cfg static_blc;
	isp_fe_blc_dyna_cfg dyna_blc;
} isp_fe_blc_cfg;

typedef struct ISP_FE_LUT_UPDATE_REG_CFG_S {
	GK_BOOL bAe1LutUpdate;
	GK_BOOL bLsc1LutUpdate;
} ISP_FE_LUT_UPDATE_REG_CFG_S;

typedef struct ISP_BE_LUT_UPDATE_REG_CFG_S {
	GK_BOOL bAeLutUpdate;
	GK_BOOL bDrcTmLutUpdate;
	GK_BOOL bLCacLutUpdate;
	GK_BOOL bNddmGfLutUpdate;
	GK_BOOL bLscLutUpdate;
	GK_BOOL bPreGammaLutUpdate;
} ISP_BE_LUT_UPDATE_REG_CFG_S;

typedef struct ISP_BE_STT2LUT_REGNEW_REG_CFG_S {
	GK_BOOL bGammaStt2LutRegnew;
	GK_BOOL bCaStt2LutRegnew;
	GK_BOOL bBnrStt2LutRegnew;
	GK_BOOL bLscStt2LutRegnew;
	GK_BOOL bPreGammaStt2LutRegnew;

	/* pingpong */
	GK_BOOL bDehazeStt2LutRegnew; /* every frame */
	GK_BOOL bLdciStt2LutRegnew; /* every frame */
	GK_BOOL bSharpenStt2LutRegnew; /* every frame */
} ISP_BE_STT2LUT_REGNEW_REG_CFG_S;

typedef struct ISP_DEHAZE_LUT2STT_SYNC_REG_CFG_S {
	GK_U16 u16AirR;
	GK_U16 u16AirG;
	GK_U16 u16AirB;
	GK_U8 u8Strength;
} ISP_DEHAZE_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_LSC_LUT2STT_SYNC_REG_CFG_S {
	GK_BOOL bResh;
	GK_U8 u8MeshScale;
} ISP_LSC_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_BNR_LUT2STT_SYNC_REG_CFG_S {
	GK_U32 au32JnlmLimitMultGain[4];
	GK_U32 au32JnlmLimitMultGainS[4];
	GK_U8 u8JnlmGain;
} ISP_BNR_LUT2STT_SYNC_REG_CFG_S;
typedef struct ISP_LDCI_LUT2STT_SYNC_REG_CFG_S {
	GK_U32 u32CalcBlcCtrl;
} ISP_LDCI_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_SHARPEN_LUT2STT_SYNC_REG_CFG_S {
	ISP_SHARPEN_MPI_DYNA_REG_CFG_S stMpiDynaRegCfg;
	ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S stDefaultDynaRegCfg;
} ISP_SHARPEN_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_CA_LUT2STT_SYNC_REG_CFG_S {
	GK_S16 s16CaLumaRatioHigh;
	GK_S16 s16CaLumaRatioLow;
	GK_S16 s16CaYLumaRatio;
	GK_U16 u16CaIsoRatio;
} ISP_CA_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_LUT2STT_SYNC_REG_CFG_S {
	GK_BOOL bAeEn;
	GK_BOOL bLaEn;
	GK_BOOL bAwbEn;
	GK_BOOL bWbEn;
	GK_BOOL bCcmEn;
	GK_BOOL bCcmColorToneEn;
	GK_BOOL bAfEn;
	GK_BOOL bSharpenEn;
	GK_BOOL bVhdmEn;
	GK_BOOL bNddmEn;
	GK_BOOL bLdciEn;
	GK_BOOL bLcacEn;
	GK_BOOL bGcacEn;
	GK_BOOL bFcrEn;
	GK_BOOL abDpcEn[4];
	GK_BOOL abGeEn;
	GK_BOOL bLscEn;
	GK_BOOL bGammaEn;
	GK_BOOL bCscEn;
	GK_BOOL bCaEn;
	GK_BOOL bWdrEn;
	GK_BOOL bDrcEn;
	GK_BOOL bDehazeEn;
	GK_BOOL bBnrEn;
	GK_U8 bBnrLscEn;
	GK_BOOL bDgEn;
	GK_BOOL b4DgEn;
	GK_BOOL bPregammaEn;
	GK_BOOL bClutEn;
	GK_BOOL bWdrSplitEn;
	GK_BOOL bExpanderEn;
	GK_BOOL bRgbirEn;
	ISP_DEHAZE_LUT2STT_SYNC_REG_CFG_S stDehazeSyncCfg;
	ISP_LSC_LUT2STT_SYNC_REG_CFG_S stLscSyncCfg;
	ISP_BNR_LUT2STT_SYNC_REG_CFG_S stBnrSyncCfg;
	ISP_LDCI_LUT2STT_SYNC_REG_CFG_S stLdciSyncCfg;
	ISP_SHARPEN_LUT2STT_SYNC_REG_CFG_S stSharpenSyncCfg;
	ISP_CA_LUT2STT_SYNC_REG_CFG_S stCaSyncCfg;
} ISP_LUT2STT_SYNC_REG_CFG_S;

typedef union ISP_REG_KERNEL_CFG_KEY_U {
	GK_U32 u32Key;
	struct {
		GK_U32 bit1AwbCfg : 1; /* [0] */
		GK_U32 bit1BeAfCfg : 1; /* [1] */
		GK_U32 bit30Rsv : 30; /* [2:31] */
	};
} ISP_REG_KERNEL_CFG_KEY_U;

typedef struct ISP_AWB_ZONE_CFG_S {
	GK_U32 u32Colum;
	GK_U32 u32Row;
	GK_U32 u32ZoneBin;
} AWB_ZONE_CFG_S;

typedef struct ISP_AF_ZONE_CFG_S {
	GK_U32 u32Colum;
	GK_U32 u32Row;
} AF_ZONE_CFG_S;

typedef struct ISP_REG_KERNEL_CFG_S {
	AWB_ZONE_CFG_S stAWBZoneCfg;
	AF_ZONE_CFG_S stBeAFZoneCfg;
} ISP_REG_KERNEL_CFG_S;

typedef struct ISP_KERNEL_REG_CFG_S {
	ISP_REG_KERNEL_CFG_KEY_U unKey;
	ISP_REG_KERNEL_CFG_S astAlgKernelCfg[ISP_STRIPING_MAX_NUM];
} ISP_KERNEL_REG_CFG_S;

typedef union ISP_REG_CFG_KEY_U {
	GK_U64 u64Key;
	struct {
		GK_U64 bit1AeCfg1 : 1; /* [0] */
		GK_U64 bit1AeCfg2 : 1; /* [1] */
		GK_U64 bit1AwbDynCfg : 1; /* [2] */
		GK_U64 bit1AwbCfg : 1; /* [3] */
		GK_U64 bit1AfFeCfg : 1; /* [4] */
		GK_U64 bit1DrcCfg : 1; /* [5] */
		GK_U64 bit1DpCfg : 1; /* [6] */
		GK_U64 bit1SharpenCfg : 1; /* [7] */
		GK_U64 bit1GeCfg : 1; /* [8] */
		GK_U64 bit1WdrCfg : 1; /* [9] */
		GK_U64 bit1DehazeCfg : 1; /* [10] */
		GK_U64 bit1AwbCfg3 : 1; /* [11] */
		GK_U64 bit1LscCfg : 1; /* [12] */
		GK_U64 bit1DemCfg : 1; /* [13] */
		GK_U64 bit1FcrCfg : 1; /* [14] */
		GK_U64 bit1McdsCfg : 1; /* [15] */
		GK_U64 bit1RcCfg : 1; /* [16] */
		GK_U64 bit1FsWdrCfg : 1; /* [17] */
		GK_U64 bit1BayernrCfg : 1; /* [18] */
		GK_U64 bit1FlickerCfg : 1; /* [19] */
		GK_U64 bit1CaCfg : 1; /* [20] */
		GK_U64 bit1NddmCfg : 1; /* [21] */
		GK_U64 bit1LocalCacCfg : 1; /* [22] */
		GK_U64 bit1GlobalCacCfg : 1; /* [23] */
		GK_U64 bit1DciCfg : 1; /* [24] */
		GK_U64 bit1LdciCfg : 1; /* [25] */
		GK_U64 bit1CscCfg : 1; /* [26] */
		GK_U64 bit1GammaCfg : 1; /* [27] */
		GK_U64 bit1PreGammaCfg : 1; /* [28] */
		GK_U64 bit1SplitCfg : 1; /* [29] */
		GK_U64 bit1EdgeMarkCfg : 1; /* [30] */
		GK_U64 bit1HlcCfg : 1; /* [31] */
		GK_U64 bit1HrsCfg : 1; /* [32] */
		GK_U64 bit1DgCfg : 1; /* [33] */
		GK_U64 bit1WDRDgCfg : 1; /* [34] */
		GK_U64 bit1FeDgCfg : 1; /* [35] */
		GK_U64 bit1AfBeCfg : 1; /* [36] */
		GK_U64 bit1FeBlcCfg : 1; /* [37] */
		GK_U64 bit1BeBlcCfg : 1; /* [38] */
		GK_U64 bit1ClutCfg : 1; /* [39] */
		GK_U64 bit1RLscCfg : 1; /* [40] */
		GK_U64 bit1FpnCfg : 1; /* [41] */
		GK_U64 bit1DetailCfg : 1; /* [42] */
		GK_U64 bit1ExpanderCfg : 1; /* [43] */
		GK_U64 bit1RgbirCfg : 1; /* [44] */
		GK_U64 bit19Rsv : 19; /* [45:63] */
	};
} ISP_REG_CFG_KEY_U;

typedef struct ISP_ALG_REG_CFG_S {
	ISP_AE_REG_CFG_1_S stAeRegCfg1;
	ISP_AE_REG_CFG_2_S stAeRegCfg2;
	ISP_AE_REG_CFG_S stAeRegCfg;
	ISP_MG_REG_CFG_S stMgRegCfg;
	ISP_DG_REG_CFG_S stDgRegCfg;
	ISP_4DG_REG_CFG_S st4DgRegCfg;
	ISP_FE_DG_REG_CFG_S stFeDgRegCfg;
	ISP_AWB_REG_CFG_S stAwbRegCfg;

	ISP_AF_REG_CFG_S stBEAfRegCfg;
	ISP_DPC_REG_CFG_S stDpRegCfg;
	ISP_DRC_REG_CFG_S stDrcRegCfg;
	ISP_PREGAMMA_REG_CFG_S stPreGammaCfg;
	ISP_GE_REG_CFG_S stGeRegCfg;
	ISP_SHARPEN_REG_CFG_S stSharpenRegCfg;
	ISP_WDR_REG_CFG_S stWdrRegCfg;
	ISP_DEHAZE_REG_CFG_S stDehazeRegCfg;
	ISP_MCDS_REG_CFG_S stMcdsRegCfg;
	ISP_LSC_REG_CFG_S stLscRegCfg;
	ISP_DEMOSAIC_REG_CFG_S stDemRegCfg;
	ISP_ANTIFALSECOLOR_REG_CFG_S stAntiFalseColorRegCfg;
	ISP_BAYERNR_REG_CFG_S stBnrRegCfg;
	ISP_EXPANDER_REG_CFG_S stExpanderCfg;
	ISP_CA_REG_CFG_S stCaRegCfg;
	ISP_LCAC_REG_CFG_S stLCacRegCfg;
	ISP_LDCI_REG_CFG_S stLdciRegCfg;
	ISP_FPN_REG_CFG_S stFpnRegCfg;
	isp_be_blc_cfg stBeBlcCfg;
	isp_fe_blc_cfg stFeBlcCfg;
	ISP_CSC_REG_CFG_S stCscCfg;
	ISP_GAMMA_REG_CFG_S stGammaCfg;
	ISP_RGBIR_REG_CFG_S stRgbirCfg;
	ISP_RADIAL_CROP_REG_CFG_S stRcRegCfg;
	ISP_FE_LUT_UPDATE_REG_CFG_S stFeLutUpdateCfg;
	ISP_BE_LUT_UPDATE_REG_CFG_S stBeLutUpdateCfg;
	ISP_LUT2STT_SYNC_REG_CFG_S
	astLut2SttSyncCfg[ISP_LUT2STT_SYNC_BUF_MAX_NUM];
	ISP_BE_STT2LUT_REGNEW_REG_CFG_S stStt2LutRegnewCfg;
} ISP_ALG_REG_CFG_S;

typedef struct ISP_REG_CFG_S {
	GK_U8 u8CfgNum;
	ISP_REG_CFG_KEY_U unKey;
	ISP_ALG_REG_CFG_S stAlgRegCfg[ISP_STRIPING_MAX_NUM];
	ISP_KERNEL_REG_CFG_S stKernelRegCfg;
} ISP_REG_CFG_S;

typedef struct ISP_BE_WO_REG_CFG_S {
	S_ISPBE_REGS_TYPE stBeRegCfg[ISP_STRIPING_MAX_NUM];
} ISP_BE_WO_REG_CFG_S;

typedef struct {
	S_ISP_ONLINE_STT_REGS_TYPE stBeOnlineSttReg[ISP_MAX_BE_NUM];
} ISP_BE_RW_ONLINE_STT_REG_S;

typedef struct ISP_RO_REG_CFG_S {
	ISP_DPC_REG_CFG_S stDpRegCfg;
} ISP_RO_REG_CFG_S;

typedef struct ISP_BE_WO_CFG_BUF_S {
	GK_U64 u64PhyAddr;
	GK_U64 u64Size;
	GK_VOID ATTRIBUTE *pVirAddr;
} ISP_BE_WO_CFG_BUF_S;

typedef struct ISP_BE_CFG_BUF_INFO_S {
	GK_BOOL bInit;
	GK_S32 s32UseCnt;
	ISP_BE_WO_CFG_BUF_S stBeBufHaddr;
} ISP_BE_CFG_BUF_INFO_S;

/* offline mode isp sync para */
#define SYNC_DRC_PRELUMA_NUM 8
#define SYNC_WDR_EXP_VAL_NUM 2
typedef struct ISP_BE_SYNC_PARA_S {
	GK_U32 u32LdciComp;
	GK_U8 u8DrcShpLog;
	GK_U32 u32DrcDivDenomLog;
	GK_U32 u32DrcDenomExp;
	GK_U32 au32DrcPrevLuma[SYNC_DRC_PRELUMA_NUM];
	GK_U32 au32IspDgain[ISP_BAYER_CHN_NUM];
	GK_U32 au32WDRGain[SYNC_WDR_EXP_VAL_NUM];
	GK_U32 u32WdrExpRatio;
	GK_U32 u32FlickExpRatio;
	GK_U32 au32WdrExpVal[SYNC_WDR_EXP_VAL_NUM];
	GK_U32 u32WdrBlcComp;
	GK_BOOL bWDRMdtEn;
	GK_BOOL bFusionMode;
	GK_U16 u16ShortThr;
	GK_U16 u16LongThr;
	GK_U16 u16WgtSlope;
	GK_U32 u32WdrMaxRatio;
	GK_U16 au16CCM[CCM_MATRIX_SIZE];
	GK_U64 au64LdciWriteSttAddr[ISP_STRIPING_MAX_NUM];
	GK_U64 au64LdciReadSttAddr[ISP_STRIPING_MAX_NUM];
	isp_be_blc_dyna_cfg be_blc;
} ISP_BE_SYNC_PARA_S;

typedef struct ISP_BE_REGS_ATTR_S {
	GK_U8 u8BlockNum;
	GK_U8 u8BlockDev;
} ISP_BE_REGS_ATTR_S;

typedef struct ISP_MMZ_BUF_EX_S {
	GK_U64 u64PhyAddr;
	GK_U64 u64Size;
	GK_VOID ATTRIBUTE *pVirAddr;
} ISP_MMZ_BUF_EX_S;

typedef struct ISP_BE_LUT2STT_BUF_S {
	ISP_MMZ_BUF_EX_S astLutSttBuf[2]; /* ping pong be lut stt info */
} ISP_BE_LUT2STT_BUF_S;

typedef struct ISP_BE_LUT2STT_ATTR_S {
	GK_BOOL bInit;
	ISP_BE_LUT2STT_BUF_S astBeLutSttBuf[ISP_MAX_BE_NUM];
} ISP_BE_LUT2STT_ATTR_S;

typedef struct ISP_BE_ONLINE_STT_INFO_S {
	GK_BOOL bInit;
	GK_BOOL bDrvGetFirtStatInfoFlag; /*frame 2 read online stt buffer 0 info */
	GK_U32 u32CurWriteFlag;
	ISP_MMZ_BUF_EX_S astBeSttBuf[2]; /* ping pong be online stt info */
} ISP_BE_ONLINE_STT_INFO_S;

typedef struct ISP_FE_STT_INFO_S {
	GK_BOOL bFirstFrame;
	GK_U32 u32CurWriteFlag;
	ISP_MMZ_BUF_EX_S astFeSttBuf[2]; /* ping pong fe stt info */
} ISP_FE_STT_INFO_S;

typedef struct ISP_FE_STT_ATTR_S {
	GK_BOOL bInit;
	ISP_FE_STT_INFO_S stFeSttBuf[ISP_WDR_CHN_MAX];
} ISP_FE_STT_ATTR_S;

typedef struct ISP_FE_STIT_STT_ATTR_S {
	GK_BOOL bInit;
	S_ISP_FE_STT_REGS_TYPE
	*pstSaveSttStat[ISP_MAX_PIPE_NUM][ISP_WDR_CHN_MAX];
	ISP_FE_STT_INFO_S stFeSttBuf[ISP_MAX_PIPE_NUM][ISP_WDR_CHN_MAX];
} ISP_FE_STIT_STT_ATTR_S;
typedef struct ISP_BE_OFF_STT_ATTR_S {
	GK_BOOL bInit;
	ISP_MMZ_BUF_EX_S astBeSttBuf[ISP_STRIPING_MAX_NUM];
} ISP_BE_OFF_STT_ATTR_S;

typedef struct ISP_BE_STIT_STT_ATTR_S {
	GK_BOOL bInit;
	ISP_MMZ_BUF_EX_S astBeStitchBuf[ISP_MAX_PIPE_NUM];
} ISP_BE_STIT_STT_ATTR_S;

typedef struct ISP_LDCI_BUF_S {
	GK_BOOL bInit;
	GK_U8 u8BufIdx;
	GK_U8 u8BufNum;
	ISP_MMZ_BUF_EX_S astLdciBuf[MAX_ISP_BE_BUF_NUM];
} ISP_LDCI_BUF_S;

typedef struct ISP_LDCI_STT_ADDR_S {
	GK_U64 au64CurWriteSttAddr[ISP_STRIPING_MAX_NUM];
	GK_U64 au64ReadSttAddr[ISP_STRIPING_MAX_NUM];
	ISP_LDCI_STAT_S stLdciStat;
} ISP_LDCI_STT_ADDR_S;

typedef struct ISP_LDCI_READ_STT_BUF_S {
	GK_U8 u8BufNum;
	GK_U8 u8BufIdx;
	GK_U64 u64HeadPhyAddr;
	ISP_MMZ_BUF_EX_S astReadBuf[MAX_ISP_BE_BUF_NUM];
} ISP_LDCI_READ_STT_BUF_S;

typedef struct ISP_CLUT_BUF_S {
	GK_BOOL bInit;
	ISP_MMZ_BUF_EX_S stClutBuf;
} ISP_CLUT_BUF_S;

typedef struct ISP_SPEC_AWB_BUF_S {
	GK_BOOL bInit;
	ISP_MMZ_BUF_EX_S stSpecAwbBuf;
} ISP_SPEC_AWB_BUF_S;

#define ISP_SYNC_BUF_NODE_NUM 5
#define CFG2VLD_DLY_LIMIT 6
#define WDR_EXP_RATIO_SHIFT 6
#define DRC_COMP_SHIFT 12

#define ISP_MAX_UPDATEINFO_BUF_NUM (CFG2VLD_DLY_LIMIT)
#define ISP_MAX_FRAMEINFO_BUF_NUM (CFG2VLD_DLY_LIMIT)
#define ISP_SAVEINFO_MAX (CFG2VLD_DLY_LIMIT)

typedef struct ISP_SYNC_CFG_BUF_NODE_S {
	GK_BOOL bValid;

	ISP_SNS_REGS_INFO_S stSnsRegsInfo;
	ISP_AE_REG_CFG_2_S stAERegCfg;
	ISP_FSWDR_SYNC_CFG_S stWDRRegCfg;
	ISP_DRC_REG_CFG_2_S stDRCRegCfg;
	ISP_AWB_REG_CFG_2_S stAWBRegCfg;
	isp_fe_blc_dyna_cfg fe_blc_reg_cfg;
	isp_be_blc_dyna_cfg be_blc_reg_cfg;
} ISP_SYNC_CFG_BUF_NODE_S;

typedef struct ISP_REGCFG_S {
	GK_BOOL bInit;
	ISP_REG_CFG_S stRegCfg;
	ISP_SYNC_CFG_BUF_NODE_S stSyncCfgNode;
} ISP_REGCFG_S;

typedef struct ISP_CHN_SWITCH_S {
	GK_U32 u32ChannelSel;
	GK_U32 au32WdrChnSel[5];
} ISP_CHN_SWITCH_S;

typedef struct ISP_WDR_CFG_S {
	GK_U8 u8WDRMode;
	GK_U32 au32ExpRatio[EXP_RATIO_NUM];
	ISP_FSWDR_SYNC_CFG_S stWdrRegCfg;
} ISP_WDR_CFG_S;

typedef struct ISP_RES_SWITCH_MODE_S {
	GK_U16 u16Width;
	GK_U16 u16Height;
	GK_FLOAT f32Fps;
	GK_U8 u8SnsMode;
} ISP_RES_SWITCH_MODE_S;

typedef struct ISP_PROC_MEM_S {
	GK_BOOL bInit;
	GK_U32 u32ProcSize;
	GK_U64 u64ProcPhyAddr;
	GK_VOID *pProcVirtAddr;
} ISP_PROC_MEM_S;

typedef struct ISP_MMZ_BUF_S {
	GK_U64 u64PhyAddr;
	GK_VOID *pVirAddr;
} ISP_MMZ_BUF_S;

typedef struct ISP_TRANS_INFO_BUF_S {
	GK_BOOL bInit;
	ISP_MMZ_BUF_S stDngInfo;
	ISP_MMZ_BUF_S stAttaInfo;
	ISP_MMZ_BUF_S stColorGammutInfo;
	ISP_MMZ_BUF_S stFrameInfo;
	ISP_MMZ_BUF_S stUpdateInfo;
} ISP_TRANS_INFO_BUF_S;

typedef struct ISP_PRO_INFO_BUF_S {
	GK_BOOL bInit;
	ISP_MMZ_BUF_S stProNrParamInfo;
	ISP_MMZ_BUF_S stProShpParamInfo;
} ISP_PRO_INFO_BUF_S;

typedef struct ISP_PRO_PARAM_S {
	OPERATION_MODE_E enOperationMode;
	GK_U8 u8ProFrameNum;
	SNAP_PRO_AUTO_PARAM_S stAutoParam;
	SNAP_PRO_MANUAL_PARAM_S stManualParam;
} ISP_PRO_PARAM_S;

/*
0 = default mode:no snap
1 = only preview
2 = only picture
3 = preview and picture is the same pipe
*/
typedef enum ISP_SNAP_PIPE_MODE_E {
	ISP_SNAP_NONE = 0,
	ISP_SNAP_PREVIEW,
	ISP_SNAP_PICTURE,
	ISP_SNAP_PREVIEW_PICTURE,
	ISP_SNAP_BUTT
} ISP_SNAP_PIPE_MODE_E;

typedef struct ISP_SNAP_ATTR_S {
	SNAP_TYPE_E enSnapType;
	GK_S32 s32PreviewPipeId; /* -1,none snap */
	GK_S32 s32PicturePipeId; /* -1,none snap */
	ISP_PRO_PARAM_S stProParam;
	GK_BOOL bLoadCCM; /* GK_TRUE:use CCM of SnapIspInfo, GK_FALSE: Algorithm calculate */
} ISP_SNAP_ATTR_S;

typedef struct ISP_PRO_NR_PARAM_S {
	GK_BOOL bEnable;
	GK_U32 u32ParamNum;
	ISP_NR_AUTO_ATTR_S astNrAttr[PRO_MAX_FRAME_NUM];
} ISP_PRO_NR_PARAM_S;
typedef struct ISP_PRO_SHP_PARAM_S {
	GK_BOOL bEnable;
	GK_U32 u32ParamNum;
	ISP_SHARPEN_AUTO_ATTR_S astShpAttr[PRO_MAX_FRAME_NUM];
} ISP_PRO_SHP_PARAM_S;
#define ISP_1ST_INT 0x1
#define ISP_2ND_INT (1 << 4)
#define ISP_UNDEF_INT 0xF4
#define ISP_BE_FSTART (1 << 16)
#define VIPROC_FRAME_START_INT (1 << 10)

#define ISP_DEV_SET_FD _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, GK_S32)
#define ISP_GET_FRAME_EDGE _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, GK_U32)
#define ISP_GET_VD_TIMEOUT \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_GET_VD_END_TIMEOUT \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_END_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_GET_VD_BEEND_TIMEOUT \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_BEEND_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_SET_INT_ENABLE \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, GK_BOOL)
#define ISP_GET_VERSION \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, GK_U64)
#define ISP_STAT_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, ISP_STAT_INFO_S)
#define ISP_STAT_BUF_PUT \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, ISP_STAT_INFO_S)
#define ISP_STAT_ACT_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_ACT_GET, ISP_STAT_INFO_S)
#define ISP_REG_CFG_INIT \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, ISP_REG_CFG_S)
#define ISP_REG_CFG_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, ISP_KERNEL_REG_CFG_S)

#define ISP_BE_CFG_BUF_INIT \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_INIT, GK_U64)
#define ISP_GET_BE_BUF_FIRST \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_BE_BUF_FIRST, GK_U64)
#define ISP_BE_FREE_BUF_GET \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_FREE_BUF_GET, ISP_BE_WO_CFG_BUF_S)
#define ISP_BE_LAST_BUF_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_LAST_BUF_GET, GK_U64)
#define ISP_BE_CFG_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_EXIT)
#define ISP_BE_CFG_BUF_CTL \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_CTL, ISP_BE_WO_CFG_BUF_S)
#define ISP_BE_CFG_BUF_RUNNING _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_RUNNING)
#define ISP_BE_All_BUF_INIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_All_BUF_INIT)

#define ISP_MEM_INFO_SET _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, GK_BOOL)
#define ISP_MEM_INFO_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, GK_BOOL)
#define ISP_P2EN_INFO_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_P2EN_INFO_GET, GK_BOOL)
#define ISP_INIT_INFO_SET _IOW(IOC_TYPE_ISP, IOC_NR_ISP_INIT_INFO_SET, GK_BOOL)
#define ISP_SYNC_INIT_SET _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_INIT_SET, GK_BOOL)
#define ISP_RUN_STATE_SET _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RUN_STATE_SET, GK_U64)
#define ISP_RESET_CTX _IO(IOC_TYPE_ISP, IOC_NR_ISP_RESET_CTX)
#define ISP_CONFIG_INFO_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_CONFIG_INFO_SET, ISP_CONFIG_INFO_S)
#define ISP_SNAP_INFO_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_INFO_GET, ISP_SNAP_INFO_S)
#define ISP_PRO_TRIGGER_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_EN_GET, GK_BOOL)
#define ISP_UPDATE_POS_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_POS_GET, GK_U32)
#define ISP_FRAME_CNT_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_CNT_GET, GK_U32)
#define ISP_SNAP_ATTR_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_ATTR_GET, ISP_SNAP_ATTR_S)

#define ISP_PROC_INIT _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, ISP_PROC_MEM_S)
#define ISP_PROC_WRITE_ING _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, GK_U32)
#define ISP_TRANS_BUF_INIT \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_INIT, ISP_TRANS_INFO_BUF_S)
#define ISP_TRANS_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_EXIT)
#define ISP_PRO_BUF_INIT \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_BUF_INIT, ISP_PRO_INFO_BUF_S)
#define ISP_PRO_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_PRO_BUF_EXIT)

#define ISP_UPDATE_INFO_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_INFO_SET, ISP_DCF_UPDATE_INFO_S)
#define ISP_FRAME_INFO_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_SET, ISP_FRAME_INFO_S)
#define ISP_FRAME_INFO_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_GET, ISP_FRAME_INFO_S)
#define ISP_DNG_INFO_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_SET, DNG_IMAGE_DYNAMIC_INFO_S)
#define ISP_DNG_INFO_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_GET, DNG_IMAGE_STATIC_INFO_S)
#define ISP_SYNC_CFG_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, ISP_SYNC_CFG_BUF_NODE_S)
#define ISP_WDR_CFG_SET \
	_IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, ISP_WDR_CFG_S)
#define ISP_BE_SYNC_PARAM_INIT \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_SYNC_PARAM_INIT, ISP_BE_SYNC_PARA_S)
#define ISP_RES_SWITCH_SET _IO(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET)
#define ISP_CHN_SELECT_CFG _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CHN_SELECT_CFG, GK_U32)
#define ISP_BE_SWITCH_FINISH_STATE_SET \
	_IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET)
#define ISP_MODE_SWITCH_SET _IO(IOC_TYPE_ISP, IOC_NR_ISP_MODE_SWITCH_SET)

#define ISP_PWM_NUM_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, GK_U32)
#define ISP_WORK_MODE_INIT \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_INIT, ISP_BLOCK_ATTR_S)
#define ISP_WORK_MODE_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_GET, ISP_WORKING_MODE_S)
#define ISP_WORK_MODE_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_EXIT)
#define ISP_SET_MOD_PARAM \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_MOD_PARAM \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_SET_CTRL_PARAM \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_CTRL_PARAM, ISP_CTRL_PARAM_S)
#define ISP_GET_CTRL_PARAM \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_CTRL_PARAM, ISP_CTRL_PARAM_S)
#define ISP_PRE_BLK_NUM_UPDATE \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_PRE_BLK_NUM_UPDATE, GK_U8)

#define ISP_GET_HDR_ATTR \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_HDR_ATTR, VI_PIPE_HDR_ATTR_S)
#define ISP_GET_WDR_ATTR \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_WDR_ATTR, VI_PIPE_WDR_ATTR_S)
#define ISP_GET_STITCH_ATTR \
	_IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_STITCH_ATTR, VI_STITCH_ATTR_S)
#define ISP_GET_PIPE_SIZE _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_PIPE_SIZE, SIZE_S)
#define ISP_PUB_ATTR_INFO \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_PUB_ATTR_INFO, ISP_PUB_ATTR_S)
#define ISP_KERNEL_RUNONCE _IO(IOC_TYPE_ISP, IOC_NR_ISP_KERNEL_RUNONCE)
#define ISP_OPT_RUNONCE_INFO \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_OPT_RUNONCE_INFO, GK_BOOL)
#define ISP_SET_PROCALCDONE _IO(IOC_TYPE_ISP, IOC_NR_ISP_SET_PROCALCDONE)
#define ISP_YUV_RUNONCE_INFO \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_YUV_RUNONCE_INFO, GK_BOOL)
#define ISP_KERNEL_YUV_RUNONCE _IO(IOC_TYPE_ISP, IOC_NR_ISP_KERNEL_YUV_RUNONCE)
#define ISP_SYNC_STITCH_PARAM_INIT \
	_IO(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_STITCH_PARAM_INIT)
#define ISP_CLUT_BUF_INIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_INIT)
#define ISP_CLUT_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_EXIT)
#define ISP_CLUT_BUF_GET _IOR(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_GET, GK_U64)
#define ISP_LDCI_BUF_INIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_INIT)
#define ISP_LDCI_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_EXIT)
#define ISP_LDCI_READ_STT_BUF_GET                            \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_READ_STT_BUF_GET, \
	     ISP_LDCI_READ_STT_BUF_S)

#define ISP_STT_BUF_INIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_INIT)
#define ISP_STT_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_EXIT)
#define ISP_STT_ADDR_INIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_ADDR_INIT)
#define ISP_BE_LUT_STT_BUF_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_LUT_STT_BUF_GET, GK_U64)

#define ISP_SPECAWB_BUF_INIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_INIT)
#define ISP_SPECAWB_BUF_EXIT _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_EXIT)
#define ISP_SPECAWB_BUF_GET \
	_IOR(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_GET, GK_U64)
#define ISP_SET_RGBIR_FORMAT \
	_IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_RGBIR_FORMAT, ISP_IRBAYER_FORMAT_E)

#define ISP_GET_DEV(f) (*((GK_U32 *)(f)))

#define ISP_CHECK_PIPE(pipe)                                                   \
	do {                                                                   \
		if (((pipe) < 0) || ((pipe) >= ISP_MAX_PIPE_NUM)) {            \
			ISP_TRACE(MODULE_DBG_ERR, "Err isp pipe %d!\n", pipe); \
			return ERR_CODE_ISP_ILLEGAL_PARAM;                     \
		}                                                              \
	} while (0)

#define ISP_CHECK_VIR_PIPE(pipe)                        \
	do {                                            \
		if (((pipe) < ISP_MAX_PIPE_NUM) &&      \
		    ((pipe) >= ISP_MAX_PHY_PIPE_NUM)) { \
			return GK_SUCCESS;              \
		}                                       \
	} while (0)

#define ISP_CHECK_POINTER(ptr)                                        \
	do {                                                          \
		if (ptr == GK_NULL) {                                 \
			ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n"); \
			return ERR_CODE_ISP_NULL_PTR;                 \
		}                                                     \
	} while (0)

#define ISP_CHECK_POINTER_VOID(ptr)                                   \
	do {                                                          \
		if (ptr == GK_NULL) {                                 \
			ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n"); \
			return;                                       \
		}                                                     \
	} while (0)

#define ISP_CHECK_BOOL(bool)                                            \
	do {                                                            \
		if ((GK_TRUE != bool)&&(GK_FALSE != bool)) {            \
			ISP_TRACE(MODULE_DBG_ERR,                       \
				  "Invalid ISP Bool Type %d!\n", bool); \
			return ERR_CODE_ISP_ILLEGAL_PARAM;              \
		}                                                       \
	} while (0)

#define ISP_CHECK_BUF_INIT(pipe, init)                                                 \
	do {                                                                           \
		if ((init) != GK_FALSE) {                                              \
			ISP_TRACE(                                                     \
				MODULE_DBG_ERR,                                        \
				"ISP[%d] buffer is initialized, Please exit ISP!!!\n", \
				pipe);                                                 \
			return GK_FAILURE;                                             \
		}                                                                      \
	} while (0)

#define ISP_CHECK_BUF_EXIT(pipe, init)     \
	do {                               \
		if ((init) != GK_TRUE) {   \
			return GK_SUCCESS; \
		}                          \
	} while (0)

#define ISP_CHECK_BEBUF_INIT(pipe)                                        \
	do {                                                              \
		if (g_astIspDrvCtx[pipe].stBeBufInfo.bInit != GK_TRUE) {  \
			ISP_TRACE(MODULE_DBG_ERR,                         \
				  "ISP[%d] BeBuf doesn't initialized!\n", \
				  pipe);                                  \
			return ERR_CODE_ISP_NOT_INIT;                     \
		}                                                         \
	} while (0)

#define ISP_CHECK_STABUF_INIT(pipe)                                          \
	do {                                                                 \
		if (g_astIspDrvCtx[pipe].stStatisticsBuf.bInit != GK_TRUE) { \
			ISP_TRACE(MODULE_DBG_ERR,                            \
				  "ISP[%d] StaBuf doesn't initialized!\n",   \
				  pipe);                                     \
			return ERR_CODE_ISP_NOT_INIT;                        \
		}                                                            \
	} while (0)

#define ISP_CHECK_TRANBUF_INIT(pipe)                                         \
	do {                                                                 \
		if (g_astIspDrvCtx[pipe].stTransInfo.bInit != GK_TRUE) {     \
			ISP_TRACE(MODULE_DBG_WARN,                           \
				  "ISP[%d] TransBuf doesn't initialized!\n", \
				  pipe);                                     \
			return ERR_CODE_ISP_NOT_INIT;                        \
		}                                                            \
	} while (0)

#define ISP_CHECK_EXIT_STATE(pipe)                                          \
	do {                                                                \
		if (g_astIspDrvCtx[pipe].bIspRunFlag == GK_TRUE) {          \
			ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] cann't exit!\n", \
				  pipe);                                    \
			return GK_FAILURE;                                  \
		}                                                           \
	} while (0)

#define ISP_CHECK_ONLINE_MODE(pipe)                                           \
	do {                                                                  \
		if ((IS_ONLINE_MODE(g_astIspDrvCtx[pipe]                      \
					    .stWorkMode.enIspRunningMode)) || \
		    (IS_SIDEBYSIDE_MODE(                                      \
			    g_astIspDrvCtx[pipe]                              \
				    .stWorkMode.enIspRunningMode))) {         \
			return GK_SUCCESS;                                    \
		}                                                             \
	} while (0)

#define ISP_DBG(bEn, fmt...)         \
	do {                         \
		if (bEn)             \
			printf(fmt); \
	} while (0)

#define SLAVE_CHECK_DEV(dev)                                                   \
	do {                                                                   \
		if (((dev) < 0) || ((dev) >= CAP_SLAVE_MAX_NUM)) {             \
			ISP_TRACE(MODULE_DBG_ERR, "Err slave dev %d!\n", dev); \
			return ERR_CODE_ISP_ILLEGAL_PARAM;                     \
		}                                                              \
	} while (0)

#define ISP_ALG_CHECK(AlgKey)              \
	do {                               \
		if (AlgKey == 0) {         \
			return GK_SUCCESS; \
		}                          \
	} while (0)

#define ISP_STATISTICS_KEY (0xFFFBBF77)
#define ISP_PIPE_FEAF_MODULE_ENABLE 0, 0, 0, 0
#define ISP_PIPE_FELOGLUT_MODULE_ENABLE 0, 0, 0, 0

#define ISP_CHECK_FEAF_MODULE(dev)                                            \
	do {                                                                  \
		GK_U8 bFeHaveAfMod[] = { ISP_PIPE_FEAF_MODULE_ENABLE };       \
		if (!bFeHaveAfMod[dev]) {                                     \
			ISP_TRACE(MODULE_DBG_ERR, "Invalid Af Fe Pipe %d!\n", \
				  dev);                                       \
			return ERR_CODE_ISP_ILLEGAL_PARAM;                    \
		}                                                             \
	} while (0)

#endif
