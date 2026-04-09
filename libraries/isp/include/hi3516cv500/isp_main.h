/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_main.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_MAIN_H__
#define __ISP_MAIN_H__

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <pthread.h>

#include "hi_comm_3a_adapt.h"
#include "isp_debug.h"
#include "mkp_isp.h"
#include "isp_dcfinfo.h"
#include "isp_frameinfo.h"
#include "isp_dnginfo.h"
#include "isp_block.h"
#include "vi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_MAX_ALGS_NUM     64

typedef struct {
    hi_bool used;
    hi_isp_alg_lib alg_lib;
    union {
        hi_isp_ae_register  ae_regsiter;
        hi_isp_awb_register awb_regsiter;
    };
} isp_lib_node;

typedef struct {
    hi_u32       active_lib; /* the lib used present. */
    isp_lib_node libs[MAX_REGISTER_ALG_LIB_NUM];
} isp_lib_info;

typedef struct {
    hi_s32 (*pfn_alg_init)(hi_vi_pipe vi_pipe, hi_void *reg_cfg);
    hi_s32 (*pfn_alg_run)(hi_vi_pipe vi_pipe,
                          const hi_void *stat_info,
                          hi_void *reg_cfg,
                          hi_s32 rsv);
    hi_s32 (*pfn_alg_ctrl)(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value);
    hi_s32 (*pfn_alg_exit)(hi_vi_pipe vi_pipe);
} isp_alg_func;

typedef struct {
    hi_void (*pfn_alg_register)(hi_vi_pipe vi_pipe);
} hi_isp_alg_exp_func;

typedef struct {
    hi_bool         used;
    hi_isp_alg_mod  alg_type;
    isp_alg_func    alg_func;
} isp_alg_node;

#define ISP_SYNC_ISO_BUF_MAX 6

typedef struct {
    hi_u16 high_ccm[CCM_MATRIX_SIZE];
    hi_u16 low_ccm[CCM_MATRIX_SIZE];
    hi_u16 high_color_temp;
    hi_u16 low_color_temp;
} isp_dng_ccm;

typedef struct {
    hi_bool defect_pixel;  /* each alg need to check this status */

    hi_u32  isp_dgain;
    hi_u32  again;
    hi_u32  dgain;
    hi_u32  isp_dgain_shift;
    hi_u32  iso;
    hi_u32  iso_max;
    hi_u32  sensor_iso;
    hi_u32  exp_ratio;
    hi_u32  exp_ratio_lut[WDR_MAX_FRAME_NUM - 1];
    hi_u32  wdr_gain[WDR_MAX_FRAME_NUM];
    hi_u32  int_time;
    hi_u32  piris_gain;
    hi_u8   ae_run_interval;
    hi_isp_fswdr_mode fswdr_mode;
    hi_isp_fswdr_mode pre_fswdr_mode;
    hi_u32  color_temp;
    hi_u32  white_balance_gain[ISP_BAYER_CHN_NUM];
    hi_u16  ccm[CCM_MATRIX_SIZE];
    hi_u8   saturation;
    hi_bool snap_state;
    hi_bool pro_trigger;
    hi_bool load_ccm;
    hi_snap_type snap_type;
    isp_snap_pipe_mode snap_pipe_mode;
    hi_s32  preview_pipe_id;
    hi_s32  picture_pipe_id;
    isp_running_mode preview_running_mode;
    isp_running_mode picture_running_mode;
    hi_u32  update_pos;
    hi_u32  sync_color_temp[ISP_SYNC_ISO_BUF_MAX];
    hi_u32  sync_ccm[ISP_SYNC_ISO_BUF_MAX][CCM_MATRIX_SIZE];
    hi_u32  sync_wb_gain[ISP_SYNC_ISO_BUF_MAX][ISP_BAYER_CHN_NUM];
    hi_u32  sync_iso_buf[ISP_SYNC_ISO_BUF_MAX];
    hi_u32  sync_isp_dgain_buf[ISP_SYNC_ISO_BUF_MAX];
    hi_u32  sync_all_exp_ratio_buf[ISP_SYNC_ISO_BUF_MAX];
    hi_u32  sync_exp_ratio_buf[WDR_MAX_FRAME_NUM - 1][ISP_SYNC_ISO_BUF_MAX];
    hi_u32  pro_index_buf[ISP_SYNC_ISO_BUF_MAX];
    hi_u8   pro_index;
    hi_u32  iso_done_frm_cnt;
    hi_u8   cfg2valid_delay_max;
    hi_bool flicker;
    hi_u8   freq_result;
    hi_bool stat_ready;
    hi_bool run_once;
} isp_linkage;

typedef struct {
    hi_bool wdr_cfg;
    hi_bool pub_cfg;
    hi_bool init;
    hi_bool run;
    hi_bool stitch_sync;

    hi_bool run_en;
} isp_para_rec;

typedef struct {
    hi_bool be_on_stt_update[ISP_MAX_BE_NUM];
    hi_bool fe_stt_update;
} isp_special_opt;

typedef struct {
    hi_u16   width;
    hi_u16   height;
    hi_float fps;
    hi_u8    sns_mode;
} isp_sensor_image_mode;

typedef struct {
    hi_u16 out_width;
    hi_u16 out_height;
} isp_bas_out_image_mode;

typedef struct {
    hi_bool                  enable_be_raw;
    hi_bool                  pre_enabe_be_raw;
    hi_u32                     be_ctrl0;
    hi_u32                     be_ctrl1;
    hi_isp_raw_pos         isp_raw_pos;
} isp_be_raw_info;

typedef union {
    hi_u64 key;
    struct {
        hi_u64  bit1_ae          : 1 ;   /* [0] */
        hi_u64  bit1_awb         : 1 ;   /* [1] */
        hi_u64  bit1_af          : 1 ;   /* [2] */
        hi_u64  bit1_dp          : 1 ;   /* [3] */
        hi_u64  bit1_ge          : 1 ;   /* [4] */
        hi_u64  bit1_wdr         : 1 ;   /* [5] */
        hi_u64  bit1_expander    : 1 ;   /* [6] */
        hi_u64  bit1_blc         : 1 ;   /* [7] */
        hi_u64  bit1_bayernr     : 1 ;   /* [8] */
        hi_u64  bit1_split       : 1 ;   /* [9] */
        hi_u64  bit1_flicker     : 1 ;   /* [10] */
        hi_u64  bit1_dg          : 1 ;   /* [11] */
        hi_u64  bit1_hrs         : 1 ;   /* [12] */
        hi_u64  bit1_fe_lsc      : 1 ;   /* [13] */
        hi_u64  bit1_lsc         : 1 ;   /* [14] */
        hi_u64  bit1_r_lsc       : 1 ;   /* [15] */
        hi_u64  bit1_rc          : 1 ;   /* [16] */
        hi_u64  bit1_drc         : 1 ;   /* [17] */
        hi_u64  bit1_dehaze      : 1 ;   /* [18] */
        hi_u64  bit1_lcac        : 1 ;   /* [19] */
        hi_u64  bit1_gcac        : 1 ;   /* [20] */
        hi_u64  bit1_demosaic    : 1 ;   /* [21] */
        hi_u64  bit1_fcr         : 1 ;   /* [22] */
        hi_u64  bit1_gamma       : 1 ;   /* [23] */
        hi_u64  bit1_csc         : 1 ;   /* [24] */
        hi_u64  bit1_ca          : 1 ;   /* [25] */
        hi_u64  bit1_fpn         : 1;    /* [26] */
        hi_u64  bit1_sharpen     : 1;    /* [27] */
        hi_u64  bit1_edge_mark   : 1 ;   /* [28] */
        hi_u64  bit1_mcds        : 1 ;   /* [29] */
        hi_u64  bit1_ldci        : 1 ;   /* [30] */
        hi_u64  bit1_pre_gamma   : 1 ;   /* [31] */
        hi_u64  bit1_fe_log_lut  : 1 ;   /* [32] */
        hi_u64  bit1_log_lut     : 1 ;   /* [33] */
        hi_u64  bit1_clut        : 1 ;   /* [34] */
        hi_u64  bit1_hlc         : 1 ;   /* [35] */
        hi_u64  bit1_detail      : 1 ;   /* [36] */
        hi_u64  bit1_rgbir       : 1 ;   /* [37] */
        hi_u64  bit1_acs         : 1 ;   /* [38] */
        hi_u64  bit25_rsv        : 25;   /* [39:63] */
    };
} isp_alg_key;

typedef struct {
    /* 1. ctrl param */
    hi_bool                  mem_init;
    hi_bool                  sns_reg;
    hi_bool                  isp_yuv_mode;
    isp_para_rec             para_rec;
    hi_rect                  sys_rect;
    isp_block_attr           block_attr;
    pthread_mutex_t          lock;
    isp_special_opt          special_opt;
    isp_be_raw_info          be_raw_info;
    hi_u8                    be_buf_num;
    /* 2. algorithm ctrl param */
    hi_bool                  freeze_fw;
    hi_bool                  change_wdr_mode;
    hi_bool                  change_image_mode;
    hi_bool                  change_isp_res;
    hi_bool                  isp0_p2_en;
    hi_bool                  ldci_tpr_flt_en;
    hi_u8                    pre_sns_wdr_mode;
    hi_u8                    sns_wdr_mode;
    hi_u32                   frame_cnt;    /* frame count for 3a libs which can decide the interval of calculate. */
    hi_isp_bayer_format      bayer;
    isp_alg_key              alg_key;

    hi_u8                    isp_image_mode_flag;
    isp_sensor_image_mode    pre_sns_image_mode;
    isp_sensor_image_mode    sns_image_mode;

    hi_isp_ae_result         ae_result;
    hi_isp_awb_result        awb_result;

    isp_bas_out_image_mode   pre_bas_image_mode;
    isp_bas_out_image_mode   cur_bas_image_mode;
    hi_size                  pipe_size;
    vi_pipe_hdr_attr         hdr_attr;
    vi_pipe_wdr_attr         wdr_attr;
    vi_stitch_attr           stitch_attr;
    hi_color_gamut           color_gamut;
    hi_isp_smart_info        smart_info;

    /* 3. 3a register info */
    hi_isp_bind_attr         bind_attr;
    isp_lib_info             ae_lib_info;
    isp_lib_info             awb_lib_info;
    isp_lib_info             af_lib_info;

    /* 4. algorithm ctrl */
    isp_alg_node             algs[ISP_MAX_ALGS_NUM];
    hi_isp_alg_exp_func      isp_alg_exp;
    isp_linkage              linkage;

    /* 5. debug info */
    isp_dbg_ctrl             isp_dbg;

    /* 6. DCF info */
    isp_trans_info_buf       isp_trans_info;
    isp_pro_info_buf         isp_pro_info;

    isp_update_info_ctrl     update_info_ctrl;
    isp_frame_info_ctrl      frame_info_ctrl;
    isp_attach_info_ctrl     attach_info_ctrl;
    isp_gammut_info_ctrl     gamut_info_ctrl;
    isp_pro_nr_param_ctrl    pro_nr_param_ctrl;
    isp_pro_shp_param_ctrl   pro_shp_param_ctrl;
    hi_isp_dcf_update_info   dcf_update_info;
    hi_isp_frame_info        frame_info;

    /* DNG info */
    isp_dng_info_ctrl        dng_info_ctrl;
    hi_isp_dng_color_param   pre_dng_color_param;
    isp_dng_ccm              pre_dng_ccm;

    isp_pro_param            pro_param;
    hi_u8                    pro_frm_cnt;
    isp_snap_info            snap_isp_info;

    pq_ai_attr              ai_info;

    /* 7. vi_proc/BE addr */
    hi_void                 *isp_be_virt_addr[ISP_STRIPING_MAX_NUM];
    hi_void                 *viproc_virt_addr[ISP_STRIPING_MAX_NUM];
} isp_usr_ctx;

