/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/19
  Description   : 
  History       :
  1.Date        : 2012/12/19
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __MKP_ISP_H__
#define __MKP_ISP_H__

#include "hi_type.h"
#include "hi_comm_3a.h"
#include "hi_common.h"
#include "hi_defines.h"

#define IOC_TYPE_ISP   'I'

#define MAX_ISP_STAT_BUF_NUM    2

#define ISP_VER_PRIX            "_ISP_V"
#define ISP_VERSION             CHIP_NAME ISP_VER_PRIX MK_VERSION(VER_X,VER_Y,VER_Z,VER_P,VER_B) VER_D

#define DRC_STRENGTH_MAX 0x3FF  /* max DRC strength 1023 */
#define DRC_STRENGTH_MIN 0x0   /* min DRC strength 0 */

typedef enum hiIOC_NR_ISP_E
{
    IOC_NR_ISP_SET_FD = 0,
    IOC_NR_ISP_GET_FRAME_EDGE,
    IOC_NR_ISP_GET_VD_TIMEOUT,
    IOC_NR_ISP_GET_VD_END_TIMEOUT,
    IOC_NR_ISP_SET_INT_ENABLE,
    IOC_NR_ISP_GET_VERSION,

    IOC_NR_ISP_STAT_BUF_INIT,
    IOC_NR_ISP_STAT_BUF_EXIT,
    IOC_NR_ISP_STAT_BUF_GET,
    IOC_NR_ISP_STAT_BUF_PUT,
    IOC_NR_ISP_STAT_ACT_GET,
    IOC_NR_ISP_REG_CFG_INIT,
    IOC_NR_ISP_REG_CFG_SET,
    IOC_NR_ISP_PROC_INIT,
    IOC_NR_ISP_PROC_WRITE_ING,
    IOC_NR_ISP_PROC_WRITE_OK,
    IOC_NR_ISP_PROC_EXIT,
    IOC_NR_ISP_PROC_PARAM_GET,
    IOC_NR_ISP_UPDATE_POS_GET,
    IOC_NR_ISP_SYNC_CFG_SET,
    IOC_NR_WDR_SYNC_CFG_SET,
    IOC_NR_ISP_UPDATEINFO_BUF_INIT,
    IOC_NR_ISP_UPDATEINFO_BUF_EXIT,
    IOC_NR_ACM_ATTR_GET,
    IOC_NR_ACM_ATTR_SET,
    IOC_NR_ACM_LUT_PHY_ADDR_GET,
    IOC_NR_ACM_COEF_SET,
    IOC_NR_ISP_MEM_INFO_SET,
    IOC_NR_ISP_MEM_INFO_GET,
    IOC_NR_ISP_CAP_INFO_SET,
    IOC_NR_ISP_CAP_INFO_GET,
    IOC_NR_ISP_DNG_INFO_SET,
    IOC_NR_ISP_PRO_EN_GET,
    IOC_NR_ISP_PRO_PARAM_SET,
    IOC_NR_ISP_PWM_NUM_GET,
    IOC_NR_ISP_RES_SWITCH_SET,

    IOC_NR_ISP_FRAME_INFO_SET,
    IOC_NR_ISP_FRAME_INFO_GET,
    IOC_NR_ISP_FRAME_INFO_BUF_INIT,
    IOC_NR_ISP_FRAME_INFO_BUF_EXIT,
    IOC_NR_ISP_DNG_INFO_GET,
    IOC_NR_ISP_DNG_INFO_BUF_INIT,
    IOC_NR_ISP_DNG_INFO_BUF_EXIT,

    IOC_NR_ISP_SET_MOD_PARAM,
    IOC_NR_ISP_GET_MOD_PARAM,

    IOC_NR_ISP_MODE_INFO,

    IOC_NR_ISP_BUTT,
} IOC_NR_ISP_E;

typedef struct hiISP_VD_TIMEOUT_S
{
   ISP_VD_INFO_S stVdInfo;
   HI_U32  u32MilliSec;     /*RW, Range: [0x0, 0xFFFFFFFF], the number of the */
   HI_S32  s32IntStatus;    /*RO, when there is ISP interrupt, then the s32IntStatus is 1 */
}ISP_VD_TIMEOUT_S;

