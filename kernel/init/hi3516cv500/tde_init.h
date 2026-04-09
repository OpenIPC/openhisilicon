/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: tde init operation
 */

#ifndef __SOURCE_MSP_DRV_TDE_INIT__
#define __SOURCE_MSP_DRV_TDE_INIT__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
hi_void set_tde_irq(unsigned int temp_tde_irq);
hi_s32 tde_drv_mod_init(hi_void);
hi_void tde_drv_mod_exit(hi_void);
hi_void tde_hal_set_base_vir_addr(hi_u32 *temp_base_vir_addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

