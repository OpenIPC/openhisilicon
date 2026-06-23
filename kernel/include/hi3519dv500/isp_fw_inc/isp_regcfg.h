/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_REGCFG_H
#define ISP_REGCFG_H

#include "ot_common_isp.h"
#include "isp_sensor.h"
#include "mkp_isp.h"
#include "vi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define is_hrs_on(vi_pipe)    (isp_check_hrs_on(vi_pipe))
#define isp_alignup(x, g) ((((x) + (g)-1) / (g)) * (g))

typedef struct {
    td_u64 be_phy_addr; /* head addr */
    td_u64 be_buf_size;
    td_void *be_virt_addr;

    isp_be_wo_cfg_buf be_wo_cfg_buf;
} isp_be_buf;

typedef struct {
    isp_mmz_buf_ex lut_stt_buf[ISP_MAX_BE_NUM];
} isp_be_lut_buf;

td_s32 isp_ctrl_param_get(ot_isp_ctrl_param *ctrl_param);
td_s32 isp_reset_fe_stt_en(ot_vi_pipe vi_pipe);
td_s32 isp_alg_en_exit(ot_vi_pipe vi_pipe);
td_s32 isp_ldci_buf_init(ot_vi_pipe vi_pipe);
td_void isp_ldci_buf_exit(ot_vi_pipe vi_pipe);
td_s32 isp_alg_stats_buf_init(ot_vi_pipe vi_pipe);
td_void isp_alg_stats_buf_exit(ot_vi_pipe vi_pipe);
td_s32 isp_clut_buf_init(ot_vi_pipe vi_pipe);
td_void isp_clut_buf_exit(ot_vi_pipe vi_pipe);
td_s32 isp_stt_buf_init(ot_vi_pipe vi_pipe);
td_void isp_stt_buf_exit(ot_vi_pipe vi_pipe);
td_s32 isp_stt_addr_init(ot_vi_pipe vi_pipe);
td_s32 isp_cfg_be_buf_init(ot_vi_pipe vi_pipe);
td_void isp_cfg_be_buf_exit(ot_vi_pipe vi_pipe);
#ifdef CONFIG_OT_ISP_DETAIL_STATS_SUPPORT
td_s32 isp_detail_stats_buf_init(ot_vi_pipe vi_pipe);
td_void isp_detail_stats_buf_exit(ot_vi_pipe vi_pipe);
#endif
td_s32 isp_reg_cfg_init(ot_vi_pipe vi_pipe);
td_s32 isp_get_reg_cfg_ctx(ot_vi_pipe vi_pipe, td_void **reg_cfg_info);
td_s32 isp_reg_cfg_ctrl(ot_vi_pipe vi_pipe);
td_void isp_reg_cfg_exit(ot_vi_pipe vi_pipe);
td_s32 isp_switch_reg_set(ot_vi_pipe vi_pipe);
td_s32 isp_get_be_last_buf(ot_vi_pipe vi_pipe);
td_s32 isp_be_vreg_addr_init(ot_vi_pipe vi_pipe);
td_s32 isp_get_be_buf_first(ot_vi_pipe vi_pipe);
td_s32 isp_get_be_free_buf(ot_vi_pipe vi_pipe);
td_s32 isp_all_cfgs_be_buf_init(ot_vi_pipe vi_pipe);
td_s32 isp_set_cfg_be_buf_state(ot_vi_pipe vi_pipe);
td_s32 isp_reg_cfg_info_set(ot_vi_pipe vi_pipe);
td_s32 isp_reg_cfg_info_init(ot_vi_pipe vi_pipe);
td_s32 isp_lut2stt_apb_reg(ot_vi_pipe vi_pipe);
td_s32 isp_sync_cfg_set(ot_vi_pipe vi_pipe);
#ifdef CONFIG_OT_SNAP_SUPPORT
td_s32 isp_snap_reg_cfg_set(ot_vi_pipe vi_pipe, ot_isp_config_info *snap_info);
td_bool isp_pro_trigger_get(ot_vi_pipe vi_pipe);
#endif
td_void *isp_get_pre_be_vir_addr(ot_vi_pipe vi_pipe, ot_blk_dev blk_dev);
td_void *isp_get_fe_vir_addr(ot_vi_pipe vi_pipe);
td_s32 isp_switch_state_set(ot_vi_pipe vi_pipe);

#define isp_check_fe_pipe_return_null(pipe)                          \
    do {                                                        \
        if (((pipe) < 0) || ((pipe) >= OT_ISP_MAX_FE_PIPE_NUM)) {  \
            isp_err_trace("Invalid ISP-FE Pipe!\n");            \
            return TD_NULL;                                     \
        }                                                       \
    } while (0)

#define isp_check_pipe_valid_return(pipe)                          \
    do {                                                        \
        if (((pipe) < 0) || ((pipe) >= OT_ISP_MAX_PIPE_NUM)) {  \
            isp_err_trace("Invalid ISP Pipe!\n");            \
            return TD_NULL;                                     \
        }                                                       \
    } while (0)

#define isp_check_be_dev_return(dev)                                 \
    do {                                                      \
        if (((dev) < 0) || ((dev) >= OT_ISP_STRIPING_MAX_NUM)) { \
            isp_err_trace("Err ISP-BE dev!\n");       \
            return TD_NULL;                                   \
        }                                                     \
    } while (0)

#define isp_check_reg_nullptr_return(ptr)                                                \
    do {                                                                      \
        if ((ptr) == TD_NULL) {                                                 \
            isp_err_trace("Null point when writing isp register!\n"); \
            return TD_NULL;                                                   \
        }                                                                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
