/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef __HIFB_REG_H__
#define __HIFB_REG_H__


#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */
/* Define the union U_VOCTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 25  ; /* [24..0]  */
        unsigned int    wbc_dhd_ck_gt_en      : 1   ; /* [25]  */
        unsigned int    g1_ck_gt_en           : 1   ; /* [26]  */
        unsigned int    g0_ck_gt_en           : 1   ; /* [27]  */
        unsigned int    v1_ck_gt_en           : 1   ; /* [28]  */
        unsigned int    v0_ck_gt_en           : 1   ; /* [29]  */
        unsigned int    chk_sum_en            : 1   ; /* [30]  */
        unsigned int    vo_ck_gt_en           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOCTRL;

/* Define the union U_VOINTSTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dhd0vtthd1_int        : 1   ; /* [0]  */
        unsigned int    dhd0vtthd2_int        : 1   ; /* [1]  */
        unsigned int    dhd0vtthd3_int        : 1   ; /* [2]  */
        unsigned int    dhd0uf_int            : 1   ; /* [3]  */
        unsigned int    dhd1vtthd1_int        : 1   ; /* [4]  */
        unsigned int    dhd1vtthd2_int        : 1   ; /* [5]  */
        unsigned int    dhd1vtthd3_int        : 1   ; /* [6]  */
        unsigned int    dhd1uf_int            : 1   ; /* [7]  */
        unsigned int    dsdvtthd1_int         : 1   ; /* [8]  */
        unsigned int    dsdvtthd2_int         : 1   ; /* [9]  */
        unsigned int    dsdvtthd3_int         : 1   ; /* [10]  */
        unsigned int    dsduf_int             : 1   ; /* [11]  */
        unsigned int    b0_err_int            : 1   ; /* [12]  */
        unsigned int    b1_err_int            : 1   ; /* [13]  */
        unsigned int    b2_err_int            : 1   ; /* [14]  */
        unsigned int    wbc_dhd_over_int      : 1   ; /* [15]  */
        unsigned int    vdac0_int             : 1   ; /* [16]  */
        unsigned int    vdac1_int             : 1   ; /* [17]  */
        unsigned int    vdac2_int             : 1   ; /* [18]  */
        unsigned int    vdac3_int             : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOINTSTA;

/* Define the union U_VOMSKINTSTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dhd0vtthd1_clr        : 1   ; /* [0]  */
        unsigned int    dhd0vtthd2_clr        : 1   ; /* [1]  */
        unsigned int    dhd0vtthd3_clr        : 1   ; /* [2]  */
        unsigned int    dhd0uf_clr            : 1   ; /* [3]  */
        unsigned int    dhd1vtthd1_clr        : 1   ; /* [4]  */
        unsigned int    dhd1vtthd2_clr        : 1   ; /* [5]  */
        unsigned int    dhd1vtthd3_clr        : 1   ; /* [6]  */
        unsigned int    dhd1uf_clr            : 1   ; /* [7]  */
        unsigned int    dsdvtthd1_clr         : 1   ; /* [8]  */
        unsigned int    dsdvtthd2_clr         : 1   ; /* [9]  */
        unsigned int    dsdvtthd3_clr         : 1   ; /* [10]  */
        unsigned int    dsduf_clr             : 1   ; /* [11]  */
        unsigned int    b0_err_clr            : 1   ; /* [12]  */
        unsigned int    b1_err_clr            : 1   ; /* [13]  */
        unsigned int    b2_err_clr            : 1   ; /* [14]  */
        unsigned int    wbc_dhd_over_clr      : 1   ; /* [15]  */
        unsigned int    vdac0_clr             : 1   ; /* [16]  */
        unsigned int    vdac1_clr             : 1   ; /* [17]  */
        unsigned int    vdac2_clr             : 1   ; /* [18]  */
        unsigned int    vdac3_clr             : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOMSKINTSTA;

/* Define the union U_VOINTMSK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dhd0vtthd1_intmask    : 1   ; /* [0]  */
        unsigned int    dhd0vtthd2_intmask    : 1   ; /* [1]  */
        unsigned int    dhd0vtthd3_intmask    : 1   ; /* [2]  */
        unsigned int    dhd0uf_intmask        : 1   ; /* [3]  */
        unsigned int    dhd1vtthd1_intmask    : 1   ; /* [4]  */
        unsigned int    dhd1vtthd2_intmask    : 1   ; /* [5]  */
        unsigned int    dhd1vtthd3_intmask    : 1   ; /* [6]  */
        unsigned int    dhd1uf_intmask        : 1   ; /* [7]  */
        unsigned int    dsdvtthd1_intmask     : 1   ; /* [8]  */
        unsigned int    dsdvtthd2_intmask     : 1   ; /* [9]  */
        unsigned int    dsdvtthd3_intmask     : 1   ; /* [10]  */
        unsigned int    dsduf_intmask         : 1   ; /* [11]  */
        unsigned int    b0_err_intmask        : 1   ; /* [12]  */
        unsigned int    b1_err_intmask        : 1   ; /* [13]  */
        unsigned int    b2_err_intmask        : 1   ; /* [14]  */
        unsigned int    wbc_dhd_over_intmask  : 1   ; /* [15]  */
        unsigned int    vdac0_intmask         : 1   ; /* [16]  */
        unsigned int    vdac1_intmask         : 1   ; /* [17]  */
        unsigned int    vdac2_intmask         : 1   ; /* [18]  */
        unsigned int    vdac3_intmask         : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOINTMSK;

