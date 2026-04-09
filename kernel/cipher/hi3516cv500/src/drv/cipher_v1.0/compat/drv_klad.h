/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv klad.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef _DRV_KLAD_H_
#define _DRV_KLAD_H_

#include "hi_cipher_compat.h"

/* Define the union klad_ctrl */
typedef union {
    struct {
        hi_u32    start                 : 1   ; /* [0]  */
        hi_u32    decrypt               : 1   ; /* [1]  */
        hi_u32    type                  : 2   ; /* [3..2]  */
        hi_u32    high_low_128bit_flag  : 1   ; /* [4]  */
        hi_u32    resv1                 : 11  ; /* [15..5]  */
        hi_u32    klad2ci_addr          : 3   ; /* [18..16]  */
        hi_u32    resv2                 : 13  ; /* [31..19]  */
    } bits;

    hi_u32    u32;
} klad_ctrl;

#define klad_data_in(id)                (0x10 + (id) * WORD_WIDTH)
#define klad_data_out(id)               (0x20 + (id) * WORD_WIDTH)

#define KLAD_CTRL_NOT_START             0
#define KLAD_CTRL_START                 1

#define KLAD_HIGN_128BIT_BLOCK          1
#define KLAD_CTRL_LOW_128BIT_FLAG       0
#define KLAD_CTRL_HIGH_128BIT_FLAG      1

hi_s32 hal_cipher_klad_config(hi_u32 chn_id, hi_u32 opt_id, hi_cipher_klad_target target, hi_bool is_decrypt);
hi_void hal_cipher_start_klad(hi_u32 block_num);
hi_void hal_cipher_set_klad_data(hi_u32 *data_in, hi_u32 data_len_in_word);
hi_void hal_cipher_get_klad_data(hi_u32 *data_out, hi_u32 data_len_in_word);
hi_s32 hal_cipher_wait_klad_done(hi_void);

hi_s32 drv_cipher_klad_init(hi_void);
hi_void drv_cipher_klad_deinit(hi_void);

hi_s32 drv_cipher_klad_load_key(hi_u32 chn_id,  hi_cipher_ca_type root_key, hi_cipher_klad_target target,
    hi_u8 *data_in, hi_u32 key_len);

hi_s32 drv_cipher_klad_encrypt_key(hi_cipher_ca_type root_key, hi_cipher_klad_target target,
    hi_u8 *clean_key, hi_u8 *encrypt_key, hi_u32 key_len);

#endif
