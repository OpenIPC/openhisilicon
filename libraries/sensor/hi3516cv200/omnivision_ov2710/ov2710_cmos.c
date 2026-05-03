#if !defined(__OV2710_CMOS_H_)
#define __OV2710_CMOS_H_

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


#define OV2710_ID 2710


/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

#define FULL_LINES_MAX  (0xFFF)

extern const unsigned int sensor_i2c_addr;
extern unsigned int sensor_addr_byte;
extern unsigned int sensor_data_byte;

#define SENSOR_1080P_30FPS_MODE (1)

#define VMAX_OV2710_1080P30_LINEAR     (1104)


HI_U8 gu8SensorImageMode = SENSOR_1080P_30FPS_MODE;
WDR_MODE_E genSensorMode = WDR_MODE_NONE;

static HI_U32 gu32FullLinesStd = VMAX_OV2710_1080P30_LINEAR;
static HI_U32 gu32FullLines = VMAX_OV2710_1080P30_LINEAR;

static HI_BOOL bInit = HI_FALSE;
HI_BOOL bSensorInit = HI_FALSE;
ISP_SNS_REGS_INFO_S g_stSnsRegsInfo = {0};
ISP_SNS_REGS_INFO_S g_stPreSnsRegsInfo = {0};

/* Piris attr */
static ISP_PIRIS_ATTR_S gstPirisAttr=
{
    0,      // bStepFNOTableChange
    1,      // bZeroIsMax
    93,     // u16TotalStep
    62,     // u16StepCount
    /* Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1 */
    {30,35,40,45,50,56,61,67,73,79,85,92,98,105,112,120,127,135,143,150,158,166,174,183,191,200,208,217,225,234,243,252,261,270,279,289,298,307,316,325,335,344,353,362,372,381,390,399,408,417,426,435,444,453,462,470,478,486,493,500,506,512},
    ISP_IRIS_F_NO_1_4, // enMaxIrisFNOTarget
    ISP_IRIS_F_NO_5_6  // enMinIrisFNOTarget
};



/* AE default parameter and function */
static HI_S32 cmos_get_ae_default(AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return -1;
    }

    pstAeSnsDft->u32LinesPer500ms = VMAX_OV2710_1080P30_LINEAR * 30 / 2;
    pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32FullLinesMax = FULL_LINES_MAX;

    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60;
    pstAeSnsDft->au8HistThresh[3] = 0x80;

    pstAeSnsDft->u8AeCompensation = 0x38;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 3;
    pstAeSnsDft->u32MinIntTime = 2;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535;
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;


    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;
    pstAeSnsDft->u32MaxAgain = 32768;
    pstAeSnsDft->u32MinAgain = 1024;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0625;
    pstAeSnsDft->u32MaxDgain = 31;
    pstAeSnsDft->u32MinDgain = 16;
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 256 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enIrisType = ISP_IRIS_DC_TYPE;
    memcpy(&pstAeSnsDft->stPirisAttr, &gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->u8AERunInterval = 1;

    return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if ((f32Fps <= 30) && (f32Fps >= 8.1))
    {
        if(SENSOR_1080P_30FPS_MODE == gu8SensorImageMode)
        {
            gu32FullLinesStd = VMAX_OV2710_1080P30_LINEAR * 30 / f32Fps;
            gu32FullLinesStd = (gu32FullLinesStd > FULL_LINES_MAX) ? FULL_LINES_MAX : gu32FullLinesStd;

            g_stSnsRegsInfo.astI2cData[8].u32Data = (gu32FullLinesStd >> 8) & 0xff;
            g_stSnsRegsInfo.astI2cData[9].u32Data = gu32FullLinesStd & 0xff;

            pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 3;

            pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
        }
    }
    else
    {
        printf("Not support Fps: %f\n", f32Fps);
        return;
    }

    return;
}

