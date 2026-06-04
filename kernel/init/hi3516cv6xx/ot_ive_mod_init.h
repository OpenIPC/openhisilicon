/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_IVE_MOD_INIT_H
#define OT_IVE_MOD_INIT_H
#include "ot_type.h"

typedef struct {
    td_u16 mod_node_num;
    td_u8 power_save_en;
} ot_ive_mod_param;

td_s32 ive_set_init_ive_reg(td_u32 dev_id, const td_void *reg);

td_s32 ive_set_init_ive_irq(td_u32 reg_id, td_u32 ive_irq);

td_s32 ive_std_mod_init(const ot_ive_mod_param *mod_param);

td_void ive_std_mod_exit(td_void);

td_s32 ive_svp_npu_load_all_model(td_void);

td_void ive_svp_npu_unload_all_model(td_void);
#endif