/* Define the union U_VOINTSTA1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dhd0vtthd1_int        : 1   ; /* [0]  */
        unsigned int    dhd0vtthd2_int        : 1   ; /* [1]  */
        unsigned int    dhd0vtthd3_int        : 1   ; /* [2]  */
        unsigned int    dhd0uf_int            : 1   ; /* [3]  */
        unsigned int    dhd1vtthd1_int        : 1   ; /* [4]  */
        unsigned int    dhd1vtthd2_int        : 1   ; /* [5]  */
        unsigned int    dhd1vtthd3_int        : 1   ; /* [6]  */
        unsigned int    dhd1uf_int            : 1   ; /* [7]  */
        unsigned int    dsdvtthd1_int         : 1   ; /* [8]  */
        unsigned int    dsdvtthd2_int         : 1   ; /* [9]  */
        unsigned int    dsdvtthd3_int         : 1   ; /* [10]  */
        unsigned int    dsduf_int             : 1   ; /* [11]  */
        unsigned int    b0_err_int            : 1   ; /* [12]  */
        unsigned int    b1_err_int            : 1   ; /* [13]  */
        unsigned int    b2_err_int            : 1   ; /* [14]  */
        unsigned int    wbc_dhd_over_int      : 1   ; /* [15]  */
        unsigned int    vdac0_int             : 1   ; /* [16]  */
        unsigned int    vdac1_int             : 1   ; /* [17]  */
        unsigned int    vdac2_int             : 1   ; /* [18]  */
        unsigned int    vdac3_int             : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOINTSTA1;

/* Define the union U_VOMSKINTSTA1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dhd0vtthd1_clr        : 1   ; /* [0]  */
        unsigned int    dhd0vtthd2_clr        : 1   ; /* [1]  */
        unsigned int    dhd0vtthd3_clr        : 1   ; /* [2]  */
        unsigned int    dhd0uf_clr            : 1   ; /* [3]  */
        unsigned int    dhd1vtthd1_clr        : 1   ; /* [4]  */
        unsigned int    dhd1vtthd2_clr        : 1   ; /* [5]  */
        unsigned int    dhd1vtthd3_clr        : 1   ; /* [6]  */
        unsigned int    dhd1uf_clr            : 1   ; /* [7]  */
        unsigned int    dsdvtthd1_clr         : 1   ; /* [8]  */
        unsigned int    dsdvtthd2_clr         : 1   ; /* [9]  */
        unsigned int    dsdvtthd3_clr         : 1   ; /* [10]  */
        unsigned int    dsduf_clr             : 1   ; /* [11]  */
        unsigned int    b0_err_clr            : 1   ; /* [12]  */
        unsigned int    b1_err_clr            : 1   ; /* [13]  */
        unsigned int    b2_err_clr            : 1   ; /* [14]  */
        unsigned int    wbc_dhd_over_clr      : 1   ; /* [15]  */
        unsigned int    vdac0_clr             : 1   ; /* [16]  */
        unsigned int    vdac1_clr             : 1   ; /* [17]  */
        unsigned int    vdac2_clr             : 1   ; /* [18]  */
        unsigned int    vdac3_clr             : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOMSKINTSTA1;

/* Define the union U_VOINTMSK1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dhd0vtthd1_intmask    : 1   ; /* [0]  */
        unsigned int    dhd0vtthd2_intmask    : 1   ; /* [1]  */
        unsigned int    dhd0vtthd3_intmask    : 1   ; /* [2]  */
        unsigned int    dhd0uf_intmask        : 1   ; /* [3]  */
        unsigned int    dhd1vtthd1_intmask    : 1   ; /* [4]  */
        unsigned int    dhd1vtthd2_intmask    : 1   ; /* [5]  */
        unsigned int    dhd1vtthd3_intmask    : 1   ; /* [6]  */
        unsigned int    dhd1uf_intmask        : 1   ; /* [7]  */
        unsigned int    dsdvtthd1_intmask     : 1   ; /* [8]  */
        unsigned int    dsdvtthd2_intmask     : 1   ; /* [9]  */
        unsigned int    dsdvtthd3_intmask     : 1   ; /* [10]  */
        unsigned int    dsduf_intmask         : 1   ; /* [11]  */
        unsigned int    b0_err_intmask        : 1   ; /* [12]  */
        unsigned int    b1_err_intmask        : 1   ; /* [13]  */
        unsigned int    b2_err_intmask        : 1   ; /* [14]  */
        unsigned int    wbc_dhd_over_intmask  : 1   ; /* [15]  */
        unsigned int    vdac0_intmask         : 1   ; /* [16]  */
        unsigned int    vdac1_intmask         : 1   ; /* [17]  */
        unsigned int    vdac2_intmask         : 1   ; /* [18]  */
        unsigned int    vdac3_intmask         : 1   ; /* [19]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VOINTMSK1;

/* Define the union U_MIXG0_BKG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mixer_bkgcr           : 10  ; /* [9..0]  */
        unsigned int    mixer_bkgcb           : 10  ; /* [19..10]  */
        unsigned int    mixer_bkgy            : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MIXG0_BKG;

/* Define the union U_LINK_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    v1_link               : 1   ; /* [0]  */
        unsigned int    cbm1_sel              : 1   ; /* [1]  */
        unsigned int    hc_link               : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LINK_CTRL;

