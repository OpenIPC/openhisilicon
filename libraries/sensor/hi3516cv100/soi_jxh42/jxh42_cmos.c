#if !defined(__JXH42_CMOS_H_)
#define __JXH42_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "mpi_af.h"
#include "mpi_vpss.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define FULL_LINES_DEFAULT 750

#define JXH42_ID 42

#define HIGH_8BITS(x) ((x & 0xff00) >> 8)
#define LOW_8BITS(x) (x & 0x00ff)


HI_U32 gu32FullLinesStd = FULL_LINES_DEFAULT;
HI_U32 gu32FullLines = FULL_LINES_DEFAULT;

HI_U8 gu8SensorMode = 0; // which WDR mode default?


HI_VOID cmos_set_wdr_mode(HI_U8 u8Mode)
{
    //fprintf(stderr, "DBG: cmos_set_wdr_mode(%d)\n", u8Mode);
    if (u8Mode >= 2)
    {
        fprintf(stderr, "NO support for this mode!");
    }
    else
    {
        gu8SensorMode = u8Mode;
    }
}

static HI_S32 cmos_get_ae_default(AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    //fprintf(stderr, "DBG: cmos_get_ae_default(*)\n");
    if (HI_NULL == pstAeSnsDft)
    {
        fprintf(stderr, "null pointer when get ae default value!\n");
        return -1;
    }

    gu32FullLinesStd = FULL_LINES_DEFAULT;

    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60;
    pstAeSnsDft->au8HistThresh[3] = 0x80;

    pstAeSnsDft->u8AeCompensation = 0x40;

    pstAeSnsDft->u32LinesPer500ms = 11265; // FULL_LINES_DEFAULT * FPS30 / 2
    pstAeSnsDft->u32FullLinesStd = gu32FullLinesStd;
    pstAeSnsDft->u32FlickerFreq = 0;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->u32MaxIntTime = 747; // ??? FULL_LINES_DEFAULT - 1;
    pstAeSnsDft->u32MinIntTime = 8;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535;
    pstAeSnsDft->u32MinIntTimeTarget = 2;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_DB;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 6.0;
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0625;

    pstAeSnsDft->u32MaxAgain = 3;
    pstAeSnsDft->u32MinAgain = 0;
    pstAeSnsDft->u32MaxDgain = 31;
    pstAeSnsDft->u32MinDgain = 16;

    pstAeSnsDft->u32MaxDgainTarget = 31;
    pstAeSnsDft->u32MinDgainTarget = 16;
    pstAeSnsDft->u32MaxAgainTarget = 3;
    pstAeSnsDft->u32MinAgainTarget = 0;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; // 512
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift; // 256

    return 0;
}

HI_U32 cmos_get_isp_black_level(ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;

    if (HI_NULL == pstBlackLevel)
    {
        fprintf(stderr, "null pointer when get isp black level value!\n");
        return -1;
    }

    //fprintf(stderr, "DBG: cmos_get_isp_black_level(*)\n");

    pstBlackLevel->bUpdate = HI_FALSE;
    pstBlackLevel->au16BlackLevel[0] = 0x14;
    pstBlackLevel->au16BlackLevel[1] = 0x14;
    pstBlackLevel->au16BlackLevel[2] = 0x14;
    pstBlackLevel->au16BlackLevel[3] = 0x14;
 
    return 0;
}

static unsigned old_gain_val = 0;

unsigned int cmos_get_gaininfo()
{
    ISP_INNER_STATE_INFO_EX_S inf;
    HI_MPI_ISP_QueryInnerStateInfoEx(&inf);
//    fprintf(stderr, "\tu32AnalogGain, u32ISPDigitalGain = %u, %u\n", inf.u32AnalogGain, inf.u32ISPDigitalGain);
    unsigned gain_val = (inf.u32AnalogGain * inf.u32ISPDigitalGain) >> 20;

    if (old_gain_val == gain_val) return 0;

    old_gain_val = gain_val;

    return gain_val;
}