static HI_VOID cmos_slow_framerate_set(HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    u32FullLines = (u32FullLines > FULL_LINES_MAX) ? FULL_LINES_MAX : u32FullLines;
    gu32FullLines = u32FullLines;

    g_stSnsRegsInfo.astI2cData[8].u32Data = (gu32FullLines >> 8) & 0xff;
    g_stSnsRegsInfo.astI2cData[9].u32Data = gu32FullLines & 0xff;

    pstAeSnsDft->u32MaxIntTime = gu32FullLines - 3;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(HI_U32 u32IntTime)
{
    g_stSnsRegsInfo.astI2cData[0].u32Data = (u32IntTime >> 12) & 0x0F;
    g_stSnsRegsInfo.astI2cData[1].u32Data = (u32IntTime >> 4) & 0xFF;
    g_stSnsRegsInfo.astI2cData[2].u32Data = (u32IntTime << 4) & 0xFF;


    return;
}

static HI_U32 analog_gain_table[6] =
{
    1024, 2048, 4096, 8192, 16384, 32768
};


static HI_VOID cmos_again_calc_table(HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    if (*pu32AgainLin >= analog_gain_table[5])
    {
         *pu32AgainLin = analog_gain_table[5];
         *pu32AgainDb = 5;
         return ;
    }

    for (i = 1; i < 6; i++)
    {
        if (*pu32AgainLin < analog_gain_table[i])
        {
            *pu32AgainLin = analog_gain_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }

    return;

}

static HI_VOID cmos_gains_update(HI_U32 u32Again, HI_U32 u32Dgain)
{
    HI_U16 u16RegValue;

    u16RegValue = (((1 << u32Again) - 1) << 4) | ((u32Dgain - 16) & 0xf);

    g_stSnsRegsInfo.astI2cData[4].u32Data = (u16RegValue >> 8) & 0x1;
    g_stSnsRegsInfo.astI2cData[5].u32Data = u16RegValue & 0xff;


    return;
}

HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set= cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = NULL;

    return 0;
}


/* AWB default parameter and function */
static AWB_CCM_S g_stAwbCcm =
{
    4850,
    {   0x0201, 0x80a7, 0x805a,
        0x8040, 0x019d, 0x805d,
        0x8010, 0x8150, 0x0260
    },
    3160,
    {
        0x018e, 0x8018, 0x8076,
        0x8045, 0x0184, 0x803e,
        0x8035, 0x8134, 0x026a
    },
    2470,
    {
        0x019d, 0x8017, 0x8086,
        0x8031, 0x017e, 0x804c,
        0x8047, 0x819b, 0x02e2
    }
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /*1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768*/
    /* saturation */
    {0x80,0x80,0x7a,0x78,0x70,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38}

};

static HI_S32 cmos_get_awb_default(AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    if (HI_NULL == pstAwbSnsDft)
    {
        printf("null pointer when get awb default value!\n");
        return -1;
    }

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 5000;
    pstAwbSnsDft->au16GainOffset[0] = 0x154;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100;
    pstAwbSnsDft->au16GainOffset[3] = 0X1bd;
    pstAwbSnsDft->as32WbPara[0] = 156;
    pstAwbSnsDft->as32WbPara[1] = -71;
    pstAwbSnsDft->as32WbPara[2] = -170;
    pstAwbSnsDft->as32WbPara[3] = 198034;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -147889;

    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
    memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));

    return 0;
}

HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return 0;
}

#define DMNR_CALIB_CARVE_NUM_OV2710 9

float g_coef_calib_ov2710[DMNR_CALIB_CARVE_NUM_OV2710][4] =
{
    {100.000000f, 2.000000f, 0.036470f, 6.872425f, },
    {200.000000f, 2.301030f, 0.039808f, 6.327385f, },
    {400.000000f, 2.602060f, 0.041653f, 6.645809f, },
    {800.000000f, 2.903090f, 0.043177f, 7.786633f, },
    {1600.000000f, 3.204120f, 0.046225f, 8.775841f, },
    {3200.000000f, 3.505150f, 0.058395f, 9.478638f, },
    {6200.000000f, 3.792392f, 0.062279f, 16.687279f, },
    {12400.000000f, 4.093422f, 0.073368f, 27.761475f, },
    {24800.000000f, 4.394452f, 0.088682f, 46.984421f, },

};



