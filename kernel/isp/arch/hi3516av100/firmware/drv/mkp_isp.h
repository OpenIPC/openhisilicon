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

typedef enum hiIOC_NR_ISP_E
{
    IOC_NR_ISP_SET_FD = 0,
    IOC_NR_ISP_GET_FRAME_EDGE,
    IOC_NR_ISP_GET_VD_TIMEOUT,
    IOC_NR_ISP_SET_INT_ENABLE,
    IOC_NR_ISP_GET_VERSION,

    IOC_NR_ISP_STAT_BUF_INIT,
    IOC_NR_ISP_STAT_BUF_EXIT,
    IOC_NR_ISP_STAT_BUF_GET,
    IOC_NR_ISP_STAT_BUF_PUT,
    IOC_NR_ISP_REG_CFG_INIT,
    IOC_NR_ISP_REG_CFG_SET,
    IOC_NR_ISP_PROC_INIT,
    IOC_NR_ISP_PROC_WRITE_ING,
    IOC_NR_ISP_PROC_WRITE_OK,
    IOC_NR_ISP_PROC_EXIT,
    IOC_NR_ISP_PROC_PARAM_GET,
    IOC_NR_ISP_DCF_INFO_SET,
    IOC_NR_ISP_DCF_INFO_GET,
    IOC_NR_ISP_SYNC_CFG_SET,
    IOC_NR_WDR_SYNC_CFG_SET,
    IOC_NR_ISP_DCFINFO_BUF_INIT,
    IOC_NR_ISP_DCFINFO_BUF_EXIT,
    IOC_NR_ACM_ATTR_GET,
    IOC_NR_ACM_ATTR_SET,
    IOC_NR_ACM_COEF_GET,
    IOC_NR_ACM_COEF_SET,
    IOC_NR_ISP_MEM_INFO_SET,
    IOC_NR_ISP_MEM_INFO_GET,
    IOC_NR_ISP_PWM_NUM_GET,
    IOC_NR_ISP_RES_SWITCH_SET,
    IOC_NR_ISP_SET_MOD_PARAM,
    IOC_NR_ISP_GET_MOD_PARAM,

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
    HI_U32  au32DehazeMinDout[DEFOG_ZONE_NUM/4];
} ISP_DEHAZE_STAT_S;


typedef struct hiISP_COMM_STAT_S
{
    HI_U16  au16WhiteBalanceGain[4];
} ISP_COMM_STAT_S;

typedef struct hiISP_STAT_S
{
    ISP_AE_STAT_1_S     stAeStat1;
    ISP_AE_STAT_2_S     stAeStat2;
    ISP_AE_STAT_3_S     stAeStat3;
    ISP_AE_STAT_4_S     stAeStat4;
    ISP_AE_STAT_5_S     stAeStat5;
    ISP_AWB_STAT_1_S    stAwbStat1;
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_AWB_STAT_3_S    stAwbStat3;
    ISP_AWB_STAT_4_S    stAwbStat4;
    ISP_AF_STAT_S       stAfStat;
    ISP_DP_STAT_S       stDpStat;
    ISP_COMM_STAT_S     stCommStat;
    ISP_DEHAZE_STAT_S   stDehazeStat;
} ISP_STAT_S;

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
        HI_U32  bit1AwbStat1    : 1 ;   /* [5] */
        HI_U32  bit1AwbStat2    : 1 ;   /* [6] */
        HI_U32  bit1AwbStat3    : 1 ;   /* [7] */
        HI_U32  bit1AwbStat4    : 1 ;   /* [8] */
        HI_U32  bit1AfStat      : 1 ;   /* [9] */
        HI_U32  bit1DpStat      : 1 ;   /* [10] */
        HI_U32  bit1CommStat    : 1 ;   /* [11] */
        HI_U32  bit1Dehaze      : 1 ;   /* [12] */
        HI_U32  bit19Rsv        : 19;   /* [13:31] */
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
    HI_U32  u32DehazeAir;
    HI_U32  u32Update;
} ISP_DEHAZE_REG_CFG_S;

