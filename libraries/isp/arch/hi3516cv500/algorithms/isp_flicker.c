/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_flicker.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#if 1
#define HI_ISP_FLICKER_GROUP            16
#define HI_ISP_FLICKER_MINBANDNUM       3
#define HI_ISP_FLICKER_MINVALIDBANDPCNT 6
#define HI_ISP_FLICKER_WAVEDIFF1        3
#define HI_ISP_FLICKER_WAVEDIFF2        4
#define HI_ISP_FLICKER_PERIOD           3
#define HI_ISP_FLICKER_GRCNT            5
#define HI_ISP_FLICKER_GBCNT            5
#define HI_ISP_FLICKER_OVERTH           50
#define HI_ISP_FLICKER_OVERCNTTH        520560
#define HI_ISP_FLICKER_MUTIPLE          2
#define HI_ISP_FLICKER_MINWIDTH         2

typedef struct {
    hi_u8   min_band_num;
    hi_u8   min_valid_band_pcnt;
    hi_u8   wave_diff1;
    hi_u8   wave_diff2;
    hi_u8   period;
    hi_u8   gr_cnt;
    hi_u8   gb_cnt;
    hi_s32  gb_avgcnt;
    hi_s32  gr_avgcnt;
} isp_flicker_extern_reg;

typedef struct {
    /* alloc mem when initial */
    hi_s32 *gr_mean;
    hi_s32 *gb_mean;
    hi_s32 *gr_cros0;
    hi_s32 *gb_cros0;
    hi_s32 *gr_cnt;
    hi_s32 *gb_cnt;
} ISP_FLICKER_POINT_ATTR;

typedef struct {
    hi_s32 cur_frame_diff_sum_gr; /* s24.0, current frame gr sum */
    hi_s32 cur_frame_diff_sum_gb; /* s24.0, current frame gb sum */

    hi_u32 cur_frame_abs_sum_gr;  /* u23.0 */
    hi_u32 cur_frame_abs_sum_gb;  /* u23.0 */
    hi_u32 cur_frame_count_over; /* u26.0 */
    hi_s32 *gr_mean;          /* s11.0 */
    hi_s32 *gb_mean;          /* s11.0 */
} ISP_FLICKER_STAT_INFO;

typedef struct {
    hi_bool enable;
    hi_bool malloc;
    hi_u16  fck_inx;
    hi_u16  height;
    hi_u16  over_thr;
    hi_u32  over_cnt_thr;
    hi_u32  frame_num;
    hi_s32  pre_frame_avg_gr;  /* s15.0, previous frame gr avg */
    hi_s32  pre_frame_avg_gb;  /* s15.0, previous frame gb avg */
    hi_s32  cur_frame_avg_gr;  /* s15.0, previous frame gr avg */
    hi_s32  cur_frame_avg_gb;  /* s15.0, previous frame gb avg */
    hi_u32  cur_frame_var_gr;
    hi_u32  cur_frame_var_gb;
    hi_u32  cur_frame_count_over; /* u26.0 */
    hi_s32  cur_flicker;        /* u1.0,  current frame flicker,1 is flicker,0 is no flicker */
    hi_s32  ret_flick_result;    /* u1.0,  the result,1 is flicker,0 is no flicker */
    hi_s32  cur_freq;           /* u1.0,  current frame flicker,1 is 50_hz,0 is 60_hz */
    hi_s32  pre_freq;
    hi_s32  ret_freq_result;
    hi_s32  flicker_hist;
    hi_s32  freq_hist;
    hi_s32 *mem;

    isp_flicker_extern_reg fck_ext_reg;
    ISP_FLICKER_POINT_ATTR   fck_point;
    ISP_FLICKER_STAT_INFO    fck_stat_info;
} ISP_FLICKER_ATTR;

typedef struct {
    hi_s32 grindx;
    hi_s32 gbindx;
    hi_s32 gr_avg;
    hi_s32 gb_avg;
    hi_s32 gr_pat;
    hi_s32 gb_pat;
    hi_s32 grflicker_thd;
    hi_s32 gbflicker_thd;
    hi_s32 gr_diff3cnt;
    hi_s32 gb_diff3cnt;
    hi_s32 gr_avgcnt;
    hi_s32 gb_avgcnt;
} ISP_FLICKER_AVG;

