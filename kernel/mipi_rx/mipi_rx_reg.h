/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: mipi_rx_reg.h
 * Author:
 * Create: 2016-10-07
 */

#ifndef __MIPI_RX_REG_H__
#define __MIPI_RX_REG_H__

/* Define the union U_PHY_MODE_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy0_rg_en_d : 4; /* [3..0]  */
		unsigned int phy0_rg_en_clk0 : 1; /* [4]  */
		unsigned int phy0_rg_en_clk1 : 1; /* [5]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int phy0_rg_en_cmos : 1; /* [8]  */
		unsigned int reserved_1 : 3; /* [11..9]  */
		unsigned int phy0_rg_en_lp0 : 1; /* [12]  */
		unsigned int phy0_rg_en_lp1 : 1; /* [13]  */
		unsigned int reserved_2 : 2; /* [15..14]  */
		unsigned int phy0_rg_faclk0_en : 1; /* [16]  */
		unsigned int phy0_rg_faclk1_en : 1; /* [17]  */
		unsigned int reserved_3 : 2; /* [19..18]  */
		unsigned int phy0_rg_en_2l2l : 1; /* [20]  */
		unsigned int reserved_4 : 3; /* [23..21]  */
		unsigned int phy0_rg_mipi_mode0 : 1; /* [24]  */
		unsigned int phy0_rg_mipi_mode1 : 1; /* [25]  */
		unsigned int reserved_5 : 6; /* [31..26]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_MODE_LINK;

/* Define the union U_PHY_SKEW_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy0_clk0_skew : 3; /* [2..0]  */
		unsigned int reserved_0 : 1; /* [3]  */
		unsigned int phy0_clk1_skew : 3; /* [6..4]  */
		unsigned int reserved_1 : 1; /* [7]  */
		unsigned int phy0_d0_skew : 3; /* [10..8]  */
		unsigned int reserved_2 : 1; /* [11]  */
		unsigned int phy0_d1_skew : 3; /* [14..12]  */
		unsigned int reserved_3 : 1; /* [15]  */
		unsigned int phy0_d2_skew : 3; /* [18..16]  */
		unsigned int reserved_4 : 1; /* [19]  */
		unsigned int phy0_d3_skew : 3; /* [22..20]  */
		unsigned int reserved_5 : 9; /* [31..23]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SKEW_LINK;

/* Define the union U_PHY_EN_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy_da_d0_valid : 1; /* [0]  */
		unsigned int phy_da_d1_valid : 1; /* [1]  */
		unsigned int phy_da_d2_valid : 1; /* [2]  */
		unsigned int phy_da_d3_valid : 1; /* [3]  */
		unsigned int reserved_0 : 4; /* [7..4]  */
		unsigned int phy_d0_term_en : 1; /* [8]  */
		unsigned int phy_d1_term_en : 1; /* [9]  */
		unsigned int phy_d2_term_en : 1; /* [10]  */
		unsigned int phy_d3_term_en : 1; /* [11]  */
		unsigned int phy_clk0_term_en : 1; /* [12]  */
		unsigned int phy_clk1_term_en : 1; /* [13]  */
		unsigned int reserved_1 : 18; /* [31..14]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_EN_LINK;

/* Define the union U_PHY_CFG_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy0_da_ulpb : 4; /* [3..0]  */
		unsigned int phy0_da_ulpb_ck0 : 1; /* [4]  */
		unsigned int phy0_da_ulpb_ck1 : 1; /* [5]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int phy0_rg_it_lp : 2; /* [9..8]  */
		unsigned int phy0_rg_it_ck : 2; /* [11..10]  */
		unsigned int phy0_rg_it_dt : 2; /* [13..12]  */
		unsigned int phy0_rg_sa_en : 1; /* [14]  */
		unsigned int reserved_1 : 17; /* [31..15]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_CFG_LINK;

/* Define the union U_PHY_DATA_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy_data0_mipi : 8; /* [7..0]  */
		unsigned int phy_data1_mipi : 8; /* [15..8]  */
		unsigned int phy_data2_mipi : 8; /* [23..16]  */
		unsigned int phy_data3_mipi : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_DATA_LINK;

/* Define the union U_PHY_PH_MIPI_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy_ph0_mipi : 8; /* [7..0]  */
		unsigned int phy_ph1_mipi : 8; /* [15..8]  */
		unsigned int phy_ph2_mipi : 8; /* [23..16]  */
		unsigned int phy_ph3_mipi : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_PH_MIPI_LINK;

/* Define the union U_PHY_DATA_MIPI_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy_data0_mipi_hs : 8; /* [7..0]  */
		unsigned int phy_data1_mipi_hs : 8; /* [15..8]  */
		unsigned int phy_data2_mipi_hs : 8; /* [23..16]  */
		unsigned int phy_data3_mipi_hs : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_DATA_MIPI_LINK;

/* Define the union U_PHY_SYNC_DCT_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_raw_type0 : 3; /* [2..0]  */
		unsigned int reserved_0 : 5; /* [7..3]  */
		unsigned int cil_code_big_endian0 : 1; /* [8]  */
		unsigned int reserved_1 : 3; /* [11..9]  */
		unsigned int cil_raw_type1 : 3; /* [14..12]  */
		unsigned int reserved_2 : 5; /* [19..15]  */
		unsigned int cil_code_big_endian1 : 1; /* [20]  */
		unsigned int reserved_3 : 4; /* [24..21]  */
		unsigned int cil_sync_det_mode : 1; /* [25]  */
		unsigned int reserved_4 : 6; /* [31..26]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_DCT_LINK;

/* Define the union U_PHY_SYNC_SOF0_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sof0_word4_0 : 16; /* [15..0]  */
		unsigned int cil_sof1_word4_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOF0_LINK;

/* Define the union U_PHY_SYNC_SOF1_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sof0_word4_1 : 16; /* [15..0]  */
		unsigned int cil_sof1_word4_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOF1_LINK;

/* Define the union U_PHY_SYNC_SOF2_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sof0_word4_2 : 16; /* [15..0]  */
		unsigned int cil_sof1_word4_2 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOF2_LINK;

/* Define the union U_PHY_SYNC_SOF3_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sof0_word4_3 : 16; /* [15..0]  */
		unsigned int cil_sof1_word4_3 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOF3_LINK;

/* Define the union U_PHY_OUT_VALID_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_lvds_out_valid : 4; /* [3..0]  */
		unsigned int cil_mipi_out_valid : 4; /* [7..4]  */
		unsigned int reserved_0 : 24; /* [31..8]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_OUT_VALID_LINK;

/* Define the union U_PHY_DATA_LVDS_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy_data0_lvds_hs : 8; /* [7..0]  */
		unsigned int phy_data1_lvds_hs : 8; /* [15..8]  */
		unsigned int phy_data2_lvds_hs : 8; /* [23..16]  */
		unsigned int phy_data3_lvds_hs : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_DATA_LVDS_LINK;

/* Define the union U_PHY_PIX_PUM_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy_lane02_pix_num : 16; /* [15..0]  */
		unsigned int phy_lane13_pix_num : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_PIX_PUM_LINK;

/* Define the union U_PHY_SYNC_SOL0_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sol0_word4_0 : 16; /* [15..0]  */
		unsigned int cil_sol1_word4_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOL0_LINK;

/* Define the union U_PHY_SYNC_SOL1_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sol0_word4_1 : 16; /* [15..0]  */
		unsigned int cil_sol1_word4_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOL1_LINK;

/* Define the union U_PHY_SYNC_SOL2_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sol0_word4_2 : 16; /* [15..0]  */
		unsigned int cil_sol1_word4_2 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOL2_LINK;

/* Define the union U_PHY_SYNC_SOL3_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_sol0_word4_3 : 16; /* [15..0]  */
		unsigned int cil_sol1_word4_3 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_SYNC_SOL3_LINK;

