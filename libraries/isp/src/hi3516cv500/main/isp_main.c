/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_main.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_main.h"
#include "isp_ext_config.h"
#include "isp_defaults.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_inner.h"

/* Isp Version Proc will be Hi3518_ISP_V1.0.4.x, ISP_LIB_VERSION stands for x */
#define ISP_LIB_VERSION   "0"    /* [0~F] */

static isp_version g_version = {
    .mpp_version = ISP_LIB_VERSION,
    .magic = 0,
};

static hi_bool isp_get_version(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_VERSION, &g_version);
    if (ret) {
        isp_err_trace("register ISP[%d] lib info ec %x!\n", vi_pipe, ret);
        return HI_FALSE;
    }

    return HI_TRUE;
}

static hi_s32 pq_ai_attr_get(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    isp_ctx->ai_info.ai_pipe_id   = -1;
    isp_ctx->ai_info.base_pipe_id = -1;
    isp_ctx->ai_info.pq_ai_en     = HI_FALSE;
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
    hi_s32 ret;
    pq_ai_attr ai_attr;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_PQ_AI_GROUP_ATTR_GET, &ai_attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    if (ai_attr.pq_ai_en == HI_TRUE) {
        if ((ai_attr.ai_pipe_id < 0) || (ai_attr.ai_pipe_id >= ISP_MAX_PIPE_NUM) ||
            (ai_attr.base_pipe_id < 0) || (ai_attr.base_pipe_id >= ISP_MAX_PIPE_NUM)) {
            return HI_FAILURE;
        }

        if (ai_attr.ai_pipe_id == ai_attr.base_pipe_id) {
            return HI_FAILURE;
        }

        if ((vi_pipe != ai_attr.ai_pipe_id) && (vi_pipe != ai_attr.base_pipe_id)) {
            return HI_FAILURE;
        }
    }

    isp_ctx->ai_info.pq_ai_en     = ai_attr.pq_ai_en;
    isp_ctx->ai_info.ai_pipe_id   = ai_attr.ai_pipe_id;
    isp_ctx->ai_info.base_pipe_id = ai_attr.base_pipe_id;

    return HI_SUCCESS;
#else
    return HI_SUCCESS;
#endif
}

hi_s32 isp_mem_init_update_ctx(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    isp_working_mode isp_work_mode;

    /* get pipe size */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_PIPE_SIZE, &isp_ctx->pipe_size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get pipe size failed\n", vi_pipe);
        return ret;
    }

    /* HDR attribute */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_HDR_ATTR, &isp_ctx->hdr_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get HDR attr failed\n", vi_pipe);
        return ret;
    }

    /* stitch attribute */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_STITCH_ATTR, &isp_ctx->stitch_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get stitch attr failed\n", vi_pipe);
        return ret;
    }

    /* p2en info */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_P2EN_INFO_GET, &isp_ctx->isp0_p2_en);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get p2en info failed\n", vi_pipe);
        return ret;
    }

    /* be buf num */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_BE_BUF_NUM_GET, &isp_ctx->be_buf_num);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get be buf num failed\n", vi_pipe);
        return ret;
    }

    if (isp_get_version(vi_pipe) != HI_TRUE) {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* init block attribute(inclde write ext registers, should after vreg_init) */
    ret = isp_block_init(vi_pipe, &isp_ctx->block_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init block failed!\n", vi_pipe);
        return ret;
    }

    ret = pq_ai_attr_get(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get pq ai attr failed!\n", vi_pipe);
        return ret;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] work mode get failed!\n", vi_pipe);
        return ret;
    }

    if (isp_work_mode.data_input_mode == ISP_MODE_BT1120_YUV) {
        isp_ctx->isp_yuv_mode = HI_TRUE;
        ret = isp_sensor_ctx_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            return ret;
        }
    } else if (isp_work_mode.data_input_mode == ISP_MODE_RAW) {
        isp_ctx->isp_yuv_mode = HI_FALSE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_check_sns_register(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    if ((isp_ctx->sns_reg != HI_TRUE) && (isp_ctx->isp_yuv_mode != HI_TRUE)) {
        isp_err_trace("Sensor doesn't register to ISP[%d]!\n", vi_pipe);
        return HI_ERR_ISP_SNS_UNREGISTER;
    }

    return HI_SUCCESS;
}

hi_s32 isp_mem_info_set(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;

    memset_s(&isp_ctx->para_rec, sizeof(isp_para_rec), 0, sizeof(isp_para_rec));
    hi_ext_top_wdr_cfg_write(vi_pipe, isp_ctx->para_rec.wdr_cfg);
    hi_ext_top_pub_attr_cfg_write(vi_pipe, isp_ctx->para_rec.pub_cfg);

    hi_ext_top_wdr_switch_write(vi_pipe, HI_FALSE);
    hi_ext_top_res_switch_write(vi_pipe, HI_FALSE);

    isp_ctx->mem_init = HI_TRUE;
    ret = ioctl(isp_get_fd(vi_pipe), ISP_MEM_INFO_SET, &isp_ctx->mem_init);
    if (ret != HI_SUCCESS) {
        isp_ctx->mem_init = HI_FALSE;
        isp_err_trace("ISP[%d] set mem info failed!\n", vi_pipe);
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    return HI_SUCCESS;
}

hi_s32 isp_update_info(hi_vi_pipe vi_pipe)
{
    hi_s32 i, ret;

    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    isp_check_open_return(vi_pipe);

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (isp_ctx->algs[i].used) {
            if (isp_ctx->algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                isp_ctx->algs[i].alg_func.pfn_alg_ctrl(vi_pipe, ISP_UPDATE_INFO_GET, &isp_ctx->dcf_update_info);
            }
        }
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_UPDATE_INFO_SET, &isp_ctx->dcf_update_info);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set dcf updateinfo failed\n", vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 isp_update_frame_info(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_open_return(vi_pipe);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_FRAME_INFO_SET, &isp_ctx->frame_info);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set frameinfo failed\n", vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 isp_update_attach_info(hi_vi_pipe vi_pipe)
{
    hi_s32 i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->attach_info_ctrl.attach_info == HI_NULL) {
        isp_err_trace("pipe:%d the isp attach info hasn't init!\n", vi_pipe);
        return HI_FAILURE;
    }

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (isp_ctx->algs[i].used) {
            if (isp_ctx->algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                isp_ctx->algs[i].alg_func.pfn_alg_ctrl(vi_pipe, ISP_ATTACHINFO_GET,
                                                       isp_ctx->attach_info_ctrl.attach_info);
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_update_color_gamut_info(hi_vi_pipe vi_pipe)
{
    hi_s32 i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->gamut_info_ctrl.color_gamut_info == HI_NULL) {
        isp_err_trace("pipe:%d the isp color gamut info hasn't init!\n", vi_pipe);
        return HI_FAILURE;
    }

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (isp_ctx->algs[i].used) {
            if (isp_ctx->algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                isp_ctx->algs[i].alg_func.pfn_alg_ctrl(vi_pipe, ISP_COLORGAMUTINFO_GET,
                                                       isp_ctx->gamut_info_ctrl.color_gamut_info);
            }
        }
    }

    return HI_SUCCESS;
}

hi_void isp_read_ext_regs(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_u64 phy_addr_high;
    hi_u64 phy_addr_temp;

    isp_get_ctx(vi_pipe, isp_ctx);

    phy_addr_high = (hi_u64)hi_ext_system_sys_debug_high_addr_read(vi_pipe);
    phy_addr_temp = (hi_u64)hi_ext_system_sys_debug_low_addr_read(vi_pipe);
    phy_addr_temp |= (phy_addr_high << 32); /* left shift 32bits */

    isp_ctx->freeze_fw = hi_ext_system_freeze_firmware_read(vi_pipe);

    isp_ctx->isp_dbg.debug_en = hi_ext_system_sys_debug_enable_read(vi_pipe);
    isp_ctx->isp_dbg.phy_addr = phy_addr_temp;

    isp_ctx->isp_dbg.depth = hi_ext_system_sys_debug_depth_read(vi_pipe);
    isp_ctx->isp_dbg.size  = hi_ext_system_sys_debug_size_read(vi_pipe);
    isp_ctx->be_raw_info.enable_be_raw = hi_ext_system_isp_raw_position_enable_read(vi_pipe);
}

hi_void isp_update_ext_regs(hi_vi_pipe vi_pipe)
{
    return;
}

hi_s32 isp_wdr_cfg_set(hi_vi_pipe vi_pipe)
{
    hi_u8  i;
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_reg_cfg_attr    *reg_cfg = HI_NULL;
    isp_wdr_cfg wdr_cfg;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);
    isp_check_pointer_return(reg_cfg);

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_sensor_get_default(vi_pipe, &sns_dft);
    memset_s(&wdr_cfg, sizeof(isp_wdr_cfg), 0, sizeof(isp_wdr_cfg));

    wdr_cfg.wdr_mode = isp_ctx->sns_wdr_mode;

    for (i = 0; i < EXP_RATIO_NUM; i++) {
        wdr_cfg.exp_ratio[i] = CLIP3(sns_dft->wdr_switch_attr.exp_ratio[i], 0x40, 0xFFF);
    }

    memcpy_s(&wdr_cfg.wdr_reg_cfg, sizeof(isp_fswdr_sync_cfg),
             &reg_cfg->reg_cfg.alg_reg_cfg[0].wdr_reg_cfg.sync_reg_cfg, sizeof(isp_fswdr_sync_cfg));

    ret = ioctl(isp_get_fd(vi_pipe), ISP_WDR_CFG_SET, &wdr_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe:%d config WDR register failed with ec %#x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_init_set_sensor_mode(hi_vi_pipe vi_pipe)
{
    hi_s32  ret;
    hi_u8   wdr_mode;
    hi_u32  fps_value;
    hi_void *value = HI_NULL;
    hi_isp_cmos_sensor_image_mode sns_image_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->isp_yuv_mode == HI_TRUE) {
        return HI_SUCCESS;
    }

    /* set sensor wdr mode */
    wdr_mode = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    ret      = isp_sensor_set_wdr_mode(vi_pipe, wdr_mode);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set sensor WDR mode failed!\n", vi_pipe);
        return HI_ERR_ISP_NOT_INIT;
    }

    sns_image_mode.width  = hi_ext_top_sensor_width_read(vi_pipe);
    sns_image_mode.height = hi_ext_top_sensor_height_read(vi_pipe);
    fps_value = hi_ext_system_fps_base_read(vi_pipe);
    value     = (hi_void *)&fps_value;

    sns_image_mode.fps      = *(hi_float *)value;
    sns_image_mode.sns_mode = hi_ext_system_sensor_mode_read(vi_pipe);

    ret = isp_sensor_set_image_mode(vi_pipe, &sns_image_mode);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set sensor image mode failed!\n", vi_pipe);
        return HI_ERR_ISP_NOT_INIT;
    }

    return HI_SUCCESS;
}

