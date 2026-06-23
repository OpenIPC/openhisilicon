/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_main.h"
#include "ot_debug.h"
#include "mm_ext.h"
#include "securec.h"
#include "drv_tde.h"
#include "gfbg_graphics.h"
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
#include "gfbg_proc.h"
#endif
#include "gfbg.h"
#include "gfbg_blit.h"
#include "gfbg_ext.h"
#include "gfbg_def.h"
#include "gfbg_rotate.h"
#include "gfbg_comm.h"
#include "gfbg_init.h"
#include "sys_ext.h"
#include "proc_ext.h"

#define mkstr(exp) # exp
#define mkmarcotostr(exp) mkstr(exp)

#define GFBG_CMAP_LEN 256
#define GFBG_ROTBUF_NAME_LEN 16

typedef td_s32(*drv_gfbg_ioctl_func)(struct fb_info *info, unsigned long arg);

typedef struct {
    td_u32 cmd;
    drv_gfbg_ioctl_func func;
}drv_gfbg_ioctl_func_item;

#define PAGE_SIZE_ALIGN_MAX ((~0ul - PAGE_SIZE) / 1024)

#ifdef __LITEOS__
#define in_atomic() (TD_FALSE)
#endif

#define GFBG_ALIGNMENT 0xf
#define GFBG_ALIGN     16

#define gfbg_debug_print_vcnt(dev, format, args...) \
    do { \
        td_u32 __vcnt = 0; \
        graphic_drv_get_int_state_vcnt(dev, &__vcnt); \
        gfbg_debug("dev %d cnt %d in "format"\n", dev, __vcnt, ##args); \
    } while (0)

/*
 * the interface to operate the chip
 * Collection of properties and methods,filled in gfbg_init
 */
static gfbg_drv_ops g_drv_ops;

static td_bool g_soft_cursor = TD_FALSE;
static td_bool g_display_on = TD_FALSE;
static td_bool g_gfbg_register = TD_TRUE;
static osal_spinlock g_gfbg_vdp_state_count_lock;
static td_u32 g_gfbg_open_layer_count = 0;
static td_bool g_gfbg_vdp_clk_state = TD_FALSE;
#define VO_INVALID_DEV (-1)
#define VO_DEV_DHD0 0
#define VO_DEV_DHD1 1
#define VO_DEV_DSD0 2

#define GFBG_INTMSK_HD0_VTTHD1 0x1
#define GFBG_INTMSK_HD0_VTTHD2 0x2
#define GFBG_INTMSK_HD0_VTTHD3 0x4
#define GFBG_INTMSK_HD1_VTTHD1 0x10
#define GFBG_INTMSK_HD1_VTTHD2 0x20
#define GFBG_INTMSK_HD1_VTTHD3 0x40
#define GFBG_INTMSK_SD0_VTTHD1 0x100
#define GFBG_INTMSK_SD0_VTTHD2 0x200
#define GFBG_INTMSK_SD0_VTTHD3 0x400

#define DRV_GFBG_IOCTL_CMD_NUM_MAX 154
#define DRV_GFBG_IOCTL_FUNC_ITEM_NUM_MAX 30

static td_s32 gfbg_parse_cfg(td_void);
static td_s32 gfbg_overlay_probe(td_u32 layer_id);
static td_void gfbg_overlay_cleanup(td_u32 layer_id, td_bool unregister);
#ifdef __LITEOS__
typedef gfbg_info fb_info;
typedef fb_cmap_s fb_cmap;
static td_s32 gfbg_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo);
#else
static td_s32 gfbg_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
#endif
static ot_fb_color_format gfbg_getfmtbyargb(const struct fb_bitfield *red, const struct fb_bitfield *green,
                                            const struct fb_bitfield *blue, const struct fb_bitfield *transp,
                                            td_u32 color_depth);
static td_void gfbg_buf_freemem(td_phys_addr_t phyaddr);
static td_phys_addr_t gfbg_buf_allocmem(const td_char *buf_name, td_u32 buf_name_size, td_ulong layer_size,
    const td_char *mmz_name);

