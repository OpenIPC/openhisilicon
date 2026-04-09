/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_regcfg.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_regcfg.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "isp_config.h"
#include "isp_lut_config.h"
#include "isp_ext_config.h"
#include "isp_main.h"
#include "mpi_sys.h"
#include "hi_math_adapt.h"

isp_be_buf            g_be_buf_ctx[ISP_MAX_PIPE_NUM]   = {{ 0 }};
isp_reg_cfg_attr     *g_reg_cfg_ctx[ISP_MAX_PIPE_NUM] = { HI_NULL };
isp_be_lut_buf        g_be_lut_buf_ctx[ISP_MAX_PIPE_NUM] = { 0 };
isp_ldci_read_stt_buf g_ldci_read_stt_buf_ctx[ISP_MAX_PIPE_NUM]  = { 0 };

#define isp_regcfg_set_ctx(dev, ctx)    (g_reg_cfg_ctx[dev] = (ctx))
#define isp_regcfg_reset_ctx(dev)       (g_reg_cfg_ctx[dev] = HI_NULL)
#define be_reg_get_ctx(dev, ctx)        ctx = &g_be_buf_ctx[dev]
#define be_lut_buf_get_ctx(dev, ctx)    ctx = &g_be_lut_buf_ctx[dev]
#define ldci_buf_get_ctx(dev, ctx)      ctx = &g_ldci_read_stt_buf_ctx[dev]
#define STT_LUT_CONFIG_TIMES  3
isp_reg_cfg_attr *isp_get_regcfg_ctx(hi_vi_pipe vi_pipe)
{
    return g_reg_cfg_ctx[vi_pipe];
}

hi_s32 isp_mod_param_get(hi_isp_mod_param *mod_param)
{
    isp_check_pointer_return(mod_param);

    mod_param->quick_start = HI_FALSE;

    return HI_SUCCESS;
}

hi_s32 isp_clut_buf_init(hi_vi_pipe vi_pipe)
{
    if (ioctl(isp_get_fd(vi_pipe), ISP_CLUT_BUF_INIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] clut buffer init failed\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_clut_buf_exit(hi_vi_pipe vi_pipe)
{
    if (ioctl(isp_get_fd(vi_pipe), ISP_CLUT_BUF_EXIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] exit clut bufs failed\n", vi_pipe);
        return;
    }
}

hi_s32 isp_spec_awb_buf_init(hi_vi_pipe vi_pipe)
{
    if (ioctl(isp_get_fd(vi_pipe), ISP_SPECAWB_BUF_INIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] specawb buffer init failed\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_spec_awb_buf_exit(hi_vi_pipe vi_pipe)
{
    if (ioctl(isp_get_fd(vi_pipe), ISP_SPECAWB_BUF_EXIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] exit specawb bufs failed\n", vi_pipe);
        return;
    }
}

hi_s32 isp_update_be_lut_stt_buf_ctx(hi_vi_pipe vi_pipe, hi_u64 phy_addr)
{
    hi_u8   i;
    hi_u64  size;
    hi_void *virt_addr = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) ||
        is_striping_mode(isp_ctx->block_attr.running_mode)) {
        return HI_FAILURE;
    }

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    size = sizeof(S_ISP_LUT_WSTT_TYPE);

    virt_addr  = HI_MPI_SYS_Mmap(phy_addr, size * 2 * ISP_MAX_BE_NUM); /* lut_stt_buf number 2  */
    if (virt_addr == HI_NULL) {
        isp_err_trace("Pipe:%d get be lut stt bufs address failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    for (i = 0; i < ISP_MAX_BE_NUM; i++) {
        be_lut_buf->lut_stt_buf[i].phy_addr = phy_addr + 2 * i * size; /* phy_addr index 2  */
        be_lut_buf->lut_stt_buf[i].vir_addr = (hi_void *)((hi_u8 *)virt_addr + 2 * i * size); /* lut_stt_buf 2  */
        be_lut_buf->lut_stt_buf[i].size     = size;
    }

    return HI_SUCCESS;
}

hi_s32 isp_be_lut_buf_addr_init(hi_vi_pipe vi_pipe)
{
    hi_s32  ret;
    hi_u64  phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) ||
        is_striping_mode(isp_ctx->block_attr.running_mode)) {
        return HI_SUCCESS;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_LUT_STT_BUF_GET, &phy_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d get be lut2stt bufs address failed%x!\n", vi_pipe, ret);
        return ret;
    }

    hi_ext_system_be_lut_stt_buffer_high_addr_write(vi_pipe, (phy_addr >> 32)); /* shitf right  32 bit  */
    hi_ext_system_be_lut_stt_buffer_low_addr_write(vi_pipe, (phy_addr & 0xFFFFFFFF));

    ret = isp_update_be_lut_stt_buf_ctx(vi_pipe, phy_addr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_be_lut_buf_addr_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) ||
        is_striping_mode(isp_ctx->block_attr.running_mode)) {
        return HI_SUCCESS;
    }

    if (be_lut_buf->lut_stt_buf[0].vir_addr != HI_NULL) {
        HI_MPI_SYS_Munmap(be_lut_buf->lut_stt_buf[0].vir_addr,
                          sizeof(S_ISP_LUT_WSTT_TYPE) * 2 * ISP_MAX_BE_NUM); /* lut buf number is 2 */
        for (i = 0; i < ISP_MAX_BE_NUM; i++) {
            be_lut_buf->lut_stt_buf[i].vir_addr = HI_NULL;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_stt_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    if (ioctl(isp_get_fd(vi_pipe), ISP_STT_BUF_INIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] stt buffer init failed\n", vi_pipe);
        return HI_FAILURE;
    }

    ret = isp_be_lut_buf_addr_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] be lut2stt buffer address init failed\n", vi_pipe);
        isp_stt_buf_exit(vi_pipe);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_stt_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    ret = isp_be_lut_buf_addr_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] be lut stt buffer exit failed\n", vi_pipe);
    }

    if (ioctl(isp_get_fd(vi_pipe), ISP_STT_BUF_EXIT) != HI_SUCCESS) {
        isp_err_trace("exit stt bufs failed\n");
        return;
    }
}

hi_s32 isp_stt_addr_init(hi_vi_pipe vi_pipe)
{
    if (ioctl(isp_get_fd(vi_pipe), ISP_STT_ADDR_INIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] stt address init failed\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_update_ldci_read_stt_buf_ctx(hi_vi_pipe vi_pipe, hi_u64 phy_addr)
{
    hi_u8  i;
    hi_u64 size;
    hi_void *virt_addr = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *ldci_read_stt_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    ldci_buf_get_ctx(vi_pipe, ldci_read_stt_buf);

    size = sizeof(isp_ldci_stat);
    virt_addr = HI_MPI_SYS_Mmap(phy_addr, size * ldci_read_stt_buf->buf_num);
    if (virt_addr == HI_NULL) {
        isp_err_trace("ISP[%d]:map ldci read stt buffer failed\n", vi_pipe);
        return HI_FAILURE;
    }

    for (i = 0; i < ldci_read_stt_buf->buf_num; i++) {
        ldci_read_stt_buf->read_buf[i].phy_addr = phy_addr + i * size;
        ldci_read_stt_buf->read_buf[i].vir_addr = (hi_void *)((hi_u8 *)virt_addr + i * size);
        ldci_read_stt_buf->read_buf[i].size     = size;
    }

    return HI_SUCCESS;
}

hi_s32 isp_ldci_read_stt_buf_addr_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u64 phy_addr;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *ldci_read_stt_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    ldci_buf_get_ctx(vi_pipe, ldci_read_stt_buf);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_LDCI_READ_STT_BUF_GET, ldci_read_stt_buf);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d]:Get ldci read stt buffer address failed\n", vi_pipe);
        return ret;
    }

    phy_addr = ldci_read_stt_buf->head_phy_addr;
    hi_ext_system_ldci_read_stt_buffer_low_addr_write(vi_pipe, phy_addr & 0xFFFFFFFF);
    hi_ext_system_ldci_read_stt_buffer_high_addr_write(vi_pipe, phy_addr >> 32); /* shitf right  32 bit  */

    ret = isp_update_ldci_read_stt_buf_ctx(vi_pipe, phy_addr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_ldci_read_stt_buf_addr_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *ldci_read_stt_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    ldci_buf_get_ctx(vi_pipe, ldci_read_stt_buf);

    if (ldci_read_stt_buf->read_buf[0].vir_addr != HI_NULL) {
        HI_MPI_SYS_Munmap(ldci_read_stt_buf->read_buf[0].vir_addr,
                          sizeof(isp_ldci_stat) * ldci_read_stt_buf->buf_num);

        for (i = 0; i < ISP_BE_BUF_NUM_MAX; i++) {
            ldci_read_stt_buf->read_buf[i].vir_addr = HI_NULL;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_ldci_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_isp_ctrl_param isp_ctrl_param = { 0 };
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_CTRL_PARAM, &isp_ctrl_param);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get ctrlparam failed\n", vi_pipe);
        return ret;
    }

    isp_ctx->ldci_tpr_flt_en = isp_ctrl_param.ldci_tpr_flt_en;

    if ((isp_ctx->ldci_tpr_flt_en == HI_FALSE) &&
        (is_online_mode(isp_ctx->block_attr.running_mode))) {
        return HI_SUCCESS;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_LDCI_BUF_INIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] ldci buffer init failed\n", vi_pipe);
        return HI_FAILURE;
    }

    if (isp_ctx->ldci_tpr_flt_en == HI_TRUE) {
        ret = isp_ldci_read_stt_buf_addr_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] ldci read stt buffer address init failed\n", vi_pipe);
            isp_ldci_buf_exit(vi_pipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

hi_void isp_ldci_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    if ((isp_ctx->ldci_tpr_flt_en == HI_FALSE) &&
        is_online_mode(isp_ctx->block_attr.running_mode)) {
        return;
    }

    if (isp_ctx->ldci_tpr_flt_en == HI_TRUE) {
        ret = isp_ldci_read_stt_buf_addr_exit(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] exit readstt bufaddr failed\n", vi_pipe);
        }
    }

    if (ioctl(isp_get_fd(vi_pipe), ISP_LDCI_BUF_EXIT) != HI_SUCCESS) {
        isp_err_trace("ISP[%d] exit ldci bufs failed\n", vi_pipe);
        return;
    }
}

hi_s32 isp_cfg_be_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u32 be_buf_size;
    isp_be_buf  *be_buf  = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_offline_mode_return(vi_pipe);
    be_reg_get_ctx(vi_pipe, be_buf);
    isp_get_ctx(vi_pipe, isp_ctx);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_CFG_BUF_INIT, &be_buf->be_phy_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d init be config bufs failed %x!\n", vi_pipe, ret);
        return ret;
    }

    be_buf->be_virt_addr = HI_MPI_SYS_MmapCache(be_buf->be_phy_addr,
                                                sizeof(isp_be_wo_reg_cfg) * isp_ctx->be_buf_num);

    if (be_buf->be_virt_addr == HI_NULL) {
        isp_err_trace("Pipe:%d init be config bufs failed!\n", vi_pipe);
        ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_CFG_BUF_EXIT);
        if (ret != HI_SUCCESS) {
            isp_err_trace("Pipe:%d exit be config bufs failed %x!\n", vi_pipe, ret);
            return ret;
        }

        return HI_ERR_ISP_NOMEM;
    }

    be_buf->be_wo_cfg_buf.phy_addr = be_buf->be_phy_addr;

    /* Get be buffer start address & size */
    be_buf_size = sizeof(isp_be_wo_reg_cfg) * isp_ctx->be_buf_num;
    hi_ext_system_be_buffer_address_high_write(vi_pipe, (be_buf->be_phy_addr >> 32)); /* shitf right  32 bit  */
    hi_ext_system_be_buffer_address_low_write(vi_pipe, (be_buf->be_phy_addr & 0xFFFFFFFF));
    hi_ext_system_be_buffer_size_write(vi_pipe, be_buf_size);

    return HI_SUCCESS;
}

hi_s32 isp_update_be_buf_addr(hi_vi_pipe vi_pipe, hi_void *virt_addr)
{
    hi_u16 i;
    hi_u64 buf_size;
    isp_running_mode isp_runing_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_runing_mode = isp_ctx->block_attr.running_mode;
    buf_size = sizeof(isp_be_wo_reg_cfg) / ISP_STRIPING_MAX_NUM;

    switch (isp_runing_mode) {
        case ISP_MODE_RUNNING_STRIPING:
            for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
                isp_ctx->isp_be_virt_addr[i]  = (hi_void *)((hi_u8 *)virt_addr + i * buf_size);
                isp_ctx->viproc_virt_addr[i] = (hi_void *)((hi_u8 *)isp_ctx->isp_be_virt_addr[i] +
                                                           VIPROC_OFFLINE_OFFSET);
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE:
            for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
                if (i == 0) {
                    isp_ctx->isp_be_virt_addr[i]  = virt_addr;
                    isp_ctx->viproc_virt_addr[i] = (hi_void *)((hi_u8 *)virt_addr + VIPROC_OFFLINE_OFFSET);
                } else {
                    isp_ctx->isp_be_virt_addr[i]  = HI_NULL;
                    isp_ctx->viproc_virt_addr[i] = HI_NULL;
                }
            }
            break;

        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_cfg_be_buf_mmap(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u64 be_phy_addr;
    isp_be_buf *be_buf = HI_NULL;

    be_reg_get_ctx(vi_pipe, be_buf);

    be_phy_addr = be_buf->be_wo_cfg_buf.phy_addr;
    hi_ext_system_be_free_buffer_high_addr_write(vi_pipe, (be_phy_addr >> 32)); /* shitf right  32 bit  */
    hi_ext_system_be_free_buffer_low_addr_write(vi_pipe, (be_phy_addr & 0xFFFFFFFF));

    if (be_buf->be_virt_addr != HI_NULL) {
        be_buf->be_wo_cfg_buf.vir_addr = (hi_void *)((hi_u8 *)be_buf->be_virt_addr + \
                                                     (be_buf->be_wo_cfg_buf.phy_addr - be_buf->be_phy_addr));
    } else {
        be_buf->be_wo_cfg_buf.vir_addr = HI_NULL;
    }

    if (be_buf->be_wo_cfg_buf.vir_addr == HI_NULL) {
        return HI_FAILURE;
    }

    ret = isp_update_be_buf_addr(vi_pipe, be_buf->be_wo_cfg_buf.vir_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp update BE bufs failed %x!\n", vi_pipe, ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_be_buf_first(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_be_buf *be_buf = HI_NULL;

    isp_check_offline_mode_return(vi_pipe);
    be_reg_get_ctx(vi_pipe, be_buf);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_BE_BUF_FIRST, &be_buf->be_wo_cfg_buf.phy_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d Get be free bufs failed %x!\n", vi_pipe, ret);
        return ret;
    }

    ret = isp_cfg_be_buf_mmap(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_cfg_be_buf_mmap failed %x!\n", vi_pipe, ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_be_free_buf(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_be_buf *be_buf = HI_NULL;

    isp_check_offline_mode_return(vi_pipe);
    be_reg_get_ctx(vi_pipe, be_buf);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_FREE_BUF_GET, &be_buf->be_wo_cfg_buf);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_cfg_be_buf_mmap(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_cfg_be_buf_mmap failed %x!\n", vi_pipe, ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_be_last_buf(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_be_buf *be_buf = HI_NULL;

    isp_check_offline_mode_return(vi_pipe);
    be_reg_get_ctx(vi_pipe, be_buf);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_LAST_BUF_GET, &be_buf->be_wo_cfg_buf.phy_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d Get be busy bufs failed %x!\n", vi_pipe, ret);
        return ret;
    }

    ret = isp_cfg_be_buf_mmap(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_cfg_be_buf_mmap failed %x!\n", vi_pipe, ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_cfg_be_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_be_buf  *be_buf  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if ((is_online_mode(isp_ctx->block_attr.running_mode)) ||
        (is_sidebyside_mode(isp_ctx->block_attr.running_mode))) {
        return;
    }
    be_reg_get_ctx(vi_pipe, be_buf);

    if (be_buf->be_virt_addr != HI_NULL) {
        HI_MPI_SYS_Munmap(be_buf->be_virt_addr, sizeof(isp_be_wo_reg_cfg) * isp_ctx->be_buf_num);
        be_buf->be_virt_addr = HI_NULL;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_CFG_BUF_EXIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d exit be config bufs failed %x!\n", vi_pipe, ret);
        return;
    }
}

hi_s32 isp_cfg_be_buf_ctl(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_be_buf *be_buf = HI_NULL;

    isp_check_offline_mode_return(vi_pipe);
    be_reg_get_ctx(vi_pipe, be_buf);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_CFG_BUF_CTL, &be_buf->be_wo_cfg_buf);
    if (ret) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_cfg_be_buf_state(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    isp_check_offline_mode_return(vi_pipe);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_CFG_BUF_RUNNING);
    if (ret) {
        return ret;
    }

    return HI_SUCCESS;
}

/* init isp be cfgs all buffer */
hi_s32 isp_all_cfgs_be_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    isp_check_offline_mode_return(vi_pipe);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_All_BUF_INIT);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init be all bufs Failed with ec %#x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s8 isp_get_block_id_by_pipe(hi_vi_pipe vi_pipe)
{
    hi_s8 block_id = 0;
    switch (vi_pipe) {
        case ISP_BE0_PIPE_ID:
            block_id = 0;
            break;

        case ISP_BE1_PIPE_ID:
            block_id = 1;
            break;

        default:
            return HI_FAILURE;
    }

    return block_id;
}

hi_s32 isp_be_vreg_addr_init(hi_vi_pipe vi_pipe)
{
    hi_u8  k = 0;
    hi_s8  blk_dev = 0;
    hi_u8  block_id = 0;
    hi_u64 buf_size;
    isp_running_mode isp_runing_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_be_buf *be_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    be_reg_get_ctx(vi_pipe, be_buf);

    isp_runing_mode = isp_ctx->block_attr.running_mode;
    buf_size      = sizeof(isp_be_wo_reg_cfg) / ISP_STRIPING_MAX_NUM;

    switch (isp_runing_mode) {
        case ISP_MODE_RUNNING_ONLINE:
            blk_dev = isp_get_block_id_by_pipe(vi_pipe);
            if (blk_dev == -1) {
                isp_err_trace("ISP[%d] init Online Mode Pipe Err!\n", vi_pipe);
                return HI_FAILURE;
            }

            block_id = (hi_u8)blk_dev;
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    isp_ctx->isp_be_virt_addr[k] = vreg_get_virt_addr_base(isp_be_reg_base(block_id));
                    isp_ctx->viproc_virt_addr[k] = vreg_get_virt_addr_base(isp_viproc_reg_base(block_id));
                } else {
                    isp_ctx->isp_be_virt_addr[k] = HI_NULL;
                    isp_ctx->viproc_virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    isp_ctx->isp_be_virt_addr[k] = be_buf->be_wo_cfg_buf.vir_addr;
                    isp_ctx->viproc_virt_addr[k] = (hi_void *)((hi_u8 *)be_buf->be_wo_cfg_buf.vir_addr +
                                                               VIPROC_OFFLINE_OFFSET);
                } else {
                    isp_ctx->isp_be_virt_addr[k] = HI_NULL;
                    isp_ctx->viproc_virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k < ISP_MAX_BE_NUM) {
                    isp_ctx->isp_be_virt_addr[k] = vreg_get_virt_addr_base(isp_be_reg_base(k));
                    isp_ctx->viproc_virt_addr[k] = vreg_get_virt_addr_base(isp_viproc_reg_base(k));
                } else {
                    isp_ctx->isp_be_virt_addr[k] = HI_NULL;
                    isp_ctx->viproc_virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_STRIPING:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                isp_ctx->isp_be_virt_addr[k] = (hi_void *)((hi_u8 *)be_buf->be_wo_cfg_buf.vir_addr +
                                                           k * buf_size);
                isp_ctx->viproc_virt_addr[k] = (hi_void *)((hi_u8 *)isp_ctx->isp_be_virt_addr[k] +
                                                           VIPROC_OFFLINE_OFFSET);
            }
            break;

        default:
            isp_err_trace("ISP[%d] init Running Mode Err!\n", vi_pipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void *isp_vreg_cfg_buf_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev)
{
    hi_u64 size;
    hi_u64 phy_addr_high;
    hi_u64 phy_addr_temp;
    isp_be_buf *be_buf = HI_NULL;

    be_reg_get_ctx(vi_pipe, be_buf);

    size = sizeof(isp_be_wo_reg_cfg) / ISP_STRIPING_MAX_NUM;

    if (be_buf->be_wo_cfg_buf.vir_addr != HI_NULL) {
        return ((hi_u8 *)be_buf->be_wo_cfg_buf.vir_addr + blk_dev * size);
    }

    phy_addr_high = (hi_u64)hi_ext_system_be_free_buffer_high_addr_read(vi_pipe);
    phy_addr_temp = (hi_u64)hi_ext_system_be_free_buffer_low_addr_read(vi_pipe);
    phy_addr_temp |= (phy_addr_high << 32); /* shitf left  32 bit  */

    be_buf->be_wo_cfg_buf.phy_addr = phy_addr_temp;
    be_buf->be_wo_cfg_buf.vir_addr = HI_MPI_SYS_MmapCache(be_buf->be_wo_cfg_buf.phy_addr,
                                                          sizeof(isp_be_wo_reg_cfg));

    return ((hi_u8 *)be_buf->be_wo_cfg_buf.vir_addr + blk_dev * size);
}

hi_s32 isp_get_be_vreg_cfg_addr(hi_vi_pipe vi_pipe, hi_void *virt_addr[])
{
    hi_u8 k = 0;
    hi_s8 blk_dev = 0;
    hi_u8 block_id = 0;
    hi_s32 ret;
    isp_working_mode isp_work_mode;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode);
    if (ret) {
        isp_err_trace("get isp work mode failed!\n");
        return ret;
    }

    switch (isp_work_mode.running_mode) {
        case ISP_MODE_RUNNING_ONLINE:
            blk_dev = isp_get_block_id_by_pipe(vi_pipe);
            if (blk_dev == -1) {
                isp_err_trace("ISP[%d] Online Mode Pipe Err!\n", vi_pipe);
                return HI_FAILURE;
            }

            block_id = (hi_u8)blk_dev;
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    virt_addr[k] = vreg_get_virt_addr_base(isp_be_reg_base(block_id));
                } else {
                    virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    virt_addr[k] = isp_vreg_cfg_buf_addr(vi_pipe, (hi_blk_dev)k);
                } else {
                    virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k < ISP_MAX_BE_NUM) {
                    virt_addr[k] = vreg_get_virt_addr_base(isp_be_reg_base(k));
                } else {
                    virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_STRIPING:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                virt_addr[k] = isp_vreg_cfg_buf_addr(vi_pipe, (hi_blk_dev)k);
            }
            break;

        default:
            isp_err_trace("ISP[%d] GetBe Running Mode Err!\n", vi_pipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_vi_proc_cfg_addr(hi_vi_pipe vi_pipe, hi_void *virt_addr[])
{
    hi_u8 k = 0;
    hi_s8 blk_dev = 0;
    hi_u8 block_id = 0;
    hi_s32 ret;
    hi_void *be_virt_addr = HI_NULL;
    isp_working_mode isp_work_mode;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode);
    if (ret) {
        isp_err_trace("get isp work mode failed!\n");
        return ret;
    }

    switch (isp_work_mode.running_mode) {
        case ISP_MODE_RUNNING_ONLINE:
            blk_dev = isp_get_block_id_by_pipe(vi_pipe);
            if (blk_dev == -1) {
                isp_err_trace("ISP[%d] Online Mode Pipe Err!\n", vi_pipe);
                return HI_FAILURE;
            }

            block_id = (hi_u8)blk_dev;
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    virt_addr[k] = vreg_get_virt_addr_base(isp_viproc_reg_base(block_id));
                } else {
                    virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    be_virt_addr = isp_vreg_cfg_buf_addr(vi_pipe, (hi_blk_dev)k);
                    virt_addr[k] = (hi_void *)((hi_u8 *)be_virt_addr + VIPROC_OFFLINE_OFFSET);
                } else {
                    virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k < ISP_MAX_BE_NUM) {
                    virt_addr[k] = vreg_get_virt_addr_base(isp_viproc_reg_base(k));
                } else {
                    virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_STRIPING:
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                be_virt_addr = isp_vreg_cfg_buf_addr(vi_pipe, (hi_blk_dev)k);
                virt_addr[k] = (hi_void *)((hi_u8 *)be_virt_addr + VIPROC_OFFLINE_OFFSET);
            }
            break;

        default:
            isp_err_trace("ISP[%d] GetBe Running Mode Err!\n", vi_pipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void *isp_get_ldci_read_stt_vir_addr(hi_vi_pipe vi_pipe, hi_u8 buf_idx)
{
    hi_s32 ret;
    hi_u64 phy_addr_high, phy_addr_tmp;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *ldci_read_stt_buf = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_NULL;
    }

    ldci_buf_get_ctx(vi_pipe, ldci_read_stt_buf);

    if (ldci_read_stt_buf->read_buf[buf_idx].vir_addr != HI_NULL) {
        return ldci_read_stt_buf->read_buf[buf_idx].vir_addr;
    }

    phy_addr_high = (hi_u64)hi_ext_system_ldci_read_stt_buffer_high_addr_read(vi_pipe);
    phy_addr_tmp  = (hi_u64)hi_ext_system_ldci_read_stt_buffer_low_addr_read(vi_pipe);
    phy_addr_tmp |= (phy_addr_high << 32); /* shitf left  32 bit  */

    ret = isp_update_ldci_read_stt_buf_ctx(vi_pipe, phy_addr_tmp);
    if (ret != HI_SUCCESS) {
        return HI_NULL;
    }

    return ldci_read_stt_buf->read_buf[buf_idx].vir_addr;
}

hi_void *isp_get_fe_vir_addr(hi_vi_pipe vi_pipe)
{
    isp_check_fe_pipe_return(vi_pipe);

    return vreg_get_virt_addr_base(isp_fe_reg_base(vi_pipe));
}

hi_void *isp_get_be_lut2stt_vir_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev, hi_u8 buf_id)
{
    hi_s32 ret;
    hi_u64 size;
    hi_u64 phy_addr_high, phy_addr_temp;
    isp_be_lut_buf *be_lut_buf = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_fe_pipe_return(vi_pipe);
    isp_check_be_dev_return(blk_dev);
    isp_get_ctx(vi_pipe, isp_ctx);
    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) || \
        is_striping_mode(isp_ctx->block_attr.running_mode)) {
        return HI_NULL;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);

    if (be_lut_buf->lut_stt_buf[blk_dev].vir_addr != HI_NULL) {
        return (hi_void *)((hi_u8 *)be_lut_buf->lut_stt_buf[blk_dev].vir_addr + size * buf_id);
    }

    phy_addr_high = (hi_u64)hi_ext_system_be_lut_stt_buffer_high_addr_read(vi_pipe);
    phy_addr_temp = (hi_u64)hi_ext_system_be_lut_stt_buffer_low_addr_read(vi_pipe);
    phy_addr_temp |= (phy_addr_high << 32); /* shitf left  32 bit  */

    ret = isp_update_be_lut_stt_buf_ctx(vi_pipe, phy_addr_temp);
    if (ret != HI_SUCCESS) {
        return HI_NULL;
    }

    return (hi_void *)((hi_u8 *)be_lut_buf->lut_stt_buf[blk_dev].vir_addr + size * buf_id);
}

hi_void *isp_get_be_vir_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_void *virt_addr[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_fe_pipe_return(vi_pipe);
    isp_check_be_dev_return(blk_dev);
    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->isp_be_virt_addr[blk_dev] != HI_NULL) {
        return isp_ctx->isp_be_virt_addr[blk_dev];
    }

    ret = isp_get_be_vreg_cfg_addr(vi_pipe, virt_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Get Be CfgAddr Failed!\n", vi_pipe);
        return HI_NULL;
    }

    isp_check_reg_nullptr_return(virt_addr[blk_dev]);

    return virt_addr[blk_dev];
}

hi_void *isp_get_vi_proc_vir_addr(hi_vi_pipe vi_pipe, hi_blk_dev blk_dev)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_void *virt_addr[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_fe_pipe_return(vi_pipe);
    isp_check_be_dev_return(blk_dev);
    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->viproc_virt_addr[blk_dev] != HI_NULL) {
        return isp_ctx->viproc_virt_addr[blk_dev];
    }

    ret = isp_get_vi_proc_cfg_addr(vi_pipe, virt_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Get viproc CfgAddr Failed!\n", vi_pipe);
        return HI_NULL;
    }

    isp_check_reg_nullptr_return(virt_addr[blk_dev]);

    return virt_addr[blk_dev];
}

static hi_s32 isp_clut_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                           S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;
    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    isp_clut_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr  + ISP_CLUT_LUT_WSTT_OFFSET + buf_id * size));
    isp_clut_lut_width_word_write(vi_proc_reg, HI_ISP_CLUT_LUT_WIDTH_WORD_DEFAULT);

    return HI_SUCCESS;
}

static hi_s32 isp_sharpen_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                              S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size, addr_offset;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    addr_offset = ISP_SHARPEN_LUT_WSTT_OFFSET + buf_id * size;
    isp_sharpen_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + addr_offset));

    return HI_SUCCESS;
}

static hi_s32 isp_dehaze_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                             S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size, addr_offset;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    addr_offset = ISP_DEHAZE_LUT_WSTT_OFFSET + buf_id * size;
    isp_dehaze_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + addr_offset));

    return HI_SUCCESS;
}

static hi_s32 isp_gamma_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                            S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size, addr_offset;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    addr_offset = ISP_GAMMA_LUT_WSTT_OFFSET + buf_id * size;
    isp_gamma_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + addr_offset));

    return HI_SUCCESS;
}

static hi_s32 isp_bnr_rlsc_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                               S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size, addr_offset;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    addr_offset = ISP_BNR_RLSC_LUT_WSTT_OFFSET + buf_id * size;
    isp_bnr_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + addr_offset));

    return HI_SUCCESS;
}

static hi_s32 isp_dpc_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id, S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    isp_dcg_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + ISP_DPC_LUT_WSTT_OFFSET + buf_id * size));

    return HI_SUCCESS;
}

static hi_s32 isp_lsc_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id, S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    isp_lsc_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + ISP_LSC_LUT_WSTT_OFFSET + buf_id * size));

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_ISP_CA_SUPPORT
static hi_s32 isp_ca_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id, S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    isp_ca_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + ISP_CA_LUT_WSTT_OFFSET + buf_id * size));

    return HI_SUCCESS;
}
#endif
static hi_s32 isp_ldci_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                           S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size, addr_offset;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    addr_offset = ISP_LDCI_LUT_WSTT_OFFSET + buf_id * size;
    isp_ldci_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + addr_offset));

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
static hi_s32 isp_pregamma_lut_wstt_addr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 buf_id,
                                               S_VIPROC_REGS_TYPE *vi_proc_reg)
{
    hi_u64 phy_addr, size, addr_offset;
    isp_be_lut_buf *be_lut_buf = HI_NULL;

    be_lut_buf_get_ctx(vi_pipe, be_lut_buf);

    phy_addr = be_lut_buf->lut_stt_buf[i].phy_addr;

    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    size = sizeof(S_ISP_LUT_WSTT_TYPE);
    addr_offset = ISP_PREGAMMA_LUT_WSTT_OFFSET + buf_id * size;
    isp_pregamma_lut_addr_low_write(vi_proc_reg, get_low_addr(phy_addr + addr_offset));

    return HI_SUCCESS;
}
#endif
static hi_s32 isp_fe_dg_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u16 i;
    hi_vi_pipe vi_pipe_bind;
    isp_fe_dg_dyna_cfg *dyna_reg_cfg = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg;

    if (isp_ctx->wdr_attr.mast_pipe != HI_TRUE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        isp_check_pipe_return(vi_pipe_bind);
        fe_reg = (S_ISPFE_REGS_TYPE*)isp_get_fe_vir_addr(vi_pipe_bind);
        isp_check_pointer_return(fe_reg);

        if (reg_cfg_info->cfg_key.bit1_fe_dg_cfg) {
            isp_fe_dg2_en_write(fe_reg, reg_cfg_info->alg_reg_cfg[0].fe_dg_reg_cfg.dg_en);

            if (dyna_reg_cfg->resh) {
                isp_dg2_gain_write(fe_reg, dyna_reg_cfg->gain_r[i], dyna_reg_cfg->gain_gr[i],
                                   dyna_reg_cfg->gain_gb[i], dyna_reg_cfg->gain_b[i]);
                isp_dg2_clip_value_write(fe_reg, dyna_reg_cfg->clip_value);
            }
        }
    }

    dyna_reg_cfg->resh = HI_FALSE;
    reg_cfg_info->cfg_key.bit1_fe_dg_cfg = 0;

    return HI_SUCCESS;
}

static hi_s32 isp_fe_blc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u16  i;
    hi_vi_pipe vi_pipe_bind;
    isp_fe_blc_static_cfg *static_cfg = HI_NULL;
    isp_fe_blc_dyna_cfg   *dyna_cfg   = HI_NULL;
    isp_usr_ctx           *isp_ctx    = HI_NULL;
    S_ISPFE_REGS_TYPE     *fe_reg     = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    static_cfg = &reg_cfg_info->alg_reg_cfg[0].fe_blc_cfg.static_blc;
    dyna_cfg   = &reg_cfg_info->alg_reg_cfg[0].fe_blc_cfg.dyna_blc;

    if (isp_ctx->wdr_attr.mast_pipe != HI_TRUE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        isp_check_pipe_return(vi_pipe_bind);
        fe_reg = (S_ISPFE_REGS_TYPE*)isp_get_fe_vir_addr(vi_pipe_bind);
        isp_check_pointer_return(fe_reg);

        if (reg_cfg_info->cfg_key.bit1_fe_blc_cfg) {
            if (static_cfg->resh_static) {
                isp_dg2_en_write(fe_reg, static_cfg->fe_dg_blc.blc_in, static_cfg->fe_dg_blc.blc_out);
                isp_wb1_en_write(fe_reg, static_cfg->fe_wb_blc.blc_in, static_cfg->fe_wb_blc.blc_out);
                isp_ae1_blc_en_write(fe_reg, static_cfg->fe_ae_blc.blc_in);
                isp_fe_blc1_en_write(fe_reg, static_cfg->fe_blc.blc_in);
            }

            if (reg_cfg_info->alg_reg_cfg[0].fe_blc_cfg.resh_dyna_init == HI_TRUE) {
                isp_dg2_offset_write(fe_reg, dyna_cfg->fe_dg_blc.blc);
                isp_wb1_offset_write(fe_reg, dyna_cfg->fe_wb_blc.blc);
                isp_ae1_offset_write(fe_reg, dyna_cfg->fe_ae_blc.blc);
                isp_blc1_offset_write(fe_reg, dyna_cfg->fe_blc.blc);
            }
        }
    }

    static_cfg->resh_static                                = HI_FALSE;
    reg_cfg_info->alg_reg_cfg[0].fe_blc_cfg.resh_dyna_init = HI_FALSE;
    reg_cfg_info->cfg_key.bit1_fe_blc_cfg                  = 0;

    return HI_SUCCESS;
}

static hi_void isp_fe_ae_weight_config(S_ISPFE_REGS_TYPE *fe_reg, isp_ae_dyna_cfg *dyna_reg_cfg)
{
    hi_u16  j, k;
    hi_u32  table_weight_tmp = 0;
    hi_u32  combin_weight = 0;
    hi_u32  combin_weight_num = 0;

    isp_ae1_wei_waddr_write(fe_reg, 0);

    for (j = 0; j < AE_ZONE_ROW; j++) {
        for (k = 0; k < AE_ZONE_COLUMN; k++) {
            table_weight_tmp = (hi_u32)dyna_reg_cfg->fe_weight_table[j][k];
            combin_weight |= (table_weight_tmp << (8 * combin_weight_num)); /* weightTmp shift left 8 */
            combin_weight_num++;

            if (combin_weight_num == HI_ISP_AE_WEI_COMBIN_COUNT) {
                isp_ae1_wei_wdata_write(fe_reg, combin_weight);
                combin_weight_num = 0;
                combin_weight = 0;
            }
        }
    }

    if ((combin_weight_num != HI_ISP_AE_WEI_COMBIN_COUNT) && (combin_weight_num != 0)) {
        isp_ae1_wei_wdata_write(fe_reg, combin_weight);
    }
}

static hi_s32 isp_fe_ae_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_bool lut_update_en = HI_FALSE;
    hi_u16  i;
    hi_vi_pipe vi_pipe_bind;
    isp_ae_static_cfg *static_reg_cfg = HI_NULL;
    isp_ae_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg         = HI_NULL;
    isp_usr_ctx       *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe != HI_TRUE) {
        return HI_SUCCESS;
    }

    if (reg_cfg_info->cfg_key.bit1_ae_cfg1) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            isp_check_pipe_return(vi_pipe_bind);

            fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
            isp_check_pointer_return(fe_reg);
            /* ae fe static */
            static_reg_cfg = &reg_cfg_info->alg_reg_cfg[0].ae_reg_cfg.static_reg_cfg;
            dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg;

            isp_fe_ae1_en_write(fe_reg, static_reg_cfg->fe_enable);
            isp_ae1_crop_pos_write(fe_reg, static_reg_cfg->fe_crop_pos_x, static_reg_cfg->fe_crop_pos_y);
            isp_ae1_crop_outsize_write(fe_reg, static_reg_cfg->fe_crop_out_width - 1,
                                       static_reg_cfg->fe_crop_out_height - 1);

            /* ae fe dynamic */
            isp_ae1_zone_write(fe_reg, dyna_reg_cfg->be_weight_table_width, dyna_reg_cfg->be_weight_table_height);
            isp_ae1_skip_crg_write(fe_reg, dyna_reg_cfg->fe_hist_skip_x, dyna_reg_cfg->fe_hist_offset_x,
                                   dyna_reg_cfg->fe_hist_skip_y, dyna_reg_cfg->fe_hist_offset_y);
            isp_ae1_bitmove_write(fe_reg, dyna_reg_cfg->fe_bit_move, dyna_reg_cfg->fe_hist_gamma_mode,
                                  dyna_reg_cfg->fe_aver_gamma_mode);
            isp_ae1_gamma_limit_write(fe_reg, dyna_reg_cfg->fe_gamma_limit);
            isp_ae1_fourplanemode_write(fe_reg, dyna_reg_cfg->fe_four_plane_mode);

            isp_fe_ae_weight_config(fe_reg, dyna_reg_cfg);

            lut_update_en = dyna_reg_cfg->fe_weight_table_update;
        }
    }

    reg_cfg_info->alg_reg_cfg[0].fe_lut_update_cfg.ae1_lut_update = lut_update_en;
    return HI_SUCCESS;
}

static hi_void isp_ae_weight_reg_config(isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                        isp_ae_dyna_cfg *dyna_reg_be_cfg)
{
    hi_u16 j, k, m;
    hi_u32 combin_weight, combin_weight_num;
    hi_u32 combine_wgt[64] = { 0 };        /* ae weight table number 64 */

    m = 0;
    combin_weight = 0;
    combin_weight_num = 0;

    for (j = 0; j < dyna_reg_be_cfg->be_weight_table_height; j++) {
        for (k = 0; k < dyna_reg_be_cfg->be_weight_table_width; k++) {
            combin_weight |= ((hi_u32)dyna_reg_be_cfg->be_weight_table[j][k] << (8 * combin_weight_num)); /* 8 */
            combin_weight_num++;

            if (combin_weight_num == HI_ISP_AE_WEI_COMBIN_COUNT) {
                combine_wgt[m++] = combin_weight;
                combin_weight_num = 0;
                combin_weight = 0;
            }
        }
    }

    if ((combin_weight_num != HI_ISP_AE_WEI_COMBIN_COUNT) &&
        (combin_weight_num != 0)) {
        combine_wgt[m++] = combin_weight;
    }

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_ae_wei_waddr_write(be_reg, 0);

        for (m = 0; m < 64; m++) {   /* ae weight table number 64 */
            isp_ae_wei_wdata_write(be_reg, combine_wgt[m]);
        }
    } else {
        isp_ae_weight_write(be_reg, combine_wgt);
    }
}

