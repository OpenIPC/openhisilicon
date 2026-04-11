#if !defined(__os08a10_CMOS_H_)
#define __os08a10_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "mpi_af.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define os08a10_ID 0x08a10

#ifndef CLIP3
#define CLIP3(x,min,max)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#endif

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
ISP_SNS_STATE_S             g_astos08a10[ISP_MAX_DEV_NUM] = {{0}};
ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_DEV_NUM] = {&g_astos08a10[0], &g_astos08a10[1]};
ISP_SNS_COMMBUS_U     g_aunos08a10BusInfo[ISP_MAX_DEV_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1] = { .s8I2cDev = 1}
};
static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_DEV_NUM] = {ISP_FSWDR_NORMAL_MODE,ISP_FSWDR_NORMAL_MODE};
//static HI_U32 gu32MaxTimeGetCnt[ISP_MAX_DEV_NUM] = {0,0};

static HI_U32 g_au32InitExposure[ISP_MAX_DEV_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_DEV_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_DEV_NUM] = {0};

static HI_U32 u32PreAGain[ISP_MAX_DEV_NUM] = {0x80, 0x80};
static HI_U32 u32CurAGain[ISP_MAX_DEV_NUM] = {0x80, 0x80};

extern const unsigned int os08a10_i2c_addr;
extern unsigned int os08a10_addr_byte;
extern unsigned int os08a10_data_byte;

extern void os08a10_init(ISP_DEV IspDev);
extern void os08a10_exit(ISP_DEV IspDev);
extern void os08a10_standby(ISP_DEV IspDev);
extern void os08a10_restart(ISP_DEV IspDev);
extern int os08a10_write_register(ISP_DEV IspDev, int addr, int data);
extern int os08a10_read_register(ISP_DEV IspDev, int addr);

#define os08a10_FULL_LINES_MAX  (0xE245)
#define os08a10_FULL_LINES_MAX_2TO1_WDR  (0xE245)    // considering the YOUT_SIZE and bad frame

/*****os08a10 Register Address*****/
#define os08a10_EXPO_H_ADDR  (0x3501)
#define os08a10_EXPO_L_ADDR  (0x3502)
#define os08a10_AGAIN_H_ADDR (0x3508)
#define os08a10_AGAIN_L_ADDR (0x3509)
#define os08a10_DGAIN_H_ADDR (0x350a)
#define os08a10_DGAIN_L_ADDR (0x350b)
#define os08a10_VMAX_H_ADDR  (0x380e)
#define os08a10_VMAX_L_ADDR  (0x380f)
#define os08a10_R3740_ADDR   (0x3740)
#define os08a10_R3741_ADDR   (0x3741)
/*****os08a10 2to1 WDR Register Address*****/
#define os08a10_SHORT_EXPO_H_ADDR  (0x3511)
#define os08a10_SHORT_EXPO_L_ADDR  (0x3512)
#define os08a10_SHORT_AGAIN_H_ADDR (0x350c)
#define os08a10_SHORT_AGAIN_L_ADDR (0x350d)
#define os08a10_SHORT_DGAIN_H_ADDR (0x350e)
#define os08a10_SHORT_DGAIN_L_ADDR (0x350f)

#define os08a10_INCREASE_LINES (1) /* make real fps less than stand fps because NVR require*/

#define os08a10_VMAX_4k2k30_LINEAR  (2316+os08a10_INCREASE_LINES)
//#define os08a10_VMAX_4k2k25_WDR     (2316+os08a10_INCREASE_LINES)
#define os08a10_VMAX_4k2k30_WDR     (2316+os08a10_INCREASE_LINES)

//sensor fps mode
#define os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE      (1)
#define os08a10_SENSOR_4k2k_30FPS_2t1_WDR_MODE     (3)

static HI_S32 cmos_get_ae_default(ISP_DEV IspDev,AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return - 1;
    }

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50*256;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0078125;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

    if(g_au32LinesPer500ms[IspDev] == 0)
    {
        pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd*30/2;
    }
    else
    {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[IspDev];
    }

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch(g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:   /*linear mode*/
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u32MaxAgain = 2047;
            pstAeSnsDft->u32MinAgain = 128;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 2047;
            pstAeSnsDft->u32MinDgain = 128;
            pstAeSnsDft->u32MaxDgainTarget = 2047;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev] ? g_au32InitExposure[IspDev] : 10000;

            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 8;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;
            pstAeSnsDft->u32FullLinesMax = os08a10_FULL_LINES_MAX;
        break;

        case WDR_MODE_2To1_LINE:
            pstAeSnsDft->au8HistThresh[0] = 0xC;
            pstAeSnsDft->au8HistThresh[1] = 0x18;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 0x48;
            pstAeSnsDft->u32MinIntTime = 4;

            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

            pstAeSnsDft->u32MaxAgain = 2047;
            pstAeSnsDft->u32MinAgain = 128;
            pstAeSnsDft->u32MaxAgainTarget = 2047;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 2047;
            pstAeSnsDft->u32MinDgain = 128;
            pstAeSnsDft->u32MaxDgainTarget = 2047;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev] ? g_au32InitExposure[IspDev] : 10000;
            pstAeSnsDft->u32FullLinesMax = os08a10_FULL_LINES_MAX_2TO1_WDR;

            if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
            {
                pstAeSnsDft->u8AeCompensation = 56;
                pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
            }
            else
            {
                pstAeSnsDft->u32MaxDgainTarget = 2047;
                pstAeSnsDft->u32MaxISPDgainTarget = 267;
                pstAeSnsDft->u8AeCompensation = 24;
                pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
                pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
                pstAeSnsDft->au32Ratio[0] = 0x400;
                pstAeSnsDft->au32Ratio[1] = 0x40;
                pstAeSnsDft->au32Ratio[2] = 0x40;
            }
       break;
    }

    return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(ISP_DEV IspDev, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

    HI_U32 u32VMAX = os08a10_VMAX_4k2k30_LINEAR;

    switch (g_apstSnsState[IspDev]->u8ImgMode)
    {
        case os08a10_SENSOR_4k2k_30FPS_2t1_WDR_MODE:
           if ((f32Fps <= 30) && (f32Fps >= 1.2))
           {
               u32VMAX = os08a10_VMAX_4k2k30_WDR * 30 / f32Fps;
           }
           else
           {
               printf("Not support Fps: %f\n", f32Fps);
               return;
           }
           u32VMAX = (u32VMAX > os08a10_FULL_LINES_MAX_2TO1_WDR) ? os08a10_FULL_LINES_MAX_2TO1_WDR : u32VMAX;
           break;

      case os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE:
           if ((f32Fps <= 30) && (f32Fps >= 1.2))
           {
               u32VMAX = os08a10_VMAX_4k2k30_LINEAR * 30 / f32Fps;
           }
           else
           {
               printf("Not support Fps: %f\n", f32Fps);
               return;
           }
           u32VMAX = (u32VMAX > os08a10_FULL_LINES_MAX) ? os08a10_FULL_LINES_MAX : u32VMAX;
           break;

      default:
          return;
          break;
    }

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = ((u32VMAX & 0xFF00) >> 8);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = (u32VMAX & 0xFF);

    g_apstSnsState[IspDev]->u32FLStd = u32VMAX;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 0x48;
    }
    else
    {
        pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 8;
    }
    g_apstSnsState[IspDev]->au32FL[0] = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];

    return;

}

static HI_VOID cmos_slow_framerate_set(ISP_DEV IspDev,HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        u32FullLines = (u32FullLines > os08a10_FULL_LINES_MAX_2TO1_WDR) ? os08a10_FULL_LINES_MAX_2TO1_WDR : u32FullLines;
    }
    else
    {
        u32FullLines = (u32FullLines > os08a10_FULL_LINES_MAX) ? os08a10_FULL_LINES_MAX : u32FullLines;
    }

    g_apstSnsState[IspDev]->au32FL[0] = u32FullLines;

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = (((g_apstSnsState[IspDev]->au32FL[0]) & 0xFF00) >> 8);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = ((g_apstSnsState[IspDev]->au32FL[0]) & 0xFF);

    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->au32FL[0] - 0x48;
    }
    else
    {
        pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->au32FL[0] - 8;
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(ISP_DEV IspDev,HI_U32 u32IntTime)
{
    HI_U32 u32Value = 0;

    static HI_BOOL bFirst[ISP_MAX_DEV_NUM] ={1, 1};

    static HI_U32 u32ShortIntTime[ISP_MAX_DEV_NUM] = {0};
    static HI_U32 u32LongIntTime[ISP_MAX_DEV_NUM] = {0};

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
      if (bFirst[IspDev]) /* short exposure */
      {
           g_apstSnsState[IspDev]->au32WDRIntTime[0] = u32IntTime;
           u32ShortIntTime[IspDev] = u32IntTime;

           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32Data = ((u32ShortIntTime[IspDev] & 0xFF00) >> 8);
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u32Data = (u32ShortIntTime[IspDev] & 0xFF);

           bFirst[IspDev] = HI_FALSE;
       }
       else   /* long exposure */
       {
           g_apstSnsState[IspDev]->au32WDRIntTime[1] = u32IntTime;
           u32LongIntTime[IspDev] = (u32IntTime + u32ShortIntTime[IspDev] <= g_apstSnsState[IspDev]->au32FL[1] - 0x48) ? u32IntTime:g_apstSnsState[IspDev]->au32FL[1] - 0x48 - u32ShortIntTime[IspDev];

           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = ((u32LongIntTime[IspDev] & 0xFF00) >> 8);
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = (u32LongIntTime[IspDev] & 0xFF);

           bFirst[IspDev] = HI_TRUE;
        }
    }
    else
    {
         u32Value = u32IntTime;

         g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = ((u32Value & 0xFF00) >> 8);
         g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = (u32Value & 0xFF);
    }

  return;
}

