
/******************************************************************************

  Copyright (C), 2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sc3336_mipi_cmos.c

  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2018/07/23
  Description   :
  History       :
  1.Date        : 2018/07/23
    Author      :
    Modification: Created file

******************************************************************************/
#if !defined(__SC3336_MIPI_CMOS_H_)
#define __SC3336_MIPI_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "hi_comm_isp.h"

#include "sc3336_mipi_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define SC3336_ID	3336

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

#define HIGH_8BITS(x)   (((x) & 0xFF00) >> 8)
#define LOW_8BITS(x)    ( (x) & 0x00FF)
#define LOWER_4BITS(x)  (((x) & 0x000F) << 4)
#define HIGHER_4BITS(x) (((x) & 0xF000) >> 12)
#define HIGHER_8BITS(x) (((x) & 0x0FF0) >> 4)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSc3336[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define SC3336_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastSc3336[dev])
#define SC3336_SENSOR_SET_CTX(dev, pstCtx)   (g_pastSc3336[dev] = pstCtx)
#define SC3336_SENSOR_RESET_CTX(dev)         (g_pastSc3336[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunSc3336BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};

static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int smartsens_i2c_addr;
extern unsigned int smartsens_addr_byte;
extern unsigned int smartsens_data_byte;

extern void smartsens_init(VI_PIPE ViPipe);
extern void smartsens_exit(VI_PIPE ViPipe);
extern void smartsens_standby(VI_PIPE ViPipe);
extern void smartsens_restart(VI_PIPE ViPipe);
extern int  smartsens_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  smartsens_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/


/******** SC3336 Register Address ********/
#define SC3336_EXP_H_ADDR               (0x3E00)
#define SC3336_EXP_M_ADDR               (0x3E01)
#define SC3336_EXP_L_ADDR               (0x3E02)
#define SC3336_AGAIN_H_ADDR             (0x3E08)
#define SC3336_AGAIN_L_ADDR             (0x3E09)
#define SC3336_DGAIN_H_ADDR             (0x3E06)
#define SC3336_DGAIN_L_ADDR             (0x3E07)
#define SC3336_VMAX_H_ADDR              (0x320E)
#define SC3336_VMAX_L_ADDR              (0x320F)

#define SC3336_FULL_LINES_MAX           (0x7FFF)
#define SC3336_INCREASE_LINES           (0) /* make real fps less than stand fps because NVR require */
#define SC3336_VMAX_1296P30_LINEAR      (1350 + SC3336_INCREASE_LINES)
#define SC3336_FRAME_RATE_MIN   		(0x34BC)
#define EXP_OFFSET   					(4)

// sensor fps mode
#define SC3336_SENSOR_1296P_30FPS_LINEAR_MODE (1)
#define SC3336_RES_IS_1296P(w, h)       ((w) <= 2304 && (h) <= 1296)



//static HI_U32 gu32MaxShortEXP = SC2336_SEXP_MAX_DEFAULT;

HI_U16 Dgain_mapping_realgain_to_sensorgain(HI_FLOAT gain_real);
HI_U16 Dgain_mapping_realgain_to_sensorgain(HI_FLOAT gain_real)
{
	HI_U16 u16sensorgain;
	HI_U32 u32Dgain = gain_real*128;
	HI_U8 dCoarseGain = 0;
	HI_U8 dFineGain = 0;
	HI_U8 u8Reg0x3e06 = 0;
	for(dCoarseGain = 1; dCoarseGain <= 16; dCoarseGain = dCoarseGain * 2)		//1,2,4,8,16
	{
		if(u32Dgain < (128 * 2 * dCoarseGain))
		{
			break;
		}
	}
	dFineGain = u32Dgain / dCoarseGain;
	//printf("dCoarseGain %d, dFineGain 0x%x, ", dCoarseGain, dFineGain);

	for ( ; dCoarseGain >= 2; dCoarseGain=dCoarseGain/2)
	{
		u8Reg0x3e06 = (u8Reg0x3e06 << 1) | 0x01;
	}
	u16sensorgain = ((HI_U16)u8Reg0x3e06) << 8;
	u16sensorgain += dFineGain;
	
	return u16sensorgain;
}
static HI_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256;
    pstAeSnsDft->u32FullLinesMax = SC3336_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125;//1/128

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0078125;//1/128 

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 20 / 2;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            pstAeSnsDft->au8HistThresh[0] = 0x0D;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u32MaxAgain =	6300; //32*128*1.52
            pstAeSnsDft->u32MinAgain =	0x80;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain =	508;//3.969x128
            pstAeSnsDft->u32MinDgain =	0x80;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u8AeCompensation = 0x40;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 148859;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - EXP_OFFSET;
            pstAeSnsDft->u32MinIntTime = 1;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 1;
            break;
    }

    return HI_SUCCESS;
}


