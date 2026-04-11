/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : imx326_cmos.c
  Version       : Initial Draft
  Author        : Hisilicon BVT ISP group
  Created       : 2017/04/05
  Description   : Sony IMX326 sensor driver
  History       :
  1.Date        : 2017/04/05
  Author        : 
  Modification  : Created file

******************************************************************************/

#if !defined(__IMX326_CMOS_H_)
#define __IMX326_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
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

#define IMX326_ID 326
#define HIGHER_4BITS(x) ((x & 0xf0000) >> 16)
#define HIGH_8BITS(x) ((x & 0xff00) >> 8)
#define LOW_8BITS(x)  (x & 0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

/****************************************************************************
 * extern function reference                                                *
 ****************************************************************************/

extern void imx326_init(ISP_DEV IspDev);
extern void imx326_exit(ISP_DEV IspDev);
extern void imx326_standby(ISP_DEV IspDev);
extern void imx326_restart(ISP_DEV IspDev);
extern int imx326_write_register(ISP_DEV IspDev, int addr, int data);
extern int imx326_read_register(ISP_DEV IspDev, int addr);

ISP_SNS_STATE_S             g_astImx326[ISP_MAX_DEV_NUM] = {{0}};
static ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_DEV_NUM] = {&g_astImx326[0], &g_astImx326[1]};
ISP_SNS_COMMBUS_U g_aunImx326BusInfo[ISP_MAX_DEV_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1] = { .s8I2cDev = 1}
};

extern const unsigned int imx326_i2c_addr;
extern unsigned int imx326_addr_byte;
extern unsigned int imx326_data_byte;

HI_U32 g_au32InitExposure[ISP_MAX_DEV_NUM]  = {0};
HI_U32 g_au32LinesPer500ms[ISP_MAX_DEV_NUM] = {0};
HI_U16 g_au16InitWBGain[ISP_MAX_DEV_NUM][3] = {{0}};
HI_U16 g_au16SampleRgain[ISP_MAX_DEV_NUM] = {0};
HI_U16 g_au16SampleBgain[ISP_MAX_DEV_NUM] = {0};

#define IMX326_FULL_LINES_MAX  (0xFFFFF)
#define IMX326_SHR_MAX  (0xFFFF)
#define IMX326_SHR_L    (0x300C)
#define IMX326_SHR_H    (0x300D)
#define IMX326_SVR_L    (0x300E)
#define IMX326_SVR_H    (0x300F)
#define IMX326_PGC_L    (0x300A)
#define IMX326_PGC_H    (0x300B)
#define IMX326_DGAIN    (0x3012)
#define IMX326_VMAX_L   (0x30F8)
#define IMX326_VMAX_M   (0x30F9)
#define IMX326_VMAX_H   (0x30FA)

#define IMX326_INCREASE_LINES (1)
#define IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR  (0x12C0+IMX326_INCREASE_LINES)
#define IMX326_VMAX_2592x1944_5M_30FPS_12BIT_LINEAR  (0x12C0+IMX326_INCREASE_LINES)
#define IMX326_VMAX_2560x1440_4M_30FPS_12BIT_LINEAR  (0x12C0+IMX326_INCREASE_LINES)
#define IMX326_VMAX_1920x1080_2M_30FPS_12BIT_LINEAR  (0x12C0+IMX326_INCREASE_LINES)

#define IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE (1)
#define IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE (2)
#define IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE (3)
#define IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE (4)
#define IMX326_RES_IS_3072x1728_5M30fps(w, h)    (w <= 3072 && h <= 1728)
#define IMX326_RES_IS_2592x1944_5M30fps(w, h)    (w <= 2592 && h <= 1944)
#define IMX326_RES_IS_2560x1440_4M30fps(w, h)    (w <= 2560 && h <= 1440)
#define IMX326_RES_IS_1920x1080_2M30fps(w, h)    (w <= 1920 && h <= 1080)

static HI_S32 cmos_get_ae_default(ISP_DEV IspDev,AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32Fll = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
    HI_FLOAT f32maxFps = 30;
    
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return -1;
    }
    
    if(IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32Fll = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
        f32maxFps = 30;
    }
    else if(IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32Fll = IMX326_VMAX_2592x1944_5M_30FPS_12BIT_LINEAR;
        f32maxFps = 30;
    }
    else if(IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32Fll = IMX326_VMAX_2560x1440_4M_30FPS_12BIT_LINEAR;
        f32maxFps = 30;
    }
    else if(IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32Fll = IMX326_VMAX_1920x1080_2M_30FPS_12BIT_LINEAR;
        f32maxFps = 30;
    }
    
	g_apstSnsState[IspDev]->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;

	if(g_au32InitExposure[IspDev] == 0)
	{
		pstAeSnsDft->u32InitExposure = 130000;
	}
	else
	{
		pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev];
	}

    if(g_au32LinesPer500ms[IspDev] == 0)
	{
		pstAeSnsDft->u32LinesPer500ms = ((HI_U64)(u32Fll * f32maxFps)) >> 1;
	}
	else
	{
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[IspDev];
	}

    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 12;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0.221;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.3;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.3;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 6 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch(g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:
        {
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u8AeCompensation = 0x38;

            pstAeSnsDft->u32MinIntTime = 4;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 4;

            pstAeSnsDft->u32MaxAgain = 22795;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 65536;
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            break;  
        }
    }
    return 0;
}

static HI_VOID cmos_fps_set(ISP_DEV IspDev,HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_FLOAT f32maxFps;
    HI_U32 u32Lines;

    switch (g_apstSnsState[IspDev]->u8ImgMode)                                                                                           
    {
        case IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE:
            f32maxFps = 30;
            u32Lines = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR * f32maxFps / f32Fps;
            break;
            
        case IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE:
            f32maxFps = 30;
            u32Lines = IMX326_VMAX_2592x1944_5M_30FPS_12BIT_LINEAR * f32maxFps / f32Fps;
            break;

        case IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE:
            f32maxFps = 30;
            u32Lines = IMX326_VMAX_2560x1440_4M_30FPS_12BIT_LINEAR * f32maxFps / f32Fps;
            break;

        case IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE:
            f32maxFps = 30;
            u32Lines = IMX326_VMAX_1920x1080_2M_30FPS_12BIT_LINEAR * f32maxFps / f32Fps;
            break;

        default:
            printf("Not support this Mode\n");
            return;
            break;
    }

    /* VMAX was 20bit, so limit full_lines as 0xFFFFF */
    if ((f32Fps > f32maxFps) || (u32Lines > IMX326_FULL_LINES_MAX))
    {
        printf("[%s]:[%d] Not support Fps: %f\n", __FUNCTION__, __LINE__, f32Fps);
        return;
    }

    g_apstSnsState[IspDev]->u32FLStd = u32Lines;
    g_apstSnsState[IspDev]->u32FLStd = (g_apstSnsState[IspDev]->u32FLStd > IMX326_FULL_LINES_MAX) ? IMX326_FULL_LINES_MAX : g_apstSnsState[IspDev]->u32FLStd;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd * f32Fps / 2;
	
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 12;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    g_apstSnsState[IspDev]->au32FL[0]= g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(g_apstSnsState[IspDev]->au32FL[0]);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(g_apstSnsState[IspDev]->au32FL[0]);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = HIGHER_4BITS(g_apstSnsState[IspDev]->au32FL[0]);

    return;
}

