#if !defined(__OV4689_CMOS_H_)
#define __OV4689_CMOS_H_

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
#include "ov4689_slave_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define OV4689_ID 4689
#define FSIN_DELAY_FRAMES 3

ISP_SNS_STATE_S             g_astOv4689[ISP_MAX_DEV_NUM] = {{0}};
static ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_DEV_NUM] = {&g_astOv4689[0], &g_astOv4689[1]};

ISP_SNS_COMMBUS_U g_aunOv4689BusInfo[ISP_MAX_DEV_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1] = { .s8I2cDev = 1}
};

/* make real fps less than stand fps because of NVR require */
#define OV4689_INCREASE_LINES (0)

const OV4689_VIDEO_MODE_TBL_S g_astOv4689ModeTbl[OV4689_MODE_BUTT+1] = {
    
    //{0xc3500,  1175+OV4689_INCREASE_LINES, 30, "1080P_4CH_12BIT_30FPS"},
    //{0xc3500,  1554+OV4689_INCREASE_LINES, 30, "4M_4CH_12BIT_30FPS"   }, 
    {0xc3500,  1175+OV4689_INCREASE_LINES, 30, "1080P_4CH_12BIT_30FPS"},
    {0xc3500,  1600+OV4689_INCREASE_LINES, 30, "4M_4CH_12BIT_30FPS"   }, 
    { -1,       -1,   -1, "NOT SUPPORT"         },
};

ISP_SLAVE_SNS_SYNC_S gstOv4689Sync[ISP_MAX_DEV_NUM];

HI_U8 gu8SetFpsCount[ISP_MAX_DEV_NUM] = {0,0};
HI_U8 gu8SlowShutterCount[ISP_MAX_DEV_NUM] = {0,0};
HI_U8 gu8SetPixellowfpsCount[ISP_MAX_DEV_NUM] = {0,0};
HI_U8 gu8SetPixelstdfpsCount[ISP_MAX_DEV_NUM] = {0,0};

HI_BOOL gbSetFpsFlag[ISP_MAX_DEV_NUM] = {0,0};
HI_BOOL gbSlowShutterFlag[ISP_MAX_DEV_NUM] = {0,0};
HI_BOOL gbSetPixellowfpsFlag[ISP_MAX_DEV_NUM] = {0,0};
HI_BOOL gbSetPixelstdfpsFlag[ISP_MAX_DEV_NUM] = {0,0}; 

static HI_U32 g_au32InitExposure[ISP_MAX_DEV_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_DEV_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_DEV_NUM] = {0};

/****************************************************************************
 * extern                                                            *
 ****************************************************************************/

extern unsigned char ov4689_i2c_addr;
extern unsigned int  ov4689_addr_byte;
extern unsigned int  ov4689_data_byte;

extern void ov4689_init(ISP_DEV IspDev);
extern void ov4689_exit(ISP_DEV IspDev);
extern void ov4689_standby(ISP_DEV IspDev);
extern void ov4689_restart(ISP_DEV IspDev);
extern int  ov4689_write_register(ISP_DEV IspDev, int addr, int data);
extern int  ov4689_read_register(ISP_DEV IspDev, int addr);

