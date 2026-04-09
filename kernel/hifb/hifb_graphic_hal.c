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

#include "hi_osal.h"
#include "hi_debug.h"
#include "hifb_graphics_drv.h"
#include "hifb_coef.h"
#include "mddrc_reg.h"
#include "hi_math.h"
#include "hifb_graphic_hal.h"
#include "hifb_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define HAL_PRINT HI_PRINT

#define DDRC_BASE_ADDR  0x04605000
#define VOU_REGS_ADDR   0x11440000
#define VOU_REGS_SIZE   0x40000

// For CMP and DCMP
#define CMP_SEG_OFFSET  (0x80/4)
#define DCMP_SEG_OFFSET (0x20/4)

/****************************************************************************
 * EXTERN VARIABLES                                                         *
 ****************************************************************************/
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT

#define SYS_REGS_ADDR 0X12020000

typedef union {
    struct {
        unsigned int software_int : 1;  /* [0]     */
        unsigned int reserved     : 31; /* [1 ~31] */
    } bits;
    unsigned int u32;
} u_sys_reg;

typedef struct {
    volatile hi_u32 reserved[7]; /* 0x0~0x18, reserved 7 num register */
    volatile u_sys_reg soft_int; /* 0x1c */
    volatile hi_u32 reserved1; /* 0x20*/
} s_sys_regs_type;

volatile s_sys_regs_type *g_sys_reg = NULL;

#endif

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
volatile S_VDP_REGS_TYPE*   pHifbReg     = NULL;
volatile MDDRC_REGS_S*      pMddrcReg  = NULL;

/****************************************************************************
 * EXTERN FUNCTION DECLARATION                                              *
 ****************************************************************************/


/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/

/****************************************************************************
 * VOU FUNCTION                                                             *
 *            ---- Assistant Function Area                                  *
 ****************************************************************************/


/*****************************************************************************
 Prototype         : Hal initial
 Description     :
*****************************************************************************/

HI_VOID HAL_VOU_Init(HI_VOID)
{
    if (pHifbReg == HI_NULL) {
        pHifbReg = (volatile S_VDP_REGS_TYPE*)osal_ioremap(VOU_REGS_ADDR, (HI_U32)VOU_REGS_SIZE);
    }

    if (g_sys_reg == NULL) {
        g_sys_reg = (volatile s_sys_regs_type*)osal_ioremap(SYS_REGS_ADDR, (hi_u32)sizeof(s_sys_regs_type));
    }

    if (pHifbReg == HI_NULL) {
        osal_printk("ioremap_nocache failed\n");
    }

    if (g_sys_reg == NULL) {
        osal_printk("ioremap_nocache failed\n");
    }

}

HI_VOID HAL_VOU_Exit(HI_VOID)
{
    if (pHifbReg != HI_NULL) {
        osal_iounmap((void *)pHifbReg);
        pHifbReg = HI_NULL;
    }

    if (g_sys_reg != HI_NULL) {
        osal_iounmap((void *)g_sys_reg);
        g_sys_reg = HI_NULL;
    }

}
extern HIFB_COEF_ADDR_S g_stHifbCoefBufAddr;


/*****************************************************************************
 Prototype         : HAL_WriteReg
 Description       : write reg
*****************************************************************************/
HI_VOID HAL_WriteReg(HI_U32* pAddress, HI_U32 Value)
{
    *(volatile HI_U32*)pAddress = Value;
    return;
}

/*****************************************************************************
 Prototype         : HAL_ReadReg
 Description       : read reg
*****************************************************************************/
HI_U32 HAL_ReadReg(HI_U32* pAddress)
{
    return *(volatile HI_U32*)(pAddress);
}


// for p00-p22 use 14:0 coef
HI_U32 inline HAL_Conver_CscCoef(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10) / 1000 );

    if (s32Result < 0)
    {
        s32Result = (~((-1) * s32Result) + 1);

        /* 5.8 format */
        s32Result = ((s32Result & 0x7fff) | 0x4000);
    }
    else
    {
        s32Result = s32Result & 0x7fff;
    }

    return (HI_U32)s32Result;
}

HI_U32 inline HAL_Conver_CscCoef_NEW(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10) / 1000 );

    if (s32Result < 0)
    {
        s32Result = (~((-1) * s32Result) + 1);

        s32Result = ((s32Result & 0xffff) | 0x8000);
    }
    else
    {
        s32Result = s32Result & 0xffff;
    }

    return (HI_U32)s32Result;
}


/*****************************************************************************
 Prototype         : Vou_GetAbsAddr
 Description       : Get the absolute address of the layer (video layer and graphics layer)
*****************************************************************************/
HI_UL Vou_GetAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    HI_UL RegAbsAddr;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0 :
        case HAL_DISP_LAYER_VHD1 :
        case HAL_DISP_LAYER_VHD2 :
        {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_GFX0) * GFX_REGS_LEN;
            break;
        }

        // one wbc dev
        case HAL_DISP_LAYER_WBC:
            RegAbsAddr = (pReg);
            break;

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

/*****************************************************************************
 Prototype         : Vou_GetChnAbsAddr
 Description       : Get the absolute address of the video channel
*****************************************************************************/
//for get channel abs addr
HI_UL Vou_GetChnAbsAddr(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            RegAbsAddr = pReg + (enChan - HAL_DISP_CHANNEL_DHD0) * DHD_REGS_LEN;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

HI_UL Vou_GetGfxAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_GFX0 :
        case HAL_DISP_LAYER_GFX1 :
        case HAL_DISP_LAYER_GFX3 :
        {
            RegAbsAddr = pReg + (enLayer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;
        }
        default:
        {
            HAL_PRINT("Error layer id found in FUNC:%s,LINE:%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}
/*
* Name : HAL_DISP_GetIntfEnable
* Desc : Get the status (enable,disable status) of display interface.
*/
HI_BOOL HAL_DISP_GetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbIntfEn)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pbIntfEn = DHD0_CTRL.bits.intf_en;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL HAL_DISP_GetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                                     HAL_DISP_SYNCINFO_S *pstSyncInfo)
{
    volatile U_DHD0_CTRL   DHD0_CTRL;
    volatile U_DHD0_VSYNC1 DHD0_VSYNC1;
    volatile U_DHD0_VSYNC2 DHD0_VSYNC2;
    volatile U_DHD0_HSYNC1 DHD0_HSYNC1;
    volatile U_DHD0_HSYNC2 DHD0_HSYNC2;
    volatile U_DHD0_VPLUS1 DHD0_VPLUS1;
    volatile U_DHD0_VPLUS2 DHD0_VPLUS2;
    volatile U_DHD0_PWR    DHD0_PWR;
    volatile HI_UL         addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan,(HI_UL)&(pHifbReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->bIop = DHD0_CTRL.bits.iop;

            addr_REG = Vou_GetChnAbsAddr(enChan,(HI_UL)&(pHifbReg->DHD0_HSYNC1.u32));
            DHD0_HSYNC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Hact = DHD0_HSYNC1.bits.hact + 1;
            pstSyncInfo->u16Hbb = DHD0_HSYNC1.bits.hbb + 1;

            addr_REG = Vou_GetChnAbsAddr(enChan,(HI_UL)&(pHifbReg->DHD0_HSYNC2.u32));
            DHD0_HSYNC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Hmid = (0 == DHD0_HSYNC2.bits.hmid)?0:DHD0_HSYNC2.bits.hmid + 1;
            pstSyncInfo->u16Hfb = DHD0_HSYNC2.bits.hfb + 1;

            //Config VHD interface veritical timming
            addr_REG = Vou_GetChnAbsAddr(enChan,(HI_UL)&(pHifbReg->DHD0_VSYNC1.u32));
            DHD0_VSYNC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Vact = DHD0_VSYNC1.bits.vact + 1;
            pstSyncInfo->u16Vbb = DHD0_VSYNC1.bits.vbb + 1;

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL)&(pHifbReg->DHD0_VSYNC2.u32));
            DHD0_VSYNC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Vfb = DHD0_VSYNC2.bits.vfb + 1;

            //Config VHD interface veritical bottom timming,no use in progressive mode
            addr_REG = Vou_GetChnAbsAddr(enChan,(HI_UL)&(pHifbReg->DHD0_VPLUS1.u32));
            DHD0_VPLUS1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Bvact = DHD0_VPLUS1.bits.bvact + 1;
            pstSyncInfo->u16Bvbb = DHD0_VPLUS1.bits.bvbb + 1;

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_VPLUS2.u32));
            DHD0_VPLUS2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Bvfb = DHD0_VPLUS2.bits.bvfb + 1;

            //Config VHD interface veritical bottom timming,
            addr_REG = Vou_GetChnAbsAddr(enChan,(HI_UL)&(pHifbReg->DHD0_PWR.u32));
            DHD0_PWR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            pstSyncInfo->u16Hpw = DHD0_PWR.bits.hpw + 1;
            pstSyncInfo->u16Vpw = DHD0_PWR.bits.vpw + 1;

            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL  HAL_DISP_GetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan,VO_INTF_TYPE_E *pbenIntfType)
{
    volatile U_VO_MUX VO_MUX;
    if(enChan > HAL_DISP_CHANNEL_DHD1)
     {
         HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
         return HI_FALSE;
     }

    VO_MUX.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->VO_MUX.u32));

    switch(VO_MUX.bits.digital_sel)
    {
        case 0:
        {
            *pbenIntfType = HAL_DISP_INTF_BT1120;
            break;
        }
        case 1:
        {
            *pbenIntfType = HAL_DISP_INTF_BT656;
            break;
        }
        case 2:
        {
            *pbenIntfType = HAL_DISP_INTF_LCD;
            break;
        }

        default:
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL HAL_DISP_GetIntState(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbBottom)
{
    volatile U_DHD0_STATE DHD0_STATE;
    volatile HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_STATE.u32));
            DHD0_STATE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pbBottom = DHD0_STATE.bits.bottom_field;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


/*****************************************************************************
 Prototype         : HAL_DISP_GetDispIoP
 Description       : Interlace or Progressive
*****************************************************************************/
HI_BOOL  HAL_DISP_GetDispIoP(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbIop)
{
    U_DHD0_CTRL DHD0_CTRL;
    volatile  HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pbIop = DHD0_CTRL.bits.iop;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL HAL_DISP_GetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL* pbFieldMode)
{
    volatile U_DHD0_VTTHD  DHD0_VTTHD;
    volatile  HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_VTTHD.u32));
            DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pbFieldMode = DHD0_VTTHD.bits.thd1_mode;
            break;
        }
        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*
* Name : HAL_DISP_SetIntMask
* Desc : Set intterupt mask to open or close intterupt.
*/
HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK1 VOINTMSK1;
    /* Dispaly interrupt mask enable */
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOINTMSK1.u32));
    VOINTMSK1.u32 = VOINTMSK1.u32 | u32MaskEn;
    HAL_WriteReg((HI_U32*) & (pHifbReg->VOINTMSK1.u32), VOINTMSK1.u32);
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOINTMSK1.u32));
    return HI_TRUE;
}

/*
* Name : HAL_DISP_ClrIntMask
* Desc : TODO.
*/
HI_BOOL  HAL_DISP_ClrIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK1 VOINTMSK1;

    /* Dispaly interrupt mask enable */
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOINTMSK1.u32));
    VOINTMSK1.u32 = VOINTMSK1.u32 & (~u32MaskEn);
    HAL_WriteReg((HI_U32*) & (pHifbReg->VOINTMSK1.u32), VOINTMSK1.u32);

    return HI_TRUE;
}

//Get interrupt status
HI_U32 HAL_DISP_GetIntStatus(HI_U32 u32IntMsk)
{
    volatile U_VOMSKINTSTA1 VOMSKINTSTA1;

    /* read interrupt status */
    VOMSKINTSTA1.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOMSKINTSTA1.u32));

    return (VOMSKINTSTA1.u32 & u32IntMsk);
}


//Get the original interrupt status
HI_U32 HAL_DISP_GetOriIntStatus(HI_U32 u32IntStatus)
{
    volatile U_VOINTSTA VOINTSTA;

    /* read original interrupt status */
    VOINTSTA.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOINTSTA.u32));

    return (VOINTSTA.u32 & u32IntStatus);
}

/*
* Name : HAL_DISP_ClearIntStatus
* Desc : Clear interrupt status.
*/
HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk)
{
    /* read interrupt status */
    HAL_WriteReg((HI_U32*) & (pHifbReg->VOMSKINTSTA.u32), u32IntMsk);
    return HI_TRUE;
}

HI_U32 HAL_DISP_GetRawIntStatus(HI_U32 u32IntMsk)
{
    volatile U_VOINTSTA VOINTSTA;

    /* read interrupt status */
    VOINTSTA.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOINTSTA.u32));

    return (VOINTSTA.u32 & u32IntMsk);
}

HI_U32 HAL_DISP_GetRawIntStatus1(HI_U32 u32IntMsk)
{
    volatile U_VOINTSTA1 VOINTSTA1;

    /* read interrupt status */
    VOINTSTA1.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOINTSTA1.u32));

    return (VOINTSTA1.u32 & u32IntMsk);
}


/*
* Name : HAL_DISP_SetClkGateEnable
* Desc : Set VO Clock gate enable
*/
HI_BOOL HAL_DISP_SetClkGateEnable(HI_U32 u32Data)
{
    volatile U_VOCTRL VOCTRL;

    VOCTRL.u32 = HAL_ReadReg((HI_U32*) & (pHifbReg->VOCTRL.u32));
    VOCTRL.bits.vo_ck_gt_en = u32Data;
    HAL_WriteReg((HI_U32*) & (pHifbReg->VOCTRL.u32), VOCTRL.u32);

    return HI_TRUE;
}

/*
* Name : HAL_DISP_SetRegUp
* Desc : Set device register update.
*/
HI_VOID HAL_DISP_SetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile  HI_UL addr_REG;

    if (enChan > HAL_DISP_CHANNEL_DHD1)
    {
        HI_PRINT("Error,HAL_DISP_SetRegUp Select Wrong CHANNEL ID\n");
        return ;
    }

    // If wbc?
    // TODO
    // If DHD0 or DHD1
    addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_CTRL.u32));
    DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    DHD0_CTRL.bits.regup = 0x1;
    HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32);
    return;
}

/*
* Name : HAL_DISP_SetRegUp
* Desc : Get device register update.
*/
HI_U32 HAL_DISP_GetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan)
{
    HI_U32 u32Data;
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile  HI_UL addr_REG;

    addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pHifbReg->DHD0_CTRL.u32));
    DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    u32Data = DHD0_CTRL.bits.regup;
    return u32Data & 0x1;
}

HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT* pstRect)
{
    volatile U_G0_DFPOS G0_DFPOS;
    volatile U_G0_DLPOS G0_DLPOS;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_DFPOS.u32));
            G0_DFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_DFPOS.bits.disp_xfpos = pstRect->x;
            G0_DFPOS.bits.disp_yfpos = pstRect->y;
            HAL_WriteReg((HI_U32*)addr_REG, G0_DFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_DLPOS.u32));
            G0_DLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_DLPOS.bits.disp_xlpos = pstRect->x + pstRect->w - 1;
            G0_DLPOS.bits.disp_ylpos = pstRect->y + pstRect->h - 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_DLPOS.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

//Set the video image display area window
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT* pstRect)
{
    volatile U_G0_VFPOS G0_VFPOS;
    volatile U_G0_VLPOS G0_VLPOS;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_VFPOS.u32));
            G0_VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_VFPOS.bits.video_xfpos = pstRect->x;
            G0_VFPOS.bits.video_yfpos = pstRect->y;
            HAL_WriteReg((HI_U32*)addr_REG, G0_VFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_VLPOS.u32));
            G0_VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_VLPOS.bits.video_xlpos = pstRect->x + pstRect->w - 1;
            G0_VLPOS.bits.video_ylpos = pstRect->y + pstRect->h - 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_VLPOS.u32);
            break;

        }

        default:
        {
            HAL_PRINT("Error layer id %d# found in %s,%s: L%d\n", enLayer, __FILE__, __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}
/*
* Name : HAL_LAYER_EnableLayer
* Desc : Set layer enable
*/
HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)

