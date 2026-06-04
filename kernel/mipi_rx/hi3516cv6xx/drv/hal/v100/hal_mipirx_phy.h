/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef HAL_MIPIRX_PHY_H
#define HAL_MIPIRX_PHY_H

#include "drv_mipirx_comm.h"

typedef struct {
    td_u32 clk_fsm_tmout_err_cnt;
    td_u32 d0_fsm_tmout_err_cnt;
    td_u32 d1_fsm_tmout_err_cnt;
    td_u32 d2_fsm_tmout_err_cnt;
    td_u32 d3_fsm_tmout_err_cnt;

    td_u32 clk_fsm_escape_err_cnt;
    td_u32 d0_fsm_escape_err_cnt;
    td_u32 d1_fsm_escape_err_cnt;
    td_u32 d2_fsm_escape_err_cnt;
    td_u32 d3_fsm_escape_err_cnt;
} mipirx_phy_err_cnt;

td_void hal_mipirx_phy_set_lane_mode(mipirx_lane_mode lane_mode);
td_void hal_mipirx_phy_set_mode_link(td_u8 phy_id, mipirx_lane_mode lane_mode);
td_void hal_mipirx_phy_set_skew(td_u8 phy_id);

td_void hal_mipirx_phy_cfg_lanes(td_u8 phy_id, td_u8 phy_lane_bit);

td_void hal_mipirx_phy_set_work_mode(td_u8 phy_id, mipirx_work_mode work_mode);

td_void hal_mipirx_phy_set_sync_code(td_u8 phy_id, td_s8 *lane_id,
    const td_u16 sync_code[][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM], td_u8 lane_num, td_u32 shift_code);

td_void hal_mipirx_phy_set_sync_dct(td_u8 phy_id, mipirx_data_type data_type,
    mipirx_lvds_endian data_endian, td_u32 lane_bitmap);

td_void hal_mipirx_phy_get_mipi_data(td_u8 phy_id, td_u32 *mipi_lane_data, td_u16 size);

td_void hal_mipirx_phy_get_phy_data(td_u8 phy_id, td_u32 *phy_lane_data, td_u16 size);

td_void hal_mipirx_phy_set_eq(td_u8 phy_id, td_u32 eq);

td_void hal_mipirx_phy_get_mipi_ph_data(td_u8 phy_id, td_u32 *data, td_u16 size);

td_char *hal_mipirx_phy_get_lane_mode(td_void);

td_bool hal_mipirx_phy_get_enable_stat(td_u8 phy_id);

td_bool hal_mipirx_phy_get_lane_term_on(td_u8 phy_id, td_s32 lane_id);

td_bool hal_mipirx_phy_get_lane_en(td_s32 lane_id);

td_char *hal_mipirx_phy_get_lane_cur_fsm_stat(td_u32 phy_id, td_s16 lane_id);

td_char *hal_mipirx_phy_get_lane_next_fsm_stat(td_u8 phy_id, td_s16 lane_id);

td_char *hal_mipirx_phy_get_clk_cur_fsm_stat(td_u8 phy_id, td_u32 clk_num);

td_char *hal_mipirx_phy_get_clk_next_fsm_stat(td_u8 phy_id, td_u32 clk_num);

td_void hal_mipirx_phy_get_lvds_data(td_u8 phy_id, td_u32 *data, td_u32 size);

td_void hal_mipirx_phy_get_lvds_pix_num(td_u8 phy_id, td_u32 *data, td_u32 size);

td_void hal_mipirx_phy_count_err_irq(td_u8 phy_id);
mipirx_phy_err_cnt *hal_mipi_get_phy_err_cnt_ctx(td_u8 phy_id);
td_void hal_mipirx_phy_clean_err_cnt(td_void);

td_void hal_mipirx_phy_color_bar(td_bool enable, td_bool mipi_mode, td_bool is_stripe);
td_void hal_mipirx_phy_irq_open(td_u8 phy_id);
td_u32 hal_mipirx_phy_get_freq(td_u8 phy_id);
#endif
