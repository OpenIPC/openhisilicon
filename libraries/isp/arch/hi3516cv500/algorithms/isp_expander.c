/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_expander.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"

static hi_void expander_static_regs_initialize(hi_vi_pipe vi_pipe, isp_expander_static_cfg *static_reg_cfg)
{
    hi_u32 _i, _v;
    hi_u32 X0, Y0, X1, Y1, X2, Y2, X3, Y3, x_max, y_max;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_expander) {
        static_reg_cfg->bit_depth_in   = sns_dft->expander->bit_depth_in;
        static_reg_cfg->bit_depth_out  = sns_dft->expander->bit_depth_out;

        X0       = sns_dft->expander->expander_point[0].x; /* array index 0 */
        Y0       = sns_dft->expander->expander_point[0].y; /* array index 0 */
        X1       = sns_dft->expander->expander_point[1].x; /* array index 1 */
        Y1       = sns_dft->expander->expander_point[1].y; /* array index 1 */
        X2       = sns_dft->expander->expander_point[2].x; /* array index 2 */
        Y2       = sns_dft->expander->expander_point[2].y; /* array index 2 */
        X3       = sns_dft->expander->expander_point[3].x; /* array index 3 */
        Y3       = sns_dft->expander->expander_point[3].y; /* array index 3 */
        x_max    = sns_dft->expander->expander_point[4].x; /* array index 4 */
        y_max    = sns_dft->expander->expander_point[4].y; /* array index 4 */

        for (_i = 0; _i < X0; _i++) {
            _v = (((_i * Y0) / div_0_to_1(X0)) >> 0);
            static_reg_cfg->lut[_i] = _v;
        }

        for (; _i < X1; _i++) {
            _v = ((((_i - X0) * (Y1 - Y0)) / div_0_to_1(X1 - X0) + Y0)  >> 0);
            static_reg_cfg->lut[_i] = _v;
        }

        for (; _i < X2; _i++) {
            _v = ((((_i - X1) * (Y2 - Y1)) / div_0_to_1(X2 - X1) + Y1)  >> 0);
            static_reg_cfg->lut[_i] = _v;
        }

        for (; _i < X3; _i++) {
            _v = ((((_i - X2) * (Y3 - Y2)) / div_0_to_1(X3 - X2) + Y2)  >> 0);
            static_reg_cfg->lut[_i] = _v;
        }

        for (; _i < x_max; _i++) {
            _v = (y_max  >> 0);
            static_reg_cfg->lut[_i] = _v;
        }
    } else {
        static_reg_cfg->bit_depth_in   = 12; /* bit depth in  12 */
        static_reg_cfg->bit_depth_out  = 16; /* bit depth out 16 */
        memset_s(static_reg_cfg->lut, EXPANDER_NODE_NUM * sizeof(hi_u32), 0, EXPANDER_NODE_NUM * sizeof(hi_u32));
    }

    static_reg_cfg->resh = HI_TRUE;
}

static hi_s32 expander_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_expander *cmos_expander)
{
    hi_u8 i;

    isp_check_bool_return(cmos_expander->enable);

    if ((cmos_expander->bit_depth_in > 0x14) || (cmos_expander->bit_depth_in < 0xC)) {
        isp_err_trace("invalid bit_depth_in!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((cmos_expander->bit_depth_out > 0x14) || (cmos_expander->bit_depth_out < 0xC)) {
        isp_err_trace("invalid bit_depth_out!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_EXPANDER_POINT_NUM; i++) {
        if (cmos_expander->expander_point[i].x > 0x101) {
            isp_err_trace("invalid expander_point[%d].x!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_expander->expander_point[i].y > 0x100000) {
            isp_err_trace("invalid expander_point[%d].y!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 expander_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i;
    hi_s32 ret;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_get_ctx(vi_pipe, isp_ctx);

    if (sns_dft->key.bit1_expander) {
        isp_check_pointer_return(sns_dft->expander);

        ret = expander_check_cmos_param(vi_pipe, sns_dft->expander);
        if (ret != HI_SUCCESS) {
            return ret;
        }
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        expander_static_regs_initialize(vi_pipe, &reg_cfg->alg_reg_cfg[i].expander_cfg.static_cfg);

        if (isp_ctx->sns_wdr_mode == WDR_MODE_BUILT_IN) {
            if (sns_dft->key.bit1_expander) {
                reg_cfg->alg_reg_cfg[i].expander_cfg.enable = sns_dft->expander->enable;
            } else {
                reg_cfg->alg_reg_cfg[i].expander_cfg.enable = HI_FALSE;
            }
        } else {
            reg_cfg->alg_reg_cfg[i].expander_cfg.enable = HI_FALSE;
        }
    }

    reg_cfg->cfg_key.bit1_expander_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_expander_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    hi_s32  ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    ret = expander_regs_initialize(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_expander_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                        hi_void *reg_cfg_info, hi_s32 rsv)
{
    return HI_SUCCESS;
}

hi_s32 isp_expander_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg_attr = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg_attr);
            isp_check_pointer_return(reg_cfg_attr);
            isp_expander_init(vi_pipe, (hi_void *)&reg_cfg_attr->reg_cfg);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_expander_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg_attr = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg_attr);

    for (i = 0; i < reg_cfg_attr->reg_cfg.cfg_num; i++) {
        reg_cfg_attr->reg_cfg.alg_reg_cfg[i].expander_cfg.enable = HI_FALSE;
    }

    reg_cfg_attr->reg_cfg.cfg_key.bit1_expander_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_expander(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_expander);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_EXPANDER;
    algs->alg_func.pfn_alg_init = isp_expander_init;
    algs->alg_func.pfn_alg_run  = isp_expander_run;
    algs->alg_func.pfn_alg_ctrl = isp_expander_ctrl;
    algs->alg_func.pfn_alg_exit = isp_expander_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

