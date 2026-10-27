/*
 * imx662_cmos.c — IMX662 ISP-facing logic + registration for Hi3516CV610.
 *
 * SCAFFOLD (original). Implements the public HiSilicon sensor-driver contract
 * (ot_isp_sns_obj + pfn_cmos_*) with IMX662's Sony SHR0/VMAX exposure model and
 * 0.3 dB/LSB gain. Fields that drive image quality (AE routes, ISP defaults,
 * CCM/AWB/gamma) are seeded from public data where possible and marked TODO
 * otherwise — those are the on-device tuning phase, not the scaffold.
 *
 * No vendor (Shenshu/HiSilicon) source is copied; this matches the SDK
 * interface only. Compile in-tree against the hi3516cv6xx ISP headers.
 */

#include <math.h>
#include <stdio.h>
#include "securec.h"
/* The 3A types (ot_isp_ae_sensor_*, ot_isp_awb_sensor_*) and the registration
 * entry points live in these MPI headers, not in the ISP headers the sensor's
 * own headers pull in. Same include set the in-tree cv6xx drivers use --
 * cf. libraries/sensor/hi3516cv6xx/smart_sc450ai/sc450ai_cmos.c. */
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "imx662_cmos.h"
#include "imx662_cmos_param.h"

#ifndef high_8bits
#define high_8bits(x)    (((x) >> 8) & 0xff)
#endif
#ifndef low_8bits
#define low_8bits(x)     ((x) & 0xff)
#endif
#ifndef higher_4bits
#define higher_4bits(x)  (((x) >> 16) & 0x0f)
#endif

/* ---- per-pipe context ----------------------------------------------------- */
static ot_isp_sns_state   g_imx662_state[OT_ISP_MAX_PIPE_NUM];
/* Static zero initialization selects I2C bus 0 for every pipe. */
static ot_isp_sns_commbus g_imx662_bus_info[OT_ISP_MAX_PIPE_NUM];
static td_u32 g_init_exposure[OT_ISP_MAX_PIPE_NUM] = {0};

ot_isp_sns_state *imx662_get_ctx(ot_vi_pipe vi_pipe)
{
	return &g_imx662_state[vi_pipe];
}
ot_isp_sns_commbus *imx662_get_bus_info(ot_vi_pipe vi_pipe)
{
	return &g_imx662_bus_info[vi_pipe];
}

/* ---- mode table ----------------------------------------------------------- */
static const imx662_video_mode_tbl g_imx662_mode_tbl[IMX662_MODE_BUTT] = {
	{ IMX662_VMAX_1080P30_LINEAR, IMX662_FULL_LINES_MAX, 30.0f, 5.0f,
	  1920, 1080, 0, OT_WDR_MODE_NONE, "IMX662_2M_30FPS_12BIT_LINEAR" },
	{ IMX662_VMAX_1080P30_LINEAR, IMX662_FULL_LINES_MAX, 60.0f, 5.0f,
	  1920, 1080, 1, OT_WDR_MODE_NONE, "IMX662_2M_60FPS_12BIT_LINEAR" },
	{ IMX662_VMAX_1080P30_LINEAR, IMX662_FULL_LINES_MAX, 90.0f, 5.0f,
	  1920, 1080, 2, OT_WDR_MODE_NONE, "IMX662_2M_90FPS_10BIT_LINEAR" },
	{ IMX662_VMAX_1080P30_LINEAR, IMX662_FULL_LINES_MAX, 100.0f, 5.0f,
	  1920, 1080, 3, OT_WDR_MODE_NONE, "IMX662_2M_100FPS_10BIT_LINEAR" },
};

/* ==========================================================================
 *  AE (exposure / gain)
 * ========================================================================== */
static td_void cmos_get_ae_linear_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft,
										   const ot_isp_sns_state *sns_state)
{
	ae_sns_dft->max_again        = IMX662_AGAIN_MAX;
	ae_sns_dft->min_again        = IMX662_AGAIN_MIN;
	ae_sns_dft->max_again_target = ae_sns_dft->max_again;
	ae_sns_dft->min_again_target = ae_sns_dft->min_again;

	/* IMX662 has no separate digital-gain register; keep dgain unity. */
	ae_sns_dft->max_dgain        = 1024;
	ae_sns_dft->min_dgain        = 1024;
	ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
	ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

	ae_sns_dft->ae_compensation  = 0x38;
	ae_sns_dft->init_exposure    = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 40000;

	/* SHR model: int_time in lines, max = frame_length - MIN_SHR0. */
	ae_sns_dft->max_int_time        = sns_state->fl_std - IMX662_MIN_SHR0_LINEAR;
	ae_sns_dft->min_int_time        = 1;
	ae_sns_dft->max_int_time_target = 65535;
	ae_sns_dft->min_int_time_target = 1;
	/* TODO(tuning): ae_route_attr / highlight-prior config. */
}

