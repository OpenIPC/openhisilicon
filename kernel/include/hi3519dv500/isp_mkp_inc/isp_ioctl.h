/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef ISP_IOCTL_H
#define ISP_IOCTL_H
#include "ot_common.h"
#include "ot_osal.h"
#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 isp_init_ioctl_info_check(td_void);
td_void isp_set_ioctl_cmd_list(osal_fileops *isp_fops);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_IOCTL_H */