static hi_s32 isp_ae_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool lut_update_en = HI_FALSE;
    hi_bool offline_mode;
    isp_ae_static_cfg *static_reg_be_cfg = HI_NULL;
    isp_ae_dyna_cfg   *dyna_reg_be_cfg   = HI_NULL;
    isp_mg_static_cfg *mg_static_reg_cfg = HI_NULL;
    isp_mg_dyna_cfg   *mg_dyna_reg_cfg   = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg            = HI_NULL;
    isp_usr_ctx       *isp_ctx           = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_ae_cfg1) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        /* ae be static */
        static_reg_be_cfg = &reg_cfg_info->alg_reg_cfg[i].ae_reg_cfg.static_reg_cfg;
        dyna_reg_be_cfg   = &reg_cfg_info->alg_reg_cfg[i].ae_reg_cfg.dyna_reg_cfg;

        isp_ae_crop_pos_x_write(be_reg, static_reg_be_cfg->be_crop_pos_x);
        isp_ae_crop_pos_y_write(be_reg, static_reg_be_cfg->be_crop_pos_y);
        isp_ae_crop_out_width_write(be_reg, static_reg_be_cfg->be_crop_out_width - 1);
        isp_ae_crop_out_height_write(be_reg, static_reg_be_cfg->be_crop_out_height - 1);
        /* ae be dynamic */
        isp_ae_sel_write(be_reg, dyna_reg_be_cfg->be_ae_sel);
        isp_ae_hnum_write(be_reg, dyna_reg_be_cfg->be_weight_table_width);
        isp_ae_vnum_write(be_reg, dyna_reg_be_cfg->be_weight_table_height);
        isp_ae_skip_x_write(be_reg, dyna_reg_be_cfg->be_hist_skip_x);
        isp_ae_offset_x_write(be_reg, dyna_reg_be_cfg->be_hist_offset_x);
        isp_ae_skip_y_write(be_reg, dyna_reg_be_cfg->be_hist_skip_y);
        isp_ae_offset_y_write(be_reg, dyna_reg_be_cfg->be_hist_offset_y);
        isp_ae_bitmove_write(be_reg, dyna_reg_be_cfg->be_bit_move);
        isp_ae_hist_gamma_mode_write(be_reg, dyna_reg_be_cfg->be_hist_gamma_mode);
        isp_ae_aver_gamma_mode_write(be_reg, dyna_reg_be_cfg->be_aver_gamma_mode);
        isp_ae_gamma_limit_write(be_reg, dyna_reg_be_cfg->be_gamma_limit);
        isp_ae_four_plane_mode_write(be_reg, dyna_reg_be_cfg->be_four_plane_mode);

        isp_ae_weight_reg_config(isp_ctx, be_reg, dyna_reg_be_cfg);

        lut_update_en = dyna_reg_be_cfg->be_weight_table_update;

        /* mg static */
        mg_static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].mg_reg_cfg.static_reg_cfg;
        mg_dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].mg_reg_cfg.dyna_reg_cfg;

        isp_la_crop_pos_x_write(be_reg, mg_static_reg_cfg->crop_pos_x);
        isp_la_crop_pos_y_write(be_reg, mg_static_reg_cfg->crop_pos_y);
        isp_la_crop_out_width_write(be_reg, mg_static_reg_cfg->crop_out_width - 1);
        isp_la_crop_out_height_write(be_reg, mg_static_reg_cfg->crop_out_height - 1);

        /* mg dynamic */
        isp_la_hnum_write(be_reg, mg_dyna_reg_cfg->zone_width);
        isp_la_vnum_write(be_reg, mg_dyna_reg_cfg->zone_height);
        isp_la_bitmove_write(be_reg, mg_dyna_reg_cfg->bit_move);
        isp_la_gamma_en_write(be_reg, mg_dyna_reg_cfg->gamma_mode);
        isp_la_gamma_limit_write(be_reg, mg_dyna_reg_cfg->gamma_limit);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.ae_lut_update = lut_update_en || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_af_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_AF_SUPPORT
    hi_bool  offline_mode;
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    isp_af_reg_cfg     *af_reg_be_cfg = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg        = HI_NULL;
    isp_usr_ctx        *isp_ctx       = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    af_reg_be_cfg = &reg_cfg_info->alg_reg_cfg[i].be_af_reg_cfg;
    offline_mode  = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);

    idx_resh = (isp_af_update_index_read(be_reg) != af_reg_be_cfg->update_index);
    usr_resh = (offline_mode) ? (reg_cfg_info->cfg_key.bit1_af_be_cfg && idx_resh) :
        (reg_cfg_info->cfg_key.bit1_af_be_cfg);

    if (usr_resh) {
        isp_af_update_index_write(be_reg, af_reg_be_cfg->update_index);
        isp_af_lpf_en_write(be_reg, af_reg_be_cfg->lpf_enable);
        isp_af_fir0_lpf_en_write(be_reg, af_reg_be_cfg->fir0_lpf_enable);
        isp_af_fir1_lpf_en_write(be_reg, af_reg_be_cfg->fir1_lpf_enable);
        isp_af_iir0_ds_en_write(be_reg, af_reg_be_cfg->iir0_ds_enable);
        isp_af_iir1_ds_en_write(be_reg, af_reg_be_cfg->iir1_ds_enable);
        isp_af_iir_dilate0_write(be_reg, af_reg_be_cfg->iir_dilate0);
        isp_af_iir_dilate1_write(be_reg, af_reg_be_cfg->iir_dilate1);
        isp_af_iirplg_0_write(be_reg, af_reg_be_cfg->iir_plg_group0);
        isp_af_iirpls_0_write(be_reg, af_reg_be_cfg->iir_pls_group0);
        isp_af_iirplg_1_write(be_reg, af_reg_be_cfg->iir_plg_group1);
        isp_af_iirpls_1_write(be_reg, af_reg_be_cfg->iir_pls_group1);

        isp_af_iir0_en0_write(be_reg, af_reg_be_cfg->iir0_enable0);
        isp_af_iir0_en1_write(be_reg, af_reg_be_cfg->iir0_enable1);
        isp_af_iir0_en2_write(be_reg, af_reg_be_cfg->iir0_enable2);
        isp_af_iir1_en0_write(be_reg, af_reg_be_cfg->iir1_enable0);
        isp_af_iir1_en1_write(be_reg, af_reg_be_cfg->iir1_enable1);
        isp_af_iir1_en2_write(be_reg, af_reg_be_cfg->iir1_enable2);
        isp_af_peak_mode_write(be_reg, af_reg_be_cfg->peak_mode);
        isp_af_squ_mode_write(be_reg, af_reg_be_cfg->squ_mode);
        isp_af_hnum_write(be_reg, af_reg_be_cfg->window_hnum);
        isp_af_vnum_write(be_reg, af_reg_be_cfg->window_vnum);

        isp_af_iirg0_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain0_group0);
        isp_af_iirg0_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain0_group1);

        isp_af_iirg1_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain1_group0);
        isp_af_iirg1_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain1_group1);

        isp_af_iirg2_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain2_group0);
        isp_af_iirg2_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain2_group1);

        isp_af_iirg3_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain3_group0);
        isp_af_iirg3_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain3_group1);

        isp_af_iirg4_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain4_group0);
        isp_af_iirg4_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain4_group1);

        isp_af_iirg5_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain5_group0);
        isp_af_iirg5_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain5_group1);

        isp_af_iirg6_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain6_group0);
        isp_af_iirg6_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain6_group1);

        isp_af_iirshift0_0_write(be_reg, af_reg_be_cfg->iir0_shift_group0);
        isp_af_iirshift0_1_write(be_reg, af_reg_be_cfg->iir1_shift_group0);
        isp_af_iirshift0_2_write(be_reg, af_reg_be_cfg->iir2_shift_group0);
        isp_af_iirshift0_3_write(be_reg, af_reg_be_cfg->iir3_shift_group0);
        isp_af_iirshift1_0_write(be_reg, af_reg_be_cfg->iir0_shift_group1);
        isp_af_iirshift1_1_write(be_reg, af_reg_be_cfg->iir1_shift_group1);
        isp_af_iirshift1_2_write(be_reg, af_reg_be_cfg->iir2_shift_group1);
        isp_af_iirshift1_3_write(be_reg, af_reg_be_cfg->iir3_shift_group1);

        isp_af_firh0_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain0_group0);
        isp_af_firh0_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain0_group1);

        isp_af_firh1_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain1_group0);
        isp_af_firh1_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain1_group1);

        isp_af_firh2_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain2_group0);
        isp_af_firh2_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain2_group1);

        isp_af_firh3_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain3_group0);
        isp_af_firh3_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain3_group1);

        isp_af_firh4_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain4_group0);
        isp_af_firh4_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain4_group1);

        /* AF BE crop */
        isp_af_crop_en_write(be_reg, af_reg_be_cfg->crop_enable);
        if (af_reg_be_cfg->crop_enable) {
            isp_af_pos_x_write(be_reg, af_reg_be_cfg->crop_pos_x);
            isp_af_pos_y_write(be_reg, af_reg_be_cfg->crop_pos_y);
            isp_af_crop_hsize_write(be_reg, af_reg_be_cfg->crop_h_size - 1);
            isp_af_crop_vsize_write(be_reg, af_reg_be_cfg->crop_v_size - 1);
        }

        /* AF BE raw cfg */
        isp_af_sel_write(be_reg, af_reg_be_cfg->af_pos_sel);
        isp_af_raw_mode_write(be_reg, af_reg_be_cfg->raw_mode);
        isp_af_gain_lmt_write(be_reg, af_reg_be_cfg->gain_limit);
        isp_af_gamma_write(be_reg, af_reg_be_cfg->gamma);
        isp_af_bayer_mode_write(be_reg, af_reg_be_cfg->bayer_mode);
        isp_af_offset_en_write(be_reg, af_reg_be_cfg->offset_enable);
        isp_af_offset_gr_write(be_reg, af_reg_be_cfg->offset_gr);
        isp_af_offset_gb_write(be_reg, af_reg_be_cfg->offset_gb);

        /* AF BE pre median filter */
        isp_af_mean_en_write(be_reg, af_reg_be_cfg->mean_enable);
        isp_af_mean_thres_write(be_reg, 0xFFFF - af_reg_be_cfg->mean_thres);

        /* level depend gain */
        isp_af_iir0_ldg_en_write(be_reg, af_reg_be_cfg->iir0_ldg_enable);
        isp_af_iir_thre0_l_write(be_reg, af_reg_be_cfg->iir_thre0_low);
        isp_af_iir_thre0_h_write(be_reg, af_reg_be_cfg->iir_thre0_high);
        isp_af_iir_slope0_l_write(be_reg, af_reg_be_cfg->iir_slope0_low);
        isp_af_iir_slope0_h_write(be_reg, af_reg_be_cfg->iir_slope0_high);
        isp_af_iir_gain0_l_write(be_reg, af_reg_be_cfg->iir_gain0_low);
        isp_af_iir_gain0_h_write(be_reg, af_reg_be_cfg->iir_gain0_high);

        isp_af_iir1_ldg_en_write(be_reg, af_reg_be_cfg->iir1_ldg_enable);
        isp_af_iir_thre1_l_write(be_reg, af_reg_be_cfg->iir_thre1_low);
        isp_af_iir_thre1_h_write(be_reg, af_reg_be_cfg->iir_thre1_high);
        isp_af_iir_slope1_l_write(be_reg, af_reg_be_cfg->iir_slope1_low);
        isp_af_iir_slope1_h_write(be_reg, af_reg_be_cfg->iir_slope1_high);
        isp_af_iir_gain1_l_write(be_reg, af_reg_be_cfg->iir_gain1_low);
        isp_af_iir_gain1_h_write(be_reg, af_reg_be_cfg->iir_gain1_high);

        isp_af_fir0_ldg_en_write(be_reg, af_reg_be_cfg->fir0_ldg_enable);
        isp_af_fir_thre0_l_write(be_reg, af_reg_be_cfg->fir_thre0_low);
        isp_af_fir_thre0_h_write(be_reg, af_reg_be_cfg->fir_thre0_high);
        isp_af_fir_slope0_l_write(be_reg, af_reg_be_cfg->fir_slope0_low);
        isp_af_fir_slope0_h_write(be_reg, af_reg_be_cfg->fir_slope0_high);
        isp_af_fir_gain0_l_write(be_reg, af_reg_be_cfg->fir_gain0_low);
        isp_af_fir_gain0_h_write(be_reg, af_reg_be_cfg->fir_gain0_high);

        isp_af_fir1_ldg_en_write(be_reg, af_reg_be_cfg->fir1_ldg_enable);
        isp_af_fir_thre1_l_write(be_reg, af_reg_be_cfg->fir_thre1_low);
        isp_af_fir_thre1_h_write(be_reg, af_reg_be_cfg->fir_thre1_high);
        isp_af_fir_slope1_l_write(be_reg, af_reg_be_cfg->fir_slope1_low);
        isp_af_fir_slope1_h_write(be_reg, af_reg_be_cfg->fir_slope1_high);
        isp_af_fir_gain1_l_write(be_reg, af_reg_be_cfg->fir_gain1_low);
        isp_af_fir_gain1_h_write(be_reg, af_reg_be_cfg->fir_gain1_high);

        /* AF BE coring */
        isp_af_iir_thre0_c_write(be_reg, af_reg_be_cfg->iir_thre0_coring);
        isp_af_iir_slope0_c_write(be_reg, af_reg_be_cfg->iir_slope0_coring);
        isp_af_iir_peak0_c_write(be_reg, af_reg_be_cfg->iir_peak0_coring);

        isp_af_iir_thre1_c_write(be_reg, af_reg_be_cfg->iir_thre1_coring);
        isp_af_iir_slope1_c_write(be_reg, af_reg_be_cfg->iir_slope1_coring);
        isp_af_iir_peak1_c_write(be_reg, af_reg_be_cfg->iir_peak1_coring);

        isp_af_fir_thre0_c_write(be_reg, af_reg_be_cfg->fir_thre0_coring);
        isp_af_fir_slope0_c_write(be_reg, af_reg_be_cfg->fir_slope0_coring);
        isp_af_fir_peak0_c_write(be_reg, af_reg_be_cfg->fir_peak0_coring);

        isp_af_fir_thre1_c_write(be_reg, af_reg_be_cfg->fir_thre1_coring);
        isp_af_fir_slope1_c_write(be_reg, af_reg_be_cfg->fir_slope1_coring);
        isp_af_fir_peak1_c_write(be_reg, af_reg_be_cfg->fir_peak1_coring);

        /* high luma counter */
        isp_af_hilight_write(be_reg, af_reg_be_cfg->hiligh_thre);

        /* AF output shift */
        isp_af_acc_shift0_h_write(be_reg, af_reg_be_cfg->acc_shift0_h);
        isp_af_acc_shift1_h_write(be_reg, af_reg_be_cfg->acc_shift1_h);
        isp_af_acc_shift0_v_write(be_reg, af_reg_be_cfg->acc_shift0_v);
        isp_af_acc_shift1_v_write(be_reg, af_reg_be_cfg->acc_shift1_v);
        isp_af_acc_shift_y_write(be_reg, af_reg_be_cfg->acc_shift_y);
        isp_af_cnt_shift_y_write(be_reg, af_reg_be_cfg->shift_count_y);
        isp_af_cnt_shift0_v_write(be_reg, ISP_AF_CNT_SHIFT0_V_DEFAULT);
        isp_af_cnt_shift0_h_write(be_reg, 0x0);
        isp_af_cnt_shift1_h_write(be_reg, 0x0);
        isp_af_cnt_shift1_v_write(be_reg, 0x0);

        reg_cfg_info->cfg_key.bit1_af_be_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }
#endif

    return HI_SUCCESS;
}

static hi_s32 isp_fe_awb_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u32 i;
    hi_vi_pipe vi_pipe_bind;
    isp_awb_reg_dyn_cfg  *awb_reg_dyn_cfg = HI_NULL;
    isp_awb_reg_sta_cfg  *awb_reg_sta_cfg = HI_NULL;
    S_ISPFE_REGS_TYPE    *fe_reg  = HI_NULL;
    isp_usr_ctx          *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe != HI_TRUE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        isp_check_pipe_return(vi_pipe_bind);

        fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
        isp_check_pointer_return(fe_reg);

        if (isp_ctx->linkage.snap_state == HI_TRUE) {
            isp_wb1_gain_write(fe_reg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain);
        }

        if (reg_cfg_info->cfg_key.bit1_awb_dyn_cfg) {
            awb_reg_dyn_cfg = &reg_cfg_info->alg_reg_cfg[0].awb_reg_cfg.awb_reg_dyn_cfg;
            isp_wb1_gain_write(fe_reg, awb_reg_dyn_cfg->fe_white_balance_gain);
            isp_fe_wb1_en_write(fe_reg, awb_reg_dyn_cfg->fe_wb_work_en);
        }

        awb_reg_sta_cfg = &reg_cfg_info->alg_reg_cfg[0].awb_reg_cfg.awb_reg_sta_cfg;

        if (awb_reg_sta_cfg->fe_awb_sta_cfg) {
            awb_reg_sta_cfg = &reg_cfg_info->alg_reg_cfg[0].awb_reg_cfg.awb_reg_sta_cfg;
            isp_wb1_clip_value_write(fe_reg, awb_reg_sta_cfg->fe_clip_value);
        }
    }

    return HI_SUCCESS;
}

static hi_void isp_awb_cc_set(S_ISPBE_REGS_TYPE *be_reg, hi_u16 *be_cc)
{
    hi_u8 i;
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        if ((be_cc[i] >> 12) < 0x8) { /* valid bit 12, sign bit 0x8 */
            be_cc[i] = MIN2(be_cc[i], 0xFFF); /* max positive value 0xFFF */
        } else if ((be_cc[i] >> 12) >= 0x8) { /* valid bit 12, sign bit 0x8 */
            be_cc[i] = MIN2(be_cc[i], 0x8FFF); /* max negative value 0x8FFF */
        }
    }
    isp_cc_coef00_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[0]))); /* array index 0 */
    isp_cc_coef01_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[1]))); /* array index 1 */
    isp_cc_coef02_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[2]))); /* array index 2 */
    isp_cc_coef10_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[3]))); /* array index 3 */
    isp_cc_coef11_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[4]))); /* array index 4 */
    isp_cc_coef12_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[5]))); /* array index 5 */
    isp_cc_coef20_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[6]))); /* array index 6 */
    isp_cc_coef21_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[7]))); /* array index 7 */
    isp_cc_coef22_write(be_reg, ccm_convert(ccm_convert_pre(be_cc[8]))); /* array index 8 */
}

static hi_void isp_awb_gain_set(S_ISPBE_REGS_TYPE *be_reg, hi_u32 *be_wb_gain)
{
    isp_wb_rgain_write(be_reg, be_wb_gain[0]); /* array index 0 */
    isp_wb_grgain_write(be_reg, be_wb_gain[1]); /* array index 1 */
    isp_wb_gbgain_write(be_reg, be_wb_gain[2]); /* array index 2 */
    isp_wb_bgain_write(be_reg, be_wb_gain[3]); /* array index 3 */
}

static hi_s32 isp_awb_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    isp_awb_reg_dyn_cfg  *awb_reg_dyn_cfg = HI_NULL;
    isp_awb_reg_sta_cfg  *awb_reg_sta_cfg = HI_NULL;
    isp_awb_reg_usr_cfg  *awb_reg_usr_cfg = HI_NULL;
    S_VIPROC_REGS_TYPE   *vi_proc_reg     = HI_NULL;
    S_ISPBE_REGS_TYPE    *be_reg          = HI_NULL;
    S_ISPBE_REGS_TYPE    *be_reg_ex       = HI_NULL;

    hi_bool offline_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_bool idx_resh, usr_resh;

    isp_get_ctx(vi_pipe, isp_ctx);

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);
    isp_check_pointer_return(vi_proc_reg);

    awb_reg_dyn_cfg = &reg_cfg_info->alg_reg_cfg[i].awb_reg_cfg.awb_reg_dyn_cfg;
    awb_reg_sta_cfg = &reg_cfg_info->alg_reg_cfg[i].awb_reg_cfg.awb_reg_sta_cfg;
    awb_reg_usr_cfg = &reg_cfg_info->alg_reg_cfg[i].awb_reg_cfg.awb_reg_usr_cfg;

    if (isp_ctx->linkage.snap_state == HI_TRUE) {
        if (isp_ctx->linkage.load_ccm == HI_TRUE) {
            isp_awb_cc_set(be_reg, isp_ctx->snap_isp_info.isp_cfg_info.cap_ccm);
        } else {
            isp_awb_cc_set(be_reg, awb_reg_dyn_cfg->be_color_matrix);
        }

        isp_awb_gain_set(be_reg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain);
    }

    if (reg_cfg_info->cfg_key.bit1_awb_dyn_cfg) {
        if (isp_ctx->linkage.snap_state != HI_TRUE) {
            if (isp_ctx->linkage.snap_pipe_mode != ISP_SNAP_PICTURE) {
                isp_awb_cc_set(be_reg, awb_reg_dyn_cfg->be_color_matrix);
                isp_awb_gain_set(be_reg, awb_reg_dyn_cfg->be_white_balance_gain);
            }
        }

        if ((is_online_mode(isp_ctx->linkage.picture_running_mode) || \
             is_sidebyside_mode(isp_ctx->linkage.picture_running_mode)) && \
            (isp_ctx->linkage.snap_pipe_mode == ISP_SNAP_PREVIEW)) {
            isp_check_pipe_return(isp_ctx->linkage.picture_pipe_id);
            be_reg_ex = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(isp_ctx->linkage.picture_pipe_id, i);
            isp_check_pointer_return(be_reg_ex);
            isp_awb_cc_set(be_reg_ex, awb_reg_dyn_cfg->be_color_matrix);
            isp_awb_gain_set(be_reg_ex, awb_reg_dyn_cfg->be_white_balance_gain);
        }

        isp_awb_threshold_max_write(be_reg, awb_reg_dyn_cfg->be_metering_white_level_awb);
        isp_awb_threshold_min_write(be_reg, awb_reg_dyn_cfg->be_metering_black_level_awb);
        isp_awb_cr_ref_max_write(be_reg, awb_reg_dyn_cfg->be_metering_cr_ref_max_awb);
        isp_awb_cr_ref_min_write(be_reg, awb_reg_dyn_cfg->be_metering_cr_ref_min_awb);
        isp_awb_cb_ref_max_write(be_reg, awb_reg_dyn_cfg->be_metering_cb_ref_max_awb);
        isp_awb_cb_ref_min_write(be_reg, awb_reg_dyn_cfg->be_metering_cb_ref_min_awb);

        isp_cc_r_gain_write(be_reg, awb_reg_dyn_cfg->be_cc_r_gain);
        isp_cc_g_gain_write(be_reg, awb_reg_dyn_cfg->be_cc_g_gain);
        isp_cc_b_gain_write(be_reg, awb_reg_dyn_cfg->be_cc_b_gain);

        isp_cc_recover_en_write(be_reg, HI_ISP_CCM_RECOVER_EN_DEFAULT);

        isp_awb_crop_pos_x_write(be_reg, awb_reg_dyn_cfg->be_crop_pos_x);
        isp_awb_crop_pos_y_write(be_reg, awb_reg_dyn_cfg->be_crop_pos_y);
        isp_awb_crop_out_width_write(be_reg, awb_reg_dyn_cfg->be_crop_out_width - 1);
        isp_awb_crop_out_height_write(be_reg, awb_reg_dyn_cfg->be_crop_out_height - 1);
    }

    if (awb_reg_sta_cfg->be_awb_sta_cfg) {
        isp_awb_bitmove_write(be_reg, awb_reg_sta_cfg->be_awb_bitmove);
        isp_awb_en_write(vi_proc_reg, awb_reg_sta_cfg->be_awb_work_en);

        isp_awb_stat_raddr_write(be_reg, awb_reg_sta_cfg->be_awb_stat_raddr);

        isp_cc_prot_en_write(be_reg, HI_ISP_CCM_PROT_EN_DEFAULT);
        isp_cc_data_cc_thd0_write(be_reg, HI_ISP_CCM_CC_THD0_DEFAULT);
        isp_cc_data_cc_thd1_write(be_reg, HI_ISP_CCM_CC_THD1_DEFAULT);
        isp_cc_cc_prot_ratio_write(be_reg, HI_ISP_CCM_CC_PROT_RATIO_DEFAULT);
        isp_cc_data_rr_thd0_write(be_reg, HI_ISP_CCM_RR_THD0_DEFAULT);
        isp_cc_data_rr_thd1_write(be_reg, HI_ISP_CCM_RR_THD1_DEFAULT);
        isp_cc_data_gg_thd0_write(be_reg, HI_ISP_CCM_GG_THD0_DEFAULT);
        isp_cc_data_gg_thd1_write(be_reg, HI_ISP_CCM_GG_THD1_DEFAULT);
        isp_cc_data_bb_thd0_write(be_reg, HI_ISP_CCM_BB_THD0_DEFAULT);
        isp_cc_data_bb_thd1_write(be_reg, HI_ISP_CCM_BB_THD1_DEFAULT);
        isp_cc_max_rgb_thd_write(be_reg, HI_ISP_CCM_MAX_RGB_DEFAULT);
        isp_cc_rgb_prot_ratio_write(be_reg, HI_ISP_CCM_RGB_PROT_RATIO_DEFAULT);
        isp_cc_recover_en_write(be_reg, HI_ISP_CCM_RECOVER_EN_DEFAULT);
        isp_cc_lumin_ratio_write(be_reg, HI_ISP_CCM_LUM_RATIO_DEFAULT);
        isp_cc_hue_ratio_write(be_reg, HI_ISP_CCM_HUE_RATIO_DEFAULT);

        isp_cc_in_dc0_write(be_reg, awb_reg_sta_cfg->be_cc_in_dc0);
        isp_cc_in_dc1_write(be_reg, awb_reg_sta_cfg->be_cc_in_dc1);
        isp_cc_in_dc2_write(be_reg, awb_reg_sta_cfg->be_cc_in_dc2);
        isp_cc_out_dc0_write(be_reg, awb_reg_sta_cfg->be_cc_out_dc0);
        isp_cc_out_dc1_write(be_reg, awb_reg_sta_cfg->be_cc_out_dc1);
        isp_cc_out_dc2_write(be_reg, awb_reg_sta_cfg->be_cc_out_dc2);
        isp_wb_clip_value_write(be_reg, awb_reg_sta_cfg->be_wb_clip_value);
        isp_awb_offset_comp_write(be_reg, awb_reg_sta_cfg->be_awb_offset_comp);

        awb_reg_sta_cfg->be_awb_sta_cfg = 0;
    }

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    idx_resh = (isp_awb_update_index_read(be_reg) != awb_reg_usr_cfg->update_index);
    usr_resh = (offline_mode) ? (awb_reg_usr_cfg->resh && idx_resh) : (awb_reg_usr_cfg->resh);

    if (usr_resh) {
        isp_awb_update_index_write(be_reg, awb_reg_usr_cfg->update_index);

        isp_awb_sel_write(be_reg, awb_reg_usr_cfg->be_awb_switch);
        isp_awb_hnum_write(be_reg, awb_reg_usr_cfg->be_zone_col);
        isp_awb_vnum_write(be_reg, awb_reg_usr_cfg->be_zone_row);
        /* if online mode, resh=0; if offline mode, resh=1; but only index != will resh */
        awb_reg_usr_cfg->resh = offline_mode;
    }

    return HI_SUCCESS;
}

static hi_void isp_sharpen_dyna_reg_config(S_ISPBE_REGS_TYPE *be_reg,
                                           isp_sharpen_mpi_dyna_reg_cfg *mpi_dyna_reg_cfg,
                                           isp_sharpen_default_dyna_reg_cfg *def_dyna_reg_cfg)
{
    isp_sharpen_bendetailctrl_write    (be_reg, mpi_dyna_reg_cfg->en_detail_ctrl);
    isp_sharpen_osht_dtl_wgt_write     (be_reg, mpi_dyna_reg_cfg->detail_osht_amt);
    isp_sharpen_usht_dtl_wgt_write     (be_reg, mpi_dyna_reg_cfg->detail_usht_amt);
    isp_sharpen_detl_oshtmul_write     (be_reg, mpi_dyna_reg_cfg->detail_osht_mul);
    isp_sharpen_detl_ushtmul_write     (be_reg, mpi_dyna_reg_cfg->detail_usht_mul);
    isp_sharpen_oshtamt_write          (be_reg, mpi_dyna_reg_cfg->osht_amt);
    isp_sharpen_ushtamt_write          (be_reg, mpi_dyna_reg_cfg->usht_amt);
    isp_sharpen_benshtctrlbyvar_write  (be_reg, mpi_dyna_reg_cfg->en_sht_ctrl_by_var);
    isp_sharpen_shtbldrt_write         (be_reg, mpi_dyna_reg_cfg->sht_bld_rt);
    isp_sharpen_oshtvarthd1_write      (be_reg, mpi_dyna_reg_cfg->osht_var_thd1);
    isp_sharpen_ushtvarthd1_write      (be_reg, mpi_dyna_reg_cfg->usht_var_thd1);
    isp_sharpen_dirdiffsft_write       (be_reg, mpi_dyna_reg_cfg->dir_diff_sft);
    isp_sharpen_dirrt0_write           (be_reg, mpi_dyna_reg_cfg->dir_rt[0]);
    isp_sharpen_dirrt1_write           (be_reg, mpi_dyna_reg_cfg->dir_rt[1]);
    isp_sharpen_benlumactrl_write      (be_reg, mpi_dyna_reg_cfg->en_luma_ctrl);

    isp_sharpen_lumawgt0_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[0]); /* array index 0 */
    isp_sharpen_lumawgt1_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[1]); /* array index 1 */
    isp_sharpen_lumawgt2_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[2]); /* array index 2 */
    isp_sharpen_lumawgt3_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[3]); /* array index 3 */
    isp_sharpen_lumawgt4_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[4]); /* array index 4 */
    isp_sharpen_lumawgt5_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[5]); /* array index 5 */
    isp_sharpen_lumawgt6_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[6]); /* array index 6 */
    isp_sharpen_lumawgt7_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[7]); /* array index 7 */
    isp_sharpen_lumawgt8_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[8]); /* array index 8 */
    isp_sharpen_lumawgt9_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[9]); /* array index 9 */
    isp_sharpen_lumawgt10_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[10]); /* array index 10 */
    isp_sharpen_lumawgt11_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[11]); /* array index 11 */
    isp_sharpen_lumawgt12_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[12]); /* array index 12 */
    isp_sharpen_lumawgt13_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[13]); /* array index 13 */
    isp_sharpen_lumawgt14_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[14]); /* array index 14 */
    isp_sharpen_lumawgt15_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[15]); /* array index 15 */
    isp_sharpen_lumawgt16_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[16]); /* array index 16 */
    isp_sharpen_lumawgt17_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[17]); /* array index 17 */
    isp_sharpen_lumawgt18_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[18]); /* array index 18 */
    isp_sharpen_lumawgt19_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[19]); /* array index 19 */
    isp_sharpen_lumawgt20_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[20]); /* array index 20 */
    isp_sharpen_lumawgt21_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[21]); /* array index 21 */
    isp_sharpen_lumawgt22_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[22]); /* array index 22 */
    isp_sharpen_lumawgt23_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[23]); /* array index 23 */
    isp_sharpen_lumawgt24_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[24]); /* array index 24 */
    isp_sharpen_lumawgt25_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[25]); /* array index 25 */
    isp_sharpen_lumawgt26_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[26]); /* array index 26 */
    isp_sharpen_lumawgt27_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[27]); /* array index 27 */
    isp_sharpen_lumawgt28_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[28]); /* array index 28 */
    isp_sharpen_lumawgt29_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[29]); /* array index 29 */
    isp_sharpen_lumawgt30_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[30]); /* array index 30 */
    isp_sharpen_lumawgt31_write(be_reg, mpi_dyna_reg_cfg->luma_wgt[31]); /* array index 31 */

    isp_sharpen_oshtvarmul_write      (be_reg, mpi_dyna_reg_cfg->osht_var_mul);
    isp_sharpen_ushtvarmul_write      (be_reg, mpi_dyna_reg_cfg->usht_var_mul);
    isp_sharpen_benskinctrl_write     (be_reg, mpi_dyna_reg_cfg->en_skin_ctrl);
    isp_sharpen_skinedgewgt0_write    (be_reg, mpi_dyna_reg_cfg->skin_edge_wgt[0]); /* array index 0 */
    isp_sharpen_skinedgewgt1_write    (be_reg, mpi_dyna_reg_cfg->skin_edge_wgt[1]); /* array index 1 */
    isp_sharpen_skinedgemul_write     (be_reg, mpi_dyna_reg_cfg->skin_edge_mul);
    isp_sharpen_benchrctrl_write      (be_reg, mpi_dyna_reg_cfg->en_chr_ctrl);
    isp_sharpen_chrrmul_write         (be_reg, mpi_dyna_reg_cfg->chr_r_mul);
    isp_sharpen_chrbmul_write         (be_reg, mpi_dyna_reg_cfg->chr_b_mul);
    isp_sharpen_chrrgain_write        (be_reg, mpi_dyna_reg_cfg->chr_r_gain);
    isp_sharpen_chrbgain_write        (be_reg, mpi_dyna_reg_cfg->chr_b_gain);
    isp_sharpen_chrggain_write        (be_reg, mpi_dyna_reg_cfg->chr_g_gain);
    isp_sharpen_chrgmfgain_write      (be_reg, mpi_dyna_reg_cfg->chr_gmf_gain);
    isp_sharpen_chrgmfmul_write       (be_reg, mpi_dyna_reg_cfg->chr_gmf_mul);
    isp_sharpen_chrgmul_write         (be_reg, mpi_dyna_reg_cfg->chr_g_mul);
    isp_sharpen_osht_dtl_thd0_write   (be_reg, mpi_dyna_reg_cfg->detail_osht_thr[0]); /* array index 0 */
    isp_sharpen_osht_dtl_thd1_write   (be_reg, mpi_dyna_reg_cfg->detail_osht_thr[1]); /* array index 1 */
    isp_sharpen_usht_dtl_thd0_write   (be_reg, mpi_dyna_reg_cfg->detail_usht_thr[0]); /* array index 0 */
    isp_sharpen_usht_dtl_thd1_write   (be_reg, mpi_dyna_reg_cfg->detail_usht_thr[1]); /* array index 1 */
    isp_sharpen_skinmaxu_write        (be_reg, mpi_dyna_reg_cfg->skin_max_u);
    isp_sharpen_skinminu_write        (be_reg, mpi_dyna_reg_cfg->skin_min_u);
    isp_sharpen_skinmaxv_write        (be_reg, mpi_dyna_reg_cfg->skin_max_v);
    isp_sharpen_skinminv_write        (be_reg, mpi_dyna_reg_cfg->skin_min_v);
    isp_sharpen_omaxgain_write        (be_reg, mpi_dyna_reg_cfg->o_max_gain);
    isp_sharpen_umaxgain_write        (be_reg, mpi_dyna_reg_cfg->u_max_gain);

    isp_sharpen_mfthdsftd_write       (be_reg, def_dyna_reg_cfg->mf_thd_sft_d);
    isp_sharpen_mfthdselud_write      (be_reg, def_dyna_reg_cfg->mf_thd_sel_ud);
    isp_sharpen_mfthdsftud_write      (be_reg, def_dyna_reg_cfg->mf_thd_sft_ud);
    isp_sharpen_hfthdsftd_write       (be_reg, def_dyna_reg_cfg->hf_thd_sft_d);
    isp_sharpen_hfthdselud_write      (be_reg, def_dyna_reg_cfg->hf_thd_sel_ud);
    isp_sharpen_hfthdsftud_write      (be_reg, def_dyna_reg_cfg->hf_thd_sft_ud);
    isp_sharpen_dirvarsft_write       (be_reg, def_dyna_reg_cfg->dir_var_sft);
    isp_sharpen_range_thd0_write      (be_reg, def_dyna_reg_cfg->range_thd0);
    isp_sharpen_range_wgt0_write      (be_reg, def_dyna_reg_cfg->range_dir_wgt0);
    isp_sharpen_range_mul_write       (be_reg, def_dyna_reg_cfg->range_dir_mul);
    isp_sharpen_hfepsvar_thr0_write   (be_reg, def_dyna_reg_cfg->hf_eps_var_thr0);
    isp_sharpen_hfepsvar_thr1_write   (be_reg, def_dyna_reg_cfg->hf_eps_var_thr1);
    isp_sharpen_hfepsval1_write       (be_reg, def_dyna_reg_cfg->hf_eps_val1);
    isp_sharpen_hfepsvar_mul_write    (be_reg, def_dyna_reg_cfg->hf_eps_mul);
    isp_sharpen_oshtvarwgt0_write     (be_reg, def_dyna_reg_cfg->osht_var_wgt0);
    isp_sharpen_ushtvarwgt0_write     (be_reg, def_dyna_reg_cfg->usht_var_wgt0);
    isp_sharpen_oshtvardiffthd0_write (be_reg, def_dyna_reg_cfg->osht_var_diff_thd[0]); /* array index 0 */
    isp_sharpen_ushtvardiffthd0_write (be_reg, def_dyna_reg_cfg->usht_var_diff_thd[0]); /* array index 0 */
    isp_sharpen_selpixwgt_write       (be_reg, def_dyna_reg_cfg->sel_pix_wgt);
    isp_sharpen_oshtvardiffthd1_write (be_reg, def_dyna_reg_cfg->osht_var_diff_thd[1]); /* array index 1 */
    isp_sharpen_oshtvardiffwgt1_write (be_reg, def_dyna_reg_cfg->osht_var_diff_wgt1);
    isp_sharpen_ushtvardiffthd1_write (be_reg, def_dyna_reg_cfg->usht_var_diff_thd[1]); /* array index 1 */
    isp_sharpen_ushtvardiffwgt1_write (be_reg, def_dyna_reg_cfg->usht_var_diff_wgt1);
    isp_sharpen_oshtvardiffmul_write  (be_reg, def_dyna_reg_cfg->osht_var_diff_mul);
    isp_sharpen_ushtvardiffmul_write  (be_reg, def_dyna_reg_cfg->usht_var_diff_mul);
    isp_sharpen_rmfscale_write        (be_reg, def_dyna_reg_cfg->rmf_gain_scale);
    isp_sharpen_bmfscale_write        (be_reg, def_dyna_reg_cfg->bmf_gain_scale);
}

