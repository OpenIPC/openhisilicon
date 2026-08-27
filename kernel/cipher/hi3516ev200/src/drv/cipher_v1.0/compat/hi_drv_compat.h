#ifndef _HI_DRV_CPMPAT_H_
#define _HI_DRV_CPMPAT_H_

hi_s32 hi_drv_compat_init(hi_void);
hi_s32 hi_drv_compat_deinit(hi_void);
hi_s32 klad_load_hard_key(hi_u32 handle, hi_u32 ca_type, hi_u8 *key, hi_u32 key_len);
hi_s32 klad_encrypt_key(hi_u32 keysel, hi_u32 target, hi_u32 clear[4], hi_u32 encrypt[4]);

#endif

