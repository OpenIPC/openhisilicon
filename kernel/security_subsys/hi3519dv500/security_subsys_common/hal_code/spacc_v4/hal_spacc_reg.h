/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_HASH_REG_H
#define HAL_HASH_REG_H

typedef enum {
    SPACC_CPU_IDLE = 0x00000000,
    SPACC_CPU_REE = 0x00000001,
    SPACC_CPU_TEE = 0x00000002,
    SPACC_CPU_PCPU = 0x00000004,
    SPACC_CPU_AIDSP = 0x00000005,
    SPACC_CPU_INVALID = 0xffffffff
} spacc_cpu_mask;

#define SPACC_CALC_CRG_CFG                  (0x8030)

#define SPACC_IE                            (0x00000004)

#define CHN_WHO_USED_CLR(used, chn)      (used) &= ~(0x0fU << ((chn) * 4U))
#define CHN_WHO_USED_SET(used, chn, who) (used) |= (td_u32)(who) << ((td_u32)(chn) * 4U)
#define CHN_WHO_USED_GET(used, chn)      ((((used) >> ((chn) * 4U))) & 0x0fU)

/*! \Define the offset of HASH reg */
#define SPACC_HASH_CHN_LOCK                        0x0040
#define SPACC_HASH_CHN_CLEAR_REQ                   0x0068
#define SPACC_INT_RAW_HASH_CLEAR_FINISH            0x0014
#define CHANN_HASH_STATE_VAL_ADDR(id)              (0xa004 + (id) * 0x80)
#define CHANN_HASH_STATE_VAL(id)                   (0xa000 + (id) * 0x80)
#define IN_HASH_CHN_CTRL(id)                       (0x5100 + ((id) - 1) * 0x100)
#define IN_HASH_CHN_KEY_CTRL(id)                   (0x5110 + ((id) - 1) * 0x100)
#define IN_HASH_CHN_NODE_START_ADDR_H(id)          (0x5120 + ((id) - 1) * 0x100)
#define IN_HASH_CHN_NODE_START_ADDR_L(id)          (0x5124 + ((id) - 1) * 0x100)
#define IN_HASH_CHN_NODE_LENGTH(id)                (0x512c + ((id) - 1) * 0x100)
#define IN_HASH_CHN_NODE_WR_POINT(id)              (0x5130 + ((id) - 1) * 0x100)
#define IN_HASH_CHN_NODE_RD_POINT(id)              (0x5134 + ((id) - 1) * 0x100)
#define IN_HASH_CHN_DATA_LEN(id)                   (0x51C0 + ((id) - 1) * 0x100)
#define HASH_CHANN_RAW_INT                         0x8600
#define HASH_CHANN_RAW_INT_EN                      0x8604
#define TEE_HASH_CALC_CTRL_CHECK_ERR               0x8068
#define TEE_HASH_CALC_CTRL_CHECK_ERR_STATUS        0x806c
#define REE_HASH_CALC_CTRL_CHECK_ERR               0x8078
#define REE_HASH_CALC_CTRL_CHECK_ERR_STATUS        0x807c
#define PCPU_HASH_CALC_CTRL_CHECK_ERR              0x8088
#define PCPU_HASH_CALC_CTRL_CHECK_ERR_STATUS       0x808c
#define AIDSP_HASH_CALC_CTRL_CHECK_ERR             0x8098
#define AIDSP_HASH_CALC_CTRL_CHECK_ERR_STATUS      0x809c

#define SYM_CALC_CTRL_CHECK_ERR(cpu)        (0x8000 + 0x80 - (cpu) * 0x10)

#define ENCRYPT 0x5A
#define DECRYPT 0xA5