static HI_VOID cmos_slow_framerate_set(ISP_DEV IspDev,HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    g_apstSnsState[IspDev]->au32FL[0] = (u32FullLines > IMX326_FULL_LINES_MAX) ? IMX326_FULL_LINES_MAX : u32FullLines;
	pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->au32FL[0] - 12;

    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(g_apstSnsState[IspDev]->au32FL[0]);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(g_apstSnsState[IspDev]->au32FL[0]);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = HIGHER_4BITS(g_apstSnsState[IspDev]->au32FL[0]);

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(ISP_DEV IspDev,HI_U32 u32IntTime)
{
    HI_U32 u32Shr = g_apstSnsState[IspDev]->au32FL[0] - u32IntTime;
    u32Shr = MIN(u32Shr, IMX326_SHR_MAX);
    u32Shr = MIN(MAX(u32Shr, 12), g_apstSnsState[IspDev]->au32FL[0] - 4);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(u32Shr);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(u32Shr); 

    return;
}

static HI_U32 again_table[490]=
{
    1024, 1026, 1028, 1030, 1032, 1034, 1036, 1038, 1040, 1042, 1044, 1046, 1049, 1051, 1053, 1055, 1057, 1059, 1061, 1063,
    1066, 1068, 1070, 1072, 1074, 1077, 1079, 1081, 1083, 1085, 1088, 1090, 1092, 1095, 1097, 1099, 1101, 1104, 1106, 1108,
    1111, 1113, 1116, 1118, 1120, 1123, 1125, 1128, 1130, 1132, 1135, 1137, 1140, 1142, 1145, 1147, 1150, 1152, 1155, 1157,
    1160, 1163, 1165, 1168, 1170, 1173, 1176, 1178, 1181, 1183, 1186, 1189, 1192, 1194, 1197, 1200, 1202, 1205, 1208, 1211,
    1214, 1216, 1219, 1222, 1225, 1228, 1231, 1234, 1237, 1239, 1242, 1245, 1248, 1251, 1254, 1257, 1260, 1263, 1266, 1269,
    1273, 1276, 1279, 1282, 1285, 1288, 1291, 1295, 1298, 1301, 1304, 1307, 1311, 1314, 1317, 1321, 1324, 1327, 1331, 1334,
    1337, 1341, 1344, 1348, 1351, 1355, 1358, 1362, 1365, 1369, 1372, 1376, 1380, 1383, 1387, 1391, 1394, 1398, 1402, 1406,
    1409, 1413, 1417, 1421, 1425, 1429, 1432, 1436, 1440, 1444, 1448, 1452, 1456, 1460, 1464, 1469, 1473, 1477, 1481, 1485,
    1489, 1494, 1498, 1502, 1507, 1511, 1515, 1520, 1524, 1529, 1533, 1538, 1542, 1547, 1551, 1556, 1560, 1565, 1570, 1574,
    1579, 1584, 1589, 1594, 1598, 1603, 1608, 1613, 1618, 1623, 1628, 1633, 1638, 1644, 1649, 1654, 1659, 1664, 1670, 1675,
    1680, 1686, 1691, 1697, 1702, 1708, 1713, 1719, 1725, 1730, 1736, 1742, 1748, 1753, 1759, 1765, 1771, 1777, 1783, 1789,
    1796, 1802, 1808, 1814, 1820, 1827, 1833, 1840, 1846, 1853, 1859, 1866, 1872, 1879, 1886, 1893, 1900, 1907, 1913, 1920,
    1928, 1935, 1942, 1949, 1956, 1964, 1971, 1978, 1986, 1993, 2001, 2009, 2016, 2024, 2032, 2040, 2048, 2056, 2064, 2072,
    2081, 2089, 2097, 2106, 2114, 2123, 2131, 2140, 2149, 2158, 2166, 2175, 2185, 2194, 2203, 2212, 2222, 2231, 2241, 2250,
    2260, 2270, 2280, 2289, 2300, 2310, 2320, 2330, 2341, 2351, 2362, 2372, 2383, 2394, 2405, 2416, 2427, 2439, 2450, 2461,
    2473, 2485, 2497, 2509, 2521, 2533, 2545, 2558, 2570, 2583, 2595, 2608, 2621, 2635, 2648, 2661, 2675, 2689, 2703, 2717,
    2731, 2745, 2759, 2774, 2789, 2804, 2819, 2834, 2849, 2865, 2881, 2897, 2913, 2929, 2945, 2962, 2979, 2996, 3013, 3031,
    3048, 3066, 3084, 3102, 3121, 3139, 3158, 3178, 3197, 3216, 3236, 3256, 3277, 3297, 3318, 3339, 3361, 3383, 3404, 3427,
    3449, 3472, 3495, 3519, 3542, 3567, 3591, 3616, 3641, 3666, 3692, 3718, 3745, 3772, 3799, 3827, 3855, 3884, 3913, 3942,
    3972, 4002, 4033, 4064, 4096, 4128, 4161, 4194, 4228, 4263, 4297, 4333, 4369, 4406, 4443, 4481, 4520, 4559, 4599, 4640,
    4681, 4723, 4766, 4810, 4855, 4900, 4946, 4993, 5041, 5090, 5140, 5191, 5243, 5296, 5350, 5405, 5461, 5519, 5578, 5638,
    5699, 5761, 5825, 5891, 5958, 6026, 6096, 6168, 6242, 6317, 6394, 6473, 6554, 6637, 6722, 6809, 6899, 6991, 7085, 7182,
    7282, 7384, 7490, 7598, 7710, 7825, 7944, 8066, 8192, 8322, 8456, 8595, 8738, 8886, 9039, 9198, 9362, 9533, 9709, 9892,
    10082, 10280, 10486, 10700, 10923, 11155, 11398, 11651, 11916, 12193, 12483, 12788, 13107, 13443, 13797, 14170, 14564,
    14980, 15420, 15888, 16384, 16913, 17476, 18079, 18725, 19418, 20165, 20972, 21845, 22795
};

static HI_VOID cmos_again_calc_table(ISP_DEV IspDev,HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    if((HI_NULL == pu32AgainLin) ||(HI_NULL == pu32AgainDb))
    {
        printf("null pointer when get ae sensor gain info  value!\n");
        return;
    }

    if (*pu32AgainLin >= again_table[489])
    {
         *pu32AgainLin = again_table[489];
         *pu32AgainDb = 489;
         goto calc_table_end;
    }

    for (i = 1; i < 490; i++)
    {
        if (*pu32AgainLin < again_table[i])
        {
            *pu32AgainLin = again_table[i - 1];
            *pu32AgainDb = i - 1;
            goto calc_table_end;
        }
    }

calc_table_end:

    *pu32AgainDb <<= 2;

    return;
}

static HI_U32 dgain_table[7]=
{
    1024,2048,4096,8192,16384,32768,65536
};

static HI_VOID cmos_dgain_calc_table(ISP_DEV IspDev,HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    int i;

    if((HI_NULL == pu32DgainLin) ||(HI_NULL == pu32DgainDb))
    {
        printf("null pointer when get ae sensor gain info value!\n");
        return;
    }

    if (*pu32DgainLin >= dgain_table[6])
    {
         *pu32DgainLin = dgain_table[6];
         *pu32DgainDb = 6;
         return ;
    }

    for (i = 1; i < 7; i++)
    {
        if (*pu32DgainLin < dgain_table[i])
        {
            *pu32DgainLin = dgain_table[i - 1];
            *pu32DgainDb = i - 1;
            break;
        }
    }

    return;
}

static HI_VOID cmos_gains_update(ISP_DEV IspDev,HI_U32 u32Again, HI_U32 u32Dgain)
{
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = (u32Again & 0xFF);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32Data = ((u32Again >> 8) & 0x00FF);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32Data = (u32Dgain & 0x7);

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
    pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;

    return 0;
}

#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_R_R (0x01C5)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_R_G (0x80D0)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_R_B (0x000B)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_G_R (0x805C)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_G_G (0x0195)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_G_B (0x8039)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_B_R (0x8004)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_B_G (0x80ED)
#define MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_B_B (0x01F1)

#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_R_R (0x01D2)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_R_G (0x80CE)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_R_B (0x8004)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_G_R (0x8075)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_G_G (0x0184)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_G_B (0x800F)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_B_R (0x001E)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_B_G (0x810B)
#define MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_B_B (0x01ED)

#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_R_R (0x021E)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_R_G (0x8131)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_R_B (0x0013)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_G_R (0x80AC)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_G_G (0x01BB)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_G_B (0x800F)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_B_R (0x001F)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_B_G (0x8232)
#define MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_B_B (0x0313)

static AWB_CCM_S g_stAwbCcm =
{
    5120,
    {
        MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_R_R, MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_R_G, MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_R_B,
        MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_G_R, MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_G_G, MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_G_B,
        MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_B_R, MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_B_G, MT_ABSOLUTE_LS_H_CALIBRATE_CCM_LINEAR_B_B
    },

    3633,
    {
        MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_R_R, MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_R_G, MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_R_B,
        MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_G_R, MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_G_G, MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_G_B,
        MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_B_R, MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_B_G, MT_ABSOLUTE_LS_M_CALIBRATE_CCM_LINEAR_B_B
    },

    2465,
    {
        MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_R_R, MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_R_G, MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_R_B,
        MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_G_R, MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_G_G, MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_G_B,
        MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_B_R, MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_B_G, MT_ABSOLUTE_LS_L_CALIBRATE_CCM_LINEAR_B_B
    }
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /* saturation */
    {0x80,0x80,0x78,0x74,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38}
};

#define CALIBRATE_STATIC_WB_R_GAIN (0x206)
#define CALIBRATE_STATIC_WB_GR_GAIN (0x100)
#define CALIBRATE_STATIC_WB_GB_GAIN (0x100)
#define CALIBRATE_STATIC_WB_B_GAIN (0x1C6)

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 (-41)
#define CALIBRATE_AWB_P2 (297)
#define CALIBRATE_AWB_Q1 (0)
#define CALIBRATE_AWB_A1 (155887)
#define CALIBRATE_AWB_B1 (128)
#define CALIBRATE_AWB_C1 (-106021)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0x01D1
#define GOLDEN_BGAIN 0x01F6

static HI_S32 cmos_get_awb_default(ISP_DEV IspDev,AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    if (HI_NULL == pstAwbSnsDft)
    {
        printf("null pointer when get awb default value!\n");
        return -1;
    }

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 5120;

    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN;
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;

    pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;
    pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;
    pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1;
    pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1;
    pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1;
    pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1;

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;
    
    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
    memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));

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
    {20, 20, 20, 38, 38, 38, 34, 34, 32, 32, 32, 32, 32, 32, 32, 32},
    /* sharpen_alt_ud to Sharpen */
    {22, 20, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 16, 16},
    /* demosaic_lum_thresh to Sharpen */
    {0x50,0x50,0x4c,0x42,0x38,0x30,0x28,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20},
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
    {0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     1,     1,     1,     1,     1,     1},
    /* TextureSt */
    {30, 28, 25, 20, 20, 26, 24, 24, 22, 20, 20, 18, 18, 12, 12, 12},
    /* EdgeSt */     
    {40, 40, 38, 36, 34, 32, 30, 28, 20, 20, 16, 16, 16, 16, 16, 16},
    /* OverShoot */
    {2, 2, 10, 20, 30, 50, 80, 80, 70, 60, 50, 40, 30, 24, 24, 24},
    /* UnderShoot */
    {20, 30, 50, 70, 90, 120, 120, 100, 80, 50, 30, 26, 26, 24, 24, 24},
    /* TextureThd */
    {10,   16,    20,    32,    36,    40,    48,    56,    64,   128,   156,    156,    156,    160,    160,   160},
    /* EdgeThd */
    {0,     0,     0,    10,    10,    10,    16,    32,    64,   128,   156,    156,    156,    160,    160,   160},
    /* JagCtrl */
    {16,   14,    12,    10,     8,     6,     4,     4,     4,     4,     4,      4,      2,      2,      2,     2},
    /* SaltCtrl */
    {50,   50,    50,    90,    90,    90,    90,    90,    90,    90,     90,    90,     90,     50,     50,    50},
    /* PepperCtrl */
    {0,     0,      0,     20,     60,     60,     60,     80,    120,    160,    180,     180,   180,     180,    180,   180},
    /* DetailCtrl */
    {245,  240,   230,    220,    210,    200,    200,    180,    180,    180,    180,     160,    160,      100,     50,    50},
    /* LumaThd */
    {
        {20,    20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20,     20},
        {40,    40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40,     40},
        {65,    65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65,     65},
        {90,    90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90}
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
    {
        0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,  0xC,  0x11, 0x15, 0x17, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 
        0x21, 0x22, 0x23, 0x23, 0x24, 0x25, 0x26, 0x26, 0x27, 0x27, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2A, 
        0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 
        0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 
        0x34, 0x34, 0x34, 0x34, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
        0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39,
        0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3B, 0x3B, 
        0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 
    },

    /* demosaic_weight_lut */
    {
        0x3,  0xC,  0x11, 0x15, 0x17, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 0x21, 0x22, 0x23, 0x23, 0x24, 0x25, 
        0x26, 0x26, 0x27, 0x27, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 
        0x2D, 0x2E, 0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 
        0x32, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x34, 0x34, 0x35, 0x35, 
        0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 
        0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 
        0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 0x3B, 
        0x3B, 0x3B, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 
    }
};

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaic =
{
    /* bvalid */
    1,
    /*vh_slope*/
    0xac,
    /*aa_slope*/
    0xa3,
    /*va_slope*/
    0xa0,
    /*uu_slope*/
    140,
    /*sat_slope*/
    0x5d,
    /*ac_slope*/
    0xa0,
    /*fc_slope*/
    0x80,
    /*vh_thresh*/
    0x0,
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
    {
        0,76,151,224,281,335,425,465,509,559,612,668,725,783,842,899,954,988,1023,1058,1093,
        1128,1164,1200,1235,1271,1306,1340,1374,1408,1440,1472,1503,1533,1562,1591,1620,1648,
        1675,1702,1728,1755,1780,1805,1830,1855,1879,1902,1925,1948,1970,1992,2014,2035,2056,
        2077,2097,2116,2136,2155,2174,2192,2210,2228,2246,2263,2280,2296,2313,2329,2344,2360,
        2375,2390,2405,2420,2434,2448,2462,2475,2489,2502,2515,2528,2541,2553,2566,2578,2590,
        2602,2614,2625,2637,2648,2659,2670,2681,2692,2703,2714,2724,2735,2745,2756,2766,2776,
        2786,2796,2806,2816,2826,2836,2846,2856,2865,2875,2885,2894,2904,2914,2923,2933,2942,
        2952,2961,2971,2980,2990,2999,3009,3018,3028,3037,3046,3056,3065,3074,3084,3093,3103,
        3112,3121,3130,3140,3149,3158,3168,3177,3186,3195,3204,3214,3223,3232,3241,3250,3259,
        3268,3277,3285,3294,3303,3312,3320,3329,3338,3346,3355,3363,3371,3380,3388,3396,3404,
        3412,3420,3427,3435,3443,3450,3458,3465,3472,3481,3490,3499,3508,3517,3525,3534,3543,
        3552,3561,3570,3578,3587,3596,3605,3613,3622,3631,3640,3648,3657,3666,3674,3683,3692,
        3700,3709,3718,3726,3735,3743,3752,3761,3769,3778,3786,3795,3803,3812,3820,3829,3837,
        3846,3854,3863,3871,3879,3888,3896,3905,3913,3921,3930,3938,3947,3955,3963,3972,3980,
        3988,3996,4005,4013,4021,4030,4038,4046,4054,4063,4071,4079,4087,4094,4095
    }
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
            
            memcpy(&pstDef->stAgcTbl, &g_stIspAgcTable, sizeof(ISP_CMOS_AGC_TABLE_S));
            memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTable, sizeof(ISP_CMOS_NOISE_TABLE_S));            
            memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stGamma, &g_stIspGamma, sizeof(ISP_CMOS_GAMMA_S));
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpen, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
        break;
    }
    pstDef->stSensorMaxResolution.u32MaxWidth  = 3100;
    pstDef->stSensorMaxResolution.u32MaxHeight = 2000;
    pstDef->stSensorMode.u32SensorID = IMX326_ID;
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
    for (i=0; i<4; i++)
    {
        pstBlackLevel->au16BlackLevel[i] = 0xc8;/*10bit : 0x3c*/
    }
    return 0;    
}

