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

/* the value of 2TO1_OFFSET relate with black_level_out and gamma_fe */
#define WDR_2TO1_FSWDR_OUT_OFFSET   (32 << 4)
#define WDR_2TO1_GAMMAFE_OUT_OFFSET (333 << 4)
//#define WDR_2TO1_FSWDR_OUT_OFFSET   (128 << 4)
//#define WDR_2TO1_GAMMAFE_OUT_OFFSET (623 << 4)


static HI_U8 u8PreBypassFSWDR = -1;

typedef struct hiISP_BLACKLEVEL_S
{
    HI_U8  u8BlackLevelChange;
    HI_U16 au16BlackLevel[4];

} ISP_BLACKLEVEL_S;

ISP_BLACKLEVEL_S g_astBlackLevelCtx[ISP_MAX_DEV_NUM] = {{0}};
#define BLACKLEVEL_GET_CTX(dev, pstBlackLevelCtx)   pstBlackLevelCtx = &g_astBlackLevelCtx[dev]

static HI_VOID BlcExtRegsDefault(HI_VOID)
{
    hi_ext_system_black_level_change_write(HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT);
    hi_ext_system_black_level_00_write(HI_EXT_SYSTEM_BLACK_LEVEL_00_DEFAULT);
    hi_ext_system_black_level_01_write(HI_EXT_SYSTEM_BLACK_LEVEL_01_DEFAULT);
    hi_ext_system_black_level_10_write(HI_EXT_SYSTEM_BLACK_LEVEL_10_DEFAULT);
    hi_ext_system_black_level_11_write(HI_EXT_SYSTEM_BLACK_LEVEL_11_DEFAULT);

    hi_ext_system_black_level_query_00_write(HI_EXT_SYSTEM_BLACK_LEVEL_QUERY_00_DEFAULT);
    hi_ext_system_black_level_query_01_write(HI_EXT_SYSTEM_BLACK_LEVEL_QUERY_01_DEFAULT);
    hi_ext_system_black_level_query_10_write(HI_EXT_SYSTEM_BLACK_LEVEL_QUERY_10_DEFAULT);
    hi_ext_system_black_level_query_11_write(HI_EXT_SYSTEM_BLACK_LEVEL_QUERY_11_DEFAULT);

    return;
}