void cmos_Set_DPC(HI_U16 pxThresh, HI_U16 pxSlope)
{
    ISP_DP_ATTR_S dp;

    HI_MPI_ISP_GetDefectPixelAttr(&dp);
    dp.bEnableDynamic = HI_TRUE;
    dp.u16DynamicBadPixelThresh = pxThresh;
    dp.u16DynamicBadPixelSlope = pxSlope;
    HI_MPI_ISP_SetDefectPixelAttr(&dp);
}

void cmos_Set_DRC(HI_BOOL en, HI_BOOL manual, HI_U32 strength_target)
{
    ISP_DRC_ATTR_S drc;

    HI_MPI_ISP_GetDRCAttr(&drc);
    drc.bDRCEnable = en;
    drc.bDRCManualEnable = manual;
    drc.u32StrengthTarget = strength_target;
    HI_MPI_ISP_SetDRCAttr(&drc);
}

void cmos_set_3Ddenoise(HI_U32 sf, HI_U32 tf, HI_U32 chroma)
{
    VPSS_GRP_PARAM_S grp_param;

    HI_MPI_VPSS_GetGrpParam(0, &grp_param);
    grp_param.u32SfStrength = sf;
    grp_param.u32TfStrength = tf;
    grp_param.u32ChromaRange = chroma;
    HI_MPI_VPSS_SetGrpParam(0, &grp_param);
}

static HI_VOID cmos_inttime_update(HI_U32 u32IntTime)
{
    unsigned gaininfo = cmos_get_gaininfo();
    if (gaininfo)
    {
        //fprintf(stderr, "\tgaininfo = %u\n", gaininfo);
        cmos_Set_DPC(40 - gaininfo, 38 * gaininfo + 1111);

        if (gaininfo <= 2)
        {
            cmos_Set_DRC(HI_TRUE, HI_TRUE, 64);
            cmos_set_3Ddenoise(28, 7, 60);
        }
        else if (gaininfo <= 4)
        {
            cmos_Set_DRC(HI_TRUE, HI_TRUE, 48);
            cmos_set_3Ddenoise(32, 8, 80);
        }
        else if (gaininfo <= 6)
        {
            cmos_Set_DRC(HI_TRUE, HI_TRUE, 32);
            cmos_set_3Ddenoise(36, 9, 100);
        }
        else if (gaininfo <= 8)
        {
            cmos_Set_DRC(HI_TRUE, HI_TRUE, 16);
            cmos_set_3Ddenoise(40, 10, 120);
        }
        else if (gaininfo > 12)
        {
            cmos_Set_DRC(HI_FALSE, HI_FALSE, 0);
            cmos_set_3Ddenoise(60, 12, 160);
        }
        else // 9...11
        {
            cmos_Set_DRC(HI_TRUE, HI_TRUE, 8);
            cmos_set_3Ddenoise(50, 11, 140);
        }
    }

    //fprintf(stderr, "DBG: set explosure to %u\n", u32IntTime);
    unsigned explosure = u32IntTime;
    sensor_write_register(0x1, explosure & 0xFF);
    sensor_write_register(0x2, (explosure >> 8) & 0xFF);
}

static unsigned char additional_gain_vals[4] = { 0, 0x10, 0x20, 0x30 };

static HI_VOID cmos_gains_update(HI_U32 u32Again, HI_U32 u32Dgain)
{
    //fprintf(stderr, "DBG: cmos_gains_update(%u, %u)\n", u32Again, u32Dgain);
    // TODO large work
    unsigned char new_gain_value = u32Dgain & 0xF;
    if (u32Again <= 3)
    {
        new_gain_value |= additional_gain_vals[u32Again];
    }
    //fprintf(stderr, "DBG: podsovyvaem 0x%u gain\n", new_gain_value);
    sensor_write_register(0x0, new_gain_value);
}

void sensor_global_init()
{
    gu8SensorMode = 0;
}

