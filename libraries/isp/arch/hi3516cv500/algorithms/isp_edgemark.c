/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_edgemark.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"

typedef struct {
    hi_bool edge_mark_mpi_update_en;
    hi_bool enable;             /* RW; range:[0, 1]; format:1.0;enable/disable edge mark */
    hi_u8   threshold;          /* RW; range: [0, 255];  format:8.0; */
    hi_u32  color;              /* RW; range: [0, 0xFFFFFF];  format:32.0; */
} isp_edgemark_ctx;

isp_edgemark_ctx g_edge_mark_ctx[ISP_MAX_PIPE_NUM] = {{0}};
#define edgemark_get_ctx(dev, ctx)   ctx = &g_edge_mark_ctx[dev]

static hi_void edge_mark_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    isp_edgemark_ctx *edge_mark = HI_NULL;

    edgemark_get_ctx(vi_pipe, edge_mark);

    hi_ext_system_edgemark_mpi_update_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_manual_edgemark_en_write(vi_pipe, edge_mark->enable);
    hi_ext_system_manual_edgemark_color_write(vi_pipe, edge_mark->color);
    hi_ext_system_manual_edgemark_threshold_write(vi_pipe, edge_mark->threshold);
}

static void edge_mark_check_reg(isp_edgemark_reg_cfg *edge_mark_reg)
{
    edge_mark_reg->enable        = MIN2(edge_mark_reg->enable, 0x1);
    edge_mark_reg->mark_edge_sft = MIN2(edge_mark_reg->mark_edge_sft, 12); /* Range[0,12]  */
    edge_mark_reg->u_mark_value  = MIN2(edge_mark_reg->u_mark_value, 255); /* Range[0,255] */
    edge_mark_reg->v_mark_value  = MIN2(edge_mark_reg->v_mark_value, 255); /* Range[0,255] */
}

/* sharpen hardware regs that will change with MPI and ISO */
static hi_void edge_mark_reg_init(hi_vi_pipe vi_pipe, isp_edgemark_reg_cfg *edge_mark_reg)
{
    isp_edgemark_ctx *edge_mark = HI_NULL;

    edgemark_get_ctx(vi_pipe, edge_mark);

    edge_mark_reg->enable        = edge_mark->enable;
    edge_mark_reg->mark_edge_sft = 0;
    edge_mark_reg->mark_edge_thd = edge_mark->threshold;
    edge_mark_reg->u_mark_value  = 120; /* u mark value  120 */
    edge_mark_reg->v_mark_value  = 220; /* u mark value  220 */
    edge_mark_reg->update_index  = 1;
    edge_mark_check_reg(edge_mark_reg);
}

static hi_void edge_mark_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u32 i;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        edge_mark_reg_init(vi_pipe, &reg_cfg->alg_reg_cfg[i].edge_mark_reg_cfg);
    }

    reg_cfg->cfg_key.bit1_edge_mark_cfg = 1;
}

static hi_void edge_mark_read_extregs(hi_vi_pipe vi_pipe)
{
    isp_edgemark_ctx *edge_mark = HI_NULL;

    edgemark_get_ctx(vi_pipe, edge_mark);
    edge_mark->edge_mark_mpi_update_en = hi_ext_system_edgemark_mpi_update_en_read(vi_pipe);

    hi_ext_system_edgemark_mpi_update_en_write(vi_pipe, HI_FALSE);

    if (edge_mark->edge_mark_mpi_update_en) {
        edge_mark->color     = hi_ext_system_manual_edgemark_color_read(vi_pipe);
        edge_mark->threshold = hi_ext_system_manual_edgemark_threshold_read(vi_pipe);
    }
}