typedef struct hiISP_VERSION_S
{
    HI_U8 au8MppVersion[4];
    HI_U8 au8Date[20];
    HI_U8 au8Time[20];
    HI_U32 u32Magic;
} ISP_VERSION_S;

/* the statistics of isp */
typedef struct hiISP_DP_STAT_S
{
    HI_U16  u16DefectPixelCount;
} ISP_DP_STAT_S;

typedef struct hiISP_DEHAZE_STAT_S
{
    //statics for dehaze run ,lsw
    //HI_U32 au32DehazeMinStatDout[256];
    //HI_U32 au32DehazeMaxStatDout[1024];
    //HI_U32 au32Hist[32];
    HI_U32  au32DehazeMinDout[DEFOG_ZONE_NUM/2];
} ISP_DEHAZE_STAT_S;


typedef struct hiISP_COMM_STAT_S
{
    HI_U16  au16WhiteBalanceGain[4];    
} ISP_COMM_STAT_S;

typedef struct hiISP_STAT_S
{
    ISP_AE_STAT_1_S     stAeStat1;     
    ISP_AE_STAT_2_S     stAeStat2;     /* Hi3519 not support */
    ISP_AE_STAT_3_S     stAeStat3;
    ISP_AE_STAT_4_S     stAeStat4;
    ISP_AE_STAT_5_S     stAeStat5;    
    ISP_AE_STAT_6_S     stAeStat6;
    ISP_AWB_STAT_1_S    stAwbStat1;    
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_AWB_STAT_3_S    stAwbStat3;    
    ISP_AWB_STAT_4_S    stAwbStat4;    
    ISP_AF_STAT_S       stAfStat;
    ISP_DP_STAT_S       stDpStat;
    ISP_COMM_STAT_S     stCommStat;
    ISP_DEHAZE_STAT_S   stDehazeStat;
} ISP_STAT_S;
typedef struct hiISP_PRIM_STAT_S
{
    ISP_AE_STAT_3_S     *pstAeStat3;
    ISP_AE_STAT_5_S     *pstAeStat5;   
    ISP_AWB_STAT_4_S    *pstAwbStat4;
} ISP_PRIM_STAT_S;

typedef union hiISP_STAT_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1AeStat1     : 1 ;   /* [0] */
        HI_U32  bit1AeStat2     : 1 ;   /* [1] */
        HI_U32  bit1AeStat3     : 1 ;   /* [2] */
        HI_U32  bit1AeStat4     : 1 ;   /* [3] */
        HI_U32  bit1AeStat5     : 1 ;   /* [4] */
        HI_U32  bit1AeStat6     : 1 ;   /* [5] */
        HI_U32  bit1AwbStat1    : 1 ;   /* [6] */
        HI_U32  bit1AwbStat2    : 1 ;   /* [7] */
        HI_U32  bit1AwbStat3    : 1 ;   /* [8] */
        HI_U32  bit1AwbStat4    : 1 ;   /* [9] */
        HI_U32  bit1AfStat      : 1 ;   /* [10] */
        HI_U32  bit1DpStat      : 1 ;   /* [11] */
        HI_U32  bit1CommStat    : 1 ;   /* [12] */
        HI_U32  bit1Dehaze      : 1 ;   /* [13] */
        HI_U32  bit2Rsv         : 2 ;   /* [14:15] */
        HI_U32  bit16IsrAccess  : 16;   /* [16:31] */
    };
}ISP_STAT_KEY_U;

typedef struct hiISP_STAT_INFO_S
{
    ISP_STAT_KEY_U  unKey;    
    HI_U32  u32PhyAddr;
    HI_VOID *pVirtAddr;                 /* (ISP_STAT_S *) */
} ISP_STAT_INFO_S;

/* the register config of isp */
typedef struct hiISP_AE_REG_CFG_1_S
{
    HI_U8   au8MeteringHistThresh[4];
    HI_U8   au8WeightTable[15][17];
} ISP_AE_REG_CFG_1_S;

