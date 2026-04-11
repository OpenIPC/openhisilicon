//****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co., Ltd.
// File name     :  isp_reg_define.h
// Author        :  q00214668
// Version       :  1.0 
// Date          :  2015-12-22
// Description   :  Define all registers/tables for Hi3519
// Others        :  Generated automatically by nManager V4.0 
// History       :  q00214668 2015-12-22 Create file
//******************************************************************************

#ifndef __ISP_REG_DEFINE_H__
#define __ISP_REG_DEFINE_H__

/* Define the union U_ISP_RESERVED0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_RESERVED0;
/* Define the union U_ISP_FE_VERSION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    customer              : 8   ; /* [7..0]  */
        unsigned int    release               : 8   ; /* [15..8]  */
        unsigned int    build                 : 8   ; /* [23..16]  */
        unsigned int    version               : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_VERSION;

/* Define the union U_ISP_FE_DATE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int date                   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_DATE;
/* Define the union U_ISP_FE_MODULE_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ae_sel                : 2   ; /* [1..0]  */
        unsigned int    awb_sel               : 2   ; /* [3..2]  */
        unsigned int    af_sel                : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    dis_sel               : 1   ; /* [6]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    reserved_2            : 8   ; /* [15..8]  */
        unsigned int    dcg_sel               : 1   ; /* [16]  */
        unsigned int    reserved_3            : 1   ; /* [17]  */
        unsigned int    nr_sel                : 1   ; /* [18]  */
        unsigned int    reserved_4            : 1   ; /* [19]  */
        unsigned int    reserved_5            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_MODULE_POS;

/* Define the union U_ISP_FE_FSTART_DELAY */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int delay                  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_FSTART_DELAY;
/* Define the union U_ISP_FE_USER_DEFINE0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int user_define0           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_USER_DEFINE0;
/* Define the union U_ISP_FE_USER_DEFINE1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int user_define1           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_USER_DEFINE1;
/* Define the union U_ISP_FE_STARTUP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    startup               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_STARTUP;

/* Define the union U_ISP_FE_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fstart                : 1   ; /* [0]  */
        unsigned int    update_cfg            : 1   ; /* [1]  */
        unsigned int    cfg_loss              : 1   ; /* [2]  */
        unsigned int    fstart_delay          : 1   ; /* [3]  */
        unsigned int    ae_int                : 1   ; /* [4]  */
        unsigned int    awb_int               : 1   ; /* [5]  */
        unsigned int    mg_int                : 1   ; /* [6]  */
        unsigned int    dis_int               : 1   ; /* [7]  */
        unsigned int    aewdr_int             : 1   ; /* [8]  */
        unsigned int    flick_int             : 1   ; /* [9]  */
        unsigned int    reserved_0            : 7   ; /* [16..10]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_INT;

/* Define the union U_ISP_FE_INT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fstart                : 1   ; /* [0]  */
        unsigned int    update_cfg            : 1   ; /* [1]  */
        unsigned int    cfg_loss              : 1   ; /* [2]  */
        unsigned int    int_delay             : 1   ; /* [3]  */
        unsigned int    ae_int                : 1   ; /* [4]  */
        unsigned int    awb_int               : 1   ; /* [5]  */
        unsigned int    mg_int                : 1   ; /* [6]  */
        unsigned int    dis_int               : 1   ; /* [7]  */
        unsigned int    aewdr_int             : 1   ; /* [8]  */
        unsigned int    flick_int             : 1   ; /* [9]  */
        unsigned int    reserved_0            : 7   ; /* [16..10]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_INT_MASK;

/* Define the union U_ISP_FE_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_CTRL_F;

/* Define the union U_ISP_FE_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_CTRL_I;

/* Define the union U_ISP_FE_TIMING_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    fix_timing            : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_TIMING_CFG;

/* Define the union U_ISP_FE_REG_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_REG_UPDATE;

/* Define the union U_ISP_FE_RESERVED1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_RESERVED1;
/* Define the union U_ISP_CROP0_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    n0_en                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP0_CFG;

/* Define the union U_ISP_CROP0_RESERVED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_CROP0_RESERVED;
/* Define the union U_ISP_CROP0_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x_start               : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    y_start               : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP0_START;

/* Define the union U_ISP_CROP0_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP0_SIZE;

/* Define the union U_ISP_DRC_DITHER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    isp_dither_en         : 1   ; /* [0]  */
        unsigned int    isp_dither_round      : 2   ; /* [2..1]  */
        unsigned int    spatial_mode          : 1   ; /* [3]  */
        unsigned int    out_bits              : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DITHER;

/* Define the union U_ISP_INPUT_MUX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    input0_sel            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    input1_sel            : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    input2_sel            : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    input3_sel            : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    input4_sel            : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_INPUT_MUX;

/* Define the union U_ISP_FE_BLACK_SUM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_BLACK_SUM_CFG;

/* Define the union U_ISP_FE_BAYER_SUM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_BAYER_SUM_CFG;

/* Define the union U_ISP_FE_WDR_SUM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_WDR_SUM_CFG;

/* Define the union U_ISP_FE_BLACK_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_BLACK_SUM0;
/* Define the union U_ISP_FE_BLACK_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_BLACK_SUM1;
/* Define the union U_ISP_FE_BAYER_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_BAYER_SUM0;
/* Define the union U_ISP_FE_BAYER_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_BAYER_SUM1;
/* Define the union U_ISP_FE_WDR0_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR0_SUM0;
/* Define the union U_ISP_FE_WDR0_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR0_SUM1;
/* Define the union U_ISP_FE_WDR1_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR1_SUM0;
/* Define the union U_ISP_FE_WDR1_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR1_SUM1;
/* Define the union U_ISP_FE_WDR2_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR2_SUM0;
/* Define the union U_ISP_FE_WDR2_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR2_SUM1;
/* Define the union U_ISP_FE_WDR3_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR3_SUM0;
/* Define the union U_ISP_FE_WDR3_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FE_WDR3_SUM1;
/* Define the union U_ISP_COLORBAR_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_CFG;

/* Define the union U_ISP_COLORBAR_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_COLORBAR_VERSION;
/* Define the union U_ISP_COLORBAR_PATTERN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pattern               : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_PATTERN;

/* Define the union U_ISP_COLORBAR_RBACKGND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rbackgnd              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_RBACKGND;

/* Define the union U_ISP_COLORBAR_GBACKGND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbackgnd              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_GBACKGND;

/* Define the union U_ISP_COLORBAR_BBACKGND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bbackgnd              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_BBACKGND;

/* Define the union U_ISP_COLORBAR_RFOREGND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rforegnd              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_RFOREGND;

/* Define the union U_ISP_COLORBAR_GFOREGND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gforegnd              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_GFOREGND;

/* Define the union U_ISP_COLORBAR_BFOREGND */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bforegnd              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_BFOREGND;

/* Define the union U_ISP_COLORBAR_INIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgbinit               : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_INIT;

/* Define the union U_ISP_COLORBAR_GRAD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgbgrad               : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_GRAD;

/* Define the union U_ISP_COLORBAR_POS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pos_x1                : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    pos_y1                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_POS1;

/* Define the union U_ISP_COLORBAR_POS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pos_x2                : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    pos_y2                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_POS2;

/* Define the union U_ISP_COLORBAR_RGGB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb                  : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_RGGB;

/* Define the union U_ISP_COLORBAR_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_CTRL_I;

/* Define the union U_ISP_COLORBAR_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_UPDATE;

/* Define the union U_ISP_COLORBAR_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_SIZE;

/* Define the union U_ISP_BLC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_CFG;

/* Define the union U_ISP_BLC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BLC_VERSION;
/* Define the union U_ISP_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_OFFSET1;

/* Define the union U_ISP_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_OFFSET2;

/* Define the union U_ISP_BLC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_CTRL_F;

/* Define the union U_ISP_BLC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_CTRL_I;

/* Define the union U_ISP_BLC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_UPDATE;

/* Define the union U_ISP_4BLC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC_CFG;

/* Define the union U_ISP_4BLC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_4BLC_VERSION;
/* Define the union U_ISP_4BLC0_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC0_OFFSET1;

/* Define the union U_ISP_4BLC0_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC0_OFFSET2;

/* Define the union U_ISP_4BLC1_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC1_OFFSET1;

/* Define the union U_ISP_4BLC1_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC1_OFFSET2;

/* Define the union U_ISP_4BLC2_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC2_OFFSET1;

/* Define the union U_ISP_4BLC2_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC2_OFFSET2;

/* Define the union U_ISP_4BLC3_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC3_OFFSET1;

/* Define the union U_ISP_4BLC3_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC3_OFFSET2;

/* Define the union U_ISP_4BLC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC_CTRL_F;

/* Define the union U_ISP_4BLC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC_CTRL_I;

/* Define the union U_ISP_4BLC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4BLC_UPDATE;

/* Define the union U_ISP_WB_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_CFG;

/* Define the union U_ISP_WB_BLC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en_in                 : 1   ; /* [0]  */
        unsigned int    en_out                : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_BLC_CFG;

/* Define the union U_ISP_WB_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_WB_VERSION;
/* Define the union U_ISP_WB_GAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    rgain                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_GAIN1;

/* Define the union U_ISP_WB_GAIN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbgain                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    bgain                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_GAIN2;

/* Define the union U_ISP_WB_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_BLC_OFFSET1;

/* Define the union U_ISP_WB_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_BLC_OFFSET2;

/* Define the union U_ISP_WB_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_CTRL_F;

/* Define the union U_ISP_WB_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_CTRL_I;

/* Define the union U_ISP_WB_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_UPDATE;

/* Define the union U_ISP_DG_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_CFG;

/* Define the union U_ISP_DG_BLC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en_in                 : 1   ; /* [0]  */
        unsigned int    en_out                : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_BLC_CFG;

/* Define the union U_ISP_DG_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DG_VERSION;
/* Define the union U_ISP_DG_GAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain                : 16  ; /* [15..0]  */
        unsigned int    rgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_GAIN1;

/* Define the union U_ISP_DG_GAIN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbgain                : 16  ; /* [15..0]  */
        unsigned int    bgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_GAIN2;

/* Define the union U_ISP_DG_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_BLC_OFFSET1;

/* Define the union U_ISP_DG_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_BLC_OFFSET2;

/* Define the union U_ISP_DG_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_CTRL_F;

/* Define the union U_ISP_DG_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_CTRL_I;

/* Define the union U_ISP_DG_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_UPDATE;

/* Define the union U_ISP_4DG_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_CFG;

/* Define the union U_ISP_4DG_BLC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en_in                 : 1   ; /* [0]  */
        unsigned int    en_out                : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_BLC_CFG;

/* Define the union U_ISP_4DG_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_4DG_VERSION;
/* Define the union U_ISP_4DG_0_GAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain                : 16  ; /* [15..0]  */
        unsigned int    rgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_GAIN1;

/* Define the union U_ISP_4DG_0_GAIN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbgain                : 16  ; /* [15..0]  */
        unsigned int    bgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_GAIN2;

/* Define the union U_ISP_4DG_0_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_BLC_OFFSET1;

/* Define the union U_ISP_4DG_0_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_BLC_OFFSET2;

/* Define the union U_ISP_4DG_1_GAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain                : 16  ; /* [15..0]  */
        unsigned int    rgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_GAIN1;

/* Define the union U_ISP_4DG_1_GAIN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbgain                : 16  ; /* [15..0]  */
        unsigned int    bgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_GAIN2;

/* Define the union U_ISP_4DG_1_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_BLC_OFFSET1;

/* Define the union U_ISP_4DG_1_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_BLC_OFFSET2;

/* Define the union U_ISP_4DG_2_GAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain                : 16  ; /* [15..0]  */
        unsigned int    rgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_GAIN1;

/* Define the union U_ISP_4DG_2_GAIN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbgain                : 16  ; /* [15..0]  */
        unsigned int    bgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_GAIN2;

/* Define the union U_ISP_4DG_2_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_BLC_OFFSET1;

/* Define the union U_ISP_4DG_2_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_BLC_OFFSET2;

/* Define the union U_ISP_4DG_3_GAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain                : 16  ; /* [15..0]  */
        unsigned int    rgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_GAIN1;

/* Define the union U_ISP_4DG_3_GAIN2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbgain                : 16  ; /* [15..0]  */
        unsigned int    bgain                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_GAIN2;

/* Define the union U_ISP_4DG_3_BLC_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgr                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsr                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_BLC_OFFSET1;

/* Define the union U_ISP_4DG_3_BLC_OFFSET2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ofsgb                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    ofsb                  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_BLC_OFFSET2;

/* Define the union U_ISP_4DG_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_CTRL_F;

/* Define the union U_ISP_4DG_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_CTRL_I;

/* Define the union U_ISP_4DG_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_UPDATE;

/* Define the union U_ISP_AE_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_CFG;

/* Define the union U_ISP_AE_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_VERSION;
/* Define the union U_ISP_AE_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    vnum                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_ZONE;

/* Define the union U_ISP_AE_SKIP_CRG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skip_x                : 3   ; /* [2..0]  */
        unsigned int    offset_x              : 1   ; /* [3]  */
        unsigned int    skip_y                : 3   ; /* [6..4]  */
        unsigned int    offset_y              : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_SKIP_CRG;

/* Define the union U_ISP_AE_TOTAL_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    total_pixels          : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_STAT;

/* Define the union U_ISP_AE_COUNT_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    count_pixels          : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_COUNT_STAT;

/* Define the union U_ISP_AE_TOTAL_R_AVER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int total_r_aver           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_TOTAL_R_AVER;
/* Define the union U_ISP_AE_TOTAL_GR_AVER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int total_gr_aver          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_TOTAL_GR_AVER;
/* Define the union U_ISP_AE_TOTAL_GB_AVER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int total_gb_aver          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_TOTAL_GB_AVER;
/* Define the union U_ISP_AE_TOTAL_B_AVER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int total_b_aver           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_TOTAL_B_AVER;
/* Define the union U_ISP_AE_HIST_HIGH */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_high              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_HIST_HIGH;
/* Define the union U_ISP_AE_BITMOVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitmove               : 8   ; /* [7..0]  */
        unsigned int    gamma_en              : 1   ; /* [8]  */
        unsigned int    blc_en                : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_BITMOVE;

/* Define the union U_ISP_AE_OFFSET_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_r              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_R;

/* Define the union U_ISP_AE_OFFSET_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gr             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_GR;

/* Define the union U_ISP_AE_OFFSET_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gb             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_GB;

/* Define the union U_ISP_AE_OFFSET_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_b              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_B;

/* Define the union U_ISP_AE_GAMMA_LIMIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_limit           : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_GAMMA_LIMIT;

/* Define the union U_ISP_AE_FOURPLANEMODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fourplanemode         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_FOURPLANEMODE;

/* Define the union U_ISP_AE_MEM_HIST_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_waddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_HIST_WADDR;
/* Define the union U_ISP_AE_MEM_HIST_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_wdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_HIST_WDATA;
/* Define the union U_ISP_AE_MEM_HIST_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_raddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_HIST_RADDR;
/* Define the union U_ISP_AE_MEM_HIST_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_rdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_HIST_RDATA;
/* Define the union U_ISP_AE_MEM_AVER_R_GR_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_r_gr_waddr        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_R_GR_WADDR;
/* Define the union U_ISP_AE_MEM_AVER_R_GR_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_r_gr_wdata        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_R_GR_WDATA;
/* Define the union U_ISP_AE_MEM_AVER_R_GR_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_r_gr_raddr        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_R_GR_RADDR;
/* Define the union U_ISP_AE_MEM_AVER_R_GR_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_r_gr_rdata        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_R_GR_RDATA;
/* Define the union U_ISP_AE_MEM_AVER_GB_B_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_gb_b_waddr        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_GB_B_WADDR;
/* Define the union U_ISP_AE_MEM_AVER_GB_B_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_gb_b_wdata        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_GB_B_WDATA;
/* Define the union U_ISP_AE_MEM_AVER_GB_B_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_gb_b_raddr        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_GB_B_RADDR;
/* Define the union U_ISP_AE_MEM_AVER_GB_B_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_gb_b_rdata        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_AVER_GB_B_RDATA;
/* Define the union U_ISP_AE_MEM_WEIGHT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_waddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_WEIGHT_WADDR;
/* Define the union U_ISP_AE_MEM_WEIGHT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_wdata              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_WEIGHT_WDATA;
/* Define the union U_ISP_AE_MEM_WEIGHT_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_raddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_WEIGHT_RADDR;
/* Define the union U_ISP_AE_MEM_WEIGHT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_rdata              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AE_MEM_WEIGHT_RDATA;
/* Define the union U_ISP_AE_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_CTRL_F;

/* Define the union U_ISP_AE_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_CTRL_I;

/* Define the union U_ISP_AE_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_UPDATE;

/* Define the union U_ISP_AE_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsize                 : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_SIZE;

/* Define the union U_ISP_AWB_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    work_en               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CFG;

/* Define the union U_ISP_AWB_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AWB_VERSION;
/* Define the union U_ISP_AWB_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    vnum                  : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_ZONE;

/* Define the union U_ISP_AWB_BITMOVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitmove               : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_BITMOVE;

/* Define the union U_ISP_AWB_THD_MIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    threshold_min         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_THD_MIN;

/* Define the union U_ISP_AWB_THD_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    threshold_max         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_THD_MAX;

/* Define the union U_ISP_AWB_CR_MM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cr_ref_max            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    cr_ref_min            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CR_MM;

/* Define the union U_ISP_AWB_CB_MM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cb_ref_max            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    cb_ref_min            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CB_MM;

/* Define the union U_ISP_AWB_OFFSET_COMP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_comp           : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_OFFSET_COMP;

/* Define the union U_ISP_AWB_TOP_K */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    awb_top_k             : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_TOP_K;

/* Define the union U_ISP_AWB_TOP_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    awb_top_b             : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_TOP_B;

/* Define the union U_ISP_AWB_BOT_K */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    awb_bot_k             : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_BOT_K;

/* Define the union U_ISP_AWB_BOT_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    awb_bot_b             : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_BOT_B;

/* Define the union U_ISP_AWB_AVG_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avg_r                 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_R;

/* Define the union U_ISP_AWB_AVG_G */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avg_g                 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_G;

/* Define the union U_ISP_AWB_AVG_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    avg_b                 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_B;

/* Define the union U_ISP_AWB_CNT_ALL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    count_all             : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CNT_ALL;

/* Define the union U_ISP_AWB_STAT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AWB_STAT_WADDR;
/* Define the union U_ISP_AWB_STAT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AWB_STAT_WDATA;
/* Define the union U_ISP_AWB_STAT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stat_raddr            : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_STAT_RADDR;

/* Define the union U_ISP_AWB_STAT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int stat_rdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AWB_STAT_RDATA;
/* Define the union U_ISP_AWB_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CTRL_F;

/* Define the union U_ISP_AWB_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CTRL_I;

/* Define the union U_ISP_AWB_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_UPDATE;

/* Define the union U_ISP_AWB_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsize                 : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_SIZE;

/* Define the union U_ISP_DIS_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    work_en               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_CFG;

/* Define the union U_ISP_DIS_BLK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blk_size_0            : 1   ; /* [0]  */
        unsigned int    blk_size              : 7   ; /* [7..1]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    srch_range_0          : 1   ; /* [16]  */
        unsigned int    srch_range            : 6   ; /* [22..17]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_BLK;

/* Define the union U_ISP_DIS_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_VERSION;
/* Define the union U_ISP_DIS_V0POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V0POS;

/* Define the union U_ISP_DIS_V4POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V4POS;

/* Define the union U_ISP_DIS_V8POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V8POS;

/* Define the union U_ISP_DIS_V0POSE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V0POSE;

/* Define the union U_ISP_DIS_V4POSE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V4POSE;

/* Define the union U_ISP_DIS_V8POSE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V8POSE;

/* Define the union U_ISP_DIS_H0POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H0POS;

/* Define the union U_ISP_DIS_H4POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H4POS;

/* Define the union U_ISP_DIS_H8POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H8POS;

/* Define the union U_ISP_DIS_H0POSE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H0POSE;

/* Define the union U_ISP_DIS_H4POSE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H4POSE;

/* Define the union U_ISP_DIS_H8POSE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hor                   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    ver                   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H8POSE;

/* Define the union U_ISP_DIS_RAW_LUMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    raw_luma              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_RAW_LUMA;

/* Define the union U_ISP_DIS_GAMMA_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_en              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_GAMMA_EN;

/* Define the union U_ISP_DIS_H_STAT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_H_STAT_WADDR;
/* Define the union U_ISP_DIS_H_STAT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_H_STAT_WDATA;
/* Define the union U_ISP_DIS_H_STAT_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int h_stat_raddr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_H_STAT_RADDR;
/* Define the union U_ISP_DIS_H_STAT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int h_stat_rdata           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_H_STAT_RDATA;
/* Define the union U_ISP_DIS_V_STAT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_V_STAT_WADDR;
/* Define the union U_ISP_DIS_V_STAT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_V_STAT_WDATA;
/* Define the union U_ISP_DIS_V_STAT_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int v_stat_raddr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_V_STAT_RADDR;
/* Define the union U_ISP_DIS_V_STAT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int v_stat_rdata           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DIS_V_STAT_RDATA;
/* Define the union U_ISP_DIS_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_CTRL_F;

/* Define the union U_ISP_DIS_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_CTRL_I;

/* Define the union U_ISP_DIS_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_UPDATE;

/* Define the union U_ISP_MG_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_CFG;

/* Define the union U_ISP_MG_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_MG_VERSION;
/* Define the union U_ISP_MG_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    vnum                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_ZONE;

/* Define the union U_ISP_MG_BITMOVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitmove               : 8   ; /* [7..0]  */
        unsigned int    gamma_en              : 1   ; /* [8]  */
        unsigned int    blc_en                : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_BITMOVE;

/* Define the union U_ISP_MG_OFFSET_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_r              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_OFFSET_R;

/* Define the union U_ISP_MG_OFFSET_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gr             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_OFFSET_GR;

/* Define the union U_ISP_MG_OFFSET_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gb             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_OFFSET_GB;

/* Define the union U_ISP_MG_OFFSET_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_b              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_OFFSET_B;

/* Define the union U_ISP_MG_GAMMA_LIMIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gamma_limit           : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_GAMMA_LIMIT;

/* Define the union U_ISP_MG_MEM_AVER_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_waddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_MG_MEM_AVER_WADDR;
/* Define the union U_ISP_MG_MEM_AVER_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_wdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_MG_MEM_AVER_WDATA;
/* Define the union U_ISP_MG_MEM_AVER_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_raddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_MG_MEM_AVER_RADDR;
/* Define the union U_ISP_MG_MEM_AVER_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver_rdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_MG_MEM_AVER_RDATA;
/* Define the union U_ISP_MG_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_CTRL_F;

/* Define the union U_ISP_MG_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_CTRL_I;

/* Define the union U_ISP_MG_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_UPDATE;

/* Define the union U_ISP_MG_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsize                 : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MG_SIZE;

/* Define the union U_ISP_FLICK_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CFG;

/* Define the union U_ISP_FLICK_FRM_MERGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frm_merge             : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_FRM_MERGE;

/* Define the union U_ISP_FLICK_BLC_IN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frm0_blc              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    frm1_blc              : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_BLC_IN;

/* Define the union U_ISP_FLICK_EXP_RAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    exp_ratios0           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_EXP_RAT;

/* Define the union U_ISP_FLICK_OVER_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    over_th               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_OVER_TH;

/* Define the union U_ISP_FLICK_PRE_AVG_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pre_avg_gr            : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_PRE_AVG_GR;

/* Define the union U_ISP_FLICK_PRE_AVG_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pre_avg_gb            : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_PRE_AVG_GB;

/* Define the union U_ISP_FLICK_CUR_SUM_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_sum_gr            : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CUR_SUM_GR;

/* Define the union U_ISP_FLICK_CUR_SUM_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_sum_gb            : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CUR_SUM_GB;

/* Define the union U_ISP_FLICK_CUR_ABS_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_abs_gr            : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CUR_ABS_GR;

/* Define the union U_ISP_FLICK_CUR_ABS_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_abs_gb            : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CUR_ABS_GB;

/* Define the union U_ISP_FLICK_PGMEAN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gmean_waddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FLICK_PGMEAN_WADDR;
/* Define the union U_ISP_FLICK_PGMEAN_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbmean_wdata          : 11  ; /* [10..0]  */
        unsigned int    grmean_wdata          : 11  ; /* [21..11]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_PGMEAN_WDATA;

/* Define the union U_ISP_FLICK_PGMEAN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gmean_raddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FLICK_PGMEAN_RADDR;
/* Define the union U_ISP_FLICK_PGMEAN_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gbmean_rdata          : 11  ; /* [10..0]  */
        unsigned int    grmean_rdata          : 11  ; /* [21..11]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_PGMEAN_RDATA;

/* Define the union U_ISP_FLICK_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CTRL_F;

/* Define the union U_ISP_FLICK_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CTRL_I;

/* Define the union U_ISP_FLICK_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    newer                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_UPDATE;

/* Define the union U_ISP_FLICK_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_SIZE;

/* Define the union U_ISP_LSC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_CFG;

/* Define the union U_ISP_LSC_WINNUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    numh                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    numv                  : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINNUM;

/* Define the union U_ISP_LSC_WINX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX;

/* Define the union U_ISP_LSC_WINX1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax1               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx1                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX1;

/* Define the union U_ISP_LSC_WINX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax2               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx2                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX2;

/* Define the union U_ISP_LSC_WINX3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax3               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx3                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX3;

/* Define the union U_ISP_LSC_WINX4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax4               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx4                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX4;

/* Define the union U_ISP_LSC_WINX5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax5               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx5                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX5;

/* Define the union U_ISP_LSC_WINX6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax6               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx6                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX6;

/* Define the union U_ISP_LSC_WINX7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax7               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx7                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX7;

/* Define the union U_ISP_LSC_WINX8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltax8               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invx8                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX8;

/* Define the union U_ISP_LSC_WINY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY;

/* Define the union U_ISP_LSC_WINY1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay1               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy1                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY1;

/* Define the union U_ISP_LSC_WINY2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay2               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy2                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY2;

/* Define the union U_ISP_LSC_WINY3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay3               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy3                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY3;

/* Define the union U_ISP_LSC_WINY4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay4               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy4                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY4;

/* Define the union U_ISP_LSC_WINY5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay5               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy5                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY5;

/* Define the union U_ISP_LSC_WINY6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay6               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy6                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY6;

/* Define the union U_ISP_LSC_WINY7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay7               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy7                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY7;

/* Define the union U_ISP_LSC_WINY8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    deltay8               : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    invy8                 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY8;

/* Define the union U_ISP_LSC_BLCEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blc_in_en             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    blc_out_en            : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BLCEN;

/* Define the union U_ISP_LSC_RBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blc_r                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_RBLC;

/* Define the union U_ISP_LSC_GRBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blc_gr                : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRBLC;

/* Define the union U_ISP_LSC_GBBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blc_gb                : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBBLC;

/* Define the union U_ISP_LSC_BBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blc_b                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BBLC;

/* Define the union U_ISP_LSC_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_update            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_LUT_UPDATE;

/* Define the union U_ISP_LSC_GRRGAIN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rgain_waddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_LSC_GRRGAIN_WADDR;
/* Define the union U_ISP_LSC_GRRGAIN_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgain_wdata           : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRRGAIN_WDATA;

/* Define the union U_ISP_LSC_GRRGAIN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int rgain_raddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_LSC_GRRGAIN_RADDR;
/* Define the union U_ISP_LSC_GRRGAIN_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain_rdata          : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRRGAIN_RDATA;

/* Define the union U_ISP_LSC_GBBGAIN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int grgain_waddr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_LSC_GBBGAIN_WADDR;
/* Define the union U_ISP_LSC_GBBGAIN_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain_wdata          : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBBGAIN_WDATA;

/* Define the union U_ISP_LSC_GBBGAIN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int grgain_raddr           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_LSC_GBBGAIN_RADDR;
/* Define the union U_ISP_LSC_GBBGAIN_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grgain_rdata          : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBBGAIN_RDATA;

/* Define the union U_ISP_LSC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_CTRL_F;

/* Define the union U_ISP_LSC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_CTRL_I;

/* Define the union U_ISP_LSC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    newer                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_UPDATE;

/* Define the union U_ISP_GE_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge0_en                : 1   ; /* [0]  */
        unsigned int    ge1_en                : 1   ; /* [1]  */
        unsigned int    ge2_en                : 1   ; /* [2]  */
        unsigned int    ge3_en                : 1   ; /* [3]  */
        unsigned int    ge_aver_en            : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    ge_cor_en             : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_CFG;

/* Define the union U_ISP_GE_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_VERSION;
/* Define the union U_ISP_GE0_CT_TH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge0_ct_th1            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_TH1;

/* Define the union U_ISP_GE0_CT_TH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge0_ct_th2            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_TH2;

/* Define the union U_ISP_GE0_CT_TH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge0_ct_th3            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_TH3;

/* Define the union U_ISP_GE0_CT_SLOPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge0_ct_slope1         : 4   ; /* [3..0]  */
        unsigned int    ge0_ct_slope2         : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_SLOPE;

/* Define the union U_ISP_GE1_CT_TH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge1_ct_th1            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_TH1;

/* Define the union U_ISP_GE1_CT_TH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge1_ct_th2            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_TH2;

/* Define the union U_ISP_GE1_CT_TH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge1_ct_th3            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_TH3;

/* Define the union U_ISP_GE1_CT_SLOPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge1_ct_slope1         : 4   ; /* [3..0]  */
        unsigned int    ge1_ct_slope2         : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_SLOPE;

/* Define the union U_ISP_GE2_CT_TH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge2_ct_th1            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_TH1;

/* Define the union U_ISP_GE2_CT_TH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge2_ct_th2            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_TH2;

/* Define the union U_ISP_GE2_CT_TH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge2_ct_th3            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_TH3;

/* Define the union U_ISP_GE2_CT_SLOPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge2_ct_slope1         : 4   ; /* [3..0]  */
        unsigned int    ge2_ct_slope2         : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_SLOPE;

/* Define the union U_ISP_GE3_CT_TH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge3_ct_th1            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_TH1;

/* Define the union U_ISP_GE3_CT_TH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge3_ct_th2            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_TH2;

/* Define the union U_ISP_GE3_CT_TH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge3_ct_th3            : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_TH3;

/* Define the union U_ISP_GE3_CT_SLOPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge3_ct_slope1         : 4   ; /* [3..0]  */
        unsigned int    ge3_ct_slope2         : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_SLOPE;

/* Define the union U_ISP_GE_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    vnum                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_ZONE;

/* Define the union U_ISP_GE_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ge_mode                 : 3   ; /* [0]  */
        //unsigned int    gb_en                 : 1   ; /* [1]  */
        //unsigned int    gr_gb_en              : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MODE;

/* Define the union U_ISP_GE_STRENGTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    strength              : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_STRENGTH;

/* Define the union U_ISP_GE_MEM_AVER_WADDR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver0_waddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_WADDR0;
/* Define the union U_ISP_GE_MEM_AVER_WDATA0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver0_wdata            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_WDATA0;
/* Define the union U_ISP_GE_MEM_AVER_RADDR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver0_raddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_RADDR0;
/* Define the union U_ISP_GE_MEM_AVER_RDATA0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver0_rdata            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_RDATA0;
/* Define the union U_ISP_GE_MEM_AVER_WADDR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver1_waddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_WADDR1;
/* Define the union U_ISP_GE_MEM_AVER_WDATA1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver1_wdata            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_WDATA1;
/* Define the union U_ISP_GE_MEM_AVER_RADDR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver1_raddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_RADDR1;
/* Define the union U_ISP_GE_MEM_AVER_RDATA1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int aver1_rdata            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GE_MEM_AVER_RDATA1;
/* Define the union U_ISP_GE_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_CTRL_F;

/* Define the union U_ISP_GE_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_CTRL_I;

/* Define the union U_ISP_GE_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_TIMING;

/* Define the union U_ISP_GE_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_UPDATE;

/* Define the union U_ISP_GE_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_SIZE;

/* Define the union U_ISP_FPN_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    enable                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    calib_corr            : 1   ; /* [8]  */
        unsigned int    line_frame            : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CFG;

/* Define the union U_ISP_FPN_CALIB_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    calib_start           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CALIB_START;

/* Define the union U_ISP_FPN_CORR_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    correct0_en           : 1   ; /* [0]  */
        unsigned int    correct1_en           : 1   ; /* [1]  */
        unsigned int    correct2_en           : 1   ; /* [2]  */
        unsigned int    correct3_en           : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR_CFG;

/* Define the union U_ISP_FPN_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    busy                  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    vcnt                  : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    hcnt                  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_STAT;

/* Define the union U_ISP_FPN_WHITE_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    white_level           : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_WHITE_LEVEL;

/* Define the union U_ISP_FPN_DIVCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    divcoef               : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_DIVCOEF;

/* Define the union U_ISP_FPN_FRAMELOG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cpi_fpn_framelog2     : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_FRAMELOG2;

/* Define the union U_ISP_FPN_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_SUM0;
/* Define the union U_ISP_FPN_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_SUM1;
/* Define the union U_ISP_FPN_CORR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    strength              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR0;

/* Define the union U_ISP_FPN_CORR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    strength              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR1;

/* Define the union U_ISP_FPN_CORR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    strength              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR2;

/* Define the union U_ISP_FPN_CORR3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    strength              : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR3;

/* Define the union U_ISP_FPN_SHIFT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fpn_shift             : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    in_shift              : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    out_shift             : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    frame_calib_shift     : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SHIFT;

/* Define the union U_ISP_FPN_MAX_O */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_o                 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_MAX_O;

/* Define the union U_ISP_FPN_OVERFLOWTHR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    overflowthr           : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_OVERFLOWTHR;

/* Define the union U_ISP_FPN_RESERVED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fpn_reserved           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_RESERVED;
/* Define the union U_ISP_FPN_LINE_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fpn_line_waddr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_LINE_WADDR;
/* Define the union U_ISP_FPN_LINE_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fpn_line_wdata         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_LINE_WDATA;
/* Define the union U_ISP_FPN_LINE_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fpn_line_raddr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_LINE_RADDR;
/* Define the union U_ISP_FPN_LINE_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fpn_line_rdata         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_FPN_LINE_RDATA;
/* Define the union U_ISP_FPN_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CTRL_F;

/* Define the union U_ISP_FPN_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CTRL_I;

/* Define the union U_ISP_FPN_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fpn_reserved1         : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_TIMING;

/* Define the union U_ISP_FPN_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_UPDATE;

/* Define the union U_ISP_FPN_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 16  ; /* [15..0]  */
        unsigned int    height                : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SIZE;

/* Define the union U_ISP_DPC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en0                   : 1   ; /* [0]  */
        unsigned int    en1                   : 1   ; /* [1]  */
        unsigned int    en2                   : 1   ; /* [2]  */
        unsigned int    en3                   : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_CFG;

/* Define the union U_ISP_DCG_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mode                  : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DCG_MODE;

/* Define the union U_ISP_DPC_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    alpha1_g              : 8   ; /* [7..0]  */
        unsigned int    alpha1_rb             : 8   ; /* [15..8]  */
        unsigned int    alpha0_g              : 8   ; /* [23..16]  */
        unsigned int    alpha0_rb             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_ALPHA;

/* Define the union U_ISP_DPC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DPC_VERSION;
/* Define the union U_ISP_DPC_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    det_en                : 1   ; /* [0]  */
        unsigned int    cor_en                : 1   ; /* [1]  */
        unsigned int    six_det_en            : 1   ; /* [2]  */
        unsigned int    bpt_update            : 1   ; /* [3]  */
        unsigned int    grayscale_mode        : 1   ; /* [4]  */
        unsigned int    bpt_cor_en            : 1   ; /* [5]  */
        unsigned int    dpt_det_sel           : 1   ; /* [6]  */
        unsigned int    dp_hightlight_en      : 1   ; /* [7]  */
        unsigned int    ir_position           : 1   ; /* [8]  */
        unsigned int    ir_channel            : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_MODE;

/* Define the union U_ISP_DPC_OUTPUT_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stage1_incl_gr_center : 1   ; /* [0]  */
        unsigned int    stage1_incl_rb_center : 1   ; /* [1]  */
        unsigned int    stage1_incl_g_3x3     : 1   ; /* [2]  */
        unsigned int    stage1_incl_rb_3x3    : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_OUTPUT_MODE;

/* Define the union U_ISP_DPC_SET_USE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    stage1_use_set1       : 1   ; /* [0]  */
        unsigned int    stage1_use_set2       : 1   ; /* [1]  */
        unsigned int    stage1_use_set3       : 1   ; /* [2]  */
        unsigned int    stage1_use_fix_set    : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_SET_USE;

/* Define the union U_ISP_DPC_METHODS_SET_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pg_green1_enable      : 1   ; /* [0]  */
        unsigned int    lc_green1_enable      : 1   ; /* [1]  */
        unsigned int    ro_green1_enable      : 1   ; /* [2]  */
        unsigned int    rnd_green1_enable     : 1   ; /* [3]  */
        unsigned int    rg_green1_enable      : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    pg_red_blue1_enable   : 1   ; /* [8]  */
        unsigned int    lc_red_blue1_enable   : 1   ; /* [9]  */
        unsigned int    ro_red_blue1_enable   : 1   ; /* [10]  */
        unsigned int    rnd_red_blue1_enable  : 1   ; /* [11]  */
        unsigned int    rg_red_blue1_enable   : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_METHODS_SET_1;

/* Define the union U_ISP_DPC_METHODS_SET_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pg_green2_enable      : 1   ; /* [0]  */
        unsigned int    lc_green2_enable      : 1   ; /* [1]  */
        unsigned int    ro_green2_enable      : 1   ; /* [2]  */
        unsigned int    rnd_green2_enable     : 1   ; /* [3]  */
        unsigned int    rg_green2_enable      : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    pg_red_blue2_enable   : 1   ; /* [8]  */
        unsigned int    lc_red_blue2_enable   : 1   ; /* [9]  */
        unsigned int    ro_red_blue2_enable   : 1   ; /* [10]  */
        unsigned int    rnd_red_blue2_enable  : 1   ; /* [11]  */
        unsigned int    rg_red_blue2_enable   : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_METHODS_SET_2;

/* Define the union U_ISP_DPC_METHODS_SET_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pg_green3_enable      : 1   ; /* [0]  */
        unsigned int    lc_green3_enable      : 1   ; /* [1]  */
        unsigned int    ro_green3_enable      : 1   ; /* [2]  */
        unsigned int    rnd_green3_enable     : 1   ; /* [3]  */
        unsigned int    rg_green3_enable      : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    pg_red_blue3_enable   : 1   ; /* [8]  */
        unsigned int    lc_red_blue3_enable   : 1   ; /* [9]  */
        unsigned int    ro_red_blue3_enable   : 1   ; /* [10]  */
        unsigned int    rnd_red_blue3_enable  : 1   ; /* [11]  */
        unsigned int    rg_red_blue3_enable   : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_METHODS_SET_3;

/* Define the union U_ISP_DPC_LINE_THRESH_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    line_thr1_g           : 8   ; /* [7..0]  */
        unsigned int    line_thr1_rb          : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_THRESH_1;

/* Define the union U_ISP_DPC_LINE_MAD_FAC_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    line_mad_fac_1_g      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    line_mad_fac_1_rb     : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_MAD_FAC_1;

/* Define the union U_ISP_DPC_PG_FAC_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pg_fac_1_g            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    pg_fac_1_rb           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_PG_FAC_1;

/* Define the union U_ISP_DPC_RND_THRESH_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rnd_thr1_g            : 8   ; /* [7..0]  */
        unsigned int    rnd_thr1_rb           : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_THRESH_1;

/* Define the union U_ISP_DPC_RG_FAC_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rg_fac_1_g            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    rg_fac_1_rb           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RG_FAC_1;

/* Define the union U_ISP_DPC_LINE_THRESH_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    line_thr2_g           : 8   ; /* [7..0]  */
        unsigned int    line_thr2_rb          : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_THRESH_2;

/* Define the union U_ISP_DPC_LINE_MAD_FAC_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    line_mad_fac_2_g      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    line_mad_fac_2_rb     : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_MAD_FAC_2;

/* Define the union U_ISP_DPC_PG_FAC_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pg_fac_2_g            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    pg_fac_2_rb           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_PG_FAC_2;

/* Define the union U_ISP_DPC_RND_THRESH_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rnd_thr2_g            : 8   ; /* [7..0]  */
        unsigned int    rnd_thr2_rb           : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_THRESH_2;

/* Define the union U_ISP_DPC_RG_FAC_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rg_fac_2_g            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    rg_fac_2_rb           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RG_FAC_2;

/* Define the union U_ISP_DPC_LINE_THRESH_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    line_thr3_g           : 8   ; /* [7..0]  */
        unsigned int    line_thr3_rb          : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_THRESH_3;

/* Define the union U_ISP_DPC_LINE_MAD_FAC_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    line_mad_fac_3_g      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    line_mad_fac_3_rb     : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_MAD_FAC_3;

/* Define the union U_ISP_DPC_PG_FAC_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pg_fac_3_g            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    pg_fac_3_rb           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_PG_FAC_3;

/* Define the union U_ISP_DPC_RND_THRESH_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rnd_thr3_g            : 8   ; /* [7..0]  */
        unsigned int    rnd_thr3_rb           : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_THRESH_3;

/* Define the union U_ISP_DPC_RG_FAC_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rg_fac_3_g            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    rg_fac_3_rb           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RG_FAC_3;

/* Define the union U_ISP_DPC_RO_LIMITS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ro_1_g                : 2   ; /* [1..0]  */
        unsigned int    ro_1_rb               : 2   ; /* [3..2]  */
        unsigned int    ro_2_g                : 2   ; /* [5..4]  */
        unsigned int    ro_2_rb               : 2   ; /* [7..6]  */
        unsigned int    ro_3_g                : 2   ; /* [9..8]  */
        unsigned int    ro_3_rb               : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RO_LIMITS;

/* Define the union U_ISP_DPC_RND_OFFS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rnd_offs_1_g          : 2   ; /* [1..0]  */
        unsigned int    rnd_offs_1_rb         : 2   ; /* [3..2]  */
        unsigned int    rnd_offs_2_g          : 2   ; /* [5..4]  */
        unsigned int    rnd_offs_2_rb         : 2   ; /* [7..6]  */
        unsigned int    rnd_offs_3_g          : 2   ; /* [9..8]  */
        unsigned int    rnd_offs_3_rb         : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_OFFS;

/* Define the union U_ISP_DPC_BPT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    bpt_use_set_1         : 1   ; /* [4]  */
        unsigned int    bpt_use_set_2         : 1   ; /* [5]  */
        unsigned int    bpt_use_set_3         : 1   ; /* [6]  */
        unsigned int    bpt_use_fix_set       : 1   ; /* [7]  */
        unsigned int    bpt_incl_green_center : 1   ; /* [8]  */
        unsigned int    bpt_incl_rb_center    : 1   ; /* [9]  */
        unsigned int    g_3x3                 : 1   ; /* [10]  */
        unsigned int    rb_3x3                : 1   ; /* [11]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    bp_data               : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_CTRL;

/* Define the union U_ISP_DPC_BPT_NUMBER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpt_number            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_NUMBER;

/* Define the union U_ISP_DPC_BPT_CALIB_NUMBER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bpt_calib_number      : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_CALIB_NUMBER;

/* Define the union U_ISP_DPC_RESERVED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DPC_RESERVED;
/* Define the union U_ISP_DPC_BPT_THRD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dev_dead_thresh       : 8   ; /* [7..0]  */
        unsigned int    abs_dead_thresh       : 8   ; /* [15..8]  */
        unsigned int    dev_hot_thresh        : 8   ; /* [23..16]  */
        unsigned int    abs_hot_thresh        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_THRD;

/* Define the union U_ISP_DPC_BPT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int bpt_waddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DPC_BPT_WADDR;
/* Define the union U_ISP_DPC_BPT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int bpt_wdata              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DPC_BPT_WDATA;
/* Define the union U_ISP_DPC_BPT_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int bpt_raddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DPC_BPT_RADDR;
/* Define the union U_ISP_DPC_BPT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int bpt_rdata              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DPC_BPT_RDATA;
/* Define the union U_ISP_DPC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_CTRL_F;

/* Define the union U_ISP_DPC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_CTRL_I;

/* Define the union U_ISP_DPC_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_TIMING;

/* Define the union U_ISP_DPC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_UPDATE;

/* Define the union U_ISP_DPC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_SIZE;

/* Define the union U_ISP_DPC_SOFT_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    soft_thr_min          : 8   ; /* [7..0]  */
        unsigned int    soft_thr_max          : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_SOFT_THR;

/* Define the union U_ISP_DPC_HARDTHR_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hard_thr_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_HARDTHR_EN;

/* Define the union U_ISP_DPC_RAKERATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rake_ratio            : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RAKERATIO;

/* Define the union U_ISP_DPC_EX_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_EX_CTRL_I;

/* Define the union U_ISP_DPC_EX_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_EX_UPDATE;

/* Define the union U_ISP_BNR_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_en                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CFG;

/* Define the union U_ISP_BNR_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_VERSION;
/* Define the union U_ISP_BNR_CRATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rg_bnr_cratio         : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    gr_bnr_cratio         : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    gb_bnr_cratio         : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    bg_bnr_cratio         : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CRATIO;

/* Define the union U_ISP_BNR_LEV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rg_amed_lev           : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    gr_amed_lev           : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 6   ; /* [15..10]  */
        unsigned int    gb_amed_lev           : 2   ; /* [17..16]  */
        unsigned int    reserved_2            : 6   ; /* [23..18]  */
        unsigned int    bg_amed_lev           : 2   ; /* [25..24]  */
        unsigned int    reserved_3            : 2   ; /* [27..26]  */
        unsigned int    rg_amed_mode          : 1   ; /* [28]  */
        unsigned int    gr_amed_mode          : 1   ; /* [29]  */
        unsigned int    gb_amed_mode          : 1   ; /* [30]  */
        unsigned int    bg_amed_mode          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LEV;

/* Define the union U_ISP_BNR_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_jnlm_sel          : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    bnr_jnlm_bitshift     : 4   ; /* [7..4]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_SEL;

/* Define the union U_ISP_BNR_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_jnlm_gain         : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_GAIN;

/* Define the union U_ISP_BNR_SYMCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_jnlm_symcoef      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_SYMCOEF;

/* Define the union U_ISP_BNR_CORING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    bnr_coring_hig        : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CORING;

/* Define the union U_ISP_BNR_FRMNUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frm_num               : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_FRMNUM;

/* Define the union U_ISP_BNR_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_update            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LUT_UPDATE;

/* Define the union U_ISP_BNR_RLMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_rlmt_blc          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    bnr_rlmt_rgain        : 10  ; /* [17..8]  */
        unsigned int    reserved_1            : 2   ; /* [19..18]  */
        unsigned int    bnr_rlmt_bgain        : 10  ; /* [29..20]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_RLMT;

/* Define the union U_ISP_BNR_SATU0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_satu_ratio        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_SATU0;

/* Define the union U_ISP_BNR_SATU1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_satu_thlow1       : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    bnr_satu_thhig1       : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_SATU1;

/* Define the union U_ISP_BNR_SATU2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bnr_satu_thlow2       : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    bnr_satu_thhig2       : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_SATU2;

/* Define the union U_ISP_BNR_RGGB_ODD_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_ODD_WADDR;
/* Define the union U_ISP_BNR_RGGB_ODD_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_ODD_WDATA;
/* Define the union U_ISP_BNR_RGGB_ODD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_ODD_RADDR;
/* Define the union U_ISP_BNR_RGGB_ODD_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_ODD_RDATA;
/* Define the union U_ISP_BNR_GRBG_ODD_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_ODD_WADDR;
/* Define the union U_ISP_BNR_GRBG_ODD_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_ODD_WDATA;
/* Define the union U_ISP_BNR_GRBG_ODD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_ODD_RADDR;
/* Define the union U_ISP_BNR_GRBG_ODD_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_ODD_RDATA;
/* Define the union U_ISP_BNR_GBRG_ODD_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_ODD_WADDR;
/* Define the union U_ISP_BNR_GBRG_ODD_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_ODD_WDATA;
/* Define the union U_ISP_BNR_GBRG_ODD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_ODD_RADDR;
/* Define the union U_ISP_BNR_GBRG_ODD_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_ODD_RDATA;
/* Define the union U_ISP_BNR_BGGR_ODD_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_ODD_WADDR;
/* Define the union U_ISP_BNR_BGGR_ODD_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_ODD_WDATA;
/* Define the union U_ISP_BNR_BGGR_ODD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_ODD_RADDR;
/* Define the union U_ISP_BNR_BGGR_ODD_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_ODD_RDATA;
/* Define the union U_ISP_BNR_RLMT_ODD_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_ODD_WADDR;
/* Define the union U_ISP_BNR_RLMT_ODD_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_ODD_WDATA;
/* Define the union U_ISP_BNR_RLMT_ODD_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_ODD_RADDR;
/* Define the union U_ISP_BNR_RLMT_ODD_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_ODD_RDATA;
/* Define the union U_ISP_BNR_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CTRL_F;

/* Define the union U_ISP_BNR_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CTRL_I;

/* Define the union U_ISP_BNR_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_TIMING;

/* Define the union U_ISP_BNR_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_UPDATE;

/* Define the union U_ISP_BNR_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_SIZE;

/* Define the union U_ISP_BNR_RGGB_EVEN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_EVEN_WADDR;
/* Define the union U_ISP_BNR_RGGB_EVEN_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_EVEN_WDATA;
/* Define the union U_ISP_BNR_RGGB_EVEN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_EVEN_RADDR;
/* Define the union U_ISP_BNR_RGGB_EVEN_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RGGB_EVEN_RDATA;
/* Define the union U_ISP_BNR_GRBG_EVEN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_EVEN_WADDR;
/* Define the union U_ISP_BNR_GRBG_EVEN_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_EVEN_WDATA;
/* Define the union U_ISP_BNR_GRBG_EVEN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_EVEN_RADDR;
/* Define the union U_ISP_BNR_GRBG_EVEN_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GRBG_EVEN_RDATA;
/* Define the union U_ISP_BNR_GBRG_EVEN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_EVEN_WADDR;
/* Define the union U_ISP_BNR_GBRG_EVEN_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_EVEN_WDATA;
/* Define the union U_ISP_BNR_GBRG_EVEN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_EVEN_RADDR;
/* Define the union U_ISP_BNR_GBRG_EVEN_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_GBRG_EVEN_RDATA;
/* Define the union U_ISP_BNR_BGGR_EVEN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_EVEN_WADDR;
/* Define the union U_ISP_BNR_BGGR_EVEN_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_EVEN_WDATA;
/* Define the union U_ISP_BNR_BGGR_EVEN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_EVEN_RADDR;
/* Define the union U_ISP_BNR_BGGR_EVEN_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_BGGR_EVEN_RDATA;
/* Define the union U_ISP_BNR_RLMT_EVEN_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwaddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_EVEN_WADDR;
/* Define the union U_ISP_BNR_RLMT_EVEN_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hwdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_EVEN_WDATA;
/* Define the union U_ISP_BNR_RLMT_EVEN_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hraddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_EVEN_RADDR;
/* Define the union U_ISP_BNR_RLMT_EVEN_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int lut_hrdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BNR_RLMT_EVEN_RDATA;
/* Define the union U_ISP_WDR_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_CFG;

/* Define the union U_ISP_WDR_INBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    f1_inblc              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    f0_inblc              : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_INBLC;

/* Define the union U_ISP_WDR_OUTBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    outblc                : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_OUTBLC;

/* Define the union U_ISP_WDR_EXPOSRATIOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    exporatio             : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    exporatio_r           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_EXPOSRATIOS;

/* Define the union U_ISP_WDR_BLCSAVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bsaveblc              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    esaveblc              : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BLCSAVE;

/* Define the union U_ISP_WDR_ZOOMBLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    zoomblc               : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_ZOOMBLC;

/* Define the union U_ISP_WDR_BLENDRATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bldrclridx            : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    bldrlhfidx            : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BLENDRATIO;

/* Define the union U_ISP_WDR_FLRGTTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    flrgtth_low           : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    flrgtth_high          : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FLRGTTH;

/* Define the union U_ISP_WDR_FLDFTWGT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fl_dftwgt             : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FLDFTWGT;

/* Define the union U_ISP_WDR_MDT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    balgprocmdt           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    bmdtstrong            : 1   ; /* [16]  */
        unsigned int    reserved_1            : 7   ; /* [23..17]  */
        unsigned int    fl_bmdtmnu            : 1   ; /* [24]  */
        unsigned int    reserved_2            : 3   ; /* [27..25]  */
        unsigned int    bmdthf                : 1   ; /* [28]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MDT_CTRL;

/* Define the union U_ISP_WDR_BLDRMDTMAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bldrmdtmax            : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BLDRMDTMAX;

/* Define the union U_ISP_WDR_NOSCLIPTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nosclipth             : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSCLIPTH;

/* Define the union U_ISP_WDR_NOSFACTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nosfactor_high        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    nosfactor_low         : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSFACTOR;

/* Define the union U_ISP_WDR_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noslut_update         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_LUT_UPDATE;

/* Define the union U_ISP_WDR_FSNRJUDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fsnr_judge            : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FSNRJUDGE;

/* Define the union U_ISP_WDR_FSNRTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fsnrth_high           : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    fsnrth_low            : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FSNRTH;

/* Define the union U_ISP_WDR_FSNRGN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fsnrgn_high           : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    fsnrgn_low            : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FSNRGN;

/* Define the union U_ISP_WDR_MDT_REFNOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bmdtrefnos            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MDT_REFNOS;

/* Define the union U_ISP_WDR_NOSLUT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noslut_waddr          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT_WADDR;

/* Define the union U_ISP_WDR_NOSLUT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noslut_wdata          : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT_WDATA;

/* Define the union U_ISP_WDR_NOSLUT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noslut_raddr          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT_RADDR;

/* Define the union U_ISP_WDR_NOSLUT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    noslut_rdata          : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT_RDATA;

/* Define the union U_ISP_WDR_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_CTRL_F;

/* Define the union U_ISP_WDR_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_CTRL_I;

/* Define the union U_ISP_WDR_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_TIMING;

/* Define the union U_ISP_WDR_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_UPDATE;

/* Define the union U_ISP_WDR_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_SIZE;

/* Define the union U_ISP_DRC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    drc_en                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CFG;

/* Define the union U_ISP_DRC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_VERSION;
/* Define the union U_ISP_DRC_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    vnum                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE;

/* Define the union U_ISP_DRC_ZONE_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 9   ; /* [8..0]  */
        unsigned int    chk_x                 : 5   ; /* [13..9]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    vsize                 : 9   ; /* [24..16]  */
        unsigned int    chk_y                 : 5   ; /* [29..25]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_SIZE;

/* Define the union U_ISP_DRC_ZONE_DIV0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    div_x                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    div_y                 : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_DIV0;

/* Define the union U_ISP_DRC_ZONE_DIV1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    div_x                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    div_y                 : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_DIV1;

/* Define the union U_ISP_DRC_BIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bin                   : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN;

/* Define the union U_ISP_DRC_BIN_DIV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    div_z                 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_DIV;

/* Define the union U_ISP_DRC_BIN_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bin_scale             : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_SCALE;

/* Define the union U_ISP_DRC_RANGE_FLT_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    range_flt_coef        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_RANGE_FLT_COEF;

/* Define the union U_ISP_DRC_EXPOSURE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    exposure              : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_EXPOSURE;

/* Define the union U_ISP_DRC_LOCAL_EDGE_LMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    local_edge_lmt        : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_LOCAL_EDGE_LMT;

/* Define the union U_ISP_DRC_STRENGTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    strength              : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_STRENGTH;

/* Define the union U_ISP_DRC_DETAIL_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    detail_mixing_bright  : 8   ; /* [7..0]  */
        unsigned int    detail_mixing_dark    : 8   ; /* [15..8]  */
        unsigned int    detail_mixing_thres   : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DETAIL_GAIN;

/* Define the union U_ISP_DRC_DARK_GAIN_LMT_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    val1                  : 7   ; /* [6..0]  */
        unsigned int    sft1                  : 3   ; /* [9..7]  */
        unsigned int    val2                  : 4   ; /* [13..10]  */
        unsigned int    sft2                  : 2   ; /* [15..14]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_GAIN_LMT_Y;

/* Define the union U_ISP_DRC_DARK_GAIN_LMT_C */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    val1                  : 7   ; /* [6..0]  */
        unsigned int    sft1                  : 3   ; /* [9..7]  */
        unsigned int    val2                  : 4   ; /* [13..10]  */
        unsigned int    sft2                  : 2   ; /* [15..14]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_GAIN_LMT_C;

/* Define the union U_ISP_DRC_BRIGHT_GAIN_LMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    val                   : 7   ; /* [6..0]  */
        unsigned int    sft                   : 3   ; /* [9..7]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BRIGHT_GAIN_LMT;

/* Define the union U_ISP_DRC_RGB_WGT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    r_wgt                 : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    g_wgt                 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    b_wgt                 : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_RGB_WGT;

/* Define the union U_ISP_DRC_LUT_MIX_CRTL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_mix_ctrl          : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_LUT_MIX_CRTL;

/* Define the union U_ISP_DRC_GAIN_CLIP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gain_clip_step        : 4   ; /* [3..0]  */
        unsigned int    gain_clip_knee        : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_GAIN_CLIP;

/* Define the union U_ISP_DRC_COLOR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cc_lut_ctrl           : 1   ; /* [0]  */
        unsigned int    cc_lin_pow            : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_COLOR_CTRL;

/* Define the union U_ISP_DRC_GLOBAL_CORR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cc_global_corr        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_GLOBAL_CORR;

/* Define the union U_ISP_DRC_MIXING_CORING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mixing_coring         : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_MIXING_CORING;

/* Define the union U_ISP_DRC_MIXING_DARK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dark_min              : 8   ; /* [7..0]  */
        unsigned int    dark_max              : 8   ; /* [15..8]  */
        unsigned int    dark_thr              : 8   ; /* [23..16]  */
        unsigned int    dark_slo              : 3   ; /* [26..24]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_MIXING_DARK;

/* Define the union U_ISP_DRC_MIXING_BRIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bright_min            : 8   ; /* [7..0]  */
        unsigned int    bright_max            : 8   ; /* [15..8]  */
        unsigned int    bright_thr            : 8   ; /* [23..16]  */
        unsigned int    bright_slo            : 3   ; /* [26..24]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_MIXING_BRIGHT;

/* Define the union U_ISP_DRC_RG_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rg_ctr                : 8   ; /* [7..0]  */
        unsigned int    rg_wid                : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rg_slo                : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_RG_CTRL;

/* Define the union U_ISP_DRC_BG_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bg_thr                : 8   ; /* [7..0]  */
        unsigned int    bg_slo                : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BG_CTRL;

/* Define the union U_ISP_DRC_PDW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    low_thr               : 8   ; /* [7..0]  */
        unsigned int    low_slo               : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    high_thr              : 8   ; /* [23..16]  */
        unsigned int    high_slo              : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PDW;

/* Define the union U_ISP_DRC_PPDTC_SUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ppdtc_sum             : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PPDTC_SUM;

/* Define the union U_ISP_DRC_STAT_IND_WADDR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_waddr0    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_WADDR0;
/* Define the union U_ISP_DRC_STAT_IND_WDATA0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_wdata0    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_WDATA0;
/* Define the union U_ISP_DRC_STAT_IND_RADDR0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_raddr0    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_RADDR0;
/* Define the union U_ISP_DRC_STAT_IND_RDATA0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_rdata0    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_RDATA0;
/* Define the union U_ISP_DRC_STAT_IND_WADDR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_waddr1    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_WADDR1;
/* Define the union U_ISP_DRC_STAT_IND_WDATA1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_wdata1    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_WDATA1;
/* Define the union U_ISP_DRC_STAT_IND_RADDR1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_raddr1    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_RADDR1;
/* Define the union U_ISP_DRC_STAT_IND_RDATA1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_rdata1    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_RDATA1;
/* Define the union U_ISP_DRC_STAT_IND_WADDR2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_waddr2    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_WADDR2;
/* Define the union U_ISP_DRC_STAT_IND_WDATA2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_wdata2    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_WDATA2;
/* Define the union U_ISP_DRC_STAT_IND_RADDR2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_raddr2    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_RADDR2;
/* Define the union U_ISP_DRC_STAT_IND_RDATA2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int drc_stat_ind_rdata2    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DRC_STAT_IND_RDATA2;
/* Define the union U_ISP_DRC_GRAD_REV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    grad_rev_thres        : 8   ; /* [7..0]  */
        unsigned int    grad_rev_max          : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    grad_rev_slope        : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 4   ; /* [23..20]  */
        unsigned int    grad_rev_shift        : 3   ; /* [26..24]  */
        unsigned int    reserved_2            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_GRAD_REV;

/* Define the union U_ISP_DRC_VBI_STRENGTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    var_rng_fine          : 4   ; /* [3..0]  */
        unsigned int    var_rng_medium        : 4   ; /* [7..4]  */
        unsigned int    var_rng_coarse        : 4   ; /* [11..8]  */
        unsigned int    var_spa_fine          : 4   ; /* [15..12]  */
        unsigned int    var_spa_medium        : 4   ; /* [19..16]  */
        unsigned int    var_sap_coarse        : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_VBI_STRENGTH;

/* Define the union U_ISP_DRC_VBI_MIXING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bin_mix_medium        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    bin_mix_coarse        : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_VBI_MIXING;

/* Define the union U_ISP_DRC_VBI_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbi_state             : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_VBI_STATE;

/* Define the union U_ISP_DRC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CTRL_F;

/* Define the union U_ISP_DRC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CTRL_I;

/* Define the union U_ISP_DRC_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TIMING;

/* Define the union U_ISP_DRC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_UPDATE;

/* Define the union U_ISP_DRC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsize                 : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_SIZE;

/* Define the union U_ISP_EXPANDER_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_CFG;

/* Define the union U_ISP_EXPANDER_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_EXPANDER_VERSION;
/* Define the union U_ISP_EXPANDER_BITW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitw_out              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    bitw_in               : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_BITW;

/* Define the union U_ISP_EXPANDER_OFFSETR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_r              : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_OFFSETR;

/* Define the union U_ISP_EXPANDER_OFFSETGR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gr             : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_OFFSETGR;

/* Define the union U_ISP_EXPANDER_OFFSETGB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gb             : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_OFFSETGB;

/* Define the union U_ISP_EXPANDER_OFFSETB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_b              : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_OFFSETB;

/* Define the union U_ISP_EXPANDER_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int expander_waddr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_EXPANDER_WADDR;
/* Define the union U_ISP_EXPANDER_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int expander_wdata         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_EXPANDER_WDATA;
/* Define the union U_ISP_EXPANDER_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int expander_raddr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_EXPANDER_RADDR;
/* Define the union U_ISP_EXPANDER_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int expander_rdata         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_EXPANDER_RDATA;
/* Define the union U_ISP_EXPANDER_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_CTRL_F;

/* Define the union U_ISP_EXPANDER_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_CTRL_I;

/* Define the union U_ISP_EXPANDER_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_EXPANDER_UPDATE;

/* Define the union U_ISP_BCOM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_CFG;

/* Define the union U_ISP_BCOM_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BCOM_VERSION;
/* Define the union U_ISP_BCOM_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    alpha                 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_ALPHA;

/* Define the union U_ISP_BCOM_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_CTRL_I;

/* Define the union U_ISP_BCOM_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_UPDATE;

/* Define the union U_ISP_BDEC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_CFG;

/* Define the union U_ISP_BDEC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BDEC_VERSION;
/* Define the union U_ISP_BDEC_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    alpha                 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_ALPHA;

/* Define the union U_ISP_BDEC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_CTRL_I;

/* Define the union U_ISP_BDEC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_UPDATE;

/* Define the union U_ISP_WDRSPLIT_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_CFG;

/* Define the union U_ISP_WDRSPLIT_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_WDRSPLIT_VERSION;
/* Define the union U_ISP_WDRSPLIT_BITW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitw_out              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    bitw_in               : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_BITW;

/* Define the union U_ISP_WDRSPLIT_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mode_out              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    mode_in               : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_MODE;

/* Define the union U_ISP_WDRSPLIT_OFFSETR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_r              : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETR;

/* Define the union U_ISP_WDRSPLIT_OFFSETGR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gr             : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETGR;

/* Define the union U_ISP_WDRSPLIT_OFFSETGB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gb             : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETGB;

/* Define the union U_ISP_WDRSPLIT_OFFSETB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_b              : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETB;

/* Define the union U_ISP_WDRSPLIT_BLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blc                   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_BLC;

/* Define the union U_ISP_WDRSPLIT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wdrsplit_waddr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_WDRSPLIT_WADDR;
/* Define the union U_ISP_WDRSPLIT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wdrsplit_wdata         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_WDRSPLIT_WDATA;
/* Define the union U_ISP_WDRSPLIT_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wdrsplit_raddr         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_WDRSPLIT_RADDR;
/* Define the union U_ISP_WDRSPLIT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wdrsplit_rdata         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_WDRSPLIT_RDATA;
/* Define the union U_ISP_WDRSPLIT_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_CTRL_F;

/* Define the union U_ISP_WDRSPLIT_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_CTRL_I;

/* Define the union U_ISP_WDRSPLIT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_UPDATE;

/* Define the union U_ISP_AEWDR0_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_CFG;

/* Define the union U_ISP_AEWDR0_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_VERSION;
/* Define the union U_ISP_AEWDR0_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    vnum                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_ZONE;

/* Define the union U_ISP_AEWDR0_SKIP_CRG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skip_x                : 3   ; /* [2..0]  */
        unsigned int    offset_x              : 1   ; /* [3]  */
        unsigned int    skip_y                : 3   ; /* [6..4]  */
        unsigned int    offset_y              : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_SKIP_CRG;

/* Define the union U_ISP_AEWDR0_TOTAL_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    total_pixels          : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_TOTAL_STAT;

/* Define the union U_ISP_AEWDR0_COUNT_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    count_pixels          : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_COUNT_STAT;

/* Define the union U_ISP_AEWDR0_HIST_HIGH */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_high              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_HIST_HIGH;
/* Define the union U_ISP_AEWDR0_BITMOVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitmove               : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 4   ; /* [8..5]  */
        unsigned int    blc_en                : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_BITMOVE;

/* Define the union U_ISP_AEWDR0_OFFSET_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_r              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_OFFSET_R;

/* Define the union U_ISP_AEWDR0_OFFSET_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gr             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_OFFSET_GR;

/* Define the union U_ISP_AEWDR0_OFFSET_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gb             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_OFFSET_GB;

/* Define the union U_ISP_AEWDR0_OFFSET_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_b              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_OFFSET_B;

/* Define the union U_ISP_AEWDR0_MEM_HIST_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_waddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_HIST_WADDR;
/* Define the union U_ISP_AEWDR0_MEM_HIST_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_wdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_HIST_WDATA;
/* Define the union U_ISP_AEWDR0_MEM_HIST_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_raddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_HIST_RADDR;
/* Define the union U_ISP_AEWDR0_MEM_HIST_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_rdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_HIST_RDATA;
/* Define the union U_ISP_AEWDR0_MEM_WEIGHT_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_waddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_WEIGHT_WADDR;
/* Define the union U_ISP_AEWDR0_MEM_WEIGHT_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_wdata              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_WEIGHT_WDATA;
/* Define the union U_ISP_AEWDR0_MEM_WEIGHT_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_raddr              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_WEIGHT_RADDR;
/* Define the union U_ISP_AEWDR0_MEM_WEIGHT_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int wei_rdata              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR0_MEM_WEIGHT_RDATA;
/* Define the union U_ISP_AEWDR0_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_CTRL_F;

/* Define the union U_ISP_AEWDR0_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_CTRL_I;

/* Define the union U_ISP_AEWDR0_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_UPDATE;

/* Define the union U_ISP_AEWDR0_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsize                 : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR0_SIZE;

/* Define the union U_ISP_AEWDR1_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_CFG;

/* Define the union U_ISP_AEWDR1_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR1_VERSION;
/* Define the union U_ISP_AEWDR1_ZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hnum                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    vnum                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_ZONE;

/* Define the union U_ISP_AEWDR1_SKIP_CRG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skip_x                : 3   ; /* [2..0]  */
        unsigned int    offset_x              : 1   ; /* [3]  */
        unsigned int    skip_y                : 3   ; /* [6..4]  */
        unsigned int    offset_y              : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_SKIP_CRG;

/* Define the union U_ISP_AEWDR1_TOTAL_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    total_pixels          : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_TOTAL_STAT;

/* Define the union U_ISP_AEWDR1_COUNT_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    count_pixels          : 28  ; /* [27..0]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_COUNT_STAT;

/* Define the union U_ISP_AEWDR1_HIST_HIGH */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_high              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR1_HIST_HIGH;
/* Define the union U_ISP_AEWDR1_BITMOVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bitmove               : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 4   ; /* [8..5]  */
        unsigned int    blc_en                : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_BITMOVE;

/* Define the union U_ISP_AEWDR1_OFFSET_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_r              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_OFFSET_R;

/* Define the union U_ISP_AEWDR1_OFFSET_GR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gr             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_OFFSET_GR;

/* Define the union U_ISP_AEWDR1_OFFSET_GB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_gb             : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_OFFSET_GB;

/* Define the union U_ISP_AEWDR1_OFFSET_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    offset_b              : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_OFFSET_B;

/* Define the union U_ISP_AEWDR1_MEM_HIST_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_waddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR1_MEM_HIST_WADDR;
/* Define the union U_ISP_AEWDR1_MEM_HIST_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_wdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR1_MEM_HIST_WDATA;
/* Define the union U_ISP_AEWDR1_MEM_HIST_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_raddr             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR1_MEM_HIST_RADDR;
/* Define the union U_ISP_AEWDR1_MEM_HIST_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int hist_rdata             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_AEWDR1_MEM_HIST_RDATA;
/* Define the union U_ISP_AEWDR1_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_CTRL_F;

/* Define the union U_ISP_AEWDR1_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_CTRL_I;

/* Define the union U_ISP_AEWDR1_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_UPDATE;

/* Define the union U_ISP_AEWDR1_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsize                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vsize                 : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AEWDR1_SIZE;


/* Define the union U_ISP_BE_VERSION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    customer              : 8   ; /* [7..0]  */
        unsigned int    release               : 8   ; /* [15..8]  */
        unsigned int    build                 : 8   ; /* [23..16]  */
        unsigned int    version               : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_VERSION;

/* Define the union U_ISP_BE_DATE */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int date                   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BE_DATE;
/* Define the union U_ISP_BE_MODULE_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_MODULE_POS;

/* Define the union U_ISP_BE_FSTART_DELAY */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int delay                  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BE_FSTART_DELAY;
/* Define the union U_ISP_BE_USER_DEFINE0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int user_define0           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BE_USER_DEFINE0;
/* Define the union U_ISP_BE_USER_DEFINE1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int user_define1           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BE_USER_DEFINE1;
/* Define the union U_ISP_BE_STARTUP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    startup               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_STARTUP;

/* Define the union U_ISP_BE_INT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fstart                : 1   ; /* [0]  */
        unsigned int    update_cfg            : 1   ; /* [1]  */
        unsigned int    cfg_loss              : 1   ; /* [2]  */
        unsigned int    fstart_delay          : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    acm_para_finish       : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_INT;

/* Define the union U_ISP_BE_INT_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fstart                : 1   ; /* [0]  */
        unsigned int    update_cfg            : 1   ; /* [1]  */
        unsigned int    cfg_loss              : 1   ; /* [2]  */
        unsigned int    int_delay             : 1   ; /* [3]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    acm_para_finish       : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_INT_MASK;

/* Define the union U_ISP_BE_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_CTRL_F;

/* Define the union U_ISP_BE_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_CTRL_I;

/* Define the union U_ISP_BE_TIMING_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    fix_timing            : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_TIMING_CFG;

/* Define the union U_ISP_BE_REG_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_REG_UPDATE;

/* Define the union U_ISP_BE_RESERVED1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_BE_RESERVED1;
/* Define the union U_ISP_CLIP_Y_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    min                   : 16  ; /* [15..0]  */
        unsigned int    max                   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLIP_Y_CFG;

/* Define the union U_ISP_CLIP_C_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    min                   : 16  ; /* [15..0]  */
        unsigned int    max                   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLIP_C_CFG;

/* Define the union U_ISP_SKIP_Y_CFG */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int skip_cfg               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SKIP_Y_CFG;
/* Define the union U_ISP_SKIP_Y_WIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    height                : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SKIP_Y_WIN;

/* Define the union U_ISP_SKIP_C_CFG */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int skip_cfg               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SKIP_C_CFG;
/* Define the union U_ISP_SKIP_C_WIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    height                : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SKIP_C_WIN;

/* Define the union U_ISP_CSC_SUM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_SUM_CFG;

/* Define the union U_ISP_YUV444_SUM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_SUM_CFG;

/* Define the union U_ISP_YUV422_SUM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV422_SUM_CFG;

/* Define the union U_ISP_DMNR_DITHER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    isp_dither_en         : 1   ; /* [0]  */
        unsigned int    isp_dither_round      : 2   ; /* [2..1]  */
        unsigned int    spatial_mode          : 1   ; /* [3]  */
        unsigned int    out_bits              : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DMNR_DITHER;

/* Define the union U_ISP_ACM_DITHER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    isp_dither_en         : 1   ; /* [0]  */
        unsigned int    isp_dither_round      : 2   ; /* [2..1]  */
        unsigned int    spatial_mode          : 1   ; /* [3]  */
        unsigned int    out_bits              : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_DITHER;

/* Define the union U_ISP_CROP_Y_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    n0_en                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_Y_CFG;

/* Define the union U_ISP_CROP_Y_RESERVED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_CROP_Y_RESERVED;
/* Define the union U_ISP_CROP_Y_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x_start               : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    y_start               : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_Y_START;

/* Define the union U_ISP_CROP_Y_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_Y_SIZE;

/* Define the union U_ISP_CROP_C_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    n0_en                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_C_CFG;

/* Define the union U_ISP_CROP_C_RESERVED */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_CROP_C_RESERVED;
/* Define the union U_ISP_CROP_C_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x_start               : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    y_start               : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_C_START;

/* Define the union U_ISP_CROP_C_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_C_SIZE;

/* Define the union U_ISP_Y_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_Y_SUM0;
/* Define the union U_ISP_Y_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_Y_SUM1;
/* Define the union U_ISP_YUV444_Y_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV444_Y_SUM0;
/* Define the union U_ISP_YUV444_Y_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV444_Y_SUM1;
/* Define the union U_ISP_YUV444_U_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV444_U_SUM0;
/* Define the union U_ISP_YUV444_U_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV444_U_SUM1;
/* Define the union U_ISP_YUV444_V_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV444_V_SUM0;
/* Define the union U_ISP_YUV444_V_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV444_V_SUM1;
/* Define the union U_ISP_YUV422_Y_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV422_Y_SUM0;
/* Define the union U_ISP_YUV422_Y_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV422_Y_SUM1;
/* Define the union U_ISP_YUV422_C_SUM0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV422_C_SUM0;
/* Define the union U_ISP_YUV422_C_SUM1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int sum                    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_YUV422_C_SUM1;
/* Define the union U_ISP_SPLIT_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_CFG;

/* Define the union U_ISP_SPLIT_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SPLIT_VERSION;
/* Define the union U_ISP_SPLIT_DATA_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    replace_y_en          : 1   ; /* [0]  */
        unsigned int    replace_u_en          : 1   ; /* [1]  */
        unsigned int    replace_v_en          : 1   ; /* [2]  */
        unsigned int    ex_y_en               : 1   ; /* [3]  */
        unsigned int    ex_u_en               : 1   ; /* [4]  */
        unsigned int    ex_v_en               : 1   ; /* [5]  */
        unsigned int    ex_yuv_en             : 3   ; /* [8..6]  */
        unsigned int    split_en              : 1   ; /* [9]  */
        unsigned int    data_sel              : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    dc_data               : 7   ; /* [22..16]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_CFG;

/* Define the union U_ISP_SPLIT_DATA_VERSION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data_version          : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_VERSION;

/* Define the union U_ISP_SPLIT_DATA_BITWIDTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data_bitw             : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_BITWIDTH;

/* Define the union U_ISP_SPLIT_DATA_AGAIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int data_again             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SPLIT_DATA_AGAIN;
/* Define the union U_ISP_SPLIT_DATA_DGAIN */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int data_dgain             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SPLIT_DATA_DGAIN;
/* Define the union U_ISP_SPLIT_DATA_SHUTTER */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int data_shutter           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SPLIT_DATA_SHUTTER;
/* Define the union U_ISP_SPLIT_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_CTRL_I;

/* Define the union U_ISP_SPLIT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    newer                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_UPDATE;

/* Define the union U_ISP_CC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    prot_en               : 1   ; /* [1]  */
        unsigned int    colortone_en          : 1   ; /* [2]  */
        unsigned int    prot_ext_en           : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_CFG;

/* Define the union U_ISP_CC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_CC_VERSION;
/* Define the union U_ISP_CC_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        //unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef00                : 16  ; /* [15..1]  */
        //unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef01                : 16  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF0;

/* Define the union U_ISP_CC_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        //unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef02                : 16  ; /* [15..1]  */
        //unsigned int    reserved_1            : 1   ; /* [16]  */
       	unsigned int    coef10                : 16  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF1;

/* Define the union U_ISP_CC_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        //unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef11                : 16  ; /* [15..1]  */
        //unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef12                : 16  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF2;

/* Define the union U_ISP_CC_COEF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        //unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef20                : 16  ; /* [15..1]  */
        //unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef21                : 16  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF3;

/* Define the union U_ISP_CC_COEF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        //unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef22                : 16; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    reserved_2            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF4;

/* Define the union U_ISP_CC_BRIT_THRES */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    brit_thres0           : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    brit_thres1           : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_BRIT_THRES;

/* Define the union U_ISP_CC_COLOR_THRES */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    color_thres0          : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    color_thres1          : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COLOR_THRES;

/* Define the union U_ISP_CC_LOW_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    low_ratio             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_LOW_RATIO;

/* Define the union U_ISP_CC_PRO_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    color_ratio           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    brit_ratio            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_PRO_RATIO;

/* Define the union U_ISP_CC_COLORTONE_RB_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    b_gain                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    r_gain                : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COLORTONE_RB_GAIN;

/* Define the union U_ISP_CC_COLORTONE_G_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gain                : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COLORTONE_G_GAIN;

/* Define the union U_ISP_CC_PROT_EX_TH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data_rr_thd0          : 10  ; /* [9..0]  */
        unsigned int    data_gg_thd0          : 10  ; /* [19..10]  */
        unsigned int    data_bb_thd0          : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_PROT_EX_TH0;

/* Define the union U_ISP_CC_PROT_EX_TH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    data_rr_thd1          : 10  ; /* [9..0]  */
        unsigned int    data_gg_thd1          : 10  ; /* [19..10]  */
        unsigned int    data_bb_thd1          : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_PROT_EX_TH1;

/* Define the union U_ISP_CC_PROT_EX_RGB_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb_prot_ration       : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    max_rgb_thd           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_PROT_EX_RGB_SET;

/* Define the union U_ISP_CC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_CTRL_I;

/* Define the union U_ISP_CC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_UPDATE;

/* Define the union U_ISP_CSC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_CFG;

/* Define the union U_ISP_CSC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_CSC_VERSION;
/* Define the union U_ISP_CSC_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef00                : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef01                : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF0;

/* Define the union U_ISP_CSC_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef02                : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef10                : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF1;

/* Define the union U_ISP_CSC_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef11                : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef12                : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF2;

/* Define the union U_ISP_CSC_COEF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef20                : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    coef21                : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF3;

/* Define the union U_ISP_CSC_COEF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    coef22                : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF4;

/* Define the union U_ISP_CSC_IN_DC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    in_dc0                : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    in_dc1                : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_IN_DC0;

/* Define the union U_ISP_CSC_IN_DC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    in_dc2                : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_IN_DC1;

/* Define the union U_ISP_CSC_OUT_DC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    out_dc0               : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    out_dc1               : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_OUT_DC0;

/* Define the union U_ISP_CSC_OUT_DC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    out_dc2               : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_OUT_DC1;

/* Define the union U_ISP_CSC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_CTRL_I;

/* Define the union U_ISP_CSC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_UPDATE;

/* Define the union U_ISP_MCDS_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hcds_en               : 1   ; /* [0]  */
        unsigned int    vcds_en               : 1   ; /* [1]  */
        unsigned int    reserved_0            : 1   ; /* [2]  */
        unsigned int    uv2c_mode             : 1   ; /* [3]  */
        unsigned int    midf_en               : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_CFG;

/* Define the union U_ISP_MCDS_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_MCDS_VERSION;
/* Define the union U_ISP_HCDS_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width_in              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    width_out             : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_SIZE;

/* Define the union U_ISP_HCDS_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    coef0                 : 10  ; /* [9..0]  */
        unsigned int    coef1                 : 10  ; /* [19..10]  */
        unsigned int    coef2                 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEF0;

/* Define the union U_ISP_HCDS_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    coef3                 : 10  ; /* [9..0]  */
        unsigned int    coef4                 : 10  ; /* [19..10]  */
        unsigned int    coef5                 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEF1;

/* Define the union U_ISP_HCDS_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    coef6                 : 10  ; /* [9..0]  */
        unsigned int    coef7                 : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEF2;

/* Define the union U_ISP_VCDS_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    coef0                 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 5   ; /* [7..3]  */
        unsigned int    coef1                 : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    coef2                 : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 5   ; /* [23..19]  */
        unsigned int    coef3                 : 3   ; /* [26..24]  */
        unsigned int    reserved_3            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VCDS_COEF;

/* Define the union U_ISP_MIDF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MIDF_SIZE;

/* Define the union U_ISP_MCDS_PARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    limit                 : 7   ; /* [22..16]  */
        unsigned int    reserved_1            : 1   ; /* [23]  */
        unsigned int    midf_bldr             : 5   ; /* [28..24]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_PARA;

/* Define the union U_ISP_MCDS_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_CTRL_I;

/* Define the union U_ISP_MCDS_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_TIMING;

/* Define the union U_ISP_MCDS_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_UPDATE;

/* Define the union U_ISP_MCDS_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_SIZE;

/* Define the union U_ISP_GCAC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gcac_en               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CFG;

/* Define the union U_ISP_GCAC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GCAC_VERSION;
/* Define the union U_ISP_GCAC_PARAMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        		 int    red_a                 : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        		 int    blue_a                : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_PARAMA;

/* Define the union U_ISP_GCAC_PARAMB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        		 int    red_b                 : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        		 int    blue_b                : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_PARAMB;

/* Define the union U_ISP_GCAC_PARAMC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        		 int    red_c                 : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        		 int    blue_c                : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_PARAMC;

/* Define the union U_ISP_GCAC_YNORM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nf_y                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    ns_y                  : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_YNORM;

/* Define the union U_ISP_GCAC_XNORM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    nf_x                  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    ns_x                  : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_XNORM;

/* Define the union U_ISP_GCAC_CNTSTART */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cnt_start_h           : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    cnt_start_v           : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CNTSTART;

/* Define the union U_ISP_GCAC_CORSTART */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cor_start_h           : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    cor_start_v           : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CORSTART;

/* Define the union U_ISP_GCAC_FILTEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ver_filt_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_FILTEN;

/* Define the union U_ISP_GCAC_CHRVMODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chrv_mode             : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CHRVMODE;

/* Define the union U_ISP_GCAC_CLIPMODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_mode_h           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    clip_mode_v           : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CLIPMODE;

/* Define the union U_ISP_GCAC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CTRL_F;

/* Define the union U_ISP_GCAC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CTRL_I;

/* Define the union U_ISP_GCAC_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_TIMING;

/* Define the union U_ISP_GCAC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_UPDATE;

/* Define the union U_ISP_GCAC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_SIZE;

/* Define the union U_ISP_DEMOSAIC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CFG;

/* Define the union U_ISP_DEMOSAIC_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ahd_en                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    fcr_en                : 1   ; /* [16]  */
        unsigned int    local_cac_en          : 1   ; /* [17]  */
        unsigned int    gcac_blend_select     : 1   ; /* [18]  */
        unsigned int    hv_dir_en             : 1   ; /* [19]  */
        unsigned int    mid_filter_en         : 1   ; /* [20]  */
        unsigned int    de_fake_en            : 1   ; /* [21]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CFG1;

/* Define the union U_ISP_DEMOSAIC_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DEMOSAIC_VERSION;
/* Define the union U_ISP_DEMOSAIC_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bld_limit2            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    bld_limit1            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF0;

/* Define the union U_ISP_DEMOSAIC_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bld_ratio2            : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    bld_ratio1            : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF1;

/* Define the union U_ISP_DEMOSAIC_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_limit2            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    fcr_limit1            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF2;

/* Define the union U_ISP_DEMOSAIC_COEF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ahd_par2              : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    ahd_par1              : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF3;

/* Define the union U_ISP_DEMOSAIC_COEF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_gain              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    fcr_thr               : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF4;

/* Define the union U_ISP_DEMOSAIC_COEF5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_ratio              : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF5;

/* Define the union U_ISP_DEMOSAIC_COEF6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_ratio             : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF6;

/* Define the union U_ISP_DEMOSAIC_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hv_sel                : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SEL;

/* Define the union U_ISP_DEMOSAIC_TMP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tmplate               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_TMP;

/* Define the union U_ISP_DEMOSAIC_LCAC_CNT_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    r_counter_thr         : 5   ; /* [4..0]  */
        unsigned int    b_counter_thr         : 5   ; /* [9..5]  */
        unsigned int    g_counter_thr         : 6   ; /* [15..10]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LCAC_CNT_THR;

/* Define the union U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    r_luma_thr            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    b_luma_thr            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR;

/* Define the union U_ISP_DEMOSAIC_LCAC_LUMA_G_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_luma_thr            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LCAC_LUMA_G_THR;

/* Define the union U_ISP_DEMOSAIC_PURPLE_VAR_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    purple_var_thr        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_PURPLE_VAR_THR;

/* Define the union U_ISP_DEMOSAIC_FAKE_CR_VAR_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fake_cr_var_thr_low   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    fake_cr_var_thr_high  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FAKE_CR_VAR_THR;

/* Define the union U_ISP_DEMOSAIC_DEPURPLECTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    depurplectrcb         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    depurplectrcr         : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_DEPURPLECTR;

/* Define the union U_ISP_DEMOSAIC_HV_POINT_THLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sig_point_thld        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    tot_point_thld        : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HV_POINT_THLD;

/* Define the union U_ISP_DEMOSAIC_BLD_ADJUST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bld_adjust            : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_BLD_ADJUST;

/* Define the union U_ISP_DEMOSAIC_CBCRAVGTHLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cbcr_avg_thld         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CBCRAVGTHLD;

/* Define the union U_ISP_DEMOSAIC_GAPTHLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cr_gap_thld           : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    g_gap_thld            : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_GAPTHLD;

/* Define the union U_ISP_DEMOSAIC_COLORTHLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorthld             : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COLORTHLD;

/* Define the union U_ISP_DEMOSAIC_GVAR_BLEND_THLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    varthrforblend        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_GVAR_BLEND_THLD;

/* Define the union U_ISP_DEMOSAIC_SATU_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    satu_thr              : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SATU_THR;

/* Define the union U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
         		 int    cbcr_ratio_low_limit  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
         		 int    cbcr_ratio_high_limit : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT;

/* Define the union U_ISP_DEMOSAIC_FCR_GRAY_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_gray_ratio        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FCR_GRAY_RATIO;

/* Define the union U_ISP_DEMOSAIC_FCR_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_detg_sel          : 4   ; /* [3..0]  */
        unsigned int    fcr_cmax_sel          : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FCR_SEL;

/* Define the union U_ISP_DEMOSAIC_BLEND_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dif_blend_ratio0      : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    dif_blend_ratio1      : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    var_blend_th          : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_BLEND_RATIO;

/* Define the union U_ISP_DEMOSAIC_BLEND_DIF_LIMIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dif_blend_limit0      : 8   ; /* [7..0]  */
        unsigned int    dif_blend_limit1      : 8   ; /* [15..8]  */
        unsigned int    dif2max_res_th        : 10  ; /* [25..16]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_BLEND_DIF_LIMIT;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hf_intp_th_low        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    hf_intp_th_high       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_TH;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_BLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hf_intp_bld_low       : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    hf_intp_bld_high      : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_BLD;


/* Define the union U_ISP_DEMOSAIC_HF_INTP_TH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hf_intp_th_low1       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    hf_intp_th_high1      : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_TH1;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_RATIO0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    interp_ratio          : 17  ; /* [16..0]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_RATIO0;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_RATIO1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    interp_ratio          : 17  ; /* [16..0]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_RATIO1;


/* Define the union U_ISP_DEMOSAIC_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CTRL_F;

/* Define the union U_ISP_DEMOSAIC_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CTRL_I;

/* Define the union U_ISP_DEMOSAIC_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_TIMING;

/* Define the union U_ISP_DEMOSAIC_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_UPDATE;

/* Define the union U_ISP_DEMOSAIC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SIZE;

/* Define the union U_ISP_SHARPEN_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CFG;

/* Define the union U_ISP_SHARPEN_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_SHARPEN_VERSION;
/* Define the union U_ISP_SHARPEN_AMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sharp_amt             : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    edge_amt              : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_AMT;

/* Define the union U_ISP_SHARPEN_THD1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sharp_thd1            : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    edge_thd1             : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_THD1;

/* Define the union U_ISP_SHARPEN_THD2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sharp_thd2            : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    edge_thd2             : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_THD2;

/* Define the union U_ISP_SHARPEN_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sharp_coef            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    edge_coef             : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_COEF;

/* Define the union U_ISP_SHARPEN_SHOOTAMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    under_amt             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    over_amt              : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHOOTAMT;

/* Define the union U_ISP_SHARPEN_JAGTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    varjagthd1            : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    varjagthd2            : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_JAGTHD;

/* Define the union U_ISP_SHARPEN_EDGEJAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    edgejagamt            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    edgejagmulcoef        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_EDGEJAG;

/* Define the union U_ISP_SHARPEN_OSHTJAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    oshtjagamt            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    oshtjagmulcoef        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSHTJAG;

/* Define the union U_ISP_SHARPEN_USHTJAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ushtjagamt            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    ushtjagmulcoef        : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USHTJAG;

/* Define the union U_ISP_SHARPEN_MID0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mid_tmp00             : 8   ; /* [7..0]  */
        unsigned int    mid_tmp01             : 8   ; /* [15..8]  */
        unsigned int    mid_tmp02             : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MID0;

/* Define the union U_ISP_SHARPEN_MID1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mid_tmp10             : 8   ; /* [7..0]  */
        unsigned int    mid_tmp11             : 8   ; /* [15..8]  */
        unsigned int    mid_tmp12             : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MID1;

/* Define the union U_ISP_SHARPEN_MID2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mid_tmp20             : 8   ; /* [7..0]  */
        unsigned int    mid_tmp21             : 8   ; /* [15..8]  */
        unsigned int    mid_tmp22             : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MID2;

/* Define the union U_ISP_SHARPEN_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinctrl              : 1   ; /* [0]  */
        unsigned int    lumactrl              : 1   ; /* [1]  */
        unsigned int    shtvarctrl            : 1   ; /* [2]  */
        unsigned int    lumactrlnoise         : 1   ; /* [3]  */
        unsigned int    jagctrl               : 1   ; /* [4]  */
        unsigned int    shadsup_en            : 1   ; /* [5]  */
        unsigned int    rgbshp_en             : 1   ; /* [6]  */
        unsigned int    lowbandesm_en         : 1   ; /* [7]  */
        unsigned int    pixsel                : 5   ; /* [12..8]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CTRL;

/* Define the union U_ISP_SHARPEN_SHIFT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    minshift              : 8   ; /* [7..0]  */
        unsigned int    maxshift              : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHIFT;

/* Define the union U_ISP_SHARPEN_ST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_limit            : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    luma_nost             : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    dirrlyshft            : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_ST;

/* Define the union U_ISP_SHARPEN_OSHTVARTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    overvarth0            : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    overvarth1            : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSHTVARTH;

/* Define the union U_ISP_SHARPEN_OSHTVAR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    overvaramt            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    overvarmul            : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSHTVAR;

/* Define the union U_ISP_SHARPEN_USHTVARTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    undervarth0           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    undervarth1           : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USHTVARTH;

/* Define the union U_ISP_SHARPEN_USHTVAR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    undervaramt           : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    undervarmul           : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USHTVAR;

/* Define the union U_ISP_SHARPEN_LUMAWGT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt0              : 8   ; /* [7..0]  */
        unsigned int    lumawgt1              : 8   ; /* [15..8]  */
        unsigned int    lumawgt2              : 8   ; /* [23..16]  */
        unsigned int    lumawgt3              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT;

/* Define the union U_ISP_SHARPEN_LUMAWGT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt4              : 8   ; /* [7..0]  */
        unsigned int    lumawgt5              : 8   ; /* [15..8]  */
        unsigned int    lumawgt6              : 8   ; /* [23..16]  */
        unsigned int    lumawgt7              : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT1;

/* Define the union U_ISP_SHARPEN_LUMAWGT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt8              : 8   ; /* [7..0]  */
        unsigned int    lumawgt9              : 8   ; /* [15..8]  */
        unsigned int    lumawgt10             : 8   ; /* [23..16]  */
        unsigned int    lumawgt11             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT2;

/* Define the union U_ISP_SHARPEN_LUMAWGT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt12             : 8   ; /* [7..0]  */
        unsigned int    lumawgt13             : 8   ; /* [15..8]  */
        unsigned int    lumawgt14             : 8   ; /* [23..16]  */
        unsigned int    lumawgt15             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT3;

/* Define the union U_ISP_SHARPEN_LUMAWGT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt16             : 8   ; /* [7..0]  */
        unsigned int    lumawgt17             : 8   ; /* [15..8]  */
        unsigned int    lumawgt18             : 8   ; /* [23..16]  */
        unsigned int    lumawgt19             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT4;

/* Define the union U_ISP_SHARPEN_LUMAWGT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt20             : 8   ; /* [7..0]  */
        unsigned int    lumawgt21             : 8   ; /* [15..8]  */
        unsigned int    lumawgt22             : 8   ; /* [23..16]  */
        unsigned int    lumawgt23             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT5;

/* Define the union U_ISP_SHARPEN_LUMAWGT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt24             : 8   ; /* [7..0]  */
        unsigned int    lumawgt25             : 8   ; /* [15..8]  */
        unsigned int    lumawgt26             : 8   ; /* [23..16]  */
        unsigned int    lumawgt27             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT6;

/* Define the union U_ISP_SHARPEN_LUMAWGT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumawgt28             : 8   ; /* [7..0]  */
        unsigned int    lumawgt29             : 8   ; /* [15..8]  */
        unsigned int    lumawgt30             : 8   ; /* [23..16]  */
        unsigned int    lumawgt31             : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT7;

/* Define the union U_ISP_SHARPEN_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CTRL_I;

/* Define the union U_ISP_SHARPEN_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_TIMING;

/* Define the union U_ISP_SHARPEN_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_UPDATE;

/* Define the union U_ISP_SHARPEN_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SIZE;

/* Define the union U_ISP_SHARPEN_SKIN_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_cnt_th1          : 4   ; /* [3..0]  */
        unsigned int    skin_cnt_th2          : 4   ; /* [7..4]  */
        unsigned int    skincntmulcoef        : 5   ; /* [12..8]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_CNT;

/* Define the union U_ISP_SHARPEN_SKIN_VARTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_var_th1          : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skin_var_th2          : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_VARTH;

/* Define the union U_ISP_SHARPEN_SKIN_VARWGT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_var_wgt1         : 8   ; /* [7..0]  */
        unsigned int    skin_var_wgt2         : 8   ; /* [15..8]  */
        unsigned int    skinvarwgtmulcoef     : 13  ; /* [28..16]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_VARWGT;

/* Define the union U_ISP_SHARPEN_SKIN_U */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_u_min            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    skin_u_max            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_U;

/* Define the union U_ISP_SHARPEN_SKIN_V */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_v_min            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    skin_v_max            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_V;

/* Define the union U_ISP_SHARPEN_SKIN_LEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_len              : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_LEN;

/* Define the union U_ISP_SHARPEN_EDGSM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    diff_mul              : 4   ; /* [3..0]  */
        unsigned int    diff_thd              : 8   ; /* [11..4]  */
        unsigned int    edgsm_str             : 5   ; /* [16..12]  */
        unsigned int    reserved_0            : 7   ; /* [23..17]  */
        unsigned int    edgsm_en              : 1   ; /* [24]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_EDGSM;

/* Define the union U_ISP_SHARPEN_SHPGAIN_MUL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bshpgainmul           : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    rshpgainmul           : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHPGAIN_MUL;

/* Define the union U_ISP_SHARPEN_OSUP_VAR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    oshtvarmin            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    oshtvarmax            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSUP_VAR;

/* Define the union U_ISP_SHARPEN_OSUP_VARCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    oshtsupmul            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    oshtwgtmin            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSUP_VARCOEF;

/* Define the union U_ISP_SHARPEN_USUP_VAR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ushtvarmin            : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    ushtvarmax            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USUP_VAR;

/* Define the union U_ISP_SHARPEN_USUP_VARCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ushtsupmul            : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    ushtwgtmin            : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USUP_VARCOEF;

/* Define the union U_ISP_SHARPEN_OSUP_DIFF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    oshtdiffmin           : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    oshtdiffmax           : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSUP_DIFF;

/* Define the union U_ISP_SHARPEN_OSUP_DIFFCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    oshtsupdiffmul        : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    oshtdiffwgtmin        : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSUP_DIFFCOEF;

/* Define the union U_ISP_SHARPEN_USUP_DIFF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ushtdiffmin           : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    ushtdiffmax           : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USUP_DIFF;

/* Define the union U_ISP_SHARPEN_USUP_DIFFCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ushtsupdiffmul        : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    ushtdiffwgtmin        : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USUP_DIFFCOEF;

/* Define the union U_ISP_SHARPEN_SUP_NOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    minnosthd             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    maxnosthd             : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SUP_NOS;

/* Define the union U_ISP_SHARPEN_SUP_BLDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    shtsupbldr            : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    shtsup_en             : 1   ; /* [16]  */
        unsigned int    shtmax_en             : 1   ; /* [17]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SUP_BLDR;

/* Define the union U_ISP_SHARPEN_UDHFGAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    udhfgain              : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_UDHFGAIN;

/* Define the union U_ISP_SHARPEN_SHTMAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    oshtmaxvargain        : 8   ; /* [7..0]  */
        unsigned int    ushtmaxvargain        : 8   ; /* [15..8]  */
        unsigned int    oshtmaxchg            : 8   ; /* [23..16]  */
        unsigned int    ushtmaxchg            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHTMAX;

/* Define the union U_ISP_SHARPEN_SHADVAR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    shadvar1              : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    shadvar2              : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHADVAR;

/* Define the union U_ISP_SHARPEN_GRADTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gradthd1              : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    gradthd2              : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_GRADTHD;

/* Define the union U_ISP_SHARPEN_WGTCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgtthd1               : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    wgtthd2               : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    wgtmul                : 8   ; /* [23..16]  */
        unsigned int    reserved_2            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_WGTCOEF;

/* Define the union U_ISP_SHARPEN_RC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rcbmax                : 8   ; /* [7..0]  */
        unsigned int    rcrmax                : 8   ; /* [15..8]  */
        unsigned int    rcbmin                : 8   ; /* [23..16]  */
        unsigned int    rcrmin                : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_RC;

/* Define the union U_ISP_SHARPEN_BC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bcbmax                : 8   ; /* [7..0]  */
        unsigned int    bcrmax                : 8   ; /* [15..8]  */
        unsigned int    bcbmin                : 8   ; /* [23..16]  */
        unsigned int    bcrmin                : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_BC;

/* Define the union U_ISP_SHARPEN_GC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gcbmax                : 8   ; /* [7..0]  */
        unsigned int    gcrmax                : 8   ; /* [15..8]  */
        unsigned int    gcbmin                : 8   ; /* [23..16]  */
        unsigned int    gcrmin                : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_GC;

/* Define the union U_ISP_SHARPEN_RGBWGT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gwgt                  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    bwgt                  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    rwgt                  : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_RGBWGT;

/* Define the union U_ISP_SHARPEN_RBCOEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bshpgainsft           : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    rshpgainsft           : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    bcbthd                : 8   ; /* [23..16]  */
        unsigned int    rcrthd                : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_RBCOEF;

/* Define the union U_ISP_SHARPEN_SKIN_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_CTRL_I;

/* Define the union U_ISP_SHARPEN_SKIN_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_UPDATE;

/* Define the union U_ISP_NDDM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    cac_blend_en          : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_CFG;

/* Define the union U_ISP_NDDM_GF_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gf_th_low             : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    gf_th_high            : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_TH;

/* Define the union U_ISP_NDDM_GF_BLDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bldr_gf_str           : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    bldr_var_str          : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    bldr_cbcr             : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_BLDR;

/* Define the union U_ISP_NDDM_GF_CHRM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chrm_low              : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    chrm_high             : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    chrm_fix              : 4   ; /* [23..20]  */
        unsigned int    reserved_2            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_CHRM;

/* Define the union U_ISP_NDDM_USM_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_usm              : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    multi_mf              : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_CFG;

/* Define the union U_ISP_NDDM_USM_SATU */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    satu_th_low           : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    satu_th_high          : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    satu_th_fix           : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_SATU;

/* Define the union U_ISP_NDDM_USM_GRAY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gray_th_low           : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    gray_th_high          : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    gray_th_fix           : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_GRAY;

/* Define the union U_ISP_NDDM_APT_INTP_BLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bldr_gr_gb            : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_INTP_BLD;

/* Define the union U_ISP_NDDM_DITHER_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dith_mask             : 8   ; /* [7..0]  */
        unsigned int    dith_ratio            : 8   ; /* [15..8]  */
        unsigned int    dith_max              : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_DITHER_CFG;

/* Define the union U_ISP_NDDM_FCR_LIMIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_limit_low         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    fcr_limit_high        : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_FCR_LIMIT;

/* Define the union U_ISP_NDDM_FCR_GAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fcr_gf_gain           : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    fcr_det_low           : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_FCR_GAIN;

/* Define the union U_ISP_NDDM_FCR_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scale                 : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_FCR_SCALE;

/* Define the union U_ISP_NDDM_DM_BLDRATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cac_blend_rate        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_DM_BLDRATE;

/* Define the union U_ISP_NDDM_GF_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_UPDATE;

/* Define the union U_ISP_NDDM_USM_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_UPDATE;

/* Define the union U_ISP_NDDM_USM_GRAY_TH_FIX2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gray_th_fix2          : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_GRAY_TH_FIX2;

/* Define the union U_ISP_NDDM_APT_INTP_CLIP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_adjust_max       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    clip_delta_gain       : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_INTP_CLIP2;

/* Define the union U_ISP_NDDM_APT_AVG_CFG_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_delta_intp_low   : 12  ; /* [11..0]  */
        unsigned int    clip_delta_intp_high  : 12  ; /* [23..12]  */
        unsigned int    filter_str_intp       : 5   ; /* [28..24]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_AVG_CFG_0;

/* Define the union U_ISP_NDDM_APT_AVG_CFG_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_delta_filt_low   : 12  ; /* [11..0]  */
        unsigned int    clip_delta_filt_high  : 12  ; /* [23..12]  */
        unsigned int    filter_str_filt       : 5   ; /* [28..24]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_AVG_CFG_1;

/* Define the union U_ISP_NDDM_VAR_OFFSET_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    var_offset_gain       : 8   ; /* [7..0]  */
        unsigned int    var_offset_low        : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_VAR_OFFSET_CFG;

/* Define the union U_ISP_NDDM_SHT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sht_ctrl_gain         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    sht_ctrl_th           : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SHT_CTRL;

/* Define the union U_ISP_NDDM_SHT_CLIP_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_r_ud_sht         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    clip_r_ov_sht         : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SHT_CLIP_R;

/* Define the union U_ISP_NDDM_SHT_CLIP_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_b_ud_sht         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    clip_b_ov_sht         : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SHT_CLIP_B;

/* Define the union U_ISP_NDDM_MULTI_MF_RB */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    multi_mf_r            : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    multi_mf_b            : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_MULTI_MF_RB;

/* Define the union U_ISP_NDDM_USM_SATU_R */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    satu_r_th_low         : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    satu_r_th_high        : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    satu_r_th_fix         : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_SATU_R;

/* Define the union U_ISP_NDDM_USM_SATU_B */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    satu_b_th_low         : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    satu_b_th_high        : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    satu_b_th_fix         : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_SATU_B;

/* Define the union U_ISP_NDDM_SATU_FIX_EHCY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    satu_fix_ehcy         : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SATU_FIX_EHCY;

/* Define the union U_ISP_NDDM_GF_LUT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gf_lut_waddr          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_WADDR;

/* Define the union U_ISP_NDDM_GF_LUT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gf_lut_wdata          : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_WDATA;

/* Define the union U_ISP_NDDM_GF_LUT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gf_lut_raddr          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_RADDR;

/* Define the union U_ISP_NDDM_GF_LUT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gf_lut_rdata          : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_RDATA;

/* Define the union U_ISP_NDDM_USM_LUT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usm_lut_waddr         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_WADDR;

/* Define the union U_ISP_NDDM_USM_LUT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usm_lut_wdata         : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_WDATA;

/* Define the union U_ISP_NDDM_USM_LUT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usm_lut_raddr         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_RADDR;

/* Define the union U_ISP_NDDM_USM_LUT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    usm_lut_rdata         : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_RDATA;

/* Define the union U_ISP_NDDM_CTRL_F */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rggb_cfg              : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_CTRL_F;

/* Define the union U_ISP_NDDM_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_CTRL_I;

/* Define the union U_ISP_NDDM_TIMING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    cpi_fix_timing_stat   : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_TIMING;

/* Define the union U_ISP_NDDM_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_UPDATE;

/* Define the union U_ISP_NDDM_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SIZE;

/* Define the union U_ISP_DEHAZE_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_CFG;

/* Define the union U_ISP_DEHAZE_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_DEHAZE_VERSION;
/* Define the union U_ISP_DEHAZE_PRE_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pre_update            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRE_UPDATE;

/* Define the union U_ISP_DEHAZE_BLK_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    block_sizev           : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    block_sizeh           : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_BLK_SIZE;

/* Define the union U_ISP_DEHAZE_BLK_SUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    block_sum             : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_BLK_SUM;

/* Define the union U_ISP_DEHAZE_DC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dc_numv               : 5   ; /* [4..0]  */
        unsigned int    dc_numh               : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_DC_SIZE;

/* Define the union U_ISP_DEHAZE_X */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phase_x               : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_X;

/* Define the union U_ISP_DEHAZE_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phase_y               : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_Y;

/* Define the union U_ISP_DEHAZE_STAT_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    max_mode              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_STAT_MODE;

/* Define the union U_ISP_DEHAZE_NEG_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    neg_mode              : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_NEG_MODE;

/* Define the union U_ISP_DEHAZE_AIR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    air_b                 : 10  ; /* [9..0]  */
        unsigned int    air_g                 : 10  ; /* [19..10]  */
        unsigned int    air_r                 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_AIR;

/* Define the union U_ISP_DEHAZE_THLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    thld                  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_THLD;

/* Define the union U_ISP_DEHAZE_GSTRTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gstrth                : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_GSTRTH;

/* Define the union U_ISP_DEHAZE_BLTHLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    blthld                : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_BLTHLD;

/* Define the union U_ISP_DEHAZE_STR_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    str_lut_update        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_STR_LUT_UPDATE;

/* Define the union U_ISP_DEHAZE_MINSTAT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    minstat_waddr         : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_WADDR;

/* Define the union U_ISP_DEHAZE_MINSTAT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    minstat_wdata_l       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    minstat_wdata_h       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_WDATA;

/* Define the union U_ISP_DEHAZE_MINSTAT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    minstat_raddr         : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_RADDR;

/* Define the union U_ISP_DEHAZE_MINSTAT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    minstat_rdata_l       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    minstat_rdata_h       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_RDATA;

/* Define the union U_ISP_DEHAZE_MAXSTAT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    maxstat_waddr         : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_WADDR;

/* Define the union U_ISP_DEHAZE_MAXSTAT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    maxstat_wdata         : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_WDATA;

/* Define the union U_ISP_DEHAZE_MAXSTAT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    maxstat_raddr         : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_RADDR;

/* Define the union U_ISP_DEHAZE_MAXSTAT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    maxstat_rdata         : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_RDATA;

/* Define the union U_ISP_DEHAZE_PRESTAT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prestat_waddr         : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_WADDR;

/* Define the union U_ISP_DEHAZE_PRESTAT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prestat_wdata_l       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    prestat_wdata_h       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_WDATA;

/* Define the union U_ISP_DEHAZE_PRESTAT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prestat_raddr         : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_RADDR;

/* Define the union U_ISP_DEHAZE_PRESTAT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prestat_rdata_l       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    prestat_rdata_h       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_RDATA;

/* Define the union U_ISP_DEHAZE_LUT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_waddr             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_WADDR;

/* Define the union U_ISP_DEHAZE_LUT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_wdata             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_WDATA;

/* Define the union U_ISP_DEHAZE_LUT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_raddr             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_RADDR;

/* Define the union U_ISP_DEHAZE_LUT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_rdata             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_RDATA;

/* Define the union U_ISP_DEHAZE_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_CTRL_I;

/* Define the union U_ISP_DEHAZE_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    newer                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_UPDATE;

/* Define the union U_ISP_DEHAZE_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_SIZE;

/* Define the union U_ISP_GAMMA_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_CFG;

/* Define the union U_ISP_GAMMA_VERSION */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int version                : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GAMMA_VERSION;
/* Define the union U_ISP_GAMMA_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT_UPDATE;

/* Define the union U_ISP_GAMMA_WADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gamma_waddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GAMMA_WADDR;
/* Define the union U_ISP_GAMMA_WDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gamma_wdata            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GAMMA_WDATA;
/* Define the union U_ISP_GAMMA_RADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gamma_raddr            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GAMMA_RADDR;
/* Define the union U_ISP_GAMMA_RDATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gamma_rdata            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_GAMMA_RDATA;
/* Define the union U_ISP_GAMMA_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_CTRL_I;

/* Define the union U_ISP_GAMMA_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_UPDATE;

/* Define the union U_ISP_CA_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    en                    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_CFG;

/* Define the union U_ISP_CA_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    satadj_en             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    colorproc_en          : 1   ; /* [8]  */
        unsigned int    reserved_1            : 7   ; /* [15..9]  */
        unsigned int    skinproc_en           : 1   ; /* [16]  */
        unsigned int    reserved_2            : 7   ; /* [23..17]  */
        unsigned int    llhcproc_en           : 1   ; /* [24]  */
        unsigned int    reserved_3            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_CTRL;

/* Define the union U_ISP_CA_LUMATH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumath_high           : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    lumath_low            : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUMATH;

/* Define the union U_ISP_CA_DARKCHROMA_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    darkchromath_high     : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    darkchromath_low      : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_DARKCHROMA_TH;

/* Define the union U_ISP_CA_SDARKCHROMA_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sdarkchromath_high    : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    sdarkchromath_low     : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SDARKCHROMA_TH;

/* Define the union U_ISP_CA_LLHC_RATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lumaratio_high        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    lumaratio_low         : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LLHC_RATIO;

/* Define the union U_ISP_CA_ISORATIO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    isoratio              : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_ISORATIO;

/* Define the union U_ISP_CA_LUT_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lut_update            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT_UPDATE;

/* Define the union U_ISP_CA_YUV2RGB_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_coef01        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    yuv2rgb_coef00        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF0;

/* Define the union U_ISP_CA_YUV2RGB_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_coef10        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    yuv2rgb_coef02        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF1;

/* Define the union U_ISP_CA_YUV2RGB_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_coef12        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    yuv2rgb_coef11        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF2;

/* Define the union U_ISP_CA_YUV2RGB_COEF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_coef21        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    yuv2rgb_coef20        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF3;

/* Define the union U_ISP_CA_YUV2RGB_COEF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_coef22        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF4;

/* Define the union U_ISP_CA_YUV2RGB_INDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_indc1         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    yuv2rgb_indc0         : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_INDC0;

/* Define the union U_ISP_CA_YUV2RGB_INDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_indc2         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_INDC1;

/* Define the union U_ISP_CA_YUV2RGB_OUTDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_outdc1        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    yuv2rgb_outdc0        : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_OUTDC0;

/* Define the union U_ISP_CA_YUV2RGB_OUTDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    yuv2rgb_outdc2        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_OUTDC1;

/* Define the union U_ISP_CA_RGB2YUV_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_coef01        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_coef00        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_COEF0;

/* Define the union U_ISP_CA_RGB2YUV_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_coef10        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_coef02        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_COEF1;

/* Define the union U_ISP_CA_RGB2YUV_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_coef12        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_coef11        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_COEF2;

/* Define the union U_ISP_CA_RGB2YUV_COEF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_coef21        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_coef20        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_COEF3;

/* Define the union U_ISP_CA_RGB2YUV_COEF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_coef22        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_COEF4;

/* Define the union U_ISP_CA_RGB2YUV_INDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_indc1         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    rgb2yuv_indc0         : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_INDC0;

/* Define the union U_ISP_CA_RGB2YUV_INDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_indc2         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_INDC1;

/* Define the union U_ISP_CA_RGB2YUV_OUTDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_outdc1        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    rgb2yuv_outdc0        : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_OUTDC0;

/* Define the union U_ISP_CA_RGB2YUV_OUTDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_outdc2        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_RGB2YUV_OUTDC1;

/* Define the union U_ISP_CA_SKINLLUMA_UTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinlluma_umax        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinlluma_umin        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_UTH;

/* Define the union U_ISP_CA_SKINLLUMA_UYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinlluma_uymax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinlluma_uymin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_UYTH;

/* Define the union U_ISP_CA_SKINHLUMA_UTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinhluma_umax        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinhluma_umin        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_UTH;

/* Define the union U_ISP_CA_GAINLUT_WADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gainlut_waddr         : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_GAINLUT_WADDR;

/* Define the union U_ISP_CA_GAINLUT_WDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gainlut_wdata         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_GAINLUT_WDATA;

/* Define the union U_ISP_CA_GAINLUT_RADDR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gainlut_raddr         : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_GAINLUT_RADDR;

/* Define the union U_ISP_CA_GAINLUT_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gainlut_rdata         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_GAINLUT_RDATA;

/* Define the union U_ISP_CA_SKINHLUMA_UYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinhluma_uymax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinhluma_uymin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_UYTH;

/* Define the union U_ISP_CA_SKINLLUMA_VTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinlluma_vmax        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinlluma_vmin        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_VTH;

/* Define the union U_ISP_CA_SKINLLUMA_VYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinlluma_vymax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinlluma_vymin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_VYTH;

/* Define the union U_ISP_CA_SKINHLUMA_VTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinhluma_vmax        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinhluma_vmin        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_VTH;

/* Define the union U_ISP_CA_SKINHLUMA_VYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinhluma_vymax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    skinhluma_vymin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_VYTH;

/* Define the union U_ISP_CA_SKIN_UVDIFF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skin_uvdiff           : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKIN_UVDIFF;

/* Define the union U_ISP_CA_SKIN_RATIOTH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinratioth_mid       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    skinratioth_low       : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKIN_RATIOTH0;

/* Define the union U_ISP_CA_SKIN_RATIOTH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    skinratioth_high      : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKIN_RATIOTH1;

/* Define the union U_ISP_CA_COLORLLUMA_UTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorlluma_umax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorlluma_umin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORLLUMA_UTH;

/* Define the union U_ISP_CA_COLORLLUMA_UYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorlluma_uymax      : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorlluma_uymin      : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORLLUMA_UYTH;

/* Define the union U_ISP_CA_COLORHLUMA_UTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorhluma_umax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorhluma_umin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORHLUMA_UTH;

/* Define the union U_ISP_CA_COLORHLUMA_UYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorhluma_uymax      : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorhluma_uymin      : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORHLUMA_UYTH;

/* Define the union U_ISP_CA_COLORLLUMA_VTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorlluma_vmax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorlluma_vmin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORLLUMA_VTH;

/* Define the union U_ISP_CA_COLORLLUMA_VYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorlluma_vymax      : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorlluma_vymin      : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORLLUMA_VYTH;

/* Define the union U_ISP_CA_COLORHLUMA_VTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorhluma_vmax       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorhluma_vmin       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORHLUMA_VTH;

/* Define the union U_ISP_CA_COLORHLUMA_VYTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorhluma_vymax      : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    colorhluma_vymin      : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLORHLUMA_VYTH;

/* Define the union U_ISP_CA_COLOR_UVDIFF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    color_uvdiff          : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLOR_UVDIFF;

/* Define the union U_ISP_CA_COLOR_RATIOTH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorratioth_mid      : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    colorratioth_low      : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLOR_RATIOTH0;

/* Define the union U_ISP_CA_COLOR_RATIOTH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorratioth_high     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_COLOR_RATIOTH1;

/* Define the union U_ISP_CA_CTRL_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update_mode           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_CTRL_I;

/* Define the union U_ISP_CA_UPDATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    update                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_UPDATE;

/* Define the union U_ISP_CA_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SIZE;

/* Define the union U_ISP_HLDC_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hldc_in_b422          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 29  ; /* [29..1]  */
        unsigned int    hldc_c_en             : 1   ; /* [30]  */
        unsigned int    hldc_l_en             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HLDC_CFG;

/* Define the union U_ISP_HLDC_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HLDC_SIZE;

/* Define the union U_ISP_HLDC_CENTER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ldc_x_center          : 12  ; /* [11..0]  */
        unsigned int    ldc_y_center          : 12  ; /* [23..12]  */
        unsigned int    ldc_norcoef           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HLDC_CENTER;

/* Define the union U_ISP_HLDC_PARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    scale                 : 7   ; /* [6..0]  */
        unsigned int    ratio                 : 9   ; /* [15..7]  */
        unsigned int    shift_right           : 5   ; /* [20..16]  */
        unsigned int    ldc_black_mode        : 1   ; /* [21]  */
        unsigned int    ldc_barrel_mode       : 1   ; /* [22]  */
        unsigned int    ldc_mode              : 1   ; /* [23]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HLDC_PARA;

/* Define the union U_ISP_ACM_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acm_cbcrthr           : 9   ; /* [8..0]  */
        unsigned int    acm_dbg_mode          : 1   ; /* [9]  */
        unsigned int    acm_dbg_pos           : 13  ; /* [22..10]  */
        unsigned int    reserved_0            : 4   ; /* [26..23]  */
        unsigned int    acm_cliporwrap        : 1   ; /* [27]  */
        unsigned int    acm_cliprange         : 1   ; /* [28]  */
        unsigned int    acm_stretch           : 1   ; /* [29]  */
        unsigned int    acm_dbg_en            : 1   ; /* [30]  */
        unsigned int    acm_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_CTRL;

/* Define the union U_ISP_ACM_ADJ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acm_gain2             : 10  ; /* [9..0]  */
        unsigned int    acm_gain1             : 10  ; /* [19..10]  */
        unsigned int    acm_gain0             : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_ADJ;

/* Define the union U_ISP_ACM_PARA_REN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acm_para_rd_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_PARA_REN;

/* Define the union U_ISP_ACM_PARA_DATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int acm_para_data          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_ACM_PARA_DATA;
/* Define the union U_ISP_ACM_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    width                 : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    height                : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_SIZE;

/* Define the union U_ISP_ACM_PARA_ADDR */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int acm_para_addr          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_ACM_PARA_ADDR;
/* Define the union U_ISP_ACM_PARA_UP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    para_up               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_PARA_UP;

/* Define the union U_ISP_VPDCICTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 19  ; /* [18..0]  */
        unsigned int    dci_histlpf_en        : 1   ; /* [19]  */
        unsigned int    dci_shift_ctrl        : 2   ; /* [21..20]  */
        unsigned int    dci_out_range         : 1   ; /* [22]  */
        unsigned int    dci_in_range          : 1   ; /* [23]  */
        unsigned int    dci_cbcrsta_en        : 1   ; /* [24]  */
        unsigned int    dci_cbcrcmp_en        : 1   ; /* [25]  */
        unsigned int    dci_man_adj2          : 1   ; /* [26]  */
        unsigned int    dci_man_adj1          : 1   ; /* [27]  */
        unsigned int    dci_man_adj0          : 1   ; /* [28]  */
        unsigned int    dci_scene_flg         : 1   ; /* [29]  */
        unsigned int    dci_dbg_en            : 1   ; /* [30]  */
        unsigned int    dci_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCICTRL;

/* Define the union U_ISP_VPDCIHPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_hend              : 12  ; /* [19..8]  */
        unsigned int    dci_hstart            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIHPOS;

/* Define the union U_ISP_VPDCIVPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_vend              : 12  ; /* [19..8]  */
        unsigned int    dci_vstart            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIVPOS;

/* Define the union U_ISP_VPDCIHISBLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_cbcrsta_cr        : 8   ; /* [15..8]  */
        unsigned int    dci_cbcrsta_cb        : 8   ; /* [23..16]  */
        unsigned int    dci_cbcrsta_y         : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIHISBLD;

/* Define the union U_ISP_VPDCIHISOFT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 23  ; /* [22..0]  */
        unsigned int    dci_cbcrsta_oft       : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIHISOFT;

/* Define the union U_ISP_VPDCIHISCOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_histcor_thr2      : 8   ; /* [15..8]  */
        unsigned int    dci_histcor_thr1      : 8   ; /* [23..16]  */
        unsigned int    dci_histcor_thr0      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIHISCOR;

/* Define the union U_ISP_VPDCIMERBLD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_org_abld          : 6   ; /* [7..2]  */
        unsigned int    dci_hist_abld         : 6   ; /* [13..8]  */
        unsigned int    dci_metrc_abld2       : 6   ; /* [19..14]  */
        unsigned int    dci_metrc_abld1       : 6   ; /* [25..20]  */
        unsigned int    dci_metrc_abld0       : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIMERBLD;

/* Define the union U_ISP_VPDCIADJWGT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_man_adjwgt2       : 8   ; /* [15..8]  */
        unsigned int    dci_man_adjwgt1       : 8   ; /* [23..16]  */
        unsigned int    dci_man_adjwgt0       : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIADJWGT;

/* Define the union U_ISP_VPDCICLIP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    dci_wgt_cliphigh0     : 8   ; /* [23..16]  */
        unsigned int    dci_wgt_cliplow0      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCICLIP;


/* Define the union U_ISP_VPDCICLIP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    dci_wgt_cliphigh0     : 8   ; /* [23..16]  */
        unsigned int    dci_wgt_cliplow0      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCICLIP0;

/* Define the union U_ISP_VPDCICLIP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    dci_wgt_cliphigh1     : 8   ; /* [23..16]  */
        unsigned int    dci_wgt_cliplow1      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCICLIP1;

/* Define the union U_ISP_VPDCICLIP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    dci_wgt_cliphigh2     : 8   ; /* [23..16]  */
        unsigned int    dci_wgt_cliplow2      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCICLIP2;

/* Define the union U_ISP_VPDCIGLBGAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 14  ; /* [13..0]  */
        unsigned int    dci_glb_gain2         : 6   ; /* [19..14]  */
        unsigned int    dci_glb_gain1         : 6   ; /* [25..20]  */
        unsigned int    dci_glb_gain0         : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIGLBGAIN;

/* Define the union U_ISP_VPDCIPOSTHR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainpos_thr4      : 8   ; /* [7..0]  */
        unsigned int    dci_gainpos_thr3      : 8   ; /* [15..8]  */
        unsigned int    dci_gainpos_thr2      : 8   ; /* [23..16]  */
        unsigned int    dci_gainpos_thr1      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSTHR;


/* Define the union U_ISP_VPDCIPOSTHR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainpos_thr4      : 8   ; /* [7..0]  */
        unsigned int    dci_gainpos_thr3      : 8   ; /* [15..8]  */
        unsigned int    dci_gainpos_thr2      : 8   ; /* [23..16]  */
        unsigned int    dci_gainpos_thr1      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSTHR0;

/* Define the union U_ISP_VPDCIPOSTHR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_gainpos_thr7      : 8   ; /* [15..8]  */
        unsigned int    dci_gainpos_thr6      : 8   ; /* [23..16]  */
        unsigned int    dci_gainpos_thr5      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSTHR1;

/* Define the union U_ISP_VPDCIPOSGAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainpos_cbcr7     : 4   ; /* [3..0]  */
        unsigned int    dci_gainpos_cbcr6     : 4   ; /* [7..4]  */
        unsigned int    dci_gainpos_cbcr5     : 4   ; /* [11..8]  */
        unsigned int    dci_gainpos_cbcr4     : 4   ; /* [15..12]  */
        unsigned int    dci_gainpos_cbcr3     : 4   ; /* [19..16]  */
        unsigned int    dci_gainpos_cbcr2     : 4   ; /* [23..20]  */
        unsigned int    dci_gainpos_cbcr1     : 4   ; /* [27..24]  */
        unsigned int    dci_gainpos_cbcr0     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSGAIN;


/* Define the union U_ISP_VPDCIPOSGAIN0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainpos_cbcr7     : 4   ; /* [3..0]  */
        unsigned int    dci_gainpos_cbcr6     : 4   ; /* [7..4]  */
        unsigned int    dci_gainpos_cbcr5     : 4   ; /* [11..8]  */
        unsigned int    dci_gainpos_cbcr4     : 4   ; /* [15..12]  */
        unsigned int    dci_gainpos_cbcr3     : 4   ; /* [19..16]  */
        unsigned int    dci_gainpos_cbcr2     : 4   ; /* [23..20]  */
        unsigned int    dci_gainpos_cbcr1     : 4   ; /* [27..24]  */
        unsigned int    dci_gainpos_cbcr0     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSGAIN0;

/* Define the union U_ISP_VPDCIPOSGAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 28  ; /* [27..0]  */
        unsigned int    dci_gainpos_cbcr8     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSGAIN1;

/* Define the union U_ISP_VPDCIPOSSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_gainpos_slp2      : 10  ; /* [11..2]  */
        unsigned int    dci_gainpos_slp1      : 10  ; /* [21..12]  */
        unsigned int    dci_gainpos_slp0      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSSLP;


/* Define the union U_ISP_VPDCIPOSSLP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_gainpos_slp2      : 10  ; /* [11..2]  */
        unsigned int    dci_gainpos_slp1      : 10  ; /* [21..12]  */
        unsigned int    dci_gainpos_slp0      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSSLP0;

/* Define the union U_ISP_VPDCIPOSSLP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_gainpos_slp5      : 10  ; /* [11..2]  */
        unsigned int    dci_gainpos_slp4      : 10  ; /* [21..12]  */
        unsigned int    dci_gainpos_slp3      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSSLP1;

/* Define the union U_ISP_VPDCIPOSSLP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 12  ; /* [11..0]  */
        unsigned int    dci_gainpos_slp7      : 10  ; /* [21..12]  */
        unsigned int    dci_gainpos_slp6      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCIPOSSLP2;

/* Define the union U_ISP_VPDCINEGTHR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainneg_thr4      : 8   ; /* [7..0]  */
        unsigned int    dci_gainneg_thr3      : 8   ; /* [15..8]  */
        unsigned int    dci_gainneg_thr2      : 8   ; /* [23..16]  */
        unsigned int    dci_gainneg_thr1      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGTHR;


/* Define the union U_ISP_VPDCINEGTHR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainneg_thr4      : 8   ; /* [7..0]  */
        unsigned int    dci_gainneg_thr3      : 8   ; /* [15..8]  */
        unsigned int    dci_gainneg_thr2      : 8   ; /* [23..16]  */
        unsigned int    dci_gainneg_thr1      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGTHR0;

/* Define the union U_ISP_VPDCINEGTHR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    dci_gainneg_thr7      : 8   ; /* [15..8]  */
        unsigned int    dci_gainneg_thr6      : 8   ; /* [23..16]  */
        unsigned int    dci_gainneg_thr5      : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGTHR1;

/* Define the union U_ISP_VPDCINEGGAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainneg_cbcr7     : 4   ; /* [3..0]  */
        unsigned int    dci_gainneg_cbcr6     : 4   ; /* [7..4]  */
        unsigned int    dci_gainneg_cbcr5     : 4   ; /* [11..8]  */
        unsigned int    dci_gainneg_cbcr4     : 4   ; /* [15..12]  */
        unsigned int    dci_gainneg_cbcr3     : 4   ; /* [19..16]  */
        unsigned int    dci_gainneg_cbcr2     : 4   ; /* [23..20]  */
        unsigned int    dci_gainneg_cbcr1     : 4   ; /* [27..24]  */
        unsigned int    dci_gainneg_cbcr0     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGGAIN;


/* Define the union U_ISP_VPDCINEGGAIN0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_gainneg_cbcr7     : 4   ; /* [3..0]  */
        unsigned int    dci_gainneg_cbcr6     : 4   ; /* [7..4]  */
        unsigned int    dci_gainneg_cbcr5     : 4   ; /* [11..8]  */
        unsigned int    dci_gainneg_cbcr4     : 4   ; /* [15..12]  */
        unsigned int    dci_gainneg_cbcr3     : 4   ; /* [19..16]  */
        unsigned int    dci_gainneg_cbcr2     : 4   ; /* [23..20]  */
        unsigned int    dci_gainneg_cbcr1     : 4   ; /* [27..24]  */
        unsigned int    dci_gainneg_cbcr0     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGGAIN0;

/* Define the union U_ISP_VPDCINEGGAIN1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 28  ; /* [27..0]  */
        unsigned int    dci_gainneg_cbcr8     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGGAIN1;

/* Define the union U_ISP_VPDCINEGSLP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_gainneg_slp2      : 10  ; /* [11..2]  */
        unsigned int    dci_gainneg_slp1      : 10  ; /* [21..12]  */
        unsigned int    dci_gainneg_slp0      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGSLP;


/* Define the union U_ISP_VPDCINEGSLP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_gainneg_slp2      : 10  ; /* [11..2]  */
        unsigned int    dci_gainneg_slp1      : 10  ; /* [21..12]  */
        unsigned int    dci_gainneg_slp0      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGSLP0;

/* Define the union U_ISP_VPDCINEGSLP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    dci_gainneg_slp5      : 10  ; /* [11..2]  */
        unsigned int    dci_gainneg_slp4      : 10  ; /* [21..12]  */
        unsigned int    dci_gainneg_slp3      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGSLP1;

/* Define the union U_ISP_VPDCINEGSLP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 12  ; /* [11..0]  */
        unsigned int    dci_gainneg_slp7      : 10  ; /* [21..12]  */
        unsigned int    dci_gainneg_slp6      : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VPDCINEGSLP2;

/* Define the union U_ISP_VPDCI_RESERVED0 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED0;
/* Define the union U_ISP_VPDCI_RESERVED1 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED1;
/* Define the union U_ISP_VPDCI_RESERVED2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED2;
/* Define the union U_ISP_VPDCI_RESERVED3 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED3;
/* Define the union U_ISP_VPDCI_RESERVED4 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED4;
/* Define the union U_ISP_VPDCI_RESERVED5 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED5;
/* Define the union U_ISP_VPDCI_RESERVED6 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED6;
/* Define the union U_ISP_VPDCI_RESERVED7 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED7;
/* Define the union U_ISP_VPDCI_RESERVED8 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED8;
/* Define the union U_ISP_VPDCI_RESERVED9 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED9;
/* Define the union U_ISP_VPDCI_RESERVED10 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED10;
/* Define the union U_ISP_VPDCI_RESERVED11 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_ISP_VPDCI_RESERVED11;

/* Define the union U_ISP_VDDCIBINWGT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut3            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT;

/* Define the union U_ISP_VDDCIBINWGT00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_0            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_1            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_2            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_3            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT00;

/* Define the union U_ISP_VDDCIBINWGT01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_4            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_5            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_6            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_7            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT01;

/* Define the union U_ISP_VDDCIBINWGT02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_8            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_9            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_10           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_11           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT02;

/* Define the union U_ISP_VDDCIBINWGT03 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_12           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_13           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_14           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_15           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT03;

/* Define the union U_ISP_VDDCIBINWGT04 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_16           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_17           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_18           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_19           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT04;

/* Define the union U_ISP_VDDCIBINWGT05 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_20           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_21           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_22           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_23           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT05;

/* Define the union U_ISP_VDDCIBINWGT06 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_24           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_25           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_26           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_27           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT06;

/* Define the union U_ISP_VDDCIBINWGT07 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut0_28           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut0_29           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut0_30           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut0_31           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT07;

/* Define the union U_ISP_VDDCIBINWGT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_0            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_1            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_2            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_3            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT10;

/* Define the union U_ISP_VDDCIBINWGT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_4            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_5            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_6            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_7            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT11;

/* Define the union U_ISP_VDDCIBINWGT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_8            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_9            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_10           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_11           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT12;

/* Define the union U_ISP_VDDCIBINWGT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_12           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_13           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_14           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_15           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT13;

/* Define the union U_ISP_VDDCIBINWGT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_16           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_17           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_18           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_19           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT14;

/* Define the union U_ISP_VDDCIBINWGT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_20           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_21           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_22           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_23           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT15;

/* Define the union U_ISP_VDDCIBINWGT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_24           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_25           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_26           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_27           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT16;

/* Define the union U_ISP_VDDCIBINWGT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut1_28           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut1_29           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut1_30           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut1_31           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT17;

/* Define the union U_ISP_VDDCIBINWGT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_0            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_1            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_2            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_3            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT20;

/* Define the union U_ISP_VDDCIBINWGT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_4            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_5            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_6            : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_7            : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT21;

/* Define the union U_ISP_VDDCIBINWGT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_8            : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_9            : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_10           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_11           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT22;

/* Define the union U_ISP_VDDCIBINWGT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_12           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_13           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_14           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_15           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT23;

/* Define the union U_ISP_VDDCIBINWGT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_16           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_17           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_18           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_19           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT24;

/* Define the union U_ISP_VDDCIBINWGT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_20           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_21           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_22           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_23           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT25;

/* Define the union U_ISP_VDDCIBINWGT26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_24           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_25           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_26           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_27           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT26;

/* Define the union U_ISP_VDDCIBINWGT27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wgt_lut2_28           : 5   ; /* [4..0]  */
        unsigned int    wgt_lut2_29           : 5   ; /* [9..5]  */
        unsigned int    wgt_lut2_30           : 5   ; /* [14..10]  */
        unsigned int    wgt_lut2_31           : 5   ; /* [19..15]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIBINWGT27;

/* Define the union U_ISP_VDDCIADJLUT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj_0                : 10  ; /* [9..0]  */
        unsigned int    adj_1                : 10  ; /* [19..10]  */
        unsigned int    adj_2                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT;


/* Define the union U_ISP_VDDCIADJLUT00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_0                : 10  ; /* [9..0]  */
        unsigned int    adj0_1                : 10  ; /* [19..10]  */
        unsigned int    adj0_2                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT00;

/* Define the union U_ISP_VDDCIADJLUT01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_3                : 10  ; /* [9..0]  */
        unsigned int    adj0_4                : 10  ; /* [19..10]  */
        unsigned int    adj0_5                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT01;

/* Define the union U_ISP_VDDCIADJLUT02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_6                : 10  ; /* [9..0]  */
        unsigned int    adj0_7                : 10  ; /* [19..10]  */
        unsigned int    adj0_8                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT02;

/* Define the union U_ISP_VDDCIADJLUT03 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_9                : 10  ; /* [9..0]  */
        unsigned int    adj0_10               : 10  ; /* [19..10]  */
        unsigned int    adj0_11               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT03;

/* Define the union U_ISP_VDDCIADJLUT04 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_12               : 10  ; /* [9..0]  */
        unsigned int    adj0_13               : 10  ; /* [19..10]  */
        unsigned int    adj0_14               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT04;

/* Define the union U_ISP_VDDCIADJLUT05 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_15               : 10  ; /* [9..0]  */
        unsigned int    adj0_16               : 10  ; /* [19..10]  */
        unsigned int    adj0_17               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT05;

/* Define the union U_ISP_VDDCIADJLUT06 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_18               : 10  ; /* [9..0]  */
        unsigned int    adj0_19               : 10  ; /* [19..10]  */
        unsigned int    adj0_20               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT06;

/* Define the union U_ISP_VDDCIADJLUT07 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_21               : 10  ; /* [9..0]  */
        unsigned int    adj0_22               : 10  ; /* [19..10]  */
        unsigned int    adj0_23               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT07;

/* Define the union U_ISP_VDDCIADJLUT08 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_24               : 10  ; /* [9..0]  */
        unsigned int    adj0_25               : 10  ; /* [19..10]  */
        unsigned int    adj0_26               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT08;

/* Define the union U_ISP_VDDCIADJLUT09 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_27               : 10  ; /* [9..0]  */
        unsigned int    adj0_28               : 10  ; /* [19..10]  */
        unsigned int    adj0_29               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT09;

/* Define the union U_ISP_VDDCIADJLUT0A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj0_30               : 10  ; /* [9..0]  */
        unsigned int    adj0_31               : 10  ; /* [19..10]  */
        unsigned int    adj0_32               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT0A;

/* Define the union U_ISP_VDDCIADJLUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_0                : 10  ; /* [9..0]  */
        unsigned int    adj1_1                : 10  ; /* [19..10]  */
        unsigned int    adj1_2                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT10;

/* Define the union U_ISP_VDDCIADJLUT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_3                : 10  ; /* [9..0]  */
        unsigned int    adj1_4                : 10  ; /* [19..10]  */
        unsigned int    adj1_5                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT11;

/* Define the union U_ISP_VDDCIADJLUT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_6                : 10  ; /* [9..0]  */
        unsigned int    adj1_7                : 10  ; /* [19..10]  */
        unsigned int    adj1_8                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT12;

/* Define the union U_ISP_VDDCIADJLUT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_9                : 10  ; /* [9..0]  */
        unsigned int    adj1_10               : 10  ; /* [19..10]  */
        unsigned int    adj1_11               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT13;

/* Define the union U_ISP_VDDCIADJLUT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_12               : 10  ; /* [9..0]  */
        unsigned int    adj1_13               : 10  ; /* [19..10]  */
        unsigned int    adj1_14               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT14;

/* Define the union U_ISP_VDDCIADJLUT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_15               : 10  ; /* [9..0]  */
        unsigned int    adj1_16               : 10  ; /* [19..10]  */
        unsigned int    adj1_17               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT15;

/* Define the union U_ISP_VDDCIADJLUT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_18               : 10  ; /* [9..0]  */
        unsigned int    adj1_19               : 10  ; /* [19..10]  */
        unsigned int    adj1_20               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT16;

/* Define the union U_ISP_VDDCIADJLUT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_21               : 10  ; /* [9..0]  */
        unsigned int    adj1_22               : 10  ; /* [19..10]  */
        unsigned int    adj1_23               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT17;

/* Define the union U_ISP_VDDCIADJLUT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_24               : 10  ; /* [9..0]  */
        unsigned int    adj1_25               : 10  ; /* [19..10]  */
        unsigned int    adj1_26               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT18;

/* Define the union U_ISP_VDDCIADJLUT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_27               : 10  ; /* [9..0]  */
        unsigned int    adj1_28               : 10  ; /* [19..10]  */
        unsigned int    adj1_29               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT19;

/* Define the union U_ISP_VDDCIADJLUT1A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj1_30               : 10  ; /* [9..0]  */
        unsigned int    adj1_31               : 10  ; /* [19..10]  */
        unsigned int    adj1_32               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT1A;

/* Define the union U_ISP_VDDCIADJLUT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_0                : 10  ; /* [9..0]  */
        unsigned int    adj2_1                : 10  ; /* [19..10]  */
        unsigned int    adj2_2                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT20;

/* Define the union U_ISP_VDDCIADJLUT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_3                : 10  ; /* [9..0]  */
        unsigned int    adj2_4                : 10  ; /* [19..10]  */
        unsigned int    adj2_5                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT21;

/* Define the union U_ISP_VDDCIADJLUT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_6                : 10  ; /* [9..0]  */
        unsigned int    adj2_7                : 10  ; /* [19..10]  */
        unsigned int    adj2_8                : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT22;

/* Define the union U_ISP_VDDCIADJLUT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_9                : 10  ; /* [9..0]  */
        unsigned int    adj2_10               : 10  ; /* [19..10]  */
        unsigned int    adj2_11               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT23;

/* Define the union U_ISP_VDDCIADJLUT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_12               : 10  ; /* [9..0]  */
        unsigned int    adj2_13               : 10  ; /* [19..10]  */
        unsigned int    adj2_14               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT24;

/* Define the union U_ISP_VDDCIADJLUT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_15               : 10  ; /* [9..0]  */
        unsigned int    adj2_16               : 10  ; /* [19..10]  */
        unsigned int    adj2_17               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT25;

/* Define the union U_ISP_VDDCIADJLUT26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_18               : 10  ; /* [9..0]  */
        unsigned int    adj2_19               : 10  ; /* [19..10]  */
        unsigned int    adj2_20               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT26;

/* Define the union U_ISP_VDDCIADJLUT27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_21               : 10  ; /* [9..0]  */
        unsigned int    adj2_22               : 10  ; /* [19..10]  */
        unsigned int    adj2_23               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT27;

/* Define the union U_ISP_VDDCIADJLUT28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_24               : 10  ; /* [9..0]  */
        unsigned int    adj2_25               : 10  ; /* [19..10]  */
        unsigned int    adj2_26               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT28;

/* Define the union U_ISP_VDDCIADJLUT29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_27               : 10  ; /* [9..0]  */
        unsigned int    adj2_28               : 10  ; /* [19..10]  */
        unsigned int    adj2_29               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT29;

/* Define the union U_ISP_VDDCIADJLUT2A */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adj2_30               : 10  ; /* [9..0]  */
        unsigned int    adj2_31               : 10  ; /* [19..10]  */
        unsigned int    adj2_32               : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIADJLUT2A;

/* Define the union U_ISP_VDDCIDIVLUT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div0              : 12  ; /* [11..0]  */
        unsigned int    dci_div1              : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT;


/* Define the union U_ISP_VDDCIDIVLUT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div0              : 12  ; /* [11..0]  */
        unsigned int    dci_div1              : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT0;

/* Define the union U_ISP_VDDCIDIVLUT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div2              : 12  ; /* [11..0]  */
        unsigned int    dci_div3              : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT1;

/* Define the union U_ISP_VDDCIDIVLUT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div4              : 12  ; /* [11..0]  */
        unsigned int    dci_div5              : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT2;

/* Define the union U_ISP_VDDCIDIVLUT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div6              : 12  ; /* [11..0]  */
        unsigned int    dci_div7              : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT3;

/* Define the union U_ISP_VDDCIDIVLUT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div8              : 12  ; /* [11..0]  */
        unsigned int    dci_div9              : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT4;

/* Define the union U_ISP_VDDCIDIVLUT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div10             : 12  ; /* [11..0]  */
        unsigned int    dci_div11             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT5;

/* Define the union U_ISP_VDDCIDIVLUT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div12             : 12  ; /* [11..0]  */
        unsigned int    dci_div13             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT6;

/* Define the union U_ISP_VDDCIDIVLUT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div14             : 12  ; /* [11..0]  */
        unsigned int    dci_div15             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT7;

/* Define the union U_ISP_VDDCIDIVLUT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div16             : 12  ; /* [11..0]  */
        unsigned int    dci_div17             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT8;

/* Define the union U_ISP_VDDCIDIVLUT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div18             : 12  ; /* [11..0]  */
        unsigned int    dci_div19             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT9;

/* Define the union U_ISP_VDDCIDIVLUT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div20             : 12  ; /* [11..0]  */
        unsigned int    dci_div21             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT10;

/* Define the union U_ISP_VDDCIDIVLUT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div22             : 12  ; /* [11..0]  */
        unsigned int    dci_div23             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT11;

/* Define the union U_ISP_VDDCIDIVLUT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div24             : 12  ; /* [11..0]  */
        unsigned int    dci_div25             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT12;

/* Define the union U_ISP_VDDCIDIVLUT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div26             : 12  ; /* [11..0]  */
        unsigned int    dci_div27             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT13;

/* Define the union U_ISP_VDDCIDIVLUT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div28             : 12  ; /* [11..0]  */
        unsigned int    dci_div29             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT14;

/* Define the union U_ISP_VDDCIDIVLUT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div30             : 12  ; /* [11..0]  */
        unsigned int    dci_div31             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT15;

/* Define the union U_ISP_VDDCIDIVLUT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div32             : 12  ; /* [11..0]  */
        unsigned int    dci_div33             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT16;

/* Define the union U_ISP_VDDCIDIVLUT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div34             : 12  ; /* [11..0]  */
        unsigned int    dci_div35             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT17;

/* Define the union U_ISP_VDDCIDIVLUT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div36             : 12  ; /* [11..0]  */
        unsigned int    dci_div37             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT18;

/* Define the union U_ISP_VDDCIDIVLUT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div38             : 12  ; /* [11..0]  */
        unsigned int    dci_div39             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT19;

/* Define the union U_ISP_VDDCIDIVLUT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div40             : 12  ; /* [11..0]  */
        unsigned int    dci_div41             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT20;

/* Define the union U_ISP_VDDCIDIVLUT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div42             : 12  ; /* [11..0]  */
        unsigned int    dci_div43             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT21;

/* Define the union U_ISP_VDDCIDIVLUT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div44             : 12  ; /* [11..0]  */
        unsigned int    dci_div45             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT22;

/* Define the union U_ISP_VDDCIDIVLUT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div46             : 12  ; /* [11..0]  */
        unsigned int    dci_div47             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT23;

/* Define the union U_ISP_VDDCIDIVLUT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div48             : 12  ; /* [11..0]  */
        unsigned int    dci_div49             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT24;

/* Define the union U_ISP_VDDCIDIVLUT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div50             : 12  ; /* [11..0]  */
        unsigned int    dci_div51             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT25;

/* Define the union U_ISP_VDDCIDIVLUT26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div52             : 12  ; /* [11..0]  */
        unsigned int    dci_div53             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT26;

/* Define the union U_ISP_VDDCIDIVLUT27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div54             : 12  ; /* [11..0]  */
        unsigned int    dci_div55             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT27;

/* Define the union U_ISP_VDDCIDIVLUT28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div56             : 12  ; /* [11..0]  */
        unsigned int    dci_div57             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT28;

/* Define the union U_ISP_VDDCIDIVLUT29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div58             : 12  ; /* [11..0]  */
        unsigned int    dci_div59             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT29;

/* Define the union U_ISP_VDDCIDIVLUT30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div60             : 12  ; /* [11..0]  */
        unsigned int    dci_div61             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT30;

/* Define the union U_ISP_VDDCIDIVLUT31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dci_div62             : 12  ; /* [11..0]  */
        unsigned int    dci_div63             : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VDDCIDIVLUT31;

typedef struct
{
	U_ISP_AEWDR0_CFG       ISP_AEWDR0_CFG                    ; /* 0x29000 */
    unsigned int           reserved_124[2]                   ; /* 0x29004~0x29008 */
    U_ISP_AEWDR0_VERSION   ISP_AEWDR0_VERSION                ; /* 0x2900c */
    U_ISP_AEWDR0_ZONE      ISP_AEWDR0_ZONE                   ; /* 0x29010 */
    U_ISP_AEWDR0_SKIP_CRG   ISP_AEWDR0_SKIP_CRG              ; /* 0x29014 */
    U_ISP_AEWDR0_TOTAL_STAT   ISP_AEWDR0_TOTAL_STAT          ; /* 0x29018 */
    U_ISP_AEWDR0_COUNT_STAT   ISP_AEWDR0_COUNT_STAT          ; /* 0x2901c */
    unsigned int           reserved_125[4]                     ; /* 0x29020~0x2902c */
    U_ISP_AEWDR0_HIST_HIGH   ISP_AEWDR0_HIST_HIGH            ; /* 0x29030 */
    unsigned int           reserved_126[3]                     ; /* 0x29034~0x2903c */
    U_ISP_AEWDR0_BITMOVE   ISP_AEWDR0_BITMOVE                ; /* 0x29040 */
    U_ISP_AEWDR0_OFFSET_R   ISP_AEWDR0_OFFSET_R              ; /* 0x29044 */
    U_ISP_AEWDR0_OFFSET_GR   ISP_AEWDR0_OFFSET_GR            ; /* 0x29048 */
    U_ISP_AEWDR0_OFFSET_GB   ISP_AEWDR0_OFFSET_GB            ; /* 0x2904c */
    U_ISP_AEWDR0_OFFSET_B   ISP_AEWDR0_OFFSET_B              ; /* 0x29050 */
    unsigned int           reserved_127[11]                     ; /* 0x29054~0x2907c */
    U_ISP_AEWDR0_MEM_HIST_WADDR   ISP_AEWDR0_MEM_HIST_WADDR  ; /* 0x29080 */
    U_ISP_AEWDR0_MEM_HIST_WDATA   ISP_AEWDR0_MEM_HIST_WDATA  ; /* 0x29084 */
    U_ISP_AEWDR0_MEM_HIST_RADDR   ISP_AEWDR0_MEM_HIST_RADDR  ; /* 0x29088 */
    U_ISP_AEWDR0_MEM_HIST_RDATA   ISP_AEWDR0_MEM_HIST_RDATA  ; /* 0x2908c */
    unsigned int           reserved_128[4]                     ; /* 0x29090~0x2909c */
    U_ISP_AEWDR0_MEM_WEIGHT_WADDR   ISP_AEWDR0_MEM_WEIGHT_WADDR ; /* 0x290a0 */
    U_ISP_AEWDR0_MEM_WEIGHT_WDATA   ISP_AEWDR0_MEM_WEIGHT_WDATA ; /* 0x290a4 */
    U_ISP_AEWDR0_MEM_WEIGHT_RADDR   ISP_AEWDR0_MEM_WEIGHT_RADDR ; /* 0x290a8 */
    U_ISP_AEWDR0_MEM_WEIGHT_RDATA   ISP_AEWDR0_MEM_WEIGHT_RDATA ; /* 0x290ac */
    unsigned int           reserved_129[12]                     ; /* 0x290b0~0x290dc */
    U_ISP_AEWDR0_CTRL_F    ISP_AEWDR0_CTRL_F                 ; /* 0x290e0 */
    U_ISP_AEWDR0_CTRL_I    ISP_AEWDR0_CTRL_I                 ; /* 0x290e4 */
    unsigned int           reserved_130                     ; /* 0x290e8 */
    U_ISP_AEWDR0_UPDATE    ISP_AEWDR0_UPDATE                 ; /* 0x290ec */
    U_ISP_AEWDR0_SIZE      ISP_AEWDR0_SIZE                   ; /* 0x290f0 */
    unsigned int           reserved_131[3]                     ; /* 0x290f4~0x290fc */
}ISP_AE_WDR_S;

//==============================================================================
/* Define the global struct */
typedef struct
{
	volatile unsigned int           reserved_0[32]                    ; /* 0x20000~0x2007c */
    volatile U_ISP_FE_VERSION       ISP_FE_VERSION                    ; /* 0x20080 */
    volatile U_ISP_FE_DATE          ISP_FE_DATE                       ; /* 0x20084 */
    volatile unsigned int           reserved_1[2]                     ; /* 0x20088~0x2008c */
    volatile U_ISP_FE_MODULE_POS    ISP_FE_MODULE_POS                 ; /* 0x20090 */
    volatile U_ISP_FE_FSTART_DELAY   ISP_FE_FSTART_DELAY              ; /* 0x20094 */
    volatile unsigned int           reserved_2[2]                     ; /* 0x20098~0x2009c */
    volatile U_ISP_FE_USER_DEFINE0   ISP_FE_USER_DEFINE0              ; /* 0x200a0 */
    volatile U_ISP_FE_USER_DEFINE1   ISP_FE_USER_DEFINE1              ; /* 0x200a4 */
    volatile unsigned int           reserved_3[2]                     ; /* 0x200a8~0x200ac */
    volatile U_ISP_FE_STARTUP       ISP_FE_STARTUP                    ; /* 0x200b0 */
    volatile unsigned int           reserved_4[15]                     ; /* 0x200b4~0x200ec */
    volatile U_ISP_FE_INT           ISP_FE_INT                        ; /* 0x200f0 */
    volatile unsigned int           reserved_5                     ; /* 0x200f4 */
    volatile U_ISP_FE_INT_MASK      ISP_FE_INT_MASK                   ; /* 0x200f8 */
    volatile unsigned int           reserved_6[57]                     ; /* 0x200fc~0x201dc */
    volatile U_ISP_FE_CTRL_F        ISP_FE_CTRL_F                     ; /* 0x201e0 */
    volatile U_ISP_FE_CTRL_I        ISP_FE_CTRL_I                     ; /* 0x201e4 */
    volatile U_ISP_FE_TIMING_CFG    ISP_FE_TIMING_CFG                 ; /* 0x201e8 */
    volatile U_ISP_FE_REG_UPDATE    ISP_FE_REG_UPDATE                 ; /* 0x201ec */
    volatile U_ISP_RESERVED0        ISP_RESERVED0                     ; /* 0x201f0 */
    volatile unsigned int           reserved_7[387]                   ; /* 0x201f4~0x207fc */
    volatile U_ISP_CROP0_CFG        ISP_CROP0_CFG                     ; /* 0x20800 */
    volatile U_ISP_CROP0_RESERVED   ISP_CROP0_RESERVED                ; /* 0x20804 */
    volatile U_ISP_CROP0_START      ISP_CROP0_START                   ; /* 0x20808 */
    volatile U_ISP_CROP0_SIZE       ISP_CROP0_SIZE                    ; /* 0x2080c */
    volatile unsigned int           reserved_8[4]                     ; /* 0x20810~0x2081c */
    volatile U_ISP_DRC_DITHER       ISP_DRC_DITHER                    ; /* 0x20820 */
    volatile unsigned int           reserved_9[3]                     ; /* 0x20824~0x2082c */
    volatile U_ISP_INPUT_MUX        ISP_INPUT_MUX                     ; /* 0x20830 */
    volatile unsigned int           reserved_10[3]                     ; /* 0x20834~0x2083c */
    volatile U_ISP_FE_BLACK_SUM_CFG   ISP_FE_BLACK_SUM_CFG            ; /* 0x20840 */
    volatile U_ISP_FE_BAYER_SUM_CFG   ISP_FE_BAYER_SUM_CFG            ; /* 0x20844 */
    volatile U_ISP_FE_WDR_SUM_CFG   ISP_FE_WDR_SUM_CFG                ; /* 0x20848 */
    volatile unsigned int           reserved_11[13]                     ; /* 0x2084c~0x2087c */
    volatile U_ISP_FE_BLACK_SUM0    ISP_FE_BLACK_SUM0                 ; /* 0x20880 */
    volatile U_ISP_FE_BLACK_SUM1    ISP_FE_BLACK_SUM1                 ; /* 0x20884 */
    volatile U_ISP_FE_BAYER_SUM0    ISP_FE_BAYER_SUM0                 ; /* 0x20888 */
    volatile U_ISP_FE_BAYER_SUM1    ISP_FE_BAYER_SUM1                 ; /* 0x2088c */
    volatile U_ISP_FE_WDR0_SUM0     ISP_FE_WDR0_SUM0                  ; /* 0x20890 */
    volatile U_ISP_FE_WDR0_SUM1     ISP_FE_WDR0_SUM1                  ; /* 0x20894 */
    volatile U_ISP_FE_WDR1_SUM0     ISP_FE_WDR1_SUM0                  ; /* 0x20898 */
    volatile U_ISP_FE_WDR1_SUM1     ISP_FE_WDR1_SUM1                  ; /* 0x2089c */
    volatile U_ISP_FE_WDR2_SUM0     ISP_FE_WDR2_SUM0                  ; /* 0x208a0 */
    volatile U_ISP_FE_WDR2_SUM1     ISP_FE_WDR2_SUM1                  ; /* 0x208a4 */
    volatile U_ISP_FE_WDR3_SUM0     ISP_FE_WDR3_SUM0                  ; /* 0x208a8 */
    volatile U_ISP_FE_WDR3_SUM1     ISP_FE_WDR3_SUM1                  ; /* 0x208ac */
    volatile unsigned int           reserved_12[84]                     ; /* 0x208b0~0x209fc */
    volatile U_ISP_COLORBAR_CFG     ISP_COLORBAR_CFG                  ; /* 0x20a00 */
    volatile unsigned int           reserved_13[2]                     ; /* 0x20a04~0x20a08 */
    volatile U_ISP_COLORBAR_VERSION   ISP_COLORBAR_VERSION            ; /* 0x20a0c */
    volatile U_ISP_COLORBAR_PATTERN   ISP_COLORBAR_PATTERN            ; /* 0x20a10 */
    volatile U_ISP_COLORBAR_RBACKGND   ISP_COLORBAR_RBACKGND          ; /* 0x20a14 */
    volatile U_ISP_COLORBAR_GBACKGND   ISP_COLORBAR_GBACKGND          ; /* 0x20a18 */
    volatile U_ISP_COLORBAR_BBACKGND   ISP_COLORBAR_BBACKGND          ; /* 0x20a1c */
    volatile U_ISP_COLORBAR_RFOREGND   ISP_COLORBAR_RFOREGND          ; /* 0x20a20 */
    volatile U_ISP_COLORBAR_GFOREGND   ISP_COLORBAR_GFOREGND          ; /* 0x20a24 */
    volatile U_ISP_COLORBAR_BFOREGND   ISP_COLORBAR_BFOREGND          ; /* 0x20a28 */
    volatile U_ISP_COLORBAR_INIT    ISP_COLORBAR_INIT                 ; /* 0x20a2c */
    volatile U_ISP_COLORBAR_GRAD    ISP_COLORBAR_GRAD                 ; /* 0x20a30 */
    volatile U_ISP_COLORBAR_POS1    ISP_COLORBAR_POS1                 ; /* 0x20a34 */
    volatile U_ISP_COLORBAR_POS2    ISP_COLORBAR_POS2                 ; /* 0x20a38 */
    volatile U_ISP_COLORBAR_RGGB    ISP_COLORBAR_RGGB                 ; /* 0x20a3c */
    volatile unsigned int           reserved_14[41]                     ; /* 0x20a40~0x20ae0 */
    volatile U_ISP_COLORBAR_CTRL_I   ISP_COLORBAR_CTRL_I              ; /* 0x20ae4 */
    volatile unsigned int           reserved_15                     ; /* 0x20ae8 */
    volatile U_ISP_COLORBAR_UPDATE   ISP_COLORBAR_UPDATE              ; /* 0x20aec */
    volatile U_ISP_COLORBAR_SIZE    ISP_COLORBAR_SIZE                 ; /* 0x20af0 */
    volatile unsigned int           reserved_16[323]                     ; /* 0x20af4~0x20ffc */
    volatile U_ISP_BLC_CFG          ISP_BLC_CFG                       ; /* 0x21000 */
    volatile unsigned int           reserved_17[2]                     ; /* 0x21004~0x21008 */
    volatile U_ISP_BLC_VERSION      ISP_BLC_VERSION                   ; /* 0x2100c */
    volatile U_ISP_BLC_OFFSET1      ISP_BLC_OFFSET1                   ; /* 0x21010 */
    volatile U_ISP_BLC_OFFSET2      ISP_BLC_OFFSET2                   ; /* 0x21014 */
    volatile unsigned int           reserved_18[50]                     ; /* 0x21018~0x210dc */
    volatile U_ISP_BLC_CTRL_F       ISP_BLC_CTRL_F                    ; /* 0x210e0 */
    volatile U_ISP_BLC_CTRL_I       ISP_BLC_CTRL_I                    ; /* 0x210e4 */
    volatile unsigned int           reserved_19                     ; /* 0x210e8 */
    volatile U_ISP_BLC_UPDATE       ISP_BLC_UPDATE                    ; /* 0x210ec */
    volatile unsigned int           reserved_20[4]                     ; /* 0x210f0~0x210fc */
    volatile U_ISP_4BLC_CFG         ISP_4BLC_CFG                      ; /* 0x21100 */
    volatile unsigned int           reserved_21[2]                     ; /* 0x21104~0x21108 */
    volatile U_ISP_4BLC_VERSION     ISP_4BLC_VERSION                  ; /* 0x2110c */
    volatile U_ISP_4BLC0_OFFSET1    ISP_4BLC0_OFFSET1                 ; /* 0x21110 */
    volatile U_ISP_4BLC0_OFFSET2    ISP_4BLC0_OFFSET2                 ; /* 0x21114 */
    volatile U_ISP_4BLC1_OFFSET1    ISP_4BLC1_OFFSET1                 ; /* 0x21118 */
    volatile U_ISP_4BLC1_OFFSET2    ISP_4BLC1_OFFSET2                 ; /* 0x2111c */
    volatile U_ISP_4BLC2_OFFSET1    ISP_4BLC2_OFFSET1                 ; /* 0x21120 */
    volatile U_ISP_4BLC2_OFFSET2    ISP_4BLC2_OFFSET2                 ; /* 0x21124 */
    volatile U_ISP_4BLC3_OFFSET1    ISP_4BLC3_OFFSET1                 ; /* 0x21128 */
    volatile U_ISP_4BLC3_OFFSET2    ISP_4BLC3_OFFSET2                 ; /* 0x2112c */
    volatile unsigned int           reserved_22[44]                     ; /* 0x21130~0x211dc */
    volatile U_ISP_4BLC_CTRL_F      ISP_4BLC_CTRL_F                   ; /* 0x211e0 */
    volatile U_ISP_4BLC_CTRL_I      ISP_4BLC_CTRL_I                   ; /* 0x211e4 */
    volatile unsigned int           reserved_23                     ; /* 0x211e8 */
    volatile U_ISP_4BLC_UPDATE      ISP_4BLC_UPDATE                   ; /* 0x211ec */
    volatile unsigned int           reserved_24[4]                     ; /* 0x211f0~0x211fc */
    volatile U_ISP_WB_CFG           ISP_WB_CFG                        ; /* 0x21200 */
    volatile U_ISP_WB_BLC_CFG       ISP_WB_BLC_CFG                    ; /* 0x21204 */
    volatile unsigned int           reserved_25                     ; /* 0x21208 */
    volatile U_ISP_WB_VERSION       ISP_WB_VERSION                    ; /* 0x2120c */
    volatile U_ISP_WB_GAIN1         ISP_WB_GAIN1                      ; /* 0x21210 */
    volatile U_ISP_WB_GAIN2         ISP_WB_GAIN2                      ; /* 0x21214 */
    volatile U_ISP_WB_BLC_OFFSET1   ISP_WB_BLC_OFFSET1                ; /* 0x21218 */
    volatile U_ISP_WB_BLC_OFFSET2   ISP_WB_BLC_OFFSET2                ; /* 0x2121c */
    volatile unsigned int           reserved_26[48]                     ; /* 0x21220~0x212dc */
    volatile U_ISP_WB_CTRL_F        ISP_WB_CTRL_F                     ; /* 0x212e0 */
    volatile U_ISP_WB_CTRL_I        ISP_WB_CTRL_I                     ; /* 0x212e4 */
    volatile unsigned int           reserved_27                     ; /* 0x212e8 */
    volatile U_ISP_WB_UPDATE        ISP_WB_UPDATE                     ; /* 0x212ec */
    volatile unsigned int           reserved_28[4]                     ; /* 0x212f0~0x212fc */
    volatile U_ISP_DG_CFG           ISP_DG_CFG                        ; /* 0x21300 */
    volatile U_ISP_DG_BLC_CFG       ISP_DG_BLC_CFG                    ; /* 0x21304 */
    volatile unsigned int           reserved_29                     ; /* 0x21308 */
    volatile U_ISP_DG_VERSION       ISP_DG_VERSION                    ; /* 0x2130c */
    volatile U_ISP_DG_GAIN1         ISP_DG_GAIN1                      ; /* 0x21310 */
    volatile U_ISP_DG_GAIN2         ISP_DG_GAIN2                      ; /* 0x21314 */
    volatile U_ISP_DG_BLC_OFFSET1   ISP_DG_BLC_OFFSET1                ; /* 0x21318 */
    volatile U_ISP_DG_BLC_OFFSET2   ISP_DG_BLC_OFFSET2                ; /* 0x2131c */
    volatile unsigned int           reserved_30[48]                     ; /* 0x21320~0x213dc */
    volatile U_ISP_DG_CTRL_F        ISP_DG_CTRL_F                     ; /* 0x213e0 */
    volatile U_ISP_DG_CTRL_I        ISP_DG_CTRL_I                     ; /* 0x213e4 */
    volatile unsigned int           reserved_31                     ; /* 0x213e8 */
    volatile U_ISP_DG_UPDATE        ISP_DG_UPDATE                     ; /* 0x213ec */
    volatile unsigned int           reserved_32[516]                     ; /* 0x213f0~0x21bfc */
    volatile U_ISP_4DG_CFG          ISP_4DG_CFG                       ; /* 0x21c00 */
    volatile U_ISP_4DG_BLC_CFG      ISP_4DG_BLC_CFG                   ; /* 0x21c04 */
    volatile unsigned int           reserved_33                     ; /* 0x21c08 */
    volatile U_ISP_4DG_VERSION      ISP_4DG_VERSION                   ; /* 0x21c0c */
    volatile U_ISP_4DG_0_GAIN1      ISP_4DG_0_GAIN1                   ; /* 0x21c10 */
    volatile U_ISP_4DG_0_GAIN2      ISP_4DG_0_GAIN2                   ; /* 0x21c14 */
    volatile U_ISP_4DG_0_BLC_OFFSET1   ISP_4DG_0_BLC_OFFSET1          ; /* 0x21c18 */
    volatile U_ISP_4DG_0_BLC_OFFSET2   ISP_4DG_0_BLC_OFFSET2          ; /* 0x21c1c */
    volatile U_ISP_4DG_1_GAIN1      ISP_4DG_1_GAIN1                   ; /* 0x21c20 */
    volatile U_ISP_4DG_1_GAIN2      ISP_4DG_1_GAIN2                   ; /* 0x21c24 */
    volatile U_ISP_4DG_1_BLC_OFFSET1   ISP_4DG_1_BLC_OFFSET1          ; /* 0x21c28 */
    volatile U_ISP_4DG_1_BLC_OFFSET2   ISP_4DG_1_BLC_OFFSET2          ; /* 0x21c2c */
    volatile U_ISP_4DG_2_GAIN1      ISP_4DG_2_GAIN1                   ; /* 0x21c30 */
    volatile U_ISP_4DG_2_GAIN2      ISP_4DG_2_GAIN2                   ; /* 0x21c34 */
    volatile U_ISP_4DG_2_BLC_OFFSET1   ISP_4DG_2_BLC_OFFSET1          ; /* 0x21c38 */
    volatile U_ISP_4DG_2_BLC_OFFSET2   ISP_4DG_2_BLC_OFFSET2          ; /* 0x21c3c */
    volatile U_ISP_4DG_3_GAIN1      ISP_4DG_3_GAIN1                   ; /* 0x21c40 */
    volatile U_ISP_4DG_3_GAIN2      ISP_4DG_3_GAIN2                   ; /* 0x21c44 */
    volatile U_ISP_4DG_3_BLC_OFFSET1   ISP_4DG_3_BLC_OFFSET1          ; /* 0x21c48 */
    volatile U_ISP_4DG_3_BLC_OFFSET2   ISP_4DG_3_BLC_OFFSET2          ; /* 0x21c4c */
    volatile unsigned int           reserved_34[36]                     ; /* 0x21c50~0x21cdc */
    volatile U_ISP_4DG_CTRL_F       ISP_4DG_CTRL_F                    ; /* 0x21ce0 */
    volatile U_ISP_4DG_CTRL_I       ISP_4DG_CTRL_I                    ; /* 0x21ce4 */
    volatile unsigned int           reserved_35                     ; /* 0x21ce8 */
    volatile U_ISP_4DG_UPDATE       ISP_4DG_UPDATE                    ; /* 0x21cec */
    volatile unsigned int           reserved_36[196]                     ; /* 0x21cf0~0x21ffc */
    volatile U_ISP_AE_CFG           ISP_AE_CFG                        ; /* 0x22000 */
    volatile unsigned int           reserved_37[2]                     ; /* 0x22004~0x22008 */
    volatile U_ISP_AE_VERSION       ISP_AE_VERSION                    ; /* 0x2200c */
    volatile U_ISP_AE_ZONE          ISP_AE_ZONE                       ; /* 0x22010 */
    volatile U_ISP_AE_SKIP_CRG      ISP_AE_SKIP_CRG                   ; /* 0x22014 */
    volatile U_ISP_AE_TOTAL_STAT    ISP_AE_TOTAL_STAT                 ; /* 0x22018 */
    volatile U_ISP_AE_COUNT_STAT    ISP_AE_COUNT_STAT                 ; /* 0x2201c */
    volatile U_ISP_AE_TOTAL_R_AVER   ISP_AE_TOTAL_R_AVER              ; /* 0x22020 */
    volatile U_ISP_AE_TOTAL_GR_AVER   ISP_AE_TOTAL_GR_AVER            ; /* 0x22024 */
    volatile U_ISP_AE_TOTAL_GB_AVER   ISP_AE_TOTAL_GB_AVER            ; /* 0x22028 */
    volatile U_ISP_AE_TOTAL_B_AVER   ISP_AE_TOTAL_B_AVER              ; /* 0x2202c */
    volatile U_ISP_AE_HIST_HIGH     ISP_AE_HIST_HIGH                  ; /* 0x22030 */
    volatile unsigned int           reserved_38[3]                     ; /* 0x22034~0x2203c */
    volatile U_ISP_AE_BITMOVE       ISP_AE_BITMOVE                    ; /* 0x22040 */
    volatile U_ISP_AE_OFFSET_R      ISP_AE_OFFSET_R                   ; /* 0x22044 */
    volatile U_ISP_AE_OFFSET_GR     ISP_AE_OFFSET_GR                  ; /* 0x22048 */
    volatile U_ISP_AE_OFFSET_GB     ISP_AE_OFFSET_GB                  ; /* 0x2204c */
    volatile U_ISP_AE_OFFSET_B      ISP_AE_OFFSET_B                   ; /* 0x22050 */
    volatile unsigned int           reserved_39[3]                     ; /* 0x22054~0x2205c */
    volatile U_ISP_AE_GAMMA_LIMIT   ISP_AE_GAMMA_LIMIT                ; /* 0x22060 */
    volatile unsigned int           reserved_40[3]                     ; /* 0x22064~0x2206c */
    volatile U_ISP_AE_FOURPLANEMODE   ISP_AE_FOURPLANEMODE            ; /* 0x22070 */
    volatile unsigned int           reserved_411[3]                     ; /* 0x22074~0x2207c */
    volatile U_ISP_AE_MEM_HIST_WADDR   ISP_AE_MEM_HIST_WADDR          ; /* 0x22080 */
    volatile U_ISP_AE_MEM_HIST_WDATA   ISP_AE_MEM_HIST_WDATA          ; /* 0x22084 */
    volatile U_ISP_AE_MEM_HIST_RADDR   ISP_AE_MEM_HIST_RADDR          ; /* 0x22088 */
    volatile U_ISP_AE_MEM_HIST_RDATA   ISP_AE_MEM_HIST_RDATA          ; /* 0x2208c */
    volatile U_ISP_AE_MEM_AVER_R_GR_WADDR   ISP_AE_MEM_AVER_R_GR_WADDR ; /* 0x22090 */
    volatile U_ISP_AE_MEM_AVER_R_GR_WDATA   ISP_AE_MEM_AVER_R_GR_WDATA ; /* 0x22094 */
    volatile U_ISP_AE_MEM_AVER_R_GR_RADDR   ISP_AE_MEM_AVER_R_GR_RADDR ; /* 0x22098 */
    volatile U_ISP_AE_MEM_AVER_R_GR_RDATA   ISP_AE_MEM_AVER_R_GR_RDATA ; /* 0x2209c */
    volatile U_ISP_AE_MEM_AVER_GB_B_WADDR   ISP_AE_MEM_AVER_GB_B_WADDR ; /* 0x220a0 */
    volatile U_ISP_AE_MEM_AVER_GB_B_WDATA   ISP_AE_MEM_AVER_GB_B_WDATA ; /* 0x220a4 */
    volatile U_ISP_AE_MEM_AVER_GB_B_RADDR   ISP_AE_MEM_AVER_GB_B_RADDR ; /* 0x220a8 */
    volatile U_ISP_AE_MEM_AVER_GB_B_RDATA   ISP_AE_MEM_AVER_GB_B_RDATA ; /* 0x220ac */
    volatile U_ISP_AE_MEM_WEIGHT_WADDR   ISP_AE_MEM_WEIGHT_WADDR      ; /* 0x220b0 */
    volatile U_ISP_AE_MEM_WEIGHT_WDATA   ISP_AE_MEM_WEIGHT_WDATA      ; /* 0x220b4 */
    volatile U_ISP_AE_MEM_WEIGHT_RADDR   ISP_AE_MEM_WEIGHT_RADDR      ; /* 0x220b8 */
    volatile U_ISP_AE_MEM_WEIGHT_RDATA   ISP_AE_MEM_WEIGHT_RDATA      ; /* 0x220bc */
    volatile unsigned int           reserved_41[8]                     ; /* 0x220c0~0x220dc */
    volatile U_ISP_AE_CTRL_F        ISP_AE_CTRL_F                     ; /* 0x220e0 */
    volatile U_ISP_AE_CTRL_I        ISP_AE_CTRL_I                     ; /* 0x220e4 */
    volatile unsigned int           reserved_42                     ; /* 0x220e8 */
    volatile U_ISP_AE_UPDATE        ISP_AE_UPDATE                     ; /* 0x220ec */
    volatile U_ISP_AE_SIZE          ISP_AE_SIZE                       ; /* 0x220f0 */
    volatile unsigned int           reserved_43[3]                     ; /* 0x220f4~0x220fc */
    volatile U_ISP_AWB_CFG          ISP_AWB_CFG                       ; /* 0x22100 */
    volatile unsigned int           reserved_44[2]                     ; /* 0x22104~0x22108 */
    volatile U_ISP_AWB_VERSION      ISP_AWB_VERSION                   ; /* 0x2210c */
    volatile U_ISP_AWB_ZONE         ISP_AWB_ZONE                      ; /* 0x22110 */
    volatile U_ISP_AWB_BITMOVE      ISP_AWB_BITMOVE                   ; /* 0x22114 */
    volatile U_ISP_AWB_THD_MIN      ISP_AWB_THD_MIN                   ; /* 0x22118 */
    volatile U_ISP_AWB_THD_MAX      ISP_AWB_THD_MAX                   ; /* 0x2211c */
    volatile U_ISP_AWB_CR_MM        ISP_AWB_CR_MM                     ; /* 0x22120 */
    volatile U_ISP_AWB_CB_MM        ISP_AWB_CB_MM                     ; /* 0x22124 */
    volatile U_ISP_AWB_OFFSET_COMP   ISP_AWB_OFFSET_COMP              ; /* 0x22128 */
    volatile unsigned int           reserved_45                     ; /* 0x2212c */
    volatile U_ISP_AWB_TOP_K        ISP_AWB_TOP_K                     ; /* 0x22130 */
    volatile U_ISP_AWB_TOP_B        ISP_AWB_TOP_B                     ; /* 0x22134 */
    volatile U_ISP_AWB_BOT_K        ISP_AWB_BOT_K                     ; /* 0x22138 */
    volatile U_ISP_AWB_BOT_B        ISP_AWB_BOT_B                     ; /* 0x2213c */
    volatile U_ISP_AWB_AVG_R        ISP_AWB_AVG_R                     ; /* 0x22140 */
    volatile U_ISP_AWB_AVG_G        ISP_AWB_AVG_G                     ; /* 0x22144 */
    volatile U_ISP_AWB_AVG_B        ISP_AWB_AVG_B                     ; /* 0x22148 */
    volatile U_ISP_AWB_CNT_ALL      ISP_AWB_CNT_ALL                   ; /* 0x2214c */
    volatile unsigned int           reserved_46[12]                     ; /* 0x22150~0x2217c */
    volatile U_ISP_AWB_STAT_WADDR   ISP_AWB_STAT_WADDR                ; /* 0x22180 */
    volatile U_ISP_AWB_STAT_WDATA   ISP_AWB_STAT_WDATA                ; /* 0x22184 */
    volatile U_ISP_AWB_STAT_RADDR   ISP_AWB_STAT_RADDR                ; /* 0x22188 */
    volatile U_ISP_AWB_STAT_RDATA   ISP_AWB_STAT_RDATA                ; /* 0x2218c */
    volatile unsigned int           reserved_47[20]                     ; /* 0x22190~0x221dc */
    volatile U_ISP_AWB_CTRL_F       ISP_AWB_CTRL_F                    ; /* 0x221e0 */
    volatile U_ISP_AWB_CTRL_I       ISP_AWB_CTRL_I                    ; /* 0x221e4 */
    volatile unsigned int           reserved_48                     ; /* 0x221e8 */
    volatile U_ISP_AWB_UPDATE       ISP_AWB_UPDATE                    ; /* 0x221ec */
    volatile U_ISP_AWB_SIZE         ISP_AWB_SIZE                      ; /* 0x221f0 */
    volatile unsigned int           reserved_49[131]                     ; /* 0x221f4~0x223fc */
    volatile U_ISP_DIS_CFG          ISP_DIS_CFG                       ; /* 0x22400 */
    volatile U_ISP_DIS_BLK          ISP_DIS_BLK                       ; /* 0x22404 */
    volatile unsigned int           reserved_50                     ; /* 0x22408 */
    volatile U_ISP_DIS_VERSION      ISP_DIS_VERSION                   ; /* 0x2240c */
    volatile U_ISP_DIS_V0POS        ISP_DIS_V0POS                     ; /* 0x22410 */
    volatile U_ISP_DIS_V4POS        ISP_DIS_V4POS                     ; /* 0x22414 */
    volatile U_ISP_DIS_V8POS        ISP_DIS_V8POS                     ; /* 0x22418 */
    volatile unsigned int           reserved_51                     ; /* 0x2241c */
    volatile U_ISP_DIS_V0POSE       ISP_DIS_V0POSE                    ; /* 0x22420 */
    volatile U_ISP_DIS_V4POSE       ISP_DIS_V4POSE                    ; /* 0x22424 */
    volatile U_ISP_DIS_V8POSE       ISP_DIS_V8POSE                    ; /* 0x22428 */
    volatile unsigned int           reserved_52                     ; /* 0x2242c */
    volatile U_ISP_DIS_H0POS        ISP_DIS_H0POS                     ; /* 0x22430 */
    volatile U_ISP_DIS_H4POS        ISP_DIS_H4POS                     ; /* 0x22434 */
    volatile U_ISP_DIS_H8POS        ISP_DIS_H8POS                     ; /* 0x22438 */
    volatile unsigned int           reserved_53                     ; /* 0x2243c */
    volatile U_ISP_DIS_H0POSE       ISP_DIS_H0POSE                    ; /* 0x22440 */
    volatile U_ISP_DIS_H4POSE       ISP_DIS_H4POSE                    ; /* 0x22444 */
    volatile U_ISP_DIS_H8POSE       ISP_DIS_H8POSE                    ; /* 0x22448 */
    volatile unsigned int           reserved_54                     ; /* 0x2244c */
    volatile U_ISP_DIS_RAW_LUMA     ISP_DIS_RAW_LUMA                  ; /* 0x22450 */
    volatile U_ISP_DIS_GAMMA_EN     ISP_DIS_GAMMA_EN                  ; /* 0x22454 */
    volatile unsigned int           reserved_55[10]                     ; /* 0x22458~0x2247c */
    volatile U_ISP_DIS_H_STAT_WADDR   ISP_DIS_H_STAT_WADDR            ; /* 0x22480 */
    volatile U_ISP_DIS_H_STAT_WDATA   ISP_DIS_H_STAT_WDATA            ; /* 0x22484 */
    volatile U_ISP_DIS_H_STAT_RADDR   ISP_DIS_H_STAT_RADDR            ; /* 0x22488 */
    volatile U_ISP_DIS_H_STAT_RDATA   ISP_DIS_H_STAT_RDATA            ; /* 0x2248c */
    volatile U_ISP_DIS_V_STAT_WADDR   ISP_DIS_V_STAT_WADDR            ; /* 0x22490 */
    volatile U_ISP_DIS_V_STAT_WDATA   ISP_DIS_V_STAT_WDATA            ; /* 0x22494 */
    volatile U_ISP_DIS_V_STAT_RADDR   ISP_DIS_V_STAT_RADDR            ; /* 0x22498 */
    volatile U_ISP_DIS_V_STAT_RDATA   ISP_DIS_V_STAT_RDATA            ; /* 0x2249c */
    volatile unsigned int           reserved_56[16]                     ; /* 0x224a0~0x224dc */
    volatile U_ISP_DIS_CTRL_F       ISP_DIS_CTRL_F                    ; /* 0x224e0 */
    volatile U_ISP_DIS_CTRL_I       ISP_DIS_CTRL_I                    ; /* 0x224e4 */
    volatile unsigned int           reserved_57                     ; /* 0x224e8 */
    volatile U_ISP_DIS_UPDATE       ISP_DIS_UPDATE                    ; /* 0x224ec */
    volatile unsigned int           reserved_58[4]                     ; /* 0x224f0~0x224fc */
    volatile U_ISP_MG_CFG           ISP_MG_CFG                        ; /* 0x22500 */
    volatile unsigned int           reserved_60[2]                     ; /* 0x22504~0x22508 */
    volatile U_ISP_MG_VERSION       ISP_MG_VERSION                    ; /* 0x2250c */
    volatile U_ISP_MG_ZONE          ISP_MG_ZONE                       ; /* 0x22510 */
    volatile unsigned int           reserved_61[11]                     ; /* 0x22514~0x2253c */
    volatile U_ISP_MG_BITMOVE       ISP_MG_BITMOVE                    ; /* 0x22540 */
    volatile U_ISP_MG_OFFSET_R      ISP_MG_OFFSET_R                   ; /* 0x22544 */
    volatile U_ISP_MG_OFFSET_GR     ISP_MG_OFFSET_GR                  ; /* 0x22548 */
    volatile U_ISP_MG_OFFSET_GB     ISP_MG_OFFSET_GB                  ; /* 0x2254c */
    volatile U_ISP_MG_OFFSET_B      ISP_MG_OFFSET_B                   ; /* 0x22550 */
    volatile unsigned int           reserved_62[3]                     ; /* 0x22554~0x2255c */
    volatile U_ISP_MG_GAMMA_LIMIT   ISP_MG_GAMMA_LIMIT                ; /* 0x22560 */
    volatile unsigned int           reserved_63[11]                     ; /* 0x22564~0x2258c */
    volatile U_ISP_MG_MEM_AVER_WADDR   ISP_MG_MEM_AVER_WADDR          ; /* 0x22590 */
    volatile U_ISP_MG_MEM_AVER_WDATA   ISP_MG_MEM_AVER_WDATA          ; /* 0x22594 */
    volatile U_ISP_MG_MEM_AVER_RADDR   ISP_MG_MEM_AVER_RADDR          ; /* 0x22598 */
    volatile U_ISP_MG_MEM_AVER_RDATA   ISP_MG_MEM_AVER_RDATA          ; /* 0x2259c */
    volatile unsigned int           reserved_64[16]                     ; /* 0x225a0~0x225dc */
    volatile U_ISP_MG_CTRL_F        ISP_MG_CTRL_F                     ; /* 0x225e0 */
    volatile U_ISP_MG_CTRL_I        ISP_MG_CTRL_I                     ; /* 0x225e4 */
    volatile unsigned int           reserved_65                     ; /* 0x225e8 */
    volatile U_ISP_MG_UPDATE        ISP_MG_UPDATE                     ; /* 0x225ec */
    volatile U_ISP_MG_SIZE          ISP_MG_SIZE                       ; /* 0x225f0 */
    volatile unsigned int           reserved_66[3]                     ; /* 0x225f4~0x225fc */
    volatile U_ISP_FLICK_CFG        ISP_FLICK_CFG                     ; /* 0x22600 */
    volatile unsigned int           reserved_66_1[3]                     ; /* 0x22604~0x2260c */
    volatile U_ISP_FLICK_FRM_MERGE   ISP_FLICK_FRM_MERGE              ; /* 0x22610 */
    volatile U_ISP_FLICK_BLC_IN     ISP_FLICK_BLC_IN                  ; /* 0x22614 */
    volatile U_ISP_FLICK_EXP_RAT    ISP_FLICK_EXP_RAT                 ; /* 0x22618 */
    volatile U_ISP_FLICK_OVER_TH    ISP_FLICK_OVER_TH                 ; /* 0x2261c */
    volatile U_ISP_FLICK_PRE_AVG_GR   ISP_FLICK_PRE_AVG_GR            ; /* 0x22620 */
    volatile U_ISP_FLICK_PRE_AVG_GB   ISP_FLICK_PRE_AVG_GB            ; /* 0x22624 */
    volatile U_ISP_FLICK_CUR_SUM_GR   ISP_FLICK_CUR_SUM_GR            ; /* 0x22628 */
    volatile U_ISP_FLICK_CUR_SUM_GB   ISP_FLICK_CUR_SUM_GB            ; /* 0x2262c */
    volatile U_ISP_FLICK_CUR_ABS_GR   ISP_FLICK_CUR_ABS_GR            ; /* 0x22630 */
    volatile U_ISP_FLICK_CUR_ABS_GB   ISP_FLICK_CUR_ABS_GB            ; /* 0x22634 */
    volatile unsigned int           reserved_67[18]                     ; /* 0x22638~0x2267c */
    volatile U_ISP_FLICK_PGMEAN_WADDR   ISP_FLICK_PGMEAN_WADDR        ; /* 0x22680 */
    volatile U_ISP_FLICK_PGMEAN_WDATA   ISP_FLICK_PGMEAN_WDATA        ; /* 0x22684 */
    volatile U_ISP_FLICK_PGMEAN_RADDR   ISP_FLICK_PGMEAN_RADDR        ; /* 0x22688 */
    volatile U_ISP_FLICK_PGMEAN_RDATA   ISP_FLICK_PGMEAN_RDATA        ; /* 0x2268c */
    volatile unsigned int           reserved_68[20]                     ; /* 0x22690~0x226dc */
    volatile U_ISP_FLICK_CTRL_F     ISP_FLICK_CTRL_F                  ; /* 0x226e0 */
    volatile U_ISP_FLICK_CTRL_I     ISP_FLICK_CTRL_I                  ; /* 0x226e4 */
    volatile unsigned int           reserved_69                     ; /* 0x226e8 */
    volatile U_ISP_FLICK_UPDATE     ISP_FLICK_UPDATE                  ; /* 0x226ec */
    volatile U_ISP_FLICK_SIZE       ISP_FLICK_SIZE                    ; /* 0x226f0 */
    volatile unsigned int           reserved_70[579]                     ; /* 0x226f4~0x22ffc */
    volatile U_ISP_LSC_CFG          ISP_LSC_CFG                       ; /* 0x23000 */
    volatile unsigned int           reserved_71[3]                     ; /* 0x23004~0x2300c */
    volatile U_ISP_LSC_WINNUM       ISP_LSC_WINNUM                    ; /* 0x23010 */
	volatile U_ISP_LSC_WINX         ISP_LSC_WINX[8]                   ; /* 0x23014 */
    volatile U_ISP_LSC_WINY           ISP_LSC_WINY[8]                   ; /* 0x23034 */
    volatile U_ISP_LSC_BLCEN        ISP_LSC_BLCEN                     ; /* 0x23054 */
    volatile U_ISP_LSC_RBLC         ISP_LSC_RBLC                      ; /* 0x23058 */
    volatile U_ISP_LSC_GRBLC        ISP_LSC_GRBLC                     ; /* 0x2305c */
    volatile U_ISP_LSC_GBBLC        ISP_LSC_GBBLC                     ; /* 0x23060 */
    volatile U_ISP_LSC_BBLC         ISP_LSC_BBLC                      ; /* 0x23064 */
    volatile U_ISP_LSC_LUT_UPDATE   ISP_LSC_LUT_UPDATE                ; /* 0x23068 */
    volatile unsigned int           reserved_72[5]                     ; /* 0x2306c~0x2307c */
    volatile U_ISP_LSC_GRRGAIN_WADDR   ISP_LSC_GRRGAIN_WADDR          ; /* 0x23080 */
    volatile U_ISP_LSC_GRRGAIN_WDATA   ISP_LSC_GRRGAIN_WDATA          ; /* 0x23084 */
    volatile U_ISP_LSC_GRRGAIN_RADDR   ISP_LSC_GRRGAIN_RADDR          ; /* 0x23088 */
    volatile U_ISP_LSC_GRRGAIN_RDATA   ISP_LSC_GRRGAIN_RDATA          ; /* 0x2308c */
    volatile U_ISP_LSC_GBBGAIN_WADDR   ISP_LSC_GBBGAIN_WADDR          ; /* 0x23090 */
    volatile U_ISP_LSC_GBBGAIN_WDATA   ISP_LSC_GBBGAIN_WDATA          ; /* 0x23094 */
    volatile U_ISP_LSC_GBBGAIN_RADDR   ISP_LSC_GBBGAIN_RADDR          ; /* 0x23098 */
    volatile U_ISP_LSC_GBBGAIN_RDATA   ISP_LSC_GBBGAIN_RDATA          ; /* 0x2309c */
    volatile unsigned int           reserved_73[16]                     ; /* 0x230a0~0x230dc */
    volatile U_ISP_LSC_CTRL_F       ISP_LSC_CTRL_F                    ; /* 0x230e0 */
    volatile U_ISP_LSC_CTRL_I       ISP_LSC_CTRL_I                    ; /* 0x230e4 */
    volatile unsigned int           reserved_74                     ; /* 0x230e8 */
    volatile U_ISP_LSC_UPDATE       ISP_LSC_UPDATE                    ; /* 0x230ec */
    volatile unsigned int           reserved_75[516]                     ; /* 0x230f0~0x238fc */
    volatile U_ISP_GE_CFG           ISP_GE_CFG                        ; /* 0x23900 */
    volatile unsigned int           reserved_76[2]                     ; /* 0x23904~0x23908 */
    volatile U_ISP_GE_VERSION       ISP_GE_VERSION                    ; /* 0x2390c */
    volatile U_ISP_GE0_CT_TH1       ISP_GE0_CT_TH1                    ; /* 0x23910 */
    volatile U_ISP_GE0_CT_TH2       ISP_GE0_CT_TH2                    ; /* 0x23914 */
    volatile U_ISP_GE0_CT_TH3       ISP_GE0_CT_TH3                    ; /* 0x23918 */
    volatile U_ISP_GE0_CT_SLOPE     ISP_GE0_CT_SLOPE                  ; /* 0x2391c */
    volatile U_ISP_GE1_CT_TH1       ISP_GE1_CT_TH1                    ; /* 0x23920 */
    volatile U_ISP_GE1_CT_TH2       ISP_GE1_CT_TH2                    ; /* 0x23924 */
    volatile U_ISP_GE1_CT_TH3       ISP_GE1_CT_TH3                    ; /* 0x23928 */
    volatile U_ISP_GE1_CT_SLOPE     ISP_GE1_CT_SLOPE                  ; /* 0x2392c */
    volatile U_ISP_GE2_CT_TH1       ISP_GE2_CT_TH1                    ; /* 0x23930 */
    volatile U_ISP_GE2_CT_TH2       ISP_GE2_CT_TH2                    ; /* 0x23934 */
    volatile U_ISP_GE2_CT_TH3       ISP_GE2_CT_TH3                    ; /* 0x23938 */
    volatile U_ISP_GE2_CT_SLOPE     ISP_GE2_CT_SLOPE                  ; /* 0x2393c */
    volatile U_ISP_GE3_CT_TH1       ISP_GE3_CT_TH1                    ; /* 0x23940 */
    volatile U_ISP_GE3_CT_TH2       ISP_GE3_CT_TH2                    ; /* 0x23944 */
    volatile U_ISP_GE3_CT_TH3       ISP_GE3_CT_TH3                    ; /* 0x23948 */
    volatile U_ISP_GE3_CT_SLOPE     ISP_GE3_CT_SLOPE                  ; /* 0x2394c */
    volatile U_ISP_GE_ZONE          ISP_GE_ZONE                       ; /* 0x23950 */
    volatile U_ISP_GE_MODE          ISP_GE_MODE                       ; /* 0x23954 */
    volatile U_ISP_GE_STRENGTH      ISP_GE_STRENGTH                   ; /* 0x23958 */
    volatile unsigned int           reserved_77[9]                     ; /* 0x2395c~0x2397c */
    volatile U_ISP_GE_MEM_AVER_WADDR0   ISP_GE_MEM_AVER_WADDR0        ; /* 0x23980 */
    volatile U_ISP_GE_MEM_AVER_WDATA0   ISP_GE_MEM_AVER_WDATA0        ; /* 0x23984 */
    volatile U_ISP_GE_MEM_AVER_RADDR0   ISP_GE_MEM_AVER_RADDR0        ; /* 0x23988 */
    volatile U_ISP_GE_MEM_AVER_RDATA0   ISP_GE_MEM_AVER_RDATA0        ; /* 0x2398c */
    volatile U_ISP_GE_MEM_AVER_WADDR1   ISP_GE_MEM_AVER_WADDR1        ; /* 0x23990 */
    volatile U_ISP_GE_MEM_AVER_WDATA1   ISP_GE_MEM_AVER_WDATA1        ; /* 0x23994 */
    volatile U_ISP_GE_MEM_AVER_RADDR1   ISP_GE_MEM_AVER_RADDR1        ; /* 0x23998 */
    volatile U_ISP_GE_MEM_AVER_RDATA1   ISP_GE_MEM_AVER_RDATA1        ; /* 0x2399c */
    volatile unsigned int           reserved_78[16]                     ; /* 0x239a0~0x239dc */
    volatile U_ISP_GE_CTRL_F        ISP_GE_CTRL_F                     ; /* 0x239e0 */
    volatile U_ISP_GE_CTRL_I        ISP_GE_CTRL_I                     ; /* 0x239e4 */
    volatile U_ISP_GE_TIMING        ISP_GE_TIMING                     ; /* 0x239e8 */
    volatile U_ISP_GE_UPDATE        ISP_GE_UPDATE                     ; /* 0x239ec */
    volatile U_ISP_GE_SIZE          ISP_GE_SIZE                       ; /* 0x239f0 */
    volatile unsigned int           reserved_79[3]                     ; /* 0x239f4~0x239fc */
    volatile U_ISP_FPN_CFG          ISP_FPN_CFG                       ; /* 0x23a00 */
    volatile U_ISP_FPN_CALIB_START   ISP_FPN_CALIB_START              ; /* 0x23a04 */
    volatile U_ISP_FPN_CORR_CFG     ISP_FPN_CORR_CFG                  ; /* 0x23a08 */
    volatile U_ISP_FPN_STAT         ISP_FPN_STAT                      ; /* 0x23a0c */
    volatile U_ISP_FPN_WHITE_LEVEL   ISP_FPN_WHITE_LEVEL              ; /* 0x23a10 */
    volatile unsigned int           reserved_80                     ; /* 0x23a14 */
    volatile U_ISP_FPN_DIVCOEF      ISP_FPN_DIVCOEF                   ; /* 0x23a18 */
    volatile U_ISP_FPN_FRAMELOG2    ISP_FPN_FRAMELOG2                 ; /* 0x23a1c */
    volatile U_ISP_FPN_SUM0         ISP_FPN_SUM0                      ; /* 0x23a20 */
    volatile U_ISP_FPN_SUM1         ISP_FPN_SUM1                      ; /* 0x23a24 */
    volatile unsigned int           reserved_81[2]                     ; /* 0x23a28~0x23a2c */
    volatile U_ISP_FPN_CORR0        ISP_FPN_CORR0                     ; /* 0x23a30 */
    volatile U_ISP_FPN_CORR1        ISP_FPN_CORR1                     ; /* 0x23a34 */
    volatile U_ISP_FPN_CORR2        ISP_FPN_CORR2                     ; /* 0x23a38 */
    volatile U_ISP_FPN_CORR3        ISP_FPN_CORR3                     ; /* 0x23a3c */
    volatile U_ISP_FPN_SHIFT        ISP_FPN_SHIFT                     ; /* 0x23a40 */
    volatile unsigned int           reserved_82[3]                     ; /* 0x23a44~0x23a4c */
    volatile U_ISP_FPN_MAX_O        ISP_FPN_MAX_O                     ; /* 0x23a50 */
    volatile U_ISP_FPN_OVERFLOWTHR   ISP_FPN_OVERFLOWTHR              ; /* 0x23a54 */
    volatile unsigned int           reserved_83                     ; /* 0x23a58 */
    volatile U_ISP_FPN_RESERVED     ISP_FPN_RESERVED                  ; /* 0x23a5c */
    volatile unsigned int           reserved_84[8]                     ; /* 0x23a60~0x23a7c */
    volatile U_ISP_FPN_LINE_WADDR   ISP_FPN_LINE_WADDR                ; /* 0x23a80 */
    volatile U_ISP_FPN_LINE_WDATA   ISP_FPN_LINE_WDATA                ; /* 0x23a84 */
    volatile U_ISP_FPN_LINE_RADDR   ISP_FPN_LINE_RADDR                ; /* 0x23a88 */
    volatile U_ISP_FPN_LINE_RDATA   ISP_FPN_LINE_RDATA                ; /* 0x23a8c */
    volatile unsigned int           reserved_85[20]                     ; /* 0x23a90~0x23adc */
    volatile U_ISP_FPN_CTRL_F       ISP_FPN_CTRL_F                    ; /* 0x23ae0 */
    volatile U_ISP_FPN_CTRL_I       ISP_FPN_CTRL_I                    ; /* 0x23ae4 */
    volatile U_ISP_FPN_TIMING       ISP_FPN_TIMING                    ; /* 0x23ae8 */
    volatile U_ISP_FPN_UPDATE       ISP_FPN_UPDATE                    ; /* 0x23aec */
    volatile U_ISP_FPN_SIZE         ISP_FPN_SIZE                      ; /* 0x23af0 */
    volatile unsigned int           reserved_86[67]                     ; /* 0x23af4~0x23bfc */
    volatile U_ISP_DPC_CFG          ISP_DPC_CFG                       ; /* 0x23c00 */
    volatile U_ISP_DCG_MODE         ISP_DCG_MODE                      ; /* 0x23c04 */
    volatile U_ISP_DPC_ALPHA        ISP_DPC_ALPHA                     ; /* 0x23c08 */
    volatile U_ISP_DPC_VERSION      ISP_DPC_VERSION                   ; /* 0x23c0c */
    volatile U_ISP_DPC_MODE         ISP_DPC_MODE                      ; /* 0x23c10 */
    volatile U_ISP_DPC_OUTPUT_MODE   ISP_DPC_OUTPUT_MODE              ; /* 0x23c14 */
    volatile U_ISP_DPC_SET_USE      ISP_DPC_SET_USE                   ; /* 0x23c18 */
    volatile U_ISP_DPC_METHODS_SET_1   ISP_DPC_METHODS_SET_1          ; /* 0x23c1c */
    volatile U_ISP_DPC_METHODS_SET_2   ISP_DPC_METHODS_SET_2          ; /* 0x23c20 */
    volatile U_ISP_DPC_METHODS_SET_3   ISP_DPC_METHODS_SET_3          ; /* 0x23c24 */
    volatile U_ISP_DPC_LINE_THRESH_1   ISP_DPC_LINE_THRESH_1          ; /* 0x23c28 */
    volatile U_ISP_DPC_LINE_MAD_FAC_1   ISP_DPC_LINE_MAD_FAC_1        ; /* 0x23c2c */
    volatile U_ISP_DPC_PG_FAC_1     ISP_DPC_PG_FAC_1                  ; /* 0x23c30 */
    volatile U_ISP_DPC_RND_THRESH_1   ISP_DPC_RND_THRESH_1            ; /* 0x23c34 */
    volatile U_ISP_DPC_RG_FAC_1     ISP_DPC_RG_FAC_1                  ; /* 0x23c38 */
    volatile U_ISP_DPC_LINE_THRESH_2   ISP_DPC_LINE_THRESH_2          ; /* 0x23c3c */
    volatile U_ISP_DPC_LINE_MAD_FAC_2   ISP_DPC_LINE_MAD_FAC_2        ; /* 0x23c40 */
    volatile U_ISP_DPC_PG_FAC_2     ISP_DPC_PG_FAC_2                  ; /* 0x23c44 */
    volatile U_ISP_DPC_RND_THRESH_2   ISP_DPC_RND_THRESH_2            ; /* 0x23c48 */
    volatile U_ISP_DPC_RG_FAC_2     ISP_DPC_RG_FAC_2                  ; /* 0x23c4c */
    volatile U_ISP_DPC_LINE_THRESH_3   ISP_DPC_LINE_THRESH_3          ; /* 0x23c50 */
    volatile U_ISP_DPC_LINE_MAD_FAC_3   ISP_DPC_LINE_MAD_FAC_3        ; /* 0x23c54 */
    volatile U_ISP_DPC_PG_FAC_3     ISP_DPC_PG_FAC_3                  ; /* 0x23c58 */
    volatile U_ISP_DPC_RND_THRESH_3   ISP_DPC_RND_THRESH_3            ; /* 0x23c5c */
    volatile U_ISP_DPC_RG_FAC_3     ISP_DPC_RG_FAC_3                  ; /* 0x23c60 */
    volatile U_ISP_DPC_RO_LIMITS    ISP_DPC_RO_LIMITS                 ; /* 0x23c64 */
    volatile U_ISP_DPC_RND_OFFS     ISP_DPC_RND_OFFS                  ; /* 0x23c68 */
    volatile U_ISP_DPC_BPT_CTRL     ISP_DPC_BPT_CTRL                  ; /* 0x23c6c */
    volatile U_ISP_DPC_BPT_NUMBER   ISP_DPC_BPT_NUMBER                ; /* 0x23c70 */
    volatile U_ISP_DPC_BPT_CALIB_NUMBER   ISP_DPC_BPT_CALIB_NUMBER    ; /* 0x23c74 */
    volatile U_ISP_DPC_RESERVED     ISP_DPC_RESERVED                  ; /* 0x23c78 */
    volatile U_ISP_DPC_BPT_THRD     ISP_DPC_BPT_THRD                  ; /* 0x23c7c */
    volatile U_ISP_DPC_BPT_WADDR    ISP_DPC_BPT_WADDR                 ; /* 0x23c80 */
    volatile U_ISP_DPC_BPT_WDATA    ISP_DPC_BPT_WDATA                 ; /* 0x23c84 */
    volatile U_ISP_DPC_BPT_RADDR    ISP_DPC_BPT_RADDR                 ; /* 0x23c88 */
    volatile U_ISP_DPC_BPT_RDATA    ISP_DPC_BPT_RDATA                 ; /* 0x23c8c */
    volatile unsigned int           reserved_87[20]                     ; /* 0x23c90~0x23cdc */
    volatile U_ISP_DPC_CTRL_F       ISP_DPC_CTRL_F                    ; /* 0x23ce0 */
    volatile U_ISP_DPC_CTRL_I       ISP_DPC_CTRL_I                    ; /* 0x23ce4 */
    volatile U_ISP_DPC_TIMING       ISP_DPC_TIMING                    ; /* 0x23ce8 */
    volatile U_ISP_DPC_UPDATE       ISP_DPC_UPDATE                    ; /* 0x23cec */
    volatile U_ISP_DPC_SIZE         ISP_DPC_SIZE                      ; /* 0x23cf0 */
    volatile unsigned int           reserved_88[7]                     ; /* 0x23cf4~0x23d0c */
    volatile U_ISP_DPC_SOFT_THR     ISP_DPC_SOFT_THR                  ; /* 0x23d10 */
	volatile U_ISP_DPC_HARDTHR_EN  ISP_DPC_HARDTHR_EN				  ;/*0x23D14*/
    volatile U_ISP_DPC_RAKERATIO    ISP_DPC_RAKERATIO                 ; /* 0x23d18 */
    volatile unsigned int           reserved_89[50]                     ; /* 0x23d1c~0x23de0 */
    volatile U_ISP_DPC_EX_CTRL_I    ISP_DPC_EX_CTRL_I                 ; /* 0x23de4 */
    volatile unsigned int           reserved_90                     ; /* 0x23de8 */
    volatile U_ISP_DPC_EX_UPDATE    ISP_DPC_EX_UPDATE                 ; /* 0x23dec */
    volatile unsigned int           reserved_91[1412]                     ; /* 0x23df0~0x253fc */
    volatile U_ISP_BNR_CFG          ISP_BNR_CFG                       ; /* 0x25400 */
    volatile unsigned int           reserved_92[2]                     ; /* 0x25404~0x25408 */
    volatile U_ISP_BNR_VERSION      ISP_BNR_VERSION                   ; /* 0x2540c */
    volatile U_ISP_BNR_CRATIO       ISP_BNR_CRATIO                    ; /* 0x25410 */
    volatile U_ISP_BNR_LEV          ISP_BNR_LEV                       ; /* 0x25414 */
    volatile U_ISP_BNR_SEL          ISP_BNR_SEL                       ; /* 0x25418 */
    volatile U_ISP_BNR_GAIN         ISP_BNR_GAIN                      ; /* 0x2541c */
    volatile U_ISP_BNR_SYMCOEF      ISP_BNR_SYMCOEF                   ; /* 0x25420 */
    volatile U_ISP_BNR_CORING       ISP_BNR_CORING                    ; /* 0x25424 */
    volatile U_ISP_BNR_FRMNUM       ISP_BNR_FRMNUM                    ; /* 0x25428 */
    volatile U_ISP_BNR_LUT_UPDATE   ISP_BNR_LUT_UPDATE                ; /* 0x2542c */
    volatile U_ISP_BNR_RLMT         ISP_BNR_RLMT                      ; /* 0x25430 */
    volatile U_ISP_BNR_SATU0        ISP_BNR_SATU0                     ; /* 0x25434 */
    volatile U_ISP_BNR_SATU1        ISP_BNR_SATU1                     ; /* 0x25438 */
    volatile U_ISP_BNR_SATU2        ISP_BNR_SATU2                     ; /* 0x2543c */
    volatile unsigned int           reserved_93[16]                     ; /* 0x25440~0x2547c */
    volatile U_ISP_BNR_RGGB_ODD_WADDR   ISP_BNR_RGGB_ODD_WADDR        ; /* 0x25480 */
    volatile U_ISP_BNR_RGGB_ODD_WDATA   ISP_BNR_RGGB_ODD_WDATA        ; /* 0x25484 */
    volatile U_ISP_BNR_RGGB_ODD_RADDR   ISP_BNR_RGGB_ODD_RADDR        ; /* 0x25488 */
    volatile U_ISP_BNR_RGGB_ODD_RDATA   ISP_BNR_RGGB_ODD_RDATA        ; /* 0x2548c */
    volatile U_ISP_BNR_GRBG_ODD_WADDR   ISP_BNR_GRBG_ODD_WADDR        ; /* 0x25490 */
    volatile U_ISP_BNR_GRBG_ODD_WDATA   ISP_BNR_GRBG_ODD_WDATA        ; /* 0x25494 */
    volatile U_ISP_BNR_GRBG_ODD_RADDR   ISP_BNR_GRBG_ODD_RADDR        ; /* 0x25498 */
    volatile U_ISP_BNR_GRBG_ODD_RDATA   ISP_BNR_GRBG_ODD_RDATA        ; /* 0x2549c */
    volatile U_ISP_BNR_GBRG_ODD_WADDR   ISP_BNR_GBRG_ODD_WADDR        ; /* 0x254a0 */
    volatile U_ISP_BNR_GBRG_ODD_WDATA   ISP_BNR_GBRG_ODD_WDATA        ; /* 0x254a4 */
    volatile U_ISP_BNR_GBRG_ODD_RADDR   ISP_BNR_GBRG_ODD_RADDR        ; /* 0x254a8 */
    volatile U_ISP_BNR_GBRG_ODD_RDATA   ISP_BNR_GBRG_ODD_RDATA        ; /* 0x254ac */
    volatile U_ISP_BNR_BGGR_ODD_WADDR   ISP_BNR_BGGR_ODD_WADDR        ; /* 0x254b0 */
    volatile U_ISP_BNR_BGGR_ODD_WDATA   ISP_BNR_BGGR_ODD_WDATA        ; /* 0x254b4 */
    volatile U_ISP_BNR_BGGR_ODD_RADDR   ISP_BNR_BGGR_ODD_RADDR        ; /* 0x254b8 */
    volatile U_ISP_BNR_BGGR_ODD_RDATA   ISP_BNR_BGGR_ODD_RDATA        ; /* 0x254bc */
    volatile U_ISP_BNR_RLMT_ODD_WADDR   ISP_BNR_RLMT_ODD_WADDR        ; /* 0x254c0 */
    volatile U_ISP_BNR_RLMT_ODD_WDATA   ISP_BNR_RLMT_ODD_WDATA        ; /* 0x254c4 */
    volatile U_ISP_BNR_RLMT_ODD_RADDR   ISP_BNR_RLMT_ODD_RADDR        ; /* 0x254c8 */
    volatile U_ISP_BNR_RLMT_ODD_RDATA   ISP_BNR_RLMT_ODD_RDATA        ; /* 0x254cc */
    volatile unsigned int           reserved_94[4]                     ; /* 0x254d0~0x254dc */
    volatile U_ISP_BNR_CTRL_F       ISP_BNR_CTRL_F                    ; /* 0x254e0 */
    volatile U_ISP_BNR_CTRL_I       ISP_BNR_CTRL_I                    ; /* 0x254e4 */
    volatile U_ISP_BNR_TIMING       ISP_BNR_TIMING                    ; /* 0x254e8 */
    volatile U_ISP_BNR_UPDATE       ISP_BNR_UPDATE                    ; /* 0x254ec */
    volatile U_ISP_BNR_SIZE         ISP_BNR_SIZE                      ; /* 0x254f0 */
    volatile unsigned int           reserved_95[35]                     ; /* 0x254f4~0x2557c */
    volatile U_ISP_BNR_RGGB_EVEN_WADDR   ISP_BNR_RGGB_EVEN_WADDR      ; /* 0x25580 */
    volatile U_ISP_BNR_RGGB_EVEN_WDATA   ISP_BNR_RGGB_EVEN_WDATA      ; /* 0x25584 */
    volatile U_ISP_BNR_RGGB_EVEN_RADDR   ISP_BNR_RGGB_EVEN_RADDR      ; /* 0x25588 */
    volatile U_ISP_BNR_RGGB_EVEN_RDATA   ISP_BNR_RGGB_EVEN_RDATA      ; /* 0x2558c */
    volatile U_ISP_BNR_GRBG_EVEN_WADDR   ISP_BNR_GRBG_EVEN_WADDR      ; /* 0x25590 */
    volatile U_ISP_BNR_GRBG_EVEN_WDATA   ISP_BNR_GRBG_EVEN_WDATA      ; /* 0x25594 */
    volatile U_ISP_BNR_GRBG_EVEN_RADDR   ISP_BNR_GRBG_EVEN_RADDR      ; /* 0x25598 */
    volatile U_ISP_BNR_GRBG_EVEN_RDATA   ISP_BNR_GRBG_EVEN_RDATA      ; /* 0x2559c */
    volatile U_ISP_BNR_GBRG_EVEN_WADDR   ISP_BNR_GBRG_EVEN_WADDR      ; /* 0x255a0 */
    volatile U_ISP_BNR_GBRG_EVEN_WDATA   ISP_BNR_GBRG_EVEN_WDATA      ; /* 0x255a4 */
    volatile U_ISP_BNR_GBRG_EVEN_RADDR   ISP_BNR_GBRG_EVEN_RADDR      ; /* 0x255a8 */
    volatile U_ISP_BNR_GBRG_EVEN_RDATA   ISP_BNR_GBRG_EVEN_RDATA      ; /* 0x255ac */
    volatile U_ISP_BNR_BGGR_EVEN_WADDR   ISP_BNR_BGGR_EVEN_WADDR      ; /* 0x255b0 */
    volatile U_ISP_BNR_BGGR_EVEN_WDATA   ISP_BNR_BGGR_EVEN_WDATA      ; /* 0x255b4 */
    volatile U_ISP_BNR_BGGR_EVEN_RADDR   ISP_BNR_BGGR_EVEN_RADDR      ; /* 0x255b8 */
    volatile U_ISP_BNR_BGGR_EVEN_RDATA   ISP_BNR_BGGR_EVEN_RDATA      ; /* 0x255bc */
    volatile U_ISP_BNR_RLMT_EVEN_WADDR   ISP_BNR_RLMT_EVEN_WADDR      ; /* 0x255c0 */
    volatile U_ISP_BNR_RLMT_EVEN_WDATA   ISP_BNR_RLMT_EVEN_WDATA      ; /* 0x255c4 */
    volatile U_ISP_BNR_RLMT_EVEN_RADDR   ISP_BNR_RLMT_EVEN_RADDR      ; /* 0x255c8 */
    volatile U_ISP_BNR_RLMT_EVEN_RDATA   ISP_BNR_RLMT_EVEN_RDATA      ; /* 0x255cc */
    volatile unsigned int           reserved_96[652]                     ; /* 0x255d0~0x25ffc */
    volatile U_ISP_WDR_CFG          ISP_WDR_CFG                       ; /* 0x26000 */
    volatile unsigned int           reserved_97[3]                     ; /* 0x26004~0x2600c */
    volatile U_ISP_WDR_INBLC        ISP_WDR_INBLC                     ; /* 0x26010 */
    volatile U_ISP_WDR_OUTBLC       ISP_WDR_OUTBLC                    ; /* 0x26014 */
    volatile U_ISP_WDR_EXPOSRATIOS   ISP_WDR_EXPOSRATIOS              ; /* 0x26018 */
    volatile U_ISP_WDR_BLCSAVE      ISP_WDR_BLCSAVE                   ; /* 0x2601c */
    volatile U_ISP_WDR_ZOOMBLC      ISP_WDR_ZOOMBLC                   ; /* 0x26020 */
    volatile U_ISP_WDR_BLENDRATIO   ISP_WDR_BLENDRATIO                ; /* 0x26024 */
    volatile U_ISP_WDR_FLRGTTH      ISP_WDR_FLRGTTH                   ; /* 0x26028 */
    volatile U_ISP_WDR_FLDFTWGT     ISP_WDR_FLDFTWGT                  ; /* 0x2602c */
    volatile U_ISP_WDR_MDT_CTRL     ISP_WDR_MDT_CTRL                  ; /* 0x26030 */
    volatile U_ISP_WDR_BLDRMDTMAX   ISP_WDR_BLDRMDTMAX                ; /* 0x26034 */
    volatile U_ISP_WDR_NOSCLIPTH    ISP_WDR_NOSCLIPTH                 ; /* 0x26038 */
    volatile U_ISP_WDR_NOSFACTOR    ISP_WDR_NOSFACTOR                 ; /* 0x2603c */
    volatile U_ISP_WDR_LUT_UPDATE   ISP_WDR_LUT_UPDATE                ; /* 0x26040 */
    volatile U_ISP_WDR_FSNRJUDGE    ISP_WDR_FSNRJUDGE                 ; /* 0x26044 */
    volatile U_ISP_WDR_FSNRTH       ISP_WDR_FSNRTH                    ; /* 0x26048 */
    volatile U_ISP_WDR_FSNRGN       ISP_WDR_FSNRGN                    ; /* 0x2604c */
    volatile U_ISP_WDR_MDT_REFNOS   ISP_WDR_MDT_REFNOS                ; /* 0x26050 */
    volatile unsigned int           reserved_98[11]                     ; /* 0x26054~0x2607c */
    volatile U_ISP_WDR_NOSLUT_WADDR   ISP_WDR_NOSLUT_WADDR            ; /* 0x26080 */
    volatile U_ISP_WDR_NOSLUT_WDATA   ISP_WDR_NOSLUT_WDATA            ; /* 0x26084 */
    volatile U_ISP_WDR_NOSLUT_RADDR   ISP_WDR_NOSLUT_RADDR            ; /* 0x26088 */
    volatile U_ISP_WDR_NOSLUT_RDATA   ISP_WDR_NOSLUT_RDATA            ; /* 0x2608c */
    volatile unsigned int           reserved_99[20]                     ; /* 0x26090~0x260dc */
    volatile U_ISP_WDR_CTRL_F       ISP_WDR_CTRL_F                    ; /* 0x260e0 */
    volatile U_ISP_WDR_CTRL_I       ISP_WDR_CTRL_I                    ; /* 0x260e4 */
    volatile U_ISP_WDR_TIMING       ISP_WDR_TIMING                    ; /* 0x260e8 */
    volatile U_ISP_WDR_UPDATE       ISP_WDR_UPDATE                    ; /* 0x260ec */
    volatile U_ISP_WDR_SIZE         ISP_WDR_SIZE                      ; /* 0x260f0 */
    volatile unsigned int           reserved_100[67]                     ; /* 0x260f4~0x261fc */
    volatile U_ISP_DRC_CFG          ISP_DRC_CFG                       ; /* 0x26200 */
    volatile unsigned int           reserved_101[2]                     ; /* 0x26204~0x26208 */
    volatile U_ISP_DRC_VERSION      ISP_DRC_VERSION                   ; /* 0x2620c */
    volatile U_ISP_DRC_ZONE         ISP_DRC_ZONE                      ; /* 0x26210 */
    volatile U_ISP_DRC_ZONE_SIZE    ISP_DRC_ZONE_SIZE                 ; /* 0x26214 */
    volatile U_ISP_DRC_ZONE_DIV0    ISP_DRC_ZONE_DIV0                 ; /* 0x26218 */
    volatile U_ISP_DRC_ZONE_DIV1    ISP_DRC_ZONE_DIV1                 ; /* 0x2621c */
    volatile U_ISP_DRC_BIN          ISP_DRC_BIN                       ; /* 0x26220 */
    volatile U_ISP_DRC_BIN_DIV      ISP_DRC_BIN_DIV                   ; /* 0x26224 */
    volatile U_ISP_DRC_BIN_SCALE    ISP_DRC_BIN_SCALE                 ; /* 0x26228 */
    volatile U_ISP_DRC_RANGE_FLT_COEF   ISP_DRC_RANGE_FLT_COEF        ; /* 0x2622c */
    volatile U_ISP_DRC_EXPOSURE     ISP_DRC_EXPOSURE                  ; /* 0x26230 */
    volatile U_ISP_DRC_LOCAL_EDGE_LMT   ISP_DRC_LOCAL_EDGE_LMT        ; /* 0x26234 */
    volatile U_ISP_DRC_STRENGTH     ISP_DRC_STRENGTH                  ; /* 0x26238 */
    volatile U_ISP_DRC_DETAIL_GAIN   ISP_DRC_DETAIL_GAIN              ; /* 0x2623c */
    volatile U_ISP_DRC_DARK_GAIN_LMT_Y   ISP_DRC_DARK_GAIN_LMT_Y      ; /* 0x26240 */
    volatile U_ISP_DRC_DARK_GAIN_LMT_C   ISP_DRC_DARK_GAIN_LMT_C      ; /* 0x26244 */
    volatile U_ISP_DRC_BRIGHT_GAIN_LMT   ISP_DRC_BRIGHT_GAIN_LMT      ; /* 0x26248 */
    volatile U_ISP_DRC_RGB_WGT      ISP_DRC_RGB_WGT                   ; /* 0x2624c */
    volatile unsigned int           reserved_103_1                     ; /* 0x26250 */
    volatile U_ISP_DRC_LUT_MIX_CRTL   ISP_DRC_LUT_MIX_CRTL            ; /* 0x26254 */
    volatile U_ISP_DRC_GAIN_CLIP    ISP_DRC_GAIN_CLIP                 ; /* 0x26258 */
    volatile U_ISP_DRC_COLOR_CTRL   ISP_DRC_COLOR_CTRL                ; /* 0x2625c */
    volatile U_ISP_DRC_GLOBAL_CORR   ISP_DRC_GLOBAL_CORR              ; /* 0x26260 */
    volatile U_ISP_DRC_MIXING_CORING   ISP_DRC_MIXING_CORING          ; /* 0x26264 */
    volatile U_ISP_DRC_MIXING_DARK   ISP_DRC_MIXING_DARK              ; /* 0x26268 */
    volatile U_ISP_DRC_MIXING_BRIGHT   ISP_DRC_MIXING_BRIGHT          ; /* 0x2626c */
    volatile U_ISP_DRC_RG_CTRL      ISP_DRC_RG_CTRL                   ; /* 0x26270 */
    volatile U_ISP_DRC_BG_CTRL      ISP_DRC_BG_CTRL                   ; /* 0x26274 */
    volatile U_ISP_DRC_PDW          ISP_DRC_PDW                       ; /* 0x26278 */
    volatile U_ISP_DRC_PPDTC_SUM    ISP_DRC_PPDTC_SUM                 ; /* 0x2627c */
    volatile U_ISP_DRC_STAT_IND_WADDR0   ISP_DRC_STAT_IND_WADDR0      ; /* 0x26280 */
    volatile U_ISP_DRC_STAT_IND_WDATA0   ISP_DRC_STAT_IND_WDATA0      ; /* 0x26284 */
    volatile U_ISP_DRC_STAT_IND_RADDR0   ISP_DRC_STAT_IND_RADDR0      ; /* 0x26288 */
    volatile U_ISP_DRC_STAT_IND_RDATA0   ISP_DRC_STAT_IND_RDATA0      ; /* 0x2628c */
    volatile U_ISP_DRC_STAT_IND_WADDR1   ISP_DRC_STAT_IND_WADDR1      ; /* 0x26290 */
    volatile U_ISP_DRC_STAT_IND_WDATA1   ISP_DRC_STAT_IND_WDATA1      ; /* 0x26294 */
    volatile U_ISP_DRC_STAT_IND_RADDR1   ISP_DRC_STAT_IND_RADDR1      ; /* 0x26298 */
    volatile U_ISP_DRC_STAT_IND_RDATA1   ISP_DRC_STAT_IND_RDATA1      ; /* 0x2629c */
    volatile U_ISP_DRC_STAT_IND_WADDR2   ISP_DRC_STAT_IND_WADDR2      ; /* 0x262a0 */
    volatile U_ISP_DRC_STAT_IND_WDATA2   ISP_DRC_STAT_IND_WDATA2      ; /* 0x262a4 */
    volatile U_ISP_DRC_STAT_IND_RADDR2   ISP_DRC_STAT_IND_RADDR2      ; /* 0x262a8 */
    volatile U_ISP_DRC_STAT_IND_RDATA2   ISP_DRC_STAT_IND_RDATA2      ; /* 0x262ac */
    volatile unsigned int           reserved_102[20]                     ; /* 0x262b0~0x262fc */
    volatile U_ISP_DRC_GRAD_REV     ISP_DRC_GRAD_REV                  ; /* 0x26300 */
    volatile U_ISP_DRC_VBI_STRENGTH   ISP_DRC_VBI_STRENGTH            ; /* 0x26304 */
    volatile U_ISP_DRC_VBI_MIXING   ISP_DRC_VBI_MIXING                ; /* 0x26308 */
    volatile unsigned int           reserved_1022                     ; /* 0x2630c*/
    volatile U_ISP_DRC_VBI_STATE    ISP_DRC_VBI_STATE                 ; /* 0x26310 */
    volatile unsigned int           reserved_10222[51]                     ; /* 0x26314~0x263dc */
    volatile U_ISP_DRC_CTRL_F       ISP_DRC_CTRL_F                    ; /* 0x263e0 */
    volatile U_ISP_DRC_CTRL_I       ISP_DRC_CTRL_I                    ; /* 0x263e4 */
    volatile U_ISP_DRC_TIMING       ISP_DRC_TIMING                    ; /* 0x263e8 */
    volatile U_ISP_DRC_UPDATE       ISP_DRC_UPDATE                    ; /* 0x263ec */
    volatile U_ISP_DRC_SIZE         ISP_DRC_SIZE                      ; /* 0x263f0 */
    volatile unsigned int           reserved_103[259]                     ; /* 0x263f4~0x267fc */
    volatile U_ISP_EXPANDER_CFG     ISP_EXPANDER_CFG                  ; /* 0x26800 */
    volatile unsigned int           reserved_104[2]                     ; /* 0x26804~0x26808 */
    volatile U_ISP_EXPANDER_VERSION   ISP_EXPANDER_VERSION            ; /* 0x2680c */
    volatile U_ISP_EXPANDER_BITW    ISP_EXPANDER_BITW                 ; /* 0x26810 */
    volatile U_ISP_EXPANDER_OFFSETR   ISP_EXPANDER_OFFSETR            ; /* 0x26814 */
    volatile U_ISP_EXPANDER_OFFSETGR   ISP_EXPANDER_OFFSETGR          ; /* 0x26818 */
    volatile U_ISP_EXPANDER_OFFSETGB   ISP_EXPANDER_OFFSETGB          ; /* 0x2681c */
    volatile U_ISP_EXPANDER_OFFSETB   ISP_EXPANDER_OFFSETB            ; /* 0x26820 */
    volatile unsigned int           reserved_105[23]                     ; /* 0x26824~0x2687c */
    volatile U_ISP_EXPANDER_WADDR   ISP_EXPANDER_WADDR                ; /* 0x26880 */
    volatile U_ISP_EXPANDER_WDATA   ISP_EXPANDER_WDATA                ; /* 0x26884 */
    volatile U_ISP_EXPANDER_RADDR   ISP_EXPANDER_RADDR                ; /* 0x26888 */
    volatile U_ISP_EXPANDER_RDATA   ISP_EXPANDER_RDATA                ; /* 0x2688c */
    volatile unsigned int           reserved_106[20]                     ; /* 0x26890~0x268dc */
    volatile U_ISP_EXPANDER_CTRL_F   ISP_EXPANDER_CTRL_F              ; /* 0x268e0 */
    volatile U_ISP_EXPANDER_CTRL_I   ISP_EXPANDER_CTRL_I              ; /* 0x268e4 */
    volatile unsigned int           reserved_107                     ; /* 0x268e8 */
    volatile U_ISP_EXPANDER_UPDATE   ISP_EXPANDER_UPDATE              ; /* 0x268ec */
    volatile unsigned int           reserved_108[196]                     ; /* 0x268f0~0x26bfc */
    volatile U_ISP_BCOM_CFG         ISP_BCOM_CFG                      ; /* 0x26c00 */
    volatile unsigned int           reserved_109[2]                     ; /* 0x26c04~0x26c08 */
    volatile U_ISP_BCOM_VERSION     ISP_BCOM_VERSION                  ; /* 0x26c0c */
    volatile U_ISP_BCOM_ALPHA       ISP_BCOM_ALPHA                    ; /* 0x26c10 */
    volatile unsigned int           reserved_110[52]                     ; /* 0x26c14~0x26ce0 */
    volatile U_ISP_BCOM_CTRL_I      ISP_BCOM_CTRL_I                   ; /* 0x26ce4 */
    volatile unsigned int           reserved_111                     ; /* 0x26ce8 */
    volatile U_ISP_BCOM_UPDATE      ISP_BCOM_UPDATE                   ; /* 0x26cec */
    volatile unsigned int           reserved_112[4]                     ; /* 0x26cf0~0x26cfc */
    volatile U_ISP_BDEC_CFG         ISP_BDEC_CFG                      ; /* 0x26d00 */
    volatile unsigned int           reserved_113[2]                     ; /* 0x26d04~0x26d08 */
    volatile U_ISP_BDEC_VERSION     ISP_BDEC_VERSION                  ; /* 0x26d0c */
    volatile U_ISP_BDEC_ALPHA       ISP_BDEC_ALPHA                    ; /* 0x26d10 */
    volatile unsigned int           reserved_114[52]                     ; /* 0x26d14~0x26de0 */
    volatile U_ISP_BDEC_CTRL_I      ISP_BDEC_CTRL_I                   ; /* 0x26de4 */
    volatile unsigned int           reserved_115                     ; /* 0x26de8 */
    volatile U_ISP_BDEC_UPDATE      ISP_BDEC_UPDATE                   ; /* 0x26dec */
    volatile unsigned int           reserved_116[4]                     ; /* 0x26df0~0x26dfc */
    volatile U_ISP_WDRSPLIT_CFG     ISP_WDRSPLIT_CFG                  ; /* 0x26e00 */
    volatile unsigned int           reserved_117[2]                     ; /* 0x26e04~0x26e08 */
    volatile U_ISP_WDRSPLIT_VERSION   ISP_WDRSPLIT_VERSION            ; /* 0x26e0c */
    volatile U_ISP_WDRSPLIT_BITW    ISP_WDRSPLIT_BITW                 ; /* 0x26e10 */
    volatile U_ISP_WDRSPLIT_MODE    ISP_WDRSPLIT_MODE                 ; /* 0x26e14 */
    volatile unsigned int           reserved_118[2]                     ; /* 0x26e18~0x26e1c */
    volatile U_ISP_WDRSPLIT_OFFSETR   ISP_WDRSPLIT_OFFSETR            ; /* 0x26e20 */
    volatile U_ISP_WDRSPLIT_OFFSETGR   ISP_WDRSPLIT_OFFSETGR          ; /* 0x26e24 */
    volatile U_ISP_WDRSPLIT_OFFSETGB   ISP_WDRSPLIT_OFFSETGB          ; /* 0x26e28 */
    volatile U_ISP_WDRSPLIT_OFFSETB   ISP_WDRSPLIT_OFFSETB            ; /* 0x26e2c */
    volatile U_ISP_WDRSPLIT_BLC     ISP_WDRSPLIT_BLC                  ; /* 0x26e30 */
    volatile unsigned int           reserved_119[19]                     ; /* 0x26e34~0x26e7c */
    volatile U_ISP_WDRSPLIT_WADDR   ISP_WDRSPLIT_WADDR                ; /* 0x26e80 */
    volatile U_ISP_WDRSPLIT_WDATA   ISP_WDRSPLIT_WDATA                ; /* 0x26e84 */
    volatile U_ISP_WDRSPLIT_RADDR   ISP_WDRSPLIT_RADDR                ; /* 0x26e88 */
    volatile U_ISP_WDRSPLIT_RDATA   ISP_WDRSPLIT_RDATA                ; /* 0x26e8c */
    volatile unsigned int           reserved_120[20]                     ; /* 0x26e90~0x26edc */
    volatile U_ISP_WDRSPLIT_CTRL_F   ISP_WDRSPLIT_CTRL_F              ; /* 0x26ee0 */
    volatile U_ISP_WDRSPLIT_CTRL_I   ISP_WDRSPLIT_CTRL_I              ; /* 0x26ee4 */
    volatile unsigned int           reserved_121                     ; /* 0x26ee8 */
    volatile U_ISP_WDRSPLIT_UPDATE   ISP_WDRSPLIT_UPDATE              ; /* 0x26eec */
    volatile unsigned int           reserved_122[2116]                     ; /* 0x26ef0~0x28ffc */
    //volatile U_ISP_AEWDR0_CFG       ISP_AEWDR0_CFG                    ; /* 0x29000 */
    //volatile unsigned int           reserved_124[2]                     ; /* 0x29004~0x29008 */
    //volatile U_ISP_AEWDR0_VERSION   ISP_AEWDR0_VERSION                ; /* 0x2900c */
    //volatile U_ISP_AEWDR0_ZONE      ISP_AEWDR0_ZONE                   ; /* 0x29010 */
    //volatile U_ISP_AEWDR0_SKIP_CRG   ISP_AEWDR0_SKIP_CRG              ; /* 0x29014 */
    //volatile U_ISP_AEWDR0_TOTAL_STAT   ISP_AEWDR0_TOTAL_STAT          ; /* 0x29018 */
    //volatile U_ISP_AEWDR0_COUNT_STAT   ISP_AEWDR0_COUNT_STAT          ; /* 0x2901c */
    //volatile unsigned int           reserved_125[4]                     ; /* 0x29020~0x2902c */
    //volatile U_ISP_AEWDR0_HIST_HIGH   ISP_AEWDR0_HIST_HIGH            ; /* 0x29030 */
    //volatile unsigned int           reserved_126[3]                     ; /* 0x29034~0x2903c */
    //volatile U_ISP_AEWDR0_BITMOVE   ISP_AEWDR0_BITMOVE                ; /* 0x29040 */
    //volatile U_ISP_AEWDR0_OFFSET_R   ISP_AEWDR0_OFFSET_R              ; /* 0x29044 */
    //volatile U_ISP_AEWDR0_OFFSET_GR   ISP_AEWDR0_OFFSET_GR            ; /* 0x29048 */
    //volatile U_ISP_AEWDR0_OFFSET_GB   ISP_AEWDR0_OFFSET_GB            ; /* 0x2904c */
    //volatile U_ISP_AEWDR0_OFFSET_B   ISP_AEWDR0_OFFSET_B              ; /* 0x29050 */
    //volatile unsigned int           reserved_127[11]                     ; /* 0x29054~0x2907c */
    //volatile U_ISP_AEWDR0_MEM_HIST_WADDR   ISP_AEWDR0_MEM_HIST_WADDR  ; /* 0x29080 */
    //volatile U_ISP_AEWDR0_MEM_HIST_WDATA   ISP_AEWDR0_MEM_HIST_WDATA  ; /* 0x29084 */
    //volatile U_ISP_AEWDR0_MEM_HIST_RADDR   ISP_AEWDR0_MEM_HIST_RADDR  ; /* 0x29088 */
    //volatile U_ISP_AEWDR0_MEM_HIST_RDATA   ISP_AEWDR0_MEM_HIST_RDATA  ; /* 0x2908c */
    //volatile unsigned int           reserved_128[4]                     ; /* 0x29090~0x2909c */
    //volatile U_ISP_AEWDR0_MEM_WEIGHT_WADDR   ISP_AEWDR0_MEM_WEIGHT_WADDR ; /* 0x290a0 */
    //volatile U_ISP_AEWDR0_MEM_WEIGHT_WDATA   ISP_AEWDR0_MEM_WEIGHT_WDATA ; /* 0x290a4 */
    //volatile U_ISP_AEWDR0_MEM_WEIGHT_RADDR   ISP_AEWDR0_MEM_WEIGHT_RADDR ; /* 0x290a8 */
    //volatile U_ISP_AEWDR0_MEM_WEIGHT_RDATA   ISP_AEWDR0_MEM_WEIGHT_RDATA ; /* 0x290ac */
    //volatile unsigned int           reserved_129[12]                     ; /* 0x290b0~0x290dc */
    //volatile U_ISP_AEWDR0_CTRL_F    ISP_AEWDR0_CTRL_F                 ; /* 0x290e0 */
    //volatile U_ISP_AEWDR0_CTRL_I    ISP_AEWDR0_CTRL_I                 ; /* 0x290e4 */
    //volatile unsigned int           reserved_130                     ; /* 0x290e8 */
    //volatile U_ISP_AEWDR0_UPDATE    ISP_AEWDR0_UPDATE                 ; /* 0x290ec */
    //volatile U_ISP_AEWDR0_SIZE      ISP_AEWDR0_SIZE                   ; /* 0x290f0 */
    //volatile unsigned int           reserved_131[3]                     ; /* 0x290f4~0x290fc */
    //volatile U_ISP_AEWDR1_CFG       ISP_AEWDR1_CFG                    ; /* 0x29100 */
    //volatile unsigned int           reserved_132[2]                     ; /* 0x29104~0x29108 */
    //volatile U_ISP_AEWDR1_VERSION   ISP_AEWDR1_VERSION                ; /* 0x2910c */
    //volatile U_ISP_AEWDR1_ZONE      ISP_AEWDR1_ZONE                   ; /* 0x29110 */
    //volatile U_ISP_AEWDR1_SKIP_CRG   ISP_AEWDR1_SKIP_CRG              ; /* 0x29114 */
    //volatile U_ISP_AEWDR1_TOTAL_STAT   ISP_AEWDR1_TOTAL_STAT          ; /* 0x29118 */
    //volatile U_ISP_AEWDR1_COUNT_STAT   ISP_AEWDR1_COUNT_STAT          ; /* 0x2911c */
    //volatile unsigned int           reserved_133[4]                     ; /* 0x29120~0x2912c */
    //volatile U_ISP_AEWDR1_HIST_HIGH   ISP_AEWDR1_HIST_HIGH            ; /* 0x29130 */
    //volatile unsigned int           reserved_134[3]                     ; /* 0x29134~0x2913c */
    //volatile U_ISP_AEWDR1_BITMOVE   ISP_AEWDR1_BITMOVE                ; /* 0x29140 */
    //volatile U_ISP_AEWDR1_OFFSET_R   ISP_AEWDR1_OFFSET_R              ; /* 0x29144 */
    //volatile U_ISP_AEWDR1_OFFSET_GR   ISP_AEWDR1_OFFSET_GR            ; /* 0x29148 */
    //volatile U_ISP_AEWDR1_OFFSET_GB   ISP_AEWDR1_OFFSET_GB            ; /* 0x2914c */
    //volatile U_ISP_AEWDR1_OFFSET_B   ISP_AEWDR1_OFFSET_B              ; /* 0x29150 */
    //volatile unsigned int           reserved_135[11]                     ; /* 0x29154~0x2917c */
    //volatile U_ISP_AEWDR1_MEM_HIST_WADDR   ISP_AEWDR1_MEM_HIST_WADDR  ; /* 0x29180 */
    //volatile U_ISP_AEWDR1_MEM_HIST_WDATA   ISP_AEWDR1_MEM_HIST_WDATA  ; /* 0x29184 */
    //volatile U_ISP_AEWDR1_MEM_HIST_RADDR   ISP_AEWDR1_MEM_HIST_RADDR  ; /* 0x29188 */
    //volatile U_ISP_AEWDR1_MEM_HIST_RDATA   ISP_AEWDR1_MEM_HIST_RDATA  ; /* 0x2918c */
    //volatile unsigned int           reserved_136[4]                     ; /* 0x29190~0x2919c */
    //volatile U_ISP_AEWDR1_MEM_WEIGHT_WADDR   ISP_AEWDR1_MEM_WEIGHT_WADDR ; /* 0x291a0 */
    //volatile U_ISP_AEWDR1_MEM_WEIGHT_WDATA   ISP_AEWDR1_MEM_WEIGHT_WDATA ; /* 0x291a4 */
    //volatile U_ISP_AEWDR1_MEM_WEIGHT_RADDR   ISP_AEWDR1_MEM_WEIGHT_RADDR ; /* 0x291a8 */
    //volatile U_ISP_AEWDR1_MEM_WEIGHT_RDATA   ISP_AEWDR1_MEM_WEIGHT_RDATA ; /* 0x291ac */
    //volatile unsigned int           reserved_137[12]                     ; /* 0x291b0~0x291dc */
    //volatile U_ISP_AEWDR1_CTRL_F    ISP_AEWDR1_CTRL_F                 ; /* 0x291e0 */
    //volatile U_ISP_AEWDR1_CTRL_I    ISP_AEWDR1_CTRL_I                 ; /* 0x291e4 */
    //volatile unsigned int           reserved_138                     ; /* 0x291e8 */
    //volatile U_ISP_AEWDR1_UPDATE    ISP_AEWDR1_UPDATE                 ; /* 0x291ec */
    //volatile U_ISP_AEWDR1_SIZE      ISP_AEWDR1_SIZE                   ; /* 0x291f0 */
    //volatile unsigned int           reserved_139[3]                     ; /* 0x291f4~0x291fc */
    //volatile U_ISP_AEWDR2_CFG       ISP_AEWDR2_CFG                    ; /* 0x29200 */
    //volatile unsigned int           reserved_140[2]                     ; /* 0x29204~0x29208 */
    //volatile U_ISP_AEWDR2_VERSION   ISP_AEWDR2_VERSION                ; /* 0x2920c */
    //volatile U_ISP_AEWDR2_ZONE      ISP_AEWDR2_ZONE                   ; /* 0x29210 */
    //volatile U_ISP_AEWDR2_SKIP_CRG   ISP_AEWDR2_SKIP_CRG              ; /* 0x29214 */
    //volatile U_ISP_AEWDR2_TOTAL_STAT   ISP_AEWDR2_TOTAL_STAT          ; /* 0x29218 */
    //volatile U_ISP_AEWDR2_COUNT_STAT   ISP_AEWDR2_COUNT_STAT          ; /* 0x2921c */
    //volatile unsigned int           reserved_141[4]                     ; /* 0x29220~0x2922c */
    //volatile U_ISP_AEWDR2_HIST_HIGH   ISP_AEWDR2_HIST_HIGH            ; /* 0x29230 */
    //volatile unsigned int           reserved_142[3]                     ; /* 0x29234~0x2923c */
    //volatile U_ISP_AEWDR2_BITMOVE   ISP_AEWDR2_BITMOVE                ; /* 0x29240 */
    //volatile U_ISP_AEWDR2_OFFSET_R   ISP_AEWDR2_OFFSET_R              ; /* 0x29244 */
    //volatile U_ISP_AEWDR2_OFFSET_GR   ISP_AEWDR2_OFFSET_GR            ; /* 0x29248 */
    //volatile U_ISP_AEWDR2_OFFSET_GB   ISP_AEWDR2_OFFSET_GB            ; /* 0x2924c */
    //volatile U_ISP_AEWDR2_OFFSET_B   ISP_AEWDR2_OFFSET_B              ; /* 0x29250 */
    //volatile unsigned int           reserved_143[11]                     ; /* 0x29254~0x2927c */
    //volatile U_ISP_AEWDR2_MEM_HIST_WADDR   ISP_AEWDR2_MEM_HIST_WADDR  ; /* 0x29280 */
    //volatile U_ISP_AEWDR2_MEM_HIST_WDATA   ISP_AEWDR2_MEM_HIST_WDATA  ; /* 0x29284 */
    //volatile U_ISP_AEWDR2_MEM_HIST_RADDR   ISP_AEWDR2_MEM_HIST_RADDR  ; /* 0x29288 */
    //volatile U_ISP_AEWDR2_MEM_HIST_RDATA   ISP_AEWDR2_MEM_HIST_RDATA  ; /* 0x2928c */
    //volatile unsigned int           reserved_144[4]                     ; /* 0x29290~0x2929c */
    //volatile U_ISP_AEWDR2_MEM_WEIGHT_WADDR   ISP_AEWDR2_MEM_WEIGHT_WADDR ; /* 0x292a0 */
    //volatile U_ISP_AEWDR2_MEM_WEIGHT_WDATA   ISP_AEWDR2_MEM_WEIGHT_WDATA ; /* 0x292a4 */
    //volatile U_ISP_AEWDR2_MEM_WEIGHT_RADDR   ISP_AEWDR2_MEM_WEIGHT_RADDR ; /* 0x292a8 */
    //volatile U_ISP_AEWDR2_MEM_WEIGHT_RDATA   ISP_AEWDR2_MEM_WEIGHT_RDATA ; /* 0x292ac */
    //volatile unsigned int           reserved_145[12]                     ; /* 0x292b0~0x292dc */
    //volatile U_ISP_AEWDR2_CTRL_F    ISP_AEWDR2_CTRL_F                 ; /* 0x292e0 */
    //volatile U_ISP_AEWDR2_CTRL_I    ISP_AEWDR2_CTRL_I                 ; /* 0x292e4 */
    //volatile unsigned int           reserved_146                     ; /* 0x292e8 */
    //volatile U_ISP_AEWDR2_UPDATE    ISP_AEWDR2_UPDATE                 ; /* 0x292ec */
    //volatile U_ISP_AEWDR2_SIZE      ISP_AEWDR2_SIZE                   ; /* 0x292f0 */
    //volatile unsigned int           reserved_147[3]                     ; /* 0x292f4~0x292fc */
    //volatile U_ISP_AEWDR3_CFG       ISP_AEWDR3_CFG                    ; /* 0x29300 */
    //volatile unsigned int           reserved_148[2]                     ; /* 0x29304~0x29308 */
    //volatile U_ISP_AEWDR3_VERSION   ISP_AEWDR3_VERSION                ; /* 0x2930c */
    //volatile U_ISP_AEWDR3_ZONE      ISP_AEWDR3_ZONE                   ; /* 0x29310 */
    //volatile U_ISP_AEWDR3_SKIP_CRG   ISP_AEWDR3_SKIP_CRG              ; /* 0x29314 */
    //volatile U_ISP_AEWDR3_TOTAL_STAT   ISP_AEWDR3_TOTAL_STAT          ; /* 0x29318 */
    //volatile U_ISP_AEWDR3_COUNT_STAT   ISP_AEWDR3_COUNT_STAT          ; /* 0x2931c */
    //volatile unsigned int           reserved_149[4]                     ; /* 0x29320~0x2932c */
    //volatile U_ISP_AEWDR3_HIST_HIGH   ISP_AEWDR3_HIST_HIGH            ; /* 0x29330 */
    //volatile unsigned int           reserved_150[3]                     ; /* 0x29334~0x2933c */
    //volatile U_ISP_AEWDR3_BITMOVE   ISP_AEWDR3_BITMOVE                ; /* 0x29340 */
    //volatile U_ISP_AEWDR3_OFFSET_R   ISP_AEWDR3_OFFSET_R              ; /* 0x29344 */
    //volatile U_ISP_AEWDR3_OFFSET_GR   ISP_AEWDR3_OFFSET_GR            ; /* 0x29348 */
    //volatile U_ISP_AEWDR3_OFFSET_GB   ISP_AEWDR3_OFFSET_GB            ; /* 0x2934c */
    //volatile U_ISP_AEWDR3_OFFSET_B   ISP_AEWDR3_OFFSET_B              ; /* 0x29350 */
    //volatile unsigned int           reserved_151[11]                     ; /* 0x29354~0x2937c */
    //volatile U_ISP_AEWDR3_MEM_HIST_WADDR   ISP_AEWDR3_MEM_HIST_WADDR  ; /* 0x29380 */
    //volatile U_ISP_AEWDR3_MEM_HIST_WDATA   ISP_AEWDR3_MEM_HIST_WDATA  ; /* 0x29384 */
    //volatile U_ISP_AEWDR3_MEM_HIST_RADDR   ISP_AEWDR3_MEM_HIST_RADDR  ; /* 0x29388 */
    //volatile U_ISP_AEWDR3_MEM_HIST_RDATA   ISP_AEWDR3_MEM_HIST_RDATA  ; /* 0x2938c */
    //volatile unsigned int           reserved_152[4]                     ; /* 0x29390~0x2939c */
    //volatile U_ISP_AEWDR3_MEM_WEIGHT_WADDR   ISP_AEWDR3_MEM_WEIGHT_WADDR ; /* 0x293a0 */
    //volatile U_ISP_AEWDR3_MEM_WEIGHT_WDATA   ISP_AEWDR3_MEM_WEIGHT_WDATA ; /* 0x293a4 */
    //volatile U_ISP_AEWDR3_MEM_WEIGHT_RADDR   ISP_AEWDR3_MEM_WEIGHT_RADDR ; /* 0x293a8 */
    //volatile U_ISP_AEWDR3_MEM_WEIGHT_RDATA   ISP_AEWDR3_MEM_WEIGHT_RDATA ; /* 0x293ac */
    //volatile unsigned int           reserved_153[12]                     ; /* 0x293b0~0x293dc */
    //volatile U_ISP_AEWDR3_CTRL_F    ISP_AEWDR3_CTRL_F                 ; /* 0x293e0 */
    //volatile U_ISP_AEWDR3_CTRL_I    ISP_AEWDR3_CTRL_I                 ; /* 0x293e4 */
    //volatile unsigned int           reserved_154                     ; /* 0x293e8 */
    //volatile U_ISP_AEWDR3_UPDATE    ISP_AEWDR3_UPDATE                 ; /* 0x293ec */
    //volatile U_ISP_AEWDR3_SIZE      ISP_AEWDR3_SIZE                   ; /* 0x293f0 */
    volatile ISP_AE_WDR_S           ISP_AE_WDR[4];                        /* 0x29000~0x293fc*/
    volatile unsigned int           reserved_155[23328]               ; /* 0x29400~0x4007c */
    volatile U_ISP_BE_VERSION       ISP_BE_VERSION                    ; /* 0x40080 */
    volatile U_ISP_BE_DATE          ISP_BE_DATE                       ; /* 0x40084 */
    volatile unsigned int           reserved_156[2]                     ; /* 0x40088~0x4008c */
    volatile U_ISP_BE_MODULE_POS    ISP_BE_MODULE_POS                 ; /* 0x40090 */
    volatile U_ISP_BE_FSTART_DELAY   ISP_BE_FSTART_DELAY              ; /* 0x40094 */
    volatile unsigned int           reserved_157[2]                     ; /* 0x40098~0x4009c */
    volatile U_ISP_BE_USER_DEFINE0   ISP_BE_USER_DEFINE0              ; /* 0x400a0 */
    volatile U_ISP_BE_USER_DEFINE1   ISP_BE_USER_DEFINE1              ; /* 0x400a4 */
	volatile unsigned int           reserved_158[2]                     ; /* 0x400a8~0x400ac */
    volatile U_ISP_BE_STARTUP       ISP_BE_STARTUP                    ; /* 0x400b0 */
    volatile unsigned int           reserved_1588[15]                     ; /* 0x400b4~0x400ec */
    volatile U_ISP_BE_INT           ISP_BE_INT                        ; /* 0x400f0 */
    volatile unsigned int           reserved_159                     ; /* 0x400f4 */
    volatile U_ISP_BE_INT_MASK      ISP_BE_INT_MASK                   ; /* 0x400f8 */
    volatile unsigned int           reserved_160[57]                     ; /* 0x400fc~0x401dc */
    volatile U_ISP_BE_CTRL_F        ISP_BE_CTRL_F                     ; /* 0x401e0 */
    volatile U_ISP_BE_CTRL_I        ISP_BE_CTRL_I                     ; /* 0x401e4 */
    volatile U_ISP_BE_TIMING_CFG    ISP_BE_TIMING_CFG                 ; /* 0x401e8 */
    volatile U_ISP_BE_REG_UPDATE    ISP_BE_REG_UPDATE                 ; /* 0x401ec */
    volatile unsigned int           reserved_161[388]                     ; /* 0x401f0~0x407fc */
    volatile U_ISP_CLIP_Y_CFG       ISP_CLIP_Y_CFG                    ; /* 0x40800 */
    volatile U_ISP_CLIP_C_CFG       ISP_CLIP_C_CFG                    ; /* 0x40804 */
    volatile unsigned int           reserved_162[2]                     ; /* 0x40808~0x4080c */
    volatile U_ISP_SKIP_Y_CFG       ISP_SKIP_Y_CFG                    ; /* 0x40810 */
    volatile U_ISP_SKIP_Y_WIN       ISP_SKIP_Y_WIN                    ; /* 0x40814 */
    volatile U_ISP_SKIP_C_CFG       ISP_SKIP_C_CFG                    ; /* 0x40818 */
    volatile U_ISP_SKIP_C_WIN       ISP_SKIP_C_WIN                    ; /* 0x4081c */
    volatile U_ISP_CSC_SUM_CFG      ISP_CSC_SUM_CFG                   ; /* 0x40820 */
    volatile U_ISP_YUV444_SUM_CFG   ISP_YUV444_SUM_CFG                ; /* 0x40824 */
    volatile U_ISP_YUV422_SUM_CFG   ISP_YUV422_SUM_CFG                ; /* 0x40828 */
    volatile unsigned int           reserved_163[5]                     ; /* 0x4082c~0x4083c */
    volatile U_ISP_DMNR_DITHER      ISP_DMNR_DITHER                   ; /* 0x40840 */
    volatile unsigned int           reserved_164[3]                     ; /* 0x40844~0x4084c */
    volatile U_ISP_ACM_DITHER       ISP_ACM_DITHER                    ; /* 0x40850 */
    volatile unsigned int           reserved_165[3]                     ; /* 0x40854~0x4085c */
    volatile U_ISP_CROP_Y_CFG       ISP_CROP_Y_CFG                    ; /* 0x40860 */
    volatile U_ISP_CROP_Y_RESERVED   ISP_CROP_Y_RESERVED              ; /* 0x40864 */
    volatile U_ISP_CROP_Y_START     ISP_CROP_Y_START                  ; /* 0x40868 */
    volatile U_ISP_CROP_Y_SIZE      ISP_CROP_Y_SIZE                   ; /* 0x4086c */
    volatile U_ISP_CROP_C_CFG       ISP_CROP_C_CFG                    ; /* 0x40870 */
    volatile U_ISP_CROP_C_RESERVED   ISP_CROP_C_RESERVED              ; /* 0x40874 */
    volatile U_ISP_CROP_C_START     ISP_CROP_C_START                  ; /* 0x40878 */
    volatile U_ISP_CROP_C_SIZE      ISP_CROP_C_SIZE                   ; /* 0x4087c */
    volatile U_ISP_Y_SUM0           ISP_Y_SUM0                        ; /* 0x40880 */
    volatile U_ISP_Y_SUM1           ISP_Y_SUM1                        ; /* 0x40884 */
    volatile unsigned int           reserved_166[2]                     ; /* 0x40888~0x4088c */
    volatile U_ISP_YUV444_Y_SUM0    ISP_YUV444_Y_SUM0                 ; /* 0x40890 */
    volatile U_ISP_YUV444_Y_SUM1    ISP_YUV444_Y_SUM1                 ; /* 0x40894 */
    volatile U_ISP_YUV444_U_SUM0    ISP_YUV444_U_SUM0                 ; /* 0x40898 */
    volatile U_ISP_YUV444_U_SUM1    ISP_YUV444_U_SUM1                 ; /* 0x4089c */
    volatile U_ISP_YUV444_V_SUM0    ISP_YUV444_V_SUM0                 ; /* 0x408a0 */
    volatile U_ISP_YUV444_V_SUM1    ISP_YUV444_V_SUM1                 ; /* 0x408a4 */
    volatile unsigned int           reserved_167[2]                     ; /* 0x408a8~0x408ac */
    volatile U_ISP_YUV422_Y_SUM0    ISP_YUV422_Y_SUM0                 ; /* 0x408b0 */
    volatile U_ISP_YUV422_Y_SUM1    ISP_YUV422_Y_SUM1                 ; /* 0x408b4 */
    volatile U_ISP_YUV422_C_SUM0    ISP_YUV422_C_SUM0                 ; /* 0x408b8 */
    volatile U_ISP_YUV422_C_SUM1    ISP_YUV422_C_SUM1                 ; /* 0x408bc */
    volatile unsigned int           reserved_168[16]                     ; /* 0x408c0~0x408fc */
    volatile U_ISP_SPLIT_CFG        ISP_SPLIT_CFG                     ; /* 0x40900 */
    volatile unsigned int           reserved_169[2]                     ; /* 0x40904~0x40908 */
    volatile U_ISP_SPLIT_VERSION    ISP_SPLIT_VERSION                 ; /* 0x4090c */
    volatile U_ISP_SPLIT_DATA_CFG   ISP_SPLIT_DATA_CFG                ; /* 0x40910 */
    volatile U_ISP_SPLIT_DATA_VERSION   ISP_SPLIT_DATA_VERSION        ; /* 0x40914 */
    volatile U_ISP_SPLIT_DATA_BITWIDTH   ISP_SPLIT_DATA_BITWIDTH      ; /* 0x40918 */
    volatile U_ISP_SPLIT_DATA_AGAIN   ISP_SPLIT_DATA_AGAIN            ; /* 0x4091c */
    volatile U_ISP_SPLIT_DATA_DGAIN   ISP_SPLIT_DATA_DGAIN            ; /* 0x40920 */
    volatile U_ISP_SPLIT_DATA_SHUTTER   ISP_SPLIT_DATA_SHUTTER        ; /* 0x40924 */
    volatile unsigned int           reserved_170[47]                     ; /* 0x40928~0x409e0 */
    volatile U_ISP_SPLIT_CTRL_I     ISP_SPLIT_CTRL_I                  ; /* 0x409e4 */
    volatile unsigned int           reserved_171                     ; /* 0x409e8 */
    volatile U_ISP_SPLIT_UPDATE     ISP_SPLIT_UPDATE                  ; /* 0x409ec */
    volatile unsigned int           reserved_172[644]                     ; /* 0x409f0~0x413fc */
    volatile U_ISP_CC_CFG           ISP_CC_CFG                        ; /* 0x41400 */
    volatile unsigned int           reserved_173[2]                     ; /* 0x41404~0x41408 */
    volatile U_ISP_CC_VERSION       ISP_CC_VERSION                    ; /* 0x4140c */
    volatile U_ISP_CC_COEF0         ISP_CC_COEF0                      ; /* 0x41410 */
    volatile U_ISP_CC_COEF1         ISP_CC_COEF1                      ; /* 0x41414 */
    volatile U_ISP_CC_COEF2         ISP_CC_COEF2                      ; /* 0x41418 */
    volatile U_ISP_CC_COEF3         ISP_CC_COEF3                      ; /* 0x4141c */
    volatile U_ISP_CC_COEF4         ISP_CC_COEF4                      ; /* 0x41420 */
    volatile unsigned int           reserved_174[9]                     ; /* 0x41424~0x41444 */
    volatile U_ISP_CC_BRIT_THRES    ISP_CC_BRIT_THRES                 ; /* 0x41448 */
    volatile U_ISP_CC_COLOR_THRES   ISP_CC_COLOR_THRES                ; /* 0x4144c */
    volatile U_ISP_CC_LOW_RATIO     ISP_CC_LOW_RATIO                  ; /* 0x41450 */
    volatile U_ISP_CC_PRO_RATIO     ISP_CC_PRO_RATIO                  ; /* 0x41454 */
    volatile U_ISP_CC_COLORTONE_RB_GAIN   ISP_CC_COLORTONE_RB_GAIN    ; /* 0x41458 */
    volatile U_ISP_CC_COLORTONE_G_GAIN   ISP_CC_COLORTONE_G_GAIN      ; /* 0x4145c */
    volatile U_ISP_CC_PROT_EX_TH0   ISP_CC_PROT_EX_TH0                ; /* 0x41460 */
    volatile U_ISP_CC_PROT_EX_TH1   ISP_CC_PROT_EX_TH1                ; /* 0x41464 */
    volatile U_ISP_CC_PROT_EX_RGB_SET   ISP_CC_PROT_EX_RGB_SET        ; /* 0x41468 */
    volatile unsigned int           reserved_175[30]                     ; /* 0x4146c~0x414e0 */
    volatile U_ISP_CC_CTRL_I        ISP_CC_CTRL_I                     ; /* 0x414e4 */
    volatile unsigned int           reserved_176                     ; /* 0x414e8 */
    volatile U_ISP_CC_UPDATE        ISP_CC_UPDATE                     ; /* 0x414ec */
    volatile unsigned int           reserved_177[4]                     ; /* 0x414f0~0x414fc */
    volatile U_ISP_CSC_CFG          ISP_CSC_CFG                       ; /* 0x41500 */
    volatile unsigned int           reserved_178[2]                     ; /* 0x41504~0x41508 */
    volatile U_ISP_CSC_VERSION      ISP_CSC_VERSION                   ; /* 0x4150c */
    volatile U_ISP_CSC_COEF0        ISP_CSC_COEF0                     ; /* 0x41510 */
    volatile U_ISP_CSC_COEF1        ISP_CSC_COEF1                     ; /* 0x41514 */
    volatile U_ISP_CSC_COEF2        ISP_CSC_COEF2                     ; /* 0x41518 */
    volatile U_ISP_CSC_COEF3        ISP_CSC_COEF3                     ; /* 0x4151c */
    volatile U_ISP_CSC_COEF4        ISP_CSC_COEF4                     ; /* 0x41520 */
    volatile unsigned int           reserved_179[3]                     ; /* 0x41524~0x4152c */
    volatile U_ISP_CSC_IN_DC0       ISP_CSC_IN_DC0                    ; /* 0x41530 */
    volatile U_ISP_CSC_IN_DC1       ISP_CSC_IN_DC1                    ; /* 0x41534 */
    volatile unsigned int           reserved_180[2]                     ; /* 0x41538~0x4153c */
    volatile U_ISP_CSC_OUT_DC0      ISP_CSC_OUT_DC0                   ; /* 0x41540 */
    volatile U_ISP_CSC_OUT_DC1      ISP_CSC_OUT_DC1                   ; /* 0x41544 */
    volatile unsigned int           reserved_181[39]                     ; /* 0x41548~0x415e0 */
    volatile U_ISP_CSC_CTRL_I       ISP_CSC_CTRL_I                    ; /* 0x415e4 */
    volatile unsigned int           reserved_182                     ; /* 0x415e8 */
    volatile U_ISP_CSC_UPDATE       ISP_CSC_UPDATE                    ; /* 0x415ec */
    volatile unsigned int           reserved_183[132]                     ; /* 0x415f0~0x417fc */
    volatile U_ISP_MCDS_CFG         ISP_MCDS_CFG                      ; /* 0x41800 */
    volatile unsigned int           reserved_184[2]                     ; /* 0x41804~0x41808 */
    volatile U_ISP_MCDS_VERSION     ISP_MCDS_VERSION                  ; /* 0x4180c */
    volatile unsigned int           reserved_185                     ; /* 0x41810 */
    volatile U_ISP_HCDS_SIZE        ISP_HCDS_SIZE                     ; /* 0x41814 */
    volatile unsigned int           reserved_186                     ; /* 0x41818 */
    volatile U_ISP_HCDS_COEF0       ISP_HCDS_COEF0                    ; /* 0x4181c */
    volatile U_ISP_HCDS_COEF1       ISP_HCDS_COEF1                    ; /* 0x41820 */
    volatile U_ISP_HCDS_COEF2       ISP_HCDS_COEF2                    ; /* 0x41824 */
    volatile U_ISP_VCDS_COEF        ISP_VCDS_COEF                     ; /* 0x41828 */
    volatile U_ISP_MIDF_SIZE        ISP_MIDF_SIZE                     ; /* 0x4182c */
    volatile unsigned int           reserved_187[5]                     ; /* 0x41830~0x41840 */
    volatile U_ISP_MCDS_PARA        ISP_MCDS_PARA                     ; /* 0x41844 */
    volatile unsigned int           reserved_188[39]                     ; /* 0x41848~0x418e0 */
    volatile U_ISP_MCDS_CTRL_I      ISP_MCDS_CTRL_I                   ; /* 0x418e4 */
    volatile U_ISP_MCDS_TIMING      ISP_MCDS_TIMING                   ; /* 0x418e8 */
    volatile U_ISP_MCDS_UPDATE      ISP_MCDS_UPDATE                   ; /* 0x418ec */
    volatile U_ISP_MCDS_SIZE        ISP_MCDS_SIZE                     ; /* 0x418f0 */
    volatile unsigned int           reserved_189[1603]                     ; /* 0x418f4~0x431fc */
    volatile U_ISP_GCAC_CFG         ISP_GCAC_CFG                      ; /* 0x43200 */
    volatile unsigned int           reserved_190[2]                     ; /* 0x43204~0x43208 */
    volatile U_ISP_GCAC_VERSION     ISP_GCAC_VERSION                  ; /* 0x4320c */
    volatile U_ISP_GCAC_PARAMA      ISP_GCAC_PARAMA                   ; /* 0x43210 */
    volatile U_ISP_GCAC_PARAMB      ISP_GCAC_PARAMB                   ; /* 0x43214 */
    volatile U_ISP_GCAC_PARAMC      ISP_GCAC_PARAMC                   ; /* 0x43218 */
    volatile U_ISP_GCAC_YNORM       ISP_GCAC_YNORM                    ; /* 0x4321c */
    volatile U_ISP_GCAC_XNORM       ISP_GCAC_XNORM                    ; /* 0x43220 */
    volatile U_ISP_GCAC_CNTSTART    ISP_GCAC_CNTSTART                 ; /* 0x43224 */
    volatile U_ISP_GCAC_CORSTART    ISP_GCAC_CORSTART                 ; /* 0x43228 */
    volatile U_ISP_GCAC_FILTEN      ISP_GCAC_FILTEN                   ; /* 0x4322c */
    volatile U_ISP_GCAC_CHRVMODE    ISP_GCAC_CHRVMODE                 ; /* 0x43230 */
    volatile U_ISP_GCAC_CLIPMODE    ISP_GCAC_CLIPMODE                 ; /* 0x43234 */
    volatile unsigned int           reserved_191[42]                     ; /* 0x43238~0x432dc */
    volatile U_ISP_GCAC_CTRL_F      ISP_GCAC_CTRL_F                   ; /* 0x432e0 */
    volatile U_ISP_GCAC_CTRL_I      ISP_GCAC_CTRL_I                   ; /* 0x432e4 */
    volatile U_ISP_GCAC_TIMING      ISP_GCAC_TIMING                   ; /* 0x432e8 */
    volatile U_ISP_GCAC_UPDATE      ISP_GCAC_UPDATE                   ; /* 0x432ec */
    volatile U_ISP_GCAC_SIZE        ISP_GCAC_SIZE                     ; /* 0x432f0 */
    volatile unsigned int           reserved_192[835]                     ; /* 0x432f4~0x43ffc */
    volatile U_ISP_DEMOSAIC_CFG     ISP_DEMOSAIC_CFG                  ; /* 0x44000 */
    volatile U_ISP_DEMOSAIC_CFG1    ISP_DEMOSAIC_CFG1                 ; /* 0x44004 */
    volatile unsigned int           reserved_193                     ; /* 0x44008 */
    volatile U_ISP_DEMOSAIC_VERSION   ISP_DEMOSAIC_VERSION            ; /* 0x4400c */
    volatile U_ISP_DEMOSAIC_COEF0   ISP_DEMOSAIC_COEF0                ; /* 0x44010 */
    volatile U_ISP_DEMOSAIC_COEF1   ISP_DEMOSAIC_COEF1                ; /* 0x44014 */
    volatile U_ISP_DEMOSAIC_COEF2   ISP_DEMOSAIC_COEF2                ; /* 0x44018 */
    volatile U_ISP_DEMOSAIC_COEF3   ISP_DEMOSAIC_COEF3                ; /* 0x4401c */
    volatile U_ISP_DEMOSAIC_COEF4   ISP_DEMOSAIC_COEF4                ; /* 0x44020 */
    volatile U_ISP_DEMOSAIC_COEF5   ISP_DEMOSAIC_COEF5                ; /* 0x44024 */
    volatile U_ISP_DEMOSAIC_COEF6   ISP_DEMOSAIC_COEF6                ; /* 0x44028 */
    volatile unsigned int           reserved_194                     ; /* 0x4402c */
    volatile U_ISP_DEMOSAIC_SEL     ISP_DEMOSAIC_SEL                  ; /* 0x44030 */
    volatile U_ISP_DEMOSAIC_TMP     ISP_DEMOSAIC_TMP                  ; /* 0x44034 */
    volatile U_ISP_DEMOSAIC_LCAC_CNT_THR   ISP_DEMOSAIC_LCAC_CNT_THR  ; /* 0x44038 */
    volatile U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR   ISP_DEMOSAIC_LCAC_LUMA_RB_THR ; /* 0x4403c */
    volatile U_ISP_DEMOSAIC_LCAC_LUMA_G_THR   ISP_DEMOSAIC_LCAC_LUMA_G_THR ; /* 0x44040 */
    volatile U_ISP_DEMOSAIC_PURPLE_VAR_THR   ISP_DEMOSAIC_PURPLE_VAR_THR ; /* 0x44044 */
    volatile U_ISP_DEMOSAIC_FAKE_CR_VAR_THR   ISP_DEMOSAIC_FAKE_CR_VAR_THR ; /* 0x44048 */
    volatile U_ISP_DEMOSAIC_DEPURPLECTR   ISP_DEMOSAIC_DEPURPLECTR    ; /* 0x4404c */
    volatile U_ISP_DEMOSAIC_HV_POINT_THLD   ISP_DEMOSAIC_HV_POINT_THLD ; /* 0x44050 */
    volatile U_ISP_DEMOSAIC_BLD_ADJUST   ISP_DEMOSAIC_BLD_ADJUST      ; /* 0x44054 */
    volatile U_ISP_DEMOSAIC_CBCRAVGTHLD   ISP_DEMOSAIC_CBCRAVGTHLD    ; /* 0x44058 */
    volatile U_ISP_DEMOSAIC_GAPTHLD   ISP_DEMOSAIC_GAPTHLD            ; /* 0x4405c */
    volatile U_ISP_DEMOSAIC_COLORTHLD   ISP_DEMOSAIC_COLORTHLD        ; /* 0x44060 */
    volatile U_ISP_DEMOSAIC_GVAR_BLEND_THLD   ISP_DEMOSAIC_GVAR_BLEND_THLD ; /* 0x44064 */
    volatile U_ISP_DEMOSAIC_SATU_THR   ISP_DEMOSAIC_SATU_THR          ; /* 0x44068 */
    volatile U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT   ISP_DEMOSAIC_CBCR_RATIO_LIMIT ; /* 0x4406c */
    volatile U_ISP_DEMOSAIC_FCR_GRAY_RATIO   ISP_DEMOSAIC_FCR_GRAY_RATIO ; /* 0x44070 */
    volatile U_ISP_DEMOSAIC_FCR_SEL   ISP_DEMOSAIC_FCR_SEL            ; /* 0x44074 */
    volatile U_ISP_DEMOSAIC_BLEND_RATIO   ISP_DEMOSAIC_BLEND_RATIO    ; /* 0x44078 */
    volatile U_ISP_DEMOSAIC_BLEND_DIF_LIMIT   ISP_DEMOSAIC_BLEND_DIF_LIMIT ; /* 0x4407c */
    volatile unsigned int           reserved_194_1[36]                     ; /* 0x44080~0x4410c */
    volatile U_ISP_DEMOSAIC_HF_INTP_TH   ISP_DEMOSAIC_HF_INTP_TH      ; /* 0x44110 */
    volatile U_ISP_DEMOSAIC_HF_INTP_BLD   ISP_DEMOSAIC_HF_INTP_BLD    ; /* 0x44114 */
    volatile U_ISP_DEMOSAIC_HF_INTP_TH1   ISP_DEMOSAIC_HF_INTP_TH1    ; /* 0x44118 */
    volatile U_ISP_DEMOSAIC_HF_INTP_RATIO0   ISP_DEMOSAIC_HF_INTP_RATIO0 ; /* 0x4411c */
    volatile U_ISP_DEMOSAIC_HF_INTP_RATIO1   ISP_DEMOSAIC_HF_INTP_RATIO1 ; /* 0x44120 */
    volatile unsigned int           reserved_195[47]                     ; /* 0x44124~0x441dc */
    volatile U_ISP_DEMOSAIC_CTRL_F   ISP_DEMOSAIC_CTRL_F              ; /* 0x441e0 */
    volatile U_ISP_DEMOSAIC_CTRL_I   ISP_DEMOSAIC_CTRL_I              ; /* 0x441e4 */
    volatile U_ISP_DEMOSAIC_TIMING   ISP_DEMOSAIC_TIMING              ; /* 0x441e8 */
    volatile U_ISP_DEMOSAIC_UPDATE   ISP_DEMOSAIC_UPDATE              ; /* 0x441ec */
    volatile U_ISP_DEMOSAIC_SIZE    ISP_DEMOSAIC_SIZE                 ; /* 0x441f0 */
    volatile unsigned int           reserved_200[1027]                     ; /* 0x441f4~0x451fc */
    volatile U_ISP_SHARPEN_CFG      ISP_SHARPEN_CFG                   ; /* 0x45200 */
    volatile unsigned int           reserved_201[2]                     ; /* 0x45204~0x45208 */
    volatile U_ISP_SHARPEN_VERSION   ISP_SHARPEN_VERSION              ; /* 0x4520c */
    volatile U_ISP_SHARPEN_AMT      ISP_SHARPEN_AMT                   ; /* 0x45210 */
    volatile U_ISP_SHARPEN_THD1     ISP_SHARPEN_THD1                  ; /* 0x45214 */
    volatile U_ISP_SHARPEN_THD2     ISP_SHARPEN_THD2                  ; /* 0x45218 */
    volatile U_ISP_SHARPEN_COEF     ISP_SHARPEN_COEF                  ; /* 0x4521c */
    volatile U_ISP_SHARPEN_SHOOTAMT   ISP_SHARPEN_SHOOTAMT            ; /* 0x45220 */
    volatile U_ISP_SHARPEN_JAGTHD   ISP_SHARPEN_JAGTHD                ; /* 0x45224 */
    volatile U_ISP_SHARPEN_EDGEJAG   ISP_SHARPEN_EDGEJAG              ; /* 0x45228 */
    volatile U_ISP_SHARPEN_OSHTJAG   ISP_SHARPEN_OSHTJAG              ; /* 0x4522c */
    volatile U_ISP_SHARPEN_USHTJAG   ISP_SHARPEN_USHTJAG              ; /* 0x45230 */
    volatile U_ISP_SHARPEN_MID0     ISP_SHARPEN_MID0                  ; /* 0x45234 */
    volatile U_ISP_SHARPEN_MID1     ISP_SHARPEN_MID1                  ; /* 0x45238 */
    volatile U_ISP_SHARPEN_MID2     ISP_SHARPEN_MID2                  ; /* 0x4523c */
    volatile unsigned int           reserved_201_1                     ; /* 0x45240 */
    volatile U_ISP_SHARPEN_CTRL     ISP_SHARPEN_CTRL                  ; /* 0x45244 */
    volatile U_ISP_SHARPEN_SHIFT    ISP_SHARPEN_SHIFT                 ; /* 0x45248 */
    volatile U_ISP_SHARPEN_ST       ISP_SHARPEN_ST                    ; /* 0x4524c */
    volatile U_ISP_SHARPEN_OSHTVARTH   ISP_SHARPEN_OSHTVARTH          ; /* 0x45250 */
    volatile U_ISP_SHARPEN_OSHTVAR   ISP_SHARPEN_OSHTVAR              ; /* 0x45254 */
    volatile U_ISP_SHARPEN_USHTVARTH   ISP_SHARPEN_USHTVARTH          ; /* 0x45258 */
    volatile U_ISP_SHARPEN_USHTVAR   ISP_SHARPEN_USHTVAR              ; /* 0x4525c */
	volatile U_ISP_SHARPEN_LUMAWGT   ISP_SHARPEN_LUMAWGT[8]            ; /* 0x45260 */
    //volatile U_ISP_SHARPEN_LUMAWGT0   ISP_SHARPEN_LUMAWGT0            ; /* 0x45260 */
    //volatile U_ISP_SHARPEN_LUMAWGT1   ISP_SHARPEN_LUMAWGT1            ; /* 0x45264 */
    //volatile U_ISP_SHARPEN_LUMAWGT2   ISP_SHARPEN_LUMAWGT2            ; /* 0x45268 */
    //volatile U_ISP_SHARPEN_LUMAWGT3   ISP_SHARPEN_LUMAWGT3            ; /* 0x4526c */
    //volatile U_ISP_SHARPEN_LUMAWGT4   ISP_SHARPEN_LUMAWGT4            ; /* 0x45270 */
    //volatile U_ISP_SHARPEN_LUMAWGT5   ISP_SHARPEN_LUMAWGT5            ; /* 0x45274 */
    //volatile U_ISP_SHARPEN_LUMAWGT6   ISP_SHARPEN_LUMAWGT6            ; /* 0x45278 */
    //volatile U_ISP_SHARPEN_LUMAWGT7   ISP_SHARPEN_LUMAWGT7            ; /* 0x4527c */
    volatile unsigned int           reserved_202[25]                     ; /* 0x45280~0x452e0 */
    volatile U_ISP_SHARPEN_CTRL_I   ISP_SHARPEN_CTRL_I                ; /* 0x452e4 */
    volatile U_ISP_SHARPEN_TIMING   ISP_SHARPEN_TIMING                ; /* 0x452e8 */
    volatile U_ISP_SHARPEN_UPDATE   ISP_SHARPEN_UPDATE                ; /* 0x452ec */
    volatile U_ISP_SHARPEN_SIZE     ISP_SHARPEN_SIZE                  ; /* 0x452f0 */
    volatile unsigned int           reserved_203[7]                     ; /* 0x452f4~0x4530c */
    volatile U_ISP_SHARPEN_SKIN_CNT   ISP_SHARPEN_SKIN_CNT            ; /* 0x45310 */
    volatile U_ISP_SHARPEN_SKIN_VARTH   ISP_SHARPEN_SKIN_VARTH        ; /* 0x45314 */
    volatile U_ISP_SHARPEN_SKIN_VARWGT   ISP_SHARPEN_SKIN_VARWGT      ; /* 0x45318 */
    volatile U_ISP_SHARPEN_SKIN_U   ISP_SHARPEN_SKIN_U                ; /* 0x4531c */
    volatile U_ISP_SHARPEN_SKIN_V   ISP_SHARPEN_SKIN_V                ; /* 0x45320 */
    volatile U_ISP_SHARPEN_SKIN_LEN   ISP_SHARPEN_SKIN_LEN            ; /* 0x45324 */
    volatile U_ISP_SHARPEN_EDGSM    ISP_SHARPEN_EDGSM                 ; /* 0x45328 */
    volatile U_ISP_SHARPEN_SHPGAIN_MUL   ISP_SHARPEN_SHPGAIN_MUL      ; /* 0x4532c */
    volatile U_ISP_SHARPEN_OSUP_VAR   ISP_SHARPEN_OSUP_VAR            ; /* 0x45330 */
    volatile U_ISP_SHARPEN_OSUP_VARCOEF   ISP_SHARPEN_OSUP_VARCOEF    ; /* 0x45334 */
    volatile U_ISP_SHARPEN_USUP_VAR   ISP_SHARPEN_USUP_VAR            ; /* 0x45338 */
    volatile U_ISP_SHARPEN_USUP_VARCOEF   ISP_SHARPEN_USUP_VARCOEF    ; /* 0x4533c */
    volatile U_ISP_SHARPEN_OSUP_DIFF   ISP_SHARPEN_OSUP_DIFF          ; /* 0x45340 */
    volatile U_ISP_SHARPEN_OSUP_DIFFCOEF   ISP_SHARPEN_OSUP_DIFFCOEF  ; /* 0x45344 */
    volatile U_ISP_SHARPEN_USUP_DIFF   ISP_SHARPEN_USUP_DIFF          ; /* 0x45348 */
    volatile U_ISP_SHARPEN_USUP_DIFFCOEF   ISP_SHARPEN_USUP_DIFFCOEF  ; /* 0x4534c */
    volatile U_ISP_SHARPEN_SUP_NOS   ISP_SHARPEN_SUP_NOS              ; /* 0x45350 */
    volatile U_ISP_SHARPEN_SUP_BLDR   ISP_SHARPEN_SUP_BLDR            ; /* 0x45354 */
    volatile U_ISP_SHARPEN_UDHFGAIN   ISP_SHARPEN_UDHFGAIN            ; /* 0x45358 */
    volatile U_ISP_SHARPEN_SHTMAX   ISP_SHARPEN_SHTMAX                ; /* 0x4535c */
    volatile U_ISP_SHARPEN_SHADVAR   ISP_SHARPEN_SHADVAR              ; /* 0x45360 */
    volatile U_ISP_SHARPEN_GRADTHD   ISP_SHARPEN_GRADTHD              ; /* 0x45364 */
    volatile U_ISP_SHARPEN_WGTCOEF   ISP_SHARPEN_WGTCOEF              ; /* 0x45368 */
    volatile U_ISP_SHARPEN_RC       ISP_SHARPEN_RC                    ; /* 0x4536c */
    volatile U_ISP_SHARPEN_BC       ISP_SHARPEN_BC                    ; /* 0x45370 */
    volatile U_ISP_SHARPEN_GC       ISP_SHARPEN_GC                    ; /* 0x45374 */
    volatile U_ISP_SHARPEN_RGBWGT   ISP_SHARPEN_RGBWGT                ; /* 0x45378 */
    volatile U_ISP_SHARPEN_RBCOEF   ISP_SHARPEN_RBCOEF                ; /* 0x4537c */
    volatile unsigned int           reserved_204[25]                     ; /* 0x45380~0x453e0 */
    volatile U_ISP_SHARPEN_SKIN_CTRL_I   ISP_SHARPEN_SKIN_CTRL_I      ; /* 0x453e4 */
    volatile unsigned int           reserved_205                     ; /* 0x453e8 */
    volatile U_ISP_SHARPEN_SKIN_UPDATE   ISP_SHARPEN_SKIN_UPDATE      ; /* 0x453ec */
    volatile unsigned int           reserved_206[132]                     ; /* 0x453f0~0x455fc */
    volatile U_ISP_NDDM_CFG         ISP_NDDM_CFG                      ; /* 0x45600 */
    volatile U_ISP_NDDM_GF_TH       ISP_NDDM_GF_TH                    ; /* 0x45604 */
    volatile U_ISP_NDDM_GF_BLDR     ISP_NDDM_GF_BLDR                  ; /* 0x45608 */
    volatile U_ISP_NDDM_GF_CHRM     ISP_NDDM_GF_CHRM                  ; /* 0x4560c */
    volatile U_ISP_NDDM_USM_CFG     ISP_NDDM_USM_CFG                  ; /* 0x45610 */
    volatile U_ISP_NDDM_USM_SATU    ISP_NDDM_USM_SATU                 ; /* 0x45614 */
    volatile U_ISP_NDDM_USM_GRAY    ISP_NDDM_USM_GRAY                 ; /* 0x45618 */
    volatile U_ISP_NDDM_APT_INTP_BLD   ISP_NDDM_APT_INTP_BLD          ; /* 0x4561c */
    volatile unsigned int           reserved_206_1                     ; /* 0x45620 */
    volatile U_ISP_NDDM_DITHER_CFG   ISP_NDDM_DITHER_CFG              ; /* 0x45624 */
    volatile U_ISP_NDDM_FCR_LIMIT   ISP_NDDM_FCR_LIMIT                ; /* 0x45628 */
    volatile U_ISP_NDDM_FCR_GAIN    ISP_NDDM_FCR_GAIN                 ; /* 0x4562c */
    volatile U_ISP_NDDM_FCR_SCALE   ISP_NDDM_FCR_SCALE                ; /* 0x45630 */
    volatile U_ISP_NDDM_DM_BLDRATE   ISP_NDDM_DM_BLDRATE              ; /* 0x45634 */
    volatile U_ISP_NDDM_GF_LUT_UPDATE   ISP_NDDM_GF_LUT_UPDATE        ; /* 0x45638 */
    volatile unsigned int           reserved_2066                     ; /* 0x4563c */
    volatile U_ISP_NDDM_USM_LUT_UPDATE   ISP_NDDM_USM_LUT_UPDATE      ; /* 0x45640 */
    volatile U_ISP_NDDM_USM_GRAY_TH_FIX2   ISP_NDDM_USM_GRAY_TH_FIX2  ; /* 0x45644 */
    volatile U_ISP_NDDM_APT_INTP_CLIP2   ISP_NDDM_APT_INTP_CLIP2      ; /* 0x45648 */
    volatile U_ISP_NDDM_APT_AVG_CFG_0   ISP_NDDM_APT_AVG_CFG_0        ; /* 0x4564c */
    volatile U_ISP_NDDM_APT_AVG_CFG_1   ISP_NDDM_APT_AVG_CFG_1        ; /* 0x45650 */
    volatile U_ISP_NDDM_VAR_OFFSET_CFG   ISP_NDDM_VAR_OFFSET_CFG      ; /* 0x45654 */
    volatile U_ISP_NDDM_SHT_CTRL    ISP_NDDM_SHT_CTRL                 ; /* 0x45658 */
    volatile U_ISP_NDDM_SHT_CLIP_R   ISP_NDDM_SHT_CLIP_R              ; /* 0x4565c */
    volatile U_ISP_NDDM_SHT_CLIP_B   ISP_NDDM_SHT_CLIP_B              ; /* 0x45660 */
    volatile U_ISP_NDDM_MULTI_MF_RB   ISP_NDDM_MULTI_MF_RB            ; /* 0x45664 */
    volatile U_ISP_NDDM_USM_SATU_R   ISP_NDDM_USM_SATU_R              ; /* 0x45668 */
    volatile U_ISP_NDDM_USM_SATU_B   ISP_NDDM_USM_SATU_B              ; /* 0x4566c */
    volatile U_ISP_NDDM_SATU_FIX_EHCY   ISP_NDDM_SATU_FIX_EHCY        ; /* 0x45670 */
    volatile unsigned int           reserved_20666[3]                 ; /* 0x45674~0x4567c */
    volatile U_ISP_NDDM_GF_LUT_WADDR   ISP_NDDM_GF_LUT_WADDR          ; /* 0x45680 */
    volatile U_ISP_NDDM_GF_LUT_WDATA   ISP_NDDM_GF_LUT_WDATA          ; /* 0x45684 */
    volatile U_ISP_NDDM_GF_LUT_RADDR   ISP_NDDM_GF_LUT_RADDR          ; /* 0x45688 */
    volatile U_ISP_NDDM_GF_LUT_RDATA   ISP_NDDM_GF_LUT_RDATA          ; /* 0x4568c */
    volatile U_ISP_NDDM_USM_LUT_WADDR   ISP_NDDM_USM_LUT_WADDR        ; /* 0x45690 */
    volatile U_ISP_NDDM_USM_LUT_WDATA   ISP_NDDM_USM_LUT_WDATA        ; /* 0x45694 */
    volatile U_ISP_NDDM_USM_LUT_RADDR   ISP_NDDM_USM_LUT_RADDR        ; /* 0x45698 */
    volatile U_ISP_NDDM_USM_LUT_RDATA   ISP_NDDM_USM_LUT_RDATA        ; /* 0x4569c */
    volatile unsigned int           reserved_206666[16]                     ; /* 0x456a0~0x456dc */
    volatile U_ISP_NDDM_CTRL_F      ISP_NDDM_CTRL_F                   ; /* 0x456e0 */
    volatile U_ISP_NDDM_CTRL_I      ISP_NDDM_CTRL_I                   ; /* 0x456e4 */
    volatile U_ISP_NDDM_TIMING      ISP_NDDM_TIMING                   ; /* 0x456e8 */
    volatile U_ISP_NDDM_UPDATE      ISP_NDDM_UPDATE                   ; /* 0x456ec */
    volatile U_ISP_NDDM_SIZE        ISP_NDDM_SIZE                     ; /* 0x456f0 */
    volatile unsigned int           reserved_nd2da[1027]              ; /* 0x456f4~0x466fc */
    volatile U_ISP_DEHAZE_CFG       ISP_DEHAZE_CFG                    ; /* 0x46700 */
    volatile unsigned int           reserved_207[2]                   ; /* 0x46704~0x46708 */
    volatile U_ISP_DEHAZE_VERSION   ISP_DEHAZE_VERSION                ; /* 0x4670c */
    volatile U_ISP_DEHAZE_PRE_UPDATE   ISP_DEHAZE_PRE_UPDATE          ; /* 0x46710 */
    volatile U_ISP_DEHAZE_BLK_SIZE   ISP_DEHAZE_BLK_SIZE              ; /* 0x46714 */
    volatile U_ISP_DEHAZE_BLK_SUM   ISP_DEHAZE_BLK_SUM                ; /* 0x46718 */
    volatile U_ISP_DEHAZE_DC_SIZE   ISP_DEHAZE_DC_SIZE                ; /* 0x4621c */
    volatile U_ISP_DEHAZE_X         ISP_DEHAZE_X                      ; /* 0x46220 */
    volatile U_ISP_DEHAZE_Y         ISP_DEHAZE_Y                      ; /* 0x46224 */
    volatile U_ISP_DEHAZE_STAT_MODE   ISP_DEHAZE_STAT_MODE            ; /* 0x46228 */
    volatile U_ISP_DEHAZE_NEG_MODE   ISP_DEHAZE_NEG_MODE              ; /* 0x4622c */
    volatile U_ISP_DEHAZE_AIR       ISP_DEHAZE_AIR                    ; /* 0x46230 */
    volatile U_ISP_DEHAZE_THLD      ISP_DEHAZE_THLD                   ; /* 0x46234 */
    volatile U_ISP_DEHAZE_GSTRTH    ISP_DEHAZE_GSTRTH                 ; /* 0x46238 */
    volatile U_ISP_DEHAZE_BLTHLD    ISP_DEHAZE_BLTHLD                 ; /* 0x4623c */
    volatile U_ISP_DEHAZE_STR_LUT_UPDATE   ISP_DEHAZE_STR_LUT_UPDATE  ; /* 0x46240 */
    volatile unsigned int           reserved_208[15]                     ; /* 0x46244~0x4627c */
    volatile U_ISP_DEHAZE_MINSTAT_WADDR   ISP_DEHAZE_MINSTAT_WADDR    ; /* 0x46280 */
    volatile U_ISP_DEHAZE_MINSTAT_WDATA   ISP_DEHAZE_MINSTAT_WDATA    ; /* 0x46284 */
    volatile U_ISP_DEHAZE_MINSTAT_RADDR   ISP_DEHAZE_MINSTAT_RADDR    ; /* 0x46288 */
    volatile U_ISP_DEHAZE_MINSTAT_RDATA   ISP_DEHAZE_MINSTAT_RDATA    ; /* 0x4628c */
    volatile U_ISP_DEHAZE_MAXSTAT_WADDR   ISP_DEHAZE_MAXSTAT_WADDR    ; /* 0x46290 */
    volatile U_ISP_DEHAZE_MAXSTAT_WDATA   ISP_DEHAZE_MAXSTAT_WDATA    ; /* 0x46294 */
    volatile U_ISP_DEHAZE_MAXSTAT_RADDR   ISP_DEHAZE_MAXSTAT_RADDR    ; /* 0x46298 */
    volatile U_ISP_DEHAZE_MAXSTAT_RDATA   ISP_DEHAZE_MAXSTAT_RDATA    ; /* 0x4629c */
    volatile U_ISP_DEHAZE_PRESTAT_WADDR   ISP_DEHAZE_PRESTAT_WADDR    ; /* 0x462a0 */
    volatile U_ISP_DEHAZE_PRESTAT_WDATA   ISP_DEHAZE_PRESTAT_WDATA    ; /* 0x462a4 */
    volatile U_ISP_DEHAZE_PRESTAT_RADDR   ISP_DEHAZE_PRESTAT_RADDR    ; /* 0x462a8 */
    volatile U_ISP_DEHAZE_PRESTAT_RDATA   ISP_DEHAZE_PRESTAT_RDATA    ; /* 0x462ac */
    volatile U_ISP_DEHAZE_LUT_WADDR   ISP_DEHAZE_LUT_WADDR            ; /* 0x462b0 */
    volatile U_ISP_DEHAZE_LUT_WDATA   ISP_DEHAZE_LUT_WDATA            ; /* 0x462b4 */
    volatile U_ISP_DEHAZE_LUT_RADDR   ISP_DEHAZE_LUT_RADDR            ; /* 0x462b8 */
    volatile U_ISP_DEHAZE_LUT_RDATA   ISP_DEHAZE_LUT_RDATA            ; /* 0x462bc */
    volatile unsigned int           reserved_209[9]                     ; /* 0x462c0~0x462e0 */
    volatile U_ISP_DEHAZE_CTRL_I    ISP_DEHAZE_CTRL_I                 ; /* 0x462e4 */
    volatile unsigned int           reserved_210                     ; /* 0x462e8 */
    volatile U_ISP_DEHAZE_UPDATE    ISP_DEHAZE_UPDATE                 ; /* 0x462ec */
    volatile U_ISP_DEHAZE_SIZE      ISP_DEHAZE_SIZE                   ; /* 0x467f0 */    
    volatile unsigned int           reserved_211[131]                 ; /* 0x467f4~0x469fc */
	volatile U_ISP_GAMMA_CFG        ISP_GAMMA_CFG                     ; /* 0x46a00 */
	volatile unsigned int           reserved_212[2]                     ; /* 0x46a04~0x46a08 */
	volatile U_ISP_GAMMA_VERSION    ISP_GAMMA_VERSION                 ; /* 0x46a0c */
	volatile U_ISP_GAMMA_LUT_UPDATE   ISP_GAMMA_LUT_UPDATE            ; /* 0x46a10 */
	volatile unsigned int           reserved_213[27]                     ; /* 0x46a14~0x46a7c */
	volatile U_ISP_GAMMA_WADDR      ISP_GAMMA_WADDR                   ; /* 0x46a80 */
	volatile U_ISP_GAMMA_WDATA      ISP_GAMMA_WDATA                   ; /* 0x46a84 */
	volatile U_ISP_GAMMA_RADDR      ISP_GAMMA_RADDR                   ; /* 0x46a88 */
	volatile U_ISP_GAMMA_RDATA      ISP_GAMMA_RDATA                   ; /* 0x46a8c */
	volatile unsigned int           reserved_214[21]                     ; /* 0x46a90~0x46ae0 */
	volatile U_ISP_GAMMA_CTRL_I     ISP_GAMMA_CTRL_I                  ; /* 0x46ae4 */
	volatile unsigned int           reserved_215                     ; /* 0x46ae8 */
	volatile U_ISP_GAMMA_UPDATE     ISP_GAMMA_UPDATE                  ; /* 0x46aec */
    volatile unsigned int           reserved_2151[324]                ; /* 0x46af0~0x46ffc */
	volatile U_ISP_CA_CFG           ISP_CA_CFG                        ; /* 0x47000 */
	volatile unsigned int           reserved_2152[3]                  ; /* 0x47004~0x4700c */
	volatile U_ISP_CA_CTRL          ISP_CA_CTRL                       ; /* 0x47010 */
	volatile U_ISP_CA_LUMATH        ISP_CA_LUMATH                     ; /* 0x47014 */
	volatile U_ISP_CA_DARKCHROMA_TH   ISP_CA_DARKCHROMA_TH            ; /* 0x47018 */
	volatile U_ISP_CA_SDARKCHROMA_TH   ISP_CA_SDARKCHROMA_TH          ; /* 0x4701c */
	volatile U_ISP_CA_LLHC_RATIO    ISP_CA_LLHC_RATIO                 ; /* 0x47020 */
	volatile U_ISP_CA_ISORATIO      ISP_CA_ISORATIO                   ; /* 0x47024 */
	volatile U_ISP_CA_LUT_UPDATE    ISP_CA_LUT_UPDATE                 ; /* 0x47028 */
	volatile U_ISP_CA_YUV2RGB_COEF0   ISP_CA_YUV2RGB_COEF0            ; /* 0x4702c */
	volatile U_ISP_CA_YUV2RGB_COEF1   ISP_CA_YUV2RGB_COEF1            ; /* 0x47030 */
	volatile U_ISP_CA_YUV2RGB_COEF2   ISP_CA_YUV2RGB_COEF2            ; /* 0x47034 */
	volatile U_ISP_CA_YUV2RGB_COEF3   ISP_CA_YUV2RGB_COEF3            ; /* 0x47038 */
	volatile U_ISP_CA_YUV2RGB_COEF4   ISP_CA_YUV2RGB_COEF4            ; /* 0x4703c */
	volatile U_ISP_CA_YUV2RGB_INDC0   ISP_CA_YUV2RGB_INDC0            ; /* 0x47040 */
	volatile U_ISP_CA_YUV2RGB_INDC1   ISP_CA_YUV2RGB_INDC1            ; /* 0x47044 */
	volatile U_ISP_CA_YUV2RGB_OUTDC0   ISP_CA_YUV2RGB_OUTDC0          ; /* 0x47048 */
	volatile U_ISP_CA_YUV2RGB_OUTDC1   ISP_CA_YUV2RGB_OUTDC1          ; /* 0x4704c */
	volatile U_ISP_CA_RGB2YUV_COEF0   ISP_CA_RGB2YUV_COEF0            ; /* 0x47050 */
	volatile U_ISP_CA_RGB2YUV_COEF1   ISP_CA_RGB2YUV_COEF1            ; /* 0x47054 */
	volatile U_ISP_CA_RGB2YUV_COEF2   ISP_CA_RGB2YUV_COEF2            ; /* 0x47058 */
	volatile U_ISP_CA_RGB2YUV_COEF3   ISP_CA_RGB2YUV_COEF3            ; /* 0x4705c */
	volatile U_ISP_CA_RGB2YUV_COEF4   ISP_CA_RGB2YUV_COEF4            ; /* 0x47060 */
	volatile U_ISP_CA_RGB2YUV_INDC0   ISP_CA_RGB2YUV_INDC0            ; /* 0x47064 */
	volatile U_ISP_CA_RGB2YUV_INDC1   ISP_CA_RGB2YUV_INDC1            ; /* 0x47068 */
	volatile U_ISP_CA_RGB2YUV_OUTDC0   ISP_CA_RGB2YUV_OUTDC0          ; /* 0x4706c */
	volatile U_ISP_CA_RGB2YUV_OUTDC1   ISP_CA_RGB2YUV_OUTDC1          ; /* 0x47070 */
	volatile U_ISP_CA_SKINLLUMA_UTH   ISP_CA_SKINLLUMA_UTH            ; /* 0x47074 */
	volatile U_ISP_CA_SKINLLUMA_UYTH   ISP_CA_SKINLLUMA_UYTH          ; /* 0x47078 */
	volatile U_ISP_CA_SKINHLUMA_UTH   ISP_CA_SKINHLUMA_UTH            ; /* 0x4707c */
	volatile U_ISP_CA_GAINLUT_WADDR   ISP_CA_GAINLUT_WADDR            ; /* 0x47080 */
	volatile U_ISP_CA_GAINLUT_WDATA   ISP_CA_GAINLUT_WDATA            ; /* 0x47084 */
	volatile U_ISP_CA_GAINLUT_RADDR   ISP_CA_GAINLUT_RADDR            ; /* 0x47088 */
	volatile U_ISP_CA_GAINLUT_RDATA   ISP_CA_GAINLUT_RDATA            ; /* 0x4708c */
	volatile unsigned int           reserved_2153[32]                 ; /* 0x47090~0x4710c */
	volatile U_ISP_CA_SKINHLUMA_UYTH   ISP_CA_SKINHLUMA_UYTH          ; /* 0x47110 */
	volatile U_ISP_CA_SKINLLUMA_VTH   ISP_CA_SKINLLUMA_VTH            ; /* 0x47114 */
	volatile U_ISP_CA_SKINLLUMA_VYTH   ISP_CA_SKINLLUMA_VYTH          ; /* 0x47118 */
	volatile U_ISP_CA_SKINHLUMA_VTH   ISP_CA_SKINHLUMA_VTH            ; /* 0x4711c */
	volatile U_ISP_CA_SKINHLUMA_VYTH   ISP_CA_SKINHLUMA_VYTH          ; /* 0x47120 */
	volatile U_ISP_CA_SKIN_UVDIFF   ISP_CA_SKIN_UVDIFF                ; /* 0x47124 */
	volatile U_ISP_CA_SKIN_RATIOTH0   ISP_CA_SKIN_RATIOTH0            ; /* 0x47128 */
	volatile U_ISP_CA_SKIN_RATIOTH1   ISP_CA_SKIN_RATIOTH1            ; /* 0x4712c */
	volatile U_ISP_CA_COLORLLUMA_UTH   ISP_CA_COLORLLUMA_UTH          ; /* 0x47130 */
	volatile U_ISP_CA_COLORLLUMA_UYTH   ISP_CA_COLORLLUMA_UYTH        ; /* 0x47134 */
	volatile U_ISP_CA_COLORHLUMA_UTH   ISP_CA_COLORHLUMA_UTH          ; /* 0x47138 */
	volatile U_ISP_CA_COLORHLUMA_UYTH   ISP_CA_COLORHLUMA_UYTH        ; /* 0x4713c */
	volatile U_ISP_CA_COLORLLUMA_VTH   ISP_CA_COLORLLUMA_VTH          ; /* 0x47140 */
	volatile U_ISP_CA_COLORLLUMA_VYTH   ISP_CA_COLORLLUMA_VYTH        ; /* 0x47144 */
	volatile U_ISP_CA_COLORHLUMA_VTH   ISP_CA_COLORHLUMA_VTH          ; /* 0x47148 */
	volatile U_ISP_CA_COLORHLUMA_VYTH   ISP_CA_COLORHLUMA_VYTH        ; /* 0x4714c */
	volatile U_ISP_CA_COLOR_UVDIFF   ISP_CA_COLOR_UVDIFF              ; /* 0x47150 */
	volatile U_ISP_CA_COLOR_RATIOTH0   ISP_CA_COLOR_RATIOTH0          ; /* 0x47154 */
	volatile U_ISP_CA_COLOR_RATIOTH1   ISP_CA_COLOR_RATIOTH1          ; /* 0x47158 */
	volatile unsigned int           reserved_2154[34]                 ; /* 0x4715c~0x471e0 */
	volatile U_ISP_CA_CTRL_I        ISP_CA_CTRL_I                     ; /* 0x471e4 */
	volatile unsigned int           reserved_2155                     ; /* 0x471e8 */
	volatile U_ISP_CA_UPDATE        ISP_CA_UPDATE                     ; /* 0x471ec */
	volatile U_ISP_CA_SIZE          ISP_CA_SIZE                       ; /* 0x471f0 */
	volatile unsigned int           reserved_216[10307]               ; /* 0x471f4~0x512fc */
	volatile U_ISP_HLDC_CFG         ISP_HLDC_CFG                      ; /* 0x51300 */
	volatile unsigned int           reserved_217[3]                   ; /* 0x51304~0x5130c */
	volatile U_ISP_HLDC_SIZE        ISP_HLDC_SIZE                     ; /* 0x51310 */
	volatile unsigned int           reserved_218[3]                   ; /* 0x51314~0x5131c */
	volatile U_ISP_HLDC_CENTER      ISP_HLDC_CENTER                   ; /* 0x51320 */
	volatile U_ISP_HLDC_PARA        ISP_HLDC_PARA                     ; /* 0x51324 */
	volatile unsigned int           reserved_219[54]                  ; /* 0x51328~0x513fc */
	volatile U_ISP_ACM_CTRL         ISP_ACM_CTRL                      ; /* 0x51400 */
	volatile U_ISP_ACM_ADJ          ISP_ACM_ADJ                       ; /* 0x51404 */
	volatile U_ISP_ACM_PARA_REN     ISP_ACM_PARA_REN                  ; /* 0x51408 */
	volatile U_ISP_ACM_PARA_DATA    ISP_ACM_PARA_DATA                 ; /* 0x5140c */
	volatile U_ISP_ACM_SIZE         ISP_ACM_SIZE                      ; /* 0x51410 */
	volatile U_ISP_ACM_PARA_ADDR    ISP_ACM_PARA_ADDR                 ; /* 0x51414 */
	volatile unsigned int           reserved_220[2]                   ; /* 0x51418~0x5141c */
	volatile U_ISP_ACM_PARA_UP      ISP_ACM_PARA_UP                   ; /* 0x51420 */
	volatile unsigned int           reserved_221[503]                 ; /* 0x51424~0x51bfc */
	volatile U_ISP_VPDCICTRL        ISP_VPDCICTRL                     ; /* 0x51c00 */
	volatile U_ISP_VPDCIHPOS        ISP_VPDCIHPOS                     ; /* 0x51c04 */
	volatile U_ISP_VPDCIVPOS        ISP_VPDCIVPOS                     ; /* 0x51c08 */
	volatile U_ISP_VPDCIHISBLD      ISP_VPDCIHISBLD                   ; /* 0x51c0c */
	volatile U_ISP_VPDCIHISOFT      ISP_VPDCIHISOFT                   ; /* 0x51c10 */
	volatile U_ISP_VPDCIHISCOR      ISP_VPDCIHISCOR                   ; /* 0x51c14 */
	volatile U_ISP_VPDCIMERBLD      ISP_VPDCIMERBLD                   ; /* 0x51c18 */
	volatile U_ISP_VPDCIADJWGT      ISP_VPDCIADJWGT                   ; /* 0x51c1c */
	volatile U_ISP_VPDCICLIP       ISP_VPDCICLIP[3]                    ; /* 0x51c20 ~ 0x51c28*/
    //volatile U_ISP_VPDCICLIP1       ISP_VPDCICLIP1                    ; /* 0x51c24 */
    //volatile U_ISP_VPDCICLIP2       ISP_VPDCICLIP2                    ; /* 0x51c28 */
	volatile U_ISP_VPDCIGLBGAIN     ISP_VPDCIGLBGAIN                  ; /* 0x51c2c */
	volatile U_ISP_VPDCIPOSTHR      ISP_VPDCIPOSTHR[2]                ; /* 0x51c30 ~ 0x51c34*/
    //volatile U_ISP_VPDCIPOSTHR0     ISP_VPDCIPOSTHR0                  ; /* 0x51c30 */
    //volatile U_ISP_VPDCIPOSTHR1     ISP_VPDCIPOSTHR1                  ; /* 0x51c34 */
    volatile U_ISP_VPDCIPOSGAIN     ISP_VPDCIPOSGAIN[2]                 ; /* 0x51c38~0x51c3c */
    //volatile U_ISP_VPDCIPOSGAIN0    ISP_VPDCIPOSGAIN0                 ; /* 0x51c38 */
    //volatile U_ISP_VPDCIPOSGAIN1    ISP_VPDCIPOSGAIN1                 ; /* 0x51c3c */
    volatile U_ISP_VPDCIPOSSLP      ISP_VPDCIPOSSLP[3]                ; /* 0x51c40 ~ 0x51c48*/
    //volatile U_ISP_VPDCIPOSSLP0     ISP_VPDCIPOSSLP0                  ; /* 0x51c40 */
    //volatile U_ISP_VPDCIPOSSLP1     ISP_VPDCIPOSSLP1                  ; /* 0x51c44 */
    //volatile U_ISP_VPDCIPOSSLP2     ISP_VPDCIPOSSLP2                  ; /* 0x51c48 */
    volatile U_ISP_VPDCINEGTHR      ISP_VPDCINEGTHR[2]                ; /* 0x51c4c ~ 0x51c50*/
    //volatile U_ISP_VPDCINEGTHR0     ISP_VPDCINEGTHR0                  ; /* 0x51c4c */
    //volatile U_ISP_VPDCINEGTHR1     ISP_VPDCINEGTHR1                  ; /* 0x51c50 */
    volatile U_ISP_VPDCINEGGAIN     ISP_VPDCINEGGAIN[2]                 ; /* 0x51c54 ~ 0x51c58*/
    //volatile U_ISP_VPDCINEGGAIN0    ISP_VPDCINEGGAIN0                 ; /* 0x51c54 */
    //volatile U_ISP_VPDCINEGGAIN1    ISP_VPDCINEGGAIN1                 ; /* 0x51c58 */
    volatile U_ISP_VPDCINEGSLP      ISP_VPDCINEGSLP[3]                ; /* 0x51c5c ~ 0x51c64*/
    //volatile U_ISP_VPDCINEGSLP0     ISP_VPDCINEGSLP0                  ; /* 0x51c5c */
    //volatile U_ISP_VPDCINEGSLP1     ISP_VPDCINEGSLP1                  ; /* 0x51c60 */
    //volatile U_ISP_VPDCINEGSLP2     ISP_VPDCINEGSLP2                  ; /* 0x51c64 */
	volatile U_ISP_VPDCI_RESERVED0   ISP_VPDCI_RESERVED0              ; /* 0x51c68 */
	volatile U_ISP_VPDCI_RESERVED1   ISP_VPDCI_RESERVED1              ; /* 0x51c6c */
	volatile U_ISP_VPDCI_RESERVED2   ISP_VPDCI_RESERVED2              ; /* 0x51c70 */
	volatile U_ISP_VPDCI_RESERVED3   ISP_VPDCI_RESERVED3              ; /* 0x51c74 */
	volatile U_ISP_VPDCI_RESERVED4   ISP_VPDCI_RESERVED4              ; /* 0x51c78 */
	volatile U_ISP_VPDCI_RESERVED5   ISP_VPDCI_RESERVED5              ; /* 0x51c7c */
	volatile U_ISP_VPDCI_RESERVED6   ISP_VPDCI_RESERVED6              ; /* 0x51c80 */
	volatile U_ISP_VPDCI_RESERVED7   ISP_VPDCI_RESERVED7              ; /* 0x51c84 */
	volatile U_ISP_VPDCI_RESERVED8   ISP_VPDCI_RESERVED8              ; /* 0x51c88 */
	volatile U_ISP_VPDCI_RESERVED9   ISP_VPDCI_RESERVED9              ; /* 0x51c8c */
	volatile U_ISP_VPDCI_RESERVED10   ISP_VPDCI_RESERVED10            ; /* 0x51c90 */
	volatile U_ISP_VPDCI_RESERVED11   ISP_VPDCI_RESERVED11            ; /* 0x51c94 */
	volatile U_ISP_VDDCIBINWGT      ISP_VDDCIBINWGT[24]               ; /* 0x51c98 ~ 0x51cf4*/
    //volatile U_ISP_VDDCIBINWGT00    ISP_VDDCIBINWGT00                 ; /* 0x51c98 */
    //volatile U_ISP_VDDCIBINWGT01    ISP_VDDCIBINWGT01                 ; /* 0x51c9c */
    //volatile U_ISP_VDDCIBINWGT02    ISP_VDDCIBINWGT02                 ; /* 0x51ca0 */
    //volatile U_ISP_VDDCIBINWGT03    ISP_VDDCIBINWGT03                 ; /* 0x51ca4 */
    //volatile U_ISP_VDDCIBINWGT04    ISP_VDDCIBINWGT04                 ; /* 0x51ca8 */
    //volatile U_ISP_VDDCIBINWGT05    ISP_VDDCIBINWGT05                 ; /* 0x51cac */
    //volatile U_ISP_VDDCIBINWGT06    ISP_VDDCIBINWGT06                 ; /* 0x51cb0 */
    //volatile U_ISP_VDDCIBINWGT07    ISP_VDDCIBINWGT07                 ; /* 0x51cb4 */
    //volatile U_ISP_VDDCIBINWGT10    ISP_VDDCIBINWGT10                 ; /* 0x51cb8 */
    //volatile U_ISP_VDDCIBINWGT11    ISP_VDDCIBINWGT11                 ; /* 0x51cbc */
    //volatile U_ISP_VDDCIBINWGT12    ISP_VDDCIBINWGT12                 ; /* 0x51cc0 */
    //volatile U_ISP_VDDCIBINWGT13    ISP_VDDCIBINWGT13                 ; /* 0x51cc4 */
    //volatile U_ISP_VDDCIBINWGT14    ISP_VDDCIBINWGT14                 ; /* 0x51cc8 */
    //volatile U_ISP_VDDCIBINWGT15    ISP_VDDCIBINWGT15                 ; /* 0x51ccc */
    //volatile U_ISP_VDDCIBINWGT16    ISP_VDDCIBINWGT16                 ; /* 0x51cd0 */
    //volatile U_ISP_VDDCIBINWGT17    ISP_VDDCIBINWGT17                 ; /* 0x51cd4 */
    //volatile U_ISP_VDDCIBINWGT20    ISP_VDDCIBINWGT20                 ; /* 0x51cd8 */
    //volatile U_ISP_VDDCIBINWGT21    ISP_VDDCIBINWGT21                 ; /* 0x51cdc */
    //volatile U_ISP_VDDCIBINWGT22    ISP_VDDCIBINWGT22                 ; /* 0x51ce0 */
    //volatile U_ISP_VDDCIBINWGT23    ISP_VDDCIBINWGT23                 ; /* 0x51ce4 */
    //volatile U_ISP_VDDCIBINWGT24    ISP_VDDCIBINWGT24                 ; /* 0x51ce8 */
    //volatile U_ISP_VDDCIBINWGT25    ISP_VDDCIBINWGT25                 ; /* 0x51cec */
    //volatile U_ISP_VDDCIBINWGT26    ISP_VDDCIBINWGT26                 ; /* 0x51cf0 */
    //volatile U_ISP_VDDCIBINWGT27    ISP_VDDCIBINWGT27                 ; /* 0x51cf4 */
    volatile U_ISP_VDDCIADJLUT       ISP_VDDCIADJLUT[33]             ; /* 0x51cf8 ~ 0x51d78*/
    //volatile U_ISP_VDDCIADJLUT00    ISP_VDDCIADJLUT00                 ; /* 0x51cf8 */
    //volatile U_ISP_VDDCIADJLUT01    ISP_VDDCIADJLUT01                 ; /* 0x51cfc */
    //volatile U_ISP_VDDCIADJLUT02    ISP_VDDCIADJLUT02                 ; /* 0x51d00 */
    //volatile U_ISP_VDDCIADJLUT03    ISP_VDDCIADJLUT03                 ; /* 0x51d04 */
    //volatile U_ISP_VDDCIADJLUT04    ISP_VDDCIADJLUT04                 ; /* 0x51d08 */
    //volatile U_ISP_VDDCIADJLUT05    ISP_VDDCIADJLUT05                 ; /* 0x51d0c */
    //volatile U_ISP_VDDCIADJLUT06    ISP_VDDCIADJLUT06                 ; /* 0x51d10 */
    //volatile U_ISP_VDDCIADJLUT07    ISP_VDDCIADJLUT07                 ; /* 0x51d14 */
    //volatile U_ISP_VDDCIADJLUT08    ISP_VDDCIADJLUT08                 ; /* 0x51d18 */
    //volatile U_ISP_VDDCIADJLUT09    ISP_VDDCIADJLUT09                 ; /* 0x51d1c */
    //volatile U_ISP_VDDCIADJLUT0A    ISP_VDDCIADJLUT0A                 ; /* 0x51d20 */
    //volatile U_ISP_VDDCIADJLUT10    ISP_VDDCIADJLUT10                 ; /* 0x51d24 */
    //volatile U_ISP_VDDCIADJLUT11    ISP_VDDCIADJLUT11                 ; /* 0x51d28 */
    //volatile U_ISP_VDDCIADJLUT12    ISP_VDDCIADJLUT12                 ; /* 0x51d2c */
    //volatile U_ISP_VDDCIADJLUT13    ISP_VDDCIADJLUT13                 ; /* 0x51d30 */
    //volatile U_ISP_VDDCIADJLUT14    ISP_VDDCIADJLUT14                 ; /* 0x51d34 */
    //volatile U_ISP_VDDCIADJLUT15    ISP_VDDCIADJLUT15                 ; /* 0x51d38 */
    //volatile U_ISP_VDDCIADJLUT16    ISP_VDDCIADJLUT16                 ; /* 0x51d3c */
    //volatile U_ISP_VDDCIADJLUT17    ISP_VDDCIADJLUT17                 ; /* 0x51d40 */
    //volatile U_ISP_VDDCIADJLUT18    ISP_VDDCIADJLUT18                 ; /* 0x51d44 */
    //volatile U_ISP_VDDCIADJLUT19    ISP_VDDCIADJLUT19                 ; /* 0x51d48 */
    //volatile U_ISP_VDDCIADJLUT1A    ISP_VDDCIADJLUT1A                 ; /* 0x51d4c */
    //volatile U_ISP_VDDCIADJLUT20    ISP_VDDCIADJLUT20                 ; /* 0x51d50 */
    //volatile U_ISP_VDDCIADJLUT21    ISP_VDDCIADJLUT21                 ; /* 0x51d54 */
    //volatile U_ISP_VDDCIADJLUT22    ISP_VDDCIADJLUT22                 ; /* 0x51d58 */
    //volatile U_ISP_VDDCIADJLUT23    ISP_VDDCIADJLUT23                 ; /* 0x51d5c */
    //volatile U_ISP_VDDCIADJLUT24    ISP_VDDCIADJLUT24                 ; /* 0x51d60 */
    //volatile U_ISP_VDDCIADJLUT25    ISP_VDDCIADJLUT25                 ; /* 0x51d64 */
    //volatile U_ISP_VDDCIADJLUT26    ISP_VDDCIADJLUT26                 ; /* 0x51d68 */
    //volatile U_ISP_VDDCIADJLUT27    ISP_VDDCIADJLUT27                 ; /* 0x51d6c */
    //volatile U_ISP_VDDCIADJLUT28    ISP_VDDCIADJLUT28                 ; /* 0x51d70 */
    //volatile U_ISP_VDDCIADJLUT29    ISP_VDDCIADJLUT29                 ; /* 0x51d74 */
    //volatile U_ISP_VDDCIADJLUT2A    ISP_VDDCIADJLUT2A                 ; /* 0x51d78 */
    volatile U_ISP_VDDCIDIVLUT      ISP_VDDCIDIVLUT[32]            ; /* 0x51d7c ~ 0x51df8*/
    //volatile U_ISP_VDDCIDIVLUT0     ISP_VDDCIDIVLUT0                  ; /* 0x51d7c */
    //volatile U_ISP_VDDCIDIVLUT1     ISP_VDDCIDIVLUT1                  ; /* 0x51d80 */
    //volatile U_ISP_VDDCIDIVLUT2     ISP_VDDCIDIVLUT2                  ; /* 0x51d84 */
    //volatile U_ISP_VDDCIDIVLUT3     ISP_VDDCIDIVLUT3                  ; /* 0x51d88 */
    //volatile U_ISP_VDDCIDIVLUT4     ISP_VDDCIDIVLUT4                  ; /* 0x51d8c */
    //volatile U_ISP_VDDCIDIVLUT5     ISP_VDDCIDIVLUT5                  ; /* 0x51d90 */
    //volatile U_ISP_VDDCIDIVLUT6     ISP_VDDCIDIVLUT6                  ; /* 0x51d94 */
    //volatile U_ISP_VDDCIDIVLUT7     ISP_VDDCIDIVLUT7                  ; /* 0x51d98 */
    //volatile U_ISP_VDDCIDIVLUT8     ISP_VDDCIDIVLUT8                  ; /* 0x51d9c */
    //volatile U_ISP_VDDCIDIVLUT9     ISP_VDDCIDIVLUT9                  ; /* 0x51da0 */
    //volatile U_ISP_VDDCIDIVLUT10    ISP_VDDCIDIVLUT10                 ; /* 0x51da4 */
    //volatile U_ISP_VDDCIDIVLUT11    ISP_VDDCIDIVLUT11                 ; /* 0x51da8 */
    //volatile U_ISP_VDDCIDIVLUT12    ISP_VDDCIDIVLUT12                 ; /* 0x51dac */
    //volatile U_ISP_VDDCIDIVLUT13    ISP_VDDCIDIVLUT13                 ; /* 0x51db0 */
    //volatile U_ISP_VDDCIDIVLUT14    ISP_VDDCIDIVLUT14                 ; /* 0x51db4 */
    //volatile U_ISP_VDDCIDIVLUT15    ISP_VDDCIDIVLUT15                 ; /* 0x51db8 */
    //volatile U_ISP_VDDCIDIVLUT16    ISP_VDDCIDIVLUT16                 ; /* 0x51dbc */
    //volatile U_ISP_VDDCIDIVLUT17    ISP_VDDCIDIVLUT17                 ; /* 0x51dc0 */
    //volatile U_ISP_VDDCIDIVLUT18    ISP_VDDCIDIVLUT18                 ; /* 0x51dc4 */
    //volatile U_ISP_VDDCIDIVLUT19    ISP_VDDCIDIVLUT19                 ; /* 0x51dc8 */
    //volatile U_ISP_VDDCIDIVLUT20    ISP_VDDCIDIVLUT20                 ; /* 0x51dcc */
    //volatile U_ISP_VDDCIDIVLUT21    ISP_VDDCIDIVLUT21                 ; /* 0x51dd0 */
    //volatile U_ISP_VDDCIDIVLUT22    ISP_VDDCIDIVLUT22                 ; /* 0x51dd4 */
    //volatile U_ISP_VDDCIDIVLUT23    ISP_VDDCIDIVLUT23                 ; /* 0x51dd8 */
    //volatile U_ISP_VDDCIDIVLUT24    ISP_VDDCIDIVLUT24                 ; /* 0x51ddc */
    //volatile U_ISP_VDDCIDIVLUT25    ISP_VDDCIDIVLUT25                 ; /* 0x51de0 */
    //volatile U_ISP_VDDCIDIVLUT26    ISP_VDDCIDIVLUT26                 ; /* 0x51de4 */
    //volatile U_ISP_VDDCIDIVLUT27    ISP_VDDCIDIVLUT27                 ; /* 0x51de8 */
    //volatile U_ISP_VDDCIDIVLUT28    ISP_VDDCIDIVLUT28                 ; /* 0x51dec */
    //volatile U_ISP_VDDCIDIVLUT29    ISP_VDDCIDIVLUT29                 ; /* 0x51df0 */
    //volatile U_ISP_VDDCIDIVLUT30    ISP_VDDCIDIVLUT30                 ; /* 0x51df4 */
    //volatile U_ISP_VDDCIDIVLUT31    ISP_VDDCIDIVLUT31                 ; /* 0x51df8 */
} S_HIISP_REGS_TYPE;

/* Declare the struct pointor of the module VICAP */

#endif /* __ISP_REG_DEFINE_H__ */