/* Define the union U_PARA_HADDR_VHD_CHN06 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int para_haddr_vhd_chn06   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_PARA_HADDR_VHD_CHN06;
/* Define the union U_PARA_ADDR_VHD_CHN06 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int para_addr_vhd_chn06    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_PARA_ADDR_VHD_CHN06;
/* Define the union U_PARA_UP_VHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    para_up_vhd_chn00     : 1   ; /* [0]  */
        unsigned int    para_up_vhd_chn01     : 1   ; /* [1]  */
        unsigned int    para_up_vhd_chn02     : 1   ; /* [2]  */
        unsigned int    para_up_vhd_chn03     : 1   ; /* [3]  */
        unsigned int    para_up_vhd_chn04     : 1   ; /* [4]  */
        unsigned int    para_up_vhd_chn05     : 1   ; /* [5]  */
        unsigned int    para_up_vhd_chn06     : 1   ; /* [6]  */
        unsigned int    para_up_vhd_chn07     : 1   ; /* [7]  */
        unsigned int    para_up_vhd_chn08     : 1   ; /* [8]  */
        unsigned int    para_up_vhd_chn09     : 1   ; /* [9]  */
        unsigned int    para_up_vhd_chn10     : 1   ; /* [10]  */
        unsigned int    para_up_vhd_chn11     : 1   ; /* [11]  */
        unsigned int    para_up_vhd_chn12     : 1   ; /* [12]  */
        unsigned int    para_up_vhd_chn13     : 1   ; /* [13]  */
        unsigned int    para_up_vhd_chn14     : 1   ; /* [14]  */
        unsigned int    para_up_vhd_chn15     : 1   ; /* [15]  */
        unsigned int    para_up_vhd_chn16     : 1   ; /* [16]  */
        unsigned int    para_up_vhd_chn17     : 1   ; /* [17]  */
        unsigned int    para_up_vhd_chn18     : 1   ; /* [18]  */
        unsigned int    para_up_vhd_chn19     : 1   ; /* [19]  */
        unsigned int    para_up_vhd_chn20     : 1   ; /* [20]  */
        unsigned int    para_up_vhd_chn21     : 1   ; /* [21]  */
        unsigned int    para_up_vhd_chn22     : 1   ; /* [22]  */
        unsigned int    para_up_vhd_chn23     : 1   ; /* [23]  */
        unsigned int    para_up_vhd_chn24     : 1   ; /* [24]  */
        unsigned int    para_up_vhd_chn25     : 1   ; /* [25]  */
        unsigned int    para_up_vhd_chn26     : 1   ; /* [26]  */
        unsigned int    para_up_vhd_chn27     : 1   ; /* [27]  */
        unsigned int    para_up_vhd_chn28     : 1   ; /* [28]  */
        unsigned int    para_up_vhd_chn29     : 1   ; /* [29]  */
        unsigned int    para_up_vhd_chn30     : 1   ; /* [30]  */
        unsigned int    para_up_vhd_chn31     : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PARA_UP_VHD;

/* Define the union U_V0_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    galpha                : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 20  ; /* [27..8]  */
        unsigned int    rupd_field            : 1   ; /* [28]  */
        unsigned int    rgup_mode             : 1   ; /* [29]  */
        unsigned int    nosec_flag            : 1   ; /* [30]  */
        unsigned int    surface_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_V0_CTRL;

/* Define the union U_V0_UPD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regup                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_V0_UPD;

/* Define the union U_V1_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    galpha                : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 20  ; /* [27..8]  */
        unsigned int    rupd_field            : 1   ; /* [28]  */
        unsigned int    rgup_mode             : 1   ; /* [29]  */
        unsigned int    nosec_flag            : 1   ; /* [30]  */
        unsigned int    surface_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_V1_CTRL;

/* Define the union U_V1_UPD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regup                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_V1_UPD;

/* Define the union U_V2_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    galpha                : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 20  ; /* [27..8]  */
        unsigned int    rupd_field            : 1   ; /* [28]  */
        unsigned int    rgup_mode             : 1   ; /* [29]  */
        unsigned int    nosec_flag            : 1   ; /* [30]  */
        unsigned int    surface_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_V2_CTRL;

/* Define the union U_V2_UPD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regup                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_V2_UPD;

/* Define the union U_G0_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    galpha                : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 19  ; /* [26..8]  */
        unsigned int    g0_depremult          : 1   ; /* [27]  */
        unsigned int    rupd_field            : 1   ; /* [28]  */
        unsigned int    rgup_mode             : 1   ; /* [29]  */
        unsigned int    nosec_flag            : 1   ; /* [30]  */
        unsigned int    surface_en            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_CTRL;

/* Define the union U_G0_UPD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regup                 : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_UPD;

/* Define the union U_G0_GALPHA_SUM */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int fk_alpha_sum           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_G0_GALPHA_SUM;
/* Define the union U_G0_0RESO_READ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ow                    : 16  ; /* [15..0]  */
        unsigned int    oh                    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_0RESO_READ;

/* Define the union U_G0_DFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    disp_xfpos            : 16  ; /* [15..0]  */
        unsigned int    disp_yfpos            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_DFPOS;

/* Define the union U_G0_DLPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    disp_xlpos            : 16  ; /* [15..0]  */
        unsigned int    disp_ylpos            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_DLPOS;

/* Define the union U_G0_VFPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_xfpos           : 16  ; /* [15..0]  */
        unsigned int    video_yfpos           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_VFPOS;

/* Define the union U_G0_VLPOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    video_xlpos           : 16  ; /* [15..0]  */
        unsigned int    video_ylpos           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_VLPOS;

/* Define the union U_G0_HIPP_CSC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_en           : 1   ; /* [0]  */
        unsigned int    hipp_csc_demo_en      : 1   ; /* [1]  */
        unsigned int    hipp_csc_ck_gt_en     : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_CTRL;

