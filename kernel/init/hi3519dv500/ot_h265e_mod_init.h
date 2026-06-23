/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_H265E_MOD_INIT_H
#define OT_H265E_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int h265e_module_init(void);
void h265e_module_exit(void);
td_void h265e_set_feature_en_mod(td_u32 val);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_H265E_MOD_INIT_H */
