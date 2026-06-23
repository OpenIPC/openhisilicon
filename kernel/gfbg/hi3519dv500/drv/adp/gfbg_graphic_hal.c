/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_graphic_hal.h"
#include "ot_osal.h"
#include "ot_debug.h"
#include "gfbg_graphics_drv.h"

/* MACRO DEFINITION */
#define HAL_PRINT       OT_PRINT

#define GFBG_REGS_ADDR   0x17A00000
#define GFBG_REGS_SIZE   0x40000

#define GFBG_FMT_CLUT_LAYER_G0   0
#define GFBG_FMT_CLUT_LAYER_G3   3
#define GFBG_FMT_CLUT_LAYER_G4   4

#define SMART_RECT_LAYER_G3   3
#define SMART_RECT_LAYER_G4   4

volatile vdp_regs_type *g_gfbg_reg = TD_NULL;

__inline static td_u32 get_low_addr(td_phys_addr_t phys_addr)
{
    return (td_u32)phys_addr;
}

__inline static td_u32 get_high_addr(td_phys_addr_t phys_addr)
{
    td_u32 low_width = 32; /* 32bit low addr */
    return (sizeof(phys_addr) > (low_width / 8)) ? (td_u32)(phys_addr >> low_width) : 0; /* 8 bits per byte */
}


td_s32 fb_hal_gfbg_init(td_void)
{
    if (g_gfbg_reg == TD_NULL) {
        g_gfbg_reg = (volatile vdp_regs_type *)osal_ioremap(GFBG_REGS_ADDR, (td_u32)GFBG_REGS_SIZE);
    }

    if (g_gfbg_reg == TD_NULL) {
        osal_printk("ioremap_nocache failed\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_void fb_hal_gfbg_deinit(td_void)
{
    if (g_gfbg_reg != TD_NULL) {
        osal_iounmap((void *)g_gfbg_reg, GFBG_REGS_SIZE);
        g_gfbg_reg = TD_NULL;
    }
}

static td_void fb_hal_write_reg(td_u32 *address, td_u32 value)
{
    if (address == TD_NULL) {
        return;
    }
    *(volatile td_u32 *)address = value;
    return;
}

static td_u32 fb_hal_read_reg(const td_u32 *address)
{
    if (address == TD_NULL) {
        return 0;
    }
    return *(volatile td_u32 *)(address);
}

/*****************************************************************************
 Prototype         : fb_hal_get_abs_addr
 Description       : Get the absolute address of the layer (video layer and graphics layer)
*****************************************************************************/
static td_ulong fb_hal_get_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            reg_abs_addr = (reg) + (layer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
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
 * Prototype         : fb_hal_get_chn_abs_addr
 * Description       : Get the absolute address of the video channel
 */
static td_ulong fb_hal_get_chn_abs_addr(hal_disp_outputchannel chan, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:
            reg_abs_addr = reg + (chan - HAL_DISP_CHANNEL_DHD0) * DHD_REGS_LEN;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

static td_u32 hal_get_addr_chnabs(hal_disp_outputchannel chan, const volatile td_u32 *value)
{
    volatile td_ulong addr_reg;
    addr_reg = fb_hal_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)value);
    return fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
}

static td_u32 hal_get_addr_abs(volatile td_ulong *addr_reg, hal_disp_layer layer, const volatile td_u32 *value)
{
    *addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)value);
    return fb_hal_read_reg((td_u32 *)(uintptr_t)(*addr_reg));
}

static td_ulong fb_hal_get_gfx_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;

        default:
            HAL_PRINT("Error layer id found in FUNC:%s,LINE:%d\n", __FUNCTION__, __LINE__);
            return 0;
    }

    return reg_abs_addr;
}

td_void fb_hal_disp_get_int_state_vcnt(hal_disp_outputchannel chan, td_u32 *vcnt)
{
    volatile u_dhd0_state dhd0_state;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:{
            dhd0_state.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_state.u32));
            *vcnt = dhd0_state.bits.vcnt;
            break;
        }
        default: {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return;
        }
    }

    return;
}

td_s32 fb_hal_disp_set_int_mask1(td_u32 mask_en)
{
    volatile u_vointmsk1 vo_intmsk1;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    /* display interrupt mask enable */
    vo_intmsk1.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)&(g_gfbg_reg->vointmsk1.u32));
    vo_intmsk1.u32 = vo_intmsk1.u32 | mask_en;
    fb_hal_write_reg((td_u32 *)(uintptr_t)&(g_gfbg_reg->vointmsk1.u32), vo_intmsk1.u32);
    return TD_SUCCESS;
}

td_s32 fb_hal_disp_clr_int_mask1(td_u32 mask_en)
{
    volatile u_vointmsk1 vo_intmsk1;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    /* display interrupt mask enable */
    vo_intmsk1.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)&(g_gfbg_reg->vointmsk1.u32));
    vo_intmsk1.u32 = vo_intmsk1.u32 & (~mask_en);
    fb_hal_write_reg((td_u32 *)(uintptr_t)&(g_gfbg_reg->vointmsk1.u32), vo_intmsk1.u32);
    return TD_SUCCESS;
}

/*
 * Name : fb_hal_disp_get_intf_enable
 * Desc : Get the status (enable,disable status) of display interface.
 */
td_s32 fb_hal_disp_get_intf_enable(hal_disp_outputchannel chan, td_bool *intf_en)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:
            addr_reg = fb_hal_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)&(g_gfbg_reg->dhd0_ctrl.u32));
            dhd0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *intf_en = dhd0_ctrl.bits.intf_en;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_disp_get_intf_sync(hal_disp_outputchannel chan, hal_disp_syncinfo *sync_info)
{
    volatile u_dhd0_ctrl dhd0_ctrl;
    volatile u_dhd0_vsync1 dhd0_vsync1;
    volatile u_dhd0_vsync2 dhd0_vsync2;
    volatile u_dhd0_hsync1 dhd0_hsync1;
    volatile u_dhd0_hsync2 dhd0_hsync2;
    volatile u_dhd0_vplus1 dhd0_vplus1;
    volatile u_dhd0_vplus2 dhd0_vplus2;
    volatile u_dhd0_pwr dhd0_pwr;
    if ((g_gfbg_reg == TD_NULL) || (sync_info == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:{
            dhd0_ctrl.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_ctrl.u32));
            sync_info->iop = dhd0_ctrl.bits.iop;

            dhd0_hsync1.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_hsync1.u32));
            sync_info->hact = dhd0_hsync1.bits.hact + 1;
            sync_info->hbb = dhd0_hsync1.bits.hbb + 1;

            dhd0_hsync2.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_hsync2.u32));
            sync_info->hmid = (dhd0_hsync2.bits.hmid == 0) ? 0 : (dhd0_hsync2.bits.hmid + 1);
            sync_info->hfb = dhd0_hsync2.bits.hfb + 1;

            /* Config VHD interface vertical timing */
            dhd0_vsync1.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vsync1.u32));
            sync_info->vact = dhd0_vsync1.bits.vact + 1;
            sync_info->vbb = dhd0_vsync1.bits.vbb + 1;

            dhd0_vsync2.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vsync2.u32));
            sync_info->vfb = dhd0_vsync2.bits.vfb + 1;

            /* Config VHD interface vertical bottom timing,no use in progressive mode */
            dhd0_vplus1.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vplus1.u32));
            sync_info->bvact = dhd0_vplus1.bits.bvact + 1;
            sync_info->bvbb = dhd0_vplus1.bits.bvbb + 1;

            dhd0_vplus2.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_vplus2.u32));
            sync_info->bvfb = dhd0_vplus2.bits.bvfb + 1;

            /* Config VHD interface vertical bottom timing, */
            dhd0_pwr.u32 = hal_get_addr_chnabs(chan, &(g_gfbg_reg->dhd0_pwr.u32));
            sync_info->hpw = dhd0_pwr.bits.hpw + 1;
            sync_info->vpw = dhd0_pwr.bits.vpw + 1;
            break;
        }
        default: {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_disp_get_intf_mux_sel(hal_disp_outputchannel chan, vo_intf_type *intf_type)
{
    volatile u_vo_mux vo_mux;
    if (chan > HAL_DISP_CHANNEL_DSD0) {
        HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if ((g_gfbg_reg == TD_NULL) || (intf_type == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    vo_mux.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->vo_mux.u32));

    switch (vo_mux.bits.digital_sel) {
        case 0: { /* 0 alg data */
            *intf_type = HAL_DISP_INTF_BT1120;
            break;
        }
        case 1: { /* 1 alg data */
            *intf_type = HAL_DISP_INTF_BT656;
            break;
        }

        default: {
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

/*****************************************************************************
 Prototype         : fb_hal_disp_get_disp_iop
 Description       : Interlace or Progressive
*****************************************************************************/
td_s32 fb_hal_disp_get_disp_iop(hal_disp_outputchannel chan, td_bool *iop)
{
    u_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (iop == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:{
            addr_reg = fb_hal_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)&(g_gfbg_reg->dhd0_ctrl.u32));
            dhd0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *iop = dhd0_ctrl.bits.iop;
            break;
        }

        default:
            gfbg_graphics_error("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_disp_get_vt_thd_mode(hal_disp_outputchannel chan, td_bool *field_mode)
{
    volatile u_dhd0_vtthd dhd0_vtthd;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (chan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        case HAL_DISP_CHANNEL_DSD0:
            addr_reg = fb_hal_get_chn_abs_addr(chan, (td_ulong)(uintptr_t)&(g_gfbg_reg->dhd0_vtthd.u32));
            dhd0_vtthd.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *field_mode = dhd0_vtthd.bits.thd1_mode;
            break;
        default:
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* Get interrupt status */
td_u32 fb_hal_disp_get_int_status(td_u32 int_msk)
{
    volatile u_vomskintsta1 vomskintsta1;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return 0;
    }

    /* read interrupt status */
    vomskintsta1.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->vomskintsta1.u32));

    return (vomskintsta1.u32 & int_msk);
}

/*
 * Name : fb_hal_disp_clear_int_status
 * Desc : Clear interrupt status.
 */
td_s32 fb_hal_disp_clear_int_status(td_u32 int_msk)
{
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    /* read interrupt status */
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->vomskintsta1.u32), int_msk);

    return TD_SUCCESS;
}

td_s32 fb_hal_video_set_layer_disp_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_g0_dfpos g0_dfpos;
    volatile u_g0_dlpos g0_dlpos;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_dfpos.u32));
            g0_dfpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_dfpos.bits.disp_xfpos = rect->x;
            g0_dfpos.bits.disp_yfpos = rect->y;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_dfpos.u32);

            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_dlpos.u32));
            g0_dlpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_dlpos.bits.disp_xlpos = rect->x + rect->width - 1;
            g0_dlpos.bits.disp_ylpos = rect->y + rect->height - 1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_dlpos.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* Set the video image display area window */
td_s32 fb_hal_video_set_layer_video_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_g0_vfpos g0_vfpos;
    volatile u_g0_vlpos g0_vlpos;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_vfpos.u32));
            g0_vfpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_vfpos.bits.video_xfpos = rect->x;
            g0_vfpos.bits.video_yfpos = rect->y;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_vfpos.u32);

            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_vlpos.u32));
            g0_vlpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_vlpos.bits.video_xlpos = rect->x + rect->width - 1;
            g0_vlpos.bits.video_ylpos = rect->y + rect->height - 1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_vlpos.u32);
            break;
        default:
            HAL_PRINT("Error layer id %d# found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_void fb_hal_link_get_hc_link(hal_disp_layer layer, td_u32 *data)
{
    volatile u_link_ctrl link_ctrl;

    if ((g_gfbg_reg == TD_NULL) || (data == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    link_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)&(g_gfbg_reg->link_ctrl.u32));
    switch (layer) {
        case HAL_DISP_LAYER_GFX4:
            *data = link_ctrl.bits.g4_link;
            break;
        case HAL_DISP_LAYER_GFX3:
            *data = link_ctrl.bits.g3_link;
            break;
        case HAL_DISP_LAYER_GFX2:
            *data = link_ctrl.bits.g2_link;
            break;
        default:
            break;
    }

    return;
}

/*
 * Name : fb_hal_set_layer_enable
 * Desc : Set layer enable
 */
td_s32 fb_hal_set_layer_enable(hal_disp_layer layer, td_u32 enable)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2: {
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            v0_ctrl.bits.surface_en = enable;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, v0_ctrl.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_ctrl.bits.surface_en = enable;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_void fb_hal_set_layer_ck_gt_en(hal_disp_layer layer, td_u32 ck_gt_en)
{
    volatile u_voctrl voctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    addr_reg = (td_ulong)(uintptr_t)&(g_gfbg_reg->voctrl.u32);
    voctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
            voctrl.bits.g0_ck_gt_en = ck_gt_en;
            break;
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
        case HAL_DISP_LAYER_GFX1:
            voctrl.bits.g1_ck_gt_en = ck_gt_en;
            break;
        case HAL_DISP_LAYER_GFX3:
            voctrl.bits.g3_ck_gt_en = ck_gt_en;
            break;
#endif
        default:
            /* Logic aren't configured for G2. Don't write the configuration to avoid affecting other layers. */
            return;
    }
    fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, voctrl.u32);
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_hal_get_layer_enable(hal_disp_layer layer, td_u32 *enable)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (enable == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2: {
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *enable = v0_ctrl.bits.surface_en;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *enable = g0_ctrl.bits.surface_en;
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}
#endif

/* Desc : Set layer data type */
td_s32 fb_hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_src_info.bits.ifmt = data_fmt;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_src_info.u32);
    } else {
        HAL_PRINT("Error layer id%d found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_layer_get_layer_data_fmt(hal_disp_layer layer, td_u32 *fmt)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (fmt == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        *fmt = gfx_src_info.bits.ifmt;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void fb_hal_layer_csc_set_enable(hal_disp_layer layer, td_bool csc_en)
{
    volatile u_g0_ot_pp_csc_ctrl g0_ot_pp_csc_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_ctrl.u32));
        g0_ot_pp_csc_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_ctrl.bits.ot_pp_csc_en = csc_en;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_ctrl.u32);
    }
}

static td_void fb_hal_layer_csc_set_ck_gt_en(hal_disp_layer layer, td_bool ck_gt_en)
{
    volatile u_g0_ot_pp_csc_ctrl g0_ot_pp_csc_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_ctrl.u32));
        g0_ot_pp_csc_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_ctrl.bits.ot_pp_csc_ck_gt_en = ck_gt_en;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_ctrl.u32);
    }
}

static td_void fb_hal_layer_csc_set_coef(hal_disp_layer layer, const vdp_csc_coef *coef)
{
    volatile u_g0_ot_pp_csc_coef00 g0_ot_pp_csc_coef00;
    volatile u_g0_ot_pp_csc_coef01 g0_ot_pp_csc_coef01;
    volatile u_g0_ot_pp_csc_coef02 g0_ot_pp_csc_coef02;
    volatile u_g0_ot_pp_csc_coef10 g0_ot_pp_csc_coef10;
    volatile u_g0_ot_pp_csc_coef11 g0_ot_pp_csc_coef11;
    volatile u_g0_ot_pp_csc_coef12 g0_ot_pp_csc_coef12;
    volatile u_g0_ot_pp_csc_coef20 g0_ot_pp_csc_coef20;
    volatile u_g0_ot_pp_csc_coef21 g0_ot_pp_csc_coef21;
    volatile u_g0_ot_pp_csc_coef22 g0_ot_pp_csc_coef22;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= HAL_DISP_LAYER_GFX0) && (layer <= HAL_DISP_LAYER_GFX4)) {
        g0_ot_pp_csc_coef00.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef00.u32));
        g0_ot_pp_csc_coef00.bits.ot_pp_csc_coef00 = coef->csc_coef00;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef00.u32);

        g0_ot_pp_csc_coef01.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef01.u32));
        g0_ot_pp_csc_coef01.bits.ot_pp_csc_coef01 = coef->csc_coef01;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef01.u32);

        g0_ot_pp_csc_coef02.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef02.u32));
        g0_ot_pp_csc_coef02.bits.ot_pp_csc_coef02 = coef->csc_coef02;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef02.u32);

        g0_ot_pp_csc_coef10.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef10.u32));
        g0_ot_pp_csc_coef10.bits.ot_pp_csc_coef10 = coef->csc_coef10;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef10.u32);

        g0_ot_pp_csc_coef11.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef11.u32));
        g0_ot_pp_csc_coef11.bits.ot_pp_csc_coef11 = coef->csc_coef11;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef11.u32);

        g0_ot_pp_csc_coef12.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef12.u32));
        g0_ot_pp_csc_coef12.bits.ot_pp_csc_coef12 = coef->csc_coef12;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef12.u32);

        g0_ot_pp_csc_coef20.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef20.u32));
        g0_ot_pp_csc_coef20.bits.ot_pp_csc_coef20 = coef->csc_coef20;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef20.u32);

        g0_ot_pp_csc_coef21.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef21.u32));
        g0_ot_pp_csc_coef21.bits.ot_pp_csc_coef21 = coef->csc_coef21;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef21.u32);

        g0_ot_pp_csc_coef22.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef22.u32));
        g0_ot_pp_csc_coef22.bits.ot_pp_csc_coef22 = coef->csc_coef22;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef22.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %d\n", __FUNCTION__, __LINE__);
    }
    return;
}

static td_void fb_hal_layer_csc_set_dc_coef(hal_disp_layer layer, const vdp_csc_dc_coef *csc_dc_coef)
{
    volatile u_g0_ot_pp_csc_idc0 g0_ot_pp_csc_idc0;
    volatile u_g0_ot_pp_csc_idc1 g0_ot_pp_csc_idc1;
    volatile u_g0_ot_pp_csc_idc2 g0_ot_pp_csc_idc2;
    volatile u_g0_ot_pp_csc_odc0 g0_ot_pp_csc_odc0;
    volatile u_g0_ot_pp_csc_odc1 g0_ot_pp_csc_odc1;
    volatile u_g0_ot_pp_csc_odc2 g0_ot_pp_csc_odc2;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    if ((layer >= HAL_DISP_LAYER_GFX0) && (layer <= HAL_DISP_LAYER_GFX4)) {
        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_idc0.u32));
        g0_ot_pp_csc_idc0.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_idc0.bits.ot_pp_csc_idc0 = csc_dc_coef->csc_in_dc0;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_idc0.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_idc1.u32));
        g0_ot_pp_csc_idc1.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_idc1.bits.ot_pp_csc_idc1 = csc_dc_coef->csc_in_dc1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_idc1.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_idc2.u32));
        g0_ot_pp_csc_idc2.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_idc2.bits.ot_pp_csc_idc2 = csc_dc_coef->csc_in_dc2;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_idc2.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_odc0.u32));
        g0_ot_pp_csc_odc0.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_odc0.bits.ot_pp_csc_odc0 = csc_dc_coef->csc_out_dc0;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_odc0.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_odc1.u32));
        g0_ot_pp_csc_odc1.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_odc1.bits.ot_pp_csc_odc1 = csc_dc_coef->csc_out_dc1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_odc1.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_odc2.u32));
        g0_ot_pp_csc_odc2.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_odc2.bits.ot_pp_csc_odc2 = csc_dc_coef->csc_out_dc2;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_odc2.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %d\n", __FUNCTION__, __LINE__);
    }
}

static td_void fb_hal_layer_csc_set_param(hal_disp_layer layer, const csc_coef_param *coef_param)
{
    volatile u_g0_ot_pp_csc_scale g0_ot_pp_csc_scale;
    volatile u_g0_ot_pp_csc_min_y g0_ot_pp_csc_min_y;
    volatile u_g0_ot_pp_csc_min_c g0_ot_pp_csc_min_c;
    volatile u_g0_ot_pp_csc_max_y g0_ot_pp_csc_max_y;
    volatile u_g0_ot_pp_csc_max_c g0_ot_pp_csc_max_c;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_scale.u32));
        g0_ot_pp_csc_scale.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_scale.bits.ot_pp_csc_scale = coef_param->csc_scale2p;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_scale.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_min_y.u32));
        g0_ot_pp_csc_min_y.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_min_y.bits.ot_pp_csc_min_y = coef_param->csc_clip_min;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_min_y.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_min_c.u32));
        g0_ot_pp_csc_min_c.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_min_c.bits.ot_pp_csc_min_c = coef_param->csc_clip_min;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_min_c.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_max_y.u32));
        g0_ot_pp_csc_max_y.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_max_y.bits.ot_pp_csc_max_y = coef_param->csc_clip_max;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_max_y.u32);

        addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_max_c.u32));
        g0_ot_pp_csc_max_c.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_max_c.bits.ot_pp_csc_max_c = coef_param->csc_clip_max;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_max_c.u32);
    }
}

td_s32 fb_hal_layer_set_csc_coef(hal_disp_layer layer, const csc_coef *coef,
    const csc_coef_param *coef_param)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX4)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return TD_FAILURE;
    }

    fb_hal_layer_csc_set_dc_coef(layer, (vdp_csc_dc_coef *)(&coef->csc_in_dc0));
    fb_hal_layer_csc_set_coef(layer, (vdp_csc_coef *)(&coef->csc_coef00));
    fb_hal_layer_csc_set_param(layer, coef_param);

    return TD_SUCCESS;
}

td_s32 fb_hal_layer_set_csc_en(hal_disp_layer layer, td_bool csc_en)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX4)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return TD_FAILURE;
    }

    fb_hal_layer_csc_set_ck_gt_en(layer, TD_TRUE); /* Turn on low power consumption */
    fb_hal_layer_csc_set_enable(layer, csc_en);

    return TD_SUCCESS;
}