typedef struct hiISP_AE_REG_CFG_2_S
{
    HI_U64  u64Exposure;
    HI_U32  u32IntTime[4];  /* 0: Shortest shutter(SS), 1: Short shutter(S), 2: long shutter(L), 3: longest shutter(LL) */
    HI_U32  u32IspDgain;

    HI_BOOL bPirisValid;
    HI_S32  s32PirisPos;

    ISP_FSWDR_MODE_E enFSWDRMode;
	HI_U32  u32WDRGain[4];
} ISP_AE_REG_CFG_2_S;

typedef struct hiISP_AWB_REG_CFG_1_S
{
    HI_U32  au32WhiteBalanceGain[4];
    HI_U16  au16ColorMatrix[9];
} ISP_AWB_REG_CFG_1_S;

typedef struct hiISP_AWB_REG_CFG_2_S
{
    HI_U16  u16MeteringWhiteLevelAwb;
    HI_U16  u16MeteringBlackLevelAwb;
    HI_U16  u16MeteringCrRefMaxAwb;
    HI_U16  u16MeteringCbRefMaxAwb;
    HI_U16  u16MeteringCrRefMinAwb;
    HI_U16  u16MeteringCbRefMinAwb;
    HI_U16  u16MeteringCrRefHighAwb;
    HI_U16  u16MeteringCrRefLowAwb;
    HI_U16  u16MeteringCbRefHighAwb;
    HI_U16  u16MeteringCbRefLowAwb;
} ISP_AWB_REG_CFG_2_S;

typedef struct hiISP_AWB_REG_CFG_3_S
{
    HI_BOOL bAboveWhiteLevelClip;
    HI_BOOL bBelowBlackLevelClip;
    HI_U16  u16MeteringWhiteLevelAwb;
    HI_U16  u16MeteringBlackLevelAwb;
    HI_U16  u16MeteringCrRefMaxAwb;
    HI_U16  u16MeteringCbRefMaxAwb;
    HI_U16  u16MeteringCrRefMinAwb;
    HI_U16  u16MeteringCbRefMinAwb;
    HI_U16  u16MeteringCrRefHighAwb;
    HI_U16  u16MeteringCrRefLowAwb;
    HI_U16  u16MeteringCbRefHighAwb;
    HI_U16  u16MeteringCbRefLowAwb;
} ISP_AWB_REG_CFG_3_S;

typedef struct hiISP_ACM_REG_CFG_S
{
    HI_U32  u32GainLuma; 
    HI_U32  u32GainHue; 
    HI_U32  u32GainSat; 
} ISP_ACM_REG_CFG_S;

typedef struct hiISP_AF_REG_CFG_S
{
    HI_U16  u16ThresholdWrite;
    HI_U8   u8MetricsShift;
    HI_U8   u8NpOffset;
} ISP_AF_REG_CFG_S;

typedef struct hiISP_OFFSET_REG_CFG_S
{
    HI_U16  au16BlackLevelOffset[4];
} ISP_OFFSET_REG_CFG_S;

typedef struct hiISP_DRC_REG_CFG_S
{
    HI_U32   u32Strength;
} ISP_DRC_REG_CFG_S;

typedef struct hiISP_DEHAZE_REG_CFG_S
{
    HI_U8   u8Strength;
    HI_U8   u8DehazeEnable;
    HI_U16  u16AirR;
	HI_U16  u16AirG;
	HI_U16  u16AirB;
    HI_U32  u32Update;
} ISP_DEHAZE_REG_CFG_S;

/*
typedef struct hiISP_GAMMA_REG_CFG_S
{
    HI_U16  au16GammaLut[257];
    HI_U8  u8GammaBankSel;
} ISP_GAMMA_REG_CFG_S;
*/

typedef struct hiISP_DP_REG_CFG_S
{
    HI_U8   u8DpEnable;
    HI_U8   u8DpPointerReset;
    HI_U8   u8DpDetectionTrigger;
    HI_U8   u8ShowStaticDefectPixels;
    HI_U16  u16DpSlope;
    HI_U16  u16DPThresh;
    HI_U16  u16DpCountIn;
} ISP_DP_REG_CFG_S;

