/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_VDEC_MOD_INIT_H
#define OT_VDEC_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int vdec_module_init(void);
void vdec_module_exit(void);

void vdec_set_max_chn_num(td_s32 chn_num);
void vdec_set_compat_mode(td_u32 mode);
#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_VDEC_MOD_INIT_H */
