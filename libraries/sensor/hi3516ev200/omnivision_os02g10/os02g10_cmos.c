/*
 * Copyright (c) GK.  All rights reserved.
 */


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "os02g10_cmos_ex.h"
#include "os02g10_cmos.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define OS02G10_ID 0x02

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
ISP_SNS_STATE_S  *g_pastOs02g10[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define OS02G10_SENSOR_SET_CTX(dev, pstCtx)   (g_pastOs02g10[dev] = pstCtx)
#define OS02G10_SENSOR_RESET_CTX(dev)         (g_pastOs02g10[dev] = GK_NULL)


//static ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_PIPE_NUM] = {&g_pastos02g10[0], &g_pastos02g10[1]};


#define ISO_LUT_NUM     8

ISP_SNS_COMMBUS_U g_aunOs02g10_BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};


static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {ISP_FSWDR_NORMAL_MODE,ISP_FSWDR_NORMAL_MODE};

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0,0};

static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};  //默认初始曝光量，等于曝光行数*系统增益（6bit小数精度），AE算法采用该值作为初始5帧的曝光量
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static GK_BOOL g_abIsIrMode[ISP_MAX_PIPE_NUM] = {GK_FALSE};
static GK_U32 g_au32InitIso[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = 100};
static const GK_U32 g_au32IsoLut[ISO_LUT_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800};


#define EXPEXPTAR (4429) //这个地方设置exposureattr中exp的最大值
#define OS02G10_FULL_LINES_MAX  (0xFFFF)  //sensor降帧后1帧所能达到的最大行数，一般设置为sensor所支持的最大行数

/*****Os02g10 Register Address*****/
#define OS02G10_PAGE_ADDR (0xfd)
#define OS02G10_EXP_GKGH_ADDR (0x03)
#define OS02G10_EXP_LOW_ADDR (0x04)
#define OS02G10_AGAIN_ADDR (0x24)
#define OS02G10_DGAIN_ADDR_H (0x37)
#define OS02G10_DGAIN_ADDR_L (0x39)
#define OS02G10_VMAX_ADDR_H (0x05)
#define OS02G10_VMAX_ADDR_L (0x06)


#define OS02G10_INCREASE_LINES (1) /* make real fps less than stand fps because NVR require*/

#define OS02G10_VMAX_1080P30_DEFAULT_VBLANK 49
#define OS02G10_VMAX_1080P30_LINEAR  (1109+OS02G10_INCREASE_LINES + OS02G10_VMAX_1080P30_DEFAULT_VBLANK)  //帧长

//sensor fps mode
#define OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE      (0)


#define OS02G10_ERR_MODE_PRINT(pstSensorImageMode,pstSnsState)\
    do{\
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",\
                  pstSensorImageMode->u16Width,  \
                  pstSensorImageMode->u16Height, \
                  pstSensorImageMode->f32Fps,    \
                  pstSnsState->enWDRMode);\
    }while(0)

ISP_SNS_COMMBUS_U* os02g10_get_bus_Info(VI_PIPE vi_pipe_value)
{
    return &g_aunOs02g10_BusInfo[vi_pipe_value];
}

ISP_SNS_STATE_S* os02g10_get_ctx(VI_PIPE vi_pipe_value)
{
    return g_pastOs02g10[vi_pipe_value];
}


static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe,AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (GK_NULL == pstAeSnsDft)
    {
        //printf("null pointer when get ae default value!\n");
        return - 1;
    }

    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER(g_apstSnsState);

    //memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = g_apstSnsState->u32FLStd;  //基准帧率为1帧的有效行数
    pstAeSnsDft->u32FlickerFreq = 50*256;  //抗闪频率，数值为当前电源的256倍
    pstAeSnsDft->u32FullLinesMax = OS02G10_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.1;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;  //ISP的最小增益
    pstAeSnsDft->u32MaxISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;  //ISP的最大增益

	pstAeSnsDft->u32MaxIntTime = g_apstSnsState->u32FLStd - 9;
    pstAeSnsDft->u32MinIntTime = 4;    //最小曝光时间
    pstAeSnsDft->u32MaxIntTimeTarget = pstAeSnsDft->u32MaxIntTime;
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;             //最小曝光行

    if(g_au32LinesPer500ms[ViPipe] == 0)
	{
		pstAeSnsDft->u32LinesPer500ms = g_apstSnsState->u32FLStd*30/2;  //每500ms的总行数
	}
	else
	{
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
	}


    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch(g_apstSnsState->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:   /*linear mode*/
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u32MaxAgain = 15872;          //sensor的最大模拟增益
            pstAeSnsDft->u32MinAgain = 1024;           //sensor的最小模拟增益
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 15872;//1024;          //sensor的最大数字增益
            pstAeSnsDft->u32MinDgain = 1024;          //sensor的最小数字增益
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u8AeCompensation = 0x49;       // AE亮度目标值，取值范围[0,255],建议使用0x38~0x40
            //pstAeSnsDft->enAeExpMode = AE_EXP_GKGHLIGHT_PRIOR;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 130000;

            //pstAeSnsDft->u32MaxIntTime = 2680;//g_apstSnsState->u32FLStd - 2;
            //pstAeSnsDft->u32MinIntTime = 1;

		    pstAeSnsDft->u32MaxIntTime = g_apstSnsState->u32FLStd - 9;
            pstAeSnsDft->u32MinIntTime = 4;    //最小曝光时间
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 1;             //最小曝光行
        break;

    }

    return 0;
}


