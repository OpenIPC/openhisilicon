#if !defined(__IMX274_CMOS_H_)
#define __IMX274_CMOS_H_

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

#include "imx274_slave_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define IMX274_ID 274

#define IMX274_VMAX_4K_30FPS_DOL_WDR 2284
#define IMX274_VMAX_4K_30FPS_LINEAR  4550


const IMX274_VIDEO_MODE_TBL_S g_astImx274ModeTbl[IMX274_MODE_BUTT] = {
    
    {72000000, 528,  0x24A860, 4550, 30, "4K2K_6CH_12BIT_29.97FPS"},
    {72000000, 1052, 0x24A9D0, 2284, 30, "4K2K_DOL_10CH_10BIT_29.97FPS"},
};

ISP_SLAVE_SNS_SYNC_S gstImx274Sync[ISP_MAX_DEV_NUM];
static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_DEV_NUM] = {ISP_FSWDR_NORMAL_MODE,ISP_FSWDR_NORMAL_MODE};

/****************************************************************************
 * extern function reference                                                *
 ****************************************************************************/

extern void imx274_init(ISP_DEV IspDev);
extern void imx274_exit(ISP_DEV IspDev);
extern void imx274_standby(ISP_DEV IspDev);
extern void imx274_restart(ISP_DEV IspDev);
extern int imx274_write_register(ISP_DEV IspDev, int addr, int data);
extern int imx274_read_register(ISP_DEV IspDev, int addr);


ISP_SNS_STATE_S             g_astImx274[ISP_MAX_DEV_NUM] = {{0}};
static ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_DEV_NUM] = {&g_astImx274[0], &g_astImx274[1]};
ISP_SNS_COMMBUS_U g_aunImx274BusInfo[ISP_MAX_DEV_NUM] = {
    [0] = {.s8SspDev = { .bit4SspDev = 0, .bit4SspCs = 0}},
    [1] = {.s8SspDev = { .bit4SspDev = 1, .bit4SspCs = 0}}
}; 

static HI_U32 g_au32InitExposure[ISP_MAX_DEV_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_DEV_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_DEV_NUM] = {0};

typedef struct hiIMX274_STATE_S
{
    HI_U32  u32DgainVal;
    HI_U32  u32RHS1_LocRLimit;
} IMX274_STATE_S;

IMX274_STATE_S g_astimx274State[ISP_MAX_DEV_NUM] = {{0}};

/* Piris attr: Forecam NV03105P */
static ISP_PIRIS_ATTR_S g_astPirisAttr[ISP_MAX_DEV_NUM] =
{
    {
        0,      // bStepFNOTableChange
        1,      // bZeroIsMax
        94,     // u16TotalStep
        62,     // u16StepCount
        /* Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1 */
        {30,35,40,45,50,56,61,67,73,79,85,92,98,105,112,120,127,135,143,150,158,166,174,183,191,200,208,217,225,234,243,252,261,270,279,289,298,307,316,325,335,344,353,362,372,381,390,399,408,417,426,435,444,453,462,470,478,486,493,500,506,512},
        ISP_IRIS_F_NO_1_4, // enMaxIrisFNOTarget
        ISP_IRIS_F_NO_5_6,  // enMinIrisFNOTarget
        0,                 // bFNOExValid
        512,               // u32MaxIrisFNOTarget  
        30                 // u32MinIrisFNOTarget

    },
    {
        0,      // bStepFNOTableChange
        1,      // bZeroIsMax
        94,     // u16TotalStep
        62,     // u16StepCount
        /* Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1 */
        {30,35,40,45,50,56,61,67,73,79,85,92,98,105,112,120,127,135,143,150,158,166,174,183,191,200,208,217,225,234,243,252,261,270,279,289,298,307,316,325,335,344,353,362,372,381,390,399,408,417,426,435,444,453,462,470,478,486,493,500,506,512},
        ISP_IRIS_F_NO_1_4, // enMaxIrisFNOTarget
        ISP_IRIS_F_NO_5_6,  // enMinIrisFNOTarget
        0,                 // bFNOExValid
        512,               // u32MaxIrisFNOTarget  
        30                 // u32MinIrisFNOTarget
    },
};


static HI_S32 cmos_get_ae_default(ISP_DEV IspDev,AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	
	HI_U32 u32Fll, U32MaxFps;
	
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return -1;
    }
    
    u32Fll = g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines;
    U32MaxFps = g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32MaxFps;
	g_apstSnsState[IspDev]->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;

    switch(g_apstSnsState[IspDev]->u8ImgMode) 
    {
        case IMX274_MODE0:
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.320;
            break;
        case IMX274_MODE1_DOL:
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
            break;
        default:
            printf("NOT support this mode!\n");
            break;
    }

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.3;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.3;
    
    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift; 

    memcpy(&pstAeSnsDft->stPirisAttr, &g_astPirisAttr[IspDev], sizeof(ISP_PIRIS_ATTR_S));
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
		pstAeSnsDft->u32LinesPer500ms = (u32Fll * U32MaxFps) >> 1;
	}
	else
	{
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[IspDev];
	}


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
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR; 
            
            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 12;
            pstAeSnsDft->u32MinIntTime = 4;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
            
            pstAeSnsDft->u32MaxAgain = 22795;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
            
            pstAeSnsDft->u32MaxDgain = 8192;  
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u32InitExposure = 1293046;     /*4K@30fps linear, inttime 4538 lines, gain 1024(10bits) */
            
            break;  
        }
        
        case WDR_MODE_2To1_LINE:  
        {
            pstAeSnsDft->au8HistThresh[0] = 0xC;
            pstAeSnsDft->au8HistThresh[1] = 0x18;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;
            
            pstAeSnsDft->u8AeCompensation = 0x18;  
            pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR; 
            
            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 12;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
            
            pstAeSnsDft->u32MaxAgain = 22795 << 3;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
            
            pstAeSnsDft->u32MaxDgain = 1024;  
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u32InitExposure = 41200;     /*4K@30fps, 1:16 DOL, short inttime 43 lines, gain 1024(10bits) */

            if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
            {
                pstAeSnsDft->u32MaxAgainTarget = 22795 << 3;
                pstAeSnsDft->u8AeCompensation = 56; 
                pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
            }
            else
            {
                pstAeSnsDft->u32MaxAgainTarget = 22795 << 1;
                pstAeSnsDft->u32MaxISPDgainTarget = 268;
				pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
                pstAeSnsDft->u16ManRatioEnable = 1;
                pstAeSnsDft->au32Ratio[0] = 0x400;
            }
            
            break;  
        }         
    }

    return 0;
}


static HI_VOID cmos_fps_set(ISP_DEV IspDev,HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{   
    HI_U32 u32MaxFps;
    HI_U32 u32Lines;

    u32MaxFps = g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32MaxFps;
    u32Lines = g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32VertiLines * u32MaxFps / f32Fps;  
    
    /* SHR 16bit, So limit full_lines as 0xFFFF */
    if (f32Fps > u32MaxFps || u32Lines > FULL_LINES_MAX)
    {
        printf("[%s]:[%d] Not support Fps: %f\n", __FUNCTION__, __LINE__, f32Fps);
        return;
    }
 
    switch(g_apstSnsState[IspDev]->u8ImgMode) 
    {
        case IMX274_MODE0:
                gstImx274Sync[IspDev].u32VsTime = (g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * u32MaxFps / f32Fps;
                g_apstSnsState[IspDev]->u32FLStd = u32Lines;
            break;
        case IMX274_MODE1_DOL:
                gstImx274Sync[IspDev].u32VsTime = (g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * u32MaxFps / f32Fps;
                g_apstSnsState[IspDev]->u32FLStd = u32Lines;
                g_astimx274State[IspDev].u32RHS1_LocRLimit = g_apstSnsState[IspDev]->u32FLStd - 2233;
            break;            
        default:
            printf("NOT support this mode!\n");
            return;
            break;
    }

    g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstImx274Sync[IspDev].u32VsTime;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 12;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;

    g_apstSnsState[IspDev]->au32FL[0] = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];
    
    return;
}


static HI_VOID cmos_slow_framerate_set(ISP_DEV IspDev,HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{ 
    u32FullLines = (u32FullLines > FULL_LINES_MAX) ? FULL_LINES_MAX : u32FullLines;
    g_apstSnsState[IspDev]->au32FL[0]  = u32FullLines;
   
    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];
    gstImx274Sync[IspDev].u32VsTime = g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerHs * u32FullLines;
    g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstImx274Sync[IspDev].u32VsTime;   
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->au32FL[0]  - 12;
    
    return; 
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(ISP_DEV IspDev,HI_U32 u32IntTime)
{
    static HI_BOOL bFirst = HI_TRUE;
    HI_U32 u32Shr = 0;
    static HI_U32 u32ShortIntTime;
    static HI_U32 u32LongIntTime; 
    HI_U32 u32SHS1 = 0;
    HI_U32 u32SHS2 = 0;
    HI_U32 u32RHS1 = 0; 

    if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        if(bFirst)
        {
            g_apstSnsState[IspDev]->au32WDRIntTime[0] = u32IntTime;
            u32ShortIntTime = u32IntTime;
            bFirst = HI_FALSE;
        }
        else
        {
            g_apstSnsState[IspDev]->au32WDRIntTime[1] = u32IntTime;
            u32LongIntTime = u32IntTime;    
            /* allocate the SHS1 ,SHS2 */ 
            u32SHS2 = g_apstSnsState[IspDev]->au32FL[1]  - u32LongIntTime - 1; 
            u32SHS1 = 6;
            u32RHS1 = u32SHS1 + u32ShortIntTime + 1;
            
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[0].u32Data = LOW_8BITS(u32SHS1);
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[1].u32Data = HIGH_8BITS(u32SHS1);
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[5].u32Data = LOW_8BITS(u32SHS2);
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[6].u32Data = HIGH_8BITS(u32SHS2);
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[7].u32Data = LOW_8BITS(u32RHS1);
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[8].u32Data = HIGH_8BITS(u32RHS1);                      
            
            bFirst = HI_TRUE;
        }
    }
    else
    {
        u32Shr = g_apstSnsState[IspDev]->au32FL[0] - u32IntTime;
        u32Shr = MIN(u32Shr, 0xffff);
        u32Shr = MIN(MAX(u32Shr, 12), g_apstSnsState[IspDev]->au32FL[0] - 4);
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[0].u32Data = LOW_8BITS(u32Shr);
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[1].u32Data = HIGH_8BITS(u32Shr);
    }
    
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
    int i,j;
    HI_U32 u32AgainLinTmp = 0;

    if((HI_NULL == pu32AgainLin) ||(HI_NULL == pu32AgainDb))
    {
        printf("null pointer when get ae sensor gain info  value!\n");
        return;
    }

    if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)  
    {        
        j = 3;
        while((*pu32AgainLin >> j) <= again_table[489])
        {
            g_astimx274State[IspDev].u32DgainVal = j;
            if(j == 0)
            {
                break;
            }
            j--; 
        }

        u32AgainLinTmp = *pu32AgainLin >> g_astimx274State[IspDev].u32DgainVal;            
    }
    else
    {
        g_astimx274State[IspDev].u32DgainVal = 0;
        u32AgainLinTmp = *pu32AgainLin;
    }


#if 1 
    if (u32AgainLinTmp >= again_table[489])
    {
         u32AgainLinTmp = again_table[489];
         *pu32AgainDb = 489;
         goto calc_table_end;
    }
    
    for (i = 1; i < 490; i++)
    {
        if (u32AgainLinTmp < again_table[i])
        {
            u32AgainLinTmp = again_table[i - 1];
            *pu32AgainDb = i - 1;
            goto calc_table_end;;
        }
    }
#else
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
            goto calc_table_end;;
        }
    }
#endif

