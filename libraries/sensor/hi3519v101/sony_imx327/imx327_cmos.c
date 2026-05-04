#if !defined(__IMX327_CMOS_H_)
#define __IMX327_CMOS_H_

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


#define IMX327_ID 327

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
ISP_SNS_STATE_S             g_astimx327[ISP_MAX_DEV_NUM] = {{0}};
static ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_DEV_NUM] = {&g_astimx327[0], &g_astimx327[1]};
ISP_SNS_COMMBUS_U     g_aunImx327BusInfo[ISP_MAX_DEV_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1] = { .s8I2cDev = 1}
}; 
static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_DEV_NUM] = {ISP_FSWDR_NORMAL_MODE,ISP_FSWDR_NORMAL_MODE};
static HI_U32 gu32MaxTimeGetCnt[ISP_MAX_DEV_NUM] = {0,0};

static HI_U32 g_au32InitExposure[ISP_MAX_DEV_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_DEV_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_DEV_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_DEV_NUM] = {0};

extern const unsigned int imx327_i2c_addr;
extern unsigned int imx327_addr_byte;
extern unsigned int imx327_data_byte;

typedef struct hiIMX327_STATE_S
{
    HI_U8       u8Hcg;
    HI_U32      u32BRL;
    HI_U32      u32RHS1_MAX;
    HI_U32      u32RHS2_MAX;
} IMX327_STATE_S;

IMX327_STATE_S g_astimx327State[ISP_MAX_DEV_NUM] = {{0}};

extern void imx327_init(ISP_DEV IspDev);
extern void imx327_exit(ISP_DEV IspDev);
extern void imx327_standby(ISP_DEV IspDev);
extern void imx327_restart(ISP_DEV IspDev);
extern int imx327_write_register(ISP_DEV IspDev, int addr, int data);
extern int imx327_read_register(ISP_DEV IspDev, int addr);

#define IMX327_FULL_LINES_MAX  (0x3FFFF)
#define IMX327_FULL_LINES_MAX_2TO1_WDR  (0x8AA)    // considering the YOUT_SIZE and bad frame
#define IMX327_FULL_LINES_MAX_3TO1_WDR  (0x7FC)

/*****Imx327 Register Address*****/
#define IMX327_SHS1_ADDR (0x3020) 
#define IMX327_SHS2_ADDR (0x3024) 
#define IMX327_SHS3_ADDR (0x3028) 
#define IMX327_GAIN_ADDR (0x3014)
#define IMX327_HCG_ADDR  (0x3009)
#define IMX327_VMAX_ADDR (0x3018)
#define IMX327_HMAX_ADDR (0x301c)
#define IMX327_RHS1_ADDR (0x3030) 
#define IMX327_RHS2_ADDR (0x3034)
#define IMX327_Y_OUT_SIZE_ADDR (0x3418)

#define IMX327_INCREASE_LINES (1) /* make real fps less than stand fps because NVR require*/

#define IMX327_VMAX_1080P30_LINEAR  (1125+IMX327_INCREASE_LINES)
#define IMX327_VMAX_1080P60TO30_WDR (1125+IMX327_INCREASE_LINES)
#define IMX327_VMAX_1080P120TO30_WDR (1125+IMX327_INCREASE_LINES)

//sensor fps mode
#define IMX327_SENSOR_1080P_30FPS_LINEAR_MODE      (1)
#define IMX327_SENSOR_1080P_30FPS_3t1_WDR_MODE     (2)
#define IMX327_SENSOR_1080P_30FPS_2t1_WDR_MODE     (3)

HI_S32 cmos_get_ae_default(ISP_DEV IspDev,AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if (HI_NULL == pstAeSnsDft)
    {
        printf("null pointer when get ae default value!\n");
        return - 1;
    }

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));
      
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50*256;
    pstAeSnsDft->u32FullLinesMax = IMX327_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;
    
    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

    if(g_au32LinesPer500ms[IspDev] == 0)
	{
		pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd*30/2;
	}
	else
	{
		pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[IspDev];
	}


    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch(g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:   /*linear mode*/
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u32MaxAgain = 62564; 
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 38577;  
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = 20013;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
            
            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR; 

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev] ? g_au32InitExposure[IspDev] : 148859;
            
            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 2;
            pstAeSnsDft->u32MinIntTime = 1;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 1;
        break;

        case WDR_MODE_2To1_LINE:                                                                      
            pstAeSnsDft->au8HistThresh[0] = 0xC;                                                      
            pstAeSnsDft->au8HistThresh[1] = 0x18;                                                     
            pstAeSnsDft->au8HistThresh[2] = 0x60;                                                     
            pstAeSnsDft->au8HistThresh[3] = 0x80;                                                     
                                                                                                   
                                 
            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 2;                                                           
            pstAeSnsDft->u32MinIntTime = 2;                                                           
                                                                                       
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;                                                 
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;                            
                                                                                                   
            pstAeSnsDft->u32MaxAgain = 62564; 
            pstAeSnsDft->u32MinAgain = 1024;                                                          
            pstAeSnsDft->u32MaxAgainTarget = 62564;                                                 
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain; 

            pstAeSnsDft->u32MaxDgain = 38577;  
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = 20013;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev] ? g_au32InitExposure[IspDev] : 16462;

            if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
            {
                pstAeSnsDft->u8AeCompensation = 56; 
                pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR; 
            }
            else
            {
                pstAeSnsDft->u32MaxDgainTarget = 8153;
                pstAeSnsDft->u32MaxISPDgainTarget = 267;
                pstAeSnsDft->u8AeCompensation = 24;  
                pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR; 
                pstAeSnsDft->u16ManRatioEnable = HI_TRUE;                                                 
                pstAeSnsDft->au32Ratio[0] = 0x400;  
                pstAeSnsDft->au32Ratio[1] = 0x40; 
                pstAeSnsDft->au32Ratio[2] = 0x40; 
            }
       break;

      case WDR_MODE_3To1_LINE:                                                                      
            pstAeSnsDft->au8HistThresh[0] = 0xC;                                                      
            pstAeSnsDft->au8HistThresh[1] = 0x18;                                                     
            pstAeSnsDft->au8HistThresh[2] = 0x60;                                                     
            pstAeSnsDft->au8HistThresh[3] = 0x80;                                                     
                                 
            pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 2;                                                           
            pstAeSnsDft->u32MinIntTime = 3;                                                           
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;                                                 
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;                            
                                                                                                   
            pstAeSnsDft->u32MaxAgain = 62564; 
            pstAeSnsDft->u32MinAgain = 1024;                                                          
            pstAeSnsDft->u32MaxAgainTarget = 62564;                                                 
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain; 

            pstAeSnsDft->u32MaxDgain = 38577;  
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = 20013;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[IspDev] ? g_au32InitExposure[IspDev] : 7436;

            if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
            {
                pstAeSnsDft->u8AeCompensation = 56; 
                pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
            }
            else
            {
                pstAeSnsDft->u32MaxDgainTarget = 8153;
                pstAeSnsDft->u32MaxISPDgainTarget = 267;
                pstAeSnsDft->u8AeCompensation = 24;  
                pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR; 
                pstAeSnsDft->u16ManRatioEnable = HI_TRUE;                                                 
                pstAeSnsDft->au32Ratio[0] = 0x200;  
                pstAeSnsDft->au32Ratio[1] = 0x200; 
                pstAeSnsDft->au32Ratio[2] = 0x40;
            }
       break; 
    
    }

    return 0;
}


