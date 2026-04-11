/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_black_offset.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/24
  Description   : 
  History       :
  1.Date        : 2013/04/24
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef CLIP3
#define CLIP3(x,min,max)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#endif

/* the value of 2TO1_OFFSET relate with black_level_out and gamma_fe */
#define WDR_2TO1_FSWDR_OUT_OFFSET   (128 << 4)
#define WDR_2TO1_GAMMAFE_OUT_OFFSET (623 << 4)

static HI_U8 au8PreBypassFSWDR[ISP_MAX_DEV_NUM] = {0,0};

typedef struct hiISP_BLACKLEVEL_S
{
    HI_U8  u8BlackLevelChange;
    HI_U16 au16BlackLevel[4];

} ISP_BLACKLEVEL_S;

ISP_BLACKLEVEL_S g_astBlackLevelCtx[ISP_MAX_DEV_NUM] = {{0}};
#define BLACKLEVEL_GET_CTX(dev, pstBlackLevelCtx)   pstBlackLevelCtx = &g_astBlackLevelCtx[dev]

static HI_VOID BlcExtRegsDefault(ISP_DEV IspDev)
{
    hi_ext_system_black_level_change_write(IspDev, HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT);
    hi_ext_system_black_level_00_write(IspDev, HI_EXT_SYSTEM_BLACK_LEVEL_00_DEFAULT);
    hi_ext_system_black_level_01_write(IspDev, HI_EXT_SYSTEM_BLACK_LEVEL_01_DEFAULT);
    hi_ext_system_black_level_10_write(IspDev, HI_EXT_SYSTEM_BLACK_LEVEL_10_DEFAULT);
    hi_ext_system_black_level_11_write(IspDev, HI_EXT_SYSTEM_BLACK_LEVEL_11_DEFAULT);
    return;
}

static HI_VOID BlcRegsDefault(ISP_DEV IspDev) 
{
    return;
}

