/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_KEYSLOT_REG_H
#define HAL_KEYSLOT_REG_H

#define KS_CPU_LOCK   1
#define KS_CPU_UNLOCK 0

#define KCTRL_REG_OFFSET          (0x00001000)

#define KC_TEECPU_LOCK_CMD      (KCTRL_REG_OFFSET + 0xB00)
#define KC_REECPU_LOCK_CMD      (KCTRL_REG_OFFSET + 0xB04)
#define KC_PCPU_LOCK_CMD        (KCTRL_REG_OFFSET + 0xB08)
#define KC_AIDSP_LOCK_CMD       (KCTRL_REG_OFFSET + 0xB0c)
#define KC_TEECPU_FLUSH_BUSY    (KCTRL_REG_OFFSET + 0xB10)
#define KC_REECPU_FLUSH_BUSY    (KCTRL_REG_OFFSET + 0xB14)
#define KC_PCPU_FLUSH_BUSY      (KCTRL_REG_OFFSET + 0xB18)
#define KC_AIDSP_FLUSH_BUSY     (KCTRL_REG_OFFSET + 0xB1c)
#define KC_RD_SLOT_NUM          (KCTRL_REG_OFFSET + 0xB30)
#define KC_RD_LOCK_STATUS       (KCTRL_REG_OFFSET + 0xB34)

typedef enum {
    KS_STAT_UN_LOCK     = 0x0,
    KS_STAT_REE_LOCK    = 0x1,
    KS_STAT_TEE_LOCK    = 0x2,
    KS_STAT_PCPU_LOCK   = 0x4,
    KS_STAT_AIDSP_LOCK  = 0x6,
    KS_STAT_MAX
} ks_slot_stat;

/* define the union u_kc_cpu_lock_cmd */
typedef union {
    struct {
        unsigned int    key_slot_num                : 10; /* [9..0]  */
        unsigned int    reserved_0                  : 5; /* [14..10]  */
        unsigned int    flush_hmac_kslot_ind        : 1; /* [15]  */
        unsigned int    tscipher_ind                : 1; /* [16] */
        unsigned int    reserved_1                  : 3; /* [19..17]  */
        unsigned int    lock_cmd                    : 1; /* [20]  */
        unsigned int    reserved_2                    : 11; /* [31..21]  */
    } bits;
    unsigned int    u32;
} kc_cpu_lock_cmd;

/* define the union u_kc_teecpu_lock_cmd */
typedef union {
    struct {
        unsigned int    tee_key_slot_num              : 10; /* [9..0]  */
        unsigned int    reserved_0                    : 5; /* [14..10]  */
        unsigned int    tee_flush_hmac_kslot_ind      : 1; /* [15]  */
        unsigned int    tee_tscipher_ind              : 1; /* [16] */
        unsigned int    reserved_1                    : 3; /* [19..17]  */
        unsigned int    tee_lock_cmd                  : 1; /* [20]  */
        unsigned int    reserved_2                    : 11; /* [31..21]  */
    } bits;
    unsigned int    u32;
} kc_tee_lock_cmd;

/* define the union u_kc_reecpu_lock_cmd */
typedef union {
    struct {
        unsigned int    ree_key_slot_num              : 10; /* [9..0]  */
        unsigned int    reserved_0                    : 5; /* [14..10]  */
        unsigned int    ree_flush_hmac_kslot_ind      : 1; /* [15]  */
        unsigned int    ree_tscipher_ind              : 1; /* [16] */
        unsigned int    reserved_1                    : 3; /* [19..17]  */
        unsigned int    ree_lock_cmd                  : 1; /* [20]  */
        unsigned int    reserved_2                    : 11; /* [31..21]  */
    } bits;
    unsigned int    u32;
} kc_ree_lock_cmd;

/* define the union u_kc_pcpu_lock_cmd */
typedef union {
    struct {
        unsigned int    pcpu_key_slot_num              : 10; /* [9..0]  */
        unsigned int    reserved_0                     : 5; /* [14..10]  */
        unsigned int    pcpu_flush_hmac_kslot_ind      : 1; /* [15]  */
        unsigned int    pcpu_tscipher_ind              : 1; /* [16] */
        unsigned int    reserved_1                     : 3; /* [19..17]  */
        unsigned int    pcpu_lock_cmd                  : 1; /* [20]  */
        unsigned int    reserved_2                     : 11; /* [31..21]  */
    } bits;
    unsigned int    u32;
} kc_pcpu_lock_cmd;

/* define the union u_kc_aidsp_lock_cmd */
typedef union {
    struct {
        unsigned int    aidsp_key_slot_num              : 10; /* [9..0]  */
        unsigned int    reserved_0                      : 5; /* [14..10]  */
        unsigned int    aidsp_flush_hmac_kslot_ind      : 1; /* [15]  */
        unsigned int    aidsp_tscipher_ind              : 1; /* [16] */
        unsigned int    reserved_1                      : 3; /* [19..17]  */
        unsigned int    aidsp_lock_cmd                  : 1; /* [20]  */
        unsigned int    reserved_2                      : 11; /* [31..21]  */
    } bits;
    unsigned int    u32;
} kc_aidsp_lock_cmd;

/* define the union kc_flush_busy */
typedef union {
    struct {
        unsigned int    flush_busy        : 1; /* [0]  */
        unsigned int    flush_unlock_fail : 1; /* [1]  */
        unsigned int    flush_timeout_err : 1; /* [2]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;
    unsigned int    u32;
} kc_flush_busy;

/* define the union kc_send_dbg */
typedef union {
    struct {
        unsigned int    main_fsm_cur          : 8; /* [7..0]  */
        unsigned int    reserved_1            : 24; /* [31..8]  */
    } bits;
    unsigned int    u32;
} kc_send_dbg;

/* define the union kc_rob_alarm */
typedef union {
    struct {
        unsigned int    kc_rob_alarm          : 4; /* [3..0]  */
        unsigned int    reserved_0            : 28; /* [31..4]  */
    } bits;
    unsigned int    u32;
} kc_rob_alarm;

/* define the union kc_rd_slot_num */
typedef union {
    struct {
        unsigned int    slot_num_cfg          : 10; /* [9..0]  */
        unsigned int    reserved_0            : 5; /* [14..10]  */
        unsigned int    slot_cfg_type         : 1; /* [15]  */
        unsigned int    tscipher_slot_ind     : 1; /* [16]  */
        unsigned int    reserved_1            : 15; /* [31..17]  */
    } bits;
    unsigned int    u32;
} kc_rd_slot_num;

/* define the union kc_rd_lock_status */
typedef union {
    struct {
        unsigned int    rd_lock_status        : 3;  /* [2..0]  */
        unsigned int    reserved_0            : 29; /* [31..3]  */
    } bits;
    unsigned int    u32;
} kc_rd_lock_status;

#endif