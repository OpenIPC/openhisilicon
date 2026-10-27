/*
 * imx662_cmos.h — Sony IMX662 (STARVIS 2) driver for HiSilicon Hi3516CV610
 *                 (hi3516cv6xx, V5 "OT" SDK, ss_* / ot_* MPI API).
 *
 * SCAFFOLD — original code, no vendor (Shenshu/HiSilicon) source copied. It
 * matches the *public* HiSilicon sensor-driver interface (ot_isp_sns_obj /
 * pfn_cmos_*) that the SDK headers below define. Register addresses/values are
 * from the Sony IMX662 datasheet and the public Raspberry Pi V4L2 driver
 * (will127534/imx662-v4l2-driver, GPL-2.0). Items to confirm on real hardware
 * are marked "VERIFY"; items to complete at integration are marked "TODO".
 *
 * Build: not standalone — compile in-tree under
 *   OpenIPC/openhisilicon/libraries/sensor/hi3516cv6xx/sony_imx662/
 * against the hi3516cv6xx ISP/kernel headers (see Makefile).
 */

#ifndef IMX662_CMOS_H
#define IMX662_CMOS_H

#include "ot_common.h"
#include "ot_common_isp.h"
#include "ot_common_video.h"
#include "ot_sns_ctrl.h"
#include "ot_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---- I2C wiring -----------------------------------------------------------
 * IMX662 has a strap-selectable address: 0x34 (8-bit) / 0x1A (7-bit), or the
 * alternate 0x36 / 0x1B. VERIFY against the CV610 board with i2cdetect/ipctool.
 */
#define IMX662_I2C_ADDR    0x34
#define IMX662_ADDR_BYTE   2      /* 16-bit register address */
#define IMX662_DATA_BYTE   1      /* 8-bit register data     */

/* ---- IMX662 register map (Sony STARVIS 2; shared with IMX585/678/664) ------
 * Source: IMX662 datasheet + will127534/imx662-v4l2-driver. Multi-byte fields
 * are little-endian across consecutive addresses (L, M, H).
 */
#define IMX662_REG_STANDBY     0x3000  /* 0x01 = standby, 0x00 = operating     */
#define IMX662_REG_XMSTA       0x3002  /* 0x01 = master stop, 0x00 = start     */ /* VERIFY */
#define IMX662_REG_INCK_SEL    0x3014  /* master clock select (see table)      */
#define IMX662_REG_DATARATE    0x3015  /* MIPI data-rate select                */
#define IMX662_REG_WINMODE     0x3018  /* windowing/mode                       */ /* VERIFY */
#define IMX662_REG_WDMODE      0x301A  /* 0x00 normal / Clear-HDR select       */
#define IMX662_REG_HREVERSE    0x3020  /* horizontal flip (bit0)               */
#define IMX662_REG_VREVERSE    0x3021  /* vertical flip (bit0)                  */
#define IMX662_REG_ADBIT       0x3022  /* AD bit depth (10/12-bit)             */ /* VERIFY */
#define IMX662_REG_ODBIT       0x3023  /* MIPI output bit depth (10/12-bit)    */
#define IMX662_REG_VMAX_L      0x3028  /* VMAX [19:0], default 1250            */
#define IMX662_REG_VMAX_M      0x3029
#define IMX662_REG_VMAX_H      0x302A
#define IMX662_REG_HMAX_L      0x302C  /* HMAX [15:0]                          */
#define IMX662_REG_HMAX_H      0x302D
#define IMX662_REG_FDG_SEL0    0x3030  /* conversion gain HCG/LCG select       */
#define IMX662_REG_LANEMODE    0x3040  /* 0x01 = 2-lane, 0x03 = 4-lane         */ /* VERIFY */
#define IMX662_REG_SHR0_L      0x3050  /* SHR0 [19:0] exposure                 */
#define IMX662_REG_SHR0_M      0x3051
#define IMX662_REG_SHR0_H      0x3052
/* GAIN[10:0], 0.3 dB / LSB, 0.0..72 dB.  Sony's register list (IMX662
 * Register.xlsx, and SRM Rev3.0) puts GAIN at 3070h/3071h and jumps straight
 * from 3069h to 3070h -- 306Ch/306Dh are not registers at all, so the gain
 * writes landed nowhere and analog gain never actuated.  The vendor's own
 * reference driver carries the same wrong pair; do not "restore" it.
 * 3072h/3073h and 3074h/3075h are GAIN_1/GAIN_2 (DOL per-frame), not this. */
#define IMX662_REG_GAIN_L      0x3070
#define IMX662_REG_GAIN_H      0x3071
#define IMX662_REG_BLKLEVEL_L  0x30DC  /* black level (OFFSET), default 0x32=50 */
#define IMX662_REG_BLKLEVEL_H  0x30DD
#define IMX662_REG_DIG_CLAMP   0x3458  /* digital clamp                        */ /* VERIFY */
#define IMX662_REG_AD10_0      0x3A50  /* ADC timing: differs for 10/12 bit    */
#define IMX662_REG_AD10_1      0x3A51
#define IMX662_REG_AD10_2      0x3A52

/* ---- exposure / gain model ------------------------------------------------
 * Sony SHR model:  integration_lines = VMAX - SHR0   (SHR0 even-aligned)
 *   -> SHR0 = VMAX - int_time, clamped to [MIN_SHR0, VMAX - 1].
 * Gain: single register, 0.3 dB per LSB. HiSilicon passes 'again' as a linear
 *   value where 1024 == 1x, so:  gain_reg = round( 20*log10(again/1024) / 0.3 ).
 */
#define IMX662_VMAX_1080P30_LINEAR   1250u
#define IMX662_HMAX_1080P30_LINEAR   1980u   /* 37.125 MHz INCK, 30 fps        */
#define IMX662_HMAX_1080P60_LINEAR    990u   /* 37.125 MHz INCK, 60 fps        */
#define IMX662_HMAX_1080P90_LINEAR    660u   /* 37.125 MHz INCK, 90 fps        */
#define IMX662_HMAX_1080P100_LINEAR   668u   /* 27 MHz overclocked 24 MHz mode */
#define IMX662_FULL_LINES_MAX        0xFFFFFu
#define IMX662_MIN_SHR0_LINEAR       8u       /* min shutter (linear)           */
#define IMX662_MIN_SHR0_HDR          10u      /* min shutter (Clear-HDR)        */

#define IMX662_AGAIN_MIN             1024u                  /* 1x  (0 dB)        */
/* 398x = 52 dB, NOT the ~72 dB the old comment claimed -- the /10 makes it a
 * factor of 10 (20 dB) smaller than 10^(72/20).  Left at 52 dB deliberately:
 * IMX662_GAIN_REG_MAX (240 codes x 0.3 dB) does allow 72 dB, but the measured
 * complaint on this bench is too much gain, not too little.  Raise this only
 * together with the ISP-digital cap below, and A/B it -- the two together set
 * the total ceiling. */
#define IMX662_AGAIN_MAX             (1024u * 3981u / 10u)  /* 398x = 52 dB     */
#define IMX662_GAIN_STEP_MDB         30       /* 0.3 dB, in milli-dB            */
#define IMX662_GAIN_REG_MAX          240u     /* 72 dB / 0.3 dB                 */

/* Dual conversion gain.
 *
 * HCG is NOT brightness-neutral.  The GAIN register is PGA gain applied after
 * the conversion gain, so the two multiply: asserting FDG_SEL0=1 at an
 * unchanged code brightens the frame by the conversion-efficiency ratio.  The
 * datasheet gives that ratio directly -- Rcg (HCG/LCG) min 5.6, typ 5.8, max
 * 6.0, corroborated by the electro-optical table's G sensitivity, HCG 18383 vs
 * LCG 3166 Digit/lx/s = 5.81x.  5.8x is 15.3 dB, and at 0.3 dB per code that
 * is IMX662_HCG_GAIN_OFFSET below.
 *
 * Sony's 22h (34-code) floor is NOT the conversion gain -- that reading would
 * require a 51-code floor.  It is a saturation constraint: Vsat is 3895 digits
 * in LCG against 1204 in HCG, a ratio of 3.235, and 34 codes x 0.3 dB = 10.2 dB
 * = 3.236x.  The floor exists so HCG's clip level refills the ADC to LCG's.
 *
 * So a write must subtract the offset while HCG is selected, and the thresholds
 * must sit where the compensated code still clears the floor: a stable HCG
 * solution needs (requested - 51) >= 34, i.e. requested >= 85.  Latching on
 * below that has no fixed point -- AE brightens 5.8x, slams the code down past
 * the floor, HCG drops out, the frame goes dark, and it limit-cycles.  That
 * band is ordinary dim-indoor light, between the daylight and dark-room cases
 * that are easy to test. */