static td_s32 cmos_get_ae_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_check_pointer_return(ae_sns_dft);
	sns_check_pointer_return(sns_state);

	ae_sns_dft->fps            = g_imx662_mode_tbl[sns_state->img_mode].max_fps;
	ae_sns_dft->full_lines_std = sns_state->fl_std;
	ae_sns_dft->full_lines_max = g_imx662_mode_tbl[sns_state->img_mode].max_ver_lines;
	ae_sns_dft->flicker_freq   = 0;
	ae_sns_dft->hmax_times     = (td_u32)(1000000000.0f /
		(sns_state->fl_std * ae_sns_dft->fps));
	ae_sns_dft->lines_per500ms = (td_u32)(sns_state->fl_std * ae_sns_dft->fps / 2);
	ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
	ae_sns_dft->int_time_accu.accuracy  = 1;
	ae_sns_dft->int_time_accu.offset    = 0;
	ae_sns_dft->again_accu.accu_type    = OT_ISP_AE_ACCURACY_TABLE;
	ae_sns_dft->again_accu.accuracy     = 1;
	/* The AE core passes the table code returned by cmos_dgain_calc_table()
	 * to cmos_gains_update(), just as it does for analog gain.  Declaring
	 * this LINEAR makes the core scale the unity value (1024) a second time,
	 * which reports a fictitious 1024x digital gain and drives ISO/NR to the
	 * final bucket. */
	ae_sns_dft->dgain_accu.accu_type    = OT_ISP_AE_ACCURACY_TABLE;
	ae_sns_dft->dgain_accu.accuracy     = 1;
	/* ISP digital gain is fixed-point with 8 fractional bits. Leaving these
	 * zero makes the AE library emit 1, below the ISP's legal minimum 256. */
	ae_sns_dft->isp_dgain_shift          = 8;
	ae_sns_dft->min_isp_dgain_target     = 1u << ae_sns_dft->isp_dgain_shift;
	/* ISP digital gain is the worst gain in the chain: it amplifies the noise
	 * and the quantisation equally and buys no SNR, where analog gain and HCG
	 * at least act before the ADC.  32x let the total ceiling reach 398 * 32
	 * = 12739x (~82 dB), and on the .181 bench a dark room at that ceiling was
	 * judged "super grainy" while 1-2 steps down looked better.  4x puts the
	 * ceiling at 398 * 4 = 1592x (~64 dB), inside the range that was preferred,
	 * and makes AE exhaust analog before it reaches for digital.
	 *
	 * This is an operator trade -- darker but cleaner. */
	ae_sns_dft->max_isp_dgain_target     = 4u << ae_sns_dft->isp_dgain_shift;

	switch (sns_state->wdr_mode) {
		case OT_WDR_MODE_NONE:
		default:
			cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
			break;
		/* TODO(HDR): OT_WDR_MODE_2To1_LINE Clear-HDR variant. */
	}
	return TD_SUCCESS;
}

/* VMAX lives at fast-update registers i2c_data[5..7]. */
static td_void cmos_config_vmax(ot_isp_sns_state *sns_state, td_u32 vmax)
{
	sns_state->regs_info[0].i2c_data[5].data = low_8bits(vmax);
	sns_state->regs_info[0].i2c_data[6].data = high_8bits(vmax);
	sns_state->regs_info[0].i2c_data[7].data = higher_4bits(vmax);
}