typedef struct hiISP_NR_REG_CFG_S
{
    HI_U16  u16NpExpThresh;
    HI_U8   u8NpShortRatio;
    HI_U8   u8NpLongRatio;
    HI_U8   u8SinterThreshShort;
    HI_U8   u8SinterThreshLong;
    HI_U8   u8NpOffset;
} ISP_NR_REG_CFG_S;

typedef struct hiISP_SHARPEN_REG_CFG_S
{
    /* Bayer Sharpen */
    HI_BOOL bShpAlgSel;
    HI_U8   u8SharpAltD;
    HI_U8   u8SharpAltUd;
    HI_U16  u16LumThresh;
    HI_U8   u8SharpenHF;  
    HI_U8   u8SharpenMF;  
    HI_U8   u8SharpenLF;  
    HI_U8   u8SadAmplifier;

    /* Yuv Sharpen */
    HI_U8  G3[3][3];               /* mid-band template */
    HI_U8  enPixSel;               /* switch for selecting local max and min value in overshoot/undershoot control */
    HI_U16 u16SharpAmt;            /* mid-band sharpen strength */
    HI_U16 u16EdgeAmt;             /* high-band sharpen strength */
    HI_U16 u16SharpThd1;           /* threshold for sharp1 */
    HI_U16 u16SharpThd2;           /* threshold for sharp1 */
    HI_U16 u16SharpThd1MulCoef;    /* convert 1/(u16SharpThd1-u16SharpThd2) to multiplication  */
    HI_U16 u16EdgeThd1;            /* threshold for sharp1  */
    HI_U16 u16EdgeThd2;            /* threshold for sharp1  */
    HI_U16 u16EdgeThd1MulCoef;     /* convert 1/(u16EdgeThd1-u16EdgeThd2) to multiplication. */
    HI_U8  u8LineThd1;             /* threshold for line detection  */
    HI_U8  u8LineThd2;             /* threshold for line detection  */
    HI_U8  u8OvershootAmt;         /* overshoot amplitude  */
    HI_U8  u8UndershootAmt;        /* undershoot amplitude */
    HI_BOOL bEnJagCtrl;            // switch for jag control   // 1-bit unsigned integer  // U1.0
    HI_U16  u16VarJagThd1;            // threshold for jag control                           // 10-bit unsigned integer  // U10.0
    HI_U16  u16VarJagThd2;            // threshold for jag control                           // 10-bit unsigned integer  // U10.0
    HI_U16  u16EdgeJagAmt;            // attenuation of Edge amplitude for jag control           // 12-bit unsigned integer  // U12.0
    HI_S16  s16EdgeJagMulCoef;        // convert (EdgeJagAmt-edgeAmt)/(varJagThd2-varJagThd1) to multiplication.       // 13-bit signed integer // s13.0
    HI_U8   u8OshtJagAmt;            // amplify overshoot amplitude for jag control           // 8-bit unsigned integer  // U8.0
    HI_S16  s16OshtJagMulCoef;        // convert (OshtJagAmt-overshootAmt)/(varJagThd2-varJagThd1) to multiplication.    // 13-bit signed integer // s8.4
    HI_U8   u8UshtJagAmt;            // amplify undershoot amplitude for jag control           // 8-bit unsigned integer  // U8.0
    HI_S16  s16UshtJagMulCoef;        // convert (UshtJagAmt-undershootAmt)/(varJagThd2-varJagThd1) to multiplication.  // 13-bit signed integer // s8.4
    HI_S8   u8ShtMaxShft;            // shift for local max pixel, [-128, 127]                 // 8-bit signed integer    // S8.0
    HI_S8   u8ShtMinShft;            // shift for local min pixel, [-128, 127]                 // 8-bit signed integer    // S8.0

    // control sharpen based on luma
    HI_BOOL bEnLumaCtrl;           // switch for using local luma to control sharpen strength   // 1-bit unsigned integer  // U1.0
    HI_U8   u8LumaTh[4];             // local luma threshold, [0, 255]                            // 8-bit unsigned integer  // U8.0 x 4
    HI_U8   u8LumaWgt[4];            // weight of sharpen result, [0, 255]                        // 8-bit unsigned integer  // U8.0 x 4
    HI_S16  s16LumaMul[3];            // convert (lumaWgt[i+1]-lumaWgt[i])/(lumaTh[i+1]-lumaTh[i]) to multiplication.         // 12-bit signed integer  // S8.4 x 3

    // control shoot based on var
    HI_BOOL bEnShtCtrlVarMod;      // switch for using local var to control shoot                // 1-bit unsigned integer  // U1.0
    HI_U16  u16OShtVarTh[2];          // local var threshold, [0, 1023]                             // 10-bit unsigned integer // U10.0 x 2
    HI_U8   u8OShtVarAmt;            // overshoot amplitude , [0, 255]                             // 8-bit unsigned integer  // U8.0
    HI_S16  s16OShtVarMul;            // convert (oShtVarAmt-overshootAmt)/(oShtVarTh[i+1]-oShtVarTh[i]) to multiplication.    // 12-bit signed integer // S8.4
    HI_U16  u16UShtVarTh[2];          // local var threshold, [0, 1023]                             // 10-bit unsigned integer // U10.0 x 2 
    HI_U8   u8UShtVarAmt;            // undershoot amplitude, [0, 255]                             // 8-bit unsigned integer  // U8.0
    HI_S16  s16UShtVarMul;            // convert (uShtVarAmt-undershootAmt)/(uShtVarTh[i+1]-uShtVarTh[i]) to multiplication.   // 12-bit signed integer // S8.4

    // control shoot based on luma
    HI_BOOL bEnShtCtrlLumaMod;     // switch for using local luma to control shoot              // 1-bit unsigned integer        // U1.0
    HI_U8   u8OShtLumaTh[2];         // local luma threshold, [0, 255]                            // 8-bit unsigned integer        // U8.0 x 2
    HI_U8   u8OShtLumaWgt[2];        // weigth of shoot amt, [0, 255]                             // 8-bit unsigned integer        // U8.0
    HI_S16  s16OShtLumaMul;           // convert (oShtLumaWgt[1]-oShtLumaWgt[0])/(oShtLumaTh[1]-oShtLumaTh[0]) to multiplication.   // 12-bit signed integer // S8.4
    HI_U8   u8UShtLumaTh[2];         // local luma threshold, [0, 255]                            // 8-bit unsigned integer        // U8.0 x 2 
    HI_U8   u8UShtLumaWgt[2];        // weigth of shoot amt, [0, 255]                             // 8-bit unsigned integer        // U8.0
    HI_S16  s16UShtLumaMul;           // convert (uShtLumaWgt[1]-uShtLumaWgt[0])/(uShtLumaTh[1]-uShtLumaTh[0]) to multiplication.   // 12-bit signed integer // S8.4
    // coring
    HI_U8   u8Limit;                 // [0,15] // 4-bit unsigned integer    
} ISP_SHARPEN_REG_CFG_S;

