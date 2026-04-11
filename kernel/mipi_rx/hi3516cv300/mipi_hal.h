#ifndef __MIPI_HAL__
#define __MIPI_HAL__
#include "hi_mipi.h"

/* int mask */
#define MIPI_CIL_INT_MASK      (0x00001f1f)

#define MIPI_CHN0_INT_MASK     (0x0000000f)     // disable ALIGN interrupt

#define MIPI_CTRL_INT_MASK     (0x00030003)

#define LVDS_CTRL0_INT_MASK    (0x00000010)

#define MIPI_FRAME_INT_MASK    (0x000f000f)     // disable err_f_seq_vc
#define MIPI_PKT_INT_MASK      (0x0001000f)

#define ALIGN0_INT_MASK        (0x00000001)
#define ALIGN1_INT_MASK        (0x00000001)

/* int def */
#define MIPI_FRAME_INT_DEF     (0x000f0f0f)
#define MIPI_PKT_INT1_DEF      (0x0001000f)
#define MIPI_PKT_INT2_DEF      (0x000f000f)

#define MIPI_CTRL_INT_DEF      (0x00030003)

#define LVDS_CTRL0_INT_DEF     (0x073fffff)
#define LVDS_CTRL1_INT_DEF     (0x01200e07)

#define ALIGN0_INT_DEF         (0x00001fff)
#define ALIGN1_INT_DEF         (0x0000001f)

/****************************************************************************
 * Struct DEFINITION                                                        *
 ****************************************************************************/
typedef unsigned int MIPI_PHY;

typedef struct
{
    short          link_idx;                      /* -1: disable */
    short          lane_num;                      /* total lane_num */
    short          lane_idx[LANE_NUM_PER_LINK];   /* -1: disable, lane_idx: [0, LANE_NUM_PER_LINK) */
}link_lane_s;

typedef struct
{
    HI_BOOL        b_order;                       /* 1: order, 0: dis order, no use in Hi3519 */
    short          link_num;                      /* total link num */
    short          total_lane_num;                /* total lane num */
    link_lane_s    link_arr[COMBO_MAX_LINK_NUM];
}link_info_s;


/****************************************************************************
 * fucntion declaration                                                     *
 ****************************************************************************/
void mipi_drv_reset_sensor(COMBO_DEV dev);
void mipi_drv_unreset_sensor(COMBO_DEV dev);
void mipi_drv_apb_reset(COMBO_DEV dev);
void mipi_drv_apb_unreset(COMBO_DEV dev);
void mipi_drv_core_reset(COMBO_DEV dev);
void mipi_drv_core_unreset(COMBO_DEV dev);
void mipi_drv_set_sys_work_mode(COMBO_DEV devno, input_mode_t input_mode);
work_mode_t mipi_drv_get_sys_work_mode(COMBO_DEV devno);


void mipi_drv_set_phy_te_hs_en(COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK]);

void mipi_drv_set_phy_chn_ctrl(COMBO_DEV devno, link_info_s* p_link_info);
void mipi_drv_clear_phy_chn_ctrl(COMBO_DEV devno, link_info_s* p_link_info);


void mipi_drv_set_vc(COMBO_LINK link_id, unsigned int vc_num);
void mipi_drv_set_di_dt(COMBO_LINK link_id, raw_data_type_e raw_data_type);
void mipi_drv_set_mipi_user_dt(COMBO_DEV devno, raw_data_type_e raw_data_type,
                                    short data_type[WDR_VC_NUM]);
void mipi_drv_set_mipi_dol_id(COMBO_DEV devno, raw_data_type_e raw_data_type,
                                    short dol_id[]);
void mipi_drv_set_mipi_wdr_mode(COMBO_DEV devno, mipi_wdr_mode_e wdr_mode);

void mipi_drv_set_mipi_image_rect(COMBO_DEV devno, img_rect_t *p_img_rect);
void mipi_drv_set_mipi_crop_en(COMBO_DEV devno, HI_BOOL en);

unsigned int mipi_drv_get_lanenum(COMBO_LINK link_id);
unsigned int mipi_drv_get_vc(COMBO_LINK link_id, unsigned int data_id);
raw_data_type_e mipi_drv_get_dt(COMBO_LINK link_id, unsigned int data_id);

void mipi_drv_set_output_clk_edge(MIPI_PHY mipi_phy, clk_edge clkedge);
void mipi_drv_set_output_msb(MIPI_PHY mipi_phy, output_msb data);

void mipi_drv_set_lane_num(COMBO_LINK link_id, unsigned int lane_num);
void mipi_drv_set_mem_cken(HI_BOOL en);

unsigned int mipi_drv_init_reg(void);
void mipi_drv_set_input_data_width(MIPI_PHY mipi_phy, unsigned datawidth);

/***************LVDS***********/
int  mipi_drv_set_lvds_image_rect(COMBO_DEV devno, img_rect_t* p_img_rect, short lane_num);
void mipi_drv_set_lvds_crop_en(COMBO_DEV devno, HI_BOOL en);

int mipi_drv_set_lvds_image_size(COMBO_DEV devno, img_size_t* p_img_size);
int mipi_drv_set_lvds_wdr_mode(COMBO_DEV devno, wdr_mode_e wdr_mode,
	                            lvds_vsync_type_t* vsync_type,
	                            lvds_fid_type_t* fid_type);

int mipi_drv_set_lvds_ctrl_mode(COMBO_DEV devno, lvds_sync_mode_e sync_mode,
                                raw_data_type_e raw_data_type,
                                lvds_bit_endian  data_endian,
                                lvds_bit_endian sync_code_endian,
                                HI_BOOL b_disorder);

int mipi_drv_set_link_lane_id(COMBO_DEV devno, COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK]);


int mipi_drv_set_lvds_sync_code(COMBO_DEV devno, unsigned int lane_cnt,
                    unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);
int mipi_drv_set_lvds_nxt_sync_code(COMBO_DEV devno, unsigned int lane_cnt,
                    unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);
void mipi_drv_set_dol_line_information(COMBO_DEV devno, wdr_mode_e wdr_mode,
                    raw_data_type_e raw_type);


void mipi_drv_set_lvds_phy_sync_code(COMBO_LINK link_id, short lane_idx[],
                    unsigned short   n_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
                    unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]);
void mipi_drv_set_phy_sync_dct(COMBO_LINK link_id, raw_data_type_e raw_data_type,
	                lvds_bit_endian code_endian);


int mipi_drv_set_phy_mode_hs_lp_te(input_mode_t input_mode, link_info_s *p_link_info);
int mipi_drv_set_phy_lvds_param(unsigned int phy_lvds_cyc);

void mipi_drv_set_phy_mode(COMBO_LINK link_id, work_mode_t work_mode, short lane_id[LANE_NUM_PER_LINK]);

void mipi_drv_set_phy_cmv(COMBO_LINK link_id, phy_cmv_e cmv_mode);
void mipi_drv_set_phy_cfg_en(COMBO_DEV devno, HI_BOOL en);

void mipi_drv_set_lvds_presync_clr_en(HI_BOOL en);
void mipi_drv_set_phy_ckgt_en(COMBO_LINK link_id, HI_BOOL en);

void mipi_drv_get_mipi_imgsize_statis(COMBO_DEV devno, unsigned int vc, img_size_t *p_size);
void mipi_drv_get_lvds_imgsize_statis(COMBO_DEV devno, unsigned int ef, img_size_t *p_size);
raw_data_type_e mipi_drv_get_lvds_raw_data_type(COMBO_DEV devno);


unsigned int mipi_drv_get_data(COMBO_LINK link_id);

/**************** interrupt ****************/
void mipi_drv_set_cil_int_mask(COMBO_LINK link_id, unsigned int mask);
void mipi_drv_set_chn_int_mask(COMBO_DEV devno, unsigned int mask);
void mipi_drv_set_mipi_ctrl_int_mask(COMBO_DEV devno, unsigned int mask);
void mipi_drv_set_lvds_ctrl_int_mask(COMBO_DEV devno, unsigned int mask);
void mipi_drv_set_mipi_pkt_int1_mask(COMBO_DEV devno, unsigned int mask);
void mipi_drv_set_mipi_frame_int_mask(COMBO_DEV devno, unsigned int mask);
void mipi_drv_set_align_int_mask(COMBO_DEV devno, unsigned int mask);

void mipi_drv_clear_cil_int(COMBO_LINK link_id, unsigned int intclr);
unsigned int mipi_drv_get_cil_raw_int(COMBO_LINK link_id);
unsigned int mipi_drv_get_cil_int(COMBO_LINK link_id);
void mipi_drv_clear_mipi_csi_int(COMBO_DEV devno, unsigned int intclr);
unsigned int mipi_drv_get_mipi_pkt_int1(COMBO_DEV devno);
unsigned int mipi_drv_get_mipi_pkt_int2(COMBO_DEV devno);
unsigned int mipi_drv_get_mipi_frame_int(COMBO_DEV devno);
void mipi_drv_clear_mipi_ctrl_int(COMBO_DEV devno, unsigned int clrint);
unsigned int mipi_drv_get_mipi_ctrl_raw_int(COMBO_DEV devno);
unsigned int mipi_drv_get_mipi_ctrl_int(COMBO_DEV devno);
void mipi_drv_clear_lvds_ctrl_int(COMBO_DEV devno, unsigned int clrint);
unsigned int mipi_drv_get_lvds_ctrl_raw_int(COMBO_DEV devno);
unsigned int mipi_drv_get_lvds_ctrl_int(COMBO_DEV devno);
void mipi_drv_clear_align_int(COMBO_DEV devno, unsigned int clrint);
unsigned int mipi_drv_get_align_raw_int(COMBO_DEV devno);
unsigned int mipi_drv_get_align_int(COMBO_DEV devno);

void mipi_exception_handling(int devno);


#endif