static HI_S32 cmos_get_ae_default(ISP_DEV IspDev, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return -1;
    }

    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32FullLinesMax = OV4689_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;
    
    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; 
    
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 4;
    pstAeSnsDft->u32MinIntTime = 2;
    pstAeSnsDft->u32MaxIntTimeTarget = OV4689_FULL_LINES_MAX;
    pstAeSnsDft->u32MinIntTimeTarget = 2;

    pstAeSnsDft->u32MaxAgain = 16832;  
    pstAeSnsDft->u32MinAgain = 1024;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 1;  
    pstAeSnsDft->u32MinDgain = 1;
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if(g_au32InitExposure[IspDev] == 0)
	{
		pstAeSnsDft->u32InitExposure = 200000;
	}
	else
	{
		pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev];
	}

    if(g_au32LinesPer500ms[IspDev] == 0)
	{    
    	pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd*30/2;
	}
	else
	{
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[IspDev];
	}

    switch(g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:   /*linear mode*/
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;
           
            pstAeSnsDft->u8AeCompensation = 0x38;
        break;

    }
	
    return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(ISP_DEV IspDev, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
     if(gu8SetFpsCount[IspDev] != 0)
     {
         return;
     }

     HI_FLOAT f32MODE0Comp = 0;
     HI_FLOAT f32MODE1Comp = 0;

     //f32MODE0Comp = 340;
     //f32MODE1Comp = 256;
  
     if(g_apstSnsState[IspDev]->enWDRMode == WDR_MODE_NONE)
     {
         if ((f32Fps <= 30) && (f32Fps >= 1.5) && g_apstSnsState[IspDev]->u8ImgMode == OV4689_MODE0)
         {
             gstOv4689Sync[IspDev].u32VsTime = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * 30 / f32Fps + f32MODE0Comp;
             g_apstSnsState[IspDev]->u32FLStd = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines * 30) / f32Fps;
         }

         else if((f32Fps <= 30) && (f32Fps >= 1.5) && g_apstSnsState[IspDev]->u8ImgMode == OV4689_MODE1)     //FPGA Phase 4M Only 15fps
         {
             gstOv4689Sync[IspDev].u32VsTime = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * 30 / f32Fps + f32MODE1Comp;
             g_apstSnsState[IspDev]->u32FLStd = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines * 30) / f32Fps;
         }
         
         else
         {
             printf("Not support Fps: %f\n", f32Fps);
             return;
         }
           
     }
    
    g_apstSnsState[IspDev]->u32FLStd = g_apstSnsState[IspDev]->u32FLStd > OV4689_FULL_LINES_MAX ? OV4689_FULL_LINES_MAX : g_apstSnsState[IspDev]->u32FLStd;

    // Before Set vstime and Vmax, disable FSIN
    
    if (WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = (g_apstSnsState[IspDev]->u32FLStd & 0xFF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = (g_apstSnsState[IspDev]->u32FLStd & 0x7F00) >> 8;
    }
    else
    {}

    g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstOv4689Sync[IspDev].u32VsTime;

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x10; //Disable Fsin Register
    gbSetFpsFlag[IspDev] = HI_TRUE;
   
    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 4;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    g_apstSnsState[IspDev]->au32FL[0] = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];

    return;
}

