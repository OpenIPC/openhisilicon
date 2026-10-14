/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef DRV_LOG_H
#define DRV_LOG_H

#include "xmedia_type.h"

#ifdef __cplusplus
extern "C" {
#endif

xmedia_s32 drv_log_mod_init(xmedia_u32 buf_len);
xmedia_void drv_log_mod_exit(xmedia_void);
xmedia_s32 drv_log_write(xmedia_s32 level, xmedia_u32 mod_id, const xmedia_char *fmt, ...) __attribute__((format(printf,3,4)));
xmedia_s32 drv_log_check_level(xmedia_s32 level, xmedia_u32 mod_id);
xmedia_char *drv_get_module_name(xmedia_u32 mod_id);

#ifdef __cplusplus
}
#endif

#endif