typedef struct hiISP_GE_REG_CFG_S
{    
    HI_U8   u8GeStrength;
} ISP_GE_REG_CFG_S;

typedef struct hiISP_WDR_REG_CFG_S
{
    HI_U8   u8TopChannelSwitch;
} ISP_WDR_REG_CFG_S;

typedef union hiISP_REG_KERNEL_CFG_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1GammaCfg    : 1 ;   /* [0] */
        HI_U32  bit20Rsv        : 31;   /* [1:31] */
    };
} ISP_REG_KERNEL_CFG_KEY_U;

typedef struct hiISP_REG_KERNEL_CFG_S
{
    ISP_REG_KERNEL_CFG_KEY_U    unKey;
    //ISP_GAMMA_REG_CFG_S         stGammaRegCfg;
} ISP_REG_KERNEL_CFG_S;

typedef union hiISP_REG_CFG_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1AeCfg1      : 1 ;   /* [0] */
        HI_U32  bit1AeCfg2      : 1 ;   /* [1] */
        HI_U32  bit1AwbCfg1     : 1 ;   /* [2] */
        HI_U32  bit1AwbCfg2     : 1 ;   /* [3] */
        HI_U32  bit1AfCfg       : 1 ;   /* [4] */
        HI_U32  bit1OffsetCfg   : 1 ;   /* [5] */
        HI_U32  bit1DrcCfg      : 1 ;   /* [6] */
        HI_U32  bit1NrCfg       : 1 ;   /* [7] */
        HI_U32  bit1DpCfg       : 1 ;   /* [8] */
        HI_U32  bit1BayerShpCfg : 1 ;   /* [9] */
        HI_U32  bit1YuvShpCfg   : 1 ;   /* [10] */
        HI_U32  bit1GeCfg       : 1 ;   /* [11] */
        HI_U32  bit1WdrCfg      : 1 ;   /* [12] */
        HI_U32  bit1DehazeCfg   : 1 ;   /* [13] */
        HI_U32  bit1AwbCfg3     : 1 ;   /* [14] */
        HI_U32  bit1ACMCfg      : 1 ;   /* [15] */
        HI_U32  bit18Rsv        : 16;   /* [16:31] */
    };
}ISP_REG_CFG_KEY_U;

