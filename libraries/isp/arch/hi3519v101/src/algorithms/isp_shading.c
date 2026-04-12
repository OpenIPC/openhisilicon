/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_shading.c
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
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_shading_mem_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef MAX2
#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#endif

#ifndef CLIP3
#define CLIP3(x,min,max)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#endif

typedef struct hiSHADING_ATTR_S
{
    HI_BOOL bRadialEnable;
    HI_BOOL bMeshEnable;
    HI_U8   u8MeshLSMode;
    HI_U16  u16MeshStrength;
    HI_U32  au32MeshColorTemp[ISP_MESH_LS_NUM_MAX];
} SHADING_ATTR_S;


SHADING_ATTR_S g_astShadingCtx[ISP_MAX_DEV_NUM] = {{0}};
#define SHADING_GET_CTX(dev, pstCtx)   pstCtx = &g_astShadingCtx[dev]


static HI_VOID ShadingExtRegsDefault(ISP_DEV IspDev)
{    
    hi_ext_system_shading_table_node_number_write(IspDev, HI_EXT_SYSTEM_SHADING_TABLE_NODE_NUMBER_DEFAULT);
    hi_ext_system_mesh_col_temp_h_write(IspDev, HI_EXT_SYSTEM_MESH_COL_TEMP_H_DEFAULT);
    hi_ext_system_mesh_col_temp_m_write(IspDev, HI_EXT_SYSTEM_MESH_COL_TEMP_M_DEFAULT);
    hi_ext_system_mesh_col_temp_l_write(IspDev, HI_EXT_SYSTEM_MESH_COL_TEMP_L_DEFAULT);
    hi_ext_system_mesh_col_temp_ll_write(IspDev, HI_EXT_SYSTEM_MESH_COL_TEMP_LL_DEFAULT);
    return;
}

static HI_VOID ShadingRegsDefault(ISP_DEV IspDev) 
{
    return;
}

static HI_VOID ShadingExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stRadialShading.bValid)
    {
        pstSnsDft->stRadialShading.u16TblNodeNum =
            (pstSnsDft->stRadialShading.u16TblNodeNum < SHADING_NODE_NUM_MAX) ?
            pstSnsDft->stRadialShading.u16TblNodeNum : SHADING_NODE_NUM_MAX;
        
        hi_ext_system_shading_table_node_number_write(IspDev, pstSnsDft->stRadialShading.u16TblNodeNum);
    }

    if (pstSnsDft->stMeshShading.bValid)
    {
        hi_ext_system_mesh_col_temp_h_write(IspDev, pstSnsDft->stMeshShading.au32MeshColorTemp[3]);
        hi_ext_system_mesh_col_temp_m_write(IspDev, pstSnsDft->stMeshShading.au32MeshColorTemp[2]);
        hi_ext_system_mesh_col_temp_l_write(IspDev, pstSnsDft->stMeshShading.au32MeshColorTemp[1]);
        hi_ext_system_mesh_col_temp_ll_write(IspDev, pstSnsDft->stMeshShading.au32MeshColorTemp[0]);
    }
    return;
}

