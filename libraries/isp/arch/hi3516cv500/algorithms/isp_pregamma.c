/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_pregamma.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_ext_config.h"
#include "hi_math.h"

#define PREGAMMA_BIT_DEP_IN  20
#define PREGAMMA_BIT_DEP_OUT 20

#define PREGAMMA_SEG_NUM HI_ISP_PREGAMMA_SEG_NUM

static const hi_u32 g_pregamma_default[PREGAMMA_NODE_NUM] = {
    0, 256, 512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560, 2816, 3072, 3328, 3584, 3840,
    4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680,
    7936, 8192, 8448, 8704, 8960, 9216, 9472, 9728, 9984, 10240, 10496, 10752, 11008, 11264,
    11520, 11776, 12032, 12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080, 14336, 14592,
    14848, 15104, 15360, 15616, 15872, 16128, 16384, 16896, 17408, 17920, 18432, 18944, 19456,
    19968, 20480, 20992, 21504, 22016, 22528, 23040, 23552, 24064, 24576, 25088, 25600, 26112,
    26624, 27136, 27648, 28160, 28672, 29184, 29696, 30208, 30720, 31232, 31744, 32256, 32768,
    33792, 34816, 35840, 36864, 37888, 38912, 39936, 40960, 41984, 43008, 44032, 45056, 46080,
    47104, 48128, 49152, 50176, 51200, 52224, 53248, 54272, 55296, 56320, 57344, 58368, 59392,
    60416, 61440, 62464, 63488, 64512, 65536, 67584, 69632, 71680, 73728, 75776, 77824, 79872,
    81920, 83968, 86016, 88064, 90112, 92160, 94208, 96256, 98304, 100352, 102400, 104448, 106496,
    108544, 110592, 112640, 114688, 116736, 118784, 120832, 122880, 124928, 126976, 129024, 131072,
    135168, 139264, 143360, 147456, 151552, 155648, 159744, 163840, 167936, 172032, 176128, 180224,
    184320, 188416, 192512, 196608, 200704, 204800, 208896, 212992, 217088, 221184, 225280, 229376,
    233472, 237568, 241664, 245760, 249856, 253952, 258048, 262144, 270336, 278528, 286720, 294912,
    303104, 311296, 319488, 327680, 335872, 344064, 352256, 360448, 368640, 376832, 385024, 393216,
    401408, 409600, 417792, 425984, 434176, 442368, 450560, 458752, 466944, 475136, 483328, 491520,
    499712, 507904, 516096, 524288, 540672, 557056, 573440, 589824, 606208, 622592, 638976, 655360,
    671744, 688128, 704512, 720896, 737280, 753664, 770048, 786432, 802816, 819200, 835584, 851968,
    868352, 884736, 901120, 917504, 933888, 950272, 966656, 983040, 999424, 1015808, 1032192, 1048575
};

static const hi_u8 g_pregamma_seg_idx_base[PREGAMMA_SEG_NUM] = {0, 0, 32, 64, 96, 128, 160, 192};
static const hi_u8 g_pregamma_seg_max_val[PREGAMMA_SEG_NUM]  = {0, 2,  4,  8, 16,  32,  64, 128};

typedef struct {
    hi_bool enable;
    hi_bool lut_update;
} isp_pregamma;

isp_pregamma g_pregamma_ctx[ISP_MAX_PIPE_NUM] = {{0}};
#define pregamma_get_ctx(dev, ctx)   ((ctx) = &g_pregamma_ctx[(dev)])