static HI_VOID cmos_set_pixel_detect(ISP_DEV IspDev,HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
    
    if(IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32FullLines_5Fps = (IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR * 30) / 5;
    }
    else if(IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32FullLines_5Fps = (IMX326_VMAX_2592x1944_5M_30FPS_12BIT_LINEAR * 30) / 5;
    }
    else if(IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32FullLines_5Fps = (IMX326_VMAX_2560x1440_4M_30FPS_12BIT_LINEAR * 30) / 5;
    }
    else if(IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
    {
        u32FullLines_5Fps = (IMX326_VMAX_1920x1080_2M_30FPS_12BIT_LINEAR * 30) / 5;
    }
    
    /* Detect set 5fps */
    if (bEnable)
    {
        /* Analog and Digital gains both must be programmed for their minimum values */
        imx326_write_register (IspDev,IMX326_VMAX_L, u32FullLines_5Fps & 0xFF);
        imx326_write_register (IspDev,IMX326_VMAX_M, (u32FullLines_5Fps & 0xFF00) >> 8);
        imx326_write_register (IspDev,IMX326_VMAX_H, (u32FullLines_5Fps & 0xF0000) >> 16);
        
        imx326_write_register (IspDev,IMX326_SHR_L, 0xC);
        imx326_write_register (IspDev,IMX326_SHR_H, 0x0);
        imx326_write_register (IspDev,IMX326_PGC_L, 0x0);
        imx326_write_register (IspDev,IMX326_PGC_H, 0x0);
        imx326_write_register (IspDev,IMX326_DGAIN, 0x0);
    }
    else /* setup for ISP 'normal mode' */
    {
        imx326_write_register(IspDev,IMX326_VMAX_L, (g_apstSnsState[IspDev]->u32FLStd & 0xff)); 
        imx326_write_register(IspDev,IMX326_VMAX_M, (g_apstSnsState[IspDev]->u32FLStd & 0xff00)>>8);
        imx326_write_register(IspDev,IMX326_VMAX_H,  (g_apstSnsState[IspDev]->u32FLStd & 0xf0000)>>16);
        g_apstSnsState[IspDev]->bSyncInit= HI_FALSE;
    }
    return;
}

static HI_VOID cmos_set_wdr_mode(ISP_DEV IspDev,HI_U8 u8Mode)
{
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    
    switch(u8Mode)
    {
        case WDR_MODE_NONE:
            if(IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
            {
                g_apstSnsState[IspDev]->u32FLStd = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
                g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            }
            else if(IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
            {
                g_apstSnsState[IspDev]->u32FLStd = IMX326_VMAX_2592x1944_5M_30FPS_12BIT_LINEAR;
                g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            }
            else if(IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
            {
                g_apstSnsState[IspDev]->u32FLStd = IMX326_VMAX_2560x1440_4M_30FPS_12BIT_LINEAR;
                g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            }
            else if(IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
            {
                g_apstSnsState[IspDev]->u32FLStd = IMX326_VMAX_1920x1080_2M_30FPS_12BIT_LINEAR;
                g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            }
            printf("linear mode\n");
        break;
        
        default:
            printf("[%s]:[%d] NOT support this mode!\n", __FUNCTION__, __LINE__);
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
        g_apstSnsState[IspDev]->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx326BusInfo[IspDev].s8I2cDev;        
        g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum = 10;

        for (i=0; i<g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u8DevAddr = imx326_i2c_addr;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32AddrByteNum = imx326_addr_byte;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32DataByteNum = imx326_data_byte;
        }

        //SHR registers
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX326_SHR_L;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX326_SHR_H;

        // gain related
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX326_PGC_L;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX326_PGC_H;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX326_DGAIN;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1;

        //VMAX registers
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX326_VMAX_L;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX326_VMAX_M;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX326_VMAX_H;

        //svr registers
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX326_SVR_L;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32Data = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX326_SVR_H;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32Data = 0;       
        
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

    memcpy(pstSnsRegsInfo, &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S)); 
    memcpy(&g_apstSnsState[IspDev]->astRegsInfo[1], &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S)); 
    g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];

    return 0;
}

static HI_S32 cmos_set_image_mode(ISP_DEV IspDev,ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = g_apstSnsState[IspDev]->u8ImgMode;
    HI_U32 u32W, u32H;
    
    g_apstSnsState[IspDev]->bSyncInit= HI_FALSE;    

        
    if (HI_NULL == pstSensorImageMode )
    {
        printf("null pointer when set image mode\n");
        return -1;
    }

    u32H = pstSensorImageMode->u16Height;
    u32W = pstSensorImageMode->u16Width;
    if(IMX326_RES_IS_1920x1080_2M30fps(u32W, u32H))
    {
        u8SensorImageMode = IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE;
    }
    else if(IMX326_RES_IS_2560x1440_4M30fps(u32W, u32H))
    {
        u8SensorImageMode = IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE;
    }
    else if ( IMX326_RES_IS_3072x1728_5M30fps(u32W, u32H))
    {
        u8SensorImageMode = IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE;
    }
    else if ( IMX326_RES_IS_2592x1944_5M30fps(u32W, u32H))
    {
        u8SensorImageMode = IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE;
    }
    else
    {
        printf("Not Support This Mode\n");
        return -1;
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
    g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
    g_apstSnsState[IspDev]->bInit = HI_FALSE;
    g_apstSnsState[IspDev]->bSyncInit= HI_FALSE;

    g_apstSnsState[IspDev]->u32FLStd= IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
    g_apstSnsState[IspDev]->u8ImgMode = IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE;
    g_apstSnsState[IspDev]->au32FL[0] = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
    g_apstSnsState[IspDev]->au32FL[1] = IMX326_VMAX_3072x1728_5M_30FPS_12BIT_LINEAR;
    
    memset(&g_apstSnsState[IspDev]->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_apstSnsState[IspDev]->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init                  = imx326_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit                  = imx326_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init           = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode               = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode                 = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default              = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level          = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect             = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info             = cmos_get_sns_regs_info;

    return 0;
}

static int imx326_set_bus_info(ISP_DEV IspDev, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx326BusInfo[IspDev].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return 0;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX326_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }
    
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, pstAeLib, IMX326_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, pstAwbLib, IMX326_ID, &stAwbRegister);
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, IMX326_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, pstAeLib, IMX326_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, pstAwbLib, IMX326_ID);
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

ISP_SNS_OBJ_S stSnsImx326Obj = 
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx326_standby,
    .pfnRestart             = imx326_restart,
    .pfnWriteReg            = imx326_write_register,
    .pfnReadReg             = imx326_read_register,
    .pfnSetBusInfo          = imx326_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif 