static HI_VOID ShadingRegsInitialize(ISP_DEV IspDev)
{
    HI_S32 i,j;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_U32 u32MeshLutData = 0;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);   

    hi_isp_mesh_shading_mesh_page_r_write(IspDev, HI_ISP_MESH_SHADING_MESH_PAGE_R_DEFAULT);
    hi_isp_mesh_shading_mesh_page_g_write(IspDev, HI_ISP_MESH_SHADING_MESH_PAGE_G_DEFAULT);
    hi_isp_mesh_shading_mesh_page_b_write(IspDev, HI_ISP_MESH_SHADING_MESH_PAGE_B_DEFAULT);
    hi_isp_mesh_shading_mesh_alpha_bank_r_write(IspDev, HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_R_DEFAULT);
    hi_isp_mesh_shading_mesh_alpha_bank_g_write(IspDev, HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_G_DEFAULT);
    hi_isp_mesh_shading_mesh_alpha_bank_b_write(IspDev, HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_B_DEFAULT);
    hi_isp_mesh_shading_mesh_alpha_r_write(IspDev, HI_ISP_MESH_SHADING_MESH_ALPHA_R_DEFAULT);
    hi_isp_mesh_shading_mesh_alpha_g_write(IspDev, HI_ISP_MESH_SHADING_MESH_ALPHA_R_DEFAULT);
    hi_isp_mesh_shading_mesh_alpha_b_write(IspDev, HI_ISP_MESH_SHADING_MESH_ALPHA_R_DEFAULT); 
    hi_isp_mesh_shading_mesh_show_write(IspDev, HI_ISP_MESH_SHADING_MESH_SHOW_DEFAULT);

    /* if radial and mesh enabled simultaneously in xxx_cmos.c, radial shading select*/
    if (pstSnsDft->stRadialShading.bValid)
    {
        hi_isp_radial_shading_centerr_x_write(IspDev, pstSnsDft->stRadialShading.u16RCenterX);
        hi_isp_radial_shading_centerr_y_write(IspDev, pstSnsDft->stRadialShading.u16RCenterY);
        hi_isp_radial_shading_centerg_x_write(IspDev, pstSnsDft->stRadialShading.u16GCenterX);
        hi_isp_radial_shading_centerg_y_write(IspDev, pstSnsDft->stRadialShading.u16GCenterY);
        hi_isp_radial_shading_centerb_x_write(IspDev, pstSnsDft->stRadialShading.u16BCenterX);
        hi_isp_radial_shading_centerb_y_write(IspDev, pstSnsDft->stRadialShading.u16BCenterY);

        hi_isp_radial_shading_mcu_priority_write(IspDev, 1);
        for(i = 0; i < pstSnsDft->stRadialShading.u16TblNodeNum; i++)
        {
            hi_radial_shading_mem_luts_shading_lutr_write(IspDev, i, pstSnsDft->stRadialShading.au16RShadingTbl[i]);
            hi_radial_shading_mem_luts_shading_lutg_write(IspDev, i, pstSnsDft->stRadialShading.au16GShadingTbl[i]);
            hi_radial_shading_mem_luts_shading_lutb_write(IspDev, i, pstSnsDft->stRadialShading.au16BShadingTbl[i]);
        }
        hi_isp_radial_shading_mcu_priority_write(IspDev, 0);

        hi_isp_radial_shading_off_center_multr_write(IspDev, pstSnsDft->stRadialShading.u16ROffCenter);
        hi_isp_radial_shading_off_center_multg_write(IspDev, pstSnsDft->stRadialShading.u16GOffCenter);
        hi_isp_radial_shading_off_center_multb_write(IspDev, pstSnsDft->stRadialShading.u16BOffCenter);

        hi_isp_radial_shading_enable_write(IspDev, 1);
    }    

    if (pstSnsDft->stMeshShading.bValid)
    {        
        hi_isp_mesh_shading_mesh_alpha_mode_write(IspDev, pstSnsDft->stMeshShading.u8MeshLSMode);       
        hi_isp_mesh_shading_mesh_strength_write(IspDev, pstSnsDft->stMeshShading.u16MeshStrength);
        hi_isp_mesh_shading_mesh_scale_write(IspDev, pstSnsDft->stMeshShading.u8MeshScale);        
        
        switch(pstSnsDft->stMeshShading.u8MeshLSMode)
        {
            case 0: /*single light source*/
                for (j=0; j<ISP_MESH_SHADING_PLANE_NUM; j++)
                {
                    for (i=0; i<ISP_MESH_SHADING_NODE_NUM; i+=4)
                    {
                        u32MeshLutData = pstSnsDft->stMeshShading.au8MeshTable[j][i] + (pstSnsDft->stMeshShading.au8MeshTable[j][i+1] << 8)
                            + (pstSnsDft->stMeshShading.au8MeshTable[j][i+2] << 16) + (pstSnsDft->stMeshShading.au8MeshTable[j][i+3] << 24);                        
                        if (0 == j) hi_mesh_shading_mem_luts_shading_lutr_write(IspDev, ((HI_U32)i>>2), u32MeshLutData);
                        if (1 == j) hi_mesh_shading_mem_luts_shading_lutg_write(IspDev, ((HI_U32)i>>2), u32MeshLutData);
                        if (2 == j) hi_mesh_shading_mem_luts_shading_lutb_write(IspDev, ((HI_U32)i>>2), u32MeshLutData);                  
                    }
                }
                hi_isp_mesh_shading_mesh_width_write(IspDev, 64-1);
                hi_isp_mesh_shading_mesh_height_write(IspDev, 64-1);
                break;
            case 1: /*two light source*/
                for (j=0; j<ISP_MESH_SHADING_PLANE_NUM; j++)
                {
                    for (i=0; i<(ISP_MESH_SHADING_NODE_NUM>>1); i+=2)
                    {
                        u32MeshLutData = pstSnsDft->stMeshShading.au8MeshTable[j][i] + (pstSnsDft->stMeshShading.au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>1)] << 8)
                            + (pstSnsDft->stMeshShading.au8MeshTable[j][i+1] << 16) + (pstSnsDft->stMeshShading.au8MeshTable[j][i+1+(ISP_MESH_SHADING_NODE_NUM>>1)] << 24);
                        if (0 == j) hi_mesh_shading_mem_luts_shading_lutr_write(IspDev, ((HI_U32)i>>1), u32MeshLutData);
                        if (1 == j) hi_mesh_shading_mem_luts_shading_lutg_write(IspDev, ((HI_U32)i>>1), u32MeshLutData);
                        if (2 == j) hi_mesh_shading_mem_luts_shading_lutb_write(IspDev, ((HI_U32)i>>1), u32MeshLutData); 
                    }
                }
                hi_isp_mesh_shading_mesh_width_write(IspDev, 32-1);
                hi_isp_mesh_shading_mesh_height_write(IspDev, 64-1);
                break;
            case 2: /*four light source*/
                for (j=0; j<ISP_MESH_SHADING_PLANE_NUM; j++)
                {
                    for (i=0; i<(ISP_MESH_SHADING_NODE_NUM>>2); i++)
                    {
                        u32MeshLutData = pstSnsDft->stMeshShading.au8MeshTable[j][i] + (pstSnsDft->stMeshShading.au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>2)] << 8)
                            + (pstSnsDft->stMeshShading.au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>1)] << 16) + (pstSnsDft->stMeshShading.au8MeshTable[j][i+(ISP_MESH_SHADING_NODE_NUM>>1)+(ISP_MESH_SHADING_NODE_NUM>>2)] << 24);
                        if (0 == j) hi_mesh_shading_mem_luts_shading_lutr_write(IspDev, (i), u32MeshLutData);
                        if (1 == j) hi_mesh_shading_mem_luts_shading_lutg_write(IspDev, (i), u32MeshLutData);
                        if (2 == j) hi_mesh_shading_mem_luts_shading_lutb_write(IspDev, (i), u32MeshLutData);
                    }
                }
                hi_isp_mesh_shading_mesh_width_write(IspDev, 32-1);
                hi_isp_mesh_shading_mesh_height_write(IspDev, 32-1);
                break;
            default:
                break;                
        }
        hi_isp_mesh_shading_enable_write(IspDev, 1);
        hi_isp_mesh_shading_mesh_reload_write(IspDev, 1);
    }
    
    return;
}