/* Define the union U_CIL_TIMEOUT_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_timeout_cyc : 24; /* [23..0]  */
		unsigned int reserved_0 : 6; /* [29..24]  */
		unsigned int cil_timeout_en_ck : 1; /* [30]  */
		unsigned int cil_timeout_en_d : 1; /* [31]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CIL_TIMEOUT_LINK;

/* Define the union U_CIL_FSM0_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_cyc_clk_lp00 : 6; /* [5..0]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int cil_cyc_clk_hs0 : 6; /* [13..8]  */
		unsigned int reserved_1 : 2; /* [15..14]  */
		unsigned int cil_cyc_clk_hs_exit : 6; /* [21..16]  */
		unsigned int reserved_2 : 2; /* [23..22]  */
		unsigned int cil_cyc_clk_hs_trail : 6; /* [29..24]  */
		unsigned int reserved_3 : 2; /* [31..30]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CIL_FSM0_LINK;

/* Define the union U_CIL_FSM_ST0_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_cur_st_d0 : 4; /* [3..0]  */
		unsigned int cil_nxt_st_d0 : 4; /* [7..4]  */
		unsigned int cil_cur_st_d1 : 4; /* [11..8]  */
		unsigned int cil_nxt_st_d1 : 4; /* [15..12]  */
		unsigned int cil_cur_st_d2 : 4; /* [19..16]  */
		unsigned int cil_nxt_st_d2 : 4; /* [23..20]  */
		unsigned int cil_cur_st_d3 : 4; /* [27..24]  */
		unsigned int cil_nxt_st_d3 : 4; /* [31..28]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CIL_FSM_ST0_LINK;

/* Define the union U_CIL_FSM_ST1_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_cur_st_ck : 4; /* [3..0]  */
		unsigned int cil_nxt_st_ck : 4; /* [7..4]  */
		unsigned int reserved_0 : 24; /* [31..8]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CIL_FSM_ST1_LINK;

/* Define the union U_PHY_ST0_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int physt0_rg_en_d : 4; /* [3..0]  */
		unsigned int physt0_rg_en_clk0 : 1; /* [4]  */
		unsigned int physt0_rg_en_clk1 : 1; /* [5]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int physt0_rg_en_cmos : 1; /* [8]  */
		unsigned int reserved_1 : 3; /* [11..9]  */
		unsigned int physt0_rg_en_lp0 : 1; /* [12]  */
		unsigned int physt0_rg_en_lp1 : 1; /* [13]  */
		unsigned int reserved_2 : 2; /* [15..14]  */
		unsigned int physt0_rg_faclk0_en : 1; /* [16]  */
		unsigned int physt0_rg_faclk1_en : 1; /* [17]  */
		unsigned int reserved_3 : 2; /* [19..18]  */
		unsigned int physt0_rg_en_2l2l : 1; /* [20]  */
		unsigned int reserved_4 : 3; /* [23..21]  */
		unsigned int physt0_rg_mipi_mode0 : 1; /* [24]  */
		unsigned int physt0_rg_mipi_mode1 : 1; /* [25]  */
		unsigned int reserved_5 : 6; /* [31..26]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_ST0_LINK;

/* Define the union U_PHY_ST1_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int physt0_clk0_skew : 3; /* [2..0]  */
		unsigned int reserved_0 : 1; /* [3]  */
		unsigned int physt0_clk1_skew : 3; /* [6..4]  */
		unsigned int reserved_1 : 1; /* [7]  */
		unsigned int physt0_d0_skew : 3; /* [10..8]  */
		unsigned int reserved_2 : 1; /* [11]  */
		unsigned int physt0_d1_skew : 3; /* [14..12]  */
		unsigned int reserved_3 : 1; /* [15]  */
		unsigned int physt0_d2_skew : 3; /* [18..16]  */
		unsigned int reserved_4 : 1; /* [19]  */
		unsigned int physt0_d3_skew : 3; /* [22..20]  */
		unsigned int reserved_5 : 9; /* [31..23]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_ST1_LINK;

/* Define the union U_PHY_ST2_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int physt_da_d0_valid : 1; /* [0]  */
		unsigned int physt_da_d1_valid : 1; /* [1]  */
		unsigned int physt_da_d2_valid : 1; /* [2]  */
		unsigned int physt_da_d3_valid : 1; /* [3]  */
		unsigned int reserved_0 : 4; /* [7..4]  */
		unsigned int physt_d0_term_en : 1; /* [8]  */
		unsigned int physt_d1_term_en : 1; /* [9]  */
		unsigned int physt_d2_term_en : 1; /* [10]  */
		unsigned int physt_d3_term_en : 1; /* [11]  */
		unsigned int physt_clk0_term_en : 1; /* [12]  */
		unsigned int physt_clk1_term_en : 1; /* [13]  */
		unsigned int reserved_1 : 18; /* [31..14]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_ST2_LINK;

/* Define the union U_PHY_ST3_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int physt0_da_ulpb : 4; /* [3..0]  */
		unsigned int physt0_da_ulpb_ck0 : 1; /* [4]  */
		unsigned int physt0_da_ulpb_ck1 : 1; /* [5]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int physt0_rg_it_lp : 2; /* [9..8]  */
		unsigned int physt0_rg_it_ck : 2; /* [11..10]  */
		unsigned int physt0_rg_it_dt : 2; /* [13..12]  */
		unsigned int physt0_rg_sa_en : 1; /* [14]  */
		unsigned int reserved_1 : 17; /* [31..15]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_ST3_LINK;

/* Define the union U_CIL_FSM1_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int cil_cyc_data_lp00 : 6; /* [5..0]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int cil_cyc_data_hs0 : 6; /* [13..8]  */
		unsigned int reserved_1 : 2; /* [15..14]  */
		unsigned int cil_cyc_data_hs_exit : 6; /* [21..16]  */
		unsigned int reserved_2 : 2; /* [23..22]  */
		unsigned int cil_cyc_data_hs_trail : 6; /* [29..24]  */
		unsigned int reserved_3 : 2; /* [31..30]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CIL_FSM1_LINK;

/* Define the union U_MIPI_CIL_INT_RAW_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err0_escape_d0_raw : 1; /* [0]  */
		unsigned int err0_escape_d1_raw : 1; /* [1]  */
		unsigned int err0_escape_d2_raw : 1; /* [2]  */
		unsigned int err0_escape_d3_raw : 1; /* [3]  */
		unsigned int err0_escape_ck0_raw : 1; /* [4]  */
		unsigned int reserved_0 : 3; /* [7..5]  */
		unsigned int err0_timeout_d0_raw : 1; /* [8]  */
		unsigned int err0_timeout_d1_raw : 1; /* [9]  */
		unsigned int err0_timeout_d2_raw : 1; /* [10]  */
		unsigned int err0_timeout_d3_raw : 1; /* [11]  */
		unsigned int err0_timeout_ck0_raw : 1; /* [12]  */
		unsigned int reserved_1 : 19; /* [31..13]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CIL_INT_RAW_LINK;

/* Define the union U_MIPI_CIL_INT_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err0_escape_d0_st : 1; /* [0]  */
		unsigned int err0_escape_d1_st : 1; /* [1]  */
		unsigned int err0_escape_d2_st : 1; /* [2]  */
		unsigned int err0_escape_d3_st : 1; /* [3]  */
		unsigned int err0_escape_ck0_st : 1; /* [4]  */
		unsigned int err0_escape_ck1_st : 1; /* [5]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int err0_timeout_d0_st : 1; /* [8]  */
		unsigned int err0_timeout_d1_st : 1; /* [9]  */
		unsigned int err0_timeout_d2_st : 1; /* [10]  */
		unsigned int err0_timeout_d3_st : 1; /* [11]  */
		unsigned int err0_timeout_ck0_st : 1; /* [12]  */
		unsigned int err0_timeout_ck1_st : 1; /* [13]  */
		unsigned int reserved_1 : 18; /* [31..14]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CIL_INT_LINK;

/* Define the union U_MIPI_CIL_INT_MSK_LINK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err3_escape_d0_msk : 1; /* [0]  */
		unsigned int err3_escape_d1_msk : 1; /* [1]  */
		unsigned int err3_escape_d2_msk : 1; /* [2]  */
		unsigned int err3_escape_d3_msk : 1; /* [3]  */
		unsigned int err3_escape_ck0_msk : 1; /* [4]  */
		unsigned int err3_escape_ck1_msk : 1; /* [5]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int err3_timeout_d0_msk : 1; /* [8]  */
		unsigned int err3_timeout_d1_msk : 1; /* [9]  */
		unsigned int err3_timeout_d2_msk : 1; /* [10]  */
		unsigned int err3_timeout_d3_msk : 1; /* [11]  */
		unsigned int err3_timeout_ck0_msk : 1; /* [12]  */
		unsigned int err3_timeout_ck1_msk : 1; /* [13]  */
		unsigned int reserved_1 : 18; /* [31..14]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CIL_INT_MSK_LINK;

/* Define the union U_PHY_EN */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phy0_en : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int d_p_swap_data0 : 1; /* [4]  */
		unsigned int d_p_swap_data1 : 1; /* [5]  */
		unsigned int d_p_swap_data2 : 1; /* [6]  */
		unsigned int d_p_swap_data3 : 1; /* [7]  */
		unsigned int d_p_swap_clk : 1; /* [8]  */
		unsigned int reserved_1 : 23; /* [31..9]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_EN;

/* Define the union U_LANE_EN */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane0_en : 1; /* [0]  */
		unsigned int lane1_en : 1; /* [1]  */
		unsigned int lane2_en : 1; /* [2]  */
		unsigned int lane3_en : 1; /* [3]  */
		unsigned int reserved_0 : 28; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LANE_EN;

/* Define the union U_PHY_CIL_CTRL */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phycil0_cken : 1; /* [0]  */
		unsigned int reserved_0 : 7; /* [7..1]  */
		unsigned int cil0_rst_req : 1; /* [8]  */
		unsigned int reserved_1 : 23; /* [31..9]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY_CIL_CTRL;

/* Define the union U_PHYCFG_MODE */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phycil0_0_cfg_mode : 3; /* [2..0]  */
		unsigned int phycil0_cfg_mode_sel : 1; /* [3]  */
		unsigned int reserved_0 : 28; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHYCFG_MODE;

/* Define the union U_PHYCFG_EN */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int phycil0_cfg_en : 1; /* [0]  */
		unsigned int reserved_0 : 31; /* [31..1]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHYCFG_EN;

/* Define the union U_CHN0_MEM_CTRL */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int chn0_mem_ck_gt : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int chn0_mem_emasa : 1; /* [4]  */
		unsigned int chn0_mem_ret1n : 1; /* [5]  */
		unsigned int chn0_mem_colldisn : 1; /* [6]  */
		unsigned int reserved_1 : 1; /* [7]  */
		unsigned int chn0_mem_emaa : 3; /* [10..8]  */
		unsigned int reserved_2 : 1; /* [11]  */
		unsigned int chn0_mem_emab : 3; /* [14..12]  */
		unsigned int reserved_3 : 17; /* [31..15]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CHN0_MEM_CTRL;

/* Define the union U_CHN0_CLR_EN */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int chn0_clr_en_lvds : 1; /* [0]  */
		unsigned int chn0_clr_en_align : 1; /* [1]  */
		unsigned int reserved_0 : 30; /* [31..2]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CHN0_CLR_EN;

/* Define the union U_MIPI_TEST_PHY */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int test_phy_en : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int test_phy_srst_req : 1; /* [4]  */
		unsigned int reserved_1 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_TEST_PHY;

/* Define the union U_COLORBAR_CTRL_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_en : 1; /* [0]  */
		unsigned int colorbar0_mode : 1; /* [1]  */
		unsigned int colorbar0_pattern : 1; /* [2]  */
		unsigned int colorbar0_line_init : 1; /* [3]  */
		unsigned int colorbar0_width : 10; /* [13..4]  */
		unsigned int reserved_0 : 18; /* [31..14]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_CTRL_LINK0;

/* Define the union U_COLORBAR_INIT_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_data_init : 12; /* [11..0]  */
		unsigned int reserved_0 : 4; /* [15..12]  */
		unsigned int colorbar0_data_inc : 12; /* [27..16]  */
		unsigned int reserved_1 : 4; /* [31..28]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_INIT_LINK0;

/* Define the union U_COLORBAR_V_BLK_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_vertical_blk : 20; /* [19..0]  */
		unsigned int reserved_0 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_V_BLK_LINK0;

/* Define the union U_COLORBAR_H_BLK_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_line_blk : 20; /* [19..0]  */
		unsigned int reserved_0 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_H_BLK_LINK0;

/* Define the union U_COLORBAR_IMG_SIZE_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_imgwidth : 16; /* [15..0]  */
		unsigned int colorbar0_imgheight : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_IMG_SIZE_LINK0;

/* Define the union U_COLORBAR_SYNC_CODE0_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_sof : 12; /* [11..0]  */
		unsigned int reserved_0 : 4; /* [15..12]  */
		unsigned int colorbar0_eof : 12; /* [27..16]  */
		unsigned int reserved_1 : 4; /* [31..28]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_SYNC_CODE0_LINK0;

/* Define the union U_COLORBAR_SYNC_CODE1_LINK0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int colorbar0_sol : 12; /* [11..0]  */
		unsigned int reserved_0 : 4; /* [15..12]  */
		unsigned int colorbar0_eol : 12; /* [27..16]  */
		unsigned int reserved_1 : 4; /* [31..28]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_COLORBAR_SYNC_CODE1_LINK0;

/* Define the union U_PHY0_TEST_OUT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int test_phy0_resault : 4; /* [3..0]  */
		unsigned int test_phy0_compare : 4; /* [7..4]  */
		unsigned int test_phy0_done : 4; /* [11..8]  */
		unsigned int reserved_0 : 20; /* [31..12]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_PHY0_TEST_OUT;

/* Define the union U_MIPI_INT_RAW */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_phycil0_raw : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int int_chn0_raw : 1; /* [4]  */
		unsigned int reserved_1 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_INT_RAW;

/* Define the union U_MIPI_INT_ST */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_phycil0_st : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int int_chn0_st : 1; /* [4]  */
		unsigned int reserved_1 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_INT_ST;

/* Define the union U_MIPI_INT_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_phycil0_mask : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int int_chn0_mask : 1; /* [4]  */
		unsigned int reserved_1 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_INT_MSK;

/* Define the union U_MIPI(N)_LANES_NUM */
/* 0x1004+N*0x1000 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_num : 3; /* [2..0]  */
		unsigned int reserved_0 : 29; /* [31..3]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_LANES_NUM;

/* Define the union U_MIPI(N)_MAIN_INT_ST */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int reserved_0 : 1; /* [0]  */
		unsigned int status_int_pkt_fatal : 1; /* [1]  */
		unsigned int status_int_frame_fatal : 1; /* [2]  */
		unsigned int reserved_1 : 14; /* [16..3]  */
		unsigned int status_int_pkt : 1; /* [17]  */
		unsigned int status_int_line : 1; /* [18]  */
		unsigned int reserved_2 : 13; /* [31..19]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_MAIN_INT_ST;

/* Define the union U_MIPI(N)_DI_1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int di0_dt : 6; /* [5..0]  */
		unsigned int di0_vc : 2; /* [7..6]  */
		unsigned int di1_dt : 6; /* [13..8]  */
		unsigned int di1_vc : 2; /* [15..14]  */
		unsigned int di2_dt : 6; /* [21..16]  */
		unsigned int di2_vc : 2; /* [23..22]  */
		unsigned int di3_dt : 6; /* [29..24]  */
		unsigned int di3_vc : 2; /* [31..30]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_DI_1;

/* Define the union U_MIPI(N)_DI_2 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int di4_dt : 6; /* [5..0]  */
		unsigned int di4_vc : 2; /* [7..6]  */
		unsigned int di5_dt : 6; /* [13..8]  */
		unsigned int di5_vc : 2; /* [15..14]  */
		unsigned int di6_dt : 6; /* [21..16]  */
		unsigned int di6_vc : 2; /* [23..22]  */
		unsigned int di7_dt : 6; /* [29..24]  */
		unsigned int di7_vc : 2; /* [31..30]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_DI_2;

/* Define the union U_MIPI(N)_PKT_INTR_ST */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int vc0_err_crc : 1; /* [0]  */
		unsigned int vc1_err_crc : 1; /* [1]  */
		unsigned int vc2_err_crc : 1; /* [2]  */
		unsigned int vc3_err_crc : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int err_ecc_double : 1; /* [16]  */
		unsigned int reserved_1 : 15; /* [31..17]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_PKT_INTR_ST;

/* Define the union U_MIPI(N)_PKT_INTR_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mask_vc0_err_crc : 1; /* [0]  */
		unsigned int mask_vc1_err_crc : 1; /* [1]  */
		unsigned int mask_vc2_err_crc : 1; /* [2]  */
		unsigned int mask_vc3_err_crc : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int mask_err_ecc_double : 1; /* [16]  */
		unsigned int reserved_1 : 15; /* [31..17]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_PKT_INTR_MSK;

/* Define the union U_MIPI(N)_PKT_INTR_FORCE */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int force_vc0_err_crc : 1; /* [0]  */
		unsigned int force_vc1_err_crc : 1; /* [1]  */
		unsigned int force_vc2_err_crc : 1; /* [2]  */
		unsigned int force_vc3_err_crc : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int force_err_ecc_double : 1; /* [16]  */
		unsigned int reserved_1 : 15; /* [31..17]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_PKT_INTR_FORCE;

/* Define the union U_MIPI(N)_PKT_INTR2_ST */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err_id_vc0 : 1; /* [0]  */
		unsigned int err_id_vc1 : 1; /* [1]  */
		unsigned int err_id_vc2 : 1; /* [2]  */
		unsigned int err_id_vc3 : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int vc0_err_ecc_corrected : 1; /* [16]  */
		unsigned int vc1_err_ecc_corrected : 1; /* [17]  */
		unsigned int vc2_err_ecc_corrected : 1; /* [18]  */
		unsigned int vc3_err_ecc_corrected : 1; /* [19]  */
		unsigned int reserved_1 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_PKT_INTR2_ST;

/* Define the union b_MIPI_PKT_INTR2_MSK_U */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mask_err_id_vc0 : 1; /* [0]  */
		unsigned int mask_err_id_vc1 : 1; /* [1]  */
		unsigned int mask_err_id_vc2 : 1; /* [2]  */
		unsigned int mask_err_id_vc3 : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int mask_vc0_err_ecc_corrected : 1; /* [16]  */
		unsigned int mask_vc1_err_ecc_corrected : 1; /* [17]  */
		unsigned int mask_vc2_err_ecc_corrected : 1; /* [18]  */
		unsigned int mask_vc3_err_ecc_corrected : 1; /* [19]  */
		unsigned int reserved_1 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_PKT_INTR2_MSK;

/* Define the union U_MIPI(N)_PKT_INTR2_FORCE */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int force_err_id_vc0 : 1; /* [0]  */
		unsigned int force_err_id_vc1 : 1; /* [1]  */
		unsigned int force_err_id_vc2 : 1; /* [2]  */
		unsigned int force_err_id_vc3 : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int force_vc0_err_ecc_corrected : 1; /* [16]  */
		unsigned int force_vc1_err_ecc_corrected : 1; /* [17]  */
		unsigned int force_vc2_err_ecc_corrected : 1; /* [18]  */
		unsigned int force_vc3_err_ecc_corrected : 1; /* [19]  */
		unsigned int reserved_1 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_PKT_INTR2_FORCE;

/* Define the union U_MIPI(N)_FRAME_INTR_ST */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err_f_bndry_match_vc0 : 1; /* [0]  */
		unsigned int err_f_bndry_match_vc1 : 1; /* [1]  */
		unsigned int err_f_bndry_match_vc2 : 1; /* [2]  */
		unsigned int err_f_bndry_match_vc3 : 1; /* [3]  */
		unsigned int reserved_0 : 4; /* [7..4]  */
		unsigned int err_f_seq_vc0 : 1; /* [8]  */
		unsigned int err_f_seq_vc1 : 1; /* [9]  */
		unsigned int err_f_seq_vc2 : 1; /* [10]  */
		unsigned int err_f_seq_vc3 : 1; /* [11]  */
		unsigned int reserved_1 : 4; /* [15..12]  */
		unsigned int err_frame_data_vc0 : 1; /* [16]  */
		unsigned int err_frame_data_vc1 : 1; /* [17]  */
		unsigned int err_frame_data_vc2 : 1; /* [18]  */
		unsigned int err_frame_data_vc3 : 1; /* [19]  */
		unsigned int reserved_2 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_FRAME_INTR_ST;

/* Define the union U_MIPI(N)_FRAME_INTR_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mask_err_f_bndry_match_vc0 : 1; /* [0]  */
		unsigned int mask_err_f_bndry_match_vc1 : 1; /* [1]  */
		unsigned int mask_err_f_bndry_match_vc2 : 1; /* [2]  */
		unsigned int mask_err_f_bndry_match_vc3 : 1; /* [3]  */
		unsigned int reserved_0 : 4; /* [7..4]  */
		unsigned int mask_err_f_seq_vc0 : 1; /* [8]  */
		unsigned int mask_err_f_seq_vc1 : 1; /* [9]  */
		unsigned int mask_err_f_seq_vc2 : 1; /* [10]  */
		unsigned int mask_err_f_seq_vc3 : 1; /* [11]  */
		unsigned int reserved_1 : 4; /* [15..12]  */
		unsigned int mask_err_frame_data_vc0 : 1; /* [16]  */
		unsigned int mask_err_frame_data_vc1 : 1; /* [17]  */
		unsigned int mask_err_frame_data_vc2 : 1; /* [18]  */
		unsigned int mask_err_frame_data_vc3 : 1; /* [19]  */
		unsigned int reserved_2 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_FRAME_INTR_MSK;

/* Define the union b_MIPI_FRAME_INTR_FORCE_U */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int force_err_f_bndry_match_vc0 : 1; /* [0]  */
		unsigned int force_err_f_bndry_match_vc1 : 1; /* [1]  */
		unsigned int force_err_f_bndry_match_vc2 : 1; /* [2]  */
		unsigned int force_err_f_bndry_match_vc3 : 1; /* [3]  */
		unsigned int reserved_0 : 4; /* [7..4]  */
		unsigned int force_err_f_seq_vc0 : 1; /* [8]  */
		unsigned int force_err_f_seq_vc1 : 1; /* [9]  */
		unsigned int force_err_f_seq_vc2 : 1; /* [10]  */
		unsigned int force_err_f_seq_vc3 : 1; /* [11]  */
		unsigned int reserved_1 : 4; /* [15..12]  */
		unsigned int force_err_frame_data_vc0 : 1; /* [16]  */
		unsigned int force_err_frame_data_vc1 : 1; /* [17]  */
		unsigned int force_err_frame_data_vc2 : 1; /* [18]  */
		unsigned int force_err_frame_data_vc3 : 1; /* [19]  */
		unsigned int reserved_2 : 12; /* [31..20]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_FRAME_INTR_FORCE;

/* Define the union U_MIPI(N)_LINE_INTR_ST */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err_l_bndry_match_di0 : 1; /* [0]  */
		unsigned int err_l_bndry_match_di1 : 1; /* [1]  */
		unsigned int err_l_bndry_match_di2 : 1; /* [2]  */
		unsigned int err_l_bndry_match_di3 : 1; /* [3]  */
		unsigned int err_l_bndry_match_di4 : 1; /* [4]  */
		unsigned int err_l_bndry_match_di5 : 1; /* [5]  */
		unsigned int err_l_bndry_match_di6 : 1; /* [6]  */
		unsigned int err_l_bndry_match_di7 : 1; /* [7]  */
		unsigned int reserved_0 : 8; /* [15..8]  */
		unsigned int err_l_seq_di0 : 1; /* [16]  */
		unsigned int err_l_seq_di1 : 1; /* [17]  */
		unsigned int err_l_seq_di2 : 1; /* [18]  */
		unsigned int err_l_seq_di3 : 1; /* [19]  */
		unsigned int err_l_seq_di4 : 1; /* [20]  */
		unsigned int err_l_seq_di5 : 1; /* [21]  */
		unsigned int err_l_seq_di6 : 1; /* [22]  */
		unsigned int err_l_seq_di7 : 1; /* [23]  */
		unsigned int reserved_1 : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_LINE_INTR_ST;

/* Define the union U_MIPI(N)_LINE_INTR_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mask_err_l_bndry_match_di0 : 1; /* [0]  */
		unsigned int mask_err_l_bndry_match_di1 : 1; /* [1]  */
		unsigned int mask_err_l_bndry_match_di2 : 1; /* [2]  */
		unsigned int mask_err_l_bndry_match_di3 : 1; /* [3]  */
		unsigned int mask_err_l_bndry_match_di4 : 1; /* [4]  */
		unsigned int mask_err_l_bndry_match_di5 : 1; /* [5]  */
		unsigned int mask_err_l_bndry_match_di6 : 1; /* [6]  */
		unsigned int mask_err_l_bndry_match_di7 : 1; /* [7]  */
		unsigned int reserved_0 : 8; /* [15..8]  */
		unsigned int mask_err_l_seq_di0 : 1; /* [16]  */
		unsigned int mask_err_l_seq_di1 : 1; /* [17]  */
		unsigned int mask_err_l_seq_di2 : 1; /* [18]  */
		unsigned int mask_err_l_seq_di3 : 1; /* [19]  */
		unsigned int mask_err_l_seq_di4 : 1; /* [20]  */
		unsigned int mask_err_l_seq_di5 : 1; /* [21]  */
		unsigned int mask_err_l_seq_di6 : 1; /* [22]  */
		unsigned int mask_err_l_seq_di7 : 1; /* [23]  */
		unsigned int reserved_1 : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_LINE_INTR_MSK;

/* Define the union U_MIPI(N)_LINE_INTR_FORCE */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int force_err_l_bndry_match_di0 : 1; /* [0]  */
		unsigned int force_err_l_bndry_match_di1 : 1; /* [1]  */
		unsigned int force_err_l_bndry_match_di2 : 1; /* [2]  */
		unsigned int force_err_l_bndry_match_di3 : 1; /* [3]  */
		unsigned int force_err_l_bndry_match_di4 : 1; /* [4]  */
		unsigned int force_err_l_bndry_match_di5 : 1; /* [5]  */
		unsigned int force_err_l_bndry_match_di6 : 1; /* [6]  */
		unsigned int force_err_l_bndry_match_di7 : 1; /* [7]  */
		unsigned int reserved_0 : 8; /* [15..8]  */
		unsigned int force_err_l_seq_di0 : 1; /* [16]  */
		unsigned int force_err_l_seq_di1 : 1; /* [17]  */
		unsigned int force_err_l_seq_di2 : 1; /* [18]  */
		unsigned int force_err_l_seq_di3 : 1; /* [19]  */
		unsigned int force_err_l_seq_di4 : 1; /* [20]  */
		unsigned int force_err_l_seq_di5 : 1; /* [21]  */
		unsigned int force_err_l_seq_di6 : 1; /* [22]  */
		unsigned int force_err_l_seq_di7 : 1; /* [23]  */
		unsigned int reserved_1 : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_LINE_INTR_FORCE;

/* Define the union U_MIPI(N)_USERDEF_DT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int user_def0_dt : 3; /* [2..0]  */
		unsigned int reserved_0 : 1; /* [3]  */
		unsigned int user_def1_dt : 3; /* [6..4]  */
		unsigned int reserved_1 : 1; /* [7]  */
		unsigned int user_def2_dt : 3; /* [10..8]  */
		unsigned int reserved_2 : 1; /* [11]  */
		unsigned int user_def3_dt : 3; /* [14..12]  */
		unsigned int reserved_3 : 17; /* [31..15]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_USERDEF_DT;

/* Define the union U_MIPI(N)_USER_DEF */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int user_def0 : 6; /* [5..0]  */
		unsigned int reserved_0 : 2; /* [7..6]  */
		unsigned int user_def1 : 6; /* [13..8]  */
		unsigned int reserved_1 : 2; /* [15..14]  */
		unsigned int user_def2 : 6; /* [21..16]  */
		unsigned int reserved_2 : 2; /* [23..22]  */
		unsigned int user_def3 : 6; /* [29..24]  */
		unsigned int reserved_3 : 2; /* [31..30]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_USER_DEF;

/* Define the union U_MIPI(N)_CTRL_MODE_HS */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int hdr_mode : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int vc_mode : 1; /* [4]  */
		unsigned int reserved_1 : 3; /* [7..5]  */
		unsigned int user_def_en : 1; /* [8]  */
		unsigned int reserved_2 : 3; /* [11..9]  */
		unsigned int lane0_location : 3; /* [14..12]  */
		unsigned int reserved_3 : 17; /* [31..15]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CTRL_MODE_HS;

/* Define the union U_MIPI(N)_VHEND_DELAY */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int vend_delay : 12; /* [11..0]  */
		unsigned int reserved_0 : 4; /* [15..12]  */
		unsigned int hend_delay : 8; /* [23..16]  */
		unsigned int reserved_1 : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_VHEND_DELAY;

/* Define the union U_MIPI(N)_DOL_ID_CODE0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int id_code_reg0 : 16; /* [15..0]  */
		unsigned int id_code_reg1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_DOL_ID_CODE0;

/* Define the union U_MIPI_DOL_ID_CODE1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int id_code_reg2 : 16; /* [15..0]  */
		unsigned int id_code_reg3 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_DOL_ID_CODE1;

/* Define the union U_MIPI_DOL_ID_CODE2 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int id_code_reg4 : 16; /* [15..0]  */
		unsigned int id_code_reg5 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_DOL_ID_CODE2;

/* Define the union U_MIPI_CROP_START_CHN0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mipi_start_x_chn0 : 16; /* [15..0]  */
		unsigned int mipi_start_y_chn0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CROP_START_CHN0;

/* Define the union U_MIPI_CROP_START_CHN1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mipi_start_x_chn1 : 16; /* [15..0]  */
		unsigned int mipi_start_y_chn1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CROP_START_CHN1;

/* Define the union U_MIPI_IMGSIZE */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int mipi_imgwidth : 16; /* [15..0]  */
		unsigned int mipi_imgheight : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_IMGSIZE;

/* Define the union U_MIPI_CTRL_MODE_PIXEL */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int crop_en : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int mipi_dol_mode : 1; /* [4]  */
		unsigned int reserved_1 : 3; /* [7..5]  */
		unsigned int rx2mode : 1; /* [8]  */
		unsigned int data_merge_mode : 2; /* [10..9]  */
		unsigned int sync_clear_en : 1; /* [11]  */
		unsigned int stagger_hdr_mode : 1; /* [12]  */
		unsigned int stagger_frm_num : 2; /* [14..13]  */
		unsigned int dummy_line_detect : 1; /* [15]  */
		unsigned int mipi_double_pix_en : 1; /* [16]  */
		unsigned int mipi_double_yuv_en : 1; /* [17]  */
		unsigned int mipi_yuv_422_en : 1; /* [18]  */
		unsigned int mipi_yuv_420_nolegacy_en : 1; /* [19]  */
		unsigned int mipi_yuv_420_legacy_en : 1; /* [20]  */
		unsigned int mipi_yuv_420_nolegacy_init : 1; /* [21]  */
		unsigned int mipi_yuv420_even_detect : 1; /* [22]  */
		unsigned int reserved_2 : 9; /* [31..23]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CTRL_MODE_PIXEL;

/* Define the union U_MIPI_DUMMY_PIX_REG */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int dummy_pix_reg : 16; /* [15..0]  */
		unsigned int reserved_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_DUMMY_PIX_REG;

/* Define the union U_MIPI_IMGSIZE0_STATIS */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int imgwidth_statis_vc0 : 16; /* [15..0]  */
		unsigned int imgheight_statis_vc0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_IMGSIZE0_STATIS;

/* Define the union U_MIPI_IMGSIZE1_STATIS */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int imgwidth_statis_vc1 : 16; /* [15..0]  */
		unsigned int imgheight_statis_vc1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_IMGSIZE1_STATIS;

/* Define the union U_MIPI_CTRL_INT_RAW */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_cfifo_wrerr_raw : 1; /* [0]  */
		unsigned int int_dfifo_wrerr_raw : 1; /* [1]  */
		unsigned int reserved_0 : 2; /* [3..2]  */
		unsigned int int_vsync_raw : 1; /* [4]  */
		unsigned int reserved_1 : 11; /* [15..5]  */
		unsigned int int_cfifo_rderr_raw : 1; /* [16]  */
		unsigned int int_dfifo_rderr_raw : 1; /* [17]  */
		unsigned int reserved_2 : 14; /* [31..18]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CTRL_INT_RAW;

/* Define the union U_MIPI_CTRL_INT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_cfifo_wrerr_st : 1; /* [0]  */
		unsigned int int_dfifo_wrerr_st : 1; /* [1]  */
		unsigned int reserved_0 : 2; /* [3..2]  */
		unsigned int int_vsync_st : 1; /* [4]  */
		unsigned int reserved_1 : 11; /* [15..5]  */
		unsigned int int_cfifo_rderr_st : 1; /* [16]  */
		unsigned int int_dfifo_rderr_st : 1; /* [17]  */
		unsigned int reserved_2 : 14; /* [31..18]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CTRL_INT;

/* Define the union U_MIPI_CTRL_INT_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_cfifo_wrerr_msk : 1; /* [0]  */
		unsigned int int_dfifo_wrerr_msk : 1; /* [1]  */
		unsigned int reserved_0 : 2; /* [3..2]  */
		unsigned int int_vsync_msk : 1; /* [4]  */
		unsigned int reserved_1 : 11; /* [15..5]  */
		unsigned int int_cfifo_rderr_msk : 1; /* [16]  */
		unsigned int int_dfifo_rderr_msk : 1; /* [17]  */
		unsigned int reserved_2 : 14; /* [31..18]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_MIPI_CTRL_INT_MSK;

/* Define the union U_LVDS_WDR */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_wdr_en : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int lvds_wdr_num : 2; /* [5..4]  */
		unsigned int reserved_1 : 2; /* [7..6]  */
		unsigned int lvds_wdr_mode : 4; /* [11..8]  */
		unsigned int lvds_wdr_id_shift : 4; /* [15..12]  */
		unsigned int reserved_2 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_WDR;

/* Define the union U_LVDS_DOLSCD_HBLK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int dol_hblank1 : 16; /* [15..0]  */
		unsigned int dol_hblank2 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_DOLSCD_HBLK;

/* Define the union U_LVDS_CTRL */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_sync_mode : 1; /* [0]  */
		unsigned int reserved_0 : 3; /* [3..1]  */
		unsigned int lvds_raw_type : 3; /* [6..4]  */
		unsigned int reserved_1 : 1; /* [7]  */
		unsigned int lvds_pix_big_endian : 1; /* [8]  */
		unsigned int lvds_code_big_endian : 1; /* [9]  */
		unsigned int reserved_2 : 2; /* [11..10]  */
		unsigned int lvds_crop_en : 1; /* [12]  */
		unsigned int reserved_3 : 19; /* [31..13]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_CTRL;

/* Define the union U_LVDS_IMGSIZE */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_imgwidth_lane : 16; /* [15..0]  */
		unsigned int lvds_imgheight : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_IMGSIZE;

/* Define the union U_LVDS_CROP_START0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_start_x0_lane : 16; /* [15..0]  */
		unsigned int lvds_start_y0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_CROP_START0;

/* Define the union U_LVDS_CROP_START1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_start_x1_lane : 16; /* [15..0]  */
		unsigned int lvds_start_y1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_CROP_START1;

/* Define the union U_LVDS_LANE(M)_SOF_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_sof_0 : 16; /* [15..0]  */
		unsigned int lane_sof_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_SOF_01;

/* Define the union U_LVDS_LANE_EOF_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_eof_0 : 16; /* [15..0]  */
		unsigned int lane_eof_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_EOF_01;

/* Define the union U_LVDS_LANE_SOL_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_sol_0 : 16; /* [15..0]  */
		unsigned int lane_sol_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_SOL_01;

/* Define the union U_LVDS_LANE_EOL_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_eol_0 : 16; /* [15..0]  */
		unsigned int lane_eol_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_EOL_01;

/* Define the union U_LVDS_LANE_NXT_SOF_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_nxt_sof_0 : 16; /* [15..0]  */
		unsigned int lane_nxt_sof_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_NXT_SOF_01;

/* Define the union U_LVDS_LANE_NXT_EOF_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_nxt_eof_0 : 16; /* [15..0]  */
		unsigned int lane_nxt_eof_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_NXT_EOF_01;

/* Define the union U_LVDS_LANE_NXT_SOL_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_nxt_sol_0 : 16; /* [15..0]  */
		unsigned int lane_nxt_sol_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_NXT_SOL_01;

/* Define the union U_LVDS_LANE_NXT_EOL_01 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_nxt_eol_0 : 16; /* [15..0]  */
		unsigned int lane_nxt_eol_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_NXT_EOL_01;

/* Define the union U_LVDS_LI_WORD0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int li_word0_0 : 16; /* [15..0]  */
		unsigned int li_word0_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LI_WORD0;

/* Define the union U_LVDS_LI_WORD1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int li_word1_0 : 16; /* [15..0]  */
		unsigned int li_word1_1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LI_WORD1;

/* Define the union U_LVDS_SYNC_BORD0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int sync_bord0_lane0 : 4; /* [3..0]  */
		unsigned int sync_bord0_lane1 : 4; /* [7..4]  */
		unsigned int sync_bord0_lane2 : 4; /* [11..8]  */
		unsigned int sync_bord0_lane3 : 4; /* [15..12]  */
		unsigned int reserved_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_SYNC_BORD0;

/* Define the union U_LVDS_LANE_IMGSIZE_STATIS */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane_imgwidth : 16; /* [15..0]  */
		unsigned int lane_imgheight : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_IMGSIZE_STATIS;

/* Define the union U_LVDS_IMGSIZE0_STATIS */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_imgwidth0 : 16; /* [15..0]  */
		unsigned int lvds_imgheight0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_IMGSIZE0_STATIS;

/* Define the union U_LVDS_IMGSIZE1_STATIS */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_imgwidth1 : 16; /* [15..0]  */
		unsigned int lvds_imgheight1 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_IMGSIZE1_STATIS;

/* Define the union U_LVDS_LANE_ORDERS0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lvds_lane0_id : 4; /* [3..0]  */
		unsigned int lvds_lane1_id : 4; /* [7..4]  */
		unsigned int lvds_lane2_id : 4; /* [11..8]  */
		unsigned int lvds_lane3_id : 4; /* [15..12]  */
		unsigned int reserved_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_LANE_ORDERS0;

/* Define the union U_LVDS_CTRL_INT_RAW */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane0_sync_err_raw : 1; /* [0]  */
		unsigned int lane1_sync_err_raw : 1; /* [1]  */
		unsigned int lane2_sync_err_raw : 1; /* [2]  */
		unsigned int lane3_sync_err_raw : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int link0_wr_err_raw : 1; /* [16]  */
		unsigned int reserved_1 : 3; /* [19..17]  */
		unsigned int link0_rd_err_raw : 1; /* [20]  */
		unsigned int reserved_2 : 3; /* [23..21]  */
		unsigned int lvds_state_err_raw : 1; /* [24]  */
		unsigned int pop_err_raw : 1; /* [25]  */
		unsigned int cmd_wr_err_raw : 1; /* [26]  */
		unsigned int cmd_rd_err_raw : 1; /* [27]  */
		unsigned int lvds_vsync_raw : 1; /* [28]  */
		unsigned int reserved_3 : 3; /* [31..29]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_CTRL_INT_RAW;

/* Define the union U_LVDS_CTRL_INT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane0_sync_err_st : 1; /* [0]  */
		unsigned int lane1_sync_err_st : 1; /* [1]  */
		unsigned int lane2_sync_err_st : 1; /* [2]  */
		unsigned int lane3_sync_err_st : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int link0_wr_err_st : 1; /* [16]  */
		unsigned int reserved_1 : 3; /* [19..17]  */
		unsigned int link0_rd_err_st : 1; /* [20]  */
		unsigned int reserved_2 : 3; /* [23..21]  */
		unsigned int lvds_state_err_st : 1; /* [24]  */
		unsigned int pop_err_st : 1; /* [25]  */
		unsigned int cmd_wr_err_st : 1; /* [26]  */
		unsigned int cmd_rd_err_st : 1; /* [27]  */
		unsigned int lvds_vsync_st : 1; /* [28]  */
		unsigned int reserved_3 : 3; /* [31..29]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_CTRL_INT;

/* Define the union U_LVDS_CTRL_INT_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane0_sync_err_msk : 1; /* [0]  */
		unsigned int lane1_sync_err_msk : 1; /* [1]  */
		unsigned int lane2_sync_err_msk : 1; /* [2]  */
		unsigned int lane3_sync_err_msk : 1; /* [3]  */
		unsigned int reserved_0 : 12; /* [15..4]  */
		unsigned int link0_wr_err_msk : 1; /* [16]  */
		unsigned int reserved_1 : 3; /* [19..17]  */
		unsigned int link0_rd_err_msk : 1; /* [20]  */
		unsigned int reserved_2 : 3; /* [23..21]  */
		unsigned int lvds_state_err_msk : 1; /* [24]  */
		unsigned int pop_err_msk : 1; /* [25]  */
		unsigned int cmd_wr_err_msk : 1; /* [26]  */
		unsigned int cmd_rd_err_msk : 1; /* [27]  */
		unsigned int lvds_vsync_msk : 1; /* [28]  */
		unsigned int reserved_3 : 3; /* [31..29]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LVDS_CTRL_INT_MSK;

/* Define the union U_LANE_ID0_CHN(N) */
/* 0x1800+N*0x1000 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int lane0_id : 4;
		unsigned int lane1_id : 4;
		unsigned int lane2_id : 4;
		unsigned int lane3_id : 4;
		unsigned int reserved_0 : 16;
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_LANE_ID0_CHN;

/* Define the union U_ALIGN_INT_RAW */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err_full_raw : 1; /* [0]  */
		unsigned int err_lane0_raw : 1; /* [1]  */
		unsigned int err_lane1_raw : 1; /* [2]  */
		unsigned int err_lane2_raw : 1; /* [3]  */
		unsigned int err_lane3_raw : 1; /* [4]  */
		unsigned int reserved_0 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ALIGN_INT_RAW;

/* Define the union U_ALIGN_INT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err_full_st : 1; /* [0]  */
		unsigned int err_lane0_st : 1; /* [1]  */
		unsigned int err_lane1_st : 1; /* [2]  */
		unsigned int err_lane2_st : 1; /* [3]  */
		unsigned int err_lane3_st : 1; /* [4]  */
		unsigned int reserved_0 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ALIGN_INT;

/* Define the union U_ALIGN_INT_MSK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int err_full_mask : 1; /* [0]  */
		unsigned int err_lane0_mask : 1; /* [1]  */
		unsigned int err_lane1_mask : 1; /* [2]  */
		unsigned int err_lane2_mask : 1; /* [3]  */
		unsigned int err_lane3_mask : 1; /* [4]  */
		unsigned int reserved_0 : 27; /* [31..5]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ALIGN_INT_MSK;

/* Define the union U_CHN_INT_RAW */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_lvds_ctrl_raw : 1; /* [0]  */
		unsigned int int_mipi_csi_raw : 1; /* [1]  */
		unsigned int int_mipi_ctrl_raw : 1; /* [2]  */
		unsigned int int_data_align_raw : 1; /* [3]  */
		unsigned int reserved_0 : 28; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CHN_INT_RAW;

/* Define the union U_CHN_INT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_lvds_ctrl_st : 1; /* [0]  */
		unsigned int int_mipi_csi_st : 1; /* [1]  */
		unsigned int int_mipi_ctrl_st : 1; /* [2]  */
		unsigned int int_data_align_st : 1; /* [3]  */
		unsigned int reserved_0 : 28; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CHN_INT;

/* Define the union U_CHN_INT_MASK */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int int_lvds_ctrl_mask : 1; /* [0]  */
		unsigned int int_mipi_csi_mask : 1; /* [1]  */
		unsigned int int_mipi_ctrl_mask : 1; /* [2]  */
		unsigned int int_data_align_mask : 1; /* [3]  */
		unsigned int reserved_0 : 28; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_CHN_INT_MASK;

/* Define the global struct */
typedef struct {
	U_PHY_MODE_LINK PHY_MODE_LINK; /* 0x0 */
	U_PHY_SKEW_LINK PHY_SKEW_LINK; /* 0x4 */
	unsigned int reserved_0; /* 0x8 */
	U_PHY_EN_LINK PHY_EN_LINK; /* 0xc */
	unsigned int reserved_1; /* 0x10 */
	U_PHY_CFG_LINK PHY_CFG_LINK; /* 0x14 */
	U_PHY_DATA_LINK PHY_DATA_LINK; /* 0x18 */
	U_PHY_PH_MIPI_LINK PHY_PH_MIPI_LINK; /* 0x1c */
	U_PHY_DATA_MIPI_LINK PHY_DATA_MIPI_LINK; /* 0x20 */
	U_PHY_SYNC_DCT_LINK PHY_SYNC_DCT_LINK; /* 0x24 */
	unsigned int reserved_2[2]; /* 0x28~0x2c */
	U_PHY_SYNC_SOF0_LINK PHY_SYNC_SOF0_LINK; /* 0x30 */
	U_PHY_SYNC_SOF1_LINK PHY_SYNC_SOF1_LINK; /* 0x34 */
	U_PHY_SYNC_SOF2_LINK PHY_SYNC_SOF2_LINK; /* 0x38 */
	U_PHY_SYNC_SOF3_LINK PHY_SYNC_SOF3_LINK; /* 0x3c */
	U_PHY_OUT_VALID_LINK PHY_OUT_VALID_LINK; /* 0x40 */
	U_PHY_DATA_LVDS_LINK PHY_DATA_LVDS_LINK; /* 0x44 */
	unsigned int reserved_3[3]; /* 0x48~0x50 */
	U_PHY_PIX_PUM_LINK PHY_PIX_PUM_LINK; /* 0x54 */
	unsigned int reserved_4[2]; /* 0x58~0x5c */
	U_PHY_SYNC_SOL0_LINK PHY_SYNC_SOL0_LINK; /* 0x60 */
	U_PHY_SYNC_SOL1_LINK PHY_SYNC_SOL1_LINK; /* 0x64 */
	U_PHY_SYNC_SOL2_LINK PHY_SYNC_SOL2_LINK; /* 0x68 */
	U_PHY_SYNC_SOL3_LINK PHY_SYNC_SOL3_LINK; /* 0x6c */
	unsigned int reserved_5[36]; /* 0x70~0xfc */
	U_CIL_TIMEOUT_LINK CIL_TIMEOUT_LINK; /* 0x100 */
	U_CIL_FSM0_LINK CIL_FSM0_LINK; /* 0x104 */
	U_CIL_FSM_ST0_LINK CIL_FSM_ST0_LINK; /* 0x108 */
	U_CIL_FSM_ST1_LINK CIL_FSM_ST1_LINK; /* 0x10c */
	U_PHY_ST0_LINK PHY_ST0_LINK; /* 0x110 */
	U_PHY_ST1_LINK PHY_ST1_LINK; /* 0x114 */
	U_PHY_ST2_LINK PHY_ST2_LINK; /* 0x118 */
	U_PHY_ST3_LINK PHY_ST3_LINK; /* 0x11c */
	unsigned int reserved_6[3]; /* 0x120~0x128 */
	U_CIL_FSM1_LINK CIL_FSM1_LINK; /* 0x12c */
	unsigned int reserved_7[48]; /* 0x130~0x1ec */
	U_MIPI_CIL_INT_RAW_LINK MIPI_CIL_INT_RAW_LINK; /* 0x1f0 */
	U_MIPI_CIL_INT_LINK MIPI_CIL_INT_LINK; /* 0x1f4 */
	U_MIPI_CIL_INT_MSK_LINK MIPI_CIL_INT_MSK_LINK; /* 0x1f8 */
	unsigned int reserved_8[387]; /* 0x1fc~0x804 */

} mipi_rx_phy_cfg_t;

typedef struct {
	U_PHY_EN PHY_EN; /* 0x808 */
	U_LANE_EN LANE_EN; /* 0x80c */
	U_PHY_CIL_CTRL PHY_CIL_CTRL; /* 0x810 */
	unsigned int reserved_9; /* 0x814 */
	U_PHYCFG_MODE PHYCFG_MODE; /* 0x818 */
	U_PHYCFG_EN PHYCFG_EN; /* 0x81c */
	U_CHN0_MEM_CTRL CHN0_MEM_CTRL; /* 0x820 */
	U_CHN0_CLR_EN CHN0_CLR_EN; /* 0x824 */
	unsigned int reserved_10[22]; /* 0x828~0x87c */
	U_MIPI_TEST_PHY MIPI_TEST_PHY; /* 0x880 */
	unsigned int reserved_11[31]; /* 0x884~0x8fc */
	U_COLORBAR_CTRL_LINK0 COLORBAR_CTRL_LINK0; /* 0x900 */
	U_COLORBAR_INIT_LINK0 COLORBAR_INIT_LINK0; /* 0x904 */
	U_COLORBAR_V_BLK_LINK0 COLORBAR_V_BLK_LINK0; /* 0x908 */
	U_COLORBAR_H_BLK_LINK0 COLORBAR_H_BLK_LINK0; /* 0x90c */
	U_COLORBAR_IMG_SIZE_LINK0 COLORBAR_IMG_SIZE_LINK0; /* 0x910 */
	U_COLORBAR_SYNC_CODE0_LINK0 COLORBAR_SYNC_CODE0_LINK0; /* 0x914 */
	U_COLORBAR_SYNC_CODE1_LINK0 COLORBAR_SYNC_CODE1_LINK0; /* 0x918 */
	unsigned int reserved_12[57]; /* 0x91c~0x9fc */
	U_PHY0_TEST_OUT PHY0_TEST_OUT; /* 0xa00 */
	unsigned int reserved_13[379]; /* 0xa04~0xfec */
	U_MIPI_INT_RAW MIPI_INT_RAW; /* 0xff0 */
	U_MIPI_INT_ST MIPI_INT_ST; /* 0xff4 */
	U_MIPI_INT_MSK MIPI_INT_MSK; /* 0xff8 */
	unsigned int reserved_14[2]; /* 0xffc~0x1000 */

} mipi_rx_sys_regs_t;