#define isp_regcfg_get_ctx(dev, ctx) ((ctx) = isp_get_regcfg_ctx(dev))

#define isp_check_open_return(dev)                   \
    do {                                      \
        hi_s32 ret = isp_check_dev_open(dev); \
        if (ret != HI_SUCCESS) {              \
            return ret;                       \
        }                                     \
    } while (0)

#define isp_check_mem_init_return(dev)                \
    do {                                       \
        hi_s32 ret = isp_check_mem_init_func(dev);  \
        if (ret != HI_SUCCESS) {               \
            return ret;                        \
        }                                      \
    } while (0)

#define isp_get_ctx(dev, isp_ctx)    \
    do {                               \
        (isp_ctx) = isp_get_usr_ctx(dev); \
    } while (0)

#define isp_check_offline_mode_return(dev)                                             \
        do {                                                                        \
            isp_usr_ctx *isp_ctx_tmp = HI_NULL;                                         \
            isp_get_ctx(dev, isp_ctx_tmp);                                              \
            if ((is_online_mode(isp_ctx_tmp->block_attr.running_mode)) ||               \
                (is_sidebyside_mode(isp_ctx_tmp->block_attr.running_mode))) {           \
                return HI_SUCCESS;                                                  \
            }                                                                       \
        } while (0)

#define isp_check_wdr_main_pipe_return(wdr_mode, mast_pipe)                    \
    do {                                                                \
        if (is_wdr_mode(wdr_mode) && ((mast_pipe) != HI_TRUE)) {        \
            return HI_SUCCESS;                                          \
        }                                                               \
    } while (0)

/*
 * HIGH LEVEL functions used to set-up and run ISP.
 */
/*
 * this function executes the auto-exposure algorithm
 * and its results are used to drive sensor parameters,
 * drc engine and noise reduction algorithms,
 * results are based on the collected image histogram data.
 */
hi_s32 isp_run(hi_vi_pipe vi_pipe);
hi_s32 isp_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_stitch_sync_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_switch_wdr_mode(hi_vi_pipe vi_pipe);
hi_s32 isp_switch_image_mode(hi_vi_pipe vi_pipe);
hi_s32 isp_switch_mode(hi_vi_pipe vi_pipe);

hi_s32 isp_wdr_cfg_set(hi_vi_pipe vi_pipe);
hi_u32 isp_frame_cnt_get(hi_vi_pipe vi_pipe);

hi_u16 isp_get_max_width(hi_vi_pipe vi_pipe);
hi_u16 isp_get_max_height(hi_vi_pipe vi_pipe);

hi_s32 isp_check_dev_open(hi_isp_dev dev);
hi_s32 isp_check_mem_init_func(hi_isp_dev dev);
hi_bool isp_check_hrs_on(hi_vi_pipe vi_pipe);
hi_s32 isp_get_fd(hi_vi_pipe vi_pipe);
isp_usr_ctx *isp_get_usr_ctx(hi_vi_pipe vi_pipe);
isp_reg_cfg_attr *isp_get_regcfg_ctx(hi_vi_pipe vi_pipe);

hi_s32 isp_mem_info_set(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);
hi_s32 isp_check_sns_register(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);
hi_s32 isp_mem_init_update_ctx(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);

hi_s32 isp_init(hi_vi_pipe vi_pipe);

hi_s32 isp_alg_buf_init(hi_vi_pipe vi_pipe);
hi_void isp_alg_buf_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_run_thread_proc(hi_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_MAIN_H__ */
