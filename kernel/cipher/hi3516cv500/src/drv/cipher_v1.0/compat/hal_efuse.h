/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for hal efuse.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef _HAL_EFUSE_H_
#define _HAL_EFUSE_H_

#include "hi_types.h"

#define hal_cipher_read_reg(addr,  result)    (*(result) = *(volatile unsigned int *)(hi_uintptr_t)(addr))
#define hal_cipher_write_reg(addr, result)    (*(volatile unsigned int *)(hi_uintptr_t)(addr) = (result))

#define hal_set_bit(src, bit)               ((src) |= (1 << (bit)))
#define hal_clear_bit(src, bit)             ((src) &= ~(1 << (bit)))

hi_s32 hal_efuse_write_key(hi_u32 *p_key, hi_u32 opt_id);
hi_s32 hal_efuse_otp_load_cipher_key(hi_u32 chn_id, hi_u32 opt_id);
hi_s32 hal_efuse_load_hash_key(hi_u32 opt_id);
hi_s32 hal_efuse_otp_init(hi_void);
hi_void hal_efuse_otp_set_reg_base(hi_u8 *reg_base);
hi_u8 *hal_efuse_otp_get_reg_base(hi_void);

#endif

