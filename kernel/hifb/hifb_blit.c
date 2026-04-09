/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "hifb_blit.h"
#include "hi_tde_ext.h"
#include "mod_ext.h"

#include <linux/hardirq.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __HuaweiLite__
#define in_atomic()	(HI_FALSE)
#endif
extern bool g_bUpdateRotateRect;

static IntCallBack s_pTdeCallBack;
static IntCallBack s_pTdeRotateCallBack;

/* tde support which color format */
static HI_BOOL s_bTdeColFmt[HIFB_FMT_BUTT] = {
    HI_FALSE,    /* HIFB_FMT_RGB565 */
    HI_FALSE,    /* HIFB_FMT_RGB888 */
    HI_FALSE,    /* HIFB_FMT_KRGB444 */
    HI_FALSE,    /* HIFB_FMT_KRGB555 */
    HI_FALSE,   /* HIFB_FMT_KRGB888 */
    HI_TRUE,    /* HIFB_FMT_ARGB4444 */
    HI_TRUE,    /* HIFB_FMT_ARGB1555 */
    HI_TRUE,    /* HIFB_FMT_ARGB8888 */
    HI_FALSE,    /* HIFB_FMT_ARGB8565 */
    HI_FALSE,   /* HIFB_FMT_RGBA4444 */
    HI_FALSE,   /* HIFB_FMT_RGBA5551 */
    HI_FALSE,   /* HIFB_FMT_RGBA5658 */
    HI_FALSE,   /* HIFB_FMT_RGBA8888 */

    HI_FALSE,   /**< BGR565 */
    HI_FALSE,   /**< BGR888 */
    HI_FALSE,   /**< ABGR4444 */
    HI_FALSE,   /**< ABGR1555 */
    HI_FALSE,   /**< ABGR8888 */
    HI_FALSE,   /**< ABGR8565 */
    HI_FALSE,   /**< BGR444 16bpp */
    HI_FALSE,   /**< BGR555 16bpp */
    HI_FALSE,   /**< BGR888 32bpp */

    HI_FALSE,    /* HIFB_FMT_1BPP */
    HI_FALSE,    /* HIFB_FMT_2BPP */
    HI_FALSE,    /* HIFB_FMT_4BPP */
    HI_FALSE,    /* HIFB_FMT_8BPP */
    HI_FALSE,    /* HIFB_FMT_ACLUT44 */
    HI_FALSE,    /* HIFB_FMT_ACLUT88 */
    HI_FALSE,   /* HIFB_FMT_PUYVY */
    HI_FALSE,   /* HIFB_FMT_PYUYV */
    HI_FALSE,   /* HIFB_FMT_PYVYU */
    HI_FALSE,   /* HIFB_FMT_YUV888*/
    HI_FALSE,   /* HIFB_FMT_AYUV8888 */
    HI_FALSE,   /* HIFB_FMT_YUVA8888 */
};

HI_BOOL hifb_TdeIsSupportFmt(HIFB_COLOR_FMT_E fmt)
{
	if(HIFB_FMT_BUTT <= fmt)
	{
		return HI_FALSE;
	}
	return s_bTdeColFmt[fmt];
}
#if 1
hi_tde_color_fmt HIFB_DRV_ConvFmt(HIFB_COLOR_FMT_E Fmt)
{
    switch (Fmt)
    {
        case HIFB_FMT_RGB565:
            return HI_TDE_COLOR_FMT_RGB565;
        case HIFB_FMT_RGB888:
            return HI_TDE_COLOR_FMT_RGB888;
        case HIFB_FMT_KRGB444:
            return HI_TDE_COLOR_FMT_RGB444;
        case HIFB_FMT_KRGB555:
            return HI_TDE_COLOR_FMT_RGB555;
        /*HIFB_FMT_KRGB888 32bpp;TDE2_COLOR_FMT_RGB888:24bpp.so transform HIFB_FMT_KRGB888 to
        TDE2_COLOR_FMT_ARGB8888 ,but ignore alpha data in process*/
        case HIFB_FMT_KRGB888:
            return HI_TDE_COLOR_FMT_ARGB8888;
        case HIFB_FMT_ARGB4444:
            return HI_TDE_COLOR_FMT_ARGB4444;
        case HIFB_FMT_ARGB1555:
            return HI_TDE_COLOR_FMT_ARGB1555;
        case HIFB_FMT_ARGB8888:
            return HI_TDE_COLOR_FMT_ARGB8888;
        case HIFB_FMT_ARGB8565:
            return HI_TDE_COLOR_FMT_ARGB8565;

        case HIFB_FMT_BGR565:
            return HI_TDE_COLOR_FMT_BGR565;
        case HIFB_FMT_ABGR1555:
            return HI_TDE_COLOR_FMT_ABGR1555;
        case HIFB_FMT_ABGR4444:
            return HI_TDE_COLOR_FMT_ABGR4444;
        case HIFB_FMT_KBGR444:
            return HI_TDE_COLOR_FMT_BGR444;
        case HIFB_FMT_KBGR555:
            return HI_TDE_COLOR_FMT_BGR555;
        case HIFB_FMT_BGR888:
            return HI_TDE_COLOR_FMT_BGR888;
        case HIFB_FMT_ABGR8888:
            return HI_TDE_COLOR_FMT_ABGR8888;
        case HIFB_FMT_ABGR8565:
            return HI_TDE_COLOR_FMT_ABGR8565;
        case HIFB_FMT_KBGR888:
            return HI_TDE_COLOR_FMT_ABGR8888;

        case HIFB_FMT_1BPP:
            return HI_TDE_COLOR_FMT_CLUT1;
        case HIFB_FMT_2BPP:
            return HI_TDE_COLOR_FMT_CLUT2;
        case HIFB_FMT_4BPP:
            return HI_TDE_COLOR_FMT_CLUT4;
        case HIFB_FMT_8BPP:
            return HI_TDE_COLOR_FMT_CLUT8;
        case HIFB_FMT_ACLUT44:
            return HI_TDE_COLOR_FMT_ACLUT44;
        case HIFB_FMT_ACLUT88:
            return HI_TDE_COLOR_FMT_ACLUT88;

        default:
            return HI_TDE_COLOR_FMT_MAX;

    }
}