/* the function of sensor set fps */
HI_VOID cmos_fps_set(ISP_DEV IspDev, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

    HI_U32 u32VMAX = IMX327_VMAX_1080P30_LINEAR;                                                                            
                                                                                                                          
    switch (g_apstSnsState[IspDev]->u8ImgMode)                                                                                           
    {
      case IMX327_SENSOR_1080P_30FPS_2t1_WDR_MODE:
           if ((f32Fps <= 30) && (f32Fps >= 16.5))                                                                            
           {
               u32VMAX = IMX327_VMAX_1080P60TO30_WDR * 30 / f32Fps;  
           }
           else                                                                                                              
           {                                                                                                                 
               printf("Not support Fps: %f\n", f32Fps);                                                                      
               return;                                                                                                       
           }
           u32VMAX = (u32VMAX > IMX327_FULL_LINES_MAX_2TO1_WDR) ? IMX327_FULL_LINES_MAX_2TO1_WDR : u32VMAX; 
           break;

     case IMX327_SENSOR_1080P_30FPS_3t1_WDR_MODE:                                                                                  
           if ((f32Fps <= 30) && (f32Fps >= 16.5))                                                                            
           {
               u32VMAX = IMX327_VMAX_1080P120TO30_WDR * 30 / f32Fps;  
           }
           else                                                                                                              
           {                                                                                                                 
               printf("Not support Fps: %f\n", f32Fps);                                                                      
               return;                                                                                                       
           }
           u32VMAX = (u32VMAX > IMX327_FULL_LINES_MAX_3TO1_WDR) ? IMX327_FULL_LINES_MAX_3TO1_WDR : u32VMAX; 
           break;
                
      case IMX327_SENSOR_1080P_30FPS_LINEAR_MODE:
           if ((f32Fps <= 30) && (f32Fps >= 0.5))                                                                            
           {
               u32VMAX = IMX327_VMAX_1080P30_LINEAR * 30 / f32Fps;  
           }
           else                                                                                                              
           {                                                                                                                 
               printf("Not support Fps: %f\n", f32Fps);                                                                      
               return;                                                                                                       
           } 
           u32VMAX = (u32VMAX > IMX327_FULL_LINES_MAX) ? IMX327_FULL_LINES_MAX : u32VMAX;   
           break;
                                                                                                       
      default:                                                                                                              
          return;
          break;                                                                                                            
    }                                                                                                                                                                                          
                                                                                                                          
    if (WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)                                                                                   
    {                                                                                                                     
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = (u32VMAX & 0xFF);                                                         
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = ((u32VMAX & 0xFF00) >> 8);                                                
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = ((u32VMAX & 0xF0000) >> 16);                                              
    }                                                                                                                     
    else                                                                                                                  
    {                                                                                                                     
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32Data = (u32VMAX & 0xFF);                                                         
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32Data = ((u32VMAX & 0xFF00) >> 8);                                                
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32Data = ((u32VMAX & 0xF0000) >> 16);                                             
    }                                                                                                                     
                                                                                                                          
    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                              
    {                                                                                                                     
        g_apstSnsState[IspDev]->u32FLStd = u32VMAX * 2;
        //printf("gu32FullLinesStd:%d\n",g_apstSnsState[IspDev]->u32FLStd);
                                                                                                                          
        /*                                                                                                                
            RHS1 limitation:                                                                                              
            2n + 5                                                                                                        
            RHS1 <= FSC - BRL*2 -21                                                                                       
            (2 * VMAX_IMX327_1080P30_WDR - 2 * gu32BRL - 21) - (((2 * VMAX_IMX327_1080P30_WDR - 2 * 1109 - 21) - 5) %2)   
        */                                                                                                                

        g_astimx327State[IspDev].u32RHS1_MAX = (u32VMAX - g_astimx327State[IspDev].u32BRL) * 2 - 21;
                                                                                                                      
    }

    else if (WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                              
    {                                                                                                                     
        g_apstSnsState[IspDev]->u32FLStd = u32VMAX * 4;
        
        //printf("u32VMAX:%d gu32FullLinesStd:%d\n",u32VMAX,g_apstSnsState[IspDev]->u32FLStd);
                                                                                                                          
        /*                                                                                                                
            RHS2 limitation:                                                                                              
            3n + 14                                                                                                        
            RHS2 <= FSC - BRL*3 -25                                                                                       
        */                                                                                                                
        g_astimx327State[IspDev].u32RHS2_MAX = u32VMAX*4 - g_astimx327State[IspDev].u32BRL * 3 - 25;                                                                                                                                    
                                                                                                                         
    }
    else                                                                                                                  
    {                                                                                                                     
        g_apstSnsState[IspDev]->u32FLStd = u32VMAX;                                                                                       
    }                                                                                                                     
                                                                                                                          
    pstAeSnsDft->f32Fps = f32Fps;                                    
    pstAeSnsDft->u32LinesPer500ms = g_apstSnsState[IspDev]->u32FLStd * f32Fps / 2;                                                        
    pstAeSnsDft->u32FullLinesStd = g_apstSnsState[IspDev]->u32FLStd;  
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->u32FLStd - 2;   
    g_apstSnsState[IspDev]->au32FL[0] = g_apstSnsState[IspDev]->u32FLStd;
    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];
                                                                                                                          
    return;                                                                                                               

}

