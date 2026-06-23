/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_IRQ_H
#define CRYPTO_IRQ_H

#include "crypto_type.h"

/* IRQ NUM */
extern td_s32 g_spacc_tee_irq_num;
extern td_s32 g_spacc_ree_irq_num;
extern td_s32 g_pke_tee_irq_num;
extern td_s32 g_pke_ree_irq_num;

td_s32 crypto_irq_init(td_void);

td_void crypto_irq_deinit(td_void);

#endif