/******************************************************************************

  Copyright (C), 2021, Dmitry Ilyin, OpenIPC

 ******************************************************************************
  File Name     : sc2315e_cmos.c

******************************************************************************/
#if !defined(__SC2315E_CMOS_H_)
#define __SC2315E_CMOS_H_

#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "gk_api_isp.h"
#include "hicompat.h"
#include "universal.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sc2315e_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define SC2315E_ID 2315

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

#define HIGH_8BITS(x) ((x & 0xFF00) >> 8)
#define LOW_8BITS(x) (x & 0x00FF)
#define LOWER_4BITS(x) (x & 0x000F)
#define HIGHER_4BITS(x) ((x & 0xF000) >> 12)
#define HIGHER_8BITS(x) ((x & 0x0FF0) >> 4)
#define ISO_LUT_NUM 8

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSc2315e[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define SC2315E_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastSc2315e[dev])
#define SC2315E_SENSOR_SET_CTX(dev, pstCtx) (g_pastSc2315e[dev] = pstCtx)
#define SC2315E_SENSOR_RESET_CTX(dev) (g_pastSc2315e[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc2315eBusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = {.s8I2cDev = 0}, [1 ... ISP_MAX_PIPE_NUM - 1] = {.s8I2cDev = -1}};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE};

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32InitIso[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] =
                                                     100};

