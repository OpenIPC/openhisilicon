/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MDDDRC_REG_H
#define MDDDRC_REG_H

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef union {
    struct {
        unsigned int mem_mode : 1;  // [0]
        unsigned int mem_comb : 2;  // [2..1]
        unsigned int reserved : 29;  // [31..3]
    } bits;
    unsigned int u32;
} u_ddr_mode;

typedef union {
    struct {
        unsigned int apg_gt_en : 1;  // [0]
        unsigned int muxcmd_gt_en : 1;  // [1]
        unsigned int detaddr_gt_en : 1;  // [2]
        unsigned int reserved : 29;  // [31..3]
    } bits;
    unsigned int u32;
} u_clk_cfg;

typedef union {
    struct {
        unsigned int awaddr_srvlnc_start : 32;  // [31..0]
    } bits;
    unsigned int u32;
} u_awaddr_srvlnc_start;

typedef union {
    struct {
        unsigned int awaddr_srvlnc_end : 32;  // [31..0]
    } bits;
    unsigned int u32;
} u_awaddr_srvlnc_end;

typedef struct {
    u_ddr_mode ddr_mode;
    u_clk_cfg clk_cfg;
    unsigned int reserved_1[62]; /* 62 reserved length */
    u_awaddr_srvlnc_start awaddr_srvlnc_start;
    u_awaddr_srvlnc_end awaddr_srvlnc_end;
    unsigned int reserved_2[62]; /* 62 reserved length */
    unsigned int awaddr_srvlnc_status;
} mddrc_regs;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* MDDDRC_REG_H */