ISP_FLICKER_ATTR *g_flicker_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define flicker_get_ctx(dev, ctx)   ((ctx) = g_flicker_ctx[dev])
#define flicker_set_ctx(dev, ctx)   (g_flicker_ctx[dev] = (ctx))
#define flicker_reset_ctx(dev)      (g_flicker_ctx[dev] = HI_NULL)

hi_s32 flicker_ctx_init(hi_vi_pipe vi_pipe)
{
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;

    flicker_get_ctx(vi_pipe, flicker_ctx);

    if (flicker_ctx == HI_NULL) {
        flicker_ctx = (ISP_FLICKER_ATTR *)isp_malloc(sizeof(ISP_FLICKER_ATTR));
        if (flicker_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] flicker_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }

        memset_s(flicker_ctx, sizeof(ISP_FLICKER_ATTR), 0, sizeof(ISP_FLICKER_ATTR));
    }

    flicker_set_ctx(vi_pipe, flicker_ctx);

    return HI_SUCCESS;
}

hi_void flicker_ctx_exit(hi_vi_pipe vi_pipe)
{
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;

    flicker_get_ctx(vi_pipe, flicker_ctx);
    isp_free(flicker_ctx);
    flicker_reset_ctx(vi_pipe);
}

static hi_void flicker_free_mem(hi_vi_pipe vi_pipe)
{
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;

    flicker_get_ctx(vi_pipe, flicker_ctx);
    isp_check_pointer_void_return(flicker_ctx);

    if ((flicker_ctx->malloc == HI_TRUE) && (flicker_ctx->mem != HI_NULL)) {
        isp_free(flicker_ctx->mem);
        flicker_ctx->fck_stat_info.gr_mean = HI_NULL;
        flicker_ctx->fck_stat_info.gb_mean = HI_NULL;
        flicker_ctx->fck_point.gr_mean    = HI_NULL;
        flicker_ctx->fck_point.gb_mean    = HI_NULL;
        flicker_ctx->fck_point.gr_cros0   = HI_NULL;
        flicker_ctx->fck_point.gb_cros0   = HI_NULL;
        flicker_ctx->fck_point.gr_cnt     = HI_NULL;
        flicker_ctx->fck_point.gb_cnt     = HI_NULL;
        flicker_ctx->malloc                  = HI_FALSE;
    }
}

hi_void set_read_flicker_stat_key(hi_vi_pipe vi_pipe, hi_bool read_en)
{
    hi_u32 isr_access;

    isr_access = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);

    if (read_en == HI_TRUE) {
        isr_access |= (1 << FLICKER_STAT_KEY_BIT);
    } else {
        isr_access &= (~(1 << FLICKER_STAT_KEY_BIT));
    }

    hi_ext_system_statistics_ctrl_highbit_write(vi_pipe, isr_access);
}

static hi_void flicker_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_ext_system_flicker_result_write(vi_pipe, 0);
    hi_ext_system_freq_result_write(vi_pipe, 0x2);
    hi_ext_system_flicker_min_band_num_write(vi_pipe, HI_ISP_FLICKER_MINBANDNUM);
    hi_ext_system_flicker_min_valid_band_pcnt_write(vi_pipe, HI_ISP_FLICKER_MINVALIDBANDPCNT);
    hi_ext_system_flicker_wave_diff1_write(vi_pipe, HI_ISP_FLICKER_WAVEDIFF1);
    hi_ext_system_flicker_wave_diff2_write(vi_pipe, HI_ISP_FLICKER_WAVEDIFF2);
    hi_ext_system_flicker_period_write(vi_pipe, HI_ISP_FLICKER_PERIOD);
    hi_ext_system_flicker_gr_cnt_write(vi_pipe, HI_ISP_FLICKER_GRCNT);
    hi_ext_system_flicker_gb_cnt_write(vi_pipe, HI_ISP_FLICKER_GBCNT);
    hi_ext_system_flicker_over_cnt_thr_write(vi_pipe, HI_ISP_FLICKER_OVERCNTTH);
    hi_ext_system_flicker_over_thr_write(vi_pipe, HI_ISP_FLICKER_OVERTH);
}