static HI_S32 ShadingReadRegs(ISP_DEV IspDev)
{
    SHADING_ATTR_S* pstShading;

    SHADING_GET_CTX(IspDev, pstShading);
    
    if (hi_isp_radial_shading_enable_read(IspDev))
    {
        pstShading->bRadialEnable = HI_TRUE;
    }
    else
    {
        pstShading->bRadialEnable = HI_FALSE;
    }

    if (hi_isp_mesh_shading_enable_read(IspDev))
    {
        pstShading->bMeshEnable = HI_TRUE;
    }
    else
    {
        pstShading->bMeshEnable = HI_FALSE;
    }
    pstShading->au32MeshColorTemp[3] = hi_ext_system_mesh_col_temp_h_read(IspDev);
    pstShading->au32MeshColorTemp[2] = hi_ext_system_mesh_col_temp_m_read(IspDev);
    pstShading->au32MeshColorTemp[1] = hi_ext_system_mesh_col_temp_l_read(IspDev);
    pstShading->au32MeshColorTemp[0] = hi_ext_system_mesh_col_temp_ll_read(IspDev);
    pstShading->u8MeshLSMode = hi_isp_mesh_shading_mesh_alpha_mode_read(IspDev);
    pstShading->u16MeshStrength = hi_isp_mesh_shading_mesh_strength_read(IspDev);
    
    return 0;
}