typedef struct hiISP_REG_CFG_S
{
    ISP_REG_CFG_KEY_U       unKey;

    ISP_AE_REG_CFG_1_S      stAeRegCfg1;
    ISP_AE_REG_CFG_2_S      stAeRegCfg2;
    ISP_AWB_REG_CFG_1_S     stAwbRegCfg1;
    ISP_AWB_REG_CFG_2_S     stAwbRegCfg2;
    ISP_AWB_REG_CFG_3_S     stAwbRegCfg3;
    ISP_AF_REG_CFG_S        stAfRegCfg;
    ISP_DP_REG_CFG_S        stDpRegCfg;
    ISP_DRC_REG_CFG_S       stDrcRegCfg;
    ISP_NR_REG_CFG_S        stNrRegCfg;
    ISP_GE_REG_CFG_S        stGeRegCfg;
    ISP_SHARPEN_REG_CFG_S   stSharpenRegCfg;
    ISP_OFFSET_REG_CFG_S    stOffsetRegCfg;
    ISP_WDR_REG_CFG_S       stWdrRegCfg;
    ISP_DEHAZE_REG_CFG_S    stDehazeRegCfg;
    ISP_REG_KERNEL_CFG_S    stKernelCfg;
    ISP_ACM_REG_CFG_S       stAcmRegCfg;
} ISP_REG_CFG_S;

#define ISP_SYNC_BUF_NODE_NUM 5
#define CFG2VLD_DLY_LIMIT 4
#define WDR_EXP_RATIO_SHIFT 6
#define DRC_COMP_SHIFT 8

#define IS_LINEAR_MODE(mode)      (WDR_MODE_NONE == (mode))
#define IS_WDR_MODE(mode)         ((!IS_LINEAR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_BUILT_IN_WDR_MODE(mode) (WDR_MODE_BUILT_IN == (mode))
#define IS_FS_WDR_MODE(mode)      ((!IS_LINEAR_MODE(mode))&& (!IS_BUILT_IN_WDR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_2to1_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME == (mode)) || (WDR_MODE_2To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_2To1_LINE == (mode)))
#define IS_3to1_WDR_MODE(mode)    ((WDR_MODE_3To1_FRAME == (mode)) || (WDR_MODE_3To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_3To1_LINE == (mode)))
#define IS_4to1_WDR_MODE(mode)    ((WDR_MODE_4To1_FRAME == (mode)) || (WDR_MODE_4To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_4To1_LINE == (mode)))
#define IS_FULL_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_3To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_4To1_FRAME_FULL_RATE == (mode)))
#define IS_HALF_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME == (mode)) || (WDR_MODE_3To1_FRAME == (mode)) || (WDR_MODE_4To1_FRAME == (mode)))
#define IS_LINE_WDR_MODE(mode)    ((WDR_MODE_2To1_LINE == (mode)) || (WDR_MODE_3To1_LINE == (mode)) || (WDR_MODE_4To1_LINE == (mode)))

