/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_MAIN_H
#define ISP_MAIN_H

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <pthread.h>

#include "ot_common_3a.h"
#include "isp_debug.h"
#include "mkp_isp.h"
#include "isp_dcfinfo.h"
#include "isp_frameinfo.h"
#include "isp_dnginfo.h"
#include "vi_ext.h"
#include "isp_ext.h"
#include "securec.h"
#include "ot_common_video.h"
#ifdef CONFIG_OT_ISP_DUMP_DEBUG_SUPPORT
#include "ot_mpi_isp_dbg_inner.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_MAX_ALGS_NUM     64

typedef struct {
    td_bool used;
    ot_isp_3a_alg_lib alg_lib;
    union {
        ot_isp_ae_register  ae_regsiter;
        ot_isp_awb_register awb_regsiter;
    };
} isp_lib_node;

typedef struct {
    td_u32       active_lib; /* the lib used present. */
    isp_lib_node libs[OT_ISP_MAX_REGISTER_ALG_LIB_NUM];
} isp_lib_info;

typedef struct {
    td_s32 (*pfn_alg_init)(ot_vi_pipe vi_pipe, td_void *reg_cfg);
    td_s32 (*pfn_alg_run)(ot_vi_pipe vi_pipe,
                          const td_void *stat_info,
                          td_void *reg_cfg,
                          td_s32 rsv);
    td_s32 (*pfn_alg_ctrl)(ot_vi_pipe vi_pipe, td_u32 cmd, td_void *value);
    td_s32 (*pfn_alg_exit)(ot_vi_pipe vi_pipe);
} isp_alg_func;

typedef struct {
    td_void (*pfn_alg_register)(ot_vi_pipe vi_pipe);
} ot_isp_alg_exp_func;

typedef struct {
    td_bool         used;
    ot_isp_alg_mod  alg_type;
    isp_alg_func    alg_func;
} isp_alg_node;

#define ISP_SYNC_ISO_BUF_MAX 6

typedef struct {
    td_u16 high_ccm[OT_ISP_CCM_MATRIX_SIZE];
    td_u16 low_ccm[OT_ISP_CCM_MATRIX_SIZE];
    td_u16 high_color_temp;
    td_u16 low_color_temp;
} isp_dng_ccm;

typedef struct {
    td_bool enable;
    td_u32 blc;
} isp_blc_fix;

typedef struct {
    td_bool defect_pixel;  /* each alg need to check this status */

    td_u32  isp_dgain;
    td_u32  again;
    td_u32  dgain;
    td_u32  isp_dgain_shift;
    td_u32  iso;
    td_u32  iso_max;
    td_u32  sensor_iso;
    td_u32  exp_ratio;
    td_u32  exp_ratio_lut[OT_ISP_WDR_MAX_FRAME_NUM - 1];
    td_u32  wdr_gain[OT_ISP_WDR_MAX_FRAME_NUM];
    td_u32  int_time;
    td_u32  piris_gain;
    td_u8   ae_run_interval;
    ot_isp_fswdr_mode fswdr_mode;
    ot_isp_fswdr_mode pre_fswdr_mode;
    td_u32  color_temp;
    td_u32  white_balance_gain[OT_ISP_BAYER_CHN_NUM];
    td_u16  ccm[OT_ISP_CCM_MATRIX_SIZE];
    td_u8   saturation;
    td_bool snap_state;
#ifdef CONFIG_OT_SNAP_SUPPORT
    td_bool pro_trigger;
    td_bool load_ccm;
    ot_snap_type snap_type;
    isp_snap_pipe_mode snap_pipe_mode;
    td_s32  preview_pipe_id;
    td_s32  picture_pipe_id;
    isp_running_mode preview_running_mode;
    isp_running_mode picture_running_mode;
#endif
    td_u32  update_pos;
    td_u32  sync_color_temp[ISP_SYNC_ISO_BUF_MAX];
    td_u32  sync_ccm[ISP_SYNC_ISO_BUF_MAX][OT_ISP_CCM_MATRIX_SIZE];
    td_u32  sync_wb_gain[ISP_SYNC_ISO_BUF_MAX][OT_ISP_BAYER_CHN_NUM];
    td_u32  sync_iso_buf[ISP_SYNC_ISO_BUF_MAX];
    td_u32  sync_isp_dgain_buf[ISP_SYNC_ISO_BUF_MAX];
    td_u32  sync_all_exp_ratio_buf[ISP_SYNC_ISO_BUF_MAX];
    td_u32  sync_exp_ratio_buf[OT_ISP_WDR_MAX_FRAME_NUM - 1][ISP_SYNC_ISO_BUF_MAX];
    td_u32  pro_index_buf[ISP_SYNC_ISO_BUF_MAX];
    td_u8   pro_index;
    td_u32  iso_done_frm_cnt;
    td_u8   cfg2valid_delay_max;
    td_bool flicker;
    td_u8   freq_result;
    td_bool stat_ready;
    td_bool run_once;
    isp_blc_fix blc_fix;
} isp_linkage;

typedef enum {
    ISP_RUN_THREAD_STATUS_EXIT = 0,
    ISP_RUN_THREAD_STATUS_RUN,
    ISP_RUN_THREAD_STATUS_STOP,
    ISP_RUN_THREAD_STATUS_INIT,
    ISP_RUN_THREAD_STATUS_BUTT
} isp_run_thread_status;

typedef struct {
    td_bool wdr_cfg;
    td_bool pub_cfg;
    td_bool init;
    td_bool stitch_sync;
} isp_para_rec;


typedef struct {
    td_bool be_on_stt_update[ISP_MAX_BE_NUM];
    td_bool fe_stt_update;
} isp_special_opt;

typedef struct {
    td_u16   width;
    td_u16   height;
    td_float fps;
    td_u8    sns_mode;
} isp_sensor_image_mode;

typedef union {
    td_u64 key;
    struct {
        td_u64  bit1_ae          : 1;   /* [0] */
        td_u64  bit1_awb         : 1;   /* [1] */
        td_u64  bit1_af          : 1;   /* [2] */
        td_u64  bit1_dp          : 1;   /* [3] */
        td_u64  bit1_ge          : 1;   /* [4] */
        td_u64  bit1_wdr         : 1;   /* [5] */
        td_u64  bit1_expander    : 1;   /* [6] */
        td_u64  bit1_blc         : 1;   /* [7] */
        td_u64  bit1_bayernr     : 1;   /* [8] */
        td_u64  bit1_split       : 1;   /* [9] */
        td_u64  bit1_flicker     : 1;   /* [10] */
        td_u64  bit1_dg          : 1;   /* [11] */
        td_u64  bit1_hrs         : 1;   /* [12] */
        td_u64  bit1_fe_lsc      : 1;   /* [13] */
        td_u64  bit1_lsc         : 1;   /* [14] */
        td_u64  bit1_r_lsc       : 1;   /* [15] */
        td_u64  bit1_rc          : 1;   /* [16] */
        td_u64  bit1_drc         : 1;   /* [17] */
        td_u64  bit1_dehaze      : 1;   /* [18] */
        td_u64  bit1_cac         : 1;   /* [19] */
        td_u64  bit1_demosaic    : 1;   /* [20] */
        td_u64  bit1_fcr         : 1;   /* [21] */
        td_u64  bit1_gamma       : 1;   /* [22] */
        td_u64  bit1_csc         : 1;   /* [23] */
        td_u64  bit1_ca          : 1;   /* [24] */
        td_u64  bit1_fpn         : 1;   /* [25] */
        td_u64  bit1_sharpen     : 1;   /* [26] */
        td_u64  bit1_edge_mark   : 1;   /* [27] */
        td_u64  bit1_mcds        : 1;   /* [28] */
        td_u64  bit1_ldci        : 1;   /* [29] */
        td_u64  bit1_pregamma    : 1;   /* [30] */
        td_u64  bit1_fe_log_lut  : 1;   /* [31] */
        td_u64  bit1_log_lut     : 1;   /* [32] */
        td_u64  bit1_clut        : 1;   /* [33] */
        td_u64  bit1_hlc         : 1;   /* [34] */
        td_u64  bit1_detail      : 1;   /* [35] */
        td_u64  bit1_rgbir       : 1;   /* [36] */
        td_u64  bit1_acs         : 1;   /* [37] */
        td_u64  bit1_bshp        : 1;   /* [38] */
        td_u64  bit1_lblc        : 1;   /* [39] */
#ifdef CONFIG_SD3403V100
        td_u64  bit1_lcac        : 1;   /* [40] for 3403 */
        td_u64  bit1_acac        : 1;   /* [41] for 3403 */
        td_u64  bit22_rsv        : 22;  /* [42:63] */
#else
        td_u64  bit24_rsv        : 24;  /* [40:63] */
#endif
    };
} isp_alg_key;

typedef struct {
    /* 1. ctrl param */
    td_bool                  mem_init;
    td_bool                  sns_reg;
    td_bool                  isp_yuv_mode;
    td_bool                  is_vreg_checked;
    isp_para_rec             para_rec;
    ot_rect                  sys_rect;
    isp_block_attr           block_attr;
    isp_special_opt          special_opt;
    ot_isp_be_frame_attr     be_frame_attr;
#ifdef CONFIG_OT_ISP_DUMP_DEBUG_SUPPORT
    isp_be_pos_attr          be_pos_attr;
#endif
    td_u8                    be_buf_num;
    isp_run_thread_status    run_thread_status;
    /* 2. algorithm ctrl param */
    td_bool                  freeze_fw;
    td_bool                  change_wdr_mode;
    td_bool                  change_image_mode;
    td_bool                  change_isp_res;
    td_bool                  isp0_p2_en;
    td_bool                  ldci_tpr_flt_en;
    td_bool                  is_ia_nr_enable;
    td_u8                    pre_sns_wdr_mode;
    td_u8                    sns_wdr_mode;
    td_u32                   frame_cnt;    /* frame count for 3a libs which can decide the interval of calculate. */
    ot_isp_alg_run_select    alg_run_select;
    ot_isp_bayer_format      bayer;
    isp_alg_key              alg_key;
    ot_isp_fe_roi_cfg        fe_roi_cfg;

    td_u8                    isp_image_mode_flag;
    isp_sensor_image_mode    pre_sns_image_mode;
    isp_sensor_image_mode    sns_image_mode;

    ot_isp_ae_result         ae_result;
    ot_isp_awb_result        awb_result;

    ot_size                  pipe_size;
    vi_pipe_hdr_attr         hdr_attr;
    vi_pipe_wdr_attr         wdr_attr;
    vi_stitch_attr           stitch_attr;
    ot_color_gamut           color_gamut;
    ot_isp_smart_info        smart_info;
    isp_blc_actual_info      actual_blc;
    isp_stagger_attr         stagger_attr;
    /* 3. 3a register info */
    ot_isp_bind_attr         bind_attr;
    isp_lib_info             ae_lib_info;
    isp_lib_info             awb_lib_info;
    isp_lib_info             af_lib_info;

    /* 4. algorithm ctrl */
    isp_alg_node             algs[ISP_MAX_ALGS_NUM];
    ot_isp_alg_exp_func      isp_alg_exp;
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
#ifdef CONFIG_OT_SNAP_SUPPORT
    isp_pro_param            pro_param;
#endif
    ot_isp_dcf_update_info   dcf_update_info;
    ot_isp_frame_info        frame_info;

    /* DNG info */
    isp_dng_info_ctrl        dng_info_ctrl;
    ot_isp_dng_color_param   pre_dng_color_param;
    isp_dng_ccm              pre_dng_ccm;
    td_u8                    pro_frm_cnt;
    pq_ai_attr               ai_info;

    /* 7. vi_proc/BE addr */
    td_void                 *isp_post_be_virt_addr[OT_ISP_STRIPING_MAX_NUM];
    td_void                 *isp_pre_be_virt_addr[OT_ISP_STRIPING_MAX_NUM];
    td_void                 *pre_viproc_virt_addr[OT_ISP_STRIPING_MAX_NUM];
    td_void                 *post_viproc_virt_addr[OT_ISP_STRIPING_MAX_NUM];

    /* 8. detail stats info */
    td_void                 *isp_be_ae_virt_addr;
    td_void                 *isp_be_awb_virt_addr;
#ifdef CONFIG_OT_ISP_DETAIL_STATS_SUPPORT
    isp_detail_stats_info   detail_stats_info;
#endif
} isp_usr_ctx;

#define isp_regcfg_get_ctx(pipe, ctx) ((ctx) = isp_get_regcfg_ctx(pipe))

#define isp_check_open_goto(vi_pipe, ret, failed)               \
    do {                                         \
        td_s32 ot_ret = isp_check_dev_open(vi_pipe); \
        if (ot_ret != TD_SUCCESS) {              \
            (ret) = ot_ret;                      \
            goto failed;                       \
        }                                        \
    } while (0)
#define isp_check_mem_init_goto(vi_pipe, ret, failed)                 \
    do {                                               \
        td_s32 ot_ret = isp_check_mem_init_func(vi_pipe);  \
        if (ot_ret != TD_SUCCESS) {                    \
            (ret) = ot_ret;                             \
            goto failed;                              \
        }                                              \
    } while (0)

#define isp_check_open_return(vi_pipe)               \
    do {                                         \
        td_s32 ot_ret = isp_check_dev_open(vi_pipe); \
        if (ot_ret != TD_SUCCESS) {              \
            return ot_ret;                       \
        }                                        \
    } while (0)

#define isp_check_mem_init_return(pipe)                 \
    do {                                               \
        td_s32 ot_ret = isp_check_mem_init_func(pipe);  \
        if (ot_ret != TD_SUCCESS) {                    \
            return ot_ret;                             \
        }                                              \
    } while (0)

#define isp_check_mem_init_notrace_return(pipe)                 \
    do {                                               \
        td_s32 ot_ret = isp_check_mem_init_func_notrace(pipe);  \
        if (ot_ret != TD_SUCCESS) {                    \
            return ot_ret;                             \
        }                                              \
    } while (0)
#define isp_check_vreg_permission_goto(dev, ret, exit)                 \
    do {                                               \
        td_s32 ot_ret = isp_check_vreg_permission(dev);  \
        if (ot_ret != TD_SUCCESS) {                    \
            (ret) = ot_ret;                              \
            goto exit;                                 \
        }                                              \
    } while (0)

#define isp_check_vreg_permission_return(dev)                 \
    do {                                               \
        td_s32 ot_ret = isp_check_vreg_permission(dev);  \
        if (ot_ret != TD_SUCCESS) {                    \
            return ot_ret;                             \
        }                                              \
    } while (0)

#define isp_get_ctx(pipe, isp_ctx)    \
    do {                               \
        (isp_ctx) = isp_get_usr_ctx(pipe); \
    } while (0)

#define isp_check_offline_mode_return(pipe)                                          \
        do {                                                                        \
            isp_usr_ctx *isp_ctx_tmp = TD_NULL;                                     \
            isp_get_ctx(pipe, isp_ctx_tmp);                                          \
            if ((is_online_mode(isp_ctx_tmp->block_attr.running_mode)) ||           \
                (is_sidebyside_mode(isp_ctx_tmp->block_attr.running_mode))) {       \
                return TD_SUCCESS;                                                  \
            }                                                                       \
        } while (0)

#define isp_check_not_offline_mode_return(pipe)                                          \
        do {                                                                        \
            isp_usr_ctx *isp_ctx_tmp = TD_NULL;                                     \
            isp_get_ctx(pipe, isp_ctx_tmp);                                          \
            if (is_offline_mode(isp_ctx_tmp->block_attr.running_mode) == TD_FALSE) {            \
                return TD_SUCCESS;                                                  \
            }                                                                       \
        } while (0)
#define isp_check_not_offline_mode_void(pipe)                                          \
        do {                                                                        \
            isp_usr_ctx *isp_ctx_tmp = TD_NULL;                                     \
            isp_get_ctx(pipe, isp_ctx_tmp);                                          \
            if (is_offline_mode(isp_ctx_tmp->block_attr.running_mode) == TD_FALSE) {       \
                return;                                                  \
            }                                                                       \
        } while (0)
#define isp_check_wdr_main_pipe_goto(wdr_mode, mast_pipe, ret, exit)             \
    do {                                                                \
        if (is_wdr_mode(wdr_mode) && ((mast_pipe) != TD_TRUE)) {        \
            (ret) = TD_SUCCESS;                                           \
            goto exit;                                                  \
        }                                                               \
    } while (0)

#define isp_check_wdr_main_pipe_return(wdr_mode, mast_pipe)             \
    do {                                                                \
        if (is_wdr_mode(wdr_mode) && ((mast_pipe) != TD_TRUE)) {        \
            return TD_SUCCESS;                                          \
        }                                                               \
    } while (0)
#define isp_check_demosaic_init_return(pipe)                                 \
    do {                                                                    \
        if (ot_ext_system_isp_demosaic_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] demosaic NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                                 \
        }                                                                   \
    } while (0)
