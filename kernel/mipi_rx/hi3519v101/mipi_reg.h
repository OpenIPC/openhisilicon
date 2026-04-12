// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  hi_mipi.h
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2013/3/10
// Description   :  MIPI Rx Register
// Others        :  Generated automatically by nManager V4.0.2.6 
// History       :  cWx281900 2015/05/14 10:51:21 Create file
// History       :  cWx281900 2015/05/26 20:16:55 update MIPI registers (MIPI_DOL_ID)
// History       :  cWx281900 2015/06/16 add MIPI soft reset register
//                                       add MIPI Main Int status register
//                                       update MIPI Int registers
// ******************************************************************************

#ifndef __MIPI_REG_H__
#define __MIPI_REG_H__

#include "hi_mipi.h"

//==============================================================================
/* Define the LINK REGS unions */

/* Define the union U_PHY_MODE_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     phy_rg_en_d          : 4   ; /* [3..0]  */
        unsigned int     phy_rg_en_clk        : 1   ; /* [4]  */
        unsigned int     reserved_0           : 3   ; /* [7..5]  */
        unsigned int     phy_rg_en_cmos       : 1   ; /* [8]  */
        unsigned int     reserved_1           : 3   ; /* [11..9]  */
        unsigned int     phy_rg_en_lp         : 1   ; /* [12]  */
        unsigned int     reserved_2           : 3   ; /* [15..13]  */
        unsigned int     phy_rg_faclk_en      : 1   ; /* [16]  */
        unsigned int     reserved_3           : 3   ; /* [19..17]  */
        unsigned int     phy_rg_ext_clk_en    : 1   ; /* [20]  */
        unsigned int     reserved_4           : 3   ; /* [23..21]  */
        unsigned int     phy_rg_mipi_mode     : 1   ; /* [24]  */
        unsigned int     reserved_5           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_MODE_LINK;

/* Define the union U_PHY_SKEW_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     phy_clk_skew         : 3   ; /* [2..0]  */
        unsigned int     reserved_0           : 1   ; /* [3]  */
        unsigned int     phy_d0_skew          : 3   ; /* [6..4]  */
        unsigned int     reserved_1           : 1   ; /* [7]  */
        unsigned int     phy_d1_skew          : 3   ; /* [10..8]  */
        unsigned int     reserved_2           : 1   ; /* [11]  */
        unsigned int     phy_d2_skew          : 3   ; /* [14..12]  */
        unsigned int     reserved_3           : 1   ; /* [15]  */
        unsigned int     phy_d3_skew          : 3   ; /* [18..16]  */
        unsigned int     reserved_4           : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_SKEW_LINK;

/* Define the union U_PHY_EN_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     phy_da_d0_valid      : 1   ; /* [0]  */
        unsigned int     phy_da_d1_valid      : 1   ; /* [1]  */
        unsigned int     phy_da_d2_valid      : 1   ; /* [2]  */
        unsigned int     phy_da_d3_valid      : 1   ; /* [3]  */
        unsigned int     phy_d0_term_en       : 1   ; /* [4]  */
        unsigned int     phy_d1_term_en       : 1   ; /* [5]  */
        unsigned int     phy_d2_term_en       : 1   ; /* [6]  */
        unsigned int     phy_d3_term_en       : 1   ; /* [7]  */
        unsigned int     phy_clk_term_en      : 1   ; /* [8]  */
        unsigned int     reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_EN_LINK;


/* Define the union U_PHY_CFG_LINK0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy0_da_ulpb          : 4   ; /* [3..0]  */
        unsigned int    phy0_da_ulpb_ck       : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    phy0_rg_it_lp         : 2   ; /* [9..8]  */
        unsigned int    phy0_rg_it_ck         : 2   ; /* [11..10]  */
        unsigned int    phy0_rg_it_dt         : 2   ; /* [13..12]  */
        unsigned int    phy0_rg_sa_en         : 1   ; /* [14]  */
        unsigned int    reserved_1            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_CFG_LINK;


/* Define the union U_PHY_DATA_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     phy_data0_mipi       : 8   ; /* [7..0]  */
        unsigned int     phy_data1_mipi       : 8   ; /* [15..8]  */
        unsigned int     phy_data2_mipi       : 8   ; /* [23..16]  */
        unsigned int     phy_data3_mipi       : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_DATA_LINK;

/* Define the union U_PHY_PH_MIPI_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     phy_ph0_mipi         : 8   ; /* [7..0]  */
        unsigned int     phy_ph1_mipi         : 8   ; /* [15..8]  */
        unsigned int     phy_ph2_mipi         : 8   ; /* [23..16]  */
        unsigned int     phy_ph3_mipi         : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_PH_MIPI_LINK;

/* Define the union U_PHY_DATA_MIPI_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     phy_data0_mipi_hs    : 8   ; /* [7..0]  */
        unsigned int     phy_data1_mipi_hs    : 8   ; /* [15..8]  */
        unsigned int     phy_data2_mipi_hs    : 8   ; /* [23..16]  */
        unsigned int     phy_data3_mipi_hs    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_DATA_MIPI_LINK;

/* Define the union U_PHY_SYNC_DCT_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     cil_raw_type         : 3   ; /* [2..0]  */
        unsigned int     reserved_0            : 1   ; /* [3]  */
        unsigned int     cil_split_mode       : 3   ; /* [6..4]  */
        unsigned int     reserved_1            : 1   ; /* [7]  */
        unsigned int     cil_code_big_endian  : 1   ; /* [8]  */
        unsigned int     reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_SYNC_DCT_LINK;

/* Define the union U_PHY_SYNC_CODE_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     cil_sof0_word4     : 16  ; /* [15..0]  */
        unsigned int     cil_sof1_word4     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_SYNC_CODE_LINK;

/* Define the union U_PHY_OUT_VALID_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cil0_lvds_out_valid   : 4   ; /* [3..0]  */
        unsigned int    cil0_mipi_out_valid   : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_OUT_VALID_LINK;

/* Define the union U_PHY_DATA_LVDS_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy0_data0_lvds_hs    : 8   ; /* [7..0]  */
        unsigned int    phy0_data1_lvds_hs    : 8   ; /* [15..8]  */
        unsigned int    phy0_data2_lvds_hs    : 8   ; /* [23..16]  */
        unsigned int    phy0_data3_lvds_hs    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_DATA_LVDS_LINK;


/* Define the union U_MIPI_CIL_INT_RAW_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     err_escape_d0_raw    : 1   ; /* [0]  */
        unsigned int     err_escape_d1_raw    : 1   ; /* [1]  */
        unsigned int     err_escape_d2_raw    : 1   ; /* [2]  */
        unsigned int     err_escape_d3_raw    : 1   ; /* [3]  */
        unsigned int     err_escape_ck_raw    : 1   ; /* [4]  */
        unsigned int     reserved_0           : 3   ; /* [7..5]  */
        unsigned int     err_timeout_d0_raw   : 1   ; /* [8]  */
        unsigned int     err_timeout_d1_raw   : 1   ; /* [9]  */
        unsigned int     err_timeout_d2_raw   : 1   ; /* [10]  */
        unsigned int     err_timeout_d3_raw   : 1   ; /* [11]  */
        unsigned int     err_timeout_ck_raw   : 1   ; /* [12]  */
        unsigned int     reserved_1           : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CIL_INT_RAW_LINK;

/* Define the union U_MIPI_CIL_INT_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     err_escape_d0_st     : 1   ; /* [0]  */
        unsigned int     err_escape_d1_st     : 1   ; /* [1]  */
        unsigned int     err_escape_d2_st     : 1   ; /* [2]  */
        unsigned int     err_escape_d3_st     : 1   ; /* [3]  */
        unsigned int     err_escape_ck_st     : 1   ; /* [4]  */
        unsigned int     reserved_0           : 3   ; /* [7..5]  */
        unsigned int     err_timeout_d0_st    : 1   ; /* [8]  */
        unsigned int     err_timeout_d1_st    : 1   ; /* [9]  */
        unsigned int     err_timeout_d2_st    : 1   ; /* [10]  */
        unsigned int     err_timeout_d3_st    : 1   ; /* [11]  */
        unsigned int     err_timeout_ck_st    : 1   ; /* [12]  */
        unsigned int     reserved_1           : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CIL_INT_LINK;