static hi_void isp_sharpen_static_reg_config(S_ISPBE_REGS_TYPE *be_reg, isp_sharpen_static_reg_cfg *static_reg_cfg)
{
    isp_sharpen_mfthdseld_write(be_reg, static_reg_cfg->mf_thd_sel_d);
    isp_sharpen_hfthdseld_write(be_reg, static_reg_cfg->hf_thd_sel_d);
    isp_sharpen_dirvarscale_write(be_reg, static_reg_cfg->dir_var_scale);
    isp_sharpen_dirrly0_write(be_reg, static_reg_cfg->dir_rly[0]);
    isp_sharpen_dirrly1_write(be_reg, static_reg_cfg->dir_rly[1]);
    isp_sharpen_range_thd1_write(be_reg, static_reg_cfg->range_thd1);
    isp_sharpen_range_wgt1_write(be_reg, static_reg_cfg->range_dir_wgt1);
    isp_sharpen_benhfeps_write(be_reg, static_reg_cfg->en_hf_eps);
    isp_sharpen_hfepsval0_write(be_reg, static_reg_cfg->hf_eps_val0);
    isp_sharpen_omaxchg_write(be_reg, static_reg_cfg->o_max_chg);
    isp_sharpen_umaxchg_write(be_reg, static_reg_cfg->u_max_chg);
    isp_sharpen_shtvarsft_write(be_reg, static_reg_cfg->sht_var_sft);
    isp_sharpen_oshtvarthd0_write(be_reg, static_reg_cfg->osht_var_thd0);
    isp_sharpen_ushtvarthd0_write(be_reg, static_reg_cfg->usht_var_thd0);
    isp_sharpen_oshtvardiffwgt0_write(be_reg, static_reg_cfg->osht_var_diff_wgt0);
    isp_sharpen_ushtvardiffwgt0_write(be_reg, static_reg_cfg->usht_var_diff_wgt0);
    isp_sharpen_oshtvarwgt1_write(be_reg, static_reg_cfg->osht_var_wgt1);
    isp_sharpen_ushtvarwgt1_write(be_reg, static_reg_cfg->usht_var_wgt1);
    isp_sharpen_lmtmf0_write(be_reg, static_reg_cfg->lmt_mf[0]); /* array index 0 */
    isp_sharpen_lmthf0_write(be_reg, static_reg_cfg->lmt_hf[0]); /* array index 0 */
    isp_sharpen_lmtmf1_write(be_reg, static_reg_cfg->lmt_mf[1]); /* array index 1 */
    isp_sharpen_lmthf1_write(be_reg, static_reg_cfg->lmt_hf[1]); /* array index 1 */
    isp_sharpen_lmtmf2_write(be_reg, static_reg_cfg->lmt_mf[2]); /* array index 2 */
    isp_sharpen_lmthf2_write(be_reg, static_reg_cfg->lmt_hf[2]); /* array index 2 */
    isp_sharpen_lmtmf3_write(be_reg, static_reg_cfg->lmt_mf[3]); /* array index 3 */
    isp_sharpen_lmthf3_write(be_reg, static_reg_cfg->lmt_hf[3]); /* array index 3 */
    isp_sharpen_lmtmf4_write(be_reg, static_reg_cfg->lmt_mf[4]); /* array index 4 */
    isp_sharpen_lmthf4_write(be_reg, static_reg_cfg->lmt_hf[4]); /* array index 4 */
    isp_sharpen_lmtmf5_write(be_reg, static_reg_cfg->lmt_mf[5]); /* array index 5 */
    isp_sharpen_lmthf5_write(be_reg, static_reg_cfg->lmt_hf[5]); /* array index 5 */
    isp_sharpen_lmtmf6_write(be_reg, static_reg_cfg->lmt_mf[6]); /* array index 6 */
    isp_sharpen_lmthf6_write(be_reg, static_reg_cfg->lmt_hf[6]); /* array index 6 */
    isp_sharpen_lmtmf7_write(be_reg, static_reg_cfg->lmt_mf[7]); /* array index 7 */
    isp_sharpen_lmthf7_write(be_reg, static_reg_cfg->lmt_hf[7]); /* array index 7 */

    isp_sharpen_skinsrcsel_write(be_reg, static_reg_cfg->skin_src_sel);

    isp_sharpen_skinedgesft_write(be_reg, static_reg_cfg->skin_edge_sft);

    isp_sharpen_skincntthd0_write(be_reg, static_reg_cfg->skin_cnt_thd[0]); /* array index 0 */
    isp_sharpen_skinedgethd0_write(be_reg, static_reg_cfg->skin_edge_thd[0]); /* array index 0 */
    isp_sharpen_skinaccumthd0_write(be_reg, static_reg_cfg->skin_accum_thd[0]); /* array index 0 */
    isp_sharpen_skinaccumwgt0_write(be_reg, static_reg_cfg->skin_accum_wgt[0]); /* array index 0 */
    isp_sharpen_skincntthd1_write(be_reg, static_reg_cfg->skin_cnt_thd[1]); /* array index 1 */
    isp_sharpen_skinedgethd1_write(be_reg, static_reg_cfg->skin_edge_thd[1]); /* array index 1 */
    isp_sharpen_skinaccumthd1_write(be_reg, static_reg_cfg->skin_accum_thd[1]); /* array index 1 */
    isp_sharpen_skinaccumwgt1_write(be_reg, static_reg_cfg->skin_accum_wgt[1]); /* array index 1 */

    isp_sharpen_chrrvarshift_write(be_reg, static_reg_cfg->chr_r_var_sft);
    isp_sharpen_chrrminu_write(be_reg, static_reg_cfg->rc_bmin);
    isp_sharpen_chrrmaxu_write(be_reg, static_reg_cfg->rc_bmax);
    isp_sharpen_chrrminv_write(be_reg, static_reg_cfg->rc_rmin);
    isp_sharpen_chrrmaxv_write(be_reg, static_reg_cfg->rc_rmax);
    isp_sharpen_chrrori0_write(be_reg, static_reg_cfg->chr_r_ori_cb);
    isp_sharpen_chrrori1_write(be_reg, static_reg_cfg->chr_r_ori_cr);
    isp_sharpen_chrrsft0_write(be_reg, static_reg_cfg->chr_r_sft[0]); /* array index 0 */
    isp_sharpen_chrrsft1_write(be_reg, static_reg_cfg->chr_r_sft[1]); /* array index 1 */
    isp_sharpen_chrrsft2_write(be_reg, static_reg_cfg->chr_r_sft[2]); /* array index 2 */
    isp_sharpen_chrrsft3_write(be_reg, static_reg_cfg->chr_r_sft[3]); /* array index 3 */
    isp_sharpen_chrrthd0_write(be_reg, static_reg_cfg->chr_r_thd[0]); /* array index 0 */
    isp_sharpen_chrrthd1_write(be_reg, static_reg_cfg->chr_r_thd[1]); /* array index 1 */

    isp_sharpen_chrgminu_write(be_reg, static_reg_cfg->gc_bmin);
    isp_sharpen_chrgmaxu_write(be_reg, static_reg_cfg->gc_bmax);
    isp_sharpen_chrgminv_write(be_reg, static_reg_cfg->gc_rmin);
    isp_sharpen_chrgmaxv_write(be_reg, static_reg_cfg->gc_rmax);
    isp_sharpen_chrgori0_write(be_reg, static_reg_cfg->chr_g_ori_cb);
    isp_sharpen_chrgori1_write(be_reg, static_reg_cfg->chr_g_ori_cr);
    isp_sharpen_chrgsft0_write(be_reg, static_reg_cfg->chr_g_sft[0]); /* array index 0 */
    isp_sharpen_chrgsft1_write(be_reg, static_reg_cfg->chr_g_sft[1]); /* array index 1 */
    isp_sharpen_chrgsft2_write(be_reg, static_reg_cfg->chr_g_sft[2]); /* array index 2 */
    isp_sharpen_chrgsft3_write(be_reg, static_reg_cfg->chr_g_sft[3]); /* array index 3 */
    isp_sharpen_chrgthd0_write(be_reg, static_reg_cfg->chr_g_thd[0]); /* array index 0 */
    isp_sharpen_chrgthd1_write(be_reg, static_reg_cfg->chr_g_thd[1]); /* array index 1 */

    isp_sharpen_chrbvarshift_write(be_reg, static_reg_cfg->chr_b_var_sft);
    isp_sharpen_chrbminu_write(be_reg, static_reg_cfg->bc_bmin);
    isp_sharpen_chrbmaxu_write(be_reg, static_reg_cfg->bc_bmax);
    isp_sharpen_chrbminv_write(be_reg, static_reg_cfg->bc_rmin);
    isp_sharpen_chrbmaxv_write(be_reg, static_reg_cfg->bc_rmax);
    isp_sharpen_chrbori0_write(be_reg, static_reg_cfg->chr_b_ori_cb);
    isp_sharpen_chrbori1_write(be_reg, static_reg_cfg->chr_b_ori_cr);
    isp_sharpen_chrbsft0_write(be_reg, static_reg_cfg->chr_b_sft[0]); /* array index 0 */
    isp_sharpen_chrbsft1_write(be_reg, static_reg_cfg->chr_b_sft[1]); /* array index 1 */
    isp_sharpen_chrbsft2_write(be_reg, static_reg_cfg->chr_b_sft[2]); /* array index 2 */
    isp_sharpen_chrbsft3_write(be_reg, static_reg_cfg->chr_b_sft[3]); /* array index 3 */
    isp_sharpen_chrbthd0_write(be_reg, static_reg_cfg->chr_b_thd[0]); /* array index 0 */
    isp_sharpen_chrbthd1_write(be_reg, static_reg_cfg->chr_b_thd[1]); /* array index 1 */

    isp_sharpen_shtnoisemax_write(be_reg, static_reg_cfg->sht_noise_max);
    isp_sharpen_shtnoisemin_write(be_reg, static_reg_cfg->sht_noise_min);
    isp_sharpen_max_var_clip_write(be_reg, static_reg_cfg->max_var_clip_min);

    isp_sharpen_lumasrcsel_write(be_reg, static_reg_cfg->luma_src_sel);
    isp_sharpen_skincntmul_write(be_reg, static_reg_cfg->skin_cnt_mul);
    isp_sharpen_skinaccummul_write(be_reg, static_reg_cfg->skin_accum_mul);

    /* filter */
    isp_sharpen_udlpfcoef0_write(be_reg, static_reg_cfg->lpf_coef_ud[0]); /* array index 0 */
    isp_sharpen_udlpfcoef1_write(be_reg, static_reg_cfg->lpf_coef_ud[1]); /* array index 1 */
    isp_sharpen_udlpfcoef2_write(be_reg, static_reg_cfg->lpf_coef_ud[2]); /* array index 2 */

    isp_sharpen_dlpfcoef0_write(be_reg, static_reg_cfg->lpf_coef_d[0]); /* array index 0 */
    isp_sharpen_dlpfcoef1_write(be_reg, static_reg_cfg->lpf_coef_d[1]); /* array index 1 */
    isp_sharpen_dlpfcoef2_write(be_reg, static_reg_cfg->lpf_coef_d[2]); /* array index 2 */

    isp_sharpen_udhsfcoef0_write(be_reg, static_reg_cfg->hsf_coef_ud[0]); /* array index 0 */
    isp_sharpen_udhsfcoef1_write(be_reg, static_reg_cfg->hsf_coef_ud[1]); /* array index 1 */
    isp_sharpen_udhsfcoef2_write(be_reg, static_reg_cfg->hsf_coef_ud[2]); /* array index 2 */

    isp_sharpen_dhsfcoef0_0_write(be_reg, static_reg_cfg->hsf_coef_d0[0]); /* array index 0 */
    isp_sharpen_dhsfcoef0_1_write(be_reg, static_reg_cfg->hsf_coef_d0[1]); /* array index 1 */
    isp_sharpen_dhsfcoef0_2_write(be_reg, static_reg_cfg->hsf_coef_d0[2]); /* array index 2 */
    isp_sharpen_dhsfcoef0_3_write(be_reg, static_reg_cfg->hsf_coef_d0[3]); /* array index 3 */
    isp_sharpen_dhsfcoef0_4_write(be_reg, static_reg_cfg->hsf_coef_d0[4]); /* array index 4 */
    isp_sharpen_dhsfcoef0_5_write(be_reg, static_reg_cfg->hsf_coef_d0[5]); /* array index 5 */
    isp_sharpen_dhsfcoef0_6_write(be_reg, static_reg_cfg->hsf_coef_d0[6]); /* array index 6 */
    isp_sharpen_dhsfcoef0_7_write(be_reg, static_reg_cfg->hsf_coef_d0[7]); /* array index 7 */
    isp_sharpen_dhsfcoef0_8_write(be_reg, static_reg_cfg->hsf_coef_d0[8]); /* array index 8 */
    isp_sharpen_dhsfcoef0_9_write(be_reg, static_reg_cfg->hsf_coef_d0[9]); /* array index 9 */
    isp_sharpen_dhsfcoef0_10_write(be_reg, static_reg_cfg->hsf_coef_d0[10]); /* array index 10 */
    isp_sharpen_dhsfcoef0_11_write(be_reg, static_reg_cfg->hsf_coef_d0[11]); /* array index 11 */
    isp_sharpen_dhsfcoef0_12_write(be_reg, static_reg_cfg->hsf_coef_d0[12]); /* array index 12 */

    isp_sharpen_dhsfcoef1_0_write(be_reg, static_reg_cfg->hsf_coef_d1[0]); /* array index 0 */
    isp_sharpen_dhsfcoef1_1_write(be_reg, static_reg_cfg->hsf_coef_d1[1]); /* array index 1 */
    isp_sharpen_dhsfcoef1_2_write(be_reg, static_reg_cfg->hsf_coef_d1[2]); /* array index 2 */
    isp_sharpen_dhsfcoef1_3_write(be_reg, static_reg_cfg->hsf_coef_d1[3]); /* array index 3 */
    isp_sharpen_dhsfcoef1_4_write(be_reg, static_reg_cfg->hsf_coef_d1[4]); /* array index 4 */
    isp_sharpen_dhsfcoef1_5_write(be_reg, static_reg_cfg->hsf_coef_d1[5]); /* array index 5 */
    isp_sharpen_dhsfcoef1_6_write(be_reg, static_reg_cfg->hsf_coef_d1[6]); /* array index 6 */
    isp_sharpen_dhsfcoef1_7_write(be_reg, static_reg_cfg->hsf_coef_d1[7]); /* array index 7 */
    isp_sharpen_dhsfcoef1_8_write(be_reg, static_reg_cfg->hsf_coef_d1[8]); /* array index 8 */
    isp_sharpen_dhsfcoef1_9_write(be_reg, static_reg_cfg->hsf_coef_d1[9]); /* array index 9 */
    isp_sharpen_dhsfcoef1_10_write(be_reg, static_reg_cfg->hsf_coef_d1[10]); /* array index 10 */
    isp_sharpen_dhsfcoef1_11_write(be_reg, static_reg_cfg->hsf_coef_d1[11]); /* array index 11 */
    isp_sharpen_dhsfcoef1_12_write(be_reg, static_reg_cfg->hsf_coef_d1[12]); /* array index 12 */

    isp_sharpen_dhsfcoef2_0_write(be_reg, static_reg_cfg->hsf_coef_d2[0]); /* array index 0 */
    isp_sharpen_dhsfcoef2_1_write(be_reg, static_reg_cfg->hsf_coef_d2[1]); /* array index 1 */
    isp_sharpen_dhsfcoef2_2_write(be_reg, static_reg_cfg->hsf_coef_d2[2]); /* array index 2 */
    isp_sharpen_dhsfcoef2_3_write(be_reg, static_reg_cfg->hsf_coef_d2[3]); /* array index 3 */
    isp_sharpen_dhsfcoef2_4_write(be_reg, static_reg_cfg->hsf_coef_d2[4]); /* array index 4 */
    isp_sharpen_dhsfcoef2_5_write(be_reg, static_reg_cfg->hsf_coef_d2[5]); /* array index 5 */
    isp_sharpen_dhsfcoef2_6_write(be_reg, static_reg_cfg->hsf_coef_d2[6]); /* array index 6 */
    isp_sharpen_dhsfcoef2_7_write(be_reg, static_reg_cfg->hsf_coef_d2[7]); /* array index 7 */
    isp_sharpen_dhsfcoef2_8_write(be_reg, static_reg_cfg->hsf_coef_d2[8]); /* array index 8 */
    isp_sharpen_dhsfcoef2_9_write(be_reg, static_reg_cfg->hsf_coef_d2[9]); /* array index 9 */
    isp_sharpen_dhsfcoef2_10_write(be_reg, static_reg_cfg->hsf_coef_d2[10]); /* array index 10 */
    isp_sharpen_dhsfcoef2_11_write(be_reg, static_reg_cfg->hsf_coef_d2[11]); /* array index 11 */
    isp_sharpen_dhsfcoef2_12_write(be_reg, static_reg_cfg->hsf_coef_d2[12]); /* array index 12 */

    isp_sharpen_dhsfcoef3_0_write(be_reg, static_reg_cfg->hsf_coef_d3[0]); /* array index 0 */
    isp_sharpen_dhsfcoef3_1_write(be_reg, static_reg_cfg->hsf_coef_d3[1]); /* array index 1 */
    isp_sharpen_dhsfcoef3_2_write(be_reg, static_reg_cfg->hsf_coef_d3[2]); /* array index 2 */
    isp_sharpen_dhsfcoef3_3_write(be_reg, static_reg_cfg->hsf_coef_d3[3]); /* array index 3 */
    isp_sharpen_dhsfcoef3_4_write(be_reg, static_reg_cfg->hsf_coef_d3[4]); /* array index 4 */
    isp_sharpen_dhsfcoef3_5_write(be_reg, static_reg_cfg->hsf_coef_d3[5]); /* array index 5 */
    isp_sharpen_dhsfcoef3_6_write(be_reg, static_reg_cfg->hsf_coef_d3[6]); /* array index 6 */
    isp_sharpen_dhsfcoef3_7_write(be_reg, static_reg_cfg->hsf_coef_d3[7]); /* array index 7 */
    isp_sharpen_dhsfcoef3_8_write(be_reg, static_reg_cfg->hsf_coef_d3[8]); /* array index 8 */
    isp_sharpen_dhsfcoef3_9_write(be_reg, static_reg_cfg->hsf_coef_d3[9]); /* array index 9 */
    isp_sharpen_dhsfcoef3_10_write(be_reg, static_reg_cfg->hsf_coef_d3[10]); /* array index 10 */
    isp_sharpen_dhsfcoef3_11_write(be_reg, static_reg_cfg->hsf_coef_d3[11]); /* array index 11 */
    isp_sharpen_dhsfcoef3_12_write(be_reg, static_reg_cfg->hsf_coef_d3[12]); /* array index 12 */

    isp_sharpen_udlpfsft_write(be_reg, static_reg_cfg->lpf_sft_ud);
    isp_sharpen_dlpfsft_write(be_reg, static_reg_cfg->lpf_sft_d);
    isp_sharpen_udhsfsft_write(be_reg, static_reg_cfg->hsf_sft_ud);
    isp_sharpen_dhsfsft_write(be_reg, static_reg_cfg->hsf_sft_d);

    isp_sharpen_ben8dir_sel_write(be_reg, static_reg_cfg->en_shp8_dir);

    /* shoot ctrl */
    isp_sharpen_hfgain_sft_write(be_reg, static_reg_cfg->hf_gain_sft);
    isp_sharpen_mfgain_sft_write(be_reg, static_reg_cfg->mf_gain_sft);
    isp_sharpen_benshtvar_sel_write(be_reg, static_reg_cfg->sht_var_sel);
    isp_sharpen_shtvar5x5_sft_write(be_reg, static_reg_cfg->sht_var5x5_sft);
    isp_sharpen_detailthd_sel_write(be_reg, static_reg_cfg->detail_thd_sel);
    isp_sharpen_dtl_thdsft_write(be_reg, static_reg_cfg->detail_thd_sft);
}

static hi_s32 isp_sharpen_usr_reg_config(hi_bool *cur_reg_new_en, hi_vi_pipe vi_pipe, hi_u8 i,
                                         S_ISPBE_REGS_TYPE *be_reg, isp_sharpen_mpi_dyna_reg_cfg *mpi_dyna_reg_cfg)
{
    hi_u8  buf_id;
    hi_s32 ret;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg   = HI_NULL;
    isp_usr_ctx         *isp_ctx          = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg      = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        if (mpi_dyna_reg_cfg->switch_mode != HI_TRUE) {
            /* online lut2stt regconfig */
            buf_id = mpi_dyna_reg_cfg->buf_id;
            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
            isp_check_pointer_return(be_lut_stt_reg);

            isp_sharpen_lut_wstt_write(be_lut_stt_reg, mpi_dyna_reg_cfg->mf_gain_d, mpi_dyna_reg_cfg->mf_gain_ud,
                                       mpi_dyna_reg_cfg->hf_gain_d, mpi_dyna_reg_cfg->hf_gain_ud);

            ret = isp_sharpen_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("ISP[%d] isp_sharpen_lut_wstt_addr_write failed\n", vi_pipe);
                return ret;
            }

            isp_sharpen_stt2lut_en_write(be_reg, HI_TRUE);
            isp_sharpen_stt2lut_regnew_write(be_reg, HI_TRUE);

            mpi_dyna_reg_cfg->buf_id = 1 - buf_id;

            *cur_reg_new_en = HI_TRUE;
        }
    } else {
        isp_sharpen_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, mpi_dyna_reg_cfg->mf_gain_d,
                                   mpi_dyna_reg_cfg->mf_gain_ud, mpi_dyna_reg_cfg->hf_gain_d,
                                   mpi_dyna_reg_cfg->hf_gain_ud);

        isp_sharpen_stt2lut_en_write(be_reg, HI_TRUE);
        isp_sharpen_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_sharpen_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_bool  lut_update = HI_FALSE;
    hi_bool  cur_reg_new_en = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_s32   ret;
    isp_sharpen_mpi_dyna_reg_cfg *mpi_dyna_reg_cfg = HI_NULL;
    isp_sharpen_static_reg_cfg   *static_reg_cfg   = HI_NULL;
    S_VIPROC_REGS_TYPE           *vi_proc_reg      = HI_NULL;
    S_ISPBE_REGS_TYPE            *be_reg           = HI_NULL;
    isp_usr_ctx                  *isp_ctx          = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_sharpen_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(vi_proc_reg);

        static_reg_cfg  = &reg_cfg_info->alg_reg_cfg[i].sharpen_reg_cfg.static_reg_cfg;
        mpi_dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_sharpen_lut_width_word_write(vi_proc_reg, HI_ISP_SHARPEN_LUT_WIDTH_WORD_DEFAULT);

            isp_sharpen_static_reg_config(be_reg, static_reg_cfg);
            static_reg_cfg->static_resh = HI_FALSE;
        }

        idx_resh = (isp_sharpen_update_index_read(be_reg) != mpi_dyna_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (mpi_dyna_reg_cfg->resh && idx_resh) : (mpi_dyna_reg_cfg->resh);

        if (usr_resh) {
            isp_sharpen_update_index_write(be_reg, mpi_dyna_reg_cfg->update_index);

            ret = isp_sharpen_usr_reg_config(&cur_reg_new_en, vi_pipe, i, be_reg, mpi_dyna_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            mpi_dyna_reg_cfg->resh = offline_mode;
        }

        if (reg_cfg_info->alg_reg_cfg[i].sharpen_reg_cfg.lut2_stt_en == HI_TRUE) {
            lut_update = mpi_dyna_reg_cfg->switch_mode ? (HI_FALSE) : (mpi_dyna_reg_cfg->pre_reg_new_en);
        }

        mpi_dyna_reg_cfg->pre_reg_new_en       = cur_reg_new_en;
        reg_cfg_info->cfg_key.bit1_sharpen_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.sharpen_lut_update = lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_edge_mark_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    hi_bool  offline_mode;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    isp_usr_ctx  *isp_ctx = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_edge_mark_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_sharpen_mark_thdsft_write (be_reg, reg_cfg_info->alg_reg_cfg[i].edge_mark_reg_cfg.mark_edge_sft);
        isp_sharpen_mark_udata_write (be_reg, reg_cfg_info->alg_reg_cfg[i].edge_mark_reg_cfg.u_mark_value);
        isp_sharpen_mark_vdata_write (be_reg, reg_cfg_info->alg_reg_cfg[i].edge_mark_reg_cfg.v_mark_value);
        isp_sharpen_mark_thd_write    (be_reg, reg_cfg_info->alg_reg_cfg[i].edge_mark_reg_cfg.mark_edge_thd);
    }

    reg_cfg_info->cfg_key.bit1_edge_mark_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
#endif

    return HI_SUCCESS;
}

static hi_s32 isp_hlc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    hi_bool  offline_mode;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    isp_usr_ctx *isp_ctx  = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_hlc_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_hlc_y_max_load_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.y_max_load_value);
        isp_hlc_y_max_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.y_max_value);
        isp_hlc_y_min_load_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.y_min_load_value);
        isp_hlc_y_min_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.y_min_value);
        isp_hlc_c_max_load_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.c_max_load_value);
        isp_hlc_c_max_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.c_max_value);
        isp_hlc_c_min_load_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.c_min_load_value);
        isp_hlc_c_min_value_write(be_reg, reg_cfg_info->alg_reg_cfg[i].hlc_reg_cfg.c_min_value);
    }

    reg_cfg_info->cfg_key.bit1_hlc_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
#endif

    return HI_SUCCESS;
}

static hi_void isp_demosaic_lut_reg_config(isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                           isp_demosaic_dyna_cfg *dyna_reg_cfg)
{
    hi_u16  j;

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_nddm_gf_lut_waddr_write(be_reg, 0);

        for (j = 0; j < HI_ISP_DEMOSAIC_LUT_LENGTH; j++) {
            isp_nddm_gf_lut_wdata_write(be_reg, dyna_reg_cfg->gf_blur_luts[j]);
        }
    } else {
        isp_nddm_gflut_write(be_reg, dyna_reg_cfg->gf_blur_luts);
    }
}

static hi_s32 isp_demosaic_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool offline_mode;
    hi_bool gf_lut_update  = HI_FALSE;
    hi_u8   blk_num        = reg_cfg_info->cfg_num;
    isp_demosaic_static_cfg *static_reg_cfg = HI_NULL;
    isp_demosaic_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    S_ISPBE_REGS_TYPE       *be_reg         = HI_NULL;
    isp_usr_ctx             *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_dem_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].dem_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].dem_reg_cfg.dyna_reg_cfg;

        if (static_reg_cfg->resh) {  /* static */
            isp_demosaic_desat_enable_write(be_reg, static_reg_cfg->de_sat_enable);
            isp_demosaic_g_intp_control_write(be_reg, static_reg_cfg->g_intp_ctrl_enable);
            isp_demosaic_ahd_en_write(be_reg, static_reg_cfg->ahd_enable);
            isp_demosaic_de_fake_en_write(be_reg, static_reg_cfg->de_fake_enable);
            isp_demosaic_lpf_f3_write(be_reg, static_reg_cfg->lpff3);
            isp_demosaic_desat_thresh1_write(be_reg, static_reg_cfg->de_sat_thresh1);
            isp_demosaic_desat_thresh2_write(be_reg, static_reg_cfg->de_sat_thresh2);
            isp_demosaic_desat_hig_write(be_reg, static_reg_cfg->de_sat_hig);
            isp_demosaic_desat_protect_sl_write(be_reg, static_reg_cfg->de_sat_prot_sl);
            isp_demosaic_bld_limit1_write(be_reg, static_reg_cfg->hv_blend_limit1);
            isp_demosaic_bld_limit2_write(be_reg, static_reg_cfg->hv_blend_limit2);
            isp_demosaic_ahd_par1_write(be_reg, static_reg_cfg->ahd_part1);
            isp_demosaic_ahd_par2_write(be_reg, static_reg_cfg->ahd_part2);
            isp_demosaic_cx_var_max_rate_write(be_reg, static_reg_cfg->cx_var_max_rate);
            isp_demosaic_cx_var_min_rate_write(be_reg, static_reg_cfg->cx_var_min_rate);
            isp_demosaic_g_clip_sft_bit_write(be_reg, static_reg_cfg->g_clip_bit_sft);
            isp_demosaic_hv_ratio_write(be_reg, static_reg_cfg->hv_color_ratio);
            isp_demosaic_hv_sel_write(be_reg, static_reg_cfg->hv_selection);
            isp_demosaic_cbcr_avg_thld_write(be_reg, static_reg_cfg->cb_cr_avg_thr);
            isp_nddm_dith_mask_write(be_reg, static_reg_cfg->dither_mask);
            isp_nddm_dith_ratio_write(be_reg, static_reg_cfg->dither_ratio);
            isp_nddm_gf_th_low_write(be_reg, static_reg_cfg->gf_th_low);
            isp_nddm_gf_th_high_write(be_reg, static_reg_cfg->gf_th_hig);
            isp_demosaic_hlc_thresh_write(be_reg, 0x100);

            reg_cfg_info->alg_reg_cfg[i].dem_reg_cfg.static_reg_cfg.resh = HI_FALSE;
        }

        if (dyna_reg_cfg->resh) {   /* dynamic */
            isp_demosaic_fake_cr_var_thr_high_write(be_reg, dyna_reg_cfg->fake_cr_var_thr_high);
            isp_demosaic_fake_cr_var_thr_low_write(be_reg, dyna_reg_cfg->fake_cr_var_thr_low);
            isp_demosaic_defcolor_cr_write(be_reg, dyna_reg_cfg->de_f_color_cr);
            isp_demosaic_defcolor_cb_write(be_reg, dyna_reg_cfg->de_f_color_cb);
            isp_demosaic_cc_hf_max_ratio_write(be_reg, dyna_reg_cfg->cc_hf_max_ratio);
            isp_demosaic_cc_hf_min_ratio_write(be_reg, dyna_reg_cfg->cc_hf_min_ratio);
            isp_demosaic_lpf_f0_write(be_reg, dyna_reg_cfg->lpff0);
            isp_demosaic_lpf_f1_write(be_reg, dyna_reg_cfg->lpff1);
            isp_demosaic_lpf_f2_write(be_reg, dyna_reg_cfg->lpff2);
            isp_demosaic_desat_low_write(be_reg, dyna_reg_cfg->de_sat_low);
            isp_demosaic_desat_protect_th_write(be_reg, dyna_reg_cfg->de_sat_prot_th);
            isp_nddm_dith_max_write(be_reg, dyna_reg_cfg->dither_max);
            isp_nddm_fcr_gf_gain_write(be_reg, dyna_reg_cfg->fcr_gf_gain);
            isp_nddm_awb_gf_gn_low_write(be_reg, dyna_reg_cfg->awb_gf_gain_low);
            isp_nddm_awb_gf_gn_high_write(be_reg, dyna_reg_cfg->awb_gf_gain_hig);
            isp_nddm_awb_gf_gn_max_write(be_reg, dyna_reg_cfg->awb_gf_gain_max);
            isp_nddm_ehc_gray_write(be_reg, dyna_reg_cfg->ehc_gray);
            isp_demosaic_hf_intp_blur_th1_write(be_reg, dyna_reg_cfg->hf_intp_blur_th_low);
            isp_demosaic_hf_intp_blur_th2_write(be_reg, dyna_reg_cfg->hf_intp_blur_th_hig);
            isp_nddm_fcr_det_low_write(be_reg, dyna_reg_cfg->fcr_det_low);

            isp_demosaic_lut_reg_config(isp_ctx, be_reg, dyna_reg_cfg);

            isp_cc_prot_en_write(be_reg, dyna_reg_cfg->pro_dark_region.ccm_prot_en);
            isp_cc_data_cc_thd0_write(be_reg, dyna_reg_cfg->pro_dark_region.data_cc_thd0);
            isp_cc_data_cc_thd1_write(be_reg, dyna_reg_cfg->pro_dark_region.data_cc_thd1);
            isp_cc_cc_prot_ratio_write(be_reg, dyna_reg_cfg->pro_dark_region.cc_prot_ratio);
            isp_cc_data_rr_thd0_write(be_reg, dyna_reg_cfg->pro_dark_region.data_rr_thd0);
            isp_cc_data_rr_thd1_write(be_reg, dyna_reg_cfg->pro_dark_region.data_rr_thd1);
            isp_cc_data_gg_thd0_write(be_reg, dyna_reg_cfg->pro_dark_region.data_gg_thd0);
            isp_cc_data_gg_thd1_write(be_reg, dyna_reg_cfg->pro_dark_region.data_gg_thd1);
            isp_cc_data_bb_thd0_write(be_reg, dyna_reg_cfg->pro_dark_region.data_bb_thd0);
            isp_cc_data_bb_thd1_write(be_reg, dyna_reg_cfg->pro_dark_region.data_bb_thd1);
            isp_cc_max_rgb_thd_write(be_reg, dyna_reg_cfg->pro_dark_region.max_rgb_thd);
            isp_cc_rgb_prot_ratio_write(be_reg, dyna_reg_cfg->pro_dark_region.rgb_prot_ratio);

            gf_lut_update      = dyna_reg_cfg->update_gf;
            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_dem_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.nddm_gf_lut_update  = gf_lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_fpn_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_VI_FPN_SUPPORT
    isp_fpn_dyna_cfg *dyna_reg_cfg = HI_NULL;

    if (reg_cfg_info->cfg_key.bit1_fpn_cfg) {
        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].fpn_reg_cfg.dyna_reg_cfg;

        isp_fpn_overflowthr_write(vi_pipe, i, dyna_reg_cfg->isp_fpn_overflow_thr);
        isp_fpn_strength0_write(vi_pipe, i, dyna_reg_cfg->isp_fpn_strength[0]);

        reg_cfg_info->cfg_key.bit1_fpn_cfg = 0;
    }
#endif
    return HI_SUCCESS;
}

static hi_s32 isp_ldci_read_stt_addr_write(S_VIPROC_REGS_TYPE *vi_proc_reg, hi_u64 phy_addr)
{
    if (phy_addr == 0) {
        return HI_FAILURE;
    }

    viproc_para_dci_addr_low_write(vi_proc_reg, get_low_addr(phy_addr));

    return HI_SUCCESS;
}

static hi_s32 isp_ldci_static_lut_reg_config(hi_vi_pipe vi_pipe, hi_u8 i, S_ISPBE_REGS_TYPE *be_reg,
                                             isp_ldci_static_cfg *static_reg_cfg, isp_ldci_dyna_cfg *dyna_reg_cfg)
{
    hi_u16  j, k;
    isp_usr_ctx         *isp_ctx        = HI_NULL;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_ldci_poply_waddr_write(be_reg, 0);
        isp_ldci_plyq01_waddr_write(be_reg, 0);
        isp_ldci_plyq23_waddr_write(be_reg, 0);

        for (j = 0; j < 65; j++) { /* ldci lut table number 65 */
            isp_ldci_poply_wdata_write(be_reg, static_reg_cfg->poly_p1[j],
                                       static_reg_cfg->stat_poly_p2[j], static_reg_cfg->stat_poly_p3[j]);
            isp_ldci_plyq01_wdata_write(be_reg, static_reg_cfg->poly_q0[j], static_reg_cfg->poly_q1[j]);
            isp_ldci_plyq23_wdata_write(be_reg, static_reg_cfg->poly_q2[j], static_reg_cfg->poly_q3[j]);
        }

        for (k = 0; k < 2; k++) {  /* config all 2 lut2stt buffer for static lut */
            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, k);
            isp_check_pointer_return(be_lut_stt_reg);
            isp_ldci_drc_cgain_lut_wstt_write(be_lut_stt_reg, static_reg_cfg->calc_dyn_rng_cmp_lut,
                                              static_reg_cfg->stat_dyn_rng_cmp_lut, dyna_reg_cfg->color_gain_lut);
            isp_ldci_usm_lut_wstt_write(be_lut_stt_reg, dyna_reg_cfg->usm_pos_lut, dyna_reg_cfg->usm_neg_lut);
        }
    } else {
        isp_ldci_poply1_wlut_write(be_reg, static_reg_cfg->poly_p1);
        isp_ldci_poply2_wlut_write(be_reg, static_reg_cfg->stat_poly_p2);
        isp_ldci_poply3_wlut_write(be_reg, static_reg_cfg->stat_poly_p3);
        isp_ldci_plyq0_wlut_write(be_reg, static_reg_cfg->poly_q0);
        isp_ldci_plyq1_wlut_write(be_reg, static_reg_cfg->poly_q1);
        isp_ldci_plyq2_wlut_write(be_reg, static_reg_cfg->poly_q2);
        isp_ldci_plyq3_wlut_write(be_reg, static_reg_cfg->poly_q3);

        isp_ldci_drc_cgain_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, static_reg_cfg->calc_dyn_rng_cmp_lut,
                                          static_reg_cfg->stat_dyn_rng_cmp_lut, dyna_reg_cfg->color_gain_lut);
        isp_ldci_usm_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, dyna_reg_cfg->usm_pos_lut,
                                    dyna_reg_cfg->usm_neg_lut);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_ldci_tpr_flt_attr_config(hi_vi_pipe vi_pipe, hi_u8 i, S_ISPBE_REGS_TYPE *be_reg,
                                           S_VIPROC_REGS_TYPE *vi_proc_reg, isp_ldci_dyna_cfg *dyna_reg_cfg)
{
    hi_bool rdstat_en;
    hi_u8   read_buf_idx;
    hi_u8   blk_num;
    hi_s32  ret;
    isp_usr_ctx           *isp_ctx           = HI_NULL;
    isp_ldci_read_stt_buf *ldci_read_stt_buf = HI_NULL;
    isp_ldci_stat         *read_stt_buf      = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    blk_num = isp_ctx->block_attr.block_num;

    if (isp_ctx->ldci_tpr_flt_en == HI_TRUE) {
        ldci_buf_get_ctx(vi_pipe, ldci_read_stt_buf);
        read_buf_idx = ldci_read_stt_buf->buf_idx;

        if (i == 0) {
            read_stt_buf = (isp_ldci_stat *)isp_get_ldci_read_stt_vir_addr(vi_pipe, read_buf_idx);
            isp_check_pointer_return(read_stt_buf);
            memcpy_s(read_stt_buf, sizeof(isp_ldci_stat), &dyna_reg_cfg->tpr_stat, sizeof(isp_ldci_stat));
        }

        if ((i + 1) == blk_num) {
            ldci_read_stt_buf->buf_idx = (read_buf_idx + 1) % div_0_to_1(ldci_read_stt_buf->buf_num);
        }

        /* Set ReadStt Addr */
        ret = isp_ldci_read_stt_addr_write(vi_proc_reg, ldci_read_stt_buf->read_buf[read_buf_idx].phy_addr);
        rdstat_en = (ret == HI_SUCCESS) ? (dyna_reg_cfg->rdstat_en) : (HI_FALSE);
        isp_ldci_rdstat_en_write(be_reg, rdstat_en);
    } else {
        isp_ldci_rdstat_en_write(be_reg, dyna_reg_cfg->rdstat_en);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_ldci_dyna_lut_reg_config(hi_vi_pipe vi_pipe, hi_u8 i, S_ISPBE_REGS_TYPE *be_reg,
                                           S_VIPROC_REGS_TYPE  *vi_proc_reg, isp_ldci_dyna_cfg *dyna_reg_cfg)
{
    hi_u8   buf_id;
    hi_s32  ret;
    isp_usr_ctx         *isp_ctx        = HI_NULL;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        /* online Lut2stt regconfig */
        buf_id = dyna_reg_cfg->buf_id;

        be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
        isp_check_pointer_return(be_lut_stt_reg);

        isp_ldci_he_delut_lut_wstt_write(be_lut_stt_reg, dyna_reg_cfg->de_lut, dyna_reg_cfg->he_pos_lut,
                                         dyna_reg_cfg->he_neg_lut);
        ret = isp_ldci_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
        if (ret != HI_SUCCESS) {
            isp_err_trace("isp[%d]_ldci_lut_wstt_addr_write failed\n", vi_pipe);
            return ret;
        }
        dyna_reg_cfg->buf_id = 1 - buf_id;
    } else {
        isp_ldci_he_delut_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, dyna_reg_cfg->de_lut,
                                         dyna_reg_cfg->he_pos_lut, dyna_reg_cfg->he_neg_lut);
    }

    isp_ldci_stt2lut_en_write(be_reg, HI_TRUE);
    isp_ldci_stt2lut_regnew_write(be_reg, HI_TRUE);

    return HI_SUCCESS;
}