static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16InitCCM[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = {{0}};
static GK_BOOL g_abIsIrMode[ISP_MAX_PIPE_NUM] = {GK_FALSE};
static const GK_U32 g_au32IsoLut[ISO_LUT_NUM] = {100,  200,  400,  800,
                                                 1600, 3200, 6400, 12800};

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int sc2315e_i2c_addr;
extern unsigned int sc2315e_addr_byte;
extern unsigned int sc2315e_data_byte;

extern void sc2315e_init(VI_PIPE ViPipe);
extern void sc2315e_exit(VI_PIPE ViPipe);
extern void sc2315e_standby(VI_PIPE ViPipe);
extern void sc2315e_restart(VI_PIPE ViPipe);
extern int sc2315e_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sc2315e_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

/******** Sc2315e Register Address ********/
#define SC2315E_EXP_H_ADDR (0x3e00)
#define SC2315E_EXP_M_ADDR (0x3e01)
#define SC2315E_EXP_L_ADDR (0x3e02)
#define SC2315E_AGAIN_H_ADDR (0x3e08)
#define SC2315E_AGAIN_L_ADDR (0x3e09)
#define SC2315E_DGAIN_H_ADDR (0x3e06)
#define SC2315E_DGAIN_L_ADDR (0x3e07)
#define SC2315E_VMAX_H_ADDR (0x320e)
#define SC2315E_VMAX_L_ADDR (0x320f)
#define SC2315E_HMAX_H_ADDR (0x320c)
#define SC2315E_HMAX_L_ADDR (0x320d)

#define SC2315E_FULL_LINES_MAX (0x3FFFF)
#define SC2315E_INCREASE_LINES                                                 \
  (0) /* make real fps less than stand fps because NVR require */
#define SC2315E_VMAX_1080P30_LINEAR (1125 + SC2315E_INCREASE_LINES)

// sensor fps mode
#define SC2315E_SENSOR_1080P_30FPS_LINEAR_MODE (1)
#define SC2315E_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,
                                  AE_SENSOR_DEFAULT_S *pstAeSnsDft) {
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER(pstAeSnsDft);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

  pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
  pstAeSnsDft->stAgainAccu.f32Accuracy = 1;
  pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

  pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;
  pstAeSnsDft->bAERouteExValid = 0;
  pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
  pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;
  pstAeSnsDft->u32FullLinesMax = SC2315E_FULL_LINES_MAX;

  pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
  pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
  pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;

  pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
  if (g_au32LinesPer500ms[ViPipe] == 0) {
    pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
  } else {
    pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
  }
  pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
  pstAeSnsDft->u32FlickerFreq = 50 * 256;
  pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
  pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;

  pstAeSnsDft->u32InitExposure =
      g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 0x2457B;

  pstAeSnsDft->au8HistThresh[0] = 0x0D;
  pstAeSnsDft->au8HistThresh[1] = 0x28;
  pstAeSnsDft->au8HistThresh[2] = 0x60;
  pstAeSnsDft->au8HistThresh[3] = 0x80;

  pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

  pstAeSnsDft->u32ISPDgainShift = 8;
  pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
  pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

  pstAeSnsDft->u32MaxAgain = 0x3E00;
  pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;

  pstAeSnsDft->u32MinAgain = 1024;
  pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

  pstAeSnsDft->u32MinDgain = 1024;
  pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;

  pstAeSnsDft->u32MaxDgain = 0x880;
  pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

  pstAeSnsDft->u8AeCompensation = 0x40;

  pstAeSnsDft->u32MinIntTime = 2;
  pstAeSnsDft->u32MinIntTimeTarget = 2;
  pstAeSnsDft->u32MaxIntTimeTarget = 0xFFFF;

  return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps,
                            AE_SENSOR_DEFAULT_S *pstAeSnsDft) {
  GK_U32 u32FullLines;
  GK_U32 u32VMAX = SC2315E_VMAX_1080P30_LINEAR;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER_VOID(pstSnsState);

  float lines;
  GK_U32 maxIntTime;
  unsigned char LVMAX, HVMAX;

  if (pstSnsState->u8ImgMode != 1) // SC2315E_SENSOR_1080P_30FPS_LINEAR_MODE
  {
    lines = 2250.0; // SC2315E_VMAX_1080P30_LINEAR*2
    maxIntTime = 2246;
    u32FullLines = 2250; // SC2315E_VMAX_1080P30_LINEAR*2
    HVMAX = 4;
    LVMAX = 0x65; // 1125
  LABEL_10:
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LVMAX;
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HVMAX;
    /* The frame length this rate gives, kept where the stock driver keeps it
     * (see the #if 0 version below): u32FLStd, and au32FL[0] further down.
     * These two lines came from a disassembly of the vendor build and read
     * pstSnsState[1].astRegsInfo[0].astI2cData[19].{u32RegAddr,bUpdate} --
     * the vendor's state struct is 0x200 bytes longer, so its u32FLStd and
     * au32FL[0] land there. Here there is one ISP_SNS_STATE_S per pipe, so
     * pstSnsState[1] was ~490 bytes past the 2156-byte allocation: every
     * call wrote into whatever the heap held next, and musl's allocator
     * caught it when the context was freed on a reload. */
    pstSnsState->u32FLStd = u32FullLines;
    pstAeSnsDft->u32MaxIntTime = maxIntTime;
    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32FullLinesStd = u32FullLines;
    pstAeSnsDft->u32LinesPer500ms =
        (unsigned int)(float)((float)(f32Fps * lines) *
                              0.5); // orig SC2315E_VMAX_1080P30_LINEAR * 2 *
                                    // 15;
    pstSnsState->au32FL[0] = u32FullLines;
    pstAeSnsDft->u32FullLines = u32FullLines;
    return;
  }
  if (f32Fps <= 30.0 && f32Fps >= 5.0) {
    if (f32Fps >= 1.0e-10 || f32Fps <= -1.0e-10) {
      u32VMAX = (unsigned int)(float)(33750.0 / f32Fps);
      if (u32VMAX >= 0x3FFFF)
        u32VMAX = 0x3FFFF;
      HVMAX = u32VMAX >> 8;
      LVMAX = u32VMAX;
      u32FullLines = 2 * u32VMAX;
      if ((unsigned int)u32FullLines >= 0x3FFFF)
        u32FullLines = 0x3FFFF;
      maxIntTime = u32FullLines - 4;
      lines = (float)u32FullLines;
    } else {
      lines = 67500.0;
      maxIntTime = 67496;
      u32FullLines = 0x107AC;
      HVMAX = 0x83;
      LVMAX = 0xD6; // 33750
    }
    goto LABEL_10;
  }

#if 0
  switch (pstSnsState->u8ImgMode) {
  case SC2315E_SENSOR_1080P_30FPS_LINEAR_MODE:
    if ((f32Fps <= 30) && (f32Fps >= 5)) { /* Minimum 5fps */
      u32VMAX = SC2315E_VMAX_1080P30_LINEAR * 30 / DIV_0_TO_1_FLOAT(f32Fps);
    } else {
      ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n", f32Fps);
      return;
    }
    pstAeSnsDft->u32LinesPer500ms = SC2315E_VMAX_1080P30_LINEAR * 2 * 15;
    break;
  default:
    break;
  }

  u32VMAX =
      (u32VMAX > SC2315E_FULL_LINES_MAX) ? SC2315E_FULL_LINES_MAX : u32VMAX;

  pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS(u32VMAX);
  pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32VMAX);

  u32FullLines = u32VMAX << 1;
  pstSnsState->u32FLStd = (u32FullLines > SC2315E_FULL_LINES_MAX)
                              ? SC2315E_FULL_LINES_MAX
                              : u32FullLines;
  pstAeSnsDft->f32Fps = f32Fps;

  pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
  pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
  pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
  pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
#endif
  return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft) {
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER_VOID(pstSnsState);

  u32FullLines = (u32FullLines > SC2315E_FULL_LINES_MAX)
                     ? SC2315E_FULL_LINES_MAX
                     : u32FullLines;

  pstSnsState->au32FL[0] = u32FullLines;
  pstSnsState->astRegsInfo[0].astI2cData[7].u32Data =
      (u32FullLines >> 1) & 0xFF;
  pstSnsState->astRegsInfo[0].astI2cData[8].u32Data =
      (u32FullLines >> 9) & 0xFF;

  pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
  pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2;

  return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime) {
  GK_U32 u32Val;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER_VOID(pstSnsState);

  u32Val = (u32IntTime > SC2315E_FULL_LINES_MAX) ? SC2315E_FULL_LINES_MAX
                                                 : u32IntTime;

  pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (u32Val << 4) & 0xF0;
  pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32Val >> 4) & 0xFF;
  pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = (u32Val >> 12) & 0x0F;

  if (u32IntTime < 0x250)
    sc2315e_write_register(ViPipe, 0x3314, 20);
  else if (u32IntTime > 0x450)
    sc2315e_write_register(ViPipe, 0x3314, 4);

  return;
}

static const GK_U16 gau16GainIndex[64] = {
    0x400,  0x440,  0x480,  0x4C0,  0x500,  0x540,  0x580,  0x5C0,
    0x600,  0x640,  0x680,  0x6C0,  0x700,  0x740,  0x780,  0x7C0,
    0x800,  0x880,  0x900,  0x980,  0xA00,  0xA80,  0xB00,  0xB80,
    0xC00,  0xC80,  0xD00,  0xD80,  0xE00,  0xE80,  0xF00,  0xF80,
    0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500, 0x1600, 0x1700,
    0x1800, 0x1900, 0x1A00, 0x1B00, 0x1C00, 0x1D00, 0x1E00, 0x1F00,
    0x2000, 0x2200, 0x2400, 0x2600, 0x2800, 0x2A00, 0x2C00, 0x2E00,
    0x3000, 0x3200, 0x3400, 0x3600, 0x3800, 0x3A00, 0x3C00, 0x3E00};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin,
                                     GK_U32 *pu32AgainDb) {
  int i;

  CMOS_CHECK_POINTER_VOID(pu32AgainLin);
  CMOS_CHECK_POINTER_VOID(pu32AgainDb);

  if (*pu32AgainLin >= gau16GainIndex[63]) {
    *pu32AgainLin = gau16GainIndex[63];
    *pu32AgainDb = 63;
    return;
  }

  for (i = 1; i < 64; i++) {
    if (*pu32AgainLin < gau16GainIndex[i]) {
      *pu32AgainLin = gau16GainIndex[i - 1];
      *pu32AgainDb = i - 1;
      break;
    }
  }

  return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin,
                                     GK_U32 *pu32DgainDb) {
  int i;

  CMOS_CHECK_POINTER_VOID(pu32DgainLin);
  CMOS_CHECK_POINTER_VOID(pu32DgainDb);

  if (*pu32DgainLin >= gau16GainIndex[63]) {
    *pu32DgainLin = gau16GainIndex[63];
    *pu32DgainDb = 63;
    return;
  }

  for (i = 1; i < 64; i++) {
    if (*pu32DgainLin < gau16GainIndex[i]) {
      *pu32DgainLin = gau16GainIndex[i - 1];
      *pu32DgainDb = i - 1;
      break;
    }
  }

  return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again,
                                 GK_U32 u32Dgain) {
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER_VOID(pstSnsState);

  int u8Reg0x3e09;
  int v5;
  unsigned int u8Reg0x3e08;
  unsigned int gain;
  unsigned int v8;
  int v9;
  int u8Reg0x3e08_final;
  char u8Reg0x3e07;
  unsigned int u8Reg0x3e06;
  int u8Reg0x3e07_final;
  unsigned int v14;
  int v15;

  u8Reg0x3e09 = (u32Again & 0xF) | 0x10;
  v5 = gau16GainIndex[u32Again];
  u8Reg0x3e08 = u32Again >> 4; // high 8 bits
  gain = gau16GainIndex[u32Dgain] * v5;
  if (u8Reg0x3e08) {
    v8 = 0;
    v9 = 3;
    do {
      v8 = (v8 + 1);
      v9 = (2 * v9) | 1;
    } while (u8Reg0x3e08 > v8);
    u8Reg0x3e08_final = v9;
  } else {
    u8Reg0x3e08_final = 3;
  }
  u8Reg0x3e07 = u32Dgain & 0xF; // low 8 bits
  u8Reg0x3e06 = u32Dgain >> 4;  // high 8 bits
  u8Reg0x3e07_final = (8 * (u8Reg0x3e07 + 16));
  if (u8Reg0x3e06) {
    v14 = 0;
    v15 = 0;
    do {
      v14 = (v14 + 1);
      v15 = (2 * v15) | 1;
    } while (u8Reg0x3e06 > v14);
    u8Reg0x3e06 = v15;
  }
  pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = u8Reg0x3e09;
  pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u8Reg0x3e08_final;
  pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = u8Reg0x3e07_final;
  pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8Reg0x3e06;
  pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0;

  if (gain < 0x200000) {
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0xF;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 8;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
  } else if (gain < 0x400000) {
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x20;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 8;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
  } else if (gain < 0x800000) {
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x28;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 8;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
  } else if (gain < 0xf80000) {
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x80;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 8;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
  } else {
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x80;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x48;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x30;
  }

  if (gain >= 16252928) {
    sc2315e_write_register(ViPipe, 0x5781, 1);
    sc2315e_write_register(ViPipe, 0x5785, 2);
  } else {
    sc2315e_write_register(ViPipe, 0x5781, 96);
    sc2315e_write_register(ViPipe, 0x5785, 48);
  }
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable,
                                    GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax,
                                    GK_U32 *au32IntTimeMin,
                                    GK_U32 *pu32LFMaxIntTime) {
  return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,
                                      AE_FSWDR_ATTR_S *pstAeFSWDRAttr) {
  CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

  genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
  gu32MaxTimeGetCnt[ViPipe] = 0;

  return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs) {
  CMOS_CHECK_POINTER(pstExpFuncs);

  memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

  pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
  pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
  pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
  pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
  pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
  pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;
  pstExpFuncs->pfn_cmos_dgain_calc_table = cmos_dgain_calc_table;
  pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
  pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

  return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,
                                   AWB_SENSOR_DEFAULT_S *pstAwbSnsDft) {
  GK_S32 i;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER(pstAwbSnsDft);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

  pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
  pstAwbSnsDft->u16WbRefTemp = 4730;

  pstAwbSnsDft->as32WbPara[0] = 111;
  pstAwbSnsDft->as32WbPara[1] = 27;
  pstAwbSnsDft->as32WbPara[2] = 0xFFFFFF8A;
  pstAwbSnsDft->as32WbPara[3] = 0x39CED;
  pstAwbSnsDft->as32WbPara[4] = 128;
  pstAwbSnsDft->as32WbPara[5] = 0xFFFD3521;

  pstAwbSnsDft->au16GainOffset[0] = 0x160;
  pstAwbSnsDft->au16GainOffset[1] = 0x100;
  pstAwbSnsDft->au16GainOffset[2] = 0x100;
  pstAwbSnsDft->au16GainOffset[3] = 0x1AE;

  pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

  switch (pstSnsState->enWDRMode) {
  default:
  case WDR_MODE_NONE:
    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
    if (g_abIsIrMode[ViPipe] == GK_TRUE) {
      memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableIr,
             sizeof(AWB_AGC_TABLE_S));
    } else {
      memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable,
             sizeof(AWB_AGC_TABLE_S));
    }
    break;
  }

  pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
  pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
  pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
  pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
  pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

  for (i = 0; i < CCM_MATRIX_SIZE; i++) {
    pstAwbSnsDft->au16InitCCM[i] = g_au16InitCCM[ViPipe][i];
  }

  return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs) {
  CMOS_CHECK_POINTER(pstExpFuncs);

  memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));
  pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

  return GK_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {{378, 256, 430},
                                                      {439, 256, 439}};