/* ! \Define the offset of Cipher reg */
#define SPACC_SYM_CHN_LOCK                    0x0020
#define SPACC_INT_RAW_SYM_CLEAR_FINISH        0x0008
#define SPACC_EN_INT_RAW_SYM_CLEAR_FINISH     0x000c
#define SPACC_SYM_CHN_CLEAR_REQ               0x0060
#define SPACC_BUS_ERR                         0x0200
#define IN_SYM_CHN_CTRL(id)                   (0x4100 + ((id) - 1) * 0x100)
#define IN_SYM_OUT_CTRL(id)                   (0x4104 + ((id) - 1) * 0x100)
#define IN_SYM_CHN_KEY_CTRL(id)               (0x4110 + ((id) - 1) * 0x100)
#define IN_SYM_CHN_NODE_START_ADDR_H(id)      (0x4120 + ((id) - 1) * 0x100)
#define IN_SYM_CHN_NODE_START_ADDR_L(id)      (0x4124 + ((id) - 1) * 0x100)
#define IN_SYM_CHN_NODE_LENGTH(id)            (0x412c + ((id) - 1) * 0x100)
#define IN_SYM_CHN_NODE_WR_POINT(id)          (0x4130 + ((id) - 1) * 0x100)
#define IN_SYM_CHN_NODE_RD_POINT(id)          (0x4134 + ((id) - 1) * 0x100)
#define IN_SYM_CHN_NODE_CTRL(id)              (0x4140 + ((id) - 1) * 0x100)
#define DBG_IN_SYM_CHN_RD_ADDR_H(id)          (0x41a0 + ((id) - 1) * 0x100)
#define DBG_IN_SYM_CHN_RD_ADDR_L(id)          (0x41a4 + ((id) - 1) * 0x100)
#define DBG_IN_SYM_CHN_DATA_LEN(id)           (0x41c0 + ((id) - 1) * 0x100)

#define TEE_SYM_CALC_CTRL_CHECK_ERR           0x8060
#define TEE_SYM_CALC_CTRL_CHECK_ERR_STATUS    0x8064
#define REE_SYM_CALC_CTRL_CHECK_ERR           0x8070
#define REE_SYM_CALC_CTRL_CHECK_ERR_STATUS    0x8074
#define PCPU_SYM_CALC_CTRL_CHECK_ERR          0x8080
#define PCPU_SYM_CALC_CTRL_CHECK_ERR_STATUS   0x8084
#define AIDSP_SYM_CALC_CTRL_CHECK_ERR         0x8090
#define AIDSP_SYM_CALC_CTRL_CHECK_ERR_STATUS  0x8094

#define SYM_CHANN_RAW_INT     0x8610
#define SYM_CHANN_RAW_INT_EN  0x8614
#define SYM_CHANN_INT         0x8618

#define CHANN_CIPHER_IVOUT(id)             (0x9000 + (id) * 0x80)
#define CHANN_CIPHER_DFA_EN(id)            (0x9020 + (id) * 0x80)

#define OUT_SYM_CHAN_RAW_LAST_NODE_INT     0xc000
#define OUT_SYM_CHAN_RAW_LAST_NODE_INT_EN  0xc004
#define OUT_SYM_CHAN_LAST_NODE_INT         0xc008
#define OUT_SYM_CHAN_RAW_LEVEL_INT         0xc010
#define OUT_SYM_CHAN_RAW_LEVEL_INT_EN      0xc014
#define OUT_SYM_CHAN_LEVEL_INT             0xc018

#define OUT_SYM_CHN_STATUS(id)                (0xD004 + ((id) - 1) * 0x100)
#define OUT_SYM_CHN_NODE_START_ADDR_H(id)     (0xD020 + ((id) - 1) * 0x100)
#define OUT_SYM_CHN_NODE_START_ADDR_L(id)     (0xD024 + ((id) - 1) * 0x100)
#define OUT_SYM_CHN_NODE_LENGTH(id)           (0xD028 + ((id) - 1) * 0x100)
#define OUT_SYM_CHN_NODE_WR_POINT(id)         (0xD030 + ((id) - 1) * 0x100)
#define OUT_SYM_CHN_NODE_RD_POINT(id)         (0xD034 + ((id) - 1) * 0x100)
#define OUT_SYM_CHN_NODE_CTRL(id)             (0xD040 + ((id) - 1) * 0x100)
#define DBG_OUT_SYM_CHN_RD_ADDR_H(id)         (0xD0a0 + ((id) - 1) * 0x100)
#define DBG_OUT_SYM_CHN_RD_ADDR_L(id)         (0xD0a4 + ((id) - 1) * 0x100)
#define DBG_OUT_SYM_CHN_NODE_LEFT_BUF_LEN(id) (0xD0a8 + ((id) - 1) * 0x100)

#define IN_SYM_CHN0_CTRL                   0x4000
#define IN_SYM_CHN0_SPECIAL_CTRL           0x4004
#define IN_SYM_CHN0_KEY_CTRL               0x4010
#define IN_SYM_CHN0_IV_DATA_CTRL           0x4030
#define IN_SYM_CHN0_IV0                    0x4040
#define IN_SYM_CHN0_IV1                    0x4044
#define IN_SYM_CHN0_IV2                    0x4048
#define IN_SYM_CHN0_IV3                    0x404C
#define IN_SYM_CHN0_DATA0                  0x4050
#define IN_SYM_CHN0_DATA1                  0x4054
#define IN_SYM_CHN0_DATA2                  0x4058
#define IN_SYM_CHN0_DATA3                  0x405C
#define CHAN0_CIPHER_DOUT0                 0x8100

