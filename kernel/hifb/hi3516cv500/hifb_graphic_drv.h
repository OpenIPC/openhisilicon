/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */
#ifndef __HIFB_GRAPHIC_DRV_H__
#define __HIFB_GRAPHIC_DRV_H__
#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_vo.h"
#include "hifb_graphic_hal.h"
#include "hifb_vou_drv.h"
#include "hi_osal.h"

#define COEF_SIZE_G0ZME (4096 * 128 / 8)
/* debug print format */
#define hifb_graphics_trace(level, fmt...)                                                \
    do {                                                                                  \
        HI_TRACE(level, HI_ID_FB, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
        HI_TRACE(level, HI_ID_FB, ##fmt);                                                 \
    } while (0)

typedef hi_s32 (*fb_intcallback)(hi_void *, hi_void *);

#define GFX_SPIN_LOCK_S                        osal_spinlock_t
#define gfx_spin_lock_init(lock)               osal_spin_lock_init(lock)
#define gfx_spin_lock_deinit(lock)             osal_spin_lock_destory(lock)
#define gfx_spin_lock_irqsave(lock, flag)      osal_spin_lock_irqsave(lock, flag)
#define gfx_spin_unlock_irqrestore(lock, flag) osal_spin_unlock_irqrestore(lock, flag)

/* debug print format */
#define graphics_drv_trace(level, fmt...)                                                 \
    do {                                                                                  \
        HI_TRACE(level, HI_ID_FB, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
        HI_TRACE(level, HI_ID_FB, ##fmt);                                                 \
    } while (0)

typedef struct {
    hal_disp_layer layer_id;
    hi_bool opened;
    hi_bool binded;
    hifb_vo_dev binded_dev;

    fb_intcallback vo_callback; /* VSYNC interrupt callback function */
    hi_void *vo_callback_arg;

    fb_intcallback wbc_callback; /* VSYNC interrupt callback function */
    hi_void *wbc_callback_arg;

    fb_vo_csc gfx_csc;
    csc_coef_param coef_param;
    GFX_SPIN_LOCK_S spin_lock;

    /* DDR detection area used in this layer */
    hi_u32 start_section;
    hi_u32 zone_nums;
} vo_gfxlayer_context;

typedef enum {
    VDP_G0_ZME_TYP = 0,
    VDP_G0_ZME_TYP1,
    VDP_G0_ZME_RAND,
    VDP_G0_ZME_MAX,
    VDP_G0_ZME_MIN,
    VDP_G0_ZME_ZERO,
    VDP_G0_ZME_BUTT
} gf_g0_zme_mode;

typedef struct {
    hi_u32 ck_gt_en;
    hi_u32 in_width;
    hi_u32 out_width;
    hi_u32 out_pro;

    hi_u32 hfir_en;
    hi_u32 lhmid_en;
    hi_u32 ahmid_en;
    hi_u32 lhfir_mode;
    hi_u32 ahfir_mode;

    hi_u32 in_height;
    hi_u32 out_height;

    hi_u32 vfir_en;
    hi_u32 lvmid_en;
    hi_u32 avmid_en;
    hi_u32 lvfir_mode;
    hi_u32 avfir_mode;
} gf_g0_zme_cfg;

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

typedef struct {
    hi_u64 start_phy_addr;
    hi_void *start_vir_addr;
    hi_u32 size;
} hifb_mmz_buffer;

typedef enum {
    HIFB_COEF_BUF_V0_HZME = 0,
    HIFB_COEF_BUF_V0_VZME = 1,
    HIFB_COEF_BUF_V0_HDRD = 2,
    HIFB_COEF_BUF_V0_HDRS = 3,
    HIFB_COEF_BUF_G0_HDR_TMAP = 4,
    HIFB_COEF_BUF_G0_HDR_GMM = 5,
    HIFB_COEF_BUF_G0ZME = 6,
    HIFB_COEF_BUF_WD_HZME = 7,
    HIFB_COEF_BUF_WD_VZME = 8,
    HIFB_COEF_BUF_WD_HDRD = 9,
    HIFB_COEF_BUF_WD_HDRS = 10,

    HIFB_COEF_BUF_REGION_V0 = 12,
    HIFB_COEF_BUF_REGION_V1 = 13,
    HIFB_COEF_BUF_SHARPEN,
    HIFB_COEF_BUF_DCI,
    HIFB_COEF_BUF_ACM,
    HIFB_COEF_BUF_VZME,
    HIFB_COEF_BUF_HZME,
    HIFB_COEF_BUF_GPZME,
    HIFB_COEF_BUF_DIM,
    HIFB_COEF_BUF_GMM,
    HIFB_COEF_BUF_OD,
    HIFB_COEF_BUF_CLUT,
    HIFB_COEF_BUF_PCID,

    HIFB_COEF_BUF_ALL = 32,
    HIFB_COEF_BUF_BUTT = 33
} hifb_coef_buf;

typedef struct {
    hifb_mmz_buffer buf_base_addr;
    hi_u32 size;

    hi_u8 *coef_vir_addr[HIFB_COEF_BUF_BUTT];
    hi_u64 coef_phy_addr[HIFB_COEF_BUF_BUTT];
} hifb_coef_addr;

typedef struct {
    hi_u32 data3;
    hi_u32 data2;
    hi_u32 data1;
    hi_u32 data0;
    hi_u32 depth;
} hifb_drv_u128;

typedef enum {
    DRV_COEF_DATA_TYPE_U8 = 0,
    DRV_COEF_DATA_TYPE_S8,
    DRV_COEF_DATA_TYPE_U16,
    DRV_COEF_DATA_TYPE_S16,
    DRV_COEF_DATA_TYPE_U32,
    DRV_COEF_DATA_TYPE_S32,
    DRV_COEF_DATA_TYPE_BUTT
} hifb_drv_coef_data_type;

typedef struct {
    hi_u8 *coef_addr;
    hi_u32 lut_num;
    hi_u32 burst_num;
    hi_u32 cycle_num;
    void **coef_array;
    hi_u32 *lut_length;
    hi_u32 *coef_bit_length;
    hifb_drv_coef_data_type data_type;
} hifb_drv_coef_send_cfg;

hi_s32 graphic_drv_get_interrupt_dev(hi_u32 int_status, hifb_vo_dev *vo_dev);
hi_void graphic_drv_clr_int_status(hi_u32 int_status);
hi_u32 graphic_drv_int_get_status(hi_void);
hi_void graphic_drv_int_clear(hi_u32 int_clear, hi_s32 irq);
hi_void graphic_drv_dev_int_enable(hifb_vo_dev vo_dev, hi_bool enable);
hi_bool graphic_drv_enable_zme(hi_u32 layer, gf_g0_zme_cfg *zme_cfg, hi_bool enable_zme);
hi_void gf_vset_g0zme_coef(gf_rm_coef_mode coef_mode);
hi_void gf_drv_set_g0zme_coef(HI_S16 *coef_h, HI_S16 *coef_v);
hi_void gf_func_set_g0zme_mode(hi_u32 layer, gf_g0_zme_mode g0zme_mode, gf_g0_zme_cfg *cfg);
hi_s32 fb_graphic_drv_show_proc(osal_proc_entry_t *s);
hi_s32 fb_graphic_drv_set_csc_coef(hal_disp_layer gfx_layer, fb_vo_csc *gfx_csc, csc_coef_param *csc_coef_param);
hi_s32 graphic_drv_enable_layer(hal_disp_layer gfx_layer, hi_bool enabled);
hi_s32 graphic_drv_resource_exit(hi_void);
hi_s32 graphic_drv_resource_init(hi_void);
hi_s32 fb_graphic_drv_exit(hi_void);
hi_s32 graphic_drv_get_bind_dev(hi_s32 layer_id);
hi_s32 fb_graphic_drv_init(hi_void);
hi_s32 graphic_zme_coef_init(hi_void);
hi_bool graphic_drv_get_intf_mux_sel(hifb_vo_dev vo_dev, VO_INTF_TYPE_E *ben_intf_type);
hi_bool graphic_drv_get_intf_sync(hifb_vo_dev vo_dev, hal_disp_syncinfo *sync_info);
hi_bool graphic_drv_get_dev_enable(hifb_vo_dev vo_dev, hi_bool *intf_en);
hi_bool fb_graphic_drv_get_int_state(hal_disp_layer gfx_layer, hi_bool *bottom);
hi_bool graphic_drv_get_vt_thd_mode(hifb_vo_dev vo_dev, hi_bool *feild_update);
hi_bool graphic_drv_get_dcmp_enable_state(hal_disp_layer layer, hi_bool *enable);
hi_bool graphic_drv_enable_dcmp(hal_disp_layer layer, hi_bool enable);
hi_bool fb_graphic_drv_get_scan_mode(hifb_vo_dev vo_dev, hi_bool *iop);
hi_bool graphic_drv_get_gfx_stride(hal_disp_layer layer, hi_u32 *gfx_stride);
hi_bool graphic_drv_get_gfx_addr(hal_disp_layer layer, hi_u64 *gfx_addr);
hi_bool graphic_drv_get_layer_data_fmt(hal_disp_layer layer, hi_u32 *fmt);
hi_bool graphic_drv_get_layer_galpha(hal_disp_layer layer, hi_u8 *alpha0);
hi_bool graphic_drv_set_reg_up(hal_disp_layer layer);
hi_bool graphic_drv_set_gfx_key_en(hal_disp_layer layer, hi_u32 key_enable);
hi_bool graphic_drv_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk);
hi_bool graphic_drv_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max, hal_gfx_key_min key_min);
hi_bool graphic_drv_set_layer_out_rect(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool graphic_drv_set_src_image_resolution(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool graphic_drv_set_layer_in_rect(hal_disp_layer layer, HIFB_RECT *rect);
hi_bool graphic_drv_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
hi_bool graphic_drv_set_gfx_pre_mult(hal_disp_layer layer, hi_u32 enable);
hi_bool graphic_drv_get_gfx_pre_mult(hal_disp_layer layer, hi_u32 *enable);
hi_bool graphic_drv_set_gfx_stride(hal_disp_layer layer, HI_U16 pitch);
hi_bool graphic_drv_set_layer_addr(hal_disp_layer layer_id, hi_u64 addr);
hi_bool graphic_drv_layer_set_csc_en(hal_disp_layer layer, hi_bool csc_en);
hi_bool graphic_drv_layer_set_layer_galpha(hal_disp_layer layer, hi_u8 alpha0);
hi_bool graphic_drv_set_gfx_palpha(hal_disp_layer layer, hi_u32 alpha_en, hi_u32 arange, hi_u8 alpha0,
                                   hi_u8 alpha1);
hi_bool graphic_drv_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode);
hi_bool graphic_drv_set_gfx_key_mode(hal_disp_layer layer, hi_u32 key_out);
hi_s32 fb_graphic_drv_get_layer_id(hi_u32 layer, hal_disp_layer *disp_layer);
hi_s32 fb_graphic_drv_get_layer_index(hal_disp_layer disp_layer, hi_u32 *layer);

#endif /* __HIFB_GRAPHIC_DRV_H__ */