static hi_bool get_flick_enable(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_wdr_mode(isp_ctx->sns_wdr_mode) && \
        ((isp_ctx->block_attr.running_mode == ISP_MODE_RUNNING_OFFLINE)  || \
         (isp_ctx->block_attr.running_mode == ISP_MODE_RUNNING_ONLINE))) {
        return HI_TRUE;
    } else {
        return HI_FALSE;
    }
}

static hi_void flicker_dyna_reg_init(hi_u8 wdr_mode, isp_flicker_dyna_cfg *dyna_reg_cfg)
{
    dyna_reg_cfg->gr_avg_pre   = 0;
    dyna_reg_cfg->gb_avg_pre   = 0;
    dyna_reg_cfg->over_thr     = HI_ISP_FLICKER_OVERTH;
    dyna_reg_cfg->resh         = HI_TRUE;
}

static hi_void flicker_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_bool enable;
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    enable = get_flick_enable(vi_pipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        flicker_dyna_reg_init(isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].flicker_reg_cfg.dyna_reg_cfg);
        reg_cfg->alg_reg_cfg[i].flicker_reg_cfg.enable = enable;
    }

    reg_cfg->cfg_key.bit1_flicker_cfg = 1;
}

static hi_void flicker_read_hw_stat_info(hi_vi_pipe vi_pipe, isp_stat *stat_info)
{
    hi_u32 i;
    hi_u32 val, tmp;
    hi_s32 hw_val, hw_tmp;
    hi_u32 sft = 11;
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;

    flicker_get_ctx(vi_pipe, flicker_ctx);

    val = stat_info->flicker_stat.cur_diff_gr[0]; /* diff */
    hw_val = (val & 0x400000) ? (-((hi_s32)(((~val) & 0x3FFFFF) + 1))) : (hi_s32)(val & 0x3FFFFF);
    flicker_ctx->fck_stat_info.cur_frame_diff_sum_gr = hw_val;

    val = stat_info->flicker_stat.cur_diff_gb[0]; /* diff */
    hw_val = (val & 0x400000) ? (-((hi_s32)(((~val) & 0x3FFFFF) + 1))) : (hi_s32)(val & 0x3FFFFF);
    flicker_ctx->fck_stat_info.cur_frame_diff_sum_gb = hw_val;

    flicker_ctx->fck_stat_info.cur_frame_abs_sum_gr = stat_info->flicker_stat.cur_abs_gr[0]; /* ABS */
    flicker_ctx->fck_stat_info.cur_frame_abs_sum_gb = stat_info->flicker_stat.cur_abs_gb[0]; /* ABS */

    flicker_ctx->cur_frame_avg_gr = (HI_ISP_FLICKER_MUTIPLE * flicker_ctx->fck_stat_info.cur_frame_diff_sum_gr +
                                     (hi_s32)(flicker_ctx->height >> 1)) / div_0_to_1((hi_s32)flicker_ctx->height);
    flicker_ctx->cur_frame_avg_gb = (HI_ISP_FLICKER_MUTIPLE * flicker_ctx->fck_stat_info.cur_frame_diff_sum_gb +
                                     (hi_s32)(flicker_ctx->height >> 1)) / div_0_to_1((hi_s32)flicker_ctx->height);

    for (i = 0; i < flicker_ctx->fck_inx; i++) {
        val = stat_info->flicker_stat.g_mean[0][i];
        tmp = (val & 0x3FF800) >> sft;
        hw_tmp = (tmp & 0x400) ? (-((hi_s32)(((~tmp) & 0x3FF) + 1))) : (hi_s32)(tmp & 0x3FF);
        flicker_ctx->fck_stat_info.gr_mean[i] = hw_tmp;

        tmp = val & 0X7FF;
        hw_tmp = (tmp & 0x400) ? (-((hi_s32)(((~tmp) & 0x3FF) + 1))) : (hi_s32)(tmp & 0x3FF);
        flicker_ctx->fck_stat_info.gb_mean[i] = hw_tmp;
    }

    flicker_ctx->cur_frame_var_gr  = (HI_ISP_FLICKER_MUTIPLE * flicker_ctx->fck_stat_info.cur_frame_abs_sum_gr) /
                                     div_0_to_1(flicker_ctx->height);
    flicker_ctx->cur_frame_var_gb  = (HI_ISP_FLICKER_MUTIPLE * flicker_ctx->fck_stat_info.cur_frame_abs_sum_gb) /
                                     div_0_to_1(flicker_ctx->height);
}

