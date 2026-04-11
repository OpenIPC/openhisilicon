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

typedef struct hiISP_GAMMA_S
{
    HI_U8   u8change;
    HI_U16  au16GammaLut[GAMMA_NODE_NUMBER];
} ISP_GAMMA_S;

ISP_GAMMA_S g_astGammaCtx[ISP_MAX_DEV_NUM] = {{0}};
#define GAMMA_GET_CTX(dev, pstCtx)   pstCtx = &g_astGammaCtx[dev]

HI_VOID GammaInitLin(HI_VOID)
{
    HI_U16 i;
    #if 0
    HI_U16  au16sRGB[GAMMA_NODE_NUMBER]={
           0,  203,  347,  452,  539,  613,  679,  739,  794,  846,  894,  939,  982, 1023,
        1062, 1100, 1136, 1171, 1204, 1237, 1268, 1299, 1329, 1358, 1386, 1414, 1441, 1467,
        1493, 1519, 1543, 1568, 1592, 1615, 1638, 1661, 1683, 1705, 1726, 1748, 1769, 1789,
        1810, 1830, 1849, 1869, 1888, 1907, 1926, 1945, 1963, 1981, 1999, 2017, 2034, 2052,
        2069, 2086, 2102, 2119, 2136, 2152, 2168, 2184, 2200, 2216, 2231, 2247, 2262, 2277,
        2292, 2307, 2322, 2337, 2351, 2366, 2380, 2394, 2408, 2422, 2436, 2450, 2464, 2477,
        2491, 2504, 2518, 2531, 2544, 2557, 2570, 2583, 2596, 2609, 2621, 2634, 2646, 2659,
        2671, 2683, 2696, 2708, 2720, 2732, 2744, 2756, 2767, 2779, 2791, 2802, 2814, 2825,
        2837, 2848, 2859, 2871, 2882, 2893, 2904, 2915, 2926, 2937, 2948, 2959, 2969, 2980,
        2991, 3001, 3012, 3023, 3033, 3043, 3054, 3064, 3074, 3085, 3095, 3105, 3115, 3125,
        3135, 3145, 3155, 3165, 3175, 3185, 3194, 3204, 3214, 3224, 3233, 3243, 3252, 3262,
        3271, 3281, 3290, 3300, 3309, 3318, 3327, 3337, 3346, 3355, 3364, 3373, 3382, 3391,
        3400, 3409, 3418, 3427, 3436, 3445, 3454, 3463, 3471, 3480, 3489, 3498, 3506, 3515,
        3523, 3532, 3540, 3549, 3557, 3566, 3574, 3583, 3591, 3600, 3608, 3616, 3624, 3633,
        3641, 3649, 3657, 3665, 3674, 3682, 3690, 3698, 3706, 3714, 3722, 3730, 3738, 3746,
        3754, 3762, 3769, 3777, 3785, 3793, 3801, 3808, 3816, 3824, 3832, 3839, 3847, 3855,
        3862, 3870, 3877, 3885, 3892, 3900, 3907, 3915, 3922, 3930, 3937, 3945, 3952, 3959,
        3967, 3974, 3981, 3989, 3996, 4003, 4010, 4018, 4025, 4032, 4039, 4046, 4054, 4061,
        4068, 4075, 4082, 4089, 4095
        };
    #endif

    /*the following gamma is not really sRGB gamma*/
    HI_U16  au16sRGB[GAMMA_NODE_NUMBER]={
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

    hi_isp_gamma_mcu_priority_write(1);
    for (i=0; i<GAMMA_NODE_NUMBER; i++)
    {
        hi_gamma_rgb_mem_array_data_write(i, (HI_U32)(au16sRGB[i]));
    }
    hi_isp_gamma_mcu_priority_write(0);

    return;
}

HI_VOID GammaInitWDR(HI_VOID)
{
    HI_U16 i;
#if 0   /* sRGB gamma in WDR mode */
    HI_U16 au16sqrt2sRGB[GAMMA_NODE_NUMBER]={
           0,   1,   3,   7,  13,  20,  29,  40,  52,  65,  81,  98, 116, 136, 158, 181,
         203, 226, 248, 269, 291, 312, 334, 355, 376, 396, 417, 438, 458, 478, 499, 519,
         538, 558, 578, 598, 617, 637, 656, 675, 695, 714, 733, 752, 771, 789, 808, 827,
         845, 864, 883, 901, 919, 938, 956, 974, 992,1010,1028,1046,1064,1082,1100,1118,
        1136,1153,1171,1189,1206,1224,1241,1259,1276,1293,1311,1328,1345,1362,1379,1397,
        1414,1431,1448,1465,1482,1499,1515,1532,1549,1566,1583,1599,1616,1633,1649,1666,
        1683,1699,1716,1732,1749,1765,1781,1798,1814,1830,1847,1863,1879,1896,1912,1928,
        1944,1960,1976,1992,2008,2024,2041,2056,2072,2088,2104,2120,2136,2152,2168,2184,
        2199,2215,2231,2247,2262,2278,2294,2309,2325,2341,2356,2372,2387,2403,2418,2434,
        2449,2465,2480,2496,2511,2527,2542,2557,2573,2588,2603,2619,2634,2649,2664,2680,
        2695,2710,2725,2740,2756,2771,2786,2801,2816,2831,2846,2861,2876,2891,2906,2921,
        2936,2951,2966,2981,2996,3011,3026,3041,3056,3070,3085,3100,3115,3130,3145,3159,
        3174,3189,3204,3218,3233,3248,3262,3277,3292,3306,3321,3336,3350,3365,3379,3394,
        3409,3423,3438,3452,3467,3481,3496,3510,3525,3539,3554,3568,3582,3597,3611,3626,
        3640,3654,3669,3683,3697,3712,3726,3740,3755,3769,3783,3798,3812,3826,3840,3855,
        3869,3883,3897,3911,3926,3940,3954,3968,3982,3996,4010,4025,4039,4053,4067,4081,4095
        };
#endif
#if 1   /* default gamma in WDR mode */
    HI_U16 au16sqrt2sRGB[GAMMA_NODE_NUMBER]={
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
#if 0   /* high contrast gamma in WDR mode */
    HI_U16 au16sqrt2sRGB[GAMMA_NODE_NUMBER]={
        0,1,1,2,3,5,8,10,14,17,21,26,30,36,41,47,54,61,68,75,83,92,100,109,119,129,139,150,161,173,184,196,209,222,235,248,262,276,290,305,320,335,351,366,382,399,415,433,450,467,484,502,520,539,557,576,595,614,634,653,673,693,714,734,754,775,796,816,837,858,879,901,923,944,966,988,1010,1032,1054,1076,1098,1120,1142,1165,1188,1210,1232,1255,1278,1301,1324,1346,1369,1391,1414,1437,1460,1483,1505,1528,1551,1574,1597,1619,1642,1665,1687,1710,1732,1755,1777,1799,1822,1845,1867,1889,1911,1933,1955,1977,1999,2021,2043,2064,2086,2108,2129,2150,2172,2193,2214,2236,2256,2277,2298,2319,2340,2360,2380,2401,2421,2441,2461,2481,2501,2521,2541,2560,2580,2599,2618,2637,2656,2675,2694,2713,2732,2750,2769,2787,2805,2823,2841,2859,2877,2895,2912,2929,2947,2964,2982,2999,3015,3032,3049,3066,3082,3099,3115,3131,3147,3164,3179,3195,3211,3227,3242,3258,3273,3288,3303,3318,3333,3348,3362,3377,3392,3406,3420,3434,3448,3462,3477,3490,3504,3517,3531,3544,3558,3571,3584,3597,3611,3623,3636,3649,3662,3674,3686,3698,3711,3723,3736,3748,3759,3771,3783,3795,3806,3818,3829,3841,3852,3863,3874,3885,3896,3907,3918,3929,3939,3949,3961,3971,3981,3991,4001,4012,4022,4032,4042,4051,4061,4071,4081,4090,4095
        };
#endif

    hi_isp_gamma_mcu_priority_write(1);
    for (i=0; i<GAMMA_NODE_NUMBER; i++)
    {
        hi_gamma_rgb_mem_array_data_write(i, (HI_U32)(au16sqrt2sRGB[i]));
    }
    hi_isp_gamma_mcu_priority_write(0);

    return;
}

static HI_VOID GammaExtRegsDefault(HI_VOID)
{
    hi_ext_system_gamma_curve_type_write(HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT);
    hi_isp_ext_system_gamma_change_write(HI_ISP_EXT_SYSTEM_GAMMA_CHANGE_DEFAULT);
    
    return;
}

static HI_VOID GammaRegsDefault(HI_VOID) 
{
    return;
}

static HI_VOID GammaExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;    
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_S32 u8WDRMode;
    HI_S32 i;
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (pstSnsDft->stGamma.bValid)
    {
        hi_isp_gamma_mcu_priority_write(1);
        for (i=0; i<GAMMA_NODE_NUMBER; i++)
        {
            hi_gamma_rgb_mem_array_data_write(i, (HI_U32)pstSnsDft->stGamma.au16Gamma[i]);
        }
        hi_isp_gamma_mcu_priority_write(0);
    }
    else
    {
        if (IS_LINEAR_MODE(u8WDRMode))
        {
            /* sensor in linear mode */
            GammaInitLin();
        }
        else if (IS_WDR_MODE(u8WDRMode))
        {
            /* sensor in WDR mode */
            GammaInitWDR();                
        }
        else
        {
            /* unknow u8Mode */
        }
    }
    
    hi_isp_ext_system_gamma_change_write(HI_FALSE);
    
    return;
}

static HI_VOID GammaRegsInitialize(ISP_DEV IspDev)
{    
    /* Enable gamma */
    hi_isp_gamma_enable_write(1);
    
    return;
}

static HI_S32 GammaReadExtregs(ISP_DEV IspDev)
{
    HI_U16 i;
    ISP_GAMMA_S *pstGamma = HI_NULL;
    
    GAMMA_GET_CTX(IspDev, pstGamma);

    pstGamma->u8change = hi_isp_ext_system_gamma_change_read();
    for(i = 0; i < GAMMA_NODE_NUMBER; i++)
    {
        pstGamma->au16GammaLut[i] = hi_isp_ext_gamma_rgb_mem_array_data_read(i);
    }
    
    return 0;
}

HI_S32 ISP_GammaInit(ISP_DEV IspDev)
{
    GammaRegsDefault();
    GammaExtRegsDefault();
    GammaRegsInitialize(IspDev);
    GammaExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_GAMMA_S *pstGamma = HI_NULL;    
    GAMMA_GET_CTX(IspDev, pstGamma);
    
    GammaReadExtregs(IspDev);
    
    if(HI_TRUE == pstGamma->u8change)
    {       
        memcpy(((ISP_REG_CFG_S *)pRegCfg)->stKernelCfg.stGammaRegCfg.au16GammaLut, pstGamma->au16GammaLut, sizeof(pstGamma->au16GammaLut));

        ((ISP_REG_CFG_S *)pRegCfg)->stKernelCfg.unKey.bit1GammaCfg = 1;
      
    
        hi_isp_ext_system_gamma_change_write(0);
        pstGamma->u8change = HI_FALSE; 
    }
    else
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stKernelCfg.unKey.bit1GammaCfg = 0;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    HI_S32 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_U8 u8Mode = *((HI_U8 *)pValue);
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            if (pstSnsDft->stGamma.bValid)
            {
                hi_isp_gamma_mcu_priority_write(1);
                for (i=0; i<GAMMA_NODE_NUMBER; i++)
                {
                    hi_gamma_rgb_mem_array_data_write(i, (HI_U32)pstSnsDft->stGamma.au16Gamma[i]);
                }
                hi_isp_gamma_mcu_priority_write(0);
            }
            else
            {
                if (IS_LINEAR_MODE(u8Mode))
                {
                    /* sensor in linear mode */
                    GammaInitLin();
                }
                else if (IS_WDR_MODE(u8Mode))
                {
                    /* sensor in WDR mode */
                    GammaInitWDR();                
                }
                else
                {
                    /* unknow u8Mode */
                }            
            }

            hi_isp_ext_system_gamma_change_write(HI_FALSE);
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