#define SYMC_KEY_SIZE (32)
#define NODE_DEPTH    (2)

#define SYMC_CFG_SECURE             (0x05)
#define SYMC_CFG_NON_SECURE         (0x0A)


#define ALG_DMA_REG   (0)
#define ALG_AES_REG   (2)
#define ALG_LEA_REG   (4)
#define ALG_SM4_REG   (5)
#define ALG_GHASH_REG (6)
#define ALG_TDES_REG  (7)

#define MODE_ECB_REG        (0x01)
#define MODE_CBC_REG        (0x03)
#define MODE_CTR_REG        (0x06)
#define MODE_OFB_REG        (0x07)
#define MODE_CFB_REG        (0x08)
#define MODE_CCM_REG        (0x09)
#define MODE_GCM_REG        (0x0A)
#define MODE_GCTR_REG       (0x0B)
#define MODE_CMAC_REG       (0x0C)
#define MODE_CBC_NOOUT_REG  (0x0D)
#define MODE_GCTR_NOOUT_REG (0x0E)

#define BIT_WIDTH_1_REG   (2)
#define BIT_WIDTH_8_REG   (1)
#define BIT_WIDTH_64_REG  (3)
#define BIT_WIDTH_128_REG (3)

// internal iv state
#define SYMC_IV_STATE_START  (0)
#define SYMC_IV_STATE_UPDATE (1)
#define SYMC_IV_STATE_FINISH (2)

typedef union {
    struct {
        td_u32 reserved_0 : 8;
        td_u32 hash_chn_ss : 4;
        td_u32 reserved_1 : 19;
        td_u32 hash_chn_en : 1;
    } bits;
    td_u32 u32;
} in_hash_chn_ctrl;

typedef union {
    struct {
        td_u32 hash_key_chn_id : 4;
        td_u32 reserved_0 : 5;
        td_u32 hmac_vld : 1;
        td_u32 reserved_1 : 6;
        td_u32 hash_chn_alg_sel : 4;
        td_u32 hash_chn_alg_mode : 4;
        td_u32 reserved_2 : 8;
    } bits;
    td_u32 u32;
} in_hash_chn_key_ctrl;

// Define the union in_hash_chn_node_wr_point
typedef union {
    // Define the struct bits
    struct {
        td_u32 hash_chn_node_wr_point : 8; // [7..0]
        td_u32 reserved_0 : 24;            // [31..8]
    } bits;

    // Define an unsigned member
    td_u32 u32;
} in_hash_chn_node_wr_point;

// Define the union in_hash_chn_node_wr_point
typedef union {
    // Define the struct bits
    struct {
        td_u32 hash_chn_node_rd_point : 8; // [7..0]
        td_u32 reserved_0 : 24;            // [31..8]
    } bits;

    // Define an unsigned member
    td_u32 u32;
} in_hash_chn_node_rd_point;

// Define the union hash_chann_raw_int
typedef union {
    // Define the struct bits
    struct {
        td_u32 hash_chann_raw_int : 16; // [15..0]
        td_u32 reserved_0 : 16;         // [31..16]
    } bits;

    // Define an unsigned member
    td_u32 u32;
} hash_chann_raw_int;

// Define the union tee_hash_calc_ctrl_check_err
typedef union {
    // Define the struct bits
    struct {
        td_u32 reserved_0 : 1;
        td_u32 hash_check_ss_error : 1;
        td_u32 hash_check_alg_error : 1;
        td_u32 hash_check_alg_invld_error : 1;
        td_u32 reserved_1 : 2;
        td_u32 hash_check_sc_error : 1;
        td_u32 reserved_2 : 6;
        td_u32 hash_check_sm3_disable_error : 1;
        td_u32 hash_check_sha1_disable_error : 1;
        td_u32 reserved_3 : 2;
        td_u32 hash_tonly_error : 1;
        td_u32 hash_aidsp_error : 1;
        td_u32 reserved_4 : 13;
    } bits;

    // Define an unsigned member
    td_u32 u32;
} hash_calc_ctrl_check_err;

// Define the union tee_hash_calc_ctrl_check_err
typedef union {
    // Define the struct bits
    struct {
        td_u32 hash_error_code_clr : 1;
        td_u32 reserved_0 : 15;
        td_u32 error_hash_chan_id : 4;
        td_u32 reserved_1 : 12;
    } bits;

    // Define an unsigned member
    td_u32 u32;
} hash_calc_ctrl_check_err_status;