/* Define the union U_G0_HIPP_CSC_COEF00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef00       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF00;

/* Define the union U_G0_HIPP_CSC_COEF01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef01       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF01;

/* Define the union U_G0_HIPP_CSC_COEF02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef02       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF02;

/* Define the union U_G0_HIPP_CSC_COEF10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef10       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF10;

/* Define the union U_G0_HIPP_CSC_COEF11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef11       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF11;

/* Define the union U_G0_HIPP_CSC_COEF12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef12       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF12;

/* Define the union U_G0_HIPP_CSC_COEF20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef20       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF20;

/* Define the union U_G0_HIPP_CSC_COEF21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef21       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF21;

/* Define the union U_G0_HIPP_CSC_COEF22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_coef22       : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_COEF22;

/* Define the union U_G0_HIPP_CSC_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_scale        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_SCALE;

/* Define the union U_G0_HIPP_CSC_IDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_idc0         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_IDC0;

/* Define the union U_G0_HIPP_CSC_IDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_idc1         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_IDC1;

/* Define the union U_G0_HIPP_CSC_IDC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_idc2         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_IDC2;

/* Define the union U_G0_HIPP_CSC_ODC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_odc0         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_ODC0;

/* Define the union U_G0_HIPP_CSC_ODC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_odc1         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_ODC1;

/* Define the union U_G0_HIPP_CSC_ODC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_odc2         : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_ODC2;

/* Define the union U_G0_HIPP_CSC_MIN_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_min_y        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_MIN_Y;

/* Define the union U_G0_HIPP_CSC_MIN_C */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_min_c        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_MIN_C;

/* Define the union U_G0_HIPP_CSC_MAX_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_max_y        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_MAX_Y;

/* Define the union U_G0_HIPP_CSC_MAX_C */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc_max_c        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_MAX_C;

/* Define the union U_G0_HIPP_CSC2_COEF00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef00      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF00;

/* Define the union U_G0_HIPP_CSC2_COEF01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef01      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF01;

/* Define the union U_G0_HIPP_CSC2_COEF02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef02      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF02;

/* Define the union U_G0_HIPP_CSC2_COEF10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef10      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF10;

/* Define the union U_G0_HIPP_CSC2_COEF11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef11      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF11;

/* Define the union U_G0_HIPP_CSC2_COEF12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef12      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF12;

/* Define the union U_G0_HIPP_CSC2_COEF20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef20      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF20;

/* Define the union U_G0_HIPP_CSC2_COEF21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef21      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF21;

/* Define the union U_G0_HIPP_CSC2_COEF22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_coef22      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_COEF22;

/* Define the union U_G0_HIPP_CSC2_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_scale       : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_SCALE;

/* Define the union U_G0_HIPP_CSC2_IDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_idc0        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_IDC0;

/* Define the union U_G0_HIPP_CSC2_IDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_idc1        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_IDC1;

/* Define the union U_G0_HIPP_CSC2_IDC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_idc2        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_IDC2;

/* Define the union U_G0_HIPP_CSC2_ODC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_odc0        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_ODC0;

/* Define the union U_G0_HIPP_CSC2_ODC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_odc1        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_ODC1;

/* Define the union U_G0_HIPP_CSC2_ODC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_odc2        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_ODC2;

/* Define the union U_G0_HIPP_CSC2_MIN_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_min_y       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_MIN_Y;

/* Define the union U_G0_HIPP_CSC2_MIN_C */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_min_c       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_MIN_C;

/* Define the union U_G0_HIPP_CSC2_MAX_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_max_y       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_MAX_Y;

/* Define the union U_G0_HIPP_CSC2_MAX_C */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hipp_csc2_max_c       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC2_MAX_C;

/* Define the union U_G0_HIPP_CSC_INK_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ink_en                : 1   ; /* [0]  */
        unsigned int    ink_sel               : 1   ; /* [1]  */
        unsigned int    data_fmt              : 1   ; /* [2]  */
        unsigned int    cross_enable          : 1   ; /* [3]  */
        unsigned int    color_mode            : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_INK_CTRL;

/* Define the union U_G0_HIPP_CSC_INK_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    x_pos                 : 16  ; /* [15..0]  */
        unsigned int    y_pos                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_HIPP_CSC_INK_POS;

/* Define the union U_G0_HIPP_CSC_INK_DATA */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int ink_data               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_G0_HIPP_CSC_INK_DATA;
/* Define the union U_G0_HIPP_CSC_INK_DATA2 */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int ink_data2              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_G0_HIPP_CSC_INK_DATA2;
/* Define the union U_G0_DOF_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 31  ; /* [30..0]  */
        unsigned int    dof_en                : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_DOF_CTRL;

/* Define the union U_G0_DOF_STEP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    left_step             : 8   ; /* [7..0]  */
        unsigned int    right_step            : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_DOF_STEP;

/* Define the union U_G0_DOF_BKG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dof_bk_cr             : 10  ; /* [9..0]  */
        unsigned int    dof_bk_cb             : 10  ; /* [19..10]  */
        unsigned int    dof_bk_y              : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_DOF_BKG;

/* Define the union U_G0_DOF_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dof_bk_alpha          : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_DOF_ALPHA;

/* Define the union U_G0_ZME_HINFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    out_width             : 16  ; /* [15..0]  */
        unsigned int    ck_gt_en              : 1   ; /* [16]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_HINFO;

/* Define the union U_G0_ZME_HSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hratio                : 24  ; /* [23..0]  */
        unsigned int    hfir_order            : 1   ; /* [24]  */
        unsigned int    ahfir_mode            : 1   ; /* [25]  */
        unsigned int    lhfir_mode            : 1   ; /* [26]  */
        unsigned int    reserved_0            : 1   ; /* [27]  */
        unsigned int    chfir_mid_en          : 1   ; /* [28]  */
        unsigned int    lhfir_mid_en          : 1   ; /* [29]  */
        unsigned int    ahfir_mid_en          : 1   ; /* [30]  */
        unsigned int    hfir_en               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_HSP;

/* Define the union U_G0_ZME_HLOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lhfir_offset          : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_HLOFFSET;

/* Define the union U_G0_ZME_HCOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chfir_offset          : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_HCOFFSET;

/* Define the union U_G0_ZME_COEF_REN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    apb_g0_vf_lren        : 1   ; /* [1]  */
        unsigned int    reserved_1            : 1   ; /* [2]  */
        unsigned int    apb_g0_hf_lren        : 1   ; /* [3]  */
        unsigned int    reserved_2            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_COEF_REN;

/* Define the union U_G0_ZME_COEF_RDATA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    apb_vhd_coef_raddr    : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_COEF_RDATA;

/* Define the union U_G0_ZME_VINFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    out_height            : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 2   ; /* [17..16]  */
        unsigned int    out_pro               : 1   ; /* [18]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_VINFO;

/* Define the union U_G0_ZME_VSP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vratio                : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 9   ; /* [24..16]  */
        unsigned int    vafir_mode            : 1   ; /* [25]  */
        unsigned int    lvfir_mode            : 1   ; /* [26]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    cvfir_mid_en          : 1   ; /* [28]  */
        unsigned int    lvfir_mid_en          : 1   ; /* [29]  */
        unsigned int    avfir_mid_en          : 1   ; /* [30]  */
        unsigned int    vfir_en               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_VSP;

/* Define the union U_G0_ZME_VOFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vbtm_offset           : 16  ; /* [15..0]  */
        unsigned int    vtp_offset            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_G0_ZME_VOFFSET;

/* Define the union U_DHD0_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    regup                 : 1   ; /* [0]  */
        unsigned int    disp_mode             : 3   ; /* [3..1]  */
        unsigned int    iop                   : 1   ; /* [4]  */
        unsigned int    intf_ivs              : 1   ; /* [5]  */
        unsigned int    intf_ihs              : 1   ; /* [6]  */
        unsigned int    intf_idv              : 1   ; /* [7]  */
        unsigned int    reserved_0            : 1   ; /* [8]  */
        unsigned int    hdmi420c_sel          : 1   ; /* [9]  */
        unsigned int    hdmi420_en            : 1   ; /* [10]  */
        unsigned int    uf_offline_en         : 1   ; /* [11]  */
        unsigned int    reserved_1            : 2   ; /* [13..12]  */
        unsigned int    hdmi_mode             : 1   ; /* [14]  */
        unsigned int    twochn_debug          : 1   ; /* [15]  */
        unsigned int    twochn_en             : 1   ; /* [16]  */
        unsigned int    reserved_2            : 1   ; /* [17]  */
        unsigned int    cbar_mode             : 1   ; /* [18]  */
        unsigned int    sin_en                : 1   ; /* [19]  */
        unsigned int    fpga_lmt_width        : 7   ; /* [26..20]  */
        unsigned int    fpga_lmt_en           : 1   ; /* [27]  */
        unsigned int    p2i_en                : 1   ; /* [28]  */
        unsigned int    cbar_sel              : 1   ; /* [29]  */
        unsigned int    cbar_en               : 1   ; /* [30]  */
        unsigned int    intf_en               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_CTRL;

/* Define the union U_DHD0_VSYNC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vact                  : 16  ; /* [15..0]  */
        unsigned int    vbb                   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_VSYNC1;

/* Define the union U_DHD0_VSYNC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vfb                   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_VSYNC2;

/* Define the union U_DHD0_HSYNC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hact                  : 16  ; /* [15..0]  */
        unsigned int    hbb                   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_HSYNC1;

/* Define the union U_DHD0_HSYNC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hfb                   : 16  ; /* [15..0]  */
        unsigned int    hmid                  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_HSYNC2;

/* Define the union U_DHD0_VPLUS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bvact                 : 16  ; /* [15..0]  */
        unsigned int    bvbb                  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_VPLUS1;

/* Define the union U_DHD0_VPLUS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bvfb                  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_VPLUS2;

/* Define the union U_DHD0_PWR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hpw                   : 16  ; /* [15..0]  */
        unsigned int    vpw                   : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 3   ; /* [26..24]  */
        unsigned int    multichn_en           : 2   ; /* [28..27]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_PWR;

/* Define the union U_DHD0_VTTHD3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vtmgthd3              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 2   ; /* [14..13]  */
        unsigned int    thd3_mode             : 1   ; /* [15]  */
        unsigned int    vtmgthd4              : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 2   ; /* [30..29]  */
        unsigned int    thd4_mode             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_VTTHD3;

/* Define the union U_DHD0_VTTHD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vtmgthd1              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 2   ; /* [14..13]  */
        unsigned int    thd1_mode             : 1   ; /* [15]  */
        unsigned int    vtmgthd2              : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 2   ; /* [30..29]  */
        unsigned int    thd2_mode             : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_VTTHD;

/* Define the union U_DHD0_PARAUP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 31  ; /* [30..0]  */
        unsigned int    paraup_mode           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_PARAUP;

/* Define the union U_DHD0_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vback_blank           : 1   ; /* [0]  */
        unsigned int    vblank                : 1   ; /* [1]  */
        unsigned int    bottom_field          : 1   ; /* [2]  */
        unsigned int    vcnt                  : 13  ; /* [15..3]  */
        unsigned int    count_int             : 8   ; /* [23..16]  */
        unsigned int    dhd_even              : 1   ; /* [24]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DHD0_STATE;

/* Define the union U_VO_MUX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mipi_sel              : 4   ; /* [3..0]  */
        unsigned int    lcd_sel               : 4   ; /* [7..4]  */
        unsigned int    bt_sel                : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 16  ; /* [27..12]  */
        unsigned int    digital_sel           : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VO_MUX;

/* Define the union U_GFX_READ_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    read_mode             : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    draw_mode             : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    flip_en               : 1   ; /* [8]  */
        unsigned int    reserved_2            : 1   ; /* [9]  */
        unsigned int    mute_en               : 1   ; /* [10]  */
        unsigned int    mute_req_en           : 1   ; /* [11]  */
        unsigned int    fdr_ck_gt_en          : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_READ_CTRL;

