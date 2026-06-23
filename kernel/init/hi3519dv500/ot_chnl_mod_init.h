/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_CHNL_MOD_INIT_H
#define OT_CHNL_MOD_INIT_H

#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int chnl_module_init(void);
void chnl_module_exit(void);
void *chnl_reg_vpu(const void *info);
int chnl_unreg_vpu(void *vpu_hdl);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_CHNL_MOD_INIT_H */
