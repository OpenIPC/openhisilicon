/*
 * imx662_cfg.h — IMX662 power-on register sequence.
 *
 * Sony's one-shot init block: the reserved analog/ADC trim, the MIPI TX
 * timing, the readout window and the HDR-context registers that must be
 * parked for linear mode.  The ISP never writes these; the plugin does,
 * once, while the sensor is in standby.
 *
 * Transcribed from imx662_global_settings[] in pauliustumas/imx662
 * (driver/imx662.c), covering 0x301A..0x4549.  Values are held verbatim,
 * comments included, so a future diff against that source stays mechanical.
 *
 * Departures from the source list:
 *
 *   - {0x3002, 0x00} (master mode operation start) is dropped.  This
 *     table runs inside standby; imx662_default_reg_init() releases
 *     STANDBY and XMSTA itself once the mode registers are in.
 *   - The HMAX pair is absent — it is commented out upstream, and HMAX
 *     is per-mode state that imx662_linear_1080p_init() owns.
 *   - {0x44C0, 0x7F} is ADDED: upstream omits it and Sony's map requires
 *     it.  See the comment at that entry.
 *   - {0x30A6, 0x0F} is KEPT as upstream has it, which leaves XVS_DRV /
 *     XHS_DRV at Hi-Z, where Sony's All-pixel modes set 0x00 (drive).
 *     Nothing on this module consumes XVS/XHS, and starting to drive two
 *     pads whose wiring is unknown is a hardware risk with no benefit
 *     today.  Revisit if frame-sync or a second sensor is ever wired.
 *
 * The 0x3A50/51/52 AD-timing triplet below carries the 12-bit values, and
 * 0x301A appears here and again in the per-mode block.  Both are harmless
 * overlaps only because this table runs FIRST — see imx662_sensor_ctl.c,
 * where that ordering is load-bearing for the RAW10 modes.
 *
 * Measured against silicon on the 192.168.2.181 bench, 2026-08-16:
 * 94 of these entries differ from the sensor's power-on state, and the
 * readout-window group (0x303C-0x303F, 0x3044-0x3047) already matches
 * bit for bit, so writing it is a confirmed no-op on this module.  A
 * RAW10-vs-RAW12 dump of the whole range showed the reserved blocks are
 * identical in both, i.e. none of this is bit-depth dependent.
 */

#ifndef IMX662_CFG_H
#define IMX662_CFG_H

#include "ot_type.h"

typedef struct {
	td_u16 addr;
	td_u8  data;
} imx662_reg_cfg;

