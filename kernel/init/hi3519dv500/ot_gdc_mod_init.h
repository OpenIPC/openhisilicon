/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_GDC_MOD_INIT_H
#define OT_GDC_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

td_s32 gdc_set_init_gdc_reg(td_void *gdc_reg, td_u32 ip_index);
td_s32 gdc_set_init_gdc_irq(td_s32 gdc_irq, td_u32 ip_index);

td_void gdc_set_max_job_num(td_u32 num);
td_void gdc_set_max_task_num(td_u32 num);
td_void gdc_set_max_node_num(td_u32 num);

td_s32 gdc_module_init(td_void);
td_void gdc_module_exit(td_void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_GDC_MOD_INIT_H */

