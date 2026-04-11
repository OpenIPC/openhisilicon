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
//#define WDR_2TO1_OFFSET 1959
#define WDR_2TO1_OFFSET 5312

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

    return;
}

static HI_VOID BlcRegsDefault(HI_VOID) 
{
	

	  
    return;
}

static HI_VOID BlcExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID BlcRegsConfig(ISP_DEV IspDev, ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel)
{
    ISP_CTX_S    *pstIspCtx = HI_NULL;
	HI_U16 i;
    ISP_GET_CTX(IspDev, pstIspCtx);
	

	  /* ISP Digital gain offset & WB gain offset */

    /* ISP Digital gain offset & WB gain offset */

	/* AE */
	hi_isp_ae_blc_en_write(IspDev, HI_FALSE);
	hi_isp_ae_offset_r_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_isp_ae_offset_gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_isp_ae_offset_gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_isp_ae_offset_b_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
	
	/* AE_WDR */
	for(i = 0; i < 4; i++)
	{
		hi_isp_ae_wdr_blc_en_write(IspDev, i, HI_TRUE);
		hi_isp_ae_wdr_offset_r_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[0] << 2);
		hi_isp_ae_wdr_offset_gr_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[1] << 2);
		hi_isp_ae_wdr_offset_gb_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[2] << 2);
		hi_isp_ae_wdr_offset_b_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[3] << 2);
	}

	hi_isp_lsc_blc_in_enable_write(IspDev,HI_TRUE);
	hi_isp_lsc_blc_out_enable_write(IspDev,HI_TRUE);
	hi_isp_lsc_blc_r_write(IspDev,pstSnsBlackLevel->au16BlackLevel[0]  << 2);
	hi_isp_lsc_blc_gr_write(IspDev,pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_isp_lsc_blc_gb_write(IspDev,pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_isp_lsc_blc_b_write(IspDev,pstSnsBlackLevel->au16BlackLevel[3]  << 2);

	hi_isp_compander_offsetr_in_write (IspDev, pstSnsBlackLevel->au16BlackLevel[0]);
	hi_isp_compander_offsetgr_in_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1]);
	hi_isp_compander_offsetgb_in_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2]);
	hi_isp_compander_offsetb_in_write (IspDev, pstSnsBlackLevel->au16BlackLevel[3]);



	hi_ext_system_wdr_blc0_write(pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_ext_system_wdr_blc1_write(pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_ext_system_wdr_blc2_write(pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_ext_system_wdr_blc3_write(pstSnsBlackLevel->au16BlackLevel[3] << 2);
	hi_ext_system_wdr_out_blc_write(pstSnsBlackLevel->au16BlackLevel[0] << 2);


		
  if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode))
    {		
		/*DG*/
		hi_isp_dg_blc_in_enable_write(IspDev, HI_TRUE);
		//hi_isp_dg_blc_out_enable_write(HI_TRUE);
		hi_isp_dg_blc_out_enable_write(IspDev, HI_FALSE);
	
		hi_isp_dg_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
		hi_isp_dg_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
		hi_isp_dg_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
		hi_isp_dg_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);

    }
    else if (IS_BUILT_IN_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        /* offset has been subtracted by gammaa_fe module, so set to 0 */
		hi_isp_dg_blc_in_enable_write(IspDev, HI_FALSE);
		hi_isp_dg_blc_out_enable_write(IspDev, HI_FALSE);

    }
    else if (IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
		/*DG*/
		hi_isp_dg_blc_in_enable_write(IspDev, HI_TRUE);
		//hi_isp_dg_blc_out_enable_write(HI_TRUE);
		hi_isp_dg_blc_out_enable_write(IspDev, HI_FALSE);
		hi_isp_dg_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
		hi_isp_dg_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
		hi_isp_dg_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
		hi_isp_dg_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
	
		hi_isp_wdr_outblc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);				
		hi_isp_wdr_f0_inblc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
		hi_isp_wdr_f1_inblc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
		hi_isp_flick_frm0_blc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
		hi_isp_flick_frm1_blc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
    }
    else
    {
        /* unknow u8SnsWDRMode */
    }

    return;
}    


static HI_VOID BlcRegsInitialize(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel = HI_NULL;
    ISP_BLACKLEVEL_S *pstBlackLevel = HI_NULL;
	ISP_CTX_S    *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);
	
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


	/* AE */
	hi_isp_ae_blc_en_write(IspDev, HI_FALSE);
	hi_isp_ae_offset_r_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_isp_ae_offset_gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_isp_ae_offset_gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_isp_ae_offset_b_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);

	/* AE_WDR */
	for(i = 0; i < 4; i++)
	{
	  hi_isp_ae_wdr_blc_en_write(IspDev, i, HI_TRUE);
	  hi_isp_ae_wdr_offset_r_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	  hi_isp_ae_wdr_offset_gr_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	  hi_isp_ae_wdr_offset_gb_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	  hi_isp_ae_wdr_offset_b_write(IspDev, i, pstSnsBlackLevel->au16BlackLevel[3] << 2);
	}

	hi_isp_lsc_blc_in_enable_write(IspDev,HI_TRUE);
	hi_isp_lsc_blc_out_enable_write(IspDev,HI_TRUE);
	hi_isp_lsc_blc_r_write(IspDev,pstSnsBlackLevel->au16BlackLevel[0]  << 2);
	hi_isp_lsc_blc_gr_write(IspDev,pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_isp_lsc_blc_gb_write(IspDev,pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_isp_lsc_blc_b_write(IspDev,pstSnsBlackLevel->au16BlackLevel[3]  << 2);

	hi_isp_compander_offsetr_in_write (IspDev, pstSnsBlackLevel->au16BlackLevel[0]);
	hi_isp_compander_offsetgr_in_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1]);
	hi_isp_compander_offsetgb_in_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2]);
	hi_isp_compander_offsetb_in_write (IspDev, pstSnsBlackLevel->au16BlackLevel[3]);



	hi_ext_system_wdr_blc0_write(pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_ext_system_wdr_blc1_write(pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_ext_system_wdr_blc2_write(pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_ext_system_wdr_blc3_write(pstSnsBlackLevel->au16BlackLevel[3] << 2);
	hi_ext_system_wdr_out_blc_write(pstSnsBlackLevel->au16BlackLevel[0] << 2);
    /*
	hi_isp_4dg_blc_in_enable_write(IspDev,HI_TRUE);
	hi_isp_4dg_blc_out_enable_write(IspDev,HI_TRUE);
	hi_isp_4dg_0_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_isp_4dg_0_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_isp_4dg_0_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_isp_4dg_0_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
	
	hi_isp_4dg_1_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_isp_4dg_1_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	hi_isp_4dg_1_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	hi_isp_4dg_1_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
	*/
	if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode))
	{ 	  
	  /*DG*/
	  hi_isp_dg_blc_in_enable_write(IspDev, HI_TRUE);
	  //hi_isp_dg_blc_out_enable_write(HI_TRUE);
	  hi_isp_dg_blc_out_enable_write(IspDev, HI_FALSE);

	  hi_isp_dg_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	  hi_isp_dg_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	  hi_isp_dg_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	  hi_isp_dg_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);

	  hi_isp_mg_blc_en_write(IspDev, HI_FALSE);
	  hi_isp_mg_offset_r_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	  hi_isp_mg_offset_gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	  hi_isp_mg_offset_gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
	  hi_isp_mg_offset_b_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
      hi_isp_4dg_blc_in_enable_write(IspDev,HI_TRUE);
	  hi_isp_4dg_blc_out_enable_write(IspDev,HI_TRUE);
	  hi_isp_4dg_0_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
 	  hi_isp_4dg_0_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
 	  hi_isp_4dg_0_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
 	  hi_isp_4dg_0_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
 	  hi_isp_4dg_1_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
 	  hi_isp_4dg_1_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
 	  hi_isp_4dg_1_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
 	  hi_isp_4dg_1_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
	}
	else if (IS_BUILT_IN_WDR_MODE(pstIspCtx->u8SnsWDRMode))
	{
	  /* offset has been subtracted by gammaa_fe module, so set to 0 */

	}
	else if (IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
	{

	  hi_isp_dg_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
	  hi_isp_dg_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
	  hi_isp_dg_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
	  hi_isp_dg_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);

	  hi_isp_mg_blc_en_write(IspDev, HI_FALSE);
	  hi_isp_mg_offset_r_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
	  hi_isp_mg_offset_gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] >> 4);
	  hi_isp_mg_offset_gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] >> 4);
	  hi_isp_mg_offset_b_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] >> 4);
	  
	  hi_isp_wdr_outblc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);			  
	  hi_isp_wdr_f0_inblc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	  hi_isp_wdr_f1_inblc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	  hi_isp_flick_frm0_blc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
	  hi_isp_flick_frm1_blc_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
      hi_isp_4dg_blc_in_enable_write(IspDev,HI_TRUE);
	  hi_isp_4dg_blc_out_enable_write(IspDev,HI_TRUE);
	  hi_isp_4dg_0_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
 	  hi_isp_4dg_0_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
 	  hi_isp_4dg_0_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
 	  hi_isp_4dg_0_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
 	  hi_isp_4dg_1_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
 	  hi_isp_4dg_1_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
 	  hi_isp_4dg_1_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
 	  hi_isp_4dg_1_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
	}
	else
	{
	  /* unknow u8SnsWDRMode */
	}

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
	ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL; 
    BlcRegsDefault();
    BlcExtRegsDefault();
	ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);  
    BlcRegsConfig(IspDev, pstSnsBlackLevel);
    BlcRegsInitialize(IspDev);
    BlcExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_BlcRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    static HI_BOOL bWdrFswdrEnable = HI_FALSE;
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
        BlcRegsConfig(IspDev, pstSnsBlackLevel);
    }

	/*sensors's blacklevel is changed by mpi. */    
    if (HI_TRUE == pstBlackLevel->u8BlackLevelChange)
    {    
                
    	memcpy(&pstSnsBlackLevel->au16BlackLevel[0], &pstBlackLevel->au16BlackLevel[0], 4*sizeof(HI_U16));
        BlcRegsConfig(IspDev, pstSnsBlackLevel);
		hi_ext_system_black_level_change_write(HI_FALSE);
    }
	if(IS_FS_WDR_MODE(pstIspCtx->u8SnsWDRMode))
    {
        if(0==hi_isp_wdr_enable_read(IspDev))
        {
            hi_isp_dg_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] << 2);
            hi_isp_dg_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[1] << 2);
            hi_isp_dg_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[2] << 2);
            hi_isp_dg_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[3] << 2);
            bWdrFswdrEnable = HI_FALSE;
        }
        else if(bWdrFswdrEnable==HI_FALSE)
        {
            hi_isp_dg_blc_offset_R_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
            hi_isp_dg_blc_offset_Gr_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
            hi_isp_dg_blc_offset_Gb_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
            hi_isp_dg_blc_offset_B_write(IspDev, pstSnsBlackLevel->au16BlackLevel[0] >> 4);
            bWdrFswdrEnable = HI_TRUE;
        }
    }

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


