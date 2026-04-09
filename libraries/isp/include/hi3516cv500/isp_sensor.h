/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_sensor.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_SENSOR_H__
#define __ISP_SENSOR_H__

#include "mkp_isp.h"
#include "hi_comm_sns_adapt.h"
#include "hi_comm_isp_adapt.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_s32 isp_sensor_reg_callback(hi_vi_pipe vi_pipe, hi_isp_sns_attr_info *sns_attr_info,
                               hi_isp_sensor_register *sns_register);
hi_s32 isp_sensor_unreg_callback(hi_vi_pipe vi_pipe);

hi_s32 isp_sensor_update_all(hi_vi_pipe vi_pipe);

hi_s32 isp_sensor_get_id(hi_vi_pipe vi_pipe, SENSOR_ID *sensor_id);
hi_s32 isp_sensor_get_blc(hi_vi_pipe vi_pipe, hi_isp_cmos_black_level **sns_black_level);
hi_s32 isp_sensor_get_default(hi_vi_pipe vi_pipe, hi_isp_cmos_default **sns_dft);
hi_s32 isp_sensor_get_sns_reg(hi_vi_pipe vi_pipe, hi_isp_sns_regs_info **sns_reg_info);
hi_s32 isp_sensor_get_max_resolution(hi_vi_pipe vi_pipe, hi_isp_cmos_sensor_max_resolution **sns_max_resolution);

hi_s32 isp_sensor_ctx_init(hi_vi_pipe vi_pipe);
hi_s32 isp_sensor_update_all_yuv(hi_vi_pipe vi_pipe);
hi_s32 isp_get_yuv_default(hi_isp_cmos_default *sns_dft);
hi_s32 isp_sensor_init(hi_vi_pipe vi_pipe);
hi_s32 isp_sensor_switch(hi_vi_pipe vi_pipe);
hi_void isp_sensor_exit(hi_vi_pipe vi_pipe);
hi_s32 isp_sensor_update_blc(hi_vi_pipe vi_pipe);
hi_s32 isp_sensor_update_default(hi_vi_pipe vi_pipe);
hi_s32 isp_sensor_set_wdr_mode(hi_vi_pipe vi_pipe, hi_u8 mode);
hi_s32 isp_sensor_set_resolution_mode(hi_vi_pipe vi_pipe, hi_u32 mode);
hi_s32 isp_sensor_set_pixel_detect(hi_vi_pipe vi_pipe, hi_bool enable);
hi_s32 isp_sensor_get_awb_gain(hi_vi_pipe vi_pipe, hi_u32 *sensor_awb_gain);
hi_s32 isp_sensor_update_sns_reg(hi_vi_pipe vi_pipe);
hi_s32 isp_sensor_set_image_mode(hi_vi_pipe vi_pipe, hi_isp_cmos_sensor_image_mode *sns_image_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
