/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef HAL_MIPIRX_MIPI_CTRL_H
#define HAL_MIPIRX_MIPI_CTRL_H
#include "drv_mipirx_comm.h"

typedef struct {
    td_u32 vc0_err_crc_cnt;
    td_u32 vc1_err_crc_cnt;
    td_u32 vc2_err_crc_cnt;
    td_u32 vc3_err_crc_cnt;
    td_u32 vc0_err_ecc_cnt;
    td_u32 vc1_err_ecc_cnt;
    td_u32 vc2_err_ecc_cnt;
    td_u32 vc3_err_ecc_cnt;
    td_u32 err_ecc_mult_cnt;

    td_u32 vc0_err_frame_num_cnt;
    td_u32 vc1_err_frame_num_cnt;
    td_u32 vc2_err_frame_num_cnt;
    td_u32 vc3_err_frame_num_cnt;
    td_u32 vc0_err_frame_s_e_num_mismatch_cnt;
    td_u32 vc1_err_frame_s_e_num_mismatch_cnt;
    td_u32 vc2_err_frame_s_e_num_mismatch_cnt;
    td_u32 vc3_err_frame_s_e_num_mismatch_cnt;

    td_u32 data_fifo_rderr_cnt;
    td_u32 cmd_fifo_rderr_cnt;
    td_u32 vsync_cnt;
    td_u32 data_fifo_wrerr_cnt;
    td_u32 cmd_fifo_wrerr_cnt;
    td_u32 cmd_fifo_not_empty_cnt;
    td_u32 data_fifo_not_empty_cnt;
} mipirx_mipi_err_cnt;

typedef struct {
    td_u32 fifo_ovfl;
    td_u32 lane0_fifo_ovfl;
    td_u32 lane1_fifo_ovfl;
    td_u32 lane2_fifo_ovfl;
    td_u32 lane3_fifo_ovfl;
} mipirx_align_err;

td_void hal_mipirx_ctrl_set_work_mode(td_u8 port_id);
td_void hal_mipirx_mipi_ctrl_set_mipi_dt(td_u8 port_id, mipirx_data_type input_data_type);
td_void hal_mipirx_mipi_ctrl_set_wdr_mode(td_u8 port_id, const mipirx_mipi_cfg *mipi_dev,
    mipirx_data_type input_data_type);
td_void hal_mipirx_mipi_ctrl_set_data_rate(td_u8 port_id, mipirx_data_rate data_rate);
td_void hal_mipirx_mipi_ctrl_enable_mem_clk(td_u8 port_id, td_bool enable);
td_void hal_mipirx_mipi_ctrl_set_lane_id(td_u8 port_id, const td_s8 *p_lane_id, td_u8 size);
td_void hal_mipirx_mipi_ctrl_set_cli_mode(td_u16 lane_bit_map);
td_void hal_mipirx_mipi_ctrl_set_rect(td_u8 port_id, const mipirx_img_rect *p_img_rect, td_bool enable);
td_void hal_mipirx_mipi_ctrl_set_user_defined_dt(td_u8 port_id, const mipirx_mipi_udf_dt *usr_data_type,
    mipirx_data_type input_data_type);
td_u32 hal_mipirx_ctrl_get_lane_map(td_u8 port_id, td_s16 lane_idx);
td_void hal_mipirx_mipi_ctrl_get_mipi_imgsize(td_u8 port_id, td_u8 vc, mipirx_img_rect *img_size);
td_void hal_mipirx_mipi_ctrl_count_err(td_u8 port_id);
mipirx_mipi_err_cnt *hal_mipi_get_ctl_err_cnt_ctx(td_u8 port_id);
mipirx_align_err *hal_mipi_get_align_err_ctx(td_u8 port_id);
td_void hal_mipirx_align_err(td_u8 port_id);
td_void hal_mipirx_align_irq_open(td_u8 port_id);
td_void hal_mipirx_mipi_ctrl_irq_open(td_u8 port_id);
td_void hal_mipirx_mipi_ctrl_clean_err_cnt(td_void);
#endif
