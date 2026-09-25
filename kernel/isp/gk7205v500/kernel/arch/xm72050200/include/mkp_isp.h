/*
 * Copyright (c) XMEDIA. All rights reserved.
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

#define IOC_TYPE_ISP                    'I'

#define MAX_ISP_STAT_BUF_NUM            2
#define MAX_ISP_BE_BUF_NUM              4
#define ISP_LUT2STT_SYNC_BUF_MAX_NUM    2

#define ISP_VER_PRIX                    "ISP_V"
#define ISP_VERSION                     ISP_VER_PRIX MK_VERSION(VER_X,VER_Y,VER_Z,VER_P,VER_B) VER_D

#define MAX_MMZ_NAMELEN                 16
#define ISP_INIT_HAND_SIGNAL            0x66778899
#define ISP_EXIT_HAND_SIGNAL            0x99887766


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
    XMEDIA_U32  u32MilliSec;     /* RW, Range: [0x0, 0xFFFFFFFF], the number of the */
    XMEDIA_U32  u32IntStatus;    /* RO, when there is ISP interrupt, then the s32IntStatus is 1 */
} ISP_VD_TIMEOUT_S;

typedef struct ISP_VERSION_S {
    XMEDIA_U8 au8MppVersion[4];
    XMEDIA_U8 au8Date[20];
    XMEDIA_U8 au8Time[20];
    XMEDIA_U32 u32Magic;
} ISP_VERSION_S;

/* the statistics of isp */
typedef struct ISP_DP_STAT_S {
    XMEDIA_U16  au16DefectPixelCount[ISP_STRIPING_MAX_NUM];
    XMEDIA_U32  au32DefectPixelLut[ISP_STRIPING_MAX_NUM][STATIC_DP_COUNT_NORMAL];
} ISP_DP_STAT_S;

#define ISP_FLICKER_G_MEAN_LENGTH       (144)
typedef struct ISP_FLICKER_STAT_S {
    XMEDIA_U32 au32CurDiffGr[ISP_STRIPING_MAX_NUM];                            // s24.0, current Frame Gr Sum
    XMEDIA_U32 au32CurDiffGb[ISP_STRIPING_MAX_NUM];                            // s24.0, current Frame Gb Sum
    XMEDIA_U32 au32CurAbsGr[ISP_STRIPING_MAX_NUM];                             // u23.0
    XMEDIA_U32 au32CurAbsGb[ISP_STRIPING_MAX_NUM];                             // u23.0
    XMEDIA_U32 au32GMean[ISP_STRIPING_MAX_NUM][ISP_FLICKER_G_MEAN_LENGTH];    // u22.0
} ISP_FLICKER_STAT_S;
typedef struct ISP_DEHAZE_STAT_S {
    XMEDIA_U32  au32DehazeMaxStatDout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM];
    XMEDIA_U32  au32DehazeMinDout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM / 2];
} ISP_DEHAZE_STAT_S;

typedef struct ISP_MG_STAT_S {
    XMEDIA_U16  au16ZoneAvg[MG_ZONE_ROW][MG_ZONE_COLUMN][4];
} ISP_MG_STAT_S;

typedef struct ISP_LDCI_STAT_S {
    XMEDIA_U32  au32GlobalMap[LDCI_STAT_WND_Y][LDCI_STAT_WND_X];
} ISP_LDCI_STAT_S;

typedef struct ISP_COMM_STAT_S {
    XMEDIA_U16  au16WhiteBalanceGain[4];
} ISP_COMM_STAT_S;

typedef struct ISP_BE_STAT_S {
    ISP_BE_AE_STAT_1_S  stBEAeStat1;
    ISP_BE_AE_STAT_2_S  stBEAeStat2;
    ISP_BE_AE_STAT_3_S  stBEAeStat3;
    ISP_AWB_STAT_1_S    stAwbStat1;
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_BE_AF_STAT_S    stAfStat;
    ISP_COMM_STAT_S     stCommStat;
    ISP_DEHAZE_STAT_S   stDehazeStat;
    ISP_MG_STAT_S       stMgStat;
} ISP_BE_STAT_S;

typedef struct ISP_STAT_S {
    ISP_FE_AE_STAT_1_S  stFEAeStat1;
    ISP_FE_AE_STAT_2_S  stFEAeStat2;
    ISP_FE_AE_STAT_3_S  stFEAeStat3;
    ISP_BE_AE_STAT_1_S  stBEAeStat1;
    ISP_BE_AE_STAT_2_S  stBEAeStat2;
    ISP_BE_AE_STAT_3_S  stBEAeStat3;
    ISP_AWB_STAT_1_S    stAwbStat1;
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_BE_AF_STAT_S    stBEAfStat;
    ISP_COMM_STAT_S     stCommStat;
    ISP_DEHAZE_STAT_S   stDehazeStat;
    ISP_MG_STAT_S       stMgStat;
    ISP_LDCI_STAT_S     stLdciStat;
    XMEDIA_BOOL             bBEUpdate;
} ISP_STAT_S;

#define DP_STAT_KEY_BIT                 (14)
#define FLICKER_STAT_KEY_BIT            (18)

typedef union ISP_STAT_KEY_U {
    XMEDIA_U64  u64Key;
    struct {
        XMEDIA_U64  bit1FEAeGloStat     : 1 ;   /* [0] */
        XMEDIA_U64  bit1FEAeLocStat     : 1 ;   /* [1] */
        XMEDIA_U64  bit1FEAeStiGloStat  : 1 ;   /* [2] */
        XMEDIA_U64  bit1FEAeStiLocStat  : 1 ;   /* [3] */
        XMEDIA_U64  bit1BEAeGloStat     : 1 ;   /* [4] */
        XMEDIA_U64  bit1BEAeLocStat     : 1 ;   /* [5] */
        XMEDIA_U64  bit1BEAeStiGloStat  : 1 ;   /* [6] */
        XMEDIA_U64  bit1BEAeStiLocStat  : 1 ;   /* [7] */
        XMEDIA_U64  bit1AwbStat1        : 1 ;   /* [8] */
        XMEDIA_U64  bit1AwbStat2        : 1 ;   /* [9] */
        XMEDIA_U64  bit1AwbStat3        : 1 ;   /* [10] */
        XMEDIA_U64  bit1AwbStat4        : 1 ;   /* [11] */
        XMEDIA_U64  bit1FEAfStat        : 1 ;   /* [12] */
        XMEDIA_U64  bit1BEAfStat        : 1 ;   /* [13] */
        XMEDIA_U64  bit1DpStat          : 1 ;   /* [14] */
        XMEDIA_U64  bit1CommStat        : 1 ;   /* [15] */
        XMEDIA_U64  bit1Dehaze          : 1 ;   /* [16] */
        XMEDIA_U64  bit1MgStat          : 1 ;   /* [17] */
        XMEDIA_U64  bit1Flicker         : 1 ;   /* [18] */
        XMEDIA_U64  bit13Rsv            : 13;   /* [19:31] */
        XMEDIA_U64  bit32IsrAccess      : 32;   /* [32:63] */
    };
} ISP_STAT_KEY_U;

typedef struct ISP_STAT_INFO_S {
    ISP_STAT_KEY_U  unKey;
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_VOID *pVirtAddr;                 /* (ISP_STAT_S *) */
} ISP_STAT_INFO_S;


typedef struct ISP_STAT_SHADOW_MEM_S {
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_VOID *pVirtAddr;
    XMEDIA_U32 u32Size;
    XMEDIA_BOOL bUsrAccess;
} ISP_STAT_SHADOW_MEM_S;


/* the register config of isp */
typedef struct ISP_AE_REG_CFG_1_S {
    XMEDIA_U8   au8MeteringHistThresh[4];
    XMEDIA_U8   au8WeightTable[15][17];
} ISP_AE_REG_CFG_1_S;

typedef struct ISP_AE_REG_CFG_2_S {
    XMEDIA_U64  u64Exposure;
    XMEDIA_U64  u64ExposureSF;
    XMEDIA_U32  u32IntTime[4];  /* 0: Shortest shutter(SS), 1: Short shutter(S), 2: long shutter(L), 3: longest shutter(LL) */
    XMEDIA_U32  u32IspDgain;
    XMEDIA_U32  au32WDRGain[4];

    XMEDIA_BOOL bPirisValid;
    XMEDIA_S32  s32PirisPos;

    ISP_FSWDR_MODE_E enFSWDRMode;
} ISP_AE_REG_CFG_2_S;

typedef struct ISP_AE_STATIC_CFG_S {
    XMEDIA_U8 u8FEEnable;
    XMEDIA_U8 u8BEEnable;

    // Overlap Configs
    XMEDIA_U16 u16FECropPosX;
    XMEDIA_U16 u16FECropPosY;
    XMEDIA_U16 u16FECropOutWidth;
    XMEDIA_U16 u16FECropOutHeight;

    XMEDIA_U16 u16BECropPosX;
    XMEDIA_U16 u16BECropPosY;
    XMEDIA_U16 u16BECropOutWidth;
    XMEDIA_U16 u16BECropOutHeight;
} ISP_AE_STATIC_CFG_S;

typedef struct ISP_AE_DYNA_CFG_S {
    // ISP MPI Configs
    XMEDIA_U8 u8FEFourPlaneMode;
    XMEDIA_U8 u8FEHistOffsetX;
    XMEDIA_U8 u8FEHistOffsetY;
    XMEDIA_U8 u8FEHistSkipX;
    XMEDIA_U8 u8FEHistSkipY;

    XMEDIA_U8 u8BEAESel;
    XMEDIA_U8 u8BEFourPlaneMode;
    XMEDIA_U8 u8BEHistOffsetX;
    XMEDIA_U8 u8BEHistOffsetY;
    XMEDIA_U8 u8BEHistSkipX;
    XMEDIA_U8 u8BEHistSkipY;

    // WDR Configs
    XMEDIA_U8 u8FEGammaLimit;
    XMEDIA_U8 u8FEHistGammaMode;
    XMEDIA_U8 u8FEAverGammaMode;
    XMEDIA_U8 u8FEBitMove;

    XMEDIA_U8 u8BEGammaLimit;
    XMEDIA_U8 u8BEHistGammaMode;
    XMEDIA_U8 u8BEAverGammaMode;
    XMEDIA_U8 u8BEBitMove;

    // Weight Table Configs
    XMEDIA_U8 u8BEWightTableUpdate;
    XMEDIA_U8 u8BEWeightTableWidth;
    XMEDIA_U8 u8BEWeightTableHeight;
    XMEDIA_U8 au8BEWeightTable[15][17];

    XMEDIA_U8 u8FEWightTableUpdate;
    XMEDIA_U8 u8FEWeightTableWidth;
    XMEDIA_U8 u8FEWeightTableHeight;
    XMEDIA_U8 au8FEWeightTable[15][17];
} ISP_AE_DYNA_CFG_S;

typedef struct ISP_AE_MODULE_REG_CFG_S {
    ISP_AE_STATIC_CFG_S stStaticRegCfg;
    ISP_AE_DYNA_CFG_S stDynaRegCfg;
} ISP_AE_REG_CFG_S;

typedef struct ISP_MG_STATIC_CFG_S {
    XMEDIA_U8 u8Enable;

    // Overlap Configs
    XMEDIA_U16 u16CropPosX;
    XMEDIA_U16 u16CropPosY;
    XMEDIA_U16 u16CropOutWidth;
    XMEDIA_U16 u16CropOutHeight;
} ISP_MG_STATIC_CFG_S;

typedef struct ISP_MG_DYNA_CFG_S {
    // WDR Configs
    XMEDIA_U8 u8GammaLimit;
    XMEDIA_U8 u8GammaMode;
    XMEDIA_U8 u8BitMove;

    // Zone Configs
    XMEDIA_U8 u8ZoneWidth;
    XMEDIA_U8 u8ZoneHeight;
} ISP_MG_DYNA_CFG_S;

typedef struct ISP_MG_MODULE_REG_CFG_S {
    ISP_MG_STATIC_CFG_S stStaticRegCfg;
    ISP_MG_DYNA_CFG_S stDynaRegCfg;
} ISP_MG_REG_CFG_S;

typedef struct ISP_DG_STATIC_CFG_S {
    XMEDIA_BOOL bResh;
} ISP_DG_STATIC_CFG_S;

typedef struct ISP_DG_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U16  u16GainR;
    XMEDIA_U16  u16GainGR;
    XMEDIA_U16  u16GainGB;
    XMEDIA_U16  u16GainB;
    XMEDIA_U32  u32ClipValue;
} ISP_DG_DYNA_CFG_S;

typedef struct ISP_FE_DG_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U16  au16GainR[4];
    XMEDIA_U16  au16GainGR[4];
    XMEDIA_U16  au16GainGB[4];
    XMEDIA_U16  au16GainB[4];
    XMEDIA_U32  u32ClipValue;
} ISP_FE_DG_DYNA_CFG_S;

typedef struct ISP_DG_MODULE_REG_CFG_S {
    XMEDIA_BOOL bDgEn;
    ISP_DG_STATIC_CFG_S stStaticRegCfg;
    ISP_DG_DYNA_CFG_S   stDynaRegCfg;
} ISP_DG_REG_CFG_S;

typedef struct ISP_FE_DG_MODULE_REG_CFG_S {
    XMEDIA_BOOL bDgEn;
    ISP_DG_STATIC_CFG_S stStaticRegCfg;
    ISP_FE_DG_DYNA_CFG_S   stDynaRegCfg;
} ISP_FE_DG_REG_CFG_S;

typedef struct ISP_4DG_STATIC_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U16 u16GainR0;
    XMEDIA_U16 u16GainGR0;
    XMEDIA_U16 u16GainGB0;
    XMEDIA_U16 u16GainB0;
    XMEDIA_U16 u16GainR1;
    XMEDIA_U16 u16GainGR1;
    XMEDIA_U16 u16GainGB1;
    XMEDIA_U16 u16GainB1;
    XMEDIA_U16 u16GainR2;
    XMEDIA_U16 u16GainGR2;
    XMEDIA_U16 u16GainGB2;
    XMEDIA_U16 u16GainB2;
    XMEDIA_U16 u16GainR3;
    XMEDIA_U16 u16GainGR3;
    XMEDIA_U16 u16GainGB3;
    XMEDIA_U16 u16GainB3;
} ISP_4DG_STATIC_CFG_S;

typedef struct ISP_4DG_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U32 u32ClipValue0;
    XMEDIA_U32 u32ClipValue1;
    XMEDIA_U32 u32ClipValue2;
    XMEDIA_U32 u32ClipValue3;
} ISP_4DG_DYNA_CFG_S;

typedef struct ISP_4DG_MODULE_REG_CFG_S {
    XMEDIA_BOOL bEnable;
    ISP_4DG_STATIC_CFG_S stStaticRegCfg;
    ISP_4DG_DYNA_CFG_S   stDynaRegCfg;
} ISP_4DG_REG_CFG_S;

typedef struct ISP_AWB_REG_CFG_1_S {
    // BE DYNAMIC
    XMEDIA_U32  au32BEWhiteBalanceGain[ISP_BAYER_CHN_NUM];
    XMEDIA_U16  au16BEColorMatrix[9];
    XMEDIA_U16  u16BEMeteringWhiteLevelAwb;
    XMEDIA_U16  u16BEMeteringBlackLevelAwb;
    XMEDIA_U16  u16BEMeteringCrRefMaxAwb;
    XMEDIA_U16  u16BEMeteringCbRefMaxAwb;
    XMEDIA_U16  u16BEMeteringCrRefMinAwb;
    XMEDIA_U16  u16BEMeteringCbRefMinAwb;
    XMEDIA_U8   u8BEWbWorkEn;
    XMEDIA_U8   u8BECcEn;
    XMEDIA_BOOL u16BECcColortoneEn;
    XMEDIA_U16  u16BECcRGain;
    XMEDIA_U16  u16BECcGGain;
    XMEDIA_U16  u16BECcBGain;

    XMEDIA_U32 u32BECropPosX;
    XMEDIA_U32 u32BECropPosY;
    XMEDIA_U32 u32BEWidth;
    XMEDIA_U32 u32BEHeight;
    XMEDIA_U32 u32BECropOutHeight;
    XMEDIA_U32 u32BECropOutWidth;

    // FE DYNAMIC
    XMEDIA_U32 au32FEWhiteBalanceGain[4];
    XMEDIA_U8 u8FEWbWorkEn;
    XMEDIA_U32 au32WDRWBGain[ISP_BAYER_CHN_NUM];
} ISP_AWB_REG_DYN_CFG_S;

typedef struct ISP_AWB_REG_CFG_2_S {
    // BESTATIC
    XMEDIA_BOOL bBEAwbStaCfg;
    XMEDIA_U8   u8BEAwbBitmove;
    XMEDIA_U8   u8BEAwbWorkEn;
    XMEDIA_U8   u8BEWbWorkEn;

    XMEDIA_U32 u32BEAwbStatRaddr;
    XMEDIA_U8  u8BECcColortoneEn;
    XMEDIA_U8  u8BECcEn;

    XMEDIA_U32 u32BETopK;
    XMEDIA_U32 u32BETopB;
    XMEDIA_U32 u32BEBotK;
    XMEDIA_U32 u32BEBotB;

    XMEDIA_U32 u32BECcInDc0;
    XMEDIA_U32 u32BECcInDc1;
    XMEDIA_U32 u32BECcInDc2;
    XMEDIA_U32 u32BECcOutDc0;
    XMEDIA_U32 u32BECcOutDc1;
    XMEDIA_U32 u32BECcOutDc2;
    XMEDIA_U32 u32BEWbClipValue;

    XMEDIA_U16 u16BEAwbOffsetComp;

    // FESTATIC
    XMEDIA_BOOL bFEAwbStaCfg;
    XMEDIA_U8   u8FEWbWorkEn;

    XMEDIA_U32  u32FEClipValue;
} ISP_AWB_REG_STA_CFG_S;