typedef struct hiISP_SYNC_CFG_BUF_NODE_S
{
    HI_BOOL bValid;
    
    ISP_SNS_REGS_INFO_S stSnsRegsInfo;
    ISP_AE_REG_CFG_2_S  stAERegCfg;
    ISP_DRC_REG_CFG_S   stDRCRegCfg;
} ISP_SYNC_CFG_BUF_NODE_S;

typedef struct hiISP_REGCFG_S
{
    HI_BOOL bInit;
    ISP_REG_CFG_S stRegCfg;
    ISP_SYNC_CFG_BUF_NODE_S stSyncCfgNode;
} ISP_REGCFG_S;

typedef struct hiISP_WDR_CFG_S
{
    HI_U8 u8WDRMode;
    HI_U32 au32ExpRatio[EXP_RATIO_NUM];
} ISP_WDR_CFG_S;

typedef struct hiISP_RES_SWITCH_MODE_S
{
    HI_U16   u16Width;
    HI_U16   u16Height;
    HI_FLOAT f32Fps;
}ISP_RES_SWITCH_MODE_S;


typedef struct hiISP_PROC_MEM_S
{
    HI_U32 u32ProcSize;
    HI_U32 u32ProcPhyAddr;
    HI_VOID *pProcVirtAddr;
} ISP_PROC_MEM_S;

typedef struct hi_ISP_ACM_MMZ_S
{
    HI_U32      u32AcmSize  ;
    HI_VOID*    pVirAddr    ;
    HI_U32      u32PhyAddr  ;
}ISP_ACM_BUFFER_S;


#define ISP_1ST_INT                0x1
#define ISP_2ND_INT                0x2
#define ISP_UNDEF_INT              0xF4
                                   
#define ISP_DEV_SET_FD             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, HI_S32)
#define ISP_GET_FRAME_EDGE         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, HI_S32)
#define ISP_GET_VD_TIMEOUT         _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_GET_VD_END_TIMEOUT     _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_END_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_SET_INT_ENABLE         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, HI_BOOL)
#define ISP_GET_VERSION            _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT          _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, HI_U32)
#define ISP_STAT_BUF_EXIT          _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, ISP_STAT_INFO_S)
#define ISP_STAT_BUF_PUT           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, ISP_STAT_INFO_S)
#define ISP_STAT_ACT_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_ACT_GET, ISP_STAT_INFO_S)
#define ISP_REG_CFG_INIT           _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, ISP_REG_CFG_S)
#define ISP_REG_CFG_SET            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, ISP_REG_KERNEL_CFG_S)
#define ISP_MEM_INFO_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, HI_BOOL)
#define ISP_MEM_INFO_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, HI_BOOL)
#define ISP_CAP_INFO_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CAP_INFO_SET, ISP_CONFIG_INFO_S)
#define ISP_CAP_INFO_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_CAP_INFO_GET, ISP_CONFIG_INFO_S)
#define ISP_DNG_INFO_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_SET, DNG_IMAGE_DYNAMIC_INFO_S)
#define ISP_PRO_TRIGGER_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_EN_GET, HI_BOOL)
#define ISP_PRO_EXP_ATTR_SET       _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PRO_PARAM_SET, ISP_SNAP_ATTR_S)
#define ISP_MODE_INFO              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MODE_INFO, ISP_MODE_E)
                                   