static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	td_float max_fps = g_imx662_mode_tbl[sns_state->img_mode].max_fps;
	td_u32   vmax;

	sns_check_pointer_void_return(ae_sns_dft);
	if (fps > max_fps || fps < g_imx662_mode_tbl[sns_state->img_mode].min_fps) {
		isp_err_trace("Unsupported IMX662 fps: %f\n", fps);
		return;
	}
	vmax = (td_u32)((td_float)IMX662_VMAX_1080P30_LINEAR * max_fps / fps);
	vmax = (vmax > IMX662_FULL_LINES_MAX) ? IMX662_FULL_LINES_MAX : vmax;

	sns_state->fl_std = vmax;
	ae_sns_dft->fps            = fps;
	ae_sns_dft->full_lines_std = sns_state->fl_std;
	ae_sns_dft->max_int_time   = sns_state->fl_std - IMX662_MIN_SHR0_LINEAR;
	cmos_config_vmax(sns_state, vmax);
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines,
									   ot_isp_ae_sensor_default *ae_sns_dft)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_check_pointer_void_return(ae_sns_dft);

	full_lines = (full_lines > IMX662_FULL_LINES_MAX) ? IMX662_FULL_LINES_MAX : full_lines;
	sns_state->fl[0] = full_lines;
	ae_sns_dft->full_lines = full_lines;
	ae_sns_dft->max_int_time = full_lines - IMX662_MIN_SHR0_LINEAR;
	cmos_config_vmax(sns_state, full_lines);
}

/* SHR0 = frame_length - int_time (even-aligned), at i2c_data[0..2]. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	td_u32 fl = sns_state->fl[0] ? sns_state->fl[0] : sns_state->fl_std;
	td_u32 shr0;

	if (int_time > fl - IMX662_MIN_SHR0_LINEAR) {
		int_time = fl - IMX662_MIN_SHR0_LINEAR;
	}
	shr0 = fl - int_time;
	shr0 &= ~0x1u;                        /* even alignment */
	if (shr0 < IMX662_MIN_SHR0_LINEAR) {
		shr0 = IMX662_MIN_SHR0_LINEAR;
	}
	sns_state->regs_info[0].i2c_data[0].data = low_8bits(shr0);
	sns_state->regs_info[0].i2c_data[1].data = high_8bits(shr0);
	sns_state->regs_info[0].i2c_data[2].data = higher_4bits(shr0);
}

/* 0.3 dB / LSB. HiSilicon 'again' linear units: 1024 == 1x. */
static td_u32 imx662_gain_lin_to_reg(td_u32 again_lin)
{
	td_double db, reg;
	if (again_lin < IMX662_AGAIN_MIN) {
		again_lin = IMX662_AGAIN_MIN;
	}
	db  = 20.0 * log10((td_double)again_lin / 1024.0);
	reg = db / 0.3;
	if (reg < 0) {
		reg = 0;
	}
	if (reg > IMX662_GAIN_REG_MAX) {
		reg = IMX662_GAIN_REG_MAX;
	}
	return (td_u32)(reg + 0.5);
}

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
	td_u32 reg;
	ot_unused(vi_pipe);
	sns_check_pointer_void_return(again_lin);
	sns_check_pointer_void_return(again_db);

	reg = imx662_gain_lin_to_reg(*again_lin);
	*again_db  = reg;                                        /* register value */
	*again_lin = (td_u32)(1024.0 * pow(10.0, (reg * 0.3) / 20.0) + 0.5); /* snapped */
}

static td_void cmos_dgain_calc_table(ot_vi_pipe vi_pipe, td_u32 *dgain_lin, td_u32 *dgain_db)
{
	ot_unused(vi_pipe);
	sns_check_pointer_void_return(dgain_lin);
	sns_check_pointer_void_return(dgain_db);
	*dgain_lin = 1024;   /* no separate digital gain register on IMX662 */
	*dgain_db  = 0;
}

/* Conversion-gain state, per pipe.  Held here rather than derived fresh each
 * frame so the hysteresis below has something to latch against. */
static td_bool g_imx662_hcg_on[OT_ISP_MAX_PIPE_NUM] = { 0 };

