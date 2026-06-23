/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_PKE_REG_H
#define HAL_PKE_REG_H

#define PKE_INT_NOMASK_ALARM                 0x5
#define PKE_INT_NOMASK_CLR                   0x6b4a89c6
#define PKE_LEN_BLOCK_MASK                   0x03

#define PKE_KEY_RANDOM                  (0x24)

#define PKE_BUSY                        0x4
#define PKE_WORK_MODE                   0x8
#define PKE_START                       0xc
#define PKE_INT_ENABLE                  0x10
#define PKE_INT_STATUS                  0x14
#define PKE_INT_NOMASK_STATUS           0x18
#define PKE_NOISE_EN                    (0x1c)
#define PKE_LOCK_INT_ENABLE             (0x1c00)
#define PKE_INT_RAW                     (0x1c04)
#define PKE_LOCK_CTRL                   (0x1C10)
#define PKE_LOCK_STATUS                 (0x1c14)
#define PKE_LOCK_UNLOCK                  0x00

#define PKE_FAILURE_FLAG                (0x28)
#define PKE_START_CODE                   0x05

#define SET_KEY_LEN                     0x48
#define RAM_SECTION_LEN                 0x44

#define LOCK_STATUS_SCPU                0xa5
#define LOCK_STATUS_ACPU                0xaa
// same with v900
typedef union {
    struct {
        unsigned int    pke_lock              : 1; /* [0]  */
        unsigned int    pke_lock_type         : 1; /* [1]  */
        unsigned int    reserved_0            : 30; /* [31..2]  */
    } bits;
    unsigned int    u32;
} pke_lock_ctrl;

typedef union {
    struct {
        unsigned int    pke_lock_busy         : 1; /* [0]  */
        unsigned int    pke_unlock_fail       : 1; /* [1]  */
        unsigned int    reserved_0            : 2; /* [3..2]  */
        unsigned int    pke_lock_cnt          : 3; /* [6..4]  */
        unsigned int    reserved_1            : 1; /* [7]  */
        unsigned int    pke_lock_stat         : 8; /* [15..8]  */
        unsigned int    reserved_2            : 16; /* [31..16]  */
    } bits;
    unsigned int    u32;
} pke_lock_status;

typedef union {
    struct {
        unsigned int pke_start : 4;   /* [3..0] */
    }bits;
    unsigned int u32;
} pke_start;

// difference with v900

typedef enum {
    HAL_PKE_LEN_256 = 32,
    HAL_PKE_LEN_384 = 48,
    HAL_PKE_LEN_512 = 64,
    HAL_PKE_LEN_521 = 68, // deference with v900
    HAL_PKE_LEN_2048 = 256,
    HAL_PKE_LEN_3072 = 384,
    HAL_PKE_LEN_4096 = 512,
} hal_pke_len;

/*! \Define the operation mode */
typedef enum {
    PKE_MODE_CLR_RAM        = 0x0,
    PKE_MODE_EXP_MOD        = 0x1,
    PKE_MODE_MUL_DOT        = 0x2,
    PKE_MODE_ADD_DOT        = 0x3,
    PKE_MODE_TIMES_DOT      = 0x4,
    PKE_MODE_MINV_MOD       = 0x5,
    PKE_MODE_SUB_MOD        = 0x6,
    PKE_MODE_MUL_MOD        = 0x7,
    PKE_MODE_ADD_MOD        = 0x8,
    PKE_MODE_MOD            = 0x9,
    PKE_MODE_MUL            = 0xa,
    PKE_MODE_MG_MUL_DOT     = 0xb,
    PKE_MODE_ED_MUL_DOT     = 0xc,
    PKE_MODE_ED_ADD_DOT     = 0xd,
    PKE_MODE_UNSUPPORTTED   = 0xff
} pke_mode;

