/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_ca.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_proc.h"

#define HI_ISP_CA_CSC_DC_LEN        3
#define HI_ISP_CA_CSC_COEF_LEN      9
#define HI_ISP_CA_CSC_TYPE_DEFAULT  0
#define HI_ISP_CA_RATIO_MAX         2047
#define HI_ISP_CA_CSC_FIXED_POINT   1024
#define HI_ISP_CA_CSC_DEC           1000
#define HI_ISP_CA_LUMA_THR_HIGH     2
#define HI_ISP_CA_ISO_RATIO_DEFAULT 1024
#define HI_ISP_CA_FRAME_CNT         2

static const  hi_u16 g_y_ratio_lut[HI_ISP_CA_YRATIO_LUT_LENGTH] = {
    36, 81, 111, 136, 158, 182, 207, 228, 259, 290, 317, 345, 369, 396, 420, 444,
    468, 492, 515, 534, 556, 574, 597, 614, 632, 648, 666, 681, 697, 709, 723, 734,
    748, 758, 771, 780, 788, 800, 808, 815, 822, 829, 837, 841, 848, 854, 858, 864,
    868, 871, 878, 881, 885, 890, 893, 897, 900, 903, 906, 909, 912, 915, 918, 921,
    924, 926, 929, 931, 934, 936, 938, 941, 943, 945, 947, 949, 951, 952, 954, 956,
    958, 961, 962, 964, 966, 968, 969, 970, 971, 973, 974, 976, 977, 979, 980, 981,
    983, 984, 985, 986, 988, 989, 990, 991, 992, 993, 995, 996, 997, 998, 999, 1000,
    1001, 1004, 1005, 1006, 1007, 1009, 1010, 1011, 1012, 1014, 1016, 1017, 1019, 1020, 1022, 1024
};
static const  hi_s16 g_iso_ratio[ISP_AUTO_ISO_STRENGTH_NUM] = {
    1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024
};

typedef enum {
    CA_CS_BT_709 = 0,
    CA_CS_BT_601 = 1,
    CA_CS_BUTT
} hi_isp_ca_cs;

typedef struct {
    hi_s32 csc_idc[HI_ISP_CA_CSC_DC_LEN];
    hi_s32 csc_odc[HI_ISP_CA_CSC_DC_LEN];
    hi_s32 csc_coef[HI_ISP_CA_CSC_COEF_LEN];
} hi_isp_ca_csc_table;

static hi_isp_ca_csc_table  g_csc_table_hdycbcr_to_rgb = {
    {0,    -512, -512},
    {0,    0,    0},
    {1000, 0,    1575, 1000, -187, -468, 1000, 1856, 0},
}; /* range[0,255]  X1000 */

static hi_isp_ca_csc_table  g_csc_table_sdycbcr_to_rgb = {
    {0,    -512, -512},
    {0,    0,    0},
    {1000, 0,    1402, 1000, -344, -714, 1000, 1772, 0},
}; /* range[0,255]  X1000 */

typedef struct {
    hi_bool ca_en;      /* u1.0 */
    hi_bool ca_coef_update_en;
    hi_u16 luma_thd_high;
    hi_s16 saturation_ratio;
    hi_u16 y_ratio_lut[HI_ISP_CA_YRATIO_LUT_LENGTH];
    hi_s16 ca_iso_ratio[ISP_AUTO_ISO_STRENGTH_NUM]; /* 16 */
    hi_s16 actual_iso_ratio;
} isp_ca;

isp_ca *g_ca_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define ca_get_ctx(dev, ctx)   ctx = g_ca_ctx[dev]
#define ca_set_ctx(dev, ctx)   g_ca_ctx[dev] = ctx
#define ca_reset_ctx(dev)      g_ca_ctx[dev] = HI_NULL

hi_s32 ca_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_ca *ca_ctx = HI_NULL;

    ca_get_ctx(vi_pipe, ca_ctx);

    if (ca_ctx == HI_NULL) {
        ca_ctx = (isp_ca *)isp_malloc(sizeof(isp_ca));
        if (ca_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] ca_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(ca_ctx, sizeof(isp_ca), 0, sizeof(isp_ca));

    ca_set_ctx(vi_pipe, ca_ctx);

    return HI_SUCCESS;
}

hi_void ca_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_ca *ca_ctx = HI_NULL;

    ca_get_ctx(vi_pipe, ca_ctx);
    isp_free(ca_ctx);
    ca_reset_ctx(vi_pipe);
}

static hi_void ca_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    isp_ca *ca  = HI_NULL;

    ca_get_ctx(vi_pipe, ca);

    hi_ext_system_ca_en_write(vi_pipe, ca->ca_en);
    hi_ext_system_ca_luma_thd_high_write(vi_pipe, HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT);
    hi_ext_system_ca_saturation_ratio_write(vi_pipe, HI_ISP_EXT_CA_SATURATION_RATIO_DEFAULT);
    hi_ext_system_ca_coef_update_en_write(vi_pipe, HI_TRUE);

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        hi_ext_system_ca_y_ratio_lut_write(vi_pipe, i, ca->y_ratio_lut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_ca_iso_ratio_lut_write(vi_pipe, i, ca->ca_iso_ratio[i]);
    }
}

static hi_void get_csc_table(hi_isp_ca_cs csc, hi_isp_ca_csc_table **yuv2_rgb)
{
    switch (csc) {
        case CA_CS_BT_709:
            *yuv2_rgb = &g_csc_table_hdycbcr_to_rgb;
            break;
        case CA_CS_BT_601:
            *yuv2_rgb = &g_csc_table_sdycbcr_to_rgb;
            break;
        default:
            *yuv2_rgb = HI_NULL;
            break;
    }
}

static hi_void set_color_space_convert_params_def(isp_ca_static_cfg *static_reg_cfg, hi_isp_ca_cs csc)
{
    hi_u8 i;
    hi_isp_ca_csc_table *yuv2_rgb = HI_NULL;

    get_csc_table(csc, &yuv2_rgb);

    if (yuv2_rgb == HI_NULL) {
        isp_err_trace("Unable to handle null point in ca moudle!\n");
        return;
    }

    for (i = 0; i < HI_ISP_CA_CSC_COEF_LEN; i++) {
        static_reg_cfg->ca_yuv2_rgb_coef[i] = yuv2_rgb->csc_coef[i] * HI_ISP_CA_CSC_FIXED_POINT / HI_ISP_CA_CSC_DEC;
    }

    for (i = 0; i < HI_ISP_CA_CSC_DC_LEN; i++) {
        static_reg_cfg->ca_yuv2_rgb_in_dc[i]  = yuv2_rgb->csc_idc[i];
        static_reg_cfg->ca_yuv2_rgb_out_dc[i] = yuv2_rgb->csc_odc[i];
    }
}