static hi_s32 isp_ldci_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool offline_mode;
    hi_bool ldci_drc_lut_update = HI_FALSE;
    hi_bool cur_ldci_drc_lut_reg_new = HI_FALSE;
    hi_bool ldci_calc_lut_update = HI_FALSE;
    hi_bool cur_ldci_calc_lut_reg_new;
    hi_u8   blk_num = reg_cfg_info->cfg_num;
    hi_s32  ret;
    isp_ldci_static_cfg *static_reg_cfg = HI_NULL;
    isp_ldci_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE   *be_reg         = HI_NULL;
    isp_usr_ctx         *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_ldci_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(vi_proc_reg);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].ldci_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].ldci_reg_cfg.dyna_reg_cfg;

        if (static_reg_cfg->static_resh) {
            /* static */
            isp_ldci_lut_width_word_write(vi_proc_reg, HI_ISP_LDCI_LUT_WIDTH_WORD_DEFAULT);

            isp_ldci_stat_evratio_write(be_reg, 0x1000);
            isp_ldci_luma_sel_write(be_reg, static_reg_cfg->calc_luma_sel);
            isp_ldci_bende_write(be_reg, static_reg_cfg->de_enable);
            isp_ldci_deref_write(be_reg, static_reg_cfg->calc_de_ref);
            isp_ldci_deh_lpfsft_write(be_reg, static_reg_cfg->deh_lpf_sft);
            isp_ldci_coefh0_write(be_reg, static_reg_cfg->deh_lpf_coef_h[0]); /* array index 0 */
            isp_ldci_coefh1_write(be_reg, static_reg_cfg->deh_lpf_coef_h[1]); /* array index 1 */
            isp_ldci_coefh2_write(be_reg, static_reg_cfg->deh_lpf_coef_h[2]); /* array index 2 */
            isp_ldci_coefv0_write(be_reg, static_reg_cfg->deh_lpf_coef_v[0]); /* array index 0 */
            isp_ldci_coefv1_write(be_reg, static_reg_cfg->deh_lpf_coef_v[1]); /* array index 1 */
            isp_ldci_calc_drcen_write(be_reg, static_reg_cfg->calc_drc_enable);
            isp_ldci_pflmt_en_write(be_reg, static_reg_cfg->calc_pf_lmt_enable);
            isp_ldci_pfori0_write(be_reg, static_reg_cfg->calc_pf_ori[0]); /* array index 0 */
            isp_ldci_pfori1_write(be_reg, static_reg_cfg->calc_pf_ori[1]); /* array index 1 */
            isp_ldci_pfsft0_write(be_reg, static_reg_cfg->calc_pf_sft[0]); /* array index 0 */
            isp_ldci_pfsft1_write(be_reg, static_reg_cfg->calc_pf_sft[1]); /* array index 1 */
            isp_ldci_pfsft2_write(be_reg, static_reg_cfg->calc_pf_sft[2]); /* array index 2 */
            isp_ldci_pfsft3_write(be_reg, static_reg_cfg->calc_pf_sft[3]); /* array index 3 */
            isp_ldci_pfthd0_write(be_reg, static_reg_cfg->calc_pf_thd[0]); /* array index 0 */
            isp_ldci_pfthd1_write(be_reg, static_reg_cfg->calc_pf_thd[1]); /* array index 1 */
            isp_ldci_pfrly0_write(be_reg, static_reg_cfg->calc_pf_rly[0]); /* array index 0 */
            isp_ldci_pfrly1_write(be_reg, static_reg_cfg->calc_pf_rly[1]); /* array index 1 */
            isp_ldci_pfmul_write(be_reg, static_reg_cfg->calc_pfmul);
            isp_ldci_lpfsft_write(be_reg, static_reg_cfg->lpf_sft);
            isp_ldci_stat_drcen_write(be_reg, static_reg_cfg->stat_drc_enable);
            isp_ldci_chrposdamp_write(be_reg, static_reg_cfg->chr_pos_damp);
            isp_ldci_chrnegdamp_write(be_reg, static_reg_cfg->chr_neg_damp);
            isp_ldci_glb_hewgt_write(be_reg, static_reg_cfg->global_he_wgt);

            ret = isp_ldci_static_lut_reg_config(vi_pipe, i, be_reg, static_reg_cfg, dyna_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            cur_ldci_drc_lut_reg_new    = HI_TRUE;
            static_reg_cfg->static_resh = HI_FALSE;
        }

        ldci_drc_lut_update = static_reg_cfg->pre_drc_lut_update;
        static_reg_cfg->pre_drc_lut_update = cur_ldci_drc_lut_reg_new;

        /* dynamic */
        ret = isp_ldci_tpr_flt_attr_config(vi_pipe, i, be_reg, vi_proc_reg, dyna_reg_cfg);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        isp_ldci_en_write(vi_proc_reg, dyna_reg_cfg->enable);
        isp_ldci_wrstat_en_write(be_reg, dyna_reg_cfg->wrstat_en);
        isp_ldci_calc_en_write(be_reg, dyna_reg_cfg->calc_enable);
        isp_ldci_lpfcoef0_write(be_reg, dyna_reg_cfg->lpf_coef[0]); /* array index 0 */
        isp_ldci_lpfcoef1_write(be_reg, dyna_reg_cfg->lpf_coef[1]); /* array index 1 */
        isp_ldci_lpfcoef2_write(be_reg, dyna_reg_cfg->lpf_coef[2]); /* array index 2 */
        isp_ldci_lpfcoef3_write(be_reg, dyna_reg_cfg->lpf_coef[3]); /* array index 3 */
        isp_ldci_lpfcoef4_write(be_reg, dyna_reg_cfg->lpf_coef[4]); /* array index 4 */
        isp_ldci_lpfcoef5_write(be_reg, dyna_reg_cfg->lpf_coef[5]); /* array index 5 */
        isp_ldci_lpfcoef6_write(be_reg, dyna_reg_cfg->lpf_coef[6]); /* array index 6 */
        isp_ldci_lpfcoef7_write(be_reg, dyna_reg_cfg->lpf_coef[7]); /* array index 7 */
        isp_ldci_lpfcoef8_write(be_reg, dyna_reg_cfg->lpf_coef[8]); /* array index 8 */
        isp_ldci_calc_map_offsetx_write(be_reg, dyna_reg_cfg->calc_map_offset_x);
        isp_ldci_smlmapstride_write(be_reg, dyna_reg_cfg->calc_sml_map_stride);
        isp_ldci_smlmapheight_write(be_reg, dyna_reg_cfg->calc_sml_map_height);
        isp_ldci_total_zone_write(be_reg, dyna_reg_cfg->calc_total_zone);
        isp_ldci_scalex_write(be_reg, dyna_reg_cfg->calc_scale_x);
        isp_ldci_scaley_write(be_reg, dyna_reg_cfg->calc_scale_y);
        isp_ldci_stat_smlmapwidth_write(be_reg, dyna_reg_cfg->stat_sml_map_width);
        isp_ldci_stat_smlmapheight_write(be_reg, dyna_reg_cfg->stat_sml_map_height);
        isp_ldci_stat_total_zone_write(be_reg, dyna_reg_cfg->stat_total_zone);
        isp_ldci_blk_smlmapwidth0_write(be_reg, dyna_reg_cfg->blk_sml_map_width[0]); /* array index 0 */
        isp_ldci_blk_smlmapwidth1_write(be_reg, dyna_reg_cfg->blk_sml_map_width[1]); /* array index 1 */
        isp_ldci_blk_smlmapwidth2_write(be_reg, dyna_reg_cfg->blk_sml_map_width[2]); /* array index 2 */
        isp_ldci_hstart_write(be_reg, dyna_reg_cfg->stat_h_start);
        isp_ldci_hend_write(be_reg, dyna_reg_cfg->stat_h_end);
        isp_ldci_vstart_write(be_reg, dyna_reg_cfg->stat_v_start);
        isp_ldci_vend_write(be_reg, dyna_reg_cfg->stat_v_end);

        ret = isp_ldci_dyna_lut_reg_config(vi_pipe, i, be_reg, vi_proc_reg, dyna_reg_cfg);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        cur_ldci_calc_lut_reg_new = HI_TRUE;
        ldci_calc_lut_update = dyna_reg_cfg->pre_calc_lut_reg_new;
        dyna_reg_cfg->pre_calc_lut_reg_new = cur_ldci_calc_lut_reg_new;

        reg_cfg_info->cfg_key.bit1_ldci_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.ldci_calc_lut_update = ldci_calc_lut_update || offline_mode;
    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.ldci_drc_lut_update  = ldci_drc_lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_void isp_lcac_lut_reg_config(hi_bool *lut_update, isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                       isp_local_cac_static_cfg *static_reg_cfg)
{
    hi_u8 j;

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_demosaic_depurplut_waddr_write(be_reg, 0);

        for (j = 0; j < LCAC_STRENGTH_NUM; j++) {
            isp_demosaic_depurplut_wdata_write(be_reg, static_reg_cfg->de_purple_str[j]);
        }

        *lut_update = HI_TRUE;
    } else {
        isp_demosaic_depurp_lut_write(be_reg, static_reg_cfg->de_purple_str);
    }
}

static hi_s32 isp_lcac_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  idx_resh;
    hi_bool  usr_resh;
    hi_bool  lut_update = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    isp_local_cac_usr_cfg    *usr_reg_cfg    = HI_NULL;
    isp_local_cac_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_local_cac_static_cfg *static_reg_cfg = HI_NULL;
    S_ISPBE_REGS_TYPE        *be_reg         = HI_NULL;
    isp_usr_ctx              *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_local_cac_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].lcac_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_nddm_cac_blend_en_write(be_reg, static_reg_cfg->nddm_cac_blend_en);
            isp_nddm_cac_blend_rate_write(be_reg, static_reg_cfg->nddm_cac_blend_rate);
            isp_demosaic_r_counter_thr_write(be_reg, static_reg_cfg->r_counter_thr);
            isp_demosaic_g_counter_thr_write(be_reg, static_reg_cfg->g_counter_thr);
            isp_demosaic_b_counter_thr_write(be_reg, static_reg_cfg->b_counter_thr);
            isp_demosaic_satu_thr_write(be_reg, static_reg_cfg->satu_thr);
            isp_lcac_lut_reg_config(&lut_update, isp_ctx, be_reg, static_reg_cfg);

            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* Usr */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].lcac_reg_cfg.usr_reg_cfg;
        idx_resh = (isp_lcac_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->resh && idx_resh) : (usr_reg_cfg->resh);

        if (usr_resh) {
            isp_lcac_update_index_write(be_reg, usr_reg_cfg->update_index);

            isp_demosaic_purple_var_thr_write(be_reg, usr_reg_cfg->var_thr);
            isp_demosaic_cb_thr_write(be_reg, usr_reg_cfg->cb_thr);
            isp_demosaic_cac_luma_high_cnt_thr_write(be_reg, usr_reg_cfg->luma_high_cnt_thr);
            isp_demosaic_cac_cb_cnt_low_thr_write(be_reg, usr_reg_cfg->cb_cnt_low_thr);
            isp_demosaic_cac_cb_cnt_high_thr_write(be_reg, usr_reg_cfg->cb_cnt_high_thr);
            isp_demosaci_cac_bld_avg_cur_write(be_reg, usr_reg_cfg->bld_avg_cur);
            isp_demosaic_cbcr_ratio_high_limit_write(be_reg, usr_reg_cfg->cb_cr_ratio_lmt_high);

            usr_reg_cfg->resh = offline_mode;
        }

        /* dynamic */
        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].lcac_reg_cfg.dyna_reg_cfg;

        if (dyna_reg_cfg->resh) {
            isp_demosaic_r_luma_thr_write(be_reg, dyna_reg_cfg->r_luma_thr);
            isp_demosaic_g_luma_thr_write(be_reg, dyna_reg_cfg->g_luma_thr);
            isp_demosaic_b_luma_thr_write(be_reg, dyna_reg_cfg->b_luma_thr);
            isp_demosaic_luma_thr_write(be_reg, dyna_reg_cfg->luma_thr);
            isp_demosaic_cbcr_ratio_low_limit_write(be_reg, dyna_reg_cfg->cb_cr_ratio_lmt_low);
            isp_demosaic_depurplectr_cr_write(be_reg, dyna_reg_cfg->de_purple_ctr_cr);
            isp_demosaic_depurplectr_cb_write(be_reg, dyna_reg_cfg->de_purple_ctr_cb);
            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_local_cac_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.lcac_lut_update = lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_fcr_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8 blk_num = reg_cfg_info->cfg_num;
    hi_bool offline_mode;
    isp_antifalsecolor_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_antifalsecolor_static_cfg *static_reg_cfg = HI_NULL;
    S_ISPBE_REGS_TYPE             *be_reg         = HI_NULL;
    isp_usr_ctx                   *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_fcr_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].anti_false_color_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].anti_false_color_reg_cfg.dyna_reg_cfg;

        /* static */
        if (static_reg_cfg->resh) {
            isp_demosaic_fcr_limit1_write(be_reg, static_reg_cfg->fcr_limit1);
            isp_demosaic_fcr_limit2_write(be_reg, static_reg_cfg->fcr_limit2);
            isp_demosaic_fcr_thr_write(be_reg, static_reg_cfg->fcr_thr);
            static_reg_cfg->resh = HI_FALSE;
        }

        /* dynamic */
        if (dyna_reg_cfg->resh) {
            isp_demosaic_fcr_gain_write(be_reg, dyna_reg_cfg->fcr_gain);
            isp_demosaic_fcr_ratio_write(be_reg, dyna_reg_cfg->fcr_ratio);
            isp_demosaic_fcr_gray_ratio_write(be_reg, dyna_reg_cfg->fcr_gray_ratio);
            isp_demosaic_fcr_cmax_sel_write(be_reg, dyna_reg_cfg->fcr_cmax_sel);
            isp_demosaic_fcr_detg_sel_write(be_reg, dyna_reg_cfg->fcr_detg_sel);
            isp_demosaic_fcr_thresh1_write(be_reg, dyna_reg_cfg->fcr_hf_thresh_low);
            isp_demosaic_fcr_thresh2_write(be_reg, dyna_reg_cfg->fcr_hf_thresh_hig);
            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_fcr_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_gcac_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_GCAC_SUPPORT
    hi_bool  offline_mode;
    hi_bool  usr_resh  = HI_FALSE;
    hi_bool  idx_resh  = HI_FALSE;
    hi_u8 blk_num = reg_cfg_info->cfg_num;
    S_ISPBE_REGS_TYPE           *be_reg         = HI_NULL;
    isp_global_cac_usr_cfg      *usr_reg_cfg    = HI_NULL;
    isp_global_cac_static_cfg   *static_reg_cfg = HI_NULL;
    isp_usr_ctx                 *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_global_cac_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].gcac_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_gcac_ver_filt_en_write(be_reg, static_reg_cfg->gcac_ver_fil_en);
            isp_demosaic_gcac_blend_select_write(be_reg, static_reg_cfg->gcac_blend_sel);
            isp_gcac_chrv_mode_write(be_reg, static_reg_cfg->gcac_chr_ver_mode);
            isp_gcac_clip_mode_v_write(be_reg, static_reg_cfg->gcac_clip_mode_ver);
            isp_gcac_clip_mode_h_write(be_reg, static_reg_cfg->gcac_clip_mode_hor);
            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* usr */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].gcac_reg_cfg.usr_reg_cfg;
        idx_resh = (isp_gcac_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->resh && idx_resh) : (usr_reg_cfg->resh);

        if (usr_resh) {
            isp_gcac_update_index_write(be_reg, usr_reg_cfg->update_index);
            isp_gcac_red_a_write(be_reg, usr_reg_cfg->param_red_a);
            isp_gcac_red_b_write(be_reg, usr_reg_cfg->param_red_b);
            isp_gcac_red_c_write(be_reg, usr_reg_cfg->param_red_c);
            isp_gcac_blue_a_write(be_reg, usr_reg_cfg->param_blue_a);
            isp_gcac_blue_b_write(be_reg, usr_reg_cfg->param_blue_b);
            isp_gcac_blue_c_write(be_reg, usr_reg_cfg->param_blue_c);
            isp_gcac_ns_y_write(be_reg, usr_reg_cfg->ver_norm_shift);
            isp_gcac_nf_y_write(be_reg, usr_reg_cfg->ver_norm_factor);
            isp_gcac_ns_x_write(be_reg, usr_reg_cfg->hor_norm_shift);
            isp_gcac_nf_x_write(be_reg, usr_reg_cfg->hor_norm_factor);
            isp_gcac_cnt_start_h_write(be_reg, usr_reg_cfg->center_coor_hor);
            isp_gcac_cnt_start_v_write(be_reg, usr_reg_cfg->center_coor_ver);
            isp_demosaic_varthr_for_blend_write(be_reg, usr_reg_cfg->cor_var_thr);
            isp_gcac_cor_start_h_write(be_reg, usr_reg_cfg->start_coor_hor);
            isp_gcac_cor_start_v_write(be_reg, usr_reg_cfg->start_coor_ver);
            usr_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_global_cac_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }
#endif
    return HI_SUCCESS;
}

static hi_s32 isp_dpc_bpt_lut_reg_config(hi_bool *stt2_lut_regnew, hi_vi_pipe vi_pipe, hi_u8 i,
                                         S_ISPBE_REGS_TYPE *be_reg, isp_dpc_usr_cfg *usr_reg_cfg)
{
    hi_u8  buf_id;
    hi_s32 ret;
    S_VIPROC_REGS_TYPE    *vi_proc_reg    = HI_NULL;
    S_ISP_LUT_WSTT_TYPE   *be_lut_stt_reg = HI_NULL;
    isp_usr_ctx           *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        /* online lut2stt regconfig */
        buf_id = usr_reg_cfg->usr_sta_cor_reg_cfg.buf_id;

        be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
        isp_check_pointer_return(be_lut_stt_reg);

        isp_dpc_lut_wstt_write(be_lut_stt_reg, usr_reg_cfg->usr_sta_cor_reg_cfg.dpcc_bp_table);

        ret = isp_dpc_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
        if (ret != HI_SUCCESS) {
            isp_err_trace("isp[%d]_dpc_lut_wstt_addr_write failed\n", vi_pipe);
            return ret;
        }

        isp_dpc_stt2lut_en_write(be_reg, HI_TRUE);

        usr_reg_cfg->usr_sta_cor_reg_cfg.buf_id = 1 - buf_id;

        *stt2_lut_regnew = HI_TRUE;
    } else {
        isp_dpc_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt,
                               usr_reg_cfg->usr_sta_cor_reg_cfg.dpcc_bp_table);
        isp_dpc_stt2lut_en_write(be_reg, HI_TRUE);
        isp_dpc_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_dpc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_bool  stt2_lut_regnew = HI_FALSE;
    hi_bool  offline_mode;

    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_s32   ret = HI_SUCCESS;
    isp_dpc_static_cfg    *static_reg_cfg = HI_NULL;
    isp_dpc_dyna_cfg      *dyna_reg_cfg   = HI_NULL;
    isp_dpc_usr_cfg       *usr_reg_cfg    = HI_NULL;

    S_ISPBE_REGS_TYPE     *be_reg         = HI_NULL;
    S_VIPROC_REGS_TYPE    *vi_proc_reg    = HI_NULL;
    isp_usr_ctx           *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_dp_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].dp_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_dpc_output_mode_write(be_reg, static_reg_cfg->dpcc_output_mode);
            isp_dpc_bpt_ctrl_write(be_reg, static_reg_cfg->dpcc_bpt_ctrl);
            isp_dcg_lut_width_word_write(vi_proc_reg, HI_ISP_DPC_LUT_WIDTH_WORD_DEFAULT);
            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* usr */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].dp_reg_cfg.usr_reg_cfg;

        if (usr_reg_cfg->usr_dyna_cor_reg_cfg.resh) {
            isp_dpc_ex_soft_thr_max_write(be_reg, usr_reg_cfg->usr_dyna_cor_reg_cfg.dpcc_sup_twinkle_thr_max);
            isp_dpc_ex_soft_thr_min_write(be_reg, usr_reg_cfg->usr_dyna_cor_reg_cfg.dpcc_sup_twinkle_thr_min);
            isp_dpc_ex_hard_thr_en_write(be_reg, usr_reg_cfg->usr_dyna_cor_reg_cfg.dpcc_hard_thr_en);
            isp_dpc_ex_rake_ratio_write(be_reg, usr_reg_cfg->usr_dyna_cor_reg_cfg.dpcc_rake_ratio);
            usr_reg_cfg->usr_dyna_cor_reg_cfg.resh = offline_mode;
        }

        idx_resh = (isp_dpc_update_index_read(be_reg) != usr_reg_cfg->usr_sta_cor_reg_cfg.update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->usr_sta_cor_reg_cfg.resh && idx_resh) :
            (usr_reg_cfg->usr_sta_cor_reg_cfg.resh);

        if (usr_resh) {
            isp_dpc_update_index_write(be_reg, usr_reg_cfg->usr_sta_cor_reg_cfg.update_index);
            isp_dpc_bpt_number_write(be_reg, usr_reg_cfg->usr_sta_cor_reg_cfg.dpcc_bpt_number);

            ret = isp_dpc_bpt_lut_reg_config(&stt2_lut_regnew, vi_pipe, i, be_reg, usr_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            usr_reg_cfg->usr_sta_cor_reg_cfg.resh = offline_mode;
        }

        /* dynamic */
        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].dp_reg_cfg.dyna_reg_cfg;

        if (dyna_reg_cfg->resh) {
            isp_dpc_stat_en_write(vi_proc_reg, dyna_reg_cfg->dpc_stat_en);
            isp_dpc_blend_write(be_reg, dyna_reg_cfg->dpcc_alpha);
            isp_dpc_mode_write(be_reg, dyna_reg_cfg->dpcc_mode);
            isp_dpc_set_use_write(be_reg, dyna_reg_cfg->dpcc_set_use);
            isp_dpc_methods_set_1_write(be_reg, dyna_reg_cfg->dpcc_methods_set1);
            isp_dpc_methods_set_2_write(be_reg, dyna_reg_cfg->dpcc_methods_set2);
            isp_dpc_methods_set_3_write(be_reg, dyna_reg_cfg->dpcc_methods_set3);
            isp_dpc_line_thresh_1_write(be_reg, dyna_reg_cfg->dpcc_line_thr[0]); /* array index 0 */
            isp_dpc_line_mad_fac_1_write(be_reg, dyna_reg_cfg->dpcc_line_mad_fac[0]); /* array index 0 */
            isp_dpc_pg_fac_1_write(be_reg, dyna_reg_cfg->dpcc_pg_fac[0]); /* array index 0 */
            isp_dpc_rnd_thresh_1_write(be_reg, dyna_reg_cfg->dpcc_rnd_thr[0]); /* array index 0 */
            isp_dpc_rg_fac_1_write(be_reg, dyna_reg_cfg->dpcc_rg_fac[0]); /* array index 0 */
            isp_dpc_line_thresh_2_write(be_reg, dyna_reg_cfg->dpcc_line_thr[1]); /* array index 1 */
            isp_dpc_line_mad_fac_2_write(be_reg, dyna_reg_cfg->dpcc_line_mad_fac[1]); /* array index 1 */
            isp_dpc_pg_fac_2_write(be_reg, dyna_reg_cfg->dpcc_pg_fac[1]); /* array index 1 */
            isp_dpc_rnd_thresh_2_write(be_reg, dyna_reg_cfg->dpcc_rnd_thr[1]); /* array index 1 */
            isp_dpc_rg_fac_2_write(be_reg, dyna_reg_cfg->dpcc_rg_fac[1]); /* array index 1 */
            isp_dpc_line_thresh_3_write(be_reg, dyna_reg_cfg->dpcc_line_thr[2]); /* array index 2 */
            isp_dpc_line_mad_fac_3_write(be_reg, dyna_reg_cfg->dpcc_line_mad_fac[2]); /* array index 2 */
            isp_dpc_pg_fac_3_write(be_reg, dyna_reg_cfg->dpcc_pg_fac[2]); /* array index 2 */
            isp_dpc_rnd_thresh_3_write(be_reg, dyna_reg_cfg->dpcc_rnd_thr[2]); /* array index 2 */
            isp_dpc_rg_fac_3_write(be_reg, dyna_reg_cfg->dpcc_rg_fac[2]); /* array index 2 */
            isp_dpc_ro_limits_write(be_reg, dyna_reg_cfg->dpcc_ro_limits);
            isp_dpc_rnd_offs_write(be_reg, dyna_reg_cfg->dpcc_rnd_offs);
            isp_dpc_bpt_thresh_write(be_reg, dyna_reg_cfg->dpcc_bad_thresh);

            isp_dpc_line_std_thr_1_write(be_reg, dyna_reg_cfg->dpcc_line_std_thr[0]); /* array index 0 */
            isp_dpc_line_std_thr_2_write(be_reg, dyna_reg_cfg->dpcc_line_std_thr[1]); /* array index 1 */
            isp_dpc_line_std_thr_3_write(be_reg, dyna_reg_cfg->dpcc_line_std_thr[2]); /* array index 2 */
            isp_dpc_line_std_thr_4_write(be_reg, dyna_reg_cfg->dpcc_line_std_thr[3]); /* array index 3 */
            isp_dpc_line_std_thr_5_write(be_reg, dyna_reg_cfg->dpcc_line_std_thr[4]); /* array index 4 */

            isp_dpc_line_diff_thr_1_write(be_reg, dyna_reg_cfg->dpcc_line_diff_thr[0]); /* array index 0 */
            isp_dpc_line_diff_thr_2_write(be_reg, dyna_reg_cfg->dpcc_line_diff_thr[1]); /* array index 1 */
            isp_dpc_line_diff_thr_3_write(be_reg, dyna_reg_cfg->dpcc_line_diff_thr[2]); /* array index 2 */
            isp_dpc_line_diff_thr_4_write(be_reg, dyna_reg_cfg->dpcc_line_diff_thr[3]); /* array index 3 */
            isp_dpc_line_diff_thr_5_write(be_reg, dyna_reg_cfg->dpcc_line_diff_thr[4]); /* array index 4 */

            isp_dpc_line_aver_fac_1_write(be_reg, dyna_reg_cfg->dpcc_line_aver_fac[0]); /* array index 0 */
            isp_dpc_line_aver_fac_2_write(be_reg, dyna_reg_cfg->dpcc_line_aver_fac[1]); /* array index 1 */
            isp_dpc_line_aver_fac_3_write(be_reg, dyna_reg_cfg->dpcc_line_aver_fac[2]); /* array index 2 */
            isp_dpc_line_aver_fac_4_write(be_reg, dyna_reg_cfg->dpcc_line_aver_fac[3]); /* array index 3 */
            isp_dpc_line_aver_fac_5_write(be_reg, dyna_reg_cfg->dpcc_line_aver_fac[4]); /* array index 4 */

            isp_dpc_line_kerdiff_fac_write(be_reg, dyna_reg_cfg->dpcc_line_kerdiff_fac);
            isp_dpc_blend_mode_write(be_reg, dyna_reg_cfg->dpcc_blend_mode);
            isp_dpc_bit_depth_sel_write(be_reg, dyna_reg_cfg->dpcc_bit_depth_sel);

            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_dp_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.dpc_stt2lut_regnew = stt2_lut_regnew;

    return HI_SUCCESS;
}

static hi_s32 isp_ge_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    hi_u16 j;
    hi_u8 blk_num = reg_cfg_info->cfg_num;
    hi_bool offline_mode;
    isp_ge_static_cfg  *static_reg_cfg = HI_NULL;
    isp_ge_dyna_cfg    *dyna_reg_cfg   = HI_NULL;
    isp_ge_usr_cfg     *usr_reg_cfg    = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg         = HI_NULL;
    isp_usr_ctx        *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_ge_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].ge_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_ge_vnum_write(be_reg, static_reg_cfg->ge_num_v);
            isp_ge_gr_en_write(be_reg, static_reg_cfg->ge_gr_en);
            isp_ge_gb_en_write(be_reg, static_reg_cfg->ge_gb_en);
            isp_ge_gr_gb_en_write(be_reg, static_reg_cfg->ge_gr_gb_en);
            isp_ge_bit_depth_sel_write(be_reg, HI_ISP_GE_BIT_DEPTH_DEFAULT);
            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* usr */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].ge_reg_cfg.usr_reg_cfg;

        if (usr_reg_cfg->resh) {
            for (j = 0; j < reg_cfg_info->alg_reg_cfg[i].ge_reg_cfg.chn_num; j++) {
                isp_ge_ct_th2_write(be_reg, j, usr_reg_cfg->ge_ct_th2[j]);
                isp_ge_ct_slope1_write(be_reg, j, usr_reg_cfg->ge_ct_slope1[j]);
                isp_ge_ct_slope2_write(be_reg, j, usr_reg_cfg->ge_ct_slope2[j]);
            }

            isp_ge_hnum_write(be_reg, usr_reg_cfg->ge_num_h);
            isp_ge_crop_pos_x_write(be_reg, usr_reg_cfg->ge_crop_pos_x);
            isp_ge_crop_pos_y_write(be_reg, usr_reg_cfg->ge_crop_pos_y);
            isp_ge_crop_out_width_write(be_reg, usr_reg_cfg->ge_crop_out_width - 1);
            isp_ge_crop_out_height_write(be_reg, usr_reg_cfg->ge_crop_out_height - 1);
            usr_reg_cfg->resh = offline_mode;
        }

        /* dynamic */
        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].ge_reg_cfg.dyna_reg_cfg;

        if (dyna_reg_cfg->resh) {
            for (j = 0; j < reg_cfg_info->alg_reg_cfg[i].ge_reg_cfg.chn_num; j++) {
                isp_ge_ct_th1_write(be_reg, j, dyna_reg_cfg->ge_ct_th1[j]);
                isp_ge_ct_th3_write(be_reg, j, dyna_reg_cfg->ge_ct_th3[j]);
            }

            isp_ge_strength_write(be_reg, dyna_reg_cfg->ge_strength);
            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_ge_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }
#endif

    return HI_SUCCESS;
}

static hi_s32 isp_lsc_usr_reg_config(hi_bool *stt2_lut_regnew, hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i,
                                     isp_lsc_usr_cfg *usr_reg_cfg)
{
    hi_u8    buf_id;
    hi_u16   j;
    hi_s32   ret = HI_SUCCESS;
    isp_usr_ctx         *isp_ctx        = HI_NULL;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE   *be_reg         = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    be_reg      = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);
    isp_check_pointer_return(be_reg);

    isp_lsc_width_offset_write(be_reg, usr_reg_cfg->width_offset);

    for (j = 0; j < (HI_ISP_LSC_GRID_ROW - 1) / 2; j++) {   /* lsc grid row number divide by 2 */
        isp_lsc_win_y_info_write(be_reg, j, usr_reg_cfg->delta_y[j], usr_reg_cfg->inv_y[j]);
    }

    for (j = 0; j < (HI_ISP_LSC_GRID_COL - 1); j++) {
        isp_lsc_win_x_info_write(be_reg, j, usr_reg_cfg->delta_x[j], usr_reg_cfg->inv_x[j]);
    }

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        if (reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.lut2_stt_en == HI_TRUE) {  /* online lut2stt regconfig */
            buf_id = usr_reg_cfg->buf_id;

            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
            isp_check_pointer_return(be_lut_stt_reg);

            isp_lsc_lut_wstt_write(be_lut_stt_reg, usr_reg_cfg->r_gain, usr_reg_cfg->gr_gain,
                                   usr_reg_cfg->gb_gain, usr_reg_cfg->b_gain);
            ret = isp_lsc_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("isp[%d]_lsc_lut_wstt_addr_write failed\n", vi_pipe);
                return ret;
            }

            isp_lsc_stt2lut_en_write(be_reg, HI_TRUE);

            usr_reg_cfg->buf_id = 1 - buf_id;
            *stt2_lut_regnew = HI_TRUE;
        }
    } else {
        isp_lsc_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, usr_reg_cfg->r_gain,
                               usr_reg_cfg->gr_gain, usr_reg_cfg->gb_gain, usr_reg_cfg->b_gain);
        isp_lsc_stt2lut_en_write(be_reg, HI_TRUE);
        isp_lsc_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_lsc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_bool  lut_update = HI_FALSE;
    hi_bool  stt2_lut_regnew = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_s32   ret = HI_SUCCESS;
    isp_lsc_usr_cfg    *usr_reg_cfg    = HI_NULL;
    isp_lsc_static_cfg *static_reg_cfg = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg         = HI_NULL;
    isp_usr_ctx        *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_lsc_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);
        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.static_reg_cfg;
        usr_reg_cfg    = &reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_lsc_numh_write(be_reg, static_reg_cfg->win_num_h);
            isp_lsc_numv_write(be_reg, static_reg_cfg->win_num_v);
            isp_lsc_lut_width_word_write(vi_proc_reg, HI_ISP_LSC_LUT_WIDTH_WORD_DEFAULT);
            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* usr */
        if (usr_reg_cfg->coef_update) {
            isp_lsc_mesh_str_write(be_reg, usr_reg_cfg->mesh_str);
            isp_lsc_mesh_weight_write(be_reg, usr_reg_cfg->weight);
            usr_reg_cfg->coef_update = offline_mode;
        }

        idx_resh = (isp_lsc_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->lut_update && idx_resh) : (usr_reg_cfg->lut_update);

        if (usr_resh) {
            isp_lsc_update_index_write(be_reg, usr_reg_cfg->update_index);

            ret = isp_lsc_usr_reg_config(&stt2_lut_regnew, vi_pipe, reg_cfg_info, i, usr_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            lut_update = HI_TRUE;

            usr_reg_cfg->lut_update = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_lsc_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.lsc_lut_update       = lut_update;
    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.lsc_stt2lut_regnew = stt2_lut_regnew;

    if (reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_lut2_stt_reg_new == HI_TRUE) {
        if (reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt < 3) { /* lsc reg config conunt 3 */
            reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.lsc_stt2lut_regnew = HI_TRUE;
            reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt++;
        } else {
            reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_lut2_stt_reg_new = HI_FALSE;
            reg_cfg_info->alg_reg_cfg[i].lsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt      = 0;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_gamma_lut_reg_config(hi_bool *stt2_lut_regnew, hi_vi_pipe vi_pipe, hi_u8 i,
                                       S_ISPBE_REGS_TYPE *be_reg, isp_gamma_usr_cfg *usr_reg_cfg)
{
    hi_u8    buf_id;
    hi_s32   ret;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg  = HI_NULL;
    isp_usr_ctx         *isp_ctx         = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg     = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);

    isp_gamma_lut_width_word_write(vi_proc_reg, HI_ISP_GAMMA_LUT_WIDTH_WORD_DEFAULT);

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        if (usr_reg_cfg->switch_mode != HI_TRUE) {
            buf_id = usr_reg_cfg->buf_id;

            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
            isp_check_pointer_return(be_lut_stt_reg);

            isp_gamma_lut_wstt_write(be_lut_stt_reg, usr_reg_cfg->gamma_lut);
            ret = isp_gamma_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("isp[%d]_gamma_lut_wstt_addr_write failed\n", vi_pipe);
                return ret;
            }
            isp_gamma_stt2lut_en_write(be_reg, HI_TRUE);
            usr_reg_cfg->buf_id = 1 - buf_id;
            *stt2_lut_regnew = HI_TRUE;
        }
    } else {
        isp_gamma_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, usr_reg_cfg->gamma_lut);
        isp_gamma_stt2lut_en_write(be_reg, HI_TRUE);
        isp_gamma_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}
static hi_s32 isp_gamma_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_bool  stt2_lut_regnew = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_s32   ret = HI_SUCCESS;
    S_ISPBE_REGS_TYPE *be_reg      = HI_NULL;
    isp_gamma_usr_cfg *usr_reg_cfg = HI_NULL;
    isp_usr_ctx       *isp_ctx     = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    usr_reg_cfg    = &reg_cfg_info->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg;

    if (reg_cfg_info->cfg_key.bit1_gamma_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        idx_resh = (isp_gamma_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->gamma_lut_update_en && idx_resh) :
            (usr_reg_cfg->gamma_lut_update_en);

        if (usr_resh) {
            isp_gamma_update_index_write(be_reg, usr_reg_cfg->update_index);

            ret = isp_gamma_lut_reg_config(&stt2_lut_regnew, vi_pipe, i, be_reg, usr_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            isp_gamma_inseg_write(be_reg, usr_reg_cfg->gamma_in_seg); /* inseg */
            isp_gamma_pos_write(be_reg,   usr_reg_cfg->gamma_pos);  /* pos */
            isp_gamma_step_write(be_reg,  usr_reg_cfg->gamma_step);  /* step */

            usr_reg_cfg->gamma_lut_update_en = offline_mode;
            usr_reg_cfg->switch_mode         = HI_FALSE;
        }

        reg_cfg_info->cfg_key.bit1_gamma_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.gamma_stt2lut_regnew = stt2_lut_regnew;

    return HI_SUCCESS;
}
static hi_s32 isp_csc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8 blk_num = reg_cfg_info->cfg_num;
    hi_bool offline_mode;
    isp_csc_dyna_cfg  *dyna_reg_cfg = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg       = HI_NULL;
    isp_usr_ctx        *isp_ctx     = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_csc_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].csc_cfg.dyna_reg_cfg;

        /* dynamic */
        if (dyna_reg_cfg->resh) {
            isp_csc_coef00_write(be_reg, dyna_reg_cfg->csc_coef[0]); /* coef0 */
            isp_csc_coef01_write(be_reg, dyna_reg_cfg->csc_coef[1]); /* coef1 */
            isp_csc_coef02_write(be_reg, dyna_reg_cfg->csc_coef[2]); /* coef2 */
            isp_csc_coef10_write(be_reg, dyna_reg_cfg->csc_coef[3]); /* coef3 */
            isp_csc_coef11_write(be_reg, dyna_reg_cfg->csc_coef[4]); /* coef4 */
            isp_csc_coef12_write(be_reg, dyna_reg_cfg->csc_coef[5]); /* coef5 */
            isp_csc_coef20_write(be_reg, dyna_reg_cfg->csc_coef[6]); /* coef6 */
            isp_csc_coef21_write(be_reg, dyna_reg_cfg->csc_coef[7]); /* coef7 */
            isp_csc_coef22_write(be_reg, dyna_reg_cfg->csc_coef[8]); /* coef8 */
            isp_csc_in_dc0_write(be_reg, dyna_reg_cfg->csc_in_dc[0]); /* in_dc0 */
            isp_csc_in_dc1_write(be_reg, dyna_reg_cfg->csc_in_dc[1]); /* in_dc1 */
            isp_csc_in_dc2_write(be_reg, dyna_reg_cfg->csc_in_dc[2]); /* in_dc2 */

            isp_csc_out_dc0_write(be_reg, dyna_reg_cfg->csc_out_dc[0]); /* out_dc0 */
            isp_csc_out_dc1_write(be_reg, dyna_reg_cfg->csc_out_dc[1]); /* out_dc1 */
            isp_csc_out_dc2_write(be_reg, dyna_reg_cfg->csc_out_dc[2]); /* out_dc2 */
            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_csc_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_ISP_CA_SUPPORT
static hi_s32 isp_ca_lut_reg_config(hi_bool *stt2_lut_regnew, hi_vi_pipe vi_pipe, hi_u8 i, S_ISPBE_REGS_TYPE *be_reg,
                                    isp_ca_usr_cfg *usr_reg_cfg)
{
    hi_u8    buf_id;
    hi_s32   ret;
    isp_usr_ctx         *isp_ctx        = HI_NULL;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg    = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);

    isp_ca_lut_width_word_write(vi_proc_reg, HI_ISP_CA_LUT_WIDTH_WORD_DEFAULT);

    if (usr_reg_cfg->ca_lut_update_en) {
        if (is_online_mode(isp_ctx->block_attr.running_mode) || \
            is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
            buf_id = usr_reg_cfg->buf_id;

            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
            isp_check_pointer_return(be_lut_stt_reg);

            isp_ca_lut_wstt_write(be_lut_stt_reg, usr_reg_cfg->y_ratio_lut);
            ret = isp_ca_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("isp[%d]_ca_lut_wstt_addr_write failed\n", vi_pipe);
                return ret;
            }

            isp_ca_stt2lut_en_write(be_reg, HI_TRUE);

            usr_reg_cfg->buf_id = 1 - buf_id;

            *stt2_lut_regnew = HI_TRUE;
        } else {
            isp_ca_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, usr_reg_cfg->y_ratio_lut);
            isp_ca_stt2lut_en_write(be_reg, HI_TRUE);
            isp_ca_stt2lut_regnew_write(be_reg, HI_TRUE);
        }
    }

    return HI_SUCCESS;
}
#endif