calc_table_end:

    // downsample require
    *pu32AgainLin = u32AgainLinTmp << g_astimx274State[IspDev].u32DgainVal;
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
    if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[2].u32Data = (u32Again & 0xFF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[3].u32Data = ((u32Again >> 8) & 0x00FF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[4].u32Data = (g_astimx274State[IspDev].u32DgainVal & 0x0F);
    }
    else
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[2].u32Data = (u32Again & 0xFF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[3].u32Data = ((u32Again >> 8) & 0x00FF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[4].u32Data = (u32Dgain & 0x0F);
    }

    return;
}

static HI_VOID cmos_get_inttime_max(ISP_DEV IspDev,HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32RatioTmp = 0x40;
    HI_U32 u32ShortIntTimeMax = 2; 
    HI_U32 u32LongIntTimeMax = 2;
    HI_U32 u32IntTimeMaxTmp = 0;

    if((HI_NULL == au32Ratio) || (HI_NULL == au32IntTimeMax) || (HI_NULL == au32IntTimeMin))
    {
        printf("Null pointer when get ae sensor ExpRatio/IntTimeMax/IntTimeMin value!\n");
        return;
    }

    /* Electronic shutter timing of short exposure (SEF) 6 <= SHR_DOL1 <= RHS1 - 2 */
    /* Readout timing of short exposure (SEF)            SHR_DOL1 + 2 <= RHS1 <= VMAX - 2233 */
    /* Electronic shutter timing of long exposure (LEF)  RHS1 + 6 <= SHR_DOL2 <= VMAX */
    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
        {
            u32IntTimeMaxTmp = g_apstSnsState[IspDev]->au32FL[1] - 16 - g_apstSnsState[IspDev]->au32WDRIntTime[0];
            u32ShortIntTimeMax = 2;
            u32LongIntTimeMax = g_apstSnsState[IspDev]->au32FL[0] - 20;
            u32LongIntTimeMax = (u32IntTimeMaxTmp < u32LongIntTimeMax) ? u32IntTimeMaxTmp : u32LongIntTimeMax;
            au32IntTimeMax[0] = u32LongIntTimeMax;
            au32IntTimeMin[0] = u32ShortIntTimeMax;
            return;
        }
        else
        {
            u32IntTimeMaxTmp = ((g_apstSnsState[IspDev]->au32FL[1] - 16 - g_apstSnsState[IspDev]->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
            u32ShortIntTimeMax = ((g_apstSnsState[IspDev]->au32FL[0] - 16) << 6) / (au32Ratio[0] + 0x40);
            u32ShortIntTimeMax = (u32IntTimeMaxTmp < u32ShortIntTimeMax) ? u32IntTimeMaxTmp : u32ShortIntTimeMax;
            u32ShortIntTimeMax = (u32ShortIntTimeMax > (g_astimx274State[IspDev].u32RHS1_LocRLimit - 8))? (g_astimx274State[IspDev].u32RHS1_LocRLimit - 8) : u32ShortIntTimeMax;
        }
    }

    if(u32ShortIntTimeMax >= 2)
    {
        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
        {
            u32LongIntTimeMax = u32ShortIntTimeMax * au32Ratio[0] >> 6; 
            au32IntTimeMax[0] = u32ShortIntTimeMax;
            au32IntTimeMax[1] = u32LongIntTimeMax;
            au32IntTimeMin[0] = 2;
            au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >> 6;   
        }

    }
    else
    {
        if(1 == u16ManRatioEnable)
        {
            printf("Manaul ExpRatio is too large!\n");
            return;
        }
        else
        {
            u32ShortIntTimeMax = 2; 
            if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
            {
                u32RatioTmp = 0xFFF;
                u32LongIntTimeMax = u32ShortIntTimeMax * u32RatioTmp >> 6; 
                au32IntTimeMax[0] = u32ShortIntTimeMax;
                au32IntTimeMax[1] = u32LongIntTimeMax;
                au32IntTimeMin[0] = 2;
                au32IntTimeMin[1] = au32IntTimeMin[0] * u32RatioTmp >> 6;
            }
        }
    }

    return;
}

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(ISP_DEV IspDev, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    genFSWDRMode[IspDev] = pstAeFSWDRAttr->enFSWDRMode;
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
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;  

    return 0;
}


static AWB_CCM_S g_stAwbCcm =
{
#if 0    
    5100,
    {      
        0x01C6, 0x80BD, 0x8009,
        0x8058, 0x01AB, 0x8053,
        0x0005, 0x80AC, 0x01A7     
    },
    
    3600,
    {
        0x01CF, 0x80AF, 0x8020,
        0x8051, 0x0197, 0x8046,
        0x0015, 0x80C0, 0x01AB,        
    }, 

    2500,
    {     
        0x01BD, 0x8097, 0x8026,
        0x8088, 0x01AE, 0x8026,
        0x0003, 0x8129, 0x0226   
    }
#else
    5100,
    {      
        0x01AB, 0x80A4, 0x8007,
        0x804B, 0x0182, 0x8037,
        0x8006, 0x80A4, 0x01AA   
    },

    3600,
    {
        0x018E, 0x807A, 0x8014,
        0x8066, 0x019C, 0x8036,
        0x0002, 0x80A2, 0x01A0,       
    }, 

    2500,
    {     
        0x01B0, 0x80A1, 0x800F,
        0x807D, 0x018B, 0x800E,
        0x0010, 0x81C0, 0x02B0        
    }
#endif
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,

    /* saturation */ 
    {0x80,0x80,0x78,0x74,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38}
};

static AWB_AGC_TABLE_S g_stAwbAgcTableFSWDR =
{
    /* bvalid */
    1,

    /* saturation */ 
    {0x80,0x80,0x78,0x74,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38}
};


/* Calibration results for Static WB */
#define CALIBRATE_STATIC_WB_R_GAIN 0x226 
#define CALIBRATE_STATIC_WB_GR_GAIN 0x100 
#define CALIBRATE_STATIC_WB_GB_GAIN 0x100 
#define CALIBRATE_STATIC_WB_B_GAIN 0x1B8 
/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 -27  
#define CALIBRATE_AWB_P2 283 
#define CALIBRATE_AWB_Q1 0 
#define CALIBRATE_AWB_A1 165329  
#define CALIBRATE_AWB_B1 128  
#define CALIBRATE_AWB_C1 -118136 
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
    pstAwbSnsDft->u16WbRefTemp = 4900;

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

    switch (g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
        break;
        case WDR_MODE_2To1_LINE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
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
    {0x3a,0x38,0x36,0x34,0x32,0x30,0x30,0x30,0x28,0x24,0x24,0x20,0x20,0x20,0x10,0x10},
        
    /* demosaic_lum_thresh to Sharpen */
    {0x50,0x50,0x4c,0x48,0x42,0x3a,0x32,0x28,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20},
        
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
     {52,    52,   52,    50,    50,    50,    46,    40,    36,    26,    20,    12,    12,     8,     8,     8},
         
     /* EdgeSt */
     {56,    56,   56,    56,    56,    56,    50,    50,    44,    44,    38,    38,    38,    20,     20,    20},      
         
     /* OverShoot */
     {64,   64,    60,    60,    56,    56,    56,    56,    56,    50,    44,    40,    40,    40,    40,    40},
        
     /* UnderShoot */
     {64,   64,    60,    60,    56,    56,    56,    56,    56,    50,    44,    40,    40,    40,    40,    40},
#if 0
     /* TextureThd */
     {10,   16,    20,    24,    32,    40,    48,    56,    64,   128,   156,    156,    156,    160,    160,   160},
        
     /* EdgeThd */
     {0,     0,     0,    10,    10,    10,    16,    32,    64,   128,   156,    156,    156,    160,    160,   160},
#else        
     /* TextureThd */
     {80,   80,    80,   80,   80,    80,   90,   90,   90,     100,    100,    100,    100,    100,    110, 110},

     /* EdgeThd */
     {0,    0,     5,    10,   10,   10,   16,   20,   30,   40,  50,    50,     60,     60,     60,   60},        
#endif    
     /* JagCtrl */
     {16,   14,    12,    10,     8,     6,     4,     4,     4,     4,     4,      4,      2,      2,      2,     2},
    
     /* SaltCtrl */
     {50,   50,    50,    90,    90,    90,    90,    90,    90,    90,     90,    90,     90,     50,     50,    50},
    
     /* PepperCtrl */
     {0,     0,      0,     20,     60,     60,     60,     80,    120,    160,    180,     180,   180,     180,    180,   180},
    
     /* DetailCtrl */
     {150,  148,   146,    144,    140,    136,    132,    130,    128,    126,    124,     122,    120,    100,     80,    80}, 
    
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

static ISP_CMOS_AGC_TABLE_S g_stIspAgcTableFSWDR =
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


static ISP_CMOS_NOISE_TABLE_S g_stIspNoiseTable =
{
    /* bvalid */
    1,

    /* nosie_profile_weight_lut */
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xA, 0xF, 0x12, 0x15, 0x17, 0x19, 0x1A, 0x1B, 0x1D, 0x1E,
        0x1F, 0x20, 0x21, 0x21, 0x22, 0x23, 0x24, 0x24, 0x24, 0x25, 0x26, 0x26, 0x27, 0x27, 0x28, 0x28,
        0x29, 0x29, 0x29, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 0x2D,
        0x2E, 0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x32, 0x32, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34,
        0x34, 0x34, 0x34, 0x34, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x38,
        0x38, 0x38, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39
    },

    /* demosaic_weight_lut */
    {
        0x3, 0xA, 0xF, 0x12, 0x15, 0x17, 0x19, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x21, 0x22,
        0x23, 0x24, 0x24, 0x24, 0x25, 0x26, 0x26, 0x27, 0x27, 0x28, 0x28, 0x29, 0x29, 0x29, 0x2A, 0x2A,
        0x2A, 0x2B, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x2E, 0x2E, 0x2F,
        0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x35,
        0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x37,
        0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
        0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39
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
        0x3, 0xA, 0xF, 0x12, 0x15, 0x17, 0x19, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x21, 0x22,
        0x23, 0x24, 0x24, 0x24, 0x25, 0x26, 0x26, 0x27, 0x27, 0x28, 0x28, 0x29, 0x29, 0x29, 0x2A, 0x2A,
        0x2A, 0x2B, 0x2B, 0x2B, 0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x2E, 0x2E, 0x2F,
        0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x35,
        0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x37,
        0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
        0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39 
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
    0x80,

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

static ISP_CMOS_DEMOSAIC_S g_stIspDemosaicFSWDR =
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
    0x80,

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
    0x00,

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
        0, 180, 320, 426, 516, 590, 660, 730, 786, 844, 896, 946, 994, 1040, 1090, 1130, 1170, 1210, 1248,
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
        3994, 4002, 4006, 4010, 4018, 4022, 4032, 4038, 4046, 4050, 4056, 4062, 4072, 4076, 4084, 4090, 4095
    }
};

static ISP_CMOS_GAMMA_S g_stIspGammaFSWDR =
{
    /* bvalid */
    1,
    
    {0   ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,    
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
    3981,3989,3996,4003,4010,4018,4025,4032,4039,4046,4054,4061,4068,4075,4082,4089,4095}
};

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam =
{
    {378, 256, 430},
    {439, 256, 439}
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
            pstDef->stDrc.u32VarianceIntensity  = 0x02;
            pstDef->stDrc.u32Asymmetry          = 0x08;
            pstDef->stDrc.u32BrightEnhance      = 0xE6;
            pstDef->stDrc.bFilterMux            = 0x1;
            pstDef->stDrc.u32Svariance          = 0x8;
            pstDef->stDrc.u32BrightPr           = 0xA0;
            pstDef->stDrc.u32Contrast           = 0xA0;
            pstDef->stDrc.u32DarkEnhance        = 0x8000;
            
            memcpy(&pstDef->stAgcTbl, &g_stIspAgcTable, sizeof(ISP_CMOS_AGC_TABLE_S));
            memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTable, sizeof(ISP_CMOS_NOISE_TABLE_S));            
            memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stGamma, &g_stIspGamma, sizeof(ISP_CMOS_GAMMA_S));
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpen, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
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
            pstDef->stDrc.u32BrightEnhance      = 0xE6;     // if bFilterMux = 0x0, use 0xc8
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
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpen, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
            memcpy(&pstDef->stGamma, &g_stIspGammaFSWDR, sizeof(ISP_CMOS_GAMMA_S));
             
        break;        
    }

    pstDef->stSensorMaxResolution.u32MaxWidth  = 3840;
    pstDef->stSensorMaxResolution.u32MaxHeight = 2160;
    pstDef->stSensorMode.u32SensorID = IMX274_ID;
    pstDef->stSensorMode.u8SensorMode = g_apstSnsState[IspDev]->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));
    switch (g_apstSnsState[IspDev]->u8ImgMode)
    {
        default:
        case IMX274_MODE0:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; 
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
        break;
        
        case IMX274_MODE1_DOL:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;  
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023;
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
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
    pstDef->stSensorMode.bValidDngRawFormat = HI_TRUE;

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
    HI_U32 u32MaxFps;
    u32MaxFps = g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32MaxFps;    
    /* Detect set 5fps */
    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        return;    
    }
    
    CHECK_RET(HI_MPI_ISP_GetHvSync(IspDev, &gstImx274Sync[IspDev]));

    if (bEnable)
    {
        switch(g_apstSnsState[IspDev]->u8ImgMode) 
        {
            case IMX274_MODE0:
                gstImx274Sync[IspDev].u32VsTime = ((g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs) * u32MaxFps)/5;
                break;   
            default:
                printf("NOT support this mode for PixDetect!\n");
                break;
        } 
        
        /* Analog and Digital gains both must be programmed for their minimum values */
		imx274_write_register(IspDev,IMX274_SHR_LINEAR, 0xC);
		imx274_write_register(IspDev,IMX274_SHR_LINEAR+1, 0x0);
        
        imx274_write_register(IspDev,IMX274_PGC, 0x1);                 
        imx274_write_register(IspDev,IMX274_PGC+1, 0x0); 
        imx274_write_register(IspDev,IMX274_DGAIN, 0x0);
        
    }
    else /* setup for ISP 'normal mode' */
    {
        gstImx274Sync[IspDev].u32VsTime = (g_astImx274ModeTbl[g_apstSnsState[IspDev]->u8ImgMode].u32InckPerVs);
        g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    }
    
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstImx274Sync[IspDev]));
    
    return;
}