static ISP_NR_ISO_PARA_TABLE_S g_stNrIsoParaTab[HI_ISP_NR_ISO_LEVEL_MAX] =
{
     //u16Threshold//u8varStrength//u8fixStrength//u8LowFreqSlope
       {1500,       160,             256-256,            0 },  //100    //                      //
       {1500,       120,             256-256,            0 },  //200    // ISO                  // ISO //u8LowFreqSlope
       {1500,       100,             256-256,            0 },  //400    //{400,  1200, 96,256}, //{400 , 0  }
       {1750,       80,              256-256,            8 },  //800    //{800,  1400, 80,256}, //{600 , 2  }
       {1500,       255,             256-256,            6 },  //1600   //{1600, 1200, 72,256}, //{800 , 8  }
       {1500,       255,             256-256,            0 },  //3200   //{3200, 1200, 64,256}, //{1000, 12 }
       {1375,       255,             256-256,            0 },  //6400   //{6400, 1100, 56,256}, //{1600, 6  }
       {1375,       255,             256-256,            0 },  //12800  //{12000,1100, 48,256}, //{2400, 0  }
       {1375,       255,             256-256,            0 },  //25600  //{36000,1100, 48,256}, //
       {1375,       255,             256-256,            0 },  //51200  //{64000,1100, 96,256}, //
       {1250,       255,             256-256,            0 },  //102400 //{82000,1000,240,256}, //
       {1250,       255,             256-256,            0 },  //204800 //                           //
       {1250,       255,             256-256,            0 },  //409600 //                           //
       {1250,       255,             256-256,            0 },  //819200 //                           //
       {1250,       255,             256-256,            0 },  //1638400//                           //
       {1250,       255,             256-256,            0 },  //3276800//                           //
};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
	/*For Demosaic*/
	1, /*bEnable*/
	24,/*u16VhLimit*/
	40-24,/*u16VhOffset*/
	24,   /*u16VhSlope*/
	/*False Color*/
	1,    /*bFcrEnable*/
	{ 8, 8, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 0, 0},    /*au8FcrStrength[ISP_AUTO_ISO_STENGTH_NUM]*/
	{24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24},    /*au8FcrThreshold[ISP_AUTO_ISO_STENGTH_NUM]*/
	/*For Ahd*/
	400, /*u16UuSlope*/
	{512,512,512,512,512,512,512,  400,  0,0,0,0,0,0,0,0}    /*au16NpOffset[ISP_AUTO_ISO_STENGTH_NUM]*/
};

static ISP_CMOS_GE_S g_stIspGe =
{
	/*For GE*/
	1,    /*bEnable*/
	7,    /*u8Slope*/
	7,    /*u8Sensitivity*/
	8192, /*u16Threshold*/
	8192, /*u16SensiThreshold*/
	{1024,1024,1024,2048,2048,2048,2048,  2048,  2048,2048,2048,2048,2048,2048,2048,2048}    /*au16Strength[ISP_AUTO_ISO_STENGTH_NUM]*/
};

static ISP_CMOS_RGBSHARPEN_S g_stIspRgbSharpen =
{
  //{100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
    {0,	  0,   0,   0,    0,   1,   1,   1,    1,    1,    1,     1,     1,     1,     1,       1},/* enPixSel = ~bEnLowLumaShoot */

    {120,  64,  64,  43,  43,  43,  18,  18,    18,  200,  250,   250,   250,   250,    250,    250},/*maxSharpAmt1 = SharpenUD*16 */
    {128, 200, 103, 86,  86,  86,  80,  80,    80,  250,  250,   250,   250,   250,    250,    250},/*maxEdgeAmt = SharpenD*16 */

    {0,   0,   0,    0,   0,   0,   0,   40,  190,  200,  220,   250,   250,   250,     250,       250},/*sharpThd2 = TextureNoiseThd*4 */
    {0,   0,   0,    0,   0,   0,   0,   60,  140,    0,    0,     0,    0,     0,     0,       0},/*edgeThd2 = EdgeNoiseThd*4 */
    {59,  59,  59,  59,  59,  59,  59,   59,  101,  0,   0,    0,   0,    0,    0,      0},/*overshootAmt*/
    {117, 117, 117, 108, 108, 108, 122,  122, 139,  0,   0,    0,   0,    0,    0,     0},/*undershootAmt*/
};

