#if !defined(__JXF22_CMOS_H_)
#define __JXF22_CMOS_H_

/*
 * JXF22 sensor driver — single source for hi3516cv200 (V2) and
 * hi3516cv300 (V3). The two SDK generations share the T2 MPI
 * registration ABI but ship dramatically different ISP_CMOS_*_S struct
 * layouts and use opposite byte orders in g_stSnsRegsInfo.astI2cData[].
 *
 * Build dispatch is driven by -D<chiparch> in each platform's Makefile;
 * see docs/sensor-driver-api.md §3.2 for the cv200/cv300 ISP-feature
 * ABI drift. Each top-level body is wrapped in a single
 * #if defined(hi3516cv300) / #else / #endif wall: cv300 builds compile
 * the V3 implementation, cv200 builds compile the V2 implementation.
 * Symbol exports (sensor_register_callback, cmos_init_*_exp_function,
 * etc.) match on both sides.
 */

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

#define JXF22_ID 22

#if defined(hi3516cv300)
/* ============================================================== *
 * hi3516cv300 (V3) implementation                                 *
 * ============================================================== */

/****************************************************************************
 * local variables                                                          *
 ****************************************************************************/

static HI_U16 g_au16InitWBGain[ISP_MAX_DEV_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_DEV_NUM] = {0};

extern const unsigned int sensor_i2c_addr;
extern unsigned int sensor_addr_byte;
extern unsigned int sensor_data_byte;

#define FULL_LINES_MAX  (0xFFFF)

#define EXPOSURE_ADDR_H        (0x02)
#define EXPOSURE_ADDR_L        (0x01)
#define AGC_ADDR                (0x00)
#define VMAX_ADDR_L             (0x22)
#define VMAX_ADDR_H             (0x23)

#define INCREASE_LINES (0) /* make real fps less than stand fps because NVR require*/
#define VMAX_1080P30_LINEAR  (1134+INCREASE_LINES)

#define SENSOR_1080P_30FPS_MODE (1)

HI_U8 gu8SensorImageMode = SENSOR_1080P_30FPS_MODE;
WDR_MODE_E genSensorMode = WDR_MODE_NONE;

static HI_U32 gu32FullLinesStd = VMAX_1080P30_LINEAR;
static HI_U32 gu32FullLines = VMAX_1080P30_LINEAR;
static HI_U32 gu32PreFullLines = VMAX_1080P30_LINEAR;
static HI_BOOL bInit = HI_FALSE;
HI_BOOL bSensorInit = HI_FALSE;
ISP_SNS_REGS_INFO_S g_stSnsRegsInfo = {0};
ISP_SNS_REGS_INFO_S g_stPreSnsRegsInfo = {0};

static HI_S32 cmos_get_ae_default(AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return -1;
    }

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32LinesPer500ms = gu32FullLinesStd*30/2;
    pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32FullLinesMax = FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0625;

	pstAeSnsDft->u32MaxDgain = 16;
	pstAeSnsDft->u32MinDgain = 16;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    pstAeSnsDft->u32InitExposure = 921600;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    switch(genSensorMode)
    {
        default:
        case WDR_MODE_NONE:   /*linear mode*/
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;
            pstAeSnsDft->u8AeCompensation = 0x32;
			gu32FullLinesStd = VMAX_1080P30_LINEAR ;
		    pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 4;
		    pstAeSnsDft->u32MinIntTime = 2;
		    pstAeSnsDft->u32MaxIntTimeTarget = 65535;
		    pstAeSnsDft->u32MinIntTimeTarget = 2;
		    pstAeSnsDft->u32MaxAgain = 15872;  // 15.5
		    pstAeSnsDft->u32MinAgain = 1024;
		    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
		    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
		    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
		    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
			pstAeSnsDft->u32LinesPer500ms = gu32FullLinesStd*30/2;
	    break;
    }
    return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (SENSOR_1080P_30FPS_MODE == gu8SensorImageMode)
    {
        if ((f32Fps <= 30) && (f32Fps > 0.50))
        {
            gu32FullLinesStd = (VMAX_1080P30_LINEAR * 30) / f32Fps;
        }
        else
        {
            printf("Not support Fps: %f\n", f32Fps);
            return;
        }
    }
    else
    {
        printf("Not support! gu8SensorImageMode:%d, f32Fps:%f\n", gu8SensorImageMode, f32Fps);
        return;
    }

    gu32FullLinesStd = (gu32FullLinesStd > FULL_LINES_MAX) ? FULL_LINES_MAX : gu32FullLinesStd;
    if(WDR_MODE_NONE == genSensorMode)
    {
	    g_stSnsRegsInfo.astI2cData[3].u32Data = (gu32FullLinesStd & 0xFF);     // 0X22
	    g_stSnsRegsInfo.astI2cData[4].u32Data = ((gu32FullLinesStd & 0xFF00) >> 8);   // 0X23
    }
    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = gu32FullLinesStd * f32Fps / 2;
    pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 4;
    pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
    gu32FullLines = gu32FullLinesStd;
    pstAeSnsDft->u32FullLines = gu32FullLines;
    return;
}

static HI_VOID cmos_slow_framerate_set(HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    u32FullLines = (u32FullLines > FULL_LINES_MAX) ? FULL_LINES_MAX : u32FullLines;
	gu32FullLines = u32FullLines;
	pstAeSnsDft->u32FullLines = gu32FullLines;
    if(WDR_MODE_NONE == genSensorMode)
    {
        g_stSnsRegsInfo.astI2cData[3].u32Data = (u32FullLines & 0xFF);
        g_stSnsRegsInfo.astI2cData[4].u32Data = ((u32FullLines & 0xFF00) >> 8);
    }
    else
    {
        pstAeSnsDft->u32FullLines = gu32FullLinesStd;
        return;
    }
    pstAeSnsDft->u32MaxIntTime = gu32FullLines - 4;
    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(HI_U32 u32IntTime)
{
    g_stSnsRegsInfo.astI2cData[0].u32Data = (u32IntTime & 0xFF);
    g_stSnsRegsInfo.astI2cData[1].u32Data = ((u32IntTime & 0xFF00) >> 8);

    return;
}

static HI_VOID cmos_again_calc_table(HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
	int again;
	int tmp=0;
	again = *pu32AgainLin >> 6;  //10bit -> 4bit ,1 = 16;
	while(again>31)
	{
		again = again/2;
		tmp++;
	}
	if(again > 15)
	{
		again = again-16;
	}

	*pu32AgainDb = (tmp<<4)|again;

	return;
}

static HI_VOID cmos_gains_update(HI_U32 u32Again, HI_U32 u32Dgain)
{
	g_stSnsRegsInfo.astI2cData[2].u32Data = u32Again & 0xFF;

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
    return 0;
}

static AWB_CCM_S g_stAwbCcm =
{
    4950,
    {
        0x0270,0x814F,0x8021,
        0x8047,0x01D2,0x808B,
        0x801A,0x80D8,0x01F2
    },

    3650,
    {
        0x1C3,0x80A7,0x801C,
        0x807F,0x1AD,0x802E,
        0x8048,0x80E7,0x22F
    },

    2700,
    {
        0x022B,0x80D1,0x805A,
        0x8053,0x01B2,0x805F,
        0x8065,0x81FE,0x0363
    }
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /*1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768*/
    /* saturation */
    {118, 115, 110, 100, 90, 85, 75, 65, 65, 65, 70, 64, 64, 56, 56, 56}
};

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static HI_S32 cmos_get_awb_default(AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
	HI_U8 i;
    ISP_DEV IspDev = 0;

    if (HI_NULL == pstAwbSnsDft)
    {
        printf("null pointer when get awb default value!\n");
        return -1;
    }
    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4950;
	pstAwbSnsDft->au16GainOffset[0] = 0x19C;
	pstAwbSnsDft->au16GainOffset[1] = 0x100;
	pstAwbSnsDft->au16GainOffset[2] = 0x100;
	pstAwbSnsDft->au16GainOffset[3] = 0x1CB;
	pstAwbSnsDft->as32WbPara[0] = 49;
	pstAwbSnsDft->as32WbPara[1] = 207;
	pstAwbSnsDft->as32WbPara[2] = 0;
	pstAwbSnsDft->as32WbPara[3] = 160025;
	pstAwbSnsDft->as32WbPara[4] = 128;
	pstAwbSnsDft->as32WbPara[5] = -107016;

	for (i=0; i<4; i++)
    {
        pstAwbSnsDft->au32BlcOffset[i] = 0x0;
    }

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
    memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));

    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[IspDev];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[IspDev];
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[IspDev][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[IspDev][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[IspDev][2];

    pstAwbSnsDft->u8AWBRunInterval = 4;

    return 0;
}

HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return 0;
}

