/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_graphic_hal.h"
#include "hi_osal.h"
#include "hi_debug.h"
#include "hifb_coef.h"
#include "mddrc_reg.h"
#include "hi_math.h"
#include "hifb_def.h"

/* MACRO DEFINITION */
#define HAL_PRINT HI_PRINT

#define DDRC_BASE_ADDR 0x04605000
#define VOU_REGS_ADDR  0x11440000
#define VOU_REGS_SIZE  0x40000

/* For CMP and DCMP */
#define CMP_SEG_OFFSET  (0x80 / 4)
#define DCMP_SEG_OFFSET (0x20 / 4)

/* EXTERN VARIABLES */
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
#define SYS_REGS_ADDR 0X12020000

typedef union {
    struct {
        unsigned int software_int : 1; /* [0] */
        unsigned int reserved : 31;    /* [1 ~31] */
    } bits;
    unsigned int u32;
} u_sys_reg;

typedef struct {
    volatile hi_u32 reserved[7]; /* 0x0~0x18, reserved 7 num register */
    volatile u_sys_reg soft_int; /* 0x1c */
    volatile hi_u32 reserved1;   /* 0x20 */
} s_sys_regs_type;

volatile s_sys_regs_type *g_sys_reg = HI_NULL;
#endif

volatile s_vdp_regs_type *g_hifb_reg = HI_NULL;
volatile mddrc_regs *g_mddrc_reg = HI_NULL;

hi_s32 fb_hal_vou_init(hi_void)
{
    if (g_hifb_reg == HI_NULL) {
        g_hifb_reg = (volatile s_vdp_regs_type *)osal_ioremap(VOU_REGS_ADDR, (hi_u32)VOU_REGS_SIZE);
    }

    if (g_sys_reg == HI_NULL) {
        g_sys_reg = (volatile s_sys_regs_type *)osal_ioremap(SYS_REGS_ADDR, (hi_u32)sizeof(s_sys_regs_type));
    }

    if (g_hifb_reg == HI_NULL) {
        osal_printk("ioremap_nocache failed\n");
        return HI_FAILURE;
    }

    if (g_sys_reg == HI_NULL) {
        osal_printk("ioremap_nocache failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

hi_void fb_hal_vou_exit(hi_void)
{
    if (g_hifb_reg != HI_NULL) {
        osal_iounmap((void *)g_hifb_reg);
        g_hifb_reg = HI_NULL;
    }

    if (g_sys_reg != HI_NULL) {
        osal_iounmap((void *)g_sys_reg);
        g_sys_reg = HI_NULL;
    }
}

hi_void fb_hal_write_reg(hi_u32 *address, hi_u32 value)
{
    if (address == HI_NULL) {
        return;
    }
    *(volatile hi_u32 *)address = value;
    return;
}

hi_u32 fb_hal_read_reg(hi_u32 *address)
{
    if (address == HI_NULL) {
        return 0;
    }
    return *(volatile hi_u32 *)(address);
}

/*
 * Prototype         : fb_vou_get_abs_addr
 * Description       : Get the absolute address of the layer (video layer and graphics layer)
 */
hi_ulong fb_vou_get_abs_addr(hal_disp_layer layer, hi_ulong reg)
{
    hi_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            reg_abs_addr = (reg) + (layer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            reg_abs_addr = (reg) + (layer - HAL_DISP_LAYER_GFX0) * GFX_REGS_LEN;
            break;
        /* one wbc dev */
        case HAL_DISP_LAYER_WBC:
            reg_abs_addr = (reg);
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

/*
 * Prototype         : fb_vou_get_chn_abs_addr
 * Description       : Get the absolute address of the video channel
 */
hi_ulong fb_vou_get_chn_abs_addr(hal_disp_outputchannel chan, hi_ulong reg)
{
    volatile hi_ulong reg_abs_addr;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
            reg_abs_addr = reg + (chan - HAL_DISP_CHANNEL_DHD0) * DHD_REGS_LEN;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

static hi_u32 hal_get_addr_chnabs(hal_disp_outputchannel chan, volatile hi_u32 *value)
{
    volatile hi_ulong addr_reg;
    addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)value);
    return fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
}

static hi_u32 hal_get_addr_abs(volatile hi_ulong *addr_reg, hal_disp_layer layer, volatile hi_u32 *value)
{
    *addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)value);
    return fb_hal_read_reg ((hi_u32 *)(uintptr_t)(*addr_reg));
}

hi_ulong fb_vou_get_gfx_abs_addr(hal_disp_layer layer, hi_ulong reg)
{
    volatile hi_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;

        default:
            HAL_PRINT("Error layer id found in FUNC:%s,LINE:%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

/*
 * Name : fb_hal_disp_get_intf_enable
 * Desc : Get the status (enable,disable status) of display interface.
 */
hi_bool fb_hal_disp_get_intf_enable(hal_disp_outputchannel chan, hi_bool *intf_en)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile hi_ulong addr_reg;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
            addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)&(g_hifb_reg->dhd0_ctrl.u32));
            dhd0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *intf_en = dhd0_ctrl.bits.intf_en;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_disp_get_intf_sync(hal_disp_outputchannel chan, hal_disp_syncinfo *sync_info)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile u_dhd0_vsync1 dhd0_vsync1;
    volatile u_dhd0_vsync2 dhd0_vsync2;
    volatile u_dhd0_hsync1 dhd0_hsync1;
    volatile u_dhd0_hsync2 dhd0_hsync2;
    volatile u_dhd0_vplus1 dhd0_vplus1;
    volatile u_dhd0_vplus2 dhd0_vplus2;
    volatile u_dhd0_pwr dhd0_pwr;
    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1: {
            dhd0_ctrl.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_ctrl.u32));
            sync_info->iop = dhd0_ctrl.bits.iop;

            dhd0_hsync1.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_hsync1.u32));
            sync_info->hact = dhd0_hsync1.bits.hact + 1;
            sync_info->hbb = dhd0_hsync1.bits.hbb + 1;

            dhd0_hsync2.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_hsync2.u32));
            sync_info->hmid = (dhd0_hsync2.bits.hmid == 0) ? 0 : dhd0_hsync2.bits.hmid + 1;
            sync_info->hfb = dhd0_hsync2.bits.hfb + 1;

            /* Config VHD interface veritical timming */
            dhd0_vsync1.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_vsync1.u32));
            sync_info->vact = dhd0_vsync1.bits.vact + 1;
            sync_info->vbb = dhd0_vsync1.bits.vbb + 1;

            dhd0_vsync2.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_vsync2.u32));
            sync_info->vfb = dhd0_vsync2.bits.vfb + 1;

            /* Config VHD interface veritical bottom timming,no use in progressive mode */
            dhd0_vplus1.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_vplus1.u32));
            sync_info->bvact = dhd0_vplus1.bits.bvact + 1;
            sync_info->bvbb = dhd0_vplus1.bits.bvbb + 1;

            dhd0_vplus2.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_vplus2.u32));
            sync_info->bvfb = dhd0_vplus2.bits.bvfb + 1;

            /* Config VHD interface veritical bottom timming, */
            dhd0_pwr.u32 = hal_get_addr_chnabs(chan, &(g_hifb_reg->dhd0_pwr.u32));
            sync_info->hpw = dhd0_pwr.bits.hpw + 1;
            sync_info->vpw = dhd0_pwr.bits.vpw + 1;
            break;
        }
        default: {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    return HI_TRUE;
}