{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->V0_CTRL.u32));
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_CTRL.bits.surface_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_CTRL.bits.surface_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_GetLayerEnable(HAL_DISP_LAYER_E enLayer, HI_U32* pu32Enable)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;

    volatile  HI_UL addr_REG;

    switch (enLayer)
    {

        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->V0_CTRL.u32));
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Enable = V0_CTRL.bits.surface_en;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu32Enable = G0_CTRL.bits.surface_en;
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*Desc : Set layer data type*/
HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                  HAL_DISP_PIXEL_FORMAT_E  enDataFmt)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile  HI_UL addr_REG;

    if (HAL_DISP_LAYER_GFX0 == enLayer ||
             HAL_DISP_LAYER_GFX1 == enLayer ||
             HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_SRC_INFO.bits.ifmt = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_SRC_INFO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id%d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer, HI_U32* pu32Fmt)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile  HI_UL addr_REG;

    if(HAL_DISP_LAYER_GFX0 == enLayer ||
            HAL_DISP_LAYER_GFX1 == enLayer ||
            HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pu32Fmt = GFX_SRC_INFO.bits.ifmt;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/
    return HI_TRUE;
}

/**********************************************************************************
*  Begin  : Video   layer CSC relative hal functions.
**********************************************************************************/
HI_VOID HAL_LAYER_CSC_SetEnable(HAL_DISP_LAYER_E enLayer, HI_BOOL csc_en)
{
    U_G0_HIPP_CSC_CTRL     G0_HIPP_CSC_CTRL;

    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_CTRL.u32));
        G0_HIPP_CSC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_CTRL.bits.hipp_csc_en = csc_en;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_CTRL.u32);

    }
}

HI_VOID HAL_LAYER_CSC_SetCkGtEn(HAL_DISP_LAYER_E enLayer, HI_BOOL ck_gt_en)
{
    U_G0_HIPP_CSC_CTRL     G0_HIPP_CSC_CTRL;

    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_CTRL.u32));
        G0_HIPP_CSC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_CTRL.bits.hipp_csc_ck_gt_en = ck_gt_en;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_CTRL.u32);
    }
}

HI_VOID HAL_LAYER_CSC_SetCoef(HAL_DISP_LAYER_E enLayer, VDP_CSC_COEF_S *pstCscCoef)
{
    U_G0_HIPP_CSC_COEF00   G0_HIPP_CSC_COEF00;
    U_G0_HIPP_CSC_COEF01   G0_HIPP_CSC_COEF01;
    U_G0_HIPP_CSC_COEF02   G0_HIPP_CSC_COEF02;
    U_G0_HIPP_CSC_COEF10   G0_HIPP_CSC_COEF10;
    U_G0_HIPP_CSC_COEF11   G0_HIPP_CSC_COEF11;
    U_G0_HIPP_CSC_COEF12   G0_HIPP_CSC_COEF12;
    U_G0_HIPP_CSC_COEF20   G0_HIPP_CSC_COEF20;
    U_G0_HIPP_CSC_COEF21   G0_HIPP_CSC_COEF21;
    U_G0_HIPP_CSC_COEF22   G0_HIPP_CSC_COEF22;

    volatile HI_U32 addr_REG;

    if(enLayer >= HAL_DISP_LAYER_GFX0 && enLayer <= HAL_DISP_LAYER_GFX3)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF00.u32));
        G0_HIPP_CSC_COEF00.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF00.bits.hipp_csc_coef00 = pstCscCoef->csc_coef00;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF00.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF01.u32));
        G0_HIPP_CSC_COEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF01.bits.hipp_csc_coef01 = pstCscCoef->csc_coef01;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF01.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF02.u32));
        G0_HIPP_CSC_COEF02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF02.bits.hipp_csc_coef02 = pstCscCoef->csc_coef02;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF02.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF10.u32));
        G0_HIPP_CSC_COEF10.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF10.bits.hipp_csc_coef10 = pstCscCoef->csc_coef10;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF10.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF11.u32));
        G0_HIPP_CSC_COEF11.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF11.bits.hipp_csc_coef11 = pstCscCoef->csc_coef11;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF11.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF12.u32));
        G0_HIPP_CSC_COEF12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF12.bits.hipp_csc_coef12 = pstCscCoef->csc_coef12;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF12.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF20.u32));
        G0_HIPP_CSC_COEF20.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF20.bits.hipp_csc_coef20 = pstCscCoef->csc_coef20;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF20.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF21.u32));
        G0_HIPP_CSC_COEF21.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF21.bits.hipp_csc_coef21 = pstCscCoef->csc_coef21;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF21.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_COEF22.u32));
        G0_HIPP_CSC_COEF22.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_COEF22.bits.hipp_csc_coef22 = pstCscCoef->csc_coef22;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_COEF22.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
    }

    return ;
}



