/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_regcfg.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_REGCFG_H__
#define __ISP_REGCFG_H__

#include "hi_comm_isp_adapt.h"
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
    hi_u64 be_phy_addr; /* head addr */
    hi_void *be_virt_addr;

    isp_be_wo_cfg_buf be_wo_cfg_buf;
} isp_be_buf;

typedef struct {
    isp_mmz_buf_ex lut_stt_buf[ISP_MAX_BE_NUM];
} isp_be_lut_buf;

hi_s32 isp_mod_param_get(hi_isp_mod_param *mod_param);
hi_s32 isp_reset_fe_stt_en(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_en_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_ldci_buf_init(hi_vi_pipe vi_pipe);
hi_void isp_ldci_buf_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_clut_buf_init(hi_vi_pipe vi_pipe);
hi_void isp_clut_buf_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_spec_awb_buf_init(hi_vi_pipe vi_pipe);
hi_void isp_spec_awb_buf_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_stt_buf_init(hi_vi_pipe vi_pipe);
hi_void isp_stt_buf_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_stt_addr_init(hi_vi_pipe vi_pipe);
hi_s32 isp_cfg_be_buf_init(hi_vi_pipe vi_pipe);
hi_void isp_cfg_be_buf_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_reg_cfg_init(hi_vi_pipe vi_pipe);
hi_s32 isp_get_reg_cfg_ctx(hi_vi_pipe vi_pipe, hi_void **reg_cfg);
hi_s32 isp_reg_cfg_ctrl(hi_vi_pipe vi_pipe);
hi_void isp_reg_cfg_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_switch_reg_set(hi_vi_pipe vi_pipe);
hi_s32 isp_get_be_last_buf(hi_vi_pipe vi_pipe);
hi_s32 isp_be_vreg_addr_init(hi_vi_pipe vi_pipe);
hi_s32 isp_get_be_vreg_cfg_addr(hi_vi_pipe vi_pipe, hi_void *virt_addr[]);
hi_s32 isp_get_be_buf_first(hi_vi_pipe vi_pipe);
hi_s32 isp_get_be_free_buf(hi_vi_pipe vi_pipe);
hi_s32 isp_all_cfgs_be_buf_init(hi_vi_pipe vi_pipe);
hi_s32 isp_set_cfg_be_buf_state(hi_vi_pipe vi_pipe);
hi_s32 isp_reg_cfg_info_set(hi_vi_pipe vi_pipe);
hi_s32 isp_reg_cfg_info_init(hi_vi_pipe vi_pipe);
hi_s32 isp_lut2stt_apb_reg(hi_vi_pipe vi_pipe);
hi_s32 isp_sync_cfg_set(hi_vi_pipe vi_pipe);
hi_s32 isp_snap_reg_cfg_set(hi_vi_pipe vi_pipe, hi_isp_config_info *snap_info);
hi_s32 isp_snap_reg_cfg_get(hi_vi_pipe vi_pipe, isp_snap_info *snap_info);
hi_bool isp_pro_trigger_get(hi_vi_pipe vi_pipe);
hi_void *isp_get_be_vir_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev);
hi_void *isp_get_be_lut2stt_vir_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev, hi_u8 buf_id);
hi_void *isp_get_vi_proc_vir_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev);
hi_void *isp_get_fe_vir_addr(hi_vi_pipe vi_pipe);
hi_s32 isp_switch_state_set(hi_vi_pipe vi_pipe);
hi_s32  isp_set_be_raw(hi_vi_pipe vi_pipe);

#define isp_check_fe_pipe_return(pipe)                              \
    do {                                                     \
        if (((pipe) < 0) || ((pipe) >= ISP_MAX_PIPE_NUM)) {  \
            isp_err_trace("Invalid ISP-FE Pipe!\n"); \
            return HI_NULL;                                  \
        }                                                    \
    } while (0)

#define isp_check_be_dev_return(dev)                                 \
    do {                                                      \
        if (((dev) < 0) || ((dev) >= ISP_STRIPING_MAX_NUM)) { \
            isp_err_trace("Err ISP-BE dev!\n");       \
            return HI_NULL;                                   \
        }                                                     \
    } while (0)

#define isp_check_reg_nullptr_return(ptr)                                                \
    do {                                                                      \
        if ((ptr) == HI_NULL) {                                                 \
            isp_err_trace("Null point when writing isp register!\n"); \
            return HI_NULL;                                                   \
        }                                                                     \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