static GK_U32 Interpulate(GK_U32 u32Mid, GK_U32 u32Left, GK_U32 u32LValue,
                          GK_U32 u32Right, GK_U32 u32RValue) {
  GK_U32 u32Value = 0;
  GK_U32 k = 0;

  if (u32Mid <= u32Left) {
    u32Value = u32LValue;
    return u32Value;
  }

  if (u32Mid >= u32Right) {
    u32Value = u32RValue;
    return u32Value;
  }

  k = (u32Right - u32Left);
  u32Value = (((u32Right - u32Mid) * u32LValue +
               (u32Mid - u32Left) * u32RValue + (k >> 1)) /
              k);

  return u32Value;
}

static GK_VOID cmos_set_drc_init_str(VI_PIPE ViPipe) {
  GK_U32 u32Level = 0;
  GK_U32 au32DrcInitStr0[ISO_LUT_NUM] = {DRC_INIT_STR_NA};
  GK_U32 au32DrcInitStr1[ISO_LUT_NUM] = {DRC_INIT_STR_IR};

  for (u32Level = 0; u32Level < ISO_LUT_NUM; u32Level++) {
    if (g_au32InitIso[ViPipe] <= g_au32IsoLut[u32Level]) {
      break;
    }
  }

  if (u32Level == ISO_LUT_NUM) {
    u32Level = ISO_LUT_NUM - 1;
  }

  if (g_abIsIrMode[ViPipe] == GK_TRUE) {
    if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
      g_stIspDRC.u16ManualStrength = au32DrcInitStr1[u32Level];
    } else {
      g_stIspDRC.u16ManualStrength =
          Interpulate(g_au32InitIso[ViPipe], g_au32IsoLut[u32Level - 1],
                      au32DrcInitStr1[u32Level - 1], g_au32IsoLut[u32Level],
                      au32DrcInitStr1[u32Level]);
    }
  } else {
    if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
      g_stIspDRC.u16ManualStrength = au32DrcInitStr0[u32Level];
    } else {
      g_stIspDRC.u16ManualStrength =
          Interpulate(g_au32InitIso[ViPipe], g_au32IsoLut[u32Level - 1],
                      au32DrcInitStr0[u32Level - 1], g_au32IsoLut[u32Level],
                      au32DrcInitStr0[u32Level]);
    }
  }
}

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef) {
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER(pstDef);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

  cmos_set_drc_init_str(ViPipe);

  pstDef->unKey.bit1Dpc = 1;
  pstDef->pstDpc = &g_stCmosDpc;
  pstDef->unKey.bit1Wdr = 1;
  pstDef->pstWdr = &g_stIspWDR;
  pstDef->unKey.bit1Lsc = 0;
  pstDef->pstLsc = &g_stCmosLsc;
  pstDef->unKey.bit1PreGamma = 0;
  pstDef->pstPreGamma = &g_stPreGamma;
  pstDef->unKey.bit1Demosaic = 1;
  pstDef->pstDemosaic = &g_stIspDemosaic;
  pstDef->unKey.bit1Sharpen = 1;
  pstDef->pstSharpen = &g_stIspYuvSharpen;
  pstDef->unKey.bit1Drc = 1;
  // not completed
  pstDef->pstDrc = &g_stIspDRC;
  pstDef->unKey.bit1BayerNr = 1;
  pstDef->pstBayerNr = &g_stIspBayerNr;
  pstDef->unKey.bit1AntiFalseColor = 1;
  pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
  pstDef->unKey.bit1Ldci = 1;
  pstDef->pstLdci = &g_stIspLdci;
  pstDef->unKey.bit1Gamma = 1;
  pstDef->pstGamma = &g_stIspGamma;
  pstDef->unKey.bit1Ge = 1;
  pstDef->pstGe = &g_stIspGe;
  pstDef->unKey.bit1Ca = 1;
  pstDef->pstCa = &g_stIspCA;
  pstDef->unKey.bit1Dehaze = 1;
  pstDef->pstDehaze = &g_stIspDehaze;

  // bad item
  pstDef->unKey.u64Key = 0x6FFB;

  memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio,
         sizeof(ISP_CMOS_NOISE_CALIBRATION_S));

  pstDef->stSensorMode.u32SensorID = SC2315E_ID;
  pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

  memcpy(&pstDef->stDngColorParam, &g_stDngColorParam,
         sizeof(ISP_CMOS_DNG_COLORPARAM_S));

  pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
  pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;

  pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH
      .u32Denominator = 1;
  pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH
      .u32Numerator = 1;
  pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV
      .u32Denominator = 1;
  pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV
      .u32Numerator = 1;
  pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
      .u16RepeatPatternDimRows = 2;
  pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim
      .u16RepeatPatternDimCols = 2;
  pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2;
  pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2;
  pstDef->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
  pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
  pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;

  return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,
                                       ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel) {
  GK_S32 i;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER(pstBlackLevel);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  /* Don't need to update black level when iso change */
  pstBlackLevel->bUpdate = GK_FALSE;

  for (i = 0; i < 4; i++) {
    pstBlackLevel->au16BlackLevel[i] = 66; // got from Sofia
  }

  return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable) {
  GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER_VOID(pstSnsState);

  u32FullLines_5Fps = (SC2315E_VMAX_1080P30_LINEAR * 30) / 5;
  u32MaxIntTime_5Fps = pstSnsState->u32FLStd - 8;

  if (bEnable) { /* setup for ISP pixel calibration mode */
    sc2315e_write_register(ViPipe, SC2315E_VMAX_H_ADDR,
                           (u32FullLines_5Fps & 0xFF00) >> 8); /* 5fps */
    sc2315e_write_register(ViPipe, SC2315E_VMAX_L_ADDR,
                           (u32FullLines_5Fps & 0xFF)); /* 5fps */
    sc2315e_write_register(ViPipe, SC2315E_EXP_M_ADDR,
                           (u32MaxIntTime_5Fps & 0x0FF0) >>
                               4); /* max exposure lines */
    sc2315e_write_register(ViPipe, SC2315E_EXP_L_ADDR,
                           (u32MaxIntTime_5Fps & 0xF)
                               << 4); /* max exposure lines */

    sc2315e_write_register(ViPipe, SC2315E_AGAIN_H_ADDR,
                           0x00); /* AG, Context A */
    sc2315e_write_register(ViPipe, SC2315E_AGAIN_L_ADDR,
                           0x10); /* DG, Context A */
  } else {                        /* setup for ISP 'normal mode' */
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > SC2315E_FULL_LINES_MAX * 2)
                                ? (SC2315E_FULL_LINES_MAX * 2)
                                : pstSnsState->u32FLStd;
    sc2315e_write_register(ViPipe, SC2315E_VMAX_H_ADDR,
                           (pstSnsState->u32FLStd & 0xFF00) >> 8); /* 30fps */
    sc2315e_write_register(ViPipe, SC2315E_VMAX_L_ADDR,
                           (pstSnsState->u32FLStd & 0xFF));

    pstSnsState->bSyncInit = GK_FALSE;
  }
  return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode) {
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  pstSnsState->bSyncInit = GK_FALSE;

  switch (u8Mode) {
  case WDR_MODE_NONE:
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    log_d("linear mode");
    break;
  default:
    ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
    return GK_FAILURE;
  }

  memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

  return GK_SUCCESS;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe,
                                     ISP_SNS_REGS_INFO_S *pstSnsRegsInfo) {
  GK_S32 i;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER(pstSnsRegsInfo);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  if ((pstSnsState->bSyncInit == GK_FALSE) ||
      (pstSnsRegsInfo->bConfig == GK_FALSE)) {
    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev =
        g_aunSc2315eBusInfo[ViPipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
    pstSnsState->astRegsInfo[0].u32RegNum = 13;

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
      pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
      pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = sc2315e_i2c_addr;
      pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum =
          sc2315e_addr_byte;
      pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum =
          sc2315e_data_byte;
    }

    // Linear Mode Regs
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = SC2315E_EXP_L_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = SC2315E_EXP_M_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = SC2315E_EXP_H_ADDR;

    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = SC2315E_AGAIN_L_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = SC2315E_AGAIN_H_ADDR;

    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = SC2315E_DGAIN_L_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = SC2315E_DGAIN_H_ADDR;

    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = SC2315E_VMAX_L_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = SC2315E_VMAX_H_ADDR;

    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr =
        0x3812; // Grouphold Logic
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = 0x3301;
    pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = 0x3632;
    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = 0x3812;

    pstSnsState->bSyncInit = GK_TRUE;
  } else {
    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
      if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
          pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
      } else {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
      }
    }

    // if AGain or DGain were updated mark GL regs also as valid
    if (pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate ||
        pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate ||
        pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate ||
        pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate) {
      pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate = GK_TRUE;
      pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate = GK_TRUE;
      pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate = GK_TRUE;
      pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate = GK_TRUE;
    } else {
      pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate = GK_FALSE;
      pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate = GK_FALSE;
      pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate = GK_FALSE;
      pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate = GK_FALSE;
    }
  }

  pstSnsRegsInfo->bConfig = GK_FALSE;
  memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0],
         sizeof(ISP_SNS_REGS_INFO_S));
  memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0],
         sizeof(ISP_SNS_REGS_INFO_S));

  pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

  return GK_SUCCESS;
}

