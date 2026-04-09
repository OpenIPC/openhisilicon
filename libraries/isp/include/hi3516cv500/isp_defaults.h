/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_defaults.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DEFAULTS_H_
#define __ISP_DEFAULTS_H_

#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* ext register default value */
hi_void isp_ext_regs_default(hi_vi_pipe vi_pipe);

/* initialize ext register */
hi_void isp_ext_regs_initialize(hi_vi_pipe vi_pipe);

/* isp register default value */
hi_void isp_regs_default(hi_vi_pipe vi_pipe);

/* initialize isp register */
hi_void isp_regs_initialize(hi_vi_pipe vi_pipe);

/* this function initialises an instance of isp_usr_ctx */
hi_void isp_global_initialize(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx);

hi_void isp_dng_ext_regs_initialize(hi_vi_pipe vi_pipe, hi_isp_dng_color_param *dng_color_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_DEFAULTS_H_ */