/* gain register at fast-update i2c_data[3..4], conversion gain at [8]. */
static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	/* cmos_again_calc_table() returns the snapped linear gain through its
	 * first pointer and the sensor's 0.3 dB register code through its second.
	 * The AE core calls this function with that register code, not the linear
	 * gain.  Re-converting it as 1x=1024 clamps every request to register 0. */
	td_u32 reg = again;
	ot_unused(dgain);

	if (reg > IMX662_GAIN_REG_MAX) {
		reg = IMX662_GAIN_REG_MAX;
	}

	/* Dual conversion gain.  HCG lowers read noise; it also multiplies the
	 * frame by ~5.8x, so the written code must give that back or AE has no
	 * fixed point in the band where it engages (see imx662_cmos.h).
	 *
	 * `reg` arrives as the TOTAL gain the AE asked for, so subtracting the
	 * offset keeps cmos_again_calc_table()'s reported again_lin -- and hence
	 * the ISP's ISO bucket, which indexes the sharpen, saturation and NR
	 * tables -- describing the gain actually delivered.
	 *
	 * Slot 8 is linear-mode only: ClearHDR needs 3030h = 02h and a matching
	 * FDG_SEL1 (3031h) that has no slot here, so stamping 0/1 every frame
	 * would silently corrupt an HDR blend.  cmos_set_wdr_mode() rejects every
	 * non-linear mode today; this keeps that true if it stops doing so. */
	if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
		if (!g_imx662_hcg_on[vi_pipe] && reg >= IMX662_HCG_ON_REG) {
			g_imx662_hcg_on[vi_pipe] = TD_TRUE;
		} else if (g_imx662_hcg_on[vi_pipe] && reg < IMX662_HCG_OFF_REG) {
			g_imx662_hcg_on[vi_pipe] = TD_FALSE;
		}
		if (g_imx662_hcg_on[vi_pipe]) {
			/* Test BEFORE subtracting: reg is unsigned, so an underflow
			 * would wrap to a huge value that a post-subtraction floor
			 * check reads as "above the floor" and lets through.  The
			 * hysteresis makes this unreachable (latched implies
			 * reg >= OFF_REG, and the header asserts
			 * OFF_REG - OFFSET >= the floor), but a guard that cannot
			 * catch the case it names is worse than no guard. */
			if (reg < IMX662_HCG_GAIN_OFFSET + IMX662_HCG_GAIN_REG_MIN) {
				reg = IMX662_HCG_GAIN_REG_MIN;
			} else {
				reg -= IMX662_HCG_GAIN_OFFSET;
			}
		}
		sns_state->regs_info[0].i2c_data[8].data =
			g_imx662_hcg_on[vi_pipe] ? IMX662_FDG_HCG : IMX662_FDG_LCG;
	}

	sns_state->regs_info[0].i2c_data[3].data = low_8bits(reg);
	sns_state->regs_info[0].i2c_data[4].data = high_8bits(reg);
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
									ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
	/* Linear-only for now: no long/short split. TODO(HDR). */
	ot_unused(vi_pipe); ot_unused(man_ratio_enable); ot_unused(ratio);
	ot_unused(int_time); ot_unused(lf_max_int_time);
}

static td_void cmos_ae_fswdr_attr_set(ot_vi_pipe vi_pipe, ot_isp_ae_fswdr_attr *ae_fswdr_attr)
{
	ot_unused(vi_pipe); ot_unused(ae_fswdr_attr);   /* TODO(HDR) */
}

static td_s32 cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
	sns_check_pointer_return(exp_func);
	(td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));

	exp_func->pfn_cmos_get_ae_default     = cmos_get_ae_default;
	exp_func->pfn_cmos_fps_set            = cmos_fps_set;
	exp_func->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
	exp_func->pfn_cmos_inttime_update     = cmos_inttime_update;
	exp_func->pfn_cmos_gains_update       = cmos_gains_update;
	exp_func->pfn_cmos_again_calc_table   = cmos_again_calc_table;
	exp_func->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
	exp_func->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
	exp_func->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;
	return TD_SUCCESS;
}

/* ==========================================================================
 *  AWB — factory mode-0 calibration recovered from xipc
 * ========================================================================== */
/* The recovered factory matrices overcorrect this module strongly toward
 * magenta. Hardware A/B testing with colored and neutral references selected
 * a 15% blend from identity toward each factory anchor as the conservative
 * first production baseline. Keep the per-CCT interpolation rather than
 * freezing the single 4800 K matrix used during the live test. */
static const ot_isp_awb_ccm g_imx662_awb_ccm = {
	4,
	{
		{ 7500, { 0x0114, 0x801a, 0x0006, 0x8009, 0x010c, 0x8003, 0x0005, 0x801e, 0x0119 } },
		{ 6500, { 0x0119, 0x801a, 0x0002, 0x800b, 0x010e, 0x8003, 0x0005, 0x801e, 0x0119 } },
		{ 4800, { 0x0119, 0x801b, 0x0001, 0x800c, 0x0115, 0x8008, 0x0009, 0x8023, 0x011b } },
		{ 2600, { 0x011f, 0x801c, 0x8004, 0x8010, 0x0110, 0x0000, 0x0012, 0x803c, 0x012a } },
	},
};

