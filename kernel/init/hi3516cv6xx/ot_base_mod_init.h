/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_BASE_MOD_INIT_H
#define OT_BASE_MOD_INIT_H

#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int comm_init(void);

void comm_exit(void);

td_void base_set_logmpp_show(td_bool logmpp_show);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_BASE_MOD_INIT_H */