static GK_S32
cmos_set_image_mode(VI_PIPE ViPipe,
                    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode) {
  GK_U8 u8SensorImageMode = 0;
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  CMOS_CHECK_POINTER(pstSensorImageMode);
  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER(pstSnsState);

  u8SensorImageMode = pstSnsState->u8ImgMode;
  pstSnsState->bSyncInit = GK_FALSE;

  if (pstSensorImageMode->f32Fps <= 30) {
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
      if (SC2315E_RES_IS_1080P(pstSensorImageMode->u16Width,
                               pstSensorImageMode->u16Height)) {
        u8SensorImageMode = SC2315E_SENSOR_1080P_30FPS_LINEAR_MODE;
      } else {
        ISP_TRACE(MODULE_DBG_ERR,
                  "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                  pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                  pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
        return GK_FAILURE;
      }
    } else {
      ISP_TRACE(MODULE_DBG_ERR,
                "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
      return GK_FAILURE;
    }
  } else {
  }

  if ((pstSnsState->bInit == GK_TRUE) &&
      (u8SensorImageMode == pstSnsState->u8ImgMode)) {
    /* Don't need to switch SensorImageMode */
    return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
  }

  return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe) {
  ISP_SNS_STATE_S *pstSnsState = GK_NULL;

  SC2315E_SENSOR_GET_CTX(ViPipe, pstSnsState);
  CMOS_CHECK_POINTER_VOID(pstSnsState);

  pstSnsState->bInit = GK_FALSE;
  pstSnsState->bSyncInit = GK_FALSE;
  pstSnsState->u8ImgMode = SC2315E_SENSOR_1080P_30FPS_LINEAR_MODE;
  pstSnsState->enWDRMode = WDR_MODE_NONE;
  pstSnsState->u32FLStd = SC2315E_VMAX_1080P30_LINEAR;
  pstSnsState->au32FL[0] = SC2315E_VMAX_1080P30_LINEAR;
  pstSnsState->au32FL[1] = SC2315E_VMAX_1080P30_LINEAR;

  memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
  memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

void nullsub() {}

static GK_S32
cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc) {
  CMOS_CHECK_POINTER(pstSensorExpFunc);

  memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

  pstSensorExpFunc->pfn_cmos_sensor_init = sc2315e_init;
  pstSensorExpFunc->pfn_cmos_sensor_exit = sc2315e_exit;
  pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
  pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
  pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

  pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
  pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
  pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
  pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

  return GK_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static GK_S32 sc2315e_set_bus_info(VI_PIPE ViPipe,
                                   ISP_SNS_COMMBUS_U unSNSBusInfo) {
  g_aunSc2315eBusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

  return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe) {
  ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

  SC2315E_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

  if (pastSnsStateCtx == GK_NULL) {
    pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
    if (pastSnsStateCtx == GK_NULL) {
      ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
      return ERR_CODE_ISP_NOMEM;
    }
  }

  memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

  SC2315E_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

  return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe) {
  ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

  SC2315E_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
  SENSOR_FREE(pastSnsStateCtx);
  SC2315E_SENSOR_RESET_CTX(ViPipe);
}

static GK_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                       ALG_LIB_S *pstAwbLib) {
  GK_S32 s32Ret;

  ISP_SENSOR_REGISTER_S stIspRegister;
  AE_SENSOR_REGISTER_S stAeRegister;
  AWB_SENSOR_REGISTER_S stAwbRegister;
  ISP_SNS_ATTR_INFO_S stSnsAttrInfo;

  CMOS_CHECK_POINTER(pstAeLib);
  CMOS_CHECK_POINTER(pstAwbLib);

  s32Ret = sensor_ctx_init(ViPipe);

  if (s32Ret != GK_SUCCESS) {
    return GK_FAILURE;
  }

  stSnsAttrInfo.eSensorId = SC2315E_ID;

  s32Ret = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
  s32Ret |=
      GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);

  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
    return s32Ret;
  }

  s32Ret = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
  s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo,
                                        &stAeRegister);

  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR,
              "sensor register callback function to ae lib failed!\n");
    return s32Ret;
  }

  s32Ret = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
  s32Ret |= GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo,
                                         &stAwbRegister);

  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR,
              "sensor register callback function to awb lib failed!\n");
    return s32Ret;
  }

  return GK_SUCCESS;
}

static GK_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                         ALG_LIB_S *pstAwbLib) {
  GK_S32 s32Ret;

  CMOS_CHECK_POINTER(pstAeLib);
  CMOS_CHECK_POINTER(pstAwbLib);

  s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC2315E_ID);
  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
    return s32Ret;
  }

  s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC2315E_ID);
  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR,
              "sensor unregister callback function to ae lib failed!\n");
    return s32Ret;
  }

  s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC2315E_ID);
  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR,
              "sensor unregister callback function to awb lib failed!\n");
    return s32Ret;
  }

  sensor_ctx_exit(ViPipe);

  return GK_SUCCESS;
}

static GK_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr) {
  GK_S32 i;

  CMOS_CHECK_POINTER(pstInitAttr);

  if (pstInitAttr->u32InitIso) {
    g_au32InitIso[ViPipe] = pstInitAttr->u32InitIso;
  }
  g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
  g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
  g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
  g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
  g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
  g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
  g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;
  g_abIsIrMode[ViPipe] = pstInitAttr->bIsIrMode;

  for (i = 0; i < CCM_MATRIX_SIZE; i++) {
    g_au16InitCCM[ViPipe][i] = pstInitAttr->au16CCM[i];
  }

  return GK_SUCCESS;
}

static ISP_SNS_OBJ_S stSnsSc2315EObj = {
    .pfnRegisterCallback = sensor_register_callback,
    .pfnUnRegisterCallback = sensor_unregister_callback,
    .pfnStandby = sc2315e_standby,
    .pfnRestart = sc2315e_restart,
    .pfnMirrorFlip = GK_NULL,
    .pfnWriteReg = sc2315e_write_register,
    .pfnReadReg = sc2315e_read_register,
    .pfnSetBusInfo = sc2315e_set_bus_info,
    .pfnSetInit = sensor_set_init};

log_func_t main_log;

__attribute__((visibility("default"))) ISP_SNS_OBJ_S *
sensor_driver(driver_params_t *params) {
  main_log = params->log_func;
  log_i("SC2315E Universal driver © openipc.org " __DATE__ " " __TIME__);

  return &stSnsSc2315EObj;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SC2315E_CMOS_H_ */
