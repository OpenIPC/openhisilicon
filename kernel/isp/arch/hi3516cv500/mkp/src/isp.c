/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "hi_spi.h"
#include "hi_osal.h"
#include "hi_common.h"
#include "hi_comm_isp.h"
#include "isp_drv_defines.h"
#include "openipc_frame_ts.h"
#include "mkp_isp.h"
#include "isp.h"
#include "isp_drv.h"
#include "isp_list.h"
#include "isp_drv_vreg.h"
#include "mm_ext.h"
#include "proc_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"
#include "isp_ext.h"
#include "dev_ext.h"
#include "isp_reg_define.h"
#include "isp_stt_define.h"
#include "hi_i2c.h"

/* MACRO DEFINITION */
int  isp_irq_route(hi_vi_pipe vi_pipe);
int  isp_drv_int_bottom_half(int irq);

/*  GLOBAL VARIABLES */
static isp_drv_ctx              g_isp_drv_ctx[ISP_MAX_PIPE_NUM] = {{0}};

static osal_spinlock_t          g_isp_lock[ISP_MAX_PIPE_NUM];
static osal_spinlock_t          g_isp_sync_lock[ISP_MAX_PIPE_NUM];
/* ldci temporal filter enable */
static hi_bool                  g_ldci_tpr_flt_en[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_FALSE };

static isp_version    g_isp_lib_info = {{0}};

hi_u32                g_isp_fe_irq = ISP_FE_IRQ_NR;

/* ISP ModParam info */
hi_u32  g_pwm_number[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 3 }; /* num 3 */
hi_u32  g_update_pos[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 0 }; /* 0: frame start; 1: frame end */

/* The time(unit:ms) of interrupt timeout */
hi_u32  g_int_timeout[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 200 }; /* inter timeout 200 */

/* update isp statistic information per stat-intval frame, purpose to reduce CPU load */
hi_u32  g_stat_intvl[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 1 };

/* 0: close proc; n: write proc info's interval int num */
hi_u32  g_proc_param[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 30 }; /* interval num 30 */
hi_u32  g_port_int_delay[ISP_MAX_PIPE_NUM] = { 0 }; /* Port intertupt delay value */
hi_u8   g_be_buf_num[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 4 }; /* buf num 4 */
hi_bool g_int_bottom_half = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */
hi_bool g_quick_start = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */
hi_bool g_long_frm_int_en = HI_FALSE; /* 1 : enable long frame  pipe interrupt */
hi_bool g_use_bottom_half = HI_FALSE;  /* 1 to use interrupt processing at bottom half */
hi_u32  g_isp_exit_timeout = 2000;  /* The time(unit:ms) of exit be buffer timeout */
hi_u16  g_res_max_width[ISP_MAX_PIPE_NUM]  = { ISP_RES_WIDTH_MAX };
hi_u16  g_res_max_height[ISP_MAX_PIPE_NUM] = { ISP_RES_HEIGHT_MAX };

isp_drv_ctx *isp_drv_get_ctx(hi_vi_pipe vi_pipe)
{
    return &g_isp_drv_ctx[vi_pipe];
}

osal_spinlock_t *isp_drv_get_lock(hi_vi_pipe vi_pipe)
{
    return &g_isp_lock[vi_pipe];
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
osal_spinlock_t *isp_drv_get_sync_lock(hi_vi_pipe vi_pipe)
{
    return &g_isp_sync_lock[vi_pipe];
}
#endif

hi_bool isp_drv_get_ldci_tpr_flt_en(hi_vi_pipe vi_pipe)
{
    return g_ldci_tpr_flt_en[vi_pipe];
}


hi_void isp_drv_slave_pipe_int_enable(hi_vi_pipe vi_pipe, hi_bool en)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u8 k;
    hi_u8 chn_num_max;
    hi_vi_pipe vi_pipe_bind;
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (g_long_frm_int_en == HI_FALSE) {
        return ;
    }
    if (is_fs_wdr_mode(drv_ctx->sync_cfg.wdr_mode) == HI_FALSE) {
        return;
    }

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    if (en) {
        for (k = 0; k < chn_num_max; k++) {
            vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
            if (vi_pipe_bind == vi_pipe) {
                continue;
            }
            if (((vi_pipe_bind) < 0) || ((vi_pipe_bind) >= ISP_MAX_PIPE_NUM)) {
                continue;
            }
            io_rw_fe_address(vi_pipe_bind, ISP_INT_FE) = 0xff;
            io_rw_fe_address(vi_pipe_bind, ISP_INT_FE_MASK) |= ISP_INT_FE_FSTART;
            io_rw_fe_address(vi_pipe_bind, ISP_INT_FE_MASK) |= ISP_INT_FE_FEND;
        }
    } else {
        for (k = 0; k < chn_num_max; k++) {
            vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
            if (vi_pipe_bind == vi_pipe) {
                continue;
            }
            if (((vi_pipe_bind) < 0) || ((vi_pipe_bind) >= ISP_MAX_PIPE_NUM)) {
                continue;
            }
            io_rw_fe_address(vi_pipe_bind, ISP_INT_FE) = 0xff;
            io_rw_fe_address(vi_pipe_bind, ISP_INT_FE_MASK) &= ~ISP_INT_FE_FSTART;
            io_rw_fe_address(vi_pipe_bind, ISP_INT_FE_MASK) &= ~ISP_INT_FE_FEND;
        }
    }
}


hi_s32 isp_drv_set_int_enable(hi_vi_pipe vi_pipe, hi_bool en)
{
    hi_s32 vi_dev;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    vi_dev = drv_ctx->wdr_attr.vi_dev;

    isp_check_vir_pipe_return(vi_pipe);

    if (en) {
        if (is_full_wdr_mode(drv_ctx->wdr_cfg.wdr_mode) ||
            is_half_wdr_mode(drv_ctx->wdr_cfg.wdr_mode)) {
            io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK) |= VI_PT_INT_FSTART;
            io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_FSTART_DLY) = g_port_int_delay[vi_pipe];
        }
        io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK) |= VI_PT_INT_ERR;

        io_rw_fe_address(vi_pipe, ISP_INT_FE) = 0xFF;
        io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK) |= ISP_INT_FE_FSTART;
        io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK) |= ISP_INT_FE_FEND;
    } else {
        if (is_full_wdr_mode(drv_ctx->wdr_cfg.wdr_mode) ||
            is_half_wdr_mode(drv_ctx->wdr_cfg.wdr_mode)) {
            io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK) &= ~(VI_PT_INT_FSTART);
            io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_FSTART_DLY) = 0;
        }
        io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT) = 0xF;
        io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK) &= ~(VI_PT_INT_ERR);

        io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK) &= ~(ISP_INT_FE_FSTART);
        io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK) &= ~(ISP_INT_FE_FEND);
    }
    isp_drv_slave_pipe_int_enable(vi_pipe, en);
    return HI_SUCCESS;
}

hi_s32 isp_drv_wait_condition_callback(const hi_void *param)
{
    hi_bool condition;

    condition = *(hi_bool *)param;

    return (condition == HI_TRUE);
}

hi_s32 isp_drv_wait_exit_callback(const hi_void *param)
{
    hi_s32 condition;

    condition = *(hi_s32 *)param;

    return (condition == 0);
}

hi_s32 isp_get_frame_edge(hi_vi_pipe vi_pipe, hi_u32 *status)
{
    unsigned long flags;
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(status);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    ret = osal_wait_event_timeout_interruptible(&drv_ctx->isp_wait, isp_drv_wait_condition_callback,
                                                &drv_ctx->edge, g_int_timeout[vi_pipe]);
    if (ret <= 0) {
        *status = 0;
        isp_warn_trace("Get Interrupt timeout failed!\n");
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->edge = HI_FALSE;
    *status = drv_ctx->status;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

static int isp_get_vd_start_time_out(hi_vi_pipe vi_pipe, hi_u32 milli_sec, hi_u32 *pu32status)
{
    unsigned long flags;
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (osal_down_interruptible(&drv_ctx->isp_sem)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->vd_start = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (milli_sec != 0) {
        ret = osal_wait_event_timeout_interruptible(&drv_ctx->isp_wait_vd_start, isp_drv_wait_condition_callback,
                                                    &drv_ctx->vd_start, (milli_sec));
        if (ret <= 0) {
            osal_up(&drv_ctx->isp_sem);
            return HI_FAILURE;
        }
    } else {
        ret = osal_wait_event_interruptible(&drv_ctx->isp_wait_vd_start, isp_drv_wait_condition_callback,
                                            &drv_ctx->vd_start);
        if (ret) {
            osal_up(&drv_ctx->isp_sem);
            return HI_FAILURE;
        }
    }

    *pu32status = drv_ctx->status;

    osal_up(&drv_ctx->isp_sem);

    return HI_SUCCESS;
}

static int isp_get_vd_end_time_out(hi_vi_pipe vi_pipe, hi_u32 milli_sec, hi_u32 *pu32status)
{
    unsigned long flags;
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (osal_down_interruptible(&drv_ctx->isp_sem_vd)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->vd_end = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (milli_sec != 0) {
        ret = osal_wait_event_timeout_interruptible(&drv_ctx->isp_wait_vd_end, isp_drv_wait_condition_callback,
                                                    &drv_ctx->vd_end, (milli_sec));
        if (ret <= 0) {
            osal_up(&drv_ctx->isp_sem_vd);
            return HI_FAILURE;
        }
    } else {
        ret = osal_wait_event_interruptible(&drv_ctx->isp_wait_vd_end, isp_drv_wait_condition_callback,
                                            &drv_ctx->vd_end);
        if (ret) {
            osal_up(&drv_ctx->isp_sem_vd);
            return HI_FAILURE;
        }
    }

    *pu32status = drv_ctx->status;

    osal_up(&drv_ctx->isp_sem_vd);

    return HI_SUCCESS;
}

static int isp_get_vd_be_end_time_out(hi_vi_pipe vi_pipe, hi_u32 milli_sec, hi_u32 *pu32status)
{
    unsigned long flags;
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (osal_down_interruptible(&drv_ctx->isp_sem_be_vd)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->vd_be_end = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (milli_sec != 0) {
        ret = osal_wait_event_timeout_interruptible(&drv_ctx->isp_wait_vd_be_end, isp_drv_wait_condition_callback,
                                                    &drv_ctx->vd_be_end, (milli_sec));
        if (ret <= 0) {
            osal_up(&drv_ctx->isp_sem_be_vd);
            return HI_FAILURE;
        }
    } else {
        ret = osal_wait_event_interruptible(&drv_ctx->isp_wait_vd_be_end, isp_drv_wait_condition_callback,
                                            &drv_ctx->vd_be_end);
        if (ret) {
            osal_up(&drv_ctx->isp_sem_be_vd);
            return HI_FAILURE;
        }
    }

    *pu32status = 1;

    osal_up(&drv_ctx->isp_sem_be_vd);

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_split_attr(hi_vi_pipe vi_pipe, vi_pipe_split_attr *pipe_split_attr)
{
    if (!ckfn_vi_get_split_attr()) {
        return HI_FAILURE;
    }

    return call_vi_get_split_attr(vi_pipe, pipe_split_attr);
}

hi_s32 isp_drv_get_frame_interrupt_attr(hi_vi_pipe vi_pipe, hi_frame_interrupt_attr *frame_int_attr)
{
    if (!ckfn_vi_get_vi_frame_interrupt_attr()) {
        return HI_FAILURE;
    }

    return call_vi_get_vi_frame_interrupt_attr(vi_pipe, frame_int_attr);
}

hi_s32 isp_drv_get_running_mode(vi_pipe_split_attr *pipe_split_attr, isp_running_mode *running_mode)
{
    if ((pipe_split_attr->mode == VI_PARALLEL_VPSS_OFFLINE) ||
        (pipe_split_attr->mode == VI_PARALLEL_VPSS_PARALLEL)) {
        *running_mode = ISP_MODE_RUNNING_SIDEBYSIDE;
    } else if ((pipe_split_attr->mode == VI_ONLINE_VPSS_OFFLINE) ||
               (pipe_split_attr->mode == VI_ONLINE_VPSS_ONLINE)) {
        *running_mode = ISP_MODE_RUNNING_ONLINE;
    } else if ((pipe_split_attr->mode == VI_OFFLINE_VPSS_OFFLINE) ||
               (pipe_split_attr->mode == VI_OFFLINE_VPSS_ONLINE)) {
        if (pipe_split_attr->split_num == 1) {
            *running_mode = ISP_MODE_RUNNING_OFFLINE;
        } else {
            *running_mode = ISP_MODE_RUNNING_STRIPING;
        }
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_void isp_drv_get_yuv_mode(vi_pipe_split_attr *pipe_split_attr, hi_bool *yuv_mode,
                             isp_data_input_mode *data_input_mode)
{
    if ((pipe_split_attr->pix_fmt == PIXEL_FORMAT_YUV_SEMIPLANAR_420) ||
        (pipe_split_attr->pix_fmt == PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (pipe_split_attr->pix_fmt == PIXEL_FORMAT_YUV_SEMIPLANAR_422) ||
        (pipe_split_attr->pix_fmt == PIXEL_FORMAT_YVU_SEMIPLANAR_422) ||
        (pipe_split_attr->pix_fmt == PIXEL_FORMAT_YUV_SEMIPLANAR_444) ||
        (pipe_split_attr->pix_fmt == PIXEL_FORMAT_YVU_SEMIPLANAR_444)) {
        *yuv_mode = HI_TRUE;
        *data_input_mode = ISP_MODE_BT1120_YUV;
    } else {
        *yuv_mode = HI_FALSE;
        *data_input_mode = ISP_MODE_RAW;
    }

    return;
}

hi_s32 isp_drv_work_mode_init(hi_vi_pipe vi_pipe, isp_block_attr  *blk_attr)
{
    hi_s32  ret;
    unsigned long flags = 0;
    isp_running_mode running_mode = ISP_MODE_RUNNING_OFFLINE;
    vi_pipe_split_attr pipe_split_attr;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_frame_interrupt_attr frame_int_attr = { 0 };
    hi_bool yuv_mode = HI_FALSE;
    isp_data_input_mode data_input_mode = ISP_MODE_RAW;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(blk_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    drv_ctx->work_mode.pre_block_num = drv_ctx->work_mode.block_num;

    osal_memset(&pipe_split_attr, 0, sizeof(vi_pipe_split_attr));

    ret = isp_drv_get_split_attr(vi_pipe, &pipe_split_attr);
    isp_check_ret(vi_pipe, ret, "isp_drv_get_split_attr");

    isp_check_block_num_return(pipe_split_attr.split_num);

    ret = isp_drv_get_running_mode(&pipe_split_attr, &running_mode);
    isp_check_ret(vi_pipe, ret, "isp_drv_get_running_mode");

    isp_drv_get_yuv_mode(&pipe_split_attr, &yuv_mode, &data_input_mode);

    ret = isp_drv_get_frame_interrupt_attr(vi_pipe, &frame_int_attr);
    isp_check_ret(vi_pipe, ret, "isp_drv_get_frame_interrupt_attr");

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->yuv_mode = yuv_mode;
    drv_ctx->work_mode.data_input_mode = data_input_mode;

    drv_ctx->work_mode.block_num    = pipe_split_attr.split_num;
    drv_ctx->work_mode.running_mode = running_mode;
    drv_ctx->work_mode.over_lap     = pipe_split_attr.over_lap;

    blk_attr->block_num    = pipe_split_attr.split_num;
    blk_attr->over_lap     = pipe_split_attr.over_lap;
    blk_attr->running_mode = running_mode;

    drv_ctx->work_mode.frame_rect.width  = pipe_split_attr.wch_out_rect.width;
    drv_ctx->work_mode.frame_rect.height = pipe_split_attr.wch_out_rect.height;

    osal_memcpy(&blk_attr->frame_rect, &drv_ctx->work_mode.frame_rect, sizeof(hi_size));
    osal_memcpy(drv_ctx->work_mode.block_rect, pipe_split_attr.rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM);
    osal_memcpy(blk_attr->block_rect, pipe_split_attr.rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM);
    drv_ctx->frame_int_attr = frame_int_attr;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_work_mode_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    drv_ctx->work_mode.running_mode = ISP_MODE_RUNNING_OFFLINE;

    return HI_SUCCESS;
}

hi_u8 isp_drv_get_block_num(hi_vi_pipe vi_pipe)
{
    return g_isp_drv_ctx[vi_pipe].work_mode.block_num;
}

hi_s8 isp_drv_get_block_id(hi_vi_pipe vi_pipe, isp_running_mode running_mode)
{
    hi_s8 block_id = 0;

    switch (running_mode) {
        default:
        case ISP_MODE_RUNNING_OFFLINE:
        case ISP_MODE_RUNNING_SIDEBYSIDE:
        case ISP_MODE_RUNNING_STRIPING:
            block_id = 0;
            break;
        case ISP_MODE_RUNNING_ONLINE:
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
            break;
    }

    return block_id;
}

hi_s32 isp_drv_get_be_regs_attr(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_be_regs_attr *blk_attr)
{
    hi_u8  k = 0;
    hi_u8  blk_dev, striping_num, blk_num;
    hi_s8  block_id;
    isp_running_mode running_mode;
    isp_be_wo_reg_cfg *isp_be_reg_cfg = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(blk_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    running_mode = drv_ctx->work_mode.running_mode;
    blk_num = isp_drv_get_block_num(vi_pipe);
    blk_num = div_0_to_1(blk_num);
    block_id = isp_drv_get_block_id(vi_pipe, running_mode);
    if (block_id == -1) {
        isp_err_trace("ISP[%d] Online Mode Pipe Err!\n", vi_pipe);
        return HI_FAILURE;
    }

    blk_dev = (hi_u8)block_id;
    blk_attr->block_dev = blk_dev;
    blk_attr->block_num = blk_num;

    striping_num = (running_mode == ISP_MODE_RUNNING_STRIPING) ? ISP_STRIPING_MAX_NUM : 1;

    switch (running_mode) {
        case ISP_MODE_RUNNING_ONLINE:
            isp_drv_bereg_ctx(blk_dev, be_reg[blk_dev]);
            break;
        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_MAX_BE_NUM; k++) {
                isp_drv_bereg_ctx(k, be_reg[k]);
            }
            break;
        case ISP_MODE_RUNNING_OFFLINE:
        case ISP_MODE_RUNNING_STRIPING:
            isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);
            if (drv_ctx->use_node == HI_NULL) {
                isp_warn_trace("ISP[%d] UseNode is Null!\n", vi_pipe);
                return HI_FAILURE;
            }
            isp_be_reg_cfg = (isp_be_wo_reg_cfg *)drv_ctx->use_node->be_cfg_buf.vir_addr;
            for (k = 0; k < striping_num; k++) {
                be_reg[k] = &isp_be_reg_cfg->be_reg_cfg[k];
            }
            break;
        default:
            isp_err_trace("ISP[%d] Running Mode Err!\n", vi_pipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_chn_select_write(hi_vi_pipe vi_pipe, hi_u32 channel_sel)
{
    hi_u8 i, k;
    hi_u32 chn_switch[ISP_CHN_SWITCH_NUM] = { 0 };
    hi_s32 ret;
    isp_chn_sel chn_sel;
    isp_be_regs_attr be_regs_attr = { 0 };
    S_ISPBE_REGS_TYPE *be_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    isp_drv_ctx *drv_ctx = HI_NULL;
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_pipe_return(vi_pipe);

    ret = isp_drv_get_be_regs_attr(vi_pipe, be_reg, &be_regs_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Get BeRegs Attr Err!\n", vi_pipe);
        return HI_FAILURE;
    }

    chn_switch[4] = drv_ctx->yuv_mode ? 1 : 0; /* register[4] value 1 means yuv mode */

    for (i = 0; i < ISP_CHN_SWITCH_NUM - 1; i++) {
        chn_switch[i] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[i];
    }

    chn_sel = (isp_chn_sel)(channel_sel & 0x3);
    switch (chn_sel) {
        default:
        case ISP_CHN_SWITCH_NORMAL:
            break;

        case ISP_CHN_SWITCH_2LANE:
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0];
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1];
            break;

        case ISP_CHN_SWITCH_3LANE:
            chn_switch[2] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0]; /* array index 2 */
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1];
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[2]; /* array index 2 */
            break;

        case ISP_CHN_SWITCH_4LANE:
            chn_switch[3] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[0]; /* array index 3 */
            chn_switch[2] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[1]; /* array index 2 */
            chn_switch[1] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[2]; /* array index 2 */
            chn_switch[0] = drv_ctx->chn_sel_attr[0].wdr_chn_sel[3]; /* array index 3 */
            break;
    }

    if (is_fs_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        for (k = 0; k < be_regs_attr.block_num; k++) {
            isp_drv_set_input_sel(be_reg[k + be_regs_attr.block_dev], &chn_switch[0]);
        }
    }

    return HI_SUCCESS;
}

/* ISP FE read sta */
hi_s32 isp_drv_fe_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    hi_u8 chn_num_max;
    hi_u32 k;
    hi_vi_pipe vi_pipe_bind;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_stat *stat = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg  = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_vir_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->wdr_attr.mast_pipe == HI_FALSE) {
        return HI_SUCCESS;
    }

    stat = (isp_stat *)stat_info->virt_addr;
    if (stat == HI_NULL) {
        return HI_FAILURE;
    }

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);

    for (k = 0; k < chn_num_max; k++) {
        /* get side statistics */
        vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
        isp_check_pipe_return(vi_pipe_bind);
        isp_drv_fereg_ctx(vi_pipe_bind, fe_reg);
        isp_drv_fe_apb_statistics_read(stat_info, fe_reg, drv_ctx, vi_pipe_bind, k);
        isp_drv_fe_stt_statistics_read(stat_info, fe_reg, drv_ctx, vi_pipe_bind, k);
    }

    return HI_SUCCESS;
}

/* ISP BE read sta from FHY, online mode */
hi_s32 isp_drv_be_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    hi_s32 ret;
    isp_stat *stat = HI_NULL;

    isp_stat_key un_statkey;
    isp_be_regs_attr be_regs_attr = { 0 };
    S_ISPBE_REGS_TYPE *be_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);

    ret = isp_drv_get_be_regs_attr(vi_pipe, be_reg, &be_regs_attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    stat = (isp_stat *)stat_info->virt_addr;

    if (stat == HI_NULL) {
        return HI_FAILURE;
    }

    un_statkey.key = stat_info->stat_key.bit32_isr_access;

    stat->be_update = HI_TRUE;

    isp_drv_be_apb_statistics_read(vi_pipe, stat, be_reg, be_regs_attr, un_statkey);
    isp_drv_be_stt_statistics_read(vi_pipe, stat, be_reg, be_regs_attr, un_statkey);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_offline_stt_get(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, S_ISP_STT_REGS_TYPE *be_stt[],
                                  hi_u8  blk_num, isp_stat *stat)
{
    hi_s32 k, ret;
    for (k = 0; k < blk_num; k++) {
        be_stt[k] = (S_ISP_STT_REGS_TYPE *)drv_ctx->be_off_stt_attr.be_stt_buf[k].vir_addr;
        if (be_stt[k] == HI_NULL) {
            isp_err_trace("be_stt is HI_NULL point\n");
            return HI_FAILURE;
        }

        if (ckfn_vi_get_be_stt_info()) {
            ret = call_vi_get_be_stt_info(vi_pipe, k, be_stt[k]);
            if (ret != HI_SUCCESS) {
                isp_err_trace("Pipe[%d] call_vi_get_be_stt_info err 0x%x\n", vi_pipe, ret);
                return HI_FAILURE;
            }
        } else {
            isp_err_trace("Pipe[%d] vi_get_be_stt_info is HI_NULL\n", vi_pipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

hi_void isp_drv_be_ae_offline_statistics_all_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[],
                                                  hi_u8  blk_num, isp_stat_key un_statkey)
{
    if (un_statkey.bit1_be_ae_glo_stat) {
        isp_drv_be_ae_global_offline_statistics_read(stat, be_stt, blk_num);
    }

    if (un_statkey.bit1_be_ae_loc_stat) {
        isp_drv_be_ae_local_offline_statistics_read(stat, be_stt, blk_num);
    }

    if (un_statkey.bit1_mg_stat) {
        isp_drv_be_mg_offline_statistics_read(stat, be_stt, blk_num);
    }
}

/* ISP BE read sta from VI, offline mode */
hi_s32 isp_drv_be_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    hi_s32 ret;
    hi_u8 blk_num;
    isp_stat_key un_statkey;
    isp_stat *stat = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;
    S_ISP_STT_REGS_TYPE *be_stt[ISP_STRIPING_MAX_NUM] = { [0 ...(ISP_STRIPING_MAX_NUM - 1)] = HI_NULL };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    blk_num = isp_drv_get_block_num(vi_pipe);
    blk_num = div_0_to_1(blk_num);

    stat = (isp_stat *)stat_info->virt_addr;
    isp_check_pointer_return(stat);

    stat->be_update = HI_FALSE;
    ret = isp_drv_be_offline_stt_get(vi_pipe, drv_ctx, be_stt, blk_num, stat);
    isp_check_ret(vi_pipe, ret, "isp_drv_be_offline_stt_get");
    stat->be_update = HI_TRUE;

    if (stat->be_update == HI_FALSE) {
        drv_ctx->drv_dbg_info.isp_be_sta_lost++;
    }

    un_statkey.key = stat_info->stat_key.bit32_isr_access;

    /* BE AE statistics */
    isp_drv_be_ae_offline_statistics_all_read(stat, be_stt, blk_num, un_statkey);

    /* BE AWB statistics */
    isp_drv_be_awb_offline_statistics_read(vi_pipe, stat,  be_stt, blk_num, un_statkey);

    /* BE AF statistics */
    if (un_statkey.bit1_be_af_stat) {
        isp_drv_be_af_offline_statistics_read(vi_pipe, stat, be_stt, blk_num);
    }

    if (un_statkey.bit1_dehaze) {
        isp_drv_be_dehaze_offline_statistics_read(stat, be_stt, blk_num);
    }

    isp_drv_dpc_offline_calib_info_read(stat, be_stt, blk_num, un_statkey.bit1_dp_stat);

    isp_drv_flicker_offline_statistics_read(stat, be_stt, blk_num, un_statkey.bit1_flicker);

    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        isp_drv_ldci_offline_statistics_read(vi_pipe, stat);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    /* online snap, AE and AWB params set by the preview pipe.
      In order to get picture as fast as, dehaze don't used. */
    if (is_online_mode(drv_ctx->work_mode.running_mode) || is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        if ((drv_ctx->snap_attr.picture_pipe_id == vi_pipe) &&
            (drv_ctx->snap_attr.picture_pipe_id != drv_ctx->snap_attr.preview_pipe_id)) {
            return HI_SUCCESS;
        }
    }

    if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
        if (drv_ctx->stitch_attr.main_pipe == HI_TRUE) {
            isp_drv_fe_stitch_statistics_read(vi_pipe, stat_info);
        }

        isp_drv_fe_stitch_non_statistics_read(vi_pipe, stat_info);
#endif
    } else {
        ret = isp_drv_fe_statistics_read(vi_pipe, stat_info);
        if (ret != HI_SUCCESS) {
            isp_err_trace("isp_drv_fe_statistics_read failed!\n");
            return HI_FAILURE;
        }
    }

    if (is_online_mode(drv_ctx->work_mode.running_mode) || is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        /* BE statistics for online */
        ret = isp_drv_be_statistics_read(vi_pipe, stat_info);
        if (ret != HI_SUCCESS) {
            isp_err_trace("isp_drv_be_statistics_read failed!\n");
            return HI_FAILURE;
        }

        isp_drv_be_stitch_statistics_read(vi_pipe, stat_info);
    } else if (is_offline_mode(drv_ctx->work_mode.running_mode) || is_striping_mode(drv_ctx->work_mode.running_mode)) {
        /* BE statistics for offline */
        ret = isp_drv_be_offline_statistics_read(vi_pipe, stat_info);
        if (ret) {
            isp_err_trace("isp_drv_be_offline_statistics_read failed!\n");
            return HI_FAILURE;
        }

        isp_drv_be_offline_stitch_statistics_read(vi_pipe, stat_info);
    } else {
        isp_err_trace("enIspOnlineMode err 0x%x!\n", drv_ctx->work_mode.running_mode);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_stat_buf_init(hi_vi_pipe vi_pipe, hi_u64 *point_phy_addr)
{
    hi_s32 ret, i;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr, size;
    hi_u8 *vir_addr = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = {0};
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(point_phy_addr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->statistics_buf.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].Stat", vi_pipe);

    size = sizeof(isp_stat) * MAX_ISP_STAT_BUF_NUM;
    ret = cmpi_mmz_malloc_cached(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc ISP statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->statistics_buf.phy_addr = phy_addr;
    drv_ctx->statistics_buf.vir_addr = (hi_void *)vir_addr;
    drv_ctx->statistics_buf.size  = size;

    OSAL_INIT_LIST_HEAD(&drv_ctx->statistics_buf.free_list);
    OSAL_INIT_LIST_HEAD(&drv_ctx->statistics_buf.busy_list);
    OSAL_INIT_LIST_HEAD(&drv_ctx->statistics_buf.user_list);

    for (i = 0; i < MAX_ISP_STAT_BUF_NUM; i++) {
        drv_ctx->statistics_buf.node[i].stat_info.phy_addr =
            phy_addr + i * sizeof(isp_stat);
        drv_ctx->statistics_buf.node[i].stat_info.virt_addr =
            (hi_void *)(vir_addr + i * sizeof(isp_stat));

        drv_ctx->statistics_buf.node[i].stat_info.stat_key.key =
            ((hi_u64)ISP_STATISTICS_KEY << 32) + ISP_STATISTICS_KEY; /* left shift 32 bits */

        osal_list_add_tail(&drv_ctx->statistics_buf.node[i].list,
                           &drv_ctx->statistics_buf.free_list);
    }

    drv_ctx->statistics_buf.init      = HI_TRUE;
    drv_ctx->statistics_buf.busy_num = 0;
    drv_ctx->statistics_buf.user_num = 0;
    drv_ctx->statistics_buf.free_num = MAX_ISP_STAT_BUF_NUM;

    *point_phy_addr = drv_ctx->statistics_buf.phy_addr;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_stat_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr;
    hi_u8 *vir_addr = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->statistics_buf.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    phy_addr = drv_ctx->statistics_buf.phy_addr;
    vir_addr = (hi_u8 *)drv_ctx->statistics_buf.vir_addr;

    drv_ctx->statistics_buf.vir_addr = HI_NULL;
    drv_ctx->statistics_buf.node[0].stat_info.virt_addr = HI_NULL;
    drv_ctx->statistics_buf.node[1].stat_info.virt_addr = HI_NULL;
    drv_ctx->statistics_buf.phy_addr = 0;
    drv_ctx->statistics_buf.node[0].stat_info.phy_addr = 0;
    drv_ctx->statistics_buf.node[1].stat_info.phy_addr = 0;
    drv_ctx->statistics_buf.init    = HI_FALSE;

    OSAL_INIT_LIST_HEAD(&drv_ctx->statistics_buf.free_list);
    OSAL_INIT_LIST_HEAD(&drv_ctx->statistics_buf.busy_list);
    OSAL_INIT_LIST_HEAD(&drv_ctx->statistics_buf.user_list);
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_stat_buf_user_get(hi_vi_pipe vi_pipe, isp_stat_info **stat_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    struct osal_list_head *plist = HI_NULL;
    isp_stat_node *node = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_stabuf_init_return(vi_pipe, drv_ctx->statistics_buf.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (osal_list_empty(&drv_ctx->statistics_buf.busy_list)) {
        isp_warn_trace("busy list empty\n");
        *stat_info = HI_NULL;
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }

    /* get busy */
    plist = drv_ctx->statistics_buf.busy_list.next;
    if (plist == HI_NULL) {
        isp_warn_trace("busy list empty\n");
        *stat_info = HI_NULL;
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }
    osal_list_del(plist);
    drv_ctx->statistics_buf.busy_num--;

    /* return info */
    node = osal_list_entry(plist, isp_stat_node, list);
    *stat_info = &node->stat_info;

    /* put user */
    osal_list_add_tail(plist, &drv_ctx->statistics_buf.user_list);
    drv_ctx->statistics_buf.user_num++;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_stat_buf_user_put(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    struct osal_list_head *plist = HI_NULL;
    isp_stat_node *node = HI_NULL;
    hi_bool valid = HI_FALSE;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_stabuf_init_return(vi_pipe, drv_ctx->statistics_buf.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    osal_list_for_each(plist, &drv_ctx->statistics_buf.user_list) {
        node = osal_list_entry(plist, isp_stat_node, list);
        if (node == HI_NULL) {
            isp_err_trace("node  null pointer\n");
            break;
        }

        if (node->stat_info.phy_addr == stat_info->phy_addr) {
            valid = HI_TRUE;
            node->stat_info.stat_key.key = stat_info->stat_key.key;
            break;
        }
    }

    if (!valid) {
        isp_err_trace("invalid stat info, phy 0x%llx\n", stat_info->phy_addr);
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }

    /* get user */
    if (plist == HI_NULL) {
        isp_err_trace("user list empty\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }
    osal_list_del(plist);
    drv_ctx->statistics_buf.user_num--;

    /* put free */
    osal_list_add_tail(plist, &drv_ctx->statistics_buf.free_list);
    drv_ctx->statistics_buf.free_num++;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_set_ldci_stt_addr(hi_vi_pipe vi_pipe, hi_u64 read_stt_addr, hi_u64 write_stt_addr)
{
    hi_u8  k, blk_dev, blk_num;
    hi_s32 ret;
    hi_u32 size = sizeof(isp_ldci_stat);
    hi_u64 write_addr;
    isp_be_regs_attr  be_regs_attr = { 0 };
    S_VIPROC_REGS_TYPE *viproc_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_pipe_return(vi_pipe);

    ret = isp_drv_get_viproc_regs_attr(vi_pipe, viproc_reg, &be_regs_attr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Get ViprocRegs Attr Err!\n", vi_pipe);
        return HI_FAILURE;
    }

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;
    for (k = 0; k < blk_num; k++) {
        write_addr = write_stt_addr + k * (size / div_0_to_1(blk_num));
        isp_drv_set_ldci_write_stt_addr(viproc_reg[k + blk_dev], write_addr);

        if (g_ldci_tpr_flt_en[vi_pipe] == HI_FALSE) {
            isp_drv_set_ldci_read_stt_addr(viproc_reg[k + blk_dev], read_stt_addr);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_update_ldci_normal_online_attr(hi_vi_pipe vi_pipe)
{
    hi_u8 write_buf_idx;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    write_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;

    drv_ctx->ldci_read_buf_attr.buf_idx  = write_buf_idx;
    drv_ctx->ldci_write_buf_attr.buf_idx = (write_buf_idx + 1) % div_0_to_1(drv_ctx->ldci_write_buf_attr.buf_num);

    return HI_SUCCESS;
}

hi_s32 isp_drv_update_ldci_tpr_online_attr(hi_vi_pipe vi_pipe, isp_stat *stat)
{
    hi_u8 cur_read_buf_idx;
    isp_drv_ctx   *drv_ctx   = HI_NULL;
    isp_ldci_stat *ldci_stat = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    cur_read_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;

    /* Get LDCI Statistics from WriteBuffer(copy statistics to stat), then update WriteSttAddr */
    ldci_stat = (isp_ldci_stat *)drv_ctx->ldci_write_buf_attr.ldci_buf[cur_read_buf_idx].vir_addr;

    if (ldci_stat != HI_NULL) {
        osal_memcpy(&stat->ldci_stat, ldci_stat, sizeof(isp_ldci_stat));
    }
    drv_ctx->ldci_write_buf_attr.buf_idx = (cur_read_buf_idx + 1) % div_0_to_1(drv_ctx->ldci_write_buf_attr.buf_num);

    return HI_SUCCESS;
}

hi_s32 isp_drv_update_ldci_normal_offline_attr(hi_vi_pipe vi_pipe, isp_be_sync_para *be_sync_para)
{
    hi_u8  i, read_buf_idx, write_buf_idx, blk_num;
    hi_u32 size;
    hi_u64 write_stt_addr, read_stt_addr;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    blk_num      = drv_ctx->work_mode.block_num;
    size       = sizeof(isp_ldci_stat);
    read_buf_idx  = drv_ctx->ldci_read_buf_attr.buf_idx;
    write_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;
    write_stt_addr = drv_ctx->ldci_write_buf_attr.ldci_buf[write_buf_idx].phy_addr;
    read_stt_addr  = drv_ctx->ldci_read_buf_attr.ldci_buf[read_buf_idx].phy_addr;

    for (i = 0; i < blk_num; i++) {
        be_sync_para->ldci_read_stt_addr[i]  = read_stt_addr;
        be_sync_para->ldci_write_stt_addr[i] = write_stt_addr + i * (size / div_0_to_1(blk_num));
    }

    drv_ctx->ldci_read_buf_attr.buf_idx  = write_buf_idx;
    drv_ctx->ldci_write_buf_attr.buf_idx = (write_buf_idx + 1) % div_0_to_1(drv_ctx->ldci_write_buf_attr.buf_num);

    return HI_SUCCESS;
}

hi_s32 isp_drv_update_ldci_tpr_offline_attr(hi_vi_pipe vi_pipe, isp_be_sync_para *be_sync_para)
{
    hi_u8 i;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (i = 0; i < drv_ctx->work_mode.block_num; i++) {
        be_sync_para->ldci_read_stt_addr[i]  = drv_ctx->ldci_stt_addr.read_stt_addr[i];
        be_sync_para->ldci_write_stt_addr[i] = drv_ctx->ldci_stt_addr.cur_write_stt_addr[i];
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_ldci_online_attr_update(hi_vi_pipe vi_pipe, isp_stat *stat)
{
    isp_check_pipe_return(vi_pipe);

    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        /* Copy LDCI statistics information to stat, then update LDCI WriteSttAddr */
        isp_drv_update_ldci_tpr_online_attr(vi_pipe, stat);
    } else {
        /* Only update LDCI ReadSttAddr and WriteSttAddr */
        isp_drv_update_ldci_normal_online_attr(vi_pipe);
    }
    return HI_SUCCESS;
}

HI_S32 isp_drv_set_online_statistics_stt_addr(hi_vi_pipe vi_pipe, hi_u32 viproc_irq_status)
{
    hi_u32 cur_read_flag;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u8 read_buf_idx;
    hi_u8 cur_read_buf_idx;
    hi_u8 write_buf_idx;
    hi_s32 ret;
    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (!drv_ctx->isp_init) {
        return HI_FAILURE;
    }

    if ((viproc_irq_status & VIPROC_FRAME_START_INT) == 0) {
        return HI_SUCCESS;
    }

    if (is_offline_mode(drv_ctx->work_mode.running_mode) ||
        is_striping_mode(drv_ctx->work_mode.running_mode)) {
        return HI_SUCCESS;
    }

    cur_read_flag =  1 - drv_ctx->be_online_stt_buf.cur_write_flag;

    ret = isp_drv_set_online_stt_addr(vi_pipe, drv_ctx->be_online_stt_buf.be_stt_buf[cur_read_flag].phy_addr);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] Set ISP online stt addr Err!\n", vi_pipe);
    }

    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        cur_read_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;
        ret = isp_drv_set_ldci_stt_addr(vi_pipe, drv_ctx->ldci_read_buf_attr.ldci_buf[0].phy_addr,
                                        drv_ctx->ldci_write_buf_attr.ldci_buf[cur_read_buf_idx].phy_addr);
    } else {
        read_buf_idx  = drv_ctx->ldci_read_buf_attr.buf_idx;
        write_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;

        ret = isp_drv_set_ldci_stt_addr(vi_pipe, drv_ctx->ldci_read_buf_attr.ldci_buf[read_buf_idx].phy_addr,
                                        drv_ctx->ldci_write_buf_attr.ldci_buf[write_buf_idx].phy_addr);
    }
    return ret;
}

hi_s32 isp_drv_stat_buf_busy_put(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    struct osal_list_head *plist = HI_NULL;
    isp_stat_node *node = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_stabuf_init_return(vi_pipe, drv_ctx->statistics_buf.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    /* There should be one frame of the newest statistics info in busy list. */
    while (!osal_list_empty(&drv_ctx->statistics_buf.busy_list)) {
        plist = drv_ctx->statistics_buf.busy_list.next;
        if (plist == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            return HI_FAILURE;
        }
        osal_list_del(plist);
        drv_ctx->statistics_buf.busy_num--;

        osal_list_add_tail(plist, &drv_ctx->statistics_buf.free_list);
        drv_ctx->statistics_buf.free_num++;
    }

    if (osal_list_empty(&drv_ctx->statistics_buf.free_list)) {
        isp_warn_trace("free list empty\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

        return HI_FAILURE;
    }

    /* get free */
    plist = drv_ctx->statistics_buf.free_list.next;
    if (plist == HI_NULL) {
        isp_warn_trace("free list empty\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }
    osal_list_del(plist);
    drv_ctx->statistics_buf.free_num--;

    /* read statistics */
    node = osal_list_entry(plist, isp_stat_node, list);

    drv_ctx->statistics_buf.act_stat = &node->stat_info;
    ret = isp_drv_statistics_read(vi_pipe, &node->stat_info);

    /* put busy */
    osal_list_add_tail(plist, &drv_ctx->statistics_buf.busy_list);
    drv_ctx->statistics_buf.busy_num++;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return ret;
}

static hi_void isp_drv_ldci_buf_attr_reset(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_u8 i;

    drv_ctx->ldci_read_buf_attr.init  = HI_FALSE;
    drv_ctx->ldci_write_buf_attr.init = HI_FALSE;
    drv_ctx->ldci_read_buf_attr.buf_num = 0;
    drv_ctx->ldci_write_buf_attr.buf_num = 0;

    for (i = 0; i < g_be_buf_num[vi_pipe]; i++) {
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].phy_addr = 0;
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].vir_addr = HI_NULL;
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].size     = 0;
    }

    for (i = 0; i < g_be_buf_num[vi_pipe]; i++) {
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].phy_addr  = 0;
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].vir_addr  = HI_NULL;
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].size      = 0;
    }
}

hi_s32 isp_drv_ldci_tpr_buf_malloc(hi_vi_pipe vi_pipe, hi_u64 *ldci_phy_addr, hi_u8 **ldci_vir_addr)
{
    hi_u8 i, j, write_buf_num, read_buf_num, buf_num_all;
    hi_u8 *vir_addr = HI_NULL;
    hi_s32 ret;
    hi_u64 phy_addr, size;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].LDCI", vi_pipe);
    size = sizeof(isp_ldci_stat);

    if (is_online_mode(drv_ctx->work_mode.running_mode) || is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        write_buf_num = ISP_ONLINE_LDCI_TPR_BUF_NUM;
        read_buf_num  = ISP_ONLINE_LDCI_TPR_BUF_NUM;
    } else {
        write_buf_num = g_be_buf_num[vi_pipe];
        read_buf_num  = g_be_buf_num[vi_pipe];
    }

    buf_num_all = write_buf_num + read_buf_num;

    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size * buf_num_all);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] MmzMalloc Ldci buffer Failure!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size * buf_num_all);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->ldci_write_buf_attr.init    = HI_TRUE;
    drv_ctx->ldci_write_buf_attr.buf_num = write_buf_num;
    drv_ctx->ldci_write_buf_attr.buf_idx = 0;

    drv_ctx->ldci_read_buf_attr.init     = HI_TRUE;
    drv_ctx->ldci_read_buf_attr.buf_num = read_buf_num;
    drv_ctx->ldci_read_buf_attr.buf_idx = 0;

    for (i = 0; i < write_buf_num; i++) {
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].phy_addr = phy_addr + i * size;
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].vir_addr = (hi_void *)(vir_addr + i * size);
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].size     = size;
    }

    for (i = 0; i < read_buf_num; i++) {
        j = i + write_buf_num;
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].phy_addr = phy_addr + j * size;
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].vir_addr = (hi_void *)(vir_addr + j * size);
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].size     = size;
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    *ldci_phy_addr = phy_addr;
    *ldci_vir_addr = vir_addr;

    return HI_SUCCESS;
}

hi_s32 isp_drv_ldci_tpr_buf_init(hi_vi_pipe vi_pipe)
{
    hi_u8 *vir_addr = HI_NULL;
    hi_s32 ret;
    hi_u64 phy_addr;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_buf_init_return(vi_pipe, drv_ctx->ldci_read_buf_attr.init);
    isp_check_buf_init_return(vi_pipe, drv_ctx->ldci_write_buf_attr.init);

    ret = isp_drv_ldci_tpr_buf_malloc(vi_pipe, &phy_addr, &vir_addr);
    isp_check_ret(vi_pipe, ret, "isp_drv_ldci_tpr_buf_malloc");

    if (is_online_mode(drv_ctx->work_mode.running_mode) || is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        ret = isp_drv_set_ldci_stt_addr(vi_pipe, drv_ctx->ldci_read_buf_attr.ldci_buf[0].phy_addr,
                                        drv_ctx->ldci_write_buf_attr.ldci_buf[0].phy_addr);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] Set Ldci Param/OutParam addr Err!\n", vi_pipe);
            goto fail0;
        }

        /* update Write Index */
        drv_ctx->ldci_write_buf_attr.buf_idx =
            (drv_ctx->ldci_write_buf_attr.buf_idx + 1) % div_0_to_1(drv_ctx->ldci_write_buf_attr.buf_num);
    }

    return HI_SUCCESS;

fail0:
    isp_drv_ldci_buf_attr_reset(vi_pipe, drv_ctx);
    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, (hi_void *)vir_addr);
    }
    return HI_FAILURE;
}

hi_s32 isp_drv_ldci_tpr_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    hi_void *vir_addr = HI_NULL;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->ldci_read_buf_attr.init);
    isp_check_buf_exit_return(vi_pipe, drv_ctx->ldci_write_buf_attr.init);

    phy_addr = drv_ctx->ldci_write_buf_attr.ldci_buf[0].phy_addr;
    vir_addr   = drv_ctx->ldci_write_buf_attr.ldci_buf[0].vir_addr;

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    isp_drv_ldci_buf_attr_reset(vi_pipe, drv_ctx);
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_ldci_normal_buf_init(hi_vi_pipe vi_pipe)
{
    hi_u8 i, buf_num;
    hi_s32 ret;
    hi_u64 phy_addr, size;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->ldci_read_buf_attr.init);
    isp_check_buf_init_return(vi_pipe, drv_ctx->ldci_write_buf_attr.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].LDCI", vi_pipe);
    size  = sizeof(isp_ldci_stat);

    buf_num = is_striping_mode(drv_ctx->work_mode.running_mode) ? ISP_STRIPING_LDCI_NORMAL_BUF_NUM : 1;

    phy_addr = cmpi_mmz_malloc(HI_NULL, ac_name, size * buf_num);
    if (phy_addr == 0) {
        isp_err_trace("alloc ISP[%d] Ldci buf err\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->ldci_read_buf_attr.init    = HI_TRUE;
    drv_ctx->ldci_read_buf_attr.buf_num = buf_num;
    drv_ctx->ldci_read_buf_attr.buf_idx = 0;

    drv_ctx->ldci_write_buf_attr.init    = HI_TRUE;
    drv_ctx->ldci_write_buf_attr.buf_num = buf_num;
    drv_ctx->ldci_write_buf_attr.buf_idx = MIN2(buf_num - 1, drv_ctx->ldci_read_buf_attr.buf_idx + 1);

    for (i = 0; i < buf_num; i++) {
        drv_ctx->ldci_read_buf_attr.ldci_buf[i].phy_addr  = phy_addr + i * size;
        drv_ctx->ldci_write_buf_attr.ldci_buf[i].phy_addr = phy_addr + i * size;
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (is_sidebyside_mode(drv_ctx->work_mode.running_mode) || is_online_mode(drv_ctx->work_mode.running_mode)) {
        ret = isp_drv_set_ldci_stt_addr(vi_pipe, phy_addr, phy_addr);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] Set Ldci stt addr Err!\n", vi_pipe);
            goto fail0;
        }

        /* update Write & Read Index */
        drv_ctx->ldci_read_buf_attr.buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;
        drv_ctx->ldci_write_buf_attr.buf_idx =
            (drv_ctx->ldci_write_buf_attr.buf_idx + 1) % div_0_to_1(drv_ctx->ldci_write_buf_attr.buf_num);
    }

    return HI_SUCCESS;

fail0:
    isp_drv_ldci_buf_attr_reset(vi_pipe, drv_ctx);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, HI_NULL);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_ldci_normal_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u64 phy_addr;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->ldci_read_buf_attr.init);
    isp_check_buf_exit_return(vi_pipe, drv_ctx->ldci_write_buf_attr.init);

    phy_addr = drv_ctx->ldci_write_buf_attr.ldci_buf[0].phy_addr;

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    isp_drv_ldci_buf_attr_reset(vi_pipe, drv_ctx);
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, HI_NULL);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_ldci_buf_init(hi_vi_pipe vi_pipe)
{
    isp_check_pipe_return(vi_pipe);

    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        return isp_drv_ldci_tpr_buf_init(vi_pipe);
    } else {
        return isp_drv_ldci_normal_buf_init(vi_pipe);
    }
}

