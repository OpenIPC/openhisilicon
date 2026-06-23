/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_VO_MOD_INIT_H
#define OT_VO_MOD_INIT_H

#include "ot_defines.h"
#include "ot_type.h"
#include "ot_osal.h"
#include "ot_vo_export.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

td_void vo_set_init_vo_reg(void *reg);
td_void vo_set_init_vo_irq(unsigned int vo_irq);
int vo_mod_init(void);
void vo_mod_exit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_VO_MOD_INIT_H */