HI_S32 HIFB_DRV_Fill(HIFB_BUFFER_S *pDstImg, HI_U32 u32FillData)
{
    HI_S32 s32Ret;
    hi_tde_surface stDstSur = {0};
    hi_tde_rect stDstRect = {0};
    hi_s32 handle;
    hi_tde_export_func *pTdeExportFunc = NULL;

    pTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
    if ((NULL == pTdeExportFunc) || (NULL == pTdeExportFunc->drv_tde_module_begin_job)
        || (NULL == pTdeExportFunc->drv_tde_module_end_job) || (NULL == pTdeExportFunc->drv_tde_module_cancel_job)
        || (NULL == pTdeExportFunc->drv_tde_module_quick_fill)
        )
    {
        HI_TRACE_FB(HI_DBG_ERR, "TDE pTdeExportFunc is NULL!\n");
        return HI_FAILURE;
    }

    /** config dst*/
    stDstSur.phy_addr      = pDstImg->stCanvas.u64PhyAddr;
    stDstSur.width     = pDstImg->stCanvas.u32Width;
    stDstSur.height    = pDstImg->stCanvas.u32Height;
    stDstSur.stride    = pDstImg->stCanvas.u32Pitch;
    stDstSur.alpha_max_is_255 = HI_TRUE;
    stDstSur.is_ycbcr_clut   = HI_FALSE;
    stDstSur.color_fmt   = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);

    stDstRect.pos_x     = 0;
    stDstRect.pos_y     = 0;
    stDstRect.height   = stDstSur.height;
    stDstRect.width    = stDstSur.width;

    s32Ret = pTdeExportFunc->drv_tde_module_begin_job(&handle);
    if(s32Ret != HI_SUCCESS)
    {
        osal_printk("HiFB Warning: TDE begin job failed\n");
        return s32Ret;
    }

    s32Ret = pTdeExportFunc->drv_tde_module_quick_fill(handle, &stDstSur, &stDstRect, u32FillData);
    if(s32Ret != HI_SUCCESS)
    {
        osal_printk("HiFB Warning: TDE blit failed. ret = 0x%x handle %d \n",s32Ret,handle);
        pTdeExportFunc->drv_tde_module_cancel_job(handle);
        return s32Ret;
    }

    s32Ret = pTdeExportFunc->drv_tde_module_end_job(handle, HI_TRUE, 1000, HI_FALSE, HI_NULL, HI_NULL);
    if(s32Ret != HI_SUCCESS)
    {
        osal_printk("HiFB Warning: FILL job submitted to TDE failed!!! Ret = 0x%x\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


static HI_U32 HIFB_DRV_GetDcmpStride(HIFB_SURFACE_S *pstCanvas)
{
    HI_U32 comp_ratio_int = 1000;
    HI_U32 u32BytePerPix = 2;
    HI_U32 extend_width;
    HI_U32 exp_num_temp = 0;
    HI_U32 u32Stride = pstCanvas->u32Pitch;

    if(HIFB_FMT_ARGB8888 == pstCanvas->enFmt)
    {
        if(pstCanvas->u32Width <= 320)
        {
            exp_num_temp = 2;
            comp_ratio_int = 1000;
        }
        else if(pstCanvas->u32Width <= 720)
        {
            exp_num_temp = 10;
            comp_ratio_int = 2000;
        }
        else if(pstCanvas->u32Width <= 3840)
        {
            exp_num_temp = 10;
            comp_ratio_int = 2000;
        }

        u32BytePerPix  = 4;

        u32Stride = ((pstCanvas->u32Width * u32BytePerPix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    }
    else if(HIFB_FMT_ARGB1555 == pstCanvas->enFmt || HIFB_FMT_ARGB4444 == pstCanvas->enFmt)
    {
        if(pstCanvas->u32Width <= 720)
        {
            exp_num_temp = 2;
            comp_ratio_int = 1000;
        }
        else if(pstCanvas->u32Width <= 3840)
        {
            exp_num_temp = 0;
            comp_ratio_int = 1000;
        }
        u32BytePerPix = 2;

        extend_width = ((pstCanvas->u32Width + 31) / 32) * 32;
        u32Stride = ((extend_width * u32BytePerPix * 8 * 1000 / comp_ratio_int + 127) / 128 + exp_num_temp) * 16;
    }

    u32Stride = (u32Stride + 0xf) & 0xfffffff0;

    return u32Stride;
}

HI_S32 HIFB_DRV_Blit(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg, HIFB_BLIT_OPT_S *pstOpt,
                     HI_BOOL bRefreshScreen, TDE_HANDLE *refresh_handle)
{
    hi_s32                    s32Ret;
    hi_tde_surface            stSrcSur       = {0};
    hi_tde_surface            stDstSur       = {0};
    hi_tde_rect               stSrcRect;
    hi_tde_rect               stDstRect;
    hi_s32                handle;
    hi_tde_opt                option          = {0};
    HIFB_TDECALLBACK_PARAM *  pParam         = HI_NULL;
    hi_tde_export_func  *      pTdeExportFunc = NULL;
    hi_u32                    u32timeout     = 100; /* 100ms */

    pTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
    if ((NULL == pTdeExportFunc) || (NULL == pTdeExportFunc->drv_tde_module_begin_job)
        || (NULL == pTdeExportFunc->drv_tde_module_end_job) || (NULL == pTdeExportFunc->drv_tde_module_cancel_job)
        || (NULL == pTdeExportFunc->drv_tde_module_enable_region_deflicker) || (NULL == pTdeExportFunc->drv_tde_module_blit)
        )
    {
        HI_TRACE_FB(HI_DBG_ERR, "TDE pTdeExportFunc is NULL!\n");
        return HI_FAILURE;
    }

    /** config src*/
    stSrcSur.phy_addr   = pSrcImg->stCanvas.u64PhyAddr;
    stSrcSur.width  = pSrcImg->stCanvas.u32Width;
    stSrcSur.height = pSrcImg->stCanvas.u32Height;
    stSrcSur.stride = pSrcImg->stCanvas.u32Pitch;
    stSrcSur.alpha_max_is_255 = HI_TRUE;
    stSrcSur.is_ycbcr_clut = HI_FALSE;
    stSrcSur.color_fmt = HIFB_DRV_ConvFmt(pSrcImg->stCanvas.enFmt);
    stSrcSur.alpha0 = pstOpt->stAlpha.u8Alpha0;
    stSrcSur.alpha1 = pstOpt->stAlpha.u8Alpha1;

    stSrcRect.pos_x = pSrcImg->UpdateRect.x;
    stSrcRect.pos_y = pSrcImg->UpdateRect.y;
    stSrcRect.width = pSrcImg->UpdateRect.w;
    stSrcRect.height = pSrcImg->UpdateRect.h;

    /** config dst*/
    stDstSur.phy_addr   = pDstImg->stCanvas.u64PhyAddr;
    stDstSur.width  = pDstImg->stCanvas.u32Width;
    stDstSur.height = pDstImg->stCanvas.u32Height;
    stDstSur.stride = pDstImg->stCanvas.u32Pitch;
    stDstSur.alpha_max_is_255 = HI_TRUE;
    stDstSur.is_ycbcr_clut = HI_FALSE;
    stDstSur.color_fmt = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);
    stDstSur.alpha0 = pstOpt->stAlpha.u8Alpha0;
    stDstSur.alpha1 = pstOpt->stAlpha.u8Alpha1;

    if(HI_TRUE == pstOpt->bCompress)
    {
        stDstSur.cbcr_phy_addr   = pDstImg->stCanvas.u64GBPhyAddr;
        stDstSur.cbcr_stride = pDstImg->stCanvas.u32Pitch;

        stDstSur.stride = HIFB_DRV_GetDcmpStride(&pDstImg->stCanvas);
    }

    if (HI_TRUE == bRefreshScreen)
    {
        stDstRect.pos_x   =  stSrcRect.pos_x * stDstSur.width / stSrcSur.width;
        stDstRect.pos_y   =  stSrcRect.pos_y * stDstSur.height / stSrcSur.height;
        stDstRect.width  =  stSrcRect.width * stDstSur.width / stSrcSur.width;
        stDstRect.height =  stSrcRect.height * stDstSur.height / stSrcSur.height;

        if (g_bUpdateRotateRect)
        {
            stDstRect.pos_x = stDstSur.width - stDstRect.width - stDstRect.pos_x;
            stDstRect.pos_y = stDstSur.height- stDstRect.height - stDstRect.pos_y;
        }
    }
    else
    {
        stDstRect.pos_x = pDstImg->UpdateRect.x;
        stDstRect.pos_y = pDstImg->UpdateRect.y;
        stDstRect.width = pDstImg->UpdateRect.w;
        stDstRect.height = pDstImg->UpdateRect.h;
    }

    option.resize = pstOpt->bScale;
    if ((stSrcSur.color_fmt >= HIFB_DRV_ConvFmt(HIFB_FMT_1BPP)) && (stSrcSur.color_fmt <= HIFB_DRV_ConvFmt(HIFB_FMT_ACLUT88)))
    {
        option.clut_reload = HI_TRUE;
        stSrcSur.clut_phy_addr = pstOpt->u64CmapAddr;
        stDstSur.clut_phy_addr = pstOpt->u64CmapAddr;
    }

    switch(pstOpt->enAntiflickerLevel)
    {
        case HIFB_LAYER_ANTIFLICKER_NONE:
        {
            option.filter_mode = HI_TDE_FILTER_MODE_COLOR;
            option.deflicker_mode= HI_TDE_DEFLICKER_LEVEL_MODE_NONE;
            break;
        }
        case HIFB_LAYER_ANTIFLICKER_LOW:
        {
            option.filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        }
        case HIFB_LAYER_ANTIFLICKER_MIDDLE:
       {
            option.filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        }
        case HIFB_LAYER_ANTIFLICKER_HIGH:
        {
            option.filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        }
        case HIFB_LAYER_ANTIFLICKER_AUTO:
        {
            option.filter_mode = HI_TDE_FILTER_MODE_COLOR;
            break;
        }
        default:
            option.deflicker_mode = HI_TDE_DEFLICKER_LEVEL_MODE_NONE;
            break;
    }

    if (pstOpt->stCKey.bKeyEnable)
    {
        if ((stSrcSur.color_fmt >= HIFB_DRV_ConvFmt(HIFB_FMT_1BPP)) && (stSrcSur.color_fmt <= HIFB_DRV_ConvFmt(HIFB_FMT_ACLUT88)))
        {
            option.color_key_mode = HI_TDE_COLOR_KEY_MODE_FOREGROUND;
            option.color_key_value.clut_color_key.alpha.is_component_ignore = HI_TRUE;
            option.color_key_value.clut_color_key.clut.is_component_out = pstOpt->stCKey.u32KeyMode;
            option.color_key_value.clut_color_key.clut.component_max = pstOpt->stCKey.u8BlueMax;
            option.color_key_value.clut_color_key.clut.component_min = pstOpt->stCKey.u8BlueMin;
        }
        else
        {
            option.color_key_mode = HI_TDE_COLOR_KEY_MODE_FOREGROUND;
            option.color_key_value.argb_color_key.alpha.is_component_ignore = HI_TRUE;
            option.color_key_value.argb_color_key.red.component_max = pstOpt->stCKey.u8RedMax;
            option.color_key_value.argb_color_key.red.component_min = pstOpt->stCKey.u8RedMin;
            option.color_key_value.argb_color_key.red.is_component_out = pstOpt->stCKey.u32KeyMode;
            option.color_key_value.argb_color_key.red.component_mask = 0xff;
            option.color_key_value.argb_color_key.green.component_max = pstOpt->stCKey.u8GreenMax;
            option.color_key_value.argb_color_key.green.component_min = pstOpt->stCKey.u8GreenMin;
            option.color_key_value.argb_color_key.green.is_component_out = pstOpt->stCKey.u32KeyMode;
            option.color_key_value.argb_color_key.green.component_mask = 0xff;
            option.color_key_value.argb_color_key.blue.component_max = pstOpt->stCKey.u8BlueMax;
            option.color_key_value.argb_color_key.blue.component_min = pstOpt->stCKey.u8BlueMin;
            option.color_key_value.argb_color_key.blue.is_component_out = pstOpt->stCKey.u32KeyMode;
            option.color_key_value.argb_color_key.blue.component_mask = 0xff;
        }
    }

    if (pstOpt->stAlpha.bAlphaEnable)
    {
        option.alpha_blending_cmd = HI_TDE_ALPHA_BLENDING_BLEND;
        option.global_alpha = pstOpt->stAlpha.u8GlobalAlpha;
        option.out_alpha_from = HI_TDE_OUT_ALPHA_FROM_NORM;
    }
    else
    {
        option.out_alpha_from = HI_TDE_OUT_ALPHA_FROM_FOREGROUND;
    }

    switch (pstOpt->enMirrorMode)
    {
        case HIFB_MIRROR_NONE:
            option.mirror = HI_TDE_MIRROR_NONE;
            break;
        case HIFB_MIRROR_HORIZONTAL:
            option.mirror = HI_TDE_MIRROR_HORIZONTAL;
            break;
        case HIFB_MIRROR_VERTICAL:
            option.mirror = HI_TDE_MIRROR_VERTICAL;
            break;
        case HIFB_MIRROR_BOTH:
            option.mirror = HI_TDE_MIRROR_BOTH;
            break;
        default:
            option.mirror = HI_TDE_MIRROR_NONE;
            break;
    }

#if 1
    if (pstOpt->bRegionDeflicker)
    {
        s32Ret = pTdeExportFunc->drv_tde_module_enable_region_deflicker(HI_TRUE);
        if (s32Ret != HI_SUCCESS)
        {
            HI_TRACE_FB(HI_DBG_ERR, "enable region deflicker failed!\n");
            return s32Ret;
        }
    }
#endif

    s32Ret = pTdeExportFunc->drv_tde_module_begin_job(&handle);
    if(s32Ret != HI_SUCCESS)
    {
        HI_TRACE_FB(HI_DBG_ERR, "begin job failed\n");
        return s32Ret;
    }

    if (refresh_handle != NULL) {
        *refresh_handle = handle;
    }
    option.is_compress = pstOpt->bCompress;
    s32Ret = pTdeExportFunc->drv_tde_module_blit(handle, &stDstSur, &stDstRect, &stSrcSur, &stSrcRect, &stDstSur, \
        &stDstRect, &option);
    if(s32Ret != HI_SUCCESS)
    {
        HI_TRACE_FB(HI_DBG_ERR, "tde blit failed ret = 0x%x handle %d \n", s32Ret, handle);
        pTdeExportFunc->drv_tde_module_cancel_job(handle);
        return s32Ret;
    }
    #ifdef HI_FPGA
    // ARGB1555 : widthxheight/8/period s, period is ASIC or FPGA CORE clock of tde.
    // eg. FPGA core 48MHz, 3840 * 2160 / 8 / (48 * 10 ^ 6) = 0.0216(s)
    u32timeout = 432; // 0.216 * 2
    #endif
    if (pstOpt->bCallBack)
    {
        if (in_atomic())
        {
            pParam = kmalloc(sizeof(HIFB_TDECALLBACK_PARAM), GFP_ATOMIC);
        }

        else
        {
            pParam = kmalloc(sizeof(HIFB_TDECALLBACK_PARAM), GFP_KERNEL);
        }
        if (HI_NULL == pParam)
        {
            pTdeExportFunc->drv_tde_module_cancel_job(handle);
            return HI_FAILURE;
        }

        pParam->u32LayerId = *(HI_U32 *)pstOpt->pParam;
        pParam->bSoftCursorUpdate = pstOpt->bSoftCursorUpdate;
        pParam->bCompress = pstOpt->bCompress;

        s32Ret = pTdeExportFunc->drv_tde_module_end_job(handle, pstOpt->bBlock, u32timeout, HI_FALSE, \
            (tde_func_cb)s_pTdeCallBack, pParam);
    }
    else
    {
        s32Ret = pTdeExportFunc->drv_tde_module_end_job(handle, pstOpt->bBlock, u32timeout, HI_FALSE, \
            HI_NULL, HI_NULL);
    }

    if(s32Ret != HI_SUCCESS)
    {
        HI_TRACE_FB(HI_DBG_ERR, "end job failed!s32Ret = %x\n", s32Ret);
        s32Ret = pTdeExportFunc->drv_tde_module_cancel_job(handle);
        // if pfnTdeCancelJob returns HI_FAILURE, it means : job has be submitted, so we do not free pParam.
        if(HI_SUCCESS == s32Ret)
        {
            if (pParam)
            {
                kfree(pParam);
            }
        }
        else
        {
            HI_TRACE_FB(HI_DBG_ERR, "cancel job failed!s32Ret = %x\n", s32Ret);
        }
        return s32Ret;
    }

#if 1
    if (pstOpt->bRegionDeflicker)
    {
        s32Ret = pTdeExportFunc->drv_tde_module_enable_region_deflicker(HI_FALSE);
        if (s32Ret != HI_SUCCESS)
        {
            HI_TRACE_FB(HI_DBG_ERR, "disable region deflicker failed!\n");
            return s32Ret;
        }
    }
#endif
    return HI_SUCCESS;
}
#endif

// for graphic rotation.
HI_S32 HIFB_DRV_Rotate(HIFB_BUFFER_S *pSrcImg, HIFB_BUFFER_S *pDstImg, HIFB_ROTATE_OPT_S *pstRotOpt, HI_BOOL bRefreshScreen)
{
    HI_S32                   s32Ret;
    hi_tde_surface           stSrcSur         = {0};
    hi_tde_surface           stDstSur         = {0};
    hi_tde_rect              stSrcRect = {0};
    hi_tde_rect              stDstRect = {0};
    hi_s32               handle;
    HIFB_TDECALLBACK_PARAM*  pParam           = HI_NULL;
    hi_tde_export_func*       pTdeExportFunc   = NULL;
    hi_tde_rotate_angle       enTdeRotateAngle = HI_TDE_ROTATE_MAX;
    HI_U32                   u32timeout       = 2000; // 100ms

    pTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
    if (   (NULL == pTdeExportFunc)
        || (NULL == pTdeExportFunc->drv_tde_module_begin_job)
        || (NULL == pTdeExportFunc->drv_tde_module_rotate)
        || (NULL == pTdeExportFunc->drv_tde_module_cancel_job)
        || (NULL == pTdeExportFunc->drv_tde_module_end_job)
        )
    {
        HI_TRACE_FB(HI_DBG_ERR, "TDE pTdeExportFunc is NULL!\n");
        return HI_FAILURE;
    }

    /** config src*/
    stSrcSur.phy_addr   = pSrcImg->stCanvas.u64PhyAddr;
    stSrcSur.width  = pSrcImg->stCanvas.u32Width;
    stSrcSur.height = pSrcImg->stCanvas.u32Height;
    stSrcSur.stride = pSrcImg->stCanvas.u32Pitch;
    stSrcSur.alpha_max_is_255 = HI_TRUE;
    stSrcSur.is_ycbcr_clut = HI_FALSE;
    stSrcSur.color_fmt = HIFB_DRV_ConvFmt(pSrcImg->stCanvas.enFmt);

    stSrcRect.pos_x   = pSrcImg->UpdateRect.x;
    stSrcRect.pos_y   = pSrcImg->UpdateRect.y;
    stSrcRect.width  = pSrcImg->UpdateRect.w;
    stSrcRect.height = pSrcImg->UpdateRect.h;

    /** config dst*/
    stDstSur.phy_addr      = pDstImg->stCanvas.u64PhyAddr;
    stDstSur.width     = pDstImg->stCanvas.u32Width;
    stDstSur.height    = pDstImg->stCanvas.u32Height;
    stDstSur.stride    = pDstImg->stCanvas.u32Pitch;
    stDstSur.alpha_max_is_255 = HI_TRUE;
    stDstSur.is_ycbcr_clut   = HI_FALSE;
    stDstSur.color_fmt   = HIFB_DRV_ConvFmt(pDstImg->stCanvas.enFmt);


    stDstRect.pos_x   = pDstImg->UpdateRect.x;
    stDstRect.pos_y   = pDstImg->UpdateRect.y;
    stDstRect.width  = pSrcImg->UpdateRect.h;
    stDstRect.height = pSrcImg->UpdateRect.w;

#ifdef HI_FPGA
    // ARGB1555 : widthxheight/8/period s, period is ASIC or FPGA CORE clock of tde.
    // eg. FPGA core 48MHz, 3840 * 2160 / 8 / (48 * 10 ^ 6) = 0.0216(s)
    u32timeout = 432; // 0.216 * 2
#endif


    s32Ret = pTdeExportFunc->drv_tde_module_begin_job(&handle);
    if(s32Ret != HI_SUCCESS)
    {
        HI_TRACE_FB(HI_DBG_ERR, "begin job for rotate failed\n");
        return s32Ret;
    }

    switch(pstRotOpt->enRotateMode)
    {
        case HIFB_ROTATE_90:
            enTdeRotateAngle = HI_TDE_ROTATE_CLOCKWISE_90;
            break;
        case HIFB_ROTATE_180:
            enTdeRotateAngle = HI_TDE_ROTATE_CLOCKWISE_180;
            break;
        case HIFB_ROTATE_270:
            enTdeRotateAngle = HI_TDE_ROTATE_CLOCKWISE_270;
            break;
        default:
            break;
    }

    s32Ret = pTdeExportFunc->drv_tde_module_rotate(handle, &stSrcSur, &stSrcRect, &stDstSur, &stDstRect, enTdeRotateAngle);
    if(s32Ret != HI_SUCCESS)
    {
        HI_TRACE_FB(HI_DBG_ERR, "tde rotate failed ret = 0x%x handle %d \n", s32Ret, handle);
        pTdeExportFunc->drv_tde_module_cancel_job(handle);
        return s32Ret;
    }

    if (pstRotOpt->bCallBack)
    {
        if (in_atomic())
        {
            pParam = kmalloc(sizeof(HIFB_TDECALLBACK_PARAM), GFP_ATOMIC);
        }

        else
        {
            pParam = kmalloc(sizeof(HIFB_TDECALLBACK_PARAM), GFP_KERNEL);
        }
        if (HI_NULL == pParam)
        {
            pTdeExportFunc->drv_tde_module_cancel_job(handle);
            return HI_FAILURE;
        }

        pParam->u32LayerId = *(HI_U32 *)pstRotOpt->pParam;

        s32Ret = pTdeExportFunc->drv_tde_module_end_job(handle, pstRotOpt->bBlock, u32timeout, HI_FALSE, \
            (tde_func_cb)s_pTdeRotateCallBack, pParam);
    }
    else
    {
        s32Ret = pTdeExportFunc->drv_tde_module_end_job(handle, pstRotOpt->bBlock, u32timeout, HI_FALSE, \
            HI_NULL, HI_NULL);
    }

    if(s32Ret != HI_SUCCESS)
    {
        HI_TRACE_FB(HI_DBG_ERR, "end job failed!s32Ret = %x\n", s32Ret);
        s32Ret = pTdeExportFunc->drv_tde_module_cancel_job(handle);
        if(HI_SUCCESS == s32Ret)
        {
            if (pParam)
            {
                kfree(pParam);
            }
        }
        else
        {
            HI_TRACE_FB(HI_DBG_ERR, "cancel job failed!s32Ret = %x\n", s32Ret);
        }
        return s32Ret;
    }

    return handle;
}

HI_S32 HIFB_DRV_SetTdeCallBack(IntCallBack pTdeCallBack)
{
   s_pTdeCallBack = pTdeCallBack;
   return HI_SUCCESS;
}
HI_S32 HIFB_DRV_SetTdeRotateCallBack(IntCallBack pTdeRotCallBack)
{
   s_pTdeRotateCallBack = pTdeRotCallBack;
   return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