hi_bool hal_disp_get_intf_mux_sel(hal_disp_outputchannel chan, VO_INTF_TYPE_E *intf_type)
{
    volatile u_vo_mux vo_mux;
    if (chan > HAL_DISP_CHANNEL_DHD1) {
        HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    vo_mux.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->vo_mux.u32));

    switch (vo_mux.bits.digital_sel) {
        case 0: { /* 0 is Type */
            *intf_type = HAL_DISP_INTF_BT1120;
            break;
        }
        case 1: { /* 1 is Type */
            *intf_type = HAL_DISP_INTF_BT656;
            break;
        }
        case 2: { /* 2 is Type */
            *intf_type = HAL_DISP_INTF_LCD;
            break;
        }
        default: {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

hi_bool fb_hal_disp_get_int_state(hal_disp_outputchannel chan, hi_bool *bottom)
{
    volatile u_dhd0_state dhd0_state;
    volatile hi_ulong addr_reg;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
            addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)&(g_hifb_reg->dhd0_state.u32));
            dhd0_state.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *bottom = dhd0_state.bits.bottom_field;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

/*
 * Prototype         : fb_hal_disp_get_disp_iop
 * Description       : Interlace or Progressive
 */
hi_bool fb_hal_disp_get_disp_iop(hal_disp_outputchannel chan, hi_bool *iop)
{
    u_dhd0_ctrl dhd0_ctrl;
    volatile hi_ulong addr_reg;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
            addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)&(g_hifb_reg->dhd0_ctrl.u32));
            dhd0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *iop = dhd0_ctrl.bits.iop;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_disp_get_vt_thd_mode(hal_disp_outputchannel chan, hi_bool *field_mode)
{
    volatile u_dhd0_vtthd dhd0_vtthd;
    volatile hi_ulong addr_reg;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
            addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)&(g_hifb_reg->dhd0_vtthd.u32));
            dhd0_vtthd.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *field_mode = dhd0_vtthd.bits.thd1_mode;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

/*
 * Name : fb_hal_disp_set_int_mask
 * Desc : Set intterupt mask to open or close intterupt.
 */
hi_bool fb_hal_disp_set_int_mask(hi_u32 mask_en)
{
    volatile u_vointmsk1 vointmsk1;
    /* Dispaly interrupt mask enable */
    vointmsk1.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->vointmsk1.u32));
    vointmsk1.u32 = vointmsk1.u32 | mask_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->vointmsk1.u32), vointmsk1.u32);
    vointmsk1.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->vointmsk1.u32));
    return HI_TRUE;
}

