/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_demosaic.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DEMOSAIC_H__
#define __ISP_DEMOSAIC_H__

#include "isp_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_vi_pipe vi_pipe;
    hi_u8   non_dir_str;
    hi_u32  iso;
} gf_blur_lut_cfg;

typedef struct {
    hi_u32 iso;
    hi_u32 iso1;
    hi_u32 iso2;
    hi_u8  idx_upper;
    hi_u8  idx_low;
} demosaic_iso_cfg;

typedef struct {
    /* Processing Depth */
    hi_bool  init;
    hi_bool  dem_attr_update;

    hi_u8    bit_depth_prc;    /* u5.0 */
    hi_u8    wdr_mode;
    hi_u32   sigma_demcfg[HI_ISP_DEMOSAIC_LUT_LENGTH];

    hi_u8    lut_awb_gfgain_low[ISP_AUTO_ISO_STRENGTH_NUM]; /* u3.4, */
    hi_u8    lut_awb_gfgain_hig[ISP_AUTO_ISO_STRENGTH_NUM]; /* u3.4, */
    hi_u8    lut_awb_gfgain_max[ISP_AUTO_ISO_STRENGTH_NUM]; /* u4.0, */
    hi_u8    lut_cc_hf_max_ratio[ISP_AUTO_ISO_STRENGTH_NUM]; /* u5.0, 0~16 */
    hi_u8    lut_cc_hf_min_ratio[ISP_AUTO_ISO_STRENGTH_NUM]; /* u5.0, 0~16 */
    hi_s8    lut_fcr_gf_gain[ISP_AUTO_ISO_STRENGTH_NUM]; /* s3.2, fcr control */
    hi_u16   lut_desat_low[ISP_AUTO_ISO_STRENGTH_NUM]; /* u9.0, 0~256 */
    hi_u16   lut_desat_prot_th[ISP_AUTO_ISO_STRENGTH_NUM]; /* u10.0, 0~1023 */
    hi_u16   lut_fcr_det_low[ISP_AUTO_ISO_STRENGTH_NUM]; /* u12.0,  fcr det thresh */
    hi_u16   hf_intp_blur_th_low[ISP_AUTO_ISO_STRENGTH_NUM]; /* u10.0 */
    hi_u16   hf_intp_blur_th_hig[ISP_AUTO_ISO_STRENGTH_NUM]; /* u10.0, */

    hi_isp_demosaic_manual_attr actual; /* actual param */

    /* MPI */
    hi_isp_demosaic_attr mpi_cfg; /* param read from mpi */
} isp_demosaic;

hi_s32 isp_demosaic_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_demosaic *demosaic_ctx);
hi_void isp_demosaic_read_extregs(hi_vi_pipe vi_pipe, isp_demosaic *demosaic_ctx);
hi_void isp_demosaic_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_demosaic *demosaic_ctx);
hi_void isp_demosaic_set_long_frame_mode(hi_vi_pipe vi_pipe, isp_demosaic *demosaic_ctx);
hi_s32 demosaic_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc, isp_demosaic *demosaic);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