/* Define the union in_sym_out_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_dma_copy : 2; /* [1..0] */
        td_u32 reserved : 30;    /* [31..2] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_out_ctrl;

/* Define the union in_sym_chn_key_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_key_chn_id : 9;     /* [8..0] */
        td_u32 reserved : 7;           /* [15..9] */
        td_u32 sym_alg_sel : 4;        /* [19..16] */
        td_u32 sym_alg_mode : 4;       /* [23..20] */
        td_u32 sym_alg_key_len : 2;    /* [25..24] */
        td_u32 sym_alg_data_width : 2; /* [27..26] */
        td_u32 sym_alg_decrypt : 1;    /* [28] */
        td_u32 reserved_2 : 3;         /* [31..29] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn_key_ctrl;

/* Define the union in_sym_chn_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved : 8;       /* [7..0] */
        td_u32 sym_chn_ss : 4;     /* [8..11] */
        td_u32 sym_chn_ds : 4;     /* [12..15] */
        td_u32 reserved_2 : 15;    /* [30..16] */
        td_u32 sym_chn_en : 1;     /* [31] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn_ctrl;

/* Define the union in_sym_chn0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved : 31;     /* [30..0] */
        td_u32 sym_chn0_req : 1;  /* [31] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn0_ctrl;

/* Define the union in_sym_chn0_special_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn0_odd_even : 1; /* [0] */
        td_u32 reserved : 31;         /* [31..1] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn0_special_ctrl;

/* Define the union in_sym_chn0_iv_data_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn0_first_block : 1; /* [0] */
        td_u32 sym_chn0_last_block : 1;  /* [1] */
        td_u32 reserved : 14;            /* [15..2] */
        td_u32 sym_chn0_block_len : 5;   /* [20..16] */
        td_u32 reserved2 : 11;           /* [31..21] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn0_iv_data_ctrl;

/* Define the union out_sym_chan_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 out_sym_chan_raw_int : 16; /* [15..0] */
        td_u32 reserved_1 : 16;           /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} out_sym_chan_raw_int;

/* Define the union out_sym_chan_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chan_int_en : 16; /* [15..1] */
        u32 reserved_1 : 16;          /* [31..16] */
    } bits;
    /* Define an unsigned member */
    u32 u32;
} out_sym_chan_raw_int_en;

/* Define the union out_sym_chn_status */
typedef union {
    /* Define the struct bits */
    struct {
        u32 out_sym_chn_int_status : 16;     /* [15..1] */
        u32 reserved_1 : 16;                 /* [31..16] */
    } bits;
    /* Define an unsigned member */
    u32 u32;
} out_sym_chn_status;

/* Define the union out_sym_chn_node_wr_point */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_wr_point : 8; /* [7..0] */
        td_u32 reserved_1 : 24;               /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} out_sym_chn_node_wr_point;

/* Define the union out_sym_chn_node_wr_point */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_wr_point : 8; /* [7..0] */
        td_u32 reserved_1 : 24;           /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn_node_wr_point;

/* Define the union out_sym_chn_node_rd_point */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_rd_point : 8; /* [7..0] */
        td_u32 reserved_1 : 24;               /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} out_sym_chn_node_rd_point;

/* Define the union out_sym_chn_node_rd_point */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_rd_point : 8; /* [7..0] */
        td_u32 reserved_1 : 24;           /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn_node_rd_point;

/* Define the union in_sym_chn_node_length */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_length : 8; /* [7..0] */
        td_u32 reserved_1 : 24;         /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn_node_length;

/* Define the union out_sym_chn_node_length */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_length : 8; /* [7..0] */
        td_u32 reserved_1 : 24;             /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} out_sym_chn_node_length;

