/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_KLAD_REG_H
#define HAL_KLAD_REG_H

#define KEY_SIZE_64_BIT    8
#define KEY_SIZE_128_BIT   16
#define KEY_SIZE_192_BIT   24
#define KEY_SIZE_256_BIT   32
#define SBRK_DISABLE_VAL   0x5
#define ABRK_DISABLE_VAL   0x5

#define KEY_SIZE_64_BIT_REG_VAL    0x0
#define KEY_SIZE_128_BIT_REG_VAL   0x1
#define KEY_SIZE_192_BIT_REG_VAL   0x2
#define KEY_SIZE_256_BIT_REG_VAL   0x3
#define KLAD_LOCK_REG_CONFIG_VALUE   1
#define KLAD_UNLOCK_REG_CONFIG_VALUE 1
#define KLAD_INVALID_HANDLE_INDEX    (-1)

/*
 * keyload
 */
#define KLAD_REG_OFFSET              (0x00001000)

#define KL_DATA_IN_0        (KLAD_REG_OFFSET + 0x000)
#define KL_DATA_IN_1        (KLAD_REG_OFFSET + 0x004)
#define KL_DATA_IN_2        (KLAD_REG_OFFSET + 0x008)
#define KL_DATA_IN_3        (KLAD_REG_OFFSET + 0x00c)
#define KL_DATA_IN(n)       (KLAD_REG_OFFSET + 0x000 + ((n) * 0x4))  /* n 0~3 */
#define KL_KEY_ADDR         (KLAD_REG_OFFSET + 0x010)
#define KL_KEY_CFG          (KLAD_REG_OFFSET + 0x014)
#define KL_KEY_SEC_CFG      (KLAD_REG_OFFSET + 0x018)
#define KL_STATE            (KLAD_REG_OFFSET + 0x030)
#define KL_CRC              (KLAD_REG_OFFSET + 0x034)
#define KL_ERROR            (KLAD_REG_OFFSET + 0x038)
#define KC_ERROR            (KLAD_REG_OFFSET + 0x03c)
#define KL_INT_EN           (KLAD_REG_OFFSET + 0x040)
#define KL_INT_RAW          (KLAD_REG_OFFSET + 0x044)
#define KL_INT              (KLAD_REG_OFFSET + 0x048)
#define SBRK_DISABLE        (KLAD_REG_OFFSET + 0x060)
#define ABRK_DISABLE        (KLAD_REG_OFFSET + 0x064)
#define KL_RK_GEN_STATUS    (KLAD_REG_OFFSET + 0x070)
#define KL_LOCK_CTRL        (KLAD_REG_OFFSET + 0x074)
#define KL_UNLOCK_CTRL      (KLAD_REG_OFFSET + 0x078)
#define KL_COM_LOCK_INFO    (KLAD_REG_OFFSET + 0x07c)
#define KL_COM_LOCK_STATUS  (KLAD_REG_OFFSET + 0x080)
#define KL_COM_CTRL         (KLAD_REG_OFFSET + 0x084)
#define KL_COM_STATUS       (KLAD_REG_OFFSET + 0x088)
#define KL_CLR_CTRL         (KLAD_REG_OFFSET + 0x438)
#define KL_ALARM_INFO       (KLAD_REG_OFFSET + 0x600)

/*
    klad attach kslot type
*/
#define ATTACH_FLASH    2
#define ATTACH_MCIPHER  3
#define ATTACH_HMAC     4

/*
    klad data in reg num
*/
#define KALD_DATA_IN_REG_NUM  4

/*
    pbkdf2_key_config
*/
#define KDF_SW_GEN      3
#define KDF_USD         4
#define KDF_SBRK        5
#define KDF_ABRK        6
#define KDF_ODRK0       7
#define KDF_ODRK1       8
#define KDF_RDRK        9
#define KDF_MDRK0       10
#define KDF_FDRK        11
#define KDF_MDRK1       12
#define KDF_DRK         13
#define KDF_MDRK2       14
#define KDF_MDRK3       15
#define KDF_SBRK2       16
#define KDF_ABRK2       17
#define KDF_PSK         18
#define KDF_ABRK_REE    20
#define KDF_RDRK_REE    22