/* ISP default parameter and function */
static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
    /*bEnable*/
	1,
    /*au16EdgeSmoothThr*/
	{8,16,16,16,16,16,128,1022,1022,1022,1022,1022,1022,1022,1022,1022},
	/*au16EdgeSmoothSlope*/
	{16,16,16,16,16,32,32,0,0,0,0,0,0,0,0,0},
	/*au16AntiAliasThr*/
	{53,53,53,86,112,112,1022,1022,1022,1022,1022,1022,1022,1022,1022,1022},
	/*au16AntiAliasSlope*/
	{256,256,256,256,256,256,0,0,0,0,0,0,0,0,0,0},
    /*NrCoarseStr*/
    {128, 128, 128, 112, 96, 48, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	/*NoiseSuppressStr*/
	{4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 36},
	/*DetailEnhanceStr*/
	{4, 4, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /*SharpenLumaStr*/
    {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256},
    /*ColorNoiseCtrlThr*/
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

static ISP_CMOS_GE_S g_stIspGe =
{
	/*For GE*/
	1,    /*bEnable*/
	9,    /*u8Slope*/
	9,    /*u8SensiSlope*/
	300, /*u16SensiThr*/
	{300,300,300,300,310,310,310,  310,  320,320,320,320,330,330,330,330}, /*au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM]*/
	{ 128, 128, 128, 128, 129, 129, 129,   129,   130, 130, 130, 130, 131, 131, 131, 131}, /*au16Strength[ISP_AUTO_ISO_STRENGTH_NUM]*/
	{1024,1024,1024,2048,2048,2048,2048,  2048,  2048,2048,2048,2048,2048,2048,2048,2048}    /*au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM]*/
};

static ISP_CMOS_FCR_S g_stIspFcr =
{
    /*For FCR*/
    /*bEnable*/
	1,
	/*au8Strength[ISP_AUTO_ISO_STRENGTH_NUM]*/
	{4, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0},
    /*au8Threhold[ISP_AUTO_ISO_STRENGTH_NUM]*/
	{24, 24, 24, 24, 20, 20, 20, 16, 14, 12, 10, 8, 6, 4, 2, 0},
    /*au16Offset[ISP_AUTO_ISO_STRENGTH_NUM]*/
	{150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}
};

static ISP_CMOS_YUV_SHARPEN_S g_stIspYuvSharpen =
{
    /* bvalid */
    1,
    /* au16SharpenUd */
    {23, 20, 18, 16, 14, 7, 4, 2, 2, 6, 3, 3, 3, 2, 1, 1},
    /* au8SharpenD */
    {80, 70, 66, 60, 55, 42, 32, 20, 15, 15, 35, 30, 30, 30, 30, 30},
    /*au8TextureThd*/
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* au8SharpenEdge */
    {75, 75, 75, 75, 75, 75, 65, 60, 50, 45, 60, 60, 60, 60, 60, 60},
    /* au8EdgeThd */
    {80, 80, 80, 90, 100, 100, 100, 100, 100, 110, 120, 130, 150, 170, 180, 190},
    /* au8OverShoot */
    {170, 140, 100, 75, 65, 50, 26, 16, 10, 10, 20, 10, 10, 10, 10, 10},
    /* au8UnderShoot */
    {190, 150, 100, 75, 65, 65, 50, 40, 30, 20, 20, 10, 10, 10, 10, 10},
    /*au8shootSupSt*/
    {40, 30, 30, 30, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* au8DetailCtrl */
    {128, 128, 128, 128, 118, 100, 80, 70, 65, 55, 55, 55, 55, 55, 55, 55},
    /* au8RGain */
    {16,   16,   16,   16,   24,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31},
	/* au8BGain */
    {20,   20,   20,   20,   26,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,   31},
	/* au8SkinGain */
    {255, 255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255},
	/*  au8EdgeFiltStr*/
	{0,	  0,	   0,	   0,	   0,		0,     0,		0,		0,		0,		0,			0,			0,		0,		0,		0},
	/*  au8JagCtrl*/
	{252 ,   252 ,    252,     252,    252,	       252,      252,      252,	   252,	     252,	   252,	  252,	  252,	  252,	  252,    252},
	/*  au8NoiseLumaCtrl*/
	{0,	  0,	   0,	   0,	   0,		0,     0,		0,		0,		0,		0,			0,			0,		0,		0,		0},
};

static ISP_CMOS_DRC_S g_stIspDRC =
{
    /*bEnable*/
    0,
    /*u8SpatialVar*/
    3,
	/*u8RangeVar*/
	15,
	/*u8Asymmetry8*/
    2,
	/*u8SecondPole; */
    180,
	/*u8Stretch*/
    54,
	/*u8Compress*/
    180,
	/*u8PDStrength*/
    35,
	/*u8LocalMixingBrigtht*/
    20,
    /*u8LocalMixingDark*/
    20,
    /*ColorCorrectionLut[33];*/
    {1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024},
    /*bOpType 0:auto  1:manual*/
    0,
    /*u8ManualStrength*/
    128,
    /*u8AutoStrength*/
    128,
};

static ISP_CMOS_GAMMA_S g_stIspGamma =
{
    /* bvalid */
    1,
    {
        0,93,189,285,380,473,563,649,712,772,829,885,940,995,1051,1108,1167,1201,1236,1272,
		1308,1345,1381,1418,1455,1492,1528,1564,1599,1634,1667,1699,1731,1761,1790,1818,
		1846,1874,1900,1926,1952,1977,2001,2025,2049,2072,2095,2117,2140,2161,2182,2202,
		2222,2241,2260,2279,2297,2314,2332,2349,2366,2383,2400,2417,2434,2450,2467,2483,
		2500,2516,2532,2547,2563,2578,2593,2608,2623,2638,2652,2666,2681,2694,2708,2721,
		2734,2747,2759,2772,2784,2796,2809,2821,2833,2845,2857,2869,2882,2894,2906,2919,
		2932,2944,2957,2970,2982,2995,3007,3019,3031,3043,3055,3066,3078,3088,3099,3110,
		3120,3131,3141,3151,3161,3170,3180,3189,3199,3208,3218,3227,3237,3246,3255,3264,
		3273,3282,3291,3300,3308,3317,3326,3334,3343,3351,3360,3368,3377,3385,3393,3402,
		3410,3418,3427,3435,3443,3451,3460,3468,3476,3483,3491,3499,3507,3514,3521,3529,
		3536,3543,3550,3557,3564,3571,3578,3585,3592,3598,3605,3612,3619,3625,3632,3638,
		3645,3651,3658,3664,3671,3677,3683,3690,3696,3702,3709,3715,3722,3728,3734,3741,
		3747,3753,3760,3766,3773,3779,3785,3792,3798,3804,3811,3817,3824,3830,3836,3843,
		3850,3856,3863,3870,3876,3883,3889,3896,3902,3908,3915,3921,3927,3932,3938,3943,
		3949,3954,3959,3965,3970,3975,3980,3985,3990,3995,4000,4005,4011,4016,4021,4026,
		4032,4037,4042,4047,4053,4058,4063,4068,4074,4079,4084,4089,4095,
	}
};

static ISP_CMOS_CA_S g_stIspCA =
{
    /*enable*/
	1,
	/*YRatioLut*/
    {
        36 ,81  ,111 ,136 ,158 ,182 ,207 ,228 ,259 ,290 ,317 ,345 ,369 ,396 ,420 ,444 ,
    	468 ,492 ,515 ,534 ,556 ,574 ,597 ,614 ,632 ,648 ,666 ,681 ,697 ,709 ,723 ,734 ,
    	748 ,758 ,771 ,780 ,788 ,800 ,808 ,815 ,822 ,829 ,837 ,841 ,848 ,854 ,858 ,864 ,
    	868 ,871 ,878 ,881 ,885 ,890 ,893 ,897 ,900 ,903 ,906 ,909 ,912 ,915 ,918 ,921 ,
    	924 ,926 ,929 ,931 ,934 ,936 ,938 ,941 ,943 ,945 ,947 ,949 ,951 ,952 ,954 ,956 ,
    	958 ,961 ,962 ,964 ,966 ,968 ,969 ,970 ,971 ,973 ,974 ,976 ,977 ,979 ,980 ,981 ,
    	983 ,984 ,985 ,986 ,988 ,989 ,990 ,991 ,992 ,993 ,995 ,996 ,997 ,998 ,999 ,1000,
    	1001,1004,1005,1006,1007,1009,1010,1011,1012,1014,1016,1017,1019,1020,1022,1024
    },
	/*ISORatio*/
    {1300,1300,1250,1200,1150,1100,1050,1000,1000,1000,900,900,800,800,800,800}
};

static ISP_CMOS_DPC_S g_stCmosDpc =
{

	{0, 0, 200, 200 ,200, 200, 220, 220, 220, 220, 152, 152, 152, 152, 152, 152},/*au16Strength[16]*/
	{0, 0, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50},/*au16BlendRatio[16]*/
};

static ISP_LSC_CABLI_UNI_TABLE_S g_stCmosLscUniTable =
{
   /*Mesh Grid Info: Width*/
   {60, 60, 60, 60, 60, 60, 60, 60},
   /*Mesh Grid Info: Height*/
   {60, 60, 60, 60, 60, 60, 60, 60}
};

static ISP_LSC_CABLI_TABLE_S g_stCmosLscTable[HI_ISP_LSC_LIGHT_NUM] =
{
    /* Light0: 4950K (D50, indoor) */
    {
        /* astLscParaTableR */
        {
        {296,287,283,267,265,261,251,242,238},
        {283,271,261,254,238,235,231,226,221},
        {267,254,247,240,235,228,221,215,212},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200}
        },
        /* astLscParaTableGr */
        {
        {221,217,210,201,196,190,183,175,170},
        {209,201,193,187,181,176,169,162,156},
        {198,193,186,180,174,168,162,154,147},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140}
        },
        /* astLscParaTableGb */
        {
        {221,217,210,201,196,190,183,175,170},
        {209,201,193,187,181,176,169,162,156},
        {198,193,186,180,174,168,162,154,147},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140}
        },
        /* astLscParaTableB */
        {
        {179,177,170,165,160,156,150,145,141},
        {171,165,158,154,150,147,142,137,131},
        {163,157,153,149,144,140,134,128,123},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116}
        }
    },
    /* Light1: 2700K (incandescent) */
    {
        /* astLscParaTableR */
        {
        {296,287,283,267,265,261,251,242,238},
        {283,271,261,254,238,235,231,226,221},
        {267,254,247,240,235,228,221,215,212},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200},
        {255,247,234,230,225,219,212,205,200}
        },
        /* astLscParaTableGr */
        {
        {221,217,210,201,196,190,183,175,170},
        {209,201,193,187,181,176,169,162,156},
        {198,193,186,180,174,168,162,154,147},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140}
        },
        /* astLscParaTableGb */
        {
        {221,217,210,201,196,190,183,175,170},
        {209,201,193,187,181,176,169,162,156},
        {198,193,186,180,174,168,162,154,147},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140},
        {193,186,179,172,168,161,155,147,140}
        },
        /* astLscParaTableB */
        {
        {179,177,170,165,160,156,150,145,141},
        {171,165,158,154,150,147,142,137,131},
        {163,157,153,149,144,140,134,128,123},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116},
        {158,152,148,143,139,134,127,122,116}
        }
    },
};

/***BAYER NR**/
static ISP_CMOS_BAYERNR_S g_stIspBayerNr =
{
    /*Calibration Lut Num*/
	7,
	/*************Calibration LUT Table*************/
	{
	    {100.000000f, 0.047971f},
        {200.000000f, 0.082093f},
        {400.000000f, 0.144531f},
        {800.000000f, 0.264869f},
        {1600.000000f, 0.504513f},
        {3200.000000f, 0.525013f},
        {6400.000000f, 0.521649f},
	},
	/*********************************************/
	{ 140, 110, 110, 140},           //CoarseStr
	{ 20, 25, 30, 35, 40, 50, 60, 70, 75, 30, 35, 35, 35, 40, 40, 50},    //lutFineStr
	{
	    {1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3},   //ChromaStrR
        {0,0,0,0,1,1,1,2,2,2,2,2,2,0,0,0},   //ChromaStrGr
	    {0,0,0,0,1,1,1,2,2,2,2,2,2,0,0,0},   //ChromaStrGb
	    {1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3}    //ChromaStrB
	},
	{ 600, 600, 600, 550, 500, 450, 400, 350, 300, 600, 600, 600, 600, 600, 600, 600},	   //lutCoringWeight
	{ 600, 600, 600, 600, 650, 650, 650, 650, 700, 700, 700, 700, 700, 700, 700, 700, \
           800, 800, 800, 850, 850, 850, 900, 900, 900, 950, 950, 950, 1000, 1000, 1000, 1000, 1000}
};

HI_U32 cmos_get_isp_default(ISP_CMOS_DEFAULT_S *pstDef)
{
    if (HI_NULL == pstDef)
    {
        printf("null pointer when get isp default value!\n");
        return -1;
    }

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
	memcpy(&pstDef->stLsc.stLscUniParaTable, &g_stCmosLscUniTable, sizeof(ISP_LSC_CABLI_UNI_TABLE_S));
	memcpy(&pstDef->stLsc.stLscParaTable[0], &g_stCmosLscTable[0], sizeof(ISP_LSC_CABLI_TABLE_S)*HI_ISP_LSC_LIGHT_NUM);
    memcpy(&pstDef->stCa,        &g_stIspCA,sizeof(ISP_CMOS_CA_S));
    switch (genSensorMode)
    {
        default:
        case WDR_MODE_NONE:
		    memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
            memcpy(&pstDef->stDrc, &g_stIspDRC, sizeof(ISP_CMOS_DRC_S));
			memcpy(&pstDef->stGamma, &g_stIspGamma, sizeof(ISP_CMOS_GAMMA_S));
			memcpy(&pstDef->stBayerNr, &g_stIspBayerNr, sizeof(ISP_CMOS_BAYERNR_S));
		    memcpy(&pstDef->stGe, &g_stIspGe, sizeof(ISP_CMOS_GE_S));
		    memcpy(&pstDef->stFcr, &g_stIspFcr, sizeof(ISP_CMOS_FCR_S));
			memcpy(&pstDef->stDpc, &g_stCmosDpc, sizeof(ISP_CMOS_DPC_S));
        break;
    }
    pstDef->stSensorMaxResolution.u32MaxWidth  = 1920;
    pstDef->stSensorMaxResolution.u32MaxHeight = 1080;
    return 0;
}

HI_U32 cmos_get_isp_black_level(ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;

    if (HI_NULL == pstBlackLevel)
    {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }

    pstBlackLevel->bUpdate = HI_FALSE;

	for (i=0; i<4; i++)
	{
		pstBlackLevel->au16BlackLevel[i] = 0x40;
	}

    return 0;
}

