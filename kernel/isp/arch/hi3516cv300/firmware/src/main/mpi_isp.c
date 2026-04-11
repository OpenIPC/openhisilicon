/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   : 
  History       :
  1.Date        : 2011/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

#include "hi_comm_isp.h"
#include "mpi_sys.h"
#include "mpi_isp.h"
#include "mkp_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_shading_mem_config.h"
#include "isp_metering_mem_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_gamma_fe0_mem_config.h"
#include "isp_gamma_fe1_mem_config.h"
#include "isp_debug.h"
#include "isp_main.h"

#include "hi_vreg.h"

#include "hi_comm_vi.h"
#include "mkp_vi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

#define MPI_ISP_PARAM_CHECK_RETURN(x, min, max, fmt, ...)                     \
    do {                                                                      \
        if ((x) < (min) || (x) > (max))                                       \
        {                                                                     \
            printf("[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);     \
            printf("%#x "fmt, x, ##__VA_ARGS__);                              \
            return HI_ERR_ISP_ILLEGAL_PARAM;                                  \
        }                                                                     \
    } while (0)
    
#define MPI_ISP_MAX_PARAM_CHECK_RETURN(x, max, fmt, ...)                     \
			do {																	  \
				if ((x) > (max)) 									  \
				{																	  \
					printf("[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);	  \
					printf("%#x "fmt, x, ##__VA_ARGS__);							  \
					return HI_ERR_ISP_ILLEGAL_PARAM;								  \
				}																	  \
			} while (0)


#define MPI_ISP_ARRAY_PARAM_CHECK_RETURN(x, type, size, min, max, fmt, ...)   \
    do {                                                                      \
        int loops = size;                                                     \
        type *p = (type *)x;                                                  \
        while (loops--)                                                       \
        {                                                                     \
            if ((p[loops]) < (min) || (p[loops]) > (max))                     \
            {                                                                 \
                printf("[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__); \
                printf("array[%d] = %#x "fmt, loops, p[loops], ##__VA_ARGS__);\
                return HI_ERR_ISP_ILLEGAL_PARAM;                              \
            }                                                                 \
        }                                                                     \
    } while (0)  

#define MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(x, type, size, max, fmt, ...)   \
    do {                                                                      \
        int loops = size;                                                     \
        type *p = (type *)x;                                                  \
        while (loops--)                                                       \
        {                                                                     \
            if ((p[loops]) > (max))                     \
            {                                                                 \
                printf("[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__); \
                printf("array[%d] = %#x "fmt, loops, p[loops], ##__VA_ARGS__);\
                return HI_ERR_ISP_ILLEGAL_PARAM;                              \
            }                                                                 \
        }                                                                     \
    } while (0)  			

#ifdef _MSC_VER
#define MUTEX_INIT_LOCK(mutex) InitializeCriticalSection(&mutex)
#define MUTEX_LOCK(mutex) EnterCriticalSection(&mutex)
#define MUTEX_UNLOCK(mutex) LeaveCriticalSection(&mutex)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(&mutex)
#else
#define MUTEX_INIT_LOCK(mutex) \
			do \
			{ \
			(void)pthread_mutex_init(&mutex, NULL); \
			}while(0)
#define MUTEX_LOCK(mutex) \
			do \
			{ \
			(void)pthread_mutex_lock(&mutex); \
			}while(0)
#define MUTEX_UNLOCK(mutex) \
			do \
			{ \
			(void)pthread_mutex_unlock(&mutex); \
			}while(0)
#define MUTEX_DESTROY(mutex) \
			do \
			{ \
			(void)pthread_mutex_destroy(&mutex); \
			}while(0)
#endif


/*****************************************************************************
 Prototype       : isp pub configure
 Description     : need I/F cowork:
                    OB area, need to configure window, and I/F font edge;
                    others configure I/F as back edge;                    
 Input           : None
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/14
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetPubAttr(ISP_DEV IspDev, const ISP_PUB_ATTR_S *pstPubAttr)
{
    HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    int i;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
   
    if (pstPubAttr->enBayer >= BAYER_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Bayer Pattern:%d!\n", pstPubAttr->enBayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	
	if ( (pstPubAttr->stWndRect.u32Width > 2048)||(pstPubAttr->stWndRect.u32Width< 120)||(pstPubAttr->stWndRect.u32Width % 2 != 0))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Width:%d!\n", pstPubAttr->stWndRect.u32Width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.u32Height > 2048)||(pstPubAttr->stWndRect.u32Height < 120)||(pstPubAttr->stWndRect.u32Height % 2 != 0))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Height:%d!\n", pstPubAttr->stWndRect.u32Height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_res_switch_write(HI_FALSE);
    
	hi_isp_crop_en_write(IspDev, HI_TRUE);
	hi_isp_crop_x_start_write(IspDev, pstPubAttr->stWndRect.s32X);
	hi_isp_crop_y_start_write(IspDev, pstPubAttr->stWndRect.s32Y);
	hi_isp_crop_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_crop_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);

	hi_isp_yuv444_rggb_start_write(IspDev, (HI_U8)pstPubAttr->enBayer);
	hi_isp_yuv422_rggb_start_write(IspDev, (HI_U8)pstPubAttr->enBayer);
	hi_ext_sync_total_width_write(pstPubAttr->stWndRect.u32Width);
    hi_ext_sync_total_height_write(pstPubAttr->stWndRect.u32Height);
	hi_ext_system_bas_outwidth_write(pstPubAttr->stWndRect.u32Width);
	hi_ext_system_bas_outheight_write(pstPubAttr->stWndRect.u32Height);

    hi_isp_dci_hpos_write(IspDev, 0, pstPubAttr->stWndRect.u32Width);   
	hi_isp_dci_vpos_write(IspDev, 0, pstPubAttr->stWndRect.u32Height);
	hi_isp_acm_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_acm_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
    hi_isp_ae_width_write(IspDev,pstPubAttr->stWndRect.u32Width-1);
    hi_isp_ae_height_write(IspDev,pstPubAttr->stWndRect.u32Height-1);
    hi_isp_mg_width_write(IspDev,pstPubAttr->stWndRect.u32Width-1);
    hi_isp_mg_height_write(IspDev,pstPubAttr->stWndRect.u32Height-1);
    for(i = 0; i<4;i++)
    {
        hi_isp_ae_wdr_hsize_write(IspDev,i,pstPubAttr->stWndRect.u32Width-1);
        hi_isp_ae_wdr_vsize_write(IspDev,i,pstPubAttr->stWndRect.u32Height-1);
    }
    hi_isp_awb_hsize_write(IspDev,pstPubAttr->stWndRect.u32Width-1);
    hi_isp_awb_vsize_write(IspDev,pstPubAttr->stWndRect.u32Height-1);
    hi_isp_bnr_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_bnr_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_ca_width_write(IspDev,pstPubAttr->stWndRect.u32Width-1);
	hi_isp_ca_height_write(IspDev,pstPubAttr->stWndRect.u32Height - 1);
	hi_isp_gcac_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_gcac_height_write(IspDev,   pstPubAttr->stWndRect.u32Height-1);
	hi_isp_dpc_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_dpc_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_demosaic_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_demosaic_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_drc_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_drc_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_flick_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_flick_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_fpn_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_fpn_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_wdr_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_wdr_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_ge_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
	hi_isp_ge_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
	hi_isp_sharpen_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
    hi_isp_sharpen_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
    hi_vi_af_hsize_write(pstPubAttr->stWndRect.u32Width-1);
    hi_vi_af_vsize_write(pstPubAttr->stWndRect.u32Height-1); 
    hi_isp_dehaze_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
    hi_isp_dehaze_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
    hi_isp_nddm_width_write(IspDev, pstPubAttr->stWndRect.u32Width-1);
    hi_isp_nddm_height_write(IspDev, pstPubAttr->stWndRect.u32Height-1);
    pValue = (HI_VOID *)(&pstPubAttr->f32FrameRate);
    hi_ext_system_fps_base_write(*(HI_U32 *)pValue);

	hi_isp_yuv444_timing_stat_write(IspDev, HI_ISP_CPI_FIX_TIMING_STAT);
	hi_isp_yuv422_timing_stat_write(IspDev, HI_ISP_CPI_FIX_TIMING_STAT);

    pstIspCtx->stIspParaRec.bPubCfg = HI_TRUE;
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);
   
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetPubAttr(ISP_DEV IspDev, ISP_PUB_ATTR_S *pstPubAttr)
{
    HI_U8 u8Bayer;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8Bayer = hi_isp_yuv444_rggb_start_read(IspDev);
    if (u8Bayer >= BAYER_BUTT)
    {
        pstPubAttr->enBayer = BAYER_BUTT;
    }
    else
    {
        pstPubAttr->enBayer = u8Bayer;
    }
    
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    pstPubAttr->f32FrameRate = *(HI_FLOAT *)pValue;

    pstPubAttr->stWndRect.s32X      = hi_isp_crop_x_start_read(IspDev);
    pstPubAttr->stWndRect.u32Width  = hi_ext_sync_total_width_read();
    pstPubAttr->stWndRect.s32Y      = hi_isp_crop_y_start_read(IspDev);
    pstPubAttr->stWndRect.u32Height = hi_ext_sync_total_height_read();
	    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFMWState(ISP_DEV IspDev, const ISP_FMW_STATE_E enState)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (enState >= ISP_FMW_STATE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid firmware state %d in %s!\n", enState, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_freeze_firmware_write((HI_U8)enState);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFMWState(ISP_DEV IspDev, ISP_FMW_STATE_E *penState)
{
    HI_U8 u8FMWState;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(penState);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8FMWState = hi_ext_top_freeze_firmware_read();

    if (u8FMWState >=  ISP_FMW_STATE_BUTT)
    {
        *penState = ISP_FMW_STATE_BUTT;
    }
    else
    {
        *penState = u8FMWState;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetWDRMode(ISP_DEV IspDev, const ISP_WDR_MODE_S *pstWDRMode)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    
    if (pstWDRMode->enWDRMode >= WDR_MODE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid WDR mode %d in %s!\n", pstWDRMode->enWDRMode, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_wdr_switch_write(HI_FALSE);

    pstIspCtx->stIspParaRec.bWDRCfg = HI_TRUE;
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);

    if ((HI_U8)pstWDRMode->enWDRMode == hi_ext_top_sensor_wdr_mode_read())
    {
        hi_ext_top_wdr_switch_write(HI_TRUE);
        return HI_SUCCESS;
    }
    
    hi_ext_top_sensor_wdr_mode_write((HI_U8)pstWDRMode->enWDRMode);
  
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetWDRMode(ISP_DEV IspDev, ISP_WDR_MODE_S *pstWDRMode)
{
    HI_U8 u8WDRmode;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8WDRmode = hi_ext_top_sensor_wdr_mode_read();
    if (u8WDRmode >= WDR_MODE_BUTT)
    {
        pstWDRMode->enWDRMode = WDR_MODE_BUTT;
    }
    else
    {
        pstWDRMode->enWDRMode = u8WDRmode;
    }   
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetModuleControl(ISP_DEV IspDev, const ISP_MODULE_CTRL_U *punModCtrl)
{
    HI_U32 u32Key = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u32Key = punModCtrl->u32Key;

	hi_ext_system_isp_bit_bypass_write(u32Key);	
	hi_isp_color_bar_cfg_write(IspDev,!(u32Key&0x1));
	hi_ext_system_isp_balancefe_enable_write(!((u32Key >> 1)&0x1));
	hi_isp_dg_enable_write(IspDev,(!((u32Key >> 2)&0x1)));
	hi_ext_system_fcr_enable_write(!((u32Key >> 3)&0x1));
	hi_ext_system_ge_enable_write(!((u32Key >> 4)&0x1));
	hi_ext_system_dpc_dynamic_cor_enable_write(!((u32Key >> 5)&0x1));
	hi_ext_system_bayernr_enable_write(!((u32Key >> 6)&0x1));
	hi_isp_dehaze_en_write(IspDev,(!((u32Key >> 7)&0x1)));
    hi_isp_wb_enable_write(IspDev,(!((u32Key >> 8)&0x1)));
	hi_ext_system_isp_mesh_shading_enable_write(!((u32Key >> 9)&0x1));
	hi_isp_acm_enable_write(IspDev,!(((u32Key >> 10)&0x1)));
	hi_ext_system_drc_enable_write((!((u32Key >> 11)&0x1)));
	hi_ext_system_demosaic_enable_write(!((u32Key >> 12)&0x1));
	hi_isp_cc_enable_write(IspDev,(!((u32Key >> 13)&0x1)));
	hi_isp_gamma_enable_write(IspDev,(!((u32Key >> 14)&0x1)));
	hi_isp_wdr_enable_write(IspDev,(!((u32Key >> 15)&0x1)));
	hi_isp_ca_cfg_en_write(IspDev,(!((u32Key >> 16)&0x1)));
	hi_ext_system_top_bit_reserve0_write((u32Key >> 17)&0x3);
	hi_isp_csc_enable_write(IspDev,(!((u32Key >> 19)&0x1)));
	hi_ext_system_top_bit_reserve1_write((u32Key >> 20)&0x3);
	hi_isp_sharpen_cfg_enable_write(IspDev,(!((u32Key >> 22)&0x1)));
	hi_ext_system_localCAC_enable_write((!((u32Key >> 23)&0x1)));
    //hi_isp_top_chn_switch0_write(IspDev,(!((u32Key >> 24)&0x1)));
    hi_ext_system_chnswitch_write((!((u32Key >> 24)&0x1)));
	hi_ext_system_top_bit_reserve2_write((u32Key >> 25)&0x3);
	hi_ext_system_top_bit_reserve3_write((u32Key >> 27)&0x3);
	hi_ext_system_top_bit_reserve4_write((u32Key >> 29)&0x7);



    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetModuleControl(ISP_DEV IspDev, ISP_MODULE_CTRL_U *punModCtrl)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	punModCtrl->bitBypassVideoTest = !(hi_isp_color_bar_cfg_read(IspDev));
	punModCtrl->bitBypassBalanceFe = !(hi_ext_system_isp_balancefe_enable_read());
    punModCtrl->bitBypassISPDGain = !(hi_isp_dg_enable_read(IspDev));
	punModCtrl->bitBypassAntiFC = !(hi_ext_system_fcr_enable_read());
	punModCtrl->bitBypassCrosstalkR = !(hi_ext_system_ge_enable_read());
	punModCtrl->bitBypassDPC = !(hi_ext_system_dpc_dynamic_cor_enable_read());
	punModCtrl->bitBypassNR = !(hi_ext_system_bayernr_enable_read());
	punModCtrl->bitBypassDehaze = !(hi_isp_dehaze_en_read(IspDev));
    punModCtrl->bitBypassWBGain = !(hi_isp_wb_enable_read(IspDev));
    punModCtrl->bitBypassShading = !(hi_ext_system_isp_mesh_shading_enable_read());
    punModCtrl->bitBypassACM = !(hi_isp_acm_enable_read(IspDev));
    punModCtrl->bitBypassDRC = !(hi_ext_system_drc_enable_read());
    punModCtrl->bitBypassDemosaic = !(hi_ext_system_demosaic_enable_read());
    punModCtrl->bitBypassColorMatrix = !(hi_isp_cc_enable_read(IspDev));
    punModCtrl->bitBypassGamma = !(hi_isp_gamma_enable_read(IspDev));
    punModCtrl->bitBypassFSWDR = !(hi_isp_wdr_enable_read(IspDev));
	punModCtrl->bitBypassCA = !(hi_isp_ca_cfg_en_read(IspDev));
	punModCtrl->bitRsv0       = hi_ext_system_top_bit_reserve0_read();
    punModCtrl->bitBypassCsConv = !(hi_isp_csc_enable_read(IspDev));
	punModCtrl->bitRsv1 = hi_ext_system_top_bit_reserve1_read();
    punModCtrl->bitBypassSharpen = !(hi_isp_sharpen_cfg_enable_read(IspDev));
	punModCtrl->bitBypassCAC = !(hi_ext_system_localCAC_enable_read());
	punModCtrl->bitChnSwitch = !(hi_isp_top_chn_switch0_read(IspDev));
	punModCtrl->bitRsv2     = hi_ext_system_top_bit_reserve2_read();
	punModCtrl->bitRsv3     = hi_ext_system_top_bit_reserve3_read();
	punModCtrl->bitRsv4     = hi_ext_system_top_bit_reserve4_read();
    
    return HI_SUCCESS;
}

/* General Function Settings */
HI_S32 HI_MPI_ISP_SetDRCAttr(ISP_DEV IspDev, const ISP_DRC_ATTR_S *pstDRC)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_BOOL(pstDRC->bEnable);
    ISP_CHECK_BOOL(pstDRC->bUserTmEnable);
    ISP_CHECK_BOOL(pstDRC->bUserTmUpdate);
    
    if ((pstDRC->u8Asymmetry > 0x1E)||(pstDRC->u8Asymmetry < 0x1))
    {
        printf("Invalid u8Asymmetry Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8LocalMixingBrigtht > 0x80))
    {
        printf("Invalid u8LocalMixingBrigtht Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8LocalMixingBrigthtMax > 0x80))
    {
        printf("Invalid u8LocalMixingBrigthtMax Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8LocalMixingDarkMin > 0x80) )
    {
        printf("Invalid u8LocalMixingDarkMin Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8LocalMixingDark > 0x80))
    {
        printf("Invalid u8LocalMixingDark Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstDRC->u8LocalMixingThres > 0x80))
    {
        printf("Invalid u8LocalMixingThres Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstDRC->u8RangeVar > 0xF)
    {
        printf("Invalid u8RangeVar Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstDRC->u8SpatialVar > 0xF)
    {
        printf("Invalid u8SpatialVar Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8RangeVarFlt1 > 0xF)
    {
        printf("Invalid u8RangeVarFlt1 Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8SpatialVarFlt1 > 0xF)
    {
        printf("Invalid u8SpatialVarFlt1 Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8RangeVarFlt2 > 0xF)
    {
        printf("Invalid u8RangeVarFlt2 Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8SpatialVarFlt2 > 0xF)
    {
        printf("Invalid u8SpatialVarFlt2 Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8FltBldr > 0xF)
    {
        printf("Invalid u8FltBldr Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8SpatialStr > 0xF)
    {
        printf("Invalid u8SpatialStr Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if ((pstDRC->u8SecondPole > 0xD2)||(pstDRC->u8SecondPole < 0x96))
    {
        printf("Invalid u8SecondPole Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if ((pstDRC->u8Stretch > 0x3C) ||(pstDRC->u8Stretch < 0x1E))
    {
        printf("Invalid u8Stretch Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8Compress > 0xc8) ||(pstDRC->u8Compress < 0x64))
    {
        printf("Invalid u8Compress Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8PDStrength > 0x80))
    {
        printf("Invalid u8PDStrength Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u16BrightGainLmt > 0xA0)
    {
        printf("Invalid u16BrightGainLmt Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u16DarkGainLmtC > 0x85)
    {
        printf("Invalid u16DarkGainLmtC Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u16DarkGainLmtY > 0x85)
    {
        printf("Invalid u16DarkGainLmtY Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8GainClipKnee > 0xF)
    {
        printf("Invalid u8GainClipKnee Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->u8GainClipStep > 0xF)
    {
        printf("Invalid u8GainClipStep Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

	switch (pstDRC->enOpType)
	{
		case OP_TYPE_AUTO:
			if (pstDRC->stAuto.u8Strength > 0xFF)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid u8Strength Parameter Input in %s!\n", __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if (pstDRC->stAuto.u8StrengthMax > 0xFF || pstDRC->stAuto.u8StrengthMax < pstDRC->stAuto.u8StrengthMin)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid u8StrengthMax Parameter Input in %s!\n", __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if (pstDRC->stAuto.u8StrengthMin > 0xFF || pstDRC->stAuto.u8StrengthMax < pstDRC->stAuto.u8StrengthMin)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid u8StrengthMin Parameter Input in %s!\n", __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			hi_ext_system_drc_manual_mode_write(0);
			break;
		case OP_TYPE_MANUAL:
			if (pstDRC->stManual.u8Strength > 0xFF)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid u8Strength Parameter Input in %s!\n", __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			hi_ext_system_drc_manual_mode_write(1);
			break;
		default:
			ISP_TRACE(HI_DBG_ERR, "Invalid DRC Op mode in %s!\n", __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
			break;
	}
	
	hi_ext_system_drc_auto_strength_write(pstDRC->stAuto.u8Strength);
	hi_ext_system_drc_manual_strength_write(pstDRC->stManual.u8Strength);

	hi_ext_system_drc_enable_write(pstDRC->bEnable);

	hi_ext_system_drc_asymmetry_write(pstDRC->u8Asymmetry);
	hi_ext_system_drc_secondpole_write(pstDRC->u8SecondPole);
	hi_ext_system_drc_stretch_write(pstDRC->u8Stretch);	
	hi_ext_system_drc_compress_write(pstDRC->u8Compress);

	hi_ext_system_drc_pdw_high_write(pstDRC->u8PDStrength);

	hi_ext_system_drc_mixing_bright_min_write( pstDRC->u8LocalMixingBrigtht);
    hi_ext_system_drc_mixing_bright_max_write(pstDRC->u8LocalMixingBrigthtMax);
	hi_ext_system_drc_mixing_dark_min_write(pstDRC->u8LocalMixingDarkMin);
	hi_ext_system_drc_mixing_dark_max_write(pstDRC->u8LocalMixingDark);
    hi_ext_system_drc_mixing_coring_write(pstDRC->u8LocalMixingThres);

	for (i = 0; i < 33; i++)
	{
		hi_ext_system_drc_colorcc_lut_write(i, pstDRC->u16ColorCorrectionLut[i]);
	}

	hi_ext_system_drc_user_tm_enable_write(pstDRC->bUserTmEnable);
	hi_ext_system_drc_user_tm_update_write(pstDRC->bUserTmUpdate);
	for (i = 0; i < DRC_TM_NODE_NUM; i++)
	{
		hi_ext_system_drc_tm_lut_write(i, pstDRC->au16ToneMappingValue[i]);
	}

	hi_ext_system_drc_auto_strength_max_write(pstDRC->stAuto.u8StrengthMax);
	hi_ext_system_drc_auto_strength_min_write(pstDRC->stAuto.u8StrengthMin);

	hi_ext_system_drc_var_rng_fine_write(pstDRC->u8RangeVar);
	hi_ext_system_drc_bin_mix_medium_write(pstDRC->u8SpatialVar);
    hi_ext_system_drc_bin_mix_coarse_write(pstDRC->u8FltBldr);
    hi_ext_system_drc_var_spa_fine_write(pstDRC->u8SpatialStr);
    hi_ext_system_drc_var_rng_medium_write(pstDRC->u8RangeVarFlt1);
    hi_ext_system_drc_var_spa_medium_write(pstDRC->u8SpatialVarFlt1);
    hi_ext_system_drc_var_rng_coarse_write(pstDRC->u8RangeVarFlt2);
    hi_ext_system_drc_var_spa_coarse_write(pstDRC->u8SpatialVarFlt2);

    hi_ext_system_drc_dark_gain_lmt_y_write(pstDRC->u16DarkGainLmtY);
    hi_ext_system_drc_dark_gain_lmt_c_write(pstDRC->u16DarkGainLmtC);
    hi_ext_system_drc_bright_gain_lmt_write(pstDRC->u16BrightGainLmt);
    hi_ext_system_drc_gain_clip_knee_write(pstDRC->u8GainClipKnee);
    hi_ext_system_drc_gain_clip_step_write(pstDRC->u8GainClipStep);
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDRCAttr(ISP_DEV IspDev, ISP_DRC_ATTR_S *pstDRC)
{    
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDRC->enOpType = (ISP_OP_TYPE_E)hi_ext_system_drc_manual_mode_read();
    pstDRC->stAuto.u8Strength = hi_ext_system_drc_auto_strength_read();
    pstDRC->stManual.u8Strength = hi_ext_system_drc_manual_strength_read();
	pstDRC->bEnable = hi_ext_system_drc_enable_read();
	
    //pstDRC->u16BrightGainLmt = hi_ext_system_drc_bright_gain_lmt_read();
	//pstDRC->u16DarkGainLmtC  = hi_ext_system_drc_dark_gain_lmt_c_read();
	//pstDRC->u16DarkGainLmtY = hi_ext_system_drc_dark_gain_lmt_y_read();

	pstDRC->u8Asymmetry = hi_ext_system_drc_asymmetry_read();
	pstDRC->u8SecondPole = hi_ext_system_drc_secondpole_read();
	pstDRC->u8Stretch = hi_ext_system_drc_stretch_read();

	pstDRC->u8LocalMixingBrigtht = hi_ext_system_drc_mixing_bright_min_read();
    pstDRC->u8LocalMixingBrigthtMax = hi_ext_system_drc_mixing_bright_max_read();
	pstDRC->u8LocalMixingDarkMin =	hi_ext_system_drc_mixing_dark_min_read();
	pstDRC->u8LocalMixingDark =	hi_ext_system_drc_mixing_dark_max_read();
	pstDRC->u8LocalMixingThres = hi_ext_system_drc_mixing_coring_read();
	pstDRC->u8PDStrength = hi_ext_system_drc_pdw_high_read();

	pstDRC->u8Compress =  hi_ext_system_drc_compress_read();

	pstDRC->u8RangeVar = hi_ext_system_drc_var_rng_fine_read();
	pstDRC->u8SpatialVar = hi_ext_system_drc_bin_mix_medium_read();
    pstDRC->u8SpatialStr = hi_ext_system_drc_var_spa_fine_read();
    pstDRC->u8FltBldr = hi_ext_system_drc_bin_mix_coarse_read();
    pstDRC->u8RangeVarFlt1= hi_ext_system_drc_var_rng_medium_read();
    pstDRC->u8SpatialVarFlt1= hi_ext_system_drc_var_spa_medium_read();
    pstDRC->u8RangeVarFlt2= hi_ext_system_drc_var_rng_coarse_read();
    pstDRC->u8SpatialVarFlt2= hi_ext_system_drc_var_spa_coarse_read();

	for (i = 0; i < 33; i++)
	{        
		pstDRC->u16ColorCorrectionLut[i] = hi_ext_system_drc_colorcc_lut_read(i);
	}

	pstDRC->bUserTmEnable = hi_ext_system_drc_user_tm_enable_read();
	pstDRC->bUserTmUpdate = hi_ext_system_drc_user_tm_update_read();
	for (i = 0; i < DRC_TM_NODE_NUM; i++)
	{
		pstDRC->au16ToneMappingValue[i] = hi_ext_system_drc_tm_lut_read(i);
	}

	pstDRC->stAuto.u8StrengthMax = hi_ext_system_drc_auto_strength_max_read();
	pstDRC->stAuto.u8StrengthMin = hi_ext_system_drc_auto_strength_min_read();

    pstDRC->u16DarkGainLmtY = hi_ext_system_drc_dark_gain_lmt_y_read();
    pstDRC->u16DarkGainLmtC = hi_ext_system_drc_dark_gain_lmt_c_read();
    pstDRC->u16BrightGainLmt = hi_ext_system_drc_bright_gain_lmt_read();
    pstDRC->u8GainClipKnee = hi_ext_system_drc_gain_clip_knee_read();
    pstDRC->u8GainClipStep = hi_ext_system_drc_gain_clip_step_read();

	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDPStaticAttr(ISP_DEV IspDev, const ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
	HI_BOOL bWDRModeEn;
	HI_U16  i = 0, j = 0, m = 0, u16CountIn = 0;
	HI_U32  au32DefectPixelTable[STATIC_DP_COUNT_MAX];
	ISP_CTX_S *pstIspCtx = HI_NULL;

	ISP_GET_CTX(IspDev, pstIspCtx);
	
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPStaticAttr);	
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);	
	ISP_CHECK_BOOL(pstDPStaticAttr->bEnable);
	ISP_CHECK_BOOL(pstDPStaticAttr->bShow);
	
	if (pstDPStaticAttr->u16BrightCount > STATIC_DP_COUNT_MAX)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid StaticAttr u16BrightCount value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstDPStaticAttr->u16DarkCount> STATIC_DP_COUNT_MAX)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid StaticAttr u16DarkCount value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	/* merging dark talbe and bright table */
	while ((i <  pstDPStaticAttr->u16BrightCount) && (j < pstDPStaticAttr->u16DarkCount))
	{
		if (m >= STATIC_DP_COUNT_MAX)
		{
		    printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
		    return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if (pstDPStaticAttr->au32BrightTable[i] > pstDPStaticAttr->au32DarkTable[j])
		{
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32DarkTable[j++];
		}
		else if (pstDPStaticAttr->au32BrightTable[i] < pstDPStaticAttr->au32DarkTable[j])
		{
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i++];
		}
		else
		{
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i];
		    i++;
		    j++;
		}
	}

	if (i >=  pstDPStaticAttr->u16BrightCount)
	{
		while (j < pstDPStaticAttr->u16DarkCount)
		{
		    if (m >= STATIC_DP_COUNT_MAX)
		    {
		        printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
		        return HI_ERR_ISP_ILLEGAL_PARAM;
		    }
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32DarkTable[j++];
		}
	}
	if (j >=  pstDPStaticAttr->u16DarkCount)
	{
		while (i < pstDPStaticAttr->u16BrightCount)
		{
		    if (m >= STATIC_DP_COUNT_MAX)
		    {
		        printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
		        return HI_ERR_ISP_ILLEGAL_PARAM;
		    }
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i++];
		}
	}

	u16CountIn = m;
	
	hi_ext_system_dpc_static_cor_enable_write(pstDPStaticAttr->bEnable);
	hi_ext_system_dpc_static_dp_show_write(pstDPStaticAttr->bShow);
	bWDRModeEn = hi_isp_wdr_enable_read(IspDev);
	//should bypass DPC module when set defect pixel information
	MUTEX_LOCK(pstIspCtx->stLock);
	
    if (bWDRModeEn)
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_FALSE);
    	hi_isp_dpc_enable_write(IspDev, 1, HI_FALSE);
    }
    else
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_FALSE);
    }
	hi_isp_dp_bpt_waddr_write(IspDev, 0);
	hi_isp_dp_bpt_number_write(IspDev, u16CountIn);
	hi_usleep(200000);
	for(i = 0; i < STATIC_DP_COUNT_MAX; i++)
	{		
		if(i <u16CountIn )
		{
			hi_isp_dp_bpt_wdata_write(IspDev,au32DefectPixelTable[i]);
		}
		else
		{
			hi_isp_dp_bpt_wdata_write(IspDev,0);
		}
	}
	
    if (bWDRModeEn)
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_TRUE);
    	hi_isp_dpc_enable_write(IspDev, 1, HI_TRUE);
    }
    else
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_TRUE);
    }
	MUTEX_UNLOCK(pstIspCtx->stLock);
	hi_usleep(200000);	

	return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_GetDPStaticAttr(ISP_DEV IspDev,  ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
	HI_BOOL  bWDRModeEn;
	HI_U32   i = 0;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPStaticAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    ISP_GET_CTX(IspDev, pstIspCtx);
	
	pstDPStaticAttr->bEnable 		=  hi_ext_system_dpc_static_cor_enable_read();
	pstDPStaticAttr->bShow 			= hi_ext_system_dpc_static_dp_show_read();
	pstDPStaticAttr->u16BrightCount = hi_isp_dp_bpt_number_read(IspDev);
	pstDPStaticAttr->u16DarkCount 	= 0;
	bWDRModeEn = hi_isp_wdr_enable_read(IspDev);
	//should bypass DPC module when get defect pixel information
	MUTEX_LOCK(pstIspCtx->stLock);
    if (bWDRModeEn)
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_FALSE);
    	hi_isp_dpc_enable_write(IspDev, 1, HI_FALSE);
    }
    else
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_FALSE);
    }
	hi_isp_dp_bpt_raddr_write(IspDev, 0);
	hi_usleep(200000);

	for(i = 0; i < STATIC_DP_COUNT_MAX; i++)
	{
		if(i < pstDPStaticAttr->u16BrightCount)
		{
			
			pstDPStaticAttr->au32BrightTable[i] = hi_isp_dp_bpt_rdata_read(IspDev);
			
		}
		else
		{
			pstDPStaticAttr->au32BrightTable[i] = 0;
		}
	}
    if (bWDRModeEn)
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_TRUE);
    	hi_isp_dpc_enable_write(IspDev, 1, HI_TRUE);
    }
    else
    {
    	hi_isp_dpc_enable_write(IspDev, 0, HI_TRUE);
    }
	MUTEX_UNLOCK(pstIspCtx->stLock);
	
	hi_usleep(200000);
	return HI_SUCCESS;
}
HI_S32 HI_MPI_ISP_SetDPDynamicAttr(ISP_DEV IspDev, const ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPDynamicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);	
	ISP_CHECK_BOOL(pstDPDynamicAttr->bEnable);
	ISP_CHECK_BOOL(pstDPDynamicAttr->bSupTwinkleEn);

	if(pstDPDynamicAttr->enOpType >= OP_TYPE_BUTT)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid DynamicAttr enOpType value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	
	hi_ext_system_dpc_dynamic_cor_enable_write(pstDPDynamicAttr->bEnable);

	if (OP_TYPE_AUTO == pstDPDynamicAttr->enOpType)
	{
		hi_ext_system_dpc_dynamic_manual_enable_write(pstDPDynamicAttr->enOpType);
		
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{	
			if(pstDPDynamicAttr->stAuto.au16Strength[i] > 255)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid DynamicAttr au16Strength[%d] value in %s!\n",i, __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if (pstDPDynamicAttr->stAuto.au16BlendRatio[i]>0x100)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid DynamicAttr au16BlendRatio[%d] value in %s!\n",i, __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			
			hi_ext_system_dpc_dynamic_strength_table_write(i,pstDPDynamicAttr->stAuto.au16Strength[i]);
			hi_ext_system_dpc_dynamic_blend_ratio_table_write(i,pstDPDynamicAttr->stAuto.au16BlendRatio[i]);
		}
	}
	else if(OP_TYPE_MANUAL == pstDPDynamicAttr->enOpType)
	{       
		if(pstDPDynamicAttr->stManual.u16Strength > 255)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid DynamicAttr Strength value in %s!\n", __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if (pstDPDynamicAttr->stManual.u16BlendRatio>0x100)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid DynamicAttr BlendRatio value in %s!\n", __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
		
		hi_ext_system_dpc_dynamic_manual_enable_write(pstDPDynamicAttr->enOpType);
		hi_ext_system_dpc_dynamic_strength_write(pstDPDynamicAttr->stManual.u16Strength);
		hi_ext_system_dpc_dynamic_blend_ratio_write(pstDPDynamicAttr->stManual.u16BlendRatio);         
	}

	hi_ext_system_dpc_suppress_twinkle_enable_write(pstDPDynamicAttr->bSupTwinkleEn);
	hi_ext_system_dpc_suppress_twinkle_thr_write(pstDPDynamicAttr->s8SoftThr);
	hi_ext_system_dpc_suppress_twinkle_slope_write(pstDPDynamicAttr->u8SoftSlope);
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDPDynamicAttr(ISP_DEV IspDev,  ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPDynamicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	pstDPDynamicAttr->bEnable 		= hi_ext_system_dpc_dynamic_cor_enable_read ();
	pstDPDynamicAttr->enOpType 		= (ISP_OP_TYPE_E)hi_ext_system_dpc_dynamic_manual_enable_read();
	pstDPDynamicAttr->bSupTwinkleEn = hi_ext_system_dpc_suppress_twinkle_enable_read();
	pstDPDynamicAttr->s8SoftThr		= hi_ext_system_dpc_suppress_twinkle_thr_read();
	pstDPDynamicAttr->u8SoftSlope	= hi_ext_system_dpc_suppress_twinkle_slope_read();

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		pstDPDynamicAttr->stAuto.au16Strength[i] 		= hi_ext_system_dpc_dynamic_strength_table_read(i);
		pstDPDynamicAttr->stAuto.au16BlendRatio[i] 	= hi_ext_system_dpc_dynamic_blend_ratio_table_read(i);
	}

	pstDPDynamicAttr->stManual.u16Strength 	 = hi_ext_system_dpc_dynamic_strength_read();
	pstDPDynamicAttr->stManual.u16BlendRatio = hi_ext_system_dpc_dynamic_blend_ratio_read();  

	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetLocalCacAttr(ISP_DEV IspDev, const ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstLocalCacAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);	
	ISP_CHECK_BOOL(pstLocalCacAttr->bEnable);

	
	if(pstLocalCacAttr->enOpType >= OP_TYPE_BUTT)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr enOpType value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	
	if (pstLocalCacAttr->u16VarThr > 4095)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr u16VarThr value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if (pstLocalCacAttr->u16PurpleDetRange > 410)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr u8PurpleDetRange value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	hi_ext_system_localCAC_manual_enable_write(pstLocalCacAttr->enOpType);
	
	for (i = 0; i < 16; i++)
	{	
		if(pstLocalCacAttr->stAuto.au8DePurpleCrStr[i] > 8)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr au8DePurpleCrStr[%d] value in %s!\n",i, __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstLocalCacAttr->stAuto.au8DePurpleCbStr[i] > 8)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr au8DePurpleCbStr[%d] value in %s!\n",i, __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
				
		hi_ext_system_localCAC_auto_cb_str_table_write(i,pstLocalCacAttr->stAuto.au8DePurpleCbStr[i]);
		hi_ext_system_localCAC_auto_cr_str_table_write(i,pstLocalCacAttr->stAuto.au8DePurpleCrStr[i]);
	}

	if(pstLocalCacAttr->stManual.u8DePurpleCrStr> 8)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr u8DePurpleCrStr value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstLocalCacAttr->stManual.u8DePurpleCbStr> 8)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid LocalCacAttr u8DePurpleCrStr value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
				
	hi_ext_system_localCAC_manual_cb_str_write(pstLocalCacAttr->stManual.u8DePurpleCbStr);
	hi_ext_system_localCAC_manual_cr_str_write(pstLocalCacAttr->stManual.u8DePurpleCrStr);
	

	hi_ext_system_localCAC_enable_write(pstLocalCacAttr->bEnable);

	hi_ext_system_localCAC_purple_var_thr_write(pstLocalCacAttr->u16VarThr);
	hi_ext_system_localCAC_purple_det_range_write(pstLocalCacAttr->u16PurpleDetRange);
	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetLocalCacAttr(ISP_DEV IspDev, ISP_LOCAL_CAC_ATTR_S *pstLocalCacAttr)
{	
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstLocalCacAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	pstLocalCacAttr->bEnable 	 		= hi_ext_system_localCAC_enable_read();//hi_isp_demosaic_local_cac_enable_read(IspDev);
	pstLocalCacAttr->u16PurpleDetRange 	= hi_ext_system_localCAC_purple_det_range_read();
	pstLocalCacAttr->u16VarThr 	 		= hi_ext_system_localCAC_purple_var_thr_read();//hi_isp_demosaic_purple_var_thr_read(IspDev);
	pstLocalCacAttr->enOpType			= hi_ext_system_localCAC_manual_enable_read();
	pstLocalCacAttr->stManual.u8DePurpleCbStr = hi_ext_system_localCAC_manual_cb_str_read();
	pstLocalCacAttr->stManual.u8DePurpleCrStr = hi_ext_system_localCAC_manual_cr_str_read();
	
	
	for(i = 0; i < 16; i++)
	{
		pstLocalCacAttr->stAuto.au8DePurpleCbStr[i] = hi_ext_system_localCAC_auto_cb_str_table_read(i);
		pstLocalCacAttr->stAuto.au8DePurpleCrStr[i] = hi_ext_system_localCAC_auto_cr_str_table_read(i);
	}	
	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetGlobalCacAttr(ISP_DEV IspDev, const ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr)
{
	HI_U16 u16Width = 0, u16Height = 0;
		
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstGlobalCacAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);	
	ISP_CHECK_BOOL(pstGlobalCacAttr->bEnable);

	u16Width = hi_ext_sync_total_width_read();
	u16Height = hi_ext_sync_total_height_read();

	if (pstGlobalCacAttr->u16HorCoordinate > u16Width)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u16HorCoordinate value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if (pstGlobalCacAttr->u16VerCoordinate > u16Height)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u16VerCoordinate value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if ((pstGlobalCacAttr->s16ParamRedA > 255) || (pstGlobalCacAttr->s16ParamRedA < -256))
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr s16ParamRedA value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if ((pstGlobalCacAttr->s16ParamRedB > 255) || (pstGlobalCacAttr->s16ParamRedB < -256))
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr s16ParamRedB value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if ((pstGlobalCacAttr->s16ParamRedC > 255) || (pstGlobalCacAttr->s16ParamRedC < -256))
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr s16ParamRedC value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if ((pstGlobalCacAttr->s16ParamBlueA > 255) || (pstGlobalCacAttr->s16ParamBlueA < -256))
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr s16ParamBlueA value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if ((pstGlobalCacAttr->s16ParamBlueB > 255) || (pstGlobalCacAttr->s16ParamBlueB < -256))
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr s16ParamBlueB value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if ((pstGlobalCacAttr->s16ParamBlueC > 255) || (pstGlobalCacAttr->s16ParamBlueC < -256))
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr s16ParamBlueC value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if (pstGlobalCacAttr->u8VerNormShift > 7)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u8VerNormShift value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstGlobalCacAttr->u8VerNormFactor > 31)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u8VerNormFactor value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if (pstGlobalCacAttr->u8HorNormShift > 7)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u8HorNormShift value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstGlobalCacAttr->u8HorNormFactor > 31)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u8HorNormFactor value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if (pstGlobalCacAttr->u16CorVarThr > 4095)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid GlobalCacAttr u16CorVarThr value in %s!\n", __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	hi_isp_gcac_enable_write(IspDev, pstGlobalCacAttr->bEnable);
	hi_isp_gcac_cnt_start_hor_write(IspDev, pstGlobalCacAttr->u16HorCoordinate);
	hi_isp_gcac_cnt_start_ver_write(IspDev, pstGlobalCacAttr->u16VerCoordinate);
	hi_isp_gcac_param_red_A_write(IspDev, pstGlobalCacAttr->s16ParamRedA);
	hi_isp_gcac_param_red_B_write(IspDev, pstGlobalCacAttr->s16ParamRedB);
	hi_isp_gcac_param_red_C_write(IspDev, pstGlobalCacAttr->s16ParamRedC);
	hi_isp_gcac_param_blue_A_write(IspDev, pstGlobalCacAttr->s16ParamBlueA);
	hi_isp_gcac_param_blue_B_write(IspDev, pstGlobalCacAttr->s16ParamBlueB);
	hi_isp_gcac_param_blue_C_write(IspDev, pstGlobalCacAttr->s16ParamBlueC);


	hi_isp_gcac_y_ns_norm_write(IspDev, pstGlobalCacAttr->u8VerNormShift);
	hi_isp_gcac_y_nf_norm_write(IspDev, pstGlobalCacAttr->u8VerNormFactor);
	hi_isp_gcac_x_ns_norm_write(IspDev, pstGlobalCacAttr->u8HorNormShift);
	hi_isp_gcac_x_nf_norm_write(IspDev, pstGlobalCacAttr->u8HorNormFactor);

	hi_isp_demosaic_varthrforblend_write(IspDev, pstGlobalCacAttr->u16CorVarThr);

	return HI_SUCCESS;	
}
HI_S32 HI_MPI_ISP_GetGlobalCacAttr(ISP_DEV IspDev, ISP_GLOBAL_CAC_ATTR_S *pstGlobalCacAttr)
{

	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstGlobalCacAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	pstGlobalCacAttr->bEnable 			= hi_isp_gcac_enable_read(IspDev);
	pstGlobalCacAttr->u16HorCoordinate 	= hi_isp_gcac_cnt_start_hor_read(IspDev);
	pstGlobalCacAttr->u16VerCoordinate 	= hi_isp_gcac_cnt_start_ver_read(IspDev);
	pstGlobalCacAttr->s16ParamRedA 		= hi_isp_gcac_param_red_A_read(IspDev);
	pstGlobalCacAttr->s16ParamRedB 		= hi_isp_gcac_param_red_B_read(IspDev);
	pstGlobalCacAttr->s16ParamRedC 		= hi_isp_gcac_param_red_C_read(IspDev);
	pstGlobalCacAttr->s16ParamBlueA 	= hi_isp_gcac_param_blue_A_read(IspDev);
	pstGlobalCacAttr->s16ParamBlueB 	= hi_isp_gcac_param_blue_B_read(IspDev);
	pstGlobalCacAttr->s16ParamBlueC 	= hi_isp_gcac_param_blue_C_read(IspDev);
	pstGlobalCacAttr->u8VerNormShift 	= hi_isp_gcac_y_ns_norm_read(IspDev);
	pstGlobalCacAttr->u8VerNormFactor 	= hi_isp_gcac_y_nf_norm_read(IspDev);
	pstGlobalCacAttr->u8HorNormShift 	= hi_isp_gcac_x_ns_norm_read(IspDev);
	pstGlobalCacAttr->u8HorNormFactor 	= hi_isp_gcac_x_nf_norm_read(IspDev);
	pstGlobalCacAttr->u16CorVarThr 		= hi_isp_demosaic_varthrforblend_read(IspDev);

	return HI_SUCCESS;

}


HI_S32 HI_MPI_ISP_SetFSWDRAttr(ISP_DEV IspDev, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);	
	ISP_CHECK_BOOL(pstFSWDRAttr->bMotionComp);
	ISP_CHECK_BOOL(pstFSWDRAttr->bMDRefNoise);

	if (pstFSWDRAttr->u16ShortThr > 0x3FFF)
	{
		printf("Invalid u16ShortThr !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstFSWDRAttr->u16LongThr > 0x3FFF)
	{
		printf("Invalid u16LongThr !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstFSWDRAttr->u16LongThr > pstFSWDRAttr->u16ShortThr)
	{
		printf("u16LongThresh should NOT be larger than u16ShortThresh !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstFSWDRAttr->u16MDSfNrThr > 0x1FF)
	{
		printf("Invalid u16MDSfNrThr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}    
	if (pstFSWDRAttr->enFSWDRComMode >= FS_WDR_COMBINE_BUTT)
	{
		printf("Invalid enFSWDRComMode !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstFSWDRAttr->enOpType >= OP_TYPE_BUTT)
	{
		printf("Invalid  enOpType Input!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	hi_ext_system_wdr_manual_mode_write(pstFSWDRAttr->enOpType);

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{   
	   if (pstFSWDRAttr->stAuto.au16MDClipThr[i] > 0x3FFF)
		{
			printf("Invalid au16MDClipThr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}  
		if (pstFSWDRAttr->stAuto.au8MDBldRatio[i] > 0x1F)
		{
			printf("Invalid au8MDBldRatio!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		} 
		if (pstFSWDRAttr->stAuto.au8MDHighThr[i] > 0x3F)
		{
			printf("Invalid au8MDHighThr !\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		} 
		if (pstFSWDRAttr->stAuto.au8MDLowThr[i] > 0x3F)
		{
			printf("Invalid au8MDLowThr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		} 
		if (pstFSWDRAttr->stAuto.au8MDLowThr[i] > pstFSWDRAttr->stAuto.au8MDHighThr[i] )
		{
			printf("au8MDLowThr should NOT be larger than au8MDHighThr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
		
		hi_ext_system_wdr_lutmdtlowth_write	(i, pstFSWDRAttr->stAuto.au8MDLowThr[i]);
		hi_ext_system_wdr_lutmdthigth_write	(i, pstFSWDRAttr->stAuto.au8MDHighThr[i]);
		hi_ext_system_wdr_lutmdtclip_write 	(i, pstFSWDRAttr->stAuto.au16MDClipThr[i] );
		hi_ext_system_wdr_mdtmaxth_write     (i, pstFSWDRAttr->stAuto.au8MDBldRatio[i]);			
	}

	if (pstFSWDRAttr->stManual.u16MDClipThr> 0x3FFF)
	{
		printf("Invalid u16MDClipThr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstFSWDRAttr->stManual.u8MDBldRatio > 0x1F)
	{
		printf("Invalid u8MDBldRatio !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	} 
	if (pstFSWDRAttr->stManual.u8MDHighThr > 0x3F)
	{
		printf("Invalid u8MDHighThr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	} 
	if (pstFSWDRAttr->stManual.u8MDLowThr > 0x3F)
	{
		printf("Invalid u8MDLowThr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstFSWDRAttr->stManual.u8MDLowThr> pstFSWDRAttr->stManual.u8MDHighThr )
	{
		printf("u8MDLowThr should NOT be larger than u8MDHighThr !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	hi_ext_system_manual_wdr_lutmdtlowth_write(pstFSWDRAttr->stManual.u8MDLowThr);
	hi_ext_system_manual_wdr_lutmdthigth_write(pstFSWDRAttr->stManual.u8MDHighThr);
	hi_ext_system_manual_wdr_lutmdtclip_write (pstFSWDRAttr->stManual.u16MDClipThr );
	hi_ext_system_manual_wdr_mdtmaxth_write(pstFSWDRAttr->stManual.u8MDBldRatio);

	//hi_ext_system_wdr_mdtcomp_write(pstFSWDRAttr->bMotionComp);
	hi_ext_system_wdr_mdtnosref_write(pstFSWDRAttr->bMDRefNoise);
	hi_ext_system_wdr_mdtcomp_mode_write(pstFSWDRAttr->enFSWDRComMode);
	//hi_ext_system_wdr_longthr_write(pstFSWDRAttr->u16LongThr);
	//hi_ext_system_wdr_shortthr_write(pstFSWDRAttr->u16ShortThr);
	hi_ext_system_wdr_snthr_write(pstFSWDRAttr->u16MDSfNrThr);

	hi_isp_wdr_balgprocmdt_write(IspDev,pstFSWDRAttr->bMotionComp);
	hi_isp_wdr_flrgtth_low_write (IspDev, pstFSWDRAttr->u16LongThr);
	hi_isp_wdr_flrgtth_high_write(IspDev, pstFSWDRAttr->u16ShortThr);
	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFSWDRAttr(ISP_DEV IspDev, ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	//pstFSWDRAttr->bMotionComp			=	hi_ext_system_wdr_mdtcomp_read();	
	pstFSWDRAttr->bMDRefNoise			=	hi_ext_system_wdr_mdtnosref_read();
	pstFSWDRAttr->enFSWDRComMode		=	hi_ext_system_wdr_mdtcomp_mode_read();
	//pstFSWDRAttr->u16LongThr			=	hi_ext_system_wdr_longthr_read();
	//pstFSWDRAttr->u16ShortThr			=	hi_ext_system_wdr_shortthr_read();
	pstFSWDRAttr->u16MDSfNrThr			=	hi_ext_system_wdr_snthr_read();

	pstFSWDRAttr->bMotionComp			=	hi_isp_wdr_balgprocmdt_read(IspDev);
	pstFSWDRAttr->u16LongThr			=	hi_isp_wdr_flrgtth_low_read(IspDev);
	pstFSWDRAttr->u16ShortThr			=	hi_isp_wdr_flrgtth_high_read(IspDev);

	pstFSWDRAttr->enOpType                     	= hi_ext_system_wdr_manual_mode_read();
	pstFSWDRAttr->stManual.u16MDClipThr   = hi_ext_system_manual_wdr_lutmdtclip_read ();
	pstFSWDRAttr->stManual.u8MDBldRatio   = hi_ext_system_manual_wdr_mdtmaxth_read(); 
	pstFSWDRAttr->stManual.u8MDHighThr  	= hi_ext_system_manual_wdr_lutmdthigth_read();
	pstFSWDRAttr->stManual.u8MDLowThr 	= hi_ext_system_manual_wdr_lutmdtlowth_read();
 	
	for(i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		pstFSWDRAttr->stAuto.au16MDClipThr[i]	= hi_ext_system_wdr_lutmdtclip_read (i);
		pstFSWDRAttr->stAuto.au8MDBldRatio[i] 	= hi_ext_system_wdr_mdtmaxth_read(i); 
		pstFSWDRAttr->stAuto.au8MDHighThr[i]  	= hi_ext_system_wdr_lutmdthigth_read(i);
		pstFSWDRAttr->stAuto.au8MDLowThr[i] 	= hi_ext_system_wdr_lutmdtlowth_read(i);
	}

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDPCalibrate(ISP_DEV IspDev, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{

    HI_BOOL  bWDRModeEn;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    //ISP_CHECK_BOOL(pstDPCalibrate->bEnable);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnableDetect);

    bWDRModeEn = hi_isp_wdr_enable_read(IspDev);

    if(0 != bWDRModeEn)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid static Calib Mode in %s,The WDR mode calibration is not supported!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }


    if (pstDPCalibrate->u16CountMax > 0x800)//FFF)//2048
    {
		ISP_TRACE(HI_DBG_ERR, "Invalid static Calib  u16CountMax value in %s!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16CountMin > pstDPCalibrate->u16CountMax)
    {
		ISP_TRACE(HI_DBG_ERR, "Invalid static Calib u16CountMin value in %s!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16TimeLimit > 0x640)
    {
		ISP_TRACE(HI_DBG_ERR, "Invalid static Calib u16TimeLimit value in %s!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if(pstDPCalibrate->enStaticDPType >= ISP_STATIC_DP_BUTT)
    {
		ISP_TRACE(HI_DBG_ERR, "Invalid static Calib enStaticDPType value in %s!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(0 == pstDPCalibrate->u8StartThresh)
    {
		ISP_TRACE(HI_DBG_ERR, "Invalid static Calib u8StartThresh value in %s!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }    
    
    hi_ext_system_dpc_static_calib_enable_write(pstDPCalibrate->bEnableDetect);

    hi_ext_system_dpc_static_defect_type_write(pstDPCalibrate->enStaticDPType);
    hi_ext_system_dpc_start_thresh_write(pstDPCalibrate->u8StartThresh);
    hi_ext_system_dpc_count_max_write(pstDPCalibrate->u16CountMax);
    hi_ext_system_dpc_count_min_write(pstDPCalibrate->u16CountMin);
    hi_ext_system_dpc_trigger_time_write(pstDPCalibrate->u16TimeLimit);
    hi_ext_system_dpc_trigger_status_write(ISP_STATE_INIT);

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_GetDPCalibrate(ISP_DEV IspDev, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
	HI_BOOL  bWDRModeEn;
	HI_U16   i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
   
    ISP_GET_CTX(IspDev, pstIspCtx);
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	bWDRModeEn = hi_isp_wdr_enable_read(IspDev);
	pstDPCalibrate->bEnableDetect  = hi_ext_system_dpc_static_calib_enable_read();
	pstDPCalibrate->enStaticDPType = hi_ext_system_dpc_static_defect_type_read();
	pstDPCalibrate->u8StartThresh  = hi_ext_system_dpc_start_thresh_read();
	pstDPCalibrate->u16CountMax    = hi_ext_system_dpc_count_max_read();
	pstDPCalibrate->u16CountMin    = hi_ext_system_dpc_count_min_read();
	pstDPCalibrate->u16TimeLimit   = hi_ext_system_dpc_trigger_time_read();
	pstDPCalibrate->enStatus       = hi_ext_system_dpc_trigger_status_read();
	pstDPCalibrate->u8FinishThresh = hi_ext_system_dpc_finish_thresh_read();
	pstDPCalibrate->u16Count       = hi_isp_dp_bpt_calib_number_read(IspDev);

	if(pstDPCalibrate->enStatus == ISP_STATE_INIT)
	{
		for(i=0; i<STATIC_DP_COUNT_MAX; i++)
		{		
			 pstDPCalibrate->au32Table[i] = 0;       
		}
	}
	else//the calibration process is finished
	{
		//should bypass DPC module when get defect pixel information
		MUTEX_LOCK(pstIspCtx->stLock);
		if (bWDRModeEn)
		{
			hi_isp_dpc_enable_write(IspDev, 0, HI_FALSE);
			hi_isp_dpc_enable_write(IspDev, 1, HI_FALSE);
		}
		else
		{
			hi_isp_dpc_enable_write(IspDev, 0, HI_FALSE);
		}
        hi_isp_dp_bpt_raddr_write(IspDev, 0);
		hi_usleep(200000);
		
		for(i=0; i<STATIC_DP_COUNT_MAX; i++)
		{
			if(i < pstDPCalibrate->u16Count)
			{
				pstDPCalibrate->au32Table[i] = hi_isp_dp_bpt_rdata_read(IspDev);    
			}
			else
			{
				pstDPCalibrate->au32Table[i] = 0;
			}       
		}
		if (bWDRModeEn)
		{
			hi_isp_dpc_enable_write(IspDev, 0, HI_TRUE);
			hi_isp_dpc_enable_write(IspDev, 1, HI_TRUE);
		}
		else
		{
			hi_isp_dpc_enable_write(IspDev, 0, HI_TRUE);
		}
		MUTEX_UNLOCK(pstIspCtx->stLock);
		hi_usleep(200000);
	}	
	return HI_SUCCESS;
}




extern HI_S32 VI_SetDISAttr(VI_DEV ViDev, const VI_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_SetDISAttr(ISP_DEV IspDev, const ISP_DIS_ATTR_S *pstDISAttr)
{

	VI_DIS_ATTR_S stDisAttr = {0};

	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDISAttr);
	ISP_CHECK_BOOL(pstDISAttr->bEnable);
	ISP_CHECK_OPEN(IspDev);
	ISP_CHECK_MEM_INIT(IspDev);    

	stDisAttr.bEnable = pstDISAttr->bEnable;
	return VI_SetDISAttr(0, &stDisAttr);
}

extern HI_S32 VI_GetDISAttr(VI_DEV ViDev, VI_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_GetDISAttr(ISP_DEV IspDev, ISP_DIS_ATTR_S *pstDISAttr)
{

	VI_DIS_ATTR_S stDisAttr = {0};
	HI_S32 s32Ret;

	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDISAttr);
	ISP_CHECK_OPEN(IspDev);
	ISP_CHECK_MEM_INIT(IspDev);    


	s32Ret = VI_GetDISAttr(0, &stDisAttr);
	pstDISAttr->bEnable = stDisAttr.bEnable;
	return s32Ret;
}

HI_S32 HI_MPI_ISP_SetMeshShadingAttr(ISP_DEV IspDev, const ISP_SHADING_ATTR_S *pstShadingAttr)
{
    HI_U16 i,j;
    HI_U32 u32Width, u32Height;
    HI_U32 u32XSum = 0;
    HI_U32 u32YSum = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    ISP_CHECK_BOOL(pstShadingAttr->bEnable);

    u32Width 	= hi_ext_sync_total_width_read();//hi_isp_yuv444_width_read()+1;
    u32Height 	= hi_ext_sync_total_height_read();//hi_isp_yuv444_height_read()+1;
    
	hi_ext_system_isp_mesh_shading_enable_write(pstShadingAttr->bEnable);

	if(pstShadingAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading type %d in %s!\n", pstShadingAttr->enOpType, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
	hi_ext_system_isp_mesh_shading_manu_mode_write(pstShadingAttr->enOpType);

    for (i = 0; i < SHADING_MESH_NUM; i++)
	{
	    if(pstShadingAttr->au32NoiseControlGain[i]>0x1fff)
	    {
	        ISP_TRACE(HI_DBG_ERR, "Invalid au32NoiseControlGain[%d]:%d in %s!\n",i, pstShadingAttr->au32NoiseControlGain[i], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
	    }
	    if(pstShadingAttr->au32BGain[i]>0x1fff)
	    {
	        ISP_TRACE(HI_DBG_ERR, "Invalid au32BGain[%d]:%d in %s!\n",i, pstShadingAttr->au32BGain[i], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
	    }
	    if(pstShadingAttr->au32RGain[i]>0x1fff)
	    {
	        ISP_TRACE(HI_DBG_ERR, "Invalid au32RGain[%d]:%d in %s!\n",i, pstShadingAttr->au32RGain[i], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
	    }
	    if(pstShadingAttr->au32GbGain[i]>0x1fff)
	    {
	        ISP_TRACE(HI_DBG_ERR, "Invalid au32GbGain[%d]:%d in %s!\n",i, pstShadingAttr->au32GbGain[i], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
	    }
	    if(pstShadingAttr->au32GrGain[i]>0x1fff)
	    {
	        ISP_TRACE(HI_DBG_ERR, "Invalid au32GrGain[%d]:%d in %s!\n",i, pstShadingAttr->au32GrGain[i], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
	    }
	    hi_ext_system_isp_mesh_shading_noise_control_write(i, pstShadingAttr->au32NoiseControlGain[i]);
		hi_ext_system_isp_mesh_shading_b_gain_write(i,pstShadingAttr->au32BGain[i]);
		hi_ext_system_isp_mesh_shading_r_gain_write(i,pstShadingAttr->au32RGain[i]);
		hi_ext_system_isp_mesh_shading_gb_gain_write(i,pstShadingAttr->au32GbGain[i]);
		hi_ext_system_isp_mesh_shading_gr_gain_write(i,pstShadingAttr->au32GrGain[i]);
	}

    for ( j = 0; j < 8; j++ )
    {
        u32XSum += pstShadingAttr->au32XGridWidth[j];
        u32YSum += pstShadingAttr->au32YGridWidth[j];
    }

    if ((u32XSum != (u32Width / 4)) || (u32YSum != (u32Height / 4)))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading block width u32XSum = %d, u32YSum = %d in %s!\n", u32XSum, u32YSum, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for ( j = 0; j < 8; j++ )
    {
        if ( (pstShadingAttr->au32XGridWidth[j] > (u32Width/4 - 28)) || (pstShadingAttr->au32XGridWidth[j] < 4) )
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading Block Width value in %s!\n", __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_isp_mesh_shading_xgrid_write(j, pstShadingAttr->au32XGridWidth[j]);
    }
    for ( j = 0; j < 8; j++)
    {
        if ( (pstShadingAttr->au32YGridWidth[j] > (u32Height/4 - 28)) || (pstShadingAttr->au32YGridWidth[j] < 4) )
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading Block Height value in %s!\n", __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_isp_mesh_shading_ygrid_write(j, pstShadingAttr->au32YGridWidth[j]);
    }
	if(pstShadingAttr->u16MeshStrength > 65535)
	{
	    ISP_TRACE(HI_DBG_ERR, "Invalid u16MeshStrength:%d in %s!\n", pstShadingAttr->u16MeshStrength, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_isp_mesh_shading_meshstrength_write(pstShadingAttr->u16MeshStrength);
	hi_ext_system_isp_mesh_shading_updata_write(HI_TRUE);
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetMeshShadingAttr(ISP_DEV IspDev, ISP_SHADING_ATTR_S *pstShadingAttr)
{
	HI_U16 i, j;
    ISP_CHECK_DEV(IspDev);    
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    pstShadingAttr->bEnable = hi_ext_system_isp_mesh_shading_enable_read();//hi_isp_lsc_cfg_enable_read(IspDev);
	pstShadingAttr->enOpType = hi_ext_system_isp_mesh_shading_manu_mode_read();
	
    for (i = 0; i < SHADING_MESH_NUM; i++)
	{
	    pstShadingAttr->au32NoiseControlGain[i] = hi_ext_system_isp_mesh_shading_noise_control_read(i);
		pstShadingAttr->au32BGain[i] = hi_ext_system_isp_mesh_shading_b_gain_read(i);
		pstShadingAttr->au32RGain[i] = hi_ext_system_isp_mesh_shading_r_gain_read(i);
		pstShadingAttr->au32GbGain[i]= hi_ext_system_isp_mesh_shading_gb_gain_read(i);
		pstShadingAttr->au32GrGain[i]= hi_ext_system_isp_mesh_shading_gr_gain_read(i);
	}
    for ( j = 0 ; j < 8 ; j++ )
    {
        pstShadingAttr->au32XGridWidth[j] = hi_ext_system_isp_mesh_shading_xgrid_read(j);
        pstShadingAttr->au32YGridWidth[j] = hi_ext_system_isp_mesh_shading_ygrid_read(j);
    }
    pstShadingAttr->u16MeshStrength = hi_ext_system_isp_mesh_shading_meshstrength_read();
    return HI_SUCCESS;
}
HI_S32 HI_MPI_ISP_MeshShadingCalibration(HI_U16* pu16SrcRaw, 
    ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg , ISP_MESH_SHADING_TABLE_S* pstMeshShadingResult)
{
    int i,j;
    ISP_CHECK_POINTER(pu16SrcRaw);
    ISP_CHECK_POINTER(pstLSCCaliCfg);
    ISP_CHECK_POINTER(pstMeshShadingResult);
    if (pstLSCCaliCfg->u16ImageWidth % 4 != 0)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u16ImageWidth:%d!\n", pstLSCCaliCfg->u16ImageWidth);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstLSCCaliCfg->u16ImageHeight % 4 != 0)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u16ImageHeight:%d!\n", pstLSCCaliCfg->u16ImageHeight);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstLSCCaliCfg->enRawBit >= BAYER_RAW_BUTT)||(pstLSCCaliCfg->enRawBit < BAYER_RAW_8BIT)||(pstLSCCaliCfg->enRawBit == 9)
        ||(pstLSCCaliCfg->enRawBit == 11)||(pstLSCCaliCfg->enRawBit == 13)||(pstLSCCaliCfg->enRawBit == 15))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid enRawBit type %d in %s!\n", pstLSCCaliCfg->enRawBit, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < pstLSCCaliCfg->u16ImageHeight; i++)
    {
        for (j = 0; j < pstLSCCaliCfg->u16ImageWidth; j++)
        {
            if (pu16SrcRaw[i * pstLSCCaliCfg->u16ImageWidth + j] >= (1 << pstLSCCaliCfg->enRawBit))
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid SrcRaw[Line:%d, Col:%d]: 0x%x (enRawBit:%d)!\n", \
                    i, j, pu16SrcRaw[i * pstLSCCaliCfg->u16ImageWidth + j], pstLSCCaliCfg->enRawBit);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
    }
    if(pstLSCCaliCfg->enGSModeX >= LSC_GS_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid enGSModeX type %d in %s!\n", pstLSCCaliCfg->enGSModeX, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstLSCCaliCfg->enGSModeY >= LSC_GS_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid enGSModeY type %d in %s!\n", pstLSCCaliCfg->enGSModeY, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstLSCCaliCfg->enBayer>= BAYER_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid enBayer type %d in %s!\n", pstLSCCaliCfg->enBayer, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstLSCCaliCfg->u16BLCOffsetB > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u16BLCOffsetB %d in %s!\n", pstLSCCaliCfg->u16BLCOffsetB, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstLSCCaliCfg->u16BLCOffsetGb > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u16BLCOffsetGb %d in %s!\n", pstLSCCaliCfg->u16BLCOffsetGb, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstLSCCaliCfg->u16BLCOffsetGr > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u16BLCOffsetGr %d in %s!\n", pstLSCCaliCfg->u16BLCOffsetGr, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstLSCCaliCfg->u16BLCOffsetR > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid u16BLCOffsetB %d in %s!\n", pstLSCCaliCfg->u16BLCOffsetR, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
   
    if (LSC_GS_GEOMETRIC == pstLSCCaliCfg->enGSModeX)
	{
		if((pstLSCCaliCfg->fCommRatioX<1.0)||(pstLSCCaliCfg->fCommRatioX>2.0))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid fCommRatioX %f in %s!\n", pstLSCCaliCfg->fCommRatioX, __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;        
        }
	}
	else // LSC_GS_MANUAL X
	{
		for(i = 0;i<8;i++)
        {
            if((pstLSCCaliCfg->au8GridStepX[i]<1)||(pstLSCCaliCfg->au8GridStepX[i]>50))
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid au8GridStepX[%d] %d in %s!\n", i,pstLSCCaliCfg->au8GridStepX[i], __FUNCTION__);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        for(i = 0;i<7;i++)
        {
            if(pstLSCCaliCfg->au8GridStepX[i]>pstLSCCaliCfg->au8GridStepX[i+1]) 
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid au8GridStepX[%d] and au8GridStepX[%d] in %s!\n",i,(i+1), __FUNCTION__);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        if(pstLSCCaliCfg->au8GridStepX[7]!=50)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid au8GridStepX[7] %d in %s!\n",pstLSCCaliCfg->au8GridStepX[7], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
	}
	if (LSC_GS_GEOMETRIC == pstLSCCaliCfg->enGSModeY)
	{
		if((pstLSCCaliCfg->fCommRatioY<1.0)||(pstLSCCaliCfg->fCommRatioY>2.0))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid fCommRatioY %f in %s!\n", pstLSCCaliCfg->fCommRatioY, __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;        
        }
	}
	else // LSC_GS_MANUAL Y
	{
        for(i = 0;i<8;i++)
        {
            if((pstLSCCaliCfg->au8GridStepY[i]<1)||(pstLSCCaliCfg->au8GridStepY[i]>50))
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid au8GridStepY[%d] %d in %s!\n",i, pstLSCCaliCfg->au8GridStepY[i], __FUNCTION__);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        for(i = 0;i<7;i++)
        {
            if(pstLSCCaliCfg->au8GridStepY[i]>pstLSCCaliCfg->au8GridStepY[i+1]) 
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid au8GridStepY[%d] and au8GridStepY[%d] in %s!\n",i, (i+1), __FUNCTION__);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        if(pstLSCCaliCfg->au8GridStepY[7]!=50)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid au8GridStepY[7] %d in %s!\n",pstLSCCaliCfg->au8GridStepY[7], __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
	}
    return ISP_MeshShadingCalibration(pu16SrcRaw,pstLSCCaliCfg,pstMeshShadingResult);
}

HI_S32 HI_MPI_ISP_SetGammaAttr(ISP_DEV IspDev, const ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;
    HI_U32 u32WDRMode;
    const HI_U16 *pu16GammaLut; 
    static HI_U16 au16GammaDef[GAMMA_NODE_NUM] = {
         0  ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,    
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
    static HI_U16 au16GammasRGB[GAMMA_NODE_NUM] = {
        0,    203,  347,  452,  539,  613,  679,  739,  794,  846,  894,  939,  982,  1023,  1062,  1100,  
        1136, 1171, 1204, 1237, 1268, 1299, 1329, 1358, 1386, 1414, 1441, 1467, 1493, 1519,  1543,  1568, 
        1592, 1615, 1638, 1661, 1683, 1705, 1726, 1748, 1769, 1789, 1810, 1830, 1849, 1869,  1888,  1907, 
        1926, 1945, 1963, 1981, 1999, 2017, 2034, 2052, 2069, 2086, 2102, 2119, 2136, 2152,  2168,  2184,  
        2200, 2216, 2231, 2247, 2262, 2277, 2292, 2307, 2322, 2337, 2351, 2366, 2380, 2394,  2408,  2422,  
        2436, 2450, 2464, 2477, 2491, 2504, 2518, 2531, 2544, 2557, 2570, 2583, 2596, 2609,  2621,  2634,  
        2646, 2659, 2671, 2683, 2696, 2708, 2720, 2732, 2744, 2756, 2767, 2779, 2791, 2802,  2814,  2825,  
        2837, 2848, 2859, 2871, 2882, 2893, 2904, 2915, 2926, 2937, 2948, 2959, 2969, 2980,  2991,  3001, 
        3012, 3023, 3033, 3043, 3054, 3064, 3074, 3085, 3095, 3105, 3115, 3125, 3135, 3145,  3155,  3165, 
        3175, 3185, 3194, 3204, 3214, 3224, 3233, 3243, 3252, 3262, 3271, 3281, 3290, 3300,  3309,  3318, 
        3327, 3337, 3346, 3355, 3364, 3373, 3382, 3391, 3400, 3409, 3418, 3427, 3436, 3445,  3454,  3463, 
        3471, 3480, 3489, 3498, 3506, 3515, 3523, 3532, 3540, 3549, 3557, 3566, 3574, 3583,  3591,  3600, 
        3608, 3616, 3624, 3633, 3641, 3649, 3657, 3665, 3674, 3682, 3690, 3698, 3706, 3714,  3722,  3730,  
        3738, 3746, 3754, 3762, 3769, 3777, 3785, 3793, 3801, 3808, 3816, 3824, 3832, 3839,  3847,  3855,  
        3862, 3870, 3877, 3885, 3892, 3900, 3907, 3915, 3922, 3930, 3937, 3945, 3952, 3959,  3967,  3974, 
        3981, 3989, 3996, 4003, 4010, 4018, 4025, 4032, 4039, 4046, 4054, 4061, 4068, 4075,  4082,  4089, 4095
    };   

    static HI_U16 au16GammaDefWDR[GAMMA_NODE_NUM]={
        0,1,2,4,8,12,17,23,30,38,47,57,68,79,92,105,120,133,147,161,176,192,209,226,243,260,278,296,317,340,365,390,416,440,466,491,517,538,561,584,607,631,656,680,705,730,756,784,812,835,858,882,908,934,958,982,1008,1036,1064,1092,1119,1143,1167,1192,1218,1243,1269,1296,1323,1351,1379,1408,1434,1457,1481,1507,1531,1554,1579,1603,1628,1656,1683,1708,1732,1756,1780,1804,1829,1854,1877,1901,1926,1952,1979,2003,2024,2042,2062,2084,2106,2128,2147,2168,2191,2214,2233,2256,2278,2296,2314,2335,2352,2373,2391,2412,2431,2451,2472,2492,2513,2531,2547,2566,2581,2601,2616,2632,2652,2668,2688,2705,2721,2742,2759,2779,2796,2812,2826,2842,2857,2872,2888,2903,2920,2934,2951,2967,2983,3000,3015,3033,3048,3065,3080,3091,3105,3118,3130,3145,3156,3171,3184,3197,3213,3224,3240,3252,3267,3281,3295,3310,3323,3335,3347,3361,3372,3383,3397,3409,3421,3432,3447,3459,3470,3482,3497,3509,3521,3534,3548,3560,3572,3580,3592,3602,3613,3625,3633,3646,3657,3667,3679,3688,3701,3709,3719,3727,3736,3745,3754,3764,3773,3781,3791,3798,3806,3816,3823,3833,3840,3847,3858,3865,3872,3879,3888,3897,3904,3911,3919,3926,3933,3940,3948,3955,3962,3970,3973,3981,3988,3996,4003,4011,4018,4026,4032,4037,4045,4053,4057,4064,4072,4076,4084,4088,4095
        };
    static HI_U16 au16GammasRGBWDR[GAMMA_NODE_NUM]={
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
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_BOOL(pstGammaAttr->bEnable);

    u32WDRMode = hi_ext_top_sensor_wdr_mode_read();

    switch (pstGammaAttr->enCurveType)
    {
        case ISP_GAMMA_CURVE_DEFAULT:
            if (0 == u32WDRMode)
            {
                pu16GammaLut = au16GammaDef;
            }
            else
            {
                pu16GammaLut = au16GammaDefWDR;
            }
            break;
        case ISP_GAMMA_CURVE_SRGB:
            if (0 == u32WDRMode)
            {
                pu16GammaLut = au16GammasRGB;
            }
            else
            {
                pu16GammaLut = au16GammasRGBWDR;
            }
            break;
        case ISP_GAMMA_CURVE_USER_DEFINE:
            for(i=0; i<GAMMA_NODE_NUM; i++)
            {
		        if(pstGammaAttr->u16Table[i]>4095)
		        {
		            ISP_TRACE(HI_DBG_ERR, "Invalid Gamma Value[%d]: %d in %s!\n", i,pstGammaAttr->u16Table[i], __FUNCTION__);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
		        }
            }
            pu16GammaLut = pstGammaAttr->u16Table;
            break;
        default:
            printf("Invalid ISP Gamma Curve Type %d in %s!\n", pstGammaAttr->enCurveType, __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
            break;
    }
    
    hi_isp_gamma_enable_write(IspDev, pstGammaAttr->bEnable);

    hi_isp_gamma_waddr_write(IspDev, 0);
    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
		hi_isp_gamma_wdata_write(IspDev, (HI_U32)(pu16GammaLut[i]));
    }    

	hi_isp_gamma_lut_update_write(IspDev, 1);
	
    hi_ext_system_gamma_curve_type_write(pstGammaAttr->enCurveType);
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetGammaAttr(ISP_DEV IspDev, ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstGammaAttr->bEnable = hi_isp_gamma_enable_read(IspDev);

    pstGammaAttr->enCurveType = hi_ext_system_gamma_curve_type_read();

	hi_isp_gamma_raddr_write(IspDev, 0);
	
    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
        //pstGammaAttr->u16Table[i] = hi_gamma_rgb_mem_array_data_read(i);
		pstGammaAttr->u16Table[i] = (hi_isp_gamma_rdata_read(IspDev));
    }    

    return HI_SUCCESS;   
}



HI_S32 HI_MPI_ISP_SetSharpenAttr(ISP_DEV IspDev, const ISP_SHARPEN_ATTR_S *pstShpAttr)
{
    HI_U8 i,j;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstShpAttr);
    ISP_CHECK_BOOL(pstShpAttr->bEnable);
   
    if(pstShpAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen Type %d!\n", pstShpAttr->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    //hi_ext_system_yuv_sharpening_enable_write(pstShpAttr->bEnable);
    hi_isp_sharpen_cfg_enable_write(IspDev, pstShpAttr->bEnable);
    hi_ext_system_sharpen_manu_mode_write(pstShpAttr->enOpType);    

	//check 
	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		if(pstShpAttr->stAuto.au16SharpenUd[i] > 1023 )
		{
			printf("Invalid  SharpenUd Input! Value range of SharpenUd is [0, 1023]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
		if(pstShpAttr->stAuto.au8SharpenD[i] > 255 )
		{
			printf("Invalid  SharpenD Input! Value range of SharpenD is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8TextureThr[i] > 255 )
		{
			printf("Invalid  TextureThr Input! Value range of TextureThr is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8OverShoot[i] > 255 )
		{
			printf("Invalid  OverShoot Input! Value range of OverShoot is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8UnderShoot[i] > 255 )
		{
			printf("Invalid  UnderShoot Input! Value range of UnderShoot is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8SharpenEdge[i] > 255 )
		{
			printf("Invalid  SharpenEdge Input! Value range of SharpenEdge is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8EdgeThr[i] > 255 )
		{
			printf("Invalid  EdgeThr Input! Value range of EdgeThr is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8DetailCtrl[i] > 255 )
		{
			printf("Invalid  DetailCtrl Input! Value range of DetailCtrl is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8shootSupStr[i] > 255 )
		{
			printf("Invalid  shootSupStr Input! Value range of shootSupStr is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstShpAttr->stAuto.au8RGain[i] > 31 )
		{
			printf("Invalid  RGain Input! Value range of RGain is [0, 31]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstShpAttr->stAuto.au8BGain[i] > 31 )
		{
			printf("Invalid  BGain Input! Value range of BGain is [0, 31]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstShpAttr->stAuto.au8SkinGain[i] > 255 )
		{
			printf("Invalid  SkinGain Input! Value range of SkinGain is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstShpAttr->stAuto.au8EdgeFiltStr[i] > 127 )
		{
			printf("Invalid  DetailCtrl Input! Value range of EdgeFiltStr is [0, 127]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if(pstShpAttr->stAuto.au8JagCtrl[i] > 255 )
		{
			printf("Invalid  shootSupStr Input! Value range of JagCtrl is [0, 255]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstShpAttr->stAuto.au8NoiseLumaCtrl[i] > 31 )
		{
			printf("Invalid  RGain Input! Value range of NoiseLumaCtrl is [0, 31]\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
	}

	if(pstShpAttr->stManual.u16SharpenUd > 1023)
	{
		printf("Invalid  SharpenUd Input! Value range of SharpenUd is [0, 1023]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8SharpenD > 255 )
	{
		printf("Invalid  SharpenD Input! Value range of SharpenD is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8TextureThr > 255 )
	{
		printf("Invalid  TextureThr Input! Value range of TextureThr is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8OverShoot > 255 )
	{
		printf("Invalid  OverShoot Input! Value range of OverShoot is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8UnderShoot > 255 )
	{
		printf("Invalid  UnderShoot Input! Value range of UnderShoot is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8SharpenEdge > 255 )
	{
		printf("Invalid  SharpenEdge Input! Value range of SharpenEdge is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8EdgeThr > 255 )
	{
		printf("Invalid  EdgeThr Input! Value range of EdgeThr is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8DetailCtrl > 255 )
	{
		printf("Invalid  DetailCtrl Input! Value range of DetailCtrl is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstShpAttr->stManual.u8shootSupStr > 255 )
	{
		printf("Invalid  shootSupStr Input! Value range of shootSupStr is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstShpAttr->stManual.u8RGain > 31 )
	{
		printf("Invalid  RGain Input! Value range of RGain is [0, 31]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstShpAttr->stManual.u8BGain > 31 )
	{
		printf("Invalid  BGain Input! Value range of BGain is [0, 31]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstShpAttr->stManual.u8SkinGain > 255 )
	{
		printf("Invalid  SkinGain Input! Value range ofSkinGain is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstShpAttr->stManual.u8EdgeFiltStr> 127 )
	{
		printf("Invalid  RGain Input! Value range of EdgeFiltStr is [0, 127]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstShpAttr->stManual.u8JagCtrl> 255 )
	{
		printf("Invalid  BGain Input! Value range of JagCtrl is [0, 255]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstShpAttr->stManual.u8NoiseLumaCtrl> 31 )
	{
		printf("Invalid  SkinGain Input! Value range of NoiseLumaCtrl is [0, 31]\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	//check

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{

		hi_ext_system_sharpen_SharpenUd_write(i, pstShpAttr->stAuto.au16SharpenUd[i]);
		hi_ext_system_sharpen_SharpenD_write(i, pstShpAttr->stAuto.au8SharpenD[i]);
		hi_ext_system_sharpen_TextureThd_write(i, pstShpAttr->stAuto.au8TextureThr[i]);
	    hi_ext_system_sharpen_overshootAmt_write(i, pstShpAttr->stAuto.au8OverShoot[i]);
	    hi_ext_system_sharpen_undershootAmt_write(i, pstShpAttr->stAuto.au8UnderShoot[i]);
		hi_ext_system_sharpen_SharpenEdge_write(i, pstShpAttr->stAuto.au8SharpenEdge[i]);
		hi_ext_system_sharpen_EdgeThd_write(i, pstShpAttr->stAuto.au8EdgeThr[i]);
		hi_ext_system_sharpen_DetailCtrl_write(i, pstShpAttr->stAuto.au8DetailCtrl[i]);
		hi_ext_system_sharpen_shootSupSt_write(i, pstShpAttr->stAuto.au8shootSupStr[i]);      
		hi_ext_system_sharpen_RGain_write(i, pstShpAttr->stAuto.au8RGain[i]);  
		hi_ext_system_sharpen_BGain_write(i, pstShpAttr->stAuto.au8BGain[i]); 
		hi_ext_system_sharpen_SkinGain_write(i, pstShpAttr->stAuto.au8SkinGain[i]); 
		hi_ext_system_sharpen_EdgeFiltStr_write(i, pstShpAttr->stAuto.au8EdgeFiltStr[i]);
		hi_ext_system_sharpen_JagCtrl_write(i, pstShpAttr->stAuto.au8JagCtrl[i]);
		hi_ext_system_sharpen_NoiseLumaCtrl_write(i, pstShpAttr->stAuto.au8NoiseLumaCtrl[i]);
		
	}
		
	hi_ext_system_manual_sharpen_SharpenUd_write(pstShpAttr->stManual.u16SharpenUd);
	hi_ext_system_manual_sharpen_SharpenD_write(pstShpAttr->stManual.u8SharpenD);
	hi_ext_system_manual_sharpen_TextureThd_write(pstShpAttr->stManual.u8TextureThr);
	hi_ext_system_manual_sharpen_overshootAmt_write(pstShpAttr->stManual.u8OverShoot);
	hi_ext_system_manual_sharpen_undershootAmt_write(pstShpAttr->stManual.u8UnderShoot);
	hi_ext_system_manual_sharpen_SharpenEdge_write(pstShpAttr->stManual.u8SharpenEdge);
	hi_ext_system_manual_sharpen_EdgeThd_write(pstShpAttr->stManual.u8EdgeThr);
	hi_ext_system_manual_sharpen_DetailCtrl_write(pstShpAttr->stManual.u8DetailCtrl);
	hi_ext_system_manual_sharpen_shootSupSt_write(pstShpAttr->stManual.u8shootSupStr);
	hi_ext_system_manual_sharpen_RGain_write(pstShpAttr->stManual.u8RGain);
	hi_ext_system_manual_sharpen_BGain_write(pstShpAttr->stManual.u8BGain);
	hi_ext_system_manual_sharpen_SkinGain_write(pstShpAttr->stManual.u8SkinGain);
	hi_ext_system_manual_sharpen_EdgeFiltStr_write(pstShpAttr->stManual.u8EdgeFiltStr);
	hi_ext_system_manual_sharpen_JagCtrl_write(pstShpAttr->stManual.u8JagCtrl);
	hi_ext_system_manual_sharpen_NoiseLumaCtrl_write(pstShpAttr->stManual.u8NoiseLumaCtrl);
	
	for (j = 0; j < ISP_YUV_SHPLUMA_NUM; j++)
	{
		hi_ext_system_manual_sharpen_LumaWgt_write(j, pstShpAttr->u8LumaWgt[j]);
	}
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetSharpenAttr(ISP_DEV IspDev, ISP_SHARPEN_ATTR_S *pstShpAttr)
{
    HI_U8 i,j;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstShpAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstShpAttr->bEnable = hi_isp_sharpen_cfg_enable_read(IspDev);
    pstShpAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_sharpen_manu_mode_read();

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
		pstShpAttr->stAuto.au8SharpenD[i] = hi_ext_system_sharpen_SharpenD_read(i);
		pstShpAttr->stAuto.au16SharpenUd[i] = hi_ext_system_sharpen_SharpenUd_read(i);
		pstShpAttr->stAuto.au8TextureThr[i] = hi_ext_system_sharpen_TextureThd_read(i);
        pstShpAttr->stAuto.au8OverShoot[i] = hi_ext_system_sharpen_overshootAmt_read(i); 
        pstShpAttr->stAuto.au8UnderShoot[i] = hi_ext_system_sharpen_undershootAmt_read(i);  
        pstShpAttr->stAuto.au8SharpenEdge[i] = hi_ext_system_sharpen_SharpenEdge_read(i);
		pstShpAttr->stAuto.au8EdgeThr[i] = hi_ext_system_sharpen_EdgeThd_read(i);
		pstShpAttr->stAuto.au8DetailCtrl[i] = hi_ext_system_sharpen_DetailCtrl_read(i);
		pstShpAttr->stAuto.au8shootSupStr[i] = hi_ext_system_sharpen_shootSupSt_read(i);    
		pstShpAttr->stAuto.au8RGain[i] = hi_ext_system_sharpen_RGain_read(i);
		pstShpAttr->stAuto.au8BGain[i] = hi_ext_system_sharpen_BGain_read(i);
		pstShpAttr->stAuto.au8SkinGain[i] = hi_ext_system_sharpen_SkinGain_read(i);
		pstShpAttr->stAuto.au8EdgeFiltStr[i] = hi_ext_system_sharpen_EdgeFiltStr_read(i);
		pstShpAttr->stAuto.au8JagCtrl[i] = hi_ext_system_sharpen_JagCtrl_read(i);
		pstShpAttr->stAuto.au8NoiseLumaCtrl[i] = hi_ext_system_sharpen_NoiseLumaCtrl_read(i);
    }

	pstShpAttr->stManual.u16SharpenUd = hi_ext_system_manual_sharpen_SharpenUd_read();
	pstShpAttr->stManual.u8SharpenD = hi_ext_system_manual_sharpen_SharpenD_read();
	pstShpAttr->stManual.u8TextureThr = hi_ext_system_manual_sharpen_TextureThd_read();
	pstShpAttr->stManual.u8OverShoot = hi_ext_system_manual_sharpen_overshootAmt_read();
    pstShpAttr->stManual.u8UnderShoot = hi_ext_system_manual_sharpen_undershootAmt_read();
	pstShpAttr->stManual.u8SharpenEdge = hi_ext_system_manual_sharpen_SharpenEdge_read();
	pstShpAttr->stManual.u8EdgeThr = hi_ext_system_manual_sharpen_EdgeThd_read();
	pstShpAttr->stManual.u8DetailCtrl = hi_ext_system_manual_sharpen_DetailCtrl_read();
	pstShpAttr->stManual.u8shootSupStr = hi_ext_system_manual_sharpen_shootSupSt_read(); 
	pstShpAttr->stManual.u8RGain = hi_ext_system_manual_sharpen_RGain_read(); 
	pstShpAttr->stManual.u8BGain = hi_ext_system_manual_sharpen_BGain_read();
	pstShpAttr->stManual.u8SkinGain =  hi_ext_system_manual_sharpen_SkinGain_read();
	pstShpAttr->stManual.u8EdgeFiltStr= hi_ext_system_manual_sharpen_EdgeFiltStr_read();
	pstShpAttr->stManual.u8JagCtrl= hi_ext_system_manual_sharpen_JagCtrl_read();
	pstShpAttr->stManual.u8NoiseLumaCtrl= hi_ext_system_manual_sharpen_NoiseLumaCtrl_read();
    for (j = 0; j < ISP_YUV_SHPLUMA_NUM; j++)
    {
        pstShpAttr->u8LumaWgt[j] = (HI_U8)hi_ext_system_manual_sharpen_LumaWgt_read(j);
    } 
   
    return HI_SUCCESS;
}

/* Crosstalk Removal Strength */
HI_S32 HI_MPI_ISP_SetCrosstalkAttr(ISP_DEV IspDev, const ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    ISP_CHECK_BOOL(pstCRAttr->bEnable);

	if(pstCRAttr->u8Slope > 0xE)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk slope %d in %s!\n", pstCRAttr->u8Slope, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstCRAttr->u8SensiSlope > 0xE)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk Sensitivity %d in %s!\n", pstCRAttr->u8SensiSlope, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstCRAttr->u16SensiThr > 0x3FFF)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk SensiThreshold %d in %s!\n", pstCRAttr->u16SensiThr, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
    
    hi_ext_system_ge_enable_write(pstCRAttr->bEnable);

    hi_ext_system_ge_slope_write(pstCRAttr->u8Slope);
    hi_ext_system_ge_sensitivity_write   (pstCRAttr->u8SensiSlope);
    hi_ext_system_ge_sensithreshold_write(pstCRAttr->u16SensiThr);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
    	if(pstCRAttr->au16Strength[i] > 0x100)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk Strength %d in %s!\n", pstCRAttr->au16Strength[i], __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
        hi_ext_system_ge_strength_write(i, pstCRAttr->au16Strength[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
    	if(pstCRAttr->au16NpOffset[i] > 0x3FFF)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk NpOffset %d in %s!\n", pstCRAttr->au16NpOffset[i], __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
        hi_ext_system_ge_npoffset_write(i, pstCRAttr->au16NpOffset[i]);
    }
	
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
    	if(pstCRAttr->au16Threshold[i] > 0x3FFF)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk Threshold %d in %s!\n", pstCRAttr->au16Threshold[i], __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
        hi_ext_system_ge_threshold_write(i, pstCRAttr->au16Threshold[i]);
    }	
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetCrosstalkAttr(ISP_DEV IspDev, ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    pstCRAttr->bEnable           = hi_ext_system_ge_enable_read();

    pstCRAttr->u8Slope          = hi_ext_system_ge_slope_read();
    pstCRAttr->u8SensiSlope    = hi_ext_system_ge_sensitivity_read();
    pstCRAttr->u16SensiThr = hi_ext_system_ge_sensithreshold_read();

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
        pstCRAttr->au16Strength[i] = hi_ext_system_ge_strength_read(i);
        pstCRAttr->au16NpOffset[i] = hi_ext_system_ge_npoffset_read(i);
        pstCRAttr->au16Threshold[i] = hi_ext_system_ge_threshold_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAntiFalseColorAttr(ISP_DEV IspDev, const ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
	ISP_CHECK_BOOL(pstAntiFC->bEnable);

    if(pstAntiFC->enOpType >= OP_TYPE_BUTT)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid AntiFalseColor Type %d in %s!\n", pstAntiFC->enOpType, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	hi_ext_system_fcr_manual_mode_write(pstAntiFC->enOpType);
	
	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		hi_ext_system_auto_fcr_strength_write(i,pstAntiFC->stAuto.au8Strength[i]);
	    hi_ext_system_auto_fcr_threshold_write(i, pstAntiFC->stAuto.au8Threshold[i]);
		
	}
	
	hi_ext_system_manual_fcr_strength_write(pstAntiFC->stManual.u8Strength);  
	hi_ext_system_manual_fcr_threshold_write(pstAntiFC->stManual.u8Threshold);

		
	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		if(pstAntiFC->stAuto.au16Offset[i] > 0xFFF)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid AFC Offset %d in %s!\n", pstAntiFC->stAuto.au16Offset[i], __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
		hi_ext_system_auto_fcr_offset_write(i,pstAntiFC->stAuto.au16Offset[i]);
	}

	  
	if(pstAntiFC->stManual.u16Offset > 0xFFF)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid AFC Offset %d in %s!\n", pstAntiFC->stManual.u16Offset, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	
	hi_ext_system_manual_fcr_offset_write(pstAntiFC->stManual.u16Offset);
	
	hi_ext_system_fcr_enable_write(pstAntiFC->bEnable);
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetAntiFalseColorAttr(ISP_DEV IspDev, ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	pstAntiFC->bEnable  = hi_ext_system_fcr_enable_read(); 
	pstAntiFC->enOpType = hi_ext_system_fcr_manual_mode_read();

   for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
   	{
	 pstAntiFC->stAuto.au8Strength[i] = hi_ext_system_auto_fcr_strength_read(i);
	 pstAntiFC->stAuto.au8Threshold[i] = hi_ext_system_auto_fcr_threshold_read(i);
	 pstAntiFC->stAuto.au16Offset[i]  =  hi_ext_system_auto_fcr_offset_read(i);
    }
   
	pstAntiFC->stManual.u8Strength = hi_ext_system_manual_fcr_strength_read();  
	pstAntiFC->stManual.u8Threshold  = hi_ext_system_manual_fcr_threshold_read();
	pstAntiFC->stManual.u16Offset =  hi_ext_system_manual_fcr_offset_read();
      
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDemosaicAttr(ISP_DEV IspDev, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
     HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    ISP_CHECK_BOOL(pstDemosaicAttr->bEnable);     
	
	hi_ext_system_demosaic_enable_write(pstDemosaicAttr->bEnable);
	hi_ext_system_demosaic_manual_mode_write(pstDemosaicAttr->enOpType);


	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{   
		if (pstDemosaicAttr->stAuto.au16AntiAliasSlope [i] > 0x3FF)
	    {
			printf("Invalid au16AntiAliasSlope!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}  
		if (pstDemosaicAttr->stAuto.au16AntiAliasThr[i] > 0x3FE)
			{
				printf("Invalid au16AntiAliasThr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			} 
			if (pstDemosaicAttr->stAuto.au16EdgeSmoothSlope[i]> 0x3FF)
			{
				printf("Invalid au16EdgeSmoothSlope !\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			} 
			if (pstDemosaicAttr->stAuto.au16EdgeSmoothThr[i]> 0x3FE)
			{
				printf("Invalid au16EdgeSmoothThr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			} 
		    if (pstDemosaicAttr->stAuto.au16NoiseSuppressStr[i]> 0x3FF )
			{
				printf("Invalid au16NoiseSuppressStr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}

			if (pstDemosaicAttr->stAuto.au16SharpenLumaStr[i]> 0x200 )
			{
				printf("Invalid au16SharpenLumaStr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}

		    if (pstDemosaicAttr->stAuto.au16NrCoarseStr[i] > 0x100 )
			{
				printf("Invalid au16NrCoarseStr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}

		    if (pstDemosaicAttr->stAuto.au8DetailEnhanceStr[i]> 0xa )
			{
				printf("Invalid au8DetailEnhanceStr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}

   		    if (pstDemosaicAttr->stAuto.au16ColorNoiseCtrlThr[i]> 0x12c )
			{
				printf("Invalid au16ColorNoiseCtrlThr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			} 
			if (pstDemosaicAttr->stAuto.au16EdgeSmoothThr[i]> pstDemosaicAttr->stAuto.au16AntiAliasThr[i])
			{
				printf("Invalid au16EdgeSmoothThr and au16AntiAliasThr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			} 

			if ((pstDemosaicAttr->stAuto.au16EdgeSmoothThr[i] + pstDemosaicAttr->stAuto.au16EdgeSmoothSlope[i])> pstDemosaicAttr->stAuto.au16AntiAliasThr[i])
			{
				printf("Invalid au16EdgeSmoothSlope for au16EdgeSmoothThr!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			} 
			hi_ext_system_demosaic_auto_edge_smooth_thresh_write(i,pstDemosaicAttr->stAuto.au16EdgeSmoothThr[i]);
			hi_ext_system_demosaic_auto_edge_smooth_slope_write(i,pstDemosaicAttr->stAuto.au16EdgeSmoothSlope[i]);
			hi_ext_system_demosaic_auto_antialias_thresh_write(i, pstDemosaicAttr->stAuto.au16AntiAliasThr[i]);
			hi_ext_system_demosaic_auto_antialias_slope_write(i, pstDemosaicAttr->stAuto.au16AntiAliasSlope[i]);
			hi_ext_system_demosaic_auto_gain_write(i, pstDemosaicAttr->stAuto.au16NrCoarseStr[i]); //au16LutNddmGain
			hi_ext_system_demosaic_auto_detail_enhance_write(i, pstDemosaicAttr->stAuto.au8DetailEnhanceStr[i]);//au8LutMultiMF
		hi_ext_system_satufixehcy_lut_write(i, pstDemosaicAttr->stAuto.au16SharpenLumaStr[i]);//as16LutSatuFixEhcY
		hi_ext_system_demosaic_auto_noise_suppress_strength_write(i, pstDemosaicAttr->stAuto.au16NoiseSuppressStr[i]);//au16LutClipDeltaIntpLow
			hi_ext_system_demosaic_auto_colornoise_ctrl_thresh_write(i, pstDemosaicAttr->stAuto.au16ColorNoiseCtrlThr[i]);//au16ColorNoiseCtrlThr
		
	}
  

	if (pstDemosaicAttr->stManual.u16AntiAliasSlope > 0x3FF)
	{
		printf("Invalid pstDemosaicAttr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}  
	if (pstDemosaicAttr->stManual.u16AntiAliasThr> 0x3FE)
	{
		printf("Invalid u16AntiAliasThr !\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	} 
	 if (pstDemosaicAttr->stManual.u16EdgeSmoothSlope > 0x3FF)
	{
			printf("Invalid u16EdgeSmoothSlope!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		} 
		if (pstDemosaicAttr->stManual.u16EdgeSmoothThr> 0x3FE)
		{
			printf("Invalid u16EdgeSmoothThr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		} 
		  if (pstDemosaicAttr->stManual.u16NoiseSuppressStr> 0X3FF )
		{
			printf("Invalid u16NoiseSuppressStr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		  if (pstDemosaicAttr->stManual.u16SharpenLumaStr > 0X200 )
		{
			printf("Invalid u16SharpenLumaStr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		  if (pstDemosaicAttr->stManual.u16NrCoarseStr > 0X100 )
		{
			printf("Invalid u16NrCoarseStr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		  if (pstDemosaicAttr->stManual.u8DetailEnhanceStr> 0Xa )
		{
			printf("Invalid u8DetailEnhanceStr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}

		if (pstDemosaicAttr->stManual.u16ColorNoiseCtrlThr > 0X12c )
		{
			printf("Invalid u16ColorNoiseCtrlThr!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}          
		if (pstDemosaicAttr->stManual.u16EdgeSmoothThr > pstDemosaicAttr->stManual.u16AntiAliasThr)
		{
			printf("Invalid u16EdgeSmoothThr and u16AntiAliasThr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	} 

	if ((pstDemosaicAttr->stManual.u16EdgeSmoothThr + pstDemosaicAttr->stManual.u16EdgeSmoothSlope) > pstDemosaicAttr->stManual.u16AntiAliasThr)
	{
		printf("Invalid u16EdgeSmoothSlope for u16EdgeSmoothThr!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	hi_ext_system_demosaic_manual_edge_smooth_thresh_write(pstDemosaicAttr->stManual.u16EdgeSmoothThr);
	hi_ext_system_demosaic_manual_edge_smooth_slope_write(pstDemosaicAttr->stManual.u16EdgeSmoothSlope);
	hi_ext_system_demosaic_manual_antialias_thresh_write(pstDemosaicAttr->stManual.u16AntiAliasThr);
	hi_ext_system_demosaic_manual_antialias_slope_write(pstDemosaicAttr->stManual.u16AntiAliasSlope);
	hi_ext_system_demosaic_manual_gain_write(pstDemosaicAttr->stManual.u16NrCoarseStr);
	hi_ext_system_demosaic_manual_detail_enhance_write(pstDemosaicAttr->stManual.u8DetailEnhanceStr);		
	hi_ext_system_demosaic_manual_satufixehcy_write(pstDemosaicAttr->stManual.u16SharpenLumaStr);
	hi_ext_system_demosaic_manual_noise_suppress_strength_write(pstDemosaicAttr->stManual.u16NoiseSuppressStr);        			
		hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_write(pstDemosaicAttr->stManual.u16ColorNoiseCtrlThr);        			

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDemosaicAttr(ISP_DEV IspDev, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
	pstDemosaicAttr->bEnable = hi_ext_system_demosaic_enable_read();
	pstDemosaicAttr->enOpType = hi_ext_system_demosaic_manual_mode_read();

		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{	
			pstDemosaicAttr->stAuto.au16EdgeSmoothThr[i] = hi_ext_system_demosaic_auto_edge_smooth_thresh_read(i);
			pstDemosaicAttr->stAuto.au16EdgeSmoothSlope[i] = hi_ext_system_demosaic_auto_edge_smooth_slope_read(i);
			pstDemosaicAttr->stAuto.au16AntiAliasThr[i] = hi_ext_system_demosaic_auto_antialias_thresh_read(i);
			pstDemosaicAttr->stAuto.au16AntiAliasSlope[i] = hi_ext_system_demosaic_auto_antialias_slope_read(i);					  
			pstDemosaicAttr->stAuto.au16NrCoarseStr[i]     = hi_ext_system_demosaic_auto_gain_read(i); //au16LutNddmGain
			pstDemosaicAttr->stAuto.au8DetailEnhanceStr[i]  = hi_ext_system_demosaic_auto_detail_enhance_read(i);//au8LutMultiMF
			pstDemosaicAttr->stAuto.au16NoiseSuppressStr[i]	= hi_ext_system_demosaic_auto_noise_suppress_strength_read(i);//au16LutClipDeltaIntpLow
			pstDemosaicAttr->stAuto.au16SharpenLumaStr[i]  = hi_ext_system_demosaic_auto_satufixehcy_lut_read(i);//as16LutSatuFixEhcY
			pstDemosaicAttr->stAuto.au16ColorNoiseCtrlThr[i]  = hi_ext_system_demosaic_auto_colornoise_ctrl_thresh_read(i);//au16ColorNoiseCtrlThr
	    }
		pstDemosaicAttr->stManual.u16EdgeSmoothThr = hi_ext_system_demosaic_manual_edge_smooth_thresh_read();
		pstDemosaicAttr->stManual.u16EdgeSmoothSlope = hi_ext_system_demosaic_manual_edge_smooth_slope_read();
		pstDemosaicAttr->stManual.u16AntiAliasThr = hi_ext_system_demosaic_manual_antialias_thresh_read();
		pstDemosaicAttr->stManual.u16AntiAliasSlope = hi_ext_system_demosaic_manual_antialias_slope_read();
		pstDemosaicAttr->stManual.u16NrCoarseStr = hi_ext_system_demosaic_manual_gain_read();
		pstDemosaicAttr->stManual.u8DetailEnhanceStr  = hi_ext_system_demosaic_manual_detail_enhance_read();		
		pstDemosaicAttr->stManual.u16NoiseSuppressStr  = hi_ext_system_demosaic_manual_noise_suppress_strength_read();     
		pstDemosaicAttr->stManual.u16SharpenLumaStr  = hi_ext_system_demosaic_manual_satufixehcy_read();
		pstDemosaicAttr->stManual.u16ColorNoiseCtrlThr= hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_read();
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetNRAttr(ISP_DEV IspDev, const ISP_NR_ATTR_S *pstNRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
	ISP_CHECK_BOOL(pstNRAttr->bEnable);
	HI_U8 i;

	hi_ext_system_bayernr_enable_write(pstNRAttr->bEnable);

	if(pstNRAttr->enOpType >= OP_TYPE_BUTT)
	{
		printf("Invalid  enOpType Input!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	for (i = 0; i < BAYER_PATTERN_BUTT; i++)
	{
		if(pstNRAttr->au16CoarseStr[i] >1000)
		{
			printf("Invalid  au16CoarseStr Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		hi_ext_system_bayernr_coarse_strength_write(i,pstNRAttr->au16CoarseStr[i]);
	}

	for(i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++)
	{
		if(pstNRAttr->au16CoringRatio[i] > 1000)
		{
			printf("Invalid  au16CoringRatio Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		hi_ext_system_bayernr_coring_low_ratio_write(i, pstNRAttr->au16CoringRatio[i]);
	}

	hi_ext_system_bayernr_manual_mode_write(pstNRAttr->enOpType);
	
	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{	
		if(pstNRAttr->stAuto.au8ChromaStr[0][i] > 3)
		{
				printf("Invalid  au8ChromaStr[0][%d] Input!\n",i);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if(pstNRAttr->stAuto.au8ChromaStr[1][i] > 3)
			{
				printf("Invalid  au8ChromaStr[1][%d] Input!\n",i);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if(pstNRAttr->stAuto.au8ChromaStr[2][i] > 3)
			{
				printf("Invalid  au8ChromaStr[2][%d] Input!\n",i);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if(pstNRAttr->stAuto.au8ChromaStr[3][i] > 3)
			{
				printf("Invalid  au8ChromaStr[3][%d] Input!\n",i);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if(pstNRAttr->stAuto.au8FineStr[i] > 128)
			{
				printf("Invalid  au8FineStr[%d] Input!\n",i);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstNRAttr->stAuto.au16CoringWeight[i] > 3200)
		{
			printf("Invalid  au16CoringWeight[%d] Input!\n", i);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
	    hi_ext_system_bayernr_auto_chroma_strength_r_write(i, pstNRAttr->stAuto.au8ChromaStr[0][i]);
		hi_ext_system_bayernr_auto_chroma_strength_gr_write(i, pstNRAttr->stAuto.au8ChromaStr[1][i]);
	    hi_ext_system_bayernr_auto_chroma_strength_gb_write(i, pstNRAttr->stAuto.au8ChromaStr[2][i]);
		hi_ext_system_bayernr_auto_chroma_strength_b_write(i, pstNRAttr->stAuto.au8ChromaStr[3][i]);
		hi_ext_system_bayernr_auto_fine_strength_write(i, pstNRAttr->stAuto.au8FineStr[i]);
		hi_ext_system_bayernr_auto_coring_weight_write(i, pstNRAttr->stAuto.au16CoringWeight[i]);	
	}
	
	if(pstNRAttr->stManual.u8ChromaStr[0] > 3)
	{
		printf("Invalid  u8ChromaStr[0] Input!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstNRAttr->stManual.u8ChromaStr[1] > 3)
		{
			printf("Invalid  u8ChromaStr[1] Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstNRAttr->stManual.u8ChromaStr[2] > 3)
		{
			printf("Invalid  u8ChromaStr[2] Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstNRAttr->stManual.u8ChromaStr[3] > 3)
		{
			printf("Invalid  u8ChromaStr[3] Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstNRAttr->stManual.u8FineStr > 128)
		{
			printf("Invalid  u8FineStr Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if(pstNRAttr->stManual.u16CoringWeight > 3200)
		{
			printf("Invalid  u16CoringWeight Input!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	hi_ext_system_bayernr_manual_chroma_strength_write(0,pstNRAttr->stManual.u8ChromaStr[0]);
	hi_ext_system_bayernr_manual_chroma_strength_write(1,pstNRAttr->stManual.u8ChromaStr[1]);
	hi_ext_system_bayernr_manual_chroma_strength_write(2,pstNRAttr->stManual.u8ChromaStr[2]);
	hi_ext_system_bayernr_manual_chroma_strength_write(3,pstNRAttr->stManual.u8ChromaStr[3]);
	hi_ext_system_bayernr_manual_fine_strength_write(pstNRAttr->stManual.u8FineStr);
	hi_ext_system_bayernr_manual_coring_weight_write(pstNRAttr->stManual.u16CoringWeight);


	for(i=0;i< WDR_MAX_FRAME; i++)
	{
		if(pstNRAttr->stWDR.au8WDRCoarseStr[i] > 80)
		{
			printf("Invalid  enOpType Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
	  hi_ext_system_bayernr_wdr_strength_write(i, pstNRAttr->stWDR.au8WDRCoarseStr[i]);
	}
		
	return HI_SUCCESS;
}
HI_S32 HI_MPI_ISP_GetNRAttr(ISP_DEV IspDev,  ISP_NR_ATTR_S *pstNRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
	HI_U8 i;

	pstNRAttr->bEnable = hi_ext_system_bayernr_enable_read();
	pstNRAttr->enOpType = hi_ext_system_bayernr_manual_mode_read();
	for (i = 0; i < BAYER_PATTERN_BUTT; i++)
	{
		pstNRAttr->au16CoarseStr[i] = hi_ext_system_bayernr_coarse_strength_read(i);
	}

	for(i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++)
	{
		pstNRAttr->au16CoringRatio[i] = hi_ext_system_bayernr_coring_low_ratio_read(i);
	}

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{	

		pstNRAttr->stAuto.au8ChromaStr[0][i]  = hi_ext_system_bayernr_auto_chroma_strength_r_read(i);
		pstNRAttr->stAuto.au8ChromaStr[1][i]  =	hi_ext_system_bayernr_auto_chroma_strength_gr_read(i);
		pstNRAttr->stAuto.au8ChromaStr[2][i]  = hi_ext_system_bayernr_auto_chroma_strength_gb_read(i);
		pstNRAttr->stAuto.au8ChromaStr[3][i]  =	hi_ext_system_bayernr_auto_chroma_strength_b_read(i);
		pstNRAttr->stAuto.au8FineStr[i]       = hi_ext_system_bayernr_auto_fine_strength_read(i);		
		pstNRAttr->stAuto.au16CoringWeight[i] = hi_ext_system_bayernr_auto_coring_weight_read(i);
	}
	
    pstNRAttr->stManual.u8ChromaStr[0] = hi_ext_system_bayernr_manual_chroma_strength_read(0);
    pstNRAttr->stManual.u8ChromaStr[1] = hi_ext_system_bayernr_manual_chroma_strength_read(1);
    pstNRAttr->stManual.u8ChromaStr[2] = hi_ext_system_bayernr_manual_chroma_strength_read(2);
	pstNRAttr->stManual.u8ChromaStr[3] = hi_ext_system_bayernr_manual_chroma_strength_read(3);
	pstNRAttr->stManual.u8FineStr = hi_ext_system_bayernr_manual_fine_strength_read();
	pstNRAttr->stManual.u16CoringWeight = hi_ext_system_bayernr_manual_coring_weight_read();
		
	for(i=0;i< WDR_MAX_FRAME; i++)
	{
	  pstNRAttr->stWDR.au8WDRCoarseStr[i]  =  hi_ext_system_bayernr_wdr_strength_read(i);
	}	
	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBlackLevelAttr(ISP_DEV IspDev, const ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    if(pstBlackLevel->au16BlackLevel[0]>0xfff)
	{
	    ISP_TRACE(HI_DBG_ERR, "Invalid au16BlackLevel[0]:%d in %s!\n",pstBlackLevel->au16BlackLevel[0], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstBlackLevel->au16BlackLevel[1]>0xfff)
	{
	    ISP_TRACE(HI_DBG_ERR, "Invalid au16BlackLevel[1]:%d in %s!\n",pstBlackLevel->au16BlackLevel[1], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstBlackLevel->au16BlackLevel[2]>0xfff)
	{
	    ISP_TRACE(HI_DBG_ERR, "Invalid au16BlackLevel[2]:%d in %s!\n",pstBlackLevel->au16BlackLevel[2], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if(pstBlackLevel->au16BlackLevel[3]>0xfff)
	{
	    ISP_TRACE(HI_DBG_ERR, "Invalid au16BlackLevel[3]:%d in %s!\n",pstBlackLevel->au16BlackLevel[3], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
	}
    hi_ext_system_black_level_00_write(pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_01_write(pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_10_write(pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_11_write(pstBlackLevel->au16BlackLevel[3]);
    hi_ext_system_black_level_change_write((HI_U8)HI_TRUE);
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBlackLevelAttr(ISP_DEV IspDev, ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstBlackLevel->au16BlackLevel[0] = hi_ext_system_black_level_00_read();
    pstBlackLevel->au16BlackLevel[1] = hi_ext_system_black_level_01_read();
    pstBlackLevel->au16BlackLevel[2] = hi_ext_system_black_level_10_read();
    pstBlackLevel->au16BlackLevel[3] = hi_ext_system_black_level_11_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_FPNCalibrate(ISP_DEV IspDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCalibrateAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);   
    return ISP_SetCalibrateAttr(0, pstCalibrateAttr);
}

HI_S32 HI_MPI_ISP_SetFPNAttr(ISP_DEV IspDev, const ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_SetCorrectionAttr(0, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_GetFPNAttr(ISP_DEV IspDev, ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_GetCorrectionAttr(0, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_GetLightboxGain(ISP_DEV IspDev, ISP_AWB_Calibration_Gain_S *pstAWBCalibrationGain)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAWBCalibrationGain);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
   return ISP_GetLightboxGain(IspDev, pstAWBCalibrationGain);
}

HI_S32 HI_MPI_ISP_SetDeFogAttr(ISP_DEV IspDev, const ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    int i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    ISP_CHECK_BOOL(pstDefogAttr->bEnable);
    if(pstDefogAttr->enOpType >= OP_TYPE_BUTT)
    {
        printf("Invalid Defog OpType!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->bEnable != HI_FALSE)&&(pstDefogAttr->bEnable != HI_TRUE))
    {
       printf("Invalid Defog Enable Input\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }    
    
    hi_isp_dehaze_en_write(IspDev, pstDefogAttr->bEnable);
    //hi_ext_system_manual_dehaze_hblk_write(pstDefogAttr->u8HorizontalBlock);
  	//hi_ext_system_manual_dehaze_vblk_write(pstDefogAttr->u8VerticalBlock);
  	if((pstDefogAttr->bUserLutEnable != HI_FALSE)&&(pstDefogAttr->bUserLutEnable != HI_TRUE))
    {
       printf("Invalid Defog bUserLutEnable Input\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }  
    hi_ext_system_user_defog_lut_enable_write(pstDefogAttr->bUserLutEnable);
  	for(i = 0;i<256;i++)
  	{
  	    hi_ext_system_defog_lut_write(i,pstDefogAttr->au8DefogLut[i]);
  	}
  	if(pstDefogAttr->bUserLutEnable)
  	{
  	    hi_ext_system_user_defog_lut_update_write(1);//1:update the defog lut,FW will change it to 0 when the lut updating is finished.
  	}
    
    if(pstDefogAttr->enOpType == OP_TYPE_MANUAL)
    {
        hi_ext_system_dehaze_manu_mode_write(OP_TYPE_MANUAL);
    	
    }
    else if(pstDefogAttr->enOpType == OP_TYPE_AUTO)
    {
        hi_ext_system_dehaze_manu_mode_write(OP_TYPE_AUTO);
    }
	hi_ext_system_manual_dehaze_strength_write(pstDefogAttr->stManual.u8strength);
	hi_ext_system_manual_dehaze_autostrength_write(pstDefogAttr->stAuto.u8strength);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDeFogAttr(ISP_DEV IspDev, ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    int i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    pstDefogAttr->bEnable = hi_isp_dehaze_en_read(IspDev);  
    pstDefogAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_dehaze_manu_mode_read();
    pstDefogAttr->stManual.u8strength = hi_ext_system_manual_dehaze_strength_read();
    pstDefogAttr->stAuto.u8strength = hi_ext_system_manual_dehaze_autostrength_read();
    pstDefogAttr->bUserLutEnable= hi_ext_system_user_defog_lut_enable_read();
    for(i=0;i<256;i++)
    {
        pstDefogAttr->au8DefogLut[i] = hi_ext_system_defog_lut_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    return ISP_ACM_SetAttr(pstAcmAttr);
}
HI_S32 HI_MPI_ISP_GetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    return ISP_ACM_GetAttr(pstAcmAttr);
}

HI_S32 HI_MPI_ISP_SetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT)
{
    HI_S32 s32Ret;
    HI_S32 i = 0, j = 0, k = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);   
    for (i = 0; i < ACM_Y_NUM; i++)
    {
        for (j = 0; j < ACM_S_NUM; j++)
        {
            for (k = 0; k < ACM_H_NUM; k++)
            {
                if ((pstAcmLUT->as16Y[i][j][k] < -256) || (pstAcmLUT->as16Y[i][j][k] >= 256))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16Y[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16Y[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
                if ((pstAcmLUT->as16H[i][j][k] < -64) || (pstAcmLUT->as16H[i][j][k] >= 64))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16H[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16H[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
                if ((pstAcmLUT->as16S[i][j][k] < -256) || (pstAcmLUT->as16S[i][j][k] >= 256))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16S[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16S[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
            }
        }
    }
  

    s32Ret = ISP_ACM_SetCoeff(pstAcmLUT);
    return s32Ret;
}
HI_S32 HI_MPI_ISP_GetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT)
{
    HI_S32 s32Ret;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    s32Ret =  ISP_ACM_GetCoeff(pstAcmLUT);
    return s32Ret;
}

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(ISP_DEV IspDev, ISP_INNER_STATE_INFO_S *pstInnerStateInfo)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstInnerStateInfo);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);


	pstInnerStateInfo->u16SharpenUd = hi_ext_system_actual_sharpen_SharpenUd_read();  
	pstInnerStateInfo->u8SharpenD = hi_ext_system_actual_sharpen_SharpenD_read();
	pstInnerStateInfo->u8TextureThr = hi_ext_system_actual_sharpen_TextureThd_read();
	pstInnerStateInfo->u8SharpenEdge = hi_ext_system_actual_sharpen_SharpenEdge_read();
	pstInnerStateInfo->u8EdgeThr = hi_ext_system_actual_sharpen_EdgeThd_read();
	pstInnerStateInfo->u8OverShoot = hi_ext_system_actual_sharpen_overshootAmt_read();
	pstInnerStateInfo->u8UnderShoot = hi_ext_system_actual_sharpen_undershootAmt_read();
	pstInnerStateInfo->u8DetailCtrl = hi_ext_system_actual_sharpen_DetailCtrl_read();
	pstInnerStateInfo->u8ShootSupStr =hi_ext_system_actual_sharpen_shootSupSt_read();

	pstInnerStateInfo->u8ChromaStr[0] = hi_ext_system_bayernr_actual_chroma_strength_read(0);
	pstInnerStateInfo->u8ChromaStr[1] = hi_ext_system_bayernr_actual_chroma_strength_read(1);
	pstInnerStateInfo->u8ChromaStr[2] = hi_ext_system_bayernr_actual_chroma_strength_read(2);
	pstInnerStateInfo->u8ChromaStr[3] = hi_ext_system_bayernr_actual_chroma_strength_read(3);
	pstInnerStateInfo->u8FineStr = hi_ext_system_bayernr_actual_fine_strength_read();
	pstInnerStateInfo->u16CoringWeight = hi_ext_system_bayernr_actual_coring_weight_read();
	pstInnerStateInfo->u32WDRExpRatioActual = hi_isp_wdr_exporatio0_read(IspDev);
	pstInnerStateInfo->u32DefogStrengthActual = hi_ext_system_dehaze_actual_strength_read();
	pstInnerStateInfo->u32DRCStrengthActual = hi_ext_system_drc_actual_strength_read();

    pstInnerStateInfo->bWDRSwitchFinish = hi_ext_top_wdr_switch_read();
    pstInnerStateInfo->bResSwitchFinish = hi_ext_top_res_switch_read();
 
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetStatisticsConfig(ISP_DEV IspDev, const ISP_STATISTICS_CFG_S *pstStatCfg)
{
    HI_U8 u8Shift0;
	HI_U8 i;
    HI_S16 s16G0, s16G1, s16G2;
    HI_FLOAT f32Temp, f32Pl;
    HI_U32 u32Plg, u32Pls;
    ISP_AF_H_PARAM_S *pstIIR;
	
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_BOOL(pstStatCfg->stAECfg.bAERootingEnable);
    ISP_CHECK_BOOL(pstStatCfg->stAECfg.bMGRootingEnable);
   
    if (pstStatCfg->stAECfg.enAESwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Switch %d in %s!\n", pstStatCfg->stAECfg.enAESwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistSkipX > 6)
    {
        printf("u8HistSkipX should not be larger than 6\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistSkipY > 6)
    {
        printf("u8HistSkipY should not be larger than 6\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX > 1)
    {
        printf("u8HistOffsetX should not be larger than 1\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY > 1)
    {
        printf("u8HistOffsetY should not be larger than 1\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enFourPlaneMode > 1)
    {
        printf("enFourPlaneMode should not be larger than 1\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

	if (pstStatCfg->stAECfg.enFourPlaneMode == 0 && pstStatCfg->stAECfg.stHistConfig.u8HistSkipX == 0)
    {
        printf("u8HistSkipX should not be 0 when not in FourPlaneMode\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel > 0xFFFF)
    {
        printf("Max value of Bayer Config u16WhiteLevel is 0xFFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrMax > 0xFFF)
    {
        printf("Max value of Bayer Config u16CrMax is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbMax > 0xFFF)
    {
        printf("Max value of Bayer Config u16CbMax is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel > pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel)
    {
        printf("Bayer Config BlackLevel should not larger than WhiteLevel!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrMin > pstStatCfg->stWBCfg.stBayerCfg.u16CrMax)
    {
        printf("Bayer CrMin should not larger than CrMax!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbMin > pstStatCfg->stWBCfg.stBayerCfg.u16CbMax)
    {
        printf("Bayer CbMin should not larger than CbMax!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

  
    hi_ext_system_statistics_ctrl_write(pstStatCfg->unKey.u32Key);

    if(pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_DG)
    {
        hi_isp_module_ae_sel_write(IspDev, HI_ISP_TOP_AE_SELECT_AFTER_DG);
    }
    else if(pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_STATIC_WB)
    {
        hi_isp_module_ae_sel_write(IspDev, HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB);
    }
    else if(pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_DRC)
    {
        hi_isp_module_ae_sel_write(IspDev, HI_ISP_TOP_AE_SELECT_AFTER_DRC);
    }
    else
    {
        printf("not support!");
    }

    if (HI_TRUE == pstStatCfg->stAECfg.bAERootingEnable)
    {
        hi_isp_ae_gamma_write(IspDev, 0x1);
        hi_isp_ae_gamma_limit_write(IspDev, 0x6);
    }
    else if (HI_FALSE == pstStatCfg->stAECfg.bAERootingEnable)
    {
        hi_isp_ae_gamma_write(IspDev, 0x0);
        hi_isp_ae_gamma_limit_write(IspDev, 0x6);
    }

    if (HI_TRUE == pstStatCfg->stAECfg.bMGRootingEnable)
    {
        hi_isp_mg_gamma_write(IspDev, 0x1);
        hi_isp_mg_gamma_limit_write(IspDev, 0x3);
    }
    else if (HI_FALSE == pstStatCfg->stAECfg.bMGRootingEnable)
    {
        hi_isp_mg_gamma_write(IspDev, 0x0);
        hi_isp_mg_gamma_limit_write(IspDev, 0x3);
    }

	hi_isp_ae_fourplanemode_write(IspDev, pstStatCfg->stAECfg.enFourPlaneMode);
	if(pstStatCfg->stAECfg.enFourPlaneMode == 0)
	{
		for(i = 0; i < 4; i++)
        {
            hi_isp_ae_wdr_skip_x_write(IspDev, i, (pstStatCfg->stAECfg.stHistConfig.u8HistSkipX - 1));
            hi_isp_ae_wdr_skip_y_write(IspDev, i, pstStatCfg->stAECfg.stHistConfig.u8HistSkipY);
            hi_isp_ae_wdr_offset_x_write(IspDev, i, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX);
            hi_isp_ae_wdr_offset_y_write(IspDev, i, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY);
        }
	}
	hi_isp_metering_skip_x_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistSkipX);
	hi_isp_metering_skip_y_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistSkipY);
	hi_isp_metering_offset_x_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX);
	hi_isp_metering_offset_y_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY);

	MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.bEnable,              0x1,       "Invalid AF bEnable input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Hwnd,                 1, 17,        "Invalid AF u16Hwnd input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Vwnd,                 1, 15,        "Invalid AF u16Vwnd input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Hsize,                1, 2048,      "Invalid AF u16Hsize input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Vsize,                1, 2048,      "Invalid AF u16Vsize input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enPeakMode,              0x1,       "Invalid AF enPeakMode input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enSquMode,               0x1,       "Invalid AF enSquMode input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable,          0x1,       "Invalid AF stCrop.bEnable input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16X,             0x1FFF,    "Invalid AF stCrop.u16X input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y,             0x1FFF,    "Invalid AF stCrop.u16Y input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16W,             0x1FFF,    "Invalid AF stCrop.u16W input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16H,             0x1FFF,    "Invalid AF stCrop.u16H input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enStatisticsPos,         0x2,       "Invalid AF enStatisticsPos input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.bGammaEn,       0x1,       "Invalid AF stRawCfg.bGammaEn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn,      0x1,       "Invalid AF stRawCfg.bOffsetEn input\n!");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GrOffset,    0x3FFF,    "Invalid AF stRawCfg.u16GrOffset input\n!");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GbOffset,    0x3FFF,    "Invalid AF stRawCfg.u16GbOffset input\n!");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern,      0x3,       "Invalid AF stRawCfg.enPattern input\n!");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn,         0x1,       "Invalid AF stPreFltCfg.bEn input\n!");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength, 0xFFFF,    "Invalid AF stPreFltCfg.u16strength input\n!");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh,           0xFF,      "Invalid AF u16HighLumaTh input\n!");

    /* IIR0 */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand,            0x1,       "Invalid AF stHParam_IIR0.bNarrowBand input!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn,         HI_BOOL,      3,   0x1, "Invalid AF stHParam_IIR0.abIIREn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0],         0xFF,      "Invalid AF stHParam_IIR0.as16IIRGain[0] input\n!");
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(&pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1], HI_S16,       6, -511, 511, "Invalid AF stHParam_IIR0.as16IIRGain input!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift,    HI_U16,       4,   0x7, "Invalid AF stHParam_IIR0.au16IIRShift input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn,             0x1,       "Invalid AF stHParam_IIR0.stLd.bLdEn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow,          0xFF,      "Invalid AF stHParam_IIR0.stLd.u16ThLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow,        0xFF,      "Invalid AF stHParam_IIR0.stLd.u16GainLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow,         0xF,       "Invalid AF stHParam_IIR0.stLd.u16SlpLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh,         0xFF,      "Invalid AF stHParam_IIR0.stLd.u16ThHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh,       0xFF,      "Invalid AF stHParam_IIR0.stLd.u16GainHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh,        0xF,       "Invalid AF stHParam_IIR0.stLd.u16SlpHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th,         0x7FF,     "Invalid AF stHParam_IIR0.stCoring.u16Th input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp,        0xF,       "Invalid AF stHParam_IIR0.stCoring.u16Slp input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt,        0x7FF,     "Invalid AF stHParam_IIR0.stCoring.u16Lmt input!\n");
    /* IIR1 */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand,            0x1,       "Invalid AF stHParam_IIR1.bNarrowBand input!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn,         HI_BOOL,      3,  0x1, "Invalid AF stHParam_IIR1.abIIREn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0],         0xFF,      "Invalid AF stHParam_IIR1.as16IIRGain[0] input\n!");
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(&pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1], HI_S16,       6, -511, 511, "Invalid AF stHParam_IIR1.as16IIRGain input!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift,    HI_U16,       4,   0x7, "Invalid AF stHParam_IIR1.au16IIRShift input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn,            0x1,       "Invalid AF stHParam_IIR1.stLd.bLdEn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow,         0xFF,      "Invalid AF stHParam_IIR1.stLd.u16ThLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow,       0xFF,      "Invalid AF stHParam_IIR1.stLd.u16GainLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow,        0xF,       "Invalid AF stHParam_IIR1.stLd.u16SlpLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh,        0xFF,      "Invalid AF stHParam_IIR1.stLd.u16ThHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh,      0xFF,      "Invalid AF stHParam_IIR1.stLd.u16GainHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh,       0xF,       "Invalid AF stHParam_IIR1.stLd.u16SlpHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th,        0x7FF,     "Invalid AF stHParam_IIR1.stCoring.u16Th input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp,       0xF,       "Invalid AF stHParam_IIR1.stCoring.u16Slp input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt,       0x7FF,     "Invalid AF stHParam_IIR1.stCoring.u16Lmt input!\n"); 
    /* FIR0 */
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH,     HI_S16,       5, -31,  31,  "Invalid AF stVParam_FIR0.as16FIRH input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn,          0x1,       "Invalid AF stVParam_FIR0.stLd.bLdEn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow,       0xFF,      "Invalid AF stVParam_FIR0.stLd.u16ThLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow,     0xFF,      "Invalid AF stVParam_FIR0.stLd.u16GainLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow,      0xF,       "Invalid AF stVParam_FIR0.stLd.u16SlpLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh,      0xFF,      "Invalid AF stVParam_FIR0.stLd.u16ThHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh,    0xFF,      "Invalid AF stVParam_FIR0.stLd.u16GainHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh,     0xF,       "Invalid AF stVParam_FIR0.stLd.u16SlpHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th,      0x7FF,     "Invalid AF stVParam_FIR0.stCoring.u16Th input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp,     0xF,       "Invalid AF stVParam_FIR0.stCoring.u16Slp input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt,     0x7FF,     "Invalid AF stVParam_FIR0.stCoring.u16Lmt input!\n");    
    /* FIR1 */
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH,     HI_S16,       5, -31,  31,  "Invalid AF stVParam_FIR1.as16FIRH input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn,          0x1,       "Invalid AF stVParam_FIR1.stLd.bLdEn input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow,       0xFF,      "Invalid AF stVParam_FIR1.stLd.u16ThLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow,     0xFF,      "Invalid AF stVParam_FIR1.stLd.u16GainLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow,      0xF,       "Invalid AF stVParam_FIR1.stLd.u16SlpLow input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh,      0xFF,      "Invalid AF stVParam_FIR1.stLd.u16ThHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh,    0xFF,      "Invalid AF stVParam_FIR1.stLd.u16GainHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh,     0xF,       "Invalid AF stVParam_FIR1.stLd.u16SlpHigh input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th,      0x7FF,     "Invalid AF stVParam_FIR1.stCoring.u16Th input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp,     0xF,       "Invalid AF stVParam_FIR1.stCoring.u16Slp input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt,     0x7FF,     "Invalid AF stVParam_FIR1.stCoring.u16Lmt input!\n"); 
    /* FVPARAM */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY,            0xF,       "Invalid AF stFVParam.u16AccShiftY input!\n"); 
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH,    HI_U16,       2,   0xF, "Invalid AF stFVParam.au16AccShiftH input!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV,    HI_U16,       2,   0xF, "Invalid AF stFVParam.au16AccShiftH input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift,           0xF,       "Invalid AF stFVParam.u16HlCntShift input!\n"); 
   
   /* AF paramters check end */

    
    //hi_isp_module_ae_sel_write(pstStatCfg->stAECfg.enAESwitch);
    //hi_isp_top_aesum_switch_write(pstStatCfg->stAECfg.enAESumSwitch);
    //hi_isp_top_histogram_switch_write(pstStatCfg->stAECfg.enHistSwitch);

    hi_isp_metering_max_threshold_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel);
    hi_isp_metering_min_threshold_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel);
    hi_isp_metering_cr_ref_max_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CrMax);
    hi_isp_metering_cr_ref_min_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CrMin);
    hi_isp_metering_cb_ref_max_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CbMax);
    hi_isp_metering_cb_ref_min_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CbMin);
    //hi_isp_metering_cr_ref_high_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh);
    //hi_isp_metering_cr_ref_low_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrLow);
    //hi_isp_metering_cb_ref_high_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh);
    //hi_isp_metering_cb_ref_low_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbLow);

    /* AF STATISTICS CONIFG START */
    
    hi_vi_af_enable_write(pstStatCfg->stFocusCfg.stConfig.bEnable);
    hi_vi_af_iir0_en0_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0]);
    hi_vi_af_iir0_en1_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1]);
    hi_vi_af_iir0_en2_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2]);
    hi_vi_af_iir1_en0_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0]);
    hi_vi_af_iir1_en1_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1]);
    hi_vi_af_iir1_en2_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2]);
    hi_vi_af_fvmode_peak_write(pstStatCfg->stFocusCfg.stConfig.enPeakMode);
    hi_vi_af_fvmode_squ_write(pstStatCfg->stFocusCfg.stConfig.enSquMode);
    hi_vi_af_hwnd_write(pstStatCfg->stFocusCfg.stConfig.u16Hwnd);
    hi_vi_af_vwnd_write(pstStatCfg->stFocusCfg.stConfig.u16Vwnd);
    hi_vi_af_iir0_gain0_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
    hi_vi_af_iir1_gain0_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
    hi_vi_af_iir0_gain1_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1]);
    hi_vi_af_iir1_gain1_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1]);
    hi_vi_af_iir0_gain2_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2]);
    hi_vi_af_iir1_gain2_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2]);
    hi_vi_af_iir0_gain3_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3]);
    hi_vi_af_iir1_gain3_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3]);
    hi_vi_af_iir0_gain4_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4]);
    hi_vi_af_iir1_gain4_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4]);
    hi_vi_af_iir0_gain5_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5]);
    hi_vi_af_iir1_gain5_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5]);
    hi_vi_af_iir0_gain6_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6]);
    hi_vi_af_iir1_gain6_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6]);
    hi_vi_af_iir0_shift0_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0]);
    hi_vi_af_iir0_shift1_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1]);
    hi_vi_af_iir0_shift2_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2]);
    hi_vi_af_iir0_shift3_write(pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3]);
    hi_vi_af_iir1_shift0_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0]);
    hi_vi_af_iir1_shift1_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1]);
    hi_vi_af_iir1_shift2_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2]);
    hi_vi_af_iir1_shift3_write(pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3]);
    hi_vi_af_fir0_h0_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0]);
    hi_vi_af_fir1_h0_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0]);
    hi_vi_af_fir0_h1_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1]);
    hi_vi_af_fir1_h1_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1]);
    hi_vi_af_fir0_h2_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2]);
    hi_vi_af_fir1_h2_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2]);
    hi_vi_af_fir0_h3_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3]);
    hi_vi_af_fir1_h3_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3]);
    hi_vi_af_fir0_h4_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4]);
    hi_vi_af_fir1_h4_write((HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4]);

    /* ds */
    hi_vi_af_iir0_ds_en_write(pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand);
    hi_vi_af_iir1_ds_en_write(pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand);

	/* PLG and PLS */
	{

	   pstIIR = (ISP_AF_H_PARAM_S *)(&pstStatCfg->stFocusCfg.stHParam_IIR0);
	   
	   u8Shift0 =  pstIIR->au16IIRShift[0];
	   s16G0 = pstIIR->as16IIRGain[0];
	   s16G1 = pstIIR->as16IIRGain[1];
	   s16G2 = pstIIR->as16IIRGain[2];
	   
	   f32Pl = (512.f / (512-2*s16G1-s16G2) * s16G0) /	(1 << u8Shift0);
	   f32Temp = f32Pl;
	   f32Temp = MIN2(7-floor(log(f32Temp)/log(2)), 7);
	   
	   u32Pls = (HI_U32)f32Temp;
	   u32Plg = (HI_U32)((f32Pl * (1 << u32Pls)) + 0.5);

	   hi_vi_af_iir0_pls_write(u32Pls);
	   hi_vi_af_iir0_plg_write(u32Plg);

	}
	
    {
		pstIIR = (ISP_AF_H_PARAM_S *)(&pstStatCfg->stFocusCfg.stHParam_IIR1) ;

		u8Shift0 =  pstIIR->au16IIRShift[0];
		s16G0 = pstIIR->as16IIRGain[0];
		s16G1 = pstIIR->as16IIRGain[1];
		s16G2 = pstIIR->as16IIRGain[2];

		f32Pl = (512.f / (512-2*s16G1-s16G2) * s16G0) /  (1 << u8Shift0);
		f32Temp = f32Pl;
		f32Temp = MIN2(7-floor(log(f32Temp)/log(2)), 7);

		u32Pls = (HI_U32)f32Temp;
		u32Plg = (HI_U32)((f32Pl * (1 << u32Pls)) + 0.5);

		hi_vi_af_iir1_pls_write(u32Pls);
		hi_vi_af_iir1_plg_write(u32Plg);		  
	}


	/* AF crop */
	hi_vi_af_crop_en_write(pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable);
	//if (pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable)
	{
        hi_vi_af_crop_posx_write(pstStatCfg->stFocusCfg.stConfig.stCrop.u16X);
        hi_vi_af_crop_posy_write(pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y);
        hi_vi_af_crop_hsize_write(pstStatCfg->stFocusCfg.stConfig.stCrop.u16W);
        hi_vi_af_crop_vsize_write(pstStatCfg->stFocusCfg.stConfig.stCrop.u16H);
    }

    /* AF raw cfg */
    if (ISP_AF_STATISTICS_YUV == pstStatCfg->stFocusCfg.stConfig.enStatisticsPos)
    {
        hi_vi_af_pos_write(HI_VI_AF_POS_BE);
        hi_vi_af_raw_mode_write(HI_VI_AF_RAW_MODE_YUV);
    }
    else if (ISP_AF_STATISTICS_RAW == pstStatCfg->stFocusCfg.stConfig.enStatisticsPos)
    {
        hi_vi_af_pos_write(HI_VI_AF_POS_FE);
        hi_vi_af_raw_mode_write(HI_VI_AF_RAW_MODE_RAW);
        hi_isp_module_af_sel_write(IspDev, HI_ISP_MOUDLE_POS_AF_AFTER_DG);
    }
    else if (ISP_AF_STATISTICS_RAW_AFTER_DRC == pstStatCfg->stFocusCfg.stConfig.enStatisticsPos)
    {
        hi_vi_af_pos_write(HI_VI_AF_POS_FE);
        hi_vi_af_raw_mode_write(HI_VI_AF_RAW_MODE_RAW);
        hi_isp_module_af_sel_write(IspDev, HI_ISP_MOUDLE_POS_AF_AFTER_DRC);
    }
    else
    {
        printf("Invalid AF enStatisticsPos input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    //hi_vi_af_pos_write(~pstStatCfg->stFocusCfg.stConfig.enStatisticsPos);
    //hi_vi_af_raw_mode_write(pstStatCfg->stFocusCfg.stConfig.enStatisticsPos);
    
    //if (ISP_AF_STATISTICS_RAW == pstStatCfg->stFocusCfg.stConfig.enStatisticsPos)
    {
        hi_vi_af_sqrt_en_write(pstStatCfg->stFocusCfg.stConfig.stRawCfg.bGammaEn);
        hi_vi_af_bayer_mode_write(pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern);
        hi_vi_af_offset_en_write(pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn);
        //if (pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn)
        {
            hi_vi_af_offset_gr_write(pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GrOffset);
            hi_vi_af_offset_gb_write(pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GbOffset);
        }         
    }

    /* AF pre median filter */
    hi_vi_af_median_en_write(pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn);
    //if (pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn)
    {    
        hi_vi_af_median_th_write(0xffff - pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength);
    }

    /* level depend gain */
    hi_vi_af_iir0_ldg_en_write(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn)
    {
        hi_vi_af_iir0_ldg_th_l_write  (pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow    );
        hi_vi_af_iir0_ldg_th_h_write  (pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh   );
        hi_vi_af_iir0_ldg_slp_l_write (pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow   );
        hi_vi_af_iir0_ldg_slp_h_write (pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh  );
        hi_vi_af_iir0_ldg_gain_l_write(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow  );
        hi_vi_af_iir0_ldg_gain_h_write(pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh );

    }
    
    hi_vi_af_iir1_ldg_en_write(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn)
    {
        hi_vi_af_iir1_ldg_th_l_write  (pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow    );
        hi_vi_af_iir1_ldg_th_h_write  (pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh   );
        hi_vi_af_iir1_ldg_slp_l_write (pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow   );
        hi_vi_af_iir1_ldg_slp_h_write (pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh  );
        hi_vi_af_iir1_ldg_gain_l_write(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow  );
        hi_vi_af_iir1_ldg_gain_h_write(pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh );
    }

    hi_vi_af_fir0_ldg_en_write(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn)
    {
        hi_vi_af_fir0_ldg_th_l_write  (pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow    );
        hi_vi_af_fir0_ldg_th_h_write  (pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh   );
        hi_vi_af_fir0_ldg_slp_l_write (pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow   );
        hi_vi_af_fir0_ldg_slp_h_write (pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh  );
        hi_vi_af_fir0_ldg_gain_l_write(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow  );
        hi_vi_af_fir0_ldg_gain_h_write(pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh );
    }

    hi_vi_af_fir1_ldg_en_write(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn)
    {
        hi_vi_af_fir1_ldg_th_l_write  (pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow    );
        hi_vi_af_fir1_ldg_th_h_write  (pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh   );
        hi_vi_af_fir1_ldg_slp_l_write (pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow   );
        hi_vi_af_fir1_ldg_slp_h_write (pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh  );
        hi_vi_af_fir1_ldg_gain_l_write(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow  );
        hi_vi_af_fir1_ldg_gain_h_write(pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh );
    }

    /* AF coring */
    hi_vi_af_iir0_cor_th_write   (pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th   );
    hi_vi_af_iir0_cor_slp_write  (pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp  );
    hi_vi_af_iir0_cor_peak_write (pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt  );

    hi_vi_af_iir1_cor_th_write   (pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th   );
    hi_vi_af_iir1_cor_slp_write  (pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp  );
    hi_vi_af_iir1_cor_peak_write (pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt  ); 
                                                                      
    hi_vi_af_fir0_cor_th_write   (pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th   );
    hi_vi_af_fir0_cor_slp_write  (pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp  );
    hi_vi_af_fir0_cor_peak_write (pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt  );
                                                                      
    hi_vi_af_fir1_cor_th_write   (pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th   );
    hi_vi_af_fir1_cor_slp_write  (pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp  );
    hi_vi_af_fir1_cor_peak_write (pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt  );

    /* high luma counter */
    hi_vi_af_highluma_th_write(pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh);

    /* AF output shift */
    hi_vi_af_acc_shift_h0_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0]);
    hi_vi_af_acc_shift_h1_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1]);
    hi_vi_af_acc_shift_v0_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0]);
    hi_vi_af_acc_shift_v1_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1]);
    hi_vi_af_acc_shift_y_write(pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY);
    hi_vi_af_cnt_shift_highluma_write(pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift);
    
    hi_vi_af_hsize_write(pstStatCfg->stFocusCfg.stConfig.u16Hsize - 1);
    hi_vi_af_vsize_write(pstStatCfg->stFocusCfg.stConfig.u16Vsize - 1);
    
	
    return HI_SUCCESS;
}

#define MY 0

HI_S32 HI_MPI_ISP_GetStatisticsConfig(ISP_DEV IspDev, ISP_STATISTICS_CFG_S *pstStatCfg)
{
	HI_U8  u8Tmp;
    HI_U32 u32AFPos = 0, u32AFRawCfg = 0;

    HI_U8 u8AE_Gamma = 0;
    HI_U8 u8MG_Gamma = 0;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    pstStatCfg->unKey.u32Key = hi_ext_system_statistics_ctrl_read();
   
   	u8Tmp = hi_isp_module_ae_sel_read(IspDev);
    if (u8Tmp == HI_ISP_TOP_AE_SELECT_AFTER_DG)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_DG;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_STATIC_WB;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SELECT_AFTER_DRC)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_DRC;
    }
    else
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_SWITCH_BUTT;
    }

	u8Tmp = hi_isp_ae_fourplanemode_read(IspDev);
	if (u8Tmp == HI_ISP_AE_FOUR_PLANE_MODE_DISABLE)
    {
        pstStatCfg->stAECfg.enFourPlaneMode= ISP_AE_FOUR_PLANE_MODE_DISABLE;
    }
    else if (u8Tmp == HI_ISP_AE_FOUR_PLANE_MODE_ENABLE)
    {
        pstStatCfg->stAECfg.enFourPlaneMode = ISP_AE_FOUR_PLANE_MODE_ENABLE;
    }
	else
	{
		pstStatCfg->stAECfg.enFourPlaneMode = ISP_AE_FOUR_PLANE_MODE_BUTT;
    }

	pstStatCfg->stAECfg.stHistConfig.u8HistSkipX = hi_isp_metering_skip_x_read(IspDev);
    pstStatCfg->stAECfg.stHistConfig.u8HistSkipY = hi_isp_metering_skip_y_read(IspDev);
    pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX = hi_isp_metering_offset_x_read(IspDev);
    pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY = hi_isp_metering_offset_y_read(IspDev);
	//pstStatCfg->stAECfg.enAESwitch = (u8Tmp > 7) ? ISP_AE_SWITCH_BUTT : u8Tmp;

    pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel = hi_isp_metering_max_threshold_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel = hi_isp_metering_min_threshold_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMax = hi_isp_metering_cr_ref_max_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMin = hi_isp_metering_cr_ref_min_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMax = hi_isp_metering_cb_ref_max_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMin = hi_isp_metering_cb_ref_min_awb_sum_read(IspDev);
    //pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh = hi_isp_metering_cr_ref_high_awb_sum_read();
    //pstStatCfg->stWBCfg.stBayerCfg.u16CrLow = hi_isp_metering_cr_ref_low_awb_sum_read();
    //pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh = hi_isp_metering_cb_ref_high_awb_sum_read();
    //pstStatCfg->stWBCfg.stBayerCfg.u16CbLow = hi_isp_metering_cb_ref_low_awb_sum_read();

    pstStatCfg->stFocusCfg.stConfig.bEnable = hi_vi_af_enable_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0] = hi_vi_af_iir0_en0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1] = hi_vi_af_iir0_en1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2] = hi_vi_af_iir0_en2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0] = hi_vi_af_iir1_en0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1] = hi_vi_af_iir1_en1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2] = hi_vi_af_iir1_en2_read();
    pstStatCfg->stFocusCfg.stConfig.enPeakMode = hi_vi_af_fvmode_peak_read();
    pstStatCfg->stFocusCfg.stConfig.enSquMode = hi_vi_af_fvmode_squ_read();
    pstStatCfg->stFocusCfg.stConfig.u16Hwnd = hi_vi_af_hwnd_read();
    pstStatCfg->stFocusCfg.stConfig.u16Vwnd = hi_vi_af_vwnd_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] = (HI_S16)hi_vi_af_iir0_gain0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] = (HI_S16)hi_vi_af_iir1_gain0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1] = (HI_S16)hi_vi_af_iir0_gain1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1] = (HI_S16)hi_vi_af_iir1_gain1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2] = (HI_S16)hi_vi_af_iir0_gain2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2] = (HI_S16)hi_vi_af_iir1_gain2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3] = (HI_S16)hi_vi_af_iir0_gain3_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3] = (HI_S16)hi_vi_af_iir1_gain3_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4] = (HI_S16)hi_vi_af_iir0_gain4_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4] = (HI_S16)hi_vi_af_iir1_gain4_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5] = (HI_S16)hi_vi_af_iir0_gain5_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5] = (HI_S16)hi_vi_af_iir1_gain5_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6] = (HI_S16)hi_vi_af_iir0_gain6_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6] = (HI_S16)hi_vi_af_iir1_gain6_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0] = hi_vi_af_iir0_shift0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1] = hi_vi_af_iir0_shift1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2] = hi_vi_af_iir0_shift2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3] = hi_vi_af_iir0_shift3_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0] = hi_vi_af_iir1_shift0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1] = hi_vi_af_iir1_shift1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2] = hi_vi_af_iir1_shift2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3] = hi_vi_af_iir1_shift3_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0] = (HI_S16)hi_vi_af_fir0_h0_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0] = (HI_S16)hi_vi_af_fir1_h0_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1] = (HI_S16)hi_vi_af_fir0_h1_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1] = (HI_S16)hi_vi_af_fir1_h1_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2] = (HI_S16)hi_vi_af_fir0_h2_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2] = (HI_S16)hi_vi_af_fir1_h2_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3] = (HI_S16)hi_vi_af_fir0_h3_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3] = (HI_S16)hi_vi_af_fir1_h3_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4] = (HI_S16)hi_vi_af_fir0_h4_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4] = (HI_S16)hi_vi_af_fir1_h4_read();

    /* ds */
    pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand = hi_vi_af_iir0_ds_en_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand = hi_vi_af_iir1_ds_en_read();

    /* AF crop */
    pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable = hi_vi_af_crop_en_read();
    {
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16X = hi_vi_af_crop_posx_read();
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y = hi_vi_af_crop_posy_read();
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16W = hi_vi_af_crop_hsize_read();
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16H = hi_vi_af_crop_vsize_read();
    }

    /* AF raw cfg */
    u32AFPos    = hi_vi_af_pos_read();
    u32AFRawCfg = hi_isp_module_af_sel_read(IspDev);
    if (HI_VI_AF_POS_BE == u32AFPos)
    {
        pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ISP_AF_STATISTICS_YUV;
    }
    else
    {
        if (HI_ISP_MOUDLE_POS_AF_AFTER_DG == u32AFRawCfg)
        {
            pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ISP_AF_STATISTICS_RAW;
        }
        else if (HI_ISP_MOUDLE_POS_AF_AFTER_DRC == u32AFRawCfg)
        {
            pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ISP_AF_STATISTICS_RAW_AFTER_DRC;
        }
        else
        {
            pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ISP_AF_STATISTICS_BUTT;
        }
    }
    
    u8AE_Gamma = hi_isp_ae_gamma_read(IspDev);
    u8MG_Gamma = hi_isp_mg_gamma_read(IspDev);
    
    if (u8AE_Gamma == 0x1)
    {
        pstStatCfg->stAECfg.bAERootingEnable = HI_TRUE;
    }
    else 
    {
        pstStatCfg->stAECfg.bAERootingEnable = HI_FALSE;
    }

    if (u8MG_Gamma == 0x1)
    {
        pstStatCfg->stAECfg.bMGRootingEnable = HI_TRUE;
    }
    else
    {
        pstStatCfg->stAECfg.bMGRootingEnable = HI_FALSE;
    }

    //pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ~hi_vi_af_pos_read();
    //pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = hi_vi_af_raw_mode_read();
    {
        pstStatCfg->stFocusCfg.stConfig.stRawCfg.bGammaEn = hi_vi_af_sqrt_en_read();
        pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern = hi_vi_af_bayer_mode_read();
        pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn = hi_vi_af_offset_en_read();
        {
            pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GrOffset = hi_vi_af_offset_gr_read();
            pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GbOffset = hi_vi_af_offset_gb_read();
        } 
    }

    /* AF pre median filter */
    pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn = hi_vi_af_median_en_read();
    {    
        pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength = 0xffff - hi_vi_af_median_th_read();
    }

    /* level depend gain */
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn = hi_vi_af_iir0_ldg_en_read();
    {
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow    = hi_vi_af_iir0_ldg_th_l_read  ();
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh   = hi_vi_af_iir0_ldg_th_h_read  ();
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow   = hi_vi_af_iir0_ldg_slp_l_read ();
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh  = hi_vi_af_iir0_ldg_slp_h_read ();
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow  = hi_vi_af_iir0_ldg_gain_l_read();
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh = hi_vi_af_iir0_ldg_gain_h_read();

    }
    
    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn = hi_vi_af_iir1_ldg_en_read();
    {
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow    = hi_vi_af_iir1_ldg_th_l_read  ();
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh   = hi_vi_af_iir1_ldg_th_h_read  ();
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow   = hi_vi_af_iir1_ldg_slp_l_read ();
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh  = hi_vi_af_iir1_ldg_slp_h_read ();
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow  = hi_vi_af_iir1_ldg_gain_l_read();
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh = hi_vi_af_iir1_ldg_gain_h_read();
    }

    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn = hi_vi_af_fir0_ldg_en_read();
    {
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow    = hi_vi_af_fir0_ldg_th_l_read  ();
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh   = hi_vi_af_fir0_ldg_th_h_read  ();
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow   = hi_vi_af_fir0_ldg_slp_l_read ();
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh  = hi_vi_af_fir0_ldg_slp_h_read ();
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow  = hi_vi_af_fir0_ldg_gain_l_read();
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh = hi_vi_af_fir0_ldg_gain_h_read();
    }

    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn = hi_vi_af_fir1_ldg_en_read();
    {
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow    = hi_vi_af_fir1_ldg_th_l_read  ();
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh   = hi_vi_af_fir1_ldg_th_h_read  ();
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow   = hi_vi_af_fir1_ldg_slp_l_read ();
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh  = hi_vi_af_fir1_ldg_slp_h_read ();
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow  = hi_vi_af_fir1_ldg_gain_l_read();
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh = hi_vi_af_fir1_ldg_gain_h_read();
    }

    /* AF coring */
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th  = hi_vi_af_iir0_cor_th_read   ();
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp = hi_vi_af_iir0_cor_slp_read  ();
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt = hi_vi_af_iir0_cor_peak_read ();
                                                          
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th  = hi_vi_af_iir1_cor_th_read   ();
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp = hi_vi_af_iir1_cor_slp_read  ();
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt = hi_vi_af_iir1_cor_peak_read (); 
                                                                      
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th  = hi_vi_af_fir0_cor_th_read   ();
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp = hi_vi_af_fir0_cor_slp_read  ();
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt = hi_vi_af_fir0_cor_peak_read ();
                                                                      
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th  = hi_vi_af_fir1_cor_th_read   ();
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp = hi_vi_af_fir1_cor_slp_read  ();
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt = hi_vi_af_fir1_cor_peak_read ();

    /* high luma counter */
    pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh = hi_vi_af_highluma_th_read();
  
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0] = hi_vi_af_acc_shift_h0_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1] = hi_vi_af_acc_shift_h1_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0] = hi_vi_af_acc_shift_v0_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1] = hi_vi_af_acc_shift_v1_read();
    pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY = hi_vi_af_acc_shift_y_read();
    pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift = hi_vi_af_cnt_shift_highluma_read();
    pstStatCfg->stFocusCfg.stConfig.u16Hsize = hi_vi_af_hsize_read() + 1;
    pstStatCfg->stFocusCfg.stConfig.u16Vsize = hi_vi_af_vsize_read() + 1;
	
    return HI_SUCCESS;
}

HI_U32 ISP_TransStatInfo(HI_VOID **ppSrcVirAddr, HI_VOID **ppDestVirAddr)
{
    ISP_STATISTICS_S* pstStat = *ppDestVirAddr;
    HI_U32 IspDev = 0;
    ISP_STATISTICS_CTRL_U unStatKey;
    HI_U8 u8WDRMode;
    HI_S32 i, j;

    HI_U32 *pAWBVirAddr = NULL;
    HI_U32 *pAE3VirAddr = NULL;
    HI_U32 *pAE5VirAddr1 = NULL;
    HI_U32 *pAE5VirAddr2 = NULL;
    HI_U32 *pAE6VirAddr0 = NULL;
    HI_U32 *pAE6VirAddr1 = NULL;
    HI_U32 *pAE6VirAddr2 = NULL;
    HI_U32 *pAE6VirAddr3 = NULL;
    HI_U32 *pMGVirAddr = NULL;
    HI_U32 *pAFVirAddr = NULL;

    unStatKey.u32Key = hi_ext_system_statistics_ctrl_read();

    pAE3VirAddr = (HI_U32 *)(*ppSrcVirAddr);
    pAWBVirAddr = pAE3VirAddr + 1024;
    pAE5VirAddr1 = pAWBVirAddr + 2048;
    pAE5VirAddr2 = pAE5VirAddr1 + 256;
    pAE6VirAddr0 = pAE5VirAddr2 + 256;
    pAE6VirAddr1 = pAE6VirAddr0 + 256;
    pAE6VirAddr2 = pAE6VirAddr1 + 256;
    pAE6VirAddr3 = pAE6VirAddr2 + 256;
    pMGVirAddr = pAE6VirAddr3 + 256;
    pAFVirAddr = pMGVirAddr + 256;

    if (HI_NULL == pstStat)
    {
        return HI_ERR_ISP_NULL_PTR;
    }

    if (unStatKey.bit1AwbStat3)
    {
        pstStat->stWBStat.stBayerStatistics.u16GlobalR = hi_isp_metering_avg_r_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16GlobalG = hi_isp_metering_avg_g_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16GlobalB = hi_isp_metering_avg_b_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16CountAll = hi_isp_metering_count_all_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16CountMax = 0;
        pstStat->stWBStat.stBayerStatistics.u16CountMin = 0;
    }
    
    if(unStatKey.bit1AwbStat4)
    {
        for(i = 0; i < AWB_ZONE_ROW; i++)
        {
            for(j = 0; j < AWB_ZONE_COLUMN; j++)
            {
                HI_U32 tmp1,tmp2,k; 
                k = (i*AWB_ZONE_COLUMN+j) * 2;
                tmp1 = pAWBVirAddr[k];
                tmp2 = pAWBVirAddr[k + 1];

                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] = tmp1 & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] = (tmp1 >> 16) & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] = tmp2 & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i][j] = (tmp2 >> 16) & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i][j] = 0;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i][j] = 0;
            }
        }
    }

    if (unStatKey.bit1AfStat)
    {
        HI_U32 u32Zone = hi_area_block_distribute_data_read_af();
        HI_U8 u8Col = (u32Zone & 0x1F);
        HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);

        if( u8Col > AF_ZONE_COLUMN )
        {
          u8Col = AF_ZONE_COLUMN;
        }
        if(u8Row > AF_ZONE_ROW)
        {
          u8Row = AF_ZONE_ROW;
        }

        for (i = 0; i < u8Row; i++)
        {
            for (j = 0; j < u8Col; j++)
            {
                HI_U32 tmp1,tmp2,tmp3,k;
                k=(i*u8Col+j)*4;

                tmp1 = pAFVirAddr[k];
                tmp2 = pAFVirAddr[k+1];
                tmp3 = pAFVirAddr[k+2];

                pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((0xFFFF0000 & tmp1) >> 16);
                pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(0xFFFF & tmp1);
                pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((0xFFFF0000 & tmp2) >> 16);
                pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(0xFFFF & tmp2);
                pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = (HI_U16)(0xFFFF & tmp3);
                pstStat->stFocusStat.stZoneMetrics[i][j].u16HlCnt = (HI_U16)((0xFFFF0000 & tmp3) >> 16);
            }
        }
    }

    if(unStatKey.bit1AeStat3)
    {
        for(i = 0; i < 1024; i++)
        {
            pstStat->stAEStat.au32Hist1024Value[i] = *((HI_U32 *)pAE3VirAddr+i);
        }
    }

    if (unStatKey.bit1AeStat4)
    {
        pstStat->stAEStat.au16GlobalAvg[0] = hi_isp_ae_total_r_aver_read(IspDev);
        pstStat->stAEStat.au16GlobalAvg[1] = hi_isp_ae_total_gr_aver_read(IspDev);
        pstStat->stAEStat.au16GlobalAvg[2] = hi_isp_ae_total_gb_aver_read(IspDev);
        pstStat->stAEStat.au16GlobalAvg[3] = hi_isp_ae_total_b_aver_read(IspDev);
    }

    if (unStatKey.bit1AeStat5)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                HI_U32 tmp1,tmp2;
                tmp1 = pAE5VirAddr1[i * AE_ZONE_COLUMN+j];
                tmp2 = pAE5VirAddr2[i * AE_ZONE_COLUMN+j];

                pstStat->stAEStat.au16ZoneAvg[i][j][0] = (HI_U16)((tmp1 & 0xffff0000) >> 16);
                pstStat->stAEStat.au16ZoneAvg[i][j][1] = (HI_U16)((tmp1 & 0xffff));
                pstStat->stAEStat.au16ZoneAvg[i][j][2] = (HI_U16)((tmp2 & 0xffff0000) >> 16);
                pstStat->stAEStat.au16ZoneAvg[i][j][3] = (HI_U16)((tmp2 & 0xffff));
            }       
        }
    }

    if(unStatKey.bit1AeStat6)
    {
        for(i = 0; i < 256; i++)
        {
            pstStat->stAEStat.au32PreChn0Hist256Value[i] = *((HI_U32 *)pAE6VirAddr0+i);
            pstStat->stAEStat.au32PreChn1Hist256Value[i] = *((HI_U32 *)pAE6VirAddr1+i);
        }
        u8WDRMode = hi_ext_top_sensor_wdr_mode_read();
        if((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode)))
        {
            memset(&pstStat->stAEStat.au32PreChn1Hist256Value, 0, sizeof(pstStat->stAEStat.au32PreChn1Hist256Value));
        }
        else
        {
        }
    }

    if(unStatKey.bit1MgStat)
    {
        for(i = 0;i < MG_ZONE_ROW ; i++)
        {
            for(j=0;j< MG_ZONE_COLUMN ;j++)
            {
                HI_U32 tmp;

                tmp = pMGVirAddr[i*MG_ZONE_COLUMN+j];
                pstStat->stMGStat.au16ZoneAvg[i][j][0] = (HI_U16)((tmp & 0xff000000) >> 24);
                pstStat->stMGStat.au16ZoneAvg[i][j][1] = (HI_U16)((tmp & 0xff0000) >> 16);             
                pstStat->stMGStat.au16ZoneAvg[i][j][2] = (HI_U16)((tmp & 0xff00) >> 8);
                pstStat->stMGStat.au16ZoneAvg[i][j][3] = (HI_U16)(tmp & 0xff);
            }      
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetStatistics(ISP_DEV IspDev, ISP_STATISTICS_S *pstStat)
{
	HI_S32 s32Ret;
    ISP_STAT_INFO_S stActStatInfo = {{0}};

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    ISP_CHECK_OPEN(IspDev);

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_ACT_GET, &stActStatInfo);
    if (s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "Get Active Stat Buffer Err\n");
        return s32Ret;
    }
    stActStatInfo.pVirtAddr = HI_MPI_SYS_MmapCache(stActStatInfo.u32PhyAddr, sizeof(ISP_STAT_S));
    if ( !stActStatInfo.pVirtAddr )
    {
        return HI_ERR_ISP_NULL_PTR;
    }

    s32Ret = ISP_TransStatInfo(&stActStatInfo.pVirtAddr, (HI_VOID *)&pstStat);
    if (s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "Get Statistics Err : %x\n", s32Ret);
        HI_MPI_SYS_Munmap(stActStatInfo.pVirtAddr, sizeof(ISP_STAT_S));
        return s32Ret;
    }
    HI_MPI_SYS_Munmap(stActStatInfo.pVirtAddr, sizeof(ISP_STAT_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetISPRegAttr(ISP_DEV IspDev, ISP_REG_ATTR_S *pstIspRegAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspRegAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    MUTEX_LOCK(pstIspCtx->stLock);
    VReg_Munmap();

    pstIspRegAttr->u32IspRegAddr    = VReg_GetVirtAddr(ISP_REG_BASE);
    pstIspRegAttr->u32IspRegSize    = ISP_REG_SIZE;
    pstIspRegAttr->u32IspExtRegAddr = VReg_GetVirtAddr(ISP_VREG_BASE);
    pstIspRegAttr->u32IspExtRegSize = ISP_VREG_SIZE;
    pstIspRegAttr->u32AeExtRegAddr  = VReg_GetVirtAddr(AE_LIB_VREG_BASE(0));
    pstIspRegAttr->u32AeExtRegSize  = ALG_LIB_VREG_SIZE;
    pstIspRegAttr->u32AwbExtRegAddr = VReg_GetVirtAddr(AWB_LIB_VREG_BASE(0));
    pstIspRegAttr->u32AwbExtRegSize = ALG_LIB_VREG_SIZE;

    MUTEX_UNLOCK(pstIspCtx->stLock);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDebug(ISP_DEV IspDev, const ISP_DEBUG_INFO_S *pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_BOOL(pstIspDebug->bDebugEn);

    return ISP_DbgSet(pstIspDebug);
}

HI_S32 HI_MPI_ISP_GetDebug(ISP_DEV IspDev, ISP_DEBUG_INFO_S * pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);    
    return ISP_DbgGet(pstIspDebug);
}

HI_S32 HI_MPI_ISP_SetCAAttr(ISP_DEV IspDev, const ISP_CA_ATTR_S *pstCAAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstCAAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);	
	ISP_CHECK_BOOL(pstCAAttr->bEnable);
	
	hi_isp_ca_cfg_en_write(IspDev, pstCAAttr->bEnable);


	for(i=0;i<HI_ISP_CA_YRATIO_LUT_LENGTH;i++)
	{
		if(pstCAAttr->au16YRatioLut[i] > 2047)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid CaAttr  au16YRatioLut[%d] value in %s!\n",i, __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		hi_ext_system_ca_y_ratio_lut_write(i, pstCAAttr->au16YRatioLut[i]);
	}
	for(i=0;i<ISP_AUTO_ISO_STRENGTH_NUM;i++)
	{
		if(pstCAAttr->as32ISORatio[i] > 2047)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid CaAttr  as32ISORatio[%d] value in %s!\n",i, __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		hi_ext_system_ca_iso_ratio_lut_write(i,pstCAAttr->as32ISORatio[i]);
	}
	
	return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetCAAttr(ISP_DEV IspDev, ISP_CA_ATTR_S *pstCAAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstCAAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	pstCAAttr->bEnable = hi_isp_ca_cfg_en_read(IspDev);


	for(i=0;i<HI_ISP_CA_YRATIO_LUT_LENGTH;i++)
	{
		pstCAAttr->au16YRatioLut[i] = hi_ext_system_ca_y_ratio_lut_read(i);
	}
	for(i=0;i<ISP_AUTO_ISO_STRENGTH_NUM;i++)
	{
		pstCAAttr->as32ISORatio[i] = hi_ext_system_ca_iso_ratio_lut_read(i);
	}

	return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{	
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(0);    
	
	return 	ioctl(g_as32IspFd[0], ISP_SET_MOD_PARAM, pstIspModParam);
}

HI_S32 HI_MPI_ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(0);    

    return ioctl(g_as32IspFd[0], ISP_GET_MOD_PARAM, pstIspModParam);
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

