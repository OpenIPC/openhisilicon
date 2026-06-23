/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef VGS_MOD_INIT_H
#define VGS_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_s32 vgs_set_init_vgs_reg(td_void *vgs_reg, td_u32 ip_index);
td_s32 vgs_set_init_vgs_irq(td_s32 vgs_irq, td_u32 ip_index);

td_void vgs_set_max_job_num(td_u32 num);
td_void vgs_set_max_task_num(td_u32 num);
td_void vgs_set_max_node_num(td_u32 num);

td_s32 vgs_module_init(td_u32 *max_vgs_job, td_u32 *max_vgs_task, td_u32 *max_vgs_node);
td_void vgs_module_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef VGS_MOD_INIT_H */