HI_VOID cmos_slow_framerate_set(ISP_DEV IspDev,HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
    	u32FullLines = (u32FullLines > 2*IMX327_FULL_LINES_MAX_2TO1_WDR) ? 2*IMX327_FULL_LINES_MAX_2TO1_WDR : u32FullLines;
        g_apstSnsState[IspDev]->au32FL[0] = (u32FullLines >> 1) << 1;
		g_astimx327State[IspDev].u32RHS1_MAX = g_apstSnsState[IspDev]->au32FL[0] - g_astimx327State[IspDev].u32BRL * 2 - 21;
    }
    else if(WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
    	u32FullLines = (u32FullLines > 4*IMX327_FULL_LINES_MAX_3TO1_WDR) ? 4*IMX327_FULL_LINES_MAX_3TO1_WDR : u32FullLines;
        g_apstSnsState[IspDev]->au32FL[0] = (u32FullLines >> 2) << 2;
        g_astimx327State[IspDev].u32RHS2_MAX = g_apstSnsState[IspDev]->au32FL[0] - g_astimx327State[IspDev].u32BRL * 3 - 25; 
    }
    else
    {
    	u32FullLines = (u32FullLines > IMX327_FULL_LINES_MAX) ? IMX327_FULL_LINES_MAX : u32FullLines;
        g_apstSnsState[IspDev]->au32FL[0] = u32FullLines;  
    }

    if(WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)
    {
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = (g_apstSnsState[IspDev]->au32FL[0] & 0xFF);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = ((g_apstSnsState[IspDev]->au32FL[0] & 0xFF00) >> 8);
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = ((g_apstSnsState[IspDev]->au32FL[0] & 0xF0000) >> 16);
    }
    else if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                                     
    {                                                                                         
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32Data = ((g_apstSnsState[IspDev]->au32FL[0]>>1) & 0xFF);                       
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32Data = (((g_apstSnsState[IspDev]->au32FL[0]>>1) & 0xFF00) >> 8);              
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32Data = (((g_apstSnsState[IspDev]->au32FL[0]>>1) & 0xF0000) >> 16);           
    }
    else if(WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                                     
    {                                                                                         
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32Data = ((g_apstSnsState[IspDev]->au32FL[0]>>2) & 0xFF);                       
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32Data = (((g_apstSnsState[IspDev]->au32FL[0]>>2) & 0xFF00) >> 8);              
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32Data = (((g_apstSnsState[IspDev]->au32FL[0]>>2) & 0xF0000) >> 16);           
    }
    else                                                                                      
    {                                                                                         
    }   

    pstAeSnsDft->u32FullLines = g_apstSnsState[IspDev]->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = g_apstSnsState[IspDev]->au32FL[0] - 2; 

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
HI_VOID cmos_inttime_update(ISP_DEV IspDev,HI_U32 u32IntTime)
{
    static HI_BOOL bFirst[ISP_MAX_DEV_NUM] ={1, 1}; 
    HI_U32 u32Value = 0;

    static HI_U8 u8Count[ISP_MAX_DEV_NUM] = {0};

    static HI_U32 u32ShortIntTime[ISP_MAX_DEV_NUM] = {0};
    static HI_U32 u32ShortIntTime1[ISP_MAX_DEV_NUM] = {0}; 
    static HI_U32 u32ShortIntTime2[ISP_MAX_DEV_NUM] = {0};
    static HI_U32 u32LongIntTime[ISP_MAX_DEV_NUM] = {0};         

    static HI_U32 u32RHS1[ISP_MAX_DEV_NUM]  = {0}; 
    static HI_U32 u32RHS2[ISP_MAX_DEV_NUM]  = {0};  

    static HI_U32 u32SHS1[ISP_MAX_DEV_NUM]  = {0};                      
    static HI_U32 u32SHS2[ISP_MAX_DEV_NUM]  = {0};
    static HI_U32 u32SHS3[ISP_MAX_DEV_NUM]  = {0};

    HI_U32 u32YOUTSIZE;

    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                          
    {
      //printf("bFirst = %d\n",bFirst[IspDev]);
      if (bFirst[IspDev]) /* short exposure */                                                              
      {
           g_apstSnsState[IspDev]->au32WDRIntTime[0] = u32IntTime;
           u32ShortIntTime[IspDev] = u32IntTime;
            //printf("u32ShortIntTime = %d \n",u32ShortIntTime[IspDev]);
           bFirst[IspDev] = HI_FALSE;  
       }                                                                                             

       else   /* long exposure */                                                                      
       {                                                                                             
           g_apstSnsState[IspDev]->au32WDRIntTime[1] = u32IntTime;  
           u32LongIntTime[IspDev] = u32IntTime;                                                              
           
           u32SHS2[IspDev] = g_apstSnsState[IspDev]->au32FL[1] - u32LongIntTime[IspDev] - 1; 
                                                                                                    
           //allocate the RHS1                                                                       
           u32SHS1[IspDev] = (u32ShortIntTime[IspDev] % 2) + 2;
		   u32RHS1[IspDev] = u32ShortIntTime[IspDev] + u32SHS1[IspDev] + 1; 
                                                     

           u32YOUTSIZE=(1097+(u32RHS1[IspDev]-1)/2+7)*2;
           u32YOUTSIZE=(u32YOUTSIZE>=0x1FFF)?0x1FFF:u32YOUTSIZE;
           //printf("u32ShortIntTime = %d u32SHS1 = %d \n",u32ShortIntTime[IspDev],u32SHS1);
           //printf("IspDev = %d RHS1 = %d u32YOUTSIZE = %d \n",IspDev,u32RHS1[IspDev], u32YOUTSIZE);
                                                                                     
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = (u32SHS1[IspDev] & 0xFF);                                 
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = ((u32SHS1[IspDev] & 0xFF00) >> 8);                        
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = ((u32SHS1[IspDev] & 0xF0000) >> 16);                      
                                                                                                     
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = (u32SHS2[IspDev] & 0xFF);                                 
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = ((u32SHS2[IspDev] & 0xFF00) >> 8);                        
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = ((u32SHS2[IspDev] & 0xF0000) >> 16); 
                                                                                                      
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u32Data = (u32RHS1[IspDev] & 0xFF);                                
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u32Data = ((u32RHS1[IspDev] & 0xFF00) >> 8);                       
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u32Data = ((u32RHS1[IspDev] & 0xF0000) >> 16);  

           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u32Data = (u32YOUTSIZE & 0xFF);                                
           g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u32Data = ((u32YOUTSIZE & 0x1F00) >> 8);
                                                                                              
           bFirst[IspDev] = HI_TRUE;                                                                         
        }                                                                                             
     }   
    

     else if(WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
     {
           if (0 == u8Count[IspDev])        /* short exposure */
           {
               g_apstSnsState[IspDev]->au32WDRIntTime[0] = u32IntTime; 
               u32ShortIntTime1[IspDev] = u32IntTime;
               u8Count[IspDev]++;
           }
           else if (1 == u8Count[IspDev])  /* short short exposure */
           {
               g_apstSnsState[IspDev]->au32WDRIntTime[1] = u32IntTime; 
               u32ShortIntTime2[IspDev] = u32IntTime;
               u8Count[IspDev]++;
           }
           else                    /* long exposure */
           {
              g_apstSnsState[IspDev]->au32WDRIntTime[2] = u32IntTime; 
              u32LongIntTime[IspDev] = u32IntTime;
              u32SHS3[IspDev] = g_apstSnsState[IspDev]->au32FL[1] - u32LongIntTime[IspDev] -1;
   
              //allocate the RHS1 and RHS2  
              u32SHS1[IspDev] = (3 - (u32ShortIntTime2[IspDev] % 3)) + 3;
              u32RHS1[IspDev] = u32ShortIntTime2[IspDev] + u32SHS1[IspDev] + 1;
              u32SHS2[IspDev] = u32RHS1[IspDev] + (3 - (u32ShortIntTime1[IspDev] % 3)) + 3;
              u32RHS2[IspDev] = u32ShortIntTime1[IspDev] + u32SHS2[IspDev] + 1;

              u32YOUTSIZE=(1097+(u32RHS2[IspDev]-2)/3+9)*3;
              u32YOUTSIZE=(u32YOUTSIZE>=0x1FFF)?0x1FFF:u32YOUTSIZE;
   
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = (u32SHS1[IspDev] & 0xFF);                                 
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = ((u32SHS1[IspDev] & 0xFF00) >> 8);                        
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = ((u32SHS1[IspDev] & 0xF0000) >> 16);                      
                                                                                                        
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32Data = (u32SHS2[IspDev] & 0xFF);                                 
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32Data = ((u32SHS2[IspDev] & 0xFF00) >> 8);                        
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32Data = ((u32SHS2[IspDev] & 0xF0000) >> 16); 
                                                                                                       
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u32Data = (u32RHS1[IspDev] & 0xFF);                                
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u32Data = ((u32RHS1[IspDev] & 0xFF00) >> 8);                       
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u32Data = ((u32RHS1[IspDev] & 0xF0000) >> 16);
    
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u32Data = (u32RHS2[IspDev] & 0xFF);                                
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u32Data = ((u32RHS2[IspDev] & 0xFF00) >> 8);                       
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[16].u32Data = ((u32RHS2[IspDev] & 0xF0000) >> 16);
    
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[17].u32Data = (u32SHS3[IspDev] & 0xFF);                                
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[18].u32Data = ((u32SHS3[IspDev] & 0xFF00) >> 8);                       
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[19].u32Data = ((u32SHS3[IspDev] & 0xF0000) >> 16);
    
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[20].u32Data = (u32YOUTSIZE & 0xFF);                                
              g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[21].u32Data = ((u32YOUTSIZE & 0x1F00) >> 8);
                
              u8Count[IspDev] = 0;
           }
     }
     else                                                                                             
     {                                                                                                 
         u32Value = g_apstSnsState[IspDev]->au32FL[0] - u32IntTime - 1; 
                                                                                                       
         g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32Data = (u32Value & 0xFF);                                    
         g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32Data = ((u32Value & 0xFF00) >> 8);                           
         g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32Data = ((u32Value & 0x30000) >> 16); 
         
         bFirst[IspDev] = HI_TRUE;                                                                             
     }                                                                                                 
                                                                                                    
  return;                                                                                           

}

static HI_U32 gain_table[262]=
{
    1024,1059,1097,1135,1175,1217,1259,1304,1349,1397,1446,1497,1549,1604,1660,1719,1779,1842,1906,
    1973,2043,2048,2119,2194,2271,2351,2434,2519,2608,2699,2794,2892,2994,3099,3208,3321,3438,3559,
    3684,3813,3947,4086,4229,4378,4532,4691,4856,5027,5203,5386,5576,5772,5974,6184,6402,6627,6860,
    7101,7350,7609,7876,8153,8439,8736,9043,9361,9690,10030,10383,10748,11125,11516,11921,12340,12774,
    13222,13687,14168,14666,15182,15715,16267,16839,17431,18043,18677,19334,20013,20717,21445,22198,
    22978,23786,24622,25487,26383,27310,28270,29263,30292,31356,32458,33599,34780,36002,37267,38577,
    39932,41336,42788,44292,45849,47460,49128,50854,52641,54491,56406,58388,60440,62564,64763,67039,
    69395,71833,74358,76971,79676,82476,85374,88375,91480,94695,98023,101468,105034,108725,112545,
    116501,120595,124833,129220,133761,138461,143327,148364,153578,158975,164562,170345,176331,182528,
    188942,195582,202455,209570,216935,224558,232450,240619,249074,257827,266888,276267,285976,296026,
    306429,317197,328344,339883,351827,364191,376990,390238,403952,418147,432842,448053,463799,480098,
    496969,514434,532512,551226,570597,590649,611406,632892,655133,678156,701988,726657,752194,778627,
    805990,834314,863634,893984,925400,957921,991585,1026431,1062502,1099841,1138491,1178500,1219916,
    1262786,1307163,1353100,1400651,1449872,1500824,1553566,1608162,1664676,1723177,1783733,1846417,
    1911304,1978472,2048000,2119971,2194471,2271590,2351418,2434052,2519590,2608134,2699789,2794666,
    2892876,2994538,3099773,3208706,3321467,3438190,3559016,3684087,3813554,3947571,4086297,4229898,
    4378546,4532417,4691696,4856573,5027243,5203912,5386788,5576092,5772048,5974890,6184861,6402210,
    6627198,6860092,7101170,7350721,7609041,7876439,8153234
};

HI_VOID cmos_again_calc_table(ISP_DEV IspDev,HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    if (*pu32AgainLin >= gain_table[120])
    {
         *pu32AgainLin = gain_table[120];
         *pu32AgainDb = 120;
         return ;
    }
    
    for (i = 1; i < 121; i++)
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

HI_VOID cmos_dgain_calc_table(ISP_DEV IspDev,HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    int i;

    if((HI_NULL == pu32DgainLin) ||(HI_NULL == pu32DgainDb))
    {
        printf("null pointer when get ae sensor gain info value!\n");
        return;
    }

    if (*pu32DgainLin >= gain_table[106])
    {
         *pu32DgainLin = gain_table[106];
         *pu32DgainDb = 106;
         return ;
    }
    
    for (i = 1; i < 106; i++)
    {
        if (*pu32DgainLin < gain_table[i])
        {
            *pu32DgainLin = gain_table[i - 1];
            *pu32DgainDb = i - 1;
            break;
        }
    }

    return;
}

HI_VOID cmos_gains_update(ISP_DEV IspDev,HI_U32 u32Again, HI_U32 u32Dgain)
{  
    HI_U32 u32HCG = g_astimx327State[IspDev].u8Hcg;
    HI_U32 u32Tmp;
    
    if(u32Again >= 21)
    {
        u32HCG = u32HCG | 0x10;  // bit[4] HCG  .Reg0x3009[7:0]
        u32Again = u32Again - 21;
    }

    u32Tmp=u32Again+u32Dgain;
        
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32Data = (u32Tmp & 0xFF);
    g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32Data = (u32HCG & 0xFF);
    

    return;
}

HI_VOID cmos_get_inttime_max(ISP_DEV IspDev,HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i = 0;
    HI_U32 u32IntTimeMaxTmp0 = 0;
    HI_U32 u32IntTimeMaxTmp  = 0;
    HI_U32 u32RHS2_Max=0;
    HI_U32 u32RatioTmp = 0x40;
    HI_U32 u32ShortTimeMinLimit = 0;

    u32ShortTimeMinLimit = (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode) ? 2 : ((WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode) ? 3 : 2);
        
    if((HI_NULL == au32Ratio) || (HI_NULL == au32IntTimeMax) || (HI_NULL == au32IntTimeMin))
    {
        printf("null pointer when get ae sensor ExpRatio/IntTimeMax/IntTimeMin value!\n");
        return;
    }                                                                                                                       
    
    if(WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                             
    {                                                                                                                        
        /*  limitation for line base WDR                                                                                     
                                                                                                                             
            SHS1 limitation:                                                                                                 
            2 or more                                                                                                        
            RHS1 - 2 or less                                                                                                 
                                                                                                                             
            SHS2 limitation:                                                                                                 
            RHS1 + 2 or more                                                                                                 
            FSC - 2 or less                                                                                                  
                                                                                                                             
            RHS1 Limitation                                                                                                  
            2n + 5 (n = 0,1,2...)                                                                                            
            RHS1 <= FSC - BRL * 2 - 21                                                                                       
                                                                                                                             
            short exposure time = RHS1 - (SHS1 + 1) <= RHS1 - 3                                                              
            long exposure time = FSC - (SHS2 + 1) <= FSC - (RHS1 + 3)                                                        
            ExposureShort + ExposureLong <= FSC - 6                                                                          
            short exposure time <= (FSC - 6) / (ratio + 1)                                                                   
        */
        if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
        {
            u32IntTimeMaxTmp0 = g_apstSnsState[IspDev]->au32FL[1] - 6 - g_apstSnsState[IspDev]->au32WDRIntTime[0];
            u32IntTimeMaxTmp = g_apstSnsState[IspDev]->au32FL[0] - 10;
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
            au32IntTimeMax[0] = u32IntTimeMaxTmp;
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            return;
        }
        else
        {
            u32IntTimeMaxTmp0 = ((g_apstSnsState[IspDev]->au32FL[1] - 6 - g_apstSnsState[IspDev]->au32WDRIntTime[0]) * 0x40)  / DIV_0_TO_1(au32Ratio[0]);                                                                                                                     
            u32IntTimeMaxTmp = ((g_apstSnsState[IspDev]->au32FL[0] - 6) * 0x40)  / DIV_0_TO_1(au32Ratio[0] + 0x40); 
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp > (g_astimx327State[IspDev].u32RHS1_MAX- 3))? (g_astimx327State[IspDev].u32RHS1_MAX - 3) : u32IntTimeMaxTmp; 
            u32IntTimeMaxTmp = (0 == u32IntTimeMaxTmp)? 1: u32IntTimeMaxTmp;  
        }
        
    } 
    else if(WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                             
    {                                                                                                                        
        /*  limitation for DOL 3t1                                                                                     
                                                                                                                             
            SHS1 limitation:                                                                                                 
            3 or more                                                                                                        
            RHS1 - 2 or less 

            RHS1 Limitation                                                                                                  
            3n + 7 (n = 0,1,2...)                                                                                            
                                                                                                                            
            SHS2 limitation:                                                                                                 
            RHS1 + 3 or more                                                                                                 
            RHS2 - 2 or less 
                                                                                                                             
            RHS2 Limitation                                                                                                  
            3n + 14 (n = 0,1,2...)                                                                                            
            RHS2 <= FSC - BRL * 3 - 25  
            
            SHS3 limitation:                                                                                                 
            RHS2 + 3 or more                                                                                                 
            FSC - 2 or less 
                                                                                                                             
            short exposure time 1 = RHS1 - (SHS1 + 1) <= RHS1 - 4
            short exposure time 2 = RHS2 - (SHS2 + 1) <= RHS2 - (RHS1 + 4) 
            short exposure time 2 <= (RHS2 - 8) / (ratio[0] + 1)
            long exposure time = FSC - (SHS3 + 1) <= FSC - (RHS2 + 4)                                                        
            short exposure time 1 + short exposure time 2 + long exposure time <= FSC -12                                                                         
            short exposure time 2 <= (FSC - 12) / (ratio[0]*ratio[1] + ratio[0] + 1)                                                                   
        */
        if(ISP_FSWDR_LONG_FRAME_MODE == genFSWDRMode[IspDev])
        {
            /* when change LongFrameMode, the first 2 frames must limit the MaxIntTime to avoid flicker */
            if(gu32MaxTimeGetCnt[IspDev] < 2)
            {
                u32IntTimeMaxTmp0 = g_apstSnsState[IspDev]->au32FL[1] - 100 - g_apstSnsState[IspDev]->au32WDRIntTime[0] - g_apstSnsState[IspDev]->au32WDRIntTime[1];
            }
            else
            {
                u32IntTimeMaxTmp0 = g_apstSnsState[IspDev]->au32FL[1] - 16 - g_apstSnsState[IspDev]->au32WDRIntTime[0] - g_apstSnsState[IspDev]->au32WDRIntTime[1];
            }
            u32IntTimeMaxTmp = g_apstSnsState[IspDev]->au32FL[0] - 24;
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
            au32IntTimeMax[0] = u32IntTimeMaxTmp;
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            gu32MaxTimeGetCnt[IspDev]++;
            return;
        }
        else
        {
            u32IntTimeMaxTmp0 = ((g_apstSnsState[IspDev]->au32FL[1] - 16 - g_apstSnsState[IspDev]->au32WDRIntTime[0] - g_apstSnsState[IspDev]->au32WDRIntTime[1]) * 0x40*0x40)  / (au32Ratio[0]*au32Ratio[1]);                                                                                                                      
            u32IntTimeMaxTmp = ((g_apstSnsState[IspDev]->au32FL[0] - 16) * 0x40*0x40)  / (au32Ratio[0]*au32Ratio[1] + au32Ratio[0]*0x40 + 0x40*0x40); 
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
            u32RHS2_Max = ((g_astimx327State[IspDev].u32RHS2_MAX-8)*0x40)/(au32Ratio[0]+0x40);
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp > u32RHS2_Max)? (u32RHS2_Max) : u32IntTimeMaxTmp; 
            u32IntTimeMaxTmp = (0 == u32IntTimeMaxTmp)? 1: u32IntTimeMaxTmp; 
        }

    }
    else
    {
    }

    if(u32IntTimeMaxTmp >= u32ShortTimeMinLimit)
    {
        if (IS_LINE_WDR_MODE(g_apstSnsState[IspDev]->enWDRMode))
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

            if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
            {
                u32RatioTmp = 0xFFF;
                au32IntTimeMax[0] = u32IntTimeMaxTmp;
                au32IntTimeMax[1] = au32IntTimeMax[0] * u32RatioTmp >> 6;
            }
            else if (WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
            {
                for(i = 0x40; i <= 0xFFF; i++)
                {
                    if((u32IntTimeMaxTmp + (u32IntTimeMaxTmp*i >> 6) + (u32IntTimeMaxTmp*i*i >> 12)) > (g_apstSnsState[IspDev]->au32FL[0] - 12))
                    {
                        u32RatioTmp = i - 1;
                        break;
                    }
                }
                au32IntTimeMax[0] = u32IntTimeMaxTmp;
                au32IntTimeMax[1] = au32IntTimeMax[0] * u32RatioTmp >> 6;
                au32IntTimeMax[2] = au32IntTimeMax[1] * u32RatioTmp >> 6;
            }
            else
            {
            }
            au32IntTimeMin[0] = au32IntTimeMax[0];
            au32IntTimeMin[1] = au32IntTimeMax[1];
            au32IntTimeMin[2] = au32IntTimeMax[2];
            au32IntTimeMin[3] = au32IntTimeMax[3];
        }
    }

    return;
                                                                                                                
}

/* Only used in LINE_WDR mode */
HI_VOID cmos_ae_fswdr_attr_set(ISP_DEV IspDev,AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    genFSWDRMode[IspDev] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[IspDev] = 0;
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
    pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max; 
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set; 

    return 0;
}

static AWB_CCM_S g_stAwbCcm =
{  
   4900,
   {
      0x1CE, 0x80BF, 0x800F,
      0x8052,0x184,  0x8032,
      0x5,   0x80D1, 0x1CC
   },
   3770,
   {
      0x1CB,  0x80AF, 0x801C,
      0x806D, 0x18C,  0x801F,
      0x11,   0x80EE, 0x1DD
   }, 
   2640,
   {     
      0x1B4,  0x80AA, 0x800A,
      0x8091, 0x1A1,  0x8010,
      0x0,    0x80DB, 0x1DB
   }     
};

static AWB_CCM_S g_stAwbCcmWDR =
{  
   4900,
   {
      0x1CE, 0x80BF, 0x800F,
      0x8052,0x184,  0x8032,
      0x5,   0x80D1, 0x1CC
   },
   3770,
   {
      0x1CB,  0x80AF, 0x801C,
      0x806D, 0x18C,  0x801F,
      0x11,   0x80EE, 0x1DD
   }, 
   2640,
   {     
      0x1B4,  0x80AA, 0x800A,
      0x8091, 0x1A1,  0x8010,
      0x0,    0x80DB, 0x1DB
   }     
};

static AWB_AGC_TABLE_S g_stAwbAgcTable =
{
    /* bvalid */
    1,
    
    /*1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768*/
    /* saturation */   
    {0x80,0x7a,0x78,0x74,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38}
};

static AWB_AGC_TABLE_S g_stAwbAgcTableFSWDR =
{
    /* bvalid */
    1,

    /* saturation */ 
    {0x78,0x78,0x6e,0x64,0x5E,0x58,0x50,0x48,0x40,0x38,0x38,0x38,0x38,0x38,0x38,0x38}
};

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0   
#define GOLDEN_BGAIN 0  

HI_S32 cmos_get_awb_default(ISP_DEV IspDev,AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    if (HI_NULL == pstAwbSnsDft)
    {
        printf("null pointer when get awb default value!\n");
        return -1;
    }

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 4900;

    pstAwbSnsDft->au16GainOffset[0] = 0x1C3;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100;
    pstAwbSnsDft->au16GainOffset[3] = 0x1D4;

    pstAwbSnsDft->as32WbPara[0] = -37;
    pstAwbSnsDft->as32WbPara[1] = 293;
    pstAwbSnsDft->as32WbPara[2] = 0;
    pstAwbSnsDft->as32WbPara[3] = 179537;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -123691;
   
    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;
    
    switch (g_apstSnsState[IspDev]->enWDRMode)
    {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
        break;
        case WDR_MODE_2To1_LINE:
        case WDR_MODE_3To1_LINE:
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmWDR, sizeof(AWB_CCM_S));
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

HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
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


static ISP_CMOS_BAYER_SHARPEN_S g_stIspBayerSharpenFSWDR = 
{
    /* bvalid */
    1,

    /* ShpAlgSel = 1 is Demosaic SharpenEx, else Demosaic sharpen. */ 
    0,
    
    /* sharpen_alt_d to Sharpen */
    {0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x40,0x30,0x20,0x10},
        
    /* sharpen_alt_ud to Sharpen */
    {0x48,0x46,0x42,0x40,0x40,0x40,0x40,0x40,0x40,0x3c,0x38,0x32,0x28,0x20,0x10,0x10},
        
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
     {80,   80,    80,    80,    80,    80,     90,     90,     90,   100,   100,    100,    100,    100,    110,   110},
         
     /* EdgeThd */
     {0,     0,     5,    10,    10,    10,     16,     20,     30,   40,     50,    50,      60,    60,     60,    60},
    
     /* JagCtrl */
     {63,   38,    20,    18,    16,    12,     10,      8,      8,     4,     4,      4,      2,      2,      2,     2},
    
     /* SaltCtrl */
     {50,   50,     50,     90,     90,     90,     90,     90,     90,     90,     90,     90,     90,     50,     50,    50},
    
     /* PepperCtrl */
     {0,    0,       0,     20,     60,     60,     60,     80,    120,    160,    180,     180,   180,     180,   180,   180},
    
     /* DetailCtrl */
     {150,  150,   135,    135,    130,    130,    130,    130,    120,    120,    120,     120,    100,     50,    50,    50}, 
    
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


static ISP_CMOS_YUV_SHARPEN_S g_stIspYuvSharpenFSWDR = 
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
     {32,   36,    40,    48,    48,    48,     44,     40,     40,    32,    32,     28,     25,     20,     20,    20},
        
     /* UnderShoot */
     {32,   36,    40,    48,    48,    48,     44,     40,     40,    32,    32,     28,     25,     20,     20,    20},
         
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
         {65,    65,     65,     65,     65,     65,     70,     75,     80,     80,     80,     80,     80,     80,     80,     80}, /* LumaThd2 */
         {90,    90,     90,     90,     90,     90,     100,   110,    120,    120,    120,    120,    120,    120,    120,    120}  /* LumaThd3 */  
     }, 
    
     /* LumaWgt */
     {
         {160,   160,    160,    150,    120,     80,     40,     20,     10,    10,     10,     10,     10,     10,     10,     10},
         {200,   200,    200,    180,    160,    120,     80,     60,     40,    40,     40,     40,     40,     40,     40,     40},
         {240,   240,    240,    200,    200,    180,    160,    140,    128,   128,    128,    128,    128,    128,    128,    128},
         {255,   255,    255,    255,    255,    255,    255,    255,    255,   255,    255,    255,    255,    255,    255,    255},
     } 
};


static ISP_CMOS_AGC_TABLE_S g_stIspAgcTableFSWDR =
{
    /* bvalid */
    1,
        
    /* snr_thresh */
    {0x8,0xC,0x10,0x14,0x18,0x20,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30},
        
    /* demosaic_np_offset */
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30},
        
    /* ge_strength */
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37,0x37},
};

static ISP_CMOS_NOISE_TABLE_S g_stIspNoiseTable =
{
    /* bvalid */
    1,
    
    /* nosie_profile_weight_lut */
    {
      0,0,0,0,0,0,0,8,14,18,21,24,26,27,28,30,31,32,33,34,34,36,36,37,38,38,39,39,
      40,40,41,41,42,42,42,43,43,44,44,44,45,45,45,46,46,46,46,47,47,47,48,48,48,48,49,49,49,
      49,49,50,50,50,50,51,51,51,51,51,51,52,52,52,52,52,53,53,53,53,53,53,53,54,54,54,54,54,
      54,55,55,55,55,55,55,55,55,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,57,57,58,58,58,
      58,58,58,58,58,58,59,59,59,59,59,59,59
    },

    /* demosaic_weight_lut */
    {
      0,8,14,18,21,24,26,27,28,30,31,32,33,34,34,36,36,37,38,38,39,39,40,40,41,41,42,42,42,
      43,43,44,44,44,45,45,45,46,46,46,46,47,47,47,48,48,48,48,49,49,49,49,49,50,50,50,50,
      51,51,51,51,51,51,52,52,52,52,52,53,53,53,53,53,53,53,54,54,54,54,54,54,55,55,55,55,
      55,55,55,55,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,57,57,58,58,58,58,58,58,58,
      58,58,59,59,59,59,59,59,59,59,59,59,59,59,59
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
        0,2,15,20,24,27,30,32,33,35,36,37,38,39,40,41,42,42,43,44,44,45,45,46,
        46,47,47,48,48,49,49,49,50,50,50,51,51,51,52,52,52,53,53,53,53,54,54,54,54,55,55,
        55,55,56,56,56,56,56,57,57,57,57,57,58,58,58,58,58,59,59,59,59,59,59,59,60,60,60,
        60,60,60,61,61,61,61,61,61,61,62,62,62,62,62,62,62,62,63,63,63,63,63,63,63,63,63,
        64,64,64,64,64,64,64,64,64,65,65,65,65,65,65,65,65,65,65,65,65,65,65
    }
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

HI_U32 cmos_get_isp_default(ISP_DEV IspDev,ISP_CMOS_DEFAULT_S *pstDef)
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
            pstDef->stDrc.u32SlopeMin           = 0x0;
            pstDef->stDrc.u32VarianceSpace      = 0x4;
            pstDef->stDrc.u32VarianceIntensity  = 0x2;
            pstDef->stDrc.u32Asymmetry          = 0x08;
            pstDef->stDrc.u32BrightEnhance      = 0xE6;     
            pstDef->stDrc.bFilterMux            = 0x1;
            pstDef->stDrc.u32Svariance          = 0x6;
            pstDef->stDrc.u32BrightPr           = 0xA0;
            pstDef->stDrc.u32Contrast           = 0xA0;
            pstDef->stDrc.u32DarkEnhance        = 0x9000;
            
            memcpy(&pstDef->stAgcTbl, &g_stIspAgcTable, sizeof(ISP_CMOS_AGC_TABLE_S));
            memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTable, sizeof(ISP_CMOS_NOISE_TABLE_S));            
            memcpy(&pstDef->stDemosaic, &g_stIspDemosaic, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpen, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpen, sizeof(ISP_CMOS_YUV_SHARPEN_S));
            memcpy(&pstDef->stGamma, &g_stIspGamma, sizeof(ISP_CMOS_GAMMA_S));
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
            pstDef->stDrc.u32BrightEnhance      = 0xE6;     
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
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpenFSWDR, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpenFSWDR, sizeof(ISP_CMOS_YUV_SHARPEN_S));          
            memcpy(&pstDef->stGamma, &g_stIspGammaFSWDR, sizeof(ISP_CMOS_GAMMA_S));
        break;

        case WDR_MODE_3To1_LINE:
            pstDef->stDrc.bEnable               = HI_TRUE;
            pstDef->stDrc.u32BlackLevel         = 0x00;
            pstDef->stDrc.u32WhiteLevel         = 0xB0000; 
            pstDef->stDrc.u32SlopeMax           = 0x60;
            pstDef->stDrc.u32SlopeMin           = 0xD0;
            pstDef->stDrc.u32VarianceSpace      = 0x06;
            pstDef->stDrc.u32VarianceIntensity  = 0x08;
            pstDef->stDrc.u32Asymmetry          = 0x08;
            pstDef->stDrc.u32BrightEnhance      = 0xE6;     
            pstDef->stDrc.bFilterMux            = 0x1;
            pstDef->stDrc.u32Svariance          = 0xC;
            pstDef->stDrc.u32BrightPr           = 0x94;
            pstDef->stDrc.u32Contrast           = 0x80;
            pstDef->stDrc.u32DarkEnhance        = 0x6000;

            pstDef->stWDRAttr.au32ExpRatio[0]   = 0x200;
            pstDef->stWDRAttr.au32ExpRatio[1]   = 0x200;
            pstDef->stWDRAttr.au32ExpRatio[2]   = 0x40;

            memcpy(&pstDef->stAgcTbl, &g_stIspAgcTableFSWDR, sizeof(ISP_CMOS_AGC_TABLE_S));
            memcpy(&pstDef->stNoiseTbl, &g_stIspNoiseTableFSWDR, sizeof(ISP_CMOS_NOISE_TABLE_S));            
            memcpy(&pstDef->stDemosaic, &g_stIspDemosaicFSWDR, sizeof(ISP_CMOS_DEMOSAIC_S));
            memcpy(&pstDef->stBayerSharpen, &g_stIspBayerSharpenFSWDR, sizeof(ISP_CMOS_BAYER_SHARPEN_S));
            memcpy(&pstDef->stYuvSharpen, &g_stIspYuvSharpenFSWDR, sizeof(ISP_CMOS_YUV_SHARPEN_S));          
            memcpy(&pstDef->stGamma, &g_stIspGammaFSWDR, sizeof(ISP_CMOS_GAMMA_S));
        break;

    }

    pstDef->stSensorMaxResolution.u32MaxWidth  = 1920;
    pstDef->stSensorMaxResolution.u32MaxHeight = 1080;
    pstDef->stSensorMode.u32SensorID = IMX327_ID;
    pstDef->stSensorMode.u8SensorMode = g_apstSnsState[IspDev]->u8ImgMode;

    return 0;
}