static hi_s32 edge_mark_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_edgemark *edge_mark)
{
    isp_check_bool_return(edge_mark->enable);

    if (edge_mark->color > 0xFFFFFF) {
        isp_err_trace("invalid color!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 edge_mark_initialize(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_edgemark_ctx    *edge_mark = HI_NULL;
    hi_isp_cmos_default *sns_dft   = HI_NULL;

    edgemark_get_ctx(vi_pipe, edge_mark);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_edge_mark) {
        isp_check_pointer_return(sns_dft->edge_mark);

        ret = edge_mark_check_cmos_param(vi_pipe, sns_dft->edge_mark);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        edge_mark->enable    = sns_dft->edge_mark->enable;
        edge_mark->threshold = sns_dft->edge_mark->threshold;
        edge_mark->color     = sns_dft->edge_mark->color;
    } else {
        edge_mark->enable    = HI_FALSE;
        edge_mark->threshold = 100;      /* edge mark thd 100 */
        edge_mark->color     = 0xFF0000;
    }

    return HI_SUCCESS;
}

hi_s32 isp_edge_mark_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    hi_s32 ret;

    ret = edge_mark_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    edge_mark_regs_initialize(vi_pipe, (isp_reg_cfg *)reg_cfg_info);
    edge_mark_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

static void edge_mark_rgb2uv(hi_u8 r, hi_u8 g, hi_u8 b,  hi_u8 *u,  hi_u8 *v)
{
    hi_u8 i;
    /* RGB -> BT.709 (i.e. HD) */
    const hi_s16 csc_mat[6] = { /* array size 6 */
        /* R    G   B */
        /* 183, 614, 62,        Y 183  614  62   */
        -101, -338, 439,        /* U -101 -338 439 */
        439, -399, -40          /* V 439  -399 -40 */
    };  /* range[16,235] */

    hi_s16 csc_coef[6];         /* array size 6 */
    hi_s16 tmp;

    for (i = 0; i < 6; i++) {   /* array size 6 */
        csc_coef[i] = csc_mat[i] * 1024 / 1000; /* mat * 1024 / 1000 */
    }

    tmp = signed_right_shift((csc_coef[0] * r + csc_coef[1] * g + csc_coef[2] * b + (1 << 17)), 0xA); /* (0 1 2) 17 */
    *u = (hi_u16)(CLIP3(tmp, 0, 255)); /* Range[0,255] */
    tmp = signed_right_shift((csc_coef[3] * r + csc_coef[4] * g + csc_coef[5] * b + (1 << 17)), 0xA); /* (3 4 5) 17 */
    *v = (hi_u16)(CLIP3(tmp, 0, 255)); /* Range[0,255] */
}

static void edge_mark_mpi2reg(isp_edgemark_reg_cfg *edge_mark_reg_cfg, isp_edgemark_ctx *edge_mark)
{
    hi_u8 r_value, g_value, b_value;

    edge_mark_reg_cfg->enable        = edge_mark->enable;
    edge_mark_reg_cfg->mark_edge_thd = edge_mark->threshold;
    edge_mark_reg_cfg->mark_edge_sft = 0;

    b_value = (hi_u8)((edge_mark->color) & 0xFF);
    g_value = (hi_u8)((edge_mark->color >> 0x8) & 0xFF);
    r_value = (hi_u8)((edge_mark->color >> 0x10) & 0xFF);

    edge_mark_rgb2uv(r_value, g_value, b_value, &(edge_mark_reg_cfg->u_mark_value), &(edge_mark_reg_cfg->v_mark_value));

    edge_mark_check_reg(edge_mark_reg_cfg);
}

static hi_bool __inline check_edge_mark_open(isp_edgemark_ctx *edge_mark)
{
    return (edge_mark->enable == HI_TRUE);
}

hi_s32 edge_mark_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;

    isp_edgemark_ctx *edge_mark = HI_NULL;

    edgemark_get_ctx(vi_pipe, edge_mark);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----EDGEMARK INFO----------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s"    "%16s"        "%16s\n",
                    "bEnable", "Threshold", "Color");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u"  "%16u"      "%16u\n",
                    (hi_u16)edge_mark->enable,
                    (hi_u16)edge_mark->threshold,
                    (hi_u32)edge_mark->color);

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_edge_mark_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_u8 i;
    isp_usr_ctx      *isp_ctx   = HI_NULL;
    isp_edgemark_ctx *edge_mark = HI_NULL;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    isp_get_ctx(vi_pipe, isp_ctx);
    edgemark_get_ctx(vi_pipe, edge_mark);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if ((isp_ctx->frame_cnt % 0x2 != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    edge_mark->enable = hi_ext_system_manual_edgemark_en_read(vi_pipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].edge_mark_reg_cfg.enable = edge_mark->enable;
    }

    reg_cfg->cfg_key.bit1_edge_mark_cfg = 1;

    /* check hardware setting */
    if (!check_edge_mark_open(edge_mark)) {
        return HI_SUCCESS;
    }

    edge_mark_read_extregs(vi_pipe);

    if (edge_mark->edge_mark_mpi_update_en) {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            reg_cfg->alg_reg_cfg[i].edge_mark_reg_cfg.update_index      += 1;
            edge_mark_mpi2reg(&(reg_cfg->alg_reg_cfg[i].edge_mark_reg_cfg), edge_mark);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_edge_mark_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    switch (cmd) {
        case ISP_PROC_WRITE:
            edge_mark_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_edge_mark_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg_attr = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg_attr);

    for (i = 0; i < reg_cfg_attr->reg_cfg.cfg_num; i++) {
        reg_cfg_attr->reg_cfg.alg_reg_cfg[i].edge_mark_reg_cfg.enable = HI_FALSE;
    }

    reg_cfg_attr->reg_cfg.cfg_key.bit1_edge_mark_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_edge_mark(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_edge_mark);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type =  ISP_ALG_EDGEAMRK;
    algs->alg_func.pfn_alg_init = isp_edge_mark_init;
    algs->alg_func.pfn_alg_run  = isp_edge_mark_run;
    algs->alg_func.pfn_alg_ctrl = isp_edge_mark_ctrl;
    algs->alg_func.pfn_alg_exit = isp_edge_mark_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
