/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: mipi_rx_hal.h
 * Author:
 * Create: 2016-10-07
 */

#ifndef __MIPI_HAL__
#define __MIPI_HAL__

#include "hi_mipi.h"

#define MIPI_RX_MAX_PHY_NUM 1

#define MIPI_CIL_INT_MASK (0x00003f3f)
#define MIPI_CTRL_INT_MASK (0x00030003)
#define LVDS_CTRL_INT_MASK \
	(0x0f110000) /* lvds_vsync_msk and lane0~3_sync_err_msk ignore, not err int */
#define MIPI_FRAME_INT_MASK (0x000f0000)
#define MIPI_PKT_INT1_MASK (0x0001000f)
#define MIPI_PKT_INT2_MASK (0x000f000f)
#define ALIGN0_INT_MASK (0x0000001f)

typedef struct {
	unsigned int clk1_fsm_timeout_err_cnt;
	unsigned int clk0_fsm_timeout_err_cnt;
	unsigned int d0_fsm_timeout_err_cnt;
	unsigned int d1_fsm_timeout_err_cnt;
	unsigned int d2_fsm_timeout_err_cnt;
	unsigned int d3_fsm_timeout_err_cnt;

	unsigned int clk1_fsm_escape_err_cnt;
	unsigned int clk0_fsm_escape_err_cnt;
	unsigned int d0_fsm_escape_err_cnt;
	unsigned int d1_fsm_escape_err_cnt;
	unsigned int d2_fsm_escape_err_cnt;
	unsigned int d3_fsm_escape_err_cnt;
} phy_err_int_cnt_t;

typedef struct {
	/* Packet err */
	unsigned int err_ecc_double_cnt;
	unsigned int vc3_err_crc_cnt;
	unsigned int vc2_err_crc_cnt;
	unsigned int vc1_err_crc_cnt;
	unsigned int vc0_err_crc_cnt;
	unsigned int vc3_err_ecc_corrected_cnt;
	unsigned int vc2_err_ecc_corrected_cnt;
	unsigned int vc1_err_ecc_corrected_cnt;
	unsigned int vc0_err_ecc_corrected_cnt;
	unsigned int err_id_vc3_cnt;
	unsigned int err_id_vc2_cnt;
	unsigned int err_id_vc1_cnt;
	unsigned int err_id_vc0_cnt;

	/* Frame err */
	unsigned int err_frame_data_vc3_cnt;
	unsigned int err_frame_data_vc2_cnt;
	unsigned int err_frame_data_vc1_cnt;
	unsigned int err_frame_data_vc0_cnt;
	unsigned int err_f_seq_vc3_cnt;
	unsigned int err_f_seq_vc2_cnt;
	unsigned int err_f_seq_vc1_cnt;
	unsigned int err_f_seq_vc0_cnt;
	unsigned int err_f_bndry_match_vc3_cnt;
	unsigned int err_f_bndry_match_vc2_cnt;
	unsigned int err_f_bndry_match_vc1_cnt;
	unsigned int err_f_bndry_match_vc0_cnt;

	/* Ctrl err */
	unsigned int data_fifo_rderr_cnt;
	unsigned int cmd_fifo_rderr_cnt;
	unsigned int data_fifo_wrerr_cnt;
	unsigned int cmd_fifo_wrerr_cnt;

} mipi_err_int_cnt_t;

typedef struct {
	unsigned int cmd_rd_err_cnt;
	unsigned int cmd_wr_err_cnt;
	unsigned int pop_err_cnt;
	unsigned int lvds_state_err_cnt;

	unsigned int link0_rd_err_cnt;
	unsigned int link0_wr_err_cnt;

	/* Lane sync err */
} lvds_err_int_cnt_t;

typedef struct {
	unsigned int lane3_align_err_cnt;
	unsigned int lane2_align_err_cnt;
	unsigned int lane1_align_err_cnt;
	unsigned int lane0_align_err_cnt;
	unsigned int fifo_full_err_cnt;
} align_err_int_cnt_t;
/* sensor function */
void sensor_drv_enable_clock(sns_clk_source_t sns_clk_source);
void sensor_drv_disable_clock(sns_clk_source_t sns_clk_source);

void sensor_drv_reset(sns_rst_source_t sns_reset_source);
void sensor_drv_unreset(sns_rst_source_t sns_reset_source);

/* mipi_rx function */
void mipi_rx_drv_set_work_mode(combo_dev_t devno, input_mode_t input_mode);
void mipi_rx_drv_set_mipi_image_rect(combo_dev_t devno, img_rect_t *p_img_rect);
void mipi_rx_drv_set_mipi_crop_en(combo_dev_t devno, int enable);
void mipi_rx_drv_set_di_dt(combo_dev_t devno, data_type_t input_data_type);
void mipi_rx_drv_set_mipi_yuv_dt(combo_dev_t devno,
				 data_type_t input_data_type);
void mipi_rx_drv_set_mipi_user_dt(combo_dev_t devno,
				  data_type_t input_data_type,
				  const short data_type[WDR_VC_NUM]);
