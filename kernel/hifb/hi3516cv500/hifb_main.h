/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */
#ifndef __HIFB_MAIN_H__
#define __HIFB_MAIN_H__

#include "hifb_vou_drv.h"
#include "drv_tde_type.h"

/* define the value of default set of each layer */
#define HIFB_4K_DEF_WIDTH 3840
#define HIFB_4K_DEF_HEIGHT 2160

#define HIFB_HD_DEF_WIDTH 1920 /* 1280 */                           /* unit: pixel */
#define HIFB_HD_DEF_HEIGHT 1080 /* 720 */                           /* unit: pixel */
#define HIFB_HD_DEF_STRIDE (HIFB_HD_DEF_WIDTH * 4)                  /* unit: byte */
#define HIFB_HD_DEF_VRAM 16200 /* 7200, unit:KB 1280*720*4*2, unit:KB 1920*1080*4*2 */

#define HIFB_SD_DEF_WIDTH 720
#define HIFB_SD_DEF_HEIGHT 576
#define HIFB_SD_DEF_STRIDE 1440
#define HIFB_SD_DEF_VRAM 3240 /* unit:KB 720*576*4 */

#define HIFB_AD_DEF_WIDTH 720
#define HIFB_AD_DEF_HEIGHT 576
#define HIFB_AD_DEF_STRIDE 1440
#define HIFB_AD_DEF_VRAM 3240 /* unit:KB 720*576*4 */

#define HIFB_CURSOR_DEF_WIDTH 256
#define HIFB_CURSOR_DEF_HEIGHT 256
#define HIFB_CURSOR_DEF_STRIDE 512
#define HIFB_CURSOR_DEF_VRAM 512 /* unit:KB for doudble buffer mode, we need 2 memory buffer, save cursor */

#define HIFB_SOFTCURSOR_STRIDE 512

#define HIFB_DEF_DEPTH 16 /* unit: bits */
#define HIFB_DEF_XSTART 0
#define HIFB_DEF_YSTART 0
#define HIFB_DEF_ALPHA 0xff

#define HIFB_MAX_ZOOMIN 15

#if (HICHIP == HI3519A_V100)
#define HIFB_TDE_DISTINCT_WIDTH 1920
#define HIFB_TDE_DISTINCT_HEIGHT 1080
#else
#define HIFB_TDE_DISTINCT_WIDTH 3840
#define HIFB_TDE_DISTINCT_HEIGHT 2160
#endif

#define HIFB_CMAP_SIZE 0 /* unit:KB 256*4 */
#define MMB_ADDR_INVALID 0

#define hifb_is_clutfmt(fmt) (((fmt) >= HIFB_FMT_1BPP) && ((fmt) <= HIFB_FMT_ACLUT88))
#define HIFB_ALPHA_OPAQUE 0xff
#define HIFB_ALPHA_TRANSPARENT 0x00

#define HIFB_DEFLICKER_LEVEL_MAX 5 /* support level 5 deflicker most */

#define HIFB_MAX_CURSOR_WIDTH 128
#define HIFB_MAX_CURSOR_HEIGHT 128

#define HIFB_SPIN_LOCK_S spinlock_t
#define hifb_spin_lock_init(lock) spin_lock_init(lock)
#define hifb_spin_lock_irqsave(lock, flag) spin_lock_irqsave(lock, flag)
#define hifb_spin_unlock_irqrestore(lock, flag) spin_unlock_irqrestore(lock, flag)

typedef enum {
    HIFB_ANTIFLICKER_NONE, /* no antiflicker.If scan mode is progressive, hifb will set antiflicker mode to none */
    HIFB_ANTIFLICKER_TDE,  /* tde antiflicker mode, it's effect for 1buf or 2buf only */
    HIFB_ANTIFLICKER_VO,   /* vo antiflicker mode, need hardware supprot */
    HIFB_ANTIFLICKER_BUTT
} hifb_layer_antiflicker_mode;

typedef struct {
    HIFB_POINT_S pos;
    hi_u32 display_width;
    hi_u32 display_height;
    hi_u32 screen_width;
    hi_u32 screen_height;
    hi_bool premul;
    hi_bool need_antiflicker;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E antiflicker_level; /* antiflicker level */
    hifb_layer_antiflicker_mode antiflicker_mode;   /* antiflicker mode */
    hi_u32 vir_x_res;
    hi_u32 vir_y_res;
    hi_u32 x_res;
    hi_u32 y_res;
    hi_u32 max_screen_width;
    hi_u32 max_screen_height;
    HIFB_MIRROR_MODE_E mirror_mode;
    HIFB_ROTATE_MODE_E rotate_mode;
    HIFB_DYNAMIC_RANGE_E dynamic_range; /* which dynamic range */
} hifb_display_info;

