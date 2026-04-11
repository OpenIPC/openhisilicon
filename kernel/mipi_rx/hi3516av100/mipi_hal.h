#ifndef __MIPI_HAL__
#define __MIPI_HAL__
#include "hi_mipi.h"

/****************************************************************************
 * Struct DEFINITION                                                        *
 ****************************************************************************/
typedef struct
{
    short          link_idx;                      /* -1: disable */
    short          lane_num;                      /* total lane_num */
    short          lane_idx[LANE_NUM_PER_LINK];   /* -1: disable, lane_idx: [0, LANE_NUM_PER_LINK) */
} link_lane_s;

typedef struct
{
    bool           b_order;                       /* 1: order, 0: dis order  */
    short          link_num;                      /* total link num */
    short          total_lane_num;                /* total lane num */
    link_lane_s    link_arr[COMBO_MAX_LINK_NUM];
} link_info_s;

/****************************************************************************
 * fucntion declaration                                                     *
 ****************************************************************************/
void mipi_drv_set_csi2_rstn(COMBO_LINK link_id, unsigned int csi2_rstn);
void mipi_drv_set_phy_pwdn(COMBO_LINK link_id, unsigned int upwdn);
void mipi_drv_set_phy_rstn(COMBO_LINK link_id, unsigned int urstn);
void mipi_drv_set_mipi_mask(COMBO_LINK link_id, unsigned int mask);
void mipi_drv_set_csi2_mask1(COMBO_LINK link_id, unsigned int mask);
void mipi_drv_set_csi2_mask2(COMBO_LINK link_id, unsigned int mask);

void mipi_drv_set_phy_int_mask(MIPI_PHY mipi_phy, unsigned int mask);

void mipi_drv_set_phy_te_hs_en(COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK]);

void mipi_drv_set_phy_lp_ck_en(COMBO_LINK link_id, work_mode_t e_mode,
                               short lane_id[LANE_NUM_PER_LINK]);

void mipi_drv_set_vc(COMBO_LINK link_id, unsigned int vc_num);
void mipi_drv_set_di_dt(COMBO_LINK link_id, raw_data_type_e raw_data_type);

unsigned int mipi_drv_get_lanenum(COMBO_LINK link_id);
unsigned int mipi_drv_get_vc0(COMBO_LINK link_id);
unsigned int mipi_drv_get_vc1(COMBO_LINK link_id);
unsigned int mipi_drv_get_vc2(COMBO_LINK link_id);
unsigned int mipi_drv_get_vc3(COMBO_LINK link_id);
char* mipi_drv_get_dt(COMBO_LINK link_id);

char* mipi_drv_get_phymode(COMBO_LINK combo_link, MIPI_PHY mipi_phy);

void mipi_drv_set_output_clk_edge(MIPI_PHY mipi_phy, clk_edge clkedge);
void mipi_drv_set_output_msb(MIPI_PHY mipi_phy, output_msb data);
void mipi_drv_set_input_clk_edge(MIPI_PHY mipi_phy, clk_edge clkedge);

void mipi_drv_set_lane_num(COMBO_LINK link_id, unsigned int lane_num);
void mipi_drv_set_mem_cken(HI_BOOL en);

unsigned int  mipi_drv_get_mipi_raw_int1(COMBO_LINK link_id);
unsigned int mipi_drv_get_raw_int2(COMBO_LINK link_id);
unsigned int mipi_drv_get_mipi_int_status(COMBO_LINK link_id);
unsigned int mipi_drv_get_link_int_status(COMBO_LINK link_id);

void mipi_drv_set_phy_int_clr(MIPI_PHY mipi_phy, unsigned int intclr);
unsigned int mipi_drv_init_reg(void);
void mipi_drv_set_phy_mode(MIPI_PHY mipi_phy, work_mode_t mode);
void mipi_drv_set_input_data_width(MIPI_PHY mipi_phy, unsigned datawidth);

unsigned int mipi_drv_get_lvds_int_raw(COMBO_LINK link_id);
void mipi_drv_set_link_int_clr(COMBO_LINK link_id, unsigned intclr);

int mipi_drv_set_phy_reg_start(HI_BOOL en);

void set_combo_dev_attr(int type);

void mipi_drv_set_link_mask(COMBO_LINK link_id, unsigned int mask);

/***************LVDS***********/
int mipi_set_lvds_dev_attr(input_mode_t input_mode, lvds_dev_attr_t* p_attr);
int mipi_drv_set_lvds_image_size(img_size_t* p_img_size);
int mipi_drv_set_lvds_wdr_mode(wdr_mode_e wdr_mode);

int mipi_drv_set_lvds_ctrl_mode(lvds_sync_mode_e sync_mode,
                                raw_data_type_e raw_data_type,
                                lvds_bit_endian  data_endian,
                                lvds_bit_endian sync_code_endian,
                                HI_BOOL b_disorder);

int mipi_drv_set_link_lane_id(COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK]);

int mipi_drv_set_lvds_sync_code(COMBO_LINK link_id,
                                unsigned short sync_code[LANE_NUM_PER_LINK][WDR_VC_NUM][SYNC_CODE_NUM]);

int mipi_drv_set_phy_mode_hs_lp_te(input_mode_t input_mode, link_info_s* p_link_info);
int mipi_drv_set_phy_lvds_param(unsigned int phy_lvds_cyc);

void mipi_drv_init_phy_cfg(COMBO_LINK link_id);
void mipi_drv_set_phy_cfg(COMBO_LINK link_id, work_mode_t work_mode,
                          short lane_id[LANE_NUM_PER_LINK]);

void mipi_drv_get_image_size(COMBO_LINK link_id, img_size_t* p_size);

unsigned int mipi_drv_get_data(COMBO_LINK link_id);

#endif