hi_s32 isp_drv_ldci_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        return isp_drv_ldci_tpr_buf_exit(vi_pipe);
    } else {
        return isp_drv_ldci_normal_buf_exit(vi_pipe);
    }

    return HI_FAILURE;
}

hi_s32 isp_drv_ldci_read_stt_buf_get(hi_vi_pipe vi_pipe, isp_ldci_read_stt_buf *ldci_read_stt_buf)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(ldci_read_stt_buf);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    ldci_read_stt_buf->buf_num       = drv_ctx->ldci_read_buf_attr.buf_num;
    ldci_read_stt_buf->buf_idx       = 0;
    ldci_read_stt_buf->head_phy_addr = drv_ctx->ldci_read_buf_attr.ldci_buf[0].phy_addr;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_clut_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u64 phy_addr, size;
    hi_u8 *vir_addr = HI_NULL;
    isp_drv_ctx      *drv_ctx = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->clut_buf_attr.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].Clut", vi_pipe);

    size =  HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32);

    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] MmzMalloc Clut buffer Failure!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size);

    drv_ctx->clut_buf_attr.init              = HI_TRUE;
    drv_ctx->clut_buf_attr.clut_buf.size     = size;
    drv_ctx->clut_buf_attr.clut_buf.phy_addr = phy_addr;
    drv_ctx->clut_buf_attr.clut_buf.vir_addr = (hi_void *)vir_addr;

    return HI_SUCCESS;
}

hi_s32 isp_drv_clut_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_u64   phy_addr;
    hi_void *vir_addr = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->clut_buf_attr.init);

    phy_addr = drv_ctx->clut_buf_attr.clut_buf.phy_addr;
    vir_addr   = drv_ctx->clut_buf_attr.clut_buf.vir_addr;

    drv_ctx->clut_buf_attr.init              = HI_FALSE;
    drv_ctx->clut_buf_attr.clut_buf.size     = 0;
    drv_ctx->clut_buf_attr.clut_buf.phy_addr = 0;
    drv_ctx->clut_buf_attr.clut_buf.vir_addr = HI_NULL;

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_spec_awb_buf_init(hi_vi_pipe vi_pipe)
{
#ifdef CONFIG_HI_ISP_SPECAWB_SUPPORT
    hi_s32 ret;
    hi_u64 phy_addr, size;
    hi_u8 *vir_addr = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->spec_awb_buf_attr.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].SpecAwb", vi_pipe);

    size = ISP_SPECAWB_BUF_SIZE;
    ret  = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] MmzMalloc SpecAwb buffer Failure!\n", vi_pipe);
        return HI_FAILURE;
    }

    osal_memset(vir_addr, 0, size);

    drv_ctx->spec_awb_buf_attr.init                    = HI_TRUE;
    drv_ctx->spec_awb_buf_attr.spec_awb_buf.size       = size;
    drv_ctx->spec_awb_buf_attr.spec_awb_buf.phy_addr   = phy_addr;
    drv_ctx->spec_awb_buf_attr.spec_awb_buf.vir_addr   = (hi_void *)vir_addr;
#endif
    return HI_SUCCESS;
}

hi_s32 isp_drv_spec_awb_buf_exit(hi_vi_pipe vi_pipe)
{
#ifdef CONFIG_HI_ISP_SPECAWB_SUPPORT
    hi_u64   phy_addr;
    hi_void *vir_addr = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->spec_awb_buf_attr.init);

    phy_addr = drv_ctx->spec_awb_buf_attr.spec_awb_buf.phy_addr;
    vir_addr   = drv_ctx->spec_awb_buf_attr.spec_awb_buf.vir_addr;

    drv_ctx->spec_awb_buf_attr.init                  = HI_FALSE;
    drv_ctx->spec_awb_buf_attr.spec_awb_buf.size     = 0;
    drv_ctx->spec_awb_buf_attr.spec_awb_buf.phy_addr = 0;
    drv_ctx->spec_awb_buf_attr.spec_awb_buf.vir_addr = HI_NULL;

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }
#endif
    return HI_SUCCESS;
}

hi_s32 isp_drv_be_buf_malloc(hi_vi_pipe vi_pipe, hi_u64 *be_phy_addr, hi_u8 **be_vir_addr, hi_u64 *be_size)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr, size;
    hi_u8  *vir_addr = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = {0};
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].BeCfg", vi_pipe);

    size = sizeof(isp_be_wo_reg_cfg);

    ret = cmpi_mmz_malloc_cached(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size * g_be_buf_num[vi_pipe]);
    if (ret != HI_SUCCESS) {
        isp_err_trace("Pipe[%d] alloc ISP BeCfgBuf err!\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size * g_be_buf_num[vi_pipe]);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->be_buf_info.init                  = HI_TRUE;
    drv_ctx->be_buf_info.be_buf_haddr.phy_addr = phy_addr;
    drv_ctx->be_buf_info.be_buf_haddr.vir_addr = (hi_void *)vir_addr;
    drv_ctx->be_buf_info.be_buf_haddr.size     = size * g_be_buf_num[vi_pipe];

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    *be_phy_addr = phy_addr;
    *be_vir_addr = vir_addr;
    *be_size     = size;

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_buf_init(hi_vi_pipe vi_pipe, hi_u64 *point_phy_addr)
{
    hi_s32 ret, i;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_be_buf_node *node = HI_NULL;
    hi_u64 phy_addr, size;
    hi_u8  *vir_addr = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(point_phy_addr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);

    isp_check_buf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    ret = isp_drv_be_buf_malloc(vi_pipe, &phy_addr, &vir_addr, &size);
    isp_check_ret(vi_pipe, ret, "isp_drv_be_buf_malloc");

    ret = isp_creat_be_buf_queue(&drv_ctx->be_buf_queue, g_be_buf_num[vi_pipe]);
    isp_check_ret_goto(ret, ret, fail0, "Pipe[%d] CreatBeBufQueue fail!\n", vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    for (i = 0; i < g_be_buf_num[vi_pipe]; i++) {
        node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);
        if (node == HI_NULL) {
            isp_err_trace("Pipe[%d] Get QueueGetFreeBeBuf fail!\r\n", vi_pipe);
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            goto fail1;
        }

        node->be_cfg_buf.phy_addr = phy_addr + i * size;
        node->be_cfg_buf.vir_addr = (hi_void *)(vir_addr + i * size);
        node->be_cfg_buf.size     = size;
        node->hold_cnt            = 0;

        isp_queue_put_free_be_buf(&drv_ctx->be_buf_queue, node);
    }

    drv_ctx->use_node        = HI_NULL;
    drv_ctx->running_state = ISP_BE_BUF_STATE_INIT;
    drv_ctx->exit_state    = ISP_BE_BUF_READY;

    *point_phy_addr = drv_ctx->be_buf_info.be_buf_haddr.phy_addr;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;

fail1:
    isp_destroy_be_buf_queue(&drv_ctx->be_buf_queue);

fail0:
    drv_ctx->be_buf_info.init                  = HI_FALSE;
    drv_ctx->be_buf_info.be_buf_haddr.phy_addr = 0;
    drv_ctx->be_buf_info.be_buf_haddr.vir_addr = HI_NULL;
    drv_ctx->be_buf_info.be_buf_haddr.size     = 0;

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, (hi_void *)vir_addr);
    }

    return HI_FAILURE;
}

hi_s32 isp_drv_be_buf_exit(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    hi_u64 phy_addr;
    hi_void *vir_addr = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->be_buf_info.init);

    phy_addr = drv_ctx->be_buf_info.be_buf_haddr.phy_addr;
    vir_addr   = drv_ctx->be_buf_info.be_buf_haddr.vir_addr;

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->exit_state = ISP_BE_BUF_WAITING;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (check_func_entry(HI_ID_VI) && ckfn_vi_update_vi_vpss_mode()) {
        /* Note: this function cannot be placed in the ISP lock, otherwise it will be deadlocked. */
        call_vi_isp_clear_input_queue(vi_pipe);
    }

    ret = osal_wait_event_timeout_interruptible(&drv_ctx->isp_exit_wait, isp_drv_wait_exit_callback,
                                                &drv_ctx->be_buf_info.use_cnt, g_isp_exit_timeout);
    if (ret <= 0) {
        isp_err_trace("Pipe:%d isp exit wait failed:ret:%d!\n", vi_pipe, ret);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    isp_destroy_be_buf_queue(&drv_ctx->be_buf_queue);

    drv_ctx->be_buf_info.init                  = HI_FALSE;
    drv_ctx->be_buf_info.be_buf_haddr.phy_addr = 0;
    drv_ctx->be_buf_info.be_buf_haddr.vir_addr = HI_NULL;
    drv_ctx->be_buf_info.be_buf_haddr.size     = 0;
    drv_ctx->exit_state                    = ISP_BE_BUF_EXIT;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

osal_spinlock_t *isp_drv_get_spin_lock(hi_vi_pipe vi_pipe)
{
    hi_vi_pipe main_pipe;
    isp_drv_ctx *drv_ctx = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        return &g_isp_lock[vi_pipe];
    } else {
        main_pipe = drv_ctx->stitch_attr.stitch_bind_id[0];
        return &g_isp_sync_lock[main_pipe];
    }
}

hi_s32 isp_drv_get_ready_be_buf(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf)
{
    osal_spinlock_t *isp_spin_lock = HI_NULL;
    unsigned long flags;
    isp_be_buf_node *node = HI_NULL;
    isp_drv_ctx *drv_ctx  = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_cfg_buf);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if ((drv_ctx->exit_state == ISP_BE_BUF_EXIT) ||
        (drv_ctx->exit_state == ISP_BE_BUF_WAITING)) {
        isp_err_trace("ViPipe[%d] ISP BE Buf not existed!!!\n", vi_pipe);
        osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
        return HI_FAILURE;
    }

    node = isp_queue_query_busy_be_buf(&drv_ctx->be_buf_queue);
    if (node == HI_NULL) {
        isp_err_trace("ViPipe[%d] QueueQueryBusyBeBuf pstNode is null!\n", vi_pipe);
        osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
        return HI_FAILURE;
    }

    node->hold_cnt++;
    drv_ctx->be_buf_info.use_cnt++;

    osal_memcpy(be_cfg_buf, &node->be_cfg_buf, sizeof(isp_be_wo_cfg_buf));
    isp_drv_update_ldci_tpr_stt_addr(vi_pipe, (isp_be_wo_reg_cfg *)node->be_cfg_buf.vir_addr);
    drv_ctx->exit_state = ISP_BE_BUF_READY;

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_void isp_drv_put_busy_to_free(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf)
{
    isp_be_buf_node *node = HI_NULL;
    struct osal_list_head *list_tmp = HI_NULL;
    struct osal_list_head *list_node = HI_NULL;
    isp_drv_ctx *drv_ctx  = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_list_for_each_safe(list_node, list_tmp, &drv_ctx->be_buf_queue.busy_list) {
        node = osal_list_entry(list_node, isp_be_buf_node, list);
        if (node->be_cfg_buf.phy_addr == be_cfg_buf->phy_addr) {
            if (node->hold_cnt > 0) {
                node->hold_cnt--;
            }

            if ((node->hold_cnt == 0) && (isp_queue_get_busy_num(&drv_ctx->be_buf_queue) > 1)) {
                isp_queue_del_busy_be_buf(&drv_ctx->be_buf_queue, node);
                isp_queue_put_free_be_buf(&drv_ctx->be_buf_queue, node);
            }
        }
    }

    return;
}

hi_s32 isp_drv_put_free_be_buf(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf)
{
    osal_spinlock_t *isp_spin_lock = HI_NULL;
    isp_drv_ctx *drv_ctx  = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_cfg_buf);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if (drv_ctx->exit_state == ISP_BE_BUF_EXIT) {
        isp_err_trace("ViPipe[%d] ISP BE Buf not existed!!!\n", vi_pipe);
        osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
        return HI_FAILURE;
    }

    isp_drv_put_busy_to_free(vi_pipe, be_cfg_buf);

    if (drv_ctx->be_buf_info.use_cnt > 0) {
        drv_ctx->be_buf_info.use_cnt--;
    }
    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        isp_drv_update_ldci_tpr_offline_stat(vi_pipe, (isp_be_wo_reg_cfg *)be_cfg_buf->vir_addr);
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    osal_wakeup(&drv_ctx->isp_exit_wait);

    return HI_SUCCESS;
}

hi_s32 isp_drv_hold_busy_be_buf(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf)
{
    osal_spinlock_t *isp_spin_lock = HI_NULL;
    isp_be_buf_node *node = HI_NULL;
    struct osal_list_head *list_tmp = HI_NULL;
    struct osal_list_head *list_node = HI_NULL;
    isp_drv_ctx *drv_ctx  = HI_NULL;
    unsigned long  flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_cfg_buf);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if ((drv_ctx->exit_state == ISP_BE_BUF_EXIT) ||
        (drv_ctx->exit_state == ISP_BE_BUF_WAITING)) {
        isp_err_trace("ViPipe[%d] ISP BE Buf not existed!!!\n", vi_pipe);
        osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
        return HI_FAILURE;
    }

    osal_list_for_each_safe(list_node, list_tmp, &drv_ctx->be_buf_queue.busy_list) {
        node = osal_list_entry(list_node, isp_be_buf_node, list);
        if (node->be_cfg_buf.phy_addr == be_cfg_buf->phy_addr) {
            node->hold_cnt++;
            drv_ctx->be_buf_info.use_cnt++;
        }
    }

    drv_ctx->exit_state = ISP_BE_BUF_READY;

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_be_sync_para(hi_vi_pipe vi_pipe, isp_be_sync_para *be_sync_para)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_sync_para);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
        if (isp_drv_get_stitch_be_sync_para(vi_pipe, be_sync_para) != HI_SUCCESS) {
            return HI_FAILURE;
        }
#endif
    } else {
        osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
        osal_memcpy(be_sync_para, &drv_ctx->be_sync_para, sizeof(isp_be_sync_para));
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (g_ldci_tpr_flt_en[vi_pipe] == HI_TRUE) {
        isp_drv_update_ldci_tpr_offline_attr(vi_pipe, be_sync_para);
    } else {
        isp_drv_update_ldci_normal_offline_attr(vi_pipe, be_sync_para);
    }
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_buf_switch_state(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);

    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        drv_ctx->running_state = ISP_BE_BUF_STATE_SWITCH_START;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_void isp_drv_switch_mode_cfg_vc(isp_sync_cfg *sync_cfg)
{
    sync_cfg->vc_num = 0;
    sync_cfg->vc_cfg_num = 0;
    sync_cfg->cfg2_vld_dly_max = 1;

    /* get N (N to 1 WDR) */
    switch (sync_cfg->wdr_mode) {
        default:
            sync_cfg->vc_num_max = 0;
            break;

        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_2To1_FRAME_FULL_RATE:
            sync_cfg->vc_num_max = 1;
            break;

        case WDR_MODE_3To1_FRAME:
        case WDR_MODE_3To1_FRAME_FULL_RATE:
            sync_cfg->vc_num_max = 2; /* reg config is 2 */
            break;

        case WDR_MODE_4To1_FRAME:
        case WDR_MODE_4To1_FRAME_FULL_RATE:
            sync_cfg->vc_num_max = 3; /* reg config is 3 */
            break;
    }
}

hi_void isp_drv_switch_mode_cfg_chn_switch(isp_drv_ctx *drv_ctx, hi_u32 *chn_switch)
{
    hi_u8 i;

    for (i = 0; i < ISP_CHN_SWITCH_NUM - 1; i++) {
        chn_switch[i] = i;
    }

    if (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        chn_switch[0] = 1 % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
        chn_switch[1] = (chn_switch[0] + 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
        chn_switch[2] = (chn_switch[0] + 2) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* array index 2 */
        chn_switch[3] = (chn_switch[0] + 3) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1); /* array index 3 */
    }

    chn_switch[4] = drv_ctx->yuv_mode ? 1 : 0;  /* array index 4 */
}

hi_s32 isp_drv_switch_mode(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_u8   i, j, k;
    hi_u8   blk_dev, blk_num;
    hi_s32  vi_dev;
    hi_u32  ret;
    hi_u32  chn_switch[ISP_CHN_SWITCH_NUM] = { 0 };
    isp_sync_cfg *sync_cfg = HI_NULL;
    isp_be_regs_attr be_regs_attr = { 0 };
    S_ISPBE_REGS_TYPE *be_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(drv_ctx);
    vi_dev = drv_ctx->wdr_attr.vi_dev;

    ret = isp_drv_get_be_regs_attr(vi_pipe, be_reg, &be_regs_attr);
    isp_check_ret_success(vi_pipe, ret, "isp_drv_get_be_regs_attr");

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;

    sync_cfg = &drv_ctx->sync_cfg;
    sync_cfg->wdr_mode = drv_ctx->wdr_cfg.wdr_mode;

    for (j = 0; j < EXP_RATIO_NUM; j++) {
        for (i = 0; i < CFG2VLD_DLY_LIMIT; i++) {
            sync_cfg->exp_ratio[j][i] = drv_ctx->wdr_cfg.exp_ratio[j];
        }
    }

    /* init cfg when modes change */
    osal_memset(&drv_ctx->sync_cfg.sync_cfg_buf, 0, sizeof(isp_sync_cfg_buf));
    osal_memset(&drv_ctx->sync_cfg.node, 0, sizeof(drv_ctx->sync_cfg.node));
    osal_memset(&drv_ctx->isp_int_info, 0, sizeof(isp_interrupt_info));

    isp_drv_switch_mode_cfg_vc(sync_cfg);

    /* Channel Switch config */
    isp_drv_switch_mode_cfg_chn_switch(drv_ctx, chn_switch);

    for (k = 0; k < blk_num; k++) {
        isp_drv_set_input_sel(be_reg[k + blk_dev], &chn_switch[0]);

        osal_memcpy(&drv_ctx->chn_sel_attr[k].wdr_chn_sel, chn_switch, sizeof(chn_switch));
    }

    /* pt_int_mask */
    if ((is_full_wdr_mode(sync_cfg->wdr_mode)) || (is_half_wdr_mode(sync_cfg->wdr_mode))) {
        io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK) |= VI_PT_INT_FSTART;
    } else {
        io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK) &= ~(VI_PT_INT_FSTART);
    }

    sync_cfg->pre_wdr_mode = sync_cfg->wdr_mode;

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_sync_controlnfo(hi_vi_pipe vi_pipe, isp_sync_cfg *sync_cfg)
{
    hi_s32 vi_dev;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_sync_cfg_buf_node *cur_node = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(sync_cfg);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    vi_dev = drv_ctx->wdr_attr.vi_dev;

    sync_cfg->vc_num = (io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT0_ID) & 0x30) >> 4; /* right shift 4 */

    if (sync_cfg->vc_num_max == 0) {
        sync_cfg->vc_num = 0;
    }

    if (sync_cfg->vc_num > sync_cfg->vc_num_max) {
        isp_err_trace("err VC number(%d), can't be large than VC total(%d)!\n",
                      sync_cfg->vc_num, sync_cfg->vc_num_max);
    }

    /* get Cfg2VldDlyMAX */
    if (!isp_sync_buf_is_emrty(&sync_cfg->sync_cfg_buf)) {
        cur_node = &sync_cfg->sync_cfg_buf.sync_cfg_buf_node[sync_cfg->sync_cfg_buf.buf_rd_flag];

        if (cur_node != HI_NULL) {
            if (cur_node->valid) {
                sync_cfg->cfg2_vld_dly_max = cur_node->sns_regs_info.cfg2_valid_delay_max;
            }
        }
    }

    if ((sync_cfg->cfg2_vld_dly_max > CFG2VLD_DLY_LIMIT) || (sync_cfg->cfg2_vld_dly_max < 1)) {
        isp_warn_trace("Delay of config to valid is:0x%x\n", sync_cfg->cfg2_vld_dly_max);
        sync_cfg->cfg2_vld_dly_max = 1;
    }

    sync_cfg->vc_cfg_num = (sync_cfg->vc_num + sync_cfg->vc_num_max * sync_cfg->cfg2_vld_dly_max)
                           % div_0_to_1(sync_cfg->vc_num_max + 1);

    return HI_SUCCESS;
}

static hi_void isp_drv_calc_wdr_gain(isp_sync_cfg *sync_cfg, isp_sync_cfg_buf_node *cur_node, hi_u64 *exp)
{
    hi_u32 i, j;
    hi_u64 tmp   = 0;
    hi_u64 ratio = 0;

    for (j = 0; j < EXP_RATIO_NUM; j++) {
        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            sync_cfg->exp_ratio[j][i] = sync_cfg->exp_ratio[j][i - 1];
            sync_cfg->wdr_gain[j][i] = sync_cfg->wdr_gain[j][i - 1];
        }

        ratio = exp[j + 1];
        tmp   = exp[j];
        tmp   = div_0_to_1(tmp);

        while (ratio > (0x1LL << 25) || tmp > (0x1LL << 25)) { /* left shift 25 */
            tmp >>= 1;
            ratio >>= 1;
        }

        ratio = (ratio * cur_node->ae_reg_cfg.wdr_gain[j + 1]) << WDR_EXP_RATIO_SHIFT;
        tmp = (tmp * cur_node->ae_reg_cfg.wdr_gain[j]);

        while (tmp > (0x1LL << 31)) { /* left shift 31 */
            tmp >>= 1;
            ratio >>= 1;
        }

        ratio = osal_div64_u64(ratio, div_0_to_1(tmp));
        sync_cfg->exp_ratio[j][0] = (hi_u32)ratio;
        sync_cfg->wdr_gain[j][0] = cur_node->ae_reg_cfg.wdr_gain[j];
    }

    for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
        sync_cfg->lf_mode[i] = sync_cfg->lf_mode[i - 1];
        sync_cfg->wdr_gain[3][i] = sync_cfg->wdr_gain[3][i - 1]; /* array index 3 */
    }

    sync_cfg->wdr_gain[3][0] = 0x100; /* array index 3 */

    if ((is_line_wdr_mode(sync_cfg->wdr_mode)) && (cur_node->ae_reg_cfg.fs_wdr_mode == ISP_FSWDR_LONG_FRAME_MODE)) {
        for (j = 0; j < EXP_RATIO_NUM; j++) {
            sync_cfg->exp_ratio[j][0] = 0x40;
            sync_cfg->wdr_gain[j][0] = 0x100;
        }

        sync_cfg->lf_mode[0] = ISP_FSWDR_LONG_FRAME_MODE;
    } else if ((is_line_wdr_mode(sync_cfg->wdr_mode)) &&
               (cur_node->ae_reg_cfg.fs_wdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        for (j = 0; j < EXP_RATIO_NUM; j++) {
            sync_cfg->exp_ratio[j][0] = sync_cfg->exp_ratio[j][0] < 0x45 ? 0x40 : sync_cfg->exp_ratio[j][0];
        }

        sync_cfg->lf_mode[0] = ISP_FSWDR_AUTO_LONG_FRAME_MODE;
    } else {
        sync_cfg->lf_mode[0] = ISP_FSWDR_NORMAL_MODE;
    }
}

