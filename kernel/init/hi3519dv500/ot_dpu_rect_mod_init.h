/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_DPU_RECT_MOD_INIT_H
#define OT_DPU_RECT_MOD_INIT_H
#include "ot_type.h"

td_void dpu_rect_set_init_irq(td_u32 ear_id, td_u32 dpu_rect_irq);

td_s32 dpu_rect_init(td_void);
td_void dpu_rect_exit(td_void);
#endif
