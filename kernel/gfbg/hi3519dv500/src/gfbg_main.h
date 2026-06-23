/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef GFBG_MAIN_H
#define GFBG_MAIN_H
#include "gfbg_drv.h"

/* define the value of default set of each layer */
#define GFBG_4K_DEF_WIDTH 3840
#define GFBG_4K_DEF_HEIGHT 2160
#define GFBG_4K_DEF_STRIDE (GFBG_4K_DEF_WIDTH * 4)
#define GFBG_4K_DEF_VRAM 64800 /* unit:KB 3840 * 2160 * 4 * 2 */

#define GFBG_HD_DEF_WIDTH 1920 /* unit: pixel */
#define GFBG_HD_DEF_HEIGHT 1080 /* unit: pixel */
#define GFBG_HD_DEF_STRIDE (GFBG_HD_DEF_WIDTH * 4) /* unit: byte */
#define GFBG_HD_DEF_VRAM 16200 /* unit:KB 1280*720*4*2 */ /* unit:KB 1920*1080*4*2 */

#define GFBG_SD_DEF_WIDTH 720
#define GFBG_SD_DEF_HEIGHT 576
#define GFBG_SD_DEF_STRIDE 1440
#define GFBG_SD_DEF_VRAM 3240 /* unit:KB 720*576*4 */

#define GFBG_AD_DEF_WIDTH 720
#define GFBG_AD_DEF_HEIGHT 576
#define GFBG_AD_DEF_STRIDE 1440
#define GFBG_AD_DEF_VRAM 3240 /* unit:KB 720*576*4 */

#define GFBG_CURSOR_DEF_WIDTH 256
#define GFBG_CURSOR_DEF_HEIGHT 256
#define GFBG_CURSOR_DEF_STRIDE 512
#define GFBG_CURSOR_DEF_VRAM 512 /* unit:KB for doudble buffer mode, we need 2 memory buffer, save cursor */

#define GFBG_SOFTCURSOR_STRIDE 512

#define GFBG_DEF_DEPTH 16 /* unit: bits */
#define GFBG_DEF_XSTART 0
#define GFBG_DEF_YSTART 0
#define GFBG_DEF_ALPHA 0xff

#define GFBG_MAX_ZOOMIN 15

#define GFBG_TDE_MEMSET_WIDTH 3840
#define GFBG_TDE_MEMSET_HEIGHT 2160

#define SMART_RECT_SIZE (128 * 16)

#define GFBG_CMAP_SIZE 0 /* unit:KB 256*4 */
#define MMB_ADDR_INVALID (0)

#define gfbg_is_clutfmt(eFmt) (OT_FB_FORMAT_1BPP <= (eFmt) && (eFmt) <= OT_FB_FORMAT_ACLUT88)
#define GFBG_ALPHA_OPAQUE 0xff
#define GFBG_ALPHA_TRANSPARENT 0x00

#define gfbg_min(m, n) (((m) > (n)) ? (n) : (m))

#define GFBG_DEFLICKER_LEVEL_MAX 5 /* support level 5 deflicker most */

#define GFBG_MAX_CURSOR_WIDTH 128
#define GFBG_MAX_CURSOR_HEIGHT 128

#define GFBG_MIN_LOW_DELAY_WIDTH 1920
#define GFBG_MIN_LOW_DELAY_HEIGHT 1080

#define GFBG_MAX_LAYER_WIDTH_CLUT_G3 3840
#define GFBG_MAX_LAYER_HEIGTH_CLUT_G3 2160

typedef enum {
    GFBG_ANTIFLICKER_NONE, /* no antiflicker.If scan mode is progressive, gfbg will set antiflicker mode to none */
    GFBG_ANTIFLICKER_TDE,  /* tde antiflicker mode, it's effect for 1buf or 2buf only */
    GFBG_ANTIFLICKER_VO,   /* vo antiflicker mode, need hardware support  */
    GFBG_ANTIFLICKER_BUTT
} gfbg_layer_antiflicker_mode;

typedef struct {
    ot_fb_point pos;
    td_u32 display_width;
    td_u32 display_height;
    td_u32 screen_width;
    td_u32 screen_height;
    td_bool is_premul;
    td_bool need_antiflicker;
    ot_fb_layer_antiflicker_level antiflicker_level; /* antiflicker level */
    gfbg_layer_antiflicker_mode antiflicker_mode;  /* antiflicker mode */
    td_u32 vir_x_res;
    td_u32 vir_y_res;
    td_u32 x_res;
    td_u32 y_res;
    td_u32 max_screen_width;
    td_u32 max_screen_height;
    ot_fb_mirror_mode mirror_mode;
    ot_fb_rotate_mode rotate_mode;
} gfbg_display_info;