/* the function of sensor set fps */
/* the function of sensor set fps ---- modify by cai
* HTS(line):1253 VTS(frame):1117 clk：42M  fps 30.0085fps
* VTS:30fps->1253 line
* 	  25fps->1504 line
* 通过加Vb最高降帧。
*	  30fps vb = 1253-1253 = 0 =  0x00
*	  25fps vb = 1504-1253 = 251 = 0xfb。
* 	  05 06 控制Vb
*/
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

    GK_U32 u32VMAX = OS02G10_VMAX_1080P30_LINEAR;
	//GK_U32 tp = 1117;
	GK_U32 VBLANK = 0;	      //VB

    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);


    switch (g_apstSnsState->u8ImgMode)
    {
      case OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE:
           if ((f32Fps <= 30) && (f32Fps >= 5))
           {
               u32VMAX = OS02G10_VMAX_1080P30_LINEAR * 30 / f32Fps;
           }
           else
           {
               printf("Not support Fps: %f\n", f32Fps);
               return;
           }
           u32VMAX = (u32VMAX > OS02G10_FULL_LINES_MAX) ? OS02G10_FULL_LINES_MAX : u32VMAX;
		   VBLANK = u32VMAX - (OS02G10_VMAX_1080P30_LINEAR - OS02G10_VMAX_1080P30_DEFAULT_VBLANK);
           break;
      default:
          return;
    }

    if (WDR_MODE_NONE == g_apstSnsState->enWDRMode)
    {
		g_apstSnsState->astRegsInfo[1].astI2cData[6].u32Data = 0x00;
		g_apstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;
        g_apstSnsState->astRegsInfo[0].astI2cData[4].u32Data = ((VBLANK & 0xFF00) >> 8);
        g_apstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (VBLANK & 0xFF);
        g_apstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x01;
    }
    else
    {
		g_apstSnsState->astRegsInfo[1].astI2cData[6].u32Data = 0x00;
        g_apstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;
        g_apstSnsState->astRegsInfo[0].astI2cData[4].u32Data = ((u32VMAX & 0xFF00) >> 8);
        g_apstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (u32VMAX & 0xFF);
        g_apstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x01;
    }

    g_apstSnsState->u32FLStd = u32VMAX;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = g_apstSnsState->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState->u32FLStd -9;
    g_apstSnsState->au32FL[0] = g_apstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = g_apstSnsState->au32FL[0];

    // printf("cmos_fps_set fps=%f,VBLANK=%d\n",f32Fps,VBLANK);
    return;

}


