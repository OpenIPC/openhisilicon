/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __COMM_AIO_H__
#define __COMM_AIO_H__

#include "common.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define MAX_AUDIO_FRAME_NUM    300
#define MAX_AUDIO_POINT_BYTES  4

#define MAX_VOICE_POINT_NUM    480

#define MAX_AUDIO_POINT_NUM    2048
#define MAX_AO_POINT_NUM       4096
#define MIN_AUDIO_POINT_NUM    80
#define MAX_AI_POINT_NUM    2048

#define MAX_AUDIO_FRAME_LEN    (MAX_AUDIO_POINT_BYTES*MAX_AO_POINT_NUM)

#define MAX_AUDIO_STREAM_LEN   MAX_AUDIO_FRAME_LEN

#define MAX_AI_USRFRM_DEPTH     30

#define MAX_AUDIO_FILE_PATH_LEN 256
#define MAX_AUDIO_FILE_NAME_LEN 256

#define VQE_EQ_BAND_NUM  10
#define VQE_DRC_SECNUM 5

#define TALKVQEV2_EQ_BAND_NUM   30

#define AI_TALKVQEV2_MASK_PNR       0x1
#define AI_TALKVQEV2_MASK_NR        0x2
#define AI_TALKVQEV2_MASK_EQ        0x4
#define AI_TALKVQEV2_MASK_AGC       0x8
#define AI_TALKVQEV2_MASK_DRC       0x10
#define AI_TALKVQEV2_MASK_LIMITER   0x20
#define AI_TALKVQEV2_MASK_DEREVERB  0x40
#define AI_TALKVQEV2_MASK_FMP       0x80
#define AI_TALKVQEV2_MASK_WNR       0x100

#define AI_RECORDVQE_MASK_HPF       0x1
#define AI_RECORDVQE_MASK_RNR       0x2
#define AI_RECORDVQE_MASK_HDR       0x4
#define AI_RECORDVQE_MASK_DRC       0x8
#define AI_RECORDVQE_MASK_EQ        0x10
#define AI_RECORDVQE_MASK_AGC       0x20

#define AI_TALKVQE_MASK_HPF     0x1
#define AI_TALKVQE_MASK_AEC     0x2
#define AI_TALKVQE_MASK_AGC     0x8
#define AI_TALKVQE_MASK_EQ      0x10
#define AI_TALKVQE_MASK_ANR     0x20

#define AO_VQE_MASK_HPF         0x1
#define AO_VQE_MASK_ANR         0x2
#define AO_VQE_MASK_AGC         0x4
#define AO_VQE_MASK_EQ          0x8