static hi_void flicker_result(ISP_FLICKER_ATTR *flicker_ctx, isp_flicker_extern_reg *ext_reg,
                              hi_s32 gb_avgcnt, hi_s32 gr_avgcnt)
{
    /* 50_hz / 60_hz */
    if (flicker_ctx->cur_flicker == 1) {
        if ((gr_avgcnt <= ext_reg->gr_cnt) && (gb_avgcnt <= ext_reg->gb_cnt)) {
            flicker_ctx->cur_freq = 1; /* 50_hz */
        } else {
            flicker_ctx->cur_freq = 0; /* 60_hz */
        }
    } else {
        flicker_ctx->cur_freq     = 2; /* 2 is the state to light off */
    }

    if (flicker_ctx->frame_num == 1) {
        if (flicker_ctx->cur_freq == 1) {
            flicker_ctx->ret_freq_result = 1;
        } else if (flicker_ctx->cur_freq == 0) {
            flicker_ctx->ret_freq_result = 0;
        } else {
            flicker_ctx->ret_freq_result = 2; /* 2 is the state to light off */
        }
        flicker_ctx->freq_hist = 0;
    } else {
        if (flicker_ctx->freq_hist == ext_reg->period) {
            flicker_ctx->ret_freq_result = flicker_ctx->pre_freq;
            flicker_ctx->freq_hist = 0;
        }
        if (flicker_ctx->cur_freq == flicker_ctx->ret_freq_result) {
            flicker_ctx->freq_hist = 0;
        } else {
            flicker_ctx->freq_hist++;
        }
    }
    flicker_ctx->pre_freq = flicker_ctx->cur_freq;
}

static hi_void flicker_detect_result(hi_vi_pipe vi_pipe)
{
    hi_s32 gb_avgcnt;
    hi_s32 gr_avgcnt;
    isp_flicker_extern_reg *ext_reg = HI_NULL;
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;

    flicker_get_ctx(vi_pipe, flicker_ctx);

    ext_reg = &flicker_ctx->fck_ext_reg;

    gb_avgcnt = ext_reg->gb_avgcnt;
    gr_avgcnt = ext_reg->gr_avgcnt;

    flicker_ctx->pre_frame_avg_gr = flicker_ctx->cur_frame_avg_gr;
    flicker_ctx->pre_frame_avg_gb = flicker_ctx->cur_frame_avg_gb;

    /* flicker result */
    if (flicker_ctx->frame_num == 1) {
        if (flicker_ctx->cur_flicker == 1) {
            flicker_ctx->ret_flick_result = 1;
        } else {
            flicker_ctx->ret_flick_result = 0;
        }
        flicker_ctx->flicker_hist = 0;
    } else {
        if (flicker_ctx->flicker_hist == ext_reg->period) {
            flicker_ctx->ret_flick_result = !(flicker_ctx->ret_flick_result);
            flicker_ctx->flicker_hist = 0;
        }
        if (flicker_ctx->cur_flicker == flicker_ctx->ret_flick_result) {
            flicker_ctx->flicker_hist = 0;
        } else {
            flicker_ctx->flicker_hist++;
        }
    }

    flicker_result(flicker_ctx, ext_reg, gb_avgcnt, gr_avgcnt);
}