static hi_void isp_drv_calc_exp_ratio(isp_sync_cfg *sync_cfg, isp_sync_cfg_buf_node *cur_node,
                                      isp_sync_cfg_buf_node *pre_node, hi_u64 *cur_sns_gain, hi_u64 *pre_sns_gain)
{
    hi_u32 i;
    hi_u64 exp[WDR_CHN_MAX] = { 0 };
    isp_vc_num vc_cfg_num;

    vc_cfg_num = (isp_vc_num)sync_cfg->vc_cfg_num;

    for (i = 0; i < WDR_CHN_MAX; i++) {
        exp[i] = cur_node->ae_reg_cfg.int_time[i] * cur_sns_gain[0];
    }

    if (is_full_wdr_mode(sync_cfg->wdr_mode)) {
        switch (vc_cfg_num) {
            case ISP_VC_NUM_VS:
                exp[1] = pre_node->ae_reg_cfg.int_time[1] * pre_sns_gain[0];
                exp[2] = pre_node->ae_reg_cfg.int_time[2] * pre_sns_gain[0]; /* array index 2 */
                exp[3] = pre_node->ae_reg_cfg.int_time[3] * pre_sns_gain[0]; /* array index 3 */
                break;

            case ISP_VC_NUM_S:
                exp[2] = pre_node->ae_reg_cfg.int_time[2] * pre_sns_gain[0]; /* array index 2 */
                exp[3] = pre_node->ae_reg_cfg.int_time[3] * pre_sns_gain[0]; /* array index 3 */
                break;

            case ISP_VC_NUM_M:
                exp[3] = pre_node->ae_reg_cfg.int_time[3] * pre_sns_gain[0]; /* array index 3 */
                break;

            default:
            case ISP_VC_NUM_L:
                break;
        }
    } else if (is_2to1_wdr_mode(sync_cfg->wdr_mode)) {
        exp[1] = cur_node->ae_reg_cfg.int_time[1] * cur_sns_gain[1];
    }

    isp_drv_calc_wdr_gain(sync_cfg, cur_node, exp);
}

static hi_s32 isp_drv_calc_drc_comp(isp_sync_cfg *sync_cfg, isp_sync_cfg_buf_node *cur_node,
                                    isp_sync_cfg_buf_node *pre_node)
{
    hi_u32 i;
    hi_u64 tmp, cur_exp, pre_exp;

    if (is_2to1_wdr_mode(sync_cfg->wdr_mode)) {
        cur_exp = cur_node->ae_reg_cfg.exposure_sf * 0x100;
        pre_exp = pre_node->ae_reg_cfg.exposure_sf * 0x100;
    } else {
        cur_exp = cur_node->ae_reg_cfg.exposure;
        pre_exp = pre_node->ae_reg_cfg.exposure;
        cur_exp = cur_exp * cur_node->ae_reg_cfg.wdr_gain[0];
        pre_exp = pre_exp * pre_node->ae_reg_cfg.wdr_gain[0];
    }
    while (cur_exp > (0x1LL << 31) || pre_exp > (0x1LL << 31)) { /* left shift 31 */
        cur_exp >>= 1;
        pre_exp >>= 1;
    }

    for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
        sync_cfg->drc_comp[i] = sync_cfg->drc_comp[i - 1];
    }

    cur_exp = cur_exp << DRC_COMP_SHIFT;
    tmp = div_0_to_1(pre_exp);

    while (tmp > (0x1LL << 31)) { /* left shift 31 */
        tmp >>= 1;
        cur_exp >>= 1;
    }

    cur_exp = osal_div64_u64(cur_exp, tmp);
    sync_cfg->drc_comp[0] = (hi_u32)cur_exp;

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_sync_cfg_node(isp_sync_cfg *sync_cfg, isp_sync_cfg_buf_node **cur_node_point,
                                 isp_sync_cfg_buf_node **pre_node_point)
{
    hi_bool err = HI_FALSE;
    hi_s32  i;
    isp_sync_cfg_buf_node *cur_node = HI_NULL;
    isp_sync_cfg_buf_node *pre_node = HI_NULL;

    /* update node when VCCfgNum is 0 */
    if (sync_cfg->vc_cfg_num == 0) {
        for (i = CFG2VLD_DLY_LIMIT; i >= 1; i--) {
            sync_cfg->node[i] = sync_cfg->node[i - 1];
        }

        /* avoid skip effective AE results */
        if (isp_sync_buf_is_err(&sync_cfg->sync_cfg_buf)) {
            err = HI_TRUE;
        }

        /* read the newest information */
        isp_sync_buf_read2(&sync_cfg->sync_cfg_buf, &sync_cfg->node[0]);
    }

    cur_node = sync_cfg->node[0];
    if (cur_node == HI_NULL) {
        return HI_FAILURE;
    }
    if (cur_node->valid == HI_FALSE) {
        return HI_FAILURE;
    }

    if (err == HI_TRUE) {
        if (cur_node->sns_regs_info.sns_type == ISP_SNS_I2C_TYPE) {
            for (i = 0; i < cur_node->sns_regs_info.reg_num; i++) {
                cur_node->sns_regs_info.i2c_data[i].update = HI_TRUE;
            }
        } else if (cur_node->sns_regs_info.sns_type == ISP_SNS_SSP_TYPE) {
            for (i = 0; i < cur_node->sns_regs_info.reg_num; i++) {
                cur_node->sns_regs_info.ssp_data[i].update = HI_TRUE;
            }
        }

        cur_node->sns_regs_info.slv_sync.update = HI_TRUE;
    }

    pre_node = sync_cfg->node[0];
    if (sync_cfg->node[1] != HI_NULL) {
        if (sync_cfg->node[1]->valid == HI_TRUE) {
            pre_node = sync_cfg->node[1];
        }
    }

    *cur_node_point = cur_node;
    *pre_node_point = pre_node;

    return HI_SUCCESS;
}

hi_void isp_drv_calc_sns_gain(isp_sync_cfg *sync_cfg, isp_sync_cfg_buf_node *cur_node,
                              isp_sync_cfg_buf_node *pre_node, hi_u64 *cur_gain, hi_u64 *pre_gain)
{
    hi_u64 isp_total_gain;

    if (is_2to1_wdr_mode(sync_cfg->wdr_mode)) {
        if (sync_cfg->vc_cfg_num == 0) {
            isp_total_gain  = (hi_u64)pre_node->ae_reg_cfg.isp_dgain * pre_node->ae_reg_cfg.wdr_gain[0];
            isp_total_gain  = div_0_to_1(isp_total_gain >> ISP_GAIN_SHIFT);
            pre_gain[0] = pre_node->ae_reg_cfg.exposure_sf;
            pre_gain[0] = osal_div_u64(pre_gain[0], div_0_to_1(pre_node->ae_reg_cfg.int_time[0]));
            pre_gain[0] = osal_div_u64(pre_gain[0] << ISP_GAIN_SHIFT, isp_total_gain);

            isp_total_gain  = (hi_u64)cur_node->ae_reg_cfg.isp_dgain * cur_node->ae_reg_cfg.wdr_gain[0];
            isp_total_gain  = div_0_to_1(isp_total_gain >> ISP_GAIN_SHIFT);
            cur_gain[0] = cur_node->ae_reg_cfg.exposure_sf;
            cur_gain[0] = osal_div_u64(cur_gain[0], div_0_to_1(cur_node->ae_reg_cfg.int_time[0]));
            cur_gain[0] = osal_div_u64(cur_gain[0] << ISP_GAIN_SHIFT, isp_total_gain);

            isp_total_gain  = (hi_u64)pre_node->ae_reg_cfg.isp_dgain * pre_node->ae_reg_cfg.wdr_gain[1];
            isp_total_gain  = div_0_to_1(isp_total_gain >> ISP_GAIN_SHIFT);
            pre_gain[1] = pre_node->ae_reg_cfg.exposure;
            pre_gain[1] = osal_div_u64(pre_gain[1], div_0_to_1(pre_node->ae_reg_cfg.int_time[1]));
            pre_gain[1] = osal_div_u64(pre_gain[1] << ISP_GAIN_SHIFT, isp_total_gain);

            isp_total_gain  = (hi_u64)cur_node->ae_reg_cfg.isp_dgain * cur_node->ae_reg_cfg.wdr_gain[1];
            isp_total_gain  = div_0_to_1(isp_total_gain >> ISP_GAIN_SHIFT);
            cur_gain[1] = cur_node->ae_reg_cfg.exposure;
            cur_gain[1] = osal_div_u64(cur_gain[1], div_0_to_1(cur_node->ae_reg_cfg.int_time[1]));
            cur_gain[1] = osal_div_u64(cur_gain[1] << ISP_GAIN_SHIFT, isp_total_gain);

            sync_cfg->pre_sns_gain_sf = pre_gain[0];
            sync_cfg->cur_sns_gain_sf = cur_gain[0];
            sync_cfg->pre_sns_gain    = pre_gain[1];
            sync_cfg->cur_sns_gain    = cur_gain[1];
        }

        pre_gain[0] = sync_cfg->pre_sns_gain_sf;
        cur_gain[0] = sync_cfg->cur_sns_gain_sf;
        pre_gain[1] = sync_cfg->pre_sns_gain;
        cur_gain[1] = sync_cfg->cur_sns_gain;
    } else {
        if (sync_cfg->vc_cfg_num == 0) {
            pre_gain[0] = pre_node->ae_reg_cfg.exposure;
            pre_gain[0] = osal_div_u64(pre_gain[0], div_0_to_1(pre_node->ae_reg_cfg.int_time[0]));
            pre_gain[0] = osal_div_u64(pre_gain[0] << ISP_GAIN_SHIFT, div_0_to_1(pre_node->ae_reg_cfg.isp_dgain));

            cur_gain[0] = cur_node->ae_reg_cfg.exposure;
            cur_gain[0] = osal_div_u64(cur_gain[0], div_0_to_1(cur_node->ae_reg_cfg.int_time[0]));
            cur_gain[0] = osal_div_u64(cur_gain[0] << ISP_GAIN_SHIFT, div_0_to_1(cur_node->ae_reg_cfg.isp_dgain));

            sync_cfg->pre_sns_gain = pre_gain[0];
            sync_cfg->cur_sns_gain = cur_gain[0];
        }

        pre_gain[0] = sync_cfg->pre_sns_gain;
        cur_gain[0] = sync_cfg->cur_sns_gain;
    }
}

hi_s32 isp_drv_calc_sync_cfg(isp_sync_cfg *sync_cfg)
{
    hi_s32 ret;
    hi_u32 i;
    isp_sync_cfg_buf_node *cur_node = HI_NULL;
    isp_sync_cfg_buf_node *pre_node = HI_NULL;
    hi_u64 cur_sns_gain[2] = { 0 }; /* array 2 */
    hi_u64 pre_sns_gain[2] = { 0 }; /* array 2 */

    isp_check_pointer_return(sync_cfg);

    ret = isp_drv_get_sync_cfg_node(sync_cfg, &cur_node, &pre_node);
    if (ret != HI_SUCCESS) {
        return HI_SUCCESS;
    }

    isp_drv_calc_sns_gain(sync_cfg, cur_node, pre_node, cur_sns_gain, pre_sns_gain);

    /* calculate exposure ratio */
    isp_drv_calc_exp_ratio(sync_cfg, cur_node, pre_node, cur_sns_gain, pre_sns_gain);

    /* calculate AlgProc */
    if (is_line_wdr_mode(sync_cfg->wdr_mode)) {
        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            sync_cfg->alg_proc[i] = sync_cfg->alg_proc[i - 1];
        }
        sync_cfg->alg_proc[0] = cur_node->wdr_reg_cfg.wdr_mdt_en;
    }

    /* calculate DRC compensation */
    if (sync_cfg->vc_cfg_num == 0) {
        isp_drv_calc_drc_comp(sync_cfg, cur_node, pre_node);
    }

    return HI_SUCCESS;
}

hi_u8 isp_drv_get_fe_sync_index(isp_drv_ctx *drv_ctx)
{
    hi_u8 index;

    index = (drv_ctx->sync_cfg.cfg2_vld_dly_max - 1) / div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
    return index;
}

hi_u8 isp_drv_get_be_sync_index(isp_drv_ctx *drv_ctx)
{
    hi_u8 index;

    if (is_online_mode(drv_ctx->work_mode.running_mode) ||
        is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        index = (drv_ctx->sync_cfg.cfg2_vld_dly_max - 1) / div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
    } else {
        if (drv_ctx->frame_int_attr.int_type == FRAME_INTERRUPT_START) {
            if (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
                index = drv_ctx->sync_cfg.cfg2_vld_dly_max;
            } else {
                index = drv_ctx->sync_cfg.cfg2_vld_dly_max + 1;
            }
        } else {
            if (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
                index = drv_ctx->sync_cfg.cfg2_vld_dly_max - 1;
            } else {
                index = drv_ctx->sync_cfg.cfg2_vld_dly_max;
            }
        }

        if (g_use_bottom_half && drv_ctx->run_once_flag != HI_TRUE) {
            if (index == 0) {
                index = CFG2VLD_DLY_LIMIT - 1;
            } else {
                index = index - 1;
            }
        }

        if (drv_ctx->sync_cfg.node[0] != HI_NULL) {
            if (((drv_ctx->sync_cfg.node[0]->sns_regs_info.sns_type == ISP_SNS_I2C_TYPE) &&
                (drv_ctx->sync_cfg.node[0]->sns_regs_info.com_bus.i2c_dev == -1)) ||
                ((drv_ctx->sync_cfg.node[0]->sns_regs_info.sns_type == ISP_SNS_SSP_TYPE) &&
                (drv_ctx->sync_cfg.node[0]->sns_regs_info.com_bus.ssp_dev.bit4_ssp_dev == -1))) {
                index = 0;
            }
        }
    }
    return index;
}

hi_void isp_drv_get_exp_ratio(isp_drv_ctx *drv_ctx, hi_u8 sync_index, hi_u32 *ratio)
{
    hi_u8 exp_ratio_index;
    hi_u32 i;

    exp_ratio_index = sync_index;
    if (is_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        if (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            if (is_offline_mode(drv_ctx->work_mode.running_mode) ||
                is_striping_mode(drv_ctx->work_mode.running_mode)) {
                exp_ratio_index = sync_index + 0x2;
                exp_ratio_index = MIN2(exp_ratio_index, CFG2VLD_DLY_LIMIT - 1);
            }
        }

        if (drv_ctx->sync_cfg.node[0] != HI_NULL) {
            if (((drv_ctx->sync_cfg.node[0]->sns_regs_info.sns_type == ISP_SNS_I2C_TYPE) &&
                (drv_ctx->sync_cfg.node[0]->sns_regs_info.com_bus.i2c_dev == -1)) ||
                ((drv_ctx->sync_cfg.node[0]->sns_regs_info.sns_type == ISP_SNS_SSP_TYPE) &&
                (drv_ctx->sync_cfg.node[0]->sns_regs_info.com_bus.ssp_dev.bit4_ssp_dev == -1))) {
                exp_ratio_index = 0;
            }
        }

        for (i = 0; i < EXP_RATIO_NUM; i++) {
            ratio[i] = drv_ctx->sync_cfg.exp_ratio[i][exp_ratio_index];
        }
    } else {
    }

    /* when the data of sensor built-in WDR after decompand is 16bit, the ratio value is as follow. */
    if (is_built_in_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        ratio[0] = BUILT_IN_WDR_RATIO_VS_S;
        ratio[1] = BUILT_IN_WDR_RATIO_S_M;
        ratio[2] = BUILT_IN_WDR_RATIO_M_L; /* array index 2 */
    }

    for (i = 0; i < EXP_RATIO_NUM; i++) {
        ratio[i] = CLIP3(ratio[i], EXP_RATIO_MIN, EXP_RATIO_MAX);
    }
}

hi_u8 isp_drv_get_cfg_node_vc(isp_drv_ctx *drv_ctx)
{
    hi_u8 cfg_node_vc;

    if (is_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        if (is_online_mode(drv_ctx->work_mode.running_mode) ||
            is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
            cfg_node_vc = (drv_ctx->sync_cfg.cfg2_vld_dly_max - 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
        } else {
            cfg_node_vc = (drv_ctx->sync_cfg.cfg2_vld_dly_max + 1) % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
        }
    } else {
        cfg_node_vc  = 0;
    }

    return cfg_node_vc;
}

hi_bool isp_drv_snap_get_pictrue_pipe(isp_drv_ctx *drv_ctx)
{
    hi_vi_pipe picture_pipe;
    isp_drv_ctx *drv_ctx_pic = HI_NULL;
    isp_running_mode picture_running_mode = ISP_MODE_RUNNING_OFFLINE;
    hi_bool online_have_pictrue_pipe = HI_FALSE;

    if (drv_ctx->snap_attr.picture_pipe_id >= 0) {
        picture_pipe = drv_ctx->snap_attr.picture_pipe_id;
        drv_ctx_pic = isp_drv_get_ctx(picture_pipe);
        picture_running_mode = drv_ctx_pic->work_mode.running_mode;

        if ((is_online_mode(picture_running_mode)) || (is_sidebyside_mode(picture_running_mode))) {
            if (picture_pipe != drv_ctx->snap_attr.preview_pipe_id) {
                online_have_pictrue_pipe = HI_TRUE;
            }
        }
    }

    return online_have_pictrue_pipe;
}

isp_sync_cfg_buf_node *isp_drv_stitch_get_cfg_node(isp_drv_ctx *drv_ctx,
                                                   isp_sync_cfg_buf_node *cfg_node, hi_u8 cfg_node_idx)
{
    hi_vi_pipe main_pipe;
    isp_drv_ctx *drv_ctx_s = HI_NULL;

    if ((drv_ctx->stitch_attr.stitch_enable == HI_TRUE) && (drv_ctx->stitch_attr.main_pipe == HI_FALSE)) {
        main_pipe = drv_ctx->stitch_attr.stitch_bind_id[0];
        drv_ctx_s = isp_drv_get_ctx(main_pipe);
        cfg_node  = drv_ctx_s->sync_cfg.node[cfg_node_idx];
        isp_check_pointer_success(cfg_node);
    }

    return cfg_node;
}

static hi_void isp_drv_reg_config_pq_ai_dgain(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, hi_u32  isp_dgain)
{
    hi_u8  i;
    isp_drv_ctx *ai_drv_ctx = HI_NULL;

    if (vi_pipe == drv_ctx->ai_info.base_pipe_id) {
        if (drv_ctx->ai_info.ai_pipe_id < 0 || drv_ctx->ai_info.ai_pipe_id >= ISP_MAX_PIPE_NUM) {
            isp_err_trace("Err ai pipe %d!\n", drv_ctx->ai_info.ai_pipe_id);
            return;
        }

        ai_drv_ctx = isp_drv_get_ctx(drv_ctx->ai_info.ai_pipe_id);
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            ai_drv_ctx->be_sync_para.isp_dgain[i] = isp_dgain;
        }
    }
}

hi_void isp_drv_reg_config_be_dgain(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
                                    isp_drv_ctx *drv_ctx, isp_ae_reg_cfg_2 *ae_reg_cfg,
                                    isp_be_regs_attr *be_regs_attr)
{
    hi_u32  isp_dgain;
    hi_s32  ret, i;
    hi_u8   blk_dev, blk_num;
    hi_bool online_have_pictrue_pipe;
    S_ISPBE_REGS_TYPE *snap_be_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    isp_be_regs_attr isp_snap_be_regs_attr = { 0 };

    blk_dev = be_regs_attr->block_dev;
    blk_num = be_regs_attr->block_num;
    isp_dgain = ae_reg_cfg->isp_dgain;
    online_have_pictrue_pipe = isp_drv_snap_get_pictrue_pipe(drv_ctx);

    if ((drv_ctx->ai_info.pq_ai_en == HI_TRUE) && (vi_pipe == drv_ctx->ai_info.ai_pipe_id)) {
        return;
    }

    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        isp_dgain = drv_ctx->snap_info_load.isp_cfg_info.isp_dgain;
    }

    isp_dgain = CLIP3(isp_dgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);

    if (online_have_pictrue_pipe == HI_TRUE) {
        if (vi_pipe == drv_ctx->snap_attr.preview_pipe_id) {
            if (drv_ctx->snap_attr.picture_pipe_id < 0 || drv_ctx->snap_attr.picture_pipe_id > ISP_MAX_PIPE_NUM) {
                isp_err_trace("Err isp pipe %d!\n", drv_ctx->snap_attr.picture_pipe_id);
                return;
            }
            ret = isp_drv_get_be_regs_attr(drv_ctx->snap_attr.picture_pipe_id,
                                           snap_be_reg, &isp_snap_be_regs_attr);
            if (ret == HI_SUCCESS) {
                isp_drv_reg_config_dgain(snap_be_reg, isp_dgain,
                                         isp_snap_be_regs_attr.block_num, isp_snap_be_regs_attr.block_dev);
            }
        }
    }

    if (drv_ctx->ai_info.pq_ai_en == HI_TRUE) {
        isp_drv_reg_config_pq_ai_dgain(vi_pipe, drv_ctx, isp_dgain);
    }

    isp_drv_reg_config_dgain(be_reg, isp_dgain, blk_num, blk_dev);
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        drv_ctx->be_sync_para.isp_dgain[i] = isp_dgain;
    }
}

hi_void isp_drv_reg_config_sync_ccm(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
    isp_drv_ctx *drv_ctx, hi_u16 *color_matrix, isp_be_regs_attr *be_regs_attr)
{
    hi_s32  i;
    hi_u16  ccm[CCM_MATRIX_SIZE] = {
        0x01A7,  0x809D,  0x800A,
        0x804F,  0x018E,  0x803F,
        0x000B,  0x8098,  0x018D
    };

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        ccm[i] = color_matrix[i];
    }

    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        if (drv_ctx->snap_attr.load_ccm == HI_TRUE) {
            for (i = 0; i < CCM_MATRIX_SIZE; i++) {
                ccm[i] = drv_ctx->snap_info_load.isp_cfg_info.cap_ccm[i];
            }
        } else {
            for (i = 0; i < CCM_MATRIX_SIZE; i++) {
                ccm[i] = color_matrix[i];
            }
        }
    }

    isp_drv_config_ccm(drv_ctx, ccm);
}

static hi_void isp_drv_reg_config_4dgain_online_double_snap(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx,
    isp_sync_4dgain_cfg *sync_4dgain_cfg)
{
    hi_s32  ret;
    hi_bool online_have_pictrue_pipe;
    S_ISPBE_REGS_TYPE *snap_be_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    isp_be_regs_attr  isp_snap_be_regs_attr = { 0 };
    isp_drv_ctx       *snap_drv_ctx = HI_NULL;
    online_have_pictrue_pipe = isp_drv_snap_get_pictrue_pipe(drv_ctx);
    if (online_have_pictrue_pipe != HI_TRUE) {
        return;
    }

    if (vi_pipe != drv_ctx->snap_attr.preview_pipe_id) {
        return;
    }

    if (drv_ctx->snap_attr.picture_pipe_id < 0 || drv_ctx->snap_attr.picture_pipe_id > ISP_MAX_PIPE_NUM) {
        isp_err_trace("Err isp pipe %d!\n", drv_ctx->snap_attr.picture_pipe_id);
        return;
    }

    ret = isp_drv_get_be_regs_attr(drv_ctx->snap_attr.picture_pipe_id, snap_be_reg, &isp_snap_be_regs_attr);
    snap_drv_ctx = isp_drv_get_ctx(drv_ctx->snap_attr.picture_pipe_id);
    if (ret == HI_SUCCESS) {
        isp_drv_reg_config4_dgain(drv_ctx->snap_attr.picture_pipe_id, snap_be_reg, snap_drv_ctx, sync_4dgain_cfg);
    }
    return;
}

hi_void isp_drv_reg_config_sync_4dgain(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
    isp_drv_ctx *drv_ctx, isp_awb_reg_cfg_2 *awb_reg_cfg, isp_be_regs_attr *be_regs_attr)
{
    hi_u8   sync_index;
    hi_s32  i;
    hi_u32  wdr_gain;
    isp_sync_4dgain_cfg sync_4dgain_cfg;

    sync_index = isp_drv_get_be_sync_index(drv_ctx);
    sync_index = MIN2(sync_index, CFG2VLD_DLY_LIMIT - 1);

    for (i = 0; i < WDR_CHN_MAX; i++) {
        wdr_gain = drv_ctx->sync_cfg.wdr_gain[i][sync_index];
        sync_4dgain_cfg.wdr_gain[i] = CLIP3(wdr_gain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        sync_4dgain_cfg.wdr_wb_gain[i] = awb_reg_cfg->wdr_wb_gain[i];
        sync_4dgain_cfg.be_white_balance_gain[i] = awb_reg_cfg->be_white_balance_gain[i];
    }

    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            sync_4dgain_cfg.be_white_balance_gain[i] = drv_ctx->snap_info_load.isp_cfg_info.white_balance_gain[i];
        }
    }

    /* online mode double pipe snap, when vipipe == preview_pipe_id, config picture_pipe */
    isp_drv_reg_config_4dgain_online_double_snap(vi_pipe, drv_ctx, &sync_4dgain_cfg);

    isp_drv_reg_config4_dgain(vi_pipe, be_reg, drv_ctx, &sync_4dgain_cfg);
}

hi_void isp_drv_reg_config_sync_piris(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_ae_reg_cfg_2 *ae_reg_cfg)
{
    /* config Piris */
    if (ae_reg_cfg != HI_NULL) {
        if (ae_reg_cfg->piris_valid == HI_TRUE) {
            if (drv_ctx->piris_cb.pfn_piris_gpio_update != HI_NULL) {
                drv_ctx->piris_cb.pfn_piris_gpio_update(vi_pipe, &ae_reg_cfg->piris_pos);
            }
        }
    }
}