hi_s32 isp_init_sensor_update(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    if (isp_ctx->isp_yuv_mode == HI_FALSE) {
        ret = isp_sensor_update_all(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] update sensor all failed!\n", vi_pipe);
            return HI_ERR_ISP_NOT_INIT;
        }
    } else {
        ret = isp_sensor_update_all_yuv(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] update sensor all failed!\n", vi_pipe);
            return HI_ERR_ISP_NOT_INIT;
        }
    }

    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_ctx->frame_info_ctrl.isp_frame->sensor_id   = sns_dft->sensor_mode.sensor_id;
    isp_ctx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;
    if (isp_ctx->isp_yuv_mode == HI_FALSE) {
        isp_ctx->frame_info.sensor_id               = sns_dft->sensor_mode.sensor_id;
        isp_ctx->frame_info.sensor_mode             = sns_dft->sensor_mode.sensor_mode;
    }
    /* get dng paramters from CMOS.c */
    isp_dng_ext_regs_initialize(vi_pipe, (hi_isp_dng_color_param *)(&sns_dft->dng_color_param));
    hi_ext_system_dng_static_info_valid_write(vi_pipe, sns_dft->sensor_mode.valid_dng_raw_format);

    if (sns_dft->sensor_mode.valid_dng_raw_format == HI_TRUE) {
        memcpy_s(&isp_ctx->dng_info_ctrl.isp_dng->dng_raw_format, sizeof(hi_isp_dng_raw_format),
                 &sns_dft->sensor_mode.dng_raw_format, sizeof(hi_isp_dng_raw_format));
    } else {
        isp_err_trace("ISP[%d] dng_info not initialized in cmos.c!\n", vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 isp_stitch_sync_init_set(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;

    isp_ctx->para_rec.stitch_sync = HI_TRUE;
    ret = ioctl(isp_get_fd(vi_pipe), ISP_SYNC_INIT_SET, &isp_ctx->para_rec.stitch_sync);
    if (ret != HI_SUCCESS) {
        isp_ctx->para_rec.stitch_sync = HI_FALSE;
        isp_err_trace("ISP[%d] set isp stitch sync failed!\n", vi_pipe);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_init_global_info(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    const hi_u64 hand_signal = ISP_INIT_HAND_SIGNAL;

    isp_ctx->para_rec.init = HI_TRUE;
    ret = ioctl(isp_get_fd(vi_pipe), ISP_INIT_INFO_SET, &isp_ctx->para_rec.init);
    if (ret != HI_SUCCESS) {
        isp_ctx->para_rec.init = HI_FALSE;
        isp_err_trace("ISP[%d] set isp init info failed!\n", vi_pipe);
        return ret;
    }
    isp_ctx->para_rec.run_en = HI_TRUE;

    /* set handshake signal */
    if (ioctl(isp_get_fd(vi_pipe), ISP_RUN_STATE_SET, &hand_signal)) {
        return ret;
    }

    return HI_SUCCESS;
}

hi_void isp_algs_register_pq_ai(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
    if ((isp_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == isp_ctx->ai_info.ai_pipe_id)) {
        if (isp_ctx->isp_alg_exp.pfn_alg_register != HI_NULL) {
            isp_ctx->isp_alg_exp.pfn_alg_register(vi_pipe);
        }
    }
#endif
}

hi_void isp_register_algs(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    if (isp_ctx->isp_yuv_mode == HI_TRUE) {
        isp_yuv_algs_register(vi_pipe);
    } else {
        isp_algs_register(vi_pipe);
        isp_algs_register_pq_ai(vi_pipe, isp_ctx);
    }
}

hi_s32 isp_apb_reg_pre_cfg(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        ret = isp_lut2stt_apb_reg(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] init lut2stt reg failed!\n", vi_pipe);
            return ret;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_should_init_sensor(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    hi_isp_sns_regs_info *sns_regs_info = HI_NULL;

    if (isp_ctx->isp_yuv_mode == HI_TRUE) {
        isp_ctx->linkage.cfg2valid_delay_max = 2; /* 2: default delay max */
    } else {
        ret = isp_sensor_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] init sensor failed!\n", vi_pipe);
            return ret;
        }
        isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);
        isp_ctx->linkage.cfg2valid_delay_max = sns_regs_info->cfg2_valid_delay_max;
    }

    return HI_SUCCESS;
}

hi_void isp_comm_reg_init(hi_vi_pipe vi_pipe)
{
    isp_ext_regs_default(vi_pipe);
    isp_regs_default(vi_pipe);
    isp_ext_regs_initialize(vi_pipe);
    isp_regs_initialize(vi_pipe);
}

hi_s32 isp_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_void *reg_cfg = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    isp_ctx->sys_rect.x      = hi_ext_system_corp_pos_x_read(vi_pipe);
    isp_ctx->sys_rect.y      = hi_ext_system_corp_pos_y_read(vi_pipe);
    isp_ctx->sys_rect.width  = hi_ext_sync_total_width_read(vi_pipe);
    isp_ctx->sys_rect.height = hi_ext_sync_total_height_read(vi_pipe);

    /* isp alg buf init */
    ret = isp_alg_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return HI_ERR_ISP_NOT_INIT;
    }

    /* regcfg init */
    ret = isp_reg_cfg_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail1;
    }

    /* set sensor mode */
    ret = isp_init_set_sensor_mode(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail2;
    }

    /* isp update senor info */
    ret = isp_init_sensor_update(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail2;
    }

    /* init the common part of extern registers and real registers */
    isp_comm_reg_init(vi_pipe);

    /* isp algs global variable initialize */
    isp_global_initialize(vi_pipe, isp_ctx);

    /* register all algorithm to isp, and init them. */
    isp_register_algs(vi_pipe, isp_ctx);

    /* get regcfg */
    isp_get_reg_cfg_ctx(vi_pipe, &reg_cfg);

    isp_algs_init(isp_ctx->algs, vi_pipe, reg_cfg);

    ret = isp_apb_reg_pre_cfg(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail3;
    }

    /* set WDR mode to kernel. */
    ret = isp_wdr_cfg_set(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set WDR mode to kernel failed!\n", vi_pipe);
        goto fail3;
    }

    /* sensor init */
    ret = isp_should_init_sensor(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail3;
    }

    /* regcfg info set */
    ret = isp_reg_cfg_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init regcfgs info failed!\n", vi_pipe);
        goto fail4;
    }

    ret = isp_stitch_sync_init_set(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        goto fail4;
    }

    /* init isp be cfgs all buffer */
    ret = isp_all_cfgs_be_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init becfgs buffer failed!\n", vi_pipe);
        goto fail4;
    }

    /* init isp global variables */
    mutex_init_lock(isp_ctx->lock);
    mutex_lock(isp_ctx->lock);

    ret = isp_init_global_info(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        mutex_unlock(isp_ctx->lock);
        goto fail5;
    }

    mutex_unlock(isp_ctx->lock);

    return HI_SUCCESS;