/* Name : fb_hal_disp_clr_int_mask */
hi_bool fb_hal_disp_clr_int_mask(hi_u32 mask_en)
{
    volatile u_vointmsk1 vointmsk1;

    /* Dispaly interrupt mask enable */
    vointmsk1.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->vointmsk1.u32));
    vointmsk1.u32 = vointmsk1.u32 & (~mask_en);
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->vointmsk1.u32), vointmsk1.u32);

    return HI_TRUE;
}

/* Get interrupt status */
hi_u32 fb_hal_disp_get_int_status(hi_u32 int_msk)
{
    volatile u_vomskintsta1 vomskintsta1;

    /* read interrupt status */
    vomskintsta1.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->vomskintsta1.u32));

    return (vomskintsta1.u32 & int_msk);
}

/* Get the original interrupt status */
hi_u32 fb_hal_disp_get_ori_int_status(hi_u32 int_status)
{
    volatile u_vointsta vointsta;

    /* read original interrupt status */
    vointsta.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)&(g_hifb_reg->vointsta.u32));

    return (vointsta.u32 & int_status);
}

/*
 * Name : fb_hal_disp_clear_int_status
 * Desc : Clear interrupt status.
 */
hi_bool fb_hal_disp_clear_int_status(hi_u32 int_msk)
{
    /* read interrupt status */
    fb_hal_write_reg((hi_u32 *)(uintptr_t)&(g_hifb_reg->vomskintsta.u32), int_msk);
    return HI_TRUE;
}

hi_u32 hal_disp_get_raw_int_status(hi_u32 int_msk)
{
    volatile u_vointsta vointsta;

    /* read interrupt status */
    vointsta.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)&(g_hifb_reg->vointsta.u32));

    return (vointsta.u32 & int_msk);
}

hi_u32 hal_disp_get_raw_int_status1(hi_u32 int_msk)
{
    volatile u_vointsta1 vointsta1;

    /* read interrupt status */
    vointsta1.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)&(g_hifb_reg->vointsta1.u32));

    return (vointsta1.u32 & int_msk);
}

/*
 * Name : hal_disp_set_clk_gate_enable
 * Desc : Set VO Clock gate enable
 */
hi_bool hal_disp_set_clk_gate_enable(hi_u32 data)
{
    volatile u_voctrl voctrl;

    voctrl.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->voctrl.u32));
    voctrl.bits.vo_ck_gt_en = data;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->voctrl.u32), voctrl.u32);

    return HI_TRUE;
}

/*
 * Name : fb_hal_disp_set_reg_up
 * Desc : Set device register update.
 */
hi_void fb_hal_disp_set_reg_up(hal_disp_outputchannel chan)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile hi_ulong addr_reg;

    if (chan > HAL_DISP_CHANNEL_DHD1) {
        HI_PRINT("Error,fb_hal_disp_set_reg_up Select Wrong CHANNEL ID\n");
        return;
    }

    /* If DHD0 or DHD1 */
    addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)&(g_hifb_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
    dhd0_ctrl.bits.regup = 0x1;
    fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, dhd0_ctrl.u32);
    return;
}

/*
 * Name : fb_hal_disp_set_reg_up
 * Desc : Get device register update.
 */
hi_u32 fb_hal_disp_get_reg_up(hal_disp_outputchannel chan)
{
    hi_u32 data;
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile hi_ulong addr_reg;

    addr_reg = fb_vou_get_chn_abs_addr(chan, (hi_ulong)(uintptr_t)&(g_hifb_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
    data = dhd0_ctrl.bits.regup;
    return data & 0x1;
}

hi_bool fb_hal_video_set_layer_disp_rect(hal_disp_layer layer, HIFB_RECT *rect)
{
    volatile u_g0_dfpos g0_dfpos;
    volatile u_g0_dlpos g0_dlpos;
    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_dfpos.u32));
            g0_dfpos.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            g0_dfpos.bits.disp_xfpos = rect->x;
            g0_dfpos.bits.disp_yfpos = rect->y;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_dfpos.u32);

            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_dlpos.u32));
            g0_dlpos.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            g0_dlpos.bits.disp_xlpos = rect->x + rect->w - 1;
            g0_dlpos.bits.disp_ylpos = rect->y + rect->h - 1;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_dlpos.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

/* Set the video image display area window */
hi_bool fb_hal_video_set_layer_video_rect(hal_disp_layer layer, HIFB_RECT *rect)
{
    volatile u_g0_vfpos g0_vfpos;
    volatile u_g0_vlpos g0_vlpos;
    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_vfpos.u32));
            g0_vfpos.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            g0_vfpos.bits.video_xfpos = rect->x;
            g0_vfpos.bits.video_yfpos = rect->y;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_vfpos.u32);

            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_vlpos.u32));
            g0_vlpos.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            g0_vlpos.bits.video_xlpos = rect->x + rect->w - 1;
            g0_vlpos.bits.video_ylpos = rect->y + rect->h - 1;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_vlpos.u32);
            break;
        default:
            HAL_PRINT("Error layer id %d# found in %s,%s: L%d\n", layer, __FILE__, __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

/*
 * Name : fb_hal_layer_enable_layer
 * Desc : Set layer enable
 */
hi_bool fb_hal_layer_enable_layer(hal_disp_layer layer, hi_u32 enable)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            v0_ctrl.bits.surface_en = enable;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, v0_ctrl.u32);
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            g0_ctrl.bits.surface_en = enable;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool fb_hal_layer_get_layer_enable(hal_disp_layer layer, hi_u32 *enable)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;

    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *enable = v0_ctrl.bits.surface_en;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *enable = g0_ctrl.bits.surface_en;
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