typedef enum AUDIO_SAMPLE_RATE_E {
    AUDIO_SAMPLE_RATE_8000   = 8000,
    AUDIO_SAMPLE_RATE_12000  = 12000,
    AUDIO_SAMPLE_RATE_11025  = 11025,
    AUDIO_SAMPLE_RATE_16000  = 16000,
    AUDIO_SAMPLE_RATE_22050  = 22050,
    AUDIO_SAMPLE_RATE_24000  = 24000,
    AUDIO_SAMPLE_RATE_32000  = 32000,
    AUDIO_SAMPLE_RATE_44100  = 44100,
    AUDIO_SAMPLE_RATE_48000  = 48000,
    AUDIO_SAMPLE_RATE_64000  = 64000,
    AUDIO_SAMPLE_RATE_96000  = 96000,
    AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E;

typedef enum AUDIO_BIT_WIDTH_E {
    AUDIO_BIT_WIDTH_8   = 0,
    AUDIO_BIT_WIDTH_16  = 1,
    AUDIO_BIT_WIDTH_24  = 2,
    AUDIO_BIT_WIDTH_BUTT,
} AUDIO_BIT_WIDTH_E;

typedef enum AIO_MODE_E {
    AIO_MODE_I2S_MASTER  = 0,
    AIO_MODE_I2S_SLAVE,
    AIO_MODE_PCM_SLAVE_STD,
    AIO_MODE_PCM_SLAVE_NSTD,
    AIO_MODE_PCM_MASTER_STD,
    AIO_MODE_PCM_MASTER_NSTD,
    AIO_MODE_BUTT
} AIO_MODE_E;

typedef enum {
    AIO_I2STYPE_INNERCODEC = 0,
    AIO_I2STYPE_INNERHDMI,
    AIO_I2STYPE_EXTERN,
} AIO_I2STYPE_E;

typedef enum AIO_SOUND_MODE_E {
    AUDIO_SOUND_MODE_MONO   = 0,
    AUDIO_SOUND_MODE_STEREO = 1,
    AUDIO_SOUND_MODE_BUTT
} AUDIO_SOUND_MODE_E;


typedef enum G726_BPS_E {
    G726_16K = 0,
    G726_24K,
    G726_32K,
    G726_40K,
    MEDIA_G726_16K,
    MEDIA_G726_24K,
    MEDIA_G726_32K,
    MEDIA_G726_40K,
    G726_BUTT,
} G726_BPS_E;

typedef enum ADPCM_TYPE_E {
    ADPCM_TYPE_DVI4 = 0,
    ADPCM_TYPE_IMA,
    ADPCM_TYPE_ORG_DVI4,
    ADPCM_TYPE_BUTT,
} ADPCM_TYPE_E;

#define AI_EXPAND  0x01
#define AI_CUT     0x02

typedef struct AIO_ATTR_S {
    AUDIO_SAMPLE_RATE_E enSamplerate;
    AUDIO_BIT_WIDTH_E   enBitwidth;
    AIO_MODE_E          enWorkmode;
    AUDIO_SOUND_MODE_E  enSoundmode;
    GK_U32              u32EXFlag;
    GK_U32              u32FrmNum;
    GK_U32              u32PtNumPerFrm;
    GK_U32              u32ChnCnt;
    GK_U32              u32ClkSel;
    AIO_I2STYPE_E       enI2sType;
} AIO_ATTR_S;

typedef struct AI_CHN_PARAM_S {
    GK_U32 u32UsrFrmDepth;
} AI_CHN_PARAM_S;

typedef struct AUDIO_FRAME_S {
    AUDIO_BIT_WIDTH_E   enBitwidth;
    AUDIO_SOUND_MODE_E  enSoundmode;
    GK_U8*  u64VirAddr[2];
    GK_U64  u64PhyAddr[2];
    GK_U64  u64TimeStamp;
    GK_U32  u32Seq;
    GK_U32  u32Len;
    GK_U32  u32PoolId[2];
} AUDIO_FRAME_S;

typedef struct AEC_FRAME_S {
    AUDIO_FRAME_S   stRefFrame;
    GK_BOOL         bValid;
    GK_BOOL         bSysBind;
} AEC_FRAME_S;


typedef struct AUDIO_FRAME_INFO_S {
    AUDIO_FRAME_S *pstFrame;
    GK_U32         u32Id;
} AUDIO_FRAME_INFO_S;

typedef struct AUDIO_STREAM_S {
    GK_U8 ATTRIBUTE *pStream;
    GK_U64 ATTRIBUTE u64PhyAddr;
    GK_U32 u32Len;
    GK_U64 u64TimeStamp;
    GK_U32 u32Seq;
} AUDIO_STREAM_S;


typedef struct AO_CHN_STATE_S {
    GK_U32                  u32ChnTotalNum;
    GK_U32                  u32ChnFreeNum;
    GK_U32                  u32ChnBusyNum;
} AO_CHN_STATE_S;

typedef enum AUDIO_TRACK_MODE_E {
    AUDIO_TRACK_NORMAL      = 0,
    AUDIO_TRACK_BOTH_LEFT   = 1,
    AUDIO_TRACK_BOTH_RIGHT  = 2,
    AUDIO_TRACK_EXCHANGE    = 3,
    AUDIO_TRACK_MIX         = 4,
    AUDIO_TRACK_LEFT_MUTE   = 5,
    AUDIO_TRACK_RIGHT_MUTE  = 6,
    AUDIO_TRACK_BOTH_MUTE   = 7,

    AUDIO_TRACK_BUTT
} AUDIO_TRACK_MODE_E;


typedef enum AUDIO_FADE_RATE_E {
    AUDIO_FADE_RATE_1   = 0,
    AUDIO_FADE_RATE_2   = 1,
    AUDIO_FADE_RATE_4   = 2,
    AUDIO_FADE_RATE_8   = 3,
    AUDIO_FADE_RATE_16  = 4,
    AUDIO_FADE_RATE_32  = 5,
    AUDIO_FADE_RATE_64  = 6,
    AUDIO_FADE_RATE_128 = 7,

    AUDIO_FADE_RATE_BUTT
} AUDIO_FADE_RATE_E;

typedef struct AUDIO_FADE_S {
    GK_BOOL         bFade;
    AUDIO_FADE_RATE_E enFadeInRate;
    AUDIO_FADE_RATE_E enFadeOutRate;
} AUDIO_FADE_S;

typedef struct AI_AEC_CONFIG_S {
    GK_BOOL bUsrMode;
    GK_S8 s8CngMode;
    GK_S8 s8NearAllPassEnergy;
    GK_S8 s8NearCleanSupEnergy;

    GK_S16 s16DTHnlSortQTh;

    GK_S16 s16EchoBandLow;
    GK_S16 s16EchoBandHigh;
    GK_S16 s16EchoBandLow2;
    GK_S16 s16EchoBandHigh2;

    GK_S16 s16ERLBand[6];
    GK_S16 s16ERL[7];
    GK_S16 s16VioceProtectFreqL;
    GK_S16 s16VioceProtectFreqL1;
    GK_S32 s32Reserved;
} AI_AEC_CONFIG_S;

typedef struct AUDIO_ANR_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S16 s16NrIntensity;
    GK_S16 s16NoiseDbThr;
    GK_S8  s8SpProSwitch;

    GK_S32 s32Reserved;
} AUDIO_ANR_CONFIG_S;