static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe,GK_U32 u32FullLines,AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
	GK_U32 VBLANK = 0;

    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);


    u32FullLines = (u32FullLines > OS02G10_FULL_LINES_MAX) ? OS02G10_FULL_LINES_MAX : u32FullLines;
    g_apstSnsState->au32FL[0] = u32FullLines;
	VBLANK = u32FullLines - (OS02G10_VMAX_1080P30_LINEAR - OS02G10_VMAX_1080P30_DEFAULT_VBLANK);
    if(WDR_MODE_NONE == g_apstSnsState->enWDRMode)
    {
		g_apstSnsState->astRegsInfo[1].astI2cData[6].u32Data = 0x00;
        g_apstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;
        g_apstSnsState->astRegsInfo[0].astI2cData[4].u32Data = ((VBLANK & 0xFF00) >> 8);
        g_apstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (VBLANK & 0xFF);
        g_apstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x01;
    }
    else
    {
		g_apstSnsState->astRegsInfo[1].astI2cData[6].u32Data = 0x00;
		g_apstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;
        g_apstSnsState->astRegsInfo[0].astI2cData[4].u32Data = ((VBLANK & 0xFF00) >> 8);
        g_apstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (VBLANK & 0xFF);
        g_apstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x01;
    }

    pstAeSnsDft->u32FullLines = g_apstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState->au32FL[0] - 9;

    return;
}