static HI_VOID cmos_set_wdr_mode(ISP_DEV IspDev,HI_U8 u8Mode)
{
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    
    switch(u8Mode)
    {
        case WDR_MODE_NONE:
            g_apstSnsState[IspDev]->u8ImgMode = IMX274_MODE0;
            g_apstSnsState[IspDev]->u32FLStd = IMX274_VMAX_4K_30FPS_LINEAR;
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
        break;
		
        case WDR_MODE_2To1_LINE:
			g_apstSnsState[IspDev]->u8ImgMode = IMX274_MODE1_DOL;
			g_apstSnsState[IspDev]->u32FLStd = IMX274_VMAX_4K_30FPS_DOL_WDR;
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_2To1_LINE;
            printf("2to1 line WDR mode\n");
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
        g_apstSnsState[IspDev]->astRegsInfo[0].enSnsType = ISP_SNS_SSP_TYPE;
        g_apstSnsState[IspDev]->astRegsInfo[0].unComBus.s8SspDev.bit4SspDev = g_aunImx274BusInfo[IspDev].s8SspDev.bit4SspDev;
        g_apstSnsState[IspDev]->astRegsInfo[0].unComBus.s8SspDev.bit4SspCs  = g_aunImx274BusInfo[IspDev].s8SspDev.bit4SspCs;
        g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;        
        g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum = 5;        

        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
        {
			g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
            g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum += 4; 
        }
        
        for (i=0; i < g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].bUpdate = HI_TRUE;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].u32DevAddr = 0x81;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].u32DevAddrByteNum = 1;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].u32RegAddrByteNum = 2;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].u32DataByteNum = 1;
        }
        
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[0].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[0].u32RegAddr = IMX274_SHR_LINEAR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[1].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[1].u32RegAddr = IMX274_SHR_LINEAR + 1;

        // gain related
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[2].u32RegAddr = IMX274_PGC;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[2].u8DelayFrmNum = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[3].u32RegAddr = IMX274_PGC + 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[3].u8DelayFrmNum = 1;
        
        /* Dgain cfg */
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[4].u32RegAddr = IMX274_DGAIN;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[4].u8DelayFrmNum = 1;        

        /* Slave Sensor VsTime cfg */
        g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;
        g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u8DelayFrmNum = 1;

        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[2].u8IntPos = 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[3].u8IntPos = 1;

        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
        {           
			g_apstSnsState[IspDev]->astRegsInfo[0].stSlvSync.u8DelayFrmNum = 2;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[0].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[0].u32RegAddr = IMX274_SHR_DOL1;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[1].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[1].u32RegAddr = IMX274_SHR_DOL1 + 1;

            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[5].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[5].u32RegAddr = IMX274_SHR_DOL2;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[6].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[6].u32RegAddr = IMX274_SHR_DOL2 + 1;

            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[7].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[7].u32RegAddr = IMX274_RHS1;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[8].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[8].u32RegAddr = IMX274_RHS1 + 1;
        }
        
        g_apstSnsState[IspDev]->bSyncInit = HI_TRUE;
    }
    else
    {
        for (i=0; i<g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            if (g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].u32Data == g_apstSnsState[IspDev]->astRegsInfo[1].astSspData[i].u32Data)
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].bUpdate = HI_FALSE;
            }
            else
            {
                g_apstSnsState[IspDev]->astRegsInfo[0].astSspData[i].bUpdate = HI_TRUE;
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

static HI_S32 cmos_set_image_mode(ISP_DEV IspDev,ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = g_apstSnsState[IspDev]->u8ImgMode;
    HI_U32 u32W, u32H;
    
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;    

        
    if (HI_NULL == pstSensorImageMode )
    {
        printf("null pointer when set image mode\n");
        return -1;
    }

    u32H = pstSensorImageMode->u16Height;
    u32W = pstSensorImageMode->u16Width;
  
    if (IMX274_RES_IS_4K(u32W, u32H))
    {
		if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
		{
			u8SensorImageMode = IMX274_MODE1_DOL;
		}
		else
		{
			u8SensorImageMode = IMX274_MODE0;
		}
    } 
    else
    {
        printf("Not support! Width:%d, Height:%d, Fps:%f\n",u32W,u32H,
        pstSensorImageMode->f32Fps);
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
    memset(g_apstSnsState[IspDev]->au32WDRIntTime, 0, sizeof(g_apstSnsState[IspDev]->au32WDRIntTime));
    
    return 0;
}

static HI_VOID sensor_global_init(ISP_DEV IspDev)
{
    g_apstSnsState[IspDev]->bInit = HI_FALSE;           //Sensor Init
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;       //Sync Reg init

    g_apstSnsState[IspDev]->u32FLStd = IMX274_VMAX_4K_30FPS_LINEAR;
    g_apstSnsState[IspDev]->au32FL[0]= IMX274_VMAX_4K_30FPS_LINEAR;
    g_apstSnsState[IspDev]->au32FL[1] = IMX274_VMAX_4K_30FPS_LINEAR;   
    g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;    
    g_apstSnsState[IspDev]->u8ImgMode = IMX274_MODE0;
    g_astimx274State[IspDev].u32DgainVal= 0;
    g_astimx274State[IspDev].u32RHS1_LocRLimit = IMX274_VMAX_4K_30FPS_DOL_WDR - 2233;
    
    memset(&g_apstSnsState[IspDev]->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_apstSnsState[IspDev]->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init                  = imx274_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit                  = imx274_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init           = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode               = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode                 = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default              = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level          = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect             = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info             = cmos_get_sns_regs_info;

    return 0;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static int imx274_set_bus_info(ISP_DEV IspDev, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx274BusInfo[IspDev].s8SspDev.bit4SspDev = unSNSBusInfo.s8SspDev.bit4SspDev;
    g_aunImx274BusInfo[IspDev].s8SspDev.bit4SspCs  = unSNSBusInfo.s8SspDev.bit4SspCs;

    return 0;
}

static int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX274_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }
    
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, pstAeLib, IMX274_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, pstAwbLib, IMX274_ID, &stAwbRegister);
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, IMX274_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }
    
    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, pstAeLib, IMX274_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, pstAwbLib, IMX274_ID);
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

ISP_SNS_OBJ_S stSnsImx274Obj = 
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx274_standby,
    .pfnRestart             = imx274_restart,
    .pfnWriteReg            = imx274_write_register,
    .pfnReadReg             = imx274_read_register,
    .pfnSetBusInfo          = imx274_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif 