/* Define the union dbg_in_sym_chn_rd_addr_h */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 dbg_sym_chn_rd_addr_h : 4; /* [3..0] */
        td_u32 reserved : 28;             /* [31..4] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} dbg_in_sym_chn_rd_addr_h;

/* Define the union dbg_out_sym_chn_rd_addr_h */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 dbg_sym_chn_rd_addr_h : 4; /* [3..0] */
        td_u32 reserved : 28;             /* [31..4] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} dbg_out_sym_chn_rd_addr_h;

/* Define the union sym_calc_ctrl_check_err */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32    calc_ctrl_check_ds_error : 1;          /* [0]  */
        td_u32    calc_ctrl_check_ss_error : 1;          /* [1]  */
        td_u32    calc_ctrl_check_alg_error : 1;         /* [2]  */
        td_u32    calc_ctrl_check_alg_invld_error : 1;   /* [3]  */
        td_u32    calc_ctrl_check_dec_error : 1;         /* [4]  */
        td_u32    reserved0 : 1;                         /* [5]  */
        td_u32    calc_ctrl_check_sc_error : 1;          /* [6]  */
        td_u32    calc_ctrl_check_key_size_error : 1;    /* [7]  */
        td_u32    reserved : 8;                          /* [15..8]  */
        td_u32    calc_ctrl_big_key_info_diff_error : 1; /* [16]  */
        td_u32    calc_ctrl_tonly_error            : 1;  /* [17]  */
        td_u32    calc_ctrl_aidsp_error : 1;             /* [18]  */
        td_u32    reserved2 : 13;                        /* [31..19]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_calc_ctrl_check_err;

/* Define the union sym_calc_ctrl_check_err_status */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_error_code_clr : 1; /* [0] */
        td_u32 reserved : 15;              /* [15..1] */
        td_u32 error_sym_chan_id : 4;  /* [19..16] */
        td_u32 reserved1 : 12;             /* [31..20] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_calc_ctrl_check_err_status;

/* Define the union in_sym_chn_node_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_node_level : 8;     /* [7..0] */
        td_u32 reserved_0 : 8;             /* [15..8] */
        td_u32 sym_chn_node_cnt : 8;       /* [23..16] */
        td_u32 reserved_1 : 7;             /* [30..24] */
        td_u32 sym_chn_node_cnt_clear : 1; /* [31] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} in_sym_chn_node_ctrl;

/* Define the union out_sym_chn_node_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 out_sym_chn_node_level : 8; /* [7..0] */
        td_u32 reserved_0 : 24;            /* [31..8] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} out_sym_chn_node_ctrl;

/* Define the union spacc_sym_chn_clear_req */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chn_clear_req : 16; /* [15..0] */
        td_u32 reserved_0 : 16;        /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} spacc_sym_chn_clear_req;

/* Define the union spacc_int_raw_sym_clear_finish */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 int_raw_sym_clear_finish : 16; /* [15..0] */
        td_u32 reserved_0 : 16;               /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} spacc_int_raw_sym_clear_finish;

/* Define the union sym_chann_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chann_raw_int : 16; /* [0] */
        td_u32 reserved_0 : 16;        /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chann_raw_int;

/* Define the union sym_chann_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chann_int_en : 16; /* [0] */
        td_u32 reserved_0 : 16;       /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chann_raw_int_en;

/* Define the union sym_chann_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chann_int : 16; /* [0] */
        td_u32 reserved_0 : 16;    /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chann_int;

/* Define the union sym_chan0_finish_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chan0_finish_raw_int : 1; /* [0] */
        td_u32 reserved_0 : 31;              /* [31..1] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chan0_finish_raw_int;

/* Define the union sym_chan0_finish_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chan0_finish_int_en : 1; /* [0] */
        td_u32 reserved_0 : 31;             /* [31..1] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chan0_finish_raw_int_en;

/* Define the union sym_chan0_finish_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chan0_finish_int : 1; /* [0] */
        td_u32 reserved_0 : 31;          /* [31..1] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chan0_finish_int;

/* Define the union sym_chann_error_raw_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chan_raw_err_int : 16; /* [0] */
        td_u32 reserved_0 : 16;           /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chann_error_raw_int;

/* Define the union sym_chann_error_raw_int_en */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chan_err_int_en : 16; /* [0] */
        td_u32 reserved_0 : 16;          /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chann_error_raw_int_en;

typedef union {
    struct {
        td_u32 chann_dfa_en  : 4;  /* [3..0]  */
        td_u32 reserved      : 28; /* [31..4] */
    } bits;
    td_u32 u32;
} symc_chann_dfa_en;

/* Define the union sym_chann_error_int */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 sym_chan_err_int : 16; /* [0] */
        td_u32 reserved_0 : 16;       /* [31..16] */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} sym_chann_error_int;

typedef union {
    struct {
        td_u32 sec_dfa_en  : 4;  /* [3..0]  */
        td_u32 reserved    : 28; /* [31..4] */
    } bits;
    td_u32 u32;
} spacc_sec_dfa_en;

/* Define the union spacc_ie */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int spacc_ie_ree : 1; /* [0] */
        unsigned int reserved_0 : 3;   /* [3..1] */
        unsigned int spacc_ie_tee : 1; /* [4] */
        unsigned int reserved_1 : 3;   /* [7..5] */
        unsigned int spacc_ie_hpp : 1; /* [8] */
        unsigned int reserved_2 : 23;  /* [31..9] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} spacc_ie;

#endif