typedef struct ISP_AWB_REG_CFG_3_S {
    // BE USER
    XMEDIA_BOOL bResh;
    XMEDIA_U32 u32UpdateIndex;
    ISP_AWB_SWITCH_E enBEAWBSwitch;
    XMEDIA_U16 u16BEZoneRow;
    XMEDIA_U16 u16BEZoneCol;
    XMEDIA_U16 u16BEZoneBin;
    XMEDIA_U16 u16BEMeteringBinHist0;
    XMEDIA_U16 u16BEMeteringBinHist1;
    XMEDIA_U16 u16BEMeteringBinHist2;
    XMEDIA_U16 u16BEMeteringBinHist3;
} ISP_AWB_REG_USR_CFG_S;

typedef struct ISP_AWB_REG_CFG_S {
    ISP_AWB_REG_DYN_CFG_S        stAwbRegDynCfg;
    ISP_AWB_REG_STA_CFG_S        stAwbRegStaCfg;
    ISP_AWB_REG_USR_CFG_S        stAwbRegUsrCfg;
} ISP_AWB_REG_CFG_S;

typedef struct ISP_AWB_SYNC_CFG_S {
    XMEDIA_U32  au32BEWhiteBalanceGain[ISP_BAYER_CHN_NUM];
    XMEDIA_U32  au32WDRWBGain[ISP_BAYER_CHN_NUM];
    XMEDIA_U16  au16ColorMatrix[CCM_MATRIX_SIZE];
    ISP_AWB_SWITCH_E enBEAWBSwitch;
} ISP_AWB_SYNC_CFG_S;

typedef ISP_AWB_SYNC_CFG_S ISP_AWB_REG_CFG_2_S;

typedef struct ISP_AF_REG_CFG_S {
    XMEDIA_U16  u16ThresholdWrite;
    XMEDIA_U8   u8MetricsShift;
    XMEDIA_U8   u8NpOffset;
    XMEDIA_BOOL bAfEnable;
    XMEDIA_BOOL bIir0Enable0;
    XMEDIA_BOOL bIir0Enable1;
    XMEDIA_BOOL bIir0Enable2;
    XMEDIA_BOOL bIir1Enable0;
    XMEDIA_BOOL bIir1Enable1;
    XMEDIA_BOOL bIir1Enable2;
    ISP_AF_PEAK_MODE_E enPeakMode;
    ISP_AF_SQU_MODE_E enSquMode;
    XMEDIA_BOOL bOffsetEnable;
    XMEDIA_BOOL bCropEnable;
    XMEDIA_BOOL bLpfEnable;
    XMEDIA_BOOL bMeanEnable;
    XMEDIA_BOOL bRawMode;
    XMEDIA_BOOL bIir0DsEnable;
    XMEDIA_BOOL bIir1DsEnable;
    XMEDIA_BOOL bFir0LpfEnable;
    XMEDIA_BOOL bFir1LpfEnable;
    XMEDIA_BOOL bIir0LdgEnable;
    XMEDIA_BOOL bIir1LdgEnable;
    XMEDIA_BOOL bFir0LdgEnable;
    XMEDIA_BOOL bFir1LdgEnable;
    XMEDIA_BOOL bUpdate;

    XMEDIA_U8 u8BayerMode;
    XMEDIA_U8 u8Gamma;
    XMEDIA_U8 u8GainLimit;
    XMEDIA_U8 u8IirPlgGroup0;
    XMEDIA_U8 u8IirPlsGroup0;
    XMEDIA_U8 u8IirPlgGroup1;
    XMEDIA_U8 u8IirPlsGroup1;
    XMEDIA_U8 u8CntShift0H;
    XMEDIA_U8 u8CntShift1H;
    XMEDIA_U8 u8CntShift0V;
    XMEDIA_U8 u8CntShift1V;
    XMEDIA_U8 u8CntShiftY;
    XMEDIA_U8 u8IirDilate0;
    XMEDIA_U8 u8IirDilate1;
    XMEDIA_U8 u8HilighThre;
    XMEDIA_U8 u8AfPosSel;

    XMEDIA_U16 u16MeanThres;
    XMEDIA_U16 u16WindowVnum;
    XMEDIA_U16 u16WindowHnum;
    XMEDIA_U16 u16IirThre0Coring;
    XMEDIA_U16 u16IirThre1Coring;
    XMEDIA_U16 u16IirPeak0Coring;
    XMEDIA_U16 u16IirPeak1Coring;
    XMEDIA_U16 u16FirThre0Coring;
    XMEDIA_U16 u16FirThre1Coring;
    XMEDIA_U16 u16FirPeak0Coring;
    XMEDIA_U16 u16FirPeak1Coring;
    XMEDIA_U16 u16IirSlope0Coring;
    XMEDIA_U16 u16IirSlope1Coring;
    XMEDIA_U16 u16FirSlope0Coring;
    XMEDIA_U16 u16FirSlope1Coring;
    XMEDIA_U16 u16OffsetGr;
    XMEDIA_U16 u16OffsetGb;
    XMEDIA_U16 u16InputVsize;
    XMEDIA_U16 u16InputHsize;
    XMEDIA_U16 u16Iir0ShiftGroup0;
    XMEDIA_U16 u16Iir1ShiftGroup0;
    XMEDIA_U16 u16Iir2ShiftGroup0;
    XMEDIA_U16 u16Iir3ShiftGroup0;
    XMEDIA_U16 u16Iir0ShiftGroup1;
    XMEDIA_U16 u16Iir1ShiftGroup1;
    XMEDIA_U16 u16Iir2ShiftGroup1;
    XMEDIA_U16 u16Iir3ShiftGroup1;
    XMEDIA_U16 u16IirThre0Low;
    XMEDIA_U16 u16IirThre0High;
    XMEDIA_U16 u16IirThre1Low;
    XMEDIA_U16 u16IirThre1High;
    XMEDIA_U16 u16IirGain0Low;
    XMEDIA_U16 u16IirGain0High;
    XMEDIA_U16 u16IirGain1Low;
    XMEDIA_U16 u16IirGain1High;
    XMEDIA_U16 u16IirSlope0Low;
    XMEDIA_U16 u16IirSlope0High;
    XMEDIA_U16 u16IirSlope1Low;
    XMEDIA_U16 u16IirSlope1High;
    XMEDIA_U16 u16FirThre0Low;
    XMEDIA_U16 u16FirThre0High;
    XMEDIA_U16 u16FirThre1Low;
    XMEDIA_U16 u16FirThre1High;
    XMEDIA_U16 u16FirGain0Low;
    XMEDIA_U16 u16FirGain0High;
    XMEDIA_U16 u16FirGain1Low;
    XMEDIA_U16 u16FirGain1High;
    XMEDIA_U16 u16FirSlope0Low;
    XMEDIA_U16 u16FirSlope0High;
    XMEDIA_U16 u16FirSlope1Low;
    XMEDIA_U16 u16FirSlope1High;
    XMEDIA_U16 u16AccShift0H;
    XMEDIA_U16 u16AccShift1H;
    XMEDIA_U16 u16AccShift0V;
    XMEDIA_U16 u16AccShift1V;
    XMEDIA_U16 u16AccShiftY;
    XMEDIA_U16 u16CropPosY;
    XMEDIA_U16 u16CropPosX;
    XMEDIA_U16 u16ShiftCountY;
    XMEDIA_U16 u16CropVsize;
    XMEDIA_U16 u16CropHsize;

    XMEDIA_S16 s16IirGain0Group0;
    XMEDIA_S16 s16IirGain0Group1;
    XMEDIA_S16 s16IirGain1Group0;
    XMEDIA_S16 s16IirGain1Group1;
    XMEDIA_S16 s16IirGain2Group0;
    XMEDIA_S16 s16IirGain2Group1;
    XMEDIA_S16 s16IirGain3Group0;
    XMEDIA_S16 s16IirGain3Group1;
    XMEDIA_S16 s16IirGain4Group0;
    XMEDIA_S16 s16IirGain4Group1;
    XMEDIA_S16 s16IirGain5Group0;
    XMEDIA_S16 s16IirGain5Group1;
    XMEDIA_S16 s16IirGain6Group0;
    XMEDIA_S16 s16IirGain6Group1;
    XMEDIA_S16 s16FirHGain0Group0;
    XMEDIA_S16 s16FirHGain0Group1;
    XMEDIA_S16 s16FirHGain1Group0;
    XMEDIA_S16 s16FirHGain1Group1;
    XMEDIA_S16 s16FirHGain2Group0;
    XMEDIA_S16 s16FirHGain2Group1;
    XMEDIA_S16 s16FirHGain3Group0;
    XMEDIA_S16 s16FirHGain3Group1;
    XMEDIA_S16 s16FirHGain4Group0;
    XMEDIA_S16 s16FirHGain4Group1;

    XMEDIA_U32 u32StatReadAddr;
    XMEDIA_U32 u32StatReadData;
    XMEDIA_U32 u32UpdateIndex;
} ISP_AF_REG_CFG_S;

typedef struct ISP_OFFSET_REG_CFG_S {
    XMEDIA_U16  au16BlackLevelOffset[4];
} ISP_OFFSET_REG_CFG_S;

typedef struct ISP_DRC_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;

    XMEDIA_BOOL bRchEn;
    XMEDIA_BOOL bWchEn;

    XMEDIA_BOOL bRdstatEn; // DRC read statistics enable, u1.0
    XMEDIA_BOOL bWrstatEn; // DRC write statistics enable, u1.0

    XMEDIA_BOOL bMonochromeMode; // Monochrome mode enable; set to 1 if using monochrome sensor
    XMEDIA_BOOL bDetailBoostEnable;
    XMEDIA_BOOL bPdwSumEnable;
    XMEDIA_BOOL bWgtBoxTriSel;
    XMEDIA_BOOL bDpDetectEnable;

    XMEDIA_U8   u8OffLineMode;  // Offline Mode, u1.0

    XMEDIA_U8   u8BinNumZ;       // DRC bin num

    XMEDIA_U8   u8BitWidthOut;  // Output bitwidth:12/20 select
    XMEDIA_U8   u8LocalEdgeLmt; // Limit of the difference between BLT filter's input & output, u8.0

    XMEDIA_U8   u8RWgt;         // u4.0
    XMEDIA_U8   u8GWgt;         // u5.0
    XMEDIA_U8   u8BWgt;         // u4.0

    XMEDIA_U8   u8ColorControlMode; // 0 - Linear mode, 1 - Powerlaw mode, u1.0
    XMEDIA_U8   u8ColorControlLUTCtrl;  // 0 - login LUT, 1 -logout LUT, u1.0
    XMEDIA_U16  u16GlobalColorCorr; // not in use now and should be fixed to 1024, u12.0

    XMEDIA_U8   u8PFRRGSlo;    // Purple Fringe Reduce, R/G slope control, u3.0
    XMEDIA_U8   u8PFRRGWid;    // Purple Fringe Reduce, R/G width, u7.0
    XMEDIA_U8   u8PFRRGCtr;    // Purple Fringe Reduce, R/G central point, u8.0

    XMEDIA_U8   u8PFRBGSlo;    // Purple Fringe Reduce, B/G slope control, u3.0
    XMEDIA_U8   u8PFRBGThr;    // Purple Fringe Reduce, B/G Threshold detection, u8.0

    XMEDIA_U8  au8SegIdxBase[ISP_DRC_TM_SEG_NUM];
    XMEDIA_U8  au8SegMaxVal[ISP_DRC_TM_SEG_NUM];

    XMEDIA_U8   u8UpdateMode;
    XMEDIA_U16  u16TimingStat;
    XMEDIA_U8   u8DrcUpdate;

    XMEDIA_BOOL bFirstFrame;
    XMEDIA_U8   u8ShpLog;
    XMEDIA_U8   u8ShpExp;
    XMEDIA_U32  u32DivDenomLog;
    XMEDIA_U32  u32DenomExp;
    XMEDIA_U32  au32PrevLuma[ISP_DRC_EXP_COMP_SAMPLE_NUM];

    /* Regs for DRC STAT */
    XMEDIA_BOOL bDrcsEn; // DRCS enable (DRCS = DRC Statistics module in FE), u1.0
    XMEDIA_BOOL bDrcsVbiFltEn; // DRCS VBI filtering enable, u1.0
    XMEDIA_BOOL bDrcsWrtStatEn; // DRCS write statistics enable, u1.0

} ISP_DRC_STATIC_CFG_S;

typedef struct ISP_DRC_DYNA_CFG_S {
    XMEDIA_BOOL bDynaResh;
    XMEDIA_BOOL bImgSizeChanged;
    XMEDIA_BOOL bIsOfflineRepeatMode;
    XMEDIA_BOOL bVbifltEn; // DRC VBI filtering enable, u1.0
    XMEDIA_BOOL bColorCorrEnable;
    XMEDIA_BOOL bLutUpdate;

    XMEDIA_U16  u16StrengthBias;
    XMEDIA_U16  u16Strength;           // DRC strength
    XMEDIA_U16  u16LutMixCtrl;         // Mixing ctrl for LUT0 & LUT1
    XMEDIA_U16  u16AutoStrengthMax;     // Maximum strength in auto mode
    XMEDIA_U16  u16AutoStrengthMin;     // Minimum strength in auto mode

    XMEDIA_U16 au16ToneMappingValue0[ISP_DRC_TM_NODE_NUM];
    XMEDIA_U16 au16ToneMappingDiff0[ISP_DRC_TM_NODE_NUM];

    XMEDIA_U8   u8PFRHighThr;   // Purple Fringe Reduce, High Threshold, u8.0

    XMEDIA_U16  u16VSize;  // Image Height
    XMEDIA_U16  u16HSize;  // Image Width
    XMEDIA_U8   u8OffLineMode;  // Offline Mode, u1.0

    XMEDIA_U8   u8BlockVNum;    // DRC block configure, Number, Vertical direction, u6.0
    XMEDIA_U8   u8BlockHNum;    // DRC block configure, Number, Horizontal direction, u6.0
    XMEDIA_U8   u8BlockChkY;    // DRC block configure, chk_y = vsize%num, u6.0
    XMEDIA_U8   u8BlockChkX;    // DRC block configure, chk_x = hsize%num, u6.0
    XMEDIA_U16  u16BlockVSize;  // DRC block configure, Size, Vertical direction, u9.0
    XMEDIA_U16  u16BlockHSize;  // DRC block configure, Size, Horizontal direction, u9.0

    XMEDIA_U16  u16DivY0;       // DRC block configure, div_y0 = 32768/vsize, u15.0
    XMEDIA_U16  u16DivX0;       // DRC block configure, div_x0 = 32768/hsize, u15.0
    XMEDIA_U16  u16DivY1;       // DRC block configure, div_y1 = 32768/vsize, u15.0
    XMEDIA_U16  u16DivX1;       // DRC block configure, div_x1 = 32768/hsize, u15.0
    XMEDIA_U8   u8BinScale;     // DRC bin scale (pixel weight), u3.0

    XMEDIA_U8   u8BigXInit; // DRC stripe configuration, initial is_big flag
    XMEDIA_U8   u8IdxXInit; // DRC stripe configuration, initial block index
    XMEDIA_U16  u16CntXInit; // DRC stripe configuration, initial pixel count
    XMEDIA_U16  u16AccXInit; // DRC stripe configuration, initial accumulation
    XMEDIA_U16  u16WgtXInit; // DRC stripe configuration, initial pixel position for calculating pixel weight
    XMEDIA_U16  u16TotalWidth; // DRC stripe configuration, total stripe width
    XMEDIA_U16  u16StatWidth; // DRC stripe configuration, active width for statistics

    XMEDIA_U8   u8DrcsBigXInit;
    XMEDIA_U8   u8DrcsIdxXInit;
    XMEDIA_U16  u16DrcsCntXInit;
    XMEDIA_U16  u16DrcsAccXInit;
    XMEDIA_U16  u16DrcsWgtXInit;
    XMEDIA_U16  u16DrcsTotalWidth;
    XMEDIA_U16  u16DrcsStatWidth;

    XMEDIA_U16  u16DrcsVSize;
    XMEDIA_U16  u16DrcsHSize;

} ISP_DRC_DYNA_CFG_S;

typedef struct ISP_DRC_USR_CFG_S {
    XMEDIA_BOOL bUsrResh;
    XMEDIA_U16  u16Exposure;    // Exposure value, u16.0

    XMEDIA_U8   u8YSFT1;        // Used for dark region luminance gain limitation, u3.0
    XMEDIA_U8   u8YVAL1;        // Used for dark region luminance gain limitation, u7.0
    XMEDIA_U8   u8YSFT2;        // Used for dark region luminance gain limitation, u2.0
    XMEDIA_U8   u8YVAL2;        // Used for dark region luminance gain limitation, u4.0

    XMEDIA_U8   u8CSFT1;        // Used for dark region chroma gain limitation, u3.0
    XMEDIA_U8   u8CVAL1;        // Used for dark region chroma gain limitation, u7.0
    XMEDIA_U8   u8CSFT2;        // Used for dark region chroma gain limitation, u2.0
    XMEDIA_U8   u8CVAL2;        // Used for dark region chroma gain limitation, u4.0

    XMEDIA_U8   u8SFT;          // u3.0
    XMEDIA_U8   u8VAL;          // u7.0

    XMEDIA_U8   u8MixingCoring;    // u8.0
    XMEDIA_U8   u8MixingDarkMin;   // u8.0
    XMEDIA_U8   u8MixingDarkMax;   // u8.0
    XMEDIA_S8   s8MixingDarkSlo;   // u8.0
    XMEDIA_U8   u8MixingDarkThr;   // u8.0

    XMEDIA_U8   u8MixingBrightMin; // u8.0
    XMEDIA_U8   u8MixingBrightMax; // u8.0
    XMEDIA_S8   s8MixingBrightSlo; // u8.0
    XMEDIA_U8   u8MixingBrightThr; // u8.0

    XMEDIA_U8   u8GainClipKnee; // Startup for bright region gain limit, u4.0
    XMEDIA_U8   u8GainClipStep; // Region for bright region gain limit, u4.0

    XMEDIA_U8   u8PFRHighSlo;  // Purple Fringe Reduce, High Slope, u4.0
    XMEDIA_U8   u8PFRLowSlo;   // Purple Fringe Reduce, Low Slope, u4.0
    XMEDIA_U8   u8PFRLowThr;   // Purple Fringe Reduce, Low Slope, u8.0

    XMEDIA_U8   u8GradShift;   // Grad Reverse shift, u3.0
    XMEDIA_U8   u8GradSlope;   // Grad Reverse Slope, u4.0
    XMEDIA_U8   u8GradMax;     // Grad Reverse, max value, u7.0
    XMEDIA_U8   u8GradThr;     // Grad Reverse, Threshold Value, u8.0

    XMEDIA_U8   u8VarSpaCoarse;
    XMEDIA_U8   u8VarSpaMedium;
    XMEDIA_U8   u8VarSpaFine;

    XMEDIA_U8   u8VarRngCoarse;
    XMEDIA_U8   u8VarRngMedium;
    XMEDIA_U8   u8VarRngFine;

    XMEDIA_U8   au8BinMixCoarse[8];
    XMEDIA_U8   au8BinMixMedium[8];

    XMEDIA_U8   u8FltSpaCoarse;
    XMEDIA_U8   u8FltSpaMedium;
    XMEDIA_U8   u8FltSpaFine;
    XMEDIA_U8   u8FltRngCoarse;
    XMEDIA_U8   u8FltRngMedium;
    XMEDIA_U8   u8FltRngFine;

    XMEDIA_U8   u8DisThrCoefHigh;
    XMEDIA_U8   u8DisThrCoefLow;
    XMEDIA_U8   u8DisOffsetCoef;
    XMEDIA_U8   u8FltRngAdaMax;

    XMEDIA_U8   u8FltSpaAdaMax;
    XMEDIA_U8   u8DiffThrLow;
    XMEDIA_U8   u8DiffThrHigh;

    XMEDIA_S8   s8DetailSubFactor;

    XMEDIA_U16  u16PFRSum;         // May not be used in fw;
    XMEDIA_U16  u16CCLUT[ISP_DRC_CC_NODE_NUM];     // Color Correction LUT

    /* New add for HDR mode */
    XMEDIA_U8   u8DetailCoring;
    XMEDIA_U8   u8DetailDarkStep;
    XMEDIA_U8   u8DetailBrightStep;

    XMEDIA_S8   s8DetailDarkSlo;
    XMEDIA_U8   u8DetailDarkThr;
    XMEDIA_U8   u8DetailDarkMax;
    XMEDIA_U8   u8DetailDarkMin;

    XMEDIA_S8   s8DetailBrightSlo;
    XMEDIA_U8   u8DetailBrightThr;
    XMEDIA_U8   u8DetailBrightMax;
    XMEDIA_U8   u8DetailBrightMin;

    XMEDIA_U8   u8DetailDarkCurve[16];
    XMEDIA_U8   u8DetailBrightCurve[16];

    XMEDIA_U8   u8SuppressBrightMax;
    XMEDIA_U8   u8SuppressBrightMin;
    XMEDIA_U8   u8SuppressBrightThr;
    XMEDIA_U8   u8SuppressBrightSlo;
    XMEDIA_U8   u8SuppressDarkMax;
    XMEDIA_U8   u8SuppressDarkMin;
    XMEDIA_U8   u8SuppressDarkThr;
    XMEDIA_U8   u8SuppressDarkSlo;

    XMEDIA_BOOL bDpDetectG2RB;
    XMEDIA_BOOL bDpDetectRB2RB;
    XMEDIA_BOOL bDpDetectReplCtr;
    XMEDIA_U8   u8DpDetectRngRatio;
    XMEDIA_U8   u8DpDetectThrSlo;
    XMEDIA_U16  u16DpDetectThrMin;

    /* Cubic TM Curve Related Parameters */
    XMEDIA_U16  u16CubicThres01;
    XMEDIA_U16  u16CubicThres10;
    XMEDIA_U16  u16CubicThres11;
    XMEDIA_BOOL bCubicCurveSel;

    XMEDIA_U8   u8CubicCoef00AExp;
    XMEDIA_U32  u32CubicCoef00A;
    XMEDIA_U8   u8CubicCoef01BExp;
    XMEDIA_U32  u32CubicCoef01B;
    XMEDIA_U8   u8CubicCoef02CExp;
    XMEDIA_U32  u32CubicCoef02C;
    XMEDIA_U32  u32CubicCoef03D;

    XMEDIA_U8   u8CubicCoef10AExp;
    XMEDIA_U32  u32CubicCoef10A;
    XMEDIA_U8   u8CubicCoef11BExp;
    XMEDIA_U32  u32CubicCoef11B;
    XMEDIA_U8   u8CubicCoef12CExp;
    XMEDIA_U32  u32CubicCoef12C;
    XMEDIA_U32  u32CubicCoef13D;

    XMEDIA_U8   u8CubicCoef20AExp;
    XMEDIA_U32  u32CubicCoef20A;
    XMEDIA_U8   u8CubicCoef21BExp;
    XMEDIA_U32  u32CubicCoef21B;
    XMEDIA_U8   u8CubicCoef22CExp;
    XMEDIA_U32  u32CubicCoef22C;
    XMEDIA_U32  u32CubicCoef23D;

    XMEDIA_U8   u8CubicCoef30AExp;
    XMEDIA_U32  u32CubicCoef30A;
    XMEDIA_U8   u8CubicCoef31BExp;
    XMEDIA_U32  u32CubicCoef31B;
    XMEDIA_U8   u8CubicCoef32CExp;
    XMEDIA_U32  u32CubicCoef32C;
    XMEDIA_U32  u32CubicCoef33D;
    XMEDIA_U32  u32UpdateIndex;
} ISP_DRC_USR_CFG_S;

typedef struct ISP_DRC_SYNC_CFG_S {
    XMEDIA_U8  u8ShpLog;
    XMEDIA_U8  u8ShpExp;
    XMEDIA_BOOL bIsOfflineRepeatMode;
    XMEDIA_S32 as32PrevLumaDelta[ISP_DRC_EXP_COMP_SAMPLE_NUM - 1];
} ISP_DRC_SYNC_CFG_S;

typedef ISP_DRC_SYNC_CFG_S ISP_DRC_REG_CFG_2_S;

typedef struct ISP_DRC_REG_CFG_S {
    XMEDIA_BOOL bDrcEn;      // Enable
    ISP_DRC_STATIC_CFG_S stStaticRegCfg;
    ISP_DRC_DYNA_CFG_S   stDynaRegCfg;
    ISP_DRC_USR_CFG_S    stUsrRegCfg;
    ISP_DRC_SYNC_CFG_S   stSyncRegCfg;
} ISP_DRC_REG_CFG_S;

#define ISP_PREGAMMA_SEG_NUM         (12)
typedef struct ISP_PREGAMMA_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_U16  au16SegMaxVal[ISP_PREGAMMA_SEG_NUM];
    XMEDIA_U8   au8SegIdxBase[ISP_PREGAMMA_SEG_NUM];
} ISP_PREGAMMA_STATIC_CFG_S;

typedef struct ISP_PREGAMMA_DYNA_CFG_S {
    XMEDIA_BOOL bSwitchMode;
    XMEDIA_BOOL bPreGammaLutUpdateEn;
    XMEDIA_U8   u8BufId;
    XMEDIA_U32  u32UpdateIndex;
    XMEDIA_U16  au16PreGammaLUT[PREGAMMA_NODE_NUM];  // u16.0
} ISP_PREGAMMA_DYNA_CFG_S;

typedef struct ISP_PREGAMMA_REG_CFG_S {
    XMEDIA_BOOL bPreGammaEn;
    ISP_PREGAMMA_DYNA_CFG_S   stDynaRegCfg;
    ISP_PREGAMMA_STATIC_CFG_S stStaticRegCfg;
} ISP_PREGAMMA_REG_CFG_S;

typedef struct ISP_DEHAZE_STATIC_CFG_S {

    XMEDIA_BOOL bResh;
    XMEDIA_U8  u8MaxMode;

    XMEDIA_U16 u16DehazeBlthld;
    XMEDIA_U8  u8DehazeNegMode;

    XMEDIA_U16 u16BlockSum;
    XMEDIA_U8   u8Dchnum;
    XMEDIA_U8   u8Dcvnum;

    XMEDIA_U8   u8OfflineMode;
    XMEDIA_U8   u8UpdateMode;
    XMEDIA_U8   u8DehazeUpdate;
    XMEDIA_U16   u16fHblank;
    XMEDIA_U16   u16bHblank;
    XMEDIA_U16   u16fVblank;
    XMEDIA_U16   u16bVblank;

} ISP_DEHAZE_STATIC_CFG_S;

typedef struct ISP_DEHAZE_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8BufId;
    XMEDIA_U8   u8Strength;
    XMEDIA_U16  u16AirR;
    XMEDIA_U16  u16AirG;
    XMEDIA_U16  u16AirB;
    XMEDIA_U8   u8Lut[256];
    XMEDIA_U32  prestat[DEFOG_ZONE_NUM / 2];
    XMEDIA_U32  u32Update;
    XMEDIA_U32  u32LutUpdate;

    XMEDIA_U32  u32Width;
    XMEDIA_U32  u32Height;
    XMEDIA_U16  u16Blockhsize;
    XMEDIA_U16  u16Blockvsize;
    XMEDIA_U32  u32phasex;
    XMEDIA_U32  u32phasey;

    XMEDIA_U32 u32smlMapOffset; /* 20bit */
    XMEDIA_U32 u32StatStartX;
    XMEDIA_U32 u32StatEndX;
    XMEDIA_U8  u8StatnumH;
    XMEDIA_U8  u8StatnumV;

    XMEDIA_U16 u16DehazeThldR;
    XMEDIA_U16 u16DehazeThldG;
    XMEDIA_U16 u16DehazeThldB;


} ISP_DEHAZE_DYNA_CFG_S;

typedef struct ISP_DEHAZE_REG_CFG_S {
    XMEDIA_BOOL   bDehazeEn;
    XMEDIA_BOOL   bLut2SttEn;
    ISP_DEHAZE_DYNA_CFG_S   stDynaRegCfg;
    ISP_DEHAZE_STATIC_CFG_S stStaticRegCfg;
} ISP_DEHAZE_REG_CFG_S;

typedef struct ISP_DPC_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_U8   u8DpccOutputMode;      // U4.0
    XMEDIA_U32  u32DpccBptCtrl;         // U12.0
} ISP_DPC_STATIC_CFG_S;

typedef struct ISP_DPC_USR_STATIC_COR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8BufId;
    XMEDIA_U16  u16DpccBptNumber;
    XMEDIA_U32  u32UpdateIndex;
    XMEDIA_U32  au32DpccBpTable[STATIC_DP_COUNT_NORMAL];
} ISP_DPC_USR_STATIC_COR_CFG_S;

typedef struct ISP_DPC_USR_DYNA_COR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bDpccHardThrEn;
    XMEDIA_S8   s8DpccSupTwinkleThrMax;
    XMEDIA_S8   s8DpccSupTwinkleThrMin;
    XMEDIA_U16  u16DpccRakeRatio;
    XMEDIA_U8   u8DPCPos;               // U1.0
} ISP_DPC_USR_DYNA_COR_CFG_S;

typedef struct ISP_DPC_USR_CFG_S {
    ISP_DPC_USR_DYNA_COR_CFG_S   stUsrDynaCorRegCfg;
    ISP_DPC_USR_STATIC_COR_CFG_S stUsrStaCorRegCfg;
} ISP_DPC_USR_CFG_S;
typedef struct ISP_DPC_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bDpcStatEn;
    XMEDIA_U8   u8DpccSetUse;           // U4.0
    XMEDIA_U8   u8DpccLineKerdiffFac;
    XMEDIA_U8   u8DpccBlendMode;
    XMEDIA_U8   u8DpccBitDepthSel;
    XMEDIA_U16  au16DpccLineStdThr[5]; // u12.0
    XMEDIA_U8   au8DpccLineDiffThr[5];
    XMEDIA_U8   au8DpccLineAverFac[5];
    XMEDIA_U16  u16DpccMode;            // U10.0
    XMEDIA_U16  u16DpccMethodsSet1;     // U13.0
    XMEDIA_U16  u16DpccMethodsSet2;     // U13.0
    XMEDIA_U16  u16DpccMethodsSet3;     // U13.0
    XMEDIA_U16  au16DpccLineThr[3];     // U16.0
    XMEDIA_U16  au16DpccLineMadFac[3];  // U14.0
    XMEDIA_U16  au16DpccPgFac[3];       // U14.0
    XMEDIA_U16  au16DpccRndThr[3];      // U16.0
    XMEDIA_U32  au32DpccRndThrMtp[3];   // U32.0
    XMEDIA_U16  au16DpccRgFac[3];       // U14.0
    XMEDIA_U32  au32DpccRgFacMtp[3];    // U32.0
    XMEDIA_U16  u16DpccRoLimits;        // U12.0
    XMEDIA_U16  u16DpccRndOffs;         // U12.0
    XMEDIA_U32  u32DpccBadThresh;       // U32.0
    XMEDIA_U32  u32DpccAlpha;           // U32.0
} ISP_DPC_DYNA_CFG_S;

typedef struct ISP_DPC_MODULE_REG_CFG_S {
    XMEDIA_BOOL abDpcEn[4];
    XMEDIA_BOOL bLut2SttEn;
    XMEDIA_U8   u8ChnNum;
    ISP_DPC_STATIC_CFG_S    stStaticRegCfg;
    ISP_DPC_DYNA_CFG_S      stDynaRegCfg;
    ISP_DPC_USR_CFG_S       stUsrRegCfg;
} ISP_DPC_REG_CFG_S;

typedef struct ISP_SHARPEN_STATIC_REG_CFG_S {
    XMEDIA_U8    u8ditherMode;
    XMEDIA_BOOL  bStaticResh;
    XMEDIA_U8    u8gainThdSelD;
    XMEDIA_U8    u8dirVarScale;
    XMEDIA_U8    u8dirRly[2];

    XMEDIA_U8    u8MaxVarClipMin; //  8 bit
    XMEDIA_U16   u16oMaxChg;
    XMEDIA_U16   u16uMaxChg;
    XMEDIA_U8    u8shtVarWgt1;
    XMEDIA_U8    u8shtVarDiffWgt0;
    XMEDIA_U8    u8shtVarThd0;
    XMEDIA_U8    u8shtVarSft;
    XMEDIA_U8    u8lmtMF[ISP_SHARPEN_FREQ_CORING_LENGTH];
    XMEDIA_U8    u8lmtHF[ISP_SHARPEN_FREQ_CORING_LENGTH];

    XMEDIA_U8    u8skinSrcSel;
    XMEDIA_U8    u8skinCntThd[2];
    XMEDIA_U8    u8skinEdgeSft;
    XMEDIA_U8    u8skinEdgeThd[2];

    XMEDIA_U8    u8chrRVarSft;
    XMEDIA_U8    u8chrROriCb;    // U8.0
    XMEDIA_U8    u8chrROriCr;    // U8.0
    XMEDIA_U8    u8chrRSft[4];   // U0.3
    XMEDIA_U8    u8chrRThd[2];
    XMEDIA_U8    u8chrGOriCb;    // U8.0
    XMEDIA_U8    u8chrGOriCr;    // U8.0
    XMEDIA_U8    u8chrGSft[4];   // U0.3
    XMEDIA_U8    u8chrGThd[2];   // U8.0
    XMEDIA_U8    u8chrBVarSft;
    XMEDIA_U8    u8chrBOriCb;    // U8.0
    XMEDIA_U8    u8chrBOriCr;    // U8.0
    XMEDIA_U8    u8chrBSft[4];   // U0.3
    XMEDIA_U8    u8chrBThd[2];

    //XMEDIA_U8    u8dirRt[2] ;

    // filter
    XMEDIA_S8    s8lpfCoefUD[3]; // S8.0
    XMEDIA_S8    s8lpfCoefD[3];  // S8.0
    XMEDIA_S8    s8hsfCoefUD[3]; // S8.0
    XMEDIA_S8    s8hsfCoefD0[13]; // S8.0
    XMEDIA_S8    s8hsfCoefD1[13]; // S8.0
    XMEDIA_S8    s8hsfCoefD2[13]; // S8.0
    XMEDIA_S8    s8hsfCoefD3[13]; // S8.0

    XMEDIA_U8    u8lpfSftUD;     // U4.0--[3, 15]
    XMEDIA_U8    u8lpfSftD;      // U4.0--[3, 15]
    XMEDIA_U8    u8hsfSftUD;     // U4.0--[3, 15]
    XMEDIA_U8    u8hsfSftD;      // U4.0--[3, 15]

    XMEDIA_U8    bEnShp8Dir;

    // shoot ctrl
    XMEDIA_U8    u8lfGainWgt;
    XMEDIA_U8    u8hfGainSft;
    XMEDIA_U8    u8mfGainSft;
    XMEDIA_U8    u8shtVarSel;
    XMEDIA_U8    u8shtVar5x5Sft;
    XMEDIA_U8    u8detailThdSel;
    XMEDIA_U8    u8detailThdSft;

    XMEDIA_U8    u8skinCntMul;        // convert 31/(skin_cnt_th2-skin_cnt_th1) to multiplication.        // 5-bit unsigned integer    // U5.0
} ISP_SHARPEN_STATIC_REG_CFG_S;

