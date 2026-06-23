/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OTPC_REGISTER_H
#define OTPC_REGISTER_H

#include "ot_type.h"

#define OTP_RW_CTRL         (0x1000)

// [9:0] -- otp_addr , OTP read only support word operation, so addr[1:0] will be ignored.
#define OTP_ADDR            (0x1004)

#define OTP_WDATA           (0x1008)
#define OTP_RDATA           (0x100C)
#define OTP_STATUS          (0x1010)
#define OTP_UPDATE          (0x1014)
#define OTP_LOCK            (0x1038)
#define OTP_LOCK_STATUS     (0x103C)
#define OTP_AUTO_SB_CTRL    (0x1050)

#define OTP_LOCK_UNLOCK     0x0
#define OTP_LOCK_TIMEOUT    1000000

#define OTP_LOCK_CMD                1
#define OTP_UNLOCK_CMD              0
#define OTP_LOCK_TYPE_LOCK          0
#define OTP_LOCK_TYPE_UNLOCK        1

#define OTP_AUTO_STANDBY_ENABLE     1
#define OTP_AUTO_STANDBY_DISABLE    0

#define OTP_TEST_MODE_NORMAL            0x0
#define OTP_WR_SEL_TSMC_EFUSE_NORMAL_RD 0x4

#define OTP_WR_SEL_OTP_STANDBY      0x0
#define OTP_WR_SEL_OTP_RD           0x1
#define OTP_WR_SEL_PGM_ACCESS       0x2

#define OTP_START_BUSY 0x1
#define OTP_START_IDLE 0x0

#define OTP_CHECK_PASS 0x0
#define OTP_CHECK_FAIL 0x1

#define OTP_UPDATE_REQ_BUSY 0x1
#define OTP_UPDATE_REQ_IDLE 0x0

/* ! Define the offset of OTPC Shadow reg */
#define SOC_TEE_ENABLE_REG                         (0x0010)

#define TEE_NOT_ENABLE                             0x42
/* Define the union U_OTP_BYTE_ALIGNED_LOCKABLE_0 */
typedef union {
    struct {
        td_u32 otp_not_blank               : 16; /* [15:0] */
        td_u32 soc_tee_enable              : 8;  /* [23:16] */
        td_u32 obfu_mrk1_owner_id_low      : 8;  /* [31:24] */
    } bits;
    td_u32 u32;
} otp_byte_aligned_lockable_0; /* 0x10 */

/*
1.otp_start: [0]
    OTP operation start.
    1'b0: idle;
    1'b1: busy.
    Software sets 1'b1 to start operation, logic clears to 1'b0 after operation is done.
2.otp_wr_sel: [3:1]
    OTP normal mode control.
    Ememory:
    3'b000: standby;
    3'b001: read access;
    3'b010: pgm access;
    3'b011: pgm verify; (recommend)
    3'b100: pgm margin read;
    3'b101: initial margin read single;
    others: standby.
3.otp_test_mode: [6:4]
    OTP test mode control. Only used for SOC JTAG in blank chipset.
    Ememory:
    3'b000: normal mode;
    3'b001: initial margin read;
    3'b010: repair margin read;
    3'b011: repair check mode;
    3'b100: repair pgm mode;
    3'b101: puf auto enrollment;
    3'b110: puf check mode;
    others: normal mode.
*/
typedef union {
    struct {
        td_u32 start                   : 1;  // [0]
        td_u32 wr_sel                  : 3;  // [3:1]
        td_u32 test_mode               : 3;  // [4:6]
        td_u32 reserved                : 25; // [31:7]
    } bits;
    td_u32 u32;
} otp_rw_ctrl_u;

typedef union {
    struct {
        td_u32 init_rdy                : 1;  // [0]
        td_u32 check_fail              : 1;  // [1] 1'b0: pass; 1'b1: fail.
        td_u32 check_alarm             : 1;  // [2]
        td_u32 reserved                : 29; // [31:3]
    } bits;
    td_u32 u32;
} otp_status;

typedef union {
    struct {
        td_u32 req                     : 1;  // [0] 0: idle; 1: busy
        td_u32 reserved                : 31; // [31:1]
    } bits;
    td_u32 u32;
} otp_update;

typedef union {
    struct {
        td_u32 otp_lock                : 1;  // [0]
        td_u32 otp_lock_type           : 1;  // [1] 1'b0: lock command; 1'b1: unlock command.
        td_u32 reserved                : 30; // [31:2]
    } bits;
    td_u32 u32;
} otp_lock;

typedef union {
    struct {
        td_u32 otp_auto_sb_en          : 1;  // [0] 1'b0: disable; 1'b1: enable.
        td_u32 reserved                : 31; // [31:1]
    } bits;
    td_u32 u32;
} otp_auto_sb_ctrl;

#endif