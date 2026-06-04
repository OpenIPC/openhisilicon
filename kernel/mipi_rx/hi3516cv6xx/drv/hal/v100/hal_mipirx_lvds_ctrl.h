/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef HAL_MIPIRX_LVDS_CTRL_H
#define HAL_MIPIRX_LVDS_CTRL_H
#include "drv_mipirx_comm.h"
#include "drv_mipirx_osal.h"

typedef struct {
    td_u32 vsync_st;
    td_u32 link0_rd_err_st;
    td_u32 link1_rd_err_st;
    td_u32 link0_wr_err_st;
    td_u32 link1_wr_err_st;
    td_u32 lane0_sync_err_st;
    td_u32 lane1_sync_err_st;
    td_u32 lane2_sync_err_st;
    td_u32 lane3_sync_err_st;
} mipirx_lvds_err_cnt;

td_void hal_mipirx_lvds_ctrl_set_work_mode(td_u8 port_id);
td_void hal_mipirx_set_lvds_ctrl_mode(td_u8 port_id, mipirx_lvds_cfg *lvds_attr,
    mipirx_data_type input_data_type);
td_void hal_mipirx_set_lvds_data_rate(td_u8 port_id, mipirx_data_rate data_rate);
td_void hal_mipirx_lvds_ctrl_enable_clr_clock(td_u8 port_id, td_bool enable);
td_void hal_mipirx_lvds_ctrl_get_lane_img_size(td_u8 port_id, td_u8 lane_id, td_u32 *width, td_u32 *height);
td_void hal_mipirx_lvds_ctrl_set_cli_mode(td_u16 lane_bit_map);
td_void hal_mipirx_set_lvds_wdr_mode(td_u8 port_id, mipirx_lvds_wdr_attr *attr);
td_void hal_mipirx_lvds_ctrl_get_vc_img_size(td_u8 port_id, td_u8 vc, td_u32 *width, td_u32 *height);
td_void hal_mipirx_lvds_set_cropping_area(td_u8 port_id, mipirx_img_rect *img_size, td_s32 enable,
    td_u8 total_lane_num);
td_void hal_mipirx_lvds_ctrl_set_sync_code(td_u8 port_id, const td_s8 *lane_id,
    td_u16 sync_code[][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM], td_u8 lane_num);
td_void hal_mipirx_lvds_ctrl_set_nxt_sync_code(td_u8 port_id, const td_s8 *lane_id,
    const td_u16 nxt_sync_code[][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM], td_u8 lane_num, td_u32 shift_code);
td_void hal_mipirx_lvds_ctrl_count_err(td_u8 port_id);
mipirx_lvds_err_cnt *hal_mipi_get_lvds_err_cnt(td_u8 port_id);
td_void hal_mipirx_lvds_ctrl_err_irq_open(td_u8 port_id);
td_void hal_mipirx_lvds_ctrl_clean_err_cnt(td_void);
#endif