#define IMX662_HCG_GAIN_OFFSET       51u      /* 15.3 dB / 0.3 dB per code      */
#define IMX662_HCG_GAIN_REG_MIN      34u      /* 10.2 dB; SRM lower bound       */
#define IMX662_HCG_ON_REG            91u      /* requested; writes 40           */
#define IMX662_HCG_OFF_REG           85u      /* requested; writes 34 = floor   */
#define IMX662_FDG_LCG               0x00u
#define IMX662_FDG_HCG               0x01u

/* The floor is held by the OFF threshold, not by the clamp in
 * cmos_gains_update(); keep them consistent if either is ever retuned. */
/* Signed: all three macros are u-suffixed, so an unsigned subtraction wraps and
 * the assert passes clean for exactly the retune that breaks it (OFF_REG below
 * OFFSET).  Lowering OFF_REG is the natural response to "HCG drops out too
 * eagerly", and it re-enters the unstable band the block above warns about. */
typedef char imx662_hcg_off_clears_floor[
	((int)IMX662_HCG_OFF_REG - (int)IMX662_HCG_GAIN_OFFSET >=
		(int)IMX662_HCG_GAIN_REG_MIN) ? 1 : -1];
/* An inverted pair would toggle conversion gain every frame -- a visible 5.8x
 * flicker at frame rate -- and nothing else rejects it. */
typedef char imx662_hcg_hysteresis_ordered[
	(IMX662_HCG_ON_REG > IMX662_HCG_OFF_REG) ? 1 : -1];

/* ---- resolution modes -----------------------------------------------------
 * Bring linear up first. Add Clear-HDR (2-frame) as a second mode later.
 */
typedef enum {
    IMX662_SENSOR_2M_30FPS_12BIT_LINEAR_MODE = 0,
    IMX662_SENSOR_2M_60FPS_12BIT_LINEAR_MODE,
    IMX662_SENSOR_2M_90FPS_10BIT_LINEAR_MODE,
    IMX662_SENSOR_2M_100FPS_10BIT_LINEAR_MODE,
    IMX662_MODE_BUTT
} imx662_res_mode;

typedef struct {
    td_u32      ver_lines;      /* VMAX at nominal fps        */
    td_u32      max_ver_lines;  /* FULL_LINES_MAX             */
    td_float    max_fps;
    td_float    min_fps;
    td_u32      width;
    td_u32      height;
    td_u8       sns_mode;
    ot_wdr_mode wdr_mode;
    const char *mode_name;
} imx662_video_mode_tbl;

/* ---- context accessors + transport (imx662_sensor_ctl.c) ------------------ */
ot_isp_sns_state   *imx662_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *imx662_get_bus_info(ot_vi_pipe vi_pipe);

td_void imx662_init(ot_vi_pipe vi_pipe);
td_void imx662_exit(ot_vi_pipe vi_pipe);
td_void imx662_standby(ot_vi_pipe vi_pipe);
td_void imx662_restart(ot_vi_pipe vi_pipe);
td_s32  imx662_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  imx662_read_register(ot_vi_pipe vi_pipe, td_u32 addr);
td_void imx662_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip);
td_void imx662_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp);
td_s32  imx662_i2c_init(ot_vi_pipe vi_pipe);
td_s32  imx662_i2c_exit(ot_vi_pipe vi_pipe);

/* exported registration object (imx662_cmos.c) */
extern ot_isp_sns_obj g_sns_imx662_obj;

#ifdef __cplusplus
}
#endif
#endif /* IMX662_CMOS_H */