/* Define the union U_GFX_MAC_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    req_ctrl              : 2   ; /* [1..0]  */
        unsigned int    req_len               : 2   ; /* [3..2]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    ofl_master            : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_MAC_CTRL;

/* Define the union U_GFX_OUT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    palpha_range          : 1   ; /* [0]  */
        unsigned int    palpha_en             : 1   ; /* [1]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    key_mode              : 1   ; /* [4]  */
        unsigned int    key_en                : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    bitext                : 2   ; /* [9..8]  */
        unsigned int    premulti_en           : 1   ; /* [10]  */
        unsigned int    testpattern_en        : 1   ; /* [11]  */
        unsigned int    reserved_2            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_OUT_CTRL;

/* Define the union U_GFX_1555_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    alpha_0               : 8   ; /* [7..0]  */
        unsigned int    alpha_1               : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_1555_ALPHA;

/* Define the union U_GFX_SRC_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ifmt                  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    disp_mode             : 4   ; /* [19..16]  */
        unsigned int    dcmp_en               : 1   ; /* [20]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_SRC_INFO;

/* Define the union U_GFX_SRC_RESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    src_w                 : 16  ; /* [15..0]  */
        unsigned int    src_h                 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_SRC_RESO;

/* Define the union U_GFX_SRC_CROP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    src_crop_x            : 16  ; /* [15..0]  */
        unsigned int    src_crop_y            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_SRC_CROP;

/* Define the union U_GFX_IRESO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ireso_w               : 16  ; /* [15..0]  */
        unsigned int    ireso_h               : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_IRESO;