#define KLAD_KEY_TYPE_SBRK0  0x0
#define KLAD_KEY_TYPE_SBRK1  0x1
#define KLAD_KEY_TYPE_SBRK2  0x10
#define KLAD_KEY_TYPE_ABRK0  0x2
#define KLAD_KEY_TYPE_ABRK1  0x3
#define KLAD_KEY_TYPE_ABRK2  0x11
#define KLAD_KEY_TYPE_DRK0   0xc
#define KLAD_KEY_TYPE_DRK1   0xd
#define KLAD_KEY_TYPE_RDRK0  0xe
#define KLAD_KEY_TYPE_RDRK1  0xf
#define KLAD_KEY_TYPE_PSK    0x12
#define KLAD_KEY_TYPE_FDRK0  0x4
#define KLAD_KEY_TYPE_ODRK0  0x5
#define KLAD_KEY_TYPE_OARK0  0x6
#define KLAD_KEY_TYPE_ODRK1  0x7
#define KLAD_KEY_TYPE_MDRK0  0x8
#define KLAD_KEY_TYPE_ABRK_REE 0x15
#define KLAD_KEY_TYPE_RDRK_REE 0x17

#define KDF_FUNC_JTAG   0x13
#define KDF_TEE_JTAG    0x14
#define KDF_DFT_JTAG    0x15

#define KLAD_DEST_TYPE_INVALID    0
#define KLAD_DEST_TYPE_MCIPHER    1
#define KLAD_DEST_TYPE_AI_AUDIO   4
#define KLAD_DEST_TYPE_AI_NPU     5
#define KLAD_DEST_TYPE_FLASH      7
#define KLAD_UNLOCK_STATUS        0x0

#define KLAD_DEST_TYPE_INVALID_KSLOT     0xffffffff
#define KLAD_CLEAR_KEY_INVALID_HMAC      0xff

#define CLEAR_HMAC_KEY_BLOCK_SIZE_512    64
#define CLEAR_HMAC_KEY_BLOCK_SIZE_1024   128
#define CLEAR_HMAC_KEY_CAL_CNT_512       4
#define CLEAR_HMAC_KEY_CAL_CNT_1024      8
#define BYTE_TO_BITS                     8
#define HKL_KEY_LEN                      16
#define HKL_KEY_LEN_32                   32

#define KLAD_CHANNEL_MAX_SUPPORT         1

/* define the union hkl_lock_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_lock               : 1; /* [0]  */
        unsigned int    reserved_0            : 3; /* [3..1]  */
        unsigned int    kl_lock_num           : 3; /* [6..4]  */
        unsigned int    reserved_1            : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_lock_ctrl;

/* define the union hkl_com_lock_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_com_lock_busy      : 2; /* [1..0]  */
        unsigned int    kl_com_lock_fail      : 2; /* [3..2]  */
        unsigned int    kl_com_unlock_fail    : 2; /* [5..4]  */
        unsigned int    reserved_0            : 2; /* [7..6]  */
        unsigned int    kl_com_lock_num       : 3; /* [10..8]  */
        unsigned int    reserved_1            : 21; /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_com_lock_info;

/* Define the union U_KL_COM_LOCK_STATUS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    kl_com_lock_stat      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} hkl_com_lock_status;

/* define the union hkl_unlock_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_unlock             : 1; /* [0]  */
        unsigned int    reserved_0            : 3; /* [3..1]  */
        unsigned int    kl_unlock_num         : 3; /* [6..4]  */
        unsigned int    reserved_1            : 1; /* [7]  */
        unsigned int    kl_com_unlock_num     : 3; /* [10..8]  */
        unsigned int    reserved_2            : 21; /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_unlock_ctrl;

/* Define the union KL_COM_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    kl_com_start          : 1; /* [0]  */
        unsigned int    kl_com_level_sel      : 3; /* [3..1]  */
        unsigned int    kl_com_alg_sel        : 2; /* [5..4]  */
        unsigned int    kl_com_key_size       : 2; /* [7..6]  */
        unsigned int    rk_choose             : 5; /* [12..8]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} kl_com_ctrl;

/* Define the union U_KL_COM_STATUS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    kl_com_rk_rdy         : 1; /* [0]  */
        unsigned int    kl_com_lv1_rdy        : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} kl_com_status;

/* define the union hkl_key_sec_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    key_sec               : 1; /* [0]  */
        unsigned int    src_nsec              : 1; /* [1]  */
        unsigned int    src_sec               : 1; /* [2]  */
        unsigned int    dest_nsec             : 1; /* [3]  */
        unsigned int    dest_sec              : 1; /* [4]  */
        unsigned int    master_only           : 1; /* [5]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_key_sec_cfg;

/* define the union u_kl_key_addr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    key_addr              : 10; /* [9..0]  */
        unsigned int    reserved_0            : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_key_addr;

/* define the union hkl_key_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    port_sel              : 3; /* [2..0]  */
        unsigned int    reserved_0            : 1; /* [3]  */
        unsigned int    dsc_code              : 8; /* [11..4]  */
        unsigned int    reserved_1            : 4; /* [15..12]  */
        unsigned int    key_enc               : 1; /* [16]  */
        unsigned int    key_dec               : 1; /* [17]  */
        unsigned int    kl_flash_sel          : 2; /* [19..18]  */
        unsigned int    reserved_2            : 12; /* [31..20]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_key_cfg;

/* Define the union U_KL_CLR_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    kl_clr_start          : 1; /* [0]  */
        unsigned int    reserved_0            : 1; /* [1]  */
        unsigned int    kl_clr_key_size       : 2; /* [3..2]  */
        unsigned int    reserved_1            : 9; /* [12..4]  */
        unsigned int    kl_clr_key_cnt        : 3; /* [15..13]  */
        unsigned int    reserved_2            : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} kl_clr_ctrl;

/* define the union hkl_nonce_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_nonce_start        : 1; /* [0]  */
        unsigned int    reserved_0            : 3; /* [3..1]  */
        unsigned int    kl_nonce_alg_sel      : 2; /* [5..4]  */
        unsigned int    reserved_1            : 26; /* [31..6]  */
    } bits;
    /* define an unsigned member */
    unsigned int    u32;
} hkl_nonce_ctrl;

/* define the union hkl_nonce_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_nonce_rk_rdy       : 1; /* [0]  */
        unsigned int    kl_nonce_lvl1_rdy     : 1; /* [1]  */
        unsigned int    kl_nonce_lvl2_rdy     : 1; /* [2]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_nonce_status;

/* define the union hkl_clr_lock_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_clr_lock_busy      : 2; /* [1..0]  */
        unsigned int    kl_clr_lock_fail      : 2; /* [3..2]  */
        unsigned int    kl_clr_unlock_fail    : 2; /* [5..4]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_clr_lock_info;

/* define the union hkl_clr_lock_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_clr_lock_stat      : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_clr_lock_status;

/* define the union hkl_clr_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_clr_start          : 1; /* [0]  */
        unsigned int    kl_clr_iv_sel         : 1; /* [1]  */
        unsigned int    kl_clr_key_size       : 2; /* [3..2]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_clr_ctrl;

/* define the union hkl_fp_lock_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_fp_lock_busy       : 2; /* [1..0]  */
        unsigned int    kl_fp_lock_fail       : 2; /* [3..2]  */
        unsigned int    kl_fp_unlock_fail     : 2; /* [5..4]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_fp_lock_info;

/* define the union hkl_fp_lock_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_fp_lock_stat       : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_fp_lock_status;

/* define the union hkl_fp_rk_sel */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_fp_rk_sel          : 3; /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_fp_rk_sel;

/* define the union hkl_fp_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_fp_start           : 1; /* [0]  */
        unsigned int    kl_fp_level_sel       : 1; /* [1]  */
        unsigned int    reserved_0            : 4; /* [5..2]  */
        unsigned int    kl_fp_dec_sel         : 1; /* [6]  */
        unsigned int    reserved_1            : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_fp_ctrl;

/* define the union hkl_fp_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_fp_rk_rdy          : 1; /* [0]  */
        unsigned int    kl_fp_lv1_enc_rdy     : 1; /* [1]  */
        unsigned int    kl_fp_lv1_dec_rdy     : 1; /* [2]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_fp_status;

/* define the union hkl_fp_dec_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_fp_dec_rd_dis      : 4; /* [3..0]  */
        unsigned int    kl_fp_dec_route_dis   : 4; /* [7..4]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_fp_dec_ctrl;

/* define the union hkl_ta_lock_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_ta_lock_busy       : 2; /* [1..0]  */
        unsigned int    kl_ta_lock_fail       : 2; /* [3..2]  */
        unsigned int    kl_ta_unlock_fail     : 2; /* [5..4]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_ta_lock_info;

/* define the union hkl_ta_lock_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_ta_lock_stat       : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_ta_lock_status;

/* define the union hkl_ta_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_ta_start           : 1; /* [0]  */
        unsigned int    kl_ta_level_sel       : 2; /* [2..1]  */
        unsigned int    reserved_0            : 2; /* [4..3]  */
        unsigned int    kl_ta_cur_128bit_cnt  : 6; /* [10..5]  */
        unsigned int    kl_ta_last_time       : 1; /* [11]  */
        unsigned int    kl_ta_lut_alg_sel     : 1; /* [12]  */
        unsigned int    reserved_1            : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_ta_ctrl;