typedef struct AUDIO_AGC_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S8 s8TargetLevel;
    GK_S8 s8NoiseFloor;
    GK_S8 s8MaxGain;
    GK_S8 s8AdjustSpeed;

    GK_S8 s8ImproveSNR;
    GK_S8 s8UseHighPassFilt;
    GK_S8 s8OutputMode;
    GK_S16 s16NoiseSupSwitch;


    GK_S32 s32Reserved;
} AUDIO_AGC_CONFIG_S;

typedef enum AUDIO_HPF_FREQ_E {
    AUDIO_HPF_FREQ_80   = 80,
    AUDIO_HPF_FREQ_120  = 120,
    AUDIO_HPF_FREQ_150  = 150,
    AUDIO_HPF_FREQ_BUTT,
} AUDIO_HPF_FREQ_E;

typedef struct AUDIO_HPF_CONFIG_S {
    GK_BOOL bUsrMode;
    AUDIO_HPF_FREQ_E enHpfFreq;
} AUDIO_HPF_CONFIG_S;

typedef struct AI_RNR_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S32  s32NrMode;

    GK_S32 s32MaxNrLevel;

    GK_S32  s32NoiseThresh;
} AI_RNR_CONFIG_S;

typedef struct AUDIO_EQ_CONFIG_S {
    GK_S8  s8GaindB[VQE_EQ_BAND_NUM];
    GK_S32 s32Reserved;
} AUDIO_EQ_CONFIG_S;


typedef enum VQE_WORKSTATE_E {
    VQE_WORKSTATE_COMMON  = 0,
    VQE_WORKSTATE_MUSIC   = 1,
    VQE_WORKSTATE_NOISY   = 2,
} VQE_WORKSTATE_E;

typedef enum VQE_RECORD_TYPE {
    VQE_RECORD_NORMAL        = 0,
    VQE_RECORD_BUTT,
} VQE_RECORD_TYPE;


typedef GK_S32 (*pFuncGainCallBack)(GK_S32 s32SetGain);

typedef struct AI_HDR_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S32 s32MinGaindB;
    GK_S32 s32MaxGaindB;

    GK_S32 s32MicGaindB;
    GK_S32 s32MicGainStepdB;
    pFuncGainCallBack pcallback;
} AI_HDR_CONFIG_S;

typedef struct AI_DRC_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S16  s16AttackTime;
    GK_S16  s16ReleaseTime;

    GK_S16 s16OldLevDb[VQE_DRC_SECNUM];
    GK_S16 s16NewLevDb[VQE_DRC_SECNUM];
} AI_DRC_CONFIG_S;

typedef struct AI_TALKV2_EQ_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S8  s8GaindB[TALKVQEV2_EQ_BAND_NUM];
} AI_TALKV2_EQ_CONFIG_S;