HI_U32 cmos_get_isp_black_level(ISP_DEV IspDev,ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    
    if (HI_NULL == pstBlackLevel)
    {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    /* black level of linear mode */
    if (WDR_MODE_NONE == g_apstSnsState[IspDev]->enWDRMode)
    {
        for (i=0; i<4; i++)
        {
            pstBlackLevel->au16BlackLevel[i] = 0x3c;    // 240
        }
    }

    /* black level of DOL mode */
    else
    {
        pstBlackLevel->au16BlackLevel[0] = 0xF0;
        pstBlackLevel->au16BlackLevel[1] = 0xF0;
        pstBlackLevel->au16BlackLevel[2] = 0xF0;
        pstBlackLevel->au16BlackLevel[3] = 0xF0;
    }
          

    return 0;  
    
}

HI_VOID cmos_set_pixel_detect(ISP_DEV IspDev,HI_BOOL bEnable)
{
    
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    
    if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        return;
    }

    else if(WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)
    {
        return;
    }

    else
    {
        if (IMX327_SENSOR_1080P_30FPS_LINEAR_MODE == g_apstSnsState[IspDev]->u8ImgMode)
        {
            u32FullLines_5Fps = (IMX327_VMAX_1080P30_LINEAR * 30) / 5;
        }
        
        else
        {
            return;
        }
    }

    //u32FullLines_5Fps = (u32FullLines_5Fps > IMX327_FULL_LINES_MAX) ? IMX327_FULL_LINES_MAX : u32FullLines_5Fps;
    u32MaxIntTime_5Fps = 4;

    if (bEnable) /* setup for ISP pixel calibration mode */
    {
        imx327_write_register (IspDev,IMX327_GAIN_ADDR,0x00);
        
        imx327_write_register (IspDev,IMX327_VMAX_ADDR, u32FullLines_5Fps & 0xFF); 
        imx327_write_register (IspDev,IMX327_VMAX_ADDR + 1, (u32FullLines_5Fps & 0xFF00) >> 8); 
        imx327_write_register (IspDev,IMX327_VMAX_ADDR + 2, (u32FullLines_5Fps & 0xF0000) >> 16);

        imx327_write_register (IspDev,IMX327_SHS1_ADDR, u32MaxIntTime_5Fps & 0xFF);
        imx327_write_register (IspDev,IMX327_SHS1_ADDR + 1,  (u32MaxIntTime_5Fps & 0xFF00) >> 8); 
        imx327_write_register (IspDev,IMX327_SHS1_ADDR + 2, (u32MaxIntTime_5Fps & 0xF0000) >> 16); 
          
    }
    else /* setup for ISP 'normal mode' */
    {
        g_apstSnsState[IspDev]->u32FLStd = (g_apstSnsState[IspDev]->u32FLStd > 0x1FFFF) ? 0x1FFFF : g_apstSnsState[IspDev]->u32FLStd;
        imx327_write_register (IspDev,IMX327_VMAX_ADDR, g_apstSnsState[IspDev]->u32FLStd & 0xFF); 
        imx327_write_register (IspDev,IMX327_VMAX_ADDR + 1, (g_apstSnsState[IspDev]->u32FLStd & 0xFF00) >> 8); 
        imx327_write_register (IspDev,IMX327_VMAX_ADDR + 2, (g_apstSnsState[IspDev]->u32FLStd & 0xF0000) >> 16);
        g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    }

    return;
}

