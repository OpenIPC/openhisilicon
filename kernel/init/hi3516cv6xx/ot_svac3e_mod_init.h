/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_SVAC3E_MOD_INIT_H
#define OT_SVAC3E_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int svac3e_module_init(void);
void svac3e_module_exit(void);
td_void svac3e_set_feature_en_mod(td_u32 val);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_SVAC3E_MOD_INIT_H */