typedef struct hiISP_GAMMA_REG_CFG_S
{
    HI_U16  au16GammaLut[257];
} ISP_GAMMA_REG_CFG_S;

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
    HI_U8   u8SharpAltD;
    HI_U8   u8SharpAltUd;
    HI_U16  u16LumThresh;
    HI_U8   u8RGBsharpshrength;
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
    ISP_GAMMA_REG_CFG_S         stGammaRegCfg;
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
        HI_U32  bit1SharpenCfg  : 1 ;   /* [9] */
        HI_U32  bit1GeCfg       : 1 ;   /* [10] */
        HI_U32  bit1WdrCfg      : 1 ;   /* [11] */
        HI_U32  bit1DehazeCfg   : 1 ;   /* [12] */
        HI_U32  bit1AwbCfg3     : 1 ;   /* [13] */
        HI_U32  bit18Rsv        : 18;   /* [14:31] */
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

#define ISP_1ST_INT             0x1
#define ISP_2ND_INT             0x2
#define ISP_UNDEF_INT           0xF4

#define ISP_DEV_SET_FD          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, HI_S32)
#define ISP_GET_FRAME_EDGE      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, HI_S32)
#define ISP_GET_VD_TIMEOUT      _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_SET_INT_ENABLE      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, HI_BOOL)
#define ISP_GET_VERSION         _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT       _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, HI_U32)
#define ISP_STAT_BUF_EXIT       _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, ISP_STAT_INFO_S)
#define ISP_STAT_BUF_PUT        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, ISP_STAT_INFO_S)
#define ISP_REG_CFG_INIT        _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, ISP_REG_CFG_S)
#define ISP_REG_CFG_SET         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, ISP_REG_KERNEL_CFG_S)
#define ISP_MEM_INFO_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, HI_BOOL)
#define ISP_MEM_INFO_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, HI_BOOL)

#define ISP_PROC_INIT           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, ISP_PROC_MEM_S)
#define ISP_PROC_WRITE_ING      _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK       _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT           _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, HI_U32)
#define ISP_DCF_INFO_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_DCF_INFO_SET, ISP_SNS_REGS_INFO_S)
#define ISP_DCF_INFO_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DCF_INFO_GET, ISP_SNS_REGS_INFO_S)
#define ISP_DCFINFO_BUF_INIT    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DCFINFO_BUF_INIT, HI_U32)
#define ISP_DCFINFO_BUF_EXIT    _IO(IOC_TYPE_ISP, IOC_NR_ISP_DCFINFO_BUF_EXIT)

#define ISP_SYNC_CFG_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, ISP_SYNC_CFG_BUF_NODE_S)
#define ISP_WDR_CFG_SET         _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, ISP_WDR_CFG_S)
#define ISP_RES_SWITCH_SET      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET, ISP_RES_SWITCH_MODE_S)
#define ISP_ACM_ATTR_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_GET, ISP_ACM_ATTR_S)
#define ISP_ACM_ATTR_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_SET, ISP_ACM_ATTR_S)
#define ISP_ACM_COEF_GET        _IOWR(IOC_TYPE_ISP, IOC_NR_ACM_COEF_GET, ISP_ACM_COEF_S)
#define ISP_ACM_COEF_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ACM_COEF_SET, ISP_ACM_COEF_S)

#define ISP_PWM_NUM_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, HI_U32)

#define ISP_SET_MOD_PARAM      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_MOD_PARAM      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_DEV(f)          ((HI_U32)((f)->private_data))

#define ISP_CHECK_DEV(dev)\
do {\
    if (0 != dev)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Err isp dev %d!\n", dev);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)

#define ISP_CHECK_ISP_STATBUFF_INIT(dev)\
do{\
	if (g_astIspDrvCtx[dev].stStatisticsBuf.bInit != HI_TRUE)\
	{\
		ISP_TRACE(HI_DBG_ERR, "Isp[%d] statistics buffer doesn't initialized in %s!\n", dev, __FUNCTION__);\
		return HI_ERR_ISP_MEM_NOT_INIT;\
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