typedef struct AI_TALKV2_PNR_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S16 s16MinGainLimit;
    GK_S16 s16SnrPriorLimit;
    GK_S16 s16HtThreshold;
    GK_S16 s16HsThreshold;
    GK_S16 s16AlphaPh;
    GK_S16 s16AlphaPsd;
    GK_S16 s16PriorSnrFixed;
    GK_S16 s16CepThreshold;
    GK_S16 s16SpeechProtectThreshold;
    GK_S16 s16HemEnable;
    GK_S16 s16TcsEnable;

    GK_S16 s16Reserved[3];
} AI_TALKV2_PNR_CONFIG_S;

typedef struct AI_TALKV2_NR_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S16 s16MinGainLimit;
    GK_S16 s16SnrPriorLimit;
    GK_S16 s16HtThreshold;
    GK_S16 s16HsThreshold;
    GK_S16 s16PriNoiseEstiMode;
    GK_S16 s16PriorSnrFixed;
    GK_S16 s16PrioSnrThreshold;
    GK_S16 s16SmPrioSnrThreshold;

    GK_S16 s16Reserved[16];
} AI_TALKV2_NR_CONFIG_S;

typedef struct AI_TALKV2_AGC_CONFIG_S {
    GK_BOOL bUsrMode;

    GK_S8 s8TargetLevel;
    GK_U8 u8MaxGain;
    GK_S8 s8MinGain;
    GK_S8 s8AdjustSpeed;

    GK_S16 s16Reserved[3];
} AI_TALKV2_AGC_CONFIG_S;

typedef struct AI_TALKV2_WNR_CONFIG_S {
    GK_S8 s8Reserved[6];
} AI_TALKV2_WNR_CONFIG_S;

typedef struct AI_TALKVQEV2_CONFIG_S {
    GK_U32            u32OpenMask;

    GK_S32            s32WorkSampleRate;
    GK_S32            s32FrameSample;
    VQE_WORKSTATE_E   enWorkstate;

    GK_S32            s32InChNum;
    GK_S32            s32OutChNum;

    AI_TALKV2_PNR_CONFIG_S  stPnrCfg;
    AI_TALKV2_NR_CONFIG_S   stNrCfg;
    AI_TALKV2_EQ_CONFIG_S   stEqCfg;
    AI_TALKV2_AGC_CONFIG_S  stAgcCfg;

    GK_BOOL bReserved0;
    GK_S16  s16Reserved1[19];
    GK_U8   u8Reserved2[8];

    AI_TALKV2_WNR_CONFIG_S  stWnrCfg;
} AI_TALKVQEV2_CONFIG_S;

typedef struct AI_RECORDVQE_CONFIG_S {
    GK_U32              u32OpenMask;

    GK_S32              s32WorkSampleRate;
    GK_S32              s32FrameSample;
    VQE_WORKSTATE_E     enWorkstate;

    GK_S32                s32InChNum;
    GK_S32                s32OutChNum;
    VQE_RECORD_TYPE       enRecordType;

    AUDIO_HPF_CONFIG_S  stHpfCfg;
    AI_RNR_CONFIG_S     stRnrCfg;
    AI_HDR_CONFIG_S     stHdrCfg;
    AI_DRC_CONFIG_S     stDrcCfg;
    AUDIO_EQ_CONFIG_S   stEqCfg;
    AUDIO_AGC_CONFIG_S  stAgcCfg;
} AI_RECORDVQE_CONFIG_S;

typedef struct AI_TALKVQE_CONFIG_S {
    GK_U32              u32OpenMask;

    GK_S32              s32WorkSampleRate;
    GK_S32              s32FrameSample;
    VQE_WORKSTATE_E     enWorkstate;

    AUDIO_HPF_CONFIG_S  stHpfCfg;
    AI_AEC_CONFIG_S     stAecCfg;
    AUDIO_ANR_CONFIG_S  stAnrCfg;
    AUDIO_AGC_CONFIG_S  stAgcCfg;
    AUDIO_EQ_CONFIG_S   stEqCfg;
} AI_TALKVQE_CONFIG_S;

typedef struct AO_VQE_CONFIG_S {
    GK_U32              u32OpenMask;

    GK_S32              s32WorkSampleRate;
    GK_S32              s32FrameSample;
    VQE_WORKSTATE_E     enWorkstate;

    AUDIO_HPF_CONFIG_S stHpfCfg;
    AUDIO_ANR_CONFIG_S stAnrCfg;
    AUDIO_AGC_CONFIG_S stAgcCfg;
    AUDIO_EQ_CONFIG_S  stEqCfg;
} AO_VQE_CONFIG_S;