/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE ViPipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32FullLines;
    HI_U32 u32VMAX = SC3336_VMAX_1296P30_LINEAR;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case SC3336_SENSOR_1296P_30FPS_LINEAR_MODE:
            if ((f32Fps <= 20) && (f32Fps >= 4)) {
                u32VMAX = SC3336_VMAX_1296P30_LINEAR * 20 / DIV_0_TO_1_FLOAT(f32Fps);
            } else {
                ISP_TRACE(HI_DBG_ERR, "Not support Fps: %f\n", f32Fps);
                return;
            }
            break;
        default:
            break;
    }

    u32VMAX = (u32VMAX > SC3336_FULL_LINES_MAX) ? SC3336_FULL_LINES_MAX : u32VMAX;

    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGH_8BITS(u32VMAX);
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(u32VMAX);

    u32FullLines = u32VMAX;
    pstSnsState->u32FLStd = u32FullLines;
    pstAeSnsDft->f32Fps = f32Fps;
    //pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - EXP_OFFSET;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    u32FullLines = (u32FullLines > SC3336_FRAME_RATE_MIN) ? SC3336_FRAME_RATE_MIN : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGH_8BITS(u32FullLines);
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(u32FullLines);

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - EXP_OFFSET;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE ViPipe, HI_U32 u32IntTime)
{
    HI_U32 u32Val;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Val = (u32IntTime > (SC3336_FULL_LINES_MAX - EXP_OFFSET)) ? (SC3336_FULL_LINES_MAX - EXP_OFFSET) : u32IntTime;

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = HIGHER_4BITS(u32Val);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGHER_8BITS(u32Val);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOWER_4BITS(u32Val);


    return;
}

static const HI_U16 u16AgainTab[64] =
{
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856,
    1920, 1984, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456,
    3584, 3712, 3840, 3968, 4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400,
    6656, 6912, 7168, 7424, 7680, 7936, 8192, 8704, 9216, 9728, 10240, 10752, 11264,
    11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

static HI_VOID cmos_again_calc_table(VI_PIPE ViPipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int again;
    int i;
    static HI_U8 againmax = 63;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    again = *pu32AgainLin;

    if (again >= u16AgainTab[againmax])
    {
        *pu32AgainDb = againmax;
    }
    else
    {
        for (i = 1; i < 63; i++)
        {
            if (again < u16AgainTab[i])
            {
                *pu32AgainDb = i - 1;
                break;
            }
        }
    }
    *pu32AgainLin = u16AgainTab[*pu32AgainDb];

    return;
}

static HI_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    *pu32DgainDb = *pu32DgainLin / 1024;
    if (*pu32DgainDb == 3)
    {
        *pu32DgainDb = 2;
    }
    else if (*pu32DgainDb >= 4 && *pu32DgainDb < 8)
    {
        *pu32DgainDb = 4;
    }
    else if (*pu32DgainDb >= 8)
    {
        *pu32DgainDb = 8;
    }

    *pu32DgainLin = *pu32DgainDb * 1024;

    return;
}


