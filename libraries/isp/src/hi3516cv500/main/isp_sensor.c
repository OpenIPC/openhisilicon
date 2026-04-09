/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_sensor.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_sensor.h"
#include <unistd.h>
#include "hi_comm_sns_adapt.h"
#include "hi_osal.h"
#include "hi_debug_adapt.h"

typedef struct {
    hi_isp_sns_attr_info    sns_attr_info;
    hi_isp_sensor_register  sns_register;
    hi_isp_cmos_default     sns_dft;
    hi_isp_cmos_black_level sns_black_level;    /* some sensors's black level will be changed with iso */
    hi_isp_sns_regs_info    sns_reg_info;
    hi_isp_cmos_sensor_image_mode sns_image_mode;
} isp_sensor_ctx;

isp_sensor_ctx *g_sensor_ctx[ISP_MAX_PIPE_NUM] = { HI_NULL };

#define sensor_get_ctx(dev, ctx) ((ctx) = g_sensor_ctx[dev])
#define sensor_set_ctx(dev, ctx) (g_sensor_ctx[dev] = (ctx))
#define sensor_reset_ctx(dev)    (g_sensor_ctx[dev] = HI_NULL)

hi_s32 isp_sensor_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);

    if (sensor_ctx == HI_NULL) {
        sensor_ctx = (isp_sensor_ctx *)isp_malloc(sizeof(isp_sensor_ctx));
        if (sensor_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] sensor_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(sensor_ctx, sizeof(isp_sensor_ctx), 0, sizeof(isp_sensor_ctx));

    sensor_set_ctx(vi_pipe, sensor_ctx);

    return HI_SUCCESS;
}

hi_s32 isp_sensor_reg_callback(hi_vi_pipe vi_pipe, hi_isp_sns_attr_info *sns_attr_info,
                               hi_isp_sensor_register *sns_register)
{
    hi_s32 ret;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    ret = isp_sensor_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    memcpy_s(&sensor_ctx->sns_attr_info, sizeof(hi_isp_sns_attr_info), sns_attr_info, sizeof(hi_isp_sns_attr_info));
    memcpy_s(&sensor_ctx->sns_register, sizeof(hi_isp_sensor_register), sns_register, sizeof(hi_isp_sensor_register));

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_global_init != HI_NULL) {
        sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_global_init(vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_unreg_callback(hi_vi_pipe vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_free(sensor_ctx);
    sensor_reset_ctx(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_sensor_update_all(hi_vi_pipe vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;
    hi_isp_sensor_exp_func *sns_exp = HI_NULL;

    MPI_ASSERT(sizeof(ISP_CMOS_DEFAULT_S) == sizeof(hi_isp_cmos_default));
    MPI_ASSERT(sizeof(ISP_CMOS_DRC_S) == sizeof(hi_isp_cmos_drc));
    MPI_ASSERT(sizeof(ISP_CMOS_DEMOSAIC_S) == sizeof(hi_isp_cmos_demosaic));
    MPI_ASSERT(sizeof(ISP_CMOS_PREGAMMA_S) == sizeof(hi_isp_cmos_pregamma));
    MPI_ASSERT(sizeof(ISP_CMOS_GAMMA_S) == sizeof(hi_isp_cmos_gamma));
    MPI_ASSERT(sizeof(ISP_CMOS_SHARPEN_S) == sizeof(hi_isp_cmos_sharpen));
    MPI_ASSERT(sizeof(ISP_CMOS_LDCI_S) == sizeof(hi_isp_cmos_ldci));
    MPI_ASSERT(sizeof(ISP_CMOS_DPC_S) == sizeof(hi_isp_cmos_dpc));
    MPI_ASSERT(sizeof(ISP_CMOS_LSC_S) == sizeof(hi_isp_cmos_lsc));
    MPI_ASSERT(sizeof(ISP_CMOS_GE_S) == sizeof(hi_isp_cmos_ge));
    MPI_ASSERT(sizeof(ISP_CMOS_ANTIFALSECOLOR_S) == sizeof(hi_isp_cmos_afc));
    MPI_ASSERT(sizeof(ISP_CMOS_BAYERNR_S) == sizeof(hi_isp_cmos_bayernr));
    MPI_ASSERT(sizeof(ISP_CMOS_CA_S) == sizeof(hi_isp_cmos_ca));
    MPI_ASSERT(sizeof(ISP_CMOS_WDR_S) == sizeof(hi_isp_cmos_wdr));
    MPI_ASSERT(sizeof(ISP_CMOS_BLACK_LEVEL_S) == sizeof(hi_isp_cmos_black_level));

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);
    sns_exp = &sensor_ctx->sns_register.sns_exp;
    if (sns_exp->pfn_cmos_get_isp_default != HI_NULL) {
        sns_exp->pfn_cmos_get_isp_default(vi_pipe, &sensor_ctx->sns_dft);
    } else {
        isp_err_trace("Get isp[%d] default value error!\n", vi_pipe);
        return HI_FAILURE;
    }

    if (sns_exp->pfn_cmos_get_isp_black_level != HI_NULL) {
        sns_exp->pfn_cmos_get_isp_black_level(vi_pipe, &sensor_ctx->sns_black_level);
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_update_all_yuv(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    ret = isp_get_yuv_default(&sensor_ctx->sns_dft);

    sensor_ctx->sns_black_level.black_level[BAYER_CHN_R]  = 0x101;
    sensor_ctx->sns_black_level.black_level[BAYER_CHN_GR] = 0x101;
    sensor_ctx->sns_black_level.black_level[BAYER_CHN_GB] = 0x101;
    sensor_ctx->sns_black_level.black_level[BAYER_CHN_B]  = 0x101;
    sensor_ctx->sns_black_level.update = HI_TRUE;

    return ret;
}

hi_s32 isp_sensor_get_id(hi_vi_pipe vi_pipe, SENSOR_ID *sensor_id)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    *sensor_id = sensor_ctx->sns_attr_info.sensor_id;

    return HI_SUCCESS;
}

hi_s32 isp_sensor_get_blc(hi_vi_pipe vi_pipe, hi_isp_cmos_black_level **sns_black_level)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    *sns_black_level = &sensor_ctx->sns_black_level;

    return HI_SUCCESS;
}

hi_s32 isp_sensor_get_default(hi_vi_pipe vi_pipe, hi_isp_cmos_default **sns_dft)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    *sns_dft = &sensor_ctx->sns_dft;

    return HI_SUCCESS;
}

hi_s32 isp_sensor_get_max_resolution(hi_vi_pipe vi_pipe, hi_isp_cmos_sensor_max_resolution **sns_max_resolution)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    *sns_max_resolution = &sensor_ctx->sns_dft.sensor_max_resolution;

    return HI_SUCCESS;
}

hi_s32 isp_sensor_get_sns_reg(hi_vi_pipe vi_pipe, hi_isp_sns_regs_info **sns_reg_info)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    *sns_reg_info = &sensor_ctx->sns_reg_info;

    return HI_SUCCESS;
}

hi_s32 isp_sensor_init(hi_vi_pipe vi_pipe)
{
    hi_s8 ssp_dev;
    hi_s32 ret;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    /* if I2C or SSP Dev is -1, don't init sensor */
    {
        hi_isp_sns_regs_info *sns_regs_info = HI_NULL;

        ret = isp_sensor_update_sns_reg(vi_pipe);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        ret = isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        ssp_dev = sns_regs_info->com_bus.ssp_dev.bit4_ssp_dev;

        if ((sns_regs_info->sns_type == ISP_SNS_I2C_TYPE) &&
            (sns_regs_info->com_bus.i2c_dev == -1)) {
            return HI_SUCCESS;
        }

        if ((sns_regs_info->sns_type == ISP_SNS_SSP_TYPE) &&
            (ssp_dev == -1)) {
            return HI_SUCCESS;
        }
    }

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_init != HI_NULL) {
        sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_init(vi_pipe);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_switch(hi_vi_pipe vi_pipe)
{
    hi_s8  ssp_dev;
    hi_s32 ret;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    /* if I2C or SSP Dev is -1, don't init sensor */
    {
        hi_isp_sns_regs_info *sns_regs_info = HI_NULL;

        ret = isp_sensor_update_sns_reg(vi_pipe);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        ret = isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        ssp_dev = sns_regs_info->com_bus.ssp_dev.bit4_ssp_dev;

        if ((sns_regs_info->sns_type == ISP_SNS_I2C_TYPE) &&
            (sns_regs_info->com_bus.i2c_dev == -1)) {
            return HI_SUCCESS;
        }

        if ((sns_regs_info->sns_type == ISP_SNS_SSP_TYPE) &&
            (ssp_dev == -1)) {
            return HI_SUCCESS;
        }
    }

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_init != HI_NULL) {
        sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_init(vi_pipe);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_sensor_exit(hi_vi_pipe vi_pipe)
{
    hi_s8  ssp_dev;
    hi_s32 ret;
    isp_sensor_ctx       *sensor_ctx    = HI_NULL;
    hi_isp_sns_regs_info *sns_regs_info = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_void_return(sensor_ctx);

    ret = isp_sensor_update_sns_reg(vi_pipe);
    if (ret != HI_SUCCESS) {
        return;
    }
    ret = isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);
    if (ret != HI_SUCCESS) {
        return;
    }
    ssp_dev = sns_regs_info->com_bus.ssp_dev.bit4_ssp_dev;

    if ((sns_regs_info->sns_type == ISP_SNS_I2C_TYPE) &&
        (sns_regs_info->com_bus.i2c_dev == -1)) {
        return;
    }

    if ((sns_regs_info->sns_type == ISP_SNS_SSP_TYPE) &&
        (ssp_dev == -1)) {
        return;
    }

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_exit != HI_NULL) {
        sensor_ctx->sns_register.sns_exp.pfn_cmos_sensor_exit(vi_pipe);
    } else {
        isp_err_trace("pfn_cmos_sensor_exit is null!\n");
    }
}