static hi_s32 isp_ca_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_CA_SUPPORT
    hi_bool  offline_mode;
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_bool  stt2_lut_regnew = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_s32   ret;
    isp_ca_static_cfg *static_reg_cfg = HI_NULL;
    isp_ca_usr_cfg    *usr_reg_cfg    = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg         = HI_NULL;
    isp_usr_ctx       *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_ca_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        /* usr */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].ca_reg_cfg.usr_reg_cfg;
        idx_resh = (isp_ca_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->resh && idx_resh) : (usr_reg_cfg->resh);

        if (usr_resh) {
            isp_ca_update_index_write(be_reg, usr_reg_cfg->update_index);
            ret = isp_ca_lut_reg_config(&stt2_lut_regnew, vi_pipe, i, be_reg, usr_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            usr_reg_cfg->resh = offline_mode;
        }

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].ca_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_ca_llhcproc_en_write(be_reg, static_reg_cfg->ca_llhc_proc_en);
            isp_ca_skinproc_en_write(be_reg, static_reg_cfg->ca_skin_proc_en);
            isp_ca_satadj_en_write(be_reg, static_reg_cfg->ca_satu_adj_en);
            isp_ca_lumath_low_write(be_reg, static_reg_cfg->ca_luma_thr_low);
            isp_ca_darkchromath_low_write(be_reg, static_reg_cfg->ca_dark_chroma_thr_low);
            isp_ca_darkchromath_high_write(be_reg, static_reg_cfg->ca_dark_chroma_thr_high);
            isp_ca_sdarkchromath_low_write(be_reg, static_reg_cfg->ca_s_dark_chroma_thr_low);
            isp_ca_sdarkchromath_high_write(be_reg, static_reg_cfg->ca_s_dark_chroma_thr_high);
            isp_ca_lumaratio_low_write(be_reg, static_reg_cfg->ca_luma_ratio_low);
            isp_ca_yuv2rgb_coef_write(be_reg, static_reg_cfg->ca_yuv2_rgb_coef);
            isp_ca_yuv2rgb_indc_write(be_reg, static_reg_cfg->ca_yuv2_rgb_in_dc);
            isp_ca_yuv2rgb_outdc_write(be_reg, static_reg_cfg->ca_yuv2_rgb_out_dc);
            isp_ca_skinlluma_umin_write(be_reg, static_reg_cfg->ca_skin_low_luma_min_u);
            isp_ca_skinlluma_umax_write(be_reg, static_reg_cfg->ca_skin_low_luma_max_u);
            isp_ca_skinlluma_uymin_write(be_reg, static_reg_cfg->ca_skin_low_luma_min_uy);
            isp_ca_skinlluma_uymax_write(be_reg, static_reg_cfg->ca_skin_low_luma_max_uy);
            isp_ca_skinhluma_umin_write(be_reg, static_reg_cfg->ca_skin_high_luma_min_u);
            isp_ca_skinhluma_umax_write(be_reg, static_reg_cfg->ca_skin_high_luma_max_u);
            isp_ca_skinhluma_uymin_write(be_reg, static_reg_cfg->ca_skin_high_luma_min_uy);
            isp_ca_skinhluma_uymax_write(be_reg, static_reg_cfg->ca_skin_high_luma_max_uy);
            isp_ca_skinlluma_vmin_write(be_reg, static_reg_cfg->ca_skin_low_luma_min_v);
            isp_ca_skinlluma_vmax_write(be_reg, static_reg_cfg->ca_skin_low_luma_max_v);
            isp_ca_skinlluma_vymin_write(be_reg, static_reg_cfg->ca_skin_low_luma_min_vy);
            isp_ca_skinlluma_vymax_write(be_reg, static_reg_cfg->ca_skin_low_luma_max_vy);
            isp_ca_skinhluma_vmin_write(be_reg, static_reg_cfg->ca_skin_high_luma_min_v);
            isp_ca_skinhluma_vmax_write(be_reg, static_reg_cfg->ca_skin_high_luma_max_v);
            isp_ca_skinhluma_vymin_write(be_reg, static_reg_cfg->ca_skin_high_luma_min_vy);
            isp_ca_skinhluma_vymax_write(be_reg, static_reg_cfg->ca_skin_high_luma_max_vy);
            isp_ca_skin_uvdiff_write(be_reg, static_reg_cfg->ca_skin_uv_diff);
            isp_ca_skinratioth_low_write(be_reg, static_reg_cfg->ca_skin_ratio_thr_low);
            isp_ca_skinratioth_mid_write(be_reg, static_reg_cfg->ca_skin_ratio_thr_mid);
            isp_ca_skinratioth_high_write(be_reg, static_reg_cfg->ca_skin_ratio_thr_high);

            static_reg_cfg->static_resh = HI_FALSE;
        }

        reg_cfg_info->cfg_key.bit1_ca_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.ca_stt2lut_regnew = stt2_lut_regnew;
#endif

    return HI_SUCCESS;
}

static hi_s32 isp_mcds_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8 blk_num = reg_cfg_info->cfg_num;
    hi_bool offline_mode;
    isp_mcds_dyna_reg_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_mcds_static_reg_cfg *static_reg_cfg = HI_NULL;
    S_VIPROC_REGS_TYPE      *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *be_reg         = HI_NULL;
    isp_usr_ctx             *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_mcds_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);

        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg;
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].mcds_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_hcds_en_write(vi_proc_reg, static_reg_cfg->hcds_en);
            isp_mcds_coefh0_write(be_reg, static_reg_cfg->h_coef[0]); /* coefh0 */
            isp_mcds_coefh1_write(be_reg, static_reg_cfg->h_coef[1]); /* coefh1 */
            isp_mcds_coefh2_write(be_reg, static_reg_cfg->h_coef[2]); /* coefh2 */
            isp_mcds_coefh3_write(be_reg, static_reg_cfg->h_coef[3]); /* coefh3 */
            isp_mcds_coefh4_write(be_reg, static_reg_cfg->h_coef[4]); /* coefh4 */
            isp_mcds_coefh5_write(be_reg, static_reg_cfg->h_coef[5]); /* coefh5 */
            isp_mcds_coefh6_write(be_reg, static_reg_cfg->h_coef[6]); /* coefh6 */
            isp_mcds_coefh7_write(be_reg, static_reg_cfg->h_coef[7]); /* coefh7 */
            isp_de_vcds_coefv0_write(be_reg, static_reg_cfg->v_coef[0]);
            isp_de_vcds_coefv1_write(be_reg, static_reg_cfg->v_coef[1]);
            isp_de_vcds_limit_write(be_reg, static_reg_cfg->coring_limit);
            isp_mcds_midf_bldr_write(be_reg, static_reg_cfg->midf_bldr);
            static_reg_cfg->static_resh = 0;
        }

        if (dyna_reg_cfg->dyna_resh) {
            isp_mcds_midf_en_write(be_reg, dyna_reg_cfg->midf_en);
            dyna_reg_cfg->dyna_resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_mcds_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_wdr_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh   = HI_FALSE;
    hi_bool  idx_resh   = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_u16   offset0;
    hi_u32   blc_comp0;
    isp_fswdr_static_cfg *static_reg_cfg = HI_NULL;
    isp_fswdr_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_fswdr_usr_cfg    *usr_reg_cfg    = HI_NULL;
    S_VIPROC_REGS_TYPE   *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE    *be_reg         = HI_NULL;
    isp_usr_ctx          *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_fs_wdr_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);

        isp_wdr_en_write(vi_proc_reg, reg_cfg_info->alg_reg_cfg[i].wdr_reg_cfg.wdr_en);
        isp_bnr_wdr_enable_write(be_reg, reg_cfg_info->alg_reg_cfg[i].wdr_reg_cfg.wdr_en);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].wdr_reg_cfg.static_reg_cfg;
        usr_reg_cfg    = &reg_cfg_info->alg_reg_cfg[i].wdr_reg_cfg.usr_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].wdr_reg_cfg.dyna_reg_cfg;

        /* static */
        if (static_reg_cfg->first_frame == HI_TRUE) {
            isp_wdr_expovalue0_write(be_reg, static_reg_cfg->expo_value_lut[0]); /* expovalue 0 */
            isp_wdr_expovalue1_write(be_reg, static_reg_cfg->expo_value_lut[1]); /* expovalue 1 */
            isp_wdr_maxratio_write(be_reg, static_reg_cfg->max_ratio);
            isp_wdr_exporratio0_write(be_reg, static_reg_cfg->expo_r_ratio_lut[0]); /* expo_r_ratio 0 */
            isp_wdr_fusionmode_write(be_reg, usr_reg_cfg->fusion_mode);
            isp_wdr_mdt_en_write(be_reg, dyna_reg_cfg->wdr_mdt_en);
            isp_wdr_short_thr_write(be_reg, dyna_reg_cfg->short_thr);
            isp_wdr_long_thr_write(be_reg, dyna_reg_cfg->long_thr);
            offset0   = reg_cfg_info->alg_reg_cfg[i].wdr_reg_cfg.sync_reg_cfg.offset0;
            blc_comp0 = static_reg_cfg->blc_comp_lut[0] * offset0;
            static_reg_cfg->blc_comp_lut[0] = blc_comp0;
            isp_wdr_blc_comp0_write(be_reg, blc_comp0);
            isp_flick_exporatio_write(be_reg, static_reg_cfg->flick_exp_ratio);
            static_reg_cfg->first_frame = HI_FALSE;
        }

        if (static_reg_cfg->resh) {
            isp_wdr_grayscale_mode_write(be_reg, static_reg_cfg->gray_scale_mode);
            isp_wdr_bsaveblc_write(be_reg, static_reg_cfg->save_blc);
            isp_wdr_mask_similar_thr_write(be_reg, static_reg_cfg->mask_similar_thr);
            isp_wdr_mask_similar_cnt_write(be_reg, static_reg_cfg->mask_similar_cnt);
            isp_wdr_dftwgt_fl_write(be_reg, static_reg_cfg->dft_wgt_fl);
            isp_wdr_bldrlhfidx_write(be_reg, static_reg_cfg->bldr_lhf_idx);
            isp_wdr_saturate_thr_write(be_reg, static_reg_cfg->saturate_thr);
            isp_wdr_fusion_saturate_thd_write(be_reg, static_reg_cfg->fusion_saturate_thd);
            isp_wdr_forcelong_smooth_en_write(be_reg, static_reg_cfg->force_long_smooth_en);
            static_reg_cfg->resh = HI_FALSE;
        }

        /* usr */
        idx_resh = (isp_wdr_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->resh && idx_resh) : (usr_reg_cfg->resh);

        if (usr_resh) {
            isp_wdr_update_index_write(be_reg, usr_reg_cfg->update_index);
            isp_wdr_shortexpo_chk_write(be_reg, usr_reg_cfg->short_expo_chk);
            isp_wdr_mdtlbld_write(be_reg, usr_reg_cfg->mdt_l_bld);
            isp_wdr_mdt_full_thr_write(be_reg, usr_reg_cfg->mdt_full_thr);
            isp_wdr_mdt_still_thr_write(be_reg, usr_reg_cfg->mdt_still_thr);
            isp_wdr_pixel_avg_max_diff_write(be_reg, usr_reg_cfg->pixel_avg_max_diff);

            usr_reg_cfg->resh = offline_mode;
        }

        /* dynamic */
        if (dyna_reg_cfg->resh) {
            isp_wdr_sqrt_again_g_write(be_reg, dyna_reg_cfg->sqrt_again_g);
            isp_wdr_sqrt_dgain_g_write(be_reg, dyna_reg_cfg->sqrt_dgain_g);
            isp_wdr_mdt_nosfloor_write(be_reg, dyna_reg_cfg->mdt_nos_floor);
            isp_wdr_mdthr_low_gain_write(be_reg, dyna_reg_cfg->md_thr_low_gain);
            isp_wdr_mdthr_hig_gain_write(be_reg, dyna_reg_cfg->md_thr_hig_gain);

            isp_wdr_f0_still_thr_write(be_reg, dyna_reg_cfg->still_thr_lut[0]);
            isp_wdr_erosion_en_write(be_reg, dyna_reg_cfg->erosion_en);
            isp_wdr_fusion_f0_thr_r_write(be_reg, dyna_reg_cfg->fusion_thr_r_lut[0]);
            isp_wdr_fusion_f1_thr_r_write(be_reg, dyna_reg_cfg->fusion_thr_r_lut[1]);
            isp_wdr_fusion_f0_thr_g_write(be_reg, dyna_reg_cfg->fusion_thr_g_lut[0]);
            isp_wdr_fusion_f1_thr_g_write(be_reg, dyna_reg_cfg->fusion_thr_g_lut[1]);
            isp_wdr_fusion_f0_thr_b_write(be_reg, dyna_reg_cfg->fusion_thr_b_lut[0]);
            isp_wdr_fusion_f1_thr_b_write(be_reg, dyna_reg_cfg->fusion_thr_b_lut[1]);

            isp_bcom_en_write(vi_proc_reg, dyna_reg_cfg->bcom_en);
            isp_bdec_en_write(vi_proc_reg, dyna_reg_cfg->bdec_en);
            isp_bcom_alpha_write(be_reg, dyna_reg_cfg->bcom_alpha);
            isp_bdec_alpha_write(be_reg, dyna_reg_cfg->bdec_alpha);

            isp_wdr_forcelong_en_write(be_reg, dyna_reg_cfg->force_long);
            isp_wdr_forcelong_low_thd_write(be_reg, dyna_reg_cfg->force_long_low_thr);
            isp_wdr_forcelong_high_thd_write(be_reg, dyna_reg_cfg->force_long_hig_thr);

            isp_wdr_shortchk_thd_write(be_reg, dyna_reg_cfg->short_check_thd);
            isp_wdr_fusion_data_mode_write(be_reg, dyna_reg_cfg->fusion_data_mode);

            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_fs_wdr_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }
    return HI_SUCCESS;
}

static hi_void isp_drc_cclut_reg_config(isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg, isp_drc_usr_cfg *usr_reg_cfg)
{
    hi_u16   j;

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_drc_cclut_waddr_write(be_reg, 0);

        for (j = 0; j < HI_ISP_DRC_CC_NODE_NUM; j++) {
            isp_drc_cclut_wdata_write(be_reg, usr_reg_cfg->cclut[j]);
        }
    } else {
        isp_drc_cclut_write(be_reg, usr_reg_cfg->cclut);
    }
}

static hi_void isp_drc_tmlut_reg_config(hi_bool *tm_lut_update, isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                        isp_drc_dyna_cfg *dyna_reg_cfg)
{
    hi_u16   j;

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        if (dyna_reg_cfg->lut_update) {
            isp_drc_tmlut0_waddr_write(be_reg, 0);
            for (j = 0; j < HI_ISP_DRC_TM_NODE_NUM; j++) {
                isp_drc_tmlut0_wdata_write(be_reg, ((dyna_reg_cfg->tone_mapping_value0[j]) << 14) | /* 14 bit */
                                           (dyna_reg_cfg->tone_mapping_diff0[j]));
            }
            *tm_lut_update = HI_TRUE;
        }
    } else {
        isp_drc_tmlut0_value_write(be_reg, dyna_reg_cfg->tone_mapping_value0);
        isp_drc_tmlut0_diff_write(be_reg, dyna_reg_cfg->tone_mapping_diff0);
    }
}

static hi_s32 isp_drc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh;
    hi_bool  idx_resh;
    hi_bool  tm_lut_update = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    isp_drc_static_cfg *static_reg_cfg = HI_NULL;
    isp_drc_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_drc_usr_cfg    *usr_reg_cfg    = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg         = HI_NULL;
    isp_usr_ctx        *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_drc_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);

        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);

        isp_drc_en_write(vi_proc_reg, reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.drc_en);

        if (reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.drc_en == HI_TRUE) {
            isp_drc_dither_en_write(be_reg, HI_FALSE);
        } else {
            isp_drc_dither_en_write(be_reg, !(isp_ctx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR));
        }

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_drc_outbits_write(be_reg, static_reg_cfg->bit_width_out);

            isp_drc_wrstat_en_write(be_reg, static_reg_cfg->wrstat_en);
            isp_drc_rdstat_en_write(be_reg, static_reg_cfg->rdstat_en);

            isp_drc_bin_write(be_reg, static_reg_cfg->bin_num_z);

            /* static registers */
            isp_drc_local_edge_lmt_write(be_reg, static_reg_cfg->local_edge_lmt);

            isp_drc_r_wgt_write(be_reg, static_reg_cfg->r_wgt);
            isp_drc_g_wgt_write(be_reg, static_reg_cfg->g_wgt);
            isp_drc_b_wgt_write(be_reg, static_reg_cfg->b_wgt);

            isp_drc_cc_lin_pow_write(be_reg, static_reg_cfg->color_control_mode);
            isp_drc_cc_lut_ctrl_write(be_reg, static_reg_cfg->color_control_lut_ctrl);
            isp_drc_cc_global_corr_write(be_reg, static_reg_cfg->global_color_corr);

            isp_drc_wgt_box_tri_sel_write(be_reg, static_reg_cfg->wgt_box_tri_sel);
            isp_drc_detail_boost_en_write(be_reg, static_reg_cfg->detail_boost_enable);
            isp_drc_pdw_sum_en_write(be_reg, static_reg_cfg->pdw_sum_enable);
            isp_drc_mono_chroma_en_write(be_reg, static_reg_cfg->monochrome_mode);

            /* purple fringe detection & reduction */
            isp_drc_rg_ctr_write(be_reg, static_reg_cfg->pfrrg_ctr);
            isp_drc_rg_wid_write(be_reg, static_reg_cfg->pfrrg_wid);
            isp_drc_rg_slo_write(be_reg, static_reg_cfg->pfrrg_slo);

            isp_drc_bg_thr_write(be_reg, static_reg_cfg->pfrbg_thr);
            isp_drc_bg_slo_write(be_reg, static_reg_cfg->pfrbg_slo);

            isp_drc_idxbase0_write(be_reg, static_reg_cfg->seg_idx_base[0]); /* array index 0 */
            isp_drc_idxbase1_write(be_reg, static_reg_cfg->seg_idx_base[1]); /* array index 1 */
            isp_drc_idxbase2_write(be_reg, static_reg_cfg->seg_idx_base[2]); /* array index 2 */
            isp_drc_idxbase3_write(be_reg, static_reg_cfg->seg_idx_base[3]); /* array index 3 */
            isp_drc_idxbase4_write(be_reg, static_reg_cfg->seg_idx_base[4]); /* array index 4 */
            isp_drc_idxbase5_write(be_reg, static_reg_cfg->seg_idx_base[5]); /* array index 5 */
            isp_drc_idxbase6_write(be_reg, static_reg_cfg->seg_idx_base[6]); /* array index 6 */
            isp_drc_idxbase7_write(be_reg, static_reg_cfg->seg_idx_base[7]); /* array index 7 */

            isp_drc_maxval0_write(be_reg, static_reg_cfg->seg_max_val[0]); /* array index 0 */
            isp_drc_maxval1_write(be_reg, static_reg_cfg->seg_max_val[1]); /* array index 1 */
            isp_drc_maxval2_write(be_reg, static_reg_cfg->seg_max_val[2]); /* array index 2 */
            isp_drc_maxval3_write(be_reg, static_reg_cfg->seg_max_val[3]); /* array index 3 */
            isp_drc_maxval4_write(be_reg, static_reg_cfg->seg_max_val[4]); /* array index 4 */
            isp_drc_maxval5_write(be_reg, static_reg_cfg->seg_max_val[5]); /* array index 5 */
            isp_drc_maxval6_write(be_reg, static_reg_cfg->seg_max_val[6]); /* array index 6 */
            isp_drc_maxval7_write(be_reg, static_reg_cfg->seg_max_val[7]); /* array index 7 */

            if (static_reg_cfg->first_frame) {
                isp_drc_shp_log_write(be_reg, static_reg_cfg->shp_log);
                isp_drc_shp_exp_write(be_reg, static_reg_cfg->shp_exp);
                isp_drc_div_denom_log_write(be_reg, static_reg_cfg->div_denom_log);
                isp_drc_denom_exp_write(be_reg, static_reg_cfg->denom_exp);
                isp_drc_prev_luma_0_write(be_reg, static_reg_cfg->prev_luma[0]); /* array index 0 */
                isp_drc_prev_luma_1_write(be_reg, static_reg_cfg->prev_luma[1]); /* array index 1 */
                isp_drc_prev_luma_2_write(be_reg, static_reg_cfg->prev_luma[2]); /* array index 2 */
                isp_drc_prev_luma_3_write(be_reg, static_reg_cfg->prev_luma[3]); /* array index 3 */
                isp_drc_prev_luma_4_write(be_reg, static_reg_cfg->prev_luma[4]); /* array index 4 */
                isp_drc_prev_luma_5_write(be_reg, static_reg_cfg->prev_luma[5]); /* array index 5 */
                isp_drc_prev_luma_6_write(be_reg, static_reg_cfg->prev_luma[6]); /* array index 6 */
                isp_drc_prev_luma_7_write(be_reg, static_reg_cfg->prev_luma[7]); /* array index 7 */

                static_reg_cfg->first_frame = HI_FALSE;
            }

            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* user */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.usr_reg_cfg;
        idx_resh = (isp_drc_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->usr_resh && idx_resh) : (usr_reg_cfg->usr_resh);

        if (usr_resh) {
            isp_drc_update_index_write(be_reg, usr_reg_cfg->update_index);

            isp_drc_sft1_y_write(be_reg, usr_reg_cfg->ysft1);
            isp_drc_val1_y_write(be_reg, usr_reg_cfg->yval1);
            isp_drc_sft2_y_write(be_reg, usr_reg_cfg->ysft2);
            isp_drc_val2_y_write(be_reg, usr_reg_cfg->yval2);

            isp_drc_sft1_c_write(be_reg, usr_reg_cfg->csft1);
            isp_drc_val1_c_write(be_reg, usr_reg_cfg->cval1);
            isp_drc_sft2_c_write(be_reg, usr_reg_cfg->csft2);
            isp_drc_val2_c_write(be_reg, usr_reg_cfg->cval2);

            isp_drc_sft_write(be_reg, usr_reg_cfg->sft);
            isp_drc_val_write(be_reg, usr_reg_cfg->val);

            isp_drc_var_spa_coarse_write(be_reg, usr_reg_cfg->var_spa_coarse);
            isp_drc_var_spa_medium_write(be_reg, usr_reg_cfg->var_spa_medium);
            isp_drc_var_spa_fine_write(be_reg, usr_reg_cfg->var_spa_fine);

            isp_drc_var_rng_coarse_write(be_reg, usr_reg_cfg->var_rng_coarse);
            isp_drc_var_rng_medium_write(be_reg, usr_reg_cfg->var_rng_medium);
            isp_drc_var_rng_fine_write(be_reg, usr_reg_cfg->var_rng_fine);

            isp_drc_grad_rev_shift_write(be_reg, usr_reg_cfg->grad_shift);
            isp_drc_grad_rev_slope_write(be_reg, usr_reg_cfg->grad_slope);
            isp_drc_grad_rev_max_write(be_reg, usr_reg_cfg->grad_max);
            isp_drc_grad_rev_thres_write(be_reg, usr_reg_cfg->grad_thr);

            isp_drc_high_slo_write(be_reg, usr_reg_cfg->pfr_high_slo);
            isp_drc_low_slo_write(be_reg, usr_reg_cfg->pfr_low_slo);
            isp_drc_low_thr_write(be_reg, usr_reg_cfg->pfr_low_thr);

            isp_drc_gain_clip_knee_write(be_reg, usr_reg_cfg->gain_clip_knee);
            isp_drc_gain_clip_step_write(be_reg, usr_reg_cfg->gain_clip_step);

            isp_drc_mixing_coring_write(be_reg, usr_reg_cfg->mixing_coring);
            isp_drc_dark_min_write(be_reg, usr_reg_cfg->mixing_dark_min);
            isp_drc_dark_max_write(be_reg, usr_reg_cfg->mixing_dark_max);
            isp_drc_dark_thr_write(be_reg, usr_reg_cfg->mixing_dark_thr);
            isp_drc_dark_slo_write(be_reg, usr_reg_cfg->mixing_dark_slo);

            isp_drc_bright_min_write(be_reg, usr_reg_cfg->mixing_bright_min);
            isp_drc_bright_max_write(be_reg, usr_reg_cfg->mixing_bright_max);
            isp_drc_bright_thr_write(be_reg, usr_reg_cfg->mixing_bright_thr);
            isp_drc_bright_slo_write(be_reg, usr_reg_cfg->mixing_bright_slo);

            isp_drc_detail_coring_write(be_reg, usr_reg_cfg->detail_coring);
            isp_drc_dark_step_write(be_reg, usr_reg_cfg->detail_dark_step);
            isp_drc_bright_step_write(be_reg, usr_reg_cfg->detail_bright_step);

            isp_drc_detail_dark_min_write(be_reg, usr_reg_cfg->detail_dark_min);
            isp_drc_detail_dark_max_write(be_reg, usr_reg_cfg->detail_dark_max);
            isp_drc_detail_dark_thr_write(be_reg, usr_reg_cfg->detail_dark_thr);
            isp_drc_detail_dark_slo_write(be_reg, usr_reg_cfg->detail_dark_slo);

            isp_drc_detail_bright_min_write(be_reg, usr_reg_cfg->detail_bright_min);
            isp_drc_detail_bright_max_write(be_reg, usr_reg_cfg->detail_bright_max);
            isp_drc_detail_bright_thr_write(be_reg, usr_reg_cfg->detail_bright_thr);
            isp_drc_detail_bright_slo_write(be_reg, usr_reg_cfg->detail_bright_slo);

            /* dark & bright curve write */
            isp_drc_detail_dark_curve0_write(be_reg, usr_reg_cfg->detail_dark_curve[0]); /* array index 0 */
            isp_drc_detail_dark_curve1_write(be_reg, usr_reg_cfg->detail_dark_curve[1]); /* array index 1 */
            isp_drc_detail_dark_curve2_write(be_reg, usr_reg_cfg->detail_dark_curve[2]); /* array index 2 */
            isp_drc_detail_dark_curve3_write(be_reg, usr_reg_cfg->detail_dark_curve[3]); /* array index 3 */
            isp_drc_detail_dark_curve4_write(be_reg, usr_reg_cfg->detail_dark_curve[4]); /* array index 4 */
            isp_drc_detail_dark_curve5_write(be_reg, usr_reg_cfg->detail_dark_curve[5]); /* array index 5 */
            isp_drc_detail_dark_curve6_write(be_reg, usr_reg_cfg->detail_dark_curve[6]); /* array index 6 */
            isp_drc_detail_dark_curve7_write(be_reg, usr_reg_cfg->detail_dark_curve[7]); /* array index 7 */
            isp_drc_detail_dark_curve8_write(be_reg, usr_reg_cfg->detail_dark_curve[8]); /* array index 8 */
            isp_drc_detail_dark_curve9_write(be_reg, usr_reg_cfg->detail_dark_curve[9]); /* array index 9 */
            isp_drc_detail_dark_curve10_write(be_reg, usr_reg_cfg->detail_dark_curve[10]); /* array index 10 */
            isp_drc_detail_dark_curve11_write(be_reg, usr_reg_cfg->detail_dark_curve[11]); /* array index 11 */
            isp_drc_detail_dark_curve12_write(be_reg, usr_reg_cfg->detail_dark_curve[12]); /* array index 12 */
            isp_drc_detail_dark_curve13_write(be_reg, usr_reg_cfg->detail_dark_curve[13]); /* array index 13 */
            isp_drc_detail_dark_curve14_write(be_reg, usr_reg_cfg->detail_dark_curve[14]); /* array index 14 */
            isp_drc_detail_dark_curve15_write(be_reg, usr_reg_cfg->detail_dark_curve[15]); /* array index 15 */

            isp_drc_detail_bright_curve0_write(be_reg, usr_reg_cfg->detail_bright_curve[0]); /* array index 0 */
            isp_drc_detail_bright_curve1_write(be_reg, usr_reg_cfg->detail_bright_curve[1]); /* array index 1 */
            isp_drc_detail_bright_curve2_write(be_reg, usr_reg_cfg->detail_bright_curve[2]); /* array index 2 */
            isp_drc_detail_bright_curve3_write(be_reg, usr_reg_cfg->detail_bright_curve[3]); /* array index 3 */
            isp_drc_detail_bright_curve4_write(be_reg, usr_reg_cfg->detail_bright_curve[4]); /* array index 4 */
            isp_drc_detail_bright_curve5_write(be_reg, usr_reg_cfg->detail_bright_curve[5]); /* array index 5 */
            isp_drc_detail_bright_curve6_write(be_reg, usr_reg_cfg->detail_bright_curve[6]); /* array index 6 */
            isp_drc_detail_bright_curve7_write(be_reg, usr_reg_cfg->detail_bright_curve[7]); /* array index 7 */
            isp_drc_detail_bright_curve8_write(be_reg, usr_reg_cfg->detail_bright_curve[8]); /* array index 8 */
            isp_drc_detail_bright_curve9_write(be_reg, usr_reg_cfg->detail_bright_curve[9]); /* array index 9 */
            isp_drc_detail_bright_curve10_write(be_reg, usr_reg_cfg->detail_bright_curve[10]); /* array index 10 */
            isp_drc_detail_bright_curve11_write(be_reg, usr_reg_cfg->detail_bright_curve[11]); /* array index 11 */
            isp_drc_detail_bright_curve12_write(be_reg, usr_reg_cfg->detail_bright_curve[12]); /* array index 12 */
            isp_drc_detail_bright_curve13_write(be_reg, usr_reg_cfg->detail_bright_curve[13]); /* array index 13 */
            isp_drc_detail_bright_curve14_write(be_reg, usr_reg_cfg->detail_bright_curve[14]); /* array index 14 */
            isp_drc_detail_bright_curve15_write(be_reg, usr_reg_cfg->detail_bright_curve[15]); /* array index 15 */

            isp_drc_cubic_sel_write(be_reg, usr_reg_cfg->cubic_curve_sel);

            /* spatial/range filtering coefficients */
            isp_drc_flt_spa_fine_write(be_reg, usr_reg_cfg->flt_spa_fine);
            isp_drc_flt_spa_medium_write(be_reg, usr_reg_cfg->flt_spa_medium);
            isp_drc_flt_spa_coarse_write(be_reg, usr_reg_cfg->flt_spa_coarse);
            isp_drc_flt_rng_fine_write(be_reg, usr_reg_cfg->flt_rng_fine);
            isp_drc_flt_rng_medium_write(be_reg, usr_reg_cfg->flt_rng_medium);
            isp_drc_flt_rng_coarse_write(be_reg, usr_reg_cfg->flt_rng_coarse);

            /* adaptive range filtering parameters */
            isp_drc_fr_ada_max_write(be_reg, usr_reg_cfg->flt_rng_ada_max);
            isp_drc_dis_offset_coef_write(be_reg, usr_reg_cfg->dis_offset_coef);
            isp_drc_thr_coef_low_write(be_reg, usr_reg_cfg->dis_thr_coef_low);
            isp_drc_thr_coef_high_write(be_reg, usr_reg_cfg->dis_thr_coef_high);

            /* adaptive spatial filtering parameters */
            isp_drc_fs_ada_max_write(be_reg, usr_reg_cfg->flt_spa_ada_max);
            isp_drc_diff_thr_low_write(be_reg, usr_reg_cfg->diff_thr_low);
            isp_drc_diff_thr_high_write(be_reg, usr_reg_cfg->diff_thr_high);

            /* detail suppression parameters */
            isp_drc_suppress_bright_max_write(be_reg, usr_reg_cfg->suppress_bright_max);
            isp_drc_suppress_bright_min_write(be_reg, usr_reg_cfg->suppress_bright_min);
            isp_drc_suppress_bright_thr_write(be_reg, usr_reg_cfg->suppress_bright_thr);
            isp_drc_suppress_bright_slo_write(be_reg, usr_reg_cfg->suppress_bright_slo);
            isp_drc_suppress_dark_max_write(be_reg, usr_reg_cfg->suppress_dark_max);
            isp_drc_suppress_dark_min_write(be_reg, usr_reg_cfg->suppress_dark_min);
            isp_drc_suppress_dark_thr_write(be_reg, usr_reg_cfg->suppress_dark_thr);
            isp_drc_suppress_dark_slo_write(be_reg, usr_reg_cfg->suppress_dark_slo);

            isp_drc_detail_sub_factor_write(be_reg, usr_reg_cfg->detail_sub_factor);

            isp_drc_bin_mix_factor_coarse_0_write(be_reg, usr_reg_cfg->bin_mix_coarse[0]); /* array index 0 */
            isp_drc_bin_mix_factor_coarse_1_write(be_reg, usr_reg_cfg->bin_mix_coarse[1]); /* array index 1 */
            isp_drc_bin_mix_factor_coarse_2_write(be_reg, usr_reg_cfg->bin_mix_coarse[2]); /* array index 2 */
            isp_drc_bin_mix_factor_coarse_3_write(be_reg, usr_reg_cfg->bin_mix_coarse[3]); /* array index 3 */
            isp_drc_bin_mix_factor_coarse_4_write(be_reg, usr_reg_cfg->bin_mix_coarse[4]); /* array index 4 */
            isp_drc_bin_mix_factor_coarse_5_write(be_reg, usr_reg_cfg->bin_mix_coarse[5]); /* array index 5 */
            isp_drc_bin_mix_factor_coarse_6_write(be_reg, usr_reg_cfg->bin_mix_coarse[6]); /* array index 6 */
            isp_drc_bin_mix_factor_coarse_7_write(be_reg, usr_reg_cfg->bin_mix_coarse[7]); /* array index 7 */

            isp_drc_bin_mix_factor_medium_0_write(be_reg, usr_reg_cfg->bin_mix_medium[0]); /* array index 0 */
            isp_drc_bin_mix_factor_medium_1_write(be_reg, usr_reg_cfg->bin_mix_medium[1]); /* array index 1 */
            isp_drc_bin_mix_factor_medium_2_write(be_reg, usr_reg_cfg->bin_mix_medium[2]); /* array index 2 */
            isp_drc_bin_mix_factor_medium_3_write(be_reg, usr_reg_cfg->bin_mix_medium[3]); /* array index 3 */
            isp_drc_bin_mix_factor_medium_4_write(be_reg, usr_reg_cfg->bin_mix_medium[4]); /* array index 4 */
            isp_drc_bin_mix_factor_medium_5_write(be_reg, usr_reg_cfg->bin_mix_medium[5]); /* array index 5 */
            isp_drc_bin_mix_factor_medium_6_write(be_reg, usr_reg_cfg->bin_mix_medium[6]); /* array index 6 */
            isp_drc_bin_mix_factor_medium_7_write(be_reg, usr_reg_cfg->bin_mix_medium[7]); /* array index 7 */

            isp_drc_cclut_reg_config(isp_ctx, be_reg, usr_reg_cfg);
            usr_reg_cfg->usr_resh = offline_mode;
        }

        /* dynamic */
        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.dyna_reg_cfg;

        if (dyna_reg_cfg->dyna_resh) {
            isp_drc_high_thr_write(be_reg, dyna_reg_cfg->pfr_high_thr);
            isp_drc_color_corr_en_write(be_reg, dyna_reg_cfg->color_corr_enable);

            if (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                is_striping_mode(isp_ctx->block_attr.running_mode)) {
                isp_drc_wrstat_en_write(be_reg, reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.drc_en);
                isp_drc_rdstat_en_write(be_reg, reg_cfg_info->alg_reg_cfg[i].drc_reg_cfg.drc_en);
            }

            isp_drc_tmlut_reg_config(&tm_lut_update, isp_ctx, be_reg, dyna_reg_cfg);

            isp_drc_lut_mix_ctrl_write(be_reg, dyna_reg_cfg->lut_mix_ctrl);
            isp_drc_strength_write(be_reg, dyna_reg_cfg->strength);

            if (dyna_reg_cfg->img_size_changed) {
                isp_drc_vbiflt_en_write(be_reg, dyna_reg_cfg->vbiflt_en);
                isp_drc_big_x_init_write(be_reg, dyna_reg_cfg->big_x_init);
                isp_drc_idx_x_init_write(be_reg, dyna_reg_cfg->idx_x_init);
                isp_drc_cnt_x_init_write(be_reg, dyna_reg_cfg->cnt_x_init);
                isp_drc_acc_x_init_write(be_reg, dyna_reg_cfg->acc_x_init);
                isp_drc_blk_wgt_init_write(be_reg, dyna_reg_cfg->wgt_x_init); /* drc v4.0 */
                isp_drc_total_width_write(be_reg, dyna_reg_cfg->total_width - 1);
                isp_drc_stat_width_write(be_reg, dyna_reg_cfg->stat_width - 1);

                isp_drc_hnum_write(be_reg, dyna_reg_cfg->block_h_num);
                isp_drc_vnum_write(be_reg, dyna_reg_cfg->block_v_num);

                isp_drc_zone_hsize_write(be_reg, dyna_reg_cfg->block_h_size - 1);
                isp_drc_zone_vsize_write(be_reg, dyna_reg_cfg->block_v_size - 1);
                isp_drc_chk_x_write(be_reg, dyna_reg_cfg->block_chk_x);
                isp_drc_chk_y_write(be_reg, dyna_reg_cfg->block_chk_y);

                isp_drc_div_x0_write(be_reg, dyna_reg_cfg->div_x0);
                isp_drc_div_x1_write(be_reg, dyna_reg_cfg->div_x1);
                isp_drc_div_y0_write(be_reg, dyna_reg_cfg->div_y0);
                isp_drc_div_y1_write(be_reg, dyna_reg_cfg->div_y1);

                isp_drc_bin_scale_write(be_reg, dyna_reg_cfg->bin_scale);

                dyna_reg_cfg->img_size_changed = offline_mode;
            }

            dyna_reg_cfg->dyna_resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_drc_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.drc_tm_lut_update = tm_lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_dehaze_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool offline_mode;
    hi_u8   buf_id;
    hi_u8   sys_blk_num = reg_cfg_info->cfg_num;
    hi_u16  blk_num;
    hi_s32  ret = HI_SUCCESS;
    isp_dehaze_static_cfg *static_reg_cfg = HI_NULL;
    isp_dehaze_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    S_ISP_LUT_WSTT_TYPE   *be_lut_stt_reg = HI_NULL;
    S_VIPROC_REGS_TYPE    *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE     *be_reg         = HI_NULL;
    isp_usr_ctx           *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_dehaze_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].dehaze_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].dehaze_reg_cfg.dyna_reg_cfg;

        blk_num = ((static_reg_cfg->dchnum + 1) * (static_reg_cfg->dcvnum + 1) + 1) / 2; /* blk caculation 2 */
        /* static registers */
        if (static_reg_cfg->resh) {
            isp_dehaze_max_mode_write(be_reg, static_reg_cfg->max_mode);
            isp_dehaze_thld_write(be_reg, static_reg_cfg->dehaze_thld);
            isp_dehaze_blthld_write(be_reg, static_reg_cfg->dehaze_blthld);
            isp_dehaze_neg_mode_write(be_reg, static_reg_cfg->dehaze_neg_mode);
            isp_dehaze_block_sum_write(be_reg, static_reg_cfg->block_sum);
            isp_dehaze_dc_numh_write(be_reg, static_reg_cfg->dchnum);
            isp_dehaze_dc_numv_write(be_reg, static_reg_cfg->dcvnum);
            isp_dehaze_lut_width_word_write(vi_proc_reg, HI_ISP_DEHAZE_LUT_WIDTH_WORD_DEFAULT);
            static_reg_cfg->resh = HI_FALSE;
        }

        /* dynamic regs */
        if (is_online_mode(isp_ctx->block_attr.running_mode) ||
            is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
            /* online lut2stt regconfig */
            buf_id = dyna_reg_cfg->buf_id;

            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
            isp_check_pointer_return(be_lut_stt_reg);

            isp_dehaze_lut_wstt_write(be_lut_stt_reg, blk_num, dyna_reg_cfg->prestat, dyna_reg_cfg->lut);
            ret = isp_dehaze_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("isp[%d]_dehaze_lut_wstt_addr_write failed\n", vi_pipe);
            }
            isp_dehaze_stt2lut_en_write(be_reg, HI_TRUE);
            isp_dehaze_stt2lut_regnew_write(be_reg, HI_TRUE);
            dyna_reg_cfg->buf_id = 1 - buf_id;
        } else {
            isp_dehaze_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, blk_num,
                                      dyna_reg_cfg->prestat, dyna_reg_cfg->lut);
            isp_dehaze_stt2lut_en_write(be_reg, HI_TRUE);
            isp_dehaze_stt2lut_regnew_write(be_reg, HI_TRUE);
        }

        isp_dehaze_block_sizeh_write(be_reg, dyna_reg_cfg->blockhsize);
        isp_dehaze_block_sizev_write(be_reg, dyna_reg_cfg->blockvsize);
        isp_dehaze_phase_x_write(be_reg, dyna_reg_cfg->phasex);
        isp_dehaze_phase_y_write(be_reg, dyna_reg_cfg->phasey);

        isp_dehaze_smlmapoffset_write(be_reg, dyna_reg_cfg->sml_map_offset);
        isp_dehaze_statstartx_write(be_reg, dyna_reg_cfg->stat_start_x);
        isp_dehaze_statendx_write(be_reg, dyna_reg_cfg->stat_end_x);

        isp_dehaze_stat_numv_write(be_reg, dyna_reg_cfg->statnum_v);
        isp_dehaze_stat_numh_write(be_reg, dyna_reg_cfg->statnum_h);

        reg_cfg_info->cfg_key.bit1_dehaze_cfg  = offline_mode ? 1 : ((sys_blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_void isp_bnr_lmt_reg_config(isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                      isp_bayernr_dyna_cfg *dyna_reg_cfg)
{
    hi_u16   j;

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_bnr_lmt_odd_waddr_write(be_reg, 0);
        isp_bnr_lmt_s_odd_waddr_write(be_reg, 0);

        for (j = 1; j < HI_ISP_BAYERNR_LMTLUTNUM; j += 2) { /* bayer nr lut step 2 */
            isp_bnr_lmt_odd_wdata_write(be_reg, dyna_reg_cfg->jnlm_limit_lut[j]);
            isp_bnr_lmt_s_odd_wdata_write(be_reg, dyna_reg_cfg->jnlm_limit_s_lut[j]);
        }

        isp_bnr_lmt_even_waddr_write(be_reg, 0);
        isp_bnr_lmt_s_even_waddr_write(be_reg, 0);

        for (j = 0; j < HI_ISP_BAYERNR_LMTLUTNUM; j += 2) { /* bayer nr lut step 2 */
            isp_bnr_lmt_even_wdata_write(be_reg, dyna_reg_cfg->jnlm_limit_lut[j]);
            isp_bnr_lmt_s_even_wdata_write(be_reg, dyna_reg_cfg->jnlm_limit_s_lut[j]);
        }

        isp_bnr_cor_odd_waddr_write(be_reg, 0);
        isp_bnr_gcor_odd_waddr_write(be_reg, 0);

        for (j = 1; j < HI_ISP_BAYERNR_LUT_LENGTH; j += 2) { /* bayer nr lut step 2 */
            isp_bnr_cor_odd_wdata_write(be_reg, dyna_reg_cfg->jnlm_coring_low_lut[j]);
            isp_bnr_gcor_odd_wdata_write(be_reg, dyna_reg_cfg->g_coring_gain_lut[j]);
        }

        isp_bnr_cor_even_waddr_write(be_reg, 0);
        isp_bnr_gcor_even_waddr_write(be_reg, 0);

        for (j = 0; j < HI_ISP_BAYERNR_LUT_LENGTH; j += 2) { /* bayer nr lut step 2 */
            isp_bnr_cor_even_wdata_write(be_reg, dyna_reg_cfg->jnlm_coring_low_lut[j]);
            isp_bnr_gcor_even_wdata_write(be_reg, dyna_reg_cfg->g_coring_gain_lut[j]);
        }
    } else {
        isp_bnr_lmt_even_write(be_reg, dyna_reg_cfg->jnlm_limit_lut);
        isp_bnr_lmt_odd_write(be_reg, dyna_reg_cfg->jnlm_limit_lut);
        isp_bnr_cor_even_write(be_reg, dyna_reg_cfg->jnlm_coring_low_lut);
        isp_bnr_cor_odd_write(be_reg, dyna_reg_cfg->jnlm_coring_low_lut);
        isp_bnr_gcor_even_write(be_reg, dyna_reg_cfg->g_coring_gain_lut);
        isp_bnr_gcor_odd_write(be_reg, dyna_reg_cfg->g_coring_gain_lut);
        isp_bnr_lmt_s_odd_write(be_reg, dyna_reg_cfg->jnlm_limit_s_lut);
        isp_bnr_lmt_s_even_write(be_reg, dyna_reg_cfg->jnlm_limit_s_lut);
    }
}

static hi_s32 isp_bayer_nr_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_bool  offline_mode;
    hi_bool  lut_update = HI_FALSE;
    isp_bayernr_static_cfg *static_reg_cfg = HI_NULL;
    isp_bayernr_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_bayernr_usr_cfg    *usr_reg_cfg    = HI_NULL;
    S_ISPBE_REGS_TYPE      *be_reg         = HI_NULL;
    isp_usr_ctx            *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_bayernr_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].bnr_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].bnr_reg_cfg.dyna_reg_cfg;
        usr_reg_cfg    = &reg_cfg_info->alg_reg_cfg[i].bnr_reg_cfg.usr_reg_cfg;

        if (static_reg_cfg->resh) {         /* satic */
            isp_bnr_jnlm_sel_write(be_reg, static_reg_cfg->jnlm_sel);
            isp_bnr_sad_winsize_sel_write(be_reg, static_reg_cfg->sad_win_size_sel);
            isp_bnr_jnlm_maxwtcoef_write(be_reg, static_reg_cfg->jnlm_max_wt_coef);
            static_reg_cfg->resh = HI_FALSE;
        }

        if (dyna_reg_cfg->resh) {
            isp_bnr_medc_enable_write(be_reg, dyna_reg_cfg->medc_enable);
            isp_bnr_wdr_enfusion_write(be_reg, dyna_reg_cfg->wdr_fusion_en);
            isp_bnr_encenterweight_write(be_reg, dyna_reg_cfg->center_wgt_en);
            isp_bnr_wdr_mapgain_write(be_reg, dyna_reg_cfg->wdr_map_gain);
            isp_bnr_wdr_mapfltmod_write(be_reg, dyna_reg_cfg->wdr_map_flt_mode);
            isp_bnr_ratio_r_write(be_reg, dyna_reg_cfg->c_ratio[0]); /* array index 0 */
            isp_bnr_ratio_gr_write(be_reg, dyna_reg_cfg->c_ratio[1]); /* array index 1 */
            isp_bnr_ratio_gb_write(be_reg, dyna_reg_cfg->c_ratio[2]); /* array index 2 */
            isp_bnr_ratio_b_write(be_reg, dyna_reg_cfg->c_ratio[3]); /* array index 3 */
            isp_bnr_amed_mode_r_write(be_reg, dyna_reg_cfg->amed_mode[0]); /* array index 0 */
            isp_bnr_amed_mode_gr_write(be_reg, dyna_reg_cfg->amed_mode[1]); /* array index 1 */
            isp_bnr_amed_mode_gb_write(be_reg, dyna_reg_cfg->amed_mode[2]); /* array index 2 */
            isp_bnr_amed_mode_b_write(be_reg, dyna_reg_cfg->amed_mode[3]); /* array index 3 */
            isp_bnr_amed_lev_r_write(be_reg, dyna_reg_cfg->amed_level[0]); /* array index 0 */
            isp_bnr_amed_lev_gr_write(be_reg, dyna_reg_cfg->amed_level[1]); /* array index 1 */
            isp_bnr_amed_lev_gb_write(be_reg, dyna_reg_cfg->amed_level[2]); /* array index 2 */
            isp_bnr_amed_lev_b_write(be_reg, dyna_reg_cfg->amed_level[3]); /* array index 3 */
            isp_bnr_jnlm_symcoef_write(be_reg, dyna_reg_cfg->jnlm_sym_coef);
            isp_bnr_jnlm_gain_write(be_reg, dyna_reg_cfg->jnlm_gain);
            isp_bnr_jnlm_coringhig_write(be_reg, dyna_reg_cfg->jnlm_coring_hig);
            isp_bnr_shotratio_write(be_reg, dyna_reg_cfg->shot_ratio);
            isp_bnr_jnlm_edgedetth_write(be_reg, dyna_reg_cfg->edge_det_thr);
            isp_bnr_rlmt_rgain_write(be_reg, dyna_reg_cfg->r_lmt_rgain);
            isp_bnr_rlmt_bgain_write(be_reg, dyna_reg_cfg->r_lmt_bgain);

            isp_bnr_lmt_reg_config(isp_ctx, be_reg, dyna_reg_cfg);
            lut_update = HI_TRUE;

            isp_bnr_jnlmgain_r0_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain[BAYER_RGGB]);
            isp_bnr_jnlmgain_gr0_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain[BAYER_GRBG]);
            isp_bnr_jnlmgain_gb0_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain[BAYER_GBRG]);
            isp_bnr_jnlmgain_b0_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain[BAYER_BGGR]);
            isp_bnr_jnlmgain_r_s_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain_s[BAYER_RGGB]);
            isp_bnr_jnlmgain_gr_s_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain_s[BAYER_GRBG]);
            isp_bnr_jnlmgain_gb_s_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain_s[BAYER_GBRG]);
            isp_bnr_jnlmgain_b_s_write(be_reg, dyna_reg_cfg->jnlm_limit_mult_gain_s[BAYER_BGGR]);

            dyna_reg_cfg->resh = offline_mode;
        }

        if (usr_reg_cfg->resh) {
            isp_bnr_mono_sensor_write(be_reg, usr_reg_cfg->bnr_mono_sensor_en);
            isp_bnr_rlsc_maxgain_write(be_reg, usr_reg_cfg->bnr_lsc_max_gain);
            isp_bnr_rlsc_cmp_str_write(be_reg, usr_reg_cfg->bnr_lsc_cmp_strength);

            usr_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_bayernr_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.bnr_lut_update = lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_bnr_rlsc_lut_reg_config(hi_bool *stt2_lut_regnew, hi_vi_pipe vi_pipe, hi_u8 i,
                                          S_ISPBE_REGS_TYPE *be_reg, isp_rlsc_usr_cfg *usr_reg_cfg)
{
    hi_u8 buf_id;
    S_VIPROC_REGS_TYPE  *vi_proc_reg    = HI_NULL;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg = HI_NULL;
    isp_usr_ctx         *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        /* online lut2stt regconfig */
        buf_id = usr_reg_cfg->buf_id;

        be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
        isp_check_pointer_return(be_lut_stt_reg);
        isp_bnr_lut_wstt_write(be_lut_stt_reg,
                               usr_reg_cfg->gain_lut[0], usr_reg_cfg->gain_lut[1],  /* 0:channel R, 1: channel Gr */
                               usr_reg_cfg->gain_lut[2], usr_reg_cfg->gain_lut[3]);  /* 2: channel Gb, 3: channel B */

        isp_bnr_rlsc_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);

        isp_bnr_stt2lut_en_write(be_reg, HI_TRUE);

        usr_reg_cfg->buf_id = 1 - buf_id;

        *stt2_lut_regnew = HI_TRUE;
    } else {
        isp_bnr_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt,
                               usr_reg_cfg->gain_lut[0], usr_reg_cfg->gain_lut[1], /* 0: channel R, 1: channel Gr */
                               usr_reg_cfg->gain_lut[2],  usr_reg_cfg->gain_lut[3]); /* 2: channel Gb, 3: channel B */
        isp_bnr_stt2lut_en_write(be_reg, HI_TRUE);
        isp_bnr_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_bnr_lsc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh = HI_FALSE;
    hi_bool  idx_resh = HI_FALSE;
    hi_bool  stt2_lut_regnew = HI_FALSE;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_s32   ret;
    isp_rlsc_usr_cfg    *usr_reg_cfg    = HI_NULL;
    isp_rlsc_static_cfg *static_reg_cfg = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE   *be_reg         = HI_NULL;
    isp_usr_ctx         *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_rlsc_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);
        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->static_resh) {
            isp_bnr_rlsc_nodenum_write(be_reg, static_reg_cfg->node_num);
            isp_bnr_lut_width_word_write(vi_proc_reg, HI_ISP_BNR_RLSC_LUT_WIDTH_WORD_DEFAULT);
            static_reg_cfg->static_resh = HI_FALSE;
        }

        /* usr */
        usr_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg;

        isp_bnr_rlsc_rx_write(be_reg, usr_reg_cfg->center_rx);
        isp_bnr_rlsc_ry_write(be_reg, usr_reg_cfg->center_ry);
        isp_bnr_rlsc_grx_write(be_reg, usr_reg_cfg->center_gr_x);
        isp_bnr_rlsc_gry_write(be_reg, usr_reg_cfg->center_gr_y);
        isp_bnr_rlsc_gbx_write(be_reg, usr_reg_cfg->center_gb_x);
        isp_bnr_rlsc_gby_write(be_reg, usr_reg_cfg->center_gb_y);
        isp_bnr_rlsc_bx_write(be_reg, usr_reg_cfg->center_bx);
        isp_bnr_rlsc_by_write(be_reg, usr_reg_cfg->center_by);

        isp_bnr_rlsc_offsetcenterr_write(be_reg, usr_reg_cfg->off_center_r);
        isp_bnr_rlsc_offsetcentergr_write(be_reg, usr_reg_cfg->off_center_gr);
        isp_bnr_rlsc_offsetcentergb_write(be_reg, usr_reg_cfg->off_center_gb);
        isp_bnr_rlsc_offsetcenterb_write(be_reg, usr_reg_cfg->off_center_b);

        if (usr_reg_cfg->coef_update) {
            isp_bnr_rlsc_gainstr_write(be_reg, usr_reg_cfg->gain_str);

            usr_reg_cfg->coef_update = offline_mode;
        }

        idx_resh = (isp_rlsc_update_index_read(be_reg) != usr_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (usr_reg_cfg->lut_update && idx_resh) : (usr_reg_cfg->lut_update);

        if (usr_resh) {
            isp_rlsc_update_index_write(be_reg, usr_reg_cfg->update_index);
            isp_bnr_rlsc_gainscale_write(be_reg, usr_reg_cfg->gain_scale);
            isp_bnr_rlsc_widthoffset_write(be_reg, usr_reg_cfg->width_offset);

            ret = isp_bnr_rlsc_lut_reg_config(&stt2_lut_regnew, vi_pipe, i, be_reg, usr_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }
            usr_reg_cfg->lut_update = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_rlsc_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.bnr_lsc_stt2lut_regnew = stt2_lut_regnew;

    if (reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_lut2_stt_reg_new == HI_TRUE) {
        if (reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt < 3) { /* reg new count 3 */
            reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.bnr_lsc_stt2lut_regnew = HI_TRUE;
            reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt++;
        } else {
            reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_lut2_stt_reg_new = HI_FALSE;
            reg_cfg_info->alg_reg_cfg[i].rlsc_reg_cfg.usr_reg_cfg.switch_reg_new_cnt    = 0;
        }
    }

    return HI_SUCCESS;
}

static hi_void isp_de_luma_gain_reg_config(isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                           isp_detail_usr_cfg *usr_reg_cfg)
{
    hi_u16 j;
    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_de_luma_gain_waddr_write(be_reg, 0);
        for (j = 0; j < HI_ISP_DE_LUMA_GAIN_LUT_N; j += 1) {
            isp_de_luma_gain_wdata_write(be_reg, usr_reg_cfg->luma_gain_lut[j]);
        }
    } else {
        isp_de_luma_gain_write(be_reg, usr_reg_cfg->luma_gain_lut);
    }
}