static HI_VOID cmos_gains_update(ISP_DEV IspDev,HI_U32 u32Again, HI_U32 u32Dgain)
{
    HI_U32 u32AGainReg = 0;
    HI_U32 u32DGainReg = 0;
    HI_U32 u32AnalogTiming_1 = 0;
    HI_U32 u32AnalogTiming_2 = 0;

    u32PreAGain[IspDev] = u32CurAGain[IspDev];

    //Analog Gain
    u32Again = CLIP3(u32Again, 0x80, 0x7FF);
    u32AGainReg = u32Again;
    u32CurAGain[IspDev] = u32Again;
    if (u32Again < 0x100)
    {
        u32AnalogTiming_1 = 0x07;
        u32AnalogTiming_2 = 0x03;
    }
    else if (u32Again < 0x200)
    {
        u32AnalogTiming_1 = 0x09;
        u32AnalogTiming_2 = 0x03;
    }
    else if (u32Again < 0x400)
    {
        u32AnalogTiming_1 = 0x10;
        u32AnalogTiming_2 = 0x03;
    }
    else
    {
        u32AnalogTiming_1 = 0x1b;
        u32AnalogTiming_2 = 0x03;
    }

    // Digital Gain
    if(u32Dgain < 0x80)
    {
        u32DGainReg = 0x400;
    }
    else if(u32Dgain < 0x800)
    {
        u32DGainReg = u32Dgain*0x8;
    }
    else
    {
        u32DGainReg = 0x3fff;
    }

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = ((u32AGainReg & 0xff00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32Data = (u32AGainReg & 0xff);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32Data = ((u32DGainReg & 0xff00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = (u32DGainReg & 0xff);

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u32Data = ((u32AGainReg & 0xff00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u32Data = (u32AGainReg & 0xff);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u32Data = ((u32DGainReg & 0xff00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u32Data = (u32DGainReg & 0xff);

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32Data = (u32AnalogTiming_1 & 0xff);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32Data = (u32AnalogTiming_2 & 0xff);
    }
    else
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = ((u32AGainReg & 0xff00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32Data = (u32AGainReg & 0xff);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32Data = ((u32DGainReg & 0xff00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = (u32DGainReg & 0xff);

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32Data = (u32AnalogTiming_1 & 0xff);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32Data = (u32AnalogTiming_2 & 0xff);
    }

    return;
}

static HI_VOID cmos_get_inttime_max(ISP_DEV IspDev,HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32IntTimeMaxTmp = 0;

    if((HI_NULL == au32Ratio) || (HI_NULL == au32IntTimeMax) || (HI_NULL == au32IntTimeMin))
    {
        printf("null pointer when get ae sensor ExpRatio/IntTimeMax/IntTimeMin value!\n");
        return;
    }

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        /* Short + Long < VMax - 0x48; */
        /* Ratio = Long * 0x40 / Short */
        u32IntTimeMaxTmp = ((g_apstSnsState[IspDev]->au32FL[1] - 0x48 - g_apstSnsState[IspDev]->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
        au32IntTimeMax[0] = (g_apstSnsState[IspDev]->au32FL[0] - 0x48) * 0x40 / (au32Ratio[0] + 0x40);

        au32IntTimeMax[0] = (u32IntTimeMaxTmp < au32IntTimeMax[0]) ? u32IntTimeMaxTmp : au32IntTimeMax[0];
        au32IntTimeMax[1] = au32IntTimeMax[0] * au32Ratio[0] >> 6;
        au32IntTimeMin[0] = 2;
        au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >> 6;
    }

    return;

}

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(ISP_DEV IspDev,AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    genFSWDRMode[IspDev] = pstAeFSWDRAttr->enFSWDRMode;
//    gu32MaxTimeGetCnt[IspDev] = 0;
}


static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set= cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    //pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    //pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return 0;
}

static AWB_CCM_S g_stAwbCcm =
{
   4950,
   {
       0x01CB,  0x80AF,  0x801C,
       0x8040,  0x018B,  0x804B,
       0x0019,  0x80BD,  0x01A4,
   },
   3894,
   {
       0x01BC,  0x8099,  0x8023,
       0x8061,  0x01A1,  0x8040,
       0x0027,  0x80EC,  0x01C5,
   },
   2604,
   {
       0x01F7,  0x80F2,  0x8005,
       0x804F,  0x0159,  0x800A,
       0x0059,  0x81B8,  0x025F,
   }
};

static AWB_CCM_S g_stAwbCcmWDR =
{
   4950,
   {
       0x01CB,  0x80AF,  0x801C,
       0x8040,  0x018B,  0x804B,
       0x0019,  0x80BD,  0x01A4,
   },
   3894,
   {
       0x01BC,  0x8099,  0x8023,
       0x8061,  0x01A1,  0x8040,
       0x0027,  0x80EC,  0x01C5,
   },
   2604,
   {
       0x01F7,  0x80F2,  0x8005,
       0x804F,  0x0159,  0x800A,
       0x0059,  0x81B8,  0x025F,
   }
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /*1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768*/
    /* saturation */
    {0x80,0x7a,0x78,0x74,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38}
};

static AWB_AGC_TABLE_S g_stAwbAgcTableFSWDR =
{
    /* bvalid */
    1,

    /* saturation */
    {0x80,0x78,0x6e,0x64,0x5E,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38,0x38}
};

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static HI_S32 cmos_get_awb_default(ISP_DEV IspDev,AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    if (HI_NULL == pstAwbSnsDft)
    {
        printf("null pointer when get awb default value!\n");
        return -1;
    }

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 4950;
    pstAwbSnsDft->au16GainOffset[0] = 531;
    pstAwbSnsDft->au16GainOffset[1] = 256;
    pstAwbSnsDft->au16GainOffset[2] = 256;
    pstAwbSnsDft->au16GainOffset[3] = 450;
    pstAwbSnsDft->as32WbPara[0] = -42;
    pstAwbSnsDft->as32WbPara[1] = 298;
    pstAwbSnsDft->as32WbPara[2] = 0;
    pstAwbSnsDft->as32WbPara[3] = 175532;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -125725;

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
        break;
        case WDR_MODE_2To1_LINE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmWDR, sizeof(AWB_CCM_S));
        break;
    }

    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[IspDev];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[IspDev];
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[IspDev][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[IspDev][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[IspDev][2];

    pstAwbSnsDft->u8AWBRunInterval = 2;

    return 0;
}

static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return 0;
}


static ISP_CMOS_AGC_TABLE_S g_stIspAgcTable =
{
    /* bvalid */
    1,

    /* snr_thresh */
    {0x08,0x0c,0x10,0x14,0x18,0x20,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30},

    /* demosaic_np_offset */
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30},

    /* ge_strength */
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37}

};

static ISP_CMOS_BAYER_SHARPEN_S g_stIspBayerSharpen =
{
    /* bvalid */
    1,

    /* ShpAlgSel = 1 is Demosaic SharpenEx, else Demosaic sharpen. */
    0,

    /* sharpen_alt_d to Sharpen */
    {0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x40,0x30,0x20,0x10},

    /* sharpen_alt_ud to Sharpen */
    {0x3a,0x36,0x32,0x30,0x2c,0x30,0x30,0x30,0x28,0x24,0x24,0x20,0x20,0x20,0x10,0x10},

    /* demosaic_lum_thresh to Sharpen */
    {0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x40,0x30,0x20,0x20,0x20,0x20,0x20,0x20},

    /* SharpenHF to SharpenEx */
    {0x30,0x30,0x30,0x30,0x30,0x30,0x2c,0x28,0x20,0x18,0x14,0x10,0x10,0x10,0x10,0x10},

    /* SharpenMF to SharpenEx */
    {0x30,0x30,0x30,0x30,0x28,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10},

    /* SharpenLF to SharpenEx */
    {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},

    /* SadAmplifier to SharpenEx */
    {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10}

};


static ISP_CMOS_BAYER_SHARPEN_S g_stIspBayerSharpenFSWDR =
{
    /* bvalid */
    1,

    /* ShpAlgSel = 1 is Demosaic SharpenEx, else Demosaic sharpen. */
    0,

    /* sharpen_alt_d to Sharpen */
    {0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x40,0x30,0x20,0x10},

    /* sharpen_alt_ud to Sharpen */
    {0x48,0x46,0x42,0x40,0x40,0x40,0x40,0x40,0x40,0x3c,0x38,0x32,0x28,0x20,0x10,0x10},

    /* demosaic_lum_thresh to Sharpen */
    {0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x40,0x30,0x20,0x20,0x20,0x20,0x20,0x20},

    /* SharpenHF to SharpenEx */
    {0x30,0x30,0x30,0x30,0x30,0x30,0x2c,0x28,0x20,0x18,0x14,0x10,0x10,0x10,0x10,0x10},

    /* SharpenMF to SharpenEx */
    {0x30,0x30,0x30,0x30,0x28,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10},

    /* SharpenLF to SharpenEx */
    {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},

    /* SadAmplifier to SharpenEx */
    {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10}

};


static ISP_CMOS_YUV_SHARPEN_S g_stIspYuvSharpen =
{
     /* bvalid */
     1,

     /* 100,  200,    400,     800,    1600,    3200,    6400,    12800,    25600,   51200,  102400,  204800,   409600,   819200,   1638400,  3276800 */

     /* bEnLowLumaShoot */
     {0,     0,     0,     0,     0,     0,      0,      0,      0,     1,     1,      1,      1,      1,      1,     1},

     /* TextureSt */
     {50,   48,    46,    44,    48,    56,     56,     56,     48,    36,    20,      8,      4,      4,      4,     4},

     /* EdgeSt */
     {64,    64,       64,      64,     64,    64,        56,        56,        56,       48,      48,      48,      48,      32,      20,    20},

     /* OverShoot */
     {64,   64,    64,    64,    72,    72,     72,     72,     72,    64,    64,     56,     50,     40,     30,    30},

     /* UnderShoot */
     {64,   64,    64,    64,    72,    72,     72,     72,     72,    64,    64,     56,     50,     40,     30,    30},

     /* TextureThd */
     {80,   80,    80,    80,    80,    80,     90,     90,     90,   100,   100,    100,    100,    100,    110,   110},

     /* EdgeThd */
     {0,     0,     5,    10,    10,    10,     16,     20,     30,   40,     50,    50,      60,    60,     60,    60},

     /* JagCtrl */
     {63,   38,    20,    18,    16,    12,     10,      8,      8,     4,     4,      4,      2,      2,      2,     2},

     /* SaltCtrl */
     {50,   50,     50,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     50,     50,    50},

     /* PepperCtrl */
     {0,    0,       0,     20,     60,     60,     60,     80,    120,    160,    180,     180,   180,     180,   180,   180},

     /* DetailCtrl */
     {150,  150,   135,    135,    130,    130,    130,    130,    120,    120,    120,     120,    100,     50,    50,    50},

     /* LumaThd */
     {
         {20,    20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20}, /* LumaThd0 */
         {40,    40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40}, /* LumaThd1 */
         {65,    65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65}, /* LumaThd2 */
         {90,    90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90}  /* LumaThd3 */
     },

     /* LumaWgt */
     {
         {160,   160,    160,    150,    140,    130,    120,    110,    100,    100,     90,     90,     80,     80,     80,     80},
         {200,   200,    200,    180,    170,    160,    150,    150,    150,    150,    120,    120,    120,    120,    120,    120},
         {240,   240,    240,    200,    200,    190,    180,    180,    180,    180,    160,    160,    160,    160,    160,    160},
         {255,   255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255},
     }
};


static ISP_CMOS_YUV_SHARPEN_S g_stIspYuvSharpenFSWDR =
{
     /* bvalid */
     1,

     /* 100,  200,    400,     800,    1600,    3200,    6400,    12800,    25600,   51200,  102400,  204800,   409600,   819200,   1638400,  3276800 */

     /* bEnLowLumaShoot */
     {0,     0,     0,     0,     0,     0,      0,      0,      0,     1,     1,      1,      1,      1,      1,     1},

     /* TextureSt */
     {50,   48,    46,    44,    48,    56,     56,     56,     48,    36,    20,      8,      4,      4,      4,     4},

     /* EdgeSt */
     {64,    64,       64,      64,     64,    64,        56,        56,        56,       48,      48,      48,      48,      32,      20,    20},

     /* OverShoot */
     {32,   36,    40,    48,    48,    48,     44,     40,     40,    32,    32,     28,     25,     20,     20,    20},

     /* UnderShoot */
     {32,   36,    40,    48,    48,    48,     44,     40,     40,    32,    32,     28,     25,     20,     20,    20},

     /* TextureThd */
     {20,   24,    28,    32,    36,    40,     48,     56,     64,   128,   156,    156,    156,    160,    160,   160},

     /* EdgeThd */
     {0,     0,     0,    10,    10,    10,     16,     32,     64,   128,   156,    156,    156,    160,    160,   160},

     /* JagCtrl */
     {24,   22,    20,    18,    16,    12,     10,      8,      8,     4,     4,      4,      2,      2,      2,     2},

     /* SaltCtrl */
     {50,   50,     50,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     50,     50,    50},

     /* PepperCtrl */
     {0,    0,       0,     20,     60,     60,     60,     80,    120,    160,    180,     180,   180,     180,   180,   180},

     /* DetailCtrl */
     {140,  140,   135,    135,    130,    130,    130,    130,    120,    120,    120,     120,    100,     50,    50,    50},

     /* LumaThd */
     {
         {20,    20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20}, /* LumaThd0 */
         {40,    40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40}, /* LumaThd1 */
         {65,    65,     65,     65,     65,     65,     70,     75,     80,     80,     80,     80,     80,     80,     80,     80}, /* LumaThd2 */
         {90,    90,     90,     90,     90,     90,     100,   110,    120,    120,    120,    120,    120,    120,    120,    120}  /* LumaThd3 */
     },

     /* LumaWgt */
     {
         {160,   160,    160,    150,    120,     80,     40,     20,     10,    10,     10,     10,     10,     10,     10,     10},
         {200,   200,    200,    180,    160,    120,     80,     60,     40,    40,     40,     40,     40,     40,     40,     40},
         {240,   240,    240,    200,    200,    180,    160,    140,    128,   128,    128,    128,    128,    128,    128,    128},
         {255,   255,    255,    255,    255,    255,    255,    255,    255,   255,    255,    255,    255,    255,    255,    255},
     }
};


static ISP_CMOS_AGC_TABLE_S g_stIspAgcTableFSWDR =
{
    /* bvalid */
    1,

    /* snr_thresh */
    {0x8,0xC,0x10,0x14,0x18,0x20,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30},

    /* demosaic_np_offset */
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30},

    /* ge_strength */
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37},
};

static ISP_CMOS_NOISE_TABLE_S g_stIspNoiseTable =
{
    /* bvalid */
    1,

    /* nosie_profile_weight_lut */
    {
      0,0,0,0,0,0,0,8,14,18,21,24,26,27,28,30,31,32,33,34,34,36,36,37,38,38,39,39,
      40,40,41,41,42,42,42,43,43,44,44,44,45,45,45,46,46,46,46,47,47,47,48,48,48,48,49,49,49,
      49,49,50,50,50,50,51,51,51,51,51,51,52,52,52,52,52,53,53,53,53,53,53,53,54,54,54,54,54,
      54,55,55,55,55,55,55,55,55,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,57,57,58,58,58,
      58,58,58,58,58,58,59,59,59,59,59,59,59
    },

    /* demosaic_weight_lut */
    {
      0,8,14,18,21,24,26,27,28,30,31,32,33,34,34,36,36,37,38,38,39,39,40,40,41,41,42,42,42,
      43,43,44,44,44,45,45,45,46,46,46,46,47,47,47,48,48,48,48,49,49,49,49,49,50,50,50,50,
      51,51,51,51,51,51,52,52,52,52,52,53,53,53,53,53,53,53,54,54,54,54,54,54,55,55,55,55,
      55,55,55,55,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,57,57,58,58,58,58,58,58,58,
      58,58,59,59,59,59,59,59,59,59,59,59,59,59,59
    }
};

static ISP_CMOS_NOISE_TABLE_S g_stIspNoiseTableFSWDR =
{
    /* bvalid */
    1,

    /* nosie_profile_weight_lut */
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,7,10,13,15,18,21,23,26,29,31,34,37,39,42,45
    },

    /* demosaic_weight_lut */
    {
        0,2,15,20,24,27,30,32,33,35,36,37,38,39,40,41,42,42,43,44,44,45,45,46,
        46,47,47,48,48,49,49,49,50,50,50,51,51,51,52,52,52,53,53,53,53,54,54,54,54,55,55,
        55,55,56,56,56,56,56,57,57,57,57,57,58,58,58,58,58,59,59,59,59,59,59,59,60,60,60,
        60,60,60,61,61,61,61,61,61,61,62,62,62,62,62,62,62,62,63,63,63,63,63,63,63,63,63,
        64,64,64,64,64,64,64,64,64,65,65,65,65,65,65,65,65,65,65,65,65,65,65
    }
};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
    /* bvalid */
    1,

    /*vh_slope*/
    170,

    /*aa_slope*/
    166,

    /*va_slope*/
    155,

    /*uu_slope*/
    86,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xa0,

    /*fc_slope*/
    0x80,

    /*vh_thresh*/
    0,

    /*aa_thresh*/
    0,

    /*va_thresh*/
    0,

    /*uu_thresh*/
    0,

    /*sat_thresh*/
    0x00,

    /*ac_thresh*/
    0x1b3

};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaicFSWDR =
{
    /* bvalid */
    1,

    /*vh_slope*/
    170,

    /*aa_slope*/
    166,

    /*va_slope*/
    155,

    /*uu_slope*/
    86,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xa0,

    /*fc_slope*/
    0x80,

    /*vh_thresh*/
    0,

    /*aa_thresh*/
    0,

    /*va_thresh*/
    0,

    /*uu_thresh*/
    0,

    /*sat_thresh*/
    0x00,

    /*ac_thresh*/
    0x1b3
};

static ISP_CMOS_GAMMA_S g_stIspGamma =
{
    /* bvalid */
    1,

    {  0,   79,  160,  243,  327,  412,  497,  582,  666,  748,  828,  906,  980, 1051, 1118, 1180, 1236,
    1287, 1333, 1375, 1413, 1447, 1478, 1506, 1533, 1557, 1580, 1602, 1623, 1645, 1666, 1689, 1712, 1736,
    1758, 1780, 1800, 1820, 1839, 1857, 1875, 1892, 1909, 1926, 1943, 1959, 1976, 1993, 2010, 2027, 2044,
    2060, 2076, 2092, 2108, 2124, 2139, 2155, 2170, 2185, 2201, 2216, 2232, 2247, 2263, 2279, 2295, 2311,
    2328, 2344, 2361, 2377, 2394, 2410, 2426, 2442, 2458, 2473, 2488, 2503, 2517, 2531, 2544, 2557, 2570,
    2582, 2594, 2606, 2618, 2630, 2641, 2653, 2664, 2676, 2687, 2698, 2710, 2722, 2733, 2745, 2756, 2767,
    2779, 2790, 2801, 2812, 2824, 2835, 2846, 2857, 2867, 2878, 2889, 2900, 2910, 2921, 2931, 2941, 2952,
    2962, 2972, 2982, 2992, 3002, 3012, 3023, 3033, 3043, 3053, 3063, 3074, 3084, 3095, 3105, 3115, 3126,
    3136, 3147, 3157, 3167, 3177, 3187, 3197, 3207, 3216, 3225, 3234, 3242, 3250, 3259, 3266, 3274, 3282,
    3290, 3298, 3306, 3314, 3323, 3331, 3340, 3350, 3360, 3371, 3382, 3393, 3405, 3416, 3428, 3440, 3452,
    3464, 3476, 3487, 3499, 3509, 3519, 3529, 3538, 3547, 3555, 3563, 3570, 3577, 3584, 3591, 3598, 3605,
    3612, 3619, 3626, 3633, 3640, 3648, 3656, 3664, 3673, 3681, 3690, 3698, 3707, 3716, 3725, 3733, 3742,
    3750, 3758, 3767, 3774, 3782, 3789, 3797, 3804, 3811, 3817, 3824, 3831, 3837, 3843, 3850, 3856, 3862,
    3868, 3875, 3881, 3887, 3893, 3899, 3905, 3911, 3917, 3922, 3928, 3933, 3939, 3945, 3951, 3956, 3962,
    3968, 3975, 3981, 3988, 3994, 4001, 4008, 4015, 4022, 4029, 4037, 4044, 4051, 4059, 4066, 4073, 4081, 4088, 4095, }
};

static ISP_CMOS_GAMMA_S g_stIspGammaFSWDR =
{
    /* bvalid */
    1,

    {  0,   79,  160,  243,  327,  412,  497,  582,  666,  748,  828,  906,  980, 1051, 1118, 1180, 1236,
    1287, 1333, 1375, 1413, 1447, 1478, 1506, 1533, 1557, 1580, 1602, 1623, 1645, 1666, 1689, 1712, 1736,
    1758, 1780, 1800, 1820, 1839, 1857, 1875, 1892, 1909, 1926, 1943, 1959, 1976, 1993, 2010, 2027, 2044,
    2060, 2076, 2092, 2108, 2124, 2139, 2155, 2170, 2185, 2201, 2216, 2232, 2247, 2263, 2279, 2295, 2311,
    2328, 2344, 2361, 2377, 2394, 2410, 2426, 2442, 2458, 2473, 2488, 2503, 2517, 2531, 2544, 2557, 2570,
    2582, 2594, 2606, 2618, 2630, 2641, 2653, 2664, 2676, 2687, 2698, 2710, 2722, 2733, 2745, 2756, 2767,
    2779, 2790, 2801, 2812, 2824, 2835, 2846, 2857, 2867, 2878, 2889, 2900, 2910, 2921, 2931, 2941, 2952,
    2962, 2972, 2982, 2992, 3002, 3012, 3023, 3033, 3043, 3053, 3063, 3074, 3084, 3095, 3105, 3115, 3126,
    3136, 3147, 3157, 3167, 3177, 3187, 3197, 3207, 3216, 3225, 3234, 3242, 3250, 3259, 3266, 3274, 3282,
    3290, 3298, 3306, 3314, 3323, 3331, 3340, 3350, 3360, 3371, 3382, 3393, 3405, 3416, 3428, 3440, 3452,
    3464, 3476, 3487, 3499, 3509, 3519, 3529, 3538, 3547, 3555, 3563, 3570, 3577, 3584, 3591, 3598, 3605,
    3612, 3619, 3626, 3633, 3640, 3648, 3656, 3664, 3673, 3681, 3690, 3698, 3707, 3716, 3725, 3733, 3742,
    3750, 3758, 3767, 3774, 3782, 3789, 3797, 3804, 3811, 3817, 3824, 3831, 3837, 3843, 3850, 3856, 3862,
    3868, 3875, 3881, 3887, 3893, 3899, 3905, 3911, 3917, 3922, 3928, 3933, 3939, 3945, 3951, 3956, 3962,
    3968, 3975, 3981, 3988, 3994, 4001, 4008, 4015, 4022, 4029, 4037, 4044, 4051, 4059, 4066, 4073, 4081, 4088, 4095, }

};

static HI_U32 cmos_get_isp_default(ISP_DEV IspDev,ISP_CMOS_DEFAULT_S *pstDef)
{
    if (HI_NULL == pstDef)
    {
        printf("null pointer when get isp default value!\n");
        return -1;
    }

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    switch (g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:
            pstDef->stDrc.bEnable               = HI_FALSE;
            pstDef->stDrc.u32BlackLevel         = 0x00;
            pstDef->stDrc.u32WhiteLevel         = 0xB0000;
            pstDef->stDrc.u32SlopeMax           = 0x60;
            pstDef->stDrc.u32SlopeMin           = 0xD0;
            pstDef->stDrc.u32VarianceSpace      = 0x6;
            pstDef->stDrc.u32VarianceIntensity  = 0x8;
            pstDef->stDrc.u32Asymmetry          = 0x08;
            pstDef->stDrc.u32BrightEnhance      = 0xD2;
            pstDef->stDrc.bFilterMux            = 0x1;
            pstDef->stDrc.u32Svariance          = 0x8;
            pstDef->stDrc.u32BrightPr           = 0x60;
            pstDef->stDrc.u32Contrast           = 0x90;
            pstDef->stDrc.u32DarkEnhance        = 0x6000;

            memcpy(&pstDef->stAgcTbl, &g_stIspAgcTable, sizeof(ISP_CMOS_AGC_TABLE_S));
            memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTable, sizeof(ISP_CMOS_NOISE_TABLE_S));
            memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpen, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
            memcpy(&pstDef->stGamma, &g_stIspGamma, sizeof(ISP_CMOS_GAMMA_S));
        break;

        case WDR_MODE_2To1_LINE:
            pstDef->stDrc.bEnable               = HI_TRUE;
            pstDef->stDrc.u32BlackLevel         = 0x00;
            pstDef->stDrc.u32WhiteLevel         = 0xB0000;
            pstDef->stDrc.u32SlopeMax           = 0x60;
            pstDef->stDrc.u32SlopeMin           = 0xD0;
            pstDef->stDrc.u32VarianceSpace      = 0x06;
            pstDef->stDrc.u32VarianceIntensity  = 0x08;
            pstDef->stDrc.u32Asymmetry          = 0x08;
            pstDef->stDrc.u32BrightEnhance      = 0xE6;
            pstDef->stDrc.bFilterMux            = 0x1;
            pstDef->stDrc.u32Svariance          = 0xC;
            pstDef->stDrc.u32BrightPr           = 0x94;
            pstDef->stDrc.u32Contrast           = 0x80;
            pstDef->stDrc.u32DarkEnhance        = 0x6000;

            pstDef->stWDRAttr.au32ExpRatio[0]   = 0x400;
            pstDef->stWDRAttr.au32ExpRatio[1]   = 0x40;
            pstDef->stWDRAttr.au32ExpRatio[2]   = 0x40;

            memcpy(&pstDef->stAgcTbl, &g_stIspAgcTableFSWDR, sizeof(ISP_CMOS_AGC_TABLE_S));
            memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTableFSWDR, sizeof(ISP_CMOS_NOISE_TABLE_S));
            memcpy(&pstDef->stDemosaic, &g_stIspDemosaicFSWDR, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpenFSWDR, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpenFSWDR, sizeof(ISP_CMOS_YUV_SHARPEN_S));
            memcpy(&pstDef->stGamma, &g_stIspGammaFSWDR, sizeof(ISP_CMOS_GAMMA_S));
        break;

    }

    pstDef->stSensorMaxResolution.u32MaxWidth  = 3840;
    pstDef->stSensorMaxResolution.u32MaxHeight = 2160;
    pstDef->stSensorMode.u32SensorID = os08a10_ID;
    pstDef->stSensorMode.u8SensorMode = g_apstSnsState[IspDev]->u8ImgMode;

    return 0;
}

static HI_U32 cmos_get_isp_black_level(ISP_DEV IspDev,ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;

    if (HI_NULL == pstBlackLevel)
    {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    /* black level of linear mode */
    if (WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)
    {
        for (i=0; i<4; i++)
        {
            pstBlackLevel->au16BlackLevel[i] = 0x100;    // 256
        }
    }

    /* black level of DOL mode */
    else
    {
        pstBlackLevel->au16BlackLevel[0] = 0x100;
        pstBlackLevel->au16BlackLevel[1] = 0x100;
        pstBlackLevel->au16BlackLevel[2] = 0x100;
        pstBlackLevel->au16BlackLevel[3] = 0x100;
    }


    return 0;

}

static HI_VOID cmos_set_pixel_detect(ISP_DEV IspDev,HI_BOOL bEnable)
{

    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        return;
    }
    else
    {
        if (os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
        {
            u32FullLines_5Fps = (os08a10_VMAX_4k2k30_LINEAR * 30) / 5;
        }
        else
        {
            return;
        }
    }

    //u32FullLines_5Fps = (u32FullLines_5Fps > os08a10_FULL_LINES_MAX) ? os08a10_FULL_LINES_MAX : u32FullLines_5Fps;
    u32MaxIntTime_5Fps = u32FullLines_5Fps - 8;

    if (bEnable) /* setup for ISP pixel calibration mode */
    {
        os08a10_write_register (IspDev,os08a10_AGAIN_H_ADDR,0x00);
        os08a10_write_register (IspDev,os08a10_AGAIN_L_ADDR,0x80);

        os08a10_write_register (IspDev,os08a10_VMAX_L_ADDR, u32FullLines_5Fps & 0xFF);
        os08a10_write_register (IspDev,os08a10_VMAX_H_ADDR, (u32FullLines_5Fps & 0xFF00) >> 8);

        os08a10_write_register (IspDev,os08a10_EXPO_L_ADDR, u32MaxIntTime_5Fps & 0xFF);
        os08a10_write_register (IspDev,os08a10_EXPO_H_ADDR,  (u32MaxIntTime_5Fps & 0xFF00) >> 8);

    }
    else /* setup for ISP 'normal mode' */
    {
        g_apstSnsState[IspDev]->u32FLStd = (g_apstSnsState[IspDev]->u32FLStd > os08a10_FULL_LINES_MAX) ? os08a10_FULL_LINES_MAX : g_apstSnsState[IspDev]->u32FLStd;
        os08a10_write_register (IspDev,os08a10_VMAX_L_ADDR, g_apstSnsState[IspDev]->u32FLStd & 0xFF);
        os08a10_write_register (IspDev,os08a10_VMAX_H_ADDR, (g_apstSnsState[IspDev]->u32FLStd & 0xFF00) >> 8);
        g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_VOID cmos_set_wdr_mode(ISP_DEV IspDev,HI_U8 u8Mode)
{
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;

    switch(u8Mode)
    {
        case WDR_MODE_NONE:
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            g_apstSnsState[IspDev]->u32FLStd = os08a10_VMAX_4k2k30_LINEAR;
            g_apstSnsState[IspDev]->u8ImgMode = os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE;
        break;

        case WDR_MODE_2To1_LINE:
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_2To1_LINE;
            g_apstSnsState[IspDev]->u32FLStd = os08a10_VMAX_4k2k30_WDR;
            g_apstSnsState[IspDev]->u8ImgMode = os08a10_SENSOR_4k2k_30FPS_2t1_WDR_MODE;
        break;

        default:
            printf("NOT support this mode!\n");
            return;
        break;
    }

    g_apstSnsState[IspDev]->au32FL[0]= g_apstSnsState[IspDev]->u32FLStd;
    g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];
    memset(g_apstSnsState[IspDev]->au32WDRIntTime, 0, sizeof(g_apstSnsState[IspDev]->au32WDRIntTime));

    return;
}

static HI_U32 cmos_get_sns_regs_info(ISP_DEV IspDev,ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;

    if (HI_NULL == pstSnsRegsInfo)
    {
        printf("null pointer when get sns reg info!\n");
        return -1;
    }

    if ((HI_FALSE == g_apstSnsState[IspDev]->bSyncInit) || (HI_FALSE == pstSnsRegsInfo->bConfig))
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        g_apstSnsState[IspDev]->astRegsInfo[0].unComBus.s8I2cDev = g_aunos08a10BusInfo[IspDev].s8I2cDev;
        g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum = 10;

        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
        {
           g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum += 6;
           g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        }

        for (i=0; i<g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u8DevAddr = os08a10_i2c_addr;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32AddrByteNum = os08a10_addr_byte;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32DataByteNum = os08a10_data_byte;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u8IntPos = 1; /* config registers in EOF interrupt */
        }

        //Linear Mode Regs
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32RegAddr = os08a10_EXPO_H_ADDR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32RegAddr = os08a10_EXPO_L_ADDR;

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32RegAddr = os08a10_AGAIN_H_ADDR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32RegAddr = os08a10_AGAIN_L_ADDR;

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32RegAddr = os08a10_DGAIN_H_ADDR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32RegAddr = os08a10_DGAIN_L_ADDR;

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32RegAddr = os08a10_VMAX_H_ADDR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32RegAddr = os08a10_VMAX_L_ADDR;

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32RegAddr = os08a10_R3740_ADDR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32RegAddr = os08a10_R3741_ADDR;

        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32RegAddr = os08a10_SHORT_EXPO_H_ADDR;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u32RegAddr = os08a10_SHORT_EXPO_L_ADDR;

            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u32RegAddr = os08a10_SHORT_AGAIN_H_ADDR;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u32RegAddr = os08a10_SHORT_AGAIN_L_ADDR;

            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u32RegAddr = os08a10_SHORT_DGAIN_H_ADDR;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u32RegAddr = os08a10_SHORT_DGAIN_L_ADDR;
        }

        g_apstSnsState[IspDev]->bSyncInit = HI_TRUE;
    }
    else
    {
        for (i=0; i<g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            if (g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32Data == g_apstSnsState[IspDev]->astRegsInfo[1].astI2cData[i].u32Data)
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
            }
            else
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            }
        }
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy(pstSnsRegsInfo, &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&g_apstSnsState[IspDev]->astRegsInfo[1], &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];

    return 0;
}

static HI_S32 cmos_set_image_mode(ISP_DEV IspDev,ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = g_apstSnsState[IspDev]->u8ImgMode;

    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;

    if (HI_NULL == pstSensorImageMode )
    {
        printf("null pointer when set image mode\n");
        return -1;
    }

    if ((pstSensorImageMode->u16Width <= 3840) && (pstSensorImageMode->u16Height <= 2160) && (pstSensorImageMode->f32Fps <= 30))
    {
        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
        {
            u8SensorImageMode = os08a10_SENSOR_4k2k_30FPS_2t1_WDR_MODE;
        }
        else if (WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)
        {
            u8SensorImageMode = os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE;
        }
        else
        {
            printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d, SensorImageMode:%d\n",
                pstSensorImageMode->u16Width,
                pstSensorImageMode->u16Height,
                pstSensorImageMode->f32Fps,
                g_apstSnsState[IspDev]->enWDRMode,
                u8SensorImageMode);

            return -1;
        }

        printf("Success! Width:%d, Height:%d, Fps:%f, WDRMode:%d, SensorImageMode:%d\n",
            pstSensorImageMode->u16Width,
            pstSensorImageMode->u16Height,
            pstSensorImageMode->f32Fps,
            g_apstSnsState[IspDev]->enWDRMode,
            u8SensorImageMode);
    }
    else
    {
        printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d, SensorImageMode:%d\n",
            pstSensorImageMode->u16Width,
            pstSensorImageMode->u16Height,
            pstSensorImageMode->f32Fps,
            g_apstSnsState[IspDev]->enWDRMode,
            u8SensorImageMode);

        return -1;
    }

    if ((HI_TRUE == g_apstSnsState[IspDev]->bInit) && (u8SensorImageMode == g_apstSnsState[IspDev]->u8ImgMode))
    {
        /* Don't need to switch SensorImageMode */
        return -1;
    }
    g_apstSnsState[IspDev]->u8ImgMode = u8SensorImageMode;
    return 0;
}

static HI_VOID sensor_global_init(ISP_DEV IspDev)
{
    g_apstSnsState[IspDev]->bInit = HI_FALSE;
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    g_apstSnsState[IspDev]->u8ImgMode = os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE;
    g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
    g_apstSnsState[IspDev]->u32FLStd = os08a10_VMAX_4k2k30_LINEAR;
    g_apstSnsState[IspDev]->au32FL[0] = os08a10_VMAX_4k2k30_LINEAR;
    g_apstSnsState[IspDev]->au32FL[1] = os08a10_VMAX_4k2k30_LINEAR;

    memset(&g_apstSnsState[IspDev]->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_apstSnsState[IspDev]->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = os08a10_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = os08a10_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return 0;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static int os08a10_set_bus_info(ISP_DEV IspDev, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunos08a10BusInfo[IspDev].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return 0;
}

static int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, os08a10_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }

    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, pstAeLib, os08a10_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, pstAwbLib, os08a10_ID, &stAwbRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return 0;
}

static int sensor_unregister_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, os08a10_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, pstAeLib, os08a10_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, pstAwbLib, os08a10_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    return 0;
}

static int sensor_set_init(ISP_DEV IspDev, ISP_INIT_ATTR_S *pstInitAttr)
{
    g_au32InitExposure[IspDev] = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[IspDev] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[IspDev][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[IspDev][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[IspDev][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[IspDev] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[IspDev] = pstInitAttr->u16SampleBgain;

    return 0;
}

ISP_SNS_OBJ_S stSnsOs08a10Obj =
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = os08a10_standby,
    .pfnRestart             = os08a10_restart,
    .pfnWriteReg            = os08a10_write_register,
    .pfnReadReg             = os08a10_read_register,
    .pfnSetBusInfo          = os08a10_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __os08a10_CMOS_H_ */