hi_s32 isp_sensor_update_blc(hi_vi_pipe vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;
    hi_isp_sensor_exp_func *sns_exp = HI_NULL;

    MPI_ASSERT(sizeof(ISP_CMOS_BLACK_LEVEL_S) == sizeof(hi_isp_cmos_black_level));
    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);
    sns_exp = &sensor_ctx->sns_register.sns_exp;

    if (sns_exp->pfn_cmos_get_isp_black_level != HI_NULL) {
        /* sensor should record the present iso, and calculate new black level. */
        sns_exp->pfn_cmos_get_isp_black_level(vi_pipe, &sensor_ctx->sns_black_level);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_update_default(hi_vi_pipe vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;
    hi_isp_sensor_exp_func *sns_exp = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    sns_exp = &sensor_ctx->sns_register.sns_exp;

    if (sns_exp->pfn_cmos_get_isp_default != HI_NULL) {
        sns_exp->pfn_cmos_get_isp_default(vi_pipe, &sensor_ctx->sns_dft);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_set_wdr_mode(hi_vi_pipe vi_pipe, hi_u8 mode)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_set_wdr_mode != HI_NULL) {
        if (sensor_ctx->sns_register.sns_exp.pfn_cmos_set_wdr_mode(vi_pipe, mode) != HI_SUCCESS) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_set_image_mode(hi_vi_pipe vi_pipe, hi_isp_cmos_sensor_image_mode *sns_image_mode)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;
    hi_isp_sensor_exp_func *sns_exp = HI_NULL;

    MPI_ASSERT(sizeof(ISP_CMOS_SENSOR_IMAGE_MODE_S) == sizeof(hi_isp_cmos_sensor_image_mode));
    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    sns_exp = &sensor_ctx->sns_register.sns_exp;

    if (sns_exp->pfn_cmos_set_image_mode != HI_NULL) {
        return sns_exp->pfn_cmos_set_image_mode(vi_pipe, sns_image_mode);
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_set_pixel_detect(hi_vi_pipe vi_pipe, hi_bool enable)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_set_pixel_detect != HI_NULL) {
        sensor_ctx->sns_register.sns_exp.pfn_cmos_set_pixel_detect(vi_pipe, enable);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_sensor_get_awb_gain(hi_vi_pipe vi_pipe, hi_u32 *sensor_awb_gain)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    if (sensor_ctx->sns_register.sns_exp.pfn_cmos_get_awb_gains != HI_NULL) {
        sensor_ctx->sns_register.sns_exp.pfn_cmos_get_awb_gains(vi_pipe, sensor_awb_gain);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
hi_s32 isp_sensor_update_sns_reg(hi_vi_pipe vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;
    hi_isp_sensor_exp_func *sns_exp = HI_NULL;

    MPI_ASSERT(sizeof(ISP_SNS_REGS_INFO_S) == sizeof(hi_isp_sns_regs_info));
    sensor_get_ctx(vi_pipe, sensor_ctx);
    isp_check_pointer_return(sensor_ctx);

    sns_exp = &sensor_ctx->sns_register.sns_exp;

    if (sns_exp->pfn_cmos_get_sns_reg_info != HI_NULL) {
        sns_exp->pfn_cmos_get_sns_reg_info(vi_pipe, &sensor_ctx->sns_reg_info);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

