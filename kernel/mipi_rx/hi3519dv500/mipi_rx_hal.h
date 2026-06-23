/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_RX_HAL_H
#define MIPI_RX_HAL_H

#include "ot_mipi_rx.h"
#include "mipi_rx_reg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MIPI_RX_MAX_PHY_NUM                    2
#define MIPI_RX_PHY_LANE_NUM                   4
#define MIPI_RX_MAX_EXT_DATA_TYPE_BIT_WIDTH    16
#define MIPI_RX_MIN_EXT_DATA_TYPE_BIT_WIDTH    8
#define MIPI_RX_EXT_DATA_TYPE_EM_BIT_WIDTH     0

#define MIPI_CIL_INT_MASK        0x00003f3f
#define MIPI_CTRL_INT_MASK_FS    0x00030013 // turn on vsync_raw
#define MIPI_CTRL_INT_MASK       0x00030003
#define LVDS_CTRL_INT_MASK       0x0f770fff
#define MIPI_CSI_INT_MASK        0x000001ff
#define ALIGN0_INT_MASK          0x00001fff

#define MIPI_RX_START_FRAME_CNT  2
#define MIPI_RX_MAX_START_FRAME_CNT    10
#define MIPI_RX_MAX_RECOVER_CNT  10

typedef struct {
    unsigned int clk2_fsm_timeout_err_cnt;
    unsigned int clk_fsm_timeout_err_cnt;
    unsigned int d0_fsm_timeout_err_cnt;
    unsigned int d1_fsm_timeout_err_cnt;
    unsigned int d2_fsm_timeout_err_cnt;
    unsigned int d3_fsm_timeout_err_cnt;

    unsigned int clk2_fsm_escape_err_cnt;
    unsigned int clk_fsm_escape_err_cnt;
    unsigned int d0_fsm_escape_err_cnt;
    unsigned int d1_fsm_escape_err_cnt;
    unsigned int d2_fsm_escape_err_cnt;
    unsigned int d3_fsm_escape_err_cnt;
} phy_err_int_cnt_t;

typedef struct {
    /* Crc_intr err */
    unsigned int vc0_err_crc_cnt;
    unsigned int vc1_err_crc_cnt;
    unsigned int vc2_err_crc_cnt;
    unsigned int vc3_err_crc_cnt;
    unsigned int vc0_err_ecc_corrected_cnt;
    unsigned int vc1_err_ecc_corrected_cnt;
    unsigned int vc2_err_ecc_corrected_cnt;
    unsigned int vc3_err_ecc_corrected_cnt;
    unsigned int err_ecc_double_cnt;

    unsigned int vc0_err_frame_num_cnt;
    unsigned int vc1_err_frame_num_cnt;
    unsigned int vc2_err_frame_num_cnt;
    unsigned int vc3_err_frame_num_cnt;
    unsigned int vc0_err_frame_s_e_num_mismatch_cnt;
    unsigned int vc1_err_frame_s_e_num_mismatch_cnt;
    unsigned int vc2_err_frame_s_e_num_mismatch_cnt;
    unsigned int vc3_err_frame_s_e_num_mismatch_cnt;

    /* Ctrl err */
    unsigned int data_fifo_rderr_cnt;
    unsigned int cmd_fifo_rderr_cnt;
    unsigned int vsync_cnt;
    unsigned int data_fifo_wrerr_cnt;
    unsigned int cmd_fifo_wrerr_cnt;
} mipi_err_int_cnt_t;

typedef struct {
    unsigned int lvds_vsync_cnt;
    unsigned int cmd_rd_err_cnt;
    unsigned int cmd_wr_err_cnt;
    unsigned int pop_err_cnt;
    unsigned int lvds_state_err_cnt;

    unsigned int link2_rd_err_cnt;
    unsigned int link1_rd_err_cnt;
    unsigned int link0_rd_err_cnt;
    unsigned int link2_wr_err_cnt;
    unsigned int link1_wr_err_cnt;
    unsigned int link0_wr_err_cnt;

    /* Lane sync err */
    unsigned int lane7_sync_err_cnt;
    unsigned int lane6_sync_err_cnt;
    unsigned int lane5_sync_err_cnt;
    unsigned int lane4_sync_err_cnt;
    unsigned int lane3_sync_err_cnt;
    unsigned int lane2_sync_err_cnt;
    unsigned int lane1_sync_err_cnt;
    unsigned int lane0_sync_err_cnt;
} lvds_err_int_cnt_t;

typedef struct {
    unsigned int lane7_align_err_cnt;
    unsigned int lane6_align_err_cnt;
    unsigned int lane5_align_err_cnt;
    unsigned int lane4_align_err_cnt;
    unsigned int lane3_align_err_cnt;
    unsigned int lane2_align_err_cnt;
    unsigned int lane1_align_err_cnt;
    unsigned int lane0_align_err_cnt;
    unsigned int fifo_full_err_cnt;
} align_err_int_cnt_t;

void mipi_rx_drv_set_irq_num(unsigned int irq_num);
void mipi_rx_drv_set_regs(mipi_rx_regs_type_t *regs);

phy_err_int_cnt_t *mipi_rx_drv_get_phy_err_int_cnt(unsigned int phy_id);
mipi_err_int_cnt_t *mipi_rx_drv_get_mipi_err_int_cnt(combo_dev_t devno);
lvds_err_int_cnt_t *mipi_rx_drv_get_lvds_err_int_cnt(combo_dev_t devno);
align_err_int_cnt_t *mipi_rx_drv_get_align_err_int_cnt(combo_dev_t devno);
unsigned int mipi_rx_drv_get_vsync_cnt(combo_dev_t devno);
bool mipi_rx_check_ext_cfg(void);

void mipi_rx_drv_get_phy_skew_length(int phy_id, unsigned int *length0, unsigned int *length1);

/* sensor function */
void set_drv_sns_crg_addr(unsigned long addr);
void sensor_drv_enable_clock(sns_clk_source_t sns_clk_source);
void sensor_drv_disable_clock(sns_clk_source_t sns_clk_source);

void sensor_drv_reset(sns_rst_source_t sns_reset_source);
void sensor_drv_unreset(sns_rst_source_t sns_reset_source);

/* mipi_rx function */
void mipi_rx_drv_set_work_mode(combo_dev_t devno, input_mode_t input_mode);
void mipi_rx_drv_set_mipi_image_rect(combo_dev_t devno, const img_rect_t *p_img_rect);
void mipi_rx_drv_set_mipi_crop_en(combo_dev_t devno, int enable);
void mipi_rx_drv_set_mipi_yuv_dt(combo_dev_t devno, data_type_t input_data_type);
void mipi_rx_drv_set_mipi_wdr_user_dt(combo_dev_t devno, data_type_t input_data_type,
                                      const short data_type[WDR_VC_NUM]);
void mipi_rx_drv_set_mipi_dol_id(combo_dev_t devno, data_type_t input_data_type, const short dol_id[]);
void mipi_rx_drv_set_mipi_wdr_mode(combo_dev_t devno, mipi_wdr_mode_t wdr_mode);
unsigned int mipi_rx_drv_get_phy_data(int phy_id, int lane_id);
unsigned int mipi_rx_drv_get_phy_mipi_link_data(int phy_id, int lane_id);
unsigned int mipi_rx_drv_get_phy_freq(int phy_id);
unsigned int mipi_rx_drv_get_phy_lvds_link_data(int phy_id, int lane_id);

void mipi_rx_drv_set_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate);
void mipi_rx_drv_set_link_lane_id(combo_dev_t devno, input_mode_t input_mode, const short *p_lane_id);
void mipi_rx_drv_set_mem_cken(combo_dev_t devno, int enable);
void mipi_rx_drv_set_clr_cken(combo_dev_t devno, int enable);
void mipi_rx_drv_set_phy_config(combo_dev_t devno, input_mode_t input_mode,
                                unsigned int lane_bitmap, unsigned int lane_mask);
