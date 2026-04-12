/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_gamma.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "mpi_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#if 0
typedef struct hiISP_GAMMA_S
{
    HI_U8   u8change;
    HI_U16  au16GammaLut[GAMMA_NODE_NUM];
} ISP_GAMMA_S;

ISP_GAMMA_S g_astGammaCtx[ISP_MAX_DEV_NUM] = {{0}};
#define GAMMA_GET_CTX(dev, pstCtx)   pstCtx = &g_astGammaCtx[dev]
#endif

static HI_VOID RgbGammaCfg(ISP_DEV IspDev, HI_U16 *pGammaLut)
{
    HI_U32 i;
    HI_U8 u8GammaBankSel;
    
    u8GammaBankSel = hi_isp_gamma_rgb_use_lut_bank_1_read(IspDev);   
    hi_isp_gamma_mcu_priority_write(IspDev, 1);
    
    if (0 == u8GammaBankSel)
    {
        for (i = 0; i < GAMMA_NODE_NUM; i++)
        {
            hi_gamma_rgb_mem_bank1_array_data_write(IspDev, i, (HI_U32)(pGammaLut[i]));
        }
    }
    else
    {
        for (i = 0; i < GAMMA_NODE_NUM; i++)
        {
            hi_gamma_rgb_mem_bank0_array_data_write(IspDev, i, (HI_U32)(pGammaLut[i]));
        }
    }
    
    hi_isp_gamma_mcu_priority_write(IspDev, 0);
    hi_isp_gamma_rgb_use_lut_bank_1_write(IspDev, !u8GammaBankSel);    
}

HI_VOID GammaInitLinear(ISP_DEV IspDev)
{
    /*the following gamma is not really sRGB gamma*/
    HI_U16  au16sRGB[GAMMA_NODE_NUM]={
        0   ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,    
        1096,1138,1178,1218,1254,1280,1314,1346,1378,1408,1438,1467,1493,1519,1543,1568,    
        1592,1615,1638,1661,1683,1705,1726,1748,1769,1789,1810,1830,1849,1869,1888,1907,    
        1926,1945,1963,1981,1999,2017,2034,2052,2069,2086,2102,2119,2136,2152,2168,2184,    
        2200,2216,2231,2247,2262,2277,2292,2307,2322,2337,2351,2366,2380,2394,2408,2422,    
        2436,2450,2464,2477,2491,2504,2518,2531,2544,2557,2570,2583,2596,2609,2621,2634,    
        2646,2659,2671,2683,2696,2708,2720,2732,2744,2756,2767,2779,2791,2802,2814,2825,    
        2837,2848,2859,2871,2882,2893,2904,2915,2926,2937,2948,2959,2969,2980,2991,3001,    
        3012,3023,3033,3043,3054,3064,3074,3085,3095,3105,3115,3125,3135,3145,3155,3165,    
        3175,3185,3194,3204,3214,3224,3233,3243,3252,3262,3271,3281,3290,3300,3309,3318,    
        3327,3337,3346,3355,3364,3373,3382,3391,3400,3409,3418,3427,3436,3445,3454,3463,    
        3471,3480,3489,3498,3506,3515,3523,3532,3540,3549,3557,3566,3574,3583,3591,3600,    
        3608,3616,3624,3633,3641,3649,3657,3665,3674,3682,3690,3698,3706,3714,3722,3730,    
        3738,3746,3754,3762,3769,3777,3785,3793,3801,3808,3816,3824,3832,3839,3847,3855,    
        3862,3870,3877,3885,3892,3900,3907,3915,3922,3930,3937,3945,3952,3959,3967,3974,    
        3981,3989,3996,4003,4010,4018,4025,4032,4039,4046,4054,4061,4068,4075,4082,4089,4095
        };

    RgbGammaCfg(IspDev, au16sRGB);

    return;
}

HI_VOID GammaInitWDR(ISP_DEV IspDev)
{
    
#if 1
    /* sRGB gamma in WDR mode */
    HI_U16  au16sRGB[GAMMA_NODE_NUM]={
        0   ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,    
        1096,1138,1178,1218,1254,1280,1314,1346,1378,1408,1438,1467,1493,1519,1543,1568,    
        1592,1615,1638,1661,1683,1705,1726,1748,1769,1789,1810,1830,1849,1869,1888,1907,    
        1926,1945,1963,1981,1999,2017,2034,2052,2069,2086,2102,2119,2136,2152,2168,2184,    
        2200,2216,2231,2247,2262,2277,2292,2307,2322,2337,2351,2366,2380,2394,2408,2422,    
        2436,2450,2464,2477,2491,2504,2518,2531,2544,2557,2570,2583,2596,2609,2621,2634,    
        2646,2659,2671,2683,2696,2708,2720,2732,2744,2756,2767,2779,2791,2802,2814,2825,    
        2837,2848,2859,2871,2882,2893,2904,2915,2926,2937,2948,2959,2969,2980,2991,3001,    
        3012,3023,3033,3043,3054,3064,3074,3085,3095,3105,3115,3125,3135,3145,3155,3165,    
        3175,3185,3194,3204,3214,3224,3233,3243,3252,3262,3271,3281,3290,3300,3309,3318,    
        3327,3337,3346,3355,3364,3373,3382,3391,3400,3409,3418,3427,3436,3445,3454,3463,    
        3471,3480,3489,3498,3506,3515,3523,3532,3540,3549,3557,3566,3574,3583,3591,3600,    
        3608,3616,3624,3633,3641,3649,3657,3665,3674,3682,3690,3698,3706,3714,3722,3730,    
        3738,3746,3754,3762,3769,3777,3785,3793,3801,3808,3816,3824,3832,3839,3847,3855,    
        3862,3870,3877,3885,3892,3900,3907,3915,3922,3930,3937,3945,3952,3959,3967,3974,    
        3981,3989,3996,4003,4010,4018,4025,4032,4039,4046,4054,4061,4068,4075,4082,4089,4095
        };
#else
    /* default gamma in WDR mode */
    HI_U16 au16sqrt2sRGB[GAMMA_NODE_NUM]={
           0,   1,   2,   4,   8,  12,  17,  23,  30,  38,  47,  57,  68,  79,  92, 105,
         120, 133, 147, 161, 176, 192, 209, 226, 243, 260, 278, 296, 315, 333, 351, 370,
         390, 410, 431, 453, 474, 494, 515, 536, 558, 580, 602, 623, 644, 665, 686, 708,
         730, 751, 773, 795, 818, 840, 862, 884, 907, 929, 951, 974, 998,1024,1051,1073,
        1096,1117,1139,1159,1181,1202,1223,1243,1261,1275,1293,1313,1332,1351,1371,1389,
        1408,1427,1446,1464,1482,1499,1516,1533,1549,1567,1583,1600,1616,1633,1650,1667,
        1683,1700,1716,1732,1749,1766,1782,1798,1815,1831,1847,1863,1880,1896,1912,1928,
        1945,1961,1977,1993,2009,2025,2041,2057,2073,2089,2104,2121,2137,2153,2168,2184,
        2200,2216,2231,2248,2263,2278,2294,2310,2326,2341,2357,2373,2388,2403,2419,2434,
        2450,2466,2481,2496,2512,2527,2543,2558,2573,2589,2604,2619,2635,2650,2665,2680,
        2696,2711,2726,2741,2757,2771,2787,2801,2817,2832,2847,2862,2877,2892,2907,2922,
        2937,2952,2967,2982,2997,3012,3027,3041,3057,3071,3086,3101,3116,3130,3145,3160,
        3175,3190,3204,3219,3234,3248,3263,3278,3293,3307,3322,3337,3351,3365,3380,3394,
        3409,3424,3438,3453,3468,3482,3497,3511,3525,3540,3554,3569,3584,3598,3612,3626,
        3641,3655,3670,3684,3699,3713,3727,3742,3756,3770,3784,3799,3813,3827,3841,3856,
        3870,3884,3898,3912,3927,3941,3955,3969,3983,3997,4011,4026,4039,4054,4068,4082,4095
        };
#endif

    RgbGammaCfg(IspDev, au16sRGB);

    return;
}