typedef struct ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S {

    XMEDIA_BOOL  bResh;
    XMEDIA_U8    u8gainThdSftD;
    XMEDIA_U8    u8gainThdSelUD;
    XMEDIA_U8    u8gainThdSftUD;
    XMEDIA_U8    u8dirVarSft;

    XMEDIA_U8    u8dirRlyThrLow;
    XMEDIA_U8    u8dirRlyThrhigh;

    XMEDIA_U8    u8selPixWgt;
    XMEDIA_U8    u8shtVarWgt0;
    XMEDIA_U8    u8shtVarDiffThd[2];
    XMEDIA_U8    u8shtVarDiffWgt1;

    XMEDIA_U8    u8RmfGainScale;
    XMEDIA_U8    u8BmfGainScale;
    XMEDIA_S16   s16shtVarDiffMul;    // Convert division to multiplication.           // 12-bit signed integer   // S8.4

} ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S;

typedef struct ISP_SHARPEN_MPI_DYNA_REG_CFG_S {
    XMEDIA_BOOL  bResh;
    XMEDIA_BOOL  bPreRegNewEn;
    XMEDIA_BOOL  bSwitchMode;
    XMEDIA_U8    u8BufId;
    XMEDIA_U16   u16mfGainD[SHRP_GAIN_LUT_SIZE];
    XMEDIA_U16   u16mfGainUD[SHRP_GAIN_LUT_SIZE];
    XMEDIA_U16   u16hfGainD[SHRP_GAIN_LUT_SIZE];
    XMEDIA_U16   u16hfGainUD[SHRP_GAIN_LUT_SIZE];
    XMEDIA_U8    u8oshtAmt;
    XMEDIA_U8    u8ushtAmt;

    XMEDIA_U8    u8bEnChrCtrl;
    XMEDIA_U8    u8chrRGain;  // U1.5  [0 32]
    XMEDIA_U16   u16chrGGain;  // U1.5  [0 32]
    XMEDIA_U16   u16chrGmfGain;  // U1.5  [0 32]
    XMEDIA_U8    u8chrBGain;   // U1.5    [0 32]
    XMEDIA_U8    u8bEnSkinCtrl;
    XMEDIA_U8    u8skinEdgeWgt[2];  // U0.5    [0 31]

    XMEDIA_U8       bEnWeakDetailAdj;
    XMEDIA_U8    u8WeakDetailGain;

    XMEDIA_U8    u8bEnShtCtrlByVar;
    XMEDIA_U8    u8shtBldRt;
    XMEDIA_U8    u8shtVarThd1;
    XMEDIA_U8    u8bEnLumaCtrl;
    XMEDIA_BOOL  bEnDetailCtrl;
    XMEDIA_U8    u8detailOshtAmt;
    XMEDIA_U8    u8detailUshtAmt;
    XMEDIA_U8    au8LumaWgt[ISP_SHARPEN_LUMA_NUM];
    XMEDIA_U16   u16shtVarMul;        // Convert division to multiplication.           // 11-bit unsigned integer // U7.4
    XMEDIA_S16   s16detailOshtMul;
    XMEDIA_S16   s16detailUshtMul;
    XMEDIA_U8    u8dirDiffSft;
    XMEDIA_U8    u8dirRt[2] ;
    XMEDIA_S16   s16chrRMul;      // 9-bit signed integer     // S5.4
    XMEDIA_S16   s16chrGMul;          // s10.4
    XMEDIA_S16   s16chrGmfMul;        // s10.4
    XMEDIA_S16   s16chrBMul;    // 9-bit signed integer     // S5.4
    XMEDIA_S16   s16skinEdgeMul;       // convert (skin_edge_wgt2-skin_edge_wgt1)/(skin_edge_th2-skin_edge_th1) to multiplication.      // 9-bit signed integer // S5.4
    XMEDIA_U8    u8skinMaxU;
    XMEDIA_U8    u8skinMinU;
    XMEDIA_U8    u8skinMaxV;
    XMEDIA_U8    u8skinMinV;
    XMEDIA_U16   u16oMaxGain;   // 11bit
    XMEDIA_U16   u16uMaxGain;   // 11bit
    XMEDIA_U8    u8detailOshtThr[2];
    XMEDIA_U8    u8detailUshtThr[2];
    XMEDIA_U32   u32UpdateIndex;
} ISP_SHARPEN_MPI_DYNA_REG_CFG_S;

typedef struct ISP_SHARPEN_DYNA_REG_CFG_S {
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S          stMpiDynaRegCfg;
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S      stDefaultDynaRegCfg;
} ISP_SHARPEN_DYNA_REG_CFG_S;

typedef struct ISP_SHARPEN_REG_CFG_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_BOOL  bLut2SttEn;
    ISP_SHARPEN_DYNA_REG_CFG_S          stDynaRegCfg;
    ISP_SHARPEN_STATIC_REG_CFG_S        stStaticRegCfg;
} ISP_SHARPEN_REG_CFG_S;

typedef struct ISP_GE_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_BOOL bGeGrGbEn;          // u1.0
    XMEDIA_BOOL bGeGrEn;            // u1.0
    XMEDIA_BOOL bGeGbEn;            // u1.0
} ISP_GE_STATIC_CFG_S;

typedef struct ISP_GE_USR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   au8GeCtSlope1;   // u4.0    0x9
    XMEDIA_U8   au8GeCtSlope2;   // u4.0    0x9
    XMEDIA_U16  au16GeCtTh2;     // u14.0   0x200
} ISP_GE_USR_CFG_S;

typedef struct ISP_GE_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U16  au16GeCtTh1;     // u14.0   0x400
    XMEDIA_U16  au16GeCtTh3;     // u14.0   0x200
    XMEDIA_U16  u16GeStrength;      // u9.0  0x80
} ISP_GE_DYNA_CFG_S;

typedef struct ISP_EXPANDER_STATIC_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8BitDepthIn;
    XMEDIA_U8   u8BitDepthOut;
    XMEDIA_U16  au16Lut[EXPANDER_NODE_NUM];
} ISP_EXPANDER_STATIC_CFG_S;

typedef struct ISP_EXPANDER_MODULE_REG_CFG_S {
    XMEDIA_BOOL bEnable;
    ISP_EXPANDER_STATIC_CFG_S stStaticCfg;
} ISP_EXPANDER_REG_CFG_S;

typedef struct ISP_GE_MODULE_REG_CFG_S {
    XMEDIA_BOOL abGeEn;
    ISP_GE_STATIC_CFG_S     stStaticRegCfg;
    ISP_GE_DYNA_CFG_S       stDynaRegCfg;
    ISP_GE_USR_CFG_S        stUsrRegCfg;
} ISP_GE_REG_CFG_S;

typedef struct ISP_MCDS_STATIC_REG_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_BOOL bHcdsEn;        // UV Horizontal DS enable; 0: close; 1: open
    XMEDIA_S32  s32HCoef0;
    XMEDIA_S32  s32HCoef1;
    XMEDIA_S32  s32HCoef2;
    XMEDIA_S32  s32HCoef3;
    XMEDIA_S32  s32HCoef4;
    XMEDIA_S32  s32HCoef5;
    XMEDIA_S32  s32HCoef6;
    XMEDIA_S32  s32HCoef7;
    XMEDIA_S32  s32EnFilterV;
} ISP_MCDS_STATIC_REG_CFG_S;

typedef struct ISP_MCDS_DYNA_REG_CFG_S {
    XMEDIA_BOOL bDynaResh;
    XMEDIA_BOOL bVcdsEn;        // UV vertical DS enable; 0: close; 1: open
} ISP_MCDS_DYNA_REG_CFG_S;

typedef struct ISP_MCDS_REG_CFG_S {
    XMEDIA_BOOL bMCDSen;
    ISP_MCDS_STATIC_REG_CFG_S stStaticRegCfg;
    ISP_MCDS_DYNA_REG_CFG_S   stDynaRegCfg;

} ISP_MCDS_REG_CFG_S;
#define  NLUT_LENGTH       129
#define  WLUT_LENGTH       33
#define  WDR_LUT_LENGTH    128
#define  WDR_CHL_CHNLN     5

#define  WDR_FRAME0        0
#define  WDR_FRAME1        1
#define  WDR_FRAME2        2
#define  WDR_FRAME3        3

#define  WDR_CHL_Rg        0
#define  WDR_CHL_Gr        1
#define  WDR_CHL_Gb        2
#define  WDR_CHL_Bg        3
#define  WDR_CHL_MAX       2

typedef struct ISP_FSWDR_STATIC_CFG_S {
    XMEDIA_BOOL bFirstFrame;
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bGrayScaleMode;

    XMEDIA_BOOL bSaveBLC;
    XMEDIA_BOOL bForceLongSmoothEn;                    // u1.0,[0,1]



    XMEDIA_U8   u8bldrLHFIdx;                           // u5.0, [0,16]
    XMEDIA_U8   u8MaskSimilarThr;                       // u6.0, [0,63],
    XMEDIA_U8   u8MaskSimilarCnt;                       // u3.0, [0,7]
    XMEDIA_U16  au16ExpoValue[WDR_CHL_MAX];             // u8.6, [0,16383]
    XMEDIA_U16  flick_exp_ratio;
    XMEDIA_U16  u32MaxRatio;                            // u17.0, [0,131071]
    XMEDIA_U16  u16dftWgtFL;                            // u9.0, [0,256]
    XMEDIA_U16  au16ExpoRRatio[WDR_CHL_MAX - 1];        // u10.0,[0,1023]
    XMEDIA_U32  au32BlcComp[WDR_CHL_MAX - 1];           // s27.0,[0,67108863]

    XMEDIA_U16  u16SaturateThr;             // u14.0,[0,16383]
    XMEDIA_U16  u16FusionSaturateThd;       // u12.0,[0,4095]
} ISP_FSWDR_STATIC_CFG_S;

typedef struct ISP_FSWDR_USR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bFusionMode;                // u1.0,  Fusion mode
    XMEDIA_BOOL bShortExpoChk;              // u1.0, [0,1]
    XMEDIA_U8   u8MdtLBld;                  // u8.0, [0,254],the amount of long-expo is added in motion mask
    XMEDIA_U8   u8BnrFullMdtThr;            // u8.0, [0,254]

    XMEDIA_U8   u8MdtStillThr;              // u8.0, [0,254],less than this threshold, motion mask will be 0; u8MdtStillThr <= u8MdtFullThr
    XMEDIA_U8   u8MdtFullThr;               // u8.0, [0,254],more than this threshold, motion mask will look for the next one
    XMEDIA_U8   u8FullMotSigWgt;            // u5.0, [0,31]
    XMEDIA_U32  u32UpdateIndex;

    XMEDIA_U8   u8ShortSigmaGwgt;          // U4.0  [0,15]
    XMEDIA_U8   u8ShortSigmaCwgt;          // U4.0 [0,15]

    XMEDIA_BOOL bSFNR;                     // u1.0

    XMEDIA_U16 u16PixelAvgMaxDiff; // u11.0,[0,4095]


    XMEDIA_U8    u8ShortSigmaL1GWeight;           // u4.4,[0.255]
    XMEDIA_U8    u8ShortSigmaL2GWeight;           // u4.4,[0.255]
    XMEDIA_U8    u8ShortSigmaL1CWeight;           // u4.4,[0.255]
    XMEDIA_U8    u8ShortSigmaL2CWeight;           // u4.4,[0.255]

    XMEDIA_BOOL   bFusionSmooth;                 // u1.0, [0,1]
    XMEDIA_U32    u32FusionSmoothDiffThd;        // u16.0, [0,65535]



} ISP_FSWDR_USR_CFG_S;

typedef struct ISP_FSWDR_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bWDRMdtEn;                              // u1.0,  motion detection switch
    XMEDIA_U8   u8MdThrLowGain;                         // u4.2, [0,63]
    XMEDIA_U8   u8MdThrHigGain;                         // u4.2, [0,63]
    XMEDIA_U8   u8SqrtAgainG;                           // u3.0, [0,6]
    XMEDIA_U8   u8SqrtDgainG;                           // u3.0, [0,6]
    XMEDIA_U8   u8MdtNosFloor;                          // u7.0, [0,127]
    XMEDIA_U16  u16ShortThr;                            // u14.0,[0,16383],the threshold of beginning to adopt short exposure
    XMEDIA_U16  u16LongThr;                             // u14.0,[0,16383],below the threshold will adopt long exposure  u16LongThr <= u16ShortThr
    XMEDIA_U16  au16StillThr[WDR_CHL_MAX - 1];          // u14.0,[0,16383]


    XMEDIA_U16  u16TNosFloor;                           // u12.0,[0,4095]
    XMEDIA_BOOL bErosionEn;
    XMEDIA_U8  u8TextureThdWgt;                       // u4.4,[0,255]
    XMEDIA_U16  au16FusionThrR[WDR_CHL_MAX];         // u14.0,[0,16383]
    XMEDIA_U16  au16FusionThrG[WDR_CHL_MAX];         // u14.0,[0,16383]
    XMEDIA_U16  au16FusionThrB[WDR_CHL_MAX];         // u14.0,[0,16383]

    XMEDIA_BOOL bForceLong;         // u1.0,[0,1]
    XMEDIA_U16  u16ForceLongLowThr; // u11.0,[0,4095]
    XMEDIA_U16  u16ForceLongHigThr; // u11.0,[0,4095]
    XMEDIA_U16  u16ShortCheckThd; // u11.0,[0,4095]

    XMEDIA_BOOL bFlickEn;
    XMEDIA_BOOL bBcomEn;
    XMEDIA_BOOL bBdecEn;
    XMEDIA_U8   u8bcom_alpha;
    XMEDIA_U8   u8bdec_alpha;
    XMEDIA_U8   u8FrmMerge;
    XMEDIA_BOOL bFusionDataMode;

    XMEDIA_U16      u16MdtNFLowThr;    // u11.0,[0,2047]
    XMEDIA_U16      u16MdtNFHighThr;  // u11.0,[0,2047]
    XMEDIA_U16      u16GainSumLowThr;   // u8.0,[0,255]
    XMEDIA_U16      u16GainSumHighThr; // u8.0,[0,255]

    XMEDIA_U16      u16ForceLongSlope;    // u10.4,[0,16383]
    XMEDIA_U16      u16WgtSlope;         // u8.4,[0,4095]

} ISP_FSWDR_DYNA_CFG_S;

typedef struct ISP_FSWDR_SYNC_CFG_S {
    XMEDIA_BOOL bFusionMode;
    XMEDIA_BOOL bWDRMdtEn;
    XMEDIA_U16  u16ShortThr;
    XMEDIA_U16  u16LongThr;
    XMEDIA_U16  u16Offset0;
    XMEDIA_U16  u16WgtSlope;
} ISP_FSWDR_SYNC_CFG_S;

typedef struct ISP_WDR_REG_CFG_S {
    XMEDIA_BOOL bWDREn;
    ISP_FSWDR_STATIC_CFG_S stStaticRegCfg;
    ISP_FSWDR_DYNA_CFG_S   stDynaRegCfg;
    ISP_FSWDR_USR_CFG_S    stUsrRegCfg;
    ISP_FSWDR_SYNC_CFG_S   stSyncRegCfg;
} ISP_WDR_REG_CFG_S;

typedef struct {
    XMEDIA_BOOL bStaticResh;
} ISP_RC_STATIC_CFG_S;

typedef struct {
    XMEDIA_BOOL bResh;
    XMEDIA_U16  u16CenterVerCoor;
    XMEDIA_U16  u16CenterHorCoor;
    XMEDIA_U32  u32SquareRadius;
} ISP_RC_USR_CFG_S;

typedef struct ISP_RADIAL_CROP_REG_CFG_S {
    XMEDIA_BOOL bRcEn;
    ISP_RC_STATIC_CFG_S stStaticRegCfg;
    ISP_RC_USR_CFG_S    stUsrRegCfg;
} ISP_RADIAL_CROP_REG_CFG_S;

typedef struct ISP_LSC_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_U8   u8WinNumH;
    XMEDIA_U8   u8WinNumV;
} ISP_LSC_STATIC_CFG_S;

typedef struct ISP_LSC_USR_CFG_S {
    XMEDIA_BOOL bCoefUpdate;
    XMEDIA_BOOL bLutUpdate;
    XMEDIA_U8   u8BufId;
    XMEDIA_U8   u8MeshScale;
    XMEDIA_U16  u16MeshStr;
    XMEDIA_U16  u16WidthOffset;
    XMEDIA_U16  au16DeltaX[ISP_LSC_GRID_COL - 1];
    XMEDIA_U16  au16DeltaY[(ISP_LSC_GRID_ROW - 1) / 2];
    XMEDIA_U16  au16InvX[ISP_LSC_GRID_COL - 1];
    XMEDIA_U16  au16InvY[(ISP_LSC_GRID_ROW - 1) / 2];
    XMEDIA_U32  au32RGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U32  au32GrGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U32  au32GbGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U32  au32BGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U32  u32UpdateIndex;
} ISP_LSC_USR_CFG_S;

typedef struct ISP_LSC_MODULE_REG_CFG_S {
    XMEDIA_BOOL bLscEn;
    XMEDIA_BOOL bLut2SttEn;
    ISP_LSC_USR_CFG_S       stUsrRegCfg;
    ISP_LSC_STATIC_CFG_S    stStaticRegCfg;
} ISP_LSC_REG_CFG_S;