static hi_s32 isp_detail_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_bool  offline_mode;
    hi_bool  lut_update = HI_FALSE;
    isp_detail_static_cfg *static_reg_cfg = HI_NULL;
    isp_detail_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_detail_usr_cfg    *usr_reg_cfg    = HI_NULL;
    S_ISPBE_REGS_TYPE     *be_reg         = HI_NULL;
    isp_usr_ctx           *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_detail_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].de_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].de_reg_cfg.dyna_reg_cfg;
        usr_reg_cfg    = &reg_cfg_info->alg_reg_cfg[i].de_reg_cfg.usr_reg_cfg;

        if (static_reg_cfg->resh) {
            isp_bnr_de_posclip_write(be_reg, static_reg_cfg->de_pos_clip);
            isp_bnr_de_negclip_write(be_reg, static_reg_cfg->de_neg_clip);
            isp_de_clip_ratio_write(be_reg, static_reg_cfg->clip_ratio);
            isp_de_luma_scale_x0_write(be_reg, static_reg_cfg->luma_scale_x0);
            isp_de_luma_scale_x1_write(be_reg, static_reg_cfg->luma_scale_x1);
            isp_de_luma_scale_y0_write(be_reg, static_reg_cfg->luma_scale_y0);
            isp_de_luma_scale_y1_write(be_reg, static_reg_cfg->luma_scale_y1);
            isp_de_luma_scale_k_write(be_reg, static_reg_cfg->luma_scale_k);
            isp_de_satugain_x0_write(be_reg, static_reg_cfg->satu_gain_x0);
            isp_de_satugain_x1_write(be_reg, static_reg_cfg->satu_gain_x1);
            isp_de_satugain_y0_write(be_reg, static_reg_cfg->satu_gain_y0);
            isp_de_satugain_y1_write(be_reg, static_reg_cfg->satu_gain_y1);
            isp_de_satugain_k_write(be_reg, static_reg_cfg->satu_gain_k);
            isp_de_edge_scale_x0_write(be_reg, static_reg_cfg->edge_scale_x0);
            isp_de_edge_scale_x1_write(be_reg, static_reg_cfg->edge_scale_x1);
            isp_de_edge_scale_y0_write(be_reg, static_reg_cfg->edge_scale_y0);
            isp_de_edge_scale_y1_write(be_reg, static_reg_cfg->edge_scale_y1);
            isp_de_edge_scale_k_write(be_reg, static_reg_cfg->edge_scale_k);
            isp_de_global_ratio_write(be_reg, static_reg_cfg->global_ratio);
            isp_de_sgm_mode_write(be_reg, static_reg_cfg->sgm_mode);
            isp_de_edge_mode_write(be_reg, static_reg_cfg->edge_mode);
            isp_de_lumagain_mode_write(be_reg, static_reg_cfg->luma_gain_mode);
            isp_de_lumabit_mode_write(be_reg, static_reg_cfg->luma_bit_mode);

            static_reg_cfg->resh = HI_FALSE;
        }

        if (dyna_reg_cfg->resh) {
            isp_de_sgm_pos4_gain_write(be_reg, dyna_reg_cfg->sgm_pos4_gain);
            isp_de_sgm_pos3_gain_write(be_reg, dyna_reg_cfg->sgm_pos3_gain);
            isp_de_sgm_pos2_gain_write(be_reg, dyna_reg_cfg->sgm_pos2_gain);
            isp_de_sgm_neg4_gain_write(be_reg, dyna_reg_cfg->sgm_neg4_gain);
            isp_de_sgm_neg3_gain_write(be_reg, dyna_reg_cfg->sgm_neg3_gain);
            isp_de_sgm_neg2_gain_write(be_reg, dyna_reg_cfg->sgm_neg2_gain);
            isp_de_gain_lf_write(be_reg, dyna_reg_cfg->gain_lf);
            isp_de_gain_hf_pos_write(be_reg, dyna_reg_cfg->gain_hf_pos);
            isp_de_gain_hf_neg_write(be_reg, dyna_reg_cfg->gain_hf_neg);

            dyna_reg_cfg->resh = offline_mode;
        }

        if (usr_reg_cfg->resh) {
            isp_de_luma_gain_reg_config(isp_ctx, be_reg, usr_reg_cfg);
            lut_update = HI_TRUE;
            usr_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_detail_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg.de_lut_update = lut_update || offline_mode;

    return HI_SUCCESS;
}

static hi_s32 isp_dg_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_bool  offline_mode;
    isp_dg_static_cfg  *static_reg_cfg = HI_NULL;
    isp_dg_dyna_cfg    *dyna_reg_cfg   = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg    = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg         = HI_NULL;
    isp_usr_ctx        *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_dg_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);
        isp_dg_en_write(vi_proc_reg, reg_cfg_info->alg_reg_cfg[i].dg_reg_cfg.dg_en);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].dg_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].dg_reg_cfg.dyna_reg_cfg;

        if (static_reg_cfg->resh) {
            isp_dg_rgain_write(be_reg,  dyna_reg_cfg->gain_r);
            isp_dg_grgain_write(be_reg, dyna_reg_cfg->gain_gr);
            isp_dg_gbgain_write(be_reg, dyna_reg_cfg->gain_gb);
            isp_dg_bgain_write(be_reg,  dyna_reg_cfg->gain_b);

            static_reg_cfg->resh = HI_FALSE;
        }

        if (dyna_reg_cfg->resh) {
            isp_dg_clip_value_write(be_reg, dyna_reg_cfg->clip_value);
            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_dg_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_4dg_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    hi_bool  offline_mode;
    isp_4dg_static_cfg *static_reg_cfg = HI_NULL;
    isp_4dg_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_usr_ctx        *isp_ctx        = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg         = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_wdr_dg_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        /* static */
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].four_dg_reg_cfg.static_reg_cfg;

        if (static_reg_cfg->resh) {
            isp_4dg0_rgain_write(be_reg, static_reg_cfg->gain_r0);
            isp_4dg0_grgain_write(be_reg, static_reg_cfg->gain_gr0);
            isp_4dg0_gbgain_write(be_reg, static_reg_cfg->gain_gb0);
            isp_4dg0_bgain_write(be_reg, static_reg_cfg->gain_b0);
            isp_4dg1_rgain_write(be_reg, static_reg_cfg->gain_r1);
            isp_4dg1_grgain_write(be_reg, static_reg_cfg->gain_gr1);
            isp_4dg1_gbgain_write(be_reg, static_reg_cfg->gain_gb1);
            isp_4dg1_bgain_write(be_reg, static_reg_cfg->gain_b1);
            static_reg_cfg->resh = HI_FALSE;
        }

        /* dynamic */
        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].four_dg_reg_cfg.dyna_reg_cfg;

        if (dyna_reg_cfg->resh) {
            isp_4dg0_clip_value_write(be_reg, dyna_reg_cfg->clip_value0);
            isp_4dg1_clip_value_write(be_reg, dyna_reg_cfg->clip_value1);

            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_wdr_dg_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_flick_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool offline_mode;
    hi_u8   blk_num = reg_cfg_info->cfg_num;
    S_VIPROC_REGS_TYPE    *vi_proc_reg  = HI_NULL;
    isp_flicker_dyna_cfg  *dyna_reg_cfg = HI_NULL;
    isp_usr_ctx           *isp_ctx      = HI_NULL;
    S_ISPBE_REGS_TYPE     *be_reg       = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_flicker_cfg) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);

        isp_flicker_en_write(vi_proc_reg, reg_cfg_info->alg_reg_cfg[i].flicker_reg_cfg.enable);

        dyna_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].flicker_reg_cfg.dyna_reg_cfg;

        if (dyna_reg_cfg->resh) {
            isp_flick_overth_write(be_reg, dyna_reg_cfg->over_thr);
            isp_flick_gravg_pre_write(be_reg, dyna_reg_cfg->gr_avg_pre);
            isp_flick_gbavg_pre_write(be_reg, dyna_reg_cfg->gb_avg_pre);

            dyna_reg_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_flicker_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
static hi_s32 isp_pregamma_lut_reg_config(hi_bool *stt2_lut_regnew, hi_vi_pipe vi_pipe, hi_u8 i,
                                          S_ISPBE_REGS_TYPE *be_reg, isp_pregamma_dyna_cfg *dyna_reg_cfg)
{
    hi_u8   buf_id;
    hi_s32  ret;
    S_ISP_LUT_WSTT_TYPE *be_lut_stt_reg = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg    = HI_NULL;
    isp_usr_ctx         *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(vi_proc_reg);

    isp_pregamma_lut_width_word_write(vi_proc_reg, HI_ISP_PREGAMMA_LUT_WIDTH_WORD_DEFAULT);

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        if (dyna_reg_cfg->switch_mode != HI_TRUE) {
            /* online lut2stt regconfig */
            buf_id = dyna_reg_cfg->buf_id;

            be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
            isp_check_pointer_return(be_lut_stt_reg);

            isp_pregamma_lut_wstt_write(be_lut_stt_reg, dyna_reg_cfg->pregamma_lut);
            ret = isp_pregamma_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("isp[%d]_pregamma_lut_wstt_addr_write failed\n", vi_pipe);
                return ret;
            }
            isp_pregamma_stt2lut_en_write(be_reg, HI_TRUE);
            dyna_reg_cfg->buf_id = 1 - buf_id;
            *stt2_lut_regnew = HI_TRUE;
        }
    } else {
        isp_pregamma_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, dyna_reg_cfg->pregamma_lut);
        isp_pregamma_stt2lut_en_write(be_reg, HI_TRUE);
        isp_pregamma_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}
#endif

static hi_s32 isp_pregamma_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    hi_bool offline_mode;
    hi_bool usr_resh;
    hi_bool idx_resh;
    hi_bool stt2_lut_regnew = HI_FALSE;
    hi_u8   blk_num = reg_cfg_info->cfg_num;
    hi_s32  ret;
    isp_pregamma_dyna_cfg   *dyna_reg_cfg   = HI_NULL;
    isp_pregamma_static_cfg *static_reg_cfg = HI_NULL;
    S_ISPBE_REGS_TYPE       *be_reg         = HI_NULL;
    isp_usr_ctx             *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_pregamma_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        dyna_reg_cfg   = &reg_cfg_info->alg_reg_cfg[i].pregamma_reg_cfg.dyna_reg_cfg;
        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].pregamma_reg_cfg.static_reg_cfg;

        /* enable pregamma */
        if (static_reg_cfg->static_resh) {
            isp_pregamma_idxbase_write(be_reg, static_reg_cfg->seg_idx_base);
            isp_pregamma_maxval_write(be_reg, static_reg_cfg->seg_max_val);

            static_reg_cfg->static_resh = HI_FALSE;
        }

        idx_resh = (isp_pregamma_update_index_read(be_reg) != dyna_reg_cfg->update_index);
        usr_resh = (offline_mode) ? (dyna_reg_cfg->pregamma_lut_update_en && idx_resh) :
            (dyna_reg_cfg->pregamma_lut_update_en);

        /* LUT update */
        if (usr_resh) {
            isp_pregamma_update_index_write(be_reg, dyna_reg_cfg->update_index);

            ret = isp_pregamma_lut_reg_config(&stt2_lut_regnew, vi_pipe, i, be_reg, dyna_reg_cfg);
            if (ret != HI_SUCCESS) {
                return ret;
            }

            dyna_reg_cfg->pregamma_lut_update_en = offline_mode;
            dyna_reg_cfg->switch_mode            = HI_FALSE;
        }

        reg_cfg_info->cfg_key.bit1_pregamma_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.pre_gamma_stt2lut_regnew = stt2_lut_regnew;

#endif
    return HI_SUCCESS;
}

static hi_s32 isp_be_blc_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool offline_mode;
    hi_u8   blk_num = reg_cfg_info->cfg_num;
    isp_be_blc_static_cfg *static_cfg = HI_NULL;
    isp_be_blc_dyna_cfg   *dyna_cfg   = HI_NULL;
    isp_usr_ctx       *isp_ctx = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_be_blc_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        static_cfg = &reg_cfg_info->alg_reg_cfg[i].be_blc_cfg.static_blc;
        dyna_cfg   = &reg_cfg_info->alg_reg_cfg[i].be_blc_cfg.dyna_blc;

        if (static_cfg->resh_static) {
            /* 4Dg */
            isp_4dg_en_in_write(be_reg, static_cfg->wdr_dg_blc[0].blc_in);
            isp_4dg_en_out_write(be_reg, static_cfg->wdr_dg_blc[0].blc_out);

            /* WDR */
            isp_wdr_bsaveblc_write(be_reg, static_cfg->wdr_blc[0].blc_out);

            /* lsc */
            isp_lsc_blc_in_en_write(be_reg, static_cfg->lsc_blc.blc_in);
            isp_lsc_blc_out_en_write(be_reg, static_cfg->lsc_blc.blc_out);
            /* Dg */
            isp_dg_en_in_write(be_reg, static_cfg->dg_blc.blc_in);
            isp_dg_en_out_write(be_reg, static_cfg->dg_blc.blc_out);
            /* AE */
            isp_ae_blc_en_write(be_reg, static_cfg->ae_blc.blc_in);
            /* MG */
            isp_la_blc_en_write(be_reg, static_cfg->mg_blc.blc_in);
            /* WB */
            isp_wb_en_in_write(be_reg, static_cfg->wb_blc.blc_in);
            isp_wb_en_out_write(be_reg, static_cfg->wb_blc.blc_out);

            static_cfg->resh_static = offline_mode;
        }

        /* 4Dg */
        if (reg_cfg_info->alg_reg_cfg[i].be_blc_cfg.resh_dyna_init == HI_TRUE) {
            isp_4dg0_ofsr_write(be_reg, dyna_cfg->wdr_dg_blc[0].blc[0]); /* index 0 ,0 */
            isp_4dg0_ofsgr_write(be_reg, dyna_cfg->wdr_dg_blc[0].blc[1]); /* index 0 ,1 */
            isp_4dg0_ofsgb_write(be_reg, dyna_cfg->wdr_dg_blc[0].blc[2]); /* index 0 ,2 */
            isp_4dg0_ofsb_write(be_reg, dyna_cfg->wdr_dg_blc[0].blc[3]); /* index 0 ,3 */

            isp_4dg1_ofsr_write(be_reg, dyna_cfg->wdr_dg_blc[1].blc[0]); /* index 1 ,0 */
            isp_4dg1_ofsgr_write(be_reg, dyna_cfg->wdr_dg_blc[1].blc[1]); /* index 1 ,1 */
            isp_4dg1_ofsgb_write(be_reg, dyna_cfg->wdr_dg_blc[1].blc[2]); /* index 1 ,2 */
            isp_4dg1_ofsb_write(be_reg, dyna_cfg->wdr_dg_blc[1].blc[3]); /* index 1 ,3 */

            /* WDR */
            isp_wdr_outblc_write(be_reg, dyna_cfg->wdr_blc[0].out_blc);
            isp_wdr_f0_inblc_r_write(be_reg, dyna_cfg->wdr_blc[0].blc[0]); /* index 0 ,0 */
            isp_wdr_f0_inblc_gr_write(be_reg, dyna_cfg->wdr_blc[0].blc[1]); /* index 0 ,1 */
            isp_wdr_f0_inblc_gb_write(be_reg, dyna_cfg->wdr_blc[0].blc[2]); /* index 0 ,2 */
            isp_wdr_f0_inblc_b_write(be_reg, dyna_cfg->wdr_blc[0].blc[3]); /* index 0 ,3 */

            isp_wdr_f1_inblc_r_write(be_reg, dyna_cfg->wdr_blc[1].blc[0]); /* index 1 ,0 */
            isp_wdr_f1_inblc_gr_write(be_reg, dyna_cfg->wdr_blc[1].blc[1]); /* index 1 ,1 */
            isp_wdr_f1_inblc_gb_write(be_reg, dyna_cfg->wdr_blc[1].blc[2]); /* index 1 ,2 */
            isp_wdr_f1_inblc_b_write(be_reg, dyna_cfg->wdr_blc[1].blc[3]); /* index 1 ,3 */

            /* flicker */
            isp_flick_f0_inblc_r_write(be_reg, dyna_cfg->flicker_blc[0].blc[0]); /* index 0 ,0 */
            isp_flick_f0_inblc_gr_write(be_reg, dyna_cfg->flicker_blc[0].blc[1]); /* index 0 ,1 */
            isp_flick_f0_inblc_gb_write(be_reg, dyna_cfg->flicker_blc[0].blc[2]); /* index 0 ,2 */
            isp_flick_f0_inblc_b_write(be_reg, dyna_cfg->flicker_blc[0].blc[3]); /* index 0 ,3 */

            isp_flick_f1_inblc_r_write(be_reg, dyna_cfg->flicker_blc[1].blc[0]); /* index 1 ,0 */
            isp_flick_f1_inblc_gr_write(be_reg, dyna_cfg->flicker_blc[1].blc[1]); /* index 1 ,1 */
            isp_flick_f1_inblc_gb_write(be_reg, dyna_cfg->flicker_blc[1].blc[2]); /* index 1 ,2 */
            isp_flick_f1_inblc_b_write(be_reg, dyna_cfg->flicker_blc[1].blc[3]); /* index 1 ,3 */

            /* expander */
            isp_expander_offset_r_write(be_reg, dyna_cfg->expander_blc.blc[0]); /* array index 0 */
            isp_expander_offset_gr_write(be_reg, dyna_cfg->expander_blc.blc[1]); /* array index 1 */
            isp_expander_offset_gb_write(be_reg, dyna_cfg->expander_blc.blc[2]); /* array index 2 */
            isp_expander_offset_b_write(be_reg, dyna_cfg->expander_blc.blc[3]); /* array index 3 */

            /* bnr */
            isp_bnr_rlmt_blc_write(be_reg, (dyna_cfg->bnr_blc.blc[0] >> 2)); /* index 0  shift 2 bit */

            /* lsc */
            isp_lsc_blc_r_write(be_reg, dyna_cfg->lsc_blc.blc[0]); /* array index 0 */
            isp_lsc_blc_gr_write(be_reg, dyna_cfg->lsc_blc.blc[1]); /* array index 1 */
            isp_lsc_blc_gb_write(be_reg, dyna_cfg->lsc_blc.blc[2]); /* array index 2 */
            isp_lsc_blc_b_write(be_reg, dyna_cfg->lsc_blc.blc[3]); /* array index 3 */

            /* Dg */
            isp_dg_ofsr_write(be_reg, dyna_cfg->dg_blc.blc[0]); /* array index 0 */
            isp_dg_ofsgr_write(be_reg, dyna_cfg->dg_blc.blc[1]); /* array index 1 */
            isp_dg_ofsgb_write(be_reg, dyna_cfg->dg_blc.blc[2]); /* array index 2 */
            isp_dg_ofsb_write(be_reg, dyna_cfg->dg_blc.blc[3]); /* array index 3 */

            /* AE */
            isp_ae_offset_r_write(be_reg, dyna_cfg->ae_blc.blc[0]); /* array index 0 */
            isp_ae_offset_gr_write(be_reg, dyna_cfg->ae_blc.blc[1]); /* array index 1 */
            isp_ae_offset_gb_write(be_reg, dyna_cfg->ae_blc.blc[2]); /* array index 2 */
            isp_ae_offset_b_write(be_reg, dyna_cfg->ae_blc.blc[3]); /* array index 3 */
            /* MG */
            isp_la_offset_r_write(be_reg, dyna_cfg->mg_blc.blc[0]); /* array index 0 */
            isp_la_offset_gr_write(be_reg, dyna_cfg->mg_blc.blc[1]); /* array index 1 */
            isp_la_offset_gb_write(be_reg, dyna_cfg->mg_blc.blc[2]); /* array index 2 */
            isp_la_offset_b_write(be_reg, dyna_cfg->mg_blc.blc[3]); /* array index 3 */
            /* WB */
            isp_wb_ofsr_write(be_reg, dyna_cfg->wb_blc.blc[0]); /* array index 0 */
            isp_wb_ofsgr_write(be_reg, dyna_cfg->wb_blc.blc[1]); /* array index 1 */
            isp_wb_ofsgb_write(be_reg, dyna_cfg->wb_blc.blc[2]); /* array index 2 */
            isp_wb_ofsb_write(be_reg, dyna_cfg->wb_blc.blc[3]); /* array index 3 */

            /* DE */
            isp_bnr_de_blcvalue_write(be_reg, dyna_cfg->de_blc.blc[0]);

            reg_cfg_info->alg_reg_cfg[i].be_blc_cfg.resh_dyna_init = HI_FALSE;
        }

        reg_cfg_info->cfg_key.bit1_be_blc_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_clut_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_bool  usr_resh = HI_FALSE;
    hi_bool  idx_resh = HI_FALSE;
    hi_bool  stt2_lut_regnew = HI_FALSE;
    hi_u8    buf_id;
    hi_u8    blk_num = reg_cfg_info->cfg_num;
    isp_clut_usr_coef_cfg   *clut_usr_coef_cfg = HI_NULL;
    S_ISP_LUT_WSTT_TYPE     *be_lut_stt_reg    = HI_NULL;
    S_VIPROC_REGS_TYPE      *vi_proc_reg       = HI_NULL;
    S_ISPBE_REGS_TYPE       *be_reg            = HI_NULL;
    isp_usr_ctx             *isp_ctx          = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if ((reg_cfg_info->cfg_key.bit1_clut_cfg)) {
        be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);
        isp_check_pointer_return(vi_proc_reg);

        isp_clut_sel_write(be_reg, HI_ISP_CLUT_SEL_WRITE);
        clut_usr_coef_cfg   = &reg_cfg_info->alg_reg_cfg[i].clut_cfg.usr_reg_cfg.clut_usr_coef_cfg;

        idx_resh = (isp_clut_update_index_read(be_reg) != clut_usr_coef_cfg->update_index);
        usr_resh = (offline_mode) ? (clut_usr_coef_cfg->resh && idx_resh) : (clut_usr_coef_cfg->resh);

        if (usr_resh) {
            isp_clut_update_index_write(be_reg, clut_usr_coef_cfg->update_index);

            if (is_online_mode(isp_ctx->block_attr.running_mode) ||
                is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
                /* online lut2stt regconfig */
                buf_id = clut_usr_coef_cfg->buf_id;

                be_lut_stt_reg = (S_ISP_LUT_WSTT_TYPE *)isp_get_be_lut2stt_vir_addr(vi_pipe, i, buf_id);
                isp_check_pointer_return(be_lut_stt_reg);

                isp_clut_lut_wstt_write(be_lut_stt_reg, clut_usr_coef_cfg->lu_stt);

                isp_clut_lut_wstt_addr_write(vi_pipe, i, buf_id, vi_proc_reg);
                isp_clut_stt2lut_en_write(be_reg, HI_TRUE);

                clut_usr_coef_cfg->buf_id = 1 - buf_id;
                stt2_lut_regnew = HI_TRUE;
            } else {
                isp_clut_lut_wstt_write(&be_reg->stIspBeLut.stBeLut2Stt, clut_usr_coef_cfg->lu_stt);
                isp_clut_lut_width_word_write(vi_proc_reg, HI_ISP_CLUT_LUT_WIDTH_WORD_DEFAULT);
                isp_clut_stt2lut_en_write(be_reg, HI_TRUE);
                isp_clut_stt2lut_regnew_write(be_reg, HI_TRUE);
            }

            clut_usr_coef_cfg->resh = offline_mode;
        }

        reg_cfg_info->cfg_key.bit1_clut_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg.clut_stt2lut_regnew = stt2_lut_regnew;

    return HI_SUCCESS;
}