HI_VOID HAL_LAYER_CSC_SetDcCoef(HAL_DISP_LAYER_E enLayer, VDP_CSC_DC_COEF_S *pstCscDcCoef)
{
    U_G0_HIPP_CSC_IDC0 G0_HIPP_CSC_IDC0;
    U_G0_HIPP_CSC_IDC1 G0_HIPP_CSC_IDC1;
    U_G0_HIPP_CSC_IDC2 G0_HIPP_CSC_IDC2;
    U_G0_HIPP_CSC_ODC0 G0_HIPP_CSC_ODC0;
    U_G0_HIPP_CSC_ODC1 G0_HIPP_CSC_ODC1;
    U_G0_HIPP_CSC_ODC2 G0_HIPP_CSC_ODC2;

    volatile HI_U32 addr_REG;
    if(enLayer == HAL_DISP_LAYER_GFX0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_IDC0.u32));
        G0_HIPP_CSC_IDC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_IDC0.bits.hipp_csc_idc0 = pstCscDcCoef->csc_in_dc0;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_IDC0.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_IDC1.u32));
        G0_HIPP_CSC_IDC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_IDC1.bits.hipp_csc_idc1 = pstCscDcCoef->csc_in_dc1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_IDC1.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_IDC2.u32));
        G0_HIPP_CSC_IDC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_IDC2.bits.hipp_csc_idc2 = pstCscDcCoef->csc_in_dc2;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_IDC2.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_ODC0.u32));
        G0_HIPP_CSC_ODC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_ODC0.bits.hipp_csc_odc0 = pstCscDcCoef->csc_out_dc0;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_ODC0.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_ODC1.u32));
        G0_HIPP_CSC_ODC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_ODC1.bits.hipp_csc_odc1 = pstCscDcCoef->csc_out_dc1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_ODC1.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_ODC2.u32));
        G0_HIPP_CSC_ODC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_ODC2.bits.hipp_csc_odc2 = pstCscDcCoef->csc_out_dc2;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_ODC2.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
    }
}

HI_VOID HAL_LAYER_CSC_SetParam(HAL_DISP_LAYER_E enLayer, CscCoefParam_S *pstCoefParam)
{
    U_G0_HIPP_CSC_SCALE    G0_HIPP_CSC_SCALE;
    U_G0_HIPP_CSC_MIN_Y    G0_HIPP_CSC_MIN_Y;
    U_G0_HIPP_CSC_MIN_C    G0_HIPP_CSC_MIN_C;
    U_G0_HIPP_CSC_MAX_Y    G0_HIPP_CSC_MAX_Y;
    U_G0_HIPP_CSC_MAX_C    G0_HIPP_CSC_MAX_C;

    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_SCALE.u32));
        G0_HIPP_CSC_SCALE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_SCALE.bits.hipp_csc_scale = pstCoefParam->csc_scale2p;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_SCALE.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_MIN_Y.u32));
        G0_HIPP_CSC_MIN_Y.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_MIN_Y.bits.hipp_csc_min_y = pstCoefParam->csc_clip_min;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_MIN_Y.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_MIN_C.u32));
        G0_HIPP_CSC_MIN_C.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_MIN_C.bits.hipp_csc_min_c = pstCoefParam->csc_clip_min;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_MIN_C.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_MAX_Y.u32));
        G0_HIPP_CSC_MAX_Y.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_MAX_Y.bits.hipp_csc_max_y = pstCoefParam->csc_clip_max;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_MAX_Y.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pHifbReg->G0_HIPP_CSC_MAX_C.u32));
        G0_HIPP_CSC_MAX_C.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_HIPP_CSC_MAX_C.bits.hipp_csc_max_c = pstCoefParam->csc_clip_max;
        HAL_WriteReg((HI_U32*)addr_REG, G0_HIPP_CSC_MAX_C.u32);
    }
}

HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S* pstCscCoef)
{
    if(enLayer < HAL_DISP_LAYER_VHD0 || enLayer > HAL_DISP_LAYER_GFX3)
    {
        HAL_PRINT("Error, Wrong layer ID!%d\n",__LINE__);
        return HI_FALSE;
    }

    HAL_LAYER_CSC_SetDcCoef(enLayer, (VDP_CSC_DC_COEF_S *)(&pstCscCoef->csc_in_dc0));
    HAL_LAYER_CSC_SetCoef(enLayer, (VDP_CSC_COEF_S *)(&pstCscCoef->csc_coef00));
    HAL_LAYER_CSC_SetParam(enLayer, (CscCoefParam_S *)(&pstCscCoef->new_csc_scale2p));

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetCscMode(HAL_DISP_LAYER_E enLayer, HI_BOOL bIsHCModeBy709)
{
    if(enLayer < HAL_DISP_LAYER_VHD0 || enLayer > HAL_DISP_LAYER_GFX3)
    {
        HAL_PRINT("Error, Wrong layer ID!%d\n",__LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    if(enLayer < HAL_DISP_LAYER_VHD0 || enLayer > HAL_DISP_LAYER_GFX3)
    {
        HAL_PRINT("Error, Wrong layer ID!%d\n",__LINE__);
        return HI_FALSE;
    }

    HAL_LAYER_CSC_SetCkGtEn(enLayer, bCscEn);
    HAL_LAYER_CSC_SetEnable(enLayer, bCscEn);

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer, HIFB_RECT *  pstRect)
{
    U_GFX_SRC_RESO GFX_SRC_RESO;
    volatile  HI_UL addr_REG;

    if(enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_SRC_RESO.u32));
        GFX_SRC_RESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_SRC_RESO.bits.src_w = pstRect->w- 1;
        GFX_SRC_RESO.bits.src_h = pstRect->h- 1;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_SRC_RESO.u32);
    }
    else
    {
        HAL_PRINT("Error:layer id not found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT* pstRect)
{
    U_GFX_IRESO   GFX_IRESO;
    volatile  HI_UL addr_REG;

    if(enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_IRESO.u32));
        GFX_IRESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_IRESO.bits.ireso_w = pstRect->w - 1;
        GFX_IRESO.bits.ireso_h = pstRect->h - 1;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_IRESO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT* pstRect)
{

    if(enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        return HI_TRUE;
    }
    else
    {
        HAL_PRINT("Error:layer id not found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
}

/*
* Name : HAL_LAYER_SetLayerGAlpha
* Desc : Set video/graphic layer's global alpha
*/
HI_BOOL HAL_LAYER_SetLayerGAlpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U8 u8Alpha0)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;

    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->V0_CTRL.u32));
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_CTRL.bits.galpha = u8Alpha0;
            HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_CTRL.bits.galpha = u8Alpha0;
            HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id %d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_GetLayerGAlpha(HAL_DISP_LAYER_E enLayer, HI_U8* pu8Alpha0)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->V0_CTRL.u32));
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu8Alpha0 = V0_CTRL.bits.galpha;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            *pu8Alpha0 = G0_CTRL.bits.galpha;
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetHorRatio(HAL_DISP_LAYER_E enLayer, HI_U32 uRatio)
{
    volatile U_G0_ZME_HSP G0_ZME_HSP;

    volatile HI_UL addr_REG;

    if (HAL_DISP_LAYER_GFX0 == enLayer)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_ZME_HSP.u32));
        G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_ZME_HSP.bits.hratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, G0_ZME_HSP.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*