typedef struct ISP_RLSC_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;

    XMEDIA_U16  u16NodeNum;

} ISP_RLSC_STATIC_CFG_S;

typedef struct ISP_RLSC_USR_CFG_S {
    XMEDIA_BOOL bUsrResh;
    XMEDIA_BOOL bRadialCropEn;
    XMEDIA_BOOL bRLscFuncEn;
    XMEDIA_BOOL bCoefUpdate;
    XMEDIA_BOOL bLutUpdate;
    XMEDIA_U8   u8BufId;

    XMEDIA_U16  u16WidthOffset;

    XMEDIA_U16  u16CenterRX;
    XMEDIA_U16  u16CenterRY;
    XMEDIA_U16  u16CenterGrX;
    XMEDIA_U16  u16CenterGrY;
    XMEDIA_U16  u16CenterGbX;
    XMEDIA_U16  u16CenterGbY;
    XMEDIA_U16  u16CenterBX;
    XMEDIA_U16  u16CenterBY;

    XMEDIA_U16  u16OffCenterR;
    XMEDIA_U16  u16OffCenterGr;
    XMEDIA_U16  u16OffCenterGb;
    XMEDIA_U16  u16OffCenterB;

    XMEDIA_U32  u32ValidRadius;

    XMEDIA_U16  u16GainStr;
    XMEDIA_U8   u8GainScale;

    XMEDIA_U32  au32Lut0Chn0[ISP_RLSC_POINTS + 1];
    XMEDIA_U32  au32Lut0Chn1[ISP_RLSC_POINTS + 1];
    XMEDIA_U32  au32Lut1Chn2[ISP_RLSC_POINTS + 1];
    XMEDIA_U32  au32Lut1Chn3[ISP_RLSC_POINTS + 1];
    XMEDIA_U32  u32UpdateIndex;

} ISP_RLSC_USR_CFG_S;

typedef struct ISP_RLSC_MODULE_REG_CFG_S {
    XMEDIA_BOOL bRLscEn;
    XMEDIA_BOOL bLut2SttEn;
    ISP_RLSC_USR_CFG_S      stUsrRegCfg;
    ISP_RLSC_STATIC_CFG_S   stStaticRegCfg;
} ISP_RLSC_REG_CFG_S;


typedef struct ISP_LOCAL_CAC_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_BOOL bNddmCacBlendEn;
    XMEDIA_U8   u8RCounterThr;
    XMEDIA_U8   u8GCounterThr;
    XMEDIA_U8   u8BCounterThr;
    XMEDIA_U8   au8DePurpleStr[LCAC_STRENGTH_NUM];
    XMEDIA_U16  u16SatuThr;
    XMEDIA_U16  u16NddmCacBlendRate;
    XMEDIA_BOOL bCacCorMode;
} ISP_LOCAL_CAC_STATIC_CFG_S;

typedef struct ISP_LOCAL_CAC_USR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8LumaHighCntThr;    // u8.0, [0,153]
    XMEDIA_U8   u8CbCntHighThr;      // u7.0, [0,68]
    XMEDIA_U8   u8CbCntLowThr;       // u7.0, [0,68]
    XMEDIA_U8   u8BldAvgCur;         // u4.0, [0, 8]
    //XMEDIA_U8   u8DeFColorCr;
    //XMEDIA_U8   u8DeFColorCb;
    XMEDIA_U16  u16CbCntRatio;       // u7.4, [0,1024]
    XMEDIA_U16  u16CbThr;            // u12.0, [0,4095]
    XMEDIA_U16  u16VarThr;
    XMEDIA_S16  s16CbCrRatioLmtHigh;
    XMEDIA_U32  u32UpdateIndex;
} ISP_LOCAL_CAC_USR_CFG_S;


typedef struct ISP_LOCAL_CAC_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8DePurpleCtrCr;
    XMEDIA_U8   u8DePurpleCtrCb;
    XMEDIA_U16  u16LumaThr;          // u12.0, [0,4095]
    XMEDIA_U16  u16RLumaThr;
    XMEDIA_U16  u16GLumaThr;
    XMEDIA_U16  u16BLumaThr;
    XMEDIA_S16  s16CbCrRatioLmtLow;
} ISP_LOCAL_CAC_DYNA_CFG_S;

typedef struct ISP_LOCAL_CAC_MODULE_REG_CFG_S {
    XMEDIA_BOOL bLocalCacEn;
    ISP_LOCAL_CAC_USR_CFG_S    stUsrRegCfg;
    ISP_LOCAL_CAC_DYNA_CFG_S   stDynaRegCfg;
    ISP_LOCAL_CAC_STATIC_CFG_S stStaticRegCfg;
} ISP_LCAC_REG_CFG_S;

typedef struct ISP_GLOBAL_CAC_STATIC_CFG_S {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_BOOL bGCacVerFilEn;
    XMEDIA_U8   u8GCacBlendSel;
    XMEDIA_U8   u8GcacChrVerMode;
    XMEDIA_U8   u8GcacClipModeHor;
    XMEDIA_U8   u8GcacClipModeVer;
} ISP_GLOBAL_CAC_STATIC_CFG_S;

typedef struct ISP_GLOBAL_CAC_USR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8VerNormShift;
    XMEDIA_U8   u8VerNormFactor;
    XMEDIA_U8   u8HorNormShift;
    XMEDIA_U8   u8HorNormFactor;
    XMEDIA_U16  u16StartCoorHor;
    XMEDIA_U16  u16StartCoorVer;
    XMEDIA_S16  s16ParamRedA;
    XMEDIA_S16  s16ParamRedB;
    XMEDIA_S16  s16ParamRedC;
    XMEDIA_S16  s16ParamBlueA;
    XMEDIA_S16  s16ParamBlueB;
    XMEDIA_S16  s16ParamBlueC;
    XMEDIA_U16  u16CenterCoorHor;
    XMEDIA_U16  u16CenterCoorVer;
    XMEDIA_U16  u16CorVarThr;
    XMEDIA_U32  u32UpdateIndex;
} ISP_GLOBAL_CAC_USR_CFG_S;

typedef struct ISP_GLOBAL_CAC_DYNA_CFG_S {
    XMEDIA_U32 u32Reserved;
} ISP_GLOBAL_CAC_DYNA_CFG_S;

typedef struct ISP_GLOBAL_CAC_MODULE_REG_CFG_S {
    XMEDIA_BOOL bGlobalCacEn;
    ISP_GLOBAL_CAC_USR_CFG_S    stUsrRegCfg;
    ISP_GLOBAL_CAC_DYNA_CFG_S   stDynaRegCfg;
    ISP_GLOBAL_CAC_STATIC_CFG_S stStaticRegCfg;
} ISP_GCAC_REG_CFG_S;

typedef struct ISP_DEMOSAIC_STATIC_CFG {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bDeSatEnable;       // u1.0
    XMEDIA_BOOL bAHDEnable;         // u1.0
    XMEDIA_BOOL bDeFakeEnable;      // u1.0
    XMEDIA_U8   u8Lpff3;            // u8.0,
    XMEDIA_U8   u8hvBlendLimit1;    // u8.0,
    XMEDIA_U8   u8hvBlendLimit2;    // u8.0,
    XMEDIA_U8   u8hvColorRatio;     // u8.0,
    XMEDIA_U8   u8hvSelection;      // u2.0
    XMEDIA_U8   u8GClipBitSft;      // u4.0,
    XMEDIA_U8   u8DitherMask;       // u8.0,
    XMEDIA_U8   u8DitherRatio;      // u8.0,
    XMEDIA_S16  s16CcVarThresh;     // s13.0,
    XMEDIA_U16  u16DeSatThresh1;    // u10.0,
    XMEDIA_U16  u16DeSatThresh2;    // u10.0,
    XMEDIA_U16  u16DeSatHig;        // u9.0,
    XMEDIA_U16  u16DeSatProtSl;     // u10.0
    XMEDIA_U16  u16AhdPart1;        // u10.0,
    XMEDIA_U16  u16AhdPart2;        // u8.0,
    XMEDIA_U16  u16GFThLow;         // u9.0,   gf low-fix strength
    XMEDIA_U16  u16GFThHig;         // u9.0,   gf high-fix strength
    XMEDIA_U16  u16CbCrAvgThr;      // u12.0,
} ISP_DEMOSAIC_STATIC_CFG_S;

typedef struct ISP_DEMOSAIC_DYNA_CFG {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bUpdateGF;
    XMEDIA_U16  u16FakeCrVarThrLow;  // u16.0
    XMEDIA_U16  u16FakeCrVarThrHigh; // u16.0
    XMEDIA_U8   u8DeFColorCr;
    XMEDIA_U8   u8DeFColorCb;
    XMEDIA_U8   u8CcHFMaxRatio;      // u5.0,
    XMEDIA_U8   u8CcHFMinRatio;      // u5.0,
    XMEDIA_U8   u8Lpff0;             // s4.0,
    XMEDIA_U8   u8Lpff1;             // u5.0,
    XMEDIA_U8   u8Lpff2;             // u5.0,
    XMEDIA_U8   u8DitherMax;         // u8.0,
    XMEDIA_U8   u8FcrGFGain;         // s3.2,   fcr control
    XMEDIA_U8   u8AwbGFGainLow;      // u3.4,
    XMEDIA_U8   u8AwbGFGainHig;      // u3.4,
    XMEDIA_U8   u8AwbGFGainMax;      // u4.0,
    XMEDIA_U8   u8EhcGray;           // u3.4,
    XMEDIA_U16  u16DeSatLow;         // u9.0,
    XMEDIA_S16  s16DeSatRatio;       // s9.4,
    XMEDIA_U16  u16DeSatProtTh;      // u10.0
    XMEDIA_U16  u16HfIntpBlurThLow;  // u10.0,
    XMEDIA_U16  u16HfIntpBlurThHig;  // u10.0,
    XMEDIA_U16  u16HfIntpBlurRatio;  // u9.4,
    XMEDIA_U16  u16FcrDetLow;        // u12.0,  fcr det thresh
    XMEDIA_U16  au16GFBlurLut[ISP_DEMOSAIC_LUT_LENGTH]; // u12.0,
    XMEDIA_U32  u32CoefK;            // u18.6
    XMEDIA_U32  u32CoefB;            // u13.6
    XMEDIA_U8   u8ColorNoiseThdY;    // u4.0
    XMEDIA_U8   u8ColorNoiseStrY;    // u6.0
} ISP_DEMOSAIC_DYNA_CFG_S;

typedef struct ISP_DEMOSAIC_MODULE_REG_CFG {
    XMEDIA_BOOL bVhdmEnable;       // u1.0,
    XMEDIA_BOOL bNddmEnable;       // u1.0,
    ISP_DEMOSAIC_STATIC_CFG_S stStaticRegCfg;
    ISP_DEMOSAIC_DYNA_CFG_S   stDynaRegCfg;
} ISP_DEMOSAIC_REG_CFG_S;

typedef struct ISP_ANTIFALSECOLOR_STATIC_CFG {
    XMEDIA_BOOL bResh;
    XMEDIA_U16  u16FcrLimit1;      // u12.0,
    XMEDIA_U16  u16FcrLimit2;      // u12.0,
    XMEDIA_U16  u16FcrThr;         // u12.0,
} ISP_ANTIFALSECOLOR_STATIC_CFG_S;

typedef struct ISP_ANTIFALSECOLOR_DYNA_CFG {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8FcrGain;         // u5.0,
    XMEDIA_U8   u8FcrRatio;        // u6.0,
    XMEDIA_U8   u8FcrGrayRatio;    // u5.0,
    XMEDIA_U8   u8FcrCmaxSel;      // u4.0,
    XMEDIA_U8   u8FcrDetgSel;      // u4.0,
    XMEDIA_U16  u16FcrHfThreshLow; // u10.0,  fcr hf thresh low
    XMEDIA_U16  u16FcrHfThreshHig; // u10.0,  fcr hf thresh high
} ISP_ANTIFALSECOLOR_DYNA_CFG_S;

typedef struct ISP_ANTIFALSECOLOR_MODULE_REG_CFG {
    XMEDIA_BOOL bFcrEnable;        // u1.0,
    ISP_ANTIFALSECOLOR_STATIC_CFG_S stStaticRegCfg;
    ISP_ANTIFALSECOLOR_DYNA_CFG_S   stDynaRegCfg;
} ISP_ANTIFALSECOLOR_REG_CFG_S;

typedef struct ISP_CA_USR_CFG {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bCaLutUpdateEn; // u1.0
    XMEDIA_U8   u8BufId;
    XMEDIA_U16  u16CaLumaThrHigh; // u10.0
    XMEDIA_S16  s16CaLumaRatioHigh; // U1.10   High Lightness Thresh' Ratio in Y-Alpha
    XMEDIA_S16  s16CaLumaRatioLow;
    XMEDIA_U16  au16YRatioLUT[ISP_CA_YRATIO_LUT_LENGTH]; // U1.10
    XMEDIA_S16  s16CaYLumaRatio;
    XMEDIA_S16  s16CaSkinBetaRatio;
    XMEDIA_U32  u32UpdateIndex;
} ISP_CA_USR_CFG_S;

typedef struct ISP_CA_DYNA_CFG {
    XMEDIA_BOOL bResh;
    XMEDIA_U16  u16CaISORatio; // 1.10bit  ISO Ratio  For UV,Max = 2047 FW Limi
} ISP_CA_DYNA_CFG_S;


typedef struct ISP_CA_STATIC_CFG {
    XMEDIA_BOOL bStaticResh;
    XMEDIA_BOOL bCaLlhcProcEn;  // u1.0
    XMEDIA_BOOL bCaSkinProcEn;  // u1.0
    XMEDIA_BOOL bCaSatuAdjEn;   // u1.0
    XMEDIA_U16  u16CaLumaThrLow;        // u10.0
    XMEDIA_U16  u16CaDarkChromaThrLow;  // u10.0
    XMEDIA_U16  u16CaDarkChromaThrHigh; // u10.0
    XMEDIA_U16  u16CaSDarkChromaThrLow; // u10.0
    XMEDIA_U16  u16CaSDarkChromaThrHigh; // u10.0
    XMEDIA_S16  s16CaLumaRatioLow;      // U1.10
    XMEDIA_U16  u16CaSkinLowLumaMinU;   // u10
    XMEDIA_U16  u16CaSkinLowLumaMaxU;   // u10
    XMEDIA_U16  u16CaSkinLowLumaMinUy;  // u10
    XMEDIA_U16  u16CaSkinLowLumaMaxUy;  // u10
    XMEDIA_U16  u16CaSkinHighLumaMinU;  // u10
    XMEDIA_U16  u16CaSkinHighLumaMaxU;  // u10
    XMEDIA_U16  u16CaSkinHighLumaMinUy; // u10
    XMEDIA_U16  u16CaSkinHighLumaMaxUy; // u10
    XMEDIA_U16  u16CaSkinLowLumaMinV;   // u10
    XMEDIA_U16  u16CaSkinLowLumaMaxV;   // u10
    XMEDIA_U16  u16CaSkinLowLumaMinVy;  // u10
    XMEDIA_U16  u16CaSkinLowLumaMaxVy;  // u10
    XMEDIA_U16  u16CaSkinHighLumaMinV;  // u10
    XMEDIA_U16  u16CaSkinHighLumaMaxV;  // u10
    XMEDIA_U16  u16CaSkinHighLumaMinVy; // u10
    XMEDIA_U16  u16CaSkinHighLumaMaxVy; // u10
    XMEDIA_S16  s16CaSkinUvDiff;        // S11
    XMEDIA_U16  u16CaSkinRatioThrLow;   // u12.0
    XMEDIA_U16  u16CaSkinRatioThrMid;   // u12.0
    XMEDIA_U16  u16CaSkinRatioThrHigh;  // u12.0
} ISP_CA_STATIC_CFG_S;

typedef struct ISP_CA_REG_CFG {
    XMEDIA_BOOL bCaEn;
    XMEDIA_BOOL bLut2SttEn;
    ISP_CA_STATIC_CFG_S stStaticRegCfg;
    ISP_CA_DYNA_CFG_S   stDynaRegCfg;
    ISP_CA_USR_CFG_S    stUsrRegCfg;
} ISP_CA_REG_CFG_S;

typedef struct ISP_BAYERNR_STATIC_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8JnlmSel;                          // u2.0
    XMEDIA_U8   u8SADWinSizeSel;                    // u2.0
    XMEDIA_U16  u16JnlmMaxWtCoef;                   // u10.0
    XMEDIA_BOOL bLumaSel;                           // u1.0
    XMEDIA_U8    u8EnablePostProc;   // u8.0
    XMEDIA_U8    u8BilateralEnable;  // u8.0
    XMEDIA_BOOL bEnableSymmsad;  // u1.0
    XMEDIA_U8  u8ImplsnrStrength;  // u8.0
    XMEDIA_BOOL bImplsnrEnable3; // u1.0
    XMEDIA_BOOL bImplsnrEnable2; // u1.0
    XMEDIA_BOOL bImplsnrEnable1; // u1.0
    XMEDIA_U8   u8Gainsad; // u8.0
    XMEDIA_BOOL bPatternNoiseReduction; // u1.0
    XMEDIA_U8 u8WindowSizeSel; // u8.0

} ISP_BAYERNR_STATIC_CFG_S;