static hi_void isp_expander_lut_reg_config(isp_usr_ctx *isp_ctx, S_ISPBE_REGS_TYPE *be_reg,
                                           isp_expander_static_cfg *static_reg_cfg)
{
    hi_u16   j;

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_expander_lut_waddr_write(be_reg, 0);

        for (j = 0; j < EXPANDER_NODE_NUM; j++) {
            isp_expander_lut_wdata_write(be_reg, static_reg_cfg->lut[j]);
        }
    } else {
        isp_expander_lut_write(be_reg, static_reg_cfg->lut);
    }
}

static hi_s32 isp_expander_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  offline_mode;
    hi_u8    blk_num = reg_cfg_info->cfg_num;

    isp_expander_static_cfg *static_reg_cfg = HI_NULL;
    S_ISPBE_REGS_TYPE       *be_reg         = HI_NULL;
    isp_usr_ctx             *isp_ctx        = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) || \
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_expander_cfg) {
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
        isp_check_pointer_return(be_reg);

        static_reg_cfg = &reg_cfg_info->alg_reg_cfg[i].expander_cfg.static_cfg;

        if (static_reg_cfg->resh) {
            isp_expander_bitw_out_write(be_reg, static_reg_cfg->bit_depth_out);
            isp_expander_bitw_in_write(be_reg, static_reg_cfg->bit_depth_in);

            isp_expander_lut_reg_config(isp_ctx, be_reg, static_reg_cfg);

            static_reg_cfg->resh = HI_FALSE;
        }

        reg_cfg_info->cfg_key.bit1_expander_cfg = offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_fe_update_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u32  i;
    hi_vi_pipe vi_pipe_bind;
    isp_usr_ctx         *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        isp_check_pipe_return(vi_pipe_bind);
        fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
        isp_check_pointer_return(fe_reg);

        isp_fe_update_mode_write(fe_reg, HI_FALSE);
        isp_fe_update_write(fe_reg, HI_TRUE);

        if (reg_cfg_info->alg_reg_cfg[0].fe_lut_update_cfg.ae1_lut_update) {
            isp_ae1_lut_update_write(fe_reg, reg_cfg_info->alg_reg_cfg[0].fe_lut_update_cfg.ae1_lut_update);
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_fe_system_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_bool isp_crop_en = HI_FALSE;
    hi_u8   rggb_cfg;
    hi_u32  i;
    hi_s32  x, y;
    hi_u32  width, height;
    hi_u32  pipe_w, pipe_h;
    hi_vi_pipe vi_pipe_bind, vi_pipe_id;
    isp_usr_ctx *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    vi_pipe_id = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[0];
    rggb_cfg = hi_ext_system_rggb_cfg_read(vi_pipe_id);

    x      = isp_ctx->sys_rect.x;
    y      = isp_ctx->sys_rect.y;
    width  = isp_ctx->sys_rect.width;
    height = isp_ctx->sys_rect.height;
    pipe_w = isp_ctx->pipe_size.width;
    pipe_h = isp_ctx->pipe_size.height;

    /* ISP crop low-power process */
    if ((x == 0) &&
        (y == 0) &&
        (width  == pipe_w) &&
        (height == pipe_h)) {
        isp_crop_en = HI_FALSE;
    } else {
        isp_crop_en = HI_TRUE;
    }

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        isp_check_pipe_return(vi_pipe_bind);
        fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
        isp_check_pointer_return(fe_reg);

        /* ISP FE/BE Set Offline Mode */
        /* isp regs uptate mode:   0: update; 1:frame */
        isp_fe_crop_en_write(fe_reg, isp_crop_en);
        isp_crop_pos_write(fe_reg, x, y);
        isp_crop_size_out_write(fe_reg, width - 1, height - 1);
        isp_fe_rggb_cfg_write(fe_reg, rggb_cfg);
        isp_fe_fix_timing_write(fe_reg, HI_ISP_FE_FIX_TIMING_STAT);
        isp_fe_size_write(fe_reg, pipe_w  - 1, pipe_h - 1);
        isp_fe_blk_size_write(fe_reg, pipe_w  - 1, pipe_h - 1);
        isp_fe_blk_f_hblank_write(fe_reg, 0);
        isp_fe_sync_mode_write(fe_reg, 0, 0);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_reg_default(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    S_ISPBE_REGS_TYPE  *be_reg     = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg = HI_NULL;

    be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);
    isp_check_pointer_return(vi_proc_reg);

    isp_clip_y_min_write(be_reg, ISP_CLIP_Y_MIN_DEFAULT);
    isp_clip_y_max_write(be_reg, ISP_CLIP_Y_MAX_DEFAULT);
    isp_clip_c_min_write(be_reg, ISP_CLIP_C_MIN_DEFAULT);
    isp_clip_c_max_write(be_reg, ISP_CLIP_C_MAX_DEFAULT);
    isp_csc_sum_en_write(be_reg, ISP_CSC_SUM_EN_DEFAULT);
    isp_yuv444_sum_en_write(be_reg, ISP_YUV444_SUM_EN_DEFAULT);
    isp_yuv422_sum_en_write(be_reg, ISP_YUV422_SUM_EN_DEFAULT);
    isp_wdr_sum_en_write(be_reg, ISP_WDR_SUM_EN_DEFAULT);
    isp_demosaic_sum_en_write(be_reg, ISP_DEMOSAIC_SUM_EN_DEFAULT);

    isp_blk_f_hblank_write(be_reg, HI_ISP_BLK_F_HBLANK_DEFAULT);
    isp_blk_f_vblank_write(be_reg, HI_ISP_BLK_F_VBLANK_DEFAULT);
    isp_blk_b_hblank_write(be_reg, HI_ISP_BLK_B_HBLANK_DEFAULT);
    isp_blk_b_vblank_write(be_reg, HI_ISP_BLK_B_VBLANK_DEFAULT);
    /* SDR */
    isp_sqrt_en_write(vi_proc_reg, HI_FALSE); /* sqrt in raw */
    isp_sq_en_write(vi_proc_reg, HI_FALSE); /* sq in rgb */
    isp_sqrt1_en_write(vi_proc_reg, HI_FALSE); /* sqrt in rgb */

    isp_blc_en_write(vi_proc_reg, HI_FALSE);
    isp_split_en_write(vi_proc_reg, HI_FALSE);

    return HI_SUCCESS;
}

static hi_s32 isp_system_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u32  rggb_cfg;
    isp_usr_ctx *isp_ctx = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    rggb_cfg = hi_ext_system_rggb_cfg_read(vi_pipe);

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);
    isp_check_pointer_return(vi_proc_reg);

    isp_be_rggb_cfg_write(vi_proc_reg, rggb_cfg);

    if ((is_offline_mode(isp_ctx->block_attr.running_mode)) ||
        (is_striping_mode(isp_ctx->block_attr.running_mode))) {
        isp_stt_en_write(be_reg, HI_TRUE);
    } else {
        isp_stt_en_write(be_reg, HI_FALSE);
    }

    if (reg_cfg_info->alg_reg_cfg[i].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_calib_corr == 1) {  /* fpn calib mode */
        isp_sumy_en_write(vi_proc_reg, HI_FALSE);
    } else {
        isp_sumy_en_write(vi_proc_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_dither_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    S_ISPBE_REGS_TYPE *be_reg = HI_NULL;

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);
    /* after drc module */
    isp_drc_dither_out_bits_write(be_reg, HI_ISP_DRC_DITHER_OUT_BITS_DEFAULT);
    isp_drc_dither_round_write(be_reg, HI_ISP_DRC_DITHER_ROUND_DEFAULT);
    isp_drc_dither_spatial_mode_write(be_reg, HI_ISP_DRC_DITHER_SPATIAL_MODE_DEFAULT);

    /* after gamma module */
    isp_dmnr_dither_en_write(be_reg, HI_TRUE);
    isp_dmnr_dither_out_bits_write(be_reg, HI_ISP_DMNR_DITHER_OUT_BITS_DEFAULT);
    isp_dmnr_dither_round_write(be_reg, HI_ISP_DMNR_DITHER_ROUND_DEFAULT);
    isp_dmnr_dither_spatial_mode_write(be_reg, HI_ISP_DMNR_DITHER_SPATIAL_MODE_DEFAULT);

    /* after CA module */
    isp_acm_dither_en_write(be_reg, HI_TRUE);
    isp_acm_dither_out_bits_write(be_reg, HI_ISP_ACM_DITHER_OUT_BITS_DEFAULT);
    isp_acm_dither_round_write(be_reg, HI_ISP_ACM_DITHER_ROUND_DEFAULT);
    isp_acm_dither_spatial_mode_write(be_reg, HI_ISP_ACM_DITHER_SPATIAL_MODE_DEFAULT);

    isp_sqrt1_dither_en_write(be_reg, HI_FALSE);

    return HI_SUCCESS;
}

static hi_s32 isp_fe_stt_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u16  num_h, num_v;
    hi_u32  k;
    hi_bool stt_enable = HI_TRUE;
    hi_vi_pipe vi_pipe_bind;
    isp_usr_ctx *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (k = 0; k < isp_ctx->wdr_attr.dev_bind_pipe.num; k++) {
        vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
        isp_check_pipe_return(vi_pipe_bind);
        fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
        isp_check_pointer_return(fe_reg);

        if (isp_ctx->special_opt.fe_stt_update) {
            isp_ae1_stt_en_write(fe_reg, stt_enable);
            isp_ae1_stt_bst_write(fe_reg, 0xF);
        }

        /* ae */
        num_h = reg_cfg_info->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg.fe_weight_table_width;
        num_v = reg_cfg_info->alg_reg_cfg[0].ae_reg_cfg.dyna_reg_cfg.fe_weight_table_height;
        isp_ae1_stt_size_write(fe_reg, (num_h * num_v + 3) / 4); /* plus 3 divide 4 is byte align */
    }

    isp_ctx->special_opt.fe_stt_update = HI_FALSE;

    return HI_SUCCESS;
}

static hi_s32 isp_online_stt_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u16 height;
    hi_u16 num_h, num_v, bin_num;
    hi_bool stt_enable = HI_TRUE;
    isp_usr_ctx       *isp_ctx = HI_NULL;
    S_ISPBE_REGS_TYPE *be_reg  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if ((is_offline_mode(isp_ctx->block_attr.running_mode)) ||
        (is_striping_mode(isp_ctx->block_attr.running_mode))) {
        return HI_SUCCESS;
    }

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);

    if (isp_ctx->special_opt.be_on_stt_update[i]) {
        isp_ae_stt_en_write(be_reg, stt_enable);
        isp_ae_stt_bst_write(be_reg, 0xF);

        isp_la_stt_en_write(be_reg, stt_enable);
        isp_la_stt_bst_write(be_reg, 0xF);

        isp_awb_stt_en_write(be_reg, stt_enable);
        isp_awb_stt_bst_write(be_reg, 0xF);

        isp_af_stt_en_write(be_reg, stt_enable);
        isp_af_stt_bst_write(be_reg, 0xF);

        isp_dehaze_stt_en_write(be_reg, stt_enable);
        isp_dehaze_stt_bst_write(be_reg, 0xF);

        isp_ldci_lpfstt_en_write(be_reg, stt_enable);
        isp_ldci_lpfstt_bst_write(be_reg, 0xF);

        isp_flick_stt_en_write(be_reg, stt_enable);
        isp_flick_stt_bst_write(be_reg, 0xF);

        isp_ctx->special_opt.be_on_stt_update[i] = HI_FALSE;
    }

    /* AE */
    num_h = reg_cfg_info->alg_reg_cfg[i].ae_reg_cfg.dyna_reg_cfg.be_weight_table_width;
    num_v = reg_cfg_info->alg_reg_cfg[i].ae_reg_cfg.dyna_reg_cfg.be_weight_table_height;
    isp_ae_stt_size_write(be_reg, (num_h * num_v + 3) / 4); /* plus 3 divide 4 is byte align */

    /* MG */
    num_h = reg_cfg_info->alg_reg_cfg[i].mg_reg_cfg.dyna_reg_cfg.zone_width;
    num_v = reg_cfg_info->alg_reg_cfg[i].mg_reg_cfg.dyna_reg_cfg.zone_height;
    isp_la_stt_size_write(be_reg, (num_h * num_v + 3) / 4); /* plus 3 divide 4 is byte align */

    /* AWB */
    num_h = reg_cfg_info->alg_reg_cfg[i].awb_reg_cfg.awb_reg_usr_cfg.be_zone_col;
    num_v = reg_cfg_info->alg_reg_cfg[i].awb_reg_cfg.awb_reg_usr_cfg.be_zone_row;
    bin_num = 1;
    isp_awb_stt_size_write(be_reg, (num_h * num_v * bin_num * 2  + 3) / 4); /* 2 plus 3 divide 4 is byte align */

    /* AF */
    num_h = reg_cfg_info->alg_reg_cfg[i].be_af_reg_cfg.window_hnum;
    num_v = reg_cfg_info->alg_reg_cfg[i].be_af_reg_cfg.window_vnum;
    isp_af_stt_size_write(be_reg, (num_h * num_v * 4 + 3) / 4); /* plus 3 divide 4 is byte align */

    /* dehaze */
    num_h = reg_cfg_info->alg_reg_cfg[i].dehaze_reg_cfg.static_reg_cfg.dchnum;
    num_v = reg_cfg_info->alg_reg_cfg[i].dehaze_reg_cfg.static_reg_cfg.dcvnum;
    isp_dehaze_stt_size_write(be_reg, (((num_h + 1) * (num_v + 1)) + 3) / 4); /* plus 3 divide 4 is byte align */

    /* Ldci */
    num_h = reg_cfg_info->alg_reg_cfg[i].ldci_reg_cfg.dyna_reg_cfg.stat_sml_map_width;
    num_v = reg_cfg_info->alg_reg_cfg[i].ldci_reg_cfg.dyna_reg_cfg.stat_sml_map_height;
    isp_ldci_lpfstt_size_write(be_reg, (num_h * num_v + 3) / 4);  /* plus 3 divide 4 is byte align */

    /* Flicker */
    height = isp_ctx->block_attr.frame_rect.height;
    isp_flick_stt_size_write(be_reg, (height / 16 + 3) / 4);  /* height divede 16 plus 3 divide 4 is byte align */

    return HI_SUCCESS;
}

static hi_s32 isp_be_alg_lut2stt_reg_new_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    isp_be_stt2lut_regnew_reg_cfg  *be_stt2lut_regnew_cfg = &reg_cfg_info->alg_reg_cfg[i].stt2lut_regnew_cfg;
    S_ISPBE_REGS_TYPE                *be_reg              = HI_NULL;
    isp_usr_ctx                        *isp_ctx             = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);

    if (be_stt2lut_regnew_cfg->gamma_stt2lut_regnew) {
        isp_gamma_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    if ((isp_ctx->frame_cnt < STT_LUT_CONFIG_TIMES) || be_stt2lut_regnew_cfg->lsc_stt2lut_regnew) {
        isp_lsc_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    if ((isp_ctx->frame_cnt < STT_LUT_CONFIG_TIMES) || be_stt2lut_regnew_cfg->bnr_lsc_stt2lut_regnew) {
        isp_bnr_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    if ((isp_ctx->frame_cnt < STT_LUT_CONFIG_TIMES) || be_stt2lut_regnew_cfg->ca_stt2lut_regnew) {
        isp_ca_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    if ((isp_ctx->frame_cnt < STT_LUT_CONFIG_TIMES) || be_stt2lut_regnew_cfg->clut_stt2lut_regnew) {
        isp_clut_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    if ((isp_ctx->frame_cnt < STT_LUT_CONFIG_TIMES) || be_stt2lut_regnew_cfg->dpc_stt2lut_regnew) {
        isp_dpc_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    if ((isp_ctx->frame_cnt < STT_LUT_CONFIG_TIMES) || be_stt2lut_regnew_cfg->pre_gamma_stt2lut_regnew) {
        isp_pregamma_stt2lut_regnew_write(be_reg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_alg_lut_update_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    isp_be_lut_update_reg_cfg *be_lut_update_cfg = &reg_cfg_info->alg_reg_cfg[i].be_lut_update_cfg;
    S_ISPBE_REGS_TYPE *be_reg = HI_NULL;

    be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);

    if (be_lut_update_cfg->ae_lut_update) {
        isp_ae_lut_update_write(be_reg, be_lut_update_cfg->ae_lut_update);
    }

    if (be_lut_update_cfg->sharpen_lut_update) {
        isp_sharpen_lut_update_write(be_reg, be_lut_update_cfg->sharpen_lut_update);
    }

    if (be_lut_update_cfg->drc_tm_lut_update) {
        isp_drc_lut_update_write(be_reg, be_lut_update_cfg->drc_tm_lut_update);
    }

    if (be_lut_update_cfg->lcac_lut_update) {
        isp_demosaic_depurplut_update_write(be_reg, be_lut_update_cfg->lcac_lut_update);
    }

    if (be_lut_update_cfg->nddm_gf_lut_update) {
        isp_nddm_gf_lut_update_write(be_reg, be_lut_update_cfg->nddm_gf_lut_update);
    }
    if (be_lut_update_cfg->ldci_drc_lut_update) {
        isp_ldci_drc_lut_update_write(be_reg, be_lut_update_cfg->ldci_drc_lut_update);
    }
    if (be_lut_update_cfg->ldci_calc_lut_update) {
        isp_ldci_calc_lut_update_write(be_reg, be_lut_update_cfg->ldci_calc_lut_update);
    }

    if (be_lut_update_cfg->bnr_lut_update) {
        isp_bnr_lut_update_write(be_reg, be_lut_update_cfg->bnr_lut_update);
    }
    if (be_lut_update_cfg->de_lut_update) {
        isp_de_lut_update_write(be_reg, be_lut_update_cfg->de_lut_update);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_cur_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    isp_alg_reg_cfg  *alg_reg_cfg = &reg_cfg_info->alg_reg_cfg[i];
    S_ISPBE_REGS_TYPE  *be_reg     = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg = HI_NULL;

    be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);
    isp_check_pointer_return(vi_proc_reg);

    isp_ae_en_write(vi_proc_reg, alg_reg_cfg->ae_reg_cfg.static_reg_cfg.be_enable);
    isp_la_en_write(vi_proc_reg, alg_reg_cfg->mg_reg_cfg.static_reg_cfg.enable);
    isp_wb_en_write(vi_proc_reg, alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_wb_work_en);
    isp_cc_en_write(vi_proc_reg, alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_cc_en);
    isp_cc_colortone_en_write(be_reg, alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_cc_colortone_en);
    isp_awb_en_write(vi_proc_reg, alg_reg_cfg->awb_reg_cfg.awb_reg_sta_cfg.be_awb_work_en);
    isp_af_en_write(vi_proc_reg, alg_reg_cfg->be_af_reg_cfg.af_enable);
    isp_sharpen_en_write(vi_proc_reg, alg_reg_cfg->sharpen_reg_cfg.enable);
    isp_sharpen_benmarkedge_write(be_reg, alg_reg_cfg->edge_mark_reg_cfg.enable);
    isp_hlc_en_write(vi_proc_reg, alg_reg_cfg->hlc_reg_cfg.enable);
    isp_dmnr_vhdm_en_write(vi_proc_reg, alg_reg_cfg->dem_reg_cfg.vhdm_enable);
    isp_dmnr_nddm_en_write(vi_proc_reg, alg_reg_cfg->dem_reg_cfg.nddm_enable);
    isp_demosaic_local_cac_en_write(be_reg, alg_reg_cfg->lcac_reg_cfg.local_cac_en);
    isp_gcac_en_write(be_reg, alg_reg_cfg->gcac_reg_cfg.global_cac_en);
    isp_demosaic_fcr_en_write(be_reg, alg_reg_cfg->anti_false_color_reg_cfg.fcr_enable);
    isp_dpc_en_write(vi_proc_reg, alg_reg_cfg->dp_reg_cfg.dpc_en[0]); /* array index 0 */
    isp_dpc_dpc_en1_write(be_reg, alg_reg_cfg->dp_reg_cfg.dpc_en[1]); /* array index 1 */
    isp_ge_en_write(vi_proc_reg, alg_reg_cfg->ge_reg_cfg.ge_en[0]); /* array index 0 */
    isp_ge_ge1_en_write(be_reg, alg_reg_cfg->ge_reg_cfg.ge_en[1]); /* array index 1 */
    isp_lsc_en_write(vi_proc_reg, alg_reg_cfg->lsc_reg_cfg.lsc_en);
    isp_gamma_en_write(vi_proc_reg, alg_reg_cfg->gamma_cfg.gamma_en);
    isp_csc_en_write(vi_proc_reg, alg_reg_cfg->csc_cfg.enable);
    isp_ca_en_write(vi_proc_reg, alg_reg_cfg->ca_reg_cfg.ca_en);
    isp_ca_lumath_high_write(be_reg, alg_reg_cfg->ca_reg_cfg.usr_reg_cfg.ca_luma_thr_high);
    isp_ca_lumaratio_high_write(be_reg, alg_reg_cfg->ca_reg_cfg.usr_reg_cfg.ca_luma_ratio_high);
    isp_ca_isoratio_write(be_reg, alg_reg_cfg->ca_reg_cfg.dyna_reg_cfg.ca_iso_ratio);

    isp_dehaze_en_write(vi_proc_reg, alg_reg_cfg->dehaze_reg_cfg.dehaze_en);
    if (alg_reg_cfg->lsc_reg_cfg.lsc_en) {
        isp_bnr_rlsc_en_write(be_reg, alg_reg_cfg->bnr_reg_cfg.usr_reg_cfg.bnr_lsc_en);
    } else {
        isp_bnr_rlsc_en_write(be_reg, 0);
    }
    isp_pregamma_en_write(vi_proc_reg, alg_reg_cfg->pregamma_reg_cfg.pregamma_en);
    isp_clut_en_write(vi_proc_reg, alg_reg_cfg->clut_cfg.enable);
    isp_expander_en_write(vi_proc_reg, alg_reg_cfg->expander_cfg.enable);

    isp_ldci_blc_ctrl_write(be_reg, alg_reg_cfg->ldci_reg_cfg.dyna_reg_cfg.calc_blc_ctrl);

    if (alg_reg_cfg->lsc_reg_cfg.usr_reg_cfg.lut_update) {
        isp_lsc_mesh_scale_write(be_reg, alg_reg_cfg->lsc_reg_cfg.usr_reg_cfg.mesh_scale);
    }
    if (alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.resh) {
        isp_clut_gain0_write(be_reg, alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.gain_r);
        isp_clut_gain1_write(be_reg, alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.gain_g);
        isp_clut_gain2_write(be_reg, alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.gain_b);
    }

    isp_dehaze_air_r_write(be_reg, alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.air_r);
    isp_dehaze_air_g_write(be_reg, alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.air_g);
    isp_dehaze_air_b_write(be_reg, alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.air_b);
    isp_dehaze_gstrth_write(be_reg, alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.strength);

    isp_sharpen_dyna_reg_config(be_reg, &alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg,
                                &alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.default_dyna_reg_cfg);

    return HI_SUCCESS;
}

static hi_s32 isp_be_last_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    isp_lut2stt_sync_reg_cfg *lut2stt_sync_cfg = &reg_cfg_info->alg_reg_cfg[i].lut2stt_sync_cfg[0];
    S_ISPBE_REGS_TYPE   *be_reg      = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg = HI_NULL;

    be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
    isp_check_pointer_return(be_reg);
    isp_check_pointer_return(vi_proc_reg);

    isp_ae_en_write(vi_proc_reg, lut2stt_sync_cfg->ae_en);
    isp_la_en_write(vi_proc_reg, lut2stt_sync_cfg->la_en);
    isp_wb_en_write(vi_proc_reg, lut2stt_sync_cfg->wb_en);
    isp_cc_en_write(vi_proc_reg, lut2stt_sync_cfg->ccm_en);
    isp_cc_colortone_en_write(be_reg, lut2stt_sync_cfg->ccm_color_tone_en);
    isp_af_en_write(vi_proc_reg, lut2stt_sync_cfg->af_en);
    isp_sharpen_en_write(vi_proc_reg, lut2stt_sync_cfg->sharpen_en);
    isp_sharpen_benmarkedge_write(be_reg, lut2stt_sync_cfg->edge_mark_en);
    isp_hlc_en_write(vi_proc_reg, lut2stt_sync_cfg->hlc_en);
    isp_dmnr_vhdm_en_write(vi_proc_reg, lut2stt_sync_cfg->vhdm_en);
    isp_dmnr_nddm_en_write(vi_proc_reg, lut2stt_sync_cfg->nddm_en);
    isp_demosaic_local_cac_en_write(be_reg, lut2stt_sync_cfg->lcac_en);
    isp_gcac_en_write(be_reg, lut2stt_sync_cfg->gcac_en);
    isp_demosaic_fcr_en_write(be_reg, lut2stt_sync_cfg->fcr_en);
    isp_dpc_en_write(vi_proc_reg, lut2stt_sync_cfg->dpc_en[0]); /* array index 0 */
    isp_dpc_dpc_en1_write(be_reg, lut2stt_sync_cfg->dpc_en[1]); /* array index 1 */
    isp_ge_en_write(vi_proc_reg, lut2stt_sync_cfg->ge_en[0]); /* array index 0 */
    isp_ge_ge1_en_write(be_reg, lut2stt_sync_cfg->ge_en[1]); /* array index 1 */
    isp_lsc_en_write(vi_proc_reg, lut2stt_sync_cfg->lsc_en);
    isp_gamma_en_write(vi_proc_reg, lut2stt_sync_cfg->gamma_en);
    isp_csc_en_write(vi_proc_reg, lut2stt_sync_cfg->csc_en);
    isp_ca_en_write(vi_proc_reg, lut2stt_sync_cfg->ca_en);
    isp_ca_lumath_high_write(be_reg, lut2stt_sync_cfg->ca_sync_cfg.ca_luma_thr_high);
    isp_ca_lumaratio_high_write(be_reg, lut2stt_sync_cfg->ca_sync_cfg.ca_luma_ratio_high);
    isp_ca_isoratio_write(be_reg, lut2stt_sync_cfg->ca_sync_cfg.ca_iso_ratio);

    isp_dehaze_en_write(vi_proc_reg, lut2stt_sync_cfg->dehaze_en);
    if (lut2stt_sync_cfg->lsc_en) {
        isp_bnr_rlsc_en_write(be_reg, lut2stt_sync_cfg->bnr_lsc_en);
    } else {
        isp_bnr_rlsc_en_write(be_reg, 0);
    }

    isp_pregamma_en_write(vi_proc_reg, lut2stt_sync_cfg->pregamma_en);
    isp_clut_en_write(vi_proc_reg, lut2stt_sync_cfg->clut_en);
    isp_expander_en_write(vi_proc_reg, lut2stt_sync_cfg->expander_en);

    isp_ldci_blc_ctrl_write(be_reg, lut2stt_sync_cfg->ldci_sync_cfg.calc_blc_ctrl);

    if (lut2stt_sync_cfg->lsc_sync_cfg.resh) {
        isp_lsc_mesh_scale_write(be_reg, lut2stt_sync_cfg->lsc_sync_cfg.mesh_scale);
    }
    if (lut2stt_sync_cfg->clut_sync_cfg.resh) {
        isp_clut_gain0_write(be_reg, lut2stt_sync_cfg->clut_sync_cfg.gain_r);
        isp_clut_gain1_write(be_reg, lut2stt_sync_cfg->clut_sync_cfg.gain_g);
        isp_clut_gain2_write(be_reg, lut2stt_sync_cfg->clut_sync_cfg.gain_b);
    }

    if (reg_cfg_info->alg_reg_cfg[i].dehaze_reg_cfg.lut2_stt_en == HI_TRUE) {
        isp_dehaze_air_r_write(be_reg, lut2stt_sync_cfg->dehaze_sync_cfg.air_r);
        isp_dehaze_air_g_write(be_reg, lut2stt_sync_cfg->dehaze_sync_cfg.air_g);
        isp_dehaze_air_b_write(be_reg, lut2stt_sync_cfg->dehaze_sync_cfg.air_b);
        isp_dehaze_gstrth_write(be_reg, lut2stt_sync_cfg->dehaze_sync_cfg.strength);
    }

    isp_sharpen_dyna_reg_config(be_reg, &lut2stt_sync_cfg->sharpen_sync_cfg.mpi_dyna_reg_cfg,
                                &lut2stt_sync_cfg->sharpen_sync_cfg.default_dyna_reg_cfg);

    return HI_SUCCESS;
}

static hi_s32 isp_be_alg_sync_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    isp_usr_ctx *isp_ctx   = HI_NULL;
    hi_s32 ret = HI_SUCCESS;
    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) || \
        is_striping_mode(isp_ctx->block_attr.running_mode)) {
        ret = isp_be_cur_reg_config(vi_pipe, reg_cfg_info, i);
        if (ret != HI_SUCCESS) {
            isp_err_trace("Pipe:%d isp_be_cur_reg_config failed!\n", vi_pipe);
        }
    } else {
        ret = isp_be_last_reg_config(vi_pipe, reg_cfg_info, i);
        if (ret != HI_SUCCESS) {
            isp_err_trace("Pipe:%d isp_be_last_reg_config failed!\n", vi_pipe);
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_save_be_sync_reg(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_u8 j;
    isp_usr_ctx *isp_ctx   = HI_NULL;
    isp_lut2stt_sync_reg_cfg *lut2stt_sync_cfg = &reg_cfg_info->alg_reg_cfg[i].lut2stt_sync_cfg[0];
    isp_alg_reg_cfg          *alg_reg_cfg      = &reg_cfg_info->alg_reg_cfg[i];

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) || \
        is_striping_mode(isp_ctx->block_attr.running_mode)) {
        return HI_SUCCESS;
    }

    lut2stt_sync_cfg->ae_en           = alg_reg_cfg->ae_reg_cfg.static_reg_cfg.be_enable;
    lut2stt_sync_cfg->la_en           = alg_reg_cfg->mg_reg_cfg.static_reg_cfg.enable;
    lut2stt_sync_cfg->awb_en          = alg_reg_cfg->awb_reg_cfg.awb_reg_sta_cfg.be_awb_work_en;
    lut2stt_sync_cfg->wb_en           = alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_wb_work_en;
    lut2stt_sync_cfg->ccm_en          = alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_cc_en;
    lut2stt_sync_cfg->ccm_color_tone_en = alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_cc_colortone_en;
    lut2stt_sync_cfg->af_en           = alg_reg_cfg->be_af_reg_cfg.af_enable;
    lut2stt_sync_cfg->sharpen_en      = alg_reg_cfg->sharpen_reg_cfg.enable;
    lut2stt_sync_cfg->edge_mark_en     = alg_reg_cfg->edge_mark_reg_cfg.enable;
    lut2stt_sync_cfg->hlc_en          = alg_reg_cfg->hlc_reg_cfg.enable;
    lut2stt_sync_cfg->vhdm_en         = alg_reg_cfg->dem_reg_cfg.vhdm_enable;
    lut2stt_sync_cfg->nddm_en         = alg_reg_cfg->dem_reg_cfg.nddm_enable;
    lut2stt_sync_cfg->ldci_en         = alg_reg_cfg->ldci_reg_cfg.dyna_reg_cfg.enable;
    lut2stt_sync_cfg->lcac_en         = alg_reg_cfg->lcac_reg_cfg.local_cac_en;
    lut2stt_sync_cfg->gcac_en         = alg_reg_cfg->gcac_reg_cfg.global_cac_en;
    lut2stt_sync_cfg->fcr_en          = alg_reg_cfg->anti_false_color_reg_cfg.fcr_enable;
    lut2stt_sync_cfg->lsc_en          = alg_reg_cfg->lsc_reg_cfg.lsc_en;
    lut2stt_sync_cfg->gamma_en        = alg_reg_cfg->gamma_cfg.gamma_en;
    lut2stt_sync_cfg->csc_en          = alg_reg_cfg->csc_cfg.enable;
    lut2stt_sync_cfg->ca_en           = alg_reg_cfg->ca_reg_cfg.ca_en;
    lut2stt_sync_cfg->ca_sync_cfg.ca_luma_thr_high    = alg_reg_cfg->ca_reg_cfg.usr_reg_cfg.ca_luma_thr_high;
    lut2stt_sync_cfg->ca_sync_cfg.ca_luma_ratio_high  = alg_reg_cfg->ca_reg_cfg.usr_reg_cfg.ca_luma_ratio_high;
    lut2stt_sync_cfg->ca_sync_cfg.ca_iso_ratio            = alg_reg_cfg->ca_reg_cfg.dyna_reg_cfg.ca_iso_ratio;
    lut2stt_sync_cfg->wdr_en          = alg_reg_cfg->wdr_reg_cfg.wdr_en;
    lut2stt_sync_cfg->drc_en          = alg_reg_cfg->drc_reg_cfg.drc_en;
    lut2stt_sync_cfg->dehaze_en       = alg_reg_cfg->dehaze_reg_cfg.dehaze_en;
    lut2stt_sync_cfg->bnr_lsc_en      = alg_reg_cfg->bnr_reg_cfg.usr_reg_cfg.bnr_lsc_en;
    lut2stt_sync_cfg->dg_en           = alg_reg_cfg->dg_reg_cfg.dg_en;
    lut2stt_sync_cfg->four_dg_en          = alg_reg_cfg->four_dg_reg_cfg.enable;
    lut2stt_sync_cfg->pregamma_en     = alg_reg_cfg->pregamma_reg_cfg.pregamma_en;
    lut2stt_sync_cfg->clut_en         = alg_reg_cfg->clut_cfg.enable;
    lut2stt_sync_cfg->expander_en     = alg_reg_cfg->expander_cfg.enable;

    for (j = 0; j < 4; j++) { /*  dpc ge channel number is 4 */
        lut2stt_sync_cfg->dpc_en[j] = alg_reg_cfg->dp_reg_cfg.dpc_en[j];
        lut2stt_sync_cfg->ge_en[j]  = alg_reg_cfg->ge_reg_cfg.ge_en[j];
    }

    lut2stt_sync_cfg->lsc_sync_cfg.resh       = alg_reg_cfg->be_lut_update_cfg.lsc_lut_update;
    lut2stt_sync_cfg->lsc_sync_cfg.mesh_scale = alg_reg_cfg->lsc_reg_cfg.usr_reg_cfg.mesh_scale;
    lut2stt_sync_cfg->clut_sync_cfg.resh      = alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.resh;
    lut2stt_sync_cfg->clut_sync_cfg.gain_r    = alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.gain_r;
    lut2stt_sync_cfg->clut_sync_cfg.gain_g    = alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.gain_g;
    lut2stt_sync_cfg->clut_sync_cfg.gain_b    = alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.gain_b;

    lut2stt_sync_cfg->dehaze_sync_cfg.air_r   = alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.air_r;
    lut2stt_sync_cfg->dehaze_sync_cfg.air_g   = alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.air_g;
    lut2stt_sync_cfg->dehaze_sync_cfg.air_b   = alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.air_b;
    lut2stt_sync_cfg->dehaze_sync_cfg.strength = alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.strength;

    lut2stt_sync_cfg->ldci_sync_cfg.calc_blc_ctrl  = alg_reg_cfg->ldci_reg_cfg.dyna_reg_cfg.calc_blc_ctrl;

    memcpy_s(&lut2stt_sync_cfg->sharpen_sync_cfg.mpi_dyna_reg_cfg, sizeof(isp_sharpen_mpi_dyna_reg_cfg),
             &alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg, sizeof(isp_sharpen_mpi_dyna_reg_cfg));
    memcpy_s(&lut2stt_sync_cfg->sharpen_sync_cfg.default_dyna_reg_cfg, sizeof(isp_sharpen_default_dyna_reg_cfg),
             &alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.default_dyna_reg_cfg, sizeof(isp_sharpen_default_dyna_reg_cfg));

    return HI_SUCCESS;
}

hi_s32 isp_be_resh_cfg(isp_alg_reg_cfg *alg_reg_cfg)
{
    alg_reg_cfg->awb_reg_cfg.awb_reg_sta_cfg.be_awb_sta_cfg = HI_TRUE;
    alg_reg_cfg->awb_reg_cfg.awb_reg_usr_cfg.resh           = HI_TRUE;

    alg_reg_cfg->dem_reg_cfg.static_reg_cfg.resh            = HI_TRUE;
    alg_reg_cfg->dem_reg_cfg.dyna_reg_cfg.resh              = HI_TRUE;

    alg_reg_cfg->ldci_reg_cfg.static_reg_cfg.static_resh    = HI_TRUE;

    alg_reg_cfg->lcac_reg_cfg.static_reg_cfg.static_resh    = HI_TRUE;
    alg_reg_cfg->lcac_reg_cfg.usr_reg_cfg.resh              = HI_TRUE;
    alg_reg_cfg->lcac_reg_cfg.dyna_reg_cfg.resh             = HI_TRUE;
    alg_reg_cfg->gcac_reg_cfg.static_reg_cfg.static_resh    = HI_TRUE;
    alg_reg_cfg->gcac_reg_cfg.usr_reg_cfg.resh              = HI_TRUE;

    alg_reg_cfg->anti_false_color_reg_cfg.static_reg_cfg.resh = HI_TRUE;
    alg_reg_cfg->anti_false_color_reg_cfg.dyna_reg_cfg.resh   = HI_TRUE;

    alg_reg_cfg->dp_reg_cfg.static_reg_cfg.static_resh            = HI_TRUE;
    alg_reg_cfg->dp_reg_cfg.usr_reg_cfg.usr_dyna_cor_reg_cfg.resh = HI_TRUE;
    alg_reg_cfg->dp_reg_cfg.usr_reg_cfg.usr_sta_cor_reg_cfg.resh  = HI_TRUE;
    alg_reg_cfg->dp_reg_cfg.dyna_reg_cfg.resh                     = HI_TRUE;

    alg_reg_cfg->ge_reg_cfg.static_reg_cfg.static_resh      = HI_TRUE;
    alg_reg_cfg->ge_reg_cfg.usr_reg_cfg.resh                = HI_TRUE;
    alg_reg_cfg->ge_reg_cfg.dyna_reg_cfg.resh               = HI_TRUE;

    alg_reg_cfg->lsc_reg_cfg.static_reg_cfg.static_resh      = HI_TRUE;
    alg_reg_cfg->lsc_reg_cfg.usr_reg_cfg.coef_update         = HI_TRUE;
    alg_reg_cfg->lsc_reg_cfg.usr_reg_cfg.lut_update          = HI_TRUE;

    alg_reg_cfg->rlsc_reg_cfg.static_reg_cfg.static_resh     = HI_TRUE;
    alg_reg_cfg->rlsc_reg_cfg.usr_reg_cfg.coef_update        = HI_TRUE;
    alg_reg_cfg->rlsc_reg_cfg.usr_reg_cfg.lut_update         = HI_TRUE;

    alg_reg_cfg->gamma_cfg.usr_reg_cfg.gamma_lut_update_en   = HI_TRUE;
    alg_reg_cfg->csc_cfg.dyna_reg_cfg.resh                   = HI_TRUE;

    alg_reg_cfg->ca_reg_cfg.static_reg_cfg.static_resh       = HI_TRUE;
    alg_reg_cfg->ca_reg_cfg.dyna_reg_cfg.resh                = HI_TRUE;
    alg_reg_cfg->ca_reg_cfg.usr_reg_cfg.resh                 = HI_TRUE;
    alg_reg_cfg->ca_reg_cfg.usr_reg_cfg.ca_lut_update_en     = HI_TRUE;

    alg_reg_cfg->mcds_reg_cfg.static_reg_cfg.static_resh     = HI_TRUE;
    alg_reg_cfg->mcds_reg_cfg.dyna_reg_cfg.dyna_resh         = HI_TRUE;

    alg_reg_cfg->wdr_reg_cfg.static_reg_cfg.resh             = HI_TRUE;
    alg_reg_cfg->wdr_reg_cfg.usr_reg_cfg.resh                = HI_TRUE;
    alg_reg_cfg->wdr_reg_cfg.dyna_reg_cfg.resh               = HI_TRUE;

    alg_reg_cfg->drc_reg_cfg.static_reg_cfg.static_resh      = HI_TRUE;
    alg_reg_cfg->drc_reg_cfg.usr_reg_cfg.usr_resh            = HI_TRUE;
    alg_reg_cfg->drc_reg_cfg.dyna_reg_cfg.dyna_resh          = HI_TRUE;

    alg_reg_cfg->dehaze_reg_cfg.static_reg_cfg.resh         = HI_TRUE;
    alg_reg_cfg->dehaze_reg_cfg.dyna_reg_cfg.lut_update     = 1;

    alg_reg_cfg->bnr_reg_cfg.static_reg_cfg.resh            = HI_TRUE;
    alg_reg_cfg->bnr_reg_cfg.dyna_reg_cfg.resh              = HI_TRUE;
    alg_reg_cfg->bnr_reg_cfg.usr_reg_cfg.resh               = HI_TRUE;

    alg_reg_cfg->four_dg_reg_cfg.static_reg_cfg.resh        = HI_TRUE;
    alg_reg_cfg->four_dg_reg_cfg.dyna_reg_cfg.resh          = HI_TRUE;
    alg_reg_cfg->dg_reg_cfg.static_reg_cfg.resh             = HI_TRUE;
    alg_reg_cfg->dg_reg_cfg.dyna_reg_cfg.resh               = HI_TRUE;

    alg_reg_cfg->pregamma_reg_cfg.static_reg_cfg.static_resh          = HI_TRUE;
    alg_reg_cfg->pregamma_reg_cfg.dyna_reg_cfg.pregamma_lut_update_en = HI_TRUE;
    alg_reg_cfg->flicker_reg_cfg.dyna_reg_cfg.resh                    = HI_TRUE;

    alg_reg_cfg->be_blc_cfg.static_blc.resh_static = HI_TRUE;
    alg_reg_cfg->be_blc_cfg.resh_dyna_init         = HI_TRUE;

    alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg.resh  = HI_TRUE;
    alg_reg_cfg->clut_cfg.usr_reg_cfg.clut_usr_coef_cfg.resh  = HI_TRUE;

    alg_reg_cfg->expander_cfg.static_cfg.resh                 = HI_TRUE;

    alg_reg_cfg->sharpen_reg_cfg.static_reg_cfg.static_resh             = HI_TRUE;
    alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.default_dyna_reg_cfg.resh = HI_TRUE;
    alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.resh     = HI_TRUE;

    alg_reg_cfg->de_reg_cfg.static_reg_cfg.resh = HI_TRUE;
    alg_reg_cfg->de_reg_cfg.dyna_reg_cfg.resh   = HI_TRUE;
    alg_reg_cfg->de_reg_cfg.usr_reg_cfg.resh    = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_reset_fe_stt_en(hi_vi_pipe vi_pipe)
{
    hi_u8   i;
    hi_vi_pipe vi_pipe_bind;
    isp_usr_ctx       *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            isp_check_pipe_return(vi_pipe_bind);

            fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
            isp_check_pointer_return(fe_reg);

            isp_ae1_stt_en_write(fe_reg, HI_FALSE);
            isp_fe_update_write(fe_reg,  HI_TRUE);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_alg_en_exit(hi_vi_pipe vi_pipe)
{
    hi_u8   i;
    hi_vi_pipe vi_pipe_bind;
    isp_usr_ctx           *isp_ctx    = HI_NULL;
    S_ISPFE_REGS_TYPE   *fe_reg     = HI_NULL;
    S_ISPBE_REGS_TYPE   *be_reg     = HI_NULL;
    S_VIPROC_REGS_TYPE  *vi_proc_reg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);

    if (isp_ctx->para_rec.init == HI_FALSE) {
        return HI_SUCCESS;
    }

    /* FE */
    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            isp_check_pipe_return(vi_pipe_bind);

            fe_reg = (S_ISPFE_REGS_TYPE *)isp_get_fe_vir_addr(vi_pipe_bind);
            isp_check_pointer_return(fe_reg);

            isp_fe_ae1_en_write(fe_reg,  HI_FALSE);
            isp_fe_wb1_en_write(fe_reg,  HI_FALSE);
            isp_fe_dg2_en_write(fe_reg,  HI_FALSE);
            isp_ae1_stt_en_write(fe_reg, HI_FALSE);
            isp_fe_update_write(fe_reg,  HI_TRUE);
        }
    }

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
            be_reg     = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);
            vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);

            isp_check_pointer_return(be_reg);
            isp_check_pointer_return(vi_proc_reg);

            isp_ae_en_write(vi_proc_reg, HI_FALSE);
            isp_la_en_write(vi_proc_reg, HI_FALSE);
            isp_awb_en_write(vi_proc_reg, HI_FALSE);
            isp_af_en_write(vi_proc_reg, HI_FALSE);
            isp_dehaze_en_write(vi_proc_reg, HI_FALSE);
            isp_ldci_en_write(vi_proc_reg, HI_FALSE);
            isp_flicker_en_write(vi_proc_reg, HI_FALSE);

            isp_ldci_wrstat_en_write(be_reg, HI_FALSE);
            isp_ldci_rdstat_en_write(be_reg, HI_FALSE);
            isp_sharpen_stt2lut_en_write(be_reg, HI_FALSE);
            isp_ldci_stt2lut_en_write(be_reg, HI_FALSE);
            isp_dehaze_stt2lut_en_write(be_reg, HI_FALSE);
            isp_gamma_stt2lut_en_write(be_reg, HI_FALSE);
            isp_lsc_stt2lut_en_write(be_reg, HI_FALSE);
            isp_bnr_stt2lut_en_write(be_reg, HI_FALSE);
            isp_ca_stt2lut_en_write(be_reg, HI_FALSE);
            isp_clut_stt2lut_en_write(be_reg, HI_FALSE);
            isp_dpc_stt2lut_en_write(be_reg, HI_FALSE);
            isp_pregamma_stt2lut_en_write(be_reg, HI_FALSE);

            isp_be_manual_reg_up_write(vi_proc_reg, HI_TRUE);
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_fe_regs_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_s64 ret = HI_SUCCESS;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_vir_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe) {
        /* FE alg cfgs setting to register */
        ret += isp_fe_ae_reg_config(vi_pipe, reg_cfg_info); /* Ae */
        ret += isp_fe_awb_reg_config(vi_pipe, reg_cfg_info); /* awb */
        ret += isp_fe_dg_reg_config(vi_pipe, reg_cfg_info); /* DG */
        ret += isp_fe_blc_reg_config(vi_pipe, reg_cfg_info); /* fe blc */

        ret += isp_fe_system_reg_config(vi_pipe, reg_cfg_info);
        ret += isp_fe_stt_reg_config(vi_pipe, reg_cfg_info);
        ret += isp_fe_update_reg_config(vi_pipe, reg_cfg_info);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] isp_fe_regs_config failed!\n", vi_pipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_sync_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u8  j;
    hi_s32 ret;
    isp_be_sync_para be_sync_param = {0};
    isp_alg_reg_cfg  *alg_reg_cfg = &reg_cfg_info->alg_reg_cfg[0];
    isp_usr_ctx      *isp_ctx    = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        return HI_SUCCESS;
    }

    /* DG */
    be_sync_param.isp_dgain[0] = alg_reg_cfg->dg_reg_cfg.dyna_reg_cfg.gain_r;  /* array 0 assignment */
    be_sync_param.isp_dgain[1] = alg_reg_cfg->dg_reg_cfg.dyna_reg_cfg.gain_gr; /* array 1 assignment */
    be_sync_param.isp_dgain[2] = alg_reg_cfg->dg_reg_cfg.dyna_reg_cfg.gain_gb; /* array 2 assignment */
    be_sync_param.isp_dgain[3] = alg_reg_cfg->dg_reg_cfg.dyna_reg_cfg.gain_b; /* array 3 assignment */

    /* LDCI */
    be_sync_param.ldci_comp = 0x1000;

    /* DRC */
    be_sync_param.drc_shp_log       = alg_reg_cfg->drc_reg_cfg.static_reg_cfg.shp_log;
    be_sync_param.drc_div_denom_log = alg_reg_cfg->drc_reg_cfg.static_reg_cfg.div_denom_log;
    be_sync_param.drc_denom_exp     = alg_reg_cfg->drc_reg_cfg.static_reg_cfg.denom_exp;

    for (j = 0; j < SYNC_DRC_PRELUMA_NUM; j++) {
        be_sync_param.drc_prev_luma[j] = alg_reg_cfg->drc_reg_cfg.static_reg_cfg.prev_luma[j];
    }

    /* WDR */
    be_sync_param.wdr_exp_ratio   = alg_reg_cfg->wdr_reg_cfg.static_reg_cfg.expo_r_ratio_lut[0];
    be_sync_param.flick_exp_ratio = alg_reg_cfg->wdr_reg_cfg.static_reg_cfg.flick_exp_ratio;

    for (j = 0; j < SYNC_WDR_EXP_VAL_NUM; j++) {
        be_sync_param.wdr_exp_val[j] = alg_reg_cfg->wdr_reg_cfg.static_reg_cfg.expo_value_lut[j];
        be_sync_param.wdr_gain[j]    = 0x100;
    }

    be_sync_param.wdr_blc_comp  = alg_reg_cfg->wdr_reg_cfg.static_reg_cfg.blc_comp_lut[0];
    be_sync_param.wdr_mdt_en    = alg_reg_cfg->wdr_reg_cfg.dyna_reg_cfg.wdr_mdt_en;
    be_sync_param.fusion_mode   = alg_reg_cfg->wdr_reg_cfg.usr_reg_cfg.fusion_mode;
    be_sync_param.short_thr     = alg_reg_cfg->wdr_reg_cfg.dyna_reg_cfg.short_thr;
    be_sync_param.long_thr      = alg_reg_cfg->wdr_reg_cfg.dyna_reg_cfg.long_thr;
    be_sync_param.wdr_max_ratio = alg_reg_cfg->wdr_reg_cfg.static_reg_cfg.max_ratio;

    /* AWB */
    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        be_sync_param.wb_gain[j] = alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_white_balance_gain[j];
    }

    /* blc */
    memcpy_s(&be_sync_param.be_blc, sizeof(isp_be_blc_dyna_cfg),
             &alg_reg_cfg->be_blc_cfg.dyna_blc, sizeof(isp_be_blc_dyna_cfg));

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_SYNC_PARAM_INIT, &be_sync_param);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Init BE Sync Param Failed with ec %#x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_alg_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_s64 ret = HI_SUCCESS;

    ret += isp_ae_reg_config(vi_pipe, reg_cfg_info, i);      /* ae */
    ret += isp_awb_reg_config(vi_pipe, reg_cfg_info, i);     /* awb */
    ret += isp_af_reg_config(vi_pipe, reg_cfg_info, i);      /* AF */
    ret += isp_sharpen_reg_config(vi_pipe, reg_cfg_info, i); /* sharpen */
    ret += isp_edge_mark_reg_config(vi_pipe, reg_cfg_info, i); /* sharpen */
    ret += isp_hlc_reg_config(vi_pipe, reg_cfg_info, i);      /* Hlc */
    ret += isp_demosaic_reg_config(vi_pipe, reg_cfg_info, i); /* demosaic */
    ret += isp_fpn_reg_config(vi_pipe, reg_cfg_info, i);     /* FPN */
    ret += isp_ldci_reg_config(vi_pipe, reg_cfg_info, i);    /* ldci */
    ret += isp_lcac_reg_config(vi_pipe, reg_cfg_info, i);    /* Local cac */
    ret += isp_gcac_reg_config(vi_pipe, reg_cfg_info, i);    /* global cac */
    ret += isp_fcr_reg_config(vi_pipe, reg_cfg_info, i);     /* FCR */
    ret += isp_dpc_reg_config(vi_pipe, reg_cfg_info, i);     /* dpc */
    ret += isp_ge_reg_config(vi_pipe, reg_cfg_info, i);      /* ge */
    ret += isp_lsc_reg_config(vi_pipe, reg_cfg_info, i);     /* BE LSC */
    ret += isp_gamma_reg_config(vi_pipe, reg_cfg_info, i);   /* gamma */
    ret += isp_csc_reg_config(vi_pipe, reg_cfg_info, i);     /* csc */
    ret += isp_ca_reg_config(vi_pipe, reg_cfg_info, i);      /* ca */
    ret += isp_mcds_reg_config(vi_pipe, reg_cfg_info, i);    /* mcds */
    ret += isp_wdr_reg_config(vi_pipe, reg_cfg_info, i);     /* wdr */
    ret += isp_drc_reg_config(vi_pipe, reg_cfg_info, i);     /* drc */
    ret += isp_dehaze_reg_config(vi_pipe, reg_cfg_info, i);  /* Dehaze */
    ret += isp_bayer_nr_reg_config(vi_pipe, reg_cfg_info, i); /* BayerNR */
    ret += isp_bnr_lsc_reg_config(vi_pipe, reg_cfg_info, i);  /* bayer_nr-LSC */
    ret += isp_dg_reg_config(vi_pipe, reg_cfg_info, i);      /* DG */
    ret += isp_4dg_reg_config(vi_pipe, reg_cfg_info, i);     /* 4DG */
    ret += isp_pregamma_reg_config(vi_pipe, reg_cfg_info, i); /* PreGamma */
    ret += isp_flick_reg_config(vi_pipe, reg_cfg_info, i);   /* Flicker */
    ret += isp_be_blc_reg_config(vi_pipe, reg_cfg_info, i);  /* be blc */
    ret += isp_clut_reg_config(vi_pipe, reg_cfg_info, i);    /* CLUT */
    ret += isp_detail_reg_config(vi_pipe, reg_cfg_info, i);  /* detail */
    ret += isp_expander_reg_config(vi_pipe, reg_cfg_info, i); /* expander */
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_regs_config(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u32 i;
    hi_s32 ret1;
    hi_s64 ret = HI_SUCCESS;
    S_VIPROC_REGS_TYPE *vi_proc_reg = HI_NULL;
    isp_usr_ctx        *isp_ctx     = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    ret += isp_set_be_raw(vi_pipe);

    for (i = 0; i < reg_cfg_info->cfg_num; i++) {
        ret += isp_system_reg_config(vi_pipe, reg_cfg_info, i);   /* sys */
        ret += isp_dither_reg_config(vi_pipe, reg_cfg_info, i);   /* dither */
        ret += isp_online_stt_reg_config(vi_pipe, reg_cfg_info, i); /* online stt */

        /* Be alg cfgs setting to register */
        ret += isp_be_alg_reg_config(vi_pipe, reg_cfg_info, i);
    }

    for (i = 0; i < reg_cfg_info->cfg_num; i++) {
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(vi_proc_reg);
        ret += isp_be_alg_sync_reg_config(vi_pipe, reg_cfg_info, i);
        ret += isp_be_alg_lut2stt_reg_new_reg_config(vi_pipe, reg_cfg_info, i);
        ret += isp_be_alg_lut_update_reg_config(vi_pipe, reg_cfg_info, i);
    }

    for (i = 0; i < reg_cfg_info->cfg_num; i++) {
        ret += isp_save_be_sync_reg(vi_pipe, reg_cfg_info, i);
    }

    if ((is_offline_mode(isp_ctx->block_attr.running_mode)) || (is_striping_mode(isp_ctx->block_attr.running_mode))) {
        ret1 = isp_cfg_be_buf_ctl(vi_pipe);
        if (ret1 != HI_SUCCESS) {
            isp_err_trace("ISP[%d] Be config bufs ctl failed %x!\n", vi_pipe, ret1);
            return ret1;
        }
    }

    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] isp_be_regs_config failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_regs_config_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u32 i;
    hi_s64 ret = HI_SUCCESS;
    S_VIPROC_REGS_TYPE *vi_proc_reg = HI_NULL;

    for (i = 0; i < reg_cfg_info->cfg_num; i++) {
        ret += isp_save_be_sync_reg(vi_pipe, reg_cfg_info, i);
    }

    for (i = 0; i < reg_cfg_info->cfg_num; i++) {
        ret += isp_reg_default(vi_pipe, reg_cfg_info, i);
        ret += isp_system_reg_config(vi_pipe, reg_cfg_info, i);    /* sys */
        ret += isp_dither_reg_config(vi_pipe, reg_cfg_info, i);    /* dither */
        ret += isp_online_stt_reg_config(vi_pipe, reg_cfg_info, i);
        /* Be alg cfgs setting to register */
        ret += isp_be_alg_reg_config(vi_pipe, reg_cfg_info, i);
    }

    for (i = 0; i < reg_cfg_info->cfg_num; i++) {
        vi_proc_reg = (S_VIPROC_REGS_TYPE *)isp_get_vi_proc_vir_addr(vi_pipe, i);
        isp_check_pointer_return(vi_proc_reg);
        ret += isp_be_alg_sync_reg_config(vi_pipe, reg_cfg_info, i);
        ret += isp_be_alg_lut2stt_reg_new_reg_config(vi_pipe, reg_cfg_info, i);
        isp_be_reg_up_write(vi_proc_reg, HI_TRUE);
        ret += isp_be_alg_lut_update_reg_config(vi_pipe, reg_cfg_info, i);
    }

    ret += isp_be_sync_param_init(vi_pipe, reg_cfg_info);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] isp_be_regs_config_init failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_reg_cfg_init(hi_vi_pipe vi_pipe)
{
    isp_reg_cfg_attr *reg_cfg_ctx = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg_ctx);

    if (reg_cfg_ctx == HI_NULL) {
        reg_cfg_ctx = (isp_reg_cfg_attr *)isp_malloc(sizeof(isp_reg_cfg_attr));
        if (reg_cfg_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] RegCfgCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(reg_cfg_ctx, sizeof(isp_reg_cfg_attr), 0, sizeof(isp_reg_cfg_attr));

    isp_regcfg_set_ctx(vi_pipe, reg_cfg_ctx);

    return HI_SUCCESS;
}