/* wGKle isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe,GK_U32 u32IntTime)
{
    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);


	//printf("------------------------------ %d\n",u32IntTime);
	g_apstSnsState->astRegsInfo[1].astI2cData[6].u32Data = 0x00;

    g_apstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;
    g_apstSnsState->astRegsInfo[0].astI2cData[1].u32Data = ((u32IntTime & 0xFF00) >> 8);
    g_apstSnsState->astRegsInfo[0].astI2cData[2].u32Data = (u32IntTime & 0xFF);
    g_apstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x01;


  return;

}

static GK_U32 again_table[64]=
{
    1024 ,  1088 , 1152 , 1216 , 1280 ,  1344 ,  1408 ,  1472 ,  1536 ,  1600 ,  1664 ,  1728 ,  1792 ,  1856 , 1920 , 1984 ,
	2048 ,  2176 , 2304 , 2432 , 2560 ,  2688 ,  2816 ,  2944 ,  3072 ,  3200 ,  3328 ,  3456 ,  3584 ,  3712 , 3840 , 3968 ,
	4096 ,  4352 , 4608 , 4864 , 5120 ,  5376 ,  5632 ,  5888 ,  6144 ,  6400 ,  6656 ,  6912 ,  7168 ,  7424 , 7680 , 7936 ,
	8192 ,  8704 , 9216 , 9728 , 10240 , 10752 , 11264 , 11776 , 12288 , 12800 , 13312 , 13824 , 14336 , 14848 , 15360 , 15872
};        //ana_gain

static GK_U32 dgain_table[64]=
{
    1024 ,  1088 , 1152 , 1216 , 1280 ,  1344 ,  1408 ,  1472 ,  1536 ,  1600 ,  1664 ,  1728 ,  1792 ,  1856 , 1920 , 1984 ,
	2048 ,  2176 , 2304 , 2432 , 2560 ,  2688 ,  2816 ,  2944 ,  3072 ,  3200 ,  3328 ,  3456 ,  3584 ,  3712 , 3840 , 3968 ,
	4096 ,  4352 , 4608 , 4864 , 5120 ,  5376 ,  5632 ,  5888 ,  6144 ,  6400 ,  6656 ,  6912 ,  7168 ,  7424 , 7680 , 7936 ,
	8192 ,  8704 , 9216 , 9728 , 10240 , 10752 , 11264 , 11776 , 12288 , 12800 , 13312 , 13824 , 14336 , 14848 , 15360 , 15872
};         //dig_gain


GK_S32 hotFlag = FALSE;
#define BLC_Level_Max 0x18
#define BLC_Level_Min 0x06

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe,GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    int i;

#if 0
    if (*pu32AgainLin >= again_table[63])
    {
         *pu32AgainLin = again_table[63];
         *pu32AgainDb = 63;
         return ;
    }

    for (i = 1; i < 64; i++)
    {
        if (*pu32AgainLin < again_table[i])
        {
            *pu32AgainLin = again_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }
#else	//加入高温对策
	int blcValue = 0;

	blcValue = os02g10_read_register(ViPipe, 0xdd);
	//printf("cmos_again_calc_table======%d!\n", blcValue);

	if(blcValue > 0xF0)
		blcValue = 0;
	if(blcValue <= BLC_Level_Min)
		hotFlag = 0;

	if((blcValue <= BLC_Level_Max) && (hotFlag == 0))
	{
		if (*pu32AgainLin >= again_table[63])
		{
			 *pu32AgainLin = again_table[63];
			 *pu32AgainDb = 63;
			 return ;
		}

		for (i = 1; i < 64; i++)
		{
			if (*pu32AgainLin < again_table[i])
			{
				*pu32AgainLin = again_table[i - 1];
				*pu32AgainDb = i - 1;
				break;
			}
		}
		hotFlag = 0;
	}
	else
	{
		if (*pu32AgainLin >= again_table[48])
		{
			 *pu32AgainLin = again_table[48];
			 *pu32AgainDb = 48;
			 return ;
		}

		for (i = 1; i < 49; i++)
		{
			if (*pu32AgainLin < again_table[i])
			{
				*pu32AgainLin = again_table[i - 1];
				*pu32AgainDb = i - 1;
				break;
			}
		}
		hotFlag = 1;
	}
#endif
    return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe,GK_U32 *pu32DgainLin, GK_U32 *pu32DgainDb)
{
    int i;

    if((GK_NULL == pu32DgainLin) ||(GK_NULL == pu32DgainDb))
    {
        printf("null pointer when get ae sensor gain info value!\n");
        return;
    }

#if 0
	if (*pu32DgainLin >= dgain_table[63])
	{
		 *pu32DgainLin = dgain_table[63];
		 *pu32DgainDb = 63;
		 return ;
	}

	for (i = 1; i < 64; i++)
	{
		if (*pu32DgainLin < dgain_table[i])
		{
			*pu32DgainLin = dgain_table[i - 1];
			*pu32DgainDb = i - 1;
			break;
		}
	}
#else	//加入高温对策

	int blcValue = 0;

	blcValue = os02g10_read_register(ViPipe, 0xdd);
	// printf("cmos_dgain_calc_table======%d!\n", blcValue);

	if(blcValue > 0xF0)
		blcValue = 0;
	if(blcValue <= BLC_Level_Min)
		hotFlag = 0;

	if((blcValue <= BLC_Level_Max) && (hotFlag == 0))
	{
	    if (*pu32DgainLin >= dgain_table[63])
	    {
	         *pu32DgainLin = dgain_table[63];
	         *pu32DgainDb = 63;
	         return ;
	    }

	    for (i = 1; i < 64; i++)
	    {
	        if (*pu32DgainLin < dgain_table[i])
	        {
	            *pu32DgainLin = dgain_table[i - 1];
	            *pu32DgainDb = i - 1;
	            break;
	        }
	    }
		hotFlag = 0;
	}
	else
	{
		hotFlag =1;

		*pu32DgainLin = dgain_table[0];
		*pu32DgainDb = 0;
	}
#endif

    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe,GK_U32 u32Again, GK_U32 u32Dgain)
{
    GK_U32 Again, Dgain;
    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);

	g_apstSnsState->astRegsInfo[1].astI2cData[6].u32Data = 0x00;
    Again = (again_table[u32Again] / 0x40) ;
	Dgain = (dgain_table[u32Dgain] / 0x10);

	g_apstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;
	g_apstSnsState->astRegsInfo[0].astI2cData[3].u32Data = (Again & 0xFF);
	g_apstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x01;

	g_apstSnsState->astRegsInfo[0].astI2cData[7].u32Data = (Dgain & 0xFF00) >> 8;
	g_apstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (Dgain & 0xFF);
    return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe,GK_U16 u16ManRatioEnable, GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax, GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime)
{
#if 0
    //GK_U32 i = 0;
    GK_U32 u32IntTimeMaxTmp  = 0;
    GK_U32 u32ShortTimeMinLimit = 0;
    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);

    u32ShortTimeMinLimit = 2;

    if((GK_NULL == au32Ratio) || (GK_NULL == au32IntTimeMax) || (GK_NULL == au32IntTimeMin))
    {
        printf("null pointer when get ae sensor ExpRatio/IntTimeMax/IntTimeMin value!\n");
        return;
    }

    if(u32IntTimeMaxTmp >= u32ShortTimeMinLimit)
    {
        if (IS_LINE_WDR_MODE(g_apstSnsState->enWDRMode))
        {
            au32IntTimeMax[0] = u32IntTimeMaxTmp;
            au32IntTimeMax[1] = au32IntTimeMax[0] * au32Ratio[0] >> 6;
            au32IntTimeMax[2] = au32IntTimeMax[1] * au32Ratio[1] >> 6;
            au32IntTimeMax[3] = au32IntTimeMax[2] * au32Ratio[2] >> 6;
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >> 6;
            au32IntTimeMin[2] = au32IntTimeMin[1] * au32Ratio[1] >> 6;
            au32IntTimeMin[3] = au32IntTimeMin[2] * au32Ratio[2] >> 6;
        }
        else
        {
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
            u32IntTimeMaxTmp = u32ShortTimeMinLimit;
            au32IntTimeMin[0] = au32IntTimeMax[0];
            au32IntTimeMin[1] = au32IntTimeMax[1];
            au32IntTimeMin[2] = au32IntTimeMax[2];
            au32IntTimeMin[3] = au32IntTimeMax[3];
        }
    }
#endif
    return;

}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe,AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[ViPipe] = 0;
}


static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
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

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

/* AWB default parameter and function */
#define CALIBRATE_STATIC_WB_R_GAIN 379	//404
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN 651	//509

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 130	//12
#define CALIBRATE_AWB_P2 -12	//213
#define CALIBRATE_AWB_Q1 -138	//-30
#define CALIBRATE_AWB_A1 211250	//186383
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 -143554	//-131654