typedef struct VQE_MODULE_CONFIG_S {
    GK_VOID *pHandle;
} VQE_MODULE_CONFIG_S;

typedef struct AUDIO_VQE_REGISTER_S {
    VQE_MODULE_CONFIG_S stResModCfg;
    VQE_MODULE_CONFIG_S stHpfModCfg;
    VQE_MODULE_CONFIG_S stHdrModCfg;
    VQE_MODULE_CONFIG_S stGainModCfg;

    VQE_MODULE_CONFIG_S stRecordModCfg;

    VQE_MODULE_CONFIG_S stAecModCfg;
    VQE_MODULE_CONFIG_S stAnrModCfg;
    VQE_MODULE_CONFIG_S stAgcModCfg;
    VQE_MODULE_CONFIG_S stEqModCfg;

    VQE_MODULE_CONFIG_S stRnrModCfg;
    VQE_MODULE_CONFIG_S stDrcModCfg;
    VQE_MODULE_CONFIG_S stPeqModCfg;

    VQE_MODULE_CONFIG_S stTalkV2ModCfg;
    VQE_MODULE_CONFIG_S stWnrModCfg;
} AUDIO_VQE_REGISTER_S;

typedef struct AUDIO_SAVE_FILE_INFO_S {
    GK_BOOL     bCfg;
    GK_CHAR     aFilePath[MAX_AUDIO_FILE_PATH_LEN];
    GK_CHAR     aFileName[MAX_AUDIO_FILE_NAME_LEN];
    GK_U32      u32FileSize;
} AUDIO_SAVE_FILE_INFO_S;

typedef struct AUDIO_FILE_STATUS_S {
    GK_BOOL     bSaving;
} AUDIO_FILE_STATUS_S;

typedef enum AUDIO_CLKSEL_E {
    AUDIO_CLKSEL_BASE       = 0,
    AUDIO_CLKSEL_SPARE,

    AUDIO_CLKSEL_BUTT,
} AUDIO_CLKSEL_E;

typedef struct AUDIO_MOD_PARAM_S {
    AUDIO_CLKSEL_E enClkSel;
} AUDIO_MOD_PARAM_S;


typedef enum EN_AIO_ERR_CODE_E {
    AIO_ERR_VQE_ERR        = 65,
} EN_AIO_ERR_CODE_E;

#define ERR_CODE_AIO_ILLEGAL_PARAM     DEFINE_ERR_CODE(MOD_ID_AIO, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_AIO_NULL_PTR          DEFINE_ERR_CODE(MOD_ID_AIO, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_AIO_NOT_PERM          DEFINE_ERR_CODE(MOD_ID_AIO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_AIO_REGISTER_ERR      DEFINE_ERR_CODE(MOD_ID_AIO, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

#define ERR_CODE_AI_INVALID_DEVID     DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define ERR_CODE_AI_INVALID_CHNID     DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_AI_ILLEGAL_PARAM     DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_AI_NULL_PTR          DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_AI_NOT_CONFIG        DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define ERR_CODE_AI_NOT_SUPPORT       DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_AI_NOT_PERM          DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_AI_NOT_ENABLED       DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_AI_NOMEM             DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_AI_NOBUF             DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_AI_BUF_EMPTY         DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_AI_BUF_FULL          DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_AI_SYS_NOTREADY      DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define ERR_CODE_AI_BUSY              DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_AI_VQE_ERR       DEFINE_ERR_CODE(MOD_ID_AI, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)

#define ERR_CODE_AO_INVALID_DEVID     DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define ERR_CODE_AO_INVALID_CHNID     DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_AO_ILLEGAL_PARAM     DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_AO_NULL_PTR          DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_AO_NOT_CONFIG        DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define ERR_CODE_AO_NOT_SUPPORT       DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_AO_NOT_PERM          DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define ERR_CODE_AO_NOT_ENABLED       DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define ERR_CODE_AO_NOMEM             DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_AO_NOBUF             DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define ERR_CODE_AO_BUF_EMPTY         DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_AO_BUF_FULL          DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_AO_SYS_NOTREADY      DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define ERR_CODE_AO_BUSY              DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_AO_VQE_ERR       DEFINE_ERR_CODE(MOD_ID_AO, EN_ERR_LEVEL_ERROR, AIO_ERR_VQE_ERR)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