static ISP_CMOS_UVNR_S g_stIspUVNR =
{
  //{100,	200,	400,	800,	1600,	3200,	6400,	12800,	25600,	51200,	102400,	204800,	409600,	819200,	1638400,	3276800};
	{1,	    2,       4,      5,      7,      48,     32,     16,     16,     16,      16,     16,     16,     16,     16,        16},      /*UVNRThreshold*/
 	{0,		0,		0,		0,		0,		0,		0,		0,		0,		1,			1,		2,		2,		2,		2,		2},  /*Coring_lutLimit*/
	{0,		0,		0,		16,		34,		34,		34,		34,		34,		34,		34,		34,		34,		34,		34,			34}  /*UVNR_blendRatio*/
};

static ISP_CMOS_DPC_S g_stCmosDpc =
{
	//1,/*IR_channel*/
	//1,/*IR_position*/
	{0,0,0,1,1,1,2,2,2,3,3,3,3,3,3,3},/*au16Strength[16]*/
	{0,0,0,0,0,0,0,0,0x24,0x80,0x80,0x80,0xE5,0xE5,0xE5,0xE5},/*au16BlendRatio[16]*/
};

static ISP_CMOS_DRC_S g_stIspDrc =
{
    0,
    10,
    0,
    2,
    192,
    60,
    0,
    0,
    0,
    {1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024}
};


HI_U32 cmos_get_isp_default(ISP_CMOS_DEFAULT_S *pstDef)
{
    if (HI_NULL == pstDef)
    {
        printf("null pointer when get isp default value!\n");
        return -1;
    }

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
    #if 0
    pstDef->stDrc.bEnable               = HI_FALSE;
    pstDef->stDrc.u8Asymmetry           = 0x02;
    pstDef->stDrc.u8SecondPole          = 0xC0;
    pstDef->stDrc.u8Stretch             = 0x3C;
    pstDef->stDrc.u8LocalMixingBrigtht  = 0x2D;
    pstDef->stDrc.u8LocalMixingDark     = 0x2D;
    pstDef->stDrc.u8LocalMixingThres    = 0x02;
    pstDef->stDrc.u16BrightGainLmt      = 0x7F;
    pstDef->stDrc.u16DarkGainLmtC       = 0x7F;
    pstDef->stDrc.u16DarkGainLmtY       = 0x7F;
    pstDef->stDrc.u8RangeVar            = 0x00;
    pstDef->stDrc.u8SpatialVar          = 0x0A;
    #endif
    memcpy(&pstDef->stDrc, &g_stIspDrc, sizeof(ISP_CMOS_DRC_S));
    memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
    memcpy(&pstDef->stRgbSharpen, &g_stIspRgbSharpen, sizeof(ISP_CMOS_RGBSHARPEN_S));
    memcpy(&pstDef->stGe, &g_stIspGe, sizeof(ISP_CMOS_GE_S));
    pstDef->stNoiseTbl.stNrCaliPara.u8CalicoefRow = DMNR_CALIB_CARVE_NUM_OV2710;
    pstDef->stNoiseTbl.stNrCaliPara.pCalibcoef    = (HI_FLOAT (*)[4])g_coef_calib_ov2710;

    memcpy(&pstDef->stNoiseTbl.stIsoParaTable[0], &g_stNrIsoParaTab[0],sizeof(ISP_NR_ISO_PARA_TABLE_S)*HI_ISP_NR_ISO_LEVEL_MAX);

    memcpy(&pstDef->stUvnr,       &g_stIspUVNR,       sizeof(ISP_CMOS_UVNR_S));
    memcpy(&pstDef->stDpc,       &g_stCmosDpc,       sizeof(ISP_CMOS_DPC_S));

    pstDef->stSensorMaxResolution.u32MaxWidth  = 1920;
    pstDef->stSensorMaxResolution.u32MaxHeight = 1080;

    return 0;
}

