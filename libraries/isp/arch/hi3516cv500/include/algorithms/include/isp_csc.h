/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_csc.c
 * Author: ISP SW
 * Create: 2012/06/28
 */
#ifndef __ISP_CSC_H__
#define __ISP_CSC_H__

#include "hi_type.h"
#include "hi_comm_isp_adapt.h"
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_bool update;
    hi_isp_csc_attr mpi_cfg;
} isp_csc;

hi_void isp_csc_read_ext_regs(hi_vi_pipe vi_pipe, isp_csc *csc);
hi_void isp_csc_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg);
hi_s32  isp_csc_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_csc *csc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif