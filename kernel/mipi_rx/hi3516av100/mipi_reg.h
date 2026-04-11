#ifndef __MIPI_REG_H__
#define __MIPI_REG_H__

#include "hi_mipi.h"

/* Define the union U_MIPI_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_start            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_START;

/* Define the union U_MIPI_CTRL_PARAM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_mem_ck_gt        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CTRL_PARAM;

/* Define the union U_MIPI_PHY_PARAM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_hs_param          : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PHY_PARAM;

/* Define the union U_LVDS_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_imgwidth         : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    lvds_imgheight        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_IMGSIZE;

/* Define the union U_LVDS_WDR_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_wdr_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lvds_wdr_num          : 2   ; /* [5..4]  */
		unsigned int    reserved_1            : 2   ; /* [7..6]  */
		unsigned int    wdr_mode              : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_WDR_MODE;

/* Define the union U_MIPI_IMGSIZE_CHN0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth_statis_vc0   : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    imgheight_statis_vc0  : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_IMGSIZE_CHN0;

/* Define the union U_MIPI_IMGSIZE_CHN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth_statis_vc1   : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    imgheight_statis_vc1  : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_IMGSIZE_CHN1;

/* Define the union U_MIPI_IMGSIZE_CHN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth_statis_vc2   : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    imgheight_statis_vc2  : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_IMGSIZE_CHN2;

/* Define the union U_MIPI_IMGSIZE_CHN3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth_statis_vc3   : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    imgheight_statis_vc3  : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_IMGSIZE_CHN3;

/* Define the union U_MIPI_ERR_OBSRV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_num_err          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_ERR_OBSRV;

/* Define the union U_LVDS_CTRL_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_sync_mode        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lvds_raw_type         : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    lvds_pix_big_endian   : 1   ; /* [8]  */
        unsigned int    lvds_code_big_endian  : 1   ; /* [9]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    lvds_link_seq         : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CTRL_MODE;

/* Define the union U_INT_STAT_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    escape_ck_stat        : 1   ; /* [0]  */
        unsigned int    escape_d0_stat        : 1   ; /* [1]  */
        unsigned int    escape_d1_stat        : 1   ; /* [2]  */
        unsigned int    escape_d2_stat        : 1   ; /* [3]  */
        unsigned int    escape_d3_stat        : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    timeout_ck_stat       : 1   ; /* [8]  */
        unsigned int    timeout_d0_stat       : 1   ; /* [9]  */
        unsigned int    timeout_d1_stat       : 1   ; /* [10]  */
        unsigned int    timeout_d2_stat       : 1   ; /* [11]  */
        unsigned int    timeout_d3_stat       : 1   ; /* [12]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    lane0_sync_err_stat   : 1   ; /* [16]  */
        unsigned int    lane1_sync_err_stat   : 1   ; /* [17]  */
        unsigned int    lane2_sync_err_stat   : 1   ; /* [18]  */
        unsigned int    lane3_sync_err_stat   : 1   ; /* [19]  */
        unsigned int    hsync_err_stat        : 1   ; /* [20]  */
        unsigned int    vsync_err_stat        : 1   ; /* [21]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_STAT_LINK;

/* Define the union U_INT_MASK_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    escape_ck_mask        : 1   ; /* [0]  */
        unsigned int    escape_d0_mask        : 1   ; /* [1]  */
        unsigned int    escape_d1_mask        : 1   ; /* [2]  */
        unsigned int    escape_d2_mask        : 1   ; /* [3]  */
        unsigned int    escape_d3_mask        : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    timeout_ck_mask       : 1   ; /* [8]  */
        unsigned int    timeout_d0_mask       : 1   ; /* [9]  */
        unsigned int    timeout_d1_mask       : 1   ; /* [10]  */
        unsigned int    timeout_d2_mask       : 1   ; /* [11]  */
        unsigned int    timeout_d3_mask       : 1   ; /* [12]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    lane0_sync_err_mask   : 1   ; /* [16]  */
        unsigned int    lane1_sync_err_mask   : 1   ; /* [17]  */
        unsigned int    lane2_sync_err_mask   : 1   ; /* [18]  */
        unsigned int    lane3_sync_err_mask   : 1   ; /* [19]  */
        unsigned int    hsync_err_mask        : 1   ; /* [20]  */
        unsigned int    vsync_err_mask        : 1   ; /* [21]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_MASK_LINK;

/* Define the union U_INT_RAW_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    escape_ck_raw         : 1   ; /* [0]  */
        unsigned int    escape_d0_raw         : 1   ; /* [1]  */
        unsigned int    escape_d1_raw         : 1   ; /* [2]  */
        unsigned int    escape_d2_raw         : 1   ; /* [3]  */
        unsigned int    escape_d3_raw         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    timeout_ck_raw        : 1   ; /* [8]  */
        unsigned int    timeout_d0_raw        : 1   ; /* [9]  */
        unsigned int    timeout_d1_raw        : 1   ; /* [10]  */
        unsigned int    timeout_d2_raw        : 1   ; /* [11]  */
        unsigned int    timeout_d3_raw        : 1   ; /* [12]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    lane0_sync_err_raw    : 1   ; /* [16]  */
        unsigned int    lane1_sync_err_raw    : 1   ; /* [17]  */
        unsigned int    lane2_sync_err_raw    : 1   ; /* [18]  */
        unsigned int    lane3_sync_err_raw    : 1   ; /* [19]  */
        unsigned int    hsync_err_raw         : 1   ; /* [20]  */
        unsigned int    vsync_err_raw         : 1   ; /* [21]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_RAW_LINK;

/* Define the union U_INT_CLR_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    escape_ck_clr         : 1   ; /* [0]  */
        unsigned int    escape_d0_clr         : 1   ; /* [1]  */
        unsigned int    escape_d1_clr         : 1   ; /* [2]  */
        unsigned int    escape_d2_clr         : 1   ; /* [3]  */
        unsigned int    escape_d3_clr         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    timeout_ck_clr        : 1   ; /* [8]  */
        unsigned int    timeout_d0_clr        : 1   ; /* [9]  */
        unsigned int    timeout_d1_clr        : 1   ; /* [10]  */
        unsigned int    timeout_d2_clr        : 1   ; /* [11]  */
        unsigned int    timeout_d3_clr        : 1   ; /* [12]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    lane0_sync_err_clr    : 1   ; /* [16]  */
        unsigned int    lane1_sync_err_clr    : 1   ; /* [17]  */
        unsigned int    lane2_sync_err_clr    : 1   ; /* [18]  */
        unsigned int    lane3_sync_err_clr    : 1   ; /* [19]  */
        unsigned int    hsync_err_clr         : 1   ; /* [20]  */
        unsigned int    vsync_err_clr         : 1   ; /* [21]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INT_CLR_LINK;

/* Define the union U_LANE_EN_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_en              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lane1_en              : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    lane2_en              : 1   ; /* [8]  */
        unsigned int    reserved_2            : 3   ; /* [11..9]  */
        unsigned int    lane3_en              : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LANE_EN_LINK;

/* Define the union U_LANE_ID_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_id              : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    lane1_id              : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    lane2_id              : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    lane3_id              : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LANE_ID_LINK;

/* Define the union U_PHY_MODE_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_ifimode           : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    phy_sbsel             : 1   ; /* [4]  */
        unsigned int    phy_btsel             : 2   ; /* [6..5]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    phy_frpck             : 1   ; /* [8]  */
        unsigned int    phy_frsck             : 1   ; /* [9]  */
        unsigned int    reserved_2            : 6   ; /* [15..10]  */
        unsigned int    reserved_6            : 1   ; /* [16]  */
        unsigned int    reserved_7            : 1   ; /* [17]  */
        unsigned int    reserved_8            : 2   ; /* [19..18]  */
        unsigned int    phy_offset            : 2   ; /* [21..20]  */
        unsigned int    reserved_9            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_MODE_LINK;

/* Define the union U_PHY_POWER_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_pwdn              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    phy_rstn              : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_POWER_LINK;

/* Define the union U_PHY_DLY_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_dlyck             : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    phy_dlyd0             : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    phy_dlyd1             : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    phy_dlyd2             : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    phy_dlyd3             : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_DLY_LINK;

/* Define the union U_PHY_EN_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_lp_cken           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    phy_lp_d0en           : 1   ; /* [4]  */
        unsigned int    phy_lp_d1en           : 1   ; /* [5]  */
        unsigned int    phy_lp_d2en           : 1   ; /* [6]  */
        unsigned int    phy_lp_d3en           : 1   ; /* [7]  */
        unsigned int    phy_hs_cken           : 1   ; /* [8]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    phy_hs_d0en           : 1   ; /* [12]  */
        unsigned int    phy_hs_d1en           : 1   ; /* [13]  */
        unsigned int    phy_hs_d2en           : 1   ; /* [14]  */
        unsigned int    phy_hs_d3en           : 1   ; /* [15]  */
        unsigned int    phy_te_cken           : 1   ; /* [16]  */
        unsigned int    reserved_2            : 3   ; /* [19..17]  */
        unsigned int    phy_te_d0en           : 1   ; /* [20]  */
        unsigned int    phy_te_d1en           : 1   ; /* [21]  */
        unsigned int    phy_te_d2en           : 1   ; /* [22]  */
        unsigned int    phy_te_d3en           : 1   ; /* [23]  */
        unsigned int    reserved_3            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_EN_LINK;


/* Define the union U_PHY_CFG_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_rg_en_d           : 4   ; /* [3..0]  */
        unsigned int    phy_rg_en_clk         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    phy_rg_en_cmos        : 1   ; /* [8]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    phy_rg_en_lp          : 1   ; /* [12]  */
        unsigned int    reserved_2            : 3   ; /* [15..13]  */
        unsigned int    phy_da_ulpb           : 4   ; /* [19..16]  */
        unsigned int    phy_da_ulpb_ck        : 1   ; /* [20]  */
        unsigned int    reserved_3            : 3   ; /* [23..21]  */
        unsigned int    phy_rg_it_lp          : 2   ; /* [25..24]  */
        unsigned int    phy_rg_it_ck          : 2   ; /* [27..26]  */
        unsigned int    phy_rg_it_dt          : 2   ; /* [29..28]  */
        unsigned int    phy_rg_sa_en          : 1   ; /* [30]  */
        unsigned int    reserved_4            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_CFG_LINK;

/* Define the union U_MIPI_LANES_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_lane_num         : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_LANES_NUM;

/* Define the union U_MIPI_RSTN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    csi2_rstn             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_RSTN;

/* Define the union U_MIPI_DI_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    di0_dt                : 6   ; /* [5..0]  */
        unsigned int    di0_vc                : 2   ; /* [7..6]  */
        unsigned int    di1_dt                : 6   ; /* [13..8]  */
        unsigned int    di1_vc                : 2   ; /* [15..14]  */
        unsigned int    di2_dt                : 6   ; /* [21..16]  */
        unsigned int    di2_vc                : 2   ; /* [23..22]  */
        unsigned int    di3_dt                : 6   ; /* [29..24]  */
        unsigned int    di3_vc                : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_DI_1;

/* Define the union U_MIPI_DI_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    di4_dt                : 6   ; /* [5..0]  */
        unsigned int    di4_vc                : 2   ; /* [7..6]  */
        unsigned int    di5_dt                : 6   ; /* [13..8]  */
        unsigned int    di5_vc                : 2   ; /* [15..14]  */
        unsigned int    di6_dt                : 6   ; /* [21..16]  */
        unsigned int    di6_vc                : 2   ; /* [23..22]  */
        unsigned int    di7_dt                : 6   ; /* [29..24]  */
        unsigned int    di7_vc                : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_DI_2;

/* Define the union U_MIPI_INTR1_RAW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 1   ; /* [1]  */
        unsigned int    reserved_2            : 1   ; /* [2]  */
        unsigned int    reserved_3            : 1   ; /* [3]  */
        unsigned int    intr_f_bndry_vc0      : 1   ; /* [4]  */
        unsigned int    intr_f_bndry_vc1      : 1   ; /* [5]  */
        unsigned int    intr_f_bndry_vc2      : 1   ; /* [6]  */
        unsigned int    intr_f_bndry_vc3      : 1   ; /* [7]  */
        unsigned int    intr_f_seq_vc0        : 1   ; /* [8]  */
        unsigned int    intr_f_seq_vc1        : 1   ; /* [9]  */
        unsigned int    intr_f_seq_vc2        : 1   ; /* [10]  */
        unsigned int    intr_f_seq_vc3        : 1   ; /* [11]  */
        unsigned int    intr_f_crc_vc0        : 1   ; /* [12]  */
        unsigned int    intr_f_crc_vc1        : 1   ; /* [13]  */
        unsigned int    intr_f_crc_vc2        : 1   ; /* [14]  */
        unsigned int    intr_f_crc_vc3        : 1   ; /* [15]  */
        unsigned int    intr_l_bndy_di0       : 1   ; /* [16]  */
        unsigned int    intr_l_bndy_di1       : 1   ; /* [17]  */
        unsigned int    intr_l_bndy_di2       : 1   ; /* [18]  */
        unsigned int    intr_l_bndy_di3       : 1   ; /* [19]  */
        unsigned int    intr_l_seq_di0        : 1   ; /* [20]  */
        unsigned int    intr_l_seq_di1        : 1   ; /* [21]  */
        unsigned int    intr_l_seq_di2        : 1   ; /* [22]  */
        unsigned int    intr_l_seq_di3        : 1   ; /* [23]  */
        unsigned int    intr_crc_vc0          : 1   ; /* [24]  */
        unsigned int    intr_crc_vc1          : 1   ; /* [25]  */
        unsigned int    intr_crc_vc2          : 1   ; /* [26]  */
        unsigned int    intr_crc_vc3          : 1   ; /* [27]  */
        unsigned int    intr_ecc_double       : 1   ; /* [28]  */
        unsigned int    reserved_4            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_INTR1_RAW;

/* Define the union U_MIPI_INTR2_RAW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 1   ; /* [1]  */
        unsigned int    reserved_2            : 1   ; /* [2]  */
        unsigned int    reserved_3            : 1   ; /* [3]  */
        unsigned int    reserved_4            : 1   ; /* [4]  */
        unsigned int    reserved_5            : 1   ; /* [5]  */
        unsigned int    reserved_6            : 1   ; /* [6]  */
        unsigned int    reserved_7            : 1   ; /* [7]  */
        unsigned int    intr_vc0_ecc_corrected : 1   ; /* [8]  */
        unsigned int    intr_vc1_ecc_corrected : 1   ; /* [9]  */
        unsigned int    intr_vc2_ecc_corrected : 1   ; /* [10]  */
        unsigned int    intr_vc3_ecc_corrected : 1   ; /* [11]  */
        unsigned int    intr_err_id_vc0       : 1   ; /* [12]  */
        unsigned int    intr_err_id_vc1       : 1   ; /* [13]  */
        unsigned int    intr_err_id_vc2       : 1   ; /* [14]  */
        unsigned int    intr_err_id_vc3       : 1   ; /* [15]  */
        unsigned int    intr_l_bndry_di4      : 1   ; /* [16]  */
        unsigned int    intr_l_bndry_di5      : 1   ; /* [17]  */
        unsigned int    intr_l_bndry_di6      : 1   ; /* [18]  */
        unsigned int    intr_l_bndry_di7      : 1   ; /* [19]  */
        unsigned int    intr_l_seq_di4        : 1   ; /* [20]  */
        unsigned int    intr_l_seq_di5        : 1   ; /* [21]  */
        unsigned int    intr_l_seq_di6        : 1   ; /* [22]  */
        unsigned int    intr_l_seq_di7        : 1   ; /* [23]  */
        unsigned int    reserved_8            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_INTR2_RAW;

/* Define the union U_MIPI_INTR1_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 1   ; /* [1]  */
        unsigned int    reserved_2            : 1   ; /* [2]  */
        unsigned int    reserved_3            : 1   ; /* [3]  */
        unsigned int    mask_f_bndry_vc0      : 1   ; /* [4]  */
        unsigned int    mask_f_bndry_vc1      : 1   ; /* [5]  */
        unsigned int    mask_f_bndry_vc2      : 1   ; /* [6]  */
        unsigned int    mask_f_bndry_vc3      : 1   ; /* [7]  */
        unsigned int    mask_f_seq_vc0        : 1   ; /* [8]  */
        unsigned int    mask_f_seq_vc1        : 1   ; /* [9]  */
        unsigned int    mask_f_seq_vc2        : 1   ; /* [10]  */
        unsigned int    mask_f_seq_vc3        : 1   ; /* [11]  */
        unsigned int    mask_f_crc_vc0        : 1   ; /* [12]  */
        unsigned int    mask_f_crc_vc1        : 1   ; /* [13]  */
        unsigned int    mask_f_crc_vc2        : 1   ; /* [14]  */
        unsigned int    mask_f_crc_vc3        : 1   ; /* [15]  */
        unsigned int    mask_l_bndy_di0       : 1   ; /* [16]  */
        unsigned int    mask_l_bndy_di1       : 1   ; /* [17]  */
        unsigned int    mask_l_bndy_di2       : 1   ; /* [18]  */
        unsigned int    mask_l_bndy_di3       : 1   ; /* [19]  */
        unsigned int    mask_l_seq_di0        : 1   ; /* [20]  */
        unsigned int    mask_l_seq_di1        : 1   ; /* [21]  */
        unsigned int    mask_l_seq_di2        : 1   ; /* [22]  */
        unsigned int    mask_l_seq_di3        : 1   ; /* [23]  */
        unsigned int    mask_crc_vc0          : 1   ; /* [24]  */
        unsigned int    mask_crc_vc1          : 1   ; /* [25]  */
        unsigned int    mask_crc_vc2          : 1   ; /* [26]  */
        unsigned int    mask_crc_vc3          : 1   ; /* [27]  */
        unsigned int    mask_ecc_double       : 1   ; /* [28]  */
        unsigned int    reserved_4            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_INTR1_MASK;

/* Define the union U_MIPI_INTR2_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 1   ; /* [1]  */
        unsigned int    reserved_2            : 1   ; /* [2]  */
        unsigned int    reserved_3            : 1   ; /* [3]  */
        unsigned int    reserved_4            : 1   ; /* [4]  */
        unsigned int    reserved_5            : 1   ; /* [5]  */
        unsigned int    reserved_6            : 1   ; /* [6]  */
        unsigned int    reserved_7            : 1   ; /* [7]  */
        unsigned int    intr_vc0_ecc_corrected : 1   ; /* [8]  */
        unsigned int    intr_vc1_ecc_corrected : 1   ; /* [9]  */
        unsigned int    intr_vc2_ecc_corrected : 1   ; /* [10]  */
        unsigned int    intr_vc3_ecc_corrected : 1   ; /* [11]  */
        unsigned int    intr_err_id_vc0       : 1   ; /* [12]  */
        unsigned int    intr_err_id_vc1       : 1   ; /* [13]  */
        unsigned int    intr_err_id_vc2       : 1   ; /* [14]  */
        unsigned int    intr_err_id_vc3       : 1   ; /* [15]  */
        unsigned int    intr_l_bndry_di4      : 1   ; /* [16]  */
        unsigned int    intr_l_bndry_di5      : 1   ; /* [17]  */
        unsigned int    intr_l_bndry_di6      : 1   ; /* [18]  */
        unsigned int    intr_l_bndry_di7      : 1   ; /* [19]  */
        unsigned int    intr_l_seq_di4        : 1   ; /* [20]  */
        unsigned int    intr_l_seq_di5        : 1   ; /* [21]  */
        unsigned int    intr_l_seq_di6        : 1   ; /* [22]  */
        unsigned int    intr_l_seq_di7        : 1   ; /* [23]  */
        unsigned int    reserved_8            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_INTR2_MASK;

/* Define the union U_MIPI_TIMEOUT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    timeout_param         : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 6   ; /* [29..24]  */
        unsigned int    timeout_en_ck         : 1   ; /* [30]  */
        unsigned int    timeout_en_d          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_TIMEOUT;

/* Define the union U_MIPI_FSM_PARAM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cyc_data_lp00         : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    cyc_data_hs0          : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    cyc_clk_lp00          : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    cyc_clk_hs0           : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FSM_PARAM;

/* Define the union U_MIPI_FSM_CKEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_ck_cken          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    mipi_d0_cken          : 1   ; /* [4]  */
        unsigned int    mipi_d1_cken          : 1   ; /* [5]  */
        unsigned int    mipi_d2_cken          : 1   ; /* [6]  */
        unsigned int    mipi_d3_cken          : 1   ; /* [7]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FSM_CKEN;

/* Define the union U_LINK0_LANE0_SOF_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_sof_0           : 16  ; /* [15..0]  */
        unsigned int    lane0_sof_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_SOF_01;

/* Define the union U_LINK_LANE_SOF_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_sof_2           : 16  ; /* [15..0]  */
        unsigned int    lane0_sof_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_SOF_23;

/* Define the union U_LINK_LANE_EOF_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_eof_0           : 16  ; /* [15..0]  */
        unsigned int    lane0_eof_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_EOF_01;

/* Define the union U_LINK_LANE_EOF_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_eof_2           : 16  ; /* [15..0]  */
        unsigned int    lane0_eof_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_EOF_23;

/* Define the union U_LINK_LANE_SOL_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_sol_0           : 16  ; /* [15..0]  */
        unsigned int    lane0_sol_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_SOL_01;

/* Define the union U_LINK_LANE_SOL_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_sol_2           : 16  ; /* [15..0]  */
        unsigned int    lane0_sol_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_SOL_23;

/* Define the union U_LINK_LANE_EOL_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_eol_0           : 16  ; /* [15..0]  */
        unsigned int    lane0_eol_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_EOL_01;

/* Define the union U_LINK_LANE_EOL_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_eol_2           : 16  ; /* [15..0]  */
        unsigned int    lane0_eol_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_LANE_EOL_23;

/* Define the union U_DATA_PHY_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0_phy             : 8   ; /* [7..0]  */
        unsigned int    data1_phy             : 8   ; /* [15..8]  */
        unsigned int    data2_phy             : 8   ; /* [23..16]  */
        unsigned int    data3_phy             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DATA_PHY_LINK;

/* Define the union U_DATA_CHN_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data0_chn             : 8   ; /* [7..0]  */
        unsigned int    data1_chn             : 8   ; /* [15..8]  */
        unsigned int    data2_chn             : 8   ; /* [23..16]  */
        unsigned int    data3_chn             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DATA_CHN_LINK;

/* Define the union U_MIPI_FSMST_DATA_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_st_d0             : 4   ; /* [3..0]  */
        unsigned int    nxt_st_d0             : 4   ; /* [7..4]  */
        unsigned int    cur_st_d1             : 4   ; /* [11..8]  */
        unsigned int    nxt_st_d1             : 4   ; /* [15..12]  */
        unsigned int    cur_st_d2             : 4   ; /* [19..16]  */
        unsigned int    nxt_st_d2             : 4   ; /* [23..20]  */
        unsigned int    cur_st_d3             : 4   ; /* [27..24]  */
        unsigned int    nxt_st_d3             : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FSMST_DATA_LINK;

/* Define the union U_MIPI_FSMST_CK_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_st_ck             : 4   ; /* [3..0]  */
        unsigned int    nxt_st_ck             : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FSMST_CK_LINK;

/* Define the union U_MIPI_FIFO_ERR_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_data_fifo_wrerr  : 1   ; /* [0]  */
        unsigned int    mipi_data_fifo_rderr  : 1   ; /* [1]  */
        unsigned int    mipi_cmd_fifo_wrerr   : 1   ; /* [2]  */
        unsigned int    mipi_cmd_fifo_rderr   : 1   ; /* [3]  */
        unsigned int    lvds_fifo_wrerr       : 1   ; /* [4]  */
        unsigned int    lvds_fifo_rderr       : 1   ; /* [5]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FIFO_ERR_LINK;

/* Define the union U_PHY_BAK_EN0_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_pwdn_bak          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    phy_rstn_bak          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    phy_ifimode_bak       : 1   ; /* [8]  */
        unsigned int    reserved_2            : 22  ; /* [30..9]  */
        unsigned int    phy_bak_en0           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_BAK_EN0_LINK;

/* Define the union U_PHY_BAK_EN1_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lp_cken_bak           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lp_d0en_bak           : 1   ; /* [4]  */
        unsigned int    lp_d1en_bak           : 1   ; /* [5]  */
        unsigned int    lp_d2en_bak           : 1   ; /* [6]  */
        unsigned int    lp_d3en_bak           : 1   ; /* [7]  */
        unsigned int    hs_cken_bak           : 1   ; /* [8]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    hs_d0en_bak           : 1   ; /* [12]  */
        unsigned int    hs_d1en_bak           : 1   ; /* [13]  */
        unsigned int    hs_d2en_bak           : 1   ; /* [14]  */
        unsigned int    hs_d3en_bak           : 1   ; /* [15]  */
        unsigned int    te_cken_bak           : 1   ; /* [16]  */
        unsigned int    reserved_2            : 3   ; /* [19..17]  */
        unsigned int    te_d0en_bak           : 1   ; /* [20]  */
        unsigned int    te_d1en_bak           : 1   ; /* [21]  */
        unsigned int    te_d2en_bak           : 1   ; /* [22]  */
        unsigned int    te_d3en_bak           : 1   ; /* [23]  */
        unsigned int    reserved_3            : 7   ; /* [30..24]  */
        unsigned int    phy_bak_en1           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_BAK_EN1_LINK;

/* Define the union U_MIPI_PHY_ACT0_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_pwdn_act          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    phy_rstn_act          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    phy_ifimode_act       : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PHY_ACT0_LINK;

/* Define the union U_MIPI_PHY_ACT1_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lp_cken_act           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lp_d0en_act           : 1   ; /* [4]  */
        unsigned int    lp_d1en_act           : 1   ; /* [5]  */
        unsigned int    lp_d2en_act           : 1   ; /* [6]  */
        unsigned int    lp_d3en_act           : 1   ; /* [7]  */
        unsigned int    hs_cken_act           : 1   ; /* [8]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    hs_d0en_act           : 1   ; /* [12]  */
        unsigned int    hs_d1en_act           : 1   ; /* [13]  */
        unsigned int    hs_d2en_act           : 1   ; /* [14]  */
        unsigned int    hs_d3en_act           : 1   ; /* [15]  */
        unsigned int    te_cken_act           : 1   ; /* [16]  */
        unsigned int    reserved_2            : 3   ; /* [19..17]  */
        unsigned int    te_d0en_act           : 1   ; /* [20]  */
        unsigned int    te_d1en_act           : 1   ; /* [21]  */
        unsigned int    te_d2en_act           : 1   ; /* [22]  */
        unsigned int    te_d3en_act           : 1   ; /* [23]  */
        unsigned int    reserved_3            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PHY_ACT1_LINK;

/* Define the union U_LVDS_SYNC_BORD_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sync_bord_lane0       : 4   ; /* [3..0]  */
        unsigned int    sync_bord_lane1       : 4   ; /* [7..4]  */
        unsigned int    sync_bord_lane2       : 4   ; /* [11..8]  */
        unsigned int    sync_bord_lane3       : 4   ; /* [15..12]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_SYNC_BORD_LINK;


//==============================================================================
/* Define the mipi struct */

#if 0
typedef struct
{
    U_PHY_MODE_LINK        PHY_MODE_LINK;
    U_PHYCIL_PWDN          PHYCIL_PWDN;
    U_PHYCIL_RSTN          PHYCIL_RSTN;
    U_PHYCIL_DLY           PHYCIL_DLY;
    U_PHYCIL_LP_EN         PHYCIL_LP_EN;
    U_PHYCIL_HS_EN         PHYCIL_HS_EN;
    U_PHYCIL_TE_EN         PHYCIL_TE_EN;
    U_PHYCIL_INT_STAT      PHYCIL_INT_STAT;
    U_PHYCIL_INT_MASK      PHYCIL_INT_MASK;
    U_PHYCIL_INT_RAW       PHYCIL_INT_RAW;
    U_PHYCIL_INT_CLR       PHYCIL_INT_CLR;
}PHY_CTRL_S;
#endif

typedef struct
{
    volatile U_DATA_PHY_LINK            DATA_PHY_LINK                    ; /* 0x1200 */
    volatile U_DATA_CHN_LINK            DATA_CHN_LINK                    ; /* 0x1204 */
    volatile U_MIPI_FSMST_DATA_LINK     MIPI_FSMST_DATA_LINK          ; /* 0x1208 */
    volatile U_MIPI_FSMST_CK_LINK       MIPI_FSMST_CK_LINK              ; /* 0x120c */
    volatile U_MIPI_FIFO_ERR_LINK       MIPI_FIFO_ERR_LINK              ; /* 0x1210 */
    volatile U_PHY_BAK_EN0_LINK         PHY_BAK_EN0_LINK                 ; /* 0x1214 */
    volatile U_PHY_BAK_EN1_LINK         PHY_BAK_EN1_LINK                 ; /* 0x1218 */
    volatile U_MIPI_PHY_ACT0_LINK       MIPI_PHY_ACT0_LINK              ; /* 0x121c */
    volatile U_MIPI_PHY_ACT1_LINK       MIPI_PHY_ACT1_LINK              ; /* 0x1220 */
    volatile U_LVDS_SYNC_BORD_LINK      LVDS_SYNC_BORD_LINK            ; /* 0x1224 */    
}PHY_STATUS_S;

typedef struct
{
    volatile U_LINK_LANE_SOF_01   LINK_LANE_SOF_01                ; /* 0x1080 */
    volatile U_LINK_LANE_SOF_23   LINK_LANE_SOF_23                ; /* 0x1084 */
    volatile U_LINK_LANE_EOF_01   LINK_LANE_EOF_01                ; /* 0x1088 */
    volatile U_LINK_LANE_EOF_23   LINK_LANE_EOF_23                ; /* 0x108c */
    volatile U_LINK_LANE_SOL_01   LINK_LANE_SOL_01                ; /* 0x1090 */
    volatile U_LINK_LANE_SOL_23   LINK_LANE_SOL_23                ; /* 0x1094 */
    volatile U_LINK_LANE_EOL_01   LINK_LANE_EOL_01                ; /* 0x1098 */
    volatile U_LINK_LANE_EOL_23   LINK_LANE_EOL_23                ; /* 0x109c */
}U_LANE_SYNC_CODE;