static hi_void ca_static_regs_initialize(hi_u8 i, isp_ca_static_cfg *static_reg_cfg)
{
    static_reg_cfg->ca_llhc_proc_en  = HI_TRUE;
    static_reg_cfg->ca_skin_proc_en  = HI_TRUE;
    static_reg_cfg->ca_satu_adj_en   = HI_TRUE;

    static_reg_cfg->ca_luma_thr_low         = HI_ISP_CA_LUMA_THD_LOW_DEFAULT;
    static_reg_cfg->ca_dark_chroma_thr_low   = HI_ISP_CA_DARKCHROMA_THD_LOW_DEFAULT;
    static_reg_cfg->ca_dark_chroma_thr_high  = HI_ISP_CA_DARKCHROMA_THD_HIGH_DEFAULT;
    static_reg_cfg->ca_s_dark_chroma_thr_low  = HI_ISP_CA_SDARKCHROMA_THD_LOW_DEFAULT;
    static_reg_cfg->ca_s_dark_chroma_thr_high = HI_ISP_CA_SDARKCHROMA_THD_HIGH_DEFAULT;
    static_reg_cfg->ca_luma_ratio_low       = HI_ISP_CA_LUMA_RATIO_LOW_DEFAULT;

    static_reg_cfg->ca_skin_low_luma_min_u    = HI_ISP_CA_SKINLOWLUAM_UMIN_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_max_u    = HI_ISP_CA_SKINLOWLUAM_UMAX_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_min_uy   = HI_ISP_CA_SKINLOWLUAM_UYMIN_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_max_uy   = HI_ISP_CA_SKINLOWLUAM_UYMAX_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_min_u   = HI_ISP_CA_SKINHIGHLUAM_UMIN_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_max_u   = HI_ISP_CA_SKINHIGHLUAM_UMAX_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_min_uy  = HI_ISP_CA_SKINHIGHLUAM_UYMIN_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_max_uy  = HI_ISP_CA_SKINHIGHLUAM_UYMAX_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_min_v    = HI_ISP_CA_SKINLOWLUAM_VMIN_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_max_v    = HI_ISP_CA_SKINLOWLUAM_VMAX_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_min_vy   = HI_ISP_CA_SKINLOWLUAM_VYMIN_DEFAULT;
    static_reg_cfg->ca_skin_low_luma_max_vy   = HI_ISP_CA_SKINLOWLUAM_VYMAX_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_min_v   = HI_ISP_CA_SKINHIGHLUAM_VMIN_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_max_v   = HI_ISP_CA_SKINHIGHLUAM_VMAX_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_min_vy  = HI_ISP_CA_SKINHIGHLUAM_VYMIN_DEFAULT;
    static_reg_cfg->ca_skin_high_luma_max_vy  = HI_ISP_CA_SKINHIGHLUAM_VYMAX_DEFAULT;
    static_reg_cfg->ca_skin_uv_diff         = HI_ISP_CA_SKINUVDIFF_DEFAULT;
    static_reg_cfg->ca_skin_ratio_thr_low    = HI_ISP_CA_SKINRATIOTHD_LOW_DEFAULT;
    static_reg_cfg->ca_skin_ratio_thr_mid    = HI_ISP_CA_SKINRATIOTHD_MID_DEFAULT;
    static_reg_cfg->ca_skin_ratio_thr_high   = HI_ISP_CA_SKINRATIOTHD_HIGH_DEFAULT;

    set_color_space_convert_params_def(static_reg_cfg, HI_ISP_CA_CSC_TYPE_DEFAULT);

    static_reg_cfg->static_resh = HI_TRUE;
}

static hi_void ca_usr_regs_initialize(isp_ca_usr_cfg *usr_reg_cfg, isp_ca  *ca)
{
    hi_u16 index;

    usr_reg_cfg->ca_luma_thr_high  = HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT;
    index = (usr_reg_cfg->ca_luma_thr_high >> HI_ISP_CA_LUMA_THR_HIGH);
    index = (index >= HI_ISP_CA_YRATIO_LUT_LENGTH) ? (HI_ISP_CA_YRATIO_LUT_LENGTH - 1) : index;
    usr_reg_cfg->ca_luma_ratio_high = ca->y_ratio_lut[index];

    memcpy_s(usr_reg_cfg->y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(hi_u16),
             ca->y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(hi_u16));

    usr_reg_cfg->ca_lut_update_en = HI_TRUE;
    usr_reg_cfg->resh          = HI_TRUE;
    usr_reg_cfg->update_index = 1;
    usr_reg_cfg->buf_id        = 0;
}

static hi_void ca_dyna_regs_initialize(isp_ca_dyna_cfg *dyna_reg_cfg)
{
    dyna_reg_cfg->ca_iso_ratio  = HI_ISP_CA_ISO_RATIO_DEFAULT;
    dyna_reg_cfg->resh          = HI_TRUE;

    return;
}