static HI_VOID BlcRegsDefault(HI_VOID) 
{
    hi_isp_top_bypass_balance_fe_write(0);
    
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
    hi_ext_system_black_level_00_write(pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_01_write(pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_10_write(pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_11_write(pstBlackLevel->au16BlackLevel[3]);

    hi_ext_system_black_level_query_00_write(pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_query_01_write(pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_query_10_write(pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_query_11_write(pstBlackLevel->au16BlackLevel[3]);
   
    return;
}

static HI_VOID BlcRegsConfig(ISP_DEV IspDev, HI_U16 *pu16BlackLevel)
{
    HI_S32 i;
    HI_U16 u16OffsetMin;
    ISP_CTX_S    *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);

    /* calulate Min BL of RGrGbG channel */
    u16OffsetMin = pu16BlackLevel[0];
    for (i=1; i<4; i++)
    {
        if (u16OffsetMin > pu16BlackLevel[i])
        {
            u16OffsetMin = pu16BlackLevel[i];
        }
    }

    hi_ext_system_black_level_query_00_write(pu16BlackLevel[0]);
    hi_ext_system_black_level_query_01_write(pu16BlackLevel[1]);
    hi_ext_system_black_level_query_10_write(pu16BlackLevel[2]);
    hi_ext_system_black_level_query_11_write(pu16BlackLevel[3]);

    /* sensor offset(balance_fe):
        sensor offset (balance_fe), subtract the difference of RGrGbB channel;
        14bit register, should right shift 2 for 12bit raw  */
    hi_isp_sensor_offset_black_00_write((pu16BlackLevel[0] - u16OffsetMin) << 2);
    hi_isp_sensor_offset_black_01_write((pu16BlackLevel[1] - u16OffsetMin) << 2);
    hi_isp_sensor_offset_black_10_write((pu16BlackLevel[2] - u16OffsetMin) << 2);
    hi_isp_sensor_offset_black_11_write((pu16BlackLevel[3] - u16OffsetMin) << 2);

    /* ISP Digital gain offset & WB gain offset */
    if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_digital_gain_offset_write(u16OffsetMin << 4);
        hi_isp_offset_black_00_write(u16OffsetMin);
        hi_isp_offset_black_01_write(u16OffsetMin);
        hi_isp_offset_black_10_write(u16OffsetMin);
        hi_isp_offset_black_11_write(u16OffsetMin);
    }
    else if (IS_BUILT_IN_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        hi_isp_digital_gain_offset_write(u16OffsetMin << 4); 
        hi_isp_offset_black_00_write(u16OffsetMin);
        hi_isp_offset_black_01_write(u16OffsetMin);
        hi_isp_offset_black_10_write(u16OffsetMin);
        hi_isp_offset_black_11_write(u16OffsetMin);
    }
    else if (IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        /* frame-stitch WDR offset */
        hi_isp_wdr_black_level_long_write(u16OffsetMin);
        hi_isp_wdr_black_level_short_write(u16OffsetMin);
        hi_isp_wdr_black_level_out_write((HI_U16)WDR_2TO1_FSWDR_OUT_OFFSET);
        
        hi_isp_digital_gain_offset_write((HI_U16)WDR_2TO1_GAMMAFE_OUT_OFFSET);
        hi_isp_offset_black_00_write((HI_U16)WDR_2TO1_GAMMAFE_OUT_OFFSET >> 4);
        hi_isp_offset_black_01_write((HI_U16)WDR_2TO1_GAMMAFE_OUT_OFFSET >> 4);
        hi_isp_offset_black_10_write((HI_U16)WDR_2TO1_GAMMAFE_OUT_OFFSET >> 4);
        hi_isp_offset_black_11_write((HI_U16)WDR_2TO1_GAMMAFE_OUT_OFFSET >> 4);

        if (hi_isp_top_bypass_frame_switch_wdr_read() && (u16OffsetMin >= (WDR_2TO1_FSWDR_OUT_OFFSET >> 4))
            && (pstIspCtx->u8SnsWDRMode == pstIspCtx->u8PreSnsWDRMode))
        {
            hi_isp_sensor_offset_black_00_write((pu16BlackLevel[0] - (WDR_2TO1_FSWDR_OUT_OFFSET >> 4)) << 2);
            hi_isp_sensor_offset_black_01_write((pu16BlackLevel[1] - (WDR_2TO1_FSWDR_OUT_OFFSET >> 4)) << 2);
            hi_isp_sensor_offset_black_10_write((pu16BlackLevel[2] - (WDR_2TO1_FSWDR_OUT_OFFSET >> 4)) << 2);
            hi_isp_sensor_offset_black_11_write((pu16BlackLevel[3] - (WDR_2TO1_FSWDR_OUT_OFFSET >> 4)) << 2);
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

    pstBlackLevel->u8BlackLevelChange = hi_ext_system_black_level_change_read();
    pstBlackLevel->au16BlackLevel[0] = hi_ext_system_black_level_00_read();
    pstBlackLevel->au16BlackLevel[1] = hi_ext_system_black_level_01_read();
    pstBlackLevel->au16BlackLevel[2] = hi_ext_system_black_level_10_read();
    pstBlackLevel->au16BlackLevel[3] = hi_ext_system_black_level_11_read();
    
    return 0;
}

HI_S32 ISP_BlcInit(ISP_DEV IspDev)
{
    BlcRegsDefault();
    BlcExtRegsDefault();
    BlcRegsInitialize(IspDev);
    BlcExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_BlcRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_CTX_S *pstIspCtx  = HI_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL;
    ISP_BLACKLEVEL_S *pstBlackLevel = HI_NULL;

    BLACKLEVEL_GET_CTX(IspDev, pstBlackLevel);    
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

    BlcReadExtregs(IspDev);

    /* some sensors's blacklevel is changed with iso. */   
    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);  
    if (pstSnsBlackLevel->bUpdate == HI_TRUE)
    {        
        ISP_SensorUpdateBlc(IspDev);        
        BlcRegsConfig(IspDev, pstSnsBlackLevel->au16BlackLevel);
    }
    else if (pstBlackLevel->u8BlackLevelChange)
    {
        BlcRegsConfig(IspDev, pstBlackLevel->au16BlackLevel);
        pstBlackLevel->u8BlackLevelChange = 0;
        hi_ext_system_black_level_change_write(pstBlackLevel->u8BlackLevelChange);
    }

    if (u8PreBypassFSWDR != (hi_isp_top_bypass_frame_switch_wdr_read()))
    {
        BlcRegsConfig(IspDev, pstBlackLevel->au16BlackLevel);
    }
    u8PreBypassFSWDR = hi_isp_top_bypass_frame_switch_wdr_read();

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