static td_s32 cmos_get_awb_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
	ot_unused(vi_pipe);
	sns_check_pointer_return(awb_sns_dft);
	(td_void)memset_s(awb_sns_dft, sizeof(ot_isp_awb_sensor_default), 0,
					  sizeof(ot_isp_awb_sensor_default));

	awb_sns_dft->wb_ref_temp    = IMX662_AWB_STATIC_TEMP;
	awb_sns_dft->gain_offset[0] = IMX662_AWB_STATIC_WB_R;    /* R  */
	awb_sns_dft->gain_offset[1] = IMX662_AWB_STATIC_WB_GR;   /* Gr */
	awb_sns_dft->gain_offset[2] = IMX662_AWB_STATIC_WB_GB;   /* Gb */
	awb_sns_dft->gain_offset[3] = IMX662_AWB_STATIC_WB_B;    /* B  */
	awb_sns_dft->wb_para[0] = IMX662_AWB_P1;
	awb_sns_dft->wb_para[1] = IMX662_AWB_P2;
	awb_sns_dft->wb_para[2] = IMX662_AWB_Q1;
	awb_sns_dft->wb_para[3] = IMX662_AWB_A1;
	awb_sns_dft->wb_para[4] = IMX662_AWB_B1;
	awb_sns_dft->wb_para[5] = IMX662_AWB_C1;
	(td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(awb_sns_dft->ccm),
					  &g_imx662_awb_ccm, sizeof(g_imx662_awb_ccm));
	/* Without the agc_tbl copy the memset above leaves agc_tbl.valid = 0 and
	 * every saturation entry 0, so the ISP is never told what saturation to
	 * run.  The sector copy is seeded but inert: g_imx662_color_sector.valid
	 * is 0, so the ISP ignores the hue/sat shifts until that is raised. */
	(td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(awb_sns_dft->agc_tbl),
					  &g_imx662_awb_agc_table, sizeof(g_imx662_awb_agc_table));
	(td_void)memcpy_s(&awb_sns_dft->sector, sizeof(awb_sns_dft->sector),
					  &g_imx662_color_sector, sizeof(g_imx662_color_sector));
	/* TODO(tuning): IR-cut-aware day/night bank. */
	return TD_SUCCESS;
}

static td_s32 cmos_init_awb_exp_function(ot_isp_awb_sensor_exp_func *exp_func)
{
	sns_check_pointer_return(exp_func);
	(td_void)memset_s(exp_func, sizeof(ot_isp_awb_sensor_exp_func), 0,
					  sizeof(ot_isp_awb_sensor_exp_func));
	exp_func->pfn_cmos_get_awb_default = cmos_get_awb_default;
	return TD_SUCCESS;
}

/* ==========================================================================
 *  ISP defaults
 * ========================================================================== */
static td_s32 cmos_get_isp_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_check_pointer_return(isp_def);
	sns_check_pointer_return(sns_state);
	(td_void)memset_s(isp_def, sizeof(ot_isp_cmos_default), 0, sizeof(ot_isp_cmos_default));

	/* ot_isp_cmos_alg_key holds its bitfields directly, not under a .bits
	 * member. Everything is 0 from the memset above, so a block is off unless
	 * it is turned on here -- and each key needs its parameter table, or the
	 * ISP reads a null pointer.
	 *
	 * Linear mode only; cmos_set_wdr_mode() rejects every other WDR mode.
	 *
	 * Everything enabled below comes from sc450ai, which targets this same ISP
	 * silicon; see imx662_cmos_param.h for which of it transfers on principle
	 * and which is borrowed noise tuning kept on a hardware A/B.
	 *
	 * Still off, each for its own reason: lsc needs a per-module shading
	 * capture, and dpc is enabled in sc450ai's common path and is a candidate
	 * here but has not been measured on this sensor yet. */
	isp_def->key.bit1_demosaic         = 1;
	isp_def->demosaic                  = &g_cmos_demosaic;
	isp_def->key.bit1_gamma            = 1;
	isp_def->gamma                     = &g_cmos_gamma;
	isp_def->key.bit1_clut             = 1;
	isp_def->clut                      = &g_cmos_clut;
	isp_def->key.bit1_anti_false_color = 1;
	isp_def->anti_false_color          = &g_cmos_anti_false_color;
	isp_def->key.bit1_cac              = 1;
	isp_def->cac                       = &g_cmos_cac;
	isp_def->key.bit1_ldci             = 1;
	isp_def->ldci                      = &g_cmos_ldci;
	isp_def->key.bit1_dehaze           = 1;
	isp_def->dehaze                    = &g_cmos_dehaze;
	isp_def->key.bit1_ca               = 1;
	isp_def->ca                        = &g_cmos_ca;
	isp_def->key.bit1_bayer_nr         = 1;
	isp_def->bayer_nr                  = &g_cmos_bayer_nr;
	isp_def->key.bit1_sharpen          = 1;
	isp_def->sharpen                   = &g_cmos_yuv_sharpen;
	isp_def->key.bit1_drc              = 1;
	isp_def->drc                       = &g_cmos_drc;
	(td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
					  &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));

	/* The ISP identifies the sensor by these. sns_id was 0, which disagrees
	 * with cv610_pipeline.c's IMX662_SNS_ID -- that file already documents
	 * the two as having to match -- and would misbind any future PQ .bin,
	 * which is chip- and sensor-locked. sns_mode was never set at all. */
	isp_def->sns_mode.sns_id        = IMX662_ID;
	isp_def->sns_mode.sns_mode      = sns_state->img_mode;
	return TD_SUCCESS;
}

