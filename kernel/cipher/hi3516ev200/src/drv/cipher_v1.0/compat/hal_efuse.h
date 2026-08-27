#ifndef _HAL_EFUSE_H_
#define _HAL_EFUSE_H_

#include "hi_types.h"

#define HAL_CIPHER_READ_REG(addr, result)    (*(result) = *(volatile unsigned int *)(hi_uintptr_t)(addr))
#define HAL_CIPHER_WRITE_REG(addr,result)    (*(volatile unsigned int *)(hi_uintptr_t)(addr) = (result))

#define HAL_SET_BIT(src, bit)               ((src) |= (1<<bit))
#define HAL_CLEAR_BIT(src,bit)              ((src) &= ~(1<<bit))

hi_s32 hal_efuse_write_key(hi_u32 *p_key, hi_u32 opt_id);
hi_s32 hal_efuse_otp_load_cipher_key(hi_u32 chn_id, hi_u32 opt_id);
hi_s32 hal_efuse_load_hash_key(hi_u32 opt_id);
hi_s32 hal_efuse_otp_init(hi_void);

#endif