static HI_FLOAT Again_real_cal[]={	128,		1,	194.56,		1.52,	194.56*2,		1.52*2,	194.56*4,		1.52*4,	194.56*8,	1.52*8,		194.56*16,		1.52*16,	194.56*32,	1.52*32};
static HI_FLOAT Again_reg_table[]={	128,	0x00,	194.56,		0x40,	194.56*2,		0x48,	194.56*4,		0x49,	194.56*8,	0x4b,		194.56*16,		0x4f,		194.56*32,	0x5f};


static HI_VOID cmos_gains_update(VI_PIPE ViPipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    HI_U8 u8Reg0x3e09, u8Reg0x3e08, u8Reg0x3e07, u8Reg0x3e06;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
	
	HI_U16 u16Again,u16Dgain;
	HI_FLOAT f_Dgain; //f_Again
	
	//HI_U8 Again_temp;
	HI_U8 Again_index;
	for(Again_index = 12; Again_index>=0 ;Again_index-=2)
	{
		if(u32Again >= Again_real_cal[Again_index])
			break;
	}
	//Again_temp = u32Again/Again_real_cal[Again_index]*128.0;
	u16Again = Again_reg_table[Again_index+1];
	
	//f_Again = Again_real_cal[Again_index+1];
	
	f_Dgain = (u32Again / Again_real_cal[Again_index] ) * u32Dgain / 128.0;
	
	//u16Again = Again_mapping_realgain_to_sensorgain(f_Again,3.406,64);
	u16Dgain = Dgain_mapping_realgain_to_sensorgain(f_Dgain);
	
    u8Reg0x3e06 = HIGH_8BITS(u16Dgain);
    u8Reg0x3e07 = LOW_8BITS (u16Dgain);
    u8Reg0x3e08 = 0x00;
    u8Reg0x3e09 = LOW_8BITS (u16Again);
	
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = u8Reg0x3e06;
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u8Reg0x3e07;
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = u8Reg0x3e08;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8Reg0x3e09;

	// /* Gain Logic */
    // pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x00;
	// if (smartsens_version == 0xff) {
		// smartsens_version = smartsens_read_register(ViPipe, 0x3040);
	//	printf("Version=0x%x\n",smartsens_version);
	// }
	// if (smartsens_version == 0x40) {
		// if (u8Reg0x3e08 < 0x07) { /* 0x0340~0x037f gain<2x */
			// pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x0e;
		// } else { /* gain >= 2x */
			// pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x07;
		// }
	// } else if (smartsens_version == 0x41) { // smartsens_version == 0x41
		// if (u8Reg0x3e08 < 0x07) { /* 0x0340~0x037f gain<2x */
			// pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x0f;
		// } else { /* gain >= 2x */
			// pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x07;
		// }
	// } else {
		// pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x07;
	// }

    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE ViPipe, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    return;
}

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[ViPipe] = 0;

    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set            = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update     = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update       = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static HI_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 5000;

    pstAwbSnsDft->au16GainOffset[0] = 478;
    pstAwbSnsDft->au16GainOffset[1] = 256;
    pstAwbSnsDft->au16GainOffset[2] = 256;
    pstAwbSnsDft->au16GainOffset[3] = 497;

    pstAwbSnsDft->as32WbPara[0] = -102;
    pstAwbSnsDft->as32WbPara[1] = 358;
    pstAwbSnsDft->as32WbPara[2] = 0;
    pstAwbSnsDft->as32WbPara[3] = 183957;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -135938;
    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
    }

    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

    return HI_SUCCESS;
}

static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));
    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return HI_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
    {378, 256, 430},
    {439, 256, 439}
};