typedef struct
{
    volatile U_INT_STAT_LINK        INT_STAT_LINK                    ; /* 0x1000 */
    volatile U_INT_MASK_LINK        INT_MASK_LINK                    ; /* 0x1004 */
    volatile U_INT_RAW_LINK         INT_RAW_LINK                     ; /* 0x1008 */
    volatile U_INT_CLR_LINK         INT_CLR_LINK                     ; /* 0x100c */
    volatile U_LANE_EN_LINK         LANE_EN_LINK                     ; /* 0x1010 */
    volatile U_LANE_ID_LINK         LANE_ID_LINK                     ; /* 0x1014 */
    volatile U_PHY_MODE_LINK        PHY_MODE_LINK                    ; /* 0x1018 */
    volatile U_PHY_POWER_LINK       PHY_POWER_LINK                   ; /* 0x101c */
    volatile U_PHY_DLY_LINK         PHY_DLY_LINK                     ; /* 0x1020 */
    volatile U_PHY_EN_LINK          PHY_EN_LINK                      ; /* 0x1024 */
    volatile U_PHY_CFG_LINK         PHY_CFG_LINK                     ; /* 0x1028 */
	volatile unsigned int           reserved_2                        ; /* 0x102c */
    volatile U_MIPI_LANES_NUM       MIPI_LANES_NUM                    ; /* 0x1030 */
    volatile U_MIPI_RSTN            MIPI_RSTN                         ; /* 0x1034 */
    volatile U_MIPI_DI_1            MIPI_DI_1                         ; /* 0x1038 */
    volatile U_MIPI_DI_2            MIPI_DI_2                         ; /* 0x103c */
    volatile U_MIPI_INTR1_RAW       MIPI_INTR1_RAW                    ; /* 0x1040 */
    volatile U_MIPI_INTR2_RAW       MIPI_INTR2_RAW                    ; /* 0x1044 */
    volatile U_MIPI_INTR1_MASK      MIPI_INTR1_MASK                   ; /* 0x1048 */
    volatile U_MIPI_INTR2_MASK      MIPI_INTR2_MASK                   ; /* 0x104c */
    volatile unsigned int           reserved_3[4]                     ; /* 0x1050~0x105c */
    volatile U_MIPI_TIMEOUT         MIPI_TIMEOUT                      ; /* 0x1060 */
    volatile U_MIPI_FSM_PARAM       MIPI_FSM_PARAM                    ; /* 0x1064 */
    volatile U_MIPI_FSM_CKEN        MIPI_FSM_CKEN                     ; /* 0x1068 */
	volatile unsigned int           reserved_4[5]                     ; /* 0x106c~0x107c */
    volatile U_LANE_SYNC_CODE       LANE_SYNC_CODE[LANE_NUM_PER_LINK];
	volatile unsigned int           reserved_5[64]                     ; /* 0x1100~0x11fc */	

    /* delete ?? */
    volatile PHY_STATUS_S           PHY_STATUS;
	volatile unsigned int           reserved_6[118]                     ; /* 0x1228~0x13fc */
}MIPI_LINK_REGS_S;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_MIPI_START           MIPI_START                        ; /* 0x0 */
    volatile U_MIPI_CTRL_PARAM      MIPI_CTRL_PARAM                   ; /* 0x4 */
    volatile unsigned int           reserved_0                        ; /* 0x8 */
    volatile U_LVDS_IMGSIZE         LVDS_IMGSIZE                      ; /* 0xc */
    volatile U_LVDS_WDR_MODE        LVDS_WDR_MODE                     ; /* 0x10 */
    volatile U_LVDS_CTRL_MODE       LVDS_CTRL_MODE                    ; /* 0x14 */
	volatile unsigned int           reserved_1[506]                   ; /* 0x18~0x7fc */
    volatile U_MIPI_IMGSIZE_CHN0    MIPI_IMGSIZE_CHN0                 ; /* 0x800 */
    volatile U_MIPI_IMGSIZE_CHN1    MIPI_IMGSIZE_CHN1                 ; /* 0x804 */

	#if 0
    volatile U_MIPI_IMGSIZE_CHN2    MIPI_IMGSIZE_CHN2                 ; /* 0x808 */
    volatile U_MIPI_IMGSIZE_CHN3    MIPI_IMGSIZE_CHN3                 ; /* 0x80c */
    volatile U_MIPI_ERR_OBSRV       MIPI_ERR_OBSRV                    ; /* 0x810 */
	#else
	volatile unsigned int           reserved_2[3];
	#endif	
	volatile unsigned int           reserved_3[507]                   ; /* 0x814~0xffc */

    MIPI_LINK_REGS_S                stLinkRegs[2];
} MIPI_REGS_TYPE_S;

#endif