static hi_void filcker_get_index(hi_s32 *cros0, hi_s32 *s32indx, hi_s32 *hw_cnt, hi_u16 inx)
{
    hi_u32 i, j;
    hi_s32 cnt = 1;
    hi_s32 tmp1, tmp2;

    *s32indx = 0;
    j = 1;

    for (i = 0; i < inx;) {
        tmp1 = *(cros0 + i);

        for (; j < inx;) {
            tmp2 = *(cros0 + j);

            if (tmp1 == tmp2) {
                cnt++;
                j++;
            } else {
                break;
            }
        }

        if (i == inx - 1) {
            *(hw_cnt + *s32indx) = cnt;
            break;
        } else if (j == inx) {
            *(hw_cnt + *s32indx) = cnt;
            i = inx;
        } else {
            *(hw_cnt + *s32indx) = cnt;
            cnt = 1;
            i = j;
            j++;
            (*s32indx)++;
        }
    }
}

static hi_void flicker_avg_count_start(ISP_FLICKER_AVG *flicker_avg, hi_s32 *gr_cnt, hi_s32 *gb_cnt)
{
    hi_u32 i;
    hi_u32 gr_avgcnt;
    hi_u32 gb_avgcnt;

    gr_avgcnt = flicker_avg->gr_avgcnt;
    gb_avgcnt = flicker_avg->gb_avgcnt;
    /* gr avg */
    flicker_avg->gr_avg = 0;
    for (i = 1; i < flicker_avg->grindx; i++) {
        if (*(gr_cnt + i) > HI_ISP_FLICKER_MINWIDTH) {
            flicker_avg->gr_avg += *(gr_cnt + i);
            gr_avgcnt++;
        }
    }
    if (gr_avgcnt) {
        flicker_avg->gr_avg = (flicker_avg->gr_avg + (gr_avgcnt >> 1)) / div_0_to_1(gr_avgcnt);
    } else {
        flicker_avg->gr_avg = -1;
    }
    /* gb avg */
    flicker_avg->gb_avg = 0;
    for (i = 1; i < flicker_avg->gbindx; i++) {
        if (*(gb_cnt + i) > HI_ISP_FLICKER_MINWIDTH) {
            flicker_avg->gb_avg += *(gb_cnt + i);
            flicker_avg->gb_avgcnt++;
        }
    }
    if (flicker_avg->gb_avgcnt) {
        flicker_avg->gb_avg = (flicker_avg->gb_avg + (gb_avgcnt >> 1)) / div_0_to_1(gb_avgcnt);
    } else {
        flicker_avg->gb_avg = -1;
    }
}

static hi_void flicker_avg_count_end(ISP_FLICKER_AVG *flicker_avg, hi_s32 *gr_cnt, hi_s32 *gb_cnt,
                                     isp_flicker_extern_reg *ext_reg)
{
    hi_u32 i;

    flicker_avg->gr_pat = 0;
    for (i = 1; i < flicker_avg->grindx; i++) {
        if (abs(flicker_avg->gr_avg - *(gr_cnt + i)) <= ext_reg->wave_diff1) {
            flicker_avg->gr_pat++;
        } else if (abs(flicker_avg->gr_avg - *(gr_cnt + i)) == ext_reg->wave_diff2) {
            flicker_avg->gr_diff3cnt++;
        }
    }

    flicker_avg->gb_pat = 0;
    for (i = 1; i < flicker_avg->gbindx; i++) {
        if (abs(flicker_avg->gb_avg - *(gb_cnt + i)) <= ext_reg->wave_diff1) {
            flicker_avg->gb_pat++;
        } else if (abs(flicker_avg->gb_avg - *(gb_cnt + i)) == ext_reg->wave_diff2) {
            flicker_avg->gb_diff3cnt++;
        }
    }
    if (flicker_avg->gr_diff3cnt == 1) {
        flicker_avg->gr_pat++;
    }

    if (flicker_avg->gb_diff3cnt == 1) {
        flicker_avg->gb_pat++;
    }

    if (flicker_avg->gr_avg == -1 || flicker_avg->gb_avg == -1) {
        flicker_avg->gr_pat = -1;
        flicker_avg->gb_pat = -1;
    }
}