static hi_s32 pregamma_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_pregamma *pregamma)
{
    hi_u16 i;

    isp_check_bool_return(pregamma->enable);

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        if (pregamma->pregamma_lut[i] > HI_ISP_PREGAMMA_LUT_MAX) {
            isp_err_trace("Invalid pregamma_lut[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_void pregamma_static_regs_init(hi_vi_pipe vi_pipe, hi_u8 blk_idx, isp_reg_cfg *reg_cfg)
{
    isp_pregamma_static_cfg *static_reg_cfg = &(reg_cfg->alg_reg_cfg[blk_idx].pregamma_reg_cfg.static_reg_cfg);
    hi_u32 idx_base_lut_size = PREGAMMA_SEG_NUM * sizeof(hi_u8);
    hi_u32 max_val_lut_size = PREGAMMA_SEG_NUM * sizeof(hi_u8);

    static_reg_cfg->bit_depth_in  = PREGAMMA_BIT_DEP_IN;
    static_reg_cfg->bit_depth_out = PREGAMMA_BIT_DEP_OUT;

    memcpy_s(static_reg_cfg->seg_idx_base, idx_base_lut_size, g_pregamma_seg_idx_base, idx_base_lut_size);
    memcpy_s(static_reg_cfg->seg_max_val, max_val_lut_size, g_pregamma_seg_max_val,  max_val_lut_size);

    static_reg_cfg->static_resh   = HI_TRUE;
}

static hi_void pregamma_dyna_regs_init(hi_vi_pipe vi_pipe, hi_u8 blk_idx, isp_reg_cfg *reg_cfg,
                                       const hi_u32 *pregamma_lut)
{
    hi_s32 ret;
    isp_pregamma_dyna_cfg *dyna_reg_cfg = &(reg_cfg->alg_reg_cfg[blk_idx].pregamma_reg_cfg.dyna_reg_cfg);
    hi_u32 pregamma_lut_size = PREGAMMA_NODE_NUM * sizeof(hi_u32);

    dyna_reg_cfg->pregamma_lut_update_en = HI_TRUE;
    dyna_reg_cfg->update_index = 1;
    dyna_reg_cfg->buf_id = 0;
    ret = memcpy_s(dyna_reg_cfg->pregamma_lut, pregamma_lut_size, pregamma_lut, pregamma_lut_size);
    if (ret != 0) {
        isp_err_trace("ISP[%d] init pregamma lut failed!\n", vi_pipe);
    }
}

static hi_s32 pregamma_regs_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_bool offline_mode;
    hi_u16 i;
    hi_s32 ret;
    const hi_u32         *pregamma_lut  = HI_NULL;
    isp_pregamma_reg_cfg *pregamma_reg_cfg = HI_NULL;
    isp_pregamma         *pregamma_ctx = HI_NULL;
    isp_usr_ctx          *isp_ctx      = HI_NULL;
    hi_isp_cmos_default  *sns_dft      = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_get_ctx(vi_pipe, isp_ctx);
    pregamma_get_ctx(vi_pipe, pregamma_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    pregamma_ctx->lut_update = HI_FALSE;

    /* read from CMOS */
    if (sns_dft->key.bit1_pregamma) {
        isp_check_pointer_return(sns_dft->pregamma);
        ret = pregamma_check_cmos_param(vi_pipe, sns_dft->pregamma);
        if (ret != HI_SUCCESS) {
            return ret;
        }
        pregamma_lut         = sns_dft->pregamma->pregamma_lut;
        pregamma_ctx->enable = sns_dft->pregamma->enable;
    } else {
        pregamma_lut         = g_pregamma_default;
        pregamma_ctx->enable =  HI_FALSE;
    }

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        hi_ext_system_pregamma_lut_write(vi_pipe, i, pregamma_lut[i]);
    }

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        pregamma_reg_cfg = &reg_cfg->alg_reg_cfg[i].pregamma_reg_cfg;
        pregamma_reg_cfg->pregamma_en = offline_mode ? (pregamma_ctx->enable) : (HI_FALSE);
        pregamma_static_regs_init(vi_pipe, i, reg_cfg);
        pregamma_dyna_regs_init(vi_pipe, i, reg_cfg, pregamma_lut);
    }

    reg_cfg->cfg_key.bit1_pregamma_cfg = 1;
    return HI_SUCCESS;
}

static hi_void pregamma_ext_regs_init(hi_vi_pipe vi_pipe)
{
    isp_pregamma *pregamma_ctx = HI_NULL;

    pregamma_get_ctx(vi_pipe, pregamma_ctx);

    hi_ext_system_pregamma_en_write(vi_pipe, pregamma_ctx->enable);
    hi_ext_system_pregamma_lut_update_write(vi_pipe, HI_FALSE);
}

hi_s32 isp_pregamma_init(hi_vi_pipe vi_pipe, hi_void *cfg)
{
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;

    ret = pregamma_regs_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    pregamma_ext_regs_init(vi_pipe);

    return HI_SUCCESS;
}

static hi_s32 isp_pregamma_wdr_mode_set(hi_vi_pipe vi_pipe, hi_void *cfg)
{
    hi_u8  i;
    hi_u32 update_idx[ISP_STRIPING_MAX_NUM] = {0};
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;
    hi_s32 ret;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        update_idx[i] = reg_cfg->alg_reg_cfg[i].pregamma_reg_cfg.dyna_reg_cfg.update_index;
    }

    ret = isp_pregamma_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].pregamma_reg_cfg.dyna_reg_cfg.update_index = update_idx[i] + 1;
        reg_cfg->alg_reg_cfg[i].pregamma_reg_cfg.dyna_reg_cfg.switch_mode  = HI_TRUE;
    }

    return HI_SUCCESS;
}