static td_s32 cmos_get_isp_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
	td_s32 i;
	ot_unused(vi_pipe);
	sns_check_pointer_return(black_level);
	(td_void)memset_s(black_level, sizeof(ot_isp_cmos_black_level), 0,
					  sizeof(ot_isp_cmos_black_level));

	black_level->auto_attr.update = TD_TRUE;
	/* One value for every mode -- the field is a fixed 14-bit ISP domain, not
	 * the sensor's output depth.  See IMX662_BLACK_LEVEL. */
	for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
		black_level->auto_attr.black_level[0][i] = IMX662_BLACK_LEVEL;
	}
	return TD_SUCCESS;
}

static td_void cmos_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
	/* Optional bad-pixel-detect assist during ISP calibration. */
	ot_unused(vi_pipe); ot_unused(enable);
}

static td_s32 cmos_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_check_pointer_return(sns_state);

	switch (mode) {
		case OT_WDR_MODE_NONE:
			sns_state->wdr_mode = OT_WDR_MODE_NONE;
			sns_state->fl_std   = IMX662_VMAX_1080P30_LINEAR;
			break;
		default:
			isp_err_trace("IMX662: unsupported WDR mode %d (linear only in scaffold)\n", mode);
			return TD_FAILURE;   /* TODO(HDR): Clear-HDR 2To1 */
	}
	sns_state->sync_init = TD_FALSE;
	return TD_SUCCESS;
}

/* ==========================================================================
 *  Fast-update register table (the 8 regs the ISP writes per frame)
 * ========================================================================== */
static td_void cmos_comm_sns_reg_info_init(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
	td_u32 i;
	/* Sized from the initializer so adding a slot cannot silently overflow;
	 * reg_num below is derived from it for the same reason. */
	const td_u16 reg_addr[] = {
		IMX662_REG_SHR0_L, IMX662_REG_SHR0_M, IMX662_REG_SHR0_H,   /* 0..2 exposure */
		IMX662_REG_GAIN_L, IMX662_REG_GAIN_H,                       /* 3..4 gain     */
		IMX662_REG_VMAX_L, IMX662_REG_VMAX_M, IMX662_REG_VMAX_H,    /* 5..7 vmax     */
		IMX662_REG_FDG_SEL0                                         /* 8    HCG/LCG  */
	};

	sns_state->regs_info[0].sns_type         = OT_ISP_SNS_TYPE_I2C;
	sns_state->regs_info[0].com_bus.i2c_dev  = g_imx662_bus_info[vi_pipe].i2c_dev;
	sns_state->regs_info[0].cfg2_valid_delay_max = 2;
	sns_state->regs_info[0].reg_num =
		(td_u32)(sizeof(reg_addr) / sizeof(reg_addr[0]));

	for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
		sns_state->regs_info[0].i2c_data[i].update        = TD_TRUE;
		sns_state->regs_info[0].i2c_data[i].delay_frame_num = 0;
		sns_state->regs_info[0].i2c_data[i].dev_addr      = IMX662_I2C_ADDR;
		sns_state->regs_info[0].i2c_data[i].addr_byte_num = IMX662_ADDR_BYTE;
		sns_state->regs_info[0].i2c_data[i].data_byte_num = IMX662_DATA_BYTE;
		sns_state->regs_info[0].i2c_data[i].reg_addr      = reg_addr[i];
	}
}