HI_VOID cmos_set_wdr_mode(ISP_DEV IspDev,HI_U8 u8Mode)                                          
{                                                                                
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;                                                          
                                                                                 
    switch(u8Mode)                                                               
    {                                                                            
        case WDR_MODE_NONE:                                                      
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;                                       
            g_apstSnsState[IspDev]->u32FLStd = IMX327_VMAX_1080P30_LINEAR;
            g_apstSnsState[IspDev]->u8ImgMode = IMX327_SENSOR_1080P_30FPS_LINEAR_MODE;
            g_astimx327State[IspDev].u8Hcg = 0x2;
            printf("linear mode\n");                                             
        break;                                                                   
                                                                                 
        case WDR_MODE_2To1_LINE:                                                 
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_2To1_LINE;                                  
            g_apstSnsState[IspDev]->u32FLStd = (IMX327_VMAX_1080P60TO30_WDR * 60) / 25;
            g_apstSnsState[IspDev]->u8ImgMode = IMX327_SENSOR_1080P_30FPS_2t1_WDR_MODE;
            g_astimx327State[IspDev].u32BRL = 1109;
            g_astimx327State[IspDev].u8Hcg = 0x1;
            printf("2to1 line WDR 1080p mode(60fps->30fps)\n");
       break;                                                                   


        case WDR_MODE_3To1_LINE:                                                 
            g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_3To1_LINE;                                  
            g_apstSnsState[IspDev]->u32FLStd = (IMX327_VMAX_1080P120TO30_WDR * 120) / 25; 
            g_apstSnsState[IspDev]->u8ImgMode = IMX327_SENSOR_1080P_30FPS_3t1_WDR_MODE;
            g_astimx327State[IspDev].u32BRL = 1109;
            g_astimx327State[IspDev].u8Hcg = 0x0;
            printf("3to1 line WDR 1080p mode(120fps->30fps)\n");
        break; 
                                                                                 
        default:                                                                 
            printf("NOT support this mode!\n");                                  
            return;                                                              
        break;                                                                   
    }  

    g_apstSnsState[IspDev]->au32FL[0]= g_apstSnsState[IspDev]->u32FLStd;
    g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];
    memset(g_apstSnsState[IspDev]->au32WDRIntTime, 0, sizeof(g_apstSnsState[IspDev]->au32WDRIntTime));
    
    return;                                                                      
}                                                                                