void mipi_rx_drv_reset_cil_int(void);
void mipi_rx_drv_reset_phy_mode(void);
void mipi_rx_drv_set_phy_cmvmode(input_mode_t input_mode, phy_cmv_mode_t cmv_mode, unsigned int lane_bitmap);
void mipi_rx_drv_set_ext_data_type(const ext_data_type_t* data_type, data_type_t input_data_type);
void mipi_rx_drv_set_speed_mode(combo_dev_t combo_dev, phy_speed_mode_t speed_mode);
void mipi_rx_drv_get_speed_mode(combo_dev_t combo_dev, phy_speed_mode_t *speed_mode);

void mipi_rx_drv_set_lvds_image_rect(combo_dev_t devno, const img_rect_t *p_img_rect, short total_lane_num);
void mipi_rx_drv_set_lvds_crop_en(combo_dev_t devno, int enable);

int mipi_rx_drv_set_lvds_wdr_mode(combo_dev_t devno, lvds_wdr_mode_t wdr_mode,
                                  const lvds_vsync_attr_t *vsync_attr, const lvds_fid_attr_t *fid_attr);
void mipi_rx_drv_set_lvds_ctrl_mode(combo_dev_t devno, lvds_sync_mode_t sync_mode,
                                    data_type_t input_data_type,
                                    lvds_bit_endian_t data_endian,
                                    lvds_bit_endian_t sync_code_endian);

void mipi_rx_drv_set_lvds_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate);

void mipi_rx_drv_set_dol_line_information(combo_dev_t devno, lvds_wdr_mode_t wdr_mode);
void mipi_rx_drv_set_lvds_sync_code(combo_dev_t devno, unsigned int lane_cnt, const short lane_id[LVDS_LANE_NUM],
                                    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);

void mipi_rx_drv_set_lvds_nxt_sync_code(combo_dev_t devno, unsigned int lane_cnt, const short lane_id[LVDS_LANE_NUM],
                                        const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);

void mipi_rx_drv_set_phy_sync_config(const lvds_dev_attr_t *p_attr, unsigned int lane_bitmap,
                                     const unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);

unsigned int mipi_rx_drv_get_lane_mask(combo_dev_t devno, lane_divide_mode_t mode);
int mipi_rx_drv_is_lane_valid(combo_dev_t devno, short lane_id, lane_divide_mode_t mode);
void mipi_rx_drv_set_hs_mode(lane_divide_mode_t mode);

void mipi_rx_drv_get_mipi_imgsize_statis(combo_dev_t devno, short vc, img_size_t *p_size);
void mipi_rx_drv_get_lvds_imgsize_statis(combo_dev_t devno, short vc, img_size_t *p_size);
void mipi_rx_drv_get_lvds_lane_imgsize_statis(combo_dev_t devno, short lane, img_size_t *p_size);

void mipi_rx_drv_set_chn_int_mask(combo_dev_t devno);
void mipi_rx_drv_set_lvds_ctrl_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_mipi_ctrl_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_mipi_csi_int_mask(combo_dev_t devno, unsigned int mask);
void mipi_rx_drv_set_align_int_mask(combo_dev_t devno, unsigned int mask);

void mipi_rx_drv_enable_clock(combo_dev_t combo_dev);
void mipi_rx_drv_disable_clock(combo_dev_t combo_dev);

void mipi_rx_drv_core_reset(combo_dev_t combo_dev);
void mipi_rx_drv_core_unreset(combo_dev_t combo_dev);

int mipi_rx_drv_init(void);
void mipi_rx_drv_exit(void);

bool mipi_rx_chip_is_hi3516dv500(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MIPI_RX_HAL_H */