* Name : HAL_LAYER_SetRegUp
* Desc : Set layer(video or graphic) register update.
*/
HI_BOOL  HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer)
{
    U_G0_UPD G0_UPD;
    volatile HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pHifbReg->G0_UPD.u32));
            G0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            /* graphic layer register update */
            G0_UPD.bits.regup = 0x1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_UPD.u32);
            break;
        }
        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*set layer addr*/
HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr)
{
    volatile HI_UL ul_GFX_ADDR_H = 0;
    volatile HI_UL ul_GFX_ADDR_L = 0;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        // Write low address to register.
        ul_GFX_ADDR_L   = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_ADDR_L));
        HAL_WriteReg((HI_U32*)ul_GFX_ADDR_L, GetLowAddr(u64LAddr));

        // Write high address to register.
        ul_GFX_ADDR_H   = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_ADDR_H));
        HAL_WriteReg((HI_U32*)ul_GFX_ADDR_H, GetHighAddr(u64LAddr));
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}


/*
* Name : HAL_GRAPHIC_GetGfxAddr
* Desc : get layer addr.
*/
HI_BOOL HAL_GRAPHIC_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64* pu64GfxAddr)
{
    volatile  HI_UL addr_REG;
    HI_U64    u64AddrH = 0x0;
    HI_U32    u32AddrL = 0x0;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_ADDR_L));
        u32AddrL  = HAL_ReadReg((HI_U32*)addr_REG);
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_ADDR_H));
        u64AddrH  = HAL_ReadReg((HI_U32*)addr_REG);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *pu64GfxAddr = u32AddrL + (u64AddrH << 32);
    return HI_TRUE;
}

/* layer stride */
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch)
{
    volatile  U_GFX_STRIDE   GFX_STRIDE;
    volatile  HI_UL          addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_STRIDE.u32));
        GFX_STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_STRIDE.bits.surface_stride = u16pitch;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_STRIDE.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*get layer stride*/
HI_BOOL HAL_GRAPHIC_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32* pu32GfxStride)
{
        volatile  HI_UL addr_REG;
        if( HAL_DISP_LAYER_GFX0 == enLayer ||
            HAL_DISP_LAYER_GFX1 == enLayer ||
            HAL_DISP_LAYER_GFX3 == enLayer)
        {
            addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_STRIDE.u32));
        }
        else
        {
            HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }

        *pu32GfxStride = HAL_ReadReg((HI_U32*)addr_REG);

    return HI_TRUE;
}


