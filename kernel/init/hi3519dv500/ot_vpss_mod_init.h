/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef VPSS_MOD_INIT_H
#define VPSS_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_s32 vpss_set_high_profile(td_bool high_profile);

td_s32 vpss_set_init_vpss_reg(td_void *vpss_reg, td_u32 ip_index);
td_s32 vpss_set_init_vpss_irq(td_s32 vpss_irq, td_u32 ip_index);

int vpss_module_init(td_void);
td_void vpss_module_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef VPSS_MOD_INIT_H */