static hi_void flicker_avg_count(isp_flicker_extern_reg *ext_reg, ISP_FLICKER_ATTR *flicker_ctx,
                                 ISP_FLICKER_AVG *flicker_avg, hi_s32 *gr_cnt, hi_s32 *gb_cnt)
{
    hi_s32 rnd_adder = 5;
    hi_s32 rnd_divisor = 10;

    /* remove first and last wave */
    flicker_avg->gr_diff3cnt = 0;
    flicker_avg->gb_diff3cnt = 0;
    flicker_avg->gr_avgcnt   = 0;
    flicker_avg->gb_avgcnt   = 0;

    if (flicker_avg->grindx > ext_reg->min_band_num && flicker_avg->gbindx > ext_reg->min_band_num) {
        flicker_avg_count_start(flicker_avg, gr_cnt, gb_cnt);
        flicker_avg_count_end(flicker_avg, gr_cnt, gb_cnt, ext_reg);
    } else {
        flicker_avg->gr_pat = -1;
        flicker_avg->gb_pat = -1;
    }
    flicker_avg->grflicker_thd = ((flicker_avg->grindx * ext_reg->min_valid_band_pcnt) + rnd_adder) / rnd_divisor;
    flicker_avg->gbflicker_thd = ((flicker_avg->gbindx * ext_reg->min_valid_band_pcnt) + rnd_adder) / rnd_divisor;
    /* result */
    flicker_ctx->cur_flicker = 0;
    if (flicker_avg->gr_pat >= flicker_avg->grflicker_thd && flicker_avg->gb_pat >= flicker_avg->gbflicker_thd) {
        flicker_ctx->cur_flicker = 1;
    }

    ext_reg->gb_avgcnt = flicker_avg->gb_avgcnt;
    ext_reg->gr_avgcnt = flicker_avg->gr_avgcnt;
}

static hi_void flicker_detect(hi_vi_pipe vi_pipe)
{
    hi_u16 inx;
    hi_u32 i;
    hi_s32 grindx, gbindx;
    hi_s32 *gr_cros0 = HI_NULL;
    hi_s32 *gb_cros0 = HI_NULL;
    hi_s32 *gr_cnt = HI_NULL;
    hi_s32 *gb_cnt = HI_NULL;

    isp_flicker_extern_reg *ext_reg = HI_NULL;
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;
    ISP_FLICKER_AVG flicker_avg;

    flicker_get_ctx(vi_pipe, flicker_ctx);

    ext_reg = &flicker_ctx->fck_ext_reg;

    inx = flicker_ctx->fck_inx;

    gr_cros0 = flicker_ctx->fck_point.gr_cros0;
    gb_cros0 = flicker_ctx->fck_point.gb_cros0;

    gr_cnt   = flicker_ctx->fck_point.gr_cnt;
    gb_cnt   = flicker_ctx->fck_point.gb_cnt;

    /* previous frame-based mean is the zero-value axis.
     * if group mean value is above this zero-value, it is marked as 1;
     * otherwise, it is marked as 0.
     */
    for (i = 0; i < inx; i++) {
        *(gr_cros0 + i) = 0;
        *(gb_cros0 + i) = 0;

        if (flicker_ctx->fck_stat_info.gr_mean[i] >= flicker_ctx->pre_frame_avg_gr) {
            *(gr_cros0 + i) = 1;
        }

        if (flicker_ctx->fck_stat_info.gb_mean[i] >= flicker_ctx->pre_frame_avg_gb) {
            *(gb_cros0 + i) = 1;
        }
    }

    /* gr_cnt, gb_cnt: the width of each wave
     * gr_avg, gb_avg: the average width of wave
     * gr_pat, gb_pat: the total number of valid wave
     * gr/gbflicker_thd: min. number of valid wave to indicate flickering
     * comparing the wave's avg. width with each wave's width,
     * if the difference is less than 2, this wave is valid flicker wave.
     * we allow one time of difference being 3 (from the video we have had.)
     */
    grindx  = 0;
    gbindx  = 0;
    filcker_get_index(gr_cros0, &grindx, gr_cnt, inx);
    filcker_get_index(gb_cros0, &gbindx, gb_cnt, inx);

    flicker_avg.grindx = grindx;

    flicker_avg.gbindx = gbindx;

    flicker_avg_count(ext_reg, flicker_ctx, &flicker_avg, gr_cnt, gb_cnt);
    flicker_detect_result(vi_pipe);
}