td_s32 fb_hal_layer_set_src_resolution(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_gfx_src_reso gfx_src_reso;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_reso.u32));
        gfx_src_reso.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_src_reso.bits.src_w = rect->width - 1;
        gfx_src_reso.bits.src_h = rect->height - 1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_src_reso.u32);
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_layer_set_layer_in_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_gfx_ireso gfx_ireso;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ireso.u32));
        gfx_ireso.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ireso.bits.ireso_w = rect->width - 1;
        gfx_ireso.bits.ireso_h = rect->height - 1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ireso.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_layer_set_layer_out_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    ot_unused(rect);
    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        return TD_SUCCESS;
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
}

/*
 * Name : hal_layer_set_layer_galpha
 * Desc : Set video/graphic layer's global alpha
 */
td_s32 fb_hal_layer_set_layer_galpha(hal_disp_layer layer, td_u8 alpha0)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            v0_ctrl.bits.galpha = alpha0;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, v0_ctrl.u32);
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_ctrl.bits.galpha = alpha0;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id %d found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_layer_get_layer_galpha(hal_disp_layer layer, td_u8 *alpha0)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (alpha0 == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *alpha0 = v0_ctrl.bits.galpha;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *alpha0 = g0_ctrl.bits.galpha;
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/*
 * Name : fb_hal_layer_set_reg_up
 * Desc : Set layer(video or graphic) register update.
 */
td_s32 fb_hal_layer_set_reg_up(hal_disp_layer layer)
{
    volatile u_v0_upd v0_upd;
    volatile u_g0_upd g0_upd;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2: {
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_upd.u32));
            v0_upd.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            /* video layer register update */
            v0_upd.bits.regup = 0x1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, v0_upd.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4: {
            addr_reg = fb_hal_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_upd.u32));
            g0_upd.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            /* graphic layer register update */
            g0_upd.bits.regup = 0x1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_upd.u32);
            break;
        }
        default: {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

/* set layer addr */
td_s32 fb_hal_graphic_set_gfx_addr(hal_disp_layer layer, td_phys_addr_t laddr)
{
    volatile td_ulong gfx_addr_h;
    volatile td_ulong gfx_addr_l;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        /* Write low address to register. */
        gfx_addr_l = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_l));
        fb_hal_write_reg((td_u32 *)(uintptr_t)gfx_addr_l, get_low_addr(laddr));

        /* Write high address to register. */
        gfx_addr_h = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_h));
        fb_hal_write_reg((td_u32 *)(uintptr_t)gfx_addr_h, get_high_addr(laddr));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * Name : hal_graphic_get_gfx_addr
 * Desc : get layer addr.
 */
td_s32 fb_hal_graphic_get_gfx_addr(hal_disp_layer layer, td_phys_addr_t *gfx_addr)
{
    volatile td_ulong addr_reg;
    volatile td_phys_addr_t addr_h = 0x0;
    volatile td_phys_addr_t addr_l = 0x0;

    if ((g_gfbg_reg == TD_NULL) || (gfx_addr == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_l));
        addr_l = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_h));
        addr_h = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    *gfx_addr = addr_l + ((td_u64)addr_h << 32); /* 32 max address */
    return TD_SUCCESS;
}

/* layer stride */
td_s32 fb_hal_graphic_set_gfx_stride(hal_disp_layer layer, td_u16 pitch)
{
    volatile u_gfx_stride gfx_stride;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_stride.u32));
        gfx_stride.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_stride.bits.surface_stride = pitch;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_stride.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* get layer stride */
td_s32 fb_hal_graphic_get_gfx_stride(hal_disp_layer layer, td_u32 *gfx_stride)
{
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (gfx_stride == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_stride.u32));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    *gfx_stride = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);

    return TD_SUCCESS;
}

/* layer bit ext. */
td_s32 fb_hal_graphic_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.bitext = mode;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_gfx_pre_mult(hal_disp_layer layer, td_u32 enable)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.premulti_en = enable;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_get_gfx_premult(hal_disp_layer layer, td_u32 *enable)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (enable == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        *enable = gfx_out_ctrl.bits.premulti_en;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_gfx_palpha(hal_disp_layer layer, td_u32 alpha_en, td_u32 arange,
                                     td_u8 alpha0, td_u8 alpha1)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile u_gfx_1555_alpha gfx_1555_alpha;
    volatile td_ulong addr_reg;
    ot_unused(arange);

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.palpha_en = alpha_en;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);

        if (alpha_en == TD_TRUE) {
            addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_1 = alpha1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);

            addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_0 = alpha0;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);
        } else {
            addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_1 = 0xff;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);

            addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_0 = 0xff;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);
        }
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_gfx_key_en(hal_disp_layer layer, td_u32 key_enable)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.enable = key_enable;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
                  (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_gfx_key_mode(hal_disp_layer layer, td_u32 key_out)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.key_mode = key_out;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mode in %s: L%d\n",
                  (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max,
                                          hal_gfx_key_min key_min)
{
    volatile u_gfx_ckey_max gfx_ckey_max;
    volatile u_gfx_ckey_min gfx_ckey_min;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ckey_max.u32));
        gfx_ckey_max.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_max.bits.key_r_max = key_max.key_max_r;
        gfx_ckey_max.bits.key_g_max = key_max.key_max_g;
        gfx_ckey_max.bits.key_b_max = key_max.key_max_b;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ckey_max.u32);

        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ckey_min.u32));
        gfx_ckey_min.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_min.bits.key_r_min = key_min.key_min_r;
        gfx_ckey_min.bits.key_g_min = key_min.key_min_g;
        gfx_ckey_min.bits.key_b_min = key_min.key_min_b;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ckey_min.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
                  (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk)
{
    volatile u_gfx_ckey_mask gfx_ckey_mask;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ckey_mask.u32));
        gfx_ckey_mask.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_mask.bits.key_r_msk = msk.mask_r;
        gfx_ckey_mask.bits.key_g_msk = msk.mask_g;
        gfx_ckey_mask.bits.key_b_msk = msk.mask_b;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ckey_mask.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mask in %s: L%d\n", (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* for gfx decmpress */
