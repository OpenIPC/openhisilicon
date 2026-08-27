/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_symc_v200.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef _DRV_SYMC_V200_H_
#define _DRV_SYMC_V200_H_

#include "drv_osal_lib.h"

/*************************** Internal Structure Definition ****************************/
/** \addtogroup      cipher drivers*/
/** @{*/  /** <!-- [cipher]*/

/*! hash in entry list size */
#define SYMC_NODE_SIZE             (SYMC_MAX_LIST_NUM * 32)

/*! hash in entry list size */
#define SYMC_NODE_LIST_SIZE        (SYMC_NODE_SIZE * 2 * CIPHER_HARD_CHANNEL_CNT)

/*! \Define the offset of reg */
#define CHANn_CIPHER_IVOUT(id)                  (0x0000 + (id)*0x10)
#define CHAN0_CIPHER_DOUT                       (0x0080)
#define CIPHER_KEY(id)                          (0x0100 + (id)*0x20)
#define SM1_SK(id)                              (0x0200 + (id)*0x10)
#define ODD_EVEN_KEY_SEL                        (0x0290)
#define HDCP_MODE_CTRL                          (0x0300)
#define SEC_CHN_CFG                             (0x0304)
#define CALC_ERR                                (0x0320)
#define CHAN0_CIPHER_CTRL                       (0x0400)
#define CIPHER_INT_STATUS                       (0x0404)
#define CIPHER_INT_EN                           (0x0408)
#define CIPHER_INT_RAW                          (0x040c)
#define CIPHER_IN_SMMU_EN                       (0x0410)
#define OUT_SMMU_EN                             (0x0414)
#define CHAN0_CIPHER_DIN                        (0x0420)
#define NORM_SMMU_START_ADDR                    (0x0440)
#define SEC_SMMU_START_ADDR                     (0x0444)
#define CHANn_CIPHER_CTRL(id)                   (0x0400 + (id)*0x80)
#define CHANn_CIPHER_IN_NODE_CFG(id)            (0x0404 + (id)*0x80)
#define CHANn_CIPHER_IN_NODE_START_ADDR(id)     (0x0408 + (id)*0x80)
#define CHANn_CIPHER_IN_BUF_RPTR(id)            (0x040C + (id)*0x80)
#define CHANn_CIPHER_OUT_NODE_CFG(id)           (0x0430 + (id)*0x80)
#define CHANn_CIPHER_OUT_NODE_START_ADDR(id)    (0x0434 + (id)*0x80)
#define CHANn_CIPHER_OUT_BUF_RPTR(id)           (0x0438 + (id)*0x80)
#define CHANN_CIPHER_IN_NODE_START_HIGH(id)     (0x0460 + (id)*0x80)
#define CHANN_CIPHER_OUT_NODE_START_HIGH(id)    (0x0470 + (id)*0x80)
#define HASH_INT_STATUS                         (0x0804)
#define CHN_WHO_USED_REE                        (0x0390)
#define CHN_WHO_USED_TEE                        (0x0394)
#define SYMC_CHN_MASK(id)                       (0x01 << (id))

/* reducing power dissipation */
#define SPACC_CALC_CRG_CFG                      (0x039C)

/* Define the union cal_crg_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0                   : 28   ; /* [27..0] */
        hi_u32    spacc_core_auto_cken_bypass  : 1    ; /* [28]  */
        hi_u32    spacc_rft_mem_wr_clk_gt_en   : 1    ; /* [29]  */
        hi_u32    spacc_rft_mem_rd_clk_gt_en   : 1    ; /* [30]  */
        hi_u32    spacc_rfs_mem_clk_gt_en      : 1    ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} cal_crg_cfg;

/* Define the union sec_chn_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    cipher_sec_chn_cfg    : 8   ; /* [7..0]  */
        hi_u32    cipher_sec_chn_cfg_lock : 1   ; /* [8]  */
        hi_u32    reserved_0            : 7   ; /* [15..9]  */
        hi_u32    hash_sec_chn_cfg      : 8   ; /* [23..16]  */
        hi_u32    hash_sec_chn_cfg_lock : 1   ; /* [24]  */
        hi_u32    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} sec_chn_cfg;

/* Define the union cipher_int_status */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 1   ; /* [0]  */
        hi_u32    cipher_chn_ibuf_int   : 7   ; /* [7..1]  */
        hi_u32    cipher_chn_obuf_int   : 8   ; /* [15..8]  */
        hi_u32    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} cipher_int_status;