static HI_VOID BlcExtRegsInitialize(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL;
    ISP_BLACKLEVEL_S *pstBlackLevel = HI_NULL;
    
    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);  
    BLACKLEVEL_GET_CTX(IspDev, pstBlackLevel);
        
    for (i=0; i<4; i++)
    {
        pstBlackLevel->au16BlackLevel[i] = pstSnsBlackLevel->au16BlackLevel[i];
    }
    hi_ext_system_black_level_00_write(IspDev, pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_01_write(IspDev, pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_10_write(IspDev, pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_11_write(IspDev, pstBlackLevel->au16BlackLevel[3]);
    
    return;
}

static HI_VOID BlcRegsConfig(ISP_DEV IspDev, HI_U16 *pu16BlackLevel)
{
    HI_S32 i;
    HI_U32 u32OffsetMin;
    ISP_CTX_S    *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);

    /* calulate Min BL of RGrGbG channel, input black level is 12bit */
    u32OffsetMin = pu16BlackLevel[0];
    for (i=1; i<4; i++)
    {
        if (u32OffsetMin > pu16BlackLevel[i])
        {
            u32OffsetMin = pu16BlackLevel[i];
        }
    }

    /* ISP sensor offset(20bit), sensor offset WDR(12bit), Digital gain offset(20bit) & WB gain offset(20bit) */
    if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_top_bypass_sensor_offset_write(IspDev, 0);
        hi_isp_top_bypass_sensor_offset_wdr_write(IspDev, 1);
        hi_isp_sensor_offset_black_00_write(IspDev, (pu16BlackLevel[0] - u32OffsetMin) << 8);
        hi_isp_sensor_offset_black_01_write(IspDev, (pu16BlackLevel[1] - u32OffsetMin) << 8);
        hi_isp_sensor_offset_black_10_write(IspDev, (pu16BlackLevel[2] - u32OffsetMin) << 8);
        hi_isp_sensor_offset_black_11_write(IspDev, (pu16BlackLevel[3] - u32OffsetMin) << 8);
        hi_isp_digital_gain_offset_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_00_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_01_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_10_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_11_write(IspDev, u32OffsetMin << 8);

        hi_isp_frame_stitch_gain_offset_wdr1_write(IspDev, 0);  
        hi_isp_frame_stitch_gain_offset_wdr2_write(IspDev, 0); 
        hi_isp_frame_stitch_gain_offset_wdr3_write(IspDev, 0); 
        hi_isp_frame_stitch_gain_offset_wdr4_write(IspDev, 0); 
    }
    else if (IS_BUILT_IN_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_top_bypass_sensor_offset_write(IspDev, 0);
        hi_isp_top_bypass_sensor_offset_wdr_write(IspDev, 1);
        hi_isp_input_formatter_black_level_write(IspDev, u32OffsetMin);
        hi_isp_sensor_offset_black_00_write(IspDev, (pu16BlackLevel[0] - u32OffsetMin) << 8);
        hi_isp_sensor_offset_black_01_write(IspDev, (pu16BlackLevel[1] - u32OffsetMin) << 8);
        hi_isp_sensor_offset_black_10_write(IspDev, (pu16BlackLevel[2] - u32OffsetMin) << 8);
        hi_isp_sensor_offset_black_11_write(IspDev, (pu16BlackLevel[3] - u32OffsetMin) << 8);

        hi_isp_frame_stitch_gain_offset_wdr1_write(IspDev, 0);  
        hi_isp_frame_stitch_gain_offset_wdr2_write(IspDev, 0); 
        hi_isp_frame_stitch_gain_offset_wdr3_write(IspDev, 0); 
        hi_isp_frame_stitch_gain_offset_wdr4_write(IspDev, 0); 
#if 1	/* 20bit to 3lane */	
		hi_isp_frame_stitch_black_level_long_write(IspDev, u32OffsetMin << 8);
        hi_isp_frame_stitch_black_level_medium_write(IspDev, u32OffsetMin << 4);
        hi_isp_frame_stitch_black_level_short_write(IspDev, u32OffsetMin);
        hi_isp_frame_stitch_black_level_very_short_write(IspDev, u32OffsetMin >> 4);
#else   /* 20bit to 2lane */
        hi_isp_frame_stitch_black_level_short_write(IspDev, u32OffsetMin << 2);
        hi_isp_frame_stitch_black_level_very_short_write(IspDev, u32OffsetMin >> 4);
#endif

        hi_isp_frame_stitch_black_level_out_write(IspDev, u32OffsetMin << 4);
		
        hi_isp_digital_gain_offset_write(IspDev, u32OffsetMin << 4); 
        hi_isp_offset_black_00_write(IspDev, u32OffsetMin << 4);
        hi_isp_offset_black_01_write(IspDev, u32OffsetMin << 4);
        hi_isp_offset_black_10_write(IspDev, u32OffsetMin << 4);
        hi_isp_offset_black_11_write(IspDev, u32OffsetMin << 4);
    }
    else if (IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_top_bypass_sensor_offset_write(IspDev, 1);
        hi_isp_top_bypass_sensor_offset_wdr_write(IspDev, 0);
        /* black level of wdr is 12bit */
        hi_isp_sensor_offset_wdr_1_black_00_write(IspDev, pu16BlackLevel[0] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_1_black_01_write(IspDev, pu16BlackLevel[1] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_1_black_10_write(IspDev, pu16BlackLevel[2] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_1_black_11_write(IspDev, pu16BlackLevel[3] - u32OffsetMin);
        
        hi_isp_sensor_offset_wdr_2_black_00_write(IspDev, pu16BlackLevel[0] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_2_black_01_write(IspDev, pu16BlackLevel[1] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_2_black_10_write(IspDev, pu16BlackLevel[2] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_2_black_11_write(IspDev, pu16BlackLevel[3] - u32OffsetMin);
        
        hi_isp_sensor_offset_wdr_3_black_00_write(IspDev, pu16BlackLevel[0] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_3_black_01_write(IspDev, pu16BlackLevel[1] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_3_black_10_write(IspDev, pu16BlackLevel[2] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_3_black_11_write(IspDev, pu16BlackLevel[3] - u32OffsetMin);

        hi_isp_sensor_offset_wdr_4_black_00_write(IspDev, pu16BlackLevel[0] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_4_black_01_write(IspDev, pu16BlackLevel[1] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_4_black_10_write(IspDev, pu16BlackLevel[2] - u32OffsetMin);
        hi_isp_sensor_offset_wdr_4_black_11_write(IspDev, pu16BlackLevel[3] - u32OffsetMin);

        /* black level of WDR gain is 14bit, FSWDR use these as ISP digital gain */
        hi_isp_frame_stitch_gain_offset_wdr1_write(IspDev, u32OffsetMin << 2);  
        hi_isp_frame_stitch_gain_offset_wdr2_write(IspDev, u32OffsetMin << 2); 
        hi_isp_frame_stitch_gain_offset_wdr3_write(IspDev, u32OffsetMin << 2); 
        hi_isp_frame_stitch_gain_offset_wdr4_write(IspDev, u32OffsetMin << 2); 

        /* black level IN of FSWDR is 12bit, black level OUT of FSWDR is 20bit */
        hi_isp_frame_stitch_black_level_long_write(IspDev, u32OffsetMin);
        hi_isp_frame_stitch_black_level_medium_write(IspDev, u32OffsetMin);
        hi_isp_frame_stitch_black_level_short_write(IspDev, u32OffsetMin);
        hi_isp_frame_stitch_black_level_very_short_write(IspDev, u32OffsetMin);
        hi_isp_frame_stitch_black_level_out_write(IspDev, u32OffsetMin << 8);

        /* black level of WB gain is 20bit */
        hi_isp_offset_black_00_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_01_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_10_write(IspDev, u32OffsetMin << 8);
        hi_isp_offset_black_11_write(IspDev, u32OffsetMin << 8);

        /* black level of ISP digital gain is 20bit, FSWDR not use */
        hi_isp_digital_gain_offset_write(IspDev, u32OffsetMin << 8); 

        if (hi_isp_top_bypass_frame_stitch_wdr_read(IspDev) && (pstIspCtx->u8SnsWDRMode == pstIspCtx->u8PreSnsWDRMode))
        {
            hi_isp_frame_stitch_gain_offset_wdr1_write(IspDev, u32OffsetMin << 2);  
            hi_isp_frame_stitch_gain_offset_wdr2_write(IspDev, u32OffsetMin << 2); 
            hi_isp_frame_stitch_gain_offset_wdr3_write(IspDev, u32OffsetMin << 2); 
            hi_isp_frame_stitch_gain_offset_wdr4_write(IspDev, u32OffsetMin << 2); 
            
            hi_isp_frame_stitch_black_level_out_write(IspDev, u32OffsetMin << 8);
            hi_isp_digital_gain_offset_write(IspDev, u32OffsetMin << 8);
            hi_isp_offset_black_00_write(IspDev, u32OffsetMin << 8);
            hi_isp_offset_black_01_write(IspDev, u32OffsetMin << 8);
            hi_isp_offset_black_10_write(IspDev, u32OffsetMin << 8);
            hi_isp_offset_black_11_write(IspDev, u32OffsetMin << 8);
        }
    }
    else
    {
        /* unknow u8SnsWDRMode */
    }

    return;
}    


static HI_VOID BlcRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel = HI_NULL;
    
    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);    
    BlcRegsConfig(IspDev, pstSnsBlackLevel->au16BlackLevel);

    return;
}

static HI_S32 BlcReadExtregs(ISP_DEV IspDev)
{
    ISP_BLACKLEVEL_S *pstBlackLevel = HI_NULL;
    BLACKLEVEL_GET_CTX(IspDev, pstBlackLevel);

    pstBlackLevel->u8BlackLevelChange = hi_ext_system_black_level_change_read(IspDev);
    pstBlackLevel->au16BlackLevel[0] = hi_ext_system_black_level_00_read(IspDev);
    pstBlackLevel->au16BlackLevel[1] = hi_ext_system_black_level_01_read(IspDev);
    pstBlackLevel->au16BlackLevel[2] = hi_ext_system_black_level_10_read(IspDev);
    pstBlackLevel->au16BlackLevel[3] = hi_ext_system_black_level_11_read(IspDev);
    
    return 0;
}

HI_S32 ISP_BlcInit(ISP_DEV IspDev)
{
    BlcRegsDefault(IspDev);
    BlcExtRegsDefault(IspDev);
    BlcRegsInitialize(IspDev);
    BlcExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_BlcRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8 i;
    ISP_CTX_S *pstIspCtx  = HI_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL;
    ISP_BLACKLEVEL_S *pstBlackLevel = HI_NULL;
    HI_S16 s16BlkOffset = 0;
        
    BLACKLEVEL_GET_CTX(IspDev, pstBlackLevel);    
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts other than capture state*/
    if ((0 != pstIspCtx->u32FrameCnt % 2) && (HI_TRUE != pstIspCtx->stLinkage.bCapState))
    {
        return HI_SUCCESS;
    }

    BlcReadExtregs(IspDev);

    /* some sensors's blacklevel is changed with iso. */   
    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);

    /* ISP dual pipe blackLevel different config */
    for(i = 0; i < 4; i++)
    {
        s16BlkOffset = (HI_S16)(hi_ext_system_isp_pipe_diff_offset_read(IspDev, i) >> 8);
        pstSnsBlackLevel->au16BlackLevel[i] = CLIP3((HI_S16)pstSnsBlackLevel->au16BlackLevel[i] + s16BlkOffset, 0, 0xFFFF);
        pstBlackLevel->au16BlackLevel[i] = CLIP3((HI_S16)pstBlackLevel->au16BlackLevel[i] + s16BlkOffset, 0, 0xFFFF);
    }
    
    if (pstSnsBlackLevel->bUpdate == HI_TRUE)
    {        
        ISP_SensorUpdateBlc(IspDev);        
        BlcRegsConfig(IspDev, pstSnsBlackLevel->au16BlackLevel);
    }

    if (pstBlackLevel->u8BlackLevelChange)
    {
        BlcRegsConfig(IspDev, pstBlackLevel->au16BlackLevel);
        pstBlackLevel->u8BlackLevelChange = 0;
        hi_ext_system_black_level_change_write(IspDev, pstBlackLevel->u8BlackLevelChange);
    }

    if (au8PreBypassFSWDR[IspDev] != (hi_isp_top_bypass_frame_stitch_wdr_read(IspDev)))
    {
        BlcRegsConfig(IspDev, pstBlackLevel->au16BlackLevel);
    }
    au8PreBypassFSWDR[IspDev] = hi_isp_top_bypass_frame_stitch_wdr_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_BlcCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_BlcInit(IspDev);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_BlcExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterBlc(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_BLC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_BlcInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_BlcRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_BlcCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_BlcExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