/* Desc : Set layer data type */
hi_bool fb_hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_src_info.bits.ifmt = data_fmt;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_src_info.u32);
    } else {
        HAL_PRINT("Error layer id%d found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool fb_hal_layer_get_layer_data_fmt(hal_disp_layer layer, hi_u32 *fmt)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        *fmt = gfx_src_info.bits.ifmt;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_void fb_hal_layer_csc_set_enable(hal_disp_layer layer, hi_bool csc_en)
{
    u_g0_hipp_csc_ctrl g0_hipp_csc_ctrl;

    volatile hi_ulong addr_reg;

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_ctrl.u32));
        g0_hipp_csc_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_ctrl.bits.hipp_csc_en = csc_en;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_ctrl.u32);
    }
}

hi_void fb_hal_layer_csc_set_ck_gt_en(hal_disp_layer layer, hi_bool ck_gt_en)
{
    u_g0_hipp_csc_ctrl g0_hipp_csc_ctrl;

    volatile hi_ulong addr_reg;

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_ctrl.u32));
        g0_hipp_csc_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_ctrl.bits.hipp_csc_ck_gt_en = ck_gt_en;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_ctrl.u32);
    }
}

hi_void fb_hal_layer_csc_set_coef(hal_disp_layer layer, vdp_csc_coef *coef)
{
    u_g0_hipp_csc_coef00 g0_hipp_csc_coef00;
    u_g0_hipp_csc_coef01 g0_hipp_csc_coef01;
    u_g0_hipp_csc_coef02 g0_hipp_csc_coef02;
    u_g0_hipp_csc_coef10 g0_hipp_csc_coef10;
    u_g0_hipp_csc_coef11 g0_hipp_csc_coef11;
    u_g0_hipp_csc_coef12 g0_hipp_csc_coef12;
    u_g0_hipp_csc_coef20 g0_hipp_csc_coef20;
    u_g0_hipp_csc_coef21 g0_hipp_csc_coef21;
    u_g0_hipp_csc_coef22 g0_hipp_csc_coef22;
    volatile hi_ulong addr_reg;

    if ((layer >= HAL_DISP_LAYER_GFX0) && (layer <= HAL_DISP_LAYER_GFX3)) {
        g0_hipp_csc_coef00.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef00.u32));
        g0_hipp_csc_coef00.bits.hipp_csc_coef00 = coef->csc_coef00;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef00.u32);

        g0_hipp_csc_coef01.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef01.u32));
        g0_hipp_csc_coef01.bits.hipp_csc_coef01 = coef->csc_coef01;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef01.u32);

        g0_hipp_csc_coef02.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef02.u32));
        g0_hipp_csc_coef02.bits.hipp_csc_coef02 = coef->csc_coef02;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef02.u32);

        g0_hipp_csc_coef10.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef10.u32));
        g0_hipp_csc_coef10.bits.hipp_csc_coef10 = coef->csc_coef10;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef10.u32);

        g0_hipp_csc_coef11.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef11.u32));
        g0_hipp_csc_coef11.bits.hipp_csc_coef11 = coef->csc_coef11;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef11.u32);

        g0_hipp_csc_coef12.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef12.u32));
        g0_hipp_csc_coef12.bits.hipp_csc_coef12 = coef->csc_coef12;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef12.u32);

        g0_hipp_csc_coef20.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef20.u32));
        g0_hipp_csc_coef20.bits.hipp_csc_coef20 = coef->csc_coef20;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef20.u32);

        g0_hipp_csc_coef21.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef21.u32));
        g0_hipp_csc_coef21.bits.hipp_csc_coef21 = coef->csc_coef21;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef21.u32);

        g0_hipp_csc_coef22.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_hifb_reg->g0_hipp_csc_coef22.u32));
        g0_hipp_csc_coef22.bits.hipp_csc_coef22 = coef->csc_coef22;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_coef22.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
    }
    return;
}

