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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>

#include "hi_comm_isp.h"
#include "mpi_sys.h"
#include "mpi_isp.h"
#include "mkp_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"

#include "isp_config.h"
#include "isp_acm_config.h"
#include "isp_ext_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_shading_mem_config.h"
#include "isp_metering_mem_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_gamma_fe0_mem_config.h"
#include "isp_gamma_fe1_mem_config.h"
#include "isp_debug.h"
#include "isp_main.h"
#include "isp_proc.h"

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
    do {                                                                      \
        if ((x) > (max))                                                      \
        {                                                                     \
            printf("[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);     \
            printf("%#x "fmt, x, ##__VA_ARGS__);                              \
            return HI_ERR_ISP_ILLEGAL_PARAM;                                  \
        }                                                                     \
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

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if ((pstPubAttr->stWndRect.u32Width < RES_WIDTH_MIN) || (pstPubAttr->stWndRect.u32Width > RES_WIDTH_MAX) \
        || (pstPubAttr->stWndRect.u32Width % 2 != 0))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Width:%d!\n", pstPubAttr->stWndRect.u32Width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.u32Height < RES_HEIGHT_MIN) || (pstPubAttr->stWndRect.u32Height > RES_HEIGHT_MAX) \
        || (pstPubAttr->stWndRect.u32Height % 2 != 0))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Height:%d!\n", pstPubAttr->stWndRect.u32Height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stSnsSize.u32Width < RES_WIDTH_MIN) || (pstPubAttr->stSnsSize.u32Width > RES_WIDTH_MAX))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sensor Image Width:%d!\n", pstPubAttr->stSnsSize.u32Width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stSnsSize.u32Height < RES_HEIGHT_MIN) || (pstPubAttr->stSnsSize.u32Height > RES_HEIGHT_MAX))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sensor Image Height:%d!\n", pstPubAttr->stSnsSize.u32Height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.s32X < 0) || (pstPubAttr->stWndRect.s32X > RES_WIDTH_MAX - RES_WIDTH_MIN))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image X:%d!\n", pstPubAttr->stWndRect.s32X);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.s32Y < 0) || (pstPubAttr->stWndRect.s32Y > RES_HEIGHT_MAX - RES_HEIGHT_MIN))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Y:%d!\n", pstPubAttr->stWndRect.s32Y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->f32FrameRate <= 0.0) || (pstPubAttr->f32FrameRate > FRAME_RATE_MAX))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid FrameRate:%f!\n", pstPubAttr->f32FrameRate);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstPubAttr->enBayer >= BAYER_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Bayer Pattern:%d!\n", pstPubAttr->enBayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_res_switch_write(IspDev, HI_FALSE);

    hi_ext_top_sensor_width_write(IspDev, pstPubAttr->stSnsSize.u32Width);
    hi_ext_top_sensor_height_write(IspDev, pstPubAttr->stSnsSize.u32Height);

    if (HI_TRUE == hi_ext_top_pub_attr_cfg_read(IspDev))
    {
        hi_isp_input_port_vc_size_write(IspDev, pstPubAttr->stWndRect.u32Height);
        hi_isp_top_active_width_write(IspDev, pstPubAttr->stWndRect.u32Width);
        hi_isp_top_active_height_write(IspDev, pstPubAttr->stWndRect.u32Height);
        hi_isp_top_rggb_start_write(IspDev, (HI_U8)pstPubAttr->enBayer);

        pValue = (HI_VOID *)(&pstPubAttr->f32FrameRate);
        hi_ext_system_fps_base_write(IspDev, *(HI_U32 *)pValue);

        hi_isp_fe_input_port_crop_enable_write(IspDev, 1);
        hi_isp_fe_input_port_crop_x_start_write(IspDev, pstPubAttr->stWndRect.s32X);
        hi_isp_fe_input_port_crop_width_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
        hi_isp_fe_input_port_crop_y_start_write(IspDev, pstPubAttr->stWndRect.s32Y);
        hi_isp_fe_input_port_crop_height_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);
    }
    else
    {
        /* himm 0x113a0130 0x80 */
        hi_isp_input_port_freeze_config_write(IspDev, HI_ISP_INPUT_PORT_FREEZE_CONFIG_HOLD_PREVIOUS_INPUT_PORT_CONFIG_STATE);
        hi_isp_input_port_mode_request_write(IspDev, HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_STOP);

        /* himm 0x113a0100 0x10020f */
        hi_isp_input_port_signal_flag0_write(IspDev, 0x10020f);

        /* himm 0x113a0104 0x2020200 */
        hi_isp_input_port_signal_flag1_write(IspDev, 0x2020200);

        /* himm 0x113a0120 0x0; himm 0x113a0124 (height) */
        hi_isp_input_port_vc_start_write(IspDev, 0);
        hi_isp_input_port_vc_size_write(IspDev, pstPubAttr->stWndRect.u32Height);

        /* himm 0x113a0130 0x4 */
        hi_isp_input_port_freeze_config_write(IspDev, 0);
        hi_isp_input_port_mode_request_write(IspDev, 4);

        /* himm 0x113a0130 0x1 */
        hi_isp_input_port_freeze_config_write(IspDev, 0);
        hi_isp_input_port_mode_request_write(IspDev, 1);

        /* himm 0x113a0010 (width); himm 0x113a0014 (height) */
        hi_isp_top_active_width_write(IspDev, pstPubAttr->stWndRect.u32Width);
        hi_isp_top_active_height_write(IspDev, pstPubAttr->stWndRect.u32Height);

        /* himm 0x113a0130 0x1 */
        hi_isp_input_port_freeze_config_write(IspDev, 0);
        hi_isp_input_port_mode_request_write(IspDev, 1);

        /* (set hblank to 64) avoid bad lines in the bottom when ACM on. */
        hi_isp_top_delay_line_regen_hblank_write(IspDev, 0x80);
		hi_isp_top_delay_line_ispbe_hblank_write(IspDev, 0x100);

        hi_isp_top_rggb_start_write(IspDev, (HI_U8)pstPubAttr->enBayer);

        pValue = (HI_VOID *)(&pstPubAttr->f32FrameRate);
        hi_ext_system_fps_base_write(IspDev, *(HI_U32 *)pValue);

        hi_isp_fe_input_port_crop_enable_write(IspDev, 1);
        hi_isp_fe_input_port_crop_x_start_write(IspDev, pstPubAttr->stWndRect.s32X);
        hi_isp_fe_input_port_crop_width_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
        hi_isp_fe_input_port_crop_y_start_write(IspDev, pstPubAttr->stWndRect.s32Y);
        hi_isp_fe_input_port_crop_height_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);
    }

    /* DCI and ACM */
    hi_vi_dci_hor_start_write(IspDev, 0);
    hi_vi_dci_ver_start_write(IspDev, 0);
    hi_vi_dci_hor_end_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
    hi_vi_dci_ver_end_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);
    hi_vi_dci_hor_area_start_write(IspDev, 0);
    hi_vi_dci_ver_area_start_write(IspDev, 0);
    hi_vi_dci_hor_area_end_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
    hi_vi_dci_ver_area_end_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);
    hi_vi_dci_image_width_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
    hi_vi_dci_image_height_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);

    hi_vi_acm_write_width(IspDev, pstPubAttr->stWndRect.u32Width - 1);
    hi_vi_acm_write_height(IspDev, pstPubAttr->stWndRect.u32Height - 1);

    hi_vi_af_hsize_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
    hi_vi_af_vsize_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);

    /* YUV sharpen */
    hi_isp_sharpen_width_write(IspDev, pstPubAttr->stWndRect.u32Width - 1);
    hi_isp_sharpen_height_write(IspDev, pstPubAttr->stWndRect.u32Height - 1);

    pstIspCtx->stIspParaRec.bPubCfg = HI_TRUE;
    hi_ext_top_pub_attr_cfg_write(IspDev, pstIspCtx->stIspParaRec.bPubCfg);

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

    u8Bayer = hi_isp_top_rggb_start_read(IspDev);
    if (u8Bayer >= BAYER_BUTT)
    {
        pstPubAttr->enBayer = BAYER_BUTT;
    }
    else
    {
        pstPubAttr->enBayer = u8Bayer;
    }

    u32Value = hi_ext_system_fps_base_read(IspDev);
    pValue = (HI_VOID *)&u32Value;
    pstPubAttr->f32FrameRate = *(HI_FLOAT *)pValue;

    pstPubAttr->stSnsSize.u32Width = hi_ext_top_sensor_width_read(IspDev);
    pstPubAttr->stSnsSize.u32Height = hi_ext_top_sensor_height_read(IspDev);

    pstPubAttr->stWndRect.s32X      = hi_isp_fe_input_port_crop_x_start_read(IspDev);
    pstPubAttr->stWndRect.u32Width  = hi_isp_fe_input_port_crop_width_read(IspDev) + 1;
    pstPubAttr->stWndRect.s32Y      = hi_isp_fe_input_port_crop_y_start_read(IspDev);
    pstPubAttr->stWndRect.u32Height = hi_isp_fe_input_port_crop_height_read(IspDev) + 1;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetISPPipeDiffAttr(ISP_DEV IspDev, ISP_PIPE_DIFF_ATTR_S *pstDiffAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDiffAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for(i = 0; i < 4; i++)
    {
        if ((pstDiffAttr->as32Offset[i] > 0xFFFFF) || (pstDiffAttr->as32Offset[i] < -0xFFFFF))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Offset :%x!\n", pstDiffAttr->as32Offset[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((pstDiffAttr->au32Gain[i] < 0x80) || (pstDiffAttr->au32Gain[i] > 0x400))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Gain :%x!\n", pstDiffAttr->au32Gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for(i = 0; i < 4; i++)
    {
        hi_ext_system_isp_pipe_diff_offset_write(IspDev, i, pstDiffAttr->as32Offset[i]);
        hi_ext_system_isp_pipe_diff_gain_write(IspDev, i, pstDiffAttr->au32Gain[i]);
    }

    for(i = 0; i < 9; i++)
    {
        hi_ext_system_isp_pipe_diff_ccm_write(IspDev, i, pstDiffAttr->au16ColorMatrix[i]);
    }

    hi_ext_system_black_level_change_write(IspDev, (HI_U8)HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetISPPipeDiffAttr(ISP_DEV IspDev, ISP_PIPE_DIFF_ATTR_S *pstDiffAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDiffAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for(i = 0; i < 4; i++)
    {
        pstDiffAttr->as32Offset[i] = hi_ext_system_isp_pipe_diff_offset_read(IspDev, i);
        pstDiffAttr->au32Gain[i] = hi_ext_system_isp_pipe_diff_gain_read(IspDev, i);
    }

    for(i = 0; i < 9; i++)
    {
        pstDiffAttr->au16ColorMatrix[i] = hi_ext_system_isp_pipe_diff_ccm_read(IspDev, i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetHvSync(ISP_DEV IspDev, const ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSnsSync);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    hi_isp_slave_mode_configs_write(IspDev, pstSnsSync->unCfg.u32Bytes);
    hi_isp_slave_mode_vstime_write(IspDev, pstSnsSync->u32VsTime);
    hi_isp_slave_mode_hstime_write(IspDev, pstSnsSync->u32HsTime);
    hi_isp_slave_mode_vscyc_write(IspDev, pstSnsSync->u32VsCyc);
    hi_isp_slave_mode_hscyc_write(IspDev, pstSnsSync->u32HsCyc);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetHvSync(ISP_DEV IspDev, ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSnsSync);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstSnsSync->unCfg.u32Bytes = hi_isp_slave_mode_configs_read(IspDev);
    pstSnsSync->u32VsTime = hi_isp_slave_mode_vstime_read(IspDev);
    pstSnsSync->u32HsTime = hi_isp_slave_mode_hstime_read(IspDev);
    pstSnsSync->u32VsCyc = hi_isp_slave_mode_vscyc_read(IspDev);
    pstSnsSync->u32HsCyc = hi_isp_slave_mode_hscyc_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFMWState(ISP_DEV IspDev, const ISP_FMW_STATE_E enState)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (enState >= ISP_FMW_STATE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid firmware state %d!\n", enState);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_freeze_firmware_write(IspDev, (HI_U8)enState);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFMWState(ISP_DEV IspDev, ISP_FMW_STATE_E *penState)
{
    HI_U8 u8FMWState;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(penState);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8FMWState = hi_ext_system_freeze_firmware_read(IspDev);

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
        ISP_TRACE(HI_DBG_ERR, "Invalid WDR mode %d!\n", pstWDRMode->enWDRMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_wdr_switch_write(IspDev, HI_FALSE);

    pstIspCtx->stIspParaRec.bWDRCfg = HI_TRUE;
    hi_ext_top_wdr_cfg_write(IspDev, pstIspCtx->stIspParaRec.bWDRCfg);

    if ((HI_U8)pstWDRMode->enWDRMode == hi_ext_system_sensor_wdr_mode_read(IspDev))
    {
        hi_ext_top_wdr_switch_write(IspDev, HI_TRUE);
        return HI_SUCCESS;
    }

    hi_ext_system_sensor_wdr_mode_write(IspDev, (HI_U8)pstWDRMode->enWDRMode);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetWDRMode(ISP_DEV IspDev, ISP_WDR_MODE_S *pstWDRMode)
{
    HI_U8 u8WDRmode;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8WDRmode = hi_ext_system_sensor_wdr_mode_read(IspDev);
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

    u32Key &= 0x3F3FFFFF;
    u32Key |= 0xC0C00000;

    hi_isp_top_bypass_flag_write(IspDev, u32Key);

    u32Key = (punModCtrl->u32Key >> 30) & 0x3;
    hi_isp_top_channel1_select_write(IspDev, u32Key);
    switch (u32Key)
    {
        default:
        case 0:
            hi_isp_top_channel2_select_write(IspDev, 1);
            hi_isp_top_channel3_select_write(IspDev, 2);
            hi_isp_top_channel4_select_write(IspDev, 3);
            break;
        case 1:
            hi_isp_top_channel2_select_write(IspDev, 0);
            hi_isp_top_channel3_select_write(IspDev, 2);
            hi_isp_top_channel4_select_write(IspDev, 3);
            break;
        case 2:
            hi_isp_top_channel2_select_write(IspDev, 1);
            hi_isp_top_channel3_select_write(IspDev, 0);
            hi_isp_top_channel4_select_write(IspDev, 3);
            break;
        case 3:
            hi_isp_top_channel2_select_write(IspDev, 2);
            hi_isp_top_channel3_select_write(IspDev, 1);
            hi_isp_top_channel4_select_write(IspDev, 0);
            break;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetModuleControl(ISP_DEV IspDev, ISP_MODULE_CTRL_U *punModCtrl)
{
    HI_U32 u32Key = 0;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    punModCtrl->u32Key = hi_isp_top_bypass_flag_read(IspDev);
    u32Key = hi_isp_top_channel1_select_read(IspDev);
    punModCtrl->u32Key = (((u32Key & 0x3) << 30) | (punModCtrl->u32Key & 0x3FFFFFFF));

    return HI_SUCCESS;
}

/* General Function Settings */
HI_S32 HI_MPI_ISP_SetDRCAttr(ISP_DEV IspDev, const ISP_DRC_ATTR_S *pstDRC)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_BOOL(pstDRC->bEnable);
    ISP_CHECK_BOOL(pstDRC->bUserTmEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstDRC->u32WhiteLevel > 0xFFFFF)
    {
        printf("Invalid u32WhiteLevel Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32BlackLevel > 0xFFFFF)
    {
        printf("Invalid u32BlackLevel Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32Asymmetry > 0xFF)
    {
        printf("Invalid u32Asymmetry Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u32BrightEnhance > 0xFF)
    {
        printf("Invalid u32BrightEnhance Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < DRC_TM_NODE_NUM; i++)
    {
        if (pstDRC->au32ToneMappingValue[i] > 0xFFFFF)
        {
            printf("Invalid au32ToneMappingValue[%d] Parameter Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    if (pstDRC->FilterMux >= DRC_CURVE_TYPE_BUTT)
    {
        printf("Invalid FilterMux Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->stDrcCurve.u32SlopeMax > 0xFF)
    {
        printf("Invalid u32SlopeMax Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stDrcCurve.u32SlopeMin > 0xFF)
    {
        printf("Invalid u32SlopeMin Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stDrcCurve.u32VarianceSpace > 0xF)
    {
        printf("Invalid u32VarianceSpace Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stDrcCurve.u32VarianceIntensity > 0xF)
    {
        printf("Invalid u32VarianceIntensity Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstDRC->stDrcCurveEx.u32Svariance > 0xF)
    {
        printf("Invalid u32Svariance Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstDRC->stDrcCurveEx.u32BrightPr > 0xFF)
    {
        printf("Invalid u32BrightPr Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstDRC->stDrcCurveEx.u32Contrast > 0xFF)
    {
        printf("Invalid u32Contrast Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstDRC->stDrcCurveEx.u32DarkEnhance > 0xFFFF)
    {
        printf("Invalid u32DarkEnhance Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstDRC->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid DRC Op mode:%d!\n", pstDRC->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stAuto.u32Strength > 0x3FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid stAuto.u32Strength %d!\n", pstDRC->stAuto.u32Strength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->stManual.u32Strength > 0x3FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid stManual.u32Strength:%d!\n", pstDRC->stManual.u32Strength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_drc_enable_write(IspDev, pstDRC->bEnable);
    hi_isp_drc_black_level_write(IspDev, pstDRC->u32BlackLevel);
    hi_isp_drc_white_level_write(IspDev, pstDRC->u32WhiteLevel);

	hi_ext_system_drc_user_tm_enable_write(IspDev, pstDRC->bUserTmEnable);

    hi_ext_system_drc_asymmetry_write(IspDev, pstDRC->u32Asymmetry);
    hi_ext_system_drc_bright_enhance_write(IspDev, pstDRC->u32BrightEnhance);

	for (i = 0; i < DRC_TM_NODE_NUM; i++)
	{
		hi_ext_system_drc_tm_lut_write(IspDev, i, pstDRC->au32ToneMappingValue[i]);
	}
    hi_ext_system_drc_user_tm_update_write(IspDev, HI_TRUE);

    hi_isp_drc_slope_max_write(IspDev, pstDRC->stDrcCurve.u32SlopeMax);
    hi_isp_drc_slope_min_write(IspDev, pstDRC->stDrcCurve.u32SlopeMin);
    hi_isp_drc_variance_space_write(IspDev, pstDRC->stDrcCurve.u32VarianceSpace);
    hi_isp_drc_variance_intensity_write(IspDev, pstDRC->stDrcCurve.u32VarianceIntensity);

    hi_isp_drc_filter_mux_write(IspDev, pstDRC->FilterMux);
    hi_isp_drc_svariance_write(IspDev, pstDRC->stDrcCurveEx.u32Svariance);
    hi_isp_drc_bright_pr_write(IspDev, pstDRC->stDrcCurveEx.u32BrightPr);
    hi_isp_drc_contrast_write(IspDev, pstDRC->stDrcCurveEx.u32Contrast);
    hi_isp_drc_dark_enh_write(IspDev, pstDRC->stDrcCurveEx.u32DarkEnhance);

    hi_ext_system_manual_drc_write(IspDev, pstDRC->enOpType);
    hi_ext_system_drc_manual_strength_write(IspDev, pstDRC->stManual.u32Strength);
    hi_ext_system_drc_auto_strength_write(IspDev, pstDRC->stAuto.u32Strength);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDRCAttr(ISP_DEV IspDev, ISP_DRC_ATTR_S *pstDRC)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDRC->bEnable = hi_isp_drc_enable_read(IspDev);
    pstDRC->u32BlackLevel = hi_isp_drc_black_level_read(IspDev);
    pstDRC->u32WhiteLevel = hi_isp_drc_white_level_read(IspDev);
    pstDRC->bUserTmEnable = hi_ext_system_drc_user_tm_enable_read(IspDev);
    pstDRC->u32Asymmetry = hi_ext_system_drc_asymmetry_read(IspDev);
    pstDRC->u32BrightEnhance = hi_ext_system_drc_bright_enhance_read(IspDev);

	for (i = 0; i < DRC_TM_NODE_NUM; i++)
	{
		pstDRC->au32ToneMappingValue[i] = hi_ext_system_drc_tm_lut_read(IspDev, i);
	}

    pstDRC->FilterMux = hi_isp_drc_filter_mux_read(IspDev);

    pstDRC->stDrcCurve.u32SlopeMax = hi_isp_drc_slope_max_read(IspDev);
    pstDRC->stDrcCurve.u32SlopeMin = hi_isp_drc_slope_min_read(IspDev);
    pstDRC->stDrcCurve.u32VarianceSpace = hi_isp_drc_variance_space_read(IspDev);
    pstDRC->stDrcCurve.u32VarianceIntensity = hi_isp_drc_variance_intensity_read(IspDev);

    pstDRC->stDrcCurveEx.u32Svariance = hi_isp_drc_svariance_read(IspDev);
    pstDRC->stDrcCurveEx.u32BrightPr = hi_isp_drc_bright_pr_read(IspDev);
    pstDRC->stDrcCurveEx.u32Contrast = hi_isp_drc_contrast_read(IspDev);
    pstDRC->stDrcCurveEx.u32DarkEnhance = hi_isp_drc_dark_enh_read(IspDev);

    pstDRC->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_drc_read(IspDev);
    pstDRC->stAuto.u32Strength = hi_ext_system_drc_auto_strength_read(IspDev);
    pstDRC->stManual.u32Strength = hi_ext_system_drc_manual_strength_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFSWDRAttr(ISP_DEV IspDev, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_BOOL(pstFSWDRAttr->bLongMotionComp);
    ISP_CHECK_BOOL(pstFSWDRAttr->bMedMotionComp);
    ISP_CHECK_BOOL(pstFSWDRAttr->bShortMotionComp);

    if (pstFSWDRAttr->u16LongShortThresh > 0xFFF)
    {
        printf("Invalid u16LongShortThresh %d!\n", pstFSWDRAttr->u16LongShortThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16LongLongThresh > 0xFFF)
    {
        printf("Invalid u16LongLongThresh %d!\n", pstFSWDRAttr->u16LongLongThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16LongLongThresh > pstFSWDRAttr->u16LongShortThresh)
    {
        printf("u16LongLongThresh should NOT be larger than u16LongShortThresh!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->enLongComMode >= FS_WDR_COMBINE_BUTT)
    {
        printf("Invalid enLongComMode %d!\n", pstFSWDRAttr->enLongComMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->u16MedShortThresh > 0xFFF)
    {
        printf("Invalid u16MedShortThresh %d!\n", pstFSWDRAttr->u16MedShortThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16MedLongThresh > 0xFFF)
    {
        printf("Invalid u16MedLongThresh %d!\n", pstFSWDRAttr->u16MedLongThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16MedLongThresh > pstFSWDRAttr->u16MedShortThresh)
    {
        printf("u16MedLongThresh should NOT be larger than u16MedShortThresh!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->enMedComMode >= FS_WDR_COMBINE_BUTT)
    {
        printf("Invalid enMedComMode %d!\n", pstFSWDRAttr->enMedComMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstFSWDRAttr->u16ShortShortThresh > 0xFFF)
    {
        printf("Invalid u16ShortShortThresh %d!\n", pstFSWDRAttr->u16ShortShortThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16ShortLongThresh > 0xFFF)
    {
        printf("Invalid u16ShortLongThresh %d!\n", pstFSWDRAttr->u16ShortLongThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16ShortLongThresh > pstFSWDRAttr->u16ShortShortThresh)
    {
        printf("u16ShortLongThresh should NOT be larger than u16ShortShortThresh!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->enShortComMode >= FS_WDR_COMBINE_BUTT)
    {
        printf("Invalid enShortComMode %d!\n", pstFSWDRAttr->enShortComMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_frame_stitch_long_use_stitching_error_write(IspDev, pstFSWDRAttr->bLongMotionComp);
    hi_isp_frame_stitch_long_short_thresh_write(IspDev, pstFSWDRAttr->u16LongShortThresh);
    hi_isp_frame_stitch_long_long_thresh_write(IspDev, pstFSWDRAttr->u16LongLongThresh);
    hi_isp_wdr_use_long_override_write(IspDev, pstFSWDRAttr->enLongComMode);

    hi_isp_frame_stitch_med_use_stitching_error_write(IspDev, pstFSWDRAttr->bMedMotionComp);
    hi_isp_frame_stitch_med_short_thresh_write(IspDev, pstFSWDRAttr->u16MedShortThresh);
    hi_isp_frame_stitch_med_long_thresh_write(IspDev, pstFSWDRAttr->u16MedLongThresh);
    hi_isp_frame_stitch_med_use_long_override_write(IspDev, pstFSWDRAttr->enMedComMode);

    hi_isp_frame_stitch_short_use_stitching_error_write(IspDev, pstFSWDRAttr->bShortMotionComp);
    hi_isp_frame_stitch_short_short_thresh_write(IspDev, pstFSWDRAttr->u16ShortShortThresh);
    hi_isp_frame_stitch_short_long_thresh_write(IspDev, pstFSWDRAttr->u16ShortLongThresh);
    hi_isp_frame_stitch_short_use_long_override_write(IspDev, pstFSWDRAttr->enShortComMode);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFSWDRAttr(ISP_DEV IspDev, ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstFSWDRAttr->bLongMotionComp = hi_isp_frame_stitch_long_use_stitching_error_read(IspDev);
    pstFSWDRAttr->u16LongShortThresh = hi_isp_frame_stitch_long_short_thresh_read(IspDev);
    pstFSWDRAttr->u16LongLongThresh = hi_isp_frame_stitch_long_long_thresh_read(IspDev);
    pstFSWDRAttr->enLongComMode = hi_isp_wdr_use_long_override_read(IspDev);
    pstFSWDRAttr->bMedMotionComp = hi_isp_frame_stitch_med_use_stitching_error_read(IspDev);
    pstFSWDRAttr->u16MedShortThresh = hi_isp_frame_stitch_med_short_thresh_read(IspDev);
    pstFSWDRAttr->u16MedLongThresh = hi_isp_frame_stitch_med_long_thresh_read(IspDev);
    pstFSWDRAttr->enMedComMode = hi_isp_frame_stitch_med_use_long_override_read(IspDev);
    pstFSWDRAttr->bShortMotionComp = hi_isp_frame_stitch_short_use_stitching_error_read(IspDev);
    pstFSWDRAttr->u16ShortShortThresh = hi_isp_frame_stitch_short_short_thresh_read(IspDev);
    pstFSWDRAttr->u16ShortLongThresh = hi_isp_frame_stitch_short_long_thresh_read(IspDev);
    pstFSWDRAttr->enShortComMode = hi_isp_frame_stitch_short_use_long_override_read(IspDev);

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetInputFormatterAttr(ISP_DEV IspDev, const ISP_INPUT_FORMATTER_ATTR_S *pstIFAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIFAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstIFAttr->au16KneePoint[1] < pstIFAttr->au16KneePoint[0])
    {
        printf("au16KneePoint[1] should not less than au16KneePoint[0]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstIFAttr->au16KneePoint[2] < pstIFAttr->au16KneePoint[1])
    {
        printf("au16KneePoint[2] should not less than au16KneePoint[1]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for(i = 0; i < 4; i++)
    {
        if (pstIFAttr->au8SlopeSelect[i] > 0xF)
        {
            printf("au8SlopeSelect[%d] should not larger than 0xF!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_isp_input_formatter_knee_point0_write(IspDev, pstIFAttr->au16KneePoint[0]);
    hi_isp_input_formatter_knee_point1_write(IspDev, pstIFAttr->au16KneePoint[1]);
    hi_isp_input_formatter_knee_point2_write(IspDev, pstIFAttr->au16KneePoint[2]);
    hi_isp_input_formatter_slope0_select_write(IspDev, pstIFAttr->au8SlopeSelect[0]);
    hi_isp_input_formatter_slope1_select_write(IspDev, pstIFAttr->au8SlopeSelect[1]);
    hi_isp_input_formatter_slope2_select_write(IspDev, pstIFAttr->au8SlopeSelect[2]);
    hi_isp_input_formatter_slope3_select_write(IspDev, pstIFAttr->au8SlopeSelect[3]);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetInputFormatterAttr(ISP_DEV IspDev, ISP_INPUT_FORMATTER_ATTR_S *pstIFAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIFAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstIFAttr->au16KneePoint[0] = hi_isp_input_formatter_knee_point0_read(IspDev);
    pstIFAttr->au16KneePoint[1] = hi_isp_input_formatter_knee_point1_read(IspDev);
    pstIFAttr->au16KneePoint[2] = hi_isp_input_formatter_knee_point2_read(IspDev);
    pstIFAttr->au8SlopeSelect[0] = hi_isp_input_formatter_slope0_select_read(IspDev);
    pstIFAttr->au8SlopeSelect[1] = hi_isp_input_formatter_slope1_select_read(IspDev);
    pstIFAttr->au8SlopeSelect[2] = hi_isp_input_formatter_slope2_select_read(IspDev);
    pstIFAttr->au8SlopeSelect[3] = hi_isp_input_formatter_slope3_select_read(IspDev);

    return HI_SUCCESS;
}



HI_S32 HI_MPI_ISP_SetDPCalibrate(ISP_DEV IspDev, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnable);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnableDetect);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if ((pstDPCalibrate->u16CountMax > 0xFFF) || (pstDPCalibrate->u16CountMin > 0xFFF))
    {
        printf("Invalid u16CountMax Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16CountMin > pstDPCalibrate->u16CountMax)
    {
        printf("Invalid u16CountMin Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16TimeLimit > STATIC_DP_CAL_TIME_OUT)
    {
        printf("Invalid u16TimeLimit Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstDPCalibrate->enStaticDPType >= ISP_STATIC_DP_BUTT)
    {
        printf("Invalid  enStaticDPType Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstDPCalibrate->enStatus >= ISP_STATE_BUTT)
    {
        printf("Invalid  enStatus Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(0 == pstDPCalibrate->u8StartThresh)
    {
        printf("u8StartThresh  should be bigger than 0!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_defect_pixel_enable_write(IspDev, pstDPCalibrate->bEnable);
    hi_ext_system_calibrate_bad_pixels_write(IspDev, pstDPCalibrate->bEnableDetect);

    hi_ext_system_bad_pixel_detect_type_write(IspDev, pstDPCalibrate->enStaticDPType);
    hi_ext_system_bad_pixel_start_thresh_write(IspDev, pstDPCalibrate->u8StartThresh);
    hi_ext_system_bad_pixel_count_max_write(IspDev, pstDPCalibrate->u16CountMax);
    hi_ext_system_bad_pixel_count_min_write(IspDev, pstDPCalibrate->u16CountMin);
    hi_ext_system_bad_pixel_trigger_time_write(IspDev, pstDPCalibrate->u16TimeLimit);
    hi_ext_system_bad_pixel_trigger_status_write(IspDev, pstDPCalibrate->enStatus);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDPCalibrate(ISP_DEV IspDev, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    HI_U16 i;
    HI_U16 u16TableAddr;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDPCalibrate->bEnable = hi_isp_defect_pixel_enable_read(IspDev);
    pstDPCalibrate->bEnableDetect = hi_ext_system_calibrate_bad_pixels_read(IspDev);

    pstDPCalibrate->enStaticDPType = hi_ext_system_bad_pixel_detect_type_read(IspDev);
    pstDPCalibrate->u8StartThresh = hi_ext_system_bad_pixel_start_thresh_read(IspDev);
    pstDPCalibrate->u16CountMax = hi_ext_system_bad_pixel_count_max_read(IspDev);
    pstDPCalibrate->u16CountMin = hi_ext_system_bad_pixel_count_min_read(IspDev);
    pstDPCalibrate->u16TimeLimit = hi_ext_system_bad_pixel_trigger_time_read(IspDev);

    pstDPCalibrate->u8FinishThresh = hi_ext_system_bad_pixel_finish_thresh_read(IspDev);
    pstDPCalibrate->u16Count = hi_isp_defect_pixel_defect_pixel_count_read(IspDev);
    pstDPCalibrate->enStatus = hi_ext_system_bad_pixel_trigger_status_read(IspDev);

    u16TableAddr = hi_isp_defect_pixel_table_start_read(IspDev);

    for(i=0; i<STATIC_DP_COUNT_MAX; i++)
    {
        if(i < pstDPCalibrate->u16Count)
        {
            pstDPCalibrate->u32Table[i] = hi_isp_defect_pixel_table_lut_read(IspDev, (i+u16TableAddr)&0xFFF, 2);
        }
        else
        {
            pstDPCalibrate->u32Table[i] = 0;
        }
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetDPAttr(ISP_DEV IspDev, const ISP_DP_ATTR_S *pstDPAttr)
{
    HI_U16 i = 0, j = 0, m = 0, u16CountIn = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    HI_U32 *pu32DefectPixelTable = HI_NULL;
    ISP_CHECK_BOOL(pstDPAttr->stDynamicAttr.bEnable);
    ISP_CHECK_BOOL(pstDPAttr->stStaticAttr.bEnable);
    ISP_CHECK_BOOL(pstDPAttr->stStaticAttr.bShow);

    if(pstDPAttr->stDynamicAttr.u16Slope > 0xFFF)
    {
        printf("DynamicAttr u16Slope should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u16Thresh > 0xFFF)
    {
        printf("DynamicAttr u16Thresh should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u16LineThresh > 0xFFF)
    {
        printf("DynamicAttr u16LineThresh should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u16HpThresh > 0xFFF)
    {
        printf("DynamicAttr u16HpThresh should be less  than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stDynamicAttr.u8BlendRatio > 0xFF)
    {
        printf("DynamicAttr u8BlendRatio should be less  than 0xFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stStaticAttr.u16BrightCount > STATIC_DP_COUNT_MAX)
    {
        printf("StaticAttr u16BrightCount should be less  than STATIC_DP_COUNT_MAX!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPAttr->stStaticAttr.u16DarkCount > STATIC_DP_COUNT_MAX)
    {
        printf("StaticAttr u16DarkCount should be less  than STATIC_DP_COUNT_MAX!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for ( i = 0 ; i < STATIC_DP_COUNT_MAX ; i++ )
    {
        if (pstDPAttr->stStaticAttr.au32BrightTable[i] > 0x3FFFFFF || pstDPAttr->stStaticAttr.au32DarkTable[i] > 0x3FFFFFF)
        {
            printf("au32DarkTable and au32BrightTable should be less than 0x3FFFFFF!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (HI_TRUE == hi_ext_system_calibrate_bad_pixels_read(IspDev))
    {
        printf("Static DP is calibrating, DP attr is not config!\n");
        return HI_ERR_ISP_ATTR_NOT_CFG;
    }

    pu32DefectPixelTable = (HI_U32 *)malloc(sizeof(HI_U32) * STATIC_DP_COUNT_MAX);
    if (HI_NULL == pu32DefectPixelTable)
    {
        printf("Malloc defectPixelTable Memory failure!\n");
        return HI_ERR_ISP_NOMEM;
    }

    /* merging dark talbe and bright table */
    i = 0;
    while ((i <  pstDPAttr->stStaticAttr.u16BrightCount) && (j < pstDPAttr->stStaticAttr.u16DarkCount))
    {
        if (m >= STATIC_DP_COUNT_MAX)
        {
            printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
            s32Ret = HI_ERR_ISP_ILLEGAL_PARAM;
            goto EXIT_SET_DP_ATTR;
        }
        if (pstDPAttr->stStaticAttr.au32BrightTable[i] > pstDPAttr->stStaticAttr.au32DarkTable[j])
        {
            pu32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32DarkTable[j++];
        }
        else if (pstDPAttr->stStaticAttr.au32BrightTable[i] < pstDPAttr->stStaticAttr.au32DarkTable[j])
        {
            pu32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32BrightTable[i++];
        }
        else
        {
            pu32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32BrightTable[i];
            i++;
            j++;
        }
    }

    if (i >=  pstDPAttr->stStaticAttr.u16BrightCount)
    {
        while (j < pstDPAttr->stStaticAttr.u16DarkCount)
        {
            if (m >= STATIC_DP_COUNT_MAX)
            {
                printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
                s32Ret = HI_ERR_ISP_ILLEGAL_PARAM;
                goto EXIT_SET_DP_ATTR;
            }
            pu32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32DarkTable[j++];
        }
    }
    if (j >=  pstDPAttr->stStaticAttr.u16DarkCount)
    {
        while (i < pstDPAttr->stStaticAttr.u16BrightCount)
        {
            if (m >= STATIC_DP_COUNT_MAX)
            {
                printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
                s32Ret = HI_ERR_ISP_ILLEGAL_PARAM;
                goto EXIT_SET_DP_ATTR;
            }
            pu32DefectPixelTable[m++] = pstDPAttr->stStaticAttr.au32BrightTable[i++];
        }
    }

    u16CountIn = m;

    hi_isp_green_equalize_dp_enable_write(IspDev, pstDPAttr->stDynamicAttr.bEnable);
    hi_ext_system_dynamic_defect_pixel_slope_write(IspDev, pstDPAttr->stDynamicAttr.u16Slope);
    hi_ext_system_dynamic_defect_pixel_thresh_write(IspDev, pstDPAttr->stDynamicAttr.u16Thresh);

    hi_isp_raw_frontend_line_thresh_write(IspDev, pstDPAttr->stDynamicAttr.u16LineThresh);
    hi_isp_raw_frontend_hpdev_threshold_write(IspDev, pstDPAttr->stDynamicAttr.u16HpThresh);
    hi_isp_raw_frontend_hp_blend_write(IspDev, pstDPAttr->stDynamicAttr.u8BlendRatio);

    hi_isp_defect_pixel_enable_write(IspDev, pstDPAttr->stStaticAttr.bEnable);
    hi_isp_defect_pixel_show_static_defect_pixels_write(IspDev, pstDPAttr->stStaticAttr.bShow);

    for(i = 0; i < u16CountIn; i++)
    {
        hi_isp_defect_pixel_table_lut_write(IspDev, i, pu32DefectPixelTable[i]);
    }

    hi_isp_defect_pixel_defect_pixel_count_in_write(IspDev, u16CountIn);
    hi_isp_defect_pixel_pointer_reset_write(IspDev, 1);
    hi_isp_defect_pixel_pointer_reset_write(IspDev, 0);


EXIT_SET_DP_ATTR:

    free(pu32DefectPixelTable);

    return s32Ret;
}

HI_S32 HI_MPI_ISP_GetDPAttr(ISP_DEV IspDev, ISP_DP_ATTR_S *pstDPAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDPAttr->stDynamicAttr.bEnable = hi_isp_green_equalize_dp_enable_read(IspDev);
    pstDPAttr->stDynamicAttr.u16Slope = hi_ext_system_dynamic_defect_pixel_slope_read(IspDev);
    pstDPAttr->stDynamicAttr.u16Thresh = hi_ext_system_dynamic_defect_pixel_thresh_read(IspDev);
    pstDPAttr->stDynamicAttr.u16LineThresh = hi_isp_raw_frontend_line_thresh_read(IspDev);
    pstDPAttr->stDynamicAttr.u16HpThresh = hi_isp_raw_frontend_hpdev_threshold_read(IspDev);
    pstDPAttr->stDynamicAttr.u8BlendRatio = hi_isp_raw_frontend_hp_blend_read(IspDev);

    pstDPAttr->stStaticAttr.bEnable = hi_isp_defect_pixel_enable_read(IspDev);
    pstDPAttr->stStaticAttr.bShow = hi_isp_defect_pixel_show_static_defect_pixels_read(IspDev);
    pstDPAttr->stStaticAttr.u16BrightCount = hi_isp_defect_pixel_defect_pixel_count_in_read(IspDev);
    pstDPAttr->stStaticAttr.u16DarkCount = 0;


    for(i = 0; i < STATIC_DP_COUNT_MAX; i++)
    {
        if(i < pstDPAttr->stStaticAttr.u16BrightCount)
        {
            pstDPAttr->stStaticAttr.au32BrightTable[i] = (hi_isp_defect_pixel_table_lut_read(IspDev, i, 2) & 0x3FFFFFF);
        }
        else
        {
            pstDPAttr->stStaticAttr.au32BrightTable[i] = 0;
        }
    }

    memset(pstDPAttr->stStaticAttr.au32DarkTable, 0, sizeof(pstDPAttr->stStaticAttr.au32DarkTable));

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
    return VI_SetDISAttr(IspDev, &stDisAttr);
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

    s32Ret = VI_GetDISAttr(IspDev, &stDisAttr);
    pstDISAttr->bEnable = stDisAttr.bEnable;
    return s32Ret;
}

HI_S32 HI_MPI_ISP_SetRadialShadingAttr(ISP_DEV IspDev, const ISP_RADIAL_SHADING_ATTR_S *pstRadialShadingAttr)
{
    HI_U32 i = 0, j = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRadialShadingAttr);
    ISP_CHECK_BOOL(pstRadialShadingAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstRadialShadingAttr->u16RadialNodeNum > SHADING_NODE_NUM_MAX)
    {
        printf("Invalid Shading Table Node Number %d!\n", pstRadialShadingAttr->u16RadialNodeNum);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (j=0; j<3; j++)
    {
        if ((pstRadialShadingAttr->astCenter[j].s32X < 0) || (pstRadialShadingAttr->astCenter[j].s32X > 0xFFFF))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Shading[%d] s32X %d!\n", j, pstRadialShadingAttr->astCenter[j].s32X);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if ((pstRadialShadingAttr->astCenter[j].s32Y < 0) || (pstRadialShadingAttr->astCenter[j].s32Y > 0xFFFF))
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Shading[%d] s32Y %d!\n", j, pstRadialShadingAttr->astCenter[j].s32Y);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    for (i=0; i<pstRadialShadingAttr->u16RadialNodeNum; i++)
    {
        for (j=0; j<3; j++)
        {
            if (pstRadialShadingAttr->au32Table[j][i] > 0xFFFF)
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid Shading[%d] Table[%d] %d!\n", j, i, pstRadialShadingAttr->au32Table[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
    }

    hi_isp_radial_shading_off_center_multr_write(IspDev, pstRadialShadingAttr->au16OffCenter[0]);
    hi_isp_radial_shading_off_center_multg_write(IspDev, pstRadialShadingAttr->au16OffCenter[1]);
    hi_isp_radial_shading_off_center_multb_write(IspDev, pstRadialShadingAttr->au16OffCenter[2]);
    hi_isp_radial_shading_centerr_x_write(IspDev, pstRadialShadingAttr->astCenter[0].s32X);
    hi_isp_radial_shading_centerr_y_write(IspDev, pstRadialShadingAttr->astCenter[0].s32Y);
    hi_isp_radial_shading_centerg_x_write(IspDev, pstRadialShadingAttr->astCenter[1].s32X);
    hi_isp_radial_shading_centerg_y_write(IspDev, pstRadialShadingAttr->astCenter[1].s32Y);
    hi_isp_radial_shading_centerb_x_write(IspDev, pstRadialShadingAttr->astCenter[2].s32X);
    hi_isp_radial_shading_centerb_y_write(IspDev, pstRadialShadingAttr->astCenter[2].s32Y);

    hi_isp_radial_shading_mcu_priority_write(IspDev, 1);
    for (i=0; i<pstRadialShadingAttr->u16RadialNodeNum; i++)
    {
        hi_radial_shading_mem_luts_shading_lutr_write(IspDev, i, pstRadialShadingAttr->au32Table[0][i]);
        hi_radial_shading_mem_luts_shading_lutg_write(IspDev, i, pstRadialShadingAttr->au32Table[1][i]);
        hi_radial_shading_mem_luts_shading_lutb_write(IspDev, i, pstRadialShadingAttr->au32Table[2][i]);
    }
    hi_isp_radial_shading_mcu_priority_write(IspDev, 0);
    hi_ext_system_shading_table_node_number_write(IspDev, pstRadialShadingAttr->u16RadialNodeNum);

    hi_isp_radial_shading_enable_write(IspDev, pstRadialShadingAttr->bEnable);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetRadialShadingAttr(ISP_DEV IspDev, ISP_RADIAL_SHADING_ATTR_S *pstRadialShadingAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRadialShadingAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstRadialShadingAttr->au16OffCenter[0] = hi_isp_radial_shading_off_center_multr_read(IspDev);
    pstRadialShadingAttr->au16OffCenter[1] = hi_isp_radial_shading_off_center_multg_read(IspDev);
    pstRadialShadingAttr->au16OffCenter[2] = hi_isp_radial_shading_off_center_multb_read(IspDev);

    pstRadialShadingAttr->astCenter[0].s32X = hi_isp_radial_shading_centerr_x_read(IspDev);
    pstRadialShadingAttr->astCenter[0].s32Y = hi_isp_radial_shading_centerr_y_read(IspDev);
    pstRadialShadingAttr->astCenter[1].s32X = hi_isp_radial_shading_centerg_x_read(IspDev);
    pstRadialShadingAttr->astCenter[1].s32Y = hi_isp_radial_shading_centerg_y_read(IspDev);
    pstRadialShadingAttr->astCenter[2].s32X = hi_isp_radial_shading_centerb_x_read(IspDev);
    pstRadialShadingAttr->astCenter[2].s32Y = hi_isp_radial_shading_centerb_y_read(IspDev);

    pstRadialShadingAttr->u16RadialNodeNum = hi_ext_system_shading_table_node_number_read(IspDev);

    hi_isp_radial_shading_mcu_priority_write(IspDev, 1);
    for (i=0; i<SHADING_NODE_NUM_MAX; i++)
    {
        pstRadialShadingAttr->au32Table[0][i] = hi_radial_shading_mem_luts_shading_lutr_read(IspDev, i);
        pstRadialShadingAttr->au32Table[1][i] = hi_radial_shading_mem_luts_shading_lutg_read(IspDev, i);
        pstRadialShadingAttr->au32Table[2][i] = hi_radial_shading_mem_luts_shading_lutb_read(IspDev, i);
    }
    hi_isp_radial_shading_mcu_priority_write(IspDev, 0);

    pstRadialShadingAttr->bEnable = hi_isp_radial_shading_enable_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetMeshShadingAttr(ISP_DEV IspDev, const ISP_MESH_SHADING_ATTR_S *pstMeshShadingAttr)
{
    HI_U32 i = 0, j = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstMeshShadingAttr);
    ISP_CHECK_BOOL(pstMeshShadingAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    HI_U32 u32MeshLutData = 0;

    if (pstMeshShadingAttr->enMeshLSMode > 2)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading u8MeshAlphaMode %d!\n", pstMeshShadingAttr->enMeshLSMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstMeshShadingAttr->u8MeshScale > 7)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading u8MeshScale %d!\n", pstMeshShadingAttr->u8MeshScale);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i=0; i<ISP_MESH_LS_NUM_MAX-1; i++)
    {
        if (pstMeshShadingAttr->au32MeshColorTemp[i] > pstMeshShadingAttr->au32MeshColorTemp[i+1])
        {
            ISP_TRACE(HI_DBG_ERR, "au32MeshColorTemp[%d] should not be bigger than au32MeshColorTemp[%d]!\n", i, i+1);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_isp_mesh_shading_mesh_reload_write(IspDev, 0);

    hi_ext_system_mesh_col_temp_h_write(IspDev, pstMeshShadingAttr->au32MeshColorTemp[3]);
    hi_ext_system_mesh_col_temp_m_write(IspDev, pstMeshShadingAttr->au32MeshColorTemp[2]);
    hi_ext_system_mesh_col_temp_l_write(IspDev, pstMeshShadingAttr->au32MeshColorTemp[1]);
    hi_ext_system_mesh_col_temp_ll_write(IspDev, pstMeshShadingAttr->au32MeshColorTemp[0]);

    hi_isp_mesh_shading_mesh_alpha_mode_write(IspDev, (HI_U8)pstMeshShadingAttr->enMeshLSMode);
    hi_isp_mesh_shading_mesh_scale_write(IspDev, pstMeshShadingAttr->u8MeshScale);
    hi_isp_mesh_shading_mesh_strength_write(IspDev, pstMeshShadingAttr->u16MeshStrength);
    switch(pstMeshShadingAttr->enMeshLSMode)
    {
        case ISP_MESH_MODE_TYPE_1LS: /*single light source*/
            for (j=0; j<ISP_MESH_SHADING_PLANE_NUM; j++)
            {
                for (i=0; i<ISP_MESH_SHADING_NODE_NUM; i+=4)
                {
                    u32MeshLutData = pstMeshShadingAttr->au8MeshTable[j][i] + (pstMeshShadingAttr->au8MeshTable[j][i+1] << 8)
                        + (pstMeshShadingAttr->au8MeshTable[j][i+2] << 16) + (pstMeshShadingAttr->au8MeshTable[j][i+3] << 24);
                    if (0 == j) hi_mesh_shading_mem_luts_shading_lutr_write(IspDev, (i>>2), u32MeshLutData);
                    if (1 == j) hi_mesh_shading_mem_luts_shading_lutg_write(IspDev, (i>>2), u32MeshLutData);
                    if (2 == j) hi_mesh_shading_mem_luts_shading_lutb_write(IspDev, (i>>2), u32MeshLutData);
                }
            }
            hi_isp_mesh_shading_mesh_width_write(IspDev, 64-1);
            hi_isp_mesh_shading_mesh_height_write(IspDev, 64-1);
            break;
        case ISP_MESH_MODE_TYPE_2LS: /*two light source*/
            for (j=0; j<ISP_MESH_SHADING_PLANE_NUM; j++)
            {
                for (i=0; i<(ISP_MESH_SHADING_NODE_NUM>>1); i+=2)
                {
                    u32MeshLutData = pstMeshShadingAttr->au8MeshTable[j][i] + (pstMeshShadingAttr->au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>1)] << 8)
                        + (pstMeshShadingAttr->au8MeshTable[j][i+1] << 16) + (pstMeshShadingAttr->au8MeshTable[j][i+1+(ISP_MESH_SHADING_NODE_NUM>>1)] << 24);
                    if (0 == j) hi_mesh_shading_mem_luts_shading_lutr_write(IspDev, (i>>1), u32MeshLutData);
                    if (1 == j) hi_mesh_shading_mem_luts_shading_lutg_write(IspDev, (i>>1), u32MeshLutData);
                    if (2 == j) hi_mesh_shading_mem_luts_shading_lutb_write(IspDev, (i>>1), u32MeshLutData);
                }
            }
            hi_isp_mesh_shading_mesh_width_write(IspDev, 32-1);
            hi_isp_mesh_shading_mesh_height_write(IspDev, 64-1);
            break;
        case ISP_MESH_MODE_TYPE_4LS: /*four light source*/
            for (j=0; j<ISP_MESH_SHADING_PLANE_NUM; j++)
            {
                for (i=0; i<(ISP_MESH_SHADING_NODE_NUM>>2); i++)
                {
                    u32MeshLutData = pstMeshShadingAttr->au8MeshTable[j][i] + (pstMeshShadingAttr->au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>2)] << 8)
                        + (pstMeshShadingAttr->au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>1)] << 16) + (pstMeshShadingAttr->au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>1)+(ISP_MESH_SHADING_NODE_NUM>>2)] << 24);
                    if (0 == j) hi_mesh_shading_mem_luts_shading_lutr_write(IspDev, (i), u32MeshLutData);
                    if (1 == j) hi_mesh_shading_mem_luts_shading_lutg_write(IspDev, (i), u32MeshLutData);
                    if (2 == j) hi_mesh_shading_mem_luts_shading_lutb_write(IspDev, (i), u32MeshLutData);
                }
            }
            hi_isp_mesh_shading_mesh_width_write(IspDev, 32-1);
            hi_isp_mesh_shading_mesh_height_write(IspDev, 32-1);
            break;
        case ISP_MESH_MODE_TYPE_BUTT:
        default:
            break;
    }

    hi_isp_mesh_shading_enable_write(IspDev, pstMeshShadingAttr->bEnable);
    hi_isp_mesh_shading_mesh_reload_write(IspDev, 1);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetMeshShadingAttr(ISP_DEV IspDev, ISP_MESH_SHADING_ATTR_S *pstMeshShadingAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstMeshShadingAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    HI_U32 au32MeshTable[3];

    pstMeshShadingAttr->au32MeshColorTemp[3] = hi_ext_system_mesh_col_temp_h_read(IspDev);
    pstMeshShadingAttr->au32MeshColorTemp[2] = hi_ext_system_mesh_col_temp_m_read(IspDev);
    pstMeshShadingAttr->au32MeshColorTemp[1] = hi_ext_system_mesh_col_temp_l_read(IspDev);
    pstMeshShadingAttr->au32MeshColorTemp[0] = hi_ext_system_mesh_col_temp_ll_read(IspDev);
    pstMeshShadingAttr->bEnable = hi_isp_mesh_shading_enable_read(IspDev);
    pstMeshShadingAttr->enMeshLSMode = hi_isp_mesh_shading_mesh_alpha_mode_read(IspDev);
    pstMeshShadingAttr->u8MeshScale = hi_isp_mesh_shading_mesh_scale_read(IspDev);
    pstMeshShadingAttr->u16MeshStrength = hi_isp_mesh_shading_mesh_strength_read(IspDev);
    switch (pstMeshShadingAttr->enMeshLSMode)
    {
        case ISP_MESH_MODE_TYPE_1LS:
            for (i=0; i<ISP_MESH_SHADING_NODE_NUM; i+=4)
            {
                au32MeshTable[0] = hi_mesh_shading_mem_luts_shading_lutr_read(IspDev, i>>2);
                au32MeshTable[1] = hi_mesh_shading_mem_luts_shading_lutg_read(IspDev, i>>2);
                au32MeshTable[2] = hi_mesh_shading_mem_luts_shading_lutb_read(IspDev, i>>2);
                pstMeshShadingAttr->au8MeshTable[0][i] = ((au32MeshTable[0] & 0xff));
                pstMeshShadingAttr->au8MeshTable[1][i] = ((au32MeshTable[1] & 0xff));
                pstMeshShadingAttr->au8MeshTable[2][i] = ((au32MeshTable[2] & 0xff));
                pstMeshShadingAttr->au8MeshTable[0][i+1] = ((au32MeshTable[0] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[1][i+1] = ((au32MeshTable[1] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[2][i+1] = ((au32MeshTable[2] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[0][i+2] = ((au32MeshTable[0] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[1][i+2] = ((au32MeshTable[1] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[2][i+2] = ((au32MeshTable[2] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[0][i+3] = ((au32MeshTable[0] & 0xff000000) >> 24);
                pstMeshShadingAttr->au8MeshTable[1][i+3] = ((au32MeshTable[1] & 0xff000000) >> 24);
                pstMeshShadingAttr->au8MeshTable[2][i+3] = ((au32MeshTable[2] & 0xff000000) >> 24);
            }
            break;
        case ISP_MESH_MODE_TYPE_2LS:
            for (i=0; i<ISP_MESH_SHADING_NODE_NUM>>1; i+=2)
            {
                au32MeshTable[0] = hi_mesh_shading_mem_luts_shading_lutr_read(IspDev, i>>1);
                au32MeshTable[1] = hi_mesh_shading_mem_luts_shading_lutg_read(IspDev, i>>1);
                au32MeshTable[2] = hi_mesh_shading_mem_luts_shading_lutb_read(IspDev, i>>1);
                pstMeshShadingAttr->au8MeshTable[0][i] = ((au32MeshTable[0] & 0xff));
                pstMeshShadingAttr->au8MeshTable[1][i] = ((au32MeshTable[1] & 0xff));
                pstMeshShadingAttr->au8MeshTable[2][i] = ((au32MeshTable[2] & 0xff));
                pstMeshShadingAttr->au8MeshTable[0][i+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[0] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[1][i+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[1] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[2][i+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[2] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[0][i+1] = ((au32MeshTable[0] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[1][i+1] = ((au32MeshTable[1] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[2][i+1] = ((au32MeshTable[2] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[0][i+1+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[0] & 0xff000000) >> 24);
                pstMeshShadingAttr->au8MeshTable[1][i+1+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[1] & 0xff000000) >> 24);
                pstMeshShadingAttr->au8MeshTable[2][i+1+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[2] & 0xff000000) >> 24);
            }
            break;
        case ISP_MESH_MODE_TYPE_4LS:
            for (i=0; i<ISP_MESH_SHADING_NODE_NUM>>2; i++)
            {
                au32MeshTable[0] = hi_mesh_shading_mem_luts_shading_lutr_read(IspDev, i);
                au32MeshTable[1] = hi_mesh_shading_mem_luts_shading_lutg_read(IspDev, i);
                au32MeshTable[2] = hi_mesh_shading_mem_luts_shading_lutb_read(IspDev, i);
                pstMeshShadingAttr->au8MeshTable[0][i] = ((au32MeshTable[0] & 0xff));
                pstMeshShadingAttr->au8MeshTable[1][i] = ((au32MeshTable[1] & 0xff));
                pstMeshShadingAttr->au8MeshTable[2][i] = ((au32MeshTable[2] & 0xff));
                pstMeshShadingAttr->au8MeshTable[0][i+(ISP_MESH_SHADING_NODE_NUM>>2)] = ((au32MeshTable[0] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[1][i+(ISP_MESH_SHADING_NODE_NUM>>2)] = ((au32MeshTable[1] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[2][i+(ISP_MESH_SHADING_NODE_NUM>>2)] = ((au32MeshTable[2] & 0xff00) >> 8);
                pstMeshShadingAttr->au8MeshTable[0][i+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[0] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[1][i+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[1] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[2][i+(ISP_MESH_SHADING_NODE_NUM>>1)] = ((au32MeshTable[2] & 0xff0000) >> 16);
                pstMeshShadingAttr->au8MeshTable[0][i+(ISP_MESH_SHADING_NODE_NUM>>1)+(ISP_MESH_SHADING_NODE_NUM>>2)] = ((au32MeshTable[0] & 0xff000000) >> 24);
                pstMeshShadingAttr->au8MeshTable[1][i+(ISP_MESH_SHADING_NODE_NUM>>1)+(ISP_MESH_SHADING_NODE_NUM>>2)] = ((au32MeshTable[1] & 0xff000000) >> 24);
                pstMeshShadingAttr->au8MeshTable[2][i+(ISP_MESH_SHADING_NODE_NUM>>1)+(ISP_MESH_SHADING_NODE_NUM>>2)] = ((au32MeshTable[2] & 0xff000000) >> 24);
            }
            break;
        case ISP_MESH_MODE_TYPE_BUTT:
        default:
            break;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetNRAttr(ISP_DEV IspDev, const ISP_NR_ATTR_S *pstNRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_BOOL(pstNRAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstNRAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid NR type %d!\n", pstNRAttr->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_nr_enable_write(IspDev, pstNRAttr->bEnable);
    hi_ext_system_manual_nr_write(IspDev, pstNRAttr->enOpType);

    for(i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_snr_thresh_write(IspDev, i, pstNRAttr->stAuto.au8Thresh[i]);
    }

    hi_ext_system_nr_threshold_target_write(IspDev, pstNRAttr->stManual.u8Thresh);
    hi_ext_system_nr_threshold_long_target_write(IspDev, pstNRAttr->stManual.u8ThreshLong);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetNRAttr(ISP_DEV IspDev, ISP_NR_ATTR_S *pstNRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstNRAttr->bEnable = hi_isp_nr_enable_read(IspDev);
    pstNRAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_nr_read(IspDev);

    for(i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstNRAttr->stAuto.au8Thresh[i] = hi_ext_system_agc_table_snr_thresh_read(IspDev, i);
    }
    pstNRAttr->stManual.u8Thresh = hi_ext_system_nr_threshold_target_read(IspDev);
    pstNRAttr->stManual.u8ThreshLong = hi_ext_system_nr_threshold_long_target_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetNPTable(ISP_DEV IspDev, const ISP_NP_TABLE_S *pstNPTable)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNPTable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for(i = 0; i < NP_NODE_NUM_MAX; i++)
    {
        hi_isp_noise_profile_weight_lut_write(IspDev, i, pstNPTable->au8NPTable[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetNPTable(ISP_DEV IspDev, ISP_NP_TABLE_S *pstNPTable)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNPTable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for(i = 0; i < NP_NODE_NUM_MAX; i++)
    {
        pstNPTable->au8NPTable[i] = hi_isp_noise_profile_weight_lut_read(IspDev, i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetColorToneAttr(ISP_DEV IspDev, const ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCTAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    if (pstCTAttr->u16RedCastGain > 0xFFF)
    {
        printf("u16RedCastGain should in range [0x0 0xFFF]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstCTAttr->u16GreenCastGain > 0xFFF)
    {
        printf("u16GreenCastGain should in range [0x0 0xFFF]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstCTAttr->u16BlueCastGain > 0xFFF)
    {
        printf("u16BlueCastGain should in range [0x0 0xFFF]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_isp_matrix_coefft_wb_r_write(IspDev, pstCTAttr->u16RedCastGain);
    hi_isp_matrix_coefft_wb_g_write(IspDev, pstCTAttr->u16GreenCastGain);
    hi_isp_matrix_coefft_wb_b_write(IspDev, pstCTAttr->u16BlueCastGain);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetColorToneAttr(ISP_DEV IspDev, ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCTAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstCTAttr->u16RedCastGain = hi_isp_matrix_coefft_wb_r_read(IspDev);
    pstCTAttr->u16GreenCastGain = hi_isp_matrix_coefft_wb_g_read(IspDev);
    pstCTAttr->u16BlueCastGain = hi_isp_matrix_coefft_wb_b_read(IspDev);

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetGammaAttr(ISP_DEV IspDev, const ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;
    HI_U32 u32WDRMode;
    HI_U8 u8GammaBankSel;

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

    static HI_U16 au16GammaDefWDR[GAMMA_NODE_NUM] = {
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

    static HI_U16 au16GammasRGBWDR[GAMMA_NODE_NUM]={
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

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_BOOL(pstGammaAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u32WDRMode = hi_ext_system_sensor_wdr_mode_read(IspDev);

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
            pu16GammaLut = pstGammaAttr->u16Table;
            break;
        default:
            //pu16GammaLut = HI_NULL;
            printf("Invalid ISP Gamma Curve Type %d!\n", pstGammaAttr->enCurveType);
            return HI_ERR_ISP_ILLEGAL_PARAM;
            break;
    }

    for(i = 0; i < GAMMA_NODE_NUM; i++)
    {
        if (pu16GammaLut[i] > 0xFFF)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Gamma Table[%d] %d!\n", i, pu16GammaLut[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_isp_gamma_enable_write(IspDev, pstGammaAttr->bEnable);
    hi_ext_system_gamma_curve_type_write(IspDev, pstGammaAttr->enCurveType);

    for(i = 0; i < GAMMA_NODE_NUM; i++)
    {
        hi_isp_ext_gamma_rgb_mem_array_data_write(IspDev, (HI_U16)i, pu16GammaLut[i]);
    }

    u8GammaBankSel = hi_isp_gamma_rgb_use_lut_bank_1_read(IspDev);
    hi_isp_gamma_mcu_priority_write(IspDev, 1);

    if (0 == u8GammaBankSel)
    {
        for (i = 0; i < GAMMA_NODE_NUM; i++)
        {
            hi_gamma_rgb_mem_bank1_array_data_write(IspDev, i, (HI_U32)(pu16GammaLut[i]));
        }
    }
    else
    {
        for (i = 0; i < GAMMA_NODE_NUM; i++)
        {
            hi_gamma_rgb_mem_bank0_array_data_write(IspDev, i, (HI_U32)(pu16GammaLut[i]));
        }
    }

    hi_isp_gamma_mcu_priority_write(IspDev, 0);
    hi_isp_gamma_rgb_use_lut_bank_1_write(IspDev, !u8GammaBankSel);

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
    pstGammaAttr->enCurveType = hi_ext_system_gamma_curve_type_read(IspDev);

    for(i = 0; i < GAMMA_NODE_NUM; i++)
    {
        pstGammaAttr->u16Table[i] = hi_isp_ext_gamma_rgb_mem_array_data_read(IspDev, (HI_U16)i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetGammaFEAttr(ISP_DEV IspDev, const ISP_GAMMAFE_ATTR_S *pstGammaFEAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaFEAttr);
    ISP_CHECK_BOOL(pstGammaFEAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    hi_isp_gammafe_enable_write(IspDev, pstGammaFEAttr->bEnable);
    hi_isp_gammafe_enable1_write(IspDev, pstGammaFEAttr->bEnable);

    hi_isp_gammafe_mcu_priority_write(IspDev, 1);
    for(i = 0; i < GAMMA_FE0_NODE_NUM; i++)
    {
        hi_gamma_fe0_mem_array_data_write(IspDev, i, pstGammaFEAttr->u32Table[i]);
    }
    for(i = 0; i< GAMMA_FE1_NODE_NUM; i++)
    {
        hi_gamma_fe1_mem_array_data_write(IspDev, i, pstGammaFEAttr->u32Table[GAMMA_FE0_NODE_NUM+i]);
    }
    hi_isp_gammafe_mcu_priority_write(IspDev, 0);

    return HI_SUCCESS;

}

HI_S32 HI_MPI_ISP_GetGammaFEAttr(ISP_DEV IspDev, ISP_GAMMAFE_ATTR_S *pstGammaFEAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaFEAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstGammaFEAttr->bEnable = hi_isp_gammafe_enable1_read(IspDev);

    hi_isp_gammafe_mcu_priority_write(IspDev, 1);
    for(i = 0; i<GAMMA_FE0_NODE_NUM; i++)
    {
        pstGammaFEAttr->u32Table[i] = hi_gamma_fe0_mem_array_data_read(IspDev, i);
    }
    for(i = 0; i<GAMMA_FE1_NODE_NUM; i++)
    {
        pstGammaFEAttr->u32Table[GAMMA_FE0_NODE_NUM+i] = hi_gamma_fe1_mem_array_data_read(IspDev, i);
    }
    hi_isp_gammafe_mcu_priority_write(IspDev, 0);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBayerSharpenAttr(ISP_DEV IspDev, const ISP_BAYER_SHARPEN_ATTR_S *pstBayerShpAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBayerShpAttr);
    ISP_CHECK_BOOL(pstBayerShpAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        if (pstBayerShpAttr->stAuto.astShp[i].u16LumaThd > 0xFFF)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen value:%d!\n", pstBayerShpAttr->stAuto.astShp[i].u16LumaThd);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstBayerShpAttr->stManual.stShp.u16LumaThd > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen value:%d!\n", pstBayerShpAttr->stManual.stShp.u16LumaThd);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstBayerShpAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen OP Type %d!\n", pstBayerShpAttr->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstBayerShpAttr->enShpAlgSel >= SHARPEN_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen Alg Type %d!\n", pstBayerShpAttr->enShpAlgSel);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_sharpening_enable_write(IspDev, pstBayerShpAttr->bEnable);
    hi_ext_system_manual_sharpening_write(IspDev, pstBayerShpAttr->enOpType);
    hi_ext_system_sharpening_alg_select_write(IspDev, pstBayerShpAttr->enShpAlgSel);


    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_sharp_alt_d_write(IspDev, i, pstBayerShpAttr->stAuto.astShp[i].u8SharpenD);
        hi_ext_system_agc_table_sharp_alt_ud_write(IspDev, i, pstBayerShpAttr->stAuto.astShp[i].u8SharpenUd);
        hi_ext_system_agc_table_demosaic_lum_thresh_write(IspDev, i, pstBayerShpAttr->stAuto.astShp[i].u16LumaThd);
        hi_ext_system_agc_table_sharping_hf_write(IspDev, i, pstBayerShpAttr->stAuto.astShpEx[i].u8SharpenHF);
        hi_ext_system_agc_table_sharping_mf_write(IspDev, i, pstBayerShpAttr->stAuto.astShpEx[i].u8SharpenMF);
        hi_ext_system_agc_table_sharping_lf_write(IspDev, i, pstBayerShpAttr->stAuto.astShpEx[i].u8SharpenLF);
        hi_ext_system_agc_table_sharping_sad_amplifier_write(IspDev, i, pstBayerShpAttr->stAuto.astShpEx[i].u8SadAmplifier);
    }

    hi_ext_system_sharpening_d_target_write(IspDev, pstBayerShpAttr->stManual.stShp.u8SharpenD);
    hi_ext_system_sharpening_ud_target_write(IspDev, pstBayerShpAttr->stManual.stShp.u8SharpenUd);
    hi_ext_system_demosaic_lum_thresh_target_write(IspDev, pstBayerShpAttr->stManual.stShp.u16LumaThd);
    hi_ext_system_sharpening_hf_target_write(IspDev, pstBayerShpAttr->stManual.stShpEx.u8SharpenHF);
    hi_ext_system_sharpening_mf_target_write(IspDev, pstBayerShpAttr->stManual.stShpEx.u8SharpenMF);
    hi_ext_system_sharpening_lf_target_write(IspDev, pstBayerShpAttr->stManual.stShpEx.u8SharpenLF);
    hi_ext_system_sharpening_sad_amplifier_write(IspDev, pstBayerShpAttr->stManual.stShpEx.u8SadAmplifier);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBayerSharpenAttr(ISP_DEV IspDev, ISP_BAYER_SHARPEN_ATTR_S *pstBayerShpAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBayerShpAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstBayerShpAttr->bEnable = hi_ext_system_sharpening_enable_read(IspDev);
    pstBayerShpAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_sharpening_read(IspDev);
    pstBayerShpAttr->enShpAlgSel = hi_ext_system_sharpening_alg_select_read(IspDev);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstBayerShpAttr->stAuto.astShp[i].u8SharpenD = hi_ext_system_agc_table_sharp_alt_d_read(IspDev, i);
        pstBayerShpAttr->stAuto.astShp[i].u8SharpenUd = hi_ext_system_agc_table_sharp_alt_ud_read(IspDev, i);
        pstBayerShpAttr->stAuto.astShp[i].u16LumaThd = hi_ext_system_agc_table_demosaic_lum_thresh_read(IspDev, i);
        pstBayerShpAttr->stAuto.astShpEx[i].u8SharpenHF = hi_ext_system_agc_table_sharping_hf_read(IspDev, i);
        pstBayerShpAttr->stAuto.astShpEx[i].u8SharpenMF = hi_ext_system_agc_table_sharping_mf_read(IspDev, i);
        pstBayerShpAttr->stAuto.astShpEx[i].u8SharpenLF = hi_ext_system_agc_table_sharping_lf_read(IspDev, i);
        pstBayerShpAttr->stAuto.astShpEx[i].u8SadAmplifier = hi_ext_system_agc_table_sharping_sad_amplifier_read(IspDev, i);
    }

    pstBayerShpAttr->stManual.stShp.u8SharpenD = hi_ext_system_sharpening_d_target_read(IspDev);
    pstBayerShpAttr->stManual.stShp.u8SharpenUd = hi_ext_system_sharpening_ud_target_read(IspDev);
    pstBayerShpAttr->stManual.stShp.u16LumaThd = hi_ext_system_demosaic_lum_thresh_target_read(IspDev);
    pstBayerShpAttr->stManual.stShpEx.u8SharpenHF = hi_ext_system_sharpening_hf_target_read(IspDev);
    pstBayerShpAttr->stManual.stShpEx.u8SharpenMF = hi_ext_system_sharpening_mf_target_read(IspDev);
    pstBayerShpAttr->stManual.stShpEx.u8SharpenLF = hi_ext_system_sharpening_lf_target_read(IspDev);
    pstBayerShpAttr->stManual.stShpEx.u8SadAmplifier = hi_ext_system_sharpening_sad_amplifier_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetYuvSharpenAttr(ISP_DEV IspDev, const ISP_YUV_SHARPEN_ATTR_S *pstYuvShpAttr)
{
    HI_U8 i,j;
    HI_U32 u32LumaThd;
    HI_U32 u32LumaWgt;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstYuvShpAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_BOOL(pstYuvShpAttr->bEnable);
    ISP_CHECK_BOOL(pstYuvShpAttr->stManual.bEnLowLumaShoot);

    for(i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        ISP_CHECK_BOOL(pstYuvShpAttr->stAuto.abEnLowLumaShoot[i]);
    }

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        if (pstYuvShpAttr->stAuto.au8JagCtrl[i] > 0x3F)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid Yuv Sharpen value:%d!\n", pstYuvShpAttr->stAuto.au8JagCtrl[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstYuvShpAttr->stManual.u8JagCtrl > 0x3F)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Yuv Sharpen value:%d!\n", pstYuvShpAttr->stManual.u8JagCtrl);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstYuvShpAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen Type %d!\n", pstYuvShpAttr->enOpType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    //hi_ext_system_yuv_sharpening_enable_write(IspDev, pstYuvShpAttr->bEnable);
    hi_isp_sharpen_cfg_enable_write(IspDev, pstYuvShpAttr->bEnable);
    hi_ext_system_manual_yuv_sharpening_write(IspDev, pstYuvShpAttr->enOpType);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_yuv_sharpening_TextureSt_write(IspDev, i, pstYuvShpAttr->stAuto.au8TextureSt[i]);
        hi_ext_system_yuv_sharpening_EdgeSt_write(IspDev, i, pstYuvShpAttr->stAuto.au8EdgeSt[i]);
        hi_ext_system_yuv_sharpening_OverShoot_write(IspDev, i, pstYuvShpAttr->stAuto.au8OverShoot[i]);
        hi_ext_system_yuv_sharpening_UnderShoot_write(IspDev, i, pstYuvShpAttr->stAuto.au8UnderShoot[i]);
        hi_ext_system_yuv_sharpening_EnLowLumaShoot_write(IspDev, i, pstYuvShpAttr->stAuto.abEnLowLumaShoot[i]);
        hi_ext_system_yuv_sharpening_TextureThd_write(IspDev, i, pstYuvShpAttr->stAuto.au8TextureThd[i]);
        hi_ext_system_yuv_sharpening_EdgeThd_write(IspDev, i, pstYuvShpAttr->stAuto.au8EdgeThd[i]);
        hi_ext_system_yuv_sharpening_JagCtrl_write(IspDev, i, pstYuvShpAttr->stAuto.au8JagCtrl[i]);
        hi_ext_system_yuv_sharpening_SaltCtrl_write(IspDev, i, pstYuvShpAttr->stAuto.au8SaltCtrl[i]);
        hi_ext_system_yuv_sharpening_PepperCtrl_write(IspDev, i, pstYuvShpAttr->stAuto.au8PepperCtrl[i]);
        hi_ext_system_yuv_sharpening_DetailCtrl_write(IspDev, i, pstYuvShpAttr->stAuto.au8DetailCtrl[i]);

        u32LumaThd = 0;
        u32LumaWgt = 0;
        for (j = 0; j < 4; j++)
        {
            u32LumaThd |= (pstYuvShpAttr->stAuto.au8LumaThd[j][i] << j*8);
            u32LumaWgt |= (pstYuvShpAttr->stAuto.au8LumaWgt[j][i] << j*8);
        }

        hi_ext_system_yuv_sharpening_LumaThd_write(IspDev, i, u32LumaThd);
        hi_ext_system_yuv_sharpening_LumaWgt_write(IspDev, i, u32LumaWgt);
    }

    hi_ext_system_manual_yuv_sharpening_TextureSt_write(IspDev, pstYuvShpAttr->stManual.u8TextureSt);
    hi_ext_system_manual_yuv_sharpening_EdgeSt_write(IspDev, pstYuvShpAttr->stManual.u8EdgeSt);
    hi_ext_system_manual_yuv_sharpening_OverShoot_write(IspDev, pstYuvShpAttr->stManual.u8OverShoot);
    hi_ext_system_manual_yuv_sharpening_UnderShoot_write(IspDev, pstYuvShpAttr->stManual.u8UnderShoot);
    hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_write(IspDev, pstYuvShpAttr->stManual.bEnLowLumaShoot);
    hi_ext_system_manual_yuv_sharpening_TextureThd_write(IspDev, pstYuvShpAttr->stManual.u8TextureThd);
    hi_ext_system_manual_yuv_sharpening_EdgeThd_write(IspDev, pstYuvShpAttr->stManual.u8EdgeThd);
    hi_ext_system_manual_yuv_sharpening_JagCtrl_write(IspDev, pstYuvShpAttr->stManual.u8JagCtrl);
    hi_ext_system_manual_yuv_sharpening_SaltCtrl_write(IspDev, pstYuvShpAttr->stManual.u8SaltCtrl);
    hi_ext_system_manual_yuv_sharpening_PepperCtrl_write(IspDev, pstYuvShpAttr->stManual.u8PepperCtrl);
    hi_ext_system_manual_yuv_sharpening_DetailCtrl_write(IspDev, pstYuvShpAttr->stManual.u8DetailCtrl);

    u32LumaThd = 0;
    u32LumaWgt = 0;
    for (j = 0; j < 4; j++)
    {
        u32LumaThd |= (pstYuvShpAttr->stManual.au8LumaThd[j] << j*8);
        u32LumaWgt |= (pstYuvShpAttr->stManual.au8LumaWgt[j] << j*8);
    }

    hi_ext_system_manual_yuv_sharpening_LumaThd_write(IspDev, u32LumaThd);
    hi_ext_system_manual_yuv_sharpening_LumaWgt_write(IspDev, u32LumaWgt);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetYuvSharpenAttr(ISP_DEV IspDev, ISP_YUV_SHARPEN_ATTR_S *pstYuvShpAttr)
{
    HI_U8 i,j;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstYuvShpAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstYuvShpAttr->bEnable = hi_isp_sharpen_cfg_enable_read(IspDev);
    pstYuvShpAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_manual_yuv_sharpening_read(IspDev);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstYuvShpAttr->stAuto.au8TextureSt[i] = hi_ext_system_yuv_sharpening_TextureSt_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8EdgeSt[i] = hi_ext_system_yuv_sharpenng_EdgeSt_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8OverShoot[i] = hi_ext_system_yuv_sharpening_OverShoot_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8UnderShoot[i] = hi_ext_system_yuv_sharpening_UnderShoot_read(IspDev, i);
        pstYuvShpAttr->stAuto.abEnLowLumaShoot[i] = hi_ext_system_yuv_sharpening_EnLowLumaShoot_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8TextureThd[i] = hi_ext_system_yuv_sharpening_TextureThd_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8EdgeThd[i] = hi_ext_system_yuv_sharpening_EdgeThd_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8JagCtrl[i] = hi_ext_system_yuv_sharpening_JagCtrl_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8SaltCtrl[i] = hi_ext_system_yuv_sharpening_SaltCtrl_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8PepperCtrl[i] = hi_ext_system_yuv_sharpening_PepperCtrl_read(IspDev, i);
        pstYuvShpAttr->stAuto.au8DetailCtrl[i] = hi_ext_system_yuv_sharpening_DetailCtrl_read(IspDev, i);

        for (j = 0; j < 4; j++)
        {
            pstYuvShpAttr->stAuto.au8LumaThd[j][i] = (HI_U8)((hi_ext_system_yuv_sharpening_LumaThd_read(IspDev, i) & (0xFF << j*8)) >> j*8);
            pstYuvShpAttr->stAuto.au8LumaWgt[j][i] = (HI_U8)((hi_ext_system_yuv_sharpening_LumaWgt_read(IspDev, i) & (0xFF << j*8)) >> j*8);
        }
    }

    pstYuvShpAttr->stManual.u8TextureSt = hi_ext_system_manual_yuv_sharpening_TextureSt_read(IspDev);
    pstYuvShpAttr->stManual.u8EdgeSt = hi_ext_system_manual_yuv_sharpening_EdgeSt_read(IspDev);
    pstYuvShpAttr->stManual.u8OverShoot = hi_ext_system_manual_yuv_sharpening_OverShoot_read(IspDev);
    pstYuvShpAttr->stManual.u8UnderShoot = hi_ext_system_manual_yuv_sharpening_UnderShoot_read(IspDev);
    pstYuvShpAttr->stManual.bEnLowLumaShoot = hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_read(IspDev);
    pstYuvShpAttr->stManual.u8TextureThd = hi_ext_system_manual_yuv_sharpening_TextureThd_read(IspDev);
    pstYuvShpAttr->stManual.u8EdgeThd = hi_ext_system_manual_yuv_sharpening_EdgeThd_read(IspDev);
    pstYuvShpAttr->stManual.u8JagCtrl = hi_ext_system_manual_yuv_sharpening_JagCtrl_read(IspDev);
    pstYuvShpAttr->stManual.u8SaltCtrl = hi_ext_system_manual_yuv_sharpening_SaltCtrl_read(IspDev);
    pstYuvShpAttr->stManual.u8PepperCtrl = hi_ext_system_manual_yuv_sharpening_PepperCtrl_read(IspDev);
    pstYuvShpAttr->stManual.u8DetailCtrl = hi_ext_system_manual_yuv_sharpening_DetailCtrl_read(IspDev);

    for (j = 0; j < 4; j++)
    {
        pstYuvShpAttr->stManual.au8LumaThd[j] = (HI_U8)((hi_ext_system_manual_yuv_sharpening_LumaThd_read(IspDev) & (0xFF << j*8)) >> j*8);
        pstYuvShpAttr->stManual.au8LumaWgt[j] = (HI_U8)((hi_ext_system_manual_yuv_sharpening_LumaWgt_read(IspDev) & (0xFF << j*8)) >> j*8);
    }

    return HI_SUCCESS;
}

/* Crosstalk Removal Strength */
HI_S32 HI_MPI_ISP_SetCrosstalkAttr(ISP_DEV IspDev, const ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_BOOL(pstCRAttr->bEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstCRAttr->u16Threshold > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid CrosstalkAttr u16Threshold:%d!\n", pstCRAttr->u16Threshold);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstCRAttr->u16Slope > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid CrosstalkAttr u16Slope:%d!\n", pstCRAttr->u16Slope);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_green_equalize_ge_enable_write(IspDev, pstCRAttr->bEnable);
    hi_isp_green_equalize_ge_threshold_write(IspDev, pstCRAttr->u16Threshold);
    hi_isp_green_equalize_ge_slope_write(IspDev, pstCRAttr->u16Slope);
    hi_isp_green_equalize_ge_sens_write(IspDev, pstCRAttr->u8Sensitivity);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_ge_strength_write(IspDev, i, pstCRAttr->au8Strength[i]);
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

    pstCRAttr->bEnable = hi_isp_green_equalize_ge_enable_read(IspDev);
    pstCRAttr->u16Threshold = hi_isp_green_equalize_ge_threshold_read(IspDev);
    pstCRAttr->u16Slope = hi_isp_green_equalize_ge_slope_read(IspDev);
    pstCRAttr->u8Sensitivity = hi_isp_green_equalize_ge_sens_read(IspDev);

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstCRAttr->au8Strength[i] = hi_ext_system_agc_table_ge_strength_read(IspDev, i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAntiFalseColorAttr(ISP_DEV IspDev, const ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    hi_isp_demosaic_fc_slope_write(IspDev, pstAntiFC->u8Strength);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetAntiFalseColorAttr(ISP_DEV IspDev, ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstAntiFC->u8Strength = hi_isp_demosaic_fc_slope_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDemosaicAttr(ISP_DEV IspDev, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstDemosaicAttr->u16VhThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic VhThresh %d!\n", pstDemosaicAttr->u16VhThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16AaThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic AaThresh %d!\n", pstDemosaicAttr->u16AaThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16VaThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic VaThresh %d!\n", pstDemosaicAttr->u16VaThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16UuThresh > 0xFFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic UuThresh %d!\n", pstDemosaicAttr->u16UuThresh);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstDemosaicAttr->enCfgType >= ISP_DEMOSAIC_CFG_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic Config %d!\n", pstDemosaicAttr->enCfgType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_isp_demosaic_vh_slope_write(IspDev, pstDemosaicAttr->u8VhSlope);
    hi_isp_demosaic_aa_slope_write(IspDev, pstDemosaicAttr->u8AaSlope);
    hi_isp_demosaic_va_slope_write(IspDev, pstDemosaicAttr->u8VaSlope);
    hi_isp_demosaic_uu_slope_write(IspDev, pstDemosaicAttr->u8UuSlope);

    hi_isp_demosaic_vh_thresh_write(IspDev, pstDemosaicAttr->u16VhThresh);
    hi_isp_demosaic_aa_thresh_write(IspDev, pstDemosaicAttr->u16AaThresh);
    hi_isp_demosaic_va_thresh_write(IspDev, pstDemosaicAttr->u16VaThresh);
    hi_isp_demosaic_uu_thresh_write(IspDev, pstDemosaicAttr->u16UuThresh);

    switch(pstDemosaicAttr->enCfgType)
    {
        case ISP_DEMOSAIC_CFG_VH:
            hi_isp_demosaic_dmsc_config_write(IspDev, 0x13);
            break;
        case ISP_DEMOSAIC_CFG_AA:
            hi_isp_demosaic_dmsc_config_write(IspDev, 0x11);
            break;
        case ISP_DEMOSAIC_CFG_VA:
            hi_isp_demosaic_dmsc_config_write(IspDev, 0x12);
            break;
        case ISP_DEMOSAIC_CFG_UU:
            hi_isp_demosaic_dmsc_config_write(IspDev, 0x4);
            break;
        default:
            hi_isp_demosaic_dmsc_config_write(IspDev, 0x0);
    }
    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        hi_ext_system_agc_table_demosaic_np_offset_write(IspDev, i, pstDemosaicAttr->au8NpOffset[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDemosaicAttr(ISP_DEV IspDev, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDemosaicAttr->u8VhSlope = hi_isp_demosaic_vh_slope_read(IspDev);
    pstDemosaicAttr->u8AaSlope = hi_isp_demosaic_aa_slope_read(IspDev);
    pstDemosaicAttr->u8VaSlope = hi_isp_demosaic_va_slope_read(IspDev);
    pstDemosaicAttr->u8UuSlope = hi_isp_demosaic_uu_slope_read(IspDev);

    pstDemosaicAttr->u16VhThresh = hi_isp_demosaic_vh_thresh_read(IspDev);
    pstDemosaicAttr->u16AaThresh = hi_isp_demosaic_aa_thresh_read(IspDev);
    pstDemosaicAttr->u16VaThresh = hi_isp_demosaic_va_thresh_read(IspDev);
    pstDemosaicAttr->u16UuThresh = hi_isp_demosaic_uu_thresh_read(IspDev);

    switch(hi_isp_demosaic_dmsc_config_read(IspDev))
    {
        case 0x13:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_VH;
            break;
        case 0x11:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_AA;
            break;
        case 0x12:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_VA;
            break;
        case 0x4:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_UU;
            break;
        case 0x0:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_DEFAULT;
            break;
        default:
            pstDemosaicAttr->enCfgType = ISP_DEMOSAIC_CFG_BUTT;
    }

    for (i = 0; i < ISP_AUTO_STENGTH_NUM; i++)
    {
        pstDemosaicAttr->au8NpOffset[i] = hi_ext_system_agc_table_demosaic_np_offset_read(IspDev, i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBlackLevelAttr(ISP_DEV IspDev, const ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    for (i=0; i<4; i++)
    {
        if (pstBlackLevel->au16BlackLevel[i] > 0xFFF)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid BlackLevel[%d]:%d!\n", i, pstBlackLevel->au16BlackLevel[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_ext_system_black_level_00_write(IspDev, pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_01_write(IspDev, pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_10_write(IspDev, pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_11_write(IspDev, pstBlackLevel->au16BlackLevel[3]);
    hi_ext_system_black_level_change_write(IspDev, (HI_U8)HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBlackLevelAttr(ISP_DEV IspDev, ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstBlackLevel->au16BlackLevel[0] = hi_ext_system_black_level_00_read(IspDev);
    pstBlackLevel->au16BlackLevel[1] = hi_ext_system_black_level_01_read(IspDev);
    pstBlackLevel->au16BlackLevel[2] = hi_ext_system_black_level_10_read(IspDev);
    pstBlackLevel->au16BlackLevel[3] = hi_ext_system_black_level_11_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_FPNCalibrate(ISP_DEV IspDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCalibrateAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_SetCalibrateAttr(IspDev, pstCalibrateAttr);
}

HI_S32 HI_MPI_ISP_SetFPNAttr(ISP_DEV IspDev, const ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_SetCorrectionAttr(IspDev, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_GetFPNAttr(ISP_DEV IspDev, ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_GetCorrectionAttr(IspDev, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_MeshShadingCalibration(HI_U16* pu16SrcRaw,
    ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg , ISP_MESH_SHADING_TABLE_S stMeshShadingResult)
{
    HI_U32 i = 0, j = 0;
    ISP_CHECK_POINTER(pu16SrcRaw);
    ISP_CHECK_POINTER(pstLSCCaliCfg);
    ISP_CHECK_POINTER(stMeshShadingResult.pu8Rgain);
    ISP_CHECK_POINTER(stMeshShadingResult.pu8Ggain);
    ISP_CHECK_POINTER(stMeshShadingResult.pu8Bgain);

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

    return ISP_MeshShadingCalibration(pu16SrcRaw,pstLSCCaliCfg,stMeshShadingResult);
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
    HI_S32 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_BOOL(pstDefogAttr->bEnable);
    ISP_CHECK_BOOL(pstDefogAttr->bUserLutEnable);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

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

    if(pstDefogAttr->u16TmprfltIncrCoef > 0x80)
    {
       printf("Invalid Defog u16TmprfltIncrCoef Input\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(pstDefogAttr->u16TmprfltDecrCoef > 0x80)
    {
       printf("Invalid Defog u16TmprfltDecrCoef Input\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    isp_ext_system_dehaze_enable_write(IspDev, pstDefogAttr->bEnable);
    isp_ext_system_manual_dehaze_enable_write(IspDev, pstDefogAttr->enOpType);
    isp_ext_system_manual_dehaze_strength_write(IspDev, pstDefogAttr->stManual.u8strength);
    isp_ext_system_manual_dehaze_autostrength_write(IspDev, pstDefogAttr->stAuto.u8strength);

    hi_ext_system_user_defog_lut_enable_write(IspDev, pstDefogAttr->bUserLutEnable);
  	for (i = 0; i<256; i++)
  	{
  	    hi_ext_system_defog_lut_write(IspDev, i,pstDefogAttr->au8DefogLut[i]);
  	}
  	if(pstDefogAttr->bUserLutEnable)
  	{
  	    hi_ext_system_user_defog_lut_update_write(IspDev, 1);//1:update the defog lut,FW will change it to 0 when the lut updating is finished.
  	}
	
	 isp_ext_system_dehaze_tfic_write(IspDev, pstDefogAttr->u16TmprfltIncrCoef);
     isp_ext_system_dehaze_tfdc_write(IspDev, pstDefogAttr->u16TmprfltDecrCoef);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDeFogAttr(ISP_DEV IspDev, ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    HI_S32 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDefogAttr->bEnable = isp_ext_system_dehaze_enable_read(IspDev);
    pstDefogAttr->enOpType = (ISP_OP_TYPE_E)isp_ext_system_manual_dehaze_enable_read(IspDev);
    pstDefogAttr->stManual.u8strength = isp_ext_system_manual_dehaze_strength_read(IspDev);
    pstDefogAttr->stAuto.u8strength = isp_ext_system_manual_dehaze_autostrength_read(IspDev);
    pstDefogAttr->bUserLutEnable= hi_ext_system_user_defog_lut_enable_read(IspDev);
    for (i=0; i<256; i++)
    {
        pstDefogAttr->au8DefogLut[i] = hi_ext_system_defog_lut_read(IspDev, i);
    }
	 pstDefogAttr->u16TmprfltDecrCoef = isp_ext_system_dehaze_tfdc_read(IspDev);
     pstDefogAttr->u16TmprfltIncrCoef = isp_ext_system_dehaze_tfic_read(IspDev);
 
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_ACM_SetAttr(IspDev, pstAcmAttr);
}
HI_S32 HI_MPI_ISP_GetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_ACM_GetAttr(IspDev, pstAcmAttr);
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
                if ((pstAcmLUT->as16Y[i][j][k] < -256) || (pstAcmLUT->as16Y[i][j][k] > 255))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16Y[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16Y[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
                if ((pstAcmLUT->as16H[i][j][k] < -64) || (pstAcmLUT->as16H[i][j][k] > 63))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16H[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16H[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
                if ((pstAcmLUT->as16S[i][j][k] < -256) || (pstAcmLUT->as16S[i][j][k] > 255))
                {
                    ISP_TRACE(HI_DBG_ERR, "Invalid AcmLUT as16S[%d][%d][%d]:%d!\n", i, j, k, pstAcmLUT->as16S[i][j][k]);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
            }
        }
    }


    s32Ret = ISP_ACM_SetCoeff(IspDev, pstAcmLUT);
    return s32Ret;
}
HI_S32 HI_MPI_ISP_GetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT)
{
    HI_S32 s32Ret;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    s32Ret =  ISP_ACM_GetCoeff(IspDev, pstAcmLUT);
    return s32Ret;
}

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(ISP_DEV IspDev, ISP_INNER_STATE_INFO_S *pstInnerStateInfo)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstInnerStateInfo);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstInnerStateInfo->u32DRCStrengthActual = hi_isp_drc_strength_inroi_read(IspDev);
    pstInnerStateInfo->u32NRStrengthActual = hi_isp_nr_thresh_short_read(IspDev);
    pstInnerStateInfo->u32SharpenStrengthDActual = hi_isp_demosaic_sharp_alt_d_read(IspDev);
    pstInnerStateInfo->u32SharpenStrengthUdActual = hi_isp_demosaic_sharp_alt_ud_read(IspDev);
    pstInnerStateInfo->u32DefogStrengthActual = hi_isp_dehaze_gstrth_read(IspDev);
    pstInnerStateInfo->u32LongExpRatioActual = hi_isp_frame_stitch_long_exposure_ratio_read(IspDev);
    pstInnerStateInfo->u32MedExpRatioActual = hi_isp_frame_stitch_med_exposure_ratio_read(IspDev);
    pstInnerStateInfo->u32ShortExpRatioActual = hi_isp_frame_stitch_short_exposure_ratio_read(IspDev);

    pstInnerStateInfo->bWDRSwitchFinish = hi_ext_top_wdr_switch_read(IspDev);
    pstInnerStateInfo->bResSwitchFinish = hi_ext_top_res_switch_read(IspDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetStatisticsConfig(ISP_DEV IspDev, const ISP_STATISTICS_CFG_S *pstStatCfg)
{

    HI_U8 u8Shift0, u8I;
    HI_S16 s16G0, s16G1, s16G2;
    HI_FLOAT f32Temp, f32Pl;
    HI_U32 u32Plg, u32Pls;
    const ISP_AF_H_PARAM_S *pstIIR;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if(pstStatCfg->stAECfg.au8HistThresh[1] < pstStatCfg->stAECfg.au8HistThresh[0])
    {
        printf("AE Thresh[1] should not less than Thresh[0]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstStatCfg->stAECfg.au8HistThresh[2] < pstStatCfg->stAECfg.au8HistThresh[1])
    {
        printf("AE Thresh[2] should not less than Thresh[1]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstStatCfg->stAECfg.au8HistThresh[3] < pstStatCfg->stAECfg.au8HistThresh[2])
    {
        printf("AE Thresh[3] should not less than Thresh[2]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enAESwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Switch %d in %s!\n", pstStatCfg->stAECfg.enAESwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enAESumSwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Sum Switch %d in %s!\n", pstStatCfg->stAECfg.enAESumSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enHistSwitch >= ISP_AE_HIST_SWITCH_BUTT)
    {
        printf("Invalid AE 1024 Histogram Switch %d in %s!\n", pstStatCfg->stAECfg.enHistSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enPreHistSwitch >= ISP_AE_PRE_HIST_BUTT)
    {
        printf("Invalid AE 256 Pre_Histogram Switch %d in %s!\n", pstStatCfg->stAECfg.enPreHistSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_STATIC_WB)
    {
        hi_isp_top_ae_switch_write(IspDev, HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB);
    }
    else if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_FROM_SENSOR_CHANNEL_1)
    {
        hi_isp_top_ae_switch_write(IspDev, HI_ISP_TOP_AE_SWITCH_AFTER_TPG_CHANNEL1);
    }
    else if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_SHADING)
    {
        hi_isp_top_ae_switch_write(IspDev, HI_ISP_TOP_AE_SWITCH_AFTER_SHADING);
    }
    else if (pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_WDR_STITCH)
    {
        hi_isp_top_ae_switch_write(IspDev, HI_ISP_TOP_AE_SWITCH_AFTER_WDR_STITCH);
    }
    else
    {
        printf("not support!\n");
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistSkipX > 5)
    {
        printf("u8HistSkipX should not larger than 5\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistSkipY > 6)
    {
        printf("u8HistSkipY should not larger than 6\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX > 1)
    {
        printf("u8HistOffsetX should not larger than 1\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY > 1)
    {
        printf("u8HistOffsetY should not larger than 1\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enAESumSwitch == ISP_AE_AFTER_STATIC_WB)
    {
        hi_isp_top_aesum_switch_write(IspDev, HI_ISP_TOP_AESUM_SWITCH_AFTER_STATIC_WB);
    }
    else if (pstStatCfg->stAECfg.enAESumSwitch == ISP_AE_FROM_SENSOR_CHANNEL_1)
    {
        hi_isp_top_aesum_switch_write(IspDev, HI_ISP_TOP_AESUM_SWITCH_AFTER_TPG_CHANNEL1);
    }
    else if (pstStatCfg->stAECfg.enAESumSwitch == ISP_AE_AFTER_SHADING)
    {
        hi_isp_top_aesum_switch_write(IspDev, HI_ISP_TOP_AESUM_SWITCH_AFTER_SHADING);
    }
    else if (pstStatCfg->stAECfg.enAESumSwitch == ISP_AE_AFTER_WDR_STITCH)
    {
        hi_isp_top_aesum_switch_write(IspDev, HI_ISP_TOP_AESUM_SWITCH_AFTER_WDR_STITCH);
    }
    else
    {
        printf("not support!\n");
    }

    if (pstStatCfg->stAECfg.enHistSwitch == ISP_AE_HIST_SAME_AS_AE)
    {
        hi_isp_top_histogram_switch_write(IspDev, HI_ISP_TOP_HISTOGRAM_SWITCH_SAME_AS_AE);
    }
    else if (pstStatCfg->stAECfg.enHistSwitch == ISP_AE_HIST_FROM_SENSOR_CHANNEL_1)
    {
        hi_isp_top_histogram_switch_write(IspDev, HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_TPG_CHANNEL1);
    }
    else if (pstStatCfg->stAECfg.enHistSwitch == ISP_AE_HIST_AFTER_SHADING)
    {
        hi_isp_top_histogram_switch_write(IspDev, HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_SHADING);
    }
    else if (pstStatCfg->stAECfg.enHistSwitch == ISP_AE_HIST_AFTER_WDR_STITCH)
    {
        hi_isp_top_histogram_switch_write(IspDev, HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_WDR_STITCH);
    }
    else
    {
        printf("not support!\n");
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel > 0x3FF)
    {
        printf("Max value of RGB Config u16WhiteLevel is 0x3FF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CrMax > 0xFFF)
    {
        printf("Max value of RGB Config u16CrMax is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CbMax > 0xFFF)
    {
        printf("Max value of RGB Config u16CbMax is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel > pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel)
    {
        printf("RGB Config BlackLevel should not larger than WhiteLevel!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CrMin > pstStatCfg->stWBCfg.stRGBCfg.u16CrMax)
    {
        printf("RGB Config CrMin should not larger than CrMax!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stRGBCfg.u16CbMin > pstStatCfg->stWBCfg.stRGBCfg.u16CbMax)
    {
        printf("RGB Config CbMin should not larger than CbMax!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

#if 0
    /*confilict with the logic default setting, delete*/
    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh > pstStatCfg->stWBCfg.stRGBCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh < pstStatCfg->stWBCfg.stRGBCfg.u16CrMin) )
    {
        printf("RGB Config u16CrHigh should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CrLow> pstStatCfg->stWBCfg.stRGBCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CrLow < pstStatCfg->stWBCfg.stRGBCfg.u16CrMin) )
    {
        printf("RGB Config u16CrLow should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh > pstStatCfg->stWBCfg.stRGBCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh < pstStatCfg->stWBCfg.stRGBCfg.u16CbMin) )
    {
        printf("RGB Config u16CbHigh should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stRGBCfg.u16CbLow> pstStatCfg->stWBCfg.stRGBCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stRGBCfg.u16CbLow < pstStatCfg->stWBCfg.stRGBCfg.u16CbMin) )
    {
        printf("RGB Config u16CbLow should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
#endif

    if (pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel > 0xFFF)
    {
        printf("Max value of Bayer Config u16WhiteLevel is 0xFFF!\n");
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

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh > 0xFFF)
    {
        printf("Max value of Bayer Config u16CrHigh is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrLow > 0xFFF)
    {
        printf("Max value of Bayer Config u16CrLow is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh > 0xFFF)
    {
        printf("Max value of Bayer Config u16CbHigh is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbLow > 0xFFF)
    {
        printf("Max value of Bayer Config u16CbLow is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* AF paramters check start */
    /* confg */
	MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.bEnable,              0x1,       "Invalid AF bEnable input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Hwnd,                 1, 17,        "Invalid AF u16Hwnd input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Vwnd,                 1, 15,        "Invalid AF u16Vwnd input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Hsize,                1, RES_WIDTH_MAX,      "Invalid AF u16Hsize input!\n");
    MPI_ISP_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.u16Vsize,                1, RES_HEIGHT_MAX,      "Invalid AF u16Vsize input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enPeakMode,              0x1,       "Invalid AF enPeakMode input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enSquMode,               0x1,       "Invalid AF enSquMode input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable,          0x1,       "Invalid AF stCrop.bEnable input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16X,             0x1FFF,    "Invalid AF stCrop.u16X input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y,             0x1FFF,    "Invalid AF stCrop.u16Y input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16W,             0x1FFF,    "Invalid AF stCrop.u16W input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.stCrop.u16H,             0x1FFF,    "Invalid AF stCrop.u16H input!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(pstStatCfg->stFocusCfg.stConfig.enStatisticsPos,         0x1,       "Invalid AF enStatisticsPos input!\n");
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


    hi_ext_system_statistics_ctrl_write(IspDev, pstStatCfg->unKey.u32Key);

    hi_isp_metering_hist_thresh_0_1_write(IspDev, pstStatCfg->stAECfg.au8HistThresh[0]);
    hi_isp_metering_hist_thresh_1_2_write(IspDev, pstStatCfg->stAECfg.au8HistThresh[1]);
    hi_isp_metering_hist_thresh_3_4_write(IspDev, pstStatCfg->stAECfg.au8HistThresh[2]);
    hi_isp_metering_hist_thresh_4_5_write(IspDev, pstStatCfg->stAECfg.au8HistThresh[3]);

//    hi_isp_top_histogram_pe_switch_write(IspDev, pstStatCfg->stAECfg.enPreHistSwitch);
    hi_isp_top_histogram_pe1_switch_write(IspDev, pstStatCfg->stAECfg.enPreHistSwitch);
    hi_isp_top_histogram_pe2_switch_write(IspDev, pstStatCfg->stAECfg.enPreHistSwitch);
    hi_isp_top_histogram_pe3_switch_write(IspDev, pstStatCfg->stAECfg.enPreHistSwitch);
    hi_isp_top_histogram_pe4_switch_write(IspDev, pstStatCfg->stAECfg.enPreHistSwitch);

    hi_isp_metering_skip_x_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistSkipX);
    hi_isp_metering_skip_y_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistSkipY);
    hi_isp_metering_offset_x_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX);
    hi_isp_metering_offset_y_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY);
    hi_isp_metering_skip_x_pe_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistSkipX);
    hi_isp_metering_skip_y_pe_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistSkipY);
    hi_isp_metering_offset_x_pe_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX);
    hi_isp_metering_offset_y_pe_write(IspDev, pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY);

    hi_isp_metering_white_level_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel);
    hi_isp_metering_black_level_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel);
    hi_isp_metering_cr_ref_max_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CrMax);
    hi_isp_metering_cr_ref_min_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CrMin);
    hi_isp_metering_cb_ref_max_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CbMax);
    hi_isp_metering_cb_ref_min_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CbMin);
    hi_isp_metering_cr_ref_high_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh);
    hi_isp_metering_cr_ref_low_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CrLow);
    hi_isp_metering_cb_ref_high_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh);
    hi_isp_metering_cb_ref_low_awb_write(IspDev, pstStatCfg->stWBCfg.stRGBCfg.u16CbLow);

    hi_ext_system_bayer_statistics_whitelevel_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel);
    hi_ext_system_bayer_statistics_blacklevel_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel);
    hi_isp_metering_cr_ref_max_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CrMax);
    hi_isp_metering_cr_ref_min_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CrMin);
    hi_isp_metering_cb_ref_max_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CbMax);
    hi_isp_metering_cb_ref_min_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CbMin);
    hi_isp_metering_cr_ref_high_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh);
    hi_isp_metering_cr_ref_low_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CrLow);
    hi_isp_metering_cb_ref_high_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh);
    hi_isp_metering_cb_ref_low_awb_sum_write(IspDev, pstStatCfg->stWBCfg.stBayerCfg.u16CbLow);

    /* AF STATISTICS CONIFG START */

    hi_vi_af_enable_write(IspDev, pstStatCfg->stFocusCfg.stConfig.bEnable);
    hi_vi_af_iir0_en0_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0]);
    hi_vi_af_iir0_en1_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1]);
    hi_vi_af_iir0_en2_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2]);
    hi_vi_af_iir1_en0_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0]);
    hi_vi_af_iir1_en1_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1]);
    hi_vi_af_iir1_en2_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2]);
    hi_vi_af_fvmode_peak_write(IspDev, pstStatCfg->stFocusCfg.stConfig.enPeakMode);
    hi_vi_af_fvmode_squ_write(IspDev, pstStatCfg->stFocusCfg.stConfig.enSquMode);
    hi_vi_af_hwnd_write(IspDev, pstStatCfg->stFocusCfg.stConfig.u16Hwnd);
    hi_vi_af_vwnd_write(IspDev, pstStatCfg->stFocusCfg.stConfig.u16Vwnd);
    hi_vi_af_iir0_gain0_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
    hi_vi_af_iir1_gain0_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
    hi_vi_af_iir0_gain1_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1]);
    hi_vi_af_iir1_gain1_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1]);
    hi_vi_af_iir0_gain2_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2]);
    hi_vi_af_iir1_gain2_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2]);
    hi_vi_af_iir0_gain3_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3]);
    hi_vi_af_iir1_gain3_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3]);
    hi_vi_af_iir0_gain4_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4]);
    hi_vi_af_iir1_gain4_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4]);
    hi_vi_af_iir0_gain5_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5]);
    hi_vi_af_iir1_gain5_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5]);
    hi_vi_af_iir0_gain6_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6]);
    hi_vi_af_iir1_gain6_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6]);
    hi_vi_af_iir0_shift0_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0]);
    hi_vi_af_iir0_shift1_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1]);
    hi_vi_af_iir0_shift2_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2]);
    hi_vi_af_iir0_shift3_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3]);
    hi_vi_af_iir1_shift0_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0]);
    hi_vi_af_iir1_shift1_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1]);
    hi_vi_af_iir1_shift2_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2]);
    hi_vi_af_iir1_shift3_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3]);
    hi_vi_af_fir0_h0_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0]);
    hi_vi_af_fir1_h0_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0]);
    hi_vi_af_fir0_h1_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1]);
    hi_vi_af_fir1_h1_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1]);
    hi_vi_af_fir0_h2_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2]);
    hi_vi_af_fir1_h2_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2]);
    hi_vi_af_fir0_h3_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3]);
    hi_vi_af_fir1_h3_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3]);
    hi_vi_af_fir0_h4_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4]);
    hi_vi_af_fir1_h4_write(IspDev, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4]);

    /* ds */
    hi_vi_af_iir0_ds_en_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand);
    hi_vi_af_iir1_ds_en_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand);

    /* PLG and PLS */
    for ( u8I = 0 ; u8I < 2 ; u8I++ )
    {
        pstIIR = u8I? &(pstStatCfg->stFocusCfg.stHParam_IIR1) : &(pstStatCfg->stFocusCfg.stHParam_IIR0);

        u8Shift0 =  pstIIR->au16IIRShift[0];
        s16G0 = pstIIR->as16IIRGain[0];
        s16G1 = pstIIR->as16IIRGain[1];
        s16G2 = pstIIR->as16IIRGain[2];

        f32Pl = (512.f / (512-2*s16G1-s16G2) * s16G0) /  (1 << u8Shift0);
        f32Temp = f32Pl;
        f32Temp = MIN2(7-floor(log(f32Temp)/log(2)), 7);

        u32Pls = (HI_U32)f32Temp;
        u32Plg = (HI_U32)((f32Pl * (1 << u32Pls)) + 0.5);

        if (0 == u8I)
        {
            hi_vi_af_iir0_pls_write(IspDev, u32Pls);
            hi_vi_af_iir0_plg_write(IspDev, u32Plg);
        }
        else
        {
            hi_vi_af_iir1_pls_write(IspDev, u32Pls);
            hi_vi_af_iir1_plg_write(IspDev, u32Plg);
        }
    }

    /* AF crop */
    hi_vi_af_crop_en_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable);
    //if (pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable)
    {
        hi_vi_af_crop_posx_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stCrop.u16X);
        hi_vi_af_crop_posy_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y);
        hi_vi_af_crop_hsize_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stCrop.u16W);
        hi_vi_af_crop_vsize_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stCrop.u16H);
    }

    /* AF raw cfg */
    hi_vi_af_pos_write(IspDev, ~pstStatCfg->stFocusCfg.stConfig.enStatisticsPos);
    hi_vi_af_raw_mode_write(IspDev, pstStatCfg->stFocusCfg.stConfig.enStatisticsPos);
    //if (ISP_AF_STATISTICS_RAW == pstStatCfg->stFocusCfg.stConfig.enStatisticsPos)
    {
        hi_vi_af_sqrt_en_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stRawCfg.bGammaEn);
        hi_vi_af_bayer_mode_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern);
        hi_vi_af_offset_en_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn);
        //if (pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn)
        {
            hi_vi_af_offset_gr_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GrOffset);
            hi_vi_af_offset_gb_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GbOffset);
        }
    }

    /* AF pre median filter */
    hi_vi_af_median_en_write(IspDev, pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn);
    //if (pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn)
    {
        hi_vi_af_median_th_write(IspDev, 0xffff - pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength);
    }

    /* level depend gain */
    hi_vi_af_iir0_ldg_en_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn)
    {
        hi_vi_af_iir0_ldg_th_l_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow    );
        hi_vi_af_iir0_ldg_th_h_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh   );
        hi_vi_af_iir0_ldg_slp_l_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow   );
        hi_vi_af_iir0_ldg_slp_h_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh  );
        hi_vi_af_iir0_ldg_gain_l_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow  );
        hi_vi_af_iir0_ldg_gain_h_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh );

    }

    hi_vi_af_iir1_ldg_en_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn)
    {
        hi_vi_af_iir1_ldg_th_l_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow    );
        hi_vi_af_iir1_ldg_th_h_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh   );
        hi_vi_af_iir1_ldg_slp_l_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow   );
        hi_vi_af_iir1_ldg_slp_h_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh  );
        hi_vi_af_iir1_ldg_gain_l_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow  );
        hi_vi_af_iir1_ldg_gain_h_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh );
    }

    hi_vi_af_fir0_ldg_en_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn)
    {
        hi_vi_af_fir0_ldg_th_l_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow    );
        hi_vi_af_fir0_ldg_th_h_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh   );
        hi_vi_af_fir0_ldg_slp_l_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow   );
        hi_vi_af_fir0_ldg_slp_h_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh  );
        hi_vi_af_fir0_ldg_gain_l_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow  );
        hi_vi_af_fir0_ldg_gain_h_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh );
    }

    hi_vi_af_fir1_ldg_en_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn);
    //if (pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn)
    {
        hi_vi_af_fir1_ldg_th_l_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow    );
        hi_vi_af_fir1_ldg_th_h_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh   );
        hi_vi_af_fir1_ldg_slp_l_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow   );
        hi_vi_af_fir1_ldg_slp_h_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh  );
        hi_vi_af_fir1_ldg_gain_l_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow  );
        hi_vi_af_fir1_ldg_gain_h_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh );
    }

    /* AF coring */
    hi_vi_af_iir0_cor_th_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th   );
    hi_vi_af_iir0_cor_slp_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp  );
    hi_vi_af_iir0_cor_peak_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt  );

    hi_vi_af_iir1_cor_th_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th   );
    hi_vi_af_iir1_cor_slp_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp  );
    hi_vi_af_iir1_cor_peak_write(IspDev, pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt  );

    hi_vi_af_fir0_cor_th_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th   );
    hi_vi_af_fir0_cor_slp_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp  );
    hi_vi_af_fir0_cor_peak_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt  );

    hi_vi_af_fir1_cor_th_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th   );
    hi_vi_af_fir1_cor_slp_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp  );
    hi_vi_af_fir1_cor_peak_write(IspDev, pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt  );

    /* high luma counter */
    hi_vi_af_highluma_th_write(IspDev, pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh);

    /* AF output shift */
    hi_vi_af_acc_shift_h0_write(IspDev, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0]);
    hi_vi_af_acc_shift_h1_write(IspDev, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1]);
    hi_vi_af_acc_shift_v0_write(IspDev, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0]);
    hi_vi_af_acc_shift_v1_write(IspDev, pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1]);
    hi_vi_af_acc_shift_y_write(IspDev, pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY);
    hi_vi_af_cnt_shift_highluma_write(IspDev, pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift);

    hi_vi_af_hsize_write(IspDev, pstStatCfg->stFocusCfg.stConfig.u16Hsize - 1);
    hi_vi_af_vsize_write(IspDev, pstStatCfg->stFocusCfg.stConfig.u16Vsize - 1);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetStatisticsConfig(ISP_DEV IspDev, ISP_STATISTICS_CFG_S *pstStatCfg)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    HI_U8  u8Tmp;

    pstStatCfg->unKey.u32Key = hi_ext_system_statistics_ctrl_read(IspDev);
    pstStatCfg->stAECfg.au8HistThresh[0] = hi_isp_metering_hist_thresh_0_1_read(IspDev);
    pstStatCfg->stAECfg.au8HistThresh[1] = hi_isp_metering_hist_thresh_1_2_read(IspDev);
    pstStatCfg->stAECfg.au8HistThresh[2] = hi_isp_metering_hist_thresh_3_4_read(IspDev);
    pstStatCfg->stAECfg.au8HistThresh[3] = hi_isp_metering_hist_thresh_4_5_read(IspDev);

    u8Tmp = hi_isp_top_ae_switch_read(IspDev);
    if (u8Tmp == HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_STATIC_WB;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SWITCH_AFTER_TPG_CHANNEL1)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_FROM_SENSOR_CHANNEL_1;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SWITCH_AFTER_SHADING)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_SHADING;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SWITCH_AFTER_WDR_STITCH)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_WDR_STITCH;
    }
    else
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_SWITCH_BUTT;
    }

    u8Tmp = hi_isp_top_aesum_switch_read(IspDev);
    if (u8Tmp == HI_ISP_TOP_AESUM_SWITCH_AFTER_STATIC_WB)
    {
        pstStatCfg->stAECfg.enAESumSwitch = ISP_AE_AFTER_STATIC_WB;
    }
    else if (u8Tmp == HI_ISP_TOP_AESUM_SWITCH_AFTER_TPG_CHANNEL1)
    {
        pstStatCfg->stAECfg.enAESumSwitch = ISP_AE_FROM_SENSOR_CHANNEL_1;
    }
    else if (u8Tmp == HI_ISP_TOP_AESUM_SWITCH_AFTER_SHADING)
    {
        pstStatCfg->stAECfg.enAESumSwitch = ISP_AE_AFTER_SHADING;
    }
    else if (u8Tmp == HI_ISP_TOP_AESUM_SWITCH_AFTER_WDR_STITCH)
    {
        pstStatCfg->stAECfg.enAESumSwitch = ISP_AE_AFTER_WDR_STITCH;
    }
    else
    {
        pstStatCfg->stAECfg.enAESumSwitch = ISP_AE_SWITCH_BUTT;
    }

    u8Tmp = hi_isp_top_histogram_switch_read(IspDev);
    if (u8Tmp == HI_ISP_TOP_HISTOGRAM_SWITCH_SAME_AS_AE)
    {
        pstStatCfg->stAECfg.enHistSwitch = ISP_AE_HIST_SAME_AS_AE;
    }
    else if (u8Tmp == HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_TPG_CHANNEL1)
    {
        pstStatCfg->stAECfg.enHistSwitch = ISP_AE_HIST_FROM_SENSOR_CHANNEL_1;
    }
    else if (u8Tmp == HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_SHADING)
    {
        pstStatCfg->stAECfg.enHistSwitch = ISP_AE_HIST_AFTER_SHADING;
    }
    else if (u8Tmp == HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_WDR_STITCH)
    {
        pstStatCfg->stAECfg.enHistSwitch = ISP_AE_HIST_AFTER_WDR_STITCH;
    }
    else
    {
        pstStatCfg->stAECfg.enHistSwitch = ISP_AE_HIST_SWITCH_BUTT;
    }

    u8Tmp = hi_isp_top_histogram_pe1_switch_read(IspDev);
    pstStatCfg->stAECfg.enPreHistSwitch = (u8Tmp >= ISP_AE_PRE_HIST_BUTT) ? ISP_AE_PRE_HIST_BUTT : u8Tmp;

    pstStatCfg->stAECfg.stHistConfig.u8HistSkipX = hi_isp_metering_skip_x_read(IspDev);
    pstStatCfg->stAECfg.stHistConfig.u8HistSkipY = hi_isp_metering_skip_y_read(IspDev);
    pstStatCfg->stAECfg.stHistConfig.u8HistOffsetX = hi_isp_metering_offset_x_read(IspDev);
    pstStatCfg->stAECfg.stHistConfig.u8HistOffsetY = hi_isp_metering_offset_y_read(IspDev);

    pstStatCfg->stWBCfg.stRGBCfg.u16WhiteLevel = hi_isp_metering_white_level_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16BlackLevel = hi_isp_metering_black_level_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CrMax = hi_isp_metering_cr_ref_max_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CrMin = hi_isp_metering_cr_ref_min_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CbMax = hi_isp_metering_cb_ref_max_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CbMin = hi_isp_metering_cb_ref_min_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CrHigh = hi_isp_metering_cr_ref_high_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CrLow = hi_isp_metering_cr_ref_low_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CbHigh = hi_isp_metering_cb_ref_high_awb_read(IspDev);
    pstStatCfg->stWBCfg.stRGBCfg.u16CbLow = hi_isp_metering_cb_ref_low_awb_read(IspDev);

    pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel = hi_ext_system_bayer_statistics_whitelevel_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel = hi_ext_system_bayer_statistics_blacklevel_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMax = hi_isp_metering_cr_ref_max_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMin = hi_isp_metering_cr_ref_min_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMax = hi_isp_metering_cb_ref_max_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMin = hi_isp_metering_cb_ref_min_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh = hi_isp_metering_cr_ref_high_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CrLow = hi_isp_metering_cr_ref_low_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh = hi_isp_metering_cb_ref_high_awb_sum_read(IspDev);
    pstStatCfg->stWBCfg.stBayerCfg.u16CbLow = hi_isp_metering_cb_ref_low_awb_sum_read(IspDev);

    /* AF STATISTICS CONIFG START */
    pstStatCfg->stFocusCfg.stConfig.bEnable = hi_vi_af_enable_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0] = hi_vi_af_iir0_en0_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1] = hi_vi_af_iir0_en1_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2] = hi_vi_af_iir0_en2_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0] = hi_vi_af_iir1_en0_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1] = hi_vi_af_iir1_en1_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2] = hi_vi_af_iir1_en2_read(IspDev);
    pstStatCfg->stFocusCfg.stConfig.enPeakMode = hi_vi_af_fvmode_peak_read(IspDev);
    pstStatCfg->stFocusCfg.stConfig.enSquMode = hi_vi_af_fvmode_squ_read(IspDev);
    pstStatCfg->stFocusCfg.stConfig.u16Hwnd = hi_vi_af_hwnd_read(IspDev);
    pstStatCfg->stFocusCfg.stConfig.u16Vwnd = hi_vi_af_vwnd_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] = (HI_S16)hi_vi_af_iir0_gain0_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] = (HI_S16)hi_vi_af_iir1_gain0_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[1] = (HI_S16)hi_vi_af_iir0_gain1_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[1] = (HI_S16)hi_vi_af_iir1_gain1_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[2] = (HI_S16)hi_vi_af_iir0_gain2_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[2] = (HI_S16)hi_vi_af_iir1_gain2_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[3] = (HI_S16)hi_vi_af_iir0_gain3_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[3] = (HI_S16)hi_vi_af_iir1_gain3_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[4] = (HI_S16)hi_vi_af_iir0_gain4_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[4] = (HI_S16)hi_vi_af_iir1_gain4_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[5] = (HI_S16)hi_vi_af_iir0_gain5_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[5] = (HI_S16)hi_vi_af_iir1_gain5_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[6] = (HI_S16)hi_vi_af_iir0_gain6_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[6] = (HI_S16)hi_vi_af_iir1_gain6_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[0] = hi_vi_af_iir0_shift0_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[1] = hi_vi_af_iir0_shift1_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[2] = hi_vi_af_iir0_shift2_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[3] = hi_vi_af_iir0_shift3_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[0] = hi_vi_af_iir1_shift0_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[1] = hi_vi_af_iir1_shift1_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[2] = hi_vi_af_iir1_shift2_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[3] = hi_vi_af_iir1_shift3_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[0] = (HI_S16)hi_vi_af_fir0_h0_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[0] = (HI_S16)hi_vi_af_fir1_h0_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[1] = (HI_S16)hi_vi_af_fir0_h1_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[1] = (HI_S16)hi_vi_af_fir1_h1_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[2] = (HI_S16)hi_vi_af_fir0_h2_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[2] = (HI_S16)hi_vi_af_fir1_h2_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[3] = (HI_S16)hi_vi_af_fir0_h3_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[3] = (HI_S16)hi_vi_af_fir1_h3_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[4] = (HI_S16)hi_vi_af_fir0_h4_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[4] = (HI_S16)hi_vi_af_fir1_h4_read(IspDev);

    /* ds */
    pstStatCfg->stFocusCfg.stHParam_IIR0.bNarrowBand = hi_vi_af_iir0_ds_en_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.bNarrowBand = hi_vi_af_iir1_ds_en_read(IspDev);

    /* AF crop */
    pstStatCfg->stFocusCfg.stConfig.stCrop.bEnable = hi_vi_af_crop_en_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16X = hi_vi_af_crop_posx_read(IspDev);
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16Y = hi_vi_af_crop_posy_read(IspDev);
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16W = hi_vi_af_crop_hsize_read(IspDev);
        pstStatCfg->stFocusCfg.stConfig.stCrop.u16H = hi_vi_af_crop_vsize_read(IspDev);
    }

    /* AF raw cfg */
    pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = ~hi_vi_af_pos_read(IspDev);
    pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = hi_vi_af_raw_mode_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stConfig.stRawCfg.bGammaEn = hi_vi_af_sqrt_en_read(IspDev);
        pstStatCfg->stFocusCfg.stConfig.stRawCfg.enPattern = hi_vi_af_bayer_mode_read(IspDev);
        pstStatCfg->stFocusCfg.stConfig.stRawCfg.bOffsetEn = hi_vi_af_offset_en_read(IspDev);
        {
            pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GrOffset = hi_vi_af_offset_gr_read(IspDev);
            pstStatCfg->stFocusCfg.stConfig.stRawCfg.u16GbOffset = hi_vi_af_offset_gb_read(IspDev);
        }
    }

    /* AF pre median filter */
    pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.bEn = hi_vi_af_median_en_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stConfig.stPreFltCfg.u16strength = 0xffff - hi_vi_af_median_th_read(IspDev);
    }

    /* level depend gain */
    pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.bLdEn = hi_vi_af_iir0_ldg_en_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThLow    = hi_vi_af_iir0_ldg_th_l_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16ThHigh   = hi_vi_af_iir0_ldg_th_h_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpLow   = hi_vi_af_iir0_ldg_slp_l_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16SlpHigh  = hi_vi_af_iir0_ldg_slp_h_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainLow  = hi_vi_af_iir0_ldg_gain_l_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR0.stLd.u16GainHigh = hi_vi_af_iir0_ldg_gain_h_read(IspDev);

    }

    pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.bLdEn = hi_vi_af_iir1_ldg_en_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThLow    = hi_vi_af_iir1_ldg_th_l_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16ThHigh   = hi_vi_af_iir1_ldg_th_h_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpLow   = hi_vi_af_iir1_ldg_slp_l_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16SlpHigh  = hi_vi_af_iir1_ldg_slp_h_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainLow  = hi_vi_af_iir1_ldg_gain_l_read(IspDev);
        pstStatCfg->stFocusCfg.stHParam_IIR1.stLd.u16GainHigh = hi_vi_af_iir1_ldg_gain_h_read(IspDev);
    }

    pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.bLdEn = hi_vi_af_fir0_ldg_en_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThLow    = hi_vi_af_fir0_ldg_th_l_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16ThHigh   = hi_vi_af_fir0_ldg_th_h_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpLow   = hi_vi_af_fir0_ldg_slp_l_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16SlpHigh  = hi_vi_af_fir0_ldg_slp_h_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainLow  = hi_vi_af_fir0_ldg_gain_l_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR0.stLd.u16GainHigh = hi_vi_af_fir0_ldg_gain_h_read(IspDev);
    }

    pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.bLdEn = hi_vi_af_fir1_ldg_en_read(IspDev);
    {
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThLow    = hi_vi_af_fir1_ldg_th_l_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16ThHigh   = hi_vi_af_fir1_ldg_th_h_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpLow   = hi_vi_af_fir1_ldg_slp_l_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16SlpHigh  = hi_vi_af_fir1_ldg_slp_h_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainLow  = hi_vi_af_fir1_ldg_gain_l_read(IspDev);
        pstStatCfg->stFocusCfg.stVParam_FIR1.stLd.u16GainHigh = hi_vi_af_fir1_ldg_gain_h_read(IspDev);
    }

    /* AF coring */
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Th  = hi_vi_af_iir0_cor_th_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Slp = hi_vi_af_iir0_cor_slp_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR0.stCoring.u16Lmt = hi_vi_af_iir0_cor_peak_read(IspDev);

    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Th  = hi_vi_af_iir1_cor_th_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Slp = hi_vi_af_iir1_cor_slp_read(IspDev);
    pstStatCfg->stFocusCfg.stHParam_IIR1.stCoring.u16Lmt = hi_vi_af_iir1_cor_peak_read(IspDev);

    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Th  = hi_vi_af_fir0_cor_th_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Slp = hi_vi_af_fir0_cor_slp_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR0.stCoring.u16Lmt = hi_vi_af_fir0_cor_peak_read(IspDev);

    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Th  = hi_vi_af_fir1_cor_th_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Slp = hi_vi_af_fir1_cor_slp_read(IspDev);
    pstStatCfg->stFocusCfg.stVParam_FIR1.stCoring.u16Lmt = hi_vi_af_fir1_cor_peak_read(IspDev);

    /* high luma counter */
    pstStatCfg->stFocusCfg.stConfig.u16HighLumaTh = hi_vi_af_highluma_th_read(IspDev);

    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0] = hi_vi_af_acc_shift_h0_read(IspDev);
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1] = hi_vi_af_acc_shift_h1_read(IspDev);
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0] = hi_vi_af_acc_shift_v0_read(IspDev);
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1] = hi_vi_af_acc_shift_v1_read(IspDev);
    pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY = hi_vi_af_acc_shift_y_read(IspDev);
    pstStatCfg->stFocusCfg.stFVParam.u16HlCntShift = hi_vi_af_cnt_shift_highluma_read(IspDev);
    pstStatCfg->stFocusCfg.stConfig.u16Hsize = hi_vi_af_hsize_read(IspDev) + 1;
    pstStatCfg->stFocusCfg.stConfig.u16Vsize = hi_vi_af_vsize_read(IspDev) + 1;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetStatistics(ISP_DEV IspDev, ISP_STATISTICS_S *pstStat)
{
    HI_S32 i, j, k;
    HI_U32 u32Sum;
    //HI_U32 u32Hist10, u32Hist43;
    HI_U32 u32HighBit = 0, u32LowBit = 0;
    ISP_STATISTICS_CTRL_U unStatKey;
    ISP_STAT_INFO_S stActStatInfo;
    ISP_STAT_S *pstIspActStat;
    HI_S32 s32Ret;
    ISP_MODE_E uIspMode;
    HI_U32  u32PhyAddr;
    HI_U32  u32Size;
    HI_VOID *pVirtAddr;
    HI_U8 u8WDRMode;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    unStatKey.u32Key = hi_ext_system_statistics_ctrl_read(IspDev);

    uIspMode = (ISP_MODE_E)hi_ext_top_isp_working_mode_read(IspDev);
    if(ISP_MODE_STITCH == uIspMode)
    {
        u32PhyAddr = hi_ext_isp_stitch_buffer_phyaddr_read(IspDev);
        u32Size = hi_ext_isp_stitch_buffer_size_read(IspDev);

        pVirtAddr = HI_MPI_SYS_MmapCache(u32PhyAddr, u32Size);

        if(NULL == pVirtAddr)
        {
            return HI_ERR_ISP_NULL_PTR;
        }

        /* AE statistics */
        if(unStatKey.bit1AeStat1)
        {
            for(i = 0; i < HIST_5_NUM; i++)
            {
                pstStat->stAEStat.au16Hist5Value[i] = ((ISP_STAT_S *)pVirtAddr)->stAeStat1.au16MeteringHist[i];
            }
        }

        if (unStatKey.bit1AeStat3)
        {
            for(i = 0; i < HIST_1024_NUM; i++)
            {
                pstStat->stAEStat.au32Hist1024Value[i] = ((ISP_STAT_S *)pVirtAddr)->stAeStat3.au32HistogramMemArray[i];
            }
        }

        if (unStatKey.bit1AeStat4)
        {
            pstStat->stAEStat.au16GlobalAvg[0] = ((ISP_STAT_S *)pVirtAddr)->stAeStat4.u16GlobalAvgR;
            pstStat->stAEStat.au16GlobalAvg[1] = ((ISP_STAT_S *)pVirtAddr)->stAeStat4.u16GlobalAvgGr;
            pstStat->stAEStat.au16GlobalAvg[2] = ((ISP_STAT_S *)pVirtAddr)->stAeStat4.u16GlobalAvgGb;
            pstStat->stAEStat.au16GlobalAvg[3] = ((ISP_STAT_S *)pVirtAddr)->stAeStat4.u16GlobalAvgB;
        }

        if (unStatKey.bit1AeStat5)
        {
            for(i = 0;i < AE_ZONE_ROW ; i++)
            {
                for(j=0;j< AE_ZONE_COLUMN ;j++)
                {
                    for(k = 0; k < ISP_BAYER_CHN_NUM; k++)
                    {
                        pstStat->stAEStat.au16ZoneAvg[i][j][k] = ((ISP_STAT_S *)pVirtAddr)->stAeStat5.au16ZoneAvg[i][j][k];
                    }
                }
            }
        }

        if (unStatKey.bit1AeStat6)
        {
            for(i = 0; i < HIST_256_NUM; i++)
            {
                pstStat->stAEStat.au32PreHistogram1[i] = ((ISP_STAT_S *)pVirtAddr)->stAeStat6.au32PreHistogram0[i];
                pstStat->stAEStat.au32PreHistogram2[i] = ((ISP_STAT_S *)pVirtAddr)->stAeStat6.au32PreHistogram1[i];
                pstStat->stAEStat.au32PreHistogram3[i] = ((ISP_STAT_S *)pVirtAddr)->stAeStat6.au32PreHistogram2[i];
                pstStat->stAEStat.au32PreHistogram4[i] = ((ISP_STAT_S *)pVirtAddr)->stAeStat6.au32PreHistogram3[i];
            }
        }

        /* AWB statistics */
#if 0
        if (unStatKey.bit1AwbStat1)
        {
            pstStat->stWBStat.stRGBStatistics.u16GlobalGR = ((ISP_STAT_S *)pVirtAddr)->stAwbStat1.u16MeteringAwbRg;
            pstStat->stWBStat.stRGBStatistics.u16GlobalGB = ((ISP_STAT_S *)pVirtAddr)->stAwbStat1.u16MeteringAwbBg;
            pstStat->stWBStat.stRGBStatistics.u32GlobalSum = ((ISP_STAT_S *)pVirtAddr)->stAwbStat1.u32MeteringAwbSum;
        }

        if (unStatKey.bit1AwbStat2)
        {
            for(i = 0; i < AWB_ZONE_NUM; i++)
            {
                pstStat->stWBStat.stRGBStatistics.au16ZoneGR[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat2.au16MeteringMemArrayRg[i];
                pstStat->stWBStat.stRGBStatistics.au16ZoneGB[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat2.au16MeteringMemArrayBg[i];
                pstStat->stWBStat.stRGBStatistics.au32ZoneSum[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat2.au16MeteringMemArraySum[i];
            }
        }
#endif
        if (unStatKey.bit1AwbStat3)
        {
            pstStat->stWBStat.stBayerStatistics.u16GlobalR = ((ISP_STAT_S *)pVirtAddr)->stAwbStat3.u16MeteringAwbAvgR;
            pstStat->stWBStat.stBayerStatistics.u16GlobalG = ((ISP_STAT_S *)pVirtAddr)->stAwbStat3.u16MeteringAwbAvgG;
            pstStat->stWBStat.stBayerStatistics.u16GlobalB = ((ISP_STAT_S *)pVirtAddr)->stAwbStat3.u16MeteringAwbAvgB;
            pstStat->stWBStat.stBayerStatistics.u16CountAll = ((ISP_STAT_S *)pVirtAddr)->stAwbStat3.u16MeteringAwbCountAll;
            pstStat->stWBStat.stBayerStatistics.u16CountMin = ((ISP_STAT_S *)pVirtAddr)->stAwbStat3.u16MeteringAwbCountMin;
            pstStat->stWBStat.stBayerStatistics.u16CountMax = ((ISP_STAT_S *)pVirtAddr)->stAwbStat3.u16MeteringAwbCountMax;
        }

        if (unStatKey.bit1AwbStat4)
        {
            for(i = 0; i < AWB_ZONE_NUM; i++)
            {
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgR[i];
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgG[i];
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat4.au16MeteringMemArrayAvgB[i];
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountAll[i];
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountMin[i];
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i] = ((ISP_STAT_S *)pVirtAddr)->stAwbStat4.au16MeteringMemArrayCountMax[i];
            }
        }

        /* AF statistics */
        if (unStatKey.bit1AfStat)
        {
            HI_U32 u32Zone = hi_area_block_distribute_data_read_af(0);
            HI_U8 u8Col = (u32Zone & 0x1F);
            HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);

            u8Col = (u8Col > AF_ZONE_COLUMN) ? AF_ZONE_COLUMN : u8Col;
            u8Row = (u8Row > AF_ZONE_ROW) ? AF_ZONE_ROW : u8Row;

            for (i = 0; i < u8Row; i++)
            {
                for (j = 0; j < u8Col; j++)
                {
                    pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = ((ISP_STAT_S *)pVirtAddr)->stAfStat.stZoneMetrics[i][j].u16v1;
                    pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = ((ISP_STAT_S *)pVirtAddr)->stAfStat.stZoneMetrics[i][j].u16h1;
                    pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = ((ISP_STAT_S *)pVirtAddr)->stAfStat.stZoneMetrics[i][j].u16v2;
                    pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = ((ISP_STAT_S *)pVirtAddr)->stAfStat.stZoneMetrics[i][j].u16h2;
                    pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = ((ISP_STAT_S *)pVirtAddr)->stAfStat.stZoneMetrics[i][j].u16y;
                    pstStat->stFocusStat.stZoneMetrics[i][j].u16HlCnt = ((ISP_STAT_S *)pVirtAddr)->stAfStat.stZoneMetrics[i][j].u16HlCnt;
                }
            }
        }

        if (HI_NULL != pVirtAddr)
        {
            HI_MPI_SYS_Munmap(pVirtAddr, u32Size);
            pVirtAddr = HI_NULL;
        }

        return HI_SUCCESS;
    }

    if (unStatKey.bit1AeStat1)
    {
        pstStat->stAEStat.au16Hist5Value[0] = hi_isp_metering_hist_0_read(IspDev);
        pstStat->stAEStat.au16Hist5Value[1] = hi_isp_metering_hist_1_read(IspDev);
        pstStat->stAEStat.au16Hist5Value[3] = hi_isp_metering_hist_3_read(IspDev);
        pstStat->stAEStat.au16Hist5Value[4] = hi_isp_metering_hist_4_read(IspDev);

        u32Sum = pstStat->stAEStat.au16Hist5Value[0]
            + pstStat->stAEStat.au16Hist5Value[1]
            + pstStat->stAEStat.au16Hist5Value[3]
            + pstStat->stAEStat.au16Hist5Value[4];
        if(u32Sum > 0xFFFF)
        {
            u32Sum = 0xFFFF;
        }
        pstStat->stAEStat.au16Hist5Value[2] = 0xFFFF - u32Sum;
    }

#if 0
    if (unStatKey.bit1AeStat2)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j < AE_ZONE_COLUMN ;j++)
            {
                u32Hist10 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 0);
                u32Hist43 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 1);

                pstStat->stAEStat.au16ZoneHist5Value[i][j][0] = (HI_U16)(u32Hist10 & 0xffff);
                pstStat->stAEStat.au16ZoneHist5Value[i][j][1] = (HI_U16)((u32Hist10 & 0xffff0000) >> 16);
                pstStat->stAEStat.au16ZoneHist5Value[i][j][3] = (HI_U16)(u32Hist43 & 0xffff);
                pstStat->stAEStat.au16ZoneHist5Value[i][j][4] = (HI_U16)((u32Hist43 & 0xffff0000) >> 16);

                u32Sum = pstStat->stAEStat.au16ZoneHist5Value[i][j][0]
                    + pstStat->stAEStat.au16ZoneHist5Value[i][j][1]
                    + pstStat->stAEStat.au16ZoneHist5Value[i][j][3]
                    + pstStat->stAEStat.au16ZoneHist5Value[i][j][4];
                if(u32Sum > 0xFFFF)
                {
                    u32Sum = 0xFFFF;
                }

                pstStat->stAEStat.au16ZoneHist5Value[i][j][2] = 0xFFFF - u32Sum;
            }
        }
    }
#endif

    if (unStatKey.bit1AeStat3)
    {
        for(i = 0; i < 1024; i++)
        {
            pstStat->stAEStat.au32Hist1024Value[i] = (HI_U16)(hi_histogram_mem_array_data_read(IspDev, i) & 0xffff);
            u32HighBit = pstStat->stAEStat.au32Hist1024Value[i] >> 12;
            u32LowBit  = pstStat->stAEStat.au32Hist1024Value[i] & 0xfff;
            pstStat->stAEStat.au32Hist1024Value[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));
        }
    }

    if (unStatKey.bit1AeStat4)
    {
        pstStat->stAEStat.au16GlobalAvg[0]  = hi_isp_metering_sum_r_read(IspDev);
        pstStat->stAEStat.au16GlobalAvg[1] = hi_isp_metering_sum_rg_read(IspDev);
        pstStat->stAEStat.au16GlobalAvg[2] = hi_isp_metering_sum_bg_read(IspDev);
        pstStat->stAEStat.au16GlobalAvg[3]  = hi_isp_metering_sum_b_read(IspDev);
    }

    if (unStatKey.bit1AeStat5)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
                pstStat->stAEStat.au16ZoneAvg[i][j][0] = hi_ae_sum_array_data_read_r(IspDev, i*AE_ZONE_COLUMN+j);   /*R */
                pstStat->stAEStat.au16ZoneAvg[i][j][1] = hi_ae_sum_array_data_read_gr(IspDev, i*AE_ZONE_COLUMN+j);  /*Gr*/
                pstStat->stAEStat.au16ZoneAvg[i][j][2] = hi_ae_sum_array_data_read_gb(IspDev, i*AE_ZONE_COLUMN+j);  /*Gb*/
                pstStat->stAEStat.au16ZoneAvg[i][j][3] = hi_ae_sum_array_data_read_b(IspDev, i*AE_ZONE_COLUMN+j);   /*B */
            }
        }
    }

    if (unStatKey.bit1AeStat6)
    {
        for(i = 0; i < 256; i++)
        {
            pstStat->stAEStat.au32PreHistogram1[i] = (HI_U16)(hi_histogram_1_mem_array_data_read(IspDev, i) & 0xffff);
            u32HighBit = pstStat->stAEStat.au32PreHistogram1[i] >> 12;
            u32LowBit  = pstStat->stAEStat.au32PreHistogram1[i] & 0xfff;
            pstStat->stAEStat.au32PreHistogram1[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));

            pstStat->stAEStat.au32PreHistogram2[i] = (HI_U16)(hi_histogram_2_mem_array_data_read(IspDev, i) & 0xffff);
            u32HighBit = pstStat->stAEStat.au32PreHistogram2[i] >> 12;
            u32LowBit  = pstStat->stAEStat.au32PreHistogram2[i] & 0xfff;
            pstStat->stAEStat.au32PreHistogram2[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));

            pstStat->stAEStat.au32PreHistogram3[i] = (HI_U16)(hi_histogram_3_mem_array_data_read(IspDev, i) & 0xffff);
            u32HighBit = pstStat->stAEStat.au32PreHistogram3[i] >> 12;
            u32LowBit  = pstStat->stAEStat.au32PreHistogram3[i] & 0xfff;
            pstStat->stAEStat.au32PreHistogram3[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));

            pstStat->stAEStat.au32PreHistogram4[i] = (HI_U16)(hi_histogram_4_mem_array_data_read(IspDev, i) & 0xffff);
            u32HighBit = pstStat->stAEStat.au32PreHistogram4[i] >> 12;
            u32LowBit  = pstStat->stAEStat.au32PreHistogram4[i] & 0xfff;
            pstStat->stAEStat.au32PreHistogram4[i] = (u32HighBit == 0) ? u32LowBit : ((4096 + u32LowBit) << (u32HighBit - 1));
        }
    }

    u8WDRMode = hi_ext_system_sensor_wdr_mode_read(IspDev);
    if((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode)))
    {
        memset(&pstStat->stAEStat.au32PreHistogram2, 0, sizeof(pstStat->stAEStat.au32PreHistogram2));
        memset(&pstStat->stAEStat.au32PreHistogram3, 0, sizeof(pstStat->stAEStat.au32PreHistogram3));
        memset(&pstStat->stAEStat.au32PreHistogram4, 0, sizeof(pstStat->stAEStat.au32PreHistogram4));
    }
    else if(IS_2to1_WDR_MODE(u8WDRMode))
    {
        memset(&pstStat->stAEStat.au32PreHistogram3, 0, sizeof(pstStat->stAEStat.au32PreHistogram3));
        memset(&pstStat->stAEStat.au32PreHistogram4, 0, sizeof(pstStat->stAEStat.au32PreHistogram4));
    }
    else if(IS_3to1_WDR_MODE(u8WDRMode))
    {
        memset(&pstStat->stAEStat.au32PreHistogram4, 0, sizeof(pstStat->stAEStat.au32PreHistogram4));
    }
    else
    {
    }

    if (unStatKey.bit1AwbStat1)
    {
        pstStat->stWBStat.stRGBStatistics.u16GlobalGR = hi_isp_metering_awb_rg_read(IspDev);
        pstStat->stWBStat.stRGBStatistics.u16GlobalGB = hi_isp_metering_awb_bg_read(IspDev);
        pstStat->stWBStat.stRGBStatistics.u32GlobalSum = hi_isp_metering_awb_sum_read(IspDev);
    }

    if (unStatKey.bit1AwbStat2)
    {
        for(i = 0; i < AWB_ZONE_NUM; i++)
        {
            HI_U32 u32BgRg = hi_metering_mem_array_data_read_bg_rg(IspDev, i);
            HI_U32 u32Sum = hi_metering_mem_array_data_read_sum(IspDev, i);

            pstStat->stWBStat.stRGBStatistics.au16ZoneGR[i] = u32BgRg & 0xfff;
            pstStat->stWBStat.stRGBStatistics.au16ZoneGB[i] = (u32BgRg >> 16) & 0xfff;
            pstStat->stWBStat.stRGBStatistics.au32ZoneSum[i] = u32Sum;
        }
    }

    if (unStatKey.bit1AwbStat3)
    {
#if 0
        HI_U16 u16BlackLevelR = (HI_U16)(hi_isp_offset_black_00_read(IspDev) >> 4);
        HI_U16 u16BlackLevelG = (HI_U16)(hi_isp_offset_black_01_read(IspDev) >> 4);
        HI_U16 u16BlackLevelB = (HI_U16)(hi_isp_offset_black_11_read(IspDev) >> 4);
#endif
        pstStat->stWBStat.stBayerStatistics.u16GlobalR = hi_isp_metering_avg_r_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16GlobalG = hi_isp_metering_avg_g_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16GlobalB = hi_isp_metering_avg_b_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16CountAll = hi_isp_metering_count_all_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16CountMin = hi_isp_metering_count_min_read(IspDev);
        pstStat->stWBStat.stBayerStatistics.u16CountMax= hi_isp_metering_count_max_read(IspDev);

#if 0
        if (0 != pstStat->stWBStat.stBayerStatistics.u16CountAll)
        {
            if ((pstStat->stWBStat.stBayerStatistics.u16GlobalR >= u16BlackLevelR) &&
                (pstStat->stWBStat.stBayerStatistics.u16GlobalG >= u16BlackLevelG) &&
                (pstStat->stWBStat.stBayerStatistics.u16GlobalB >= u16BlackLevelB))
            {
                pstStat->stWBStat.stBayerStatistics.u16GlobalR -= u16BlackLevelR;
                pstStat->stWBStat.stBayerStatistics.u16GlobalG -= u16BlackLevelG;
                pstStat->stWBStat.stBayerStatistics.u16GlobalB -= u16BlackLevelB;
            }
            else
            {
                pstStat->stWBStat.stBayerStatistics.u16GlobalR = 0;
                pstStat->stWBStat.stBayerStatistics.u16GlobalG = 0;
                pstStat->stWBStat.stBayerStatistics.u16GlobalB = 0;
                pstStat->stWBStat.stBayerStatistics.u16CountAll = 0x0;
                pstStat->stWBStat.stBayerStatistics.u16CountMin = 0xFFFF;
                pstStat->stWBStat.stBayerStatistics.u16CountMax= 0x0;
            }
        }
#endif
    }


    if (unStatKey.bit1AwbStat4)
    {
#if 0
        HI_U16 u16BlackLevelR = (HI_U16)(hi_isp_offset_black_00_read(IspDev) >> 4);
        HI_U16 u16BlackLevelG = (HI_U16)(hi_isp_offset_black_01_read(IspDev) >> 4);
        HI_U16 u16BlackLevelB = (HI_U16)(hi_isp_offset_black_11_read(IspDev) >> 4);
#endif

        for(i = 0; i < AWB_ZONE_NUM; i++)
        {
            HI_U32 u32GR = hi_awb_sum_array_data_read_avg_g_r(IspDev, i);
            HI_U32 u32ALLB = hi_awb_sum_array_data_read_all_avgb(IspDev, i);
            HI_U32 u32MINMAX = hi_awb_sum_array_data_read_min_max(IspDev, i);

            pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i] = u32GR & 0xffff;
            pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i] = (u32GR >> 16) & 0xffff;
            pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i] = u32ALLB & 0xffff;
            pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i] = (u32ALLB >> 16) & 0xffff;
            pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i] = u32MINMAX & 0xffff;
            pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i] = (u32MINMAX >> 16) & 0xffff;
        }
    }


    if (unStatKey.bit1AfStat)
    {

        HI_U32 u32Zone = hi_area_block_distribute_data_read_af(IspDev);
        HI_U8 u8Col = (u32Zone & 0x1F);
        HI_U8 u8Row = ((u32Zone & 0x1F00) >> 8);

        u8Col = (u8Col > AF_ZONE_COLUMN) ? AF_ZONE_COLUMN : u8Col;
        u8Row = (u8Row > AF_ZONE_ROW) ? AF_ZONE_ROW : u8Row;

        ISP_CHECK_OPEN(IspDev);

        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_ACT_GET, &stActStatInfo);
        if (s32Ret)
        {
            ISP_TRACE(HI_DBG_ERR, "Get Active Stat Buffer Err\n");
            return HI_ERR_ISP_NO_INT;
        }

        stActStatInfo.pVirtAddr = HI_MPI_SYS_MmapCache(stActStatInfo.u32PhyAddr, sizeof(ISP_STAT_S));
        if ( !stActStatInfo.pVirtAddr )
        {
            return HI_ERR_ISP_NULL_PTR;
        }
        pstIspActStat = (ISP_STAT_S *)stActStatInfo.pVirtAddr;

        for (i = 0; i < u8Row; i++)
        {
            for (j = 0; j < u8Col; j++)
            {
                pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = pstIspActStat->stAfStat.stZoneMetrics[i][j].u16v1;
                pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = pstIspActStat->stAfStat.stZoneMetrics[i][j].u16h1;
                pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = pstIspActStat->stAfStat.stZoneMetrics[i][j].u16v2;
                pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = pstIspActStat->stAfStat.stZoneMetrics[i][j].u16h2;
                pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = pstIspActStat->stAfStat.stZoneMetrics[i][j].u16y;
                pstStat->stFocusStat.stZoneMetrics[i][j].u16HlCnt = pstIspActStat->stAfStat.stZoneMetrics[i][j].u16HlCnt;
            }
        }

        HI_MPI_SYS_Munmap(stActStatInfo.pVirtAddr, sizeof(ISP_STAT_S));
    }


    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetISPRegAttr(ISP_DEV IspDev, ISP_REG_ATTR_S *pstIspRegAttr)
{
    HI_U32 u32IspBindAttr;
    ALG_LIB_S stAeLib;
    ALG_LIB_S stAwbLib;

    ISP_CHECK_DEV(IspDev);
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspRegAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u32IspBindAttr = hi_ext_system_bind_attr_read(IspDev);
    stAeLib.s32Id = (u32IspBindAttr >> 8) & 0xFF;
    stAwbLib.s32Id = u32IspBindAttr & 0xFF;

    (HI_VOID)pthread_mutex_lock(&pstIspCtx->stLock);

    VReg_Munmap(ISP_REG_BASE(IspDev), ISP_REG_SIZE_BIN);
    VReg_Munmap(ISP_VREG_BASE(IspDev), ISP_VREG_SIZE_BIN);
    VReg_Munmap(AE_LIB_VREG_BASE(stAeLib.s32Id), ALG_LIB_VREG_SIZE_BIN);
    VReg_Munmap(AWB_LIB_VREG_BASE(stAwbLib.s32Id), ALG_LIB_VREG_SIZE_BIN);

    pstIspRegAttr->u32IspRegAddr    = VReg_GetVirtAddr(ISP_REG_BASE(IspDev));
    pstIspRegAttr->u32IspRegSize    = ISP_REG_SIZE_BIN;
    pstIspRegAttr->u32IspExtRegAddr = VReg_GetVirtAddr(ISP_VREG_BASE(IspDev));
    pstIspRegAttr->u32IspExtRegSize = ISP_VREG_SIZE_BIN;
    pstIspRegAttr->u32AeExtRegAddr  = VReg_GetVirtAddr(AE_LIB_VREG_BASE(stAeLib.s32Id));
    pstIspRegAttr->u32AeExtRegSize  = ALG_LIB_VREG_SIZE_BIN;
    pstIspRegAttr->u32AwbExtRegAddr = VReg_GetVirtAddr(AWB_LIB_VREG_BASE(stAwbLib.s32Id));
    pstIspRegAttr->u32AwbExtRegSize = ALG_LIB_VREG_SIZE_BIN;

    (HI_VOID)pthread_mutex_unlock(&pstIspCtx->stLock);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDebug(ISP_DEV IspDev, const ISP_DEBUG_INFO_S *pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_BOOL(pstIspDebug->bDebugEn);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_DbgSet(IspDev, pstIspDebug);
}

HI_S32 HI_MPI_ISP_GetDebug(ISP_DEV IspDev, ISP_DEBUG_INFO_S * pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ISP_DbgGet(IspDev, pstIspDebug);
}

HI_S32 HI_MPI_ISP_SetModParam(ISP_DEV IspDev, const ISP_MOD_PARAM_S *pstIspModParam)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspModParam);

    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

	return 	ioctl(g_as32IspFd[IspDev], ISP_SET_MOD_PARAM, pstIspModParam);
}

HI_S32 HI_MPI_ISP_GetModParam(ISP_DEV IspDev, ISP_MOD_PARAM_S *pstIspModParam)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    return ioctl(g_as32IspFd[IspDev], ISP_GET_MOD_PARAM, pstIspModParam);
}

HI_S32 HI_MPI_ISP_SetSnapAttr(ISP_DEV IspDev, const ISP_SNAP_ATTR_S *pstSnapAttr)
{
    ISP_CHECK_DEV(IspDev);
	ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_POINTER(pstSnapAttr);
    ISP_CHECK_BOOL(pstSnapAttr->bUseInfoCCM);
    ISP_CHECK_MEM_INIT(IspDev);
	HI_U8 i;
	HI_S32 s32Ret;

    if (pstSnapAttr->enSnapType >= SNAP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Snap Type %d!\n", pstSnapAttr->enSnapType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if (pstSnapAttr->enSnapPipeMode >= ISP_SNAP_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid enSnapPipeMode %d!\n", pstSnapAttr->enSnapPipeMode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if (pstSnapAttr->stProParam.enOpType >= OP_TYPE_BUTT)
    {
        printf("Invalid Pro mode!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstSnapAttr->stProParam.u8ProFrameNum > PRO_MAX_FRAME_NUM) || (pstSnapAttr->stProParam.u8ProFrameNum < 1))
    {
        printf("Invalid u8ProFrameNum!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < pstSnapAttr->stProParam.u8ProFrameNum; i++)
    {
        if (pstSnapAttr->stProParam.stManual.au32ManSysgain[i] < 0x400)
        {
            printf("Invalid au32ManSysgain!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_ext_system_snap_type_write(IspDev, pstSnapAttr->enSnapType);
    hi_ext_system_snap_pipe_mode_write(IspDev, pstSnapAttr->enSnapPipeMode);
    hi_ext_system_snap_use_info_ccm_enable_write(IspDev, pstSnapAttr->bUseInfoCCM);

	hi_ext_system_pro_type_write(IspDev, pstSnapAttr->stProParam.enOpType);
    hi_ext_system_pro_frame_num_write(IspDev, pstSnapAttr->stProParam.u8ProFrameNum);
    for (i=0; i<PRO_MAX_FRAME_NUM; i++)
    {
        hi_ext_system_pro_exp_step_table_write(IspDev, i, pstSnapAttr->stProParam.stAuto.au16ProExpStep[i]);
        hi_ext_system_pro_man_exp_time_table_write(IspDev, i, pstSnapAttr->stProParam.stManual.au32ManExpTime[i]);
        hi_ext_system_pro_man_sysgain_table_write(IspDev, i, pstSnapAttr->stProParam.stManual.au32ManSysgain[i]);
    }

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_PRO_EXP_ATTR_SET, pstSnapAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_ISP_SetSnapAttr Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetSnapAttr(ISP_DEV IspDev, ISP_SNAP_ATTR_S *pstSnapAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSnapAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
	HI_U8 i;

    pstSnapAttr->enSnapType = hi_ext_system_snap_type_read(IspDev);
    pstSnapAttr->enSnapPipeMode = hi_ext_system_snap_pipe_mode_read(IspDev);
    pstSnapAttr->bUseInfoCCM = hi_ext_system_snap_use_info_ccm_enable_read(IspDev);
	pstSnapAttr->stProParam.enOpType = hi_ext_system_pro_type_read(IspDev);
    pstSnapAttr->stProParam.u8ProFrameNum = hi_ext_system_pro_frame_num_read(IspDev);
    for (i=0; i<PRO_MAX_FRAME_NUM; i++)
    {
        pstSnapAttr->stProParam.stAuto.au16ProExpStep[i] = hi_ext_system_pro_exp_step_table_read(IspDev, i);
        pstSnapAttr->stProParam.stManual.au32ManExpTime[i] = hi_ext_system_pro_man_exp_time_table_read(IspDev, i);
        pstSnapAttr->stProParam.stManual.au32ManSysgain[i] = hi_ext_system_pro_man_sysgain_table_read(IspDev, i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDngImageStaticInfo(ISP_DEV IspDev, DNG_IMAGE_STATIC_INFO_S *pstDngImageStaticInfo)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDngImageStaticInfo);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    HI_BOOL bValidDngImageStaticInfo;

    bValidDngImageStaticInfo = hi_ext_system_dng_static_info_valid_read(IspDev);

    if (HI_FALSE == bValidDngImageStaticInfo)
    {
        printf("DngImageStaticInfo have not been set in xxx_Cmos.x!\n");
        return HI_SUCCESS;
    }

    if (ioctl(g_as32IspFd[IspDev], ISP_DNG_INFO_GET, pstDngImageStaticInfo))
    {
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDngColorParam(ISP_DEV IspDev, const ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDngColorParam);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (pstDngColorParam->stWbGain1.u16Bgain > 0xFFF)
    {
        printf("stWbGain1.u16Bgain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain1.u16Ggain > 0xFFF)
    {
        printf("stWbGain1.u16Ggain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain1.u16Rgain > 0xFFF)
    {
        printf("stWbGain1.u16Rgain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain2.u16Bgain > 0xFFF)
    {
        printf("stWbGain2.u16Bgain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain2.u16Ggain > 0xFFF)
    {
        printf("stWbGain2.u16Ggain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDngColorParam->stWbGain2.u16Rgain > 0xFFF)
    {
        printf("stWbGain2.u16Rgain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_dng_high_wb_gain_r_write(IspDev, pstDngColorParam->stWbGain1.u16Rgain);
    hi_ext_system_dng_high_wb_gain_g_write(IspDev, pstDngColorParam->stWbGain1.u16Ggain);
    hi_ext_system_dng_high_wb_gain_b_write(IspDev, pstDngColorParam->stWbGain1.u16Bgain);
    hi_ext_system_dng_low_wb_gain_r_write(IspDev, pstDngColorParam->stWbGain2.u16Rgain);
    hi_ext_system_dng_low_wb_gain_g_write(IspDev, pstDngColorParam->stWbGain2.u16Ggain);
    hi_ext_system_dng_low_wb_gain_b_write(IspDev, pstDngColorParam->stWbGain2.u16Bgain);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDngColorParam(ISP_DEV IspDev, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDngColorParam);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstDngColorParam->stWbGain1.u16Rgain = hi_ext_system_dng_high_wb_gain_r_read(IspDev);
    pstDngColorParam->stWbGain1.u16Ggain = hi_ext_system_dng_high_wb_gain_g_read(IspDev);
    pstDngColorParam->stWbGain1.u16Bgain = hi_ext_system_dng_high_wb_gain_b_read(IspDev);

    pstDngColorParam->stWbGain2.u16Rgain = hi_ext_system_dng_low_wb_gain_r_read(IspDev);
    pstDngColorParam->stWbGain2.u16Ggain = hi_ext_system_dng_low_wb_gain_g_read(IspDev);
    pstDngColorParam->stWbGain2.u16Bgain = hi_ext_system_dng_low_wb_gain_b_read(IspDev);

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

