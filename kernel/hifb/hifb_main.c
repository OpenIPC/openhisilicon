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

#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/version.h>

#include <asm/uaccess.h>

#include <asm/types.h>
#include <asm/stat.h>
#include <asm/fcntl.h>

#include <linux/interrupt.h>
#include <linux/spinlock_types.h>
#include <linux/seq_file.h>

#include "hi_debug.h"

#include "hifb_vou_drv.h"
#include "hifb_vou_graphics.h"
#include "hifb_proc.h"
#include "hifb.h"
#include "hifb_blit.h"

#include "hifb_vsync.h"
#include "hi_tde_ext.h"

#include "hifb_main.h"
#include "mm_ext.h"

#ifndef __HuaweiLite__
#include "hifb_dmabuf.h"
#endif

#include "proc_ext.h"
#include "sys_ext.h"

#ifndef __HuaweiLite__
#include <linux/of_platform.h>
#endif

#ifdef __HuaweiLite__
#include <proc_fs.h>
#include "hi_module_param.h"
#endif

#ifdef CONFIG_HIFB_FENCE_SUPPORT
#include "drv_hifb_fence.h"
#endif

#ifndef CONFIG_HI_VO_FB_SEPARATE
#include "vgs_ext.h"
#endif



#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)

#define HIFB_MAX_WIDTH(u32LayerId)  s_stDrvOps.pstCapability[u32LayerId].u32MaxWidth
#define HIFB_MAX_HEIGHT(u32LayerId) s_stDrvOps.pstCapability[u32LayerId].u32MaxHeight
#define HIFB_MIN_WIDTH(u32LayerId)  s_stDrvOps.pstCapability[u32LayerId].u32MinWidth
#define HIFB_MIN_HEIGHT(u32LayerId) s_stDrvOps.pstCapability[u32LayerId].u32MinHeight

#define HIFB_FB_INFO(u32LayerId) (gphifb_fb_infos[u32LayerId])

#define HIFB_CMAP_LEN 256
#define HIFB_ROTBUF_NAME_LEN 16

extern unsigned int g_fifb_irq;
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    #ifdef __HuaweiLite__
        static hi_u32 g_fifb_soft_irq = 83;
    #else
        static hi_u32 g_fifb_soft_irq = 0;
    #endif
    static hi_bool g_hifb_soft_irq_register = HI_FALSE;
#endif

#define PAGE_SIZE_ALIGN_MAX ((~0ul - PAGE_SIZE) / 1024)

#define IS_SOFT_CURSOR() (g_bSoftCursor == HI_TRUE)

#ifdef __HuaweiLite__
#define in_atomic() (HI_FALSE)
#define HIGO
#endif

#define HIFB_CHECK_CURSOR_LAYERID(u32LayerId) do\
{\
    if (!IS_CURSOR_LAYER(u32LayerId))\
    {\
        HIFB_ERROR("layer %d is not cursor layer!\n", u32LayerId);\
        return HI_FAILURE;\
    }\
}while(0)

#define HIFB_ALIGNMENT 0xf

#define HIFB_ALIGN     16

/* the interface to operate the chip */
/* Collection of properties and methods,filled in hifb_init*/
static HIFB_DRV_OPS_S s_stDrvOps;

static HI_BOOL g_bSoftCursor = HI_FALSE;
static HI_BOOL g_bDisplayOn = HI_FALSE;
static HI_BOOL hifb_register = HI_TRUE;
static hi_bool g_hifb_irq_register = HI_FALSE;
#define        VO_INVALID_DEV  (-1)
#define        VO_DEV_DHD0      (0)
#define        VO_DEV_DHD1      (1)
#define        VO_DEV_DSD0      (2)

#define        HIFB_INTMSK_HD0_VTTHD1 (0x1)
#define        HIFB_INTMSK_HD0_VTTHD2 (0x2)
#define        HIFB_INTMSK_HD0_VTTHD3 (0x4)
#define        HIFB_INTMSK_HD1_VTTHD2 (0x20)
#define        HIFB_INTMSK_HD1_VTTHD3 (0x40)

#define DEV_NAME "hifb"


extern HI_S32 GRAPHIC_DRV_GetBindDev(HI_S32 s32LayaerId);
extern HI_S32 VOU_GraphicsInit(HI_VOID);
extern HI_S32 VOU_GraphicsDeInit(HI_VOID);

static HI_S32 hifb_parse_cfg(HI_VOID);
static HI_S32 hifb_overlay_probe(HI_U32 u32LayerId);
static HI_VOID hifb_overlay_cleanup(HI_U32 u32LayerId, HI_BOOL bUnregister);
#ifdef __HuaweiLite__
#define fb_info hifb_info
#define fb_cmap fb_cmap_s

static HI_S32 hifb_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo);
#else
static HI_S32 hifb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
#endif
static HIFB_COLOR_FMT_E hifb_getfmtbyargb(struct fb_bitfield *red, struct fb_bitfield *green,
                       struct fb_bitfield *blue, struct fb_bitfield *transp, HI_U32 u32ColorDepth);

HI_VOID hifb_buf_freemem(HI_U64 u64Phyaddr);
HI_U64 hifb_buf_allocmem(const HI_CHAR *pBufName, HI_UL ulLayerSize, const HI_CHAR *pszMmzName);


static HI_S32 hifb_wait_regconfig_work(HI_U32 u32LayerId);

static HI_S32 hifb_freeccanbuf(HIFB_PAR_S *par);
static HI_VOID hifb_set_dispbufinfo(HI_U32 u32LayerId);
static HI_VOID hifb_set_bufmode(HI_U32 u32LayerId, HIFB_LAYER_BUF_E enLayerBufMode);
static inline HI_VOID hifb_get_bufmode(HIFB_PAR_S *pstPar, HIFB_LAYER_BUF_E *penBufMode);
static HI_S32 hifb_onrefresh(HIFB_PAR_S* par, HI_VOID __user *argp);
static HI_S32 hifb_refresh_0buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf);
static HI_S32 hifb_refresh_1buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf);
static HI_S32 hifb_refresh_2buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf);
#ifdef CONFIG_HIFB_FENCE_SUPPORT
static HI_S32 hifb_refresh_2buf_fence(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf);
#endif
static HI_S32 hifb_refresh_2buf_immediate_display(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf);
static HI_S32 hifb_set_mirrormode(HIFB_PAR_S *pstPar, HIFB_MIRROR_MODE_E enMirrorMode);
static HI_S32 hifb_set_rotatemode(struct fb_info *info, HIFB_ROTATE_MODE_E enRotateMode);
#ifdef CURSOR
static inline HI_VOID hifb_set_cursorinfo(HIFB_PAR_S *pstPar, HIFB_CURSOR_INFO_S *pstCursorInfo);
static inline HI_VOID hifb_get_cursorinfo(HIFB_PAR_S *pstPar, HIFB_CURSOR_INFO_S *pstCursorInfo);
static HI_S32 hifb_cursor_attach(HI_U32 u32CursorId, HI_U32 u32LayerId);
static HI_S32 hifb_cursor_detach(HI_U32 u32LayerId);
static HI_S32 hifb_cursor_changepos(HI_U32 u32CursorId, HIFB_POINT_S stPos);
static HI_S32 hifb_cursor_changestate(HIFB_PAR_S *pstCursorPar, HI_BOOL bShow);
static HI_S32 hifb_cursor_putinfo(HIFB_PAR_S *pstCursorPar, HIFB_CURSOR_S* pstCursor);
#endif
static HI_S32 hifb_onputlayerinfo(struct fb_info *info, HIFB_PAR_S* par, const HI_VOID __user *argp);
static HI_VOID hifb_get_layerinfo(HIFB_PAR_S *pstPar, HIFB_LAYER_INFO_S *pstLayerInfo);
static inline HI_VOID hifb_get_antiflickerlevel(HIFB_PAR_S *pstPar,
    HIFB_LAYER_ANTIFLICKER_LEVEL_E *penAntiflickerLevel);
static HI_VOID hifb_set_antiflickerlevel(HI_U32 u32LayerId,
    HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel);
static inline HI_VOID hifb_get_fmt(HIFB_PAR_S *pstPar, HIFB_COLOR_FMT_E *penColorFmt);
static inline HI_VOID hifb_set_fmt(HIFB_PAR_S *pstPar, HIFB_COLOR_FMT_E enColorFmt);
static inline HI_VOID hifb_set_alpha(HIFB_PAR_S *pstPar, HIFB_ALPHA_S *pstAlpha);
static inline HI_VOID hifb_get_alpha(HIFB_PAR_S *pstPar, HIFB_ALPHA_S *pstAlpha);
static inline HI_VOID hifb_set_key(HIFB_PAR_S *pstPar, HIFB_COLORKEYEX_S *pstKey);
static inline HI_VOID hifb_get_key(HIFB_PAR_S *pstPar, HIFB_COLORKEYEX_S *pstKey);
static inline HI_VOID hifb_get_layerpos(HIFB_PAR_S *pstPar, HIFB_POINT_S *pstPos);
static inline HI_VOID hifb_get_screensize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height);
static inline HI_S32 hifb_set_screensize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height);
static inline HI_VOID hifb_get_maxscreensize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height);
static inline HI_VOID hifb_get_dispsize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height);
static inline HI_VOID hifb_get_premul(HIFB_PAR_S *pstPar, HI_BOOL *pbPremul);
static inline HI_BOOL hifb_get_show(HIFB_PAR_S *pstPar);
static inline HI_VOID hifb_set_show(HIFB_PAR_S *pstPar, HI_BOOL bShow);
static inline HI_VOID hifb_set_layerpos(HIFB_PAR_S *pstPar, HIFB_POINT_S *pstPos);
#if(HICHIP == HI3516E_V200)
static HI_S32 hifb_tde_rotate_callback(HI_U32 call_mod_id, HI_U32 call_dev_id, HI_U32 call_chn_id,HI_VOID *pJobData);
static HI_S32 hifb_tde_callback(HI_U32 call_mod_id, HI_U32 call_dev_id, HI_U32 call_chn_id,HI_VOID *pJobData);
#else
static HI_S32 hifb_tde_rotate_callback(const HI_VOID *pParaml, HI_VOID *pParamr);
static HI_S32 hifb_tde_callback(const HI_VOID *pParaml, HI_VOID *pParamr);
#endif
static HI_S32 hifb_vo_callback(const HI_VOID *pParaml, HI_VOID *pParamr);
static HI_S32 hifb_wbc_callback(const HI_VOID *pParaml, HI_VOID *pParamr);
static inline HI_VOID hifb_get_idledispbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr);
static inline HI_VOID hifb_get_workdispbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr);
static inline HI_VOID hifb_get_workcursorbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr);
#ifdef CURSOR
static inline HI_VOID hifb_get_idlecursorbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr);
#endif
static inline HI_BOOL hifb_is_interlace(HIFB_PAR_S *pstPar);
extern HI_VOID *hifb_buf_map(HI_U64 u64PhyAddr, HI_U32 u32Size);
static HI_S32 hifb_setDynamicRange(HIFB_PAR_S *pstPar, HIFB_DYNAMIC_RANGE_E enDynamicRange);


HI_S32 hifb_read_proc(struct osal_proc_dir_entry *entry);
HI_S32 hifb_write_proc(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);

/* default fix information */
/*
* Name : s_stDefFix
* Desc : default fixed info: fix_info
*/





#ifndef __HuaweiLite__

static HI_UL hifb_get_smem_start(struct fb_info* info)
{
     return (HI_UL)info->fix.smem_start;
}

static HI_S8 *hifb_get_screen_base(struct fb_info* info)
{
     return (HI_S8*)info->screen_base;
}

static HI_U32 hifb_get_xres(struct fb_info* info)
{
     return (HI_U32)info->var.xres;
}

static HI_U32 hifb_get_yres(struct fb_info* info)
{
     return (HI_U32)info->var.yres;
}

static HI_U32 hifb_get_xres_virtual(struct fb_info* info)
{
     return (HI_U32)info->var.xres_virtual;
}

static HI_U32 hifb_get_yres_virtual(struct fb_info* info)
{
     return (HI_U32)info->var.yres_virtual;;
}


static HI_U32 hifb_get_smem_len(struct fb_info* info)
{
     return (HI_U32)info->fix.smem_len;
}


static HI_U32 hifb_get_line_length(struct fb_info* info)
{
     return (HI_U32)info->fix.line_length;
}



static HI_U32 hifb_get_bits_per_pixel(struct fb_info* info)
{
     return (HI_U32)info->var.bits_per_pixel;
}



static HI_U32 hifb_get_yoffset(struct fb_info* info)
{
     return (HI_U32)info->var.yoffset;
}

static HI_U32 hifb_get_xoffset(struct fb_info* info)
{
     return (HI_U32)info->var.xoffset;
}



static struct fb_fix_screeninfo s_stDefFix[HIFB_LAYER_TYPE_BUTT] =
{
    {
        .id          = "hifb",                    // String identifier
        .type        = FB_TYPE_PACKED_PIXELS,     // FB type
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = HIFB_HD_DEF_STRIDE,        // The number of bytes in one line(width * 4 (32 bits per pixels / 8 bits per byte))
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "hifb",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = HIFB_SD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "hifb",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = HIFB_SD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "hifb",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = HIFB_HD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    }
};

/* default variable information */
/*
* Name : s_stDefVar
* Desc : default variable info: s_stDefVar
* See  : hifb_open
*/
static struct fb_var_screeninfo s_stDefVar[HIFB_LAYER_TYPE_BUTT] =
{
    /*for HD layer*/
    {
        .xres           = HIFB_HD_DEF_WIDTH,
        .yres           = HIFB_HD_DEF_HEIGHT,
        .xres_virtual   = HIFB_HD_DEF_WIDTH,
        .yres_virtual   = HIFB_HD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /*for SD layer*/
    {
        .xres           = HIFB_SD_DEF_WIDTH,
        .yres           = HIFB_SD_DEF_HEIGHT,
        .xres_virtual   = HIFB_SD_DEF_WIDTH,
        .yres_virtual   = HIFB_SD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /*for AD layer*/
    {
        .xres           = HIFB_AD_DEF_WIDTH,
        .yres           = HIFB_AD_DEF_HEIGHT,
        .xres_virtual   = HIFB_AD_DEF_WIDTH,
        .yres_virtual   = HIFB_AD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    },
    /*for cursor layer*/
    {
        .xres           = HIFB_CURSOR_DEF_WIDTH,
        .yres           = HIFB_CURSOR_DEF_HEIGHT,
        .xres_virtual   = HIFB_CURSOR_DEF_WIDTH,
        .yres_virtual   = HIFB_CURSOR_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = {5, 5, 0},
        .blue           = {0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1, /* pixel clock in ps (pico seconds) */
        .left_margin    = 0, /* time from sync to picture   */
        .right_margin   = 0, /* time from picture to sync   */
        .upper_margin   = 0, /* time from sync to picture   */
        .lower_margin   = 0,
        .hsync_len      = 0, /* length of horizontal sync   */
        .vsync_len      = 0, /* length of vertical sync */
    }
};
#else

static HI_UL hifb_get_smem_start(struct hifb_info* info)
{
     return (HI_UL)(uintptr_t)info->oinfo.fbmem;
}

static HI_S8 *hifb_get_screen_base(struct hifb_info* info)
{
     return (HI_S8 *)info->oinfo.fbmem;
}

static HI_U32 hifb_get_xres(struct hifb_info* info)
{
     return (HI_U32)info->vinfo.xres;
}

static HI_U32 hifb_get_yres(struct hifb_info* info)
{
     return (HI_U32)info->vinfo.yres;
}

static HI_U32 hifb_get_xres_virtual(struct hifb_info* info)
{
     return (HI_U32)info->oinfo.sarea.w;
}

static HI_U32 hifb_get_yres_virtual(struct hifb_info* info)
{
     return (HI_U32)info->oinfo.sarea.h;
}


static HI_U32 hifb_get_smem_len(struct hifb_info* info)
{
     return (HI_U32)info->oinfo.fblen;
}


static HI_U32 hifb_get_line_length(struct hifb_info* info)
{
     return (HI_U32)info->oinfo.stride;
}



static HI_U32 hifb_get_bits_per_pixel(struct hifb_info* info)
{
     return (HI_U32)info->oinfo.bpp;
}

static HI_U32 hifb_get_yoffset(struct hifb_info* info)
{
     return (HI_U32)info->oinfo.sarea.y;

}

static HI_U32 hifb_get_xoffset(struct hifb_info* info)
{
    return (HI_U32)info->oinfo.sarea.x;
}

struct hifb_screeninfo
{
    uint8_t    fmt;         /* see FB_FMT_*  */
    fb_coord_t xres;        /* Horizontal resolution in pixel columns */
    fb_coord_t yres;        /* Vertical resolution in pixel rows */
    struct fb_area_s sarea;     /* Selected area within the overlay */
    fb_coord_t stride;          /* Length of a line in bytes */
    uint8_t    bpp;         /* Bits per pixel */
    uint32_t   accl;            /* Supported hardware acceleration */
    uint8_t    nplanes;     /* Number of color planes supported */
    uint8_t    noverlays;   /* Number of overlays supported */
};

static struct hifb_screeninfo s_stDefInfo[HIFB_LAYER_TYPE_BUTT] =
{
    /*for HD layer*/
    {
        .fmt          = HIFB_FMT_ARGB1555,
        .xres       = HIFB_HD_DEF_WIDTH,
        .yres         = HIFB_HD_DEF_HEIGHT,
        .sarea       = {0, 0, HIFB_HD_DEF_WIDTH, HIFB_HD_DEF_HEIGHT},
        .stride       = HIFB_HD_DEF_STRIDE,
        .bpp          = HIFB_DEF_DEPTH,
        .accl          = 0,
        .nplanes    = 1,
        .noverlays  = 1,
    },
    /*for SD layer*/
    {
        .fmt          = HIFB_FMT_ARGB1555,
        .xres       = HIFB_SD_DEF_WIDTH,
        .yres         = HIFB_SD_DEF_HEIGHT,
        .sarea       = {0, 0, HIFB_SD_DEF_WIDTH, HIFB_SD_DEF_HEIGHT},
        .stride       = HIFB_SD_DEF_STRIDE,
        .bpp          = HIFB_DEF_DEPTH,
        .accl          = 0,
        .nplanes    = 1,
        .noverlays  = 1,
    },
    /*for AD layer*/
    {
        .fmt          = HIFB_FMT_ARGB1555,
        .xres       = HIFB_AD_DEF_WIDTH,
        .yres         = HIFB_AD_DEF_HEIGHT,
        .sarea       = {0, 0, HIFB_AD_DEF_WIDTH, HIFB_AD_DEF_HEIGHT},
        .stride       = HIFB_AD_DEF_STRIDE,
        .bpp          = HIFB_DEF_DEPTH,
        .accl          = 0,
        .nplanes    = 1,
        .noverlays  = 1,
    },
    /*for cursor layer*/
    {
        .fmt          = HIFB_FMT_ARGB1555,
        .xres       = HIFB_CURSOR_DEF_WIDTH,
        .yres         = HIFB_CURSOR_DEF_HEIGHT,
        .sarea       = {0, 0, HIFB_CURSOR_DEF_WIDTH, HIFB_CURSOR_DEF_HEIGHT},
        .stride       = HIFB_CURSOR_DEF_STRIDE,
        .bpp          = HIFB_DEF_DEPTH,
        .accl          = 0,
        .nplanes    = 1,
        .noverlays  = 1,
    },
};

static void hifb_screeninfo_init(struct hifb_info *info, struct hifb_screeninfo *sinfo)
{
    info->vinfo.xres = sinfo->xres;
    info->vinfo.yres = sinfo->yres;
    info->oinfo.sarea = sinfo->sarea;
    info->oinfo.bpp = sinfo->bpp;
    info->oinfo.stride = sinfo->stride;
    info->oinfo.accl = sinfo->accl;
    info->vinfo.nplanes = sinfo->nplanes;
    info->vinfo.noverlays = sinfo->noverlays;
};
#endif


/* bit filed info of color fmt, the order must be the same as HIFB_COLOR_FMT_E */
/*
* Name : s_stArgbBitField
* Desc : bit fields of each color format in HIFB_COLOR_FMT_E,
*        the order must be the same as that of HIFB_COLOR_FMT_E
*/
static HIFB_ARGB_BITINFO_S s_stArgbBitField[] =
{   /*RGB565*/
    {
        .stRed    = {11, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*RGB888*/
    {
        .stRed    = {16, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*KRGB444*/
    {
        .stRed    = {8, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {0, 4, 0},
        .stTransp = {0, 0, 0},
    },
    /*KRGB555*/
    {
        .stRed    = {10, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*KRGB888*/
    {
        .stRed    = {16,8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*ARGB4444*/
    {
        .stRed    = {8, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {0, 4, 0},
        .stTransp = {12, 4, 0},
    },
    /*ARGB1555*/
    {
        .stRed    = {10, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {15, 1, 0},
    },
    /*ARGB8888*/
    {
        .stRed    = {16, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {24, 8, 0},
    },
    /*ARGB8565*/
    {
        .stRed    = {11, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {16, 8, 0},
    },
    /*RGBA4444*/
    {
        .stRed    = {12, 4, 0},
        .stGreen  = {8, 4, 0},
        .stBlue   = {4, 4, 0},
        .stTransp = {0, 4, 0},
    },
    /*RGBA5551*/
    {
        .stRed    = {11, 5, 0},
        .stGreen  = {6, 5, 0},
        .stBlue   = {1, 5, 0},
        .stTransp = {0, 1, 0},
    },
    /*RGBA5658*/
    {
        .stRed    = {19, 5, 0},
        .stGreen  = {13, 6, 0},
        .stBlue   = {8, 5, 0},
        .stTransp = {0, 8, 0},
    },
    /*RGBA8888*/
    {
        .stRed    = {24, 8, 0},
        .stGreen  = {16, 8, 0},
        .stBlue   = {8, 8, 0},
        .stTransp = {0, 8, 0},
    },
    /*BGR565*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {11, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*BGR888*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {16, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*ABGR4444*/
    {
        .stRed    = {0, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {8, 4, 0},
        .stTransp = {12, 4, 0},
    },
    /*ABGR1555*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {10, 5, 0},
        .stTransp = {15, 1, 0},
    },
    /*ABGR8888*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {16, 8, 0},
        .stTransp = {24, 8, 0},
    },
    /*ABGR8565*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {11, 5, 0},
        .stTransp = {16, 8, 0},
    },
    /*KBGR444 16bpp*/
    {
        .stRed    = {0, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {8, 4, 0},
        .stTransp = {0, 0, 0},
    },
    /*KBGR555 16bpp*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {10, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*KBGR888 32bpp*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {16, 8, 0},
        .stTransp = {0, 0, 0},
    },

    /*1bpp*/
    {
        .stRed = {0, 1, 0},
        .stGreen = {0, 1, 0},
        .stBlue = {0, 1, 0},
        .stTransp = {0, 0, 0},
    },
    /*2bpp*/
    {
        .stRed = {0, 2, 0},
        .stGreen = {0, 2, 0},
        .stBlue = {0, 2, 0},
        .stTransp = {0, 0, 0},
    },
    /*4bpp*/
    {
        .stRed = {0, 4, 0},
        .stGreen = {0, 4, 0},
        .stBlue = {0, 4, 0},
        .stTransp = {0, 0, 0},
    },
    /*8bpp*/
    {
        .stRed = {0, 8, 0},
        .stGreen = {0, 8, 0},
        .stBlue = {0, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*ACLUT44*/
    {
        .stRed = {4, 4, 0},
        .stGreen = {4, 4, 0},
        .stBlue = {4, 4, 0},
        .stTransp = {0, 4, 0},
    },
    /*ACLUT88*/
    {
        .stRed = {8, 8, 0},
        .stGreen = {8, 8, 0},
        .stBlue = {8, 8, 0},
        .stTransp = {0, 8, 0},
    }
};

extern HI_S32 HIGO_Log_Init(HI_VOID);
extern HI_S32 HIGO_Log_Deinit(HI_VOID);
extern hi_s32 TdeCalScaleRect_HIFB(const hi_tde_rect* pstSrcRect, const hi_tde_rect* pstDstRect,
                                hi_tde_rect* pstRectInSrc, hi_tde_rect* pstRectInDst);
static HI_VOID hifb_version(HI_VOID);
#ifdef CURSOR
static HI_VOID hifb_cursor_calcdispinfo(HI_U32 u32CursorId, HIFB_PAR_S *pstPar, const HIFB_POINT_S* pstCurNewPos);
#endif
static HI_S32 hifb_refresh(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf, HIFB_LAYER_BUF_E enBufMode);
static HI_VOID hifb_select_antiflicker_mode(HIFB_PAR_S *pstPar);
static HI_S32 hifb_setcolreg(unsigned regno, unsigned red, unsigned green,
                unsigned blue, unsigned transp, struct fb_info *info);


bool g_bUpdateRotateRect = HI_FALSE;
#ifndef __HuaweiLite__
module_param(g_bUpdateRotateRect, bool, S_IRUGO);
module_param_string(video, video, 64, 0);
#endif

static char display[8] = "off";   /*open vo or not*/

static char softcursor[8] = "off";   /*use soft cursor or not*/

/*
* Name : hifb_memset
* Desc : fill buffer, it is faster than memset.
* See  : Called by hifb_overlay_probe
*/
static HI_U32 hifb_memset(struct fb_info* info, HI_U32 u32FillData, size_t n)
{
    HI_S32        s32Ret;
    HIFB_BUFFER_S stDstImg;
    HI_S8 *       pRest = NULL;
    HI_U32        u32CanPitch;

    u32CanPitch = HIFB_TDE_MEMSET_WIDTH * 4;
    if (NULL == info)
    {
         HIFB_ERROR("The pointer is NULL\n");
         return HI_FAILURE;
    }
    /* Assume that the image width is 3840, the format is ARBG8888, and the image is filled by TDE.
    * Since the size of the image that TDE can handle is limited,
    * the width and height are limited to HIFB_TDE_MEMSET_WIDTH
    * and HIFB_TDE_MEMSET_HEIGHT, Exceeded parts are cleared using memset
    */
    // Configure the target image, pitch, format, width and height, physical address (pointing to the memory start address)
    stDstImg.stCanvas.u64PhyAddr = hifb_get_smem_start(info);
    stDstImg.stCanvas.u32Pitch   = u32CanPitch;
    stDstImg.stCanvas.enFmt      = HIFB_FMT_ABGR8888;
    stDstImg.stCanvas.u32Width   = HIFB_TDE_MEMSET_WIDTH;
    stDstImg.stCanvas.u32Height  = n / u32CanPitch;

    if (HIFB_TDE_MEMSET_HEIGHT < stDstImg.stCanvas.u32Height)
    {
        stDstImg.stCanvas.u32Height = HIFB_TDE_MEMSET_HEIGHT;
    }

    if (0 != stDstImg.stCanvas.u32Height)
    {
        s32Ret = HIFB_DRV_Fill(&stDstImg, u32FillData);
        if (s32Ret != HI_SUCCESS)
        {
            HIFB_ERROR("HIFB_DRV_Fill fail!\n");
            return HI_FAILURE;
        }
    }

    /* The memory not in the canvas is set by memset */
    if (0 != (n - stDstImg.stCanvas.u32Height * u32CanPitch))
    {
        pRest = hifb_get_screen_base(info)+  u32CanPitch * (stDstImg.stCanvas.u32Height);
        osal_memset(pRest, u32FillData, n - stDstImg.stCanvas.u32Height * u32CanPitch);
    }
    return HI_SUCCESS;
}


#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hifb_freeze(osal_dev_t *pdev)
{
    return 0;
}

static int hifb_restore(osal_dev_t *pdev)
{
    int i = 0;

    for (i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        struct fb_info *info = g_stLayer[i].pstInfo;

        if (info)
        {
            hifb_memset(info , 0, hifb_get_smem_len(info));
        }
    }

    return 0;
}

static struct osal_fileops stHifbFileOp =
{
    .unlocked_ioctl = NULL,
    .open           = NULL,
    .release        = NULL
};

struct osal_pmops stHifbDrvOps = {
    .pm_freeze  = hifb_freeze,
    .pm_restore = hifb_restore,
};

#endif


#define COMPRESS_HEADER_STRIDE 16
#define COMPRESS_STRIDE_ALIGNMENT 16

/*
* Name : hifb_AllocCompressBuffer
* Desc : the compression is TRUE, the memory is allocated and the address is put in the compressed information of the private data.
*/
HI_S32 hifb_AllocCompressBuffer(HIFB_PAR_S *par)
{
    HI_U32 u32BufHeight;
    HI_U32 u32BufStride;
    HI_U32 u32HeaderSize;
    HI_U32 u32BufSize;
    HI_U64 u64BufPhyAddr;
    HI_U32 u32LayerIndex = 0;

    if (par == NULL) {
        return HI_FAILURE;
    }

    u32LayerIndex = par->u32LayerID;
    if (u32LayerIndex >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    u32BufHeight = par->stDisplayInfo.u32DisplayHeight;
    u32BufStride = (par->stDisplayInfo.u32DisplayWidth * 2 + COMPRESS_STRIDE_ALIGNMENT-1)
        & (~(COMPRESS_STRIDE_ALIGNMENT-1));
    u32HeaderSize = COMPRESS_HEADER_STRIDE * u32BufHeight;
    u32BufSize = u32BufStride * u32BufHeight + u32HeaderSize;
    u64BufPhyAddr = hifb_buf_allocmem("Compress buffer", 2*u32BufSize, apszLayerMmzNames[u32LayerIndex]);
    if (0 == u64BufPhyAddr)
    {
        HIFB_TRACE(HI_DBG_ERR, "Mmz Malloc Compress buffer fail\n");
        return HI_FAILURE;
    }

    par->stCompressInfo.u64AR_BufPhyAddr = u64BufPhyAddr;
    par->stCompressInfo.u64GB_BufPhyAddr = u64BufPhyAddr + u32BufSize;
    par->stCompressInfo.u32HeaderSize = u32HeaderSize;
    par->stCompressInfo.u32Stride = u32BufStride;

    return HI_SUCCESS;
}

HI_VOID hifb_FreeCompressBuffer(HIFB_PAR_S *par)
{
    if (par == NULL) {
        return;
    }

    if (0 != par->stCompressInfo.u64AR_BufPhyAddr)
    {
        hifb_buf_freemem(par->stCompressInfo.u64AR_BufPhyAddr);
        par->stCompressInfo.u64AR_BufPhyAddr = 0;
    }
}



/*
* Name : hifb_SetDcmpInfo
* Desc : set the decompression info.
*/
HI_VOID hifb_SetDcmpInfo(HIFB_PAR_S *par)
{
    volatile HIFB_COMPRESS_INFO_S * pstCompressInfo = NULL;
    HIFB_REFRESH_INFO_S *           pstRefreshInfo  = NULL;
    VO_GRAPHIC_DCMP_INFO_S          stDcmpInfo;
    HIFB_COLOR_FMT_E                enHifbColorFmt  = HIFB_FMT_BUTT;

    if (par == NULL) {
        return;
    }

    pstCompressInfo = &par->stCompressInfo;
    pstRefreshInfo  = &par->stRefreshInfo;

    if (0 == pstRefreshInfo->u64ScreenAddr || 0 == pstRefreshInfo->u64GBScreenAddr)
    {
        /* close the dcmp */
        s_stDrvOps.HIFB_DRV_EnableDcmp(par->u32LayerID,HI_FALSE);
        HIFB_TRACE(HI_DBG_WARN, "Compression is opened, but compressed buffer phyaddr for refreshing is NULL(0)\n");
        return;
    }

    /* fill GRAPHIC_DCMP_INFO_S by HIFB_PAR_S */
    /* Decompressing information from compressed information: compression channels AR, GB address, and its size size0, size1 */
    enHifbColorFmt = pstRefreshInfo->stUserBuffer.stCanvas.enFmt;
    switch (enHifbColorFmt)
    {
        case HIFB_FMT_ARGB4444 :
            stDcmpInfo.enPixelFmt  = VO_INPUTFMT_ARGB_4444;
            break;
        case HIFB_FMT_ARGB1555 :
            stDcmpInfo.enPixelFmt  = VO_INPUTFMT_ARGB_1555;
            break;
        case HIFB_FMT_ARGB8888 :
            stDcmpInfo.enPixelFmt  = VO_INPUTFMT_ARGB_8888;
            break;
        default:
            HIFB_TRACE(HI_DBG_ERR, "pixel format(%d) is invalid!\n", enHifbColorFmt);
            return;

    }
    stDcmpInfo.u64AR_PhyAddr  = HIFB_DRV_GetDcmpOffsetAddr(pstRefreshInfo->u64ScreenAddr);
    stDcmpInfo.u64GB_PhyAddr  = HIFB_DRV_GetDcmpOffsetAddr(pstRefreshInfo->u64GBScreenAddr);
    stDcmpInfo.u32Width       = pstRefreshInfo->stUserBuffer.stCanvas.u32Width;
    stDcmpInfo.u32Height      = pstRefreshInfo->stUserBuffer.stCanvas.u32Height;
    stDcmpInfo.u32Stride      = pstRefreshInfo->stDispBufInfo.u32Stride;
    stDcmpInfo.u32Frame_Size0 = pstCompressInfo->u32Frame_Size0;
    stDcmpInfo.u32Frame_Size1 = pstCompressInfo->u32Frame_Size1;
    stDcmpInfo.IsLosslessA    = HI_FALSE;   // Whether lossless,FALSE:Lossy,TRUE:lossless
    stDcmpInfo.IsLossless     = HI_FALSE;   // Whether lossless,FALSE:Lossy,TRUE:lossless

    /* Decompressing information to drv level */
    if(HI_FALSE == s_stDrvOps.HIFB_DRV_SetDcmpInfo(par->u32LayerID, &stDcmpInfo))
    {
        /* close dcmp */
        s_stDrvOps.HIFB_DRV_EnableDcmp(par->u32LayerID,HI_FALSE);
    }
}

#ifdef CONFIG_HI_VO_FB_SEPARATE
static inline int HifbInterruptRoute(HI_S32 irq, HI_VOID *dev_id);
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
static hi_s32 hifb_soft_interrupt(hi_s32 irq, hi_void *dev_id);
#endif

/******************************************************************************
 Function        : hifb_init
 Description     : initialize framebuffer. the function is called when
                   loading the moudel
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : HI_VOID
 Return          : 0, if success; otherwise, return error code
 Others          : 0
******************************************************************************/
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static osal_dev_t *astHifbDevice;
#endif


#ifdef __HuaweiLite__
HI_S32 __init hifb_init(void* pArgs)
{
    HI_S32 i = 0;
    HI_S32 j = 0;
    HI_CHAR entry_name[16];
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    HI_S32 s32Ret;
#endif

    HIFB_MODULE_PARAMS_S *pstHifbModuleParam = (HIFB_MODULE_PARAMS_S*)pArgs;
    if(NULL != pstHifbModuleParam)
    {
        osal_strncpy(video, pstHifbModuleParam->video, 63);
        video[63] = '\0';
        g_bUpdateRotateRect = pstHifbModuleParam -> bUpdateRotateRect;
    }
#ifndef DISABLE_DEBUG_INFO
    HIFB_PROC_Init();
#endif
    /* get the chip operation method */
    HIFB_DRV_GetOps(&s_stDrvOps);

    /* parse the \arg video && softcursor && display string */
    if (hifb_parse_cfg() < 0)
    {
        HIFB_ERROR("Usage:insmod hifb.ko video=\"hifb:vrami_size:xxx,vramj_size:xxx,...\"\n");
        HIFB_ERROR("i,j means layer id, xxx means layer size in kbytes!\n");
        HIFB_ERROR("example:insmod hifb.ko video=\"hifb:vram0_size:810,vram1_size:810\"\n\n");
#ifndef DISABLE_DEBUG_INFO
        HIFB_PROC_Cleanup();
#endif
        return HI_FAILURE;
    }

#ifndef DISABLE_DEBUG_INFO
    /*initialize fb file according the config*/
    for(i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        /* register the layer */
        if (hifb_overlay_probe(i) == HI_SUCCESS)
        {
            /* create a proc entry in 'hifb' for the layer */
            osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", i);
            HIFB_PROC_AddModule(entry_name, hifb_read_proc, hifb_write_proc, NULL,
                g_stLayer[i].pstInfo);
        }
        else
        {
            /* roll back hifb0~hifbi-1's proc entry, remove them.  */
            j = i;
            for(j = i-1; j >= 0; j--)
            {
                /* destroy a proc entry in 'hifb' for the layer */
                osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", j);
                HIFB_PROC_RemoveModule(entry_name);
                /* unregister the layer */
                hifb_overlay_cleanup(j, HI_TRUE);
            }
            return HI_FAILURE;
        }
    }
#endif

    /* open vo */
    if (g_bDisplayOn)
    {
        s_stDrvOps.HIFB_DRV_OpenDisplay();
    }

    /* inital adoption layer */
    if (HI_SUCCESS != s_stDrvOps.HIFB_DRV_Init())
    {
        HIFB_ERROR("drv init failed\n");
        goto ERR1;
    }

    HIFB_DRV_SetTdeCallBack(hifb_tde_callback);
    HIFB_DRV_SetTdeRotateCallBack(hifb_tde_rotate_callback);

#if defined(HIGO)
#ifndef DISABLE_DEBUG_INFO
    HIGO_Log_Init();
#endif
#endif

#ifdef CONFIG_HI_VO_FB_SEPARATE
    if (osal_request_irq(g_fifb_irq, HifbInterruptRoute, NULL,"HIFB Int", HifbInterruptRoute)) {
        hifb_register = HI_FALSE;
        HIFB_ERROR("request_irq for Hifb failure!\n");
        goto ERR2;
    }
    g_hifb_irq_register = HI_TRUE;
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    if (osal_request_irq(g_fifb_soft_irq, hifb_soft_interrupt, NULL, "HIFB_SOFT_INT", hifb_soft_interrupt)) {
        hifb_register = HI_FALSE;
        HIFB_ERROR("request_irq for hifb soft irq failure!\n");
        goto ERR2;
    }
    g_hifb_soft_irq_register = HI_TRUE;
#endif

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    char buf[20];
    osal_snprintf(buf, 20, "%s", DEV_NAME);
    astHifbDevice = osal_createdev(buf);
    astHifbDevice->fops = &stHifbFileOp;
    astHifbDevice->osal_pmops = &stHifbDrvOps;
    astHifbDevice->minor = HIMEDIA_DYNAMIC_MINOR;
    s32Ret = osal_registerdevice(astHifbDevice);
    if(s32Ret)
    {
        osal_destroydev(astHifbDevice);
        HIFB_ERROR("osal_registerdevice failed\n");
        goto ERR2;
    }
#endif
    /* show version */
    hifb_version();

    printk("Load hifb.ko OK!\n");

    return 0;

ERR2:
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    if (g_hifb_soft_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_soft_irq, hifb_soft_interrupt);
        g_hifb_soft_irq_register = HI_FALSE;
    }
#endif

#ifdef CONFIG_HI_VO_FB_SEPARATE
    if (g_hifb_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_irq, HifbInterruptRoute);
        g_hifb_irq_register = HI_FALSE;
    }
#endif

    s_stDrvOps.HIFB_DRV_DeInit();

#if defined(HIGO)
#ifndef DISABLE_DEBUG_INFO
    HIGO_Log_Deinit();
#endif
#endif

    HIFB_DRV_SetTdeCallBack(NULL);
    HIFB_DRV_SetTdeRotateCallBack(NULL);

ERR1:

    /*close vo*/
    if (g_bDisplayOn)
    {
        s_stDrvOps.HIFB_DRV_CloseDisplay();
    }

    for (i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        hifb_overlay_cleanup(i, HI_TRUE);
    }

#ifndef DISABLE_DEBUG_INFO
    HIFB_PROC_RemoveAllModule();

    HIFB_PROC_Cleanup();
#endif
    printk("ERROR: Load hifb.ko FAILED!\n");
    return HI_FAILURE;
}
#else
static HI_S32 __init hifb_init(HI_VOID)
{
    HI_S32 i = 0;
    HI_S32 j = 0;
    HI_CHAR entry_name[16] = {'\0'};
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    HI_S32 s32Ret;
    char buf[20]= {'\0'};
#endif

    /* make dir 'hifb' under proc/graphics */
#ifndef DISABLE_DEBUG_INFO
    HIFB_PROC_Init();
#endif

    /* get the chip operation method */
    HIFB_DRV_GetOps(&s_stDrvOps);
    /* parse the \arg video && softcursor && display string */
    if (hifb_parse_cfg() < 0)
    {
        HIFB_ERROR("Usage:insmod hifb.ko video=\"hifb:vrami_size:xxx,vramj_size:xxx,...\"\n");
        HIFB_ERROR("i,j means layer id, xxx means layer size in kbytes!\n");
        HIFB_ERROR("example:insmod hifb.ko video=\"hifb:vram0_size:1620,vram1_size:1620\"\n\n");
#ifndef DISABLE_DEBUG_INFO
        HIFB_PROC_Cleanup();
#endif
        hifb_register = HI_FALSE;
        printk("ERROR: Load hifb.ko FAILED!\n");
        return HI_FAILURE;
    }
    /*initialize fb file according the config*/
    for(i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        /* register the layer */
        if (hifb_overlay_probe(i) == HI_SUCCESS)
        {
            /* create a proc entry in 'hifb' for the layer */
            osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", i);

#ifndef DISABLE_DEBUG_INFO
            HIFB_PROC_AddModule(entry_name, hifb_read_proc, hifb_write_proc, NULL,
                g_stLayer[i].pstInfo);
#endif
        }
        else
        {
            /* roll back hifb0~hifbi-1's proc entry, remove them.  */
            j = i;
            for(j = i-1; j >= 0; j--)
            {
                /* destroy a proc entry in 'hifb' for the layer */
                osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", j);
                #ifndef DISABLE_DEBUG_INFO
                HIFB_PROC_RemoveModule(entry_name);
                #endif
                /* unregister the layer */
                hifb_overlay_cleanup(j, HI_TRUE);
            }
            hifb_register = HI_FALSE;
            printk("ERROR: Load hifb.ko FAILED!\n");
            return HI_FAILURE;
        }
    }

    /* open vo */
    if (g_bDisplayOn)
    {
        s_stDrvOps.HIFB_DRV_OpenDisplay();
    }

    /* inital adoption layer */
    if (HI_SUCCESS != s_stDrvOps.HIFB_DRV_Init())
    {
        HIFB_ERROR("Drv init failed\n");
        goto ERR1;
    }


    HIFB_DRV_SetTdeCallBack(hifb_tde_callback);
    HIFB_DRV_SetTdeRotateCallBack(hifb_tde_rotate_callback);


#if defined(HIGO)
#ifndef DISABLE_DEBUG_INFO
    HIGO_Log_Init();
#endif
#endif
    /*
    * Interrupt registration interrupt is placed at the end.
    * Since the interrupt will be opened in the boot screen,
    * it will respond when loading ko, and the context is used in the interrupt.
    */
#ifdef CONFIG_HI_VO_FB_SEPARATE
    if (osal_request_irq(g_fifb_irq, HifbInterruptRoute, NULL,"HIFB Int", HifbInterruptRoute)) {
        hifb_register = HI_FALSE;
        HIFB_ERROR("request_irq for Hifb failure!\n");
        goto ERR2;
    }
        g_hifb_irq_register = HI_TRUE;
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
        if (osal_request_irq(g_fifb_soft_irq, hifb_soft_interrupt, NULL, "HIFB_SOFT_INT", hifb_soft_interrupt)) {
            hifb_register = HI_FALSE;
            HIFB_ERROR("request_irq for hifb soft irq failure!\n");
            goto ERR2;
        }
        g_hifb_soft_irq_register = HI_TRUE;
#endif

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    osal_snprintf(buf, 20, "%s", DEV_NAME);
    astHifbDevice = osal_createdev(buf);
    astHifbDevice->fops = &stHifbFileOp;
    astHifbDevice->osal_pmops = &stHifbDrvOps;
    astHifbDevice->minor = HIMEDIA_DYNAMIC_MINOR;
    s32Ret = osal_registerdevice(astHifbDevice);
    if(s32Ret)
    {
        osal_destroydev(astHifbDevice);
        HIFB_ERROR("osal_registerdevice failed\n");
        goto ERR2;
    }
#endif
    /* show version */
    hifb_version();

    (void)hifb_vsync_init();
    (void)hifb_pts_init();

    printk("load hifb.ko OK!\n");

    return 0;

ERR2:
#ifndef CONFIG_HI_HIFB_VGS
    s_stDrvOps.HIFB_DRV_DeInit();

    #if defined(HIGO)
    #ifndef DISABLE_DEBUG_INFO
    HIGO_Log_Deinit();
    #endif
    #endif
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    if (g_hifb_soft_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_soft_irq, hifb_soft_interrupt);
        g_hifb_soft_irq_register = HI_FALSE;
    }
#endif
#ifdef CONFIG_HI_VO_FB_SEPARATE
    if (g_hifb_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_irq, HifbInterruptRoute);
        g_hifb_irq_register = HI_FALSE;
    }
#endif
    HIFB_DRV_SetTdeCallBack(NULL);
    HIFB_DRV_SetTdeRotateCallBack(NULL);

ERR1:

    /*close vo*/
    if (g_bDisplayOn)
    {
        s_stDrvOps.HIFB_DRV_CloseDisplay();
    }

    for (i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        hifb_overlay_cleanup(i, HI_TRUE);
    }

#ifndef DISABLE_DEBUG_INFO
    HIFB_PROC_RemoveAllModule();

    HIFB_PROC_Cleanup();
#endif
    hifb_register = HI_FALSE;
    printk("ERROR: Load hifb.ko FAILED!\n");
    return HI_FAILURE;
}
#endif

/******************************************************************************
 Function        : hifb_cleanup
 Description     : cleanup the resource when exiting the framebuffer
                   module
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : HI_VOID
 Return          : static
 Others          : 0
******************************************************************************/
HI_VOID __exit hifb_cleanup(HI_VOID)
{
    HI_S32 i;

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    if (g_hifb_soft_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_soft_irq, hifb_soft_interrupt);
        g_hifb_soft_irq_register = HI_FALSE;
    }
#endif

#ifdef CONFIG_HI_VO_FB_SEPARATE
    if (g_hifb_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_irq, HifbInterruptRoute);
        g_hifb_irq_register = HI_FALSE;
    }
#endif

    if (hifb_register) {
        hifb_pts_exit();
        hifb_vsync_exit();

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
        osal_deregisterdevice(astHifbDevice);
        osal_destroydev(astHifbDevice);
#endif

#if defined(HIGO)
#ifndef DISABLE_DEBUG_INFO
        HIGO_Log_Deinit();
#endif
#endif
        HIFB_DRV_SetTdeCallBack(NULL);
        HIFB_DRV_SetTdeRotateCallBack(NULL);

        s_stDrvOps.HIFB_DRV_DeInit();

        if (g_bDisplayOn)
        {
            s_stDrvOps.HIFB_DRV_CloseDisplay();
        }

        /* remove all entry under dir 'hifb'*/
#ifndef DISABLE_DEBUG_INFO
        HIFB_PROC_RemoveAllModule();

        /* remove dir 'hifb' under proc */
        HIFB_PROC_Cleanup();
#endif
        for (i = 0; i < s_stDrvOps.u32LayerCount; i++)
        {
            hifb_overlay_cleanup(i, HI_TRUE);
        }
    }
    osal_printk("unload hifb.ko ... ok!\n");
    return;
}

/*
* Name : HifbInterruptRoute
* Desc : hifb interrupt handler
* See  : hifb_vo_callback
*/
#ifdef CONFIG_HI_VO_FB_SEPARATE
static inline int HifbInterruptRoute(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 u32IntStatus = 0;
    VO_DEV VoDev        = VO_INVALID_DEV;
    HI_S32 i            = 0;
    HI_S32 s32Ret;
    struct fb_info *  info   = NULL;
    HIFB_PAR_S *      pstPar = NULL;
    HI_U32     u32VtthdFlag     = 0x0;
    HI_U32     u32FrmStartFlag  = 0x0;

    s_stDrvOps.HIFB_DRV_GraphicsGetINT(&u32IntStatus);
    /*
    * HIFB_DRV_GraphicsClearINTStatus will clear all reported interrupts.
    * If devices' interrupt are reported at the same time,
    * Will cause only one device to be processed, the interruption of other devices is ignored,
    * so it cannot be cleared here.
    * All reported interrupts can only be cleared one by one in HIFB_DRV_GraphicsGetINTDev
    */
    HIFB_TRACE(HI_DBG_DEBUG, "intstatus : %d\n",u32IntStatus);

    s32Ret = s_stDrvOps.HIFB_DRV_GraphicsGetINTDev(u32IntStatus,&VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        return OSAL_IRQ_HANDLED;
    }

    if(VoDev != VO_INVALID_DEV)
    {
        // Graphic layer interrupt display processing
    }
    else
    {
        HIFB_ERROR("unknow dev:%d \n", VoDev);
        return OSAL_IRQ_HANDLED;
    }

    /* Handling all graphics layers on this device */
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i)
    {
        info  = g_stLayer[i].pstInfo;
        pstPar= (HIFB_PAR_S *)(info->par);

        /* If the layer is not open, do not act */
        if (HI_FALSE == pstPar->bLayerOpen)
        {
            continue;
        }

        /* if the layer is bound to the device */
        if (VoDev != GRAPHIC_DRV_GetBindDev(i))
        {
            continue;
        }

        if(VO_DEV_DHD0 == VoDev)
        {
            u32VtthdFlag    = HIFB_INTMSK_HD0_VTTHD2;
            u32FrmStartFlag = HIFB_INTMSK_HD0_VTTHD3;
        }
        else if(VO_DEV_DHD1 == VoDev)
        {
            u32VtthdFlag    = HIFB_INTMSK_HD1_VTTHD2;
            u32FrmStartFlag = HIFB_INTMSK_HD1_VTTHD3;
        }
        /*
        * 1. The frame start interrupt and the vertical timing interrupt appear
        * in the interrupt status register at the same time.
        * Frame start interrupts and vertical timing interrupts
        * are processed one by one and cannot be processed at the same time because
        * interrupts are cleared one by one, and the interrupt processing order
        * and the clearing order should be also consistent.
        * The first clear must be processed first, then the post-cleared
        * 2, as long as there is a vertical timing interrupt, it is necessary to drive the display
        */
        if(u32IntStatus & u32VtthdFlag)  //VO vertical timing interrupt
        {
#ifndef CONFIG_HIFB_SOFT_IRQ_SUPPORT
            pstPar->stRefreshInfo.bDoRefreshJob = HI_FALSE;
            hifb_vo_callback(&i,NULL);
#endif
        }
        else if (u32IntStatus & u32FrmStartFlag)    //VO frame start interrupt
        {
            pstPar->stRefreshInfo.bDoRefreshJob = HI_TRUE;
            wake_up_interruptible(&(pstPar->doRefreshJob));
        }
    }
    return OSAL_IRQ_HANDLED;
}
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
static hi_s32 hifb_soft_interrupt(hi_s32 irq, hi_void *dev_id)
{
    hi_s32 i = 0;
    struct fb_info *info = NULL;
    HIFB_PAR_S *par = NULL;

    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        hifb_hal_soft_int_en(HI_FALSE);
        info  = g_stLayer[i].pstInfo;
        par = (HIFB_PAR_S *)(info->par);
        par->stRefreshInfo.bDoRefreshJob = HI_FALSE;
        hifb_vo_callback(&i, NULL);
    }
    return OSAL_IRQ_HANDLED;
}
#endif

/******************************************************************************
 Function        : hifb_open
 Description     : open the framebuffer and using the default parameter to set the layer
 Data Accessed   :
 Data Updated    :
 Output          : None
                   struct fb_info *info
 Return          : return 0
 Others          : 0
******************************************************************************/

#ifdef __HuaweiLite__
void *g_hifb_info_ptr = NULL;
static HI_S32 hifb_open (struct fb_vtable_s *vtable)
#else
static HI_S32 hifb_open (struct fb_info *info, HI_S32 user)
#endif
{
#ifdef __HuaweiLite__
    struct hifb_info *info = NULL;
#else
    struct fb_var_screeninfo * pvar           = NULL;
    struct fb_fix_screeninfo * pfix           = NULL;
#endif
    HIFB_PAR_S *               par            = NULL;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = NULL;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = NULL;
    volatile
        HIFB_COMPRESS_INFO_S * pstCompressInfo= NULL;
    HIFB_OSD_DATA_S            stOsdData      = {0};
    HIFB_COLOR_FMT_E           enColorFmt;
    HI_U32                     u32LayerId     = 0;
    HI_S32                     s32Ret;
    HIFB_SPIN_LOCK_FLAG        lockflag;

    HIFB_ALPHA_S               stAlpha        = {HI_TRUE, HI_FALSE, HIFB_ALPHA_TRANSPARENT,
                HIFB_ALPHA_OPAQUE, HIFB_ALPHA_OPAQUE, 0};

#ifdef __HuaweiLite__
    if (vtable == NULL) {
        return HI_FAILURE;
    }
    info = (struct hifb_info *)vtable;
#else
    if (info == NULL) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }
    pvar = &info->var;
    pfix = &info->fix;
#endif
    par = (HIFB_PAR_S *)info->par;
    pstDisplayInfo = &par->stDisplayInfo;
    pstRefreshInfo = &par->stRefreshInfo;
    pstCompressInfo = &par->stCompressInfo;
    u32LayerId = par->u32LayerID;
    if (par->u32LayerID >= HIFB_MAX_LAYER_NUM) {
        HIFB_ERROR("layer %d is not supported!\n", par->u32LayerID);
        return HI_FAILURE;
    }

    if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
    {
        if (!atomic_read(&par->ref_count))
        {
            osal_memset(par, 0, sizeof(HIFB_PAR_S));

            par->u32LayerID = u32LayerId;
            atomic_set(&par->ref_count, 0);

            /*Configuring alpha properties*/
            hifb_set_alpha(par, &stAlpha);
        }
        atomic_inc(&par->ref_count);

        return HI_SUCCESS;
    }

    /*assure layer is legal*/
    if (!s_stDrvOps.pstCapability[par->u32LayerID].bLayerSupported)
    {
        HIFB_ERROR("layer %d is not supported!\n", par->u32LayerID);
        return HI_FAILURE;
    }

    /* open the layer first time */
    if (!atomic_read(&par->ref_count))
    {
        int ret;
        HIFB_CURSOR_INFO_S stCursorInfo;
        HIFB_RECT stRect;
#if(HICHIP == HI3516E_V200)
        HIFB_GraphicsInit();
#else
        VOU_GraphicsInit();
#endif

        osal_memset(&stCursorInfo, 0, sizeof(HIFB_CURSOR_INFO_S));
        s32Ret = s_stDrvOps.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);
        if (HI_SUCCESS != s32Ret)
        {
            HIFB_ERROR("Failed to get osd data!\n");
            return HI_FAILURE;
        }

        // Configure the layer's default variable parameter var_info according to the type of layer
        ret = s_stDrvOps.HIFB_OpenLayer(u32LayerId);
        if (ret != HI_SUCCESS)
        {
            HIFB_ERROR("Open graphic layer %d# failed!\n", u32LayerId);
            return ret;
        }
#ifdef __HuaweiLite__
        if (IS_HD_LAYER(u32LayerId))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_HD]);
        }
        else if (IS_SD_LAYER(u32LayerId))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_SD]);
        }
        else if (IS_AD_LAYER(u32LayerId))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_AD]);
        }
        else if (IS_CURSOR_LAYER(u32LayerId))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_CURSOR]);
        }
        else
        {
            HIFB_ERROR("error layer id:%d\n", par->u32LayerID);
        }
        enColorFmt = info->vinfo.fmt;
        if (enColorFmt == HIFB_FMT_BUTT)
        {
            HIFB_ERROR("Invalid default color format!\n");
            return HI_FAILURE;
        }

        info->oinfo.stride = (info->oinfo.sarea.w * (info->oinfo.bpp >> 3) + HIFB_ALIGNMENT)& (~HIFB_ALIGNMENT);
#else
        if (IS_HD_LAYER(u32LayerId))
        {
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_HD];
        }
        else if (IS_SD_LAYER(u32LayerId))
        {
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_SD];
        }
        else if (IS_AD_LAYER(u32LayerId))
        {
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_AD];
        }
        else if (IS_CURSOR_LAYER(u32LayerId))
        {
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_CURSOR];
        }
        else
        {
            HIFB_ERROR("error layer id:%d\n", par->u32LayerID);
        }
        /*transform colorfmt form bitfiled to hifb fmt, and record it*/
        enColorFmt = hifb_getfmtbyargb(&pvar->red, &pvar->green, &pvar->blue,
            &pvar->transp, pvar->bits_per_pixel);
        if (enColorFmt == HIFB_FMT_BUTT)
        {
            HIFB_ERROR("Invalid default color format!\n");
            return HI_FAILURE;
        }
        // By default it is width*4 (32 bits per pixel / 8)
        pfix->line_length = (pvar->xres_virtual*(pvar->bits_per_pixel >> 3) + HIFB_ALIGNMENT)
            & (~HIFB_ALIGNMENT);
#endif


        osal_memset(par, 0, sizeof(HIFB_PAR_S));
        par->u32LayerID = u32LayerId;
        atomic_set(&par->ref_count, 0);

        // Initialize the display information in private data
        pstDisplayInfo = &par->stDisplayInfo;
        pstDisplayInfo->u32DisplayWidth    = hifb_get_xres(info);
        pstDisplayInfo->u32DisplayHeight   = hifb_get_yres(info);
        pstDisplayInfo->u32ScreenWidth     = hifb_get_xres(info);
        pstDisplayInfo->u32ScreenHeight    = hifb_get_yres(info);
        pstDisplayInfo->u32VirXRes         = hifb_get_xres_virtual(info);
        pstDisplayInfo->u32VirYRes         = hifb_get_yres_virtual(info);
        pstDisplayInfo->u32XRes            = hifb_get_xres(info);
        pstDisplayInfo->u32YRes            = hifb_get_yres(info);
        pstDisplayInfo->u32MaxScreenWidth  = stOsdData.u32ScreenWidth;
        pstDisplayInfo->u32MaxScreenHeight = stOsdData.u32ScreenHeight;
        pstDisplayInfo->enMirrorMode       = HIFB_MIRROR_NONE;
        pstDisplayInfo->enRotateMode       = HIFB_ROTATE_NONE;
        par->u64RotateVb = 0;

        init_waitqueue_head(&(par->vblEvent));
        init_waitqueue_head(&(par->doRefreshJob));

        /*Initialization lock*/
        HIFB_SPIN_LOCK_INIT(&par->lock);

        HIFB_SPIN_LOCK_IRQSAVE(&par->lock,lockflag);

        hifb_set_bufmode(par->u32LayerID, HIFB_LAYER_BUF_BUTT);

        hifb_set_alpha(par, &stAlpha);

        hifb_set_dispbufinfo(u32LayerId);

        hifb_set_fmt(par, enColorFmt);

        // Anti-flicker when interlaced
        pstDisplayInfo->bNeedAntiflicker = (stOsdData.eScanMode == HIFB_SCANMODE_I)?(HI_TRUE):(HI_FALSE);

        hifb_set_antiflickerlevel(par->u32LayerID, HIFB_LAYER_ANTIFLICKER_AUTO);
#ifdef CURSOR
        if (IS_SOFT_CURSOR() && !IS_CURSOR_LAYER(u32LayerId))
        {
            stCursorInfo.stCursor.stCursor.u64PhyAddr = hifb_get_smem_start(info)+ hifb_get_smem_len(info);
            stCursorInfo.stCursor.stCursor.u32Pitch = HIFB_SOFTCURSOR_STRIDE;
            stCursorInfo.stCursor.stCursor.enFmt = enColorFmt;
            hifb_set_cursorinfo(par, &stCursorInfo);
        }
#endif
        HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock,lockflag);
        s_stDrvOps.HIFB_DRV_LayerDefaultSetting(u32LayerId);

        s_stDrvOps.HIFB_DRV_SetLayerAlpha(u32LayerId, par->stAlpha);

        s_stDrvOps.HIFB_DRV_SetLayerDataFmt(u32LayerId, par->enColFmt);
#ifdef MDDRDETECT

        /*Memory detection is open by default. This function is not supported at this time. */
        s_stDrvOps.HIFB_DRV_GetDDR_ZoneCfg(u32LayerId, (HI_U32*)&pstCompressInfo->u32StartSection,
            (HI_U32*)&pstCompressInfo->u32ZoneNums);
#endif
        // Decompression information settings
        pstCompressInfo->u32NewStartSection = pstCompressInfo->u32StartSection;
        pstCompressInfo->u32NewZoneNums = pstCompressInfo->u32ZoneNums;
        pstCompressInfo->bClearZone = HI_FALSE;

        s_stDrvOps.HIFB_DRV_EnableDcmp(u32LayerId, HI_FALSE);//Clear decompression status

        stRect.x = 0;
        stRect.y = 0;
        stRect.w = hifb_get_xres(info);
        stRect.h = hifb_get_yres(info);

        s_stDrvOps.HIFB_DRV_SetLayerRect(par->u32LayerID, &stRect, &stRect);
        s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso(par->u32LayerID, &stRect);

        s_stDrvOps.HIFB_DRV_SetLayerStride(par->u32LayerID, hifb_get_line_length(info));

        s_stDrvOps.HIFB_DRV_SetPreMul(par->u32LayerID, pstDisplayInfo->bPreMul);
        if (s_stDrvOps.pstCapability[u32LayerId].bKeyRgb
            || s_stDrvOps.pstCapability[u32LayerId].bKeyAlpha)
        {
            s_stDrvOps.HIFB_DRV_SetLayerKeyMask(par->u32LayerID, &par->stCkey);
        }

        //Set to memory address
        s_stDrvOps.HIFB_DRV_SetLayerAddr(par->u32LayerID, hifb_get_smem_start(info));
        pstRefreshInfo->u64ScreenAddr = hifb_get_smem_start(info);
        pstRefreshInfo->u64GBScreenAddr = 0x0;// Clear the decompression address
        pstCompressInfo->bLayerAddrUpdate = HI_TRUE;
#ifndef CONFIG_HI_VO_FB_SEPARATE
        /*
        * The soft interrupt handlers are registered in the context of the graphics layer.
        */
        s_stDrvOps.HIFB_DRV_SetIntCallback(HIFB_INTTYPE_VO, hifb_vo_callback,
            par->u32LayerID, &par->u32LayerID);
#endif

        s_stDrvOps.HIFB_DRV_SetIntCallback(HIFB_INTTYPE_WBC, hifb_wbc_callback,
            par->u32LayerID, &par->u32LayerID);

        HIFB_SPIN_LOCK_IRQSAVE(&par->lock,lockflag);

        pstCompressInfo->bCompressOpen = HI_FALSE;

        par->bModifying = HI_TRUE;
        hifb_set_show(par, HI_TRUE);
        par->u32ParamModifyMask = HIFB_LAYER_PARAMODIFY_SHOW;
        par->bModifying = HI_FALSE;
        par->bLayerOpen = HI_TRUE;

        HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock,lockflag);

        /* ��������ʹ��layer��vo���fb�ӿ���Ҫ�øüĴ����ж�ͼ�β��Ƿ�� */
        s_stDrvOps.HIFB_DRV_EnableLayer(par->u32LayerID, HI_TRUE);
#ifdef CONFIG_HIFB_FENCE_SUPPORT

            ret = DRV_HIFB_FENCE_Init(par);
            if (HI_SUCCESS != ret)
            {
                HIFB_ERROR("DRV_HIFB_FENCE_Init %s:%d# failed!\n", __FUNCTION__, __LINE__);
                s_stDrvOps.HIFB_CloseLayer(u32LayerId);
                return HI_FAILURE;
            }

#endif

    }

    /* increase reference count */
    atomic_inc(&par->ref_count);
#ifdef __HuaweiLite__
    g_hifb_info_ptr = (void *)info;
#endif
    return 0;
}

/******************************************************************************
 Function        : hifb_release
 Description     : open the framebuffer and disable the layer
 Data Accessed   :
 Data Updated    :
 Output          : None
                   struct fb_info *info
 Return          : return 0 if succeed, otherwise return -EINVAL
 Others          : 0
******************************************************************************/
HI_VOID hifb_release_info(HIFB_PAR_S *par)
{
    HIFB_SPIN_LOCK_FLAG u32LockFlag;
    if (par == NULL) {
        return;
    }

    if (s_stDrvOps.HIFB_DRV_EnableWbcInt == NULL ||
        s_stDrvOps.HIFB_DRV_EnableLayer == NULL ||
        s_stDrvOps.HIFB_DRV_UpdataLayerReg == NULL ||
        s_stDrvOps.HIFB_DRV_SetIntCallback == NULL ||
        s_stDrvOps.HIFB_CloseLayer == NULL) {
        HIFB_ERROR("hifb_release_info failed!\n");
        return;
    }
#ifdef MDDRDETECT
    if (s_stDrvOps.HIFB_DRV_ClearDDRDectectZone == NULL) {
        HIFB_ERROR("hifb_release_info failed!\n");
        return;
    }
#endif
    HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
    hifb_set_show(par, HI_FALSE);

    if ((!IS_CURSOR_LAYER(par->u32LayerID)) || (!IS_SOFT_CURSOR()))
    {
        /* disable the compress */
        if (par->stCompressInfo.bCompressOpen)
        {
            /* No memory alloc, no need to release */
            par->stCompressInfo.bCompressOpen = HI_FALSE;
        }

        /* disable the layer */
        /* clear wbc interrupt when close fb */
        s_stDrvOps.HIFB_DRV_EnableWbcInt(par->u32LayerID, HI_FALSE);
        s_stDrvOps.HIFB_DRV_EnableLayer(par->u32LayerID, HI_FALSE);
        s_stDrvOps.HIFB_DRV_UpdataLayerReg(par->u32LayerID);

        s_stDrvOps.HIFB_DRV_SetIntCallback(HIFB_INTTYPE_VO, HI_NULL, par->u32LayerID, HI_NULL);
        s_stDrvOps.HIFB_DRV_SetIntCallback(HIFB_INTTYPE_WBC, HI_NULL, par->u32LayerID, HI_NULL);

        s_stDrvOps.HIFB_CloseLayer(par->u32LayerID);
        /* Clear memory detection area when closing graphics layer */
#ifdef MDDRDETECT
        s_stDrvOps.HIFB_DRV_ClearDDRDectectZone(par->stCompressInfo.u32StartSection, par->stCompressInfo.u32ZoneNums);
#endif
        par->stCompressInfo.u32StartSection = 0;
        par->stCompressInfo.u32ZoneNums = 0;
        par->stCompressInfo.u32NewStartSection = 0;
        par->stCompressInfo.u32NewZoneNums = 0;
        par->stCompressInfo.bClearZone = HI_TRUE;

        /* MMzFree in hifb_freeccanbuf has a sleep function call, you need to release the spin lock first. */
        HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
        hifb_freeccanbuf(par);
        HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);

    }
    HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
}

static HI_S32 hifb_releaseLayer(HIFB_PAR_S *par)
{
    if (par == NULL) {
        return HI_FAILURE;
    }
    /* Direct release of resources is not allowed if the graphics layer is opened more than once*/
    if(atomic_read(&par->ref_count) > 1)
    {
        HIFB_ERROR("the layer is open, please close the layer first!\n");
        return -1;
    }

    hifb_release_info(par);

    return 0;
}

static HI_S32 hifb_release(struct fb_info *info, HI_S32 user)
{
    HIFB_PAR_S *par = NULL;
    HI_U32 mem_len = 0;
    HI_S8 *screen_base = NULL;

    if (info == NULL) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;

    if (atomic_dec_and_test(&par->ref_count))
    {
        hifb_release_info(par);

        screen_base = hifb_get_screen_base(info);
        mem_len = hifb_get_smem_len(info);
        if ((screen_base != NULL) && (mem_len != 0)) {
            osal_memset(screen_base, 0, mem_len);
        }
#ifdef CONFIG_HI_VO_FB_SEPARATE
        VOU_GraphicsDeInit();
        par->bLayerOpen = HI_FALSE;
#endif
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        DRV_HIFB_FENCE_DeInit(par);
#endif
    }

    return 0;
}

/* Soft mouse needs to filter out the hardware mouse interface */
HI_BOOL hifb_cmdfilter(HI_U32 cmd)
{
    HI_BOOL bFilter = HI_FALSE;
    switch (cmd)
    {
        case FBIO_REFRESH:
        case FBIOGET_CANVAS_BUFFER:
        case FBIOPUT_LAYER_INFO:
        case FBIOGET_LAYER_INFO:
        case FBIOGET_ALPHA_HIFB:
        case FBIOPUT_ALPHA_HIFB:
        case FBIOGET_DEFLICKER_HIFB:
        case FBIOPUT_DEFLICKER_HIFB:
        case FBIOGET_COLORKEY_HIFB:
        case FBIOPUT_COLORKEY_HIFB:
        case FBIOPUT_SCREENSIZE:
        case FBIOGET_SCREENSIZE:
        case FBIOGET_SCREEN_ORIGIN_HIFB:
        case FBIOPUT_SCREEN_ORIGIN_HIFB:
        case FBIOGET_VBLANK_HIFB:
        case FBIOPUT_SHOW_HIFB:
        case FBIOGET_SHOW_HIFB:
        case FBIO_WAITFOR_FREFRESH_DONE:
        case FBIOPUT_COMPRESSION_HIFB:
        case FBIOGET_COMPRESSION_HIFB:
        case FBIOFLIP_SURFACE:

#ifdef MDDRDETECT
        case FBIOPUT_MDDRDETECT_HIFB:
        case FBIOGET_MDDRDETECT_HIFB:
#endif
        {
            bFilter = HI_TRUE;//These interfaces need to be filtered
            break;
        }
        default:
            break;
    }

    return bFilter;
}


static HI_S32 hifb_check_cusor_phyaddr(HIFB_SURFACE_S *pstCursor)
{
    HIFB_SURFACE_S *pstCursorSur = pstCursor;
    HI_U64          u64Len    = 0;
    if (pstCursor == NULL) {
        return HI_FAILURE;
    }

    u64Len = pstCursorSur->u32Pitch * pstCursorSur->u32Height;
    return cmpi_check_mmz_phy_addr(pstCursorSur->u64PhyAddr, u64Len);
}


/******************************************************************************
 Function        : hifb_ioctl
 Description     : set the colorkey or alpha for overlay
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : struct inode *inode
                   struct file *file
                   unsigned HI_S32 cmd
                   unsigned long arg
                   struct fb_info *info
 Return          : return 0 if succeed, otherwise return error code
 Others          : 0
******************************************************************************/
static HI_S32 hifb_ioctl(struct fb_info *info, HI_U32 cmd, unsigned long arg)
{
    HIFB_PAR_S *                   par                = NULL;
    HIFB_DISPLAY_INFO_S *          pstDisplayInfo     = NULL;
    HIFB_REFRESH_INFO_S *          pstRefreshInfo     = NULL;
    volatile
        HIFB_COMPRESS_INFO_S *     pstCompressInfo    = NULL;
    HI_VOID __user *               argp               = (HI_VOID __user *)(HI_UINTPTR_T)arg;
    HI_U32                         u32LayerId         = 0;
    HIFB_SPIN_LOCK_FLAG            u32LockFlag;
    HI_S32 s32Ret = HI_SUCCESS;

    if (info == NULL) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;
    pstDisplayInfo = &par->stDisplayInfo;
    pstRefreshInfo = &par->stRefreshInfo;
    pstCompressInfo = &par->stCompressInfo;
    u32LayerId = par->u32LayerID;
    if (u32LayerId >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    if ((argp == NULL) && (cmd != FBIOGET_VBLANK_HIFB)
        && (cmd != FBIO_WAITFOR_FREFRESH_DONE) && (cmd != FBIO_RELEASE_HIFB))
    {
        HIFB_ERROR("NULL arg!\n");
        return -EINVAL;
    }

    if ((!s_stDrvOps.pstCapability[u32LayerId].bLayerSupported)
        && (!IS_CURSOR_LAYER(u32LayerId)|| (!IS_SOFT_CURSOR())))
    {
        HIFB_ERROR("not supprot layer %d!\n", u32LayerId);
        return HI_FAILURE;
    }

    /* Turn on the soft mouse function, and it is the mouse layer, you need to filter the hard interface. */
    if((IS_SOFT_CURSOR())&&(IS_CURSOR_LAYER(u32LayerId)))
    {
        if(hifb_cmdfilter(cmd))
        {
            HIFB_ERROR("soft cursor layer %d not support this function!\n", u32LayerId);
            return HI_FAILURE;
        }
    }

    switch (cmd)
    {
#ifdef __HuaweiLite__
        struct hifb_info *info_temp;
        struct hifb_info info_copy;
        case FBIOGET_SCREENINFO_HIFB:
        {
            osal_memcpy((void *)arg, info, sizeof(struct hifb_info));
            break;
        }
        case FBIOPUT_SCREENINFO_HIFB:
        {
            osal_memcpy(&info_copy, info, sizeof(info_copy));
            info_temp = (struct hifb_info *)arg;
            if (info->vtable.fb_set_par)
            {
#if 1
                info->vinfo.xres = info_temp->vinfo.xres;
                info->vinfo.yres = info_temp->vinfo.yres;
                info->vinfo.fmt = info_temp->vinfo.fmt;
                info->oinfo.sarea.w = info_temp->oinfo.sarea.w;
                info->oinfo.sarea.h = info_temp->oinfo.sarea.h;
                info->oinfo.sarea.x = info_temp->oinfo.sarea.x;
                info->oinfo.sarea.y = info_temp->oinfo.sarea.y;
                info->oinfo.bpp = info_temp->oinfo.bpp;
#endif
                s32Ret = info->vtable.fb_set_par(&info->vtable);
                if (s32Ret != HI_SUCCESS)
                {
                    HIFB_ERROR("Put screeninfo error! ret=%d\n", s32Ret);
                    osal_memcpy(info, &info_copy, sizeof(info_copy));
                    break;
                }

                hifb_pan_display(&info->vtable, &info->oinfo);
            }
            break;
        }

        case FBIOPAN_DISPLAY_HIFB:
        {
            struct fb_overlayinfo_s *oinfo = (struct fb_overlayinfo_s *)arg;
            unsigned int xres = info->vinfo.yres;
            unsigned int yres = info->vinfo.yres;
            unsigned int xoffset = oinfo->sarea.x;
            unsigned int yoffset = oinfo->sarea.y;
            if (info->vtable.fb_pan_display)
            {
                s32Ret = info->vtable.fb_pan_display(&info->vtable, oinfo);
                if (s32Ret != HI_SUCCESS)
                {
                    HIFB_ERROR("Put screeninfo error! ret=%d\n", s32Ret);
                    osal_memcpy(info, &info_copy, sizeof(info_copy));
                    break;
                }
                info->oinfo.sarea.x = xoffset;
                info->oinfo.sarea.y = yoffset;
            }
        }
            break;
#endif
        case FBIO_REFRESH:
        {
            s32Ret = hifb_onrefresh(par, argp);
            break;
        }

        case FBIOPUT_MIRROR_MODE:
        {
            HIFB_MIRROR_MODE_E enMirrorMode;
            if (copy_from_user(&enMirrorMode, argp, sizeof(HIFB_MIRROR_MODE_E)))
            {
                return -EFAULT;
            }
            s32Ret = hifb_set_mirrormode(par, enMirrorMode);
            break;
        }

        case FBIOGET_MIRROR_MODE:
        {
            HIFB_LAYER_BUF_E enBufMode = 0;
            HIFB_MIRROR_MODE_E enMirrorMode = 0;
            hifb_get_bufmode(par, &enBufMode);
            enMirrorMode = pstDisplayInfo->enMirrorMode;
            if (HIFB_LAYER_BUF_BUTT == enBufMode || HIFB_LAYER_BUF_NONE == enBufMode )
            {
                HIFB_ERROR("doesn't support FBIOGET_MIRROR_MODE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
                return HI_FAILURE;
            }

            if (copy_to_user(argp, &enMirrorMode, sizeof(HIFB_MIRROR_MODE_E)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;

        }

        case FBIOPUT_ROTATE_MODE:
        {
            HIFB_ROTATE_MODE_E enRotateMode;
            if (copy_from_user(&enRotateMode, argp, sizeof(HIFB_ROTATE_MODE_E)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_set_rotatemode(info, enRotateMode);
            break;

        }

        case FBIOGET_ROTATE_MODE:
        {
            HIFB_LAYER_BUF_E enBufMode = 0;
            HIFB_ROTATE_MODE_E enRotateMode = 0;

            hifb_get_bufmode(par, &enBufMode);

            if (HIFB_LAYER_BUF_BUTT == enBufMode || HIFB_LAYER_BUF_NONE == enBufMode )
            {
                HIFB_ERROR("doesn't support FBIOGET_ROTATE_MODE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
                return HI_FAILURE;
            }
            enRotateMode = pstDisplayInfo->enRotateMode;
            if (copy_to_user(argp, &enRotateMode, sizeof(HIFB_ROTATE_MODE_E)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;
        }

        case FBIOGET_CANVAS_BUFFER:
        {
            HIFB_BUFFER_S stBuf = {0};

            osal_memcpy(&(stBuf.stCanvas), &(par->CanvasSur), sizeof(HIFB_SURFACE_S));
            osal_memcpy(&(stBuf.UpdateRect), &(pstRefreshInfo->stUserBuffer.UpdateRect), sizeof(HIFB_RECT));
            if (copy_to_user(argp, &(stBuf), sizeof(HIFB_BUFFER_S)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;
        }

        case FBIOPUT_LAYER_INFO:
        {
            s32Ret= hifb_onputlayerinfo(info, par, argp);
            break;
        }

        case FBIOGET_LAYER_INFO:
        {
            HIFB_LAYER_INFO_S stLayerInfo = {0} ;

            hifb_get_layerinfo(par, &stLayerInfo);

            return copy_to_user(argp, &stLayerInfo, sizeof(HIFB_LAYER_INFO_S));
        }

        case FBIOGET_ALPHA_HIFB:
        {
            HIFB_ALPHA_S stAlpha = {0};

            hifb_get_alpha(par, &stAlpha);

            return copy_to_user(argp, &stAlpha, sizeof(HIFB_ALPHA_S));
        }

        case FBIOPUT_ALPHA_HIFB:
        {
            HIFB_ALPHA_S stAlpha = {0};
            if (copy_from_user(&stAlpha, argp, sizeof(HIFB_ALPHA_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != stAlpha.bAlphaEnable && HI_FALSE != stAlpha.bAlphaEnable)
            {
                HIFB_ERROR("pstAlpha->bAlphaEnable should be HI_TRUE or HI_FALSE!\n");
                return HI_FAILURE;
            }

            if (HI_TRUE != stAlpha.bAlphaChannel && HI_FALSE != stAlpha.bAlphaChannel)
            {
                HIFB_ERROR("pstAlpha->bAlphaChannel should be HI_TRUE or HI_FALSE!\n");
                return HI_FAILURE;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
            par->bModifying = HI_TRUE;
            hifb_set_alpha(par, &stAlpha);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_ALPHA;
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
            break;
        }

        case FBIOGET_DEFLICKER_HIFB:
        {
            if (!s_stDrvOps.pstCapability[par->u32LayerID].u32HDefLevel
                && !s_stDrvOps.pstCapability[par->u32LayerID].u32VDefLevel)
            {
                HIFB_ERROR("deflicker is not supported!\n");
                return -EPERM;
            }

/* There is currently no chip that needs to support this feature, so comment out first. */
            break;
        }

        case FBIOPUT_DEFLICKER_HIFB:
        {
            if (!s_stDrvOps.pstCapability[par->u32LayerID].u32HDefLevel
                && !s_stDrvOps.pstCapability[par->u32LayerID].u32VDefLevel)
            {
                HIFB_ERROR("deflicker is not supported!\n");
                return -EPERM;
            }
/* There is currently no chip that needs to support this feature, so comment out first. */
            break;
        }

        case FBIOGET_COLORKEY_HIFB:
        {
            HIFB_COLORKEY_S   ck       = {0};
            HIFB_COLORKEYEX_S stCkeyEx = {0};

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                HIFB_ERROR("Layer %d doesn't support colorkey!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            hifb_get_key(par, &stCkeyEx);

            ck.bKeyEnable = stCkeyEx.bKeyEnable;
            ck.u32Key = stCkeyEx.u32Key;

            return copy_to_user(argp, &ck, sizeof(HIFB_COLORKEY_S));
        }

        case FBIOPUT_COLORKEY_HIFB:
        {
            HIFB_COLORKEY_S ckey;
            HIFB_COLORKEYEX_S stCkeyEx;
            unsigned long lockflag;

            if (copy_from_user(&ckey, argp, sizeof(HIFB_COLORKEY_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != ckey.bKeyEnable && HI_FALSE != ckey.bKeyEnable)
            {
                HIFB_ERROR("bKeyEnable(%d) should be HI_TRUE or HI_FALSE!\n", ckey.bKeyEnable);
                return HI_FAILURE;
            }

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                HIFB_ERROR("Layer %d doesn't support colorkey!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (ckey.bKeyEnable && pstDisplayInfo->bPreMul)
            {
                HIFB_ERROR("colorkey and premul couldn't take effect at the same time!\n");
                return HI_FAILURE;
            }

            stCkeyEx.u32Key = ckey.u32Key;
            stCkeyEx.bKeyEnable = ckey.bKeyEnable;

            if (hifb_get_bits_per_pixel(info)<= 8)
            {
                if (ckey.u32Key >= (2 << hifb_get_bits_per_pixel(info)))
                {
                    HIFB_ERROR("The key :%d is out of range the palette: %d!\n",
                                ckey.u32Key, 2 << hifb_get_bits_per_pixel(info));
                    return HI_FAILURE;
                }

                stCkeyEx.u8BlueMax = stCkeyEx.u8BlueMin = info->cmap.blue[ckey.u32Key];
                stCkeyEx.u8GreenMax = stCkeyEx.u8GreenMin = info->cmap.green[ckey.u32Key];
                stCkeyEx.u8RedMax = stCkeyEx.u8RedMin = info->cmap.red[ckey.u32Key];
            }
            else
            {
#ifdef __HuaweiLite__
                s_stDrvOps.HIFB_DRV_ColorConvert(info, &stCkeyEx);
#else
                s_stDrvOps.HIFB_DRV_ColorConvert(&info->var, &stCkeyEx);
#endif
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;
            hifb_set_key(par, &stCkeyEx);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            break;
        }

        case FBIOPUT_SCREENSIZE:
        {
            HIFB_SIZE_S  stScreenSize;
            HIFB_SIZE_S  stMaxScreenSize;
            HIFB_POINT_S stPos             = {0};
            unsigned long lockflag;

            if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't set soft cursor screensize!");
                return HI_FAILURE;
            }

            if (copy_from_user(&stScreenSize, argp, sizeof(HIFB_SIZE_S)))
            {
                return -EFAULT;
            }

            if ((stScreenSize.u32Width == 0) || (stScreenSize.u32Height == 0))
            {
                HIFB_ERROR("screen width(%u) height(%u) shouldn't be 0\n", stScreenSize.u32Width, stScreenSize.u32Height);
                return HI_FAILURE;
            }

            if (stScreenSize.u32Width % 2 || stScreenSize.u32Height % 2)
            {
                HIFB_ERROR("stScreenSize (%u, %u) should align to 2!\n", stScreenSize.u32Width, stScreenSize.u32Height);
                return HI_FAILURE;
            }

            hifb_get_maxscreensize(par, &stMaxScreenSize.u32Width, &stMaxScreenSize.u32Height);
            hifb_get_layerpos(par, &stPos);

            if (stScreenSize.u32Width > stMaxScreenSize.u32Width - stPos.s32XPos)
            {
                HIFB_WARNING("the sum of u32Width(%d) and s32XPos(%d) larger than Vodev screen width(%d),"\
                    "u32Width will be changed!\n",\
                    stScreenSize.u32Width, stPos.s32XPos, stMaxScreenSize.u32Width);
                stScreenSize.u32Width = stMaxScreenSize.u32Width - stPos.s32XPos;
            }

            if (stScreenSize.u32Height > stMaxScreenSize.u32Height - stPos.s32YPos)
            {
                HIFB_WARNING("the sum of u32Height(%d) and s32YPos(%d) larger than Vodev screen height(%d),"\
                    "u32Width will be changed!\n",\
                    stScreenSize.u32Height, stPos.s32YPos, stMaxScreenSize.u32Height);
                stScreenSize.u32Height = stMaxScreenSize.u32Height - stPos.s32YPos;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;

            s32Ret = hifb_set_screensize(par, &stScreenSize.u32Width, &stScreenSize.u32Height);
            if (HI_SUCCESS == s32Ret)
            {
                par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;
            }

            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);
            break;
        }

        case FBIOGET_SCREENSIZE:
        {
            HIFB_SIZE_S stScreenSize = {0};

            hifb_get_screensize(par, &stScreenSize.u32Width, &stScreenSize.u32Height);

            return copy_to_user(argp, &stScreenSize, sizeof(HIFB_SIZE_S));
        }

        case FBIOGET_SCREEN_ORIGIN_HIFB:
        {
            HIFB_POINT_S stPos = {0};

            hifb_get_layerpos(par, &stPos);

            return copy_to_user(argp, &stPos, sizeof(HIFB_POINT_S));

            break;
        }

        case FBIOPUT_SCREEN_ORIGIN_HIFB:
        {
            HIFB_POINT_S stExpectedPos;
            HI_U32 u32LayerId = par->u32LayerID;
            unsigned long lockflag;
            HI_BOOL bInterlace = hifb_is_interlace(par);

            if (IS_CURSOR_LAYER(u32LayerId) && IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't set soft cursor origion by this cmd, try FBIOPUT_CURSOR_POS");
                return HI_FAILURE;
            }

            if (copy_from_user(&stExpectedPos, argp, sizeof(HIFB_POINT_S)))
            {
                return -EFAULT;
            }

            if (stExpectedPos.s32XPos < 0 || stExpectedPos.s32YPos < 0)
            {
                HIFB_ERROR("It's not supported to set start pos of layer to negative!\n");
                return HI_FAILURE;
            }

            if (bInterlace && (0 != stExpectedPos.s32YPos % 2))
            {
                HIFB_ERROR("s32YPos should be even for interlace vodev!\n");
                return HI_FAILURE;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;

            /* Record the old location first */

            hifb_set_layerpos(par, &stExpectedPos);
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            break;
        }

        case FBIOGET_VBLANK_HIFB:
        {
            if (hifb_wait_regconfig_work(par->u32LayerID))
            {
                HIFB_ERROR("It is not support VBL!\n");
                return -EPERM;
            }

            break;
        }

        case FBIOPUT_SHOW_HIFB:
        {
            HI_BOOL bShow;
            unsigned long lockflag;

            if (IS_CURSOR_LAYER(u32LayerId) && IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't show sot cursor by this cmd, try FBIOPUT_CURSOR_STATE!");
                return HI_FAILURE;
            }

            if (copy_from_user(&bShow, argp, sizeof(HI_BOOL)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != bShow && HI_FALSE != bShow)
            {
                HIFB_ERROR("bShow(%d) should be HI_TRUE or HI_FALSE!\n", bShow);
                return HI_FAILURE;
            }

            if (bShow == hifb_get_show(par))
            {
                HIFB_INFO("The layer is show(%d) now!\n", par->bShow);
                return 0;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;

            hifb_set_show(par, bShow);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;

            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            break;
        }

        case FBIOGET_SHOW_HIFB:
        {
            HI_BOOL bShow = HI_FALSE;
            bShow = par->bShow;
            return copy_to_user(argp, &bShow, sizeof(HI_BOOL));
        }

        case FBIO_WAITFOR_FREFRESH_DONE:
        {
            HIFB_LAYER_BUF_E enBufMode;

            hifb_get_bufmode(par, &enBufMode);
            if(HIFB_LAYER_BUF_NONE != enBufMode && HIFB_LAYER_BUF_BUTT != enBufMode)
            {
                s32Ret = wait_event_interruptible_timeout(par->vblEvent, par->vblflag, msecs_to_jiffies(80));
                if(0 > s32Ret )
                {
                    return -ERESTARTSYS;
                }
                else
                {
                    s32Ret = HI_SUCCESS;
                }
            }
            else
            {
                HIFB_ERROR("doesn't support FBIO_WAITFOR_FREFRESH_DONE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
                return HI_FAILURE;
            }

            break;
        }

        case FBIOGET_CAPABILITY_HIFB:
        {
            HIFB_CAPABILITY_S stCapability = {0};
            stCapability = s_stDrvOps.pstCapability[par->u32LayerID];
            return copy_to_user(argp, (HI_VOID *)&stCapability, sizeof(HIFB_CAPABILITY_S));
        }
#ifdef CURSOR
        case FBIOPUT_CURSOR_ATTCHCURSOR:
        {
            HI_U32 u32Layer;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't attach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&u32Layer, argp, sizeof(HI_U32)))
            {
                return -EFAULT;
            }

            if ((u32Layer >= s_stDrvOps.u32LayerCount)
                || (HI_FALSE == s_stDrvOps.pstCapability[u32Layer].bLayerSupported))
            {
                HIFB_ERROR("LayerId %d should be in [0, %d) or it is not supported!\n", u32Layer, s_stDrvOps.u32LayerCount);
                return -EFAULT;
            }

            if(IS_CURSOR_LAYER(u32Layer))
            {
                HIFB_ERROR("Don't allow attach cursor to itself or other cursor layer!\n");
                return HI_FAILURE;
            }

            return hifb_cursor_attach(par->u32LayerID,u32Layer);
        }

        case FBIOPUT_CURSOR_DETACHCURSOR:
        {
            HI_U32 u32Layer;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&u32Layer, argp, sizeof(HI_U32)))
            {
                return -EFAULT;
            }

            if ((u32Layer >= s_stDrvOps.u32LayerCount)
                || (HI_FALSE == s_stDrvOps.pstCapability[u32Layer].bLayerSupported))
            {
                HIFB_ERROR("LayerId %d should be in [0, %d) or it is not supported!\n", u32Layer, s_stDrvOps.u32LayerCount);
                return -EFAULT;
            }

            if(IS_CURSOR_LAYER(u32Layer))
            {
                HIFB_ERROR("Detach cursor to itself or other cursor layer!\n");
                return HI_FAILURE;
            }

            return hifb_cursor_detach(u32Layer);
        }

        case FBIOPUT_CURSOR_INFO:
        {
            HIFB_CURSOR_S stCursorInfo;
            osal_memset(&stCursorInfo, 0, sizeof(HIFB_CURSOR_S));
            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&stCursorInfo, argp, sizeof(HIFB_CURSOR_S)))
            {
                return -EFAULT;
            }

            if(HI_FAILURE == hifb_check_cusor_phyaddr(&stCursorInfo.stCursor))
            {
                HIFB_ERROR("cursor mmz phy addr 0x%llx invalid.\n",stCursorInfo.stCursor.u64PhyAddr);
                return HI_FAILURE;
            }

            s32Ret = hifb_cursor_putinfo(par, &stCursorInfo);

            break;
        }

        case FBIOGET_CURSOR_INFO:
        {
            HIFB_CURSOR_INFO_S stCursorInfo = {0};
            HIFB_CURSOR_S stCursor = {0};
            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_cursorinfo(par, &stCursorInfo);
            stCursor = stCursorInfo.stCursor;
            return copy_to_user(argp, &stCursor, sizeof(HIFB_CURSOR_S));
        }

        case FBIOPUT_CURSOR_STATE:
        {
            HI_BOOL bShow;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&bShow, argp, sizeof(HI_BOOL)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_cursor_changestate(par, bShow);

            break;
        }

        case FBIOGET_CURSOR_STATE:
        {
            HI_BOOL bShow = HI_FALSE;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            bShow = hifb_get_show(par);

            return copy_to_user(argp, &bShow, sizeof(HI_BOOL));
        }

        case FBIOPUT_CURSOR_POS:
        {
            HIFB_POINT_S stPos = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&stPos, argp, sizeof(HIFB_POINT_S)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_cursor_changepos(par->u32LayerID,stPos);

            break;
        }

        case FBIOGET_CURSOR_POS:
        {
            HIFB_POINT_S stPos = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_layerpos(par, &stPos);

            return copy_to_user(argp, &stPos, sizeof(HIFB_POINT_S));
        }

        case FBIOPUT_CURSOR_ALPHA:
        {
            HIFB_ALPHA_S stAlpha = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't set hard cursor alpha by this cmd!\n");
                return HI_FAILURE;
            }

            if (copy_from_user(&stAlpha, argp, sizeof(HIFB_ALPHA_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != stAlpha.bAlphaEnable && HI_FALSE != stAlpha.bAlphaEnable)
            {
                HIFB_ERROR("pstAlphaInfo->bAlphaEnable(%d) should be HI_TRUE or HI_FALSE!\n", stAlpha.bAlphaEnable);
                return HI_FAILURE;
            }

            if (HI_TRUE != stAlpha.bAlphaChannel && HI_FALSE != stAlpha.bAlphaChannel)
            {
                HIFB_ERROR("pstAlphaInfo->bAlphaChannel(%d) should be HI_TRUE or HI_FALSE!\n", stAlpha.bAlphaChannel);
                return HI_FAILURE;
            }

            hifb_set_alpha(par, &stAlpha);

            break;
        }

        case FBIOGET_CURSOR_ALPHA:
        {
            HIFB_ALPHA_S stAlpha = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_alpha(par, &stAlpha);

            return copy_to_user(argp, &stAlpha, sizeof(HIFB_ALPHA_S));
        }

        case FBIOPUT_CURSOR_COLORKEY:
        {
            HIFB_COLORKEY_S stColorKey;
            HIFB_COLORKEYEX_S stCKeyEx;
            HIFB_COLOR_FMT_E enColorFmt;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&stColorKey, argp, sizeof(HIFB_COLORKEY_S)))
            {
                return -EFAULT;
            }

            stCKeyEx.u32Key = stColorKey.u32Key;
            stCKeyEx.bKeyEnable = stColorKey.bKeyEnable;

            hifb_get_fmt(par, &enColorFmt);

#ifdef __HuaweiLite__
            s_stDrvOps.HIFB_DRV_ColorConvert(info, &stCKeyEx);
#else
            s_stDrvOps.HIFB_DRV_ColorConvert(&info->var, &stCKeyEx);
#endif

            hifb_set_key(par, &stCKeyEx);

            break;
        }

        case FBIOGET_CURSOR_COLORKEY:
        {
            HIFB_COLORKEY_S ck = {0};
            HIFB_COLORKEYEX_S stCkeyEx = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't detach hard cursor by this cmd!\n");
                return HI_FAILURE;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_key(par, &stCkeyEx);

            ck.bKeyEnable = stCkeyEx.bKeyEnable;
            ck.u32Key = stCkeyEx.u32Key;

            return copy_to_user(argp, &(ck), sizeof(HIFB_COLORKEY_S));
        }
#endif
        case FBIOPUT_COMPRESSION_HIFB:
        {
            HI_BOOL bCompress;
            HIFB_LAYER_BUF_E enBufMode;

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_ERROR("Layer %d doesn't support compression operation!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (copy_from_user(&bCompress, argp, sizeof(HI_BOOL)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != bCompress && HI_FALSE != bCompress)
            {
                HIFB_ERROR("bCompress(%d) should be HI_TRUE or HI_FALSE!\n", bCompress);
                return HI_FAILURE;
            }

            if (bCompress != pstCompressInfo->bCompressOpen)
            {
                if (bCompress)
                {
                    if (par->enColFmt != HIFB_FMT_ARGB8888 && par->enColFmt != HIFB_FMT_ARGB1555 && par->enColFmt != HIFB_FMT_ARGB4444)
                    {
                        HIFB_ERROR("compression only support pixel format (ARGB8888,ARGB1555,ARGB4444)\n");
                        return HI_FAILURE;
                    }

                    if(HIFB_MIRROR_NONE != pstDisplayInfo->enMirrorMode || HIFB_ROTATE_NONE != pstDisplayInfo->enRotateMode)
                    {
                        HIFB_ERROR("Can't do compression when mirror or rotate!\n");
                        return HI_FAILURE;
                    }

                    hifb_get_bufmode(par, &enBufMode);

                    /* FB uses frame decompression, can not be displayed while refreshing, so only supports double buf mode */
#ifdef CONFIG_HIFB_FENCE_SUPPORT
                    if (HIFB_LAYER_BUF_DOUBLE != enBufMode && HIFB_LAYER_BUF_DOUBLE_IMMEDIATE != enBufMode
                    && HIFB_LAYER_BUF_FENCE != enBufMode)
#else
                    if (HIFB_LAYER_BUF_DOUBLE != enBufMode && HIFB_LAYER_BUF_DOUBLE_IMMEDIATE != enBufMode)
#endif
                    {
                        HIFB_ERROR("only HIFB_LAYER_BUF_DOUBLE or HIFB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
                        return HI_FAILURE;
                    }

                    /* Record the entire image as the area to be compressed */
                    HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
                    pstCompressInfo->stCompressRect.x = 0;
                    pstCompressInfo->stCompressRect.y = 0;
                    pstCompressInfo->stCompressRect.w = pstDisplayInfo->u32DisplayWidth;
                    pstCompressInfo->stCompressRect.h = pstDisplayInfo->u32DisplayHeight;
                    pstCompressInfo->bUpdateFinished = HI_TRUE;
                    pstCompressInfo->bDelay = HI_TRUE;
                    pstCompressInfo->bCompressOpen = HI_TRUE;
                    HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);



                }
                else
                {
                    HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
                    pstCompressInfo->bCompressOpen = HI_FALSE;
                    HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
                    /* Waiting for an interrupt before proceeding to the next two operations */
                    hifb_wait_regconfig_work(par->u32LayerID);
                    hifb_FreeCompressBuffer(par);
                    /* Reset compression information */
                    pstCompressInfo->stUpdateRect.w = 0;
                    pstCompressInfo->stUpdateRect.h = 0;
                }
            }

            return HI_SUCCESS;
        }

        case FBIOGET_COMPRESSION_HIFB:
        {
            HI_BOOL bCompress = HI_FALSE;

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_WARNING("Layer %d doesn't support get compression!\n", par->u32LayerID);
            }
            else
            {
                bCompress = pstCompressInfo->bCompressOpen;
            }

            return copy_to_user(argp, &bCompress, sizeof(HI_BOOL));
        }

        case FBIOFLIP_SURFACE:
        {
            HIFB_SURFACEEX_S surface;
            unsigned long addr;
            unsigned long smem_end;
            unsigned int differ;
            unsigned int xoffset;
            unsigned int yoffset;
            unsigned long lockflag;
            HIFB_COLORKEYEX_S stCkeyEx = {0};
#ifdef __HuaweiLite__
            struct fb_overlayinfo_s oinfo;
#else
            struct fb_var_screeninfo var;
#endif

            if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
            {
                HIFB_ERROR("you shouldn't use FBIOFLIP_SURFACE for soft cursor!");
                return HI_FAILURE;
            }

            if (copy_from_user(&surface, argp, sizeof(HIFB_SURFACEEX_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != surface.stColorkey.bKeyEnable && HI_FALSE != surface.stColorkey.bKeyEnable)
            {
                HIFB_ERROR("stColorkey.bKeyEnable(%d) should be HI_TRUE or HI_FALSE!\n", surface.stColorkey.bKeyEnable);
                return HI_FAILURE;
            }

            if (HI_TRUE != surface.stAlpha.bAlphaEnable && HI_FALSE != surface.stAlpha.bAlphaEnable)
            {
                HIFB_ERROR("stAlpha.bAlphaEnable(%d) should be HI_TRUE or HI_FALSE!\n", surface.stAlpha.bAlphaEnable);
                return HI_FAILURE;
            }

            if (HI_TRUE != surface.stAlpha.bAlphaChannel && HI_FALSE != surface.stAlpha.bAlphaChannel)
            {
                HIFB_ERROR("stAlpha.bAlphaChannel (%d) should be HI_TRUE or HI_FALSE!\n", surface.stAlpha.bAlphaChannel);
                return HI_FAILURE;
            }


            if (surface.stColorkey.bKeyEnable && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                HIFB_ERROR("Layer %d doesn't support colorkey!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (surface.stColorkey.bKeyEnable && pstDisplayInfo->bPreMul)
            {
                HIFB_ERROR("colorkey and premul couldn't take effect at the same time!\n");
                return HI_FAILURE;
            }

            addr = (unsigned long)surface.u64PhyAddr;
            smem_end = hifb_get_smem_start(info)+ hifb_get_smem_len(info)- hifb_get_yres(info)* hifb_get_line_length(info);
            if((addr < hifb_get_smem_start(info))
                || (addr > smem_end))
            {
                HIFB_ERROR("the addr is out of range!\n");
                return HI_FAILURE;
            }

            if (hifb_get_line_length(info) == 0) {
                return HI_FAILURE;
            }

            if (hifb_get_bits_per_pixel(info) == 0) {
                return HI_FAILURE;
            }

            differ = addr - hifb_get_smem_start(info);
            yoffset = differ/hifb_get_line_length(info);
            xoffset = ((differ%hifb_get_line_length(info) << 3)/(hifb_get_bits_per_pixel(info)));
#ifdef __HuaweiLite__
            osal_memcpy(&oinfo, &info->oinfo, sizeof(oinfo));
            oinfo.sarea.x = xoffset;
            oinfo.sarea.y = yoffset;

            if (hifb_pan_display(info, &oinfo) < 0)
            {
                HIFB_ERROR("error!\n");
                return HI_FAILURE;
            }

            info->oinfo.sarea.x = xoffset;
            info->oinfo.sarea.y = yoffset;
#else
            osal_memcpy(&var, &info->var,sizeof(var));
            var.xoffset = xoffset;
            var.yoffset = yoffset;

            if (fb_pan_display(info, &var) < 0)
            {
                HIFB_ERROR("pan_display error!\n");
                return HI_FAILURE;
            }
#endif
            stCkeyEx.u32Key = surface.stColorkey.u32Key;
            stCkeyEx.bKeyEnable = surface.stColorkey.bKeyEnable;

            if (hifb_get_bits_per_pixel(info) <= 8)
            {
                if (surface.stColorkey.u32Key >= (2 << hifb_get_bits_per_pixel(info)))
                {
                    HIFB_ERROR("The key :%d is out of range the palette: %d!\n",
                                surface.stColorkey.u32Key, 2 << hifb_get_bits_per_pixel(info));
                    return HI_FAILURE;
                }

                stCkeyEx.u8BlueMax = stCkeyEx.u8BlueMin = info->cmap.blue[surface.stColorkey.u32Key];
                stCkeyEx.u8GreenMax = stCkeyEx.u8GreenMin = info->cmap.green[surface.stColorkey.u32Key];
                stCkeyEx.u8RedMax = stCkeyEx.u8RedMin = info->cmap.red[surface.stColorkey.u32Key];
            }
            else
            {
#ifdef __HuaweiLite__
                s_stDrvOps.HIFB_DRV_ColorConvert(info, &stCkeyEx);
#else
                s_stDrvOps.HIFB_DRV_ColorConvert(&info->var, &stCkeyEx);
#endif
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;
            hifb_set_alpha(par, &surface.stAlpha);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_ALPHA;
            if (s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                || s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                hifb_set_key(par, &stCkeyEx);
                par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
            }
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            return 0;
        }

#ifdef MDDRDETECT
        case FBIOPUT_MDDRDETECT_HIFB:
        {
            HIFB_DDRZONE_S stDDRZonePara = {0};
            if (HI_FALSE == s_stDrvOps.HIFB_DRV_IsLayerSupportDDRDetect(par->u32LayerID))
            {
                HIFB_ERROR("Layer %d doesn't support DDR detect PUT.\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_ERROR("Layer %d doesn't support DDR detect!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (copy_from_user(&stDDRZonePara, argp, sizeof(HIFB_DDRZONE_S)))
            {
                return -EFAULT;
            }

            /* check detect zone*/
            s32Ret = s_stDrvOps.HIFB_DRV_CheckDDR_DectectZone(par->u32LayerID,
            stDDRZonePara.u32StartSection, stDDRZonePara.u32ZoneNums,
            pstCompressInfo->u32StartSection, pstCompressInfo->u32ZoneNums);
            if (HI_FAILURE == s32Ret)
            {
                HIFB_ERROR("Layer %d DDR detect zone ERR!\n", par->u32LayerID);

                return s32Ret;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);

            /*If you repeat the settings, return directly*/
            if (stDDRZonePara.u32StartSection == pstCompressInfo->u32StartSection
                && stDDRZonePara.u32ZoneNums == pstCompressInfo->u32ZoneNums)
            {
                HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

                return HI_SUCCESS;
            }

            pstCompressInfo->u32NewStartSection = stDDRZonePara.u32StartSection;
            pstCompressInfo->u32NewZoneNums = stDDRZonePara.u32ZoneNums;
            if (!stDDRZonePara.u32ZoneNums)
            {
                pstCompressInfo->bClearZone = HI_TRUE;
            }
            else
            {
                pstCompressInfo->bClearZone = HI_FALSE;
            }
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

            return HI_SUCCESS;
        }
        case FBIOGET_MDDRDETECT_HIFB:
        {
            HIFB_DDRZONE_S stDDRZonePara = {0};
            if (HI_FALSE == s_stDrvOps.HIFB_DRV_IsLayerSupportDDRDetect(par->u32LayerID))
            {
                HIFB_ERROR("Layer %d doesn't support DDR detect GET.\n", par->u32LayerID);
                return HI_FAILURE;
            }
            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_ERROR("Layer %d doesn't support DDR detect!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
            stDDRZonePara.u32StartSection = pstCompressInfo->u32NewStartSection;
            stDDRZonePara.u32ZoneNums = pstCompressInfo->u32NewZoneNums;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

            return copy_to_user(argp, &stDDRZonePara, sizeof(HIFB_DDRZONE_S));
        }
#endif
        case FBIOPUT_DYNAMIC_RANGE_HIFB:
        {
            HIFB_DYNAMIC_RANGE_E enDynamicRange = HIFB_DYNAMIC_RANGE_BUTT;
            if(!s_stDrvOps.pstCapability[par->u32LayerID].bGHDR)
            {
                HIFB_ERROR("Layer %d# doesn't support HDR function.\n",par->u32LayerID);
                return HI_FAILURE;
            }
            if (copy_from_user(&enDynamicRange, argp, sizeof(HIFB_DYNAMIC_RANGE_E)))
            {
                return -EFAULT;
            }
            s32Ret = hifb_setDynamicRange(par,enDynamicRange);
            break;
        }

        case FBIOGET_DYNAMIC_RANGE_HIFB:
        {
            HIFB_DYNAMIC_RANGE_E DynamicRange = 0;
            if(!s_stDrvOps.pstCapability[par->u32LayerID].bGHDR)
            {
                HIFB_ERROR("Layer %d# doesn't support HDR function.\n",par->u32LayerID);
                return HI_FAILURE;
            }
            DynamicRange = pstDisplayInfo->enDynamicRange;
            if (copy_to_user(argp, &DynamicRange, sizeof(HIFB_DYNAMIC_RANGE_E)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;
        }

        case FBIO_RELEASE_HIFB:
        {
            s32Ret = hifb_releaseLayer(par);
            break;
        }

        default:
        {
            HIFB_ERROR("the command:0x%x is unsupported!\n", cmd);
            return -EINVAL;
        }
    }

    return s32Ret;
}

#ifdef CONFIG_COMPAT
/******************************************************************************
 Function        : compat_hifb_ioctl
 Description     : set the colorkey or alpha for overlay
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : struct inode *inode
                   struct file *file
                   unsigned HI_S32 cmd
                   unsigned long arg
                   struct fb_info *info
 Return          : return 0 if succeed, otherwise return error code
 Others          : 0
******************************************************************************/

static HI_S32 hifb_compat_ioctl(struct fb_info *info, unsigned cmd, unsigned long arg)
{
    HIFB_PAR_S *                   par                = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *          pstDisplayInfo     = &par->stDisplayInfo;
    HIFB_REFRESH_INFO_S *          pstRefreshInfo     = &par->stRefreshInfo;
    volatile
        HIFB_COMPRESS_INFO_S *     pstCompressInfo    = &par->stCompressInfo;
    HI_VOID __user *               argp               = (HI_VOID __user *)arg;
    HI_U32                         u32LayerId         = par->u32LayerID;
    HIFB_SPIN_LOCK_FLAG            u32LockFlag;

    HI_S32                         s32Ret             = HI_SUCCESS;

    if ((argp == NULL) && (cmd != FBIOGET_VBLANK_HIFB) && (cmd != FBIO_WAITFOR_FREFRESH_DONE))
    {
        HIFB_ERROR("NULL arg!\n");
        return -EINVAL;
    }

    if ((!s_stDrvOps.pstCapability[u32LayerId].bLayerSupported)
        && (!IS_CURSOR_LAYER(u32LayerId)|| (!IS_SOFT_CURSOR())))
    {
        HIFB_ERROR("Layer %d# not supprot !\n", u32LayerId);
        return HI_FAILURE;
    }
    switch (cmd)
    {
        case FBIO_REFRESH:
        {
            s32Ret = hifb_onrefresh(par, argp);
            break;
        }

        case FBIOPUT_MIRROR_MODE:
        {
            HIFB_MIRROR_MODE_E enMirrorMode = HIFB_MIRROR_BUTT;
            if (copy_from_user(&enMirrorMode, argp, sizeof(HIFB_MIRROR_MODE_E)))
            {
                return -EFAULT;
            }
            s32Ret = hifb_set_mirrormode(par, enMirrorMode);
            break;
        }

        case FBIOGET_MIRROR_MODE:
        {
            if (copy_to_user(argp, &(pstDisplayInfo->enMirrorMode), sizeof(HIFB_MIRROR_MODE_E)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;

        }

        case FBIOPUT_ROTATE_MODE:
        {
            HIFB_ROTATE_MODE_E enRotateMode = HIFB_ROTATE_BUTT;
            if (copy_from_user(&enRotateMode, argp, sizeof(HIFB_ROTATE_MODE_E)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_set_rotatemode(info, enRotateMode);
            break;

        }

        case FBIOGET_ROTATE_MODE:
        {
            if (copy_to_user(argp, &(pstDisplayInfo->enRotateMode), sizeof(HIFB_ROTATE_MODE_E)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;
        }

        case FBIOGET_CANVAS_BUFFER:
        {
            HIFB_BUFFER_S stBuf = {0};

            osal_memcpy(&(stBuf.stCanvas), &(par->CanvasSur), sizeof(HIFB_SURFACE_S));
            osal_memcpy(&(stBuf.UpdateRect), &(pstRefreshInfo->stUserBuffer.UpdateRect), sizeof(HIFB_RECT));
            if (copy_to_user(argp, &(stBuf), sizeof(HIFB_BUFFER_S)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;
        }

        case FBIOPUT_LAYER_INFO:
        {
            s32Ret= hifb_onputlayerinfo(info, par, argp);
            break;
        }

        case FBIOGET_LAYER_INFO:
        {
            HIFB_LAYER_INFO_S stLayerInfo;

            hifb_get_layerinfo(par, &stLayerInfo);

            return copy_to_user(argp, &stLayerInfo, sizeof(HIFB_LAYER_INFO_S));
        }

        case FBIOGET_ALPHA_HIFB:
        {
            HIFB_ALPHA_S stAlpha;

            hifb_get_alpha(par, &stAlpha);

            return copy_to_user(argp, &stAlpha, sizeof(HIFB_ALPHA_S));
        }

        case FBIOPUT_ALPHA_HIFB:
        {
            HIFB_ALPHA_S stAlpha = {0};
            if (copy_from_user(&stAlpha, argp, sizeof(HIFB_ALPHA_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != stAlpha.bAlphaEnable && HI_FALSE != stAlpha.bAlphaEnable)
            {
                HIFB_ERROR("pstAlpha->bAlphaEnable should be HI_TRUE or HI_FALSE!\n");
                return HI_FAILURE;
            }

            if (HI_TRUE != stAlpha.bAlphaChannel && HI_FALSE != stAlpha.bAlphaChannel)
            {
                HIFB_ERROR("pstAlpha->bAlphaChannel should be HI_TRUE or HI_FALSE!\n");
                return HI_FAILURE;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
            par->bModifying = HI_TRUE;
            hifb_set_alpha(par, &stAlpha);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_ALPHA;
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
            break;
        }

        case FBIOGET_DEFLICKER_HIFB:
        {
            HIFB_DEFLICKER_S deflicker = {0};

            if (!s_stDrvOps.pstCapability[par->u32LayerID].u32HDefLevel
                && !s_stDrvOps.pstCapability[par->u32LayerID].u32VDefLevel)
            {
                HIFB_WARNING("deflicker is not supported!\n");
                return -EPERM;
            }

            if (copy_from_user(&deflicker, argp, sizeof(HIFB_DEFLICKER_S)))
            {
                return -EFAULT;
            }

            deflicker.u32HDfLevel = par->u32HDflevel;
            deflicker.u32VDfLevel = par->u32VDflevel;
            if (par->u32HDflevel > 1)
            {
                if (NULL == deflicker.pu8HDfCoef)
                {
                    return -EFAULT;
                }
                COMPAT_POINTER(deflicker.pu8HDfCoef,HI_U8 *);
                if (copy_to_user(deflicker.pu8HDfCoef, par->ucHDfcoef, par->u32HDflevel - 1))
                {
                    return -EFAULT;
                }
            }

            if (par->u32VDflevel > 1)
            {
                if (NULL == deflicker.pu8VDfCoef)
                {
                    return -EFAULT;
                }
                COMPAT_POINTER(deflicker.pu8VDfCoef,HI_U8 *);
                if (copy_to_user(deflicker.pu8VDfCoef, par->ucVDfcoef, par->u32VDflevel - 1))
                {
                    return -EFAULT;
                }
            }

            if (copy_to_user(argp, &deflicker, sizeof(deflicker)))
            {
                return -EFAULT;
            }

            break;
        }

        case FBIOPUT_DEFLICKER_HIFB:
        {
            HIFB_DEFLICKER_S deflicker;
            HI_U32      u32HDflevel;
            HI_U32      u32VDflevel;
            HI_UCHAR    aucHDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];
            HI_UCHAR    aucVDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];

            if (!s_stDrvOps.pstCapability[par->u32LayerID].u32HDefLevel
                && !s_stDrvOps.pstCapability[par->u32LayerID].u32VDefLevel)
            {
                HIFB_WARNING("deflicker is not supported!\n");
                return -EPERM;
            }

            if (copy_from_user(&deflicker, argp, sizeof(HIFB_DEFLICKER_S)))
            {
                return -EFAULT;
            }
            u32HDflevel = HIFB_MIN(deflicker.u32HDfLevel, s_stDrvOps.pstCapability[par->u32LayerID].u32HDefLevel);
            u32VDflevel = HIFB_MIN(deflicker.u32VDfLevel, s_stDrvOps.pstCapability[par->u32LayerID].u32VDefLevel);
            if (((u32HDflevel > 1) && (NULL == deflicker.pu8HDfCoef))
                || ((u32VDflevel > 1) && (NULL == deflicker.pu8VDfCoef)))
            {
                HIFB_ERROR("deflicker parameter is illegal!\n");
                return HI_FAILURE;
            }

            if (u32HDflevel > 1)
            {
                if (copy_from_user(aucHDfcoef, deflicker.pu8HDfCoef, u32HDflevel - 1))
                {
                    return -EFAULT;
                }
                COMPAT_POINTER(deflicker.pu8HDfCoef,HI_U8 *);
            }

            if (u32VDflevel > 1)
            {
                if (copy_from_user(aucVDfcoef, deflicker.pu8VDfCoef, u32VDflevel - 1))
                {
                    return -EFAULT;
                }
                COMPAT_POINTER(deflicker.pu8VDfCoef,HI_U8 *);
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
            par->bModifying = HI_TRUE;

            par->u32HDflevel = u32HDflevel;
            if ((par->u32HDflevel > 1))
            {
                osal_memcpy(par->ucHDfcoef, aucHDfcoef, par->u32HDflevel - 1);
            }
            par->u32VDflevel = u32VDflevel;
            if (par->u32VDflevel > 1)
            {
                osal_memcpy(par->ucVDfcoef, aucVDfcoef, par->u32VDflevel - 1);
            }

            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL;
            par->bModifying = HI_FALSE;

            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

            break;
        }

        case FBIOGET_COLORKEY_HIFB:
        {
            HIFB_COLORKEY_S   ck       = {0};
            HIFB_COLORKEYEX_S stCkeyEx = {0};

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                HIFB_ERROR("Layer %d doesn't support colorkey!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            hifb_get_key(par, &stCkeyEx);

            ck.bKeyEnable = stCkeyEx.bKeyEnable;
            ck.u32Key = stCkeyEx.u32Key;

            return copy_to_user(argp, &ck, sizeof(HIFB_COLORKEY_S));
        }

        case FBIOPUT_COLORKEY_HIFB:
        {
            HIFB_COLORKEY_S ckey;
            HIFB_COLORKEYEX_S stCkeyEx;
            unsigned long lockflag;

            if (copy_from_user(&ckey, argp, sizeof(HIFB_COLORKEY_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != ckey.bKeyEnable && HI_FALSE != ckey.bKeyEnable)
            {
                HIFB_ERROR("bKeyEnable(%d) should be HI_TRUE or HI_FALSE!\n", ckey.bKeyEnable);
                return HI_FAILURE;
            }

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                HIFB_ERROR("Layer %d doesn't support colorkey!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (ckey.bKeyEnable && pstDisplayInfo->bPreMul)
            {
                HIFB_ERROR("colorkey and premul couldn't take effect at the same time!\n");
                return HI_FAILURE;
            }

            stCkeyEx.u32Key = ckey.u32Key;
            stCkeyEx.bKeyEnable = ckey.bKeyEnable;

            if (hifb_get_bits_per_pixel(info) <= 8)
            {
                if (ckey.u32Key >= (2 << hifb_get_bits_per_pixel(info)))
                {
                    HIFB_ERROR("The key :%d is out of range the palette: %d!\n",
                                ckey.u32Key, 2 << hifb_get_bits_per_pixel(info));
                    return HI_FAILURE;
                }

                stCkeyEx.u8BlueMax = stCkeyEx.u8BlueMin = info->cmap.blue[ckey.u32Key];
                stCkeyEx.u8GreenMax = stCkeyEx.u8GreenMin = info->cmap.green[ckey.u32Key];
                stCkeyEx.u8RedMax = stCkeyEx.u8RedMin = info->cmap.red[ckey.u32Key];
            }
            else
            {
#ifdef __HuaweiLite__
                 s_stDrvOps.HIFB_DRV_ColorConvert(info, &stCkeyEx);
#else
                 s_stDrvOps.HIFB_DRV_ColorConvert(&info->var, &stCkeyEx);
#endif
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;
            hifb_set_key(par, &stCkeyEx);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            break;
        }

        case FBIOPUT_SCREENSIZE:
        {
            HIFB_SIZE_S   stScreenSize;
            HIFB_SIZE_S   stMaxScreenSize  = { 0, 0 };
            HIFB_POINT_S  stPos            = {0};
            unsigned long lockflag;

            if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't set soft cursor screensize!");
                return HI_SUCCESS;
            }

            if (copy_from_user(&stScreenSize, argp, sizeof(HIFB_SIZE_S)))
            {
                return -EFAULT;
            }

            /* For interlaced output, the actual height of the layer must be an even number.
            Progressive output without this limit */
            if ((0 != (stScreenSize.u32Height % 2)) && hifb_is_interlace(par))
            {
                HIFB_ERROR("stScreenSize u32Height(%d) should be even for interlace vodev!", \
                    stScreenSize.u32Height);
                return HI_FAILURE;
            }

            hifb_get_maxscreensize(par, &stMaxScreenSize.u32Width, &stMaxScreenSize.u32Height);
            hifb_get_layerpos(par, &stPos);

            if (stScreenSize.u32Width > stMaxScreenSize.u32Width - stPos.s32XPos)
            {
                HIFB_WARNING("the sum of u32Width(%d) and s32XPos(%d) larger than Vodev screen width(%d),"\
                    "u32Width will be changed!\n",\
                    stScreenSize.u32Width, stPos.s32XPos, stMaxScreenSize.u32Width);
                stScreenSize.u32Width = stMaxScreenSize.u32Width - stPos.s32XPos;
            }

            if (stScreenSize.u32Height > stMaxScreenSize.u32Height - stPos.s32YPos)
            {
                HIFB_WARNING("the sum of u32Height(%d) and s32YPos(%d) larger than Vodev screen height(%d),"\
                    "u32Width will be changed!\n",\
                    stScreenSize.u32Height, stPos.s32YPos, stMaxScreenSize.u32Height);
                stScreenSize.u32Height = stMaxScreenSize.u32Height - stPos.s32YPos;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;
            // Set screen size
            s32Ret = hifb_set_screensize(par, &stScreenSize.u32Width, &stScreenSize.u32Height);
            // Set the modified output size mask
            if (HI_SUCCESS == s32Ret)
            {
                par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;
            }

            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);
            break;
        }

        case FBIOGET_SCREENSIZE:
        {
            HIFB_SIZE_S stScreenSize = {0};

            hifb_get_screensize(par, &stScreenSize.u32Width, &stScreenSize.u32Height);

            return copy_to_user(argp, &stScreenSize, sizeof(HIFB_SIZE_S));
        }

        case FBIOGET_SCREEN_ORIGIN_HIFB:
        {
            HIFB_POINT_S stPos = {0};

            hifb_get_layerpos(par, &stPos);

            return copy_to_user(argp, &stPos, sizeof(HIFB_POINT_S));

            break;
        }

        case FBIOPUT_SCREEN_ORIGIN_HIFB:
        {
            HIFB_POINT_S stExpectedPos;
            HI_U32 u32LayerId = par->u32LayerID;
            unsigned long lockflag;
            HI_BOOL bInterlace = hifb_is_interlace(par);

            if (IS_CURSOR_LAYER(u32LayerId) && IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't set soft cursor origion by this cmd, try FBIOPUT_CURSOR_POS");
                return HI_SUCCESS;
            }

            if (copy_from_user(&stExpectedPos, argp, sizeof(HIFB_POINT_S)))
            {
                return -EFAULT;
            }

            if (stExpectedPos.s32XPos < 0 || stExpectedPos.s32YPos < 0)
            {
                HIFB_ERROR("It's not supported to set start pos of layer to negative!\n");
                return HI_FAILURE;
            }

            if (bInterlace && (0 != stExpectedPos.s32YPos % 2))
            {
                HIFB_ERROR("s32YPos should be even for interlace vodev!\n");
                return HI_FAILURE;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;

            hifb_set_layerpos(par, &stExpectedPos);
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            break;
        }

        case FBIOGET_VBLANK_HIFB:
        {
            if (hifb_wait_regconfig_work(par->u32LayerID))
            {
                HIFB_WARNING("It is not support VBL!\n");
                return -EPERM;
            }

            break;
        }

        case FBIOPUT_SHOW_HIFB:
        {
            HI_BOOL bShow = HI_FALSE;
            unsigned long lockflag = 0;

            if (IS_CURSOR_LAYER(u32LayerId) && IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't show sot cursor by this cmd, try FBIOPUT_CURSOR_STATE!");
                return HI_SUCCESS;
            }

            if (copy_from_user(&bShow, argp, sizeof(HI_BOOL)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != bShow && HI_FALSE != bShow)
            {
                HIFB_ERROR("bShow(%d) should be HI_TRUE or HI_FALSE!\n", bShow);
                return HI_FAILURE;
            }

            if (bShow == hifb_get_show(par))
            {
                HIFB_INFO("The layer is show(%d) now!\n", par->bShow);
                return 0;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;

            hifb_set_show(par, bShow);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;

            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            break;
        }

        case FBIOGET_SHOW_HIFB:
        {
            return copy_to_user(argp, &par->bShow, sizeof(HI_BOOL));
        }

        case FBIO_WAITFOR_FREFRESH_DONE:
        {
            HIFB_LAYER_BUF_E enBufMode;

            hifb_get_bufmode(par, &enBufMode);
            if(enBufMode != HIFB_LAYER_BUF_NONE)
            {
                s32Ret = wait_event_interruptible_timeout(par->vblEvent, par->vblflag, msecs_to_jiffies(80));
                if(0 > s32Ret )
                {
                    return -ERESTARTSYS;
                }
                else
                {
                    s32Ret = HI_SUCCESS;
                }
            }

            break;
        }

        case FBIOGET_CAPABILITY_HIFB:
        {
            return copy_to_user(argp, (HI_VOID *)&s_stDrvOps.pstCapability[par->u32LayerID], sizeof(HIFB_CAPABILITY_S));
        }

        case FBIOPUT_CURSOR_ATTCHCURSOR:
        {
            HI_U32 u32Layer;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't attach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&u32Layer, argp, sizeof(HI_U32)))
            {
                return -EFAULT;
            }

            if ((u32Layer >= s_stDrvOps.u32LayerCount)
                || (HI_FALSE == s_stDrvOps.pstCapability[u32Layer].bLayerSupported))
            {
                HIFB_ERROR("LayerId %d should be in [0, %d) or it is not supported!\n", u32Layer, s_stDrvOps.u32LayerCount);
                return -EFAULT;
            }

            if(IS_CURSOR_LAYER(u32Layer))
            {
                HIFB_ERROR("Don't allow attach cursor to itself or other cursor layer!\n");
                return HI_FAILURE;
            }

            return hifb_cursor_attach(par->u32LayerID,u32Layer);
        }

        case FBIOPUT_CURSOR_DETACHCURSOR:
        {
            HI_U32 u32Layer = 0;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&u32Layer, argp, sizeof(HI_U32)))
            {
                return -EFAULT;
            }

            if ((u32Layer >= s_stDrvOps.u32LayerCount)
                || (HI_FALSE == s_stDrvOps.pstCapability[u32Layer].bLayerSupported))
            {
                HIFB_ERROR("LayerId %d should be in [0, %d) or it is not supported!\n", u32Layer, s_stDrvOps.u32LayerCount);
                return -EFAULT;
            }

            if(IS_CURSOR_LAYER(u32Layer))
            {
                HIFB_ERROR("Detach cursor to itself or other cursor layer!\n");
                return HI_FAILURE;
            }

            return hifb_cursor_detach(u32Layer);
        }

        case FBIOPUT_CURSOR_INFO:
        {
            HIFB_CURSOR_S stCursorInfo = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&stCursorInfo, argp, sizeof(HIFB_CURSOR_S)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_cursor_putinfo(par, &stCursorInfo);

            break;
        }

        case FBIOGET_CURSOR_INFO:
        {
            HIFB_CURSOR_INFO_S stCursorInfo = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_cursorinfo(par, &stCursorInfo);

            return copy_to_user(argp, &stCursorInfo.stCursor, sizeof(HIFB_CURSOR_S));
        }

        case FBIOPUT_CURSOR_STATE:
        {
            HI_BOOL bShow;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&bShow, argp, sizeof(HI_BOOL)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_cursor_changestate(par, bShow);

            break;
        }

        case FBIOGET_CURSOR_STATE:
        {
            HI_BOOL bShow = HI_FALSE;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            bShow = hifb_get_show(par);

            return copy_to_user(argp, &bShow, sizeof(HI_BOOL));
        }

        case FBIOPUT_CURSOR_POS:
        {
            HIFB_POINT_S stPos = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&stPos, argp, sizeof(HIFB_POINT_S)))
            {
                return -EFAULT;
            }

            s32Ret = hifb_cursor_changepos(par->u32LayerID,stPos);

            break;
        }

        case FBIOGET_CURSOR_POS:
        {
            HIFB_POINT_S stPos = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_layerpos(par, &stPos);

            return copy_to_user(argp, &stPos, sizeof(HIFB_POINT_S));
        }

        case FBIOPUT_CURSOR_ALPHA:
        {
            HIFB_ALPHA_S stAlpha = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't set hard cursor alpha by this cmd!\n");
                return HI_SUCCESS;
            }

            if (copy_from_user(&stAlpha, argp, sizeof(HIFB_ALPHA_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != stAlpha.bAlphaEnable && HI_FALSE != stAlpha.bAlphaEnable)
            {
                HIFB_ERROR("pstAlphaInfo->bAlphaEnable(%d) should be HI_TRUE or HI_FALSE!\n", stAlpha.bAlphaEnable);
                return HI_FAILURE;
            }

            if (HI_TRUE != stAlpha.bAlphaChannel && HI_FALSE != stAlpha.bAlphaChannel)
            {
                HIFB_ERROR("pstAlphaInfo->bAlphaChannel(%d) should be HI_TRUE or HI_FALSE!\n", stAlpha.bAlphaChannel);
                return HI_FAILURE;
            }

            hifb_set_alpha(par, &stAlpha);

            break;
        }

        case FBIOGET_CURSOR_ALPHA:
        {
            HIFB_ALPHA_S stAlpha = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_alpha(par, &stAlpha);

            return copy_to_user(argp, &stAlpha, sizeof(HIFB_ALPHA_S));
        }

        case FBIOPUT_CURSOR_COLORKEY:
        {
            HIFB_COLORKEY_S stColorKey;
            HIFB_COLORKEYEX_S stCKeyEx;
            HIFB_COLOR_FMT_E enColorFmt;

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            if (copy_from_user(&stColorKey, argp, sizeof(HIFB_COLORKEY_S)))
            {
                return -EFAULT;
            }

            stCKeyEx.u32Key = stColorKey.u32Key;
            stCKeyEx.bKeyEnable = stColorKey.bKeyEnable;

            hifb_get_fmt(par, &enColorFmt);

#ifdef __HuaweiLite__
            s_stDrvOps.HIFB_DRV_ColorConvert(info, &stCkeyEx);
#else
            s_stDrvOps.HIFB_DRV_ColorConvert(&info->var, &stCKeyEx);
#endif

            hifb_set_key(par, &stCKeyEx);

            break;
        }

        case FBIOGET_CURSOR_COLORKEY:
        {
            HIFB_COLORKEY_S ck = {0};
            HIFB_COLORKEYEX_S stCkeyEx = {0};

            if (!IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't detach hard cursor by this cmd!\n");
                return HI_SUCCESS;
            }

            HIFB_CHECK_CURSOR_LAYERID(par->u32LayerID);

            hifb_get_key(par, &stCkeyEx);

            ck.bKeyEnable = stCkeyEx.bKeyEnable;
            ck.u32Key = stCkeyEx.u32Key;

            return copy_to_user(argp, &(ck), sizeof(HIFB_COLORKEY_S));
        }

        case FBIOPUT_COMPRESSION_HIFB:
        {
            HI_BOOL bCompress;
            // Whether the layer supports decompression, the premise of supporting compression is to support decompression.
            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_WARNING("Layer %d doesn't support compression operation!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (copy_from_user(&bCompress, argp, sizeof(HI_BOOL)))
            {
                return -EFAULT;
            }
            if (bCompress != pstCompressInfo->bCompressOpen)
            {
                if (bCompress)
                {
                    if (par->enColFmt != HIFB_FMT_ARGB8888 && par->enColFmt != HIFB_FMT_ARGB1555 && par->enColFmt != HIFB_FMT_ARGB4444)
                    {
                        HIFB_ERROR("compression only support pixel format (ARGB8888,ARGB1555,ARGB4444)\n");
                        return HI_FAILURE;
                    }

                    if (HIFB_MIRROR_NONE != pstDisplayInfo->enMirrorMode)
                    {
                        HIFB_ERROR("Can't do compression when mirror.\n");
                        return HI_FAILURE;
                    }

                    /* Record the entire image as the area to be compressed */
                    HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
                    pstCompressInfo->stCompressRect.x = 0;
                    pstCompressInfo->stCompressRect.y = 0;
                    pstCompressInfo->stCompressRect.w = pstDisplayInfo->u32DisplayWidth;
                    pstCompressInfo->stCompressRect.h = pstDisplayInfo->u32DisplayHeight;
                    pstCompressInfo->bUpdateFinished = HI_TRUE;
                    pstCompressInfo->bDelay = HI_TRUE;

                    HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
                    // Allocate memory
                    s32Ret = hifb_AllocCompressBuffer(par);
                    if (s32Ret != HI_SUCCESS)
                    {
                        HIFB_ERROR("alloc compress buffer fail!\n");
                        return HI_FAILURE;
                    }
                    // compression is setted to TRUE.
                    pstCompressInfo->bCompressOpen = HI_TRUE;

                }
                else
                {
                    HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
                    pstCompressInfo->bCompressOpen = HI_FALSE;
                    HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);
                    /* Waiting for an interrupt before proceeding to the next two operations */
                    hifb_wait_regconfig_work(par->u32LayerID);
                    hifb_FreeCompressBuffer(par);
                    /* Reset compression information */
                    pstCompressInfo->stUpdateRect.w = 0;
                    pstCompressInfo->stUpdateRect.h = 0;
                }
            }

            return HI_SUCCESS;
        }

        case FBIOGET_COMPRESSION_HIFB:
        {
            HI_BOOL bCompress = HI_FALSE;

            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_WARNING("Layer %d doesn't support get compression!\n", par->u32LayerID);
            }
            else
            {
                bCompress = pstCompressInfo->bCompressOpen;
            }

            return copy_to_user(argp, &bCompress, sizeof(HI_BOOL));
        }

        case FBIOFLIP_SURFACE:
        {
            HIFB_SURFACEEX_S surface;
            unsigned long addr;
            unsigned long smem_end;
            unsigned int differ;
            unsigned int xoffset;
            unsigned int yoffset;
            unsigned long lockflag;
            HIFB_COLORKEYEX_S stCkeyEx = {0};
            struct fb_var_screeninfo var;

            if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
            {
                HIFB_WARNING("you shouldn't use FBIOFLIP_SURFACE for soft cursor!");
                return HI_SUCCESS;
            }

            if (copy_from_user(&surface, argp, sizeof(HIFB_SURFACEEX_S)))
            {
                return -EFAULT;
            }

            if (HI_TRUE != surface.stColorkey.bKeyEnable && HI_FALSE != surface.stColorkey.bKeyEnable)
            {
                HIFB_ERROR("stColorkey.bKeyEnable(%d) should be HI_TRUE or HI_FALSE!\n", surface.stColorkey.bKeyEnable);
                return HI_FAILURE;
            }

            if (HI_TRUE != surface.stAlpha.bAlphaEnable && HI_FALSE != surface.stAlpha.bAlphaEnable)
            {
                HIFB_ERROR("stAlpha.bAlphaEnable(%d) should be HI_TRUE or HI_FALSE!\n", surface.stAlpha.bAlphaEnable);
                return HI_FAILURE;
            }

            if (HI_TRUE != surface.stAlpha.bAlphaChannel && HI_FALSE != surface.stAlpha.bAlphaChannel)
            {
                HIFB_ERROR("stAlpha.bAlphaChannel (%d) should be HI_TRUE or HI_FALSE!\n", surface.stAlpha.bAlphaChannel);
                return HI_FAILURE;
            }


            if (surface.stColorkey.bKeyEnable && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                && !s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                HIFB_ERROR("Layer %d doesn't support colorkey!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (surface.stColorkey.bKeyEnable && pstDisplayInfo->bPreMul)
            {
                HIFB_ERROR("colorkey and premul couldn't take effect at the same time!\n");
                return HI_FAILURE;
            }

            addr = (unsigned long)surface.u64PhyAddr;
            smem_end = hifb_get_smem_start(info) + hifb_get_smem_len(info) - hifb_get_yres(info) * hifb_get_line_length(info);
            if((addr < hifb_get_smem_start(info))
                || (addr > smem_end))
            {
                HIFB_WARNING("the addr is out of range!\n");
                return -1;
            }
            if (hifb_get_line_length(info) == 0) {
                return HI_FAILURE;
            }
            if (hifb_get_bits_per_pixel(info) == 0) {
                return HI_FAILURE;
            }

            differ = addr - hifb_get_smem_start(info);
            yoffset = differ/hifb_get_line_length(info);
            xoffset = ((differ%hifb_get_line_length(info)) << 3)/(hifb_get_bits_per_pixel(info));

            osal_memcpy(&var, &info->var,sizeof(var));
            var.xoffset = xoffset;
            var.yoffset = yoffset;

            if (fb_pan_display(info, &var) < 0)
            {
                HIFB_ERROR("error!\n");
                return -1;
            }

            stCkeyEx.u32Key = surface.stColorkey.u32Key;
            stCkeyEx.bKeyEnable = surface.stColorkey.bKeyEnable;

            if (hifb_get_bits_per_pixel(info) <= 8)
            {
                if (surface.stColorkey.u32Key >= (2 << hifb_get_bits_per_pixel(info)))
                {
                    HIFB_ERROR("The key :%d is out of range the palette: %d!\n",
                                surface.stColorkey.u32Key, 2 << hifb_get_bits_per_pixel(info));
                    return HI_FAILURE;
                }

                stCkeyEx.u8BlueMax = stCkeyEx.u8BlueMin = info->cmap.blue[surface.stColorkey.u32Key];
                stCkeyEx.u8GreenMax = stCkeyEx.u8GreenMin = info->cmap.green[surface.stColorkey.u32Key];
                stCkeyEx.u8RedMax = stCkeyEx.u8RedMin = info->cmap.red[surface.stColorkey.u32Key];
            }
            else
            {
#ifdef __HuaweiLite__
                s_stDrvOps.HIFB_DRV_ColorConvert(info, &stCkeyEx);
#else
                s_stDrvOps.HIFB_DRV_ColorConvert(&info->var, &stCkeyEx);
#endif
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
            par->bModifying = HI_TRUE;
            hifb_set_alpha(par, &surface.stAlpha);
            par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_ALPHA;
            if (s_stDrvOps.pstCapability[par->u32LayerID].bKeyRgb
                || s_stDrvOps.pstCapability[par->u32LayerID].bKeyAlpha)
            {
                hifb_set_key(par, &stCkeyEx);
                par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
            }
            par->bModifying = HI_FALSE;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

            return 0;
        }
        case FBIOPUT_MDDRDETECT_HIFB:
        {
            HIFB_DDRZONE_S stDDRZonePara = {0};
            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_ERROR("Layer %d doesn't support DDR detect!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            if (copy_from_user(&stDDRZonePara, argp, sizeof(HIFB_DDRZONE_S)))
            {
                return -EFAULT;
            }

            s32Ret = s_stDrvOps.HIFB_DRV_CheckDDR_DectectZone(par->u32LayerID,
            stDDRZonePara.u32StartSection, stDDRZonePara.u32ZoneNums,
            pstCompressInfo->u32StartSection, pstCompressInfo->u32ZoneNums);
            if (HI_FAILURE == s32Ret)
            {
                HIFB_ERROR("Layer %d DDR detect zone ERR!\n", par->u32LayerID);

                return s32Ret;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);

            /*Repeat settings, return directly*/
            if (stDDRZonePara.u32StartSection == pstCompressInfo->u32StartSection
                && stDDRZonePara.u32ZoneNums == pstCompressInfo->u32ZoneNums)
            {
                HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

                return HI_SUCCESS;
            }

            pstCompressInfo->u32NewStartSection = stDDRZonePara.u32StartSection;
            pstCompressInfo->u32NewZoneNums = stDDRZonePara.u32ZoneNums;
            if (!stDDRZonePara.u32ZoneNums)
            {
                pstCompressInfo->bClearZone = HI_TRUE;
            }
            else
            {
                pstCompressInfo->bClearZone = HI_FALSE;
            }
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

            return HI_SUCCESS;
        }
        case FBIOGET_MDDRDETECT_HIFB:
        {
            HIFB_DDRZONE_S stDDRZonePara = {0};
            if (!s_stDrvOps.pstCapability[par->u32LayerID].bDcmp)
            {
                HIFB_ERROR("Layer %d doesn't support DDR detect!\n", par->u32LayerID);
                return HI_FAILURE;
            }

            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, u32LockFlag);
            stDDRZonePara.u32StartSection = pstCompressInfo->u32NewStartSection;
            stDDRZonePara.u32ZoneNums = pstCompressInfo->u32NewZoneNums;
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, u32LockFlag);

            return copy_to_user(argp, &stDDRZonePara, sizeof(HIFB_DDRZONE_S));
        }

        case FBIOPUT_DYNAMIC_RANGE_HIFB:
        {
            HIFB_DYNAMIC_RANGE_E enDynamicRange = HIFB_DYNAMIC_RANGE_BUTT;
            if(!s_stDrvOps.pstCapability[par->u32LayerID].bGHDR)
            {
                HIFB_ERROR("Layer %d# doesn't support HDR function.\n",par->u32LayerID);
                return HI_FAILURE;
            }
            if (copy_from_user(&enDynamicRange, argp, sizeof(HIFB_DYNAMIC_RANGE_E)))
            {
                return -EFAULT;
            }
            s32Ret = hifb_setDynamicRange(par,enDynamicRange);
            break;
        }

        case FBIOGET_DYNAMIC_RANGE_HIFB:
        {
            if (copy_to_user(argp, &(pstDisplayInfo->enDynamicRange), sizeof(HIFB_DYNAMIC_RANGE_E)))
            {
                return -EFAULT;
            }
            return HI_SUCCESS;
        }

        default:
        {
            osal_printk("+++Attention!!!cmd in default=\t\t0x%x,%s,%d+++\n",cmd,__FUNCTION__,__LINE__);
            HIFB_ERROR("the command:0x%x is unsupported!\n", cmd);
            return -EINVAL;
        }
    }

    return s32Ret;
}
#endif

static HI_VOID hifb_version(HI_VOID)
{
    /* Use "strings hifb.ko | grep "HIFB_MAIN_VERSION"" to get the version */
    // HI_CHAR HIFBVersion[80] ="HIFB_MAIN_VERSION["
    //     MKMARCOTOSTR(HIFB_MAIN_VERSION)"] Build Time["
    //     __DATE__", "__TIME__"]";
    HI_CHAR HIFBVersion[80] ="HIFB_MAIN_VERSION["\
        MKMARCOTOSTR(HIFB_MAIN_VERSION)"] Build Time[202009291801]";

    HIFB_INFO("%s\n", HIFBVersion);
}

static HI_S32 hifb_bitfieldcmp(struct fb_bitfield x, struct fb_bitfield y)
{
    if ((x.offset == y.offset)
        && (x.length == y.length)
        && (x.msb_right == y.msb_right))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static HI_U32 hifb_getbppbyfmt(HIFB_COLOR_FMT_E enColorFmt)
{
    switch(enColorFmt)
    {
        case HIFB_FMT_RGB565:
        case HIFB_FMT_KRGB444:
        case HIFB_FMT_KRGB555:
        case HIFB_FMT_ARGB4444:
        case HIFB_FMT_ARGB1555:
        case HIFB_FMT_RGBA4444:
        case HIFB_FMT_RGBA5551:
        case HIFB_FMT_ACLUT88:
        case HIFB_FMT_BGR565:
        case HIFB_FMT_ABGR1555:
        case HIFB_FMT_ABGR4444:
        case HIFB_FMT_KBGR444:
        case HIFB_FMT_KBGR555:
        {
            return 16;
        }
        case HIFB_FMT_RGB888:
        case HIFB_FMT_ARGB8565:
        case HIFB_FMT_RGBA5658:
        case HIFB_FMT_ABGR8565:
        case HIFB_FMT_BGR888:
        {
            return 24;
        }
        case HIFB_FMT_KRGB888:
        case HIFB_FMT_ARGB8888:
        case HIFB_FMT_RGBA8888:
        case HIFB_FMT_ABGR8888:
        case HIFB_FMT_KBGR888:
        {
            return 32;
        }
        case HIFB_FMT_1BPP:
        {
            return 1;
        }
        case HIFB_FMT_2BPP:
        {
            return 2;
        }
        case HIFB_FMT_4BPP:
        {
            return 4;
        }
        case HIFB_FMT_8BPP:
        case HIFB_FMT_ACLUT44:
        {
            return 8;
        }
        default:
            return 0;
    }
}

static HIFB_COLOR_FMT_E hifb_getfmtbyargb(struct fb_bitfield *red, struct fb_bitfield *green,
                       struct fb_bitfield *blue, struct fb_bitfield *transp, HI_U32 u32ColorDepth)
{
    HI_U32 i = 0;
    HI_U32 u32Bpp;

    if ((red == NULL) || (green == NULL) || (blue == NULL) || (transp == NULL)) {
        return  HIFB_FMT_BUTT;
    }

    /* not support color palette low than 8bit*/
    if (u32ColorDepth < 8)
    {
        return  HIFB_FMT_BUTT;
    }

    if (u32ColorDepth == 8)
    {
        return HIFB_FMT_8BPP;
    }

    /* Find the pixel format (HIFB_ARGB_BITINFO_S) corresponding to the given red,
     * green, and blue bit field information and the number of bits per pixel (bpp)
     */
    for (i = 0; i < sizeof(s_stArgbBitField)/sizeof(HIFB_ARGB_BITINFO_S); i++)
    {
        if ((hifb_bitfieldcmp(*red, s_stArgbBitField[i].stRed) == 0)
            && (hifb_bitfieldcmp(*green, s_stArgbBitField[i].stGreen) == 0)
            && (hifb_bitfieldcmp(*blue, s_stArgbBitField[i].stBlue) == 0)
            && (hifb_bitfieldcmp(*transp, s_stArgbBitField[i].stTransp) == 0))
        {
            u32Bpp = hifb_getbppbyfmt(i);
            if (u32Bpp == u32ColorDepth)
            {
                return i;
            }
        }
    }
    i = HIFB_FMT_BUTT;
    return i;
}

HI_S32 hifb_checkmem_enough(struct fb_info *info,HI_U32 u32Pitch,HI_U32 u32Height)
{
    HI_U32                     u32BufferNum   = 0;
    HI_U32                     uBuffersize    = 0;
    HIFB_PAR_S *par = NULL;
    HIFB_REFRESH_INFO_S *pstRefreshInfo = NULL;
    if (info == NULL) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;
    pstRefreshInfo = &par->stRefreshInfo;

    switch(pstRefreshInfo->enBufMode)
    {
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        case HIFB_LAYER_BUF_FENCE:
#endif
        case HIFB_LAYER_BUF_DOUBLE:
        case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
        {
            u32BufferNum = 2;
            break;
        }
        case HIFB_LAYER_BUF_ONE:
        {
            u32BufferNum = 1;
            break;
        }
        default:
            return HI_SUCCESS;
    }
    // The interface setting requires uBuffersize, the actual memory size info->fix.smem_len
    uBuffersize = u32BufferNum * u32Pitch * u32Height;
    if(hifb_get_smem_len(info)>= uBuffersize)
    {
        return HI_SUCCESS;
    }
    HIFB_ERROR("memory is not enough!  now is %d u32Pitch %d u32Height %d expect %d\n",
        hifb_get_smem_len(info),u32Pitch, u32Height,uBuffersize);
    return HI_FAILURE;
}


/*
* Address check only for uncompressed data
*/
static HI_S32 hifb_check_phyaddr(HIFB_BUFFER_S * pstCanvasBuf)
{
    HIFB_BUFFER_S * pstCanBuf = pstCanvasBuf;
    HI_U64          u64Len    = 0;
    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    u64Len = (HI_U64)pstCanBuf->stCanvas.u32Pitch * pstCanBuf->stCanvas.u32Height;

    return cmpi_check_mmz_phy_addr(pstCanBuf->stCanvas.u64PhyAddr, u64Len);
}


#ifndef __HuaweiLite__
static HI_S32 hifb_check_fmt(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_COLOR_FMT_E enFmt;
    HIFB_PAR_S *par = NULL;
    HI_U32 u32LayerId = 0;
    if ((info == NULL) || (var == NULL)) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;
    u32LayerId = par->u32LayerID;

    enFmt = hifb_getfmtbyargb(&var->red, &var->green, &var->blue, &var->transp, var->bits_per_pixel);
    if (enFmt == HIFB_FMT_BUTT)
    {
        HIFB_ERROR("Unknown fmt(offset, length) r:(%d,%d,%d) ,g:(%d,%d,%d), b(%d,%d,%d), \
            a(%d,%d,%d), bpp:%d!\n",
            var->red.offset, var->red.length, var->red.msb_right,
            var->green.offset, var->green.length, var->green.msb_right,
            var->blue.offset, var->blue.length, var->blue.msb_right,
            var->transp.offset, var->transp.length, var->transp.msb_right,
            var->bits_per_pixel);
        return -EINVAL;
    }

    if (!s_stDrvOps.pstCapability[u32LayerId].bColFmt[enFmt])
    {
        HIFB_ERROR("Unsupported PIXEL FORMAT!\n");
        return -EINVAL;
    }

    return HI_SUCCESS;
}
#endif
/*
* Name : hifb_buf_map
* Desc : Memory mapping, which generates virtual addresses based on physical address mappings.
*/
HI_VOID *hifb_buf_map(HI_U64 u64PhyAddr, HI_U32 u32Size)
{
    return cmpi_remap_nocache(u64PhyAddr, u32Size);
}

HI_S32 hifb_buf_ummap(HI_VOID *pViraddr)
{
    cmpi_unmap(pViraddr);

    return HI_SUCCESS;
}

/******************************************************************************
 Function        : hifb_check_var
 Description     : check if the paramter for framebuffer is supported.
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : struct fb_var_screeninfo *var
                   struct fb_info *info
 Return          : return 0, if the paramter is supported, otherwise,return error
                   code.
 Others          : 0
******************************************************************************/
#ifndef __HuaweiLite__
static HI_S32 hifb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_PAR_S *pstPar = NULL;
    HI_U32 u32ExpectedLen = 0;
    HI_U32 u32LayerId = 0;
    HI_U32 u32HTotal;
    HI_U32 u32VTotal;
    if ((info == NULL) || (var == NULL)) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }
    pstPar = (HIFB_PAR_S *)info->par;
    u32LayerId = pstPar->u32LayerID;

    if (IS_CURSOR_LAYER(pstPar->u32LayerID) && IS_SOFT_CURSOR())
    {
        HIFB_ERROR("cursor layer doesn't support this operation!\n");
        return HI_FAILURE;
    }

    if (HI_SUCCESS != hifb_check_fmt(var, info))
    {
        return HI_FAILURE;
    }

    /* For interlaced output, the actual height of the layer must be an even number. Progressive output without this limit */
    if ((0 != (var->yres % 2)) && (hifb_is_interlace(pstPar)))
    {
        HIFB_ERROR("yres(%d) of u32LayerId %d should be even when vodev output is interlace\n", \
            var->yres, u32LayerId);
        return HI_FAILURE;
    }

    /* virtual resolution can't be less than minimal resolution */
    if (var->xres < HIFB_MIN_WIDTH(u32LayerId))
    {
        HIFB_ERROR("xres(%d) of u32LayerId %d can't be less than min_width(%d)\n", \
            var->xres, u32LayerId, HIFB_MIN_WIDTH(u32LayerId));
        return HI_FAILURE;
    }
    if (var->yres < HIFB_MIN_HEIGHT(u32LayerId))
    {
        HIFB_ERROR("yres(%d) of u32LayerId %d can't be less than min_height(%d)\n", \
            var->yres, u32LayerId, HIFB_MIN_HEIGHT(u32LayerId));
        return HI_FAILURE;
    }

    if (var->xres > var->xres_virtual)
    {
        HIFB_ERROR("xres(%d) of u32LayerId %d should be less than xres_virtual(%d)\n", \
            var->xres, u32LayerId, var->xres_virtual);
        return HI_FAILURE;
    }
    if (var->yres > var->yres_virtual)
    {
        HIFB_ERROR("yres(%d) of u32LayerId %d should be less than yres_virtual(%d)\n", \
            var->yres, u32LayerId, var->yres_virtual);
        return HI_FAILURE;
    }

    /*check if the offset is valid*/
    if ((var->xoffset + var->xres > var->xres_virtual)
        || (var->xoffset > var->xres_virtual))
    {
        HIFB_ERROR("the sum of layer%d's xoffset(%d) and xres(%d) should be less than xres_virtual(%d)\n",
            u32LayerId, var->xoffset, var->xres, var->xres_virtual);
        return -EINVAL;
    }

    if ((var->yoffset + var->yres > var->yres_virtual)
        || (var->yoffset > var->yres_virtual))
    {
        HIFB_ERROR("the sum of layer%d's yoffset(%d) and yres(%d) should be less than yres_virtual(%d)\n",
            u32LayerId, var->yoffset, var->yres, var->yres_virtual);
        return -EINVAL;
    }

    /* The FB driver in the Linux kernel will use u32HTotal and u32VTotal as divisors, so they cannot be 0. */
    u32HTotal = var->left_margin + var->xres + var->right_margin + var->hsync_len;
    if (0 == u32HTotal)
    {
        HIFB_ERROR("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d) and hsync_len(%d) can't be 0\n",
            u32LayerId, var->left_margin, var->xres, var->right_margin, var->hsync_len);
        return HI_FAILURE;
    }
    u32VTotal = var->yres + var->lower_margin + var->vsync_len + var->upper_margin;
    if (0 == u32VTotal)
    {
        HIFB_ERROR("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d) and hsync_len(%d) can't be 0\n",
            u32LayerId, var->upper_margin, var->yres, var->lower_margin, var->vsync_len);
        return HI_FAILURE;
    }

    HIFB_INFO("xres:%d, yres:%d, xres_virtual:%d, yres_virtual:%d\n",
        var->xres, var->yres, var->xres_virtual, var->yres_virtual);

    u32ExpectedLen = var->yres_virtual *
        ((((var->xres_virtual * var->bits_per_pixel) >> 3) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT)) ;

    if (info->fix.smem_len && (u32ExpectedLen > info->fix.smem_len))
    {
        HIFB_ERROR("layer %d don't has enough mem! expected: %d KBytes, real:%d KBytes\n",
             u32LayerId, u32ExpectedLen/1024, info->fix.smem_len/1024);
        return -EINVAL;
    }

    return HI_SUCCESS;
}
#endif

static HI_VOID hifb_set_dispbufinfo(HI_U32 u32LayerId)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)(info->par);
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &pstPar->stRefreshInfo;
    HIFB_DISPBUF_INFO_S *      pstDispBufInfo = &pstRefreshInfo->stDispBufInfo;


    /* there's a limit from hardware that screen buf shoule be 16 bytes aligned,maybe it's proper
    to get this info from drv adapter*/
    HI_U32 u32BufSize = ((hifb_get_line_length(info)* hifb_get_yres(info))+HIFB_ALIGNMENT)&(~HIFB_ALIGNMENT);
    if (hifb_get_smem_len(info) == 0)
    {
        return;
    }
    else if ((hifb_get_smem_len(info) >= u32BufSize) && (hifb_get_smem_len(info) < u32BufSize * 2))
    {
        pstDispBufInfo->u64PhyAddr[0] = hifb_get_smem_start(info);
        pstDispBufInfo->u64PhyAddr[1] = hifb_get_smem_start(info);
    }
    else if (hifb_get_smem_len(info) >= u32BufSize * 2)
    {
        pstDispBufInfo->u64PhyAddr[0] = hifb_get_smem_start(info);
        pstDispBufInfo->u64PhyAddr[1] = hifb_get_smem_start(info)+ u32BufSize;
    }
    return;
}

/*
* Name : hifb_get_dcmp_framesize
* Desc : Read two numbers from the virtual address.
*/
static HI_VOID hifb_get_dcmp_framesize(HIFB_BUFFER_S * pstDst,
HI_U32  * pu32AR_size,HI_U32  * pu32GB_size,const HI_VOID* screen_base,HI_U32 offset)
{
    *pu32AR_size=*(HI_U64*)screen_base;
    /* Screen_base cannot be converted to any type except HI_U8* */
    *pu32GB_size=*(HI_U64*)(screen_base+offset);
}
/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static HI_S32 hifb_refresh_1buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &pstPar->stDisplayInfo;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &pstPar->stRefreshInfo;
    HIFB_DISPBUF_INFO_S *      pstDispBufInfo = &pstRefreshInfo->stDispBufInfo;
    volatile
        HIFB_COMPRESS_INFO_S * pstCompressInfo= &pstPar->stCompressInfo;
    HIFB_OSD_DATA_S            stOsdData;
    unsigned long              lockflag;
    HI_S32                     s32Ret;

    HIFB_BLIT_OPT_S            stBlitOpt      = {0};
    HIFB_BUFFER_S              dst_rect;

    HI_U32                     u32BufSize     =  ((hifb_get_line_length(info)* hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    HI_U32                     u32BytesPerPixel = 2;

    hi_tde_export_func *pfnTdeExportFunc = NULL;

    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    osal_memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    stBlitOpt.bCallBack = HI_TRUE;
    // Non-blocking mode
    stBlitOpt.bBlock = HI_FALSE;
    dst_rect.stCanvas.enFmt = pstPar->enColFmt;
    dst_rect.stCanvas.u32Height = pstDisplayInfo->u32DisplayHeight;
    dst_rect.stCanvas.u32Width = pstDisplayInfo->u32DisplayWidth;
    dst_rect.stCanvas.u32Pitch = hifb_get_line_length(info);

    if( HIFB_ROTATE_90  == pstDisplayInfo->enRotateMode||
        HIFB_ROTATE_270 == pstDisplayInfo->enRotateMode)
    {
        dst_rect.stCanvas.u32Height = pstDisplayInfo->u32DisplayWidth;
        dst_rect.stCanvas.u32Width = pstDisplayInfo->u32DisplayHeight;
        if (HIFB_FMT_ARGB1555 == dst_rect.stCanvas.enFmt ||
            HIFB_FMT_ARGB4444 == dst_rect.stCanvas.enFmt)
        {
            u32BytesPerPixel = 2;
        }
        else if(HIFB_FMT_ARGB8888 == dst_rect.stCanvas.enFmt)
        {
            u32BytesPerPixel = 4;
        }
        dst_rect.stCanvas.u32Pitch = ((u32BytesPerPixel*dst_rect.stCanvas.u32Width + HIFB_ALIGN - 1) / HIFB_ALIGN)*HIFB_ALIGN;

        if((HIFB_FMT_ARGB4444 != pstPar->enColFmt)
        && (HIFB_FMT_ARGB1555 != pstPar->enColFmt)
        && (HIFB_FMT_ARGB8888 != pstPar->enColFmt))
        {
            HIFB_ERROR("The rotate mode only support HIFB_FMT_ARGB4444,HIFB_FMT_ARGB1555,HIFB_FMT_ARGB8888 which is %d\n!\n",pstPar->enColFmt);
            return HI_FAILURE;
        }

        pfnTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
        if ((NULL == pfnTdeExportFunc)
            || (NULL == pfnTdeExportFunc->drv_tde_module_begin_job)
            || (NULL == pfnTdeExportFunc->drv_tde_module_rotate)
            || (NULL == pfnTdeExportFunc->drv_tde_module_end_job))
        {
            HIFB_ERROR("can't get TDE export function, it may be TDE module has not been inserted!\n");
            return HI_FAILURE;
        }

        dst_rect.stCanvas.u64PhyAddr = pstPar->u64RotateVb;
    }
    else
    {
        // The target address is selected as the display buf configured for the interrupt, which is indicated by index for interrupt.
        dst_rect.stCanvas.u64PhyAddr = pstDispBufInfo->u64PhyAddr[pstDispBufInfo->u32IndexForInt];

        // If compression is not open, you do not have to configure a compressed address for this refresh.
        if(pstCompressInfo->bCompressOpen)
        {
            // Use half of the video memory instead of another buffer
            dst_rect.stCanvas.u64GBPhyAddr = dst_rect.stCanvas.u64PhyAddr+u32BufSize/2;
        }
    }

    /* 1buf does not support compression, close compression configuration */
    pstRefreshInfo->stDispBufInfo.bCompress = HI_FALSE;

    s_stDrvOps.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);

    if (stOsdData.u64BufferPhyAddr != pstPar->stRefreshInfo.stDispBufInfo.u64PhyAddr[0] &&
        pstDispBufInfo->u64PhyAddr[0])
    {
        HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag );
        pstPar->bModifying = HI_TRUE;
        // Notify the interrupt handler to modify the display address.
        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;

        // The buf address in the display information is configured to the screen display address for refreshing the screen.
        pstRefreshInfo->u64ScreenAddr = pstDispBufInfo->u64PhyAddr[pstDispBufInfo->u32IndexForInt];
        // If compression is not open, you do not have to configure a compressed address for this refresh.
        if(pstCompressInfo->bCompressOpen)
        {

            // Use half of the video memory instead of another buffer
            pstRefreshInfo->u64GBScreenAddr = pstRefreshInfo->u64ScreenAddr+u32BufSize/2;
        }
        pstDispBufInfo->u32Stride = 4*dst_rect.stCanvas.u32Width;

        pstPar->bModifying = HI_FALSE;
        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
    }

    if (pstDisplayInfo->enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = HIFB_LAYER_ANTIFLICKER_NONE;
    }


    if (pstCanvasBuf->stCanvas.u32Height != dst_rect.stCanvas.u32Height
        || pstCanvasBuf->stCanvas.u32Width != dst_rect.stCanvas.u32Width)
    {

        /* Rotate 0 or 180 degrees, zoomed, then global refresh */
        stBlitOpt.bScale = HI_TRUE;

        dst_rect.UpdateRect.x = 0;
        dst_rect.UpdateRect.y = 0;
        dst_rect.UpdateRect.w = dst_rect.stCanvas.u32Width;
        dst_rect.UpdateRect.h = dst_rect.stCanvas.u32Height;
    }
    else
    {
        /* Rotate 0 or 180 degrees, no zoomed, then partial refresh */
        dst_rect.UpdateRect = pstCanvasBuf->UpdateRect;
    }


    stBlitOpt.bRegionDeflicker = HI_TRUE;

    stBlitOpt.pParam = &(pstPar->u32LayerID);

    if(HIFB_ROTATE_180 == pstDisplayInfo->enRotateMode)
    {
        stBlitOpt.enMirrorMode = HIFB_MIRROR_BOTH;
    }
    else
    {
        stBlitOpt.enMirrorMode = pstDisplayInfo->enMirrorMode;
    }


    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    if (pstCompressInfo->bCompressOpen)
    {
        /* This is just updating the refresh area. The refresh flag is first set to FALSE to
        indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.*/
        osal_memcpy((void *)&pstCompressInfo->stUpdateRect, &dst_rect.UpdateRect, sizeof(HIFB_RECT));
        pstCompressInfo->bUpdateFinished = HI_FALSE;
        stBlitOpt.bCompress = HI_TRUE;
    }
    else
    {
        stBlitOpt.bCompress = HI_FALSE;
    }

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    /* The user buf is used as the source by blit, and the user buf is moved to the display buf with
    the target set (with the target showing the buff address) as the target.*/
    s32Ret = HIFB_DRV_Blit(pstCanvasBuf, &dst_rect, &stBlitOpt, HI_TRUE, &pstDispBufInfo->s32RefreshHandle);

    if (s32Ret < 0)
    {
        HIFB_ERROR("blit err s32Ret:0x%x!\n",s32Ret);
        return HI_FAILURE;
    }

    osal_memcpy(&(pstRefreshInfo->stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    return HI_SUCCESS;
}

/* unit rect */
#define HIFB_UNITE_RECT(stDstRect, stSrcRect) do\
{\
    HIFB_RECT stRect;\
    stRect.x = (stDstRect.x < stSrcRect.x)? stDstRect.x : stSrcRect.x;\
    stRect.y = (stDstRect.y < stSrcRect.y)? stDstRect.y : stSrcRect.y;\
    stRect.w = ((stDstRect.x + stDstRect.w) > (stSrcRect.x + stSrcRect.w))? \
        (stDstRect.x + stDstRect.w - stRect.x) : (stSrcRect.x + stSrcRect.w - stRect.x);\
    stRect.h = ((stDstRect.y + stDstRect.h) > (stSrcRect.y + stSrcRect.h))? \
        (stDstRect.y + stDstRect.h - stRect.y) : (stSrcRect.y + stSrcRect.h - stRect.y);\
    osal_memcpy(&stDstRect, &stRect, sizeof(HIFB_RECT));\
}while(0)


/*check these two rectangle cover each other*/
static HI_BOOL hifb_iscontain(HIFB_RECT stParentRect, HIFB_RECT stChildRect)
{
    HIFB_POINT_S stPoint;
    stPoint.s32XPos = stChildRect.x;
    stPoint.s32YPos = stChildRect.y;
    if ((stPoint.s32XPos < stParentRect.x) || (stPoint.s32XPos > (stParentRect.x + stParentRect.w))
        || (stPoint.s32YPos < stParentRect.y) || (stPoint.s32YPos > (stParentRect.y + stParentRect.h)))
    {
        return HI_FALSE;
    }
    stPoint.s32XPos = stChildRect.x + stChildRect.w;
    stPoint.s32YPos = stChildRect.y + stChildRect.h;
    if ((stPoint.s32XPos < stParentRect.x) || (stPoint.s32XPos > (stParentRect.x + stParentRect.w))
        || (stPoint.s32YPos < stParentRect.y) || (stPoint.s32YPos > (stParentRect.y + stParentRect.h)))
    {
        return HI_FALSE;
    }
    return HI_TRUE;
}

#ifdef CONFIG_HIFB_FENCE_SUPPORT
typedef struct tagHifbRefreshWorkQueue_S
{
    HI_U32 u32LayerId;
    HIFB_BUFFER_S *pstCanvasBuf;
    struct work_struct FenceRefreshWork;
}HIFB_REFRESH_WORKQUEUE_S;

static HI_VOID HIFB_REFRESH_Work(struct work_struct *work)
{
    HI_U32 u32LayerId = 0;
    HIFB_REFRESH_WORKQUEUE_S *pstWork = NULL;
    HIFB_BUFFER_S *pstCanvasBuf = NULL;

    pstWork = (HIFB_REFRESH_WORKQUEUE_S*)container_of(work, HIFB_REFRESH_WORKQUEUE_S, FenceRefreshWork);
    if (pstWork == NULL) {
        return;
    }
    u32LayerId = pstWork->u32LayerId;
    pstCanvasBuf = pstWork->pstCanvasBuf;

    hifb_refresh_2buf(u32LayerId, pstCanvasBuf);

    osal_vfree(pstCanvasBuf);
    pstCanvasBuf = NULL;
    osal_vfree(pstWork);
    pstWork = NULL;

    return;
}


static HI_S32 hifb_refresh_2buf_fence(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    struct fb_info *                  info      = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *                      pstPar    = (HIFB_PAR_S *)info->par;
    HIFB_REFRESH_WORKQUEUE_S         *pstWork   = NULL;

    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    pstWork = (HIFB_REFRESH_WORKQUEUE_S*)osal_vmalloc(sizeof(HIFB_REFRESH_WORKQUEUE_S));
    if (NULL == pstWork)
    {
        return -EFAULT;
    }

    pstWork->u32LayerId = u32LayerId;
    pstWork->pstCanvasBuf = osal_vmalloc(sizeof(HIFB_BUFFER_S));
    if (NULL == pstWork->pstCanvasBuf)
    {
        osal_vfree(pstWork);
        pstWork = NULL;
        return HI_FAILURE;
    }
    osal_memcpy(pstWork->pstCanvasBuf, pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    INIT_WORK(&(pstWork->FenceRefreshWork), HIFB_REFRESH_Work);
    if (NULL == pstPar->pFenceRefreshWorkqueue)
    {
        osal_vfree(pstWork->pstCanvasBuf);
        pstWork->pstCanvasBuf = NULL;
        osal_vfree(pstWork);
        pstWork = NULL;
        return HI_FAILURE;
    }
    queue_work(pstPar->pFenceRefreshWorkqueue, &(pstWork->FenceRefreshWork));
    return HI_SUCCESS;
}
#endif

/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static HI_S32 hifb_refresh_2buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    struct fb_info *           info             = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar           = (HIFB_PAR_S *)info->par;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo   = &pstPar->stRefreshInfo;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo   = &pstPar->stDisplayInfo;
    volatile
        HIFB_COMPRESS_INFO_S * pstCompressInfo  = &pstPar->stCompressInfo;
    HIFB_BLIT_OPT_S            stBlitOpt        = {0};
    HIFB_BUFFER_S              stForeBuf = {0};
    HIFB_BUFFER_S              stBackBuf = {0};
    HI_S32                     s32Ret;
    HIFB_RECT                  NewUnionRect     = {0};
    unsigned long              lockflag;
    HI_U64                     u64OsdBufferPhyAddr;
    HI_U64                     u64WorkBufAddr = 0;
    HI_U32                     u32BufSize       =  ((hifb_get_line_length(info) * hifb_get_yres(info))+HIFB_ALIGNMENT)&(~HIFB_ALIGNMENT);
    HI_U32                     u32BytesPerPixel = 2;
    hi_tde_export_func *pfnTdeExportFunc = NULL;

    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    /* Refresh task submitted between VO vertical timing interrupt and frame start interrupt
    Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
    Blocked here, it is forbidden to submit the refresh task during this time.*/
    s32Ret = wait_event_interruptible_timeout(pstPar->doRefreshJob, pstPar->stRefreshInfo.bDoRefreshJob, msecs_to_jiffies(40));
    if (s32Ret < 0) {
        HIFB_ERROR("wait interrupt!\n");
    }
    else if (s32Ret == 0) {
        HIFB_ERROR("wait timeout!\n");
    }
    osal_memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    osal_memset(&stForeBuf, 0, sizeof(HIFB_BUFFER_S));

    stBlitOpt.bCallBack = HI_TRUE;
    stBlitOpt.pParam = &(pstPar->u32LayerID);

    pstRefreshInfo->u32RefreshNum++;

    s_stDrvOps.HIFB_DRV_GetLayerAddr(u32LayerId, &u64OsdBufferPhyAddr);

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    pstRefreshInfo->stDispBufInfo.bNeedFlip = HI_FALSE;
    pstRefreshInfo->stDispBufInfo.s32RefreshHandle = 0;

    stBackBuf.stCanvas.enFmt = pstPar->enColFmt;
    stBackBuf.stCanvas.u32Height = pstDisplayInfo->u32DisplayHeight;
    stBackBuf.stCanvas.u32Width = pstDisplayInfo->u32DisplayWidth;
    stBackBuf.stCanvas.u32Pitch = hifb_get_line_length(info);

    if(HIFB_ROTATE_90  == pstDisplayInfo->enRotateMode ||
       HIFB_ROTATE_270 == pstDisplayInfo->enRotateMode)
    {
        stBackBuf.stCanvas.u32Width = pstDisplayInfo->u32DisplayHeight;
        stBackBuf.stCanvas.u32Height = pstDisplayInfo->u32DisplayWidth;
        if (HIFB_FMT_ARGB1555 == stBackBuf.stCanvas.enFmt ||
            HIFB_FMT_ARGB4444 == stBackBuf.stCanvas.enFmt)
        {
            u32BytesPerPixel = 2;
        }
        else if(HIFB_FMT_ARGB8888 == stBackBuf.stCanvas.enFmt)
        {
            u32BytesPerPixel = 4;
        }
        stBackBuf.stCanvas.u32Pitch = ((u32BytesPerPixel*stBackBuf.stCanvas.u32Width + HIFB_ALIGN - 1)/HIFB_ALIGN)*HIFB_ALIGN;

        if((HIFB_FMT_ARGB4444 != pstPar->enColFmt)
        && (HIFB_FMT_ARGB1555 != pstPar->enColFmt)
        && (HIFB_FMT_ARGB8888 != pstPar->enColFmt))
        {
            HIFB_ERROR("The rotate mode only support HIFB_FMT_ARGB4444,HIFB_FMT_ARGB1555,HIFB_FMT_ARGB8888 which is %d\n!\n",pstPar->enColFmt);
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            return HI_FAILURE;
        }

        pfnTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
        if ((NULL == pfnTdeExportFunc)
            || (NULL == pfnTdeExportFunc->drv_tde_module_begin_job)
            || (NULL == pfnTdeExportFunc->drv_tde_module_rotate)
            || (NULL == pfnTdeExportFunc->drv_tde_module_end_job))
        {
            HIFB_ERROR("can't get TDE export function, it may be TDE module has not been inserted!\n");
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            return HI_FAILURE;
        }

        stBackBuf.stCanvas.u64PhyAddr = pstPar->u64RotateVb;
    }
    else
    {
        // Set the background buf as the target, get the free buf to the background buf
        hifb_get_idledispbuf(pstPar, &stBackBuf.stCanvas.u64PhyAddr);

        if(pstCompressInfo->bCompressOpen)
        {
            /* Use half of the video memory instead of another buffer. Currently only 59A on the himpp platform needs
            to be configured with this address. Other chips do not need to be configured.*/
            stBackBuf.stCanvas.u64GBPhyAddr = stBackBuf.stCanvas.u64PhyAddr+u32BufSize / 2;
        }
    }

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);


    /* according to the hw arithemetic, calculate  source and Dst fresh rectangle */
    if (pstCanvasBuf->stCanvas.u32Height != stBackBuf.stCanvas.u32Height
        || pstCanvasBuf->stCanvas.u32Width != stBackBuf.stCanvas.u32Width)
    {
        hi_tde_rect SrcRect ={0}, DstRect={0}, InSrcRect ={0}, InDstRect ={0};

        SrcRect.width = pstCanvasBuf->stCanvas.u32Width;
        SrcRect.height = pstCanvasBuf->stCanvas.u32Height;
        DstRect.width = stBackBuf.stCanvas.u32Width;
        DstRect.height = stBackBuf.stCanvas.u32Height;
        InSrcRect.pos_x = pstCanvasBuf->UpdateRect.x;
        InSrcRect.pos_y = pstCanvasBuf->UpdateRect.y;
        InSrcRect.width= (HI_U32)pstCanvasBuf->UpdateRect.w;
        InSrcRect.height= (HI_U32)pstCanvasBuf->UpdateRect.h;
        TdeCalScaleRect_HIFB(&SrcRect, &DstRect, &InSrcRect, &InDstRect);

        NewUnionRect.x = InDstRect.pos_x;
        NewUnionRect.y = InDstRect.pos_y;
        NewUnionRect.w = (HI_S32)InDstRect.width;
        NewUnionRect.h = (HI_S32)InDstRect.height;
        stBlitOpt.bScale = HI_TRUE;
    }
    else
    {
        NewUnionRect = pstCanvasBuf->UpdateRect;
    }

    /*We should check is address changed, for make sure that the address configed to the hw reigster is in effect*/
    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);

    hifb_get_workdispbuf(pstPar, &u64WorkBufAddr);

    if (pstRefreshInfo->stDispBufInfo.bFliped
        && (u64OsdBufferPhyAddr == u64WorkBufAddr ||
        pstPar->stCompressInfo.bCompressOpen))
    {
        HIFB_BLIT_OPT_S tmp = {0};
        // Background as a target pointing to an idle buf
        osal_memcpy(&stForeBuf, &stBackBuf, sizeof(HIFB_BUFFER_S));

        // Foreground as a source points to the buf at work
        hifb_get_workdispbuf(pstPar, &stForeBuf.stCanvas.u64PhyAddr);

        // The union rect is also used as an update area for the foreground and background.
        osal_memcpy(&stForeBuf.UpdateRect, &pstRefreshInfo->stDispBufInfo.stUnionRect, sizeof(HIFB_RECT));
        osal_memcpy(&stBackBuf.UpdateRect, &stForeBuf.UpdateRect , sizeof(HIFB_RECT));

        osal_memset(&tmp, 0x0, sizeof(HIFB_BLIT_OPT_S));
        /*blit with union rect*/
        if (HIFB_ROTATE_90  != pstDisplayInfo->enRotateMode &&
            HIFB_ROTATE_270 != pstDisplayInfo->enRotateMode)
        {
            if (!hifb_iscontain(NewUnionRect, pstRefreshInfo->stDispBufInfo.stUnionRect))
            {
                s32Ret = HIFB_DRV_Blit(&stForeBuf, &stBackBuf, &tmp, HI_TRUE, NULL);
                if (s32Ret < 0)
                {
                    HIFB_ERROR("blit err!\n");
                    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
                    return HI_FAILURE;
                }
            }
        }
        else
        {

            HIFB_ROTATE_OPT_S stRotTmp = {0};
            if(HIFB_ROTATE_90  == pstDisplayInfo->enRotateMode)
            {
                stRotTmp.enRotateMode = HIFB_ROTATE_270;
            }
            else
            {
                stRotTmp.enRotateMode = HIFB_ROTATE_90;
            }
            stForeBuf.stCanvas.u32Width=pstDisplayInfo->u32DisplayWidth;
            stForeBuf.stCanvas.u32Height=pstDisplayInfo->u32DisplayHeight;
            stForeBuf.stCanvas.u32Pitch=((u32BytesPerPixel * stForeBuf.stCanvas.u32Width+(HIFB_ALIGN - 1))/HIFB_ALIGN)*HIFB_ALIGN;
            stForeBuf.UpdateRect.x = 0;
            stForeBuf.UpdateRect.y = 0;
            stForeBuf.UpdateRect.w = stForeBuf.stCanvas.u32Width;
            stForeBuf.UpdateRect.h = stForeBuf.stCanvas.u32Height;


            stBackBuf.UpdateRect.x = 0;
            stBackBuf.UpdateRect.y = 0;
            stBackBuf.UpdateRect.w = stBackBuf.stCanvas.u32Width;
            stBackBuf.UpdateRect.h = stBackBuf.stCanvas.u32Height;
            s32Ret = HIFB_DRV_Rotate(&stForeBuf, &stBackBuf, &stRotTmp, HI_TRUE);
            if (s32Ret <= 0)
            {
                HIFB_ERROR("blit err!\n");
                HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
                return HI_FAILURE;
            }
        }

        /*clear union rect*/
        osal_memset(&(pstRefreshInfo->stDispBufInfo.stUnionRect), 0, sizeof(HIFB_RECT));

        pstRefreshInfo->stDispBufInfo.bFliped = HI_FALSE;
    }
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    /* update union rect */
    if ((pstRefreshInfo->stDispBufInfo.stUnionRect.w == 0)
        || (pstRefreshInfo->stDispBufInfo.stUnionRect.h == 0))
    {
        osal_memcpy(&pstRefreshInfo->stDispBufInfo.stUnionRect, &NewUnionRect, sizeof(HIFB_RECT));
    }
    else
    {
        // new union rect
        HIFB_UNITE_RECT(pstRefreshInfo->stDispBufInfo.stUnionRect, NewUnionRect);
    }

    if (pstDisplayInfo->enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = HIFB_LAYER_ANTIFLICKER_NONE;
    }

    if (stBlitOpt.bScale == HI_TRUE)
    {
        /*actual area, calculate by TDE, here is just use for let pass the test */
        stBackBuf.UpdateRect.x = 0;
        stBackBuf.UpdateRect.y = 0;
        stBackBuf.UpdateRect.w = stBackBuf.stCanvas.u32Width;
        stBackBuf.UpdateRect.h = stBackBuf.stCanvas.u32Height;
    }
    else
    {
        stBackBuf.UpdateRect = pstCanvasBuf->UpdateRect;
    }

    if(HIFB_ROTATE_90 == pstPar->stDisplayInfo.enRotateMode
        || HIFB_ROTATE_270 == pstPar->stDisplayInfo.enRotateMode)
    {
        stBackBuf.stCanvas.u32Height = pstPar->stDisplayInfo.u32DisplayWidth;
        stBackBuf.stCanvas.u32Width = pstPar->stDisplayInfo.u32DisplayHeight;
        stBackBuf.stCanvas.u32Pitch = ((u32BytesPerPixel*stBackBuf.stCanvas.u32Width+ (HIFB_ALIGN - 1))/HIFB_ALIGN)*HIFB_ALIGN;

        stBackBuf.UpdateRect = pstCanvasBuf->UpdateRect;
    }

    stBlitOpt.bRegionDeflicker = HI_TRUE;

    if(HIFB_ROTATE_180 == pstDisplayInfo->enRotateMode)
    {
        stBlitOpt.enMirrorMode = HIFB_MIRROR_BOTH;
    }
    else
    {
        stBlitOpt.enMirrorMode = pstDisplayInfo->enMirrorMode;
    }

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    if (pstPar->stCompressInfo.bCompressOpen)
    {
        /* This is just updating the refresh area. The refresh flag is first set to FALSE
        to indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback. */
        osal_memcpy((void *)&pstPar->stCompressInfo.stUpdateRect, &stBackBuf.UpdateRect, sizeof(HIFB_RECT));
        pstCompressInfo->bUpdateFinished = HI_FALSE;
        stBlitOpt.bCompress = HI_TRUE;
    }
    else
    {
        stBlitOpt.bCompress = HI_FALSE;
    }

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    /* blit with refresh rect */
    s32Ret = HIFB_DRV_Blit(pstCanvasBuf, &stBackBuf, &stBlitOpt,
	                       HI_TRUE, &pstRefreshInfo->stDispBufInfo.s32RefreshHandle);
    if (s32Ret < 0)
    {
        HIFB_ERROR("blit err:0x%x!\n",s32Ret);
        return HI_FAILURE;
    }


    osal_memcpy(&(pstRefreshInfo->stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    return HI_SUCCESS;
}

static HI_S32 hifb_wait_regconfig_work(HI_U32 u32LayerId)
{
    HI_S32 s32Ret;
    HIFB_PAR_S *pstPar = NULL;

    if (u32LayerId >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    pstPar = (HIFB_PAR_S *)g_stLayer[u32LayerId].pstInfo->par;

    pstPar->vblflag = 0;
    /*Assuming TDE is fast enough, 40ms*/
    s32Ret = wait_event_interruptible_timeout(pstPar->vblEvent, pstPar->vblflag, msecs_to_jiffies(40));
    if (s32Ret < 0)
    {
        HIFB_ERROR("Wait vblank failed!");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/*In this function we should wait the new contain has been show on the screen before return,
and the operations such as address configuration no needed do in interrupt handle*/
static HI_S32 hifb_refresh_2buf_immediate_display(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    struct fb_info *           info            = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar          = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo  = &pstPar->stDisplayInfo;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo  = &pstPar->stRefreshInfo;
    volatile
        HIFB_COMPRESS_INFO_S * pstCompressInfo = &pstPar->stCompressInfo;
    HIFB_BLIT_OPT_S            stBlitOpt       = {0};
    HI_U32                     u32Index        = pstRefreshInfo->stDispBufInfo.u32IndexForInt;
    HIFB_BUFFER_S              stForeBuf = {0};
    HIFB_BUFFER_S              stBackBuf = {0};
    HI_S32                     s32Ret;
    HIFB_RECT                  NewUnionRect    = {0};
    HIFB_BLIT_OPT_S*           ptmp             = NULL;
    unsigned long              lockflag;
    HI_U32                     u32AR_size;
    HI_U32                     u32GB_size;
    HI_U32                     u32BufSize       =  ((hifb_get_line_length(info) * hifb_get_yres(info))+HIFB_ALIGNMENT)&(~HIFB_ALIGNMENT);
    HI_U32                     u32BytesPerPixel = 2;
    hi_tde_export_func *pfnTdeExportFunc = NULL;
    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    /* Refresh task submitted between VO vertical timing interrupt and frame start interrupt
    Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
    Blocked here, it is forbidden to submit the refresh task during this time.*/
    s32Ret = wait_event_interruptible_timeout(pstPar->doRefreshJob, pstPar->stRefreshInfo.bDoRefreshJob, msecs_to_jiffies(40));
    if (s32Ret < 0)
    {
        osal_printk("Func:%s, Line:%d, wait.\n", __FUNCTION__, __LINE__);
    }

    osal_memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));

    /*TDE use the blocking mode*/
    /* Immediate refresh mode requires blocking mode, non-callback mode, avoiding callback mode when scrolling subtitles.
       Return immediately. Maybe the next time the user quickly call the interface refresh, it may flush the last content. */
    stBlitOpt.bCallBack = HI_TRUE;
    stBlitOpt.bBlock = HI_TRUE;
    stBlitOpt.pParam = &(pstPar->u32LayerID);

    pstRefreshInfo->u32RefreshNum++;

    /*forbid changing display buffer in interrupt handle*/
    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    pstRefreshInfo->stDispBufInfo.bFliped = HI_FALSE;
    pstRefreshInfo->stDispBufInfo.bNeedFlip = HI_FALSE;
    pstRefreshInfo->stDispBufInfo.s32RefreshHandle = 0;

    stBackBuf.stCanvas.enFmt = pstPar->enColFmt;
    stBackBuf.stCanvas.u32Height = pstDisplayInfo->u32DisplayHeight;
    stBackBuf.stCanvas.u32Width = pstDisplayInfo->u32DisplayWidth;
    stBackBuf.stCanvas.u32Pitch = hifb_get_line_length(info);
    if(HIFB_ROTATE_90  == pstDisplayInfo->enRotateMode ||
       HIFB_ROTATE_270 == pstDisplayInfo->enRotateMode)
    {

        stBackBuf.stCanvas.u32Width = pstDisplayInfo->u32DisplayHeight;
        stBackBuf.stCanvas.u32Height = pstDisplayInfo->u32DisplayWidth;
        if (HIFB_FMT_ARGB1555 == stBackBuf.stCanvas.enFmt ||
            HIFB_FMT_ARGB4444 == stBackBuf.stCanvas.enFmt)
        {
            u32BytesPerPixel = 2;
        }
        else if(HIFB_FMT_ARGB8888 == stBackBuf.stCanvas.enFmt)
        {
            u32BytesPerPixel = 4;
        }
        stBackBuf.stCanvas.u32Pitch = ((u32BytesPerPixel*stBackBuf.stCanvas.u32Width + HIFB_ALIGN - 1) / HIFB_ALIGN) * HIFB_ALIGN;
        if( (HIFB_FMT_ARGB4444 != pstPar->enColFmt) &&
            (HIFB_FMT_ARGB1555 != pstPar->enColFmt) &&
            (HIFB_FMT_ARGB8888 != pstPar->enColFmt))
        {
            HIFB_ERROR("The rotate mode only support HIFB_FMT_ARGB4444 and HIFB_FMT_ARGB1555 which is %d\n!\n",pstPar->enColFmt);
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            return HI_FAILURE;
        }

        pfnTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
        if ((NULL == pfnTdeExportFunc)                ||
            (NULL == pfnTdeExportFunc->drv_tde_module_begin_job)||
            (NULL == pfnTdeExportFunc->drv_tde_module_rotate)  ||
            (NULL == pfnTdeExportFunc->drv_tde_module_end_job))
        {
            HIFB_ERROR("can't get TDE export function, it may be TDE module has not been inserted!\n");
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            return HI_FAILURE;
        }

        stBackBuf.stCanvas.u64PhyAddr = pstPar->u64RotateVb;
    }
    else
    {
        stBackBuf.stCanvas.u64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[1-u32Index];
        if(pstCompressInfo->bCompressOpen)
        {
            stBackBuf.stCanvas.u64GBPhyAddr = stBackBuf.stCanvas.u64PhyAddr+u32BufSize/2;
        }
    }

    /* according to the hw arithemetic, calculate  source and Dst fresh rectangle */
    if ((pstCanvasBuf->stCanvas.u32Height != stBackBuf.stCanvas.u32Height)
        || (pstCanvasBuf->stCanvas.u32Width != stBackBuf.stCanvas.u32Width))
    {
        hi_tde_rect SrcRect ={0}, DstRect={0}, InSrcRect ={0}, InDstRect ={0};

        SrcRect.width = pstCanvasBuf->stCanvas.u32Width;
        SrcRect.height = pstCanvasBuf->stCanvas.u32Height;
        DstRect.width = stBackBuf.stCanvas.u32Width;
        DstRect.height = stBackBuf.stCanvas.u32Height;
        InSrcRect.pos_x = pstCanvasBuf->UpdateRect.x;
        InSrcRect.pos_y = pstCanvasBuf->UpdateRect.y;
        InSrcRect.width= (HI_U32)pstCanvasBuf->UpdateRect.w;
        InSrcRect.height= (HI_U32)pstCanvasBuf->UpdateRect.h;
        TdeCalScaleRect_HIFB(&SrcRect, &DstRect, &InSrcRect, &InDstRect);

        NewUnionRect.x = InDstRect.pos_x;
        NewUnionRect.y = InDstRect.pos_y;
        NewUnionRect.w = (HI_S32)InDstRect.width;
        NewUnionRect.h = (HI_S32)InDstRect.height;
        stBlitOpt.bScale = HI_TRUE;
    }
    else
    {
        NewUnionRect = pstCanvasBuf->UpdateRect;
    }

    if (HIFB_ROTATE_90  != pstDisplayInfo->enRotateMode &&
        HIFB_ROTATE_270 != pstDisplayInfo->enRotateMode)
    {
        /* because reverse, the 2 buffer needed to sync contain, if the fresh area has cover last fresh area, then no need to sync*/
        if (!hifb_iscontain(NewUnionRect, pstRefreshInfo->stDispBufInfo.stUnionRect)
            && pstRefreshInfo->stDispBufInfo.stUnionRect.w && pstRefreshInfo->stDispBufInfo.stUnionRect.h)
        {
            osal_memcpy(&stForeBuf, &stBackBuf, sizeof(HIFB_BUFFER_S));
            stForeBuf.stCanvas.u64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index];
            osal_memcpy(&stForeBuf.UpdateRect, &pstRefreshInfo->stDispBufInfo.stUnionRect, sizeof(HIFB_RECT));
            osal_memcpy(&stBackBuf.UpdateRect, &stForeBuf.UpdateRect , sizeof(HIFB_RECT));

            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

            ptmp = osal_vmalloc(sizeof(HIFB_BLIT_OPT_S));

            if (ptmp == HI_NULL)
            {
                HIFB_ERROR("NO mem!\n");
                return HI_FAILURE;
            }

            osal_memset(ptmp, 0x0, sizeof(HIFB_BLIT_OPT_S));

            s32Ret = HIFB_DRV_Blit(&stForeBuf, &stBackBuf, ptmp, HI_TRUE, NULL);

            osal_vfree(ptmp);
            ptmp = NULL;
            if (s32Ret < 0)
            {
                HIFB_ERROR("blit err 4!\n");
                return HI_FAILURE;
            }
            HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
        }
    }
    else
    {
        HIFB_ROTATE_OPT_S stRotTmp = {0};

        osal_memcpy(&stForeBuf, &stBackBuf, sizeof(HIFB_BUFFER_S));
        stForeBuf.stCanvas.u64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index];
        osal_memcpy(&stForeBuf.UpdateRect, &pstRefreshInfo->stDispBufInfo.stUnionRect, sizeof(HIFB_RECT));
        osal_memcpy(&stBackBuf.UpdateRect, &stForeBuf.UpdateRect , sizeof(HIFB_RECT));

        if(HIFB_ROTATE_90  == pstDisplayInfo->enRotateMode)
        {
            stRotTmp.enRotateMode = HIFB_ROTATE_270;
        }
        else
        {
            stRotTmp.enRotateMode = HIFB_ROTATE_90;
        }
        stForeBuf.stCanvas.u32Width=pstDisplayInfo->u32DisplayWidth;
        stForeBuf.stCanvas.u32Height=pstDisplayInfo->u32DisplayHeight;
        stForeBuf.stCanvas.u32Pitch= ((u32BytesPerPixel * stForeBuf.stCanvas.u32Width+HIFB_ALIGN - 1)/HIFB_ALIGN)*HIFB_ALIGN;
        stForeBuf.UpdateRect.x = 0;
        stForeBuf.UpdateRect.y = 0;
        stForeBuf.UpdateRect.w = stForeBuf.stCanvas.u32Width;
        stForeBuf.UpdateRect.h = stForeBuf.stCanvas.u32Height;


        stBackBuf.UpdateRect.x = 0;
        stBackBuf.UpdateRect.y = 0;
        stBackBuf.UpdateRect.w = stBackBuf.stCanvas.u32Width;
        stBackBuf.UpdateRect.h = stBackBuf.stCanvas.u32Height;
        s32Ret = HIFB_DRV_Rotate(&stForeBuf, &stBackBuf, &stRotTmp, HI_TRUE);
        if (s32Ret <= 0)
        {
            HIFB_ERROR("blit err!\n");
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            return HI_FAILURE;
        }
    }

    /*record the fresh area */
    osal_memcpy(&pstRefreshInfo->stDispBufInfo.stUnionRect, &NewUnionRect, sizeof(HIFB_RECT));

    if (pstDisplayInfo->enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = HIFB_LAYER_ANTIFLICKER_NONE;
    }

    if (stBlitOpt.bScale == HI_TRUE)
    {
        /*actual ares is calculate by TDE, here just let is pass the test */
        stBackBuf.UpdateRect.x = 0;
        stBackBuf.UpdateRect.y = 0;
        stBackBuf.UpdateRect.w = stBackBuf.stCanvas.u32Width;
        stBackBuf.UpdateRect.h = stBackBuf.stCanvas.u32Height;
    }
    else
    {
        stBackBuf.UpdateRect = pstCanvasBuf->UpdateRect;
    }

    stBlitOpt.bRegionDeflicker = HI_TRUE;

    if(HIFB_ROTATE_180 == pstDisplayInfo->enRotateMode)
    {
        stBlitOpt.enMirrorMode = HIFB_MIRROR_BOTH;
    }
    else
    {
        stBlitOpt.enMirrorMode = pstDisplayInfo->enMirrorMode;
    }

    if (pstPar->stCompressInfo.bCompressOpen)
    {
        /* This is just updating the refresh area. The refresh flag is first set to FALSE to
        indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback. */
        osal_memcpy((void *)&pstPar->stCompressInfo.stUpdateRect, &stBackBuf.UpdateRect, sizeof(HIFB_RECT));
        pstCompressInfo->bUpdateFinished = HI_FALSE;
        stBlitOpt.bCompress = HI_TRUE;
    }


    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    /* blit with refresh rect */
    s32Ret = HIFB_DRV_Blit(pstCanvasBuf, &stBackBuf, &stBlitOpt,
	                       HI_TRUE, &pstRefreshInfo->stDispBufInfo.s32RefreshHandle);
    if (s32Ret < 0)
    {
        HIFB_ERROR("blit err!5\n");
        return HI_FAILURE;
    }

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    /*set the backup buffer to register and show it  */
    u32Index = 1 - u32Index;
    pstRefreshInfo->stDispBufInfo.u32IndexForInt = u32Index;
    pstPar->bModifying = HI_TRUE;
    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    pstRefreshInfo->u64ScreenAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index];
    pstRefreshInfo->u64GBScreenAddr = pstRefreshInfo->u64ScreenAddr+u32BufSize/2;
    pstRefreshInfo->stDispBufInfo.bCompress = pstCompressInfo->bCompressOpen;
    pstPar->bModifying = HI_FALSE;

    if (pstPar->stCompressInfo.bCompressOpen)
    {
        pstPar->stCompressInfo.bUpdateFinished = HI_TRUE;
        osal_memcpy((void*)&pstPar->stCompressInfo.stUpdateRect, &NewUnionRect, sizeof(HIFB_RECT));
        // When blocking, if there is a callback, still get FrameSize0 and FrameSize1 here as early as possible.
        if((stBlitOpt.bBlock == HI_TRUE && stBlitOpt.bCallBack == HI_FALSE)
            || (stBlitOpt.bBlock == HI_TRUE && stBlitOpt.bCallBack == HI_TRUE))
        {
            hifb_get_dcmp_framesize(&stBackBuf, &u32AR_size,&u32GB_size,(HI_VOID*)((HI_U8 *)(hifb_get_screen_base(info)) +u32Index * u32BufSize) ,u32BufSize/2);

            pstCompressInfo->u32Frame_Size0 =  u32AR_size;
            pstCompressInfo->u32Frame_Size1 =  u32GB_size;
        }
        else if((stBlitOpt.bBlock == HI_FALSE && stBlitOpt.bCallBack == HI_TRUE))
        {
            // get u32AR_size and u32GB_size in callback function
        }
        else  // stBlitOpt.bBlock == HI_FALSE && stBlitOpt.bCallBack == HI_FALSE
        {
            hifb_get_dcmp_framesize(&stBackBuf, &u32AR_size,&u32GB_size,(HI_VOID*)((HI_U8 *)(hifb_get_screen_base(info)) + u32Index*u32BufSize),u32BufSize/2);
            pstCompressInfo->u32Frame_Size0 = u32AR_size;
            pstCompressInfo->u32Frame_Size1 = u32GB_size;
            HIFB_ERROR("error:tde blit(do compressing) bBlock=%d,bCallBack=%d,FrameSize0 = %d,FrameSize1 = %d\n",
                stBlitOpt.bBlock, stBlitOpt.bCallBack, pstCompressInfo->u32Frame_Size0, pstCompressInfo->u32Frame_Size1);
        }
    }
    osal_memcpy(&(pstRefreshInfo->stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));
    pstPar->vblflag = 0;
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    /*wait the address register's configuration take effect before return*/
    if(!in_atomic())
    {
        hifb_wait_regconfig_work(u32LayerId);
    }

    return HI_SUCCESS;
}

static HI_VOID hifb_refresh_again(HI_U32 u32LayerId)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &pstPar->stRefreshInfo;
    HIFB_BUFFER_S              stCanvas;

    // Prerequisites for the canvas to be refreshed
    if (!(pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_INRECT))
    {
        return;
    }

    if (pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr == 0)
    {
        return;
    }

    if (HIFB_LAYER_BUF_NONE == pstRefreshInfo->enBufMode)
    {
        return ;
    }
    // Fills the canvas object with refresh information from private data for refresh.
    stCanvas = pstRefreshInfo->stUserBuffer;
    stCanvas.UpdateRect.x = 0;
    stCanvas.UpdateRect.y = 0;
    stCanvas.UpdateRect.w = stCanvas.stCanvas.u32Width;
    stCanvas.UpdateRect.h = stCanvas.stCanvas.u32Height;
#ifdef CONFIG_HIFB_FENCE_SUPPORT
    hifb_refresh(u32LayerId, &stCanvas, HIFB_LAYER_BUF_DOUBLE);
#else
    hifb_refresh(u32LayerId, &stCanvas, pstRefreshInfo->enBufMode);
#endif

}


/* This function has a lock inside, and there is a call to the sleep function.
Before calling this function, you must first unlock the spin lock. */
static HI_S32 hifb_disp_setdispsize(HI_U32 u32LayerId, HI_U32 u32Width, HI_U32 u32Height)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
#ifndef __HuaweiLite__
    struct fb_var_screeninfo * pvar           = &info->var;
    struct fb_fix_screeninfo * pfix           = &info->fix;
#endif
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &pstPar->stDisplayInfo;
    HI_U32                     u32Pitch;
    HIFB_SPIN_LOCK_FLAG        lockflag;

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock,lockflag);
    if ((pstDisplayInfo->u32DisplayWidth == u32Width) && (pstDisplayInfo->u32DisplayHeight == u32Height))
    {
        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
        return HI_SUCCESS;
    }

    if (!s_stDrvOps.pstCapability[pstPar->u32LayerID].bVoScale)
    {
        HIFB_SIZE_S stMaxScreenSize = {0};
        hifb_get_maxscreensize(pstPar, &stMaxScreenSize.u32Width, &stMaxScreenSize.u32Height);

        if ((u32Width > stMaxScreenSize.u32Width) || (u32Height > stMaxScreenSize.u32Height)
            || (u32Width > s_stDrvOps.pstCapability[pstPar->u32LayerID].u32MaxWidth)
            || (u32Height > s_stDrvOps.pstCapability[pstPar->u32LayerID].u32MaxHeight))
        {
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
            HIFB_ERROR("the resolution(%u, %u) is larger than the max size of the screen(%u, %u) or the layer(%u, %u)!\n",
            u32Width, u32Height, stMaxScreenSize.u32Width, stMaxScreenSize.u32Height,
            s_stDrvOps.pstCapability[pstPar->u32LayerID].u32MaxWidth, s_stDrvOps.pstCapability[pstPar->u32LayerID].u32MaxHeight);
            return HI_FAILURE;
        }
    }

    u32Pitch = ((u32Width * hifb_get_bits_per_pixel(info)>> 3) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    if(HI_FAILURE == hifb_checkmem_enough(info, u32Pitch, u32Height))
    {
        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
        HIFB_ERROR("++ setdispsize .memory is not enough!\n");
        return HI_FAILURE;
    }

    pstDisplayInfo->u32DisplayWidth = u32Width;
    pstDisplayInfo->u32DisplayHeight = u32Height;
    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_INRECT;

#ifdef __HuaweiLite__
    if (u32Pitch > info->oinfo.stride)
        {
            info->oinfo.stride = u32Pitch;
            info->oinfo.sarea.h = info->vinfo.yres = pstDisplayInfo->u32DisplayHeight;
            info->oinfo.sarea.w = info->vinfo.xres = pstDisplayInfo->u32DisplayWidth;
            pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
        }
#else
    if (u32Pitch > pfix->line_length)
    {
        pfix->line_length = u32Pitch;
        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    }

    /* If the user calls FBIOPUT_LAYER_INFO to set u32DisplayWidth and u32DisplayHeight, then sync to xres yres,
    Otherwise, there will be an error in the memory address in hifb_set_dispbufinfo. */

    pvar->xres = pstDisplayInfo->u32DisplayWidth;
    pvar->yres = pstDisplayInfo->u32DisplayHeight;

    if (pvar->xres_virtual < pstDisplayInfo->u32DisplayWidth)
    {
        pvar->xres_virtual = pstDisplayInfo->u32DisplayWidth;
    }

    if (pvar->yres_virtual < pstDisplayInfo->u32DisplayHeight)
    {
        pvar->yres_virtual = pstDisplayInfo->u32DisplayHeight;
    }

#endif

    hifb_set_dispbufinfo(u32LayerId);

    if (!s_stDrvOps.pstCapability[pstPar->u32LayerID].bVoScale)
    {
        pstDisplayInfo->u32ScreenWidth = u32Width;
        pstDisplayInfo->u32ScreenHeight = u32Height;
        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;
    }

    /* here we need to think about how to resist flicker again, we use VO do flicker resist before , but now if the display H size is the
    same as the screen, VO will not do flicker resist, so should choose TDE to do flicker resist*/
    hifb_select_antiflicker_mode(pstPar);
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);

    return HI_SUCCESS;
}

  /* we handle it by two case:
      case 1 : if VO support Zoom, we only change screen size, display size keep not change
      case 2: if VO can't support zoom, display size should keep the same as screen size*/
static HI_S32 hifb_disp_setscreensize(HI_U32 u32LayerId, HI_U32 u32Width, HI_U32 u32Height)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &pstPar->stDisplayInfo;

    /* If the chip does not support scaling, it is consistent with the process of setting the display area. */
    if (!s_stDrvOps.pstCapability[pstPar->u32LayerID].bVoScale)
    {
        return hifb_disp_setdispsize(u32LayerId, u32Width, u32Height);
    }

    pstDisplayInfo->u32ScreenWidth = u32Width;
    pstDisplayInfo->u32ScreenHeight = u32Height;

    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    /* Here  we need to think about how to resist flicker again, we use VO do flicker resist before , but now if the display H size is the
    same as the screen, VO will not do flicker resist, so should choose TDE to do flicker resist*/
    hifb_select_antiflicker_mode(pstPar);

    return HI_SUCCESS;
}


HI_VOID hifb_buf_freemem(HI_U64 u64Phyaddr)
{
    cmpi_mmz_free(u64Phyaddr, NULL);
}

static HI_S32 hifb_freeccanbuf(HIFB_PAR_S *par)
{
    HIFB_SURFACE_S *           pstCanvasSur   = NULL;
    if (par == NULL) {
        return HI_FAILURE;
    }
    pstCanvasSur   = &par->CanvasSur;

    if (pstCanvasSur->u64PhyAddr != 0)
    {
        hifb_buf_freemem(pstCanvasSur->u64PhyAddr);
    }
    pstCanvasSur->u64PhyAddr = 0;

    if (par->u64RotateVb != 0)
    {
        cmpi_mmz_free(par->u64RotateVb, NULL);
        par->u64RotateVb = 0;
    }

    return HI_SUCCESS;
}


/******************************************************************************
 Function        : hifb_set_par
 Description     : set the variable parmater and make it use
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : struct fb_info *info
 Return          : return 0
 Others          : 0
******************************************************************************/
static HI_S32 hifb_set_par(struct fb_info *info)
{
    HIFB_PAR_S *               pstPar             = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo     = &pstPar->stDisplayInfo;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo     = &pstPar->stRefreshInfo;
    HI_U64                     u64DisplayAddr     = 0;
    HI_U32                     u32Stride;
    HIFB_COLOR_FMT_E           enFmt;
    HI_S32                     s32Ret;
    HIFB_SPIN_LOCK_FLAG        lockflag;

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);

    pstPar->bModifying = HI_TRUE;

    /* set the stride if stride change */
    u32Stride = ((hifb_get_xres_virtual(info)* hifb_get_bits_per_pixel(info) >> 3) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);

    if (u32Stride != hifb_get_line_length(info) ||
       (hifb_get_yres(info) != pstDisplayInfo->u32YRes))
    {
#ifdef __HuaweiLite__
        info->oinfo.stride = u32Stride;
#else
        info->fix.line_length = u32Stride;
#endif
        hifb_set_dispbufinfo(pstPar->u32LayerID);
        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    }

    u64DisplayAddr = (hifb_get_smem_start(info) + u32Stride * hifb_get_yoffset(info)
                   + hifb_get_xoffset(info) * (hifb_get_bits_per_pixel(info) >> 3)) & 0xfffffff0;

    if (u64DisplayAddr != pstRefreshInfo->u64ScreenAddr)
    {
        pstRefreshInfo->u64ScreenAddr = u64DisplayAddr;
        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    }

    /* If xres or yres change */
    if (hifb_get_xres(info)!= pstDisplayInfo->u32XRes
        || hifb_get_yres(info) != pstDisplayInfo->u32YRes)
    {
        if ((hifb_get_xres(info) == 0) || (hifb_get_yres(info) == 0))
        {
            if (pstPar->bShow == HI_TRUE)
            {
                pstPar->bShow = HI_FALSE;
                pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
            }
        }

         /* The following two functions have a sleep operation, you must unlock before calling,
         and lock the global variable inside the function. */
        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);

        s32Ret = hifb_disp_setdispsize(pstPar->u32LayerID, hifb_get_xres(info), hifb_get_yres(info));
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
        s32Ret = hifb_disp_setscreensize(pstPar->u32LayerID, hifb_get_xres(info), hifb_get_yres(info));
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }

        HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    }

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
#ifdef __HuaweiLite__
    enFmt = info->vinfo.fmt;
#else
    enFmt = hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue,
        &info->var.transp, hifb_get_bits_per_pixel(info));
#endif
    if ((pstPar->enColFmt != enFmt))
    {
        hifb_freeccanbuf(pstPar);
        hifb_set_fmt(pstPar, enFmt);
        pstPar->stCursorInfo.stCursor.stCursor.enFmt = enFmt;
        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FMT;
    }

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    pstDisplayInfo->u32XRes = hifb_get_xres(info);
    pstDisplayInfo->u32YRes = hifb_get_yres(info);
    pstDisplayInfo->u32VirXRes = hifb_get_xres_virtual(info);
    pstDisplayInfo->u32VirYRes = hifb_get_yres_virtual(info);

    pstPar->bModifying = HI_FALSE;
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);

    return 0;
}

/******************************************************************************
 Function        : hifb_pan_display
 Description     : pan display.
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : struct fb_var_screeninfo *var
 Return          : return 0
 Others          : 0
******************************************************************************/
#ifdef __HuaweiLite__
static HI_S32 hifb_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo)
#else
static HI_S32 hifb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
#endif
{
#ifdef __HuaweiLite__
        struct hifb_info *info = (struct hifb_info *)vtable;
#endif

    HIFB_PAR_S *               par            = (HIFB_PAR_S *)info->par;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &par->stRefreshInfo;
    HI_U64                     u64DisplayAddr = 0;
    HI_U32                     u32Stride = 0;
    HIFB_BUFFER_S              stCanvasBuf;
    HI_S32                     s32Ret;

    /*set the stride and display start address*/
    u32Stride = (hifb_get_xres_virtual(info)* (hifb_get_bits_per_pixel(info) >> 3) + 0xf) & 0xfffffff0;

#ifdef __HuaweiLite__
    u64DisplayAddr = (hifb_get_smem_start(info) + (HI_U64)u32Stride * oinfo->sarea.y
                + (HI_U64)oinfo->sarea.x * (hifb_get_bits_per_pixel(info) >> 3))&0xfffffffffffffff0;
#else
    u64DisplayAddr = (hifb_get_smem_start(info) + (HI_U64)u32Stride * var->yoffset
                + (HI_U64)var->xoffset * (hifb_get_bits_per_pixel(info) >> 3))&0xfffffffffffffff0;
#endif
    stCanvasBuf.stCanvas.enFmt = par->enColFmt;
    stCanvasBuf.stCanvas.u64PhyAddr = u64DisplayAddr;
    stCanvasBuf.stCanvas.u32Pitch = u32Stride;
    stCanvasBuf.UpdateRect.x = 0;
    stCanvasBuf.UpdateRect.y = 0;
#ifdef __HuaweiLite__
    stCanvasBuf.stCanvas.u32Width = info->vinfo.xres;
    stCanvasBuf.stCanvas.u32Height = info->vinfo.yres;
    stCanvasBuf.UpdateRect.w = info->vinfo.xres;
    stCanvasBuf.UpdateRect.h = info->vinfo.xres;
    g_stLayer[par->u32LayerID].pstInfo->activate = info->activate;
#else
    stCanvasBuf.stCanvas.u32Width = hifb_get_xres(info);
    stCanvasBuf.stCanvas.u32Height = hifb_get_yres(info);
    stCanvasBuf.UpdateRect.w = hifb_get_xres(info);
    stCanvasBuf.UpdateRect.h = hifb_get_yres(info);
    g_stLayer[par->u32LayerID].pstInfo->var.activate = info->var.activate;
#endif
    pstRefreshInfo->enBufMode = HIFB_LAYER_BUF_BUTT;
    s32Ret = hifb_refresh_0buf(par->u32LayerID, &stCanvasBuf);

    return s32Ret;
}

static inline HI_VOID hifb_get_fmt(HIFB_PAR_S *pstPar, HIFB_COLOR_FMT_E *penColorFmt)
{
    *penColorFmt = pstPar->enColFmt;
    return;
}

static inline HI_VOID hifb_set_fmt(HIFB_PAR_S *pstPar, HIFB_COLOR_FMT_E enColorFmt)
{
    pstPar->enColFmt = enColorFmt;

    return;
}

static inline HI_VOID hifb_set_alpha(HIFB_PAR_S *pstPar, HIFB_ALPHA_S *pstAlpha)
{
    osal_memcpy(&pstPar->stAlpha, pstAlpha, sizeof(HIFB_ALPHA_S));

    return;
}

static inline HI_VOID hifb_get_alpha(HIFB_PAR_S *pstPar, HIFB_ALPHA_S *pstAlpha)
{
    osal_memcpy(pstAlpha, &pstPar->stAlpha, sizeof(HIFB_ALPHA_S));

    return;
}

static inline HI_VOID hifb_set_key(HIFB_PAR_S *pstPar, HIFB_COLORKEYEX_S *pstKey)
{
    osal_memcpy(&pstPar->stCkey, pstKey, sizeof(HIFB_COLORKEYEX_S));

    return;
}

static inline HI_VOID hifb_get_key(HIFB_PAR_S *pstPar, HIFB_COLORKEYEX_S *pstKey)
{
    osal_memcpy(pstKey, &pstPar->stCkey, sizeof(HIFB_COLORKEYEX_S));

    return;
}

static inline HI_VOID hifb_set_layerpos(HIFB_PAR_S *pstPar, HIFB_POINT_S *pstPos)
{
    HI_S32                     s32XPos;
    HI_S32                     s32YPos;
    SIZE_S                     stMaxScreensize;
    HI_U32                     u32LayerId     = 0;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = NULL;
    if ((pstPar == NULL) || (pstPos == NULL)) {
        return;
    }
    u32LayerId = pstPar->u32LayerID;
    pstDisplayInfo = &pstPar->stDisplayInfo;

    hifb_get_maxscreensize(pstPar, &stMaxScreensize.u32Width, &stMaxScreensize.u32Height);
    s32XPos = pstPos->s32XPos;
    s32YPos = pstPos->s32YPos;
    if (s32XPos > stMaxScreensize.u32Width - HIFB_MIN_WIDTH(u32LayerId))
    {
        HIFB_WARNING("the sum of s32XPos(%d) and min_width(%d) larger than Vodev screen width(%d)!\n",\
            s32XPos, HIFB_MIN_WIDTH(u32LayerId), stMaxScreensize.u32Width);
        s32XPos = stMaxScreensize.u32Width - HIFB_MIN_WIDTH(u32LayerId);
    }

    if (s32YPos > stMaxScreensize.u32Height - HIFB_MIN_HEIGHT(u32LayerId))
    {
        HIFB_WARNING("the sum of s32YPos(%d) and min_height(%d) larger than Vodev screen height(%d)!\n",\
            s32YPos, HIFB_MIN_HEIGHT(u32LayerId), stMaxScreensize.u32Height);
        s32YPos = stMaxScreensize.u32Height - HIFB_MIN_HEIGHT(u32LayerId);
    }

    pstDisplayInfo->stPos.s32XPos = s32XPos;
    pstDisplayInfo->stPos.s32YPos = s32YPos;

    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    return;
}
#ifdef CURSOR

static inline HI_VOID hifb_set_cursorinfo(HIFB_PAR_S *pstPar, HIFB_CURSOR_INFO_S *pstCursorInfo)
{
    osal_memcpy(&pstPar->stCursorInfo, pstCursorInfo, sizeof(HIFB_CURSOR_INFO_S));

    return;
}

static inline HI_VOID hifb_get_cursorinfo(HIFB_PAR_S *pstPar, HIFB_CURSOR_INFO_S *pstCursorInfo)
{
    osal_memcpy(pstCursorInfo, &pstPar->stCursorInfo, sizeof(HIFB_CURSOR_INFO_S));

    return;
}
#endif
static inline HI_BOOL hifb_get_show(HIFB_PAR_S *pstPar)
{
    if (pstPar != NULL) {
        return pstPar->bShow;
    } else {
        return HI_FALSE;
    }
}

static inline HI_VOID hifb_set_show(HIFB_PAR_S *pstPar, HI_BOOL bShow)
{
    if (pstPar != NULL) {
        pstPar->bShow = bShow;
    }
}

HI_VOID hifb_get_layerinfo(HIFB_PAR_S *pstPar, HIFB_LAYER_INFO_S *pstLayerInfo)
{
    HIFB_POINT_S stPos = {0};
    if ((pstPar != NULL) && (pstLayerInfo != NULL)) {
        hifb_get_premul(pstPar, &pstLayerInfo->bPreMul);
        hifb_get_bufmode(pstPar, &pstLayerInfo->BufMode);
        hifb_get_antiflickerlevel(pstPar, &pstLayerInfo->eAntiflickerLevel);
        hifb_get_layerpos(pstPar, &stPos);
        pstLayerInfo->s32XPos = stPos.s32XPos;
        pstLayerInfo->s32YPos = stPos.s32YPos;
        hifb_get_dispsize(pstPar, &pstLayerInfo->u32DisplayWidth, &pstLayerInfo->u32DisplayHeight);
        hifb_get_screensize(pstPar, &pstLayerInfo->u32ScreenWidth, &pstLayerInfo->u32ScreenHeight);
        pstLayerInfo->u32CanvasWidth = pstPar->CanvasSur.u32Width;
        pstLayerInfo->u32CanvasHeight = pstPar->CanvasSur.u32Height;

        pstLayerInfo->u32Mask = HIFB_LAYERMASK_BUTT;
    }

    return;
}

static inline HI_VOID hifb_get_idledispbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr)
{
    HIFB_REFRESH_INFO_S *      pstRefreshInfo   = NULL;
    HI_U32                     u32IndexForInt   = 0;
    if ((pstPar == NULL) || (pu64PhyAddr == NULL)) {
        return;
    }
    pstRefreshInfo = &pstPar->stRefreshInfo;
    u32IndexForInt = pstRefreshInfo->stDispBufInfo.u32IndexForInt;
    // Only the 2buf refresh mode allows to get free buf, otherwise it is the buf in the current interrupt processing.
#ifdef CONFIG_HIFB_FENCE_SUPPORT
    if (pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_DOUBLE
        || pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE
        || pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_FENCE)
#else
     if (pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_DOUBLE
        || pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
#endif
    {
        *pu64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[1 - u32IndexForInt];
    }
    else
    {
        *pu64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32IndexForInt];
    }

    return;
}

static inline HI_VOID hifb_get_workdispbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr)
{
    HIFB_REFRESH_INFO_S *      pstRefreshInfo   = NULL;
    HI_U32                     u32IndexForInt   = 0;
    if ((pstPar == NULL) || (pu64PhyAddr == NULL)) {
        return;
    }
    pstRefreshInfo = &pstPar->stRefreshInfo;
    u32IndexForInt = pstRefreshInfo->stDispBufInfo.u32IndexForInt;

    *pu64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32IndexForInt];

    return;
}

static inline HI_VOID hifb_get_dispsize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height)
{
    HIFB_DISPLAY_INFO_S *pstDisplayInfo = NULL;
    if (pstPar == NULL) {
        return;
    }
    pstDisplayInfo = &pstPar->stDisplayInfo;

    if ((pu32Width != NULL) && (pu32Height != NULL)) {
        *pu32Width = pstDisplayInfo->u32DisplayWidth;
        *pu32Height = pstDisplayInfo->u32DisplayHeight;
    }

    return;
}
#ifdef CURSOR
static inline HI_VOID hifb_get_idlecursorbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr)
{
    HI_U32 u32IndexForInt = 0;
    if ((pstPar == NULL) || (pu64PhyAddr == NULL)) {
        return;
    }
    u32IndexForInt = pstPar->stRefreshInfo.stDispBufInfo.u32IndexForInt;

#ifdef CONFIG_HIFB_FENCE_SUPPORT
    if (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE
        || pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE
        || pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_FENCE)
#else
    if (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE
        || pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
#endif
    {
        *pu64PhyAddr = pstPar->stCursorInfo.stCursor.stCursor.u64PhyAddr
            + (HIFB_CURSOR_DEF_VRAM*1024) * (HI_U64)(1-u32IndexForInt) /2;
    }
    else
    {
        *pu64PhyAddr = pstPar->stCursorInfo.stCursor.stCursor.u64PhyAddr
            + (HIFB_CURSOR_DEF_VRAM*1024) * (HI_U64)u32IndexForInt /2;
    }

    return;
}
#endif
static inline HI_VOID hifb_get_workcursorbuf(HIFB_PAR_S *pstPar, HI_U64 *pu64PhyAddr)
{
    HI_U32 u32IndexForInt = pstPar->stRefreshInfo.stDispBufInfo.u32IndexForInt;

    *pu64PhyAddr = pstPar->stCursorInfo.stCursor.stCursor.u64PhyAddr
        + (HIFB_CURSOR_DEF_VRAM*1024) * (HI_U64)u32IndexForInt /2;

    return;
}


static inline HI_VOID hifb_set_dispbufflip(HIFB_PAR_S *pstPar, HI_BOOL bNeedFlip)
{
    pstPar->stRefreshInfo.stDispBufInfo.bNeedFlip = bNeedFlip;

    return;
}

static inline HI_VOID hifb_get_bufmode(HIFB_PAR_S *pstPar, HIFB_LAYER_BUF_E *penBufMode)
{
    *penBufMode = pstPar->stRefreshInfo.enBufMode;

    return;
}

static inline HI_VOID hifb_get_premul(HIFB_PAR_S *pstPar, HI_BOOL *pbPremul)
{
    *pbPremul = pstPar->stDisplayInfo.bPreMul;

    return;
}

static inline HI_VOID hifb_get_antiflickerlevel(HIFB_PAR_S *pstPar, HIFB_LAYER_ANTIFLICKER_LEVEL_E *penAntiflickerLevel)
{
    *penAntiflickerLevel = pstPar->stDisplayInfo.enAntiflickerLevel;

    return;
}

static inline HI_VOID hifb_get_layerpos(HIFB_PAR_S *pstPar, HIFB_POINT_S *pstPos)
{
    osal_memcpy(pstPos, &pstPar->stDisplayInfo.stPos, sizeof(HIFB_POINT_S));

    return;
}

static inline HI_VOID hifb_get_screensize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height)
{
    HIFB_DISPLAY_INFO_S *pstDisplayInfo = NULL;
    if (pstPar == NULL) {
        return;
    }
    pstDisplayInfo = &pstPar->stDisplayInfo;

    if ((pu32Width != NULL) && (pu32Height != NULL)) {
        *pu32Width = pstDisplayInfo->u32ScreenWidth;
        *pu32Height = pstDisplayInfo->u32ScreenHeight;
    }

    return;
}

static inline HI_S32 hifb_set_screensize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HIFB_DISPLAY_INFO_S *pstDisplayInfo = NULL;

    if ((pstPar == NULL) || (pu32Width == NULL) || (pu32Height == NULL)) {
        return HI_FAILURE;
    }
    pstDisplayInfo = &pstPar->stDisplayInfo;

    if(s_stDrvOps.pstCapability[pstPar->u32LayerID].bVoScale)
    {
        pstDisplayInfo->u32ScreenWidth = *pu32Width;
        pstDisplayInfo->u32ScreenHeight = *pu32Height;
    }
    else
    {
       if  (pstDisplayInfo->u32DisplayWidth != *pu32Width || pstDisplayInfo->u32DisplayHeight != *pu32Height)
        {
            HIFB_ERROR("ScreenSize(%d, %d) can't be different with DisplaySize(%d, %d) \
                when layer %d don't scale\n",*pu32Width, *pu32Height,
            pstDisplayInfo->u32DisplayWidth, pstDisplayInfo->u32DisplayHeight,pstPar->u32LayerID);
            s32Ret = HI_FAILURE;
        }
    }

    return s32Ret;
}

static inline HI_VOID hifb_get_maxscreensize(HIFB_PAR_S *pstPar, HI_U32 *pu32Width, HI_U32 *pu32Height)
{
    HIFB_OSD_DATA_S            stLayerData;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = NULL;
    if (pstPar == NULL) {
        return;
    }
    pstDisplayInfo = &pstPar->stDisplayInfo;

    if (HI_SUCCESS == s_stDrvOps.HIFB_DRV_GetOSDData(pstPar->u32LayerID, &stLayerData))
    {
        pstDisplayInfo->u32MaxScreenWidth = stLayerData.u32ScreenWidth;
        pstDisplayInfo->u32MaxScreenHeight = stLayerData.u32ScreenHeight;
    }

    if ((pu32Width != NULL) && (pu32Height != NULL)) {
        *pu32Width = pstDisplayInfo->u32MaxScreenWidth;
        *pu32Height = pstDisplayInfo->u32MaxScreenHeight;
    }

    return;
}
#if(HICHIP == HI3516E_V200)
static HI_S32 hifb_tde_rotate_callback(HI_U32 call_mod_id, HI_U32 call_dev_id, HI_U32 call_chn_id,HI_VOID* pJobData)
#else
static HI_S32 hifb_tde_rotate_callback(const HI_VOID *pParaml, HI_VOID *pParamr)
#endif
{
    HI_U32 u32LayerId = 0;
    HI_S32  s32TdeFinishHandle = 0;
    HIFB_PAR_S *               pstPar             = NULL;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo     = NULL;
    HIFB_LAYER_BUF_E           enBufMode;
    unsigned long              lockflag;
#if(HICHIP == HI3516E_V200)
    vgs_job_data *pCallbackJobData = (vgs_job_data *)pJobData;
    u32LayerId = pCallbackJobData->private_data[0];
    s32TdeFinishHandle = pCallbackJobData->private_data[1];
#else
    HIFB_TDECALLBACK_PARAM *   pParam             = (HIFB_TDECALLBACK_PARAM *)pParaml;
    s32TdeFinishHandle = *(HI_S32 *)pParamr;
#endif


#if(HICHIP != HI3516E_V200)
    if (pParam!=NULL)
    {
        u32LayerId = pParam->u32LayerId;
    }
    else
    {
        HIFB_ERROR("pParam is NULL\n");
        return HI_FAILURE;
    }
#endif
    if (u32LayerId <= s_stDrvOps.u32LayerCount)
    {
        pstPar             = (HIFB_PAR_S *)(g_stLayer[u32LayerId].pstInfo->par);
    }
    else
    {
        HIFB_ERROR("u32LayerId = %d is invalid\n", u32LayerId);
        return HI_FAILURE;
    }
    pstRefreshInfo     = &pstPar->stRefreshInfo;
    hifb_get_bufmode(pstPar, &enBufMode);

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);

#ifdef CONFIG_HIFB_FENCE_SUPPORT
    if (((HIFB_LAYER_BUF_DOUBLE == enBufMode) || (HIFB_LAYER_BUF_FENCE == enBufMode))
    && (pstRefreshInfo->stDispBufInfo.s32RefreshHandle == s32TdeFinishHandle))
#else
    if ((HIFB_LAYER_BUF_DOUBLE == enBufMode)
    && (pstRefreshInfo->stDispBufInfo.s32RefreshHandle == s32TdeFinishHandle))
#endif
    {
        /*Notify VO, it can be updated now*/
        hifb_set_dispbufflip(pstPar, HI_TRUE);

        /* Compression is not supported when there is rotation */
        pstPar->stRefreshInfo.stDispBufInfo.bCompress = HI_FALSE;

    }
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
#if(HICHIP != HI3516E_V200)
    if(pParam != NULL)
    {
        kfree(pParam);
    }
#endif
    return HI_SUCCESS;
}

#if(HICHIP == HI3516E_V200)
static HI_S32 hifb_tde_callback(HI_U32 call_mod_id, HI_U32 call_dev_id, HI_U32 call_chn_id,HI_VOID* pJobData)
#else
static HI_S32 hifb_tde_callback(const HI_VOID *pParaml, HI_VOID *pParamr)
#endif
{
    HI_U32 u32LayerId = 0;
    HI_S32  s32TdeFinishHandle = 0;
    struct fb_info *           info               = NULL;
    HIFB_PAR_S *               pstPar             = NULL;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo     = NULL;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo     = NULL;
    HIFB_LAYER_BUF_E           enBufMode;
    unsigned long              lockflag;
    HI_U32                     u32AR_size;
    HI_U32                     u32GB_size;
    HI_U32                     u32BufSize         = 0;
    HI_U32                     u32IndexForInt     = 0;
    HIFB_BUFFER_S         stSrcImg;
    HIFB_BUFFER_S         stDstImg;
    HIFB_ROTATE_OPT_S     stRotOpt;
    HI_BOOL               bRefreshScreen     = HI_TRUE;
    HI_U32                u32BytesPerPixel   = 2;
#if(HICHIP == HI3516E_V200)
    vgs_job_data *pCallbackJobData = (vgs_job_data *)pJobData;
    u32LayerId = pCallbackJobData->private_data[0];
    s32TdeFinishHandle = pCallbackJobData->private_data[1];
#else
    HIFB_TDECALLBACK_PARAM *   pParam             = (HIFB_TDECALLBACK_PARAM *)pParaml;
    s32TdeFinishHandle = *(HI_S32 *)pParamr;
#endif

    // Here got a wrong u32LayerId, make hifb core dump here.


    // for graphic rot.
        /*********************
        * TDE rotate info declaration.
        ***********************/


#if(HICHIP != HI3516E_V200)
    if (pParam!=NULL)
    {
        u32LayerId = pParam->u32LayerId;
    }
    else
    {
        HIFB_ERROR("pParam is NULL\n");
        return HI_FAILURE;
    }
#endif
    if (u32LayerId <= s_stDrvOps.u32LayerCount)
    {
        info = g_stLayer[u32LayerId].pstInfo;
    }
    else
    {
        HIFB_ERROR("u32LayerId = %d is invalid\n", u32LayerId);
        return HI_FAILURE;
    }
    pstPar             = (HIFB_PAR_S *)(info->par);
    pstRefreshInfo     = &pstPar->stRefreshInfo;
    pstDisplayInfo     = &pstPar->stDisplayInfo;
    u32BufSize         = ((hifb_get_line_length(info) * hifb_get_yres(info))+HIFB_ALIGNMENT)&(~HIFB_ALIGNMENT);
    u32IndexForInt     = pstRefreshInfo->stDispBufInfo.u32IndexForInt;

    HIFB_INFO("tde callback blit handle:%x, end handle:%x\n",
            pstRefreshInfo->stDispBufInfo.s32RefreshHandle, s32TdeFinishHandle);

    hifb_get_bufmode(pstPar, &enBufMode);

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);

    if((HIFB_ROTATE_90 == pstDisplayInfo->enRotateMode
        || HIFB_ROTATE_270 == pstDisplayInfo->enRotateMode)
        /*&& hifb_check_tdeHandle(pstPar,s32TdeFinishHandle)*/)
    {

        HI_S32 s32Ret = HI_SUCCESS;

        /***********
        *Fill src image info.
        ***************/
        stSrcImg.stCanvas.u64PhyAddr = pstPar->u64RotateVb;
        stSrcImg.stCanvas.u32Width   = pstDisplayInfo->u32DisplayHeight;
        stSrcImg.stCanvas.u32Height  = pstDisplayInfo->u32DisplayWidth;
        stSrcImg.UpdateRect.x = 0;
        stSrcImg.UpdateRect.y = 0;
        stSrcImg.UpdateRect.w = stSrcImg.stCanvas.u32Width;
        stSrcImg.UpdateRect.h = stSrcImg.stCanvas.u32Height;

        stSrcImg.stCanvas.enFmt      =  pstPar->enColFmt;
        if (HIFB_FMT_ARGB1555 == stSrcImg.stCanvas.enFmt ||
            HIFB_FMT_ARGB4444 == stSrcImg.stCanvas.enFmt)
        {
            u32BytesPerPixel = 2;
        }
        else if(HIFB_FMT_ARGB8888 == stSrcImg.stCanvas.enFmt)
        {
            u32BytesPerPixel = 4;
        }
        stSrcImg.stCanvas.u32Pitch = ((u32BytesPerPixel * stSrcImg.stCanvas.u32Width + HIFB_ALIGN - 1)/HIFB_ALIGN)*HIFB_ALIGN;
        /***********
        *Fill dst image info.
        ***************/
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        if((HIFB_LAYER_BUF_DOUBLE == enBufMode) || (HIFB_LAYER_BUF_FENCE == enBufMode))
#else
        if(HIFB_LAYER_BUF_DOUBLE == enBufMode)
#endif
        {
            hifb_get_idledispbuf(pstPar, &stDstImg.stCanvas.u64PhyAddr);
        }
        else if(HIFB_LAYER_BUF_ONE == enBufMode)
        {
            stDstImg.stCanvas.u64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[pstRefreshInfo->stDispBufInfo.u32IndexForInt];
        }
        else if(HIFB_LAYER_BUF_DOUBLE_IMMEDIATE == enBufMode)
        {
            stDstImg.stCanvas.u64PhyAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[1-pstRefreshInfo->stDispBufInfo.u32IndexForInt];
        }
        stDstImg.stCanvas.u32Width = stSrcImg.stCanvas.u32Height;
        stDstImg.stCanvas.u32Height = stSrcImg.stCanvas.u32Width;
        stDstImg.stCanvas.enFmt = stSrcImg.stCanvas.enFmt;
        stDstImg.UpdateRect.x = 0;
        stDstImg.UpdateRect.y = 0;
        stDstImg.UpdateRect.w = stDstImg.stCanvas.u32Width;
        stDstImg.UpdateRect.h = stDstImg.stCanvas.u32Height;
        if (HIFB_FMT_ARGB1555 == stDstImg.stCanvas.enFmt ||
            HIFB_FMT_ARGB4444 == stDstImg.stCanvas.enFmt)
        {
            u32BytesPerPixel = 2;
        }
        else if(HIFB_FMT_ARGB8888 == stDstImg.stCanvas.enFmt)
        {
            u32BytesPerPixel = 4;
        }
        stDstImg.stCanvas.u32Pitch = ((u32BytesPerPixel * stDstImg.stCanvas.u32Width + HIFB_ALIGN - 1)/HIFB_ALIGN)*HIFB_ALIGN;;
        /***********
        *Fill rot option info.
        ***************/
        stRotOpt.enRotateMode = pstDisplayInfo->enRotateMode;
        /*Note: After turning on the callback, the rotation job callback function may not be called.
          Need to wait until the next tde task is called, the direct phenomenon is: if the last run
          In the use case, the rotation angle of 90 or 270 is set and the tde rotation branch is entered.
          Unloading ko and loading ko will result in core dump.*/
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        if((HIFB_LAYER_BUF_DOUBLE == enBufMode) || (HIFB_LAYER_BUF_FENCE == enBufMode))
#else
        if(HIFB_LAYER_BUF_DOUBLE == enBufMode)
#endif
        {
            stRotOpt.bCallBack = HI_TRUE;
        }
        else if (HIFB_LAYER_BUF_ONE == enBufMode)
        {
            stRotOpt.bCallBack = HI_FALSE;
        }else
        {
            stRotOpt.bCallBack = HI_FALSE;
        }
        // In the interrupt, the blocking mode is not allowed, so the non-blocking mode is used.
        stRotOpt.bBlock    = HI_FALSE;
        stRotOpt.pParam = &(pstPar->u32LayerID);
        /***********
        *Fill bRefreshScreen.Not used now.
        ***************/
        bRefreshScreen = HI_TRUE;
        s32Ret = HIFB_DRV_Rotate(&stSrcImg, &stDstImg, &stRotOpt, bRefreshScreen);
        pstRefreshInfo->stDispBufInfo.s32RefreshHandle = s32Ret;
    }
    else
    {
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        if (((HIFB_LAYER_BUF_DOUBLE == enBufMode) || (HIFB_LAYER_BUF_FENCE == enBufMode))
#else
        if ((HIFB_LAYER_BUF_DOUBLE == enBufMode)
#endif
        && (pstRefreshInfo->stDispBufInfo.s32RefreshHandle == s32TdeFinishHandle))
        {
            /*Notify VO, can be updated*/
            hifb_set_dispbufflip(pstPar, HI_TRUE);
#if(HICHIP != HI3516E_V200)
            pstPar->stRefreshInfo.stDispBufInfo.bCompress = pParam->bCompress;
#endif
        }

        if (pstPar->stCompressInfo.bCompressOpen)
        {
            // In the case of compression, the size of the compressed data stored in the first 16 bytes is obtained.
            if((HIFB_LAYER_BUF_ONE == enBufMode)
            && (pstRefreshInfo->stDispBufInfo.s32RefreshHandle == s32TdeFinishHandle))
            {
                hifb_get_dcmp_framesize(NULL, &u32AR_size,&u32GB_size,(HI_VOID*)((HI_U8 *)(hifb_get_screen_base(info))) ,u32BufSize/2);
            }
            else if
#ifdef CONFIG_HIFB_FENCE_SUPPORT
            ((((HIFB_LAYER_BUF_DOUBLE == enBufMode) || (HIFB_LAYER_BUF_FENCE == enBufMode)) && (pstRefreshInfo->stDispBufInfo.s32RefreshHandle == s32TdeFinishHandle))
            || (HIFB_LAYER_BUF_DOUBLE_IMMEDIATE == enBufMode))
#else
            ((HIFB_LAYER_BUF_DOUBLE == enBufMode && pstRefreshInfo->stDispBufInfo.s32RefreshHandle == s32TdeFinishHandle)
            || (HIFB_LAYER_BUF_DOUBLE_IMMEDIATE == enBufMode))
#endif
            {
                hifb_get_dcmp_framesize(NULL, &u32AR_size,&u32GB_size,(HI_VOID*)((HI_U8 *)(hifb_get_screen_base(info)) + (1-u32IndexForInt) * u32BufSize) ,u32BufSize/2);
            }
            else
            {
                u32AR_size = 0;
                u32GB_size = 0;
                HIFB_ERROR("%s,%d,Layer(%d) buffer mode %d is supported,ARSize and GBSize are set to %d,%d\n",__FUNCTION__,__LINE__, u32LayerId,enBufMode,u32AR_size,u32GB_size);
            }
            pstPar->stCompressInfo.u32Frame_Size0 =  u32AR_size;
            pstPar->stCompressInfo.u32Frame_Size1 =  u32GB_size;

            pstPar->stCompressInfo.bUpdateFinished = HI_TRUE;

        }
    }


    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
#if(HICHIP != HI3516E_V200)
    if(pParam != NULL)
    {
        kfree(pParam);
    }
#endif
    return HI_SUCCESS;
}

static inline HI_BOOL hifb_is_interlace(HIFB_PAR_S *pstPar)
{
    HIFB_OSD_DATA_S stOsdData = {0};
    if (pstPar != NULL) {
        s_stDrvOps.HIFB_DRV_GetOSDData(pstPar->u32LayerID, &stOsdData);
    }
    return (HIFB_SCANMODE_I == stOsdData.eScanMode);
}

HI_U64 hifb_buf_allocmem(const HI_CHAR *pBufName, HI_UL ulLayerSize, const HI_CHAR *pszMmzName)
{
    HI_U64 addr;

    if ((ulLayerSize == 0) || (ulLayerSize > 0x40000000))
    {
        return 0;
    }

    addr = cmpi_mmz_malloc((HI_CHAR *)pszMmzName, (HI_CHAR *)pBufName, ulLayerSize);
    if (MMB_ADDR_INVALID == addr && NULL != pszMmzName)
    {
        addr = cmpi_mmz_malloc(NULL, (HI_CHAR *)pBufName, ulLayerSize);
    }

    if (MMB_ADDR_INVALID == addr)
    {
        HIFB_ERROR("alloc mem failed!\n");
        return 0;
    }

    return addr;
}

static HI_VOID hifb_set_bufmode(HI_U32 u32LayerId, HIFB_LAYER_BUF_E enLayerBufMode)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &pstPar->stRefreshInfo;

    /* in 0 buf mode ,maybe the stride or fmt will be changed! */
    if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE)
        && (pstRefreshInfo->enBufMode != enLayerBufMode))
    {
        pstPar->bModifying = HI_TRUE;

        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;

        pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FMT;

        pstPar->bModifying = HI_FALSE;
    }

    pstRefreshInfo->enBufMode = enLayerBufMode;
}

/*choose the module to do  flicker resiting, TDE or VOU ? the rule is as this ,the moudle should do flicker resisting who has do scaling*/
static HI_VOID hifb_select_antiflicker_mode(HIFB_PAR_S *pstPar)
{

    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = NULL;
    if (pstPar == NULL) {
        return;
    }
    pstDisplayInfo = &pstPar->stDisplayInfo;

    /*if the usr's configuration is no needed to do flicker resisting, so no needed to do it  */
   if (pstDisplayInfo->enAntiflickerLevel == HIFB_LAYER_ANTIFLICKER_NONE)
   {
       pstDisplayInfo->enAntiflickerMode = HIFB_ANTIFLICKER_NONE;
   }
   else
   {
       /*current standard no needed to do flicker resisting */
       if (!pstDisplayInfo->bNeedAntiflicker)
       {
           pstDisplayInfo->enAntiflickerMode = HIFB_ANTIFLICKER_NONE;
       }
       else
       {
           /*VO has done scaling , so should do flicker resisting at the same time*/
           if ((pstDisplayInfo->u32DisplayWidth != pstDisplayInfo->u32ScreenWidth)
             || (pstDisplayInfo->u32DisplayHeight != pstDisplayInfo->u32ScreenHeight))
           {
               pstDisplayInfo->enAntiflickerMode = HIFB_ANTIFLICKER_VO;
           }
           else
           {
               pstDisplayInfo->enAntiflickerMode = HIFB_ANTIFLICKER_TDE;
           }
       }
   }

}

static HI_VOID hifb_set_antiflickerlevel(HI_U32 u32LayerId, HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel)
{
    struct fb_info *           info   = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &pstPar->stDisplayInfo;

    pstDisplayInfo->enAntiflickerLevel = enAntiflickerLevel;
    hifb_select_antiflicker_mode(pstPar);

    return;
}
#ifdef CURSOR

/* restore or update cursor backup */
static HI_VOID hifb_cursor_bakup(HI_U32 u32LayerId)
{
    struct fb_info *  info = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *      pstPar = (HIFB_PAR_S *)info->par;

    HIFB_BUFFER_S     stCursorBuf;
    HIFB_BUFFER_S     stDisplayBuf;

    HIFB_BLIT_OPT_S   stBlitOpt = {0};

    HI_S32            s32Ret;

    osal_memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));

    if (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_NONE)
    {
        if (pstPar->stRefreshInfo.stUserBuffer.stCanvas.u64PhyAddr == 0)
        {
            return;
        }

        osal_memcpy(&stDisplayBuf.stCanvas, &(pstPar->stRefreshInfo.stUserBuffer.stCanvas), sizeof(HIFB_SURFACE_S));
    }
    else
    {
        stDisplayBuf.stCanvas.enFmt = pstPar->enColFmt;
        stDisplayBuf.stCanvas.u32Height = pstPar->stDisplayInfo.u32DisplayHeight;
        stDisplayBuf.stCanvas.u32Width = pstPar->stDisplayInfo.u32DisplayWidth;
        stDisplayBuf.stCanvas.u32Pitch = hifb_get_line_length(info);
        hifb_get_workdispbuf(pstPar, &stDisplayBuf.stCanvas.u64PhyAddr);
    }

    osal_memcpy(&stCursorBuf.stCanvas, &pstPar->stCursorInfo.stCursor.stCursor, sizeof(HIFB_SURFACE_S));

    osal_memcpy(&stCursorBuf.UpdateRect, &(pstPar->stCursorInfo.stRectInDispBuf), sizeof(HIFB_RECT));
    stCursorBuf.UpdateRect.x = 0;
    stCursorBuf.UpdateRect.y = 0;
    hifb_get_workcursorbuf(pstPar, &stCursorBuf.stCanvas.u64PhyAddr);

    osal_memcpy(&stDisplayBuf.UpdateRect, &(pstPar->stCursorInfo.stRectInDispBuf), sizeof(HIFB_RECT));

    stBlitOpt.enMirrorMode = pstPar->stDisplayInfo.enMirrorMode;

    s32Ret = HIFB_DRV_Blit(&stDisplayBuf, &stCursorBuf, &stBlitOpt,HI_FALSE, NULL);
    if (s32Ret < 0)
    {
        HIFB_ERROR("blit err! 1\n");
        return;
    }
#ifdef CONFIG_HIFB_FENCE_SUPPORT
    if ((pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_FENCE))
#else
    if ((pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE))
#endif
    {
        hifb_get_idledispbuf(pstPar, &stDisplayBuf.stCanvas.u64PhyAddr);
        hifb_get_idlecursorbuf(pstPar, &stCursorBuf.stCanvas.u64PhyAddr);
        s32Ret = HIFB_DRV_Blit(&stDisplayBuf, &stCursorBuf, &stBlitOpt,HI_FALSE, NULL);
        if (s32Ret < 0)
        {
            HIFB_ERROR("blit err! 2\n");
            return ;
        }
    }

    return;
}

static HI_S32 hifb_cursor_show(HI_U32 u32LayerId)
{
    struct fb_info *   info          = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *       pstPar        = info->par;

    struct fb_info *   cursorinfo ;
    HIFB_PAR_S *       pstCursorPar;

    HIFB_BLIT_OPT_S    stBlitOpt     = {0};

    HIFB_BUFFER_S      stCursorBuf;
    HIFB_BUFFER_S      stDisplayBuf;

    HI_S32             s32Ret;

    if (!pstPar->stCursorInfo.bAttched || !pstPar->bShow)
    {
        HIFB_INFO("Cursor should be attached to layer%d \n", u32LayerId);
        return HI_FAILURE;
    }

    cursorinfo = g_stLayer[pstPar->stCursorInfo.u32AttachedCursorId].pstInfo;
    pstCursorPar = cursorinfo->par;

    if (!pstCursorPar->bShow)
    {
        return HI_SUCCESS;
    }

    if (pstCursorPar->stCursorInfo.stCursor.stCursor.u64PhyAddr == 0)
    {
        HIFB_INFO("No cusor img set!\n");
        return HI_FAILURE;
    }

    hifb_cursor_calcdispinfo(pstCursorPar->u32LayerID,pstPar, &pstCursorPar->stDisplayInfo.stPos);

    osal_memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));

    hifb_cursor_bakup(u32LayerId);

    stCursorBuf.stCanvas = pstCursorPar->stCursorInfo.stCursor.stCursor;
    stBlitOpt.stAlpha = pstCursorPar->stAlpha;
    stBlitOpt.stCKey = pstCursorPar->stCkey;
    stBlitOpt.enMirrorMode = pstPar->stDisplayInfo.enMirrorMode;

    if (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_NONE)
    {
        if (pstPar->stRefreshInfo.stUserBuffer.stCanvas.u64PhyAddr == 0)
        {
            return HI_FAILURE;
        }
        osal_memcpy(&stDisplayBuf, &(pstPar->stRefreshInfo.stUserBuffer), sizeof(HIFB_BUFFER_S));
    }
    else
    {
        stDisplayBuf.stCanvas.enFmt = pstPar->enColFmt;
        stDisplayBuf.stCanvas.u32Height = pstPar->stDisplayInfo.u32DisplayHeight;
        stDisplayBuf.stCanvas.u32Width = pstPar->stDisplayInfo.u32DisplayWidth;
        stDisplayBuf.stCanvas.u32Pitch = hifb_get_line_length(info);
        hifb_get_idledispbuf(pstPar, &stDisplayBuf.stCanvas.u64PhyAddr);
    }

    osal_memcpy(&stCursorBuf.UpdateRect, &(pstPar->stCursorInfo.stRectInDispBuf), sizeof(HIFB_RECT));
    stCursorBuf.UpdateRect.x = pstPar->stCursorInfo.stPosInCursor.s32XPos;
    stCursorBuf.UpdateRect.y = pstPar->stCursorInfo.stPosInCursor.s32YPos;

    osal_memcpy(&stDisplayBuf.UpdateRect, &(pstPar->stCursorInfo.stRectInDispBuf), sizeof(HIFB_RECT));

    s32Ret = HIFB_DRV_Blit(&stCursorBuf, &stDisplayBuf,&stBlitOpt,HI_FALSE, NULL);
    if (s32Ret < 0)
    {
        HIFB_ERROR("blit err! 6\n");
        return HI_FAILURE;
    }
#ifdef CONFIG_HIFB_FENCE_SUPPORT
    if ((pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_FENCE))
#else
    if ((pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE))
#endif
    {
        hifb_get_workdispbuf(pstPar, &stDisplayBuf.stCanvas.u64PhyAddr);
        s32Ret = HIFB_DRV_Blit(&stCursorBuf, &stDisplayBuf,&stBlitOpt,HI_FALSE, NULL);
        if (s32Ret < 0)
        {
            HIFB_ERROR("blit err!\n");
            return HI_FAILURE;
        }

    }

    pstPar->bShow = HI_TRUE;

    if (pstPar->stCompressInfo.bCompressOpen)
    {
        pstPar->stCompressInfo.bUpdateFinished = HI_FALSE;
        /* Waiting for TDE to complete the blit */
        schedule_timeout(msecs_to_jiffies(40));
        pstPar->stCompressInfo.stUpdateRect.x = 0;
        pstPar->stCompressInfo.stUpdateRect.y = 0;
        pstPar->stCompressInfo.stUpdateRect.w = pstPar->stDisplayInfo.u32DisplayWidth;
        pstPar->stCompressInfo.stUpdateRect.h = pstPar->stDisplayInfo.u32DisplayHeight;
        pstPar->stCompressInfo.bUpdateFinished = HI_TRUE;
    }
    return HI_SUCCESS;
}

static HI_S32 hifb_cursor_hide(HI_U32 u32LayerId)
{
    struct fb_info *          info          = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *              pstPar        = info->par;
    struct fb_info *          cursorinfo;
    HIFB_PAR_S *              pstCursorPar;

    HIFB_BLIT_OPT_S           stBlitOpt     = {0};
    HIFB_BUFFER_S             stCursorBuf;
    HIFB_BUFFER_S             stDisplayBuf;
    HI_S32                    s32Ret;

    //unsigned long lock_flag;

    if (!pstPar->stCursorInfo.bAttched)
    {
        HIFB_INFO("Cursor should be attached to layer%d \n", u32LayerId);
        return HI_FAILURE;
    }

    cursorinfo = g_stLayer[pstPar->stCursorInfo.u32AttachedCursorId].pstInfo;
    pstCursorPar = (HIFB_PAR_S *)cursorinfo->par;

    if (!pstCursorPar->bShow || !pstPar->bShow)
    {
        return HI_SUCCESS;
    }


    osal_memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));

    stCursorBuf.stCanvas = pstPar->stCursorInfo.stCursor.stCursor;

    if (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_NONE)
    {
        if (pstPar->stRefreshInfo.stUserBuffer.stCanvas.u64PhyAddr == 0)
        {
            HIFB_INFO("No user buf send to fb in 0 buf mode!\n");
            return HI_FAILURE;
        }
        osal_memcpy(&stDisplayBuf, &(pstPar->stRefreshInfo.stUserBuffer), sizeof(HIFB_BUFFER_S));
    }
    else
    {
        stDisplayBuf.stCanvas.enFmt = pstPar->enColFmt;
        stDisplayBuf.stCanvas.u32Height = pstPar->stDisplayInfo.u32DisplayHeight;
        stDisplayBuf.stCanvas.u32Width = pstPar->stDisplayInfo.u32DisplayWidth;
        stDisplayBuf.stCanvas.u32Pitch = hifb_get_line_length(info);
        hifb_get_idledispbuf(pstPar, &stDisplayBuf.stCanvas.u64PhyAddr);
    }

    osal_memcpy(&stCursorBuf.UpdateRect, &(pstPar->stCursorInfo.stRectInDispBuf), sizeof(HIFB_RECT));
    stCursorBuf.UpdateRect.x = 0;
    stCursorBuf.UpdateRect.y = 0;

    osal_memcpy(&stDisplayBuf.UpdateRect, &(pstPar->stCursorInfo.stRectInDispBuf), sizeof(HIFB_RECT));

    hifb_get_idlecursorbuf(pstPar, &stCursorBuf.stCanvas.u64PhyAddr);

    stBlitOpt.enMirrorMode = pstPar->stDisplayInfo.enMirrorMode;

    s32Ret = HIFB_DRV_Blit(&stCursorBuf, &stDisplayBuf, &stBlitOpt, HI_FALSE, NULL);
    if (s32Ret < 0)
    {
        HIFB_ERROR("blit err!\n");
        return HI_FAILURE;
    }
#ifdef CONFIG_HIFB_FENCE_SUPPORT
    if ((pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_FENCE))
#else
    if ((pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE)
        || (pstPar->stRefreshInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE))
#endif
    {
        hifb_get_workdispbuf(pstPar, &stDisplayBuf.stCanvas.u64PhyAddr);
        hifb_get_workcursorbuf(pstPar, &stCursorBuf.stCanvas.u64PhyAddr);

        s32Ret = HIFB_DRV_Blit(&stCursorBuf, &stDisplayBuf, &stBlitOpt, HI_FALSE, NULL);
        if (s32Ret < 0)
        {
            HIFB_ERROR("blit err!\n");
            return HI_FAILURE;
        }

    }

    if (pstPar->stCompressInfo.bCompressOpen)
    {
        pstPar->stCompressInfo.bUpdateFinished = HI_FALSE;
        /* Waiting for TDE to complete the blit */
        schedule_timeout(msecs_to_jiffies(40));
        pstPar->stCompressInfo.stUpdateRect.x = 0;
        pstPar->stCompressInfo.stUpdateRect.y = 0;
        pstPar->stCompressInfo.stUpdateRect.w = pstPar->stDisplayInfo.u32DisplayWidth;
        pstPar->stCompressInfo.stUpdateRect.h = pstPar->stDisplayInfo.u32DisplayHeight;
        pstPar->stCompressInfo.bUpdateFinished = HI_TRUE;
    }
    return HI_SUCCESS;
}

static HI_S32 hifb_cursor_attach(HI_U32 u32CursorId, HI_U32 u32LayerId)
{
    HI_U32 u32Cnt;

    struct fb_info *info = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

    struct fb_info *cursorinfo = g_stLayer[u32CursorId].pstInfo;
    HIFB_PAR_S *pstCursorPar = (HIFB_PAR_S *)cursorinfo->par;

    unsigned long lock_flag;

    u32Cnt = atomic_read(&pstPar->ref_count);
    if (u32Cnt == 0)
    {
        HIFB_ERROR("failed to attach layer! The layer :%d is not opened!\n", u32LayerId);
        return HI_FAILURE;
    }

    if (HIFB_IS_CLUTFMT(pstPar->enColFmt))
    {
        HIFB_ERROR("failed to attach layer! The layer format is clut not supported!\n");
        return HI_FAILURE;
    }

    if (0 == pstCursorPar->stCursorInfo.stCursor.stCursor.u64PhyAddr)
    {
        HIFB_ERROR("failed to attach layer! The cursor info is not set yet!\n");
        return HI_FAILURE;
    }

    if (pstPar->stCursorInfo.bAttched)
    {
        if (pstPar->stCursorInfo.u32AttachedCursorId == u32CursorId)
        {
            return HI_SUCCESS;
        }
        else
        {
            HIFB_ERROR("The layer has attached another cursor, you should detach it first!\n");
            return HI_FAILURE;
        }
    }

    pstPar->stCursorInfo.bAttched = 1;
    pstPar->stCursorInfo.u32AttachedCursorId = u32CursorId;

    if ((pstPar->stCursorInfo.stCursor.stCursor.u32Height > pstPar->stDisplayInfo.u32DisplayHeight)
        || (pstPar->stCursorInfo.stCursor.stCursor.u32Width > pstPar->stDisplayInfo.u32DisplayWidth))
    {
        return HI_FAILURE;
    }

    pstPar->stCursorInfo.stCursor.stCursor.u32Height
        = pstCursorPar->stCursorInfo.stCursor.stCursor.u32Height;
    pstPar->stCursorInfo.stCursor.stCursor.u32Width
        = pstCursorPar->stCursorInfo.stCursor.stCursor.u32Width;
    pstPar->stCursorInfo.stCursor.stHotPos = pstCursorPar->stCursorInfo.stCursor.stHotPos;

    /*when cursor attach to layer, we use the positon calculate before*/
    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lock_flag);
    hifb_cursor_show(u32LayerId);
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lock_flag);
    return HI_SUCCESS;
}

static HI_S32 hifb_cursor_detach(HI_U32 u32LayerId)
{
    struct fb_info *info = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

    if (!pstPar->stCursorInfo.bAttched)
    {
        return HI_SUCCESS;
    }

    hifb_cursor_hide(u32LayerId);

    pstPar->stCursorInfo.bAttched = HI_FALSE;
    return HI_SUCCESS;
}

/*  calculate cusor display position info such as the start position of copying, display buffer position, width of copying */
static HI_VOID hifb_cursor_calcdispinfo(HI_U32 u32CursorId, HIFB_PAR_S *pstPar, const HIFB_POINT_S* pstCurNewPos)
{
    struct fb_info *   info              = NULL;
    HIFB_PAR_S *       pstCursorPar      = NULL;

    HIFB_POINT_S       stPosInCursor     = {0};
    HIFB_RECT          stRectInDispBuf   = {0};
    info = g_stLayer[u32CursorId].pstInfo;
    pstCursorPar = (HIFB_PAR_S *)info->par;

    if ((pstPar == NULL) || (pstCurNewPos == NULL)) {
        return;
    }

    if ((pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Height && pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Width && (pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Height != pstPar->stDisplayInfo.u32DisplayHeight))
        || (pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Width && pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Height && (pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Width != pstPar->stDisplayInfo.u32DisplayWidth)))
    {
        hi_tde_rect SrcRect ={0}, DstRect={0}, InSrcRect ={0}, InDstRect ={0};

        SrcRect.width = pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Width;
        SrcRect.height = pstPar->stRefreshInfo.stUserBuffer.stCanvas.u32Height;
        DstRect.width = pstPar->stDisplayInfo.u32DisplayWidth;
        DstRect.height = pstPar->stDisplayInfo.u32DisplayHeight;
        InSrcRect.pos_x = pstCurNewPos->s32XPos;
        InSrcRect.pos_y = pstCurNewPos->s32YPos;
        TdeCalScaleRect_HIFB(&SrcRect, &DstRect, &InSrcRect, &InDstRect);
        stRectInDispBuf.x = InDstRect.pos_x;
        stRectInDispBuf.y = InDstRect.pos_y;
    }
    else
    {
        stRectInDispBuf.x = pstCurNewPos->s32XPos;
        stRectInDispBuf.y = pstCurNewPos->s32YPos;
    }

    if (stRectInDispBuf.x > (HI_S32)pstPar->stDisplayInfo.u32DisplayWidth)
    {
        stRectInDispBuf.x = (HI_S32)(pstPar->stDisplayInfo.u32DisplayWidth - 1);
    }

    if (stRectInDispBuf.y > (HI_S32)pstPar->stDisplayInfo.u32DisplayHeight)
    {
        stRectInDispBuf.y = (HI_S32)(pstPar->stDisplayInfo.u32DisplayHeight - 1);
    }


    stRectInDispBuf.x -= pstCursorPar->stCursorInfo.stCursor.stHotPos.s32XPos;
    stRectInDispBuf.y -= pstCursorPar->stCursorInfo.stCursor.stHotPos.s32YPos;

    stRectInDispBuf.w = (HI_S32)pstCursorPar->stCursorInfo.stCursor.stCursor.u32Width;
    stRectInDispBuf.h = (HI_S32)pstCursorPar->stCursorInfo.stCursor.stCursor.u32Height;
    if ((HI_S32)stRectInDispBuf.x < 0)
    {
        stRectInDispBuf.x = 0;
        stPosInCursor.s32XPos = pstCursorPar->stCursorInfo.stCursor.stHotPos.s32XPos;
        stRectInDispBuf.w -= stPosInCursor.s32XPos;
    }

    if ((HI_S32)stRectInDispBuf.y < 0)
    {
        stRectInDispBuf.y = 0;
        stPosInCursor.s32YPos = pstCursorPar->stCursorInfo.stCursor.stHotPos.s32YPos;
        stRectInDispBuf.h -= stPosInCursor.s32YPos;
    }

    if (stRectInDispBuf.x + stRectInDispBuf.w > (HI_S32)pstPar->stDisplayInfo.u32DisplayWidth)
    {
        stRectInDispBuf.w = (HI_S32)(pstPar->stDisplayInfo.u32DisplayWidth - stRectInDispBuf.x);
    }

    if (stRectInDispBuf.y+ stRectInDispBuf.h > (HI_S32)pstPar->stDisplayInfo.u32DisplayHeight)
    {
        stRectInDispBuf.h = (HI_S32)(pstPar->stDisplayInfo.u32DisplayHeight - stRectInDispBuf.y);
    }

    pstPar->stCursorInfo.stPosInCursor = stPosInCursor;
    pstPar->stCursorInfo.stRectInDispBuf = stRectInDispBuf;
}

static HI_S32 hifb_cursor_changepos(HI_U32 u32CursorId, HIFB_POINT_S stPos)
{
    struct fb_info *cursorinfo = g_stLayer[u32CursorId].pstInfo;
    HIFB_PAR_S *pstCursorPar = (HIFB_PAR_S *)cursorinfo->par;

    HI_U32 i;

    if (stPos.s32XPos < 0 || stPos.s32YPos < 0)
    {
        HIFB_ERROR("the cursor pos(%d,%d) out of range !\n",
                     stPos.s32XPos, stPos.s32YPos);
        return HI_FAILURE;
    }

    /* pos no change */
    if ((stPos.s32XPos == pstCursorPar->stDisplayInfo.stPos.s32XPos)
        && (stPos.s32YPos == pstCursorPar->stDisplayInfo.stPos.s32YPos))
    {
        return HI_SUCCESS;
    }

    pstCursorPar->stDisplayInfo.stPos.s32XPos = stPos.s32XPos;
    pstCursorPar->stDisplayInfo.stPos.s32YPos = stPos.s32YPos;

    if (!pstCursorPar->bShow)
    {
        return HI_FAILURE;
    }

    /* process all layers attached to cursor */
    for (i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        struct fb_info *info = g_stLayer[i].pstInfo;
        HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

        if (IS_CURSOR_LAYER(i))
        {
            continue;
        }

        if(!pstPar->stCursorInfo.bAttched)
        {
            continue;
        }
        hifb_cursor_hide(i);
        hifb_cursor_show(i);
    }

    return HI_SUCCESS;
}

HI_S32 hifb_cursor_changestate(HIFB_PAR_S *pstCursorPar, HI_BOOL bShow)
{
    HI_U32 i;
    if (pstCursorPar == NULL) {
        return HI_FAILURE;
    }

    if (!pstCursorPar->stCursorInfo.stCursor.stCursor.u64PhyAddr)
    {
        HIFB_ERROR("The cursor image addr is NULL!\n");
        return HI_FAILURE;
    }

    /* no change to state , return */
    if (bShow == pstCursorPar->bShow)
    {
        return HI_SUCCESS;
    }

    for(i = 0; i < s_stDrvOps.u32LayerCount; i++)
    {
        if (IS_CURSOR_LAYER(i))
        {
            continue;
        }

        if (bShow)
        {
            pstCursorPar->bShow = HI_TRUE;
            hifb_cursor_show(i);
        }
        else
        {
            hifb_cursor_hide(i);
        }
    }

    pstCursorPar->bShow = bShow;

    return HI_SUCCESS;
}

static HI_S32 hifb_cursor_putinfo(HIFB_PAR_S *pstCursorPar, HIFB_CURSOR_S* pstCursor)
{
    if ((pstCursorPar == NULL) || (pstCursor == NULL)) {
        return HI_FAILURE;
    }

    if (pstCursor->stCursor.u64PhyAddr == 0)
    {
        HIFB_ERROR("cursor image addr is equal to 0!\n");
        pstCursorPar->stCursorInfo.stCursor.stCursor.u64PhyAddr = 0;
        return HI_SUCCESS;
    }

    if (pstCursor->stCursor.u32Width == 0 || pstCursor->stCursor.u32Height == 0)
    {
        HIFB_ERROR("cursor's width or height shouldn't be 0!\n");
        return HI_FAILURE;
    }

    if (pstCursor->stCursor.u32Pitch == 0)
    {
        HIFB_ERROR("cursor's pitch shouldn't be 0!\n");
        return HI_FAILURE;
    }

    if (pstCursor->stCursor.enFmt == HIFB_FMT_BUTT)
    {
        HIFB_ERROR("unknown color fmt!\n");
        return HI_FAILURE;
    }

    pstCursorPar->enColFmt = pstCursor->stCursor.enFmt;
    if (pstCursorPar->enColFmt >= (sizeof(s_stArgbBitField) / sizeof(s_stArgbBitField[0]))) {
        HIFB_ERROR("unknown color fmt!\n");
        return HI_FAILURE;
    }
#ifndef __HuaweiLite__
    osal_memcpy(&g_stLayer[pstCursorPar->u32LayerID].pstInfo->var.red, &s_stArgbBitField[pstCursorPar->enColFmt].stRed,
        sizeof(struct fb_bitfield));
    osal_memcpy(&g_stLayer[pstCursorPar->u32LayerID].pstInfo->var.green, &s_stArgbBitField[pstCursorPar->enColFmt].stGreen, sizeof(struct fb_bitfield));
    osal_memcpy(&g_stLayer[pstCursorPar->u32LayerID].pstInfo->var.blue, &s_stArgbBitField[pstCursorPar->enColFmt].stBlue, sizeof(struct fb_bitfield));
    osal_memcpy(&g_stLayer[pstCursorPar->u32LayerID].pstInfo->var.transp, &s_stArgbBitField[pstCursorPar->enColFmt].stTransp, sizeof(struct fb_bitfield));
#endif
    /*change hotx or hoty will result in cursor position change*/
    if (pstCursor->stCursor.u32Height > HIFB_MAX_CURSOR_HEIGHT)
    {
        pstCursor->stCursor.u32Height = HIFB_MAX_CURSOR_HEIGHT;
    }

    if (pstCursor->stCursor.u32Width > HIFB_MAX_CURSOR_WIDTH)
    {
        pstCursor->stCursor.u32Width = HIFB_MAX_CURSOR_WIDTH;
    }

    if (pstCursor->stHotPos.s32XPos < 0
        || pstCursor->stHotPos.s32XPos > pstCursor->stCursor.u32Width
        || pstCursor->stHotPos.s32YPos < 0
        || pstCursor->stHotPos.s32YPos > pstCursor->stCursor.u32Height)
    {
        HIFB_ERROR("hotpos err!\n");
        return HI_FAILURE;
    }

    if (pstCursor->stHotPos.s32XPos != pstCursorPar->stCursorInfo.stCursor.stHotPos.s32XPos
        || pstCursor->stHotPos.s32YPos != pstCursorPar->stCursorInfo.stCursor.stHotPos.s32YPos)
    {
        pstCursorPar->stCursorInfo.stCursor.stHotPos.s32XPos = pstCursor->stHotPos.s32XPos;
        pstCursorPar->stCursorInfo.stCursor.stHotPos.s32YPos = pstCursor->stHotPos.s32YPos;
    }

    /* to do :update backup */
    osal_memcpy(&(pstCursorPar->stCursorInfo.stCursor), pstCursor, sizeof(HIFB_CURSOR_S));
    return HI_SUCCESS;
}
#endif
/* Does the chip support scaling */
static HI_BOOL hifb_check_imagezoomenable(HI_U32 u32LayerId, HIFB_RECT *pstInRect, HIFB_RECT *pstOutRect)
{
    HI_BOOL bIsLayerSupportZoomOut = HI_TRUE;
    HI_BOOL bNeedZoom = HI_FALSE;

    if ((pstInRect == NULL) || (pstOutRect == NULL)) {
        return HI_FALSE;
    }
    if (!s_stDrvOps.pstCapability[u32LayerId].bVoScale)
    {
        return HI_FALSE;
    }

    if(s_stDrvOps.HIFB_DRV_IsLayerSupportZoomOut)
    {
        bIsLayerSupportZoomOut = s_stDrvOps.HIFB_DRV_IsLayerSupportZoomOut(u32LayerId);
    }

    /* The chip does not support zoomout, and the current image output size is smaller than the input size. */
    if(HI_FALSE == bIsLayerSupportZoomOut && (pstOutRect->w < pstInRect->w || pstOutRect->h < pstInRect->h))
    {
        HIFB_ERROR("HIFB layer%d not support zoomout, please check the display and screen size!\n", u32LayerId);
        return HI_FALSE;
    }

    /* The chip zoomin ratio exceeds the maximum allowed by the chip, and the error is returned. */
    if(pstOutRect->w > (pstInRect->w*HIFB_MAX_ZOOMIN) || pstOutRect->h > (pstInRect->h*HIFB_MAX_ZOOMIN))
    {
        HIFB_ERROR("HIFB layer%d in_size(%d, %d) and out_size(%d, %d) do out of ZoomRatio[1, %d]!!\n", u32LayerId,
                    pstInRect->w, pstInRect->h, pstOutRect->w, pstOutRect->h, VPSS_MAX_ZOOMIN);
        return HI_FALSE;
    }

    bNeedZoom = (pstOutRect->w != pstInRect->w || pstOutRect->h != pstInRect->h);

    if (bNeedZoom && (pstInRect->w > HIFB_LINE_BUF))
    {
        HIFB_ERROR("HIFB layer%d in width: %u is bigger than %d, will not zoom in!!\n", u32LayerId,
                    pstInRect->w, HIFB_LINE_BUF);
        return HI_FALSE;
    }


    return HI_TRUE;
}


HI_VOID hifb_dcmp_config(HI_U32 u32LayerId)
{
    struct fb_info *           info                 = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar               = (HIFB_PAR_S *)(info->par);
    HIFB_REFRESH_INFO_S *      pstRefreshInfo       = &pstPar->stRefreshInfo;
    HI_BOOL                    bDcmpEnState   = HI_FALSE;

     /* In order to support compressed and non-compressed dynamic switching, you cannot use
        pstCompressInfo->bCompressOpen to determine whether to open compression.
        To use the compression information that follows the frame, and whether or not
        to flip to decide whether to switch compression or non-compression */
    if (pstRefreshInfo->stDispBufInfo.bCompress)
    {
        // See HIFB_DRV_EnableDcmp
        s_stDrvOps.HIFB_DRV_EnableDcmp(u32LayerId, HI_TRUE);
        // See HIFB_DRV_GetDcmpEnableState
        s_stDrvOps.HIFB_DRV_GetDcmpEnableState(u32LayerId, &bDcmpEnState);
        if (bDcmpEnState)
        {
            hifb_SetDcmpInfo(pstPar);
        }

    }
    else
    {
        /* Set the address and stride to ensure that the compression will display correctly when switched to non-compressed */
        s_stDrvOps.HIFB_DRV_SetLayerAddr(u32LayerId, pstRefreshInfo->u64ScreenAddr);
        if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE)
            && pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr)
        {
            s_stDrvOps.HIFB_DRV_SetLayerStride(u32LayerId, pstRefreshInfo->stUserBuffer.stCanvas.u32Pitch);
        }
        else
        {
            s_stDrvOps.HIFB_DRV_SetLayerStride(u32LayerId, hifb_get_line_length(info));
        }

        s_stDrvOps.HIFB_DRV_EnableDcmp(u32LayerId, HI_FALSE);
        s_stDrvOps.HIFB_DRV_EnableWbc(u32LayerId, HI_FALSE);
    }
}

/* Callback function for VO vertical timing interrupt */
static HI_S32 hifb_vo_callback(const HI_VOID *pParaml, HI_VOID *pParamr)
{
    HI_U32 *                   pu32LayerId          = NULL;

    struct fb_info *           info                 = NULL;
    HIFB_PAR_S *               pstPar               = NULL;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo       = NULL;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo       = NULL;
    volatile HIFB_COMPRESS_INFO_S * pstCompressInfo = NULL;
    unsigned long              lockflag;
    HIFB_OSD_DATA_S            stLayerData={0};
    HI_S32                     s32Ret;
    HIFB_RECT                  stInRect       = {0};
    HIFB_RECT                  stOutRect      = {0};
    HI_BOOL                    bZmeEnable     = HI_FALSE;
    HI_U32                     u32BufSize     =  0;
#ifndef CONFIG_HI_VO_FB_SEPARATE
    HI_U32     u32VtthdFlag     = 0x0;
    HI_U32     u32FrmStartFlag  = 0x0;
    HI_U32     u32IntStatus     = 0x0;
#endif

    if (pParaml == NULL) {
        return HI_FAILURE;
    }
    pu32LayerId = (HI_U32 *)pParaml;
    if (*pu32LayerId >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    info = g_stLayer[*pu32LayerId].pstInfo;
    if (info == NULL) {
        return HI_FAILURE;
    }
    if (info->par == NULL) {
        return HI_FAILURE;
    }

    pstPar = (HIFB_PAR_S *)(info->par);
    pstDisplayInfo = &pstPar->stDisplayInfo;
    pstRefreshInfo = &pstPar->stRefreshInfo;
    pstCompressInfo = &pstPar->stCompressInfo;
    u32BufSize =  ((hifb_get_line_length(info) * hifb_get_yres(info))+HIFB_ALIGNMENT)&(~HIFB_ALIGNMENT);

#ifndef CONFIG_HI_VO_FB_SEPARATE
    if (pParamr == NULL) {
        HIFB_ERROR("NULL pointer !\n");
        return HI_FAILURE;
    }
    u32IntStatus = *(HI_U32 *)pParamr;
    u32VtthdFlag    = HIFB_INTMSK_HD0_VTTHD1;
    u32FrmStartFlag = HIFB_INTMSK_HD0_VTTHD3;

    if (u32IntStatus & u32FrmStartFlag) {
        pstPar->stRefreshInfo.bDoRefreshJob = HI_TRUE;
        wake_up_interruptible(&(pstPar->doRefreshJob));
        return HI_SUCCESS;
    } else if (u32IntStatus & u32VtthdFlag) {
        pstPar->stRefreshInfo.bDoRefreshJob = HI_FALSE;
    }
#endif

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock,lockflag);

    /* If not displayed, close the graphics layer and exit */
    if (HI_FALSE == pstPar->bShow)
    {
        // HIFB_DRV_EnableLayer
        if(s_stDrvOps.HIFB_DRV_EnableLayer)
        {
            s_stDrvOps.HIFB_DRV_EnableLayer(*pu32LayerId, HI_FALSE);
        }

        // HIFB_DRV_UpdataLayerReg
        if(s_stDrvOps.HIFB_DRV_UpdataLayerReg)
        {
            s_stDrvOps.HIFB_DRV_UpdataLayerReg(*pu32LayerId);
        }

        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
        return HI_SUCCESS;
    }

    if (0 == *pu32LayerId || 1 == *pu32LayerId)
    {
#ifndef __HuaweiLite__
        hifb_vsync_notify(); //vsync signal
#endif
    }

    // Non-modified status, modified, can be modified
    if (!pstPar->bModifying)
    {
        s32Ret = s_stDrvOps.HIFB_DRV_GetOSDData(pstPar->u32LayerID, &stLayerData);
        if (HI_SUCCESS != s32Ret)
        {
            HIFB_ERROR("failed to get layer%d's osd data!\n", pstPar->u32LayerID);
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
            return s32Ret;
        }
        pstDisplayInfo->u32MaxScreenWidth = stLayerData.u32ScreenWidth;
        pstDisplayInfo->u32MaxScreenHeight = stLayerData.u32ScreenHeight;

        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_ALPHA)
        {
            // HIFB_DRV_SetLayerAlpha
            s_stDrvOps.HIFB_DRV_SetLayerAlpha(*pu32LayerId, pstPar->stAlpha);
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_ALPHA;
        }

        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_COLORKEY)
        {
            // HIFB_DRV_SetLayerKeyMask
            s_stDrvOps.HIFB_DRV_SetLayerKeyMask(*pu32LayerId, &pstPar->stCkey);
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_COLORKEY;
        }

        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_FMT)
        {
            if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE)
                && pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr)
            {
                // HIFB_DRV_SetLayerDataFmt
                s_stDrvOps.HIFB_DRV_SetLayerDataFmt(*pu32LayerId, pstRefreshInfo->stUserBuffer.stCanvas.enFmt);
            }
            else
            {
                s_stDrvOps.HIFB_DRV_SetLayerDataFmt(*pu32LayerId, pstPar->enColFmt);
            }
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_FMT;
        }

        // NEW modify graphic dynamic range
        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_DYNAMICRANGE)
        {
            // Modified to HDR10
            if(HIFB_DYNAMIC_RANGE_HDR10 == pstDisplayInfo->enDynamicRange)
            {
                // HIFB_DRV_EnableGHDR
                if(s_stDrvOps.HIFB_DRV_EnableGHDR) s_stDrvOps.HIFB_DRV_EnableGHDR(*pu32LayerId,HI_TRUE);
            }else if(HIFB_DYNAMIC_RANGE_SDR8 == pstDisplayInfo->enDynamicRange || HIFB_DYNAMIC_RANGE_SDR10 == pstDisplayInfo->enDynamicRange)
            {
                // See HIFB_DRV_EnableGHDR
                if(s_stDrvOps.HIFB_DRV_EnableGHDR) s_stDrvOps.HIFB_DRV_EnableGHDR(*pu32LayerId,HI_FALSE);
            }
            /* After the modification is completed, the mask is cleared, just like the interrupt processing mechanism.
             When the interrupt is processed, the interrupt is cleared.*/
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_DYNAMICRANGE;
        }

        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_BMUL)
        {
            if(HI_FAILURE == s_stDrvOps.HIFB_DRV_SetPreMul(*pu32LayerId, pstDisplayInfo->bPreMul))
            {
                if(HI_TRUE == pstPar->stDisplayInfo.bPreMul)
                {
                    pstPar->stDisplayInfo.bPreMul = HI_FALSE;
                }
                else
                {
                    pstPar->stDisplayInfo.bPreMul = HI_TRUE;
                }
            }
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_BMUL;
        }

        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_STRIDE)
        {
            if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE)
                && pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr)
            {
                s_stDrvOps.HIFB_DRV_SetLayerStride(*pu32LayerId, pstRefreshInfo->stUserBuffer.stCanvas.u32Pitch);
            }
            else
            {
                s_stDrvOps.HIFB_DRV_SetLayerStride(*pu32LayerId, hifb_get_line_length(info));
            }
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_STRIDE;
        }

        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL)
        {
            HIFB_DEFLICKER_S stDeflicker;

            stDeflicker.pu8HDfCoef = pstPar->ucHDfcoef;
            stDeflicker.pu8VDfCoef = pstPar->ucVDfcoef;
            stDeflicker.u32HDfLevel = pstPar->u32HDflevel;
            stDeflicker.u32VDfLevel = pstPar->u32VDflevel;

            s_stDrvOps.HIFB_DRV_SetLayerDeFlicker(*pu32LayerId, stDeflicker);
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL;
        }

        // Handles requests to modify input and output sizes.
        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_INRECT
             || pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_OUTRECT)
        {
            /*if VO can't support scaling,  we set the screen's H/W as the same as display */
            if (!s_stDrvOps.pstCapability[*pu32LayerId].bVoScale)
            {
               pstDisplayInfo->u32ScreenWidth = pstDisplayInfo->u32DisplayWidth;
               pstDisplayInfo->u32ScreenHeight = pstDisplayInfo->u32DisplayHeight;
            }

            stInRect.x = pstDisplayInfo->stPos.s32XPos;
            stInRect.y = pstDisplayInfo->stPos.s32YPos;
            stInRect.w = (HI_S32)pstDisplayInfo->u32DisplayWidth;
            stInRect.h = (HI_S32)pstDisplayInfo->u32DisplayHeight;

            stOutRect.x = pstDisplayInfo->stPos.s32XPos;
            stOutRect.y = pstDisplayInfo->stPos.s32YPos;
            stOutRect.w = pstDisplayInfo->u32ScreenWidth;
            stOutRect.h = pstDisplayInfo->u32ScreenHeight;

            if ((HI_U32)stOutRect.x + stOutRect.w > pstDisplayInfo->u32MaxScreenWidth)
            {
                stOutRect.w = (HI_S32)(pstDisplayInfo->u32MaxScreenWidth - stOutRect.x);
            }

            if ((HI_U32)stOutRect.y + stOutRect.h > (HI_S32)pstDisplayInfo->u32MaxScreenHeight)
            {
                stOutRect.h = (HI_S32)(pstDisplayInfo->u32MaxScreenHeight - stOutRect.y);
            }

            /*after cut off, the input rectangle keep rate with output rectangle */

            // Prevent the occurrence of the divide-by-zero error
            if (0 != pstDisplayInfo->u32ScreenWidth && 0 != pstDisplayInfo->u32ScreenHeight)
            {
                stInRect.w = stInRect.w * stOutRect.w / (HI_S32)pstDisplayInfo->u32ScreenWidth;
                stInRect.h = stInRect.h * stOutRect.h / (HI_S32)pstDisplayInfo->u32ScreenHeight;
            }

            if(HI_TRUE == hifb_check_imagezoomenable(*pu32LayerId, &stInRect, &stOutRect))
            {
                /* If you want to go through the zoom module, you need to correct it to 2 alignment,
                otherwise it will appear abnormal.*/
                stInRect.w = ALIGN_DOWN(stInRect.w, 2);
                stInRect.h = ALIGN_DOWN(stInRect.h, 2);

                if(s_stDrvOps.HIFB_DRV_SetLayerRect)
                {
                    s_stDrvOps.HIFB_DRV_SetLayerRect(*pu32LayerId, &stInRect, &stOutRect);
                }

                // TODO
                if(s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso)
                {
                    s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso(*pu32LayerId, &stInRect);
                }

                // NEW!!!   Set zoom
                // See out width and height not equal to in width and height,then enable zme.
                bZmeEnable = (stOutRect.w != stInRect.w || stOutRect.h != stInRect.h);
                if(s_stDrvOps.HIFB_DRV_EnableZME)
                {
                     s_stDrvOps.HIFB_DRV_EnableZME(*pu32LayerId, &stInRect, &stOutRect, bZmeEnable);
                }
            }
            else
            {
                //If scaling is not enabled, the input size is used as the output size and the zoom module is closed.
                if(s_stDrvOps.HIFB_DRV_SetLayerRect)
                {
                    s_stDrvOps.HIFB_DRV_SetLayerRect(*pu32LayerId, &stInRect, &stInRect);
                }

                // TODO
                if(s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso)
                {
                    s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso(*pu32LayerId, &stInRect);
                }

                if(s_stDrvOps.HIFB_DRV_EnableZME)
                {
                     s_stDrvOps.HIFB_DRV_EnableZME(*pu32LayerId, &stInRect, &stOutRect, HI_FALSE);
                }
            }

            // Processing completed, clear mask
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_INRECT;
            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_OUTRECT;
        }

        // The display address is refreshed and the display address is modified.
        if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_DISPLAYADDR)
        {
            if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_FMT)
            {
                if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE)
                    && pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr)
                {
                    s_stDrvOps.HIFB_DRV_SetLayerDataFmt(*pu32LayerId, pstRefreshInfo->stUserBuffer.stCanvas.enFmt);
                }
                else
                {
                    s_stDrvOps.HIFB_DRV_SetLayerDataFmt(*pu32LayerId, pstPar->enColFmt);
                }
            }

            if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_STRIDE)
            {
                if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE)
                    && pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr)
                {
                    s_stDrvOps.HIFB_DRV_SetLayerStride(*pu32LayerId, pstRefreshInfo->stUserBuffer.stCanvas.u32Pitch);
                }
                else
                {
                    s_stDrvOps.HIFB_DRV_SetLayerStride(*pu32LayerId, hifb_get_line_length(info));
                }
            }

            if (pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_INRECT
                || pstPar->u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_OUTRECT)
            {
                HIFB_RECT stInRect = {0};
                HIFB_RECT stOutRect = {0};

                /*if VO can't support scaling,  we set the screen's H/W as the same as display */
                if (!s_stDrvOps.pstCapability[*pu32LayerId].bVoScale)
                {
                   pstDisplayInfo->u32ScreenWidth = pstDisplayInfo->u32DisplayWidth;
                   pstDisplayInfo->u32ScreenHeight = pstDisplayInfo->u32DisplayHeight;
                }

                stInRect.w = (HI_S32)pstDisplayInfo->u32DisplayWidth;
                stInRect.h = (HI_S32)pstDisplayInfo->u32DisplayHeight;

                stOutRect.x = pstDisplayInfo->stPos.s32XPos;
                stOutRect.y = pstDisplayInfo->stPos.s32YPos;
                stOutRect.w = pstDisplayInfo->u32ScreenWidth;
                stOutRect.h = pstDisplayInfo->u32ScreenHeight;

                if ((HI_U32)stOutRect.x + stOutRect.w > pstDisplayInfo->u32MaxScreenWidth)
                {
                    stOutRect.w = (HI_S32)(pstDisplayInfo->u32MaxScreenWidth - stOutRect.x);
                }

                if ((HI_U32)stOutRect.y + stOutRect.h > (HI_S32)pstDisplayInfo->u32MaxScreenHeight)
                {
                    stOutRect.h = (HI_S32)(pstDisplayInfo->u32MaxScreenHeight - stOutRect.y);
                }

                /*after cut off, the input rectangle keep rate with output rectangle */
                if (0 != pstDisplayInfo->u32ScreenWidth && 0 != pstDisplayInfo->u32ScreenHeight) {
                    stInRect.w = stInRect.w * stOutRect.w / (HI_S32)pstDisplayInfo->u32ScreenWidth;
                    stInRect.h = stInRect.h * stOutRect.h / (HI_S32)pstDisplayInfo->u32ScreenHeight;
                }

                if(HI_TRUE == hifb_check_imagezoomenable(*pu32LayerId, &stInRect, &stOutRect))
                {
                    s_stDrvOps.HIFB_DRV_SetLayerRect(*pu32LayerId, &stInRect, &stOutRect);
                    // TODO
                    if( s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso) s_stDrvOps.HIFB_DRV_SetLayerSrcImageReso(*pu32LayerId, &stInRect);
                }
            }

            s_stDrvOps.HIFB_DRV_SetLayerAddr(*pu32LayerId, pstRefreshInfo->u64ScreenAddr);

            pstPar->u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
            pstCompressInfo->bLayerAddrUpdate = HI_TRUE;

            if ((pstRefreshInfo->stDispBufInfo.u64PhyAddr[0] != pstRefreshInfo->stDispBufInfo.u64PhyAddr[1])
                && (pstRefreshInfo->stDispBufInfo.u64PhyAddr[0]))
            {
                if (pstRefreshInfo->u64ScreenAddr >=  pstRefreshInfo->stDispBufInfo.u64PhyAddr[0]
                    && pstRefreshInfo->u64ScreenAddr < pstRefreshInfo->stDispBufInfo.u64PhyAddr[1])
                {
                    pstRefreshInfo->stDispBufInfo.u32IndexForInt = 0;
                }
                else if ((pstRefreshInfo->u64ScreenAddr >= pstRefreshInfo->stDispBufInfo.u64PhyAddr[1])
                    && (pstRefreshInfo->u64ScreenAddr < (pstRefreshInfo->stDispBufInfo.u64PhyAddr[0] + hifb_get_smem_len(info))))
                {
                    pstRefreshInfo->stDispBufInfo.u32IndexForInt = 1;
                }
            }
        }
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        if (((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_DOUBLE)
           || (pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_FENCE))
#else
        if ((pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_DOUBLE)
#endif
            && (pstRefreshInfo->stDispBufInfo.bNeedFlip == HI_TRUE))
        {
            HI_U32 u32Index = pstRefreshInfo->stDispBufInfo.u32IndexForInt;

            // Work buf to change to free buf. Take free buf to display
            u32Index = 1 - u32Index;
            pstRefreshInfo->stDispBufInfo.u32IndexForInt = u32Index;
            /* The display address is set to the address of the free buf,
            which is set to the screen buf address differently from 0buf.*/
            s_stDrvOps.HIFB_DRV_SetLayerAddr(*pu32LayerId,
                pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index]);
            pstRefreshInfo->u64ScreenAddr = pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index];
            pstRefreshInfo->u64GBScreenAddr = pstRefreshInfo->u64ScreenAddr+u32BufSize/2;
            pstCompressInfo->bLayerAddrUpdate = HI_TRUE;
#ifdef __HuaweiLite__
            if (info->oinfo.stride != 0) {
                info->oinfo.sarea.y = (pstPar->stRefreshInfo.stDispBufInfo.u64PhyAddr[u32Index] - (HI_U32)(uintptr_t)info->oinfo.fbmem)
                    / info->oinfo.stride;
                if ((info->oinfo.bpp >> 3) != 0) {
                    info->oinfo.sarea.x = ((pstPar->stRefreshInfo.stDispBufInfo.u64PhyAddr[u32Index] - (HI_U32)(uintptr_t)info->oinfo.fbmem)
                        % info->oinfo.stride) / (info->oinfo.bpp >> 3);
                }
            }
#else
            info->var.yoffset = osal_div_u64((pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index] - hifb_get_smem_start(info)),
                hifb_get_line_length(info));
            if ((hifb_get_line_length(info) != 0) && ((hifb_get_bits_per_pixel(info)>> 3) != 0)) {
                info->var.xoffset = ((HI_UL)(pstRefreshInfo->stDispBufInfo.u64PhyAddr[u32Index] - hifb_get_smem_start(info))
                    % hifb_get_line_length(info))/(hifb_get_bits_per_pixel(info)>> 3);
            }
#endif
            pstRefreshInfo->stDispBufInfo.bFliped = HI_TRUE;
            pstRefreshInfo->stDispBufInfo.bNeedFlip = HI_FALSE;
            pstRefreshInfo->stDispBufInfo.u32IntPicNum++;
        }

        // HIFB_DRV_EnableLayer
        s_stDrvOps.HIFB_DRV_EnableLayer(*pu32LayerId, pstPar->bShow);
    }

    /* Decompression configuration */
    hifb_dcmp_config(*pu32LayerId);

    // HIFB_DRV_UpdataLayerReg
    s_stDrvOps.HIFB_DRV_UpdataLayerReg(*pu32LayerId);

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);

    // Field blanking mark
    pstPar->vblflag = 1;
    wake_up_interruptible(&(pstPar->vblEvent));

    return HI_SUCCESS;
}



/* Compression completion interrupt callback function */
static HI_S32 hifb_wbc_callback(const HI_VOID *pParaml, HI_VOID *pParamr)
{
    HI_U32 *pu32LayerId = NULL;
    struct fb_info *info = NULL;
    HIFB_PAR_S *pstPar = NULL;
    HIFB_SPIN_LOCK_FLAG lockflag;
    HI_BOOL bWbcStop;
    if (pParaml == NULL) {
        return HI_FAILURE;
    }
    pu32LayerId = (HI_U32 *)pParaml;
    if (*pu32LayerId >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    info = g_stLayer[*pu32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)(info->par);

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);

    s_stDrvOps.HIFB_DRV_GetWbcStopState(*pu32LayerId, &bWbcStop);
    if (bWbcStop)
    {
        HIFB_INFO("layer %d wbc autostop!\n", *pu32LayerId);
        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
        return HI_SUCCESS;
    }

    /* close compression */
    s_stDrvOps.HIFB_DRV_EnableWbc(*pu32LayerId, HI_FALSE);

    /* Switch to compressed display */
    hifb_SetDcmpInfo(pstPar);
    s_stDrvOps.HIFB_DRV_EnableDcmp(*pu32LayerId, HI_TRUE);
    s_stDrvOps.HIFB_DRV_UpdataLayerReg(*pu32LayerId);

    /* Clear compression area and compression flag */
    osal_memset((void*)&pstPar->stCompressInfo.stCompressRect, 0, sizeof(HIFB_RECT));

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    return HI_SUCCESS;
}

static HI_S32 hifb_refresh_0buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    struct fb_info *           info           = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &pstPar->stRefreshInfo;
    volatile
        HIFB_COMPRESS_INFO_S *     pstCompressInfo    = &pstPar->stCompressInfo;

    HIFB_SPIN_LOCK_FLAG        lockflag;
    HI_S32                     s32Ret;
    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock,lockflag);

    if (pstCompressInfo->bCompressOpen)
    {
        HIFB_ERROR("only HIFB_LAYER_BUF_DOUBLE or HIFB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
        HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
        return HI_FAILURE;
    }

    pstRefreshInfo->stDispBufInfo.bCompress = HI_FALSE;

    pstPar->bModifying = HI_TRUE;
    //modify by wxl : if change flush type between 2buffer and 0 buffer, the addr couldn't be changed.
    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    /* The graphic address is taken from the canvas of the user data and
    filled in the screen address of the refresh information.*/
    pstRefreshInfo->u64ScreenAddr = pstCanvasBuf->stCanvas.u64PhyAddr;

    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    pstRefreshInfo->stUserBuffer.stCanvas.u32Pitch = pstCanvasBuf->stCanvas.u32Pitch;

    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FMT;
    pstRefreshInfo->stUserBuffer.stCanvas.enFmt = pstCanvasBuf->stCanvas.enFmt;

    // NEW
    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DYNAMICRANGE;
    pstRefreshInfo->stUserBuffer.stCanvas.enDynamicRange= pstCanvasBuf->stCanvas.enDynamicRange;

    // NEW feature: zme
    pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);
    /* In hifb_disp_setdispsize, it is possible that kmalloc
    allocates memory in a non-atomic manner, so the lock must be released first. */
    s32Ret = hifb_disp_setdispsize(u32LayerId, pstCanvasBuf->stCanvas.u32Width,
                          pstCanvasBuf->stCanvas.u32Height);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock,lockflag);
    pstPar->bModifying = HI_FALSE;
    osal_memcpy(&(pstRefreshInfo->stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    if (pstPar->stCompressInfo.bCompressOpen)
    {
        osal_memcpy((void*)&pstPar->stCompressInfo.stUpdateRect, &pstCanvasBuf->UpdateRect, sizeof(HIFB_RECT));
        pstPar->stCompressInfo.bUpdateFinished = HI_TRUE;
    }


    pstPar->vblflag = 0;
    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock,lockflag);

    /*if the flag "FB_ACTIVATE_VBL" has been set, we should wait for register update finish*/
#ifdef __HuaweiLite__
    if(!in_atomic())
#else
    if(!in_atomic() && (info->var.activate & FB_ACTIVATE_VBL))
#endif
    {
        hifb_wait_regconfig_work(u32LayerId);
    }

    return HI_SUCCESS;
}

/*
* Name : hifb_refresh
* Desc : It is refreshed according to the canvas information and the layer's buf refresh mode.
         It is called indirectly when setting the layer attr.
* See  : references hifb_refresh_again,hifb_onrefresh
         calls hifb_refresh_0buf,hifb_refresh_1buf,hifb_refresh_2buf
*/
static HI_S32 hifb_refresh(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf, HIFB_LAYER_BUF_E enBufMode)
{
    HI_S32                     s32Ret         = HI_FAILURE;
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)g_stLayer[u32LayerId].pstInfo->par;
    HIFB_CURSOR_INFO_S *       pstCursorInfo  = &pstPar->stCursorInfo;
    HI_BOOL                    bOverlay       = HI_FALSE; /*is the cusor overlay with refresh area*/
    HIFB_RECT                  rcCursor;

    if (pstCanvasBuf == NULL) {
        return HI_FAILURE;
    }

    if (pstCursorInfo->bAttched)
    {
        HIFB_PAR_S *pstCursorPar = (HIFB_PAR_S *)g_stLayer[pstPar->stCursorInfo.u32AttachedCursorId].pstInfo->par;

        rcCursor.x = pstCursorPar->stDisplayInfo.stPos.s32XPos -
                 pstCursorPar->stCursorInfo.stCursor.stHotPos.s32XPos;
        rcCursor.y = pstCursorPar->stDisplayInfo.stPos.s32YPos -
                 pstCursorPar->stCursorInfo.stCursor.stHotPos.s32YPos;
        rcCursor.w = pstCursorPar->stCursorInfo.stCursor.stCursor.u32Width;
        rcCursor.h = pstCursorPar->stCursorInfo.stCursor.stCursor.u32Height;

        /* check the cusor overlay with refresh area*/
        if (pstCursorPar->bShow &&
            (((rcCursor.x >= pstCanvasBuf->UpdateRect.x &&
              rcCursor.x <=  pstCanvasBuf->UpdateRect.x + pstCanvasBuf->UpdateRect.w)) ||
             (rcCursor.x < pstCanvasBuf->UpdateRect.x &&
              rcCursor.x + rcCursor.w >=  pstCanvasBuf->UpdateRect.x)))
        {
            if (((rcCursor.y >= pstCanvasBuf->UpdateRect.y &&
                  rcCursor.y <=  pstCanvasBuf->UpdateRect.y + pstCanvasBuf->UpdateRect.h)) ||
                (rcCursor.y < pstCanvasBuf->UpdateRect.y &&
                 rcCursor.y + rcCursor.h >=  pstCanvasBuf->UpdateRect.y))
            {
                bOverlay = HI_TRUE;
            }
        }
    }
#ifdef CURSOR
    if (bOverlay && IS_SOFT_CURSOR())
    {
        hifb_cursor_hide(u32LayerId);
    }
#endif
    switch (enBufMode)
    {
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        case HIFB_LAYER_BUF_FENCE:
             s32Ret = hifb_refresh_2buf_fence(u32LayerId, pstCanvasBuf);
             break;
#endif
        case HIFB_LAYER_BUF_DOUBLE:
            s32Ret = hifb_refresh_2buf(u32LayerId, pstCanvasBuf);
            break;

        case HIFB_LAYER_BUF_ONE:
            s32Ret = hifb_refresh_1buf(u32LayerId, pstCanvasBuf);
            break;

        case HIFB_LAYER_BUF_NONE:
           s32Ret = hifb_refresh_0buf(u32LayerId, pstCanvasBuf);
           break;

        case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
            s32Ret = hifb_refresh_2buf_immediate_display(u32LayerId, pstCanvasBuf);
            break;

        default:
            break;
    }
#ifdef CURSOR
    if (bOverlay && IS_SOFT_CURSOR())
    {
        hifb_cursor_show(u32LayerId);
    }
#endif
    return s32Ret;
}

#ifdef __HuaweiLite__
static void hifb_free(struct hifb_info *info)
{
    if (info != NULL) {
        free(info);
    }
}

static struct hifb_info *hifb_alloc(size_t size)
{
    HI_S32 hifb_info_len = sizeof(struct hifb_info);
    struct hifb_info *info = NULL;

    hifb_info_len = ALIGN(hifb_info_len, sizeof(unsigned long));

    info = (struct hifb_info *)malloc(hifb_info_len + size);
    if (info == HI_NULL) {
        return NULL;
    }
    osal_memset(info, 0, (hifb_info_len + size));

    info->par = (void *)((char *)info + hifb_info_len);

    return info;
}
#endif



static HI_S32 hifb_alloccanbuf(struct fb_info *info, HIFB_LAYER_INFO_S * pLayerInfo)
{
    HIFB_PAR_S *               par            = NULL;
    HIFB_SURFACE_S *           pstCanvasSur   = NULL;

    if ((info == NULL) || (pLayerInfo == NULL)) {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;
    pstCanvasSur = &par->CanvasSur;

    if (!(pLayerInfo->u32Mask & HIFB_LAYERMASK_CANVASSIZE))
    {
        return HI_SUCCESS;
    }

    /** if  with old canvas buffer*/
    if (pstCanvasSur->u64PhyAddr)
    {
        /* if  old is the same with new , then return, else free the old buffer*/
        if ((pLayerInfo->u32CanvasWidth == pstCanvasSur->u32Width) &&
             (pLayerInfo->u32CanvasHeight == pstCanvasSur->u32Height))
        {
            HIFB_INFO("mem size is the same , no need alloc new memory");
            return HI_SUCCESS;
        }

        /** free new old buffer*/
        HIFB_INFO("free old canvas buffer\n");
        hifb_freeccanbuf(par);
    }

    /** new canvas buffer*/
    if ((pLayerInfo->u32CanvasWidth > 0) && (pLayerInfo->u32CanvasHeight > 0))
    {
        HI_U32 u32LayerSize;
        HI_U32 u32Pitch;
        HI_CHAR *pBuf = NULL;
        HI_CHAR name[16];
        struct fb_info * infoTemp = NULL;
        /* Applying fb_info without private data is not for registration,
        but for the convenience of passing to hifb_memset to empty the memory. */
#ifdef __HuaweiLite__
        infoTemp = hifb_alloc(0);
#else
        infoTemp = framebuffer_alloc(0, NULL);
#endif
        if(NULL == infoTemp)
        {
            return HI_FAILURE;
        }

        /*16 bytes alignment*/
        u32Pitch = ((pLayerInfo->u32CanvasWidth * hifb_get_bits_per_pixel(info) >> 3) + 15)>>4;
        u32Pitch = u32Pitch << 4;

        u32LayerSize = u32Pitch * pLayerInfo->u32CanvasHeight;
        /** alloc new buffer*/
        osal_snprintf(name, sizeof(name), "hifb_canvas%u", par->u32LayerID);
        pstCanvasSur->u64PhyAddr = hifb_buf_allocmem(name, u32LayerSize,
            apszLayerMmzNames[par->u32LayerID]);
        if (pstCanvasSur->u64PhyAddr == 0)
        {
            HIFB_ERROR("alloc canvas buffer no mem");
#ifdef __HuaweiLite__
            hifb_free(infoTemp);
#else
            framebuffer_release(infoTemp);
#endif
            return HI_FAILURE;
        }

        pBuf = (HI_CHAR *)hifb_buf_map(pstCanvasSur->u64PhyAddr, u32LayerSize);
        if (pBuf == HI_NULL)
        {
            HIFB_ERROR("map canvas buffer failed!\n");
            hifb_buf_freemem(pstCanvasSur->u64PhyAddr);
#ifdef __HuaweiLite__
            hifb_free(infoTemp);
#else
            framebuffer_release(infoTemp);
#endif
            return HI_FAILURE;
        }
        /* hifb_memset is faster than memset */
#ifdef __HuaweiLite__
         infoTemp->oinfo.fbmem = (uintptr_t)pstCanvasSur->u64PhyAddr;
        osal_memset(infoTemp->oinfo.fbmem,0x0,u32LayerSize);
#else

        infoTemp->fix.smem_start = pstCanvasSur->u64PhyAddr;
        infoTemp->screen_base = pBuf;
        hifb_memset(infoTemp, 0, u32LayerSize);
#endif
        hifb_buf_ummap(pBuf);

        HIFB_INFO("alloc new memory for canvas buffer success\n");
        pstCanvasSur->u32Width = pLayerInfo->u32CanvasWidth;
        pstCanvasSur->u32Height = pLayerInfo->u32CanvasHeight;
        pstCanvasSur->u32Pitch = u32Pitch;
#ifdef __HuaweiLite__
        par->CanvasSur.enFmt =  info->vinfo.fmt;
        hifb_free(infoTemp);
#else
        pstCanvasSur->enFmt =  hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp, info->var.bits_per_pixel);
        framebuffer_release(infoTemp);
#endif
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

static HI_S32 hifb_onrefresh(HIFB_PAR_S* par, HI_VOID __user *argp)
{
    HI_S32                     s32Ret;
    HIFB_BUFFER_S              stCanvasBuf;
    HIFB_LAYER_BUF_E           enBufMode;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &par->stDisplayInfo;
    HIFB_SPIN_LOCK_FLAG        lockflag;

    if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
    {
        HIFB_WARNING("you shouldn't refresh cursor layer!\n");
        return HI_SUCCESS;
    }

    if (argp == NULL) {
        HIFB_ERROR("NULL arg!\n");
        return -EINVAL;
    }

    if (copy_from_user(&stCanvasBuf, argp, sizeof(HIFB_BUFFER_S)))
    {
        return -EFAULT;
    }

    if (stCanvasBuf.stCanvas.enFmt >= HIFB_FMT_BUTT) {
        return HI_FAILURE;
    }

    // Check the legality of physical address and size
    if(HI_FAILURE == hifb_check_phyaddr(&stCanvasBuf))
    {
        HIFB_ERROR("mmz phy addr 0x%llx invalid.\n",stCanvasBuf.stCanvas.u64PhyAddr);
        return HI_FAILURE;
    }

    hifb_get_bufmode(par, &enBufMode);

    if (HIFB_LAYER_BUF_BUTT == enBufMode)
    {
        HIFB_ERROR("doesn't support FBIO_REFRESH operation when refresh mode is HIFB_LAYER_BUF_BUTT!\n");
        return HI_FAILURE;
    }

    if ((stCanvasBuf.UpdateRect.x >=  stCanvasBuf.stCanvas.u32Width)
        || (stCanvasBuf.UpdateRect.y >= stCanvasBuf.stCanvas.u32Height)
        || (stCanvasBuf.UpdateRect.w <= 0) || (stCanvasBuf.UpdateRect.h <= 0))
    {
        HIFB_ERROR("rect error: update rect:(%d,%d,%d,%d), canvas range:(%d,%d)\n",
                  stCanvasBuf.UpdateRect.x, stCanvasBuf.UpdateRect.y,
                  stCanvasBuf.UpdateRect.w, stCanvasBuf.UpdateRect.h,
                  stCanvasBuf.stCanvas.u32Width, stCanvasBuf.stCanvas.u32Height);
        return HI_FAILURE;
    }

    if (stCanvasBuf.UpdateRect.x + stCanvasBuf.UpdateRect.w > stCanvasBuf.stCanvas.u32Width)
    {
        stCanvasBuf.UpdateRect.w = stCanvasBuf.stCanvas.u32Width - stCanvasBuf.UpdateRect.x;
    }

    if (stCanvasBuf.UpdateRect.y + stCanvasBuf.UpdateRect.h > stCanvasBuf.stCanvas.u32Height)
    {
        stCanvasBuf.UpdateRect.h =  stCanvasBuf.stCanvas.u32Height - stCanvasBuf.UpdateRect.y;
    }
    if (HIFB_LAYER_BUF_NONE == enBufMode)
    {
        if (stCanvasBuf.stCanvas.enFmt >= HIFB_FMT_BUTT)
        {
            HIFB_ERROR("The Canvas Format is long, the value is %d\n", stCanvasBuf.stCanvas.enFmt);
            return HI_FAILURE;
        }
        /* Check if the format of the canvas supported or not by HiFB*/
        if (!s_stDrvOps.pstCapability[par->u32LayerID].bColFmt[stCanvasBuf.stCanvas.enFmt])
        {
            HIFB_ERROR("Unsupported PIXEL FORMAT!\n");
            return -EINVAL;
        }
        /* there's a limit from hardware that the start address of screen buf
              should be 16byte aligned! */
        HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
        if ((stCanvasBuf.stCanvas.u64PhyAddr & 0xf) || (stCanvasBuf.stCanvas.u32Pitch & 0xf))
        {
            HIFB_ERROR("addr 0x%llx or pitch: 0x%x is not 16 bytes align !\n",
                stCanvasBuf.stCanvas.u64PhyAddr,
                stCanvasBuf.stCanvas.u32Pitch);
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);
            return HI_FAILURE;
        }
        HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);
    }
    else
    {
        /* Check if the format of the canvas and the format of
        HiFB belong to the category that TDE can support. */
        if ((!hifb_TdeIsSupportFmt(stCanvasBuf.stCanvas.enFmt))
            || (!hifb_TdeIsSupportFmt(par->enColFmt)))
        {
            HIFB_ERROR("Unsupported PIXEL FORMAT!CanvasFmt:%d,par->enColFmt:%d\n",stCanvasBuf.stCanvas.enFmt,par->enColFmt);
            return -EINVAL;
        }
    }

    if (HIFB_LAYER_BUF_NONE == enBufMode && (HIFB_MIRROR_NONE != pstDisplayInfo->enMirrorMode))
    {
        HIFB_ERROR("Can't do mirror when the layer buf is none!\n");
        return -EINVAL;
    }

    if (HIFB_LAYER_BUF_NONE == enBufMode && (HIFB_ROTATE_NONE != pstDisplayInfo->enRotateMode))
    {
        HIFB_ERROR("Can't do rotate when the layer buf is none!\n");
        return -EINVAL;
    }

    s32Ret = hifb_refresh(par->u32LayerID, &stCanvasBuf, enBufMode);

    return s32Ret;
}

static HI_S32 hifb_set_mirrormode(HIFB_PAR_S *pstPar, HIFB_MIRROR_MODE_E enMirrorMode)
{
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo     = &pstPar->stDisplayInfo;
    volatile HIFB_COMPRESS_INFO_S *     pstCompressInfo    = &pstPar->stCompressInfo;
    HIFB_LAYER_BUF_E           enBufMode;

    hifb_get_bufmode(pstPar, &enBufMode);

    if (HIFB_LAYER_BUF_BUTT == enBufMode || HIFB_LAYER_BUF_NONE == enBufMode )
    {
        HIFB_ERROR("doesn't support FBIOPUT_MIRROR_MODE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }

    if (enMirrorMode >= HIFB_MIRROR_BUTT)
    {
        HIFB_ERROR("The input mirror mode is wrong!\n");
        return -EINVAL;
    }

    if((HIFB_MIRROR_NONE != enMirrorMode)
        && (HIFB_ROTATE_NONE != pstDisplayInfo->enRotateMode))
    {
        HIFB_ERROR("Can't do mirror when rotate!\n");
        return -EINVAL;
    }

    if((HIFB_MIRROR_NONE != enMirrorMode) && (HI_TRUE == pstCompressInfo->bCompressOpen))
    {
        HIFB_ERROR("Can't do mirror when compression is on!\n");
        return -EINVAL;
    }

    pstDisplayInfo->enMirrorMode = enMirrorMode;
    return HI_SUCCESS;

}

static HI_S32 hifb_set_rotatemode(struct fb_info *info, HIFB_ROTATE_MODE_E enRotateMode)
{
    HIFB_PAR_S *               pstPar         = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &pstPar->stDisplayInfo;
    HI_U32                     u32Size        = 0;
    HI_VOID *                  pMmzName       = HI_NULL;
    HI_U64*                    pu64VirRotateVb= HI_NULL;
    HIFB_LAYER_BUF_E           enBufMode;
    HI_CHAR                    name[HIFB_ROTBUF_NAME_LEN];
    volatile
        HIFB_COMPRESS_INFO_S * pstCompressInfo  = &pstPar->stCompressInfo;

    if(!HIFB_DRV_SupportRotate())
    {
        HIFB_ERROR("can't support rotate\n!\n");
        return -EINVAL;
    }
    hifb_get_bufmode(pstPar, &enBufMode);

    if (HIFB_LAYER_BUF_BUTT == enBufMode || HIFB_LAYER_BUF_NONE == enBufMode )
    {
        HIFB_ERROR("doesn't support FBIOPUT_ROTATE_MODE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }

    if((HIFB_FMT_ARGB4444 != pstPar->enColFmt)
    && (HIFB_FMT_ARGB1555 != pstPar->enColFmt)
    && (HIFB_FMT_ARGB8888 != pstPar->enColFmt))
    {
        HIFB_ERROR("The rotate mode only support HIFB_FMT_ARGB4444 and HIFB_FMT_ARGB1555 which is %d\n!\n",pstPar->enColFmt);
        return -EINVAL;
    }

    if (enRotateMode >= HIFB_ROTATE_BUTT)
    {
        HIFB_ERROR("The input rotate mode is wrong!\n");
        return -EINVAL;
    }

    if (pstCompressInfo->bCompressOpen && HIFB_ROTATE_NONE != enRotateMode)
    {
        HIFB_ERROR("Can't rotate when in compress mode!\n");
        return -EINVAL;
    }

    if((HIFB_ROTATE_NONE != enRotateMode)
        && (HIFB_MIRROR_NONE != pstDisplayInfo->enMirrorMode))
    {
        HIFB_ERROR("Can't do rotate when mirror!\n");
        return -EINVAL;
    }

    if((HIFB_ROTATE_90 == enRotateMode
        || HIFB_ROTATE_270 == enRotateMode)
        && (0 == pstPar->u64RotateVb))
    {
        hi_mpp_chn stChn;
        stChn.mod_id  = HI_ID_FB;
        stChn.dev_id = pstPar->u32LayerID;
        stChn.chn_id = 0;

        u32Size = hifb_get_line_length(info) * (((pstDisplayInfo->u32DisplayHeight + 15)/16)*16);

        if (FUNC_ENTRY(sys_export_func, HI_ID_SYS)->pfn_get_mmz_name(&stChn,&pMmzName))
        {
            HIFB_ERROR("get mmz name fail!\n");
            return ENOMEM;
        }

        osal_snprintf(name, sizeof(name), "hifb%d_rotate", pstPar->u32LayerID);
        pstPar->u64RotateVb = cmpi_mmz_malloc(pMmzName, name, u32Size);
        if (0 == pstPar->u64RotateVb )
        {
            HIFB_ERROR("get buffer fail,size %d !\n",u32Size);
            return ENOMEM;
        }

        pu64VirRotateVb = cmpi_remap_nocache(pstPar->u64RotateVb, u32Size);
        if (NULL == pu64VirRotateVb )
        {
            HIFB_ERROR("get buffer fail,size %d !\n",u32Size);
            cmpi_mmz_free(pstPar->u64RotateVb, NULL);
            return ENOMEM;
        }

        osal_memset(pu64VirRotateVb, 0x0, u32Size);

        cmpi_unmap(pu64VirRotateVb);

    }

    pstDisplayInfo->enRotateMode = enRotateMode;

    return HI_SUCCESS;

}

/*
* Name : hifb_check_layerinfo
* Desc :check layer information: buf refresh mode,position,canvas width
        and height, display width and height, screen width and height.
*/
static HI_S32 hifb_check_layerinfo(HIFB_LAYER_INFO_S* pstLayerInfo)
{
    if (pstLayerInfo == NULL) {
        return HI_FAILURE;
    }

    if (pstLayerInfo->u32Mask & HIFB_LAYERMASK_BUFMODE)
    {
#ifdef CONFIG_HIFB_FENCE_SUPPORT
        if (pstLayerInfo->BufMode > HIFB_LAYER_BUF_FENCE)
#else
        if (pstLayerInfo->BufMode > HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
#endif
        {
#ifdef CONFIG_HIFB_FENCE_SUPPORT
            HIFB_ERROR("BufMode(%d) is error, should between %d and %d\n", pstLayerInfo->BufMode, HIFB_LAYER_BUF_DOUBLE, HIFB_LAYER_BUF_FENCE);
#else
            HIFB_ERROR("BufMode(%d) is error, should between %d and %d\n", pstLayerInfo->BufMode, HIFB_LAYER_BUF_DOUBLE, HIFB_LAYER_BUF_DOUBLE_IMMEDIATE);
#endif
            return HI_FAILURE;
        }
    }

    // Detection of anti-flicker mode
    if (pstLayerInfo->u32Mask & HIFB_LAYERMASK_ANTIFLICKER_MODE)
    {
        if (pstLayerInfo->eAntiflickerLevel > HIFB_LAYER_ANTIFLICKER_AUTO)
        {
            HIFB_ERROR("eAntiflickerLevel(%d) is error, should between %d and %d\n",
                pstLayerInfo->eAntiflickerLevel, HIFB_LAYER_ANTIFLICKER_NONE, HIFB_LAYER_ANTIFLICKER_AUTO);
            return HI_FAILURE;
        }
    }

    // check the width and height
    if (pstLayerInfo->u32Mask & HIFB_LAYERMASK_DISPSIZE)
    {
        if (pstLayerInfo->u32DisplayWidth % 2 || pstLayerInfo->u32DisplayHeight % 2)
        {
            HIFB_ERROR("Disaplay W(%u) and H(%u) should align to 2!\n",
                pstLayerInfo->u32DisplayWidth, pstLayerInfo->u32DisplayHeight);
            return HI_FAILURE;
        }
    }

    if (pstLayerInfo->u32Mask & HIFB_LAYERMASK_SCREENSIZE)
    {
        if (pstLayerInfo->u32ScreenWidth % 2 || pstLayerInfo->u32ScreenHeight % 2)
        {
            HIFB_ERROR("Screenaplay W(%u) and H(%u) should align to 2!\n",
                pstLayerInfo->u32ScreenWidth, pstLayerInfo->u32ScreenHeight);
            return HI_FAILURE;
        }
    }


    // check pre-multiplier value.
    if (pstLayerInfo->u32Mask & HIFB_LAYERMASK_BMUL)
    {
        if (HI_TRUE != pstLayerInfo->bPreMul && HI_FALSE != pstLayerInfo->bPreMul)
        {
            HIFB_ERROR("pstLayerInfo->bPreMul should be HI_TRUE or HI_FALSE but it is %d\n", pstLayerInfo->bPreMul);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;

}

static HI_S32 hifb_onputlayerinfo(struct fb_info *info, HIFB_PAR_S* par, const HI_VOID __user *argp)
{
    HI_S32                     s32Ret         = HI_SUCCESS;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo = &par->stDisplayInfo;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo = &par->stRefreshInfo;
    HIFB_LAYER_INFO_S          stLayerInfo;
    HI_U32                     u32Pitch;
    HIFB_SPIN_LOCK_FLAG        lockflag;
    HIFB_POINT_S               stPos;
    HI_BOOL                    bInterlace;
    volatile HIFB_COMPRESS_INFO_S* pstCompressInfo = &par->stCompressInfo;

    if (IS_CURSOR_LAYER(par->u32LayerID) && IS_SOFT_CURSOR())
    {
       HIFB_WARNING("you shouldn't put cursor layer info!");
       return HI_SUCCESS;
    }

    if (argp == NULL) {
        HIFB_ERROR("NULL arg!\n");
        return -EINVAL;
    }

    if (copy_from_user(&stLayerInfo, argp, sizeof(HIFB_LAYER_INFO_S)))
    {
       return -EFAULT;
    }

    s32Ret = hifb_check_layerinfo(&stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    // s_astHifbGfxCap
    if (!s_stDrvOps.pstCapability[par->u32LayerID].bVoScale)
    {
        /* If the chip does not support scaling, if both the display size
        and the screen size are set, both must be equal. */
        if ((stLayerInfo.u32Mask & HIFB_LAYERMASK_DISPSIZE)
            && (stLayerInfo.u32Mask & HIFB_LAYERMASK_SCREENSIZE))
        {
            if ((stLayerInfo.u32DisplayWidth != stLayerInfo.u32ScreenWidth)
                || (stLayerInfo.u32DisplayHeight!= stLayerInfo.u32ScreenHeight))
            {
                HIFB_ERROR("DisplaySize(%d, %d) can't be different with ScreenSize(%d, %d) \
                    when layer %d don't scale\n", stLayerInfo.u32DisplayWidth, stLayerInfo.u32DisplayHeight,
                    stLayerInfo.u32ScreenWidth, stLayerInfo.u32ScreenHeight, par->u32LayerID);
                return HI_FAILURE;
            }
        }
    }

    bInterlace = hifb_is_interlace(par);
    // Modify the display size, the memory size has changed, limited by the size of the memory
    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_DISPSIZE)
    {
        u32Pitch = stLayerInfo.u32DisplayWidth * hifb_get_bits_per_pixel(info) >> 3;
        u32Pitch = (u32Pitch + 0xf) & 0xfffffff0;
        if(HI_FAILURE == hifb_checkmem_enough(info, u32Pitch, stLayerInfo.u32DisplayHeight))
        {
            HIFB_ERROR("memory is not enough!\n");
            return HI_FAILURE;
        }

        if (stLayerInfo.u32DisplayWidth == 0 || stLayerInfo.u32DisplayHeight == 0)
        {
            HIFB_ERROR("display witdh/height shouldn't be 0!\n");
            return HI_FAILURE;
        }
        /* For interlaced output, the height of the layer must be even.
        Progressive output without this limit. */
        if (bInterlace && (0 != (stLayerInfo.u32DisplayHeight % 2)))
        {
            HIFB_ERROR("u32DisplayHeight(%d) of u32LayerId %d should be even when vodev output is interlace\n",\
                stLayerInfo.u32DisplayHeight, par->u32LayerID);
            return HI_FAILURE;
        }
    }

    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_CANVASSIZE)
    {
       if ((stLayerInfo.u32CanvasWidth == 0) || (stLayerInfo.u32CanvasHeight == 0))
       {
           HIFB_ERROR("canvas width/height shouldn't be 0\n");
           return HI_FAILURE;
       }
    }

    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_SCREENSIZE)
    {
       if ((stLayerInfo.u32ScreenWidth == 0) || (stLayerInfo.u32ScreenHeight == 0))
       {
           HIFB_ERROR("screen width/height shouldn't be 0\n");
           return HI_FAILURE;
       }
       /* For interlaced output, the height of the layer must be even.
        Progressive output without this limit. */
       if (bInterlace && (0 != (stLayerInfo.u32ScreenHeight % 2)))
       {
           HIFB_ERROR("u32ScreenHeight(%d) of u32LayerId %d should be even when vodev output is interlace\n",\
               stLayerInfo.u32ScreenHeight, par->u32LayerID);
           return HI_FAILURE;
       }
    }

    // Modify the display buf mode, the memory size has changed, limited by the size of the memory
    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_BUFMODE)
    {
        HI_U32 u32LayerSize;

        if (stLayerInfo.BufMode == HIFB_LAYER_BUF_ONE)
        {
            u32LayerSize = hifb_get_line_length(info) * hifb_get_yres(info);
        }
       else if
#ifdef CONFIG_HIFB_FENCE_SUPPORT
       ((stLayerInfo.BufMode == HIFB_LAYER_BUF_DOUBLE)
           || (stLayerInfo.BufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
           || (stLayerInfo.BufMode == HIFB_LAYER_BUF_FENCE))
#else
        ((stLayerInfo.BufMode == HIFB_LAYER_BUF_DOUBLE)
        || (stLayerInfo.BufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE))
#endif
        {
            u32LayerSize = 2 * hifb_get_line_length(info) * hifb_get_yres(info);
        }
        else
        {
            u32LayerSize = 0;
        }

        if (u32LayerSize > hifb_get_smem_len(info))
        {
            HIFB_ERROR("No enough mem! layer real memory size:%d KBytes, expected:%d KBtyes\n",
                       hifb_get_smem_len(info) / 1024, u32LayerSize / 1024);
            return HI_FAILURE;
        }

        /* If compression is enabled, only 2buf mode can be set. */
        if (pstCompressInfo->bCompressOpen)
        {
#ifdef CONFIG_HIFB_FENCE_SUPPORT
            if (HIFB_LAYER_BUF_DOUBLE != stLayerInfo.BufMode && HIFB_LAYER_BUF_DOUBLE_IMMEDIATE != stLayerInfo.BufMode
            && HIFB_LAYER_BUF_FENCE != stLayerInfo.BufMode)
#else
            if (HIFB_LAYER_BUF_DOUBLE != stLayerInfo.BufMode && HIFB_LAYER_BUF_DOUBLE_IMMEDIATE != stLayerInfo.BufMode)
#endif
            {
                HIFB_ERROR("only HIFB_LAYER_BUF_DOUBLE or HIFB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
                return HI_FAILURE;
            }
        }
    }

    /*if x>width or y>height ,how to deal with: see nothing in screen or return failure. */
    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_POS)
    {
        if ((stLayerInfo.s32XPos < 0) || (stLayerInfo.s32YPos < 0))
        {
            HIFB_ERROR("It's not supported to set start pos of layer to negative!\n");
            return HI_FAILURE;
        }
        /* For interlaced output, the start of the layer must be even.
        Progressive output without this limit */
        if (bInterlace && (0 != stLayerInfo.s32YPos % 2))
        {
            HIFB_ERROR("s32YPos should be even for interlace vodev!\n");
            return HI_FAILURE;
        }
    }

    // Limit the pre-multiplication and color key.
    if ((stLayerInfo.u32Mask & HIFB_LAYERMASK_BMUL) && par->stCkey.bKeyEnable)
    {
       HIFB_ERROR("Colorkey and premul couldn't take effect at same time!\n");
       return HI_FAILURE;
    }

    // return HI_FAILURE, not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444
    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_BMUL)
    {
        if((HI_TRUE== stLayerInfo.bPreMul) &&
            (HIFB_FMT_ARGB4444 == par->enColFmt || HIFB_FMT_ARGB1555 == par->enColFmt))
        {
           HIFB_ERROR("not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
           return HI_FAILURE;
        }
    }
    // not allow to set pre-mult mode when the GlobalAlpha is 1
    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_BMUL)
    {
        if((HI_TRUE== stLayerInfo.bPreMul) &&  (HI_TRUE == par->stAlpha.bAlphaChannel && 1 ==par->stAlpha.u8GlobalAlpha))
        {
           HIFB_ERROR("not allow to set pre-mult mode when the GlobalAlpha is 1\n");
           return HI_FAILURE;
        }
    }

    /*avoid modifying register in vo isr before all params has been recorded! In vo irq,
       flag bModifying will be checked.*/
    HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
    par->bModifying = HI_TRUE;

    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_BMUL)
    {
        pstDisplayInfo->bPreMul = stLayerInfo.bPreMul;
        par->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_BMUL;
    }

    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_BUFMODE)
    {
        hifb_set_bufmode(par->u32LayerID, stLayerInfo.BufMode);
    }

    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_POS)
    {
        stPos.s32XPos = stLayerInfo.s32XPos;
        stPos.s32YPos = stLayerInfo.s32YPos;
        hifb_set_layerpos(par, &stPos);
    }

    if (stLayerInfo.u32Mask & HIFB_LAYERMASK_ANTIFLICKER_MODE)
    {
        hifb_set_antiflickerlevel(par->u32LayerID, stLayerInfo.eAntiflickerLevel);
    }

   /**  set screen size in the scaling mode*/
   // s_astHifbGfxCap
    if (s_stDrvOps.pstCapability[par->u32LayerID].bVoScale)
    {
         HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock,lockflag);
         /* The following two functions have a sleep operation inside,
         you must unlock before calling, and lock the global amount inside the function. */
         if (stLayerInfo.u32Mask & HIFB_LAYERMASK_SCREENSIZE)
         {
             s32Ret = hifb_disp_setscreensize(par->u32LayerID, stLayerInfo.u32ScreenWidth, stLayerInfo.u32ScreenHeight);
             if (HI_SUCCESS != s32Ret)
             {
                 return s32Ret;
             }
         }
         if (stLayerInfo.u32Mask & HIFB_LAYERMASK_DISPSIZE)
         {
             s32Ret = hifb_disp_setdispsize(par->u32LayerID, stLayerInfo.u32DisplayWidth, stLayerInfo.u32DisplayHeight);
             if (HI_SUCCESS != s32Ret)
             {
                 return s32Ret;
             }
         }
         HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
    }
    else
    {
        /* no scaling mode, no buffer mode, screen size and display size can be set */
        if(pstRefreshInfo->enBufMode != HIFB_LAYER_BUF_NONE)
        {
            HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);
            if (stLayerInfo.u32Mask & HIFB_LAYERMASK_SCREENSIZE)
            {
                s32Ret = hifb_disp_setscreensize(par->u32LayerID, stLayerInfo.u32ScreenWidth, stLayerInfo.u32ScreenHeight);
                if (HI_SUCCESS != s32Ret)
                {
                    return s32Ret;
                }
            }
            if (stLayerInfo.u32Mask & HIFB_LAYERMASK_DISPSIZE)
            {
                s32Ret = hifb_disp_setdispsize(par->u32LayerID, stLayerInfo.u32DisplayWidth, stLayerInfo.u32DisplayHeight);
                if (HI_SUCCESS != s32Ret)
                {
                    return s32Ret;
                }
            }
            HIFB_SPIN_LOCK_IRQSAVE(&par->lock, lockflag);
        }
    }

    par->bModifying = HI_FALSE;
    HIFB_SPIN_UNLOCK_IRQRESTORE(&par->lock, lockflag);

    s32Ret = hifb_alloccanbuf(info, &stLayerInfo);
    if (s32Ret != HI_SUCCESS)
    {
        /* There is no error returned here, because the user can also
           specify this memory; in addition, even if the allocation is successful,
           The user also needs to call FBIOGET_CANVAS_BUFFER to get it to operate. */
        HIFB_WARNING("alloc canvas buffer failed\n");
    }

    hifb_refresh_again(par->u32LayerID);

    return s32Ret;
}

static HI_S32 hifb_dosetcolreg(unsigned regno, unsigned red, unsigned green,
                          unsigned blue, unsigned transp, struct fb_info *info, HI_BOOL bUpdateFinishedReg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    HI_U32 argb = ((transp & 0xff) << 24) | ((red & 0xff) << 16) | ((green & 0xff) << 8) | (blue & 0xff);

    if (regno > 255)
    {
        HIFB_WARNING("regno: %d, larger than 255!\n", regno);
        return HI_FAILURE;
    }

    s_stDrvOps.HIFB_DRV_SetColorReg(par->u32LayerID, regno, argb, bUpdateFinishedReg);
    return HI_SUCCESS;
}

static HI_S32 hifb_setcolreg(unsigned regno, unsigned red, unsigned green,
                          unsigned blue, unsigned transp, struct fb_info *info)
{
    return hifb_dosetcolreg(regno, red, green, blue, transp, info, HI_TRUE);
}

static HI_S32 hifb_setcmap(struct fb_cmap *cmap, struct fb_info *info)
{
    HI_S32 i, start;
    unsigned short *red, *green, *blue, *transp;
    unsigned short hred, hgreen, hblue, htransp = 0xffff;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (IS_CURSOR_LAYER(par->u32LayerID))
    {
        return -EINVAL;
    }

    if (!s_stDrvOps.pstCapability[par->u32LayerID].bCmap)
    {
        /* AE6D03519, delete this color map warning! */
        return -EPERM;
    }

    red    = cmap->red;
    green  = cmap->green;
    blue   = cmap->blue;
    transp = cmap->transp;

#ifdef __HuaweiLite__
    start  = cmap->first;
#else
    start  = cmap->start;
#endif
    for (i = 0; i < cmap->len; i++)
    {
        hred   = *red++;
        hgreen = *green++;
        hblue  = *blue++;
        if (transp)
        {
            htransp = *transp++;
        }

        if (i < cmap->len - 1)
        {
            if (hifb_dosetcolreg(start++, hred, hgreen, hblue, htransp, info, HI_FALSE))
            {
                break;
            }
        }
        else
        {   /*the last time update register*/
            if (hifb_dosetcolreg(start++, hred, hgreen, hblue, htransp, info, HI_TRUE))
            {
                break;
            }
        }

    }

    return 0;
}
#ifdef CONFIG_FB_CFB_IMAGEBLIT
static HI_VOID hifb_imageblit(struct fb_info *p, const struct fb_image *image)
{
    cfb_imageblit(p, image);
}
#endif


#ifdef __HuaweiLite__
#define HIFB_MAX_LAYER_NUM  1
static struct hifb_info *g_hifb[HIFB_MAX_LAYER_NUM] = {NULL};

static int fb_copy_cmap(const struct fb_cmap_s *src, struct fb_cmap_s *dst)
{
    int offset = 0;
    int copy_size;
    uint8_t *cmap_start = NULL;
    int cmap_size;

    if ((src == NULL) || (dst == NULL)) {
        return -EINVAL;
    }

    cmap_start = dst->red;
    if (NULL == cmap_start)
        return -EINVAL;

    cmap_size = dst->len * sizeof(u_short);

    if(src->first > dst->first ){
        offset = src->first - dst->first;
        copy_size = (src->len > (dst->len-offset)) ? (dst->len - offset) : src->len;

        if (copy_size <= 0)
            return -EINVAL;
        copy_size *= sizeof(u_short);

        osal_memcpy(cmap_start + offset, src->red, copy_size);
        cmap_start += cmap_size;
        osal_memcpy(cmap_start + offset, src->green, copy_size);
        cmap_start += cmap_size;
        osal_memcpy(cmap_start + offset, src->blue, copy_size);
        cmap_start += cmap_size;
        if (src->transp && dst->transp)
            osal_memcpy(cmap_start + offset, src->transp, copy_size);
    }
    else
    {
        offset = dst->first - src->first;
        copy_size = ((src->len - offset) > dst->len) ? dst->len : src->len - offset;

        if (copy_size <= 0)
            return -EINVAL;
        copy_size *= sizeof(u_short);

        osal_memcpy(cmap_start, src->red + offset, copy_size);
        cmap_start += cmap_size;
        osal_memcpy(cmap_start, src->green + offset, copy_size);
        cmap_start += cmap_size;
        osal_memcpy(cmap_start, src->blue + offset, copy_size);
        cmap_start += cmap_size;
        if (src->transp && dst->transp)
            osal_memcpy(cmap_start, src->transp + offset, copy_size);
    }
    return 0;
}

int fb_set_cmap(struct fb_cmap_s *cmap, struct hifb_info *info)
{
    int ret=0;

    if (cmap == NULL) {
        return -EINVAL;
    }

    if (cmap->first < 0 || HI_NULL == info)
        return -EINVAL;

    ret = hifb_setcmap(cmap, info);

    if (ret == 0)
        fb_copy_cmap(cmap, &info->cmap);

    return ret;
}

FAR struct fb_vtable_s *up_fbgetvplane(int display, int vplane)
{
    (void)vplane;
    if (NULL == g_hifb[display])
        return NULL;

    return &g_hifb[display]->vtable;
}

static int hifb_getvideoinfo(FAR struct fb_vtable_s *vtable, FAR struct fb_videoinfo_s *vinfo)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if (HI_NULL == info || HI_NULL == vinfo)
        return -EINVAL;

    osal_memcpy(vinfo, &info->vinfo, sizeof(struct fb_videoinfo_s));

    return 0;
}

static int hifb_getplaneinfo(FAR struct fb_vtable_s *vtable, int planeno, FAR struct fb_planeinfo_s *pinfo)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if (HI_NULL == info || HI_NULL == pinfo)
        return -EINVAL;

    pinfo->fbmem = info->oinfo.fbmem;
    pinfo->fblen = info->oinfo.fblen;
    pinfo->display = info->oinfo.overlay;
    pinfo->bpp = info->oinfo.bpp;
    pinfo->stride = info->oinfo.stride;

    return 0;
}

static int hifb_getoverlayinfo(FAR struct fb_vtable_s *vtable, int overlayno, FAR struct fb_overlayinfo_s *oinfo)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if (HI_NULL == info || HI_NULL == oinfo)
        return -EINVAL;

    osal_memcpy(oinfo, &info->oinfo, sizeof(struct fb_overlayinfo_s));

    return 0;
}

int hifb_getcmap(FAR struct fb_vtable_s *vtable, FAR struct fb_cmap_s *cmap)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if (HI_NULL == info || HI_NULL == cmap)
        return -EINVAL;

    return fb_copy_cmap(&info->cmap, cmap);
}

int hifb_putcmap(FAR struct fb_vtable_s *vtable, FAR const struct fb_cmap_s *cmap)
{
    struct hifb_info *info = (struct hifb_info *)vtable;
    int cpopy_size;

    if (HI_NULL == info || HI_NULL == cmap)
        return -EINVAL;

    if ((cpopy_size = cmap->len * sizeof(HI_U16)) < 0)
        return -EINVAL;

    return fb_set_cmap(cmap, info);
}




int up_fbinitialize(int display)
{
    HI_S32 s32Ret = HI_FAILURE;
    struct hifb_info *info = NULL;
    HIFB_PAR_S *pstPar = NULL;

    /* Creates a new frame buffer info structure. reserves hifb_par for driver private data (info->par) */
    info = hifb_alloc(sizeof(HIFB_PAR_S));
    if (HI_NULL == info)
    {
        HIFB_ERROR("failed to malloc the hifb_info!\n");
        return -ENOMEM;
    }

    /* save the info pointer in global pointer array */
    g_stLayer[display].pstInfo = info;

    info->oinfo.overlay = display;
    info->vtable.fb_open = hifb_open;
    info->vtable.fb_release = hifb_release;
    info->vtable.fb_set_par = hifb_set_par;
    info->vtable.fb_pan_display = hifb_pan_display;
    info->vtable.fb_ioctl = hifb_ioctl;
    info->vtable.getvideoinfo = hifb_getvideoinfo;
    info->vtable.getplaneinfo = hifb_getplaneinfo;
    info->vtable.getoverlayinfo = hifb_getoverlayinfo;
    info->vtable.getcmap = NULL;
    info->vtable.putcmap = NULL;

    pstPar = (HIFB_PAR_S *)(info->par);
    osal_memset(pstPar, 0, sizeof(HIFB_PAR_S));
    pstPar->u32LayerID = display;
    pstPar->enColFmt = HIFB_FMT_ARGB1555;
    info->vinfo.fmt = HIFB_FMT_ARGB1555;

    /*It's not need to alloc mem for cursor layer if use softcursor */
    if (g_stLayer[display].ulLayerSize)
    {
        HI_CHAR name[16] = {'\0'};

        osal_snprintf(name, sizeof(name), "hifb_layer%d", display);

        /* initialize the fix screen info */
        if (IS_HD_LAYER(display))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_HD]);
        }
        else if (IS_SD_LAYER(display))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_SD]);
        }
        else if (IS_AD_LAYER(display))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_AD]);
        }
        else if (IS_CURSOR_LAYER(display))
        {
            hifb_screeninfo_init(info, &s_stDefInfo[HIFB_LAYER_TYPE_CURSOR]);
        }

        info->oinfo.fbmem = (void *)hifb_buf_allocmem(name,
            (g_stLayer[display].ulLayerSize + g_stLayer[display].u32CurosrBufSize)* 1024,
            apszLayerMmzNames[display]);
        if (HI_NULL == info->oinfo.fbmem)
        {
            HIFB_ERROR("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_stLayer[display].ulLayerSize + g_stLayer[display].u32CurosrBufSize));
            goto ERR;
        }
        /* u32LayerSize is KB */
        info->oinfo.fblen = g_stLayer[display].ulLayerSize * 1024;

        /* initialize the virtual address and clear memory */
        info->oinfo.fbmem = hifb_buf_map((HI_U32)(uintptr_t)info->oinfo.fbmem, \
            (g_stLayer[display].ulLayerSize + g_stLayer[display].u32CurosrBufSize)* 1024);
        if (HI_NULL == info->oinfo.fbmem)
        {
            HIFB_WARNING("Failed to call map video memory,size:0x%x, start: 0x%lx\n",
                     info->oinfo.fblen, info->oinfo.fbmem);
        }

        if (info)
        {
            osal_memset(info->oinfo.fbmem,0,info->oinfo.fblen);
        }

        /* alloc color map */
        if (s_stDrvOps.pstCapability[display].bCmap) { /* now unsupport cmap function */
        }
    }

    g_hifb[display] = info;

    return 0;

ERR:
    hifb_free(info);
    g_stLayer[display].pstInfo = NULL;
    return s32Ret;
}

void up_fbuninitialize(int display)
{
    struct hifb_info* info = NULL;

    /* get framebuffer info structure pointer */
    info = g_stLayer[display].pstInfo;
    if (info != NULL) {
        if (info->oinfo.fbmem)
        {
            hifb_buf_ummap(info->oinfo.fbmem);
        }

        if (info->oinfo.fbmem)
        {
            hifb_buf_freemem((HI_U32)(uintptr_t)info->oinfo.fbmem);
        }

        hifb_free(info);
        g_stLayer[display].pstInfo = NULL;
        g_hifb[display] = NULL;
    }
}
#endif



static HI_S32 hifb_setDynamicRange(HIFB_PAR_S *pstPar, HIFB_DYNAMIC_RANGE_E enDynamicRange)
{
    if (enDynamicRange >= HIFB_DYNAMIC_RANGE_BUTT)
    {
        HIFB_ERROR("The input dynamic range value is wrong!\n");
        return -EINVAL;
    }

    pstPar->stDisplayInfo.enDynamicRange = enDynamicRange;
    return HI_SUCCESS;
}

#if HICHIP == HI3559A_V100
#ifdef CONFIG_DMA_SHARED_BUFFER
struct dma_buf * hifb_dmabuf_export(struct fb_info *info)
{
     return hifb_memblock_export(hifb_get_smem_start(info), hifb_get_smem_len(info), 0);
}
#endif
#endif

/*
* Name : s_sthifbops
* Desc : fb struct
* See  : hifb_overlay_probe
*/
#ifndef __HuaweiLite__
static struct fb_ops s_sthifbops =
{
    .owner                 = THIS_MODULE,
    .fb_open               = hifb_open,
    .fb_release           = hifb_release,
    .fb_check_var       = hifb_check_var,
    .fb_set_par           = hifb_set_par,
    .fb_pan_display    = hifb_pan_display,
    .fb_ioctl               = hifb_ioctl,
    .fb_setcolreg        = hifb_setcolreg,
    .fb_setcmap         = hifb_setcmap,
#ifdef CONFIG_COMPAT
    .fb_compat_ioctl   = hifb_compat_ioctl,
#endif
#ifdef CONFIG_FB_CFB_IMAGEBLIT
    .fb_imageblit      = hifb_imageblit,
#endif
#if HICHIP == HI3559A_V100
#ifdef CONFIG_DMA_SHARED_BUFFER
    .fb_dmabuf_export   = hifb_dmabuf_export,
#endif
#endif

};
#endif
/******************************************************************************
 Function        : hifb_overlay_cleanup
 Description     : releae the resource for certain framebuffer
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : HI_S32 which_layer
                   HI_S32 need_unregister
 Return          : static
 Others          : 0
******************************************************************************/
#ifdef __HuaweiLite__
static HI_VOID hifb_overlay_cleanup(HI_U32 u32LayerId, HI_BOOL bUnregister)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret= fb_unregister(u32LayerId);
    if (s32Ret < 0)
    {
        HIFB_ERROR("hifb overlay cleanup fail!\n");
    }

    return;
}
#else
static HI_VOID hifb_overlay_cleanup(HI_U32 u32LayerId, HI_BOOL bUnregister)
{
    struct fb_info* info = NULL;

    /* get framebuffer info structure pointer */
    info = g_stLayer[u32LayerId].pstInfo;
    if (info != NULL)
    {
        if (hifb_get_screen_base(info))
        {
            hifb_buf_ummap(hifb_get_screen_base(info));
        }

        if (hifb_get_smem_start(info))
        {
            hifb_buf_freemem(hifb_get_smem_start(info));
        }

        if (bUnregister)
        {
            unregister_framebuffer(info);
        }

        framebuffer_release(info);
        g_stLayer[u32LayerId].pstInfo = NULL;
    }

    return;
}
#endif
/******************************************************************************
 Function        : hifb_overlay_probe
 Description     : initialze the framebuffer for the overlay and set
                   the register .
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : HI_S32 which_layer
                   unsigned long vram_size
 Return          : 0 if success; otherwise return error code
 Others          : 0
******************************************************************************/
#ifdef __HuaweiLite__
static HI_S32 __init hifb_overlay_probe(HI_U32 u32LayerId)
{
    HI_S32 s32Ret = HI_FAILURE;

    if ((s32Ret = fb_register(u32LayerId, 0)) < 0)
    {
        HIFB_ERROR("failed to register_framebuffer!\n");
        s32Ret = -EINVAL;
        goto ERR;
    }

    return HI_SUCCESS;

ERR:
    hifb_overlay_cleanup(u32LayerId, HI_FALSE);

    return s32Ret;
}

#else
static HI_S32 __init hifb_overlay_probe(HI_U32 u32LayerId)
{
    HI_S32                     s32Ret         = HI_FAILURE;
    struct fb_info *           info           = NULL;
    struct fb_fix_screeninfo * pfix           = NULL;
    struct fb_var_screeninfo * pvar           = NULL;
    HIFB_PAR_S *               pstPar         = NULL;

    /* Creates a new frame buffer info structure. reserves hifb_par for driver private data (info->par) */
    /* The size of the size represents the private data space of the device,
    and the par of fb_info points to the private space, that is,
    info->par already points to the memory space.*/
    info = framebuffer_alloc(sizeof(HIFB_PAR_S), NULL);
    if (HI_NULL == info)
    {
        HIFB_ERROR("failed to malloc the fb_info!\n");
        return -ENOMEM;
    }
    pfix           = &info->fix;
    pvar           = &info->var;
    /* save the info pointer in global pointer array */
    g_stLayer[u32LayerId].pstInfo = info;

    osal_snprintf(pfix->id, sizeof(pfix->id), "ovl%u", u32LayerId);
    info->flags = FBINFO_FLAG_DEFAULT | FBINFO_HWACCEL_YPAN | FBINFO_HWACCEL_XPAN;
    // fbops members in fb_info point to s_sthifbops, so open, release, ioctl, etc. can get fb_info.
    info->fbops = &s_sthifbops;

    pstPar = (HIFB_PAR_S *)(info->par);
    osal_memset(pstPar, 0, sizeof(HIFB_PAR_S));
    pstPar->u32LayerID = u32LayerId;
    pstPar->enColFmt = HIFB_FMT_ARGB1555;

    /*It's not need to alloc mem for cursor layer if use softcursor */
    if (g_stLayer[u32LayerId].ulLayerSize)
    {
        HI_CHAR name[16];

        osal_snprintf(name, sizeof(name), "hifb_layer%u", u32LayerId);

        /* initialize the fix screen info */
        if (IS_HD_LAYER(u32LayerId))
        {
            *pfix = s_stDefFix[HIFB_LAYER_TYPE_HD];
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_HD];
        }
        else if (IS_SD_LAYER(u32LayerId))
        {
            *pfix = s_stDefFix[HIFB_LAYER_TYPE_SD];
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_SD];
        }
        else if (IS_AD_LAYER(u32LayerId))
        {
            *pfix = s_stDefFix[HIFB_LAYER_TYPE_AD];
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_AD];
        }
        else if (IS_CURSOR_LAYER(u32LayerId))
        {
            *pfix = s_stDefFix[HIFB_LAYER_TYPE_CURSOR];
            *pvar = s_stDefVar[HIFB_LAYER_TYPE_CURSOR];
        }
        pfix->smem_start = hifb_buf_allocmem(name,
            (g_stLayer[u32LayerId].ulLayerSize + g_stLayer[u32LayerId].u32CurosrBufSize)* 1024,
            apszLayerMmzNames[u32LayerId]);
        if (0 == pfix->smem_start)
        {
            HIFB_ERROR("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_stLayer[u32LayerId].ulLayerSize + g_stLayer[u32LayerId].u32CurosrBufSize));
            goto ERR;
        }
        /* u32LayerSize is KB */
        pfix->smem_len = g_stLayer[u32LayerId].ulLayerSize * 1024;

        /* initialize the virtual address and clear memory */
        info->screen_base = hifb_buf_map(pfix->smem_start,\
            (g_stLayer[u32LayerId].ulLayerSize + g_stLayer[u32LayerId].u32CurosrBufSize)* 1024);

        if (HI_NULL == info->screen_base)
        {
            HIFB_ERROR("Failed to call map video memory,size:0x%x, start: 0x%lx\n",
                     pfix->smem_len, pfix->smem_start);
            goto ERR;
        }

        // Initialize the memory to 0. Call TDE to do it.
#if(HICHIP == HI3516E_V200)
       osal_memset(info->screen_base, 0x0, hifb_get_smem_len(info));
#else
       hifb_memset(info, 0, hifb_get_smem_len(info));
#endif
        /* alloc color map */
        // See s_astHifbGfxCap
        if (s_stDrvOps.pstCapability[u32LayerId].bCmap) { /* now unsupport cmap function */
        }
    }

    if ((s32Ret = register_framebuffer(info)) < 0)
    {
        HIFB_ERROR("failed to register_framebuffer!layerid = %d, s32Ret = %d\n",u32LayerId,s32Ret);
        s32Ret = -EINVAL;
        goto ERR;
    }

    HIFB_INFO("succeed in registering the fb%d: %s frame buffer device\n",
              info->node, pfix->id);

    return HI_SUCCESS;

ERR:
    hifb_overlay_cleanup(u32LayerId, HI_FALSE);

    return s32Ret;
}
#endif
/******************************************************************************
 Function        : hifb_get_vram_size
 Description     : parse the parameter string and get the size. if
                   the parameter is invalid, the size is default value.
 Data Accessed   :
 Data Updated    :
 Output          : None
 Input           : const char* pstr  the string for the vram size
 Return          : the video memory size
 Others          : 0
******************************************************************************/
static unsigned long hifb_get_vram_size(char* pstr)
{
    HI_S32 str_is_valid = HI_TRUE;
    unsigned long vram_size = 0;
    unsigned long vram_size_temp = 0;
    char* ptr = pstr;

    if ((ptr == NULL) || (*ptr == '\0'))
    {
        return 0;
    }

    /*check if the string is valid*/
    while (*ptr != '\0')
    {
        if (*ptr == ',')
        {
            //*ptr = '\0';
            break;
        }
        else if ((!isdigit(*ptr)) && ('X' != *ptr) && ('x' != *ptr)
            && ((*ptr > 'f' && *ptr <= 'z') || (*ptr > 'F' && *ptr <= 'Z')))
        {
            str_is_valid = HI_FALSE;
            break;
        }

        ptr++;
    }

    if (str_is_valid)
    {
        #ifdef __HuaweiLite__
        vram_size = strtoul(pstr, (char **)NULL, 0);
        #else
        vram_size = simple_strtoul(pstr, (char **)NULL, 0);
        #endif

        if (vram_size > PAGE_SIZE_ALIGN_MAX )
        {
            HIFB_ERROR("vram_size(%lu)( > %lu) is overflow, it will be set to %u!\n", vram_size,PAGE_SIZE_ALIGN_MAX,
                 0);
            vram_size = 0;
        }

        vram_size_temp = vram_size;

        /*make the size PAGE_SIZE align*/
        vram_size = ((vram_size * 1024 + PAGE_SIZE - 1) & PAGE_MASK)/1024;

        if (vram_size_temp != vram_size)
        {
             HIFB_ERROR("vram_size(%lu) if not align in 4, it will be set to %lu!\n", vram_size_temp,
                 vram_size);
        }
    }


    return vram_size;
}

/*
* Name : hifb_parse_cfg
* Desc : Parse the parameters.
* See  : hifb_overlay_probe
*/
static HI_S32 hifb_parse_cfg(HI_VOID)
{
    HI_CHAR *pscstr = NULL;
    HI_CHAR number[4] = {0};
    HI_U32 i = 0;
    HI_U32 u32LayerId;
    HI_UL ulLayerSize;
    HI_U32 j = 0;
    HI_CHAR acParam[12] = {0};
    HI_CHAR acTemp[12] = {0};
    HI_BOOL bParaValid = HI_FALSE;

    /*judge the cursor if use soft or hard layer*/
    if (!strcmp("off", softcursor))
    {
        g_bSoftCursor = HI_FALSE;
    }
    else
    {
        g_bSoftCursor = HI_TRUE;
    }

    /*judge the display is need on*/
    if (!strcmp("on", display))
    {
        g_bDisplayOn = HI_TRUE;
    }
    else
    {
        g_bDisplayOn = HI_FALSE;
    }

    /* get the string before next varm */
    pscstr = strstr(video, "vram");
    HIFB_INFO("video:%s\n", video);

    while (pscstr != NULL)
    {
        /* parse the layer id and save it in a string */
        i = 0;

        /* if the number of graphics layers is 10 or more, the string is risky */
        for (j = 0; j < s_stDrvOps.u32LayerCount; j++)
        {
            osal_snprintf(acParam, sizeof(acParam), "vram%u_size", j);
            osal_strncpy(acTemp, pscstr, 10);
            if(!strcmp(acParam, acTemp))
            {
                bParaValid = HI_TRUE;
            }
        }
        if (!bParaValid)
        {
            HIFB_ERROR("insmod parameter is invalid!\n");
            return HI_FAILURE;
        }
        /*skip "vram"*/
        pscstr += 4;
        while (*pscstr != '_')
        {
            /* i>1 means layer id is bigger than 100, it's obviously out of range!*/
            if (i > 1)
            {
                HIFB_ERROR("layer id is out of range!\n");
                return -1;
            }

            number[i] = *pscstr;
            i++;
            pscstr++;
        }

        number[i] = '\0';

        /* change the layer id string into digital and assure it's legal*/
        #ifdef __HuaweiLite__
        u32LayerId = strtoul(number, (char **)NULL, 10);
        #else
        u32LayerId = simple_strtoul(number, (char **)NULL, 10);
        #endif
        if (u32LayerId >= s_stDrvOps.u32LayerCount)
        {
            HIFB_ERROR("Layer %d is in module_param---video out of range!\n", u32LayerId);
            return HI_FAILURE;
        }

        if (!s_stDrvOps.pstCapability[u32LayerId].bLayerSupported)
        {
            HIFB_ERROR("Layer %d unsupported, so module_param---video can't contain vram_size for it!\n",
                u32LayerId);
            return HI_FAILURE;
        }

        /* get the layer size string and change it to digital */
        pscstr += sizeof("size") + i;

        ulLayerSize = hifb_get_vram_size(pscstr);


        if (s_stDrvOps.pstCapability[u32LayerId].bLayerSupported)
        {
            if (IS_CURSOR_LAYER(u32LayerId))
            {
                if (!IS_SOFT_CURSOR())
                {
                    g_stLayer[u32LayerId].ulLayerSize = ulLayerSize;
                }
            }
            else
            {
                g_stLayer[u32LayerId].ulLayerSize = ulLayerSize;
                if (IS_SOFT_CURSOR() && ulLayerSize)
                {
                    g_stLayer[u32LayerId].u32CurosrBufSize = HIFB_CURSOR_DEF_VRAM;
                }
            }
        }

        /* get next layer string */
        pscstr = strstr(pscstr, "vram");
    }

    return 0;
}

#ifndef DISABLE_DEBUG_INFO
static const HI_CHAR* s_pszFmtName[] = {
    "RGB565",
    "RGB888",
    "KRGB444",
    "KRGB555",
    "KRGB888",
    "ARGB4444",
    "ARGB1555",
    "ARGB8888",
    "ARGB8565",
    "RGBA4444",
    "RGBA5551",
    "RGBA5658",
    "RGBA8888",
    "1BPP",
    "2BPP",
    "4BPP",
    "8BPP",
    "ACLUT44",
    "ACLUT88",
    "PUYVY",
    "PYUYV",
    "PYVYU",
    "YUV888",
    "AYUV8888",
    "YUVA8888",
    "BUTT"};

const static HI_CHAR* s_pszLayerName[] = {"layer_0", "layer_1", "layer_2", "layer_3"};
HI_S32 hifb_print_softcursor_proc(struct fb_info * info, struct seq_file *p, HI_VOID *v)
{

    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    const HI_CHAR* pLayerName = NULL;

    if (par->u32LayerID >= sizeof(s_pszLayerName)/sizeof(*s_pszLayerName))
    {
        pLayerName = "unkown layer";
    }
    else
    {
        pLayerName = s_pszLayerName[par->u32LayerID];
    }

    seq_printf(p,  "layer name                 \t: %s \n", pLayerName);
    seq_printf(p,  "Show   State               \t :%s\n",   par->bShow ? "ON" : "OFF");
    seq_printf(p,  "referecce count            \t :%d\n", atomic_read(&par->ref_count));
    seq_printf(p,  "position                   \t :(%d, %d)\n", par->stDisplayInfo.stPos.s32XPos, par->stDisplayInfo.stPos.s32YPos);
    seq_printf(p,  "ColorFormat:               \t :%s\n",s_pszFmtName[par->enColFmt]);
    seq_printf(p,  "Alpha  Enable              \t :%s\n",par->stAlpha.bAlphaEnable ? "ON" : "OFF");
    seq_printf(p,  "AlphaChannel Enable        \t :%s\n",par->stAlpha.bAlphaChannel ? "ON" : "OFF");
    seq_printf(p,  "Alpha0, Alpha1             \t :%d, %d\n", par->stAlpha.u8Alpha0, par->stAlpha.u8Alpha1);
    seq_printf(p,  "Alpha Global               \t :%d\n", par->stAlpha.u8GlobalAlpha);
    seq_printf(p,  "Colorkey Enable            \t :%s\n", par->stCkey.bKeyEnable ? "ON" : "OFF");
    seq_printf(p,  "Colorkey value             \t :0x%x\n",par->stCkey.u32Key);
    seq_printf(p,  "cursor hot pos(x, y)       \t :(%d, %d)\n",par->stCursorInfo.stCursor.stHotPos.s32XPos, par->stCursorInfo.stCursor.stHotPos.s32YPos);
    return 0;
}

HI_S32 hifb_print_layer_proc(struct fb_info * info, struct seq_file *p, HI_VOID *v)
{
    HIFB_PAR_S *               par                   = (HIFB_PAR_S *)info->par;
    HIFB_DISPLAY_INFO_S *      pstDisplayInfo        = &par->stDisplayInfo;
    HIFB_REFRESH_INFO_S *      pstRefreshInfo        = &par->stRefreshInfo;
    volatile
        HIFB_COMPRESS_INFO_S * pstCompressInfo       = &par->stCompressInfo;
    const HI_CHAR*             pszBufMode[]          =
        {"triple", "double", "single", "triple( no frame discarded)", "fence", "unkown"};

    const HI_CHAR*             pszAntiflicerLevel[]  =
        {"NONE", "LOW" , "MIDDLE", "HIGH", "AUTO" ,"ERROR"};

    const HI_CHAR*             pszMirrorMode[]  =
        {"NONE", "HORIZONTAL" , "VERTICAL", "BOTH", "unkown"};

    const HI_CHAR*             pszDynamicRange[]  =
        {"SDR8", "SDR10" , "HDR10", "HLG", "SLF", "unknow"};

    const HI_CHAR*             pszAntiMode[]         =
        {"NONE", "TDE" , "VOU" ,"ERROR"};

    const HI_CHAR*             pszRotationMode[]     =
        {"0","90","180","270","-"};

    const HI_CHAR*             pLayerName            = NULL;

    if (par->u32LayerID >= sizeof(s_pszLayerName)/sizeof(*s_pszLayerName))
    {
        pLayerName = "unkown layer";
    }
    else
    {
        pLayerName = s_pszLayerName[par->u32LayerID];
    }

    if (pstDisplayInfo->enAntiflickerMode > HIFB_ANTIFLICKER_BUTT)
    {
        pstDisplayInfo->enAntiflickerMode = HIFB_ANTIFLICKER_BUTT;
    }

    if (pstDisplayInfo->enAntiflickerLevel > HIFB_LAYER_ANTIFLICKER_BUTT)
    {
        pstDisplayInfo->enAntiflickerLevel = HIFB_LAYER_ANTIFLICKER_BUTT;
    }

    // seq_printf(p,  "\n[HIFB] Version: ["MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n");
    seq_printf(p,  "\n[HIFB] Version: ["MPP_VERSION"], Build Time[202009291802]\n");
    seq_printf(p,  "\n");

    seq_printf(p,  "layer name                 \t :%s \n", pLayerName);
    seq_printf(p,  "Open count                 \t :%d\n", atomic_read(&par->ref_count));
    seq_printf(p,  "Show state                 \t :%s\n", par->bShow ? "ON" : "OFF");
    seq_printf(p,  "Start position             \t :(%d, %d)\n", pstDisplayInfo->stPos.s32XPos, pstDisplayInfo->stPos.s32YPos);
    seq_printf(p,  "xres, yres                 \t :(%d, %d)\n", hifb_get_xres(info), hifb_get_yres(info));
    seq_printf(p,  "xres_virtual, yres_virtual \t :(%d, %d)\n", hifb_get_xres_virtual(info), hifb_get_yres_virtual(info));
    seq_printf(p,  "xoffset, yoffset           \t :(%d, %d)\n", hifb_get_xoffset(info), hifb_get_yoffset(info));
    seq_printf(p,  "fix.line_length            \t :%d\n", hifb_get_line_length(info));
    seq_printf(p,  "Mem size:                  \t :%d KB\n", hifb_get_smem_len(info) / 1024);
    seq_printf(p,  "Layer Scale (hw):          \t :%s \n", s_stDrvOps.pstCapability[par->u32LayerID].bVoScale ? "YES" : "NO");
    seq_printf(p,  "ColorFormat:               \t :%s\n", s_pszFmtName[par->enColFmt]);
    seq_printf(p,  "Alpha Enable               \t :%s\n", par->stAlpha.bAlphaEnable ? "ON" : "OFF");
    seq_printf(p,  "AlphaChannel Enable        \t :%s\n", par->stAlpha.bAlphaChannel ? "ON" : "OFF");
    seq_printf(p,  "Alpha0, Alpha1             \t :%d, %d\n", par->stAlpha.u8Alpha0, par->stAlpha.u8Alpha1);
    seq_printf(p,  "Alpha Global               \t :%d\n", par->stAlpha.u8GlobalAlpha);
    seq_printf(p,  "Colorkey Enable            \t :%s\n", par->stCkey.bKeyEnable ? "ON" : "OFF");
    seq_printf(p,  "Colorkey value             \t :0x%x\n",par->stCkey.u32Key);
    seq_printf(p,  "Mirror Mode:            \t :%s\n",pszMirrorMode[pstDisplayInfo->enMirrorMode]);
    seq_printf(p,  "Dynamic Range:            \t :%s\n",pszDynamicRange[pstDisplayInfo->enDynamicRange]);
    seq_printf(p,  "Deflicker Mode:            \t :%s\n",pszAntiMode[pstDisplayInfo->enAntiflickerMode]);
    seq_printf(p,  "Rotation mode:             \t :%s\n",pszRotationMode[pstDisplayInfo->enRotateMode]);
    seq_printf(p,  "Deflicker Level:           \t :%s\n",pszAntiflicerLevel[pstDisplayInfo->enAntiflickerLevel]);
    seq_printf(p,  "HiFB mode:                  \t :%s\n",HIFB_LAYER_BUF_BUTT == pstRefreshInfo->enBufMode ? "STANDARD" : "EXTEND");
    seq_printf(p,  "Display Buffer mode(+UsrBuf)\t :%s\n",pszBufMode[pstRefreshInfo->enBufMode]);
    seq_printf(p,  "Displaying addr (register) \t :0x%llx\n",pstRefreshInfo->u64ScreenAddr);
    seq_printf(p,  "display buffer[0] addr     \t :0x%llx\n",pstRefreshInfo->stDispBufInfo.u64PhyAddr[0]);
    seq_printf(p,  "display buffer[1] addr     \t :0x%llx\n",pstRefreshInfo->stDispBufInfo.u64PhyAddr[1]);
    seq_printf(p,  "Be PreMul Mode:            \t :%s\n",HI_TRUE == pstDisplayInfo->bPreMul ? "YES" : "NO");
    seq_printf(p,  "displayrect                \t :(%d, %d)\n",pstDisplayInfo->u32DisplayWidth, pstDisplayInfo->u32DisplayHeight);
    seq_printf(p,  "screenrect                 \t :(%d, %d)\n",pstDisplayInfo->u32ScreenWidth, pstDisplayInfo->u32ScreenHeight);
    seq_printf(p,  "device max resolution      \t :%d, %d\n",pstDisplayInfo->u32MaxScreenWidth, pstDisplayInfo->u32MaxScreenHeight);
    seq_printf(p,  "IsNeedFlip(2buf)           \t :%s\n",pstRefreshInfo->stDispBufInfo.bNeedFlip? "YES" : "NO");
    seq_printf(p,  "BufferIndexDisplaying(2buf)\t :%d\n",pstRefreshInfo->stDispBufInfo.u32IndexForInt);
    seq_printf(p,  "refresh request num(2buf)  \t :%d\n",pstRefreshInfo->u32RefreshNum);
    seq_printf(p,  "switch buf num(2buf)       \t :%d\n",pstRefreshInfo->stDispBufInfo.u32IntPicNum);
    seq_printf(p,  "union rect (2buf)          \t :(%d,%d,%d,%d)\n",
        pstRefreshInfo->stDispBufInfo.stUnionRect.x, pstRefreshInfo->stDispBufInfo.stUnionRect.y,
        pstRefreshInfo->stDispBufInfo.stUnionRect.w, pstRefreshInfo->stDispBufInfo.stUnionRect.h);

    seq_printf(p,  "canavas updated addr       \t :0x%llx\n",pstRefreshInfo->stUserBuffer.stCanvas.u64PhyAddr + pstRefreshInfo->stUserBuffer.UpdateRect.y*
                            pstRefreshInfo->stUserBuffer.UpdateRect.w + pstRefreshInfo->stUserBuffer.UpdateRect.x);
    seq_printf(p,  "canavas updated (w, h)     \t :%d,%d \n", pstRefreshInfo->stUserBuffer.UpdateRect.w, pstRefreshInfo->stUserBuffer.UpdateRect.h);
    seq_printf(p,  "canvas width               \t :%d\n", pstRefreshInfo->stUserBuffer.stCanvas.u32Width);
    seq_printf(p,  "canvas height              \t :%d\n", pstRefreshInfo->stUserBuffer.stCanvas.u32Height);
    seq_printf(p,  "canvas pitch               \t :%d\n", pstRefreshInfo->stUserBuffer.stCanvas.u32Pitch);
    seq_printf(p,  "canvas format              \t :%s\n", s_pszFmtName[pstRefreshInfo->stUserBuffer.stCanvas.enFmt]);
    seq_printf(p,  "IsCompress                 \t :%s\n", pstCompressInfo->bCompressOpen ?"YES" : "NO");
    seq_printf(p,  "Is DDR Dettect             \t :%s\n", (pstCompressInfo->bCompressOpen && pstCompressInfo->u32ZoneNums
        && (pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE || pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_BUTT))?"YES" : "NO");
    seq_printf(p,  "DDR Detect Zones           \t :%d\n", (pstCompressInfo->bCompressOpen
        && (pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_NONE || pstRefreshInfo->enBufMode == HIFB_LAYER_BUF_BUTT))? pstCompressInfo->u32ZoneNums : 0);
    seq_printf(p,  "PreMul Enable                 \t :%s\n", par->stDisplayInfo.bPreMul? "ON" : "OFF");
    if (par->stCursorInfo.bAttched && IS_SOFT_CURSOR())
    {
        seq_printf(p,  "attach cursor id:         \t  :%d \n", par->stCursorInfo.u32AttachedCursorId);
        seq_printf(p,  "backup cursor addr         \t :0x%lld\n",par->stCursorInfo.stCursor.stCursor.u64PhyAddr);
        seq_printf(p,  "backup cursor fmt          \t :%s\n",s_pszFmtName[par->stCursorInfo.stCursor.stCursor.enFmt]);
        seq_printf(p,  "backup cursor stride       \t :%d\n",par->stCursorInfo.stCursor.stCursor.u32Pitch);
        seq_printf(p,  "backup cursor (w, h)       \t :(%d, %d)\n",
                    par->stCursorInfo.stCursor.stCursor.u32Width, par->stCursorInfo.stCursor.stCursor.u32Height);
        seq_printf(p,  "cursor rect in display buffer \t :(%d, %d, %d, %d)\n",
                    par->stCursorInfo.stRectInDispBuf.x, par->stCursorInfo.stRectInDispBuf.y,
                    par->stCursorInfo.stRectInDispBuf.w, par->stCursorInfo.stRectInDispBuf.h);
        seq_printf(p,  "cursor pos in cursor image \t :(%d, %d)\n",
                    par->stCursorInfo.stPosInCursor.s32XPos, par->stCursorInfo.stPosInCursor.s32YPos);
    }
    return 0;
}

HI_S32 hifb_read_proc(struct osal_proc_dir_entry *entry)
{
    struct fb_info* info = NULL;
    HIFB_PAR_S *par = NULL;
    if (entry == NULL) {
        return HI_FAILURE;
    }
    info = (struct fb_info *)(entry->private);
    if (info == NULL) {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;
    if (par == NULL) {
        return HI_FAILURE;
    }

    if (!IS_CURSOR_LAYER(par->u32LayerID) || !IS_SOFT_CURSOR())
    {
        return hifb_print_layer_proc(info, entry->seqfile, NULL);
    }
    else
    {
        return hifb_print_softcursor_proc(info,  entry->seqfile, NULL);
    }

    return HI_SUCCESS;
}

static HI_VOID hifb_parse_proccmd(struct seq_file* p, HI_U32 u32LayerId, HI_CHAR *pCmd)
{
    struct fb_info *info = g_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
    HI_S32 cnt;
    HIFB_SPIN_LOCK_FLAG lockflag;

    HIFB_SPIN_LOCK_IRQSAVE(&pstPar->lock, lockflag);
    cnt = atomic_read(&pstPar->ref_count);

    if (strncmp("show", pCmd, 4) == 0)
    {
        if (cnt == 0)
        {
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            HIFB_ERROR("err:u32LayerId %d no open!\n", pstPar->u32LayerID);
            return;
        }

        if (IS_CURSOR_LAYER(pstPar->u32LayerID) && IS_SOFT_CURSOR())
        {
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            HIFB_ERROR("cursor layer %d doesn't support this cmd!\n", pstPar->u32LayerID);
            return;
        }

        if (!pstPar->bShow)
        {
            pstPar->bModifying = HI_TRUE;
            pstPar->bShow = HI_TRUE;
            pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
            pstPar->bModifying = HI_FALSE;
        }
    }
    else if (strncmp("hide", pCmd, 4) == 0)
    {
        if (cnt == 0)
        {
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            HIFB_ERROR("err:u32LayerId %d no open!\n", pstPar->u32LayerID);
            return;
        }

        if (IS_CURSOR_LAYER(pstPar->u32LayerID) && IS_SOFT_CURSOR())
        {
            HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);
            HIFB_ERROR("cursor layer %d doesn't support this cmd!\n", pstPar->u32LayerID);
            return;
        }

        if (pstPar->bShow)
        {
            pstPar->bModifying = HI_TRUE;
            pstPar->bShow = HI_FALSE;
            pstPar->u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
            pstPar->bModifying = HI_FALSE;
        }
    }

    HIFB_SPIN_UNLOCK_IRQRESTORE(&pstPar->lock, lockflag);

    if (strncmp("help", pCmd, 4) == 0) {
        printk("help info:\n");
        printk("echo cmd > proc file\n");
        printk("hifb support cmd:\n");
        printk("show:show layer\n");
        printk("hide:hide layer\n");
        printk("For example, if you want to hide layer 0,you can input:\n");
        printk("   echo hide > /proc/umap/hifb0\n");
    } else {
       HIFB_ERROR("u32LayerID %d doesn't support cmd:%s, use help cmd to show help info!\n", \
        pstPar->u32LayerID, pCmd);
    }

    return;
}

HI_S32 hifb_write_proc(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *ppos)
{
#ifndef __HuaweiLite__
#define TMP_BUF_LEN 32
    struct seq_file *s = NULL;
    struct fb_info *info = NULL;
    HIFB_PAR_S *pstPar = NULL;
    char tmp_buf[TMP_BUF_LEN] = {0};
    int len = TMP_BUF_LEN;

    if (entry == NULL) {
        return -ENOSYS;
    }
    s = entry->seqfile;

    if (HI_NULL == s)
    {
        return -ENOSYS;
    }
    if (count <= 0)
    {
        return -ENOSYS;
    }

    len = (count >= TMP_BUF_LEN) ? TMP_BUF_LEN : count;

    if(osal_copy_from_user(tmp_buf, buf, len ))
        return HI_FAILURE;

    tmp_buf[len-1] = '\0';

    info = (struct fb_info *)(entry->private);
    if (info == NULL) {
        return HI_FAILURE;
    }
    pstPar = (HIFB_PAR_S *)(info->par);
    if (pstPar == NULL) {
        return HI_FAILURE;
    }

    hifb_parse_proccmd(s, pstPar->u32LayerID, (HI_CHAR*)tmp_buf);
    osal_memset((void*)tmp_buf, 0, count);
#endif
    return count;
}
#endif

// for linux
#ifndef __HuaweiLite__
#ifdef CONFIG_HI_VO_FB_SEPARATE

#define HIFB_INT_NAME_LENGTH 10

static int hi35xx_hifb_probe(struct platform_device *pdev)
{
    HI_CHAR HifbIntName[HIFB_INT_NAME_LENGTH] = "hifb";

    g_fifb_irq = osal_platform_get_irq_byname(pdev, HifbIntName);
    if (g_fifb_irq <= 0) {
        dev_err(&pdev->dev, "cannot find hifb IRQ\n");
        return HI_FAILURE;
    }

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    g_fifb_soft_irq = osal_platform_get_irq_byname(pdev, "hifb_soft");
    if (g_fifb_soft_irq <= 0) {
        return HI_FAILURE;
    }
#endif

    hifb_init();

    return 0;
}

static int hi35xx_hifb_remove(struct platform_device *pdev)
{
    hifb_cleanup();

    return 0;
}


static const struct of_device_id hi35xx_hifb_match[] = {
        { .compatible = "hisilicon,hisi-hifb" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_hifb_match);

static struct platform_driver hi35xx_hifb_driver = {
        .probe          = hi35xx_hifb_probe,
        .remove         = hi35xx_hifb_remove,
        .driver         = {
                .name   = "hi35xx_hifb",
                .of_match_table = hi35xx_hifb_match,
        },
};

osal_module_platform_driver(hi35xx_hifb_driver);

MODULE_LICENSE("GPL");
// end for CONFIG_HI_VO_FB_SEPARATE=y in linux
#else
// for CONFIG_HI_VO_FB_SEPARATE=n in linux
#ifdef MODULE
module_init(hifb_init);
module_exit(hifb_cleanup);
MODULE_LICENSE("GPL");
#else
subsys_initcall(hifb_init);
#endif
#endif
// end for CONFIG_HI_VO_FB_SEPARATE=n in linux

// for liteos
#else

#ifdef MODULE
module_init(hifb_init);
module_exit(hifb_cleanup);
MODULE_LICENSE("GPL");
#else
subsys_initcall(hifb_init);
#endif

#endif