td_s32 fb_hal_graphic_set_gfx_dcmp_enable(hal_disp_layer layer, td_u32 enable)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));

        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_src_info.bits.dcmp_en = enable;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_src_info.u32);
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n", (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 fb_hal_graphic_get_gfx_dcmp_enable_state(hal_disp_layer layer, td_bool *enable)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (enable == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1)) {
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        *enable = gfx_src_info.bits.dcmp_en;
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n", (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 fb_hal_graphic_set_gfx_dcmp_addr(hal_disp_layer layer, td_phys_addr_t addr_ar)
{
    volatile td_u32 gfx_addr_h;
    volatile td_u32 gfx_addr_l;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1)) {
        /* DCMP low addr for AR */
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_l));
        gfx_addr_l = get_low_addr(addr_ar);
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_addr_l);

        /* DCMP high addr for AR */
        addr_reg = fb_hal_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_h));
        gfx_addr_h = get_high_addr(addr_ar);
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_addr_h);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_void fb_hal_para_set_para_addr_vhd_chn06(td_phys_addr_t addr)
{
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn06), get_high_addr(addr));
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn06), get_low_addr(addr));

    return;
}

td_void fb_hal_para_set_para_up_vhd_chn(td_u32 chn_num)
{
    volatile u_para_up_vhd para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    para_up_vhd.u32 = (1 << chn_num);
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);

    return;
}

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
td_void fb_hal_fdr_gfx_set_source_mode(td_u32 layer, td_u32 source_mode)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.osd_mode = source_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);
    return;
}

td_void fb_hal_fdr_gfx_set_cmp_mode(td_u32 layer, td_u32 cmp_mode)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.cmp_mode = cmp_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_fdr_gfx_set_is_loss_lessa(td_u32 layer, td_u32 is_lossless_a)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.is_lossless_a = is_lossless_a;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_fdr_gfx_set_is_loss_less(td_u32 layer, td_u32 is_lossless)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.is_lossless = is_lossless;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);

    return;
}
#endif

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
td_void fb_hal_v3r2_gfx_set_source_mode(td_u32 layer, td_u32 source_mode)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.osd_mode = source_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_v3r2_gfx_set_is_loss_less(td_u32 layer, td_u32 is_lossless)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.is_lossless = is_lossless;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_v3r2_gfx_set_is_conv_en(td_u32 layer, td_u32 conv_en)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.conv_en = conv_en;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_v3r2_gfx_set_cmp_mode(td_u32 layer, td_u32 cmp_mode)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.cmp_mode = cmp_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_v3r2_gfx_set_is_ice_en(td_u32 layer, td_u32 ice_en)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.ice_en = ice_en;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_v3r2_gfx_set_frame_size(td_u32 layer, td_u32 width, td_u32 height)
{
    volatile u_vdp_v3r2_line_osd_dcmp_frame_size frame_size;

    if (layer >= GFBG_MAX_LAYER_NUM) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    frame_size.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_frame_size.u32)) +
        layer * FDR_GFX_OFFSET));
    frame_size.bits.frame_width = width - 1;
    frame_size.bits.frame_height = height - 1;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_frame_size.u32)) +
        layer * FDR_GFX_OFFSET), frame_size.u32);
}
#endif
#endif

/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
td_void hal_g0_zme_set_ck_gt_en(td_u32 ck_gt_en)
{
    volatile u_g0_zme_hinfo g0_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32));
    g0_zme_hinfo.bits.ck_gt_en = ck_gt_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32), g0_zme_hinfo.u32);

    return;
}

td_void hal_g0_zme_set_out_width(td_u32 out_width)
{
    volatile u_g0_zme_hinfo g0_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32));
    g0_zme_hinfo.bits.out_width = out_width - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32), g0_zme_hinfo.u32);

    return;
}

td_void hal_g0_zme_set_hfir_en(td_u32 hfir_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hfir_en = hfir_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_ahfir_mid_en(td_u32 ahfir_mid_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.ahfir_mid_en = ahfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_lhfir_mid_en(td_u32 lhfir_mid_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.lhfir_mid_en = lhfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_chfir_mid_en(td_u32 chfir_mid_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.chfir_mid_en = chfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_lhfir_mode(td_u32 lhfir_mode)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.lhfir_mode = lhfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_ahfir_mode(td_u32 ahfir_mode)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.ahfir_mode = ahfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_hfir_order(td_u32 hfir_order)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hfir_order = hfir_order;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_hratio(td_u32 hratio)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hratio = hratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}

td_void hal_g0_zme_set_lhfir_offset(td_u32 lhfir_offset)
{
    volatile u_g0_zme_hloffset g0_zme_hloffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hloffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hloffset.u32));
    g0_zme_hloffset.bits.lhfir_offset = lhfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hloffset.u32), g0_zme_hloffset.u32);

    return;
}

td_void hal_g0_zme_set_chfir_offset(td_u32 chfir_offset)
{
    volatile u_g0_zme_hcoffset g0_zme_hcoffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_hcoffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hcoffset.u32));
    g0_zme_hcoffset.bits.chfir_offset = chfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hcoffset.u32), g0_zme_hcoffset.u32);

    return;
}

td_void hal_g0_zme_set_out_pro(td_u32 out_pro)
{
    volatile u_g0_zme_vinfo g0_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32));
    g0_zme_vinfo.bits.out_pro = out_pro;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32), g0_zme_vinfo.u32);

    return;
}