static hi_s32 isp_drv_reg_config_sync(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_be_regs_attr *be_regs_attr,
                                      hi_u8 cfg_node_idx, hi_u8 cfg_node_vc)
{
    hi_vi_pipe main_pipe;
    isp_ae_reg_cfg_2  *ae_reg_cfg   = HI_NULL;
    isp_awb_reg_cfg_2 *awb_reg_cfg  = HI_NULL;
    isp_drv_ctx       *drv_ctx_s    = HI_NULL;
    isp_drv_ctx       *drv_ctx      = HI_NULL;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    isp_sync_cfg_buf_node *ccm_cfg_node = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if ((drv_ctx->stitch_attr.stitch_enable == HI_TRUE) && (drv_ctx->stitch_attr.main_pipe == HI_FALSE)) {
        main_pipe    = drv_ctx->stitch_attr.stitch_bind_id[0];
        drv_ctx_s    = isp_drv_get_ctx(main_pipe);
        cfg_node     = drv_ctx_s->sync_cfg.node[cfg_node_idx];
        ccm_cfg_node = drv_ctx_s->sync_cfg.node[0];
    } else {
        cfg_node     = drv_ctx->sync_cfg.node[cfg_node_idx];
        ccm_cfg_node = drv_ctx->sync_cfg.node[0];
    }

    isp_check_pointer_return(cfg_node);

    if (drv_ctx->sync_cfg.vc_cfg_num == cfg_node_vc) {
        ae_reg_cfg  = &cfg_node->ae_reg_cfg;
        awb_reg_cfg = &cfg_node->awb_reg_cfg;

        isp_drv_reg_config_be_dgain(vi_pipe, be_reg, drv_ctx, ae_reg_cfg, be_regs_attr);

        isp_drv_reg_config_sync_4dgain(vi_pipe, be_reg, drv_ctx, awb_reg_cfg, be_regs_attr);

        isp_drv_reg_config_sync_piris(vi_pipe, drv_ctx, ae_reg_cfg);

        isp_drv_reg_config_sync_ccm(vi_pipe, be_reg, drv_ctx, ccm_cfg_node->awb_reg_cfg.color_matrix, be_regs_attr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_isp(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_s32 ret;
    hi_u8  cfg_node_idx, cfg_node_vc, blk_dev, blk_num;
    hi_u32 ratio[EXP_RATIO_NUM] = { 0x40 };
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    isp_be_regs_attr be_regs_attr = { 0 };
    S_ISPBE_REGS_TYPE *be_reg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    hi_bool online_have_pictrue_pipe;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(drv_ctx);

    ret = isp_drv_get_be_regs_attr(vi_pipe, be_reg, &be_regs_attr);
    isp_check_ret_success(vi_pipe, ret, "isp_drv_get_be_regs_attr");

    online_have_pictrue_pipe = isp_drv_snap_get_pictrue_pipe(drv_ctx);

    blk_dev = be_regs_attr.block_dev;
    blk_num = be_regs_attr.block_num;

    cfg_node_idx = MIN2(isp_drv_get_be_sync_index(drv_ctx), CFG2VLD_DLY_LIMIT - 1);

    isp_drv_get_exp_ratio(drv_ctx, cfg_node_idx, ratio);

    cfg_node_vc = isp_drv_get_cfg_node_vc(drv_ctx);

    if (is_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
        /* Channel Switch */
        isp_drv_reg_config_chn_sel(be_reg, drv_ctx, blk_num, blk_dev);
    }

    isp_drv_reg_config_fe_blc(vi_pipe, drv_ctx);

    isp_drv_reg_config_fe_dgain(vi_pipe, drv_ctx);

    cfg_node = drv_ctx->sync_cfg.node[cfg_node_idx];
    isp_check_pointer_success(cfg_node);

    if (drv_ctx->sync_cfg.vc_cfg_num == cfg_node_vc) {
        isp_drv_reg_config_be_blc(vi_pipe, be_reg, drv_ctx, cfg_node, &be_regs_attr);
        isp_drv_reg_config_wdr(vi_pipe, be_reg, &cfg_node->wdr_reg_cfg, &be_regs_attr, ratio);

        /* config Ldci compensation */
        isp_drv_reg_config_ldci(vi_pipe, be_reg, drv_ctx, blk_num, blk_dev);
        isp_drv_reg_config_de(vi_pipe, be_reg, cfg_node);

        /* config drc strength */
        isp_drv_reg_config_drc(vi_pipe, be_reg, &cfg_node->drc_reg_cfg, &be_regs_attr, ratio);

        isp_drv_reg_config_be_regup(vi_pipe);
    }

    /* online mode double pipe snap, when vipipe == preview_pipe_id, config picture_pipe */
    if ((online_have_pictrue_pipe == HI_TRUE) && (vi_pipe == drv_ctx->snap_attr.picture_pipe_id)) {
        return HI_SUCCESS;
    }

    ret = isp_drv_reg_config_sync(vi_pipe, be_reg, &be_regs_attr, cfg_node_idx, cfg_node_vc);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

isp_sync_cfg_buf_node *isp_drv_get_sns_cfg_node(isp_drv_ctx *drv_ctx, hi_u8 delay_frm_num)
{
    hi_u8 wdr_mode, cfg_node_idx, cfg_node_vc;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    isp_sync_cfg_buf_node *pre_cfg_node = HI_NULL;

    wdr_mode = drv_ctx->wdr_cfg.wdr_mode;
    cfg_node_idx = delay_frm_num / div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
    cfg_node_vc  = delay_frm_num % div_0_to_1(drv_ctx->sync_cfg.vc_num_max + 1);
    if (drv_ctx->sync_cfg.vc_cfg_num == cfg_node_vc) {
        if (cfg_node_idx > CFG2VLD_DLY_LIMIT - 1) {
            return HI_NULL;
        }

        cfg_node    = drv_ctx->sync_cfg.node[cfg_node_idx];
        pre_cfg_node = drv_ctx->sync_cfg.node[cfg_node_idx + 1];

        if (cfg_node == HI_NULL) {
            return HI_NULL;
        }

        /* not config sensor when cur == pre */
        if ((pre_cfg_node != HI_NULL) && (cfg_node == pre_cfg_node)) {
            if ((is_linear_mode(wdr_mode)) || (is_built_in_wdr_mode(wdr_mode))) {
                return HI_NULL;
            }
        }
    }

    return cfg_node;
}

hi_s32 isp_drv_write_i2c_data(isp_drv_ctx *drv_ctx, isp_sync_cfg_buf_node *cur_node,
                              hi_u32 update_pos, hi_s8 i2c_dev)
{
    hi_s32 i;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    hi_isp_i2c_data *i2c_data = HI_NULL;

    if (i2c_dev == -1) {
        return HI_SUCCESS;
    }

    if (drv_ctx->bus_cb.pfn_isp_write_i2c_data == HI_NULL) {
        isp_warn_trace("pfn_isp_write_i2c_data is HI_NULL point!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < cur_node->sns_regs_info.reg_num; i++) {
        cfg_node = isp_drv_get_sns_cfg_node(drv_ctx, cur_node->sns_regs_info.i2c_data[i].delay_frm_num);
        if (!cfg_node) {
            continue;
        }

        i2c_data = &cfg_node->sns_regs_info.i2c_data[i];
        if ((i2c_data->update == HI_TRUE) && (update_pos == i2c_data->int_pos)) {
            drv_ctx->bus_cb.pfn_isp_write_i2c_data(i2c_dev, i2c_data->dev_addr,
                                                   i2c_data->reg_addr, i2c_data->addr_byte_num,
                                                   i2c_data->data, i2c_data->data_byte_num);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_write_ssp_data(isp_drv_ctx *drv_ctx, isp_sync_cfg_buf_node *cur_node,
                              hi_u32 update_pos, hi_s8 ssp_dev, hi_s8 ssp_cs)
{
    hi_s32 i;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    hi_isp_ssp_data *ssp_data = HI_NULL;

    if (ssp_dev == -1) {
        return HI_SUCCESS;
    }

    if (drv_ctx->bus_cb.pfn_isp_write_ssp_data == HI_NULL) {
        isp_warn_trace("pfn_isp_write_ssp_data is HI_NULL point!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < cur_node->sns_regs_info.reg_num; i++) {
        cfg_node = isp_drv_get_sns_cfg_node(drv_ctx, cur_node->sns_regs_info.ssp_data[i].delay_frm_num);
        if (!cfg_node) {
            continue;
        }

        ssp_data = &cfg_node->sns_regs_info.ssp_data[i];
        if ((ssp_data->update == HI_TRUE) && (update_pos == ssp_data->int_pos)) {
            drv_ctx->bus_cb.pfn_isp_write_ssp_data(ssp_dev, ssp_cs, ssp_data->dev_addr,
                                                   ssp_data->dev_addr_byte_num, ssp_data->reg_addr,
                                                   ssp_data->reg_addr_byte_num, ssp_data->data,
                                                   ssp_data->data_byte_num);
        }
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
static hi_s32 isp_drv_sticth_regs_cfg_sensor_check(isp_sync_cfg_buf_node *cur_node)
{
    if (cur_node == HI_NULL) {
        isp_warn_trace("NULL point Stitch!\n");
        return HI_FAILURE;
    }
    if (cur_node->valid == HI_FALSE) {
        isp_warn_trace("Invalid Node Stitch!\n");
        return HI_FAILURE;
    }

    if (cur_node->sns_regs_info.reg_num == 0) {
        isp_warn_trace("Err reg_num Stitch!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_stitch_regs_cfg_sensor(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, hi_u32 update_pos)
{
    hi_s32 ret;
    hi_u32 slave_dev;
    hi_vi_pipe main_pipe = 0;
    isp_sync_cfg_buf_node *cur_node = HI_NULL;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    hi_s8 i2c_dev = 0;
    hi_s8 ssp_dev = 0;
    hi_s8 ssp_cs  = 0;

    cur_node = drv_ctx->sync_cfg.node[0];
    ret = isp_drv_sticth_regs_cfg_sensor_check(cur_node);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    slave_dev = cur_node->sns_regs_info.slv_sync.slave_bind_dev;

    if (cur_node->sns_regs_info.sns_type == ISP_SNS_I2C_TYPE) {
        i2c_dev = cur_node->sns_regs_info.com_bus.i2c_dev;
    } else if (cur_node->sns_regs_info.sns_type == ISP_SNS_SSP_TYPE) {
        ssp_dev = cur_node->sns_regs_info.com_bus.ssp_dev.bit4_ssp_dev;
        ssp_cs = cur_node->sns_regs_info.com_bus.ssp_dev.bit4_ssp_cs;
    }

    if (drv_ctx->stitch_attr.main_pipe == HI_FALSE) {
        main_pipe = drv_ctx->stitch_attr.stitch_bind_id[0];
        drv_ctx   = isp_drv_get_ctx(main_pipe);
        cur_node  = drv_ctx->sync_cfg.node[0];
        if (cur_node == HI_NULL) {
            isp_warn_trace("cur_node NULL point Stitch!\n");
            return HI_FAILURE;
        }
    }

    if (cur_node->sns_regs_info.sns_type == ISP_SNS_I2C_TYPE) {
        ret = isp_drv_write_i2c_data(drv_ctx, cur_node, update_pos, i2c_dev);
        isp_check_ret(vi_pipe, ret, "isp_drv_write_i2c_data");
    } else if (cur_node->sns_regs_info.sns_type == ISP_SNS_SSP_TYPE) {
        ret = isp_drv_write_ssp_data(drv_ctx, cur_node, update_pos, ssp_dev, ssp_cs);
        isp_check_ret(vi_pipe, ret, "isp_drv_write_ssp_data");
    }

    /* write slave sns vmax sync */
    cfg_node = isp_drv_get_sns_cfg_node(drv_ctx, cur_node->sns_regs_info.slv_sync.delay_frm_num);
    if (cfg_node == HI_NULL) {
        return HI_FAILURE;
    }

    if ((cfg_node->valid == HI_TRUE) && (cfg_node->sns_regs_info.slv_sync.update == HI_TRUE)) {
        /* adjust the relationship between slavedev and vipipe */
        io_rw_pt_address(vicap_slave_vstime(slave_dev)) = cfg_node->sns_regs_info.slv_sync.slave_vs_time;
    }

    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_normal_regs_cfg_sensor(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_s32 ret;
    hi_u32 slave_dev = 0;
    isp_sync_cfg_buf_node *cur_node = HI_NULL;
    isp_sync_cfg_buf_node *cfg_node = HI_NULL;
    hi_s8 i2c_dev, ssp_dev, ssp_cs;

    cur_node = drv_ctx->sync_cfg.node[0];
    if (cur_node == HI_NULL) {
        isp_warn_trace("NULL point Normal!\n");
        return HI_FAILURE;
    }

    if (cur_node->valid == HI_FALSE) {
        isp_warn_trace("Invalid Node Normal!\n");
        return HI_FAILURE;
    }

    if (cur_node->sns_regs_info.reg_num == 0) {
        isp_warn_trace("Err reg_num Normal!\n");
        return HI_FAILURE;
    }

    if (cur_node->sns_regs_info.sns_type == ISP_SNS_I2C_TYPE) {
        i2c_dev = cur_node->sns_regs_info.com_bus.i2c_dev;

        ret = isp_drv_write_i2c_data(drv_ctx, cur_node, drv_ctx->int_pos, i2c_dev);
        isp_check_ret_success(vi_pipe, ret, "isp_drv_write_i2c_data");
    } else if (cur_node->sns_regs_info.sns_type == ISP_SNS_SSP_TYPE) {
        ssp_dev = cur_node->sns_regs_info.com_bus.ssp_dev.bit4_ssp_dev;
        ssp_cs = cur_node->sns_regs_info.com_bus.ssp_dev.bit4_ssp_cs;

        ret = isp_drv_write_ssp_data(drv_ctx, cur_node, drv_ctx->int_pos, ssp_dev, ssp_cs);
        isp_check_ret_success(vi_pipe, ret, "isp_drv_write_ssp_data");
    }

    /* write slave sns vmax sync */
    cfg_node = isp_drv_get_sns_cfg_node(drv_ctx, cur_node->sns_regs_info.slv_sync.delay_frm_num);
    if (cfg_node == HI_NULL) {
        return HI_FAILURE;
    }

    if ((cfg_node->valid == HI_TRUE) && (cfg_node->sns_regs_info.slv_sync.update == HI_TRUE)) {
        /* adjust the relationship between slavedev and vipipe */
        slave_dev = cfg_node->sns_regs_info.slv_sync.slave_bind_dev;
        io_rw_pt_address(vicap_slave_vstime(slave_dev)) = cfg_node->sns_regs_info.slv_sync.slave_vs_time;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_reg_config_sensor(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_s32 ret;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(drv_ctx);

    if (drv_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        ret = isp_drv_normal_regs_cfg_sensor(vi_pipe, drv_ctx);
        if (ret != HI_SUCCESS) {
            isp_warn_trace("isp_drv_normal_regs_cfg_sensor failure!\n");
            return ret;
        }
    } else {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
        ret = isp_drv_stitch_regs_cfg_sensor(vi_pipe, drv_ctx, drv_ctx->int_pos);
        if (ret != HI_SUCCESS) {
            isp_warn_trace("isp_drv_stitch_regs_cfg_sensor failure!\n");
            return ret;
        }
#endif
    }

    return HI_SUCCESS;
}

hi_s32 isp_register_bus_call_back(hi_vi_pipe vi_pipe,
                                  isp_bus_type type, isp_bus_callback *bus_cb)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pointer_return(bus_cb);
    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (type == ISP_BUS_TYPE_I2C) {
        drv_ctx->bus_cb.pfn_isp_write_i2c_data = bus_cb->pfn_isp_write_i2c_data;
    } else if (type == ISP_BUS_TYPE_SSP) {
        drv_ctx->bus_cb.pfn_isp_write_ssp_data = bus_cb->pfn_isp_write_ssp_data;
    } else {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        osal_printk("The bus type %d registerd to isp is err!", type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    return HI_SUCCESS;
}

hi_s32 isp_register_piris_call_back(hi_vi_pipe vi_pipe, isp_piris_callback *piris_cb)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pointer_return(piris_cb);
    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->piris_cb.pfn_piris_gpio_update = piris_cb->pfn_piris_gpio_update;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_register_vi_bus_call_back(hi_s32 vi_pipe, isp_vibus_callback *vi_bus_cb)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    isp_check_pointer_return(vi_bus_cb);
    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->vi_bus_cb.pfn_isp_be_sta_read = vi_bus_cb->pfn_isp_be_sta_read;
    drv_ctx->vi_bus_cb.pfn_isp_be_cfg_read = vi_bus_cb->pfn_isp_be_cfg_read;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_SNAP_SUPPORT
/* vi get isp_config from isp */
hi_s32 isp_save_snap_config(hi_vi_pipe vi_pipe, hi_isp_config_info *snap_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    hi_u8 cfg_dly_max;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(snap_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    cfg_dly_max = MAX2(drv_ctx->sync_cfg.cfg2_vld_dly_max, 0x2);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (g_update_pos[vi_pipe] == 0) {  /* frame start */
        osal_memcpy(snap_info, &drv_ctx->snap_info_save[cfg_dly_max - 1], sizeof(hi_isp_config_info));
    } else {                 /* frame end */
        osal_memcpy(snap_info, &drv_ctx->snap_info_save[cfg_dly_max - 0x2], sizeof(hi_isp_config_info));
    }
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

/* vi send Proenable */
hi_s32 isp_set_pro_enable(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->pro_enable = HI_TRUE;
    drv_ctx->pro_start  = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    return HI_SUCCESS;
}
#endif

/* vi get pubAttr */
hi_s32 isp_get_pub_attr(hi_vi_pipe vi_pipe, hi_isp_pub_attr *pub_attr)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pub_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (!drv_ctx->pub_attr_ok) {
        return HI_FAILURE;
    }
    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    osal_memcpy(pub_attr, &drv_ctx->proc_pub_info, sizeof(hi_isp_pub_attr));
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_SNAP_SUPPORT
/* vi get ProCtrl */
hi_bool isp_get_pro_ctrl(hi_vi_pipe vi_pipe, isp_pro_ctrl *pro_ctrl)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    hi_u8 cfg_dly_max;
    hi_bool pro_start = HI_FALSE;

    if ((vi_pipe < 0) || (vi_pipe >= ISP_MAX_PIPE_NUM)) {
        isp_err_trace("Err isp pipe %d!\n", vi_pipe);
        return pro_start;
    }

    if (pro_ctrl == HI_NULL) {
        isp_err_trace("Null Pointer!\n");
        return pro_start;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    cfg_dly_max = MAX2(drv_ctx->sync_cfg.cfg2_vld_dly_max, 1);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (g_update_pos[vi_pipe] == 0) {
        pro_ctrl->vc_num = cfg_dly_max;
    } else {
        pro_ctrl->vc_num = cfg_dly_max - 1;
    }
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (drv_ctx->pro_trig_flag == 0x2) {
        pro_start = drv_ctx->pro_start;
    } else {
        pro_start = HI_FALSE;
    }

    return pro_start;
}

hi_s32 isp_set_snap_attr(hi_vi_pipe vi_pipe, isp_snap_attr *snap_attr)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    hi_u8 i;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(snap_attr);

    for (i = 0; i < ISP_MAX_PIPE_NUM; i++) {
        drv_ctx = isp_drv_get_ctx(i);
        if (!drv_ctx->mem_init) {
            continue;
        }

        if ((i == snap_attr->picture_pipe_id) || (i == snap_attr->preview_pipe_id)) {
            osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
            osal_memcpy(&drv_ctx->snap_attr, snap_attr, sizeof(isp_snap_attr));
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        }
    }
    return HI_SUCCESS;
}

static hi_s32 isp_check_pro_nr_attr(hi_isp_nr_auto_attr *nr_attr)
{
    hi_u8 i, j;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            if (nr_attr->chroma_str[j][i] > 0x3) {
                isp_err_trace("Invalid  au8ChromaStr[%d][%d] Input!\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if (nr_attr->coarse_str[j][i] > 0x360) {
                isp_err_trace("Invalid  au16CoarseStr[%d][%d] Input!\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        if (nr_attr->fine_str[i] > 0x80) {
            isp_err_trace("Invalid  au8FineStr[%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (nr_attr->coring_wgt[i] > 0xc80) {
            isp_err_trace("Invalid  au16CoringWgt[%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_check_pro_nr_param(hi_vi_pipe vi_pipe, const hi_isp_pro_bnr_param *pro_nr_param)
{
    hi_s32 ret;
    isp_check_pointer_return(pro_nr_param);

    if (pro_nr_param->nr_auto_attr == HI_NULL) {
        isp_err_trace("NrParam buf address can't null!\n");
        return HI_ERR_ISP_NULL_PTR;
    }

    ret = read_user_linear_space_valid((hi_u8 *)(pro_nr_param->nr_auto_attr), sizeof(hi_isp_nr_auto_attr));
    if (ret == HI_FALSE) {
        isp_err_trace("Pipe:%d pastShpAttr address is invalid!\n", vi_pipe);
        return HI_ERR_ISP_INVALID_ADDR;
    }

    isp_check_bool_return(pro_nr_param->enable);

    if ((pro_nr_param->param_num > PRO_MAX_FRAME_NUM) || (pro_nr_param->param_num < 1)) {
        isp_err_trace("Invalid u32ParamNum value %d! Value range: [1, %d]\n",
                      pro_nr_param->param_num, PRO_MAX_FRAME_NUM);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_pro_nr_param(hi_vi_pipe vi_pipe, const hi_isp_pro_bnr_param *pro_nr_param)
{
    hi_s32 ret;
    hi_u32 param_idex = 0;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_pro_nr_param *pro_nr_param_ctx = HI_NULL;
    hi_isp_nr_auto_attr *pro_nr_attr   = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    ret = isp_check_pro_nr_param(vi_pipe, pro_nr_param);
    isp_check_ret(vi_pipe, ret, "isp_check_pro_nr_param");

    pro_nr_attr = osal_vmalloc(sizeof(hi_isp_nr_auto_attr) * pro_nr_param->param_num);
    isp_check_pointer_goto(pro_nr_attr, ret, HI_ERR_ISP_NOMEM, failed0,
                           "Pipe:%d vmalloc for pro_nr_attr failed!\n", vi_pipe);

    ret = osal_copy_from_user(pro_nr_attr, pro_nr_param->nr_auto_attr,
                              sizeof(hi_isp_nr_auto_attr) * pro_nr_param->param_num);
    isp_check_ret_goto(ret, HI_FAILURE, failed0, "Pipe:%d,copy from user faild !\n", vi_pipe);

    for (param_idex = 0; param_idex < pro_nr_param->param_num; param_idex++) {
        ret = isp_check_pro_nr_attr(&pro_nr_attr[param_idex]);
        isp_check_ret_goto(ret, ret, failed0, "isp_check_pro_nr_attr");
    }

    ret = osal_down_interruptible(&drv_ctx->pro_info_sem);
    isp_check_ret_goto(ret, -ERESTARTSYS, failed0, "osal_down_interruptible");

    if (drv_ctx->pro_info.init == HI_FALSE) {
        isp_warn_trace("pro_info buf doesn't initialized!\n");
        ret = HI_ERR_ISP_NOT_INIT;
        goto failed1;
    }

    isp_check_pointer_goto(drv_ctx->pro_info.pro_nr_param_info.vir_addr, ret, HI_ERR_ISP_NOT_INIT, failed1,
                           "pro_nr_param_info buf don't init ok!\n");

    pro_nr_param_ctx = (isp_pro_nr_param *)drv_ctx->pro_info.pro_nr_param_info.vir_addr;

    pro_nr_param_ctx->enable = pro_nr_param->enable;
    pro_nr_param_ctx->param_num = pro_nr_param->param_num;
    osal_memcpy(&pro_nr_param_ctx->nr_attr, pro_nr_attr, sizeof(hi_isp_nr_auto_attr) * pro_nr_param->param_num);

    osal_up(&drv_ctx->pro_info_sem);

    if (pro_nr_attr != HI_NULL) {
        osal_vfree(pro_nr_attr);
    }

    return HI_SUCCESS;

failed1:
    osal_up(&drv_ctx->pro_info_sem);
failed0:
    if (pro_nr_attr != HI_NULL) {
        osal_vfree(pro_nr_attr);
    }

    return ret;
}

hi_s32 isp_get_pro_nr_param(hi_vi_pipe vi_pipe, hi_isp_pro_bnr_param *pro_nr_param)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_pro_nr_param *pro_nr_param_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pro_nr_param);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    if (pro_nr_param->nr_auto_attr == HI_NULL) {
        isp_err_trace("NrParam buf address can't null!\n");
        return HI_ERR_ISP_NULL_PTR;
    }
    ret = write_user_linear_space_valid((hi_u8 *)(pro_nr_param->nr_auto_attr), sizeof(hi_isp_nr_auto_attr));
    if (ret == HI_FALSE) {
        isp_err_trace("Pipe:%d pastNrAttr address is invalid!\n", vi_pipe);
        return HI_ERR_ISP_INVALID_ADDR;
    }

    if (osal_down_interruptible(&drv_ctx->pro_info_sem)) {
        return -ERESTARTSYS;
    }

    if (drv_ctx->pro_info.init == HI_FALSE) {
        isp_warn_trace("ProInfo buf doesn't initialized!\n");
        osal_up(&drv_ctx->pro_info_sem);
        return HI_ERR_ISP_NOT_INIT;
    }

    if (drv_ctx->pro_info.pro_nr_param_info.vir_addr == HI_NULL) {
        isp_warn_trace("ShpParam buf don't init ok!\n");
        osal_up(&drv_ctx->pro_info_sem);
        return HI_ERR_ISP_NOT_INIT;
    }

    pro_nr_param_ctx = (isp_pro_nr_param *)drv_ctx->pro_info.pro_nr_param_info.vir_addr;

    if (pro_nr_param_ctx->param_num > PRO_MAX_FRAME_NUM) {
        pro_nr_param_ctx->param_num = PRO_MAX_FRAME_NUM;
    }

    if (pro_nr_param_ctx->param_num < 1) {
        pro_nr_param_ctx->param_num = 1;
    }

    pro_nr_param->enable    = pro_nr_param_ctx->enable;
    pro_nr_param->param_num = pro_nr_param_ctx->param_num;

    ret = osal_copy_to_user(pro_nr_param->nr_auto_attr, &pro_nr_param_ctx->nr_attr,
                            sizeof(hi_isp_nr_auto_attr) * pro_nr_param_ctx->param_num);
    if (ret != 0) {
        isp_err_trace("Pipe:%d,copy to user faild !\n", vi_pipe);
        osal_up(&drv_ctx->pro_info_sem);
        return HI_FAILURE;
    }

    osal_up(&drv_ctx->pro_info_sem);

    return HI_SUCCESS;
}

static hi_s32 isp_check_pro_shp_attr(hi_isp_sharpen_auto_attr *shp_attr)
{
    hi_u8 i, j;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            isp_check_max_param_return(shp_attr->texture_str[j][i], 0xFFF,
                "Invalid texture_str[%d][%d]:%d! Range:[0, 4095]\n", j, i, shp_attr->texture_str[j][i]);

            isp_check_max_param_return(shp_attr->edge_str[j][i], 0xFFF,
                "Invalid edge_str[%d][%d]:%d! Range:[0, 4095]\n", j, i, shp_attr->edge_str[j][i]);
        }

        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            isp_check_max_param_return(shp_attr->luma_wgt[j][i], 127, /* Range: [0, 127] */
                "Invalid luma_wgt[%d][%d]:%d! Range:[0, 127]\n", j, i, shp_attr->luma_wgt[j][i]);
        }

        isp_check_max_param_return(shp_attr->texture_freq[i], 0xFFF,
            "Invalid texture_freq[%d]:%d! Range:[0, 4095]\n", i, shp_attr->texture_freq[i]);

        isp_check_max_param_return(shp_attr->edge_freq[i], 0xFFF,
            "Invalid edge_freq[%d]:%d! Range:[0, 4095]\n", i, shp_attr->edge_freq[i]);

        isp_check_max_param_return(shp_attr->over_shoot[i], 127, /* Range: [0, 127] */
            "Invalid over_shoot[%d]:%d! Range:[0, 127]\n", i, shp_attr->over_shoot[i]);

        isp_check_max_param_return(shp_attr->under_shoot[i], 127, /* Range: [0, 127] */
            "Invalid under_shoot[%d]:%d! Range:[0, 127]\n", i, shp_attr->under_shoot[i]);

        isp_check_max_param_return(shp_attr->shoot_sup_adj[i], 15, /* Range: [0, 15] */
            "Invalid shoot_sup_str[%d]:%d! Range:[0, 15]\n", i, shp_attr->shoot_sup_adj[i]);

        isp_check_max_param_return(shp_attr->edge_filt_str[i], 63, /* Range: [0, 63] */
            "Invalid edge_filt_str[%d]:%d! Range:[0, 63]\n", i, shp_attr->edge_filt_str[i]);

        isp_check_max_param_return(shp_attr->edge_filt_max_cap[i], 47, /* Range: [0, 47] */
            "Invalid edge_filt_str[%d]:%d! Range:[0, 47]\n", i, shp_attr->edge_filt_max_cap[i]);

        isp_check_max_param_return(shp_attr->r_gain[i], HI_ISP_SHARPEN_RGAIN,
            "Invalid r_gain[%d]:%d! Range:[0, %d]\n", i, shp_attr->r_gain[i], HI_ISP_SHARPEN_RGAIN);

        isp_check_max_param_return(shp_attr->b_gain[i], HI_ISP_SHARPEN_BGAIN,
            "Invalid b_gain[%d]:%d! Range:[0, %d]\n", i, shp_attr->b_gain[i], HI_ISP_SHARPEN_BGAIN);

        isp_check_max_param_return(shp_attr->skin_gain[i], 31, /* Range: [0, 31] */
            "Invalid skin_gain[%d]:%d! Range:[0, 31]\n", i, shp_attr->skin_gain[i]);

        isp_check_max_param_return(shp_attr->max_sharp_gain[i], 0x7FF,
            "Invalid skin_gain[%d]:%d! Range:[0, 0x7FF]\n", i, shp_attr->max_sharp_gain[i]);

        isp_check_max_param_return(shp_attr->weak_detail_gain[i], 127, /* Range: [0, 127] */
            "Invalid skin_gain[%d]:%d! Range:[0, 127]\n", i, shp_attr->weak_detail_gain[i]);
    }

    return HI_SUCCESS;
}

hi_s32 isp_check_pro_shp_param(hi_vi_pipe vi_pipe, const hi_isp_pro_sharpen_param *pro_shp_param)
{
    hi_s32 ret;

    isp_check_pointer_return(pro_shp_param);

    if (pro_shp_param->shp_auto_attr == HI_NULL) {
        isp_err_trace("ShpParam buf address can't null!\n");
        return HI_ERR_ISP_NULL_PTR;
    }

    ret = read_user_linear_space_valid((hi_u8 *)(pro_shp_param->shp_auto_attr), sizeof(hi_isp_sharpen_auto_attr));
    if (ret == HI_FALSE) {
        isp_err_trace("Pipe:%d pastShpAttr address is invalid!\n", vi_pipe);
        return HI_ERR_ISP_INVALID_ADDR;
    }

    isp_check_bool_return(pro_shp_param->enable);

    if ((pro_shp_param->param_num > PRO_MAX_FRAME_NUM) || (pro_shp_param->param_num < 1)) {
        isp_err_trace("Invalid u32ParamNum value %d! Value range of u32ParamNum is [1, %d]\n",
                      pro_shp_param->param_num, PRO_MAX_FRAME_NUM);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_pro_shp_param(hi_vi_pipe vi_pipe, const hi_isp_pro_sharpen_param *pro_shp_param)
{
    hi_s32 ret;
    hi_u32 param_idex = 0;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_pro_shp_param *pro_shp_param_ctx = HI_NULL;
    hi_isp_sharpen_auto_attr *pro_shp_attr = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    ret = isp_check_pro_shp_param(vi_pipe, pro_shp_param);
    isp_check_ret(vi_pipe, ret, "isp_check_pro_shp_param");

    pro_shp_attr = osal_vmalloc(sizeof(hi_isp_sharpen_auto_attr) * pro_shp_param->param_num);
    isp_check_pointer_goto(pro_shp_attr, ret, HI_ERR_ISP_NOMEM, failed0,
                           "Pipe:%d vmalloc for pastProShpAttr failed!\n", vi_pipe);

    ret = osal_copy_from_user(pro_shp_attr, pro_shp_param->shp_auto_attr,
                              sizeof(hi_isp_sharpen_auto_attr) * pro_shp_param->param_num);
    isp_check_ret_goto(ret, HI_FAILURE,  failed0, "Pipe:%d,copy from user faild !\n", vi_pipe);

    for (param_idex = 0; param_idex < pro_shp_param->param_num; param_idex++) {
        ret = isp_check_pro_shp_attr(&pro_shp_attr[param_idex]);
        isp_check_ret_goto(ret, ret, failed0, "isp_check_pro_shp_attr");
    }

    ret = osal_down_interruptible(&drv_ctx->pro_info_sem);
    isp_check_ret_goto(ret, -ERESTARTSYS, failed0, "osal_down_interruptible");

    if (drv_ctx->pro_info.init == HI_FALSE) {
        isp_warn_trace("pro_info buf doesn't initialized!\n");
        ret = HI_ERR_ISP_NOT_INIT;
        goto failed1;
    }

    isp_check_pointer_goto(drv_ctx->pro_info.pro_shp_param_info.vir_addr, ret, HI_ERR_ISP_NOT_INIT, failed1,
                           "pro_shp_param_info buf don't init ok!\n");

    pro_shp_param_ctx = (isp_pro_shp_param *)drv_ctx->pro_info.pro_shp_param_info.vir_addr;

    pro_shp_param_ctx->enable = pro_shp_param->enable;
    pro_shp_param_ctx->param_num = pro_shp_param->param_num;
    osal_memcpy(&pro_shp_param_ctx->shp_attr, pro_shp_attr,
                sizeof(hi_isp_sharpen_auto_attr) * pro_shp_param->param_num);

    osal_up(&drv_ctx->pro_info_sem);

    if (pro_shp_attr != HI_NULL) {
        osal_vfree(pro_shp_attr);
    }

    return HI_SUCCESS;

failed1:
    osal_up(&drv_ctx->pro_info_sem);
failed0:
    if (pro_shp_attr != HI_NULL) {
        osal_vfree(pro_shp_attr);
    }

    return ret;
}

hi_s32 isp_get_pro_shp_param(hi_vi_pipe vi_pipe, hi_isp_pro_sharpen_param *pro_shp_param)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_pro_shp_param *pro_shp_param_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pro_shp_param);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    if (pro_shp_param->shp_auto_attr == HI_NULL) {
        isp_err_trace("ShpParam buf address can't null!\n");
        return HI_ERR_ISP_NULL_PTR;
    }

    ret = write_user_linear_space_valid((hi_u8 *)(pro_shp_param->shp_auto_attr), sizeof(hi_isp_sharpen_auto_attr));
    if (ret == HI_FALSE) {
        isp_err_trace("Pipe:%d pastShpAttr address is invalid!\n", vi_pipe);
        return HI_ERR_ISP_INVALID_ADDR;
    }

    if (osal_down_interruptible(&drv_ctx->pro_info_sem)) {
        return -ERESTARTSYS;
    }

    if (drv_ctx->pro_info.init == HI_FALSE) {
        isp_warn_trace("ProInfo buf doesn't initialized!\n");
        osal_up(&drv_ctx->pro_info_sem);
        return HI_ERR_ISP_NOT_INIT;
    }

    if (drv_ctx->pro_info.pro_shp_param_info.vir_addr == HI_NULL) {
        isp_warn_trace("ShpParam buf don't init ok!\n");
        osal_up(&drv_ctx->pro_info_sem);
        return HI_ERR_ISP_NOT_INIT;
    }

    pro_shp_param_ctx = (isp_pro_shp_param *)drv_ctx->pro_info.pro_shp_param_info.vir_addr;

    if (pro_shp_param_ctx->param_num > PRO_MAX_FRAME_NUM) {
        pro_shp_param_ctx->param_num = PRO_MAX_FRAME_NUM;
    }

    if (pro_shp_param_ctx->param_num <  1) {
        pro_shp_param_ctx->param_num = 1;
    }

    pro_shp_param->enable    = pro_shp_param_ctx->enable;
    pro_shp_param->param_num = pro_shp_param_ctx->param_num;

    ret = osal_copy_to_user(pro_shp_param->shp_auto_attr, &pro_shp_param_ctx->shp_attr,
                            sizeof(hi_isp_sharpen_auto_attr) * pro_shp_param_ctx->param_num);
    if (ret != 0) {
        isp_err_trace("Pipe:%d,copy to user faild !\n", vi_pipe);
        osal_up(&drv_ctx->pro_info_sem);
        return HI_FAILURE;
    }

    osal_up(&drv_ctx->pro_info_sem);

    return HI_SUCCESS;
}
#endif

static hi_void isp_drv_be_af_offline_statistics_read_end_int(hi_vi_pipe vi_pipe)
{
    hi_u8 blk_num;
    hi_s32 ret;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_stat *stat = HI_NULL;
    isp_stat_info *stat_info = HI_NULL;
    S_ISP_STT_REGS_TYPE *be_stt[ISP_STRIPING_MAX_NUM] = { [0 ...(ISP_STRIPING_MAX_NUM - 1)] = HI_NULL };
    isp_stat_key un_statkey;

    if ((vi_pipe < 0) || (vi_pipe >= ISP_MAX_PIPE_NUM)) {
        return;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->isp_run_flag == HI_FALSE) {
        return;
    }
    if (is_online_mode(drv_ctx->work_mode.running_mode) || is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        return;
    }

    /* read af statistics when offline mode at be end proc interrupt */
    blk_num = isp_drv_get_block_num(vi_pipe);
    blk_num = div_0_to_1(blk_num);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (drv_ctx->statistics_buf.init == HI_FALSE) {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return;
    }

    stat_info = drv_ctx->statistics_buf.act_stat;
    if (stat_info == HI_NULL) {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return;
    }
    stat = (isp_stat *)stat_info->virt_addr;
    if (stat == HI_NULL) {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return;
    }

    ret = isp_drv_be_offline_stt_get(vi_pipe, drv_ctx, be_stt, blk_num, stat);
    if (ret != HI_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return;
    }

    un_statkey.key = stat_info->stat_key.bit32_isr_access;

    if (un_statkey.bit1_be_af_stat) {
        isp_drv_be_af_offline_statistics_read(vi_pipe, stat, be_stt, blk_num);
    }
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
}

hi_s32 isp_drv_be_end_int_proc(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->run_once_ok == HI_TRUE) {
        isp_drv_stat_buf_busy_put(vi_pipe);
        drv_ctx->run_once_ok = HI_FALSE;
    }

    if (drv_ctx->yuv_run_once_ok == HI_TRUE) {
        isp_drv_stat_buf_busy_put(vi_pipe);
        drv_ctx->yuv_run_once_ok = HI_FALSE;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->vd_be_end = HI_TRUE;
    osal_wakeup(&drv_ctx->isp_wait_vd_be_end);
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if ((drv_ctx->run_once_ok == HI_TRUE) || (drv_ctx->yuv_run_once_ok == HI_TRUE)) {
        return HI_SUCCESS;
    }

    isp_drv_be_af_offline_statistics_read_end_int(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_drv_proc_init(hi_vi_pipe vi_pipe, isp_proc_mem *proc_mem)
{
    hi_s32 ret;
    hi_u64 phy_addr;
    hi_u8  *vir_addr = HI_NULL;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(proc_mem);

    if (g_proc_param[vi_pipe] == 0) {
        return HI_SUCCESS;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->porc_mem.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].Proc", vi_pipe);

    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, ISP_PROC_SIZE);
    if (ret != HI_SUCCESS) {
        isp_err_trace("alloc proc buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    ((hi_char *)vir_addr)[0] = '\0';
    ((hi_char *)vir_addr)[ISP_PROC_SIZE - 1] = '\0';

    if (osal_down_interruptible(&drv_ctx->proc_sem)) {
        if (phy_addr != 0) {
            cmpi_mmz_free(phy_addr, vir_addr);
        }
        return -ERESTARTSYS;
    }

    drv_ctx->porc_mem.init      = HI_TRUE;
    drv_ctx->porc_mem.phy_addr  = phy_addr;
    drv_ctx->porc_mem.size      = ISP_PROC_SIZE;
    drv_ctx->porc_mem.virt_addr = (hi_void *)vir_addr;

    osal_memcpy(proc_mem, &drv_ctx->porc_mem, sizeof(isp_proc_mem));

    osal_up(&drv_ctx->proc_sem);

    return HI_SUCCESS;
}

hi_s32 isp_drv_proc_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr;
    hi_u8  *vir_addr = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    if (g_proc_param[vi_pipe] == 0) {
        return HI_SUCCESS;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->porc_mem.init);

    phy_addr = drv_ctx->porc_mem.phy_addr;
    vir_addr = (hi_u8 *)drv_ctx->porc_mem.virt_addr;

    if (osal_down_interruptible(&drv_ctx->proc_sem)) {
        return -ERESTARTSYS;
    }

    drv_ctx->porc_mem.init      = HI_FALSE;
    drv_ctx->porc_mem.phy_addr  = 0;
    drv_ctx->porc_mem.size      = 0;
    drv_ctx->porc_mem.virt_addr = HI_NULL;
    osal_up(&drv_ctx->proc_sem);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_proc_printf(hi_vi_pipe vi_pipe, osal_proc_entry_t *s)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u32 proc_buf_len;
    const hi_char *psz_str = HI_NULL;
    hi_char *psz_buf = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    if (g_proc_param[vi_pipe] == 0) {
        return HI_SUCCESS;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (osal_down_interruptible(&drv_ctx->proc_sem)) {
        return -ERESTARTSYS;
    }

    if (drv_ctx->porc_mem.virt_addr != HI_NULL) {
        psz_buf = osal_kmalloc((PROC_PRT_SLICE_SIZE + 1), osal_gfp_atomic);
        if (!psz_buf) {
            isp_err_trace("isp_drv_proc_printf malloc slice buf err\n");
            osal_up(&drv_ctx->proc_sem);
            return HI_ERR_ISP_NULL_PTR;
        }

        psz_buf[PROC_PRT_SLICE_SIZE] = '\0';
        psz_str = (hi_char *)drv_ctx->porc_mem.virt_addr;
        proc_buf_len = osal_strlen((hi_char *)drv_ctx->porc_mem.virt_addr);

        while (proc_buf_len) {
            osal_strncpy(psz_buf, psz_str, PROC_PRT_SLICE_SIZE);
            osal_seq_printf(s, "%s", psz_buf);
            psz_str += PROC_PRT_SLICE_SIZE;

            if (proc_buf_len < PROC_PRT_SLICE_SIZE) {
                proc_buf_len = 0;
            } else {
                proc_buf_len -= PROC_PRT_SLICE_SIZE;
            }
        }

        osal_kfree((hi_void *)psz_buf);
    }

    osal_up(&drv_ctx->proc_sem);

    return HI_SUCCESS;
}

hi_s32 isp_update_info_sync(hi_vi_pipe vi_pipe, hi_isp_dcf_update_info *isp_update_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    hi_s32 i;
    hi_isp_dcf_update_info *update_info_vir_addr = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (drv_ctx->trans_info.update_info.vir_addr == HI_NULL) {
        isp_warn_trace("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    update_info_vir_addr = (hi_isp_dcf_update_info *)drv_ctx->trans_info.update_info.vir_addr;

    for (i = ISP_MAX_UPDATEINFO_BUF_NUM - 1; i >= 1; i--) {
        osal_memcpy(update_info_vir_addr + i, update_info_vir_addr + i - 1, sizeof(hi_isp_dcf_update_info));
    }
    osal_memcpy(update_info_vir_addr, isp_update_info, sizeof(hi_isp_dcf_update_info));
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_frame_info_sync(hi_vi_pipe vi_pipe, hi_isp_frame_info *ispframe_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    hi_s32 i;
    hi_isp_frame_info *pframe_info_vir_addr = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (drv_ctx->trans_info.frame_info.vir_addr == HI_NULL) {
        isp_warn_trace("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    pframe_info_vir_addr = (hi_isp_frame_info *)drv_ctx->trans_info.frame_info.vir_addr;

    for (i = ISP_MAX_FRAMEINFO_BUF_NUM - 1; i >= 1; i--) {
        osal_memcpy(pframe_info_vir_addr + i, pframe_info_vir_addr + i - 1, sizeof(hi_isp_frame_info));
    }
    osal_memcpy(pframe_info_vir_addr, ispframe_info, sizeof(hi_isp_frame_info));
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_void isp_cal_sync_info_index(hi_vi_pipe vi_pipe, hi_s32 *index)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u8 cfg_dly_max;
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    cfg_dly_max = MAX2(drv_ctx->sync_cfg.cfg2_vld_dly_max, 2); /* cfg_dly_max max 2] */
    if (g_update_pos[vi_pipe] == 0) {
        *index = cfg_dly_max - 1;
    } else {
        *index = cfg_dly_max - 2; /* index [cfg_dly_max - 2] */
    }

    *index = CLIP3(*index, 0, CFG2VLD_DLY_LIMIT - 1);
}

hi_s32 isp_get_dcf_info(hi_vi_pipe vi_pipe, hi_isp_dcf_info *isp_dcf)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_isp_dcf_update_info *isp_update_info = HI_NULL;
    hi_isp_dcf_const_info *isp_dcf_const_info = HI_NULL;
    unsigned long flags;
    hi_s32 index = 0;
    hi_isp_dcf_update_info *update_info_vir_addr = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_dcf);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_tranbuf_init_return(vi_pipe, drv_ctx->trans_info.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (drv_ctx->trans_info.update_info.vir_addr == HI_NULL) {
        isp_warn_trace("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    update_info_vir_addr = (hi_isp_dcf_update_info *)drv_ctx->trans_info.update_info.vir_addr;
    isp_cal_sync_info_index(vi_pipe, &index);

    isp_update_info = update_info_vir_addr + index;

    isp_dcf_const_info = (hi_isp_dcf_const_info *)(update_info_vir_addr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(&isp_dcf->isp_dcf_const_info, isp_dcf_const_info, sizeof(hi_isp_dcf_const_info));
    osal_memcpy(&isp_dcf->isp_dcf_update_info, isp_update_info, sizeof(hi_isp_dcf_update_info));
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_pro_info_buf_init(hi_vi_pipe vi_pipe, isp_pro_info_buf *pro_info)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr;
    hi_u64 size = 0;
    hi_u8  *vir_addr = HI_NULL;
    hi_u32 size_pro_nr_param, size_pro_shp_param;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pro_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->pro_info.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].Pro", vi_pipe);

    size_pro_nr_param  = sizeof(isp_pro_nr_param);
    size_pro_shp_param = sizeof(isp_pro_shp_param);

    size += size_pro_nr_param;
    size += size_pro_shp_param;

    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] alloc ISP Pro info buf err\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size);

    if (osal_down_interruptible(&drv_ctx->pro_info_sem)) {
        if (phy_addr != 0) {
            cmpi_mmz_free(phy_addr, vir_addr);
        }
        return -ERESTARTSYS;
    }
    drv_ctx->pro_info.init = HI_TRUE;

    drv_ctx->pro_info.pro_nr_param_info.phy_addr = phy_addr;
    drv_ctx->pro_info.pro_nr_param_info.vir_addr = (hi_void *)vir_addr;

    drv_ctx->pro_info.pro_shp_param_info.phy_addr =
        drv_ctx->pro_info.pro_nr_param_info.phy_addr + size_pro_nr_param;
    drv_ctx->pro_info.pro_shp_param_info.vir_addr =
        (hi_void *)((hi_u8 *)drv_ctx->pro_info.pro_nr_param_info.vir_addr + size_pro_nr_param);

    osal_memcpy(pro_info, &drv_ctx->pro_info, sizeof(isp_pro_info_buf));

    osal_up(&drv_ctx->pro_info_sem);

    return HI_SUCCESS;
}

hi_s32 isp_drv_pro_info_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr;
    hi_u8  *vir_addr = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->pro_info.init);

    phy_addr = drv_ctx->pro_info.pro_nr_param_info.phy_addr;
    vir_addr = (hi_u8 *)drv_ctx->pro_info.pro_nr_param_info.vir_addr;

    if (osal_down_interruptible(&drv_ctx->pro_info_sem)) {
        return -ERESTARTSYS;
    }

    drv_ctx->pro_info.init                        = HI_FALSE;

    drv_ctx->pro_info.pro_nr_param_info.phy_addr  = 0;
    drv_ctx->pro_info.pro_nr_param_info.vir_addr  = HI_NULL;

    drv_ctx->pro_info.pro_shp_param_info.phy_addr = 0;
    drv_ctx->pro_info.pro_shp_param_info.vir_addr = HI_NULL;

    osal_up(&drv_ctx->pro_info_sem);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_trans_info_buf_init(hi_vi_pipe vi_pipe, isp_trans_info_buf *trans_info)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr, size;
    hi_u8  *vir_addr = HI_NULL;
    hi_u32 size_dng_info, size_update_info, size_frame_info, size_attach_info, size_color_gammut;
    hi_char ac_name[MAX_MMZ_NAME_LEN] = { 0 };
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(trans_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_check_buf_init_return(vi_pipe, drv_ctx->trans_info.init);

    osal_snprintf(ac_name, sizeof(ac_name), "ISP[%d].Trans", vi_pipe);

    size_dng_info     = sizeof(hi_isp_dng_image_static_info);
    size_attach_info  = sizeof(hi_isp_attach_info);
    size_color_gammut = sizeof(hi_isp_colorgammut_info);
    size_frame_info   = sizeof(hi_isp_frame_info) * ISP_MAX_FRAMEINFO_BUF_NUM;
    size_update_info  = sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info);

    size = size_dng_info + size_attach_info + size_color_gammut + size_frame_info + size_update_info;

    ret = cmpi_mmz_malloc_nocache(HI_NULL, ac_name, &phy_addr, (hi_void **)&vir_addr, size);
    if (ret != HI_SUCCESS) {
        isp_err_trace("ISP[%d] alloc ISP Trans info buf err\n", vi_pipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(vir_addr, 0, size);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->trans_info.init               = HI_TRUE;

    drv_ctx->trans_info.dng_info.phy_addr  = phy_addr;
    drv_ctx->trans_info.dng_info.vir_addr  = (hi_void *)vir_addr;

    drv_ctx->trans_info.atta_info.phy_addr = drv_ctx->trans_info.dng_info.phy_addr + size_dng_info;
    drv_ctx->trans_info.atta_info.vir_addr =
        (hi_void *)((hi_u8 *)drv_ctx->trans_info.dng_info.vir_addr + size_dng_info);

    drv_ctx->trans_info.color_gammut_info.phy_addr =
        drv_ctx->trans_info.atta_info.phy_addr + size_attach_info;
    drv_ctx->trans_info.color_gammut_info.vir_addr =
        (hi_void *)((hi_u8 *)drv_ctx->trans_info.atta_info.vir_addr + size_attach_info);

    drv_ctx->trans_info.frame_info.phy_addr =
        drv_ctx->trans_info.color_gammut_info.phy_addr + size_color_gammut;
    drv_ctx->trans_info.frame_info.vir_addr =
        (hi_void *)((hi_u8 *)drv_ctx->trans_info.color_gammut_info.vir_addr + size_color_gammut);

    drv_ctx->trans_info.update_info.phy_addr =
        drv_ctx->trans_info.frame_info.phy_addr + size_frame_info;
    drv_ctx->trans_info.update_info.vir_addr =
        (hi_void *)((hi_u8 *)drv_ctx->trans_info.frame_info.vir_addr + size_frame_info);

    osal_memcpy(trans_info, &drv_ctx->trans_info, sizeof(isp_trans_info_buf));

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_trans_info_buf_exit(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u64 phy_addr;
    hi_u8  *vir_addr = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_exit_state_return(vi_pipe, drv_ctx->isp_run_flag);

    isp_check_buf_exit_return(vi_pipe, drv_ctx->trans_info.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    phy_addr = drv_ctx->trans_info.dng_info.phy_addr;
    vir_addr = (hi_u8 *)drv_ctx->trans_info.dng_info.vir_addr;

    drv_ctx->trans_info.init                       = HI_FALSE;

    drv_ctx->trans_info.dng_info.phy_addr          = 0;
    drv_ctx->trans_info.dng_info.vir_addr          = HI_NULL;

    drv_ctx->trans_info.atta_info.phy_addr         = 0;
    drv_ctx->trans_info.atta_info.vir_addr         = HI_NULL;

    drv_ctx->trans_info.color_gammut_info.phy_addr = 0;
    drv_ctx->trans_info.color_gammut_info.vir_addr = HI_NULL;

    drv_ctx->trans_info.frame_info.phy_addr        = 0;
    drv_ctx->trans_info.frame_info.vir_addr        = HI_NULL;

    drv_ctx->trans_info.update_info.phy_addr       = 0;
    drv_ctx->trans_info.update_info.vir_addr       = HI_NULL;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    if (phy_addr != 0) {
        cmpi_mmz_free(phy_addr, vir_addr);
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_actual_frame_info(hi_vi_pipe vi_pipe, hi_isp_frame_info *isp_frame)
{
    hi_u8 vi_pipes;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;
    unsigned long flags;
    hi_s32 index = 0;
    hi_isp_frame_info *frame_info_vir_addr = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_frame);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_tranbuf_init_return(vi_pipe, drv_ctx->trans_info.init);

    if ((drv_ctx->mem_init == HI_FALSE) && (drv_ctx->wdr_attr.wdr_mode == WDR_MODE_NONE)) {
        for (vi_pipes = 0; vi_pipes < ISP_MAX_PIPE_NUM; vi_pipes++) {
            drv_ctx_s = isp_drv_get_ctx(vi_pipes);
            if ((drv_ctx_s->mem_init == HI_FALSE) || (!is_wdr_mode(drv_ctx_s->wdr_attr.wdr_mode))) {
                continue;
            }

            osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

            if (isp_frame && drv_ctx_s->trans_info.frame_info.vir_addr) {
                frame_info_vir_addr = (hi_isp_frame_info *)drv_ctx_s->trans_info.frame_info.vir_addr;
                isp_cal_sync_info_index(vi_pipe, &index);
                osal_memcpy(isp_frame, frame_info_vir_addr + index, sizeof(hi_isp_frame_info));
            }

            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        }
    } else {
        osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

        if (isp_frame && drv_ctx->trans_info.frame_info.vir_addr) {
            frame_info_vir_addr = (hi_isp_frame_info *)drv_ctx->trans_info.frame_info.vir_addr;
            isp_cal_sync_info_index(vi_pipe, &index);
            osal_memcpy(isp_frame, frame_info_vir_addr + index, sizeof(hi_isp_frame_info));
        }

        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    }

    return HI_SUCCESS;
}

hi_s32 isp_get_frame_info(hi_vi_pipe vi_pipe, hi_isp_frame_info *isp_frame)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_vi_pipe base_pipe = vi_pipe;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_frame);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->ai_info.pq_ai_en == HI_TRUE) {
        base_pipe = drv_ctx->ai_info.base_pipe_id;
    }

    return isp_get_actual_frame_info(base_pipe, isp_frame);
}

hi_s32 isp_get_pq_info(hi_vi_pipe vi_pipe, pq_ai_attr *pq_info)
{
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pq_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    osal_memcpy(pq_info, &drv_ctx->ai_info, sizeof(pq_ai_attr));
    return HI_SUCCESS;
}
hi_s32 isp_get_attach_info(hi_vi_pipe vi_pipe, hi_isp_attach_info *isp_attach_info)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_attach_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_tranbuf_init_return(vi_pipe, drv_ctx->trans_info.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (isp_attach_info && drv_ctx->trans_info.atta_info.vir_addr) {
        osal_memcpy(isp_attach_info, (hi_isp_attach_info *)drv_ctx->trans_info.atta_info.vir_addr,
                    sizeof(hi_isp_attach_info));
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_get_color_gamut_info(hi_vi_pipe vi_pipe, hi_isp_colorgammut_info *isp_color_gamut_info)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_color_gamut_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_tranbuf_init_return(vi_pipe, drv_ctx->trans_info.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (isp_color_gamut_info && drv_ctx->trans_info.color_gammut_info.vir_addr) {
        osal_memcpy(isp_color_gamut_info, (hi_isp_colorgammut_info *)drv_ctx->trans_info.color_gammut_info.vir_addr,
                    sizeof(hi_isp_colorgammut_info));
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_dng_info(hi_vi_pipe vi_pipe, hi_isp_dng_image_static_info *dng_info)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(dng_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_tranbuf_init_return(vi_pipe, drv_ctx->trans_info.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    if (dng_info && drv_ctx->trans_info.dng_info.vir_addr) {
        osal_memcpy(dng_info, (hi_isp_dng_image_static_info *)drv_ctx->trans_info.dng_info.vir_addr,
                    sizeof(hi_isp_dng_image_static_info));
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_dng_image_dynamic_info(hi_vi_pipe vi_pipe, hi_dng_image_dynamic_info *dng_image_dynamic_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    if (g_update_pos[vi_pipe] == 0) {  /* frame start */
        osal_memcpy(dng_image_dynamic_info, &drv_ctx->dng_image_dynamic_info[1], sizeof(hi_dng_image_dynamic_info));
    } else {
        osal_memcpy(dng_image_dynamic_info, &drv_ctx->dng_image_dynamic_info[0], sizeof(hi_dng_image_dynamic_info));
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_mod_param(hi_isp_mod_param *mod_param)
{
    hi_vi_pipe vi_pipe;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pointer_return(mod_param);

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        drv_ctx = isp_drv_get_ctx(vi_pipe);
        if (drv_ctx->mem_init == HI_TRUE) {
            isp_err_trace("Does not support changed after isp init!\n");
            return HI_ERR_ISP_NOT_SUPPORT;
        }
    }

    if ((mod_param->int_bot_half != 0) && (mod_param->int_bot_half != 1)) {
        isp_err_trace("u32IntBotHalf must be 0 or 1.\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((mod_param->quick_start != 0) && (mod_param->quick_start != 1)) {
        isp_err_trace("u32QuickStart must be 0 or 1.\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((mod_param->long_frm_int_en != 0) && (mod_param->long_frm_int_en != 1)) {
        isp_err_trace("long_frm_int_en must be 0 or 1.\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    g_int_bottom_half = mod_param->int_bot_half;
    g_quick_start     = mod_param->quick_start;
    g_long_frm_int_en = mod_param->long_frm_int_en;
#ifndef __HuaweiLite__
    if (g_int_bottom_half) {
        g_use_bottom_half = HI_TRUE;
    }
#else
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    if (g_int_bottom_half) {
        g_use_bottom_half = HI_TRUE;
    }
#endif
#endif

    return HI_SUCCESS;
}

hi_s32 isp_get_mod_param(hi_isp_mod_param *mod_param)
{
    isp_check_pointer_return(mod_param);

    mod_param->int_bot_half    = g_int_bottom_half;
    mod_param->quick_start     = g_quick_start;
    mod_param->long_frm_int_en = g_long_frm_int_en;
    return HI_SUCCESS;
}

static hi_void isp_drv_update_ctrl_param(hi_vi_pipe vi_pipe, hi_isp_ctrl_param *isp_ctrl_param)
{
    g_proc_param[vi_pipe]      = isp_ctrl_param->proc_param;
    g_stat_intvl[vi_pipe]      = isp_ctrl_param->stat_intvl;
    g_update_pos[vi_pipe]      = isp_ctrl_param->update_pos;
    g_int_timeout[vi_pipe]     = isp_ctrl_param->int_time_out;
    g_pwm_number[vi_pipe]      = isp_ctrl_param->pwm_number;
    g_port_int_delay[vi_pipe]  = isp_ctrl_param->port_int_delay;
    g_ldci_tpr_flt_en[vi_pipe] = isp_ctrl_param->ldci_tpr_flt_en;
    g_be_buf_num[vi_pipe]      = isp_ctrl_param->be_buf_num;
}

hi_s32 isp_set_ctrl_param(hi_vi_pipe vi_pipe, hi_isp_ctrl_param *isp_ctrl_param)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_ctrl_param);
    isp_check_bool_return(isp_ctrl_param->ldci_tpr_flt_en);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    /* isp proc can be closed before mem init */
    /* if isp proc is opened, cannot change proc_param to 0 after mem init */
    if ((g_proc_param[vi_pipe] != 0) && (isp_ctrl_param->proc_param == 0) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d proc_param do not support to change %d to 0.\n", vi_pipe, g_proc_param[vi_pipe]);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    /* if isp proc is closed, cannot change proc_param to non-0 after mem init */
    if ((g_proc_param[vi_pipe] == 0) && (isp_ctrl_param->proc_param != 0) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d proc_param do not support to change %d to Non-0.\n", vi_pipe, g_proc_param[vi_pipe]);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if (!isp_ctrl_param->stat_intvl) {
        isp_err_trace("Vipipe:%d stat_intvl must be larger than 0.\n", vi_pipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((isp_ctrl_param->update_pos != 0) && (isp_ctrl_param->update_pos != 1)) {
        isp_err_trace("Vipipe:%d update_pos must be 0 or 1.\n", vi_pipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((g_update_pos[vi_pipe] != isp_ctrl_param->update_pos) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d Does not support changed after isp init (update_pos)!\n", vi_pipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if ((g_pwm_number[vi_pipe] != isp_ctrl_param->pwm_number) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d Does not support changed after isp init (pwm_number)!\n", vi_pipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if ((g_port_int_delay[vi_pipe] != isp_ctrl_param->port_int_delay) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d Does not support changed after isp init (port_int_delay)!\n", vi_pipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if ((g_ldci_tpr_flt_en[vi_pipe] != isp_ctrl_param->ldci_tpr_flt_en) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d Does not support changed after isp init (ldci_tpr_flt_en)!\n", vi_pipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if (isp_ctrl_param->be_buf_num < ISP_BE_BUF_NUM_MIN || isp_ctrl_param->be_buf_num > ISP_BE_BUF_NUM_MAX) {
        isp_err_trace("err be_buf_num, range:[%d, %d]\n", ISP_BE_BUF_NUM_MIN, ISP_BE_BUF_NUM_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((g_be_buf_num[vi_pipe] != isp_ctrl_param->be_buf_num) && (drv_ctx->mem_init == HI_TRUE)) {
        isp_err_trace("Vipipe:%d Does not support changed after isp init (be_buf_num)!\n", vi_pipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    isp_drv_update_ctrl_param(vi_pipe, isp_ctrl_param);

    return HI_SUCCESS;
}

hi_s32 isp_get_ctrl_param(hi_vi_pipe vi_pipe, hi_isp_ctrl_param *isp_ctrl_param)
{
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_ctrl_param);

    isp_ctrl_param->proc_param      = g_proc_param[vi_pipe];
    isp_ctrl_param->stat_intvl      = g_stat_intvl[vi_pipe];
    isp_ctrl_param->update_pos      = g_update_pos[vi_pipe];
    isp_ctrl_param->int_time_out    = g_int_timeout[vi_pipe];
    isp_ctrl_param->pwm_number      = g_pwm_number[vi_pipe];
    isp_ctrl_param->port_int_delay  = g_port_int_delay[vi_pipe];
    isp_ctrl_param->ldci_tpr_flt_en = g_ldci_tpr_flt_en[vi_pipe];
    isp_ctrl_param->be_buf_num      = g_be_buf_num[vi_pipe];

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32 isp_drv_stitch_sync_ex(hi_vi_pipe vi_pipe)
{
    hi_u8 k;
    hi_vi_pipe vi_pipe_id;
    isp_drv_ctx *drv_ctx  = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (k = 0; k < drv_ctx->stitch_attr.stitch_pipe_num; k++) {
        vi_pipe_id = drv_ctx->stitch_attr.stitch_bind_id[k];
        drv_ctx_s = isp_drv_get_ctx(vi_pipe_id);
        if (drv_ctx_s->stitch_sync != HI_TRUE) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_stitch_sync(hi_vi_pipe vi_pipe)
{
    hi_u8 k;
    hi_vi_pipe vi_pipe_id;
    isp_drv_ctx *drv_ctx  = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (k = 0; k < drv_ctx->stitch_attr.stitch_pipe_num; k++) {
        vi_pipe_id = drv_ctx->stitch_attr.stitch_bind_id[k];
        drv_ctx_s = isp_drv_get_ctx(vi_pipe_id);
        if (drv_ctx_s->isp_init != HI_TRUE) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_get_version(isp_version *version)
{
    isp_check_pointer_return(version);

    osal_memcpy(&g_isp_lib_info, version, sizeof(isp_version));

    g_isp_lib_info.magic = VERSION_MAGIC + ISP_MAGIC_OFFSET;
    osal_memcpy(version, &g_isp_lib_info, sizeof(isp_version));

    return HI_SUCCESS;
}

hi_s32 isp_drv_set_dng_info(hi_vi_pipe vi_pipe, hi_dng_image_dynamic_info *dng_img_dyn_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(dng_img_dyn_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_memcpy(&drv_ctx->dng_image_dynamic_info[1], &drv_ctx->dng_image_dynamic_info[0],
                sizeof(hi_dng_image_dynamic_info));
    osal_memcpy(&drv_ctx->dng_image_dynamic_info[0], dng_img_dyn_info,
                sizeof(hi_dng_image_dynamic_info));

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_user_stat_buf(hi_vi_pipe vi_pipe, isp_stat_info *stat)
{
    hi_s32 ret;
    isp_stat_info *stat_info = HI_NULL;
    isp_check_pointer_return(stat);

    ret = isp_drv_stat_buf_user_get(vi_pipe, &stat_info);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    if (stat_info == HI_NULL) {
        return HI_FAILURE;
    }

    osal_memcpy(stat, stat_info, sizeof(isp_stat_info));

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_rgbir_format(hi_vi_pipe vi_pipe, hi_isp_ir_bayer_format *rgbir_format)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(rgbir_format);
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (!drv_ctx->isp_init) {
        return HI_FAILURE;
    }

    *rgbir_format = drv_ctx->isp_rgbir_format;
    return HI_SUCCESS;
}
hi_s32 isp_drv_get_stat_info_active(hi_vi_pipe vi_pipe, isp_stat_info *stat_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_stat_info act_stat_info;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stat_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (!drv_ctx->statistics_buf.act_stat) {
        isp_warn_trace("Pipe[%d] get statistic active buffer err, stat not ready!\n", vi_pipe);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    osal_memcpy(&act_stat_info, drv_ctx->statistics_buf.act_stat, sizeof(isp_stat_info));
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    osal_memcpy(stat_info, &act_stat_info, sizeof(isp_stat_info));

    return HI_SUCCESS;
}

hi_s32 isp_drv_set_reg_kernel_cfgs(hi_vi_pipe vi_pipe, isp_kernel_reg_cfg *reg_kernel_cfg)
{
    hi_u32 flag;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(reg_kernel_cfg);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if ((drv_ctx->reg_cfg_info_flag != 0) &&
        (drv_ctx->reg_cfg_info_flag != 1)) {
        isp_err_trace("Pipe[%d] Err u32RegCfgInfoFlag != 0/1 !!!\n", vi_pipe);
    }

    flag = 1 - drv_ctx->reg_cfg_info_flag;
    osal_memcpy(&drv_ctx->kernel_cfg[flag], reg_kernel_cfg, sizeof(isp_kernel_reg_cfg));

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    drv_ctx->reg_cfg_info_flag = flag;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_set_config_info(hi_vi_pipe vi_pipe, hi_isp_config_info *isp_config_info)
{
    hi_u32 i;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(isp_config_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    for (i = ISP_SAVEINFO_MAX - 1; i >= 1; i--) {
        osal_memcpy(&drv_ctx->snap_info_save[i], &drv_ctx->snap_info_save[i - 1], sizeof(hi_isp_config_info));
    }

    osal_memcpy(&drv_ctx->snap_info_save[0], isp_config_info, sizeof(hi_isp_config_info));

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_snap_info(hi_vi_pipe vi_pipe, isp_snap_info *snap_info)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(snap_info);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_memcpy(snap_info, &drv_ctx->snap_info_load, sizeof(isp_snap_info));

    if (drv_ctx->snap_info_load.snap_state == SNAP_STATE_CFG) {
        if (g_update_pos[vi_pipe] == 0) {
            drv_ctx->snap_info_load.snap_state = SNAP_STATE_NULL;
        }
    }

    return HI_SUCCESS;
}

hi_void isp_drv_be_buf_queue_put_busy(hi_vi_pipe vi_pipe)
{
    hi_u64  phy_addr;
    hi_u64  size;
    hi_void *vir_addr = HI_NULL;
    isp_be_buf_node *node = HI_NULL;
    struct osal_list_head *list_tmp  = HI_NULL;
    struct osal_list_head *list_node = HI_NULL;
    isp_drv_ctx *drv_ctx  = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (!drv_ctx->use_node) {
        return;
    }

    osal_list_for_each_safe(list_node, list_tmp, &drv_ctx->be_buf_queue.busy_list) {
        node = osal_list_entry(list_node, isp_be_buf_node, list);
        if (node->hold_cnt == 0) {
            isp_queue_del_busy_be_buf(&drv_ctx->be_buf_queue, node);
            isp_queue_put_free_be_buf(&drv_ctx->be_buf_queue, node);
        }
    }

    phy_addr = drv_ctx->use_node->be_cfg_buf.phy_addr;
    vir_addr   = drv_ctx->use_node->be_cfg_buf.vir_addr;
    size    = drv_ctx->use_node->be_cfg_buf.size;

    osal_flush_dcache_area(vir_addr, phy_addr, size);

    isp_queue_put_busy_be_buf(&drv_ctx->be_buf_queue, drv_ctx->use_node);

    drv_ctx->use_node = HI_NULL;

    return;
}

hi_s32 isp_drv_run_once_process(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    unsigned long flags;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_sync_cfg *sync_cfg = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->mem_init == HI_FALSE) {
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    sync_cfg = &drv_ctx->sync_cfg;

    ret = isp_drv_get_sync_controlnfo(vi_pipe, sync_cfg);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_get_sync_controlnfo");
    ret = isp_drv_calc_sync_cfg(sync_cfg);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_calc_sync_cfg");
    ret = isp_drv_reg_config_isp(vi_pipe, drv_ctx);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_isp");
    ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

    isp_update_info_sync(vi_pipe, &drv_ctx->update_info);
    isp_frame_info_sync(vi_pipe, &drv_ctx->frame_info);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (drv_ctx->run_once_flag == HI_TRUE) {
        isp_drv_be_buf_queue_put_busy(vi_pipe);
    }
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->run_once_ok = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_drv_yuv_run_once_process(hi_vi_pipe vi_pipe)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->mem_init == HI_FALSE) {
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (drv_ctx->yuv_run_once_flag == HI_TRUE) {
        isp_drv_be_buf_queue_put_busy(vi_pipe);
    }
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->yuv_run_once_ok = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_drv_opt_run_once_info(hi_vi_pipe vi_pipe, hi_bool *run_once)
{
    hi_bool run_once_flag;
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(run_once);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    run_once_flag = *run_once;

    if (run_once_flag == HI_TRUE) {
        if (drv_ctx->use_node) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("Pipe[%d] isp is running!\r\n", vi_pipe);
            return HI_FAILURE;
        }

        drv_ctx->use_node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);

        if (drv_ctx->use_node == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("Pipe[%d] get FreeBeBuf is fail!\r\n", vi_pipe);

            return HI_FAILURE;
        }
    }

    drv_ctx->run_once_flag = run_once_flag;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_yuv_run_once_info(hi_vi_pipe vi_pipe, hi_bool *run_once)
{
    hi_bool yuv_run_once_flag;
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(run_once);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    yuv_run_once_flag = *run_once ;

    if (yuv_run_once_flag == HI_TRUE) {
        if (drv_ctx->use_node) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("Pipe[%d] isp is running!\r\n", vi_pipe);
            return HI_FAILURE;
        }

        drv_ctx->use_node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);

        if (drv_ctx->use_node == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("Pipe[%d] get FreeBeBuf is fail!\r\n", vi_pipe);

            return HI_FAILURE;
        }
    }

    drv_ctx->yuv_run_once_flag = yuv_run_once_flag;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
static hi_void isp_drv_stitch_be_buf_ctl(hi_vi_pipe vi_pipe)
{
    hi_s32  i;
    hi_s32  ret;
    hi_vi_pipe vi_pipes;
    hi_vi_pipe main_pipe;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;
    unsigned long flags;

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    main_pipe = drv_ctx->stitch_attr.stitch_bind_id[0];

    osal_spin_lock_irqsave(&g_isp_sync_lock[main_pipe], &flags);

    if (drv_ctx->running_state != ISP_BE_BUF_STATE_RUNNING) {
        osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
        return;
    }

    drv_ctx->running_state = ISP_BE_BUF_STATE_FINISH;

    ret = isp_drv_stitch_sync(vi_pipe);
    if (ret != HI_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
        return;
    }

    for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
        vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
        drv_ctx_s = isp_drv_get_ctx(vi_pipes);
        if (drv_ctx_s->running_state != ISP_BE_BUF_STATE_FINISH) {
            osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
            return;
        }
    }

    for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
        vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
        drv_ctx_s = isp_drv_get_ctx(vi_pipes);
        if (drv_ctx_s->be_buf_info.init != HI_TRUE) {
            isp_err_trace("Pipe[%d] BeBuf (bInit != TRUE) !\n", vi_pipe);
            osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
            return;
        }

        if (drv_ctx_s->run_once_flag != HI_TRUE) {
            isp_drv_be_buf_queue_put_busy(vi_pipes);
        }
        drv_ctx_s->running_state = ISP_BE_BUF_STATE_INIT;
    }

    osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);

    return;
}
#endif

hi_s32 isp_drv_write_all_ldci_stt_addr(hi_vi_pipe vi_pipe)
{
    hi_u8  i, k, write_buf_idx, blk_num, free_num, write_buf_num;
    hi_u32 size;
    hi_u64 write_stt_head_addr, write_stt_addr;
    isp_be_wo_reg_cfg *be_reg_cfg = HI_NULL;
    isp_be_buf_node   *node       = HI_NULL;
    isp_drv_ctx       *drv_ctx    = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    blk_num      = drv_ctx->work_mode.block_num;
    size       = drv_ctx->ldci_write_buf_attr.ldci_buf[0].size;
    write_buf_num = drv_ctx->ldci_write_buf_attr.buf_num;
    write_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;
    write_stt_head_addr = drv_ctx->ldci_write_buf_attr.ldci_buf[write_buf_idx].phy_addr;

    be_reg_cfg = (isp_be_wo_reg_cfg *)drv_ctx->use_node->be_cfg_buf.vir_addr;

    for (i = 0; i < blk_num; i++) {
        write_stt_addr = write_stt_head_addr + i * (size / div_0_to_1(blk_num));
        isp_drv_set_ldci_write_stt_addr(&be_reg_cfg->be_reg_cfg[i].vi_proc_reg, write_stt_addr);
    }

    drv_ctx->ldci_write_buf_attr.buf_idx = (write_buf_idx + 1) % div_0_to_1(write_buf_num);

    free_num = isp_queue_get_free_num(&drv_ctx->be_buf_queue);

    for (k = 0; k < free_num; k++) {
        node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);
        if (node == HI_NULL) {
            isp_err_trace("ISP[%d] Get QueueGetFreeBeBuf fail!\r\n", vi_pipe);
            return HI_FAILURE;
        }

        be_reg_cfg = (isp_be_wo_reg_cfg *)node->be_cfg_buf.vir_addr;
        write_buf_idx = drv_ctx->ldci_write_buf_attr.buf_idx;
        write_stt_head_addr = drv_ctx->ldci_write_buf_attr.ldci_buf[write_buf_idx].phy_addr;

        for (i = 0; i < blk_num; i++) {
            write_stt_addr = write_stt_head_addr + i * (size / div_0_to_1(blk_num));
            isp_drv_set_ldci_write_stt_addr(&be_reg_cfg->be_reg_cfg[i].vi_proc_reg, write_stt_addr);
        }

        drv_ctx->ldci_write_buf_attr.buf_idx = (write_buf_idx + 1) % div_0_to_1(write_buf_num);
        isp_queue_put_free_be_buf(&drv_ctx->be_buf_queue, node);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_write_be_free_buf(hi_vi_pipe vi_pipe)
{
    hi_s32 i;
    hi_s32 free_num, ret;
    isp_running_mode running_mode;
    isp_be_buf_node *node = HI_NULL;
    isp_be_wo_reg_cfg *be_reg_cfg_src = HI_NULL;
    isp_be_wo_reg_cfg *be_reg_cfg_dst = HI_NULL;
    isp_drv_ctx *drv_ctx  = HI_NULL;
    hi_u64  phy_addr;
    hi_u64  size;
    hi_void *vir_addr = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    if (drv_ctx->use_node == HI_NULL) {
        isp_err_trace("Pipe[%d] pstCurNode is null for init!\r\n", vi_pipe);
        return HI_FAILURE;
    }

    be_reg_cfg_src   = drv_ctx->use_node->be_cfg_buf.vir_addr;
    running_mode = drv_ctx->work_mode.running_mode;

    free_num = isp_queue_get_free_num(&drv_ctx->be_buf_queue);

    for (i = 0; i < free_num; i++) {
        node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);
        if (node == HI_NULL) {
            isp_err_trace("Pipe[%d] Get QueueGetFreeBeBuf fail!\r\n", vi_pipe);
            return HI_FAILURE;
        }

        be_reg_cfg_dst = (isp_be_wo_reg_cfg *)node->be_cfg_buf.vir_addr;

        if ((running_mode == ISP_MODE_RUNNING_SIDEBYSIDE) || (running_mode == ISP_MODE_RUNNING_STRIPING)) {
            osal_memcpy(be_reg_cfg_dst, be_reg_cfg_src, sizeof(isp_be_wo_reg_cfg));
        } else {
            osal_memcpy(&be_reg_cfg_dst->be_reg_cfg[0], &be_reg_cfg_src->be_reg_cfg[0], sizeof(S_ISPBE_REGS_TYPE));
        }

        phy_addr = drv_ctx->use_node->be_cfg_buf.phy_addr;
        vir_addr = drv_ctx->use_node->be_cfg_buf.vir_addr;
        size = drv_ctx->use_node->be_cfg_buf.size;

        osal_flush_dcache_area(vir_addr, phy_addr, size);

        isp_queue_put_free_be_buf(&drv_ctx->be_buf_queue, node);
    }

    ret = isp_drv_write_all_ldci_stt_addr(vi_pipe);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_buf_switch_finish_state(hi_vi_pipe vi_pipe)
{
    hi_s32  i;
    hi_vi_pipe vi_pipes;
    isp_drv_ctx *drv_ctx_s = HI_NULL;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    unsigned long flags;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);
    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        if (drv_ctx->running_state != ISP_BE_BUF_STATE_SWITCH_START) {
            osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
            isp_warn_trace("Pipe[%d] isp isn't init state!\n", vi_pipe);
            return HI_FAILURE;
        }

        drv_ctx->running_state = ISP_BE_BUF_STATE_SWITCH;

        for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
            vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
            drv_ctx_s = isp_drv_get_ctx(vi_pipes);
            if (drv_ctx_s->running_state != ISP_BE_BUF_STATE_SWITCH) {
                osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
                isp_warn_trace("Pipe[%d] isp isn't  finish state!\n", vi_pipe);
                return HI_FAILURE;
            }
        }

        for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
            vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
            drv_ctx_s = isp_drv_get_ctx(vi_pipes);
            drv_ctx_s->running_state = ISP_BE_BUF_STATE_INIT;
        }
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
static hi_s32 isp_drv_stitch_write_be_buf_all(hi_vi_pipe vi_pipe)
{
    hi_s32 i, ret;
    hi_vi_pipe vi_pipes, main_pipe;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    main_pipe = drv_ctx->stitch_attr.stitch_bind_id[0];

    ret = isp_drv_write_be_free_buf(vi_pipe);
    isp_check_ret(vi_pipe, ret, "isp_drv_write_be_free_buf");

    osal_spin_lock_irqsave(&g_isp_sync_lock[main_pipe], &flags);

    ret = isp_drv_stitch_sync_ex(vi_pipe);
    if (ret != HI_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);

        return HI_SUCCESS;
    }

    if (drv_ctx->running_state == ISP_BE_BUF_STATE_SWITCH_START) {
        drv_ctx->running_state =  ISP_BE_BUF_STATE_SWITCH;
        for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
            vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
            drv_ctx_s = isp_drv_get_ctx(vi_pipes);
            if (drv_ctx_s->running_state != ISP_BE_BUF_STATE_SWITCH) {
                osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
                drv_ctx->running_state = ISP_BE_BUF_STATE_SWITCH_START;
                return HI_SUCCESS;
            }
        }
        drv_ctx->running_state = ISP_BE_BUF_STATE_SWITCH_START;
    }

    for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
        vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
        drv_ctx_s = isp_drv_get_ctx(vi_pipes);
        if ((drv_ctx_s->be_buf_info.init != HI_TRUE) || (drv_ctx_s->use_node == HI_NULL)) {
            osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
            isp_err_trace("Pipe[%d] BeBuf (bInit != TRUE) or use_node is HI_NULL!\n", vi_pipe);
            return HI_FAILURE;
        }

        isp_queue_put_busy_be_buf(&drv_ctx_s->be_buf_queue, drv_ctx_s->use_node);
        drv_ctx_s->use_node = HI_NULL;
        drv_ctx_s->running_state = ISP_BE_BUF_STATE_INIT;
    }

    osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);

    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_get_be_free_buf(hi_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_wo_cfg_buf)
{
    osal_spinlock_t *isp_spin_lock = HI_NULL;
    unsigned long flags;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_be_wo_cfg_buf *cur_node_buf = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(be_wo_cfg_buf);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);
    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);

    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if (drv_ctx->use_node == HI_NULL) {
        osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
        return HI_FAILURE;
    }

    cur_node_buf = &drv_ctx->use_node->be_cfg_buf;
    osal_memcpy(be_wo_cfg_buf, cur_node_buf, sizeof(isp_be_wo_cfg_buf));

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_be_buf_first(hi_vi_pipe vi_pipe, hi_u64 *point_phy_addr)
{
    unsigned long flags;
    isp_drv_ctx *drv_ctx  = HI_NULL;
    isp_be_buf_node *node = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(point_phy_addr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);
    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);
    if (node == HI_NULL) {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

        isp_err_trace("Pipe[%d] Get FreeBeBuf to user fail!\r\n", vi_pipe);
        return HI_FAILURE;
    }

    drv_ctx->use_node = node;

    *point_phy_addr = drv_ctx->use_node->be_cfg_buf.phy_addr;

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_be_last_buf(hi_vi_pipe vi_pipe, hi_u64 *point_phy_addr)
{
    hi_u8  i;
    isp_drv_ctx     *drv_ctx = HI_NULL;
    isp_be_buf_node *node    = HI_NULL;
    isp_be_wo_reg_cfg     *be_reg_cfg_dst = HI_NULL;
    struct osal_list_head *list_tmp       = HI_NULL;
    struct osal_list_head *list_node      = HI_NULL;
    unsigned long flags;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(point_phy_addr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);
    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);

    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    osal_list_for_each_safe(list_node, list_tmp, &drv_ctx->be_buf_queue.busy_list) {
        node = osal_list_entry(list_node, isp_be_buf_node, list);

        node->hold_cnt = 0;

        isp_queue_del_busy_be_buf(&drv_ctx->be_buf_queue, node);
        isp_queue_put_free_be_buf(&drv_ctx->be_buf_queue, node);
    }

    if (!drv_ctx->use_node) {
        node = isp_queue_get_free_be_buf_tail(&drv_ctx->be_buf_queue);
        if (node == HI_NULL) {
            osal_spin_unlock_irqrestore(isp_spin_lock, &flags);
            isp_err_trace("Pipe[%d] Get LastBeBuf fail!\r\n", vi_pipe);
            return HI_FAILURE;
        }

        drv_ctx->use_node = node;
    }

    be_reg_cfg_dst = (isp_be_wo_reg_cfg *)drv_ctx->use_node->be_cfg_buf.vir_addr;

    for (i = drv_ctx->work_mode.pre_block_num; i < drv_ctx->work_mode.block_num; i++) {
        osal_memcpy(&be_reg_cfg_dst->be_reg_cfg[i], &be_reg_cfg_dst->be_reg_cfg[0], sizeof(S_ISPBE_REGS_TYPE));
    }

    *point_phy_addr = drv_ctx->use_node->be_cfg_buf.phy_addr;

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_buf_run_state(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    osal_spinlock_t *isp_spin_lock = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);
    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    isp_spin_lock = isp_drv_get_spin_lock(vi_pipe);

    osal_spin_lock_irqsave(isp_spin_lock, &flags);

    if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        if (drv_ctx->running_state != ISP_BE_BUF_STATE_INIT) {
            osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

            isp_warn_trace("Pipe[%d] isp isn't init state!\n", vi_pipe);
            return HI_FAILURE;
        }

        drv_ctx->running_state = ISP_BE_BUF_STATE_RUNNING;
    }

    osal_spin_unlock_irqrestore(isp_spin_lock, &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_be_buf_ctl(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);

    if (drv_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        if (drv_ctx->be_buf_info.init != HI_TRUE) {
            isp_err_trace("Pipe[%d] BeBuf (bInit != TRUE) !\n", vi_pipe);
            return HI_FAILURE;
        }

        osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
        if (drv_ctx->run_once_flag != HI_TRUE) {
            isp_drv_be_buf_queue_put_busy(vi_pipe);
        }
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    } else {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
        isp_drv_stitch_be_buf_ctl(vi_pipe);
#endif
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_all_be_buf_init(hi_vi_pipe vi_pipe)
{
    hi_s32 ret = HI_SUCCESS;
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    isp_check_online_mode_return(vi_pipe, drv_ctx->work_mode.running_mode);

    if (drv_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        if (drv_ctx->be_buf_info.init != HI_TRUE) {
            isp_err_trace("Pipe[%d] BeBuf (bInit != TRUE) !\n", vi_pipe);
            return HI_FAILURE;
        }

        osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

        ret = isp_drv_write_be_free_buf(vi_pipe);
        if (ret != HI_SUCCESS) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

            isp_err_trace("Pipe[%d] ISP_DRV_WriteBeFreeBuf fail!\n", vi_pipe);
            return ret;
        }

        isp_queue_put_busy_be_buf(&drv_ctx->be_buf_queue, drv_ctx->use_node);
        drv_ctx->use_node = HI_NULL;

        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    } else {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
        ret = isp_drv_stitch_write_be_buf_all(vi_pipe);
        if (ret != HI_SUCCESS) {
            isp_err_trace("Pipe[%d] ISP_DRV_StitchWriteBeBufAll fail!\n", vi_pipe);
            return ret;
        }
#endif
    }

    return ret;
}

hi_s32 isp_drv_sync_cfg_set(hi_vi_pipe vi_pipe, isp_sync_cfg_buf_node *sync_cfg_buf_node)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_sync_cfg_buf  *sync_cfg_buf  = HI_NULL;
    isp_sync_cfg_buf_node *cur_node  = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(sync_cfg_buf_node);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    sync_cfg_buf = &drv_ctx->sync_cfg.sync_cfg_buf;

    if (isp_sync_buf_is_full(sync_cfg_buf)) {
        isp_err_trace("Pipe[%d] isp sync buffer is full\n", vi_pipe);
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }

    if ((sync_cfg_buf_node->sns_regs_info.sns_type >= ISP_SNS_TYPE_BUTT) ||
        (sync_cfg_buf_node->ae_reg_cfg.fs_wdr_mode >= ISP_FSWDR_MODE_BUTT) ||
        (sync_cfg_buf_node->sns_regs_info.cfg2_valid_delay_max > CFG2VLD_DLY_LIMIT) ||
        (sync_cfg_buf_node->sns_regs_info.cfg2_valid_delay_max < 1) ||
        (sync_cfg_buf_node->sns_regs_info.slv_sync.delay_frm_num > CFG2VLD_DLY_LIMIT) ||
        (sync_cfg_buf_node->sns_regs_info.reg_num > ISP_MAX_SNS_REGS)) {
        isp_err_trace("Invalid SnsRegsInfo!\n");
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return HI_FAILURE;
    }

    cur_node = &sync_cfg_buf->sync_cfg_buf_node[sync_cfg_buf->buf_wr_flag];
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    osal_memcpy(cur_node, sync_cfg_buf_node, sizeof(isp_sync_cfg_buf_node));

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    sync_cfg_buf->buf_wr_flag = (sync_cfg_buf->buf_wr_flag + 1) % ISP_SYNC_BUF_NODE_NUM;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

hi_s32 isp_drv_set_wdr_cfg(hi_vi_pipe vi_pipe, isp_wdr_cfg *wdr_cfg)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(wdr_cfg);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    if (wdr_cfg->wdr_mode >= WDR_MODE_BUTT) {
        isp_err_trace("Invalid WDR mode %d!\n", wdr_cfg->wdr_mode);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    osal_memcpy(&drv_ctx->wdr_cfg, wdr_cfg, sizeof(isp_wdr_cfg));
    ret = isp_drv_switch_mode(vi_pipe, drv_ctx);
    isp_check_ret(vi_pipe, ret, "isp_drv_switch_mode");
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32 isp_drv_get_stitch_attr(hi_vi_pipe vi_pipe, vi_stitch_attr *stitch_attr)
{
    hi_u8  i;
    hi_s32 ret;
    isp_drv_ctx    *drv_ctx = HI_NULL;
    vi_stitch_attr stitch_attr_str = { 0 };
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stitch_attr);
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (!ckfn_vi_get_pipe_stitch_attr()) {
        isp_err_trace("pipe[%d] ckfn_vi_get_pipe_stitch_attr is null\n", vi_pipe);
        return HI_FAILURE;
    }

    ret = call_vi_get_pipe_stitch_attr(vi_pipe, &stitch_attr_str);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe[%d] call_vi_get_pipe_stitch_attr failed 0x%x!\n", vi_pipe, ret);
        return ret;
    }

    if (stitch_attr_str.stitch_enable) {
        if ((stitch_attr_str.stitch_pipe_num < 1) || (stitch_attr_str.stitch_pipe_num > ISP_MAX_PIPE_NUM)) {
            isp_err_trace("pipe[%d] err stitch num %d\n", vi_pipe, stitch_attr_str.stitch_pipe_num);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        for (i = 0; i < stitch_attr_str.stitch_pipe_num; i++) {
            isp_check_pipe_return(stitch_attr_str.stitch_bind_id[i]);
        }
    }
    osal_memcpy(&drv_ctx->stitch_attr, &stitch_attr_str, sizeof(vi_stitch_attr));
    osal_memcpy(stitch_attr, &stitch_attr_str, sizeof(vi_stitch_attr));
    return HI_SUCCESS;
}
#else
hi_s32 isp_drv_get_stitch_attr(hi_vi_pipe vi_pipe, vi_stitch_attr *stitch_attr)
{
    isp_drv_ctx    *drv_ctx = HI_NULL;
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(stitch_attr);
    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_memset(&drv_ctx->stitch_attr, 0, sizeof(vi_stitch_attr));
    osal_memset(stitch_attr, 0, sizeof(vi_stitch_attr));
    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_get_pipe_size(hi_vi_pipe vi_pipe, hi_size *pipe_size)
{
    hi_s32  ret;
    hi_size pipe_size_str = { 0 };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pipe_size);
    isp_check_vir_pipe_return(vi_pipe);
    if (!ckfn_vi_get_pipe_bind_dev_size()) {
        isp_err_trace("pipe[%d] ckfn_vi_get_pipe_bind_dev_size is null\n", vi_pipe);
        return HI_FAILURE;
    }

    ret = call_vi_get_pipe_bind_dev_size(vi_pipe, &pipe_size_str);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe[%d] call_vi_get_pipe_bind_dev_size failed 0x%x!\n", vi_pipe, ret);
        return ret;
    }

    if ((pipe_size_str.width < RES_WIDTH_MIN) ||
        (pipe_size_str.width > SENSOR_RES_WIDTH_MAX) ||
        (pipe_size_str.height < RES_HEIGHT_MIN) ||
        (pipe_size_str.height > SENSOR_RES_HEIGHT_MAX)) {
        isp_err_trace("pipe[%d]: Image Width should between [%d, %d], Height should between[%d, %d]\n",
                      vi_pipe, RES_WIDTH_MIN, SENSOR_RES_WIDTH_MAX, RES_HEIGHT_MIN, SENSOR_RES_HEIGHT_MAX);
        return HI_FAILURE;
    }

    osal_memcpy(pipe_size, &pipe_size_str, sizeof(hi_size));

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_hdr_attr(hi_vi_pipe vi_pipe, vi_pipe_hdr_attr *hdr_attr)
{
    hi_s32  ret;
    vi_pipe_hdr_attr hdr_attr_str = { 0 };

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(hdr_attr);

    if (!ckfn_vi_get_pipe_hdr_attr()) {
        isp_err_trace("pipe[%d] ckfn_vi_get_pipe_hdr_attr is null\n", vi_pipe);
        return HI_FAILURE;
    }

    ret = call_vi_get_pipe_hdr_attr(vi_pipe, &hdr_attr_str);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe[%d] call_vi_get_pipe_hdr_attr failed 0x%x!\n", vi_pipe, ret);
        return ret;
    }

    osal_memcpy(hdr_attr, &hdr_attr_str, sizeof(vi_pipe_hdr_attr));

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_wdr_attr(hi_vi_pipe vi_pipe, vi_pipe_wdr_attr *wdr_attr)
{
    hi_u8   i;
    hi_u32  num;
    hi_s32  ret;
    vi_pipe_wdr_attr wdr_attr_str;
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(wdr_attr);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    if (!ckfn_vi_get_pipe_wdr_attr()) {
        isp_err_trace("pipe[%d] ckfn_vi_get_pipe_wdr_attr is null\n", vi_pipe);
        return HI_FAILURE;
    }

    ret = call_vi_get_pipe_wdr_attr(vi_pipe, &wdr_attr_str);
    if (ret != HI_SUCCESS) {
        isp_err_trace("pipe[%d] call_vi_get_pipe_wdr_attr failed 0x%x!\n", vi_pipe, ret);
        return ret;
    }

    /* Not WDR mode,BindPipe attr update */
    if (!is_fs_wdr_mode(wdr_attr_str.wdr_mode) &&
        (wdr_attr_str.dev_bind_pipe.num != 1)) {
        wdr_attr_str.dev_bind_pipe.num    = 1;
        wdr_attr_str.dev_bind_pipe.pipe_id[0] = vi_pipe;
    }

    num = wdr_attr_str.dev_bind_pipe.num;
    if ((num < 1) || (num > ISP_WDR_CHN_MAX)) {
        isp_err_trace("Err wdr bind num %d!\n", num);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < num; i++) {
        isp_check_pipe_return(wdr_attr_str.dev_bind_pipe.pipe_id[i]);
    }
    if ((wdr_attr_str.wdr_mode > WDR_MODE_BUTT)) {
        isp_err_trace("Err wdr mode %d!\n", wdr_attr_str.wdr_mode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    osal_memcpy(&drv_ctx->wdr_attr, &wdr_attr_str, sizeof(vi_pipe_wdr_attr));
    osal_memcpy(wdr_attr, &wdr_attr_str, sizeof(vi_pipe_wdr_attr));

    return HI_SUCCESS;
}

hi_s32 isp_drv_chn_select_cfg(hi_vi_pipe vi_pipe, hi_u32 chn_sel)
{
    hi_u32 i;
    hi_s32 ret = HI_SUCCESS;
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        drv_ctx->chn_sel_attr[i].channel_sel = chn_sel;
    }

    if (is_online_mode(drv_ctx->work_mode.running_mode) ||
        is_sidebyside_mode(drv_ctx->work_mode.running_mode)) {
        ret = isp_drv_chn_select_write(vi_pipe, chn_sel);
        if (ret != HI_SUCCESS) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("isp[%d] ChnSelect Write err!\n", vi_pipe);
            return ret;
        }
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return ret;
}

hi_s32 isp_drv_set_isp_run_state(hi_vi_pipe vi_pipe, hi_u64 *hand_signal)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(hand_signal);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (*hand_signal == ISP_INIT_HAND_SIGNAL) {
        drv_ctx->isp_run_flag = HI_TRUE;
    } else if (*hand_signal == ISP_EXIT_HAND_SIGNAL) {
        drv_ctx->isp_run_flag = HI_FALSE;
        isp_drv_reset_fe_cfg(vi_pipe);
    } else {
        isp_err_trace("ISP[%d] set isp run state failed!\n", vi_pipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_void isp_drv_reset_snap_ctx(isp_drv_ctx *drv_ctx)
{
    drv_ctx->snap_attr.snap_type          = SNAP_TYPE_NORMAL;
    drv_ctx->snap_attr.picture_pipe_id    = -1;
    drv_ctx->snap_attr.preview_pipe_id    = -1;
    drv_ctx->snap_attr.load_ccm           = HI_TRUE;
    drv_ctx->snap_attr.pro_param.operation_mode = OPERATION_MODE_AUTO;
}

hi_s32 isp_drv_reset_ctx(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    hi_vi_pipe vi_pipes = 0;
    unsigned long flags;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;

    isp_check_pipe_return(vi_pipe);
    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (drv_ctx->isp_run_flag == HI_TRUE) {
        isp_err_trace("ISP[%d] Should set isp_run_flag to HI_FALSE first!!\n", vi_pipe);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

    drv_ctx->frame_cnt     = 0;
    drv_ctx->mem_init      = HI_FALSE;
    drv_ctx->isp_init      = HI_FALSE;
    drv_ctx->pub_attr_ok   = HI_FALSE;
    drv_ctx->run_once_ok   = HI_FALSE;
    drv_ctx->run_once_flag = HI_FALSE;
    drv_ctx->yuv_run_once_ok   = HI_FALSE;
    drv_ctx->yuv_run_once_flag = HI_FALSE;

    if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        if (drv_ctx->stitch_attr.main_pipe == HI_TRUE) {
            for (i = 0; i < drv_ctx->stitch_attr.stitch_pipe_num; i++) {
                vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[i];
                isp_check_pipe_return(vi_pipes);
                drv_ctx_s = isp_drv_get_ctx(vi_pipes);
                drv_ctx_s->stitch_sync  = HI_FALSE;
            }
        }
    }

    drv_ctx->stitch_attr.stitch_enable = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        drv_ctx->chn_sel_attr[i].channel_sel = 0;
    }

    isp_drv_reset_snap_ctx(drv_ctx);

    drv_ctx->ai_info.pq_ai_en     = HI_FALSE;
    drv_ctx->ai_info.ai_pipe_id   = -1;
    drv_ctx->ai_info.base_pipe_id = -1;

    drv_ctx->edge      = HI_FALSE;
    drv_ctx->vd_start  = HI_FALSE;
    drv_ctx->vd_end    = HI_FALSE;
    drv_ctx->vd_be_end = HI_FALSE;

    g_int_bottom_half = HI_FALSE;
    g_use_bottom_half = HI_FALSE;
    g_quick_start     = HI_FALSE;
    g_long_frm_int_en = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    return HI_SUCCESS;
}

static hi_s32 isp_set_vi_bayer_format(hi_vi_pipe vi_pipe, hi_isp_bayer_format bayer_format)
{
    hi_s32  ret;

    if (ckfn_vi_update_bayer_format()) {
        ret = call_vi_update_bayer_format(vi_pipe, bayer_format);
        if (ret != HI_SUCCESS) {
            isp_err_trace("ISP[%d] call_vi_update_bayer_format failed!\n", vi_pipe);
            return ret;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_pub_attr_info(hi_vi_pipe vi_pipe, hi_isp_pub_attr *pub_attr)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    unsigned long flags;
    hi_s32 ret;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(pub_attr);

    if ((pub_attr->wnd_rect.width < RES_WIDTH_MIN) || (pub_attr->wnd_rect.width > g_res_max_width[vi_pipe]) ||
        (pub_attr->wnd_rect.width % ISP_ALIGN_WIDTH != 0)) {
        isp_err_trace("Invalid Image Width:%d!\n", pub_attr->wnd_rect.width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->wnd_rect.height < RES_HEIGHT_MIN) || (pub_attr->wnd_rect.height > g_res_max_height[vi_pipe]) ||
        (pub_attr->wnd_rect.height % ISP_ALIGN_HEIGHT != 0)) {
        isp_err_trace("Invalid Image Height:%d!\n", pub_attr->wnd_rect.height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->sns_size.width < RES_WIDTH_MIN) || (pub_attr->sns_size.width > SENSOR_RES_WIDTH_MAX)) {
        isp_err_trace("Invalid Sensor Image Width:%d!\n", pub_attr->sns_size.width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->sns_size.height < RES_HEIGHT_MIN) || (pub_attr->sns_size.height > SENSOR_RES_HEIGHT_MAX)) {
        isp_err_trace("Invalid Sensor Image Height:%d!\n", pub_attr->sns_size.height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->wnd_rect.x < 0) || (pub_attr->wnd_rect.x > g_res_max_width[vi_pipe] - RES_WIDTH_MIN)) {
        isp_err_trace("Invalid Image X:%d!\n", pub_attr->wnd_rect.x);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->wnd_rect.y < 0) || (pub_attr->wnd_rect.y > g_res_max_height[vi_pipe] - RES_HEIGHT_MIN)) {
        isp_err_trace("Invalid Image Y:%d!\n", pub_attr->wnd_rect.y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pub_attr->bayer >= BAYER_BUTT) {
        isp_err_trace("Invalid Bayer Pattern:%d!\n", pub_attr->bayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pub_attr->wdr_mode >= WDR_MODE_BUTT) {
        isp_err_trace("Invalid WDR mode %d!\n", pub_attr->wdr_mode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    osal_memcpy(&drv_ctx->proc_pub_info, pub_attr, sizeof(hi_isp_pub_attr));
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

    ret = isp_set_vi_bayer_format(vi_pipe, pub_attr->bayer);
    isp_check_ret(vi_pipe, ret, "isp_set_vi_bayer_format");

    drv_ctx->pub_attr_ok = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_set_awb_sta_enable(VI_PIPE vi_pipe, hi_bool enable)
{
    isp_drv_ctx *drv_ctx = HI_NULL;

    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if ((is_online_mode(drv_ctx->work_mode.running_mode)) ||
        (is_sidebyside_mode(drv_ctx->work_mode.running_mode))) {
        isp_drv_reg_config_awb_en(vi_pipe, enable);
    }

    return HI_SUCCESS;
}

static long isp_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    hi_vi_pipe vi_pipe = isp_get_dev(private_data);

    switch (cmd) {
        case ISP_DEV_SET_FD: {
            isp_check_pointer_return(arg);
            *((hi_u32 *)(private_data)) = *(hi_u32 *)(hi_uintptr_t)arg;

            return HI_SUCCESS;
        }

        case ISP_GET_VERSION: {
            isp_version *version = HI_NULL;
            version = (isp_version *)(hi_uintptr_t)arg;

            return isp_drv_get_version(version);
        }

        case ISP_GET_FRAME_EDGE: {
            hi_u32 *status = HI_NULL;
            status = (hi_u32 *)(hi_uintptr_t)arg;

            return isp_get_frame_edge(vi_pipe, status);
        }

        case ISP_GET_VD_TIMEOUT: {
            isp_vd_timeout isp_vd_time_out;
            isp_vd_timeout *vd_time_out = HI_NULL;

            isp_check_pointer_return(arg);

            vd_time_out = (isp_vd_timeout *)(hi_uintptr_t)arg;
            osal_memcpy(&isp_vd_time_out, vd_time_out, sizeof(isp_vd_timeout));
            isp_get_vd_start_time_out(vi_pipe, isp_vd_time_out.milli_sec, &isp_vd_time_out.int_status);
            osal_memcpy(vd_time_out, &isp_vd_time_out, sizeof(isp_vd_timeout));

            return HI_SUCCESS;
        }

        case ISP_GET_VD_END_TIMEOUT: {
            isp_vd_timeout isp_vd_time_out;
            isp_vd_timeout *vd_time_out = HI_NULL;

            isp_check_pointer_return(arg);

            vd_time_out = (isp_vd_timeout *)(hi_uintptr_t)arg;
            osal_memcpy(&isp_vd_time_out, vd_time_out, sizeof(isp_vd_timeout));
            isp_get_vd_end_time_out(vi_pipe, isp_vd_time_out.milli_sec, &isp_vd_time_out.int_status);
            osal_memcpy(vd_time_out, &isp_vd_time_out, sizeof(isp_vd_timeout));

            return HI_SUCCESS;
        }

        case ISP_GET_VD_BEEND_TIMEOUT: {
            isp_vd_timeout isp_vd_time_out;
            isp_vd_timeout *vd_time_out = HI_NULL;

            isp_check_pointer_return(arg);

            vd_time_out = (isp_vd_timeout *)(hi_uintptr_t)arg;
            osal_memcpy(&isp_vd_time_out, vd_time_out, sizeof(isp_vd_timeout));
            isp_get_vd_be_end_time_out(vi_pipe, isp_vd_time_out.milli_sec, &isp_vd_time_out.int_status);
            osal_memcpy(vd_time_out, &isp_vd_time_out, sizeof(isp_vd_timeout));

            return HI_SUCCESS;
        }

        case ISP_UPDATE_INFO_SET: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            hi_isp_dcf_update_info *isp_update_info = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            isp_update_info = (hi_isp_dcf_update_info *)(hi_uintptr_t)arg;
            osal_memcpy(&drv_ctx->update_info, isp_update_info, sizeof(hi_isp_dcf_update_info));
            return HI_SUCCESS;
        }

        case ISP_FRAME_INFO_SET: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            hi_isp_frame_info *isp_frame_info = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            isp_frame_info = (hi_isp_frame_info *)(hi_uintptr_t)arg;
            osal_memcpy(&drv_ctx->frame_info, isp_frame_info, sizeof(hi_isp_frame_info));
            return HI_SUCCESS;
        }

        case ISP_FRAME_INFO_GET: {
            hi_isp_frame_info *isp_frame_info = HI_NULL;
            isp_frame_info = (hi_isp_frame_info *)(hi_uintptr_t)arg;

            return isp_get_frame_info(vi_pipe, isp_frame_info);
        }

        case ISP_DNG_INFO_SET: {
            hi_dng_image_dynamic_info *dng_img_dyn_info = HI_NULL;
            dng_img_dyn_info = (hi_dng_image_dynamic_info *)(hi_uintptr_t)arg;

            return isp_drv_set_dng_info(vi_pipe, dng_img_dyn_info);
        }

        case ISP_DNG_INFO_GET: {
            hi_isp_dng_image_static_info *dng_static_info = HI_NULL;
            dng_static_info = (hi_isp_dng_image_static_info *)(hi_uintptr_t)arg;

            return isp_drv_get_dng_info(vi_pipe, dng_static_info);
        }

        case ISP_SET_INT_ENABLE: {
            hi_bool en = HI_FALSE;
            isp_check_pointer_return(arg);
            en = *(hi_bool *)(hi_uintptr_t)arg;

            return isp_drv_set_int_enable(vi_pipe, en);
        }

        case ISP_TRANS_BUF_INIT: {
            isp_trans_info_buf *trans_info = HI_NULL;
            trans_info = (isp_trans_info_buf *)(hi_uintptr_t)arg;

            return isp_drv_trans_info_buf_init(vi_pipe, trans_info);
        }

        case ISP_TRANS_BUF_EXIT: {
            return isp_drv_trans_info_buf_exit(vi_pipe);
        }

        case ISP_PRO_BUF_INIT: {
            isp_pro_info_buf *pro_info = HI_NULL;
            pro_info = (isp_pro_info_buf *)(hi_uintptr_t)arg;

            return isp_drv_pro_info_buf_init(vi_pipe, pro_info);
        }

        case ISP_PRO_BUF_EXIT: {
            return isp_drv_pro_info_buf_exit(vi_pipe);
        }

        case ISP_STAT_BUF_INIT: {
            hi_u64 *stat_phy_addr = HI_NULL;
            stat_phy_addr = (hi_u64 *)(hi_uintptr_t)arg;

            return isp_drv_stat_buf_init(vi_pipe, stat_phy_addr);
        }

        case ISP_STAT_BUF_EXIT: {
            return isp_drv_stat_buf_exit(vi_pipe);
        }

        case ISP_STAT_BUF_GET: {
            isp_stat_info *stat = HI_NULL;
            stat = (isp_stat_info *)(hi_uintptr_t)arg;

            return isp_drv_get_user_stat_buf(vi_pipe, stat);
        }

        case ISP_STAT_BUF_PUT: {
            isp_stat_info *stat_info = HI_NULL;
            stat_info = (isp_stat_info *)(hi_uintptr_t)arg;

            return isp_drv_stat_buf_user_put(vi_pipe, stat_info);
        }

        case ISP_STAT_ACT_GET: {
            isp_stat_info *stat_info = HI_NULL;
            stat_info = (isp_stat_info *)(hi_uintptr_t)arg;

            return isp_drv_get_stat_info_active(vi_pipe, stat_info);
        }

        case ISP_REG_CFG_SET: {
            isp_kernel_reg_cfg *reg_kernel_cfg = HI_NULL;
            reg_kernel_cfg = (isp_kernel_reg_cfg *)(hi_uintptr_t)arg;

            return isp_drv_set_reg_kernel_cfgs(vi_pipe, reg_kernel_cfg);
        }

        case ISP_BE_BUF_NUM_GET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            *(hi_u8 *)(hi_uintptr_t)arg = g_be_buf_num[vi_pipe];
            return HI_SUCCESS;
        }

        case ISP_BE_CFG_BUF_INIT: {
            hi_u64 *be_phy_addr = HI_NULL;
            be_phy_addr = (hi_u64 *)(hi_uintptr_t)arg;

            return isp_drv_be_buf_init(vi_pipe, be_phy_addr);
        }

        case ISP_GET_BE_BUF_FIRST: {
            hi_u64 *point_phy_addr = HI_NULL;
            point_phy_addr = (hi_u64 *)(hi_uintptr_t)arg;

            return isp_drv_get_be_buf_first(vi_pipe, point_phy_addr);
        }

        case ISP_BE_FREE_BUF_GET: {
            isp_be_wo_cfg_buf *be_wo_cfg_buf = HI_NULL;
            be_wo_cfg_buf = (isp_be_wo_cfg_buf *)(hi_uintptr_t)arg;

            return isp_drv_get_be_free_buf(vi_pipe, be_wo_cfg_buf);
        }

        case ISP_BE_LAST_BUF_GET: {
            hi_u64 *point_phy_addr = HI_NULL;
            point_phy_addr = (hi_u64 *)(hi_uintptr_t)arg;

            return isp_drv_get_be_last_buf(vi_pipe, point_phy_addr);
        }

        case ISP_BE_CFG_BUF_EXIT: {
            return isp_drv_be_buf_exit(vi_pipe);
        }

        case ISP_BE_CFG_BUF_RUNNING: {
            return isp_drv_be_buf_run_state(vi_pipe);
        }

        case ISP_BE_CFG_BUF_CTL: {
            return isp_drv_be_buf_ctl(vi_pipe);
        }

        case ISP_BE_All_BUF_INIT: {
            return isp_drv_all_be_buf_init(vi_pipe);
        }

        case ISP_SYNC_CFG_SET: {
            isp_sync_cfg_buf_node *isp_sync_cfg_buf = HI_NULL;
            isp_sync_cfg_buf = (isp_sync_cfg_buf_node *)(hi_uintptr_t)arg;

            return isp_drv_sync_cfg_set(vi_pipe, isp_sync_cfg_buf);
        }

        case ISP_WDR_CFG_SET: {
            isp_wdr_cfg *wdr_cfg = HI_NULL;
            wdr_cfg = (isp_wdr_cfg *)(hi_uintptr_t)arg;

            return isp_drv_set_wdr_cfg(vi_pipe, wdr_cfg);
        }

        case ISP_BE_SYNC_PARAM_INIT: {
            isp_drv_ctx      *drv_ctx      = HI_NULL;
            isp_be_sync_para   *be_sync_param = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            be_sync_param = (isp_be_sync_para *)(hi_uintptr_t)arg;
            osal_memcpy(&drv_ctx->be_sync_para, be_sync_param, sizeof(isp_be_sync_para));
            return HI_SUCCESS;
        }

        case ISP_RES_SWITCH_SET: {
            isp_check_pipe_return(vi_pipe);
            isp_drv_be_buf_switch_state(vi_pipe);
            return HI_SUCCESS;
        }

        case ISP_BE_SWITCH_FINISH_STATE_SET: {
            return isp_drv_be_buf_switch_finish_state(vi_pipe);
        }

        case ISP_CHN_SELECT_CFG: {
            hi_u32 channel_sel = 0;
            isp_check_pointer_return(arg);
            channel_sel = *(hi_u32 *)(hi_uintptr_t)arg;

            return isp_drv_chn_select_cfg(vi_pipe, channel_sel);
        }

        case ISP_PROC_INIT: {
            isp_proc_mem *proc_mem = HI_NULL;
            proc_mem = (isp_proc_mem *)(hi_uintptr_t)arg;

            return isp_drv_proc_init(vi_pipe, proc_mem);
        }

        case ISP_PROC_WRITE_ING: {
            isp_check_pipe_return(vi_pipe);

            if (osal_down_interruptible(&g_isp_drv_ctx[vi_pipe].proc_sem)) {
                return -ERESTARTSYS;
            }
            return HI_SUCCESS;
        }

        case ISP_PROC_WRITE_OK: {
            isp_check_pipe_return(vi_pipe);

            osal_up(&g_isp_drv_ctx[vi_pipe].proc_sem);
            return HI_SUCCESS;
        }

        case ISP_PROC_EXIT: {
            return isp_drv_proc_exit(vi_pipe);
        }

        case ISP_PROC_PARAM_GET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            *(hi_u32 *)(hi_uintptr_t)arg = g_proc_param[vi_pipe];

            return HI_SUCCESS;
        }

        case ISP_MEM_INFO_SET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            isp_check_bool_return(*(hi_bool *)(hi_uintptr_t)arg);
            g_isp_drv_ctx[vi_pipe].mem_init = HI_TRUE;

            return HI_SUCCESS;
        }

        case ISP_MEM_INFO_GET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            *(hi_bool *)(hi_uintptr_t)arg = g_isp_drv_ctx[vi_pipe].mem_init;

            return HI_SUCCESS;
        }

        case ISP_P2EN_INFO_GET: {
            hi_bool *p2_en = HI_NULL;

            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            p2_en = (hi_bool *)(hi_uintptr_t)arg;

            return isp_drv_get_p2_en_info(vi_pipe, p2_en);
        }

        case ISP_SYNC_INIT_SET: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            isp_check_bool_return(*(hi_bool *)(hi_uintptr_t)arg);
            drv_ctx = isp_drv_get_ctx(vi_pipe);

            drv_ctx->stitch_sync = HI_FALSE;
            if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
                drv_ctx->stitch_sync = *(hi_bool *)(hi_uintptr_t)arg;
            }

            return HI_SUCCESS;
        }

        case ISP_INIT_INFO_SET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            isp_check_bool_return(*(hi_bool *)(hi_uintptr_t)arg);
            g_isp_drv_ctx[vi_pipe].isp_init = *(hi_bool *)(hi_uintptr_t)arg;

            return HI_SUCCESS;
        }

        case ISP_RESET_CTX: {
            return isp_drv_reset_ctx(vi_pipe);
        }

        case ISP_RUN_STATE_SET: {
            hi_u64 *hand_signal = HI_NULL;
            hand_signal = (hi_u64 *)(hi_uintptr_t)arg;

            return isp_drv_set_isp_run_state(vi_pipe, hand_signal);
        }

        case ISP_CONFIG_INFO_SET: {
            hi_isp_config_info *isp_config_info = HI_NULL;
            isp_config_info = (hi_isp_config_info *)(hi_uintptr_t)arg;

            return isp_drv_set_config_info(vi_pipe, isp_config_info);
        }

        case ISP_SNAP_INFO_GET: {
#ifdef CONFIG_HI_SNAP_SUPPORT
            isp_snap_info *snap_info = HI_NULL;
            snap_info = (isp_snap_info *)(hi_uintptr_t)arg;
            return isp_drv_get_snap_info(vi_pipe, snap_info);
#else
            return HI_SUCCESS;
#endif
        }

        case ISP_PRO_TRIGGER_GET: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            *(hi_bool *)(hi_uintptr_t)arg = drv_ctx->pro_enable;

            if (drv_ctx->pro_enable == HI_TRUE) {
                drv_ctx->pro_enable = HI_FALSE;
                drv_ctx->pro_trig_flag = 1;
            }

            return HI_SUCCESS;
        }

        case ISP_SNAP_ATTR_GET: {
            isp_snap_attr *snap_attr = HI_NULL;
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            snap_attr = (isp_snap_attr *)(hi_uintptr_t)arg;
            drv_ctx = isp_drv_get_ctx(vi_pipe);
            osal_memcpy(snap_attr, &drv_ctx->snap_attr, sizeof(isp_snap_attr));

            return HI_SUCCESS;
        }

        case ISP_UPDATE_POS_GET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            *(hi_u32 *)(hi_uintptr_t)arg = g_update_pos[vi_pipe];

            return HI_SUCCESS;
        }

        case ISP_FRAME_CNT_GET: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            *(hi_u32 *)(hi_uintptr_t)arg = drv_ctx->frame_cnt;

            return HI_SUCCESS;
        }
        case ISP_PUB_ATTR_INFO: {
            hi_isp_pub_attr *isp_pub_attr = HI_NULL;
            isp_pub_attr = (hi_isp_pub_attr *)(hi_uintptr_t)arg;

            return isp_set_pub_attr_info(vi_pipe, isp_pub_attr);
        }

        case ISP_PWM_NUM_GET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            *(hi_u32 *)(hi_uintptr_t)arg = g_pwm_number[vi_pipe];

            return HI_SUCCESS;
        }
#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
        case ISP_PQ_AI_GROUP_ATTR_SET: {
            pq_ai_attr *ai_attr = HI_NULL;

            isp_check_pointer_return(arg);

            ai_attr = (pq_ai_attr *)(hi_uintptr_t)arg;

            return isp_set_pq_ai_attr(ai_attr);
        }

        case ISP_PQ_AI_GROUP_ATTR_GET: {
            pq_ai_attr *ai_attr = HI_NULL;

            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            ai_attr = (pq_ai_attr *)(hi_uintptr_t)arg;

            return isp_get_pq_ai_attr(vi_pipe, ai_attr);
        }

        case ISP_PQ_AI_POST_NR_ATTR_SET: {
            return isp_set_pq_ai_post_nr_attr(vi_pipe, (hi_pq_ai_noiseness_post_attr *)(hi_uintptr_t)arg);
        }

        case ISP_PQ_AI_POST_NR_ATTR_GET: {
            return isp_get_pq_ai_post_nr_attr(vi_pipe, (hi_pq_ai_noiseness_post_attr *)(hi_uintptr_t)arg);
        }
#endif
        case ISP_SET_MOD_PARAM: {
            hi_isp_mod_param *mode_param = HI_NULL;
            mode_param = (hi_isp_mod_param *)(hi_uintptr_t)arg;

            return isp_set_mod_param(mode_param);
        }

        case ISP_GET_MOD_PARAM: {
            hi_isp_mod_param *mode_param = HI_NULL;
            mode_param = (hi_isp_mod_param *)(hi_uintptr_t)arg;

            return isp_get_mod_param(mode_param);
        }

        case ISP_SET_CTRL_PARAM: {
            hi_isp_ctrl_param *ctrl_param = HI_NULL;
            ctrl_param = (hi_isp_ctrl_param *)(hi_uintptr_t)arg;

            return isp_set_ctrl_param(vi_pipe, ctrl_param);
        }

        case ISP_GET_CTRL_PARAM: {
            hi_isp_ctrl_param *ctrl_param = HI_NULL;
            ctrl_param = (hi_isp_ctrl_param *)(hi_uintptr_t)arg;

            return isp_get_ctrl_param(vi_pipe, ctrl_param);
        }

        case ISP_WORK_MODE_INIT: {
            isp_block_attr *block_attr = HI_NULL;
            block_attr = (isp_block_attr *)(hi_uintptr_t)arg;

            return isp_drv_work_mode_init(vi_pipe, block_attr);
        }

        case ISP_WORK_MODE_EXIT: {
            return isp_drv_work_mode_exit(vi_pipe);
        }

        case ISP_WORK_MODE_GET: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_working_mode *isp_work_mode = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            isp_work_mode = (isp_working_mode *)(hi_uintptr_t)arg;
            drv_ctx = isp_drv_get_ctx(vi_pipe);
            osal_memcpy(isp_work_mode, &drv_ctx->work_mode, sizeof(isp_working_mode));

            return HI_SUCCESS;
        }

        case ISP_PRE_BLK_NUM_UPDATE: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            drv_ctx->work_mode.pre_block_num = *(hi_u8 *)(hi_uintptr_t)arg;

            return HI_SUCCESS;
        }

        case ISP_GET_PIPE_SIZE: {
            hi_size *pipe_size = HI_NULL;
            pipe_size = (hi_size *)(hi_uintptr_t)arg;

            return isp_drv_get_pipe_size(vi_pipe, pipe_size);
        }

        case ISP_GET_HDR_ATTR: {
            vi_pipe_hdr_attr *hdr_attr = HI_NULL;
            hdr_attr = (vi_pipe_hdr_attr *)(hi_uintptr_t)arg;

            return isp_drv_get_hdr_attr(vi_pipe, hdr_attr);
        }

        case ISP_GET_WDR_ATTR: {
            vi_pipe_wdr_attr *wdr_attr = HI_NULL;
            wdr_attr = (vi_pipe_wdr_attr *)(hi_uintptr_t)arg;

            return isp_drv_get_wdr_attr(vi_pipe, wdr_attr);
        }

        case ISP_GET_STITCH_ATTR: {
            vi_stitch_attr *stitch_attr = HI_NULL;
            stitch_attr = (vi_stitch_attr *)(hi_uintptr_t)arg; // -1022734336

            return isp_drv_get_stitch_attr(vi_pipe, stitch_attr);
        }

        case ISP_SYNC_STITCH_PARAM_INIT: {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
            return isp_drv_stitch_sync_ctrl_init(vi_pipe);
#else
            return HI_FAILURE;
#endif
        }

        case ISP_CLUT_BUF_INIT: {
            return isp_drv_clut_buf_init(vi_pipe);
        }

        case ISP_CLUT_BUF_EXIT: {
            return isp_drv_clut_buf_exit(vi_pipe);
        }

        case ISP_CLUT_BUF_GET: {
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);
            if (g_isp_drv_ctx[vi_pipe].clut_buf_attr.init == HI_FALSE) {
                return HI_ERR_ISP_NOMEM;
            }
            *(hi_u64 *)(hi_uintptr_t)arg = g_isp_drv_ctx[vi_pipe].clut_buf_attr.clut_buf.phy_addr;

            return HI_SUCCESS;
        }

        case ISP_LDCI_BUF_INIT: {
            return isp_drv_ldci_buf_init(vi_pipe);
        }

        case ISP_LDCI_BUF_EXIT: {
            return isp_drv_ldci_buf_exit(vi_pipe);
        }

        case ISP_LDCI_READ_STT_BUF_GET: {
            return isp_drv_ldci_read_stt_buf_get(vi_pipe, (isp_ldci_read_stt_buf *)(hi_uintptr_t)arg);
        }

        case ISP_OPT_RUNONCE_INFO: {
            return isp_drv_opt_run_once_info(vi_pipe, (hi_bool *)(hi_uintptr_t)arg);
        }

        case ISP_YUV_RUNONCE_INFO: {
            return isp_drv_yuv_run_once_info(vi_pipe, (hi_bool *)(hi_uintptr_t)arg);
        }

        case ISP_KERNEL_RUNONCE: {
            return isp_drv_run_once_process(vi_pipe);
        }
        case ISP_KERNEL_YUV_RUNONCE: {
            return isp_drv_yuv_run_once_process(vi_pipe);
        }
        case ISP_SET_PROCALCDONE: {
            isp_check_pipe_return(vi_pipe);
            g_isp_drv_ctx[vi_pipe].pro_start = HI_TRUE;

            return HI_SUCCESS;
        }

        case ISP_STT_BUF_INIT: {
            return isp_drv_stt_buf_init(vi_pipe);
        }

        case ISP_STT_ADDR_INIT: {
            isp_drv_ctx *drv_ctx = HI_NULL;
            isp_check_pipe_return(vi_pipe);
            isp_check_vir_pipe_return(vi_pipe);

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            if (drv_ctx->stitch_attr.stitch_enable == HI_FALSE) {
                return isp_drv_fe_stt_addr_init(vi_pipe);
            } else {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
                return isp_drv_fe_stitch_stt_addr_init(vi_pipe);
#endif
            }
        }

        case ISP_BE_LUT_STT_BUF_GET: {
            hi_u64 phy_addr;
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);

            if (g_isp_drv_ctx[vi_pipe].be_lut2stt_attr.init == HI_FALSE) {
                return HI_ERR_ISP_NOMEM;
            }
            phy_addr = g_isp_drv_ctx[vi_pipe].be_lut2stt_attr.be_lut_stt_buf[0].lut_stt_buf[0].phy_addr;
            *(hi_u64 *)(hi_uintptr_t)arg = phy_addr;

            return HI_SUCCESS;
        }

        case ISP_STT_BUF_EXIT: {
            return isp_drv_stt_buf_exit(vi_pipe);
        }

        case ISP_SPECAWB_BUF_INIT: {
            return isp_drv_spec_awb_buf_init(vi_pipe);
        }

        case ISP_SPECAWB_BUF_EXIT: {
            return isp_drv_spec_awb_buf_exit(vi_pipe);
        }

        case ISP_SPECAWB_BUF_GET: {
#ifdef CONFIG_HI_ISP_SPECAWB_SUPPORT
            isp_check_pipe_return(vi_pipe);
            isp_check_pointer_return(arg);
            if (g_isp_drv_ctx[vi_pipe].spec_awb_buf_attr.init == HI_FALSE) {
                return HI_ERR_ISP_NOMEM;
            }
            *(hi_u64 *)(hi_uintptr_t)arg = g_isp_drv_ctx[vi_pipe].spec_awb_buf_attr.spec_awb_buf.phy_addr;

            return HI_SUCCESS;
#else
            return HI_ERR_ISP_NOT_SUPPORT;
#endif
        }
        case ISP_SET_RGBIR_FORMAT: {
            isp_check_pipe_return(vi_pipe);
            g_isp_drv_ctx[vi_pipe].isp_rgbir_format = *(hi_u32 *)(hi_uintptr_t)arg;
            return HI_SUCCESS;
        }
        case ISP_AWB_EN_SET: {
            hi_bool enable;

            isp_check_pointer_return(arg);
            isp_check_bool_return(*(hi_bool *)(hi_uintptr_t)arg);
            enable = *(hi_bool*)(hi_uintptr_t)(arg);

            return isp_set_awb_sta_enable(vi_pipe, enable);
        }
        default: {
            return vreg_drv_ioctl(cmd, arg, private_data);
        }
    }

    return 0;
}

#ifdef CONFIG_COMPAT
static long isp_compat_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    switch (cmd) {
        case ISP_BE_CFG_BUF_INIT: {
            isp_be_wo_cfg_buf *be_wo_cfg_buf = (isp_be_wo_cfg_buf *)(hi_uintptr_t)arg;
            COMPAT_POINTER(be_wo_cfg_buf->vir_addr, hi_void *);
            break;
        }

        default: {
            break;
        }
    }

    return isp_ioctl(cmd, arg, private_data);
}
#endif

static int isp_open(void *data)
{
    return 0;
}

static int isp_close(void *data)
{
    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static hi_s32 isp_freeze(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}

static hi_s32 isp_restore(osal_dev_t *pdev)
{
    hi_vi_pipe vi_pipe;

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        /* enable port int & isp int */
        io_rw_pt_address(VICAP_HD_MASK) |= vicap_int_mask_pt(vi_pipe);
        io_rw_pt_address(VICAP_HD_MASK) |= vicap_int_mask_isp(vi_pipe);

        io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK) = (0x0);
    }

    return HI_SUCCESS;
}
#else
static hi_s32 isp_freeze(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}

static hi_s32 isp_restore(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}
#endif

static struct osal_fileops g_isp_file_ops = {
    .unlocked_ioctl   = isp_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = isp_compat_ioctl,
#endif
    .open             = isp_open,
    .release          = isp_close
};

struct osal_pmops g_isp_drv_ops = {
    .pm_freeze      = isp_freeze,
    .pm_restore     = isp_restore
};

static osal_dev_t *g_isp_device = HI_NULL;

#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
static void isp_drv_work_queue_handler(struct osal_work_struct *worker)
{
    hi_s32 ret;
    isp_work_queue_ctx *isp_work_queue = osal_container_of((void *)worker, isp_work_queue_ctx, worker);
    if (osal_down_interruptible(&isp_work_queue->sem)) {
        return;
    }
    ret = isp_drv_int_bottom_half(0);
    if (ret != OSAL_IRQ_HANDLED) {
        /* do nothing */
    }
    osal_up(&isp_work_queue->sem);

    return;
}

void isp_drv_work_queue_run(hi_vi_pipe vi_pipe)
{
    osal_schedule_work(&g_isp_work_queue_ctx.worker);
}

void isp_drv_work_queue_init(void)
{
    osal_sema_init(&g_isp_work_queue_ctx.sem, 1);
    OSAL_INIT_WORK(&g_isp_work_queue_ctx.worker, isp_drv_work_queue_handler);
}

void isp_drv_work_queue_exit(void)
{
    osal_sema_destory(&g_isp_work_queue_ctx.sem);
    osal_destroy_work(&g_isp_work_queue_ctx.worker);
}
#endif

static hi_void isp_update_interrupt_info(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_interrupt_sch *int_sch)
{
    hi_u8 write_flag;
    unsigned long flags;

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    if (isp_interrupt_buf_is_full(&drv_ctx->isp_int_info)) {
        isp_warn_trace("ISP[%d] interrupts buf is full!\n", vi_pipe);
        osal_memset(&drv_ctx->isp_int_info, 0, sizeof(isp_interrupt_info));
    }

    osal_memset(&drv_ctx->isp_int_info.int_sch[drv_ctx->isp_int_info.write_flag], 0, sizeof(isp_interrupt_sch));

    if (int_sch->isp_int_status || int_sch->port_int_status ||
        ((int_sch->wch_int_status != 0) && (drv_ctx->yuv_mode == HI_TRUE))) {
        write_flag = drv_ctx->isp_int_info.write_flag;
        osal_memcpy(&drv_ctx->isp_int_info.int_sch[write_flag], int_sch, sizeof(isp_interrupt_sch));
        drv_ctx->isp_int_info.write_flag = (write_flag + 1) % ISP_INTERRUPTS_SAVEINFO_MAX;
    }

    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
}

static hi_s32 isp_get_interrupt_info(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_u8 read_flag;
    unsigned long flags;
    isp_interrupt_sch *int_sch = HI_NULL;

    osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
    read_flag = drv_ctx->isp_int_info.read_flag;
    int_sch   = &drv_ctx->isp_int_info.int_sch[read_flag];

    if ((!int_sch->isp_int_status) && (!int_sch->port_int_status) &&
        (!(int_sch->wch_int_status && (drv_ctx->yuv_mode == HI_TRUE)))) {
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return 0;
    }

    if (isp_interrupt_buf_is_empty(&drv_ctx->isp_int_info)) {
        isp_warn_trace("ISP[%d] interrupts buf is empty\n", vi_pipe);
        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
        return 0;
    }

    osal_memcpy(&drv_ctx->int_sch, &drv_ctx->isp_int_info.int_sch[read_flag], sizeof(isp_interrupt_sch));
    osal_memset(&drv_ctx->isp_int_info.int_sch[read_flag], 0, sizeof(isp_interrupt_sch));
    drv_ctx->isp_int_info.read_flag = (read_flag + 1) % ISP_INTERRUPTS_SAVEINFO_MAX;
    osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    return 1;
}

hi_s32 isp_drv_int_status_process(hi_vi_pipe vi_pipe, hi_s32 vi_dev, isp_drv_ctx *drv_ctx)
{
    hi_u32 port_int, port_int_f_start, port_int_err, isp_raw_int, isp_int_status, wch_int, wch_int_f_start;
    isp_interrupt_sch int_sch = { 0 };
    /* read interrupt status */
    port_int  = io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT);
    port_int &= io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT_MASK);
    port_int_f_start = port_int & VI_PT_INT_FSTART;
    port_int_err     = port_int & VI_PT_INT_ERR;

    wch_int         = io_rw_ch_address(vi_pipe, VI_WCH_INT);
    wch_int_f_start = wch_int & VI_WCH_INT_FSTART;

    isp_raw_int    = io_rw_fe_address(vi_pipe, ISP_INT_FE);
    isp_int_status = isp_raw_int & io_rw_fe_address(vi_pipe, ISP_INT_FE_MASK);

    /*
     * openipc_frame_ts: push an ISP_FEND event when the ISP front-end
     * finished receiving the last row from the sensor. Paired with
     * MIPI_FS pushed by the MIPI RX driver at frame-start, consumers
     * see two events per frame and can compute sensor readout duration
     * as wall_ns[FEND] − wall_ns[FS].
     */
    if (isp_int_status & ISP_INT_FE_FEND)
        openipc_frame_ts_push(vi_pipe, OPENIPC_FT_EVT_ISP_FEND);

    int_sch.isp_int_status  = isp_int_status;
    int_sch.port_int_status = port_int_f_start;
    int_sch.port_int_err    = port_int_err;
    int_sch.wch_int_status  = wch_int_f_start;

    isp_update_interrupt_info(vi_pipe, drv_ctx, &int_sch);

    /* clear interrupt */
    if (!port_int && !isp_raw_int) {
        return HI_FAILURE;
    }
    if (port_int) {
        io_rw_pt_address(vi_pt_base(vi_dev) + VI_PT_INT) = port_int;
    }

    if (isp_raw_int) {
        io_rw_fe_address(vi_pipe, ISP_INT_FE) = isp_raw_int;
    }

    if (port_int_err) {
        drv_ctx->drv_dbg_info.isp_reset_cnt++;
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static int isp_isr(int irq, void *id)
{
    hi_u32  i;
    hi_s32  vi_dev, ret;
    hi_vi_pipe vi_pipe;
    hi_u64  isp_irq_begin_time;
    hi_bool vi_cap_int;
    isp_drv_ctx *drv_ctx = HI_NULL;

    vi_cap_int      = (irq == g_isp_fe_irq);

    isp_irq_begin_time = call_sys_get_time_stamp();

    /* Isp FE Interrupt Process Begin */
    if (vi_cap_int) {
        for (i = 0; i < ISP_MAX_PHY_PIPE_NUM; i++) {
            vi_pipe = i;

            drv_ctx = isp_drv_get_ctx(vi_pipe);
            if (drv_ctx->mem_init == HI_FALSE) {
                continue;
            }
            vi_dev  = drv_ctx->wdr_attr.vi_dev;

            drv_ctx->isp_sync_ctrl.pts = isp_irq_begin_time;

            ret = isp_drv_int_status_process(vi_pipe, vi_dev, drv_ctx);
            if (ret != HI_SUCCESS) {
                continue;
            }
        }

        if (!g_use_bottom_half) {
            isp_drv_int_bottom_half(irq);
        } else {
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
#ifdef __HuaweiLite__
            isp_drv_work_queue_run(vi_pipe);
#endif
#endif
        }
    }

    return OSAL_IRQ_WAKE_THREAD;
}

int isp_int_bottom_half(int irq, void *id)
{
    if (g_use_bottom_half) {
        return isp_drv_int_bottom_half(irq);
    } else {
        return OSAL_IRQ_HANDLED;
    }
}

hi_void isp_drv_wake_up_thread(hi_vi_pipe vi_pipe)
{
    hi_bool wake_up_tread = HI_TRUE;
    isp_drv_ctx *drv_ctx  = HI_NULL;

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if (is_offline_mode(drv_ctx->work_mode.running_mode) ||
        is_striping_mode(drv_ctx->work_mode.running_mode)) {
        if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
            if (drv_ctx->running_state != ISP_BE_BUF_STATE_INIT) {
                wake_up_tread = HI_FALSE;
            }
        }
    }

    if (wake_up_tread == HI_TRUE) {
        drv_ctx->edge = HI_TRUE;
        drv_ctx->vd_start = HI_TRUE;

        osal_wakeup(&drv_ctx->isp_wait);
        osal_wakeup(&drv_ctx->isp_wait_vd_start);
    }

    return;
}

hi_s32 isp_drv_get_use_node_yuv_mode(hi_vi_pipe vi_pipe, hi_u32 isp_int_status, hi_u32 wch_int_status)
{
    hi_s32 ret;
    isp_drv_ctx  *drv_ctx  = HI_NULL;
    isp_sync_cfg *sync_cfg = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx  = isp_drv_get_ctx(vi_pipe);
    sync_cfg = &drv_ctx->sync_cfg;

    if ((drv_ctx->yuv_mode == HI_TRUE) && wch_int_status && (drv_ctx->isp_run_flag == HI_TRUE)) {
        if (drv_ctx->use_node) {
            osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

            ret = isp_drv_get_sync_controlnfo(vi_pipe, sync_cfg);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_get_sync_controlnfo");
            ret = isp_drv_calc_sync_cfg(sync_cfg);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_calc_sync_cfg");
            ret = isp_drv_reg_config_isp(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_isp");

            drv_ctx->status = isp_int_status;

            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

            if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
                isp_drv_stat_buf_busy_put(vi_pipe);
            }

            osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
            isp_drv_wake_up_thread(vi_pipe);
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_sync_task_process(vi_pipe);

            return HI_FAILURE;
        }

        osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

        drv_ctx->use_node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);

        if (drv_ctx->use_node == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("Pipe[%d] get FreeBeBuf is fail!\r\n", vi_pipe);

            return HI_FAILURE;
        }

        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_use_node_raw_mode(hi_vi_pipe vi_pipe, hi_u32 isp_int_status, hi_u32 wch_int_status)
{
    hi_s32 ret;
    isp_drv_ctx  *drv_ctx  = HI_NULL;
    isp_sync_cfg *sync_cfg = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx  = isp_drv_get_ctx(vi_pipe);
    sync_cfg = &drv_ctx->sync_cfg;

    if (!is_offline_mode(drv_ctx->work_mode.running_mode) && !is_striping_mode(drv_ctx->work_mode.running_mode)) {
        return HI_SUCCESS;
    }

    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    if (isp_int_status & ISP_1ST_INT) {
        if (drv_ctx->use_node) {
            /* Need to configure the sensor registers and get statistics for AE/AWB. */
            osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

            ret = isp_drv_get_sync_controlnfo(vi_pipe, sync_cfg);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_get_sync_controlnfo");
            ret = isp_drv_calc_sync_cfg(sync_cfg);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_calc_sync_cfg");
            ret = isp_drv_reg_config_isp(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_isp");
            ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

            drv_ctx->status = isp_int_status;

            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);

            if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
                isp_drv_stat_buf_busy_put(vi_pipe);
            }

            osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);
            isp_drv_wake_up_thread(vi_pipe);
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_sync_task_process(vi_pipe);

            return HI_FAILURE;
        }

        osal_spin_lock_irqsave(&g_isp_lock[vi_pipe], &flags);

        drv_ctx->use_node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);

        if (drv_ctx->use_node == HI_NULL) {
            /* Need to configure the sensor registers. */
            ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");
            osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
            isp_err_trace("Pipe[%d] get FreeBeBuf is fail!\r\n", vi_pipe);

            return HI_FAILURE;
        }

        osal_spin_unlock_irqrestore(&g_isp_lock[vi_pipe], &flags);
    }

    return HI_SUCCESS;
}

hi_s32 isp_drv_get_use_node(hi_vi_pipe vi_pipe, hi_u32 isp_int_status, hi_u32 wch_int_status)
{
    hi_s32 ret;

    isp_check_pipe_return(vi_pipe);

    ret = isp_drv_get_use_node_raw_mode(vi_pipe, isp_int_status, wch_int_status);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = isp_drv_get_use_node_yuv_mode(vi_pipe, isp_int_status, wch_int_status);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

#ifdef CONFIG_HI_VI_STITCH_SUPPORT
hi_s32 isp_drv_stitch_get_use_node(hi_vi_pipe vi_pipe, hi_u32 isp_int_status)
{
    hi_s32 ret;
    hi_vi_pipe main_pipe;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_sync_cfg *sync_cfg = HI_NULL;
    unsigned long flags;

    isp_check_pipe_return(vi_pipe);

    drv_ctx   = isp_drv_get_ctx(vi_pipe);
    sync_cfg  = &drv_ctx->sync_cfg;
    main_pipe = drv_ctx->stitch_attr.stitch_bind_id[0];

    if (!is_offline_mode(drv_ctx->work_mode.running_mode) && !is_striping_mode(drv_ctx->work_mode.running_mode)) {
        return HI_SUCCESS;
    }

    isp_check_bebuf_init_return(vi_pipe, drv_ctx->be_buf_info.init);

    if (isp_int_status & ISP_1ST_INT) {
        osal_spin_lock_irqsave(&g_isp_sync_lock[main_pipe], &flags);

        if (drv_ctx->use_node) {
            /* Need to configure the sensor registers and get statistics for AE/AWB. */
            ret = isp_drv_get_sync_controlnfo(vi_pipe, sync_cfg);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_get_sync_controlnfo");
            ret = isp_drv_calc_sync_cfg(sync_cfg);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_calc_sync_cfg");
            ret = isp_drv_reg_config_isp(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_isp");
            ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

            drv_ctx->status = isp_int_status;

            if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
                isp_drv_stat_buf_busy_put(vi_pipe);
            }

            isp_drv_wake_up_thread(vi_pipe);
            osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
            isp_sync_task_process(vi_pipe);

            return HI_FAILURE;
        }

        drv_ctx->use_node = isp_queue_get_free_be_buf(&drv_ctx->be_buf_queue);

        if (drv_ctx->use_node == HI_NULL) {
            /* Need to configure the sensor registers. */
            ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
            isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");
            osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
            isp_err_trace("Pipe[%d] get FreeBeBuf is fail!\r\n", vi_pipe);

            return HI_FAILURE;
        }

        osal_spin_unlock_irqrestore(&g_isp_sync_lock[main_pipe], &flags);
    }

    return HI_SUCCESS;
}
#endif

hi_s32 isp_drv_get_be_buf_use_node(hi_vi_pipe vi_pipe, hi_u32 isp_int_status)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u32 wch_int_f_start;
    isp_check_pipe_return(vi_pipe);

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    wch_int_f_start = drv_ctx->int_sch.wch_int_status;
    if (drv_ctx->stitch_attr.stitch_enable != HI_TRUE) {
        return isp_drv_get_use_node(vi_pipe, isp_int_status, wch_int_f_start);
    } else {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
        return isp_drv_stitch_get_use_node(vi_pipe, isp_int_status);
#else
        return HI_FAILURE;
#endif
    }
}

hi_void isp_drv_proc_get_port_int_time1(isp_drv_ctx *drv_ctx, hi_u32 port_int_f_start, hi_u64 *pt_time1)
{
    if (port_int_f_start) {  /* port int proc */
        drv_ctx->drv_dbg_info.pt_int_cnt++;
        *pt_time1 = call_sys_get_time_stamp();

        if (drv_ctx->drv_dbg_info.pt_last_int_time) {
            drv_ctx->drv_dbg_info.pt_int_gap_time = *pt_time1 - drv_ctx->drv_dbg_info.pt_last_int_time;

            if (drv_ctx->drv_dbg_info.pt_int_gap_time > drv_ctx->drv_dbg_info.pt_int_gap_time_max) {
                drv_ctx->drv_dbg_info.pt_int_gap_time_max = drv_ctx->drv_dbg_info.pt_int_gap_time;
            }
        }

        drv_ctx->drv_dbg_info.pt_last_int_time = *pt_time1;
    }
}

hi_void isp_drv_proc_get_isp_int_time1(isp_drv_ctx *drv_ctx, hi_u32 isp_int_status, hi_u64 *isp_time1)
{
    if (isp_int_status & ISP_1ST_INT) {  /* isp int proc */
        drv_ctx->drv_dbg_info.isp_int_cnt++;
        *isp_time1 = call_sys_get_time_stamp();

        if (drv_ctx->drv_dbg_info.isp_last_int_time) {
            drv_ctx->drv_dbg_info.isp_int_gap_time = *isp_time1 - drv_ctx->drv_dbg_info.isp_last_int_time;

            if (drv_ctx->drv_dbg_info.isp_int_gap_time > drv_ctx->drv_dbg_info.isp_int_gap_time_max) {
                drv_ctx->drv_dbg_info.isp_int_gap_time_max = drv_ctx->drv_dbg_info.isp_int_gap_time;
            }
        }

        drv_ctx->drv_dbg_info.isp_last_int_time = *isp_time1;
    }
}

hi_void isp_drv_proc_calc_port_int(isp_drv_ctx *drv_ctx, hi_u32 port_int_f_start, hi_u64 pt_time1)
{
    hi_u64 pt_time2;

    if (port_int_f_start) { /* port int proc */
        pt_time2 = call_sys_get_time_stamp();
        drv_ctx->drv_dbg_info.pt_int_time = pt_time2 - pt_time1;

        if (drv_ctx->drv_dbg_info.pt_int_time > drv_ctx->drv_dbg_info.pt_int_time_max) {
            drv_ctx->drv_dbg_info.pt_int_time_max = drv_ctx->drv_dbg_info.pt_int_time;
        }

        if ((pt_time2 - drv_ctx->drv_dbg_info.pt_last_rate_time) >= 1000000ul) {
            drv_ctx->drv_dbg_info.pt_last_rate_time = pt_time2;
            drv_ctx->drv_dbg_info.pt_rate = drv_ctx->drv_dbg_info.pt_rate_int_cnt;
            drv_ctx->drv_dbg_info.pt_rate_int_cnt = 0;
        }

        drv_ctx->drv_dbg_info.pt_rate_int_cnt++;
    }
}

hi_void isp_drv_proc_calc_isp_int(isp_drv_ctx *drv_ctx, hi_u32 isp_int_status, hi_u64 isp_time1)
{
    hi_u64 isp_time2;

    if (isp_int_status & ISP_1ST_INT) { /* isp int proc */
        isp_time2 = call_sys_get_time_stamp();
        drv_ctx->drv_dbg_info.isp_int_time = isp_time2 - isp_time1;

        if (drv_ctx->drv_dbg_info.isp_int_time > drv_ctx->drv_dbg_info.isp_int_time_max) {
            drv_ctx->drv_dbg_info.isp_int_time_max = drv_ctx->drv_dbg_info.isp_int_time;
        }

        if ((isp_time2 - drv_ctx->drv_dbg_info.isp_last_rate_time) >= 1000000ul) {
            drv_ctx->drv_dbg_info.isp_last_rate_time = isp_time2;
            drv_ctx->drv_dbg_info.isp_rate = drv_ctx->drv_dbg_info.isp_rate_int_cnt;
            drv_ctx->drv_dbg_info.isp_rate_int_cnt = 0;
        }

        drv_ctx->drv_dbg_info.isp_rate_int_cnt++;
    }
}

hi_void isp_drv_proc_calc_sensor_cfg_time(isp_drv_ctx *drv_ctx)
{
    if (drv_ctx->drv_dbg_info.sensor_cfg_time > drv_ctx->drv_dbg_info.sensor_cfg_time_max) {
        drv_ctx->drv_dbg_info.sensor_cfg_time_max = drv_ctx->drv_dbg_info.sensor_cfg_time;
    }
}

hi_s32 isp_drv_sync_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx)
{
    hi_s32 ret;
    hi_u64 sensor_cfg_time1;
    hi_u64 sensor_cfg_time2;
    isp_sync_cfg *sync_cfg = HI_NULL;

    drv_ctx  = isp_drv_get_ctx(vi_pipe);
    sync_cfg = &drv_ctx->sync_cfg;

    ret = isp_drv_get_sync_controlnfo(vi_pipe, sync_cfg);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_get_sync_controlnfo");
    ret = isp_drv_calc_sync_cfg(sync_cfg);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_calc_sync_cfg");
    ret = isp_drv_reg_config_isp(vi_pipe, drv_ctx);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_isp");

    sensor_cfg_time1 = call_sys_get_time_stamp();
    ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
    isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");
    sensor_cfg_time2 = call_sys_get_time_stamp();
    drv_ctx->drv_dbg_info.sensor_cfg_time = sensor_cfg_time2 - sensor_cfg_time1;

    return HI_SUCCESS;
}


HI_VOID isp_get_slave_pipe_int_status(hi_vi_pipe vi_pipe, hi_bool *slave_pipe_int_active)
{
    hi_u8 chn_num_max;
    hi_u8 k;
    hi_u8 int_status_index;
    isp_drv_ctx *drv_ctx   = HI_NULL;
    isp_drv_ctx *drv_ctx_s = HI_NULL;
    hi_u32  isp_int_status = 0;
    hi_vi_pipe vi_pipe_bind;
    if (g_long_frm_int_en == HI_FALSE) {
        *slave_pipe_int_active = HI_FALSE;
        return ;
    }

    drv_ctx = isp_drv_get_ctx(vi_pipe);
    if ((drv_ctx->wdr_attr.mast_pipe == HI_FALSE) ||
       (IS_FS_WDR_MODE(drv_ctx->wdr_attr.wdr_mode) == HI_FALSE)) {
        osal_memset(&(drv_ctx->slave_pipe_int_status), 0, sizeof(isp_slave_pipe_int_status) * (ISP_WDR_CHN_MAX - 1));
        *slave_pipe_int_active = HI_FALSE;
        return ;
    }

    chn_num_max = CLIP3(drv_ctx->wdr_attr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    int_status_index = 0;
    for (k = 0; k < chn_num_max; k++) {
        vi_pipe_bind = drv_ctx->wdr_attr.dev_bind_pipe.pipe_id[k];
        if (vi_pipe == vi_pipe_bind) {
            continue;
        } else {
            drv_ctx_s = isp_drv_get_ctx(vi_pipe_bind);
            isp_int_status  = drv_ctx_s->int_sch.isp_int_status;

            drv_ctx->slave_pipe_int_status[int_status_index].vi_pipe_id = vi_pipe_bind;
            drv_ctx->slave_pipe_int_status[int_status_index].isp_int_status = isp_int_status ;
            int_status_index++;
            if (isp_int_status != 0) {
                *slave_pipe_int_active = HI_TRUE;
            }
        }
    }

    return ;
}

isp_cfg_sensor_time isp_drv_get_sns_cfg_int_pos(isp_drv_ctx *drv_ctx, hi_vi_pipe vi_pipe_bind)
{
    hi_vi_pipe wdr_short_frame_pipe_id;
    hi_vi_pipe wdr_long_frame_pipe_id;
    hi_vi_pipe status0_pipe_id = drv_ctx->slave_pipe_int_status[0].vi_pipe_id;
    hi_vi_pipe status1_pipe_id = drv_ctx->slave_pipe_int_status[1].vi_pipe_id;  /* 1 */
    hi_vi_pipe status2_pipe_id = drv_ctx->slave_pipe_int_status[2].vi_pipe_id;  /* 2 */

    if (drv_ctx->wdr_attr.wdr_mode == WDR_MODE_2To1_LINE) {
        return ISP_CFG_SNS_SHORT_FRAME;
    }
    if (drv_ctx->wdr_attr.wdr_mode == WDR_MODE_3To1_LINE) {
        if (status0_pipe_id < status1_pipe_id) {
            wdr_short_frame_pipe_id = status0_pipe_id;
        } else {
            wdr_short_frame_pipe_id = status1_pipe_id;
        }
        if (vi_pipe_bind == wdr_short_frame_pipe_id) {
            return ISP_CFG_SNS_SHORT_FRAME;
        } else {
            return ISP_CFG_SNS_MIDDLE_FRAME;
        }
    }
    if (drv_ctx->wdr_attr.wdr_mode == WDR_MODE_4To1_LINE) {
        if ((status0_pipe_id < status1_pipe_id) && (status0_pipe_id < status2_pipe_id)) {
            wdr_short_frame_pipe_id = status0_pipe_id;
        } else if ((status1_pipe_id < status0_pipe_id) && (status1_pipe_id < status2_pipe_id)) {
            wdr_short_frame_pipe_id = status1_pipe_id;
        } else {
            wdr_short_frame_pipe_id = status2_pipe_id;
        }
        if ((status0_pipe_id > status1_pipe_id) && (status0_pipe_id > status2_pipe_id)) {
            wdr_long_frame_pipe_id = status0_pipe_id;
        } else if ((status1_pipe_id > status0_pipe_id) && (status1_pipe_id > status2_pipe_id)) {
            wdr_long_frame_pipe_id = status1_pipe_id;
        } else {
            wdr_long_frame_pipe_id = status2_pipe_id;
        }
        if (vi_pipe_bind == wdr_short_frame_pipe_id) {
            return ISP_CFG_SNS_SHORT_FRAME;
        } else if (vi_pipe_bind == wdr_long_frame_pipe_id) {
            return ISP_CFG_SNS_LONG_FRAME;
        } else {
            return ISP_CFG_SNS_MIDDLE_FRAME;
        }
    }
    return ISP_CFG_SNS_SHORT_FRAME;
}
hi_void isp_long_frm_cfg_sensor(hi_vi_pipe vi_pipe)
{
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u32 bind_pipe_int_status;
    hi_u32 pipe_int_pos_save;
    hi_u32 pipe_int_status_save;
    hi_u32 k;
    hi_vi_pipe vi_pipe_bind;
    hi_u32 cfg_sns_int_pos;
    if (g_long_frm_int_en == HI_FALSE) {
        return;
    }
    drv_ctx  = isp_drv_get_ctx(vi_pipe);
    if (IS_FS_WDR_MODE(drv_ctx->wdr_attr.wdr_mode) == HI_FALSE) {
        return;
    }

    pipe_int_status_save = drv_ctx->status ;
    pipe_int_pos_save    = drv_ctx->int_pos;

    for (k = 0; k < ISP_WDR_CHN_MAX - 1; k++) {
        bind_pipe_int_status = drv_ctx->slave_pipe_int_status[k].isp_int_status;
        vi_pipe_bind = drv_ctx->slave_pipe_int_status[k].vi_pipe_id;
        cfg_sns_int_pos = isp_drv_get_sns_cfg_int_pos(drv_ctx, vi_pipe_bind);
        if (bind_pipe_int_status & ISP_1ST_INT) {
            drv_ctx->int_pos = ((cfg_sns_int_pos << 4) & 0xf0) + 0; /* 4, 0:frame start */
            drv_ctx->status = bind_pipe_int_status;
            isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        }
        if (bind_pipe_int_status & ISP_2ND_INT) {
            drv_ctx->int_pos = ((cfg_sns_int_pos << 4) & 0xf0) + 1; /* 4, 1:frame end */
            drv_ctx->status = bind_pipe_int_status;
            isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        }
    }
    drv_ctx->status = pipe_int_status_save;
    drv_ctx->int_pos = pipe_int_pos_save;
    return ;
}

#ifdef CONFIG_HI_SNAP_SUPPORT
hi_s32 isp_irq_snap_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_sync_cfg *sync_cfg)
{
    hi_s32 ret;
    hi_u32 isp_int_status, sensor_cfg_int;

    isp_int_status  = drv_ctx->int_sch.isp_int_status;
    if (isp_int_status & ISP_1ST_INT) {
        if (vi_pipe == drv_ctx->snap_attr.picture_pipe_id) {
            if (ckfn_vi_get_isp_config()) {
                call_vi_get_isp_config(vi_pipe, &drv_ctx->snap_info_load);
            }
        }
    }

    drv_ctx->int_pos = 0;
    sensor_cfg_int = (isp_int_status & ISP_2ND_INT);
    if (g_update_pos[vi_pipe] == 0) {  /* frame start */
        sensor_cfg_int = (isp_int_status & ISP_1ST_INT);
    }

    if (sensor_cfg_int) {
        ret = isp_drv_sync_process(vi_pipe, drv_ctx);
        isp_check_ret(vi_pipe, ret, "isp_drv_sync_process");
    }

    if (isp_int_status & ISP_2ND_INT) {
        drv_ctx->int_pos = 1;

        ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

        drv_ctx->vd_end = HI_TRUE;
        if (drv_ctx->pro_trig_flag == 1) {
            drv_ctx->pro_trig_flag++;
        }
        isp_update_info_sync(vi_pipe, &drv_ctx->update_info);
        isp_frame_info_sync(vi_pipe, &drv_ctx->frame_info);
        osal_wakeup(&drv_ctx->isp_wait_vd_end);
    }

    if (isp_int_status & ISP_1ST_INT) {
        drv_ctx->status = isp_int_status;

        if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
            isp_drv_stat_buf_busy_put(vi_pipe);
        }

        isp_drv_wake_up_thread(vi_pipe);
        /* Sync  task AF statistics */
        isp_sync_task_process(vi_pipe);
    }

    return HI_SUCCESS;
}
#endif

hi_s32 isp_irq_full_wdr_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_sync_cfg *sync_cfg)
{
    hi_s32 ret;
    hi_u32 isp_int_status;

    isp_int_status  = drv_ctx->int_sch.isp_int_status;

    drv_ctx->int_pos = 0;
    if (isp_int_status & ISP_1ST_INT) {
        if (is_full_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            ret = isp_drv_sync_process(vi_pipe, drv_ctx);
            isp_check_ret(vi_pipe, ret, "isp_drv_sync_process");
        }
    }

    if (isp_int_status & ISP_2ND_INT) {
        drv_ctx->int_pos = 1;

        ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

        drv_ctx->vd_end = HI_TRUE;
        isp_update_info_sync(vi_pipe, &drv_ctx->update_info);
        isp_frame_info_sync(vi_pipe, &drv_ctx->frame_info);
        osal_wakeup(&drv_ctx->isp_wait_vd_end);
    }

    if (isp_int_status & ISP_1ST_INT) {
        /* N to 1 fullrate frame WDR mode, get statistics only in the last frame(N-1) */
        if (is_full_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            if (drv_ctx->sync_cfg.vc_num != drv_ctx->sync_cfg.vc_num_max) {
                return HI_SUCCESS;
            }
        }

        drv_ctx->status = isp_int_status;

        if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
            isp_drv_stat_buf_busy_put(vi_pipe);
        }

        isp_drv_wake_up_thread(vi_pipe);
        /* Sync  task AF statistics */
        isp_sync_task_process(vi_pipe);
    }
    isp_long_frm_cfg_sensor(vi_pipe);
    return HI_SUCCESS;
}

hi_s32 isp_irq_half_wdr_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_sync_cfg *sync_cfg)
{
    hi_s32 ret;
    hi_u32 port_int_f_start, isp_int_status;

    port_int_f_start = drv_ctx->int_sch.port_int_status;
    isp_int_status   = drv_ctx->int_sch.isp_int_status;

    drv_ctx->int_pos = 0;
    if (port_int_f_start) {
        if (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            ret = isp_drv_sync_process(vi_pipe, drv_ctx);
            isp_check_ret(vi_pipe, ret, "isp_drv_sync_process");
        }
    }

    if (isp_int_status & ISP_2ND_INT) {
        drv_ctx->int_pos = 1;

        ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

        drv_ctx->vd_end = HI_TRUE;
        isp_update_info_sync(vi_pipe, &drv_ctx->update_info);
        isp_frame_info_sync(vi_pipe, &drv_ctx->frame_info);
        osal_wakeup(&drv_ctx->isp_wait_vd_end);
    }

    if (isp_int_status & ISP_1ST_INT) {
        drv_ctx->status = isp_int_status;

        if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
            isp_drv_stat_buf_busy_put(vi_pipe);
        }

        isp_drv_wake_up_thread(vi_pipe);
        /* Sync  task AF statistics */
        isp_sync_task_process(vi_pipe);
    }
    isp_long_frm_cfg_sensor(vi_pipe);
    return HI_SUCCESS;
}

hi_s32 isp_irq_line_wdr_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_sync_cfg *sync_cfg)
{
    hi_s32 ret;
    hi_u32 isp_int_status;

    isp_int_status  = drv_ctx->int_sch.isp_int_status;

    drv_ctx->int_pos = 0;
    if (isp_int_status & ISP_1ST_INT) {
        if (is_line_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            ret = isp_drv_sync_process(vi_pipe, drv_ctx);
            isp_check_ret(vi_pipe, ret, "isp_drv_sync_process");
        }
    }

    if (isp_int_status & ISP_2ND_INT) {
        drv_ctx->int_pos = 1;

        ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

        drv_ctx->vd_end = HI_TRUE;

        isp_update_info_sync(vi_pipe, &drv_ctx->update_info);
        isp_frame_info_sync(vi_pipe, &drv_ctx->frame_info);
        osal_wakeup(&drv_ctx->isp_wait_vd_end);
    }

    if (isp_int_status & ISP_1ST_INT) {
        drv_ctx->status = isp_int_status;

        if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
            isp_drv_stat_buf_busy_put(vi_pipe);
        }

        isp_drv_wake_up_thread(vi_pipe);
        /* Sync  task AF statistics */
        isp_sync_task_process(vi_pipe);
    }
    isp_long_frm_cfg_sensor(vi_pipe);
    return HI_SUCCESS;
}

hi_s32 isp_irq_linear_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_sync_cfg *sync_cfg)
{
    hi_s32 ret;
    hi_u32 isp_int_status, sensor_cfg_int;

    isp_int_status  = drv_ctx->int_sch.isp_int_status;

    drv_ctx->int_pos = 0;
    sensor_cfg_int = (isp_int_status & ISP_2ND_INT);
    if (g_update_pos[vi_pipe] == 0) {  /* frame start */
        sensor_cfg_int = (isp_int_status & ISP_1ST_INT);
    }
    if (sensor_cfg_int) {
        if (is_linear_mode(drv_ctx->sync_cfg.wdr_mode) || is_built_in_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            ret = isp_drv_sync_process(vi_pipe, drv_ctx);
            isp_check_ret(vi_pipe, ret, "isp_drv_sync_process");
        }
    }

    if (isp_int_status & ISP_2ND_INT) {
        drv_ctx->int_pos = 1;

        ret = isp_drv_reg_config_sensor(vi_pipe, drv_ctx);
        isp_check_ret_continue(vi_pipe, ret, "isp_drv_reg_config_sensor");

        drv_ctx->vd_end = HI_TRUE;
        if (drv_ctx->pro_trig_flag == 1) {
            drv_ctx->pro_trig_flag++;
        }
        isp_update_info_sync(vi_pipe, &drv_ctx->update_info);
        isp_frame_info_sync(vi_pipe, &drv_ctx->frame_info);
        osal_wakeup(&drv_ctx->isp_wait_vd_end);
    }

    if (isp_int_status & ISP_1ST_INT) {
        drv_ctx->status = isp_int_status;

        if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
            isp_drv_stat_buf_busy_put(vi_pipe);
        }

        isp_drv_wake_up_thread(vi_pipe);
        /* Sync  task AF statistics */
        isp_sync_task_process(vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 isp_irq_yuv_process(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, isp_sync_cfg *sync_cfg)
{
    hi_u32 wch_int_f_start;

    wch_int_f_start = drv_ctx->int_sch.wch_int_status;

    if (wch_int_f_start && (drv_ctx->yuv_mode == HI_TRUE) && (drv_ctx->isp_run_flag == HI_TRUE)) {  /* WCH int */
        drv_ctx->status = wch_int_f_start;

        if (drv_ctx->frame_cnt++ % div_0_to_1(g_stat_intvl[vi_pipe]) == 0) {
            isp_drv_stat_buf_busy_put(vi_pipe);
        }

        isp_drv_wake_up_thread(vi_pipe);
        /* Sync  task AF statistics */
        isp_sync_task_process(vi_pipe);
    }

    return HI_SUCCESS;
}

int  isp_irq_route(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_drv_ctx *drv_ctx = HI_NULL;
    isp_sync_cfg *sync_cfg = HI_NULL;
    hi_u32 port_int_f_start, isp_int_status;
    hi_u64 pt_time1 = 0;
    hi_u64 isp_time1 = 0;

    isp_check_pipe_return(vi_pipe);

    drv_ctx  = isp_drv_get_ctx(vi_pipe);
    sync_cfg = &drv_ctx->sync_cfg;

    port_int_f_start = drv_ctx->int_sch.port_int_status;
    isp_int_status   = drv_ctx->int_sch.isp_int_status;

    isp_drv_proc_get_port_int_time1(drv_ctx, port_int_f_start, &pt_time1);
    isp_drv_proc_get_isp_int_time1(drv_ctx, isp_int_status, &isp_time1);

    drv_ctx->int_pos = 0;

    ret = isp_drv_get_be_buf_use_node(vi_pipe, isp_int_status);
    if (ret != HI_SUCCESS) {
        return OSAL_IRQ_HANDLED;
    }

    if (vi_pipe != drv_ctx->snap_attr.picture_pipe_id) {
        if (is_linear_mode(drv_ctx->sync_cfg.wdr_mode) || is_built_in_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            isp_irq_linear_process(vi_pipe, drv_ctx, sync_cfg);
        }

        if (is_line_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            isp_irq_line_wdr_process(vi_pipe, drv_ctx, sync_cfg);
        }

        if (is_half_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            isp_irq_half_wdr_process(vi_pipe, drv_ctx, sync_cfg);
        }

        if (is_full_wdr_mode(drv_ctx->sync_cfg.wdr_mode)) {
            isp_irq_full_wdr_process(vi_pipe, drv_ctx, sync_cfg);
        }
    } else {
#ifdef CONFIG_HI_SNAP_SUPPORT
        isp_irq_snap_process(vi_pipe, drv_ctx, sync_cfg);
#endif
    }

    if (drv_ctx->yuv_mode == HI_TRUE) {
        isp_irq_yuv_process(vi_pipe, drv_ctx, sync_cfg);
    }

    isp_drv_proc_calc_sensor_cfg_time(drv_ctx);
    isp_drv_proc_calc_port_int(drv_ctx, port_int_f_start, pt_time1);
    isp_drv_proc_calc_isp_int(drv_ctx, isp_int_status, isp_time1);

    return OSAL_IRQ_HANDLED;
}

int  isp_drv_int_bottom_half(int irq)
{
    hi_vi_pipe vi_pipe;
    hi_u32 i;
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
    hi_vi_pipe vi_pipes;
    hi_u32 j;
    hi_s32 ret;
    isp_drv_ctx *drv_ctx_s = HI_NULL;
#endif
    isp_drv_ctx *drv_ctx = HI_NULL;
    hi_u32  port_int_status = 0;

    hi_u32  isp_int_status = 0;
    hi_u32  wch_int_f_start = 0;
    hi_bool slave_pipe_int_active;
    for (i = 0; i < ISP_MAX_PHY_PIPE_NUM; i++) {
        vi_pipe = i;
        drv_ctx = isp_drv_get_ctx(vi_pipe);
        if (isp_get_interrupt_info(vi_pipe, drv_ctx) == 0) {
            continue;
        }

        isp_int_status  = drv_ctx->int_sch.isp_int_status;
        port_int_status = drv_ctx->int_sch.port_int_status;
        wch_int_f_start  = drv_ctx->int_sch.wch_int_status;
        isp_get_slave_pipe_int_status(vi_pipe, &slave_pipe_int_active);
        if (!port_int_status && !isp_int_status && !wch_int_f_start && (slave_pipe_int_active == HI_FALSE)) {
            continue;
        }

        if (!drv_ctx->mem_init) {
            continue;
        }

        if (drv_ctx->stitch_attr.stitch_enable == HI_TRUE) {
#ifdef CONFIG_HI_VI_STITCH_SUPPORT
            if (drv_ctx->stitch_attr.main_pipe == HI_FALSE) {
                continue;
            }
            ret = isp_drv_stitch_sync(vi_pipe);
            if (ret != HI_SUCCESS) {
                continue;
            }

            for (j = 0; j < drv_ctx->stitch_attr.stitch_pipe_num; j++) {
                vi_pipes = drv_ctx->stitch_attr.stitch_bind_id[j];
                drv_ctx_s = isp_drv_get_ctx(vi_pipes);

                drv_ctx_s->int_sch.isp_int_status  = isp_int_status;
                drv_ctx_s->int_sch.port_int_status = port_int_status;
                drv_ctx_s->int_sch.port_int_err    = drv_ctx->int_sch.port_int_err;

                isp_irq_route(vi_pipes);
            }
#endif
        } else {
            isp_irq_route(vi_pipe);
        }
    }

    return OSAL_IRQ_HANDLED;
}

static int isp_drv_init(void)
{
    hi_s32 ret;

    ret = isp_drv_be_remap();
    if (ret == HI_FAILURE) {
        return ret;
    }

    ret = isp_drv_vicap_remap();
    if (ret == HI_FAILURE) {
        return ret;
    }

    ret = isp_drv_fe_remap();
    if (ret == HI_FAILURE) {
        return ret;
    }
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    isp_drv_work_queue_init();
#endif
    return 0;
}

static int isp_drv_exit(void)
{
    isp_drv_be_unmap();

    isp_drv_vicap_unmap();

    isp_drv_fe_unmap();
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    isp_drv_work_queue_exit();
#endif

    return 0;
}

#ifndef DISABLE_DEBUG_INFO
static hi_void isp_proc_module_param_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    osal_seq_printf(s, "-----MODULE/CONTROL PARAM-----------------------------------------------------\n");
    osal_seq_printf(s, " %12s" " %12s" " %12s" " %12s" " %12s" " %12s" "\n", "ProcParam",
                    "StatIntvl", "UpdatePos", "IntBothalf", "IntTimeout", "PwmNumber");
    osal_seq_printf(s, " %12u" " %12u" " %12u" " %12u" " %12u" " %12u" "\n",
                    g_proc_param[vi_pipe], g_stat_intvl[vi_pipe], g_update_pos[vi_pipe], g_use_bottom_half,
                    g_int_timeout[vi_pipe], g_pwm_number[vi_pipe]);
    osal_seq_printf(s, "\n");

    osal_seq_printf(s, " %14s" " %12s" " %12s" " %12s" " %10s""\n",
                    "PortIntDelay", "QuickStart", "LdciTprFltEn", "LongFrmIntEn", "BeBufNum");
    osal_seq_printf(s, " %14u" " %12d" " %12d" " %12d" " %10d" "\n",
                    g_port_int_delay[vi_pipe], g_quick_start, g_ldci_tpr_flt_en[vi_pipe],
                    g_long_frm_int_en, g_be_buf_num[vi_pipe]);
    osal_seq_printf(s, "\n");
}

static hi_void isp_proc_isp_mode_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    osal_seq_printf(s, "-----ISP Mode------------------------------------------------------------------\n");
    osal_seq_printf(s, " %15s" " %15s" "\n",  "StitchMode", "RunningMode");
    osal_seq_printf(s, " %15s" " %15s" "\n",
                    drv_ctx->stitch_attr.stitch_enable ? "STITCH" : "NORMAL",
                    (drv_ctx->work_mode.running_mode == ISP_MODE_RUNNING_OFFLINE) ? "OFFLINE"  :
                    (drv_ctx->work_mode.running_mode == ISP_MODE_RUNNING_ONLINE) ? "ONLINE"   :
                    (drv_ctx->work_mode.running_mode == ISP_MODE_RUNNING_SIDEBYSIDE) ? "SBS"      :
                    (drv_ctx->work_mode.running_mode == ISP_MODE_RUNNING_STRIPING) ? "STRIPING" : "BUTT");
    osal_seq_printf(s, "\n");
}

static hi_void isp_proc_sensor_info_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    hi_isp_sns_type sns_type;
    hi_s32 sns_dev;
    hi_char sns_type_name[MAX_MMZ_NAME_LEN] = { 0 };
    if (drv_ctx->sync_cfg.node[0] == HI_NULL) {
        return;
    }

    sns_type = drv_ctx->sync_cfg.node[0]->sns_regs_info.sns_type;
    if (sns_type == ISP_SNS_I2C_TYPE) {
        sns_dev = drv_ctx->sync_cfg.node[0]->sns_regs_info.com_bus.i2c_dev;
        osal_snprintf(sns_type_name, sizeof(sns_type_name), "I2C");
    } else if (sns_type == ISP_SNS_SSP_TYPE) {
        sns_dev = drv_ctx->sync_cfg.node[0]->sns_regs_info.com_bus.ssp_dev.bit4_ssp_dev;
        osal_snprintf(sns_type_name, sizeof(sns_type_name), "SSP");
    } else {
        sns_dev = -1;
        osal_snprintf(sns_type_name, sizeof(sns_type_name), "BUTT");
    }

    osal_seq_printf(s, "-----SENSOR INFO----------------------------------------------------------------\n");

    osal_seq_printf(s, "%14s" "%10s\n", "SensorType", "Dev");
    osal_seq_printf(s, "%14s" "%10d\n\n", sns_type_name, sns_dev);
}

static hi_void isp_proc_int_status_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    osal_seq_printf(s, "-----DRV INFO-------------------------------------------------------------------\n");

    osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%12s" "%14s\n",
                    "ViPipe", "IntCnt", "IntT", "MaxIntT", "IntGapT", "MaxGapT", "IntRat",
                    "IspResetCnt", "IspBeStaLost");

    osal_seq_printf(s, "%11d" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%12d" "%14d\n", vi_pipe,
                    drv_ctx->drv_dbg_info.isp_int_cnt, drv_ctx->drv_dbg_info.isp_int_time,
                    drv_ctx->drv_dbg_info.isp_int_time_max, drv_ctx->drv_dbg_info.isp_int_gap_time,
                    drv_ctx->drv_dbg_info.isp_int_gap_time_max, drv_ctx->drv_dbg_info.isp_rate,
                    drv_ctx->drv_dbg_info.isp_reset_cnt, drv_ctx->drv_dbg_info.isp_be_sta_lost);

    osal_seq_printf(s, "\n");

    osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%11s" "%12s\n", "IntTpye", "PtIntCnt",
                    "PtIntT", "PtMaxIntT", "PtIntGapT", "PtMaxGapT", "PtIntRat", "SensorCfgT", "SensorMaxT");

    osal_seq_printf(s, "%11s" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%11d" "%12d\n",
                    (drv_ctx->frame_int_attr.int_type == FRAME_INTERRUPT_START) ? "Start" : "Other",
                    drv_ctx->drv_dbg_info.pt_int_cnt, drv_ctx->drv_dbg_info.pt_int_time,
                    drv_ctx->drv_dbg_info.pt_int_time_max, drv_ctx->drv_dbg_info.pt_int_gap_time,
                    drv_ctx->drv_dbg_info.pt_int_gap_time_max, drv_ctx->drv_dbg_info.pt_rate,
                    drv_ctx->drv_dbg_info.sensor_cfg_time, drv_ctx->drv_dbg_info.sensor_cfg_time_max);

    osal_seq_printf(s, "\n");
}

static hi_void isp_proc_be_cfg_phy_addr_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    hi_u8  i, j, max_loop, remainder, loop, index;
    hi_u64 be_buf_size;

    if (is_online_mode(drv_ctx->work_mode.running_mode) ||
        is_sidebyside_mode(drv_ctx->work_mode.running_mode) ||
        (drv_ctx->be_buf_info.init == HI_FALSE)) {
        return;
    }

    osal_seq_printf(s, "\n");
    osal_seq_printf(s, "-----BeCfg Phy Addr-------------------------------------------------------- \n");

    remainder = g_be_buf_num[vi_pipe] % 4; /* const 4 */
    max_loop  = g_be_buf_num[vi_pipe] / 4; /* const 4 */
    if (remainder != 0) {
        max_loop += 1;
    }

    loop = 4; /* const 4 */
    be_buf_size = sizeof(isp_be_wo_reg_cfg);

    for (j = 0; j < max_loop; j++) {
        if ((j + 1) * 4 > g_be_buf_num[vi_pipe]) { /* const 4 */
            loop = remainder;
        }

        for (i = 0; i < loop; i++) {
            osal_seq_printf(s, "%16s""%d""%s", "BeCfg[", j * 4 + i, "]"); /* const 4 */
        }

        osal_seq_printf(s, "\n");

        for (i = 0; i < loop; i++) {
            index = j * 4 + i; /* const 4 */
            osal_seq_printf(s, "%#18llx", drv_ctx->be_buf_info.be_buf_haddr.phy_addr + index * be_buf_size); /* 4 */
        }

        osal_seq_printf(s, "\n\n");
    }
}

static hi_void isp_proc_pub_attr_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    /*  show isp attribute here. width/height/bayer_format, etc..
              Read parameter from memory directly. */
    osal_seq_printf(s, "-----PubAttr INFO--------------------------------------------------------------\n");

    osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s\n",
                    "WndX", "WndY", "WndW", "WndH", "SnsW", "SnsH", "Bayer");

    osal_seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12s\n\n",
                    drv_ctx->proc_pub_info.wnd_rect.x, drv_ctx->proc_pub_info.wnd_rect.y,
                    drv_ctx->proc_pub_info.wnd_rect.width, drv_ctx->proc_pub_info.wnd_rect.height,
                    drv_ctx->proc_pub_info.sns_size.width, drv_ctx->proc_pub_info.sns_size.height,
                    (drv_ctx->proc_pub_info.bayer == BAYER_RGGB) ? "RGGB" :
                    (drv_ctx->proc_pub_info.bayer == BAYER_GRBG) ? "GRBG" :
                    (drv_ctx->proc_pub_info.bayer == BAYER_GBRG) ? "GBRG" :
                    (drv_ctx->proc_pub_info.bayer == BAYER_BGGR) ? "BGGR" : "BUTT");
    osal_seq_printf(s, "\n");
}

static hi_void isp_proc_snap_attr_show(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx, osal_proc_entry_t *s)
{
    isp_snap_pipe_mode snap_pipe_mode = ISP_SNAP_NONE;

    if ((drv_ctx->snap_attr.picture_pipe_id == drv_ctx->snap_attr.preview_pipe_id) &&
        (drv_ctx->snap_attr.preview_pipe_id != -1)) {
        snap_pipe_mode = ISP_SNAP_PREVIEW_PICTURE;
    } else if (drv_ctx->snap_attr.picture_pipe_id == vi_pipe) {
        snap_pipe_mode = ISP_SNAP_PICTURE;
    } else if (drv_ctx->snap_attr.preview_pipe_id == vi_pipe) {
        snap_pipe_mode = ISP_SNAP_PREVIEW;
    } else {
        snap_pipe_mode = ISP_SNAP_NONE;
    }

    /* show isp snap attribute here. SnapType/PipeMode/OPType/ProFrmNum, etc.. */
    osal_seq_printf(s, "-----SNAPATTR INFO-----------------------------------------------------------------\n");

    osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s\n", "SnapType", "PipeMode", "OPType", "ProFrmNum");
    osal_seq_printf(s, "%12s" "%12s" "%12s" "%12d\n\n",
                    (drv_ctx->snap_attr.snap_type == 0) ? "NORMAL" :
                    (drv_ctx->snap_attr.snap_type == 1) ? "PRO" : "BUTT",
                    (snap_pipe_mode == ISP_SNAP_NONE) ? "NONE" :
                    (snap_pipe_mode == ISP_SNAP_PREVIEW) ? "PREVIEW" :
                    (snap_pipe_mode == ISP_SNAP_PICTURE) ? "PICTURE" : "PRE_PIC",
                    (drv_ctx->snap_attr.pro_param.operation_mode == 0) ? "Auto" : "Manul",
                    drv_ctx->snap_attr.pro_param.pro_frame_num);
}

static int isp_proc_show(osal_proc_entry_t *s)
{
    hi_s32 ret;
    hi_vi_pipe  vi_pipe = 0;
    isp_drv_ctx *drv_ctx = HI_NULL;

    osal_seq_printf(s, "\n[ISP] Version: ["MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n\n");
    osal_seq_printf(s, "\n");
    do {
        drv_ctx = isp_drv_get_ctx(vi_pipe);
        if (!drv_ctx->mem_init) {
            continue;
        }

        osal_seq_printf(s, "------------------------------------------------------------------------------\n");
        osal_seq_printf(s, "------------------------------ ISP PROC PIPE[%d] -----------------------------\n", vi_pipe);
        osal_seq_printf(s, "------------------------------------------------------------------------------\n");
        osal_seq_printf(s, "\n");

        isp_proc_module_param_show(vi_pipe, drv_ctx, s);
        isp_proc_isp_mode_show(vi_pipe, drv_ctx, s);
        isp_proc_sensor_info_show(vi_pipe, drv_ctx, s);
        isp_proc_int_status_show(vi_pipe, drv_ctx, s);
        isp_proc_be_cfg_phy_addr_show(vi_pipe, drv_ctx, s);
        isp_proc_pub_attr_show(vi_pipe, drv_ctx, s);
        isp_proc_snap_attr_show(vi_pipe, drv_ctx, s);

        ret = isp_drv_proc_printf(vi_pipe, s);
        if (ret != HI_SUCCESS) {
            /* do nothing */
        }

        osal_seq_printf(s, "-----------------------------------------------------------------------------\n");
        osal_seq_printf(s, "----------------------------------- ISP PROC END[%d] ------------------------\n", vi_pipe);
        osal_seq_printf(s, "-----------------------------------------------------------------------------\n\n\n");
    } while (++vi_pipe < ISP_MAX_PIPE_NUM);

    return 0;
}
#endif

hi_s32 isp_kern_init(void *p)
{
    hi_u32 vi_pipe;

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        osal_memset(&g_isp_drv_ctx[vi_pipe].drv_dbg_info, 0, sizeof(isp_drv_dbg_info));
    }

    return HI_SUCCESS;
}

hi_void isp_kern_exit(void)
{
    hi_u32   vi_pipe;
    hi_void  *reg_vicap_base_va                      = HI_NULL;
    hi_void  *reg_ispfe_base_va[ISP_MAX_PIPE_NUM]    = {HI_NULL};

    reg_vicap_base_va = isp_drv_get_reg_vicap_base_va();
    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        reg_ispfe_base_va[vi_pipe] = isp_drv_get_ispfe_base_va(vi_pipe);
        if ((reg_vicap_base_va != HI_NULL) && (reg_ispfe_base_va[vi_pipe] != HI_NULL)) {
            isp_drv_set_int_enable(vi_pipe, HI_FALSE);
        }
        osal_memset(&g_isp_drv_ctx[vi_pipe].drv_dbg_info, 0, sizeof(isp_drv_dbg_info));
    }

    return;
}

static hi_u32 isp_get_ver_magic(hi_void)
{
    return VERSION_MAGIC;
}

isp_export_func g_isp_exp_func   = {
    .pfn_isp_register_bus_callback       = isp_register_bus_call_back,
    .pfn_isp_register_piris_callback     = isp_register_piris_call_back,
    .pfn_isp_register_vi_bus_callback    = isp_register_vi_bus_call_back,
    .pfn_isp_get_dcf_info                = isp_get_dcf_info,
    .pfn_isp_get_frame_info              = isp_get_frame_info,
    .pfn_isp_get_attach_info             = isp_get_attach_info,
    .pfn_isp_get_color_gamut_info        = isp_get_color_gamut_info,
    .pfn_isp_get_dng_image_dynamic_info  = isp_drv_get_dng_image_dynamic_info,
#ifdef CONFIG_HI_SNAP_SUPPORT
    .pfn_isp_get_pro_ctrl                = isp_get_pro_ctrl,
    .pfn_isp_set_snap_attr               = isp_set_snap_attr,
    .pfn_isp_set_pro_nr_param            = isp_set_pro_nr_param,
    .pfn_isp_set_pro_shp_param           = isp_set_pro_shp_param,
    .pfn_isp_get_pro_nr_param            = isp_get_pro_nr_param,
    .pfn_isp_get_pro_shp_param           = isp_get_pro_shp_param,
    .pfn_isp_save_snap_config            = isp_save_snap_config,
    .pfn_isp_set_pro_enable              = isp_set_pro_enable,
#endif
    .pfn_isp_drv_get_ready_be_buf        = isp_drv_get_ready_be_buf,
    .pfn_isp_drv_put_free_be_buf         = isp_drv_put_free_be_buf,
    .pfn_isp_drv_hold_busy_be_buf        = isp_drv_hold_busy_be_buf,
    .pfn_isp_drv_get_be_sync_para        = isp_drv_get_be_sync_para,
    .pfn_isp_drv_be_end_int_proc         = isp_drv_be_end_int_proc,
    .pfn_isp_register_sync_task          = hi_isp_sync_task_register,
    .pfn_isp_unregister_sync_task        = hi_isp_sync_task_unregister,
    .pfn_isp_int_bottom_half             = isp_int_bottom_half,
    .pfn_isp_isr                         = isp_isr,
    .pfn_isp_get_pub_attr                = isp_get_pub_attr,
    .pfn_isp_drv_get_rgbir_format        = isp_drv_get_rgbir_format,
    .pfn_isp_drv_set_online_statistics_stt_addr  = isp_drv_set_online_statistics_stt_addr,
    .pfn_isp_get_pq_ai_info              = isp_get_pq_info,
};

static umap_module g_isp_module    = {
    .mod_id           = HI_ID_ISP,
    .mod_name         = "isp",

    .pfn_init         = isp_kern_init,
    .pfn_exit         = isp_kern_exit,
    .pfn_ver_checker  = isp_get_ver_magic,
    .export_funcs     = &g_isp_exp_func,
    .data             = HI_NULL,
};

int isp_module_init(void)
{
    hi_u32  vi_pipe;

#ifndef DISABLE_DEBUG_INFO
    osal_proc_entry_t *proc = HI_NULL;
#endif

    g_isp_device = osal_createdev("isp_dev");
    g_isp_device->fops = &g_isp_file_ops;
    g_isp_device->osal_pmops = &g_isp_drv_ops;
    g_isp_device->minor = UMAP_ISP_MINOR_BASE;

    if (osal_registerdevice(g_isp_device) < 0) {
        HI_PRINT("Kernel: Could not register isp devices\n");
        return HI_FAILURE;
    }

#ifndef DISABLE_DEBUG_INFO
    proc = osal_create_proc_entry(PROC_ENTRY_ISP, HI_NULL);
    if (proc == HI_NULL) {
        HI_PRINT("Kernel: Register isp proc failed!\n");
        goto OUT2;
    }

    proc->read = isp_proc_show;
#endif

    if (cmpi_register_module(&g_isp_module)) {
        goto OUT1;
    }

    for (vi_pipe = 0; vi_pipe < ISP_MAX_PIPE_NUM; vi_pipe++) {
        osal_spin_lock_init(&g_isp_lock[vi_pipe]);
        osal_spin_lock_init(&g_isp_sync_lock[vi_pipe]);
    }

    if (isp_drv_init() != 0) {
        HI_PRINT("isp init failed\n");
        goto OUT1;
    }

    HI_PRINT("load isp.ko for %s...OK !\n", CHIP_NAME);
    return HI_SUCCESS;

OUT1:
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, HI_NULL);
#endif
OUT2:
    osal_deregisterdevice(g_isp_device);
    osal_destroydev(g_isp_device);

    HI_PRINT("isp mod init failed!\n");
    return HI_FAILURE;
}

void isp_module_exit(void)
{
    int i;

    isp_drv_exit();

    for (i = 0; i < ISP_MAX_PIPE_NUM; i++) {
        osal_spin_lock_destory(&g_isp_lock[i]);
        osal_spin_lock_destory(&g_isp_sync_lock[i]);
    }

    cmpi_unregister_module(HI_ID_ISP);

#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, HI_NULL);
#endif
    osal_deregisterdevice(g_isp_device);
    osal_destroydev(g_isp_device);

    HI_PRINT("unload isp.ko for %s...OK!\n", CHIP_NAME);
}