static HI_VOID GammaExtRegsDefault(ISP_DEV IspDev)
{
    hi_ext_system_gamma_curve_type_write(IspDev, HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT);
    
    return;
}

static HI_VOID GammaRegsDefault(ISP_DEV IspDev) 
{
    return;
}

static HI_VOID GammaExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U32 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
   
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stGamma.bValid)
    {
        for (i = 0; i < GAMMA_NODE_NUM; i++)
        {
            hi_isp_ext_gamma_rgb_mem_array_data_write(IspDev, i, (HI_U32)pstSnsDft->stGamma.au16Gamma[i]);
        }        
    }
    
    return;
}

static HI_VOID GammaRegsInitialize(ISP_DEV IspDev)
{
    HI_S32 u8WDRMode;  
    ISP_CTX_S *pstIspCtx = HI_NULL;    
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    /* enable gamma */
    hi_isp_gamma_enable_write(IspDev, 1);
    hi_isp_gamma_rgb_use_lut_bank_1_write(IspDev, 1);
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (pstSnsDft->stGamma.bValid)
    {
        RgbGammaCfg(IspDev, pstSnsDft->stGamma.au16Gamma);      
    }
    else
    {
        if (IS_LINEAR_MODE(u8WDRMode))
        {
            /* sensor in linear mode */
            GammaInitLinear(IspDev);
        }
        else if (IS_WDR_MODE(u8WDRMode))
        {
            /* sensor in WDR mode */
            GammaInitWDR(IspDev);                
        }
        else
        {
            /* unknow u8Mode */
        }
    }
    
    return;
}

static HI_S32 GammaReadExtregs(ISP_DEV IspDev)
{   
    return 0;
}

HI_S32 ISP_GammaInit(ISP_DEV IspDev)
{
    GammaRegsDefault(IspDev);
    GammaExtRegsDefault(IspDev);
    GammaRegsInitialize(IspDev);
    GammaExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    GammaReadExtregs(IspDev);   
#if 0    
    ISP_GAMMA_S *pstGamma = HI_NULL;    
    GAMMA_GET_CTX(IspDev, pstGamma);
    
    GammaReadExtregs(IspDev);
    
    if(HI_TRUE == pstGamma->u8change)
    {       
        memcpy(((ISP_REG_CFG_S *)pRegCfg)->stKernelCfg.stGammaRegCfg.au16GammaLut, pstGamma->au16GammaLut, sizeof(pstGamma->au16GammaLut));

        ((ISP_REG_CFG_S *)pRegCfg)->stKernelCfg.unKey.bit1GammaCfg = 1;
      
    
        pstGamma->u8change = HI_FALSE; 
    }
    else
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stKernelCfg.unKey.bit1GammaCfg = 0;
    }

#endif

    return HI_SUCCESS;
}

HI_S32 ISP_GammaCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_U8 u8Mode = *((HI_U8 *)pValue);
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            if (pstSnsDft->stGamma.bValid)
            {
                RgbGammaCfg(IspDev, pstSnsDft->stGamma.au16Gamma);
            }
            else
            {
                if (IS_LINEAR_MODE(u8Mode))
                {
                    /* sensor in linear mode */
                    GammaInitLinear(IspDev);
                }
                else if (IS_WDR_MODE(u8Mode))
                {
                    /* sensor in WDR mode */
                    GammaInitWDR(IspDev);                
                }
                else
                {
                    /* unknow u8Mode */
                }            
            }

            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_GammaExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterGamma(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_GAMMA;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_GammaInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_GammaRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_GammaCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_GammaExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