static ISP_CMOS_NOISE_TABLE_S g_stIspNoiseTable =
{
    /* bvalid */
    1,

    //nosie_profile_weight_lut
   {
        0x19, 0x20, 0x24, 0x27, 0x29, 0x2B, 0x2D, 0x2E, 0x2F, 0x31, 0x32, 0x33, 0x34, 0x34, 0x35, 0x36,
        0x37, 0x37, 0x38, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3C, 0x3C, 0x3D, 0x3D, 0x3D, 0x3E,
        0x3E, 0x3E, 0x3F, 0x3F, 0x3F, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42,
        0x43, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x45, 0x46, 0x46,
        0x46, 0x46, 0x46, 0x46, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
        0x48, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A,
        0x4A, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C,
        0x4C, 0x4C, 0x4C, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4E, 0x4E, 0x4E
   },

    //demosaic_weight_lut
   {
        0x19, 0x20, 0x24, 0x27, 0x29, 0x2B, 0x2D, 0x2E, 0x2F, 0x31, 0x32, 0x33, 0x34, 0x34, 0x35, 0x36,
        0x37, 0x37, 0x38, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3C, 0x3C, 0x3D, 0x3D, 0x3D, 0x3E,
        0x3E, 0x3E, 0x3F, 0x3F, 0x3F, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42,
        0x43, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x45, 0x46, 0x46,
        0x46, 0x46, 0x46, 0x46, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
        0x48, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A,
        0x4A, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C,
        0x4C, 0x4C, 0x4C, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4E, 0x4E, 0x4E
   }

};

static ISP_CMOS_AGC_TABLE_S g_stIspAgcTable =
{
    /* bvalid */
    1,

    /* sharpen_alt_d */
   {0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10},

    /* sharpen_alt_ud */
   {0xb0, 0xa0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40},

    /* snr_thresh    */
   {0x01, 0x09, 0x13, 0x1e, 0x28, 0x32, 0x3c, 0x46},

    /*  demosaic_lum_thresh   */
   {0x40,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    /* demosaic_np_offset   */
   {0x02, 0x08, 0x12, 0x1a, 0x20, 0x28, 0x30, 0x30},

    /* ge_strength    */
   {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37}
};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
    /* bvalid */
    1,

    /*vh_slope*/
    0xFF,

    /*aa_slope*/
    0xE4,

    /*va_slope*/
    0xEC,

    /*uu_slope*/
    0x9F,

    /*sat_slope*/
    0x5D,

    /*ac_slope*/
    0xCF,

    /*vh_thresh*/
    0x138,

    /*aa_thresh*/
    0xBA,

    /*va_thresh*/
    0xDA,

    /*uu_thresh*/
    0x148,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3,
};

HI_VOID cmos_set_pixel_detect(HI_BOOL bEnable)
{
    fprintf(stderr, "DNG: cmos_set_pixel_detect(%s)\n", bEnable ? "TRUE" : "FALSE");
}

static HI_S32 cmos_get_sensor_max_resolution(ISP_CMOS_SENSOR_MAX_RESOLUTION *pstSensorMaxResolution)
{
    if (HI_NULL == pstSensorMaxResolution)
    {
        fprintf(stderr, "null pointer when get sensor max resolution \n");
        return -1;
    }

    memset(pstSensorMaxResolution, 0, sizeof(ISP_CMOS_SENSOR_MAX_RESOLUTION));

    pstSensorMaxResolution->u32MaxWidth  = 1280;
    pstSensorMaxResolution->u32MaxHeight = 720;

    return 0;
}

HI_U32 cmos_get_isp_default(ISP_CMOS_DEFAULT_S *pstDef)
{   
    if (HI_NULL == pstDef)
    {
        fprintf(stderr, "null pointer when get isp default value!\n");
        return -1;
    }

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->stComm.u8Rggb = 3;
    pstDef->stComm.u8BalanceFe = 1;

    pstDef->stDenoise.u8SinterThresh = 21;
    pstDef->stDenoise.u8NoiseProfile = 0;
    pstDef->stDenoise.u16Nr0 = 0;
    pstDef->stDenoise.u16Nr1 = 0;

    pstDef->stDrc.u8DrcBlack = 0;
    pstDef->stDrc.u8DrcVs = 4;
    pstDef->stDrc.u8DrcVi = 8;
    pstDef->stDrc.u8DrcSm = -96;
    pstDef->stDrc.u16DrcWl = 1279;

    memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTable, sizeof(ISP_CMOS_NOISE_TABLE_S));
    memcpy(&pstDef->stAgcTbl, &g_stIspAgcTable, sizeof(ISP_CMOS_AGC_TABLE_S));
    memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
}

HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = sensor_init;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_sensor_max_resolution = cmos_get_sensor_max_resolution;

    return 0;
}

void cmos_fps_set(HI_U8 u8Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    //fprintf(stderr, "DBG: cmos_fps_set(u8Fps = %u)\n", u8Fps);
    int full_lines; // r0

    full_lines = 22530 / u8Fps;
    gu32FullLinesStd = full_lines;
    pstAeSnsDft->u32FullLinesStd = full_lines;
    pstAeSnsDft->u32MaxIntTime = full_lines - 2;
    pstAeSnsDft->u32LinesPer500ms = (unsigned int)(full_lines * u8Fps) >> 1;
}

static HI_VOID cmos_slow_framerate_set(HI_U16 u16FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    //fprintf(stderr, "DBG: cmos_slow_framerate_set(u16FullLines = %u)\n", u16FullLines);
    HI_U16 full_lines = u16FullLines;
    if (full_lines > 2253) full_lines = 2253;

    gu32FullLines = full_lines;
    sensor_write_register(0x22, LOW_8BITS(gu32FullLines));
    sensor_write_register(0x23, HIGH_8BITS(gu32FullLines));

    pstAeSnsDft->u32MaxIntTime = gu32FullLines - 2;
}

HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set= cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;

    return 0;
}

static AWB_CCM_S g_stAwbCcm =
{
    4930,
    {
        0x1A3, 0x8073, 0x8030,
        0x8042, 0x1C5, 0x8083, 
        0x8007, 0x80A6, 0x1AD,
    },
    3885,
    {
        0x1FB, 0x80D4, 0x8027,
        0x8058, 0x1B4, 0x805C, 
        0x8005, 0x80C0, 0x1C5, 
    },
    2790,
    {
        0x1A8, 0x805B, 0x804D,
        0x8072, 0x19D, 0x802B,
        0x80B3, 0x8366, 0x519,
    }

};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /* saturation */
    {0x80,0x78,0x70,0x68,0x60,0x58,0x50,0x48}
};

static HI_S32 cmos_get_awb_default(AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    //fprintf(stderr, "DBG: cmos_get_awb_default(*)\n");
    if (HI_NULL == pstAwbSnsDft)
    {
        fprintf(stderr, "null pointer when get awb default value!\n");
        return -1;
    }

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4930;

    pstAwbSnsDft->au16GainOffset[0] = 0x1d2;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100;
    pstAwbSnsDft->au16GainOffset[3] = 0x199;

    pstAwbSnsDft->as32WbPara[0] = 99;
    pstAwbSnsDft->as32WbPara[1] = 47;
    pstAwbSnsDft->as32WbPara[2] = -110;
    pstAwbSnsDft->as32WbPara[3] = 152677;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -104036;

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

int sensor_register_callback(void)
{
    HI_S32 s32Ret;
    ALG_LIB_S stLib;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(JXH42_ID, &stIspRegister);
    if (s32Ret)
    {   
        fprintf(stderr, "sensor register callback function failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AE_LIB_NAME);
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(&stLib, JXH42_ID, &stAeRegister);
    if (s32Ret)
    {   
        fprintf(stderr, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(&stLib, JXH42_ID, &stAwbRegister);
    if (s32Ret)
    {
        fprintf(stderr, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    return 0;
}
    
int sensor_unregister_callback(void)
{
    HI_S32 s32Ret;
    ALG_LIB_S stLib;

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(JXH42_ID);
    if (s32Ret)
    {
        fprintf(stderr, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AE_LIB_NAME);
    s32Ret = HI_MPI_AE_SensorUnRegCallBack(&stLib, JXH42_ID);
    if (s32Ret)
    {
        fprintf(stderr, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(&stLib, JXH42_ID);
    if (s32Ret)
    {
        fprintf(stderr, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    return 0;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif


