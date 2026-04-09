/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_bayernr.h
 * Author: ISP SW
 * Create: 2012/06/28
 */
#ifndef __ISP_BAYERNR_H__
#define __ISP_BAYERNR_H__

#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

typedef struct {
    hi_u8 iso_index_upper, iso_index_lower;
    hi_u32 iso, iso1, iso2;
} isp_bnr_iso;

typedef struct {
    hi_bool init;
    hi_bool bnr_mono_sensor_en;
    hi_bool lut_update;
    hi_bool center_wgt_en;
    hi_bool wdr_mode_en;
    hi_bool wdr_fusion_en;

    hi_u8 wdr_map_gain;
    hi_u8 wdr_frames_merge;
    hi_u8 fine_str;
    hi_u16 center_lmt;
    hi_u16 wdr_blc_thr;
    hi_u16 coring_low;
    hi_u16 lmt_np_thresh;
    hi_u16 lmt_np_thresh_s;
    hi_u32 edge_det_thr;

    hi_u8 jnlm_limit_lut[HI_ISP_BAYERNR_LMTLUTNUM];  /* u8.0 */
    hi_u8 lut_chroma_ratio[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    hi_u8 lut_wdr_chroma_ratio[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    hi_u8 lut_g_coring_gain[HI_ISP_BAYERNR_LUT_LENGTH];  /* u4.4 */
    hi_u16 lut_center_lmt[ISP_AUTO_ISO_STRENGTH_NUM];   /* u1.8 */
    hi_u16 lmt_strength[ISP_BAYER_CHN_NUM];
    hi_u16 wdr_frame_thr[WDR_MAX_FRAME_NUM + 2]; /* 2 */
    hi_u16 lut_coring_hig[ISP_AUTO_ISO_STRENGTH_NUM];  /* u14.0 */
    hi_u16 coarse_str[ISP_BAYER_CHN_NUM];
    hi_u32 jnlm_limit_mult_gain[ISP_BAYER_CHN_NUM];  /* u21.0 */
    hi_u32 expo_values[WDR_MAX_FRAME_NUM];

    hi_u16 wdr_sync_frame_thr[CFG2VLD_DLY_LIMIT][WDR_MAX_FRAME_NUM + 2]; /* 2 */
    hi_bool wdr_sync_fusion_en[CFG2VLD_DLY_LIMIT];

    hi_isp_nr_attr mpi_cfg;
} isp_bayernr;

hi_s32 isp_bayernr_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr);
hi_void isp_bayernr_read_extregs(hi_vi_pipe vi_pipe, isp_be_blc_cfg *be_blc_cfg, isp_bayernr *bayernr);
hi_s32 isp_bayernr_read_pro_mode(hi_vi_pipe vi_pipe, isp_bayernr *bayernr);
hi_void isp_bayernr_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr);
hi_s32 bayernr_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc, isp_bayernr *bayernr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif