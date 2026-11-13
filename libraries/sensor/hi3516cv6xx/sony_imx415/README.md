# Sony IMX415 for Hi3516CV610

A driver for the Sony IMX415 (Starvis, first generation, 8 MP), the default
sensor of the SIP-K662C6S module's stock firmware (xipc: `sensors=sns0=imx415`,
"Sensor Type Error Set Default IMX415"). The firmware links no MPP driver for
it, only its own scene layer and the bring-up sequence. This driver is built
from that sequence, the Hi3516CV500 IMX415 driver and the datasheet, against
this tree's sensor interface and `common/sensor_common.c`.

Modes: 3840x2160 all-pixel and 1920x1080 2/2-line binning, both linear RAW10
at 30 fps, VMAX 2250, HMAX 1100, four lanes at 891 Mbps, at the 37.125 MHz
sensor clock `sys_config` must feed an `imx415`. No WDR:
`cmos_isp_set_wdr_mode()` rejects every mode but linear.

## Where it comes from

- `imx415_cfg.h`: the power-on register tables, byte for byte from xipc
  (8M `sub_A9278`, 2M binning `sub_A9A8C`, both through the write helper
  `sub_A9170`, a 16-bit address / 8-bit data I2C write). The 2M table is the
  8M one with HADD/VADD (0x3020/0x3021 = 1), ADDMODE 0x3022 = 1 and
  0x30d9/0x30da. `imx415_sensor_ctrl.c` writes them in standby, then cancels
  standby, waits 24 ms and starts the master.
- `imx415_cmos.h`: the register map (SHR0 0x3050, GAIN 0x3090, VMAX 0x3024,
  REVERSE 0x3030), the exposure and gain model and the AWB calibration
  (reference 4983 K, R 440 / B 559), all from the Hi3516CV500 IMX415 driver.
- `imx415_cmos_param.h`: **not IMX415's**. The ISP default tables (CLUT, DPC,
  LSC, ACS, demosaic, sharpen, DRC, Bayer NR, anti-false-colour, CA, LDCI,
  gamma, dehaze, noise calibration, black level, colour matrix, AWB
  saturation) are the IMX335 ones of the same firmware (sensor type 261),
  recovered from xipc. They are not the tables of this tree's `sony_imx335`,
  which comes from the hi3516ev200 driver. No IMX415 tuning exists in the
  firmware's MPP layer. The DNG colour parameters are IMX335's as well. Each
  table is a byte image of its B051 structure, and a `_Static_assert` beside
  it refuses to compile if a structure stops matching the image.
  - The Bayer NR table was built by the vendor against an SDK without
    `load_ref_en` and `snr_version`. It is re-laid with both fields 0, as in
    `sony_imx662` (#296).
  - LSC stays off (`bit1_lsc = 0`), as the vendor ships every CV610 sensor.
    The IMX335 grid is 16x45 / 9x45, which fits no IMX415 mode (#299).

## Exposure and gain

SHR0 (0x3050) holds the exposure, counted back from the frame end, clamped
to [9, VMAX - 1]. Exposure is 2 to VMAX - 8 lines. An exposure as long as the
frame or longer gives the longest one the frame allows, not a wrapped
subtraction.

The gain register (0x3090) takes 0.3 dB steps. Analog and digital gain are
two indexes into one table, summed into it: analog up to index 100 (30 dB,
31.6x), sensor digital up to index 140 (42 dB). This is a first-generation
Starvis sensor, so there is no conversion gain register and no high
conversion gain step, unlike IMX662/664/675/678. The table is the 241-entry
0.3 dB ladder. The CV500 driver's entry 101 (41519, a typo) is corrected to
33519.

The AE registers (SHR0, GAIN, VMAX: 8 slots) are seeded before AE first runs:
VMAX with the mode's frame length and SHR0 with the shortest exposure. So the
first bring-up never writes a VMAX of 0.

## Black level

0x320 on every channel, in the CV610 ISP's 12-bit-normalised domain, like
the other CV610 Sony drivers. The CV500 driver's 0xC8 is the older ISP's
10-bit value. The vendor's own scene (`IMX415_Set_ISP_Black_Level`,
`sub_53C7E`) sets 0x334, so a profile may want that.

## Orientation

HREVERSE and VREVERSE (0x3030 bits 0 and 1) are written by
`pfn_mirror_flip`, and again by every bring-up in standby. Both are kept per
pipe under `cis->mutex` and cleared at unregister.

Inverted readout starts one pixel further on in each direction (datasheet,
"Normal Operation and Inverted Operation"), so the Bayer phase does not move,
in all-pixel and in 2/2 binning. The CFA is GBRG in every orientation. The
CV500 driver and mainline `imx415.c` (MEDIA_BUS_FMT_SGBRG10, whatever the
flip) agree. The OpenIPC CV500 profile reads the frame from (0, 0) as GBRG.
A CV610 profile has to do the same.

## Checked against

Mainline Linux `drivers/media/i2c/imx415.c`, as part of the STARVIS
cross-check for OpenIPC/firmware#2736. The 8M table was compared with
mainline's `imx415_init_table`, plus its 891 Mbps link table, plus its
37.125 MHz clock parameters.

- **Matches:** no value differs. INCKSEL1..7, SYS_MODE 0x3033 = 5,
  BCWAIT/CPWAIT and the MIPI D-PHY timings 0x4018..0x4028 are mainline's to the
  byte.
- **HMAX:** 1100 is exactly mainline's minimum for four lanes at 891 Mbps.
- **Ours only:** VMAX/HMAX, SHR0, 0x3034 and the 0x35ec..0x39c6 analog
  settings Sony's register setting lists for 10-bit output. Lane mode 0x4001 =
  3 (four lanes) is also ours only.
- **Mainline only:** XVSOUTSEL/XHSOUTSEL 0x30c0 = 0x22 and XVS_DRV/XHS_DRV
  0x30c1 = 0x00, which drive the sync pins. Left at the sensor's defaults
  (0x2A, 0x0F: HiZ), as the vendor does. Nothing on the MIPI path uses them.
- **Binning:** mainline has none, so the 2M table is checked only against the
  8M one it differs from.

## Not done

- Not run on an IMX415 yet. It builds with `-Wall -Wextra -Werror`, exports
  `g_sns_imx415_obj` and needs only libc.
- No IMX415 ISP tuning: see `imx415_cmos_param.h` above.
- `cis_read_reg` is a stub tree-wide, so the chip is not identified on the
  bus.