/* Define the union U_GFX_ADDR_H */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gfx_addr_h             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_GFX_ADDR_H;
/* Define the union U_GFX_ADDR_L */
typedef union
{
    /* Define the struct bits  */
    struct
    {
        unsigned int gfx_addr_l             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
        unsigned int    u32;

} U_GFX_ADDR_L;
/* Define the union U_GFX_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    surface_stride        : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_STRIDE;

/* Define the union U_GFX_CKEY_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    key_b_max             : 8   ; /* [7..0]  */
        unsigned int    key_g_max             : 8   ; /* [15..8]  */
        unsigned int    key_r_max             : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_CKEY_MAX;

/* Define the union U_GFX_CKEY_MIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    key_b_min             : 8   ; /* [7..0]  */
        unsigned int    key_g_min             : 8   ; /* [15..8]  */
        unsigned int    key_r_min             : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_CKEY_MIN;

/* Define the union U_GFX_CKEY_MASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    key_b_msk             : 8   ; /* [7..0]  */
        unsigned int    key_g_msk             : 8   ; /* [15..8]  */
        unsigned int    key_r_msk             : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_GFX_CKEY_MASK;

//==============================================================================
/* Define the global struct */
typedef struct
{
    U_VOCTRL                        VOCTRL                           ; /* 0x0 */
    U_VOINTSTA                      VOINTSTA                         ; /* 0x4 */
    U_VOMSKINTSTA                   VOMSKINTSTA                      ; /* 0x8 */
    U_VOINTMSK                      VOINTMSK                         ; /* 0xc */
    unsigned int                    reserved_0                       ; /* 0x10 */
    U_VOINTSTA1                     VOINTSTA1                        ; /* 0x14 */
    U_VOMSKINTSTA1                  VOMSKINTSTA1                     ; /* 0x18 */
    U_VOINTMSK1                     VOINTMSK1                        ; /* 0x1c */
    unsigned int                    reserved_1[568]                  ; /* 0x20~0x8fc */
    U_MIXG0_BKG                     MIXG0_BKG                        ; /* 0x900 */
    unsigned int                    reserved_2[191]                  ; /* 0x904~0xbfc */
    U_LINK_CTRL                     LINK_CTRL                        ; /* 0xc00 */
    unsigned int                    reserved_3[139]                  ; /* 0xc04~0xe2c */
    U_PARA_HADDR_VHD_CHN06          PARA_HADDR_VHD_CHN06             ; /* 0xe30 */
    U_PARA_ADDR_VHD_CHN06           PARA_ADDR_VHD_CHN06              ; /* 0xe34 */
    unsigned int                    reserved_4[50]                   ; /* 0xe38~0xefc */
    U_PARA_UP_VHD                   PARA_UP_VHD                      ; /* 0xf00 */
    unsigned int                    reserved_5[63]                   ; /* 0xf04~0xffc */
    U_V0_CTRL                       V0_CTRL                          ; /* 0x1000 */
    U_V0_UPD                        V0_UPD                           ; /* 0x1004 */
    unsigned int                    reserved_6[1022]                 ; /* 0x1008~0x1ffc */
    U_V1_CTRL                       V1_CTRL                          ; /* 0x2000 */
    U_V1_UPD                        V1_UPD                           ; /* 0x2004 */
    unsigned int                    reserved_7[1022]                 ; /* 0x2008~0x2ffc */
    U_V2_CTRL                       V2_CTRL                          ; /* 0x3000 */
    U_V2_UPD                        V2_UPD                           ; /* 0x3004 */
    unsigned int                    reserved_8[4094]                 ; /* 0x3008~0x6ffc */
    U_G0_CTRL                       G0_CTRL                          ; /* 0x7000 */
    U_G0_UPD                        G0_UPD                           ; /* 0x7004 */
    U_G0_GALPHA_SUM                 G0_GALPHA_SUM                    ; /* 0x7008 */
    U_G0_0RESO_READ                 G0_0RESO_READ                    ; /* 0x700c */
    unsigned int                    reserved_9[28]                   ; /* 0x7010~0x707c */
    U_G0_DFPOS                      G0_DFPOS                         ; /* 0x7080 */
    U_G0_DLPOS                      G0_DLPOS                         ; /* 0x7084 */
    U_G0_VFPOS                      G0_VFPOS                         ; /* 0x7088 */
    U_G0_VLPOS                      G0_VLPOS                         ; /* 0x708c */
    unsigned int                    reserved_10[28]                  ; /* 0x7090~0x70fc */
    U_G0_HIPP_CSC_CTRL              G0_HIPP_CSC_CTRL                 ; /* 0x7100 */
    U_G0_HIPP_CSC_COEF00            G0_HIPP_CSC_COEF00               ; /* 0x7104 */
    U_G0_HIPP_CSC_COEF01            G0_HIPP_CSC_COEF01               ; /* 0x7108 */
    U_G0_HIPP_CSC_COEF02            G0_HIPP_CSC_COEF02               ; /* 0x710c */
    U_G0_HIPP_CSC_COEF10            G0_HIPP_CSC_COEF10               ; /* 0x7110 */
    U_G0_HIPP_CSC_COEF11            G0_HIPP_CSC_COEF11               ; /* 0x7114 */
    U_G0_HIPP_CSC_COEF12            G0_HIPP_CSC_COEF12               ; /* 0x7118 */
    U_G0_HIPP_CSC_COEF20            G0_HIPP_CSC_COEF20               ; /* 0x711c */
    U_G0_HIPP_CSC_COEF21            G0_HIPP_CSC_COEF21               ; /* 0x7120 */
    U_G0_HIPP_CSC_COEF22            G0_HIPP_CSC_COEF22               ; /* 0x7124 */
    U_G0_HIPP_CSC_SCALE             G0_HIPP_CSC_SCALE                ; /* 0x7128 */
    U_G0_HIPP_CSC_IDC0              G0_HIPP_CSC_IDC0                 ; /* 0x712c */
    U_G0_HIPP_CSC_IDC1              G0_HIPP_CSC_IDC1                 ; /* 0x7130 */
    U_G0_HIPP_CSC_IDC2              G0_HIPP_CSC_IDC2                 ; /* 0x7134 */
    U_G0_HIPP_CSC_ODC0              G0_HIPP_CSC_ODC0                 ; /* 0x7138 */
    U_G0_HIPP_CSC_ODC1              G0_HIPP_CSC_ODC1                 ; /* 0x713c */
    U_G0_HIPP_CSC_ODC2              G0_HIPP_CSC_ODC2                 ; /* 0x7140 */
    U_G0_HIPP_CSC_MIN_Y             G0_HIPP_CSC_MIN_Y                ; /* 0x7144 */
    U_G0_HIPP_CSC_MIN_C             G0_HIPP_CSC_MIN_C                ; /* 0x7148 */
    U_G0_HIPP_CSC_MAX_Y             G0_HIPP_CSC_MAX_Y                ; /* 0x714c */
    U_G0_HIPP_CSC_MAX_C             G0_HIPP_CSC_MAX_C                ; /* 0x7150 */
    U_G0_HIPP_CSC2_COEF00           G0_HIPP_CSC2_COEF00              ; /* 0x7154 */
    U_G0_HIPP_CSC2_COEF01           G0_HIPP_CSC2_COEF01              ; /* 0x7158 */
    U_G0_HIPP_CSC2_COEF02           G0_HIPP_CSC2_COEF02              ; /* 0x715c */
    U_G0_HIPP_CSC2_COEF10           G0_HIPP_CSC2_COEF10              ; /* 0x7160 */
    U_G0_HIPP_CSC2_COEF11           G0_HIPP_CSC2_COEF11              ; /* 0x7164 */
    U_G0_HIPP_CSC2_COEF12           G0_HIPP_CSC2_COEF12              ; /* 0x7168 */
    U_G0_HIPP_CSC2_COEF20           G0_HIPP_CSC2_COEF20              ; /* 0x716c */
    U_G0_HIPP_CSC2_COEF21           G0_HIPP_CSC2_COEF21              ; /* 0x7170 */
    U_G0_HIPP_CSC2_COEF22           G0_HIPP_CSC2_COEF22              ; /* 0x7174 */
    U_G0_HIPP_CSC2_SCALE            G0_HIPP_CSC2_SCALE               ; /* 0x7178 */
    U_G0_HIPP_CSC2_IDC0             G0_HIPP_CSC2_IDC0                ; /* 0x717c */
    U_G0_HIPP_CSC2_IDC1             G0_HIPP_CSC2_IDC1                ; /* 0x7180 */
    U_G0_HIPP_CSC2_IDC2             G0_HIPP_CSC2_IDC2                ; /* 0x7184 */
    U_G0_HIPP_CSC2_ODC0             G0_HIPP_CSC2_ODC0                ; /* 0x7188 */
    U_G0_HIPP_CSC2_ODC1             G0_HIPP_CSC2_ODC1                ; /* 0x718c */
    U_G0_HIPP_CSC2_ODC2             G0_HIPP_CSC2_ODC2                ; /* 0x7190 */
    U_G0_HIPP_CSC2_MIN_Y            G0_HIPP_CSC2_MIN_Y               ; /* 0x7194 */
    U_G0_HIPP_CSC2_MIN_C            G0_HIPP_CSC2_MIN_C               ; /* 0x7198 */
    U_G0_HIPP_CSC2_MAX_Y            G0_HIPP_CSC2_MAX_Y               ; /* 0x719c */
    U_G0_HIPP_CSC2_MAX_C            G0_HIPP_CSC2_MAX_C               ; /* 0x71a0 */
    unsigned int                    reserved_11[19]                  ; /* 0x71a4~0x71ec */
    U_G0_HIPP_CSC_INK_CTRL          G0_HIPP_CSC_INK_CTRL             ; /* 0x71f0 */
    U_G0_HIPP_CSC_INK_POS           G0_HIPP_CSC_INK_POS              ; /* 0x71f4 */
    U_G0_HIPP_CSC_INK_DATA          G0_HIPP_CSC_INK_DATA             ; /* 0x71f8 */
    U_G0_HIPP_CSC_INK_DATA2         G0_HIPP_CSC_INK_DATA2            ; /* 0x71fc */
    U_G0_DOF_CTRL                   G0_DOF_CTRL                      ; /* 0x7200 */
    U_G0_DOF_STEP                   G0_DOF_STEP                      ; /* 0x7204 */
    U_G0_DOF_BKG                    G0_DOF_BKG                       ; /* 0x7208 */
    U_G0_DOF_ALPHA                  G0_DOF_ALPHA                     ; /* 0x720c */
    unsigned int                    reserved_12[60]                  ; /* 0x7210~0x72fc */
    U_G0_ZME_HINFO                  G0_ZME_HINFO                     ; /* 0x7300 */
    U_G0_ZME_HSP                    G0_ZME_HSP                       ; /* 0x7304 */
    U_G0_ZME_HLOFFSET               G0_ZME_HLOFFSET                  ; /* 0x7308 */
    U_G0_ZME_HCOFFSET               G0_ZME_HCOFFSET                  ; /* 0x730c */
    unsigned int                    reserved_13[5]                   ; /* 0x7310~0x7320 */
    U_G0_ZME_COEF_REN               G0_ZME_COEF_REN                  ; /* 0x7324 */
    U_G0_ZME_COEF_RDATA             G0_ZME_COEF_RDATA                ; /* 0x7328 */
    unsigned int                    reserved_14[21]                  ; /* 0x732c~0x737c */
    U_G0_ZME_VINFO                  G0_ZME_VINFO                     ; /* 0x7380 */
    U_G0_ZME_VSP                    G0_ZME_VSP                       ; /* 0x7384 */
    U_G0_ZME_VOFFSET                G0_ZME_VOFFSET                   ; /* 0x7388 */
    unsigned int                    reserved_15[5917]                ; /* 0x738c~0xcffc */
    U_DHD0_CTRL                     DHD0_CTRL                        ; /* 0xd000 */
    U_DHD0_VSYNC1                   DHD0_VSYNC1                      ; /* 0xd004 */
    U_DHD0_VSYNC2                   DHD0_VSYNC2                      ; /* 0xd008 */
    U_DHD0_HSYNC1                   DHD0_HSYNC1                      ; /* 0xd00c */
    U_DHD0_HSYNC2                   DHD0_HSYNC2                      ; /* 0xd010 */
    U_DHD0_VPLUS1                   DHD0_VPLUS1                      ; /* 0xd014 */
    U_DHD0_VPLUS2                   DHD0_VPLUS2                      ; /* 0xd018 */
    U_DHD0_PWR                      DHD0_PWR                         ; /* 0xd01c */
    U_DHD0_VTTHD3                   DHD0_VTTHD3                      ; /* 0xd020 */
    U_DHD0_VTTHD                    DHD0_VTTHD                       ; /* 0xd024 */
    unsigned int                    reserved_16[4]                   ; /* 0xd028~0xd034 */
    U_DHD0_PARAUP                   DHD0_PARAUP                      ; /* 0xd038 */
    unsigned int                    reserved_17[19]                  ; /* 0xd03c~0xd084 */
    U_DHD0_STATE                    DHD0_STATE                       ; /* 0xd088 */
    unsigned int                    reserved_18                      ; /* 0xd08c */
    U_VO_MUX                        VO_MUX                           ; /* 0xd090 */
    unsigned int                    reserved_19[3931]                ; /* 0xd094~0x10dfc */
    U_GFX_READ_CTRL                 GFX_READ_CTRL                    ; /* 0x10e00 */
    U_GFX_MAC_CTRL                  GFX_MAC_CTRL                     ; /* 0x10e04 */
    U_GFX_OUT_CTRL                  GFX_OUT_CTRL                     ; /* 0x10e08 */
    unsigned int                    reserved_20[7]                   ; /* 0x10e0c~0x10e24 */
    U_GFX_1555_ALPHA                GFX_1555_ALPHA                   ; /* 0x10e28 */
    unsigned int                    reserved_21[5]                   ; /* 0x10e2c~0x10e3c */
    U_GFX_SRC_INFO                  GFX_SRC_INFO                     ; /* 0x10e40 */
    U_GFX_SRC_RESO                  GFX_SRC_RESO                     ; /* 0x10e44 */
    U_GFX_SRC_CROP                  GFX_SRC_CROP                     ; /* 0x10e48 */
    U_GFX_IRESO                     GFX_IRESO                        ; /* 0x10e4c */
    U_GFX_ADDR_H                    GFX_ADDR_H                       ; /* 0x10e50 */
    U_GFX_ADDR_L                    GFX_ADDR_L                       ; /* 0x10e54 */
    unsigned int                    reserved_22[2]                   ; /* 0x10e58~0x10e5c */
    U_GFX_STRIDE                    GFX_STRIDE                       ; /* 0x10e60 */
    unsigned int                    reserved_23[39]                  ; /* 0x10e64~0x10efc */
    U_GFX_CKEY_MAX                  GFX_CKEY_MAX                     ; /* 0x10f00 */
    U_GFX_CKEY_MIN                  GFX_CKEY_MIN                     ; /* 0x10f04 */
    U_GFX_CKEY_MASK                 GFX_CKEY_MASK                    ; /* 0x10f08 */

} S_VDP_REGS_TYPE;

/* Declare the struct pointor of the module VDP */

#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __VOU_REG_H__ */
