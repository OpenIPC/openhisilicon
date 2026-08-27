#ifndef __DRV_KLAD_H_
#define __DRV_KLAD_H_

#include "hi_types.h"
#include "drv_osal_lib.h"

hi_s32 hal_cipher_klad_config(hi_u32 chn_id,
                             hi_u32 opt_id,
                             hi_cipher_klad_target target,
                             hi_bool is_decrypt);

hi_void hal_cipher_start_klad(hi_u32 block_num);
hi_void hal_cipher_set_klad_data(hi_u32 *data_in);
hi_void hal_cipher_get_klad_data(hi_u32 *data_out);
hi_s32 hal_cipher_wait_klad_done(hi_void);

hi_s32 drv_cipher_klad_init(hi_void);
hi_void drv_cipher_klad_deinit(hi_void);

hi_s32 drv_cipher_klad_load_key(hi_u32 chn_id,
                              hi_cipher_ca_type root_key,
                              hi_cipher_klad_target target,
                              hi_u8 *data_in,
                              hi_u32 key_len);

hi_s32 drv_cipher_klad_encrypt_key(hi_cipher_ca_type root_key,
                                 hi_cipher_klad_target target,
                                 hi_u32 clean_key[4],
                                 hi_u32 encrypt_key[4]);

#endif