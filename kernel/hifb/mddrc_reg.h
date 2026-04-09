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

#ifndef __DDRC_REG_H__
#define __DDRC_REG_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define MDDRC_BASE_ADDR 0x20115000

typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mem_mode              : 1   ; // [0]
        unsigned int    mem_comb              : 2   ; // [2..1]
        unsigned int    Reserved              : 29  ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;
}U_DDR_MODE;

typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    apg_gt_en              : 1   ; // [0]
        unsigned int    muxcmd_gt_en           : 1   ; // [1]
        unsigned int    detaddr_gt_en          : 1   ; // [2]
        unsigned int    Reserved              : 29   ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;
}U_CLK_CFG;

// Define the union U_AWADDR_SRVLNC_START
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int awaddr_srvlnc_start                 : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_AWADDR_SRVLNC_START;

// Define the union U_AWADDR_SRVLNC_END
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int awaddr_srvlnc_end                 : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_AWADDR_SRVLNC_END;


typedef struct
{
    U_DDR_MODE ddr_mode;
    U_CLK_CFG  clk_cfg;
    unsigned int reserved_1[62];
    U_AWADDR_SRVLNC_START awaddr_srvlnc_start;
    U_AWADDR_SRVLNC_END awaddr_srvlnc_end;
    unsigned int reserved_2[62];
    unsigned int awaddr_srvlnc_status;
}MDDRC_REGS_S;




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // __DDRC_REG_H__