static HI_VOID cmos_slow_framerate_set(ISP_DEV IspDev, HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if(gu8SlowShutterCount[IspDev] != 0)
    {
        return;
    }
    u32FullLines = (u32FullLines > OV4689_FULL_LINES_MAX) ? OV4689_FULL_LINES_MAX : u32FullLines;
    g_apstSnsState[IspDev]->au32FL[0] = u32FullLines;
    
    if (WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = (g_apstSnsState[IspDev]->au32FL[0] & 0xFF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = (g_apstSnsState[IspDev]->au32FL[0] & 0x7F00) >> 8;
    }
    else
    {}

    g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime = (HI_U64)gstOv4689Sync[IspDev].u32VsTime * u32FullLines/ (DIV_0_TO_1(g_apstSnsState[IspDev]->u32FLStd));

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x10;  //Disable Fsin Register
    gbSlowShutterFlag[IspDev] = HI_TRUE;

    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->au32FL[0] - 4;
    
    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(ISP_DEV IspDev, HI_U32 u32IntTime)
{
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = u32IntTime >> 12;
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime & 0xFF0) >> 4;
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = (u32IntTime & 0xF) << 4;

    return;
}

static HI_U32 gain_table[512]=
{
     1024,  1032,  1040,  1048,  1056,  1064,  1072,  1080,  1088,  1096,  1104,  1112,  1120,  1128,
     1136,  1144,  1152,  1160,  1168,  1176,  1184,  1192,  1200,  1208,  1216,  1224,  1232,  1240,
     1248,  1256,  1264,  1272,  1280,  1288,  1296,  1304,  1312,  1320,  1328,  1336,  1344,  1352,
     1360,  1368,  1376,  1384,  1392,  1400,  1408,  1416,  1424,  1432,  1440,  1448,  1456,  1464,
     1472,  1480,  1488,  1496,  1504,  1512,  1520,  1528,  1536,  1544,  1552,  1560,  1568,  1576,
     1584,  1592,  1600,  1608,  1616,  1624,  1632,  1640,  1648,  1656,  1664,  1672,  1680,  1688,
     1696,  1704,  1712,  1720,  1728,  1736,  1744,  1752,  1760,  1768,  1776,  1784,  1792,  1800,
     1808,  1816,  1824,  1832,  1840,  1848,  1856,  1864,  1872,  1880,  1888,  1896,  1904,  1912,
     1920,  1928,  1936,  1944,  1952,  1960,  1968,  1976,  1984,  1992,  2000,  2008,  2016,  2024,
     2032,  2040,  2048,  2064,  2080,  2096,  2112,  2128,  2144,  2160,  2176,  2192,  2208,  2224,
     2240,  2256,  2272,  2288,  2304,  2320,  2336,  2352,  2368,  2384,  2400,  2416,  2432,  2448,
     2464,  2480,  2496,  2512,  2528,  2544,  2560,  2576,  2592,  2608,  2624,  2640,  2656,  2672,
     2688,  2704,  2720,  2736,  2752,  2768,  2784,  2800,  2816,  2832,  2848,  2864,  2880,  2896,
     2912,  2928,  2944,  2960,  2976,  2992,  3008,  3024,  3040,  3056,  3072,  3088,  3104,  3120,
     3136,  3152,  3168,  3184,  3200,  3216,  3232,  3248,  3264,  3280,  3296,  3312,  3328,  3344,
     3360,  3376,  3392,  3408,  3424,  3440,  3456,  3472,  3488,  3504,  3520,  3536,  3552,  3568,
     3584,  3600,  3616,  3632,  3648,  3664,  3680,  3696,  3712,  3728,  3744,  3760,  3776,  3792,
     3808,  3824,  3840,  3856,  3872,  3888,  3904,  3920,  3936,  3952,  3968,  3984,  4000,  4016,
     4032,  4048,  4064,  4080,  4096,  4128,  4160,  4192,  4224,  4256,  4288,  4320,  4352,  4384,
     4416,  4448,  4480,  4512,  4544,  4576,  4608,  4640,  4672,  4704,  4736,  4768,  4800,  4832,
     4864,  4896,  4928,  4960,  4992,  5024,  5056,  5088,  5120,  5152,  5184,  5216,  5248,  5280,
     5312,  5344,  5376,  5408,  5440,  5472,  5504,  5536,  5568,  5600,  5632,  5664,  5696,  5728,
     5760,  5792,  5824,  5856,  5888,  5920,  5952,  5984,  6016,  6048,  6080,  6112,  6144,  6176,
     6208,  6240,  6272,  6304,  6336,  6368,  6400,  6432,  6464,  6496,  6528,  6560,  6592,  6624,
     6656,  6688,  6720,  6752,  6784,  6816,  6848,  6880,  6912,  6944,  6976,  7008,  7040,  7072,
     7104,  7136,  7168,  7200,  7232,  7264,  7296,  7328,  7360,  7392,  7424,  7456,  7488,  7520,
     7552,  7584,  7616,  7648,  7680,  7712,  7744,  7776,  7808,  7840,  7872,  7904,  7936,  7968,
     8000,  8032,  8064,  8096,  8128,  8160,  8192,  8256,  8320,  8384,  8448,  8512,  8576,  8640,
     8704,  8768,  8832,  8896,  8960,  9024,  9088,  9152,  9216,  9280,  9344,  9408,  9472,  9536,
     9600,  9664,  9728,  9792,  9856,  9920,  9984, 10048, 10112, 10176, 10240, 10304, 10368, 10432,
     10496, 10560, 10624, 10688, 10752, 10816, 10880, 10944, 11008, 11072, 11136, 11200, 11264, 11328,
     11392, 11456, 11520, 11584, 11648, 11712, 11776, 11840, 11904, 11968, 12032, 12096, 12160, 12224,
     12288, 12352, 12416, 12480, 12544, 12608, 12672, 12736, 12800, 12864, 12928, 12992, 13056, 13120,
     13184, 13248, 13312, 13376, 13440, 13504, 13568, 13632, 13696, 13760, 13824, 13888, 13952, 14016,
     14080, 14144, 14208, 14272, 14336, 14400, 14464, 14528, 14592, 14656, 14720, 14784, 14848, 14912,
     14976, 15040, 15104, 15168, 15232, 15296, 15360, 15424, 15488, 15552, 15616, 15680, 15744, 15808,
     15872, 15936, 16000, 16064, 16128, 16192, 16256, 16320//, 16384, 16448, 16512, 16576, 16640, 16704,
     //16768, 16832
};


static HI_VOID cmos_again_calc_table(ISP_DEV IspDev, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    if (*pu32AgainLin >= gain_table[511])
    {
         *pu32AgainLin = gain_table[511];
         *pu32AgainDb = 511;
         return ;
    }
    
    for (i = 1; i < 512; i++)
    {
        if (*pu32AgainLin < gain_table[i])
        {
            *pu32AgainLin = gain_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }

    
    return;
}

static HI_VOID cmos_gains_update(ISP_DEV IspDev, HI_U32 u32Again, HI_U32 u32Dgain)
{
    HI_U32 u32AGainReg = 0;

    if(u32Again < 128)
    {
        u32AGainReg = 0;
        u32Again += 128;    //128~255
    }
    else if(u32Again < 256)
    {
        u32AGainReg = 1;
        u32Again -= 8;      //120~247
    }
    else if(u32Again < 384)
    {
        u32AGainReg = 3;
        u32Again -= 140;    //116~243
    }
    else
    {
        u32AGainReg = 7;
        u32Again -= 264;    //120~255
    }

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32Data = u32AGainReg;
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32Data = u32Again; 

    return;
}

/* Only used in WDR_MODE_2To1_LINE mode */
static HI_VOID cmos_get_inttime_max(ISP_DEV IspDev, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32IntTimeMaxTmp = 0;
    
    if((HI_NULL == au32Ratio) || (HI_NULL == au32IntTimeMax) || (HI_NULL == au32IntTimeMin))
    {
        printf("null pointer when get ae sensor ExpRatio/IntTimeMax/IntTimeMin value!\n");
        return;
    }
    
    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        /* PreShort + Long < 1V - 4  */
        /* Short + Long < 1V - 4; 
           Ratio = Long * 0x40 / Short */
        u32IntTimeMaxTmp = ((g_apstSnsState[IspDev]->au32FL[0] - 50 - g_apstSnsState[IspDev]->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
        au32IntTimeMax[0] = (g_apstSnsState[IspDev]->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40);
        if(g_apstSnsState[IspDev]->u8ImgMode == OV4689_MODE0)
        {
            au32IntTimeMax[0] = (g_apstSnsState[IspDev]->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40);
        }

        au32IntTimeMax[0] = (u32IntTimeMaxTmp < au32IntTimeMax[0]) ? u32IntTimeMaxTmp : au32IntTimeMax[0];
    }

    
    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set= cmos_slow_framerate_set;    
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;

    return 0;
}


static AWB_CCM_S g_stAwbCcm =
{
   4850,
   {
        0x01bf, 0x809f, 0x8020,
        0x804e, 0x01a8, 0x805a,
        0x0010, 0x80ee, 0x01dc
   },
   
   3100,
   {
        0x01b7, 0x807c, 0x803a,
        0x807e, 0x01c0, 0x8042,
        0x001c, 0x813c, 0x021f
   },
   
   2450,
   {     
        0x0183, 0x8078, 0x800a,
        0x8073, 0x0186, 0x8013,
        0x0010, 0x820a, 0x02fa
   }
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /* saturation */ 
    {0x80,0x80,0x7e,0x72,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38}
};

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0    
#define GOLDEN_BGAIN 0   

static HI_S32 cmos_get_awb_default(ISP_DEV IspDev, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    if (HI_NULL == pstAwbSnsDft)
    {
        printf("null pointer when get awb default value!\n");
        return -1;
    }

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4850;

    pstAwbSnsDft->au16GainOffset[0] = 0x212;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100;
    pstAwbSnsDft->au16GainOffset[3] = 0x1c8;

    pstAwbSnsDft->as32WbPara[0] = 63;
    pstAwbSnsDft->as32WbPara[1] = 76;
    pstAwbSnsDft->as32WbPara[2] = -117;
    pstAwbSnsDft->as32WbPara[3] = 119178;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -73499;

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));

    switch (g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
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
     {64,	64,	   64,	  64,	 64,	64,	    56,		56,		56,	   48,	  48,	  48,	  48,	  32,	  20,    20},
         
     /* OverShoot */
     {64,   64,    64,    64,    72,    72,     72,     72,     72,    64,    64,     56,     50,     40,     30,    30},
        
     /* UnderShoot */
     {64,   64,    64,    64,    72,    72,     72,     72,     72,    64,    64,     56,     50,     40,     30,    30},
         
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

static ISP_CMOS_NOISE_TABLE_S g_stIspNoiseTable =
{
    /* bvalid */
    1,

    /* nosie_profile_weight_lut */
    {0x0, 0x0, 0x8, 0xF, 0x13, 0x16, 0x19, 0x1A, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x23, 0x25, 
    0x25, 0x26, 0x27, 0x27, 0x27, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 
    0x2D, 0x2D, 0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 
    0x32, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x34, 0x34, 0x35, 0x35, 0x35, 
    0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x38,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x3A, 0x3A, 
    0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 
    0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3D, 0x3D},   

    /* demosaic_weight_lut */
    {0x8, 0xF, 0x13, 0x16, 0x19, 0x1A, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x23, 0x25, 0x25, 0x26, 
    0x27, 0x27, 0x27, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 
    0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 
    0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x34, 0x34, 0x35, 0x35, 0x35, 0x35, 0x35, 
    0x35, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 
    0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x3A, 0x3A, 0x3A, 0x3A, 
    0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3C, 0x3C, 
    0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3D, 0x3D, 0x3D, 0x3D}        
    
};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
    /* bvalid */
    1,
    
    /*vh_slope*/
    0xac,

    /*aa_slope*/
    0xaa,

    /*va_slope*/
    0xa8,

    /*uu_slope*/
    0x55,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xa0,
    
    /*fc_slope*/
    0x80,

    /*vh_thresh*/
    0x00,

    /*aa_thresh*/
    0x00,

    /*va_thresh*/
    0x00,

    /*uu_thresh*/
    0x08,

    /*sat_thresh*/
    0x00,

    /*ac_thresh*/
    0x1b3
};

static ISP_CMOS_GAMMA_S g_stIspGamma =
{
    /* bvalid */
    1,
    
#if 1 /* Normal mode */   
    {  0, 180, 320, 426, 516, 590, 660, 730, 786, 844, 896, 946, 994, 1040, 1090, 1130, 1170, 1210, 1248,
    1296, 1336, 1372, 1416, 1452, 1486, 1516, 1546, 1580, 1616, 1652, 1678, 1714, 1742, 1776, 1798, 1830,
    1862, 1886, 1912, 1940, 1968, 1992, 2010, 2038, 2062, 2090, 2114, 2134, 2158, 2178, 2202, 2222, 2246,
    2266, 2282, 2300, 2324, 2344, 2360, 2372, 2390, 2406, 2422, 2438, 2458, 2478, 2494, 2510, 2526, 2546,
    2562, 2582, 2598, 2614, 2630, 2648, 2660, 2670, 2682, 2698, 2710, 2724, 2736, 2752, 2764, 2780, 2792,
    2808, 2820, 2836, 2848, 2864, 2876, 2888, 2896, 2908, 2920, 2928, 2940, 2948, 2960, 2972, 2984, 2992,
    3004, 3014, 3028, 3036, 3048, 3056, 3068, 3080, 3088, 3100, 3110, 3120, 3128, 3140, 3148, 3160, 3168,
    3174, 3182, 3190, 3202, 3210, 3218, 3228, 3240, 3256, 3266, 3276, 3288, 3300, 3306, 3318, 3326, 3334,
    3342, 3350, 3360, 3370, 3378, 3386, 3394, 3398, 3406, 3414, 3422, 3426, 3436, 3444, 3454, 3466, 3476,
    3486, 3498, 3502, 3510, 3518, 3526, 3530, 3538, 3546, 3554, 3558, 3564, 3570, 3574, 3582, 3590, 3598,
    3604, 3610, 3618, 3628, 3634, 3640, 3644, 3652, 3656, 3664, 3670, 3678, 3688, 3696, 3700, 3708, 3712,
    3716, 3722, 3730, 3736, 3740, 3748, 3752, 3756, 3760, 3766, 3774, 3778, 3786, 3790, 3800, 3808, 3812,
    3816, 3824, 3830, 3832, 3842, 3846, 3850, 3854, 3858, 3862, 3864, 3870, 3874, 3878, 3882, 3888, 3894,
    3900, 3908, 3912, 3918, 3924, 3928, 3934, 3940, 3946, 3952, 3958, 3966, 3974, 3978, 3982, 3986, 3990,
    3994, 4002, 4006, 4010, 4018, 4022, 4032, 4038, 4046, 4050, 4056, 4062, 4072, 4076, 4084, 4090, 4095}
#else  /* Infrared or Spotlight mode */
    {  0, 120, 220, 320, 416, 512, 592, 664, 736, 808, 880, 944, 1004, 1062, 1124, 1174,
    1226, 1276, 1328, 1380, 1432, 1472, 1516, 1556, 1596, 1636, 1680, 1720, 1756, 1792,
    1828, 1864, 1896, 1932, 1968, 2004, 2032, 2056, 2082, 2110, 2138, 2162, 2190, 2218,
    2242, 2270, 2294, 2314, 2338, 2358, 2382, 2402, 2426, 2446, 2470, 2490, 2514, 2534,
    2550, 2570, 2586, 2606, 2622, 2638, 2658, 2674, 2694, 2710, 2726, 2746, 2762, 2782,
    2798, 2814, 2826, 2842, 2854, 2870, 2882, 2898, 2910, 2924, 2936, 2952, 2964, 2980,
    2992, 3008, 3020, 3036, 3048, 3064, 3076, 3088, 3096, 3108, 3120, 3128, 3140, 3152,
    3160, 3172, 3184, 3192, 3204, 3216, 3224, 3236, 3248, 3256, 3268, 3280, 3288, 3300,
    3312, 3320, 3332, 3340, 3348, 3360, 3368, 3374, 3382, 3390, 3402, 3410, 3418, 3426,
    3434, 3446, 3454, 3462, 3470, 3478, 3486, 3498, 3506, 3514, 3522, 3530, 3542, 3550,
    3558, 3566, 3574, 3578, 3586, 3594, 3602, 3606, 3614, 3622, 3630, 3634, 3642, 3650,
    3658, 3662, 3670, 3678, 3686, 3690, 3698, 3706, 3710, 3718, 3722, 3726, 3734, 3738,
    3742, 3750, 3754, 3760, 3764, 3768, 3776, 3780, 3784, 3792, 3796, 3800, 3804, 3812,
    3816, 3820, 3824, 3832, 3836, 3840, 3844, 3848, 3856, 3860, 3864, 3868, 3872, 3876,
    3880, 3884, 3892, 3896, 3900, 3904, 3908, 3912, 3916, 3920, 3924, 3928, 3932, 3936,
    3940, 3944, 3948, 3952, 3956, 3960, 3964, 3968, 3972, 3972, 3976, 3980, 3984, 3988,
    3992, 3996, 4000, 4004, 4008, 4012, 4016, 4020, 4024, 4028, 4032, 4032, 4036, 4040,
    4044, 4048, 4052, 4056, 4056, 4060, 4064, 4068, 4072, 4072, 4076, 4080, 4084, 4086,
    4088, 4092, 4095} 
#endif
};

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam =
{
    {378, 256, 430},
    {439, 256, 439}
};

static HI_U32 cmos_get_isp_default(ISP_DEV IspDev, ISP_CMOS_DEFAULT_S *pstDef)
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
   			 pstDef->stDrc.u32WhiteLevel         = 0xD0000; 
   			 pstDef->stDrc.u32SlopeMax           = 0x30;
   			 pstDef->stDrc.u32SlopeMin           = 0x00;
   			 pstDef->stDrc.u32VarianceSpace      = 0x04;
   			 pstDef->stDrc.u32VarianceIntensity  = 0x01;
   			 pstDef->stDrc.u32Asymmetry          = 0x08;
   			 pstDef->stDrc.u32BrightEnhance      = 0xE6;
   			 pstDef->stDrc.bFilterMux            = 0x1;
   			 pstDef->stDrc.u32Svariance          = 0x8;
   			 pstDef->stDrc.u32BrightPr           = 0xB0;
   			 pstDef->stDrc.u32Contrast           = 0xB0;
   			 pstDef->stDrc.u32DarkEnhance        = 0x8000;

             memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTable, sizeof(ISP_CMOS_NOISE_TABLE_S));            
             memcpy(&pstDef->stAgcTbl, &g_stIspAgcTable, sizeof(ISP_CMOS_AGC_TABLE_S));
             memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
			 memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpen, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
			 memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
             memcpy(&pstDef->stGamma, &g_stIspGamma, sizeof(ISP_CMOS_GAMMA_S));            
        break;
    }
    
    pstDef->stSensorMaxResolution.u32MaxWidth  = 2592;
    pstDef->stSensorMaxResolution.u32MaxHeight = 1520;
    pstDef->stSensorMode.u32SensorID = OV4689_ID;
    pstDef->stSensorMode.u8SensorMode = g_apstSnsState[IspDev]->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));
    switch (g_apstSnsState[IspDev]->u8ImgMode)
    {
        default:
        case OV4689_MODE0:
        case OV4689_MODE1:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; 
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
        break;
    }
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimRows = 2;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimCols = 2;
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2;
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2;
    pstDef->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 2;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 0;
    pstDef->stSensorMode.bValidDngRawFormat = HI_TRUE;
    
    return 0;
}