typedef union {
    struct {
        unsigned int power_en : 4;    /* [3..0] */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;
    unsigned int u32;
} pke_power_on;

typedef union {
    struct {
        unsigned int finish_int : 4;  /* [3..0] */
        unsigned int alarm_int : 4;   /* [7..4] */
        unsigned int reserved_0 : 24; /* [31..8] */
    } bits;
    unsigned int u32;
} pke_int_status;

typedef union {
    struct {
        unsigned int finish_int_nomask : 4; /* [3..0] */
        unsigned int alarm_int_nomask : 4;  /* [7..4] */
        unsigned int reserved_0 : 24;       /* [31..8] */
    } bits;
    unsigned int u32;
} pke_int_nomask_status;

typedef union {
    struct {
        unsigned int finish_int_enable : 1; /* [0] */
        unsigned int reserved : 31;       /* [31..1] */
    } bits;
    unsigned int u32;
} pke_int_enable;

typedef union {
    struct {
        unsigned int pke_lock_int_en : 1;   /* [0] */
        unsigned int reserved : 31;         /* [31..1] */
    } bits;
    unsigned int u32;
} pke_lock_int_enable;

typedef union {
    struct {
        unsigned int pke_busy : 1;    /* [0] */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;
    unsigned int u32;
} pke_busy;

typedef union {
    struct {
        unsigned int pke_result_flag : 4; /* [3..0] */
        unsigned int reserved_0 : 28;     /* [31..4] */
    } bits;
    unsigned int u32;
} pke_result_flag;

typedef union {
    struct {
        unsigned int otp_key_sel : 4; /* [3..0] */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;
    unsigned int u32;
} pke_otp_key_sel;

typedef enum {
    PKE_RESULT_FLAG_IDLE = 0x00,
    PKE_RESULT_FLAG_OK = 0x05,
    PKE_RESULT_FLAG_FAIL = 0x0A,
} pke_result_code;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    mode_sel              : 4; /* [3..0]  */
        unsigned int    data_len              : 3; /* [6..4]  */
        unsigned int    reserved_0            : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_work_mode;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    finish_int_nomask       : 4; /* [3..0]  */
        unsigned int    alarm_int_nomask        : 4; /* [7..4]  */
        unsigned int    reserved_0              : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_robust_warn;

/* Define the union U_PKE_FAILURE_FLAG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    invalid_k               : 1; /* [0]  */
        unsigned int    invalid_len_cfg         : 1; /* [1]  */
        unsigned int    invalid_mode            : 1; /* [2]  */
        unsigned int    invalid_p               : 1; /* [3]  */
        unsigned int    invalid_n               : 1; /* [4]  */
        unsigned int    invalid_crc             : 1; /* [5]  */
        unsigned int    invalid_data_len        : 1; /* [6]  */
        unsigned int    infinite_dot            : 1; /* [7]  */
        unsigned int    in_dot_not_on_curve     : 1; /* [8]  */
        unsigned int    out_dot_not_on_curve    : 1; /* [9]  */
        unsigned int    no_result_in_inv_mod    : 1; /* [10]  */
        unsigned int    invalid_n_in_inv_mod    : 1; /* [11]  */
        unsigned int    invalid_pm_in_mg        : 1; /* [12]  */
        unsigned int    invalid_x2_in_mg        : 1; /* [13]  */
        unsigned int    invalid_hpi_cfg         : 1; /* [14]  */
        unsigned int    reserved_0              : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_failure_flag;

/* Define the union U_PKE_POWER_NOISE_LOCK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    noise_en              : 1; /* [0]  */
        unsigned int    reserved_0            : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} pke_noise_en;

typedef union {
    // Define the struct bits
    struct {
        td_u32 pke_lock_int_raw : 1;
        td_u32 pke_int_raw : 1;
        td_u32 reserved : 30;
    } bits;

    // Define an unsigned member
    td_u32 u32;
} pke_lock_int_dat;

typedef union {
    // Define the struct bits
    struct {
        td_u32 finish_int_nomask : 4;
        td_u32 alarm_int_nomask : 4;
        td_u32 reserved : 24;
    } bits;

    // Define an unsigned member
    td_u32 u32;
} pke_int_dat;

#endif