/* Define the union U_MIPI_CIL_INT_MSK_LINK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int     err_escape_d0_msk    : 1   ; /* [0]  */
        unsigned int     err_escape_d1_msk    : 1   ; /* [1]  */
        unsigned int     err_escape_d2_msk    : 1   ; /* [2]  */
        unsigned int     err_escape_d3_msk    : 1   ; /* [3]  */
        unsigned int     err_escape_ck_msk    : 1   ; /* [4]  */
        unsigned int     reserved_0           : 3   ; /* [7..5]  */
        unsigned int     err_timeout_d0_msk   : 1   ; /* [8]  */
        unsigned int     err_timeout_d1_msk   : 1   ; /* [9]  */
        unsigned int     err_timeout_d2_msk   : 1   ; /* [10]  */
        unsigned int     err_timeout_d3_msk   : 1   ; /* [11]  */
        unsigned int     err_timeout_ck_msk   : 1   ; /* [12]  */
        unsigned int     reserved_1           : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CIL_INT_MSK_LINK;




/* 3519V101 nwe add register  */
/* Define the union U_PHY_ID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy2_id               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    phy1_id               : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_ID;

/* Define the union U_PHY_LP_SELECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy1_lp_sel           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_LP_SELECT;

/* Define the union U_CHN0_PHY_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy0_en               : 1   ; /* [0]  */
        unsigned int    phy1_en               : 1   ; /* [1]  */
        unsigned int    phy2_en               : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_PHY_EN;

/* Define the union U_CHN0_HS_CK_SELECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hs0_ck_sel            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    hs1_ck_sel            : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    hs2_ck_sel            : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_HS_CK_SELECT;

/* Define the union U_CHN0_MEM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mem_ret1n             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    mem_emaa              : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    mem_emab              : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    mem_ck_gt             : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_MEM_CTRL;

/* Define the union U_CHN0_LANE_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_en              : 1   ; /* [0]  */
        unsigned int    lane1_en              : 1   ; /* [1]  */
        unsigned int    lane2_en              : 1   ; /* [2]  */
        unsigned int    lane3_en              : 1   ; /* [3]  */
        unsigned int    lane4_en              : 1   ; /* [4]  */
        unsigned int    lane5_en              : 1   ; /* [5]  */
        unsigned int    lane6_en              : 1   ; /* [6]  */
        unsigned int    lane7_en              : 1   ; /* [7]  */
        unsigned int    lane8_en              : 1   ; /* [8]  */
        unsigned int    lane9_en              : 1   ; /* [9]  */
        unsigned int    lane10_en             : 1   ; /* [10]  */
        unsigned int    lane11_en             : 1   ; /* [11]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_LANE_EN;

/* Define the union U_CHN0_PHYCFG_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn0_phycfg_mode      : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_PHYCFG_MODE;

/* Define the union U_CHN0_PHYCFG_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn0_phycfg_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_PHYCFG_EN;

/* Define the union U_CHN0_CLR_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clr_en_lvds0          : 1   ; /* [0]  */
        unsigned int    clr_en_align0         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_CLR_EN;

/* Define the union U_CHN0_CIL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phycil0_cken          : 1   ; /* [0]  */
        unsigned int    phycil1_cken          : 1   ; /* [1]  */
        unsigned int    phycil2_cken          : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN0_CIL_CTRL;

/* Define the union U_CHN1_PHY_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn1_phy2_en          : 1   ; /* [0]  */
        unsigned int    chn1_phy1_en          : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_PHY_EN;

/* Define the union U_CHN1_HS_CK_SELECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn1_hs2_ck_sel       : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    chn1_hs1_ck_sel       : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_HS_CK_SELECT;

/* Define the union U_CHN1_MEM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn1_mem_ret1n        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    chn1_mem_emaa         : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    chn1_mem_emab         : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    chn1_mem_ck_gt        : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_MEM_CTRL;

/* Define the union U_CHN1_LANE_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_en         : 1   ; /* [0]  */
        unsigned int    lane1_en         : 1   ; /* [1]  */
        unsigned int    lane2_en         : 1   ; /* [2]  */
        unsigned int    lane3_en         : 1   ; /* [3]  */
        unsigned int    lane4_en         : 1   ; /* [4]  */
        unsigned int    lane5_en         : 1   ; /* [5]  */
        unsigned int    lane6_en         : 1   ; /* [6]  */
        unsigned int    lane7_en         : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_LANE_EN;

/* Define the union U_CHN1_PHYCFG_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn1_phycfg_mode      : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_PHYCFG_MODE;

/* Define the union U_CHN1_PHYCFG_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn1_phycfg_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_PHYCFG_EN;

/* Define the union U_CHN1_CLR_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clr_en_lvds1          : 1   ; /* [0]  */
        unsigned int    clr_en_align1         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_CLR_EN;

/* Define the union U_CHN1_CIL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn1_phycil2_cken     : 1   ; /* [0]  */
        unsigned int    chn1_phycil1_cken     : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CHN1_CIL_CTRL;



#if 0
/* Define the union U_PHY_CHN_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy0_en               : 1   ; /* [0]  */
        unsigned int    phy1_en               : 1   ; /* [1]  */
        unsigned int    phy2_en               : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    reserved_1            : 1   ; /* [4]  */
        unsigned int    reserved_2            : 3   ; /* [7..5]  */
        unsigned int    phy2_id               : 1   ; /* [8]  */
        unsigned int    reserved_3            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_CHN_CTRL;

/* Define the union U_PHY_LP_SELECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy1_lp_sel           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PHY_LP_SELECT;

/* Define the union U_MIPI_MEM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mem_ret1n             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    mem_emaa              : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    mem_emab              : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    mem_ck_gt             : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_MEM_CTRL;

/* Define the union U_LANE_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_en              : 1   ; /* [0]  */
        unsigned int    lane1_en              : 1   ; /* [1]  */
        unsigned int    lane2_en              : 1   ; /* [2]  */
        unsigned int    lane3_en              : 1   ; /* [3]  */
        unsigned int    lane4_en              : 1   ; /* [4]  */
        unsigned int    lane5_en              : 1   ; /* [5]  */
        unsigned int    lane6_en              : 1   ; /* [6]  */
        unsigned int    lane7_en              : 1   ; /* [7]  */
        unsigned int    lane8_en              : 1   ; /* [8]  */
        unsigned int    lane9_en              : 1   ; /* [9]  */
        unsigned int    lane10_en             : 1   ; /* [10]  */
        unsigned int    lane11_en             : 1   ; /* [11]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LANE_EN;

/* Define the union U_MIPI_PHYCFG_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi0_phycfg_mode     : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    mipi1_phycfg_mode     : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PHYCFG_MODE;

/* Define the union U_MIPI_PHYCFG_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chn0_phycfg_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    chn1_phycfg_en        : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PHYCFG_EN;

/* Define the union U_MIPI_CIL_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phycil0_cken          : 1   ; /* [0]  */
        unsigned int    phycil1_cken          : 1   ; /* [1]  */
        unsigned int    phycil2_cken          : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    clr_en_lvds           : 1   ; /* [4]  */
        unsigned int    clr_en_align          : 1   ; /* [5]  */
        unsigned int    reserved_1            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CIL_CTRL;

/* Define the union U_MIPI_SRST_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
            unsigned int    mipi_srst_aux         : 1   ; /* [0]  */
            unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_SRST_CFG;

#endif

//==============================================================================
/* Define the MIPI0 and MIPI1 Interrupt Regs unions */
/* Define the union U_MIPI_CHN0_INT_RAW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_phycil0_raw       : 1   ; /* [0]  */
        unsigned int    int_phycil1_raw       : 1   ; /* [1]  */
        unsigned int    int_phycil2_raw       : 1   ; /* [2]  */
        unsigned int    int_lvds_ctrl_raw     : 1   ; /* [3]  */
        unsigned int    int_mipi_csi_raw      : 1   ; /* [4]  */
        unsigned int    int_mipi_ctrl_raw     : 1   ; /* [5]  */
        unsigned int    int_data_align_raw    : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CHN0_INT_RAW;