fail5:
    mutex_destroy(isp_ctx->lock);
fail4:
    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        isp_sensor_exit(vi_pipe);
    }
fail3:
    isp_algs_exit(vi_pipe, isp_ctx->algs);
    isp_algs_unregister(vi_pipe);
fail2:
    isp_reg_cfg_exit(vi_pipe);
fail1:
    isp_alg_buf_exit(vi_pipe);
    return HI_ERR_ISP_NOT_INIT;
}

static hi_s32 isp_switch_mode_update_ctx(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32  ret;

    /* WDR attribute */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_WDR_ATTR, &isp_ctx->wdr_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] update WDR attr failed\n", vi_pipe);
        return ret;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_PIPE_SIZE, &isp_ctx->pipe_size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] update pipe size failed\n", vi_pipe);
        return ret;
    }

    /* p2en info */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_P2EN_INFO_GET, &isp_ctx->isp0_p2_en);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] update p2en info failed\n", vi_pipe);
        return ret;
    }

    if (isp_ctx->change_wdr_mode == HI_TRUE) {
        ret = isp_stt_addr_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] init stt address failed!\n", vi_pipe);
            return ret;
        }
    }

    ret = isp_block_update(vi_pipe, &isp_ctx->block_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] update ISP block attr failed !\n", vi_pipe);
        return ret;
    }

    isp_ctx->para_rec.stitch_sync = HI_FALSE;
    ret = ioctl(isp_get_fd(vi_pipe), ISP_SYNC_INIT_SET, &isp_ctx->para_rec.stitch_sync);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set stitch sync failed!\n", vi_pipe);
    }

    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        ioctl(isp_get_fd(vi_pipe), ISP_SYNC_STITCH_PARAM_INIT);
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_RES_SWITCH_SET);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] config res switch failed with!\n", vi_pipe);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_void isp_switch_mode_update_finish_flag(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    isp_ctx->pre_sns_wdr_mode = isp_ctx->sns_wdr_mode;

    if (isp_ctx->change_wdr_mode == HI_TRUE) {
        hi_ext_top_wdr_switch_write(vi_pipe, HI_TRUE);
    }

    if ((isp_ctx->change_isp_res == HI_TRUE) || (isp_ctx->change_image_mode == HI_TRUE)) {
        hi_ext_top_res_switch_write(vi_pipe, HI_TRUE);
    }
}

static hi_void isp_algs_ctrl_process(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, hi_isp_cmos_black_level *sns_black_level)
{
    hi_isp_cmos_sensor_image_mode  isp_image_mode;
    isp_image_mode.width  = hi_ext_sync_total_width_read(vi_pipe);
    isp_image_mode.height = hi_ext_sync_total_height_read(vi_pipe);
    isp_image_mode.fps    = isp_ctx->sns_image_mode.fps;
    isp_image_mode.sns_mode = isp_ctx->sns_image_mode.sns_mode;

    isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_AE_BLC_SET, (hi_void *)&sns_black_level->black_level[1]);

    /* init the common part of extern registers and real registers */
    isp_ext_regs_initialize(vi_pipe);

    if (isp_ctx->change_wdr_mode == HI_TRUE) {
        isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_WDR_MODE_SET, (hi_void *)&isp_ctx->sns_wdr_mode);
    }

    if ((isp_ctx->change_isp_res == HI_TRUE) || (isp_ctx->change_image_mode == HI_TRUE)) {
        isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_CHANGE_IMAGE_MODE_SET, (hi_void *)&isp_image_mode);
    }
    isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_AE_FPS_BASE_SET, (hi_void *)&isp_ctx->sns_image_mode.fps);
}