typedef struct ISP_BAYERNR_DYNA_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bMedcEnable;                        // u1.0
    XMEDIA_BOOL bWdrModeEn;                         // u1.0
    XMEDIA_BOOL bWdrFusionEn;                       // u1.0
    XMEDIA_BOOL bCenterWgtEn;                       // u1.0
    XMEDIA_U8   u8WdrMapFltMode;                    // u1.0
    XMEDIA_U8   au8BnrCRatio[ISP_BAYER_CHN_NUM];    // u6.0
    XMEDIA_U8   au8AmedMode[ISP_BAYER_CHN_NUM];     // u1.0
    XMEDIA_U8   au8AmedLevel[ISP_BAYER_CHN_NUM];    // u2.0
    XMEDIA_U8   u8JnlmSymCoef;                      // u8.0
    XMEDIA_U8   u8JnlmGain;                         // u8.0
    XMEDIA_U8   u8WdrMapGain;                       // u5.0
    XMEDIA_U16  u16ShotRatio;                       // u2.8
    XMEDIA_U16  u16JnlmCoringHig;                   // u16.0
    XMEDIA_U16  u16RLmtRgain;                       // u10.0
    XMEDIA_U16  u16RLmtBgain;                       // u10.0
    XMEDIA_U16  u16LmtOffset;                       // u10.0
    XMEDIA_U32  u32EdgeDetThr;                      // u17.0
    XMEDIA_U8   au8GCoringGainLut[ISP_BAYERNR_LUT_LENGTH];   // u4.4
    XMEDIA_U8   au8JnlmLimitLut[ISP_BAYERNR_LMTLUTNUM];      // u8.0
    XMEDIA_U8   au8JnlmLimitEvenLut[ISP_BAYERNR_LMTLUT_EVEN_NUM];      // u8.0
    XMEDIA_U8   au8JnlmLimitOddLut[ISP_BAYERNR_LMTLUT_ODD_NUM];      // u8.0

    XMEDIA_U8   au8JnlmLimitSLut[ISP_BAYERNR_LMTLUTNUM];     // u8.0
    XMEDIA_U8   au8JnlmLimitSEvenLut[ISP_BAYERNR_LMTLUT_EVEN_NUM];     // u8.0
    XMEDIA_U8   au8JnlmLimitSOddLut[ISP_BAYERNR_LMTLUT_ODD_NUM];     // u8.0

    XMEDIA_U16  au16JnlmCoringLowLut[ISP_BAYERNR_LUT_LENGTH]; // u14.0
    XMEDIA_U16  au16JnlmCoringLowEvenLut[ISP_BAYERNR_LUT_EVEN_LENGTH]; // u14.0
    XMEDIA_U16  au16JnlmCoringLowOddLut[ISP_BAYERNR_LUT_ODD_LENGTH]; // u14.0

    XMEDIA_U8   au8GcoringLut[ISP_BAYERNR_LUT_LENGTH];     // u8.0
    XMEDIA_U8   au8GcoringEvenLut[ISP_BAYERNR_LUT_EVEN_LENGTH];     // u8.0
    XMEDIA_U8   au8GcoringOddLut[ISP_BAYERNR_LUT_ODD_LENGTH];     // u8.0
    XMEDIA_U32  au32JnlmLimitMultGain[ISP_BAYER_CHN_NUM];          // u21.0
    XMEDIA_U32  au32JnlmLimitMultGainS[ISP_BAYER_CHN_NUM];         // u21.0
    XMEDIA_U8 u8BufId;
} ISP_BAYERNR_DYNA_CFG_S;

typedef struct ISP_BAYERNR_USR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8 bBnrLscEn;                          // u1.0
    XMEDIA_BOOL bBnrMonoSensorEn;                   // u1.0
    XMEDIA_U8   u8BnrLscMaxGain;                    // u2.6
    XMEDIA_U16  u16BnrLscCmpStrength;               // u1.8
} ISP_BAYERNR_USR_CFG_S;

typedef struct ISP_BAYERNR_MODULE_REG_CFG_S {
    XMEDIA_BOOL bBnrEnable;                             // u1.0
    ISP_BAYERNR_STATIC_CFG_S    stStaticRegCfg;
    ISP_BAYERNR_DYNA_CFG_S      stDynaRegCfg;
    ISP_BAYERNR_USR_CFG_S       stUsrRegCfg;
} ISP_BAYERNR_REG_CFG_S;


typedef struct ISP_LDCI_STATIC_CFG_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_BOOL bStaticResh;

    XMEDIA_U32  u32CalcLumaSel;
    XMEDIA_U32  u32LpfSft;
    XMEDIA_U32  u32ChrPosDamp;
    XMEDIA_U32  u32ChrNegDamp;

} ISP_LDCI_STATIC_CFG_S;

typedef struct ISP_LDCI_DYNA_CFG_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_BOOL bWrstatEn;
    XMEDIA_BOOL bRdstatEn;
    XMEDIA_BOOL bCalcEnable;
    XMEDIA_BOOL bLut2SttEn;
    XMEDIA_U8   u8BufId;

    XMEDIA_U32  u32CalcBlcCtrl;
    XMEDIA_U32  u32HePosLut[LDCI_HE_LUT_SIZE];
    XMEDIA_U32  u32HeNegLut[LDCI_HE_LUT_SIZE];
    XMEDIA_U32  u32UsmPosLut[LDCI_DE_USM_LUT_SIZE];
    XMEDIA_U32  u32UsmNegLut[LDCI_DE_USM_LUT_SIZE];
    XMEDIA_U32  u32ColorGainLut[LDCI_COLOR_GAIN_LUT_SIZE];
    XMEDIA_U32  u32LpfCoef[LDCI_LPF_LUT_SIZE];

    XMEDIA_U32  u32CalcMapOffsetX;
    XMEDIA_U32  u32CalcSmlMapStride;
    XMEDIA_U32  u32CalcSmlMapHeight;
    XMEDIA_U32  u32CalcTotalZone;
    XMEDIA_U32  u32CalcScaleX;
    XMEDIA_U32  u32CalcScaleY;
    XMEDIA_U32  u32BlkSmlMapWidth[3];
    XMEDIA_U32  u32StatVStart;
    XMEDIA_U32  u32StatVEnd;
    XMEDIA_U32  u32StatHStart;
    XMEDIA_U32  u32StatHEnd;
    XMEDIA_U32  u32StatSmlMapWidth;
    XMEDIA_U32  u32StatSmlMapHeight;
    XMEDIA_U32  u32StatTotalZone;
    ISP_LDCI_STAT_S stTprStat;
} ISP_LDCI_DYNA_CFG_S;

typedef struct ISP_LDCI_REG_CFG {
    ISP_LDCI_STATIC_CFG_S stStaticRegCfg;
    ISP_LDCI_DYNA_CFG_S   stDynaRegCfg;
} ISP_LDCI_REG_CFG_S;

typedef struct ISP_FPN_STATIC_CFG {

} ISP_FPN_STATIC_CFG_S;

typedef struct ISP_FPN_DYNA_CFG {
    XMEDIA_BOOL bIspFpnEnable;
    XMEDIA_BOOL bIspFpnOffline;

    XMEDIA_U32  u32IspFpnLineFrame;
    XMEDIA_U32  u32IspFpnCalibCorr;
    XMEDIA_U32  u32IspFpnFrameCalibShift;
    XMEDIA_U32  u32IspFpnOutShift;
    XMEDIA_U32  u32IspFpnInShift;
    XMEDIA_U32  u32IspFpnShift;
    XMEDIA_U32  u32IspFpnUpdateMode;
    XMEDIA_U32  u32IspFpnMaxO;
    XMEDIA_U32  u32IspFpnOverflowThr;
    XMEDIA_U32  u32IspFpnOffset[4];
    XMEDIA_U32  u32IspFpnStrength[4];
    XMEDIA_U32  u32IspFpnCorrectEnId[4];
    XMEDIA_U32  u32IspFpnHeight;
    XMEDIA_U32  u32IspFpnWidth;
    XMEDIA_U32  u32IspFpnUpdate;
    XMEDIA_U32  u32IspFpnLineWaddr;
    XMEDIA_U32  u32IspFpnLineRaddr;
} ISP_FPN_DYNA_CFG_S;

typedef struct ISP_FPN_REG_CFG {
    ISP_FPN_STATIC_CFG_S  stStaticRegCfg;
    ISP_FPN_DYNA_CFG_S   stDynaRegCfg;
} ISP_FPN_REG_CFG_S;

typedef struct ISP_GAMMA_STATIC_CFG_S {
    XMEDIA_BOOL  bStaticResh;
} ISP_GAMMA_STATIC_CFG_S;

typedef struct ISP_GAMMA_USR_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_BOOL bGammaLutUpdateEn;
    XMEDIA_U8   u8BufId;
    XMEDIA_BOOL bSwitchMode;
    XMEDIA_U16  au16GammaLUT[GAMMA_REG_NODE_NUM];  // u12.0
    XMEDIA_U8   au8GammaStep[GAMMA_OUTSEG_NUM];
    XMEDIA_U16  au16GammaInSeg[GAMMA_OUTSEG_NUM];
    XMEDIA_U16  au16GammaPos[GAMMA_OUTSEG_NUM];
    XMEDIA_U32  u32UpdateIndex;
} ISP_GAMMA_USR_CFG_S;

typedef struct ISP_GAMMA_REG_CFG_S {
    XMEDIA_BOOL bGammaEn;
    XMEDIA_BOOL bLut2SttEn;
    ISP_GAMMA_STATIC_CFG_S stStaticRegCfg;
    ISP_GAMMA_USR_CFG_S    stUsrRegCfg;
} ISP_GAMMA_REG_CFG_S;

typedef struct ISP_RGBIR_USR_CFG_S {
    XMEDIA_BOOL bResh;

    XMEDIA_U8   u8InPattern;
    XMEDIA_U8   u8OutPattern;
    XMEDIA_U8   u8ThresV;
    XMEDIA_U8   u8ThresH;
    XMEDIA_U16  u16Gain1;  // u12.0
    XMEDIA_U16  u16Gain2;  // u12.0

    XMEDIA_U16  u16ExpCtrl1;  // u12.0
    XMEDIA_U16  u16ExpCtrl2;  // u12.0
    XMEDIA_U16  u16ReciExp1;  // u12.0
    XMEDIA_U16  u16ReciExp2;  // u12.0

    XMEDIA_S16  s16Matrix[12];  // s15.0
    XMEDIA_U32  u32UpdateIndex;
} ISP_RGBIR_USR_CFG_S;

typedef struct ISP_RGBIR_REG_CFG_S {
    XMEDIA_BOOL bEnable;
    ISP_RGBIR_USR_CFG_S    stUsrRegCfg;
} ISP_RGBIR_REG_CFG_S;

typedef struct ISP_CSC_DYNAMIC_CFG_S {
    // Multiplier coefficients
    XMEDIA_BOOL bResh;

    XMEDIA_S16  s16CscCoef00;    // s5.10
    XMEDIA_S16  s16CscCoef01;    // s5.10
    XMEDIA_S16  s16CscCoef02;    // s5.10
    XMEDIA_S16  s16CscCoef10;    // s5.10
    XMEDIA_S16  s16CscCoef11;    // s5.10
    XMEDIA_S16  s16CscCoef12;    // s5.10
    XMEDIA_S16  s16CscCoef20;    // s5.10
    XMEDIA_S16  s16CscCoef21;    // s5.10
    XMEDIA_S16  s16CscCoef22;    // s5.10

    // DC components
    XMEDIA_S16  s16CscInDC0;     // s11.0
    XMEDIA_S16  s16CscInDC1;     // s11.0
    XMEDIA_S16  s16CscInDC2;     // s11.0

    XMEDIA_S16  s16CscOutDC0;    // s11.0
    XMEDIA_S16  s16CscOutDC1;    // s11.0
    XMEDIA_S16  s16CscOutDC2;    // s11.0
} ISP_CSC_DYNA_CFG_S;

typedef struct ISP_CSC_REG_CFG_S {
    XMEDIA_BOOL bEnable;       // u1.0
    ISP_CSC_DYNA_CFG_S    stDynaRegCfg;
} ISP_CSC_REG_CFG_S;

typedef struct {
    xmedia_bool blc_in;
    xmedia_bool blc_out;
} isp_blc_static_cfg;

typedef struct {
    xmedia_u16 blc[ISP_BAYER_CHN_NUM];
    xmedia_u16 out_blc;
} isp_blc_dyna_cfg;

typedef struct {
    XMEDIA_BOOL bReshStatic;
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
    XMEDIA_BOOL bReshDyna;
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
    XMEDIA_BOOL resh_dyna_init;
    isp_be_blc_static_cfg static_blc;
    isp_be_blc_dyna_cfg   dyna_blc;
} isp_be_blc_cfg;

typedef struct {
    XMEDIA_BOOL bReshStatic;
    /* FE module */
    isp_blc_static_cfg stFeDgBlc;
    isp_blc_static_cfg stFeWbBlc;
    isp_blc_static_cfg stFeAeBlc;
    isp_blc_static_cfg stFeBlc;
    isp_blc_static_cfg stRcBlc;
} isp_fe_blc_static_cfg;

typedef struct {
    XMEDIA_BOOL bReshDyna;
    /* FE module */
    isp_blc_dyna_cfg stFeDgBlc;
    isp_blc_dyna_cfg stFeWbBlc;
    isp_blc_dyna_cfg stFeAeBlc;
    isp_blc_dyna_cfg stFeBlc;
    isp_blc_dyna_cfg stRcBlc;
} isp_fe_blc_dyna_cfg;

typedef struct {
    XMEDIA_BOOL resh_dyna_init;
    isp_fe_blc_static_cfg static_blc;
    isp_fe_blc_dyna_cfg   dyna_blc;
} isp_fe_blc_cfg;

typedef struct ISP_FE_LUT_UPDATE_REG_CFG_S {
    XMEDIA_BOOL bAe1LutUpdate;
    XMEDIA_BOOL bLsc1LutUpdate;
} ISP_FE_LUT_UPDATE_REG_CFG_S;

typedef struct ISP_BE_LUT_UPDATE_REG_CFG_S {
    XMEDIA_BOOL bAeLutUpdate;
    XMEDIA_BOOL bDrcTmLutUpdate;
    XMEDIA_BOOL bLCacLutUpdate;
    XMEDIA_BOOL bNddmGfLutUpdate;
    XMEDIA_BOOL bLscLutUpdate;
    XMEDIA_BOOL bPreGammaLutUpdate;
} ISP_BE_LUT_UPDATE_REG_CFG_S;

typedef struct ISP_BE_STT2LUT_REGNEW_REG_CFG_S {
    XMEDIA_BOOL bGammaStt2LutRegnew;
    XMEDIA_BOOL bCaStt2LutRegnew;
    XMEDIA_BOOL bBnrStt2LutRegnew;
    XMEDIA_BOOL bLscStt2LutRegnew;
    XMEDIA_BOOL bPreGammaStt2LutRegnew;

    /* pingpong */
    XMEDIA_BOOL bDehazeStt2LutRegnew;    /* every frame */
    XMEDIA_BOOL bLdciStt2LutRegnew;      /* every frame */
    XMEDIA_BOOL bSharpenStt2LutRegnew;   /* every frame */
} ISP_BE_STT2LUT_REGNEW_REG_CFG_S;

typedef struct ISP_DEHAZE_LUT2STT_SYNC_REG_CFG_S {
    XMEDIA_U16  u16AirR;
    XMEDIA_U16  u16AirG;
    XMEDIA_U16  u16AirB;
    XMEDIA_U8   u8Strength;
} ISP_DEHAZE_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_LSC_LUT2STT_SYNC_REG_CFG_S {
    XMEDIA_BOOL bResh;
    XMEDIA_U8   u8MeshScale;
} ISP_LSC_LUT2STT_SYNC_REG_CFG_S;



typedef struct ISP_BNR_LUT2STT_SYNC_REG_CFG_S {
    XMEDIA_U32    au32JnlmLimitMultGain[4];
    XMEDIA_U32    au32JnlmLimitMultGainS[4];
    XMEDIA_U8     u8JnlmGain;
} ISP_BNR_LUT2STT_SYNC_REG_CFG_S;
typedef struct ISP_LDCI_LUT2STT_SYNC_REG_CFG_S {
    XMEDIA_U32    u32CalcBlcCtrl;
} ISP_LDCI_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_SHARPEN_LUT2STT_SYNC_REG_CFG_S {
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S          stMpiDynaRegCfg;
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S      stDefaultDynaRegCfg;
} ISP_SHARPEN_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_CA_LUT2STT_SYNC_REG_CFG_S {
    XMEDIA_S16  s16CaLumaRatioHigh;
    XMEDIA_S16  s16CaLumaRatioLow;
    XMEDIA_S16  s16CaYLumaRatio;
    XMEDIA_U16  u16CaIsoRatio;
} ISP_CA_LUT2STT_SYNC_REG_CFG_S;