static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe,AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    OS02G10_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 5082;

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
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            if (g_abIsIrMode[ViPipe] == GK_TRUE) {
                memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableIr, sizeof(AWB_AGC_TABLE_S));
            } else {
                memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            }
            break;
    }
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
    pstAwbSnsDft->u8AWBRunInterval = 4; /* need fix 4 */

    return GK_SUCCESS;

}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return 0;
}


static GK_U32 Interpulate(GK_U32 u32Mid, GK_U32 u32Left, GK_U32 u32LValue, GK_U32 u32Right, GK_U32 u32RValue)
{
    GK_U32 u32Value = 0;
    GK_U32 k = 0;

    if (u32Mid <= u32Left)
    {
        u32Value = u32LValue;
        return u32Value;
    }

    if (u32Mid >= u32Right)
    {
        u32Value = u32RValue;
        return u32Value;
    }

    k = (u32Right - u32Left);
    u32Value = (((u32Right - u32Mid) * u32LValue + (u32Mid - u32Left) * u32RValue + (k >> 1)) / k);

    return u32Value;
}


static GK_VOID cmos_set_drc_init_str(VI_PIPE ViPipe)
{
    GK_U32 u32Level = 0;
    GK_U32 au32DrcInitStr0[ISO_LUT_NUM] = { DRC_INIT_STR_NA };
    GK_U32 au32DrcInitStr1[ISO_LUT_NUM] = { DRC_INIT_STR_IR };

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
            g_stIspDRC.u16ManualStrength = Interpulate(g_au32InitIso[ViPipe],
                                                       g_au32IsoLut[u32Level - 1], au32DrcInitStr1[u32Level - 1],
                                                       g_au32IsoLut[u32Level], au32DrcInitStr1[u32Level]);
        }
    } else {
        if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
            g_stIspDRC.u16ManualStrength = au32DrcInitStr0[u32Level];
        } else {
            g_stIspDRC.u16ManualStrength = Interpulate(g_au32InitIso[ViPipe],
                                                       g_au32IsoLut[u32Level - 1], au32DrcInitStr0[u32Level - 1],
                                                       g_au32IsoLut[u32Level], au32DrcInitStr0[u32Level]);
        }
    }
}