/* Define the union U_MIPI_CHN0_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_phycil0_st        : 1   ; /* [0]  */
        unsigned int    int_phycil1_st        : 1   ; /* [1]  */
        unsigned int    int_phycil2_st        : 1   ; /* [2]  */
        unsigned int    int_lvds_ctrl_st      : 1   ; /* [3]  */
        unsigned int    int_mipi_csi_st       : 1   ; /* [4]  */
        unsigned int    int_mipi_ctrl_st      : 1   ; /* [5]  */
        unsigned int    int_data_align_st     : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CHN0_INT;

/* Define the union U_MIPI_CHN0_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_phycil0_msk       : 1   ; /* [0]  */
        unsigned int    int_phycil1_msk       : 1   ; /* [1]  */
        unsigned int    int_phycil2_msk       : 1   ; /* [2]  */
        unsigned int    int_lvds_ctrl_msk     : 1   ; /* [3]  */
        unsigned int    int_mipi_csi_msk      : 1   ; /* [4]  */
        unsigned int    int_mipi_ctrl_msk     : 1   ; /* [5]  */
        unsigned int    int_data_align_msk    : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CHN0_INT_MSK;

/* Define the union U_MIPI_CHN1_INT_RAW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    int_phycil2_raw       : 1   ; /* [2]  */
        unsigned int    int_lvds_ctrl_raw     : 1   ; /* [3]  */
        unsigned int    int_mipi_csi_raw      : 1   ; /* [4]  */
        unsigned int    int_mipi_ctrl_raw     : 1   ; /* [5]  */
        unsigned int    int_data_align_raw    : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CHN1_INT_RAW;

/* Define the union U_MIPI_CHN1_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    chn1_int_phycil2_st   : 1   ; /* [2]  */
        unsigned int    chn1_int_lvds_ctrl_st : 1   ; /* [3]  */
        unsigned int    chn1_int_mipi_csi_st  : 1   ; /* [4]  */
        unsigned int    chn1_int_mipi_ctrl_st : 1   ; /* [5]  */
        unsigned int    chn1_int_data_align_st : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CHN1_INT;

/* Define the union U_MIPI_CHN1_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    chn1_int_phycil2_msk  : 1   ; /* [2]  */
        unsigned int    chn1_int_lvds_msk     : 1   ; /* [3]  */
        unsigned int    chn1_int_mipi_csi_msk : 1   ; /* [4]  */
        unsigned int    chn1_int_mipi_ctrl_msk : 1   ; /* [5]  */
        unsigned int    chn1_int_data_align_msk : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CHN1_INT_MSK;


//==============================================================================
/* Define the MIPI_CTRL Regs */
/* Define the union U_MIPI_LANES_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_num              : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_LANES_NUM;

/* Define the union U_MIPI_MAIN_INT_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    status_int_pkt_fatal  : 1   ; /* [1]  */
        unsigned int    status_int_frame_fatal : 1   ; /* [2]  */
        unsigned int    reserved_1            : 14  ; /* [16..3]  */
        unsigned int    status_int_pkt        : 1   ; /* [17]  */
        unsigned int    status_int_line       : 1   ; /* [18]  */
        unsigned int    reserved_2            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_MAIN_INT_ST;

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

/* Define the union U_MIPI_PKT_INTR_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vc0_err_crc           : 1   ; /* [0]  */
        unsigned int    vc1_err_crc           : 1   ; /* [1]  */
        unsigned int    vc2_err_crc           : 1   ; /* [2]  */
        unsigned int    vc3_err_crc           : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    err_ecc_double        : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PKT_INTR_ST;

/* Define the union U_MIPI_PKT_INTR_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mask_vc0_err_crc      : 1   ; /* [0]  */
        unsigned int    mask_vc1_err_crc      : 1   ; /* [1]  */
        unsigned int    mask_vc2_err_crc      : 1   ; /* [2]  */
        unsigned int    mask_vc3_err_crc      : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    mask_err_ecc_double   : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PKT_INTR_MSK;


/* Define the union U_MIPI0_PKT_INTR_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    force_vc0_err_crc     : 1   ; /* [0]  */
        unsigned int    force_vc1_err_crc     : 1   ; /* [1]  */
        unsigned int    force_vc2_err_crc     : 1   ; /* [2]  */
        unsigned int    force_vc3_err_crc     : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    force_err_ecc_double  : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PKT_INTR_FORCE;