/* Define the union cipher_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 1   ; /* [0]  */
        hi_u32    cipher_chn_ibuf_en    : 7   ; /* [7..1]  */
        hi_u32    cipher_chn_obuf_en    : 8   ; /* [15..8]  */
        hi_u32    reserved_1            : 14  ; /* [29..16]  */
        hi_u32    cipher_sec_int_en     : 1   ; /* [30]  */
        hi_u32    cipher_nsec_int_en    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} cipher_int_en;

/* Define the union cipher_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 1   ; /* [0]  */
        hi_u32    cipher_chn_ibuf_raw   : 7   ; /* [7..1]  */
        hi_u32    cipher_chn_obuf_raw   : 8   ; /* [15..8]  */
        hi_u32    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} cipher_int_raw;

/* Define the union hash_int_status */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 18  ; /* [17..0]  */
        unsigned int    hash_chn_oram_int     : 8   ; /* [25..18]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} hash_int_status;

/* Define the union cipher_in_smmu_en */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    cipher_in_chan_rd_dat_smmu_en : 7   ; /* [6..0]  */
        hi_u32    reserved_0            : 9   ; /* [15..7]  */
        hi_u32    cipher_in_chan_rd_node_smmu_en : 7   ; /* [22..16]  */
        hi_u32    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} cipher_in_smmu_en;

/* Define the union out_smmu_en */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    out_chan_wr_dat_smmu_en : 7   ; /* [6..0]  */
        hi_u32    reserved_0            : 9   ; /* [15..7]  */
        hi_u32    out_chan_rd_node_smmu_en : 7   ; /* [22..16]  */
        hi_u32    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} out_smmu_en;

/* Define the union chann_chipher_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 1   ; /* [0]  */
        hi_u32    sym_chn_alg_mode      : 3   ; /* [3..1]  */
        hi_u32    sym_chn_alg_sel       : 3   ; /* [6..4]  */
        hi_u32    sym_chn_decrypt       : 1   ; /* [7]  */
        hi_u32    sym_chn_dat_width     : 2   ; /* [9..8]  */
        hi_u32    sym_chn_key_length    : 2   ; /* [11..10]  */
        hi_u32    reserved_1            : 2   ; /* [13..12]  */
        hi_u32    sym_chn_key_sel       : 1   ; /* [14]  */
        hi_u32    reserved_2            : 1   ; /* [15]  */
        hi_u32    sym_chn_dout_byte_swap_en : 1   ; /* [16]  */
        hi_u32    sym_chn_din_byte_swap_en : 1   ; /* [17]  */
        hi_u32    sym_chn_sm1_round_num : 2   ; /* [19..18]  */
        hi_u32    reserved_3            : 2   ; /* [21..20]  */
        hi_u32    weight                : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} chann_chipher_ctrl;

/* Define the union chann_cipher_in_node_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    cipher_in_node_mpackage_int_level : 7   ; /* [6..0]  */
        hi_u32    reserved_0            : 1   ; /* [7]  */
        hi_u32    cipher_in_node_rptr   : 7   ; /* [14..8]  */
        hi_u32    reserved_1            : 1   ; /* [15]  */
        hi_u32    cipher_in_node_wptr   : 7   ; /* [22..16]  */
        hi_u32    reserved_2            : 1   ; /* [23]  */
        hi_u32    cipher_in_node_total_num : 7   ; /* [30..24]  */
        hi_u32    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} chann_cipher_in_node_cfg;

/* Define the union chann_cipher_out_node_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    cipher_out_node_mpackage_int_level : 7   ; /* [6..0]  */
        hi_u32    reserved_0            : 1   ; /* [7]  */
        hi_u32    cipher_out_node_rptr  : 7   ; /* [14..8]  */
        hi_u32    reserved_1            : 1   ; /* [15]  */
        hi_u32    cipher_out_node_wptr  : 7   ; /* [22..16]  */
        hi_u32    reserved_2            : 1   ; /* [23]  */
        hi_u32    cipher_out_node_total_num : 7   ; /* [30..24]  */
        hi_u32    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;

} chann_cipher_out_node_cfg;

/** @}*/  /** <!-- ==== Structure Definition end ====*/
#endif