HI_S32 ShadingProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    SHADING_ATTR_S *pstShading = HI_NULL;

    SHADING_GET_CTX(IspDev, pstShading);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----SHADING INFO--------------------------------------------------------------\n");

    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8s"     "%8s"     "%8s"      "%8s\n",
            "Mesh",   "En",     "LSMode",  "Str");

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
             "        %8d"     "%8d"      "%8d\n",
            pstShading->bMeshEnable, pstShading->u8MeshLSMode, pstShading->u16MeshStrength);
    }
    
    {
        
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8s"     "%8s\n",
            "Radial", "En" );

        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
             "        %8d\n",
            pstShading->bRadialEnable);
    }

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_ShadingInit(ISP_DEV IspDev)
{
    ShadingRegsDefault(IspDev);
    ShadingExtRegsDefault(IspDev);
    ShadingRegsInitialize(IspDev);
    ShadingExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

static __inline HI_U8 InterAlpha(HI_U32 u32tmp, HI_U32 u32tmp0, HI_U32 u32tmp1, HI_U8 u8min, HI_U8 u8max)
{
    HI_U8 res;

    if ( u32tmp <= u32tmp1 ) return u8min;
    if ( u32tmp >= u32tmp0 ) return u8max;

    res = (u8max - u8min) * (u32tmp - u32tmp1) / (u32tmp0 - u32tmp1) + u8min;
    return res;
}


HI_S32 ISP_ShadingRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    SHADING_ATTR_S *pstShading = HI_NULL;
    //ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
    static HI_U32 u32PreColTmp[ISP_MAX_DEV_NUM] = {0, 0};
    HI_U32 u32CurColTmp = 0;
    HI_U32 au32CfgColTmp[4] = {0};
    HI_U8 i;
    HI_U8 u8CurAlpha = 0;
    HI_S8 index1, index2;
    ISP_DEV PreIspDev = 0;
    ISP_CTX_S *pstPreIspCtx = HI_NULL;

    SHADING_GET_CTX(IspDev, pstShading);
    ISP_GET_CTX(IspDev, pstIspCtx);

    ShadingReadRegs(IspDev);    

    if (pstShading->bMeshEnable)
    {
        if (0 == pstShading->u8MeshLSMode)
        {
            hi_isp_mesh_shading_mesh_alpha_bank_r_write(IspDev, 0);
            hi_isp_mesh_shading_mesh_alpha_bank_g_write(IspDev, 0);
            hi_isp_mesh_shading_mesh_alpha_bank_b_write(IspDev, 0);
            return HI_SUCCESS;
        }
        else
        {            
            if ((ISP_SNAP_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode)
                && ((SNAP_TYPE_SYNC == pstIspCtx->stLinkage.enSnapType) ||(SNAP_TYPE_SYNC_PRO == pstIspCtx->stLinkage.enSnapType)))
            {
                PreIspDev = (IspDev == 0)? 1:0;
            }
            else
            {
                PreIspDev = IspDev;
            }
            ISP_GET_CTX(PreIspDev, pstPreIspCtx);
            u32CurColTmp = (pstPreIspCtx->stLinkage.u32ColorTemp << 8);
            if (0 == u32PreColTmp[IspDev])
            {
                //u32CurColTmp = u32CurColTmp;
            }
            else
            {
                u32CurColTmp = (u32PreColTmp[IspDev] * 31 + u32CurColTmp) >> 5;
            }

            u32PreColTmp[IspDev] = u32CurColTmp;
            u32CurColTmp >>= 8; 

            for (i=0; i<4; i++)
            {
                au32CfgColTmp[i] = pstShading->au32MeshColorTemp[i];
            }
            
            switch (pstShading->u8MeshLSMode)
            {
                case 0:
                default:
                    break;
                case 1:
                    u8CurAlpha = InterAlpha(u32CurColTmp, au32CfgColTmp[1], au32CfgColTmp[0], 0, 255);
                    hi_isp_mesh_shading_mesh_alpha_bank_r_write(IspDev, 0);
                    hi_isp_mesh_shading_mesh_alpha_bank_g_write(IspDev, 0);
                    hi_isp_mesh_shading_mesh_alpha_bank_b_write(IspDev, 0);
                    break;
                case 2:
                    index1 = 3;
            
                    for (i = 0; i < 4; i++)
                    {
                        if (u32CurColTmp <= au32CfgColTmp[i])
                        {
                            index1 = i;
                            break;
                        }
                    }
                    index2 = MAX2(index1 - 1, 0);
                    u8CurAlpha = InterAlpha(u32CurColTmp, au32CfgColTmp[index1], au32CfgColTmp[index2], 0, 255);
                    hi_isp_mesh_shading_mesh_alpha_bank_r_write(IspDev, index2);
                    hi_isp_mesh_shading_mesh_alpha_bank_g_write(IspDev, index2);
                    hi_isp_mesh_shading_mesh_alpha_bank_b_write(IspDev, index2);
                    break;
            }

            hi_isp_mesh_shading_mesh_alpha_r_write(IspDev, u8CurAlpha);
            hi_isp_mesh_shading_mesh_alpha_g_write(IspDev, u8CurAlpha);
            hi_isp_mesh_shading_mesh_alpha_b_write(IspDev, u8CurAlpha);
        }                           
        
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ShadingCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_ShadingInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            ShadingProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_ShadingExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterShading(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SHADING;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_ShadingInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_ShadingRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_ShadingCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_ShadingExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


