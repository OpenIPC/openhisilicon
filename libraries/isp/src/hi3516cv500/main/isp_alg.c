/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_alg.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <isp_alg.h>

/* find the specified library */
hi_s32 isp_find_lib(const isp_lib_node *lib_node, const hi_isp_alg_lib *lib)
{
    hi_s32 i;

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        /* can't use memcmp, if user not fill the string. */
        if ((strncmp(lib_node[i].alg_lib.lib_name, lib->lib_name, ALG_LIB_NAME_SIZE_MAX) == 0) &&
            (lib_node[i].alg_lib.id == lib->id)) {
            return i;
        }
    }

    return -1;
}

/* search the empty pos of libs */
isp_lib_node *isp_search_lib(isp_lib_node *lib_node)
{
    hi_s32 i;

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (!lib_node[i].used) {
            return &lib_node[i];
        }
    }

    return HI_NULL;
}

/* search the empty pos of algs */
isp_alg_node *isp_search_alg(isp_alg_node *algs)
{
    hi_s32 i;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (!algs[i].used) {
            return &algs[i];
        }
    }

    return HI_NULL;
}

hi_void isp_algs_init(const isp_alg_node *algs, hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 i;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (algs[i].used) {
            if (algs[i].alg_func.pfn_alg_init != HI_NULL) {
                algs[i].alg_func.pfn_alg_init(vi_pipe, reg_cfg);
            }
        }
    }
}

hi_void isp_algs_run(const isp_alg_node *algs, hi_vi_pipe vi_pipe,
                     const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_s32 i;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (algs[i].used) {
            if (algs[i].alg_func.pfn_alg_run != HI_NULL) {
                algs[i].alg_func.pfn_alg_run(vi_pipe, stat_info, reg_cfg_info, rsv);
            }
        }
    }
}

hi_void isp_algs_ctrl(const isp_alg_node *algs, hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    hi_s32 i;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (algs[i].used) {
            if (algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                algs[i].alg_func.pfn_alg_ctrl(vi_pipe, cmd, value);
            }
        }
    }
}

hi_void isp_algs_exit(hi_vi_pipe vi_pipe, const isp_alg_node *algs)
{
    hi_s32 i;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (algs[i].used) {
            if (algs[i].alg_func.pfn_alg_exit != HI_NULL) {
                algs[i].alg_func.pfn_alg_exit(vi_pipe);
            }
        }
    }
}

hi_void isp_algs_register(hi_vi_pipe vi_pipe)
{
    isp_alg_register_ae(vi_pipe);
    isp_alg_register_awb(vi_pipe);
#ifdef CONFIG_HI_ISP_AF_SUPPORT
    isp_alg_register_af(vi_pipe);
#endif
    isp_alg_register_dpc(vi_pipe);

#ifdef CONFIG_HI_ISP_CR_SUPPORT
    isp_alg_register_ge(vi_pipe);
#endif
    isp_alg_register_frame_wdr(vi_pipe);
    isp_alg_register_expander(vi_pipe);
    isp_alg_register_blc(vi_pipe);
    isp_alg_register_bayer_nr(vi_pipe);
    isp_alg_register_split(vi_pipe);
    isp_alg_register_flicker(vi_pipe);
    isp_alg_register_dg(vi_pipe);
    isp_alg_register_hrs(vi_pipe);
    isp_alg_register_fe_lsc(vi_pipe);
    isp_alg_register_lsc(vi_pipe);
    isp_alg_register_acs(vi_pipe);
    isp_alg_register_rlsc(vi_pipe);
    isp_alg_register_rc(vi_pipe);
    isp_alg_register_drc(vi_pipe);
    isp_alg_register_dehaze(vi_pipe);
    isp_alg_register_lcac(vi_pipe);

#ifdef CONFIG_HI_ISP_GCAC_SUPPORT
    isp_alg_register_gcac(vi_pipe);
#endif

    isp_alg_register_demosaic(vi_pipe);
    isp_alg_register_fcr(vi_pipe);
    isp_alg_register_gamma(vi_pipe);
    isp_alg_register_csc(vi_pipe);

#ifdef CONFIG_HI_ISP_CA_SUPPORT
    isp_alg_register_ca(vi_pipe);
#endif

#ifdef CONFIG_HI_VI_FPN_SUPPORT
    isp_alg_register_fpn(vi_pipe);
#endif
    isp_alg_register_sharpen(vi_pipe);

#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    isp_alg_register_edge_mark(vi_pipe);
#endif
    isp_alg_register_mcds(vi_pipe);
    isp_alg_register_ldci(vi_pipe);

#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    isp_alg_register_pre_gamma(vi_pipe);
#endif
    isp_alg_register_fe_log_lut(vi_pipe);
    isp_alg_register_log_lut(vi_pipe);
    isp_alg_register_clut(vi_pipe);

#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    isp_alg_register_hlc(vi_pipe);
#endif
    isp_alg_register_detail(vi_pipe);
    isp_alg_register_rgbir(vi_pipe);
    return;
}

hi_void isp_yuv_algs_register(hi_vi_pipe vi_pipe)
{
#ifdef CONFIG_HI_ISP_AF_SUPPORT
    isp_alg_register_af(vi_pipe);
#endif

#ifdef CONFIG_HI_ISP_CA_SUPPORT
    isp_alg_register_ca(vi_pipe);
#endif

    isp_alg_register_sharpen(vi_pipe);
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    isp_alg_register_edge_mark(vi_pipe);
#endif
    isp_alg_register_mcds(vi_pipe);
    isp_alg_register_ldci(vi_pipe);

    return;
}

/* resolve problem: isp error: null pointer in isp_alg_registerxxx when return isp_init without app exit */
hi_void isp_algs_unregister(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_s32 i;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (isp_ctx->algs[i].used) {
            isp_ctx->algs[i].used = HI_FALSE;
        }
    }
}

/* resolve problem: isp error: HI_MPI_XX_Register failed when return 3a lib register without app exit */
hi_void isp_libs_unregister(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_s32 i;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        isp_ctx->ae_lib_info.libs[i].used  = HI_FALSE;
        isp_ctx->awb_lib_info.libs[i].used = HI_FALSE;
        isp_ctx->af_lib_info.libs[i].used  = HI_FALSE;
    }
}