static td_void cmos_sns_reg_info_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
	td_u32 i;
	ot_unused(vi_pipe);
	for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
		if (sns_state->regs_info[0].i2c_data[i].data == sns_state->regs_info[1].i2c_data[i].data) {
			sns_state->regs_info[0].i2c_data[i].update = TD_FALSE;
		} else {
			sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
		}
	}
}

static td_s32 cmos_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_check_pointer_return(sns_regs_info);
	sns_check_pointer_return(sns_state);

	if ((sns_state->sync_init == TD_FALSE) || (sns_regs_info->config == TD_FALSE)) {
		cmos_comm_sns_reg_info_init(vi_pipe, sns_state);
		sns_state->sync_init = TD_TRUE;
	} else {
		cmos_sns_reg_info_update(vi_pipe, sns_state);
	}
	sns_regs_info->config = TD_FALSE;
	(td_void)memcpy_s(sns_regs_info, sizeof(ot_isp_sns_regs_info),
					  &sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info));
	(td_void)memcpy_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info),
					  &sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info));
	sns_state->fl[1] = sns_state->fl[0];
	return TD_SUCCESS;
}

static td_s32 cmos_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_check_pointer_return(sns_image_mode);
	sns_check_pointer_return(sns_state);

	if (sns_image_mode->width > 1920 || sns_image_mode->height > 1080 ||
		sns_image_mode->fps > 100.0f) {
		isp_err_trace("IMX662: unsupported mode %ux%u@%0.2f\n",
					  sns_image_mode->width, sns_image_mode->height,
					  sns_image_mode->fps);
		return TD_FAILURE;
	}
	if (sns_image_mode->fps > 90.0f) {
		sns_state->img_mode = IMX662_SENSOR_2M_100FPS_10BIT_LINEAR_MODE;
	} else if (sns_image_mode->fps > 60.0f) {
		sns_state->img_mode = IMX662_SENSOR_2M_90FPS_10BIT_LINEAR_MODE;
	} else if (sns_image_mode->fps > 30.0f) {
		sns_state->img_mode = IMX662_SENSOR_2M_60FPS_12BIT_LINEAR_MODE;
	} else {
		sns_state->img_mode = IMX662_SENSOR_2M_30FPS_12BIT_LINEAR_MODE;
	}
	sns_state->sync_init = TD_FALSE;
	sns_state->fl_std    = IMX662_VMAX_1080P30_LINEAR;
	sns_state->fl[0]     = sns_state->fl_std;
	sns_state->fl[1]     = sns_state->fl[0];
	return TD_SUCCESS;
}

static td_void sensor_global_init(ot_vi_pipe vi_pipe)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);

	sns_state->init      = TD_FALSE;
	sns_state->sync_init = TD_FALSE;
	sns_state->img_mode  = IMX662_SENSOR_2M_30FPS_12BIT_LINEAR_MODE;
	sns_state->wdr_mode  = OT_WDR_MODE_NONE;
	sns_state->fl_std    = IMX662_VMAX_1080P30_LINEAR;
	sns_state->fl[0]     = IMX662_VMAX_1080P30_LINEAR;
	sns_state->fl[1]     = IMX662_VMAX_1080P30_LINEAR;
	(td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0,
					  sizeof(ot_isp_sns_regs_info));
	(td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0,
					  sizeof(ot_isp_sns_regs_info));
	/* Conversion-gain latch is per-session state, reset alongside regs_info.
	 * A reinit landing while the scene is still dark would otherwise assert
	 * HCG on frame 1 with no gain measurement behind it -- with the offset
	 * compensation now applied, that is a 5.8x exposure error. */
	g_imx662_hcg_on[vi_pipe] = TD_FALSE;
}