HI_VOID cmos_set_pixel_detect(HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps = 6804, u32MaxIntTime_5Fps = 6802;

    if (SENSOR_1080P_30FPS_MODE == gu8SensorImageMode)
    {
        u32FullLines_5Fps = (VMAX_1080P30_LINEAR * 30) / 5;
    }
    else
    {
        return;
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps - 2;

    if (bEnable)
    {
        sensor_write_register(0x23, (u32FullLines_5Fps & 0xFF00) >> 8);
        sensor_write_register(0x22, u32FullLines_5Fps & 0xFF);
        sensor_write_register(0x02, (u32MaxIntTime_5Fps & 0xFF00) >> 8);
        sensor_write_register(0x01, u32MaxIntTime_5Fps & 0xFF);
        sensor_write_register(0x00, 0x00);
    }
    else
    {
        sensor_write_register(VMAX_ADDR_H, (gu32FullLinesStd & 0xFF00) >> 8);
        sensor_write_register(VMAX_ADDR_L, gu32FullLinesStd & 0xFF);
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
            gu32FullLinesStd = VMAX_1080P30_LINEAR;
            genSensorMode = WDR_MODE_NONE;
            printf("linear mode\n");
        break;

        default:
            printf("NOT support this mode!\n");
            return;
        break;
    }
    gu32FullLines = gu32FullLinesStd;
    gu32PreFullLines = gu32FullLines;

    return;
}

HI_U32 cmos_get_sns_regs_info(ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;

    if (HI_FALSE == bInit)
    {
        g_stSnsRegsInfo.enSnsType = ISP_SNS_I2C_TYPE;
        g_stSnsRegsInfo.u8Cfg2ValidDelayMax = 2;
        g_stSnsRegsInfo.u32RegNum = 5;

        for (i=0; i<g_stSnsRegsInfo.u32RegNum; i++)
        {
            g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
            g_stSnsRegsInfo.astI2cData[i].u8DevAddr = sensor_i2c_addr;
            g_stSnsRegsInfo.astI2cData[i].u32AddrByteNum = sensor_addr_byte;
            g_stSnsRegsInfo.astI2cData[i].u32DataByteNum = sensor_data_byte;
        }
        g_stSnsRegsInfo.astI2cData[0].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[0].u32RegAddr = EXPOSURE_ADDR_L;
        g_stSnsRegsInfo.astI2cData[1].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[1].u32RegAddr = EXPOSURE_ADDR_H;

        g_stSnsRegsInfo.astI2cData[2].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[2].u32RegAddr = AGC_ADDR;

        g_stSnsRegsInfo.astI2cData[3].u32RegAddr = VMAX_ADDR_L;
        g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[4].u32RegAddr = VMAX_ADDR_H;
        g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 0;

        bInit = HI_TRUE;
    }
    else
    {
        for (i=0; i<g_stSnsRegsInfo.u32RegNum; i++)
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
	}

	if(gu32PreFullLines < gu32FullLines)
    {
        g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 0;
    }
    else
    {
        g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 1;
    }

    if (HI_NULL == pstSnsRegsInfo)
    {
        printf("null pointer when get sns reg info!\n");
        return -1;
    }

    memcpy(pstSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&g_stPreSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    gu32PreFullLines = gu32FullLines;

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

    if (HI_FALSE == bSensorInit)
    {
        gu8SensorImageMode = u8SensorImageMode;

        return 0;
    }

    if (u8SensorImageMode == gu8SensorImageMode)
    {
        return -1;
    }

    gu8SensorImageMode = u8SensorImageMode;

    return 0;
}

HI_VOID sensor_global_init(HI_VOID)
{
    gu8SensorImageMode = SENSOR_1080P_30FPS_MODE;
    genSensorMode = WDR_MODE_NONE;
    gu32FullLinesStd = VMAX_1080P30_LINEAR;
    gu32FullLines = VMAX_1080P30_LINEAR;
    gu32PreFullLines = VMAX_1080P30_LINEAR;
    bInit = HI_FALSE;
    bSensorInit = HI_FALSE;
    memset(&g_stSnsRegsInfo, 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_stPreSnsRegsInfo, 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
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

int sensor_set_init(ISP_INIT_ATTR_S *pstInitAttr)
{
    ISP_DEV IspDev = 0;

    g_au16InitWBGain[IspDev][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[IspDev][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[IspDev][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[IspDev] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[IspDev] = pstInitAttr->u16SampleBgain;

    return 0;
}

#else  /* hi3516cv200 / hi3518ev200 (V2 SDK) */
/* ============================================================== *
 * hi3516cv200 (V2) implementation                                 *
 * ============================================================== */

extern const unsigned int sensor_i2c_addr;
extern unsigned int sensor_addr_byte;
extern unsigned int sensor_data_byte;

#define FULL_LINES_MAX          (0xFFFF)
#define SENSOR_720P_30FPS_MODE  (1)
#define SENSOR_960P_30FPS_MODE  (2)
#define SENSOR_1080P_30FPS_MODE (3)
#define SENSOR_1080P_60FPS_MODE (4)
#define VMAX_720P30_LINEAR      (1328)
#define VMAX_960P30_LINEAR      (1328)
#define VMAX_1080P30_LINEAR     (1134)

HI_U8 gu8SensorImageMode = SENSOR_720P_30FPS_MODE;
WDR_MODE_E genSensorMode = WDR_MODE_NONE;
static HI_U32 gu32FullLinesStd = VMAX_720P30_LINEAR;
static HI_U32 gu32FullLines = VMAX_720P30_LINEAR;
static HI_BOOL bInit = HI_FALSE;
HI_BOOL bSensorInit = HI_FALSE;
ISP_SNS_REGS_INFO_S g_stSnsRegsInfo = {0};
ISP_SNS_REGS_INFO_S g_stPreSnsRegsInfo = {0};

/* AE default parameter and function */
static HI_S32 cmos_get_ae_default(AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
        if (HI_NULL == pstAeSnsDft)
        {
                printf("null pointer when get ae default value!\n");
                return -1;
        }
        pstAeSnsDft->u32LinesPer500ms = VMAX_720P30_LINEAR * 30 / 2;
        pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
        pstAeSnsDft->u32FlickerFreq = 0;
        pstAeSnsDft->au8HistThresh[0] = 0xd;
        pstAeSnsDft->au8HistThresh[1] = 0x28;
        pstAeSnsDft->au8HistThresh[2] = 0x60;
        pstAeSnsDft->au8HistThresh[3] = 0x80;
        pstAeSnsDft->u8AeCompensation = 0x38;
        pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
        pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
        pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
        pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 2;
        pstAeSnsDft->u32MinIntTime = 2;
        pstAeSnsDft->u32MaxIntTimeTarget = pstAeSnsDft->u32MaxIntTime;
        pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
        pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
        pstAeSnsDft->stAgainAccu.f32Accuracy = 1;
        pstAeSnsDft->u32MaxAgain = 8196;
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
        return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
        if ((f32Fps <= 30) && (f32Fps >= 0.5))
        {
                if(SENSOR_720P_30FPS_MODE == gu8SensorImageMode)
                {
                        gu32FullLinesStd = VMAX_720P30_LINEAR * 30 / f32Fps;
                }
                if(SENSOR_960P_30FPS_MODE == gu8SensorImageMode)
                {
                        gu32FullLinesStd = VMAX_960P30_LINEAR * 30 / f32Fps;
                }
                if(SENSOR_1080P_30FPS_MODE == gu8SensorImageMode)
                {
                        gu32FullLinesStd = VMAX_1080P30_LINEAR * 30 / f32Fps;
                }
                g_stSnsRegsInfo.astI2cData[3].u32Data = (gu32FullLinesStd >> 8) & 0xff;
                g_stSnsRegsInfo.astI2cData[4].u32Data = gu32FullLinesStd & 0xff;
                pstAeSnsDft->u32MaxIntTime = gu32FullLinesStd - 2;
                pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
        }
        else
        {
                printf("Not support Fps: %f\n", f32Fps);
                return;
        }
        return;
}

static HI_VOID cmos_slow_framerate_set(HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
        gu32FullLines = u32FullLines;
        g_stSnsRegsInfo.astI2cData[3].u32Data = (gu32FullLines >> 8) & 0xff;
        g_stSnsRegsInfo.astI2cData[4].u32Data = gu32FullLines & 0xff;
        pstAeSnsDft->u32MaxIntTime = gu32FullLines - 2;
        return;
}

static HI_VOID cmos_inttime_update(HI_U32 u32IntTime)
{
        g_stSnsRegsInfo.astI2cData[0].u32Data = (u32IntTime >> 8) & 0xFF;
        g_stSnsRegsInfo.astI2cData[1].u32Data = u32IntTime & 0xFF;
        return;
}

static HI_U32 analog_gain_table[4] =
{
        1024, 2048, 4096, 8192
};

static HI_VOID cmos_again_calc_table(HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
        int i;
        if (*pu32AgainLin >= analog_gain_table[3])
        {
                *pu32AgainLin = analog_gain_table[3];
                *pu32AgainDb = 3;
                return ;
        }
        for (i = 1; i < 4; i++)
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
        g_stSnsRegsInfo.astI2cData[2].u32Data = (u32Again << 4) | ((u32Dgain - 16) & 0xf);
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
        5000,
        {
                0x195, 0x8092, 0x8003,
                0x8067, 0x17e, 0x8017,
                0x8067, 0x80f3, 0x25b
        },
        4000,
        {
                0x195, 0x8092, 0x8003,
                0x8067, 0x17e, 0x8017,
                0x8067, 0x80f3, 0x25b
        },
        2850,
        {
                0xe9, 0x25, 0x800e,
                0x80bb, 0x269, 0x80ad,
                0x80a9, 0x8046, 0x1ef
        }
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
        /* bvalid */
        1,
        /*1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768*/
        /* saturation */
        {
                0x80,0x80,0x7a,0x78,0x70,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38
        }
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
        pstAwbSnsDft->au16GainOffset[0] = 400;
        pstAwbSnsDft->au16GainOffset[1] = 0x100;
        pstAwbSnsDft->au16GainOffset[2] = 0x100;
        pstAwbSnsDft->au16GainOffset[3] = 0x1b9;
        pstAwbSnsDft->as32WbPara[0] = 168;
        pstAwbSnsDft->as32WbPara[1] = -66;
        pstAwbSnsDft->as32WbPara[2] = -154;
        pstAwbSnsDft->as32WbPara[3] = 178500;
        pstAwbSnsDft->as32WbPara[4] = 128;
        pstAwbSnsDft->as32WbPara[5] = -130964;
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

#define DMNR_CALIB_CARVE_NUM_H42 5
float g_coef_calib_h42[DMNR_CALIB_CARVE_NUM_H42][4] =
{
        {100.000000f, 2.000000f, 0.040110f, 8.391255f, },
        {800.000000f, 2.903090f, 0.047486f, 9.635272f, },
        {2525.000000f, 3.402261f, 0.055028f, 13.068982f, },
        {5500.000000f, 3.740363f, 0.063797f, 17.750826f, },
        {6194.000000f, 3.791971f, 0.062829f, 17.908888f, },
};

static ISP_NR_ISO_PARA_TABLE_S g_stNrIsoParaTab[HI_ISP_NR_ISO_LEVEL_MAX] =
{
        {1500,       160,             256-256,            0 },
        {1500,       120,             256-256,            0 },
        {1500,       100,             256-256,            0 },
        {1750,       80,              256-256,            8 },
        {1500,       255,             256-256,            6 },
        {1500,       255,             256-256,            0 },
        {1375,       255,             256-256,            0 },
        {1375,       255,             256-256,            0 },
        {1375,       255,             256-256,            0 },
        {1375,       255,             256-256,            0 },
        {1250,       255,             256-256,            0 },
        {1250,       255,             256-256,            0 },
        {1250,       255,             256-256,            0 },
        {1250,       255,             256-256,            0 },
        {1250,       255,             256-256,            0 },
        {1250,       255,             256-256,            0 },
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
        { 8, 8, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 0, 0},
        {24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24},
        /*For Ahd*/
        400, /*u16UuSlope*/
        {512,512,512,512,512,512,512,  400,  0,0,0,0,0,0,0,0}
};

static ISP_CMOS_GE_S g_stIspGe =
{
        /*For GE*/
        1,    /*bEnable*/
        7,    /*u8Slope*/
        7,    /*u8Sensitivity*/
        8192, /*u16Threshold*/
        8192, /*u16SensiThreshold*/
        {1024,1024,1024,2048,2048,2048,2048,  2048,  2048,2048,2048,2048,2048,2048,2048,2048}
};

static ISP_CMOS_RGBSHARPEN_S g_stIspRgbSharpen =
{
        {0,       0,   0,   0,    0,   1,   1,   1,    1,    1,    1,     1,     1,     1,     1,       1},
        {120,  64,  64,  43,  43,  43,  18,  18,    18,  200,  250,   250,   250,   250,    250,    250},
        {128, 200, 103, 86,  86,  86,  80,  80,    80,  250,  250,   250,   250,   250,    250,    250},
        {0,   0,   0,    0,   0,   0,   0,   40,  190,  200,  220,   250,   250,   250,     250,       250},
        {0,   0,   0,    0,   0,   0,   0,   60,  140,    0,    0,     0,    0,     0,     0,       0},
        {59,  59,  59,  59,  59,  59,  59,   59,  101,  0,   0,    0,   0,    0,    0,      0},
        {117, 117, 117, 108, 108, 108, 122,  122, 139,  0,   0,    0,   0,    0,    0,     0},
};

static ISP_CMOS_UVNR_S g_stIspUVNR =
{
        {1,         2,       4,      5,      7,      48,     32,     16,     16,     16,      16,     16,     16,     16,     16,        16},
        {0,             0,              0,              0,              0,              0,              0,              0,              0,              1,                      1,              2,              2,              2,              2,              2},
        {0,             0,              0,              16,             34,             34,             34,             34,             34,             34,             34,             34,             34,             34,             34,                     34}
};

static ISP_CMOS_DPC_S g_stCmosDpc =
{
        {0,0,0,1,1,1,2,2,2,3,3,3,3,3,3,3},
        {0,0,0,0,0,0,0,0,0x24,0x80,0x80,0x80,0xE5,0xE5,0xE5,0xE5},
};

HI_U32 cmos_get_isp_default(ISP_CMOS_DEFAULT_S *pstDef)
{
        if (HI_NULL == pstDef)
        {
                printf("null pointer when get isp default value!\n");
                return -1;
        }
        memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
        pstDef->stDrc.bEnable               = HI_FALSE;
        pstDef->stDrc.u8Asymmetry           = 0x02;
        pstDef->stDrc.u8SecondPole          = 0xC0;
        pstDef->stDrc.u8Stretch             = 0x3C;
        pstDef->stDrc.u16BrightGainLmt      = 0x7F;
        pstDef->stDrc.u16DarkGainLmtC       = 0x7F;
        pstDef->stDrc.u16DarkGainLmtY       = 0x7F;
        pstDef->stDrc.u8RangeVar            = 0x00;
        pstDef->stDrc.u8SpatialVar          = 0x0A;
        memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
        memcpy(&pstDef->stRgbSharpen, &g_stIspRgbSharpen, sizeof(ISP_CMOS_RGBSHARPEN_S));
        memcpy(&pstDef->stGe, &g_stIspGe, sizeof(ISP_CMOS_GE_S));
        pstDef->stNoiseTbl.stNrCaliPara.u8CalicoefRow = DMNR_CALIB_CARVE_NUM_H42;
        pstDef->stNoiseTbl.stNrCaliPara.pCalibcoef    = (HI_FLOAT (*)[4])g_coef_calib_h42;
        memcpy(&pstDef->stNoiseTbl.stIsoParaTable[0], &g_stNrIsoParaTab[0],sizeof(ISP_NR_ISO_PARA_TABLE_S)*HI_ISP_NR_ISO_LEVEL_MAX);
        memcpy(&pstDef->stUvnr,       &g_stIspUVNR,       sizeof(ISP_CMOS_UVNR_S));
        memcpy(&pstDef->stDpc,       &g_stCmosDpc,       sizeof(ISP_CMOS_DPC_S));
        pstDef->stSensorMaxResolution.u32MaxWidth  = 1280;
        pstDef->stSensorMaxResolution.u32MaxHeight = 720;
        return 0;
}

HI_U32 cmos_get_isp_black_level(ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
        if (HI_NULL == pstBlackLevel)
        {
                printf("null pointer when get isp black level value!\n");
                return -1;
        }

        pstBlackLevel->bUpdate = HI_FALSE;

        pstBlackLevel->au16BlackLevel[0] = 64;
        pstBlackLevel->au16BlackLevel[1] = 64;
        pstBlackLevel->au16BlackLevel[2] = 64;
        pstBlackLevel->au16BlackLevel[3] = 64;
        return 0;
}

HI_VOID cmos_set_pixel_detect(HI_BOOL bEnable)
{
        if (bEnable)
        {
                sensor_write_register(0x22, 0x94);
                sensor_write_register(0x23, 0x11);
                sensor_write_register(0x01, 0x92);
                sensor_write_register(0x02, 0x11);
                sensor_write_register(0x00, 0x00);
        }
        else
        {
                sensor_write_register(0x22, (gu32FullLinesStd >> 8) & 0xff);
                sensor_write_register(0x23, gu32FullLinesStd & 0xff);
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
                        if (SENSOR_720P_30FPS_MODE == gu8SensorImageMode)
                        {
                                gu32FullLinesStd = VMAX_720P30_LINEAR;
                        }
                        if (SENSOR_960P_30FPS_MODE == gu8SensorImageMode)
                        {
                                gu32FullLinesStd = VMAX_960P30_LINEAR;
                        }
                        if (SENSOR_1080P_30FPS_MODE == gu8SensorImageMode)
                        {
                                gu32FullLinesStd = VMAX_1080P30_LINEAR;
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

                g_stSnsRegsInfo.u32RegNum = 5;

                for (i=0; i<g_stSnsRegsInfo.u32RegNum; i++)
                {
                        g_stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
                        g_stSnsRegsInfo.astI2cData[i].u8DevAddr = sensor_i2c_addr;
                        g_stSnsRegsInfo.astI2cData[i].u32AddrByteNum = sensor_addr_byte;
                        g_stSnsRegsInfo.astI2cData[i].u32DataByteNum = sensor_data_byte;
                }
                g_stSnsRegsInfo.astI2cData[0].u8DelayFrmNum = 0;
                g_stSnsRegsInfo.astI2cData[0].u32RegAddr = 0x02;

                g_stSnsRegsInfo.astI2cData[1].u8DelayFrmNum = 0;
                g_stSnsRegsInfo.astI2cData[1].u32RegAddr = 0x01;

                g_stSnsRegsInfo.astI2cData[2].u8DelayFrmNum = 1;
                g_stSnsRegsInfo.astI2cData[2].u32RegAddr = 0x00;

                g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 0;
                g_stSnsRegsInfo.astI2cData[3].u32RegAddr = 0x23;
                g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 0;
                g_stSnsRegsInfo.astI2cData[4].u32RegAddr = 0x22;

                bInit = HI_TRUE;
        }
        else
        {
                for (i=0; i<g_stSnsRegsInfo.u32RegNum; i++)
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

        if ((pstSensorImageMode->u16Width <= 1280) && (pstSensorImageMode->u16Height <= 720))
        {
                if (pstSensorImageMode->f32Fps <= 30)
                {
                        u8SensorImageMode = SENSOR_720P_30FPS_MODE;
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
        else if ((pstSensorImageMode->u16Width <= 1280) && (pstSensorImageMode->u16Height <= 960))
        {
                if (pstSensorImageMode->f32Fps <= 30)
                {
                        u8SensorImageMode = SENSOR_960P_30FPS_MODE;
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
        else if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080))
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

        /* Sensor first init */
        if (HI_FALSE == bSensorInit)
        {
                gu8SensorImageMode = u8SensorImageMode;
                return 0;
        }

        /* Switch SensorImageMode */
        if (u8SensorImageMode == gu8SensorImageMode)
        {
                return -1;
        }

        gu8SensorImageMode = u8SensorImageMode;

        return 0;
}

HI_VOID sensor_global_init()
{
        gu8SensorImageMode = SENSOR_720P_30FPS_MODE;
        genSensorMode = WDR_MODE_NONE;
        gu32FullLinesStd = VMAX_720P30_LINEAR;
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

#endif /* hi3516cv300 / cv200 split */

/****************************************************************************
 * registration callback — shared T2 shape across cv200 / cv300
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
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, JXF22_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, JXF22_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, JXF22_ID, &stAwbRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return 0;
}

int sensor_unregister_callback(void)
{
    ISP_DEV IspDev = 0;
    HI_S32 s32Ret;
    ALG_LIB_S stLib;

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, JXF22_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, &stLib, JXF22_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, &stLib, JXF22_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __JXF22_CMOS_H_ */