typedef struct {
    td_char *vir_addr[2]; /* 2 for y or uv vir address */
    td_phys_addr_t phys_addr[2]; /* 2 for y or uv address */
    td_u32 stride;       /* buf stride */
    td_bool need_flip;
    td_bool fliped;
    td_u32 index_for_int;
    td_u32 int_pic_num;
    ot_fb_rect union_rect;
    td_s32 refresh_handle;
    td_bool compress; /* Identifies whether the frame to be displayed is compressed */
} gfbg_dispbuf_info;

typedef struct {
    ot_fb_layer_buf buf_mode; /* buffer mode */
    ot_fb_buf user_buffer;
    td_phys_addr_t screen_addr;    /* screen buf addr */
    td_phys_addr_t gb_screen_addr; /* new GB screen buf addr */
    gfbg_dispbuf_info disp_buf_info;
    td_u32 refresh_num; /* refresh request num in 2 buf mode */
    td_bool do_refresh_job;
} gfbg_refresh_info;

typedef struct {
    /* For cursor layer, cursor means cursor buffer, it is allocated and freed
     both by user;for general layer,cursor means back buf */
    ot_fb_cursor cursor;

    /* For cursor layer,you can quary whether cursor attach to a certain layer
     for general layer, you can quary whether cursor attach to it */
    td_u32 attached;

    /* valid area:overlap region between attached layer and cursor layer */
    ot_fb_rect rect_in_disp_buf;

    /* the original position of cursor, usually is (0,0) but also has different when at margin */
    ot_fb_point pos_in_cursor;

    td_u32 attached_cursor_id;
} gfbg_cursor_info;

typedef struct {
    td_bool is_economize_memory;
    td_bool compress_open;
    td_bool is_losslessa;
    td_bool is_lossless;
    td_bool decompress_open;
    td_bool update_finished;

    ot_fb_rect update_rect;

    ot_fb_rect compress_rect;

    td_bool delay;

    td_phys_addr_t ar_buf_phy_addr;
    td_phys_addr_t gb_buf_phy_addr;
    td_u32 frame_size0;
    td_u32 frame_size1;
    td_u32 header_size;
    td_u32 stride;

    td_bool layer_addr_update;

    td_u32 start_section;
    td_u32 zone_nums;
    td_bool clear_zone;

    td_u32 new_start_section;
    td_u32 new_zone_nums;
} gfbg_compress_info;

typedef struct {
    td_u32 layer_id;                 /* layer id */
    atomic_t ref_count;              /* framebuffer reference count */
    td_bool show;                    /* show status */
    ot_fb_color_format color_format; /* color format */
    ot_fb_alpha alpha;               /* alpha attribution */
    gfbg_colorkeyex ckey;            /* colorkey attribution */
    gfbg_display_info display_info;  /* display info */
    gfbg_refresh_info refresh_info;
    gfbg_cursor_info cursor_info;
    volatile gfbg_compress_info compress_info;
    td_u32 param_modify_mask;
    volatile td_bool modifying;
    ot_fb_surface canvas_sur;                        /* canvas surface */
    td_u32 h_dflevel;                                /* horizontal deflicker level */
    td_u32 v_dflevel;                                /* vertical deflicker level */
    unsigned char h_dfcoef[GFBG_DEFLICKER_LEVEL_MAX - 1]; /* horizontal deflicker coefficients */
    unsigned char v_dfcoef[GFBG_DEFLICKER_LEVEL_MAX - 1]; /* vertical deflicker coefficients */
    volatile td_u32 vblflag;
    wait_queue_head_t vbl_event;
    wait_queue_head_t do_refresh_job;
    osal_spinlock lock;
    td_bool layer_open;
    td_phys_addr_t rotate_vb;
} gfbg_par;

typedef enum {
    GFBG_LAYER_TYPE_4K,
    GFBG_LAYER_TYPE_HD,
    GFBG_LAYER_TYPE_SD,
    GFBG_LAYER_TYPE_AD,
    GFBG_LAYER_TYPE_CURSOR,
    GFBG_LAYER_TYPE_BUTT,
} gfbg_layer_type;

typedef struct {
    struct fb_bitfield red;   /* bitfield in fb mem if true color, */
    struct fb_bitfield green; /* else only length is significant */
    struct fb_bitfield blue;
    struct fb_bitfield transp; /* transparency   */
} gfbg_argb_bitinfo;

typedef struct {
    td_u32 layer_id;
    td_bool soft_cursor_update;
    td_bool compress; /* if this tde task is compress output */
} gfbg_tde_callback_param;

typedef struct {
    unsigned regno;
    unsigned red;
    unsigned green;
    unsigned blue;
    unsigned transp;
} gfbg_cmp_reg;

td_s32 graphic_drv_vdp_state_count_lock_init(td_void);
td_void graphic_drv_vdp_state_count_lock_deinit(td_void);
#endif
