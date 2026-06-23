/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_TDE_MOD_INIT_H
#define OT_TDE_MOD_INIT_H

#include "ot_type.h"
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/poll.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_void set_tde_irq(unsigned int temp_tde_irq);
td_s32 tde_drv_mod_init(td_void);
td_void tde_drv_mod_exit(td_void);
td_void tde_hal_set_base_vir_addr(td_u32 *temp_base_vir_addr);
td_void tde_set_max_node_num(td_u32 node_num);
td_void tde_set_resize_filter(td_bool is_resize_filter);
td_void tde_set_tde_tmp_buffer(td_u32 tde_tmp_buf);
td_void tde_init_set_rgb_truncation_mode(td_u32 rgb_truncation_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OT_TDE_MOD_INIT_H */