HI_U32 cmos_get_isp_black_level(ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    if (HI_NULL == pstBlackLevel)
    {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    pstBlackLevel->au16BlackLevel[0] = 64;
    pstBlackLevel->au16BlackLevel[1] = 64;
    pstBlackLevel->au16BlackLevel[2] = 64;
    pstBlackLevel->au16BlackLevel[3] = 64;


    return 0;

}

HI_VOID cmos_set_pixel_detect(HI_BOOL bEnable)
{
    if (bEnable) /* setup for ISP pixel calibration mode */
    {
        //set frame rate
        sensor_write_register(0x380E, 0x19);
        sensor_write_register(0x380F, 0xE0);

        //set exposure time
        sensor_write_register(0x3500, 0x01);
        sensor_write_register(0x3501, 0x9D);
        sensor_write_register(0x3502, 0x00);

        //set gain
        sensor_write_register(0x350A, 0x00);
        sensor_write_register(0x350B, 0x00);

    }
    else /* setup for ISP 'normal mode' */
    {
        sensor_write_register(0x380E, 0x04);
        sensor_write_register(0x380F, 0x50);

        sensor_write_register(0x3500, 0x00);
        sensor_write_register(0x3501, 0x44);
        sensor_write_register(0x3502, 0x00);

        bInit = HI_FALSE;
    }

    return;
}

HI_VOID cmos_set_wdr_mode(HI_U8 u8Mode)
{
    bInit = HI_FALSE;

    switch(u8Mode)
    {
        case WDR_MODE_NONE:
            if (SENSOR_1080P_30FPS_MODE == gu8SensorImageMode)
            {
                gu32FullLinesStd = VMAX_OV2710_1080P30_LINEAR;
            }
            genSensorMode = WDR_MODE_NONE;
            printf("linear mode\n");
        break;
        default:
            printf("NOT support this mode!\n");
            return;
        break;
    }

    return;
}

HI_U32 cmos_get_sns_regs_info(ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;

    if (HI_FALSE == bInit)
    {
        g_stSnsRegsInfo.enSnsType = ISP_SNS_I2C_TYPE;
        g_stSnsRegsInfo.u8Cfg2ValidDelayMax = 2;
        g_stSnsRegsInfo.u32RegNum = 10;

        for (i = 0; i < g_stSnsRegsInfo.u32RegNum; i++)
        {
            g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
            g_stSnsRegsInfo.astI2cData[i].u8DevAddr = sensor_i2c_addr;
            g_stSnsRegsInfo.astI2cData[i].u32AddrByteNum = sensor_addr_byte;
            g_stSnsRegsInfo.astI2cData[i].u32DataByteNum = sensor_data_byte;
        }

        g_stSnsRegsInfo.astI2cData[0].u8DelayFrmNum = 0;         //exposure time
        g_stSnsRegsInfo.astI2cData[0].u32RegAddr = 0x3500;
        g_stSnsRegsInfo.astI2cData[1].u8DelayFrmNum = 0;         //exposure time
        g_stSnsRegsInfo.astI2cData[1].u32RegAddr = 0x3501;
        g_stSnsRegsInfo.astI2cData[2].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[2].u32RegAddr = 0x3502;
        g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[3].u32RegAddr = 0x3212;
        g_stSnsRegsInfo.astI2cData[3].u32Data = 0x00;
        g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[4].u32RegAddr = 0x350a;      //gain
        g_stSnsRegsInfo.astI2cData[5].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[5].u32RegAddr = 0x350b;
        g_stSnsRegsInfo.astI2cData[6].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[6].u32RegAddr = 0x3212;
        g_stSnsRegsInfo.astI2cData[6].u32Data = 0x10;
        g_stSnsRegsInfo.astI2cData[7].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[7].u32RegAddr = 0x3212;
        g_stSnsRegsInfo.astI2cData[7].u32Data = 0xA0;

        g_stSnsRegsInfo.astI2cData[8].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[8].u32RegAddr = 0x380e;
        g_stSnsRegsInfo.astI2cData[9].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[9].u32RegAddr = 0x380f;

        bInit = HI_TRUE;
    }
    else
    {
        for (i = 0; i < g_stSnsRegsInfo.u32RegNum; i++)
        {
            if (g_stSnsRegsInfo.astI2cData[i].u32Data == g_stPreSnsRegsInfo.astI2cData[i].u32Data)
            {
                g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_FALSE;
            }
            else
            {
                g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
            }
        }

        g_stSnsRegsInfo.astI2cData[3].bUpdate = g_stSnsRegsInfo.astI2cData[4].bUpdate
                                                    | g_stSnsRegsInfo.astI2cData[5].bUpdate;
        g_stSnsRegsInfo.astI2cData[6].bUpdate = g_stSnsRegsInfo.astI2cData[3].bUpdate;
        g_stSnsRegsInfo.astI2cData[7].bUpdate = g_stSnsRegsInfo.astI2cData[3].bUpdate;
    }

    if (HI_NULL == pstSnsRegsInfo)
    {
        printf("null pointer when get sns reg info!\n");
        return -1;
    }

    memcpy(pstSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&g_stPreSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));

    return 0;
}

