/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_SYS_MOD_INIT_H
#define OT_SYS_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_void sys_set_reg_by_dts(td_bool is_reg_by_dts);

td_void sys_set_crg_base_addr(td_void *crg_base_addr);

td_void sys_set_sys_base_addr(td_void *sys_base_addr);

td_void sys_set_ddr0_base_addr(td_void *ddr0_base_addr);

td_void sys_set_misc_base_addr(td_void *misc_base_addr);

td_s32 sys_do_mod_init(td_void);

td_void sys_do_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_SYS_MOD_INIT_H */
