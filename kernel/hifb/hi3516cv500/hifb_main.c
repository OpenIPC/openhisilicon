/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include "hifb_main.h"
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
#ifndef CONFIG_HI_VO_FB_SEPARATE
#include "vgs_ext.h"
#endif

#define mkstr(exp) #exp
#define mkmarcotostr(exp) mkstr(exp)

#define hifb_min_width(layer_id) g_drv_ops.capability[layer_id].u32MinWidth
#define hifb_min_height(layer_id) g_drv_ops.capability[layer_id].u32MinHeight

#define HIFB_CMAP_LEN 256
#define HIFB_ROTBUF_NAME_LEN 16

#define HIFB_NUMBLE  4
#define HIFB_ACFARAM 12
#define HIFB_ACTEMP  12

#define COMPRESS_HEADER_STRIDE 16
#define COMPRESS_STRIDE_ALIGNMENT 16

typedef hi_s32(*drv_hifb_ioctl_func)(struct fb_info *info, unsigned long arg);

typedef struct {
    hi_u32 cmd;
    drv_hifb_ioctl_func func;
} drv_hifb_ioctl_func_item;

#if HICHIP == HI3519A_V100
unsigned int g_fifb_irq = VOU1_IRQ_NR;
#elif HICHIP == HI3559A_V100
unsigned int g_fifb_irq = VO1_IRQ_NR;
#elif HICHIP == HI3516C_V500
unsigned int g_fifb_irq = VO1_IRQ_NR;
#endif
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
#ifdef __HuaweiLite__
    static hi_u32 g_fifb_soft_irq = 83;
#else
    static hi_u32 g_fifb_soft_irq = 0;
#endif
    static hi_bool g_hifb_soft_irq_register = HI_FALSE;
#endif

#define PAGE_SIZE_ALIGN_MAX ((~0ul - PAGE_SIZE) / 1024)

#define is_soft_cursor() (g_soft_cursor == HI_TRUE)

#ifdef __HuaweiLite__
#define in_atomic() (HI_FALSE)
#endif

#define HIFB_ALIGNMENT 0xf

#define HIFB_ALIGN 16

/*
 * the interface to operate the chip
 * Collection of properties and methods,filled in hifb_init
 */
static hifb_drv_ops g_drv_ops;

static hi_bool g_soft_cursor = HI_FALSE;
static hi_bool g_display_on = HI_FALSE;
static hi_bool g_hifb_register = HI_TRUE;
#ifdef CONFIG_HI_VO_FB_SEPARATE
static hi_bool g_hifb_irq_register = HI_FALSE;
#endif
#define VO_INVALID_DEV (-1)

#define HIFB_INTMSK_HD0_VTTHD1 0x1
#define HIFB_INTMSK_HD0_VTTHD2 0x2
#define HIFB_INTMSK_HD0_VTTHD3 0x4
#define HIFB_INTMSK_HD1_VTTHD2 0x20
#define HIFB_INTMSK_HD1_VTTHD3 0x40

#define DRV_HIFB_IOCTL_CMD_NUM_MAX 151
#define DRV_HIFB_IOCTL_FUNC_ITEM_NUM_MAX 44

#define DEV_NAME "hifb"

static hi_s32 hifb_parse_cfg(hi_void);
static hi_s32 hifb_overlay_probe(hi_u32 layer_id);
static hi_void hifb_overlay_cleanup(hi_u32 layer_id, hi_bool unregister);
#ifdef __HuaweiLite__
#define fb_info hifb_info
static hi_s32 hifb_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo);
#else
static hi_s32 hifb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
#endif
static HIFB_COLOR_FMT_E hifb_getfmtbyargb(struct fb_bitfield *red, struct fb_bitfield *green, struct fb_bitfield *blue,
                                          struct fb_bitfield *transp, hi_u32 color_depth);
hi_void hifb_buf_freemem(hi_u64 phyaddr);
hi_u64 hifb_buf_allocmem(const hi_char *buf_name, hi_u32 size, hi_ulong layer_size, const hi_char *mmz_name);

static hi_s32 hifb_wait_regconfig_work(hi_u32 layer_id);

static hi_s32 hifb_freeccanbuf(hifb_par *par);
static hi_void hifb_set_dispbufinfo(hi_u32 layer_id);
static hi_void hifb_set_bufmode(hi_u32 layer_id, HIFB_LAYER_BUF_E layer_buf_mode);
static inline hi_void hifb_get_bufmode(hifb_par *par, HIFB_LAYER_BUF_E *buf_mode);
static hi_s32 hifb_onrefresh(hifb_par *par, hi_void __user *argp);
static hi_s32 hifb_refresh_0buf(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf);
static hi_s32 hifb_refresh_1buf(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf);
static hi_s32 hifb_refresh_2buf(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf);
static hi_s32 hifb_refresh_2buf_immediate_display(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf);
static hi_s32 hifb_set_mirrormode(hifb_par *par, HIFB_MIRROR_MODE_E mirror_mode);
static hi_s32 hifb_set_rotatemode(struct fb_info *info, HIFB_ROTATE_MODE_E rotate_mode);
#ifdef CURSOR
static inline hi_void hifb_set_cursorinfo(hifb_par *par, hifb_cursor_info *cursor_info);
static inline hi_void hifb_get_cursorinfo(hifb_par *par, hifb_cursor_info *cursor_info);
static hi_s32 hifb_cursor_attach(hi_u32 cursor_id, hi_u32 layer_id);
static hi_s32 hifb_cursor_detach(hi_u32 layer_id);
static hi_s32 hifb_cursor_changepos(hi_u32 cursor_id, HIFB_POINT_S pos);
static hi_s32 hifb_cursor_changestate(hifb_par *cursor_par, hi_bool show);
static hi_s32 hifb_cursor_putinfo(hifb_par *cursor_par, HIFB_CURSOR_S *cursor);
#endif
static hi_s32 hifb_onputlayerinfo(struct fb_info *info, hifb_par *par, const hi_void __user *argp);
static hi_void hifb_get_layerinfo(hifb_par *par, HIFB_LAYER_INFO_S *layer_info);
static inline hi_void hifb_get_antiflickerlevel(hifb_par *par,
                                                HIFB_LAYER_ANTIFLICKER_LEVEL_E *pen_antiflicker_level);
static hi_void hifb_set_antiflickerlevel(hi_u32 layer_id, HIFB_LAYER_ANTIFLICKER_LEVEL_E antiflicker_level);
static inline hi_void hifb_get_fmt(hifb_par *par, HIFB_COLOR_FMT_E *color_format);
static inline hi_void hifb_set_fmt(hifb_par *par, HIFB_COLOR_FMT_E color_fmt);
static inline hi_void hifb_set_alpha(hifb_par *par, HIFB_ALPHA_S *alpha);
static inline hi_void hifb_get_alpha(hifb_par *par, HIFB_ALPHA_S *alpha);
static inline hi_void hifb_set_key(hifb_par *par, hifb_colorkeyex *key);
static inline hi_void hifb_get_key(hifb_par *par, hifb_colorkeyex *key);
static inline hi_void hifb_get_layerpos(hifb_par *par, HIFB_POINT_S *pos);
static hi_void hifb_get_screensize(hifb_par *par, hi_u32 *width, hi_u32 *height);
static hi_s32 hifb_set_screensize(hifb_par *par, hi_u32 *width, hi_u32 *height);
static hi_void hifb_get_maxscreensize(hifb_par *par, hi_u32 *width, hi_u32 *height);
static hi_void hifb_get_dispsize(hifb_par *par, hi_u32 *width, hi_u32 *height);
static inline hi_void hifb_get_premul(hifb_par *par, hi_bool *premul);
static inline hi_bool hifb_get_show(hifb_par *par);
static inline hi_void hifb_set_show(hifb_par *par, hi_bool show);
static hi_void hifb_set_layerpos(hifb_par *par, HIFB_POINT_S *pos);
#if (HICHIP == HI3516E_V200)
static hi_s32 hifb_tde_rotate_callback(hi_u32 call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id, hi_void *job_data);
static hi_s32 hifb_tde_callback(hi_u32 call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id, hi_void *job_data);
#else
static hi_s32 hifb_tde_rotate_callback(const hi_void *paraml, const hi_void *paramr);
static hi_s32 hifb_tde_callback(const hi_void *paraml, const hi_void *paramr);
#endif
static hi_s32 hifb_vo_callback(const hi_void *paraml, const hi_void *paramr);
static hi_s32 hifb_wbc_callback(const hi_void *paraml, const hi_void *paramr);
static hi_void hifb_get_idledispbuf(hifb_par *par, hi_u64 *phy_addr);
static hi_void hifb_get_workdispbuf(hifb_par *par, hi_u64 *phy_addr);
static inline hi_void hifb_get_workcursorbuf(hifb_par *par, hi_u64 *phy_addr);
#ifdef CURSOR
static hi_void hifb_get_idlecursorbuf(hifb_par *par, hi_u64 *phy_addr);
#endif
static inline hi_bool hifb_is_interlace(hifb_par *par);
static hi_s32 hifb_set_dynamic_range_display(hifb_par *par, HIFB_DYNAMIC_RANGE_E dynamic_range);

hi_s32 hifb_read_proc(struct osal_proc_dir_entry *entry);
hi_s32 hifb_write_proc(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);

static hi_s32 hifb_get_colorkey(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_colorkey(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_layer_alpha(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_layer_alpha(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_screen_origin_pos(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_screen_origin_pos(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_deflicker(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_deflicker(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_vblank(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_show_layer(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_layer_show_state(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_capability(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_sursor_info(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_sursor_info(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_cursor_state(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_cursor_state(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_cursor_pos(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_cursor_pos(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_cursor_colorkey(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_cursor_colorkey(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_cursor_alpha(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_cursor_alpha(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_attch_cursor(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_detach_cursor(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_layer_info(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_layer_info(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_canvas_buffer(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_refresh_layer(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_wait_refresh_finish(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_mirror_mode(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_mirror_mode(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_rotate_mode(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_rotate_mode(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_screen_size(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_screen_size(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_flip_surface(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_compression_mode(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_compression_mode(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_mddrdetect(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_mddrdetect(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_set_dynamic_range(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_get_dynamic_range(struct fb_info *info, unsigned long arg);
static hi_s32 hifb_info_release(struct fb_info *info, unsigned long arg);

/*
 * Name : g_stDefFix
 * Desc : default fixed info: fix_info
 */
static hi_s32 g_drv_hifb_ctl_num[DRV_HIFB_IOCTL_CMD_NUM_MAX] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  0,  7,  8,
    9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 0,  0,  0,  0,
    25, 26, 0,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 0,  0,  41,
    42, 0,  0,  0,  0,  0,  0,  0,  0,  0,  43
};

static drv_hifb_ioctl_func_item g_drv_hifb_ioctl_func[DRV_HIFB_IOCTL_FUNC_ITEM_NUM_MAX] = {{0, HI_NULL},
    {FBIOGET_COLORKEY_HIFB, hifb_get_colorkey},    {FBIOPUT_COLORKEY_HIFB, hifb_set_colorkey},
    {FBIOGET_ALPHA_HIFB, hifb_get_layer_alpha},    {FBIOPUT_ALPHA_HIFB, hifb_set_layer_alpha},
    {FBIOGET_SCREEN_ORIGIN_HIFB, hifb_get_screen_origin_pos},
    {FBIOPUT_SCREEN_ORIGIN_HIFB, hifb_set_screen_origin_pos},
    {FBIOGET_DEFLICKER_HIFB, hifb_get_deflicker},  {FBIOPUT_DEFLICKER_HIFB, hifb_set_deflicker},
    {FBIOGET_VBLANK_HIFB, hifb_get_vblank},
    {FBIOPUT_SHOW_HIFB, hifb_show_layer},          {FBIOGET_SHOW_HIFB, hifb_get_layer_show_state},
    {FBIOGET_CAPABILITY_HIFB, hifb_get_capability},
    {FBIOPUT_CURSOR_INFO, hifb_set_sursor_info},   {FBIOGET_CURSOR_INFO, hifb_get_sursor_info},
    {FBIOPUT_CURSOR_STATE, hifb_set_cursor_state}, {FBIOGET_CURSOR_STATE, hifb_get_cursor_state},
    {FBIOPUT_CURSOR_POS, hifb_set_cursor_pos},     {FBIOGET_CURSOR_POS, hifb_get_cursor_pos},
    {FBIOPUT_CURSOR_COLORKEY, hifb_set_cursor_colorkey},
    {FBIOGET_CURSOR_COLORKEY, hifb_get_cursor_colorkey},
    {FBIOPUT_CURSOR_ALPHA, hifb_set_cursor_alpha}, {FBIOGET_CURSOR_ALPHA, hifb_get_cursor_alpha},
    {FBIOPUT_CURSOR_ATTCHCURSOR, hifb_set_attch_cursor},
    {FBIOPUT_CURSOR_DETACHCURSOR, hifb_set_detach_cursor},
    {FBIOPUT_LAYER_INFO, hifb_set_layer_info},     {FBIOGET_LAYER_INFO, hifb_get_layer_info},
    {FBIOGET_CANVAS_BUFFER, hifb_get_canvas_buffer},  {FBIO_REFRESH, hifb_refresh_layer},
    {FBIO_WAITFOR_FREFRESH_DONE, hifb_wait_refresh_finish},
    {FBIOPUT_MIRROR_MODE, hifb_set_mirror_mode},   {FBIOGET_MIRROR_MODE, hifb_get_mirror_mode},
    {FBIOPUT_ROTATE_MODE, hifb_set_rotate_mode},   {FBIOGET_ROTATE_MODE, hifb_get_rotate_mode},
    {FBIOPUT_SCREENSIZE, hifb_set_screen_size},   {FBIOGET_SCREENSIZE, hifb_get_screen_size},
    {FBIOFLIP_SURFACE, hifb_flip_surface},    {FBIOPUT_COMPRESSION_HIFB, hifb_set_compression_mode},
    {FBIOGET_COMPRESSION_HIFB, hifb_get_compression_mode},
    {FBIOPUT_MDDRDETECT_HIFB, hifb_set_mddrdetect}, {FBIOGET_MDDRDETECT_HIFB, hifb_get_mddrdetect},
    {FBIOPUT_DYNAMIC_RANGE_HIFB, hifb_set_dynamic_range},
    {FBIOGET_DYNAMIC_RANGE_HIFB, hifb_get_dynamic_range},
    {FBIO_RELEASE_HIFB, hifb_info_release},
};

#ifndef __HuaweiLite__
static hi_ulong hifb_get_smem_start(struct fb_info *info)
{
    return (hi_ulong)info->fix.smem_start;
}

static hi_s8 *hifb_get_screen_base(struct fb_info *info)
{
    return (hi_s8 *)info->screen_base;
}

static hi_u32 hifb_get_xres(struct fb_info *info)
{
    return (hi_u32)info->var.xres;
}

static hi_u32 hifb_get_yres(struct fb_info *info)
{
    return (hi_u32)info->var.yres;
}

static hi_u32 hifb_get_xres_virtual(struct fb_info *info)
{
    return (hi_u32)info->var.xres_virtual;
}

static hi_u32 hifb_get_yres_virtual(struct fb_info *info)
{
    return (hi_u32)info->var.yres_virtual;
}

static hi_u32 hifb_get_smem_len(struct fb_info *info)
{
    return (hi_u32)info->fix.smem_len;
}

static hi_u32 hifb_get_line_length(struct fb_info *info)
{
    return (hi_u32)info->fix.line_length;
}

static hi_u32 hifb_get_bits_per_pixel(struct fb_info *info)
{
    return (hi_u32)info->var.bits_per_pixel;
}

static hi_u32 hifb_get_yoffset(struct fb_info *info)
{
    return (hi_u32)info->var.yoffset;
}

static hi_u32 hifb_get_xoffset(struct fb_info *info)
{
    return (hi_u32)info->var.xoffset;
}

static struct fb_fix_screeninfo g_default_fix[HIFB_LAYER_TYPE_BUTT] = {
    {
        .id = "hifb",                  /* String identifierString identifier */
        .type = FB_TYPE_PACKED_PIXELS, /* FB type */
        .visual = FB_VISUAL_TRUECOLOR,
        .xpanstep = 1,
        .ypanstep = 1,
        .ywrapstep = 0,
        /* The number of bytes in one line(width * 4 (32 bits per pixels / 8 bits per byte)) */
        .line_length = HIFB_HD_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id = "hifb",
        .type = FB_TYPE_PACKED_PIXELS,
        .visual = FB_VISUAL_TRUECOLOR,
        .xpanstep = 1,
        .ypanstep = 1,
        .ywrapstep = 0,
        .line_length = HIFB_SD_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id = "hifb",
        .type = FB_TYPE_PACKED_PIXELS,
        .visual = FB_VISUAL_TRUECOLOR,
        .xpanstep = 1,
        .ypanstep = 1,
        .ywrapstep = 0,
        .line_length = HIFB_SD_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id = "hifb",
        .type = FB_TYPE_PACKED_PIXELS,
        .visual = FB_VISUAL_TRUECOLOR,
        .xpanstep = 1,
        .ypanstep = 1,
        .ywrapstep = 0,
        .line_length = HIFB_HD_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    }
};

static struct fb_var_screeninfo g_default_var[HIFB_LAYER_TYPE_BUTT] = {
    /* for HD layer */
    {
        .xres = HIFB_HD_DEF_WIDTH,
        .yres = HIFB_HD_DEF_HEIGHT,
        .xres_virtual = HIFB_HD_DEF_WIDTH,
        .yres_virtual = HIFB_HD_DEF_HEIGHT,
        .xoffset = 0,
        .yoffset = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red = {10, 5, 0},
        .green = {5, 5, 0},
        .blue = {0, 5, 0},
        .transp = {15, 1, 0},
        .activate = FB_ACTIVATE_NOW,
        .pixclock = -1,    /* pixel clock in ps (pico seconds) */
        .left_margin = 0,  /* time from sync to picture */
        .right_margin = 0, /* time from picture to sync */
        .upper_margin = 0, /* time from sync to picture */
        .lower_margin = 0,
        .hsync_len = 0, /* length of horizontal sync */
        .vsync_len = 0, /* length of vertical sync */
    },
    /* for SD layer */
    {
        .xres = HIFB_SD_DEF_WIDTH,
        .yres = HIFB_SD_DEF_HEIGHT,
        .xres_virtual = HIFB_SD_DEF_WIDTH,
        .yres_virtual = HIFB_SD_DEF_HEIGHT,
        .xoffset = 0,
        .yoffset = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red = {10, 5, 0},
        .green = {5, 5, 0},
        .blue = {0, 5, 0},
        .transp = {15, 1, 0},
        .activate = FB_ACTIVATE_NOW,
        .pixclock = -1,    /* pixel clock in ps (pico seconds) */
        .left_margin = 0,  /* time from sync to picture */
        .right_margin = 0, /* time from picture to sync */
        .upper_margin = 0, /* time from sync to picture */
        .lower_margin = 0,
        .hsync_len = 0, /* length of horizontal sync */
        .vsync_len = 0, /* length of vertical sync */
    },
    /* for AD layer */
    {
        .xres = HIFB_AD_DEF_WIDTH,
        .yres = HIFB_AD_DEF_HEIGHT,
        .xres_virtual = HIFB_AD_DEF_WIDTH,
        .yres_virtual = HIFB_AD_DEF_HEIGHT,
        .xoffset = 0,
        .yoffset = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red = {10, 5, 0},
        .green = {5, 5, 0},
        .blue = {0, 5, 0},
        .transp = {15, 1, 0},
        .activate = FB_ACTIVATE_NOW,
        .pixclock = -1,    /* pixel clock in ps (pico seconds) */
        .left_margin = 0,  /* time from sync to picture */
        .right_margin = 0, /* time from picture to sync */
        .upper_margin = 0, /* time from sync to picture */
        .lower_margin = 0,
        .hsync_len = 0, /* length of horizontal sync */
        .vsync_len = 0, /* length of vertical sync */
    },
    /* for cursor layer */
    {
        .xres = HIFB_CURSOR_DEF_WIDTH,
        .yres = HIFB_CURSOR_DEF_HEIGHT,
        .xres_virtual = HIFB_CURSOR_DEF_WIDTH,
        .yres_virtual = HIFB_CURSOR_DEF_HEIGHT,
        .xoffset = 0,
        .yoffset = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red = {10, 5, 0},
        .green = {5, 5, 0},
        .blue = {0, 5, 0},
        .transp = {15, 1, 0},
        .activate = FB_ACTIVATE_NOW,
        .pixclock = -1,    /* pixel clock in ps (pico seconds) */
        .left_margin = 0,  /* time from sync to picture */
        .right_margin = 0, /* time from picture to sync */
        .upper_margin = 0, /* time from sync to picture */
        .lower_margin = 0,
        .hsync_len = 0, /* length of horizontal sync */
        .vsync_len = 0, /* length of vertical sync */
    }
};
#else
static hi_ulong hifb_get_smem_start(struct hifb_info *info)
{
    return (hi_ulong)(uintptr_t)info->oinfo.fbmem;
}

static hi_s8 *hifb_get_screen_base(struct hifb_info *info)
{
    return (hi_s8 *)info->oinfo.fbmem;
}

static hi_u32 hifb_get_xres(struct hifb_info *info)
{
    return (hi_u32)info->vinfo.xres;
}

static hi_u32 hifb_get_yres(struct hifb_info *info)
{
    return (hi_u32)info->vinfo.yres;
}

static hi_u32 hifb_get_xres_virtual(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.sarea.w;
}

static hi_u32 hifb_get_yres_virtual(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.sarea.h;
}

static hi_u32 hifb_get_smem_len(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.fblen;
}

static hi_u32 hifb_get_line_length(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.stride;
}

static hi_u32 hifb_get_bits_per_pixel(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.bpp;
}

static hi_u32 hifb_get_yoffset(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.sarea.y;
}

static hi_u32 hifb_get_xoffset(struct hifb_info *info)
{
    return (hi_u32)info->oinfo.sarea.x;
}

struct hifb_screeninfo {
    uint8_t fmt;            /* see FB_FMT_*  */
    fb_coord_t xres;        /* Horizontal resolution in pixel columns */
    fb_coord_t yres;        /* Vertical resolution in pixel rows */
    struct fb_area_s sarea; /* Selected area within the overlay */
    fb_coord_t stride;      /* Length of a line in bytes */
    uint8_t bpp;            /* Bits per pixel */
    uint32_t accl;          /* Supported hardware acceleration */
    uint8_t nplanes;        /* Number of color planes supported */
    uint8_t noverlays;      /* Number of overlays supported */
};

static struct hifb_screeninfo g_default_info[HIFB_LAYER_TYPE_BUTT] = {
    /* for HD layer */
    {
        .fmt = HIFB_FMT_ARGB1555,
        .xres = HIFB_HD_DEF_WIDTH,
        .yres = HIFB_HD_DEF_HEIGHT,
        .sarea = {0, 0, HIFB_HD_DEF_WIDTH, HIFB_HD_DEF_HEIGHT},
        .stride = HIFB_HD_DEF_STRIDE,
        .bpp = HIFB_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for SD layer */
    {
        .fmt = HIFB_FMT_ARGB1555,
        .xres = HIFB_SD_DEF_WIDTH,
        .yres = HIFB_SD_DEF_HEIGHT,
        .sarea = {0, 0, HIFB_SD_DEF_WIDTH, HIFB_SD_DEF_HEIGHT},
        .stride = HIFB_SD_DEF_STRIDE,
        .bpp = HIFB_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for AD layer */
    {
        .fmt = HIFB_FMT_ARGB1555,
        .xres = HIFB_AD_DEF_WIDTH,
        .yres = HIFB_AD_DEF_HEIGHT,
        .sarea = {0, 0, HIFB_AD_DEF_WIDTH, HIFB_AD_DEF_HEIGHT},
        .stride = HIFB_AD_DEF_STRIDE,
        .bpp = HIFB_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for cursor layer */
    {
        .fmt = HIFB_FMT_ARGB1555,
        .xres = HIFB_CURSOR_DEF_WIDTH,
        .yres = HIFB_CURSOR_DEF_HEIGHT,
        .sarea = {0, 0, HIFB_CURSOR_DEF_WIDTH, HIFB_CURSOR_DEF_HEIGHT},
        .stride = HIFB_CURSOR_DEF_STRIDE,
        .bpp = HIFB_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
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

/*
 * Name : g_argb_bit_field
 * Desc : bit fields of each color format in HIFB_COLOR_FMT_E,
 *        the order must be the same as that of HIFB_COLOR_FMT_E
 */
static hifb_argb_bitinfo g_argb_bit_field[] = {
    {
        .red    = {11, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {0, 5, 0},
        .transp = {0, 0, 0},
    },
    /* RGB888 */
    {
        .red    = {16, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {0, 8, 0},
        .transp = {0, 0, 0},
    },
    /* KRGB444 */
    {
        .red    = {8, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {0, 4, 0},
        .transp = {0, 0, 0},
    },
    /* KRGB555 */
    {
        .red    = {10, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {0, 5, 0},
        .transp = {0, 0, 0},
    },
    /* KRGB888 */
    {
        .red    = {16, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {0, 8, 0},
        .transp = {0, 0, 0},
    },
    /* ARGB4444 */
    {
        .red    = {8, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {0, 4, 0},
        .transp = {12, 4, 0},
    },
    /* ARGB1555 */
    {
        .red    = {10, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {0, 5, 0},
        .transp = {15, 1, 0},
    },
    /* ARGB8888 */
    {
        .red    = {16, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {0, 8, 0},
        .transp = {24, 8, 0},
    },
    /* ARGB8565 */
    {
        .red    = {11, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {0, 5, 0},
        .transp = {16, 8, 0},
    },
    /* RGBA4444 */
    {
        .red    = {12, 4, 0},
        .green  = {8, 4, 0},
        .blue   = {4, 4, 0},
        .transp = {0, 4, 0},
    },
    /* RGBA5551 */
    {
        .red    = {11, 5, 0},
        .green  = {6, 5, 0},
        .blue   = {1, 5, 0},
        .transp = {0, 1, 0},
    },
    /* RGBA5658 */
    {
        .red    = {19, 5, 0},
        .green  = {13, 6, 0},
        .blue   = {8, 5, 0},
        .transp = {0, 8, 0},
    },
    /* RGBA8888 */
    {
        .red    = {24, 8, 0},
        .green  = {16, 8, 0},
        .blue   = {8, 8, 0},
        .transp = {0, 8, 0},
    },
    /* BGR565 */
    {
        .red    = {0, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {11, 5, 0},
        .transp = {0, 0, 0},
    },
    /* BGR888 */
    {
        .red    = {0, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {16, 8, 0},
        .transp = {0, 0, 0},
    },
    /* ABGR4444 */
    {
        .red    = {0, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {8, 4, 0},
        .transp = {12, 4, 0},
    },
    /* ABGR1555 */
    {
        .red    = {0, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {10, 5, 0},
        .transp = {15, 1, 0},
    },
    /* ABGR8888 */
    {
        .red    = {0, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {16, 8, 0},
        .transp = {24, 8, 0},
    },
    /* ABGR8565 */
    {
        .red    = {0, 5, 0},
        .green  = {5, 6, 0},
        .blue   = {11, 5, 0},
        .transp = {16, 8, 0},
    },
    /* KBGR444 16bpp */
    {
        .red    = {0, 4, 0},
        .green  = {4, 4, 0},
        .blue   = {8, 4, 0},
        .transp = {0, 0, 0},
    },
    /* KBGR555 16bpp */
    {
        .red    = {0, 5, 0},
        .green  = {5, 5, 0},
        .blue   = {10, 5, 0},
        .transp = {0, 0, 0},
    },
    /* KBGR888 32bpp */
    {
        .red    = {0, 8, 0},
        .green  = {8, 8, 0},
        .blue   = {16, 8, 0},
        .transp = {0, 0, 0},
    },

    /* 1bpp */
    {
        .red = {0, 1, 0},
        .green = {0, 1, 0},
        .blue = {0, 1, 0},
        .transp = {0, 0, 0},
    },
    /* 2bpp */
    {
        .red = {0, 2, 0},
        .green = {0, 2, 0},
        .blue = {0, 2, 0},
        .transp = {0, 0, 0},
    },
    /* 4bpp */
    {
        .red = {0, 4, 0},
        .green = {0, 4, 0},
        .blue = {0, 4, 0},
        .transp = {0, 0, 0},
    },
    /* 8bpp */
    {
        .red = {0, 8, 0},
        .green = {0, 8, 0},
        .blue = {0, 8, 0},
        .transp = {0, 0, 0},
    },
    /* ACLUT44 */
    {
        .red = {4, 4, 0},
        .green = {4, 4, 0},
        .blue = {4, 4, 0},
        .transp = {0, 4, 0},
    },
    /* ACLUT88 */
    {
        .red = {8, 8, 0},
        .green = {8, 8, 0},
        .blue = {8, 8, 0},
        .transp = {0, 8, 0},
    }
};

static hi_void hifb_version(hi_void);
#ifdef CURSOR
static hi_void hifb_cursor_calcdispinfo(hi_u32 cursor_id, hifb_par *par, const HIFB_POINT_S *cur_new_pos);
#endif
static hi_s32 hifb_refresh(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf, HIFB_LAYER_BUF_E buf_mode);
static hi_void hifb_select_antiflicker_mode(hifb_par *par);
static hi_s32 hifb_setcolreg(unsigned regno, unsigned red, unsigned green,
                             unsigned blue, unsigned transp, struct fb_info *info);

bool g_bUpdateRotateRect = HI_FALSE;
hifb_layer g_layer[HIFB_MAX_LAYER_NUM];
char g_video[64] = "hifb:vram0_size:8100"; /* 64 The length of the array */
#ifndef __HuaweiLite__
module_param(g_bUpdateRotateRect, bool, S_IRUGO);
module_param_string(video, g_video, 64, 0); /* 64 The length of the array */
#endif

static char g_display[8] = "off"; /* 8 open vo or not */

static char g_softcursor[8] = "off"; /* 8 use soft cursor or not */

bool hifb_update_rotate_rect(hi_void)
{
    return g_bUpdateRotateRect;
}

/*
 * Name : hifb_completely
 * Desc : fill buffer, it is faster than memset.
 * See  : Called by hifb_overlay_probe
 */
static hi_u32 hifb_completely(struct fb_info *info, hi_u32 fill_data, size_t n)
{
    hi_s32 ret;
    HIFB_BUFFER_S dst_img;
    hi_s8 *rest = HI_NULL;
    hi_u32 can_pitch;

    can_pitch = HIFB_TDE_DISTINCT_WIDTH * 4; /* 4 alg data */
    if (info == HI_NULL) {
        hifb_error("The pointer is HI_NULL\n");
        return HI_FAILURE;
    }
    /*
     * Assume that the image width is 3840, the format is ARBG8888, and the image is filled by TDE.
     * Since the size of the image that TDE can handle is limited,
     * the width and height are limited to HIFB_TDE_DISTINCT_WIDTH
     * and HIFB_TDE_DISTINCT_HEIGHT, Exceeded parts are cleared using memset
     * Configure the target image, pitch, format, width and height, physical address (pointing to the memory start
     * address)
     */
    dst_img.stCanvas.u64PhyAddr = hifb_get_smem_start(info);
    dst_img.stCanvas.u32Pitch = can_pitch;
    dst_img.stCanvas.enFmt = HIFB_FMT_ABGR8888;
    dst_img.stCanvas.u32Width = HIFB_TDE_DISTINCT_WIDTH;
    dst_img.stCanvas.u32Height = n / can_pitch;

    if (dst_img.stCanvas.u32Height > HIFB_TDE_DISTINCT_HEIGHT) {
        dst_img.stCanvas.u32Height = HIFB_TDE_DISTINCT_HEIGHT;
    }

    if (dst_img.stCanvas.u32Height != 0) {
        ret = hifb_drv_fill(&dst_img, fill_data);
        if (ret != HI_SUCCESS) {
            hifb_error("hifb_drv_fill fail!\n");
            return HI_FAILURE;
        }
    }

    /* The memory not in the canvas is set by memset */
    if ((n - dst_img.stCanvas.u32Height * can_pitch) != 0) {
        rest = hifb_get_screen_base(info) + can_pitch * (dst_img.stCanvas.u32Height);
        osal_memset(rest, fill_data, n - dst_img.stCanvas.u32Height * can_pitch);
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
    int i;
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        struct fb_info *info = g_layer[i].info;

        if (info != HI_NULL) {
            hifb_completely(info, 0, hifb_get_smem_len(info));
        }
    }

    return 0;
}

static struct osal_fileops g_hifb_file_op = {.unlocked_ioctl = HI_NULL, .open = HI_NULL, .release = HI_NULL};

struct osal_pmops g_hifb_drv_ops = {
    .pm_freeze = hifb_freeze,
    .pm_restore = hifb_restore,
};
#endif

/*
 * Name : hifb_alloc_compress_buffer
 * Desc : the compression is TRUE, the memory is allocated and the address is put in the compressed information of the
 * private data.
 */
hi_s32 hifb_alloc_compress_buffer(hifb_par *par)
{
    hi_u32 buf_height;
    hi_u32 buf_stride;
    hi_u32 header_size;
    hi_u32 buf_size;
    hi_u64 buf_phy_addr;
    hi_u32 layer_index;

    if (par == HI_NULL) {
        return HI_FAILURE;
    }

    layer_index = par->layer_id;
    if (layer_index >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    buf_height = par->display_info.display_height;
    /* 2 alg data */
    buf_stride = (par->display_info.display_width * 2 + COMPRESS_STRIDE_ALIGNMENT - 1) &
                   (~(COMPRESS_STRIDE_ALIGNMENT - 1));
    header_size = COMPRESS_HEADER_STRIDE * buf_height;
    buf_size = buf_stride * buf_height + header_size;
    /* 2 alg data 16 size buffer */
    buf_phy_addr = hifb_buf_allocmem("Compress buffer", 16, 2 * buf_size, hifb_get_layer_mmz_names(layer_index));
    if (buf_phy_addr == 0) {
        hifb_trace(HI_DBG_ERR, "Mmz Malloc Compress buffer fail\n");
        return HI_FAILURE;
    }

    par->compress_info.ar_buf_phy_addr = buf_phy_addr;
    par->compress_info.gb_buf_phy_addr = buf_phy_addr + buf_size;
    par->compress_info.header_size = header_size;
    par->compress_info.stride = buf_stride;

    return HI_SUCCESS;
}

hi_void hifb_free_compress_buffer(hifb_par *par)
{
    if (par == HI_NULL) {
        return;
    }

    if (par->compress_info.ar_buf_phy_addr != 0) {
        hifb_buf_freemem(par->compress_info.ar_buf_phy_addr);
        par->compress_info.ar_buf_phy_addr = 0;
    }
}

/*
 * Name : hifb_set_dcmp_info
 * Desc : set the decompression info.
 */
hi_void hifb_set_dcmp_info(hifb_par *par)
{
    volatile hifb_compress_info *compress_info = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    fb_vo_graphic_dcmp_info dcmp_info;
    HIFB_COLOR_FMT_E hifb_color_fmt;

    if (par == HI_NULL) {
        return;
    }

    compress_info = &par->compress_info;
    refresh_info = &par->refresh_info;

    if ((refresh_info->screen_addr == 0) || (refresh_info->gb_screen_addr == 0)) {
        /* close the dcmp */
        g_drv_ops.hifb_drv_enable_dcmp(par->layer_id, HI_FALSE);
        hifb_trace(HI_DBG_WARN, "Compression is opened, but compressed buffer phyaddr for refreshing is HI_NULL(0)\n");
        return;
    }

    /*
     * Decompressing information from compressed information: compression channels AR, GB address, and its size size0,
     * size1
     */
    hifb_color_fmt = refresh_info->user_buffer.stCanvas.enFmt;
    switch (hifb_color_fmt) {
        case HIFB_FMT_ARGB4444:
            dcmp_info.pixel_fmt = FB_VO_INPUTFMT_ARGB_4444;
            break;
        case HIFB_FMT_ARGB1555:
            dcmp_info.pixel_fmt = FB_VO_INPUTFMT_ARGB_1555;
            break;
        case HIFB_FMT_ARGB8888:
            dcmp_info.pixel_fmt = FB_VO_INPUTFMT_ARGB_8888;
            break;
        default:
            hifb_trace(HI_DBG_ERR, "pixel format(%d) is invalid!\n", hifb_color_fmt);
            return;
    }
    dcmp_info.ar_phy_addr = hifb_drv_get_dcmp_offset_addr(refresh_info->screen_addr);
    dcmp_info.gb_phy_addr = hifb_drv_get_dcmp_offset_addr(refresh_info->gb_screen_addr);
    dcmp_info.width = refresh_info->user_buffer.stCanvas.u32Width;
    dcmp_info.height = refresh_info->user_buffer.stCanvas.u32Height;
    dcmp_info.stride = refresh_info->disp_buf_info.stride;
    dcmp_info.frame_size0 = compress_info->frame_size0;
    dcmp_info.frame_size1 = compress_info->frame_size1;
    dcmp_info.is_lossless_a = HI_FALSE; /* Whether lossless,FALSE:Lossy,TRUE:lossless */
    dcmp_info.is_lossless = HI_FALSE;  /* Whether lossless,FALSE:Lossy,TRUE:lossless */

    /* Decompressing information to drv level */
    if (g_drv_ops.hifb_drv_set_dcmp_info(par->layer_id, &dcmp_info) == HI_FALSE) {
        /* close dcmp */
        g_drv_ops.hifb_drv_enable_dcmp(par->layer_id, HI_FALSE);
    }
}

#ifdef CONFIG_HI_VO_FB_SEPARATE
static int hifb_interrupt_route(hi_s32 irq, hi_void *dev_id);
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
static hi_s32 hifb_soft_interrupt(hi_s32 irq, hi_void *dev_id);
#endif

#ifdef __HuaweiLite__
static hi_s32 hifb_debug_info_init(hi_void)
{
#ifndef DISABLE_DEBUG_INFO
    hi_s32 i;
    hi_s32 j;
    hi_char entry_name[16]; /* 16 name size */

    hifb_proc_init();
    /* initialize fb file according the config */
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        /* register the layer */
        if (hifb_overlay_probe(i) == HI_SUCCESS) {
            /* create a proc entry in 'hifb' for the layer */
            osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", i);
            hifb_proc_add_module(entry_name, 16, hifb_read_proc, hifb_write_proc, /* 16 name size */
                g_layer[i].info);
        } else {
            /* roll back hifb0~hifbi-1's proc entry, remove them.  */
            j = i;
            for (j = i - 1; j >= 0; j--) {
                /* destroy a proc entry in 'hifb' for the layer */
                osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", j);
                hifb_proc_remove_module(entry_name);
                /* unregister the layer */
                hifb_overlay_cleanup(j, HI_TRUE);
            }
            printk("ERROR: Load hifb.ko FAILED!\n");
            return HI_FAILURE;
        }
    }
#endif
    return HI_SUCCESS;
}
#else
static hi_s32 hifb_debug_info_init(hi_void)
{
    hi_s32 i;
    hi_s32 j;
    hi_char entry_name[16]; /* 16 name size */

    hifb_proc_init();
    /* initialize fb file according the config */
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        /* register the layer */
        if (hifb_overlay_probe(i) == HI_SUCCESS) {
            /* create a proc entry in 'hifb' for the layer */
            osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", i);
#ifndef DISABLE_DEBUG_INFO
            hifb_proc_add_module(entry_name, 16, hifb_read_proc, hifb_write_proc, /* 16 name size */
                                 g_layer[i].info);
#endif
        } else {
            /* roll back hifb0~hifbi-1's proc entry, remove them. */
            j = i;
            for (j = i - 1; j >= 0; j--) {
                /* destroy a proc entry in 'hifb' for the layer */
                osal_snprintf(entry_name, sizeof(entry_name), "hifb%d", j);
#ifndef DISABLE_DEBUG_INFO
                hifb_proc_remove_module(entry_name);
#endif
                /* unregister the layer */
                hifb_overlay_cleanup(j, HI_TRUE);
            }
            printk("ERROR: Load hifb.ko FAILED!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}
#endif

static hi_s32 hifb_init_process(hi_void)
{
    /* open vo */
    if (g_display_on) {
        g_drv_ops.hifb_drv_open_display();
    }

    /* inital adoption layer */
    if (g_drv_ops.hifb_drv_init() != HI_SUCCESS) {
        hifb_error("drv init failed\n");
        return HI_FAILURE;
    }

    hifb_drv_set_tde_callback(hifb_tde_callback);
    hifb_drv_set_tde_rotate_callback(hifb_tde_rotate_callback);

    return HI_SUCCESS;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static osal_dev_t *g_hifb_device;
#endif
static hi_s32 hifb_snapshot(hi_void)
{
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    hi_s32 ret;
    char buf[20]; /* 20 is Array bounds */
    osal_snprintf(buf, 20, "%s", DEV_NAME); /* 20 is Array bounds */
    g_hifb_device = osal_createdev(buf);
    g_hifb_device->fops = &g_hifb_file_op;
    g_hifb_device->osal_pmops = &g_hifb_drv_ops;
    g_hifb_device->minor = HIMEDIA_DYNAMIC_MINOR;
    ret = osal_registerdevice(g_hifb_device);
    if (ret != HI_SUCCESS) {
        osal_destroydev(g_hifb_device);
        hifb_error("osal_registerdevice failed\n");
        return ret;
    }
#endif
    return HI_SUCCESS;
}

static hi_s32 hifb_request_irq(hi_void)
{
#ifdef CONFIG_HI_VO_FB_SEPARATE
        if (osal_request_irq(g_fifb_irq, hifb_interrupt_route, HI_NULL, "HIFB Int", hifb_interrupt_route)) {
            g_hifb_register = HI_FALSE;
            hifb_error("request_irq for Hifb failure!\n");
#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
            if (g_hifb_soft_irq_register == HI_TRUE) {
                osal_free_irq(g_fifb_soft_irq, hifb_soft_interrupt);
                g_hifb_soft_irq_register = HI_FALSE;
            }
#endif
            if (g_hifb_irq_register == HI_TRUE) {
                osal_free_irq(g_fifb_irq, hifb_interrupt_route);
                g_hifb_irq_register = HI_FALSE;
            }
            return HI_FAILURE;
        }
        g_hifb_irq_register = HI_TRUE;
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
        if (osal_request_irq(g_fifb_soft_irq, hifb_soft_interrupt, HI_NULL, "HIFB_SOFT_INT", hifb_soft_interrupt)) {
            g_hifb_register = HI_FALSE;
            hifb_error("request_irq for hifb soft irq failure!\n");
            if (g_hifb_soft_irq_register == HI_TRUE) {
                osal_free_irq(g_fifb_soft_irq, hifb_soft_interrupt);
                g_hifb_soft_irq_register = HI_FALSE;
            }
#ifdef CONFIG_HI_VO_FB_SEPARATE
            if (g_hifb_irq_register == HI_TRUE) {
                osal_free_irq(g_fifb_irq, hifb_interrupt_route);
                g_hifb_irq_register = HI_FALSE;
            }
#endif
            return HI_FAILURE;
        }
        g_hifb_soft_irq_register = HI_TRUE;
#endif
    return HI_SUCCESS;
}

static hi_void hifb_init_do_err2(hi_void)
{
#ifdef __HuaweiLite__
    g_drv_ops.hifb_drv_deinit();
#else
#ifndef CONFIG_HI_HIFB_VGS
    g_drv_ops.hifb_drv_deinit();

#endif
#endif
    hifb_drv_set_tde_callback(HI_NULL);
    hifb_drv_set_tde_rotate_callback(HI_NULL);
}

static hi_void hifb_init_do_err1(hi_void)
{
    hi_s32 i;
    /* close vo */
    if (g_display_on) {
        g_drv_ops.hifb_drv_close_display();
    }

    for (i = 0; i < g_drv_ops.layer_count; i++) {
        hifb_overlay_cleanup(i, HI_TRUE);
    }

#ifndef DISABLE_DEBUG_INFO
    hifb_proc_remove_all_module();
#endif
    printk("ERROR: Load hifb.ko FAILED!\n");
    return;
}

/*
 * Function        : hifb_init
 * Description     : initialize framebuffer. the function is called when
                     loading the moudel
 * Return          : 0, if success; otherwise, return error code
 */
#ifdef __HuaweiLite__
hi_s32 __init hifb_init(hi_void *args)
{
    HIFB_MODULE_PARAMS_S *hifb_module_param = (HIFB_MODULE_PARAMS_S *)args;
    if (hifb_module_param != HI_NULL) {
        osal_strncpy(g_video, hifb_module_param->video, 63); /* 63 is video boundary */
        g_video[63] = '\0';
        g_bUpdateRotateRect = hifb_module_param->bUpdateRotateRect;
    }
#ifndef DISABLE_DEBUG_INFO
    hifb_proc_init();
#endif
    /* get the chip operation method */
    hifb_drv_get_ops(&g_drv_ops);

    /* parse the \arg video && g_softcursor && g_display string */
    if (hifb_parse_cfg() < 0) {
        hifb_error("Usage:insmod hifb.ko video=\"hifb:vrami_size:xxx,vramj_size:xxx,...\"\n");
        hifb_error("i,j means layer id, xxx means layer size in kbytes!\n");
        hifb_error("example:insmod hifb.ko video=\"hifb:vram0_size:810,vram1_size:810\"\n\n");
        return HI_FAILURE;
    }

    if (hifb_debug_info_init() != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /* open vo and inital layer */
    if (hifb_init_process() != HI_SUCCESS) {
        goto err1;
    }

    if ((hifb_snapshot() != HI_SUCCESS) || (hifb_request_irq() != HI_SUCCESS)) {
        goto err2;
    }

    /* show version */
    hifb_version();
    printk("Load hifb.ko OK!\n");
    return 0;

err2:
    hifb_init_do_err2();
err1:
    hifb_init_do_err1();
    return HI_FAILURE;
}
#else
static hi_s32 __init hifb_init(hi_void)
{
    /* make dir 'hifb' under proc/graphics */
#ifndef DISABLE_DEBUG_INFO
    hifb_proc_init();
#endif

    /* get the chip operation method */
    hifb_drv_get_ops(&g_drv_ops);
    /* parse the \arg video && g_softcursor && g_display string */
    if (hifb_parse_cfg() < 0) {
        hifb_error("Usage:insmod hifb.ko video=\"hifb:vrami_size:xxx,vramj_size:xxx,...\"\n");
        hifb_error("i,j means layer id, xxx means layer size in kbytes!\n");
        hifb_error("example:insmod hifb.ko video=\"hifb:vram0_size:1620,vram1_size:1620\"\n\n");
        g_hifb_register = HI_FALSE;
        printk("ERROR: Load hifb.ko FAILED!\n");
        return HI_FAILURE;
    }

    /* initialize fb file according the config */
    if (hifb_debug_info_init() != HI_SUCCESS) {
        g_hifb_register = HI_FALSE;
        return HI_FAILURE;
    }

    /* open vo and inital layer */
    if (hifb_init_process() != HI_SUCCESS) {
        goto err1;
    }

    /*
     * Interrupt registration interrupt is placed at the end.
     * Since the interrupt will be opened in the boot screen,
     * it will respond when loading ko, and the context is used in the interrupt
     */
    if ((hifb_snapshot() != HI_SUCCESS) || (hifb_request_irq() != HI_SUCCESS)) {
        goto err2;
    }
    /* show version */
    hifb_version();

    (void)hifb_vsync_init();
    (void)hifb_pts_init();

    printk("load hifb.ko OK!\n");

    return 0;

err2:
    hifb_init_do_err2();
err1:
    hifb_init_do_err1();
    g_hifb_register = HI_FALSE;
    return HI_FAILURE;
}
#endif

hi_void __exit hifb_cleanup(hi_void)
{
    hi_s32 i;

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
    if (g_hifb_soft_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_soft_irq, hifb_soft_interrupt);
        g_hifb_soft_irq_register = HI_FALSE;
    }
#endif

#ifdef CONFIG_HI_VO_FB_SEPARATE
    if (g_hifb_irq_register == HI_TRUE) {
        osal_free_irq(g_fifb_irq, hifb_interrupt_route);
        g_hifb_irq_register = HI_FALSE;
    }
#endif

    if (g_hifb_register) {
        hifb_pts_exit();
        hifb_vsync_exit();

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
        osal_deregisterdevice(g_hifb_device);
        osal_destroydev(g_hifb_device);
#endif

        hifb_drv_set_tde_callback(HI_NULL);
        hifb_drv_set_tde_rotate_callback(HI_NULL);

        g_drv_ops.hifb_drv_deinit();

        if (g_display_on) {
            g_drv_ops.hifb_drv_close_display();
        }

        /* remove all entry under dir 'hifb' */
#ifndef DISABLE_DEBUG_INFO
        hifb_proc_remove_all_module();

        /* remove dir 'hifb' under proc */
#endif
        for (i = 0; i < g_drv_ops.layer_count; i++) {
            hifb_overlay_cleanup(i, HI_TRUE);
        }
    }
    osal_printk("unload hifb.ko ... ok!\n");
    return;
}

#ifdef CONFIG_HI_VO_FB_SEPARATE
static int hifb_interrupt_route(hi_s32 irq, hi_void *dev_id)
{
    hi_u32 int_status = 0;
    hifb_vo_dev vo_dev = VO_INVALID_DEV;
    hi_s32 i, ret;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    hi_u32 vtthd_flag = 0x0;
    hi_u32 frm_start_flag = 0x0;

    g_drv_ops.hifb_drv_graphics_get_int(&int_status);
    /*
     * HIFB_DRV_GraphicsClearINTStatus will clear all reported interrupts.
     * If devices' interrupt are reported at the same time,
     * Will cause only one device to be processed, the interruption of other devices is ignored,
     * so it cannot be cleared here.
     * All reported interrupts can only be cleared one by one in HIFB_DRV_GraphicsGetINTDev
     */
    hifb_trace(HI_DBG_DEBUG, "intstatus : %u\n", int_status);

    ret = g_drv_ops.hifb_drv_graphics_get_int_dev(int_status, &vo_dev);
    if (ret != HI_SUCCESS) {
        return OSAL_IRQ_HANDLED;
    }

    /* Graphic layer interrupt g_display processing */
    if (vo_dev == VO_INVALID_DEV) {
        hifb_error("unknow dev:%d \n", vo_dev);
        return OSAL_IRQ_HANDLED;
    }

    /* Handling all graphics layers on this device */
    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        info = g_layer[i].info;
        par = (hifb_par *)(info->par);

        /* If the layer is not open, do not act or if the layer is bound to the device */
        if ((par->layer_open == HI_FALSE) || (vo_dev != graphic_drv_get_bind_dev(i))) {
            continue;
        }

        if (vo_dev == VO_DEV_DHD0) {
            vtthd_flag = HIFB_INTMSK_HD0_VTTHD2;
            frm_start_flag = HIFB_INTMSK_HD0_VTTHD3;
        } else if (vo_dev == VO_DEV_DHD1) {
            vtthd_flag = HIFB_INTMSK_HD1_VTTHD2;
            frm_start_flag = HIFB_INTMSK_HD1_VTTHD3;
        }
        /*
         * 1. The frame start interrupt and the vertical timing interrupt appear
         * in the interrupt status register at the same time.
         * Frame start interrupts and vertical timing interrupts
         * are processed one by one and cannot be processed at the same time because
         * interrupts are cleared one by one, and the interrupt processing order
         * and the clearing order should be also consistent.
         * The first clear must be processed first, then the post-cleared
         * 2, as long as there is a vertical timing interrupt, it is necessary to drive the g_display
         */
        if (int_status & vtthd_flag) { /* VO vertical timing interrupt */
#ifndef CONFIG_HIFB_SOFT_IRQ_SUPPORT
            par->refresh_info.do_refresh_job = HI_FALSE;
            hifb_vo_callback(&i, HI_NULL);
#endif
        } else if (int_status & frm_start_flag) { /* VO frame start interrupt */
            par->refresh_info.do_refresh_job = HI_TRUE;
            wake_up_interruptible(&(par->do_refresh_job));
        }
    }
    return OSAL_IRQ_HANDLED;
}
#endif

#ifdef CONFIG_HIFB_SOFT_IRQ_SUPPORT
static hi_s32 hifb_soft_interrupt(hi_s32 irq, hi_void *dev_id)
{
    hi_s32 i;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;

    for (i = 0; i < VO_MAX_GRAPHICS_LAYER_NUM; ++i) {
        hifb_hal_soft_int_en(HI_FALSE);
        info = g_layer[i].info;
        par = (hifb_par *)(info->par);
        par->refresh_info.do_refresh_job = HI_FALSE;
        if (hifb_vo_callback(&i, HI_NULL) != HI_SUCCESS) {
            hifb_error("hifb_vo_callback HI_FAILURE \r\n");
        }
    }
    return OSAL_IRQ_HANDLED;
}
#endif

static hi_s32 hifb_open_check_param(struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    if ((info == HI_NULL) || (info->par == HI_NULL)) {
        return HI_FAILURE;
    }

    par = (hifb_par *)info->par;
    if (par->layer_id >= HIFB_MAX_LAYER_NUM) {
        hifb_error("layer %d is not supported!\n", par->layer_id);
        return HI_FAILURE;
    }

    /* assure layer is legal */
    if (!g_drv_ops.capability[par->layer_id].bLayerSupported) {
        hifb_error("layer %d is not supported!\n", par->layer_id);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_bool hifb_open_is_cursor_layer(struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    HIFB_ALPHA_S alpha = {HI_TRUE, HI_FALSE, HIFB_ALPHA_TRANSPARENT,
                          HIFB_ALPHA_OPAQUE, HIFB_ALPHA_OPAQUE, 0};

    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        if (!atomic_read(&par->ref_count)) {
            osal_memset(par, 0, sizeof(hifb_par));

            par->layer_id = layer_id;
            atomic_set(&par->ref_count, 0);

            /* Configuring alpha properties */
            hifb_set_alpha(par, &alpha);
        }
        atomic_inc(&par->ref_count);

        return HI_TRUE;
    }
    return HI_FALSE;
}

#ifdef __HuaweiLite__
static hi_s32 hifb_open_default_param(struct fb_info *info, HIFB_COLOR_FMT_E *color_format)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if (is_hd_layer(layer_id)) {
        hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_HD]);
    } else if (is_sd_layer(layer_id)) {
        hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_SD]);
    } else if (is_ad_layer(layer_id)) {
        hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_AD]);
    }else if (is_cursor_layer(layer_id)) {
        hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_CURSOR]);
    } else {
        hifb_error("error layer id:%d\n", par->layer_id);
    }
    *color_format = info->vinfo.fmt;
    if (*color_format == HIFB_FMT_BUTT) {
        hifb_error("Invalid default color format!\n");
        return HI_FAILURE;
    }

    info->oinfo.stride = (info->oinfo.sarea.w * (info->oinfo.bpp >> 3) + HIFB_ALIGNMENT) & /* 8 is bits 2^3 */
                          (~HIFB_ALIGNMENT);

    return HI_SUCCESS;
}
#else
static hi_s32 hifb_open_default_param(struct fb_info *info, HIFB_COLOR_FMT_E *color_format)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    struct fb_var_screeninfo *var = HI_NULL;
    struct fb_fix_screeninfo *fix = HI_NULL;

    var = &info->var;
    fix = &info->fix;

    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if (is_hd_layer(layer_id)) {
        *var = g_default_var[HIFB_LAYER_TYPE_HD];
    } else if (is_sd_layer(layer_id)) {
        *var = g_default_var[HIFB_LAYER_TYPE_SD];
    } else if (is_ad_layer(layer_id)) {
        *var = g_default_var[HIFB_LAYER_TYPE_AD];
    } else if (is_cursor_layer(layer_id)) {
        *var = g_default_var[HIFB_LAYER_TYPE_CURSOR];
    } else {
        hifb_error("error layer id:%d\n", par->layer_id);
    }
    /* transform colorfmt form bitfiled to hifb fmt, and record it */
    *color_format = hifb_getfmtbyargb(&var->red, &var->green, &var->blue,
        &var->transp, var->bits_per_pixel);
    if (*color_format == HIFB_FMT_BUTT) {
        hifb_error("Invalid default color format!\n");
        return HI_FAILURE;
    }
    /* By default it is width*4 (32 bits per pixel / 8(2^3)) */
    fix->line_length = (var->xres_virtual * (var->bits_per_pixel >> 3) + HIFB_ALIGNMENT) & /* 8 is bits 2^3 */
                        (~HIFB_ALIGNMENT);
    return HI_SUCCESS;
}
#endif

static hi_void hifb_open_init_display(struct fb_info *info, hi_u32 layer_id, hifb_osd_data *osd_data)
{
    hifb_par *par = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    par = (hifb_par *)info->par;

    osal_memset(par, 0, sizeof(hifb_par));
    par->layer_id = layer_id;
    atomic_set(&par->ref_count, 0);

    display_info = &par->display_info;
    display_info->display_width    = hifb_get_xres(info);
    display_info->display_height   = hifb_get_yres(info);
    display_info->screen_width     = hifb_get_xres(info);
    display_info->screen_height    = hifb_get_yres(info);
    display_info->vir_x_res         = hifb_get_xres_virtual(info);
    display_info->vir_y_res         = hifb_get_yres_virtual(info);
    display_info->x_res            = hifb_get_xres(info);
    display_info->y_res            = hifb_get_yres(info);
    display_info->max_screen_width  = osd_data->screen_width;
    display_info->max_screen_height = osd_data->screen_height;
    display_info->mirror_mode       = HIFB_MIRROR_NONE;
    display_info->rotate_mode       = HIFB_ROTATE_NONE;
    par->rotate_vb = 0;

    init_waitqueue_head(&(par->vbl_event));
    init_waitqueue_head(&(par->do_refresh_job));

    return;
}

static hi_void hifb_open_init_config(struct fb_info *info, hi_u32 layer_id, hifb_osd_data *osd_data,
                                     HIFB_COLOR_FMT_E color_format, hifb_cursor_info *cursor_info)
{
    hifb_par *par = HI_NULL;
    HIFB_ALPHA_S alpha = {HI_TRUE, HI_FALSE, HIFB_ALPHA_TRANSPARENT,
                          HIFB_ALPHA_OPAQUE, HIFB_ALPHA_OPAQUE, 0};
    hifb_display_info *display_info = HI_NULL;
    unsigned long lock_flag;
    par = (hifb_par *)info->par;
    display_info = &par->display_info;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    hifb_set_bufmode(par->layer_id, HIFB_LAYER_BUF_BUTT);
    hifb_set_alpha(par, &alpha);
    hifb_set_dispbufinfo(layer_id);
    hifb_set_fmt(par, color_format);
    /* Anti-flicker when interlaced */
    display_info->need_antiflicker = (osd_data->scan_mode == HIFB_SCANMODE_I) ? (HI_TRUE) : (HI_FALSE);
    hifb_set_antiflickerlevel(par->layer_id, HIFB_LAYER_ANTIFLICKER_AUTO);

#ifdef CURSOR
    if (is_soft_cursor() && !is_cursor_layer(layer_id)) {
        cursor_info->cursor.stCursor.u64PhyAddr = hifb_get_smem_start(info) + hifb_get_smem_len(info);
        cursor_info->cursor.stCursor.u32Pitch = HIFB_SOFTCURSOR_STRIDE;
        cursor_info->cursor.stCursor.enFmt = color_format;
        hifb_set_cursorinfo(par, cursor_info);
    }
#endif
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    g_drv_ops.hifb_drv_layer_default_setting(layer_id);
    g_drv_ops.hifb_drv_set_layer_alpha(layer_id, par->alpha);
    g_drv_ops.hifb_drv_set_layer_data_fmt(layer_id, par->color_format);
}

static hi_void hifb_open_init_compress(struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    HIFB_RECT rect;
    hi_u32 layer_id;
    par = (hifb_par *)info->par;
    display_info = &par->display_info;
    layer_id = par->layer_id;
    compress_info = &par->compress_info;
    refresh_info = &par->refresh_info;
#ifdef MDDRDETECT
    /* Memory detection is open by default. This function is not supported at this time. */
    g_drv_ops.hifb_drv_get_ddr_zone_cfg(layer_id, (hi_u32*)&compress_info->start_section,
        (hi_u32*)&compress_info->zone_nums);
#endif
    /* Decompression information settings */
    compress_info->new_start_section = compress_info->start_section;
    compress_info->new_zone_nums = compress_info->zone_nums;
    compress_info->clear_zone = HI_FALSE;

    g_drv_ops.hifb_drv_enable_dcmp(layer_id, HI_FALSE); /* Clear decompression status */

    rect.x = 0;
    rect.y = 0;
    rect.w = hifb_get_xres(info);
    rect.h = hifb_get_yres(info);

    g_drv_ops.hifb_drv_set_layer_rect(par->layer_id, &rect, &rect);
    g_drv_ops.hifb_drv_set_layer_src_image_reso(par->layer_id, &rect);

    g_drv_ops.hifb_drv_set_layer_stride(par->layer_id, hifb_get_line_length(info));

    g_drv_ops.hifb_drv_set_pre_mul(par->layer_id, display_info->premul);
    if (g_drv_ops.capability[layer_id].bKeyRgb || g_drv_ops.capability[layer_id].bKeyAlpha) {
        g_drv_ops.hifb_drv_set_layer_key_mask(par->layer_id, &par->ckey);
    }

    /* Set to memory address */
    g_drv_ops.hifb_drv_set_layer_addr(par->layer_id, hifb_get_smem_start(info));
    refresh_info->screen_addr = hifb_get_smem_start(info);
    refresh_info->gb_screen_addr = 0x0; /* Clear the decompression address */
    compress_info->layer_addr_update = HI_TRUE;

    return;
}

static hi_void hifb_open_init_finish(struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    unsigned long lock_flag;

    par = (hifb_par *)info->par;
    compress_info = &par->compress_info;

#ifndef CONFIG_HI_VO_FB_SEPARATE
    /* The soft interrupt handlers are registered in the context of the graphics layer. */
    g_drv_ops.hifb_drv_set_int_callback(HIFB_INTTYPE_VO, hifb_vo_callback,
                                        par->layer_id, &par->layer_id);
#endif
    g_drv_ops.hifb_drv_set_int_callback(HIFB_INTTYPE_WBC, hifb_wbc_callback,
                                        par->layer_id, &par->layer_id);

    hifb_spin_lock_irqsave(&par->lock, lock_flag);

    compress_info->compress_open = HI_FALSE;

    par->modifying = HI_TRUE;
    hifb_set_show(par, HI_TRUE);
    par->param_modify_mask = HIFB_LAYER_PARAMODIFY_SHOW;
    par->modifying = HI_FALSE;
    par->layer_open = HI_TRUE;

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* enable layer for vo to decide config csc or not */
    g_drv_ops.hifb_drv_enable_layer(par->layer_id, HI_TRUE);

    return;
}

static hi_s32 hifb_open_start(struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    hifb_cursor_info cursor_info;
    hifb_osd_data osd_data = {0};
    HIFB_COLOR_FMT_E color_format;
    hi_u32 layer_id;

    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if (!atomic_read(&par->ref_count)) {
#if (HICHIP == HI3516E_V200)
        hifb_graphics_init();
#else
        vou_graphics_init();
#endif
        osal_memset(&cursor_info, 0, sizeof(hifb_cursor_info));
        if (g_drv_ops.hifb_drv_get_osd_data(layer_id, &osd_data) != HI_SUCCESS) {
            hifb_error("Failed to get osd data!\n");
            return HI_FAILURE;
        }

        /* Configure the layer's default variable parameter var_info according to the type of layer */
        if (g_drv_ops.hifb_open_layer(layer_id) != HI_SUCCESS) {
            hifb_error("Open graphic layer %d# failed!\n", layer_id);
            return HI_FAILURE;
        }

        /* get default var and fix */
        if (hifb_open_default_param(info, &color_format) != HI_SUCCESS) {
            return HI_FAILURE;
        }

        /* Initialize the display information in private data */
        hifb_open_init_display(info, layer_id, &osd_data);

        /* Initialization lock */
        hifb_spin_lock_init(&par->lock);

        /* hifb set bufmode\alpha\displaybufinfo\fmt */
        hifb_open_init_config(info, layer_id, &osd_data, color_format, &cursor_info);

        /* hifb compress init */
        hifb_open_init_compress(info);

        /* hifb set callback and init finish */
        hifb_open_init_finish(info);
    }
    return HI_SUCCESS;
}

/*
 * Function        : hifb_open
 * Description     : open the framebuffer and using the default parameter to set the layer
                     struct fb_info *info
 * Return          : return 0
 */
#ifdef __HuaweiLite__
void *g_hifb_info_ptr = HI_NULL;
static hi_s32 hifb_open (struct fb_vtable_s *vtable)
#else
static hi_s32 hifb_open (struct fb_info *info, hi_s32 user)
#endif
{
#ifdef __HuaweiLite__
    struct hifb_info *info = (struct hifb_info *)vtable;
#endif
    hifb_par *par = HI_NULL;

    /* check input param */
    if (hifb_open_check_param(info) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;

    /* check layer is cursor and is soft cursor */
    if (hifb_open_is_cursor_layer(info) == HI_TRUE) {
        return HI_SUCCESS;
    }

    /* open the layer first */
    if (hifb_open_start(info) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /* increase reference count */
    atomic_inc(&par->ref_count);
#ifdef __HuaweiLite__
    g_hifb_info_ptr = (void *)info;
#endif
    return HI_SUCCESS;
}

/*
 * Function        : hifb_release
 * Description     : open the framebuffer and disable the layer
                     struct fb_info *info
 * Return          : return 0 if succeed, otherwise return -EINVAL
 */
hi_void hifb_release_info(hifb_par *par)
{
    unsigned long lock_flag;
    if (par == HI_NULL) {
        return;
    }

    if ((g_drv_ops.hifb_drv_enable_wbc_int == HI_NULL) || (g_drv_ops.hifb_drv_enable_layer == HI_NULL) ||
        (g_drv_ops.hifb_drv_updata_layer_reg == HI_NULL) || (g_drv_ops.hifb_drv_set_int_callback == HI_NULL) ||
        (g_drv_ops.hifb_close_layer == HI_NULL)) {
        hifb_error("hifb_release_info failed!\n");
        return;
    }
#ifdef MDDRDETECT
    if (g_drv_ops.hifb_drv_clear_ddr_dectect_zone == HI_NULL) {
        hifb_error("hifb_release_info failed!\n");
        return;
    }
#endif
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    hifb_set_show(par, HI_FALSE);

    if ((!is_cursor_layer(par->layer_id)) || (!is_soft_cursor())) {
        /* disable the compress */
        if (par->compress_info.compress_open) {
            /* No memory alloc, no need to release */
            par->compress_info.compress_open = HI_FALSE;
        }

        /* clear wbc interrupt when close fb */
        g_drv_ops.hifb_drv_enable_wbc_int(par->layer_id, HI_FALSE);
        g_drv_ops.hifb_drv_enable_layer(par->layer_id, HI_FALSE);
        g_drv_ops.hifb_drv_updata_layer_reg(par->layer_id);

        g_drv_ops.hifb_drv_set_int_callback(HIFB_INTTYPE_VO, HI_NULL, par->layer_id, HI_NULL);
        g_drv_ops.hifb_drv_set_int_callback(HIFB_INTTYPE_WBC, HI_NULL, par->layer_id, HI_NULL);

        g_drv_ops.hifb_close_layer(par->layer_id);
        /* Clear memory detection area when closing graphics layer */
#ifdef MDDRDETECT
        g_drv_ops.hifb_drv_clear_ddr_dectect_zone(par->compress_info.start_section, par->compress_info.zone_nums);
#endif
        par->compress_info.start_section = 0;
        par->compress_info.zone_nums = 0;
        par->compress_info.new_start_section = 0;
        par->compress_info.new_zone_nums = 0;
        par->compress_info.clear_zone = HI_TRUE;

        /* MMzFree in hifb_freeccanbuf has a sleep function call, you need to release the spin lock first. */
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        hifb_freeccanbuf(par);
        hifb_spin_lock_irqsave(&par->lock, lock_flag);
    }
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
}

static hi_s32 hifb_release_layer(hifb_par *par)
{
    if (par == HI_NULL) {
        return HI_FAILURE;
    }
    /* Direct release of resources is not allowed if the graphics layer is opened more than once */
    if (atomic_read(&par->ref_count) > 1) {
        hifb_error("the layer is open, please close the layer first!\n");
        return -1;
    }

    hifb_release_info(par);

    return 0;
}

static hi_s32 hifb_release(struct fb_info *info, hi_s32 user)
{
    hifb_par *par = HI_NULL;
    hi_u32 mem_len;
    hi_s8 *screen_base = HI_NULL;

    if (info == HI_NULL) {
        return HI_FAILURE;
    }
    if (info->par == HI_NULL) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;

    if (atomic_dec_and_test(&par->ref_count)) {
        hifb_release_info(par);

        screen_base = hifb_get_screen_base(info);
        mem_len = hifb_get_smem_len(info);
        if ((screen_base != HI_NULL) && (mem_len != 0)) {
            osal_memset(screen_base, 0, mem_len);
        }
#ifdef CONFIG_HI_VO_FB_SEPARATE
        vou_graphics_deinit();
        par->layer_open = HI_FALSE;
#endif
    }

    return 0;
}

/* Soft mouse needs to filter out the hardware mouse interface */
hi_bool hifb_cmdfilter(hi_u32 cmd)
{
    hi_bool filter = HI_FALSE;
    switch (cmd) {
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
            filter = HI_TRUE; /* These interfaces need to be filtered */
            break;
        }
        default:
            break;
    }

    return filter;
}

static hi_s32 hifb_check_cusor_phyaddr(HIFB_SURFACE_S *cursor)
{
    HIFB_SURFACE_S *cursor_sur = cursor;
    hi_u64 len;
    if (cursor == HI_NULL) {
        return HI_FAILURE;
    }

    len = cursor_sur->u32Pitch * cursor_sur->u32Height;
    return cmpi_check_mmz_phy_addr(cursor_sur->u64PhyAddr, len);
}

static hi_s32 hifb_ioctl_check_param(struct fb_info *info, hi_u32 cmd, hi_void *argp)
{
    hifb_par *par  = HI_NULL;
    hi_u32 layer_id;

    if ((info == HI_NULL) || (info->par == HI_NULL)) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;
    if (layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    if ((argp == HI_NULL) && (cmd != FBIOGET_VBLANK_HIFB) &&
        (cmd != FBIO_WAITFOR_FREFRESH_DONE) && (cmd != FBIO_RELEASE_HIFB)) {
        hifb_error("HI_NULL arg!\n");
        return HI_FAILURE;
    }

    if ((!g_drv_ops.capability[layer_id].bLayerSupported) &&
        (!is_cursor_layer(layer_id) || (!is_soft_cursor()))) {
        hifb_error("not supprot layer %d!\n", layer_id);
        return HI_FAILURE;
    }

    /* Turn on the soft mouse function, and it is the mouse layer, you need to filter the hard interface. */
    if ((is_soft_cursor()) && (is_cursor_layer(layer_id))) {
        if (hifb_cmdfilter(cmd)) {
            hifb_error("soft cursor layer %d not support this function!\n", layer_id);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_get_colorkey(struct fb_info *info, unsigned long arg)
{
    HIFB_COLORKEY_S colorkey = {0};
    hifb_colorkeyex colorkey_ex = {0};
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    par = (hifb_par *)info->par;

    if ((!g_drv_ops.capability[par->layer_id].bKeyRgb) &&
        (!g_drv_ops.capability[par->layer_id].bKeyAlpha)) {
        hifb_error("Layer %d doesn't support colorkey!\n", par->layer_id);
        return HI_FAILURE;
    }

    hifb_get_key(par, &colorkey_ex);

    colorkey.bKeyEnable = colorkey_ex.key_enable;
    colorkey.u32Key = colorkey_ex.key;

    return copy_to_user(argp, &colorkey, sizeof(HIFB_COLORKEY_S));
}

static hi_s32 hifb_set_colorkey(struct fb_info *info, unsigned long arg)
{
    HIFB_COLORKEY_S colorkey;
    hifb_colorkeyex colorkey_ex;
    unsigned long lock_flag;
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;

    if (copy_from_user(&colorkey, argp, sizeof(HIFB_COLORKEY_S))) {
        return -EFAULT;
    }

    if ((colorkey.bKeyEnable != HI_TRUE) && (colorkey.bKeyEnable != HI_FALSE)) {
        hifb_error("bKeyEnable(%d) should be HI_TRUE or HI_FALSE!\n", colorkey.bKeyEnable);
        return HI_FAILURE;
    }

    if ((!g_drv_ops.capability[par->layer_id].bKeyRgb) && (!g_drv_ops.capability[par->layer_id].bKeyAlpha)) {
        hifb_error("Layer %d doesn't support colorkey!\n", par->layer_id);
        return HI_FAILURE;
    }

    if ((colorkey.bKeyEnable) && (display_info->premul)) {
        hifb_error("colorkey and premul couldn't take effect at the same time!\n");
        return HI_FAILURE;
    }

    colorkey_ex.key = colorkey.u32Key;
    colorkey_ex.key_enable = colorkey.bKeyEnable;
    /* 8 is bits */
    if (hifb_get_bits_per_pixel(info) <= 8) {
        if (colorkey.u32Key >= (2 << hifb_get_bits_per_pixel(info))) { /* 2 bit colorkey */
            hifb_error("The key :%u is out of range the palette: %u!\n",
                       colorkey.u32Key, 2 << hifb_get_bits_per_pixel(info)); /* 2 bit colorkey */
            return HI_FAILURE;
        }
            colorkey_ex.blue_max = colorkey_ex.blue_min = info->cmap.blue[colorkey.u32Key];
            colorkey_ex.green_max = colorkey_ex.green_min = info->cmap.green[colorkey.u32Key];
            colorkey_ex.red_max = colorkey_ex.red_min = info->cmap.red[colorkey.u32Key];
        } else {
#ifdef __HuaweiLite__
        g_drv_ops.hifb_drv_color_convert(info, &colorkey_ex);
#else
        g_drv_ops.hifb_drv_color_convert(&info->var, &colorkey_ex);
#endif
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;
    hifb_set_key(par, &colorkey_ex);
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

static hi_s32 hifb_get_layer_alpha(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_ALPHA_S alpha = {0};

    par = (hifb_par *)info->par;
    hifb_get_alpha(par, &alpha);
    return copy_to_user(argp, &alpha, sizeof(HIFB_ALPHA_S));
}

static hi_s32 hifb_set_layer_alpha(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    unsigned long lock_flag;
    HIFB_ALPHA_S alpha = {0};

    par = (hifb_par *)info->par;
    if (copy_from_user(&alpha, argp, sizeof(HIFB_ALPHA_S))) {
        return -EFAULT;
    }

    if ((alpha.bAlphaEnable != HI_TRUE) && (alpha.bAlphaEnable != HI_FALSE)) {
        hifb_error("alpha.bAlphaEnable should be HI_TRUE or HI_FALSE!\n");
        return HI_FAILURE;
    }

    if ((alpha.bAlphaChannel != HI_TRUE) && (alpha.bAlphaChannel != HI_FALSE)) {
        hifb_error("alpha.bAlphaChannel should be HI_TRUE or HI_FALSE!\n");
        return HI_FAILURE;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;
    hifb_set_alpha(par, &alpha);
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_ALPHA;

    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

static hi_s32 hifb_get_screen_origin_pos(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;

    HIFB_POINT_S pos = {0};

    par = (hifb_par *)info->par;
    hifb_get_layerpos(par, &pos);

    return copy_to_user(argp, &pos, sizeof(HIFB_POINT_S));
}

static hi_s32 hifb_set_screen_origin_pos(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_POINT_S expected_pos;
    hi_u32 layer_id;
    unsigned long lock_flag;
    hi_bool is_interlace = HI_FALSE;
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;
    is_interlace = hifb_is_interlace(par);

    if (is_cursor_layer(layer_id) && is_soft_cursor()) {
        hifb_error("you shouldn't set soft cursor origion by this cmd, try FBIOPUT_CURSOR_POS");
        return HI_FAILURE;
    }

    if (copy_from_user(&expected_pos, argp, sizeof(HIFB_POINT_S))) {
        return -EFAULT;
    }

    if (expected_pos.s32XPos < 0 || expected_pos.s32YPos < 0) {
        hifb_error("It's not supported to set start pos of layer to negative!\n");
        return HI_FAILURE;
    }

    if (is_interlace && ((expected_pos.s32YPos % 2) != 0)) { /* 2 Judge parity */
        hifb_error("s32YPos should be even for interlace vodev!\n");
        return HI_FAILURE;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;

    /* Record the old location first */
    hifb_set_layerpos(par, &expected_pos);
    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
}

static hi_s32 hifb_get_deflicker(struct fb_info *info, unsigned long arg)
{
    hifb_par *par = HI_NULL;
    par = (hifb_par *)info->par;
    if (!g_drv_ops.capability[par->layer_id].u32HDefLevel &&
        !g_drv_ops.capability[par->layer_id].u32VDefLevel) {
        hifb_error("deflicker is not supported!\n");
        return -EPERM;
    }

    /* There is currently no chip that needs to support this feature, so comment out first. */
    return HI_SUCCESS;
}

static hi_s32 hifb_set_deflicker(struct fb_info *info, unsigned long arg)
{
    hifb_par *par = HI_NULL;
    par = (hifb_par *)info->par;
    if (!g_drv_ops.capability[par->layer_id].u32HDefLevel &&
        !g_drv_ops.capability[par->layer_id].u32VDefLevel) {
        hifb_error("deflicker is not supported!\n");
        return -EPERM;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_get_vblank(struct fb_info *info, unsigned long arg)
{
    hifb_par *par = HI_NULL;
    par = (hifb_par *)info->par;
    if (hifb_wait_regconfig_work(par->layer_id)) {
        hifb_error("It is not support VBL!\n");
        return -EPERM;
    }

    return HI_SUCCESS;
}

static hi_s32 hifb_show_layer(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_bool is_show = HI_FALSE;
    unsigned long lock_flag;
    hi_u32 layer_id;

    par = (hifb_par *)info->par;
    layer_id = par->layer_id;
    if (is_cursor_layer(layer_id) && is_soft_cursor()) {
        hifb_error("you shouldn't show sot cursor by this cmd, try FBIOPUT_CURSOR_STATE!");
        return HI_FAILURE;
    }

    if (copy_from_user(&is_show, argp, sizeof(hi_bool))) {
        return -EFAULT;
    }

    if ((is_show != HI_TRUE) && (is_show != HI_FALSE)) {
        hifb_error("bShow(%d) should be HI_TRUE or HI_FALSE!\n", is_show);
        return HI_FAILURE;
    }

    if (is_show == hifb_get_show(par)) {
        hifb_dbg_info("The layer is show(%d) now!\n", par->show);
        return HI_SUCCESS;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;

    hifb_set_show(par, is_show);
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_SHOW;

    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
}

static hi_s32 hifb_get_layer_show_state(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_bool is_show = HI_FALSE;
    par = (hifb_par *)info->par;
    is_show = par->show;
    return copy_to_user(argp, &is_show, sizeof(hi_bool));
}

static hi_s32 hifb_get_capability(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_CAPABILITY_S capability = {0};

    par = (hifb_par *)info->par;
    capability = g_drv_ops.capability[par->layer_id];
    return copy_to_user(argp, (hi_void *)&capability, sizeof(HIFB_CAPABILITY_S));
}

static hi_s32 hifb_set_sursor_info(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_CURSOR_S cursor_info;

    par = (hifb_par *)info->par;
    osal_memset(&cursor_info, 0, sizeof(HIFB_CURSOR_S));
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }

    if (copy_from_user(&cursor_info, argp, sizeof(HIFB_CURSOR_S))) {
        return -EFAULT;
    }

    if (hifb_check_cusor_phyaddr(&cursor_info.stCursor) == HI_FAILURE) {
        hifb_error("cursor mmz phy addr 0x%llx invalid.\n", cursor_info.stCursor.u64PhyAddr);
        return HI_FAILURE;
    }

    if (hifb_cursor_putinfo(par, &cursor_info) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_get_sursor_info(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hifb_cursor_info cursor_info = {0};
    HIFB_CURSOR_S cursor = {0};

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    hifb_get_cursorinfo(par, &cursor_info);
    cursor = cursor_info.cursor;
    return copy_to_user(argp, &cursor, sizeof(HIFB_CURSOR_S));
#else

    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_cursor_state(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_bool is_show = HI_FALSE;

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&is_show, argp, sizeof(hi_bool))) {
        return -EFAULT;
    }

    if (hifb_cursor_changestate(par, is_show) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_get_cursor_state(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_bool is_show = HI_FALSE;
    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    is_show = hifb_get_show(par);

    return copy_to_user(argp, &is_show, sizeof(hi_bool));
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_cursor_pos(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_POINT_S pos = {0};

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&pos, argp, sizeof(HIFB_POINT_S))) {
        return -EFAULT;
    }

    if (hifb_cursor_changepos(par->layer_id, pos) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_get_cursor_pos(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_POINT_S pos = {0};

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }
    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    hifb_get_layerpos(par, &pos);

    return copy_to_user(argp, &pos, sizeof(HIFB_POINT_S));
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_cursor_colorkey(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_COLORKEY_S colorkey;
    hifb_colorkeyex colorkey_ex;
    HIFB_COLOR_FMT_E color_format;

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&colorkey, argp, sizeof(HIFB_COLORKEY_S))) {
        return -EFAULT;
    }

    colorkey_ex.key = colorkey.u32Key;
    colorkey_ex.key_enable = colorkey.bKeyEnable;
    hifb_get_fmt(par, &color_format);
#ifdef __HuaweiLite__
    g_drv_ops.hifb_drv_color_convert(info, &colorkey_ex);
#else
    g_drv_ops.hifb_drv_color_convert(&info->var, &colorkey_ex);
#endif

    hifb_set_key(par, &colorkey_ex);

    return HI_SUCCESS;
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_get_cursor_colorkey(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_COLORKEY_S colorkey = {0};
    hifb_colorkeyex colorkey_ex = {0};

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    hifb_get_key(par, &colorkey_ex);

    colorkey.bKeyEnable = colorkey_ex.key_enable;
    colorkey.u32Key = colorkey_ex.key;
    return copy_to_user(argp, &(colorkey), sizeof(HIFB_COLORKEY_S));
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_cursor_alpha(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_ALPHA_S alpha = {0};

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't set hard cursor alpha by this cmd!\n");
        return HI_FAILURE;
    }

    if (copy_from_user(&alpha, argp, sizeof(HIFB_ALPHA_S))) {
        return -EFAULT;
    }

    if ((alpha.bAlphaEnable != HI_TRUE) && (alpha.bAlphaEnable != HI_FALSE)) {
        hifb_error("pstAlphaInfo->bAlphaEnable(%d) should be HI_TRUE or HI_FALSE!\n", alpha.bAlphaEnable);
        return HI_FAILURE;
    }

    if ((alpha.bAlphaChannel != HI_TRUE) && (alpha.bAlphaChannel != HI_FALSE)) {
        hifb_error("pstAlphaInfo->bAlphaChannel(%d) should be HI_TRUE or HI_FALSE!\n", alpha.bAlphaChannel);
        return HI_FAILURE;
    }

    hifb_set_alpha(par, &alpha);

    return HI_SUCCESS;
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_get_cursor_alpha(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_ALPHA_S alpha = {0};

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }
    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    hifb_get_alpha(par, &alpha);

    return copy_to_user(argp, &alpha, sizeof(HIFB_ALPHA_S));
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_attch_cursor(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_u32 layer_id = 0;

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't attach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&layer_id, argp, sizeof(hi_u32))) {
        return -EFAULT;
    }
    if ((layer_id >= g_drv_ops.layer_count) ||
        (g_drv_ops.capability[layer_id].bLayerSupported == HI_FALSE)) {
        hifb_error("LayerId %d should be in [0, %u) or it is not supported!\n", layer_id,
                   g_drv_ops.layer_count);
        return -EFAULT;
    }

    if (is_cursor_layer(layer_id)) {
        hifb_error("Don't allow attach cursor to itself or other cursor layer!\n");
        return HI_FAILURE;
    }

    return hifb_cursor_attach(par->layer_id, layer_id);
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_detach_cursor(struct fb_info *info, unsigned long arg)
{
#ifdef CURSOR
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_u32 layer_id = 0;

    par = (hifb_par *)info->par;
    if (!is_soft_cursor()) {
        hifb_error("you shouldn't detach hard cursor by this cmd!\n");
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id)) {
        hifb_error("layer %d is not cursor layer!\n", par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&layer_id, argp, sizeof(hi_u32))) {
        return -EFAULT;
    }

    if ((layer_id >= g_drv_ops.layer_count) ||
        (g_drv_ops.capability[layer_id].bLayerSupported == HI_FALSE)) {
        hifb_error("LayerId %d should be in [0, %u) or it is not supported!\n", layer_id,
                   g_drv_ops.layer_count);
        return -EFAULT;
    }

    if (is_cursor_layer(layer_id)) {
        hifb_error("Detach cursor to itself or other cursor layer!\n");
        return HI_FAILURE;
    }
    return hifb_cursor_detach(layer_id);
#else
    hifb_error("the soft cursor operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_layer_info(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;

    par = (hifb_par *)info->par;
    return hifb_onputlayerinfo(info, par, argp);
}

static hi_s32 hifb_get_layer_info(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_LAYER_INFO_S layer_info = {0};
    par = (hifb_par *)info->par;
    hifb_get_layerinfo(par, &layer_info);
    return copy_to_user(argp, &layer_info, sizeof(HIFB_LAYER_INFO_S));
}

static hi_s32 hifb_get_canvas_buffer(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_BUFFER_S buf = {0};
    hifb_refresh_info *refresh_info = HI_NULL;

    par = (hifb_par *)info->par;
    refresh_info = &par->refresh_info;
    osal_memcpy(&(buf.stCanvas), &(par->canvas_sur), sizeof(HIFB_SURFACE_S));
    osal_memcpy(&(buf.UpdateRect), &(refresh_info->user_buffer.UpdateRect), sizeof(HIFB_RECT));
    if (copy_to_user(argp, &(buf), sizeof(HIFB_BUFFER_S))) {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_refresh_layer(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = (hifb_par *)info->par;
    return hifb_onrefresh(par, argp);
}

static hi_s32 hifb_wait_refresh_finish(struct fb_info *info, unsigned long arg)
{
    hifb_par *par = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode;
    par = (hifb_par *)info->par;
    hifb_get_bufmode(par, &buf_mode);
    if ((buf_mode != HIFB_LAYER_BUF_NONE) && (buf_mode != HIFB_LAYER_BUF_BUTT)) {
        /* 80 is timeout */
        if (wait_event_interruptible_timeout(par->vbl_event, par->vblflag, msecs_to_jiffies(80)) < 0) {
            return -ERESTARTSYS;
        }
    } else {
        hifb_error("doesn't support FBIO_WAITFOR_FREFRESH_DONE operation when in standard mode" \
                   "or HI_FB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_set_mirror_mode(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_MIRROR_MODE_E mirror_mode;
    par = (hifb_par *)info->par;
    if (copy_from_user(&mirror_mode, argp, sizeof(HIFB_MIRROR_MODE_E))) {
        return -EFAULT;
    }
    if (hifb_set_mirrormode(par, mirror_mode) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_get_mirror_mode(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode = 0;
    HIFB_MIRROR_MODE_E mirror_mode;

    par = (hifb_par *)info->par;
    display_info = &par->display_info;
    hifb_get_bufmode(par, &buf_mode);
    mirror_mode = display_info->mirror_mode;
    if ((buf_mode == HIFB_LAYER_BUF_BUTT) || (buf_mode == HIFB_LAYER_BUF_NONE)) {
        hifb_error("doesn't support FBIOGET_MIRROR_MODE operation when in standard mode" \
                   "or HI_FB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }
    if (copy_to_user(argp, &mirror_mode, sizeof(HIFB_MIRROR_MODE_E))) {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_set_rotate_mode(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    HIFB_ROTATE_MODE_E rotate_mode;

    if (copy_from_user(&rotate_mode, argp, sizeof(HIFB_ROTATE_MODE_E))) {
        return -EFAULT;
    /* Reset compression information */
    }
    if (hifb_set_rotatemode(info, rotate_mode) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 hifb_get_rotate_mode(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode = 0;
    HIFB_ROTATE_MODE_E rotate_mode;

    par = (hifb_par *)info->par;
    display_info = &par->display_info;
    hifb_get_bufmode(par, &buf_mode);

    if ((buf_mode == HIFB_LAYER_BUF_BUTT) || (buf_mode == HIFB_LAYER_BUF_NONE)) {
        hifb_error("doesn't support FBIOGET_ROTATE_MODE operation when in standard mode" \
                   "or HIFB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }
    rotate_mode = display_info->rotate_mode;
    if (copy_to_user(argp, &rotate_mode, sizeof(HIFB_ROTATE_MODE_E))) {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_set_screen_size(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_SIZE_S  screen_size;
    HIFB_SIZE_S  max_screen_size = {0};
    HIFB_POINT_S pos = {0};
    unsigned long lock_flag;

    par = (hifb_par *)info->par;
    if (is_cursor_layer(par->layer_id) && is_soft_cursor())  {
        hifb_error("you shouldn't set soft cursor screensize!");
        return HI_FAILURE;
    }

    if (copy_from_user(&screen_size, argp, sizeof(HIFB_SIZE_S))) {
        return -EFAULT;
    }
    if ((screen_size.u32Width == 0) || (screen_size.u32Height == 0)) {
        hifb_error("screen width(%u) height(%u) shouldn't be 0\n", screen_size.u32Width, screen_size.u32Height);
        return HI_FAILURE;
    }
    if (screen_size.u32Width % 2 || screen_size.u32Height % 2) { /* 2 for align */
        hifb_error("stScreenSize (%u, %u) should align to 2!\n", screen_size.u32Width, screen_size.u32Height);
        return HI_FAILURE;
    }
    hifb_get_maxscreensize(par, &max_screen_size.u32Width, &max_screen_size.u32Height);
    hifb_get_layerpos(par, &pos);
    if (screen_size.u32Width > max_screen_size.u32Width - pos.s32XPos) {
        hifb_warning("the sum of u32Width(%u) and s32XPos(%d) larger than Vodev screen width(%u),"\
             "width will be changed!\n", screen_size.u32Width, pos.s32XPos, max_screen_size.u32Width);
        screen_size.u32Width = max_screen_size.u32Width - pos.s32XPos;
    }
    if (screen_size.u32Height > max_screen_size.u32Height - pos.s32YPos) {
        hifb_warning("the sum of u32Height(%u) and s32YPos(%d) larger than Vodev screen height(%u),"\
            "width will be changed!\n", screen_size.u32Height, pos.s32YPos, max_screen_size.u32Height);
        screen_size.u32Height = max_screen_size.u32Height - pos.s32YPos;
    }
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;
    if (hifb_set_screensize(par, &screen_size.u32Width, &screen_size.u32Height) == HI_SUCCESS) {
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_OUTRECT;
    }
    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

static hi_s32 hifb_get_screen_size(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_SIZE_S screen_size = {0};
    par = (hifb_par *)info->par;
    hifb_get_screensize(par, &screen_size.u32Width, &screen_size.u32Height);
    return copy_to_user(argp, &screen_size, sizeof(HIFB_SIZE_S));
}

static hi_s32 flip_surface_check_param(struct fb_info *info, HIFB_SURFACEEX_S *surface_ex)
{
    hifb_par *par = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    unsigned long addr;
    unsigned long smem_end;
    par = (hifb_par *)info->par;
    display_info = &par->display_info;
    if (surface_ex->stColorkey.bKeyEnable != HI_TRUE && surface_ex->stColorkey.bKeyEnable != HI_FALSE) {
        hifb_error("colorkey.enable(%d) should be HI_TRUE or HI_FALSE!\n", surface_ex->stColorkey.bKeyEnable);
        return HI_FAILURE;
    }

    if (surface_ex->stAlpha.bAlphaEnable != HI_TRUE && surface_ex->stAlpha.bAlphaEnable != HI_FALSE) {
        hifb_error("alpha.alpha_en(%d) should be HI_TRUE or HI_FALSE!\n", surface_ex->stAlpha.bAlphaEnable);
        return HI_FAILURE;
    }

    if (surface_ex->stAlpha.bAlphaChannel != HI_TRUE && surface_ex->stAlpha.bAlphaChannel != HI_FALSE) {
        hifb_error("alpha.alpha_chn_en (%d) should be HI_TRUE or HI_FALSE!\n",
                   surface_ex->stAlpha.bAlphaChannel);
        return HI_FAILURE;
    }

    if ((surface_ex->stColorkey.bKeyEnable) && (!g_drv_ops.capability[par->layer_id].bKeyRgb) &&
        (!g_drv_ops.capability[par->layer_id].bKeyAlpha)) {
        hifb_error("Layer %d doesn't support colorkey!\n", par->layer_id);
        return HI_FAILURE;
    }

    if (surface_ex->stColorkey.bKeyEnable && display_info->premul) {
        hifb_error("colorkey and premul couldn't take effect at the same time!\n");
        return HI_FAILURE;
    }

    addr = (unsigned long)surface_ex->u64PhyAddr;
    smem_end = hifb_get_smem_start(info) + hifb_get_smem_len(info) - hifb_get_yres(info) *
        hifb_get_line_length(info);
    if ((addr < hifb_get_smem_start(info)) || (addr > smem_end)) {
        hifb_error("the addr is out of range!\n");
        return HI_FAILURE;
    }

    if (hifb_get_line_length(info) == 0) {
        return HI_FAILURE;
    }

    if (hifb_get_bits_per_pixel(info) == 0) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __HuaweiLite__
static hi_s32 flip_surface_pan_display(struct fb_info *info, HIFB_SURFACEEX_S *surface_ex,
                                       hifb_colorkeyex *colorkey_ex)
{
    unsigned long addr;
    unsigned int differ;
    unsigned int x_offset;
    unsigned int y_offset;
    hi_u32 x_value, y_value;
    struct fb_overlayinfo_s oinfo;
    addr = (unsigned long)surface_ex->u64PhyAddr;
    differ = addr - hifb_get_smem_start(info);

    x_value = hifb_get_bits_per_pixel(info);
    y_value = hifb_get_line_length(info);
    if ((x_value == 0) || (y_value == 0)) {
        return HI_FAILURE;
    }
    /* 3 is 8 bit */
    x_offset = ((differ % hifb_get_line_length(info)) << 3) / x_value;
    y_offset = differ / y_value;

    osal_memcpy(&oinfo, &info->oinfo, sizeof(oinfo));
    oinfo.sarea.x = x_offset;
    oinfo.sarea.y = y_offset;

    if (hifb_pan_display(info, &oinfo) < 0) {
        hifb_error("error!\n");
        return HI_FAILURE;
    }

    info->oinfo.sarea.x = x_offset;
    info->oinfo.sarea.y = y_offset;
    colorkey_ex->key = surface_ex->stColorkey.u32Key;
    colorkey_ex->key_enable = surface_ex->stColorkey.bKeyEnable;
    if (hifb_get_bits_per_pixel(info) <= 8) { /* 8 bit */
        if (surface_ex->stColorkey.u32Key >= (2 << hifb_get_bits_per_pixel(info))) { /* 2 bit Colorkey */
            hifb_error("The key :%u is out of range the palette: %u!\n",
                       surface_ex->stColorkey.u32Key, 2 << hifb_get_bits_per_pixel(info)); /* 2 bit Colorkey */
            return HI_FAILURE;
        }
        colorkey_ex->blue_max = colorkey_ex->blue_min = info->cmap.blue[surface_ex->stColorkey.u32Key];
        colorkey_ex->green_max = colorkey_ex->green_min = info->cmap.green[surface_ex->stColorkey.u32Key];
        colorkey_ex->red_max = colorkey_ex->red_min = info->cmap.red[surface_ex->stColorkey.u32Key];
    } else {
        g_drv_ops.hifb_drv_color_convert(info, colorkey_ex);
    }
    return HI_SUCCESS;
}
#else
static hi_s32 flip_surface_pan_display(struct fb_info *info, HIFB_SURFACEEX_S *surface_ex,
                                       hifb_colorkeyex *colorkey_ex)
{
    unsigned long addr;
    unsigned int differ;
    unsigned int x_offset;
    unsigned int y_offset;
    hi_u32 x_value, y_value;
    struct fb_var_screeninfo var;
    addr = (unsigned long)surface_ex->u64PhyAddr;
    differ = addr - hifb_get_smem_start(info);

    x_value = hifb_get_bits_per_pixel(info);
    y_value = hifb_get_line_length(info);
    if ((x_value == 0) || (y_value == 0)) {
        return HI_FAILURE;
    }
    /* 8 bit (2^3) */
    x_offset = ((differ % hifb_get_line_length(info)) << 3) / x_value;
    y_offset = differ / y_value;

    osal_memcpy(&var, &info->var, sizeof(var));
    var.xoffset = x_offset;
    var.yoffset = y_offset;

    if (fb_pan_display(info, &var) < 0) {
        hifb_error("pan_display error!\n");
        return HI_FAILURE;
    }

    colorkey_ex->key = surface_ex->stColorkey.u32Key;
    colorkey_ex->key_enable = surface_ex->stColorkey.bKeyEnable;

    if (hifb_get_bits_per_pixel(info) <= 8) { /* 8 bit */
        if (surface_ex->stColorkey.u32Key >= (2 << hifb_get_bits_per_pixel(info))) { /* 2 bit Colorkey */
            hifb_error("The key :%u is out of range the palette: %u!\n",
                       surface_ex->stColorkey.u32Key, 2 << hifb_get_bits_per_pixel(info)); /* 2 bit Colorkey */
            return HI_FAILURE;
        }

        colorkey_ex->blue_max = colorkey_ex->blue_min = info->cmap.blue[surface_ex->stColorkey.u32Key];
        colorkey_ex->green_max = colorkey_ex->green_min = info->cmap.green[surface_ex->stColorkey.u32Key];
        colorkey_ex->red_max = colorkey_ex->red_min = info->cmap.red[surface_ex->stColorkey.u32Key];
    } else {
        g_drv_ops.hifb_drv_color_convert(&info->var, colorkey_ex);
    }

    return HI_SUCCESS;
}
#endif

static hi_s32 hifb_flip_surface(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_SURFACEEX_S surface_ex;
    unsigned long lock_flag;
    hifb_colorkeyex colorkey_ex = {0};

    par = (hifb_par *)info->par;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        hifb_error("you shouldn't use FBIOFLIP_SURFACE for soft cursor!");
        return HI_FAILURE;
    }

    if (copy_from_user(&surface_ex, argp, sizeof(HIFB_SURFACEEX_S))) {
        return -EFAULT;
    }
    if (flip_surface_check_param(info, &surface_ex) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    if (flip_surface_pan_display(info, &surface_ex, &colorkey_ex) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;
    hifb_set_alpha(par, &surface_ex.stAlpha);
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_ALPHA;
    if (g_drv_ops.capability[par->layer_id].bKeyRgb ||
        g_drv_ops.capability[par->layer_id].bKeyAlpha) {
        hifb_set_key(par, &colorkey_ex);
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
    }
    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return 0;
}

static hi_s32 set_compression_process(struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode;
    unsigned long lock_flag;

    par = (hifb_par *)info->par;
    compress_info = &par->compress_info;
    display_info = &par->display_info;

    if (par->color_format != HIFB_FMT_ARGB8888 && par->color_format != HIFB_FMT_ARGB1555 &&
        par->color_format != HIFB_FMT_ARGB4444) {
        hifb_error("compression only support pixel format (ARGB8888,ARGB1555,ARGB4444)\n");
        return HI_FAILURE;
    }
    if ((display_info->mirror_mode != HIFB_MIRROR_NONE) ||
        (display_info->rotate_mode != HIFB_ROTATE_NONE)) {
        hifb_error("Can't do compression when mirror or rotate!\n");
        return HI_FAILURE;
    }
    hifb_get_bufmode(par, &buf_mode);

    /* FB uses frame decompression, can not be displayed while refreshing, so only supports double buf mode */
    if ((buf_mode != HIFB_LAYER_BUF_DOUBLE) && (buf_mode != HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        hifb_error("only HI_FB_LAYER_BUF_DOUBLE or HI_FB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
        return HI_FAILURE;
    }

    /* Record the entire image as the area to be compressed */
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    compress_info->compress_rect.x = 0;
    compress_info->compress_rect.y = 0;
    compress_info->compress_rect.w = display_info->display_width;
    compress_info->compress_rect.h = display_info->display_height;
    compress_info->update_finished = HI_TRUE;
    compress_info->delay = HI_TRUE;
    compress_info->compress_open = HI_TRUE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
}

static hi_s32 set_compression_start(struct fb_info *info, hi_bool is_compress)
{
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    unsigned long lock_flag;
    par = (hifb_par *)info->par;
    compress_info = &par->compress_info;
    if (is_compress != HI_TRUE && is_compress != HI_FALSE) {
        hifb_error("compress(%d) should be HI_TRUE or HI_FALSE!\n", is_compress);
        return HI_FAILURE;
    }
    if (is_compress != compress_info->compress_open) {
        if (is_compress) {
            if (set_compression_process(info) != HI_SUCCESS) {
                return HI_FAILURE;
            }
        } else {
            hifb_spin_lock_irqsave(&par->lock, lock_flag);
            compress_info->compress_open = HI_FALSE;
            hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
            /* Waiting for an interrupt before proceeding to the next two operations */
            hifb_wait_regconfig_work(par->layer_id);
            hifb_free_compress_buffer(par);
            /* Reset compression information */
            compress_info->update_rect.w = 0;
            compress_info->update_rect.h = 0;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_set_compression_mode(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    hi_bool is_compress = HI_FALSE;
    par = (hifb_par *)info->par;
    if (!g_drv_ops.capability[par->layer_id].bDcmp) {
        hifb_error("Layer %d doesn't support compression operation!\n", par->layer_id);
        return HI_FAILURE;
    }

    if (copy_from_user(&is_compress, argp, sizeof(hi_bool))) {
        return -EFAULT;
    }

    /* check detect zone */
    if (set_compression_start(info, is_compress) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_get_compression_mode(struct fb_info *info, unsigned long arg)
            {
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    hi_bool is_compress = HI_FALSE;
    par = (hifb_par *)info->par;
    compress_info = &par->compress_info;
    if (!g_drv_ops.capability[par->layer_id].bDcmp) {
        hifb_warning("Layer %d doesn't support get compression!\n", par->layer_id);
    } else {
        is_compress = compress_info->compress_open;
    }
    return copy_to_user(argp, &is_compress, sizeof(hi_bool));
}

static hi_s32 hifb_set_mddrdetect(struct fb_info *info, unsigned long arg)
{
#ifdef MDDRDETECT
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    HIFB_DDRZONE_S ddr_zone_para = {0};
    unsigned long lock_flag;
    par = (hifb_par *)info->par;
    compress_info = &par->compress_info;
    if (g_drv_ops.hifb_drv_is_layer_support_ddr_detect(par->layer_id) == HI_FALSE) {
        hifb_error("Layer %d doesn't support DDR detect PUT.\n", par->layer_id);
        return HI_FAILURE;
    }
    if (!g_drv_ops.capability[par->layer_id].bDcmp) {
        hifb_error("Layer %d doesn't support DDR detect!\n", par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&ddr_zone_para, argp, sizeof(HIFB_DDRZONE_S))) {
        return -EFAULT;
    }
    if (g_drv_ops.hifb_drv_check_ddr_dectect_zone(par->layer_id, ddr_zone_para.start_section,
                                                  ddr_zone_para.zone_nums, compress_info->start_section,
                                                  compress_info->zone_nums) == HI_FAILURE) {
        hifb_error("Layer %d DDR detect zone ERR!\n", par->layer_id);

        return HI_FAILURE;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);

    /* If you repeat the settings, return directly */
    if (ddr_zone_para.start_section == compress_info->start_section &&
        ddr_zone_para.zone_nums == compress_info->zone_nums) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

        return HI_SUCCESS;
    }

    compress_info->new_start_section = ddr_zone_para.start_section;
    compress_info->new_zone_nums = ddr_zone_para.zone_nums;
    if (ddr_zone_para.zone_nums == 0) {
        compress_info->clear_zone = HI_TRUE;
    } else {
        compress_info->clear_zone = HI_FALSE;
    }
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
#else
    hifb_error("the set mddrdetect operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_get_mddrdetect(struct fb_info *info, unsigned long arg)
{
#ifdef MDDRDETECT
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    HIFB_DDRZONE_S ddr_zone_para = {0};
    unsigned long lock_flag;
    par = (hifb_par *)info->par;
    compress_info = &par->compress_info;
    if (g_drv_ops.hifb_drv_is_layer_support_ddr_detect(par->layer_id) == HI_FALSE) {
        hifb_error("Layer %d doesn't support DDR detect GET.\n", par->layer_id);
        return HI_FAILURE;
    }
    if (!g_drv_ops.capability[par->layer_id].bDcmp) {
        hifb_error("Layer %d doesn't support DDR detect!\n", par->layer_id);
        return HI_FAILURE;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    ddr_zone_para.start_section = compress_info->new_start_section;
    ddr_zone_para.zone_nums = compress_info->new_zone_nums;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return copy_to_user(argp, &ddr_zone_para, sizeof(HIFB_DDRZONE_S));
#else
    hifb_error("the get mddrdetect operation is unsupported!\n");
    return HI_FAILURE;
#endif
}

static hi_s32 hifb_set_dynamic_range(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_DYNAMIC_RANGE_E dynamic_range = HIFB_DYNAMIC_RANGE_BUTT;
    par = (hifb_par *)info->par;
    if (g_drv_ops.capability[par->layer_id].bGHDR == HI_FALSE) {
        hifb_error("Layer %d# doesn't support HDR function.\n",par->layer_id);
        return HI_FAILURE;
    }
    if (copy_from_user(&dynamic_range, argp, sizeof(HIFB_DYNAMIC_RANGE_E))) {
        return -EFAULT;
    }
    if (hifb_set_dynamic_range_display(par, dynamic_range) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_get_dynamic_range(struct fb_info *info, unsigned long arg)
{
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
    hifb_par *par = HI_NULL;
    HIFB_DYNAMIC_RANGE_E dynamic_range;
    hifb_display_info *display_info = HI_NULL;
    par = (hifb_par *)info->par;
    display_info = &par->display_info;
    if (g_drv_ops.capability[par->layer_id].bGHDR == HI_FALSE) {
        hifb_error("Layer %d# doesn't support HDR function.\n",par->layer_id);
        return HI_FAILURE;
    }
    dynamic_range = display_info->dynamic_range;
    if (copy_to_user(argp, &dynamic_range, sizeof(HIFB_DYNAMIC_RANGE_E))) {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_info_release(struct fb_info *info, unsigned long arg)
{
    hifb_par *par = HI_NULL;
    par = (hifb_par *)info->par;
    return hifb_release_layer(par);
}

#ifdef __HuaweiLite__
static hi_void hifb_info_to_temp(struct fb_info *info, struct hifb_info *info_temp)
{
    info->vinfo.xres = info_temp->vinfo.xres;
    info->vinfo.yres = info_temp->vinfo.yres;
    info->vinfo.fmt = info_temp->vinfo.fmt;
    info->oinfo.sarea.w = info_temp->oinfo.sarea.w;
    info->oinfo.sarea.h = info_temp->oinfo.sarea.h;
    info->oinfo.sarea.x = info_temp->oinfo.sarea.x;
    info->oinfo.sarea.y = info_temp->oinfo.sarea.y;
    info->oinfo.bpp = info_temp->oinfo.bpp;
    return;
}

static hi_s32 hifb_ioctl_liteos_standard_io(struct fb_info *info, hi_u32 cmd, unsigned long arg,
                                            hi_bool *is_continue)
{
    struct hifb_info *info_temp = HI_NULL;
    struct hifb_info info_copy;
    struct fb_overlayinfo_s *oinfo = (struct fb_overlayinfo_s *)arg;
    unsigned int xres = info->vinfo.yres;
    unsigned int yres = info->vinfo.yres;
    unsigned int xoffset = oinfo->sarea.x;
    unsigned int yoffset = oinfo->sarea.y;
    hi_s32 ret = HI_SUCCESS;

    switch (cmd) {
        case FBIOGET_SCREENINFO_HIFB:
            osal_memcpy((void *)arg, info, sizeof(struct hifb_info));
            break;
        case FBIOPUT_SCREENINFO_HIFB:
            osal_memcpy(&info_copy, info, sizeof(info_copy));
            info_temp = (struct hifb_info *)arg;
            if (info->vtable.fb_set_par) {
                hifb_info_to_temp(info, info_temp);
                ret = info->vtable.fb_set_par(&info->vtable);
                if (ret != HI_SUCCESS) {
                    hifb_error("Put screeninfo error! ret=%d\n", ret);
                    osal_memcpy(info, &info_copy, sizeof(info_copy));
                    return HI_FAILURE;
                }
                hifb_pan_display(&info->vtable, &info->oinfo);
            }
            break;
        case FBIOPAN_DISPLAY_HIFB:
            if (info->vtable.fb_pan_display) {
                ret = info->vtable.fb_pan_display(&info->vtable, oinfo);
                if (ret != HI_SUCCESS) {
                    hifb_error("Put screeninfo error! ret=%d\n", ret);
                    osal_memcpy(info, &info_copy, sizeof(info_copy));
                    return HI_FAILURE;
                }
                info->oinfo.sarea.x = xoffset;
                info->oinfo.sarea.y = yoffset;
            }
            break;
        default:
            *is_continue = HI_TRUE;
            break;
    }
    return HI_SUCCESS;
}
#endif

/*
 * Function        : hifb_ioctl
 * Description     : set the colorkey or alpha for overlay
 * Return          : return 0 if succeed, otherwise return error code
 */
static hi_s32 hifb_ioctl(struct fb_info *info, hi_u32 cmd, unsigned long arg)
{
    hi_u8 hifb_cmd = _IOC_NR(cmd);
    hi_void __user *argp = (hi_void __user *)(hi_uintptr_t)arg;
#ifdef __HuaweiLite__
    hi_bool is_continue = HI_FALSE;
#endif
    if (hifb_ioctl_check_param(info, cmd, argp) != HI_SUCCESS) {
        return HI_FAILURE;
    }
#ifdef __HuaweiLite__
    if (hifb_ioctl_liteos_standard_io(info, cmd, arg, &is_continue) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    if (is_continue != HI_TRUE) {
        return HI_SUCCESS;
    }
#endif
    if ((hifb_cmd < 1) || (hifb_cmd >= DRV_HIFB_IOCTL_CMD_NUM_MAX) || (g_drv_hifb_ctl_num[hifb_cmd] < 1) ||
        (g_drv_hifb_ctl_num[hifb_cmd] >= DRV_HIFB_IOCTL_FUNC_ITEM_NUM_MAX)) {
        return HI_FAILURE;
    }
    if (g_drv_hifb_ioctl_func[g_drv_hifb_ctl_num[hifb_cmd]].func == HI_NULL) {
        return HI_FAILURE;
    }
    if (cmd != g_drv_hifb_ioctl_func[g_drv_hifb_ctl_num[hifb_cmd]].cmd) {
        hifb_error("the command:0x%x is unsupported!\n", cmd);
        return HI_FAILURE;
    }
    return g_drv_hifb_ioctl_func[g_drv_hifb_ctl_num[hifb_cmd]].func(info, arg);
}

#ifdef CONFIG_COMPAT
static hi_s32 hifb_compat_ioctl(struct fb_info *info, unsigned cmd, unsigned long arg)
{
    return hifb_ioctl(info, cmd, arg);
}
#endif

static hi_void hifb_version(hi_void)
{
    /* 80 size Use "strings hifb.ko | grep "HIFB_MAIN_VERSION"" to get the version */
    hi_char hifb_version[80] =
        "HIFB_MAIN_VERSION[" mkmarcotostr(HIFB_MAIN_VERSION) "] Build Time[" __DATE__ ", "__TIME__ "]";
    hifb_dbg_info("%s\n", hifb_version);
}

static hi_s32 hifb_bitfieldcmp(struct fb_bitfield x, struct fb_bitfield y)
{
    if ((x.offset == y.offset) && (x.length == y.length) && (x.msb_right == y.msb_right)) {
        return 0;
    } else {
        return -1;
    }
}

static hi_u32 hifb_getbppbyfmt(HIFB_COLOR_FMT_E color_fmt)
{
    switch (color_fmt) {
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
            return 16; /* 16 is bpp fmt */
        case HIFB_FMT_RGB888:
        case HIFB_FMT_ARGB8565:
        case HIFB_FMT_RGBA5658:
        case HIFB_FMT_ABGR8565:
        case HIFB_FMT_BGR888:
            return 24; /* 24 is bpp fmt */
        case HIFB_FMT_KRGB888:
        case HIFB_FMT_ARGB8888:
        case HIFB_FMT_RGBA8888:
        case HIFB_FMT_ABGR8888:
        case HIFB_FMT_KBGR888:
            return 32; /* 32 is bpp fmt */
        case HIFB_FMT_1BPP:
            return 1;
        case HIFB_FMT_2BPP:
            return 2; /* 2 is bpp fmt */
        case HIFB_FMT_4BPP:
            return 4; /* 4 is bpp fmt */
        case HIFB_FMT_8BPP:
        case HIFB_FMT_ACLUT44:
            return 8; /* 8 is bpp fmt */
        default:
            return 0;
    }
}

static HIFB_COLOR_FMT_E hifb_getfmtbyargb(struct fb_bitfield *red, struct fb_bitfield *green, struct fb_bitfield *blue,
                                          struct fb_bitfield *transp, hi_u32 color_depth)
{
    hi_u32 i;
    hi_u32 bpp;

    if ((red == HI_NULL) || (green == HI_NULL) || (blue == HI_NULL) || (transp == HI_NULL)) {
        return HIFB_FMT_BUTT;
    }

    /* not support color palette low than 8 bit */
    if (color_depth < 8) {
        return HIFB_FMT_BUTT;
    }
    /* not support color palette low than 8 bit */
    if (color_depth == 8) {
        return HIFB_FMT_8BPP;
    }

    /*
     * Find the pixel format (HIFB_ARGB_BITINFO_S) corresponding to the given red,
     * green, and blue bit field information and the number of bits per pixel (bpp)
     */
    for (i = 0; i < sizeof(g_argb_bit_field) / sizeof(hifb_argb_bitinfo); i++) {
        if ((hifb_bitfieldcmp(*red, g_argb_bit_field[i].red) == 0) &&
            (hifb_bitfieldcmp(*green, g_argb_bit_field[i].green) == 0) &&
            (hifb_bitfieldcmp(*blue, g_argb_bit_field[i].blue) == 0) &&
            (hifb_bitfieldcmp(*transp, g_argb_bit_field[i].transp) == 0)) {
            bpp = hifb_getbppbyfmt(i);
            if (bpp == color_depth) {
                return i;
            }
        }
    }
    i = HIFB_FMT_BUTT;
    return i;
}

hi_s32 hifb_check_mem_enough(struct fb_info *info, hi_u32 pitch, hi_u32 height)
{
    hi_u32 buffer_num = 0;
    hi_u32 buffer_size;
    hifb_par *par = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    if (info == HI_NULL) {
        return HI_FAILURE;
    }
    if (info->par == HI_NULL) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;
    refresh_info = &par->refresh_info;

    switch (refresh_info->buf_mode) {
        case HIFB_LAYER_BUF_DOUBLE:
        case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
            buffer_num = 2; /* 2 buffer num */
            break;
        case HIFB_LAYER_BUF_ONE:
            buffer_num = 1;
            break;
        default:
            return HI_SUCCESS;
    }
    /* The interface setting requires uBuffersize, the actual memory size info->fix.smem_len */
    buffer_size = buffer_num * pitch * height;
    if (hifb_get_smem_len(info) >= buffer_size) {
        return HI_SUCCESS;
    }
    hifb_error("memory is not enough!  now is %u u32Pitch %u u32Height %u expect %u\n", hifb_get_smem_len(info),
               pitch, height, buffer_size);
    return HI_FAILURE;
}

/* Address check only for uncompressed data */
static hi_s32 hifb_check_phyaddr(HIFB_BUFFER_S *canvas_buf)
{
    HIFB_BUFFER_S *can_buf = canvas_buf;
    hi_u64 len;
    if (canvas_buf == HI_NULL) {
        return HI_FAILURE;
    }

    len = (hi_u64)can_buf->stCanvas.u32Pitch * can_buf->stCanvas.u32Height;

    return cmpi_check_mmz_phy_addr(can_buf->stCanvas.u64PhyAddr, len);
}

#ifndef __HuaweiLite__
static hi_s32 hifb_check_fmt(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_COLOR_FMT_E fmt;
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    if ((info == HI_NULL) || (var == HI_NULL)) {
        return HI_FAILURE;
    }
    if (info->par == HI_NULL) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    fmt = hifb_getfmtbyargb(&var->red, &var->green, &var->blue, &var->transp, var->bits_per_pixel);
    if (fmt == HIFB_FMT_BUTT) {
        hifb_error("Unknown fmt(offset, length) r:(%d, %d, %d) , g:(%d, %d, %d), b(%d, %d, %d), \
            a(%d, %d, %d), bpp:%d!\n",
                   var->red.offset, var->red.length, var->red.msb_right, var->green.offset, var->green.length,
                   var->green.msb_right, var->blue.offset, var->blue.length, var->blue.msb_right, var->transp.offset,
                   var->transp.length, var->transp.msb_right, var->bits_per_pixel);
        return -EINVAL;
    }

    if (!g_drv_ops.capability[layer_id].bColFmt[fmt]) {
        hifb_error("Unsupported PIXEL FORMAT!\n");
        return -EINVAL;
    }

    return HI_SUCCESS;
}
#endif

/*
 * Name : hifb_buf_map
 * Desc : Memory mapping, which generates virtual addresses based on physical address mappings.
 */
hi_void *hifb_buf_map(hi_u64 phy_addr, hi_u32 size)
{
    return cmpi_remap_nocache(phy_addr, size);
}

hi_void hifb_buf_ummap(hi_void *viraddr)
{
    cmpi_unmap(viraddr);
    return;
}

#ifndef __HuaweiLite__
static hi_s32 hifb_check_output(struct fb_var_screeninfo *var, struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;
    /* 2 is be divided with no remainder */
    if (((var->yres % 2) != 0) && (hifb_is_interlace(par))) {
        hifb_error("yres(%d) of layer_id %d should be even when vodev output is interlace\n", var->yres,
                   layer_id);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_check_virtual_resolution(struct fb_var_screeninfo *var, struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if (var->xres < hifb_min_width(layer_id)) {
        hifb_error("xres(%d) of layer_id %d can't be less than min_width(%d)\n", var->xres,
                   layer_id, hifb_min_width(layer_id));
        return HI_FAILURE;
    }
    if (var->yres < hifb_min_height(layer_id)) {
        hifb_error("yres(%d) of layer_id %d can't be less than min_height(%d)\n", var->yres,
                   layer_id, hifb_min_height(layer_id));
        return HI_FAILURE;
    }

    if (var->xres > var->xres_virtual) {
        hifb_error("xres(%d) of layer_id %d should be less than xres_virtual(%d)\n", var->xres,
                   layer_id, var->xres_virtual);
        return HI_FAILURE;
    }
    if (var->yres > var->yres_virtual) {
        hifb_error("yres(%d) of layer_id %d should be less than yres_virtual(%d)\n", var->yres,
                   layer_id, var->yres_virtual);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_check_offset(struct fb_var_screeninfo *var, struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if ((var->xoffset + var->xres > var->xres_virtual) ||
        (var->xoffset > var->xres_virtual)) {
        hifb_error("the sum of layer%d's xoffset(%d) and xres(%d) should be less than xres_virtual(%d)\n",
            layer_id, var->xoffset, var->xres, var->xres_virtual);
        return -EINVAL;
    }

    if ((var->yoffset + var->yres > var->yres_virtual) ||
        (var->yoffset > var->yres_virtual)) {
        hifb_error("the sum of layer%d's yoffset(%d) and yres(%d) should be less than yres_virtual(%d)\n",
            layer_id, var->yoffset, var->yres, var->yres_virtual);
        return -EINVAL;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_check_total(struct fb_var_screeninfo *var, struct fb_info *info)
{
    hi_u32 hor_total;
    hi_u32 ver_total;
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    hor_total = var->left_margin + var->xres + var->right_margin + var->hsync_len;
    if (hor_total == 0) {
        hifb_error("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d),hsync_len(%d) can't be 0\n",
            layer_id, var->left_margin, var->xres, var->right_margin, var->hsync_len);
        return HI_FAILURE;
    }
    ver_total = var->yres + var->lower_margin + var->vsync_len + var->upper_margin;
    if (ver_total == 0) {
        hifb_error("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d),hsync_len(%d) can't be 0\n",
            layer_id, var->upper_margin, var->yres, var->lower_margin, var->vsync_len);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*
 * Function        : hifb_check_var
 * Description     : check if the paramter for framebuffer is supported.
 * Return          : return 0, if the paramter is supported, otherwise,return error
 */
static hi_s32 hifb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    hifb_par *par = HI_NULL;
    hi_u32 expected_len;
    hi_u32 layer_id;

    if ((info == HI_NULL) || (var == HI_NULL) || (info->par == HI_NULL)) {
        return HI_FAILURE;
    }

    par = (hifb_par *)info->par;
    layer_id = par->layer_id;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        hifb_error("cursor layer doesn't support this operation!\n");
        return HI_FAILURE;
    }

    if (hifb_check_fmt(var, info) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /*
     * For interlaced output check
     * the actual height of the layer must be an even number
     * Progressive output without this limit
     */
    if (hifb_check_output(var, info) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    /*
     * for virtual resolution check
     * virtual resolution can't be less than minimal resolution
     */
    if (hifb_check_virtual_resolution(var, info) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    /* check if the offset is valid */
    if (hifb_check_offset(var, info) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    /*
     * for hor_total and ver_total check
     * The FB driver in the Linux kernel will use u32HTotal and u32VTotal as divisors
     * so they cannot be 0
     */
    if (hifb_check_total(var, info) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    hifb_dbg_info("xres:%d, yres:%d, xres_virtual:%d, yres_virtual:%d\n", var->xres, var->yres,
                  var->xres_virtual, var->yres_virtual);
    /* for mem len check */
    expected_len = var->yres_virtual * ((((var->xres_virtual * var->bits_per_pixel) >> 3) + /* 8 bit (2^3) */
        HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT));

    if (info->fix.smem_len && (expected_len > info->fix.smem_len)) {
        hifb_error("layer %d don't has enough mem! expected: %d KBytes, real:%d KBytes\n", layer_id,
                   expected_len / 1024, info->fix.smem_len / 1024); /* 1024 (2^10) */
        return -EINVAL;
    }

    return HI_SUCCESS;
}
#endif

static hi_void hifb_set_dispbufinfo(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)(info->par);
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_dispbuf_info *disp_buf_info = &refresh_info->disp_buf_info;

    /*
     * there's a limit from hardware that screen buf shoule be 16 bytes aligned,maybe it's proper
     * to get this info from drv adapter
     */
    hi_u32 buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    if (hifb_get_smem_len(info) == 0) {
        return;
    } else if ((hifb_get_smem_len(info) >= buf_size) && (hifb_get_smem_len(info) < buf_size * 2)) { /* 2 size expand */
        disp_buf_info->phy_addr[0] = hifb_get_smem_start(info);
        disp_buf_info->phy_addr[1] = hifb_get_smem_start(info);
    } else if (hifb_get_smem_len(info) >= buf_size * 2) { /* 2 size expand */
        disp_buf_info->phy_addr[0] = hifb_get_smem_start(info);
        disp_buf_info->phy_addr[1] = hifb_get_smem_start(info) + buf_size;
    }
    return;
}

/*
 * Name : hifb_get_dcmp_framesize
 * Desc : Read two numbers from the virtual address.
 */
static hi_void hifb_get_dcmp_framesize(HIFB_BUFFER_S *dst, hi_u32 *ar_size, hi_u32 *gb_size,
                                       const hi_u8 *screen_base, hi_u32 offset)
{
    *ar_size = *(hi_u64 *)screen_base;
    /* Screen_base cannot be converted to any type except HI_U8* */
    *gb_size = *(hi_u64 *)(screen_base + offset);
}

static hi_s32 hifb_refresh_1buf_prepare_dst(HIFB_BUFFER_S *dst_rect, hifb_par *par,
    hifb_display_info *display_info, hifb_dispbuf_info *display_buf_info, struct fb_info *info)
{
    hi_u32 bytes_per_pixel = 2;
    hi_u32 buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);

    dst_rect->stCanvas.enFmt = par->color_format;
    dst_rect->stCanvas.u32Height = display_info->display_height;
    dst_rect->stCanvas.u32Width = display_info->display_width;
    dst_rect->stCanvas.u32Pitch = hifb_get_line_length(info);

    if (display_info->rotate_mode  == HIFB_ROTATE_90 || display_info->rotate_mode == HIFB_ROTATE_270) {
        dst_rect->stCanvas.u32Height = display_info->display_width;
        dst_rect->stCanvas.u32Width = display_info->display_height;
        if (dst_rect->stCanvas.enFmt == HIFB_FMT_ARGB1555 ||
            dst_rect->stCanvas.enFmt == HIFB_FMT_ARGB4444) {
            bytes_per_pixel = 2; /* 2 for bit */
        } else if (dst_rect->stCanvas.enFmt == HIFB_FMT_ARGB8888) {
            bytes_per_pixel = 4; /* 4 for bit */
        }
        dst_rect->stCanvas.u32Pitch = ((bytes_per_pixel * dst_rect->stCanvas.u32Width + HIFB_ALIGN - 1) /
                                 HIFB_ALIGN) * HIFB_ALIGN;

        if ((par->color_format != HIFB_FMT_ARGB4444) && (par->color_format != HIFB_FMT_ARGB1555) &&
            (par->color_format != HIFB_FMT_ARGB8888)) {
            hifb_error("The rotate mode only support HI_FB_FORMAT_ARGB4444,HI_FB_FORMAT_ARGB1555,"\
                       "HI_FB_FORMAT_ARGB8888 which is %d\n!\n", par->color_format);
            return HI_FAILURE;
        }

        dst_rect->stCanvas.u64PhyAddr = par->rotate_vb;
    } else {
        /*
         * The target address is selected as the display buf configured for the interrupt,
         * which is indicated by index for interrupt.
         */
        dst_rect->stCanvas.u64PhyAddr = display_buf_info->phy_addr[display_buf_info->index_for_int];
        /* If compression is not open, you do not have to configure a compressed address for this refresh */
        if (par->compress_info.compress_open) {
            /* Use half of the video memory instead of another buffer ,2 is size */
            dst_rect->stCanvas.u64GBPhyAddr = dst_rect->stCanvas.u64PhyAddr + buf_size / 2;
        }
    }
    return HI_SUCCESS;
}

static hi_void hifb_refresh_1buf_prepare_addr(hifb_refresh_info *refresh_info, hi_u32 layer_id,
    hifb_osd_data *osd_data, struct fb_info *info, HIFB_BUFFER_S *dst_rect)
{
    unsigned long lock_flag;
    hifb_par *par = (hifb_par *)info->par;
    hifb_dispbuf_info *display_buf_info = &refresh_info->disp_buf_info;
    hi_u32 buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) &
                       (~HIFB_ALIGNMENT);
    /* 1buf does not support compression, close compression configuration */
    refresh_info->disp_buf_info.compress = HI_FALSE;

    g_drv_ops.hifb_drv_get_osd_data(layer_id, osd_data);

    if (osd_data->buffer_phy_addr != par->refresh_info.disp_buf_info.phy_addr[0] &&
        display_buf_info->phy_addr[0]) {
        hifb_spin_lock_irqsave(&par->lock, lock_flag);
        par->modifying = HI_TRUE;
        /* Notify the interrupt handler to modify the display address. */
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;

        /*
         * The buf address in the display information is configured to
         * the screen display address for refreshing the screen.
         */
        refresh_info->screen_addr = display_buf_info->phy_addr[display_buf_info->index_for_int];
        /* If compression is not open, you do not have to configure a compressed address for this refresh */
        if (par->compress_info.compress_open) {
            /* Use half of the video memory instead of another buffer */
            refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 part */
        }
        display_buf_info->stride = 4 * dst_rect->stCanvas.u32Width; /* 4 for argb 8888 */

        par->modifying = HI_FALSE;
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    }
}

static hi_void hifb_refresh_1buf_prepare_opt(hifb_blit_opt *blit_opt, hifb_par *par)
{
    blit_opt->call_back = HI_TRUE;
    /* Non-blocking mode */
    blit_opt->block = HI_FALSE;

    if (par->display_info.antiflicker_mode == HIFB_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = HIFB_LAYER_ANTIFLICKER_NONE;
    }

    blit_opt->region_deflicker = HI_TRUE;

    blit_opt->param = &(par->layer_id);

    if (par->display_info.rotate_mode == HIFB_ROTATE_180) {
        blit_opt->mirror_mode = HIFB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = par->display_info.mirror_mode;
    }
}

static hi_void hifb_refresh_1buf_prepare_global_refresh(HIFB_BUFFER_S *canvas_buf, HIFB_BUFFER_S *dst_rect,
                                                        hifb_blit_opt *blit_opt)
{
    if (canvas_buf->stCanvas.u32Height != dst_rect->stCanvas.u32Height ||
        canvas_buf->stCanvas.u32Width != dst_rect->stCanvas.u32Width) {
        /* Rotate 0 or 180 degrees, zoomed, then global refresh */
        blit_opt->scale = HI_TRUE;

        dst_rect->UpdateRect.x = 0;
        dst_rect->UpdateRect.y = 0;
        dst_rect->UpdateRect.w = dst_rect->stCanvas.u32Width;
        dst_rect->UpdateRect.h = dst_rect->stCanvas.u32Height;
    } else {
        /* Rotate 0 or 180 degrees, no zoomed, then partial refresh */
        dst_rect->UpdateRect = canvas_buf->UpdateRect;
    }
}

static hi_void hifb_refresh_1buf_prepare_compress(volatile hifb_compress_info *compress_info,
                                                  HIFB_BUFFER_S *dst_rect, hifb_par *par, hifb_blit_opt *blit_opt)
{
    unsigned long lock_flag;
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if (compress_info->compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE to
         * indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        osal_memcpy((void *)&compress_info->update_rect, &dst_rect->UpdateRect, sizeof(HIFB_RECT));
        compress_info->update_finished = HI_FALSE;
        blit_opt->compress = HI_TRUE;
    } else {
        blit_opt->compress = HI_FALSE;
    }

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
}

static hi_s32 hifb_refresh_1buf_blit(HIFB_BUFFER_S *canvas_buf, HIFB_BUFFER_S *dst_rect, hifb_blit_opt *blit_opt,
                                     hifb_dispbuf_info *display_buf_info)
{
    hi_tde_export_func *tde_export_func = HI_NULL;

    tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
    if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_begin_job == HI_NULL) ||
        (tde_export_func->drv_tde_module_rotate == HI_NULL) ||
        (tde_export_func->drv_tde_module_end_job == HI_NULL)) {
        hifb_error("can't get TDE export function, it may be TDE module has not been inserted!\n");
        return HI_FAILURE;
    }
    /*
     * The user buf is used as the source by blit, and the user buf is moved to the display buf with
     * the target set (with the target showing the buff address) as the target.
     */
    if (hifb_drv_blit(canvas_buf, dst_rect, blit_opt, HI_TRUE, &display_buf_info->refresh_handle) < 0) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static hi_s32 hifb_refresh_1buf(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_dispbuf_info *display_buf_info = &refresh_info->disp_buf_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;
    hifb_osd_data osd_data;
    hi_s32 ret;
    hifb_blit_opt blit_opt = {0};
    HIFB_BUFFER_S dst_rect;

    if (canvas_buf == HI_NULL) {
        return HI_FAILURE;
    }

    ret = hifb_refresh_1buf_prepare_dst(&dst_rect, par, display_info, display_buf_info, info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    hifb_refresh_1buf_prepare_addr(refresh_info, layer_id, &osd_data, info, &dst_rect);

    hifb_refresh_1buf_prepare_opt(&blit_opt, par);

    hifb_refresh_1buf_prepare_global_refresh(canvas_buf, &dst_rect, &blit_opt);

    hifb_refresh_1buf_prepare_compress(compress_info, &dst_rect, par, &blit_opt);

    ret = hifb_refresh_1buf_blit(canvas_buf, &dst_rect, &blit_opt, display_buf_info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    osal_memcpy(&(refresh_info->user_buffer), canvas_buf, sizeof(HIFB_BUFFER_S));

    return HI_SUCCESS;
}

/* unit rect */
static void hifb_unite_rect(HIFB_RECT *dst_rect, HIFB_RECT *src_rect)
{
    HIFB_RECT rect;
    rect.x = (dst_rect->x < src_rect->x) ? dst_rect->x : src_rect->x;
    rect.y = (dst_rect->y < src_rect->y) ? dst_rect->y : src_rect->y;
    rect.w = ((dst_rect->x + dst_rect->w) > (src_rect->x + src_rect->w)) ?
        (dst_rect->x + dst_rect->w - rect.x) : (src_rect->x + src_rect->w - rect.x);
    rect.h = ((dst_rect->y + dst_rect->h) > (src_rect->y + src_rect->h)) ?
        (dst_rect->y + dst_rect->h - rect.y) : (src_rect->y + src_rect->h - rect.y);
    osal_memcpy(dst_rect, &rect, sizeof(HIFB_RECT));
    return;
}

/* check these two rectangle cover each other */
static hi_bool hifb_iscontain(HIFB_RECT parent_rect, HIFB_RECT child_rect)
{
    HIFB_POINT_S point;
    point.s32XPos = child_rect.x;
    point.s32YPos = child_rect.y;
    if ((point.s32XPos < parent_rect.x) || (point.s32XPos > (parent_rect.x + parent_rect.w)) ||
        (point.s32YPos < parent_rect.y) || (point.s32YPos > (parent_rect.y + parent_rect.h))) {
        return HI_FALSE;
    }
    point.s32XPos = child_rect.x + child_rect.w;
    point.s32YPos = child_rect.y + child_rect.h;
    if ((point.s32XPos < parent_rect.x) || (point.s32XPos > (parent_rect.x + parent_rect.w)) ||
        (point.s32YPos < parent_rect.y) || (point.s32YPos > (parent_rect.y + parent_rect.h))) {
        return HI_FALSE;
    }
    return HI_TRUE;
}

static hi_s32 refresh_2buf_prepare_back_buf(struct fb_info *info, HIFB_BUFFER_S *back_buf, hi_u32 *bytes_per_pixel)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_display_info *display_info = &par->display_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;
    hi_tde_export_func *tde_export_func = HI_NULL;
    hi_u32 buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    if (refresh_info->disp_buf_info.need_flip == HI_TRUE) {
        hifb_error("Layer(%d) refresh again before display another buf, maybe refresh too fast !\n", par->layer_id);
    }

    refresh_info->disp_buf_info.need_flip = HI_FALSE;
    refresh_info->disp_buf_info.refresh_handle = 0;

    back_buf->stCanvas.enFmt = par->color_format;
    back_buf->stCanvas.u32Height = display_info->display_height;
    back_buf->stCanvas.u32Width = display_info->display_width;
    back_buf->stCanvas.u32Pitch = hifb_get_line_length(info);

    if (display_info->rotate_mode  == HIFB_ROTATE_90 || display_info->rotate_mode == HIFB_ROTATE_270) {
        back_buf->stCanvas.u32Width = display_info->display_height;
        back_buf->stCanvas.u32Height = display_info->display_width;
        if (back_buf->stCanvas.enFmt == HIFB_FMT_ARGB1555 || back_buf->stCanvas.enFmt == HIFB_FMT_ARGB4444) {
            *bytes_per_pixel = 2; /* 2 bit per pixel */
        } else if(back_buf->stCanvas.enFmt == HIFB_FMT_ARGB8888) {
            *bytes_per_pixel = 4; /* 4 bit per pixel */
        }
        back_buf->stCanvas.u32Pitch = ((*bytes_per_pixel * back_buf->stCanvas.u32Width + HIFB_ALIGN - 1) / HIFB_ALIGN) *
                                       HIFB_ALIGN;

        if ((par->color_format != HIFB_FMT_ARGB4444) && (par->color_format != HIFB_FMT_ARGB1555) &&
            (par->color_format != HIFB_FMT_ARGB8888)) {
            hifb_error("The rotate mode only support HIFB_FMT_ARGB4444,HIFB_FMT_ARGB1555," \
                       "HIFB_FMT_ARGB8888 which is %d\n!\n", par->color_format);
            return HI_FAILURE;
        }

        tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
        if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_begin_job == HI_NULL) ||
            (tde_export_func->drv_tde_module_rotate == HI_NULL) ||
            (tde_export_func->drv_tde_module_end_job == HI_NULL)) {
            hifb_error("can't get TDE export function, it may be TDE module has not been inserted!\n");
            return HI_FAILURE;
        }

        back_buf->stCanvas.u64PhyAddr = par->rotate_vb;
    } else {
        /* Set the background buf as the target, get the free buf to the background buf */
        hifb_get_idledispbuf(par, (hi_u64*)(&back_buf->stCanvas.u64PhyAddr));
        if (compress_info->compress_open) {
            /*
             * Use half of the video memory instead of another buffer. Currently only 59A on the himpp
             * platform needs to be configured with this address. Other chips do not need to be configured.
             */
            back_buf->stCanvas.u64GBPhyAddr = back_buf->stCanvas.u64PhyAddr + buf_size / 2; /* 2 alg data */
        }
    }
    return HI_SUCCESS;
}

static hi_void refresh_2buf_get_new_rect(HIFB_BUFFER_S *canvas_buf, HIFB_BUFFER_S *back_buf,
                                         HIFB_RECT *new_union_rect, hifb_blit_opt *blit_opt)
{
    hi_tde_rect src_rect = {0};
    hi_tde_rect dst_rect = {0};
    hi_tde_rect inner_src_rect = {0};
    hi_tde_rect inner_dst_rect = {0};

    if (canvas_buf->stCanvas.u32Height != back_buf->stCanvas.u32Height ||
        canvas_buf->stCanvas.u32Width != back_buf->stCanvas.u32Width) {
        src_rect.width = canvas_buf->stCanvas.u32Width;
        src_rect.height = canvas_buf->stCanvas.u32Height;
        dst_rect.width = back_buf->stCanvas.u32Width;
        dst_rect.height = back_buf->stCanvas.u32Height;
        inner_src_rect.pos_x = canvas_buf->UpdateRect.x;
        inner_src_rect.pos_y = canvas_buf->UpdateRect.y;
        inner_src_rect.width = (hi_u32)canvas_buf->UpdateRect.w;
        inner_src_rect.height = (hi_u32)canvas_buf->UpdateRect.h;
        tde_cal_scale_rect_hifb(&src_rect, &dst_rect, &inner_src_rect, &inner_dst_rect);

        new_union_rect->x = inner_dst_rect.pos_x;
        new_union_rect->y = inner_dst_rect.pos_y;
        new_union_rect->w = (hi_s32)inner_dst_rect.width;
        new_union_rect->h = (hi_s32)inner_dst_rect.height;
        blit_opt->scale = HI_TRUE;
    } else {
        *new_union_rect = canvas_buf->UpdateRect;
    }
    return;
}

static hi_void refresh_2buf_blit_init_buf(hifb_display_info *display_info, HIFB_BUFFER_S *fore_buf,
                                          HIFB_BUFFER_S *back_buf, hi_u32 bytes_per_pixel)
{
    fore_buf->stCanvas.u32Width = display_info->display_width;
    fore_buf->stCanvas.u32Height = display_info->display_height;
    fore_buf->stCanvas.u32Pitch = ((bytes_per_pixel * fore_buf->stCanvas.u32Width + (HIFB_ALIGN - 1)) / HIFB_ALIGN) *
                                   HIFB_ALIGN;
    fore_buf->UpdateRect.x = 0;
    fore_buf->UpdateRect.y = 0;
    fore_buf->UpdateRect.w = fore_buf->stCanvas.u32Width;
    fore_buf->UpdateRect.h = fore_buf->stCanvas.u32Height;

    back_buf->UpdateRect.x = 0;
    back_buf->UpdateRect.y = 0;
    back_buf->UpdateRect.w = back_buf->stCanvas.u32Width;
    back_buf->UpdateRect.h = back_buf->stCanvas.u32Height;
    return;
}

static hi_s32 refresh_2buf_blit(hifb_par *par, hi_u64 osd_buf_addr, HIFB_BUFFER_S *back_buf,
                                HIFB_RECT *new_union_rect, hi_u32 bytes_per_pixel)
{
    hifb_blit_opt tmp = {0};
    hifb_rotate_opt rot_tmp = {0};
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_display_info *display_info = &par->display_info;
    HIFB_BUFFER_S fore_buf = {0};
    hi_u64 work_buf_addr = 0;

    hifb_get_workdispbuf(par, &work_buf_addr);

    if ((refresh_info->disp_buf_info.fliped == HI_FALSE) ||
        ((osd_buf_addr != work_buf_addr) && (par->compress_info.compress_open  == HI_FALSE))) {
        return HI_SUCCESS;
    }

    /* Background as a target pointing to an idle buf */
    osal_memcpy(&fore_buf, back_buf, sizeof(HIFB_BUFFER_S));

    /* Foreground as a source points to the buf at work */
    hifb_get_workdispbuf(par, (hi_u64*)(&fore_buf.stCanvas.u64PhyAddr));

    /* The union rect is also used as an update area for the foreground and background. */
    osal_memcpy(&fore_buf.UpdateRect, &refresh_info->disp_buf_info.union_rect, sizeof(HIFB_RECT));
    osal_memcpy(&back_buf->UpdateRect, &fore_buf.UpdateRect, sizeof(HIFB_RECT));

    osal_memset(&tmp, 0x0, sizeof(hifb_blit_opt));
    /* blit with union rect */
    if ((display_info->rotate_mode != HIFB_ROTATE_90) &&
        (display_info->rotate_mode != HIFB_ROTATE_270)) {
        if (hifb_iscontain(*new_union_rect, refresh_info->disp_buf_info.union_rect) == HI_FALSE) {
            if (hifb_drv_blit(&fore_buf, back_buf, &tmp, HI_TRUE, HI_NULL) < 0) {
                hifb_error("blit err!\n");
                return HI_FAILURE;
            }
        }
    } else {
        if (display_info->rotate_mode == HIFB_ROTATE_90) {
            rot_tmp.rotate_mode = HIFB_ROTATE_270;
        } else {
            rot_tmp.rotate_mode = HIFB_ROTATE_90;
        }
        /* init buf */
        refresh_2buf_blit_init_buf(display_info, &fore_buf, back_buf, bytes_per_pixel);
        if (hifb_drv_rotate(&fore_buf, back_buf, &rot_tmp, HI_TRUE) < 0) {
            hifb_error("blit err!\n");
            return HI_FAILURE;
        }
    }

    /* clear union rect */
    osal_memset(&(refresh_info->disp_buf_info.union_rect), 0, sizeof(HIFB_RECT));

    refresh_info->disp_buf_info.fliped = HI_FALSE;
    return HI_SUCCESS;
}

static hi_void refresh_2buf_prepare_opt(hifb_par *par, HIFB_BUFFER_S *canvas_buf, HIFB_BUFFER_S *back_buf,
                                        hi_u32 bytes_per_pixel, hifb_blit_opt *blit_opt)
{
    hifb_display_info *display_info = &par->display_info;
    blit_opt->call_back = HI_TRUE;
    blit_opt->param = &(par->layer_id);

    if (display_info->antiflicker_mode == HIFB_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = HIFB_LAYER_ANTIFLICKER_NONE;
    }

    if (blit_opt->scale == HI_TRUE) {
        /* actual area, calculate by TDE, here is just use for let pass the test */
        back_buf->UpdateRect.x = 0;
        back_buf->UpdateRect.y = 0;
        back_buf->UpdateRect.w = back_buf->stCanvas.u32Width;
        back_buf->UpdateRect.h = back_buf->stCanvas.u32Height;
    } else {
        back_buf->UpdateRect = canvas_buf->UpdateRect;
    }

    if (par->display_info.rotate_mode == HIFB_ROTATE_90 ||
        par->display_info.rotate_mode == HIFB_ROTATE_270) {
        back_buf->stCanvas.u32Height = par->display_info.display_width;
        back_buf->stCanvas.u32Width = par->display_info.display_height;
        back_buf->stCanvas.u32Pitch = ((bytes_per_pixel * back_buf->stCanvas.u32Width +
                                       (HIFB_ALIGN - 1)) / HIFB_ALIGN) * HIFB_ALIGN;
        back_buf->UpdateRect = canvas_buf->UpdateRect;
    }

    blit_opt->region_deflicker = HI_TRUE;

    if (display_info->rotate_mode == HIFB_ROTATE_180) {
        blit_opt->mirror_mode = HIFB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = display_info->mirror_mode;
    }
    return;
}

static hi_void refresh_2buf_prepare_compress(hifb_par *par, HIFB_BUFFER_S *back_buf, hifb_blit_opt *blit_opt)
{
    volatile hifb_compress_info *compress_info = &par->compress_info;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if (par->compress_info.compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE
         * to indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        osal_memcpy((void *)&par->compress_info.update_rect, &back_buf->UpdateRect, sizeof(HIFB_RECT));
        compress_info->update_finished = HI_FALSE;
        blit_opt->compress = HI_TRUE;
    } else {
        blit_opt->compress = HI_FALSE;
    }

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return;
}

static inline hi_void refresh_2buf_update_rect(hifb_refresh_info *refresh_info, HIFB_RECT *new_union_rect)
{
    if ((refresh_info->disp_buf_info.union_rect.w == 0) ||
        (refresh_info->disp_buf_info.union_rect.h == 0)) {
        osal_memcpy(&refresh_info->disp_buf_info.union_rect, new_union_rect, sizeof(HIFB_RECT));
    } else {
        hifb_unite_rect(&refresh_info->disp_buf_info.union_rect, new_union_rect);
    }
    return;
}

/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static hi_s32 hifb_refresh_2buf(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_blit_opt blit_opt = {0};
    HIFB_BUFFER_S back_buf = {0};
    HIFB_RECT new_union_rect = {0};
    unsigned long lock_flag;
    hi_u64 osd_buf_addr;
    hi_u32 bytes_per_pixel = 2;
    hi_s32 ret;

    if (canvas_buf == HI_NULL) {
        return HI_FAILURE;
    }
    osal_memset(&blit_opt, 0, sizeof(hifb_blit_opt));
    /*
     * Refresh task submitted between VO vertical timing interrupt and frame start interrupt
     * Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
     * Blocked here, it is forbidden to submit the refresh task during this time.
     */
    ret = wait_event_interruptible_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job,
                                           msecs_to_jiffies(40)); /* 40 for timeout */
    if (ret < 0) {
        hifb_error("wait interrupt!\n");
    } else if (ret == 0) {
        hifb_error("wait timeout!\n");
    }

    refresh_info->refresh_num++;

    /* get osd buffer addr */
    g_drv_ops.hifb_drv_get_layer_addr(layer_id, &osd_buf_addr);

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    /* prepare back buf and get bytes_per_pixel */
    if (refresh_2buf_prepare_back_buf(info, &back_buf, &bytes_per_pixel) != HI_SUCCESS) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_FAILURE;
    }
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* according to the hw arithemetic, calculate source and dst fresh rectangle */
    refresh_2buf_get_new_rect(canvas_buf, &back_buf, &new_union_rect, &blit_opt);
    /*
     * We should check is address changed,
     * for make sure that the address configed to the hw reigster is in effec
     */
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    /* refresh_2buf_blit */
    if (refresh_2buf_blit(par, osd_buf_addr, &back_buf, &new_union_rect, bytes_per_pixel) != HI_SUCCESS) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_FAILURE;
    }
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* update union rect */
    refresh_2buf_update_rect(refresh_info, &new_union_rect);

    /* prepare opt */
    refresh_2buf_prepare_opt(par, canvas_buf, &back_buf, bytes_per_pixel, &blit_opt);

    /* prepare compress */
    refresh_2buf_prepare_compress(par, &back_buf, &blit_opt);

    /* blit with refresh rect */
    ret = hifb_drv_blit(canvas_buf, &back_buf, &blit_opt, HI_TRUE, &refresh_info->disp_buf_info.refresh_handle);
    if (ret < 0) {
        hifb_error("blit err:0x%x!\n", ret);
        return HI_FAILURE;
    }

    osal_memcpy(&(refresh_info->user_buffer), canvas_buf, sizeof(HIFB_BUFFER_S));

    return HI_SUCCESS;
}

static hi_s32 hifb_wait_regconfig_work(hi_u32 layer_id)
{
    hi_s32 ret;
    hifb_par *par = HI_NULL;

    if (layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    par = (hifb_par *)g_layer[layer_id].info->par;

    par->vblflag = 0;
    /* Assuming TDE is fast enough, 40ms */
    ret = wait_event_interruptible_timeout(par->vbl_event, par->vblflag, msecs_to_jiffies(40));
    if (ret < 0) {
        hifb_error("Wait vblank failed!");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 refresh_2buf_immediate_prepare_back_buf(struct fb_info *info, HIFB_BUFFER_S *back_buf,
                                                      hi_u32 *bytes_per_pixel, hi_u32 index)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;
    hi_u32 buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    hi_tde_export_func *tde_export_func = HI_NULL;
    if (refresh_info->disp_buf_info.need_flip == HI_TRUE) {
        hifb_error("Layer(%d) refresh again before display another buf, maybe refresh too fast !\n", par->layer_id);
    }

    /* forbid changing display buffer in interrupt handle */
    refresh_info->disp_buf_info.fliped = HI_FALSE;
    refresh_info->disp_buf_info.need_flip = HI_FALSE;
    refresh_info->disp_buf_info.refresh_handle = 0;

    back_buf->stCanvas.enFmt = par->color_format;
    back_buf->stCanvas.u32Height = display_info->display_height;
    back_buf->stCanvas.u32Width = display_info->display_width;
    back_buf->stCanvas.u32Pitch = hifb_get_line_length(info);

    if (display_info->rotate_mode == HIFB_ROTATE_90 || display_info->rotate_mode == HIFB_ROTATE_270) {
        back_buf->stCanvas.u32Width = display_info->display_height;
        back_buf->stCanvas.u32Height = display_info->display_width;
        if (back_buf->stCanvas.enFmt == HIFB_FMT_ARGB1555 || back_buf->stCanvas.enFmt == HIFB_FMT_ARGB4444) {
            *bytes_per_pixel = 2; /* 2 depth per pixel */
        } else if(back_buf->stCanvas.enFmt == HIFB_FMT_ARGB8888) {
            *bytes_per_pixel = 4; /* 4 depth per pixel */
        }
        back_buf->stCanvas.u32Pitch = (((*bytes_per_pixel)*back_buf->stCanvas.u32Width + HIFB_ALIGN - 1) / HIFB_ALIGN) *
                                       HIFB_ALIGN;
        if ((par->color_format != HIFB_FMT_ARGB4444) && (par->color_format != HIFB_FMT_ARGB1555) &&
            (par->color_format != HIFB_FMT_ARGB8888)) {
            hifb_error("The rotate mode only support HI_FB_FORMAT_ARGB4444 and HI_FB_FORMAT_ARGB1555" \
                       "which is %d\n!\n", par->color_format);
            return HI_FAILURE;
        }

        tde_export_func = func_entry(hi_tde_export_func, HI_ID_TDE);
        if ((tde_export_func == HI_NULL) || (tde_export_func->drv_tde_module_begin_job == HI_NULL) ||
            (tde_export_func->drv_tde_module_rotate == HI_NULL) ||
            (tde_export_func->drv_tde_module_end_job == HI_NULL)) {
            return HI_FAILURE;
        }

        back_buf->stCanvas.u64PhyAddr = par->rotate_vb;
    } else {
        back_buf->stCanvas.u64PhyAddr = refresh_info->disp_buf_info.phy_addr[1 - index];
        if (compress_info->compress_open) {
            back_buf->stCanvas.u64GBPhyAddr = back_buf->stCanvas.u64PhyAddr + buf_size / 2; /* 2 buf size narrow */
        }
    }
    return HI_SUCCESS;
}

static hi_s32 refresh_2buf_imediate_blit(hifb_par *par, HIFB_BUFFER_S *back_buf, HIFB_RECT *new_union_rect,
                                         hi_u32 bytes_per_pixel, hi_u32 index)
{
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    HIFB_BUFFER_S fore_buf = {0};
    hifb_rotate_opt rot_tmp = {0};
    hifb_blit_opt *tmp_opt = HI_NULL;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if (display_info->rotate_mode  != HIFB_ROTATE_90 && display_info->rotate_mode != HIFB_ROTATE_270) {
        /*
         * because reverse, the 2 buffer needed to sync contain,
         * if the fresh area has cover last fresh area, then no need to sync
         */
        if (!hifb_iscontain(*new_union_rect, refresh_info->disp_buf_info.union_rect) &&
            refresh_info->disp_buf_info.union_rect.w && refresh_info->disp_buf_info.union_rect.h) {
            osal_memcpy(&fore_buf, back_buf, sizeof(HIFB_BUFFER_S));
            fore_buf.stCanvas.u64PhyAddr = refresh_info->disp_buf_info.phy_addr[index];
            osal_memcpy(&fore_buf.UpdateRect, &refresh_info->disp_buf_info.union_rect, sizeof(HIFB_RECT));
            osal_memcpy(&back_buf->UpdateRect, &fore_buf.UpdateRect, sizeof(HIFB_RECT));
            hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
            tmp_opt = osal_vmalloc(sizeof(hifb_blit_opt));
            if (tmp_opt == HI_NULL) {
                hifb_error("NO mem!\n");
                return HI_FAILURE;
            }
            osal_memset(tmp_opt, 0x0, sizeof(hifb_blit_opt));
            if (hifb_drv_blit(&fore_buf, back_buf, tmp_opt, HI_TRUE, HI_NULL) < 0) {
                osal_vfree(tmp_opt);
                tmp_opt = HI_NULL;
                return HI_FAILURE;
            }
            osal_vfree(tmp_opt);
            tmp_opt = HI_NULL;
            hifb_spin_lock_irqsave(&par->lock, lock_flag);
        }
    } else {
        osal_memcpy(&fore_buf, back_buf, sizeof(HIFB_BUFFER_S));
        fore_buf.stCanvas.u64PhyAddr = refresh_info->disp_buf_info.phy_addr[index];
        osal_memcpy(&fore_buf.UpdateRect, &refresh_info->disp_buf_info.union_rect, sizeof(HIFB_RECT));
        osal_memcpy(&back_buf->UpdateRect, &fore_buf.UpdateRect, sizeof(HIFB_RECT));

        rot_tmp.rotate_mode = (display_info->rotate_mode == HIFB_ROTATE_90) ? (HIFB_ROTATE_270) : (HIFB_ROTATE_90);
        refresh_2buf_blit_init_buf(display_info, &fore_buf, back_buf, bytes_per_pixel);
        if (hifb_drv_rotate(&fore_buf, back_buf, &rot_tmp, HI_TRUE) < 0) {
            hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
            return HI_FAILURE;
        }
    }
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

static hi_void refresh_2buf_imediate_prepare_opt(hifb_par *par, HIFB_BUFFER_S *canvas_buf, HIFB_BUFFER_S *back_buf,
                                                 hi_u32 bytes_per_pixel, hifb_blit_opt *blit_opt)
{
    hifb_display_info *display_info = &par->display_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;

    blit_opt->call_back = HI_TRUE;
    blit_opt->param = &(par->layer_id);
    blit_opt->block = HI_TRUE;

    if (display_info->antiflicker_mode == HIFB_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = HIFB_LAYER_ANTIFLICKER_NONE;
    }

    if (blit_opt->scale == HI_TRUE) {
        /* actual area, calculate by TDE, here is just use for let pass the test */
        back_buf->UpdateRect.x = 0;
        back_buf->UpdateRect.y = 0;
        back_buf->UpdateRect.w = back_buf->stCanvas.u32Width;
        back_buf->UpdateRect.h = back_buf->stCanvas.u32Height;
    } else {
        back_buf->UpdateRect = canvas_buf->UpdateRect;
    }

    blit_opt->region_deflicker = HI_TRUE;

    if (display_info->rotate_mode == HIFB_ROTATE_180) {
        blit_opt->mirror_mode = HIFB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = display_info->mirror_mode;
    }

    if (par->compress_info.compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE to
         * indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        osal_memcpy((void *)&par->compress_info.update_rect, &back_buf->UpdateRect, sizeof(HIFB_RECT));
        compress_info->update_finished = HI_FALSE;
        blit_opt->compress = HI_TRUE;
    }
    return;
}

static hi_void refresh_2buf_imediate_backup_buffer(struct fb_info *info, HIFB_BUFFER_S *back_buf,
                                                   HIFB_RECT *new_union_rect,
                                                   hifb_blit_opt *blit_opt, hi_u32 *index)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;
    hi_u32 buf_size;
    hi_u32 ar_size;
    hi_u32 gb_size;
    unsigned long lock_flag;

    buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    *index = 1 - *index;
    refresh_info->disp_buf_info.index_for_int = *index;
    par->modifying = HI_TRUE;
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    refresh_info->screen_addr = refresh_info->disp_buf_info.phy_addr[*index];
    refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 for half */
    refresh_info->disp_buf_info.compress = compress_info->compress_open;
    par->modifying = HI_FALSE;

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = HI_TRUE;
        osal_memcpy((HIFB_RECT*)(&par->compress_info.update_rect), new_union_rect, sizeof(HIFB_RECT));
        /*
         * When blocking, if there is a callback,
         * still get FrameSize0 and FrameSize1 here as early as possible.
         */
        if ((blit_opt->block == HI_TRUE && blit_opt->call_back == HI_FALSE) ||
            (blit_opt->block == HI_TRUE && blit_opt->call_back == HI_TRUE)) {
            hifb_get_dcmp_framesize(back_buf, &ar_size, &gb_size,
                ((hi_u8 *)(hifb_get_screen_base(info)) +
                (*index) * buf_size), buf_size / 2); /* 2 for half */

            compress_info->frame_size0 =  ar_size;
            compress_info->frame_size1 =  gb_size;
        } else if((blit_opt->block == HI_FALSE && blit_opt->call_back == HI_TRUE)) {
            /* get u32AR_size and u32GB_size in callback function */
        } else {
            hifb_get_dcmp_framesize(back_buf, &ar_size, &gb_size,
                ((hi_u8 *)(hifb_get_screen_base(info)) +
                (*index) * buf_size), buf_size / 2); /* 2 for half */
            compress_info->frame_size0 = ar_size;
            compress_info->frame_size1 = gb_size;
            hifb_error("error:tde blit(do compress) block=%d,bCallBack=%d,FrameSize0 = %d,FrameSize1 = %d\n",
                blit_opt->block, blit_opt->call_back, compress_info->frame_size0,
                compress_info->frame_size1);
        }
    }

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return;
}

/*
 * In this function we should wait the new contain has been show on the screen before return,
 * and the operations such as address configuration no needed do in interrupt handle
 */
static hi_s32 hifb_refresh_2buf_immediate_display(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_blit_opt blit_opt = {0};
    hi_u32 index = refresh_info->disp_buf_info.index_for_int;
    HIFB_BUFFER_S back_buf = {0};
    hi_s32 ret;
    HIFB_RECT new_union_rect = {0};
    unsigned long lock_flag;
    hi_u32 bytes_per_pixel = 2; /* 2 byte */

    if (canvas_buf == HI_NULL) {
        return HI_FAILURE;
    }
    /*
     * Refresh task submitted between VO vertical timing interrupt and frame start interrupt
     * Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
     * Blocked here, it is forbidden to submit the refresh task during this time.
     */
    ret = wait_event_interruptible_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job,
                                           msecs_to_jiffies(40)); /* 40 for timeout */
    if (ret < 0) {
        osal_printk("Func:%s, Line:%d, wait.\n", __FUNCTION__, __LINE__);
    }
    /*
     * TDE use the blocking mode
     * Immediate refresh mode requires blocking mode, non-callback mode,
     * avoiding callback mode when scrolling subtitles.
     * Return immediately. Maybe the next time the user quickly call the interface refresh,
     * it may flush the last content.
     */
    refresh_info->refresh_num++;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    /* prepare for back buf and get bytes per pixel */
    if (refresh_2buf_immediate_prepare_back_buf(info, &back_buf, &bytes_per_pixel, index) != HI_SUCCESS) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_FAILURE;
    }

    /* according to the hw arithemetic, calculate  source and Dst fresh rectangle */
    refresh_2buf_get_new_rect(canvas_buf, &back_buf, &new_union_rect, &blit_opt);
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    /* blit */
    if (refresh_2buf_imediate_blit(par, &back_buf, &new_union_rect, bytes_per_pixel, index) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    /* record the fresh area */
    osal_memcpy(&refresh_info->disp_buf_info.union_rect, &new_union_rect, sizeof(HIFB_RECT));

    /* prepare for opt */
    refresh_2buf_imediate_prepare_opt(par, canvas_buf, &back_buf, bytes_per_pixel, &blit_opt);
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    /* blit with refresh rect */
    ret = hifb_drv_blit(canvas_buf, &back_buf, &blit_opt, HI_TRUE, &refresh_info->disp_buf_info.refresh_handle);
    if (ret < 0) {
        return HI_FAILURE;
    }

    /* set the backup buffer to register and show it */
    refresh_2buf_imediate_backup_buffer(info, &back_buf, &new_union_rect, &blit_opt, &index);

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    osal_memcpy(&(refresh_info->user_buffer), canvas_buf, sizeof(HIFB_BUFFER_S));
    par->vblflag = 0;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    /* wait the address register's configuration take effect before return */
    if (in_atomic() == HI_FALSE) {
        hifb_wait_regconfig_work(layer_id);
    }

    return HI_SUCCESS;
}

static hi_void hifb_refresh_again(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    HIFB_BUFFER_S canvas;

    /* Prerequisites for the canvas to be refreshed */
    if (!(par->param_modify_mask & HIFB_LAYER_PARAMODIFY_INRECT)) {
        return;
    }

    if (refresh_info->user_buffer.stCanvas.u64PhyAddr == 0) {
        return;
    }

    if (refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) {
        return;
    }
    /* Fills the canvas object with refresh information from private data for refresh. */
    canvas = refresh_info->user_buffer;
    canvas.UpdateRect.x = 0;
    canvas.UpdateRect.y = 0;
    canvas.UpdateRect.w = canvas.stCanvas.u32Width;
    canvas.UpdateRect.h = canvas.stCanvas.u32Height;
    hifb_refresh(layer_id, &canvas, refresh_info->buf_mode);
}

static hi_s32 hifb_disp_check_param(struct fb_info *info, hi_u32 width, hi_u32 height)
{
    hifb_par *par = (hifb_par *)info->par;
    HIFB_SIZE_S max_screen_size = {0};
    hi_u32 pitch;

    if (!g_drv_ops.capability[par->layer_id].bVoScale) {
        hifb_get_maxscreensize(par, &max_screen_size.u32Width, &max_screen_size.u32Height);
        if ((width > max_screen_size.u32Width) || (height > max_screen_size.u32Height) ||
            (width > g_drv_ops.capability[par->layer_id].u32MaxWidth) ||
            (height > g_drv_ops.capability[par->layer_id].u32MaxHeight)) {
            hifb_error("(%u, %u)larger than the max size of the screen(%u, %u) or the layer(%u, %u)!\n",
                       width, height, max_screen_size.u32Width, max_screen_size.u32Height,
                       g_drv_ops.capability[par->layer_id].u32MaxWidth,
                       g_drv_ops.capability[par->layer_id].u32MaxHeight);
            return HI_FAILURE;
        }
    }
    /* 3 is 8 bits */
    pitch = (((width * hifb_get_bits_per_pixel(info)) >> 3) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    if (hifb_check_mem_enough(info, pitch, height) == HI_FAILURE) {
        hifb_error("++ setdispsize .memory is not enough!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*
 * This function has a lock inside, and there is a call to the sleep function.
 * Before calling this function, you must first unlock the spin lock.
 */
#ifdef __HuaweiLite__
static hi_s32 hifb_disp_setdispsize(hi_u32 layer_id, hi_u32 width, hi_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    hi_u32 pitch;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if ((display_info->display_width == width) && (display_info->display_height == height)) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_SUCCESS;
    }
    /*
     * for width and height check
     * width and height should less than stMaxScreenSize
     */
    if (hifb_disp_check_param(info, width, height) != HI_SUCCESS) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_FAILURE;
    }

    display_info->display_width = width;
    display_info->display_height = height;
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_INRECT;
    /* 3 is 8 bits */
    pitch = (((width * hifb_get_bits_per_pixel(info)) >> 3) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    if (pitch > info->oinfo.stride) {
        info->oinfo.stride = pitch;
        info->oinfo.sarea.h = info->vinfo.yres = display_info->display_height;
        info->oinfo.sarea.w = info->vinfo.xres = display_info->display_width;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    }

    hifb_set_dispbufinfo(layer_id);

    if (!g_drv_ops.capability[par->layer_id].bVoScale) {
        display_info->screen_width = width;
        display_info->screen_height = height;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_OUTRECT;
    }

    /*
     * here we need to think about how to resist flicker again,
     * we use VO do flicker resist before , but now if the display H size is the same as the screen,
     * VO will not do flicker resist, so should choose TDE to do flicker resist
     */
    hifb_select_antiflicker_mode(par);
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
}
#else
static hi_s32 hifb_disp_setdispsize(hi_u32 layer_id, hi_u32 width, hi_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    struct fb_var_screeninfo *var = &info->var;
    struct fb_fix_screeninfo *fix = &info->fix;
    hifb_display_info *display_info = &par->display_info;
    hi_u32 pitch;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if ((display_info->display_width == width) && (display_info->display_height == height)) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_SUCCESS;
    }
    /*
     * for width and height check
     * width and height should less than stMaxScreenSize
     */
    if (hifb_disp_check_param(info, width, height) != HI_SUCCESS) {
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_FAILURE;
    }

    display_info->display_width = width;
    display_info->display_height = height;
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_INRECT;
    /* 3 is 8 bits */
    pitch = (((width * hifb_get_bits_per_pixel(info))>> 3) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    if (pitch > fix->line_length) {
        fix->line_length = pitch;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    }
    /*
     * If the user calls FBIOPUT_LAYER_INFO to set display_width and display_height,then sync to xres yres,
     * Otherwise, there will be an error in the memory address in hifb_set_dispbufinfo
     */
    var->xres = display_info->display_width;
    var->yres = display_info->display_height;

    if (var->xres_virtual < display_info->display_width) {
        var->xres_virtual = display_info->display_width;
    }

    if (var->yres_virtual < display_info->display_height) {
        var->yres_virtual = display_info->display_height;
    }

    hifb_set_dispbufinfo(layer_id);

    if (!g_drv_ops.capability[par->layer_id].bVoScale) {
        display_info->screen_width = width;
        display_info->screen_height = height;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_OUTRECT;
    }

    /*
     * here we need to think about how to resist flicker again,
     * we use VO do flicker resist before , but now if the display H size is the same as the screen,
     * VO will not do flicker resist, so should choose TDE to do flicker resist
     */
    hifb_select_antiflicker_mode(par);
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
}
#endif

/*
 * we handle it by two case:
 * case 1 : if VO support Zoom, we only change screen size, g_display size keep not change
 * case 2: if VO can't support zoom, g_display size should keep the same as screen size
 */
static hi_s32 hifb_disp_setscreensize(hi_u32 layer_id, hi_u32 width, hi_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;

    /* If the chip does not support scaling, it is consistent with the process of setting the g_display area. */
    if (!g_drv_ops.capability[par->layer_id].bVoScale) {
        return hifb_disp_setdispsize(layer_id, width, height);
    }

    display_info->screen_width = width;
    display_info->screen_height = height;

    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    /*
     * Here  we need to think about how to resist flicker again, we use VO do flicker resist before ,
     * but now if the g_display H size is the same as the screen, VO will not do flicker resist, so should choose
     * TDE to do flicker resist
     */
    hifb_select_antiflicker_mode(par);

    return HI_SUCCESS;
}

hi_void hifb_buf_freemem(hi_u64 phyaddr)
{
    cmpi_mmz_free(phyaddr, HI_NULL);
}

static hi_s32 hifb_freeccanbuf(hifb_par *par)
{
    HIFB_SURFACE_S *canvas_sur = HI_NULL;
    if (par == HI_NULL) {
        return HI_FAILURE;
    }
    canvas_sur = &par->canvas_sur;

    if (canvas_sur->u64PhyAddr != 0) {
        hifb_buf_freemem(canvas_sur->u64PhyAddr);
    }
    canvas_sur->u64PhyAddr = 0;

    if (par->rotate_vb != 0) {
        cmpi_mmz_free(par->rotate_vb, HI_NULL);
        par->rotate_vb = 0;
    }

    return HI_SUCCESS;
}

static hi_void set_par_stride(struct fb_info *info)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hi_u32 stride;
    hi_u64 display_addr;

    stride = (((hifb_get_xres_virtual(info) * hifb_get_bits_per_pixel(info)) >> 3) + HIFB_ALIGNMENT) & /* 2^3 */
              (~HIFB_ALIGNMENT);
    if (stride != hifb_get_line_length(info) ||
        (hifb_get_yres(info) != display_info->y_res)) {
#ifdef __HuaweiLite__
        info->oinfo.stride = stride;
#else
        info->fix.line_length = stride;
#endif
        hifb_set_dispbufinfo(par->layer_id);
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    }

    display_addr = (hifb_get_smem_start(info) + stride * hifb_get_yoffset(info)
                   + hifb_get_xoffset(info) * (hifb_get_bits_per_pixel(info) >> 3)) & 0xfffffff0; /* 3 is 8 bits */
    if (display_addr != refresh_info->screen_addr) {
        refresh_info->screen_addr = display_addr;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    }
    return ;
}

static hi_s32 set_par_resolution(struct fb_info *info)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if (hifb_get_xres(info) != display_info->x_res ||
        hifb_get_yres(info) != display_info->y_res) {
        if ((hifb_get_xres(info) == 0) || (hifb_get_yres(info) == 0)) {
            if (par->show == HI_TRUE) {
                par->show = HI_FALSE;
                par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_SHOW;
            }
        }

         /*
          * The following two functions have a sleep operation, you must unlock before calling,
          * and lock the global variable inside the function.
          */
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

        if (hifb_disp_setdispsize(par->layer_id, hifb_get_xres(info), hifb_get_yres(info))
            != HI_SUCCESS) {
            return HI_FAILURE;
        }
        if (hifb_disp_setscreensize(par->layer_id, hifb_get_xres(info), hifb_get_yres(info))
            != HI_SUCCESS) {
            return HI_FAILURE;
        }

        hifb_spin_lock_irqsave(&par->lock, lock_flag);
    }

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

/*
 * Function        : hifb_set_par
 * Description     : set the variable parmater and make it use
 * Input           : struct fb_info *info
 * Return          : return 0
 */
static hi_s32 hifb_set_par(struct fb_info *info)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    HIFB_COLOR_FMT_E format;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);

    par->modifying = HI_TRUE;

    /* set the stride if stride change */
    set_par_stride(info);

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    /* If xres or yres change */
    if (set_par_resolution(info) != HI_SUCCESS) {
        return HI_FAILURE;
    }

#ifdef __HuaweiLite__
    format = info->vinfo.fmt;
#else
    format = hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue,
        &info->var.transp, hifb_get_bits_per_pixel(info));
#endif
    if ((par->color_format != format)) {
        hifb_freeccanbuf(par);
        hifb_set_fmt(par, format);
        par->cursor_info.cursor.stCursor.enFmt = format;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_FMT;
    }

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    display_info->x_res = hifb_get_xres(info);
    display_info->y_res = hifb_get_yres(info);
    display_info->vir_x_res = hifb_get_xres_virtual(info);
    display_info->vir_y_res = hifb_get_yres_virtual(info);

    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return 0;
}

/*
 * Function        : hifb_pan_display
 * Description     : pan g_display.
 * Input           : struct fb_var_screeninfo *var
 * Return          : return 0
 */
#ifdef __HuaweiLite__
static hi_s32 hifb_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo)
#else
static hi_s32 hifb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
#endif
{
#ifdef __HuaweiLite__
    struct hifb_info *info = (struct hifb_info *)vtable;
#endif

    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hi_u64 display_addr;
    hi_u32 stride;
    HIFB_BUFFER_S canvas_buf;
    hi_s32 ret;

    /* set the stride and g_display start address,3 is 8 bits, 0xf 0xfffffff0 mask */
    stride = (hifb_get_xres_virtual(info) * (hifb_get_bits_per_pixel(info) >> 3) + 0xf) & 0xfffffff0;

#ifdef __HuaweiLite__
    display_addr = (hifb_get_smem_start(info) + (hi_u64)stride * oinfo->sarea.y + (hi_u64)oinfo->sarea.x *
        (hifb_get_bits_per_pixel(info) >> 3)) & 0xfffffffffffffff0; /* 3 is 8 bits 0xfffffffffffffff0 mask */
#else
    /* 3 is 8 bits */
    display_addr = (hifb_get_smem_start(info) + (hi_u64)stride * var->yoffset + (hi_u64)var->xoffset *
        (hifb_get_bits_per_pixel(info) >> 3)) & 0xfffffffffffffff0; /* 3 is 8 bits 0xfffffffffffffff0 mask */
#endif
    canvas_buf.stCanvas.enFmt = par->color_format;
    canvas_buf.stCanvas.u64PhyAddr = display_addr;
    canvas_buf.stCanvas.u32Pitch = stride;
    canvas_buf.UpdateRect.x = 0;
    canvas_buf.UpdateRect.y = 0;
#ifdef __HuaweiLite__
    canvas_buf.stCanvas.u32Width = info->vinfo.xres;
    canvas_buf.stCanvas.u32Height = info->vinfo.yres;
    canvas_buf.UpdateRect.w = info->vinfo.xres;
    canvas_buf.UpdateRect.h = info->vinfo.xres;
    g_layer[par->layer_id].info->activate = info->activate;
#else
    canvas_buf.stCanvas.u32Width = hifb_get_xres(info);
    canvas_buf.stCanvas.u32Height = hifb_get_yres(info);
    canvas_buf.UpdateRect.w = hifb_get_xres(info);
    canvas_buf.UpdateRect.h = hifb_get_yres(info);
    g_layer[par->layer_id].info->var.activate = info->var.activate;
#endif
    refresh_info->buf_mode = HIFB_LAYER_BUF_BUTT;
    ret = hifb_refresh_0buf(par->layer_id, &canvas_buf);

    return ret;
}

static inline hi_void hifb_get_fmt(hifb_par *par, HIFB_COLOR_FMT_E *color_format)
{
    *color_format = par->color_format;
    return;
}

static inline hi_void hifb_set_fmt(hifb_par *par, HIFB_COLOR_FMT_E color_fmt)
{
    par->color_format = color_fmt;

    return;
}

static inline hi_void hifb_set_alpha(hifb_par *par, HIFB_ALPHA_S *alpha)
{
    osal_memcpy(&par->alpha, alpha, sizeof(HIFB_ALPHA_S));

    return;
}

static inline hi_void hifb_get_alpha(hifb_par *par, HIFB_ALPHA_S *alpha)
{
    osal_memcpy(alpha, &par->alpha, sizeof(HIFB_ALPHA_S));

    return;
}

static inline hi_void hifb_set_key(hifb_par *par, hifb_colorkeyex *key)
{
    osal_memcpy(&par->ckey, key, sizeof(hifb_colorkeyex));

    return;
}

static inline hi_void hifb_get_key(hifb_par *par, hifb_colorkeyex *key)
{
    osal_memcpy(key, &par->ckey, sizeof(hifb_colorkeyex));

    return;
}

static hi_void hifb_set_layerpos(hifb_par *par, HIFB_POINT_S *pos)
{
    hi_s32 x_pos;
    hi_s32 y_pos;
    SIZE_S max_screensize;
    hi_u32 layer_id;
    hifb_display_info *display_info = HI_NULL;
    if ((par == HI_NULL) || (pos == HI_NULL)) {
        return;
    }
    layer_id = par->layer_id;
    display_info = &par->display_info;

    hifb_get_maxscreensize(par, &max_screensize.u32Width, &max_screensize.u32Height);
    x_pos = pos->s32XPos;
    y_pos = pos->s32YPos;
    if (x_pos > max_screensize.u32Width - hifb_min_width(layer_id)) {
        hifb_warning("the sum of s32XPos(%d) and min_width(%d) larger than Vodev screen width(%d)!\n", x_pos,
                     hifb_min_width(layer_id), max_screensize.u32Width);
        x_pos = max_screensize.u32Width - hifb_min_width(layer_id);
    }

    if (y_pos > max_screensize.u32Height - hifb_min_height(layer_id)) {
        hifb_warning("the sum of s32YPos(%d) and min_height(%d) larger than Vodev screen height(%d)!\n", y_pos,
                     hifb_min_height(layer_id), max_screensize.u32Height);
        y_pos = max_screensize.u32Height - hifb_min_height(layer_id);
    }

    display_info->pos.s32XPos = x_pos;
    display_info->pos.s32YPos = y_pos;

    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    return;
}

#ifdef CURSOR
static inline hi_void hifb_set_cursorinfo(hifb_par *par, hifb_cursor_info *cursor_info)
{
    osal_memcpy(&par->cursor_info, cursor_info, sizeof(hifb_cursor_info));

    return;
}

static inline hi_void hifb_get_cursorinfo(hifb_par *par, hifb_cursor_info *cursor_info)
{
    osal_memcpy(cursor_info, &par->cursor_info, sizeof(hifb_cursor_info));

    return;
}
#endif

static inline hi_bool hifb_get_show(hifb_par *par)
{
    if (par != HI_NULL) {
        return par->show;
    } else {
        return HI_FALSE;
    }
}

static inline hi_void hifb_set_show(hifb_par *par, hi_bool show)
{
    if (par != HI_NULL) {
        par->show = show;
    }
}

hi_void hifb_get_layerinfo(hifb_par *par, HIFB_LAYER_INFO_S *layer_info)
{
    HIFB_POINT_S pos = {0};
    if ((par != HI_NULL) && (layer_info != HI_NULL)) {
        hifb_get_premul(par, &layer_info->bPreMul);
        hifb_get_bufmode(par, &layer_info->BufMode);
        hifb_get_antiflickerlevel(par, &layer_info->eAntiflickerLevel);
        hifb_get_layerpos(par, &pos);
        layer_info->s32XPos = pos.s32XPos;
        layer_info->s32YPos = pos.s32YPos;
        hifb_get_dispsize(par, &layer_info->u32DisplayWidth, &layer_info->u32DisplayHeight);
        hifb_get_screensize(par, &layer_info->u32ScreenWidth, &layer_info->u32ScreenHeight);
        layer_info->u32CanvasWidth = par->canvas_sur.u32Width;
        layer_info->u32CanvasHeight = par->canvas_sur.u32Height;
        layer_info->u32Mask = HIFB_LAYERMASK_BUTT;
    }
    return;
}

static hi_void hifb_get_idledispbuf(hifb_par *par, hi_u64 *phy_addr)
{
    hifb_refresh_info *refresh_info = HI_NULL;
    hi_u32 index_for_int;
    if ((par == HI_NULL) || (phy_addr == HI_NULL)) {
        return;
    }
    refresh_info = &par->refresh_info;
    index_for_int = refresh_info->disp_buf_info.index_for_int;
    /*
     * Only the 2buf refresh mode allows to get free buf, otherwise it is the buf in the current
     * interrupt processing.
     */
    if ((refresh_info->buf_mode == HIFB_LAYER_BUF_DOUBLE) ||
        (refresh_info->buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        *phy_addr = refresh_info->disp_buf_info.phy_addr[1 - index_for_int];
    } else {
        *phy_addr = refresh_info->disp_buf_info.phy_addr[index_for_int];
    }

    return;
}

static hi_void hifb_get_workdispbuf(hifb_par *par, hi_u64 *phy_addr)
{
    hifb_refresh_info *refresh_info = HI_NULL;
    hi_u32 index_for_int;
    if ((par == HI_NULL) || (phy_addr == HI_NULL)) {
        return;
    }
    refresh_info = &par->refresh_info;
    index_for_int = refresh_info->disp_buf_info.index_for_int;

    *phy_addr = refresh_info->disp_buf_info.phy_addr[index_for_int];

    return;
}

static hi_void hifb_get_dispsize(hifb_par *par, hi_u32 *width, hi_u32 *height)
{
    hifb_display_info *display_info = HI_NULL;
    if (par == HI_NULL) {
        return;
    }
    display_info = &par->display_info;

    if ((width != HI_NULL) && (height != HI_NULL)) {
        *width = display_info->display_width;
        *height = display_info->display_height;
    }

    return;
}

#ifdef CURSOR
static hi_void hifb_get_idlecursorbuf(hifb_par *par, hi_u64 *phy_addr)
{
    hi_u32 index_for_int;
    if ((par == HI_NULL) || (phy_addr == HI_NULL)) {
        return;
    }
    index_for_int = par->refresh_info.disp_buf_info.index_for_int;

    if ((par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE) ||
        (par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        *phy_addr = par->cursor_info.cursor.stCursor.u64PhyAddr +
                       (HIFB_CURSOR_DEF_VRAM * 1024) * (hi_u64)(1 - index_for_int) / 2; /* 2 1024 size KB */
    } else {
        *phy_addr = par->cursor_info.cursor.stCursor.u64PhyAddr +
                       (HIFB_CURSOR_DEF_VRAM * 1024) * (hi_u64)index_for_int / 2; /* 2 1024 size KB */
    }

    return;
}
#endif

static inline hi_void hifb_get_workcursorbuf(hifb_par *par, hi_u64 *phy_addr)
{
    hi_u32 index_for_int;
    index_for_int = par->refresh_info.disp_buf_info.index_for_int;
    *phy_addr = par->cursor_info.cursor.stCursor.u64PhyAddr +
        (HIFB_CURSOR_DEF_VRAM * 1024) * (hi_u64)index_for_int / 2; /* 2 1024 size KB */

    return;
}

static inline hi_void hifb_set_dispbufflip(hifb_par *par, hi_bool need_flip)
{
    par->refresh_info.disp_buf_info.need_flip = need_flip;

    return;
}

static inline hi_void hifb_get_bufmode(hifb_par *par, HIFB_LAYER_BUF_E *buf_mode)
{
    *buf_mode = par->refresh_info.buf_mode;

    return;
}

static inline hi_void hifb_get_premul(hifb_par *par, hi_bool *premul)
{
    *premul = par->display_info.premul;

    return;
}

static inline hi_void hifb_get_antiflickerlevel(hifb_par *par, HIFB_LAYER_ANTIFLICKER_LEVEL_E *pen_antiflicker_level)
{
    *pen_antiflicker_level = par->display_info.antiflicker_level;

    return;
}

static inline hi_void hifb_get_layerpos(hifb_par *par, HIFB_POINT_S *pos)
{
    osal_memcpy(pos, &par->display_info.pos, sizeof(HIFB_POINT_S));

    return;
}

static hi_void hifb_get_screensize(hifb_par *par, hi_u32 *width, hi_u32 *height)
{
    hifb_display_info *display_info = HI_NULL;
    if (par == HI_NULL) {
        return;
    }
    display_info = &par->display_info;

    if ((width != HI_NULL) && (height != HI_NULL)) {
        *width = display_info->screen_width;
        *height = display_info->screen_height;
    }

    return;
}

static hi_s32 hifb_set_screensize(hifb_par *par, hi_u32 *width, hi_u32 *height)
{
    hi_s32 ret;
    hifb_display_info *display_info = HI_NULL;

    ret = HI_SUCCESS;
    if ((par == HI_NULL) || (width == HI_NULL) || (height == HI_NULL)) {
        return HI_FAILURE;
    }
    display_info = &par->display_info;

    if (g_drv_ops.capability[par->layer_id].bVoScale) {
        display_info->screen_width = *width;
        display_info->screen_height = *height;
    } else {
        if ((display_info->display_width != *width) || (display_info->display_height != *height)) {
            hifb_error("ScreenSize(%u, %u) can't be different with DisplaySize(%u, %u) \
                       when layer %d don't scale\n", *width, *height,
                       display_info->display_width, display_info->display_height, par->layer_id);
            ret = HI_FAILURE;
        }
    }

    return ret;
}

static hi_void hifb_get_maxscreensize(hifb_par *par, hi_u32 *width, hi_u32 *height)
{
    hifb_osd_data layer_data;
    hifb_display_info *display_info = HI_NULL;
    if (par == HI_NULL) {
        return;
    }
    display_info = &par->display_info;

    if (g_drv_ops.hifb_drv_get_osd_data(par->layer_id, &layer_data) == HI_SUCCESS) {
        display_info->max_screen_width = layer_data.screen_width;
        display_info->max_screen_height = layer_data.screen_height;
    }

    if ((width != HI_NULL) && (height != HI_NULL)) {
        *width = display_info->max_screen_width;
        *height = display_info->max_screen_height;
    }

    return;
}

#if (HICHIP == HI3516E_V200)
static hi_s32 hifb_tde_rotate_callback(hi_u32 call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id, hi_void *job_data)
{
    hi_u32 layer_id;
    hi_s32 tde_finish_handle;
    hifb_par *par = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode;
    unsigned long lockflag;
    vgs_job_data *callback_job_data = (vgs_job_data *)job_data;
    layer_id = callback_job_data->private_data[0];
    tde_finish_handle = callback_job_data->private_data[1];
    if (layer_id <= g_drv_ops.layer_count) {
        par = (hifb_par *)(g_layer[layer_id].info->par);
    } else {
        hifb_error("u32LayerId = %u is invalid\n", layer_id);
        return HI_FAILURE;
    }
    refresh_info = &par->refresh_info;
    hifb_get_bufmode(par, &buf_mode);

    hifb_spin_lock_irqsave(&par->lock, lockflag);

    if (buf_mode == HIFB_LAYER_BUF_DOUBLE) {
        if (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle) {
            /* Notify VO, it can be updated now */
            hifb_set_dispbufflip(par, HI_TRUE);

            /* Compression is not supported when there is rotation */
            par->refresh_info.disp_buf_info.compress = HI_FALSE;
        } else {
            hifb_error("Layer(%d) refresh again before tde callback, maybe refresh too fast !\n", par->layer_id);
        }
    }

    hifb_spin_unlock_irqrestore(&par->lock, lockflag);
    return HI_SUCCESS;
}
#else
static hi_s32 hifb_tde_rotate_callback(const hi_void *paraml, const hi_void *paramr)
{
    hi_u32 layer_id = 0;
    hi_s32 tde_finish_handle;
    hifb_par *par = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode;
    unsigned long lockflag;
    hifb_tde_callback_param *param = (hifb_tde_callback_param *)paraml;
    tde_finish_handle = *(hi_s32 *)paramr;
    if (param != HI_NULL) {
        layer_id = param->layer_id;
    } else {
        hifb_error("param is HI_NULL\n");
        return HI_FAILURE;
    }
    if (layer_id <= g_drv_ops.layer_count) {
        par = (hifb_par *)(g_layer[layer_id].info->par);
    } else {
        hifb_error("u32LayerId = %u is invalid\n", layer_id);
        return HI_FAILURE;
    }
    refresh_info = &par->refresh_info;
    hifb_get_bufmode(par, &buf_mode);

    hifb_spin_lock_irqsave(&par->lock, lockflag);

    if (buf_mode == HIFB_LAYER_BUF_DOUBLE) {
        if (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle) {
            /* Notify VO, it can be updated now */
            hifb_set_dispbufflip(par, HI_TRUE);

            /* Compression is not supported when there is rotation */
            par->refresh_info.disp_buf_info.compress = HI_FALSE;
        } else {
            hifb_error("Layer(%d) refresh again before tde callback, maybe refresh too fast !\n", param->layer_id);
        }
    }

    hifb_spin_unlock_irqrestore(&par->lock, lockflag);
    if (param != HI_NULL) {
        kfree(param);
        param = HI_NULL;
    }
    return HI_SUCCESS;
}
#endif

static hi_void tde_callback_src_init(hifb_par *par, HIFB_BUFFER_S *src_img)
{
    hifb_display_info *display_info = &par->display_info;
    hi_u32 bytes_per_pixel = 2; /* 2 is PerPixel */
    src_img->stCanvas.u64PhyAddr = par->rotate_vb;
    src_img->stCanvas.u32Width = display_info->display_height;
    src_img->stCanvas.u32Height = display_info->display_width;
    src_img->UpdateRect.x = 0;
    src_img->UpdateRect.y = 0;
    src_img->UpdateRect.w = src_img->stCanvas.u32Width;
    src_img->UpdateRect.h = src_img->stCanvas.u32Height;

    src_img->stCanvas.enFmt = par->color_format;
    if (src_img->stCanvas.enFmt == HIFB_FMT_ARGB1555 ||
        src_img->stCanvas.enFmt == HIFB_FMT_ARGB4444) {
        bytes_per_pixel = 2; /* 2 is PerPixel */
    } else if(src_img->stCanvas.enFmt == HIFB_FMT_ARGB8888) {
        bytes_per_pixel = 4; /* 4 is PerPixel */
    }
    src_img->stCanvas.u32Pitch = ((bytes_per_pixel * src_img->stCanvas.u32Width + HIFB_ALIGN - 1) / HIFB_ALIGN) *
        HIFB_ALIGN;
    return ;
}

static hi_void tde_callback_dst_init(hifb_par *par, HIFB_BUFFER_S *dst_img, HIFB_LAYER_BUF_E buf_mode)
{
    hi_u32 bytes_per_pixel = 2; /* 2 is PerPixel */
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hifb_display_info *display_info = &par->display_info;
    if (buf_mode == HIFB_LAYER_BUF_DOUBLE) {
        hifb_get_idledispbuf(par, (hi_u64*)(&dst_img->stCanvas.u64PhyAddr));
    } else if (buf_mode == HIFB_LAYER_BUF_ONE) {
        dst_img->stCanvas.u64PhyAddr =
            refresh_info->disp_buf_info.phy_addr[refresh_info->disp_buf_info.index_for_int];
    } else if (buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE) {
        dst_img->stCanvas.u64PhyAddr =
            refresh_info->disp_buf_info.phy_addr[1 - refresh_info->disp_buf_info.index_for_int];
    }
    dst_img->stCanvas.u32Width = display_info->display_width;
    dst_img->stCanvas.u32Height = display_info->display_height;
    dst_img->stCanvas.enFmt = par->color_format;
    dst_img->UpdateRect.x = 0;
    dst_img->UpdateRect.y = 0;
    dst_img->UpdateRect.w = dst_img->stCanvas.u32Width;
    dst_img->UpdateRect.h = dst_img->stCanvas.u32Height;
    if (dst_img->stCanvas.enFmt == HIFB_FMT_ARGB1555 ||
        dst_img->stCanvas.enFmt == HIFB_FMT_ARGB4444) {
        bytes_per_pixel = 2; /* 2 is PerPixel */
    } else if (dst_img->stCanvas.enFmt == HIFB_FMT_ARGB8888) {
        bytes_per_pixel = 4; /* 4 is PerPixel */
    }
    dst_img->stCanvas.u32Pitch = ((bytes_per_pixel * dst_img->stCanvas.u32Width + HIFB_ALIGN - 1) / HIFB_ALIGN) *
        HIFB_ALIGN;
    return;
}

static hi_void tde_callback_with_rotate(hifb_par *par, HIFB_LAYER_BUF_E buf_mode)
{
    HIFB_BUFFER_S src_img;
    HIFB_BUFFER_S dst_img;
    hifb_rotate_opt rot_opt;
    hi_bool is_refresh_screen = HI_TRUE;
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;

    /* fill src image info */
    tde_callback_src_init(par, &src_img);

    /* fill dst image info */
    tde_callback_dst_init(par, &dst_img, buf_mode);

    /* fill rot option info */
    rot_opt.rotate_mode = display_info->rotate_mode;
    /*
     * Note: After turning on the callback, the rotation job callback function may not be called.
     * Need to wait until the next tde task is called, the direct phenomenon is: if the last run
     * In the use case, the rotation angle of 90 or 270 is set and the tde rotation branch is entered.
     * Unloading ko and loading ko will result in core dump.
     */
    if (buf_mode == HIFB_LAYER_BUF_DOUBLE) {
        rot_opt.call_back = HI_TRUE;
    } else if (buf_mode == HIFB_LAYER_BUF_ONE) {
        rot_opt.call_back = HI_FALSE;
    } else {
        rot_opt.call_back = HI_FALSE;
    }
    /* In the interrupt, the blocking mode is not allowed, so the non-blocking mode is used. */
    rot_opt.block = HI_FALSE;
    rot_opt.param = &(par->layer_id);

    /* Fill bRefreshScreen.Not used now */
    is_refresh_screen = HI_TRUE;
    refresh_info->disp_buf_info.refresh_handle = hifb_drv_rotate(&src_img, &dst_img, &rot_opt,
                                                                 is_refresh_screen);
    return;
}

#if (HICHIP == HI3516E_V200)
static hi_void tde_callback_without_rotate(hifb_par *par, HIFB_LAYER_BUF_E buf_mode,
                                           hi_void *job_data)
{
    hi_u32 ar_size, gb_size, buf_size, index_for_int;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    struct fb_info *info = HI_NULL;
    vgs_job_data *callback_job_data = (vgs_job_data *)job_data;
    hi_s32 tde_finish_handle = callback_job_data->private_data[1];
    info = g_layer[callback_job_data->private_data[0]].info;
    buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    index_for_int = refresh_info->disp_buf_info.index_for_int;
    if (buf_mode == HIFB_LAYER_BUF_DOUBLE) {
        if (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle) {
            /* Notify VO, can be updated */
            hifb_set_dispbufflip(par, HI_TRUE);
        } else {
            hifb_error("Layer(%d) refresh again before tde callback, maybe refresh too fast !\n", par->layer_id);
        }
    }

    if (par->compress_info.compress_open) {
        /*
         * In the case of compression,
         * the size of the compressed data stored in the first 16 bytes is obtained.
         */
        if ((buf_mode == HIFB_LAYER_BUF_ONE) && (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle)) {
            hifb_get_dcmp_framesize(HI_NULL, &ar_size, &gb_size,
                                    (hi_u8 *)(hifb_get_screen_base(info)), buf_size / 2); /* 2 buff size */
        } else if ((buf_mode == HIFB_LAYER_BUF_DOUBLE && refresh_info->disp_buf_info.refresh_handle ==
            tde_finish_handle) || (buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
            hifb_get_dcmp_framesize(HI_NULL, &ar_size, &gb_size, ((hi_u8 *)(hifb_get_screen_base(info)) +
                                    (1 - index_for_int) * buf_size), buf_size / 2); /* 2 buff size */
        } else {
            ar_size = 0;
            gb_size = 0;
            hifb_error("%s,%d,Layer(%d) buffer mode %d is supported,ARSize and GBSize are set to %d,%d\n",
                       __FUNCTION__, __LINE__, (hi_u32)callback_job_data->private_data[0], buf_mode, ar_size, gb_size);
        }
        par->compress_info.frame_size0 =  ar_size;
        par->compress_info.frame_size1 =  gb_size;
        par->compress_info.update_finished = HI_TRUE;
    }
    return;
}
#else
static hi_void tde_callback_without_rotate(hifb_par *par, HIFB_LAYER_BUF_E buf_mode,
                                           const hi_void *paraml, const hi_void *paramr)
{
    hi_u32 ar_size, gb_size, buf_size, index_for_int;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    struct fb_info *info = HI_NULL;
    hifb_tde_callback_param *param = (hifb_tde_callback_param *)paraml;
    hi_s32 tde_finish_handle = *(hi_s32 *)paramr;
    info = g_layer[param->layer_id].info;
    buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    index_for_int = refresh_info->disp_buf_info.index_for_int;
    if (buf_mode == HIFB_LAYER_BUF_DOUBLE) {
        if (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle) {
            /* Notify VO, can be updated */
            hifb_set_dispbufflip(par, HI_TRUE);
            par->refresh_info.disp_buf_info.compress = param->compress;
        } else {
            hifb_error("Layer(%d) refresh again before tde callback, maybe refresh too fast !\n", par->layer_id);
        }
    }

    if (par->compress_info.compress_open) {
        /*
         * In the case of compression,
         * the size of the compressed data stored in the first 16 bytes is obtained.
         */
        if ((buf_mode == HIFB_LAYER_BUF_ONE) && (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle)) {
            hifb_get_dcmp_framesize(HI_NULL, &ar_size, &gb_size,
                                    ((hi_u8 *)(hifb_get_screen_base(info))), buf_size / 2); /* 2 buff size */
        } else if ((buf_mode == HIFB_LAYER_BUF_DOUBLE && refresh_info->disp_buf_info.refresh_handle ==
            tde_finish_handle) || (buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
            hifb_get_dcmp_framesize(HI_NULL, &ar_size, &gb_size, ((hi_u8 *)(hifb_get_screen_base(info)) +
                                   (1 - index_for_int) * buf_size), buf_size / 2); /* 2 buff size */
        } else {
            ar_size = 0;
            gb_size = 0;
            hifb_error("%s,%d,Layer(%d) buffer mode %d is supported,ARSize and GBSize are set to %d,%d\n",
                       __FUNCTION__, __LINE__, param->layer_id, buf_mode, ar_size, gb_size);
        }
        par->compress_info.frame_size0 =  ar_size;
        par->compress_info.frame_size1 =  gb_size;
        par->compress_info.update_finished = HI_TRUE;
    }
    return;
}
#endif

#if (HICHIP == HI3516E_V200)
static hi_s32 hifb_tde_callback(hi_u32 call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id, hi_void *job_data)
{
    hi_u32 layer_id;
    hi_s32 tde_finish_handle;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode;
    unsigned long lock_flag;

    vgs_job_data *callback_job_data = (vgs_job_data *)job_data;
    layer_id = callback_job_data->private_data[0];
    tde_finish_handle = callback_job_data->private_data[1];

    if (layer_id <= g_drv_ops.layer_count) {
        info = g_layer[layer_id].info;
    } else {
        hifb_error("layer_id = %u is invalid\n", layer_id);
        return HI_FAILURE;
    }
    par          = (hifb_par *)(info->par);
    refresh_info = &par->refresh_info;
    display_info = &par->display_info;

    hifb_dbg_info("tde callback blit handle:%x, end handle:%x\n",
                  refresh_info->disp_buf_info.refresh_handle, tde_finish_handle);

    hifb_get_bufmode(par, &buf_mode);

    hifb_spin_lock_irqsave(&par->lock, lock_flag);

    if ((display_info->rotate_mode == HIFB_ROTATE_90 || display_info->rotate_mode == HIFB_ROTATE_270)) {
        /* hifb callback with rotate */
        tde_callback_with_rotate(par, buf_mode);
    } else {
        /* hifb callback without rotate */
        tde_callback_without_rotate(par, buf_mode, job_data);
    }

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    return HI_SUCCESS;
}

#else
static hi_s32 hifb_tde_callback(const hi_void *paraml, const hi_void *paramr)
{
    hi_u32 layer_id = 0;
    hi_s32 tde_finish_handle;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    HIFB_LAYER_BUF_E buf_mode;
    unsigned long lock_flag;
    hifb_tde_callback_param *param = (hifb_tde_callback_param *)paraml;
    tde_finish_handle = *(hi_s32 *)paramr;

    /* here get a wrong layer_id, make hifb core dump here */
    if (param != HI_NULL) {
        layer_id = param->layer_id;
    } else {
        hifb_error("paraml is HI_NULL\n");
        return HI_FAILURE;
    }

    if (layer_id <= g_drv_ops.layer_count) {
        info = g_layer[layer_id].info;
    } else {
        hifb_error("layer_id = %u is invalid\n", layer_id);
        return HI_FAILURE;
    }
    par          = (hifb_par *)(info->par);
    refresh_info = &par->refresh_info;
    display_info = &par->display_info;

    hifb_dbg_info("tde callback blit handle:%x, end handle:%x\n",
                  refresh_info->disp_buf_info.refresh_handle, tde_finish_handle);

    hifb_get_bufmode(par, &buf_mode);

    hifb_spin_lock_irqsave(&par->lock, lock_flag);

    if ((display_info->rotate_mode == HIFB_ROTATE_90 || display_info->rotate_mode == HIFB_ROTATE_270)) {
        /* hifb callback with rotate */
        tde_callback_with_rotate(par, buf_mode);
    } else {
        /* hifb callback without rotate */
        tde_callback_without_rotate(par, buf_mode, paraml, paramr);
    }

    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);

    if (param != HI_NULL) {
        kfree(param);
        param = HI_NULL;
    }
    return HI_SUCCESS;
}
#endif

static inline hi_bool hifb_is_interlace(hifb_par *par)
{
    hifb_osd_data osd_data = {0};
    if (par != HI_NULL) {
        g_drv_ops.hifb_drv_get_osd_data(par->layer_id, &osd_data);
    }
    return (osd_data.scan_mode == HIFB_SCANMODE_I);
}

hi_u64 hifb_buf_allocmem(const hi_char *buf_name, hi_u32 size, hi_ulong layer_size, const hi_char *mmz_name)
{
    hi_u64 addr;

    if ((layer_size == 0) || (layer_size > 0x40000000)) {
        return 0;
    }

    addr = cmpi_mmz_malloc((hi_char *)mmz_name, (hi_char *)buf_name, layer_size);
    if ((addr == MMB_ADDR_INVALID) && (mmz_name != HI_NULL)) {
        addr = cmpi_mmz_malloc(HI_NULL, (hi_char *)buf_name, layer_size);
    }

    if (addr == MMB_ADDR_INVALID) {
        hifb_error("alloc mem failed!\n");
        return 0;
    }

    return addr;
}

static hi_void hifb_set_bufmode(hi_u32 layer_id, HIFB_LAYER_BUF_E layer_buf_mode)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;

    /* in 0 buf mode ,maybe the stride or fmt will be changed! */
    if ((refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) && (refresh_info->buf_mode != layer_buf_mode)) {
        par->modifying = HI_TRUE;

        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_STRIDE;

        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_FMT;

        par->modifying = HI_FALSE;
    }

    refresh_info->buf_mode = layer_buf_mode;
}

/*
 * choose the module to do  flicker resiting, TDE or VOU ? the rule is as this ,the moudle should do flicker resisting
 * who has do scaling
 */
static hi_void hifb_select_antiflicker_mode(hifb_par *par)
{
    hifb_display_info *display_info = HI_NULL;
    if (par == HI_NULL) {
        return;
    }
    display_info = &par->display_info;

    /* if the usr's configuration is no needed to do flicker resisting, so no needed to do it */
    if (display_info->antiflicker_level == HIFB_LAYER_ANTIFLICKER_NONE) {
        display_info->antiflicker_mode = HIFB_ANTIFLICKER_NONE;
    } else {
        /* current standard no needed to do flicker resisting */
        if (!display_info->need_antiflicker) {
            display_info->antiflicker_mode = HIFB_ANTIFLICKER_NONE;
        } else {
            /* VO has done scaling , so should do flicker resisting at the same time */
            if ((display_info->display_width != display_info->screen_width) ||
                (display_info->display_height != display_info->screen_height)) {
                display_info->antiflicker_mode = HIFB_ANTIFLICKER_VO;
            } else {
                display_info->antiflicker_mode = HIFB_ANTIFLICKER_TDE;
            }
        }
    }
}

static hi_void hifb_set_antiflickerlevel(hi_u32 layer_id, HIFB_LAYER_ANTIFLICKER_LEVEL_E antiflicker_level)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;

    display_info->antiflicker_level = antiflicker_level;
    hifb_select_antiflicker_mode(par);

    return;
}

#ifdef CURSOR
/* restore or update cursor backup */
static hi_void hifb_cursor_bakup(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;

    HIFB_BUFFER_S cursor_buf, display_buf;

    hifb_blit_opt blit_opt = {0};

    osal_memset(&blit_opt, 0, sizeof(hifb_blit_opt));

    if (par->refresh_info.buf_mode == HIFB_LAYER_BUF_NONE) {
        if (par->refresh_info.user_buffer.stCanvas.u64PhyAddr == 0) {
            return;
        }

        osal_memcpy(&display_buf.stCanvas, &(par->refresh_info.user_buffer.stCanvas), sizeof(HIFB_SURFACE_S));
    } else {
        display_buf.stCanvas.enFmt = par->color_format;
        display_buf.stCanvas.u32Height = par->display_info.display_height;
        display_buf.stCanvas.u32Width = par->display_info.display_width;
        display_buf.stCanvas.u32Pitch = hifb_get_line_length(info);
        hifb_get_workdispbuf(par, &display_buf.stCanvas.u64PhyAddr);
    }

    osal_memcpy(&cursor_buf.stCanvas, &par->cursor_info.cursor.stCursor, sizeof(HIFB_SURFACE_S));

    osal_memcpy(&cursor_buf.UpdateRect, &(par->cursor_info.rect_in_disp_buf), sizeof(HIFB_RECT));
    cursor_buf.UpdateRect.x = 0;
    cursor_buf.UpdateRect.y = 0;
    hifb_get_workcursorbuf(par, &cursor_buf.stCanvas.u64PhyAddr);

    osal_memcpy(&display_buf.UpdateRect, &(par->cursor_info.rect_in_disp_buf), sizeof(HIFB_RECT));

    blit_opt.mirror_mode = par->display_info.mirror_mode;

    if (hifb_drv_blit(&display_buf, &cursor_buf, &blit_opt, HI_FALSE, HI_NULL) < 0) {
        hifb_error("blit err! 1\n");
        return;
    }
    if ((par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE) ||
        (par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        hifb_get_idledispbuf(par, &display_buf.stCanvas.u64PhyAddr);
        hifb_get_idlecursorbuf(par, &cursor_buf.stCanvas.u64PhyAddr);
        if (hifb_drv_blit(&display_buf, &cursor_buf, &blit_opt, HI_FALSE, HI_NULL) < 0) {
            hifb_error("blit err! 2\n");
            return;
        }
    }
    return;
}

static hi_s32 cursor_show_blit(hi_u32 layer_id, hifb_par *cursor_par)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = info->par;
    hifb_blit_opt blit_opt = {0};
    HIFB_BUFFER_S cursor_buf;
    HIFB_BUFFER_S display_buf;

    osal_memset(&blit_opt, 0, sizeof(hifb_blit_opt));

    cursor_buf.stCanvas = cursor_par->cursor_info.cursor.stCursor;
    blit_opt.alpha = cursor_par->alpha;
    blit_opt.ckey = cursor_par->ckey;
    blit_opt.mirror_mode = par->display_info.mirror_mode;

    if (par->refresh_info.buf_mode == HIFB_LAYER_BUF_NONE) {
        if (par->refresh_info.user_buffer.stCanvas.u64PhyAddr == 0) {
            return HI_FAILURE;
        }
        osal_memcpy(&display_buf, &(par->refresh_info.user_buffer), sizeof(HIFB_BUFFER_S));
    } else {
        display_buf.stCanvas.enFmt = par->color_format;
        display_buf.stCanvas.u32Height = par->display_info.display_height;
        display_buf.stCanvas.u32Width = par->display_info.display_width;
        display_buf.stCanvas.u32Pitch = hifb_get_line_length(info);
        hifb_get_idledispbuf(par, (hi_u64*)&display_buf.stCanvas.u64PhyAddr);
    }

    osal_memcpy(&cursor_buf.UpdateRect, &(par->cursor_info.rect_in_disp_buf), sizeof(HIFB_RECT));
    cursor_buf.UpdateRect.x = par->cursor_info.pos_in_cursor.s32XPos;
    cursor_buf.UpdateRect.y = par->cursor_info.pos_in_cursor.s32YPos;

    osal_memcpy(&display_buf.UpdateRect, &(par->cursor_info.rect_in_disp_buf), sizeof(HIFB_RECT));

    if (hifb_drv_blit(&cursor_buf, &display_buf, &blit_opt, HI_FALSE, HI_NULL) < 0) {
        hifb_error("blit err! 6\n");
        return HI_FAILURE;
    }

    if ((par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE) ||
        (par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        hifb_get_workdispbuf(par, (hi_u64*)&display_buf.stCanvas.u64PhyAddr);
        if (hifb_drv_blit(&cursor_buf, &display_buf, &blit_opt, HI_FALSE, HI_NULL) < 0) {
            hifb_error("blit err!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_cursor_show(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = info->par;

    struct fb_info *cursor_info = HI_NULL;
    hifb_par *cursor_par = HI_NULL;

    if (!par->cursor_info.attched || !par->show) {
        hifb_dbg_info("Cursor should be attached to layer%u \n", layer_id);
        return HI_FAILURE;
    }

    cursor_info = g_layer[par->cursor_info.attached_cursor_id].info;
    cursor_par = cursor_info->par;

    if (!cursor_par->show) {
        return HI_SUCCESS;
    }

    if (cursor_par->cursor_info.cursor.stCursor.u64PhyAddr == 0) {
        hifb_dbg_info("No cusor img set!\n");
        return HI_FAILURE;
    }

    hifb_cursor_calcdispinfo(cursor_par->layer_id, par, &cursor_par->display_info.pos);

    hifb_cursor_bakup(layer_id);

    /* fill cursor_buf,display_buf,blit_opt, and blit */
    if (cursor_show_blit(layer_id, cursor_par) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    par->show = HI_TRUE;

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = HI_FALSE;
        /* 40 Waiting for TDE to complete the blit */
        schedule_timeout(msecs_to_jiffies(40));
        par->compress_info.update_rect.x = 0;
        par->compress_info.update_rect.y = 0;
        par->compress_info.update_rect.w = par->display_info.display_width;
        par->compress_info.update_rect.h = par->display_info.display_height;
        par->compress_info.update_finished = HI_TRUE;
    }
    return HI_SUCCESS;
}

static hi_s32 cursor_hide_blit(hi_u32 layer_id, hifb_par *cursor_par)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = info->par;
    hifb_blit_opt blit_opt = {0};
    HIFB_BUFFER_S cursor_buf;
    HIFB_BUFFER_S display_buf;

    osal_memset(&blit_opt, 0, sizeof(hifb_blit_opt));

    cursor_buf.stCanvas = par->cursor_info.cursor.stCursor;

    if (par->refresh_info.buf_mode == HIFB_LAYER_BUF_NONE) {
        if (par->refresh_info.user_buffer.stCanvas.u64PhyAddr == 0) {
            hifb_dbg_info("No user buf send to fb in 0 buf mode!\n");
            return HI_FAILURE;
        }
        osal_memcpy(&display_buf, &(par->refresh_info.user_buffer), sizeof(HIFB_BUFFER_S));
    } else {
        display_buf.stCanvas.enFmt = par->color_format;
        display_buf.stCanvas.u32Height = par->display_info.display_height;
        display_buf.stCanvas.u32Width = par->display_info.display_width;
        display_buf.stCanvas.u32Pitch = hifb_get_line_length(info);
        hifb_get_idledispbuf(par, (hi_u64*)&display_buf.stCanvas.u64PhyAddr);
    }

    osal_memcpy(&cursor_buf.UpdateRect, &(par->cursor_info.rect_in_disp_buf), sizeof(HIFB_RECT));
    cursor_buf.UpdateRect.x = 0;
    cursor_buf.UpdateRect.y = 0;

    osal_memcpy(&display_buf.UpdateRect, &(par->cursor_info.rect_in_disp_buf), sizeof(HIFB_RECT));

    hifb_get_idlecursorbuf(par, (hi_u64*)&cursor_buf.stCanvas.u64PhyAddr);

    blit_opt.mirror_mode = par->display_info.mirror_mode;

    if (hifb_drv_blit(&cursor_buf, &display_buf, &blit_opt, HI_FALSE, HI_NULL) < 0) {
        hifb_error("blit err!\n");
        return HI_FAILURE;
    }
    if ((par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE) ||
        (par->refresh_info.buf_mode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        hifb_get_workdispbuf(par, (hi_u64*)&display_buf.stCanvas.u64PhyAddr);
        hifb_get_workcursorbuf(par, (hi_u64*)&cursor_buf.stCanvas.u64PhyAddr);

        if (hifb_drv_blit(&cursor_buf, &display_buf, &blit_opt, HI_FALSE, HI_NULL) < 0) {
            hifb_error("blit err!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_cursor_hide(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = info->par;
    struct fb_info *cursor_info = HI_NULL;
    hifb_par *cursor_par = HI_NULL;

    if (!par->cursor_info.attched) {
        hifb_dbg_info("Cursor should be attached to layer%u \n", layer_id);
        return HI_FAILURE;
    }

    cursor_info = g_layer[par->cursor_info.attached_cursor_id].info;
    cursor_par = (hifb_par *)cursor_info->par;

    if (!cursor_par->show || !par->show) {
        return HI_SUCCESS;
    }

    /* fill cursor_buf,display_buf,blit_opt, and blit */
    if (cursor_hide_blit(layer_id, cursor_par) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = HI_FALSE;
        /* 40 Waiting for TDE to complete the blit */
        schedule_timeout(msecs_to_jiffies(40));
        par->compress_info.update_rect.x = 0;
        par->compress_info.update_rect.y = 0;
        par->compress_info.update_rect.w = par->display_info.display_width;
        par->compress_info.update_rect.h = par->display_info.display_height;
        par->compress_info.update_finished = HI_TRUE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_cursor_attach(hi_u32 cursor_id, hi_u32 layer_id)
{
    hi_u32 cnt;

    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;

    struct fb_info *cursorinfo = g_layer[cursor_id].info;
    hifb_par *cursor_par = (hifb_par *)cursorinfo->par;

    unsigned long lock_flag;

    cnt = atomic_read(&par->ref_count);
    if (cnt == 0) {
        hifb_error("failed to attach layer! The layer :%u is not opened!\n", layer_id);
        return HI_FAILURE;
    }

    if (hifb_is_clutfmt(par->color_format)) {
        hifb_error("failed to attach layer! The layer format is clut not supported!\n");
        return HI_FAILURE;
    }

    if (cursor_par->cursor_info.cursor.stCursor.u64PhyAddr == 0) {
        hifb_error("failed to attach layer! The cursor info is not set yet!\n");
        return HI_FAILURE;
    }

    if (par->cursor_info.attched) {
        if (par->cursor_info.attached_cursor_id == cursor_id) {
            return HI_SUCCESS;
        } else {
            hifb_error("The layer has attached another cursor, you should detach it first!\n");
            return HI_FAILURE;
        }
    }

    par->cursor_info.attched = 1;
    par->cursor_info.attached_cursor_id = cursor_id;

    if ((par->cursor_info.cursor.stCursor.u32Height > par->display_info.display_height) ||
        (par->cursor_info.cursor.stCursor.u32Width > par->display_info.display_width)) {
        return HI_FAILURE;
    }

    par->cursor_info.cursor.stCursor.u32Height = cursor_par->cursor_info.cursor.stCursor.u32Height;
    par->cursor_info.cursor.stCursor.u32Width = cursor_par->cursor_info.cursor.stCursor.u32Width;
    par->cursor_info.cursor.stHotPos = cursor_par->cursor_info.cursor.stHotPos;

    /* when cursor attach to layer, we use the positon calculate before */
    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    if (hifb_cursor_show(layer_id) != HI_SUCCESS) {
        hifb_error("hifb_cursor_show HI_FAILURE\r\n");
    }
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

static hi_s32 hifb_cursor_detach(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;

    if (!par->cursor_info.attched) {
        return HI_SUCCESS;
    }

    if (hifb_cursor_hide(layer_id) != HI_SUCCESS) {
        hifb_error("hifb_cursor_hide HI_FAILURE\r\n");
    }

    par->cursor_info.attched = HI_FALSE;
    return HI_SUCCESS;
}

static hi_void cursor_calc_display_rect(hifb_par *par, const HIFB_POINT_S* cursor_new_pos, HIFB_RECT *rect)
{
    hi_tde_rect src_rect = {0};
    hi_tde_rect dst_rect = {0};
    hi_tde_rect in_src_rect = {0};
    hi_tde_rect in_dst_rect = {0};
    if ((par->refresh_info.user_buffer.stCanvas.u32Height && par->refresh_info.user_buffer.stCanvas.u32Width &&
        (par->refresh_info.user_buffer.stCanvas.u32Height != par->display_info.display_height)) ||
        (par->refresh_info.user_buffer.stCanvas.u32Width && par->refresh_info.user_buffer.stCanvas.u32Height &&
        (par->refresh_info.user_buffer.stCanvas.u32Width != par->display_info.display_width))) {
        src_rect.width = par->refresh_info.user_buffer.stCanvas.u32Width;
        src_rect.height = par->refresh_info.user_buffer.stCanvas.u32Height;
        dst_rect.width = par->display_info.display_width;
        dst_rect.height = par->display_info.display_height;
        in_src_rect.pos_x = cursor_new_pos->s32XPos;
        in_src_rect.pos_y = cursor_new_pos->s32YPos;
        tde_cal_scale_rect_hifb(&src_rect, &dst_rect, &in_src_rect, &in_dst_rect);
        rect->x = in_dst_rect.pos_x;
        rect->y = in_dst_rect.pos_y;
    } else {
        rect->x = cursor_new_pos->s32XPos;
        rect->y = cursor_new_pos->s32YPos;
    }

    if (rect->x > (hi_s32)par->display_info.display_width) {
        rect->x = (hi_s32)(par->display_info.display_width - 1);
    }

    if (rect->y > (hi_s32)par->display_info.display_height) {
        rect->y = (hi_s32)(par->display_info.display_height - 1);
    }
    return;
}

/*
 * calculate cusor display position info such as the start position of copying,
 * display buffer position, width of copying.
 */
static hi_void hifb_cursor_calcdispinfo(hi_u32 cursor_id, hifb_par *par, const HIFB_POINT_S *cursor_new_pos)
{
    struct fb_info *info = HI_NULL;
    hifb_par *cursor_par = HI_NULL;
    HIFB_POINT_S pos = {0};
    HIFB_RECT rect = {0};

    info = g_layer[cursor_id].info;
    cursor_par = (hifb_par *)info->par;

    if ((par == HI_NULL) || (cursor_new_pos == HI_NULL)) {
        return;
    }

    /* calculate in display rect:(x,y) */
    cursor_calc_display_rect(par, cursor_new_pos, &rect);
    rect.x -= cursor_par->cursor_info.cursor.stHotPos.s32XPos;
    rect.y -= cursor_par->cursor_info.cursor.stHotPos.s32YPos;

    /* calculate in display rect:(w,h) */
    rect.w = (hi_s32)cursor_par->cursor_info.cursor.stCursor.u32Width;
    rect.h = (hi_s32)cursor_par->cursor_info.cursor.stCursor.u32Height;
    if ((hi_s32)rect.x < 0) {
        rect.x = 0;
        pos.s32XPos = cursor_par->cursor_info.cursor.stHotPos.s32XPos;
        rect.w -= pos.s32XPos;
    }

    if ((hi_s32)rect.y < 0) {
        rect.y = 0;
        pos.s32YPos = cursor_par->cursor_info.cursor.stHotPos.s32YPos;
        rect.h -= pos.s32YPos;
    }

    if (rect.x + rect.w > (hi_s32)par->display_info.display_width) {
        rect.w = (hi_s32)(par->display_info.display_width - rect.x);
    }

    if (rect.y + rect.h > (hi_s32)par->display_info.display_height) {
        rect.h = (hi_s32)(par->display_info.display_height - rect.y);
    }

    par->cursor_info.pos_in_cursor = pos;
    par->cursor_info.rect_in_disp_buf = rect;

    return;
}

static hi_s32 hifb_cursor_changepos(hi_u32 cursor_id, HIFB_POINT_S pos)
{
    struct fb_info *cursorinfo = g_layer[cursor_id].info;
    hifb_par *cursor_par = (hifb_par *)cursorinfo->par;

    hi_u32 i;

    if ((pos.s32XPos < 0) || (pos.s32YPos < 0)) {
        hifb_error("the cursor pos(%d,%d) out of range !\n", pos.s32XPos, pos.s32YPos);
        return HI_FAILURE;
    }

    /* pos no change */
    if ((pos.s32XPos == cursor_par->display_info.pos.s32XPos) &&
        (pos.s32YPos == cursor_par->display_info.pos.s32YPos)) {
        return HI_SUCCESS;
    }

    cursor_par->display_info.pos.s32XPos = pos.s32XPos;
    cursor_par->display_info.pos.s32YPos = pos.s32YPos;

    if (!cursor_par->show) {
        return HI_FAILURE;
    }

    /* process all layers attached to cursor */
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        struct fb_info *info = g_layer[i].info;
        hifb_par *par = (hifb_par *)info->par;

        if (is_cursor_layer(i)) {
            continue;
        }

        if (!par->cursor_info.attched) {
            continue;
        }
        if (hifb_cursor_hide(i) != HI_SUCCESS) {
            hifb_error("hifb_cursor_hide HI_FAILURE\r\n");
        }
        if (hifb_cursor_show(i) != HI_SUCCESS) {
            hifb_error("hifb_cursor_show HI_FAILURE\r\n");
        }
    }

    return HI_SUCCESS;
}

hi_s32 hifb_cursor_changestate(hifb_par *cursor_par, hi_bool show)
{
    hi_u32 i;
    if (cursor_par == HI_NULL) {
        return HI_FAILURE;
    }

    if (!cursor_par->cursor_info.cursor.stCursor.u64PhyAddr) {
        hifb_error("The cursor image addr is HI_NULL!\n");
        return HI_FAILURE;
    }

    /* no change to state , return */
    if (show == cursor_par->show) {
        return HI_SUCCESS;
    }

    for (i = 0; i < g_drv_ops.layer_count; i++) {
        if (is_cursor_layer(i)) {
            continue;
        }

        if (show) {
            cursor_par->show = HI_TRUE;
            hifb_cursor_show(i);
        } else {
            hifb_cursor_hide(i);
        }
    }

    cursor_par->show = show;

    return HI_SUCCESS;
}

static hi_s32 cursor_info_check(HIFB_CURSOR_S* cursor)
{
    if (cursor->stCursor.u32Width == 0 || cursor->stCursor.u32Height == 0) {
        hifb_error("cursor's width or height shouldn't be 0!\n");
        return HI_FAILURE;
    }

    if (cursor->stCursor.u32Pitch == 0) {
        hifb_error("cursor's pitch shouldn't be 0!\n");
        return HI_FAILURE;
    }

    if (cursor->stCursor.enFmt == HIFB_FMT_BUTT) {
        hifb_error("unknown color format!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_cursor_putinfo(hifb_par *cursor_par, HIFB_CURSOR_S *cursor)
{
    if ((cursor_par == HI_NULL) || (cursor == HI_NULL)) {
        return HI_FAILURE;
    }

    if (cursor->stCursor.u64PhyAddr == 0) {
        hifb_error("cursor image addr is equal to 0!\n");
        cursor_par->cursor_info.cursor.stCursor.u64PhyAddr = 0;
        return HI_SUCCESS;
    }

    /* cursor info check:width,height,pitch,format */
    if (cursor_info_check(cursor) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    cursor_par->color_format = cursor->stCursor.enFmt;
    if (cursor_par->color_format >= (sizeof(g_argb_bit_field) / sizeof(g_argb_bit_field[0]))) {
        hifb_error("unknown color fmt!\n");
        return HI_FAILURE;
    }
#ifndef __HuaweiLite__
    osal_memcpy(&g_layer[cursor_par->layer_id].info->var.red, &g_argb_bit_field[cursor_par->color_format].red,
                sizeof(struct fb_bitfield));
    osal_memcpy(&g_layer[cursor_par->layer_id].info->var.green,
                &g_argb_bit_field[cursor_par->color_format].green, sizeof(struct fb_bitfield));
    osal_memcpy(&g_layer[cursor_par->layer_id].info->var.blue,
                &g_argb_bit_field[cursor_par->color_format].blue, sizeof(struct fb_bitfield));
    osal_memcpy(&g_layer[cursor_par->layer_id].info->var.transp,
                &g_argb_bit_field[cursor_par->color_format].transp, sizeof(struct fb_bitfield));
#endif
    /* change hotx or hoty will result in cursor position change */
    if (cursor->stCursor.u32Height > HIFB_MAX_CURSOR_HEIGHT) {
        cursor->stCursor.u32Height = HIFB_MAX_CURSOR_HEIGHT;
    }

    if (cursor->stCursor.u32Width > HIFB_MAX_CURSOR_WIDTH) {
        cursor->stCursor.u32Width = HIFB_MAX_CURSOR_WIDTH;
    }

    if ((cursor->stHotPos.s32XPos < 0) || (cursor->stHotPos.s32XPos > cursor->stCursor.u32Width) ||
        (cursor->stHotPos.s32YPos < 0) || (cursor->stHotPos.s32YPos > cursor->stCursor.u32Height)) {
        hifb_error("hotpos err!\n");
        return HI_FAILURE;
    }

    if ((cursor->stHotPos.s32XPos != cursor_par->cursor_info.cursor.stHotPos.s32XPos) ||
        (cursor->stHotPos.s32YPos != cursor_par->cursor_info.cursor.stHotPos.s32YPos)) {
        cursor_par->cursor_info.cursor.stHotPos.s32XPos = cursor->stHotPos.s32XPos;
        cursor_par->cursor_info.cursor.stHotPos.s32YPos = cursor->stHotPos.s32YPos;
    }

    /* to do :update backup */
    osal_memcpy(&(cursor_par->cursor_info.cursor), cursor, sizeof(HIFB_CURSOR_S));
    return HI_SUCCESS;
}
#endif

/* Does the chip support scaling */
static hi_bool hifb_check_imagezoomenable(hi_u32 layer_id, HIFB_RECT *in_rect, HIFB_RECT *out_rect)
{
    hi_bool is_layer_support_zoom_out = HI_TRUE;
    hi_bool need_zoom = HI_FALSE;

    if ((in_rect == HI_NULL) || (out_rect == HI_NULL)) {
        return HI_FALSE;
    }
    if (!g_drv_ops.capability[layer_id].bVoScale) {
        return HI_FALSE;
    }

    if (g_drv_ops.hifb_drv_is_layer_support_zoom_out) {
        is_layer_support_zoom_out = g_drv_ops.hifb_drv_is_layer_support_zoom_out(layer_id);
    }

    /* The chip does not support zoomout, and the current image output size is smaller than the input size. */
    if ((is_layer_support_zoom_out == HI_FALSE) && ((out_rect->w < in_rect->w) || (out_rect->h < in_rect->h))) {
        hifb_error("HIFB layer%u not support zoomout, please check the g_display and screen size!\n", layer_id);
        return HI_FALSE;
    }

    /* The chip zoomin ratio exceeds the maximum allowed by the chip, and the error is returned. */
    if ((out_rect->w > (in_rect->w * HIFB_MAX_ZOOMIN)) || (out_rect->h > (in_rect->h * HIFB_MAX_ZOOMIN))) {
        hifb_error("HIFB layer%u in_size(%d, %d) and out_size(%d, %d) do out of ZoomRatio[1, %d]!!\n", layer_id,
                   in_rect->w, in_rect->h, out_rect->w, out_rect->h, VPSS_MAX_ZOOMIN);
        return HI_FALSE;
    }

    need_zoom = ((out_rect->w != in_rect->w) || (out_rect->h != in_rect->h));

    if (need_zoom && (in_rect->w > HIFB_LINE_BUF)) {
        hifb_error("HIFB layer%u in width: %u is bigger than %d, will not zoom in!!\n", layer_id, in_rect->w,
                   HIFB_LINE_BUF);
        return HI_FALSE;
    }

    return HI_TRUE;
}

hi_void hifb_dcmp_config(hi_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)(info->par);
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hi_bool dcmp_state_en = HI_FALSE;

    /*
     * In order to support compressed and non-compressed dynamic switching, you cannot use
     * pstCompressInfo->bCompressOpen to determine whether to open compression.
     * To use the compression information that follows the frame, and whether or not
     * to flip to decide whether to switch compression or non-compression
     */
    if (refresh_info->disp_buf_info.compress) {
        /* See HIFB_DRV_EnableDcmp */
        g_drv_ops.hifb_drv_enable_dcmp(layer_id, HI_TRUE);
        /* See HIFB_DRV_GetDcmpEnableState */
        g_drv_ops.hifb_drv_get_dcmp_enable_state(layer_id, &dcmp_state_en);
        if (dcmp_state_en) {
            hifb_set_dcmp_info(par);
        }
    } else {
        /*
         * Set the address and stride to ensure that the compression will g_display correctly when switched to
         * non-compressed
         */
        g_drv_ops.hifb_drv_set_layer_addr(layer_id, refresh_info->screen_addr);
        if ((refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) && refresh_info->user_buffer.stCanvas.u64PhyAddr) {
            g_drv_ops.hifb_drv_set_layer_stride(layer_id, refresh_info->user_buffer.stCanvas.u32Pitch);
        } else {
            g_drv_ops.hifb_drv_set_layer_stride(layer_id, hifb_get_line_length(info));
        }

        g_drv_ops.hifb_drv_enable_dcmp(layer_id, HI_FALSE);
        g_drv_ops.hifb_drv_enable_wbc(layer_id, HI_FALSE);
    }
}

/* Callback function for VO vertical timing interrupt */
static hi_s32 callback_get_osd_data(hifb_par *par, hi_u32 layer_id)
{
    hifb_osd_data layer_data = {0};
    hifb_display_info *display_info = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;

    display_info = &par->display_info;
    refresh_info = &par->refresh_info;

    if (g_drv_ops.hifb_drv_get_osd_data(par->layer_id, &layer_data) != HI_SUCCESS) {
        hifb_error("failed to get layer%d's osd data!\n", par->layer_id);
        return HI_FAILURE;
    }
    display_info->max_screen_width = layer_data.screen_width;
    display_info->max_screen_height = layer_data.screen_height;

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_ALPHA) {
        g_drv_ops.hifb_drv_set_layer_alpha(layer_id, par->alpha);
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_ALPHA;
    }

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_COLORKEY) {
        g_drv_ops.hifb_drv_set_layer_key_mask(layer_id, &par->ckey);
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_COLORKEY;
    }

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_FMT) {
        if ((refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) &&
            refresh_info->user_buffer.stCanvas.u64PhyAddr) {
            g_drv_ops.hifb_drv_set_layer_data_fmt(layer_id, refresh_info->user_buffer.stCanvas.enFmt);
        } else {
            g_drv_ops.hifb_drv_set_layer_data_fmt(layer_id, par->color_format);
        }
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_FMT;
    }
    return HI_SUCCESS;
}

static hi_void callback_modify_dynamic_range(hifb_par *par, hi_u32 layer_id)
{
    hifb_display_info *display_info = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;
    struct fb_info *info = HI_NULL;
    HIFB_DEFLICKER_S deflicker;

    display_info = &par->display_info;
    refresh_info = &par->refresh_info;
    info = g_layer[layer_id].info;
    /* NEW modify graphic dynamic range */
    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_DYNAMICRANGE) {
        /* Modified to HDR10 */
        if (display_info->dynamic_range == HIFB_DYNAMIC_RANGE_HDR10) {
            /* HIFB_DRV_EnableGHDR */
            if (g_drv_ops.hifb_drv_enable_ghdr) g_drv_ops.hifb_drv_enable_ghdr(layer_id, HI_TRUE);
        } else if (display_info->dynamic_range == HIFB_DYNAMIC_RANGE_SDR8 ||
                   display_info->dynamic_range == HIFB_DYNAMIC_RANGE_SDR10) {
            /* See HIFB_DRV_EnableGHDR */
            if (g_drv_ops.hifb_drv_enable_ghdr) g_drv_ops.hifb_drv_enable_ghdr(layer_id, HI_FALSE);
        }
        /*
         * After the modification is completed, the mask is cleared, just like the interrupt
         * processing mechanism. When the interrupt is processed, the interrupt is cleared.
         */
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_DYNAMICRANGE;
    }

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_BMUL) {
        if (g_drv_ops.hifb_drv_set_pre_mul(layer_id, display_info->premul) == HI_FAILURE) {
            if (par->display_info.premul == HI_TRUE) {
                par->display_info.premul = HI_FALSE;
            } else {
                par->display_info.premul = HI_TRUE;
            }
        }
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_BMUL;
    }

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_STRIDE) {
        if ((refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) &&
            refresh_info->user_buffer.stCanvas.u64PhyAddr) {
            g_drv_ops.hifb_drv_set_layer_stride(layer_id, refresh_info->user_buffer.stCanvas.u32Pitch);
        } else {
            g_drv_ops.hifb_drv_set_layer_stride(layer_id, hifb_get_line_length(info));
        }
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_STRIDE;
    }

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL) {
        deflicker.pu8HDfCoef = par->h_dfcoef;
        deflicker.pu8VDfCoef = par->v_dfcoef;
        deflicker.u32HDfLevel = par->h_dflevel;
        deflicker.u32VDfLevel = par->v_dflevel;
        g_drv_ops.hifb_drv_set_layer_de_flicker(layer_id, deflicker);
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL;
    }
    return;
}

static hi_void callback_init_rect(HIFB_RECT *in_rect, HIFB_RECT *out_rect,
                                  hifb_display_info *display_info)
{
    in_rect->x = display_info->pos.s32XPos;
    in_rect->y = display_info->pos.s32YPos;
    in_rect->w = (hi_s32)display_info->display_width;
    in_rect->h = (hi_s32)display_info->display_height;

    out_rect->x = display_info->pos.s32XPos;
    out_rect->y = display_info->pos.s32YPos;
    out_rect->w = display_info->screen_width;
    out_rect->h = display_info->screen_height;

    if ((hi_u32)out_rect->x + out_rect->w > display_info->max_screen_width) {
        out_rect->w = (hi_s32)(display_info->max_screen_width - out_rect->x);
    }

    if ((hi_u32)out_rect->y + out_rect->h > (hi_s32)display_info->max_screen_height) {
        out_rect->h = (hi_s32)(display_info->max_screen_height - out_rect->y);
    }

    /*
     * after cut off, the input rectangle keep rate with output rectangle
     * Prevent the occurrence of the divide-by-zero error
     */
    if ((display_info->screen_width != 0) && (display_info->screen_height != 0)) {
        if (((hi_s32)display_info->screen_width == 0) || ((hi_s32)display_info->screen_height == 0)) {
            return;
        }
        in_rect->w = in_rect->w * out_rect->w / (hi_s32)display_info->screen_width;
        in_rect->h = in_rect->h * out_rect->h / (hi_s32)display_info->screen_height;
    }
    return;
}

static hi_void callback_modify_sizes(hifb_par *par, hi_u32 layer_id)
{
    hifb_display_info *display_info = HI_NULL;
    HIFB_RECT in_rect = {0};
    HIFB_RECT out_rect = {0};
    hi_bool zme_en = HI_FALSE;

    display_info = &par->display_info;
    /* Handles requests to modify input and output sizes. */
    if ((par->param_modify_mask & HIFB_LAYER_PARAMODIFY_INRECT) ||
        (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_OUTRECT)) {
        /* if VO can't support scaling,  we set the screen's H/W as the same as display */
        if (!g_drv_ops.capability[layer_id].bVoScale) {
            display_info->screen_width = display_info->display_width;
            display_info->screen_height = display_info->display_height;
        }

        /* for init rect */
        callback_init_rect(&in_rect, &out_rect, display_info);

        if (hifb_check_imagezoomenable(layer_id, &in_rect, &out_rect) == HI_TRUE) {
            /*
             * If you want to go through the zoom module, you need to correct it to 2 alignment,
             * otherwise it will appear abnormal.
             */
            in_rect.w = ALIGN_DOWN(in_rect.w, 2); /* 2 size data */
            in_rect.h = ALIGN_DOWN(in_rect.h, 2); /* 2 size data */

            if (g_drv_ops.hifb_drv_set_layer_rect) {
                g_drv_ops.hifb_drv_set_layer_rect(layer_id, &in_rect, &out_rect);
            }

            if (g_drv_ops.hifb_drv_set_layer_src_image_reso) {
                g_drv_ops.hifb_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }

            /* See out width and height not equal to in width and height,then enable zme. */
            zme_en = (out_rect.w != in_rect.w || out_rect.h != in_rect.h);
            if (g_drv_ops.hifb_drv_enable_zme) {
                g_drv_ops.hifb_drv_enable_zme(layer_id, &in_rect, &out_rect, zme_en);
            }
        } else {
            /*
             * If scaling is not enabled,
             * the input size is used as the output size and the zoom module is closed.
             */
            if (g_drv_ops.hifb_drv_set_layer_rect) {
                g_drv_ops.hifb_drv_set_layer_rect(layer_id, &in_rect, &in_rect);
            }

            if (g_drv_ops.hifb_drv_set_layer_src_image_reso) {
                g_drv_ops.hifb_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }

            if (g_drv_ops.hifb_drv_enable_zme) {
                g_drv_ops.hifb_drv_enable_zme(layer_id, &in_rect, &out_rect, HI_FALSE);
            }
        }

        /* Processing completed, clear mask */
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_INRECT;
        par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_OUTRECT;
    }
    return;
}

static hi_void callback_set_fmt_and_stride(hifb_par *par, hi_u32 layer_id)
{
    struct fb_info *info = HI_NULL;
    hifb_refresh_info *refresh_info = HI_NULL;

    info = g_layer[layer_id].info;
    refresh_info = &par->refresh_info;
    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_FMT) {
        if ((refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) &&
            refresh_info->user_buffer.stCanvas.u64PhyAddr) {
            g_drv_ops.hifb_drv_set_layer_data_fmt(layer_id, refresh_info->user_buffer.stCanvas.enFmt);
        } else {
            g_drv_ops.hifb_drv_set_layer_data_fmt(layer_id, par->color_format);
        }
    }

    if (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_STRIDE) {
        if ((refresh_info->buf_mode == HIFB_LAYER_BUF_NONE) &&
            refresh_info->user_buffer.stCanvas.u64PhyAddr) {
            g_drv_ops.hifb_drv_set_layer_stride(layer_id, refresh_info->user_buffer.stCanvas.u32Pitch);
        } else {
            g_drv_ops.hifb_drv_set_layer_stride(layer_id, hifb_get_line_length(info));
        }
    }
    return;
}

static hi_void callback_update_refresh_info(hifb_par *par, hi_u32 layer_id)
{
    hifb_refresh_info *refresh_info = HI_NULL;
    struct fb_info *info = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    hi_u32 index, buf_size;

    refresh_info = &par->refresh_info;
    index = refresh_info->disp_buf_info.index_for_int;
    info = g_layer[layer_id].info;
    buf_size = ((hifb_get_line_length(info) * hifb_get_yres(info)) + HIFB_ALIGNMENT) & (~HIFB_ALIGNMENT);
    compress_info = &par->compress_info;
    if ((refresh_info->buf_mode == HIFB_LAYER_BUF_DOUBLE) &&
        (refresh_info->disp_buf_info.need_flip == HI_TRUE)) {
        /* Work buf to change to free buf. Take free buf to display */
        index = 1 - index;
        refresh_info->disp_buf_info.index_for_int = index;
        /*
         * The display address is set to the address of the free buf,
         * which is set to the screen buf address differently from 0buf
         */
        g_drv_ops.hifb_drv_set_layer_addr(layer_id, refresh_info->disp_buf_info.phy_addr[index]);
        refresh_info->screen_addr = refresh_info->disp_buf_info.phy_addr[index];
        refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 alg data */
        compress_info->layer_addr_update = HI_TRUE;
#ifdef __HuaweiLite__
        if (info->oinfo.stride != 0) {
            info->oinfo.sarea.y = (par->refresh_info.disp_buf_info.phy_addr[index] -
                                   (hi_u32)(uintptr_t)info->oinfo.fbmem) / info->oinfo.stride;
            if ((info->oinfo.bpp >> 3) != 0) { /* 3 is 8 bits */
                info->oinfo.sarea.x = ((par->refresh_info.disp_buf_info.phy_addr[index] -
                                       (hi_u32)(uintptr_t)info->oinfo.fbmem) % info->oinfo.stride) /
                                       (info->oinfo.bpp >> 3); /* 3 is 8 bits */
            }
        }
#else
        info->var.yoffset = osal_div_u64((refresh_info->disp_buf_info.phy_addr[index] -
                                         hifb_get_smem_start(info)), hifb_get_line_length(info));
        if ((hifb_get_line_length(info) != 0) && ((hifb_get_bits_per_pixel(info)>> 3) != 0)) { /* 3 is 8 bits */
            info->var.xoffset = ((hi_ulong)(refresh_info->disp_buf_info.phy_addr[index] -
                                            hifb_get_smem_start(info)) % hifb_get_line_length(info)) /
                                            (hifb_get_bits_per_pixel(info) >> 3); /* 3 is 8 bits */
        }
#endif
        refresh_info->disp_buf_info.fliped = HI_TRUE;
        refresh_info->disp_buf_info.need_flip = HI_FALSE;
        refresh_info->disp_buf_info.int_pic_num++;
    }
    return;
}

static hi_void callback_modify_address(hifb_par *par, hi_u32 layer_id)
{
    hifb_refresh_info *refresh_info = HI_NULL;
    hifb_display_info *display_info = HI_NULL;
    volatile hifb_compress_info *compress_info = HI_NULL;
    struct fb_info *info = HI_NULL;
    HIFB_RECT in_rect = {0};
    HIFB_RECT out_rect = {0};

    refresh_info = &par->refresh_info;
    display_info = &par->display_info;
    compress_info = &par->compress_info;
    info = g_layer[layer_id].info;
    /* The display address is refreshed and the display address is modified. */
    if (!(par->param_modify_mask & HIFB_LAYER_PARAMODIFY_DISPLAYADDR)) {
        /* according to the index, decide which buf set to the screen */
        callback_update_refresh_info(par, layer_id);
        return;
    }
    /* SetLayerDataFmt and  SetLayerStride */
    callback_set_fmt_and_stride(par, layer_id);

    if ((par->param_modify_mask & HIFB_LAYER_PARAMODIFY_INRECT) ||
        (par->param_modify_mask & HIFB_LAYER_PARAMODIFY_OUTRECT)) {
        /* if VO can't support scaling,  we set the screen's H/W as the same as display */
        if (!g_drv_ops.capability[layer_id].bVoScale) {
           display_info->screen_width = display_info->display_width;
           display_info->screen_height = display_info->display_height;
        }

        /* for init rect */
        callback_init_rect(&in_rect, &out_rect, display_info);

        if (hifb_check_imagezoomenable(layer_id, &in_rect, &out_rect) == HI_TRUE) {
            g_drv_ops.hifb_drv_set_layer_rect(layer_id, &in_rect, &out_rect);
            if (g_drv_ops.hifb_drv_set_layer_src_image_reso) {
                g_drv_ops.hifb_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }
        }
    }

    g_drv_ops.hifb_drv_set_layer_addr(layer_id, refresh_info->screen_addr);

    par->param_modify_mask &= ~HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    compress_info->layer_addr_update = HI_TRUE;

    if ((refresh_info->disp_buf_info.phy_addr[0] != refresh_info->disp_buf_info.phy_addr[1]) &&
        (refresh_info->disp_buf_info.phy_addr[0])) {
        if (refresh_info->screen_addr >=  refresh_info->disp_buf_info.phy_addr[0] &&
            refresh_info->screen_addr < refresh_info->disp_buf_info.phy_addr[1]) {
            refresh_info->disp_buf_info.index_for_int = 0;
        } else if ((refresh_info->screen_addr >= refresh_info->disp_buf_info.phy_addr[1]) &&
                   (refresh_info->screen_addr < (refresh_info->disp_buf_info.phy_addr[0] +
                   hifb_get_smem_len(info)))) {
            refresh_info->disp_buf_info.index_for_int = 1;
        }
    }
    /* according to the index, decide which buf set to the screen */
    callback_update_refresh_info(par, layer_id);
    return;
}

static hi_s32 vo_callback_process(hifb_par *par, hi_u32 layer_id, hi_bool *is_continue)
{
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    /* If not displayed, close the graphics layer and exit */
    if (par->show == HI_FALSE) {
        if (g_drv_ops.hifb_drv_enable_layer) {
            g_drv_ops.hifb_drv_enable_layer(layer_id, HI_FALSE);
        }
        if (g_drv_ops.hifb_drv_updata_layer_reg) {
            g_drv_ops.hifb_drv_updata_layer_reg(layer_id);
        }
        *is_continue = HI_FALSE;
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
        return HI_SUCCESS;
    }
    if (layer_id == 0 || layer_id == 1) {
#ifndef __HuaweiLite__
        hifb_vsync_notify(); /* vsync signal */
#endif
    }

    /* Non-modified status, modified, can be modified */
    if (!par->modifying) {
        /*
         * 1.Get osd data
         * 2.Set layer alpha
         * 3.Set layer keymask
         * 4.Set layer data fmt
         */
        if (callback_get_osd_data(par, layer_id) != HI_SUCCESS) {
            hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
            return HI_FAILURE;
        }

        /*
         * 1.NEW modify graphic dynamic range
         * 2.Set premul
         * 3.Set layer stride
         * 4.Set layer deflicker
         */
        callback_modify_dynamic_range(par, layer_id);

        /*
         * Handles requests to modify input and output sizes
         * Set layer rect
         * Set layer srcImage reso
         * if enable ZME or not
         */
        callback_modify_sizes(par, layer_id);

        /*
         * The display address is refreshed and the display address is modified
         * set layer data fmt
         * set layer stride
         * set layer rect
         * set layer addr
         */
        callback_modify_address(par, layer_id);

        /* HIFB_DRV_EnableLayer */
        g_drv_ops.hifb_drv_enable_layer(layer_id, par->show);
    }
    /* Decompression configuration */
    hifb_dcmp_config(layer_id);

    /* HIFB_DRV_UpdataLayerReg */
    g_drv_ops.hifb_drv_updata_layer_reg(layer_id);
    *is_continue = HI_TRUE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

#ifndef CONFIG_HI_VO_FB_SEPARATE
static hi_s32 vo_callback_unseparate(hifb_par *par, const hi_void *paramr, hi_bool *is_continue)
{
    hi_u32 vtthd_flag;
    hi_u32 frm_start_flag;
    hi_u32 int_status;

    if (paramr == HI_NULL) {
        hifb_error("HI_NULL pointer !\n");
        return HI_FAILURE;
    }
    int_status = *(hi_u32 *)paramr;
    vtthd_flag = HIFB_INTMSK_HD0_VTTHD1;
    frm_start_flag = HIFB_INTMSK_HD0_VTTHD3;

    if (int_status & frm_start_flag) {
        par->refresh_info.do_refresh_job = HI_TRUE;
        wake_up_interruptible(&(par->do_refresh_job));
        *is_continue = HI_FALSE;
        return HI_SUCCESS;
    } else if (int_status & vtthd_flag) {
        par->refresh_info.do_refresh_job = HI_FALSE;
    }
    *is_continue = HI_TRUE;
    return HI_SUCCESS;
}
#endif

/* Callback function for VO vertical timing interrupt */
static hi_s32 hifb_vo_callback(const hi_void *paraml, const hi_void *paramr)
{
    hi_u32 *layer_id = HI_NULL;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    hi_bool is_continue = HI_FALSE;

    if (paraml == HI_NULL) {
        return HI_FAILURE;
    }
    layer_id = (hi_u32 *)paraml;
    if (*layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    info = g_layer[*layer_id].info;
    if (info == HI_NULL) {
        return HI_FAILURE;
    }
    if (info->par == HI_NULL) {
        return HI_FAILURE;
    }

    par = (hifb_par *)(info->par);
#ifndef CONFIG_HI_VO_FB_SEPARATE
    if (vo_callback_unseparate(par, paramr, &is_continue) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    if (is_continue != HI_TRUE) {
        return HI_SUCCESS;
    }
#endif

    if (vo_callback_process(par, *layer_id, &is_continue) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    if (is_continue != HI_TRUE) {
        return HI_SUCCESS;
    }

    /* Field blanking mark */
    par->vblflag = 1;
    wake_up_interruptible(&(par->vbl_event));

    return HI_SUCCESS;
}

/* Compression completion interrupt callback function */
static hi_s32 hifb_wbc_callback(const hi_void *paraml, const hi_void *paramr)
{
    hi_u32 *layer_id = HI_NULL;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    unsigned long lockflag;
    hi_bool wbc_stop = HI_FALSE;
    if (paraml == HI_NULL) {
        return HI_FAILURE;
    }
    layer_id = (hi_u32 *)paraml;
    if (*layer_id >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }
    info = g_layer[*layer_id].info;
    par = (hifb_par *)(info->par);

    hifb_spin_lock_irqsave(&par->lock, lockflag);

    g_drv_ops.hifb_drv_get_wbc_stop_state(*layer_id, &wbc_stop);
    if (wbc_stop) {
        hifb_dbg_info("layer %u wbc autostop!\n", *layer_id);
        hifb_spin_unlock_irqrestore(&par->lock, lockflag);
        return HI_SUCCESS;
    }

    /* close compression */
    g_drv_ops.hifb_drv_enable_wbc(*layer_id, HI_FALSE);

    /* Switch to compressed g_display */
    hifb_set_dcmp_info(par);
    g_drv_ops.hifb_drv_enable_dcmp(*layer_id, HI_TRUE);
    g_drv_ops.hifb_drv_updata_layer_reg(*layer_id);

    /* Clear compression area and compression flag */
    osal_memset((void *)&par->compress_info.compress_rect, 0, sizeof(HIFB_RECT));

    hifb_spin_unlock_irqrestore(&par->lock, lockflag);

    return HI_SUCCESS;
}

static hi_s32 hifb_refresh_0buf(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;
    unsigned long lockflag;
    hi_s32 ret = hifb_disp_setdispsize(layer_id, canvas_buf->stCanvas.u32Width, canvas_buf->stCanvas.u32Height);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    hifb_spin_lock_irqsave(&par->lock, lockflag);

    if (compress_info->compress_open) {
        hifb_error("only HIFB_LAYER_BUF_DOUBLE or HIFB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
        hifb_spin_unlock_irqrestore(&par->lock, lockflag);
        return HI_FAILURE;
    }

    refresh_info->disp_buf_info.compress = HI_FALSE;

    par->modifying = HI_TRUE;
    /* modify by wxl : if change flush type between 2buffer and 0 buffer, the addr couldn't be changed. */
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    /*
     * The graphic address is taken from the canvas of the user data and
     * filled in the screen address of the refresh information.
     */
    refresh_info->screen_addr = canvas_buf->stCanvas.u64PhyAddr;

    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    refresh_info->user_buffer.stCanvas.u32Pitch = canvas_buf->stCanvas.u32Pitch;

    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_FMT;
    refresh_info->user_buffer.stCanvas.enFmt = canvas_buf->stCanvas.enFmt;

    /* NEW */
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_DYNAMICRANGE;
    refresh_info->user_buffer.stCanvas.enDynamicRange = canvas_buf->stCanvas.enDynamicRange;

    /* NEW feature: zme */
    par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    hifb_spin_unlock_irqrestore(&par->lock, lockflag);
    /*
     * In hifb_disp_setdispsize, it is possible that kmalloc
     * allocates memory in a non-atomic manner, so the lock must be released first.
     */
    hifb_spin_lock_irqsave(&par->lock, lockflag);
    par->modifying = HI_FALSE;
    osal_memcpy(&(refresh_info->user_buffer), canvas_buf, sizeof(HIFB_BUFFER_S));

    if (par->compress_info.compress_open) {
        osal_memcpy((void *)&par->compress_info.update_rect, &canvas_buf->UpdateRect, sizeof(HIFB_RECT));
        par->compress_info.update_finished = HI_TRUE;
    }

    par->vblflag = 0;
    hifb_spin_unlock_irqrestore(&par->lock, lockflag);

    /* if the flag "FB_ACTIVATE_VBL" has been set, we should wait for register update finish */
#ifdef __HuaweiLite__
    if (!in_atomic()) {
#else
    if (!in_atomic() && (info->var.activate & FB_ACTIVATE_VBL)) {
#endif
        hifb_wait_regconfig_work(layer_id);
    }

    return HI_SUCCESS;
}

static hi_void refresh_is_cursor_overlay(hifb_par *par, HIFB_BUFFER_S *canvas_buf, hi_bool *is_overlay)
{
    hifb_par *cursor_par = HI_NULL;
    hifb_cursor_info *cursor_info = &par->cursor_info;
    HIFB_RECT rc_cursor;
    cursor_par = (hifb_par *)g_layer[par->cursor_info.attached_cursor_id].info->par;
    if (cursor_info->attched != 0) {
        rc_cursor.x = cursor_par->display_info.pos.s32XPos -
                 cursor_par->cursor_info.cursor.stHotPos.s32XPos;
        rc_cursor.y = cursor_par->display_info.pos.s32YPos -
                 cursor_par->cursor_info.cursor.stHotPos.s32YPos;
        rc_cursor.w = cursor_par->cursor_info.cursor.stCursor.u32Width;
        rc_cursor.h = cursor_par->cursor_info.cursor.stCursor.u32Height;

        /* check the cusor overlay with refresh area */
        if (cursor_par->show &&
            (((rc_cursor.x >= canvas_buf->UpdateRect.x &&
              rc_cursor.x <=  canvas_buf->UpdateRect.x + canvas_buf->UpdateRect.w)) ||
             (rc_cursor.x < canvas_buf->UpdateRect.x &&
              rc_cursor.x + rc_cursor.w >=  canvas_buf->UpdateRect.x))) {
            if (((rc_cursor.y >= canvas_buf->UpdateRect.y &&
                  rc_cursor.y <=  canvas_buf->UpdateRect.y + canvas_buf->UpdateRect.h)) ||
                (rc_cursor.y < canvas_buf->UpdateRect.y &&
                 rc_cursor.y + rc_cursor.h >=  canvas_buf->UpdateRect.y)) {
                *is_overlay = HI_TRUE;
            }
        }
    }
    return;
}

/*
 * Name : hifb_refresh
 * Desc : It is refreshed according to the canvas information and the layer's buf refresh mode.
 *        It is called indirectly when setting the layer attr.
 * See  : references hifb_refresh_again,hifb_onrefresh
 *        calls hifb_refresh_0buf,hifb_refresh_1buf,hifb_refresh_2buf
 */
static hi_s32 hifb_refresh(hi_u32 layer_id, HIFB_BUFFER_S *canvas_buf, HIFB_LAYER_BUF_E buf_mode)
{
    hi_s32 ret = HI_FAILURE;
    hifb_par *par = (hifb_par *)g_layer[layer_id].info->par;
    hi_bool is_overlay = HI_FALSE; /* is the cusor overlay with refresh area */

    if (canvas_buf == HI_NULL) {
        return HI_FAILURE;
    }

    /*
     * For cursor layer
     * you can quary whether cursor attach to a certain layer for general layer
     * when bAttched not zero, we should check whether the cusor overlay with refresh area or not
     */
    refresh_is_cursor_overlay(par, canvas_buf, &is_overlay);

#ifdef CURSOR
    if (is_overlay && is_soft_cursor()) {
        hifb_cursor_hide(layer_id);
    }
#endif
    switch (buf_mode) {
        case HIFB_LAYER_BUF_DOUBLE:
            ret = hifb_refresh_2buf(layer_id, canvas_buf);
            break;
        case HIFB_LAYER_BUF_ONE:
            ret = hifb_refresh_1buf(layer_id, canvas_buf);
            break;
        case HIFB_LAYER_BUF_NONE:
            ret = hifb_refresh_0buf(layer_id, canvas_buf);
            break;
        case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
            ret = hifb_refresh_2buf_immediate_display(layer_id, canvas_buf);
            break;
        default:
            break;
    }
#ifdef CURSOR
    if (is_overlay && is_soft_cursor()) {
        hifb_cursor_show(layer_id);
    }
#endif
    return ret;
}

#ifdef __HuaweiLite__
static void hifb_free(struct hifb_info *info)
{
    if (info != HI_NULL) {
        free(info);
    }
}

static struct hifb_info *hifb_alloc(size_t size)
{
    hi_s32 hifb_info_len = sizeof(struct hifb_info);
    struct hifb_info *info = HI_NULL;

    hifb_info_len = ALIGN(hifb_info_len, sizeof(unsigned long));

    info = (struct hifb_info *)malloc(hifb_info_len + size);
    if (info == HI_NULL)
        return HI_NULL;
    osal_memset(info, 0, (hifb_info_len + size));

    info->par = (void *)((char *)info + hifb_info_len);

    return info;
}
#endif

#ifdef __HuaweiLite__
static hi_s32 alloc_new_canvas_buffer(struct fb_info *info, HIFB_LAYER_INFO_S *layer_info)
{
    hifb_par *par = (hifb_par *)info->par;
    HIFB_SURFACE_S *canvas_surface = &par->canvas_sur;
    hi_u32 layer_size;
    hi_u32 pitch;
    hi_char *buf = HI_NULL;
    hi_char name[16]; /* 16 max lenght */
    struct fb_info *info_temp = HI_NULL;
    /*
     * Applying fb_info without private data is not for registration,
     * but for the convenience of passing to hifb_completely to empty the memory.
     */
    info_temp = hifb_alloc(0);
    if (info_temp == HI_NULL) {
        return HI_FAILURE;
    }

    /* 16 bytes alignment */
    pitch = (((layer_info->u32CanvasWidth * hifb_get_bits_per_pixel(info)) >> 3) + 15) >> 4; /* 3 4 15 alg data */
    pitch = pitch << 4; /* 4 alg data */

    layer_size = pitch * layer_info->u32CanvasHeight;
    /* alloc new buffer */
    osal_snprintf(name, sizeof(name), "hifb_canvas%u", par->layer_id);
    /* 16 size buffer */
    canvas_surface->u64PhyAddr = hifb_buf_allocmem(name, 16, layer_size, hifb_get_layer_mmz_names(par->layer_id));
    if (canvas_surface->u64PhyAddr == 0) {
        hifb_error("alloc canvas buffer no mem");
        hifb_free(info_temp);
        return HI_FAILURE;
    }

    buf = (hi_char *)hifb_buf_map(canvas_surface->u64PhyAddr, layer_size);
    if (buf == HI_NULL) {
        hifb_error("map canvas buffer failed!\n");
        hifb_buf_freemem(canvas_surface->u64PhyAddr);
        hifb_free(info_temp);
        return HI_FAILURE;
    }
    /* hifb_completely is faster than memset */
    info_temp->oinfo.fbmem = (uintptr_t)canvas_surface->u64PhyAddr;
    osal_memset(info_temp->oinfo.fbmem, 0x0, layer_size);

    hifb_buf_ummap(buf);

    hifb_dbg_info("alloc new memory for canvas buffer success\n");
    canvas_surface->u32Width = layer_info->u32CanvasWidth;
    canvas_surface->u32Height = layer_info->u32CanvasHeight;
    canvas_surface->u32Pitch = pitch;
    par->canvas_sur.enFmt =  info->vinfo.fmt;
    hifb_free(info_temp);

    return HI_SUCCESS;
}
#else
static hi_s32 alloc_new_canvas_buffer(struct fb_info *info, HIFB_LAYER_INFO_S *layer_info)
{
    hifb_par *par = (hifb_par *)info->par;
    HIFB_SURFACE_S *canvas_surface = &par->canvas_sur;
    hi_u32 layer_size;
    hi_u32 pitch;
    hi_char *buf = HI_NULL;
    hi_char name[16]; /* 16 max lenght */
    struct fb_info *info_temp = HI_NULL;
    /*
     * Applying fb_info without private data is not for registration,
     * but for the convenience of passing to hifb_completely to empty the memory.
     */
    info_temp = framebuffer_alloc(0, HI_NULL);
    if (HI_NULL == info_temp) {
        return HI_FAILURE;
    }

    /* 16 bytes alignment */
    pitch = (((layer_info->u32CanvasWidth * hifb_get_bits_per_pixel(info)) >> 3) + 15) >> 4; /* 3 4 15 alg data */
    pitch = pitch << 4; /* 4 alg data */

    layer_size = pitch * layer_info->u32CanvasHeight;
    /* alloc new buffer */
    osal_snprintf(name, sizeof(name), "hifb_canvas%u", par->layer_id);
    /* 16 size buffer */
    canvas_surface->u64PhyAddr = hifb_buf_allocmem(name, 16, layer_size, hifb_get_layer_mmz_names(par->layer_id));
    if (canvas_surface->u64PhyAddr == 0) {
        hifb_error("alloc canvas buffer no mem");
        framebuffer_release(info_temp);
        return HI_FAILURE;
    }

    buf = (hi_char *)hifb_buf_map(canvas_surface->u64PhyAddr, layer_size);
    if (buf == HI_NULL) {
        hifb_error("map canvas buffer failed!\n");
        hifb_buf_freemem(canvas_surface->u64PhyAddr);
        framebuffer_release(info_temp);
        return HI_FAILURE;
    }
    /* hifb_completely is faster than memset */
    info_temp->fix.smem_start = canvas_surface->u64PhyAddr;
    info_temp->screen_base = buf;
    if (hifb_completely(info_temp, 0, layer_size) != HI_SUCCESS) {
        hifb_error("hifb_memset failed!\n");
    }
    hifb_buf_ummap(buf);

    hifb_dbg_info("alloc new memory for canvas buffer success\n");
    canvas_surface->u32Width = layer_info->u32CanvasWidth;
    canvas_surface->u32Height = layer_info->u32CanvasHeight;
    canvas_surface->u32Pitch = pitch;
    canvas_surface->enFmt = hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue,
                                              &info->var.transp, info->var.bits_per_pixel);
    framebuffer_release(info_temp);
    return HI_SUCCESS;
}
#endif

static hi_s32 hifb_alloccanbuf(struct fb_info *info, HIFB_LAYER_INFO_S *layer_info)
{
    hifb_par *par = HI_NULL;
    HIFB_SURFACE_S *canvas_surface = HI_NULL;

    if ((info == HI_NULL) || (layer_info == HI_NULL)) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;
    canvas_surface = &par->canvas_sur;

    if (!(layer_info->u32Mask & HIFB_LAYERMASK_CANVASSIZE)) {
        return HI_SUCCESS;
    }

    /* if  with old canvas buffer */
    if (canvas_surface->u64PhyAddr != 0) {
        /* if old is the same with new , then return, else free the old buffer */
        if ((layer_info->u32CanvasWidth == canvas_surface->u32Width) &&
            (layer_info->u32CanvasHeight == canvas_surface->u32Height)) {
            hifb_dbg_info("mem size is the same , no need alloc new memory");
            return HI_SUCCESS;
        }

        /* free new old buffer */
        hifb_dbg_info("free old canvas buffer\n");
        hifb_freeccanbuf(par);
    }
    if (layer_info->u32CanvasWidth > HIFB_4K_DEF_WIDTH || layer_info->u32CanvasHeight > HIFB_4K_DEF_HEIGHT) {
        hifb_dbg_info("unsupport too large w(%d) and h(%d)\n", layer_info->u32CanvasWidth, layer_info->u32CanvasHeight);
        return HI_FAILURE;
    }

    /* new canvas buffer */
    if (alloc_new_canvas_buffer(info, layer_info) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 onrefresh_check_param(hifb_par* par, HIFB_BUFFER_S *canvas_buf, HIFB_LAYER_BUF_E buf_mode)
{
    if (canvas_buf->stCanvas.enFmt >= HIFB_FMT_BUTT) {
        return HI_FAILURE;
    }

    /* Check the legality of physical address and size */
    if (hifb_check_phyaddr(canvas_buf) == HI_FAILURE) {
        hifb_error("mmz phy addr 0x%x invalid.\n", canvas_buf->stCanvas.enFmt);
        return HI_FAILURE;
    }

    if (buf_mode == HIFB_LAYER_BUF_BUTT) {
        hifb_error("doesn't support FBIO_REFRESH operation when refresh mode is HI_FB_LAYER_BUF_BUTT!\n");
        return HI_FAILURE;
    }

    if ((canvas_buf->UpdateRect.x >=  canvas_buf->stCanvas.u32Width) ||
        (canvas_buf->UpdateRect.y >= canvas_buf->stCanvas.u32Height) ||
        (canvas_buf->UpdateRect.w <= 0) || (canvas_buf->UpdateRect.h <= 0)) {
        hifb_error("rect error: update rect:(%d,%d,%d,%d), canvas range:(%d,%d)\n",
                   canvas_buf->UpdateRect.x, canvas_buf->UpdateRect.y,
                   canvas_buf->UpdateRect.w, canvas_buf->UpdateRect.h,
                   canvas_buf->stCanvas.u32Width, canvas_buf->stCanvas.u32Height);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 onrefresh_get_canvas_buf(hifb_par *par, hi_void __user *argp, HIFB_BUFFER_S *canvas_buf)
{
    unsigned long lock_flag;
    HIFB_LAYER_BUF_E buf_mode;
    if (copy_from_user(canvas_buf, argp, sizeof(HIFB_BUFFER_S))) {
        return -EFAULT;
    }

    hifb_get_bufmode(par, &buf_mode);
    /*
     * check canvas format
     * check canvas phyaddr
     * check buf_mode
     * check canvas update rect legality
     */
    if (onrefresh_check_param(par, canvas_buf, buf_mode) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /* update canvas update_rect */
    if (canvas_buf->UpdateRect.x + canvas_buf->UpdateRect.w > canvas_buf->stCanvas.u32Width) {
        canvas_buf->UpdateRect.w = canvas_buf->stCanvas.u32Width - canvas_buf->UpdateRect.x;
    }

    if (canvas_buf->UpdateRect.y + canvas_buf->UpdateRect.h > canvas_buf->stCanvas.u32Height) {
        canvas_buf->UpdateRect.h =  canvas_buf->stCanvas.u32Height - canvas_buf->UpdateRect.y;
    }
    if (buf_mode == HIFB_LAYER_BUF_NONE) {
        /* Check if the format of the canvas supported or not by HiFB */
        if ((par->layer_id >= HIFB_MAX_LAYER_NUM) || (canvas_buf->stCanvas.enFmt >= HIFB_FMT_BUTT)) {
            return HI_FAILURE;
        }

        if (!g_drv_ops.capability[par->layer_id].bColFmt[canvas_buf->stCanvas.enFmt]) {
            hifb_error("Unsupported PIXEL FORMAT!\n");
            return -EINVAL;
        }
        /*
         * there's a limit from hardware that the start address of screen buf
         * should be 16byte aligned.
         */
        hifb_spin_lock_irqsave(&par->lock, lock_flag);
        if ((canvas_buf->stCanvas.u64PhyAddr & 0xf) || (canvas_buf->stCanvas.u32Pitch & 0xf)) { /* 0xf mask */
            hifb_error("addr 0x%llx or pitch: 0x%x is not 16 bytes align !\n",
                       canvas_buf->stCanvas.u64PhyAddr, canvas_buf->stCanvas.u32Pitch);
            hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
            return HI_FAILURE;
        }
        hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    } else {
        /*
         * Check if the format of the canvas and the format of
         * HiFB belong to the category that TDE can support
         */
        if ((!hifb_tde_is_support_fmt(canvas_buf->stCanvas.enFmt)) ||
            (!hifb_tde_is_support_fmt(par->color_format))) {
            hifb_error("Unsupported PIXEL FORMAT!CanvasFmt:%d,par->enColFmt:%d\n", canvas_buf->stCanvas.enFmt,
                       par->color_format);
            return -EINVAL;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hifb_onrefresh(hifb_par* par, hi_void __user *argp)
{
    hi_s32 ret;
    HIFB_BUFFER_S canvas_buf;
    HIFB_LAYER_BUF_E buf_mode;
    hifb_display_info *diplay_info = &par->display_info;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        hifb_warning("you shouldn't refresh cursor layer!\n");
        return HI_SUCCESS;
    }

    if (argp == HI_NULL) {
        hifb_error("HI_NULL arg!\n");
        return -EINVAL;
    }

    /* get canvas buffer and check legality */
    ret = onrefresh_get_canvas_buf(par, argp, &canvas_buf);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    hifb_get_bufmode(par, &buf_mode);
    if ((buf_mode == HIFB_LAYER_BUF_NONE) && (diplay_info->mirror_mode != HIFB_MIRROR_NONE)) {
        hifb_error("Can't do mirror when the layer buf is none!\n");
        return -EINVAL;
    }

    if ((buf_mode == HIFB_LAYER_BUF_NONE) && (diplay_info->rotate_mode != HIFB_ROTATE_NONE)) {
        hifb_error("Can't do rotate when the layer buf is none!\n");
        return -EINVAL;
    }

    ret = hifb_refresh(par->layer_id, &canvas_buf, buf_mode);

    return ret;
}

static hi_s32 hifb_set_mirrormode(hifb_par *par, HIFB_MIRROR_MODE_E mirror_mode)
{
    hifb_display_info *display_info = &par->display_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;
    HIFB_LAYER_BUF_E buf_mode;

    hifb_get_bufmode(par, &buf_mode);

    if ((buf_mode == HIFB_LAYER_BUF_BUTT) || (buf_mode == HIFB_LAYER_BUF_NONE)) {
        hifb_error("doesn't support FBIOPUT_MIRROR_MODE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }

    if (mirror_mode >= HIFB_MIRROR_BUTT) {
        hifb_error("The input mirror mode is wrong!\n");
        return -EINVAL;
    }

    if ((mirror_mode != HIFB_MIRROR_NONE) && (display_info->rotate_mode != HIFB_ROTATE_NONE)) {
        hifb_error("Can't do mirror when rotate!\n");
        return -EINVAL;
    }

    if ((mirror_mode != HIFB_MIRROR_NONE) && (compress_info->compress_open == HI_TRUE)) {
        hifb_error("Can't do mirror when compression is on!\n");
        return -EINVAL;
    }

    display_info->mirror_mode = mirror_mode;
    return HI_SUCCESS;
}

static hi_s32 rotate_vb_init(struct fb_info *info)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    hi_void *mmz_name = HI_NULL;
    hi_u32 size;
    hi_char name[HIFB_ROTBUF_NAME_LEN];
    hi_u64 *vir_rotate_vb = HI_NULL;
    hi_mpp_chn chn;
    chn.mod_id  = HI_ID_FB;
    chn.dev_id = par->layer_id;
    chn.chn_id = 0;

    size = hifb_get_line_length(info) * (((display_info->display_height + 15) / 16) * 16); /* 15 16 align */

    if (func_entry(sys_export_func, HI_ID_SYS)->pfn_get_mmz_name(&chn, &mmz_name)) {
        hifb_error("get mmz name fail!\n");
        return ENOMEM;
    }

    osal_snprintf(name, sizeof(name), "hifb%d_rotate", par->layer_id);
    par->rotate_vb = cmpi_mmz_malloc(mmz_name, name, size);
    if (par->rotate_vb == 0) {
        hifb_error("get buffer fail,size %d !\n", size);
        return ENOMEM;
    }

    vir_rotate_vb = cmpi_remap_nocache(par->rotate_vb, size);
    if (vir_rotate_vb == HI_NULL) {
        hifb_error("get buffer fail,size %d !\n", size);
        cmpi_mmz_free(par->rotate_vb, HI_NULL);
        return ENOMEM;
    }

    osal_memset(vir_rotate_vb, 0x0, size);

    cmpi_unmap(vir_rotate_vb);
    return HI_SUCCESS;
}

static hi_s32 hifb_set_rotatemode(struct fb_info *info, HIFB_ROTATE_MODE_E rotate_mode)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    HIFB_LAYER_BUF_E buf_mode;
    volatile hifb_compress_info *compress_info = &par->compress_info;

    if (!hifb_drv_support_rotate()) {
        hifb_error("can't support rotate\n!\n");
        return -EINVAL;
    }
    hifb_get_bufmode(par, &buf_mode);

    if ((buf_mode == HIFB_LAYER_BUF_BUTT) || (buf_mode == HIFB_LAYER_BUF_NONE)) {
        hifb_error("doesn't support FBIOPUT_ROTATE_MODE operation when in standard mode or HIFB_LAYER_BUF_NONE!\n");
        return HI_FAILURE;
    }

    if ((par->color_format != HIFB_FMT_ARGB4444) && (par->color_format != HIFB_FMT_ARGB1555) &&
        (par->color_format != HIFB_FMT_ARGB8888)) {
        hifb_error("The rotate mode only support HIFB_FMT_ARGB4444 and HIFB_FMT_ARGB1555 which is %d\n!\n",
                   par->color_format);
        return -EINVAL;
    }

    if (rotate_mode >= HIFB_ROTATE_BUTT) {
        hifb_error("The input rotate mode is wrong!\n");
        return -EINVAL;
    }

    if (compress_info->compress_open && (rotate_mode != HIFB_ROTATE_NONE)) {
        hifb_error("Can't rotate when in compress mode!\n");
        return -EINVAL;
    }

    if ((rotate_mode != HIFB_ROTATE_NONE) && (display_info->mirror_mode != HIFB_MIRROR_NONE)) {
        hifb_error("Can't do rotate when mirror!\n");
        return -EINVAL;
    }

    if (((rotate_mode == HIFB_ROTATE_90) || (rotate_mode == HIFB_ROTATE_270)) && (par->rotate_vb == 0)) {
        if (rotate_vb_init(info) != HI_SUCCESS) {
            return ENOMEM;
        }
    }

    display_info->rotate_mode = rotate_mode;

    return HI_SUCCESS;
}

/*
 * Name : hifb_check_layerinfo
 * Desc : check layer information: buf refresh mode,position,canvas width
          and height, display width and height, screen width and height.
 */
static hi_s32 hifb_check_layerinfo(HIFB_LAYER_INFO_S *layer_info)
{
    if (layer_info == HI_NULL) {
        return HI_FAILURE;
    }

    if (layer_info->u32Mask & HIFB_LAYERMASK_BUFMODE) {
        if (layer_info->BufMode > HIFB_LAYER_BUF_DOUBLE_IMMEDIATE) {
            hifb_error("BufMode(%d) is error, should between %d and %d\n", layer_info->BufMode, HIFB_LAYER_BUF_DOUBLE,
                       HIFB_LAYER_BUF_DOUBLE_IMMEDIATE);
            return HI_FAILURE;
        }
    }

    /* Detection of anti-flicker mode */
    if (layer_info->u32Mask & HIFB_LAYERMASK_ANTIFLICKER_MODE) {
        if (layer_info->eAntiflickerLevel > HIFB_LAYER_ANTIFLICKER_AUTO) {
            hifb_error("eAntiflickerLevel(%d) is error, should between %d and %d\n", layer_info->eAntiflickerLevel,
                       HIFB_LAYER_ANTIFLICKER_NONE, HIFB_LAYER_ANTIFLICKER_AUTO);
            return HI_FAILURE;
        }
    }

    /* check the width and height */
    if (layer_info->u32Mask & HIFB_LAYERMASK_DISPSIZE) {
        if (layer_info->u32DisplayWidth % 2 || layer_info->u32DisplayHeight % 2) { /* 2 align to */
            hifb_error("Disaplay W(%u) and H(%u) should align to 2!\n", layer_info->u32DisplayWidth,
                       layer_info->u32DisplayHeight);
            return HI_FAILURE;
        }
    }

    if (layer_info->u32Mask & HIFB_LAYERMASK_SCREENSIZE) {
        if (layer_info->u32ScreenWidth % 2 || layer_info->u32ScreenHeight % 2) { /* 2 align to */
            hifb_error("Screenaplay W(%u) and H(%u) should align to 2!\n", layer_info->u32ScreenWidth,
                       layer_info->u32ScreenHeight);
            return HI_FAILURE;
        }
    }

    /* check pre-multiplier value. */
    if (layer_info->u32Mask & HIFB_LAYERMASK_BMUL) {
        if ((layer_info->bPreMul != HI_TRUE) && (layer_info->bPreMul != HI_FALSE)) {
            hifb_error("pstLayerInfo->bPreMul should be HI_TRUE or HI_FALSE but it is %d\n", layer_info->bPreMul);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 check_display_size(struct fb_info *info, hifb_par* par,
                                 HIFB_LAYER_INFO_S *layer_info, hi_bool is_interlace)
{
    hi_u32 pitch;
    if (!g_drv_ops.capability[par->layer_id].bVoScale) {
        /*
         * If the chip does not support scaling,
         * if both the display size and the screen size are set , both must be equal.
         */
        if ((layer_info->u32Mask & HIFB_LAYERMASK_DISPSIZE) &&
            (layer_info->u32Mask & HIFB_LAYERMASK_SCREENSIZE)) {
            if ((layer_info->u32DisplayWidth != layer_info->u32ScreenWidth) ||
                (layer_info->u32DisplayHeight != layer_info->u32ScreenHeight)) {
                hifb_error("DisplaySize(%d, %d) can't be different with ScreenSize(%d, %d) \
                           when layer %d don't scale\n", layer_info->u32DisplayWidth, layer_info->u32DisplayHeight,
                           layer_info->u32ScreenWidth, layer_info->u32ScreenHeight, par->layer_id);
                return HI_FAILURE;
            }
        }
    }

    /* Modify the display size, the memory size has changed, limited by the size of the memory */
    if (layer_info->u32Mask & HIFB_LAYERMASK_DISPSIZE) {
        pitch = (layer_info->u32DisplayWidth * hifb_get_bits_per_pixel(info)) >> 3; /* 3 for 8 bit */
        pitch = (pitch + 0xf) & 0xfffffff0; /* 0xf 0xfffffff0 mask */
        if (hifb_check_mem_enough(info, pitch, layer_info->u32DisplayHeight) == HI_FAILURE) {
            hifb_error("memory is not enough!\n");
            return HI_FAILURE;
        }

        if (layer_info->u32DisplayWidth == 0 || layer_info->u32DisplayHeight == 0) {
            hifb_error("display witdh/height shouldn't be 0!\n");
            return HI_FAILURE;
        }
        /*
         * For interlaced output, the height of the layer must be even.
         * Progressive output without this limit.
         */
        if (is_interlace && ((layer_info->u32DisplayHeight % 2) != 0)) { /* 2 for align */
            hifb_error("display_height(%d) of u32LayerId %d should be even when vodev output is interlace\n",
                       layer_info->u32DisplayHeight, par->layer_id);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 onputlayerinfo_check_size(struct fb_info *info, hifb_par* par,
                                        HIFB_LAYER_INFO_S *layer_info, hi_bool is_interlace)
{
    /* Check the display size */
    if (check_display_size(info, par, layer_info, is_interlace) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /* Check the canvas size */
    if (layer_info->u32Mask & HIFB_LAYERMASK_CANVASSIZE) {
       if ((layer_info->u32CanvasWidth == 0) || (layer_info->u32CanvasHeight == 0)) {
           hifb_error("canvas width/height shouldn't be 0\n");
           return HI_FAILURE;
       }
    }

    /* Check the screen size */
    if (layer_info->u32Mask & HIFB_LAYERMASK_SCREENSIZE) {
       if ((layer_info->u32CanvasWidth == 0) || (layer_info->u32CanvasHeight == 0)) {
           hifb_error("screen width/height shouldn't be 0\n");
           return HI_FAILURE;
       }
       /*
        * For interlaced output, the height of the layer must be even.
        * Progressive output without this limit.
        */
       if (is_interlace && ((layer_info->u32ScreenHeight % 2) != 0)) { /* 2 for align */
           hifb_error("screen_height(%d) of u32LayerId %d should be even when vodev output is interlace\n",
                      layer_info->u32ScreenHeight, par->layer_id);
           return HI_FAILURE;
       }
    }
    return HI_SUCCESS;
}

static hi_s32 onputlayerinfo_check_buf_mode(struct fb_info *info, hifb_par* par,
                                            HIFB_LAYER_INFO_S *layer_info, hi_bool is_interlace)
{
    volatile hifb_compress_info *compress_info = &par->compress_info;
    hi_u32 layer_size;
    /* Modify the display buf mode, the memory size has changed, limited by the size of the memory */
    if (layer_info->u32Mask & HIFB_LAYERMASK_BUFMODE) {
        if (layer_info->BufMode == HIFB_LAYER_BUF_ONE) {
            layer_size = hifb_get_line_length(info) * hifb_get_yres(info);
        } else if ((layer_info->BufMode == HIFB_LAYER_BUF_DOUBLE) ||
                   (layer_info->BufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
            layer_size = 2 * hifb_get_line_length(info) * hifb_get_yres(info); /* 2 length data */
        } else {
            layer_size = 0;
        }

        if (layer_size > hifb_get_smem_len(info)) {
            /*
             * layer real memory size:%d KBytes, expected:%d KBtyes
             * real:hifb_get_smem_len(info)/1024, expectde:layer_size/1024
             */
            hifb_error("No enough mem!\n");
            return HI_FAILURE;
        }

        /* If compression is enabled, only 2buf mode can be set. */
        if (compress_info->compress_open) {
            if (layer_info->BufMode != HIFB_LAYER_BUF_DOUBLE &&
                layer_info->BufMode != HIFB_LAYER_BUF_DOUBLE_IMMEDIATE) {
                hifb_error("only HIFB_LAYER_BUF_DOUBLE/HIFB_LAYER_BUF_DOUBLE_IMMEDIATE support compress!\n");
                return HI_FAILURE;
            }
        }
    }

    /* if x>width or y>height ,how to deal with: see nothing in screen or return failure. */
    if (layer_info->u32Mask & HIFB_LAYERMASK_POS) {
        if ((layer_info->s32XPos < 0) || (layer_info->s32YPos < 0)) {
            hifb_error("It's not supported to set start pos of layer to negative!\n");
            return HI_FAILURE;
        }
        /*
         * For interlaced output, the start of the layer must be even.
         * Progressive output without this limit.
         */
        if (is_interlace && (layer_info->s32YPos % 2 != 0)) { /* 2 for align */
            hifb_error("s32YPos should be even for interlace vodev!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 onputlayerinfo_check_premult(hifb_par* par, HIFB_LAYER_INFO_S *layer_info)
{
    /* Limit the pre-multiplication and color value. */
    if ((layer_info->u32Mask & HIFB_LAYERMASK_BMUL) && par->ckey.key_enable) {
       hifb_error("Colorkey and premul couldn't take effect at same time!\n");
       return HI_FAILURE;
    }

    /* return HI_FAILURE, not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444 */
    if (layer_info->u32Mask & HIFB_LAYERMASK_BMUL) {
        if ((layer_info->bPreMul == HI_TRUE) &&
            (par->color_format == HIFB_FMT_ARGB4444 || par->color_format == HIFB_FMT_ARGB1555)) {
           hifb_error("not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
           return HI_FAILURE;
        }
    }
    /* not allow to set pre-mult mode when the GlobalAlpha is 1 */
    if (layer_info->u32Mask & HIFB_LAYERMASK_BMUL) {
        if ((layer_info->bPreMul == HI_TRUE) &&
            (par->alpha.bAlphaChannel == HI_TRUE && par->alpha.u8GlobalAlpha == 1)) {
            hifb_error("not allow to set pre-mult mode when the GlobalAlpha is 1\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_void onputlayerinfo_set_with_mask(hifb_par* par, HIFB_LAYER_INFO_S *layer_info)
{
    hifb_display_info *display_info = &par->display_info;
    HIFB_POINT_S pos;

    if (layer_info->u32Mask & HIFB_LAYERMASK_BMUL) {
        display_info->premul = layer_info->bPreMul;
        par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_BMUL;
    }

    if (layer_info->u32Mask & HIFB_LAYERMASK_BUFMODE) {
        hifb_set_bufmode(par->layer_id, layer_info->BufMode);
    }

    if (layer_info->u32Mask & HIFB_LAYERMASK_POS) {
        pos.s32XPos = layer_info->s32XPos;
        pos.s32YPos = layer_info->s32YPos;
        hifb_set_layerpos(par, &pos);
    }

    if (layer_info->u32Mask & HIFB_LAYERMASK_ANTIFLICKER_MODE) {
        hifb_set_antiflickerlevel(par->layer_id, layer_info->eAntiflickerLevel);
    }
    return;
}

static hi_s32 onputlayerinfo_process(hifb_par* par, HIFB_LAYER_INFO_S *layer_info)
{
    hifb_refresh_info *refresh_info = &par->refresh_info;
    hi_s32 ret = HI_SUCCESS;
    unsigned long lock_flag;

    hifb_spin_lock_irqsave(&par->lock, lock_flag);
    par->modifying = HI_TRUE;

    /* deal with layer_info->mask */
    onputlayerinfo_set_with_mask(par, layer_info);

    /* set screen size in the scaling mode */
    if (g_drv_ops.capability[par->layer_id].bVoScale) {
         hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
         /*
          * The following two functions have a sleep operation inside,
          * you must unlock before calling, and lock the global amount inside the function.
          */
         if (layer_info->u32Mask & HIFB_LAYERMASK_SCREENSIZE) {
             ret = hifb_disp_setscreensize(par->layer_id, layer_info->u32ScreenWidth,
                                           layer_info->u32ScreenHeight);
             if (ret != HI_SUCCESS) {
                 return ret;
             }
         }
         if (layer_info->u32Mask & HIFB_LAYERMASK_DISPSIZE) {
             ret = hifb_disp_setdispsize(par->layer_id, layer_info->u32DisplayWidth,
                                         layer_info->u32DisplayHeight);
             if (ret != HI_SUCCESS) {
                 return ret;
             }
         }
         hifb_spin_lock_irqsave(&par->lock, lock_flag);
    } else {
        /* no scaling mode, no buffer mode, screen size and display size can be set */
        if (refresh_info->buf_mode != HIFB_LAYER_BUF_NONE) {
            hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
            if (layer_info->u32Mask & HIFB_LAYERMASK_SCREENSIZE) {
                ret = hifb_disp_setscreensize(par->layer_id, layer_info->u32ScreenWidth,
                                              layer_info->u32ScreenHeight);
            }
            if (ret != HI_SUCCESS) {
                return ret;
            }
            if (layer_info->u32Mask & HIFB_LAYERMASK_DISPSIZE) {
                ret = hifb_disp_setdispsize(par->layer_id, layer_info->u32DisplayWidth,
                                            layer_info->u32DisplayHeight);
            }
            if (ret != HI_SUCCESS) {
                return ret;
            }
            hifb_spin_lock_irqsave(&par->lock, lock_flag);
        }
    }

    par->modifying = HI_FALSE;
    hifb_spin_unlock_irqrestore(&par->lock, lock_flag);
    return HI_SUCCESS;
}

static hi_s32 hifb_onputlayerinfo(struct fb_info *info, hifb_par* par, const hi_void __user *argp)
{
    hi_s32 ret = HI_SUCCESS;
    HIFB_LAYER_INFO_S layer_info;
    hi_bool is_interlace = HI_FALSE;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        hifb_warning("you shouldn't put cursor layer info!");
        return HI_SUCCESS;
    }

    if (argp == HI_NULL) {
        hifb_error("HI_NULL arg!\n");
        return -EINVAL;
    }

    if (copy_from_user(&layer_info, argp, sizeof(HIFB_LAYER_INFO_S))) {
        return -EFAULT;
    }

    ret = hifb_check_layerinfo(&layer_info);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /*
     * Check the display size
     * Check the canvas size
     * Check the screen size
     */
    is_interlace = hifb_is_interlace(par);
    if (onputlayerinfo_check_size(info, par, &layer_info, is_interlace) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /*
     * Check when modify buf mode
     * Check when modify pos
     */
    if (onputlayerinfo_check_buf_mode(info, par, &layer_info, is_interlace) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    /* Check pre-mult */
    if (onputlayerinfo_check_premult(par, &layer_info) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    /*
     * avoid modifying register in vo isr before all params has been recorded!
     * In vo irq,flag bModifying will be checked.
     */
    ret = onputlayerinfo_process(par, &layer_info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = hifb_alloccanbuf(info, &layer_info);
    if (ret != HI_SUCCESS) {
        /*
         * There is no error returned here, because the user can also
         * specify this memory; in addition, even if the allocation is successful,
         * The user also needs to call FBIOGET_CANVAS_BUF to get it to operate.
         */
        hifb_warning("alloc canvas buffer failed\n");
    }

    hifb_refresh_again(par->layer_id);

    return ret;
}

static hi_s32 hifb_dosetcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp,
                               struct fb_info *info, hi_bool update_finished_reg)
{
    hifb_par *par = (hifb_par *)info->par;

    hi_u32 argb = ((transp & 0xff) << 24) | ((red & 0xff) << 16) | /* 16 24 alg data 0xff mask */
        ((green & 0xff) << 8) | (blue & 0xff); /* 8 alg data 0xff mask */

    if (regno > 255) { /* 255 is larger than */
        hifb_warning("regno: %d, larger than 255!\n", regno);
        return HI_FAILURE;
    }

    g_drv_ops.hifb_drv_set_color_reg(par->layer_id, regno, argb, update_finished_reg);
    return HI_SUCCESS;
}

static hi_s32 hifb_setcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp,
                             struct fb_info *info)
{
    return hifb_dosetcolreg(regno, red, green, blue, transp, info, HI_TRUE);
}

#ifdef __HuaweiLite__
static hi_s32 hifb_setcmap(struct fb_cmap_s *cmap, struct fb_info *info)
#else
static hi_s32 hifb_setcmap(struct fb_cmap *cmap, struct fb_info *info)
#endif
{
    hi_s32 i, start;
    unsigned short *red = HI_NULL;
    unsigned short *green = HI_NULL;
    unsigned short *blue = HI_NULL;
    unsigned short *transp = HI_NULL;
    unsigned short hred, hgreen, hblue;
    unsigned short htransp = 0xffff; /* 0xffff The initial value */
    hifb_par *par = (hifb_par *)info->par;

    if (is_cursor_layer(par->layer_id)) {
        return -EINVAL;
    }

    if (!g_drv_ops.capability[par->layer_id].bCmap) {
        /* AE6D03519, delete this color map warning! */
        return -EPERM;
    }

    red = cmap->red;
    green = cmap->green;
    blue = cmap->blue;
    transp = cmap->transp;

#ifdef __HuaweiLite__
    start = cmap->first;
#else
    start = cmap->start;
#endif
    for (i = 0; i < cmap->len; i++) {
        hred = *red++;
        hgreen = *green++;
        hblue = *blue++;
        if (transp != HI_NULL) {
            htransp = *transp++;
        }

        if (i < cmap->len - 1) {
            if (hifb_dosetcolreg(start++, hred, hgreen, hblue, htransp, info, HI_FALSE)) {
                break;
            }
        } else {
            if (hifb_dosetcolreg(start++, hred, hgreen, hblue, htransp, info, HI_TRUE)) {
                break;
            }
        }
    }
    return 0;
}

#ifdef CONFIG_FB_CFB_IMAGEBLIT
static hi_void hifb_imageblit(struct fb_info *p, const struct fb_image *image)
{
    cfb_imageblit(p, image);
}
#endif

#ifdef __HuaweiLite__
#define HIFB_MAX_LAYER_NUM 1
static struct hifb_info *g_hifb[HIFB_MAX_LAYER_NUM] = {HI_NULL};

static int fb_copy_cmap(const struct fb_cmap_s *src, struct fb_cmap_s *dst)
{
    int offset;
    int copy_size;
    uint8_t *cmap_start = HI_NULL;
    int cmap_size;

    if ((src == HI_NULL) || (dst == HI_NULL)) {
        return -EINVAL;
    }

    cmap_start = dst->red;
    if (cmap_start == HI_NULL)
        return -EINVAL;

    cmap_size = dst->len * sizeof(u_short);

    if (src->first > dst->first) {
        offset = src->first - dst->first;
        copy_size = (src->len > (dst->len - offset)) ? (dst->len - offset) : src->len;

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
    } else {
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
    int ret;

    if (cmap == HI_NULL) {
        return -EINVAL;
    }

    if ((cmap->first < 0) || (info == HI_NULL))
        return -EINVAL;

    ret = hifb_setcmap(cmap, info);
    if (ret == 0)
        fb_copy_cmap(cmap, &info->cmap);

    return ret;
}

FAR struct fb_vtable_s *up_fbgetvplane(int display, int vplane)
{
    (void)vplane;
    if (g_hifb[display] == HI_NULL)
        return HI_NULL;

    return &g_hifb[display]->vtable;
}

static int hifb_getvideoinfo(FAR struct fb_vtable_s *vtable, FAR struct fb_videoinfo_s *vinfo)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if ((info == HI_NULL) || (vinfo == HI_NULL))
        return -EINVAL;

    osal_memcpy(vinfo, &info->vinfo, sizeof(struct fb_videoinfo_s));

    return 0;
}

static int hifb_getplaneinfo(FAR struct fb_vtable_s *vtable, int planeno, FAR struct fb_planeinfo_s *pinfo)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if ((info == HI_NULL) || (pinfo == HI_NULL))
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

    if ((info == HI_NULL) || (oinfo == HI_NULL))
        return -EINVAL;

    osal_memcpy(oinfo, &info->oinfo, sizeof(struct fb_overlayinfo_s));

    return 0;
}

int hifb_getcmap(FAR struct fb_vtable_s *vtable, FAR struct fb_cmap_s *cmap)
{
    struct hifb_info *info = (struct hifb_info *)vtable;

    if ((info == HI_NULL) || (cmap == HI_NULL))
        return -EINVAL;

    return fb_copy_cmap(&info->cmap, cmap);
}

int hifb_putcmap(FAR struct fb_vtable_s *vtable, FAR const struct fb_cmap_s *cmap)
{
    struct hifb_info *info = (struct hifb_info *)vtable;
    int cpopy_size;

    if ((info == HI_NULL) || (cmap == HI_NULL))
        return -EINVAL;

    if ((cpopy_size = cmap->len * sizeof(hi_u16)) < 0)
        return -EINVAL;

    return fb_set_cmap(cmap, info);
}

static hi_s32 fbinitialize_alloc_mem(struct hifb_info *info)
{
    hi_char name[16] = {'\0'}; /* 16 is The length of the array */
    hifb_par *par = HI_NULL;
    par = (hifb_par *)(info->par);

    if (g_layer[par->layer_id].layer_size != 0) {
        osal_snprintf(name, sizeof(name), "hifb_layer%d", par->layer_id);

        /* initialize the fix screen info */
        if (is_hd_layer(par->layer_id)) {
            hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_HD]);
        } else if (is_sd_layer(par->layer_id)) {
            hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_SD]);
        } else if (is_ad_layer(par->layer_id)) {
            hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_AD]);
        } else if (is_cursor_layer(par->layer_id)) {
            hifb_screeninfo_init(info, &g_default_info[HIFB_LAYER_TYPE_CURSOR]);
        }
        /* 16 buffer size */
        info->oinfo.fbmem = (void *)hifb_buf_allocmem(name, 16, (g_layer[par->layer_id].layer_size +
            /* 1024 Into the system */
            g_layer[par->layer_id].curosr_buf_size) * 1024, hifb_get_layer_mmz_names(par->layer_id));
        if (info->oinfo.fbmem == HI_NULL) {
            hifb_error("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_layer[par->layer_id].layer_size + g_layer[par->layer_id].curosr_buf_size));
            hifb_free(info);
            g_layer[par->layer_id].info = HI_NULL;
            return HI_FAILURE;
        }
        /* u32LayerSize is KB */
        info->oinfo.fblen = g_layer[par->layer_id].layer_size * 1024; /* 1024 layer size */

        /* initialize the virtual address and clear memory */
        info->oinfo.fbmem = hifb_buf_map((hi_u32)(uintptr_t)info->oinfo.fbmem,
                                         (g_layer[par->layer_id].layer_size +
                                          g_layer[par->layer_id].curosr_buf_size) * 1024); /* 1024 buf size */
        if (info->oinfo.fbmem == HI_NULL) {
            hifb_warning("Failed to call map video memory,size:0x%x, start: 0x%lx\n",
                info->oinfo.fblen, info->oinfo.fbmem);
        }

        if (info != HI_NULL) {
            osal_memset(info->oinfo.fbmem, 0, info->oinfo.fblen);
        }
    }
    return HI_SUCCESS;
}

int up_fbinitialize(int display)
{
    hi_s32 ret = HI_FAILURE;
    struct hifb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;

    /* Creates a new frame buffer info structure. reserves hifb_par for driver private data (info->par) */
    info = hifb_alloc(sizeof(hifb_par));
    if (info == HI_NULL) {
        hifb_error("failed to malloc the hifb_info!\n");
        return -ENOMEM;
    }

    /* save the info pointer in global pointer array */
    g_layer[display].info = info;

    info->oinfo.overlay = display;
    info->vtable.fb_open = hifb_open;
    info->vtable.fb_release = hifb_release;
    info->vtable.fb_set_par = hifb_set_par;
    info->vtable.fb_pan_display = hifb_pan_display;
    info->vtable.fb_ioctl = hifb_ioctl;
    info->vtable.getvideoinfo = hifb_getvideoinfo;
    info->vtable.getplaneinfo = hifb_getplaneinfo;
    info->vtable.getoverlayinfo = hifb_getoverlayinfo;
    info->vtable.getcmap = HI_NULL;
    info->vtable.putcmap = HI_NULL;

    par = (hifb_par *)(info->par);
    osal_memset(par, 0, sizeof(hifb_par));
    par->layer_id = display;
    par->color_format = HIFB_FMT_ARGB1555;
    info->vinfo.fmt = HIFB_FMT_ARGB1555;

    /* It's not need to alloc mem for cursor layer if use g_softcursor */
    ret = fbinitialize_alloc_mem(info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    g_hifb[display] = info;

    return 0;
}

void up_fbuninitialize(int display)
{
    struct hifb_info *info = HI_NULL;

    /* get framebuffer info structure pointer */
    info = g_layer[display].info;
    if (info != HI_NULL) {
        if (info->oinfo.fbmem) {
            hifb_buf_ummap(info->oinfo.fbmem);
        }

        if (info->oinfo.fbmem) {
            hifb_buf_freemem((hi_u32)(uintptr_t)info->oinfo.fbmem);
        }

        hifb_free(info);
        g_layer[display].info = HI_NULL;
        g_hifb[display] = HI_NULL;
    }
}
#endif

static hi_s32 hifb_set_dynamic_range_display(hifb_par *par, HIFB_DYNAMIC_RANGE_E dynamic_range)
{
    if (dynamic_range >= HIFB_DYNAMIC_RANGE_BUTT) {
        hifb_error("The input dynamic range value is wrong!\n");
        return -EINVAL;
    }

    par->display_info.dynamic_range = dynamic_range;
    return HI_SUCCESS;
}

#if HICHIP == HI3559A_V100
#ifdef CONFIG_DMA_SHARED_BUFFER
struct dma_buf *hifb_dmabuf_export(struct fb_info *info)
{
    return hifb_memblock_export(hifb_get_smem_start(info), hifb_get_smem_len(info), 0);
}
#endif
#endif

/*
 * Name : g_hi_fb_ops
 * Desc : fb struct
 * See  : hifb_overlay_probe
 */
#ifndef __HuaweiLite__
static struct fb_ops g_hi_fb_ops = {
    .owner = THIS_MODULE,
    .fb_open = hifb_open,
    .fb_release = hifb_release,
    .fb_check_var = hifb_check_var,
    .fb_set_par = hifb_set_par,
    .fb_pan_display = hifb_pan_display,
    .fb_ioctl = hifb_ioctl,
    .fb_setcolreg = hifb_setcolreg,
    .fb_setcmap = hifb_setcmap,
#ifdef CONFIG_COMPAT
    .fb_compat_ioctl = hifb_compat_ioctl,
#endif
#ifdef CONFIG_FB_CFB_IMAGEBLIT
    .fb_imageblit = hifb_imageblit,
#endif
#if HICHIP == HI3559A_V100
#ifdef CONFIG_DMA_SHARED_BUFFER
    .fb_dmabuf_export = hifb_dmabuf_export,
#endif
#endif
};
#endif

/*
 * Function        : hifb_overlay_cleanup
 * Description     : releae the resource for certain framebuffer
 */
#ifdef __HuaweiLite__
static hi_void hifb_overlay_cleanup(hi_u32 layer_id, hi_bool unregister)
{
    hi_s32 ret;
    ret = fb_unregister(layer_id);
    if (ret < 0) {
        hifb_error("hifb overlay cleanup fail!\n");
    }

    return;
}
#else
static hi_void hifb_overlay_cleanup(hi_u32 layer_id, hi_bool unregister)
{
    struct fb_info *info = HI_NULL;
    /* get framebuffer info structure pointer */
    info = g_layer[layer_id].info;
    if (info != HI_NULL) {
        if (hifb_get_screen_base(info)) {
            hifb_buf_ummap(hifb_get_screen_base(info));
        }

        if (hifb_get_smem_start(info)) {
            hifb_buf_freemem(hifb_get_smem_start(info));
        }

        if (unregister) {
            unregister_framebuffer(info);
        }

        framebuffer_release(info);
        g_layer[layer_id].info = HI_NULL;
    }

    return;
}
#endif

#ifndef __HuaweiLite__
static hi_s32 overlay_probe_alloc_mem(struct fb_info *info, struct fb_fix_screeninfo *fix,
                                      struct fb_var_screeninfo *var)
{
    hifb_par *par = HI_NULL;
    hi_u32 layer_id;
    hi_char name[16]; /* 16 for length of name */

    par = (hifb_par *)(info->par);
    layer_id = par->layer_id;

    if (g_layer[layer_id].layer_size != 0) {
        osal_snprintf(name, sizeof(name), "hifb_layer%u", layer_id);

        /* initialize the fix screen info */
        if (is_hd_layer(layer_id)) {
            *fix = g_default_fix[HIFB_LAYER_TYPE_HD];
            *var = g_default_var[HIFB_LAYER_TYPE_HD];
        } else if (is_sd_layer(layer_id)) {
            *fix = g_default_fix[HIFB_LAYER_TYPE_SD];
            *var = g_default_var[HIFB_LAYER_TYPE_SD];
        } else if (is_ad_layer(layer_id)) {
            *fix = g_default_fix[HIFB_LAYER_TYPE_AD];
            *var = g_default_var[HIFB_LAYER_TYPE_AD];
        } else if (is_cursor_layer(layer_id)) {
            *fix = g_default_fix[HIFB_LAYER_TYPE_CURSOR];
            *var = g_default_var[HIFB_LAYER_TYPE_CURSOR];
        }
        fix->smem_start = hifb_buf_allocmem(name, 16, /* 16 buffer size */
            (g_layer[layer_id].layer_size + g_layer[layer_id].curosr_buf_size) * 1024, /* 1024 for 1k */
            hifb_get_layer_mmz_names(layer_id));
        if (fix->smem_start == 0) {
            hifb_error("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_layer[layer_id].layer_size + g_layer[layer_id].curosr_buf_size));
            return HI_FAILURE;
        }
        /* u32LayerSize is KB */
        fix->smem_len = g_layer[layer_id].layer_size * 1024; /* 1024 for 1k */

        /* initialize the virtual address and clear memory */
        info->screen_base = hifb_buf_map(fix->smem_start, (g_layer[layer_id].layer_size +
                                         g_layer[layer_id].curosr_buf_size) * 1024); /* 1024 for 1k */

        if (info->screen_base == HI_NULL) {
            hifb_error("Failed to call map video memory,size:0x%x, start: 0x%lx\n",
                       fix->smem_len, fix->smem_start);
            return HI_FAILURE;
        }

        /* Initialize the memory to 0. Call TDE to do it. */
#if (HICHIP == HI3516E_V200)
        osal_memset(info->screen_base, 0x0, hifb_get_smem_len(info));
#else
        hifb_completely(info, 0, hifb_get_smem_len(info));
#endif
    }
    return HI_SUCCESS;
}
#endif

/*
 * Function        : hifb_overlay_probe
 * Description     : initialze the framebuffer for the overlay and set
 */
#ifdef __HuaweiLite__
static hi_s32 __init hifb_overlay_probe(hi_u32 layer_id)
{
    hi_s32 ret = fb_register(layer_id, 0);
    if (ret < 0) {
        hifb_error("failed to register_framebuffer!\n");
        ret = -EINVAL;
        goto ERR;
    }

    return HI_SUCCESS;

ERR:
    hifb_overlay_cleanup(layer_id, HI_FALSE);

    return ret;
}
#else
static hi_s32 __init hifb_overlay_probe(hi_u32 layer_id)
{
    hi_s32 ret;
    struct fb_info *info = HI_NULL;
    struct fb_fix_screeninfo *fix = HI_NULL;
    struct fb_var_screeninfo *var = HI_NULL;
    hifb_par *par = HI_NULL;

    /*
     * The size of the size represents the private data space of the device,
     * and the par of fb_info points to the private space, that is,
     * info->par already points to the memory space
     */
    info = framebuffer_alloc(sizeof(hifb_par), HI_NULL);
    if (info == HI_NULL) {
        hifb_error("failed to malloc the fb_info!\n");
        return -ENOMEM;
    }
    fix = &info->fix;
    var = &info->var;
    /* save the info pointer in global pointer array */
    g_layer[layer_id].info = info;

    osal_snprintf(fix->id, sizeof(fix->id), "ovl%u", layer_id);
    info->flags = FBINFO_FLAG_DEFAULT | FBINFO_HWACCEL_YPAN | FBINFO_HWACCEL_XPAN;
    /* fbops members in fb_info point to s_sthifbops, so open, release, ioctl, etc. can get fb_info. */
    info->fbops = &g_hi_fb_ops;

    par = (hifb_par *)(info->par);
    osal_memset(par, 0, sizeof(hifb_par));
    par->layer_id = layer_id;
    par->color_format = HIFB_FMT_ARGB1555;

    /* It's not need to alloc mem for cursor layer if use softcursor */
    ret = overlay_probe_alloc_mem(info, fix, var);
    if (ret != HI_SUCCESS) {
        goto ERR;
    }
    ret = register_framebuffer(info);
    if (ret < 0) {
        hifb_error("failed to register_framebuffer!layerid = %d, s32Ret = %d\n", layer_id, ret);
        ret = -EINVAL;
        goto ERR;
    }

    hifb_dbg_info("succeed in registering the fb%d: %s frame buffer device\n",
                  info->node, fix->id);

    return HI_SUCCESS;

ERR:
    hifb_overlay_cleanup(layer_id, HI_FALSE);

    return ret;
}
#endif

/*
 * Function        : hifb_get_vram_size
 * Description     : parse the parameter string and get the size. if
                     the parameter is invalid, the size is default value.
 * Input           : const char* pstr  the string for the vram size
 * Return          : the video memory size
 */
static unsigned long hifb_get_vram_size(char *pstr)
{
    hi_s32 str_is_valid = HI_TRUE;
    unsigned long vram_size = 0;
    unsigned long vram_size_temp = 0;
    char *ptr = pstr;

    if ((ptr == HI_NULL) || (*ptr == '\0')) {
        return 0;
    }

    /* check if the string is valid */
    while (*ptr != '\0') {
        if (*ptr == ',') {
            /* *ptr = '\0'; */
            break;
        } else if ((!isdigit(*ptr)) && ('X' != *ptr) && ('x' != *ptr) &&
                   ((*ptr > 'f' && *ptr <= 'z') || (*ptr > 'F' && *ptr <= 'Z'))) {
            str_is_valid = HI_FALSE;
            break;
        }

        ptr++;
    }

    if (str_is_valid) {
#ifdef __HuaweiLite__
        vram_size = strtoul(pstr, (char **)HI_NULL, 0);
#else
        vram_size = simple_strtoul(pstr, (char **)HI_NULL, 0);
#endif
        if (vram_size > PAGE_SIZE_ALIGN_MAX) {
            hifb_error("vram_size(%lu)( > %lu) is overflow, it will be set to %u!\n", vram_size, PAGE_SIZE_ALIGN_MAX,
                       0);
            vram_size = 0;
        }

        vram_size_temp = vram_size;
        /* make the size PAGE_SIZE align */
        vram_size = ((vram_size * 1024 + PAGE_SIZE - 1) & PAGE_MASK) / 1024; /* 2^10 1024 */
        if (vram_size_temp != vram_size) {
            hifb_error("vram_size(%lu) if not align in 4, it will be set to %lu!\n", vram_size_temp, vram_size);
        }
    }
    return vram_size;
}

static hi_void parse_cfg_start(hi_char **sc_str)
{
    /* judge the cursor if use soft or hard layer */
    if (!strcmp("off", g_softcursor)) {
        g_soft_cursor = HI_FALSE;
    } else {
        g_soft_cursor = HI_TRUE;
    }

    /* judge the g_display is need on */
    if (!strcmp("on", g_display)) {
        g_display_on = HI_TRUE;
    } else {
        g_display_on = HI_FALSE;
    }

    /* get the string before next varm */
    *sc_str = strstr(g_video, "vram");
    hifb_dbg_info("video:%s\n", g_video);

    return;
}

static hi_void parse_cfg_change_layer_size(hi_char *sc_str, hi_u32 layer_id)
{
    hi_ulong layer_size;
    layer_size = hifb_get_vram_size(sc_str);

    if (g_drv_ops.capability[layer_id].bLayerSupported) {
        if (is_cursor_layer(layer_id)) {
            if (!is_soft_cursor()) {
                g_layer[layer_id].layer_size = layer_size;
            }
        } else {
            g_layer[layer_id].layer_size = layer_size;
            if (is_soft_cursor() && layer_size) {
                g_layer[layer_id].curosr_buf_size = HIFB_CURSOR_DEF_VRAM;
            }
        }
    }
    return;
}

static hi_s32 parse_cfg_change_layer_id(hi_u32 *layer_id, hi_char *number, hi_u32 size)
{
#ifdef __HuaweiLite__
    *layer_id = strtoul(number, (char **)HI_NULL, 10); /* 10 base data */
#else
    *layer_id = simple_strtoul(number, (char **)HI_NULL, 10); /* 10 base data */
#endif
    if (*layer_id >= g_drv_ops.layer_count) {
        hifb_error("Layer %u is in module_param---video out of range!\n", *layer_id);
        return HI_FAILURE;
    }

    if (!g_drv_ops.capability[*layer_id].bLayerSupported) {
        hifb_error("Layer %u unsupported, so module_param---video can't contain vram_size for it!\n",
            *layer_id);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*
 * Name : hifb_parse_cfg
 * Desc : Parse the parameters.
 * See  : hifb_overlay_probe
 */
static hi_s32 hifb_parse_cfg(hi_void)
{
    hi_char *sc_str = HI_NULL;
    hi_char number[HIFB_NUMBLE] = {0};
    hi_u32 i, j;
    hi_u32 layer_id;
    hi_char ac_param[HIFB_ACFARAM] = {0};
    hi_char ac_temp[HIFB_ACTEMP] = {0};
    hi_bool is_param_valid = HI_FALSE;

    /*
     * 1.judge the cursor if use soft or hard layer
     * 2.judge the display is need on
     * 3.get the string before next varm
     */
    parse_cfg_start(&sc_str);

    /* parse cfg process */
    while (sc_str != HI_NULL) {
        /* parse the layer id and save it in a string */
        i = 0;

        /* if the number of graphics layers is 10 or more, the string is risky */
        for (j = 0; j < g_drv_ops.layer_count; j++) {
            osal_snprintf(ac_param, sizeof(ac_param), "vram%u_size", j);
            osal_strncpy(ac_temp, sc_str, 10); /* number of graphics layers is 10 or more, */
            if (!strcmp(ac_param, ac_temp)) {
                is_param_valid = HI_TRUE;
            }
        }
        if (!is_param_valid) {
            hifb_error("insmod parameter is invalid!\n");
            return HI_FAILURE;
        }
        /* skip "vram" */
        sc_str += HIFB_NUMBLE;
        while (*sc_str != '_') {
            /* i>1 means layer id is bigger than 100, it's obviously out of range! */
            if (i > 1) {
                hifb_error("layer id is out of range!\n");
                return -1;
            }

            number[i] = *sc_str;
            i++;
            sc_str++;
        }

        number[i] = '\0';

        /* change the layer id string into digital and assure it's legal */
        if (parse_cfg_change_layer_id(&layer_id, number, HIFB_NUMBLE) != HI_SUCCESS) {
            return HI_FAILURE;
        }

        sc_str += sizeof("size") + i;
        /* get the layer size string and change it to digital */
        parse_cfg_change_layer_size(sc_str, layer_id);

        /* get next layer string */
        sc_str = strstr(sc_str, "vram");
    }

    return HI_SUCCESS;
}

#ifndef DISABLE_DEBUG_INFO
static const hi_char *g_fmt_name[] = {
    "RGB565",   "RGB888",   "KRGB444",  "KRGB555",  "KRGB888", "ARGB4444", "ARGB1555", "ARGB8888", "ARGB8565",
    "RGBA4444", "RGBA5551", "RGBA5658", "RGBA8888", "1BPP",    "2BPP",     "4BPP",     "8BPP",     "ACLUT44",
    "ACLUT88",  "PUYVY",    "PYUYV",    "PYVYU",    "YUV888",  "AYUV8888", "YUVA8888", "BUTT"};

const static hi_char *g_layer_name[] = {"layer_0", "layer_1", "layer_2", "layer_3"};
hi_s32 hifb_print_softcursor_proc(struct fb_info *info, struct seq_file *p, hi_void *v)
{
    hifb_par *par = (hifb_par *)info->par;
    const hi_char *layer_name = HI_NULL;

    if (par->layer_id >= sizeof(g_layer_name) / sizeof(*g_layer_name)) {
        layer_name = "unkown layer";
    } else {
        layer_name = g_layer_name[par->layer_id];
    }

    seq_printf(p,  "layer name                 \t: %s \n", layer_name);
    seq_printf(p,  "Show   State               \t :%s\n", par->show ? "ON" : "OFF");
    seq_printf(p,  "referecce count            \t :%d\n", atomic_read(&par->ref_count));
    seq_printf(p,  "position                   \t :(%d, %d)\n", par->display_info.pos.s32XPos,
               par->display_info.pos.s32YPos);
    seq_printf(p,  "ColorFormat:               \t :%s\n", g_fmt_name[par->color_format]);
    seq_printf(p,  "Alpha  Enable              \t :%s\n", par->alpha.bAlphaEnable ? "ON" : "OFF");
    seq_printf(p,  "AlphaChannel Enable        \t :%s\n", par->alpha.bAlphaChannel ? "ON" : "OFF");
    seq_printf(p,  "Alpha0, Alpha1             \t :%u, %u\n", par->alpha.u8Alpha0, par->alpha.u8Alpha1);
    seq_printf(p,  "Alpha Global               \t :%u\n", par->alpha.u8GlobalAlpha);
    seq_printf(p,  "Colorkey Enable            \t :%s\n", par->ckey.key_enable ? "ON" : "OFF");
    seq_printf(p,  "Colorkey value             \t :0x%x\n", par->ckey.key);
    seq_printf(p,  "cursor hot pos(x, y)       \t :(%d, %d)\n", par->cursor_info.cursor.stHotPos.s32XPos,
               par->cursor_info.cursor.stHotPos.s32YPos);
    return 0;
}

static hi_void print_cursor_proc(struct seq_file *p, hifb_par *par)
{
    seq_printf(p,  "attach cursor id:         \t  :%u \n", par->cursor_info.attached_cursor_id);
    seq_printf(p,  "backup cursor addr         \t :0x%llu\n", par->cursor_info.cursor.stCursor.u64PhyAddr);
    seq_printf(p,  "backup cursor fmt          \t :%s\n", g_fmt_name[par->cursor_info.cursor.stCursor.enFmt]);
    seq_printf(p,  "backup cursor stride       \t :%u\n", par->cursor_info.cursor.stCursor.u32Pitch);
    seq_printf(p,  "backup cursor (w, h)       \t :(%u, %u)\n",
               par->cursor_info.cursor.stCursor.u32Width, par->cursor_info.cursor.stCursor.u32Height);
    seq_printf(p,  "cursor rect in g_display buffer \t :(%d, %d, %d, %d)\n",
               par->cursor_info.rect_in_disp_buf.x, par->cursor_info.rect_in_disp_buf.y,
               par->cursor_info.rect_in_disp_buf.w, par->cursor_info.rect_in_disp_buf.h);
    seq_printf(p,  "cursor pos in cursor image \t :(%d, %d)\n",
               par->cursor_info.pos_in_cursor.s32XPos, par->cursor_info.pos_in_cursor.s32YPos);
    return;
}

static hi_void print_canvas_proc(struct seq_file *p, hifb_par *par)
{
    hifb_refresh_info *refresh_info = &par->refresh_info;
    volatile hifb_compress_info *compress_info = &par->compress_info;

    seq_printf(p,  "canavas updated addr       \t :0x%llx\n", refresh_info->user_buffer.stCanvas.u64PhyAddr +
               refresh_info->user_buffer.UpdateRect.y * refresh_info->user_buffer.UpdateRect.w +
               refresh_info->user_buffer.UpdateRect.x);
    seq_printf(p,  "canavas updated (w, h)     \t :%d,%d \n", refresh_info->user_buffer.UpdateRect.w,
               refresh_info->user_buffer.UpdateRect.h);
    seq_printf(p,  "canvas width               \t :%u\n", refresh_info->user_buffer.stCanvas.u32Width);
    seq_printf(p,  "canvas height              \t :%u\n", refresh_info->user_buffer.stCanvas.u32Height);
    seq_printf(p,  "canvas pitch               \t :%u\n", refresh_info->user_buffer.stCanvas.u32Pitch);
    seq_printf(p,  "canvas format              \t :%s\n", g_fmt_name[refresh_info->user_buffer.stCanvas.enFmt]);
    seq_printf(p,  "IsCompress                 \t :%s\n", compress_info->compress_open ? "YES" : "NO");
    seq_printf(p,  "Is DDR Dettect             \t :%s\n", (compress_info->compress_open &&
               compress_info->zone_nums && (refresh_info->buf_mode == HIFB_LAYER_BUF_NONE ||
               refresh_info->buf_mode == HIFB_LAYER_BUF_BUTT)) ? "YES" : "NO");
    seq_printf(p,  "DDR Detect Zones           \t :%d\n", (compress_info->compress_open &&
              (refresh_info->buf_mode == HIFB_LAYER_BUF_NONE || refresh_info->buf_mode ==
               HIFB_LAYER_BUF_BUTT)) ? compress_info->zone_nums : 0);
    seq_printf(p,  "PreMul Enable                 \t :%s\n", par->display_info.premul ? "ON" : "OFF");
    return;
}

static hi_void print_display_proc(struct seq_file *p, hifb_par *par)
{
    const hi_char *buf_mode[] = {
        "triple", "double", "single", "triple( no frame discarded)", "fence", "unkown"
    };
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;

    seq_printf(p,  "Display Buffer mode(+UsrBuf)\t :%s\n", buf_mode[refresh_info->buf_mode]);
    seq_printf(p,  "Displaying addr (register) \t :0x%llx\n", refresh_info->screen_addr);
    seq_printf(p,  "g_display buffer[0] addr     \t :0x%llx\n", refresh_info->disp_buf_info.phy_addr[0]);
    seq_printf(p,  "g_display buffer[1] addr     \t :0x%llx\n", refresh_info->disp_buf_info.phy_addr[1]);
    seq_printf(p,  "Be PreMul Mode:            \t :%s\n", display_info->premul == HI_TRUE ? "YES" : "NO");
    seq_printf(p,  "displayrect                \t :(%u, %u)\n", display_info->display_width,
               display_info->display_height);
    seq_printf(p,  "screenrect                 \t :(%u, %u)\n", display_info->screen_width,
               display_info->screen_height);
    seq_printf(p,  "device max resolution      \t :%u, %u\n", display_info->max_screen_width,
               display_info->max_screen_height);
    seq_printf(p,  "IsNeedFlip(2buf)           \t :%s\n", refresh_info->disp_buf_info.need_flip ? "YES" : "NO");
    seq_printf(p,  "BufferIndexDisplaying(2buf)\t :%u\n", refresh_info->disp_buf_info.index_for_int);
    seq_printf(p,  "refresh request num(2buf)  \t :%u\n", refresh_info->refresh_num);
    seq_printf(p,  "switch buf num(2buf)       \t :%u\n", refresh_info->disp_buf_info.int_pic_num);
    seq_printf(p,  "union rect (2buf)          \t :(%d,%d,%d,%d)\n",
               refresh_info->disp_buf_info.union_rect.x, refresh_info->disp_buf_info.union_rect.y,
               refresh_info->disp_buf_info.union_rect.w, refresh_info->disp_buf_info.union_rect.h);
    return;
}

static hi_void print_common_proc(struct seq_file *p, struct fb_info *info, const hi_char *layer_name)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    hifb_refresh_info *refresh_info = &par->refresh_info;
    const hi_char *antiflicer_level[] = {"NONE", "LOW", "MIDDLE", "HIGH", "AUTO", "ERROR"};
    const hi_char *mirror_mode[] = {"NONE", "HORIZONTAL", "VERTICAL", "BOTH", "unkown"};
    const hi_char *dynamic_range[] = {"SDR8", "SDR10", "HDR10", "HLG", "SLF", "unknow"};
    const hi_char *anti_mode[] = {"NONE", "TDE", "VOU", "ERROR"};
    const hi_char *rotation_mode[] = {"0", "90", "180", "270", "-"};

    seq_printf(p,  "\n[HIFB] Version: ["MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n");
    seq_printf(p,  "\n");

    seq_printf(p,  "layer name                 \t :%s \n", layer_name);
    seq_printf(p,  "Open count                 \t :%d\n", atomic_read(&par->ref_count));
    seq_printf(p,  "Show state                 \t :%s\n", par->show ? "ON" : "OFF");
    seq_printf(p,  "Start position             \t :(%d, %d)\n", display_info->pos.s32XPos,
               display_info->pos.s32YPos);
    seq_printf(p,  "xres, yres                 \t :(%u, %u)\n", hifb_get_xres(info), hifb_get_yres(info));
    seq_printf(p,  "xres_virtual, yres_virtual \t :(%u, %u)\n", hifb_get_xres_virtual(info),
               hifb_get_yres_virtual(info));
    seq_printf(p,  "xoffset, yoffset           \t :(%u, %u)\n", hifb_get_xoffset(info), hifb_get_yoffset(info));
    seq_printf(p,  "fix.line_length            \t :%u\n", hifb_get_line_length(info));
    seq_printf(p,  "Mem size:                  \t :%u KB\n", hifb_get_smem_len(info) / 1024); /* 2^10 1024 */
    seq_printf(p,  "Layer Scale (hw):          \t :%s \n", g_drv_ops.capability[par->layer_id].
               bVoScale ? "YES" : "NO");
    seq_printf(p,  "ColorFormat:               \t :%s\n", g_fmt_name[par->color_format]);
    seq_printf(p,  "Alpha Enable               \t :%s\n", par->alpha.bAlphaEnable ? "ON" : "OFF");
    seq_printf(p,  "AlphaChannel Enable        \t :%s\n", par->alpha.bAlphaChannel ? "ON" : "OFF");
    seq_printf(p,  "Alpha0, Alpha1             \t :%u, %u\n", par->alpha.u8Alpha0, par->alpha.u8Alpha1);
    seq_printf(p,  "Alpha Global               \t :%u\n", par->alpha.u8GlobalAlpha);
    seq_printf(p,  "Colorkey Enable            \t :%s\n", par->ckey.key_enable ? "ON" : "OFF");
    seq_printf(p,  "Colorkey value             \t :0x%x\n", par->ckey.key);
    seq_printf(p,  "Mirror Mode:            \t :%s\n", mirror_mode[display_info->mirror_mode]);
    seq_printf(p,  "Dynamic Range:            \t :%s\n", dynamic_range[display_info->dynamic_range]);
    seq_printf(p,  "Deflicker Mode:            \t :%s\n", anti_mode[display_info->antiflicker_mode]);
    seq_printf(p,  "Rotation mode:             \t :%s\n", rotation_mode[display_info->rotate_mode]);
    seq_printf(p,  "Deflicker Level:           \t :%s\n", antiflicer_level[display_info->antiflicker_level]);
    seq_printf(p,  "HiFB mode:                  \t :%s\n", HIFB_LAYER_BUF_BUTT ==
                                                            refresh_info->buf_mode ? "STANDARD" : "EXTEND");
    return;
}

hi_s32 hifb_print_layer_proc(struct fb_info *info, struct seq_file *p, hi_void *v)
{
    hifb_par *par = (hifb_par *)info->par;
    hifb_display_info *display_info = &par->display_info;
    const hi_char *layer_name = HI_NULL;

    if (par->layer_id >= sizeof(g_layer_name) / sizeof(*g_layer_name)) {
        layer_name = "unkown layer";
    } else {
        layer_name = g_layer_name[par->layer_id];
    }

    if (display_info->antiflicker_mode > HIFB_ANTIFLICKER_BUTT) {
        display_info->antiflicker_mode = HIFB_ANTIFLICKER_BUTT;
    }

    if (display_info->antiflicker_level > HIFB_LAYER_ANTIFLICKER_BUTT) {
        display_info->antiflicker_level = HIFB_LAYER_ANTIFLICKER_BUTT;
    }

    print_common_proc(p, info, layer_name);

    print_display_proc(p, par);

    print_canvas_proc(p, par);

    if (par->cursor_info.attched && is_soft_cursor()) {
        print_cursor_proc(p, par);
    }
    return HI_SUCCESS;
}

hi_s32 hifb_read_proc(struct osal_proc_dir_entry *entry)
{
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    if (entry == HI_NULL) {
        return HI_FAILURE;
    }
    info = (struct fb_info *)(entry->private);
    if (info == HI_NULL) {
        return HI_FAILURE;
    }
    par = (hifb_par *)info->par;
    if (par == HI_NULL) {
        return HI_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id) || !is_soft_cursor()) {
        return hifb_print_layer_proc(info, entry->seqfile, HI_NULL);
    } else {
        return hifb_print_softcursor_proc(info, entry->seqfile, HI_NULL);
    }

    return HI_SUCCESS;
}

static hi_void parse_procmd_help(hifb_par *par, hi_char *cmd)
{
    if (strncmp("help", cmd, 4) == 0) { /* 4 is length */
        printk("help info:\n");
        printk("echo cmd > proc file\n");
        printk("hifb support cmd:\n");
        printk("show:show layer\n");
        printk("hide:hide layer\n");
        printk("For example, if you want to hide layer 0,you can input:\n");
        printk("   echo hide > /proc/umap/hifb0\n");
    } else {
        hifb_error("u32LayerID %d doesn't support cmd:%s, use help cmd to show help info!\n", par->layer_id, cmd);
    }
    return;
}

static hi_void hifb_parse_proccmd(struct seq_file *p, hi_u32 layer_id, hi_char *cmd)
{
    struct fb_info *info = g_layer[layer_id].info;
    hifb_par *par = (hifb_par *)info->par;
    hi_s32 cnt;
    unsigned long lockflag;

    hifb_spin_lock_irqsave(&par->lock, lockflag);
    cnt = atomic_read(&par->ref_count);

    if (strncmp("show", cmd, 4) == 0) { /* 4 is lengh */
        if (cnt == 0) {
            hifb_spin_unlock_irqrestore(&par->lock, lockflag);
            hifb_error("err:u32LayerId %d no open!\n", par->layer_id);
            return;
        }

        if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
            hifb_spin_unlock_irqrestore(&par->lock, lockflag);
            hifb_error("cursor layer %d doesn't support this cmd!\n", par->layer_id);
            return;
        }

        if (!par->show) {
            par->modifying = HI_TRUE;
            par->show = HI_TRUE;
            par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_SHOW;
            par->modifying = HI_FALSE;
        }
    } else if (strncmp("hide", cmd, 4) == 0) { /* 4 is lengh */
        if (cnt == 0) {
            hifb_spin_unlock_irqrestore(&par->lock, lockflag);
            hifb_error("err:u32LayerId %d no open!\n", par->layer_id);
            return;
        }

        if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
            hifb_spin_unlock_irqrestore(&par->lock, lockflag);
            hifb_error("cursor layer %d doesn't support this cmd!\n", par->layer_id);
            return;
        }

        if (par->show) {
            par->modifying = HI_TRUE;
            par->show = HI_FALSE;
            par->param_modify_mask |= HIFB_LAYER_PARAMODIFY_SHOW;
            par->modifying = HI_FALSE;
        }
    }

    hifb_spin_unlock_irqrestore(&par->lock, lockflag);

    parse_procmd_help(par, cmd);
    return;
}

hi_s32 hifb_write_proc(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *ppos)
{
#ifndef __HuaweiLite__
#define TMP_BUF_LEN 32
    struct seq_file *s = HI_NULL;
    struct fb_info *info = HI_NULL;
    hifb_par *par = HI_NULL;
    char tmp_buf[TMP_BUF_LEN] = {0};
    int len;

    if (entry == HI_NULL) {
        return -ENOSYS;
    }
    s = entry->seqfile;

    if (s == HI_NULL) {
        return -ENOSYS;
    }
    if (count <= 0) {
        return -ENOSYS;
    }

    len = (count >= TMP_BUF_LEN) ? TMP_BUF_LEN : count;

    if (osal_copy_from_user(tmp_buf, buf, len))
        return HI_FAILURE;

    tmp_buf[len - 1] = '\0';

    info = (struct fb_info *)(entry->private);
    if (info == HI_NULL) {
        return HI_FAILURE;
    }
    par = (hifb_par *)(info->par);
    if (par == HI_NULL) {
        return HI_FAILURE;
    }

    hifb_parse_proccmd(s, par->layer_id, (hi_char *)tmp_buf);
    osal_memset((void *)tmp_buf, 0, count);
#endif
    return count;
}
#endif

/* for linux */
#ifndef __HuaweiLite__
#ifdef CONFIG_HI_VO_FB_SEPARATE

#define HIFB_INT_NAME_LENGTH 10

static int hi35xx_hifb_probe(struct platform_device *pdev)
{
    hi_char hifb_int_name[HIFB_INT_NAME_LENGTH] = "hifb";

    osal_platform_get_modparam_string(pdev, "video", 64, g_video); /* 64 video length */

    g_fifb_irq = osal_platform_get_irq_byname(pdev, hifb_int_name);
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

    if (hifb_init() != HI_SUCCESS) {
        printk("hifb_init HI_FAILURE!\n");
    }

    return 0;
}

static int hi35xx_hifb_remove(struct platform_device *pdev)
{
    hifb_cleanup();

    return 0;
}

static const struct of_device_id g_hi35xx_hifb_match[] = {
    {.compatible = "hisilicon,hisi-hifb"},
    {},
};
MODULE_DEVICE_TABLE(of, g_hi35xx_hifb_match);

static struct platform_driver g_hi35xx_hifb_driver = {
    .probe = hi35xx_hifb_probe,
    .remove = hi35xx_hifb_remove,
    .driver =
        {
            .name = "hi35xx_hifb",
            .of_match_table = g_hi35xx_hifb_match,
        },
};

osal_module_platform_driver(g_hi35xx_hifb_driver);

MODULE_LICENSE("GPL");
#else
/* for CONFIG_HI_VO_FB_SEPARATE=n in linux */
#ifdef MODULE
module_init(hifb_init);
module_exit(hifb_cleanup);
MODULE_LICENSE("GPL");
#else
subsys_initcall(hifb_init);
#endif
#endif

/* for liteos */
#else

#ifdef MODULE
module_init(hifb_init);
module_exit(hifb_cleanup);
MODULE_LICENSE("GPL");
#else
subsys_initcall(hifb_init);
#endif

#endif