/* define the union hkl_ta_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_ta_rk_rdy          : 1; /* [0]  */
        unsigned int    kl_ta_lvl1_rdy        : 1; /* [1]  */
        unsigned int    kl_ta_lvl2_rdy        : 1; /* [2]  */
        unsigned int    kl_ta_lvl3_rdy        : 1; /* [3]  */
        unsigned int    kl_ta_f_lut_rdy       : 1; /* [4]  */
        unsigned int    kl_ta_f_m_rdy         : 1; /* [5]  */
        unsigned int    kl_ta_f_bc_rdy        : 1; /* [6]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_ta_status;

/* define the union hkl_csgk2_lock_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_csgk2_lock_busy    : 2; /* [1..0]  */
        unsigned int    kl_csgk2_lock_fail    : 2; /* [3..2]  */
        unsigned int    kl_csgk2_unlock_fail  : 2; /* [5..4]  */
        unsigned int    reserved_0            : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_csgk2_lock_info;

/* define the union hkl_csgk2_lock_status */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_csgk2_lock_stat    : 8; /* [7..0]  */
        unsigned int    reserved_0            : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_csgk2_lock_status;

/* define the union hkl_csgk2_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_csgk2_start        : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_csgk2_ctrl;

/* define the union hkl_csgk2_disable */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_csgk2_dis          : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_csgk2_disable;

/* define the union hkl_csgk2_disable_lock */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    kl_csgk2_dis_lock     : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_csgk2_disable_lock;

/* define the union hkl_alarm_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int    rng_crc4_alarm        : 1; /* [0]  */
        unsigned int    kl_cfg_sig_alarm      : 1; /* [1]  */
        unsigned int    kl_rk_tag_sig_alarm   : 1; /* [2]  */
        unsigned int    kl_rk_tag_crc16_alarm : 1; /* [3]  */
        unsigned int    kl_rk_info_crc4_alarm : 1; /* [4]  */
        unsigned int    kl_sel_sig_alarm      : 1; /* [5]  */
        unsigned int    kl_com_crc16_alarm    : 1; /* [6]  */
        unsigned int    kl_nonce_crc16_alarm  : 1; /* [7]  */
        unsigned int    kl_fp_crc16_alarm     : 1; /* [8]  */
        unsigned int    kl_ta_crc16_alarm     : 1; /* [9]  */
        unsigned int    kl_ta_fsm_alarm       : 1; /* [10]  */
        unsigned int    reserved_0            : 13; /* [23..11]  */
        unsigned int    cm_core_alarm         : 1; /* [24]  */
        unsigned int    reserved_1            : 7; /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int    u32;
} hkl_alarm_info;

/* Define the union sbrk_disable */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    sbrk0_disable                : 4; /* [3..0]  */
        unsigned int    sbrk1_disable                : 4; /* [7..4]  */
        unsigned int    reserved                     : 24; /* [31..8]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} sbrk_disable;

/* Define the union sbrk_disable */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    abrk0_disable                : 4; /* [3..0]  */
        unsigned int    reserved                     : 28; /* [31..4]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} abrk_disable;

/* Define the union kl_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    kl_int_en                    : 1; /* [0]  */
        unsigned int    reserved                     : 31; /* [31..1]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} kl_int_cfg;

/* Define the union kl_int */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    kl_int                       : 1; /* [0]  */
        unsigned int    reserved                     : 31; /* [31..1]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} kl_int_status;

/* Define the union kl_int_raw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    com_kl_int_raw               : 1; /* [0]  */
        unsigned int    reserved0                    : 3; /* [3..1]  */
        unsigned int    kl_int_num                   : 5; /* [8..4]  */
        unsigned int    reserved1                    : 5; /* [13..9]  */
        unsigned int    clr_kl_int_raw               : 1; /* [14]  */
        unsigned int    reserved2                    : 18; /* [31..15]  */
    } bits;
    /* Define an unsigned member */
    unsigned int    u32;
} kl_int_raw;


#endif