/* layer bit ext. */
HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                              HAL_GFX_BITEXTEND_E enMode)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.bitext = enMode;
        HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, GFX_OUT_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.premulti_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32* pbEnable)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbEnable = GFX_OUT_CTRL.bits.premulti_en;
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U32 bAlphaEn, HI_U32 bArange,
                                 HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    U_GFX_1555_ALPHA GFX_1555_ALPHA;

    volatile HI_UL addr_REG;

    if (HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.palpha_en = bAlphaEn;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);

        if (HI_TRUE == bAlphaEn)
        {
            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            GFX_1555_ALPHA.bits.alpha_1 = u8Alpha1;
            HAL_WriteReg((HI_U32*)addr_REG, GFX_1555_ALPHA.u32);


            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            GFX_1555_ALPHA.bits.alpha_0 = u8Alpha0;
            HAL_WriteReg((HI_U32*)addr_REG, GFX_1555_ALPHA.u32);
        }
        else
        {
            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)(HI_UL)addr_REG);
            GFX_1555_ALPHA.bits.alpha_1 = 0xff;
            HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, GFX_1555_ALPHA.u32);

            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)(HI_UL)addr_REG);
            GFX_1555_ALPHA.bits.alpha_0 = 0xff;
            HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, GFX_1555_ALPHA.u32);
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*
* Name : HAL_GRAPHIC_SetGfxPalphaRange
* Desc : alpha range setting
*/
HI_BOOL HAL_GRAPHIC_SetGfxPalphaRange(HAL_DISP_LAYER_E enLayer, HI_U32 bArange)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;
    volatile  HI_UL addr_REG;
    switch(enLayer)
    {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL) & (pHifbReg->GFX_OUT_CTRL.u32));
            GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            GFX_OUT_CTRL.bits.palpha_range = bArange;
            HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.key_en = u32KeyEnable;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.key_mode = u32KeyOut;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey mode in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetColorKeyValue(HAL_DISP_LAYER_E enLayer,
                                     HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin)
{
    U_GFX_CKEY_MAX GFX_CKEY_MAX;
    U_GFX_CKEY_MIN GFX_CKEY_MIN;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_CKEY_MAX.u32));
        GFX_CKEY_MAX.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_CKEY_MAX.bits.key_r_max = stKeyMax.u8KeyMax_R;
        GFX_CKEY_MAX.bits.key_g_max = stKeyMax.u8KeyMax_G;
        GFX_CKEY_MAX.bits.key_b_max = stKeyMax.u8KeyMax_B;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_CKEY_MAX.u32);

        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_CKEY_MIN.u32));
        GFX_CKEY_MIN.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_CKEY_MIN.bits.key_r_min = stKeyMin.u8KeyMin_R;
        GFX_CKEY_MIN.bits.key_g_min = stKeyMin.u8KeyMin_G;
        GFX_CKEY_MIN.bits.key_b_min = stKeyMin.u8KeyMin_B;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_CKEY_MIN.u32);
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk)
{
    U_GFX_CKEY_MASK GFX_CKEY_MASK;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_CKEY_MASK.u32));
        GFX_CKEY_MASK.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_CKEY_MASK.bits.key_r_msk = stMsk.u8Mask_r;
        GFX_CKEY_MASK.bits.key_g_msk = stMsk.u8Mask_g;
        GFX_CKEY_MASK.bits.key_b_msk = stMsk.u8Mask_b;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_CKEY_MASK.u32);
    }
    else
    {
        HAL_PRINT("Error layer id %d not support colorkey mask in %s: L%d\n",
            (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

//for gfx decmpress
HI_BOOL HAL_GRAPHIC_SetGfxDcmpEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile  HI_UL addr_REG;

    if( (HAL_DISP_LAYER_GFX0 == enLayer) ||
        (HAL_DISP_LAYER_GFX1 == enLayer) ||
        (HAL_DISP_LAYER_GFX3 == enLayer) )
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_SRC_INFO.u32));

        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_SRC_INFO.bits.dcmp_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_SRC_INFO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_GetGfxDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile  HI_UL addr_REG;
    if((HAL_DISP_LAYER_GFX0 == enLayer) || (HAL_DISP_LAYER_GFX1 == enLayer))
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pHifbReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        *pbEnable = GFX_SRC_INFO.bits.dcmp_en;
    }
    else
    {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

#ifdef MDDRDETECT

HI_BOOL HAL_MDDRC_InitMDDRDetect(HI_VOID)
{
    volatile  HI_U32 addr_REG;

    addr_REG = (HI_U32)&(pMddrcReg->clk_cfg);

    HAL_WriteReg((HI_U32*)addr_REG, 0x3);

    return HI_TRUE;
}

HI_VOID HAL_MDDRC_GetStatus(HI_U32* u32Status)
{
    volatile  HI_U32 addr_REG;

    addr_REG = (HI_U32)(HI_UL) & (pMddrcReg->awaddr_srvlnc_status);
    *u32Status = HAL_ReadReg((HI_U32*)addr_REG);
    return;
}

HI_VOID HAL_MDDRC_ClearStatus(HI_U32 u32Status)
{
    volatile HI_U32 addr_REG;

    addr_REG = (HI_U32)&(pMddrcReg->awaddr_srvlnc_status);
    HAL_WriteReg((HI_U32*)addr_REG, u32Status);
}

#endif


/***************************************************************
    Begin: Set  parameter functions
****************************************************************/
HI_VOID HAL_PARA_SetParaAddrVhdChn06(HI_U64 para_addr_vhd_chn06)
{
    volatile HI_U64 PARA_ADDR_VHD_CHN06;

    PARA_ADDR_VHD_CHN06 = para_addr_vhd_chn06;
    HAL_WriteReg((HI_U32*) & (pHifbReg->PARA_HADDR_VHD_CHN06), GetHighAddr(PARA_ADDR_VHD_CHN06));
    HAL_WriteReg((HI_U32*) & (pHifbReg->PARA_ADDR_VHD_CHN06), GetLowAddr(PARA_ADDR_VHD_CHN06));

    return ;
}


HI_VOID HAL_PARA_SetParaUpVhdChn(HI_U32 u32ChnNum)
{
    U_PARA_UP_VHD PARA_UP_VHD;

    PARA_UP_VHD.u32 = (1 << u32ChnNum);
    HAL_WriteReg((HI_U32*) & (pHifbReg->PARA_UP_VHD.u32), PARA_UP_VHD.u32);

    return ;
}

/**********************************************************************************
*  End     : Graphic layer DCMP relative hal functions.
**********************************************************************************/




/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
HI_VOID HAL_G0_ZME_SetCkGtEn(HI_U32 ck_gt_en)
{
    U_G0_ZME_HINFO G0_ZME_HINFO;

    G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HINFO.u32));
    G0_ZME_HINFO.bits.ck_gt_en = ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HINFO.u32),G0_ZME_HINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetOutWidth(HI_U32 out_width)
{
    U_G0_ZME_HINFO G0_ZME_HINFO;

    G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HINFO.u32));
    G0_ZME_HINFO.bits.out_width = out_width-1;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HINFO.u32),G0_ZME_HINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetHfirEn(HI_U32 hfir_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hfir_en = hfir_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetAhfirMidEn(HI_U32 ahfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.ahfir_mid_en = ahfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLhfirMidEn(HI_U32 lhfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.lhfir_mid_en = lhfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetChfirMidEn(HI_U32 chfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.chfir_mid_en = chfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLhfirMode(HI_U32 lhfir_mode)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.lhfir_mode = lhfir_mode;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetAhfirMode(HI_U32 ahfir_mode)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.ahfir_mode = ahfir_mode;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetHfirOrder(HI_U32 hfir_order)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hfir_order = hfir_order;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetHratio(HI_U32 hratio)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hratio = hratio;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLhfirOffset(HI_U32 lhfir_offset)
{
    U_G0_ZME_HLOFFSET G0_ZME_HLOFFSET;

    G0_ZME_HLOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HLOFFSET.u32));
    G0_ZME_HLOFFSET.bits.lhfir_offset = lhfir_offset;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HLOFFSET.u32),G0_ZME_HLOFFSET.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetChfirOffset(HI_U32 chfir_offset)
{
    U_G0_ZME_HCOFFSET G0_ZME_HCOFFSET;

    G0_ZME_HCOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_HCOFFSET.u32));
    G0_ZME_HCOFFSET.bits.chfir_offset = chfir_offset;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_HCOFFSET.u32),G0_ZME_HCOFFSET.u32);

    return ;
}

