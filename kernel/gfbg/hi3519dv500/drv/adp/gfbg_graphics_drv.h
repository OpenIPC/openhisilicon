/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_GRAPHICS_DRV_H
#define GFBG_GRAPHICS_DRV_H

#include "ot_osal.h"
#include "ot_type.h"
#include "ot_common.h"
#include "gfbg_graphic_hal.h"

#define COEF_SIZE_G0ZME           (4096 * 128 / 8)
#define CLUT_TABLE_SIZE           (256 * 4)

/* debug print format */
#define gfbg_graphics_error(format, ...)                                                \
    OT_ERR_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" format, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

typedef td_s32 (*fb_intcallback)(const td_void *paraml, ot_vo_dev vo_dev,  const td_void *paramr);

#define GRAPHIC_ALPHA_OPACITY     0xff

#define GFX_SPIN_LOCK_FLAG        td_ulong
#define GFX_SPIN_LOCK_S           osal_spinlock
#define gfx_spin_lock_init(pLock) osal_spin_lock_init(pLock)
#define gfx_spin_lock_deinit(pLock) osal_spin_lock_destroy(pLock)
#define gfx_spin_lock_irqsave(pLock, flag) osal_spin_lock_irqsave(pLock, flag)
#define gfx_spin_unlock_irqrestore(pLock, flag) osal_spin_unlock_irqrestore(pLock, flag)

/* debug print format */
#define graphics_drv_error(format, ...)                                                 \
    OT_ERR_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" format, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define graphics_drv_debug(format, ...)                                                 \
    OT_DEBUG_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" format, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define graphics_drv_info(format, ...)                                                 \
    OT_INFO_TRACE(OT_ID_FB, "[Func]:%s [Line]:%d [Info]:" format, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

typedef struct {
    hal_disp_pixel_format pixel_fmt;
    td_phys_addr_t ar_phy_addr;
    td_phys_addr_t gb_phy_addr;
    td_u32 frame_size0;
    td_u32 frame_size1;
    td_u32 width;
    td_u32 height;
    td_bool is_lossless_a;
    td_bool is_lossless;
    td_u32 offset;
    td_u32 stride;
} graphic_dcmp_info;

typedef struct {
    hal_disp_layer layer_id;
    td_bool opened;
    td_bool binded;
    ot_vo_dev binded_dev;

    fb_intcallback vo_callback; /* VSYNC interrupt callback function */
    td_void *vo_callback_arg;

    ot_fb_layer_csc gfx_csc;
    csc_coef_param coef_param;
    GFX_SPIN_LOCK_S spin_lock;

    /* DDR detection area used in this layer */
    td_u32 start_section;
    td_u32 zone_nums;
} graphic_layer_context;

typedef enum {
    VDP_G0_ZME_TYP = 0,
    VDP_G0_ZME_TYP1,
    VDP_G0_ZME_RAND,
    VDP_G0_ZME_MAX,
    VDP_G0_ZME_MIN,
    VDP_G0_ZME_ZERO,
    VDP_G0_ZME_BUTT
} gf_g0_zme_mode;

#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
typedef enum {
    VDP_G1_ZME_TYP = 0,
    VDP_G1_ZME_TYP1,
    VDP_G1_ZME_RAND,
    VDP_G1_ZME_MAX,
    VDP_G1_ZME_MIN,
    VDP_G1_ZME_ZERO,
    VDP_G1_ZME_BUTT
} gf_g1_zme_mode;
#endif

typedef struct {
    td_u32 ck_gt_en;
    td_u32 in_width;
    td_u32 out_width;
    td_u32 out_pro;

    td_u32 hfir_en;
    td_u32 lhmid_en;
    td_u32 ahmid_en;
    td_u32 lhfir_mode;
    td_u32 ahfir_mode;

    td_u32 in_height;
    td_u32 out_height;

    td_u32 vfir_en;
    td_u32 lvmid_en;
    td_u32 avmid_en;
    td_u32 lvfir_mode;
    td_u32 avfir_mode;
} gf_zme_cfg;

typedef enum {
    GF_RM_COEF_MODE_TYP = 0x0,
    GF_RM_COEF_MODE_RAN = 0x1,
    GF_RM_COEF_MODE_MIN = 0x2,
    GF_RM_COEF_MODE_MAX = 0x3,
    GF_RM_COEF_MODE_ZRO = 0x4,
    GF_RM_COEF_MODE_CUS = 0x5,
    GF_RM_COEF_MODE_UP = 0x6,
    GF_RM_COEF_MODE_BUTT
} gf_rm_coef_mode;

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
typedef enum {
    GFX_DCMP_SRC_MODE_RGB888 = 0,
    GFX_DCMP_SRC_MODE_ARGB8888,
    GFX_DCMP_SRC_MODE_ARGB1555,
    GFX_DCMP_SRC_MODE_ARGB4444,
    GFX_DCMP_SRC_MODE_BUTT
} gfx_dcmp_src_mode;
#endif
#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
typedef enum {
    GFX_DCMP_SRC_MODE_ARGB8888 = 0,
    GFX_DCMP_SRC_MODE_ARGB6888,
    GFX_DCMP_SRC_MODE_ARGB1555,
    GFX_DCMP_SRC_MODE_ARGB4444,
    GFX_DCMP_SRC_MODE_BUTT
} gfx_dcmp_src_mode;
#endif

typedef struct {
    td_phys_addr_t start_phy_addr;
    td_void *start_vir_addr;
    td_u32 size;
} gfbg_mmz_buffer;

typedef enum {
    GFBG_COEF_BUF_V0_HZME = 0,
    GFBG_COEF_BUF_V0_VZME = 1,
    GFBG_COEF_BUF_V0_HDRD = 2,
    GFBG_COEF_BUF_V0_HDRS = 3,
    GFBG_COEF_BUF_G0_HDR_TMAP = 4,
    GFBG_COEF_BUF_G0_HDR_GMM = 5,
    GFBG_COEF_BUF_G0ZME = 6,
    GFBG_COEF_BUF_WD_HZME = 7,
    GFBG_COEF_BUF_WD_VZME = 8,
    GFBG_COEF_BUF_WD_HDRD = 9,
    GFBG_COEF_BUF_WD_HDRS = 10,

    GFBG_COEF_BUF_REGION_V0 = 12,
    GFBG_COEF_BUF_REGION_V1 = 13,
    GFBG_COEF_BUF_SHARPEN,
    GFBG_COEF_BUF_DCI,
    GFBG_COEF_BUF_ACM,
    GFBG_COEF_BUF_VZME,
    GFBG_COEF_BUF_HZME,
    GFBG_COEF_BUF_GPZME,
    GFBG_COEF_BUF_DIM,
    GFBG_COEF_BUF_GMM,
    GFBG_COEF_BUF_OD,
    GFBG_COEF_BUF_CLUT,
    GFBG_COEF_BUF_PCID,

    GFBG_COEF_BUF_ALL = 32,
    GFBG_COEF_BUF_BUTT = 33
} gfbg_coef_buf;

typedef struct {
    gfbg_mmz_buffer buf_base_addr;
    td_u32 size;

    td_u8 *coef_vir_addr[GFBG_COEF_BUF_BUTT];
    td_phys_addr_t coef_phy_addr[GFBG_COEF_BUF_BUTT];
} gfbg_coef_addr;

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
typedef enum {
    GFBG_DCMP_SRC_MODE_RGB888 = 0,
    GFBG_DCMP_SRC_MODE_ARGB8888,
    GFBG_DCMP_SRC_MODE_ARGB1555,
    GFBG_DCMP_SRC_MODE_ARGB4444,
    GFBG_DCMP_SRC_MODE_BUTT
} gfbg_dcmp_src_mode;
#endif
#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
typedef enum {
    GFBG_DCMP_SRC_MODE_ARGB8888 = 0,
    GFBG_DCMP_SRC_MODE_ARGB1555,
    GFBG_DCMP_SRC_MODE_ARGB4444,
    GFBG_DCMP_SRC_MODE_RGB565,
    GFBG_DCMP_SRC_MODE_RGB888,
    GFBG_DCMP_SRC_MODE_YUV444,
    GFBG_DCMP_SRC_MODE_BUTT
} gfbg_dcmp_src_mode;
#endif

typedef struct {
    td_u32 data3;
    td_u32 data2;
    td_u32 data1;
    td_u32 data0;
    td_u32 depth;
} gfbg_drv_u128;

typedef enum {
    DRV_COEF_DATA_TYPE_U8 = 0,
    DRV_COEF_DATA_TYPE_S8,
    DRV_COEF_DATA_TYPE_U16,
    DRV_COEF_DATA_TYPE_S16,
    DRV_COEF_DATA_TYPE_U32,
    DRV_COEF_DATA_TYPE_S32,
    DRV_COEF_DATA_TYPE_BUTT
} gfbg_drv_coef_data_type;

typedef struct {
    td_u8 *coef_addr;
    td_u32 lut_num;
    td_u32 burst_num;
    td_u32 cycle_num;
    void **coef_array;
    td_u32 *lut_length;
    td_u32 *coef_bit_length;
    gfbg_drv_coef_data_type data_type;
} gfbg_drv_coef_send_cfg;

typedef enum {
    GFBG_RM_COEF_MODE_TYP = 0x0,
    GFBG_RM_COEF_MODE_RAN = 0x1,
    GFBG_RM_COEF_MODE_MIN = 0x2,
    GFBG_RM_COEF_MODE_MAX = 0x3,
    GFBG_RM_COEF_MODE_ZRO = 0x4,
    GFBG_RM_COEF_MODE_CUS = 0x5,
    GFBG_RM_COEF_MODE_UP = 0x6,
    GFBG_RM_COEF_MODE_BUTT
} gfbg_rm_coef_mode;

typedef struct {
    void *p_coef;
    void *p_coef_new;
    gfbg_drv_coef_data_type coef_data_type;
    td_u32 length;
    gfbg_rm_coef_mode coef_data_mode;
    td_s32 coef_max;
    td_s32 coef_min;
} gfbg_drv_coef_gen_cfg;

td_s32 graphic_drv_enable_dcmp(hal_disp_layer layer, td_bool enable);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 graphic_drv_get_dcmp_enable_state(hal_disp_layer layer, td_bool *enable);
td_s32 graphic_drv_set_dcmp_info(hal_disp_layer layer, const graphic_dcmp_info *dcmp_info);
#endif

td_s32 fb_graphic_drv_get_layer_index(hal_disp_layer disp_layer, td_u32 *layer);

td_s32 graphic_drv_set_gfx_key_mode(hal_disp_layer layer, td_u32 key_out);
td_s32 graphic_drv_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode);
td_s32 graphic_drv_set_gfx_palpha(hal_disp_layer layer, td_u32 alpha_en, td_u32 arange, td_u8 alpha0, td_u8 alpha1);
td_s32 graphic_drv_layer_set_layer_galpha(hal_disp_layer layer, td_u8 alpha0);
td_s32 graphic_drv_layer_set_csc_en(hal_disp_layer layer, td_bool csc_en);
td_s32 graphic_drv_set_layer_addr(hal_disp_layer layer, td_phys_addr_t addr);
td_s32 graphic_drv_set_gfx_stride(hal_disp_layer layer, td_u16 pitch);
td_s32 graphic_drv_get_gfx_pre_mult(hal_disp_layer layer, td_u32 *enable);
td_s32 graphic_drv_set_gfx_pre_mult(hal_disp_layer layer, td_u32 enable);
td_s32 graphic_drv_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
td_s32 graphic_drv_set_layer_in_rect(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 graphic_drv_set_src_image_resolution(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 graphic_drv_set_layer_out_rect(hal_disp_layer layer, const ot_fb_rect *rect);
td_s32 graphic_drv_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min);
td_s32 graphic_drv_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk);
td_s32 graphic_drv_set_gfx_key_en(hal_disp_layer layer, td_u32 key_enable);
td_s32 graphic_drv_set_reg_up(hal_disp_layer layer);
#ifdef CONFIG_TDE_CLUT_RECT_SUPPORT_G0
gfbg_mmz_buffer *graphic_drv_get_clut_table_g0(td_void);
#else
gfbg_mmz_buffer *graphic_drv_get_clut_table_g3(td_void);
#endif
#ifdef CONFIG_TDE_CLUT_RECT_SUPPORT_G4
gfbg_mmz_buffer *graphic_drv_get_clut_table_g4(td_void);
#endif
gfbg_mmz_buffer *graphic_drv_get_smart_rect_memory(td_void);
td_void graphic_drv_set_clut_reg(td_u32 layer_id, td_phys_addr_t clut_phy_addr);
td_void graphic_drv_set_clut_reg_up(td_u32 layer_id);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 graphic_drv_set_smart_rect_reg(td_u32 layer_id, td_phys_addr_t phys_addr);
td_void graphic_drv_smart_rect_up_param(td_u32 layer_id);
td_void graphic_drv_smart_rect_disable(td_u32 layer_id);
#endif
td_s32 graphic_drv_get_layer_galpha(hal_disp_layer layer, td_u8 *alpha0);
td_s32 graphic_drv_get_layer_data_fmt(hal_disp_layer layer, td_u32 *fmt);
td_s32 graphic_drv_get_gfx_addr(hal_disp_layer layer, td_phys_addr_t *gfx_addr);
td_s32 graphic_drv_get_gfx_stride(hal_disp_layer layer, td_u32 *gfx_stride);
td_s32 fb_graphic_drv_get_scan_mode(ot_vo_dev vo_dev, td_bool *iop);