hi_void fb_hal_layer_csc_set_dc_coef(hal_disp_layer layer, vdp_csc_dc_coef *csc_dc_coef)
{
    u_g0_hipp_csc_idc0 g0_hipp_csc_idc0;
    u_g0_hipp_csc_idc1 g0_hipp_csc_idc1;
    u_g0_hipp_csc_idc2 g0_hipp_csc_idc2;
    u_g0_hipp_csc_odc0 g0_hipp_csc_odc0;
    u_g0_hipp_csc_odc1 g0_hipp_csc_odc1;
    u_g0_hipp_csc_odc2 g0_hipp_csc_odc2;
    volatile hi_ulong addr_reg;

    if (layer == HAL_DISP_LAYER_GFX0) {
        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_idc0.u32));
        g0_hipp_csc_idc0.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_idc0.bits.hipp_csc_idc0 = csc_dc_coef->csc_in_dc0;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_idc0.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_idc1.u32));
        g0_hipp_csc_idc1.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_idc1.bits.hipp_csc_idc1 = csc_dc_coef->csc_in_dc1;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_idc1.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_idc2.u32));
        g0_hipp_csc_idc2.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_idc2.bits.hipp_csc_idc2 = csc_dc_coef->csc_in_dc2;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_idc2.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_odc0.u32));
        g0_hipp_csc_odc0.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_odc0.bits.hipp_csc_odc0 = csc_dc_coef->csc_out_dc0;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_odc0.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_odc1.u32));
        g0_hipp_csc_odc1.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_odc1.bits.hipp_csc_odc1 = csc_dc_coef->csc_out_dc1;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_odc1.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_odc2.u32));
        g0_hipp_csc_odc2.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_odc2.bits.hipp_csc_odc2 = csc_dc_coef->csc_out_dc2;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_odc2.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
    }
}

hi_void fb_hal_layer_csc_set_param(hal_disp_layer layer, csc_coef_param *coef_param)
{
    u_g0_hipp_csc_scale g0_hipp_csc_scale;
    u_g0_hipp_csc_min_y g0_hipp_csc_min_y;
    u_g0_hipp_csc_min_c g0_hipp_csc_min_c;
    u_g0_hipp_csc_max_y g0_hipp_csc_max_y;
    u_g0_hipp_csc_max_c g0_hipp_csc_max_c;

    volatile hi_ulong addr_reg;

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_scale.u32));
        g0_hipp_csc_scale.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_scale.bits.hipp_csc_scale = coef_param->csc_scale2p;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_scale.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_min_y.u32));
        g0_hipp_csc_min_y.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_min_y.bits.hipp_csc_min_y = coef_param->csc_clip_min;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_min_y.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_min_c.u32));
        g0_hipp_csc_min_c.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_min_c.bits.hipp_csc_min_c = coef_param->csc_clip_min;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_min_c.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_max_y.u32));
        g0_hipp_csc_max_y.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_max_y.bits.hipp_csc_max_y = coef_param->csc_clip_max;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_max_y.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_hipp_csc_max_c.u32));
        g0_hipp_csc_max_c.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_hipp_csc_max_c.bits.hipp_csc_max_c = coef_param->csc_clip_max;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_hipp_csc_max_c.u32);
    }
}

hi_bool fb_hal_layer_set_csc_coef(hal_disp_layer layer, csc_coef *coef)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX3)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return HI_FALSE;
    }

    fb_hal_layer_csc_set_dc_coef(layer, (vdp_csc_dc_coef *)(&coef->csc_in_dc0));
    fb_hal_layer_csc_set_coef(layer, (vdp_csc_coef *)(&coef->csc_coef00));
    fb_hal_layer_csc_set_param(layer, (csc_coef_param *)(&coef->new_csc_scale2p));

    return HI_TRUE;
}

hi_bool fb_hal_layer_set_csc_mode(hal_disp_layer layer, hi_bool is_hc_mode_by_709)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX3)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool fb_hal_layer_set_csc_en(hal_disp_layer layer, hi_bool csc_en)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX3)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return HI_FALSE;
    }

    fb_hal_layer_csc_set_ck_gt_en(layer, csc_en);
    fb_hal_layer_csc_set_enable(layer, csc_en);

    return HI_TRUE;
}

hi_bool fb_hal_layer_set_src_resolution(hal_disp_layer layer, HIFB_RECT *rect)
{
    u_gfx_src_reso gfx_src_reso;
    volatile hi_ulong addr_reg;

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_src_reso.u32));
        gfx_src_reso.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_src_reso.bits.src_w = rect->w - 1;
        gfx_src_reso.bits.src_h = rect->h - 1;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_src_reso.u32);
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool fb_hal_layer_set_layer_in_rect(hal_disp_layer layer, HIFB_RECT *rect)
{
    u_gfx_ireso gfx_ireso;
    volatile hi_ulong addr_reg;

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_ireso.u32));
        gfx_ireso.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_ireso.bits.ireso_w = rect->w - 1;
        gfx_ireso.bits.ireso_h = rect->h - 1;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_ireso.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool fb_hal_layer_set_layer_out_rect(hal_disp_layer layer, HIFB_RECT *rect)
{
    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        return HI_TRUE;
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
}

/*
 * Name : hal_layer_set_layer_galpha
 * Desc : Set video/graphic layer's global alpha
 */
