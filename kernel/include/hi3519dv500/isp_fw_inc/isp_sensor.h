/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_SENSOR_H
#define ISP_SENSOR_H

#include "mkp_isp.h"
#include "ot_common_sns.h"
#include "ot_common_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 isp_sensor_reg_callback(ot_vi_pipe vi_pipe, ot_isp_sns_attr_info *sns_attr_info,
                               const ot_isp_sns_register *sns_register);
td_s32 isp_sensor_unreg_callback(ot_vi_pipe vi_pipe);

td_s32 isp_sensor_update_all(ot_vi_pipe vi_pipe);

td_s32 isp_sensor_get_id(ot_vi_pipe vi_pipe, ot_sensor_id *sns_id);
td_s32 isp_sensor_get_blc(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level **sns_black_level);
td_s32 isp_sensor_get_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default **sns_dft);
td_s32 isp_sensor_get_sns_reg(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info **sns_reg_info);
td_s32 isp_sensor_get_max_resolution(ot_vi_pipe vi_pipe, ot_isp_cmos_sns_max_resolution **sns_max_resolution);

td_s32 isp_sensor_ctx_init(ot_vi_pipe vi_pipe);
td_s32 isp_sensor_update_all_yuv(ot_vi_pipe vi_pipe);
td_s32 isp_get_yuv_default(ot_isp_cmos_default *sns_dft);
td_s32 isp_sensor_init(ot_vi_pipe vi_pipe);
td_s32 isp_sensor_switch(ot_vi_pipe vi_pipe);
td_void isp_sensor_exit(ot_vi_pipe vi_pipe);
td_s32 isp_sensor_update_blc(ot_vi_pipe vi_pipe);
td_s32 isp_sensor_update_default(ot_vi_pipe vi_pipe);
td_s32 isp_sensor_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode);
td_s32 isp_sensor_set_resolution_mode(ot_vi_pipe vi_pipe, td_u32 mode);
td_s32 isp_sensor_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable);
td_s32 isp_sensor_get_awb_gain(ot_vi_pipe vi_pipe, td_u32 *sns_awb_gain);
td_s32 isp_sensor_update_blc_clamp_info(ot_vi_pipe vi_pipe, td_bool *clamp_en);
td_s32 isp_sensor_update_sns_reg(ot_vi_pipe vi_pipe);
td_s32 isp_sensor_set_image_mode(ot_vi_pipe vi_pipe, ot_isp_cmos_sns_image_mode *sns_image_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