static HI_U32 cmos_get_isp_black_level(ISP_DEV IspDev, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    
    if (HI_NULL == pstBlackLevel)
    {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;
          
    for (i=0; i<4; i++)
    {
        pstBlackLevel->au16BlackLevel[i] = 0x40; /*12bit,0x40 */
    }

    return 0;    
}

static HI_VOID cmos_set_pixel_detect(ISP_DEV IspDev, HI_BOOL bEnable)
{
  
    HI_U32 u32FullLines_5Fps = 0;
    HI_U32 u32MaxIntTime_5Fps = 0;
    HI_U32 u32Vstime_5Fps = 0;
    
    if (OV4689_MODE1 == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32FullLines_5Fps = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines * 30) / 5;
        u32Vstime_5Fps = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * 6;
    }
    else if (OV4689_MODE0 == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32FullLines_5Fps = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines  * 30) / 5;
        u32Vstime_5Fps = (g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * 6;
    }
    else
    {
        return;
    }

    u32FullLines_5Fps = (u32FullLines_5Fps > OV4689_FULL_LINES_MAX) ? OV4689_FULL_LINES_MAX : u32FullLines_5Fps;
    u32MaxIntTime_5Fps = u32FullLines_5Fps - 4;

    if (bEnable) /* setup for ISP pixel calibration mode */
    {
        // Before Set vstime and Vmax, disable fsin
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x10;  //Disable Fsin
        gbSetPixellowfpsFlag[IspDev] = HI_TRUE;

        //vmax
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = u32FullLines_5Fps & 0xFF;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = (u32FullLines_5Fps & 0x7F00) >> 8;

        //vstime
        g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime = u32Vstime_5Fps;

        //Shutter time
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = (u32MaxIntTime_5Fps>>12);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = (u32MaxIntTime_5Fps & 0xFF0) >> 4;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = (u32MaxIntTime_5Fps & 0xF) << 4;

        //Again Reg and Again
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32Data = 0x00;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32Data = 0x80;
   }
   else /* setup for ISP 'normal mode' */
   {
        // Before Set vstime and vmax, disable FSIN
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x10;
        gbSetPixelstdfpsFlag[IspDev] = HI_TRUE;

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = g_apstSnsState[IspDev]->u32FLStd & 0xFF;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = (g_apstSnsState[IspDev]->u32FLStd & 0x7F00) >> 8;
        
        g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstOv4689Sync[IspDev].u32VsTime;

        printf("u32FLStd = %d u32SlaveVsTime = %d\n",g_apstSnsState[IspDev]->u32FLStd,
            gstOv4689Sync[IspDev].u32VsTime);
        
        g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_VOID cmos_set_wdr_mode(ISP_DEV IspDev, HI_U8 u8Mode)
{
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    
    switch(u8Mode)
    {
        case WDR_MODE_NONE:
			g_apstSnsState[IspDev]->u32FLStd = g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines;
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
        break;
     
        default:
            printf("NOT support this mode!\n");
            return;
        break;
    }

    g_apstSnsState[IspDev]->au32FL[0] = g_apstSnsState[IspDev]->u32FLStd;
    g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];
    memset(g_apstSnsState[IspDev]->au32WDRIntTime, 0, sizeof(g_apstSnsState[IspDev]->au32WDRIntTime));
	
    return;
}

static HI_U32 cmos_get_sns_regs_info(ISP_DEV IspDev, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
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
		g_apstSnsState[IspDev]->astRegsInfo[0].unComBus.s8I2cDev = g_aunOv4689BusInfo[IspDev].s8I2cDev;
        g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;        
        g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum = 10;
        
        for (i=0; i<g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u8DevAddr = ov4689_i2c_addr;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32AddrByteNum = ov4689_addr_byte;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32DataByteNum = ov4689_data_byte;
        }        
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32RegAddr = OV4689_EXPO_H;    //long shutter
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32RegAddr = OV4689_EXPO_M;    //long shutter
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32RegAddr = OV4689_EXPO_L;    //long shutter
        
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32RegAddr = OV4689_AGAIN_REG; //Again
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32RegAddr = OV4689_AGAIN;     //Again

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32RegAddr = OV4689_FSIN_ENABLE; //FSIN_ENABLE
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x50;
        
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32RegAddr = OV4689_VMAX_L;    //VMAX
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32RegAddr = OV4689_VMAX_H;    //VMAX

        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32RegAddr = OV4689_R_RST_FSIN_L;    //VMAX
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32RegAddr = OV4689_R_RST_FSIN_H;    //VMAX
        
        /* Slave Sensor VsTime cfg */
        g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;
        g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u8DelayFrmNum = 1;
        
        g_apstSnsState[IspDev]->bSyncInit = HI_TRUE;
    }
    else
    {
        //Set Fps,Delay two frames enable fsin
        if(gbSetFpsFlag[IspDev])
        {
            gu8SetFpsCount[IspDev]++;
    
            if(gu8SetFpsCount[IspDev] == FSIN_DELAY_FRAMES)
            {
                //printf("Isp:[%d] count :%d \n",IspDev,gu8SetFpsCount[IspDev]);
                g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x50;
                gbSetFpsFlag[IspDev] = HI_FALSE;
            }
        }
        else
        {
            gu8SetFpsCount[IspDev] = 0;
        }
    
        // Slow Shutter,Delay two frames enable fsin (Slow Shutter and Set Fps can't use at the same time)
        if(gbSlowShutterFlag[IspDev])
        {
            gu8SlowShutterCount[IspDev]++;
    
            if(gu8SlowShutterCount[IspDev] == FSIN_DELAY_FRAMES)
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x50;
                gbSlowShutterFlag[IspDev] = HI_FALSE;
           }
        }
        else
        {
            gu8SlowShutterCount[IspDev] = 0;
        }

        //Set defect Pixel 5fps
        if(gbSetPixellowfpsFlag[IspDev])
        {
            gu8SetPixellowfpsCount[IspDev]++;
    
            if(gu8SetPixellowfpsCount[IspDev] == FSIN_DELAY_FRAMES)
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x50;
                gbSetPixellowfpsFlag[IspDev] = HI_FALSE;
           }
        }
        else
        {
            gu8SetPixellowfpsCount[IspDev] = 0;
        }

        //Set defect Pixel std fps
        if(gbSetPixelstdfpsFlag[IspDev])
        {
            gu8SetPixelstdfpsCount[IspDev]++;
    
            if(gu8SetPixelstdfpsCount[IspDev] == FSIN_DELAY_FRAMES)
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = 0x50;
                gbSetPixelstdfpsFlag[IspDev] = HI_FALSE;
           }
        }
        else
        {
            gu8SetPixelstdfpsCount[IspDev] = 0;
        }
        
        
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

        if (g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime == g_apstSnsState[IspDev]->astRegsInfo[1].stSlvSync.u32SlaveVsTime)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.bUpdate = HI_FALSE;
        }
        else
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;
        } 
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy(pstSnsRegsInfo, &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S)); 
    memcpy(&g_apstSnsState[IspDev]->astRegsInfo[1], &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S)); 
	g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];

    return 0;
}

static HI_S32 cmos_set_image_mode(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = g_apstSnsState[IspDev]->u8ImgMode;
    HI_U32 u32W, u32H;
    HI_FLOAT u32fps;
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    
    if (HI_NULL == pstSensorImageMode )
    {
        printf("null pointer when set image mode\n");
        return -1;
    }

    u32W = pstSensorImageMode->u16Width;
    u32H = pstSensorImageMode->u16Height;
    u32fps = pstSensorImageMode->f32Fps;
	
	if (OV4689_RES_IS_2K(u32W, u32H))
    {
        if(u32fps <= 30)
        {
           u8SensorImageMode = OV4689_MODE0;
        }
        else
        {
            printf("Not support! Width:%d, Height:%d, Fps:%f\n", u32W, u32H,u32fps);
			return -1;
        }
    }
    else if(OV4689_RES_IS_4M(u32W, u32H))
    {
        if(u32fps <= 30)
        {
           u8SensorImageMode = OV4689_MODE1;
        }
        else
        {
            printf("Not support! Width:%d, Height:%d, Fps:%f\n", u32W, u32H,u32fps);
            return -1;
        }
    }
    else
    {
        printf("Not support! Width:%d, Height:%d, Fps:%f\n", u32W, u32H,u32fps);
    }

    /* Sensor first init */
    if (HI_FALSE == g_apstSnsState[IspDev]->bInit)
    {
        g_apstSnsState[IspDev]->u8ImgMode = u8SensorImageMode;

        return 0;
    }

    /* Switch SensorImageMode */
    if (u8SensorImageMode == g_apstSnsState[IspDev]->u8ImgMode)
    {
        /* Don't need to switch SensorImageMode */
        return -1;
    }

    g_apstSnsState[IspDev]->u8ImgMode = u8SensorImageMode;

    return 0;
    
}

