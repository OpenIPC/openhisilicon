/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_DRV_VREG_H
#define OT_DRV_VREG_H

#include "ot_common_3a.h"
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    td_bool used;
    td_u64  size;
    td_phys_addr_t  base_addr;
    td_phys_addr_t  phy_addr;
    td_void *virt_addr;
} drv_vreg_args;

td_s32 isp_ioctl_set_vreg_fd(unsigned int cmd, void *arg, void *private_data);
td_s32 isp_ioctl_init_vreg(unsigned int cmd, void *arg, void *private_data);
td_s32 isp_ioctl_vreg_exit(unsigned int cmd, void *arg, void *private_data);
td_s32 isp_ioctl_release_vreg(unsigned int cmd, void *arg, void *private_data);
td_s32 isp_ioctl_get_vreg_addr(unsigned int cmd, void *arg, void *private_data);
td_s32 isp_ioctl_check_permission(unsigned int cmd, void *arg, void *private_data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef ISP_VREG_H */