td_void hal_g0_zme_set_out_height(td_u32 out_height)
{
    volatile u_g0_zme_vinfo g0_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32));
    g0_zme_vinfo.bits.out_height = out_height - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32), g0_zme_vinfo.u32);

    return;
}

td_void hal_g0_zme_set_vfir_en(td_u32 vfir_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vfir_en = vfir_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_avfir_mid_en(td_u32 avfir_mid_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.avfir_mid_en = avfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_lvfir_mid_en(td_u32 lvfir_mid_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.lvfir_mid_en = lvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_cvfir_mid_en(td_u32 cvfir_mid_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.cvfir_mid_en = cvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_lvfir_mode(td_u32 lvfir_mode)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.lvfir_mode = lvfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_vafir_mode(td_u32 vafir_mode)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vafir_mode = vafir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_vratio(td_u32 vratio)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vratio = vratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}

td_void hal_g0_zme_set_vtp_offset(td_u32 vtp_offset)
{
    volatile u_g0_zme_voffset g0_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32));
    g0_zme_voffset.bits.vtp_offset = vtp_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32), g0_zme_voffset.u32);

    return;
}

td_void hal_g0_zme_set_vbtm_offset(td_u32 vbtm_offset)
{
    volatile u_g0_zme_voffset g0_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g0_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32));
    g0_zme_voffset.bits.vbtm_offset = vbtm_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_voffset.u32), g0_zme_voffset.u32);

    return;
}

/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
td_void hal_g1_zme_set_ck_gt_en(td_u32 ck_gt_en)
{
    volatile u_g1_zme_hinfo g1_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32));
    g1_zme_hinfo.bits.ck_gt_en = ck_gt_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32), g1_zme_hinfo.u32);

    return;
}

td_void hal_g1_zme_set_out_width(td_u32 out_width)
{
    volatile u_g1_zme_hinfo g1_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32));
    g1_zme_hinfo.bits.out_width = out_width - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32), g1_zme_hinfo.u32);

    return;
}

td_void hal_g1_zme_set_hfir_en(td_u32 hfir_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.hfir_en = hfir_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_ahfir_mid_en(td_u32 ahfir_mid_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.ahfir_mid_en = ahfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_lhfir_mid_en(td_u32 lhfir_mid_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.lhfir_mid_en = lhfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_chfir_mid_en(td_u32 chfir_mid_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.chfir_mid_en = chfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_lhfir_mode(td_u32 lhfir_mode)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.lhfir_mode = lhfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_ahfir_mode(td_u32 ahfir_mode)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.ahfir_mode = ahfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_hfir_order(td_u32 hfir_order)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.hfir_order = hfir_order;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_hratio(td_u32 hratio)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.hratio = hratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_lhfir_offset(td_u32 lhfir_offset)
{
    volatile u_g1_zme_hloffset g1_zme_hloffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hloffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hloffset.u32));
    g1_zme_hloffset.bits.lhfir_offset = lhfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hloffset.u32), g1_zme_hloffset.u32);

    return;
}

td_void hal_g1_zme_set_chfir_offset(td_u32 chfir_offset)
{
    volatile u_g1_zme_hcoffset g1_zme_hcoffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hcoffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hcoffset.u32));
    g1_zme_hcoffset.bits.chfir_offset = chfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hcoffset.u32), g1_zme_hcoffset.u32);

    return;
}

td_void hal_g1_zme_set_out_pro(td_u32 out_pro)
{
    volatile u_g1_zme_vinfo g1_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32));
    g1_zme_vinfo.bits.out_pro = out_pro;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32), g1_zme_vinfo.u32);

    return;
}

td_void hal_g1_zme_set_out_height(td_u32 out_height)
{
    volatile u_g1_zme_vinfo g1_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32));
    g1_zme_vinfo.bits.out_height = out_height - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32), g1_zme_vinfo.u32);

    return;
}

td_void hal_g1_zme_set_vfir_en(td_u32 vfir_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.vfir_en = vfir_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_avfir_mid_en(td_u32 avfir_mid_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.avfir_mid_en = avfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_lvfir_mid_en(td_u32 lvfir_mid_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.lvfir_mid_en = lvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_cvfir_mid_en(td_u32 cvfir_mid_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.cvfir_mid_en = cvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_lvfir_mode(td_u32 lvfir_mode)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.lvfir_mode = lvfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_vafir_mode(td_u32 vafir_mode)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.vafir_mode = vafir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_vratio(td_u32 vratio)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.vratio = vratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_vtp_offset(td_u32 vtp_offset)
{
    volatile u_g1_zme_voffset g1_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32));
    g1_zme_voffset.bits.vtp_offset = vtp_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32), g1_zme_voffset.u32);

    return;
}

td_void hal_g1_zme_set_vbtm_offset(td_u32 vbtm_offset)
{
    volatile u_g1_zme_voffset g1_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32));
    g1_zme_voffset.bits.vbtm_offset = vbtm_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32), g1_zme_voffset.u32);

    return;
}
#endif

#ifndef CONFIG_TDE_CLUT_RECT_V2
td_void hal_clut_set_up_param(td_u32 layer_id, td_phys_addr_t clut_phy_addr)
{
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id != GFBG_FMT_CLUT_LAYER_G3) {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
        return;
    }
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn11), get_high_addr(clut_phy_addr));
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn11), get_low_addr(clut_phy_addr));
}