/* Define the union U_MIPI_PKT_INTR2_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    err_id_vc0            : 1   ; /* [0]  */
        unsigned int    err_id_vc1            : 1   ; /* [1]  */
        unsigned int    err_id_vc2            : 1   ; /* [2]  */
        unsigned int    err_id_vc3            : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    vc0_err_ecc_corrected : 1   ; /* [16]  */
        unsigned int    vc1_err_ecc_corrected : 1   ; /* [17]  */
        unsigned int    vc2_err_ecc_corrected : 1   ; /* [18]  */
        unsigned int    vc3_err_ecc_corrected : 1   ; /* [19]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PKT_INTR2_ST;

/* Define the union U_MIPI_PKT_INTR2_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mask_err_id_vc0       : 1   ; /* [0]  */
        unsigned int    mask_err_id_vc1       : 1   ; /* [1]  */
        unsigned int    mask_err_id_vc2       : 1   ; /* [2]  */
        unsigned int    mask_err_id_vc3       : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    mask_vc0_err_ecc_corrected : 1   ; /* [16]  */
        unsigned int    mask_vc1_err_ecc_corrected : 1   ; /* [17]  */
        unsigned int    mask_vc2_err_ecc_corrected : 1   ; /* [18]  */
        unsigned int    mask_vc3_err_ecc_corrected : 1   ; /* [19]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PKT_INTR2_MSK;



/* Define the union U_MIPI0_PKT_INTR2_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    force_err_id_vc0      : 1   ; /* [0]  */
        unsigned int    force_err_id_vc1      : 1   ; /* [1]  */
        unsigned int    force_err_id_vc2      : 1   ; /* [2]  */
        unsigned int    force_err_id_vc3      : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    force_vc0_err_ecc_corrected : 1   ; /* [16]  */
        unsigned int    force_vc1_err_ecc_corrected : 1   ; /* [17]  */
        unsigned int    force_vc2_err_ecc_corrected : 1   ; /* [18]  */
        unsigned int    force_vc3_err_ecc_corrected : 1   ; /* [19]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_PKT_INTR2_FORCE;


/* Define the union U_MIPI_FRAME_INTR_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    err_f_bndry_match_vc0 : 1   ; /* [0]  */
        unsigned int    err_f_bndry_match_vc1 : 1   ; /* [1]  */
        unsigned int    err_f_bndry_match_vc2 : 1   ; /* [2]  */
        unsigned int    err_f_bndry_match_vc3 : 1   ; /* [3]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    err_f_seq_vc0         : 1   ; /* [8]  */
        unsigned int    err_f_seq_vc1         : 1   ; /* [9]  */
        unsigned int    err_f_seq_vc2         : 1   ; /* [10]  */
        unsigned int    err_f_seq_vc3         : 1   ; /* [11]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    err_frame_data_vc0    : 1   ; /* [16]  */
        unsigned int    err_frame_data_vc1    : 1   ; /* [17]  */
        unsigned int    err_frame_data_vc2    : 1   ; /* [18]  */
        unsigned int    err_frame_data_vc3    : 1   ; /* [19]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FRAME_INTR_ST;

/* Define the union U_MIPI_FRAME_INTR_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mask_err_f_bndry_match_vc0 : 1   ; /* [0]  */
        unsigned int    mask_err_f_bndry_match_vc1 : 1   ; /* [1]  */
        unsigned int    mask_err_f_bndry_match_vc2 : 1   ; /* [2]  */
        unsigned int    mask_err_f_bndry_match_vc3 : 1   ; /* [3]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    mask_err_f_seq_vc0    : 1   ; /* [8]  */
        unsigned int    mask_err_f_seq_vc1    : 1   ; /* [9]  */
        unsigned int    mask_err_f_seq_vc2    : 1   ; /* [10]  */
        unsigned int    mask_err_f_seq_vc3    : 1   ; /* [11]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    mask_err_frame_data_vc0 : 1   ; /* [16]  */
        unsigned int    mask_err_frame_data_vc1 : 1   ; /* [17]  */
        unsigned int    mask_err_frame_data_vc2 : 1   ; /* [18]  */
        unsigned int    mask_err_frame_data_vc3 : 1   ; /* [19]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FRAME_INTR_MSK;


/* Define the union U_MIPI0_FRAME_INTR_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    force_err_f_bndry_match_vc0 : 1   ; /* [0]  */
        unsigned int    force_err_f_bndry_match_vc1 : 1   ; /* [1]  */
        unsigned int    force_err_f_bndry_match_vc2 : 1   ; /* [2]  */
        unsigned int    force_err_f_bndry_match_vc3 : 1   ; /* [3]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    force_err_f_seq_vc0   : 1   ; /* [8]  */
        unsigned int    force_err_f_seq_vc1   : 1   ; /* [9]  */
        unsigned int    force_err_f_seq_vc2   : 1   ; /* [10]  */
        unsigned int    force_err_f_seq_vc3   : 1   ; /* [11]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    force_err_frame_data_vc0 : 1   ; /* [16]  */
        unsigned int    force_err_frame_data_vc1 : 1   ; /* [17]  */
        unsigned int    force_err_frame_data_vc2 : 1   ; /* [18]  */
        unsigned int    force_err_frame_data_vc3 : 1   ; /* [19]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_FRAME_INTR_FORCE;

/* Define the union U_MIPI_LINE_INTR_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    err_l_bndry_match_di0 : 1   ; /* [0]  */
        unsigned int    err_l_bndry_match_di1 : 1   ; /* [1]  */
        unsigned int    err_l_bndry_match_di2 : 1   ; /* [2]  */
        unsigned int    err_l_bndry_match_di3 : 1   ; /* [3]  */
        unsigned int    err_l_bndry_match_di4 : 1   ; /* [4]  */
        unsigned int    err_l_bndry_match_di5 : 1   ; /* [5]  */
        unsigned int    err_l_bndry_match_di6 : 1   ; /* [6]  */
        unsigned int    err_l_bndry_match_di7 : 1   ; /* [7]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    err_l_seq_di0         : 1   ; /* [16]  */
        unsigned int    err_l_seq_di1         : 1   ; /* [17]  */
        unsigned int    err_l_seq_di2         : 1   ; /* [18]  */
        unsigned int    err_l_seq_di3         : 1   ; /* [19]  */
        unsigned int    err_l_seq_di4         : 1   ; /* [20]  */
        unsigned int    err_l_seq_di5         : 1   ; /* [21]  */
        unsigned int    err_l_seq_di6         : 1   ; /* [22]  */
        unsigned int    err_l_seq_di7         : 1   ; /* [23]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_LINE_INTR_ST;

/* Define the union U_MIPI_LINE_INTR_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mask_err_l_bndry_match_di0 : 1   ; /* [0]  */
        unsigned int    mask_err_l_bndry_match_di1 : 1   ; /* [1]  */
        unsigned int    mask_err_l_bndry_match_di2 : 1   ; /* [2]  */
        unsigned int    mask_err_l_bndry_match_di3 : 1   ; /* [3]  */
        unsigned int    mask_err_l_bndry_match_di4 : 1   ; /* [4]  */
        unsigned int    mask_err_l_bndry_match_di5 : 1   ; /* [5]  */
        unsigned int    mask_err_l_bndry_match_di6 : 1   ; /* [6]  */
        unsigned int    mask_err_l_bndry_match_di7 : 1   ; /* [7]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    mask_err_l_seq_di0    : 1   ; /* [16]  */
        unsigned int    mask_err_l_seq_di1    : 1   ; /* [17]  */
        unsigned int    mask_err_l_seq_di2    : 1   ; /* [18]  */
        unsigned int    mask_err_l_seq_di3    : 1   ; /* [19]  */
        unsigned int    mask_err_l_seq_di4    : 1   ; /* [20]  */
        unsigned int    mask_err_l_seq_di5    : 1   ; /* [21]  */
        unsigned int    mask_err_l_seq_di6    : 1   ; /* [22]  */
        unsigned int    mask_err_l_seq_di7    : 1   ; /* [23]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_LINE_INTR_MSK;


/* Define the union U_MIPI0_LINE_INTR_FORCE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    force_err_l_bndry_match_di0 : 1   ; /* [0]  */
        unsigned int    force_err_l_bndry_match_di1 : 1   ; /* [1]  */
        unsigned int    force_err_l_bndry_match_di2 : 1   ; /* [2]  */
        unsigned int    force_err_l_bndry_match_di3 : 1   ; /* [3]  */
        unsigned int    force_err_l_bndry_match_di4 : 1   ; /* [4]  */
        unsigned int    force_err_l_bndry_match_di5 : 1   ; /* [5]  */
        unsigned int    force_err_l_bndry_match_di6 : 1   ; /* [6]  */
        unsigned int    force_err_l_bndry_match_di7 : 1   ; /* [7]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    force_err_l_seq_di0   : 1   ; /* [16]  */
        unsigned int    force_err_l_seq_di1   : 1   ; /* [17]  */
        unsigned int    force_err_l_seq_di2   : 1   ; /* [18]  */
        unsigned int    force_err_l_seq_di3   : 1   ; /* [19]  */
        unsigned int    force_err_l_seq_di4   : 1   ; /* [20]  */
        unsigned int    force_err_l_seq_di5   : 1   ; /* [21]  */
        unsigned int    force_err_l_seq_di6   : 1   ; /* [22]  */
        unsigned int    force_err_l_seq_di7   : 1   ; /* [23]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_LINE_INTR_FORCE;


/* Define the union U_MIPI_USERDEF_DT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    user_def0_dt          : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    user_def1_dt          : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    user_def2_dt          : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    user_def3_dt          : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_USERDEF_DT;

/* Define the union U_MIPI_USER_DEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    user_def0             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    user_def1             : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    user_def2             : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    user_def3             : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_USER_DEF;

/* Define the union U_MIPI_CTRL_MODE_HS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 3   ; /* [3..1]  */
        unsigned int    reserved_2            : 1   ; /* [4]  */
        unsigned int    reserved_3            : 3   ; /* [7..5]  */
        unsigned int    user_def_en           : 1   ; /* [8]  */
        unsigned int    reserved_4            : 3   ; /* [11..9]  */
        unsigned int    reserved_5            : 2   ; /* [13..12]  */
        unsigned int    reserved_6            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CTRL_MODE_HS;

/* Define the union U_MIPI_DOL_ID_CODE0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    id_code_reg0          : 16  ; /* [15..0]  */
        unsigned int    id_code_reg1          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_DOL_ID_CODE0;

/* Define the union U_MIPI_DOL_ID_CODE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    id_code_reg2          : 16  ; /* [15..0]  */
        unsigned int    id_code_reg3          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_DOL_ID_CODE1;

/* Define the union U_MIPI_DOL_ID_CODE2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    id_code_reg4          : 16  ; /* [15..0]  */
        unsigned int    id_code_reg5          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_DOL_ID_CODE2;

/* Define the union U_MIPI0_CROP_START_CHN0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_start_x_chn0     : 16  ; /* [15..0]  */
        unsigned int    mipi_start_y_chn0     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CROP_START_CHN0;

/* Define the union U_MIPI0_CROP_START_CHN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_start_x_chn1     : 16  ; /* [15..0]  */
        unsigned int    mipi_start_y_chn1     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CROP_START_CHN1;

/* Define the union U_MIPI0_CROP_START_CHN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_start_x_chn2     : 16  ; /* [15..0]  */
        unsigned int    mipi_start_y_chn2     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CROP_START_CHN2;

/* Define the union U_MIPI0_CROP_START_CHN3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_start_x_chn3     : 16  ; /* [15..0]  */
        unsigned int    mipi_start_y_chn3     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CROP_START_CHN3;

/* Define the union U_MIPI0_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_imgwidth         : 16  ; /* [15..0]  */
        unsigned int    mipi_imgheight        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_IMGSIZE;