typedef struct ISP_LUT2STT_SYNC_REG_CFG_S {
    XMEDIA_BOOL bAeEn;
    XMEDIA_BOOL bLaEn;
    XMEDIA_BOOL bAwbEn;
    XMEDIA_BOOL bWbEn;
    XMEDIA_BOOL bCcmEn;
    XMEDIA_BOOL bCcmColorToneEn;
    XMEDIA_BOOL bAfEn;
    XMEDIA_BOOL bSharpenEn;
    XMEDIA_BOOL bVhdmEn;
    XMEDIA_BOOL bNddmEn;
    XMEDIA_BOOL bLdciEn;
    XMEDIA_BOOL bLcacEn;
    XMEDIA_BOOL bGcacEn;
    XMEDIA_BOOL bFcrEn;
    XMEDIA_BOOL abDpcEn[4];
    XMEDIA_BOOL abGeEn;
    XMEDIA_BOOL bLscEn;
    XMEDIA_BOOL bGammaEn;
    XMEDIA_BOOL bCscEn;
    XMEDIA_BOOL bCaEn;
    XMEDIA_BOOL bWdrEn;
    XMEDIA_BOOL bDrcEn;
    XMEDIA_BOOL bDehazeEn;
    XMEDIA_BOOL bBnrEn;
    XMEDIA_U8   bBnrLscEn;
    XMEDIA_BOOL bDgEn;
    XMEDIA_BOOL b4DgEn;
    XMEDIA_BOOL bPregammaEn;
    XMEDIA_BOOL bClutEn;
    XMEDIA_BOOL bWdrSplitEn;
    XMEDIA_BOOL bExpanderEn;
    XMEDIA_BOOL bRgbirEn;
    ISP_DEHAZE_LUT2STT_SYNC_REG_CFG_S stDehazeSyncCfg;
    ISP_LSC_LUT2STT_SYNC_REG_CFG_S    stLscSyncCfg;
    ISP_BNR_LUT2STT_SYNC_REG_CFG_S  stBnrSyncCfg;
    ISP_LDCI_LUT2STT_SYNC_REG_CFG_S  stLdciSyncCfg;
    ISP_SHARPEN_LUT2STT_SYNC_REG_CFG_S  stSharpenSyncCfg;
    ISP_CA_LUT2STT_SYNC_REG_CFG_S    stCaSyncCfg;
} ISP_LUT2STT_SYNC_REG_CFG_S;

typedef union ISP_REG_KERNEL_CFG_KEY_U {
    XMEDIA_U32  u32Key;
    struct {
        XMEDIA_U32  bit1AwbCfg     : 1 ;   /* [0] */
        XMEDIA_U32  bit1BeAfCfg    : 1 ;   /* [1] */
        XMEDIA_U32  bit30Rsv       : 30;   /* [2:31] */
    };
} ISP_REG_KERNEL_CFG_KEY_U;

typedef  struct ISP_AWB_ZONE_CFG_S {
    XMEDIA_U32 u32Colum;
    XMEDIA_U32 u32Row;
    XMEDIA_U32 u32ZoneBin;
} AWB_ZONE_CFG_S;

typedef struct ISP_AF_ZONE_CFG_S {
    XMEDIA_U32 u32Colum;
    XMEDIA_U32 u32Row;
} AF_ZONE_CFG_S;


typedef struct ISP_REG_KERNEL_CFG_S {
    AWB_ZONE_CFG_S              stAWBZoneCfg;
    AF_ZONE_CFG_S               stBeAFZoneCfg;
} ISP_REG_KERNEL_CFG_S;

typedef struct ISP_KERNEL_REG_CFG_S {
    ISP_REG_KERNEL_CFG_KEY_U    unKey;
    ISP_REG_KERNEL_CFG_S        astAlgKernelCfg[ISP_STRIPING_MAX_NUM];
} ISP_KERNEL_REG_CFG_S;

typedef union ISP_REG_CFG_KEY_U {
    XMEDIA_U64  u64Key;
    struct {
        XMEDIA_U64  bit1AeCfg1      : 1 ;   /* [0] */
        XMEDIA_U64  bit1AeCfg2      : 1 ;   /* [1] */
        XMEDIA_U64  bit1AwbDynCfg   : 1 ;   /* [2] */
        XMEDIA_U64  bit1AwbCfg      : 1 ;   /* [3] */
        XMEDIA_U64  bit1AfFeCfg     : 1 ;   /* [4] */
        XMEDIA_U64  bit1DrcCfg      : 1 ;   /* [5] */
        XMEDIA_U64  bit1DpCfg       : 1 ;   /* [6] */
        XMEDIA_U64  bit1SharpenCfg  : 1 ;   /* [7] */
        XMEDIA_U64  bit1GeCfg       : 1 ;   /* [8] */
        XMEDIA_U64  bit1WdrCfg      : 1 ;   /* [9] */
        XMEDIA_U64  bit1DehazeCfg   : 1 ;   /* [10] */
        XMEDIA_U64  bit1AwbCfg3     : 1 ;   /* [11] */
        XMEDIA_U64  bit1LscCfg      : 1 ;   /* [12] */
        XMEDIA_U64  bit1DemCfg      : 1 ;   /* [13] */
        XMEDIA_U64  bit1FcrCfg      : 1 ;   /* [14] */
        XMEDIA_U64  bit1McdsCfg     : 1 ;   /* [15] */
        XMEDIA_U64  bit1RcCfg       : 1 ;   /* [16] */
        XMEDIA_U64  bit1FsWdrCfg    : 1 ;   /* [17] */
        XMEDIA_U64  bit1BayernrCfg  : 1 ;   /* [18] */
        XMEDIA_U64  bit1FlickerCfg  : 1 ;   /* [19] */
        XMEDIA_U64  bit1CaCfg       : 1 ;   /* [20] */
        XMEDIA_U64  bit1NddmCfg     : 1 ;   /* [21] */
        XMEDIA_U64  bit1LocalCacCfg : 1 ;   /* [22] */
        XMEDIA_U64  bit1GlobalCacCfg: 1 ;   /* [23] */
        XMEDIA_U64  bit1DciCfg      : 1 ;   /* [24] */
        XMEDIA_U64  bit1LdciCfg     : 1 ;   /* [25] */
        XMEDIA_U64  bit1CscCfg      : 1 ;   /* [26] */
        XMEDIA_U64  bit1GammaCfg    : 1 ;   /* [27] */
        XMEDIA_U64  bit1PreGammaCfg : 1 ;   /* [28] */
        XMEDIA_U64  bit1SplitCfg    : 1 ;   /* [29] */
        XMEDIA_U64  bit1EdgeMarkCfg : 1;    /* [30] */
        XMEDIA_U64  bit1HlcCfg      : 1;    /* [31] */
        XMEDIA_U64  bit1HrsCfg      : 1 ;   /* [32] */
        XMEDIA_U64  bit1DgCfg       : 1 ;   /* [33] */
        XMEDIA_U64  bit1WDRDgCfg    : 1 ;   /* [34] */
        XMEDIA_U64  bit1FeDgCfg     : 1 ;   /* [35] */
        XMEDIA_U64  bit1AfBeCfg     : 1 ;   /* [36] */
        XMEDIA_U64  bit1FeBlcCfg    : 1 ;   /* [37] */
        XMEDIA_U64  bit1BeBlcCfg    : 1 ;   /* [38] */
        XMEDIA_U64  bit1ClutCfg     : 1 ;   /* [39] */
        XMEDIA_U64  bit1RLscCfg     : 1 ;   /* [40] */
        XMEDIA_U64  bit1FpnCfg      : 1 ;   /* [41] */
        XMEDIA_U64  bit1DetailCfg   : 1 ;   /* [42] */
        XMEDIA_U64  bit1ExpanderCfg : 1 ;   /* [43] */
        XMEDIA_U64  bit1RgbirCfg    : 1 ;   /* [44] */
        XMEDIA_U64  bit19Rsv        : 19;   /* [45:63] */
    };
} ISP_REG_CFG_KEY_U;

typedef struct ISP_ALG_REG_CFG_S {
    ISP_AE_REG_CFG_1_S           stAeRegCfg1;
    ISP_AE_REG_CFG_2_S           stAeRegCfg2;
    ISP_AE_REG_CFG_S             stAeRegCfg;
    ISP_MG_REG_CFG_S             stMgRegCfg;
    ISP_DG_REG_CFG_S             stDgRegCfg;
    ISP_4DG_REG_CFG_S            st4DgRegCfg;
    ISP_FE_DG_REG_CFG_S          stFeDgRegCfg;
    ISP_AWB_REG_CFG_S            stAwbRegCfg;

    ISP_AF_REG_CFG_S             stBEAfRegCfg;
    ISP_DPC_REG_CFG_S            stDpRegCfg;
    ISP_DRC_REG_CFG_S            stDrcRegCfg;
    ISP_PREGAMMA_REG_CFG_S       stPreGammaCfg;
    ISP_GE_REG_CFG_S             stGeRegCfg;
    ISP_SHARPEN_REG_CFG_S        stSharpenRegCfg;
    ISP_WDR_REG_CFG_S            stWdrRegCfg;
    ISP_DEHAZE_REG_CFG_S         stDehazeRegCfg;
    ISP_MCDS_REG_CFG_S           stMcdsRegCfg;
    ISP_LSC_REG_CFG_S            stLscRegCfg;
    ISP_DEMOSAIC_REG_CFG_S       stDemRegCfg;
    ISP_ANTIFALSECOLOR_REG_CFG_S stAntiFalseColorRegCfg;
    ISP_BAYERNR_REG_CFG_S        stBnrRegCfg;
    ISP_EXPANDER_REG_CFG_S       stExpanderCfg;
    ISP_CA_REG_CFG_S             stCaRegCfg;
    ISP_LCAC_REG_CFG_S           stLCacRegCfg;
    ISP_LDCI_REG_CFG_S           stLdciRegCfg;
    ISP_FPN_REG_CFG_S            stFpnRegCfg;
    isp_be_blc_cfg               stBeBlcCfg;
    isp_fe_blc_cfg               stFeBlcCfg;
    ISP_CSC_REG_CFG_S            stCscCfg;
    ISP_GAMMA_REG_CFG_S          stGammaCfg;
    ISP_RGBIR_REG_CFG_S          stRgbirCfg;
    ISP_RADIAL_CROP_REG_CFG_S    stRcRegCfg;
    ISP_FE_LUT_UPDATE_REG_CFG_S  stFeLutUpdateCfg;
    ISP_BE_LUT_UPDATE_REG_CFG_S  stBeLutUpdateCfg;
    ISP_LUT2STT_SYNC_REG_CFG_S   astLut2SttSyncCfg[ISP_LUT2STT_SYNC_BUF_MAX_NUM];
    ISP_BE_STT2LUT_REGNEW_REG_CFG_S stStt2LutRegnewCfg;
} ISP_ALG_REG_CFG_S;

typedef struct ISP_REG_CFG_S {
    XMEDIA_U8 u8CfgNum;
    ISP_REG_CFG_KEY_U     unKey;
    ISP_ALG_REG_CFG_S     stAlgRegCfg[ISP_STRIPING_MAX_NUM];
    ISP_KERNEL_REG_CFG_S  stKernelRegCfg;
} ISP_REG_CFG_S;

typedef struct ISP_BE_WO_REG_CFG_S {
    S_ISPBE_REGS_TYPE       stBeRegCfg[ISP_STRIPING_MAX_NUM];
} ISP_BE_WO_REG_CFG_S;

typedef struct {
    S_ISP_ONLINE_STT_REGS_TYPE stBeOnlineSttReg[ISP_MAX_BE_NUM];
} ISP_BE_RW_ONLINE_STT_REG_S;

typedef struct ISP_RO_REG_CFG_S {
    ISP_DPC_REG_CFG_S       stDpRegCfg;
} ISP_RO_REG_CFG_S;

typedef struct ISP_BE_WO_CFG_BUF_S {
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_U64  u64Size;
    XMEDIA_VOID ATTRIBUTE *pVirAddr;
} ISP_BE_WO_CFG_BUF_S;

typedef struct ISP_BE_CFG_BUF_INFO_S {
    XMEDIA_BOOL             bInit;
    XMEDIA_S32              s32UseCnt;
    ISP_BE_WO_CFG_BUF_S stBeBufHaddr;
} ISP_BE_CFG_BUF_INFO_S;

/* offline mode isp sync para */
#define SYNC_DRC_PRELUMA_NUM            8
#define SYNC_WDR_EXP_VAL_NUM            2
typedef struct ISP_BE_SYNC_PARA_S {
    XMEDIA_U32  u32LdciComp;
    XMEDIA_U8   u8DrcShpLog;
    XMEDIA_U32  u32DrcDivDenomLog;
    XMEDIA_U32  u32DrcDenomExp;
    XMEDIA_U32  au32DrcPrevLuma[SYNC_DRC_PRELUMA_NUM];
    XMEDIA_U32  au32IspDgain[ISP_BAYER_CHN_NUM];
    XMEDIA_U32  au32WDRGain[SYNC_WDR_EXP_VAL_NUM];
    XMEDIA_U32  u32WdrExpRatio;
    XMEDIA_U32  u32FlickExpRatio;
    XMEDIA_U32  au32WdrExpVal[SYNC_WDR_EXP_VAL_NUM];
    XMEDIA_U32  u32WdrBlcComp;
    XMEDIA_BOOL bWDRMdtEn;
    XMEDIA_BOOL bFusionMode;
    XMEDIA_U16  u16ShortThr;
    XMEDIA_U16  u16LongThr;
    XMEDIA_U16  u16WgtSlope;
    XMEDIA_U32  u32WdrMaxRatio;
    XMEDIA_U16  au16CCM[CCM_MATRIX_SIZE];
    XMEDIA_U64  au64LdciWriteSttAddr[ISP_STRIPING_MAX_NUM];
    XMEDIA_U64  au64LdciReadSttAddr[ISP_STRIPING_MAX_NUM];
    isp_be_blc_dyna_cfg be_blc;
} ISP_BE_SYNC_PARA_S;

typedef struct ISP_BE_REGS_ATTR_S {
    XMEDIA_U8   u8BlockNum;
    XMEDIA_U8   u8BlockDev;
} ISP_BE_REGS_ATTR_S;

typedef struct ISP_MMZ_BUF_EX_S {
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_U64  u64Size;
    XMEDIA_VOID ATTRIBUTE *pVirAddr;
} ISP_MMZ_BUF_EX_S;

typedef struct ISP_BE_LUT2STT_BUF_S {
    ISP_MMZ_BUF_EX_S astLutSttBuf[2];   /* ping pong be lut stt info */
} ISP_BE_LUT2STT_BUF_S;

typedef struct ISP_BE_LUT2STT_ATTR_S {
    XMEDIA_BOOL              bInit;
    ISP_BE_LUT2STT_BUF_S astBeLutSttBuf[ISP_MAX_BE_NUM];
} ISP_BE_LUT2STT_ATTR_S;

typedef struct ISP_BE_ONLINE_STT_INFO_S {
    XMEDIA_BOOL bInit;
    XMEDIA_BOOL bDrvGetFirtStatInfoFlag;  /*frame 2 read online stt buffer 0 info */
    XMEDIA_U32  u32CurWriteFlag;
    ISP_MMZ_BUF_EX_S astBeSttBuf[2];   /* ping pong be online stt info */
} ISP_BE_ONLINE_STT_INFO_S;

typedef struct ISP_FE_STT_INFO_S {
    XMEDIA_BOOL bFirstFrame;
    XMEDIA_U32  u32CurWriteFlag;
    ISP_MMZ_BUF_EX_S astFeSttBuf[2];   /* ping pong fe stt info */
} ISP_FE_STT_INFO_S;

typedef struct ISP_FE_STT_ATTR_S {
    XMEDIA_BOOL           bInit;
    ISP_FE_STT_INFO_S stFeSttBuf[ISP_WDR_CHN_MAX];
} ISP_FE_STT_ATTR_S;

typedef struct ISP_FE_STIT_STT_ATTR_S {
    XMEDIA_BOOL                bInit;
    S_ISP_FE_STT_REGS_TYPE *pstSaveSttStat[ISP_MAX_PIPE_NUM][ISP_WDR_CHN_MAX];
    ISP_FE_STT_INFO_S      stFeSttBuf[ISP_MAX_PIPE_NUM][ISP_WDR_CHN_MAX];
} ISP_FE_STIT_STT_ATTR_S;
typedef struct ISP_BE_OFF_STT_ATTR_S {
    XMEDIA_BOOL           bInit;
    ISP_MMZ_BUF_EX_S  astBeSttBuf[ISP_STRIPING_MAX_NUM];
} ISP_BE_OFF_STT_ATTR_S;

typedef struct ISP_BE_STIT_STT_ATTR_S {
    XMEDIA_BOOL           bInit;
    ISP_MMZ_BUF_EX_S  astBeStitchBuf[ISP_MAX_PIPE_NUM];
} ISP_BE_STIT_STT_ATTR_S;

typedef struct ISP_LDCI_BUF_S {
    XMEDIA_BOOL           bInit;
    XMEDIA_U8             u8BufIdx;
    XMEDIA_U8             u8BufNum;
    ISP_MMZ_BUF_EX_S  astLdciBuf[MAX_ISP_BE_BUF_NUM];
} ISP_LDCI_BUF_S;

typedef struct ISP_LDCI_STT_ADDR_S {
    XMEDIA_U64 au64CurWriteSttAddr[ISP_STRIPING_MAX_NUM];
    XMEDIA_U64 au64ReadSttAddr[ISP_STRIPING_MAX_NUM];
    ISP_LDCI_STAT_S stLdciStat;
} ISP_LDCI_STT_ADDR_S;

typedef struct ISP_LDCI_READ_STT_BUF_S {
    XMEDIA_U8  u8BufNum;
    XMEDIA_U8  u8BufIdx;
    XMEDIA_U64 u64HeadPhyAddr;
    ISP_MMZ_BUF_EX_S astReadBuf[MAX_ISP_BE_BUF_NUM];
} ISP_LDCI_READ_STT_BUF_S;

typedef struct ISP_CLUT_BUF_S {
    XMEDIA_BOOL           bInit;
    ISP_MMZ_BUF_EX_S  stClutBuf;
} ISP_CLUT_BUF_S;

typedef struct ISP_SPEC_AWB_BUF_S {
    XMEDIA_BOOL           bInit;
    ISP_MMZ_BUF_EX_S  stSpecAwbBuf;
} ISP_SPEC_AWB_BUF_S;


#define ISP_SYNC_BUF_NODE_NUM           5
#define CFG2VLD_DLY_LIMIT               6
#define WDR_EXP_RATIO_SHIFT             6
#define DRC_COMP_SHIFT                  12

