/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_JPEGD_MOD_INIT_H
#define OT_JPEGD_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int jpegd_module_init(void);
void jpegd_module_exit(void);
td_void jpegd_set_reg_addr(td_void **addr, td_u32 len);
td_void jpegd_set_jpegd_irq(td_u32 *irq, td_u32 len);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_JPEGD_MOD_INIT_H */
