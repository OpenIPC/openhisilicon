/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_dcfinfo.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_dcfinfo.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_ext_config.h"
#include "isp_main.h"

hi_s32 isp_update_info_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr, buf_size;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_trans_info.update_info.phy_addr;

    hi_ext_system_update_info_high_phyaddr_write(vi_pipe, ((phy_addr >> 32) & 0xFFFFFFFF)); /* right shift 32bits */
    hi_ext_system_update_info_low_phyaddr_write(vi_pipe, (phy_addr & 0xFFFFFFFF));

    buf_size = sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info);

    isp_ctx->update_info_ctrl.isp_update_info = HI_MPI_SYS_Mmap(phy_addr, buf_size);

    if (isp_ctx->update_info_ctrl.isp_update_info == HI_NULL) {
        isp_err_trace("isp[%d] mmap update info buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    isp_ctx->update_info_ctrl.isp_dcf_const_info =
        (hi_isp_dcf_const_info *)(isp_ctx->update_info_ctrl.isp_update_info + ISP_MAX_UPDATEINFO_BUF_NUM);

    return HI_SUCCESS;
}

hi_void isp_update_info_exit(hi_vi_pipe vi_pipe)
{
    hi_u64 buf_size;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->update_info_ctrl.isp_update_info != HI_NULL) {
        buf_size = sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info);
        HI_MPI_SYS_Munmap(isp_ctx->update_info_ctrl.isp_update_info, buf_size);
        isp_ctx->update_info_ctrl.isp_update_info = HI_NULL;
        isp_ctx->update_info_ctrl.isp_dcf_const_info = HI_NULL;
    }
}