static hi_s32 isp_switch_mode_init_sensor(hi_vi_pipe vi_pipe)
{
    hi_s32  ret   = HI_SUCCESS;
    isp_usr_ctx              *isp_ctx        = HI_NULL;
    hi_isp_cmos_black_level  *sns_black_level = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);

    /* 1. if switch wdr mode, should disable festt enable first */
    if (isp_ctx->change_wdr_mode == HI_TRUE) {
        ret = isp_reset_fe_stt_en(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] reset fe_stt enable failed\n", vi_pipe);
            return ret;
        }
    }

    /* 2.update info: WDR, HDR, stitch, blockAttr. etc */
    ret = isp_switch_mode_update_ctx(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_reg_cfg_ctrl(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_get_be_last_buf(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get be last bufs failed %x!\n", vi_pipe, ret);
        return ret;
    }

    isp_sensor_update_default(vi_pipe);
    isp_sensor_update_blc(vi_pipe);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    isp_algs_ctrl_process(vi_pipe, isp_ctx, sns_black_level);

    ret = isp_wdr_cfg_set(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] isp_wdr_cfg_set error \n", vi_pipe);
        return ret;
    }

    ret = isp_switch_reg_set(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set reg config failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    isp_sensor_update_sns_reg(vi_pipe);
    ret = isp_sensor_switch(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] init sensor failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    isp_switch_mode_update_finish_flag(vi_pipe, isp_ctx);

    return HI_SUCCESS;
}

static hi_s32 isp_switch_res_update_ctx(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;

    ret = isp_block_update(vi_pipe, &isp_ctx->block_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] update ISP block attr failed !\n", vi_pipe);
        return ret;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_PIPE_SIZE, &isp_ctx->pipe_size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get pipe size failed\n", vi_pipe);
        return ret;
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_P2EN_INFO_GET, &isp_ctx->isp0_p2_en);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get p2en info failed\n", vi_pipe);
        return ret;
    }

    isp_ctx->para_rec.stitch_sync = HI_FALSE;
    ret = ioctl(isp_get_fd(vi_pipe), ISP_SYNC_INIT_SET, &isp_ctx->para_rec.stitch_sync);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set isp stitch sync failed!\n", vi_pipe);
    }

    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        ioctl(isp_get_fd(vi_pipe), ISP_SYNC_STITCH_PARAM_INIT);
    }

    ret = ioctl(isp_get_fd(vi_pipe), ISP_RES_SWITCH_SET);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] config ISP res switch failed with ec %#x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_switch_res(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_u8  wdr_mode;
    hi_s32 ret;
    hi_u32 fps_value;
    hi_void *value = HI_NULL;
    hi_isp_cmos_sensor_image_mode  isp_image_mode;

    wdr_mode  = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    fps_value = hi_ext_system_fps_base_read(vi_pipe);
    value     = (hi_void *)&fps_value;

    isp_image_mode.width  = hi_ext_sync_total_width_read(vi_pipe);
    isp_image_mode.height = hi_ext_sync_total_height_read(vi_pipe);
    isp_image_mode.fps    = *(hi_float *)value;
    isp_image_mode.sns_mode = hi_ext_system_sensor_mode_read(vi_pipe);

    ret = isp_switch_res_update_ctx(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_reg_cfg_ctrl(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_get_be_last_buf(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_sensor_update_default(vi_pipe);
    isp_sensor_update_blc(vi_pipe);

    isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_CHANGE_IMAGE_MODE_SET, (hi_void *)&isp_image_mode);
    isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_AE_FPS_BASE_SET, (hi_void *)&isp_ctx->sns_image_mode.fps);

    if (is_fs_wdr_mode(wdr_mode)) {
        ret = isp_wdr_cfg_set(vi_pipe);
        if (ret != HI_SUCCESS) {
            return ret;
        }
    }

    ret = isp_switch_reg_set(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] set reg config failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    isp_sensor_update_sns_reg(vi_pipe);
    ret = isp_sensor_switch(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe:%d init sensor failed!\n", vi_pipe);
        return ret;
    }

    hi_ext_top_res_switch_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

static hi_void isp_switch_mode_update_sns_info(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_isp_cmos_default  *sns_dft = HI_NULL;
    hi_isp_sns_regs_info *sns_regs_info = HI_NULL;
    isp_sensor_get_default(vi_pipe, &sns_dft);

    isp_ctx->frame_info_ctrl.isp_frame->sensor_id   = sns_dft->sensor_mode.sensor_id;
    isp_ctx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;

    isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);
    isp_switch_state_set(vi_pipe);

    memcpy_s(&isp_ctx->dng_info_ctrl.isp_dng->dng_raw_format, sizeof(hi_isp_dng_raw_format),
             &sns_dft->sensor_mode.dng_raw_format, sizeof(hi_isp_dng_raw_format));

    if ((sns_regs_info->cfg2_valid_delay_max > CFG2VLD_DLY_LIMIT) || (sns_regs_info->cfg2_valid_delay_max < 1)) {
        isp_err_trace("ISP[%d] delay of config to invalid is:0x%x\n", vi_pipe, sns_regs_info->cfg2_valid_delay_max);
        sns_regs_info->cfg2_valid_delay_max = 1;
    }

    isp_ctx->linkage.cfg2valid_delay_max = sns_regs_info->cfg2_valid_delay_max;
}

static hi_s32 isp_switch_mode_process(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, hi_u8 wdr_mode)
{
    hi_s32  ret;
    hi_u8 hdr_enable;
    hi_u8   hdr_mode;
    hi_isp_cmos_sensor_image_mode sns_image_mode;

    /* HDR attribute */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_HDR_ATTR, &isp_ctx->hdr_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] get HDR attr failed\n", vi_pipe);
        return ret;
    }

    if (isp_ctx->change_wdr_mode == HI_TRUE) {
        hdr_enable = (isp_ctx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) ? 1 : 0;
        hdr_mode  = (((hdr_enable) & 0x1) << 0x6);
        hdr_mode  = hdr_mode | hi_ext_system_sensor_wdr_mode_read(vi_pipe);

        ret = isp_sensor_set_wdr_mode(vi_pipe, hdr_mode);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] set sensor wdr mode failed\n", vi_pipe);
            return ret;
        }
    }

    sns_image_mode.width  = isp_ctx->sns_image_mode.width;
    sns_image_mode.height = isp_ctx->sns_image_mode.height;
    sns_image_mode.fps    = isp_ctx->sns_image_mode.fps;
    sns_image_mode.sns_mode = isp_ctx->sns_image_mode.sns_mode;

    memcpy_s(&isp_ctx->pre_sns_image_mode, sizeof(isp_sensor_image_mode),
             &isp_ctx->sns_image_mode, sizeof(isp_sensor_image_mode));

    ret = isp_sensor_set_image_mode(vi_pipe, &sns_image_mode);
    if (ret == HI_SUCCESS) {  /* need to init sensor */
        isp_ctx->sns_wdr_mode = wdr_mode;
        isp_ctx->special_opt.fe_stt_update = HI_TRUE; /* used for fe statistics */

        ret = isp_switch_mode_init_sensor(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] switch mode failed!\n", vi_pipe);
            return ret;
        }
    } else {
        if (isp_ctx->change_isp_res == HI_TRUE) {
            isp_switch_res(vi_pipe, isp_ctx);
        }
        isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_AE_FPS_BASE_SET, (hi_void *)&isp_ctx->sns_image_mode.fps);
        hi_ext_top_res_switch_write(vi_pipe, HI_TRUE);
    }

    isp_switch_mode_update_sns_info(vi_pipe, isp_ctx);

    return HI_SUCCESS;
}