HI_VOID HAL_G0_ZME_SetOutPro(HI_U32 out_pro)
{
    U_G0_ZME_VINFO G0_ZME_VINFO;

    G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VINFO.u32));
    G0_ZME_VINFO.bits.out_pro = out_pro;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VINFO.u32),G0_ZME_VINFO.u32);

    return ;
}
HI_VOID HAL_FDR_GFX_SetDcmpEn(HI_U32 layer, HI_U32 dcmp_en)
{
    U_GFX_SRC_INFO GFX_SRC_INFO;

    if (layer >= GFX_MAX)
    {
        HI_TRACE(HI_DBG_ERR, HI_ID_FB,"Error, %s(),%d Select Wrong Layer ID\n",__FUNCTION__,__LINE__);
    }

    GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)(&(pHifbReg->GFX_SRC_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_SRC_INFO.bits.dcmp_en = dcmp_en;
    HAL_WriteReg((HI_U32*)(&(pHifbReg->GFX_SRC_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_SRC_INFO.u32);

    return ;
}

HI_VOID HAL_G0_ZME_SetOutHeight(HI_U32 out_height)
{
    U_G0_ZME_VINFO G0_ZME_VINFO;

    G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VINFO.u32));
    G0_ZME_VINFO.bits.out_height = out_height-1;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VINFO.u32),G0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVfirEn(HI_U32 vfir_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vfir_en = vfir_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetAvfirMidEn(HI_U32 avfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.avfir_mid_en = avfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLvfirMidEn(HI_U32 lvfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.lvfir_mid_en = lvfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetCvfirMidEn(HI_U32 cvfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.cvfir_mid_en = cvfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLvfirMode(HI_U32 lvfir_mode)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.lvfir_mode = lvfir_mode;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVafirMode(HI_U32 vafir_mode)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vafir_mode = vafir_mode;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVratio(HI_U32 vratio)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vratio = vratio;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVtpOffset(HI_U32 vtp_offset)
{
    U_G0_ZME_VOFFSET G0_ZME_VOFFSET;

    G0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VOFFSET.u32));
    G0_ZME_VOFFSET.bits.vtp_offset = vtp_offset;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VOFFSET.u32),G0_ZME_VOFFSET.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVbtmOffset(HI_U32 vbtm_offset)
{
    U_G0_ZME_VOFFSET G0_ZME_VOFFSET;

    G0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pHifbReg->G0_ZME_VOFFSET.u32));
    G0_ZME_VOFFSET.bits.vbtm_offset = vbtm_offset;
    HAL_WriteReg((HI_U32*)&(pHifbReg->G0_ZME_VOFFSET.u32),G0_ZME_VOFFSET.u32);

    return ;
}

hi_void hifb_hal_soft_int_en(hi_bool soft_int_en)
{
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    u_sys_reg soft_int;
    if (g_sys_reg != NULL) {
        soft_int.u32 = HAL_ReadReg((HI_U32*)&(g_sys_reg->soft_int.u32));
        soft_int.bits.software_int = (soft_int_en == HI_TRUE) ? 1 : 0;
        HAL_WriteReg((HI_U32*)&(g_sys_reg->soft_int.u32), soft_int.u32);
    }
#endif
    return ;
}

/**********************************************************************************
*  End     : Graphic layer ZME relative hal functions.
**********************************************************************************/



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