static HI_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;
    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
    pstDef->unKey.bit1Lsc      = 0;
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
            pstDef->pstDrc                   = &g_stIspDRC;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNr;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGamma;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspGe;
            pstDef->unKey.bit1Dehaze         = 1;
            pstDef->pstDehaze                = &g_stIspDehaze;			
            pstDef->unKey.bit1Lcac           = 0;
            pstDef->pstLcac                  = &g_stIspLCac;			
            pstDef->unKey.bit1Rgbir          = 0;
            memcpy(&pstDef->stNoiseCalibration,   &g_stIspNoiseCalibratio, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMode.u32SensorID = SC3336_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;


    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case SC3336_SENSOR_1296P_30FPS_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
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
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
    pstDef->stSensorMode.bValidDngRawFormat = HI_TRUE;

    return HI_SUCCESS;
}


static HI_S32 cmos_get_isp_black_level(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;
    pstBlackLevel->au16BlackLevel[0] = 0x104;
    pstBlackLevel->au16BlackLevel[1] = 0x104;
    pstBlackLevel->au16BlackLevel[2] = 0x104;
    pstBlackLevel->au16BlackLevel[3] = 0x104;

    return HI_SUCCESS;

}

static HI_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines_5Fps = (SC3336_VMAX_1296P30_LINEAR * 20) / 5;
    u32MaxIntTime_5Fps = u32FullLines_5Fps - EXP_OFFSET;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        smartsens_write_register(ViPipe, SC3336_VMAX_H_ADDR, HIGH_8BITS(u32FullLines_5Fps));		/* 5fps */
        smartsens_write_register(ViPipe, SC3336_VMAX_L_ADDR, LOW_8BITS(u32FullLines_5Fps));			/* 5fps */
        smartsens_write_register(ViPipe, SC3336_EXP_H_ADDR, HIGHER_4BITS(u32MaxIntTime_5Fps));		/* max exposure lines */        
		smartsens_write_register(ViPipe, SC3336_EXP_M_ADDR, HIGHER_8BITS(u32MaxIntTime_5Fps));		/* max exposure lines */
        smartsens_write_register(ViPipe, SC3336_EXP_L_ADDR, LOWER_4BITS(u32MaxIntTime_5Fps));		/* max exposure lines */

        smartsens_write_register(ViPipe, SC3336_DGAIN_H_ADDR, 0x00);
        smartsens_write_register(ViPipe, SC3336_DGAIN_L_ADDR, 0x80);
        smartsens_write_register(ViPipe, SC3336_AGAIN_H_ADDR, 0x00);
        smartsens_write_register(ViPipe, SC3336_AGAIN_L_ADDR, 0x00);
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > (SC3336_FULL_LINES_MAX)) ? (SC3336_FULL_LINES_MAX) : pstSnsState->u32FLStd;
        pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
        smartsens_write_register(ViPipe, SC3336_VMAX_H_ADDR, HIGH_8BITS(pstSnsState->u32FLStd >> 1));	/* Standard FPS */
        smartsens_write_register(ViPipe, SC3336_VMAX_L_ADDR, LOW_8BITS(pstSnsState->u32FLStd >> 1));	/* Standard FPS */
        pstSnsState->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;
        default:
            ISP_TRACE(HI_DBG_ERR, "cmos_set_wdr_mode_NOT support this mode!\n");
            return HI_FAILURE;
    }

    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunSc3336BusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 9;

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = smartsens_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = smartsens_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = smartsens_data_byte;
        }
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr  = SC3336_EXP_H_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr  = SC3336_EXP_M_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr  = SC3336_EXP_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr  = SC3336_DGAIN_H_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr  = SC3336_DGAIN_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr  = SC3336_AGAIN_H_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr  = SC3336_AGAIN_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr  = SC3336_VMAX_H_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum  = 0;
        pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr  = SC3336_VMAX_L_ADDR;
        // pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr  = 0x3812;
        // pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr  = 0x363c;
        // pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr  = 0x5787;
        // pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr  = 0x5788;
        // pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr  = 0x5790;
        // pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr  = 0x5791;
        // pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr  = 0x5799;
        // pstSnsState->astRegsInfo[0].astI2cData[16].u8DelayFrmNum  = 1;
        // pstSnsState->astRegsInfo[0].astI2cData[16].u32RegAddr  = 0x3812;
        pstSnsState->bSyncInit = HI_TRUE;
    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
            } else {

                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            }
        }
		// if ( HI_TRUE == (pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate || pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate || pstSnsState->astRegsInfo[0].astI2cData[15].bUpdate)) {
            // pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate  = HI_TRUE;
            // pstSnsState->astRegsInfo[0].astI2cData[16].bUpdate = HI_TRUE;
			// printf("R5787 = 0x%x, R5799 = 0x%x\n", pstSnsState->astRegsInfo[0].astI2cData[11].u32Data, pstSnsState->astRegsInfo[0].astI2cData[15].u32Data);
        // } else {
            // pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate  = HI_FALSE;
            // pstSnsState->astRegsInfo[0].astI2cData[16].bUpdate = HI_FALSE;
        // }
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    if (pstSensorImageMode->f32Fps <= 20) {
        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (SC3336_RES_IS_1296P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                u8SensorImageMode     = SC3336_SENSOR_1296P_30FPS_LINEAR_MODE;
                pstSnsState->u32FLStd = SC3336_VMAX_1296P30_LINEAR;
            } else {
                ISP_TRACE(HI_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
                return HI_FAILURE;
            }
        } else {
            ISP_TRACE(HI_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps,
                      pstSnsState->enWDRMode);
            return HI_FAILURE;
        }
    } else {}

    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC3336_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit     = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = SC3336_SENSOR_1296P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = SC3336_VMAX_1296P30_LINEAR;
    pstSnsState->au32FL[0] = SC3336_VMAX_1296P30_LINEAR;
    pstSnsState->au32FL[1] = SC3336_VMAX_1296P30_LINEAR;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x00;   // intt
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = 0x53;
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0xe0;
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = 0x00;   // gain
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = 0x80;
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = 0x00;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x00;
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x05;   // vts
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x46;
    // pstSnsState->astRegsInfo[0].astI2cData[9].u32Data  = 0x00;
    // pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x0e;
	// pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x10;
	// pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0x06;
	// pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x10;
	// pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = 0x10;
	// pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = 0x00;
    // pstSnsState->astRegsInfo[0].astI2cData[16].u32Data = 0x30;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = smartsens_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = smartsens_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static HI_S32 sensor_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunSc3336BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    SC3336_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            ISP_TRACE(HI_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    SC3336_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    SC3336_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    SC3336_SENSOR_RESET_CTX(ViPipe);
}

static HI_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    stSnsAttrInfo.eSensorId = SC3336_ID;

    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= HI_MPI_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);

    if (s32Ret != HI_SUCCESS) {
        ISP_TRACE(HI_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= HI_MPI_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);

    if (s32Ret != HI_SUCCESS) {
        ISP_TRACE(HI_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret |= HI_MPI_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);

    if (s32Ret != HI_SUCCESS) {
        ISP_TRACE(HI_DBG_ERR, "sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(ViPipe, SC3336_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_TRACE(HI_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC3336_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_TRACE(HI_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC3336_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_TRACE(HI_DBG_ERR, "sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return HI_SUCCESS;
}

static HI_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    CMOS_CHECK_POINTER(pstInitAttr);

    g_au32InitExposure[ViPipe]  = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[ViPipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe]   = pstInitAttr->u16SampleBgain;

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSmartsensObj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = smartsens_standby,
    .pfnRestart             = smartsens_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = smartsens_write_register,
    .pfnReadReg             = smartsens_read_register,
    .pfnSetBusInfo          = sensor_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif /* End of #ifdef __cplusplus */

#endif /* __SC3336_MIPI_CMOS_H_ */