hi_void isp_reg_cfg_exit(hi_vi_pipe vi_pipe)
{
    isp_reg_cfg_attr *reg_cfg_ctx = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg_ctx);
    isp_free(reg_cfg_ctx);
    isp_regcfg_reset_ctx(vi_pipe);
}

hi_s32 isp_get_reg_cfg_ctx(hi_vi_pipe vi_pipe, hi_void **reg_cfg_info)
{
    isp_usr_ctx    *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_regcfg_get_ctx(vi_pipe, reg_cfg);
    isp_check_pointer_return(reg_cfg);

    if (!reg_cfg->init) {
        reg_cfg->reg_cfg.cfg_key.key = 0;

        reg_cfg->init = HI_TRUE;
    }

    reg_cfg->reg_cfg.cfg_num = isp_ctx->block_attr.block_num;

    *reg_cfg_info = &reg_cfg->reg_cfg;

    return HI_SUCCESS;
}

hi_s32 isp_reg_cfg_info_init(hi_vi_pipe vi_pipe)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    hi_s32 ret;
    isp_regcfg_get_ctx(vi_pipe, reg_cfg);
    isp_check_pointer_return(reg_cfg);

    ret = isp_fe_regs_config(vi_pipe, &reg_cfg->reg_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_fe_regs_config failed!\n", vi_pipe);
    }
    ret = isp_be_regs_config_init(vi_pipe, &reg_cfg->reg_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_be_regs_config_init failed!\n", vi_pipe);
    }
    return HI_SUCCESS;
}

hi_s32 isp_reg_cfg_info_set(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);
    isp_check_pointer_return(reg_cfg);

    ret = isp_fe_regs_config(vi_pipe, &reg_cfg->reg_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_fe_regs_config failed!\n", vi_pipe);
    }
    ret = isp_be_regs_config(vi_pipe, &reg_cfg->reg_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe:%d isp_be_regs_config failed!\n", vi_pipe);
    }

    if (reg_cfg->reg_cfg.kernel_reg_cfg.cfg_key.key) {
        ret = ioctl(isp_get_fd(vi_pipe), ISP_REG_CFG_SET, &reg_cfg->reg_cfg.kernel_reg_cfg);
        if (ret != HI_SUCCESS) {
            isp_err_trace("Config ISP register Failed with ec %#x!\n", ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

hi_void isp_sns_regs_info_check(hi_vi_pipe vi_pipe, hi_isp_sns_regs_info *sns_regs_info)
{
    if ((sns_regs_info->sns_type >= ISP_SNS_TYPE_BUTT)) {
        isp_err_trace("senor's regs info invalid, sns_type %d\n", sns_regs_info->sns_type);
        return;
    }

    if (sns_regs_info->reg_num > ISP_MAX_SNS_REGS) {
        isp_err_trace("senor's regs info invalid, reg_num %d\n", sns_regs_info->reg_num);
        return;
    }

    return;
}

static hi_void isp_get_start_and_end_vipipe(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx,
                                            hi_s32 *pipe_st, hi_s32 *pipe_ed)
{
    hi_s8 stitch_main_pipe;

    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        stitch_main_pipe = isp_ctx->stitch_attr.stitch_bind_id[0];

        if (is_stitch_main_pipe(vi_pipe, stitch_main_pipe)) {
            *pipe_st = 0;
            *pipe_ed = isp_ctx->stitch_attr.stitch_pipe_num - 1;
        } else {
            *pipe_st = vi_pipe;
            *pipe_ed = vi_pipe - 1;
        }
    } else {
        *pipe_st = vi_pipe;
        *pipe_ed = vi_pipe;
    }
}

static hi_void isp_normal_sync_cfg_get(hi_vi_pipe vi_pipe, hi_isp_sns_regs_info *sns_regs_info,
                                       isp_alg_reg_cfg *alg_reg_cfg, isp_sync_cfg_buf_node *sync_cfg_node)
{
    memcpy_s(&sync_cfg_node->sns_regs_info, sizeof(hi_isp_sns_regs_info), sns_regs_info, sizeof(hi_isp_sns_regs_info));
    isp_sns_regs_info_check(vi_pipe, &sync_cfg_node->sns_regs_info);
    memcpy_s(&sync_cfg_node->ae_reg_cfg, sizeof(isp_ae_reg_cfg_2), &alg_reg_cfg->ae_reg_cfg2, sizeof(isp_ae_reg_cfg_2));
    memcpy_s(&sync_cfg_node->awb_reg_cfg.wdr_wb_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM,
             &alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.wdr_wb_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM);
    memcpy_s(&sync_cfg_node->awb_reg_cfg.be_white_balance_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM,
             &alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_white_balance_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM);
    memcpy_s(&sync_cfg_node->awb_reg_cfg.color_matrix[0], sizeof(hi_u16) * CCM_MATRIX_SIZE,
             &alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_color_matrix[0], sizeof(hi_u16) * CCM_MATRIX_SIZE);
    memcpy_s(&sync_cfg_node->drc_reg_cfg, sizeof(isp_drc_sync_cfg),
             &alg_reg_cfg->drc_reg_cfg.sync_reg_cfg, sizeof(isp_drc_sync_cfg));
    memcpy_s(&sync_cfg_node->wdr_reg_cfg, sizeof(isp_fswdr_sync_cfg),
             &alg_reg_cfg->wdr_reg_cfg.sync_reg_cfg, sizeof(isp_fswdr_sync_cfg));
    memcpy_s(&sync_cfg_node->de_reg_cfg, sizeof(isp_de_reg_kernel),
             &alg_reg_cfg->de_reg_cfg.kernel_reg_cfg, sizeof(isp_de_reg_kernel));
    memcpy_s(&sync_cfg_node->be_blc_reg_cfg, sizeof(isp_be_blc_dyna_cfg),
             &alg_reg_cfg->be_blc_cfg.dyna_blc, sizeof(isp_be_blc_dyna_cfg));
    memcpy_s(&sync_cfg_node->fe_blc_reg_cfg, sizeof(isp_fe_blc_dyna_cfg),
             &alg_reg_cfg->fe_blc_cfg.dyna_blc, sizeof(isp_fe_blc_dyna_cfg));
    sync_cfg_node->awb_reg_cfg.be_awb_switch = alg_reg_cfg->awb_reg_cfg.awb_reg_usr_cfg.be_awb_switch;
}

static hi_void isp_stitch_sync_cfg_get(hi_vi_pipe vi_pipe, hi_isp_sns_regs_info *sns_regs_info,
                                       isp_alg_reg_cfg *alg_reg_cfg, isp_sync_cfg_buf_node *sync_cfg_node)
{
    hi_s8  stitch_main_pipe;
    isp_usr_ctx      *isp_ctx   = HI_NULL;
    isp_reg_cfg_attr *reg_cfg_s = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        return;
    }

    stitch_main_pipe = isp_ctx->stitch_attr.stitch_bind_id[0];
    if (is_stitch_main_pipe(vi_pipe, stitch_main_pipe)) {
        return;
    }

    isp_regcfg_get_ctx(stitch_main_pipe, reg_cfg_s);
    if (reg_cfg_s == HI_NULL) {
        return;
    }

    memcpy_s(&sync_cfg_node->sns_regs_info, sizeof(hi_isp_sns_regs_info),
             &reg_cfg_s->sync_cfg_node.sns_regs_info, sizeof(hi_isp_sns_regs_info));
    memcpy_s(&sync_cfg_node->sns_regs_info.com_bus, sizeof(hi_isp_sns_commbus),
             &sns_regs_info->com_bus, sizeof(hi_isp_sns_commbus));
    memcpy_s(&sync_cfg_node->sns_regs_info.slv_sync.slave_bind_dev, sizeof(hi_u32),
             &sns_regs_info->slv_sync.slave_bind_dev, sizeof(hi_u32));
    memcpy_s(&sync_cfg_node->ae_reg_cfg, sizeof(isp_ae_reg_cfg_2),
             &reg_cfg_s->reg_cfg.alg_reg_cfg[0].ae_reg_cfg2, sizeof(isp_ae_reg_cfg_2));
    memcpy_s(&sync_cfg_node->awb_reg_cfg.color_matrix[0], sizeof(hi_u16) * CCM_MATRIX_SIZE,
             &alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_color_matrix[0], sizeof(hi_u16) * CCM_MATRIX_SIZE);
    memcpy_s(&sync_cfg_node->awb_reg_cfg.be_white_balance_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM,
             &alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.be_white_balance_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM);
    memcpy_s(&sync_cfg_node->awb_reg_cfg.wdr_wb_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM,
             &alg_reg_cfg->awb_reg_cfg.awb_reg_dyn_cfg.wdr_wb_gain[0], sizeof(hi_u32) * ISP_BAYER_CHN_NUM);
    sync_cfg_node->awb_reg_cfg.be_awb_switch = alg_reg_cfg->awb_reg_cfg.awb_reg_usr_cfg.be_awb_switch;
}

hi_s32 isp_sync_cfg_set(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_s32 pipe_st = 0;
    hi_s32 pipe_ed = 0;

    isp_usr_ctx    *isp_ctx  = HI_NULL;
    isp_reg_cfg_attr *reg_cfg  = HI_NULL;

    hi_isp_sns_regs_info *sns_regs_info = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_get_start_and_end_vipipe(vi_pipe, isp_ctx, &pipe_st, &pipe_ed);

    while (pipe_st <= pipe_ed) {
        if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
            vi_pipe = isp_ctx->stitch_attr.stitch_bind_id[pipe_st];
        } else {
            vi_pipe = pipe_st;
        }

        isp_get_ctx(vi_pipe, isp_ctx);
        isp_regcfg_get_ctx(vi_pipe, reg_cfg);
        isp_check_pointer_return(reg_cfg);
        isp_check_open_return(vi_pipe);

        if (isp_sensor_update_sns_reg(vi_pipe) != HI_SUCCESS) {
            /* If Users need to config AE sync info themselves, they can set pfn_cmos_get_sns_reg_info
             *    to HI_NULL in cmos.c
             */
            /* Then there will be NO AE sync configs in kernel of firmware */
            return HI_SUCCESS;
        }

        isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);

        isp_normal_sync_cfg_get(vi_pipe, sns_regs_info, &reg_cfg->reg_cfg.alg_reg_cfg[0], &reg_cfg->sync_cfg_node);
        isp_stitch_sync_cfg_get(vi_pipe, sns_regs_info, &reg_cfg->reg_cfg.alg_reg_cfg[0], &reg_cfg->sync_cfg_node);

        reg_cfg->sync_cfg_node.valid = HI_TRUE;
        ret = ioctl(isp_get_fd(vi_pipe), ISP_SYNC_CFG_SET, &reg_cfg->sync_cfg_node);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        sns_regs_info->config = HI_TRUE;

        pipe_st++;
    }

    return HI_SUCCESS;
}

hi_s32 isp_snap_reg_cfg_set(hi_vi_pipe vi_pipe, hi_isp_config_info *snap_info)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_CONFIG_INFO_SET, snap_info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_snap_reg_cfg_get(hi_vi_pipe vi_pipe, isp_snap_info *snap_info)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_SNAP_INFO_GET, snap_info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_bool isp_pro_trigger_get(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_bool enable;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_PRO_TRIGGER_GET, &enable);
    if (ret != HI_SUCCESS) {
        return HI_FALSE;
    }

    return enable;
}

hi_s32 isp_reg_cfg_ctrl(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_usr_ctx    *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    hi_s32 ret = HI_SUCCESS;
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_regcfg_get_ctx(vi_pipe, reg_cfg);
    reg_cfg->reg_cfg.cfg_key.key = 0xFFFFFFFFFFFFFFFF;
    for (i = isp_ctx->block_attr.pre_block_num; i < isp_ctx->block_attr.block_num; i++) {
        memcpy_s(&reg_cfg->reg_cfg.alg_reg_cfg[i], sizeof(isp_alg_reg_cfg),
                 &reg_cfg->reg_cfg.alg_reg_cfg[0], sizeof(isp_alg_reg_cfg));
    }

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        for (i = 0; i < MIN2(ISP_MAX_BE_NUM, isp_ctx->block_attr.block_num); i++) {
            isp_ctx->special_opt.be_on_stt_update[i] = HI_TRUE;
        }

        for (i = isp_ctx->block_attr.pre_block_num; i < isp_ctx->block_attr.block_num; i++) {
            ret = isp_be_resh_cfg(&reg_cfg->reg_cfg.alg_reg_cfg[i]);
            if (ret != HI_SUCCESS) {
                isp_err_trace("%d isp_be_resh_cfg failed\n", vi_pipe);
            }
        }
    }

    reg_cfg->reg_cfg.cfg_num = isp_ctx->block_attr.block_num;

    return HI_SUCCESS;
}

hi_s32  isp_set_be_raw(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}

static hi_void isp_gamma_lut_apb_reg_config(S_ISPBE_REGS_TYPE *be_reg, isp_gamma_usr_cfg *usr_reg_cfg)
{
    hi_u16 j;
    isp_gamma_lut_waddr_write(be_reg, 0);

    for (j = 0; j < GAMMA_REG_NODE_NUM; j++) {
        isp_gamma_lut_wdata_write(be_reg, usr_reg_cfg->gamma_lut[j]);
    }
}

static hi_void isp_sharpen_lut_apb_reg_config(S_ISPBE_REGS_TYPE *be_reg,
                                              isp_sharpen_mpi_dyna_reg_cfg *mpi_dyna_reg_cfg)
{
    hi_u16 j;
    isp_sharpen_mfgaind_waddr_write(be_reg, 0);
    isp_sharpen_mfgainud_waddr_write(be_reg, 0);
    isp_sharpen_hfgaind_waddr_write(be_reg, 0);
    isp_sharpen_hfgainud_waddr_write(be_reg, 0);

    for (j = 0; j < SHRP_GAIN_LUT_SIZE; j++) {
        isp_sharpen_mfgaind_wdata_write(be_reg,  mpi_dyna_reg_cfg->mf_gain_d[j]);
        isp_sharpen_mfgainud_wdata_write(be_reg, mpi_dyna_reg_cfg->mf_gain_ud[j]);
        isp_sharpen_hfgaind_wdata_write(be_reg,  mpi_dyna_reg_cfg->hf_gain_d[j]);
        isp_sharpen_hfgainud_wdata_write(be_reg, mpi_dyna_reg_cfg->hf_gain_ud[j]);
    }

    isp_sharpen_lut_update_write(be_reg, HI_TRUE);
}

static hi_void isp_pregamma_lut_apb_reg_config(S_ISPBE_REGS_TYPE *be_reg,
                                               isp_pregamma_dyna_cfg *dyna_reg_cfg)
{
    hi_u16 j;
    if (dyna_reg_cfg->switch_mode == HI_TRUE) {
        isp_pregamma_lut_waddr_write(be_reg, 0);

        for (j = 0; j < PREGAMMA_NODE_NUM; j++) {
            isp_pregamma_lut_wdata_write(be_reg, dyna_reg_cfg->pregamma_lut[j]);
        }
    }
}

static hi_void isp_ldci_lut_apb_reg_config(S_ISPBE_REGS_TYPE *be_reg, isp_usr_ctx *isp_ctx,
                                           isp_ldci_dyna_cfg *dyna_reg_cfg, isp_ldci_static_cfg *static_reg_cfg)
{
    hi_u16 j;

    isp_ldci_drc_waddr_write(be_reg, 0);
    isp_ldci_cgain_waddr_write(be_reg, 0);
    isp_ldci_de_usm_waddr_write(be_reg, 0);
    isp_ldci_he_waddr_write(be_reg, 0);

    for (j = 0; j < 65; j++) { /* ldci drc lut table number 65 */
        isp_ldci_drc_wdata_write(be_reg, static_reg_cfg->calc_dyn_rng_cmp_lut[j],
                                 static_reg_cfg->stat_dyn_rng_cmp_lut[j]);
    }

    for (j = 0; j < LDCI_COLOR_GAIN_LUT_SIZE; j++) {
        isp_ldci_cgain_wdata_write(be_reg, dyna_reg_cfg->color_gain_lut[j]);
    }

    for (j = 0; j < LDCI_DE_USM_LUT_SIZE; j++) {
        isp_ldci_de_usm_wdata_write(be_reg, dyna_reg_cfg->usm_pos_lut[j],
                                    dyna_reg_cfg->usm_neg_lut[j], dyna_reg_cfg->de_lut[j]);
    }

    for (j = 0; j < LDCI_HE_LUT_SIZE; j++) {
        isp_ldci_he_wdata_write(be_reg, dyna_reg_cfg->he_pos_lut[j], dyna_reg_cfg->he_neg_lut[j]);
    }
    if (isp_ctx->frame_cnt == 0) {
        isp_ldci_drc_lut_update_write(be_reg, HI_TRUE);
    }

    isp_ldci_calc_lut_update_write(be_reg, HI_TRUE);
}

static hi_s32 isp_lut_apb_reg_config(hi_vi_pipe vi_pipe, isp_reg_cfg_attr *reg_cfg)
{
    hi_u8  i;
    S_ISPBE_REGS_TYPE *be_reg     = HI_NULL;
    isp_usr_ctx         *isp_ctx    = HI_NULL;
    isp_alg_reg_cfg *alg_reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        alg_reg_cfg = &reg_cfg->reg_cfg.alg_reg_cfg[i];
        be_reg = (S_ISPBE_REGS_TYPE *)isp_get_be_vir_addr(vi_pipe, i);

        isp_check_pointer_return(be_reg);

        /* gamma */
        isp_gamma_lut_apb_reg_config(be_reg, &alg_reg_cfg->gamma_cfg.usr_reg_cfg);

        /* sharpen */
        isp_sharpen_lut_apb_reg_config(be_reg, &alg_reg_cfg->sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg);

        /* PreGamma */
        isp_pregamma_lut_apb_reg_config(be_reg, &alg_reg_cfg->pregamma_reg_cfg.dyna_reg_cfg);

        /* LDCI */
        isp_ldci_lut_apb_reg_config(be_reg, isp_ctx, &alg_reg_cfg->ldci_reg_cfg.dyna_reg_cfg,
                                    &alg_reg_cfg->ldci_reg_cfg.static_reg_cfg);
    }

    return HI_SUCCESS;
}

hi_s32 isp_switch_reg_set(hi_vi_pipe vi_pipe)
{
    hi_s32 ret = HI_SUCCESS;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_online_mode(isp_ctx->block_attr.running_mode) || \
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_regcfg_get_ctx(vi_pipe, reg_cfg);
        isp_check_pointer_return(reg_cfg);
        isp_lut_apb_reg_config(vi_pipe, reg_cfg);
        ret = isp_reg_cfg_info_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("vi_pipe %d isp_reg_cfg_info_init failed \n", vi_pipe);
        }
        if (reg_cfg->reg_cfg.kernel_reg_cfg.cfg_key.key) {
            ret = ioctl(isp_get_fd(vi_pipe), ISP_REG_CFG_SET, &reg_cfg->reg_cfg.kernel_reg_cfg);
            if (ret != HI_SUCCESS) {
                isp_err_trace("Config ISP register Failed with ec %#x!\n", ret);
                return ret;
            }
        }

        isp_ctx->block_attr.pre_block_num = isp_ctx->block_attr.block_num;

        return HI_SUCCESS;
    }

    /* record the register config infomation to fhy and kernel,and be valid in next frame. */
    ret = isp_reg_cfg_info_init(vi_pipe);
    if (ret) {
        return ret;
    }

    isp_ctx->para_rec.stitch_sync = HI_TRUE;
    ret = ioctl(isp_get_fd(vi_pipe), ISP_SYNC_INIT_SET, &isp_ctx->para_rec.stitch_sync);
    if (ret != HI_SUCCESS) {
        isp_ctx->para_rec.stitch_sync = HI_FALSE;
        isp_err_trace("ISP[%d] set isp stitch sync failed!\n", vi_pipe);
    }

    ret = isp_all_cfgs_be_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe:%d init all be bufs failed \n", vi_pipe);
        return ret;
    }

    isp_ctx->block_attr.pre_block_num = isp_ctx->block_attr.block_num;

    return HI_SUCCESS;
}

hi_s32 isp_lut2stt_apb_reg(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    hi_s32  ret = HI_SUCCESS;
    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_online_mode(isp_ctx->block_attr.running_mode) ||
        is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        isp_regcfg_get_ctx(vi_pipe, reg_cfg);
        isp_check_pointer_return(reg_cfg);

        ret = isp_lut_apb_reg_config(vi_pipe, reg_cfg);
        if (ret != HI_SUCCESS) {
            isp_err_trace("pipe:%d init all be bufs failed \n", vi_pipe);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_switch_state_set(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_SWITCH_FINISH_STATE_SET);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