static hi_void ca_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_bool offline_mode;
    hi_u8 i;
    isp_ca *ca = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    ca_get_ctx(vi_pipe, ca);
    isp_get_ctx(vi_pipe, isp_ctx);
    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].ca_reg_cfg.ca_en       = offline_mode ? (ca->ca_en) : (HI_FALSE);
        reg_cfg->alg_reg_cfg[i].ca_reg_cfg.lut2_stt_en = HI_TRUE;

        ca_static_regs_initialize(i, &reg_cfg->alg_reg_cfg[i].ca_reg_cfg.static_reg_cfg);
        ca_usr_regs_initialize(&reg_cfg->alg_reg_cfg[i].ca_reg_cfg.usr_reg_cfg, ca);
        ca_dyna_regs_initialize(&reg_cfg->alg_reg_cfg[i].ca_reg_cfg.dyna_reg_cfg);
    }

    reg_cfg->cfg_key.bit1_ca_cfg = 1;
}

static hi_s32 ca_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_ca *cmos_ca)
{
    hi_u16 i;

    isp_check_bool_return(cmos_ca->enable);

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        if (cmos_ca->y_ratio_lut[i] > HI_ISP_CA_RATIO_MAX) {
            isp_err_trace("Invalid y_ratio_lut[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_ca->iso_ratio[i] > HI_ISP_CA_RATIO_MAX) {
            isp_err_trace("Invalid iso_ratio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 ca_in_initialize(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_ca *ca = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    ca_get_ctx(vi_pipe, ca);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_ca) {
        isp_check_pointer_return(sns_dft->ca);

        ret = ca_check_cmos_param(vi_pipe, sns_dft->ca);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        ca->ca_en  = sns_dft->ca->enable;
        memcpy_s(ca->y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(hi_u16),
                 sns_dft->ca->y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(hi_u16));
        memcpy_s(ca->ca_iso_ratio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_s16),
                 sns_dft->ca->iso_ratio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_s16));
    } else {
        ca->ca_en  = HI_TRUE;
        memcpy_s(ca->y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(hi_u16),
                 g_y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(hi_u16));
        memcpy_s(ca->ca_iso_ratio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_s16),
                 g_iso_ratio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_s16));
    }

    return HI_SUCCESS;
}

static hi_s32 ca_read_extregs(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    isp_ca *ca = HI_NULL;

    ca_get_ctx(vi_pipe, ca);

    ca->ca_coef_update_en = hi_ext_system_ca_coef_update_en_read(vi_pipe);
    hi_ext_system_ca_coef_update_en_write(vi_pipe, HI_FALSE);

    if (ca->ca_coef_update_en) {
        for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
            ca->y_ratio_lut[i] = hi_ext_system_ca_y_ratio_lut_read(vi_pipe, i);
        }

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            ca->ca_iso_ratio[i] = hi_ext_system_ca_iso_ratio_lut_read(vi_pipe, i);
        }

        ca->luma_thd_high     = hi_ext_system_ca_luma_thd_high_read(vi_pipe);
        ca->saturation_ratio = (hi_s16)hi_ext_system_ca_saturation_ratio_read(vi_pipe);
    }

    return HI_SUCCESS;
}

static hi_s32 ca_get_value_from_lut(hi_s32 x, const hi_u32 *lut_x, hi_s16 *lut_y, hi_s32 length)
{
    hi_s32 n;

    if (x <= lut_x[0]) {
        return lut_y[0];
    }

    for (n = 1; n < length; n++) {
        if (x <= lut_x[n]) {
            return (lut_y[n - 1] + (hi_s64)(lut_y[n] - lut_y[n - 1])
                    * (hi_s64)(x - (hi_s32)lut_x[n - 1]) / div_0_to_1((hi_s32)lut_x[n] - (hi_s32)lut_x[n - 1]));
        }
    }

    return lut_y[length - 1];
}

static hi_bool __inline check_ca_open(isp_ca *ca)
{
    return (ca->ca_en == HI_TRUE);
}

