/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_ACODEC_MOD_INIT_H
#define OT_ACODEC_MOD_INIT_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int acodec_init(void);
void acodec_exit(void);

td_void acodec_set_init_mode(td_u32 init_mode);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_ACODEC_MOD_INIT_H */