td_void hal_clut_set_up(td_u32 layer_id)
{
    volatile u_para_up_vhd  para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("g_gfbg_reg is NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id != GFBG_FMT_CLUT_LAYER_G3) {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
        return;
    }
    para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
    /* up */
    para_up_vhd.bits.para_up_vhd_chn11 = 0x1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
}
#else
td_void hal_clut_set_up_param(td_u32 layer_id, td_phys_addr_t clut_phy_addr)
{
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id == GFBG_FMT_CLUT_LAYER_G3 || layer_id == GFBG_FMT_CLUT_LAYER_G0) {
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn07), get_high_addr(clut_phy_addr));
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn07), get_low_addr(clut_phy_addr));
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    } else if (layer_id == GFBG_FMT_CLUT_LAYER_G4) {
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn06), get_high_addr(clut_phy_addr));
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn06), get_low_addr(clut_phy_addr));
#endif
    } else {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
    }
}

td_void hal_clut_set_up(td_u32 layer_id)
{
    volatile u_para_up_vhd  para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
    if (layer_id == GFBG_FMT_CLUT_LAYER_G3 || layer_id == GFBG_FMT_CLUT_LAYER_G0) {
        /* up */
        para_up_vhd.bits.para_up_vhd_chn07 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    } else if (layer_id == GFBG_FMT_CLUT_LAYER_G4) {
        /* up */
        para_up_vhd.bits.para_up_vhd_chn06 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
#endif
    } else {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
    }
}

#endif

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
#ifndef CONFIG_GFBG_SMART_RECT_V2
td_void hal_smart_rect_set_up(td_u32 layer_id, td_phys_addr_t phys_addr)
{
    volatile u_osb_mute_bk osb_mute_bk;

    if (layer_id != SMART_RECT_LAYER_G3) {
        return;
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn05), get_high_addr(phys_addr));
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn05), get_low_addr(phys_addr));

    /* enable OSB_MUTE_BK */
    osb_mute_bk.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32));
    osb_mute_bk.bits.osb_mute_en = 0x1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32), osb_mute_bk.u32);
}
#else
td_void hal_smart_rect_set_up(td_u32 layer_id, td_phys_addr_t phys_addr)
{
    volatile u_g0_mute_bk g0_mute_bk;
    volatile u_g0_lbox_ctrl g0_lbox_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id == SMART_RECT_LAYER_G3) {
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn03), get_high_addr(phys_addr));
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn03), get_low_addr(phys_addr));
    } else if (layer_id == SMART_RECT_LAYER_G4) {
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn02), get_high_addr(phys_addr));
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn02), get_low_addr(phys_addr));
    }

    g0_mute_bk.u32 = 0x0;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_mute_bk.u32)) + layer_id * FDR_GFX_OFFSET),
        g0_mute_bk.u32);

    g0_lbox_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) +
        layer_id * FDR_GFX_OFFSET));
    g0_lbox_ctrl.bits.mute_en = 0x1;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) + layer_id * FDR_GFX_OFFSET),
        g0_lbox_ctrl.u32);
    return;
}
#endif

td_void hal_smart_rect_up_param(td_u32 layer_id)
{
    volatile u_para_up_vhd  para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
#ifndef CONFIG_GFBG_SMART_RECT_V2
    /* up */
    if (layer_id == SMART_RECT_LAYER_G3) {
        para_up_vhd.bits.para_up_vhd_chn05 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    }
#else
    if (layer_id == SMART_RECT_LAYER_G3) {
        para_up_vhd.bits.para_up_vhd_chn03 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    } else if (layer_id == SMART_RECT_LAYER_G4) {
        para_up_vhd.bits.para_up_vhd_chn02 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    }
#endif
    return;
}

#ifndef CONFIG_GFBG_SMART_RECT_V2
td_void hal_smart_rect_disable(td_u32 layer_id)
{
    volatile u_osb_mute_bk osb_mute_bk;
    ot_unused(layer_id);
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    /* disable OSB_MUTE_BK */
    osb_mute_bk.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32));
    osb_mute_bk.bits.osb_mute_en = 0x0;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32), osb_mute_bk.u32);
}
#else
td_void hal_smart_rect_disable(td_u32 layer_id)
{
    volatile u_g0_lbox_ctrl g0_lbox_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_lbox_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) +
        layer_id * FDR_GFX_OFFSET));
    g0_lbox_ctrl.bits.mute_en = 0x0;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) + layer_id * FDR_GFX_OFFSET),
        g0_lbox_ctrl.u32);
    return;
}
#endif
#endif

td_void hal_gfx_set_bind_mode(td_bool is_sync)
{
    volatile u_gfx_mac_ctrl gfx_mac_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_mac_ctrl.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_mac_ctrl));
    if (is_sync == TD_TRUE) {
        gfx_mac_ctrl.bits.req_ld_mode = 1; /* bind G0 */
    } else {
        gfx_mac_ctrl.bits.req_ld_mode = 0; /* not bind */
    }
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_mac_ctrl.u32), gfx_mac_ctrl.u32);
    return;
}

td_void hal_gfx_set_hardware_mute_clr(td_u32 hw_mute_clr)
{
    volatile u_gfx_ld_ctrl gfx_ld_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_ld_ctrl.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl));
    gfx_ld_ctrl.bits.hw_mute_clr = hw_mute_clr;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl.u32), gfx_ld_ctrl.u32);
    return;
}

td_void hal_gfx_set_tde_safe_dis(td_u32 start_safe_dis)
{
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_tde_safe_dis), start_safe_dis);
    return;
}

td_void hal_gfx_set_ld_mute_en(td_u32 ld_mute_en)
{
    volatile u_gfx_ld_ctrl gfx_ld_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_ld_ctrl.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl));
    gfx_ld_ctrl.bits.ld_mute_en = ld_mute_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl.u32), gfx_ld_ctrl.u32);
    return;
}