#define ISP_PROC_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, ISP_PROC_MEM_S)
#define ISP_PROC_WRITE_ING         _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK          _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, HI_U32)
#define ISP_UPDATE_POS_GET         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_POS_GET, HI_U32)
#define ISP_UPDATEINFO_BUF_INIT    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATEINFO_BUF_INIT, HI_U32)
#define ISP_UPDATEINFO_BUF_EXIT    _IO(IOC_TYPE_ISP, IOC_NR_ISP_UPDATEINFO_BUF_EXIT)
#define ISP_FRAME_INFO_SET         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_SET, FRAME_SUPPLEMENT_INFO_S)
#define ISP_FRAME_INFO_GET         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_GET, FRAME_SUPPLEMENT_INFO_S)
#define ISP_FRAME_INFO_BUF_INIT    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_BUF_INIT, HI_U32)
#define ISP_FRAME_INFO_BUF_EXIT    _IO(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_BUF_EXIT)
#define ISP_DNG_INFO_GET         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_GET, DNG_IMAGE_STATIC_INFO_S)
#define ISP_DNG_INFO_BUF_INIT    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_BUF_INIT, HI_U32)
#define ISP_DNG_INFO_BUF_EXIT    _IO(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_BUF_EXIT)
                                   
#define ISP_SYNC_CFG_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, ISP_SYNC_CFG_BUF_NODE_S)
#define ISP_WDR_CFG_SET            _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, ISP_WDR_CFG_S)
#define ISP_RES_SWITCH_SET         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET, ISP_RES_SWITCH_MODE_S)
                                   
#define ISP_ACM_ATTR_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_GET, ISP_ACM_ATTR_S)
#define ISP_ACM_ATTR_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_SET, ISP_ACM_ATTR_S)


#define ISP_ACM_LUT_PHY_ADDR_GET   _IOR(IOC_TYPE_ISP, IOC_NR_ACM_LUT_PHY_ADDR_GET, ISP_ACM_BUFFER_S)
#define ISP_ACM_COEF_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ACM_COEF_SET, ISP_ACM_BUFFER_S)

#define ISP_PWM_NUM_GET            _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, HI_U32)

#define ISP_SET_MOD_PARAM      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_MOD_PARAM      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_DEV(f)             (*((HI_U32*)(f)))

#define ISP_CHECK_DEV(dev)\
do {\
    if (((dev) < 0) || ((dev) >= ISP_MAX_DEV_NUM))\
    {\
        ISP_TRACE(HI_DBG_ERR, "Err isp dev %d!\n", dev);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)    

#define ISP_CHECK_ISP_STATBUFF_INIT(dev)\
do{\
    if (g_astIspDrvCtx[dev].stStatisticsBuf.bInit != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] statistics buffer doesn't initialized!\n", dev);\
        return HI_ERR_ISP_NOT_INIT;\
    }\
}while(0)

#define ISP_CHECK_ISP_UPDATE_INFO_INIT(dev)\
do{\
    if (g_astIspDrvCtx[dev].bUpdateInfoInit != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] UpdateInfo doesn't initialized!\n", dev);\
        return HI_ERR_ISP_NOT_INIT;\
    }\
}while(0)

#define ISP_CHECK_ISP_FRAME_INFO_INIT(dev)\
do{\
    if (g_astIspDrvCtx[dev].bFrameInfoInit != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] FrameInfo doesn't initialized!\n", dev);\
        return HI_ERR_ISP_NOT_INIT;\
    }\
}while(0)

#define ISP_CHECK_ISP_DNG_INFO_INIT(dev)\
do{\
    if (g_astIspDrvCtx[dev].bDngInfoInit != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] DngInfo doesn't initialized!\n", dev);\
        return HI_ERR_ISP_NOT_INIT;\
    }\
}while(0)

#define ISP_CHECK_POINTER(ptr)\
do {\
    if (HI_NULL == ptr)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Null Pointer!\n");\
        return HI_ERR_ISP_NULL_PTR;\
    }\
}while(0)

#define ISP_CHECK_BOOL(bool)\
do {\
    if (( HI_TRUE != bool)&&( HI_FALSE != bool))\
    {\
        ISP_TRACE(HI_DBG_ERR, "Invalid Isp Bool Type %d!\n", bool);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)

#define ISP_DBG(bEn, fmt...)\
do{\
    if (bEn)\
        printf(fmt);\
}while(0)

#endif /* __HI3518_ISP_DEF_H__ */

