/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef MIPIRX_KAPI_H
#define MIPIRX_KAPI_H

#include "drv_mipirx_kapi_struct.h"

td_s32 drv_mipirx_kapi_init(td_void *pdev);
td_void drv_mipirx_kapi_deinit(td_void);
#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_bool drv_mipirx_is_init(td_void);
mipirx_lane_mode drv_mipirx_get_lane_mode(td_void);
td_s32 drv_mipirx_kapi_resume_set_lane_mode(td_void);
td_void drv_mipirx_kapi_crg_enable_sys_clock(td_bool enable);
#endif
td_s32 drv_mipirx_kapi_set_lane_mode(mipirx_lane_mode lane_mode);
td_s32 drv_mipirx_kapi_enable_mipi_clock(td_u8 port_id, td_bool enable);
td_s32 drv_mipirx_kapi_reset_mipi(td_u8 port_id, td_bool reset);
#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_s32 drv_mipirx_kapi_resume_set_attr(td_u8 port_id);
#endif
td_s32 drv_mipirx_kapi_set_attr(mipirx_port_cfg *attr);
td_s32 drv_mipirx_kapi_set_phy_mode(td_u8 port_id, mipirx_work_mode mode);
#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_s32 drv_mipirx_kapi_resume_set_user_def_dt(td_u8 port_id);
#endif
td_s32 drv_mipirx_kapi_set_user_def_dt(td_u8 port_id, mipirx_mipi_udf_dt *data_type);
td_s32 drv_mipirx_kapi_enable_sensor_clock(td_u8 port_id, td_bool enable);
td_s32 drv_mipirx_kapi_reset_sensor(td_u8 port_id, td_bool reset);

#endif