static hi_void pregamma_read_ext_regs(hi_vi_pipe vi_pipe)
{
    isp_pregamma *pregamma_ctx = HI_NULL;

    pregamma_get_ctx(vi_pipe, pregamma_ctx);

    pregamma_ctx->lut_update = hi_ext_system_pregamma_lut_update_read(vi_pipe);
    hi_ext_system_pregamma_lut_update_write(vi_pipe, HI_FALSE);
}

hi_s32 pregamma_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_pregamma *pregamma = HI_NULL;

    pregamma_get_ctx(vi_pipe, pregamma);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----PreGamma INFO--------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len, "%16s\n", "Enable");

    isp_proc_printf(&proc_tmp, proc->write_len, "%16u\n", pregamma->enable);

    proc->write_len += 1;

    return HI_SUCCESS;
}

static hi_bool __inline check_pregamma_open(isp_pregamma *pregamma)
{
    return (pregamma->enable == HI_TRUE);
}

hi_s32 isp_pregamma_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                        hi_void *cfg, hi_s32 rsv)
{
    hi_u16 i, j;
    isp_pregamma *pregamma_ctx = HI_NULL;
    isp_reg_cfg  *reg  = (isp_reg_cfg *)cfg;
    isp_pregamma_dyna_cfg *dyna_reg_cfg = HI_NULL;

    pregamma_get_ctx(vi_pipe, pregamma_ctx);

    pregamma_ctx->enable = hi_ext_system_pregamma_en_read(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        reg->alg_reg_cfg[i].pregamma_reg_cfg.pregamma_en = pregamma_ctx->enable;
    }

    reg->cfg_key.bit1_pregamma_cfg = 1;

    /* check hardware setting */
    if (!check_pregamma_open(pregamma_ctx)) {
        return HI_SUCCESS;
    }

    pregamma_read_ext_regs(vi_pipe);

    if (pregamma_ctx->lut_update) {
        for (i = 0; i < reg->cfg_num; i++) {
            dyna_reg_cfg = &reg->alg_reg_cfg[i].pregamma_reg_cfg.dyna_reg_cfg;
            for (j = 0; j < PREGAMMA_NODE_NUM; j++) {
                dyna_reg_cfg->pregamma_lut[j] = hi_ext_system_pregamma_lut_read(vi_pipe, j);
            }

            dyna_reg_cfg->pregamma_lut_update_en = HI_TRUE;
            dyna_reg_cfg->update_index      += 1;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_pregamma_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    hi_s32 ret = HI_SUCCESS;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            ret = isp_pregamma_wdr_mode_set(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            ret = pregamma_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }

    return ret;
}

hi_s32 isp_pregamma_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].pregamma_reg_cfg.pregamma_en = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_pregamma_cfg = 1;
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_pre_gamma(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_pre_gamma);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_PREGAMMA;
    algs->alg_func.pfn_alg_init = isp_pregamma_init;
    algs->alg_func.pfn_alg_run  = isp_pregamma_run;
    algs->alg_func.pfn_alg_ctrl = isp_pregamma_ctrl;
    algs->alg_func.pfn_alg_exit = isp_pregamma_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