typedef struct {
	U_MIPI_LANES_NUM MIPI_LANES_NUM; /* 0x1004 */
	unsigned int reserved_15; /* 0x1008 */
	U_MIPI_MAIN_INT_ST MIPI_MAIN_INT_ST; /* 0x100c */
	U_MIPI_DI_1 MIPI_DI_1; /* 0x1010 */
	U_MIPI_DI_2 MIPI_DI_2; /* 0x1014 */
	unsigned int reserved_16[18]; /* 0x1018~0x105c */
	U_MIPI_PKT_INTR_ST MIPI_PKT_INTR_ST; /* 0x1060 */
	U_MIPI_PKT_INTR_MSK MIPI_PKT_INTR_MSK; /* 0x1064 */
	U_MIPI_PKT_INTR_FORCE MIPI_PKT_INTR_FORCE; /* 0x1068 */
	unsigned int reserved_17; /* 0x106c */
	U_MIPI_PKT_INTR2_ST MIPI_PKT_INTR2_ST; /* 0x1070 */
	U_MIPI_PKT_INTR2_MSK MIPI_PKT_INTR2_MSK; /* 0x1074 */
	U_MIPI_PKT_INTR2_FORCE MIPI_PKT_INTR2_FORCE; /* 0x1078 */
	unsigned int reserved_18; /* 0x107c */
	U_MIPI_FRAME_INTR_ST MIPI_FRAME_INTR_ST; /* 0x1080 */
	U_MIPI_FRAME_INTR_MSK MIPI_FRAME_INTR_MSK; /* 0x1084 */
	U_MIPI_FRAME_INTR_FORCE MIPI_FRAME_INTR_FORCE; /* 0x1088 */
	unsigned int reserved_19; /* 0x108c */
	U_MIPI_LINE_INTR_ST MIPI_LINE_INTR_ST; /* 0x1090 */
	U_MIPI_LINE_INTR_MSK MIPI_LINE_INTR_MSK; /* 0x1094 */
	U_MIPI_LINE_INTR_FORCE MIPI_LINE_INTR_FORCE; /* 0x1098 */
	unsigned int reserved_20[25]; /* 0x109c~0x10fc */
	U_MIPI_USERDEF_DT MIPI_USERDEF_DT; /* 0x1100 */
	U_MIPI_USER_DEF MIPI_USER_DEF; /* 0x1104 */
	U_MIPI_CTRL_MODE_HS MIPI_CTRL_MODE_HS; /* 0x1108 */
	U_MIPI_VHEND_DELAY MIPI_VHEND_DELAY; /* 0x110c */
	unsigned int reserved_21[60]; /* 0x1110~0x11fc */
	U_MIPI_DOL_ID_CODE0 MIPI_DOL_ID_CODE0; /* 0x1200 */
	U_MIPI_DOL_ID_CODE1 MIPI_DOL_ID_CODE1; /* 0x1204 */
	U_MIPI_DOL_ID_CODE2 MIPI_DOL_ID_CODE2; /* 0x1208 */
	unsigned int reserved_22; /* 0x120c */
	U_MIPI_CROP_START_CHN0 MIPI_CROP_START_CHN0; /* 0x1210 */
	U_MIPI_CROP_START_CHN1 MIPI_CROP_START_CHN1; /* 0x1214 */
	unsigned int reserved_23[3]; /* 0x1218~0x1220 */
	U_MIPI_IMGSIZE MIPI_IMGSIZE; /* 0x1224 */
	unsigned int reserved_24[2]; /* 0x1228~0x122c */
	U_MIPI_CTRL_MODE_PIXEL MIPI_CTRL_MODE_PIXEL; /* 0x1230 */
	unsigned int reserved_25[3]; /* 0x1234~0x123c */
	U_MIPI_DUMMY_PIX_REG MIPI_DUMMY_PIX_REG; /* 0x1240 */
	unsigned int reserved_26[3]; /* 0x1244~0x124c */
	U_MIPI_IMGSIZE0_STATIS MIPI_IMGSIZE0_STATIS; /* 0x1250 */
	U_MIPI_IMGSIZE1_STATIS MIPI_IMGSIZE1_STATIS; /* 0x1254 */
	unsigned int reserved_27[38]; /* 0x1258~0x12ec */
	U_MIPI_CTRL_INT_RAW MIPI_CTRL_INT_RAW; /* 0x12f0 */
	U_MIPI_CTRL_INT MIPI_CTRL_INT; /* 0x12f4 */
	U_MIPI_CTRL_INT_MSK MIPI_CTRL_INT_MSK; /* 0x12f8 */
	unsigned int reserved_28; /* 0x12fc */

} mipi_ctrl_regs_t;

typedef struct {
	volatile U_LVDS_LANE_SOF_01 LVDS_LANE_SOF_01; /* 0x1320 */
	unsigned int reserved0; /* 0x1324 */
	volatile U_LVDS_LANE_EOF_01 LVDS_LANE_EOF_01; /* 0x1328 */
	unsigned int reserved1; /* 0x132c */
	volatile U_LVDS_LANE_SOL_01 LVDS_LANE_SOL_01; /* 0x1330 */
	unsigned int reserved2; /* 0x1334 */
	volatile U_LVDS_LANE_EOL_01 LVDS_LANE_EOL_01; /* 0x1338 */
	unsigned int reserved3; /* 0x133c */
} lvds_sync_code_cfg_t;

typedef struct {
	U_LVDS_WDR LVDS_WDR; /* 0x1300 */
	U_LVDS_DOLSCD_HBLK LVDS_DOLSCD_HBLK; /* 0x1304 */
	U_LVDS_CTRL LVDS_CTRL; /* 0x1308 */
	U_LVDS_IMGSIZE LVDS_IMGSIZE; /* 0x130c */
	U_LVDS_CROP_START0 LVDS_CROP_START0; /* 0x1310 */
	U_LVDS_CROP_START1 LVDS_CROP_START1; /* 0x1314 */
	unsigned int reserved_29[2]; /* 0x1318~0x131c */

	volatile lvds_sync_code_cfg_t
		lvds_this_frame_sync_code[16]; /* 0x1320~0x151c */
	volatile lvds_sync_code_cfg_t
		lvds_next_frame_sync_code[16]; /* 0x1520~0x171c */

	U_LVDS_LI_WORD0 LVDS_LI_WORD0; /* 0x1720 */
	U_LVDS_LI_WORD1 LVDS_LI_WORD1; /* 0x1724 */
	unsigned int reserved_32[2]; /* 0x1728~0x172c */
	U_LVDS_SYNC_BORD0 LVDS_SYNC_BORD0; /* 0x1730 */
	unsigned int reserved_33[3]; /* 0x1734~0x173c */
	U_LVDS_LANE_IMGSIZE_STATIS
	LVDS_LANE_IMGSIZE_STATIS[4]; /* 0x1740~0x174c */
	unsigned int reserved_34[12]; /* 0x1750~0x177c */
	U_LVDS_IMGSIZE0_STATIS LVDS_IMGSIZE0_STATIS; /* 0x1780 */
	U_LVDS_IMGSIZE1_STATIS LVDS_IMGSIZE1_STATIS; /* 0x1784 */
	unsigned int reserved_35[2]; /* 0x1788~0x178c */
	U_LVDS_LANE_ORDERS0 LVDS_LANE_ORDERS0; /* 0x1790 */
	unsigned int reserved_36[23]; /* 0x1794~0x17ec */
	U_LVDS_CTRL_INT_RAW LVDS_CTRL_INT_RAW; /* 0x17f0 */
	U_LVDS_CTRL_INT LVDS_CTRL_INT; /* 0x17f4 */
	U_LVDS_CTRL_INT_MSK LVDS_CTRL_INT_MSK; /* 0x17f8 */
	unsigned int reserved_37; /* 0x17fc */
	U_LANE_ID0_CHN LANE_ID0_CHN0; /* 0x1800 */
	unsigned int reserved_38[59]; /* 0x1804~0x18ec */
	U_ALIGN_INT_RAW ALIGN_INT_RAW; /* 0x18f0 */
	U_ALIGN_INT ALIGN_INT; /* 0x18f4 */
	U_ALIGN_INT_MSK ALIGN_INT_MSK; /* 0x18f8 */
	unsigned int reserved_39[445]; /* 0x18fc~0x1fec */
	U_CHN_INT_RAW CHN_INT_RAW; /* 0x1ff0 */
	U_CHN_INT CHN_INT; /* 0x1ff4 */
	U_CHN_INT_MASK CHN_INT_MASK; /* 0x1ff8 */

} lvds_ctrl_regs_t;

typedef struct {
	mipi_ctrl_regs_t mipi_ctrl_regs; /* 0x1004 ~ 0x12fc */
	lvds_ctrl_regs_t lvds_ctrl_regs; /* 0x1300 ~ 0x1ff8 */
} mipi_rx_ctrl_regs_t;

/* Define the global struct */
typedef struct {
	mipi_rx_phy_cfg_t mipi_rx_phy_cfg[1]; /* 0x0 ~ 0x804 */
	mipi_rx_sys_regs_t mipi_rx_sys_regs; /* 0x808 ~ 0x1000 */
	mipi_rx_ctrl_regs_t mipi_rx_ctrl_regs[2]; /* 0x1004 ~ 0x1ffc */
} mipi_rx_regs_type_t;

#endif /* __16E_MIPI_BREG_H__ */