HI_U32 cmos_get_sns_regs_info(ISP_DEV IspDev,ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
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
        g_apstSnsState[IspDev]->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx327BusInfo[IspDev].s8I2cDev;        
        g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum = 8;

        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                
        {                                                                                                       
           g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum += 8; 
           g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;                                                            
        }                                                                                                       

        else if ((WDR_MODE_2To1_FRAME_FULL_RATE == g_apstSnsState[IspDev]->enWDRMode) || (WDR_MODE_2To1_FRAME == g_apstSnsState[IspDev]->enWDRMode))    
        {                                                                                                       
           g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum += 3;                                                                     
           g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;                                                            
        }

        else if (WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                                
        {                                                                                                       
           g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum += 14;
           g_apstSnsState[IspDev]->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;                                                            
        } 


        for (i=0; i<g_apstSnsState[IspDev]->astRegsInfo[0].u32RegNum; i++)
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u8DevAddr = imx327_i2c_addr;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32AddrByteNum = imx327_addr_byte;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[i].u32DataByteNum = imx327_data_byte;
        }

        //Linear Mode Regs
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX327_SHS1_ADDR;       
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX327_SHS1_ADDR + 1;        
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX327_SHS1_ADDR + 2;   
     
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;       //make shutter and gain effective at the same time
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX327_GAIN_ADDR;  //gain     
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1;       
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX327_HCG_ADDR;    
        
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX327_VMAX_ADDR;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX327_VMAX_ADDR + 1;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX327_VMAX_ADDR + 2;

        //DOL 2t1 Mode Regs
        if (WDR_MODE_2To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                               
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
            
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX327_SHS2_ADDR;                                              
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX327_SHS2_ADDR + 1;                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX327_SHS2_ADDR + 2; 
                                                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX327_VMAX_ADDR;                                              
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX327_VMAX_ADDR + 1;                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX327_VMAX_ADDR + 2; 
                                                                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u32RegAddr = IMX327_RHS1_ADDR;                                             
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u32RegAddr = IMX327_RHS1_ADDR + 1;                                         
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u32RegAddr = IMX327_RHS1_ADDR + 2; 

            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u32RegAddr = IMX327_Y_OUT_SIZE_ADDR;                                             
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u32RegAddr = IMX327_Y_OUT_SIZE_ADDR + 1; 

         }  

        //DOL 3t1 Mode Regs
        else if (WDR_MODE_3To1_LINE == g_apstSnsState[IspDev]->enWDRMode)                                                               
        {
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
                       
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX327_SHS2_ADDR;                                              
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX327_SHS2_ADDR + 1;                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX327_SHS2_ADDR + 2; 
                                                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX327_VMAX_ADDR;                                              
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX327_VMAX_ADDR + 1;                                          
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX327_VMAX_ADDR + 2; 
                                                                                   
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[11].u32RegAddr = IMX327_RHS1_ADDR;                                             
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[12].u32RegAddr = IMX327_RHS1_ADDR + 1;                                         
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[13].u32RegAddr = IMX327_RHS1_ADDR + 2; 

            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[14].u32RegAddr = IMX327_RHS2_ADDR;                                             
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[15].u32RegAddr = IMX327_RHS2_ADDR + 1;                                         
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[16].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[16].u32RegAddr = IMX327_RHS2_ADDR + 2; 

            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[17].u8DelayFrmNum = 0;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[17].u32RegAddr = IMX327_SHS3_ADDR;                                             
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[18].u8DelayFrmNum = 0;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[18].u32RegAddr = IMX327_SHS3_ADDR + 1;                                         
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[19].u8DelayFrmNum = 0;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[19].u32RegAddr = IMX327_SHS3_ADDR + 2; 

            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[20].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[20].u32RegAddr = IMX327_Y_OUT_SIZE_ADDR;                                             
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[21].u8DelayFrmNum = 1;                                                  
            g_apstSnsState[IspDev]->astRegsInfo[0].astI2cData[21].u32RegAddr = IMX327_Y_OUT_SIZE_ADDR + 1; 

         } 
        
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

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy(pstSnsRegsInfo, &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S)); 
    memcpy(&g_apstSnsState[IspDev]->astRegsInfo[1], &g_apstSnsState[IspDev]->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S)); 

    g_apstSnsState[IspDev]->au32FL[1] = g_apstSnsState[IspDev]->au32FL[0];

    return 0;
}