static hi_s32 flicker_read_extregs(hi_vi_pipe vi_pipe)
{
    isp_flicker_extern_reg *ext_reg = HI_NULL;
    ISP_FLICKER_ATTR  *flicker_ctx = HI_NULL;

    flicker_get_ctx(vi_pipe, flicker_ctx);

    ext_reg = &flicker_ctx->fck_ext_reg;

    ext_reg->min_band_num        = hi_ext_system_flicker_min_band_num_read(vi_pipe);
    ext_reg->min_valid_band_pcnt = hi_ext_system_flicker_min_valid_band_pcnt_read(vi_pipe);
    ext_reg->wave_diff1          = hi_ext_system_flicker_wave_diff1_read(vi_pipe);
    ext_reg->wave_diff2          = hi_ext_system_flicker_wave_diff2_read(vi_pipe);
    ext_reg->period              = hi_ext_system_flicker_period_read(vi_pipe);
    ext_reg->gr_cnt              = hi_ext_system_flicker_gr_cnt_read(vi_pipe);
    ext_reg->gb_cnt              = hi_ext_system_flicker_gr_cnt_read(vi_pipe);
    flicker_ctx->over_thr        = hi_ext_system_flicker_over_thr_read(vi_pipe);
    flicker_ctx->over_cnt_thr    = hi_ext_system_flicker_over_cnt_thr_read(vi_pipe);
    return HI_SUCCESS;
}

static hi_void flicker_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8  mem_cnt;
    hi_u32 mem_size;
    hi_u16 height;
    hi_u16 inx_sft = 4;
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    flicker_get_ctx(vi_pipe, flicker_ctx);

    height = isp_ctx->block_attr.frame_rect.height;

    flicker_ctx->frame_num = 0;
    flicker_ctx->height    = height;
    flicker_ctx->fck_inx   = height >> inx_sft;

    flicker_ctx->pre_frame_avg_gb = 0;
    flicker_ctx->pre_frame_avg_gr = 0;
    flicker_ctx->enable           = get_flick_enable(vi_pipe);

    flicker_free_mem(vi_pipe);

    mem_size = (height / HI_ISP_FLICKER_GROUP) * sizeof(hi_s32);
    mem_cnt   = sizeof(ISP_FLICKER_POINT_ATTR) / sizeof(hi_s32);

    if (mem_size == 0 || mem_cnt == 0) {
        return;
    }
    flicker_ctx->mem = (hi_s32 *)isp_malloc(mem_size * mem_cnt);

    if (flicker_ctx->mem == HI_NULL) {
        isp_err_trace("ISP[%d]:Alloc Mem for Flicker Failure!\n", vi_pipe);
        flicker_ctx->enable = HI_FALSE;
        flicker_ctx->malloc = HI_FALSE;
        return;
    }

    flicker_ctx->fck_point.gr_mean  = flicker_ctx->mem;
    flicker_ctx->fck_point.gb_mean  = (hi_s32 *)((hi_u8 *)flicker_ctx->fck_point.gr_mean  + mem_size);
    flicker_ctx->fck_point.gr_cros0 = (hi_s32 *)((hi_u8 *)flicker_ctx->fck_point.gb_mean  + mem_size);
    flicker_ctx->fck_point.gb_cros0 = (hi_s32 *)((hi_u8 *)flicker_ctx->fck_point.gr_cros0 + mem_size);
    flicker_ctx->fck_point.gr_cnt   = (hi_s32 *)((hi_u8 *)flicker_ctx->fck_point.gb_cros0 + mem_size);
    flicker_ctx->fck_point.gb_cnt   = (hi_s32 *)((hi_u8 *)flicker_ctx->fck_point.gr_cnt   + mem_size);

    memset_s(flicker_ctx->fck_point.gr_mean, mem_size, 0, mem_size);
    memset_s(flicker_ctx->fck_point.gb_mean, mem_size, 0, mem_size);
    memset_s(flicker_ctx->fck_point.gr_cros0, mem_size, 0, mem_size);
    memset_s(flicker_ctx->fck_point.gb_cros0, mem_size, 0, mem_size);
    memset_s(flicker_ctx->fck_point.gr_cnt,  mem_size, -1, mem_size);
    memset_s(flicker_ctx->fck_point.gb_cnt,  mem_size, -1, mem_size);

    flicker_ctx->fck_stat_info.gr_mean = flicker_ctx->fck_point.gr_mean;
    flicker_ctx->fck_stat_info.gb_mean = flicker_ctx->fck_point.gb_mean;

    flicker_ctx->malloc = HI_TRUE;
}