static hi_bool isp_stitch_main_pipe_switch_finished(isp_usr_ctx *isp_ctx)
{
    hi_s8 stitch_main_pipe;

    if ((isp_ctx->stitch_attr.stitch_enable == HI_TRUE) && (isp_ctx->stitch_attr.main_pipe == HI_FALSE)) {
        stitch_main_pipe = isp_ctx->stitch_attr.stitch_bind_id[0];

        if ((isp_ctx->change_wdr_mode == HI_TRUE) && hi_ext_top_wdr_switch_read(stitch_main_pipe) != HI_TRUE) {
            return HI_FALSE;
        }

        if (((isp_ctx->change_image_mode == HI_TRUE) || (isp_ctx->change_isp_res == HI_TRUE)) &&
            hi_ext_top_res_switch_read(stitch_main_pipe) != HI_TRUE) {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

hi_s32 isp_switch_mode(hi_vi_pipe vi_pipe)
{
    hi_u8   wdr_mode;
    hi_u32  fps_value;
    hi_void   *value    = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);

    wdr_mode  = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    fps_value = hi_ext_system_fps_base_read(vi_pipe);
    value     = (hi_void *)&fps_value;

    isp_ctx->sns_image_mode.fps      = *(hi_float *)value;
    isp_ctx->sns_image_mode.width    = hi_ext_top_sensor_width_read(vi_pipe);
    isp_ctx->sns_image_mode.height   = hi_ext_top_sensor_height_read(vi_pipe);
    isp_ctx->sns_image_mode.sns_mode = hi_ext_system_sensor_mode_read(vi_pipe);
    isp_ctx->sys_rect.x = hi_ext_system_corp_pos_x_read(vi_pipe);
    isp_ctx->sys_rect.y = hi_ext_system_corp_pos_y_read(vi_pipe);

    if (isp_ctx->isp_image_mode_flag == 0) {
        isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_AE_FPS_BASE_SET, (hi_void *)&isp_ctx->sns_image_mode.fps);

        memcpy_s(&isp_ctx->pre_sns_image_mode, sizeof(isp_sensor_image_mode),
                 &isp_ctx->sns_image_mode, sizeof(isp_sensor_image_mode));

        isp_ctx->isp_image_mode_flag = 1;
    } else {
        if (isp_ctx->sns_wdr_mode != wdr_mode) {
            isp_ctx->change_wdr_mode = HI_TRUE;
        }

        if ((isp_ctx->sns_image_mode.width  != isp_ctx->pre_sns_image_mode.width) ||
            (isp_ctx->sns_image_mode.height != isp_ctx->pre_sns_image_mode.height) ||
            (isp_ctx->sns_image_mode.fps    != isp_ctx->pre_sns_image_mode.fps) ||
            (isp_ctx->sns_image_mode.sns_mode != isp_ctx->pre_sns_image_mode.sns_mode)) {
            isp_ctx->change_image_mode = HI_TRUE;
        }

        if (isp_ctx->sys_rect.width  != hi_ext_sync_total_width_read(vi_pipe) ||
            isp_ctx->sys_rect.height != hi_ext_sync_total_height_read(vi_pipe)) {
            isp_ctx->sys_rect.width  = hi_ext_sync_total_width_read(vi_pipe);
            isp_ctx->sys_rect.height = hi_ext_sync_total_height_read(vi_pipe);

            isp_ctx->change_isp_res = HI_TRUE;
        }

        if (isp_ctx->change_wdr_mode || isp_ctx->change_image_mode || isp_ctx->change_isp_res) {
            /* stitch mode, should switch main pipe first */
            if (isp_stitch_main_pipe_switch_finished(isp_ctx) != HI_TRUE) {
                return HI_SUCCESS;
            }

            isp_switch_mode_process(vi_pipe, isp_ctx, wdr_mode);
        }

        isp_ctx->change_wdr_mode   = HI_FALSE;
        isp_ctx->change_image_mode = HI_FALSE;
        isp_ctx->change_isp_res    = HI_FALSE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_update_pos_get(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_s32 ret;

    isp_get_ctx(vi_pipe, isp_ctx);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_UPDATE_POS_GET, &isp_ctx->linkage.update_pos);
    if (ret) {
        isp_err_trace("pipe:%d get update pos %x!\n", vi_pipe, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_u32 isp_frame_cnt_get(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_s32 ret;
    hi_u32 frm_cnt = 0;
    isp_get_ctx(vi_pipe, isp_ctx);

    ret = ioctl(isp_get_fd(vi_pipe), ISP_FRAME_CNT_GET, &frm_cnt);
    if (ret) {
        isp_err_trace("pipe:%d get update pos %x!\n", vi_pipe, ret);
        return isp_ctx->linkage.iso_done_frm_cnt;
    }

    return frm_cnt;
}

hi_s32 isp_snap_attr_get(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    isp_snap_attr snap_attr;
    isp_working_mode isp_work_mode;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_SNAP_ATTR_GET, &snap_attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }
    memcpy_s(&isp_ctx->pro_param, sizeof(isp_pro_param), &snap_attr.pro_param, sizeof(isp_pro_param));
    isp_ctx->linkage.snap_type = snap_attr.snap_type;
    if ((snap_attr.picture_pipe_id == snap_attr.preview_pipe_id) && (snap_attr.preview_pipe_id != -1)) {
        isp_ctx->linkage.snap_pipe_mode = ISP_SNAP_PREVIEW_PICTURE;
    } else if (snap_attr.picture_pipe_id == vi_pipe) {
        isp_ctx->linkage.snap_pipe_mode = ISP_SNAP_PICTURE;
    } else if (snap_attr.preview_pipe_id == vi_pipe) {
        isp_ctx->linkage.snap_pipe_mode = ISP_SNAP_PREVIEW;
    } else {
        isp_ctx->linkage.snap_pipe_mode = ISP_SNAP_NONE;
    }
    isp_ctx->linkage.load_ccm = snap_attr.load_ccm;
    isp_ctx->linkage.picture_pipe_id = snap_attr.picture_pipe_id;
    isp_ctx->linkage.preview_pipe_id = snap_attr.preview_pipe_id;

    if (snap_attr.picture_pipe_id != -1) {
        ret = ioctl(isp_get_fd(snap_attr.picture_pipe_id), ISP_WORK_MODE_GET, &isp_work_mode);
        if (ret) {
            isp_err_trace("get isp work mode failed!\n");
            return ret;
        }
        isp_ctx->linkage.picture_running_mode = isp_work_mode.running_mode;
    } else {
        isp_ctx->linkage.picture_running_mode = ISP_MODE_RUNNING_BUTT;
    }

    if (snap_attr.picture_pipe_id == snap_attr.preview_pipe_id) {
        isp_ctx->linkage.preview_running_mode = isp_ctx->linkage.picture_running_mode;
    } else {
        if (snap_attr.preview_pipe_id != -1) {
            ret = ioctl(isp_get_fd(snap_attr.preview_pipe_id), ISP_WORK_MODE_GET, &isp_work_mode);
            if (ret) {
                isp_err_trace("get isp work mode failed!\n");
                return ret;
            }
            isp_ctx->linkage.preview_running_mode = isp_work_mode.running_mode;
        } else {
            isp_ctx->linkage.preview_running_mode = ISP_MODE_RUNNING_BUTT;
        }
    }

    return HI_SUCCESS;
}

hi_u8 isp_iso_index_cal(hi_vi_pipe vi_pipe, hi_u32 ae_done_frm_cnt)
{
    hi_u8  index;
    hi_u32 cnt_diff;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    index = isp_ctx->linkage.cfg2valid_delay_max;

    if (isp_ctx->linkage.iso_done_frm_cnt > ae_done_frm_cnt) {  /* preview pipe the last frame dont run complete. */
        cnt_diff = isp_ctx->linkage.iso_done_frm_cnt - ae_done_frm_cnt;
        index = index - cnt_diff;
    } else if (isp_ctx->linkage.iso_done_frm_cnt < ae_done_frm_cnt) { /* the preview pipe run first. */
        index = index + (ae_done_frm_cnt - isp_ctx->linkage.iso_done_frm_cnt - 1);
    } else if (isp_ctx->linkage.iso_done_frm_cnt == ae_done_frm_cnt) { /* the picture pipe run first. */
        index = index - 1;
    }

    if (isp_ctx->linkage.update_pos > 0) {
        index = index - 1;
    }

    index = MIN2(index, ISP_SYNC_ISO_BUF_MAX - 1);

    return index;
}

hi_u8 isp_ai_wb_index_cal(hi_vi_pipe vi_pipe, hi_u32 ae_done_frm_cnt)
{
    hi_u8  index;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->linkage.iso_done_frm_cnt < ae_done_frm_cnt) {
        index = ae_done_frm_cnt - isp_ctx->linkage.iso_done_frm_cnt - 1;
    } else  { /* the ai pipe run first. */
        index = 0;
    }

    index = MIN2(index, ISP_SYNC_ISO_BUF_MAX - 1);

    return index;
}

static hi_void isp_snap_picture_process(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_u8 i;

    isp_snap_pipe_mode snap_pipe_mode = isp_ctx->linkage.snap_pipe_mode;

    if (((snap_pipe_mode == ISP_SNAP_PICTURE) || (snap_pipe_mode == ISP_SNAP_PREVIEW_PICTURE)) &&
        (is_offline_mode(isp_ctx->block_attr.running_mode) || is_striping_mode(isp_ctx->block_attr.running_mode))) {
        isp_snap_reg_cfg_get(vi_pipe, &isp_ctx->snap_isp_info);

        if (isp_ctx->snap_isp_info.snap_state == SNAP_STATE_CFG) {
            isp_ctx->linkage.snap_state = HI_TRUE; /* double pipe offlinemode, snap_pipe */
            isp_ctx->linkage.iso        = isp_ctx->snap_isp_info.isp_cfg_info.iso;
            isp_ctx->linkage.isp_dgain  = isp_ctx->snap_isp_info.isp_cfg_info.isp_dgain;
            isp_ctx->linkage.color_temp = isp_ctx->snap_isp_info.isp_cfg_info.color_temperature;
            isp_ctx->linkage.pro_index  = isp_ctx->snap_isp_info.pro_index;
            isp_ctx->linkage.sensor_iso = ((hi_u64)isp_ctx->linkage.iso << 0x8) /
                                          div_0_to_1(isp_ctx->linkage.isp_dgain);
            isp_ctx->linkage.sensor_iso = MAX2(isp_ctx->linkage.sensor_iso, 0x64);

            for (i = 0; i < ISP_WB_GAIN_NUM; i++) {
                isp_ctx->linkage.white_balance_gain[i] =
                    isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain[i] << 0x8;
            }
        } else {
            isp_ctx->linkage.snap_state = HI_FALSE;
            isp_ctx->linkage.pro_index = 0;
        }
    }
}

static hi_void isp_snap_preview_process(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_u8 i;
    hi_snap_type snap_type = isp_ctx->linkage.snap_type;
    isp_snap_pipe_mode snap_pipe_mode = isp_ctx->linkage.snap_pipe_mode;

    if ((snap_pipe_mode == ISP_SNAP_PREVIEW) || (snap_pipe_mode == ISP_SNAP_PREVIEW_PICTURE)) {
        if (snap_type == SNAP_TYPE_PRO) {
            isp_ctx->linkage.pro_trigger = isp_pro_trigger_get(vi_pipe);
            if (isp_ctx->linkage.pro_trigger == HI_TRUE) {
                isp_algs_ctrl(isp_ctx->algs, vi_pipe, ISP_PROTRIGGER_SET, (hi_void *)&isp_ctx->pro_param);
            }
        }

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            isp_ctx->linkage.pro_index_buf[i] = isp_ctx->linkage.pro_index_buf[i - 1];
        }
        if ((isp_ctx->linkage.pro_trigger == HI_TRUE) ||
            ((isp_ctx->pro_frm_cnt > 0) && (isp_ctx->pro_frm_cnt < isp_ctx->pro_param.pro_frame_num))) {
            isp_ctx->linkage.pro_index_buf[0] = isp_ctx->pro_frm_cnt + 1;
        } else {
            isp_ctx->linkage.pro_index_buf[0] = 0;
        }
        isp_ctx->linkage.pro_index = isp_ctx->linkage.pro_index_buf[isp_ctx->linkage.cfg2valid_delay_max];
        if (isp_ctx->linkage.update_pos > 0) {
            isp_ctx->linkage.pro_index = isp_ctx->linkage.pro_index_buf[isp_ctx->linkage.cfg2valid_delay_max - 1];
        }
    }
}

hi_s32 isp_snap_pre_process(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_usr_ctx *preview_isp_ctx = HI_NULL;
    isp_snap_pipe_mode snap_pipe_mode;
    hi_u8 index;
    hi_u32 pre_pipe_frm_cnt = 0;

    isp_get_ctx(vi_pipe, isp_ctx);

    ret = isp_snap_attr_get(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    snap_pipe_mode = isp_ctx->linkage.snap_pipe_mode;

    if (snap_pipe_mode == ISP_SNAP_NONE) {
        return HI_SUCCESS;
    }

    if ((isp_ctx->linkage.preview_pipe_id > -1) && (isp_ctx->linkage.preview_pipe_id < ISP_MAX_PIPE_NUM)) {
        isp_get_ctx(isp_ctx->linkage.preview_pipe_id, preview_isp_ctx);
        pre_pipe_frm_cnt = preview_isp_ctx->linkage.iso_done_frm_cnt;
    }

    isp_snap_picture_process(vi_pipe, isp_ctx);
    isp_snap_preview_process(vi_pipe, isp_ctx);

    if (is_online_mode(isp_ctx->block_attr.running_mode) || is_sidebyside_mode(isp_ctx->block_attr.running_mode)) {
        if (snap_pipe_mode == ISP_SNAP_PICTURE) {
            index = isp_iso_index_cal(vi_pipe, pre_pipe_frm_cnt);
            if (preview_isp_ctx != HI_NULL) {
                isp_ctx->linkage.iso = preview_isp_ctx->linkage.sync_iso_buf[index];
                isp_ctx->linkage.pro_index = preview_isp_ctx->linkage.pro_index_buf[index];
            }
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_ai_process(hi_vi_pipe vi_pipe)
{
    hi_u8 index, i;
    hi_u32 base_pipe_frm_cnt = 0;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_usr_ctx *isp_base_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if ((isp_ctx->ai_info.pq_ai_en == HI_FALSE) ||
        ((isp_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == isp_ctx->ai_info.base_pipe_id))) {
        return HI_SUCCESS;
    }

    if (vi_pipe == isp_ctx->ai_info.ai_pipe_id) {
        isp_get_ctx(isp_ctx->ai_info.base_pipe_id, isp_base_ctx);
        base_pipe_frm_cnt = isp_base_ctx->linkage.iso_done_frm_cnt;
        index = isp_iso_index_cal(vi_pipe, base_pipe_frm_cnt);

        isp_ctx->linkage.iso        = isp_base_ctx->linkage.sync_iso_buf[index];
        isp_ctx->linkage.isp_dgain  = isp_base_ctx->linkage.sync_isp_dgain_buf[index];
        isp_ctx->linkage.sensor_iso = ((hi_u64)isp_ctx->linkage.iso << 0x8) / div_0_to_1(isp_ctx->linkage.isp_dgain);

        index = isp_ai_wb_index_cal(vi_pipe, base_pipe_frm_cnt);
        for (i = 0; i < ISP_WB_GAIN_NUM; i++) {
            isp_ctx->linkage.white_balance_gain[i] = isp_base_ctx->linkage.sync_wb_gain[index][i];
        }

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            isp_ctx->linkage.ccm[i] = isp_base_ctx->linkage.sync_ccm[index][i];
        }

        isp_ctx->linkage.color_temp = isp_base_ctx->linkage.sync_color_temp[index];
    }

    return HI_SUCCESS;
}

hi_s32 isp_save_snap_info(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_config_info snap_info;
    hi_u32 i;

    isp_get_ctx(vi_pipe, isp_ctx);

    snap_info.color_temperature = isp_ctx->linkage.color_temp;
    snap_info.iso               = isp_ctx->linkage.sync_iso_buf[0];
    snap_info.isp_dgain         = isp_ctx->linkage.isp_dgain;
    snap_info.exposure_time     = isp_ctx->linkage.int_time;
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        snap_info.white_balance_gain[i] = (isp_ctx->linkage.white_balance_gain[i] + 128) >> 8; /* const value 128&8 */
    }

    for (i = 0; i < ISP_CAP_CCM_NUM; i++) {
        snap_info.cap_ccm[i] = isp_ctx->linkage.ccm[i];
    }

    ret = isp_snap_reg_cfg_set(vi_pipe, &snap_info);

    return ret;
}

HI_S32 isp_set_smart_info(VI_PIPE vi_pipe, const hi_isp_smart_info *smart_info)
{
    HI_U32 i;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(smart_info);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        hi_ext_system_smart_enable_write(vi_pipe, i, smart_info->roi[i].enable);
        hi_ext_system_smart_available_write(vi_pipe, i, smart_info->roi[i].available);
        hi_ext_system_smart_luma_write(vi_pipe, i, smart_info->roi[i].luma);
    }

    hi_ext_system_smart_update_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 isp_get_smart_info(VI_PIPE vi_pipe, hi_isp_smart_info *smart_info)
{
    HI_U32 i;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(smart_info);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        smart_info->roi[i].enable     = hi_ext_system_smart_enable_read(vi_pipe, i);
        smart_info->roi[i].available  = hi_ext_system_smart_available_read(vi_pipe, i);
        smart_info->roi[i].luma       = hi_ext_system_smart_luma_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_pro_calc_done(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;

    ret = ioctl(isp_get_fd(vi_pipe), ISP_SET_PROCALCDONE);
    if (ret) {
        isp_err_trace("set isp pro calculate done failed!\n");
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 isp_stitch_sync_exit(hi_vi_pipe vi_pipe)
{
    hi_u32  k;
    hi_vi_pipe vi_pipe_s;
    isp_usr_ctx *isp_ctx = HI_NULL;

    /* 1. check status */
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);

    if ((isp_ctx->stitch_attr.stitch_enable == HI_TRUE) && (isp_ctx->stitch_attr.main_pipe != HI_TRUE)) {
        for (k = 0; k < isp_ctx->stitch_attr.stitch_pipe_num; k++) {
            vi_pipe_s = isp_ctx->stitch_attr.stitch_bind_id[k];
            isp_get_ctx(vi_pipe_s, isp_ctx);

            while ((isp_ctx->stitch_attr.main_pipe == HI_TRUE) && (isp_ctx->mem_init != HI_FALSE)) {
                usleep(10); /* sleep 10us */
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_stitch_sync_run(hi_vi_pipe vi_pipe)
{
    hi_u8 k;
    hi_vi_pipe vi_pipe_id;
    isp_usr_ctx *isp_ctx   = HI_NULL;
    isp_usr_ctx *isp_ctx_s = HI_NULL;

    /* 1. check status */
    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);

    if (isp_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        return HI_SUCCESS;
    }

    if (isp_ctx->stitch_attr.main_pipe == HI_TRUE) {
        for (k = 0; k < isp_ctx->stitch_attr.stitch_pipe_num; k++) {
            vi_pipe_id = isp_ctx->stitch_attr.stitch_bind_id[k];
            isp_get_ctx(vi_pipe_id, isp_ctx_s);

            if (isp_ctx_s->mem_init != HI_TRUE) {
                return HI_FAILURE;
            }

            if (isp_ctx_s->para_rec.init != HI_TRUE) {
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_stitch_iso_sync(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_usr_ctx *main_isp_ctx = HI_NULL;
    hi_u8 index;
    hi_u32 main_pipe_frm_cnt;
    hi_vi_pipe main_pipe;
    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        if (isp_ctx->stitch_attr.main_pipe != HI_TRUE) {
            main_pipe = isp_ctx->stitch_attr.stitch_bind_id[0];
            isp_check_pipe_return(main_pipe);
            isp_get_ctx(vi_pipe, main_isp_ctx);

            main_pipe_frm_cnt = main_isp_ctx->linkage.iso_done_frm_cnt;
            index = isp_iso_index_cal(vi_pipe, main_pipe_frm_cnt);
            isp_ctx->linkage.iso = main_isp_ctx->linkage.sync_iso_buf[index];
        }
    }

    return HI_SUCCESS;
}

static hi_s32 isp_statistics_get(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, hi_void **stat)
{
    hi_s32 ret;
    ret = isp_statistics_get_buf(vi_pipe, stat);
    if (ret != HI_SUCCESS) {
        if (isp_ctx->frame_cnt != 0) {
            return ret;
        }

        isp_ctx->linkage.stat_ready  = HI_FALSE;
    } else {
        isp_ctx->linkage.stat_ready  = HI_TRUE;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_update_trans_info(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    if (isp_ctx->linkage.snap_pipe_mode != ISP_SNAP_PICTURE) {
        ret = isp_update_info(vi_pipe);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        ret = isp_update_frame_info(vi_pipe);
        if (ret != HI_SUCCESS) {
            return ret;
        }
    }

    ret = isp_update_attach_info(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_update_color_gamut_info(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

static hi_void isp_update_pro_frm_cnt(isp_usr_ctx *isp_ctx)
{
    if (((isp_ctx->pro_frm_cnt > 0) &&
         (isp_ctx->pro_frm_cnt < isp_ctx->pro_param.pro_frame_num + 4)) || /* const value 4 */
        (isp_ctx->linkage.pro_trigger == HI_TRUE)) {
        isp_ctx->pro_frm_cnt++;
    } else {
        isp_ctx->pro_frm_cnt = 0;
    }
}

static hi_s32 isp_sync_cfg_config(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    if ((isp_ctx->frame_cnt % div_0_to_1(isp_ctx->linkage.ae_run_interval) == 0) || (isp_ctx->pro_frm_cnt > 0)) {
        if (!isp_ctx->linkage.defect_pixel) {
            ret = isp_sync_cfg_set(vi_pipe);
            if (ret != HI_SUCCESS) {
                return HI_SUCCESS;
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_alg_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    ret = isp_ldci_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail00;
    }

    ret = isp_cfg_be_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail01;
    }

    ret = isp_get_be_buf_first(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail02;
    }

    ret = isp_be_vreg_addr_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail02;
    }

    ret = isp_stt_buf_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail02;
    }

    ret = isp_stt_addr_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail03;
    }

    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        ret = isp_clut_buf_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            goto fail03;
        }
    }

    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        ret = isp_spec_awb_buf_init(vi_pipe);
        if (ret != HI_SUCCESS) {
            goto fail04;
        }
    }

    /* init statistics bufs. */
    ret = isp_statistics_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail05;
    }

    /* init proc bufs. */
    ret = isp_proc_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail06;
    }

    /* init trans info bufs. */
    ret = isp_trans_info_init(vi_pipe, &isp_ctx->isp_trans_info);
    if (ret != HI_SUCCESS) {
        goto fail07;
    }

    ret = isp_update_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail08;
    }

    ret = isp_frame_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail09;
    }

    ret = isp_attach_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail10;
    }

    ret = isp_color_gamut_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail11;
    }

    ret = isp_dng_info_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail12;
    }

    ret = isp_pro_info_init(vi_pipe, &isp_ctx->isp_pro_info);
    if (ret != HI_SUCCESS) {
        goto fail13;
    }

    ret = isp_pro_nr_param_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail14;
    }

    ret = isp_pro_shp_param_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        goto fail15;
    }

    return HI_SUCCESS;

fail15:
    isp_pro_nr_param_exit(vi_pipe);
fail14:
    isp_pro_info_exit(vi_pipe);
fail13:
    isp_dng_info_exit(vi_pipe);
fail12:
    isp_color_gamut_info_exit(vi_pipe);
fail11:
    isp_attach_info_exit(vi_pipe);
fail10:
    isp_frame_info_exit(vi_pipe);
fail09:
    isp_update_info_exit(vi_pipe);
fail08:
    isp_trans_info_exit(vi_pipe);
fail07:
    isp_proc_exit(vi_pipe);
fail06:
    isp_statistics_exit(vi_pipe);
fail05:
    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        isp_spec_awb_buf_exit(vi_pipe);
    }
fail04:
    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        isp_clut_buf_exit(vi_pipe);
    }
fail03:
    isp_stt_buf_exit(vi_pipe);
fail02:
    isp_cfg_be_buf_exit(vi_pipe);
fail01:
    isp_ldci_buf_exit(vi_pipe);
fail00:
    return HI_ERR_ISP_NOT_INIT;
}

hi_void isp_alg_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    isp_pro_shp_param_exit(vi_pipe);
    isp_pro_nr_param_exit(vi_pipe);
    isp_pro_info_exit(vi_pipe);
    isp_dng_info_exit(vi_pipe);
    isp_color_gamut_info_exit(vi_pipe);
    isp_attach_info_exit(vi_pipe);
    isp_frame_info_exit(vi_pipe);
    isp_update_info_exit(vi_pipe);
    isp_trans_info_exit(vi_pipe);
    isp_proc_exit(vi_pipe);
    isp_statistics_exit(vi_pipe);
    if (isp_ctx->isp_yuv_mode != HI_TRUE) {
        isp_spec_awb_buf_exit(vi_pipe);
        isp_clut_buf_exit(vi_pipe);
    }
    isp_stt_buf_exit(vi_pipe);
    isp_cfg_be_buf_exit(vi_pipe);
    isp_ldci_buf_exit(vi_pipe);
}