static hi_void isp_ca_usr_fw(isp_ca *ca, isp_ca_usr_cfg *usr_reg_cfg)
{
    hi_u16 j, index;

    for (j = 0; j < HI_ISP_CA_YRATIO_LUT_LENGTH; j++) {
        usr_reg_cfg->y_ratio_lut[j] = MIN2((hi_s32)ca->y_ratio_lut[j]
                                           * ca->saturation_ratio / HI_ISP_CA_CSC_DEC, HI_ISP_CA_RATIO_MAX);
    }

    index = (ca->luma_thd_high >> HI_ISP_CA_LUMA_THR_HIGH);
    index = (index >= HI_ISP_CA_YRATIO_LUT_LENGTH) ? (HI_ISP_CA_YRATIO_LUT_LENGTH - 1) : index;

    usr_reg_cfg->ca_luma_thr_high   = ca->luma_thd_high;
    usr_reg_cfg->ca_luma_ratio_high = ca->y_ratio_lut[index];

    usr_reg_cfg->ca_lut_update_en   = HI_TRUE;

    usr_reg_cfg->resh               = HI_TRUE;
    usr_reg_cfg->update_index += 1;

    return;
}

static hi_void isp_ca_dyna_fw(hi_s32 iso, isp_ca_dyna_cfg *dyna_reg_cfg, isp_ca *ca)
{
    hi_s32 iso_ratio;

    iso_ratio = ca_get_value_from_lut(iso, get_iso_lut(), ca->ca_iso_ratio, ISP_AUTO_ISO_STRENGTH_NUM);

    dyna_reg_cfg->ca_iso_ratio = CLIP3(iso_ratio, 0, HI_ISP_CA_RATIO_MAX);
    dyna_reg_cfg->resh         = HI_TRUE;
    ca->actual_iso_ratio       = dyna_reg_cfg->ca_iso_ratio;
}

static hi_s32 isp_ca_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    ret = ca_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = ca_in_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ca_regs_initialize(vi_pipe, reg_cfg);
    ca_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

static hi_s32 isp_ca_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                         hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8  i;
    isp_ca  *ca = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg *reg = (isp_reg_cfg *)reg_cfg;
    hi_s32 ret;

    isp_get_ctx(vi_pipe, isp_ctx);
    ca_get_ctx(vi_pipe, ca);
    isp_check_pointer_return(ca);

    /* calculate every two interrupts */
    if ((isp_ctx->frame_cnt % HI_ISP_CA_FRAME_CNT != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    ca->ca_en = hi_ext_system_ca_en_read(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        reg->alg_reg_cfg[i].ca_reg_cfg.ca_en = ca->ca_en;
    }

    reg->cfg_key.bit1_ca_cfg = 1;

    /* check hardware setting */
    if (!check_ca_open(ca)) {
        return HI_SUCCESS;
    }

    ret = ca_read_extregs(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    if (ca->ca_coef_update_en) {
        for (i = 0; i < reg->cfg_num; i++) {
            isp_ca_usr_fw(ca, &reg->alg_reg_cfg[i].ca_reg_cfg.usr_reg_cfg);
        }
    }

    for (i = 0; i < reg->cfg_num; i++) {
        isp_ca_dyna_fw((hi_s32)isp_ctx->linkage.iso, &reg->alg_reg_cfg[i].ca_reg_cfg.dyna_reg_cfg, ca);
    }

    return HI_SUCCESS;
}

static hi_void ca_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_ca  *ca_ctx = HI_NULL;

    ca_get_ctx(vi_pipe, ca_ctx);
    isp_check_pointer_void_return(ca_ctx);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----CA INFO-----------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12s" "%12s\n", "Enable", "IsoRatio");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12u" "%12u\n", ca_ctx->ca_en, ca_ctx->actual_iso_ratio);

    proc->write_len += 1;
}

static hi_s32 isp_ca_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    switch (cmd) {
        case ISP_PROC_WRITE:
            ca_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_ca_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].ca_reg_cfg.ca_en = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_ca_cfg = 1;

    ca_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_ca(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_ca);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_CA;
    algs->alg_func.pfn_alg_init = isp_ca_init;
    algs->alg_func.pfn_alg_run  = isp_ca_run;
    algs->alg_func.pfn_alg_ctrl = isp_ca_ctrl;
    algs->alg_func.pfn_alg_exit = isp_ca_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