static HI_VOID sensor_global_init(ISP_DEV IspDev)
{   
    g_apstSnsState[IspDev]->bInit     = HI_FALSE; 
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    g_apstSnsState[IspDev]->u8ImgMode = OV4689_MODE0;
    g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
    g_apstSnsState[IspDev]->u32FLStd = g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines;
    g_apstSnsState[IspDev]->au32FL[0] = g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines;
    g_apstSnsState[IspDev]->au32FL[1] = g_astOv4689ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines;
    
    gu8SetFpsCount[IspDev] = 0;
    gbSetFpsFlag[IspDev] = HI_FALSE;

    gu8SlowShutterCount[IspDev] = 0;
    gbSlowShutterFlag[IspDev] = HI_FALSE;

    gu8SetPixellowfpsCount[IspDev] = 0;
    gbSetPixellowfpsFlag[IspDev] = HI_FALSE;

    gu8SetPixelstdfpsCount[IspDev] = 0;
    gbSetPixelstdfpsFlag[IspDev] = HI_FALSE;

    memset(&g_apstSnsState[IspDev]->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_apstSnsState[IspDev]->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));	
   
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = ov4689_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = ov4689_exit;
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
static int ov4689_set_bus_info(ISP_DEV IspDev, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOv4689BusInfo[IspDev].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return 0;
}

static int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, OV4689_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }
    
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, pstAeLib, OV4689_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, pstAwbLib, OV4689_ID, &stAwbRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }
    
    return 0;
}

static int sensor_unregister_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, OV4689_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }
    
    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, pstAeLib, OV4689_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, pstAwbLib, OV4689_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
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

ISP_SNS_OBJ_S stSnsOv4689SlaveObj = 
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = ov4689_standby,
    .pfnRestart             = ov4689_restart,
    .pfnWriteReg            = ov4689_write_register,
    .pfnReadReg             = ov4689_read_register,
    .pfnSetBusInfo          = ov4689_set_bus_info,
    .pfnSetInit             = sensor_set_init
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif 