void mipi_rx_drv_set_mipi_dol_id(combo_dev_t devno, data_type_t input_data_type,
				 short dol_id[]);
void mipi_rx_drv_set_mipi_wdr_mode(combo_dev_t devno, mipi_wdr_mode_t wdr_mode);
unsigned int mipi_rx_drv_get_phy_data(int phy_id, int lane_id);
unsigned int mipi_rx_drv_get_phy_mipi_link_data(int phy_id, int lane_id);
unsigned int mipi_rx_drv_get_phy_lvds_link_data(int phy_id, int lane_id);

void mipi_rx_drv_set_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate);
void mipi_rx_drv_set_link_lane_id(combo_dev_t devno, input_mode_t input_mode,
				  const short *p_lane_id);
void mipi_rx_drv_set_mem_cken(combo_dev_t devno, int enable);
void mipi_rx_drv_set_clr_cken(combo_dev_t devno, int enable);
void mipi_rx_drv_set_lane_num(combo_dev_t devno, unsigned int lane_num);
void mipi_rx_drv_set_phy_config(input_mode_t input_mode,
				unsigned int lane_bitmap);
void mipi_rx_drv_set_phy_cmvmode(input_mode_t input_mode,
				 phy_cmv_mode_t cmv_mode,
				 unsigned int lane_bitmap);
void mipi_rx_drv_set_phy_en(unsigned int lane_bitmap);
void mipi_rx_drv_set_cmos_en(unsigned int phy_id, int enable);
void mipi_rx_drv_set_lane_en(unsigned int lane_bitmap);
void mipi_rx_drv_set_phy_cil_en(unsigned int lane_bitmap, int enable);
void mipi_rx_drv_set_phy_cfg_mode(input_mode_t input_mode,
				  unsigned int lane_bitmap);
void mipi_rx_drv_set_phy_cfg_en(unsigned int lane_bitmap, int enable);
void mipi_rx_set_phy_rg_lp0_mode_en(unsigned int phy_id, int enable);
void mipi_rx_set_phy_rg_lp1_mode_en(unsigned int phy_id, int enable);

void mipi_rx_drv_set_lvds_image_rect(combo_dev_t devno, img_rect_t *p_img_rect,
				     short total_lane_num);
void mipi_rx_drv_set_lvds_crop_en(combo_dev_t devno, int enable);

int mipi_rx_drv_set_lvds_wdr_mode(combo_dev_t devno, wdr_mode_t wdr_mode,
				  lvds_vsync_attr_t *vsync_attr,
				  lvds_fid_attr_t *fid_attr);
void mipi_rx_drv_set_lvds_ctrl_mode(combo_dev_t devno,
				    lvds_sync_mode_t sync_mode,
				    data_type_t input_data_type,
				    lvds_bit_endian_t data_endian,
				    lvds_bit_endian_t sync_code_endian);

void mipi_rx_drv_set_lvds_data_rate(combo_dev_t devno,
				    mipi_data_rate_t data_rate);

void mipi_rx_drv_set_dol_line_information(combo_dev_t devno,
					  wdr_mode_t wdr_mode);
void mipi_rx_drv_set_lvds_sync_code(
	combo_dev_t devno, unsigned int lane_cnt,
	const short lane_id[LVDS_LANE_NUM],
	unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);

void mipi_rx_drv_set_lvds_nxt_sync_code(
	combo_dev_t devno, unsigned int lane_cnt,
	const short lane_id[LVDS_LANE_NUM],
	unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);

void mipi_rx_drv_set_phy_sync_config(
	lvds_dev_attr_t *p_attr, unsigned int lane_bitmap,
	unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);

int mipi_rx_drv_is_lane_valid(combo_dev_t devno, short lane_id,
			      lane_divide_mode_t mode);
void mipi_rx_drv_set_hs_mode(lane_divide_mode_t lane_divide_mode);

void mipi_rx_drv_get_mipi_imgsize_statis(combo_dev_t devno, short vc,
					 img_size_t *p_size);
void mipi_rx_drv_get_lvds_imgsize_statis(combo_dev_t devno, short vc,
					 img_size_t *p_size);
void mipi_rx_drv_get_lvds_lane_imgsize_statis(combo_dev_t devno, short lane,
					      img_size_t *p_size);

void mipi_rx_drv_set_mipi_int_mask(combo_dev_t devno);
void mipi_rx_drv_set_lvds_ctrl_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_mipi_ctrl_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_mipi_pkt1_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_mipi_pkt2_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_mipi_frame_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_align_int_mask(combo_dev_t devno, unsigned int mask);

void mipi_rx_drv_enable_clock(combo_dev_t combo_dev);
void mipi_rx_drv_disable_clock(combo_dev_t combo_dev);

void mipi_rx_drv_core_reset(combo_dev_t combo_dev);
void mipi_rx_drv_core_unreset(combo_dev_t combo_dev);

int mipi_rx_drv_init(void);
void mipi_rx_drv_exit(void);

#endif
