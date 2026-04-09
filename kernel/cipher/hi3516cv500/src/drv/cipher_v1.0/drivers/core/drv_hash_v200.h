/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv hash v200.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef _DRV_HASH_V100_H_
#define _DRV_HASH_V100_H_

#include "drv_osal_lib.h"

/* ************************** Internal Structure Definition *************************** */
/* Define the offset of reg */
#define SEC_CHN_CFG                      0x0304
#define CALC_ERR                         0x0320
#define CIPHER_INT_STATUS                0x0404
#define NORM_SMMU_START_ADDR             0x0440
#define SEC_SMMU_START_ADDR              0x0444
#define HASH_INT_STATUS                  0x0804
#define HASH_INT_EN                      0x0808
#define HASH_INT_RAW                     0x080C
#define HASH_IN_SMMU_EN                  0x0810
#define CHAN0_HASH_DAT_IN                0x0818
#define CHAN0_HASH_TOTAL_DAT_LEN         0x081C
#define chann_hash_ctrl(id)              (0x0800 + (id) * 0x80)
#define chann_hash_in_node_cfg(id)       (0x0804 + (id) * 0x80)
#define chann_hash_in_node_start_addr(id)(0x0808 + (id) * 0x80)
#define chann_hash_in_buf_rptr(id)       (0x080C + (id) * 0x80)
#define chann_hash_state_val(id)         (0x0340 + (id) * 0x08)
#define chann_hash_state_val_addr(id)    (0x0344 + (id) * 0x08)
#define chann_hash_in_node_start_high(id)(0x0820 + (id) * 0x80)

/* Define the union sec_chn_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    cipher_sec_chn_cfg      : 8   ; /* [7..0]  */
        hi_u32    cipher_sec_chn_cfg_lock : 1   ; /* [8]  */
        hi_u32    reserved_0              : 7   ; /* [15..9]  */
        hi_u32    hash_sec_chn_cfg        : 8   ; /* [23..16]  */
        hi_u32    hash_sec_chn_cfg_lock   : 1   ; /* [24]  */
        hi_u32    reserved_1              : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} sec_chn_cfg;

/* Define the union chan0_hash_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    hash_ch0_start              : 1   ; /* [0]  */
        hi_u32    hash_ch0_agl_sel            : 3   ; /* [3..1]  */
        hi_u32    hash_ch0_hmac_calc_step     : 1   ; /* [4]  */
        hi_u32    hash_ch0_mode               : 1   ; /* [5]  */
        hi_u32    hash_ch0_key_sel            : 1   ; /* [6]  */
        hi_u32    reserved_0                  : 2   ; /* [8..7]  */
        hi_u32    hash_ch0_auto_padding_en    : 1   ; /* [9]  */
        hi_u32    hash_ch0_hmac_key_addr      : 3   ; /* [12..10]  */
        hi_u32    hash_ch0_used               : 1   ; /* [13]  */
        hi_u32    hash_ch0_sec_alarm          : 1   ; /* [13]  */
        hi_u32    reserved_1                  : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} chan0_hash_ctrl;

/* Define the union hash_int_status */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 18  ; /* [17..0]  */
        hi_u32    hash_chn_oram_int     : 8   ; /* [25..18]  */
        hi_u32    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hash_int_status;

/* Define the union hash_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 18  ; /* [17..0]  */
        hi_u32    hash_chn_oram_en      : 8   ; /* [25..18]  */
        hi_u32    reserved_1            : 4   ; /* [29..26]  */
        hi_u32    hash_sec_int_en       : 1   ; /* [30]  */
        hi_u32    hash_int_en           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hash_int_en;

/* Define the union hash_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0            : 18  ; /* [17..0]  */
        hi_u32    hash_chn_oram_raw     : 8   ; /* [25..18]  */
        hi_u32    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hash_int_raw;

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

/* Define the union hash_in_smmu_en */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    hash_in_chan_rd_dat_smmu_en     : 7   ; /* [6..0]  */
        hi_u32    reserved_0                      : 9   ; /* [15..7]  */
        hi_u32    hash_in_chan_rd_node_smmu_en    : 7   ; /* [22..16]  */
        hi_u32    reserved_1                      : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} hash_in_smmu_en;

/* Define the union chann_hash_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    reserved_0                      : 1   ; /* [0]  */
        hi_u32    hash_chn_agl_sel                : 3   ; /* [3..1]  */
        hi_u32    reserved_1                      : 1   ; /* [4]  */
        hi_u32    hash_chn_mode                   : 1   ; /* [5]  */
        hi_u32    hash_chn_key_sel                : 1   ; /* [6]  */
        hi_u32    hash_chn_dat_in_byte_swap_en    : 1   ; /* [7]  */
        hi_u32    hash_chn_dat_in_bit_swap_en     : 1   ; /* [8]  */
        hi_u32    hash_chn_auto_padding_en        : 1   ; /* [9]  */
        hi_u32    hash_chn_hmac_key_addr          : 3   ; /* [12..10]  */
        hi_u32    reserved_2                      : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} chann_hash_ctrl;

/* Define the union chann_hash_int_node_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    hash_in_node_mpackage_int_level : 8   ; /* [7..0]  */
        hi_u32    hash_in_node_rptr               : 8   ; /* [15..8]  */
        hi_u32    hash_in_node_wptr               : 8   ; /* [23..16]  */
        hi_u32    hash_in_node_total_num          : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} chann_hash_int_node_cfg;

#endif /* End of _DRV_HASH_V100_H_ */
