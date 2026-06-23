/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_KM_INIT_H
#define CRYPTO_KM_INIT_H

#include "ot_type.h"
#include "ca_misc_register.h"

#define UMAP_KM_MINOR_BASE          54
/* KM INIT */
td_s32 crypto_km_init(td_void);
td_void crypto_km_deinit(td_void);

/* IRQ NUM */
extern td_s32 g_rkp_tee_irq_num;
extern td_s32 g_rkp_ree_irq_num;
extern td_s32 g_klad_tee_irq_num;
extern td_s32 g_klad_ree_irq_num;

#endif