static HI_S32 cmos_set_image_mode(ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = gu8SensorImageMode;

    bInit = HI_FALSE;

    if (HI_NULL == pstSensorImageMode )
    {
        printf("null pointer when set image mode\n");
        return -1;
    }

    if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080))
    {
        if (WDR_MODE_NONE == genSensorMode)
        {
            if (pstSensorImageMode->f32Fps <= 30)
            {
                u8SensorImageMode = SENSOR_1080P_30FPS_MODE;
            }
            else
            {
                printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                    pstSensorImageMode->u16Width,
                    pstSensorImageMode->u16Height,
                    pstSensorImageMode->f32Fps,
                    genSensorMode);

                return -1;
            }
        }
        else
        {
            printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                pstSensorImageMode->u16Width,
                pstSensorImageMode->u16Height,
                pstSensorImageMode->f32Fps,
                genSensorMode);

            return -1;
        }
    }
    else
    {
        printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
            pstSensorImageMode->u16Width,
            pstSensorImageMode->u16Height,
            pstSensorImageMode->f32Fps,
            genSensorMode);

        return -1;
    }

    /* Sensor first init */
    if (HI_FALSE == bSensorInit)
    {
        gu8SensorImageMode = u8SensorImageMode;

        return 0;
    }

    /* Switch SensorImageMode */
    if (u8SensorImageMode == gu8SensorImageMode)
    {
        /* Don't need to switch SensorImageMode */
        return -1;
    }

    gu8SensorImageMode = u8SensorImageMode;

    return 0;
}

HI_VOID sensor_global_init()
{
    gu8SensorImageMode = SENSOR_1080P_30FPS_MODE;
    genSensorMode = WDR_MODE_NONE;
    gu32FullLinesStd = VMAX_OV2710_1080P30_LINEAR;
    gu32FullLines = VMAX_OV2710_1080P30_LINEAR;
    bInit = HI_FALSE;
    bSensorInit = HI_FALSE;

    memset(&g_stSnsRegsInfo, 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_stPreSnsRegsInfo, 0, sizeof(ISP_SNS_REGS_INFO_S));
}

HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = sensor_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = sensor_exit;
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

int sensor_register_callback(void)
{
    ISP_DEV IspDev = 0;
    HI_S32 s32Ret;
    ALG_LIB_S stLib;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, OV2710_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, OV2710_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, OV2710_ID, &stAwbRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    return 0;
}

int sensor_unregister_callback(void)
{
    ISP_DEV IspDev = 0;
    HI_S32 s32Ret;
    ALG_LIB_S stLib;

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, OV2710_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, &stLib, OV2710_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, &stLib, OV2710_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SOIOV2710_CMOS_H_ */