HI_S32 cmos_set_image_mode(ISP_DEV IspDev,ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = g_apstSnsState[IspDev]->u8ImgMode;
    
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE; 
        
    if (HI_NULL == pstSensorImageMode )
    {
        printf("null pointer when set image mode\n");
        return -1;
    }

    if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080)) 
    {
    }  
    else
    {
        printf("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", 
            pstSensorImageMode->u16Width, 
            pstSensorImageMode->u16Height,
            pstSensorImageMode->f32Fps,
            g_apstSnsState[IspDev]->enWDRMode);

        return -1;
    }

    if ((HI_TRUE == g_apstSnsState[IspDev]->bInit) && (u8SensorImageMode == g_apstSnsState[IspDev]->u8ImgMode))     
    {                                                                              
        /* Don't need to switch SensorImageMode */                                 
        return -1;                                                                 
    }                                                                              

    return 0;
}

HI_VOID sensor_global_init(ISP_DEV IspDev)
{     
    g_apstSnsState[IspDev]->bInit = HI_FALSE; 
    g_apstSnsState[IspDev]->bSyncInit = HI_FALSE;
    g_apstSnsState[IspDev]->u8ImgMode = IMX327_SENSOR_1080P_30FPS_LINEAR_MODE;
    g_apstSnsState[IspDev]->enWDRMode = WDR_MODE_NONE;
    g_apstSnsState[IspDev]->u32FLStd = IMX327_VMAX_1080P30_LINEAR;
    g_apstSnsState[IspDev]->au32FL[0] = IMX327_VMAX_1080P30_LINEAR;
    g_apstSnsState[IspDev]->au32FL[1] = IMX327_VMAX_1080P30_LINEAR;

    memset(&g_apstSnsState[IspDev]->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&g_apstSnsState[IspDev]->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = imx327_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = imx327_exit;
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

int imx327_set_bus_info(ISP_DEV IspDev, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx327BusInfo[IspDev].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return 0;
}

int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX327_ID, &stIspRegister);
    if (s32Ret)
    {
        printf("sensor register callback function failed!\n");
        return s32Ret;
    }
    
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, pstAeLib, IMX327_ID, &stAeRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, pstAwbLib, IMX327_ID, &stAwbRegister);
    if (s32Ret)
    {
        printf("sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }
    
    return 0;
}

int sensor_unregister_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(IspDev, IMX327_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function failed!\n");
        return s32Ret;
    }
    
    s32Ret = HI_MPI_AE_SensorUnRegCallBack(IspDev, pstAeLib, IMX327_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(IspDev, pstAwbLib, IMX327_ID);
    if (s32Ret)
    {
        printf("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }
    
    return 0;
}

int sensor_set_init(ISP_DEV IspDev, ISP_INIT_ATTR_S *pstInitAttr)
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

ISP_SNS_OBJ_S stSnsImx327Obj = 
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx327_standby,
    .pfnRestart             = imx327_restart,
    .pfnWriteReg            = imx327_write_register,
    .pfnReadReg             = imx327_read_register,
    .pfnSetBusInfo          = imx327_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __IMX327_CMOS_H_ */