typedef struct {
    hi_u64 phy_addr[2]; /* 2 buf address */
    hi_u32 stride;     /* buf stride */
    hi_bool need_flip;
    hi_bool fliped;
    hi_u32 index_for_int;
    hi_u32 int_pic_num;
    HIFB_RECT union_rect;
    hi_s32 refresh_handle;
    hi_bool compress; /* Identifies whether the frame to be displayed is compressed */
} hifb_dispbuf_info;

typedef struct {
    HIFB_LAYER_BUF_E buf_mode; /* buffer mode */
    HIFB_BUFFER_S user_buffer;
    hi_u64 screen_addr;   /* screen buf addr */
    hi_u64 gb_screen_addr; /* new GB screen buf addr */
    hifb_dispbuf_info disp_buf_info;
    hi_u32 refresh_num; /* refresh request num in 2 buf mode */
    hi_bool do_refresh_job;
} hifb_refresh_info;

typedef struct {
    /*
     * For cursor layer, stCursor means cursor buffer, it is alloced and freed
     * both by user;for general layer,stCursor means back buf
     */
    HIFB_CURSOR_S cursor;

    /*
     * For cursor layer,you can quary whether cursor attach to a certain layer
     * for general layer, you can quary whether cursor attach to it
     */
    hi_u32 attched;

    /* valid area:overlap region between attached layer and cursor layer */
    HIFB_RECT rect_in_disp_buf;

    /* the orignal position of cursor, usually is (0,0) but also has different when at margin */
    HIFB_POINT_S pos_in_cursor;

    hi_u32 attached_cursor_id;
} hifb_cursor_info;

typedef struct {
    hi_bool compress_open;
    hi_bool decompress_open;
    hi_bool update_finished;

    HIFB_RECT update_rect;

    HIFB_RECT compress_rect;

    hi_bool delay;

    hi_u64 ar_buf_phy_addr;
    hi_u64 gb_buf_phy_addr;
    hi_u32 frame_size0;
    hi_u32 frame_size1;
    hi_u32 header_size;
    hi_u32 stride;

    hi_bool layer_addr_update;

    hi_u32 start_section;
    hi_u32 zone_nums;
    hi_bool clear_zone;

    hi_u32 new_start_section;
    hi_u32 new_zone_nums;
} hifb_compress_info;

typedef struct {
    hi_u32 layer_id;                 /* layer id */
    atomic_t ref_count;              /* framebuffer reference count */
    hi_bool show;                    /* show status */
    HIFB_COLOR_FMT_E color_format;   /* color format */
    HIFB_ALPHA_S alpha;              /* alpha attribution */
    hifb_colorkeyex ckey;            /* colorkey attribution */
    hifb_display_info display_info;  /* display info */
    hifb_refresh_info refresh_info;
    hifb_cursor_info cursor_info;
    volatile hifb_compress_info compress_info;
    hi_u32 param_modify_mask;
    volatile hi_bool modifying;
    HIFB_SURFACE_S canvas_sur;                      /* canvas surface */
    hi_u32 h_dflevel;                               /* horizontal deflicker level */
    hi_u32 v_dflevel;                               /* vertical deflicker level */
    hi_u8 h_dfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];   /* horizontal deflicker coefficients */
    hi_u8 v_dfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];   /* vertical deflicker coefficients */
    volatile hi_u32 vblflag;
    wait_queue_head_t vbl_event;
    wait_queue_head_t do_refresh_job;
    spinlock_t lock;
    hi_bool layer_open;
    hi_u64 rotate_vb;
} hifb_par;

typedef enum {
    HIFB_LAYER_TYPE_HD,
    HIFB_LAYER_TYPE_SD,
    HIFB_LAYER_TYPE_AD,
    HIFB_LAYER_TYPE_CURSOR,
    HIFB_LAYER_TYPE_BUTT,
} hifb_layer_type;

typedef struct {
    struct fb_bitfield red;   /* bitfield in fb mem if true color, */
    struct fb_bitfield green; /* else only length is significant */
    struct fb_bitfield blue;
    struct fb_bitfield transp; /* transparency   */
} hifb_argb_bitinfo;

typedef struct {
    hi_u32 layer_id;
    hi_bool soft_cursor_update;
    hi_bool compress; /* if this tde task is compress output */
} hifb_tde_callback_param;

bool hifb_update_rotate_rect(hi_void);
extern hi_void *hifb_buf_map(hi_u64 phy_addr, hi_u32 size);
extern hi_s32 tde_cal_scale_rect_hifb(const hi_tde_rect *src_rect, const hi_tde_rect *dst_rect,
                                      hi_tde_rect *rect_in_src, hi_tde_rect *rect_in_dst);

#endif /* __HIFB_MAIN_H__ */