hi_s32 isp_run(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_void *stat = HI_NULL;
    hi_void *reg_cfg = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_get_ctx(vi_pipe, isp_ctx);

    ret = isp_stitch_sync_run(vi_pipe);
    if (ret != HI_SUCCESS) {
        return HI_SUCCESS;
    }

    /*  get isp be_buf info. */
    ret = isp_get_be_free_buf(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    /*  get statistics buf info. */
    ret = isp_statistics_get(vi_pipe, isp_ctx, &stat);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_set_cfg_be_buf_state(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    /* get regcfg */
    isp_get_reg_cfg_ctx(vi_pipe, &reg_cfg);

    isp_read_ext_regs(vi_pipe);

    if (isp_ctx->freeze_fw) {
        isp_reg_cfg_info_set(vi_pipe);
        return HI_SUCCESS;
    }

    ret = isp_update_pos_get(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_ai_process(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_snap_pre_process(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_stitch_iso_sync(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_ctx->frame_cnt++;

    isp_dbg_run_bgn(&isp_ctx->isp_dbg, isp_ctx->frame_cnt);

    isp_algs_run(isp_ctx->algs, vi_pipe, stat, reg_cfg, 0);

    isp_proc_write(isp_ctx->algs, vi_pipe);

    /* update info */
    ret = isp_update_trans_info(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_dbg_run_end(&isp_ctx->isp_dbg, isp_ctx->frame_cnt);

    /* release statistics buf info. */
    if (isp_ctx->linkage.stat_ready  == HI_TRUE) {
        isp_statistics_put_buf(vi_pipe);
    }

    /* record the register config infomation to fhy and kernel,and be valid in next frame. */
    ret = isp_reg_cfg_info_set(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_update_pro_frm_cnt(isp_ctx);
    ret = isp_sync_cfg_config(vi_pipe, isp_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }
    /* isp_update_ext_regs */
    /* save capture info */
    isp_save_snap_info(vi_pipe);

    isp_update_dng_image_dynamic_info(vi_pipe);

    /* pro snap mode, ae calculate done */
    if (isp_ctx->pro_frm_cnt == 1) {
        isp_set_pro_calc_done(vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 isp_run_thread_proc(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u32 int_status = 0;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    mutex_lock(isp_ctx->lock);

    if (isp_ctx->para_rec.run_en == HI_FALSE) {
        mutex_unlock(isp_ctx->lock);
        return HI_FAILURE;
    }

    /* change image mode (WDR mode or resolution) */
    ret = isp_switch_mode(vi_pipe);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] switch mode failed!\n", vi_pipe);
        mutex_unlock(isp_ctx->lock);
        return ret;
    }

    mutex_unlock(isp_ctx->lock);

    /* waked up by the interrupt */
    ret = ioctl(isp_get_fd(vi_pipe), ISP_GET_FRAME_EDGE, &int_status);
    if (ret == HI_SUCCESS) {
        /* isp firmware calculate, include AE/AWB, etc. */
        if (ISP_1ST_INT & int_status) {
            mutex_lock(isp_ctx->lock);

            if (isp_ctx->para_rec.run_en == HI_FALSE) {
                mutex_unlock(isp_ctx->lock);
                return HI_FAILURE;
            }

            isp_run(vi_pipe);

            mutex_unlock(isp_ctx->lock);
        }
    }

    return HI_SUCCESS;
}

static hi_void isp_global_ctx_reset(isp_usr_ctx *isp_ctx)
{
    hi_u8  i;
    isp_ctx->mem_init                     = HI_FALSE;
    isp_ctx->para_rec.init                = HI_FALSE;
    isp_ctx->para_rec.stitch_sync         = HI_FALSE;
    isp_ctx->linkage.run_once             = HI_FALSE;
    isp_ctx->linkage.snap_pipe_mode       = ISP_SNAP_NONE;
    isp_ctx->linkage.preview_pipe_id      = -1;
    isp_ctx->linkage.picture_pipe_id      = -1;
    isp_ctx->linkage.preview_running_mode = ISP_MODE_RUNNING_OFFLINE;
    isp_ctx->linkage.picture_running_mode = ISP_MODE_RUNNING_OFFLINE;
    isp_ctx->linkage.snap_type            = SNAP_TYPE_NORMAL;
    isp_ctx->linkage.snap_state           = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        isp_ctx->isp_be_virt_addr[i] = HI_NULL;
        isp_ctx->viproc_virt_addr[i] = HI_NULL;
    }
}

hi_s32 isp_exit(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    const hi_u64 handsignal = ISP_EXIT_HAND_SIGNAL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_check_pointer_return(isp_ctx);
    isp_check_open_return(vi_pipe);

    /* set handsignal */
    if (ioctl(isp_get_fd(vi_pipe), ISP_RUN_STATE_SET, &handsignal)) {
        isp_err_trace("ISP[%d] set run state failed!\n", vi_pipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    memset_s(&isp_ctx->para_rec, sizeof(isp_para_rec), 0, sizeof(isp_para_rec));

    hi_ext_top_wdr_cfg_write(vi_pipe, isp_ctx->para_rec.wdr_cfg);
    hi_ext_top_pub_attr_cfg_write(vi_pipe, isp_ctx->para_rec.pub_cfg);

    if (isp_ctx->isp_yuv_mode == HI_FALSE) {
        isp_sensor_exit(vi_pipe);
    }

    isp_algs_exit(vi_pipe, isp_ctx->algs);

    isp_algs_unregister(vi_pipe);

    isp_reg_cfg_exit(vi_pipe);
    isp_alg_buf_exit(vi_pipe);
    isp_block_exit(vi_pipe);
    /* exit global variables */
    isp_global_ctx_reset(isp_ctx);

    if (ioctl(isp_get_fd(vi_pipe), ISP_RESET_CTX)) {
        isp_err_trace("ISP[%d] reset ctx failed!\n", vi_pipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* release vregs */
    vreg_munmap(isp_fe_reg_base(vi_pipe), FE_REG_SIZE_ALIGN);
    vreg_munmap(isp_be_reg_base(vi_pipe), BE_REG_SIZE_ALIGN);
    vreg_exit(vi_pipe, isp_vir_reg_base(vi_pipe), ISP_VREG_SIZE);
    vreg_release_all(vi_pipe);

    return HI_SUCCESS;
}