static td_s32 gfbg_wait_regconfig_work(td_u32 layer_id);
static td_s32 gfbg_freeccanbuf(gfbg_par *par);
static td_void gfbg_set_dispbufinfo(td_u32 layer_id);
static td_void gfbg_set_bufmode(td_u32 layer_id, ot_fb_layer_buf layer_buf_mode);
static inline td_void gfbg_get_bufmode(const gfbg_par *par, ot_fb_layer_buf *buf_mode);
static td_s32 gfbg_onrefresh(gfbg_par* par, td_void __user *argp);
static td_s32 gfbg_refresh_0buf(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_refresh_1buf(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_refresh_2buf(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_refresh_2buf_immediate_display(td_u32 layer_id, const ot_fb_buf *canvas_buf);
static td_s32 gfbg_set_mirrormode(gfbg_par *par, ot_fb_mirror_mode mirror_mode);
static td_s32 gfbg_set_rotatemode(const struct fb_info *info, ot_fb_rotate_mode rotate_mode);
static td_s32 gfbg_onputlayerinfo(struct fb_info *info, gfbg_par* par, const td_void __user *argp);
static td_void gfbg_get_layerinfo(const gfbg_par *par, ot_fb_layer_info *layer_info);
static inline td_void gfbg_get_antiflickerlevel(const gfbg_par *par, ot_fb_layer_antiflicker_level *antiflicker_level);
static td_void gfbg_set_antiflickerlevel(td_u32 layer_id, ot_fb_layer_antiflicker_level antiflicker_level);
static inline td_void gfbg_get_fmt(const gfbg_par *par, ot_fb_color_format *color_format);
static inline td_void gfbg_set_fmt(gfbg_par *par, ot_fb_color_format color_fmt);
static inline td_void gfbg_set_alpha(gfbg_par *par, const ot_fb_alpha *alpha);
static inline td_void gfbg_get_alpha(const gfbg_par *par, ot_fb_alpha *alpha);
static inline td_void gfbg_set_key(gfbg_par *par, const gfbg_colorkeyex *key);
static inline td_void gfbg_get_key(const gfbg_par *par, gfbg_colorkeyex *key);
static inline td_void gfbg_get_layerpos(const gfbg_par *par, ot_fb_point *pos);
static td_void gfbg_get_screensize(const gfbg_par *par, td_u32 *width, td_u32 *height);
static td_s32 gfbg_set_screensize(gfbg_par *par, const td_u32 *width, const td_u32 *height);
static td_void gfbg_get_maxscreensize(gfbg_par *par, td_u32 *width, td_u32 *height);
static td_void gfbg_get_dispsize(const gfbg_par *par, td_u32 *width, td_u32 *height);
static inline td_void gfbg_get_premul(const gfbg_par *par, td_bool *premul);
static inline td_bool gfbg_get_show(const gfbg_par *par);
static inline td_void gfbg_set_show(gfbg_par *par, td_bool show);
static td_void gfbg_set_layerpos(gfbg_par *par, const ot_fb_point *pos);
static td_void gfbg_tde_rotate_callback(const td_void *paraml, const td_void *paramr);
static td_void gfbg_tde_callback(const td_void *paraml, const td_void *paramr);
static td_s32 gfbg_interrupt_process(td_u32 layer_id);
static td_void gfbg_get_idledispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr);
static td_void gfbg_get_workdispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr);
static inline td_bool gfbg_is_interlace(const gfbg_par *par);

static td_s32 gfbg_read_proc(struct osal_proc_dir_entry *entry);
static td_s32 gfbg_write_proc(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);

static td_s32 drv_gfbg_get_colorkey(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_colorkey(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_alpha(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_layer_alpha(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_screen_origin_pos(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_screen_origin_pos(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_vblank(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_show_layer(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_show_state(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_capability(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_layer_info(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_info(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_canvas_buffer(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_refresh_layer(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_wait_refresh_finish(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_mirror_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_mirror_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_rotate_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_rotate_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_screen_size(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_screen_size(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_flip_surface(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_compression_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_create(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_release(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_set_compression_mode(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_draw_frame(struct fb_info *info, unsigned long arg);

static td_s32 drv_gfbg_set_layer_csc(struct fb_info *info, unsigned long arg);
static td_s32 drv_gfbg_get_layer_csc(struct fb_info *info, unsigned long arg);

static td_s32 g_drv_gfbg_ctl_num[DRV_GFBG_IOCTL_CMD_NUM_MAX] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  0,  0,  0,
    7,  8,  9, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    11, 12, 0, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  0,  0,  0,  0,  0,
    0, 0,  0,  0,  0,  0,  0,  0,  0,  25, 26, 27, 28, 29
};

static drv_gfbg_ioctl_func_item g_drv_gfbg_ioctl_func[DRV_GFBG_IOCTL_FUNC_ITEM_NUM_MAX] = {
    {0, TD_NULL},
    {FBIOGET_COLORKEY_GFBG, drv_gfbg_get_colorkey},
    {FBIOPUT_COLORKEY_GFBG, drv_gfbg_set_colorkey},
    {FBIOGET_ALPHA_GFBG, drv_gfbg_get_layer_alpha},
    {FBIOPUT_ALPHA_GFBG, drv_gfbg_set_layer_alpha},
    {FBIOGET_SCREEN_ORIGIN_GFBG, drv_gfbg_get_screen_origin_pos},
    {FBIOPUT_SCREEN_ORIGIN_GFBG, drv_gfbg_set_screen_origin_pos},
    {FBIOGET_VER_BLANK_GFBG, drv_gfbg_get_vblank},
    {FBIOPUT_SHOW_GFBG, drv_gfbg_show_layer},
    {FBIOGET_SHOW_GFBG, drv_gfbg_get_layer_show_state},
    {FBIOGET_CAPABILITY_GFBG, drv_gfbg_get_capability},
    {FBIOPUT_LAYER_INFO, drv_gfbg_set_layer_info},
    {FBIOGET_LAYER_INFO, drv_gfbg_get_layer_info},
    {FBIOGET_CANVAS_BUF, drv_gfbg_get_canvas_buffer},
    {FBIO_REFRESH, drv_gfbg_refresh_layer},
    {FBIO_WAITFOR_FREFRESH_DONE, drv_gfbg_wait_refresh_finish},
    {FBIOPUT_MIRROR_MODE, drv_gfbg_set_mirror_mode},
    {FBIOGET_MIRROR_MODE, drv_gfbg_get_mirror_mode},
    {FBIOPUT_ROTATE_MODE, drv_gfbg_set_rotate_mode},
    {FBIOGET_ROTATE_MODE, drv_gfbg_get_rotate_mode},
    {FBIOPUT_SCREEN_SIZE, drv_gfbg_set_screen_size},
    {FBIOGET_SCREEN_SIZE, drv_gfbg_get_screen_size},
    {FBIOFLIP_SURFACE, drv_gfbg_flip_surface},
    {FBIOPUT_COMPRESSION_GFBG, drv_gfbg_set_compression_mode},
    {FBIOGET_COMPRESSION_GFBG, drv_gfbg_get_compression_mode},
    {FBIO_CREATE_LAYER, drv_gfbg_create},
    {FBIO_DESTROY_LAYER, drv_gfbg_release},
    {FBIO_DRAW_SMART_RECT, drv_gfbg_draw_frame},
    {FBIOPUT_GRAPHIC_LAYER_CSC_GFBG, drv_gfbg_set_layer_csc},
    {FBIOGET_GRAPHIC_LAYER_CSC_GFBG, drv_gfbg_get_layer_csc},
};

td_s32 graphic_drv_vdp_state_count_lock_init(td_void)
{
    return osal_spin_lock_init(&g_gfbg_vdp_state_count_lock);
}
td_void graphic_drv_vdp_state_count_lock_deinit(td_void)
{
    osal_spin_lock_destroy(&g_gfbg_vdp_state_count_lock);
}

static td_bool is_soft_cursor(td_void)
{
    return g_soft_cursor;
}

static td_bool gfbg_check_memory_enough(const struct fb_info *info, td_u32 stride)
{
    if ((stride * info->var.yres * 2) <= info->fix.smem_len) { /* 2 two buffer */
        return TD_TRUE;
    }
    return TD_FALSE;
}

#ifndef __LITEOS__
static td_phys_addr_t gfbg_get_smem_start(const struct fb_info* info)
{
    return (td_phys_addr_t)info->fix.smem_start;
}

static td_s8 *gfbg_get_screen_base(const struct fb_info* info)
{
    return (td_s8*)info->screen_base;
}

static td_u32 gfbg_get_xres(const struct fb_info* info)
{
    return (td_u32)info->var.xres;
}

static td_u32 gfbg_get_yres(const struct fb_info* info)
{
    return (td_u32)info->var.yres;
}

static td_u32 gfbg_get_xres_virtual(const struct fb_info* info)
{
    return (td_u32)info->var.xres_virtual;
}

static td_u32 gfbg_get_yres_virtual(const struct fb_info* info)
{
    return (td_u32)info->var.yres_virtual;
}

static td_u32 gfbg_get_smem_len(const struct fb_info* info)
{
    return (td_u32)info->fix.smem_len;
}

static td_u32 gfbg_get_line_length(const struct fb_info* info)
{
    return (td_u32)info->fix.line_length;
}

/* when compress to uncompress,use for calculate xoffset,yoffset */
static td_u32 gfbg_get_line_length_ex(const struct fb_info* info)
{
    td_u32 uncompress_stride;
    uncompress_stride = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    if (gfbg_check_memory_enough(info, uncompress_stride) == TD_TRUE) {
        return uncompress_stride;
    } else {
        return (td_u32)info->fix.line_length;
    }
}

static td_u32 gfbg_get_bits_per_pixel(const struct fb_info* info)
{
    return (td_u32)info->var.bits_per_pixel;
}

static td_u32 gfbg_get_yoffset(const struct fb_info* info)
{
    return (td_u32)info->var.yoffset;
}

static td_u32 gfbg_get_xoffset(const struct fb_info* info)
{
    return (td_u32)info->var.xoffset;
}

static struct fb_fix_screeninfo g_default_fix[GFBG_MAX_LAYER_NUM] = {
    {
        .id = "gfbg",                  /* String identifierString identifier */
        .type = FB_TYPE_PACKED_PIXELS, /* FB type */
        .visual = FB_VISUAL_TRUECOLOR,
        .xpanstep = 1,
        .ypanstep = 1,
        .ywrapstep = 0,
        .line_length = GFBG_4K_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_HD_DEF_STRIDE,
        .accel = FB_ACCEL_NONE,
        .mmio_len = 0,
        .mmio_start = 0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_SD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_SD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    },
    {
        .id          = "gfbg",
        .type        = FB_TYPE_PACKED_PIXELS,
        .visual      = FB_VISUAL_TRUECOLOR,
        .xpanstep    =                     1,
        .ypanstep    =                     1,
        .ywrapstep   =                     0,
        .line_length = GFBG_HD_DEF_STRIDE,
        .accel       = FB_ACCEL_NONE,
        .mmio_len    =                     0,
        .mmio_start  =                     0,
    }
};

/* default variable information */
/*
 * Name : g_default_var
 * Desc : default variable info: g_default_var
 * See  : gfbg_open
 */
static struct fb_var_screeninfo g_default_var[GFBG_MAX_LAYER_NUM] = {
    /* for 4K layer */
    {
        .xres           = GFBG_4K_DEF_WIDTH,
        .yres           = GFBG_4K_DEF_HEIGHT,
        .xres_virtual   = GFBG_4K_DEF_WIDTH,
        .yres_virtual   = GFBG_4K_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
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
    /* for HD layer */
    {
        .xres           = GFBG_HD_DEF_WIDTH,
        .yres           = GFBG_HD_DEF_HEIGHT,
        .xres_virtual   = GFBG_HD_DEF_WIDTH,
        .yres_virtual   = GFBG_HD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
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
    /* for SD layer */
    {
        .xres           = GFBG_SD_DEF_WIDTH,
        .yres           = GFBG_SD_DEF_HEIGHT,
        .xres_virtual   = GFBG_SD_DEF_WIDTH,
        .yres_virtual   = GFBG_SD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
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
    /* for AD layer */
    {
        .xres           = GFBG_AD_DEF_WIDTH,
        .yres           = GFBG_AD_DEF_HEIGHT,
        .xres_virtual   = GFBG_AD_DEF_WIDTH,
        .yres_virtual   = GFBG_AD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
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
    /* for cursor layer */
    {
        .xres           = GFBG_CURSOR_DEF_WIDTH,
        .yres           = GFBG_CURSOR_DEF_HEIGHT,
        .xres_virtual   = GFBG_CURSOR_DEF_WIDTH,
        .yres_virtual   = GFBG_CURSOR_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = GFBG_DEF_DEPTH,
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
static td_phys_addr_t gfbg_get_smem_start(const struct gfbg_info* info)
{
    return (td_phys_addr_t)(uintptr_t)info->oinfo.fbmem;
}

static td_s8 *gfbg_get_screen_base(const struct gfbg_info* info)
{
    return (td_s8 *)info->oinfo.fbmem;
}

static td_u32 gfbg_get_xres(const struct gfbg_info* info)
{
    return (td_u32)info->vinfo.xres;
}

static td_u32 gfbg_get_yres(const struct gfbg_info* info)
{
    return (td_u32)info->vinfo.yres;
}

static td_u32 gfbg_get_xres_virtual(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.width;
}

static td_u32 gfbg_get_yres_virtual(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.height;
}

static td_u32 gfbg_get_smem_len(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.fblen;
}

static td_u32 gfbg_get_line_length(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.stride;
}

static td_u32 gfbg_get_bits_per_pixel(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.bpp;
}

static td_u32 gfbg_get_yoffset(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.y;
}

static td_u32 gfbg_get_xoffset(const struct gfbg_info* info)
{
    return (td_u32)info->oinfo.sarea.x;
}

struct gfbg_screeninfo {
    uint8_t format;         /* see FB_FMT_* */
    fb_coord_t xres;        /* Horizontal resolution in pixel columns */
    fb_coord_t yres;        /* Vertical resolution in pixel rows */
    struct fb_area_s sarea; /* Selected area within the overlay */
    fb_coord_t stride;      /* Length of a line in bytes */
    uint8_t bpp;            /* Bits per pixel */
    uint32_t accl;          /* Supported hardware acceleration */
    uint8_t nplanes;        /* Number of color planes supported */
    uint8_t noverlays;      /* Number of overlays supported */
};

static struct gfbg_screeninfo g_default_info[GFBG_MAX_LAYER_NUM] = {
    /* for 4K layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_4K_DEF_WIDTH,
        .yres = GFBG_4K_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_4K_DEF_WIDTH, GFBG_4K_DEF_HEIGHT},
        .stride = GFBG_4K_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for HD layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_HD_DEF_WIDTH,
        .yres = GFBG_HD_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_HD_DEF_WIDTH, GFBG_HD_DEF_HEIGHT},
        .stride = GFBG_HD_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for SD layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_SD_DEF_WIDTH,
        .yres = GFBG_SD_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_SD_DEF_WIDTH, GFBG_SD_DEF_HEIGHT},
        .stride = GFBG_SD_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for AD layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_AD_DEF_WIDTH,
        .yres = GFBG_AD_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_AD_DEF_WIDTH, GFBG_AD_DEF_HEIGHT},
        .stride = GFBG_AD_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
    /* for cursor layer */
    {
        .format = OT_FB_FORMAT_ARGB1555,
        .xres = GFBG_CURSOR_DEF_WIDTH,
        .yres = GFBG_CURSOR_DEF_HEIGHT,
        .sarea = {0, 0, GFBG_CURSOR_DEF_WIDTH, GFBG_CURSOR_DEF_HEIGHT},
        .stride = GFBG_CURSOR_DEF_STRIDE,
        .bpp = GFBG_DEF_DEPTH,
        .accl = 0,
        .nplanes = 1,
        .noverlays = 1,
    },
};

static void gfbg_screeninfo_init(struct gfbg_info *info, struct gfbg_screeninfo *sinfo)
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
 * Desc : bit fields of each color format in GFBG_COLOR_FMT_E,
 *        the order must be the same as that of GFBG_COLOR_FMT_E
 */
static gfbg_argb_bitinfo g_argb_bit_field[] = {
    /* RGB565 */
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

static td_s32 tde_cal_scale_rect_gfbg(const drv_tde_rect* src_rect, const drv_tde_rect* dst_rect,
                                      const drv_tde_rect* rect_in_src, drv_tde_rect* rect_in_dst)
{
    rect_in_dst->pos_x = rect_in_src->pos_x * dst_rect->width / src_rect->width;
    rect_in_dst->pos_y = rect_in_src->pos_y * dst_rect->height / src_rect->height;
    rect_in_dst->height = dst_rect->height;
    rect_in_dst->width = dst_rect->width;

    return TD_SUCCESS;
}

static td_void gfbg_version(td_void);
static td_s32 gfbg_refresh(td_u32 layer_id, const ot_fb_buf *canvas_buf, ot_fb_layer_buf buf_mode);
static td_void gfbg_select_antiflicker_mode(gfbg_par *par);
static td_s32 gfbg_setcolreg(unsigned regno, unsigned red, unsigned green,
                             unsigned blue, unsigned transp, struct fb_info *info);

gfbg_layer g_layer[GFBG_MAX_LAYER_NUM];
char *g_video; /* 128 for vram length */

static char g_display[8] = "off";   /* 8:open vo or not */
static char g_softcursor[8] = "off";   /* 8:use soft cursor or not */

td_void set_video_name(char *temp_video)
{
    g_video = temp_video;
}

static td_s32 gfbg_drv_set_mmz_addr(td_phys_addr_t phys_addr, td_u32 phys_len, td_u32 layer_id)
{
    td_s32 ret;
    ot_tde_export_func *tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_set_gfbg_mmz_addr == TD_NULL)) {
        gfbg_error("TDE tde_export_func is NULL!\n");
        return TD_FAILURE;
    }

    ret = tde_export_func->drv_tde_module_set_gfbg_mmz_addr(phys_addr, phys_len, layer_id);
    if (ret != TD_SUCCESS) {
        osal_printk("GFBG Warning: TDE set gfbg mmz addr failed\n");
        return ret;
    }

    return TD_SUCCESS;
}

/*
 * Name : gfbg_fill_data
 * Desc : fill buffer, it is faster than memset_s.
 * See  : Called by gfbg_overlay_probe
 */
static td_u32 gfbg_fill_data(const struct fb_info* info, td_u32 fill_data, size_t n)
{
    ot_fb_buf dst_img;
    td_s8 *rest = TD_NULL;
    const td_u32 can_pitch = GFBG_TDE_MEMSET_WIDTH * 4; /* 4 alg data */

    /*
     * Assume that the image width is 3840, the format is ARBG8888, and the image is filled by TDE.
     * Since the size of the image that TDE can handle is limited,
     * the width and height are limited to GFBG_TDE_MEMSET_WIDTH
     * and GFBG_TDE_MEMSET_HEIGHT, Exceeded parts are cleared using memset_s
     * Configure the target image, pitch, format, width and height, physical address (pointing to the memory start
     * address)
     */
    dst_img.canvas.phys_addr = gfbg_get_smem_start(info);
    dst_img.canvas.pitch = can_pitch;
    dst_img.canvas.format = OT_FB_FORMAT_ABGR8888;
    dst_img.canvas.width = GFBG_TDE_MEMSET_WIDTH;
    dst_img.canvas.height = n / can_pitch;

    if (dst_img.canvas.height > GFBG_TDE_MEMSET_HEIGHT) {
        dst_img.canvas.height = GFBG_TDE_MEMSET_HEIGHT;
    }
    if (dst_img.canvas.height != 0) {
        if (gfbg_drv_fill(&dst_img, fill_data) != TD_SUCCESS) {
            gfbg_error("gfbg_drv_fill fail!\n");
            return TD_FAILURE;
        }
    }

    /* The memory not in the canvas is set by memset_s */
    if ((n - dst_img.canvas.height * can_pitch) != 0) {
        rest = gfbg_get_screen_base(info) +  can_pitch * (dst_img.canvas.height);
        (td_void)memset_s(rest, n - dst_img.canvas.height * can_pitch, fill_data,
            n - dst_img.canvas.height * can_pitch);
    }
    return TD_SUCCESS;
}

/*
 * Name : gfbg_set_dcmp_info
 * Desc : set the decompression info.
 */
static td_void gfbg_set_dcmp_info(struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)(info->par);
    volatile gfbg_compress_info *compress_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    gfbg_graphic_dcmp_info dcmp_info;
    ot_fb_color_format gfbg_color_fmt;

    if (par == TD_NULL) {
        return;
    }

    compress_info = &par->compress_info;
    refresh_info  = &par->refresh_info;
    display_info = &par->display_info;

    if ((refresh_info->screen_addr == 0) || (refresh_info->gb_screen_addr == 0)) {
        /* close the dcmp */
        g_drv_ops.gfbg_drv_enable_dcmp(par->layer_id, TD_FALSE);
        gfbg_warning("Compression is opened, but compressed buffer phyaddr for refreshing is NULL(0)\n");
        return;
    }

    /*
     * Decompressing information from compressed information: compression channels AR, GB address, and its size size0,
     * size1
     */
    gfbg_color_fmt = refresh_info->user_buffer.canvas.format;
    switch (gfbg_color_fmt) {
        case OT_FB_FORMAT_ARGB4444:
            dcmp_info.pixel_fmt = GFBG_INPUTFMT_ARGB_4444;
            break;
        case OT_FB_FORMAT_ARGB1555:
            dcmp_info.pixel_fmt = GFBG_INPUTFMT_ARGB_1555;
            break;
        case OT_FB_FORMAT_ARGB8888:
            dcmp_info.pixel_fmt = GFBG_INPUTFMT_ARGB_8888;
            break;
        default:
            gfbg_error("Pixel format(%d) is invalid!\n", gfbg_color_fmt);
            return;
    }
    dcmp_info.ar_phy_addr = gfbg_drv_get_dcmp_offset_addr(refresh_info->screen_addr);
    dcmp_info.gb_phy_addr = gfbg_drv_get_dcmp_offset_addr(refresh_info->gb_screen_addr);
    dcmp_info.width = display_info->display_width;
    dcmp_info.height = display_info->display_height;

    dcmp_info.frame_size0 = compress_info->frame_size0;
    dcmp_info.frame_size1 = compress_info->frame_size1;
    dcmp_info.is_lossless_a = TD_FALSE; /* Whether lossless,FALSE:Lossy,TRUE:lossless */
    dcmp_info.is_lossless = TD_FALSE;  /* Whether lossless,FALSE:Lossy,TRUE:lossless */

    /* Decompressing information to drv level */
    if (g_drv_ops.gfbg_drv_set_dcmp_info(par->layer_id, &dcmp_info) != TD_SUCCESS) {
        /* close dcmp */
        g_drv_ops.gfbg_drv_enable_dcmp(par->layer_id, TD_FALSE);
    }
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    /* for compress, we should replace the stride */
    info->fix.line_length = dcmp_info.stride;
#endif
}

static int gfbg_interrupt_route(td_s32 irq, td_void *dev_id);

static td_s32 drv_gfbg_alloc_cmap(td_u32 layer_id)
{
    const td_u32 cmap_len = 256;
    struct fb_info *info = g_layer[layer_id].info;

    if (info == TD_NULL) {
        gfbg_error("drv_gfbg_alloc_cmap failed\n");
        return TD_FAILURE;
    }

    if (g_drv_ops.capability[layer_id].is_cmap != TD_TRUE) {
        return TD_FAILURE;
    }

    if (fb_alloc_cmap(&info->cmap, cmap_len, 1) < 0) {
        info->cmap.len = 0;
        gfbg_error("fb_alloc_cmap failed\n");
        return TD_FAILURE;
    }

    info->cmap.len = cmap_len;

    return TD_SUCCESS;
}

static td_void drv_gfbg_free_cmap(td_u32 layer_id)
{
    struct fb_cmap *cmap = NULL;
    struct fb_info *info = g_layer[layer_id].info;

    if (info == TD_NULL) {
        return;
    }

    cmap = &info->cmap;
    if (cmap->len != 0) {
        fb_dealloc_cmap(cmap);
        info->cmap.len = 0;
    }

    return;
}

static td_void fb_increase_open_cnt(td_void)
{
    unsigned long lock_flag;
    osal_spin_lock_irqsave(&g_gfbg_vdp_state_count_lock, &lock_flag);
    g_gfbg_open_layer_count++;
    osal_spin_unlock_irqrestore(&g_gfbg_vdp_state_count_lock, &lock_flag);
}

static td_void fb_decrease_open_cnt(td_void)
{
    unsigned long lock_flag;
    osal_spin_lock_irqsave(&g_gfbg_vdp_state_count_lock, &lock_flag);
    g_gfbg_open_layer_count--;
    osal_spin_unlock_irqrestore(&g_gfbg_vdp_state_count_lock, &lock_flag);
}

static td_bool fb_osi_get_fb_open_state(td_void)
{
    return (g_gfbg_open_layer_count > 0) ? TD_TRUE : TD_FALSE;
}

static td_void fb_osi_set_vdp_clk_state(td_bool enable)
{
    unsigned long lock_flag;
    osal_spin_lock_irqsave(&g_gfbg_vdp_state_count_lock, &lock_flag);
    g_gfbg_vdp_clk_state = enable;
    osal_spin_unlock_irqrestore(&g_gfbg_vdp_state_count_lock, &lock_flag);
}

static td_bool fb_get_vdp_clk_state(td_void)
{
    return g_gfbg_vdp_clk_state;
}

static ot_fb_export_func g_gfbg_export_func = {
    .fb_get_fb_open_state = fb_osi_get_fb_open_state,
    .fb_set_vdp_clk_state = fb_osi_set_vdp_clk_state,
};

static td_s32 gfbg_cmpi_init(td_void *args)
{
    ot_unused(args);
    return 0;
}
static td_void gfbg_cmpi_exit(td_void)
{
    return;
}
static td_void gfbg_cmpi_notify(mod_notice_id notice)
{
    ot_unused(notice);
    return;
}
static td_void gfbg_cmpi_query_state(mod_state *state)
{
    if (state == TD_NULL) {
        return;
    }
    *state = MOD_STATE_FREE;
    return;
}
static td_u32 gfbg_cmpi_get_ver_magic(td_void)
{
    return VERSION_MAGIC;
}
static umap_module g_gfbg_module = {
    .mod_id = OT_ID_FB,
    .mod_name = "gfbg",
    .pfn_init = gfbg_cmpi_init,
    .pfn_exit = gfbg_cmpi_exit,
    .pfn_query_state = gfbg_cmpi_query_state,
    .pfn_notify = gfbg_cmpi_notify,
    .pfn_ver_checker = gfbg_cmpi_get_ver_magic,
    .export_funcs = &g_gfbg_export_func,
    .data = TD_NULL,
};

static td_s32 gfbg_register(td_u32 index)
{
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    const td_char *entry_name[5] = { /* 5 max */
        "gfbg0",
        "gfbg1",
        "gfbg2",
        "gfbg3",
        "gfbg4"
    };
#endif
    td_s32 ret;
    ret = gfbg_overlay_probe(index);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = drv_gfbg_alloc_cmap(index);
    if (ret != TD_SUCCESS) {
        /* clean up over layer */
        gfbg_overlay_cleanup(index, TD_TRUE);
        return TD_FAILURE;
    }

    /* create a proc entry in 'gfbg' for the layer */
    gfbg_proc_add_module(entry_name[index], gfbg_read_proc, gfbg_write_proc, TD_NULL, g_layer[index].info);

    return TD_SUCCESS;
}

static td_void gfbg_unregister(td_u32 index)
{
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    const td_char *entry_name[5] = { /* 5 max */
        "gfbg0",
        "gfbg1",
        "gfbg2",
        "gfbg3",
        "gfbg4"
    };
#endif
    if (g_drv_ops.capability[index].is_layer_support == TD_FALSE) {
        return;
    }
    drv_gfbg_free_cmap(index);
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    /* destroy a proc entry in 'gfbg' for the layer */
    gfbg_proc_remove_module(entry_name[index]);
#endif
    /* unregister the layer */
    gfbg_overlay_cleanup(index, TD_TRUE);

    return;
}

static td_s32 gfbg_init_register(td_void)
{
    td_s32 i, j;
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    gfbg_proc_init();
#endif
    /* initialize fb file according the config */
    for (i = 0; i < g_drv_ops.layer_count; i++) {
        if (g_drv_ops.capability[i].is_layer_support == TD_FALSE) {
            continue;
        }

        if (gfbg_register(i) != TD_SUCCESS) {
            for (j = i - 1; j >= 0; j--) {
                gfbg_unregister(j);
            }
            osal_printk("ERROR: Load gfbg.ko ....FAILED!\n");
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void gfbg_deinit_register(td_void)
{
    td_u32 i;

    for (i = 0; i < g_drv_ops.layer_count; i++) {
        gfbg_unregister(i);
    }
    return;
}

static td_void gfbg_deinit_process(td_void)
{
    g_drv_ops.gfbg_drv_deinit();

    gfbg_drv_set_tde_callback(TD_NULL);
    gfbg_drv_set_tde_rotate_callback(TD_NULL);
    return;
}

static td_s32 gfbg_init_process(td_void)
{
    /* initial adoption layer */
    if (g_drv_ops.gfbg_drv_init() != TD_SUCCESS) {
        gfbg_error("drv init failed\n");
        return TD_FAILURE;
    }

    /* register rotation */
    gfbg_rotation_register();

    gfbg_drv_set_tde_callback(gfbg_tde_callback);
    gfbg_drv_set_tde_rotate_callback(gfbg_tde_rotate_callback);

    return TD_SUCCESS;
}

#ifdef __LITEOS__
td_s32 gfbg_init(td_void* args)
{
    if (gfbg_get_module_para(args) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* get the chip operation method */
    gfbg_drv_get_ops(&g_drv_ops);
    if (cmpi_register_module(&g_gfbg_module)) {
        gfbg_error("cmpi_register_module for Gfbg failure!\n");
        return TD_FAILURE;
    }

    /* parse the \arg video && g_softcursor && display string */
    if (gfbg_parse_cfg() < 0) {
        gfbg_error("Usage: insmod gfbg.ko video=\"gfbg: vrami_size:xxx,vramj_size: xxx, ...\"\n");
        gfbg_error("i, j means layer id, xxx means layer size in kbytes!\n");
        gfbg_error("example: insmod gfbg.ko video=\"gfbg: vram0_size: 810, vram1_size: 810\"\n\n");
        goto ERR0;
    }

    /* initialize fb file according the config */
    if (gfbg_init_register() != TD_SUCCESS) {
        goto ERR0;
    }

    /* open vo and initial layer */
    if (gfbg_init_process() != TD_SUCCESS) {
        goto ERR1;
    }

    /* show version */
    gfbg_version();

    osal_printk("load gfbg.ko ....OK!\n");

    return TD_SUCCESS;

ERR1:
    /* do with gfbg init err1 */
    gfbg_deinit_register();
ERR0:
    cmpi_unregister_module(OT_ID_FB);
    return TD_FAILURE;
}
#else
td_s32 gfbg_init(td_void)
{
    if (ckfn_sys_entry() == TD_FALSE) {
        gfbg_error("no sys ko!\n");
        return TD_FAILURE;
    }

    /* get the chip operation method */
    gfbg_drv_get_ops(&g_drv_ops);

    if (cmpi_register_module(&g_gfbg_module)) {
        gfbg_error("cmpi_register_module for Gfbg failure!\n");
        g_gfbg_register = TD_FALSE;
        return TD_FAILURE;
    }

    /* parse the \arg video && g_softcursor && display string */
    if (gfbg_parse_cfg() < 0) {
        gfbg_error("Usage: insmod gfbg.ko video=\"gfbg: vrami_size:xxx, vramj_size:xxx, ...\"\n");
        gfbg_error("i, j means layer id, xxx means layer size in kbytes!\n");
        osal_printk("ERROR: Load gfbg.ko ....FAILED!\n");
        goto err0;
    }

    /* initialize fb file according the config */
    if (gfbg_init_register() != TD_SUCCESS) {
        goto err0;
    }

    /* open vo and initial layer */
    if (gfbg_init_process() != TD_SUCCESS) {
        goto err1;
    }

    /*
    * Interrupt registration interrupt is placed at the end.
    * Since the interrupt will be opened in the boot screen,
    * it will respond when loading ko, and the context is used in the interrupt.
    */
    if (osal_irq_request(*(fb_get_gfbg_irq()), gfbg_interrupt_route, TD_NULL, "GFBG Int", gfbg_interrupt_route)) {
        gfbg_error("request_irq for Gfbg failure!\n");
        goto err2;
    }

    /* show version */
    gfbg_version();
    osal_printk("load gfbg.ko ....OK!\n");

    return TD_SUCCESS;

err2:
    gfbg_deinit_process();
err1:
    gfbg_deinit_register();
err0:
    cmpi_unregister_module(OT_ID_FB);
    g_gfbg_register = TD_FALSE;
    osal_printk("ERROR: Load gfbg.ko ....FAILED!\n");
    return TD_FAILURE;
}
#endif

td_void gfbg_cleanup(td_void)
{
    if (g_gfbg_register) {
        osal_irq_free(*(fb_get_gfbg_irq()), gfbg_interrupt_route);
        gfbg_deinit_process();
        gfbg_deinit_register();
    }
    cmpi_unregister_module(OT_ID_FB);
    osal_printk("unload gfbg.ko ....OK!\n");
    return;
}

static td_s32 gfbg_interrupt(td_void)
{
    td_u32 int_status = 0;
    ot_vo_dev vo_dev = VO_INVALID_DEV;
    td_s32 i;
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    td_u32 vtthd_flag = 0x0;
    td_u32 frm_start_flag = 0x0;

    g_drv_ops.gfbg_drv_graphics_get_int(&int_status);
    /*
     * GFBG_DRV_GraphicsClearINTStatus will clear all reported interrupts.
     * If devices' interrupt are reported at the same time,
     * Will cause only one device to be processed, the interruption of other devices is ignored,
     * so it cannot be cleared here.
     * All reported interrupts can only be cleared one by one in GFBG_DRV_GraphicsGetINTDev
     */
    gfbg_debug("intstatus : %d\n", int_status);
    if (g_drv_ops.gfbg_drv_graphics_get_int_dev(int_status, &vo_dev) != TD_SUCCESS) {
        return OSAL_IRQ_HANDLED;
    }

    if (vo_dev == VO_INVALID_DEV) {
        gfbg_error("unknown dev:%d \n", vo_dev);
        return OSAL_IRQ_HANDLED;
    }

    /* Handling all graphics layers on this device */
    for (i = 0; i < GFBG_MAX_LAYER_NUM; i++) {
        if (g_drv_ops.capability[i].is_layer_support == TD_FALSE) {
            continue;
        }
        info = g_layer[i].info;
        par = (gfbg_par *)(info->par);
        /* If the layer is not open, do not act. if the layer is bound to the device. */
        if ((par->layer_open == TD_FALSE) || (vo_dev != graphic_drv_get_bind_dev(i))) {
            continue;
        }

        if (vo_dev == VO_DEV_DHD0) {
            vtthd_flag    = GFBG_INTMSK_HD0_VTTHD1;
            frm_start_flag = GFBG_INTMSK_HD0_VTTHD3;
        } else if (vo_dev == VO_DEV_DHD1) {
            vtthd_flag    = GFBG_INTMSK_HD1_VTTHD1;
            frm_start_flag = GFBG_INTMSK_HD1_VTTHD3;
        } else if (vo_dev == VO_DEV_DSD0) {
            vtthd_flag    = GFBG_INTMSK_SD0_VTTHD1;
            frm_start_flag = GFBG_INTMSK_SD0_VTTHD3;
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
        if (int_status & vtthd_flag) {  /* VO vertical timing interrupt */
            gfbg_debug_print_vcnt(vo_dev, "vtth1");
            par->refresh_info.do_refresh_job = TD_FALSE;
            gfbg_interrupt_process(i);
        } else if (int_status & frm_start_flag) {  /* VO frame start interrupt */
            gfbg_debug_print_vcnt(vo_dev, "vtth3");
            par->refresh_info.do_refresh_job = TD_TRUE;
            wake_up(&(par->do_refresh_job));
        }
    }
    return OSAL_IRQ_HANDLED;
}
static int gfbg_interrupt_route(td_s32 irq, td_void *dev_id)
{
    ot_unused(irq);
    ot_unused(dev_id);
    return gfbg_interrupt();
}

static td_s32 gfbg_open_check_param(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    if ((info == TD_NULL) || (info->par == TD_NULL)) {
        return TD_FAILURE;
    }

    par = (gfbg_par *)info->par;
    if (par->layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer %u is not supported!\n", par->layer_id);
        return TD_FAILURE;
    }

    /* assure layer is legal */
    if (!g_drv_ops.capability[par->layer_id].is_layer_support) {
        gfbg_error("layer %u is not supported!\n", par->layer_id);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_bool gfbg_open_is_cursor_layer(struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    ot_fb_alpha alpha = {
        TD_TRUE, TD_FALSE, GFBG_ALPHA_TRANSPARENT,
        GFBG_ALPHA_OPAQUE, GFBG_ALPHA_OPAQUE
    };

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        if (!atomic_read(&par->ref_count)) {
            (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));

            par->layer_id = layer_id;
            atomic_set(&par->ref_count, 0);

            /* Configuring alpha properties */
            gfbg_set_alpha(par, &alpha);
        }
        atomic_inc(&par->ref_count);

        return TD_TRUE;
    }
    return TD_FALSE;
}

#ifdef __LITEOS__
static td_s32 gfbg_open_default_param(struct fb_info *info, ot_fb_color_format *color_format)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_4k_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_4K]);
    } else if (is_hd_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_HD]);
    } else if (is_sd_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_SD]);
    } else if (is_ad_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_AD]);
    } else if (is_cursor_layer(layer_id)) {
        gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_CURSOR]);
    } else {
        gfbg_error("error layer id:%u\n", par->layer_id);
    }
    *color_format = info->vinfo.format;
    if (*color_format == OT_FB_FORMAT_BUTT) {
        gfbg_error("Invalid default color format!\n");
        return TD_FAILURE;
    }

    info->oinfo.stride = (info->oinfo.sarea.width * (info->oinfo.bpp >> 3) + GFBG_ALIGNMENT) & /* 8 is bits 2^3 */
                          (~GFBG_ALIGNMENT);

    return TD_SUCCESS;
}
#else
static td_s32 gfbg_open_default_param(struct fb_info *info, ot_fb_color_format *color_format)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    struct fb_var_screeninfo *var = TD_NULL;
    struct fb_fix_screeninfo *fix = TD_NULL;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    gfbg_stride_attr attr = {0};
#endif
    var = &info->var;
    fix = &info->fix;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_4k_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_4K];
    } else if (is_hd_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_HD];
    } else if (is_sd_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_SD];
    } else if (is_ad_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_AD];
    } else if (is_cursor_layer(layer_id)) {
        *var = g_default_var[GFBG_LAYER_TYPE_CURSOR];
    } else {
        gfbg_error("error layer id:%u\n", par->layer_id);
    }
    /* transform colorfmt form bitfiled to gfbg format, and record it */
    *color_format = gfbg_getfmtbyargb(&var->red, &var->green, &var->blue,
        &var->transp, var->bits_per_pixel);
    if (*color_format == OT_FB_FORMAT_BUTT) {
        gfbg_error("Invalid default color format!\n");
        return TD_FAILURE;
    }
    /* By default it is width*4 (32 bits per pixel / 8) */
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = var->xres_virtual;
    attr.format = *color_format;
    /* 3 for 8bits */
    fix->line_length = (var->xres_virtual * (var->bits_per_pixel >> 3) + GFBG_ALIGNMENT) &
                        (~GFBG_ALIGNMENT);
    gfbg_recalculate_stride(NULL, &(fix->line_length), &attr);
#else
    /* 3 for 8bits */
    fix->line_length = (var->xres_virtual * (var->bits_per_pixel >> 3) + GFBG_ALIGNMENT) &
                        (~GFBG_ALIGNMENT);
#endif
    return TD_SUCCESS;
}
#endif

static td_void gfbg_open_init_display(const struct fb_info *info, td_u32 layer_id, const gfbg_osd_data *osd_data)
{
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    par = (gfbg_par *)info->par;

    (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));
    par->layer_id = layer_id;
    atomic_set(&par->ref_count, 0);

    display_info = &par->display_info;
    display_info->display_width    = gfbg_get_xres(info);
    display_info->display_height   = gfbg_get_yres(info);
    display_info->screen_width     = gfbg_get_xres(info);
    display_info->screen_height    = gfbg_get_yres(info);
    display_info->vir_x_res         = gfbg_get_xres_virtual(info);
    display_info->vir_y_res         = gfbg_get_yres_virtual(info);
    display_info->x_res            = gfbg_get_xres(info);
    display_info->y_res            = gfbg_get_yres(info);
    display_info->max_screen_width  = osd_data->screen_width;
    display_info->max_screen_height = osd_data->screen_height;
    display_info->mirror_mode       = OT_FB_MIRROR_NONE;
    display_info->rotate_mode       = OT_FB_ROTATE_NONE;
    par->rotate_vb = 0;

    init_waitqueue_head(&(par->vbl_event));
    init_waitqueue_head(&(par->do_refresh_job));

    return;
}

static td_void gfbg_open_init_config(const struct fb_info *info, td_u32 layer_id, gfbg_osd_data *osd_data,
                                     ot_fb_color_format color_format, gfbg_cursor_info *cursor_info)
{
    gfbg_par *par = TD_NULL;
    ot_fb_alpha alpha = {
        TD_TRUE, TD_FALSE, GFBG_ALPHA_TRANSPARENT,
        GFBG_ALPHA_OPAQUE, GFBG_ALPHA_OPAQUE
    };
    gfbg_display_info *display_info = TD_NULL;
    unsigned long lock_flag;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    ot_unused(cursor_info);

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    gfbg_set_bufmode(par->layer_id, OT_FB_LAYER_BUF_BUTT);
    gfbg_set_alpha(par, &alpha);
    gfbg_set_dispbufinfo(layer_id);
    gfbg_set_fmt(par, color_format);
    /* Anti-flicker when interlaced */
    display_info->need_antiflicker = (osd_data->scan_mode == GFBG_SCANMODE_I) ? (TD_TRUE) : (TD_FALSE);
    gfbg_set_antiflickerlevel(par->layer_id, OT_FB_LAYER_ANTIFLICKER_AUTO);

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    g_drv_ops.gfbg_drv_layer_default_setting(layer_id);
    g_drv_ops.gfbg_drv_set_layer_alpha(layer_id, par->alpha);
    g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, par->color_format);
}

static td_void gfbg_open_init_compress(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_rect rect;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    layer_id = par->layer_id;
    compress_info = &par->compress_info;
    refresh_info = &par->refresh_info;

    /* Decompression information settings */
    compress_info->new_start_section = compress_info->start_section;
    compress_info->new_zone_nums = compress_info->zone_nums;
    compress_info->clear_zone = TD_FALSE;

    g_drv_ops.gfbg_drv_enable_dcmp(layer_id, TD_FALSE); /* Clear decompression status */

    rect.x = 0;
    rect.y = 0;
    rect.width = (td_s32)gfbg_get_xres(info);
    rect.height = (td_s32)gfbg_get_yres(info);

    g_drv_ops.gfbg_drv_set_layer_rect(par->layer_id, &rect, &rect);
    g_drv_ops.gfbg_drv_set_layer_src_image_reso(par->layer_id, &rect);

    g_drv_ops.gfbg_drv_set_layer_stride(par->layer_id, gfbg_get_line_length(info));

    g_drv_ops.gfbg_drv_set_pre_mul(par->layer_id, display_info->is_premul);
    if (g_drv_ops.capability[layer_id].is_key_rgb || g_drv_ops.capability[layer_id].is_key_alpha) {
        g_drv_ops.gfbg_drv_set_layer_key_mask(par->layer_id, &par->ckey);
    }

    /* Set to memory address */
    g_drv_ops.gfbg_drv_set_layer_addr(par->layer_id, gfbg_get_smem_start(info));
    refresh_info->screen_addr = gfbg_get_smem_start(info);
    refresh_info->gb_screen_addr = 0x0; /* Clear the decompression address */
    compress_info->layer_addr_update = TD_TRUE;

    return;
}

static td_void gfbg_open_init_finish(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    unsigned long lock_flag;

    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);

    compress_info->compress_open = TD_FALSE;
    compress_info->is_losslessa = TD_FALSE;
    compress_info->is_lossless = TD_FALSE;

    par->modifying = TD_TRUE;
    gfbg_set_show(par, TD_TRUE);
    par->param_modify_mask = GFBG_LAYER_PARAMODIFY_SHOW;
    par->modifying = TD_FALSE;
    par->layer_open = TD_TRUE;

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    /* enable layer for vo to decide config csc or not */
    g_drv_ops.gfbg_drv_set_layer_enable(par->layer_id, TD_TRUE);

    return;
}

static td_void gfbg_open_init_information(const struct fb_info *info, td_u32 layer_id, gfbg_osd_data *osd_data,
                                          ot_fb_color_format color_format, gfbg_cursor_info *cursor_info)
{
    /* gfbg set bufmode\alpha\displaybufinfo\fmt */
    gfbg_open_init_config(info, layer_id, osd_data, color_format, cursor_info);

    /* gfbg compress init */
    gfbg_open_init_compress(info);

    /* gfbg set callback and init finish */
    gfbg_open_init_finish(info);

    return;
}

static td_s32 gfbg_open_start(struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    gfbg_cursor_info cursor_info;
    gfbg_osd_data osd_data = {0};
    td_u32 layer_id;
    ot_fb_color_format color_format;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer_id is [%u] and more than max[%u]\n", layer_id, GFBG_MAX_LAYER_NUM);
        return TD_FAILURE;
    }

    if (!atomic_read(&par->ref_count)) {
        fb_increase_open_cnt();
        if (fb_get_vdp_clk_state() != TD_TRUE) {
            fb_decrease_open_cnt();
            gfbg_error("vo apb clk is close!\n");
            return TD_FAILURE;
        }
        fb_graphics_init();
        if (graphic_drv_dev_gfbg_int_enable(layer_id, TD_TRUE) != TD_SUCCESS) {
            fb_decrease_open_cnt();
            gfbg_error("set graphic layer %u# vointmsk failed!\n", layer_id);
            return TD_FAILURE;
        }

        (td_void)memset_s(&cursor_info, sizeof(gfbg_cursor_info), 0, sizeof(gfbg_cursor_info));
        if (g_drv_ops.gfbg_drv_get_osd_data(layer_id, &osd_data) != TD_SUCCESS) {
            fb_decrease_open_cnt();
            gfbg_error("Failed to get osd data!\n");
            return TD_FAILURE;
        }

        /* Configure the layer's default variable parameter var_info according to the type of layer */
        if (g_drv_ops.gfbg_open_layer(layer_id) != TD_SUCCESS) {
            fb_decrease_open_cnt();
            gfbg_error("Open graphic layer %u# failed!\n", layer_id);
            return TD_FAILURE;
        }

        /* get default var and fix */
        if (gfbg_open_default_param(info, &color_format) != TD_SUCCESS) {
            fb_decrease_open_cnt();
            return TD_FAILURE;
        }

        /* Initialize the display information in private data */
        gfbg_open_init_display(info, layer_id, &osd_data);

        /* Initialization lock */
        if (osal_spin_lock_init(&par->lock) != TD_SUCCESS) {
            fb_decrease_open_cnt();
            return TD_FAILURE;
        }

        gfbg_open_init_information(info, layer_id, &osd_data, color_format, &cursor_info);
    }
    return TD_SUCCESS;
}

/*
 * Function        : gfbg_open
 * Description     : open the framebuffer and using the default parameter to set the layer
                     struct fb_info *info
 * Return          : return 0
 */
#ifdef __LITEOS__
void *g_gfbg_info_ptr = TD_NULL;
static td_s32 gfbg_open(struct fb_vtable_s *vtable)
#else
static td_s32 gfbg_open(struct fb_info *info, td_s32 user)
#endif
{
#ifdef __LITEOS__
    struct gfbg_info *info = (struct gfbg_info *)vtable;
#endif
    gfbg_par *par = TD_NULL;
    ot_unused(user);

    /* check input param */
    if (gfbg_open_check_param(info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    /* check layer is cursor and is soft cursor */
    if (gfbg_open_is_cursor_layer(info) == TD_TRUE) {
        return TD_SUCCESS;
    }

    /* open the layer first */
    if (gfbg_open_start(info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* increase reference count */
    atomic_inc(&par->ref_count);
#ifdef __LITEOS__
    g_gfbg_info_ptr = (void *)info;
#endif
    return TD_SUCCESS;
}

/*
 * Function        : gfbg_release
 * Description     : open the framebuffer and disable the layer
                     struct fb_info *info
 * Return          : return 0 if succeed, otherwise return -EINVAL
 */
static td_void gfbg_release_info(gfbg_par *par)
{
    td_u32 layer_id;
    unsigned long lock_flag;

    if ((g_drv_ops.gfbg_drv_set_layer_enable == TD_NULL) || (g_drv_ops.gfbg_drv_updata_layer_reg == TD_NULL) ||
        (g_drv_ops.gfbg_drv_set_int_callback == TD_NULL) || (g_drv_ops.gfbg_close_smart_rect == TD_NULL) ||
        (g_drv_ops.gfbg_close_layer == TD_NULL)) {
        gfbg_error("gfbg_release_info failed!\n");
        return;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    layer_id = par->layer_id;
    gfbg_set_show(par, TD_FALSE);

    if ((!is_cursor_layer(par->layer_id)) || (!is_soft_cursor())) {
        /* disable the compress */
        if (par->compress_info.compress_open) {
            /* No memory alloc, no need to release */
            par->compress_info.compress_open = TD_FALSE;
            par->compress_info.is_losslessa = TD_FALSE;
            par->compress_info.is_lossless = TD_FALSE;
        }

        /* clear wbc interrupt when close fb */
        g_drv_ops.gfbg_drv_set_layer_enable(par->layer_id, TD_FALSE);
        g_drv_ops.gfbg_drv_updata_layer_reg(par->layer_id);

        /* vo has spin lock, you need to release the spin lock first */
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        g_drv_ops.gfbg_drv_set_int_callback(GFBG_INTTYPE_VO, TD_NULL, layer_id, TD_NULL);
        osal_spin_lock_irqsave(&par->lock, &lock_flag);

        g_drv_ops.gfbg_close_smart_rect(par->layer_id);
        g_drv_ops.gfbg_close_layer(par->layer_id);

        par->compress_info.start_section = 0;
        par->compress_info.zone_nums = 0;
        par->compress_info.new_start_section = 0;
        par->compress_info.new_zone_nums = 0;
        par->compress_info.clear_zone = TD_TRUE;

        /* MMzFree in gfbg_freeccanbuf has a sleep function call, you need to release the spin lock first. */
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        gfbg_freeccanbuf(par);
        osal_spin_lock_irqsave(&par->lock, &lock_flag);
    }
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
}

static td_void gfbg_set_dev_int_enable(gfbg_par *par)
{
#if defined(DCONFIG_CHIP_GFBG_SUPPORT_G3) && !defined(__LITEOS__)
    ot_vo_dev dev;
    struct fb_info *info = TD_NULL;
    gfbg_par *par_layer = TD_NULL;
    info = g_layer[2].info; /* 2 G2 */
    par_layer = (gfbg_par *)(info->par);
    if (par_layer->layer_open == TD_TRUE) {
        dev = graphic_drv_get_bind_dev(par_layer->layer_id);
        if (dev == VO_DEV_DHD0) {
            info = g_layer[0].info; /* 0 G0 */
            par_layer = (gfbg_par *)(info->par);
            if (par_layer->layer_open == TD_TRUE) {
                return;
            }
        } else {
            info = g_layer[1].info; /* 1 G1 */
            par_layer = (gfbg_par *)(info->par);
            if (par_layer->layer_open == TD_TRUE) {
                return;
            }
        }
    }
#endif
    graphic_drv_dev_gfbg_int_enable(par->layer_id, TD_FALSE);
}

static td_s32 gfbg_release(struct fb_info *info, td_s32 user)
{
    gfbg_par *par = TD_NULL;
    td_u32 mem_len;
    td_s8 *screen_base = TD_NULL;
    ot_unused(user);

    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    if (info->par == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;

    if (atomic_dec_and_test(&par->ref_count)) {
        gfbg_release_info(par);
        gfbg_set_dev_int_enable(par);

        screen_base = gfbg_get_screen_base(info);
        mem_len = gfbg_get_smem_len(info);
        if ((screen_base != TD_NULL) && (mem_len != 0)) {
            (td_void)memset_s(screen_base, mem_len, 0, mem_len);
        }
        fb_graphics_deinit();
        par->layer_open = TD_FALSE;
        par->refresh_info.do_refresh_job = TD_FALSE;
        wait_event_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job,
            (td_s32)msecs_to_jiffies(40)); /* 40 for timeout */
        osal_spin_lock_destroy(&par->lock);
        fb_decrease_open_cnt();
    }

    return 0;
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
static td_s32 gfbg_draw_smart_rect(const struct fb_info *info, const ot_fb_smart_rect_param *param)
{
    gfbg_mmz_buffer osb_chn = {0};
    gfbg_par *par = (gfbg_par *)info->par;

    osb_chn.start_phy_addr = gfbg_get_smem_start(info);
    osb_chn.start_vir_addr = gfbg_get_screen_base(info);
    osb_chn.size = SMART_RECT_SIZE;
    if (g_drv_ops.gfbg_drv_set_smart_rect == TD_NULL || par == TD_NULL) {
        gfbg_error("gfbg_draw_smart_rect failed!\n");
        return TD_FAILURE;
    }
    if (g_drv_ops.gfbg_drv_set_smart_rect(par->layer_id, par->display_info.x_res,
        par->display_info.y_res, param, &osb_chn) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
#endif

static td_s32 gfbg_ioctl_check_param(const struct fb_info *info, td_u32 cmd, td_void *argp)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;

    if ((info == TD_NULL) || (info->par == TD_NULL)) {
        gfbg_error("NULL arg!\n");
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        gfbg_error("layer_id (%u) is invalid!\n", layer_id);
        return TD_FAILURE;
    }

    if ((argp == TD_NULL) && (cmd != FBIOGET_VER_BLANK_GFBG) && (cmd != FBIO_WAITFOR_FREFRESH_DONE) &&
        (cmd != FBIO_CREATE_LAYER) && (cmd != FBIO_DESTROY_LAYER)) {
        gfbg_error("NULL arg!\n");
        return TD_FAILURE;
    }

    if ((!g_drv_ops.capability[layer_id].is_layer_support) &&
        (!is_cursor_layer(layer_id) || (!is_soft_cursor()))) {
        gfbg_error("not support layer %u!\n", layer_id);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_colorkey(struct fb_info *info, unsigned long arg)
{
    ot_fb_colorkey colorkey = {0};
    gfbg_colorkeyex colorkey_ex = {0};
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;

    if ((!g_drv_ops.capability[par->layer_id].is_key_rgb) &&
        (!g_drv_ops.capability[par->layer_id].is_key_alpha)) {
        gfbg_error("Layer %u doesn't support colorkey!\n", par->layer_id);
        return TD_FAILURE;
    }

    gfbg_get_key(par, &colorkey_ex);

    colorkey.enable = colorkey_ex.key_enable;
    colorkey.value = colorkey_ex.key;

    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &colorkey, sizeof(ot_fb_colorkey));
}

static td_s32 drv_gfbg_set_clut_ck(const struct fb_info *info, const ot_fb_colorkey *colorkey,
    gfbg_colorkeyex *colorkey_ex)
{
    if (colorkey->value >= (td_u32)(1 << gfbg_get_bits_per_pixel(info))) {
        gfbg_error("The value :%u is out of range the palette: %u!\n", colorkey->value,
            1 << gfbg_get_bits_per_pixel(info));
        return TD_FAILURE;
    }

    if ((info->cmap.blue == NULL) || (info->cmap.green == NULL) || (info->cmap.red == NULL)) {
        gfbg_error("error:cmap is NULL!\n");
        return TD_FAILURE;
    }

    colorkey_ex->blue_max = colorkey_ex->blue_min = info->cmap.blue[colorkey->value];
    colorkey_ex->green_max = colorkey_ex->green_min = info->cmap.green[colorkey->value];
    colorkey_ex->red_max = colorkey_ex->red_min = info->cmap.red[colorkey->value];
    colorkey_ex->red_mask = 0xff;
    colorkey_ex->green_mask = 0xff;
    colorkey_ex->blue_mask = 0xff;
    colorkey_ex->key_mode = 0;
    colorkey_ex->mask_enable = TD_TRUE;
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_colorkey(struct fb_info *info, unsigned long arg)
{
    ot_fb_colorkey colorkey;
    gfbg_colorkeyex colorkey_ex;
    unsigned long lock_flag;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;

    if (osal_copy_from_user(&colorkey, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_colorkey))) {
        return -EFAULT;
    }

    if ((colorkey.enable != TD_TRUE) && (colorkey.enable != TD_FALSE)) {
        gfbg_error("enable(%d) should be TRUE or FALSE!\n", colorkey.enable);
        return TD_FAILURE;
    }

    if ((!g_drv_ops.capability[par->layer_id].is_key_rgb) && (!g_drv_ops.capability[par->layer_id].is_key_alpha)) {
        gfbg_error("Layer %u doesn't support colorkey!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (colorkey.enable && display_info->is_premul) {
        gfbg_error("colorkey and premul couldn't take effect at the same time!\n");
        return TD_FAILURE;
    }

    colorkey_ex.key = colorkey.value;
    colorkey_ex.key_enable = colorkey.enable;

    if (gfbg_get_bits_per_pixel(info) <= 8) { /* 8 bits */
        if (drv_gfbg_set_clut_ck(info, &colorkey, &colorkey_ex) !=  TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        if (g_drv_ops.gfbg_drv_color_convert == TD_NULL) {
            return TD_FAILURE;
        }
#ifdef __LITEOS__
        g_drv_ops.gfbg_drv_color_convert(info, &colorkey_ex);
#else
        g_drv_ops.gfbg_drv_color_convert(&info->var, &colorkey_ex);
#endif
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_key(par, &colorkey_ex);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_COLORKEY;
    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_layer_alpha(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_alpha alpha = {0};

    par = (gfbg_par *)info->par;
    gfbg_get_alpha(par, &alpha);
    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &alpha, sizeof(ot_fb_alpha));
}

static td_s32 drv_gfbg_set_layer_alpha(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    unsigned long lock_flag;
    ot_fb_alpha alpha = {0};

    par = (gfbg_par *)info->par;
    if (osal_copy_from_user(&alpha, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_alpha))) {
        return -EFAULT;
    }

    if ((alpha.pixel_alpha != TD_TRUE) && (alpha.pixel_alpha != TD_FALSE)) {
        gfbg_error("alpha->pixel_alpha should be TRUE or FALSE!\n");
        return TD_FAILURE;
    }

    if ((alpha.global_alpha_en != TD_TRUE) && (alpha.global_alpha_en != TD_FALSE)) {
        gfbg_error("alpha->global_alpha_en should be TRUE or FALSE!\n");
        return TD_FAILURE;
    }
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_alpha(par, &alpha);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_ALPHA;

    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_screen_origin_pos(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;

    ot_fb_point pos = {0};

    par = (gfbg_par *)info->par;
    gfbg_get_layerpos(par, &pos);

    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &pos, sizeof(ot_fb_point));
}

static td_s32 drv_gfbg_set_screen_origin_pos(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_point expected_pos;
    td_u32 layer_id;
    unsigned long lock_flag;
    td_bool is_interlace;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    is_interlace = gfbg_is_interlace(par);

    if (is_cursor_layer(layer_id) && is_soft_cursor()) {
        gfbg_error("you shouldn't set soft cursor origin by this cmd, try FBIOPUT_CURSOR_POS\n");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&expected_pos, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_point))) {
        return -EFAULT;
    }

    if (expected_pos.x_pos < 0 || expected_pos.y_pos < 0) {
        gfbg_error("It's not supported to set start pos of layer to negative!\n");
        return TD_FAILURE;
    }

    if (is_interlace && (expected_pos.y_pos % 2 != 0)) { /* 2 alg data */
        gfbg_error("y_pos should be even for interlace vodev!\n");
        return TD_FAILURE;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;

    /* Record the old location first */
    gfbg_set_layerpos(par, &expected_pos);
    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_vblank(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;
    ot_unused(arg);
    if (gfbg_wait_regconfig_work(par->layer_id)) {
        gfbg_error("It is not support VBL!\n");
        return -EPERM;
    }

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_show_layer(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_bool is_show = TD_FALSE;
    unsigned long lock_flag;
    td_u32 layer_id;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;
    if (is_cursor_layer(layer_id) && is_soft_cursor()) {
        gfbg_error("you shouldn't show sot cursor by this cmd, try FBIOPUT_CURSOR_STATE!\n");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&is_show, (td_void __user *)(td_uintptr_t)arg, sizeof(td_bool))) {
        return -EFAULT;
    }

    if ((is_show != TD_TRUE) && (is_show != TD_FALSE)) {
        gfbg_error("show(%d) should be TRUE or FALSE!\n", is_show);
        return TD_FAILURE;
    }

    if (is_show == gfbg_get_show(par)) {
        gfbg_info("The layer is show(%d) now!\n", par->show);
        return TD_SUCCESS;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;

    gfbg_set_show(par, is_show);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;

    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_layer_show_state(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_bool is_show;
    par = (gfbg_par *)info->par;
    is_show = par->show;
    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &is_show, sizeof(td_bool));
}


static td_s32 drv_gfbg_get_capability(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_capability capability = {0};

    par = (gfbg_par *)info->par;
    capability = g_drv_ops.capability[par->layer_id];
    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, (td_void *)&capability, sizeof(ot_fb_capability));
}

static td_s32 drv_gfbg_set_layer_info(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;

    par = (gfbg_par *)info->par;
    return gfbg_onputlayerinfo(info, par, (td_void __user *)(td_uintptr_t)arg);
}

static td_s32 drv_gfbg_get_layer_info(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_layer_info layer_info = {0};
    par = (gfbg_par *)info->par;
    gfbg_get_layerinfo(par, &layer_info);
    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &layer_info, sizeof(ot_fb_layer_info));
}

static td_s32 drv_gfbg_get_canvas_buffer(struct fb_info *info, unsigned long arg)
{
    td_s32 ret;
    gfbg_par *par = TD_NULL;
    ot_fb_buf buf = {0};
    gfbg_refresh_info *refresh_info = TD_NULL;
    par = (gfbg_par *)info->par;
    refresh_info = &par->refresh_info;
    if (!osal_in_interrupt()) {
        ret = ot_mmz_check_phys_addr(par->canvas_sur.phys_addr, par->canvas_sur.pitch * par->canvas_sur.height);
        if (ret != TD_SUCCESS) {
            gfbg_error("Failed to Physical address(0x%lx) process isolation check !\n",
                (td_ulong)par->canvas_sur.phys_addr);
            return TD_FAILURE;
        }
    }
    ret = memcpy_s(&(buf.canvas), sizeof(ot_fb_surface), &(par->canvas_sur), sizeof(ot_fb_surface));
    gfbg_unequal_eok_return(ret);
    ret = memcpy_s(&(buf.update_rect), sizeof(ot_fb_rect), &(refresh_info->user_buffer.update_rect),
        sizeof(ot_fb_rect));
    gfbg_unequal_eok_return(ret);
    if (osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &(buf), sizeof(ot_fb_buf))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_refresh_layer(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)info->par;
    return gfbg_onrefresh(par, (td_void __user *)(td_uintptr_t)arg);
}

static td_s32 drv_gfbg_wait_refresh_finish(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_layer_buf buf_mode;
    par = (gfbg_par *)info->par;
    ot_unused(arg);
    gfbg_get_bufmode(par, &buf_mode);
    if ((buf_mode != OT_FB_LAYER_BUF_NONE) && (buf_mode != OT_FB_LAYER_BUF_BUTT)) {
        /* 80 is timeout */
        if (wait_event_timeout(par->vbl_event, par->vblflag, (td_s32)osal_msecs_to_jiffies(80)) == 0) {
            return TD_FAILURE;
        }
    } else {
        gfbg_error("doesn't support FBIO_WAITFOR_FREFRESH_DONE operation when in standard mode"
                   "or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_mirror_mode(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_mirror_mode mirror_mode;
    par = (gfbg_par *)info->par;
    if (osal_copy_from_user(&mirror_mode, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_mirror_mode))) {
        return -EFAULT;
    }
    if (gfbg_set_mirrormode(par, mirror_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_mirror_mode(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_layer_buf buf_mode = 0;
    ot_fb_mirror_mode mirror_mode;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    gfbg_get_bufmode(par, &buf_mode);
    mirror_mode = display_info->mirror_mode;
    if ((buf_mode == OT_FB_LAYER_BUF_BUTT) || (buf_mode == OT_FB_LAYER_BUF_NONE)) {
        gfbg_error("doesn't support FBIOGET_MIRROR_MODE operation when in standard mode"
                   "or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }
    if (osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &mirror_mode, sizeof(ot_fb_mirror_mode))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_rotate_mode(struct fb_info *info, unsigned long arg)
{
    ot_fb_rotate_mode rotate_mode;

    if (osal_copy_from_user(&rotate_mode, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_rotate_mode))) {
        return -EFAULT;
    /* Reset compression information */
    }
    if (gfbg_get_rotation_support() != TD_TRUE) {
        gfbg_error("doesn't support rotate\n");
        return TD_FAILURE;
    }
    if (gfbg_set_rotatemode(info, rotate_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_rotate_mode(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_layer_buf buf_mode = 0;
    ot_fb_rotate_mode rotate_mode;

    if (gfbg_get_rotation_support() != TD_TRUE) {
        gfbg_error("doesn't support rotate\n");
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    gfbg_get_bufmode(par, &buf_mode);

    if ((buf_mode == OT_FB_LAYER_BUF_BUTT) || (buf_mode == OT_FB_LAYER_BUF_NONE)) {
        gfbg_error("doesn't support FBIOGET_ROTATE_MODE operation when in standard mode"
                   "or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }
    rotate_mode = display_info->rotate_mode;
    if (osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &rotate_mode, sizeof(ot_fb_rotate_mode))) {
        return -EFAULT;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_screen_size(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_size  screen_size;
    ot_fb_size  max_screen_size = {0};
    ot_fb_point pos = {0};
    unsigned long lock_flag;

    par = (gfbg_par *)info->par;
    if (is_cursor_layer(par->layer_id) && is_soft_cursor())  {
        gfbg_error("you shouldn't set soft cursor screensize!");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&screen_size, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_size))) {
        return -EFAULT;
    }
    if ((screen_size.width == 0) || (screen_size.height == 0)) {
        gfbg_error("screen width(%u) height(%u) shouldn't be 0\n", screen_size.width, screen_size.height);
        return TD_FAILURE;
    }
    if (screen_size.width % 2 || screen_size.height % 2) { /* 2 for align */
        gfbg_error("stScreenSize (%u, %u) should align to 2!\n", screen_size.width, screen_size.height);
        return TD_FAILURE;
    }
    gfbg_get_maxscreensize(par, &max_screen_size.width, &max_screen_size.height);
    gfbg_get_layerpos(par, &pos);
    if (screen_size.width > max_screen_size.width - pos.x_pos) {
        gfbg_warning("the sum of width(%d) and x_pos(%d) larger than Vodev screen width(%d),"
                     "width will be changed!\n",
                     screen_size.width, pos.x_pos, max_screen_size.width);
        screen_size.width = max_screen_size.width - pos.x_pos;
    }
    if (screen_size.height > max_screen_size.height - pos.y_pos) {
        gfbg_warning("the sum of height(%d) and y_pos(%d) larger than Vodev screen height(%d),"
                     "width will be changed!\n",
                     screen_size.height, pos.y_pos, max_screen_size.height);
        screen_size.height = max_screen_size.height - pos.y_pos;
    }
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;
    if (gfbg_set_screensize(par, &screen_size.width, &screen_size.height) == TD_SUCCESS) {
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;
    }
    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_screen_size(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_size screen_size = {0};
    par = (gfbg_par *)info->par;
    gfbg_get_screensize(par, &screen_size.width, &screen_size.height);
    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &screen_size, sizeof(ot_fb_size));
}

static td_s32 flip_surface_check_param(const struct fb_info *info, const ot_fb_surfaceex *surface_ex)
{
    gfbg_par *par = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    td_phys_addr_t addr;
    td_phys_addr_t smem_end;
    par = (gfbg_par *)info->par;
    display_info = &par->display_info;
    if (surface_ex->colorkey.enable != TD_TRUE && surface_ex->colorkey.enable != TD_FALSE) {
        gfbg_error("colorkey.enable(%d) should be TRUE or FALSE!\n", surface_ex->colorkey.enable);
        return TD_FAILURE;
    }

    if (surface_ex->alpha.pixel_alpha != TD_TRUE && surface_ex->alpha.pixel_alpha != TD_FALSE) {
        gfbg_error("alpha.pixel_alpha(%d) should be TRUE or FALSE!\n", surface_ex->alpha.pixel_alpha);
        return TD_FAILURE;
    }

    if (surface_ex->alpha.global_alpha_en != TD_TRUE && surface_ex->alpha.global_alpha_en != TD_FALSE) {
        gfbg_error("alpha.global_alpha_en (%d) should be TRUE or FALSE!\n", surface_ex->alpha.global_alpha_en);
        return TD_FAILURE;
    }

    if ((surface_ex->colorkey.enable && !g_drv_ops.capability[par->layer_id].is_key_rgb) &&
        (!g_drv_ops.capability[par->layer_id].is_key_alpha)) {
        gfbg_error("Layer %d doesn't support colorkey!\n", par->layer_id);
        return TD_FAILURE;
    }

    if (surface_ex->colorkey.enable && display_info->is_premul) {
        gfbg_error("colorkey and premul couldn't take effect at the same time!\n");
        return TD_FAILURE;
    }

    addr = surface_ex->phys_addr;
    smem_end = gfbg_get_smem_start(info) + gfbg_get_smem_len(info) - gfbg_get_yres(info) * gfbg_get_line_length(info);
    if ((addr < gfbg_get_smem_start(info)) || (addr > smem_end)) {
        gfbg_error("the addr is out of range!\n");
        return TD_FAILURE;
    }

    if (gfbg_get_line_length(info) == 0) {
        return TD_FAILURE;
    }

    if (gfbg_get_bits_per_pixel(info) == 0) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifdef __LITEOS__
static td_s32 flip_surface_pan_display(struct fb_info *info, const ot_fb_surfaceex *surface_ex,
    gfbg_colorkeyex *colorkey_ex)
{
    td_s32 ret;
    td_phys_addr_t addr;
    unsigned int differ;
    unsigned int x_offset;
    unsigned int y_offset;
    struct fb_overlayinfo_s oinfo;
    addr = surface_ex->phys_addr;
    differ = addr - gfbg_get_smem_start(info);
    y_offset = differ / gfbg_get_line_length(info);
    /* 3 is 8 bit */
    x_offset = (((differ % gfbg_get_line_length(info)) << 3) / (gfbg_get_bits_per_pixel(info)));

    ret = memcpy_s(&oinfo, sizeof(oinfo), &info->oinfo, sizeof(oinfo));
    gfbg_unequal_eok_return(ret);
    oinfo.sarea.x = x_offset;
    oinfo.sarea.y = y_offset;

    if (gfbg_pan_display(info, &oinfo) < 0) {
        gfbg_error("error!\n");
        return TD_FAILURE;
    }

    info->oinfo.sarea.x = x_offset;
    info->oinfo.sarea.y = y_offset;
    colorkey_ex->key = surface_ex->colorkey.value;
    colorkey_ex->key_enable = surface_ex->colorkey.enable;
    if (gfbg_get_bits_per_pixel(info) <= 8) { /* 8 bit */
        if (surface_ex->colorkey.value >= (1 << gfbg_get_bits_per_pixel(info))) {
            gfbg_error("The value :%d is out of range the palette: %d!\n", surface_ex->colorkey.value,
                1 << gfbg_get_bits_per_pixel(info));
            return TD_FAILURE;
        }
        if ((info->cmap.blue != NULL) || (info->cmap.green != NULL) || (info->cmap.red != NULL)) {
            colorkey_ex.blue_max = colorkey_ex.blue_min = info->cmap.blue[surface_ex->colorkey.value];
            colorkey_ex.green_max = colorkey_ex.green_min = info->cmap.green[surface_ex->colorkey.value];
            colorkey_ex.red_max = colorkey_ex.red_min = info->cmap.red[surface_ex->colorkey.value];
            colorkey_ex.red_mask = 0xff;
            colorkey_ex.green_mask = 0xff;
            colorkey_ex.blue_mask = 0xff;
            colorkey_ex.key_mode = 0;
            colorkey_ex.mask_enable = TD_TRUE;
        }
    } else {
        g_drv_ops.gfbg_drv_color_convert(info, colorkey_ex);
    }
    return TD_SUCCESS;
}
#else
static td_s32 flip_surface_pan_display(struct fb_info *info, const ot_fb_surfaceex *surface_ex,
    gfbg_colorkeyex *colorkey_ex)
{
    td_s32 ret;
    td_phys_addr_t addr;
    unsigned int differ;
    unsigned int x_offset;
    unsigned int y_offset;
    struct fb_var_screeninfo var;
    addr = surface_ex->phys_addr;
    differ = addr - gfbg_get_smem_start(info);
    y_offset = differ / gfbg_get_line_length(info);
    /* 8 bit (2^3) */
    x_offset = (((differ % gfbg_get_line_length(info)) << 3) / (gfbg_get_bits_per_pixel(info)));
    ret = memcpy_s(&var, sizeof(var), &info->var, sizeof(var));
    gfbg_unequal_eok_return(ret);
    var.xoffset = x_offset;
    var.yoffset = y_offset;

    if (fb_pan_display(info, &var) < 0) {
        gfbg_error("pan_display error!\n");
        return TD_FAILURE;
    }

    colorkey_ex->key = surface_ex->colorkey.value;
    colorkey_ex->key_enable = surface_ex->colorkey.enable;

    if (gfbg_get_bits_per_pixel(info) <= 8UL) { /* 8 bits */
        if (surface_ex->colorkey.value >= (2UL << gfbg_get_bits_per_pixel(info))) { /* 2 for calculate */
            gfbg_error("The value :%d is out of range the palette: %d!\n", surface_ex->colorkey.value,
                       2 << gfbg_get_bits_per_pixel(info)); /* 2 for calculate */
            return TD_FAILURE;
        }

        colorkey_ex->blue_max = colorkey_ex->blue_min = info->cmap.blue[surface_ex->colorkey.value];
        colorkey_ex->green_max = colorkey_ex->green_min = info->cmap.green[surface_ex->colorkey.value];
        colorkey_ex->red_max = colorkey_ex->red_min = info->cmap.red[surface_ex->colorkey.value];
    } else {
        g_drv_ops.gfbg_drv_color_convert(&info->var, colorkey_ex);
    }

    return TD_SUCCESS;
}
#endif

static td_s32 drv_gfbg_flip_surface(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    ot_fb_surfaceex surface_ex;
    unsigned long lock_flag;
    gfbg_colorkeyex colorkey_ex = {0};

    par = (gfbg_par *)info->par;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_error("you shouldn't use FBIOFLIP_SURFACE for soft cursor!");
        return TD_FAILURE;
    }

    if (osal_copy_from_user(&surface_ex, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_surfaceex))) {
        return -EFAULT;
    }

    /* check surface is value or not */
    if (flip_surface_check_param(info, &surface_ex) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* refresh and color convert */
    if (flip_surface_pan_display(info, &surface_ex, &colorkey_ex) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_alpha(par, &surface_ex.alpha);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_ALPHA;
    if (g_drv_ops.capability[par->layer_id].is_key_rgb || g_drv_ops.capability[par->layer_id].is_key_alpha) {
        gfbg_set_key(par, &colorkey_ex);
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_COLORKEY;
    }
    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return TD_SUCCESS;
}

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
static td_s32 set_compression_process(const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lock_flag;
    td_u32 buf_size, cmp_stride;
    gfbg_stride_attr attr = {0};

    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;
    attr.format = par->color_format;
    attr.width = par->display_info.display_width;
    cmp_stride = 0;
    if (par->color_format == OT_FB_FORMAT_ARGB1555 || par->color_format == OT_FB_FORMAT_ARGB4444) {
        gfbg_recalculate_stride(&cmp_stride, NULL, &attr);
        buf_size = ((cmp_stride * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
        if (gfbg_get_smem_len(info) < (buf_size * 2)) { /* 2 for cal */
            gfbg_error("Memory is too small. ARGB1555 or ARGB4444 compression needs to allocate more memory.\n");
            gfbg_error("Memory should be greater than or equal to %d byte\n", (buf_size * 2)); /* 2 for cal */
            return TD_FAILURE;
        }
    }

    if (par->color_format != OT_FB_FORMAT_ARGB8888 && par->color_format != OT_FB_FORMAT_ARGB1555 &&
        par->color_format != OT_FB_FORMAT_ARGB4444) {
        gfbg_error("compression only support pixel format (ARGB8888,ARGB1555,ARGB4444)\n");
        return TD_FAILURE;
    }
    if (par->display_info.mirror_mode != OT_FB_MIRROR_NONE || par->display_info.rotate_mode != OT_FB_ROTATE_NONE) {
        gfbg_error("Can't do compression when mirror or rotate!\n");
        return TD_FAILURE;
    }
    gfbg_get_bufmode(par, &buf_mode);

    /* FB uses frame decompression, can not be displayed while refreshing, so only supports double buf mode */
    if ((buf_mode != OT_FB_LAYER_BUF_DOUBLE) && (buf_mode != OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        gfbg_error("only FB_LAYER_BUF_DOUBLE or FB_LAYER_BUF_DOUBLE_IMMEDIATE mode support compress!\n");
        return TD_FAILURE;
    }

    /* Record the entire image as the area to be compressed */
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    compress_info->compress_rect.x = 0;
    compress_info->compress_rect.y = 0;
    compress_info->compress_rect.width = par->display_info.display_width;
    compress_info->compress_rect.height = par->display_info.display_height;
    compress_info->update_finished = TD_TRUE;
    compress_info->delay = TD_TRUE;
    compress_info->compress_open = TD_TRUE;
    compress_info->is_losslessa = TD_FALSE;
    compress_info->is_lossless = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return TD_SUCCESS;
}

static td_s32 set_compression_start(const struct fb_info *info, td_bool is_compress)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    unsigned long lock_flag;
    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;
    if (is_compress != TD_TRUE && is_compress != TD_FALSE) {
        gfbg_error("compress(%d) should be TRUE or FALSE!\n", is_compress);
        return TD_FAILURE;
    }
    if ((compress_info->is_economize_memory) == TD_TRUE && (is_compress != TD_TRUE)) {
        gfbg_error("memory not enough, should be start compress!\n");
        return TD_FAILURE;
    }
    if (is_compress != compress_info->compress_open) {
        if (is_compress) {
            /*
             * FB uses frame decompression, can not be displayed while refreshing
             * so only supports double buf mode
             * Record the entire image as the area to be compressed
             */
            if (set_compression_process(info) != TD_SUCCESS) {
                return TD_FAILURE;
            }
        } else {
            osal_spin_lock_irqsave(&par->lock, &lock_flag);
            par->param_modify_mask |= GFBG_LAYER_PARAMODIEY_COMPRESS;
            compress_info->compress_open = TD_FALSE;
            compress_info->is_losslessa = TD_FALSE;
            compress_info->is_lossless = TD_FALSE;
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            /* Waiting for an interrupt before proceeding to the next two operations */
            gfbg_wait_regconfig_work(par->layer_id);
            /* Reset compression information */
            compress_info->update_rect.width = 0;
            compress_info->update_rect.height = 0;
        }
    }
    return TD_SUCCESS;
}
#endif

static td_s32 drv_gfbg_set_compression_mode(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_bool is_compress = TD_FALSE;
    par = (gfbg_par *)info->par;
    if (!g_drv_ops.capability[par->layer_id].is_decompress) {
        gfbg_error("Layer %d doesn't support compression operation!\n", par->layer_id);
        return TD_FAILURE;
    }

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    if (osal_copy_from_user(&is_compress, (td_void __user *)(td_uintptr_t)arg, sizeof(td_bool))) {
        return -EFAULT;
    }

    /* set compress config */
    if (set_compression_start(info, is_compress) != TD_SUCCESS) {
        return TD_FAILURE;
    }
#else
    ot_unused(is_compress);
    ot_unused(arg);
#endif
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_compression_mode(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    td_bool is_compress = TD_FALSE;
    par = (gfbg_par *)info->par;
    compress_info = &par->compress_info;

    if (!g_drv_ops.capability[par->layer_id].is_decompress) {
        gfbg_warning("Layer %d doesn't support get compression!\n", par->layer_id);
    } else {
        is_compress = compress_info->compress_open;
    }
    return osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &is_compress, sizeof(td_bool));
}

static td_s32 drv_gfbg_create(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    unsigned long lock_flag;
    ot_unused(arg);
    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        gfbg_error("par is NULL failed!\n");
        return TD_FAILURE;
    }
    layer_id = par->layer_id;

    /* when unbind/bind  */
    if (g_drv_ops.gfbg_open_layer(layer_id) != TD_SUCCESS) {
        gfbg_error("Open graphic layer %u# failed!\n", layer_id);
        return TD_FAILURE;
    }

    if (g_drv_ops.gfbg_drv_set_layer_enable(layer_id, TD_TRUE) != TD_SUCCESS) {
        gfbg_error("enable graphic layer %u# failed!\n", layer_id);
        return TD_FAILURE;
    }
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->show = TD_TRUE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_release(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    unsigned long lock_flag;
    ot_unused(arg);
    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        gfbg_error("par is NULL failed!\n");
        return TD_FAILURE;
    }
    layer_id = par->layer_id;

    if (g_drv_ops.gfbg_drv_set_layer_enable(layer_id, TD_FALSE) != TD_SUCCESS) {
        gfbg_error("disable graphic layer %u# failed!\n", layer_id);
        return TD_FAILURE;
    }
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->show = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_draw_frame(struct fb_info *info, td_ulong arg)
{
    gfbg_par *par = NULL;
    ot_fb_smart_rect_param frame_param = {0};
    td_ulong lock_flag;
    td_char *vir = TD_NULL;
    par = (gfbg_par *)info->par;
    if (g_drv_ops.capability[par->layer_id].is_osb == TD_FALSE) {
        gfbg_error("Layer %d# doesn't support draw smart rect function.\n", par->layer_id);
        return TD_FAILURE;
    }
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    if (osal_copy_from_user(&frame_param, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_smart_rect_param))) {
        return -EFAULT;
    }
    if (frame_param.num > 128) { /* frame max is 128 */
        gfbg_error("actual %d,support draw smart rect max 128.\n", frame_param.num);
        return TD_FAILURE;
    }
    if (frame_param.rect_start == NULL) {
        return TD_FAILURE;
    }
    vir = osal_kmalloc(sizeof(ot_fb_smart_rect) * 128, OSAL_GFP_KERNEL); /* frame max is 128 */
    if (vir == TD_NULL) {
        return TD_FAILURE;
    }

    if (osal_copy_from_user(vir, (td_void __user *)frame_param.rect_start,
        (td_ulong)(sizeof(ot_fb_smart_rect) * frame_param.num))) {
        osal_kfree(vir);
        return -EFAULT;
    }
    frame_param.rect_start = (ot_fb_smart_rect *)vir;
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;
    if (gfbg_draw_smart_rect(info, &frame_param) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        osal_kfree(vir);
        return TD_FAILURE;
    }
    par->param_modify_mask |= GFBG_LAYER_PARAMODIEY_SMART_RECT;
    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    osal_kfree(vir);
#else
    ot_unused(lock_flag);
    ot_unused(frame_param);
    ot_unused(vir);
#endif
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_check_layer_csc(td_u32 layer, const ot_fb_layer_csc *csc)
{
    if (csc->csc_matrix < OT_FB_CSC_MATRIX_RGBFULL_TO_BT601LIMIT ||
        csc->csc_matrix > OT_FB_CSC_MATRIX_RGBFULL_TO_BT709FULL) {
        gfbg_error("fb layer(%d) csc matrix type(%u) should be [%u, %u]!\n", layer, csc->csc_matrix,
            OT_FB_CSC_MATRIX_RGBFULL_TO_BT601LIMIT, OT_FB_CSC_MATRIX_RGBFULL_TO_BT709FULL);
        return TD_FAILURE;
    }

    if (csc->luma > FB_CSC_LUMA_MAX) {
        gfbg_error("fb csc luma value %u out of range!\n", csc->luma);
        return TD_FAILURE;
    }

    if (csc->contrast > FB_CSC_CONT_MAX) {
        gfbg_error("fb csc contrast value %u out of range!\n", csc->contrast);
        return TD_FAILURE;
    }

    if (csc->hue > FB_CSC_HUE_MAX) {
        gfbg_error("fb csc hue value %u out of range!\n", csc->hue);
        return TD_FAILURE;
    }

    if (csc->saturation > FB_CSC_SAT_MAX) {
        gfbg_error("fb csc saturation value %u out of range!\n", csc->saturation);
        return TD_FAILURE;
    }

    if ((csc->ex_csc_en != TD_TRUE) && (csc->ex_csc_en != TD_FALSE)) {
        gfbg_error("fb csc ext csc switch %u out of range!\n", csc->ex_csc_en);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 drv_gfbg_set_layer_csc(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_layer_csc layer_csc;
    td_s32 ret;
    graphic_layer_context *gfx_layer_ctx = fb_graphics_get_gfx_layer_ctx();
    hal_disp_layer gfx_layer = gfbg_drv_gfbglayer_to_hwlayer(par->layer_id);

    if (osal_copy_from_user(&layer_csc, (td_void __user *)(td_uintptr_t)arg, sizeof(ot_fb_layer_csc))) {
        gfbg_error("copy from user failed!\n");
        return -EFAULT;
    }

    ret = drv_gfbg_check_layer_csc(par->layer_id, &layer_csc);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (gfx_layer_ctx[par->layer_id].opened != TD_TRUE) {
        gfbg_error("The graphic layer %u# is not open!\n", par->layer_id);
        return TD_FAILURE;
    }

    ret = memcpy_s(&gfx_layer_ctx[par->layer_id].gfx_csc, sizeof(ot_fb_layer_csc), &layer_csc, sizeof(ot_fb_layer_csc));
    gfbg_unequal_eok_return(ret);

    ret = fb_graphic_drv_set_csc_coef(gfx_layer, &gfx_layer_ctx[par->layer_id].gfx_csc,
                                      &gfx_layer_ctx[par->layer_id].coef_param);
    if (ret != TD_SUCCESS) {
        gfbg_error("The graphic %u set csc coef failed!\n", par->layer_id);
        return TD_FAILURE;
    }
    ret = fb_graphics_set_csc_en(gfx_layer, TD_TRUE);
    if (ret != TD_SUCCESS) {
        gfbg_error("The graphics %u set csc en failed!\n", par->layer_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 drv_gfbg_get_layer_csc(struct fb_info *info, unsigned long arg)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_layer_csc layer_csc;
    td_s32 ret;
    graphic_layer_context *gfx_layer_ctx = fb_graphics_get_gfx_layer_ctx();

    ret = memcpy_s(&layer_csc, sizeof(ot_fb_layer_csc), &gfx_layer_ctx[par->layer_id].gfx_csc, sizeof(ot_fb_layer_csc));
    gfbg_unequal_eok_return(ret);

    if (osal_copy_to_user((td_void __user *)(td_uintptr_t)arg, &layer_csc, sizeof(ot_fb_layer_csc))) {
        gfbg_error("copy to user failed!\n");
        return -EFAULT;
    }
    return TD_SUCCESS;
}

#ifdef __LITEOS__
static td_s32 gfbg_ioctl_liteos_standard_io(struct fb_info *info, td_u32 cmd, unsigned long arg, td_bool *is_continue)
{
    td_s32 ret;
    /* for  FBIOPUT_SCREENINFO_GFBG */
    struct gfbg_info *info_temp = (struct gfbg_info *)arg;
    struct gfbg_info info_copy;
    /* for FBIOPAN_DISPLAY_GFBG */
    struct fb_overlayinfo_s *oinfo = (struct fb_overlayinfo_s *)arg;

    switch (cmd) {
        case FBIOGET_SCREENINFO_GFBG:
            ret = memcpy_s((void *)arg, sizeof(struct gfbg_info), info, sizeof(struct gfbg_info));
            gfbg_unequal_eok_return(ret);
            break;
        case FBIOPUT_SCREENINFO_GFBG:
            ret = memcpy_s(&info_copy, sizeof(info_copy), info, sizeof(info_copy));
            gfbg_unequal_eok_return(ret);
            if (info->vtable.fb_set_par) {
                info->vinfo.xres = info_temp->vinfo.xres;
                info->vinfo.yres = info_temp->vinfo.yres;
                info->vinfo.format = info_temp->vinfo.format;
                info->oinfo.sarea.width = info_temp->oinfo.sarea.width;
                info->oinfo.sarea.height = info_temp->oinfo.sarea.height;
                info->oinfo.sarea.x = info_temp->oinfo.sarea.x;
                info->oinfo.sarea.y = info_temp->oinfo.sarea.y;
                info->oinfo.bpp = info_temp->oinfo.bpp;
                if (info->vtable.fb_set_par(&info->vtable) != TD_SUCCESS) {
                    gfbg_error("Put screeninfo error! ret=%d\n", ret);
                    ret = memcpy_s(info, sizeof(info_copy), &info_copy, sizeof(info_copy));
                    gfbg_unequal_eok_return(ret);
                    return TD_FAILURE;
                }
                gfbg_pan_display(&info->vtable, &info->oinfo);
            }
            break;
        case FBIOPAN_DISPLAY_GFBG:
            if (info->vtable.fb_pan_display) {
                if (info->vtable.fb_pan_display(&info->vtable, oinfo) != TD_SUCCESS) {
                    gfbg_error("Put screeninfo error! ret=%d\n", ret);
                    ret = memcpy_s(info, sizeof(info_copy), &info_copy, sizeof(info_copy));
                    gfbg_unequal_eok_return(ret);
                    return TD_FAILURE;
                }
                info->oinfo.sarea.x = oinfo->sarea.x;
                info->oinfo.sarea.y = oinfo->sarea.y;
            }
            break;
        default:
            *is_continue = TD_TRUE;
            break;
    }
    return TD_SUCCESS;
}
#endif

/*
 * Function        : gfbg_ioctl
 * Description     : set the colorkey or alpha for overlay
 * Return          : return 0 if succeed, otherwise return error code
 */
static td_s32 gfbg_ioctl(struct fb_info *info, td_u32 cmd, unsigned long arg)
{
    td_u8 gfbg_cmd = _IOC_NR(cmd);
#ifdef __LITEOS__
    td_bool is_continue = TD_FALSE;
#endif
    if (gfbg_ioctl_check_param(info, cmd, (td_void __user *)(td_uintptr_t)arg) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* liteos deal with FBIOGET_SCREENINFO_GFBG\FBIOPUT_SCREENINFO_GFBG\FBIOPAN_DISPLAY_GFBG */
#ifdef __LITEOS__
    if (gfbg_ioctl_liteos_standard_io(info, cmd, arg, &is_continue) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (is_continue != TD_TRUE) {
        return TD_SUCCESS;
    }
#endif
    if ((gfbg_cmd < 1) || (gfbg_cmd >= DRV_GFBG_IOCTL_CMD_NUM_MAX) || (g_drv_gfbg_ctl_num[gfbg_cmd] < 1) ||
        (g_drv_gfbg_ctl_num[gfbg_cmd] >= DRV_GFBG_IOCTL_FUNC_ITEM_NUM_MAX)) {
        return TD_FAILURE;
    }
    if (g_drv_gfbg_ioctl_func[g_drv_gfbg_ctl_num[gfbg_cmd]].func == TD_NULL) {
        return TD_FAILURE;
    }

    if (cmd != g_drv_gfbg_ioctl_func[g_drv_gfbg_ctl_num[gfbg_cmd]].cmd) {
        gfbg_error("the command:0x%x is unsupported!\n", cmd);
        return TD_FAILURE;
    }
    return g_drv_gfbg_ioctl_func[g_drv_gfbg_ctl_num[gfbg_cmd]].func(info, arg);
}

#ifdef CONFIG_COMPAT
static td_s32 gfbg_compat_ioctl(struct fb_info *info, unsigned cmd, unsigned long arg)
{
    return gfbg_ioctl(info, cmd, arg);
}
#endif

static td_void gfbg_version(td_void)
{
    /* 80length:Use "strings gfbg.ko | grep "GFBG_MAIN_VERSION"" to get the version */
    td_char gfbg_version[80] =
        "GFBG_MAIN_VERSION[" mkmarcotostr(GFBG_MAIN_VERSION) "] Build Time[" __DATE__ ", "__TIME__ "]";
    gfbg_info("%s\n", gfbg_version);
}

static td_s32 gfbg_bitfieldcmp(struct fb_bitfield x, struct fb_bitfield y)
{
    if ((x.offset == y.offset) && (x.length == y.length) && (x.msb_right == y.msb_right)) {
        return 0;
    } else {
        return -1;
    }
}

static td_u32 gfbg_getbppbyfmt(ot_fb_color_format color_fmt)
{
    switch (color_fmt) {
        case OT_FB_FORMAT_RGB565:
        case OT_FB_FORMAT_KRGB444:
        case OT_FB_FORMAT_KRGB555:
        case OT_FB_FORMAT_ARGB4444:
        case OT_FB_FORMAT_ARGB1555:
        case OT_FB_FORMAT_RGBA4444:
        case OT_FB_FORMAT_RGBA5551:
        case OT_FB_FORMAT_ACLUT88:
        case OT_FB_FORMAT_BGR565:
        case OT_FB_FORMAT_ABGR1555:
        case OT_FB_FORMAT_ABGR4444:
        case OT_FB_FORMAT_KBGR444:
        case OT_FB_FORMAT_KBGR555:
            return 16; /* 16 bit width */
        case OT_FB_FORMAT_RGB888:
        case OT_FB_FORMAT_ARGB8565:
        case OT_FB_FORMAT_RGBA5658:
        case OT_FB_FORMAT_ABGR8565:
        case OT_FB_FORMAT_BGR888:
            return 24; /* 24 bit width */
        case OT_FB_FORMAT_KRGB888:
        case OT_FB_FORMAT_ARGB8888:
        case OT_FB_FORMAT_RGBA8888:
        case OT_FB_FORMAT_ABGR8888:
        case OT_FB_FORMAT_KBGR888:
            return 32; /* 32 bit width */
        case OT_FB_FORMAT_1BPP:
            return 1;
        case OT_FB_FORMAT_2BPP:
            return 2; /* 2 bit width */
        case OT_FB_FORMAT_4BPP:
            return 4; /* 4 bit width */
        case OT_FB_FORMAT_8BPP:
        case OT_FB_FORMAT_ACLUT44:
            return 8; /* 8 bit width */
        default:
            return 0;
    }
}

static ot_fb_color_format gfbg_getfmtbyargb(const struct fb_bitfield *red, const struct fb_bitfield *green,
                                            const struct fb_bitfield *blue, const struct fb_bitfield *transp,
                                            td_u32 color_depth)
{
    td_u32 i;
    td_u32 bpp;

    if ((red == TD_NULL) || (green == TD_NULL) || (blue == TD_NULL) || (transp == TD_NULL)) {
        return  OT_FB_FORMAT_BUTT;
    }

    /* not support color palette low than 8bit */
    if (color_depth < 2) { /* 2 bit */
        return  OT_FB_FORMAT_BUTT;
    }

    if (color_depth == 2) { /* 2 bit */
        return OT_FB_FORMAT_2BPP;
    }

    if (color_depth == 4) { /* 4 bit */
        return OT_FB_FORMAT_4BPP;
    }

    if (color_depth == 8) { /* 8 bit */
        return OT_FB_FORMAT_8BPP;
    }

    /*
     * Find the pixel format (gfbg_argb_bitinfo) corresponding to the given red,
     * green, and blue bit field information and the number of bits per pixel (bpp)
     */
    for (i = 0; i < sizeof(g_argb_bit_field) / sizeof(gfbg_argb_bitinfo); i++) {
        if ((gfbg_bitfieldcmp(*red, g_argb_bit_field[i].red) == 0) &&
            (gfbg_bitfieldcmp(*green, g_argb_bit_field[i].green) == 0) &&
            (gfbg_bitfieldcmp(*blue, g_argb_bit_field[i].blue) == 0) &&
            (gfbg_bitfieldcmp(*transp, g_argb_bit_field[i].transp) == 0)) {
            bpp = gfbg_getbppbyfmt(i);
            if (bpp == color_depth) {
                return i;
            }
        }
    }
    i = OT_FB_FORMAT_BUTT;
    return i;
}

static td_s32 gfbg_check_mem_enough(const struct fb_info *info, td_u32 pitch, td_u32 height)
{
    td_u32 buffer_num = 0;
    td_u32 buffer_size;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    if (info->par == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    refresh_info = &par->refresh_info;

    switch (refresh_info->buf_mode) {
        case OT_FB_LAYER_BUF_DOUBLE:
        case OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE:
            buffer_num = 2; /* 2 buffer num */
            break;

        case OT_FB_LAYER_BUF_ONE:
            buffer_num = 1;
            break;

        default:
            return TD_SUCCESS;
    }
    /* The interface setting requires uBuffersize, the actual memory size info->fix.smem_len */
    buffer_size = buffer_num * pitch * height;
    if (gfbg_get_smem_len(info) >= buffer_size) {
        return TD_SUCCESS;
    }
    gfbg_error("memory is not enough!  now is %d u32Pitch %d u32Height %d expect %d\n", gfbg_get_smem_len(info), pitch,
               height, buffer_size);
    return TD_FAILURE;
}

/* Address check only for uncompressed data */
static td_s32 gfbg_check_phyaddr(const ot_fb_buf *canvas_buf)
{
    td_u64 len;
    td_u32 bpp;

    bpp = gfbg_getbppbyfmt(canvas_buf->canvas.format);
    if (bpp == 0) {
        gfbg_error("Unsupported PIXEL FORMAT!\n");
        return TD_FAILURE;
    }
    len = canvas_buf->update_rect.height * (canvas_buf->update_rect.width * bpp / 8);  /* 8 alg data */

    return ot_mmz_check_phys_addr(canvas_buf->canvas.phys_addr, (td_ulong)len);
}

#ifndef __LITEOS__
static td_s32 gfbg_check_fmt(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    ot_fb_color_format format;
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    format = gfbg_getfmtbyargb(&var->red, &var->green, &var->blue, &var->transp, var->bits_per_pixel);
    if (format == OT_FB_FORMAT_BUTT) {
        gfbg_error("Unknown format(offset, length) r:(%d, %d, %d) ,g:(%d, %d, %d), b(%d, %d, %d), \
            a(%d, %d, %d), bpp:%d!\n",
            var->red.offset, var->red.length, var->red.msb_right,
            var->green.offset, var->green.length, var->green.msb_right,
            var->blue.offset, var->blue.length, var->blue.msb_right,
            var->transp.offset, var->transp.length, var->transp.msb_right,
            var->bits_per_pixel);
        return -EINVAL;
    }

    if (!g_drv_ops.capability[layer_id].is_color_format[format]) {
        gfbg_error("Unsupported PIXEL FORMAT!\n");
        return -EINVAL;
    }

    return TD_SUCCESS;
}
#endif

/*
 * Name : gfbg_buf_map
 * Desc : Memory mapping, which generates virtual addresses based on physical address mappings.
 */
static td_void *gfbg_buf_map(td_phys_addr_t phys_addr, td_u32 size)
{
    return cmpi_remap_nocache(phys_addr, size);
}

static td_void gfbg_buf_ummap(td_void *vir_addr)
{
    cmpi_unmap(vir_addr);
    return;
}

#ifndef __LITEOS__
static td_s32 gfbg_check_output(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (((var->yres % 2) != 0) && (gfbg_is_interlace(par))) { /* 2 for calculate */
        gfbg_error("yres(%d) of layer_id %d should be even when vodev output is interlace\n", var->yres, layer_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_check_virtual_resolution(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (var->xres < g_drv_ops.capability[layer_id].min_width) {
        gfbg_error("xres(%d) of layer_id %d can't be less than min_width(%d)\n", var->xres, layer_id,
                   g_drv_ops.capability[layer_id].min_width);
        return TD_FAILURE;
    }
    if (var->yres < g_drv_ops.capability[layer_id].min_height) {
        gfbg_error("yres(%d) of layer_id %d can't be less than min_height(%d)\n", var->yres, layer_id,
                   g_drv_ops.capability[layer_id].min_height);
        return TD_FAILURE;
    }

    if (var->xres > var->xres_virtual) {
        gfbg_error("xres(%d) of layer_id %d should be less than xres_virtual(%d)\n", var->xres, layer_id,
                   var->xres_virtual);
        return TD_FAILURE;
    }
    if (var->yres > var->yres_virtual) {
        gfbg_error("yres(%d) of layer_id %d should be less than yres_virtual(%d)\n", var->yres, layer_id,
                   var->yres_virtual);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_check_offset(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if ((var->xoffset + var->xres > var->xres_virtual) || (var->xoffset > var->xres_virtual)) {
        gfbg_error("the sum of layer%d's xoffset(%d) and xres(%d) should be less than xres_virtual(%d)\n", layer_id,
                   var->xoffset, var->xres, var->xres_virtual);
        return -EINVAL;
    }

    if ((var->yoffset + var->yres > var->yres_virtual) || (var->yoffset > var->yres_virtual)) {
        gfbg_error("the sum of layer%d's yoffset(%d) and yres(%d) should be less than yres_virtual(%d)\n", layer_id,
                   var->yoffset, var->yres, var->yres_virtual);
        return -EINVAL;
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_check_total(const struct fb_var_screeninfo *var, const struct fb_info *info)
{
    td_u32 hor_total;
    td_u32 ver_total;
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    hor_total = var->left_margin + var->xres + var->right_margin + var->hsync_len;
    if (hor_total == 0) {
        gfbg_error("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d),hsync_len(%d) can't be 0\n",
            layer_id, var->left_margin, var->xres, var->right_margin, var->hsync_len);
        return TD_FAILURE;
    }
    ver_total = var->yres + var->lower_margin + var->vsync_len + var->upper_margin;
    if (ver_total == 0) {
        gfbg_error("the sum of layer%d's left_margin(%d),xres(%d),right_margin(%d),hsync_len(%d) can't be 0\n",
            layer_id, var->upper_margin, var->yres, var->lower_margin, var->vsync_len);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * Function        : gfbg_check_var
 * Description     : check if the parameter for framebuffer is supported.
 * Return          : return 0, if the parameter is supported, otherwise,return error
 */
static td_s32 gfbg_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    gfbg_par *par = TD_NULL;
    td_u32 expected_len;
    td_u32 layer_id;

    if ((info == TD_NULL) || (var == TD_NULL) || (info->par == TD_NULL)) {
        return TD_FAILURE;
    }

    par = (gfbg_par *)info->par;
    layer_id = par->layer_id;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_error("cursor layer doesn't support this operation!\n");
        return TD_FAILURE;
    }

    if (gfbg_check_fmt(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /*
     * For interlaced output check
     * the actual height of the layer must be an even number
     * Progressive output without this limit
     */
    if (gfbg_check_output(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * for virtual resolution check
     * virtual resolution can't be less than minimal resolution
     */
    if (gfbg_check_virtual_resolution(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* check if the offset is valid */
    if (gfbg_check_offset(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * for hor_total and ver_total check
     * The FB driver in the Linux kernel will use u32HTotal and u32VTotal as divisors
     * so they cannot be 0
     */
    if (gfbg_check_total(var, info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    gfbg_info("xres:%d, yres:%d, xres_virtual:%d, yres_virtual:%d\n", var->xres, var->yres, var->xres_virtual,
              var->yres_virtual);
    /* for mem len check */
    expected_len = var->yres_virtual * ((((var->xres_virtual * var->bits_per_pixel) >> 3) + /* 8 bit (2^3) */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT));

    if (info->fix.smem_len && (expected_len > info->fix.smem_len)) {
        gfbg_error("layer %d don't has enough mem! expected: %d KBytes, real:%d KBytes\n", layer_id,
                   expected_len / 1024, info->fix.smem_len / 1024); /* 1024 for KB */
        return -EINVAL;
    }

    return TD_SUCCESS;
}
#endif

static td_void gfbg_set_dispbufinfo(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)(info->par);
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_dispbuf_info *disp_buf_info = &refresh_info->disp_buf_info;
    td_u32 uncompress_stride, buf_size;
    uncompress_stride = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);

    /*
     * there's a limit from hardware that screen buf should be 16 bytes aligned,maybe it's proper
     * to get this info from drv adapter
     */
    if (gfbg_check_memory_enough(info, uncompress_stride) == TD_TRUE &&
        info->var.bits_per_pixel == 32) { /* 32 only for argb8888 */
        buf_size = ((uncompress_stride * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    } else {
        /* according real stride,calculate offset */
        buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    }
    if (gfbg_get_smem_len(info) == 0) {
        return;
    } else if ((gfbg_get_smem_len(info) >= buf_size) && (gfbg_get_smem_len(info) < buf_size * 2)) { /* 2 alg data */
        disp_buf_info->phys_addr[0] = gfbg_get_smem_start(info);
        disp_buf_info->phys_addr[1] = gfbg_get_smem_start(info);
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
        disp_buf_info->vir_addr[0] = gfbg_get_screen_base(info);
        disp_buf_info->vir_addr[1] = gfbg_get_screen_base(info);
#endif
    } else if (gfbg_get_smem_len(info) >= buf_size * 2) { /* 2 alg data */
        disp_buf_info->phys_addr[0] = gfbg_get_smem_start(info);
        disp_buf_info->phys_addr[1] = gfbg_get_smem_start(info) + buf_size;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
        disp_buf_info->vir_addr[0] = gfbg_get_screen_base(info);
        disp_buf_info->vir_addr[1] = gfbg_get_screen_base(info) + buf_size;
#endif
    }
    return;
}

static td_s32 gfbg_refresh_1buf_prepare_dst(ot_fb_buf *dst_rect, const gfbg_par *par,
                                            const gfbg_display_info *display_info,
                                            const gfbg_dispbuf_info *display_buf_info, const struct fb_info *info)
{
    td_u32 bytes_per_pixel = 2;

    dst_rect->canvas.format = par->color_format;
    dst_rect->canvas.height = display_info->display_height;
    dst_rect->canvas.width = display_info->display_width;
    dst_rect->canvas.pitch = gfbg_get_line_length(info);

    if (display_info->rotate_mode  == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270) {
        dst_rect->canvas.height = display_info->display_width;
        dst_rect->canvas.width = display_info->display_height;
        if (dst_rect->canvas.format == OT_FB_FORMAT_ARGB1555 || dst_rect->canvas.format == OT_FB_FORMAT_ARGB4444) {
            bytes_per_pixel = 2; /* 2 for bit */
        } else if (dst_rect->canvas.format == OT_FB_FORMAT_ARGB8888) {
            bytes_per_pixel = 4; /* 4 for bit */
        }
        dst_rect->canvas.pitch = ((bytes_per_pixel * dst_rect->canvas.width + GFBG_ALIGN - 1) /
                                 GFBG_ALIGN) * GFBG_ALIGN;

        if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
            (par->color_format != OT_FB_FORMAT_ARGB8888)) {
            gfbg_error("rotate only support ARGB4444, ARGB1555, ARGB8888 which is %d\n", par->color_format);
            return TD_FAILURE;
        }
        dst_rect->canvas.phys_addr = par->rotate_vb;
    } else {
        /*
         * The target address is selected as the display buf configured for the interrupt,
         * which is indicated by index for interrupt.
         */
        dst_rect->canvas.phys_addr = display_buf_info->phys_addr[display_buf_info->index_for_int];

        /* If compression is not open, you do not have to configure a compressed address for this refresh */
    }
    return TD_SUCCESS;
}

static td_void gfbg_refresh_1buf_prepare_addr(gfbg_refresh_info *refresh_info, td_u32 layer_id, gfbg_osd_data *osd_data,
                                              struct fb_info *info, ot_fb_buf *dst_rect)
{
    unsigned long lock_flag;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_dispbuf_info *display_buf_info = &refresh_info->disp_buf_info;
    td_u32 buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    /* 1buf does not support compression, close compression configuration */
    refresh_info->disp_buf_info.compress = TD_FALSE;

    g_drv_ops.gfbg_drv_get_osd_data(layer_id, osd_data);

    if (osd_data->buffer_phy_addr != par->refresh_info.disp_buf_info.phys_addr[0] && display_buf_info->phys_addr[0]) {
        osal_spin_lock_irqsave(&par->lock, &lock_flag);
        par->modifying = TD_TRUE;
        /* Notify the interrupt handler to modify the display address. */
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;

        /*
         * The buf address in the display information is configured to
         * the screen display address for refreshing the screen.
         */
        refresh_info->screen_addr = display_buf_info->phys_addr[display_buf_info->index_for_int];
        /* If compression is not open, you do not have to configure a compressed address for this refresh */
        if (par->compress_info.compress_open) {
            /* Use half of the video memory instead of another buffer */
            refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 part */
        }
        display_buf_info->stride = 4 * dst_rect->canvas.width; /* 4 for argb 8888 */

        par->modifying = TD_FALSE;
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    }
}

static td_void gfbg_refresh_1buf_prepare_opt(gfbg_blit_opt *blit_opt, gfbg_par *par)
{
    blit_opt->call_back = TD_TRUE;
    /* Non-blocking mode */
    blit_opt->block = TD_FALSE;

    if (par->display_info.antiflicker_mode == GFBG_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_NONE;
    }

    blit_opt->param = &(par->layer_id);

    if (par->display_info.rotate_mode == OT_FB_ROTATE_180) {
        blit_opt->mirror_mode = OT_FB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = par->display_info.mirror_mode;
    }
}

static td_void gfbg_refresh_1buf_prepare_global_refresh(const ot_fb_buf *canvas_buf, ot_fb_buf *dst_rect,
                                                        gfbg_blit_opt *blit_opt)
{
    if (canvas_buf->canvas.height != dst_rect->canvas.height || canvas_buf->canvas.width != dst_rect->canvas.width) {
        /* Rotate 0 or 180 degrees, zoomed, then global refresh */
        blit_opt->scale = TD_TRUE;

        dst_rect->update_rect.x = 0;
        dst_rect->update_rect.y = 0;
        dst_rect->update_rect.width = dst_rect->canvas.width;
        dst_rect->update_rect.height = dst_rect->canvas.height;
    } else {
        /* Rotate 0 or 180 degrees, no zoomed, then partial refresh */
        dst_rect->update_rect = canvas_buf->update_rect;
    }
}

static td_void gfbg_refresh_1buf_prepare_compress(volatile gfbg_compress_info *compress_info, ot_fb_buf *dst_rect,
                                                  gfbg_par *par, gfbg_blit_opt *blit_opt)
{
    td_s32 ret;
    unsigned long lock_flag;
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if (compress_info->compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE to
         * indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        ret = memcpy_s((void *)&compress_info->update_rect, sizeof(ot_fb_rect), &dst_rect->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return_void(ret, &par->lock, &lock_flag);
        compress_info->update_finished = TD_FALSE;
        blit_opt->compress = TD_TRUE;
    } else {
        blit_opt->compress = TD_FALSE;
    }

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
}

static td_s32 gfbg_refresh_1buf_blit(const ot_fb_buf *canvas_buf, const ot_fb_buf *dst_rect,
    const gfbg_blit_opt *blit_opt)
{
    ot_tde_export_func *tde_export_func = TD_NULL;

    tde_export_func = func_entry(ot_tde_export_func, OT_ID_TDE);
    if ((tde_export_func == TD_NULL) || (tde_export_func->drv_tde_module_begin_job == TD_NULL) ||
        (tde_export_func->drv_tde_module_end_job == TD_NULL)) {
        gfbg_error("can't get TDE export function, it may be TDE module has not been inserted!\n");
        return TD_FAILURE;
    }
    /*
     * The user buf is used as the source by blit, and the user buf is moved to the display buf with
     * the target set (with the target showing the buff address) as the target.
     */
    if (gfbg_drv_blit(canvas_buf, dst_rect, blit_opt, TD_TRUE) < 0) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
#ifdef CONFIG_GFBG_LOW_DELAY_SUPPORT
static td_bool gfbg_refresh_check_low_delay_support(const gfbg_par *par, const ot_fb_surface *canvas)
{
    if (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_ONE &&
        (par->display_info.rotate_mode == OT_FB_ROTATE_90 ||
        par->display_info.rotate_mode == OT_FB_ROTATE_270)) {
        gfbg_info("one buf mode and 90 degree or 270 degree rotations do not support low delay!\n");
        return TD_FALSE;
    }

    if (par->refresh_info.buf_mode == OT_FB_LAYER_BUF_ONE &&
        (par->display_info.mirror_mode == OT_FB_MIRROR_VER ||
        par->display_info.mirror_mode == OT_FB_MIRROR_BOTH)) {
        gfbg_info("one buf mode and ver mirror do not support low delay!\n");
        return TD_FALSE;
    }

    if (canvas->width < GFBG_MIN_LOW_DELAY_WIDTH && canvas->height < GFBG_MIN_LOW_DELAY_HEIGHT) {
        gfbg_info("low latency is not supported when the width is less than 1920 and height is less than 1080!\n");
        return TD_FALSE;
    }
    return TD_TRUE;
}
#endif
/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static td_s32 gfbg_refresh_1buf(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_dispbuf_info *display_buf_info = &refresh_info->disp_buf_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    gfbg_osd_data osd_data;
    td_s32 ret;
    gfbg_blit_opt blit_opt = {0};
    ot_fb_buf dst_rect;

    ret = gfbg_refresh_1buf_prepare_dst(&dst_rect, par, display_info, display_buf_info, info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    gfbg_refresh_1buf_prepare_addr(refresh_info, layer_id, &osd_data, info, &dst_rect);

    gfbg_refresh_1buf_prepare_opt(&blit_opt, par);

    gfbg_refresh_1buf_prepare_global_refresh(canvas_buf, &dst_rect, &blit_opt);

    gfbg_refresh_1buf_prepare_compress(compress_info, &dst_rect, par, &blit_opt);

#ifdef CONFIG_GFBG_LOW_DELAY_SUPPORT
    if (is_layer_support_low_delay(layer_id) == TD_TRUE &&
        gfbg_refresh_check_low_delay_support(par, &(canvas_buf->canvas)) == TD_TRUE) {
        blit_opt.is_sync = TD_TRUE;
        blit_opt.block = TD_TRUE;
    }
#endif

    ret = gfbg_refresh_1buf_blit(canvas_buf, &dst_rect, &blit_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    display_buf_info->refresh_handle = ret;
    ret = memcpy_s(&(refresh_info->user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    gfbg_unequal_eok_return(ret);
    return TD_SUCCESS;
}

/* unit rect */
static td_void gfbg_unite_rect(ot_fb_rect *dst_rect, const ot_fb_rect *src_rect)
{
    td_s32 ret;
    ot_fb_rect rect;
    rect.x = (dst_rect->x < src_rect->x) ? dst_rect->x : src_rect->x;
    rect.y = (dst_rect->y < src_rect->y) ? dst_rect->y : src_rect->y;
    rect.width = ((dst_rect->x + dst_rect->width) > (src_rect->x + src_rect->width)) ?
        (dst_rect->x + dst_rect->width - rect.x) : (src_rect->x + src_rect->width - rect.x);
    rect.height = ((dst_rect->y + dst_rect->height) > (src_rect->y + src_rect->height)) ?
        (dst_rect->y + dst_rect->height - rect.y) : (src_rect->y + src_rect->height - rect.y);
    ret = memcpy_s(dst_rect, sizeof(ot_fb_rect), &rect, sizeof(ot_fb_rect));
    gfbg_unequal_eok_return_void(ret);
    return;
}

/* check these two rectangle cover each other */
static td_bool gfbg_iscontain(ot_fb_rect parent_rect, ot_fb_rect child_rect)
{
    ot_fb_point point;
    point.x_pos = child_rect.x;
    point.y_pos = child_rect.y;
    if ((point.x_pos < parent_rect.x) || (point.x_pos > (parent_rect.x + parent_rect.width)) ||
        (point.y_pos < parent_rect.y) || (point.y_pos > (parent_rect.y + parent_rect.height))) {
        return TD_FALSE;
    }
    point.x_pos = child_rect.x + child_rect.width;
    point.y_pos = child_rect.y + child_rect.height;
    if ((point.x_pos < parent_rect.x) || (point.x_pos > (parent_rect.x + parent_rect.width)) ||
        (point.y_pos < parent_rect.y) || (point.y_pos > (parent_rect.y + parent_rect.height))) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

static td_s32 refresh_2buf_prepare_back_buf(const struct fb_info *info, ot_fb_buf *back_buf, td_u32 *bytes_per_pixel)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_display_info *display_info = &par->display_info;

    refresh_info->disp_buf_info.need_flip = TD_FALSE;
    refresh_info->disp_buf_info.refresh_handle = 0;

    back_buf->canvas.format = par->color_format;
    back_buf->canvas.height = display_info->display_height;
    back_buf->canvas.width = display_info->display_width;
    back_buf->canvas.pitch = gfbg_get_line_length(info);

    if (display_info->rotate_mode  == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270) {
        back_buf->canvas.width = display_info->display_height;
        back_buf->canvas.height = display_info->display_width;
        if (back_buf->canvas.format == OT_FB_FORMAT_ARGB1555 || back_buf->canvas.format == OT_FB_FORMAT_ARGB4444) {
            *bytes_per_pixel = 2; /* 2 bit per pixel */
        } else if (back_buf->canvas.format == OT_FB_FORMAT_ARGB8888) {
            *bytes_per_pixel = 4; /* 4 bit per pixel */
        }
        back_buf->canvas.pitch = ((*bytes_per_pixel * back_buf->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) *
                                  GFBG_ALIGN;

        if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
            (par->color_format != OT_FB_FORMAT_ARGB8888)) {
            gfbg_error("The rotate mode only support FB_FORMAT_ARGB4444,FB_FORMAT_ARGB1555,"
                       "FB_FORMAT_ARGB8888 which is %d\n!\n", par->color_format);
            return TD_FAILURE;
        }
        back_buf->canvas.phys_addr = par->rotate_vb;
    } else {
        /* Set the background buf as the target, get the free buf to the background buf */
        gfbg_get_idledispbuf(par, (td_phys_addr_t*)(&back_buf->canvas.phys_addr));
    }
    return TD_SUCCESS;
}

static td_void refresh_2buf_get_new_rect(const ot_fb_buf *canvas_buf, const ot_fb_buf *back_buf,
                                         ot_fb_rect *new_union_rect, gfbg_blit_opt *blit_opt)
{
    drv_tde_rect src_rect = {0};
    drv_tde_rect dst_rect = {0};
    drv_tde_rect inner_src_rect = {0};
    drv_tde_rect inner_dst_rect = {0};

    if (canvas_buf->canvas.height != back_buf->canvas.height || canvas_buf->canvas.width != back_buf->canvas.width) {
        src_rect.width = canvas_buf->canvas.width;
        src_rect.height = canvas_buf->canvas.height;
        dst_rect.width = back_buf->canvas.width;
        dst_rect.height = back_buf->canvas.height;
        inner_src_rect.pos_x = canvas_buf->update_rect.x;
        inner_src_rect.pos_y = canvas_buf->update_rect.y;
        inner_src_rect.width = (td_u32)canvas_buf->update_rect.width;
        inner_src_rect.height = (td_u32)canvas_buf->update_rect.height;
        tde_cal_scale_rect_gfbg(&src_rect, &dst_rect, &inner_src_rect, &inner_dst_rect);

        new_union_rect->x = inner_dst_rect.pos_x;
        new_union_rect->y = inner_dst_rect.pos_y;
        new_union_rect->width = (td_s32)inner_dst_rect.width;
        new_union_rect->height = (td_s32)inner_dst_rect.height;
        blit_opt->scale = TD_TRUE;
    } else {
        *new_union_rect = canvas_buf->update_rect;
    }
    return;
}

static td_s32 refresh_2buf_blit(gfbg_par *par, td_phys_addr_t osd_buf_addr, ot_fb_buf *back_buf,
    ot_fb_rect *new_union_rect)
{
    td_s32 ret;
    gfbg_blit_opt tmp = {0};
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_buf fore_buf = {0};
    td_phys_addr_t work_buf_addr = 0;

    gfbg_get_workdispbuf(par, &work_buf_addr);

    if ((refresh_info->disp_buf_info.fliped == TD_FALSE) ||
        ((osd_buf_addr != work_buf_addr) && (par->compress_info.compress_open  == TD_FALSE))) {
        return TD_SUCCESS;
    }
    /* Background as a target pointing to an idle buf */
    ret = memcpy_s(&fore_buf, sizeof(ot_fb_buf), back_buf, sizeof(ot_fb_buf));
    gfbg_unequal_eok_return(ret);
    /* Foreground as a source points to the buf at work */
    gfbg_get_workdispbuf(par, (td_phys_addr_t*)(&fore_buf.canvas.phys_addr));
    /* The union rect is also used as an update area for the foreground and background. */
    ret = memcpy_s(&fore_buf.update_rect, sizeof(ot_fb_rect), &refresh_info->disp_buf_info.union_rect,
        sizeof(ot_fb_rect));
    gfbg_unequal_eok_return(ret);
    ret = memcpy_s(&back_buf->update_rect, sizeof(ot_fb_rect), &fore_buf.update_rect, sizeof(ot_fb_rect));
    gfbg_unequal_eok_return(ret);
    (td_void)memset_s(&tmp, sizeof(gfbg_blit_opt), 0x0, sizeof(gfbg_blit_opt));
    /* blit with union rect */
    if ((display_info->rotate_mode != OT_FB_ROTATE_90) && (display_info->rotate_mode != OT_FB_ROTATE_270)) {
        if (gfbg_iscontain(*new_union_rect, refresh_info->disp_buf_info.union_rect) == TD_FALSE) {
            if (gfbg_drv_blit(&fore_buf, back_buf, &tmp, TD_TRUE) < 0) {
                gfbg_error("blit err!\n");
                return TD_FAILURE;
            }
        }
    }
    /* clear union rect */
    (td_void)memset_s(&(refresh_info->disp_buf_info.union_rect), sizeof(ot_fb_rect), 0, sizeof(ot_fb_rect));

    refresh_info->disp_buf_info.fliped = TD_FALSE;
    return TD_SUCCESS;
}

static td_void refresh_2buf_prepare_opt(gfbg_par *par, const ot_fb_buf *canvas_buf, ot_fb_buf *back_buf,
                                        td_u32 bytes_per_pixel, gfbg_blit_opt *blit_opt)
{
    gfbg_display_info *display_info = &par->display_info;
    blit_opt->call_back = TD_TRUE;
    blit_opt->param = &(par->layer_id);

    if (display_info->antiflicker_mode == GFBG_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_NONE;
    }
    if (blit_opt->scale == TD_TRUE) {
        /* actual area, calculate by TDE, here is just use for let pass the test */
        back_buf->update_rect.x = 0;
        back_buf->update_rect.y = 0;
        back_buf->update_rect.width = back_buf->canvas.width;
        back_buf->update_rect.height = back_buf->canvas.height;
    } else {
        back_buf->update_rect = canvas_buf->update_rect;
    }

    if (par->display_info.rotate_mode == OT_FB_ROTATE_90 || par->display_info.rotate_mode == OT_FB_ROTATE_270) {
        back_buf->canvas.height = par->display_info.display_width;
        back_buf->canvas.width = par->display_info.display_height;
        back_buf->canvas.pitch = ((bytes_per_pixel * back_buf->canvas.width +
                                  (GFBG_ALIGN - 1)) / GFBG_ALIGN) * GFBG_ALIGN;
        back_buf->update_rect = canvas_buf->update_rect;
    }

    if (display_info->rotate_mode == OT_FB_ROTATE_180) {
        blit_opt->mirror_mode = OT_FB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = display_info->mirror_mode;
    }
    return;
}

static td_s32 refresh_2buf_prepare_compress(gfbg_par *par, const ot_fb_buf *back_buf, gfbg_blit_opt *blit_opt)
{
    unsigned long lock_flag;
    td_s32 ret;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if (par->compress_info.compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE
         * to indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        ret = memcpy_s((void *)&par->compress_info.update_rect, sizeof(ot_fb_rect), &back_buf->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
        par->compress_info.update_finished = TD_FALSE;
        blit_opt->compress = TD_TRUE;
    } else if (par->compress_info.is_economize_memory == TD_TRUE) {
        gfbg_error("memory not enough ,should be start compress!\n");
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    } else {
        blit_opt->compress = TD_FALSE;
    }

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_void refresh_2buf_update_rect(gfbg_refresh_info *refresh_info, const ot_fb_rect *new_union_rect)
{
    td_s32 ret;
    if ((refresh_info->disp_buf_info.union_rect.width == 0) || (refresh_info->disp_buf_info.union_rect.height == 0)) {
        ret = memcpy_s(&refresh_info->disp_buf_info.union_rect, sizeof(ot_fb_rect), new_union_rect, sizeof(ot_fb_rect));
        gfbg_unequal_eok_return_void(ret);
    } else {
        gfbg_unite_rect(&refresh_info->disp_buf_info.union_rect, new_union_rect);
    }
    return;
}

static td_void gfbg_dynamic_set_compress(const struct fb_info *info)
{
    td_u32 uncompress_stride;
    gfbg_par *par = (gfbg_par *)info->par;

    uncompress_stride = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
        GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    if (uncompress_stride == info->fix.line_length) {
        /* last and next frame are uncompress */
        return;
    }

    if (gfbg_check_memory_enough(info, uncompress_stride) == TD_TRUE) {
        /* memory enough to compress to uncompress */
        return;
    } else {
        if (par->compress_info.compress_open == TD_FALSE) {
            par->compress_info.compress_open = TD_TRUE;
            gfbg_error("memory not enough ,should not be refresh by uncompress!\n");
        }
    }
    return;
}

/* This function has a lock operation, so you can't call it if the caller has a lock operation. */
static td_s32 gfbg_refresh_2buf(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_blit_opt blit_opt = {0};
    ot_fb_buf back_buf = {0};
    ot_fb_rect new_union_rect = {0};
    unsigned long lock_flag;
    td_phys_addr_t osd_buf_addr;
    td_u32 bytes_per_pixel = 2;
    td_s32 ret;
    /*
     * Refresh task submitted between VO vertical timing interrupt and frame start interrupt
     * Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
     * Blocked here, it is forbidden to submit the refresh task during this time.
     */
    ret = wait_event_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job,
    (td_s32)msecs_to_jiffies(40)); /* 40 for timeout */
    if (ret == 0) {
        gfbg_error("wait timeout!\n");
    }

    refresh_info->refresh_num++;

    /* get osd buffer addr */
    g_drv_ops.gfbg_drv_get_layer_addr(layer_id, &osd_buf_addr);
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    /* dynamic change compress,when compress to uncompress, reset state */
    gfbg_dynamic_set_compress(info);
    /* prepare back buf and get bytes_per_pixel */
    if (refresh_2buf_prepare_back_buf(info, &back_buf, &bytes_per_pixel) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    /* according to the hw arithmetic, calculate source and dst fresh rectangle */
    refresh_2buf_get_new_rect(canvas_buf, &back_buf, &new_union_rect, &blit_opt);
    /*
     * We should check is address changed,
     * for make sure that the address configured to the hw register is in effec
     */
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    /* refresh_2buf_blit */
    if (refresh_2buf_blit(par, osd_buf_addr, &back_buf, &new_union_rect) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    /* update union rect */
    refresh_2buf_update_rect(refresh_info, &new_union_rect);

    /* prepare opt */
    refresh_2buf_prepare_opt(par, canvas_buf, &back_buf, bytes_per_pixel, &blit_opt);

    /* prepare compress */
    if (refresh_2buf_prepare_compress(par, &back_buf, &blit_opt) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* blit with refresh rect */
    ret = gfbg_drv_blit(canvas_buf, &back_buf, &blit_opt, TD_TRUE);
    if (ret < 0) {
        gfbg_error("blit err:0x%x!\n", ret);
        return TD_FAILURE;
    }

    refresh_info->disp_buf_info.refresh_handle = ret;

    ret = memcpy_s(&(refresh_info->user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    gfbg_unequal_eok_return(ret);
    return TD_SUCCESS;
}

static td_s32 gfbg_wait_regconfig_work(td_u32 layer_id)
{
    td_s32 ret;
    gfbg_par *par = TD_NULL;

    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)g_layer[layer_id].info->par;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }
    par->vblflag = 0;
    /* Assuming TDE is fast enough, 40ms */
    ret = wait_event_timeout(par->vbl_event, par->vblflag, (td_s32)msecs_to_jiffies(40));
    if (ret == 0) {
        gfbg_error("Wait vblank failed!");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 refresh_2buf_immediate_prepare_back_buf(const struct fb_info *info, ot_fb_buf *back_buf,
                                                      td_u32 *bytes_per_pixel, td_u32 index)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

    /* forbid changing display buffer in interrupt handle */
    refresh_info->disp_buf_info.fliped = TD_FALSE;
    refresh_info->disp_buf_info.need_flip = TD_FALSE;
    refresh_info->disp_buf_info.refresh_handle = 0;

    back_buf->canvas.format = par->color_format;
    back_buf->canvas.height = display_info->display_height;
    back_buf->canvas.width = display_info->display_width;
    back_buf->canvas.pitch = gfbg_get_line_length(info);
    if (display_info->rotate_mode == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270) {
        back_buf->canvas.width = display_info->display_height;
        back_buf->canvas.height = display_info->display_width;
        if (back_buf->canvas.format == OT_FB_FORMAT_ARGB1555 || back_buf->canvas.format == OT_FB_FORMAT_ARGB4444) {
            *bytes_per_pixel = 2; /* 2 depth per pixel */
        } else if (back_buf->canvas.format == OT_FB_FORMAT_ARGB8888) {
            *bytes_per_pixel = 4; /* 4 depth per pixel */
        }
        back_buf->canvas.pitch = (((*bytes_per_pixel)*back_buf->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) *
                                  GFBG_ALIGN;
        if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
            (par->color_format != OT_FB_FORMAT_ARGB8888)) {
            gfbg_error("The rotate mode only support ARGB4444 and ARGB1555 which is %d\n!\n", par->color_format);
            return TD_FAILURE;
        }
        back_buf->canvas.phys_addr = par->rotate_vb;
    } else {
        back_buf->canvas.phys_addr = refresh_info->disp_buf_info.phys_addr[1 - index];
    }
    return TD_SUCCESS;
}

static td_s32 refresh_2buf_imediate_blit(gfbg_par *par, ot_fb_buf *back_buf, ot_fb_rect *new_union_rect, td_u32 index)
{
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    ot_fb_buf fore_buf = {0};
    gfbg_blit_opt tmp_opt = {0};
    unsigned long lock_flag;
    td_s32 ret;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if (display_info->rotate_mode != OT_FB_ROTATE_90 && display_info->rotate_mode != OT_FB_ROTATE_270) {
        /*
         * because reverse, the 2 buffer needed to sync contain,
         * if the fresh area has cover last fresh area, then no need to sync
         */
        if (!gfbg_iscontain(*new_union_rect, refresh_info->disp_buf_info.union_rect) &&
            refresh_info->disp_buf_info.union_rect.width && refresh_info->disp_buf_info.union_rect.height) {
            ret = memcpy_s(&fore_buf, sizeof(ot_fb_buf), back_buf, sizeof(ot_fb_buf));
            gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
            fore_buf.canvas.phys_addr = refresh_info->disp_buf_info.phys_addr[index];
            ret = memcpy_s(&fore_buf.update_rect, sizeof(ot_fb_rect), &refresh_info->disp_buf_info.union_rect,
                sizeof(ot_fb_rect));
            gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
            ret = memcpy_s(&back_buf->update_rect, sizeof(ot_fb_rect), &fore_buf.update_rect, sizeof(ot_fb_rect));
            gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            if (gfbg_drv_blit(&fore_buf, back_buf, &tmp_opt, TD_TRUE) < 0) {
                return TD_FAILURE;
            }
            osal_spin_lock_irqsave(&par->lock, &lock_flag);
        }
    }
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 refresh_2buf_imediate_prepare_opt(gfbg_par *par, const ot_fb_buf *canvas_buf, ot_fb_buf *back_buf,
                                                gfbg_blit_opt *blit_opt)
{
    gfbg_display_info *display_info = &par->display_info;
    td_s32 ret;

    blit_opt->call_back = TD_TRUE;
    blit_opt->param = &(par->layer_id);
    blit_opt->block = TD_TRUE;

    if (display_info->antiflicker_mode == GFBG_ANTIFLICKER_TDE) {
        blit_opt->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_NONE;
    }

    if (blit_opt->scale == TD_TRUE) {
        /* actual area, calculate by TDE, here is just use for let pass the test */
        back_buf->update_rect.x = 0;
        back_buf->update_rect.y = 0;
        back_buf->update_rect.width = back_buf->canvas.width;
        back_buf->update_rect.height = back_buf->canvas.height;
    } else {
        back_buf->update_rect = canvas_buf->update_rect;
    }

    if (display_info->rotate_mode == OT_FB_ROTATE_180) {
        blit_opt->mirror_mode = OT_FB_MIRROR_BOTH;
    } else {
        blit_opt->mirror_mode = display_info->mirror_mode;
    }

    if (par->compress_info.compress_open) {
        /*
         * This is just updating the refresh area. The refresh flag is first set to FALSE to
         * indicate that the TDE has not been moved yet, and is set to TRUE in the TDE callback.
         */
        ret = memcpy_s((void *)&par->compress_info.update_rect, sizeof(ot_fb_rect), &back_buf->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unequal_eok_return(ret);
        par->compress_info.update_finished = TD_FALSE;
        blit_opt->compress = TD_TRUE;
    } else if (par->compress_info.is_economize_memory == TD_TRUE) {
        gfbg_error("memory not enough ,should be start compress!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void refresh_2buf_imediate_backup_buffer(const struct fb_info *info, const ot_fb_buf *back_buf,
                                                   const ot_fb_rect *new_union_rect, const gfbg_blit_opt *blit_opt,
                                                   td_u32 *index)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    td_u32 buf_size;
    unsigned long lock_flag;
    td_s32 ret;
    ot_unused(back_buf);
    ot_unused(blit_opt);

    buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    *index = 1 - *index;
    refresh_info->disp_buf_info.index_for_int = *index;
    par->modifying = TD_TRUE;
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    refresh_info->screen_addr = refresh_info->disp_buf_info.phys_addr[*index];
    refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 for half */
    refresh_info->disp_buf_info.compress = compress_info->compress_open;
    par->modifying = TD_FALSE;

    if (par->compress_info.compress_open) {
        par->compress_info.update_finished = TD_TRUE;
        ret = memcpy_s((ot_fb_rect*)(&par->compress_info.update_rect), sizeof(ot_fb_rect), new_union_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return_void(ret, &par->lock, &lock_flag);
        /*
         * When blocking, if there is a callback,
         * still get FrameSize0 and FrameSize1 here as early as possible.
         */
    }

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return;
}

/*
 * In this function we should wait the new contain has been show on the screen before return,
 * and the operations such as address configuration no needed do in interrupt handle
 */
static td_s32 gfbg_refresh_2buf_immediate_display(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_blit_opt blit_opt = {0};
    td_u32 index = refresh_info->disp_buf_info.index_for_int;
    ot_fb_buf back_buf = {0};
    td_s32 ret;
    ot_fb_rect new_union_rect = {0};
    unsigned long lock_flag;
    td_u32 bytes_per_pixel = 2;

    /*
     * Refresh task submitted between VO vertical timing interrupt and frame start interrupt
     * Will cause TDE/VGS to write the buffer being displayed, and a split screen will appear.
     * Blocked here, it is forbidden to submit the refresh task during this time.
     */
    if (wait_event_timeout(par->do_refresh_job, par->refresh_info.do_refresh_job,
        (td_s32)msecs_to_jiffies(40)) == 0) { /* 40 for timeout */
        osal_printk("Func:%s, Line:%d, wait event timeout.\n", __FUNCTION__, __LINE__);
    }
    /*
     * TDE use the blocking mode
     * Immediate refresh mode requires blocking mode, non-callback mode,
     * avoiding callback mode when scrolling subtitles.
     * Return immediately. Maybe the next time the user quickly call the interface refresh,
     * it may flush the last content.
     */
    refresh_info->refresh_num++;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    /* dynamic change compress,when compress to uncompress, reset state */
    gfbg_dynamic_set_compress(info);
    /* prepare for back buf and get bytes per pixel */
    if (refresh_2buf_immediate_prepare_back_buf(info, &back_buf, &bytes_per_pixel, index) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }
    /* according to the hw arithmetic, calculate  source and Dst fresh rectangle */
    refresh_2buf_get_new_rect(canvas_buf, &back_buf, &new_union_rect, &blit_opt);
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    /* blit */
    if (refresh_2buf_imediate_blit(par, &back_buf, &new_union_rect, index) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    /* record the fresh area */
    ret = memcpy_s(&refresh_info->disp_buf_info.union_rect, sizeof(ot_fb_rect), &new_union_rect, sizeof(ot_fb_rect));
    gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
    /* prepare for opt */
    if (refresh_2buf_imediate_prepare_opt(par, canvas_buf, &back_buf, &blit_opt) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    /* blit with refresh rect */
    if (gfbg_drv_blit(canvas_buf, &back_buf, &blit_opt, TD_TRUE) < 0) {
        return TD_FAILURE;
    }
    refresh_info->disp_buf_info.refresh_handle = ret;
    /* set the backup buffer to register and show it  */
    refresh_2buf_imediate_backup_buffer(info, &back_buf, &new_union_rect, &blit_opt, &index);
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    ret = memcpy_s(&(refresh_info->user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
    par->vblflag = 0;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    /* wait the address register's configuration take effect before return */
    if (in_atomic() == TD_FALSE) {
        gfbg_wait_regconfig_work(layer_id);
    }

    return TD_SUCCESS;
}

static td_void gfbg_refresh_again(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    ot_fb_buf canvas;

    /* Prerequisites for the canvas to be refreshed */
    if (!(par->param_modify_mask & GFBG_LAYER_PARAMODIFY_INRECT)) {
        return;
    }

    if (refresh_info->user_buffer.canvas.phys_addr == 0) {
        return;
    }

    if (refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) {
        return;
    }
    /* Fills the canvas object with refresh information from private data for refresh. */
    canvas = refresh_info->user_buffer;
    canvas.update_rect.x = 0;
    canvas.update_rect.y = 0;
    canvas.update_rect.width = canvas.canvas.width;
    canvas.update_rect.height = canvas.canvas.height;
    gfbg_refresh(layer_id, &canvas, refresh_info->buf_mode);
}

static td_s32 gfbg_disp_check_param(const struct fb_info *info, td_u32 width, td_u32 height)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_size max_screen_size = {0};
    td_u32 pitch;

    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        gfbg_get_maxscreensize(par, &max_screen_size.width, &max_screen_size.height);
#ifdef CONFIG_GFBG_RESOLUTION_SUPPORT_SQUARE
        max_screen_size.height = g_drv_ops.capability[par->layer_id].max_height;
#endif
        if ((par->layer_id == 3 || par->layer_id == 4) && /* 3 G3 clut2 and clut4 support 4K */
            ((gfbg_get_bits_per_pixel(info) == 4) || (gfbg_get_bits_per_pixel(info) == 2))) { /* 4,2 bits */
            if ((width > max_screen_size.width) || (height > max_screen_size.height) ||
                (width > GFBG_MAX_LAYER_WIDTH_CLUT_G3) || (height > GFBG_MAX_LAYER_HEIGTH_CLUT_G3)) {
                gfbg_error("(%u, %u)larger than the max size of the screen(%u, %u) or the layer(%u, %u)!\n", width,
                           height, max_screen_size.width, max_screen_size.height, GFBG_MAX_LAYER_WIDTH_CLUT_G3,
                           GFBG_MAX_LAYER_HEIGTH_CLUT_G3);
                return TD_FAILURE;
            }
        } else {
            if ((width > max_screen_size.width) || (height > max_screen_size.height) ||
                (width > g_drv_ops.capability[par->layer_id].max_width) ||
                (height > g_drv_ops.capability[par->layer_id].max_height)) {
                gfbg_error("(%u, %u)larger than the max size of the screen(%u, %u) or the layer(%u, %u)!\n", width,
                           height, max_screen_size.width, max_screen_size.height,
                           g_drv_ops.capability[par->layer_id].max_width,
                           g_drv_ops.capability[par->layer_id].max_height);
                return TD_FAILURE;
            }
        }
    }
    /* 3 is 8 bits */
    pitch = (((width * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    if (gfbg_check_mem_enough(info, pitch, height) != TD_SUCCESS) {
        gfbg_error("++ setdispsize .memory is not enough!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * This function has a lock inside, and there is a call to the sleep function.
 * Before calling this function, you must first unlock the spin lock.
 */
#ifdef __LITEOS__
static td_s32 gfbg_disp_setdispsize(td_u32 layer_id, td_u32 width, td_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    td_u32 pitch;
    unsigned long lock_flag;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if ((display_info->display_width == width) && (display_info->display_height == height)) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_SUCCESS;
    }
    /*
     * for width and height check
     * width and height should less than max_screen_size
     */
    if (gfbg_disp_check_param(info, width, height) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }

    display_info->display_width = width;
    display_info->display_height = height;
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_INRECT;

    pitch = (((width * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT); /* 3: 8bits */
    if (pitch > info->oinfo.stride) {
        info->oinfo.stride = pitch;
        info->oinfo.sarea.height = info->vinfo.yres = display_info->display_height;
        info->oinfo.sarea.width = info->vinfo.xres = display_info->display_width;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    }

    gfbg_set_dispbufinfo(layer_id);

    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        display_info->screen_width = width;
        display_info->screen_height = height;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;
    }

    /*
     * here we need to think about how to resist flicker again,
     * we use VO do flicker resist before , but now if the display H size is the same as the screen,
     * VO will not do flicker resist, so should choose TDE to do flicker resist
     */
    gfbg_select_antiflicker_mode(par);
    osal_spin_unlock_irqrestore(&par->lock, lock_flag);

    return TD_SUCCESS;
}
#else
static td_void gfbg_fill_var_info(gfbg_display_info *display_info, struct fb_var_screeninfo *var)
{
    var->xres = display_info->display_width;
    var->yres = display_info->display_height;
    if (var->xres_virtual < display_info->display_width) {
        var->xres_virtual = display_info->display_width;
    }

    if (var->yres_virtual < display_info->display_height) {
        var->yres_virtual = display_info->display_height;
    }
    return;
}

static td_s32 gfbg_disp_setdispsize(td_u32 layer_id, td_u32 width, td_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    struct fb_var_screeninfo *var = &info->var;
    struct fb_fix_screeninfo *fix = &info->fix;
    gfbg_display_info *display_info = &par->display_info;
    td_u32 pitch;
    unsigned long lock_flag;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    gfbg_stride_attr attr = {0};
#endif
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if ((display_info->display_width == width) && (display_info->display_height == height)) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_SUCCESS;
    }
    /*
     * for width and height check
     * width and height should less than max_screen_size
     */
    if (gfbg_disp_check_param(info, width, height) != TD_SUCCESS) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }

    display_info->display_width = width;
    display_info->display_height = height;
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_INRECT;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = width;
    attr.format = gfbg_getfmtbyargb(&var->red, &var->green, &var->blue, &var->transp, var->bits_per_pixel);
    pitch = (((width * gfbg_get_bits_per_pixel(info))>> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT); /* 3: 8bits */
    gfbg_recalculate_stride(NULL, &pitch, &attr);
#else
    pitch = (((width * gfbg_get_bits_per_pixel(info))>> 3) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT); /* 3: 8bits */
#endif
    if (pitch > fix->line_length) {
        fix->line_length = pitch;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    }
    /*
     * If the user calls FBIOPUT_LAYER_INFO to set display_width and display_height,then sync to xres yres,
     * Otherwise, there will be an error in the memory address in gfbg_set_dispbufinfo.
     */
    gfbg_fill_var_info(display_info, var);
    gfbg_set_dispbufinfo(layer_id);

    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        display_info->screen_width = width;
        display_info->screen_height = height;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;
    }

    /*
     * here we need to think about how to resist flicker again,
     * we use VO do flicker resist before , but now if the display H size is the same as the screen,
     * VO will not do flicker resist, so should choose TDE to do flicker resist
     */
    gfbg_select_antiflicker_mode(par);
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return TD_SUCCESS;
}
#endif

/*
 * we handle it by two case:
 * case 1 : if VO support Zoom, we only change screen size, g_display size keep not change
 * case 2: if VO can't support zoom, g_display size should keep the same as screen size
 */
static td_s32 gfbg_disp_setscreensize(td_u32 layer_id, td_u32 width, td_u32 height)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;

    /* If the chip does not support scaling, it is consistent with the process of setting the display area. */
    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        return gfbg_disp_setdispsize(layer_id, width, height);
    }

    display_info->screen_width = width;
    display_info->screen_height = height;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;

    /* Here  we need to think about how to resist flicker again, we use VO do flicker resist before ,
     * but now if the g_display H size is the same as the screen, VO will not do flicker resist, so should choose
     * TDE to do flicker resist
     */
    gfbg_select_antiflicker_mode(par);

    return TD_SUCCESS;
}

static td_void gfbg_buf_freemem(td_phys_addr_t phyaddr)
{
    cmpi_mmz_free(phyaddr, TD_NULL);
}

static td_s32 gfbg_freeccanbuf(gfbg_par *par)
{
    ot_fb_surface *canvas_sur = TD_NULL;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }
    canvas_sur   = &par->canvas_sur;

    if (canvas_sur->phys_addr != 0) {
        gfbg_buf_freemem(canvas_sur->phys_addr);
    }
    canvas_sur->phys_addr = 0;

    if (par->rotate_vb != 0) {
        cmpi_mmz_free(par->rotate_vb, TD_NULL);
        par->rotate_vb = 0;
    }

    return TD_SUCCESS;
}

static td_void set_par_stride(struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    td_u32 stride;
    td_phys_addr_t display_addr;
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
    gfbg_stride_attr attr = {0};
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;
    attr.width = gfbg_get_xres_virtual(info);
    attr.format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp,
        info->var.bits_per_pixel);
    stride = (((gfbg_get_xres_virtual(info) * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & /* 2^3 */
              (~GFBG_ALIGNMENT);
    gfbg_recalculate_stride(NULL, &stride, &attr);
#else
    stride = (((gfbg_get_xres_virtual(info) * gfbg_get_bits_per_pixel(info)) >> 3) + GFBG_ALIGNMENT) & /* 2^3 */
              (~GFBG_ALIGNMENT);
#endif
    if (stride != gfbg_get_line_length(info) || (gfbg_get_yres(info) != display_info->y_res)) {
#ifdef __LITEOS__
        info->oinfo.stride = stride;
#else
        info->fix.line_length = stride;
#endif
        gfbg_set_dispbufinfo(par->layer_id);
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    }

    display_addr = (gfbg_get_smem_start(info) + stride * gfbg_get_yoffset(info) +
                    gfbg_get_xoffset(info) * (gfbg_get_bits_per_pixel(info) >> 3)) & /* 3 /8bits */
                   0xfffffff0; /* 0xfffffff0 16 align */
    if (display_addr != refresh_info->screen_addr) {
        refresh_info->screen_addr = display_addr;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    }
    return;
}

static td_s32 set_par_resolution(const struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    unsigned long lock_flag;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if (gfbg_get_xres(info) != display_info->x_res || gfbg_get_yres(info) != display_info->y_res) {
        if ((gfbg_get_xres(info) == 0) || (gfbg_get_yres(info) == 0)) {
            if (par->show == TD_TRUE) {
                par->show = TD_FALSE;
                par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;
            }
        }

         /*
          * The following two functions have a sleep operation, you must unlock before calling,
          * and lock the global variable inside the function.
          */
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        if (gfbg_disp_setdispsize(par->layer_id, gfbg_get_xres(info), gfbg_get_yres(info)) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (gfbg_disp_setscreensize(par->layer_id, gfbg_get_xres(info), gfbg_get_yres(info)) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        osal_spin_lock_irqsave(&par->lock, &lock_flag);
    }

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

/*
 * Function        : gfbg_set_par
 * Description     : set the variable parmater and make it use
 * Input           : struct fb_info *info
 * Return          : return 0
 */
static td_s32 gfbg_set_par(struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_color_format format;
    unsigned long lock_flag;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);

    par->modifying = TD_TRUE;

    /* set the stride if stride change */
    set_par_stride(info);

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    /* If xres or yres change */
    if (set_par_resolution(info) != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifdef __LITEOS__
    format = info->vinfo.format;
#else
    format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp,
                               gfbg_get_bits_per_pixel(info));
#endif
    if ((par->color_format != format)) {
        gfbg_freeccanbuf(par);
        gfbg_set_fmt(par, format);
        par->cursor_info.cursor.cursor.format = format;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_FMT;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    display_info->x_res = gfbg_get_xres(info);
    display_info->y_res = gfbg_get_yres(info);
    display_info->vir_x_res = gfbg_get_xres_virtual(info);
    display_info->vir_y_res = gfbg_get_yres_virtual(info);

    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return 0;
}

/*
 * Function        : gfbg_pan_display
 * Description     : pan g_display.
 * Input           : struct fb_var_screeninfo *var
 * Return          : return 0
 */
#ifdef __LITEOS__
static td_s32 gfbg_pan_display(struct fb_vtable_s *vtable, struct fb_overlayinfo_s *oinfo)
#else
static td_s32 gfbg_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
#endif
{
#ifdef __LITEOS__
    struct gfbg_info *info = (struct gfbg_info *)vtable;
#endif

    gfbg_par*               par = (gfbg_par *)info->par;
    gfbg_refresh_info*      refresh_info = &par->refresh_info;
    td_phys_addr_t          display_addr;
    td_u32                  stride;
    ot_fb_buf               canvas_buf;
    td_s32                  ret;

    /* set the stride and display start address */
    stride = gfbg_get_line_length(info);

#ifdef __LITEOS__
    display_addr = (gfbg_get_smem_start(info) + (td_u64)stride * oinfo->sarea.y + (td_u64)oinfo->sarea.x *
        (gfbg_get_bits_per_pixel(info) >> 3)) & 0xfffffffffffffff0; /* 3 is 8 bits */
#else
    /* 3 is 8 bits */
    display_addr = (gfbg_get_smem_start(info) + (td_u64)stride * var->yoffset + (td_u64)var->xoffset *
        (gfbg_get_bits_per_pixel(info) >> 3)) & 0xfffffffffffffff0; /* 3 is 8 bits */
#endif
    canvas_buf.canvas.format = par->color_format;
    canvas_buf.canvas.phys_addr = display_addr;
    canvas_buf.canvas.pitch = stride;
    canvas_buf.update_rect.x = 0;
    canvas_buf.update_rect.y = 0;
#ifdef __LITEOS__
    canvas_buf.canvas.width = info->vinfo.xres;
    canvas_buf.canvas.height = info->vinfo.yres;
    canvas_buf.update_rect.width = info->vinfo.xres;
    canvas_buf.update_rect.height = info->vinfo.xres;
    g_layer[par->layer_id].info->activate = info->activate;
#else
    canvas_buf.canvas.width = gfbg_get_xres(info);
    canvas_buf.canvas.height = gfbg_get_yres(info);
    canvas_buf.update_rect.width = (td_s32)gfbg_get_xres(info);
    canvas_buf.update_rect.height = (td_s32)gfbg_get_yres(info);
    g_layer[par->layer_id].info->var.activate = info->var.activate;
#endif
    refresh_info->buf_mode = OT_FB_LAYER_BUF_BUTT;
    ret = gfbg_refresh_0buf(par->layer_id, &canvas_buf);

    return ret;
}

static inline td_void gfbg_get_fmt(const gfbg_par *par, ot_fb_color_format *color_format)
{
    *color_format = par->color_format;
    return;
}

static inline td_void gfbg_set_fmt(gfbg_par *par, ot_fb_color_format color_fmt)
{
    par->color_format = color_fmt;

    return;
}

static inline td_void gfbg_set_alpha(gfbg_par *par, const ot_fb_alpha *alpha)
{
    td_s32 ret;
    ret = memcpy_s(&par->alpha, sizeof(ot_fb_alpha), alpha, sizeof(ot_fb_alpha));
    gfbg_unequal_eok_return_void(ret);
    return;
}

static inline td_void gfbg_get_alpha(const gfbg_par *par, ot_fb_alpha *alpha)
{
    td_s32 ret;
    ret = memcpy_s(alpha, sizeof(ot_fb_alpha), &par->alpha, sizeof(ot_fb_alpha));
    gfbg_unequal_eok_return_void(ret);
    return;
}

static inline td_void gfbg_set_key(gfbg_par *par, const gfbg_colorkeyex *key)
{
    td_s32 ret;
    ret = memcpy_s(&par->ckey, sizeof(gfbg_colorkeyex), key, sizeof(gfbg_colorkeyex));
    gfbg_unequal_eok_return_void(ret);
    return;
}

static inline td_void gfbg_get_key(const gfbg_par *par, gfbg_colorkeyex *key)
{
    td_s32 ret;
    ret = memcpy_s(key, sizeof(gfbg_colorkeyex), &par->ckey, sizeof(gfbg_colorkeyex));
    gfbg_unequal_eok_return_void(ret);
    return;
}

static td_void gfbg_set_layerpos(gfbg_par *par, const ot_fb_point *pos)
{
    td_s32 x_pos;
    td_s32 y_pos;
    ot_size max_screensize = {0};
    td_u32 layer_id;
    gfbg_display_info *display_info = TD_NULL;

    layer_id = par->layer_id;
    display_info = &par->display_info;

    gfbg_get_maxscreensize(par, &max_screensize.width, &max_screensize.height);
    x_pos = pos->x_pos;
    y_pos = pos->y_pos;
    if (x_pos > (td_s32)(max_screensize.width - g_drv_ops.capability[layer_id].min_width)) {
        gfbg_warning("the sum of x_pos(%d) and min_width(%d) larger than Vodev screen width(%d)!\n", x_pos,
                     g_drv_ops.capability[layer_id].min_width, max_screensize.width);
        x_pos = max_screensize.width - g_drv_ops.capability[layer_id].min_width;
    }

    if (y_pos > (td_s32)(max_screensize.height - g_drv_ops.capability[layer_id].min_height)) {
        gfbg_warning("the sum of y_pos(%d) and min_height(%d) larger than Vodev screen height(%d)!\n", y_pos,
                     g_drv_ops.capability[layer_id].min_height, max_screensize.height);
        y_pos = max_screensize.height - g_drv_ops.capability[layer_id].min_height;
    }

    display_info->pos.x_pos = x_pos;
    display_info->pos.y_pos = y_pos;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;

    return;
}
static inline td_bool gfbg_get_show(const gfbg_par *par)
{
    if (par != TD_NULL) {
        return par->show;
    } else {
        return TD_FALSE;
    }
}

static inline td_void gfbg_set_show(gfbg_par *par, td_bool show)
{
    if (par != TD_NULL) {
        par->show = show;
    }
}

static td_void gfbg_get_layerinfo(const gfbg_par *par, ot_fb_layer_info *layer_info)
{
    ot_fb_point pos = {0};
    if ((par != TD_NULL) && (layer_info != TD_NULL)) {
        gfbg_get_premul(par, &layer_info->is_premul);
        gfbg_get_bufmode(par, &layer_info->buf_mode);
        gfbg_get_antiflickerlevel(par, &layer_info->antiflicker_level);
        gfbg_get_layerpos(par, &pos);
        layer_info->x_pos = pos.x_pos;
        layer_info->y_pos = pos.y_pos;
        gfbg_get_dispsize(par, &layer_info->display_width, &layer_info->display_height);
        gfbg_get_screensize(par, &layer_info->screen_width, &layer_info->screen_height);
        layer_info->canvas_width = par->canvas_sur.width;
        layer_info->canvas_height = par->canvas_sur.height;

        layer_info->mask = OT_FB_LAYER_MASK_BUTT;
    }

    return;
}

static td_void gfbg_get_idledispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr)
{
    const gfbg_refresh_info *refresh_info = TD_NULL;
    td_u32 index_for_int;
    if ((par == TD_NULL) || (phy_addr == TD_NULL)) {
        return;
    }
    refresh_info = &par->refresh_info;
    index_for_int = refresh_info->disp_buf_info.index_for_int;
    /*
     * Only the 2buf refresh mode allows to get free buf, otherwise it is the buf in the current
     * interrupt processing.
     */
    if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE) ||
        (refresh_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
        *phy_addr = refresh_info->disp_buf_info.phys_addr[1 - index_for_int];
    } else {
        *phy_addr = refresh_info->disp_buf_info.phys_addr[index_for_int];
    }
    return;
}

static td_void gfbg_get_workdispbuf(const gfbg_par *par, td_phys_addr_t *phy_addr)
{
    const gfbg_refresh_info *refresh_info = TD_NULL;
    td_u32 index_for_int;
    if ((par == TD_NULL) || (phy_addr == TD_NULL)) {
        return;
    }
    refresh_info = &par->refresh_info;
    index_for_int = refresh_info->disp_buf_info.index_for_int;

    *phy_addr = refresh_info->disp_buf_info.phys_addr[index_for_int];

    return;
}

static td_void gfbg_get_dispsize(const gfbg_par *par, td_u32 *width, td_u32 *height)
{
    const gfbg_display_info *display_info = TD_NULL;

    display_info = &par->display_info;

    if ((width != TD_NULL) && (height != TD_NULL)) {
        *width = display_info->display_width;
        *height = display_info->display_height;
    }

    return;
}

static inline td_void gfbg_set_dispbufflip(gfbg_par *par, td_bool need_flip)
{
    par->refresh_info.disp_buf_info.need_flip = need_flip;

    return;
}

static inline td_void gfbg_get_bufmode(const gfbg_par *par, ot_fb_layer_buf *buf_mode)
{
    *buf_mode = par->refresh_info.buf_mode;

    return;
}

static inline td_void gfbg_get_premul(const gfbg_par *par, td_bool *premul)
{
    *premul = par->display_info.is_premul;

    return;
}

static inline td_void gfbg_get_antiflickerlevel(const gfbg_par *par, ot_fb_layer_antiflicker_level *antiflicker_level)
{
    *antiflicker_level = par->display_info.antiflicker_level;

    return;
}

static inline td_void gfbg_get_layerpos(const gfbg_par *par, ot_fb_point *pos)
{
    td_s32 ret;
    ret = memcpy_s(pos, sizeof(ot_fb_point), &par->display_info.pos, sizeof(ot_fb_point));
    gfbg_unequal_eok_return_void(ret);
    return;
}

static td_void gfbg_get_screensize(const gfbg_par *par, td_u32 *width, td_u32 *height)
{
    const gfbg_display_info *display_info = TD_NULL;
    if (par == TD_NULL) {
        return;
    }
    display_info = &par->display_info;

    if ((width != TD_NULL) && (height != TD_NULL)) {
        *width = display_info->screen_width;
        *height = display_info->screen_height;
    }

    return;
}

static td_s32 gfbg_set_screensize(gfbg_par *par, const td_u32 *width, const td_u32 *height)
{
    td_s32 ret;
    gfbg_display_info *display_info = TD_NULL;

    ret = TD_SUCCESS;
    if ((par == TD_NULL) || (width == TD_NULL) || (height == TD_NULL)) {
        return TD_FAILURE;
    }
    display_info = &par->display_info;

    if (g_drv_ops.capability[par->layer_id].is_vo_scale) {
        display_info->screen_width = *width;
        display_info->screen_height = *height;
    } else {
        if ((display_info->display_width != *width) || (display_info->display_height != *height)) {
            gfbg_error("ScreenSize(%u, %u) can't be different with DisplaySize(%u, %u) when layer %u don't scale\n",
                       *width, *height, display_info->display_width, display_info->display_height, par->layer_id);
            ret = TD_FAILURE;
        }
    }

    return ret;
}

static td_void gfbg_get_maxscreensize(gfbg_par *par, td_u32 *width, td_u32 *height)
{
    gfbg_osd_data layer_data;
    gfbg_display_info *display_info = TD_NULL;
    if (par == TD_NULL) {
        return;
    }
    display_info = &par->display_info;

    if (g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &layer_data) == TD_SUCCESS) {
        display_info->max_screen_width = layer_data.screen_width;
        display_info->max_screen_height = layer_data.screen_height;
    }

    if ((width != TD_NULL) && (height != TD_NULL)) {
        *width = display_info->max_screen_width;
        *height = display_info->max_screen_height;
    }

    return;
}

static td_void gfbg_tde_rotate_callback(const td_void *paraml, const td_void *paramr)
{
    td_u32 layer_id = 0;
    td_s32  tde_finish_handle;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lockflag;
    gfbg_tde_callback_param *param = (gfbg_tde_callback_param *)paraml;
    tde_finish_handle = *(td_s32 *)paramr;

    if (param != TD_NULL) {
        layer_id = param->layer_id;
    } else {
        gfbg_error("pParam is NULL\n");
        return;
    }
    if (layer_id <= g_drv_ops.layer_count) {
        par = (gfbg_par *)(g_layer[layer_id].info->par);
    } else {
        osal_kfree(param);
        param = TD_NULL;
        gfbg_error("layer_id = %d is invalid\n", layer_id);
        return;
    }
    refresh_info     = &par->refresh_info;
    gfbg_get_bufmode(par, &buf_mode);

    osal_spin_lock_irqsave(&par->lock, &lockflag);

    if ((buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle)) {
        /* Notify VO, it can be updated now */
        gfbg_set_dispbufflip(par, TD_TRUE);

        /* Compression is not supported when there is rotation */
        par->refresh_info.disp_buf_info.compress = TD_FALSE;
    }
    osal_spin_unlock_irqrestore(&par->lock, &lockflag);
    if (param != TD_NULL) {
        osal_kfree(param);
        param = TD_NULL;
    }
    return;
}

static td_void tde_callback_src_init(const gfbg_par *par, ot_fb_buf *src_img)
{
    const gfbg_display_info *display_info = &par->display_info;
    td_u32 bytes_per_pixel = 2;
    src_img->canvas.phys_addr = par->rotate_vb;
    src_img->canvas.width = display_info->display_height;
    src_img->canvas.height = display_info->display_width;
    src_img->update_rect.x = 0;
    src_img->update_rect.y = 0;
    src_img->update_rect.width = src_img->canvas.width;
    src_img->update_rect.height = src_img->canvas.height;

    src_img->canvas.format = par->color_format;
    if (src_img->canvas.format == OT_FB_FORMAT_ARGB1555 || src_img->canvas.format == OT_FB_FORMAT_ARGB4444) {
        bytes_per_pixel = 2; /* 2 is PerPixel */
    } else if (src_img->canvas.format == OT_FB_FORMAT_ARGB8888) {
        bytes_per_pixel = 4; /* 4 is PerPixel */
    }
    src_img->canvas.pitch = ((bytes_per_pixel * src_img->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) * GFBG_ALIGN;
    return;
}

static td_void tde_callback_dst_init(const gfbg_par *par, ot_fb_buf *dst_img, ot_fb_layer_buf buf_mode)
{
    td_u32 bytes_per_pixel = 2;
    const gfbg_refresh_info *refresh_info = &par->refresh_info;
    const gfbg_display_info *display_info = &par->display_info;
    if (buf_mode == OT_FB_LAYER_BUF_DOUBLE) {
        gfbg_get_idledispbuf(par, (td_phys_addr_t*)(&dst_img->canvas.phys_addr));
    } else if (buf_mode == OT_FB_LAYER_BUF_ONE) {
        dst_img->canvas.phys_addr =
            refresh_info->disp_buf_info.phys_addr[refresh_info->disp_buf_info.index_for_int];
    } else if (buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE) {
        dst_img->canvas.phys_addr =
            refresh_info->disp_buf_info.phys_addr[1 - refresh_info->disp_buf_info.index_for_int];
    }
    dst_img->canvas.width = display_info->display_width;
    dst_img->canvas.height = display_info->display_height;
    dst_img->canvas.format = par->color_format;
    dst_img->update_rect.x = 0;
    dst_img->update_rect.y = 0;
    dst_img->update_rect.width = dst_img->canvas.width;
    dst_img->update_rect.height = dst_img->canvas.height;
    if (dst_img->canvas.format == OT_FB_FORMAT_ARGB1555 || dst_img->canvas.format == OT_FB_FORMAT_ARGB4444) {
        bytes_per_pixel = 2; /* 2 is PerPixel */
    } else if (dst_img->canvas.format == OT_FB_FORMAT_ARGB8888) {
        bytes_per_pixel = 4; /* 4 is PerPixel */
    }
    dst_img->canvas.pitch = ((bytes_per_pixel * dst_img->canvas.width + GFBG_ALIGN - 1) / GFBG_ALIGN) * GFBG_ALIGN;
    return;
}

static td_void tde_callback_with_rotate(gfbg_par *par, ot_fb_layer_buf buf_mode)
{
    ot_fb_buf src_img;
    ot_fb_buf dst_img;
    gfbg_rotate_opt rot_opt;
    td_bool is_refresh_screen;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

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
    rot_opt.call_back = (buf_mode == OT_FB_LAYER_BUF_DOUBLE) ? TD_TRUE : TD_FALSE;
    /* In the interrupt, the blocking mode is not allowed, so the non-blocking mode is used. */
    rot_opt.block = TD_FALSE;
    rot_opt.param = &(par->layer_id);

    /* Fill is_refresh_screen.Not used now */
    is_refresh_screen = TD_TRUE;
    if (gfbg_get_rotation() != TD_NULL) {
        refresh_info->disp_buf_info.refresh_handle = gfbg_get_rotation()(&src_img, &dst_img, &rot_opt,
            is_refresh_screen);
    }
    return;
}

static td_void tde_callback_without_rotate(gfbg_par *par, ot_fb_layer_buf buf_mode, const td_void *paraml,
                                           const td_void *paramr)
{
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    gfbg_tde_callback_param *param = (gfbg_tde_callback_param *)paraml;
    td_s32 tde_finish_handle = *(td_s32 *)paramr;

    if ((buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.refresh_handle == tde_finish_handle)) {
        /* Notify VO, can be updated */
        gfbg_set_dispbufflip(par, TD_TRUE);
        par->refresh_info.disp_buf_info.compress = param->compress;
    }
    par->compress_info.update_finished = TD_TRUE;
    return;
}

static td_void gfbg_tde_callback(const td_void *paraml, const td_void *paramr)
{
    td_u32 layer_id = 0;
    td_s32 tde_finish_handle;
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_layer_buf buf_mode;
    unsigned long lock_flag;
    gfbg_tde_callback_param *param = (gfbg_tde_callback_param *)paraml;
    tde_finish_handle = *(td_s32 *)paramr;
    /* here get a wrong layer_id, make gfbg core dump here */
    if (param != TD_NULL) {
        layer_id = param->layer_id;
    } else {
        gfbg_error("paraml is NULL\n");
        return;
    }
    if (layer_id <= g_drv_ops.layer_count) {
        info = g_layer[layer_id].info;
    } else {
        osal_kfree(param);
        param = TD_NULL;
        gfbg_error("layer_id = %d is invalid\n", layer_id);
        return;
    }
    par             = (gfbg_par *)(info->par);
    refresh_info     = &par->refresh_info;
    display_info     = &par->display_info;
    gfbg_info("tde callback blit handle:%x, end handle:%x\n", refresh_info->disp_buf_info.refresh_handle,
              tde_finish_handle);

    gfbg_get_bufmode(par, &buf_mode);
    osal_spin_lock_irqsave(&par->lock, &lock_flag);

    if ((display_info->rotate_mode == OT_FB_ROTATE_90 || display_info->rotate_mode == OT_FB_ROTATE_270)) {
        /* gfbg callback with rotate */
        tde_callback_with_rotate(par, buf_mode);
    } else {
        /* gfbg callback without rotate */
        tde_callback_without_rotate(par, buf_mode, paraml, paramr);
    }

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    if (param != TD_NULL) {
        osal_kfree(param);
        param = TD_NULL;
    }
    return;
}

static inline td_bool gfbg_is_interlace(const gfbg_par *par)
{
    gfbg_osd_data osd_data = {0};
    if (par != TD_NULL) {
        g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &osd_data);
    }
    return (osd_data.scan_mode == GFBG_SCANMODE_I);
}

static td_phys_addr_t gfbg_buf_allocmem(const td_char *buf_name, td_u32 buf_name_size, td_ulong layer_size,
    const td_char *mmz_name)
{
    td_phys_addr_t addr;
    mm_malloc_param malloc_param;
	ot_unused(buf_name_size);

    if ((layer_size == 0) || (layer_size > 0x40000000)) {
        return 0;
    }

    malloc_param.mmz_name = mmz_name;
    malloc_param.buf_name = buf_name;
    malloc_param.size = layer_size;
    malloc_param.kernel_only = TD_FALSE;
    addr = cmpi_mmz_malloc(&malloc_param);
    if ((addr == MMB_ADDR_INVALID) && (mmz_name != TD_NULL)) {
        malloc_param.mmz_name = TD_NULL;
        addr = cmpi_mmz_malloc(&malloc_param);
    }

    if (addr == MMB_ADDR_INVALID) {
        gfbg_error("alloc mem failed!\n");
        return 0;
    }

    return addr;
}

static td_void gfbg_set_bufmode(td_u32 layer_id, ot_fb_layer_buf layer_buf_mode)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

    /* in 0 buf mode ,maybe the stride or format will be changed! */
    if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && (refresh_info->buf_mode != layer_buf_mode)) {
        par->modifying = TD_TRUE;

        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;

        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_FMT;

        par->modifying = TD_FALSE;
    }

    refresh_info->buf_mode = layer_buf_mode;
}

/*
 * choose the module to do  flicker resiting, TDE or VOU ? the rule is as this ,the module should do flicker resisting
 * who has do scaling
 */
static td_void gfbg_select_antiflicker_mode(gfbg_par *par)
{
    gfbg_display_info *display_info = TD_NULL;

    display_info = &par->display_info;

    /* if the usr's configuration is no needed to do flicker resisting, so no needed to do it  */
   if (display_info->antiflicker_level == OT_FB_LAYER_ANTIFLICKER_NONE) {
       display_info->antiflicker_mode = GFBG_ANTIFLICKER_NONE;
   } else {
       /* current standard no needed to do flicker resisting */
       if (!display_info->need_antiflicker) {
           display_info->antiflicker_mode = GFBG_ANTIFLICKER_NONE;
       } else {
           /* VO has done scaling , so should do flicker resisting at the same time */
           if ((display_info->display_width != display_info->screen_width) ||
               (display_info->display_height != display_info->screen_height)) {
               display_info->antiflicker_mode = GFBG_ANTIFLICKER_VO;
           } else {
               display_info->antiflicker_mode = GFBG_ANTIFLICKER_TDE;
           }
       }
   }
}

static td_void gfbg_set_antiflickerlevel(td_u32 layer_id, ot_fb_layer_antiflicker_level antiflicker_level)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;

    display_info->antiflicker_level = antiflicker_level;
    gfbg_select_antiflicker_mode(par);

    return;
}

/* Does the chip support scaling */
static td_s32 gfbg_check_imagezoomenable(td_u32 layer_id, const ot_fb_rect *in_rect, const ot_fb_rect *out_rect)
{
    td_bool is_layer_support_zoom_out = TD_TRUE;
    td_bool need_zoom;

    if (!g_drv_ops.capability[layer_id].is_vo_scale) {
        return TD_FAILURE;
    }

    if (g_drv_ops.gfbg_drv_is_layer_support_zoom_out) {
        is_layer_support_zoom_out = g_drv_ops.gfbg_drv_is_layer_support_zoom_out(layer_id);
    }

    /* The chip does not support zoomout, and the current image output size is smaller than the input size. */
    if ((is_layer_support_zoom_out == TD_FALSE) &&
        ((out_rect->width < in_rect->width) || (out_rect->height < in_rect->height))) {
        gfbg_error("GFBG layer%d not support zoomout, please check the display and screen size!\n", layer_id);
        return TD_FAILURE;
    }

    /* The chip zoomin ratio exceeds the maximum allowed by the chip, and the error is returned. */
    if (out_rect->width > (in_rect->width * GFBG_MAX_ZOOMIN) ||
        out_rect->height > (in_rect->height * GFBG_MAX_ZOOMIN)) {
        gfbg_error("GFBG layer%d in_size(%d, %d) and out_size(%d, %d) do out of ZoomRatio[1, %d]!!\n", layer_id,
                   in_rect->width, in_rect->height, out_rect->width, out_rect->height, GFBG_MAX_ZOOMIN);
        return TD_FAILURE;
    }

    need_zoom = (out_rect->width != in_rect->width || out_rect->height != in_rect->height);
    if (need_zoom == TD_FALSE) {
        return TD_FAILURE;
    }

    if (need_zoom && (in_rect->width > GFBG_LINE_BUF)) {
        gfbg_error("GFBG layer%d in width: %u is bigger than %d, will not zoom in!!\n", layer_id, in_rect->width,
                   GFBG_LINE_BUF);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void gfbg_dcmp_config(td_u32 layer_id)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)(info->par);
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    td_bool dcmp_state_en = TD_FALSE;

    /*
     * In order to support compressed and non-compressed dynamic switching, you cannot use
     * pstCompressInfo->bCompressOpen to determine whether to open compression.
     * To use the compression information that follows the frame, and whether or not
     * to flip to decide whether to switch compression or non-compression
     */
    if (refresh_info->disp_buf_info.compress) {
        /* See GFBG_DRV_EnableDcmp */
        g_drv_ops.gfbg_drv_enable_dcmp(layer_id, TD_TRUE);
        /* See GFBG_DRV_GetDcmpEnableState */
        g_drv_ops.gfbg_drv_get_dcmp_enable_state(layer_id, &dcmp_state_en);
        if (dcmp_state_en) {
            gfbg_set_dcmp_info(info);
#ifdef CONFIG_COMPRESS_ECONOMIZE_MEMERY
            /* after recalculate stride, set buf[1] again */
            gfbg_set_dispbufinfo(layer_id);
#endif
        }
    } else {
        /*
         * Set the address and stride to ensure that the compression will g_display correctly when switched to
         * non-compressed
         */
        g_drv_ops.gfbg_drv_set_layer_addr(layer_id, refresh_info->screen_addr);
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, refresh_info->user_buffer.canvas.pitch);
        } else {
            /* dynamic change compress,when compress to uncompress, reset stride */
            if (par->param_modify_mask & GFBG_LAYER_PARAMODIEY_COMPRESS) {
                info->fix.line_length = (info->var.xres_virtual * info->var.bits_per_pixel / 8 + /* 8 one byte */
                    GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
                par->param_modify_mask &= ~GFBG_LAYER_PARAMODIEY_COMPRESS;
            }
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, gfbg_get_line_length(info));
        }

        g_drv_ops.gfbg_drv_enable_dcmp(layer_id, TD_FALSE);
    }
}

/* Callback function for VO vertical timing interrupt */
static td_s32 callback_get_osd_data(gfbg_par *par, td_u32 layer_id)
{
    gfbg_osd_data layer_data = {0};
    gfbg_display_info *display_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;

    display_info = &par->display_info;
    refresh_info = &par->refresh_info;

    if (g_drv_ops.gfbg_drv_get_osd_data(par->layer_id, &layer_data) != TD_SUCCESS) {
        gfbg_error("failed to get layer%d's osd data!\n", par->layer_id);
        return TD_FAILURE;
    }
    display_info->max_screen_width = layer_data.screen_width;
    display_info->max_screen_height = layer_data.screen_height;
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_ALPHA) {
        g_drv_ops.gfbg_drv_set_layer_alpha(layer_id, par->alpha);
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_ALPHA;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_COLORKEY) {
        g_drv_ops.gfbg_drv_set_layer_key_mask(layer_id, &par->ckey);
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_COLORKEY;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_FMT) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, refresh_info->user_buffer.canvas.format);
        } else {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, par->color_format);
        }
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_FMT;
    }
    return TD_SUCCESS;
}

static td_void callback_modify_dynamic_range(gfbg_par *par, td_u32 layer_id)
{
    gfbg_display_info *display_info = TD_NULL;
    gfbg_refresh_info *refresh_info = TD_NULL;
    struct fb_info *info = TD_NULL;

    display_info = &par->display_info;
    refresh_info = &par->refresh_info;
    info = g_layer[layer_id].info;

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_BMUL) {
        if (g_drv_ops.gfbg_drv_set_pre_mul(layer_id, display_info->is_premul) != TD_SUCCESS) {
            if (par->display_info.is_premul == TD_TRUE) {
                par->display_info.is_premul = TD_FALSE;
            } else {
                par->display_info.is_premul = TD_TRUE;
            }
        }
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_BMUL;
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_STRIDE) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, refresh_info->user_buffer.canvas.pitch);
        } else {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, gfbg_get_line_length(info));
        }
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_STRIDE;
    }
    return;
}

static td_void callback_init_rect(ot_fb_rect *in_rect, ot_fb_rect *out_rect, const gfbg_display_info *display_info)
{
    in_rect->x = display_info->pos.x_pos;
    in_rect->y = display_info->pos.y_pos;
    in_rect->width = (td_s32)display_info->display_width;
    in_rect->height = (td_s32)display_info->display_height;

    out_rect->x = display_info->pos.x_pos;
    out_rect->y = display_info->pos.y_pos;
    out_rect->width = display_info->screen_width;
    out_rect->height = display_info->screen_height;

    if (out_rect->x + out_rect->width > (td_s32)display_info->max_screen_width) {
        out_rect->width = (td_s32)(display_info->max_screen_width - out_rect->x);
    }

    if (out_rect->y + out_rect->height > (td_s32)display_info->max_screen_height) {
        out_rect->height = (td_s32)(display_info->max_screen_height - out_rect->y);
    }

    /* after cut off, the input rectangle keep rate with output rectangle */
    /* Prevent the occurrence of the divide-by-zero error */
    if ((display_info->screen_width != 0) && (display_info->screen_height != 0)) {
        in_rect->width = in_rect->width * out_rect->width / (td_s32)display_info->screen_width;
        in_rect->height = in_rect->height * out_rect->height / (td_s32)display_info->screen_height;
    }
    return;
}

static td_void callback_modify_sizes(gfbg_par *par, td_u32 layer_id)
{
    gfbg_display_info *display_info = TD_NULL;
    ot_fb_rect in_rect = {0};
    ot_fb_rect out_rect = {0};
    td_bool zme_en;

    display_info = &par->display_info;
    /* Handles requests to modify input and output sizes. */
    if ((par->param_modify_mask & GFBG_LAYER_PARAMODIFY_INRECT) ||
        (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_OUTRECT)) {
        /* if VO can't support scaling,  we set the screen's H/W as the same as display */
        if (!g_drv_ops.capability[layer_id].is_vo_scale) {
            display_info->screen_width = display_info->display_width;
            display_info->screen_height = display_info->display_height;
        }

        /* for init rect */
        callback_init_rect(&in_rect, &out_rect, display_info);
        if (gfbg_check_imagezoomenable(layer_id, &in_rect, &out_rect) == TD_SUCCESS) {
            /*
             * If you want to go through the zoom module, you need to correct it to 2 alignment,
             * otherwise it will appear abnormal.
             */
            in_rect.width = ALIGN_DOWN((td_u32)in_rect.width, 2); /* 2 align */
            in_rect.height = ALIGN_DOWN((td_u32)in_rect.height, 2); /* 2 align */

            if (g_drv_ops.gfbg_drv_set_layer_rect) {
                g_drv_ops.gfbg_drv_set_layer_rect(layer_id, &in_rect, &out_rect);
            }

            if (g_drv_ops.gfbg_drv_set_layer_src_image_reso) {
                g_drv_ops.gfbg_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }

            /* NEW!!!   Set zoom */
            /* See out width and height not equal to in width and height,then enable zme. */
            zme_en = (out_rect.width != in_rect.width || out_rect.height != in_rect.height);
            if (g_drv_ops.gfbg_drv_enable_zme) {
                g_drv_ops.gfbg_drv_enable_zme(layer_id, &in_rect, &out_rect, zme_en);
            }
        } else {
            /*
             * If scaling is not enabled,
             * the input size is used as the output size and the zoom module is closed.
             */
            if (g_drv_ops.gfbg_drv_set_layer_rect) {
                g_drv_ops.gfbg_drv_set_layer_rect(layer_id, &in_rect, &in_rect);
            }

            if (g_drv_ops.gfbg_drv_set_layer_src_image_reso) {
                g_drv_ops.gfbg_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }

            if (g_drv_ops.gfbg_drv_enable_zme) {
                g_drv_ops.gfbg_drv_enable_zme(layer_id, &in_rect, &out_rect, TD_FALSE);
            }
        }

        /* Processing completed, clear mask */
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_INRECT;
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_OUTRECT;
    }
    return;
}

static td_void callback_set_fmt_and_stride(const gfbg_par *par, td_u32 layer_id)
{
    struct fb_info *info = TD_NULL;
    const gfbg_refresh_info *refresh_info = TD_NULL;

    info = g_layer[layer_id].info;
    refresh_info = &par->refresh_info;
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_FMT) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, refresh_info->user_buffer.canvas.format);
        } else {
            g_drv_ops.gfbg_drv_set_layer_data_fmt(layer_id, par->color_format);
        }
    }

    if (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_STRIDE) {
        if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE) && refresh_info->user_buffer.canvas.phys_addr) {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, refresh_info->user_buffer.canvas.pitch);
        } else {
            g_drv_ops.gfbg_drv_set_layer_stride(layer_id, gfbg_get_line_length(info));
        }
    }
    return;
}

static td_void callback_update_refresh_info(gfbg_par *par, td_u32 layer_id)
{
    gfbg_refresh_info *refresh_info = TD_NULL;
    struct fb_info *info = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    td_u32 index, buf_size;

    refresh_info = &par->refresh_info;
    index = refresh_info->disp_buf_info.index_for_int;
    info = g_layer[layer_id].info;
    buf_size = ((gfbg_get_line_length(info) * gfbg_get_yres(info)) + GFBG_ALIGNMENT) & (~GFBG_ALIGNMENT);
    compress_info = &par->compress_info;
    if ((refresh_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE) && (refresh_info->disp_buf_info.need_flip == TD_TRUE)) {
        /* Work buf to change to free buf. Take free buf to display */
        index = 1 - index;
        refresh_info->disp_buf_info.index_for_int = index;
        /*
         * The display address is set to the address of the free buf,
         * which is set to the screen buf address differently from 0buf
         */
        g_drv_ops.gfbg_drv_set_layer_addr(layer_id, refresh_info->disp_buf_info.phys_addr[index]);
        refresh_info->screen_addr = refresh_info->disp_buf_info.phys_addr[index];
        refresh_info->gb_screen_addr = refresh_info->screen_addr + buf_size / 2; /* 2 alg data */
        compress_info->layer_addr_update = TD_TRUE;
#ifdef __LITEOS__
        if (info->oinfo.stride != 0) {
            info->oinfo.sarea.y = (par->refresh_info.disp_buf_info.phys_addr[index] -
                                   (td_u32)(uintptr_t)info->oinfo.fbmem) / info->oinfo.stride;
            if ((info->oinfo.bpp >> 3) != 0) { /* 3 is 8 bits */
                info->oinfo.sarea.x = ((par->refresh_info.disp_buf_info.phys_addr[index] -
                    (td_u32)(uintptr_t)info->oinfo.fbmem) % info->oinfo.stride) /
					(info->oinfo.bpp >> 3); /* 3 is 8 bits */
            }
        }
#else
        info->var.yoffset = osal_div_u64((refresh_info->disp_buf_info.phys_addr[index] - gfbg_get_smem_start(info)),
                                         gfbg_get_line_length_ex(info));
        if ((gfbg_get_line_length(info) != 0) && ((gfbg_get_bits_per_pixel(info)>> 3) != 0)) { /* 3 is 8 bits */
            info->var.xoffset = ((td_ulong)(refresh_info->disp_buf_info.phys_addr[index] - gfbg_get_smem_start(info)) %
                                 gfbg_get_line_length_ex(info)) /
				(gfbg_get_bits_per_pixel(info)>> 3); /* 3 is 8 bits */
        }
#endif
        refresh_info->disp_buf_info.fliped = TD_TRUE;
        refresh_info->disp_buf_info.need_flip = TD_FALSE;
        refresh_info->disp_buf_info.int_pic_num++;
    }
    return;
}

static td_void callback_modify_address(gfbg_par *par, td_u32 layer_id)
{
    gfbg_refresh_info *refresh_info = TD_NULL;
    gfbg_display_info *display_info = TD_NULL;
    volatile gfbg_compress_info *compress_info = TD_NULL;
    struct fb_info *info = TD_NULL;
    ot_fb_rect in_rect = {0};
    ot_fb_rect out_rect = {0};

    refresh_info = &par->refresh_info;
    display_info = &par->display_info;
    compress_info = &par->compress_info;
    info = g_layer[layer_id].info;
    /* The display address is refreshed and the display address is modified. */
    if (!(par->param_modify_mask & GFBG_LAYER_PARAMODIFY_DISPLAYADDR)) {
        /* according to the index, decide which buf set to the screen */
        callback_update_refresh_info(par, layer_id);
        return;
    }
        /* SetLayerDataFmt and  SetLayerStride */
        callback_set_fmt_and_stride(par, layer_id);

    if ((par->param_modify_mask & GFBG_LAYER_PARAMODIFY_INRECT) ||
        (par->param_modify_mask & GFBG_LAYER_PARAMODIFY_OUTRECT)) {
            /* if VO can't support scaling,  we set the screen's H/W as the same as display */
           if (!g_drv_ops.capability[layer_id].is_vo_scale) {
           display_info->screen_width = display_info->display_width;
           display_info->screen_height = display_info->display_height;
        }

        /* for init rect */
        callback_init_rect(&in_rect, &out_rect, display_info);

        if (gfbg_check_imagezoomenable(layer_id, &in_rect, &out_rect) == TD_SUCCESS) {
            g_drv_ops.gfbg_drv_set_layer_rect(layer_id, &in_rect, &out_rect);
            if (g_drv_ops.gfbg_drv_set_layer_src_image_reso) {
                g_drv_ops.gfbg_drv_set_layer_src_image_reso(layer_id, &in_rect);
            }
        }
    }

    g_drv_ops.gfbg_drv_set_layer_addr(layer_id, refresh_info->screen_addr);

    par->param_modify_mask &= ~GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    compress_info->layer_addr_update = TD_TRUE;

    if ((refresh_info->disp_buf_info.phys_addr[0] != refresh_info->disp_buf_info.phys_addr[1]) &&
        (refresh_info->disp_buf_info.phys_addr[0])) {
        if (refresh_info->screen_addr >=  refresh_info->disp_buf_info.phys_addr[0] &&
            refresh_info->screen_addr < refresh_info->disp_buf_info.phys_addr[1]) {
            refresh_info->disp_buf_info.index_for_int = 0;
        } else if ((refresh_info->screen_addr >= refresh_info->disp_buf_info.phys_addr[1]) &&
                   (refresh_info->screen_addr < (refresh_info->disp_buf_info.phys_addr[0] + gfbg_get_smem_len(info)))) {
            refresh_info->disp_buf_info.index_for_int = 1;
        }
    }
    /* according to the index, decide which buf set to the screen */
    callback_update_refresh_info(par, layer_id);
    return;
}

static td_void callback_modify_smart_rect(const gfbg_par *par, td_u32 layer_id)
{
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIEY_SMART_RECT) {
        if (!g_drv_ops.capability[layer_id].is_osb) {
               return;
        }
        g_drv_ops.gfbg_drv_smart_rect_up_param(par->layer_id);
    }
    return;
}

static td_void callback_modify_clut_up(gfbg_par *par, td_u32 layer_id)
{
    if (par->param_modify_mask & GFBG_LAYER_PARAMODIEY_CLUT_UP) {
        if (!g_drv_ops.capability[layer_id].is_cmap) {
               return;
        }
        g_drv_ops.gfbg_drv_set_color_reg_up(par->layer_id);
        par->param_modify_mask &= ~GFBG_LAYER_PARAMODIEY_CLUT_UP;
    }
    return;
}

static td_s32 vo_callback_process(gfbg_par *par, td_u32 layer_id, td_bool *is_continue)
{
    unsigned long lock_flag;
    gfbg_sync_attr sync_attr;
    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    /* If not displayed, close the graphics layer and exit */
    if (par->show == TD_FALSE) {
        if (g_drv_ops.gfbg_drv_set_layer_enable) {
            g_drv_ops.gfbg_drv_set_layer_enable(layer_id, TD_FALSE);
        }
        if (g_drv_ops.gfbg_drv_updata_layer_reg) {
            g_drv_ops.gfbg_drv_updata_layer_reg(layer_id);
        }
        *is_continue = TD_FALSE;
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_SUCCESS;
    }

    /* Non-modified status, modified, can be modified */
    if (!par->modifying) {
        /*
         * 1.Get osd data
         * 2.Set layer alpha
         * 3.Set layer keymask
         * 4.Set layer data fmt
         */
        if (callback_get_osd_data(par, layer_id) != TD_SUCCESS) {
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            return TD_FAILURE;
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

        /* set smart rect */
        callback_modify_smart_rect(par, layer_id);

        /* set clut up */
        callback_modify_clut_up(par, layer_id);

        /* gfbg_drv_set_layer_enable */
        g_drv_ops.gfbg_drv_set_layer_enable(layer_id, par->show);
    }
    /* Decompression configuration */
    gfbg_dcmp_config(layer_id);

    /* support low delay (only G0 support low delay) */
    sync_attr.is_sync = TD_TRUE;
    sync_attr.safe_dist = par->display_info.display_width * 3 * /* 3 rgb888 calculate */
                          par->display_info.display_height / 2; /* 2 half frame */
    if (g_drv_ops.gfbg_drv_set_tde_sync != TD_NULL) {
        g_drv_ops.gfbg_drv_set_tde_sync(layer_id, &sync_attr);
    }
    /* gfbg_drv_updata_layer_reg */
    g_drv_ops.gfbg_drv_updata_layer_reg(layer_id);
    *is_continue = TD_TRUE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

/* Callback function for VO vertical timing interrupt */
static td_s32 gfbg_interrupt_process(td_u32 layer_id)
{
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    td_bool is_continue = TD_FALSE;

    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_FAILURE;
    }
    info = g_layer[layer_id].info;
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    if (info->par == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)(info->par);
    if (vo_callback_process(par, layer_id, &is_continue) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (is_continue != TD_TRUE) {
        return TD_SUCCESS;
    }

    /* Field blanking mark */
    par->vblflag = 1;
    wake_up(&(par->vbl_event));
    return TD_SUCCESS;
}

static td_s32 refresh_0buf_process(gfbg_par *par, const ot_fb_buf *canvas_buf)
{
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    unsigned long lock_flag;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    if (compress_info->compress_open == TD_TRUE) {
        gfbg_error("only FB_LAYER_BUF_DOUBLE or FB_LAYER_BUF_DOUBLE_IMMEDIATE support compress!\n");
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
        return TD_FAILURE;
    }

    refresh_info->disp_buf_info.compress = TD_FALSE;

    par->modifying = TD_TRUE;
    /* modify by wxl : if change flush type between 2buffer and 0 buffer, the addr couldn't be changed */
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_DISPLAYADDR;
    /*
     * The graphic address is taken from the canvas of the user data and
     * filled in the screen address of the refresh information.
     */
    refresh_info->screen_addr = canvas_buf->canvas.phys_addr;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_STRIDE;
    refresh_info->user_buffer.canvas.pitch = canvas_buf->canvas.pitch;

    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_FMT;
    refresh_info->user_buffer.canvas.format = canvas_buf->canvas.format;

    /* NEW feature: zme */
    par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_OUTRECT;

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 gfbg_refresh_0buf(td_u32 layer_id, const ot_fb_buf *canvas_buf)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    unsigned long lock_flag;
    td_s32 ret;

    ret = refresh_0buf_process(par, canvas_buf);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * In gfbg_disp_setdispsize, it is possible that kmalloc
     * allocates memory in a non-atomic manner, so the lock must be released first
     */
    if (gfbg_disp_setdispsize(layer_id, canvas_buf->canvas.width, canvas_buf->canvas.height) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_FALSE;
    ret = memcpy_s(&(refresh_info->user_buffer), sizeof(ot_fb_buf), canvas_buf, sizeof(ot_fb_buf));
    gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
    if (par->compress_info.compress_open) {
        ret = memcpy_s((td_void *)&par->compress_info.update_rect, sizeof(ot_fb_rect), &canvas_buf->update_rect,
            sizeof(ot_fb_rect));
        gfbg_unlock_unequal_eok_return(ret, &par->lock, &lock_flag);
        par->compress_info.update_finished = TD_TRUE;
    }
    par->vblflag = 0;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    /* if the flag "FB_ACTIVATE_VBL" has been set, we should wait for register update finish */
#ifdef __LITEOS__
    if (!in_atomic()) {
#else
    if (!in_atomic() && (info->var.activate & FB_ACTIVATE_VBL)) {
#endif
        gfbg_wait_regconfig_work(layer_id);
    }

    return TD_SUCCESS;
}

static td_void refresh_is_cursor_overlay(const gfbg_par *par, const ot_fb_buf *canvas_buf, td_bool *is_overlay)
{
    gfbg_par *cursor_par = TD_NULL;
    const gfbg_cursor_info *cursor_info = &par->cursor_info;
    ot_fb_rect rc_cursor;
    cursor_par = (gfbg_par *)g_layer[par->cursor_info.attached_cursor_id].info->par;
    if (cursor_info->attached != 0) {
        rc_cursor.x = cursor_par->display_info.pos.x_pos - cursor_par->cursor_info.cursor.hot_pos.x_pos;
        rc_cursor.y = cursor_par->display_info.pos.y_pos - cursor_par->cursor_info.cursor.hot_pos.y_pos;
        rc_cursor.width = cursor_par->cursor_info.cursor.cursor.width;
        rc_cursor.height = cursor_par->cursor_info.cursor.cursor.height;

        /* check the cusor overlay with refresh area */
        if (cursor_par->show && (((rc_cursor.x >= canvas_buf->update_rect.x &&
            rc_cursor.x <=  canvas_buf->update_rect.x + canvas_buf->update_rect.width)) ||
            (rc_cursor.x < canvas_buf->update_rect.x && rc_cursor.x + rc_cursor.width >= canvas_buf->update_rect.x))) {
            if (((rc_cursor.y >= canvas_buf->update_rect.y &&
                rc_cursor.y <= canvas_buf->update_rect.y + canvas_buf->update_rect.height)) ||
                (rc_cursor.y < canvas_buf->update_rect.y &&
                rc_cursor.y + rc_cursor.height >=  canvas_buf->update_rect.y)) {
                *is_overlay = TD_TRUE;
            }
        }
    }
    return;
}

/*
 * Name : gfbg_refresh
 * Desc : It is refreshed according to the canvas information and the layer's buf refresh mode.
 *        It is called indirectly when setting the layer attr.
 * See  : references gfbg_refresh_again,gfbg_onrefresh
 *        calls gfbg_refresh_0buf,gfbg_refresh_1buf,gfbg_refresh_2buf
 */
static td_s32 gfbg_refresh(td_u32 layer_id, const ot_fb_buf *canvas_buf, ot_fb_layer_buf buf_mode)
{
    td_s32 ret = TD_FAILURE;
    gfbg_par *par = (gfbg_par *)g_layer[layer_id].info->par;
    td_bool is_overlay = TD_FALSE; /* is the cusor overlay with refresh area */

    if (canvas_buf == TD_NULL) {
        return TD_FAILURE;
    }
    /*
     * For cursor layer
     * you can quary whether cursor attach to a certain layer for general layer
     * when attached not zero, we should check whether the cusor overlay with refresh area or not
     */
    refresh_is_cursor_overlay(par, canvas_buf, &is_overlay);

    switch (buf_mode) {
        case OT_FB_LAYER_BUF_DOUBLE:
            ret = gfbg_refresh_2buf(layer_id, canvas_buf);
            break;
        case OT_FB_LAYER_BUF_ONE:
            ret = gfbg_refresh_1buf(layer_id, canvas_buf);
            break;
        case OT_FB_LAYER_BUF_NONE:
            ret = gfbg_refresh_0buf(layer_id, canvas_buf);
            break;
        case OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE:
            ret = gfbg_refresh_2buf_immediate_display(layer_id, canvas_buf);
            break;
        default:
            break;
    }

    return ret;
}

#ifdef __LITEOS__
static struct gfbg_info *gfbg_alloc(size_t size)
{
    td_s32 gfbg_info_len = sizeof(struct gfbg_info);
    struct gfbg_info *info = TD_NULL;

    gfbg_info_len = ALIGN(gfbg_info_len, sizeof(unsigned long));

    info = (struct gfbg_info *)malloc(gfbg_info_len + size);
    if (info == NULL) {
        return NULL;
    }
    (td_void)memset_s(info, (gfbg_info_len + size), 0, (gfbg_info_len + size));

    info->par = (void *)((char *)info + gfbg_info_len);

    return info;
}

static td_s32 alloc_new_canvas_buffer(const struct fb_info *info, const ot_fb_layer_info *layer_info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_surface *canvas_surface = &par->canvas_sur;
    td_u32 layer_size;
    td_u32 pitch;
    td_char *buf = TD_NULL;
    td_char name[16]; /* 16 name max length */
    struct fb_info *info_temp = TD_NULL;
    /*
     * Applying fb_info without private data is not for registration,
     * but for the convenience of passing to gfbg_fill_data to empty the memory.
     */
    info_temp = gfbg_alloc(0);
    if (info_temp == TD_NULL) {
        return TD_FAILURE;
    }

    /* 16 bytes alignment */
    pitch = (((layer_info->canvas_width * gfbg_get_bits_per_pixel(info)) >> 3) + 15) >> 4; /* 3 15 4 for align */
    pitch = pitch << 4; /* 4 for align */

    layer_size = pitch * layer_info->canvas_height;
    /* alloc new buffer */
    if (snprintf_s(name, sizeof(name), 13, "gfbg_canvas%01u", par->layer_id) < 0) { /* 13:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        free(info_temp);
        return TD_FAILURE;
    }
    canvas_surface->phys_addr = gfbg_buf_allocmem(name, strlen(name), layer_size,
        gfbg_get_layer_mmz_names(par->layer_id));
    if (canvas_surface->phys_addr == 0) {
        gfbg_error("alloc canvas buffer no mem");
        free(info_temp);
        return TD_FAILURE;
    }

    buf = (td_char *)gfbg_buf_map(canvas_surface->phys_addr, layer_size);
    if (buf == TD_NULL) {
        gfbg_error("map canvas buffer failed!\n");
        gfbg_buf_freemem(canvas_surface->phys_addr);
        free(info_temp);
        return TD_FAILURE;
    }
    /* gfbg_fill_data is faster than memset_s */
    info_temp->oinfo.fbmem = (uintptr_t)canvas_surface->phys_addr;
    (td_void)memset_s(info_temp->oinfo.fbmem, layer_size, 0x0, layer_size);

    gfbg_buf_ummap(buf);

    gfbg_info("alloc new memory for canvas buffer success\n");
    canvas_surface->width = layer_info->canvas_width;
    canvas_surface->height = layer_info->canvas_height;
    canvas_surface->pitch = pitch;
    par->canvas_sur.format =  info->vinfo.format;
    free(info_temp);

    return TD_SUCCESS;
}
#else
static td_s32 alloc_new_canvas_buffer(const struct fb_info *info, const ot_fb_layer_info *layer_info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    ot_fb_surface *canvas_surface = &par->canvas_sur;
    td_u32 layer_size;
    td_u32 pitch;
    td_char *buf = TD_NULL;
    td_char name[16]; /* 16 name max length */
    struct fb_info *info_temp = TD_NULL;
    /*
     * Applying fb_info without private data is not for registration,
     * but for the convenience of passing to gfbg_fill_data to empty the memory.
     */
    info_temp = framebuffer_alloc(0, TD_NULL);
    if (info_temp == TD_NULL) {
        return TD_FAILURE;
    }

    /* 16 bytes alignment */
    pitch = (((layer_info->canvas_width * gfbg_get_bits_per_pixel(info)) >> 3) + 15) >> 4; /* 3 4 15 alg data */
    pitch = pitch << 4; /* 4 alg data */

    layer_size = pitch * layer_info->canvas_height;
    /* alloc new buffer */
    if (snprintf_s(name, sizeof(name), 13, "gfbg_canvas%01u", par->layer_id) < 0) { /* 13:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        framebuffer_release(info_temp);
        return TD_FAILURE;
    }
    canvas_surface->phys_addr = gfbg_buf_allocmem(name, strlen(name), layer_size,
        gfbg_get_layer_mmz_names(par->layer_id));
    if (canvas_surface->phys_addr == 0) {
        gfbg_error("alloc canvas buffer no mem");
        framebuffer_release(info_temp);
        return TD_FAILURE;
    }

    buf = (td_char *)gfbg_buf_map(canvas_surface->phys_addr, layer_size);
    if (buf == TD_NULL) {
        gfbg_error("map canvas buffer failed!\n");
        gfbg_buf_freemem(canvas_surface->phys_addr);
        framebuffer_release(info_temp);
        return TD_FAILURE;
    }
    /* gfbg_fill_data is faster than memset_s */
    info_temp->fix.smem_start = (td_ulong)canvas_surface->phys_addr;
    info_temp->screen_base = buf;
    if (gfbg_fill_data(info_temp, 0, layer_size) != TD_SUCCESS) {
        gfbg_error("gfbg_fill_data failed!\n");
    }
    gfbg_buf_ummap(buf);

    gfbg_info("alloc new memory for canvas buffer success\n");
    canvas_surface->width = layer_info->canvas_width;
    canvas_surface->height = layer_info->canvas_height;
    canvas_surface->pitch = pitch;
    canvas_surface->format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp,
                                               info->var.bits_per_pixel);
    framebuffer_release(info_temp);
    return TD_SUCCESS;
}
#endif

static td_s32 gfbg_alloccanbuf(const struct fb_info *info, const ot_fb_layer_info *layer_info)
{
    gfbg_par *par = TD_NULL;
    ot_fb_surface *canvas_surface = TD_NULL;

    if ((info == TD_NULL) || (layer_info == TD_NULL)) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    canvas_surface = &par->canvas_sur;

    if (!(layer_info->mask & OT_FB_LAYER_MASK_CANVAS_SIZE)) {
        return TD_SUCCESS;
    }

    /* if  with old canvas buffer */
    if (canvas_surface->phys_addr != 0) {
        /* if old is the same with new , then return, else free the old buffer */
        if ((layer_info->canvas_width == canvas_surface->width) &&
            (layer_info->canvas_height == canvas_surface->height)) {
            gfbg_info("mem size is the same , no need alloc new memory");
            return TD_SUCCESS;
        }

        /* free new old buffer */
        gfbg_info("free old canvas buffer\n");
        gfbg_freeccanbuf(par);
    }
    if (layer_info->canvas_width > GFBG_4K_DEF_WIDTH || layer_info->canvas_height > GFBG_4K_DEF_HEIGHT) {
        gfbg_info("unsupported too large w(%d) and h(%d)!max width (%d), max height (%d)\n",
            layer_info->canvas_width, layer_info->canvas_height, GFBG_4K_DEF_WIDTH, GFBG_4K_DEF_HEIGHT);
        return TD_FAILURE;
    }
    /* new canvas buffer */
    if (alloc_new_canvas_buffer(info, layer_info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 onrefresh_check_param(const gfbg_par* par, const ot_fb_buf *canvas_buf, ot_fb_layer_buf buf_mode)
{
    ot_unused(par);
    if (canvas_buf->canvas.format >= OT_FB_FORMAT_BUTT) {
        return TD_FAILURE;
    }

    if (buf_mode == OT_FB_LAYER_BUF_BUTT) {
        gfbg_error("doesn't support FBIO_REFRESH operation when refresh mode is FB_LAYER_BUF_BUTT!\n");
        return TD_FAILURE;
    }

    if ((canvas_buf->update_rect.x >=  (td_s32)canvas_buf->canvas.width) ||
        (canvas_buf->update_rect.y >= (td_s32)canvas_buf->canvas.height) ||
        (canvas_buf->update_rect.width <= 0) || (canvas_buf->update_rect.height <= 0) ||
        (canvas_buf->canvas.width == 0) || (canvas_buf->canvas.height == 0)) {
        gfbg_error("rect error: update rect:(%d,%d,%d,%d), canvas range:(%d,%d)\n",
                   canvas_buf->update_rect.x, canvas_buf->update_rect.y,
                   canvas_buf->update_rect.width, canvas_buf->update_rect.height,
                   canvas_buf->canvas.width, canvas_buf->canvas.height);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 onrefresh_get_canvas_buf(gfbg_par *par, td_void __user *argp, ot_fb_buf *canvas_buf)
{
    unsigned long lock_flag;
    ot_fb_layer_buf buf_mode;
    if (osal_copy_from_user(canvas_buf, argp, sizeof(ot_fb_buf))) {
        return -EFAULT;
    }

    gfbg_get_bufmode(par, &buf_mode);
    /*
     * check canvas format
     * check canvas phyaddr
     * check buf_mode
     * check canvas update rect legality
     */
    if (onrefresh_check_param(par, canvas_buf, buf_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* update canvas update_rect */
    if (canvas_buf->update_rect.x + canvas_buf->update_rect.width > (td_s32)canvas_buf->canvas.width) {
        canvas_buf->update_rect.width = canvas_buf->canvas.width - canvas_buf->update_rect.x;
    }

    if (canvas_buf->update_rect.y + canvas_buf->update_rect.height > (td_s32)canvas_buf->canvas.height) {
        canvas_buf->update_rect.height =  canvas_buf->canvas.height - canvas_buf->update_rect.y;
    }

    /* Check the legality of physical address and size */
    /* CMPI_CheckMmzphy_addr Call a semaphore, Cannot be used in the interrupt */
    if (!osal_in_interrupt()) {
        if (gfbg_check_phyaddr(canvas_buf) != TD_SUCCESS) {
            gfbg_error("mmz phy addr 0x%lx invalid.\n", (td_ulong)canvas_buf->canvas.phys_addr);
            return TD_FAILURE;
        }
    }

    if (buf_mode == OT_FB_LAYER_BUF_NONE) {
        /* Check if the format of the canvas supported or not by gfbg */
        if ((par->layer_id >= GFBG_MAX_LAYER_NUM) || (canvas_buf->canvas.format >= OT_FB_FORMAT_BUTT)) {
            return TD_FAILURE;
        }
        if (!g_drv_ops.capability[par->layer_id].is_color_format[canvas_buf->canvas.format]) {
            gfbg_error("Unsupported PIXEL FORMAT!\n");
            return -EINVAL;
        }
        /*
         * there's a limit from hardware that the start address of screen buf
         * should be 16byte aligned.
         */
        osal_spin_lock_irqsave(&par->lock, &lock_flag);
        if ((canvas_buf->canvas.phys_addr & 0xf) || (canvas_buf->canvas.pitch & 0xf)) {
            gfbg_error("addr 0x%lx or pitch: 0x%x is not 16 bytes align !\n", (td_ulong)canvas_buf->canvas.phys_addr,
                       canvas_buf->canvas.pitch);
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            return TD_FAILURE;
        }
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    } else {
        /*
         * Check if the format of the canvas and the format of
         * GFBG belong to the category that TDE can support
         */
        if ((!gfbg_tde_is_support_fmt(canvas_buf->canvas.format)) || (!gfbg_tde_is_support_fmt(par->color_format))) {
            gfbg_error("Unsupported PIXEL FORMAT!CanvasFmt:%d,par->color_format:%d\n", canvas_buf->canvas.format,
                       par->color_format);
            return -EINVAL;
        }
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_onrefresh(gfbg_par* par, td_void __user *argp)
{
    td_s32 ret;
    ot_fb_buf canvas_buf;
    ot_fb_layer_buf buf_mode;
    gfbg_display_info *diplay_info = &par->display_info;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_warning("you shouldn't refresh cursor layer!\n");
        return TD_SUCCESS;
    }

    if (argp == TD_NULL) {
        gfbg_error("NULL arg!\n");
        return -EINVAL;
    }

    /* get canvas buffer and check legality */
    ret = onrefresh_get_canvas_buf(par, argp, &canvas_buf);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    gfbg_get_bufmode(par, &buf_mode);
    if ((buf_mode == OT_FB_LAYER_BUF_NONE) && (diplay_info->mirror_mode != OT_FB_MIRROR_NONE)) {
        gfbg_error("Can't do mirror when the layer buf is none!\n");
        return -EINVAL;
    }

    if ((buf_mode == OT_FB_LAYER_BUF_NONE) && (diplay_info->rotate_mode != OT_FB_ROTATE_NONE)) {
        gfbg_error("Can't do rotate when the layer buf is none!\n");
        return -EINVAL;
    }
    ret = gfbg_refresh(par->layer_id, &canvas_buf, buf_mode);

    return ret;
}

static td_s32 gfbg_set_mirrormode(gfbg_par *par, ot_fb_mirror_mode mirror_mode)
{
    gfbg_display_info *display_info = &par->display_info;
    volatile gfbg_compress_info *compress_info = &par->compress_info;
    ot_fb_layer_buf buf_mode;

    gfbg_get_bufmode(par, &buf_mode);

    if ((buf_mode == OT_FB_LAYER_BUF_BUTT) || (buf_mode == OT_FB_LAYER_BUF_NONE)) {
        gfbg_error("doesn't support FBIOPUT_MIRROR_MODE operation when in standard mode or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }

    if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
        (par->color_format != OT_FB_FORMAT_ARGB8888)) {
        gfbg_error("Mirror doesn't support this format:%d.\n!\n", par->color_format);
        return -EINVAL;
    }

    if (mirror_mode >= OT_FB_MIRROR_BUTT) {
        gfbg_error("The input mirror mode is wrong!\n");
        return -EINVAL;
    }

    if ((mirror_mode != OT_FB_MIRROR_NONE) && (display_info->rotate_mode != OT_FB_ROTATE_NONE)) {
        gfbg_error("Can't do mirror when rotate!\n");
        return -EINVAL;
    }

    if ((mirror_mode != OT_FB_MIRROR_NONE) && (compress_info->compress_open == TD_TRUE)) {
        gfbg_error("Can't do mirror when compression is on!\n");
        return -EINVAL;
    }

    display_info->mirror_mode = mirror_mode;
    return TD_SUCCESS;
}

static td_s32 rotate_vb_init(const struct fb_info *info)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    td_u32 size;
    td_char name[GFBG_ROTBUF_NAME_LEN];
    td_u64 *vir_rotate_vb = TD_NULL;
    ot_mpp_chn chn;
    mm_malloc_param malloc_param = {0};

    chn.mod_id  = OT_ID_FB;
    chn.dev_id = gfbg_vou_get_dev_id(par->layer_id);
    chn.chn_id = 0;

    size = gfbg_get_line_length(info) * (((display_info->display_height + 15) / 16) * 16); /* 15 16 align */

    if (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_mmz_name(&chn, (td_void **)&malloc_param.mmz_name)) {
        gfbg_error("get mmz name fail!\n");
        return ENOMEM;
    }

    if (snprintf_s(name, sizeof(name), 13, "gfbg%01d_rotate", par->layer_id) < 0) { /* 13:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    malloc_param.buf_name = name;
    malloc_param.size = size;
    malloc_param.kernel_only = TD_TRUE;
    par->rotate_vb = cmpi_mmz_malloc(&malloc_param);
    if (par->rotate_vb == 0) {
        gfbg_error("get buffer fail,size %d !\n", size);
        return ENOMEM;
    }

    vir_rotate_vb = cmpi_remap_nocache(par->rotate_vb, size);
    if (vir_rotate_vb == TD_NULL) {
        gfbg_error("get buffer fail,size %d !\n", size);
        cmpi_mmz_free(par->rotate_vb, TD_NULL);
        return ENOMEM;
    }

    (td_void)memset_s(vir_rotate_vb, size, 0x0, size);

    cmpi_unmap(vir_rotate_vb);
    return TD_SUCCESS;
}

static td_s32 gfbg_set_rotatemode(const struct fb_info *info, ot_fb_rotate_mode rotate_mode)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_layer_buf buf_mode;
    volatile gfbg_compress_info *compress_info = &par->compress_info;

    gfbg_get_bufmode(par, &buf_mode);

    if (buf_mode == OT_FB_LAYER_BUF_BUTT || buf_mode == OT_FB_LAYER_BUF_NONE) {
        gfbg_error("doesn't support FBIOPUT_ROTATE_MODE when in standard mode or FB_LAYER_BUF_NONE!\n");
        return TD_FAILURE;
    }

    if ((par->color_format != OT_FB_FORMAT_ARGB4444) && (par->color_format != OT_FB_FORMAT_ARGB1555) &&
        (par->color_format != OT_FB_FORMAT_ARGB8888)) {
        gfbg_error("rotate only support FB_FORMAT_ARGB4444 and FB_FORMAT_ARGB1555 which is %d\n!\n",
                   par->color_format);
        return -EINVAL;
    }

    if (rotate_mode >= OT_FB_ROTATE_BUTT) {
        gfbg_error("The input rotate mode is wrong!\n");
        return -EINVAL;
    }

    if (compress_info->compress_open && rotate_mode != OT_FB_ROTATE_NONE) {
        gfbg_error("Can't rotate when in compress mode!\n");
        return -EINVAL;
    }

    if ((rotate_mode != OT_FB_ROTATE_NONE) && (display_info->mirror_mode != OT_FB_MIRROR_NONE)) {
        gfbg_error("Can't do rotate when mirror!\n");
        return -EINVAL;
    }

    if ((rotate_mode == OT_FB_ROTATE_90 || rotate_mode == OT_FB_ROTATE_270) && (par->rotate_vb == 0)) {
        /* init rotate vb */
        if (rotate_vb_init(info) != TD_SUCCESS) {
            return ENOMEM;
        }
    }

    display_info->rotate_mode = rotate_mode;

    return TD_SUCCESS;
}

/*
 * Name : gfbg_check_layerinfo
 * Desc :check layer information: buf refresh mode,position,canvas width
         and height, display width and height, screen width and height.
 */
static td_s32 gfbg_check_layerinfo(const ot_fb_layer_info* layer_info)
{
    if (layer_info->mask & OT_FB_LAYER_MASK_BUF_MODE) {
        if (layer_info->buf_mode > OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE) {
            gfbg_error("buf_mode(%d) is error, should between %d and %d\n", layer_info->buf_mode,
                       OT_FB_LAYER_BUF_DOUBLE, OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE);
            return TD_FAILURE;
        }
    }

    /* Detection of anti-flicker mode */
    if (layer_info->mask & OT_FB_LAYER_MASK_ANTIFLICKER_MODE) {
        if (layer_info->antiflicker_level > OT_FB_LAYER_ANTIFLICKER_AUTO) {
            gfbg_error("antiflicker_level(%d) is error, should between %d and %d\n", layer_info->antiflicker_level,
                       OT_FB_LAYER_ANTIFLICKER_NONE, OT_FB_LAYER_ANTIFLICKER_AUTO);
            return TD_FAILURE;
        }
    }

    /* check the width and height */
    if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
        if (layer_info->display_width % 2 || layer_info->display_height % 2) { /* 2 pixel align */
            gfbg_error("Disaplay W(%u) and H(%u) should align to 2!\n", layer_info->display_width,
                       layer_info->display_height);
            return TD_FAILURE;
        }
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
        if (layer_info->screen_width % 2 || layer_info->screen_height % 2) { /* 2 pixel align */
            gfbg_error("Screenaplay W(%u) and H(%u) should align to 2!\n", layer_info->screen_width,
                       layer_info->screen_height);
            return TD_FAILURE;
        }
    }

    /* check pre-multiplier value. */
    if (layer_info->mask & OT_FB_LAYER_MASK_MUL) {
        if (TD_TRUE != layer_info->is_premul && TD_FALSE != layer_info->is_premul) {
            gfbg_error("pstLayerInfo->is_premul should be TRUE or FALSE but it is %d\n", layer_info->is_premul);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 check_display_size(const struct fb_info *info, const gfbg_par *par, const ot_fb_layer_info *layer_info,
                                 td_bool is_interlace)
{
    td_u32 pitch;
    if (!g_drv_ops.capability[par->layer_id].is_vo_scale) {
        /*
         * If the chip does not support scaling,
         * if both the display size and the screen size are set , both must be equal.
         */
        if ((layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) && (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE)) {
            if ((layer_info->display_width != layer_info->screen_width) ||
                (layer_info->display_height != layer_info->screen_height)) {
                gfbg_error("Layer %d doesn't support scaling. Display(%d, %d) is different with Screen(%d, %d).\n",
                    par->layer_id, layer_info->display_width, layer_info->display_height,
                    layer_info->screen_width, layer_info->screen_height);
                return TD_FAILURE;
            }
        }
    }

    /* Modify the display size, the memory size has changed, limited by the size of the memory */
    if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
        pitch = (layer_info->display_width * gfbg_get_bits_per_pixel(info)) >> 3; /* 3 for 8bit */
        pitch = (pitch + 0xf) & 0xfffffff0;
        if (gfbg_check_mem_enough(info, pitch, layer_info->display_height) != TD_SUCCESS) {
            gfbg_error("memory is not enough!\n");
            return TD_FAILURE;
        }

        if (layer_info->display_width == 0 || layer_info->display_height == 0) {
            gfbg_error("display width/height shouldn't be 0!\n");
            return TD_FAILURE;
        }
        /*
         * For interlaced output, the height of the layer must be even.
         * Progressive output without this limit.
         */
        if (is_interlace && ((layer_info->display_height % 2) != 0)) { /* 2 for align */
            gfbg_error("display_height(%d) of layer_id %d should be even when vodev output is interlace\n",
                       layer_info->display_height, par->layer_id);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 onputlayerinfo_check_size(const struct fb_info *info, const gfbg_par *par,
                                        const ot_fb_layer_info *layer_info, td_bool is_interlace)
{
    /* Check the display size */
    if (check_display_size(info, par, layer_info, is_interlace) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* Check the canvas size */
    if (layer_info->mask & OT_FB_LAYER_MASK_CANVAS_SIZE) {
        if ((layer_info->canvas_width == 0) || (layer_info->canvas_height == 0)) {
            gfbg_error("canvas width/height shouldn't be 0\n");
            return TD_FAILURE;
        }
    }

    /* Check the screen size */
    if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
        if ((layer_info->screen_width == 0) || (layer_info->screen_height == 0)) {
            gfbg_error("screen width/height shouldn't be 0\n");
            return TD_FAILURE;
        }
       /*
        * For interlaced output, the height of the layer must be even.
        * Progressive output without this limit.
        */
        if (is_interlace && ((layer_info->screen_height % 2) != 0)) { /* 2 for align */
            gfbg_error("screen_height(%d) of layer_id %d should be even when vodev output is interlace\n",
                layer_info->screen_height, par->layer_id);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void gfbg_calculate_argb8888_cmp_stride(const struct fb_info *info, td_u32 *cmp_stride, td_u32 *uncmp_stride)
{
    gfbg_stride_attr attr = {0};

    /* only argb8888 can support compress with economize memory */
    attr.format = gfbg_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue,
        &info->var.transp, info->var.bits_per_pixel);
    attr.width = gfbg_get_xres(info);
    attr.is_lossless = TD_FALSE;
    attr.is_losslessa = TD_FALSE;

    if (attr.format == OT_FB_FORMAT_ARGB8888) {
        gfbg_recalculate_stride(cmp_stride, uncmp_stride, &attr);
    } else {
        *cmp_stride = *uncmp_stride = gfbg_get_line_length(info);
    }

    return;
}

static td_s32 onputlayerinfo_check_buf_mode(struct fb_info *info, gfbg_par *par, ot_fb_layer_info *layer_info,
                                            td_bool is_interlace)
{
    td_u32 uncmp_layer_size, cmp_layer_size;
    td_u32 uncmp_stride, cmp_stride;

    /* Modify the display buf mode, the memory size has changed, limited by the size of the memory */
    if (layer_info->mask & OT_FB_LAYER_MASK_BUF_MODE) {
        if (layer_info->buf_mode == OT_FB_LAYER_BUF_ONE) {
            uncmp_layer_size = gfbg_get_line_length(info) * gfbg_get_yres(info);
            cmp_layer_size = uncmp_layer_size;
        } else if ((layer_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE) ||
                   (layer_info->buf_mode == OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE)) {
            gfbg_calculate_argb8888_cmp_stride(info, &cmp_stride, &uncmp_stride);
            uncmp_layer_size = 2 * uncmp_stride * gfbg_get_yres(info); /* 2 buf */
            cmp_layer_size = 2 * cmp_stride * gfbg_get_yres(info); /* 2 buf */
        } else {
            uncmp_layer_size = 0;
            cmp_layer_size = 0;
        }

        if (gfbg_get_smem_len(info) < uncmp_layer_size && gfbg_get_smem_len(info) < cmp_layer_size) {
            /*
             * layer real memory size:%d KBytes, expected:%d KBtyes
             * real:gfbg_get_smem_len(info)/1024, expectde:uncmp_layer_size/1024 or cmp_layer_size/1024
             */
            gfbg_error("No enough mem!real:%dKB;expected:uncmp%dKB;cmp%dKB\n",
                gfbg_get_smem_len(info) / 1024, uncmp_layer_size / 1024, cmp_layer_size / 1024); /* 1024 for KB */
            return TD_FAILURE;
        }

        if (gfbg_get_smem_len(info) < uncmp_layer_size && gfbg_get_smem_len(info) >= cmp_layer_size) {
            /* this should be FBIOPUT_COMPRESSION_GFBG */
            par->compress_info.is_economize_memory = TD_TRUE;
        }

        /* If compression is enabled, only 2buf mode can be set. */
        if (par->compress_info.compress_open) {
            if (layer_info->buf_mode != OT_FB_LAYER_BUF_DOUBLE &&
                layer_info->buf_mode != OT_FB_LAYER_BUF_DOUBLE_IMMEDIATE) {
                gfbg_error("only GFBG_LAYER_BUF_DOUBLE/GFBG_LAYER_BUF_DOUBLE_IMMEDIATE support compress!\n");
                return TD_FAILURE;
            }
        }
    }

    /* if x>width or y>height ,how to deal with: see nothing in screen or return failure. */
    if (layer_info->mask & OT_FB_LAYER_MASK_POS) {
        if ((layer_info->x_pos < 0) || (layer_info->y_pos < 0)) {
            gfbg_error("It's not supported to set start pos of layer to negative!\n");
            return TD_FAILURE;
        }
        /*
         * For interlaced output, the start of the layer must be even.
         * Progressive output without this limit.
         */
        if (is_interlace && (layer_info->y_pos % 2 != 0)) { /* 2 for align */
            gfbg_error("y_pos should be even for interlace vodev!\n");
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 onputlayerinfo_check_premult(const gfbg_par* par, const ot_fb_layer_info *layer_info)
{
    /* Limit the pre-multiplication and color value. */
    if ((layer_info->mask & OT_FB_LAYER_MASK_MUL) && par->ckey.key_enable) {
        gfbg_error("Colorkey and premul couldn't take effect at same time!\n");
        return TD_FAILURE;
    }

    /* return TD_FAILURE, not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444 */
    if ((layer_info->mask & OT_FB_LAYER_MASK_MUL) && (layer_info->is_premul == TD_TRUE) &&
        (par->color_format == OT_FB_FORMAT_ARGB4444 || par->color_format == OT_FB_FORMAT_ARGB1555)) {
        gfbg_error("not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
        return TD_FAILURE;
    }
    /* not allow to set pre-mult mode when the GlobalAlpha is 1 */
    if ((layer_info->mask & OT_FB_LAYER_MASK_MUL) && (layer_info->is_premul == TD_TRUE) &&
        (par->alpha.global_alpha_en == TD_TRUE && par->alpha.global_alpha == 1)) {
        gfbg_error("not allow to set pre-mult mode when the GlobalAlpha is 1\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_void onputlayerinfo_set_with_mask(gfbg_par* par, const ot_fb_layer_info *layer_info)
{
    gfbg_display_info *display_info = &par->display_info;
    ot_fb_point pos;

    if (layer_info->mask & OT_FB_LAYER_MASK_MUL) {
        display_info->is_premul = layer_info->is_premul;
        par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_BMUL;
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_BUF_MODE) {
        gfbg_set_bufmode(par->layer_id, layer_info->buf_mode);
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_POS) {
        pos.x_pos = layer_info->x_pos;
        pos.y_pos = layer_info->y_pos;
        gfbg_set_layerpos(par, &pos);
    }

    if (layer_info->mask & OT_FB_LAYER_MASK_ANTIFLICKER_MODE) {
        gfbg_set_antiflickerlevel(par->layer_id, layer_info->antiflicker_level);
    }
    return;
}

static td_s32 onputlayerinfo_process(gfbg_par* par, const ot_fb_layer_info *layer_info)
{
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    td_s32 ret = TD_SUCCESS;
    unsigned long lock_flag;

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;

    /* deal with layer_info->mask */
    onputlayerinfo_set_with_mask(par, layer_info);

    /* set screen size in the scaling mode */
    if (g_drv_ops.capability[par->layer_id].is_vo_scale) {
        osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
         /*
          * The following two functions have a sleep operation inside,
          * you must unlock before calling, and lock the global amount inside the function.
          */
        if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
            ret = gfbg_disp_setscreensize(par->layer_id, layer_info->screen_width, layer_info->screen_height);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
        if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
            ret = gfbg_disp_setdispsize(par->layer_id, layer_info->display_width, layer_info->display_height);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
        osal_spin_lock_irqsave(&par->lock, &lock_flag);
    } else {
        /* no scaling mode, no buffer mode, screen size and display size can be set */
        if (refresh_info->buf_mode != OT_FB_LAYER_BUF_NONE) {
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            if (layer_info->mask & OT_FB_LAYER_MASK_SCREEN_SIZE) {
                ret = gfbg_disp_setscreensize(par->layer_id, layer_info->screen_width, layer_info->screen_height);
            }
            if (ret != TD_SUCCESS) {
                return ret;
            }
            if (layer_info->mask & OT_FB_LAYER_MASK_DISPLAY_SIZE) {
                ret = gfbg_disp_setdispsize(par->layer_id, layer_info->display_width, layer_info->display_height);
            }
            if (ret != TD_SUCCESS) {
                return ret;
            }
            osal_spin_lock_irqsave(&par->lock, &lock_flag);
        }
    }

    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
    return TD_SUCCESS;
}

static td_s32 gfbg_onputlayerinfo(struct fb_info *info, gfbg_par* par, const td_void __user *argp)
{
    td_s32 ret = TD_SUCCESS;
    ot_fb_layer_info layer_info;
    td_bool is_interlace = TD_FALSE;

    if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
        gfbg_warning("you shouldn't put cursor layer info!");
        return TD_SUCCESS;
    }

    if (argp == TD_NULL) {
        gfbg_error("NULL arg!\n");
        return -EINVAL;
    }

    if (osal_copy_from_user(&layer_info, argp, sizeof(ot_fb_layer_info))) {
        return -EFAULT;
    }

    ret = gfbg_check_layerinfo(&layer_info);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * Check the display size
     * Check the canvas size
     * Check the screen size
     */
    is_interlace = gfbg_is_interlace(par);
    if (onputlayerinfo_check_size(info, par, &layer_info, is_interlace) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * Check when modify buf mode
     * Check when modify pos
     */
    if (onputlayerinfo_check_buf_mode(info, par, &layer_info, is_interlace) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /* Check pre-mult */
    if (onputlayerinfo_check_premult(par, &layer_info) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    /*
     * avoid modifying register in vo isr before all params has been recorded!
     * In vo irq,flag modifying will be checked.
     */
    ret = onputlayerinfo_process(par, &layer_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    ret = gfbg_alloccanbuf(info, &layer_info);
    if (ret != TD_SUCCESS) {
        /*
         * There is no error returned here, because the user can also
         * specify this memory; in addition, even if the allocation is successful,
         * The user also needs to call FBIOGET_CANVAS_BUF to get it to operate.
         */
        gfbg_warning("alloc canvas buffer failed\n");
    }
    gfbg_refresh_again(par->layer_id);
    return ret;
}

static td_s32 gfbg_dosetcolreg(const gfbg_cmp_reg *color_reg, const struct fb_info *info, td_bool update_finished_reg)
{
    gfbg_par *par = (gfbg_par *)info->par;

    td_u32 argb = ((color_reg->transp & 0xff) << 24) | ((color_reg->red & 0xff) << 16) | /* 16 24 alg data */
        ((color_reg->green & 0xff) << 8) | (color_reg->blue & 0xff); /* 8 alg data */

    if (color_reg->regno > 255) { /* 255 is larger than */
        gfbg_warning("regno: %d, larger than 255!\n", color_reg->regno);
        return TD_FAILURE;
    }
    g_drv_ops.gfbg_drv_set_color_reg(par->layer_id, color_reg->regno, argb, update_finished_reg);
    return TD_SUCCESS;
}

static td_s32 gfbg_setcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp,
                             struct fb_info *info)
{
    gfbg_cmp_reg cmp_reg = {0};
    cmp_reg.regno = regno;
    cmp_reg.red = red;
    cmp_reg.green = green;
    cmp_reg.blue = blue;
    cmp_reg.transp = transp;
    return gfbg_dosetcolreg(&cmp_reg, info, TD_TRUE);
}

static td_void gfbg_set_color_reg(struct fb_cmap *cmap, struct fb_info *info)
{
    td_u32 i;
    td_s32 start;
    unsigned short *red = TD_NULL;
    unsigned short *green = TD_NULL;
    unsigned short *blue = TD_NULL;
    unsigned short *transp = TD_NULL;
    gfbg_cmp_reg cmp_reg = {0};

    cmp_reg.transp = 0xffff;
    red = cmap->red;
    green = cmap->green;
    blue = cmap->blue;
    transp = cmap->transp;

#ifdef __LITEOS__
    start = cmap->first;
#else
    start = cmap->start;
#endif

    for (i = 0; i < cmap->len; i++) {
        cmp_reg.red   = *red++;
        cmp_reg.green = *green++;
        cmp_reg.blue  = *blue++;
        if (transp != TD_NULL) {
            cmp_reg.transp = *transp++;
        }
        cmp_reg.regno = start;
        if (i < cmap->len - 1) {
            if (gfbg_dosetcolreg(&cmp_reg, info, TD_FALSE)) {
                break;
            }
        } else {   /* the last time update register */
            if (gfbg_dosetcolreg(&cmp_reg, info, TD_TRUE)) {
                break;
            }
        }
        start++;
    }
    return;
}

static td_s32 gfbg_setcmap(struct fb_cmap *cmap, struct fb_info *info)
{
    unsigned long lock_flag;
    gfbg_par *par = (gfbg_par *)info->par;

#ifdef CONFIG_GFBG_G0_FHD_SUPPORT
    if ((is_hd_layer(par->layer_id)) == TD_FALSE) {
        return -EINVAL;
    }
#else
    if ((is_sd_layer(par->layer_id)) == TD_FALSE) {
        return -EINVAL;
    }
#endif

    if (g_drv_ops.capability[par->layer_id].is_cmap == TD_FALSE) {
        /* AE6D03519, delete this color map warning! */
        return -EPERM;
    }

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    par->modifying = TD_TRUE;
    gfbg_set_color_reg(cmap, info);
    par->param_modify_mask |= GFBG_LAYER_PARAMODIEY_CLUT_UP;
    par->modifying = TD_FALSE;
    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    return 0;
}

#ifdef CONFIG_FB_CFB_IMAGEBLIT
static td_void gfbg_imageblit(struct fb_info *p, const struct fb_image *image)
{
    cfb_imageblit(p, image);
}
#endif

#ifdef __LITEOS__
static struct gfbg_info *g_gfbg[GFBG_MAX_LAYER_NUM] = {TD_NULL};

static td_s32 copy_rgb_to_cmap(td_u8 *cmap_start, const struct fb_cmap_s *src, struct fb_cmap_s *dst)
{
    td_s32 offset;
    td_s32 copy_size;
    int cmap_size;

    cmap_size = dst->len * sizeof(u_short);
    offset = src->first - dst->first;
    copy_size = (src->len > (dst->len - offset)) ? (dst->len - offset) : src->len;

    if (copy_size <= 0) {
        return -EINVAL;
    }
    copy_size *= sizeof(u_short);

    ret = memcpy_s(cmap_start + offset, copy_size, src->red, copy_size);
    gfbg_unequal_eok_return(ret);
    cmap_start += cmap_size;
    ret = memcpy_s(cmap_start + offset, copy_size,  src->green, copy_size);
    gfbg_unequal_eok_return(ret);
    cmap_start += cmap_size;
    ret = memcpy_s(cmap_start + offset, copy_size, src->blue, copy_size);
    gfbg_unequal_eok_return(ret);
    cmap_start += cmap_size;
    if (src->transp && dst->transp) {
        ret = memcpy_s(cmap_start + offset, copy_size, src->transp, copy_size);
        gfbg_unequal_eok_return(ret);
    }
    return TD_SUCCESS;
}

static int fb_copy_cmap(const struct fb_cmap_s *src, struct fb_cmap_s *dst)
{
    int offset;
    int copy_size;
    td_u8 *cmap_start = TD_NULL;
    int cmap_size;
    int ret;

    if ((src == TD_NULL) || (dst == TD_NULL)) {
        return -EINVAL;
    }

    cmap_start = dst->red;
    if (cmap_start == TD_NULL)
        return -EINVAL;

    cmap_size = dst->len * sizeof(u_short);

    if (src->first > dst->first) {
        ret = copy_rgb_to_cmap(cmap_start, src, dst);
        if (ret != TD_SUCCESS) {
            return TD_FAILURE;
        }
    } else {
        offset = dst->first - src->first;
        copy_size = ((src->len - offset) > dst->len) ? dst->len : (src->len - offset);

        if (copy_size <= 0) {
            return -EINVAL;
        }
        copy_size *= sizeof(u_short);

        ret = memcpy_s(cmap_start, copy_size, src->red + offset, copy_size);
        gfbg_unequal_eok_return(ret);
        cmap_start += cmap_size;
        ret = memcpy_s(cmap_start, copy_size, src->green + offset, copy_size);
        gfbg_unequal_eok_return(ret);
        cmap_start += cmap_size;
        ret = memcpy_s(cmap_start, copy_size, src->blue + offset, copy_size);
        gfbg_unequal_eok_return(ret);
        cmap_start += cmap_size;
        if (src->transp && dst->transp) {
            ret = memcpy_s(cmap_start, copy_size, src->transp + offset, copy_size);
            gfbg_unequal_eok_return(ret);
        }
    }
    return 0;
}

int fb_set_cmap(struct fb_cmap_s *cmap, struct gfbg_info *info)
{
    int ret;

    if (cmap == TD_NULL) {
        return -EINVAL;
    }

    if ((cmap->first < 0) || (info == TD_NULL))
        return -EINVAL;

    ret = gfbg_setcmap(cmap, info);
    if (ret == 0)
        fb_copy_cmap(cmap, &info->cmap);

    return ret;
}

FAR struct fb_vtable_s *up_fbgetvplane(int display, int vplane)
{
    (void)vplane;
    if (g_gfbg[display] == TD_NULL)
        return TD_NULL;

    return &g_gfbg[display]->vtable;
}

static int gfbg_getvideoinfo(FAR struct fb_vtable_s *vtable, FAR struct fb_videoinfo_s *vinfo)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;
    td_s32 ret;

    if ((info == TD_NULL) || (vinfo == TD_NULL))
        return -EINVAL;

    ret = memcpy_s(vinfo, sizeof(struct fb_videoinfo_s), &info->vinfo, sizeof(struct fb_videoinfo_s));
    gfbg_unequal_eok_return(ret);
    return 0;
}

static int gfbg_getplaneinfo(FAR struct fb_vtable_s *vtable, int planeno, FAR struct fb_planeinfo_s *pinfo)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;

    if ((info == TD_NULL) || (pinfo == TD_NULL))
        return -EINVAL;

    pinfo->fbmem = info->oinfo.fbmem;
    pinfo->fblen = info->oinfo.fblen;
    pinfo->display = info->oinfo.overlay;
    pinfo->bpp = info->oinfo.bpp;
    pinfo->stride = info->oinfo.stride;

    return 0;
}

static int gfbg_getoverlayinfo(FAR struct fb_vtable_s *vtable, int overlayno, FAR struct fb_overlayinfo_s *oinfo)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;
    td_s32 ret;

    if ((info == TD_NULL) || (oinfo == TD_NULL))
        return -EINVAL;

    ret = memcpy_s(oinfo, sizeof(struct fb_overlayinfo_s), &info->oinfo, sizeof(struct fb_overlayinfo_s));
    gfbg_unequal_eok_return(ret);

    return 0;
}

static int gfbg_getcmap(FAR struct fb_vtable_s *vtable, FAR struct fb_cmap_s *cmap)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;

    if ((info == TD_NULL) || (cmap == TD_NULL))
        return -EINVAL;

    return fb_copy_cmap(&info->cmap, cmap);
}

static int gfbg_putcmap(FAR struct fb_vtable_s *vtable, FAR const struct fb_cmap_s *cmap)
{
    struct gfbg_info *info = (struct gfbg_info *)vtable;
    int cpopy_size;

    if ((info == TD_NULL) || (cmap == TD_NULL))
        return -EINVAL;

    if ((cpopy_size = cmap->len * sizeof(td_u16)) < 0)
        return -EINVAL;

    return fb_set_cmap(cmap, info);
}

static td_s32 fbinitialize_alloc_mem(struct gfbg_info *info)
{
    td_char name[16] = {'\0'}; /* 16 name max length */
    gfbg_par *par = TD_NULL;
    par = (gfbg_par *)(info->par);

    if (g_layer[par->layer_id].layer_size != 0) {
        /* initialize the fix screen info */
        if (is_4k_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_4K]);
        } else if (is_hd_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_HD]);
        } else if (is_sd_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_SD]);
        } else if (is_ad_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_AD]);
        } else if (is_cursor_layer(par->layer_id)) {
            gfbg_screeninfo_init(info, &g_default_info[GFBG_LAYER_TYPE_CURSOR]);
        }
        if (snprintf_s(name, sizeof(name), 12, "gfbg_layer%01u", par->layer_id) < 0) { /* 12:for char length */
            gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
        }

        info->oinfo.fbmem = (void *)gfbg_buf_allocmem(name, strlen(name), (g_layer[par->layer_id].layer_size +
            /* 1024 alg data */
            g_layer[par->layer_id].curosr_buf_size) * 1024, g_layer_mmz_names[par->layer_id]);
        if (info->oinfo.fbmem == TD_NULL) {
            gfbg_error("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_layer[par->layer_id].layer_size + g_layer[par->layer_id].curosr_buf_size));
            gfbg_free(info);
            g_layer[par->layer_id].info = TD_NULL;
            return TD_FAILURE;
        }
        /* 1024:u32LayerSize is KB */
        info->oinfo.fblen = g_layer[par->layer_id].layer_size * 1024;

        /* initialize the virtual address and clear memory */
        info->oinfo.fbmem = gfbg_buf_map((td_u32)(uintptr_t)info->oinfo.fbmem,
                                         (g_layer[par->layer_id].layer_size +
                                         g_layer[par->layer_id].curosr_buf_size) * 1024); /* 1024 alg data */
        if (info->oinfo.fbmem == TD_NULL) {
            gfbg_warning("Failed to call map video memory,size:0x%x, start: 0x%lx\n", info->oinfo.fblen,
                         info->oinfo.fbmem);
        }

        (td_void)memset_s(info->oinfo.fbmem, info->oinfo.fblen, 0, info->oinfo.fblen);

        /* alloc color map */
        if (g_drv_ops.capability[par->layer_id].is_cmap) {
            /* now unsupported cmap function */
        }
    }
    return TD_SUCCESS;
}

static int up_fbinitialize(int display)
{
    td_s32 ret;
    struct gfbg_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;

    /* Creates a new frame buffer info structure. reserves gfbg_par for driver private data (info->par) */
    info = gfbg_alloc(sizeof(gfbg_par));
    if (info == TD_NULL) {
        gfbg_error("failed to malloc the gfbg_info!\n");
        return -ENOMEM;
    }

    /* save the info pointer in global pointer array */
    g_layer[display].info = info;

    info->oinfo.overlay = display;
    info->vtable.fb_open = gfbg_open;
    info->vtable.fb_release = gfbg_release;
    info->vtable.fb_set_par = gfbg_set_par;
    info->vtable.fb_pan_display = gfbg_pan_display;
    info->vtable.fb_ioctl = gfbg_ioctl;
    info->vtable.getvideoinfo = gfbg_getvideoinfo;
    info->vtable.getplaneinfo = gfbg_getplaneinfo;
    info->vtable.getoverlayinfo = gfbg_getoverlayinfo;
    info->vtable.getcmap = TD_NULL;
    info->vtable.putcmap = TD_NULL;

    par = (gfbg_par *)(info->par);
    (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));
    par->layer_id = display;
    par->color_format = OT_FB_FORMAT_ARGB1555;
    info->vinfo.format = OT_FB_FORMAT_ARGB1555;

    /* It's not need to alloc mem for cursor layer if use g_softcursor */
    ret = fbinitialize_alloc_mem(info);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    g_gfbg[display] = info;

    return TD_SUCCESS;
}

static td_void up_fbuninitialize(td_s32 display)
{
    struct gfbg_info* info = TD_NULL;

    /* get framebuffer info structure pointer */
    info = g_layer[display].info;
    if (info != TD_NULL) {
        if (info->oinfo.fbmem) {
            gfbg_buf_ummap(info->oinfo.fbmem);
            gfbg_buf_freemem((td_u32)(uintptr_t)info->oinfo.fbmem);
        }

        gfbg_free(info);
        g_layer[display].info = TD_NULL;
        g_gfbg[display] = TD_NULL;
    }
}

static td_void gfbg_overlay_cleanup(td_u32 layer_id, td_bool unregister)
{
    td_s32 ret;

    ret = fb_unregister(layer_id);
    if (ret < 0) {
        gfbg_error("gfbg overlay cleanup fail!\n");
    }

    return;
}

static td_s32 gfbg_overlay_probe(td_u32 layer_id)
{
    td_s32 ret = TD_FAILURE;

    if ((ret = fb_register(layer_id, 0)) < 0) {
        gfbg_error("failed to register_framebuffer!\n");
        ret = -EINVAL;
        goto ERR;
    }

    return TD_SUCCESS;

ERR:
    gfbg_overlay_cleanup(layer_id, TD_FALSE);

    return ret;
}

#else
/*
 * Name : g_ot_fb_ops
 * Desc : fb struct
 * See  : gfbg_overlay_probe
 */
static struct fb_ops g_ot_fb_ops = {
    .owner                 = THIS_MODULE,
    .fb_open               = gfbg_open,
    .fb_release           = gfbg_release,
    .fb_check_var       = gfbg_check_var,
    .fb_set_par           = gfbg_set_par,
    .fb_pan_display    = gfbg_pan_display,
    .fb_ioctl               = gfbg_ioctl,
    .fb_setcolreg        = gfbg_setcolreg,
    .fb_setcmap         = gfbg_setcmap,
#ifdef CONFIG_COMPAT
    .fb_compat_ioctl   = gfbg_compat_ioctl,
#endif
#ifdef CONFIG_FB_CFB_IMAGEBLIT
    .fb_imageblit      = gfbg_imageblit,
#endif
};

/*
 * Function        : gfbg_overlay_cleanup
 * Description     : releae the resource for certain framebuffer
 */
static td_void gfbg_overlay_cleanup(td_u32 layer_id, td_bool unregister)
{
    struct fb_info* info = TD_NULL;

    /* get framebuffer info structure pointer */
    info = g_layer[layer_id].info;
    if (info != TD_NULL) {
        if (gfbg_get_screen_base(info)) {
            gfbg_buf_ummap(gfbg_get_screen_base(info));
        }

        if (gfbg_get_smem_start(info)) {
            gfbg_buf_freemem(gfbg_get_smem_start(info));
        }

        if (unregister) {
            unregister_framebuffer(info);
        }

        framebuffer_release(info);
        g_layer[layer_id].info = TD_NULL;
    }

    return;
}

static td_s32 overlay_probe_alloc_mem(struct fb_info *info, struct fb_fix_screeninfo *fix,
                                      struct fb_var_screeninfo *var)
{
    gfbg_par *par = TD_NULL;
    td_u32 layer_id;
    td_char name[16]; /* 16 for length of name */

    par = (gfbg_par *)(info->par);
    layer_id = par->layer_id;

    if (g_layer[layer_id].layer_size != 0) {
        /* initialize the fix screen info */
        if (is_4k_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_4K];
            *var = g_default_var[GFBG_LAYER_TYPE_4K];
        } else if (is_hd_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_HD];
            *var = g_default_var[GFBG_LAYER_TYPE_HD];
        } else if (is_sd_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_SD];
            *var = g_default_var[GFBG_LAYER_TYPE_SD];
        } else if (is_ad_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_AD];
            *var = g_default_var[GFBG_LAYER_TYPE_AD];
        } else if (is_cursor_layer(layer_id)) {
            *fix = g_default_fix[GFBG_LAYER_TYPE_CURSOR];
            *var = g_default_var[GFBG_LAYER_TYPE_CURSOR];
        }
        if (snprintf_s(name, sizeof(name), 12, "gfbg_layer%01u", layer_id) < 0) { /* 12:for char length */
            gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
            return TD_FAILURE;
        }
        fix->smem_start = (td_ulong)gfbg_buf_allocmem(name, strlen(name),
            (g_layer[layer_id].layer_size + g_layer[layer_id].curosr_buf_size) * 1024, /* 1024 for 1k */
            gfbg_get_layer_mmz_names(layer_id));
        if (fix->smem_start == 0) {
            gfbg_error("%s:failed to malloc the video memory, size: %ld KBtyes!\n", name,
                (g_layer[layer_id].layer_size + g_layer[layer_id].curosr_buf_size));
            return TD_FAILURE;
        }
        /* u32LayerSize is KB */
        fix->smem_len = g_layer[layer_id].layer_size * 1024; /* 1024 for 1k */

        /* initialize the virtual address and clear memory */
        info->screen_base = gfbg_buf_map(fix->smem_start, (g_layer[layer_id].layer_size +
                                         g_layer[layer_id].curosr_buf_size) * 1024); /* 1024 for 1k */

        if (info->screen_base == TD_NULL) {
            gfbg_error("Failed to call map video memory,size:0x%x, start: 0x%lx\n",
                       fix->smem_len, fix->smem_start);
            gfbg_buf_freemem(fix->smem_start);
            fix->smem_start = 0;
            return TD_FAILURE;
        }
        gfbg_drv_set_mmz_addr(fix->smem_start, fix->smem_len, layer_id);

        /* Initialize the memory to 0. Call TDE to do it. */
        gfbg_fill_data(info, 0, gfbg_get_smem_len(info));
    }
    return TD_SUCCESS;
}

static td_s32 gfbg_overlay_probe(td_u32 layer_id)
{
    td_s32 ret = TD_FAILURE;
    struct fb_info *info = TD_NULL;
    struct fb_fix_screeninfo *fix = TD_NULL;
    struct fb_var_screeninfo *var = TD_NULL;
    gfbg_par *par = TD_NULL;

    /*
     * The size of the size represents the private data space of the device,
     * and the par of fb_info points to the private space, that is,
     * info->par already points to the memory space
     */
    info = framebuffer_alloc(sizeof(gfbg_par), TD_NULL);
    if (info == TD_NULL) {
        gfbg_error("failed to malloc the fb_info!\n");
        return -ENOMEM;
    }
    fix = &info->fix;
    var = &info->var;
    /* save the info pointer in global pointer array */
    g_layer[layer_id].info = info;

    info->flags = FBINFO_FLAG_DEFAULT | FBINFO_HWACCEL_YPAN | FBINFO_HWACCEL_XPAN;
    /* fbops members in fb_info point to g_ot_fb_ops, so open, release, ioctl, etc. can get fb_info. */
    info->fbops = &g_ot_fb_ops;

    par = (gfbg_par *)(info->par);
    (td_void)memset_s(par, sizeof(gfbg_par), 0, sizeof(gfbg_par));
    par->layer_id = layer_id;
    par->color_format = OT_FB_FORMAT_ARGB1555;

    if (snprintf_s(fix->id, sizeof(fix->id), 5, "ovl%01u", layer_id) < 0) { /* 5:for char length */
        gfbg_error("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        ret = TD_FAILURE;
        goto ERR;
    }
    /* It's not need to alloc mem for cursor layer if use g_softcursor */
    ret = overlay_probe_alloc_mem(info, fix, var);
    if (ret != TD_SUCCESS) {
        goto ERR;
    }

    if ((ret = register_framebuffer(info)) < 0) {
        gfbg_error("failed to register_framebuffer!layerid = %d, s32Ret = %d\n", layer_id, ret);
        ret = -EINVAL;
        goto ERR;
    }

    gfbg_info("succeed in registering the fb%d: %s frame buffer device\n", info->node, fix->id);

    return TD_SUCCESS;

ERR:
    gfbg_overlay_cleanup(layer_id, TD_FALSE);

    return ret;
}

#endif

/*
 * Function        : gfbg_get_vram_size
 * Description     : parse the parameter string and get the size. if
                     the parameter is invalid, the size is default value.
 * Input           : const char* pstr  the string for the vram size
 * Return          : the video memory size
 */
static unsigned long gfbg_get_vram_size(const char* pstr)
{
    td_s32 str_is_valid = TD_TRUE;
    unsigned long vram_size = 0;
    unsigned long vram_size_temp;
    const char* ptr = pstr;

    if ((ptr == TD_NULL) || (*ptr == '\0')) {
        return 0;
    }

    /* check if the string is valid */
    while (*ptr != '\0') {
        if (*ptr == ',') {
            break;
        } else if ((!((*ptr) >= '0' && (*ptr) <= '9')) && (*ptr != 'X') && (*ptr != 'x') &&
                   ((*ptr > 'f' && *ptr <= 'z') || (*ptr > 'F' && *ptr <= 'Z'))) {
            str_is_valid = TD_FALSE;
            break;
        }

        ptr++;
    }

    if (str_is_valid) {
        vram_size = osal_strtoul(pstr, (char **)TD_NULL, 0);
        if (vram_size > PAGE_SIZE_ALIGN_MAX) {
            gfbg_error("vram_size(%lu)( > %lu) is overflow, it will be set to %u!\n", vram_size,
                PAGE_SIZE_ALIGN_MAX, 0);
            vram_size = 0;
        }

        vram_size_temp = vram_size;
        /* make the size PAGE_SIZE align */
        vram_size = ((vram_size * 1024 + PAGE_SIZE - 1) & PAGE_MASK) / 1024; /* 2^10 1024 */
        if (vram_size_temp != vram_size) {
            gfbg_error("vram_size(%lu) if not align in 4, it will be set to %lu!\n", vram_size_temp, vram_size);
        }
    }
    return vram_size;
}

static td_void parse_cfg_start(td_char **sc_str)
{
    /* judge the cursor if use soft or hard layer */
    if (!osal_strcmp("off", g_softcursor)) {
        g_soft_cursor = TD_FALSE;
    } else {
        g_soft_cursor = TD_TRUE;
    }

    /* judge the display is need on */
    if (!osal_strcmp("on", g_display)) {
        g_display_on = TD_TRUE;
    } else {
        g_display_on = TD_FALSE;
    }

    /* get the string before next varm */
    *sc_str = osal_strstr(g_video, "vram");
    gfbg_info("video:%s\n", g_video);

    return;
}

static td_void parse_cfg_change_layer_size(const td_char *sc_str, td_u32 layer_id)
{
    td_ulong layer_size;
    layer_size = gfbg_get_vram_size(sc_str);

    if (g_drv_ops.capability[layer_id].is_layer_support) {
        if (is_cursor_layer(layer_id)) {
            if (!is_soft_cursor()) {
                g_layer[layer_id].layer_size = layer_size;
            }
        } else {
            g_layer[layer_id].layer_size = layer_size;
            if (is_soft_cursor() && layer_size) {
                g_layer[layer_id].curosr_buf_size = GFBG_CURSOR_DEF_VRAM;
            }
        }
    }
    return;
}

static td_s32 parse_cfg_change_layer_id(td_u32 *layer_id, const td_char *number, td_u32 length)
{
    if (length > 4) { /* 4 for array number max length */
        gfbg_error("length = %d; out of range!\n", length);
        return TD_FAILURE;
    }

    *layer_id = osal_strtoul(number, (char **)TD_NULL, 10); /* 10 for length */
    if (*layer_id >= g_drv_ops.layer_count) {
        gfbg_error("Layer %d is in module_param---video out of range!\n", *layer_id);
        return TD_FAILURE;
    }

    if (!g_drv_ops.capability[*layer_id].is_layer_support) {
        gfbg_error("Layer %d unsupported, so module_param---video can't contain vram_size for it!\n", *layer_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * Name : gfbg_parse_cfg
 * Desc : Parse the parameters.
 * See  : gfbg_overlay_probe
 */
static td_s32 gfbg_parse_cfg(td_void)
{
    td_char *sc_str = TD_NULL;
    td_char number[4] = {0}; /* 4 cfg num */
    td_u32 i;
    td_u32 j;
    td_u32 layer_id;
    td_char ac_param[12] = {0}; /* 12 param length */
    td_char ac_temp[12] = {0}; /* 12 param length */
    td_bool is_param_valid = TD_FALSE;

    /*
     * 1.judge the cursor if use soft or hard layer
     * 2.judge the display is need on
     * 3.get the string before next varm
     */
    parse_cfg_start(&sc_str);

    /* parse cfg process */
    while (sc_str != TD_NULL) {
        /* parse the layer id and save it in a string */
        i = 0;

        /* if the number of graphics layers is 10 or more, the string is risky */
        for (j = 0; j < g_drv_ops.layer_count; j++) {
            if (snprintf_s(ac_param, sizeof(ac_param), 11, "vram%01u_size", j) < 0) { /* 11:for char length */
                gfbg_error("%s:%d:snprintf_s failure!\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            if (strncpy_s(ac_temp, sizeof(ac_temp), sc_str, 10) != EOK) { /* 10 cpy num */
                gfbg_error("%s:%d:strncpy_s failure!\n", __FUNCTION__, __LINE__);
                return TD_FAILURE;
            }
            if (!osal_strcmp(ac_param, ac_temp)) {
                is_param_valid = TD_TRUE;
            }
        }
        if (!is_param_valid) {
            gfbg_error("insmod parameter is invalid!\n");
            return TD_FAILURE;
        }
        /* 4:skip "vram" */
        sc_str += 4;
        while (*sc_str != '_') {
            /* i>1 means layer id is bigger than 100, it's obviously out of range! */
            if (i > 1) {
                gfbg_error("layer id is out of range!\n");
                return -1;
            }

            number[i] = *sc_str;
            i++;
            sc_str++;
        }

        number[i] = '\0';

        /* change the layer id string into digital and assure it's legal */
        if (parse_cfg_change_layer_id(&layer_id, number, sizeof(number)) != TD_SUCCESS) {
            return TD_FAILURE;
        }

        sc_str += sizeof("size") + i;
        /* get the layer size string and change it to digital */
        parse_cfg_change_layer_size(sc_str, layer_id);

        /* get next layer string */
        sc_str = osal_strstr(sc_str, "vram");
    }

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static const td_char* g_fmt_name[] = {
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
    "BGR565",
    "BGR888",
    "ABGR4444",
    "ABGR1555",
    "ABGR8888",
    "ABGR8565",
    "KBGR444",
    "KBGR555",
    "KBGR888",
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
    "BUTT"
};

static const td_char *g_layer_name[] = {"layer_0", "layer_1", "layer_2", "layer_3", "layer_4"};
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
static td_s32 gfbg_print_softcursor_proc(struct fb_info *info, osal_proc_entry *p, td_void *v)
{
    gfbg_par *par = (gfbg_par *)info->par;
    const td_char *layer_name = TD_NULL;
    ot_unused(v);

    if (par->layer_id >= sizeof(g_layer_name) / sizeof(td_char *)) {
        layer_name = "unknown layer";
    } else {
        layer_name = g_layer_name[par->layer_id];
    }

    osal_seq_printf(p->seqfile, "layer_name                 \t: %s \n", layer_name);
    osal_seq_printf(p->seqfile, "show_state               \t :%s\n", par->show ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "referecce_count            \t :%d\n", atomic_read(&par->ref_count));
    osal_seq_printf(p->seqfile, "position                   \t :(%d, %d)\n", par->display_info.pos.x_pos,
        par->display_info.pos.y_pos);
    osal_seq_printf(p->seqfile, "color_format:               \t :%s\n", g_fmt_name[par->color_format]);
    osal_seq_printf(p->seqfile, "pixel_alpha                 \t :%s\n", par->alpha.pixel_alpha ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "global_alpha_en           \t :%s\n", par->alpha.global_alpha_en ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "alpha0, alpha1             \t :%d, %d\n", par->alpha.alpha0, par->alpha.alpha1);
    osal_seq_printf(p->seqfile, "alpha_global               \t :%d\n", par->alpha.global_alpha);
    osal_seq_printf(p->seqfile, "colorkey_en                \t :%s\n", par->ckey.key_enable ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "colorkey_value             \t :0x%x\n", par->ckey.key);
    osal_seq_printf(p->seqfile, "cursor_hot_pos(x, y)       \t :(%d, %d)\n", par->cursor_info.cursor.hot_pos.x_pos,
        par->cursor_info.cursor.hot_pos.y_pos);
    return 0;
}

static td_void print_cursor_proc(osal_proc_entry *p, const gfbg_par *par)
{
    osal_seq_printf(p->seqfile, "attach_cursor_id:         \t  :%d \n", par->cursor_info.attached_cursor_id);
    osal_seq_printf(p->seqfile, "backup_cursor_addr         \t :0x%ld\n",
        (td_ulong)par->cursor_info.cursor.cursor.phys_addr);
    osal_seq_printf(p->seqfile, "backup_cursor_format  \t :%s\n", g_fmt_name[par->cursor_info.cursor.cursor.format]);
    osal_seq_printf(p->seqfile, "backup_cursor_stride       \t :%d\n", par->cursor_info.cursor.cursor.pitch);
    osal_seq_printf(p->seqfile, "backup_cursor (width, h)       \t :(%d, %d)\n",
        par->cursor_info.cursor.cursor.width, par->cursor_info.cursor.cursor.height);
    osal_seq_printf(p->seqfile, "cursor_rect in display buffer \t :(%d, %d, %d, %d)\n",
        par->cursor_info.rect_in_disp_buf.x, par->cursor_info.rect_in_disp_buf.y,
        par->cursor_info.rect_in_disp_buf.width, par->cursor_info.rect_in_disp_buf.height);
    osal_seq_printf(p->seqfile, "cursor_pos in cursor image \t :(%d, %d)\n",
        par->cursor_info.pos_in_cursor.x_pos, par->cursor_info.pos_in_cursor.y_pos);
    return;
}
#endif

static td_void print_canvas_proc(osal_proc_entry *p, const gfbg_par *par)
{
    const gfbg_refresh_info *refresh_info = &par->refresh_info;
    const volatile gfbg_compress_info *compress_info = &par->compress_info;
    graphic_layer_context *gfx_layer_ctx = fb_graphics_get_gfx_layer_ctx();

    osal_seq_printf(p->seqfile, "canavas_updated_addr       \t :0x%lx\n",
        (td_ulong)refresh_info->user_buffer.canvas.phys_addr +
        refresh_info->user_buffer.update_rect.y * refresh_info->user_buffer.update_rect.width +
        refresh_info->user_buffer.update_rect.x);
    osal_seq_printf(p->seqfile, "canavas_updated (w, h)     \t :%d,%d \n", refresh_info->user_buffer.update_rect.width,
        refresh_info->user_buffer.update_rect.height);
    osal_seq_printf(p->seqfile, "canvas_width               \t :%d\n", refresh_info->user_buffer.canvas.width);
    osal_seq_printf(p->seqfile, "canvas_height              \t :%d\n", refresh_info->user_buffer.canvas.height);
    osal_seq_printf(p->seqfile, "canvas_pitch               \t :%d\n", refresh_info->user_buffer.canvas.pitch);
    osal_seq_printf(p->seqfile, "canvas_format              \t :%s\n",
        g_fmt_name[refresh_info->user_buffer.canvas.format]);
    osal_seq_printf(p->seqfile, "is_compress                 \t :%s\n", compress_info->compress_open ? "YES" : "NO");
    osal_seq_printf(p->seqfile, "is_ddr_dettect             \t :%s\n", (compress_info->compress_open &&
        compress_info->zone_nums && (refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE ||
        refresh_info->buf_mode == OT_FB_LAYER_BUF_BUTT)) ? "YES" : "NO");
    osal_seq_printf(p->seqfile, "ddr_detect_zones           \t :%d\n", (compress_info->compress_open &&
        (refresh_info->buf_mode == OT_FB_LAYER_BUF_NONE ||
        refresh_info->buf_mode == OT_FB_LAYER_BUF_BUTT)) ? compress_info->zone_nums : 0);
    osal_seq_printf(p->seqfile, "premul_enable                 \t :%s\n", par->display_info.is_premul ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "layer_csc_matrix:       \t :%d\n", gfx_layer_ctx[par->layer_id].gfx_csc.csc_matrix);
    osal_seq_printf(p->seqfile, "layer_luma:             \t :%d\n", gfx_layer_ctx[par->layer_id].gfx_csc.luma);
    osal_seq_printf(p->seqfile, "layer_contrast:         \t :%d\n", gfx_layer_ctx[par->layer_id].gfx_csc.contrast);
    osal_seq_printf(p->seqfile, "layer_hue:              \t :%d\n", gfx_layer_ctx[par->layer_id].gfx_csc.hue);
    osal_seq_printf(p->seqfile, "layer_saturation:       \t :%d\n", gfx_layer_ctx[par->layer_id].gfx_csc.saturation);
    osal_seq_printf(p->seqfile, "layer_ex_csc_en:        \t :%s\n",
        gfx_layer_ctx[par->layer_id].gfx_csc.ex_csc_en == TD_TRUE ? "ON" : "OFF");
    return;
}

static td_void print_display_proc(osal_proc_entry *p, gfbg_par *par)
{
    const td_char *buf_mode[] = {"triple", "double", "single", "triple( no frame discarded)", "unknown"};
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;

    osal_seq_printf(p->seqfile, "display_buffer_mode(+usr_buf)\t :%s\n", buf_mode[refresh_info->buf_mode]);
    osal_seq_printf(p->seqfile, "displaying_addr (register) \t :0x%lx\n", (td_ulong)refresh_info->screen_addr);
    osal_seq_printf(p->seqfile, "display_buffer[0] addr     \t :0x%lx\n",
        (td_ulong)refresh_info->disp_buf_info.phys_addr[0]);
    osal_seq_printf(p->seqfile, "display_buffer[1] addr     \t :0x%lx\n",
        (td_ulong)refresh_info->disp_buf_info.phys_addr[1]);
    osal_seq_printf(p->seqfile, "is_premul_mode:            \t :%s\n",
        (display_info->is_premul == TD_TRUE) ? "YES" : "NO");
    osal_seq_printf(p->seqfile, "display_rect                \t :(%d, %d)\n", display_info->display_width,
        display_info->display_height);
    osal_seq_printf(p->seqfile, "screen_rect                 \t :(%d, %d)\n", display_info->screen_width,
        display_info->screen_height);
    osal_seq_printf(p->seqfile, "device_max_resolution      \t :%d, %d\n", display_info->max_screen_width,
        display_info->max_screen_height);
    osal_seq_printf(p->seqfile, "is_need_flip(2buf)           \t :%s\n",
        refresh_info->disp_buf_info.need_flip ? "YES" : "NO");
    osal_seq_printf(p->seqfile, "buf_index_displaying(2buf)\t :%d\n", refresh_info->disp_buf_info.index_for_int);
    osal_seq_printf(p->seqfile, "refresh_request_num(2buf)  \t :%d\n", refresh_info->refresh_num);
    osal_seq_printf(p->seqfile, "switch_buf_num(2buf)       \t :%d\n", refresh_info->disp_buf_info.int_pic_num);
    osal_seq_printf(p->seqfile, "union_rect (2buf)          \t :(%d,%d,%d,%d)\n",
        refresh_info->disp_buf_info.union_rect.x, refresh_info->disp_buf_info.union_rect.y,
        refresh_info->disp_buf_info.union_rect.width, refresh_info->disp_buf_info.union_rect.height);
    return;
}

static td_void print_common_proc(osal_proc_entry *p, struct fb_info *info, const td_char *layer_name)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    gfbg_refresh_info *refresh_info = &par->refresh_info;
    const td_char *antiflicer_level[] = {"NONE", "LOW", "MIDDLE", "HIGH", "AUTO", "ERROR"};
    const td_char *mirror_mode[] = {"NONE", "HORIZONTAL", "VERTICAL", "BOTH", "unknown"};
    const td_char *anti_mode[] = {"NONE", "TDE", "VOU", "ERROR"};
    const td_char *rotation_mode[] = {"0", "90", "180", "270", "-"};

    osal_seq_printf(p->seqfile, "\n[GFBG] Version: ["OT_MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n");
    osal_seq_printf(p->seqfile, "\n");
    osal_seq_printf(p->seqfile, "layer_name                 \t :%s \n", layer_name);
    osal_seq_printf(p->seqfile, "open_count                 \t :%d\n", atomic_read(&par->ref_count));
    osal_seq_printf(p->seqfile, "show_state                 \t :%s\n", (par->show) ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "graphic_enable             \t :%s\n", (par->layer_open == TD_TRUE) ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "start_position             \t :(%d, %d)\n", display_info->pos.x_pos,
        display_info->pos.y_pos);
    osal_seq_printf(p->seqfile, "xres, yres                 \t :(%d, %d)\n", gfbg_get_xres(info), gfbg_get_yres(info));
    osal_seq_printf(p->seqfile, "xres_virtual, yres_virtual \t :(%d, %d)\n", gfbg_get_xres_virtual(info),
        gfbg_get_yres_virtual(info));
    osal_seq_printf(p->seqfile, "xoffset, yoffset           \t :(%d, %d)\n", gfbg_get_xoffset(info),
        gfbg_get_yoffset(info));
    osal_seq_printf(p->seqfile, "fix.line_length            \t :%d\n", gfbg_get_line_length(info));
    osal_seq_printf(p->seqfile, "mem_size:                  \t :%d KB\n", gfbg_get_smem_len(info) / 1024); /* 1024 1K */
    osal_seq_printf(p->seqfile, "layer_scale (hw):          \t :%s \n",
        g_drv_ops.capability[par->layer_id].is_vo_scale ? "YES" : "NO");
    osal_seq_printf(p->seqfile, "color_format:               \t :%s\n", g_fmt_name[par->color_format]);
    osal_seq_printf(p->seqfile, "pixel_alpha                    \t :%s\n", par->alpha.pixel_alpha ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "global_alpha_en           \t :%s\n", par->alpha.global_alpha_en ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "alpha0, alpha1             \t :%d, %d\n", par->alpha.alpha0, par->alpha.alpha1);
    osal_seq_printf(p->seqfile, "alpha_global               \t :%d\n", par->alpha.global_alpha);
    osal_seq_printf(p->seqfile, "colorkey_en                \t :%s\n", par->ckey.key_enable ? "ON" : "OFF");
    osal_seq_printf(p->seqfile, "colorkey_value             \t :0x%x\n", par->ckey.key);
    osal_seq_printf(p->seqfile, "mirror_mode:            \t :%s\n", mirror_mode[display_info->mirror_mode]);
    osal_seq_printf(p->seqfile, "dynamic_range:            \t :%s\n", "SDR8");
    osal_seq_printf(p->seqfile, "deflicker_mode:            \t :%s\n", anti_mode[display_info->antiflicker_mode]);
    osal_seq_printf(p->seqfile, "rotation_mode:             \t :%s\n", rotation_mode[display_info->rotate_mode]);
    osal_seq_printf(p->seqfile, "deflicker_level:           \t :%s\n",
        antiflicer_level[display_info->antiflicker_level]);
    osal_seq_printf(p->seqfile,  "gfbg_mode:                  \t :%s\n",
        (refresh_info->buf_mode == OT_FB_LAYER_BUF_BUTT) ? "STANDARD" : "EXTEND");
    return;
}

static td_s32 gfbg_print_layer_proc(struct fb_info *info, osal_proc_entry *p, td_void *v)
{
    gfbg_par *par = (gfbg_par *)info->par;
    gfbg_display_info *display_info = &par->display_info;
    const td_char *layer_name = TD_NULL;
    ot_unused(v);

    if (par->layer_id >= sizeof(g_layer_name) / sizeof(td_char *)) {
        layer_name = "unknown layer";
    } else {
        layer_name = g_layer_name[par->layer_id];
    }

    if (display_info->antiflicker_mode > GFBG_ANTIFLICKER_BUTT) {
        display_info->antiflicker_mode = GFBG_ANTIFLICKER_BUTT;
    }

    if (display_info->antiflicker_level > OT_FB_LAYER_ANTIFLICKER_BUTT) {
        display_info->antiflicker_level = OT_FB_LAYER_ANTIFLICKER_BUTT;
    }

    print_common_proc(p, info, layer_name);

    print_display_proc(p, par);

    print_canvas_proc(p, par);

#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    if (par->cursor_info.attached && is_soft_cursor()) {
        print_cursor_proc(p, par);
    }
#endif
    return TD_SUCCESS;
}

static td_s32 gfbg_read_proc(osal_proc_entry *entry)
{
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    if (entry == TD_NULL) {
        return TD_FAILURE;
    }
    info = (struct fb_info *)(entry->private_data);
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)info->par;
    if (par == TD_NULL) {
        return TD_FAILURE;
    }

    if (!is_cursor_layer(par->layer_id) || !is_soft_cursor()) {
        return gfbg_print_layer_proc(info, entry, TD_NULL);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
    } else {
        return gfbg_print_softcursor_proc(info, entry, TD_NULL);
#endif
    }

    return TD_SUCCESS;
}

static td_void parse_procmd_help(const gfbg_par *par, const td_char *cmd)
{
    if (osal_strncmp("help", cmd, 4) == 0) { /* 4 cmp */
        osal_printk("help info:\n");
        osal_printk("echo cmd > proc file\n");
        osal_printk("gfbg support cmd:\n");
        osal_printk("show:show layer\n");
        osal_printk("hide:hide layer\n");
        osal_printk("For example, if you want to hide layer 0,you can input:\n");
        osal_printk("   echo hide > /proc/umap/gfbg0\n");
    } else {
        gfbg_error("layer_id %d doesn't support cmd:%s, use help cmd to show help info!\n",
            par->layer_id, cmd);
    }
    return;
}

static td_void gfbg_parse_proccmd(const osal_proc_entry *p, td_u32 layer_id, const td_char *cmd)
{
    struct fb_info *info = g_layer[layer_id].info;
    gfbg_par *par = (gfbg_par *)info->par;
    td_s32 cnt;
    unsigned long lock_flag;
    ot_unused(p);

    osal_spin_lock_irqsave(&par->lock, &lock_flag);
    cnt = atomic_read(&par->ref_count);

    if (osal_strncmp("show", cmd, 4) == 0) { /* 4 cmp */
        if (cnt == 0) {
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            gfbg_error("err:layer_id %d no open!\n", par->layer_id);
            return;
        }

        if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            gfbg_error("cursor layer %d doesn't support this cmd!\n", par->layer_id);
            return;
        }

        if (!par->show) {
            par->modifying = TD_TRUE;
            par->show = TD_TRUE;
            par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;
            par->modifying = TD_FALSE;
        }
    } else if (osal_strncmp("hide", cmd, 4) == 0) { /* 4 cmp */
        if (cnt == 0) {
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            gfbg_error("err:layer_id %d no open!\n", par->layer_id);
            return;
        }

        if (is_cursor_layer(par->layer_id) && is_soft_cursor()) {
            osal_spin_unlock_irqrestore(&par->lock, &lock_flag);
            gfbg_error("cursor layer %d doesn't support this cmd!\n", par->layer_id);
            return;
        }

        if (par->show) {
            par->modifying = TD_TRUE;
            par->show = TD_FALSE;
            par->param_modify_mask |= GFBG_LAYER_PARAMODIFY_SHOW;
            par->modifying = TD_FALSE;
        }
    }

    osal_spin_unlock_irqrestore(&par->lock, &lock_flag);

    parse_procmd_help(par, cmd);
    return;
}

static td_s32 gfbg_write_proc(osal_proc_entry *entry, const char *buf, int count, long long *ppos)
{
#ifndef __LITEOS__
#define TMP_BUF_LEN 32
    struct fb_info *info = TD_NULL;
    gfbg_par *par = TD_NULL;
    char tmp_buf[TMP_BUF_LEN] = {0};
    int len;

    if (entry == TD_NULL) {
        return -ENOSYS;
    }

    if (count <= 0) {
        return -ENOSYS;
    }

    len = (count >= TMP_BUF_LEN) ? TMP_BUF_LEN : count;

    if (osal_copy_from_user(tmp_buf, buf, len)) {
        return TD_FAILURE;
    }
    tmp_buf[len - 1] = '\0';

    info = (struct fb_info *)(entry->private_data);
    if (info == TD_NULL) {
        return TD_FAILURE;
    }
    par = (gfbg_par *)(info->par);
    if (par == TD_NULL) {
        return TD_FAILURE;
    }

    gfbg_parse_proccmd(entry, par->layer_id, (td_char*)tmp_buf);
    if (memset_s((td_void *)tmp_buf, TMP_BUF_LEN, 0, count) != EOK) {
        gfbg_error("%s:%d:memset_s failed!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
#endif
    ot_unused(ppos);
    return count;
}
#endif