hi_bool hal_layer_set_layer_galpha(hal_disp_layer layer, hi_u8 alpha0)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;

    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            v0_ctrl.bits.galpha = alpha0;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, v0_ctrl.u32);
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            g0_ctrl.bits.galpha = alpha0;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id %d found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_layer_get_layer_galpha(hal_disp_layer layer, hi_u8 *alpha0)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *alpha0 = v0_ctrl.bits.galpha;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            *alpha0 = g0_ctrl.bits.galpha;
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_layer_set_hor_ratio(hal_disp_layer layer, hi_u32 ratio)
{
    volatile u_g0_zme_hsp g0_zme_hsp;
    volatile hi_ulong addr_reg;

    if (layer == HAL_DISP_LAYER_GFX0) {
        addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_zme_hsp.u32));
        g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        g0_zme_hsp.bits.hratio = ratio;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_zme_hsp.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*
 * Name : fb_hal_layer_set_reg_up
 * Desc : Set layer(video or graphic) register update.
 */
hi_bool fb_hal_layer_set_reg_up(hal_disp_layer layer)
{
    u_g0_upd g0_upd;
    volatile hi_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_reg = fb_vou_get_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->g0_upd.u32));
            g0_upd.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            /* graphic layer register update */
            g0_upd.bits.regup = 0x1;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, g0_upd.u32);
            break;
        }
        default: {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/* set layer addr */
hi_bool hal_graphic_set_gfx_addr(hal_disp_layer layer, hi_u64 laddr)
{
    volatile hi_ulong gfx_addr_h;
    volatile hi_ulong gfx_addr_l;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        /* Write low address to register. */
        gfx_addr_l = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_addr_l));
        fb_hal_write_reg((hi_u32 *)(uintptr_t)gfx_addr_l, GetLowAddr(laddr));

        /* Write high address to register. */
        gfx_addr_h = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_addr_h));
        fb_hal_write_reg((hi_u32 *)(uintptr_t)gfx_addr_h, GetHighAddr(laddr));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*
 * Name : hal_graphic_get_gfx_addr
 * Desc : get layer addr.
 */
hi_bool hal_graphic_get_gfx_addr(hal_disp_layer layer, hi_u64 *gfx_addr)
{
    volatile hi_ulong addr_reg;
    hi_u64 addr_h = 0x0;
    hi_u32 addr_l = 0x0;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_addr_l));
        addr_l = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_addr_h));
        addr_h = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *gfx_addr = addr_l + (addr_h << 32); /* 32 max addres */
    return HI_TRUE;
}

/* layer stride */
hi_bool hal_graphic_set_gfx_stride(hal_disp_layer layer, hi_u16 pitch)
{
    volatile u_gfx_stride gfx_stride;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_stride.u32));
        gfx_stride.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_stride.bits.surface_stride = pitch;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_stride.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/* get layer stride */
hi_bool hal_graphic_get_gfx_stride(hal_disp_layer layer, hi_u32 *gfx_stride)
{
    volatile hi_ulong addr_reg;
    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_stride.u32));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *gfx_stride = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);

    return HI_TRUE;
}

/* layer bit ext. */
hi_bool hal_graphic_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode)
{
    u_gfx_out_ctrl gfx_out_ctrl;

    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.bitext = mode;
        fb_hal_write_reg ((hi_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_bool hal_graphic_set_gfx_pre_mult(hal_disp_layer layer, hi_u32 enable)
{
    u_gfx_out_ctrl gfx_out_ctrl;

    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.premulti_en = enable;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_bool hal_graphic_get_gfx_premult(hal_disp_layer layer, hi_u32 *enable)
{
    u_gfx_out_ctrl gfx_out_ctrl;

    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        *enable = gfx_out_ctrl.bits.premulti_en;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_graphic_set_gfx_palpha(hal_disp_layer layer, hi_u32 alpha_en, hi_u32 arange, hi_u8 alpha0,
                                   hi_u8 alpha1)
{
    u_gfx_out_ctrl gfx_out_ctrl;
    u_gfx_1555_alpha gfx_1555_alpha;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.palpha_en = alpha_en;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);

        if (alpha_en == HI_TRUE) {
            addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_1 = alpha1;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);

            addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_0 = alpha0;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);
        } else {
            addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_1 = 0xff; /* 0xff alpha_1 flag */
            fb_hal_write_reg ((hi_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);

            addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_0 = 0xff; /* 0xff alpha_0 flag */
            fb_hal_write_reg ((hi_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);
        }
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*
 * Name : hal_graphic_set_gfx_palphaRange
 * Desc : alpha range setting
 */
hi_bool hal_graphic_set_gfx_palpha_range(hal_disp_layer layer, hi_u32 arange)
{
    u_gfx_out_ctrl gfx_out_ctrl;
    volatile hi_ulong addr_reg;
    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
            gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
            gfx_out_ctrl.bits.palpha_range = arange;
            fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_graphic_set_gfx_key_en(hal_disp_layer layer, hi_u32 key_enable)
{
    u_gfx_out_ctrl gfx_out_ctrl;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.key_en = key_enable;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n", (hi_s32)layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_bool hal_graphic_set_gfx_key_mode(hal_disp_layer layer, hi_u32 key_out)
{
    u_gfx_out_ctrl gfx_out_ctrl;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.key_mode = key_out;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mode in %s: L%d\n", (hi_s32)layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_bool hal_graphic_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min)
{
    u_gfx_ckey_max gfx_ckey_max;
    u_gfx_ckey_min gfx_ckey_min;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_ckey_max.u32));
        gfx_ckey_max.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_max.bits.key_r_max = key_max.key_max_r;
        gfx_ckey_max.bits.key_g_max = key_max.key_max_g;
        gfx_ckey_max.bits.key_b_max = key_max.key_max_b;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_ckey_max.u32);

        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_ckey_min.u32));
        gfx_ckey_min.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_min.bits.key_r_min = key_min.key_min_r;
        gfx_ckey_min.bits.key_g_min = key_min.key_min_g;
        gfx_ckey_min.bits.key_b_min = key_min.key_min_b;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_ckey_min.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n", (hi_s32)layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

hi_bool hal_graphic_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk)
{
    u_gfx_ckey_mask gfx_ckey_mask;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_ckey_mask.u32));
        gfx_ckey_mask.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_mask.bits.key_r_msk = msk.mask_r;
        gfx_ckey_mask.bits.key_g_msk = msk.mask_g;
        gfx_ckey_mask.bits.key_b_msk = msk.mask_b;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_ckey_mask.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mask in %s: L%d\n", (hi_s32)layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/* for gfx decmpress */
hi_bool hal_graphic_set_gfx_dcmp_enable(hal_disp_layer layer, hi_u32 enable)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile hi_ulong addr_reg;

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1) || (layer == HAL_DISP_LAYER_GFX3)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_src_info.u32));

        gfx_src_info.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        gfx_src_info.bits.dcmp_en = enable;
        fb_hal_write_reg((hi_u32 *)(uintptr_t)addr_reg, gfx_src_info.u32);
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n", (hi_s32)layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_bool hal_graphic_get_gfx_dcmp_enable_state(hal_disp_layer layer, hi_bool *enable)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile hi_ulong addr_reg;
    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (hi_ulong)(uintptr_t)&(g_hifb_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((hi_u32 *)(uintptr_t)addr_reg);
        *enable = gfx_src_info.bits.dcmp_en;
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n", (hi_s32)layer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

#ifdef MDDRDETECT
hi_bool hal_mddrc_init_mddr_detect(hi_void)
{
    volatile hi_u32 addr_reg;

    addr_reg = (hi_u32) & (g_mddrc_reg->clk_cfg);

    fb_hal_write_reg((hi_u32 *)addr_reg, 0x3); /* 0x3 address */

    return HI_TRUE;
}

hi_void hal_mddrc_get_status(hi_u32 *status)
{
    volatile hi_u32 addr_reg;

    addr_reg = (hi_u32)(hi_ulong)&(g_mddrc_reg->awaddr_srvlnc_status);
    *status = fb_hal_read_reg((hi_u32 *)addr_reg);
    return;
}

hi_void hal_mddrc_clear_status(hi_u32 status)
{
    volatile hi_u32 addr_reg;

    addr_reg = (hi_u32)&(g_mddrc_reg->awaddr_srvlnc_status);
    fb_hal_write_reg((hi_u32 *)addr_reg, status);
}
#endif

hi_void hal_para_set_para_addr_vhd_chn06(hi_u64 para_addr_vhd_chn06)
{
    volatile hi_u64 addr_vhd_chn06;

    addr_vhd_chn06 = para_addr_vhd_chn06;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->para_haddr_vhd_chn06), GetHighAddr(addr_vhd_chn06));
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->para_addr_vhd_chn06), GetLowAddr(addr_vhd_chn06));

    return;
}

hi_void hal_para_set_para_up_vhd_chn(hi_u32 chn_num)
{
    u_para_up_vhd para_up_vhd;

    para_up_vhd.u32 = (1 << chn_num);
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->para_up_vhd.u32), para_up_vhd.u32);

    return;
}

hi_void hal_g0_zme_set_ck_gt_en(hi_u32 ck_gt_en)
{
    u_g0_zme_hinfo g0_zme_hinfo;

    g0_zme_hinfo.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hinfo.u32));
    g0_zme_hinfo.bits.ck_gt_en = ck_gt_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hinfo.u32), g0_zme_hinfo.u32);

    return;
}

hi_void hal_g0_zme_set_out_width(hi_u32 out_width)
{
    u_g0_zme_hinfo g0_zme_hinfo;

    g0_zme_hinfo.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hinfo.u32));
    g0_zme_hinfo.bits.out_width = out_width - 1;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hinfo.u32), g0_zme_hinfo.u32);

    return;
}

