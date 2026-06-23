/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_VFMW_MOD_INIT_H
#define OT_VFMW_MOD_INIT_H
#include "ot_type.h"

int vfmw_module_init(void);
void vfmw_module_exit(void);

td_void pdt_set_scd_int(td_s32 scd_int);
td_void pdt_set_pxp_int(td_s32 vdm_pxp_int);
td_void pdt_set_mdma_int(td_s32 mdma_int);
td_void pdt_set_vdm_reg_addr(td_void *reg_addr);
td_void pdt_set_scd_reg_addr(td_void *reg_addr);
td_void vfmw_set_max_chn_num(td_s32 chn_num);
#endif
