/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_VI_MOD_INIT_H
#define OT_VI_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_void vi_get_vicap_ip_info(td_u32 *irq_num, td_u32 *ip_num);
td_void vi_set_vicap_base_addr(td_u32 vicap_id, const td_void *base_addr);
td_void vi_set_vicap_irq_num(td_u32 vicap_id, td_u32 irq_index, td_u32 irq_num);

td_void vi_get_viproc_ip_info(td_u32 *irq_num, td_u32 *ip_num);
td_void vi_set_viproc_base_addr(td_u32 viproc_id, const td_void *base_addr);
td_void vi_set_viproc_irq_num(td_u32 viproc_id, td_u32 irq_index, td_u32 irq_num);

td_s32 vi_mod_init(td_void);
td_void vi_mod_exit(td_void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_VI_MOD_INIT_H */