static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
	{378, 256, 430},
	{439, 256, 439}
};


static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe,ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

     CMOS_CHECK_POINTER(pstDef);
     OS02G10_SENSOR_GET_CTX(ViPipe, pstSnsState);
     CMOS_CHECK_POINTER(pstSnsState);

     memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
     cmos_set_drc_init_str(ViPipe);

     pstDef->unKey.bit1Ca       = 1;
     pstDef->pstCa              = &g_stIspCA;
     pstDef->unKey.bit1Dpc      = 1;
     pstDef->pstDpc             = &g_stCmosDpc;
     pstDef->unKey.bit1Wdr      = 1;
     pstDef->pstWdr             = &g_stIspWDR;
     pstDef->unKey.bit1Lsc      = 1;
     pstDef->pstLsc             = &g_stCmosLsc;
     pstDef->unKey.bit1PreGamma = 0;
     pstDef->pstPreGamma        = &g_stPreGamma;

     switch (pstSnsState->enWDRMode) {
         default:
         case WDR_MODE_NONE:
             pstDef->unKey.bit1Demosaic       = 1;
             pstDef->pstDemosaic              = &g_stIspDemosaic;
             pstDef->unKey.bit1Sharpen        = 1;
             pstDef->pstSharpen               = &g_stIspYuvSharpen;
             pstDef->unKey.bit1Drc            = 1;
                 if(g_abIsIrMode[ViPipe] == GK_TRUE){
                 pstDef->pstDrc                   = &g_stIspDRCIr;
             }
             else{
                 pstDef->pstDrc                   = &g_stIspDRC;
             }
             pstDef->unKey.bit1BayerNr        = 1;
             pstDef->pstBayerNr               = &g_stIspBayerNr;
             pstDef->unKey.bit1AntiFalseColor = 1;
             pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
             pstDef->unKey.bit1Ldci           = 1;
             if (g_abIsIrMode[ViPipe] == GK_TRUE) {
                 pstDef->pstLdci              = &g_stIspLdciIr;
             } else {
                 pstDef->pstLdci              = &g_stIspLdci;
             }
             pstDef->unKey.bit1Gamma          = 1;
             if (g_abIsIrMode[ViPipe] == GK_TRUE) {
                 pstDef->pstGamma             = &g_stIspGammaIr;
             } else {
                 pstDef->pstGamma             = &g_stIspGamma;
             }
             pstDef->unKey.bit1Ge             = 1;
             pstDef->pstGe                    = &g_stIspGe;
             pstDef->unKey.bit1Dehaze         = 1;
             if (g_abIsIrMode[ViPipe] == GK_TRUE) {
                 pstDef->pstDehaze            = &g_stIspDehazeIr;
             } else {
                 pstDef->pstDehaze            = &g_stIspDehaze;
             }
             pstDef->unKey.bit1Rgbir          = 0;
             memcpy(&pstDef->stNoiseCalibration,   &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
             break;
     }

     pstDef->stSensorMode.u32SensorID = OS02G10_ID;
     pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

     memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

     switch (pstSnsState->u8ImgMode) {
         default:
         case OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE:
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
     pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;

     return GK_SUCCESS;

}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe,ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    GK_S32  i;

    if (GK_NULL == pstBlackLevel)
    {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }
    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER(g_apstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_TRUE;
    /* black level of DOL mode */
    if (WDR_MODE_NONE == g_apstSnsState->enWDRMode)
    {
        for (i=0; i<4; i++)
        {
            pstBlackLevel->au16BlackLevel[i] = 0x100;    // 240
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

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe,GK_BOOL bEnable)
{

    GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);

    if (OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE == g_apstSnsState->u8ImgMode)
    {
        u32FullLines_5Fps = (OS02G10_VMAX_1080P30_LINEAR * 30) / 5;
    }

    else
    {
        return;
    }

    u32FullLines_5Fps = (u32FullLines_5Fps > OS02G10_FULL_LINES_MAX) ? OS02G10_FULL_LINES_MAX : u32FullLines_5Fps;
    u32FullLines_5Fps = u32FullLines_5Fps - (OS02G10_VMAX_1080P30_LINEAR - OS02G10_VMAX_1080P30_DEFAULT_VBLANK);
    u32MaxIntTime_5Fps = 4;

    if (bEnable) /* setup for ISP pixel calibration mode */
    {
		os02g10_write_register (ViPipe,0xfd,0x01);
        os02g10_write_register (ViPipe,OS02G10_AGAIN_ADDR,0x80);

        os02g10_write_register (ViPipe,OS02G10_VMAX_ADDR_H, (u32FullLines_5Fps & 0xFF00) >> 8);
        os02g10_write_register (ViPipe,OS02G10_VMAX_ADDR_L, u32FullLines_5Fps & 0xFF);

        os02g10_write_register (ViPipe,OS02G10_EXP_GKGH_ADDR, (u32MaxIntTime_5Fps & 0xFF00) >> 8);
        os02g10_write_register (ViPipe,OS02G10_EXP_LOW_ADDR, u32MaxIntTime_5Fps & 0xFF);
        os02g10_write_register (ViPipe,0x01, 0x01);

    }
    else /* setup for ISP 'normal mode' */
    {
        g_apstSnsState->u32FLStd = (g_apstSnsState->u32FLStd > 0xFFFF) ? 0xFFFF : g_apstSnsState->u32FLStd;
        os02g10_write_register (ViPipe,OS02G10_VMAX_ADDR_H, (u32FullLines_5Fps & 0xFF00) >> 8);
        os02g10_write_register (ViPipe,OS02G10_VMAX_ADDR_L, u32FullLines_5Fps & 0xFF);
		os02g10_write_register (ViPipe,0x01, 0x01);
        g_apstSnsState->bSyncInit = GK_FALSE;
    }

    return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe,GK_U8 u8Mode)
{

    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER(g_apstSnsState);


    g_apstSnsState->bSyncInit = GK_FALSE;

    switch(u8Mode)
    {
        case WDR_MODE_NONE:
            g_apstSnsState->enWDRMode = WDR_MODE_NONE;
            g_apstSnsState->u32FLStd = OS02G10_VMAX_1080P30_LINEAR;
            g_apstSnsState->u8ImgMode = OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE;
        break;

        default:
            printf("NOT support this mode!\n");
            return -1;
    }

    g_apstSnsState->au32FL[0]= g_apstSnsState->u32FLStd;
    g_apstSnsState->au32FL[1] = g_apstSnsState->au32FL[0];
    memset(g_apstSnsState->au32WDRIntTime, 0, sizeof(g_apstSnsState->au32WDRIntTime));

    return 0;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe,ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    GK_S32 i;

    if (GK_NULL == pstSnsRegsInfo)
    {
        printf("null pointer when get sns reg info!\n");
        return -1;
    }

    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER(g_apstSnsState);

    if ((GK_FALSE == g_apstSnsState->bSyncInit) || (GK_FALSE == pstSnsRegsInfo->bConfig))
    {
        g_apstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        g_apstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunOs02g10_BusInfo[ViPipe].s8I2cDev;
        g_apstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        g_apstSnsState->astRegsInfo[0].u32RegNum = 9;//7;

        for (i=0; i<g_apstSnsState->astRegsInfo[0].u32RegNum; i++)
        {
            g_apstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            g_apstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = OS02G10_I2C_ADDR;
            g_apstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OS02G10_ADDR_BYTE;
            g_apstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = OS02G10_DATA_BYTE;
        }

        //Linear Mode Regs
        g_apstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        g_apstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = OS02G10_PAGE_ADDR;
        g_apstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        g_apstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = OS02G10_EXP_GKGH_ADDR;
        g_apstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        g_apstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = OS02G10_EXP_LOW_ADDR;

        g_apstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;       //make shutter and gain effective at the same time
        g_apstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = OS02G10_AGAIN_ADDR;  //again

        g_apstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
        g_apstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = OS02G10_VMAX_ADDR_H;
        g_apstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        g_apstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = OS02G10_VMAX_ADDR_L;   //vblank addr
		g_apstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        g_apstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = 0x01;

		 g_apstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
		 g_apstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = OS02G10_DGAIN_ADDR_H;	//dgain
		 g_apstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
		 g_apstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = OS02G10_DGAIN_ADDR_L;	//dgain

         g_apstSnsState->bSyncInit = GK_TRUE;

    }
    else
    {
        for (i=0; i<g_apstSnsState->astRegsInfo[0].u32RegNum; i++)
        {
            if (g_apstSnsState->astRegsInfo[0].astI2cData[i].u32Data == g_apstSnsState->astRegsInfo[1].astI2cData[i].u32Data)
            {
                g_apstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
            }

            else
            {
                g_apstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            }
        }
    }

    pstSnsRegsInfo->bConfig = GK_FALSE;
    memcpy(pstSnsRegsInfo, &g_apstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&g_apstSnsState->astRegsInfo[1], &g_apstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    g_apstSnsState->au32FL[1] = g_apstSnsState->au32FL[0];

    return 0;
}

static GK_S32 cmos_set_image_mode(VI_PIPE ViPipe,ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    GK_U8 u8SensorImageMode = 0;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    OS02G10_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = GK_FALSE;

    if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080)) {
        if (pstSensorImageMode->f32Fps <= 30) {
            u8SensorImageMode     = OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE;
            pstSnsState->u32FLStd = OS02G10_VMAX_1080P30_LINEAR;
        } else {
            OS02G10_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
            return GK_FAILURE;
        }
    } else {
        OS02G10_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
        return GK_FAILURE;
    }

    if ((GK_TRUE == pstSnsState->bInit) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }
    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return GK_SUCCESS;

}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{

    ISP_SNS_STATE_S *g_apstSnsState = GK_NULL;
    OS02G10_SENSOR_GET_CTX(ViPipe, g_apstSnsState);
    CMOS_CHECK_POINTER_VOID(g_apstSnsState);


    g_apstSnsState->bInit = GK_FALSE;
    g_apstSnsState->bSyncInit = GK_FALSE;
    g_apstSnsState->u8ImgMode = OS02G10_SENSOR_1080P_30FPS_LINEAR_MODE;
    g_apstSnsState->enWDRMode = WDR_MODE_NONE;
    g_apstSnsState->u32FLStd = OS02G10_VMAX_1080P30_LINEAR;
    g_apstSnsState->au32FL[0] = OS02G10_VMAX_1080P30_LINEAR;
    g_apstSnsState->au32FL[1] = OS02G10_VMAX_1080P30_LINEAR;

    memset(&g_apstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_apstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = os02g10_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = os02g10_exit;
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

static int os02g10_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOs02g10_BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return 0;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    OS02G10_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (GK_NULL == pastSnsStateCtx) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (GK_NULL == pastSnsStateCtx) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    OS02G10_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    OS02G10_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    OS02G10_SENSOR_RESET_CTX(ViPipe);
    return;
}


static int sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    GK_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(ViPipe);

    if (s32Ret != GK_SUCCESS) {
        return GK_FAILURE;
    }

    stSnsAttrInfo.eSensorId = OS02G10_ID;

    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret |= GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return GK_SUCCESS;

}

static int sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    GK_S32 s32Ret;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, OS02G10_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, OS02G10_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, OS02G10_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return GK_SUCCESS;

}

static int sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;

    return 0;
}

ISP_SNS_OBJ_S stSnsOs02g10Obj =
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = os02g10_standby,
    .pfnRestart             = os02g10_restart,
    .pfnMirrorFlip			= os02g10_mirror_flip,
    .pfnWriteReg            = os02g10_write_register,
    .pfnReadReg             = os02g10_read_register,
    .pfnSetBusInfo          = os02g10_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