hi_void hal_g0_zme_set_hfir_en(hi_u32 hfir_en)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hfir_en = hfir_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_ahfir_mid_en(hi_u32 ahfir_mid_en)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.ahfir_mid_en = ahfir_mid_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_lhfir_mid_en(hi_u32 lhfir_mid_en)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.lhfir_mid_en = lhfir_mid_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_chfir_mid_en(hi_u32 chfir_mid_en)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.chfir_mid_en = chfir_mid_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_lhfir_mode(hi_u32 lhfir_mode)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.lhfir_mode = lhfir_mode;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_ahfir_mode(hi_u32 ahfir_mode)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.ahfir_mode = ahfir_mode;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_hfir_order(hi_u32 hfir_order)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hfir_order = hfir_order;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_hratio(hi_u32 hratio)
{
    u_g0_zme_hsp g0_zme_hsp;

    g0_zme_hsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hratio = hratio;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

hi_void hal_g0_zme_set_lhfir_offset(hi_u32 lhfir_offset)
{
    u_g0_zme_hloffset g0_zme_hloffset;

    g0_zme_hloffset.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hloffset.u32));
    g0_zme_hloffset.bits.lhfir_offset = lhfir_offset;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hloffset.u32), g0_zme_hloffset.u32);

    return;
}

hi_void hal_g0_zme_set_chfir_offset(hi_u32 chfir_offset)
{
    u_g0_zme_hcoffset g0_zme_hcoffset;

    g0_zme_hcoffset.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hcoffset.u32));
    g0_zme_hcoffset.bits.chfir_offset = chfir_offset;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_hcoffset.u32), g0_zme_hcoffset.u32);

    return;
}

hi_void hal_g0_zme_set_out_pro(hi_u32 out_pro)
{
    u_g0_zme_vinfo g0_zme_vinfo;

    g0_zme_vinfo.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vinfo.u32));
    g0_zme_vinfo.bits.out_pro = out_pro;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vinfo.u32), g0_zme_vinfo.u32);

    return;
}

hi_void hal_fdr_gfx_set_dcmp_en(hi_u32 layer, hi_u32 dcmp_en)
{
    u_gfx_src_info gfx_src_info;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_FB, "Error, %s(), %d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    gfx_src_info.u32 = fb_hal_read_reg((hi_u32 *)((uintptr_t)(&(g_hifb_reg->gfx_src_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_src_info.bits.dcmp_en = dcmp_en;
    fb_hal_write_reg ((hi_u32 *)((uintptr_t)(&(g_hifb_reg->gfx_src_info.u32)) + layer * FDR_GFX_OFFSET),
        gfx_src_info.u32);

    return;
}

hi_void hal_g0_zme_set_out_height(hi_u32 out_height)
{
    u_g0_zme_vinfo g0_zme_vinfo;

    g0_zme_vinfo.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vinfo.u32));
    g0_zme_vinfo.bits.out_height = out_height - 1;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vinfo.u32), g0_zme_vinfo.u32);

    return;
}

hi_void hal_g0_zme_set_vfir_en(hi_u32 vfir_en)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vfir_en = vfir_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_avfir_mid_en(hi_u32 avfir_mid_en)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.avfir_mid_en = avfir_mid_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_lvfir_mid_en(hi_u32 lvfir_mid_en)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.lvfir_mid_en = lvfir_mid_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_cvfir_mid_en(hi_u32 cvfir_mid_en)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.cvfir_mid_en = cvfir_mid_en;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_lvfir_mode(hi_u32 lvfir_mode)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.lvfir_mode = lvfir_mode;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_vafir_mode(hi_u32 vafir_mode)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vafir_mode = vafir_mode;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_vratio(hi_u32 vratio)
{
    u_g0_zme_vsp g0_zme_vsp;

    g0_zme_vsp.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vratio = vratio;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

hi_void hal_g0_zme_set_vtp_offset(hi_u32 vtp_offset)
{
    u_g0_zme_voffset g0_zme_voffset;

    g0_zme_voffset.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_voffset.u32));
    g0_zme_voffset.bits.vtp_offset = vtp_offset;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_voffset.u32), g0_zme_voffset.u32);

    return;
}

hi_void hal_g0_zme_set_vbtm_offset(hi_u32 vbtm_offset)
{
    u_g0_zme_voffset g0_zme_voffset;

    g0_zme_voffset.u32 = fb_hal_read_reg((hi_u32 *)&(g_hifb_reg->g0_zme_voffset.u32));
    g0_zme_voffset.bits.vbtm_offset = vbtm_offset;
    fb_hal_write_reg((hi_u32 *)&(g_hifb_reg->g0_zme_voffset.u32), g0_zme_voffset.u32);

    return;
}

hi_void hifb_hal_soft_int_en(hi_bool soft_int_en)
{
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    u_sys_reg soft_int;
    if (g_sys_reg != HI_NULL) {
        soft_int.u32 = fb_hal_read_reg((hi_u32 *)&(g_sys_reg->soft_int.u32));
        soft_int.bits.software_int = (soft_int_en == HI_TRUE) ? 1 : 0;
        fb_hal_write_reg((hi_u32 *)&(g_sys_reg->soft_int.u32), soft_int.u32);
    }
#endif
    return;
}