td_void graphic_drv_clr_wbc_mask(hal_disp_layer disp_layer);
td_void graphic_drv_clear_wbc_int_status(hal_disp_layer disp_layer);
td_s32 graphic_drv_get_vt_thd_mode(ot_vo_dev vo_dev, td_bool *feild_update);

td_s32 graphic_zme_coef_init(td_void);
td_s32 graphic_drv_get_bind_dev(td_s32 layer_id);
td_s32 graphic_drv_dev_gfbg_int_enable(td_u32 layer_id, td_bool enable);

td_s32 fb_graphic_drv_init(td_void);
td_s32 fb_graphic_drv_exit(td_void);
td_s32 graphic_drv_resource_init(td_void);
td_s32 graphic_drv_resource_exit(td_void);

td_s32 graphic_drv_set_layer_enable(hal_disp_layer gfx_layer, td_bool enable);
#ifdef CONFIG_GFBG_NOT_SUPPORT_FUNC
td_s32 graphic_drv_get_layer_enable(hal_disp_layer gfx_layer, td_bool *enable);
#endif

td_s32 fb_graphic_drv_set_csc_coef(hal_disp_layer gfx_layer, const ot_fb_layer_csc *gfx_csc,
                                   const csc_coef_param *coef_param);

td_void graphic_drv_int_clear(td_u32 int_clear, td_s32 irq);
td_u32 graphic_drv_int_get_status(td_void);
td_s32 graphic_drv_get_interrupt_dev(td_u32 int_status, ot_vo_dev *vo_dev);
td_void graphics_drv_set_tde_sync(td_bool is_sync, td_bool is_hw_mute_clr_en, td_bool is_mute_en,
    td_u32 safe_dist);
td_s32 fb_graphics_init(td_void);
td_s32 fb_graphics_deinit(td_void);

td_void gf_func_set_g0zme_mode(td_u32 layer, gf_g0_zme_mode g0_zme_mode, const gf_zme_cfg *cfg);
#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
td_void gf_func_set_g1zme_mode(td_u32 layer, gf_g1_zme_mode g1_zme_mode, const gf_zme_cfg *cfg);
#endif
#ifdef CONFIG_GFBG_ZME_SUPPORT_G0
td_void gf_drv_set_g0zme_coef(td_s16 *coef_h, td_s16 *coef_v);
td_void gf_vset_g0zme_coef(gf_rm_coef_mode coef_mode);
#endif
td_s32 graphic_drv_enable_zme(td_u32 layer, const gf_zme_cfg *zme_cfg, td_bool enable_zme);
td_void graphic_drv_get_int_state_vcnt(ot_vo_dev vo_dev, td_u32 *vcnt);

#endif