/* Define the union U_MIPI_CTRL_MODE_PIXEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    crop_en               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    mipi_dol_mode         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    reserved_2            : 1   ; /* [8]  */
        unsigned int    reserved_3            : 2   ; /* [10..9]  */
        unsigned int    reserved_4            : 1   ; /* [11]  */
        unsigned int    stagger_hdr_mode      : 1   ; /* [12]  */
        unsigned int    stagger_frm_num       : 2   ; /* [14..13]  */
        unsigned int    reserved_5            : 1   ; /* [15]  */
        unsigned int    reserved_6            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CTRL_MODE_PIXEL;

/* Define the union U_MIPI_DUMMY_PIX_REG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dummy_pix_reg         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_DUMMY_PIX_REG;

/* Define the union U_MIPI_IMGSIZE_STATIS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    imgwidth_statis_vc   : 16  ; /* [15..0]  */
        unsigned int    imgheight_statis_vc  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_IMGSIZE_STATIS;

/* Define the union U_MIPI_CTRL_INT_RAW */
typedef union
{
    struct
    {
        unsigned int    int_cfifo_wrerr_raw   : 1   ; /* [0]  */
        unsigned int    int_dfifo_wrerr_raw   : 1   ; /* [1]  */
        unsigned int    reserved_0            : 14  ; /* [15..2]  */
        unsigned int    int_cfifo_rderr_raw   : 1   ; /* [16]  */
        unsigned int    int_dfifo_rderr_raw   : 1   ; /* [17]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CTRL_INT_RAW;

/* Define the union U_MIPI_CTRL_INT */
typedef union
{
    /* Define the struct bits */

    struct
    {
        unsigned int    int_cfifo_wrerr_st    : 1   ; /* [0]  */
        unsigned int    int_dfifo_wrerr_st    : 1   ; /* [1]  */
        unsigned int    reserved_0            : 14  ; /* [15..2]  */
        unsigned int    int_cfifo_rderr_st    : 1   ; /* [16]  */
        unsigned int    int_dfifo_rderr_st    : 1   ; /* [17]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CTRL_INT;

/* Define the union U_MIPI_CTRL_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    int_cfifo_wrerr_msk   : 1   ; /* [0]  */
        unsigned int    int_dfifo_wrerr_msk   : 1   ; /* [1]  */
        unsigned int    reserved_0            : 14  ; /* [15..2]  */
        unsigned int    int_cfifo_rderr_msk   : 1   ; /* [16]  */
        unsigned int    int_dfifo_rderr_msk   : 1   ; /* [17]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIPI_CTRL_INT_MSK;


//==============================================================================
/* Define the LVDS Ctrl Regs */
/* Define the union U_LVDS_WDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_wdr_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lvds_wdr_num          : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    lvds_wdr_mode         : 4   ; /* [11..8]  */
        unsigned int    reserved_2            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_WDR;

/* Define the union U_LVDS_DOLSCD_HBLK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dol_hblank1           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    dol_hblank2           : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_DOLSCD_HBLK;

/* Define the union U_LVDS0_CROP_START3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_start_x3_lane    : 16  ; /* [15..0]  */
        unsigned int    lvds_start_y3         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CROP_START3;



/* Define the union U_LVDS_IMGSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_imgwidth_lane    : 16  ; /* [15..0]  */
        unsigned int    lvds_imgheight        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_IMGSIZE;

/* Define the union U_LVDS_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_sync_mode        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    lvds_raw_type         : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    lvds_pix_big_endian   : 1   ; /* [8]  */
        unsigned int    lvds_code_big_endian  : 1   ; /* [9]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    lvds_crop_en          : 1   ; /* [12]  */
        unsigned int    reserved_3            : 3   ; /* [15..13]  */
        unsigned int    lvds_split_mode       : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CTRL;

/* Define the union U_LVDS0_CROP_START0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_start_x0_lane    : 16  ; /* [15..0]  */
        unsigned int    lvds_start_y0         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CROP_START0;


/* Define the union U_LVDS0_CROP_START1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_start_x1_lane    : 16  ; /* [15..0]  */
        unsigned int    lvds_start_y1         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CROP_START1;

/* Define the union U_LVDS0_CROP_START2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_start_x2_lane    : 16  ; /* [15..0]  */
        unsigned int    lvds_start_y2         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CROP_START2;


/* Define the union U_LVDS_CROP_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_start_x_lane     : 16  ; /* [15..0]  */
        unsigned int    lvds_start_y          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CROP_START;

//==============================================================================
/* Define the LVDS SYNC CODE */
/* Define the union U_LVDS_LANE_SOF_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_sof_0           : 16  ; /* [15..0]  */
        unsigned int    lane_sof_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_SOF_01;

/* Define the union U_LVDS_LANE_SOF_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_sof_2           : 16  ; /* [15..0]  */
        unsigned int    lane_sof_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_SOF_23;

/* Define the union U_LVDS_LANE_EOF_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_eof_0           : 16  ; /* [15..0]  */
        unsigned int    lane_eof_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_EOF_01;

/* Define the union U_LVDS_LANE_EOF_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_eof_2           : 16  ; /* [15..0]  */
        unsigned int    lane_eof_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_EOF_23;

/* Define the union U_LVDS_LANE_SOL_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_sol_0           : 16  ; /* [15..0]  */
        unsigned int    lane_sol_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_SOL_01;

/* Define the union U_LVDS_LANE_SOL_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_sol_2           : 16  ; /* [15..0]  */
        unsigned int    lane_sol_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_SOL_23;

/* Define the union U_LVDS_LANE_EOL_01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_eol_0           : 16  ; /* [15..0]  */
        unsigned int    lane_eol_1           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_EOL_01;

/* Define the union U_LVDS_LANE_EOL_23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_eol_2           : 16  ; /* [15..0]  */
        unsigned int    lane_eol_3           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_EOL_23;

/* Define the union U_LVDS_LI_WORD0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    li_word0_0            : 16  ; /* [15..0]  */
        unsigned int    li_word0_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LI_WORD0;

/* Define the union U_LVDS_LI_WORD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    li_word1_0            : 16  ; /* [15..0]  */
        unsigned int    li_word1_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LI_WORD1;

/* Define the union U_LVDS_LI_WORD2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    li_word2_0            : 16  ; /* [15..0]  */
        unsigned int    li_word2_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LI_WORD2;

/* Define the union U_LVDS_LI_WORD3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    li_word3_0            : 16  ; /* [15..0]  */
        unsigned int    li_word3_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LI_WORD3;

/* Define the union U_LVDS_LANE_IMGSIZE_STATIS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane_imgwidth        : 16  ; /* [15..0]  */
        unsigned int    lane_imgheight       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_LANE_IMGSIZE_STATIS;

/* Define the union U_LVDS_IMGSIZE_STATIS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lvds_imgwidth        : 16  ; /* [15..0]  */
        unsigned int    lvds_imgheight       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_IMGSIZE_STATIS;

/* Define the union U_LVDS_CTRL_INT_RAW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    link0_wrerr_raw       : 1   ; /* [0]  */
        unsigned int    link0_vsync_err_raw   : 1   ; /* [1]  */
        unsigned int    link0_hsync_err_raw   : 1   ; /* [2]  */
        unsigned int    link1_wrerr_raw       : 1   ; /* [3]  */
        unsigned int    link1_vsync_err_raw   : 1   ; /* [4]  */
        unsigned int    link1_hsync_err_raw   : 1   ; /* [5]  */
        unsigned int    link2_wrerr_raw       : 1   ; /* [6]  */
        unsigned int    link2_vsync_err_raw   : 1   ; /* [7]  */
        unsigned int    link2_hsync_err_raw   : 1   ; /* [8]  */
        unsigned int    lane0_sync_err_raw    : 1   ; /* [9]  */
        unsigned int    lane1_sync_err_raw    : 1   ; /* [10]  */
        unsigned int    lane2_sync_err_raw    : 1   ; /* [11]  */
        unsigned int    lane3_sync_err_raw    : 1   ; /* [12]  */
        unsigned int    lane4_sync_err_raw    : 1   ; /* [13]  */
        unsigned int    lane5_sync_err_raw    : 1   ; /* [14]  */
        unsigned int    lane6_sync_err_raw    : 1   ; /* [15]  */
        unsigned int    lane7_sync_err_raw    : 1   ; /* [16]  */
        unsigned int    lane8_sync_err_raw    : 1   ; /* [17]  */
        unsigned int    lane9_sync_err_raw    : 1   ; /* [18]  */
        unsigned int    lane10_sync_err_raw   : 1   ; /* [19]  */
        unsigned int    lane11_sync_err_raw   : 1   ; /* [20]  */
        unsigned int    lvds_stat_err_raw     : 1   ; /* [21]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    link0_rderr_raw       : 1   ; /* [24]  */
        unsigned int    link1_rderr_raw       : 1   ; /* [25]  */
        unsigned int    link2_rderr_raw       : 1   ; /* [26]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CTRL_INT_RAW;

/* Define the union U_LVDS_CTRL_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    link0_wrerr_st        : 1   ; /* [0]  */
        unsigned int    link0_vsync_err_st    : 1   ; /* [1]  */
        unsigned int    link0_hsync_err_st    : 1   ; /* [2]  */
        unsigned int    link1_wrerr_st        : 1   ; /* [3]  */
        unsigned int    link1_vsync_err_st    : 1   ; /* [4]  */
        unsigned int    link1_hsync_err_st    : 1   ; /* [5]  */
        unsigned int    link2_wrerr_st        : 1   ; /* [6]  */
        unsigned int    link2_vsync_err_st    : 1   ; /* [7]  */
        unsigned int    link2_hsync_err_st    : 1   ; /* [8]  */
        unsigned int    lane0_sync_err_st     : 1   ; /* [9]  */
        unsigned int    lane1_sync_err_st     : 1   ; /* [10]  */
        unsigned int    lane2_sync_err_st     : 1   ; /* [11]  */
        unsigned int    lane3_sync_err_st     : 1   ; /* [12]  */
        unsigned int    lane4_sync_err_st     : 1   ; /* [13]  */
        unsigned int    lane5_sync_err_st     : 1   ; /* [14]  */
        unsigned int    lane6_sync_err_st     : 1   ; /* [15]  */
        unsigned int    lane7_sync_err_st     : 1   ; /* [16]  */
        unsigned int    lane8_sync_err_st     : 1   ; /* [17]  */
        unsigned int    lane9_sync_err_st     : 1   ; /* [18]  */
        unsigned int    lane10_sync_err_st    : 1   ; /* [19]  */
        unsigned int    lane11_sync_err_st    : 1   ; /* [20]  */
        unsigned int    lvds_stat_err_st      : 1   ; /* [21]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    link0_rderr_st        : 1   ; /* [24]  */
        unsigned int    link1_rderr_st        : 1   ; /* [25]  */
        unsigned int    link2_rderr_st        : 1   ; /* [26]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CTRL_INT;

/* Define the union U_LVDS_CTRL_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    link0_wrerr_msk       : 1   ; /* [0]  */
        unsigned int    link0_vsync_err_msk   : 1   ; /* [1]  */
        unsigned int    link0_hsync_err_msk   : 1   ; /* [2]  */
        unsigned int    link1_wrerr_msk       : 1   ; /* [3]  */
        unsigned int    link1_vsync_err_msk   : 1   ; /* [4]  */
        unsigned int    link1_hsync_err_msk   : 1   ; /* [5]  */
        unsigned int    link2_wrerr_msk       : 1   ; /* [6]  */
        unsigned int    link2_vsync_err_msk   : 1   ; /* [7]  */
        unsigned int    link2_hsync_err_msk   : 1   ; /* [8]  */
        unsigned int    lane0_sync_err_msk    : 1   ; /* [9]  */
        unsigned int    lane1_sync_err_msk    : 1   ; /* [10]  */
        unsigned int    lane2_sync_err_msk    : 1   ; /* [11]  */
        unsigned int    lane3_sync_err_msk    : 1   ; /* [12]  */
        unsigned int    lane4_sync_err_msk    : 1   ; /* [13]  */
        unsigned int    lane5_sync_err_msk    : 1   ; /* [14]  */
        unsigned int    lane6_sync_err_msk    : 1   ; /* [15]  */
        unsigned int    lane7_sync_err_msk    : 1   ; /* [16]  */
        unsigned int    lane8_sync_err_msk    : 1   ; /* [17]  */
        unsigned int    lane9_sync_err_msk    : 1   ; /* [18]  */
        unsigned int    lane10_sync_err_msk   : 1   ; /* [19]  */
        unsigned int    lane11_sync_err_msk   : 1   ; /* [20]  */
        unsigned int    lvds_stat_err_st_msk  : 1   ; /* [21]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    link0_rderr_msk       : 1   ; /* [24]  */
        unsigned int    link1_rderr_msk       : 1   ; /* [25]  */
        unsigned int    link2_rderr_msk       : 1   ; /* [26]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LVDS_CTRL_INT_MSK;

/* Define the union U_LANE_ID_CHN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lane0_id              : 4   ; /* [3..0]  */
        unsigned int    lane1_id              : 4   ; /* [7..4]  */
        unsigned int    lane2_id              : 4   ; /* [11..8]  */
        unsigned int    lane3_id              : 4   ; /* [15..12]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LANE_ID_CHN;


//==============================================================================
/* Define the DATA ALIGN Regs */
/* Define the union U_ALIGN_INT_RAW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    err_full_raw          : 1   ; /* [0]  */
        unsigned int    err_lane0_raw         : 1   ; /* [1]  */
        unsigned int    err_lane1_raw         : 1   ; /* [2]  */
        unsigned int    err_lane2_raw         : 1   ; /* [3]  */
        unsigned int    err_lane3_raw         : 1   ; /* [4]  */
        unsigned int    err_lane4_raw         : 1   ; /* [5]  */
        unsigned int    err_lane5_raw         : 1   ; /* [6]  */
        unsigned int    err_lane6_raw         : 1   ; /* [7]  */
        unsigned int    err_lane7_raw         : 1   ; /* [8]  */
        unsigned int    err_lane8_raw         : 1   ; /* [9]  */
        unsigned int    err_lane9_raw         : 1   ; /* [10]  */
        unsigned int    err_lane10_raw        : 1   ; /* [11]  */
        unsigned int    err_lane11_raw        : 1   ; /* [12]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ALIGN_INT_RAW;

/* Define the union U_ALIGN_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    err_full_st           : 1   ; /* [0]  */
        unsigned int    err_lane0_st          : 1   ; /* [1]  */
        unsigned int    err_lane1_st          : 1   ; /* [2]  */
        unsigned int    err_lane2_st          : 1   ; /* [3]  */
        unsigned int    err_lane3_st          : 1   ; /* [4]  */
        unsigned int    err_lane4_st          : 1   ; /* [5]  */
        unsigned int    err_lane5_st          : 1   ; /* [6]  */
        unsigned int    err_lane6_st          : 1   ; /* [7]  */
        unsigned int    err_lane7_st          : 1   ; /* [8]  */
        unsigned int    err_lane8_st          : 1   ; /* [9]  */
        unsigned int    err_lane9_st          : 1   ; /* [10]  */
        unsigned int    err_lane10_st         : 1   ; /* [11]  */
        unsigned int    err_lane11_st         : 1   ; /* [12]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ALIGN_INT;

/* Define the union U_ALIGN_INT_MSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    err_full_mask         : 1   ; /* [0]  */
        unsigned int    err_lane0_mask        : 1   ; /* [1]  */
        unsigned int    err_lane1_mask        : 1   ; /* [2]  */
        unsigned int    err_lane2_mask        : 1   ; /* [3]  */
        unsigned int    err_lane3_mask        : 1   ; /* [4]  */
        unsigned int    err_lane4_mask        : 1   ; /* [5]  */
        unsigned int    err_lane5_mask        : 1   ; /* [6]  */
        unsigned int    err_lane6_mask        : 1   ; /* [7]  */
        unsigned int    err_lane7_mask        : 1   ; /* [8]  */
        unsigned int    err_lane8_mask        : 1   ; /* [9]  */
        unsigned int    err_lane9_mask        : 1   ; /* [10]  */
        unsigned int    err_lane10_mask       : 1   ; /* [11]  */
        unsigned int    err_lane11_mask       : 1   ; /* [12]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ALIGN_INT_MSK;

//==============================================================================
/* Define the struct */

typedef struct
{
    volatile U_PHY_MODE_LINK           PHY_MODE_LINK           ; /* 0x0 */
    volatile U_PHY_SKEW_LINK           PHY_SKEW_LINK           ; /* 0x4 */
    volatile U_PHY_EN_LINK             PHY_EN_LINK             ; /* 0x8 */
    volatile U_PHY_CFG_LINK        	   PHY_CFG_LINK            ; /* 0xc */
    volatile unsigned int              reserved_0[2]          ; /* 0x10~0x14 */
    volatile U_PHY_DATA_LINK           PHY_DATA_LINK           ; /* 0x18 */
    volatile U_PHY_PH_MIPI_LINK        PHY_PH_MIPI_LINK        ; /* 0x1c */
    volatile U_PHY_DATA_MIPI_LINK      PHY_DATA_MIPI_LINK      ; /* 0x20 */
    volatile U_PHY_SYNC_DCT_LINK       PHY_SYNC_DCT_LINK       ; /* 0x24 */
    volatile unsigned int              reserved_1[2]           ; /* 0x28~0x2c */

	volatile U_PHY_SYNC_CODE_LINK      PHY_SYNC_CODE_LINK[4]   ; /* 0x30~0x3c */
    volatile U_PHY_OUT_VALID_LINK      PHY_OUT_VALID_LINK      ; /* 0x40 */
    volatile U_PHY_DATA_LVDS_LINK      PHY_DATA_LVDS_LINK      ; /* 0x44 */
	
    volatile unsigned int              reserved_2[106]         ; /* 0x48~0x1ec */
    volatile U_MIPI_CIL_INT_RAW_LINK   MIPI_CIL_INT_RAW_LINK   ; /* 0x1f0 */
    volatile U_MIPI_CIL_INT_LINK       MIPI_CIL_INT_LINK       ; /* 0x1f4 */
    volatile U_MIPI_CIL_INT_MSK_LINK   MIPI_CIL_INT_MSK_LINK   ; /* 0x1f8 */
    volatile unsigned int              reserved_3              ; /* 0x1fc */
}MIPI_LINK_REGS_S;

typedef struct
{

#if 0
    volatile U_PHY_CHN_CTRL         PHY_CHN_CTRL             ; /* 0x800 */
    volatile U_PHY_LP_SELECT        PHY_LP_SELECT            ; /* 0x804 */
    volatile unsigned int           reserved_12              ; /* 0x808 */
    volatile U_MIPI_MEM_CTRL        MIPI_MEM_CTRL            ; /* 0x80c */
    volatile U_LANE_EN              LANE_EN                  ; /* 0x810 */
    volatile U_MIPI_PHYCFG_MODE     MIPI_PHYCFG_MODE         ; /* 0x814 */
    volatile U_MIPI_PHYCFG_EN       MIPI_PHYCFG_EN           ; /* 0x818 */
    volatile U_MIPI_CIL_CTRL        MIPI_CIL_CTRL            ; /* 0x81c */
#endif

	volatile U_PHY_ID               PHY_ID;						/* 0x800 */
    volatile U_PHY_LP_SELECT        PHY_LP_SELECT;				/* 0x804 */
	volatile unsigned int           reserved_12[2];  
    volatile U_CHN0_PHY_EN          CHN0_PHY_EN;				/* 0x810 */
    volatile U_CHN0_HS_CK_SELECT    CHN0_HS_CK_SELECT;			/* 0x814 */
    volatile U_CHN0_MEM_CTRL        CHN0_MEM_CTRL;				/* 0x818 */
    volatile U_CHN0_LANE_EN         CHN0_LANE_EN;				/* 0x81c */
    volatile U_CHN0_PHYCFG_MODE     CHN0_PHYCFG_MODE;			/* 0x820 */
    volatile U_CHN0_PHYCFG_EN       CHN0_PHYCFG_EN;				/* 0x824 */
    volatile U_CHN0_CLR_EN          CHN0_CLR_EN;				/* 0x828 */
    volatile U_CHN0_CIL_CTRL        CHN0_CIL_CTRL;				/* 0x82c */
    volatile U_CHN0_LANE_EN         CHN1_PHY_EN;				/* 0x830 */
    volatile U_CHN1_HS_CK_SELECT    CHN1_HS_CK_SELECT;			/* 0x834 */
    volatile U_CHN1_MEM_CTRL        CHN1_MEM_CTRL;				/* 0x838 */
    volatile U_CHN1_LANE_EN         CHN1_LANE_EN;				/* 0x83c */
    volatile U_CHN1_PHYCFG_MODE     CHN1_PHYCFG_MODE;			/* 0x840 */
    volatile U_CHN1_PHYCFG_EN       CHN1_PHYCFG_EN;				/* 0x844 */
    volatile U_CHN1_CLR_EN          CHN1_CLR_EN;				/* 0x848 */
    volatile U_CHN1_CIL_CTRL        CHN1_CIL_CTRL;				/* 0x84c */
}MIPI_SYS_REGS_S;

typedef struct
{
    volatile U_MIPI_LANES_NUM       MIPI_LANES_NUM           ; /* 0x1004 */
    volatile unsigned int           reserved_15              ; /* 0x1008 */
    volatile U_MIPI_MAIN_INT_ST     MIPI_MAIN_INT_ST         ; /* 0x100c */
    volatile U_MIPI_DI_1            MIPI_DI_1                ; /* 0x1010 */
    volatile U_MIPI_DI_2            MIPI_DI_2                ; /* 0x1014 */
    volatile unsigned int           reserved_16[18]          ; /* 0x1018~0x105c */
    volatile U_MIPI_PKT_INTR_ST     MIPI_PKT_INTR_ST         ; /* 0x1060 */
    volatile U_MIPI_PKT_INTR_MSK    MIPI_PKT_INTR_MSK        ; /* 0x1064 */
    volatile U_MIPI_PKT_INTR_FORCE  MIPI_PKT_INTR_FORCE      ; /* 0x1068 */
    volatile unsigned int           reserved_17              ; /* 0x106c */
    volatile U_MIPI_PKT_INTR2_ST    MIPI_PKT_INTR2_ST        ; /* 0x1070 */
    volatile U_MIPI_PKT_INTR2_MSK   MIPI_PKT_INTR2_MSK       ; /* 0x1074 */
    volatile U_MIPI_PKT_INTR2_FORCE MIPI_PKT_INTR2_FORCE     ; /* 0x1078 */
    volatile unsigned int           reserved_18              ; /* 0x107c */
    volatile U_MIPI_FRAME_INTR_ST   MIPI_FRAME_INTR_ST       ; /* 0x1080 */
    volatile U_MIPI_FRAME_INTR_MSK  MIPI_FRAME_INTR_MSK      ; /* 0x1084 */
    volatile U_MIPI_FRAME_INTR_FORCE  MIPI0_FRAME_INTR_FORCE ; /* 0x1088 */
    volatile unsigned int           reserved_19              ; /* 0x108c */
    volatile U_MIPI_LINE_INTR_ST    MIPI_LINE_INTR_ST        ; /* 0x1090 */
    volatile U_MIPI_LINE_INTR_MSK   MIPI_LINE_INTR_MSK       ; /* 0x1094 */
    volatile U_MIPI_LINE_INTR_FORCE MIPI_LINE_INTR_FORCE     ; /* 0x1098 */
    volatile unsigned int           reserved_20[25]          ; /* 0x109c~0x10fc */
    volatile U_MIPI_USERDEF_DT      MIPI_USERDEF_DT          ; /* 0x1100 */
    volatile U_MIPI_USER_DEF        MIPI_USER_DEF            ; /* 0x1104 */
    volatile U_MIPI_CTRL_MODE_HS    MIPI_CTRL_MODE_HS        ; /* 0x1108 */
    volatile unsigned int           reserved_21[61]          ; /* 0x110c~0x11fc */
    volatile U_MIPI_DOL_ID_CODE0    MIPI_DOL_ID_CODE0        ; /* 0x1200 */
    volatile U_MIPI_DOL_ID_CODE1    MIPI_DOL_ID_CODE1        ; /* 0x1204 */
    volatile U_MIPI_DOL_ID_CODE2    MIPI_DOL_ID_CODE2        ; /* 0x1208 */
    volatile unsigned int           reserved_211             ; /* 0x120c */
    volatile U_MIPI_CROP_START_CHN0 MIPI_CROP_START_CHN0     ; /* 0x1210 */
    volatile U_MIPI_CROP_START_CHN1 MIPI_CROP_START_CHN1     ; /* 0x1214 */
    volatile U_MIPI_CROP_START_CHN2 MIPI_CROP_START_CHN2     ; /* 0x1218 */
    volatile U_MIPI_CROP_START_CHN3 MIPI_CROP_START_CHN3     ; /* 0x121c */
    volatile unsigned int           reserved_212             ; /* 0x1220 */
    volatile U_MIPI_IMGSIZE         MIPI_IMGSIZE             ; /* 0x1224 */
    volatile unsigned int           reserved_22[2]           ; /* 0x1228~0x122c */
    volatile U_MIPI_CTRL_MODE_PIXEL MIPI_CTRL_MODE_PIXEL     ; /* 0x1230 */
    volatile unsigned int           reserved_23[3]           ; /* 0x1234~0x123c */

    volatile U_MIPI_DUMMY_PIX_REG   MIPI_DUMMY_PIX_REG     ; /* 0x1240 */
    volatile unsigned int           reserved_24[3]           ; /* 0x1244~0x124c */

    volatile U_MIPI_IMGSIZE_STATIS  MIPI_IMGSIZE_STATIS[4]   ; /* 0x1250~0x125c */

    volatile unsigned int           reserved_25[36]          ; /* 0x1260~0x12ec */
    volatile U_MIPI_CTRL_INT_RAW    MIPI_CTRL_INT_RAW        ; /* 0x12f0 */
    volatile U_MIPI_CTRL_INT        MIPI_CTRL_INT            ; /* 0x12f4 */
    volatile U_MIPI_CTRL_INT_MSK    MIPI_CTRL_INT_MSK        ; /* 0x12f8 */
    volatile unsigned int           reserved_26              ; /* 0x12fc */
}MIPI_CTRL_REGS_S;

typedef struct
{
    volatile U_LVDS_LANE_SOF_01   LANE_SOF_01       ; /* 0x1320 */
    volatile U_LVDS_LANE_SOF_23   LANE_SOF_23       ; /* 0x1324 */
    volatile U_LVDS_LANE_EOF_01   LANE_EOF_01       ; /* 0x1328 */
    volatile U_LVDS_LANE_EOF_23   LANE_EOF_23       ; /* 0x132c */
    volatile U_LVDS_LANE_SOL_01   LANE_SOL_01       ; /* 0x1330 */
    volatile U_LVDS_LANE_SOL_23   LANE_SOL_23       ; /* 0x1334 */
    volatile U_LVDS_LANE_EOL_01   LANE_EOL_01       ; /* 0x1338 */
    volatile U_LVDS_LANE_EOL_23   LANE_EOL_23       ; /* 0x133c */
}LVDS_SYNC_CODE_S;

typedef struct
{
    volatile U_LVDS_WDR               LVDS_WDR                ; /* 0x1300 */
    volatile U_LVDS_DOLSCD_HBLK       LVDS_DOLSCD_HBLK        ; /* 0x1304 */
    volatile U_LVDS_CROP_START3    	  LVDS_CROP_START3          ; /* 0x1308 */
    volatile U_LVDS_IMGSIZE           LVDS_IMGSIZE            ; /* 0x130c */
    volatile U_LVDS_CTRL              LVDS_CTRL               ; /* 0x1310 */
    volatile U_LVDS_CROP_START0       LVDS_CROP_START0         ; /* 0x1314 */
    volatile U_LVDS_CROP_START1       LVDS_CROP_START1         ; /* 0x1318 */
    volatile U_LVDS_CROP_START2       LVDS_CROP_START2         ; /* 0x131c */

	LVDS_SYNC_CODE_S                  N_SYNC_CODE[12]         ; /* 0x1320~0x149c*/
	LVDS_SYNC_CODE_S                  NXT_SYNC_CODE[12]       ; /* 0x14a0~0x161c*/

    volatile U_LVDS_LI_WORD0          LVDS_LI_WORD0           ; /* 0x1620 */
    volatile U_LVDS_LI_WORD1          LVDS_LI_WORD1           ; /* 0x1624 */
    volatile U_LVDS_LI_WORD2          LVDS_LI_WORD2           ; /* 0x1628 */
    volatile U_LVDS_LI_WORD3          LVDS_LI_WORD3           ; /* 0x162c */

    volatile unsigned int             reserved_29[4];        ; /* 0x1630~0x163C*/
    volatile U_LVDS_LANE_IMGSIZE_STATIS LVDS_LANE_IMGSIZE_STATIS[12];  /* 0x1640~0x166C */
    volatile unsigned int             reserved_291[4]          ; /* 0x1670~0x167c */

    volatile U_LVDS_IMGSIZE_STATIS    LVDS_IMGSIZE_STATIS[4]  ; /* 0x1680~0x168c */

    volatile unsigned int             reserved_30[24]         ; /* 0x1690~0x16ec */
    volatile U_LVDS_CTRL_INT_RAW      LVDS_CTRL_INT_RAW       ; /* 0x16f0 */
    volatile U_LVDS_CTRL_INT          LVDS_CTRL_INT           ; /* 0x16f4 */
    volatile U_LVDS_CTRL_INT_MSK      LVDS_CTRL_INT_MSK       ; /* 0x16f8 */
    volatile unsigned int             reserved_31             ; /* 0x16fc */

}LVDS_CTRL_REGS_S;

typedef struct
{
    MIPI_CTRL_REGS_S               stMIPI_CtrlRegs         ; /* 0x1004 ~ 0x12fc */
	LVDS_CTRL_REGS_S               stLVDS_CtrlRegs         ; /* 0x1300 ~ 0x16fc */

	volatile U_LANE_ID_CHN         LANE_ID_CHN[3];         ; /* 0x1700~0x1708 */
    volatile unsigned int          reserved_32[57]         ; /* 0x170c~0x17ec */

    volatile U_ALIGN_INT_RAW       ALIGN_INT_RAW           ; /* 0x17f0 */
    volatile U_ALIGN_INT           ALIGN_INT               ; /* 0x17f4 */
    volatile U_ALIGN_INT_MSK       ALIGN_INT_MSK           ; /* 0x17f8 */
    volatile unsigned int          reserved_33[514]        ; /* 0x17fc~0x2000 */
}COMBO_DEV_REGS_S;


//==============================================================================
/* Define the global struct */

typedef struct
{
    MIPI_LINK_REGS_S                stLinkRegs[3]            ; /* 0x000 ~ 0x5fc */
    volatile unsigned int           reserved_11[128]         ; /* 0x600 ~ 0x7fc */

    MIPI_SYS_REGS_S                 stSysRegs                ; /* 0x800 ~ 0x84c */
  //volatile U_MIPI_SRST_CFG     MIPI_SRST_CFG         ; /* 0x820 */
    volatile unsigned int           reserved_13[424]         ; /* 0x850 ~ 0xeec */

    /* MIPI0 and MIPI1 Interrupt Regs */
    volatile U_MIPI_CHN0_INT_RAW    MIPI_CHN0_INT_RAW        ; /* 0xef0 */
    volatile U_MIPI_CHN0_INT        MIPI_CHN0_INT            ; /* 0xef4 */
    volatile U_MIPI_CHN0_INT_MSK    MIPI_CHN0_INT_MSK        ; /* 0xef8 */
    volatile unsigned int           reserved_14[61]          ; /* 0xefc~0xfec */
    volatile U_MIPI_CHN1_INT_RAW    MIPI_CHN1_INT_RAW        ; /* 0xff0 */
    volatile U_MIPI_CHN1_INT        MIPI_CHN1_INT            ; /* 0xff4 */
    volatile U_MIPI_CHN1_INT_MSK    MIPI_CHN1_INT_MSK        ; /* 0xff8 */
    volatile unsigned int           reserved_15[2]           ; /* 0xffc~0x1000 */

    COMBO_DEV_REGS_S                stComboDevRegs[COMBO_MAX_DEV_NUM]        ; /* 0x1004 ~ 0x3000*/
}MIPI_REGS_TYPE_S;


#endif