static td_s32 cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
	sns_check_pointer_return(sensor_exp_func);
	(td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

	sensor_exp_func->pfn_cmos_sns_init          = imx662_init;
	sensor_exp_func->pfn_cmos_sns_exit          = imx662_exit;
	sensor_exp_func->pfn_cmos_sns_global_init   = sensor_global_init;
	sensor_exp_func->pfn_cmos_set_image_mode    = cmos_set_image_mode;
	sensor_exp_func->pfn_cmos_set_wdr_mode      = cmos_set_wdr_mode;
	sensor_exp_func->pfn_cmos_get_isp_default   = cmos_get_isp_default;
	sensor_exp_func->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
	sensor_exp_func->pfn_cmos_set_pixel_detect  = cmos_set_pixel_detect;
	sensor_exp_func->pfn_cmos_get_sns_reg_info  = cmos_get_sns_regs_info;
	return TD_SUCCESS;
}

/* ==========================================================================
 *  Registration
 * ========================================================================== */
static td_s32 sensor_register_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib,
									   ot_isp_3a_alg_lib *awb_lib)
{
	ot_isp_sns_register sns_register;
	ot_isp_sns_exp_func *sns_exp_func = &sns_register.sns_exp;
	ot_isp_ae_sensor_register ae_register;
	ot_isp_ae_sensor_exp_func *ae_exp_func = &ae_register.sns_exp;
	ot_isp_awb_sensor_register awb_register;
	ot_isp_awb_sensor_exp_func *awb_exp_func = &awb_register.sns_exp;
	/* The three reg_callback MPIs take a pointer to this, not a bare id. */
	ot_isp_sns_attr_info sns_attr_info = { .sns_id = IMX662_ID };
	td_s32 ret;

	sns_check_pointer_return(ae_lib);
	sns_check_pointer_return(awb_lib);

	cmos_init_sensor_exp_function(sns_exp_func);
	ret = ot_mpi_isp_sensor_reg_callback(vi_pipe, &sns_attr_info, &sns_register);
	if (ret != TD_SUCCESS) {
		isp_err_trace("IMX662: isp sensor reg callback failed!\n");
		return ret;
	}

	cmos_init_ae_exp_function(ae_exp_func);
	ret = ot_mpi_ae_sensor_reg_callback(vi_pipe, ae_lib, &sns_attr_info, &ae_register);
	if (ret != TD_SUCCESS) {
		isp_err_trace("IMX662: ae sensor reg callback failed!\n");
		return ret;
	}

	cmos_init_awb_exp_function(awb_exp_func);
	ret = ot_mpi_awb_sensor_reg_callback(vi_pipe, awb_lib, &sns_attr_info, &awb_register);
	if (ret != TD_SUCCESS) {
		isp_err_trace("IMX662: awb sensor reg callback failed!\n");
		return ret;
	}
	return TD_SUCCESS;
}

static td_s32 sensor_unregister_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib,
										 ot_isp_3a_alg_lib *awb_lib)
{
	td_s32 ret;
	sns_check_pointer_return(ae_lib);
	sns_check_pointer_return(awb_lib);

	ret  = ot_mpi_isp_sensor_unreg_callback(vi_pipe, IMX662_ID);
	ret += ot_mpi_ae_sensor_unreg_callback(vi_pipe, ae_lib, IMX662_ID);
	ret += ot_mpi_awb_sensor_unreg_callback(vi_pipe, awb_lib, IMX662_ID);
	return (ret != TD_SUCCESS) ? TD_FAILURE : TD_SUCCESS;
}

static td_s32 sensor_set_init(ot_vi_pipe vi_pipe, ot_isp_init_attr *init_attr)
{
	sns_check_pointer_return(init_attr);
	g_init_exposure[vi_pipe] = init_attr->exp_time;
	return TD_SUCCESS;
}

static td_s32 sensor_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus bus_info)
{
	g_imx662_bus_info[vi_pipe].i2c_dev = bus_info.i2c_dev;
	return TD_SUCCESS;
}

ot_isp_sns_obj g_sns_imx662_obj = {
	.pfn_register_callback    = sensor_register_callback,
	.pfn_un_register_callback = sensor_unregister_callback,
	.pfn_standby              = imx662_standby,
	.pfn_restart              = imx662_restart,
	.pfn_mirror_flip          = imx662_mirror_flip,
	.pfn_set_blc_clamp        = imx662_blc_clamp,
	.pfn_write_reg            = imx662_write_register,
	.pfn_read_reg             = imx662_read_register,
	.pfn_set_bus_info         = sensor_set_bus_info,
	.pfn_set_init             = sensor_set_init
};