hi_s32 isp_flicker_init(hi_vi_pipe vi_pipe, hi_void *cfg)
{
    hi_s32 ret;
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;
    isp_reg_cfg    *reg_cfg     = (isp_reg_cfg *)cfg;

    ret = flicker_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    flicker_get_ctx(vi_pipe, flicker_ctx);
    isp_check_pointer_return(flicker_ctx);

    flicker_ext_regs_initialize(vi_pipe);
    flicker_regs_initialize(vi_pipe, reg_cfg);
    flicker_initialize(vi_pipe);

    set_read_flicker_stat_key(vi_pipe, flicker_ctx->enable);

    return HI_SUCCESS;
}

hi_s32 isp_flicker_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *cfg, hi_s32 rsv)
{
    hi_s32 ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    ISP_FLICKER_ATTR *flicker_ctx = HI_NULL;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)cfg;
    isp_flicker_dyna_cfg *dyna_reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    flicker_get_ctx(vi_pipe, flicker_ctx);
    isp_check_pointer_return(flicker_ctx);

    flicker_ctx->frame_num = isp_ctx->frame_cnt;

    if (!flicker_ctx->enable) {
        return HI_SUCCESS;
    }

    if (isp_ctx->linkage.stat_ready  == HI_FALSE) {
        return HI_SUCCESS;
    }

    isp_check_pointer_return(flicker_ctx->mem);
    ret = flicker_read_extregs(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    flicker_read_hw_stat_info(vi_pipe, (isp_stat *)stat_info);

    if (flicker_ctx->frame_num) {
        flicker_detect(vi_pipe);
    }

    isp_ctx->linkage.flicker     = (hi_bool)flicker_ctx->ret_flick_result;
    isp_ctx->linkage.freq_result = (hi_u8)  flicker_ctx->ret_freq_result;

    hi_ext_system_flicker_result_write(vi_pipe, flicker_ctx->ret_flick_result);
    hi_ext_system_freq_result_write(vi_pipe, flicker_ctx->ret_freq_result);

    dyna_reg_cfg = &reg_cfg->alg_reg_cfg[0].flicker_reg_cfg.dyna_reg_cfg;

    dyna_reg_cfg->over_thr     = flicker_ctx->over_thr;
    dyna_reg_cfg->gr_avg_pre   = flicker_ctx->pre_frame_avg_gr;
    dyna_reg_cfg->gb_avg_pre   = flicker_ctx->pre_frame_avg_gb;

    reg_cfg->cfg_key.bit1_flicker_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_flicker_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *reg_cfg   = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_flicker_init(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_flicker_init(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_flicker_exit(hi_vi_pipe vi_pipe)
{
    flicker_free_mem(vi_pipe);
    flicker_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_flicker(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_flicker);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_FLICKER;
    algs->alg_func.pfn_alg_init = isp_flicker_init;
    algs->alg_func.pfn_alg_run  = isp_flicker_run;
    algs->alg_func.pfn_alg_ctrl = isp_flicker_ctrl;
    algs->alg_func.pfn_alg_exit = isp_flicker_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
#endif
