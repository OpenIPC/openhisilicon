#ifndef __HAL_OTP_H__
#define __HAL_OTP_H__

#include "hi_types.h"
#include "hal_efuse.h"

#define OTP_USER_IF_BASE            efuse_otp_reg_base
#define OTP_USER_WORK_MODE          (OTP_USER_IF_BASE+0x0000)
#define OTP_USER_OP_START           (OTP_USER_IF_BASE+0x0004)
#define OTP_USER_KEY_INDEX          (OTP_USER_IF_BASE+0x0008)
#define OTP_USER_KEY_DATA0          (OTP_USER_IF_BASE+0x000c)
#define OTP_USER_KEY_DATA1          (OTP_USER_IF_BASE+0x0010)
#define OTP_USER_KEY_DATA2          (OTP_USER_IF_BASE+0x0014)
#define OTP_USER_KEY_DATA3          (OTP_USER_IF_BASE+0x0018)
#define OTP_USER_KEY_DATA4          (OTP_USER_IF_BASE+0x001c)
#define OTP_USER_KEY_DATA5          (OTP_USER_IF_BASE+0x0020)
#define OTP_USER_KEY_DATA6          (OTP_USER_IF_BASE+0x0024)
#define OTP_USER_KEY_DATA7          (OTP_USER_IF_BASE+0x0028)
#define OTP_USER_KEY_DATA8          (OTP_USER_IF_BASE+0x002c)
#define OTP_USER_FLAG_VALUE         (OTP_USER_IF_BASE+0x0030)
#define OTP_USER_FLAG_INDEX         (OTP_USER_IF_BASE+0x0034)
#define OTP_USER_REV_ADDR           (OTP_USER_IF_BASE+0x0038)
#define OTP_USER_REV_WDATA          (OTP_USER_IF_BASE+0x003c)
#define OTP_USER_REV_RDATA          (OTP_USER_IF_BASE+0x0040)
#define OTP_USER_LOCK_STA0          (OTP_USER_IF_BASE+0x0044)
#define OTP_USER_LOCK_STA1          (OTP_USER_IF_BASE+0x0048)
#define OTP_USER_CTRL_STA           (OTP_USER_IF_BASE+0x004c)

typedef enum {
    OTP_USER_LOCK_STA0_TYPE,
    OTP_USER_LOCK_STA1_TYPE,
    OTP_USER_LOCK_UNKNOWN_STA,
} otp_lock_sta_type;

typedef enum {
    OTP_READ_LOCK_STA_MODE,
    OTP_LOCK_CIPHER_KEY_MODE,
    OTP_WRITE_KEY_ID_OR_PASSWD_MODE,
    OTP_KEY_ID_OR_PASSWD_CRC_MODE,
    OTP_SET_FLAG_ENABLE_MODE,
    OTP_WRITE_USER_ROOM_MODE,
    OTP_Read_USER_ROOM_MODE,
    OTP_UNKOOWN_MODE,
} otp_user_work_mode;

typedef enum {
    OTP_USER_KEY0,
    OTP_USER_KEY1,
    OTP_USER_KEY2,
    OTP_USER_KEY3,
    OTP_USER_KEY_JTAG_PW_ID,
    OTP_USER_KEY_JTAG_PW,
    OTP_USER_KEY_ROOTKEY,
    OTP_USER_KEY_UNKNOWN,
} otp_user_key_index;

typedef enum {
    OTP_KEY_LENGTH_64BIT,
    OTP_KEY_LENGTH_128BIT,
    OTP_KEY_LENGTH_256BIT,
    OTP_KEY_LENGTH_UNSUPPORT,
} otp_user_key_length;

hi_s32 hal_efuse_otp_init(hi_void);
hi_s32 hal_efuse_otp_load_cipher_key(hi_u32 chn_id, hi_u32 opt_id);
unsigned short calculate_crc16 (const unsigned char *data_array_ptr, unsigned long data_array_length);

#endif