#define isp_check_drc_init_return(pipe)                                 \
    do {                                                               \
        if (ot_ext_system_isp_drc_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] drc NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_pregamma_init_return(pipe)                                 \
    do {                                                                    \
        if (ot_ext_system_isp_pregamma_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] pregamma NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                                 \
        }                                                                   \
    } while (0)
#define isp_check_gamma_init_return(pipe)                                 \
    do {                                                                 \
        if (ot_ext_system_isp_gamma_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] gamma NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                              \
        }                                                                \
    } while (0)
#define isp_check_sharpen_init_return(pipe)                                 \
    do {                                                                   \
        if (ot_ext_system_isp_sharpen_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] sharpen NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                                \
        }                                                                  \
    } while (0)
#define isp_check_ldci_init_return(pipe)                                 \
    do {                                                                \
        if (ot_ext_system_isp_ldci_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] ldci NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                             \
        }                                                               \
    } while (0)
#define isp_check_dpc_init_return(pipe)                                 \
    do {                                                               \
        if (ot_ext_system_isp_dpc_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] dpc NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_mesh_shading_init_return(pipe)                       \
    do {                                                              \
        if (ot_ext_system_isp_lsc_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] mesh shading NOT init!\n", pipe);   \
            return OT_ERR_ISP_ALG_NOT_INIT;                           \
        }                                                             \
    } while (0)
#define isp_check_mesh_shading_init_goto(pipe, ret, exit)                       \
    do {                                                              \
        if (ot_ext_system_isp_lsc_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] mesh shading NOT init!\n", pipe);   \
            (ret) = OT_ERR_ISP_ALG_NOT_INIT;                           \
            goto exit;                                                 \
        }                                                             \
    } while (0)

#define isp_check_crosstalk_init_return(pipe)                             \
    do {                                                                 \
        if (ot_ext_system_isp_ge_init_status_read(pipe) != TD_TRUE) {     \
            isp_err_trace("ISP[%d] crosstalk removal NOT init!\n", pipe); \
            return OT_ERR_ISP_ALG_NOT_INIT;                              \
        }                                                                \
    } while (0)

#define isp_check_anti_false_color_init_return(pipe)                      \
    do {                                                                 \
        if (ot_ext_system_isp_afc_init_status_read(pipe) != TD_TRUE) {    \
            isp_err_trace("ISP[%d] anti false color NOT init!\n", pipe);  \
            return OT_ERR_ISP_ALG_NOT_INIT;                              \
        }                                                                \
    } while (0)
#define isp_check_ca_init_return(pipe)                                 \
    do {                                                              \
        if (ot_ext_system_isp_ca_init_status_read(pipe) != TD_TRUE) {  \
            isp_err_trace("ISP[%d] ca NOT init!\n", pipe);             \
            return OT_ERR_ISP_ALG_NOT_INIT;                           \
        }                                                             \
    } while (0)
#define isp_check_clut_init_return(pipe)                                  \
    do {                                                                 \
        if (ot_ext_system_isp_clut_init_status_read(pipe) != TD_TRUE) {   \
            isp_err_trace("ISP[%d] clut NOT init!\n", pipe);              \
            return OT_ERR_ISP_ALG_NOT_INIT;                              \
        }                                                                \
    } while (0)

#define isp_check_fswdr_init_return(pipe)                                   \
    do {                                                                   \
        td_u8 wdr_mode = ot_ext_system_sensor_wdr_mode_read(pipe);          \
        if (is_fs_wdr_mode(wdr_mode) &&                                    \
            (ot_ext_system_isp_fswdr_init_status_read(pipe) != TD_TRUE) ) { \
            isp_err_trace("ISP[%d] fswdr NOT init!\n", pipe);               \
            return OT_ERR_ISP_ALG_NOT_INIT;                                \
        }                                                                  \
    } while (0)
#define isp_check_dehaze_init_return(pipe)                                \
    do {                                                                 \
        if (ot_ext_system_isp_dehaze_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] dehaze NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                              \
        }                                                                \
    } while (0)
#define isp_check_local_cac_init_return(pipe)                           \
    do {                                                               \
        if (ot_ext_system_isp_lcac_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] local cac NOT init!\n", pipe);       \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_acac_init_return(pipe)                                \
    do {                                                               \
        if (ot_ext_system_isp_acac_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] acac NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_cac_init_return(pipe)                                \
    do {                                                               \
        if (ot_ext_system_isp_cac_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] cac NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_bayershp_init_return(pipe)                            \
    do {                                                               \
        if (ot_ext_system_isp_bshp_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] bayershp NOT init!\n", pipe);        \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_auto_color_shading_init_return(pipe)                     \
    do {                                                                  \
        if (ot_ext_system_isp_acs_init_status_read(pipe) != TD_TRUE) {     \
            isp_err_trace("ISP[%d] auto color shading NOT init!\n", pipe); \
            return OT_ERR_ISP_ALG_NOT_INIT;                               \
        }                                                                 \
    } while (0)
#define isp_check_rgbir_init_return(pipe)                                \
    do {                                                                \
        if (ot_ext_system_isp_rgbir_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] rgbir NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                            \
        }                                                              \
    } while (0)
#define isp_check_bayer_nr_init_return(pipe)                               \
    do {                                                                  \
        if (ot_ext_system_isp_bayernr_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] bayer nr NOT init!\n", pipe);           \
            return OT_ERR_ISP_ALG_NOT_INIT;                               \
        }                                                                 \
    } while (0)
#define isp_check_blc_init_return(pipe)                                \
    do {                                                              \
        if (ot_ext_system_isp_blc_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] blc NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                           \
        }                                                             \
    } while (0)

#define isp_check_expander_init_return(pipe)                                \
    do {                                                              \
        if (ot_ext_system_isp_expander_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] expander NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                           \
        }                                                             \
    } while (0)
#define isp_check_lblc_init_return(pipe)                                \
    do {                                                              \
        if (ot_ext_system_isp_lblc_init_status_read(pipe) != TD_TRUE) { \
            isp_err_trace("ISP[%d] lblc NOT init!\n", pipe);            \
            return OT_ERR_ISP_ALG_NOT_INIT;                           \
        }                                                             \
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
td_s32 isp_run(ot_vi_pipe vi_pipe);
td_s32 isp_exit(ot_vi_pipe vi_pipe);
td_s32 isp_stitch_sync_exit(ot_vi_pipe vi_pipe);

td_s32 isp_switch_wdr_mode(ot_vi_pipe vi_pipe);
td_s32 isp_switch_image_mode(ot_vi_pipe vi_pipe);
td_s32 isp_switch_mode(ot_vi_pipe vi_pipe);

td_s32 isp_wdr_cfg_set(ot_vi_pipe vi_pipe);
td_u32 isp_frame_cnt_get(ot_vi_pipe vi_pipe);

td_u16 isp_get_max_width(ot_vi_pipe vi_pipe);
td_u16 isp_get_max_height(ot_vi_pipe vi_pipe);

td_s32 isp_check_dev_open(ot_vi_pipe vi_pipe);
td_s32 isp_check_mem_init_func(ot_vi_pipe vi_pipe);
td_s32 isp_check_mem_init_func_notrace(ot_vi_pipe vi_pipe);
td_s32 isp_check_vreg_permission(ot_isp_dev dev);
td_bool isp_check_hrs_on(ot_vi_pipe vi_pipe);
td_s32 isp_get_fd(ot_vi_pipe vi_pipe);
isp_usr_ctx *isp_get_usr_ctx(ot_vi_pipe vi_pipe);
isp_reg_cfg_attr *isp_get_regcfg_ctx(ot_vi_pipe vi_pipe);

td_s32 isp_mem_info_set(ot_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);
td_s32 isp_check_sns_register(ot_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);
td_s32 isp_mem_init_update_ctx(ot_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);

td_s32 isp_init(ot_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);

td_s32 isp_alg_buf_init(ot_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);
td_void isp_alg_buf_exit(ot_vi_pipe vi_pipe);

td_s32 isp_run_thread_proc(ot_vi_pipe vi_pipe);
td_s32 isp_share_mem(td_phys_addr_t phys_addr, td_s32 pid);
td_s32 isp_unshare_mem(td_phys_addr_t phys_addr, td_s32 pid);
td_s32 isp_share_mem_all(td_phys_addr_t phys_addr);
td_s32 isp_unshare_mem_all(td_phys_addr_t phys_addr);
td_void isp_mutex_lock(ot_vi_pipe pipe);
td_void isp_mutex_unlock(ot_vi_pipe pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_MAIN_H */