static const imx662_reg_cfg g_imx662_init_seq[] = {
	{ 0x301A, 0x00 }, /* HDR mode select (Normal) */
	{ 0x301B, 0x00 }, /* Normal/binning */
	{ 0x301C, 0x00 }, /* XVS sub sample */
	{ 0x301E, 0x01 }, /* virtual channel */
	{ 0x303C, 0x00 }, /* PIX HSTART */
	{ 0x303D, 0x00 }, /* PIX HSTART */
	{ 0x303E, 0x90 }, /* H WIDTH */
	{ 0x303F, 0x07 }, /* H WIDTH */
	{ 0x3044, 0x00 }, /* PIX VSTART */
	{ 0x3045, 0x00 }, /* PIX VSTART */
	{ 0x3046, 0x4C }, /* V WIDTH */
	{ 0x3047, 0x04 }, /* V WIDTH */
	{ 0x3060, 0x16 }, /* DOL output timing */
	{ 0x3061, 0x01 }, /* DOL output timing */
	{ 0x3062, 0x00 }, /* DOL output timing */
	{ 0x3064, 0xC4 }, /* DOL output timing */
	{ 0x3065, 0x0C }, /* DOL output timing */
	{ 0x3066, 0x00 }, /* DOL output timing */
	{ 0x3069, 0x00 }, /* Direct Gain Enable */
	{ 0x3072, 0x00 }, /* GAIN SEF1 */
	{ 0x3073, 0x00 }, /* GAIN SEF1 */
	{ 0x3074, 0x00 }, /* GAIN SEF2 */
	{ 0x3075, 0x00 }, /* GAIN SEF2 */
	{ 0x3081, 0x00 }, /* EXP_GAIN */
	{ 0x308C, 0x00 }, /* Clear HDR DGAIN */
	{ 0x308D, 0x01 }, /* Clear HDR DGAIN */
	{ 0x3094, 0x00 }, /* CHDR AGAIN LG */
	{ 0x3095, 0x00 }, /* CHDR AGAIN LG */
	{ 0x3096, 0x00 }, /* CHDR AGAIN1 */
	{ 0x3097, 0x00 }, /* CHDR AGAIN1 */
	{ 0x309C, 0x00 }, /* CHDR AGAIN HG */
	{ 0x309D, 0x00 }, /* CHDR AGAIN HG */
	{ 0x30A4, 0xAA }, /* XVS/XHS OUT */
	{ 0x30A6, 0x0F }, /* XVS/XHS DRIVE HiZ */
	{ 0x30CC, 0x00 }, /* XVS width */
	{ 0x30CD, 0x00 }, /* XHS width */
	{ 0x3400, 0x01 }, /* GAIN Adjust */
	{ 0x3444, 0xAC }, /* RESERVED */
	{ 0x3460, 0x21 }, /* Normal Mode 22H=C HDR mode */
	{ 0x3492, 0x08 }, /* RESERVED */
	{ 0x3A50, 0xFF }, /* Normal 12bit */
	{ 0x3A51, 0x03 }, /* Normal 12bit */
	{ 0x3A52, 0x00 }, /* AD 12bit */
	{ 0x3B00, 0x39 }, /* RESERVED */
	{ 0x3B23, 0x2D }, /* RESERVED */
	{ 0x3B45, 0x04 }, /* RESERVED */
	{ 0x3C0A, 0x1F }, /* RESERVED */
	{ 0x3C0B, 0x1E }, /* RESERVED */
	{ 0x3C38, 0x21 }, /* RESERVED */
	{ 0x3C40, 0x06 }, /* Normal mode. CHDR=05h */
	{ 0x3C44, 0x00 }, /* RESERVED */
	{ 0x3CB6, 0xD8 }, /* RESERVED */
	{ 0x3CC4, 0xDA }, /* RESERVED */
	{ 0x3E24, 0x79 }, /* RESERVED */
	{ 0x3E2C, 0x15 }, /* RESERVED */
	{ 0x3EDC, 0x2D }, /* RESERVED */
	{ 0x4498, 0x05 }, /* RESERVED */
	{ 0x449C, 0x19 }, /* RESERVED */
	{ 0x449D, 0x00 }, /* RESERVED */
	{ 0x449E, 0x32 }, /* RESERVED */
	{ 0x449F, 0x01 }, /* RESERVED */
	{ 0x44A0, 0x92 }, /* RESERVED */
	{ 0x44A2, 0x91 }, /* RESERVED */
	{ 0x44A4, 0x8C }, /* RESERVED */
	{ 0x44A6, 0x87 }, /* RESERVED */
	{ 0x44A8, 0x82 }, /* RESERVED */
	{ 0x44AA, 0x78 }, /* RESERVED */
	{ 0x44AC, 0x6E }, /* RESERVED */
	{ 0x44AE, 0x69 }, /* RESERVED */
	{ 0x44B0, 0x92 }, /* RESERVED */
	{ 0x44B2, 0x91 }, /* RESERVED */
	{ 0x44B4, 0x8C }, /* RESERVED */
	{ 0x44B6, 0x87 }, /* RESERVED */
	{ 0x44B8, 0x82 }, /* RESERVED */
	{ 0x44BA, 0x78 }, /* RESERVED */
	{ 0x44BC, 0x6E }, /* RESERVED */
	{ 0x44BE, 0x69 }, /* RESERVED */
	/* 0x44C0 is absent from pauliustumas/imx662's imx662_global_settings[],
	 * which this table was transcribed from -- the upstream list is wrong
	 * against Sony's map, which gives 44C0h "Set to 7Fh" (reset 00h) in all
	 * 18 All-pixel modes.  0x44C0/0x44C1 is the first entry of an 8x16-bit
	 * little-endian table mirrored verbatim at 0x44D0..0x44DF, and the mirror
	 * below does carry 7F/01 -- so without this the two banks disagree at
	 * exactly one index, bank 0 latching 0x0100 instead of 0x017F. */
	{ 0x44C0, 0x7F }, /* RESERVED */
	{ 0x44C1, 0x01 }, /* RESERVED */
	{ 0x44C2, 0x7F }, /* RESERVED */
	{ 0x44C3, 0x01 }, /* RESERVED */
	{ 0x44C4, 0x7A }, /* RESERVED */
	{ 0x44C5, 0x01 }, /* RESERVED */
	{ 0x44C6, 0x7A }, /* RESERVED */
	{ 0x44C7, 0x01 }, /* RESERVED */
	{ 0x44C8, 0x70 }, /* RESERVED */
	{ 0x44C9, 0x01 }, /* RESERVED */
	{ 0x44CA, 0x6B }, /* RESERVED */
	{ 0x44CB, 0x01 }, /* RESERVED */
	{ 0x44CC, 0x6B }, /* RESERVED */
	{ 0x44CD, 0x01 }, /* RESERVED */
	{ 0x44CE, 0x5C }, /* RESERVED */
	{ 0x44CF, 0x01 }, /* RESERVED */
	{ 0x44D0, 0x7F }, /* RESERVED */
	{ 0x44D1, 0x01 }, /* RESERVED */
	{ 0x44D2, 0x7F }, /* RESERVED */
	{ 0x44D3, 0x01 }, /* RESERVED */
	{ 0x44D4, 0x7A }, /* RESERVED */
	{ 0x44D5, 0x01 }, /* RESERVED */
	{ 0x44D6, 0x7A }, /* RESERVED */
	{ 0x44D7, 0x01 }, /* RESERVED */
	{ 0x44D8, 0x70 }, /* RESERVED */
	{ 0x44D9, 0x01 }, /* RESERVED */
	{ 0x44DA, 0x6B }, /* RESERVED */
	{ 0x44DB, 0x01 }, /* RESERVED */
	{ 0x44DC, 0x6B }, /* RESERVED */
	{ 0x44DD, 0x01 }, /* RESERVED */
	{ 0x44DE, 0x5C }, /* RESERVED */
	{ 0x44DF, 0x01 }, /* RESERVED */
	{ 0x4534, 0x1C }, /* RESERVED */
	{ 0x4535, 0x03 }, /* RESERVED */
	{ 0x4538, 0x1C }, /* RESERVED */
	{ 0x4539, 0x1C }, /* RESERVED */
	{ 0x453A, 0x1C }, /* RESERVED */
	{ 0x453B, 0x1C }, /* RESERVED */
	{ 0x453C, 0x1C }, /* RESERVED */
	{ 0x453D, 0x1C }, /* RESERVED */
	{ 0x453E, 0x1C }, /* RESERVED */
	{ 0x453F, 0x1C }, /* RESERVED */
	{ 0x4540, 0x1C }, /* RESERVED */
	{ 0x4541, 0x03 }, /* RESERVED */
	{ 0x4542, 0x03 }, /* RESERVED */
	{ 0x4543, 0x03 }, /* RESERVED */
	{ 0x4544, 0x03 }, /* RESERVED */
	{ 0x4545, 0x03 }, /* RESERVED */
	{ 0x4546, 0x03 }, /* RESERVED */
	{ 0x4547, 0x03 }, /* RESERVED */
	{ 0x4548, 0x03 }, /* RESERVED */
	{ 0x4549, 0x03 }, /* RESERVED */
};

#define IMX662_INIT_SEQ_LEN (sizeof(g_imx662_init_seq) / sizeof(g_imx662_init_seq[0]))

#endif /* IMX662_CFG_H */