#define ISP_MAX_UPDATEINFO_BUF_NUM      (CFG2VLD_DLY_LIMIT)
#define ISP_MAX_FRAMEINFO_BUF_NUM       (CFG2VLD_DLY_LIMIT)
#define ISP_SAVEINFO_MAX                (CFG2VLD_DLY_LIMIT)

typedef struct ISP_SYNC_CFG_BUF_NODE_S {
    XMEDIA_BOOL bValid;

    ISP_SNS_REGS_INFO_S   stSnsRegsInfo;
    ISP_AE_REG_CFG_2_S    stAERegCfg;
    ISP_FSWDR_SYNC_CFG_S  stWDRRegCfg;
    ISP_DRC_REG_CFG_2_S   stDRCRegCfg;
    ISP_AWB_REG_CFG_2_S   stAWBRegCfg;
    isp_fe_blc_dyna_cfg   fe_blc_reg_cfg;
    isp_be_blc_dyna_cfg   be_blc_reg_cfg;
} ISP_SYNC_CFG_BUF_NODE_S;

typedef struct ISP_REGCFG_S {
    XMEDIA_BOOL bInit;
    ISP_REG_CFG_S stRegCfg;
    ISP_SYNC_CFG_BUF_NODE_S stSyncCfgNode;
} ISP_REGCFG_S;

typedef struct ISP_CHN_SWITCH_S {
    XMEDIA_U32  u32ChannelSel;
    XMEDIA_U32  au32WdrChnSel[5];
} ISP_CHN_SWITCH_S;

typedef struct ISP_WDR_CFG_S {
    XMEDIA_U8 u8WDRMode;
    XMEDIA_U32 au32ExpRatio[EXP_RATIO_NUM];
    ISP_FSWDR_SYNC_CFG_S stWdrRegCfg;
} ISP_WDR_CFG_S;

typedef struct ISP_RES_SWITCH_MODE_S {
    XMEDIA_U16   u16Width;
    XMEDIA_U16   u16Height;
    XMEDIA_FLOAT f32Fps;
    XMEDIA_U8 u8SnsMode;
} ISP_RES_SWITCH_MODE_S;

typedef struct ISP_PROC_MEM_S {
    XMEDIA_BOOL bInit;
    XMEDIA_U32  u32ProcSize;
    XMEDIA_U64  u64ProcPhyAddr;
    XMEDIA_VOID *pProcVirtAddr;
} ISP_PROC_MEM_S;

typedef struct ISP_MMZ_BUF_S {
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_VOID *pVirAddr;
} ISP_MMZ_BUF_S;

typedef struct ISP_TRANS_INFO_BUF_S {
    XMEDIA_BOOL       bInit;
    ISP_MMZ_BUF_S stDngInfo;
    ISP_MMZ_BUF_S stAttaInfo;
    ISP_MMZ_BUF_S stColorGammutInfo;
    ISP_MMZ_BUF_S stFrameInfo;
    ISP_MMZ_BUF_S stUpdateInfo;
} ISP_TRANS_INFO_BUF_S;

typedef struct ISP_PRO_INFO_BUF_S {
    XMEDIA_BOOL       bInit;
    ISP_MMZ_BUF_S stProNrParamInfo;
    ISP_MMZ_BUF_S stProShpParamInfo;
} ISP_PRO_INFO_BUF_S;

typedef struct ISP_PRO_PARAM_S {
    OPERATION_MODE_E enOperationMode;
    XMEDIA_U8 u8ProFrameNum;
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
    ISP_SNAP_NONE            = 0,
    ISP_SNAP_PREVIEW,
    ISP_SNAP_PICTURE,
    ISP_SNAP_PREVIEW_PICTURE,
    ISP_SNAP_BUTT
} ISP_SNAP_PIPE_MODE_E;

typedef struct ISP_SNAP_ATTR_S {
    SNAP_TYPE_E enSnapType;
    XMEDIA_S32 s32PreviewPipeId; /* -1,none snap */
    XMEDIA_S32 s32PicturePipeId; /* -1,none snap */
    ISP_PRO_PARAM_S stProParam;
    XMEDIA_BOOL bLoadCCM;    /* XMEDIA_TRUE:use CCM of SnapIspInfo, XMEDIA_FALSE: Algorithm calculate */
} ISP_SNAP_ATTR_S;

typedef struct ISP_PRO_NR_PARAM_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U32  u32ParamNum;
    ISP_NR_AUTO_ATTR_S astNrAttr[PRO_MAX_FRAME_NUM];
} ISP_PRO_NR_PARAM_S;
typedef struct ISP_PRO_SHP_PARAM_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U32  u32ParamNum;
    ISP_SHARPEN_AUTO_ATTR_S astShpAttr[PRO_MAX_FRAME_NUM];
} ISP_PRO_SHP_PARAM_S;
#define ISP_1ST_INT                     0x1
#define ISP_2ND_INT                     (1<<4)
#define ISP_UNDEF_INT                   0xF4
#define ISP_BE_FSTART                   (1<<16)
#define VIPROC_FRAME_START_INT          (1<<10)

#define ISP_DEV_SET_FD                  _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, XMEDIA_S32)
#define ISP_GET_FRAME_EDGE              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, XMEDIA_U32)
#define ISP_GET_VD_TIMEOUT              _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_GET_VD_END_TIMEOUT          _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_END_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_GET_VD_BEEND_TIMEOUT        _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_BEEND_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_SET_INT_ENABLE              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, XMEDIA_BOOL)
#define ISP_GET_VERSION                 _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, XMEDIA_U64)
#define ISP_STAT_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, ISP_STAT_INFO_S)
#define ISP_STAT_BUF_PUT                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, ISP_STAT_INFO_S)
#define ISP_STAT_ACT_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_ACT_GET, ISP_STAT_INFO_S)
#define ISP_REG_CFG_INIT                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, ISP_REG_CFG_S)
#define ISP_REG_CFG_SET                 _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, ISP_KERNEL_REG_CFG_S)

#define ISP_BE_CFG_BUF_INIT             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_INIT, XMEDIA_U64)
#define ISP_GET_BE_BUF_FIRST            _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_BE_BUF_FIRST, XMEDIA_U64)
#define ISP_BE_FREE_BUF_GET             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_FREE_BUF_GET, ISP_BE_WO_CFG_BUF_S)
#define ISP_BE_LAST_BUF_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_LAST_BUF_GET, XMEDIA_U64)
#define ISP_BE_CFG_BUF_EXIT             _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_EXIT)
#define ISP_BE_CFG_BUF_CTL              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_CTL, ISP_BE_WO_CFG_BUF_S)
#define ISP_BE_CFG_BUF_RUNNING          _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_RUNNING)
#define ISP_BE_All_BUF_INIT             _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_All_BUF_INIT)

#define ISP_MEM_INFO_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, XMEDIA_BOOL)
#define ISP_MEM_INFO_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, XMEDIA_BOOL)
#define ISP_P2EN_INFO_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_P2EN_INFO_GET, XMEDIA_BOOL)
#define ISP_INIT_INFO_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_INIT_INFO_SET, XMEDIA_BOOL)
#define ISP_SYNC_INIT_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_INIT_SET, XMEDIA_BOOL)
#define ISP_RUN_STATE_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RUN_STATE_SET, XMEDIA_U64)
#define ISP_RESET_CTX                   _IO(IOC_TYPE_ISP, IOC_NR_ISP_RESET_CTX)
#define ISP_CONFIG_INFO_SET             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CONFIG_INFO_SET, ISP_CONFIG_INFO_S)
#define ISP_SNAP_INFO_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_INFO_GET, ISP_SNAP_INFO_S)
#define ISP_PRO_TRIGGER_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_EN_GET, XMEDIA_BOOL)
#define ISP_UPDATE_POS_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_POS_GET, XMEDIA_U32)
#define ISP_FRAME_CNT_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_CNT_GET, XMEDIA_U32)
#define ISP_SNAP_ATTR_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_ATTR_GET, ISP_SNAP_ATTR_S)

#define ISP_PROC_INIT                   _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, ISP_PROC_MEM_S)
#define ISP_PROC_WRITE_ING              _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK               _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT                   _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, XMEDIA_U32)
#define ISP_TRANS_BUF_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_INIT, ISP_TRANS_INFO_BUF_S)
#define ISP_TRANS_BUF_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_EXIT)
#define ISP_PRO_BUF_INIT                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_BUF_INIT, ISP_PRO_INFO_BUF_S)
#define ISP_PRO_BUF_EXIT                _IO(IOC_TYPE_ISP,  IOC_NR_ISP_PRO_BUF_EXIT)

#define ISP_UPDATE_INFO_SET             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_INFO_SET, ISP_DCF_UPDATE_INFO_S)
#define ISP_FRAME_INFO_SET              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_SET, ISP_FRAME_INFO_S)
#define ISP_FRAME_INFO_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_GET, ISP_FRAME_INFO_S)
#define ISP_DNG_INFO_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_SET, DNG_IMAGE_DYNAMIC_INFO_S)
#define ISP_DNG_INFO_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_GET, DNG_IMAGE_STATIC_INFO_S)
#define ISP_SYNC_CFG_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, ISP_SYNC_CFG_BUF_NODE_S)
#define ISP_WDR_CFG_SET                 _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, ISP_WDR_CFG_S)
#define ISP_BE_SYNC_PARAM_INIT          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_SYNC_PARAM_INIT, ISP_BE_SYNC_PARA_S)
#define ISP_RES_SWITCH_SET              _IO(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET)
#define ISP_CHN_SELECT_CFG              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CHN_SELECT_CFG, XMEDIA_U32)
#define ISP_BE_SWITCH_FINISH_STATE_SET  _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET)
#define ISP_MODE_SWITCH_SET         _IO(IOC_TYPE_ISP, IOC_NR_ISP_MODE_SWITCH_SET)

#define ISP_PWM_NUM_GET                 _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, XMEDIA_U32)
#define ISP_WORK_MODE_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_INIT, ISP_BLOCK_ATTR_S)
#define ISP_WORK_MODE_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_GET, ISP_WORKING_MODE_S)
#define ISP_WORK_MODE_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_EXIT)
#define ISP_SET_MOD_PARAM               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_MOD_PARAM               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_SET_CTRL_PARAM              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_CTRL_PARAM, ISP_CTRL_PARAM_S)
#define ISP_GET_CTRL_PARAM              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_CTRL_PARAM, ISP_CTRL_PARAM_S)
#define ISP_PRE_BLK_NUM_UPDATE          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PRE_BLK_NUM_UPDATE, XMEDIA_U8)

#define ISP_GET_HDR_ATTR                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_HDR_ATTR, VI_PIPE_HDR_ATTR_S)
#define ISP_GET_WDR_ATTR                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_WDR_ATTR, VI_PIPE_WDR_ATTR_S)
#define ISP_GET_STITCH_ATTR             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_STITCH_ATTR, VI_STITCH_ATTR_S)
#define ISP_GET_PIPE_SIZE               _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_PIPE_SIZE, SIZE_S)
#define ISP_PUB_ATTR_INFO               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PUB_ATTR_INFO, ISP_PUB_ATTR_S)
#define ISP_KERNEL_RUNONCE              _IO(IOC_TYPE_ISP, IOC_NR_ISP_KERNEL_RUNONCE)
#define ISP_OPT_RUNONCE_INFO            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_OPT_RUNONCE_INFO, XMEDIA_BOOL)
#define ISP_SET_PROCALCDONE             _IO(IOC_TYPE_ISP, IOC_NR_ISP_SET_PROCALCDONE)
#define ISP_YUV_RUNONCE_INFO            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_YUV_RUNONCE_INFO, XMEDIA_BOOL)
#define ISP_KERNEL_YUV_RUNONCE          _IO(IOC_TYPE_ISP,  IOC_NR_ISP_KERNEL_YUV_RUNONCE)
#define ISP_SYNC_STITCH_PARAM_INIT      _IO(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_STITCH_PARAM_INIT)
#define ISP_CLUT_BUF_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_INIT)
#define ISP_CLUT_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_EXIT)
#define ISP_CLUT_BUF_GET                _IOR(IOC_TYPE_ISP,IOC_NR_ISP_CLUT_BUF_GET, XMEDIA_U64)
#define ISP_LDCI_BUF_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_INIT)
#define ISP_LDCI_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_EXIT)
#define ISP_LDCI_READ_STT_BUF_GET       _IOR(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_READ_STT_BUF_GET, ISP_LDCI_READ_STT_BUF_S)

#define ISP_STT_BUF_INIT                _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_INIT)
#define ISP_STT_BUF_EXIT                _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_EXIT)
#define ISP_STT_ADDR_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_ADDR_INIT)
#define ISP_BE_LUT_STT_BUF_GET          _IOR(IOC_TYPE_ISP,IOC_NR_ISP_BE_LUT_STT_BUF_GET, XMEDIA_U64)

#define ISP_SPECAWB_BUF_INIT            _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_INIT)
#define ISP_SPECAWB_BUF_EXIT            _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_EXIT)
#define ISP_SPECAWB_BUF_GET             _IOR(IOC_TYPE_ISP,IOC_NR_ISP_SPECAWB_BUF_GET, XMEDIA_U64)
#define ISP_SET_RGBIR_FORMAT            _IOW(IOC_TYPE_ISP,IOC_NR_ISP_SET_RGBIR_FORMAT, ISP_IRBAYER_FORMAT_E)

#define ISP_GET_DEV(f)             (*((XMEDIA_U32*)(f)))

#define ISP_CHECK_PIPE(pipe) \
    do {\
        if (((pipe) < 0) || ((pipe) >= ISP_MAX_PIPE_NUM))\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Err isp pipe %d!\n", pipe);\
            return ERR_CODE_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define ISP_CHECK_VIR_PIPE(pipe)\
    do {\
        if (((pipe) < ISP_MAX_PIPE_NUM) && ((pipe) >= ISP_MAX_PHY_PIPE_NUM))\
        {\
            return XMEDIA_SUCCESS;\
        }\
    }while(0)


#define ISP_CHECK_POINTER(ptr)\
    do {\
        if (ptr == XMEDIA_NULL)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n");\
            return ERR_CODE_ISP_NULL_PTR;\
        }\
    }while(0)

#define ISP_CHECK_POINTER_VOID(ptr)\
    do {\
        if (ptr == XMEDIA_NULL)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n");\
            return;\
        }\
    }while(0)

#define ISP_CHECK_BOOL(bool)\
    do {\
        if (( XMEDIA_TRUE != bool)&&( XMEDIA_FALSE != bool))\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Invalid ISP Bool Type %d!\n", bool);\
            return ERR_CODE_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define ISP_CHECK_BUF_INIT(pipe, init)\
    do{\
        if ((init) != XMEDIA_FALSE)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] buffer is initialized, Please exit ISP!!!\n", pipe);\
            return XMEDIA_FAILURE;\
        }\
    }while(0)

#define ISP_CHECK_BUF_EXIT(pipe, init)\
    do{\
        if ((init) != XMEDIA_TRUE)\
        {\
            return XMEDIA_SUCCESS;\
        }\
    }while(0)

#define ISP_CHECK_BEBUF_INIT(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].stBeBufInfo.bInit != XMEDIA_TRUE)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] BeBuf doesn't initialized!\n", pipe);\
            return ERR_CODE_ISP_NOT_INIT;\
        }\
    }while(0)

#define ISP_CHECK_STABUF_INIT(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].stStatisticsBuf.bInit != XMEDIA_TRUE)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] StaBuf doesn't initialized!\n", pipe);\
            return ERR_CODE_ISP_NOT_INIT;\
        }\
    }while(0)

#define ISP_CHECK_TRANBUF_INIT(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].stTransInfo.bInit != XMEDIA_TRUE)\
        {\
            ISP_TRACE(MODULE_DBG_WARN, "ISP[%d] TransBuf doesn't initialized!\n", pipe);\
            return ERR_CODE_ISP_NOT_INIT;\
        }\
    }while(0)

#define ISP_CHECK_EXIT_STATE(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].bIspRunFlag == XMEDIA_TRUE)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] cann't exit!\n", pipe);\
            return XMEDIA_FAILURE;\
        }\
    }while(0)

#define ISP_CHECK_ONLINE_MODE(pipe)\
    do{\
        if ((IS_ONLINE_MODE(g_astIspDrvCtx[pipe].stWorkMode.enIspRunningMode)) ||  \
            (IS_SIDEBYSIDE_MODE(g_astIspDrvCtx[pipe].stWorkMode.enIspRunningMode)))\
        {\
            return XMEDIA_SUCCESS;\
        }\
    }while(0)

#define ISP_DBG(bEn, fmt...)\
    do{\
        if (bEn)\
            printf(fmt);\
    }while(0)

#define SLAVE_CHECK_DEV(dev)\
    do {\
        if (((dev) < 0) || ((dev) >= CAP_SLAVE_MAX_NUM))\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Err slave dev %d!\n", dev);\
            return ERR_CODE_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define ISP_ALG_CHECK(AlgKey)\
    do{\
        if (AlgKey == 0)\
        {\
            return XMEDIA_SUCCESS; \
        }\
    }while(0)

#define ISP_STATISTICS_KEY              (0xFFFBBF77)
#define ISP_PIPE_FEAF_MODULE_ENABLE     0,0,0,0
#define ISP_PIPE_FELOGLUT_MODULE_ENABLE 0,0,0,0

#define ISP_CHECK_FEAF_MODULE(dev)\
    do{\
        XMEDIA_U8 bFeHaveAfMod[] = {ISP_PIPE_FEAF_MODULE_ENABLE};\
        if (!bFeHaveAfMod[dev])\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Invalid Af Fe Pipe %d!\n", dev);\
            return ERR_CODE_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#endif
