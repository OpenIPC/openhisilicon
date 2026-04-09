/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_frameinfo.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_frameinfo.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include "mpi_sys.h"
#include "isp_main.h"

hi_s32 isp_trans_info_init(hi_vi_pipe vi_pipe, isp_trans_info_buf *trans_info)
{
    hi_s32 ret;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(trans_info);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_TRANS_BUF_INIT, trans_info);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp[%d] init trans info bufs failed %x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_void isp_trans_info_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_TRANS_BUF_EXIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp[%d] exit trans info buf failed %x!\n", vi_pipe, ret);
        return;
    }
}

hi_s32 isp_frame_info_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_trans_info.frame_info.phy_addr;

    isp_ctx->frame_info_ctrl.isp_frame = HI_MPI_SYS_Mmap(phy_addr,
                                                         sizeof(hi_isp_frame_info) * ISP_MAX_FRAMEINFO_BUF_NUM);

    if (isp_ctx->frame_info_ctrl.isp_frame == HI_NULL) {
        isp_err_trace("isp[%d] mmap frame info buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

hi_void isp_frame_info_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->frame_info_ctrl.isp_frame != HI_NULL) {
        HI_MPI_SYS_Munmap(isp_ctx->frame_info_ctrl.isp_frame, sizeof(hi_isp_frame_info) * ISP_MAX_FRAMEINFO_BUF_NUM);
        isp_ctx->frame_info_ctrl.isp_frame = HI_NULL;
    }
}

hi_s32 isp_attach_info_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_trans_info.atta_info.phy_addr;

    isp_ctx->attach_info_ctrl.attach_info = HI_MPI_SYS_Mmap(phy_addr, sizeof(hi_isp_attach_info));

    if (isp_ctx->attach_info_ctrl.attach_info == HI_NULL) {
        isp_err_trace("isp[%d] mmap attach info buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

hi_void isp_attach_info_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->attach_info_ctrl.attach_info != HI_NULL) {
        HI_MPI_SYS_Munmap(isp_ctx->attach_info_ctrl.attach_info, sizeof(hi_isp_attach_info));
        isp_ctx->attach_info_ctrl.attach_info = HI_NULL;
    }
}

hi_s32 isp_color_gamut_info_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_trans_info.color_gammut_info.phy_addr;

    isp_ctx->gamut_info_ctrl.color_gamut_info = HI_MPI_SYS_Mmap(phy_addr, sizeof(hi_isp_colorgammut_info));

    if (isp_ctx->gamut_info_ctrl.color_gamut_info == HI_NULL) {
        isp_err_trace("isp[%d] mmap color gamut info buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    isp_ctx->gamut_info_ctrl.color_gamut_info->color_gamut = COLOR_GAMUT_BT709;

    return HI_SUCCESS;
}

hi_void isp_color_gamut_info_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->gamut_info_ctrl.color_gamut_info != HI_NULL) {
        HI_MPI_SYS_Munmap(isp_ctx->gamut_info_ctrl.color_gamut_info, sizeof(hi_isp_colorgammut_info));
        isp_ctx->gamut_info_ctrl.color_gamut_info = HI_NULL;
    }
}

hi_s32 isp_pro_info_init(hi_vi_pipe vi_pipe, isp_pro_info_buf *pro_info)
{
    hi_s32 ret;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pro_info);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_PRO_BUF_INIT, pro_info);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp[%d] init pro info bufs failed %x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_void isp_pro_info_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_PRO_BUF_EXIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp[%d] exit pro info buf failed %x!\n", vi_pipe, ret);
        return;
    }
}

hi_s32 isp_pro_nr_param_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_pro_info.pro_nr_param_info.phy_addr;

    isp_ctx->pro_nr_param_ctrl.pro_nr_param = HI_MPI_SYS_Mmap(phy_addr, sizeof(isp_pro_nr_param));

    if (isp_ctx->pro_nr_param_ctrl.pro_nr_param == HI_NULL) {
        isp_err_trace("isp[%d] mmap pro nr paramt buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

hi_void isp_pro_nr_param_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->pro_nr_param_ctrl.pro_nr_param != HI_NULL) {
        HI_MPI_SYS_Munmap(isp_ctx->pro_nr_param_ctrl.pro_nr_param, sizeof(isp_pro_nr_param));
        isp_ctx->pro_nr_param_ctrl.pro_nr_param = HI_NULL;
    }
}

hi_s32 isp_pro_shp_param_init(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr = isp_ctx->isp_pro_info.pro_shp_param_info.phy_addr;

    isp_ctx->pro_shp_param_ctrl.pro_shp_param = HI_MPI_SYS_Mmap(phy_addr, sizeof(isp_pro_shp_param));

    if (isp_ctx->pro_shp_param_ctrl.pro_shp_param == HI_NULL) {
        isp_err_trace("isp[%d] mmap pro shp paramt buf failed!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

hi_void isp_pro_shp_param_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->pro_shp_param_ctrl.pro_shp_param != HI_NULL) {
        HI_MPI_SYS_Munmap(isp_ctx->pro_shp_param_ctrl.pro_shp_param, sizeof(isp_pro_shp_param));
        isp_ctx->pro_shp_param_ctrl.pro_shp_param = HI_NULL;
    }
}

