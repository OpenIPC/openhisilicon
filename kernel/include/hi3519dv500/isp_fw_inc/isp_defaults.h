/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DEFAULTS_H
#define ISP_DEFAULTS_H

#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* ext register default value */
td_void isp_ext_regs_default(ot_vi_pipe vi_pipe);

/* initialize ext register */
td_void isp_ext_regs_initialize(ot_vi_pipe vi_pipe);

/* isp register default value */
td_void isp_regs_default(ot_vi_pipe vi_pipe);

/* initialize isp register */
td_void isp_regs_initialize(ot_vi_pipe vi_pipe);

/* this function initialises an instance of isp_usr_ctx */
td_void isp_global_initialize(ot_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);

td_void isp_dng_ext_regs_initialize(ot_vi_pipe vi_pipe, ot_isp_dng_color_param *dng_color_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_DEFAULTS_H */
